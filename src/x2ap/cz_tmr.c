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



/**********************************************************************

     Name:     LTE-X2AP Layer 
  
     Type:     C Source file
  
     Desc:     This file handles the timer modules.

     File:     cz_tmr.c

     Sid:      tr_tmr.c@@/main/2 - Tue Aug 30 18:36:15 2011

     Prg:       

**********************************************************************/
#include "envopt.h"        /* Environment options */
#include "envdep.h"        /* Environment dependent */
#include "envind.h"        /* Environment independent */

#include "gen.h"           /* General */
#include "ssi.h"           /* System services */
#include "cm_llist.h"      /* Common Link List defines */
#include "cm_hash.h"       /* Common Hashlist defines */
#include "cm_mblk.h"       /* common memory */
#include "cm5.h"           /* common structs 3 */
#include "cm_inet.h"       /* common tokens  */
#include "cm_tkns.h"       /* Common Token defines */
#include "cm_tpt.h"        /* common transport */
#include "cm_pasn.h"       /* Common Asn.1 library           */
#include "sct.h"           /* SCTP Layer */
#include "lcz.h"           /* LCZ Interface defines */
#include "czt.h"           /* CZT Interface defines */
#include "cz.h"            /* X2AP defines */

/* Header/extern include files (.x) */
#include "gen.x"           /* General */
#include "ssi.x"           /* System services */
#include "cm_lib.x"        /* Common library definitions */
#include "cm_tkns.x"       /* Common Token definitions */
#include "cm_llist.x"      /* Common Link List definitions */
#include "cm_hash.x"       /* Common Hashlist definitions */
#include "cm_mblk.x"       /* common memory */
#include "cm5.x"           /* common structs 3 */
#include "cm_inet.x"       /* common transport */
#include "cm_tpt.x"        /* common transport */
#include "cm_pasn.x"       /* Common ASn.1 Library           */
#include "sct.x"           /* SCTP Layer */
#include "cm5.x"           /* Common timer definitions */
#include "sct.x"           /* SCTP Layer */
#include "lcz.x"           /* LCZ Interface includes */
#include "czt.x"           /* CZT Interface includes */
#include "cz.x"            /* X2AP includes */


/** @defgroup tmrmodule X2AP Timer Module
 *  @{
 */
/* local defines */

/* local typedefs */
 
/* local externs */
 
/* forward references */


/** @brief Registers the X2AP timers with SSI
 *  @details Register and initialize timer queue structures in 
 *    X2AP during general config.
 *
 *  @return
 *    - ROK       - If the function is successful.
 *    - RFAILED   - If the function is unsuccessful due to any failure.
 *
 *
 */
#ifdef ANSI
PUBLIC S16 czRegInitTmr
(
Void
)
#else
PUBLIC S16 czRegInitTmr()
#endif
{
   U16 i;
   S16 ret;

   CZ_TRC2(czRegInitTmr);

   CZ_DBG_INFO((CZ_PRNT_BUF, "czRegInitTmr: tmrRes(%d) \n",
                                czCb.genCfg.timeRes));

   /* register general timer here */
#ifdef SS_MULTIPLE_PROCS
   ret = SRegTmr(czCb.init.proc,czCb.init.ent, czCb.init.inst,
         czCb.genCfg.timeRes, (PAIFTMRS16) czActvTmr);
#else
   ret = SRegTmr(czCb.init.ent, czCb.init.inst, czCb.genCfg.timeRes, czActvTmr);
#endif

#if (ERRCLASS & ERRCLS_DEBUG)
   if (ret != ROK)
   {
      CZ_DBG_FATAL((CZ_PRNT_BUF, "czRegInitTmr:  czActvTmr failed"));
      CZ_RETVALUE(RFAILED);
   }
#endif

   /* Timing queue control point */
   czCb.czGenTqCp.nxtEnt = 0;
   czCb.czGenTqCp.tmrLen = CZ_TQSIZE;

   /* Initialise the timing queue */
   for (i = 0; i < CZ_TQSIZE; i++)
   {
     czCb.czGenTq[i].first = NULLP;
     czCb.czGenTq[i].tail = NULLP;
   }

   /* Initialize timing queues for Lower Layer Cfg */
   czCb.czLyrTqCp.nxtEnt = 0;
   czCb.czLyrTqCp.tmrLen = CZ_TQSIZE;

   for (i = 0; i < CZ_TQSIZE; i++)
   {
      czCb.czLyrTq[i].first = NULLP;
      czCb.czLyrTq[i].tail = NULLP;
   }

   CZ_RETVALUE(ROK);

} /* czRegInitTmr */

