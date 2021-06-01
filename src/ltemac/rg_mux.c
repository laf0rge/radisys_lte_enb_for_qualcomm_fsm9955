/********************************************************************16**

            (c) Copyright 2012 by RadiSys Corporation. All rights reserved.

     This software is confidential and proprietary to RadiSys Corporation.
     No part of this software may be reproduced, stored, transmitted, 
     disclosed or used in any form or by any means other than as expressly
     provided by the written Software License Agreement between Radisys 
     and its licensee.

     Radisys warrants that for a period, as provided by the written
     Software License Agreement between Radisys and its licensee, this
     software will perform substantially to Radisys specifications as
     published at the time of shipment, exclusive of any updates or 
     upgrades, and the media used for delivery of this software will be 
     free from defects in materials and workmanship.  Radisys also warrants 
     that has the corporate authority to enter into and perform under the 
     Software License Agreement and it is the copyright owner of the software 
     as originally delivered to its licensee.

     RADISYS MAKES NO OTHER WARRANTIES, EXPRESS OR IMPLIED, INCLUDING
     WITHOUT LIMITATION WARRANTIES OF MERCHANTABILITY OR FITNESS FOR
     A PARTICULAR PURPOSE WITH REGARD TO THIS SOFTWARE, SERVICE OR ANY RELATED
     MATERIALS.

     IN NO EVENT SHALL RADISYS BE LIABLE FOR ANY INDIRECT, SPECIAL,
     CONSEQUENTIAL DAMAGES, OR PUNITIVE DAMAGES IN CONNECTION WITH OR ARISING
     OUT OF THE USE OF, OR INABILITY TO USE, THIS SOFTWARE, WHETHER BASED
     ON BREACH OF CONTRACT, TORT (INCLUDING NEGLIGENCE), PRODUCT
     LIABILITY, OR OTHERWISE, AND WHETHER OR NOT IT HAS BEEN ADVISED
     OF THE POSSIBILITY OF SUCH DAMAGE.

                       Restricted Rights Legend:

     This software and all related materials licensed hereby are
     classified as "restricted computer software" as defined in clause
     52.227-19 of the Federal Acquisition Regulation ("FAR") and were
     developed entirely at private expense for nongovernmental purposes,
     are commercial in nature and have been regularly used for
     nongovernmental purposes, and, to the extent not published and
     copyrighted, are trade secrets and confidential and are provided
     with all rights reserved under the copyright laws of the United
     States.  The government's rights to the software and related
     materials are limited and restricted as provided in clause
     52.227-19 of the FAR.

                    IMPORTANT LIMITATION(S) ON USE

     The use of this software is limited to the use set
     forth in the written Software License Agreement between Radisys and
     its Licensee. Among other things, the use of this software
     may be limited to a particular type of Designated Equipment, as 
     defined in such Software License Agreement.
     Before any installation, use or transfer of this software, please
     consult the written Software License Agreement or contact Radisys at
     the location set forth below in order to confirm that you are
     engaging in a permissible use of the software.

                    RadiSys Corporation
                    Tel: +1 (858) 882 8800
                    Fax: +1 (858) 777 3388
                    Email: support@trillium.com
                    Web: http://www.radisys.com 
 
*********************************************************************17*/




/************************************************************************
 
     Name:     LTE-MAC layer
  
     Type:     C source file
  
     Desc:     C source code for Entry point fucntions
  
     File:     rg_mux.c 
  
     Sid:      gk_mux.c@@/main/3 - Sat Jul 30 02:21:31 2011
  
     Prg:      ps
  
**********************************************************************/

/** @file rg_mux.c
@brief MAC Multiplexing API.
*/

static const char* RLOG_MODULE_NAME="MAC";
static int RLOG_MODULE_ID=4096;
static int RLOG_FILE_ID=235;

/* header include files -- defines (.h) */
#include "envopt.h"        /* environment options */
#include "envdep.h"        /* environment dependent */
#include "envind.h"        /* environment independent */
  
#include "gen.h"           /* general */
#include "ssi.h"           /* system services */

#include "cm_tkns.h"       /* Common Token Defines */
#include "cm_llist.h"      /* Common Link List Defines */
#include "cm_hash.h"       /* Common Hash List Defines */
#include "cm_mblk.h"       /* memory management */
#include "cm_lte.h"        /* Common LTE Defines */

#include "rg_env.h"        /* MAC Environment Defines */
#include "tfu.h"           /* TFU Interface defines */
#include "crg.h"           /* CRG Interface defines */
#include "rg_sch_inf.h"           /* RGR Interface defines */
#include "rgu.h"           /* RGU Interface defines */
#include "lrg.h"           /* LRG Interface defines */

#include "rg_err.h"        /* MAC error defines */
#include "rg.h"            /* MAC defines */

/* header/extern include files (.x) */
#include "gen.x"           /* general */
#include "ssi.x"           /* system services */
#include "cm5.x"           /* system services */
#include "cm_tkns.x"       /* Common Token Definitions */
#include "cm_llist.x"      /* Common Link List Definitions */
#include "cm_lib.x"        /* Common Library Definitions */
#include "cm_hash.x"       /* Common Hash List Definitions */
#include "cm_mblk.x"       /* memory management */
#include "cm_lte.x"        /* Common LTE Definitions */

#include "tfu.x"           /* CRG Interface includes */
#include "crg.x"           /* CRG Interface includes */
#include "rg_sch_inf.x"           /* RGR Interface includes */
#include "rgu.x"           /* RGU Interface includes */
#include "lrg.x"           /* LRG Interface includes */

#include "rg.x"            /* MAC includes */
#include "ss_queue.h"
#include "ss_queue.x"
#include "ss_task.x"
#include "ss_msg.x"            /* MAC includes */
/* local defines */
#ifndef T2K_MEM_LEAK_DBG
#ifndef BC_CPUL_SHM_INST
EXTERN  S16 ssGetDBufOfSize(Region region,Size size,Buffer **dBuf);
#endif
#endif
#ifdef T2K_MEM_LEAK_DBG
char* file = __FILE__;
U32 line = __LINE__;
#endif

//PRIVATE U8 padArr[RG_MAX_PAD_ARR_SZ];
/* local typedefs */
typedef struct RgMUXSubHdr
{
   U8 shLen;  
   U8 shData[RG_MAX_SDU_SUB_HDR_LEN]; /* RRELCID + F-L (O) + L (O) */
} RgMUXSubHdr;

/* global variables */
U32 rgDlrate_rgu;

/* local externs */

PRIVATE Void rgMUXGet20bitRarGrnt ARGS((U8 ulBw,
                                        RgInfRarUlGrnt *msg3Grnt,
                                        U8 *grnt));
PRIVATE U16 rgMUXCalcRiv ARGS((U8 bw,
                                U8 rbStart,
                                U8 numRb));
 
#ifndef MS_MBUF_CORRUPTION
#define MS_BUF_ADD_ALLOC_CALLER()
#endif
/* forward references */

