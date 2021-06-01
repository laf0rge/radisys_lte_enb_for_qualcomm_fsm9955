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
  
     File:     rg_sch.c
  
     Sid:      gk_sch.c@@/main/3 - Sat Jul 30 02:21:34 2011
  
     Prg:      SY
  
**********************************************************************/

/** @file rg_sch.c
@brief This file implements the schedulers main access to MAC layer code.
*/

static const char* RLOG_MODULE_NAME="MAC";
static int RLOG_FILE_ID=185;
static int RLOG_MODULE_ID=4096;

/* header include files -- defines (.h) */
#include "envopt.h"        /* environment options */
#include "envdep.h"        /* environment dependent */
#include "envind.h"        /* environment independent */
#include "gen.h"           /* general layer */
#include "ssi.h"           /* system service interface */
#include "cm_hash.h"       /* common hash list */
#include "cm_llist.h"      /* common linked list library */
#include "cm_err.h"        /* common error */
#include "cm_lte.h"        /* common LTE */
#include "lrg.h"
#include "rgr.h"
#include "tfu.h"
#include "rgm.h"
#include "rg_env.h"
#include "rg_sch_inf.h"
#include "rg_sch.h"
#include "rg_sch_err.h"
#include "rg_sch_cmn.h"
#include "rl_interface.h"
#include "rl_common.h"

/* header/extern include files (.x) */
#include "gen.x"           /* general layer typedefs */
#include "ssi.x"           /* system services typedefs */
#include "cm5.x"           /* common timers */
#include "cm_hash.x"       /* common hash list */
#include "cm_lib.x"        /* common library */
#include "cm_llist.x"      /* common linked list */
#include "cm_mblk.x"       /* memory management */
#include "cm_tkns.x"       /* common tokens */
#include "cm_lte.x"       /* common tokens */
#include "tfu.x"           /* TFU types */
#include "lrg.x"           /* layer management typedefs for MAC */
#include "rgr.x"           /* layer management typedefs for MAC */
#include "rgm.x"           /* layer management typedefs for MAC */
#include "rg_sch_inf.x"         /* typedefs for Scheduler */
#include "rg_sch.x"        /* typedefs for Scheduler */
#include "rg_sch_cmn.x"

/* local defines */

/************** LRG Interface ****************/
/**
 * @brief Layer Manager Configuration request handler. 
 *
 * @details
 *
 *     Function : RgMiLrgSchCfgReq
 *     
 *     This function handles the configuration
 *     request received at scheduler instance from the Layer Manager.
 *     -# Based on the cfg->hdr.elmId.elmnt value it invokes one of the
 *        functions rgHdlGenCfg() or rgHdlSapCfg().
 *     -# Invokes RgMiLrgSchCfgCfm() to send back the confirmation to the LM.
 *     
 *  @param[in]  Pst *pst, the post structure     
 *  @param[in]  RgMngmt *cfg, the configuration parameter's structure
 *  @return  S16
 *      -# ROK
 **/
#ifdef ANSI
PUBLIC S16 RgMiLrgSchCfgReq
(
Pst      *pst,    /* post structure  */
RgMngmt  *cfg     /* config structure  */
)
#else
PUBLIC S16 RgMiLrgSchCfgReq(pst, cfg)
Pst      *pst;    /* post structure  */
RgMngmt  *cfg;    /* config structure  */
#endif    
{
   U16       ret = LCM_PRIM_OK;
   U16       reason = LCM_REASON_NOT_APPL;
   RgMngmt   cfm;
   Pst       cfmPst;
#ifdef DEBUGP
   Inst      inst = (pst->dstInst  - RGSCH_INST_START);
#endif

   TRC3(RgMiLrgSchCfgReq)
   

   if(pst->dstInst < RGSCH_INST_START)
   {
      RLOG_ARG0(L_ERROR,DBG_INSTID,inst, "Invalid inst ID");
      RETVALUE(ROK);
   }

   /* Fill the post structure for sending the confirmation */
   rgSCHLmmFillCfmPst(pst, &cfmPst, cfg);

   /* Initialize the cfg cfm structure 
   if (SGetSBuf(cfmPst.region, cfmPst.pool, (Data **)&cfm, sizeof(RgMngmt))
      != ROK)
   {
      RLOG_ARG0(L_ERROR,DBG_INSTID,inst, "Memory Unavailable for Confirmation");
      RETVALUE(ROK);
   } */
   cmMemset((U8 *)&cfm, 0, sizeof(RgMngmt));

#ifdef LMINT3
   cfm.hdr.transId =
      cfg->hdr.transId;
#endif

   cfm.hdr.elmId.elmnt = cfg->hdr.elmId.elmnt;
   switch(cfg->hdr.elmId.elmnt)
   {
      case STSCHINST:
         reason = rgSCHLmmInstCfg(&cfg->t.cfg,pst->dstInst );
         break;
      default:
         ret = LCM_PRIM_NOK;
         reason = LCM_REASON_INVALID_ELMNT;
         RLOG_ARG1(L_ERROR,DBG_INSTID,inst, "Invalid Elmnt=%d", cfg->hdr.elmId.elmnt);
         break;
   }

   if (reason != LCM_REASON_NOT_APPL)
   {
      ret = LCM_PRIM_NOK;
   }

   cfm.cfm.status = ret;
   cfm.cfm.reason = reason;

   RgMiLrgSchCfgCfm(&cfmPst, &cfm);
   /*   SPutSBuf(pst->region, pst->pool, (Data *)cfg, sizeof(RgMngmt)); */
   
   RETVALUE(ROK);
}/*-- RgMiLrgSchCfgReq --*/


/**
 * @brief Layer Manager Control request handler. 
 *
 * @details
 *
 *     Function : RgMiLrgSchCntrlReq
 *     
 *     This function handles the control
 *     request received from the Layer Manager.
 *      -# Based on cntrl->hdr.elmId.elmnt, cntrl->t.cntrl.action
 *      and cntrl->t.cntrl.subAction, it performs the appropriate control action
 *      of SAP (enable/disable) and layer shutdown.
 *      -# Invokes the RgMiLrgSchCntrlCfm to send back the confirmation to LM.
 *     
 *  @param[in]  Pst *pst, the post structure     
 *  @param[in]  RgMngmt *cntrl, the control parameter's structure
 *  @return  S16
 *      -# ROK
 **/
#ifdef ANSI
PUBLIC S16 RgMiLrgSchCntrlReq
(
Pst      *pst,    /* post structure  */
RgMngmt  *cntrl   /* control structure  */
)
#else
PUBLIC S16 RgMiLrgSchCntrlReq(pst, cntrl)
Pst      *pst;    /* post structure  */
RgMngmt  *cntrl;  /* control structure  */
#endif    
{
   S16       ret = ROK;            /* return value */
   Pst       cfmPst;
   RgMngmt   cfm;
   
   Inst      inst = (pst->dstInst - RGSCH_INST_START); /* Scheduler instance Id */
   TRC3(RgMiLrgSchCntrlReq)
   

   /* Fill the post structure for sending the confirmation */
   rgSCHLmmFillCfmPst(pst, &cfmPst, cntrl);

   /* Initialize the cfg cfm structure 
   if (SGetSBuf(cfmPst.region, cfmPst.pool, (Data **)&cfm, sizeof(RgMngmt))
      != ROK)
   {
      RLOG_ARG0(L_ERROR,DBG_INSTID,inst, "Memory Unavailable for Confirmation");
      SPutSBuf(pst->region, pst->pool, (Data *)cntrl, sizeof(RgMngmt));
      RETVALUE(ROK);
   } */
   cmMemset((U8 *)&cfm, 0, sizeof(RgMngmt));

#ifdef LMINT3
   cfm.hdr.transId =
      cntrl->hdr.transId;
#endif

   cfm.hdr.elmId.elmnt = cntrl->hdr.elmId.elmnt;
   cfm.t.cntrl.action = cntrl->t.cntrl.action;
   cfm.t.cntrl.subAction = cntrl->t.cntrl.subAction;

   /* Check if General Config Done*/
   if(rgSchCb[inst].rgSchInit.cfgDone != TRUE)
   {
      cfm.cfm.status = LCM_PRIM_NOK;
      cfm.cfm.reason = LCM_REASON_GENCFG_NOT_DONE;
      cfm.hdr.elmId.elmnt = cntrl->hdr.elmId.elmnt;
      RgMiLrgSchCntrlCfm(&cfmPst, &cfm);
      RLOG_ARG0(L_ERROR,DBG_INSTID,inst, "Gen Cfg not done.");
      /*      SPutSBuf(pst->region, pst->pool, (Data *)cntrl, sizeof(RgMngmt)); */
      RETVALUE(ROK);
   }

   /* General Config done, process the Control request */   
   switch(cntrl->hdr.elmId.elmnt)
   {
      case STGEN:
         rgSCHLmmGenCntrl(cntrl, &cfm, &cfmPst);
         break;
      case STTFUSAP:
      case STRGRSAP:
         rgSCHLmmSapCntrl(cntrl, &cfm, &cfmPst);
         break;
      default:
         cfm.cfm.status = LCM_PRIM_NOK;
         cfm.cfm.reason = LCM_REASON_INVALID_PAR_VAL;
         RgMiLrgSchCntrlCfm(&cfmPst, &cfm);
         RLOG_ARG1(L_ERROR,DBG_INSTID,inst, "invalid elmnt=%d",
                  cntrl->hdr.elmId.elmnt);
         break;
   }
   /*   SPutSBuf(pst->region, pst->pool, (Data *)cntrl, sizeof(RgMngmt)); */
   RETVALUE(ret);
}/*-- RgMiLrgSchCntrlReq --*/

#ifdef LTE_L2_MEAS
/**
 * @brief Layer Manager L2 Measurement request handler. 
 *
 * @details
 *
 *     Function : RgMiLrgSchL2MeasReq
 *     
 *     This function handles the control
 *     measurement request received from the Layer Manager.
 *     
 *  @param[in]  Pst *pst, the post structure     
 *  @param[in]  LrgSchMeasReqInfo *measInfo, measurement request info
 *  @return  S16
 *      -# ROK
 **/
#ifdef ANSI
PUBLIC S16 RgMiLrgSchL2MeasReq
(
Pst               *pst,     /* post structure  */
LrgSchMeasReqInfo *measInfo /* Meas Req Info */
)
#else
PUBLIC S16 RgMiLrgSchL2MeasReq(pst, measInfo)
   Pst               *pst;     /* post structure  */
   LrgSchMeasReqInfo *measInfo; /* Meas Req Info */
#endif    
{
   Pst                 cfmPst;
   RgSchCellCb         *cell;
   RgSchErrInfo        err;
   S16                 ret = ROK; 
   RgSchCb             *instCb =  &rgSchCb[(pst->dstInst - 1)];
#if (ERRCLASS & ERRCLS_ADD_RES) 
   CmLList             *lnk;
#endif
   U32                 idx;
   RgSchL2MeasCb       *measCb = NULLP;
#ifdef DEBUGP
   Inst      inst = (pst->dstInst - RGSCH_INST_START); /* Scheduler instance Id */
#endif

   err.errType  = 0;
   err.errCause = 0;

   TRC3(RgMiLrgSchL2MeasReq)


   /* Find the cellCb using cellId in measInfo. Iterate through all cells
    * in rgrsapCb in RgschCb */
   cell = NULLP;
   for (idx = 0; idx < instCb->numSaps; idx++)
   {
      if ( instCb->rgrSap[idx].cell->cellId == measInfo->cellId)
      {
         /* got the cell break the loop */
         cell = instCb->rgrSap[idx].cell;
         break;
      }
   }
   /* If no cellCb return Err with Invalid Cell Id */
   if (cell == NULLP)
   {
      rgSchL2mFillCfmPst(pst, &cfmPst, measInfo);
      RGSCHFILLERR(err, RGSCHERR_L2M_MEASREQ, RGSCHERR_SCH_INVALID_CELLID);
      rgSchL2mSndCfm(&cfmPst, NULLP, measInfo, TRUE);
      RLOG_ARG2(L_ERROR,DBG_INSTID,inst, 
               "Meas req Failed.Invalid Cell Id errType(%d) errCause(%d)",
               err.errType, err.errCause);
      RETVALUE(RFAILED);
   }
   /* Validate for Meas Types */
   if ( (ret = rgSCHUtlValidateMeasReq(cell, measInfo, &err)) != ROK)
   {
      rgSchL2mFillCfmPst(pst, &cfmPst, measInfo);
      RGSCHFILLERR(err, RGSCHERR_L2M_MEASREQ, RGSCHERR_SCH_INVALID_MEASTYPE);
      rgSchL2mSndCfm(&cfmPst, NULLP, measInfo, TRUE);
      RLOG_ARG2(L_ERROR,DBG_CELLID,cell->cellId,
               "Meas req Failed.Invalid Measurement Type"
               "errCasue(%d) errType(%d)", err.errType, err.errCause);
      RETVALUE(RFAILED);
   }
   /* Iterate through all meas requests in l2mList in cellCb */
#if (ERRCLASS & ERRCLS_ADD_RES) 
   lnk = cell->l2mList.first;
   while(lnk != NULLP)
   {
      /* Get the MeasCb : RgSchL2MeasCb */
      measCb = (RgSchL2MeasCb *)lnk->node;
      lnk = lnk->next;
      if (measCb->measReq.hdr.transId == measInfo->hdr.transId)
      {
         rgSchL2mFillCfmPst(pst, &cfmPst, measInfo);
         RGSCHFILLERR(err, RGSCHERR_L2M_MEASREQ, RGSCHERR_SCH_DUP_TRANSID);
         rgSchL2mSndCfm(&cfmPst, measCb, measInfo, TRUE);
         RLOG_ARG2(L_ERROR,DBG_CELLID,cell->cellId,
                  "Meas req Failed.Dublicate TransId"
                  "errType(%d) errCause(%d)", err.errType, err.errCause);
         RETVALUE(RFAILED);
      }
   }
#endif
   /* Call L2M Function to store Meas req */
   ret = rgSchL2mMeasReq(cell, measInfo, err);
   if (ret != ROK)
   {
      rgSchL2mFillCfmPst(pst, &cfmPst, measInfo);
      RGSCHFILLERR(err, RGSCHERR_L2M_MEASREQ, RGSCHERR_SCH_L2MEAS_FAILED);
      rgSchL2mSndCfm(&cfmPst, measCb, measInfo, TRUE);
      RLOG_ARG2(L_ERROR,DBG_CELLID,cell->cellId, 
               "Meas req Failed.errType(%d) errCause(%d)",
               err.errType, err.errCause);
      RETVALUE(RFAILED);
   }
   RETVALUE(ret);
} /* -- RRgMiLrgSchL2MeasReq-- */