/**
 * @brief The function is used Activate the X2AP Timers.
 * @details This function is invoked by system services to activate 
            a task with a timer tick.
 *
 * @param [in] proc The proc ID.
 * @param [in] ent The entity ID of the timer task.
 * @param [in] inst The instance ID of the task.
 * @return 
 *  - ROK     - If the function is successful
 *  - RFAILED - If the function is unsuccessful due to any failure.
*/

#ifdef SS_MULTIPLE_PROCS
#ifdef ANSI
PUBLIC S16 czActvTmr
(
ProcId proc,
Ent    ent,
Inst   inst
)
#else

PUBLIC S16 czActvTmr(proc, ent, inst)
ProcId proc;
Ent    ent;
Inst   inst;
#endif
#else /* SS_MULTIPLE_PROCS */
#ifdef ANSI
PUBLIC S16 czActvTmr
(
 void
)
#else
PUBLIC S16 czActvTmr()
#endif
#endif /* SS_MULTIPLE_PROCS */
{
#ifdef SS_MULTIPLE_PROCS
      S16 ret;
#endif /*SS_MULTIPLE_PROCS */
   
   CZ_TRC3(czActvTmr);

#ifdef SS_MULTIPLE_PROCS
   ret = SGetXxCb(proc, ent, inst, (Void **) &czCbPtr);
   if ((ret !=ROK)||
         (czCbPtr == NULLP))
   {
      /* cz001.301:Klockwork fix-Accesing czCb if NULL */
      SLogError(ent, inst, proc,
            __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
            (ErrVal)ECZT147, (ErrVal)0, "failed, cannot derive czCb");
      RETVALUE(RFAILED);
   }
#endif /* SS_MULTIPLE_PROCS */

   /* Handle Generic layer Bind expiry */
   cmPrcTmr(&czCb.czGenTqCp, czCb.czGenTq, (PFV)czProcGenTmrExpiry);

   /* Handle Lower Layer Cfg expiry */
   cmPrcTmr(&czCb.czLyrTqCp, czCb.czLyrTq, 
            (PFV)czProcLyrTmrExpiry);

   CZ_RETVALUE(ROK);
} /* End of czActvTmr */