#define RG_PACK_CE_SHDR(_subHdr, _lcId, _eBit, _mBuf, _ret) {\
   _subHdr.shLen = RG_CE_SHDR_LEN;\
   _subHdr.shData[0] = (0x3F & ((_eBit << 5) | _lcId));\
   MS_BUF_ADD_ALLOC_CALLER(); \
   _ret = SAddPstMsgMult(&_subHdr.shData[0], _subHdr.shLen, _mBuf);\
}

#define RG_PACK_LAST_CE_SHDR(_subHdr, _lcId, _mBuf, _ret) {\
   _subHdr.shLen = RG_CE_SHDR_LEN;\
   _subHdr.shData[0] = (0x1F & _lcId);\
   MS_BUF_ADD_ALLOC_CALLER(); \
   _ret = SAddPstMsgMult(&_subHdr.shData[0], _subHdr.shLen, _mBuf);\
}

#define RG_PACK_CE(_ce, _len, _ceBuf, _ret) {\
   MS_BUF_ADD_ALLOC_CALLER(); \
   _ret = SAddPstMsgMult((U8 *)(&(_ce)), _len, _ceBuf);\
}

#define RG_MUX_CALC_LEN(_len,_lenBytes,_elmTotLen) {\
   U8 _hdrLen;\
   _lenBytes    = (_len <= 127) ? 1 : 2;\
   _hdrLen      = _lenBytes + RG_SDU_SHDR_LEN;\
   _elmTotLen   = _hdrLen + _len;\
}

#define RG_PACK_SDU_SHDR(_subHdr, _lcId, _len, _eBit, _mBuf, _ret) {\
   _ret = ROK;\
   _subHdr.shData[0] = (0x3F & ((_eBit << 5) | _lcId));\
   if(_len <= 127)\
   {\
      _subHdr.shLen = 2;\
      _subHdr.shData[1] = (0x7F & _len);\
      _subHdr.shData[2] = 0;\
   }\
   else\
   {\
      _subHdr.shLen = 3;\
      _subHdr.shData[1] = (0x80 | (0x7F & (_len >> 8)));\
      _subHdr.shData[2] = (0xFF & _len);\
   }\
   MS_BUF_ADD_ALLOC_CALLER(); \
   _ret = SAddPstMsgMult(&_subHdr.shData[0], _subHdr.shLen, _mBuf);\
}

#define RG_PACK_LAST_SDU_SHDR(_subHdr, _lcId, _mBuf, _ret) {\
   _ret = ROK;\
   _subHdr.shLen = 1;\
   _subHdr.shData[0] = (0x1F & _lcId);\
   MS_BUF_ADD_ALLOC_CALLER(); \
   _ret = SAddPstMsgMult(&_subHdr.shData[0], _subHdr.shLen, _mBuf);\
}

#define RG_PACK_SDU(_sdusBuf, _sduBuf, _ret) {\
   _ret = SCatMsg(_sdusBuf, _sduBuf, M1M2);\
}

#define RG_PACK_PAD_SHDR(_eBit, _mBuf, _ret) {\
   if(_eBit)\
   {\
      _ret = SAddPreMsg(0x3F, _mBuf);\
   }\
   else\
   {\
      _ret = SAddPstMsg(0x1F, _mBuf);\
   }\
}

#define RG_PACK_PAD(_padBuf,_size,_sduBuf) { \
  Buffer *sduEnd = NULLP; \
  SsMsgInfo *mInfo = NULLP; \
  mInfo = (SsMsgInfo *)_sduBuf->b_rptr; \
  ssGetDBufOfSize(mInfo->region,_size, &_padBuf); \
  if (_padBuf == NULLP) \
  { \
    RLOG0(L_ERROR, "RGERR_MUX_BLD_CEHDR_FAIL");\
    RETVALUE(RFAILED);\
  } \
  if (mInfo->endptr == NULLP) { \
    sduEnd = _sduBuf; \
    }\
  else \
  {  \
    sduEnd = mInfo->endptr; \
  } \
  sduEnd->b_cont = _padBuf; \
  padBuf->b_wptr += _size; \
  mInfo = (SsMsgInfo *)_sduBuf->b_rptr; \
  mInfo->endptr = _padBuf; \
  mInfo->len += _size; \
}

#define RG_PACK_RAR_SHDR(_byte, _mBuf, _ret) {\
   _ret = SAddPstMsg(_byte, _mBuf);\
}

#define RG_MAX_NUM_PAD_HDRS 2

/**
 * @brief Function to add ces along with subhdrs. 
 *
 * @details
 *
 *     Function : rgMUXAddCes
 *     
 *  @param[in] RgBldPduInfo   *pdu
 *  @param[in] Buffer        *mBuf 
 *  @param[in] Buffer        *ceBuf 
 *  @return    S16
 *      -# ROK 
 *      -# RFAILED 
 **/
#ifdef ANSI
PRIVATE S16 rgMUXAddCes
(
RgBldPduInfo   *pdu,
Buffer         *ceShdrBuf,
Buffer         *ceBuf,
RgErrInfo      *err
)
#else
PRIVATE S16 rgMUXAddCes(pdu, ceShdrBuf, ceBuf, err)
RgBldPduInfo   *pdu;
Buffer         *ceShdrBuf;
Buffer         *ceBuf;
RgErrInfo      *err;
#endif
{
   S16            ret;
   RgMUXSubHdr    subHdr;
   /* Moved up the initialization*/
   U8             ceSubHdr = RG_HDR_TYPE_CRES; 

   TRC2(rgMUXAddCes)

   switch(ceSubHdr)
   {
      case RG_HDR_TYPE_CRES:
      {
         if (NULLP != pdu->contResId)
         {
            if(pdu->schdTbSz >= RG_CRES_ELM_LEN)
            {
               if(pdu->schdTbSz > RG_CRES_ELM_LEN)
               {
                  RG_PACK_CE_SHDR(subHdr, RG_CRES_LCID_IDX, TRUE, ceShdrBuf, ret);
               }
               else
               {
                  RG_PACK_LAST_CE_SHDR(subHdr, RG_CRES_LCID_IDX, ceShdrBuf, ret);
               }

               if(ret != ROK)
               {
                  err->errCause = RGERR_MUX_BLD_CEHDR_FAIL;
                  RLOG0(L_ERROR, "RGERR_MUX_BLD_CEHDR_FAIL");
                  RETVALUE(RFAILED);
               }

               RG_PACK_CE(pdu->contResId->resId[0], RG_CRES_LEN, ceBuf, ret);

               if(ret != ROK)
               {
                  err->errCause = RGERR_MUX_BLD_CE_FAIL;
                  RETVALUE(RFAILED);
               }
               pdu->schdTbSz -= RG_CRES_ELM_LEN;
            }
         }
      }
      case RG_HDR_TYPE_TA:
      {  
         if (TRUE == pdu->ta.pres)
         {
            if(pdu->schdTbSz >= RG_TA_ELM_LEN)
            {
               U8 taVal; /* Moving from outer scope to available scope */
               if(pdu->schdTbSz > RG_TA_ELM_LEN)
               {
                  RG_PACK_CE_SHDR(subHdr, RG_TA_LCID_IDX, TRUE, ceShdrBuf, ret);
               }
               else
               {
                  RG_PACK_LAST_CE_SHDR(subHdr, RG_TA_LCID_IDX, ceShdrBuf, ret);
               }

               if(ret != ROK)
               {
                  err->errCause = RGERR_MUX_BLD_CEHDR_FAIL;
                  RLOG0(L_ERROR, "RGERR_MUX_BLD_CEHDR_FAIL");
                  RETVALUE(RFAILED);
               }

               taVal = pdu->ta.val;
               RG_PACK_CE(taVal, RG_TA_LEN, ceBuf, ret);

               if(ret != ROK)
               {
                  err->errCause = RGERR_MUX_BLD_CE_FAIL;
                  RLOG0(L_ERROR, "RGERR_MUX_BLD_CE_FAIL");
                  RETVALUE(RFAILED);
               }
               pdu->schdTbSz -= RG_TA_ELM_LEN;
               RLOG1(L_DEBUG,"*********** TA muxed by MAC: %u",
						pdu->ta.val);
            }
         }
         break;
      }
      default: 
         RETVALUE(RFAILED);
   }
   
   RETVALUE(ROK);
} /* rgMUXAddCes */

