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
  
     File:     rg_uim.c 
  
     Sid:      gk_uim.c@@/main/3 - Sat Jul 30 02:21:57 2011
  
     Prg:      sd
  
**********************************************************************/

/** @file rg_uim.c.
@brief This module acts as an interface handler for upper interface and 
manages Pst and Sap related information for upper interface APIs.
*/

static const char* RLOG_MODULE_NAME="MAC";
static int RLOG_MODULE_ID=4096;
static int RLOG_FILE_ID=178;

/* header include files -- defines (.h) */

/* header/extern include files (.x) */
/* header include files (.h) */
#include "envopt.h"        /* environment options */
#include "envdep.h"        /* environment dependent */
#include "envind.h"        /* environment independent */
#include "gen.h"           /* general layer */
#include "ssi.h"           /* system services */
#include "cm5.h"           /* common timers defines */
#include "cm_hash.h"       /* common hash list defines */
#include "cm_llist.h"      /* common linked list defines */
#include "cm_mblk.h"       /* memory management */
#include "cm_tkns.h"       /* common tokens */
#include "cm_lte.h"
#include "crg.h"           /* CRG defines */
#include "lrg.h"           /* layer management defines for LTE-MAC */
#include "tfu.h"
#include "rgu.h"
#include "rg_sch_inf.h"
#include "rg_env.h"        /* customizable defines and macros for MAC */
#include "rg.h"            /* defines and macros for MAC */
#include "rg_err.h"        /* RG error defines */

/* header/extern include files (.x) */

#include "gen.x"           /* general layer typedefs */
#include "ssi.x"           /* system services typedefs */
#include "cm5.x"           /* common timers */
#include "cm_hash.x"       /* common hash list */
#include "cm_lib.x"        /* common library */
#include "cm_llist.x"      /* common linked list */
#include "cm_mblk.x"       /* memory management */
#include "cm_tkns.x"       /* common tokens */
#include "cm_lte.x"
#include "crg.x"           /* CRG types */
#include "lrg.x"           /* layer management typedefs for MAC */
#include "tfu.x"
#include "rgu.x"
#include "rg_sch_inf.x"
#include "rg.x"            /* typedefs for MAC */
#ifdef SS_RBUF
#include "ss_rbuf.h"
#include "ss_rbuf.x"
#endif

/* local defines */

/* local typedefs */
 
/* local externs */
 
/* forward references */

#if defined(SPLIT_RLC_DL_TASK) && defined(RLC_MAC_STA_RSP_RBUF)
PUBLIC S16 rgBatchProc(Void);
#endif
PUBLIC U8 rgRguDlSap;
PUBLIC U8 rgRguUlSap;
/**
 * @brief Handler for Bind request.
 *
 * @details
 *
 *     Function : RgUiRguBndReq
 *     
 *     This function handles the bind request from MAC Service User.
 *     
 *           
 *  @param[in]  Pst  *pst
 *  @param[in]  SuId suId
 *  @param[in]  SpId spId
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED 
 **/
#ifdef ANSI
PUBLIC S16 RgUiRguBndReq
(
Pst  *pst,
SuId suId,
SpId spId
)
#else
PUBLIC S16 RgUiRguBndReq(pst, suId, spId)
Pst  *pst;
SuId suId;
SpId spId;
#endif
{
   S16       ret = ROK;
   Pst       tmpPst;   /* Temporary Post Structure */
   RgUstaDgn dgn;      /* Alarm diagnostics structure */

   TRC3(RgUiRguBndReq)


   tmpPst.prior       = pst->prior;
   tmpPst.route       = pst->route;
   tmpPst.selector    = pst->selector;
   tmpPst.region      = rgCb.rgInit.region;
   tmpPst.pool        = rgCb.rgInit.pool;
   tmpPst.srcProcId   = rgCb.rgInit.procId;
   tmpPst.srcEnt      = rgCb.rgInit.ent;
   tmpPst.srcInst     = rgCb.rgInit.inst;
   tmpPst.event       = EVTNONE;
   tmpPst.dstProcId   = pst->srcProcId;
   tmpPst.dstEnt      = pst->srcEnt;
   tmpPst.dstInst     = pst->srcInst;

   if(spId == rgCb.rguSap[spId].sapCfg.spId)
   {
      /* Check the state of the SAP */
      switch (rgCb.rguSap[spId].sapSta.sapState)
      {
         case LRG_NOT_CFG: /* SAP Not configured */
            RLOG0(L_DEBUG,"SAP Not Configured");
            rgFillDgnParams(&dgn, LRG_USTA_DGNVAL_MEM); 
            ret = rgLMMStaInd(LCM_CATEGORY_INTERFACE,LRG_NOT_CFG,
                  LCM_CAUSE_INV_SAP, &dgn);
            ret = RgUiRguBndCfm(&tmpPst, suId, CM_BND_NOK);
            break;
         case LRG_UNBND: /* SAP is not bound */
            RLOG0(L_DEBUG,"SAP Not yet bound");
            rgCb.rguSap[spId].sapSta.sapState = LRG_BND;
            rgCb.rguSap[spId].sapCfg.suId = suId;
            /* Send Bind Confirm with status as SUCCESS */
            /*T2K - Passing spId as it is required to access the SAP CB*/
            ret = rgUIMRguBndCfm(spId, CM_BND_OK);
            /* Indicate to Layer manager */ 
            rgFillDgnParams(&dgn, LRG_USTA_DGNVAL_MEM); 
            ret = rgLMMStaInd(LCM_CATEGORY_INTERFACE,LRG_EVENT_RGUSAP_ENB,
                  LCM_CAUSE_UNKNOWN, &dgn);
            break;
         case LRG_BND: /* SAP is already bound*/
            RLOG0(L_DEBUG,"SAP already bound");
            /*T2K - Passing spId as it is required to access the SAP CB*/
            ret = rgUIMRguBndCfm(spId, CM_BND_OK);
            break;
         default: /* Should Never Enter here */
#if (ERRCLASS & ERRCLS_ADD_RES)      
            RGLOGERROR(ERRCLS_INT_PAR, ERG008, (ErrVal)rgCb.rguSap[spId].sapSta.sapState,
                  "Invalid SAP State:RgUiRguBndReq failed\n");
#endif
            /*T2K - Passing spId as it is required to access the SAP CB*/
            ret = rgUIMRguBndCfm(spId, CM_BND_NOK);
            break;
      }
   }
   else
   {
#if (ERRCLASS & ERRCLS_ADD_RES)      
      RGLOGERROR(ERRCLS_INT_PAR, ERG009, (ErrVal)rgCb.rguSap[spId].sapCfg.spId,
            "Invalid SAP Id:RgUiRguBndReq failed\n");
#endif
      /*T2K - Passing spId as it is required to access the SAP CB*/
      ret = rgUIMRguBndCfm(spId, CM_BND_NOK);
   }
   RETVALUE(ret);
}  /* RgUiRguBndReq */


