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

     Name:    LTE eNodeB - DAM file

     Type:    C Source file

     Desc:    This file contains the enodeB application timer code

     File:    wr_dam_tmr.c

     Sid:      fc_dam_tmr.c@@/main/TeNB_Main_BR/6 - Mon Aug 11 16:51:34 2014

     Prg:     aj
*********************************************************************21*/

static const char* RLOG_MODULE_NAME="DAM";
static int RLOG_MODULE_ID=16;
static int RLOG_FILE_ID=108;
/* Header include files (.h) */
#include "ssi.h"
#include "wr.h" 
#include "wr_umm.h"
#include "wr_dam.h"
#include "wr_umm_msg.h"
/* Start - Fix for ccpu00123981 */
#include "wr_smm_init.h"
/* End - Fix for ccpu00123981 */
#include "wr_dam_ifm_app.h"
#include "wr_ifm_rrm.h"

PRIVATE Void wrDamProcTmr (Ptr cb, S16 tmrEvnt);

/** @brief This function registers and initializes the timer
 *         queue structures used by DAM
 *
 * @details
 *
 *     Function: wrDamRegInitTmr
 *
 *         Processing steps:
 *
 * @param[in] Void 
 * @return S16
 *        -# Success : ROK
 */
PUBLIC S16 wrDamRegInitTmr
(
Void
)
{
   U32 idx;
   S16 ret;
   ret = SRegTmrMt((Ent)ENTEU,(Inst)wrDamCb.init.inst,wrDamCb.timeRes,wrDamActvTmr);
   if (ret != ROK)
   {
      RLOG0(L_ERROR, "SRegTmr Failed");
      RETVALUE(RFAILED);
   }

   /* initialize timing queues */
   wrDamCb.tqCp.tmrLen      = WR_NUM_TQENTRY;

   for (idx = 0; idx < WR_NUM_TQENTRY; idx++)
   {
      wrDamCb.tq[idx].first = NULLP;
      wrDamCb.tq[idx].tail  = NULLP;
   }

   RETVALUE(ROK);
} /* wrDamRegInitTmr */


/** @brief This function de-registers the DAM timers
 *
 * @details
 *
 *     Function: wrDamDeRegTmr
 *
 *         Processing steps:
 *
 * @param[in] Void 
 * @return S16
 *        -# Success : ROK
 */
PUBLIC S16 wrDamDeRegTmr 
(
Void
)
{
   RETVALUE(SDeregTmrMt((Ent)ENTEU,(Inst)wrDamCb.init.inst, wrDamCb.timeRes, wrDamActvTmr));
}/* wrDamDeRegTmr */


/** @brief Invoked by system services to activate a task with
 *              a timer tick.
 *
 * @details
 *
 *     Function: wrDamActvTmr
 *
 *         Processing steps:
 *
 * @param[in] Void 
 * @return S16
 *        -# Success : ROK
 */

PUBLIC S16 wrDamActvTmr
(
Ent  ent,
Inst inst
)
{
#ifndef TENB_ACC
   wrPlatNonrtTickHdl(); 
#endif
   cmPrcTmr(&wrDamCb.tqCp,wrDamCb.tq,(PFV)wrDamProcTmr);
   RETVALUE(ROK);
} /* end of wrDamActvTmr */