/**
 * @brief Function to insert SDU along with sub headers.
 *
 * @details
 *
 *     Function : rgMUXInsSdu
 *     
 *  @param[in]       RgBldPduInfo   *pdu
 *  @param[out]      Buffer        *mBuf 
 *  @param[out]      Buffer        *sduBuf 
 *  @return    S16
 *      -# ROK 
 *      -# RFAILED
 **/
#ifdef ANSI
PRIVATE S16 rgMUXInsSdu
(
MsgLen         *schdTbSz,
U8             lcId,
Buffer         *sdu,
Buffer         *sHdrBuf,
Buffer         *sduBuf,
RgErrInfo      *err
)
#else
PRIVATE S16 rgMUXInsSdu(schdTbSz, lcId, sdu, sHdrBuf, sduBuf, err)
MsgLen         *schdTbSz;
U8             lcId;
Buffer         *sdu;
Buffer         *sHdrBuf;
Buffer         *sduBuf;
RgErrInfo      *err;
#endif
{
   S16            ret;
   MsgLen         msgLen = 0;
   U8             lenBytes;
   MsgLen         elmTotLen;

   TRC2(rgMUXInsSdu)

   SFndLenMsg(sdu, &msgLen);

   RG_MUX_CALC_LEN(msgLen,lenBytes,elmTotLen);
   
   if (lcId == 3)
   {
     rgDlrate_rgu += msgLen; 
   }
   if (*schdTbSz > elmTotLen)
   {
      RgMUXSubHdr    subHdr;
      RG_PACK_SDU_SHDR(subHdr, lcId, msgLen, TRUE, sHdrBuf, ret);
      if(ret != ROK)
      {
         err->errCause = RGERR_MUX_BLD_SDUHDR_FAIL;
         RLOG1(L_ERROR, "RGERR_MUX_BLD_SDUHDR_FAIL for LCID:%d",lcId);
         RETVALUE(RFAILED);
      }

#ifndef L2_OPTMZ /* no need to pack as passing not muxing all LCs PDUs to 1*/
      RG_PACK_SDU(sduBuf, sdu, ret);
#else
  //UDAY
      ret = ROK;
      UNUSED(sduBuf);
#endif

      if(ret != ROK)
      {
         err->errCause = RGERR_MUX_BLD_SDU_FAIL;
         RLOG1(L_ERROR, "RGERR_MUX_BLD_SDU_FAIL for LCID:%d",lcId);
         RETVALUE(RFAILED);
      }

      *schdTbSz -= elmTotLen;
   }
   else if ((*schdTbSz <= elmTotLen) && 
                     (*schdTbSz >= (elmTotLen - lenBytes)))
   {
      RgMUXSubHdr    subHdr;
      if (*schdTbSz != (elmTotLen - lenBytes))
      {
         U16 numPdHdrs = *schdTbSz - (elmTotLen - lenBytes);
         if(numPdHdrs)
         {
            /* Packing multiple subheaders at one 
             * shot for optimization */   
            U8 sHdrArr[RG_MAX_NUM_PAD_HDRS] = {0x3F, 0x3F};
            if(numPdHdrs > RG_MAX_NUM_PAD_HDRS)
            {
               err->errCause = RGERR_MUX_BLD_PADHDR_FAIL;
               RLOG1(L_ERROR, "RGERR_MUX_BLD_PADHDR_FAIL for LCID:%d",lcId);
               RETVALUE(RFAILED);
            }
            ret = SAddPreMsgMult(&sHdrArr[0], numPdHdrs, sHdrBuf);
            if(ret != ROK)
            {
               err->errCause = RGERR_MUX_BLD_PADHDR_FAIL;
               RLOG1(L_ERROR, "RGERR_MUX_BLD_PADHDR_FAIL for LCID:%d",lcId);
               RETVALUE(RFAILED);
            }
            *schdTbSz-=numPdHdrs;
         } 
      }

      RG_PACK_LAST_SDU_SHDR(subHdr, lcId, sHdrBuf, ret);
      if(ret != ROK)
      {
         err->errCause = RGERR_MUX_BLD_SDUHDR_FAIL;
         RLOG1(L_ERROR, "RGERR_MUX_BLD_SDUHDR_FAIL for LCID:%d",lcId);
         RETVALUE(RFAILED);
      }

#ifndef L2_OPTMZ /* no need to pack as passing not muxing all LCs PDUs to 1*/
      RG_PACK_SDU(sduBuf, sdu, ret);
#else
      ret = ROK;
      UNUSED(sduBuf);
#endif

      if(ret != ROK)
      {
         err->errCause = RGERR_MUX_BLD_SDU_FAIL;
         RLOG1(L_ERROR, "RGERR_MUX_BLD_SDU_FAIL for LCID:%d",lcId);
         RETVALUE(RFAILED);
      }
        
      /*UE_MAC_FREE_BUF(sdu);*/

      *schdTbSz -= (msgLen + RG_SDU_SHDR_LEN);
   }
   else
   {
      /* This PDU can not be accodmodated at all */
      RLOG4(L_ERROR, "Failed lcId %u, elmTotLen %d lenBytes %d LCID:%d",
               lcId, ((S16)elmTotLen), lenBytes,lcId);
      RLOG3(L_ERROR, "msglen %d schdTbSz %d LCID:%d",
               ((S16)msgLen), ((S16)*schdTbSz),lcId);
      RETVALUE(RFAILED);
   }
   RETVALUE(ROK);
}

/**
 * @brief Function to insert SDU along with sub headers.
 *
 * @details
 *
 *     Function : rgMUXAddPadd
 *     
 *  @param[in]       RgBldPduInfo   *pdu
 *  @param[out]      Buffer        *mBuf 
 *  @param[out]      Buffer        *sduBuf 
 *  @return    S16
 *      -# ROK 
 *      -# RFAILED
 **/
