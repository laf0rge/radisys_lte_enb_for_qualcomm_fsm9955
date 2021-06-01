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
  
     File:    rg_dux.c 
  
     Sid:      gk_dux.c@@/main/3 - Sat Jul 30 02:21:27 2011
  
     Prg:     sd 
  
**********************************************************************/

/** @file rg_dux.c
@brief This module handles de-multiplexing of the data recieved at MAC.
*/

static const char* RLOG_MODULE_NAME="MAC";
static int RLOG_MODULE_ID=4096;
static int RLOG_FILE_ID=232;
/* header include files -- defines (.h) */
#include "envopt.h"        /* environment options */
#include "envdep.h"        /* environment dependent */
#include "envind.h"        /* environment independent */
  
#include "gen.h"           /* general */
#include "ssi.h"           /* system services */

#include "cm_lte.h"        /* Common LTE */
#include "cm_tkns.h"       /* Common Token Defines */
#include "cm_llist.h"      /* Common Link List Defines */
#include "cm_hash.h"       /* Common Hash List Defines */
#include "cm_mblk.h"       /* common memory link list library */

#include "rg_env.h"        /* MAC Environment Defines */
#include "tfu.h"           /* CRG Interface defines */
#include "crg.h"           /* CRG Interface defines */
#include "rg_sch_inf.h"           /* RGR Interface defines */
#include "rgu.h"           /* RGU Interface defines */
#include "lrg.h"           /* LRG Interface defines */

#include "rg.h"            /* MAC defines */
#include "rg_err.h"        /* MAC error defines */

/* header/extern include files (.x) */
#include "gen.x"           /* general */
#include "ssi.x"           /* system services */
#include "cm5.x"           /* common timers */
#include "cm_lib.x"        /* common library */
#include "cm_lte.x"        /* Common LTE */
#include "cm_tkns.x"       /* Common Token Definitions */
#include "cm_llist.x"      /* Common Link List Definitions */
#include "cm_lib.x"        /* Common Library Definitions */
#include "cm_hash.x"       /* Common Hash List Definitions */
#include "cm_mblk.x"       /* common memory link list library */

#include "tfu.x"           /* CRG Interface includes */
#include "crg.x"           /* CRG Interface includes */
#include "rg_sch_inf.x"           /* RGR Interface includes */
#include "rgu.x"           /* RGU Interface includes */
#include "lrg.x"           /* LRG Interface includes */

#include "rg.x"            /* MAC includes */

/* local defines */

/* local typedefs */
 
/* local externs */
 
/* forward references */

#define RG_DUX_ALLOC(_pdu, _size, _dataPtr, _ret) {\
   _ret = cmGetMem((Ptr)&_pdu->memCp, _size, (Ptr *)&_dataPtr); \
}

#define RG_EXT_LCID(_lcId, _byte) {\
      (_lcId) = (_byte) & RG_LCID_MASK; \
}

#define RG_EXT_EXTN_BIT(_extnBit, _byte) {\
      (_extnBit) = ((_byte) >> RG_LCID_LEN) & 0x01; \
}

#define RG_INIT_CE_SUBHDR(_subHdr, _lcId) {\
   (_subHdr)->lcId = (_lcId); \
   (_subHdr)->subHdrLstEnt.next = NULLP; \
   (_subHdr)->subHdrLstEnt.prev = NULLP; \
   (_subHdr)->subHdrLstEnt.node = (PTR)(_subHdr); \
}

#define RG_INIT_SDU(_sdu, _lcId, _len) {\
   (_sdu)->lcId = (_lcId); \
   (_sdu)->len = (_len); \
   (_sdu)->mBuf = NULLP; \
   (_sdu)->sduLstEnt.next = NULLP; \
   (_sdu)->sduLstEnt.prev = NULLP; \
   (_sdu)->sduLstEnt.node = (PTR)(_sdu); \
}

#define RG_EXT_BS(_bsr, _bs1, _bs2, _bs3, _bs4)  {\
   _bs1 = _bsr[0] >> 2; \
   _bs2 = (((_bsr[0] & 0x3) << 4) | (_bsr[1] >> 4)); \
   _bs3 = (((_bsr[1] & 0x0F) << 2) | (_bsr[2] >> 6)); \
   _bs4 = _bsr[2] & 0x3F; \
}

#define RG_UNPACK_LONG_BSR(_bsr, _mBuf, _ret) {\
   _ret = SRemPreMsgMult((_bsr), 3, (_mBuf)); \
}

