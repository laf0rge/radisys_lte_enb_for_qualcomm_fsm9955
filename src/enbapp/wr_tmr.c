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

     Name:     EnodeB Application

     Type:     C source file

     Desc:     This file contains 
               

     File:     wr_tmr.c

     Sid:      $SID$ 

     Prg:      Sriky 

*********************************************************************21*/
/* header include files (.h) */
static const char* RLOG_MODULE_NAME="UMM";
static int RLOG_MODULE_ID=32;
static int RLOG_FILE_ID=80;


/* Header include files (.h) */
#include "ssi.h"
#include "wr.h" 
#include "wr_cmn.h"
#include "wr_emm.h"
#include "wr_umm.h"
#include "wr_umm_trans.h"
#include "wr_dam.h"
#include "wr_umm_msg.h"
/* Start - Fix for ccpu00123981 */
#include "wr_smm_init.h"
/* End - Fix for ccpu00123981 */
#include "wr_kpi.h"
#include "wr_emm_neighenb.h"
#include "wr_emm_mme.h"
#include "wr_umm_s1srcho.h"
/* LTE_ADV_ABS starts */
#include "wr_emm_lteadv.h"
/* LTE_ADV_ABS ends */
#include "wr_emm_rim.h"
#include "wr_emm_cfg.h"
#ifdef LTE_L2_MEAS
extern    U32 glbMacMeasFlag;
EXTERN SmCb smCb;
#endif
PRIVATE Void wrProcTmr (Ptr cb, S16 tmrEvnt);

/** @brief This function registers and initializes the timer
 *         queue structures in LTE eNodeB during general config.
 *
 * @details
 *
 *     Function: wrRegInitTmr
 *
 *         Processing steps:
 *
 * @param[in] Void 
 * @return S16
 *        -# Success : ROK
 */
PUBLIC S16 wrRegInitTmr
(
Void
)
{
   U16                       idx;
   S16                       ret;

   ret = SRegTmrMt((Ent)ENTWR, (Inst)0, wrCb.timeRes, wrActvTmr);
   if (ret != ROK)
   {
      RLOG0(L_ERROR, "SRegTmr Failed");
      RETVALUE(RFAILED);
   }

   /* initialize timing queues */
   wrCb.tqCp.tmrLen      = WR_NUM_TQENTRY;

   for (idx = 0; idx < WR_NUM_TQENTRY; idx++)
   {
      wrCb.tq[idx].first    = NULLP;
      wrCb.tq[idx].tail     = NULLP;
   }

   RETVALUE(ROK);

} /* wrRegInitTmr */


/** @brief Invoked by system services to activate a task with
 *              a timer tick.
 *
 * @details
 *
 *     Function: wrActvTmr
 *
 *         Processing steps:
 *
 * @param[in] Void 
 * @return S16
 *        -# Success : ROK
 */

PUBLIC S16 wrActvTmr
(
 Ent ent,
 Inst  inst
)
{
   cmPrcTmr(&wrCb.tqCp, wrCb.tq, (PFV) wrProcTmr);

   RETVALUE(ROK);
} /* end of wrActvTmr */


/** @brief This function is used to start a given timer. 
 *
 * @details
 *
 *     Function: wrStartTmr
 *
 *         Processing steps:
 *               This function based upon the timer event, derives the TimerCB 
 *               and adds it to the timer queue.
 *
 * @param[in] Cb : This holds approriate Control block for the timer event passed. 
 * @param[in] tmrEvent : One of the many possible timer types. 
 * @param[in] delay : Duration of the timer.
 * @return S16
 *        -# Success : ROK
 *        -# Failure : RFAILED
 */