#ifdef L2_OPTMZ
U32 padSize = 0;
#endif
#ifdef ANSI
PRIVATE S16 rgMUXAddPadd
(
MsgLen         *schdTbSz,
Buffer         *sHdrBuf,
Buffer         *sduBuf,
Bool           isRar,
RgErrInfo      *err
)
#else
PRIVATE S16 rgMUXAddPadd(schdTbSz, sHdrBuf, sduBuf, isRar, err)
MsgLen         *schdTbSz;
Buffer         *sHdrBuf;
Buffer         *sduBuf;
Bool           isRar;
RgErrInfo      *err;
#endif
{
   S16     ret = ROK;
   Buffer         *padBuf = NULLP;
   TRC2(rgMUXAddPadd)

#ifdef L2_OPTMZ
   padSize = 0;
#endif
   if(*schdTbSz)
   {
#ifndef L2_OPTMZ
      if(FALSE == isRar)
#else
      if((FALSE == isRar) && (NULL != sHdrBuf))
#endif

      {
         RG_PACK_PAD_SHDR(FALSE, sHdrBuf, ret);

         if(ret != ROK)
         {
            err->errCause = RGERR_MUX_BLD_PADHDR_FAIL;
            RLOG0(L_ERROR, "RGERR_MUX_BLD_PADHDR_FAIL");
            RETVALUE(RFAILED);
         }

         *schdTbSz -= 1;
      }

      if (*schdTbSz)
      {
         if (*schdTbSz <= RG_MAX_PAD_ARR_SZ)
         {
#ifndef L2_OPTMZ
            RG_PACK_PAD(padBuf,*schdTbSz,sduBuf);
#else
            if(sduBuf)
            {
               RG_PACK_PAD(padBuf,*schdTbSz,sduBuf);
            }
            else
               padSize += *schdTbSz;
#endif
            if(ret != ROK)
            {
               err->errCause = RGERR_MUX_BLD_PAD_FAIL;
               RLOG0(L_ERROR, "RGERR_MUX_BLD_PAD_FAIL");
               RETVALUE(RFAILED);
            }
            *schdTbSz = 0;
         }
         else
         {
            while (*schdTbSz)
            {
               if (*schdTbSz > RG_MAX_PAD_ARR_SZ)
               {
#ifndef L2_OPTMZ
                  RG_PACK_PAD(padBuf,RG_MAX_PAD_ARR_SZ,sduBuf);
#else
                  if(sduBuf)
                  {
                     RG_PACK_PAD(padBuf,RG_MAX_PAD_ARR_SZ,sduBuf);
                  }
                  else
                     padSize += RG_MAX_PAD_ARR_SZ;
#endif

                  if(ret != ROK)
                  {
                     err->errCause = RGERR_MUX_BLD_PAD_FAIL;
                     RLOG0(L_ERROR, "RGERR_MUX_BLD_PAD_FAIL");
                     RETVALUE(RFAILED);
                  }

                  *schdTbSz -= RG_MAX_PAD_ARR_SZ;
               }
               else
               {
#ifndef L2_OPTMZ
                  RG_PACK_PAD(padBuf,*schdTbSz,sduBuf);
#else
                  if(sduBuf)
                  {
                     RG_PACK_PAD(padBuf,*schdTbSz,sduBuf);
                  }
                  else
                     padSize += *schdTbSz;
#endif

                  if(ret != ROK)
                  {
                     err->errCause = RGERR_MUX_BLD_PAD_FAIL;
                     RLOG0(L_ERROR, "RGERR_MUX_BLD_PAD_FAIL");
                     RETVALUE(RFAILED);

                  }
                  *schdTbSz = 0;
               }
            }
         }
      }
   }

   RETVALUE(ROK);
} /* rgMUXAddPadd */

#ifndef L2_OPTMZ
/**
 * @brief Function to add SDU along with sub headers.
 *
 * @details
 *
 *     Function : rgMUXAddSdus
 *     
 *  @param[in]       RgBldPduInfo   *pdu
 *  @param[out]      Buffer        *mBuf 
 *  @param[out]      Buffer        *sduBuf 
 *  @return    S16
 *      -# ROK 
 *      -# RFAILED
 **/
#ifdef ANSI
PRIVATE S16 rgMUXAddSdus
(
RgBldPduInfo   *pdu,
Buffer         *sHdrBuf,
Buffer         *sduBuf,
RgErrInfo      *err
)
#else
PRIVATE S16 rgMUXAddSdus(pdu, sHdrBuf, sduBuf, err)
RgBldPduInfo   *pdu;
Buffer         *sHdrBuf;
Buffer         *sduBuf;
RgErrInfo      *err;
#endif
{
   RgRguDDatReqPerUe *dDatReq;
   RgRguCmnDatReq *cDatReq;

   TRC2(rgMUXAddSdus)

   switch(pdu->reqType)
   {
      case EVTRGUCDATREQ:
         cDatReq = (RgRguCmnDatReq *)(pdu->datReq);
         /* Add sdu(s) to the Message Buffer */
         if (NULLP != cDatReq)
         {
            if(rgMUXInsSdu(&pdu->schdTbSz, 
                     RG_CCCH_LCID, cDatReq->pdu, 
                     sHdrBuf, sduBuf, err) != ROK)
            {
               RETVALUE(RFAILED);
            }
            RG_FREE_MSG(cDatReq->pdu);
         }
         break;

      case EVTRGUDDATREQ:
         dDatReq = (RgRguDDatReqPerUe *)(pdu->datReq);
         /* Add sdu(s) to the Message Buffer */
         if (NULLP != dDatReq)
         {
            if(pdu->tbIndex == 1)
            {
               U16 idx1, idx2;
               /* Adding this temporary variable for optimization */
               RguDatReqTb *datReqTb = &dDatReq->datReqTb[0];

               for (idx1=0; (idx1 < datReqTb->nmbLch); idx1++)
               {
                  for(idx2=0;
                     (idx2 < datReqTb->lchData[idx1].pdu.numPdu);
                     idx2++)
                  {
                     if(pdu->schdTbSz)
                     {
                        if(rgMUXInsSdu(&pdu->schdTbSz,
                           datReqTb->lchData[idx1].lcId, 
                           datReqTb->lchData[idx1].pdu.mBuf[idx2],
                           sHdrBuf, sduBuf, err) != ROK)
                        {
                           RLOG1(L_ERROR, "FAILED for LCID:%d",datReqTb->lchData[idx1].lcId);
                           RETVALUE(RFAILED);
                        }
                     }
                     RG_FREE_MSG(datReqTb->lchData[idx1].pdu.mBuf[idx2]);
                  }
               }
            }
            else if(pdu->tbIndex == 2)
            {
               U16 idx1, idx2;
               RguDatReqTb *datReqTb = &dDatReq->datReqTb[1];
               for (idx1=0; (idx1 < datReqTb->nmbLch); idx1++)
               {
                  for(idx2=0;
                     (idx2 < datReqTb->lchData[idx1].pdu.numPdu);
                     idx2++)
                  {
                     if(pdu->schdTbSz)
                     {
                        if(rgMUXInsSdu(&pdu->schdTbSz,
                           datReqTb->lchData[idx1].lcId, 
                           datReqTb->lchData[idx1].pdu.mBuf[idx2],
                           sHdrBuf, sduBuf, err) != ROK)
                        {
                           RLOG2(L_ERROR, "FAILED TB Size %d LCID:%d",
                                    ((S16)pdu->schdTbSz),datReqTb->lchData[idx1].lcId);
                           RETVALUE(RFAILED);
                        }
                     }
                     RG_FREE_MSG(datReqTb->lchData[idx1].pdu.mBuf[idx2]);
                  }
               }
            }
         }
         break;

      case EVTTFUTTIIND:
         break;
      default:
         break;
   } /* End of switch(reqType) */
   if(rgMUXAddPadd(&pdu->schdTbSz, sHdrBuf, sduBuf, FALSE, err) != ROK)
   {
      RLOG1(L_ERROR, "FAILED for TB Size:%d",(S16)pdu->schdTbSz);
      RETVALUE(RFAILED);
   }
   RETVALUE(ROK);
}