#define RG_UNPACK_SHORT_BSR(_bsr, _mBuf, _ret) {\
   _ret = SUnpkU8((_bsr), (_mBuf)); \
}

#define RG_UNPACK_TRUNC_BSR(_bsr, _mBuf, _ret) {\
   _ret = SUnpkU8((_bsr), (_mBuf)); \
}

#define RG_UNPACK_PHR(_phr, _mBuf, _ret) {\
   _ret = SUnpkU8((_phr), (_mBuf)); \
}

#define RG_UNPACK_CRNTI(_rnti, _mBuf, _ret) {\
   Data _unpkArray[2];\
   *_rnti = 0;\
   _ret = SRemPreMsgMult(_unpkArray, (MsgLen) 2, _mBuf);\
   if (_ret == ROK)\
   {\
      *_rnti = (U16) PutHiByte(*_rnti, (U8) _unpkArray[0]);\
      *_rnti = (U16) PutLoByte(*_rnti, (U8) _unpkArray[1]);\
   }\
}

/**
 * @brief Handles the extraction of SDU length.
 *
 * @details
 *
 *     Function: rgDUXExtSduLen
 *     
 *     This API handles the extraction of SDU length.
 *     
 *     Processing Steps: 
 *      - Extract the length based on the format.
 *
 *  @param[in]  *mBuf
 *  @param[out] sduLen
 *  @param[out] *err
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED 
 **/
#ifdef ANSI
PRIVATE S16 rgDUXExtSduLen
(
Buffer      *mBuf,
U16         *sduLen,
RgErrInfo   *err
)
#else
PRIVATE S16 rgDUXExtSduLen(mBuf, sduLen, err)
Buffer      *mBuf;
U16         *sduLen;
RgErrInfo   *err;
#endif
{
   U8    byte;
   U8    fmt;

   TRC2(rgDUXExtSduLen)

   if(SUnpkU8(&byte, mBuf) != ROK)
   {
      RLOG0(L_ERROR, "SUnpkU8 failed");
      err->errCause = RGERR_DUX_UNPACK_FAILURE;
      RETVALUE(RFAILED);
   }
   fmt = byte & 0x80;
   *sduLen = byte & 0x7F;
   if(fmt)
   {
      if(SUnpkU8(&byte,mBuf) != ROK)
      {
         RLOG0(L_ERROR, "SUnpkU8 failed");
         err->errCause = RGERR_DUX_UNPACK_FAILURE;
         RETVALUE(RFAILED);
      }
      *sduLen = (*sduLen << 8) | byte;
   }
   RETVALUE(ROK);
}

/**
 * @brief Handles the insertion of SDU in to PDU.
 *
 * @details
 *
 *     Function: rgDUXInsSdu
 *     
 *     This API handles the insertion of SDU in to PDU.
 *     
 *     Processing Steps: 
 *      - Append the sdu to the sduLst of pdu.
 *
 *  @param[out] *pdu
 *  @param[in]  lcId
 *  @param[in]  sduLen
 *  @param[out] *err
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED 
 **/
#ifdef ANSI
PRIVATE S16 rgDUXInsSdu
(
RgMacPdu    *pdu,
U8          lcId,
U16         sduLen,
RgErrInfo   *err
)
#else
PRIVATE S16 rgDUXInsSdu(pdu, lcId, sduLen, err)
RgMacPdu    *pdu;
U8          lcId;
U16         sduLen;
RgErrInfo   *err;
#endif
{
   S16         ret;
   RgMacSdu    *sdu = NULLP;

   TRC2(rgDUXInsSdu)

   RG_DUX_ALLOC(pdu, sizeof(RgMacSdu), sdu, ret);
   if(ret != ROK)
   {
      RLOG1(L_ERROR, "Allocation of RgSubHdr failed for LCID:%d",lcId);
      err->errCause = RGERR_DUX_MEM_EXHAUST;
      RETVALUE(RFAILED);
   }
   RG_INIT_SDU(sdu, lcId, sduLen);
   cmLListAdd2Tail(&pdu->sduLst, &sdu->sduLstEnt);
   RETVALUE(ROK);
}