/**
 * @brief Handler for Unbind request.
 *
 * @details
 *
 *     Function : RgUiRguUbndReq
 *     
 *     This function handles the unbind request from MAC Service User.
 *     
 *           
 *  @param[in]  Pst    *pst
 *  @param[in]  SpId   spId
 *  @param[in]  Reason reason
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED 
 **/
#ifdef ANSI
PUBLIC S16 RgUiRguUbndReq
(
Pst    *pst,
SpId   spId,
Reason reason
)
#else
PUBLIC S16 RgUiRguUbndReq(pst, spId, reason)
Pst    *pst;
SpId   spId;
Reason reason;
#endif
{
   TRC3(RgUiRguUbndReq)
   

   /* SAP Id validation */
   if (spId == rgCb.rguSap[spId].sapCfg.spId)
   {
      switch(rgCb.rguSap[spId].sapSta.sapState)
      {
         case LRG_BND: /* SAP is already bound*/
            /* setting SAP state to UN BOUND */
            RLOG0(L_DEBUG,"SAP already bound");
            rgCb.rguSap[spId].sapSta.sapState = LRG_UNBND;
            break;
         default:
#if (ERRCLASS & ERRCLS_ADD_RES)      
            RLOG1(L_ERROR,"Invalid SAP State:%d RgUiRguUbndReq failed",
                  rgCb.rguSap[spId].sapSta.sapState);
#endif
            break;
      }
   }
   else
   {
#if (ERRCLASS & ERRCLS_ADD_RES)      
      RGLOGERROR(ERRCLS_INT_PAR, ERG011, (ErrVal)rgCb.rguSap[spId].sapCfg.spId,
            "Invalid SAP Id:RgUiRguUbndReq failed\n");
#endif
      RETVALUE(RFAILED);
   }
   RETVALUE(ROK);
}  /* RgUiRguUbndReq */
/**
 * @brief API for sending bind confirm from MAC to RLC
 *
 * @details
 *
 *     Function: rgUIMRguBndCfm
 *     
 *     This API is invoked to send bind confirm from MAC to RLC.
 *     This API fills in Pst structure and SAP Ids and invokes 
 *     bind confirm API towards RLC.
 *           
 *  @param[in]  SuId          suId
 *  @param[in]  U8            status
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED 
 **/
#ifdef ANSI
PUBLIC S16 rgUIMRguBndCfm
(
SpId spId,
U8 status
)
#else
PUBLIC S16 rgUIMRguBndCfm(spId, status)
SpId          spId;
U8            status;
#endif
{
   S16  ret = ROK;
   
   TRC2(rgUIMRguBndCfm)
   

   ret = RgUiRguBndCfm(&rgCb.rguSap[spId].sapCfg.sapPst, 
                      rgCb.rguSap[spId].sapCfg.suId, status);
   if (ret != ROK)
   {
      RLOG0(L_ERROR,"RgUiRguBndCfm Failed ");
      RETVALUE(ret);
   }
   RETVALUE(ret);
}  /* rgUIMRguBndCfm*/


/**
 * @brief Handler for dedicated DatReq from RGU
 *
 * @details
 *
 *     Function : RgUiRguDDatReq
 *     
 *     This function validates SAP and invokes ROM for further processing
 *     
 *  @param[in]  Pst             *pst 
 *  @param[in]  SpId            spId 
 *  @param[in]  RguDDatReqInfo  *datReq
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED 
 **/