/**
 * @brief Function to create MAC PDU from RLC SDUs and control elements, if any. 
 *
 * @details
 *
 *     Function : rgMUXBldPdu
 *     
 *     -# This function shall be invoked by Downlink Harq Module as soon as a
 *        Data request is received from RLC for a UE along with its stored 
 *        control elements to create a MAC PDU.
 *     -# It shall create subheaders for the control elements (timing advance
 *        and contention resolution ID), if given, and then shall run through
 *        all the logical channels and create subheader for each of the SDUs
 *        given on that logical channel.
 *     -# It will concatenate the created subheaders, control elements and 
 *        the SDUs to form the entire MAC PDU.
 *     -# It shall invoke rgMUXPadPdu if the total length of the created 
 *        buffer is less than the scheduled TB size. 
 *     
 *           
 *  @param[in]  RgBldPduInfo   *bldPdu
 *  @param[in]  Buffer         **txPdu
 *  @param[out] RgErrInfo      *err
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED 
 **/
#ifdef ANSI
PUBLIC S16 rgMUXBldPdu
(
RgBldPduInfo   *pdu,
Buffer         **txPdu,
RgErrInfo      *err
)
#else
PUBLIC S16 rgMUXBldPdu(pdu, txPdu, err)
RgBldPduInfo   *pdu;
Buffer         **txPdu;
RgErrInfo      *err;
#endif
{
   Buffer         *mBuf1=NULLP;
   Buffer         *mBuf2=NULLP;

   TRC2(rgMUXBldPdu)

 
   if (rgGetMsg(&mBuf1) != ROK)
   {
      /* Buffer couldnt get allocated. Return a failure */
      err->errCause = RGERR_MUX_MEM_ALLOC_FAIL;
      err->errType = RGERR_MUX_BLD_PDU;
      RLOG0(L_ERROR, "FAILED to get Msg");
      RETVALUE(RFAILED);
   }

   if (rgGetMsg(&mBuf2) != ROK)
   {
      /* Buffer couldnt get allocated. Return a failure */
      err->errCause = RGERR_MUX_MEM_ALLOC_FAIL;
      err->errType = RGERR_MUX_BLD_PDU;
      RLOG0(L_ERROR, "FAILED to get Msg");
      RG_FREE_MSG(mBuf1); 
      RETVALUE(RFAILED);
   }

   if(rgMUXAddCes(pdu, mBuf1, mBuf2, err) != ROK)
   {
      RG_FREE_MSG(mBuf1);
      RG_FREE_MSG(mBuf2);
      err->errType = RGERR_MUX_BLD_PDU;
      RLOG0(L_ERROR, "FAILED to get Msg");
      RETVALUE(RFAILED);
   }

   if(rgMUXAddSdus(pdu, mBuf1, mBuf2, err) != ROK)
   {
      RG_FREE_MSG(mBuf1);
      RG_FREE_MSG(mBuf2);
      err->errType = RGERR_MUX_BLD_PDU;
      RLOG0(L_ERROR, "FAILED to mux add sdu");
      RETVALUE(RFAILED);
   }

   if(SCatMsg(mBuf1, mBuf2, M1M2) != ROK)
   {
      RG_FREE_MSG(mBuf1);
      RG_FREE_MSG(mBuf2);
      err->errType = RGERR_MUX_BLD_PDU;
      RLOG0(L_ERROR, "FAILED to catMsg");
      RETVALUE(RFAILED);
   }
   RG_FREE_MSG(mBuf2); 

   *txPdu = mBuf1;

   RETVALUE(ROK);

}  /* rgMUXBldPdu */

#else /* else of ifndef L2_OPTMZ */

/**
 * @brief Function to add SDU along with sub headers.
 *
 * @details
 *
 *     Function : rgMUXAddSdus
 *     
 *  @param[in]       RgBldPduInfo   *pdu
 *  @param[out]      Buffer        *mBuf 
 *  @param[out]      Buffer        *sduBuf 
 *  @return    S16
 *      -# ROK 
 *      -# RFAILED
 **/