/** @brief This function is used to start a given timer. 
 *
 * @details
 *
 *     Function: wrDamStartTmr
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
PUBLIC S16 wrDamStartTmr
(
PTR cb,
S16 tmrEvnt,
U32 delay
)
{
   U32         wait;
   CmTmrArg    arg;
   CmTimer     *tmr;
   U8          maxTmrs;

   wait = WR_DAM_CALC_WAIT_TIME(delay);
   maxTmrs  = 1;

   switch (tmrEvnt)
   {
          /* For SZT Bind Timer Timer */
      case WR_TMR_EGT_SAP_BND:
      case WR_TMR_PJU_SAP_BND:
      {
         WrLiSapCb   *sapCb;
         sapCb = ( WrLiSapCb*)cb;
         tmr      = &sapCb->timer;
         break;
      }
   case WR_DAM_TMR_INACTIVITY:
      {
         WrDamUeCb   *damUeCb;
         damUeCb = (WrDamUeCb *)cb;
         tmr      = &damUeCb->inactivityTmr;
      }
      break;
      case WR_DAM_TMR_END_MARKER_TMR:
      {
         WrDamUeCb   *damUeCb;
         damUeCb = (WrDamUeCb *)cb;
         tmr      = &damUeCb->endMrkTmr;
      }
      break;
      default:
      {
         RLOG0(L_ERROR, "Invalid tmrEvnt");
         RETVALUE(RFAILED);
      }
   } /* end of switch */

   /* if the timer is already running return failure */
   if(tmr->tmrEvnt != TMR_NONE)
   {
      RETVALUE(RFAILED);
   }         
   /* initialize argument for common timer function */
   if(wait != 0)
   {
      arg.tqCp   = &(wrDamCb.tqCp);
      arg.tq     = wrDamCb.tq;
      arg.cb     = cb;
      arg.timers = tmr;
      arg.evnt   = tmrEvnt;
      arg.wait   = wait;
      arg.tNum   = 0;
      arg.max    = maxTmrs;

      cmPlcCbTq(&arg);
   }

   RETVALUE(ROK);
} /* end of wrDamStartTmr() */


/** @brief This function is used to stop a previously running timer. 
 *
 * @details
 *
 *     Function: wrDamStopTmr
 *
 *         Processing steps:
 *               This function based upon the timer event reterives relevant timerCb and 
 *               tries to determine whether timer is running or not.In case timer 
 *               was running, it is stopped .
 *
 * @param[in] Cb : This holds approriate Control block for the timer event passed. 
 * @param[in] tmrEvent : One of the many possible timer types. 
 * @return Void
 */
PUBLIC Void wrDamStopTmr
(
PTR cb,
S16 event
)
{
   CmTmrArg    arg;
   U8          idx;
   CmTimer     *timers = NULLP;
   U8          max = 0;

   idx = 0;
   max = 1;
   switch(event)
   {
      case WR_TMR_EGT_SAP_BND:
      case WR_TMR_PJU_SAP_BND:
      {
         WrLiSapCb   *sapCb;
         sapCb = (WrLiSapCb*)cb;
         timers = &sapCb->timer;
         break;
      }
      case WR_DAM_TMR_INACTIVITY:
      {
         WrDamUeCb   *damUeCb;
         damUeCb = (WrDamUeCb *)cb;
         timers = &damUeCb->inactivityTmr;
         break;
      }/* End of WR_DAM_TMR_INACTIVITY */

      case WR_DAM_TMR_END_MARKER_TMR:
      {
         WrDamUeCb   *damUeCb;
         damUeCb = (WrDamUeCb *)cb;
         timers = &damUeCb->endMrkTmr;
         break;
      }/* End of WR_DAM_TMR_END_MARKER_TMR */
      default:
         break;
   }
 
   if(NULLP == timers)
   {
      RLOG1(L_ERROR, "wrDamStopTmr: event=[%d] does not have any timers", event);
      RETVOID;
   }
   /* if the timer is already not running return failure */
   if(timers->tmrEvnt == TMR_NONE)
   {
      RETVOID;
   } 

   /* initialize argument for common timer function */
   arg.tqCp    = &wrDamCb.tqCp;
   arg.tq      = wrDamCb.tq; 
   arg.timers  = timers;
   arg.cb      = cb;
   arg.evnt    = event;
   arg.wait    = 0;
   arg.max     = max;
   arg.tNum    = idx;
   cmRmvCbTq(&arg);

   RETVOID;
} /* end of wrDamStopTmr() */



/** @brief This function handles timer expiry. 
 *
 * @details
 *
 *     Function: wrDamProcTmr
 *
 *         Processing steps:
 *               This function based upon the timer event invokes specific 
 *               routine to handle timer expiry. 
 *
 * @param[in] Cb : This holds approriate Control block for the timer event passed. 
 * @param[in] tmrEvent : One of the many possible timer types. 
 * @return Void
 */