/**
 * @brief Handles extracting the CE sub headers from the MAC PDU.
 *
 * @details
 *
 *     Function: rgDUXExtPadSubHdrs
 *     
 *     This API handles extracting the CE sub headers from the MAC PDU.
 *     
 *     Processing Steps: 
 *      - Extract the each ce sub header.
 *      - Append the extracted sub header to the subHdrLst.
 *
 *  @param[out]  *pdu
 *  @param[in]  *mBuf
 *  @param[out] *numPadSubHdrs
 *  @param[out] *unPkByte
 *  @param[out] *extnBit
 *  @param[out] *subHdrChkCmp
 *  @param[out] *err
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED 
 **/
#ifdef ANSI
PRIVATE S16 rgDUXExtPadSubHdrs
(
RgMacPdu    *pdu,
Buffer      *mBuf,
U8          *numPadSubHdrs,
U8          *unPkByte,
U8          *extnBit,
U8          *subHdrChkCmp,
RgErrInfo   *err
)
#else
PRIVATE S16 rgDUXExtPadSubHdrs(pdu, mBuf, numPadSubHdrs,
unPkByte, extnBit, subHdrChkCmp, err)
RgMacPdu    *pdu;
Buffer      *mBuf;
U8          *numPadSubHdrs;
U8          *unPkByte;
U8          *extnBit;
U8          *subHdrChkCmp;
RgErrInfo   *err;
#endif
{
   U8             byte;
   U8             lcId;

   TRC2(rgDUXExtPadSubHdrs)
   
   *numPadSubHdrs = 0;
   *subHdrChkCmp = TRUE;
   do
   {
      if(SUnpkU8(&byte,mBuf) != ROK)
      {
         RLOG0(L_ERROR, "SUnpkU8 failed");
         err->errCause = RGERR_DUX_UNPACK_FAILURE;
         RETVALUE(RFAILED);
      }

      /* Extract the lcid */
      RG_EXT_LCID(lcId, byte);
      RG_EXT_EXTN_BIT(*extnBit, byte);

      if(lcId == RG_PAD_LCID)
      {
         if(*extnBit)
         {
            ++(*numPadSubHdrs);      
            if(*numPadSubHdrs > RG_MAX_EXTN_PAD_SUBHDRS)
            {
               RLOG1(L_ERROR, "padSubHdrs received more than 2 for LCID:%d",lcId);
               err->errCause = RGERR_DUX_INV_PDU_RX;
               RETVALUE(RFAILED);      
            }
            continue;
         }
         else
         {
            if(*numPadSubHdrs)
            {
               RLOG1(L_ERROR, "invalid padSubHdrs received for LCID:%d",lcId);
               err->errCause = RGERR_DUX_INV_PDU_RX;
               RETVALUE(RFAILED);
            }
            break;
         }
      }
      else
      { 
         *unPkByte = byte;
         *subHdrChkCmp = FALSE;
         break;
      }
   }
   while(*extnBit);
   RETVALUE(ROK);
} /* rgDUXExtPadSubHdrs */

/**
 * @brief Handles extracting the CE sub headers from the MAC PDU.
 *
 * @details
 *
 *     Function: rgDUXExtCeSubHdrs
 *     
 *     This API handles extracting the CE sub headers from the MAC PDU.
 *     
 *     Processing Steps: 
 *      - Extract the each ce sub header.
 *      - Append the extracted sub header to the subHdrLst.
 *
 *  @param[out]  *pdu
 *  @param[in]  *mBuf
 *  @param[out] *numPadSubHdrs
 *  @param[out] *unPkByte
 *  @param[out] *extnBit
 *  @param[out] *subHdrChkCmp
 *  @param[out] *err
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED 
 **/