PUBLIC S16 wrStartTmr
(
PTR                          cb,
S16                          tmrEvnt,
U32                          delay
)
{
   U32                       wait;
   CmTmrArg                  arg;
   CmTimer                   *tmr;
   U8                        maxTmrs;
   WrLiSapCb                 *sapCb;
   U32                       idx;
   WrCellCb                  *cellCb;
  /* PH04_CMAS_TMR */
   WrPwsMsgRepeatInfo        *repInfo;
 
   wait = 0;
   wait = WR_CALC_WAIT_TIME(delay);
   switch (tmrEvnt)
   {
      /* For SZT Bind Timer Timer */
      case WR_TMR_NHU_SAP_BND:
      case WR_TMR_RGR_SAP_BND:
      case WR_TMR_CTF_SAP_BND:
      case WR_TMR_SZT_SAP_BND:
      case WR_TMR_NLU_SAP_BND:
#ifdef RM_INTF
      case WR_TMR_RMU_SAP_BND: /*RRM->FSM sck changes*/
#endif
#ifdef LTE_HO_SUPPORT
      case WR_TMR_CZT_SAP_BND:
#endif
      {
         sapCb = ( WrLiSapCb*)cb;
         tmr      = &sapCb->timer;
         maxTmrs  = 1;
         break;
      }
      case WR_TMR_MME_SETUP_RSP:
      {
         WrMmeCb             *mmeCb = (WrMmeCb *)cb;

         tmr = &mmeCb->timer;
         maxTmrs = 1;
         break;
      }
      case WR_TMR_MME_SETUP_WAIT:
      {
         WrMmeCb             *mmeCb = (WrMmeCb *)cb;

         tmr = &mmeCb->timer;
         maxTmrs = 1;
         break;
      }
      case WR_TMR_ENB_SETUP_RSP:
      {
         WrNeighEnbCb      *nghEnbcb = (WrNeighEnbCb *)cb;

         tmr = &nghEnbcb->timer;
         maxTmrs = 1;
         break;
      }
      case WR_TMR_ENB_SETUP_WAIT:
      {
         WrNeighEnbCb      *nghEnbcb = (WrNeighEnbCb *)cb;

         tmr = &nghEnbcb->timer;
         maxTmrs = 1;
         break;
      }
      case WR_TMR_X2_RST_RSP:
      {
         WrNeighEnbCb      *nghEnbcb = (WrNeighEnbCb *)cb;

         tmr = &nghEnbcb->timer;
         maxTmrs = 1;
         break;
      }
      case WR_TMR_ENB_CONFIG_UPDATE_WAIT:
      {
         WrNeighEnbCb      *nghEnbcb = (WrNeighEnbCb *)cb;

         tmr = &nghEnbcb->enbCfgUpTm;
         maxTmrs = 1;
         break;
      }
      case WR_TMR_ENB_CFG_TRANSFER_WAIT:
      {
         WrNeighEnbCb      *nghEnbcb = (WrNeighEnbCb *)cb;

         tmr = &nghEnbcb->enbcfgTransTmr;
         maxTmrs = 1;
         break;
      }
      case WR_TMR_ANR_MEAS_EPOCH_TMR:
      {
         cellCb = (WrCellCb*)cb;
         tmr = &cellCb->nrCb->epochTmr;
         maxTmrs  = 1;
      }
      break;
      case WR_TMR_ANR_MEAS_TRICE_TMR:
      {
         cellCb = (WrCellCb*)cb;
         tmr = &cellCb->nrCb->triceTmr;
         maxTmrs  = 1;
      }
      break;
      case WR_TMR_ANR_CELL_MAX_AGE_TMR:
      {
         cellCb = (WrCellCb*)cb;
         tmr = &cellCb->nrCb->cellValidAgeTmr;
         maxTmrs  = 1;
      }
      break;
      case WR_TMR_GUARD_UE_CTX_REL:
      {

         WrUmmCellCb *ummCellCb = (WrUmmCellCb*)cb;
         tmr = &(ummCellCb->ueCtxRelGuardTmr);
         maxTmrs  = 1;
      }
      break;

      /* PH04_CMAS_TMR */
      case WR_TMR_REPETITION_PERIOD:
      {
         repInfo = (WrPwsMsgRepeatInfo *)cb;
         tmr      = &repInfo->repetitionTmr;
         maxTmrs  = 1;
      }
      break;
      case WR_TMR_ENB_CFG_RSP:
      {
         tmr     = &(wrEmmCb.enbCfgRspTmr);
         maxTmrs = 1;
      }
      break;
      case WR_TMR_ENB_CFG_WAIT:
      {
         tmr     = &(wrEmmCb.enbCfgWaitTmr);
         maxTmrs = 1;
      }
      break;
      case WR_TMR_RST_RSP:
      {
         tmr     = &(wrEmmCb.enbResetRspTmr);
         maxTmrs = 1;
      }
      break;
#ifdef ENB_OVRLOAD
      case WR_TMR_OVERLD_SAMPLE:
      {
        tmr = &wrEmmCb.smplTimer;
        maxTmrs  = 1;
      }
      break;
      case WR_TMR_OVERLD_STATUS:
      {
        tmr = &wrEmmCb.eNbOvrldStatUpd;
        maxTmrs  = 1;
      }
      break;
#endif
#ifdef WR_RSYS_KPI 
      case WR_TMR_KPI_PREDFND_INTRVL_TMR:
      {
        tmr = &(kpiInfo.kpiPredfndIntrvlTmr);
/*ccpu00138290*/
#ifndef WR_RSYS_OAM
        wrKpiResetKpiInfo(tmrEvnt);
#endif
        tmr->tmrEvnt = TMR_NONE;
        maxTmrs = 1;    
      }
      break;      
#endif/*WR_RSYS_KPI*/   
/* LTE_ADV_ABS starts */
      case WR_TMR_ABS_LOAD_INFO_TMR: 
      {
         tmr      = &((WrEmmAbsVctmPeerEnbCb*)cb)->tmr;
         maxTmrs  = 1;
      }
      break;
      case WR_TMR_RIM_OVERALL:
      {
         tmr     = &(wrEmmCb.rimOvrlTmr);
         maxTmrs  = 1;
      }
     break;
      case WR_TMR_RIM_TRIR: 
     {
        WrRimDstCellInfo *assocCellCb  = (WrRimDstCellInfo*)cb;
        tmr     = &(assocCellCb->timer);
        maxTmrs  = 1;
     }
     break;
     case WR_TMR_RIM_TRIAE:
     {
        WrRimDstCellInfo *assocCellCb  = (WrRimDstCellInfo*)cb;
        tmr     = &(assocCellCb->timer);
        maxTmrs  = 1;
     }
     break;
/* LTE_ADV_ABS ends */
      case WR_TMR_PCI_MOD_TMR :
      {
         cellCb = (WrCellCb *)cb;
         tmr = &(cellCb->pciModTmr);
         maxTmrs = 1;
      }
      break;
#ifdef LTE_L2_MEAS
      case WR_TMR_MEM_CPU_UTIL_INFO:
      {
         tmr = &(smCb.memCpuMeasCb.cpuMemUtilTmr);
         maxTmrs = 1;
      }
      break;
#endif
      default:
      {
         RLOG1(L_ERROR, "Invalid Timer event [%d] received", tmrEvnt);
         RETVALUE(RFAILED);
      }
   } /* end of switch */

   for(idx = 0; idx < maxTmrs; idx++)
   {
      if (tmr[idx].tmrEvnt == TMR_NONE)
      {
         break;
      }
   }
   if (idx >= maxTmrs)
   {
      RETVALUE(RFAILED);
   }

   /* initialize argument for common timer function */
   if(wait != 0)
   {
      arg.tqCp   = &(wrCb.tqCp);
      arg.tq     = wrCb.tq;
      arg.cb     = cb;
      arg.timers = tmr;
      arg.evnt   = tmrEvnt;
      arg.wait   = wait;
      arg.tNum   = 0;
      arg.max    = maxTmrs;

      cmPlcCbTq(&arg);
   }

   RETVALUE(ROK);
} /* end of wrStartTmr() */