/**
 * @brief Layer Manager L2 Measurement Stop request handler. 
 *
 * @details
 *
 *     Function : RgMiLrgSchL2MeasStopReq
 *     
 *     This function handles the control
 *     measurement stop request received from the Layer Manager.
 *     
 *  @param[in]  Pst *pst, the post structure     
 *  @param[in]  LrgSchMeasReqInfo *measInfo, measurement stop request info
 *  @return  S16
 *      -# ROK
 **/
#ifdef ANSI
PUBLIC S16 RgMiLrgSchL2MeasStopReq
(
Pst               *pst,     /* post structure  */
LrgSchMeasStopReqInfo *measInfo /* Meas Req Info */
)
#else
PUBLIC S16 RgMiLrgSchL2MeasStopReq(pst, measInfo)
   Pst               *pst;     /* post structure  */
   LrgSchMeasStopReqInfo *measInfo; /* Meas Req Info */
#endif
{
   S16                 ret = ROK;
   RgSchCellCb         *cell = NULLP;
   RgSchCb             *instCb =  &rgSchCb[(pst->dstInst - 1)];
   CmLList             *node = NULLP;
   RgSchL2MeasCb       *measCb = NULLP;
   LrgSchMeasCfmInfo measCfm;
   U8                  idx;


   TRC3(RgMiLrgSchL2MeasStopReq)


   for (idx = 0; idx < instCb->numSaps; idx++)
   {
      if ( instCb->rgrSap[idx].cell->cellId == measInfo->cellId)
      {
         /* got the cell break the loop */
         cell = instCb->rgrSap[idx].cell;
         break;
      }
   }
   if (cell == NULLP)
   {
      RLOG_ARG0(L_ERROR,DBG_CELLID,measInfo->cellId,
               "Stop req Failed.Invalid Cell Id ");
      RETVALUE(RFAILED);
   }
   cmMemset((U8 *)&measCfm, 0, sizeof(LrgSchMeasCfmInfo));
   node = cell->l2mList.first;
   while(node != NULLP)
   {
      measCb = (RgSchL2MeasCb *)(node)->node;

      node = (node)->next;
      cmLListDelFrm(&cell->l2mList, &measCb->measLnk);
      rgSCHUtlFreeSBuf(cell->instIdx, (Data **)&measCb,
            sizeof(RgSchL2MeasCb));    
   }

   if(measInfo->measType & LRG_L2MEAS_AVG_PRB_PER_QCI_UL)
   {
      RgInfL2MeasStopReq measStopReq;
      Pst               pstMac;
      cmMemset((U8 *)&measStopReq, 0, sizeof(RgInfL2MeasStopReq));
      measStopReq.transId  = measInfo->hdr.transId;
      measStopReq.measType = measInfo->measType;
      /* measReq.timePrd  = measInfo->timePrd; */
      measStopReq.cellId   = measInfo->cellId;
      rgSCHUtlGetPstToLyr(&pstMac, &rgSchCb[cell->instIdx], cell->macInst);
      RgSchMacL2MeasStop(&pstMac,&measStopReq);
   }
   else
   { 
      RgMiLrgSchL2MeasStopCfm(&(rgSchCb[cell->instIdx].rgSchInit.lmPst),
            &measCfm);
   }
   RETVALUE(ret);
}/*RgMiLrgSchL2MeasStopReq*/
/**
 * @brief Layer Manager L2 Measurement request handler. 
 *        for Send l2 measurement req
 * @details
 *
 *     Function : RgMiLrgSchL2MeasSendReq
 *     
 *     This function handles the control
 *     measurement send request received from the Layer Manager.
 *     
 *  @param[in]  Pst *pst, the post structure     
 *  @param[in]  LrgSchMeasReqInfo *measInfo, measurement stop request info
 *  @return  S16
 *      -# ROK
 **/
#ifdef ANSI
PUBLIC S16 RgMiLrgSchL2MeasSendReq
(
Pst               *pst,     /* post structure  */
LrgSchMeasSndReqInfo *measInfo /* Meas Req Info */
)
#else
PUBLIC S16 RgMiLrgSchL2MeasSendReq(pst, measInfo)
   Pst               *pst;     /* post structure  */
   LrgSchMeasSndReqInfo *measInfo; /* Meas Req Info */
#endif
{
   S16                 ret = ROK;
   RgSchCellCb         *cell;
   RgSchCb             *instCb =  &rgSchCb[(pst->dstInst - 1)];
   U8                  idx;

   TRC3(RgMiLrgSchL2MeasSendReq)



   cell = NULLP;
   for (idx = 0; idx < instCb->numSaps; idx++)
   {
      if ( instCb->rgrSap[idx].cell->cellId == measInfo->cellId)
      {
         /* got the cell break the loop */
         cell = instCb->rgrSap[idx].cell;
         break;
      }
   }
   if (cell == NULLP)
   {
      RLOG_ARG0(L_ERROR,DBG_CELLID,measInfo->cellId,
               "Send req Failed.Invalid Cell Id");
      RETVALUE(RFAILED);
   }

   if(measInfo->measType & LRG_L2MEAS_AVG_PRB_PER_QCI_UL)
   {
      RgInfL2MeasSndReq measSndReq;
      Pst               pstMac;
      cmMemset((U8 *)&measSndReq, 0, sizeof(RgInfL2MeasSndReq));
      measSndReq.transId  = measInfo->hdr.transId;
      measSndReq.measType = measInfo->measType;
      measSndReq.timePrd  = measInfo->timePrd;
      measSndReq.cellId   = measInfo->cellId;
      rgSCHUtlGetPstToLyr(&pstMac, &rgSchCb[cell->instIdx], cell->macInst);
      RgSchMacL2MeasSend(&pstMac, &measSndReq);
   }
   else
   { 
      cell->sndL2Meas = TRUE;
   }

   RETVALUE(ret);
}/*RgMiLrgSchL2MeasSendReq*/
#endif /* LTE_L2_MEAS */




/************* RGR  Interface ****************/
/**
 * @brief API for bind request from RRM towards MAC. 
 *
 * @details
 *
 *     Function: RgUiRgrBndReq
 *     
 *     This API is invoked by RRM towards MAC to bind RGR SAP. 
 *     These API validates the Pst, spId, suId and sends the bind confirm to
 *     RRM.
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
PUBLIC S16 RgUiRgrBndReq
(
 Pst   *pst, 
 SuId  suId,
 SpId  spId
 )
#else
PUBLIC S16 RgUiRgrBndReq(pst, suId, spId)
   Pst   *pst; 
   SuId  suId;
   SpId  spId;
#endif
{
   S16       ret = ROK;
   Pst       tmpPst;   /* Temporary Post Structure */
   Inst      instId = pst->dstInst-RGSCH_INST_START;
   RgUstaDgn dgn;      /* Alarm diagnostics structure */

   TRC3(RgUiRgrBndReq)


   tmpPst.prior       = pst->prior;
   tmpPst.route       = pst->route;
   tmpPst.selector    = pst->selector;
   tmpPst.region      = rgSchCb[instId].rgSchInit.region;
   tmpPst.pool        = rgSchCb[instId].rgSchInit.pool;
   tmpPst.srcProcId   = rgSchCb[instId].rgSchInit.procId;
   tmpPst.srcEnt      = rgSchCb[instId].rgSchInit.ent;
   tmpPst.srcInst     = rgSchCb[instId].rgSchInit.inst + RGSCH_INST_START;
   tmpPst.event       = EVTNONE;
   tmpPst.dstProcId   = pst->srcProcId;
   tmpPst.dstEnt      = pst->srcEnt;
   tmpPst.dstInst     = pst->srcInst;

   if(spId < rgSchCb[instId].numSaps)
   {
      /* Check the state of the SAP */
      switch (rgSchCb[instId].rgrSap[spId].sapSta.sapState)
      {
         /* This case might not be needed if SAP not configured then it will go
          * to else of above if condition */
         case LRG_UNBND: /* SAP is not bound */
            RLOG0(L_DEBUG,"SAP Not yet bound");
            rgSchCb[instId].rgrSap[spId].sapSta.sapState = LRG_BND;
            rgSchCb[instId].rgrSap[spId].sapCfg.suId = suId;
            /* Send Bind Confirm with status as SUCCESS */
            ret = rgSCHUtlRgrBndCfm(instId, suId, CM_BND_OK);
            /*Indicate to Layer manager  */
            rgSCHUtlFillDgnParams(instId, &dgn, LRG_USTA_DGNVAL_MEM); 
            ret = rgSCHLmmStaInd(instId, LCM_CATEGORY_INTERFACE,
                  LRG_EVENT_RGRSAP_ENB, LCM_CAUSE_UNKNOWN, &dgn);
            break;
         case LRG_BND: /* SAP is already bound*/
            RLOG0(L_DEBUG,"SAP is already bound");
            ret = rgSCHUtlRgrBndCfm(instId, suId, CM_BND_OK);
            break;
         default: /* Should Never Enter here */
#if (ERRCLASS & ERRCLS_ADD_RES) 
            RGSCHLOGERROR(instId, ERRCLS_INT_PAR, ERG001, 
                  (ErrVal)rgSchCb[instId].rgrSap[spId].sapSta.sapState,
                  "Invalid SAP State:RgUiRrgBndReq failed\n");
#endif
            ret = rgSCHUtlRgrBndCfm(instId, suId, CM_BND_NOK);
            break;
      }
   }
   else
   {
#if (ERRCLASS & ERRCLS_ADD_RES)      
      /* ccpu00117035 - MOD - Changed ErrVal argument from accessing sap state 
         to spId to avoid seg fault due to invalid sapID */
      RGSCHLOGERROR(instId, ERRCLS_INT_PAR, ERG002,
            (ErrVal)spId, "Invalid SAP Id:RgUiRrgBndReq failed\n");
#endif
      ret = RgUiRgrBndCfm(&tmpPst, suId, CM_BND_NOK);
   }
   RETVALUE(ret);
}  /* RgUiRgrBndReq */

/**
 * @brief API for unbind request from RRM towards MAC. 
 *
 * @details
 *
 *     Function: RgUiRgrUbndReq
 *     
 *     This API is invoked by RRM towards MAC to unbind RGR SAP. 
 *     These API validates the Pst, spId, suId and transfers the unbind request 
 *     specific information to corresponding ownership module (GOM) API.
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
PUBLIC S16 RgUiRgrUbndReq
(
 Pst    *pst,
 SpId   spId,
 Reason reason
 )
#else
PUBLIC S16 RgUiRgrUbndReq(pst, spId, reason)
   Pst    *pst; 
   SpId   spId;
   Reason reason;
#endif
{
   Inst instId = pst->dstInst-RGSCH_INST_START;
   TRC3(RgUiRgrUbndReq)


   /* SAP Id validation */
   if (spId < rgSchCb[instId].numSaps)
   {
      switch(rgSchCb[instId].rgrSap[spId].sapSta.sapState)
      {
         case LRG_BND: /* SAP is already bound*/
            RLOG0(L_DEBUG,"SAP is already bound");
            /* setting SAP state to UN BOUND */
            rgSchCb[instId].rgrSap[spId].sapSta.sapState = LRG_UNBND;
            break;
         default:
#if (ERRCLASS & ERRCLS_ADD_RES)      
            RGSCHLOGERROR(instId, ERRCLS_INT_PAR, ERG003,
                  (ErrVal)rgSchCb[instId].rgrSap[spId].sapSta.sapState,
                  "Invalid SAP State: RgUiRgrUbndReq failed\n");
#endif
            RETVALUE(RFAILED);
      }
   }
   else
   {
#if (ERRCLASS & ERRCLS_ADD_RES)      
      RGSCHLOGERROR(instId,ERRCLS_INT_PAR, ERG004, 
            (ErrVal)rgSchCb[instId].rgrSap[spId].sapSta.sapState,
            "Invalid SAP Id:RgUiRgrUbndReq failed\n");
#endif
      RETVALUE(RFAILED);
   }
   RETVALUE(ROK);
}  /* RgUiRgrUbndReq */