#ifdef ANSI
PRIVATE S16 rgMUXAddSdus
(
RgBldPduInfo        *pdu,
Buffer              *sHdrBuf,
RgTfuDatReqTbInfo   *tb,
RgErrInfo           *err
)
#else
PRIVATE S16 rgMUXAddSdus(pdu, sHdrBuf, tb, err)
RgBldPduInfo        *pdu;
Buffer              *sHdrBuf;
RgTfuDatReqTbInfo   *tb;
RgErrInfo           *err;
#endif
{
   RgRguDDatReqPerUe  *dDatReq;
   RgRguCmnDatReq     *cDatReq;
   U32 lchIdx, pduIdx;

   TRC2(rgMUXAddSdus)

// printf("Received Type = %d \n", pdu->reqType);
   switch(pdu->reqType)
   {
      case EVTRGUCDATREQ:
         cDatReq = (RgRguCmnDatReq *)(pdu->datReq);
         /* Add sdu(s) to the Message Buffer */
         if (NULLP != cDatReq)
         {
            if(rgMUXInsSdu(&pdu->schdTbSz, 
                     RG_CCCH_LCID, cDatReq->pdu, 
                     sHdrBuf, NULLP, err) != ROK)
            {
               RETVALUE(RFAILED);
            }
            /* L2 Optimization for mUe/Tti: RLC pdu mbuf pointer will be passed
             * to CL it is stored in DlHqProc->TbInfo and it will be used in
             * case of harq retransmission. Store CCCH data at 0th index of
             * lch array*/
            tb->lchInfo[tb->numLch].mBuf[(tb->lchInfo[tb->numLch].numPdu)]\
               = cDatReq->pdu;
            tb->lchInfo[tb->numLch].numPdu++;
            tb->numLch++;
           // printf("MSG4 is muxed  numLch=%ld numPdu=%ld tbaddr =%p", tb->numLch,tb->lchInfo[tb->numLch-1].numPdu, (Void*)tb);
         }
         break;

      case EVTRGUDDATREQ:
         dDatReq = (RgRguDDatReqPerUe *)(pdu->datReq);
         /* Add sdu(s) to the Message Buffer */
         if (NULLP != dDatReq)
         {
            if(pdu->tbIndex == 1)
            {
               U16 idx1, idx2;
               /* Adding this temporary variable for optimization */
               RguDatReqTb *datReqTb = &dDatReq->datReqTb[0];
             
               tb->numLch = lchIdx = 0;

            //   printf("11111Received Type = %d %d \n", pdu->reqType, pdu->schdTbSz);
    //           prc_trace_format_string(0x40,3,": AddSdus: numOfLch=%d numOfPdu=%d, schdSz=%d ", datReqTb->nmbLch, datReqTb->lchData[0].pdu.numPdu, pdu->schdTbSz);
               //printf("\nAddSdus: numOfLch=%d numOfPdu=%d, schdSz=%d \n", datReqTb->nmbLch, datReqTb->lchData[0].pdu.numPdu, pdu->schdTbSz);
               for (idx1=0; (idx1 < datReqTb->nmbLch); idx1++)
               {
                  tb->lchInfo[lchIdx].numPdu = pduIdx = 0;

                  for(idx2=0;
                     (idx2 < datReqTb->lchData[idx1].pdu.numPdu);
                     idx2++)
                  {
                     if(pdu->schdTbSz)
                     {
                        if(rgMUXInsSdu(&pdu->schdTbSz,
                           datReqTb->lchData[idx1].lcId, 
                           datReqTb->lchData[idx1].pdu.mBuf[idx2],
                           sHdrBuf, NULLP, err) != ROK)
                        {
                           RGDBGERRNEW((rgPBuf, "FAILED\n"));
                           RETVALUE(RFAILED);
                        }

                        /* L2 Optimization for mUe/Tti:Increment numPdu by 1
                         * Store pdu buffer in tb to send it to CL/PHY. Increment
                         * numPdu by 1*/
                        tb->lchInfo[lchIdx].mBuf[pduIdx] = datReqTb->lchData[idx1].pdu.mBuf[idx2];
//UDAY 
#if 1
                        if(NULL != datReqTb->lchData[idx1].pdu.mBuf[idx2]->b_cont)
                        {
                             Buffer *tmp;
                             tmp = datReqTb->lchData[idx1].pdu.mBuf[idx2]->b_cont;
                             if(NULL == tmp->b_rptr)
                             {
                                printf("11111Its Null here only \n");
                             }
                        }
                        else
                        {
                            printf("222222Its Null here only \n");
                        }
#endif
                        pduIdx++;
                        //tb->lchInfo[tb->numLch].numPdu++;
                        //tb->numLch++;

                    }
                  }
                  
                  if(pduIdx)
                  {
                    tb->lchInfo[lchIdx].numPdu = pduIdx;
                    lchIdx++;
                  }
               }
               tb->numLch = lchIdx;
            }
            else if(pdu->tbIndex == 2)
            {
               U16 idx1, idx2;
               RguDatReqTb *datReqTb = &dDatReq->datReqTb[1];
               tb->numLch = lchIdx = 0;
//               printf("2222Received Type = %d %d \n", pdu->reqType, pdu->schdTbSz);
               //printf("\nAddSdus: numOfLch=%d numOfPdu=%d, schdSz=%d \n", datReqTb->nmbLch, datReqTb->lchData[0].pdu.numPdu, pdu->schdTbSz);
         //      prc_trace_format_string(0x40,3,": AddSdus: numOfLch=%d numOfPdu=%d, schdSz=%d", datReqTb->nmbLch, datReqTb->lchData[0].pdu.numPdu, pdu->schdTbSz);
              for (idx1=0; (idx1 < datReqTb->nmbLch); idx1++)
               {
                 tb->lchInfo[lchIdx].numPdu = pduIdx = 0;
                 for(idx2=0;
                     (idx2 < datReqTb->lchData[idx1].pdu.numPdu);
                     idx2++)
                  {
                     if(pdu->schdTbSz)
                     {
                        if(rgMUXInsSdu(&pdu->schdTbSz,
                           datReqTb->lchData[idx1].lcId, 
                           datReqTb->lchData[idx1].pdu.mBuf[idx2],
                           sHdrBuf, NULLP, err) != ROK)
                        {
                           RGDBGERRNEW((rgPBuf, "FAILED TB Size %d\n",
                                    ((S16)pdu->schdTbSz)));
                           RETVALUE(RFAILED);
                        }
                         /* L2 Optimization for mUe/Tti:Increment numPdu by 1
                         * Store pdu buffer in tb to send it to CL/PHY. Increment
                         * numPdu by 1*/
                        tb->lchInfo[lchIdx].mBuf[pduIdx] = datReqTb->lchData[idx1].pdu.mBuf[idx2];
#if 1
                        if(NULL != datReqTb->lchData[idx1].pdu.mBuf[idx2]->b_cont)
                        {
                             Buffer *tmp;
                             tmp = datReqTb->lchData[idx1].pdu.mBuf[idx2]->b_cont;
                             if(NULL == tmp->b_rptr)
                             {
                                printf("2212121Its Null here only \n");
                             }
                        }
                        else
                        {
                            printf("343343433ts Null here only \n");
                        }
#endif
                        pduIdx++;
                       // tb->lchInfo[tb->numLch].numPdu++;
                       // tb->numLch++;

                     }
                  }

                  if(pduIdx)
                  {
                    tb->lchInfo[lchIdx].numPdu = pduIdx;
                    lchIdx++;
                  }
               }

               tb->numLch = lchIdx;
           }
         }
         break;

      case EVTTFUTTIIND:
         break;
      default:
         break;
   } /* End of switch(reqType) */
#if 0
#else

   
   if(rgMUXAddPadd(&pdu->schdTbSz, sHdrBuf, NULLP, FALSE, err) != ROK)
   {
      RGDBGERRNEW((rgPBuf, "FAILED"));
      RETVALUE(RFAILED);
   }
   tb->padSize = padSize;
#endif

   RETVALUE(ROK);
}

/**
 * @brief Function to create MAC PDU from RLC SDUs and control elements, if any. 
 *
 * @details
 *
 *     Function : rgMUXBldPdu
 *     -# This function shall be invoked by Downlink Harq Module as soon as a
 *        Data request is received from RLC for a UE along with its stored 
 *        control elements to create a MAC PDU.
 *     -# It shall create subheaders for the control elements (timing advance
 *        and contention resolution ID), if given, and then shall run through
 *        all the logical channels and create subheader for each of the SDUs
 *        given on that logical channel.
 *     -# L2 Optimization for mUe/Tti: Avoiding muxing to reduce overhead of 
 *           additional Mbuf allocation memory related operation.
       -# MAC header, MAC CEs, MAC PDUs and MAC padding are stored in pre-
            allocated mBufs. These pointers will not be freed by CL    
 *     -# It shall invoke rgMUXPadPdu if the total length of the created 
 *        buffer is less than the scheduled TB size.
 *     -# At successfull operation of this function tb->macHdr, will have
 *        complete MAC Header. tb->macCes will have MAC CEs if any. tb->
 *        lchInfo[idx].mBuf[idx] will have MAC SDU per LCH per TB per UE 
 *     
 *           
 *  @param[in]  RgBldPduInfo   *bldPdu
 *  @param[out] RgTbInfo       *tb
 *  @param[out] RgErrInfo      *err
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED 
 **/