PUBLIC S16 wrStartUeTmr
(
WrUeCb                       *ueCb,
S16                          tmrEvnt,
U32                          delay
)
{
   U32                       wait;
   CmTmrArg                  arg;
   CmTimer                   *tmr;
   U8                        maxTmrs;
   U32                       idx;
   WrUmmTimer                   *cb;

   wait = 0;
   wait = WR_CALC_WAIT_TIME(delay);
   switch (tmrEvnt)
   {
      case WR_TMR_TGTENB_RELOCEXE:
      {
         cb       = &ueCb->s1TgtEnbRelocTmr;
         ueCb->s1TgtEnbRelocTmr.cb = ueCb;
         tmr      = &ueCb->s1TgtEnbRelocTmr.tmr;
         maxTmrs  = 1;
         break;
      }
      case WR_TMR_S1_OVRL_TMR:
      {
         cb       = &ueCb->s1OvrallTmr;
         ueCb->s1OvrallTmr.cb = ueCb;
         tmr      = &ueCb->s1OvrallTmr.tmr;
         maxTmrs  = 1;
         break;
      }
      case WR_TMR_UE_HO_ATTACH_TMR:
      {
         cb   = &ueCb->ueHoAttachTmr;
         ueCb->ueHoAttachTmr.cb = ueCb;
         tmr = &ueCb->ueHoAttachTmr.tmr;
         maxTmrs = 1;
         break;
      }
      case WR_TMR_RAD_RES_MOD_TMR:
      {
         cb       = &ueCb->radResModTmr;
         ueCb->radResModTmr.cb = ueCb;
         tmr      = &ueCb->radResModTmr.tmr;
         maxTmrs  = 1;
         break;
      }
      case WR_TMR_INITIAL_CTX_REQ:
      {
         cb       = &ueCb->intialCntRspTmr;
         ueCb->intialCntRspTmr.cb = ueCb;
         tmr = &ueCb->intialCntRspTmr.tmr;
         maxTmrs  = 1;
     }
     break;
      case WR_TMR_UE_CTX_REL_REQ:
      {
         cb   = &ueCb->ueCtxRelTmr;
         ueCb->ueCtxRelTmr.cb = ueCb;
         tmr = &ueCb->ueCtxRelTmr.tmr;
         maxTmrs  = 1;
     }
     break;
#ifdef WR_DETECT_RLF
     case WR_TMR_DETECT_RLF:
      {
    cb   = &ueCb->cqiRprtTmr;
    ueCb->cqiRprtTmr.cb = ueCb;
         tmr      = &ueCb->cqiRprtTmr.tmr;
         maxTmrs  = 1;
         break;
      }
#endif

      default:
      {
         RLOG1(L_ERROR, "Invalid Timer event [%d] received", tmrEvnt);
         RETVALUE(RFAILED);
      }
   } /* end of switch */

   for(idx = 0; idx < maxTmrs; idx++)
   {
      if (tmr[idx].tmrEvnt == TMR_NONE)
      {
         break;
      }
   }
   if (idx >= maxTmrs)
   {
      RETVALUE(RFAILED);
   }

   /* initialize argument for common timer function */
   if(wait != 0)
   {
      arg.tqCp   = &(wrCb.tqCp);
      arg.tq     = wrCb.tq;
      arg.cb     = (PTR)cb;
      arg.timers = tmr;
      arg.evnt   = tmrEvnt;
      arg.wait   = wait;
      arg.tNum   = 0;
      arg.max    = maxTmrs;

      cmPlcCbTq(&arg);

      cb->tmrLnk.node = (PTR)cb;
      cmLListAdd2Tail(&ueCb->tmrLst,&cb->tmrLnk);
   }

   RETVALUE(ROK);
}

PUBLIC S16 wrStartTransTmr
(
WrUmmTransCb                 *transCb,
S16                          tmrEvnt,
U32                          delay
)
{
   U32                       wait;
   CmTmrArg                  arg;
   CmTimer                   *tmr;
   U8                        maxTmrs;
   U32                       idx;
   WrUmmTimer                   *cb;

   wait = 0;
   wait = WR_CALC_WAIT_TIME(delay);

   switch (tmrEvnt)
   {
      case WR_TMR_RRC_REL_GUARD:
      case WR_TMR_RRC_RRCCON:
      case WR_TMR_RRC_CONREL:
      case WR_TMR_RRC_CAPENQ:
      case WR_TMR_RRC_SECUCMD:
      case WR_TMR_REESTABLISH:
      case WR_TMR_RECONFIGURE:
      case WR_TMR_RRCCON_FAIL_RRC_UNDO_CFG:
      case WR_TMR_TGT_HOPREP:
         /* eCSFB - Added as part of ecsfb phase */
      case WR_TMR_RRC_DAT_CFM:
      case WR_TMR_TGT_PATH_SWITCH:
      case WR_TMR_UL_HO_PREP_WAIT_TMR:
      case WR_TMR_DL_CDMA_RECV_HO_WAIT_TMR:
      case WR_TMR_ECSFB_HO_EXEC_TMR:
      case WR_TMR_ANR_STRONGCELL_REPORT_R1:
      {
        cb = &transCb->timer;
       transCb->timer.cb = transCb;
         tmr      = &transCb->timer.tmr;
         maxTmrs  = 1;
        break;
      }
      case WR_TMR_CSFB_CFG:
      {
        cb       = &(transCb->u.csfbTrans.csfbCfgTmr);
        transCb->u.csfbTrans.csfbCfgTmr.cb = transCb;
        tmr      = &(transCb->u.csfbTrans.csfbCfgTmr.tmr);
        maxTmrs  = 1;
        break;
      }
     case WR_TMR_ECSFB_MEAS_RPT:
     {
        cb       = &(transCb->u.ecsfbTransCb.measRptTmr);
       transCb->u.ecsfbTransCb.measRptTmr.cb = transCb;
       tmr      = &(transCb->u.ecsfbTransCb.measRptTmr.tmr);
       maxTmrs  = 1;
       break;
     }
      case WR_TMR_SRC_HO_CANCEL:
      {
         cb       = &(transCb->u.s1SrcHoTrans.s1CancelTmr);
         transCb->u.s1SrcHoTrans.s1CancelTmr.cb = transCb;
         tmr      = &(transCb->u.s1SrcHoTrans.s1CancelTmr.tmr);
         maxTmrs = 1;
         break;
      }
      case WR_TMR_S1_RELOC_TMR:
      {
         cb       = &(transCb->u.s1SrcHoTrans.s1RelocTmr);
         transCb->u.s1SrcHoTrans.s1RelocTmr.cb = transCb;
         tmr      = &(transCb->u.s1SrcHoTrans.s1RelocTmr.tmr);
         maxTmrs  = 1;
         break;
      }
      case WR_TMR_CSFB_GSM_T304_EXP_WAIT: 
      {
         cb       = &(transCb->u.csfbGeranTrans.t304WaitTmr);
         transCb->u.csfbGeranTrans.t304WaitTmr.cb = transCb;
         tmr      = &(transCb->u.csfbGeranTrans.t304WaitTmr.tmr);
         maxTmrs  = 1;
         break;
      }
      case WR_TMR_CSFB_GSM_MEAS_WAIT:
      {
         cb       = &(transCb->u.csfbGeranTrans.measWaitTmr);
         transCb->u.csfbGeranTrans.measWaitTmr.cb = transCb;
         tmr      = &(transCb->u.csfbGeranTrans.measWaitTmr.tmr);
         maxTmrs  = 1;
         break;
      }
      default:
      {
         RLOG1(L_ERROR, "Invalid Timer event [%d] received", tmrEvnt);
         RETVALUE(RFAILED);
      }
   } /* end of switch */

   for(idx = 0; idx < maxTmrs; idx++)
   {
      if (tmr[idx].tmrEvnt == TMR_NONE)
      {
         break;
      }
   }
   if (idx >= maxTmrs)
   {
      RETVALUE(RFAILED);
   }

   /* initialize argument for common timer function */
   if(wait != 0)
   {
      arg.tqCp   = &(wrCb.tqCp);
      arg.tq     = wrCb.tq;
      arg.cb     = (PTR)cb;
      arg.timers = tmr;
      arg.evnt   = tmrEvnt;
      arg.wait   = wait;
      arg.tNum   = 0;
      arg.max    = maxTmrs;

      cmPlcCbTq(&arg);

      cb->tmrLnk.node = (PTR)cb;
      cmLListAdd2Tail(&transCb->tmrLst,&cb->tmrLnk);
   }

   RETVALUE(ROK);
}
/** @brief This function is used to stop a previously running timer. 
 *
 * @details
 *
 *     Function: wrStopTmr
 *
 *         Processing steps:
 *               This function based upon the timer event reterives relevant timerCb and 
 *               tries to determine whether timer is running or not.In case timer 
 *               was running, it is stopped .
 *
 * @param[in] Cb : This holds approriate Control block for the timer event passed. 
 * @param[in] tmrEvent : One of the many possible timer types. 
 * @return S16
 *        -# Success : ROK
 *        -# Failure : RFAILED
 */