#ifdef ANSI
PRIVATE S16 rgDUXExtCeSubHdrs
(
RgMacPdu    *pdu,
Buffer      *mBuf,
U8          *numPadSubHdrs,
U8          *unPkByte,
U8          *extnBit,
U8          *subHdrChkCmp,
RgErrInfo   *err
)
#else
PRIVATE S16 rgDUXExtCeSubHdrs(pdu, mBuf, numPadSubHdrs,
unPkByte, extnBit, subHdrChkCmp, err)
RgMacPdu    *pdu;
Buffer      *mBuf;
U8          *numPadSubHdrs;
U8          *unPkByte;
U8          *extnBit;
U8          *subHdrChkCmp;
RgErrInfo   *err;
#endif
{
   S16            ret;
   U8             byte;
   U8             lcId;
   RgMacCeSubHdr  *ceSubHdr = NULLP;

   TRC2(rgDUXExtCeSubHdrs)
   
   *subHdrChkCmp = TRUE;
  
   byte = *unPkByte; 
   do
   {
      /* Extract the lcid */
      RG_EXT_LCID(lcId, byte);
      RG_EXT_EXTN_BIT(*extnBit, byte);

      if(lcId == RG_PAD_LCID)
      {
         if(*extnBit)
         {
            RLOG1(L_ERROR, "invalid padSubHdrs received LCID:%d",lcId);
            err->errCause = RGERR_DUX_INV_PDU_RX;
            RETVALUE(RFAILED);      
         }
         else
         {
            if(*numPadSubHdrs)
            {
               RLOG1(L_ERROR, "invalid padSubHdrs received  LCID:%d",lcId);
               err->errCause = RGERR_DUX_INV_PDU_RX;
               RETVALUE(RFAILED);
            }
            break;
         }
      }
   
      if ((lcId >= RG_PHR_LCID) && (lcId <= RG_LONG_BSR_LCID))
      {
         RG_DUX_ALLOC(pdu, sizeof(RgMacCeSubHdr), ceSubHdr, ret);
         if(ret != ROK)
         {
            RLOG1(L_ERROR, "Allocation of RgMacCeSubHdr failed  LCID:%d",lcId);
            err->errCause = RGERR_DUX_MEM_EXHAUST;
            RETVALUE(RFAILED);
         }
         RG_INIT_CE_SUBHDR(ceSubHdr, lcId);
         cmLListAdd2Tail(&pdu->ceLst, &ceSubHdr->subHdrLstEnt);

         if(*extnBit)
         {
            if(SUnpkU8(&byte,mBuf) != ROK)
            {
               RLOG1(L_ERROR, "SUnpkU8 failed  LCID:%d",lcId);
               err->errCause = RGERR_DUX_UNPACK_FAILURE;
               RETVALUE(RFAILED);
            }
         }
      }
      else
      {
         *unPkByte = byte;
         *subHdrChkCmp = FALSE;
         break;
      }
   }
   while(*extnBit);
   RETVALUE(ROK);
} /* rgDUXExtCeSubHdrs */

/**
 * @brief Handles extracting the SDU sub headers from the MAC PDU.
 *
 * @details
 *
 *     Function: rgDUXExtSduSubHdrs
 *     
 *     This API handles extracting the SDU sub headers from the MAC PDU.
 *     
 *     Processing Steps: 
 *      - Extract the each sdu sub header.
 *      - Append the extracted sub header to the sduLst.
 *
 *  @param[out]  *pdu
 *  @param[in]  *mBuf
 *  @param[in]  numPadSubHdrs
 *  @param[in]  unPkByte
 *  @param[out] *err
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED 
 **/
#ifdef ANSI
PRIVATE S16 rgDUXExtSduSubHdrs
(
RgMacPdu    *pdu,
RgInfCeInfo *ceInfo,
Buffer      *mBuf,
U8          numPadSubHdrs,
U8          unPkByte,
RgErrInfo   *err
)
#else
PRIVATE S16 rgDUXExtSduSubHdrs(pdu, ceInfo, mBuf,
numPadSubHdrs, unPkByte, err)
RgMacPdu    *pdu;
RgInfCeInfo *ceInfo;
Buffer      *mBuf;
U8          numPadSubHdrs;
U8          unPkByte;
RgErrInfo   *err;
#endif
{
   U8       byte;
   U8       lcId;
   U16      sduLen;
   Bool     extnBit;
   /*U8       numSduSubHdrs = 0;*/

   TRC2(rgDUXExtSduSubHdrs)
   
   byte = unPkByte;
   do 
   {
      /* Extract the lcid */
      RG_EXT_LCID(lcId, byte);
      RG_EXT_EXTN_BIT(extnBit, byte);

      if(lcId == RG_PAD_LCID)
      {
         if(extnBit)
         {
            RLOG1(L_ERROR,"invalid padSubHdrs received LCID:%d",lcId);
            err->errCause = RGERR_DUX_INV_PDU_RX;
            RETVALUE(RFAILED);      
         }
         else
         {
            if(numPadSubHdrs)
            {
               RLOG1(L_ERROR,"invalid padSubHdrs received LCID:%d",lcId);
               err->errCause = RGERR_DUX_INV_PDU_RX;
               RETVALUE(RFAILED);
            }
            break;
         }
      }

      if(lcId <= RG_DEDLC_MAX_LCID)
      {
        /* ++numSduSubHdrs;*/
         if(lcId == RG_CCCH_LCID)
         {
            ceInfo->bitMask |= RG_CCCH_SDU_PRSNT;
         }

         if(extnBit)
         {
            if(rgDUXExtSduLen(mBuf, &sduLen, err) != ROK)
            {
               RETVALUE(RFAILED);
            }
            if(rgDUXInsSdu(pdu, lcId, sduLen, err) != ROK)
            {
               RETVALUE(RFAILED);
            }
            if(SUnpkU8(&byte,mBuf) != ROK)
            {
               RLOG1(L_ERROR,"SUnpkU8 failed LCID:%d",lcId);
               err->errCause = RGERR_DUX_UNPACK_FAILURE;
               RETVALUE(RFAILED);
            }
            continue;
         }
         else
         {
            sduLen = 0;
            if(rgDUXInsSdu(pdu, lcId, sduLen, err) != ROK)
            {
               RETVALUE(RFAILED);
            }
            break;
         }
      }
      else
      {
         RLOG1(L_ERROR, "Invalid LCID:%u received" ,lcId); 
         err->errCause = RGERR_DUX_INV_LCID_RX;
         RETVALUE(RFAILED);
      }
   }
   while(extnBit);
   RETVALUE(ROK);
} /* rgDUXExtSduSubHdrs */