/**
 * @brief API for configuration request from RRM towards MAC. 
 *
 * @details
 *
 *     Function: RgUiRgrCfgReq
 *     
 *     This API is invoked by RRM towards MAC to configure MAC. 
 *     These API validates the Pst, spId, suId and transfers the config request 
 *     specific information to corresponding ownership module (GOM) API.
 *
 *           
 *  @param[in]  Pst           *pst
 *  @param[in]  SpId          spId
 *  @param[in]  RgrCfgTransId transId
 *  @param[in]  RgrCfgReqInfo *cfgReqInfo
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED 
 **/
#ifdef ANSI
PUBLIC S16 RgUiRgrCfgReq
(
 Pst           *pst, 
 SpId          spId,
 RgrCfgTransId transId,
 RgrCfgReqInfo *cfgReqInfo
 )
#else
PUBLIC S16 RgUiRgrCfgReq(pst, spId, transId, cfgReqInfo)
   Pst           *pst; 
   SpId          spId;
   RgrCfgTransId transId;
   RgrCfgReqInfo *cfgReqInfo;
#endif
{

   S16       ret       = ROK;
   U8        cfmStatus = 0x00ff;
   U8        prntTrans[RGR_CFG_TRANSID_SIZE+1];
   Inst      instId = pst->dstInst-RGSCH_INST_START;

   TRC3(RgUiRgrCfgReq);

   cmMemcpy((U8 *)prntTrans, (U8 *)transId.trans, RGR_CFG_TRANSID_SIZE);
   prntTrans[RGR_CFG_TRANSID_SIZE] = '\0';


   if (cfgReqInfo == NULLP)
   {
      RLOG_ARG0(L_ERROR,DBG_INSTID,instId,"Input Message Buffer is NULL");
      rgSCHUtlRgrCfgCfm(instId, spId, transId, cfmStatus); 
      RETVALUE(RFAILED);
   }

   if (spId < rgSchCb[instId].numSaps)
   {
      switch(rgSchCb[instId].rgrSap[spId].sapSta.sapState)
      {
         case LRG_BND: /* SAP is already bound */
            RLOG0(L_DEBUG,"SAP is already bound");
            break;
         default: /* Should never reach here */
#if (ERRCLASS & ERRCLS_ADD_RES)      
            RGSCHLOGERROR(instId, ERRCLS_INT_PAR, ERG005, 
                  (ErrVal)rgSchCb[instId].rgrSap[spId].sapSta.sapState,
                  "Invalid SAP State: RgUiRgrCfgReq failed\n");
#endif
            SPutSBuf(pst->region, pst->pool, (Data *)cfgReqInfo,
                  (Size)sizeof(*cfgReqInfo));
            rgSCHUtlRgrCfgCfm(instId, spId, transId, cfmStatus); 
            RETVALUE(RFAILED);
      }
   }
   else
   {
#if (ERRCLASS & ERRCLS_ADD_RES)      
      RGSCHLOGERROR(instId, ERRCLS_INT_PAR, ERG006,
            (ErrVal)rgSchCb[instId].rgrSap[spId].sapCfg.spId,
            "Invalid SAP Id:RgUiRgrCfgReq failed\n");
#endif
      SPutSBuf(pst->region, pst->pool, (Data *)cfgReqInfo,
            (Size)sizeof(*cfgReqInfo));
      rgSCHUtlRgrCfgCfm(instId, spId, transId, cfmStatus); 
      RETVALUE(RFAILED);
   }

   /* Handle configuration */
   ret = rgSCHGomHndlCfg(pst->region, pst->pool,
         &rgSchCb[(pst->dstInst - 1)], spId, transId, 
         cfgReqInfo);
   if (ret != ROK)
   {
      RLOG_ARG0(L_ERROR,DBG_INSTID,instId,"Configuration Request Handling Failed");
      RETVALUE(RFAILED);
   }

   RETVALUE(ROK);
}  /* RgUiRgrCfgReq */

#ifdef RGR_SI_SCH
/**
 * @brief API for SI configuration request from RRM towards MAC. 
 *
 * @details
 *
 *     Function: RgUiRgrSiCfgReq
 *     
 *     This API is invoked by RRM towards MAC to configure SI at MAC. 
 *     These API validates the Pst, spId, suId and transfers the 
 *     config request specific information to corresponding ownership 
 *     module (GOM) API.
 *
 *           
 *  @param[in]  Pst           *pst
 *  @param[in]  SpId          spId
 *  @param[in]  RgrCfgTransId transId
 *  @param[in]  RgrSiCfgReqInfo *cfgReqInfo
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED 
 **/
#ifdef ANSI
PUBLIC S16 RgUiRgrSiCfgReq
(
Pst           *pst, 
SpId          spId,
RgrCfgTransId transId,
RgrSiCfgReqInfo *cfgReqInfo
)
#else
PUBLIC S16 RgUiRgrSiCfgReq(pst, spId, transId, cfgReqInfo)
Pst           *pst; 
SpId          spId;
RgrCfgTransId transId;
RgrSiCfgReqInfo *cfgReqInfo;
#endif
{
   S16       ret       = ROK;
   U8        cfmStatus = RGR_CFG_CFM_NOK;
   U8        prntTrans[RGR_CFG_TRANSID_SIZE+1];
   Inst      instId = pst->dstInst-RGSCH_INST_START;

   TRC2(RgUiRgrSiCfgReq);

   cmMemcpy((U8 *)prntTrans, (U8 *)transId.trans, RGR_CFG_TRANSID_SIZE);
   prntTrans[RGR_CFG_TRANSID_SIZE] = '\0';


   if (cfgReqInfo == NULLP)
   {
      RLOG_ARG0(L_ERROR,DBG_INSTID,instId,"Input Message Buffer "
                   "is NULL");
      rgSCHUtlRgrSiCfgCfm(instId, spId, transId, cfmStatus); 
      RETVALUE(RFAILED);
   }

   if (spId < rgSchCb[instId].numSaps)
   {
      if(LRG_BND != rgSchCb[instId].rgrSap[spId].sapSta.sapState)
      {
#if (ERRCLASS & ERRCLS_ADD_RES)      
         RGSCHLOGERROR(instId, ERRCLS_INT_PAR, ERG007, 
                  (ErrVal)rgSchCb[instId].rgrSap[spId].sapSta.sapState,
                  "Invalid SAP State: RgUiRgrSiCfgReq failed\n");
#endif
         SPutSBuf(pst->region, pst->pool, (Data *)cfgReqInfo,
                                          (Size)sizeof(*cfgReqInfo));
         rgSCHUtlRgrSiCfgCfm(instId, spId, transId, cfmStatus); 
         RETVALUE(RFAILED);
      }
   }
   else
   {
#if (ERRCLASS & ERRCLS_ADD_RES)      
      RGSCHLOGERROR(instId, ERRCLS_INT_PAR, ERG008,
            (ErrVal)rgSchCb[instId].rgrSap[spId].sapCfg.spId,
            "Invalid SAP Id:RgUiRgrSiCfgReq failed\n");
#endif
      SPutSBuf(pst->region, pst->pool, (Data *)cfgReqInfo,
                                   (Size)sizeof(*cfgReqInfo));
      rgSCHUtlRgrSiCfgCfm(instId, spId, transId, cfmStatus); 
      RETVALUE(RFAILED);
   }

   /* Handle configuration */
   ret = rgSCHGomHndlSiCfg(pst->region, pst->pool,
            &rgSchCb[(pst->dstInst - 1)], spId, transId, 
                        cfgReqInfo);
   if (ret != ROK)
   {
      RLOG_ARG0(L_ERROR,DBG_INSTID,instId,"RgUiRgrSiCfgReq:"
                   "Configuration Request Handling Failed");
      RETVALUE(RFAILED);
   }

   RETVALUE(ROK);
}  /* RgUiRgrSiCfgReq */


/**
 * @brief API for Warning SI configuration request from RRM towards MAC. 
 *
 * @details
 *
 *     Function: RgUiRgrWarningSiCfgReq
 *     
 *     This API is invoked by RRM towards MAC to configure SI at MAC. 
 *     These API validates the Pst, spId, suId and transfers the 
 *     config request specific information to corresponding ownership 
 *     module (GOM) API.
 *
 *           
 *  @param[in]  Pst           *pst
 *  @param[in]  SpId          spId
 *  @param[in]  RgrCfgTransId transId
 *  @param[in]  RgrWarningSiCfgReqInfo *warningSiCfgReqInfo
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED 
 **/
#ifdef ANSI
PUBLIC S16 RgUiRgrWarningSiCfgReq
(
Pst           *pst, 
SpId          spId,
RgrCfgTransId transId,
RgrWarningSiCfgReqInfo *warningSiCfgReqInfo
)
#else
PUBLIC S16 RgUiRgrWarningSiCfgReq(pst, spId, transId, warningSiCfgReqInfo)
Pst           *pst; 
SpId          spId;
RgrCfgTransId transId;
RgrWarningSiCfgReqInfo *warningSiCfgReqInfo;
#endif
{
   Inst     instId = pst->dstInst-RGSCH_INST_START;
   S16      ret    = ROK;
   U8       cfmStatus = RGR_CFG_CFM_NOK;
   U8       prntTrans[RGR_CFG_TRANSID_SIZE+1];

   TRC2(RgUiRgrWarningSiCfgReq);

   cmMemcpy((U8 *)prntTrans, (U8 *)transId.trans, RGR_CFG_TRANSID_SIZE);
   prntTrans[RGR_CFG_TRANSID_SIZE] = '\0';



   if (warningSiCfgReqInfo == NULLP)
   {
      RLOG_ARG0(L_ERROR,DBG_INSTID,instId,"Input Message Buffer "
               "is NULL");
      rgSCHUtlRgrWarningSiCfgCfm(instId, spId, 0, transId, cfmStatus);
      RETVALUE(RFAILED);
   }

   if (spId < rgSchCb[instId].numSaps)
   {
      if(LRG_BND != rgSchCb[instId].rgrSap[spId].sapSta.sapState)
      {
#if (ERRCLASS & ERRCLS_ADD_RES)      
         RGSCHLOGERROR(instId, ERRCLS_INT_PAR, ERG023, 
               (ErrVal)rgSchCb[instId].rgrSap[spId].sapSta.sapState,
               "Invalid SAP State: warningSiCfgReqInfo failed\n");
#endif
         rgSCHUtlFreeWarningSiSeg(pst->region, pst->pool, 
               &warningSiCfgReqInfo->siPduLst);
         SPutSBuf(pst->region, pst->pool, (Data *)warningSiCfgReqInfo,
               sizeof(RgrWarningSiCfgReqInfo));
         rgSCHUtlRgrWarningSiCfgCfm(instId, spId, warningSiCfgReqInfo->siId, 
               transId, cfmStatus);
         RETVALUE(RFAILED);
      }
   }
   else
   {
#if (ERRCLASS & ERRCLS_ADD_RES)      
      RGSCHLOGERROR(instId, ERRCLS_INT_PAR, ERG024,
            (ErrVal)rgSchCb[instId].rgrSap[spId].sapCfg.spId,
            "Invalid SAP Id:warningSiCfgReqInfo failed\n");
#endif
      rgSCHUtlFreeWarningSiSeg(pst->region, pst->pool, 
            &warningSiCfgReqInfo->siPduLst);
      SPutSBuf(pst->region, pst->pool, (Data *)warningSiCfgReqInfo,
            sizeof(RgrWarningSiCfgReqInfo));
      rgSCHUtlRgrWarningSiCfgCfm(instId, spId, warningSiCfgReqInfo->siId, 
            transId, cfmStatus);
      RETVALUE(RFAILED);
   }

   /* Handle configuration */
   ret = rgSCHGomHndlWarningSiCfg(pst->region, pst->pool,
         &rgSchCb[instId], spId, transId, warningSiCfgReqInfo);
   if(ret != ROK)
   {
      RLOG_ARG0(L_ERROR,DBG_INSTID,instId,
               "Configuration Request Handling Failed");
      RETVALUE(RFAILED);
   }

   RETVALUE(ROK);
}


/**
 * @brief API for Warning SI Stop  request from RRM towards MAC. 
 *
 * @details
 *
 *     Function: RgUiRgrWarningSiStopReq
 *     
 *     This API is invoked by RRM towards MAC to Stop warning SI at MAC. 
 *     These API validates the Pst, spId, suId and transfers the 
 *     stop request specific information to corresponding ownership 
 *     module (GOM) API.
 *
 *           
 *  @param[in]  Pst           *pst
 *  @param[in]  SpId          spId
 *  @param[in]  RgrWarningSiCfgReqInfo *warningSiCfgReqInfo
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED 
 **/
#ifdef ANSI
PUBLIC S16 RgUiRgrWarningSiStopReq
(
Pst           *pst,
SpId          spId,
RgrCfgTransId transId,
U8            siId
)
#else
PUBLIC S16 RgUiRgrWarningSiStopReq(pst,spId, transId, siId)
Pst           *pst;
SpId          spId;
RgrCfgTransId transId;
U8            siId;
#endif
{         
   Inst         instId = pst->dstInst-RGSCH_INST_START;

   TRC3(RgUiRgrWarningSiStopReq)



   if (spId < rgSchCb[instId].numSaps)
   {
      if(LRG_BND != rgSchCb[instId].rgrSap[spId].sapSta.sapState)
      {
#if (ERRCLASS & ERRCLS_ADD_RES)      
         RGSCHLOGERROR(instId, ERRCLS_INT_PAR, ERG025, 
               (ErrVal)rgSchCb[instId].rgrSap[spId].sapSta.sapState,
               "Invalid SAP State: RgUiRgrWarningSiStopReq failed\n");
#endif
         RETVALUE(RFAILED);
      }
   }
   else
   {
#if (ERRCLASS & ERRCLS_ADD_RES)      
      RGSCHLOGERROR(instId, ERRCLS_INT_PAR, ERG026,
            (ErrVal)rgSchCb[instId].rgrSap[spId].sapCfg.spId,
            "Invalid SAP Id:RgUiRgrWarningSiStopReq failed\n");
#endif
      RETVALUE(RFAILED);
   }

   rgSCHGomHndlWarningSiStopReq(pst->region, pst->pool,
         &rgSchCb[instId], siId, transId, spId);

   RETVALUE(ROK);
}
#endif /*RGR_SI_SCH */

