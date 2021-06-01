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



/********************************************************************20**
  
        Name:    LTE-RLC Layer - Lower Interface Functions
    
        Type:    C file
  
     Desc:     Source code for RLC Lower Interface Module.
               This file contains following functions
      
        --KwLiRguBndCfm
        --KwLiRguCDatInd
        --KwLiRguDDatInd
        --KwLiRguCStaInd
        --KwLiRguDStaInd
        --KwLiRguHqStaInd

     File:     gp_lim.c

     Sid:      gp_lim.c@@/main/TeNB_Main_BR/TeNB_61750_TDD_EA2_4.0.0_SON/1 - Tue Jan 13 11:32:00 2015

     Prg:      gk 

**********************************************************************/
static const char* RLOG_MODULE_NAME="LIM";
static int RLOG_MODULE_ID=2048;
static int RLOG_FILE_ID=196;

/** 
 * @file gp_lim.c
 * @brief RLC Lower Interface module
*/

#define KW_MODULE KW_DBGMASK_INF


/*
 *      This software may be combined with the following TRILLIUM
 *      software:
 *
 *      part no.                      description
 *      --------    ----------------------------------------------
 *      1000241     LTE-MAC
 *      1000369     LTE-RRC
 */


/* header (.h) include files */
#include "envopt.h"        /* environment options */
#include "envdep.h"        /* environment dependent */
#include "envind.h"        /* environment independent */

#include "gen.h"           /* general */
#include "ssi.h"           /* system services */
#include "cm5.h"           /* common timer defines */
#include "cm_tkns.h"       /* common tokens defines */
#include "cm_mblk.h"       /* common memory allocation library defines */
#include "cm_llist.h"      /* common link list  defines  */
#include "cm_hash.h"       /* common hash list  defines */
#include "cm_lte.h"        /* common LTE defines */
#include "lkw.h"           /* LKW defines */
#include "ckw.h"           /* CKW defines */
#include "kwu.h"           /* KWU defines */
#include "rgu.h"           /* RGU defines */
#include "kw_err.h"
#include "kw_env.h"        /* RLC environment options */

#include "kw.h"            /* RLC defines */
#include "kw_udx.h"
#include "kw_dl.h"
#include "kw_ul.h"

/* extern (.x) include files */
#include "gen.x"           /* general */
#include "ssi.x"           /* system services */

#include "cm5.x"           /* common timer library */
#include "cm_tkns.x"       /* common tokens */
#include "cm_mblk.x"       /* common memory allocation */
#include "cm_llist.x"      /* common link list */
#include "cm_hash.x"       /* common hash list */
#include "cm_lte.x"        /* common LTE includes */
#include "cm_lib.x"        /* common memory allocation library */
#include "lkw.x"           /* LKW */
#include "ckw.x"           /* CKW */
#include "kwu.x"           /* KWU */
#include "rgu.x"           /* RGU */

#include "kw.x"
#include "kw_udx.x"
#include "kw_dl.x"
#include "kw_ul.x"