#ifdef ANSI
PUBLIC S16 RgUiRguDDatReq
(
Pst             *pst,
SpId            spId,
RguDDatReqInfo  *datReq
)
#else
PUBLIC S16 RgUiRguDDatReq(pst, spId, datReq)
Pst             *pst;
SpId            spId;
RguDDatReqInfo  *datReq;
#endif
{
   S16   ret = ROK;

#ifndef NO_ERRCLS 
   U32   id;
   U32   id1;
   U32   id2;
   U32   id3;
#endif
   
   TRC3(RgUiRguDDatReq)


#ifndef NO_ERRCLS
   if (datReq == NULLP)
   {
      RLOG0(L_ERROR,"Input Message Buffer is NULL");
      RETVALUE(RFAILED);
   }
   
   if(rgCb.rguSap[spId].sapCfg.spId == spId)
   {
      switch (rgCb.rguSap[spId].sapSta.sapState)
      {
         case LRG_BND: /* SAP is bound */
            RLOG0(L_DEBUG,"SAP is already bound");
            break;
         default: /* Should never reach here */
#if (ERRCLASS & ERRCLS_ADD_RES)      
            RLOG1(L_ERROR,"Invalid SAP State:%d RgUiRguDDatReq failed",
                  rgCb.rguSap[spId].sapSta.sapState);
#endif
            for(id3 = 0; id3 < datReq->nmbOfUeGrantPerTti; id3++)
            {
               RG_DROP_RGUDDATREQ_MBUF(datReq->datReq[id3]);
            }
            RETVALUE(RFAILED);
      }
   }
   else
   {
#if (ERRCLASS & ERRCLS_ADD_RES)      
      RGLOGERROR(ERRCLS_INT_PAR, ERG013, (ErrVal)spId,
            "Invalid SAP Id:RgUiRguDDatReq failed\n");
#endif
      for(id3 = 0; id3 < datReq->nmbOfUeGrantPerTti; id3++)
      {
         RG_DROP_RGUDDATREQ_MBUF(datReq->datReq[id3]);
      }
      RETVALUE(RFAILED);
   }

   /* Update RGU SAP statistics for received sdu count */
   /*ccpu00118201 - ADD - Send trace only when its enabled*/
   if(rgCb.rgInit.trc)
   {
      for(id3 = 0; id3 < datReq->nmbOfUeGrantPerTti; id3++)
      {
         RguDDatReqPerUe *datReqPerUe = &datReq->datReq[id3];
         for (id = 0; id < datReqPerUe->nmbOfTbs; id++)
         {
            for (id1 = 0; id1 < datReqPerUe->datReqTb[id].nmbLch; id1++)
            {
               /* rgCb.rguSap.sapSts.numPduRcvd is updated by 
                * rgROMDedDatReq -> rgUpdtRguDedSts function
                * So numPduRcvd updation is commented here */
               /* rgCb.rguSap.sapSts.numPduRcvd +=
                  datReq->datReqTb[id].lchData[id1].pdu.numPdu; */
               for (id2 = 0; id2 < datReqPerUe->datReqTb[id].lchData[id1].pdu.numPdu; id2++)
               {
                  RG_SEND_TRC_IND(datReqPerUe->datReqTb[id].
                        lchData[id1].pdu.mBuf[id2], EVTRGUDDATREQ);
               }
            }
         }
      }
   }
#endif

   /* Call Ownership module for further processing */
   ret = rgROMDedDatReq(datReq);
    SPutStaticBuffer(pst->region, pst->pool, (Data *)datReq,sizeof(RguDDatReqInfo), SS_SHARABLE_MEMORY);
   datReq = NULLP;
   RETVALUE(ret);
}  /* RgUiRguDDatReq */


/**
 * @brief Handler for common DatReq from RGU
 *
 * @details
 *
 *     Function : RgUiRguCDatReq
 *     
 *     This function validates SAP invokes ROM for further processing
 *     
 *  @param[in]  Pst             *pst 
 *  @param[in]  SpId            spId 
 *  @param[in]  RguCDatReqInfo  *datReq
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED 
 **/
#ifdef ANSI
PUBLIC S16 RgUiRguCDatReq
(
Pst             *pst,
SpId            spId,
RguCDatReqInfo  *datReq
)
#else
PUBLIC S16 RgUiRguCDatReq(pst, spId, datReq)
Pst             *pst;
SpId            spId;
RguCDatReqInfo  *datReq;
#endif
{
   S16   ret = ROK;
   
   TRC3(RgUiRguCDatReq)


#ifndef NO_ERRCLS
   if (datReq == NULLP)
   {
      RLOG0(L_ERROR,"Input Message Buffer is NULL");
      RETVALUE(RFAILED);
   }
   
   if(rgCb.rguSap[spId].sapCfg.spId == spId)
   {
      switch (rgCb.rguSap[spId].sapSta.sapState)
      {
         case LRG_BND: /* SAP is bound */
            RLOG0(L_DEBUG,"SAP is already bound");
            break;
         default: /* Should never reach here */
#if (ERRCLASS & ERRCLS_ADD_RES)      
            RLOG1(L_ERROR,"Invalid SAP State:%d RgUiRguCDatReq failed",
                  rgCb.rguSap[spId].sapSta.sapState);
#endif
            RETVALUE(RFAILED);
      }
   }
   else
   {
#if (ERRCLASS & ERRCLS_ADD_RES)      
      RLOG1(L_ERROR,"Invalid SAP Id:%d RgUiRguCDatReq failed ",spId);
#endif
      RETVALUE(RFAILED);
   }
#endif

   /* Update RGU SAP statistics for received sdu count */
   /* rgCb.rguSap.sapSts.numPduRcvd is updated by 
    * rgROMCmnDatReq ->rgUpdtRguCmnSts function
    * So numPduRcvd updation is commented here */
   /* rgCb.rguSap.sapSts.numPduRcvd++; */

   ret = rgROMCmnDatReq(datReq);
   /*ccpu00118201 - ADD - Send trace only when its enabled*/
   if(rgCb.rgInit.trc)
   {
      RG_SEND_TRC_IND(datReq->pdu, EVTRGUCDATREQ);
   }
   if (ret == RFAILED)
   {
      RG_DROP_RGUCDATREQ_MBUF(datReq);
   }
   ret = SPutStaticBuffer(pst->region, pst->pool,(Data *)datReq,sizeof(RguCDatReqInfo) , SS_SHARABLE_MEMORY);
   datReq = NULLP;
   RETVALUE(ret);
}  /* RgUiRguCDatReq */