/* LTE_ADV_FLAG_REMOVED_START */

/**
 * @brief API for LOAD INF request from RRM towards MAC.
 *
 * @details
 *
 *     Function: RgUiRgrLoadInfReq
 *
 *     This API is invoked by RRM towards MAC to configure LOAD INF Parameters at MAC.
 *     These API validates the Pst, spId, suId and transfers the
 *     LOAD INF request to corresponding ownership
 *     module (GOM) API.
 *
 *
 *  @param[in]  Pst               *pst
 *  @param[in]  SpId              spId
 *  @param[in]  RgrCfgTransId     transId
 *  @param[in]  RgrLoadInfReqInfo *loadInfReq
 *  @return  S16
 *      -# ROK
 *      -# RFAILED
 **/
#ifdef ANSI
PUBLIC S16 RgUiRgrLoadInfReq
(
 Pst               *pst,
 SpId              spId,
 RgrCfgTransId     transId,
 RgrLoadInfReqInfo *loadInfReq
 )
#else
PUBLIC S16 RgUiRgrLoadInfReq(pst, spId, transId, loadInfReq)
   Pst               *pst;
   SpId              spId;
   RgrCfgTransId     transId;
   RgrLoadInfReqInfo *loadInfReq;
#endif
{
   S16       ret       = ROK;
   U8        prntTrans[RGR_CFG_TRANSID_SIZE+1];
   Inst      instId = pst->dstInst-RGSCH_INST_START;

   TRC2(RgUiRgrLoadInfReq);

   cmMemcpy((U8 *)prntTrans, (U8 *)transId.trans, RGR_CFG_TRANSID_SIZE);
   prntTrans[RGR_CFG_TRANSID_SIZE] = '\0';


   if (loadInfReq == NULLP)
   {
      RLOG_ARG0(L_ERROR,DBG_INSTID,instId,"Input Message Buffer "
               "is NULL");
      RETVALUE(RFAILED);
   }

   if (spId < rgSchCb[instId].numSaps)
   {
      if(LRG_BND != rgSchCb[instId].rgrSap[spId].sapSta.sapState)
      {
#if (ERRCLASS & ERRCLS_ADD_RES)
         RGSCHLOGERROR(instId, ERRCLS_INT_PAR, ERG007, 
               (ErrVal)rgSchCb[instId].rgrSap[spId].sapSta.sapState,
               "Invalid SAP State: RgUiRgrLoadInfReq failed\n");
#endif
         SPutSBuf(pst->region, pst->pool, (Data *)loadInfReq,
               (Size)sizeof(*loadInfReq));
         RETVALUE(RFAILED);
      }
   }
   else
   {
#if (ERRCLASS & ERRCLS_ADD_RES)
      RGSCHLOGERROR(instId, ERRCLS_INT_PAR, ERG008,
            (ErrVal)rgSchCb[instId].rgrSap[spId].sapCfg.spId,
            "Invalid SAP Id:RgUiRgrLoadInfReq failed\n");
#endif
      SPutSBuf(pst->region, pst->pool, (Data *)loadInfReq,
            (Size)sizeof(*loadInfReq));
      RETVALUE(RFAILED);
   }

   /* Handle configuration */
   ret = rgSCHGomHndlLoadInf(pst->region, pst->pool,
         &rgSchCb[(pst->dstInst - 1)], spId, transId, 
         loadInfReq);
   if (ret != ROK)
   {
      RLOG_ARG0(L_ERROR,DBG_INSTID,instId,
               "Configuration Request Handling Failed");
      RETVALUE(RFAILED);
   }

   RETVALUE(ROK);
}  /* RgUiRgrLoadInfReq */
/* LTE_ADV_FLAG_REMOVED_END */

/************** MAC SCH Interface **************/ 
/**
 * @brief Function for updating dedicated channel BO at scheduler from MAC.
 *
 * @details
 *
 *     Function : rgMacSchDedBoUpdtReq
 *     
 *     This function shall be invoked whenever MAC gets staRsp from RLC
 *     Scheduler shall update BO in its dedicated logical channel control block.
 *           
 *  @param[in] Pst*           pst
 *  @param[in] S16            cellSapId
 *  @param[in] RgInfDedBoRpt  *boRpt
 *  @return  S16
 *      -# ROK 
 **/
#ifdef ANSI
PUBLIC S16 RgMacSchDedBoUpdtReq
(
Pst*           pst,
RgInfDedBoRpt  *boRpt
)
#else
PUBLIC S16 RgMacSchDedBoUpdtReq(pst, boRpt)
Pst*           pst;
RgInfDedBoRpt  *boRpt;
#endif
{
   RgSchCellCb   *cell;
   RgSchUeCb     *ue;

   Inst          inst = (pst->dstInst - RGSCH_INST_START);
   S16           cellSapId = boRpt->cellSapId;

   TRC3(RgMacSchDedBoUpdtReq)
/*
   RLOG_ARG2(L_DEBUG,DBG_CELLID,boRpt->cellId,"rgMacSchDedBoUpdtReq():"
            " boRpt->rnti = %u  boRpt->lcId = %u",boRpt->rnti, boRpt->lcId);
*/
   /* No need to chk for cell being NULL as MAC wouldn't have found instance if
    * it doesnt exist */
   cell = rgSchCb[inst].rgrSap[cellSapId].cell;

#ifndef NO_ERRCLS
   if (cell->cellId != boRpt->cellId)
   {
      /* Handle Cell fetch failure */
      RGSCHLOGERROR(inst, ERRCLS_INT_PAR,ERG009,(ErrVal)boRpt->cellId,
                            "rgMacSchDedBoUpdtReq(): Invalid cell Id");
      RETVALUE(RFAILED);
   }
#endif

   /* Update Bo in the given logical channel of the UE */
   if ((ue = rgSCHDbmGetUeCb(cell, boRpt->rnti)) != NULLP)
   {
      RgSchDlLcCb   *dlLc;
      if ((dlLc = rgSCHDbmGetDlDedLcCb(ue, boRpt->lcId)) != NULLP)
      {
#ifdef LTE_L2_MEAS
         if(dlLc->lcType == CM_LTE_LCH_DTCH)
         {
            if((dlLc->bo == 0) &&(boRpt->bo != 0))
            {
               /* UE is active */
               if(!(ue->qciActiveLCs[dlLc->qciCb->qci]))
               {
                  dlLc->qciCb->dlUeCount++;
               }
               ue->qciActiveLCs[dlLc->qciCb->qci]++;
            }
            else if((dlLc->bo != 0) && (boRpt->bo == 0) && (dlLc->qciCb->dlUeCount))
            {
               /* UE is inactive */
               if (ue->qciActiveLCs[dlLc->qciCb->qci])
               {
                  ue->qciActiveLCs[dlLc->qciCb->qci]--;
                  if (!(ue->qciActiveLCs[dlLc->qciCb->qci]))
                  {
                     dlLc->qciCb->dlUeCount--;
                  }
               }
            }
         }
#endif
#ifdef TENB_STATS
         if (boRpt->bo > dlLc->bo)
         {
            ue->tenbStats->stats.sch.dlBo += ((boRpt->bo - dlLc->bo)<<3);
         }
#endif
         /* RLC provides cumulative BO for each LC.
          * Reduce the left out unscheduled bo from total bo and 
          * update the new BO to the total bo */
         if(ue->totalBo >= dlLc->bo)
         {
            ue->totalBo -= dlLc->bo;
         }
         else
         {
            ue->totalBo = 0; /* this case should not arise
                             * Resetting for safety */
         }

         ue->totalBo +=  boRpt->bo;
         dlLc->bo = boRpt->bo;
         dlLc->oldestSduArrTime = boRpt->oldestSduArrTime;
         dlLc->setMaxUlPrio = boRpt->setMaxUlPrio;
         dlLc->setMaxDlPrio = boRpt->setMaxDlPrio;
         /* Update the scheduler */
         rgSCHUtlDlDedBoUpd(cell, ue, dlLc);
         RETVALUE(ROK);
      }
   }
   RETVALUE(RFAILED);

} /* end of rgMacSchDedBoUpdtReq */


/**
 * @brief Function for updating common channel BO at scheduler from MAC.
 *
 * @details
 *
 *     Function : RgMacSchCmnBoUpdtReq
 *     
 *     This function shall be invoked whenever MAC gets staRsp from RLC
 *     Scheduler shall update BO in its common logical channel control block.
 *           
 *  @param[in] Pst*            pst
 *  @param[in] S16             cellSapId
 *  @param[in] RgInfCmnBoRpt  *boRpt
 *  @return  S16
 *      -# ROK 
 **/
#ifdef ANSI
PUBLIC S16 RgMacSchCmnBoUpdtReq
(
Pst*           pst,
RgInfCmnBoRpt  *boRpt
)
#else
PUBLIC S16 RgMacSchCmnBoUpdtReq(pst, boRpt)
Pst*           pst;
RgInfCmnBoRpt  *boRpt;
#endif
{
   RgSchCellCb   *cell;
   Inst          inst = (pst->dstInst - RGSCH_INST_START);
   S16           cellSapId = boRpt->cellSapId;

   TRC3(RgMacSchCmnBoUpdtReq)

   /* No need to chk for cell being NULL as MAC would not have found instance if
    * it doesnt exist */
   cell = rgSchCb[inst].rgrSap[cellSapId].cell;
   if (cell->cellId != boRpt->cellId)
   {
      RLOG_ARG1(L_DEBUG,DBG_CELLID,cell->cellId,"RgMacSchCmnBoUpdtReq():"
               "Invalid boRpt cell Id:%d",boRpt->cellId);
      RETVALUE(RFAILED);
   }

   /* handle status response on CCCH */
   if(boRpt->lcId == cell->dlCcchId)
   {
      RLOG_ARG0(L_DEBUG,DBG_CELLID,cell->cellId,"RgMacSchCmnBoUpdtReq():"
               " BO update for CCCH");
      rgSCHUtlHndlCcchBoUpdt(cell, boRpt); 
   }
   else
   {
      rgSCHUtlHndlBcchPcchBoUpdt(cell, boRpt); 
   }
   
   RETVALUE(ROK);
} /* end of RgMacSchCmnBoUpdtReq */
/*Fix: start: Inform UE delete to scheduler*/
/**
 * @brief This API is used to send data indication to Scheduler instance from MAC.
 *
 * @details
 *
 *     Function : rgMacSchUeDelInd
 *     
 *     This function shall be invoked whenever MAC gets Ue delete request. 
 *      
 *           
 *  @param[in] Pst*            pst
 *  @param[in] RgInfUeDelInd   *ueDelInd
 *  @return  S16
 *      -# ROK 
 **/
#ifdef ANSI
PUBLIC S16 RgMacSchUeDelInd
(
Pst*             pst,
RgInfUeDelInd    *ueDelInd
)
#else
PUBLIC S16 RgMacSchUeDelInd(pst, ueDelInd)
Pst*             pst;
RgInfUeDelInd    *ueDelInd;
#endif
{
   RgSchCellCb       *cell;
   Inst              inst = (pst->dstInst - RGSCH_INST_START);
   S16               cellSapId = ueDelInd->cellSapId;
   CmLList           *tmp;
   RgSchRntiLnk      *rntiLnk;
   
   TRC3(RgMacSchUeDelInd)

   if (rgSchCb[inst].rgrSap == NULLP || rgSchCb[inst].rgrSap[cellSapId].cell == NULLP)
   {
      RLOG_ARG0(L_ERROR,DBG_CELLID,ueDelInd->cellId,"rgrSap or cell is not configured");
      RETVALUE(ROK);
   }
   cell = rgSchCb[inst].rgrSap[cellSapId].cell;
#ifndef NO_ERRCLS
   if (cell->cellId != ueDelInd->cellId)
   {
      /* Handle Cell fetch failure */
      RLOG_ARG1(L_ERROR,DBG_CELLID,cell->cellId,
         		"rgMacSchUeDelInd(): Invalid ueDelInd cell Id:%d",
		         ueDelInd->cellId);
      RETVALUE(ROK);
   }
#endif
      
   CM_LLIST_FIRST_NODE(&cell->rntiDb.rntiGuardPool, tmp);

   while(tmp)
   {
      rntiLnk = (RgSchRntiLnk *)(tmp->node);
      if(rntiLnk->rnti == ueDelInd->rnti)
      {
         cmLListDelFrm(&cell->rntiDb.rntiGuardPool, tmp);
         tmp->node = NULLP;
         rgSCHDbmRlsRnti(cell, rntiLnk);
      	 RLOG_ARG2(L_DEBUG,DBG_CELLID,cell->cellId,
		  "RNTI:%d Released from the Guard pool(%ld)",
		  ueDelInd->rnti, cell->rntiDb.rntiGuardPool.count);
	 
         break;
      }
      CM_LLIST_NEXT_NODE(&cell->rntiDb.rntiGuardPool, tmp);
   }

   if(tmp == NULLP)
   {
      /* Fix : syed HO UE does not have a valid ue->rntiLnk */
      RLOG_ARG2(L_INFO,DBG_CELLID,ueDelInd->cellId,"HO CRNTI:%d not present in the"
           "Guard Pool:%ld", ueDelInd->rnti, cell->rntiDb.rntiGuardPool.count);
   } 
   
   RETVALUE(ROK);
}  /* rgMacSchUeDelInd */
/*Fix: end: Inform UE delete to scheduler*/
/**
 * @brief This API is used to send data indication to Scheduler instance from MAC.
 *
 * @details
 *
 *     Function : rgMacSchSfRecpInd
 *     
 *     This function shall be invoked whenever MAC gets datInd on TFU
 *     Scheduler shall act on the CEs and data that are received as part of
 *     datInd.
 *           
 *  @param[in] Pst*            pst
 *  @param[in] S16             cellSapId
 *  @param[in] CmLteRnti       rnti
 *  @param[in] DatIndInfo      *datInd
 *  @return  S16
 *      -# ROK 
 **/