PUBLIC Void wrStopTmr
(
PTR                          cb,
S16                          event
)
{
   CmTmrArg                  arg;
   U8                        idx;
   Bool                      tmrRunning;
   CmTimer                   *timers = NULLP;
   U8                        max = 0;
   WrLiSapCb                 *sapCb;
   WrCellCb                  *cellCb;
   /* PH04_CMAS_TMR */
   WrPwsMsgRepeatInfo        *repInfo;

   idx = 0;

   tmrRunning = FALSE;
   switch(event)
   {
      case WR_TMR_NHU_SAP_BND:
      case WR_TMR_RGR_SAP_BND:
      case WR_TMR_CTF_SAP_BND:
      case WR_TMR_SZT_SAP_BND:
      case WR_TMR_NLU_SAP_BND:
#ifdef RM_INTF
      case WR_TMR_RMU_SAP_BND: /*RRM->FSM sck changes*/
#endif
#ifdef LTE_HO_SUPPORT
      case WR_TMR_CZT_SAP_BND:
#endif
      {
         sapCb = (WrLiSapCb*)cb;
         max     =  1;
         if(sapCb->timer.tmrEvnt == event)
         {
            tmrRunning = TRUE;
            sapCb->bndRetryCnt = 0;
         }
         timers = &sapCb->timer;
         break;
      }
      case WR_TMR_MME_SETUP_RSP:
      {
         WrMmeCb             *mmeCb = (WrMmeCb *)cb;
         timers = &mmeCb->timer;
         max    = 1;
         if (mmeCb->timer.tmrEvnt == event)
         {
            tmrRunning = TRUE;
         }
         break;
      }
      case WR_TMR_MME_SETUP_WAIT:
      {
         WrMmeCb             *mmeCb = (WrMmeCb *)cb;
         timers = &mmeCb->timer;
         max    = 1;
         if (mmeCb->timer.tmrEvnt == event)
         {
            tmrRunning = TRUE;
         }
         break;
      }
      case WR_TMR_ENB_SETUP_RSP:
      {
         WrNeighEnbCb        *nghEnbcb = (WrNeighEnbCb *)cb;
         timers = &nghEnbcb->timer;
         max    = 1;
         if (nghEnbcb->timer.tmrEvnt == event)
         {
            tmrRunning = TRUE;
         }
         break;
      }
      case WR_TMR_ENB_SETUP_WAIT:
      {
         WrNeighEnbCb   *nghEnbcb = (WrNeighEnbCb *)cb;
         timers = &nghEnbcb->timer;
         max    = 1;
         if (nghEnbcb->timer.tmrEvnt == event)
         {
            tmrRunning = TRUE;
         }
         break;
      }
      case WR_TMR_X2_RST_RSP:
      {
         WrNeighEnbCb   *nghEnbcb = (WrNeighEnbCb *)cb;
         timers = &nghEnbcb->timer;
         max    = 1;
         if (nghEnbcb->timer.tmrEvnt == event)
         {
            tmrRunning = TRUE;
         }
         break;
      }
      case WR_TMR_ENB_CONFIG_UPDATE_WAIT:
      {
         WrNeighEnbCb      *nghEnbcb = (WrNeighEnbCb *)cb;
         timers = &nghEnbcb->enbCfgUpTm;
         max = 1;
         if ( nghEnbcb->enbCfgUpTm.tmrEvnt == event)
         {
            tmrRunning = TRUE;
         }
         break;
      }
      case WR_TMR_ENB_CFG_TRANSFER_WAIT:
      {
          WrNeighEnbCb      *nghEnbcb = (WrNeighEnbCb *)cb;
          timers = &nghEnbcb->enbcfgTransTmr;
          max = 1;
          if ( nghEnbcb->enbcfgTransTmr.tmrEvnt == event)
          {
             tmrRunning = TRUE;
          }
          break;
      }
       /* PH04_CMAS_TMR */
      case WR_TMR_REPETITION_PERIOD:
      {
         repInfo = (WrPwsMsgRepeatInfo *)cb;
         max  = 1;
         if(repInfo->repetitionTmr.tmrEvnt == event)
         {
            tmrRunning = TRUE;
         }
         timers = &repInfo->repetitionTmr;
         break;
      }/* PH04_CMAS_TMR : end */
      case WR_TMR_ENB_CFG_RSP:
      {
         max     = 1;
         if(wrEmmCb.enbCfgRspTmr.tmrEvnt == event)
         {
            tmrRunning = TRUE;
         }
         timers  = &(wrEmmCb.enbCfgRspTmr);
         break;
      }

      case WR_TMR_ENB_CFG_WAIT:
      {
         max     = 1;
         if(wrEmmCb.enbCfgWaitTmr.tmrEvnt == event)
         {
            tmrRunning = TRUE;
         }
         timers  = &(wrEmmCb.enbCfgWaitTmr);
         break;
      }
      case WR_TMR_RST_RSP:
      {
         max     = 1;
         if(wrEmmCb.enbResetRspTmr.tmrEvnt == event)

         {
            tmrRunning = TRUE;
         }
         timers  = &(wrEmmCb.enbResetRspTmr);
         break;
      }
      case WR_TMR_ANR_MEAS_EPOCH_TMR:
      {
         cellCb = (WrCellCb*)cb;
         if(cellCb->nrCb == NULLP)
         {
            tmrRunning = FALSE;
         }
         else
         {
            max     =  1;
            if(cellCb->nrCb->epochTmr.tmrEvnt == event)
            {
               tmrRunning = TRUE;
            }
            timers  = &(cellCb->nrCb->epochTmr);
         }
         break;
      }
      case WR_TMR_ANR_MEAS_TRICE_TMR:
      {
         cellCb = (WrCellCb*)cb;
         if(cellCb->nrCb == NULLP)
         {
            tmrRunning = FALSE;
         }
         else
         {
            max     =  1;
            if(cellCb->nrCb->triceTmr.tmrEvnt == event)
            {
               tmrRunning = TRUE;
            }
            timers  = &(cellCb->nrCb->triceTmr);
         }
         break;
      }
#ifdef WR_RSYS_KPI 
     /*Added for KPI*/
      case WR_TMR_KPI_PREDFND_INTRVL_TMR:
      {
        if(kpiInfo.kpiPredfndIntrvlTmr.tmrEvnt == event)
         {
           tmrRunning = TRUE;
         }
        timers = &(kpiInfo.kpiPredfndIntrvlTmr);
        max = 1;
      }
       break;
#endif/*WR_RSYS_KPI*/
/* LTE_ADV_ABS starts */
      case WR_TMR_ABS_LOAD_INFO_TMR:
      {
         max  = 1;
         if(((WrEmmAbsVctmPeerEnbCb*)cb)->tmr.tmrEvnt == event)
         {
            tmrRunning = TRUE;
         }
         timers = &((WrEmmAbsVctmPeerEnbCb*)cb)->tmr;
      }
      break;
/* LTE_ADV_ABS ends */
    case WR_TMR_RIM_OVERALL:
      {
         max     = 1;
         if(wrEmmCb.rimOvrlTmr.tmrEvnt == event)
         {
            tmrRunning = TRUE;
         }
         timers  = &(wrEmmCb.rimOvrlTmr);
      }
     break;
     case WR_TMR_RIM_TRIR: 
     {
        max     = 1;
        WrRimDstCellInfo *assocCellCb  = (WrRimDstCellInfo*)cb;
        if( event == assocCellCb->timer.tmrEvnt)
        {
           tmrRunning = TRUE;
        }
        timers  = &(assocCellCb->timer);  
     }
     break;
     case WR_TMR_RIM_TRIAE:
     {
        max     = 1;
        WrRimDstCellInfo *assocCellCb  = (WrRimDstCellInfo*)cb;
        if( event == assocCellCb->timer.tmrEvnt)
        {
           tmrRunning = TRUE;
        }
        timers  = &(assocCellCb->timer);
     }
     break; 
     case WR_TMR_MEM_CPU_UTIL_INFO:
     {
#ifdef LTE_L2_MEAS
        tmrRunning = TRUE;
        timers = &(smCb.memCpuMeasCb.cpuMemUtilTmr);
        max = 1;
#endif
     }
     break; 
     case WR_TMR_GUARD_UE_CTX_REL:
     {
        max     = 1;
        WrUmmCellCb *ummCellCb = (WrUmmCellCb*)cb;

        if( event == ummCellCb->ueCtxRelGuardTmr.tmrEvnt)
        {
           tmrRunning = TRUE;
        }
        timers  = &(ummCellCb->ueCtxRelGuardTmr);
     }
     break;
     case WR_TMR_ANR_CELL_MAX_AGE_TMR:
      {
         cellCb = (WrCellCb*)cb;
         max     =  1;
         if(cellCb->nrCb == NULLP)
         {
            tmrRunning = FALSE;
         }
         else
         {
            if(cellCb->nrCb->cellValidAgeTmr.tmrEvnt == event)
            {
               tmrRunning = TRUE;
            }
            timers  = &(cellCb->nrCb->cellValidAgeTmr);
         }
         break;
      }
      default:
         break;
   }
   if(tmrRunning == FALSE)
   {
      RETVOID;
   }

   /* initialize argument for common timer function */
   arg.tqCp    = &wrCb.tqCp;
   arg.tq      = wrCb.tq; 
   arg.timers  = timers;
   arg.cb      = cb;
   arg.evnt    = event;
   arg.wait    = 0;
   arg.max     = max;
   arg.tNum    = idx;
   cmRmvCbTq(&arg);

   RETVOID;
} /* end of wrStopTmr() */