#ifdef ANSI
PUBLIC S16 rgMUXBldPdu
(
RgBldPduInfo       *pdu,
RgTfuDatReqTbInfo  *tb,
RgErrInfo          *err
)
#else
PUBLIC S16 rgMUXBldPdu(pdu, tb, err)
RgBldPduInfo       *pdu;
RgTfuDatReqTbInfo  *tb;
RgErrInfo          *err;
#endif
{
   Buffer         *mBuf1; /* MAC hearder */
   Buffer         *mBuf2; /* MAC CEs */
   //U32            lchIdx, pduIdx;

   TRC2(rgMUXBldPdu)
   
  /* Reseting macHdr and macCes pointers */
  if(tb->macHdr)
   SResetMBuf(tb->macHdr);
   if(tb->macCes)
   SResetMBuf(tb->macCes);
   
   mBuf1 = tb->macHdr; /* MAC hearder */
   mBuf2 = tb->macCes; /* MAC CEs */
   tb->tbSize  = pdu->schdTbSz;

   if(rgMUXAddCes(pdu, mBuf1, mBuf2, err) != ROK)
   {
      /* Reset rPtr and wPtr to the base of data buffer(db_base)*/
      printf("\n rgMUXBldPdu: rgMUXAddCes is Failed \n");
      RG_FREE_TB(tb);
      err->errType = RGERR_MUX_BLD_PDU;
      RGDBGERRNEW((rgPBuf, "FAILED"));
      RETVALUE(RFAILED);
   }
   if(rgMUXAddSdus(pdu, mBuf1, tb, err) != ROK)
   {
      /*TODO:MP Reset rPtr and wPtr to the base of data buffer(db_base)
       * Reset numLch and numPdu to zero and set MAC SDU buf to NULLP */
      printf("\n rgMUXBldPdu: rgMUXAddSdus is Failed \n");
      RG_FREE_TB(tb);

      err->errType = RGERR_MUX_BLD_PDU;
      RGDBGERRNEW((rgPBuf, "FAILED"));
      RETVALUE(RFAILED);
   }
// UDAY 
//    printf("Mac sub header at MAC : \n");
//      SPrntMsg(tb->macHdr, 0, 0);
//   prc_trace_format_string(0x40,3,": padSize=%ld", tb->padSize);

   tb->tbPres = TRUE;
   RETVALUE(ROK);

}  /* rgMUXBldPdu */

#endif /* end of L2_OPTMZ */

/**
 * @brief Function to create RAR PDU. 
 *
 * @details
 *
 *     Function : rgMUXBldRarPdu
 *                This function is used to build RAR PDUs and is being 
 *                invoked by the scheduler.
 *     
 *  @param[out]RgCellCb       *cellCb
 *  @param[in] RgRaRspAlloc   *bldPdu
 *  @param[in] Buffer         **txPdu 
 *  @param[out] RgErrInfo     *err
 *  @return    S16
 *      -# ROK 
 *      -# RFAILED 
 **/
#ifdef ANSI
PUBLIC S16 rgMUXBldRarPdu
(
RgCellCb        *cell,
RgInfRaRntiInfo *alloc,
Buffer          **txPdu,
RgErrInfo       *err
)
#else
PUBLIC S16 rgMUXBldRarPdu(cell, alloc, txPdu, err)
RgCellCb        *cell;
RgInfRaRntiInfo *alloc;
Buffer          **txPdu;
RgErrInfo       *err;
#endif
{
   Buffer      *hdrBuf = NULLP;
   Buffer      *datBuf = NULLP;
   S16         ret; 
   U8          data[RG_RAR_ELEM_LEN];
   U8          hdrByte;
   TknU8       phdrByte;
   MsgLen      schdTbSz;
   U8          idx;

   TRC2(rgMUXBldRarPdu)

   schdTbSz = alloc->schdTbSz;
   /* RAR PDU Requirements */
   /*
   1. SubHeader - E/T/RAPID.
   2. TA ( if applicable)
   3. Ul Grant:
         a. Hopping Flag - 1 Bit.
         b. Fixed Size RB Assignment. - 10 Bits.
         c. Truncated Modulation and coding scheme - 4 Bits.
         d. TPC command for scheduled PUSCH. - 3 Bits.
         e. UL Delay - 1 Bit.
         f. CQI Request - 1 Bit.
   4. Temporary Crnti.
   */

   /* Initialize the error type */
   err->errType = RGERR_MUX_BLD_RAR_PDU;
  
   if ((ret = rgGetMsg(&hdrBuf)) != ROK)
   {
      /* Buffer couldnt get allocated. Return a failure */
      err->errCause = RGERR_MUX_MEM_ALLOC_FAIL;
      RLOG_ARG0(L_ERROR,DBG_CELLID,cell->cellId, "FAILED to getMsg");
      RETVALUE(RFAILED);
   }
   if ((ret = rgGetMsg(&datBuf)) != ROK)
   {
      /* Buffer couldnt get allocated. Return a failure */
      err->errCause = RGERR_MUX_MEM_ALLOC_FAIL;
      RG_FREE_MSG(hdrBuf); 
      RLOG_ARG0(L_ERROR,DBG_CELLID,cell->cellId, "FAILED to getMsg");
      RETVALUE(RFAILED);
   }

   if (TRUE == alloc->backOffInd.pres)
   {
      /*Set E and T*/
      if((schdTbSz > (RG_RAR_ELEM_LEN+RG_RAR_SHDR_LEN)) 
                           && (alloc->numCrnti))
      {
         hdrByte = 0x80;
      }
      else
      {
         hdrByte = 0x00;
      }
      /* Add the bi */
      hdrByte |= (0x0F & (alloc->backOffInd.val));

      /* Add the header */
      RG_PACK_RAR_SHDR(hdrByte, hdrBuf, ret);
      if(ret != ROK)
      {
         err->errCause = RGERR_MUX_BLD_BI_FAIL;
         RLOG_ARG0(L_ERROR,DBG_CELLID,cell->cellId,"RGERR_MUX_BLD_BI_FAIL");
         RG_FREE_MSG(hdrBuf);
         RG_FREE_MSG(datBuf);
         RETVALUE(RFAILED);
      }
      schdTbSz--;
   }

   phdrByte.pres = FALSE;
   phdrByte.val = 0;

   for (idx=0; idx < (alloc->numCrnti) && 
         (schdTbSz >= RG_RAR_ELEM_LEN+RG_RAR_SHDR_LEN); idx++)
   {
      /* Add the tBit */
      hdrByte = 0x40; 
      /* Add the rapId */
      hdrByte |= (0x3F & (alloc->crntiInfo[idx].rapId));

      if(phdrByte.pres == TRUE)
      {
         /* Add the E bit*/
         phdrByte.val |= 0x80;
         /* Add the header */
         RG_PACK_RAR_SHDR(phdrByte.val, hdrBuf, ret);
         if(ret != ROK)
         {
            err->errCause = RGERR_MUX_BLD_RAPIDHDR_FAIL;
            RLOG_ARG0(L_ERROR,DBG_CELLID,cell->cellId,"RGERR_MUX_BLD_RAPIDHDR_FAIL");
            RG_FREE_MSG(hdrBuf);
            RG_FREE_MSG(datBuf);
            RETVALUE(RFAILED);
         }
      }

      phdrByte.pres = TRUE;
      phdrByte.val = hdrByte;
      /* Prepare the data */
      data[0]  =  0x7F & ((alloc->crntiInfo[idx].ta.val) >> 4);
      rgMUXGet20bitRarGrnt(cell->bwCfg.ulTotalBw, &(alloc->crntiInfo[idx].grnt), &data[1]);
      data[1] |=  ((U8)((alloc->crntiInfo[idx].ta.val) << 4));
      data[4]  =  (alloc->crntiInfo[idx].tmpCrnti) >> 8;
      data[5]  =  (U8) (alloc->crntiInfo[idx].tmpCrnti);

      RLOG_ARG2(L_DEBUG,DBG_CELLID,cell->cellId,
         		"Rar,Rapid=%d, Temp CRNTI:%d", 
                alloc->crntiInfo[idx].rapId,
                alloc->crntiInfo[idx].tmpCrnti);

     MS_BUF_ADD_ALLOC_CALLER();
      if(SAddPstMsgMult(&data[0], RG_RAR_ELEM_LEN, datBuf) != ROK)
      {
         err->errCause = RGERR_MUX_BLD_RAPID_FAIL;
         RLOG_ARG0(L_ERROR,DBG_CELLID,cell->cellId,"RGERR_MUX_BLD_RAPID_FAIL");
         RG_FREE_MSG(hdrBuf);
         RG_FREE_MSG(datBuf);
         RETVALUE(RFAILED);
      }
      schdTbSz -= RG_RAR_ELEM_LEN+RG_RAR_SHDR_LEN;
   }

   if(phdrByte.pres == TRUE)
   {
      /* Add the header */
      RG_PACK_RAR_SHDR(phdrByte.val, hdrBuf, ret);
      if(ret != ROK)
      {
         err->errCause = RGERR_MUX_BLD_RAPIDHDR_FAIL;
         RLOG_ARG0(L_ERROR,DBG_CELLID,cell->cellId,"RGERR_MUX_BLD_RAPIDHDR_FAIL");
         RG_FREE_MSG(hdrBuf);
         RG_FREE_MSG(datBuf);
         RETVALUE(RFAILED);
      }
   }

   if(rgMUXAddPadd(&schdTbSz, hdrBuf, datBuf, TRUE, err) != ROK)
   {
      RG_FREE_MSG(hdrBuf);
      RG_FREE_MSG(datBuf);
      RLOG_ARG0(L_ERROR,DBG_CELLID,cell->cellId,"FAILED to mux add padding");
      RETVALUE(RFAILED);
   }

   /* Header and data Bufs have been created. Concatenate them. */
   if(SCatMsg(hdrBuf, datBuf, M1M2) != ROK)
   {
      RG_FREE_MSG(hdrBuf);
      RG_FREE_MSG(datBuf);
      RLOG_ARG0(L_ERROR,DBG_CELLID,cell->cellId,"FAILED to catMsg");
      RETVALUE(RFAILED);
   }
   RG_FREE_MSG(datBuf);

   *txPdu = hdrBuf;

   RETVALUE(ROK);
} /* rgMUXBldRarPdu */