#ifdef __cplusplus
EXTERN "C" {
#endif /* __cplusplus */


/*****************************************************************************
 *                          RGU INTERFACE
 ****************************************************************************/
/**
 * @brief Handler for bind confirmation from MAC.
 *
 * @details
 *    This function handles the bind confirmation received from MAC. If the 
 *    bind was successful changes the state of the SAP to KW_SAP_BND 
 *    else KW_SAP_CFG. Sends an alarm to LM in any case
 *
 * @param[in] pst     Post structure
 * @param[in] suId    Service User ID
 * @param[in] status  Status whether the bind was successful or not
 *
 * @return  S16
 *    -# ROK 
 *    -# RFAILED 
 *
*/
#ifdef ANSI
PUBLIC S16 KwLiRguBndCfm
(
Pst    *pst,   
SuId   suId, 
U8     status 
)
#else
PUBLIC S16 KwLiRguBndCfm (pst, suId, status)
Pst    *pst; 
SuId   suId;
U8     status;
#endif
{
   U16          event;     /* Event */
   U16          cause;     /* Cause */
   KwRguSapCb   *rguSap;   /* RGU SAP Control Block */
   KwCb         *tKwCb;

   TRC3(KwLiRguBndCfm)

#if (ERRCLASS & ERRCLS_INT_PAR)
   if (pst->dstInst >= KW_MAX_RLC_INSTANCES)
   {
      RETVALUE (RFAILED);
   }
#endif
   tKwCb = KW_GET_KWCB(pst->dstInst);

   RLOG2(L_DEBUG,"KwLiRguBndCfm(suId(%d), status(%d)", suId, status);

#if (ERRCLASS & ERRCLS_INT_PAR)
   if (tKwCb->init.cfgDone != TRUE)
   {
      RLOG0(L_FATAL,"General configuration not done");
      
      KW_SEND_SAPID_ALARM(tKwCb,suId,LKW_EVENT_LI_BND_CFM, LCM_CAUSE_INV_STATE);

      RETVALUE(RFAILED);
   }

   if (suId < 0)
   {
      RLOG0(L_ERROR, "Invalid suId");
      
      KW_SEND_SAPID_ALARM(tKwCb,suId, LKW_EVENT_LI_BND_CFM, LCM_CAUSE_INV_SUID);

      RETVALUE(RFAILED);
   }
#endif /* ERRCLASS & ERRCLS_INT_PAR */

   rguSap = (tKwCb->genCfg.rlcMode == LKW_RLC_MODE_DL) ?
            &(tKwCb->u.dlCb->rguDlSap) : &(tKwCb->u.ulCb->rguUlSap);

   RLOG1(L_DEBUG, "KwLiRguBndCfm: For RGU SAP state=%d", rguSap->state)

   switch (rguSap->state)
   {
      case KW_SAP_BINDING:
      {
         kwStopTmr (tKwCb,(PTR)rguSap, KW_EVT_WAIT_BNDCFM);

         rguSap->retryCnt = 0;
          
         if (status == CM_BND_OK)
         {
            rguSap->state = KW_SAP_BND;
            event = LCM_EVENT_BND_OK;
            cause = LKW_CAUSE_SAP_BNDENB;
         }
         else
         {
            rguSap->state = KW_SAP_CFG;
            event = LCM_EVENT_BND_FAIL;
            cause = LKW_CAUSE_UNKNOWN;
         }

         break;
      }
      default:
         event = LKW_EVENT_RGU_BND_CFM;
         cause = LCM_CAUSE_INV_STATE;
         break;
   }

   /* Send an alarm with proper event and cause */
   KW_SEND_SAPID_ALARM(tKwCb, suId, event, cause);

   RETVALUE(ROK);
} /* KwLiRguBndCfm */

PUBLIC int   rlcDDatIndRcvd;
PUBLIC int   rlcCDatIndRcvd;
/**
 * @brief Handler to process PDU received from MAC for common logical channels. 
 *
 * @details
 *    This function receives the PDU from MAC for common logical channels
 *    does checks before handing over the PDU to the TM module
 *
 * @param[in] pst     Post structure
 * @param[in] suId    Service User ID
 * @param[in] datInd  Data Indication Information 
 *
 * @return  S16
 *    -# ROK 
 *    -# RFAILED 
 *
*/
#ifdef ANSI
PUBLIC S16 KwLiRguCDatInd
(
Pst              *pst,   
SuId             suId,  
RguCDatIndInfo   *datInd
)
#else
PUBLIC S16 KwLiRguCDatInd(pst,suId,datInd)
Pst              *pst; 
SuId             suId;
RguCDatIndInfo   *datInd;
#endif
{
   KwUlRbCb   *rbCb; 
   KwCb       *tKwCb; 

   rlcCDatIndRcvd++;
   TRC3(KwLiRguCDatInd)

#if (ERRCLASS & ERRCLS_INT_PAR)
   if (pst->dstInst >= KW_MAX_RLC_INSTANCES)
   {
      KW_SHRABL_STATIC_BUF_FREE(pst->region, pst->pool, datInd, sizeof(RguCDatIndInfo));
      RETVALUE (RFAILED);
   }
#endif

   tKwCb = KW_GET_KWCB(pst->dstInst);


#if (ERRCLASS & ERRCLS_DEBUG)
   if (tKwCb->genCfg.rlcMode == LKW_RLC_MODE_DL)
   {
      KW_SHRABL_STATIC_BUF_FREE(pst->region, pst->pool, datInd, sizeof(RguCDatIndInfo));
      RETVALUE(RFAILED);
   }
#endif

   /* kw006.201 ccpu00120058, Added array boundary condition check */
#if (ERRCLASS & ERRCLS_DEBUG)
   if(KW_MAX_LCH_PER_CELL <= datInd->lcId)
   {
      RLOG_ARG1(L_ERROR,DBG_LCID,datInd->lcId, "Invalid LcId, Max is [%d]",
         KW_MAX_LCH_PER_CELL);
      KW_SHRABL_STATIC_BUF_FREE(pst->region, pst->pool, datInd, sizeof(RguCDatIndInfo));
      RETVALUE(RFAILED);
   }
#endif /* (ERRCLASS & ERRCLS_DEBUG) */

   /* Fetch RbCb from lcId */
   kwDbmFetchUlRbCbFromLchId(tKwCb, 0, datInd->cellId, datInd->lcId, &rbCb);
   if (!rbCb)
   {
      RLOG_ARG1(L_ERROR, DBG_CELLID,datInd->cellId, "LcId [%d] not found",
         datInd->lcId);
      KW_SHRABL_STATIC_BUF_FREE(pst->region, pst->pool, datInd, sizeof(RguCDatIndInfo));
      RETVALUE(RFAILED);
   }

   /* Dispatch to TM Module */
#ifdef CCPU_OPT
   kwTmmRcvFrmLi(tKwCb, rbCb, datInd->rnti, datInd->pdu);
#else /* CCPU_OPT */
   kwTmmRcvFrmLi(tKwCb, rbCb, datInd->pdu);
#endif /* CCPU_OPT */

   KW_SHRABL_STATIC_BUF_FREE(pst->region, pst->pool, datInd, sizeof(RguCDatIndInfo));

   RETVALUE(ROK);
} /* KwLiRguCDatInd */

/**
 * @brief Handler to process PDU received from MAC for 
 *        dedicated logical channels. 
 *
 * @details
 *    This function receives the PDU from MAC for one or more dedicated 
 *    logical channels and passes it to the UTL module for further processing
 *
 * @param[in] pst     Post structure
 * @param[in] suId    Service User ID
 * @param[in] datInd  Data Indication Information 
 *
 * @return  S16
 *    -# ROK 
 *    -# RFAILED 
 *
*/
 
#ifdef ANSI
PUBLIC S16 KwLiRguDDatInd
(
Pst              *pst,   
SuId             suId,  
RguDDatIndInfo   *datInd
)
#else
PUBLIC S16 KwLiRguDDatInd(pst, suId, datInd)
Pst              *pst; 
SuId             suId;
RguDDatIndInfo   *datInd;
#endif
{
   TRC3(KwLiRguDDatInd)

   rlcDDatIndRcvd++;
#if (ERRCLASS & ERRCLS_INT_PAR)
   if (pst->dstInst >= KW_MAX_RLC_INSTANCES)
   {
       KW_SHRABL_STATIC_BUF_FREE(pst->region, pst->pool, datInd, sizeof(RguDDatIndInfo));
       RETVALUE (RFAILED);
   }
#endif

#if (ERRCLASS & ERRCLS_DEBUG)
   if (((KwCb*)KW_GET_KWCB(pst->dstInst))->genCfg.rlcMode == LKW_RLC_MODE_DL)
   {
       RLOG1(L_DEBUG,"KwLiRguDDatInd(pst, suId(%d))recieved in DL Inst",suId);
       KW_SHRABL_STATIC_BUF_FREE(pst->region, pst->pool, datInd, sizeof(RguDDatIndInfo));
       RETVALUE(RFAILED);
   }
#endif
   kwUtlRcvFrmLi(KW_GET_KWCB(pst->dstInst),datInd);
#ifndef SS_RBUF 
#ifdef SS_LOCKLESS_MEMORY
   KW_SHRABL_STATIC_BUF_FREE(pst->region, pst->pool, datInd, sizeof(RguDDatIndInfo));
#else
   KW_PST_FREE(pst->region, pst->pool, datInd, sizeof(RguDDatIndInfo));
#endif
#endif

   RETVALUE(ROK);
} /* KwLiRguDDatInd */

/**
 * @brief Handler for trigerring the data transfer from RLC to MAC
 *        for common logical channels.
 *
 * @details
 *    This function receives the size of the PDU to be transmitted
 *    and acts as a trigger for forming PDU and sending it to MAC. 
 *
 * @param[in] pst       Post structure
 * @param[in] suId      Service User ID
 * @param[in] staInd    Status Indication Information for Common Logical 
 *                      Channels
 *
 * @return  S16
 *    -# ROK 
 *    -# RFAILED 
 *
*/ 
#ifdef ANSI
PUBLIC S16 KwLiRguCStaInd
(
Pst              *pst,  
SuId             suId,
RguCStaIndInfo   *staInd  
)
#else
PUBLIC S16 KwLiRguCStaInd(pst,suId,staInd)
Pst              *pst;   
SuId             suId; 
RguCStaIndInfo   *staInd; 
#endif
{
   KwDlRbCb   *rbCb;  
   KwCb       *tKwCb;

   TRC3(KwLiRguCStaInd)

#if (ERRCLASS & ERRCLS_INT_PAR)
   if (pst->dstInst >= KW_MAX_RLC_INSTANCES)
   {
      KW_SHRABL_STATIC_BUF_FREE(pst->region, pst->pool, staInd, sizeof(RguCStaIndInfo));
      RETVALUE (RFAILED);
   }
#endif

   tKwCb = KW_GET_KWCB(pst->dstInst);


#if (ERRCLASS & ERRCLS_INT_PAR)
   if (tKwCb->genCfg.rlcMode == LKW_RLC_MODE_UL)
   {
       RLOG_ARG1(L_ERROR,DBG_LCID,staInd->lcId,
             "Received in RLC UL CELLID:%d",
             staInd->cellId);
       KW_SHRABL_STATIC_BUF_FREE(pst->region, pst->pool, staInd, sizeof(RguCStaIndInfo));
       RETVALUE(RFAILED);
   }
#endif

   rbCb = NULLP;

   /* kw006.201 ccpu00120058, added boundary condition check */
#if (ERRCLASS & ERRCLS_DEBUG)
   if(KW_MAX_LCH_PER_CELL < staInd->lcId)
   {
      RLOG_ARG2(L_ERROR,DBG_LCID,staInd->lcId, 
            "Invalid LcId, Max is [%d] CELLID:%d",
            KW_MAX_LCH_PER_CELL,
            staInd->cellId);
      KW_SHRABL_STATIC_BUF_FREE(pst->region, pst->pool, staInd, sizeof(RguCStaIndInfo));
      RETVALUE(RFAILED);
   }
#endif /* (ERRCLASS & ERRCLS_DEBUG) */
   /* Fertch RbCb from lcId */
   kwDbmFetchDlRbCbFromLchId(tKwCb,0, staInd->cellId, staInd->lcId, &rbCb);
   if(!rbCb)                                               
   {
      RLOG_ARG1(L_ERROR, DBG_CELLID,staInd->cellId, 
            "LcId [%d] not found CELLID:%d",
            staInd->lcId);
      KW_SHRABL_STATIC_BUF_FREE(pst->region, pst->pool, staInd, sizeof(RguCStaIndInfo));
      RETVALUE(RFAILED);
   }

   /* Dispatch to TM Module */
   rbCb->transId = staInd->transId;
   /* ccpu00136940 */
  /* If trace flag is enabled send the trace indication */
   if(tKwCb->init.trc == TRUE)
   {
     /* Populate the trace params */
      kwLmmSendTrc(tKwCb,EVTRGUCSTAIND, NULLP);
   }                            
   kwTmmSndToLi(tKwCb,rbCb, staInd);
#ifndef SS_RBUF
#ifdef SS_LOCKLESS_MEMORY
   KW_SHRABL_STATIC_BUF_FREE(pst->region, pst->pool, staInd, sizeof(RguCStaIndInfo));
#else
   KW_PST_FREE(pst->region, pst->pool, staInd, sizeof(RguCStaIndInfo));
#endif
#else
   KW_SHRABL_STATIC_BUF_FREE(pst->region, pst->pool, staInd, sizeof(RguCStaIndInfo));
#endif
   RETVALUE(ROK);
} /* KwLiRguCStaInd */

/**
 * @brief Handler for trigerring the data transfer from RLC to MAC
 *        for dedicated logical channels.
 *
 * @details
 *    This function receives the size of the PDUs to be transmitted to
 *    MAC via one or more dedicated logical channels and acts as a trigger
 *    for forming PDUs and sending them to MAC. 
 *
 * @param[in] pst       Post structure
 * @param[in] suId      Service User ID
 * @param[in] staInd    Status Indication Information for Dedicated Logical 
 *                      Channels
 *
 * @return  S16
 *    -# ROK 
 *    -# RFAILED 
 *
*/ 
#ifdef ANSI
PUBLIC S16 KwLiRguDStaInd
(
Pst              *pst,  
SuId             suId,
RguDStaIndInfo   *staInd 
)
#else
PUBLIC S16 KwLiRguDStaInd(pst, suId, staInd)
Pst              *pst; 
SuId             suId; 
RguDStaIndInfo   *staInd; 
#endif
{
   TRC3(KwLiRguDStaInd)

#if (ERRCLASS & ERRCLS_INT_PAR)
   if (pst->dstInst >= KW_MAX_RLC_INSTANCES)
   {
      KW_SHRABL_STATIC_BUF_FREE(pst->region, pst->pool, staInd, sizeof(RguDStaIndInfo));
      RETVALUE (RFAILED);
   }

   if (((KwCb*)KW_GET_KWCB(pst->dstInst))->genCfg.rlcMode == LKW_RLC_MODE_UL)
   {
       RLOG_ARG0(L_ERROR,DBG_CELLID,staInd->cellId,"Received in RLC UL ");
       KW_SHRABL_STATIC_BUF_FREE(pst->region, pst->pool, staInd, sizeof(RguDStaIndInfo));
       RETVALUE(RFAILED);
   }
#endif
   kwUtlSndToLi(KW_GET_KWCB(pst->dstInst), staInd);

   /* kw002.201 :Freeing from proper region */
   KW_SHRABL_STATIC_BUF_FREE(pst->region, pst->pool, staInd, sizeof(RguDStaIndInfo));
   RETVALUE(ROK);
} /* KwLiRguDStaInd */

/**
 * @brief Handler for handling the flow cntrl Ind from MAC
 *  to RLC      
 *
 * @details
 *   This function receives the flow control indication from
 *   MAC and calls kwUtlTrigPdbFlowCntrl
 *
 * @param[in] pst       Post structure
 * @param[in] suId      Service User ID
 * @param[in] flowCntrlInd flow control Indication Information 
 *           from MAC  
 *
 * @return  S16
 *    -# ROK 
 *    -# RFAILED 
 *
*/ 
#ifdef ANSI
PUBLIC S16 KwLiRguFlowCntrlInd
(
Pst              *pst,  
SuId             suId,
RguFlowCntrlInd *flowCntrlInd
)
#else
PUBLIC S16 KwLiRguFlowCntrlInd(pst, suId, flowCntrlInd)
Pst              *pst; 
SuId             suId; 
RguFlowCntrlInd  *flowCntrlInd; 
#endif
{
   KwCb       *tKwCb; 
   KwDlRbCb   *rbCb = NULLP;
   U32        idx;
   U32        lcIdx;

   tKwCb = KW_GET_KWCB(pst->dstInst);
   for (idx = 0; idx < flowCntrlInd->numUes; idx++)
   {
      for (lcIdx = 0; lcIdx < flowCntrlInd->ueFlowCntrlInfo[idx].numLcs; lcIdx++)  
      {
         RguLcFlowCntrlInfo *lcInfo = &(flowCntrlInd->ueFlowCntrlInfo[idx].lcInfo[lcIdx]); 
         kwDbmFetchDlRbCbFromLchId(tKwCb, flowCntrlInd->ueFlowCntrlInfo[idx].ueId, flowCntrlInd->cellId, lcInfo->lcId, &rbCb);
         if (rbCb)
         {
           
            if (lcInfo->pktAdmitCnt == 0) /* Special case */
            {
               kwUtlTrigPdbFlowCntrl(tKwCb, rbCb, lcInfo->pktAdmitCnt);
               continue;
            }
            if (rbCb->mode == CM_LTE_MODE_AM)
            {
               if ((rbCb->m.amDl.retxLst.count != 0) || 
                   ((rbCb->m.amDl.bo == 0) || 
                    (rbCb->m.amDl.bo < lcInfo->maxBo4FlowCtrl)))
               {
                  continue;
               }
            }
            else /* UM */
            {
               if ((rbCb->m.umDl.bo == 0) ||
                   (rbCb->m.umDl.bo < lcInfo->maxBo4FlowCtrl))
               {
                  continue;
               }
            }
            kwUtlTrigPdbFlowCntrl(tKwCb, rbCb, lcInfo->pktAdmitCnt);
         }
      }
   }
   RETVALUE(ROK);
}
/* kw005.201 added support for L2 Measurement */
#ifdef LTE_L2_MEAS

/**
 *
 * @brief  
 *
 *        Handler for indicating the Harq Status of the data sent.
 *
 * @b Description:
 *
 *        This function receives the harq status of the data sent to MAC.
 *        This information is used for two things.
 *        1. Computing the UuLoss of UM
 *        2. Computing the DL Delay for UM and AM.
 *
 *  @param[in] pst     -   Post structure  
 *  @param[in] suId    -   Service User ID
 *  @param[in] staInd  -   Harq Status Indication Information. 
 *
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED
 *
 */
#ifdef ANSI
PUBLIC S16 KwLiRguHqStaInd
(
Pst                *pst, 
SuId               suId,
RguHarqStatusInd   *staInd 
)
#else
PUBLIC S16 KwLiRguHqStaInd(pst,suId,staInd)
Pst                *pst;  
SuId               suId; 
RguHarqStatusInd   *staInd;
#endif
{

   KwUeKey   ueKey;
   S16       ret; 
   KwDlUeCb    *ueCb;
   U8        tbIdx;
   KwCb       *tKwCb; 

   TRC3(KwLiRguHqStaInd)

   tKwCb = KW_GET_KWCB(pst->dstInst);
   ueKey.cellId = staInd->cellId;
   ueKey.ueId   = staInd->ueId;

   ret = kwDbmFetchDlUeCb(tKwCb, ueKey.ueId, ueKey.cellId, &ueCb);
   if (ret != ROK )
   {
      RETVALUE(RFAILED);
   }

   /*Call kwUtlProcHarqInd as many times as number of Tbs present*/
   for ( tbIdx = 0; tbIdx < staInd->numTbs; tbIdx++)
   {
      kwUtlProcHarqInd(tKwCb, staInd, ueCb, tbIdx);
   }

   RETVALUE(ROK);
} /* KwLiRguHqStaInd */
#endif /* LTE_L2_MEAS */

#ifdef __cplusplus
}
#endif /* __cplusplus */

/********************************************************************30**
         End of file:     gp_lim.c@@/main/TeNB_Main_BR/TeNB_61750_TDD_EA2_4.0.0_SON/1 - Tue Jan 13 11:32:00 2015
*********************************************************************31*/


/********************************************************************40**
  
        Notes:
  
*********************************************************************41*/

/********************************************************************50**

*********************************************************************51*/

   
/********************************************************************60**
  
        Revision history:
  
*********************************************************************61*/

/********************************************************************90**
     ver       pat    init                  description
------------ -------- ---- ----------------------------------------------
/main/1      ---       gk   1. initial release.
/main/2      ---       nm   1. LTERLC Release 2.1
/main/3      kw002.201 vp   1. Patch for optimization,fixes for multi-region
                               and retransmission.
/main/4      kw005.201 ap   1. Changed the for loop condition in KwLiRguHqStaInd
                               for calling kwUtlProcHarqInd as many times as
                               number of TBs present.
                            2. Added support for L2 Measurement.
                       rd   3. Updated with important Avtecs comments.
*********************************************************************91*/