PUBLIC Void wrStopUeTmr
(
WrUeCb                       *ueCb,
S16                          event
)
{
   CmTmrArg                  arg;
   U8                        idx;
   Bool                      tmrRunning;
   CmTimer                   *timers = NULLP;
   U8                        max = 0;
   WrUmmTimer                   *cb;


   idx = 0;

   tmrRunning = FALSE;
   switch(event)
   {
#ifdef LTE_HO_SUPPORT
      case WR_TMR_TGTENB_RELOCEXE:
      {
         max     =  1;
         if(ueCb->s1TgtEnbRelocTmr.tmr.tmrEvnt == event)
         {
            tmrRunning = TRUE;
         }
         timers = &ueCb->s1TgtEnbRelocTmr.tmr;
         cb = &ueCb->s1TgtEnbRelocTmr;
         break;
      }
      case WR_TMR_S1_OVRL_TMR:
      {
         max     =  1;
         if(ueCb->s1OvrallTmr.tmr.tmrEvnt == event)
         {
            tmrRunning = TRUE;
         }
         timers = &ueCb->s1OvrallTmr.tmr;
         cb = &ueCb->s1OvrallTmr;
         break;
      }
      case WR_TMR_UE_HO_ATTACH_TMR:
      {
         max     =  1;
         if(ueCb->ueHoAttachTmr.tmr.tmrEvnt == event)
         {
            tmrRunning = TRUE;
         }
         timers = &ueCb->ueHoAttachTmr.tmr;
         cb = &ueCb->ueHoAttachTmr;
         break;
      }
#endif
      case WR_TMR_RAD_RES_MOD_TMR:
      {
         max  = 1;
         if(ueCb->radResModTmr.tmr.tmrEvnt == event)
         {
            tmrRunning = TRUE;
         }
         timers = &ueCb->radResModTmr.tmr;
         cb = &ueCb->radResModTmr;
         break;
      }
      case WR_TMR_INITIAL_CTX_REQ:
      {
         max  = 1;
         if(ueCb->intialCntRspTmr.tmr.tmrEvnt == event)
         {
            tmrRunning = TRUE;
         }
         timers = &ueCb->intialCntRspTmr.tmr;
         cb = &ueCb->intialCntRspTmr;
      }
      break;
      case WR_TMR_UE_CTX_REL_REQ:
      {
         max  = 1;
         if(ueCb->ueCtxRelTmr.tmr.tmrEvnt == event)
         {
            tmrRunning = TRUE;
         }
         timers = &ueCb->ueCtxRelTmr.tmr;
         cb = &ueCb->ueCtxRelTmr;
      }
      break;
#ifdef WR_DETECT_RLF
      case WR_TMR_DETECT_RLF:
      {
         max  = 1;
         if(ueCb->cqiRprtTmr.tmr.tmrEvnt == event)
         {
            tmrRunning = TRUE;
         }
         timers = &ueCb->cqiRprtTmr.tmr;
         cb = &ueCb->cqiRprtTmr;
      }
     break;
#endif

     default:
         break;
   }
   if(tmrRunning == FALSE)
   {
      RETVOID;
   }

   /* initialize argument for common timer function */
   arg.tqCp    = &wrCb.tqCp;
   arg.tq      = wrCb.tq; 
   arg.timers  = timers;
   arg.cb      = (PTR)cb;
   arg.evnt    = event;
   arg.wait    = 0;
   arg.max     = max;
   arg.tNum    = idx;
   cmRmvCbTq(&arg);

   cmLListDelFrm(&(ueCb->tmrLst), &(cb->tmrLnk));

   RETVOID;
} /* end of wrStopUeTmr() */