/***********************************************************
 *
 *     Func : rgMUXGet20bitRarGrnt
 *
 *     Desc : This function fills up the 20-bit grant
 *            for RA response.
 *
 *     Ret  : None.
 *
 *     Notes: None.
 *
 *     File : rg_mux.c
 *
 **********************************************************/
#ifdef ANSI
PRIVATE Void rgMUXGet20bitRarGrnt
(
U8             ulBw,
RgInfRarUlGrnt *msg3Grnt,
U8             *grnt
)
#else
PRIVATE Void rgMUXGet20bitRarGrnt(ulBw, msg3Grnt, grnt)
U8             ulBw;
RgInfRarUlGrnt *msg3Grnt;
U8             *grnt;
#endif
{
   U16       riv = rgMUXCalcRiv(ulBw, msg3Grnt->rbStart, msg3Grnt->numRb);

   TRC2(rgMUXGet20bitRarGrnt);

   grnt[2]  = msg3Grnt->cqiBit;   /* cqi bit is 0, output from sched */
   grnt[2] |= (msg3Grnt->delayBit << 1);
   grnt[2] |= (msg3Grnt->tpc << 2);
   grnt[2] |= (msg3Grnt->iMcsCrnt << 5);

   grnt[1]  = (msg3Grnt->iMcsCrnt >> 3);
   /* Forcing right shift to insert 0 as the LSB: 
    * since this is assumed in the computation */
   grnt[1] |= (U8)((riv << 1) & 0xFE);

   grnt[0]  = (U8)((riv >> 7) & 0x07);
   grnt[0] |= ((msg3Grnt->hop & 0x01) << 3);

   RETVOID;
} /* rgMUXGet20bitRarGrnt */

/***********************************************************
 *
 *     Func : rgMUXCalcRiv
 *
 *     Desc : This function calculates RIV.
 *
 *     Ret  : None.
 *
 *     Notes: None.
 *
 *     File : rg_mux.c
 *
 **********************************************************/
#ifdef ANSI
PRIVATE U16 rgMUXCalcRiv
(
U8           bw,
U8           rbStart,
U8           numRb
)
#else
PRIVATE U16 rgMUXCalcRiv(bw, rbStart, numRb)
U8           bw;
U8           rbStart;
U8           numRb;
#endif
{
   U8           numRbMinus1 = numRb - 1;
   U16          riv;

   TRC2(rgMUXCalcRiv);

   if (numRbMinus1 <= bw/2)
   {
      riv = bw * numRbMinus1 + rbStart;
   }
   else
   {
      riv = bw * (bw - numRbMinus1) + (bw - rbStart - 1);
   }
   RETVALUE(riv);
} /* rgMUXCalcRiv */



/**********************************************************************
 
         End of file:     gk_mux.c@@/main/3 - Sat Jul 30 02:21:31 2011
 
**********************************************************************/
 
/**********************************************************************
 
        Notes:
 
**********************************************************************/
 
/**********************************************************************
 
        Revision history:
 
**********************************************************************/
/********************************************************************90**
 
     ver       pat    init                  description
------------ -------- ---- ----------------------------------------------
/main/1      ---     ps                  1. Initial Release.
/main/2      ---     ps                  1. LTE MAC 2.1 release
           rg003.201 ns                  1. Trace added for functions.
           rg005.201 sm                  1. Changes for optimation.
/main/3      ---        gvj  1. Updated for LTE MAC Rel 3.1           
$SID$        ---        rt   1. LTE MAC 4.1 release
*********************************************************************91*/