/**
 * @brief Handler for dedicated StaRsp from RGU
 *
 * @details
 *
 *     Function : RgUiRguDStaRsp
 *     
 *     This function validates SAP and invokes ROM for further processing
 *           
 *  @param[in]  Pst             *pst 
 *  @param[in]  SpId            spId 
 *  @param[in]  RguDStaRspInfo  *staRsp
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED 
 **/
#ifdef ANSI
PUBLIC S16 RgUiRguDStaRsp
(
Pst             *pst,
SpId            spId,
RguDStaRspInfo  *staRsp
)
#else
PUBLIC S16 RgUiRguDStaRsp(pst, spId, staRsp)
Pst             *pst;
SpId            spId;
RguDStaRspInfo  *staRsp;
#endif
{
   S16              ret       = ROK;
   VOLATILE U32     startTime = 0;

   TRC3(RgUiRguDStaRsp)

   /*starting Task*/
   SStartTask(&startTime, PID_MAC_STA_RSP);

   ret = rgROMDedStaRsp(staRsp);
   if (ret != ROK)
   {
      RLOG_ARG0(L_ERROR,DBG_CELLID,staRsp->cellId,
                "Processing Of Status Response Failed");
   }


   /*stoping Task*/
   SStopTask(startTime, PID_MAC_STA_RSP);
   RETVALUE(ret);
}  /* RgUiRguDStaRsp */


/**
 * @brief Handler for common StaRsp from RGU
 *
 * @details
 *
 *     Function : RgUiRguCStaRsp
 *     
 *     This function validates SAP and invokes ROM 
 *     for further processing
 *     
 *           
 *  @param[in]  Pst             *pst 
 *  @param[in]  SpId            spId 
 *  @param[in]  RguCStaRspInfo  *staRsp
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED 
 **/
#ifdef ANSI
PUBLIC S16 RgUiRguCStaRsp
(
Pst             *pst,
SpId            spId,
RguCStaRspInfo  *staRsp
)
#else
PUBLIC S16 RgUiRguCStaRsp(pst, spId, staRsp)
Pst             *pst;
SpId            spId;
RguCStaRspInfo  *staRsp;
#endif
{
   S16   ret = ROK;

   TRC3(RgUiRguCStaRsp)
   

#ifndef NO_ERRCLS
   if (staRsp == NULLP)
   {
      RLOG0(L_ERROR,"Input Response Buffer is NULL");
      RETVALUE(RFAILED);
   }

   if (spId == rgCb.rguSap[spId].sapCfg.spId)
   {
      switch (rgCb.rguSap[spId].sapSta.sapState)
      {
         case LRG_BND: /* SAP is bound */
            RLOG0(L_DEBUG,"SAP is already bound");
            break;
         default: /* Should never reach here */
#if (ERRCLASS & ERRCLS_ADD_RES)      
            RLOG1(L_ERROR,"Invalid SAP State:%d RgUiRguCStaRsp failed",
                  rgCb.rguSap[spId].sapSta.sapState);
#endif
            RETVALUE(RFAILED);
      }
   }
   else
   {
#if (ERRCLASS & ERRCLS_ADD_RES)      
      RLOG1(L_ERROR,"Invalid SAP Id:%d RgUiRguCStaRsp failed",spId);
#endif
      RETVALUE(RFAILED);
   }
#endif

   ret = rgROMCmnStaRsp(staRsp);
   if (ret != ROK)
   {
      RLOG_ARG0(L_ERROR,DBG_CELLID,staRsp->cellId,"Processing Of Status Response Failed");
      RETVALUE(ret);
   }

   ret = SPutStaticBuffer(pst->region, pst->pool, (Data *)staRsp,sizeof(RguCStaRspInfo) , SS_SHARABLE_MEMORY);
   staRsp = NULLP;
   RETVALUE(ret);
}  /* RgUiRguCStaRsp */

#ifdef LTE_L2_MEAS

/**
 * @brief Handler for L2M MeasReq from RGU
 *
 * @details
 *
 *     Function :RgUiRguL2MUlThrpMeasReq 
 *     
 *     This function validates SAP and invokes ROM for further processing
 *           
 *  @param[in]  Pst             *pst 
 *  @param[in]  SpId            spId 
 *  @param[in]  RguL2MUlThrpMeasReqInfo  *measReq
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED 
 **/