/**
 * @brief Handles extracting the CEs from the MAC PDU.
 *
 * @details
 *
 *     Function: rgDUXExtCes
 *     
 *     This API handles extracting the CEs from the MAC PDU.
 *     
 *     Processing Steps: 
 *      - Based on the ce sub header extract the ce.
 *
 *  @param[in,out]  *pdu
 *  @param[in]  *mBuf
 *  @param[out] *err
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED 
 **/
#ifdef ANSI
PRIVATE S16 rgDUXExtCes
(
RgMacPdu    *pdu,
RgInfCeInfo  *ceInfo,
Buffer       *mBuf,
RgErrInfo    *err
)
#else
PRIVATE S16 rgDUXExtCes(pdu, ceInfo, mBuf, err)
RgMacPdu    *pdu;
RgInfCeInfo  *ceInfo;
Buffer       *mBuf;
RgErrInfo    *err;
#endif
{
   S16            ret;
   CmLList        *tmpNode;
   RgMacCeSubHdr  *ceSubHdr;

   TRC2(rgDUXExtCes)

   CM_LLIST_FIRST_NODE(&pdu->ceLst,tmpNode);
   
   while(tmpNode)
   {
      ceSubHdr = (RgMacCeSubHdr *)(tmpNode->node);
      switch(ceSubHdr->lcId)
      {
         case RG_PHR_LCID:
               {
                  RG_UNPACK_PHR(&ceInfo->ces.phr,mBuf,ret);
                  if(ret != ROK)
                  {
                     RLOG1(L_ERROR,"Unpacking of PHR failed LCID:%d",ceSubHdr->lcId);
                     err->errCause = RGERR_DUX_UNPACK_FAILURE;
                     RETVALUE(RFAILED);
                  }
                  ceInfo->bitMask |= RG_PHR_CE_PRSNT;
               }
               break;
         case RG_TRUNC_BSR_LCID:
               {
                  RG_UNPACK_TRUNC_BSR(&ceInfo->ces.bsr.truncBsr,mBuf,ret);
                  if(ret != ROK)
                  {
                     RLOG1(L_ERROR,"Unpacking of Trunc BSR failed LCID:%d",ceSubHdr->lcId);
                     err->errCause = RGERR_DUX_UNPACK_FAILURE;
                     RETVALUE(RFAILED);
                  }
                  ceInfo->bitMask |= RG_TRUNC_BSR_CE_PRSNT;
              }
               break;
         case RG_SHORT_BSR_LCID:
               {
                  RG_UNPACK_SHORT_BSR(&ceInfo->ces.bsr.shortBsr,mBuf,ret);
                  if(ret != ROK)
                  {
                     RLOG1(L_ERROR,"Unpacking of Short BSR failed LCID:%d",ceSubHdr->lcId);
                     err->errCause = RGERR_DUX_UNPACK_FAILURE;
                     RETVALUE(RFAILED);
                  }
                  ceInfo->bitMask |= RG_SHORT_BSR_CE_PRSNT;
               }
               break;
         case RG_LONG_BSR_LCID:
               {
                  U8 longBsr[3] = {0}; /* KW_FIXX */
                  RG_UNPACK_LONG_BSR(longBsr,mBuf,ret);
                  if(ret != ROK)
                  {
                     RLOG1(L_ERROR,"Unpacking of Long BSR failed LCID:%d",ceSubHdr->lcId);
                     err->errCause = RGERR_DUX_UNPACK_FAILURE;
                     RETVALUE(RFAILED);
                  }
                  RG_EXT_BS(longBsr, 
                     ceInfo->ces.bsr.longBsr.bs1, 
                     ceInfo->ces.bsr.longBsr.bs2, 
                     ceInfo->ces.bsr.longBsr.bs3, 
                     ceInfo->ces.bsr.longBsr.bs4);
                  ceInfo->bitMask |= RG_LONG_BSR_CE_PRSNT;
               }
               break;
         case RG_CRNTI_LCID:
               {
                  RG_UNPACK_CRNTI(&ceInfo->ces.cRnti,mBuf,ret);
                  if(ret != ROK)
                  {
                     RLOG1(L_ERROR,"Unpacking of C-RNTI failed LCID:%d",ceSubHdr->lcId);
                     err->errCause = RGERR_DUX_UNPACK_FAILURE;
                     RETVALUE(RFAILED);
                  }
                  ceInfo->bitMask |= RG_CRNTI_CE_PRSNT;
               }
               break;
         default:
            RLOG1(L_ERROR, "Invalid LCID:%u received",ceSubHdr->lcId); 
            err->errCause = RGERR_DUX_INV_LCID_RX;
            RETVALUE(RFAILED);
      }
      CM_LLIST_NEXT_NODE(&pdu->ceLst,tmpNode);
   }
   RETVALUE(ROK);
} /* rgDUXExtCes  */