#ifdef ANSI
PUBLIC S16 RgMacSchSfRecpInd
(
Pst*             pst,
RgInfSfDatInd    *subfrmInfo
)
#else
PUBLIC S16 RgMacSchSfRecpInd(pst, subfrmInfo)
Pst*             pst;
RgInfSfDatInd    *subfrmInfo;
#endif
{
   S16               ret = RFAILED;
   RgSchErrInfo      err;
   RgSchUeCb         *ue;
   RgInfUeDatInd     *datInd;
   RgSchCellCb       *cell;
   Inst              inst = (pst->dstInst - RGSCH_INST_START);
   CmLteRnti         rnti;
   CmLListCp         *lnkLst;
   CmLList           *tmp;
   S16               cellSapId = subfrmInfo->cellSapId;
   RgrUeStaIndInfo   *ueStaInd;
#ifdef RG_UNUSED
//#ifdef LTEMAC_SPS
   RgSchCmnUlUeSpsInfo   *ulSpsUe;
#endif
   /* RRM_RBC_X */
   U32                   idx;
   /* RRM_RBC_Y */
   
#ifdef LTE_L2_MEAS
   U8               qci;
   U16              datIndActQci = 0;
   U16              combDatIndActQci = 0; /* Prev and Latest Dat Ind combined */
   U16              tempUeActQci = 0; /* UE specific Active QCIs */
   U16              diffBits = 0;
   U8               lcCount;
#endif
   TRC3(RgMacSchSfRecpInd)

   /* No need to chk for cell being NULL as MAC wouldn't have found instance if
    * it doesnt exist */
   cell = rgSchCb[inst].rgrSap[cellSapId].cell;

   /* lnkLst assignment */
   lnkLst = &(subfrmInfo->ueLst);

   CM_LLIST_FIRST_NODE(lnkLst, tmp);

   while((NULLP != tmp) && ((RgInfUeDatInd *)tmp->node != NULLP))
   {
      ue = NULLP;
#ifdef LTE_L2_MEAS
      qci = 0;
#endif
      datInd   = (RgInfUeDatInd *)tmp->node;
      rnti     = datInd->rnti;

      /* We shall try and find
       * out the RaCb based on the following - 
       * 1. If the incoming PDU contained a CCCH SDU i.e. this is message 3.
       * 2. If the incoming PDU contained a CRNTI control element, i.e. we should
       * have a ueCb also for this 
       */
      
      /* It could be that a non-msg3 pdu contains a CRNTI Control element. We
       * should check for CRNTI CE and if it exists the UECb must exist, also an
       * if the CRNTI in the CE and the one with which the message came in are
       * different we shall look for an raCb as well. 
       */
      if (datInd->ceInfo.bitMask & RGSCH_CRNTI_CE_PRSNT)
      {
         /* SR_RACH_STATS : CRNTI CE*/
         rgNumMsg3CrntiCE++;

         if (datInd->ceInfo.bitMask & RGSCH_CCCH_SDU_PRSNT)
         {
            RGSCH_FREE_MEM(subfrmInfo);
            err.errType = RGSCHERR_TOM_DATIND;
            RLOG_ARG1(L_ERROR,DBG_CELLID,cell->cellId,
                      "Received MSG3 with CRNTI:%d and also CCCH ", 
                     datInd->ceInfo.ces.cRnti);
            RETVALUE(RFAILED);
         }
         ue = rgSCHDbmGetUeCb (cell, datInd->ceInfo.ces.cRnti);
         if (ue == NULLP)
         {
            /* SR_RACH_STATS : CRNTI CE UECB NOT FOUND*/
            rgNumCrntiCeCrntiNotFound++;
            /* ccpu00141318 - Removed condition for SPS rnti checking*/
            RGSCH_FREE_MEM(subfrmInfo);
            err.errType = RGSCHERR_TOM_DATIND;
            RLOG_ARG1(L_ERROR,DBG_CELLID,cell->cellId, "Received MSG3 "
                     "with CRNTI:%d unable to find ueCb", 
                     datInd->ceInfo.ces.cRnti);
            RETVALUE(RFAILED);
         }

         if ((ret = rgSCHUtlProcMsg3 (subfrmInfo, cell, ue, 
               rnti, datInd, &err)) != ROK)
         { 
            RGSCH_FREE_MEM(subfrmInfo);
            err.errType = RGSCHERR_TOM_DATIND;
            RLOG_ARG1(L_ERROR,DBG_CELLID,cell->cellId, "Processing for MSG3 failed for CRNTI:%d", 
                     datInd->rnti);
            RETVALUE(RFAILED);
         }
         
#ifdef LTEMAC_SPS
         rgSCHUtlHdlCrntiCE (cell, ue);
#endif
         ret = rgSCHUtlAllocSBuf (cell->instIdx,(Data**)&ueStaInd,
                                  sizeof(RgrUeStaIndInfo));
         if(ret == ROK)
         {
            ueStaInd->status = RGR_UESTA_MAC_CRNTI_CE_RECVD;
#ifdef RG_UNUSED
//#ifdef LTEMAC_SPS
            ulSpsUe = RG_SCH_CMN_GET_UL_SPS_UE(ue);
            if(ulSpsUe->isUlSpsActv)
            {
               ueStaInd->status = RGR_UESTA_MAC_CRNTI_CE_RECVD_IN_SPS_ACTIVE;
               ue->ul.ulSpsCfg.isLcSRMaskEnab = FALSE;
            }
#endif
            ret = rgSCHUtlFillSndUeStaInd(cell, ue, ueStaInd);
            if(ret != ROK)
            {
               RLOG_ARG1(L_ERROR,DBG_CELLID,cell->cellId,
                   "Could not Send Ue Sta Ind UEID:%d",ue->ueId);
            }
         }
         CM_LLIST_NEXT_NODE(lnkLst, tmp);
         continue;
      } /* end of CRNTI based message */
      else if (datInd->ceInfo.bitMask & RGSCH_CCCH_SDU_PRSNT)
      {
         /* SR_RACH_STATS : CCCH SDU */
         rgNumMsg3CCCHSdu++;
         /* SR_RACH_STATS : CCCH SDU RACB NOT FOUND*/
         if (NULLP == rgSCHDbmGetRaCb (cell, rnti))
         {
            rgNumCCCHSduCrntiNotFound++;
         }

         if ((ret = rgSCHUtlProcMsg3 (subfrmInfo, cell, ue, 
               rnti, datInd, &err)) != ROK)
         { 
            RGSCH_FREE_MEM(subfrmInfo);
            err.errType = RGSCHERR_TOM_DATIND;
            RLOG_ARG1(L_ERROR,DBG_CELLID,cell->cellId,"Processing for MSG3 failed for CRNTI:%d", 
                     datInd->rnti);
            RETVALUE(RFAILED);
         }
         /* fix */
          CM_LLIST_NEXT_NODE(lnkLst, tmp);
          continue;
      } /* end of Msg3 processing */

      if (ue == NULLP)
      {
         ue = rgSCHDbmGetUeCb (cell, datInd->rnti);
         if (ue == NULLP)
         {
#ifdef LTEMAC_SPS 
         if((ue = rgSCHDbmGetSpsUeCb (cell, datInd->rnti)) == NULLP)
#endif
            {
               RGSCH_FREE_MEM(subfrmInfo);
               err.errType = RGSCHERR_TOM_DATIND;
               RLOG_ARG1(L_ERROR,DBG_CELLID,cell->cellId,"Unable to get the UE CB for CRNTI:%d", 
               datInd->rnti);
               RETVALUE(RFAILED);
            }
         }
      }
/* L2_COUNTERS */
#ifdef LTE_L2_MEAS
      /* The LCs for which data is received at MAC is provided to SCH.
         This information is used to estimate the Active LCs at UE
         since estimates based on only BSR is not accurate */
      if (datInd->ceInfo.bitMask & RGSCH_ACTIVE_LC_PRSNT)
      {
      
        /* Compose a Bitmask with for the QCI's for which Data
           is received */
        for (lcCount = 0; lcCount < RGINF_MAX_NUM_DED_LC; lcCount++)
        {
           if ((datInd->ceInfo.ulActLCs[lcCount] == TRUE) && (TRUE == ue->ul.lcCb[lcCount].isValid))
           {
              datIndActQci |= (1 << (ue->ul.lcCb[lcCount].qciCb->qci -1));
           }
        }
        if (ue->ulActiveLCs && ue->lastDatIndLCs)
        {
           /* Combine previous Dat Ind and current Dat Ind to
              estimate active LCs at UE */
           combDatIndActQci = ue->lastDatIndLCs | datIndActQci;
           tempUeActQci = ue->ulActiveLCs;
           ue->ulActiveLCs = combDatIndActQci;
           diffBits = combDatIndActQci ^ tempUeActQci;
           while (diffBits)
           {
              qci++;
              if (0x1 & diffBits)
              {
                 if (0x1 & tempUeActQci)
                 {
                    /* An active QCI has become inactive */
                    cell->qciArray[qci].ulUeCount--;
                 }
                 else
                 {
                    /* An Inactive QCI has become active */
                    cell->qciArray[qci].ulUeCount++;
                 }
              }
              diffBits >>= 1;
              tempUeActQci >>= 1;
           }
        }
        ue->lastDatIndLCs = datIndActQci;
      
      }

#endif /* LTE_L2_MEAS */
      /* Just copy the timing information from the dat indication into the one
       * stored in the UE CB, will be later utilized to handle Timing advance 
       */

      if ((ret = rgSCHUtlUpdSch (subfrmInfo, cell, ue, datInd, &err)) != ROK)
      {
         RGSCH_FREE_MEM(subfrmInfo);
         err.errType = RGSCHERR_TOM_DATIND;
         RLOG_ARG1(L_ERROR,DBG_CELLID,cell->cellId,"Unable to handle Data"
                   " Indication for UEID:%d",ue->ueId);
         RETVALUE(RFAILED);
      }

      CM_LLIST_NEXT_NODE(lnkLst, tmp);
   }
   /* RRM_RBC_X */
   /* update the UL PRB usage for all GBR QCIs*/
   for (idx = 0; idx < RGM_MAX_QCI_REPORTS; idx++)
   {
      cell->prbUsage.qciPrbRpts[idx].ulTotPrbUsed += subfrmInfo->qcisUlPrbCnt[idx];
   }
   /* RRM_RBC_Y */

   /* chk if Sch needs to dealloc datInd after unpk */
   RGSCH_FREE_MEM(subfrmInfo);
   RETVALUE(ret);
}  /* rgMacSchSfRecpInd */

#ifdef LTEMAC_SPS
/**
 * @brief Function to handle relInd from MAC for a UE
 *
 * @details
 *
 *     Function: RgMacSchSpsRelInd
 *
 *     Handler for processing relInd for UL SPS of a UE
 *
 *     Invoked by: 
 *         MAC
 *
 *     Processing Steps:
 *           
 *  @param[in] Pst               *pst
 *  @param[in] RgInfSpsRelInfo   *relInfo
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED 
 **/
#ifdef ANSI
PUBLIC S16 RgMacSchSpsRelInd
(
Pst                *pst, 
RgInfSpsRelInfo    *relInfo
)
#else
PUBLIC S16 RgMacSchSpsRelInd(pst, relInfo)
Pst                *pst;
RgInfSpsRelInfo    *relInfo;
#endif
{
   RgSchUeCb       *ue;
   RgSchCellCb     *cell;
   Inst            inst = (pst->dstInst - RGSCH_INST_START);

   TRC2(RgMacSchSpsRelInd);

   /* No need to chk for cell being NULL as MAC wouldn't have found instance if
    * it doesnt exist */
   cell = rgSchCb[inst].rgrSap[relInfo->cellSapId].cell;

   if ((ue = rgSCHDbmGetUeCb(cell, relInfo->cRnti)) == NULLP)
   {
      RLOG_ARG1(L_ERROR,DBG_CELLID,cell->cellId, 
           "No Ue exists with CRNTI:%d",relInfo->cRnti);
      RETVALUE(RFAILED);
   }

   if ((rgSCHUtlSpsRelInd(cell, ue, relInfo->isExplRel)) != ROK)
   {
      RLOG_ARG1(L_ERROR,DBG_CELLID,cell->cellId, 
           "RelInd processing for CRNTI:%d failed",relInfo->cRnti);
      RETVALUE(RFAILED);
   }
   RETVALUE(ROK);
} /* end of RgMacSchSpsRelInd */
#endif /* LTEMAC_SPS */

#ifdef LTE_L2_MEAS