#ifdef ANSI
PUBLIC S16 RgUiRguL2MUlThrpMeasReq 
(
Pst             *pst,
SpId            spId,
RguL2MUlThrpMeasReqInfo  *measReq
)
#else
PUBLIC S16 RgUiRguL2MUlThrpMeasReq(pst, spId, measReq)
Pst             *pst;
SpId            spId;
RguL2MUlThrpMeasReqInfo  *measReq;
#endif
{
   S16   ret = ROK;

   TRC3(RgUiRguL2MUlThrpMeasReq)
   

#ifndef NO_ERRCLS
   if (measReq == NULLP)
   {
      RLOG0(L_ERROR,"Input Response Buffer is NULL");
      RETVALUE(RFAILED);
   }

   if (spId == rgCb.rguSap[spId].sapCfg.spId)
   {
      switch (rgCb.rguSap[spId].sapSta.sapState)
      {
         case LRG_BND: /* SAP is bound */
            RLOG0(L_DEBUG,"SAP is already bound");
            break;
         default: /* Should never reach here */
#if (ERRCLASS & ERRCLS_ADD_RES)      
            RLOG1(L_ERROR,"Invalid SAP State:%d RgUiRguL2MUlThrpMeasReq failed",
                  rgCb.rguSap[spId].sapSta.sapState);
#endif
            RETVALUE(RFAILED);
      }
   }
   else
   {
#if (ERRCLASS & ERRCLS_ADD_RES)      
      RLOG1(L_ERROR,"Invalid SAP Id:%d RgUiRguL2MUlThrpMeasReq failed",spId);
#endif
      RETVALUE(RFAILED);
   }
#endif

   ret = rgROML2MUlThrpMeasReq(measReq);
   if (ret != ROK)
   {
      RLOG_ARG0(L_ERROR,DBG_CELLID,measReq->cellId,"Processing Of Meas Request Failed");
   }

  SPutStaticBuffer(pst->region, pst->pool, (Data *)measReq,sizeof(RguL2MUlThrpMeasReqInfo) , SS_SHARABLE_MEMORY);
   measReq= NULLP;
   RETVALUE(ret);
}  /* RgUiRguL2MUlThrpMeasReq */
#endif

/**
 * @brief Handler for sending staInd to dedicated logical channels of a UE 
 *
 * @details
 *
 *     Function : rgUIMSndDedStaInd
 *     
 *     This function fills SAP and Pst information to send the staInd to
 *     a UE.
 *     
 *           
 *  @param[in]  RgRguDedStaInd  *staInd
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED 
 **/
#ifdef ANSI
PUBLIC S16 rgUIMSndDedStaInd
(
RgRguDedStaInd  *staInd
)
#else
PUBLIC S16 rgUIMSndDedStaInd(staInd)
RgRguDedStaInd  *staInd;
#endif
{
   S16  ret = ROK;
   SuId  suId;
   suId  = rgRguDlSap;
   
   TRC2(rgUIMSndDedStaInd)
   
   
   ret = RgUiRguDStaInd(&rgCb.rguSap[suId].sapCfg.sapPst, rgCb.rguSap[suId].sapCfg.suId, 
         staInd);
   if (ret != ROK)
   {
      RLOG_ARG0(L_ERROR,DBG_CELLID,staInd->cellId,"RgUiRguDStaInd Failed");
      RETVALUE(ret);
   }
   RETVALUE(ret);
}  /* rgUIMSndDedStaInd */


/**
 * @brief Handler for sending staInd to a common logical channel.
 *
 * @details
 *
 *     Function : rgUIMSndCmnStaInd
 *     
 *     This function fills SAP and Pst information to send the staInd to
 *     a common logical channel.
 *     
 *           
 *  @param[in]  RgRguCmnStaInd  *staInd
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED 
 **/
#ifdef ANSI
PUBLIC S16 rgUIMSndCmnStaInd
(
RgRguCmnStaInd  *staInd
)
#else
PUBLIC S16 rgUIMSndCmnStaInd(staInd)
RgRguCmnStaInd  *staInd;
#endif
{
   S16  ret = ROK;
   SuId  suId;

   suId = rgRguDlSap;

   TRC2(rgUIMSndCmnStaInd)
   

   ret = RgUiRguCStaInd(&rgCb.rguSap[suId].sapCfg.sapPst, rgCb.rguSap[suId].sapCfg.suId, 
         staInd);
   if (ret != ROK)
   {
      RLOG_ARG0(L_ERROR,DBG_CELLID,staInd->cellId,"RgUiRguCStaInd Failed");
      RETVALUE(ret);
   }
   RETVALUE(ret);
}  /* rgUIMSndCmnStaInd */


/**
 * @brief Handler for sending datInd to dedicated logical channels of a UE 
 *
 * @details
 *
 *     Function : rgUIMSndDedDatInd
 *     
 *     This function fills SAP and Pst information to send the datInd to
 *     a UE.
 *     
 *           
 *  @param[in]  RgRguDedDatInd  *datInd
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED 
 **/
#ifdef ANSI
PUBLIC S16 rgUIMSndDedDatInd
(
RgRguDedDatInd  *datInd
)
#else
PUBLIC S16 rgUIMSndDedDatInd(datInd)
RgRguDedDatInd  *datInd;
#endif
{
   S16  ret = ROK;
   SuId suId;

   suId = rgRguUlSap;

   TRC2(rgUIMSndDedDatInd)
   

   rgCb.rguSap[suId].sapSts.numPduTxmit += datInd->numLch;
#ifndef SS_RBUF
   ret = RgUiRguDDatInd(&rgCb.rguSap[suId].sapCfg.sapPst, rgCb.rguSap[suId].sapCfg.suId, 
         datInd);
   if (ret != ROK)
   {
      RLOG_ARG0(L_ERROR,DBG_CELLID,datInd->cellId,"RgUiRguDdatInd Failed");
      RETVALUE(ret);
   }
#else
   SRngIncrWIndx(SS_RNG_BUF_ULMAC_TO_ULRLC);
   SsRngInfoTbl[SS_RNG_BUF_ULMAC_TO_ULRLC].pktRate++;
#endif
   RETVALUE(ret);
}  /* rgUIMSndDedDatInd */