/** 
 * @brief The function handles the Lower Layer related Timer.
 * @details This function starts/stops timer for SCTP bind event. 

 * @param[in] tmrEvnt     Timer event for which timer needs to be started or stopped
 * @param[in] cb          SctlSAP cb 
 * @param[in] action      Timer Start or Stop

 * @return 
 *  - ROK     - If the function is successful
 *  - RFAILED - If the function is unsuccessful due to any failure.
*/
#ifdef ANSI
PUBLIC S16 czSchedGenTmr
(
S16       tmrEvnt,  
Void      *cb,       
U8        action    
)
#else
PUBLIC S16 czSchedGenTmr(tmrEvnt,cb, action)
S16       tmrEvnt;
Void      *cb;
U8        action;
#endif
{
   CzSctSapCb  *sctSapCb;
   U16         wait;
   CmTmrArg    arg;
   U8          tmrNum;
   S16         ret;

   CZ_TRC2(czSchedGenTmr)

   sctSapCb = NULLP;
   wait   = 0;
   tmrNum = 0;
   ret    = ROK;

   if ((action != CZ_TMR_START) &&
       (action != CZ_TMR_STOP))
   {
      CZ_DBG_ERROR((CZ_PRNT_BUF, "Invalid action passed in \
               czSchedGenTmr %d\n",action));
      CZ_RETVALUE(RFAILED);
   }
   
   sctSapCb = (CzSctSapCb *)cb;
   if(NULLP == sctSapCb)
   {
      CZ_DBG_ERROR((CZ_PRNT_BUF, "NULLP SCTP CB Passed\n "));
      CZ_RETVALUE(RFAILED);
   }

   switch(tmrEvnt)
   {
      case CZ_TMR_SCT_BIND:
      {
         if(action == CZ_TMR_START)
         {
            /* Get the wait value from LSap configuration */
            if (sctSapCb->tBndTmr.enb == TRUE)
               wait = sctSapCb->tBndTmr.val;
            if(wait == 0)
            {
               CZ_DBG_ERROR((CZ_PRNT_BUF, "Zero wait value configured\n "));
               CZ_RETVALUE(RFAILED);
            }
         }
         else 
         {
            for (tmrNum = 0; tmrNum < CZ_MAX_SCT_SAP_TMRS; tmrNum++)
            {
               if (sctSapCb->timers[tmrNum].tmrEvnt == tmrEvnt)
               {
                  break;
               }
            }
            if (tmrNum >= CZ_MAX_SCT_SAP_TMRS)
            {
               CZ_RETVALUE(LCM_REASON_EXCEED_CONF_VAL);
            }
         }
      } 
      break;
      default:
      {
         CZ_DBG_ERROR((CZ_PRNT_BUF, "Invalid tmier event passed in \
                  czSchedGenTmr %d\n",tmrEvnt));
         CZ_RETVALUE(RFAILED);
      }
   }

   arg.tq     = czCb.czGenTq;
   arg.tqCp   = &czCb.czGenTqCp;
   arg.timers = sctSapCb->timers;
   arg.cb     = (PTR)cb;
   arg.wait   = wait;
   arg.evnt   = tmrEvnt;
   arg.tNum   = tmrNum;
   arg.max    = CZ_MAX_SCT_SAP_TMRS;

   /* Start the timer only if timer value (wait) is not zero */
   if(action == CZ_TMR_START)
   {
      /* Start the Gen timer */
      cmPlcCbTq(&arg);
   }
   else
   { 
      /* Stop the Gen timer */
      cmRmvCbTq(&arg);
   }
 
   CZ_RETVALUE(ROK);
} /* End of czSchedGenTmr */