/**
 * @brief Function to handle L2MeasCfm from MAC
 *
 * @details
 *
 *     Function: RgMacSchL2MeasCfm
 *
 *     Handler for processing L2 measurement confirm 
 *
 *     Invoked by: 
 *         MAC
 *
 *     Processing Steps:
 *           
 *  @param[in] Pst               *pst
 *  @param[in] RgInfL2MeasCfm    *measCfm
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED 
 **/
#ifdef ANSI
PUBLIC S16 RgMacSchL2MeasCfm
(
Pst                *pst, 
RgInfL2MeasCfm     *measCfm
)
#else
PUBLIC S16 RgMacSchL2MeasCfm(pst, measCfm)
Pst                *pst;
RgInfL2MeasCfm     *measCfm;
#endif
{
   RgSchCellCb       *cell = NULLP;
   Inst              inst = (pst->dstInst - RGSCH_INST_START);
   CmLList           *lnk;
   RgSchL2MeasCb     *measCb = NULLP;
   RgSchCb           *instCb =  &rgSchCb[inst];
   U32               idx;
   LrgSchMeasCfmInfo schMeasCfm;
   U8                qciVal;
   U8                idx1; /*LTE_L2_MEAS_PHASE2*/ 
   U8                qciVal1;
   TRC2(RgMacSchL2MeasCfm);

   /* Find the cellCb using cellId in measInfo. Iterate through all cells
    * in rgrsapCb in RgschCb */
   for (idx = 0; idx < instCb->numSaps; idx++)
   {
      if ( instCb->rgrSap[idx].cell->cellId == measCfm->cellId)
      {
         /* got the cell break the loop */
         cell = instCb->rgrSap[idx].cell;
         break;
      }
   }
   /* If no cellCb return Err with Invalid Cell Id */
   if (cell == NULLP)
   {
      RLOG_ARG0(L_ERROR,DBG_CELLID,measCfm->cellId,
                  "Meas Cfm Failed.Invalid Cell Id");
      RETVALUE(RFAILED);
   }
   

   /* Iterate through all meas requests in l2mList in cellCb */
   lnk = cell->l2mList.first;
   while(lnk != NULLP)
   {
      /* Get the MeasCb : RgSchL2MeasCb */
      measCb = (RgSchL2MeasCb *)lnk->node;
      lnk = lnk->next;
      if (measCb->measReq.hdr.transId == measCfm->transId)
      {
         break;
      }
   }
   if ( measCb == NULLP )
   {
      RETVALUE( RFAILED );
   }


   if(measCfm->cfm.status != LCM_PRIM_OK)
   {
      for (idx = 0; idx < measCb->measReq.avgPrbQciUl.numQci; idx++)
      {
        qciVal = measCb->measReq.avgPrbQciUl.qci[idx];
        cell->qciArray[qciVal].qci = 0;
      }
      /* Delete this measCb, send the negative confirmation to
       * stack manager */
      cmLListDelFrm(&cell->l2mList, &measCb->measLnk);
      /* ccpu00117052 - MOD - Passing double pointer
         for proper NULLP assignment*/
      rgSCHUtlFreeSBuf(cell->instIdx, (Data **)&measCb, sizeof(RgSchL2MeasCb));
      cmMemset((U8 *)&schMeasCfm, 0, sizeof(LrgSchMeasCfmInfo));
      schMeasCfm.measType     = measCfm->measType;
      schMeasCfm.cfm          = measCfm->cfm;
      schMeasCfm.hdr.transId  = measCfm->transId;
      schMeasCfm.cellId       = measCfm->cellId;
      RgMiLrgSchL2MeasCfm(&(instCb->rgSchInit.lmPst), &schMeasCfm);
      RETVALUE(ROK);
   }
   for(idx = 0; idx < measCfm->u.prbCfm.numQci; idx++)
   {
      measCb->avgPrbQciUl.prbUsage[idx].prbUsage = measCfm->u.prbCfm.prbUsage[idx].prbUsage;
      measCb->avgPrbQciUl.prbUsage[idx].qciValue = measCfm->u.prbCfm.prbUsage[idx].qciValue;
      /*LTE_L2_MEAS_PHASE2*/
      qciVal1 = measCfm->u.prbCfm.prbUsage[idx].qciValue;
      for(idx1=0;idx1<measCb->measReq.avgPrbQciUl.numQci;idx1++)
      {
         if(measCb->measReq.avgPrbQciUl.qci[idx1] == qciVal1)
         {
            break;
         }
      }
      if(idx1 == measCb->measReq.avgPrbQciUl.numQci)
      {
         measCb->measReq.avgPrbQciUl.qci[measCb->measReq.avgPrbQciUl.numQci++] = qciVal1;
      }
   }
   measCb->avgPrbQciUl.numQci = measCfm->u.prbCfm.numQci;
   measCb->cfmRcvd = TRUE;
   cell->sndL2Meas = TRUE;
   RETVALUE(ROK);
} /* end of RgMacSchL2MeasCfm */

/**
 * @brief Function to handle L2MeasStopCfm from MAC
 *
 * @details
 *
 *     Function: RgMacSchL2MeasStopCfm
 *
 *     Handler for processing L2 measurement confirm 
 *
 *     Invoked by: 
 *         MAC
 *
 *     Processing Steps:
 *           
 *  @param[in] Pst               *pst
 *  @param[in] RgInfL2MeasCfm    *measCfm
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED 
 **/
#ifdef ANSI
PUBLIC S16 RgMacSchL2MeasStopCfm
(
Pst                *pst,
RgInfL2MeasCfm     *measCfm
)
#else
PUBLIC S16 RgMacSchL2MeasStopCfm(pst, measCfm)
Pst                *pst;
RgInfL2MeasCfm     *measCfm;
#endif
{
   LrgSchMeasCfmInfo schMeasCfm;
   Inst              inst = (pst->dstInst - RGSCH_INST_START);
   RgSchCb           *instCb =  &rgSchCb[inst];

   TRC2(RgMacSchL2MeasStopCfm);

   cmMemset((U8 *)&schMeasCfm, 0, sizeof(LrgSchMeasCfmInfo));
   schMeasCfm.measType     = measCfm->measType;
   schMeasCfm.cfm          = measCfm->cfm;
   schMeasCfm.hdr.transId  = measCfm->transId;
   schMeasCfm.cellId       = measCfm->cellId;
   RgMiLrgSchL2MeasStopCfm(&(instCb->rgSchInit.lmPst), &schMeasCfm);
   RETVALUE(ROK);
}
#endif

/************** TFU Interface *************/

/**
 * @brief Bind confirm API for TFU SAP on scheduler instance. 
 *
 * @details
 *
 *     Function : RgLiTfuSchBndCfm
 *      
 *      This API is invoked by PHY to confirm TFU SAP bind. 
 *     
 *           
 *  @param[in]  Pst   *pst 
 *  @param[in]  SuId  suId 
 *  @param[in]  U8    status
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED 
 **/
#ifdef ANSI
PUBLIC S16 RgLiTfuSchBndCfm 
(
Pst     *pst,
SuId    suId, 
U8      status
)
#else
PUBLIC S16 RgLiTfuSchBndCfm(pst, suId, status)
Pst     *pst; 
SuId    suId; 
U8      status;
#endif
{
   S16 ret;
   RgSchLowSapCb  *tfuSap;
   Inst  instId = pst->dstInst - RGSCH_INST_START;

   TRC3(RgLiTfuSchBndCfm);


   if(suId >= rgSchCb[instId].numSaps)
   {
      RLOG_ARG0(L_ERROR,DBG_INSTID,instId, "Incorrect SuId");
      RETVALUE(RFAILED);
   }
   /* Lets validate suId first */
   tfuSap = &(rgSchCb[instId].tfuSap[suId]);

   if (suId != tfuSap->sapCfg.suId)
   {
      RLOG_ARG2(L_ERROR,DBG_INSTID,instId, "Incorrect SuId. Configured (%d)"
            "Recieved (%d)", tfuSap->sapCfg.suId, suId);
      RETVALUE(RFAILED);
   }
   ret = rgSCHLmmBndCfm (pst, suId, status);
   RETVALUE(ret);
}  /* RgLiTfuSchBndCfm */

/**
 * @brief Random Access Request indication from PHY.
 *
 * @details
 *
 *     Function : RgLiTfuRaReqInd
 *      
 *      This API is invoked by PHY to send Random Access Request to Scheduler.
 *      This API contains information for Random Access Request including 
 *      raRnti, list of associated RAPIDs and related information.
 *           
 *  @param[in]  Pst              *pst 
 *  @param[in]  SuId             suId 
 *  @param[in]  TfuRaReqIndInfo  *raReqInd
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED 
 **/
#ifdef ANSI
PUBLIC S16 RgLiTfuRaReqInd
(
Pst              *pst, 
SuId             suId, 
TfuRaReqIndInfo  *raReqInd
)
#else
PUBLIC S16 RgLiTfuRaReqInd(pst, suId, raReqInd)
Pst              *pst; 
SuId             suId; 
TfuRaReqIndInfo  *raReqInd;
#endif
{
   S16   ret;
   Inst  inst = pst->dstInst-RGSCH_INST_START;

   TRC3(RgLiTfuRaReqInd);

   if ((ret = rgSCHUtlValidateTfuSap (inst, suId)) != ROK)
   {
      RLOG_ARG1(L_ERROR,DBG_INSTID,inst,"SAP Validation failed SuId(%d)", suId);
      /* Free up the memory for the request structure */
      RGSCH_FREE_MEM(raReqInd);
      RETVALUE(ret);
   }

   if(raReqInd == NULLP)
   {
      RLOG_ARG0(L_ERROR,DBG_INSTID,inst,"Invalid input pointer for raReqInd Failed");
      RETVALUE(RFAILED);
   }

   if (rgSchCb[inst].tfuSap[suId].cell == NULLP)
   {
      RLOG_ARG0(L_ERROR,DBG_CELLID,raReqInd->cellId,"No cell exists");
      RETVALUE(RFAILED);
   }

   ret = rgSCHTomRaReqInd(rgSchCb[inst].tfuSap[suId].cell, raReqInd);
   /* Free up the memory for the request structure */
   RGSCH_FREE_MEM(raReqInd);
   /*SPutSBuf (pst->region, pst->pool, (Data *)raReqInd,
         sizeof(TfuRaReqIndInfo)); */
   RETVALUE(ret);
}  /* RgLiTfuRaReqInd */

/**
 * @brief Uplink CQI indication from PHY.
 *
 * @details
 *
 *     Function : RgLiTfuUlCqiInd
 *      
 *      This API is invoked by PHY to send Uplink CQI to Scheduler.
 *      This API contains Uplink CQI information reported per UE.
 *           
 *  @param[in]  Pst               *pst 
 *  @param[in]  SuId              suId 
 *  @param[in]  TfuUlCqiIndInfo   *ulCqiInd
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED 
 **/
#ifdef ANSI
PUBLIC S16 RgLiTfuUlCqiInd
(
Pst              *pst, 
SuId             suId, 
TfuUlCqiIndInfo  *ulCqiInd
)
#else
PUBLIC S16 RgLiTfuUlCqiInd(pst, suId, ulCqiInd)
Pst              *pst; 
SuId             suId; 
TfuUlCqiIndInfo  *ulCqiInd;
#endif
{
   S16   ret;
   Inst  inst = pst->dstInst-RGSCH_INST_START;

   TRC3(RgLiTfuUlCqiInd);

   if ((ret = rgSCHUtlValidateTfuSap (inst, suId)) != ROK)
   {
      RLOG_ARG0(L_ERROR,DBG_INSTID,inst,"SAP Validation failed");
      /* Free up the memory for the request structure */
      RGSCH_FREE_MEM(ulCqiInd);
      RETVALUE(ret);
   }

   if (rgSchCb[inst].tfuSap[suId].cell == NULLP)
   {
      RLOG_ARG0(L_ERROR,DBG_INSTID,inst,"No cell exists");
      /* Free up the memory for the request structure */
      RGSCH_FREE_MEM(ulCqiInd);
      RETVALUE(RFAILED);
   }
   ret = rgSCHTomUlCqiInd (rgSchCb[inst].tfuSap[suId].cell, ulCqiInd);
   /* Free up the memory for the request structure */
   RGSCH_FREE_MEM(ulCqiInd);
   RETVALUE(ret);
}  /* RgLiTfuUlCqiInd */

/**
 * @brief PUCCH power adjustment indication.
 *
 * @details
 *
 *     Function : RgLiTfuPucchDeltaPwrInd
 *
 *      This API is invoked by PHY to send PUCCH power adjustment
 *      indication.
 *
 *  @param[in]  Pst                     *pst 
 *  @param[in]  SuId                    suId 
 *  @param[in]  TfuPucchDeltaPwrIndInfo *pucchDeltaPwr
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED 
 **/