PUBLIC Void wrStopTransTmr
(
WrUmmTransCb                *transCb,
S16                          event
)
{
   CmTmrArg                  arg;
   U8                        idx;
   Bool                      tmrRunning;
   CmTimer                   *timers = NULLP;
   U8                        max = 0;
   WrUmmTimer                   *cb;


   idx = 0;

   tmrRunning = FALSE;
   switch(event)
   {
      case WR_TMR_RRC_RRCCON:
      case WR_TMR_RRC_CONREL:
      case WR_TMR_RRC_CAPENQ:
      case WR_TMR_RRC_SECUCMD:
      case WR_TMR_REESTABLISH:
      case WR_TMR_RECONFIGURE:
      case WR_TMR_RRCCON_FAIL_RRC_UNDO_CFG:
      case WR_TMR_TGT_HOPREP:
         /* eCSFB - Added as part of ecsfb phase */
      case WR_TMR_RRC_DAT_CFM:
      case WR_TMR_TGT_PATH_SWITCH:
      /* Fix for the ccpu00126445.Handled the following timer stopping*/
      case WR_TMR_RRC_REL_GUARD:
      case WR_TMR_UL_HO_PREP_WAIT_TMR:
      case WR_TMR_DL_CDMA_RECV_HO_WAIT_TMR:
      case WR_TMR_ECSFB_HO_EXEC_TMR:
      case WR_TMR_ANR_STRONGCELL_REPORT_R1:
      {
         max     =  1;
         if(transCb->timer.tmr.tmrEvnt == event)
         {
            tmrRunning = TRUE;
         }
         timers = &transCb->timer.tmr;
         cb = &transCb->timer;
         break;
      }
      case WR_TMR_CSFB_CFG:
      {
         max     =  1;
         if(transCb->u.csfbTrans.csfbCfgTmr.tmr.tmrEvnt == event)
         {
            tmrRunning = TRUE;
         }
         timers  = &(transCb->u.csfbTrans.csfbCfgTmr.tmr);
         cb = &(transCb->u.csfbTrans.csfbCfgTmr);
         break;
      }
     case WR_TMR_ECSFB_MEAS_RPT:
      {
         max  =  1;
         if(transCb->u.ecsfbTransCb.measRptTmr.tmr.tmrEvnt == event)
         {
            tmrRunning = TRUE;
         }
         timers  = &(transCb->u.ecsfbTransCb.measRptTmr.tmr);
         cb = &(transCb->u.ecsfbTransCb.measRptTmr);
         break;
      }
	  case WR_TMR_CSFB_GSM_T304_EXP_WAIT:
      {
         max  =  1;
         if(transCb->u.csfbGeranTrans.t304WaitTmr.tmr.tmrEvnt == event)
         {
            tmrRunning = TRUE;
         }
         timers  = &(transCb->u.csfbGeranTrans.t304WaitTmr.tmr);
         cb = &(transCb->u.csfbGeranTrans.t304WaitTmr);
         break;
      }
	  case WR_TMR_CSFB_GSM_MEAS_WAIT:
      {
         max  =  1;
         if(transCb->u.csfbGeranTrans.measWaitTmr.tmr.tmrEvnt == event)
         {
            tmrRunning = TRUE;
         }
         timers  = &(transCb->u.csfbGeranTrans.measWaitTmr.tmr);
         cb = &(transCb->u.csfbGeranTrans.measWaitTmr);
         break;
      }
     case WR_TMR_SRC_HO_CANCEL:
     {
        max     =  1;
         if(transCb->u.s1SrcHoTrans.s1CancelTmr.tmr.tmrEvnt == event)
         {
            tmrRunning = TRUE;
         }
         timers  = &(transCb->u.s1SrcHoTrans.s1CancelTmr.tmr);
         cb = &(transCb->u.s1SrcHoTrans.s1CancelTmr);
         break;
      }
      case WR_TMR_S1_RELOC_TMR:
      {
         max     =  1;
         if(transCb->u.s1SrcHoTrans.s1RelocTmr.tmr.tmrEvnt == event)
         {
            tmrRunning = TRUE;
         }
         timers  = &(transCb->u.s1SrcHoTrans.s1RelocTmr.tmr);
         cb = &(transCb->u.s1SrcHoTrans.s1RelocTmr);
         break;
      }
      default:
         break;
   }
   if(tmrRunning == FALSE)
   {
      RETVOID;
   }

   /* initialize argument for common timer function */
   arg.tqCp    = &wrCb.tqCp;
   arg.tq      = wrCb.tq; 
   arg.timers  = timers;
   arg.cb      = (PTR)cb;
   arg.evnt    = event;
   arg.wait    = 0;
   arg.max     = max;
   arg.tNum    = idx;
   cmRmvCbTq(&arg);
   cmLListDelFrm(&(transCb->tmrLst), &(cb->tmrLnk));

   RETVOID;
} /* end of wrStopTransTmr() */