/**
 * @brief Handler for sending datInd to a common logical channel.
 *
 * @details
 *
 *     Function : rgUIMSndCmnDatInd
 *     
 *     This function fills SAP and Pst information to send the datInd to
 *     a common logical channel.
 *     
 *           
 *  @param[in]  RgRguCmnDatInd  *datInd
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED 
 **/
#ifdef ANSI
PUBLIC S16 rgUIMSndCmnDatInd
(
RgRguCmnDatInd  *datInd
)
#else
PUBLIC S16 rgUIMSndCmnDatInd(datInd)
RgRguCmnDatInd  *datInd;
#endif
{
   S16  ret = ROK;
   SuId  suId;

   TRC2(rgUIMSndCmnDatInd)

   suId = rgRguUlSap;


   rgCb.rguSap[suId].sapSts.numPduTxmit++;

   ret = RgUiRguCDatInd(&rgCb.rguSap[suId].sapCfg.sapPst, rgCb.rguSap[suId].sapCfg.suId, 
         datInd);
   if (ret != ROK)
   {
      RLOG_ARG0(L_ERROR,DBG_CELLID,datInd->cellId,"RgUiRguCDatInd Failed");
      RETVALUE(ret);
   }
   RETVALUE(ret);
}  /* rgUIMSndCmnDatInd */

/**

 * @brief API for bind request from RRC towards MAC. 
 *
 * @details
 *
 *     Function: RgUiCrgBndReq
 *     
 *     This API is invoked by RRC towards MAC to bind CRG SAP. 
 *     These API validates the Pst, spId, suId and sends the bind confirm to RRC.
 *
 *           
 *  @param[in]  Pst   *pst
 *  @param[in]  SuId  suId
 *  @param[in]  SpId  spId
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED 
 **/
#ifdef ANSI
PUBLIC S16 RgUiCrgBndReq
(
Pst   *pst, 
SuId  suId,
SpId  spId
)
#else
PUBLIC S16 RgUiCrgBndReq(pst, suId, spId)
Pst   *pst; 
SuId  suId;
SpId  spId;
#endif
{
   S16       ret = ROK;
   Pst       tmpPst;   /* Temporary Post Structure */
   RgUstaDgn dgn;      /* Alarm diagnostics structure */

   TRC3(RgUiCrgBndReq)


   tmpPst.prior       = pst->prior;
   tmpPst.route       = pst->route;
   tmpPst.selector    = pst->selector;
   tmpPst.region      = rgCb.rgInit.region;
   tmpPst.pool        = rgCb.rgInit.pool;
   tmpPst.srcProcId   = rgCb.rgInit.procId;
   tmpPst.srcEnt      = rgCb.rgInit.ent;
   tmpPst.srcInst     = rgCb.rgInit.inst;
   tmpPst.event       = EVTNONE;
   tmpPst.dstProcId   = pst->srcProcId;
   tmpPst.dstEnt      = pst->srcEnt;
   tmpPst.dstInst     = pst->srcInst;


   if(spId == rgCb.crgSap.sapCfg.spId)
   {
      /* Check the state of the SAP */
      switch (rgCb.crgSap.sapSta.sapState)
      {
         case LRG_NOT_CFG: /* SAP Not configured */
            RLOG0(L_DEBUG,"SAP Not Configured");
            rgFillDgnParams(&dgn, LRG_USTA_DGNVAL_MEM); 
            ret = rgLMMStaInd(LCM_CATEGORY_INTERFACE,LRG_NOT_CFG,
                  LCM_CAUSE_INV_SAP, &dgn);
            ret = RgUiCrgBndCfm(&tmpPst, suId, CM_BND_NOK);
            break;
         case LRG_UNBND: /* SAP is not bound */
            RLOG0(L_DEBUG,"SAP Not yet bound");
            rgCb.crgSap.sapSta.sapState = LRG_BND;
            rgCb.crgSap.sapCfg.suId = suId;
            /* Send Bind Confirm with status as SUCCESS */
            ret = rgUIMCrgBndCfm(suId, CM_BND_OK);
            /* Indicate to Layer manager */
            rgFillDgnParams(&dgn, LRG_USTA_DGNVAL_MEM); 
            ret = rgLMMStaInd(LCM_CATEGORY_INTERFACE,LRG_EVENT_CRGSAP_ENB,
                  LCM_CAUSE_UNKNOWN, &dgn);
            break;
         case LRG_BND: /* SAP is already bound*/
            RLOG0(L_DEBUG,"SAP is already bound");
            ret = rgUIMCrgBndCfm(suId, CM_BND_OK);
            break;
         default: /* Should Never Enter here */
#if (ERRCLASS & ERRCLS_ADD_RES)      
            RLOG1(L_ERROR,"Invalid SAP State:%d RgUiCrgBndReq failed",
                  rgCb.crgSap.sapSta.sapState);
#endif
            ret = rgUIMCrgBndCfm(suId, CM_BND_NOK);
            break;
      }
   }
   else
   {
#if (ERRCLASS & ERRCLS_ADD_RES)      
      RLOG1(L_ERROR,"Invalid SAP Id:%d RgUiCrgBndReq failed",
           rgCb.crgSap.sapCfg.spId);
#endif
      ret = rgUIMCrgBndCfm(suId, CM_BND_NOK);
   }
   RETVALUE(ret);
}  /* RgUiCrgBndReq */