#ifdef ANSI
PUBLIC S16 RgLiTfuPucchDeltaPwrInd
(
Pst                     *pst,
SuId                    suId,
TfuPucchDeltaPwrIndInfo *pucchDeltaPwr
)
#else
PUBLIC S16 RgLiTfuPucchDeltaPwrInd(pst, suId, pucchDeltaPwr)
Pst                     *pst;
SuId                    suId;
TfuPucchDeltaPwrIndInfo *pucchDeltaPwr;
#endif
{
   S16   ret;
   Inst  inst = pst->dstInst-RGSCH_INST_START;

   TRC3(RgLiTfuPucchDeltaPwrInd);

   if ((ret = rgSCHUtlValidateTfuSap (inst, suId)) != ROK)
   {
      RLOG_ARG0(L_ERROR,DBG_INSTID,inst,"SAP Validation failed");
      /* Free up the memory for the request structure */
      RGSCH_FREE_MEM(pucchDeltaPwr);
      RETVALUE(ret);
   }

   if (rgSchCb[inst].tfuSap[suId].cell == NULLP)
   {
      RLOG_ARG0(L_ERROR,DBG_INSTID,inst,"No cell exists");
      RETVALUE(RFAILED);
   }
   ret = rgSCHTomPucchDeltaPwrInd (rgSchCb[inst].tfuSap[suId].cell, pucchDeltaPwr);
   /* Free up the memory for the request structure */
   RGSCH_FREE_MEM(pucchDeltaPwr);
   RETVALUE(ret);
}  /* RgLiTfuPucchDeltaPwrInd */


/**
 * @brief HARQ ACK indication from PHY for Downlink transmissions.
 *
 * @details
 *
 *     Function : RgLiTfuHqInd
 *      
 *      This API is invoked by PHY to send HARQ ACK information to Scheduler
 *      on recieving HARQ ACK/NACK from UEs.
 *      This API contains HARQ ACK information recieved by PHY for downlink
 *      transmissions.
 *           
 *  @param[in]  Pst                *pst
 *  @param[in]  SuId               suId 
 *  @param[in]  TfuHqIndInfo       *harqAckInd
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED 
 **/
#ifdef ANSI
PUBLIC S16 RgLiTfuHqInd
(
Pst                *pst, 
SuId               suId, 
TfuHqIndInfo       *harqAckInd
)
#else
PUBLIC S16 RgLiTfuHqInd(pst, suId, harqAckInd)
Pst                *pst; 
SuId               suId; 
TfuHqIndInfo       *harqAckInd;
#endif
{
   S16   ret;
   Inst  inst = (pst->dstInst - RGSCH_INST_START);

   TRC3(RgLiTfuHqInd);

#ifndef NO_ERRCLS
   if ((ret = rgSCHUtlValidateTfuSap (inst, suId)) != ROK)
   {
      RLOG_ARG0(L_ERROR,DBG_INSTID,inst,"SAP Validation failed");
      RGSCH_FREE_MEM(harqAckInd);
      RETVALUE(ret);
   }

   if (rgSchCb[inst].tfuSap[suId].cell == NULLP)
   {
      RLOG_ARG0(L_ERROR,DBG_INSTID,inst,"No cell exists");
      RETVALUE(RFAILED);
   }
#endif

   /* Now call the TOM (Tfu ownership module) primitive to process further */
   ret = rgSCHTomHarqAckInd (rgSchCb[inst].tfuSap[suId].cell, harqAckInd);
   /* Free up the memory for the request structure */
   RGSCH_FREE_MEM(harqAckInd);
   RETVALUE(ret);
}  /* RgLiTfuHqInd */


/**
 * @brief Scheduling request(SR) indication from PHY for an UE.
 *
 * @details
 *
 *     Function : RgLiTfuSrInd
 *      
 *      This API is invoked by PHY to send Scheduling request information to
 *      Scheduler on recieving SR from a list of UEs.
 *      This API contains scheduling request information recieved by PHY for UEs.
 *           
 *  @param[in]  Pst           *pst
 *  @param[in]  SuId          suId 
 *  @param[in]  TfuSrIndInfo  *srInd
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED 
 **/
#ifdef ANSI
PUBLIC S16 RgLiTfuSrInd
(
Pst                *pst, 
SuId               suId, 
TfuSrIndInfo       *srInd
)
#else 
PUBLIC S16 RgLiTfuSrInd(pst, suId, srInd)
Pst                *pst; 
SuId               suId; 
TfuSrIndInfo       *srInd;
#endif
{
   S16   ret;
   Inst  inst = pst->dstInst-RGSCH_INST_START;

   TRC3(RgLiTfuSrInd);

#ifndef NO_ERRCLS
   if ((ret = rgSCHUtlValidateTfuSap (inst, suId)) != ROK)
   {
      RLOG_ARG0(L_ERROR,DBG_INSTID,inst,"() SAP Validation failed");
      RGSCH_FREE_MEM(srInd);
      RETVALUE(ret);
   }

   if (rgSchCb[inst].tfuSap[suId].cell == NULLP)
   {
      RLOG_ARG0(L_ERROR,DBG_INSTID,inst,"RgLiTfuSrInd()No cell exists");
      RETVALUE(RFAILED);
   }
#endif
   /* Now call the TOM (Tfu ownership module) primitive to process further */
   ret = rgSCHTomSrInd (rgSchCb[inst].tfuSap[suId].cell, srInd);
   /* Free up the memory for the request structure */
   RGSCH_FREE_MEM(srInd);
   RETVALUE(ret);
}  /* RgLiTfuSrInd */


/**
 * @brief Downlink CQI indication from PHY for an UE.
 *
 * @details
 *
 *     Function : RgLiTfuDlCqiInd
 *      
 *      This API is invoked by PHY to send Downlink CQI indication to Scheduler
 *      on recieving downlink CQI from UE.
 *      This API contains downlink CQI information recieved by PHY for an UE.
 *           
 *  @param[in]  Pst              *pst
 *  @param[in]  SuId             suId 
 *  @param[in]  TfuDlCqiIndInfo  *dlCqiInd
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED 
 **/
#ifdef ANSI
PUBLIC S16 RgLiTfuDlCqiInd
(
Pst                *pst, 
SuId               suId, 
TfuDlCqiIndInfo    *dlCqiInd
)
#else
PUBLIC S16 RgLiTfuDlCqiInd(pst, suId, dlCqiInd)
Pst                *pst; 
SuId               suId; 
TfuDlCqiIndInfo    *dlCqiInd;
#endif
{
   S16   ret;
   Inst  inst = pst->dstInst-RGSCH_INST_START;

   TRC3(RgLiTfuDlCqiInd);

   if ((ret = rgSCHUtlValidateTfuSap (inst, suId)) != ROK)
   {
      RLOG_ARG0(L_ERROR,DBG_INSTID,inst," SAP Validation failed");
      /* Free up the memory for the request structure */
      RGSCH_FREE_MEM(dlCqiInd);
      RETVALUE(ret);
   }

   if (rgSchCb[inst].tfuSap[suId].cell == NULLP)
   {
      RLOG_ARG0(L_ERROR,DBG_INSTID,inst,"No cell exists");
      RETVALUE(RFAILED);
   }
   ret = rgSCHTomDlCqiInd (rgSchCb[inst].tfuSap[suId].cell, dlCqiInd);
   /* Free up the memory for the request structure */
   RGSCH_FREE_MEM(dlCqiInd);
   RETVALUE(ret);
}  /* RgLiTfuDlCqiInd */
#ifdef TFU_UPGRADE

/**
 * @brief Raw CQI indication from PHY for an UE.
 *
 * @details
 *
 *     Function : RgLiTfuRawCqiInd
 *      
 *      This API is invoked by PHY to send Raw CQI indication to Scheduler
 *      on receiving Raw CQI from UE.
 *      This API contains Raw CQI information recieved by PHY for an UE.
 *           
 *  @param[in]  Pst              *pst
 *  @param[in]  SuId             suId 
 *  @param[in]  TfuRawCqiIndInfo  *rawCqiInd
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED 
 **/
#ifdef ANSI
PUBLIC S16 RgLiTfuRawCqiInd
(
Pst                *pst, 
SuId               suId, 
TfuRawCqiIndInfo    *rawCqiInd
)
#else
PUBLIC S16 RgLiTfuRawCqiInd(pst, suId, rawCqiInd)
Pst                *pst; 
SuId               suId; 
TfuRawCqiIndInfo    *rawCqiInd;
#endif
{
   S16   ret;
   Inst  inst = pst->dstInst-RGSCH_INST_START;

   TRC3(RgLiTfuRawCqiInd);

   if ((ret = rgSCHUtlValidateTfuSap (inst, suId)) != ROK)
   {
      RLOG_ARG0(L_ERROR,DBG_INSTID,inst,"SAP Validation failed");
      /* Free up the memory for the request structure */
      RGSCH_FREE_MEM(rawCqiInd);
      RETVALUE(ret);
   }

   if (rgSchCb[inst].tfuSap[suId].cell == NULLP)
   {
      RLOG_ARG0(L_ERROR,DBG_INSTID,inst," No cell exists");
      RETVALUE(RFAILED);
   }
   ret = rgSCHTomRawCqiInd (rgSchCb[inst].tfuSap[suId].cell, rawCqiInd);
   /* Free up the memory for the request structure */
   RGSCH_FREE_MEM(rawCqiInd);
   RETVALUE(ret);
}  /* RgLiTfuRawCqiInd */

/**
 * @brief SRS indication from PHY for an UE.
 *
 * @details
 *
 *     Function : RgLiTfuSrsInd
 *      
 *      This API is invoked by PHY to send UL SRS indication to Scheduler
 *      on receiving a SRS from UE.
 *      This API contains SRS information recieved by PHY for an UE.
 *           
 *  @param[in]  Pst              *pst
 *  @param[in]  SuId             suId 
 *  @param[in]  TfuSrsIndInfo  *srsInd
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED 
 **/
#ifdef ANSI
PUBLIC S16 RgLiTfuSrsInd
(
Pst                *pst, 
SuId               suId, 
TfuSrsIndInfo    *srsInd
)
#else
PUBLIC S16 RgLiTfuSrsInd(pst, suId, srsInd)
Pst                *pst; 
SuId               suId; 
TfuSrsIndInfo    *srsInd;
#endif
{
   S16   ret;
   Inst  inst = pst->dstInst-RGSCH_INST_START;

   TRC3(RgLiTfuSrsInd);

   if ((ret = rgSCHUtlValidateTfuSap (inst, suId)) != ROK)
   {
      RLOG_ARG0(L_ERROR,DBG_INSTID,inst," SAP Validation failed");
      /* Free up the memory for the request structure */
      RGSCH_FREE_MEM(srsInd);
      RETVALUE(ret);
   }

   if (rgSchCb[inst].tfuSap[suId].cell == NULLP)
   {
      RLOG_ARG0(L_ERROR,DBG_INSTID,inst,"No cell exists");
      RETVALUE(RFAILED);
   }
   ret = rgSCHTomSrsInd (rgSchCb[inst].tfuSap[suId].cell, srsInd);
   /* Free up the memory for the request structure */
   RGSCH_FREE_MEM(srsInd);
   RETVALUE(ret);
}  /* RgLiTfuSrsInd */

#endif 

/**
 * @brief DOA indication from PHY for an UE.
 *
 * @details
 *
 *     Function : RgLiTfuDoaInd
 *      
 *      This API is invoked by PHY to send Direction Of Arrival to Scheduler
 *      on calculating DOA at PHYSICAL LAYER for an UE.
 *      This API contains DOA information calculated by PHY for an UE.
 *           
 *  @param[in]  Pst              *pst
 *  @param[in]  SuId             suId 
 *  @param[in]  TfuDoaIndInfo    *doaInd
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED 
 **/
#ifdef ANSI
PUBLIC S16 RgLiTfuDoaInd 
(
Pst                *pst, 
SuId               suId, 
TfuDoaIndInfo      *doaInd
)
#else
PUBLIC S16 RgLiTfuDoaInd(pst, suId, doaInd)
Pst                *pst; 
SuId               suId; 
TfuDoaIndInfo      *doaInd;
#endif
{
   S16   ret;
   Inst  inst = pst->dstInst-RGSCH_INST_START;

   TRC2(RgLiTfuDoaInd);

   if ((ret = rgSCHUtlValidateTfuSap (inst, suId)) != ROK)
   {
      RLOG_ARG0(L_ERROR,DBG_INSTID,inst,"SAP Validation failed");
      /* Free up the memory for the request structure */
      RGSCH_FREE_MEM(doaInd);
      RETVALUE(ret);
   }

   if (rgSchCb[inst].tfuSap[suId].cell == NULLP)
   {
      RLOG_ARG0(L_ERROR,DBG_INSTID,inst,"No cell exists");
      RETVALUE(RFAILED);
   }
   ret = rgSCHTomDoaInd (rgSchCb[inst].tfuSap[suId].cell, doaInd);
   /* Free up the memory for the request structure */
   RGSCH_FREE_MEM(doaInd);
   RETVALUE(ret);
}  /* RgLiTfuDlCqiInd */

/**
 * @brief CRC indication from PHY.
 *
 * @details
 *
 *     Function : RgLiTfuCrcInd
 *      
 *      This API is invoked by PHY to give CRC indication to scheduler.
 *           
 *  @param[in]  Pst               *pst
 *  @param[in]  SuId              suId 
 *  @param[in]  TfuCrcIndInfo *crcInd
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED 
 **/
#ifdef ANSI
PUBLIC S16 RgLiTfuCrcInd
(
Pst                *pst, 
SuId               suId, 
TfuCrcIndInfo  *crcInd
)
#else
PUBLIC S16 RgLiTfuCrcInd (pst, suId, crcInd)
Pst                *pst; 
SuId               suId; 
TfuCrcIndInfo  *crcInd;
#endif
{
   S16              ret;
   Inst             inst      = pst->dstInst-RGSCH_INST_START;

   TRC3(RgLiTfuCrcInd);

#ifndef NO_ERRCLS
   if ((ret = rgSCHUtlValidateTfuSap (inst, suId)) != ROK)
   {
      RLOG_ARG0(L_ERROR,DBG_INSTID,inst,"SAP Validation failed");
      RGSCH_FREE_MEM(crcInd);
      RETVALUE(ret);
   }

   if (rgSchCb[inst].tfuSap[suId].cell == NULLP)
   {
      RLOG_ARG0(L_ERROR,DBG_INSTID,inst,"No cell exists");
      RETVALUE(RFAILED);
   }
#endif

   /* Now call the TOM (Tfu ownership module) primitive to process further */
   ret = rgSCHTomCrcInd(rgSchCb[inst].tfuSap[suId].cell, crcInd);
   /* Free up the memory for the request structure */
   RGSCH_FREE_MEM(crcInd);

   RETVALUE(ret);
}  /* RgLiTfuCrcInd */