/** @brief This function handles timer expiry. 
 *
 * @details
 *
 *     Function: wrProcTmr
 *
 *         Processing steps:
 *               This function based upon the timer event invokes specific 
 *               routine to handle timer expiry. 
 *
 * @param[in] Cb : This holds approriate Control block for the timer event passed. 
 * @param[in] tmrEvent : One of the many possible timer types. 
 * @return S16
 *        -# Success : ROK
 *        -# Failure : RFAILED
 */
PRIVATE Void wrProcTmr
(
Ptr                          cb,
S16                          event
)
{
   WrLiSapCb                 *sapCb;
  /*U32 size;*/
   switch(event)
   {
      case WR_TMR_NHU_SAP_BND:
      {
         sapCb           = (WrLiSapCb *)cb;
         sapCb->sapState = LWR_SAP_UNBOUND; 
         RLOG0(L_FATAL, "[ALARM] Sending NHU_SAP_BND_FAIL to OAM");
         wrSendLmAlarm(LCM_CATEGORY_INTERFACE, LCM_EVENT_BND_FAIL, 
                                            LWR_CAUSE_NHU_SAP_BOUND);
         break;
      }
      case WR_TMR_RGR_SAP_BND:
      {
         sapCb           = (WrLiSapCb *)cb;
         sapCb->sapState = LWR_SAP_UNBOUND; 
         RLOG0(L_FATAL, "[ALARM] Sending RGR_SAP_BND_FAIL to OAM");
         wrSendLmAlarm(LCM_CATEGORY_INTERFACE, LCM_EVENT_BND_FAIL, 
                                            LWR_CAUSE_RGR_SAP_BOUND);
         break;
      }
      case WR_TMR_CTF_SAP_BND:
      {
         sapCb           = (WrLiSapCb *)cb;
         sapCb->sapState = LWR_SAP_UNBOUND; 
         RLOG0(L_FATAL, "[ALARM] Sending CTF_SAP_BND_FAIL to OAM");
         wrSendLmAlarm(LCM_CATEGORY_INTERFACE, LCM_EVENT_BND_FAIL,
                                            LWR_CAUSE_CTF_SAP_BOUND);
         break;
      }
      case WR_TMR_SZT_SAP_BND:
      {
         sapCb           = (WrLiSapCb *)cb;
         sapCb->sapState = LWR_SAP_UNBOUND; 
         RLOG0(L_FATAL, "[ALARM] Sending SZT_SAP_BND_FAIL to OAM");
         wrSendLmAlarm(LCM_CATEGORY_INTERFACE, LCM_EVENT_BND_FAIL, 
                                            LWR_CAUSE_SZT_SAP_BOUND);
         break;
      }
#ifdef RM_INTF
      case WR_TMR_RMU_SAP_BND: /*RRM->FSM sck changes*/
      {
         sapCb           = (WrLiSapCb *)cb;
         sapCb->sapState = LWR_SAP_UNBOUND;
         RLOG0(L_FATAL, "[ALARM] Sending RMU_SAP_BND_FAIL to OAM");
         wrSendLmAlarm(LCM_CATEGORY_INTERFACE, LCM_EVENT_BND_FAIL,
                                            LWR_CAUSE_RMU_SAP_BOUND);
         break;
      }
#endif /*RM_INTF*/
      case WR_TMR_NLU_SAP_BND :
      {
         sapCb          = (WrLiSapCb *)cb;
	 sapCb->sapState = LWR_SAP_UNBOUND;
	 wrSendLmAlarm(LCM_CATEGORY_INTERFACE, LCM_EVENT_BND_FAIL,
	                                    LWR_CAUSE_NLU_SAP_BOUND);
	 break;
      }
      case WR_TMR_MME_SETUP_RSP:
      {
         wrEmmMmeSetupWaitTmrExpiry((PTR)cb);
         break;
      }
      case WR_TMR_MME_SETUP_WAIT:
      {
         wrEmmMmeSetupWaitTmrExpiry((PTR)cb);
         break;
      }
      case WR_TMR_ENB_SETUP_WAIT:
      {
         wrEmmNghEnbSetupWaitTmrExpiry((PTR)cb);
         break;
      }
      case WR_TMR_X2_RST_RSP:
      {
         wrEmmNghEnbMsgTmrExpiry((PTR)cb);
         break;
      }
      case WR_TMR_ENB_SETUP_RSP:
      {
         wrEmmNghEnbSetupTmrExpiry((PTR)cb);
         break;
      }
      case WR_TMR_ENB_CONFIG_UPDATE_WAIT:
      {
         wrEmmNghEnbCfgUptTmrExpiry((WrNeighEnbCb *)cb);
         break;
      }
      case WR_TMR_ENB_CFG_TRANSFER_WAIT:
      {
         wrEmmMmeEnbCfgTransferRspTmrExpiry((WrNeighEnbCb *)cb);
         break;
      }

      case WR_TMR_RRC_REL_GUARD:
      case WR_TMR_RRC_RRCCON:
      case WR_TMR_RRC_CONREL:
      case WR_TMR_RRC_CAPENQ:
      case WR_TMR_RRC_SECUCMD:
      case WR_TMR_REESTABLISH:
      case WR_TMR_RECONFIGURE:
      case WR_TMR_RAD_RES_MOD_TMR:
      case WR_TMR_INITIAL_CTX_REQ:         
      case WR_TMR_RRCCON_FAIL_RRC_UNDO_CFG:
      case WR_TMR_UE_CTX_REL_REQ:
      case WR_TMR_TGT_HOPREP:
         /* eCSFB - Added as part of ecsfb phase */
      case WR_TMR_RRC_DAT_CFM:
      case WR_TMR_TGT_PATH_SWITCH:
#ifdef WR_DETECT_RLF
      case WR_TMR_DETECT_RLF:
#endif
      /* ccpu00126817 */
      case WR_TMR_S1_RELOC_TMR:
      case WR_TMR_UL_HO_PREP_WAIT_TMR:
      case WR_TMR_DL_CDMA_RECV_HO_WAIT_TMR:
      case WR_TMR_ECSFB_HO_EXEC_TMR:
      case WR_TMR_SRC_HO_CANCEL:
      case WR_TMR_ANR_STRONGCELL_REPORT_R1:
      {
        //ALARM(" RRC(%d) Timer has EXPIRED \n", event ); 
        wrUmmPrcTimerExp((WrUmmTimer *)cb,event);
        break;
      }
      case WR_TMR_ENB_CFG_RSP:
      {
         wrEmmMmeEnbCfgRspTmrExp((U32)cb);
         break;
      }
      case WR_TMR_RST_RSP:
      {
         wrEmmMmeResetRspTmrExp((U32)cb);
         break;
      }
      case WR_TMR_S1_OVRL_TMR:
      {
         wrUmmS1SrcHoOvrallTmrExp((WrUeCb *)((WrUmmTimer *)cb)->cb);
         break;
      }
      case WR_TMR_TGTENB_RELOCEXE:
      {
        /* sri - HOFIX */
         wrUmmPrcTgtEnbRelocTmrExpiry((WrUeCb *)((WrUmmTimer *)cb)->cb);
         break;
      }
      case WR_TMR_ENB_CFG_WAIT:
      {
         wrEmmMmeEnbCfgUpdWaitTmrExp((U32)cb);
         break;
      }
#ifdef ENB_OVRLOAD
      case WR_TMR_OVERLD_SAMPLE:
        /*-process the timer expiry-*/
        wrEmmSampleData();
        /*---If Feature is enabled, Start Sampling and Status Updating Timer-*/
        if(wrEmmCb.eNBOvldParams.eNbOvrLdFlag)
            wrStartTmr((PTR)&wrEmmCb,WR_TMR_OVERLD_SAMPLE,wrEmmCb.eNBOvldParams.eNBOvldMonTimerVal);
        break;
      case WR_TMR_OVERLD_STATUS:
        /*-process the timer expiry-*/
        wrEmmUpdOvrLdStat();
        /*-Restart the timer!!-*/
        if(wrEmmCb.eNBOvldParams.eNbOvrLdFlag)
           wrStartTmr((PTR)&wrEmmCb,WR_TMR_OVERLD_STATUS,
                 wrEmmCb.eNBOvldParams.eNBOvldMonTimerVal * wrEmmCb.eNBOvldParams.eNBOvldMonfreq);
        break;
#endif
      case WR_TMR_CSFB_CFG:
      case WR_TMR_ECSFB_MEAS_RPT:
      {
         wrUmmPrcTimerExp((WrUmmTimer *)cb,event);
      }
      break;
      case WR_TMR_CSFB_GSM_T304_EXP_WAIT:
      case WR_TMR_CSFB_GSM_MEAS_WAIT:
      {
         wrUmmPrcTimerExp((WrUmmTimer *)cb,event);
      }
      break;
      case WR_TMR_ANR_MEAS_EPOCH_TMR:
      {
        RLOG0(L_INFO, 
              "****** EPOCH TIMER EXPIRED *******\n");
         wrEmmPrcEpochTimerExp(cb,event);
      }
      break;
      case WR_TMR_ANR_MEAS_TRICE_TMR:
      {
         RLOG0(L_INFO, 
             "****** TRICE TIMER EXPIRED *******\n");
         wrEmmPrcTriceTimerExp(cb,event);
      }
      break;
      case WR_TMR_ANR_CELL_MAX_AGE_TMR:
      {
         RLOG0(L_INFO, 
             "****** CELL VALID AGE TIMER EXPIRED *******\n");
         wrEmmPrcCellMaxAgeTmrExp(cb);
      }
      break;
      /* PH04_CMAS_TMR */
      case WR_TMR_REPETITION_PERIOD:
      {
         wrEmmPwsProcRepTimerExp(cb,event);
      }
      break;
      /* PH04_CMAS_TMR : end */
#ifdef WR_RSYS_KPI 
     /*Added for KPI*/
      case WR_TMR_KPI_PREDFND_INTRVL_TMR:
      {
         wrIncKpiPredfndIntrvlTmrExpiry();
      }
       break;
#endif/*WR_RSYS_KPI*/
/* LTE_ADV_ABS starts */
      case WR_TMR_ABS_LOAD_INFO_TMR:
         {
            wrEmmLteAdvPrcTmrExpiry((PTR)cb, event);   
         }
         break;
         /* LTE_ADV_ABS ends */
      case WR_TMR_RIM_OVERALL:
         {
            /* Sending the RIR with Multiple request to all the 
             * UTRA & GERAN Cells*/
            RLOG0(L_DEBUG,"RIM Overall Timer Expired.");
            wrEmmTrigRirProc((WrCellCb *)cb,WR_RIM_RIR_MULTI);
            if(FALSE == wrIsTmrRunning(&(wrEmmCb.rimOvrlTmr),WR_TMR_RIM_OVERALL))
            {
               wrStartTmr((PTR)cb, WR_TMR_RIM_OVERALL, wrEmmCb.tmrCfg.rimOvrlTmr);
               RLOG0(L_DEBUG,"RIM Overall Timer restarted");
            }
         }
         break;
      case WR_TMR_RIM_TRIR: 
         {
            wrEmmRimPrcTmrExp((PTR)cb,event);
         }
         break;
      case WR_TMR_RIM_TRIAE:
         {
            wrEmmRimPrcTmrExp((PTR)cb,event);
         }
         break;
      case WR_TMR_PCI_MOD_TMR :
         {  
            wrEmmPciModTmrExp((PTR)cb);
         }
         break; 
     case WR_TMR_MEM_CPU_UTIL_INFO:
     {
#ifdef LTE_L2_MEAS
           wrMemCpuUtilTmrExp();
#endif
     }
     break; 
     case WR_TMR_GUARD_UE_CTX_REL:
     {
        wrUmmUeCtxRelGrdTmrExp((WrUmmCellCb*)cb);
     }
     break;
     default:
         {
            /* Invalid Timer */
            RLOG1(L_ERROR, "Invalid Timer event [%d] received", event);
            break;
         }
   }
   RETVOID;
} /* end of wrProcTmr() */

/** @brief This function Finds whether Timer is running or not
 *
 * @details
 *
 *     Function: wrIsTmrRunning
 *
 *         Processing steps:
 *               Check Whetether Timer is running or not based on Event type
 *
 * @param[in] tmr : pointre to the Timer. 
 * @param[in] event : One of the many possible timer types. 
 * @return S16
 *        -# Success : ROK
 *        -# Failure : RFAILED
 */
PUBLIC Bool wrIsTmrRunning
(
CmTimer                      *tmr,
S16                          event
)
{
   if(tmr != NULLP)
   {   
      if(tmr->tmrEvnt == event)
      {
         RETVALUE(TRUE);
      }
      else if (tmr->tmrEvnt != TMR_NONE)
      {
         RLOG1(L_ERROR, "Invalid Timer event [%d] received", event);
      }
   }
   RETVALUE(FALSE);
}
/********************************************************************30**

           End of file:    $SID$

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
$SID$        ---      Sriky         1. initial release TotaleNodeB 1.1
*********************************************************************91*/