/** 
 * @brief  The function handles the Protocol related Timers
 * @details This function starts the reloaction preparation, relocation overall
 *          and TimeTowait timer event. 
 *          This is used by BMP and GPM module.
 *
 * @param[in] tmrType  Timer type whether RELOCPREP/RELOCOVERALL/TIMETOWAIT
 * @param[in] wait     Wait value for the timer  
 * @param[in] cb       Control Cb, either UeCb or PeerCb based on the timer
 *                     type.
 *
 * @return 
 *  - ROK     - If the function is successful
 *  - RFAILED - If the function is unsuccessful due to any failure.
*/
#ifdef ANSI
PUBLIC S16 czStartTmr
(
CzTmrType      tmrType,
U16            delay,
Void           *cb
)
#else
PUBLIC S16 czStartTmr(tmrType, delay, cb)
CzTmrType      tmrType;
U16            delay;
Void           *cb;
#endif
{
   CmTmrArg          arg;
   U8                tmrNum;
   CzPeerCb          *peerCb = NULLP;
   CzUeCb            *ueCb = NULLP;
   U16               wait = CZ_CALC_DELAY_TIME(delay);

   CZ_TRC2(czStartTmr)

   tmrNum = 0;

   switch (tmrType)
   {
      case CZ_TMR_TIME_TO_WAIT:
      {
         peerCb = (CzPeerCb *)cb;
         /* cz001.101 : CR fixes */
#if (ERRCLASS & ERRCLS_INT_PAR)
         if(NULLP == peerCb)
         {
            CZ_DBG_ERROR((CZ_PRNT_BUF, "NULLP PEER CB Passed\n "));
            CZ_RETVALUE(RFAILED);
         }
#endif
         arg.timers = &(peerCb->gpmCb.gpTimer);
         arg.max    = 1;
         break;
      }
      case CZ_TMR_TRELOCPREP:
      case CZ_TMR_TX2RELOCOVERALL:
      {
         ueCb = (CzUeCb *)cb;
#if (ERRCLASS & ERRCLS_INT_PAR)
         if(NULLP == ueCb)
         {
            CZ_DBG_ERROR((CZ_PRNT_BUF, "NULLP UECB Passed\n "));
            CZ_RETVALUE(RFAILED);
         }
#endif
         arg.timers = ueCb->bmpTimers;
         arg.max    = CZ_BMP_MAX_TIMERS;
         break;
      }
      default:
         CZ_DBG_ERROR((CZ_PRNT_BUF, "Invalid timerType passed %d\n ", tmrType));
         CZ_RETVALUE(RFAILED);
   }
   /* fill the argument */
   arg.tq     = czCb.czLyrTq;
   arg.tqCp   = &(czCb.czLyrTqCp);
   arg.cb     = (PTR)cb;
   arg.evnt   = tmrType;
   arg.wait   = wait;
   arg.tNum   = tmrNum;

   /* Start the timer only if timer value (wait) is not zero */
   if(wait != 0)
   {
      /* Start the Lower layer Cfg timer */
      cmPlcCbTq(&arg);
   }

   CZ_RETVALUE(ROK);
} /* End of czStartTmr */

/** 
 * @brief  The function handles the Protocol related Timers
 * @details This function stops timer event for Relocation adn TimeToWait.
 *
 * @param[in] tmrType   Timer event
 * @param[in] tmrCb     Control Block whether peerCb or UeCb 
 *                          
 * @return 
 *  - ROK     - If the function is successful
 *  - RFAILED - If the function is unsuccessful due to any failure.
*/
#ifdef ANSI
PUBLIC S16 czStopTmr
(
CzTmrType tmrType,
Void      *tmrCb
)
#else
PUBLIC S16 czStopTmr(tmrType, tmrCb)
CzTmrType tmrType;
Void      *tmrCb;
#endif
{
   CmTmrArg          arg;
   U8                tmrNum  = 0;
   CzPeerCb          *peerCb = NULLP;
   CzUeCb            *ueCb = NULLP;

   CZ_TRC2(czStopTmr)


   switch (tmrType)
   {
      case CZ_TMR_TRELOCPREP:
      case CZ_TMR_TX2RELOCOVERALL:
      {     
         ueCb = (CzUeCb *)tmrCb;
         if(NULLP == ueCb)
         {
            CZ_DBG_ERROR((CZ_PRNT_BUF, "NULLP UECB Passed for the \
                     timer evnet %d\n ", tmrType));
            CZ_RETVALUE(RFAILED);
         }
         for (tmrNum = 0; tmrNum < CZ_BMP_MAX_TIMERS; tmrNum++)
         {
            if (ueCb->bmpTimers[tmrNum].tmrEvnt == tmrType)
            {
               arg.timers = ueCb->bmpTimers;
               arg.max    = CZ_BMP_MAX_TIMERS;
               break;
            }
         }

         if(tmrNum >= CZ_BMP_MAX_TIMERS)
            CZ_RETVALUE(RFAILED);
         
         break;
      }
      case CZ_TMR_TIME_TO_WAIT:
      {
         peerCb = (CzPeerCb *)tmrCb;
         if(NULLP != peerCb)
         {
            arg.timers = &(peerCb->gpmCb.gpTimer);
            arg.max    = 1;
         }
         else
         {
           CZ_DBG_FATAL((CZ_PRNT_BUF, "NULLP PEERCB Passed for the\
                    CZ_TMR_TIME_TO_WAIT\n "));
            CZ_RETVALUE(RFAILED);
         }
         break;
      }
      default:
         CZ_DBG_FATAL((CZ_PRNT_BUF, "Invalid timerType passed \
                  %d\n ", tmrType));
         CZ_RETVALUE(RFAILED);
   }

   /* fill the argument */
   arg.tq     = czCb.czLyrTq;
   arg.tqCp   = &(czCb.czLyrTqCp);
   arg.cb     = (PTR)tmrCb;
   arg.evnt   = tmrType;
   arg.wait   = NOTUSED;
   arg.tNum   = tmrNum;

   /* Stop the Lower layer Cfg timer */
   cmRmvCbTq(&arg);


   CZ_RETVALUE(ROK);
} /* End of czStopTmr */