PRIVATE Void wrDamProcTmr
(
Ptr cb,
S16 event
)
{
   WrDamUeCb *damUeCb;
   WrLiSapCb     *sapCb;
   /*RRM_SP3*/
   U32   inactEvt;

   RLOG1(L_DEBUG, "Timer [%d] timedout",event);

   switch(event)
   {
      case WR_TMR_EGT_SAP_BND:
      {
         sapCb           = (WrLiSapCb *)cb;
         sapCb->sapState = LWR_SAP_UNBOUND; 
         RLOG0(L_FATAL, "[ALARM] Sending EGT_SAP_BND_FAIL to OAM");
         wrIfmDamWrSendLmAlarm(LCM_CATEGORY_INTERFACE, LCM_EVENT_BND_FAIL, 
                                            LWR_CAUSE_EGT_SAP_BOUND);
         break;
      }
      case WR_TMR_PJU_SAP_BND:
      {
         sapCb           = (WrLiSapCb *)cb;
         sapCb->sapState = LWR_SAP_UNBOUND; 
         RLOG0(L_FATAL, "[ALARM] Sending PJU_SAP_BND_FAIL to OAM");
         wrIfmDamWrSendLmAlarm(LCM_CATEGORY_INTERFACE, LCM_EVENT_BND_FAIL, 
                                            LWR_CAUSE_PJU_SAP_BOUND);
         break;
      }
   case WR_DAM_TMR_INACTIVITY:
      {
         damUeCb = (WrDamUeCb*)cb;
         if(damUeCb->dataRcvd == FALSE)
         {
/* Start - Fix for ccpu00123981 */
            inactEvt = RMU_UE_NO_DATA; 
            damUeCb->expiryCnt +=1;
            
            if(damUeCb->expiryCnt >= wrDamCb.expiryCnt)
            {
               RLOG0(L_DEBUG, "****** UE INACTIVITY OCCURED*******");
               /*
               wrUmmSndCtxtRelReq(damUeCb->ueId, 
                                  damUeCb->cellId, 
                                  WR_UMM_CTXT_REL_INACTIVITY);
               */
               wrIfmDamWrCtxtRel(damUeCb->ueId,
                                  damUeCb->cellId,
                                  WR_UMM_CTXT_REL_INACTIVITY);
            }
            else
            {
               RLOG_ARG2(L_DEBUG, DBG_UEID,damUeCb->ueId,"Inactivity Timer "
                  "Expired: Current Expiry Count=%d MAX_EXPIRY_COUNT=%d",
                  damUeCb->expiryCnt, wrDamCb.expiryCnt);
               /*wrDamStopTmr((PTR)damUeCb, WR_DAM_TMR_INACTIVITY); */
               wrDamStartTmr((PTR)damUeCb, WR_DAM_TMR_INACTIVITY, wrDamCb.inactvTmrVal);
/* End - Fix for ccpu00123981 */
            }
         }
         else
         {
            damUeCb->dataRcvd = FALSE;
            inactEvt = RMU_UE_DATA;/*RRM_SP3*/
            damUeCb->expiryCnt =0;
            wrDamStartTmr((PTR)damUeCb, WR_DAM_TMR_INACTIVITY, wrDamCb.inactvTmrVal);
         }
         /*RRM_SP3: Send to RRM an UE inactivity indication containing crnti*/
         wrIfmDamRmuUeInactInd(damUeCb->ueId,damUeCb->cellId,inactEvt);
         break;
      }
      case WR_DAM_TMR_END_MARKER_TMR:
      {
         damUeCb = (WrDamUeCb*)cb;
         wrDamHdlTmrExp(event, damUeCb);
         break;
      }
      default:
      {
         /* Invalid Timer */
         RLOG1(L_ERROR, "Invalid timer [%d]", event);
         break;
      }
   }/*End of timer event switch*/
   RETVOID;
} /* end of wrDamProcTmr() */

/********************************************************************30**

         End of file:     fc_dam_tmr.c@@/main/TeNB_Main_BR/6 - Mon Aug 11 16:51:34 2014

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
*********************************************************************91*/