/**
 * @brief Handles extracting the SDU from the MAC PDU.
 *
 * @details
 *
 *     Function: rgDUXExtSdus
 *     
 *     This API handles extracting the SDU corresponding to a logical channel.
 *     
 *     Processing Steps: 
 *      - Based on the length stored in the sub header extract the SDU.
 *
 *  @param[in,out]  *pdu
 *  @param[in]  *mBuf
 *  @param[out] *err
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED 
 **/
#ifdef ANSI
PRIVATE S16 rgDUXExtSdus
(
RgMacPdu    *pdu,
Buffer      *mBuf,
RgErrInfo   *err
)
#else
PRIVATE S16 rgDUXExtSdus(pdu, mBuf, err)
RgMacPdu    *pdu;
Buffer      *mBuf;
RgErrInfo   *err;
#endif
{
   S16         ret;
   Buffer      *tmpBuf1;
   Buffer      *tmpBuf2 = NULLP;
   CmLList     *tmpNode;
   RgMacSdu    *sdu;

   TRC2(rgDUXExtSdus)

   tmpBuf1 = mBuf;
   CM_LLIST_FIRST_NODE(&pdu->sduLst,tmpNode);
   
   while(tmpNode)
   {
      sdu = (RgMacSdu *)(tmpNode->node);

      if(!sdu->len)
      {
         MsgLen len = 0;
         if(SFndLenMsg(tmpBuf1,&len) != ROK)
         {
            RG_FREE_MSG(tmpBuf1);
            RLOG0(L_ERROR,"mBuf length check failed");
            err->errCause = RGERR_DUX_UNPACK_FAILURE;
            RETVALUE(RFAILED);
         }
         sdu->len = len;
      }

      ret = SSegMsg(tmpBuf1,sdu->len,&tmpBuf2);

      if((ret != ROK) && (!((ret == ROKDNA) && (tmpNode->next == NULLP))))
      {
         RG_FREE_MSG(tmpBuf1);
         RLOG0(L_ERROR,"SSegMsg failed");
         err->errCause = RGERR_DUX_RLC_PDU_CREAT_FAIL;
         RETVALUE(RFAILED);
      }

      sdu->mBuf = tmpBuf1;
      tmpBuf1 = tmpBuf2;

      CM_LLIST_NEXT_NODE(&pdu->sduLst,tmpNode);
   }
   RG_FREE_MSG(tmpBuf1);
   RETVALUE(ROK);
}   /* rgDUXExtSdus */