/** 
 * @brief   This function handles timer event for UEs.
 * @details This function stops timer event for UEs.
 *
 * @param[in] tmrCb     Control Block UeCb 
 *
 * @return 
 *  - ROK     - If the function is successful
 *  - RFAILED - If the function is unsuccessful due to any failure.
*/
#ifdef ANSI
PUBLIC S16 czStopUeTmrs
(
CzUeCb      *tmrCb
)
#else
PUBLIC S16 czStopUeTmrs(tmrCb)
CzUeCb      *tmrCb;
#endif
{
   CmTmrArg          arg;
   U8                tmrNum  = 0;
   CzUeCb            *ueCb = NULLP;

   CZ_TRC2(czStopUeTmrs)

   ueCb = (CzUeCb *)tmrCb;
   if(NULLP == ueCb)
   {
      CZ_DBG_ERROR((CZ_PRNT_BUF, "NULLP UECB Passed in czStopUeTmrs\n"));
      CZ_RETVALUE(RFAILED);
   }

   for (tmrNum = 0; tmrNum < CZ_BMP_MAX_TIMERS; tmrNum++)
   {
      if (ueCb->bmpTimers[tmrNum].tmrEvnt != TMR_NONE)
      {
         arg.timers = ueCb->bmpTimers;
         arg.max    = CZ_BMP_MAX_TIMERS;
         /* fill the argument */
         arg.tq     = czCb.czLyrTq;
         arg.tqCp   = &(czCb.czLyrTqCp);
         arg.cb     = (PTR)tmrCb;
         arg.evnt   = ueCb->bmpTimers[tmrNum].tmrEvnt;
         arg.wait   = NOTUSED;
         arg.tNum   = tmrNum;

         /* Stop the Lower layer Cfg timer */
         cmRmvCbTq(&arg);
      }
   }

   CZ_RETVALUE(ROK);
} /* End of czStopUeTmrs */