/**
 * @brief API for unbind request from RRC towards MAC. 
 *
 * @details
 *
 *     Function: RgUiCrgUbndReq
 *     
 *     This API is invoked by RRC towards MAC to unbind CRG SAP. 
 *     These API validates the Pst, spId, suId and sends the bind confirm to RRC.
 *
 *           
 *  @param[in]  Pst    *pst
 *  @param[in]  SuId   suId
 *  @param[in]  Reason reason
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED 
 **/
#ifdef ANSI
PUBLIC S16 RgUiCrgUbndReq
(
Pst    *pst,
SpId   spId,
Reason reason
)
#else
PUBLIC S16 RgUiCrgUbndReq(pst, spId, reason)
Pst    *pst; 
SpId   spId;
Reason reason;
#endif
{
   TRC3(RgUiCrgUbndReq)


   /* SAP Id validation */
   if (spId == rgCb.crgSap.sapCfg.spId)
   {
      switch(rgCb.crgSap.sapSta.sapState)
      {
         case LRG_BND: /* SAP is already bound*/
            RLOG0(L_DEBUG,"SAP is already bound");
            /* setting SAP state to UN BOUND */
            rgCb.crgSap.sapSta.sapState = LRG_UNBND;
            break;
         default:
#if (ERRCLASS & ERRCLS_ADD_RES)      
            RLOG1(L_ERROR,"Invalid SAP State:%d RgUiCrgUbndReq failed",
                  rgCb.crgSap.sapSta.sapState);
#endif
            RETVALUE(RFAILED);
      }
   }
   else
   {
#if (ERRCLASS & ERRCLS_ADD_RES)      
      RLOG1(L_ERROR,"Invalid SAP Id:%d RgUiCrgUbndReq failed",
            rgCb.crgSap.sapCfg.spId);
#endif
      RETVALUE(RFAILED);
   }
   RETVALUE(ROK);
}  /* RgUiCrgUbndReq */

/**
 * @brief API for sending bind confirm from MAC to RRC
 *
 * @details
 *
 *     Function: rgUIMRgrBndCfm
 *     
 *     This API is invoked to send bind confirm from MAC to RRC.
 *     This API fills in Pst structure and SAP Ids and invokes 
 *     bind confirm API towards RRC.
 *           
 *  @param[in]  SuId          suId
 *  @param[in]  U8            status
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED 
 **/
#ifdef ANSI
PUBLIC S16 rgUIMCrgBndCfm
(
SuId suId,
U8 status
)
#else
PUBLIC S16 rgUIMCrgBndCfm(suId, status)
SuId          suId;
U8            status;
#endif
{
   TRC2(rgUIMCrgBndCfm)
   

   if(RgUiCrgBndCfm(&rgCb.crgSap.sapCfg.sapPst, rgCb.crgSap.sapCfg.suId, status) != ROK)
   {
      RLOG0(L_ERROR,"RgUiCrgBndCfm Failed ");
      RETVALUE(RFAILED);
   }

   RETVALUE(ROK);
}  /* rgUIMCrgBndCfm*/

/**
 * @brief API for configuration request from RRC towards MAC. 
 *
 * @details
 *
 *     Function: RgUiCrgCfgReq
 *     
 *     This API is invoked by RRC towards MAC to configure MAC. 
 *     These API validates the Pst, spId, suId and transfers the config request 
 *     specific information to corresponding ownership module (COM) API.
 *
 *           
 *  @param[in]  Pst           *pst
 *  @param[in]  SpId          spId
 *  @param[in]  CrgCfgTransId transId
 *  @param[in]  CrgCfgReqInfo *cfgReqInfo
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED 
 **/
#ifdef ANSI
PUBLIC S16 RgUiCrgCfgReq
(
Pst           *pst, 
SpId          spId,
CrgCfgTransId transId,
CrgCfgReqInfo *cfgReqInfo
)
#else
PUBLIC S16 RgUiCrgCfgReq(pst, spId, transId, cfgReqInfo)
Pst           *pst; 
SpId          spId;
CrgCfgTransId transId;
CrgCfgReqInfo *cfgReqInfo;
#endif
{
   S16       ret       = ROK;
   U8        cfmStatus = 0x00ff;
   U8        prntTrans[CRG_CFG_TRANSID_SIZE+1];

   TRC3(RgUiCrgCfgReq);

   /* Ensuring transId is always Null terminated. */
   cmMemcpy((U8 *)prntTrans, (U8 *)transId.trans, CRG_CFG_TRANSID_SIZE);
   prntTrans[CRG_CFG_TRANSID_SIZE] = '\0';


   /* CrgCfgReqInfo Validation for NULLP */
   if (cfgReqInfo == NULLP)
   {
      RLOG0(L_ERROR,"Input Param crgReqInfo is NULL ");
      rgUIMCrgCfgCfm(transId, cfmStatus); 
      RETVALUE(RFAILED);
   }

   /* Upper SAP Id and State validation */
   if (spId == rgCb.crgSap.sapCfg.spId)
   {
      switch(rgCb.crgSap.sapSta.sapState)
      {
         case LRG_BND: /* SAP is already bound */
            RLOG0(L_DEBUG,"SAP is already bound");
            break;
         default: /* Should never reach here */
#if (ERRCLASS & ERRCLS_ADD_RES)      
            RLOG1(L_ERROR,"Invalid SAP State:%d RgUiCrgCfgReq failed",
                  rgCb.crgSap.sapSta.sapState);
#endif
         SPutSBuf (pst->region, pst->pool, (Data *)cfgReqInfo,
               sizeof(CrgCfgReqInfo));
         cfgReqInfo = NULLP;

            rgUIMCrgCfgCfm(transId, cfmStatus);
            RETVALUE(RFAILED);
      }
   }
   else
   {
#if (ERRCLASS & ERRCLS_ADD_RES)      
      RLOG1(L_ERROR,"Invalid SAP Id:%d RgUiCrgCfgReq failed",
            rgCb.crgSap.sapCfg.spId);
#endif
      SPutSBuf (pst->region, pst->pool, (Data *)cfgReqInfo,
            sizeof(CrgCfgReqInfo));
      cfgReqInfo = NULLP;
      rgUIMCrgCfgCfm(transId, cfmStatus); 
      RETVALUE(RFAILED);
   }
   ret = rgCOMCfgReq(transId, cfgReqInfo);
   SPutSBuf (pst->region, pst->pool, (Data *)cfgReqInfo,
         sizeof(CrgCfgReqInfo));
   cfgReqInfo = NULLP;
   if (ret != ROK)
   {
      RLOG0(L_ERROR,"Configuration Request Handling Failed ");
      RETVALUE(RFAILED);
   }

   RETVALUE(ROK);
}  /* RgUiCrgCfgReq */