/**
 * @brief Handles de-multiplexing of the data recieved at MAC.
 *
 * @details
 *
 *     Function: rgDUXDemuxData
 *     
 *     This API handles de-multiplexing of the data recieved at MAC.
 *     
 *     Invoked by: rgTOMTfuDatInd of TOM 
 *     
 *     Processing Steps: 
 *      - De-multiplex the mBuf
 *
 *  @param[in]  *pdu 
 *  @param[in]  *mBuf 
 *  @param[out] *err 
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED 
 **/
#ifdef ANSI
PUBLIC S16 rgDUXDemuxData
(
RgMacPdu      *pdu,
RgInfCeInfo   *ceInfo,
Buffer        **mBuf,
RgErrInfo     *err
)
#else
PUBLIC S16 rgDUXDemuxData(pdu, ceInfo, mBuf, err)
RgMacPdu      *pdu;
RgInfCeInfo   *ceInfo;
Buffer        **mBuf;
RgErrInfo     *err;
#endif
{
   U8          numPadSubHdrs = 0;
   U8          unPkByte;
   U8          subHdrChkCmp = TRUE;
   U8          extnBit = TRUE;

   TRC2(rgDUXDemuxData)

   ceInfo->bitMask = 0x0000;

   /* Initialize the ce sub header list */
   cmLListInit(&pdu->ceLst);

   /* Initialize the sdu list */
   cmLListInit(&pdu->sduLst);

   /* Extract the pad sub headers */
   if(rgDUXExtPadSubHdrs(pdu, *mBuf, &numPadSubHdrs, 
         &unPkByte, &extnBit, &subHdrChkCmp, err) != ROK)
   {
      RG_FREE_MSG(*mBuf);         
      RLOG0(L_ERROR, "Failed to extract pad sub headers");
      RETVALUE(RFAILED);
   }

   if((!extnBit) && (subHdrChkCmp))
   {
      RG_FREE_MSG(*mBuf);         
      RETVALUE(ROK);
   } 

   /* Extract the ce sub headers */
   if(rgDUXExtCeSubHdrs(pdu, *mBuf,&numPadSubHdrs, 
         &unPkByte, &extnBit, &subHdrChkCmp, err) != ROK)
   {
      RG_FREE_MSG(*mBuf);         
      RLOG0(L_ERROR, "failed to Extract the ce sub headers");
      RETVALUE(RFAILED);
   }

   if((!extnBit) && (subHdrChkCmp))
   {
      /* Extract the ces */
      if(rgDUXExtCes(pdu, ceInfo, *mBuf, err) != ROK)
      {
         RG_FREE_MSG(*mBuf);         
         RLOG0(L_ERROR, "failed to Extract the ces");
         RETVALUE(RFAILED);
      }
      RG_FREE_MSG(*mBuf);         
      RETVALUE(ROK);
   }

   /* Extract the sdu sub headers */
   if(rgDUXExtSduSubHdrs(pdu, ceInfo, *mBuf, 
         numPadSubHdrs, unPkByte, err) != ROK)
   {
      RG_FREE_MSG(*mBuf);         
      RLOG0(L_ERROR, "failed to Extract the sdu sub headers");
      RETVALUE(RFAILED);
   }

   /* Extract the ces */
   if(rgDUXExtCes(pdu, ceInfo, *mBuf, err) != ROK)
   {
      RG_FREE_MSG(*mBuf);         
      RLOG0(L_ERROR, " failed to Extract the ces");
      RETVALUE(RFAILED);
   }

   /* Extract the sdus */
   if(rgDUXExtSdus(pdu, *mBuf, err) != ROK)
   {
      /* Fix : syed rgDUXExtSdus would have segmented mBuf and hence
       * will be responsible for freeing mBuf */
      *mBuf = NULLP;
      RLOG0(L_ERROR, "failed to Extract the sdus");
      RETVALUE(RFAILED);
   }
   *mBuf = NULLP;
   RETVALUE(ROK);
}  /* rgDUXDemuxData */

/**********************************************************************
 
         End of file:     gk_dux.c@@/main/3 - Sat Jul 30 02:21:27 2011
 
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
/main/1      ---     sd   1. Initial Release.
/main/2      ---     sd   1. LTE MAC 2.1 release
/main/3      ---        gvj  1. Updated for LTE MAC Rel 3.1         
$SID$        ---     rt   1. LTE MAC 4.1 release
*********************************************************************91*/