/**
 * @brief   This function handles the SCTP bind timer expiry.
 * @details If the number of bind retries is less the maximum configured 
            retry count, the function retries to bind again. 
            If maximum retry count is reached, an alarm is sent to 
            the layer manager.

 * @param[in] cb          SctSAP cb depending on the timer event
 * @param[in] tmrEvnt     Timer event for which the timer has expired

 * @return 
 *  - RETVOID  This function doesn't return any value.
*/
#ifdef ANSI
PUBLIC Void czProcGenTmrExpiry
(
Ptr       cb,       
S16       tmrEvnt 
)
#else
PUBLIC Void czProcGenTmrExpiry(cb, tmrEvnt)
Ptr       cb;
S16       tmrEvnt;
#endif
{
   CzSctSapCb    *lSapCb;   /* Lower SAP control block */
   CzUstaDgn      dgn;       /* Alarm Diagnostics */

   CZ_TRC2(czProcGenTmrExpiry);

   lSapCb = NULLP;

   switch(tmrEvnt)
   {
      case CZ_TMR_SCT_BIND:
      {
         lSapCb = (CzSctSapCb*)cb;
         if (lSapCb == (CzSctSapCb *)NULLP)
         {
            CZ_DBG_ERROR((czCb.init.prntBuf, 
                          "Null lSapCb in czProcGenTmrExpiry\n"));
            RETVOID;
         }
         if (lSapCb->state == LCZ_SAP_BINDING)
         {
            if (lSapCb->retryCnt < lSapCb->maxBndRetry)
            {
               /* Try to Bind again */
               lSapCb->retryCnt++;

               CZ_DBG_INFO((czCb.init.prntBuf, 
                  "Sending bind request to SCTP and starting timer again\n"));
               /* start timer to wait for bind confirm */
               (Void) czSchedGenTmr((S16)tmrEvnt, (Void*)lSapCb, CZ_TMR_START);

               /* issue bind request to the service provider */
              (Void)CzLiSctBndReq(&lSapCb->pst, lSapCb->suId,
                                  lSapCb->spId); 
            } /* if(lSapCb->bndRetryCnt < lSapCb->maxBndRetry) */
            else
            {
               lSapCb->retryCnt = 0;
               lSapCb->state = LCZ_SAP_CFG;

               CZ_SET_ZERO(&dgn, sizeof(CzUstaDgn));
               dgn.alarmType = LCZ_USTA_DGNVAL_SAPID;
               dgn.u.sapId = lSapCb->suId;

               /* Send Alarm to the layer manager */
               czLmmSendLmAlarm(LCM_CATEGORY_INTERFACE, 
                             LCM_EVENT_BND_FAIL,
                             LCM_CAUSE_UNKNOWN,
                             &dgn);
            }
         } /* if (lSapCb->state == LCZ_SAP_BINDING) */
         break;
      }
      default:
         break;
   } /* End switch */

   RETVOID;
} /* End of czProcGenTmrExpiry */

/** 
 * @brief   This function handles the Protocol Layer specific timer expires.
 * @details This function handles the relocation/TimeTowait timer expiry. 
 *
 * @param[in] cb          UE control block or peer Control block 
 * @param[in] tmrEvnt     Timer event for which the timer has expired
 *
 * @return 
 *  - RETVOID  This function doesn't return any value.
*/
#ifdef ANSI
PUBLIC Void czProcLyrTmrExpiry
(
Ptr       cb,
S16       tmrEvnt
)
#else
PUBLIC Void czProcLyrTmrExpiry(cb, tmrEvnt)
Ptr       cb;
S16       tmrEvnt;
#endif
{
   CzUeCb       *ueCb = NULLP;
   CzPeerCb     *peerCb = NULLP;

   CZ_TRC2(czProcLyrTmrExpiry);

   switch (tmrEvnt)
   {
      case CZ_TMR_TIME_TO_WAIT:
      {
         peerCb = (CzPeerCb *)cb;
         if(NULLP == peerCb)
           RETVOID;
         
         czGpmWaitTmrHdl(peerCb);
         break;
      }
      case CZ_TMR_TRELOCPREP:
      case CZ_TMR_TX2RELOCOVERALL:
      {
         ueCb = (CzUeCb *)cb;
         if(NULLP == ueCb)
         {
            CZ_DBG_ERROR((CZ_PRNT_BUF, "UECb Null when timer(%d) expired\n ", tmrEvnt));
            RETVOID;
         }

         czBmpHdlTmrEvnt(ueCb, tmrEvnt);
         break;
      }
      default:
         break;
   }

   RETVOID;
} /* End of czProcLyrTmrExpiry */


/** @} */
/**********************************************************************
  
         End of file:     tr_tmr.c@@/main/2 - Tue Aug 30 18:36:15 2011
   
**********************************************************************/
 
/**********************************************************************
        Revision History:

**********************************************************************/
/********************************************************************90**
 
     ver       pat    init                  description
------------ -------- ---- ----------------------------------------------
/main/1      ---        ds              1. LTE-X2AP Initial Release.
            cz001.101   mm              1. CR Fixes, Memory Leak fixes.
/main/2      ---        pt              1. LTE-X2AP 3.1 release.
/main/2     cz001.301   akaranth        1. X2AP kwork fix. 
*********************************************************************91*/