/**
 * @brief API for sending configuration confirm from MAC to RRC
 *
 * @details
 *
 *     Function: rgUIMCrgCfgCfm
 *     
 *     This API is invoked to send configuration confirm from MAC to RRC.
 *     This API fills in Pst structure and SAP Ids and invokes 
 *     config confirm API towards RRC.
 *           
 *  @param[in]  CrgCfgTransId transId
 *  @param[in]  U8            status
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED 
 **/
#ifdef ANSI
PUBLIC S16 rgUIMCrgCfgCfm
(
CrgCfgTransId transId,
U8            status
)
#else
PUBLIC S16 rgUIMCrgCfgCfm(transId, status)
CrgCfgTransId transId;
U8            status;
#endif
{
   S16  ret = ROK;
   U8   prntTrans[CRG_CFG_TRANSID_SIZE+1];

   TRC2(rgUIMCrgCfgCfm)
   
   cmMemcpy((U8 *)prntTrans, (U8 *)transId.trans, CRG_CFG_TRANSID_SIZE);
   prntTrans[CRG_CFG_TRANSID_SIZE] = '\0';


   ret = RgUiCrgCfgCfm(&rgCb.crgSap.sapCfg.sapPst, rgCb.crgSap.sapCfg.suId, transId, status);
   if (ret != ROK)
   {
      RLOG0(L_ERROR,"RgUiCrgCfgCfm Failed ");
      RETVALUE(ret);
   }

   RETVALUE(ret);
}  /* rgUIMCrgCfgCfm */
#if defined(SPLIT_RLC_DL_TASK) && defined(RLC_MAC_STA_RSP_RBUF)

#ifdef ANSI
PUBLIC S16 rgBatchProc
(
Void
)
#else
PUBLIC S16 rgBatchProc()
Void;
#endif
{
/* Read from Ring Buffer and process RLC BO Update */
   Pst pst = {0};
   SpId spId = 0;
   RguDStaRspInfo  *staRsp;
   U32 elmIndx = 0;

/* Fill pst */
   pst.srcProcId = 1;
   pst.dstProcId = 1;
   pst.dstEnt = ENTRG;
   pst.dstInst = 0;
   pst.srcEnt = ENTKW;
   pst.srcInst = 1;
   pst.prior = PRIOR0;
   pst.route = RTESPEC;
   pst.event = EVTRGUDSTARSP;
   pst.region = 0;
   pst.pool = 0;
   pst.selector = 2; /*SM_SELECTOR_LC */
  
   elmIndx = (U32)SRngGetRIndx(SS_RNG_BUF_DLRLC_TO_DLMAC);
   while(NULLP != elmIndx)
   {
      staRsp = (RguDStaRspInfo *)elmIndx;
      RgUiRguDStaRsp(&pst, spId, staRsp);

      elmIndx = NULLP;
      staRsp = NULLP;
      SRngIncrRIndx(SS_RNG_BUF_DLRLC_TO_DLMAC);

      if((elmIndx = (U32)SRngGetRIndx(SS_RNG_BUF_DLRLC_TO_DLMAC)) == NULLP)
      break;
   }
   RETVALUE(ROK);
}
#endif

/**********************************************************************
 
         End of file:     gk_uim.c@@/main/3 - Sat Jul 30 02:21:57 2011
 
**********************************************************************/
 
/**********************************************************************
 
        Notes:
 
**********************************************************************/
 
/**********************************************************************
 
        Revision history:
 
**********************************************************************/
/********************************************************************90**
 
     ver       pat      init                  description
------------ --------   ----  ----------------------------------------------
/main/1      ---        sd           1. Initial Release.
/main/2      ---        sd           1. LTE MAC 2.1 release
         rg003.201      ns           1. Trace added for functions.
         rg004.201      ms           1. Statistics update of the DL SDUs/PDUs
         rg005.201      sm           1. moved some validations under errcls for
                                        optimization.
         rg008.201   rsharon         1. Removed commented code.
/main/3      ---        gvj  1. Updated for LTE MAC Rel 3.1           
$SID$        ---        rt   1. LTE MAC 4.1 release
*********************************************************************91*/