/**
 * @brief Timing Advance indication from PHY.
 *
 * @details
 *
 *     Function : RgLiTfuTimingAdvInd
 *      
 *      This API is invoked by PHY to indicate timing advance to Scheduler for
 *       an UE.
 *           
 *  @param[in]  Pst                  *pst
 *  @param[in]  SuId                 suId 
 *  @param[in]  TfuTimingAdvIndInfo  *timingAdvInd
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED 
 **/
#ifdef ANSI
PUBLIC S16 RgLiTfuTimingAdvInd
(
Pst                  *pst, 
SuId                 suId, 
TfuTimingAdvIndInfo  *timingAdvInd
)
#else
PUBLIC S16 RgLiTfuTimingAdvInd(pst, suId, timingAdvInd)
Pst                  *pst; 
SuId                 suId; 
TfuTimingAdvIndInfo  *timingAdvInd;
#endif
{
   S16   ret;
   Inst  inst = pst->dstInst-RGSCH_INST_START;

   TRC3(RgLiTfuTimingAdvInd);

   if ((ret = rgSCHUtlValidateTfuSap (inst, suId)) != ROK)
   {
      RLOG_ARG0(L_ERROR,DBG_INSTID,inst,"SAP Validation failed");
      /* Free up the memory for the request structure */
      RGSCH_FREE_MEM(timingAdvInd);
      RETVALUE(ret);
   }

   if (rgSchCb[inst].tfuSap[suId].cell == NULLP)
   {
      RLOG_ARG0(L_ERROR,DBG_INSTID,inst,"No cell exists");
      RETVALUE(RFAILED);
   }
   /* Now call the TOM (Tfu ownership module) primitive to process further */
   ret = rgSCHTomTimingAdvInd(rgSchCb[inst].tfuSap[suId].cell, timingAdvInd);
   /* Free up the memory for the request structure */
   RGSCH_FREE_MEM(timingAdvInd);
   RETVALUE(ret);
}  /* RgLiTfuTimingAdvInd */


/**
 * @brief Transmission time interval indication from PHY.
 *
 * @details
 *
 *     Function : RgLiTfuSchTtiInd 
 *      
 *      This API is invoked by PHY to indicate TTI indication to Scheduler for
 *      a cell.
 *           
 *  @param[in]  Pst            *pst
 *  @param[in]  SuId           suId 
 *  @param[in]  TfuTtiIndInfo  *ttiInd
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED 
 **/
#ifdef ANSI
PUBLIC S16 RgLiTfuSchTtiInd 
(
Pst                 *pst, 
SuId                suId, 
TfuTtiIndInfo       *ttiInd
)
#else
PUBLIC S16 RgLiTfuSchTtiInd(pst, suId, ttiInd)
Pst                 *pst; 
SuId                suId; 
TfuTtiIndInfo       *ttiInd;
#endif
{
   S16   ret;
   Inst  inst = pst->dstInst-RGSCH_INST_START;

   TRC3(RgLiTfuSchTtiInd);

   
   /* Removing the validation with every TTI - for optimization */
#ifndef NO_ERRCLS 
   if ((ret = rgSCHUtlValidateTfuSap (inst, suId)) != ROK)
   {
      RLOG_ARG0(L_ERROR,DBG_INSTID,inst,"SAP Validation failed");
      RETVALUE(ret);
   }
#endif
   /* Moved check for cell outside ERRCLS*/

   if (rgSchCb[inst].tfuSap[suId].cell == NULLP)
   {
      RLOG_ARG0(L_ERROR,DBG_INSTID,inst,"No cell exists");
      RETVALUE(RFAILED);
   }
   /* Now call the TOM (Tfu ownership module) primitive to process further */
   ret = rgSCHTomTtiInd(rgSchCb[inst].tfuSap[suId].cell, suId, ttiInd);

   RETVALUE(ret);
}  /* RgLiTfuSchTtiInd */

/************* RGM  Interface ****************/
/**
 * @brief API for bind request from RRM towards MAC. 
 *
 * @details
 *
 *     Function: RgUiRgmBndReq
 *     
 *     This API is invoked by RRM towards MAC to bind RGM SAP. 
 *     These API validates the Pst, spId, suId and sends the bind confirm to
 *     RRM.
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
PUBLIC S16 RgUiRgmBndReq
(
Pst   *pst, 
SuId  suId,
SpId  spId
)
#else
PUBLIC S16 RgUiRgmBndReq(pst, suId, spId)
Pst   *pst; 
SuId  suId;
SpId  spId;
#endif
{
   S16       ret = ROK;
   Pst       tmpPst;   /* Temporary Post Structure */
   Inst      instId = pst->dstInst-RGSCH_INST_START;

   TRC3(RgUiRgmBndReq)
   
   
   tmpPst.prior       = pst->prior;
   tmpPst.route       = pst->route;
   tmpPst.selector    = pst->selector;
   tmpPst.region      = rgSchCb[instId].rgSchInit.region;
   tmpPst.pool        = rgSchCb[instId].rgSchInit.pool;

   tmpPst.srcProcId   = rgSchCb[instId].rgSchInit.procId;
   tmpPst.srcEnt      = rgSchCb[instId].rgSchInit.ent;
   tmpPst.srcInst     = rgSchCb[instId].rgSchInit.inst + RGSCH_INST_START;

   tmpPst.event       = EVTNONE;

   tmpPst.dstProcId   = pst->srcProcId;
   tmpPst.dstEnt      = pst->srcEnt;
   tmpPst.dstInst     = pst->srcInst;

   /*TODO remove follo statement*/
   rgSchCb[instId].rgmSap[spId].sapSta.sapState = LRG_UNBND;

   if(spId < rgSchCb[instId].numSaps)
   {
      /* Check the state of the SAP */
      switch (rgSchCb[instId].rgmSap[spId].sapSta.sapState)
      {
         /* This case might not be needed if SAP not configured then it will go
          * to else of above if condition */
         case LRG_UNBND: /* SAP is not bound */
	         RLOG0(L_DEBUG,"SAP is not yet bound");
            rgSchCb[instId].rgmSap[spId].sapSta.sapState = LRG_BND;
            rgSchCb[instId].rgmSap[spId].sapCfg.suId = suId;
            /* Send Bind Confirm with status as SUCCESS */
            ret = rgSCHUtlRgmBndCfm(instId, suId, CM_BND_OK);
             /*Indicate to Layer manager  */
            break;
         case LRG_BND: /* SAP is already bound*/
	         RLOG0(L_DEBUG,"SAP is already bound");
            ret = rgSCHUtlRgmBndCfm(instId, suId, CM_BND_OK);
            break;
         default: /* Should Never Enter here */
#if (ERRCLASS & ERRCLS_ADD_RES) 
            RGSCHLOGERROR(instId, ERRCLS_INT_PAR, ERG001, 
                 (ErrVal)rgSchCb[instId].rgmSap[spId].sapSta.sapState,
                  "Invalid SAP State:RgUiRgmBndReq failed\n");
#endif
            ret = rgSCHUtlRgmBndCfm(instId, suId, CM_BND_NOK);
            break;
      }
   }
   else
   {
#if (ERRCLASS & ERRCLS_ADD_RES)      
/* ccpu00117035 - MOD - Changed ErrVal argument from accessing sap state 
   to spId to avoid seg fault due to invalid sapID */
      RGSCHLOGERROR(instId, ERRCLS_INT_PAR, ERG002,
            (ErrVal)spId, "Invalid SAP Id:RgUiRrmBndReq failed\n");
#endif
      ret = RgUiRgmBndCfm(&tmpPst, suId, CM_BND_NOK);
   }
   RETVALUE(ret);
}  /* RgUiRgmBndReq */

/**
 * @brief API for unbind request from RRM towards MAC. 
 *
 * @details
 *
 *     Function: RgUiRgmUbndReq
 *     
 *     This API is invoked by RRM towards MAC to unbind RGM SAP. 
 *     These API validates the Pst, spId, suId and transfers the unbind request 
 *     specific information to corresponding ownership module (GOM) API.
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
PUBLIC S16 RgUiRgmUbndReq
(
Pst    *pst,
SpId   spId,
Reason reason
)
#else
PUBLIC S16 RgUiRgmUbndReq(pst, spId, reason)
Pst    *pst; 
SpId   spId;
Reason reason;
#endif
{
   Inst instId = pst->dstInst-RGSCH_INST_START;
   TRC3(RgUiRgmUbndReq)
   

   /* SAP Id validation */
   if (spId < rgSchCb[instId].numSaps)
   {
      switch(rgSchCb[instId].rgmSap[spId].sapSta.sapState)
      {
         case LRG_BND: /* SAP is already bound*/
            /* setting SAP state to UN BOUND */
	         RLOG0(L_DEBUG,"SAP is already bound");
            rgSchCb[instId].rgmSap[spId].sapSta.sapState = LRG_UNBND;
            break;
         default:
#if (ERRCLASS & ERRCLS_ADD_RES)      
            RGSCHLOGERROR(instId, ERRCLS_INT_PAR, ERG003,
                   (ErrVal)rgSchCb[instId].rgmSap[spId].sapSta.sapState,
                  "Invalid SAP State: RgUiRgmUbndReq failed\n");
#endif
            RETVALUE(RFAILED);
      }
   }
   else
   {
#if (ERRCLASS & ERRCLS_ADD_RES)      
      RGSCHLOGERROR(instId,ERRCLS_INT_PAR, ERG004, 
            (ErrVal)rgSchCb[instId].rgmSap[spId].sapSta.sapState,
            "Invalid SAP Id:RgUiRgmUbndReq failed\n");
#endif
      RETVALUE(RFAILED);
   }
   RETVALUE(ROK);
}  /* RgUiRgmUbndReq */


/**
 * @brief API for start or stop PRB reporting from RRM towards MAC. 
 *
 * @details
 *
 *     Function: RgUiRgmCfgPrbRprt
 *     
 *     This API is invoked by RRM towards MAC to start or stop calculating
 *     Average PRB usage in downlink and uplink. The average PRB usage will
 *     be reported to RRM based on confiured periodicity.
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
PUBLIC S16 RgUiRgmCfgPrbRprt
(
Pst   *pst, 
SpId  spId,
RgmPrbRprtCfg   *prbRprtCfg
)
#else
PUBLIC S16 RgUiRgmCfgPrbRprt(pst, spId, prbRprtCfg)
Pst   *pst; 
SpId  spId;
RgmPrbRprtCfg   *prbRprtCfg;
#endif
{
/* Initalize*/
   RgSchCellCb   *cell;
   RgSchPrbUsage *prbUsage;
   Inst          inst = (pst->dstInst  - RGSCH_INST_START);

   TRC2(RgUiRgmCfgPrbRprt);
   cell = rgSchCb[inst].rgmSap[spId].cell;
   prbUsage = &cell->prbUsage;
   prbUsage->prbRprtEnabld = prbRprtCfg->bConfigType;
   prbUsage->rprtPeriod = prbRprtCfg->usPrbAvgPeriodicty;
   RG_SCH_ADD_TO_CRNT_TIME(cell->crntTime, prbUsage->startTime, 1);

   /* clear the qciPrbRpts for all GBR QCIs */
   cmMemset((U8*)&prbUsage->qciPrbRpts[0], 0, 
             (RGM_MAX_QCI_REPORTS * sizeof(RgSchQciPrbUsage)));

   RLOG_ARG2(L_DEBUG,DBG_CELLID,cell->cellId,
	     "RgUiRgmCfgPrbRprt config type %d with the report period %d",
	     prbUsage->prbRprtEnabld,prbUsage->rprtPeriod);

   /* ccpu00134393 : mem leak fix */
      SPutSBuf(pst->region, pst->pool, (Data *)prbRprtCfg, sizeof(RgmPrbRprtCfg));
  
   RETVALUE(ROK);
}




/**********************************************************************
 
         End of file:     gk_sch.c@@/main/3 - Sat Jul 30 02:21:34 2011
 
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
/main/1      ---       apany 1. Initial Release.
/main/2      ---       apany 1. LTE MAC 2.1 release
             rg001.201 sm    1. Removed extra comments. 
             rg003.201 ns    1. Trace added for functions.
             rg004.201 ns    1. Memory Fix.
             rg004.201 rnt   1. Modified for SI Enhancement
             rg005.201 sm    1. Changes for optimization
             rg007.201   ap  1. Added support for MIMO
             rg008.201 rsharon 1. Added support for SPS.
             rg009.201 dv 1. Added changes of TFU_UPGRADE.
                          2. [ccpu00116677]: Moved Cell check 
                             outside ERRCLS.
             rg009.201 gj 3. [ccpu00116453] Added transId filling
                              in confirm messages
/main/3      ---        gvj  1. Updated for LTE MAC Rel 3.1           
$SID$        ---        rt   1. LTE MAC 4.1 release
*********************************************************************91*/
