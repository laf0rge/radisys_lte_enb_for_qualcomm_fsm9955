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
  
     Name:     LTE RRC Timer Module
 
     Type:     C source file
  
     Desc:     Timer Module Functions of RRC. 

     File:     nh_rtm.c
  
     Sid:      ni_rtm.c@@/main/3 - Fri Jul  1 01:13:22 2011
   
     Prg:      Reena
  
**********************************************************************/

/** @defgroup rtm_group Timer Module
 *  @{
 */
/* header include files (.h) */
static const char* RLOG_MODULE_NAME="RRC";
static int RLOG_MODULE_ID=512;
static int RLOG_FILE_ID=96;
#include "envopt.h"         /* Environment options */
#include "envdep.h"         /* Environment dependent */
#include "envind.h"         /* Environment independent */
#include "gen.h"            /* General layer                */
#include "ssi.h"            /* System services              */
#include "cm5.h"            /* Common timer module          */
#include "cm_hash.h"        /* Common hashlist             */
#include "cm_tkns.h"        /* Common tokens                */
#include "cm_inet.h"        /* Inet header file             */
#include "cm_mblk.h"        /* Common memory allocation     */
#include "cm_llist.h"       /* Common link list */
#include "cm_lte.h"        /* common LTE structure */ 
#include "ckw.h"            /* C-RLC interface */
#include "kwu.h"            /* D-RLC interface */
#include "crg.h"            /* C-MAC interface */
#include "cpj.h"            /* C-PDCP interface */
#include "pju.h"            /* D-PDCP interface */
#include "nhu.h"           /* RRC Upper Interface */
#include "nh.h"
#include "lnh.h"
/* external headers */

/* header/extern include files (.x) */
#include "gen.x"            /* General layer                */
#include "ssi.x"            /* System services              */
#include "cm5.x"            /* Common timer module          */
#include "cm_lib.x"         /* Common library               */
#include "cm_hash.x"        /* Common hashlist             */
#include "cm_tkns.x"        /* Common tokens                */
#include "cm_mblk.x"        /* Common memory allocation     */
#include "cm_inet.x"        /* Inet header file             */
#include "cm_llist.x"       /* Common link list */
#include "cm_lte.x"         /* Common LTE structure */ 
#include "ckw.x"            /* C-RLC interface */
#include "kwu.x"            /* D-RLC interface */
#include "crg.x"            /* C-MAC interface */
#include "cpj.x"            /* C-PDCP interface */
#include "pju.x"            /* D-PDCP interface */
#include "nhu.x"           /* RRC Upper Interface */
#include "lnh.x"
#include "nh.x"

/* local defines */

/* local typedefs */
 
/* local externs */
 
/* forward references */

/**
 *
 * @brief
 * 
 * @details
 * This function is invoked by system services to activate a task with a 
 * timer tick.
 * 
 * @param[in] Void
 * 
 * @return S16
 *  -# ROK
*/
#ifdef ANSI
PUBLIC S16 nhActvTmr
(
Void
)
#else /* ANSI */
PUBLIC S16 nhActvTmr(Void)
#endif /* ANSI */
{
   TRC3(nhActvTmr)

   /* Handle Generic layer Bind expiry */
   cmPrcTmr(&nhCb.nhGenTqCp, nhCb.nhGenTq, (PFV)nhProcGenTmrExpiry);

   /* Handle Lower Layer Cfg expiry */
   cmPrcTmr(&nhCb.nhLLyrCfgTqCp, nhCb.nhLLyrCfgTq, 
            (PFV)nhProcLLyrCfgTmrExpiry);

   RETVALUE(ROK);
} /* End of nhActvTmr */

/** 
 *
 * @brief
 * 
 * @details
 * 
 * This function starts/stops timer for bind event. This timer is used by 
 * RRC to bind with RLC, PDCP, and MAC layers.
 * 
 * @param[in] ctrlBlk     LSAP ctrlBlk or Cell ctrlBlk depending on the 
 *                        timer event
 * @param[in] tmrEvnt     Timer event for which timer needs to be started 
 *                        or stopped
 * @param[in] action      Timer Start or Stop
 * 
 * @return S16
 *  -# ROK
 *  -# RFAILED
*/
#ifdef ANSI
PUBLIC S16 nhSchedGenTmr
(
Ptr       ctrlBlk,       
S16       tmrEvnt,  
Action    action    
)
#else /* ANSI */
PUBLIC S16 nhSchedGenTmr(ctrlBlk, tmrEvnt, action)
Ptr       ctrlBlk;
S16       tmrEvnt;
Action    action;
#endif /* ANSI */
{
   NhLSapCb    *lSapCb;
   NhCellCb    *cellCb;
   U16         wait;
   CmTmrArg    arg;
   U8          tmrNum;
   S16         ret;

   TRC2(nhSchedGenTmr)

   RLOG0(L_DEBUG, "nhSchedGenTmr ");

   lSapCb = NULLP;
   wait   = 0;
   tmrNum = 0;
   ret    = ROK;

   switch(tmrEvnt)
   {
      case NH_TMR_WAIT_CPJ_BNDCFM:
      case NH_TMR_WAIT_CKW_BNDCFM:
      case NH_TMR_WAIT_CRG_BNDCFM:
      case NH_TMR_WAIT_PJU_BNDCFM:
      case NH_TMR_WAIT_KWU_BNDCFM:
#ifdef RRC_PHY_CFG_SUPP
      case NH_TMR_WAIT_CTF_BNDCFM:
#endif /* RRC_PHY_CFG_SUPP */
      {
         if(action == NH_TMR_START)
         {
            lSapCb = (NhLSapCb *)ctrlBlk;
            /* Get the wait value from LSap configuration */
            if ((lSapCb) && (lSapCb->bndTmr.enb == TRUE))
            {
               wait = lSapCb->bndTmr.val;
            }
            else
            {
               RLOG0(L_DEBUG, "Cfg timer not enabled ");
               RETVALUE(ROK);
            }
         }
         else 
         {
            for (tmrNum = 0; tmrNum < NH_MAX_TMRS; tmrNum++)
            {
               if ((nhCb).timers[tmrNum].tmrEvnt == tmrEvnt)
               {
                  break;
               }
            }
            if (tmrNum >= NH_MAX_TMRS)
            {
               RLOG0(L_DEBUG, "Bind timer not running ");
               RETVALUE(ROK);
            }
         }
      } 
      break;
      case NH_TMR_CELL_CFGCFM:
      {
         if(action == NH_TMR_START)
         {
            cellCb = (NhCellCb *)ctrlBlk;
            /* Get the wait value from general configuration */
            if ((cellCb) && (nhCb.genCfg.cfgTmr.enb == TRUE))
            {
               wait = nhCb.genCfg.cfgTmr.val;
            }
            else
            {
               RLOG0(L_DEBUG, "Cfg timer not enabled ");
               RETVALUE(ROK);
            }
         }
         else
         {
            for (tmrNum = 0; tmrNum < NH_MAX_TMRS; tmrNum++)
            {
               if (((nhCb).timers[tmrNum].tmrEvnt == tmrEvnt) &&
                  (((NhCellCb *)(nhCb.timers[tmrNum].cb)) == 
                                    (NhCellCb *)ctrlBlk))
               {
                     break;
               }
            }
            if (tmrNum >= NH_MAX_TMRS)
            {
               RLOG0(L_DEBUG, "Cell Cfg timer not running ");
               RETVALUE(ROK);
            }
         }
      }
      break;
      default:
      {
         RETVALUE(RFAILED);
      }
   }

   arg.tq     = nhCb.nhGenTq;
   arg.tqCp   = &nhCb.nhGenTqCp;
   arg.timers = nhCb.timers;
   arg.cb     = (PTR)ctrlBlk;
   arg.wait   = wait;
   arg.evnt   = tmrEvnt;
   arg.tNum   = tmrNum;
   arg.max    = NH_MAX_TMRS;

   /* Start the timer only if timer value (wait) is not zero */
   if(action == NH_TMR_START)
   {
      /* Start the Gen timer */
      cmPlcCbTq(&arg);
   }
   else if(action == NH_TMR_STOP)
   { 
      /* Stop the Gen timer */
      cmRmvCbTq(&arg);
   }
 
   RETVALUE(ROK);
} /* End of nhSchedGenTmr */

/** 
 *
 * @brief
 * 
 * @details
 * 
 * This function starts the Lower Layer Configuration timer event. 
 * This is used by RRC as a guard timer to configure RLC, PDCP and MAC layers.
 *
 * @param[in] cellAndUeCb   Cell and UE Control Blocks
 * @param[in] nxTransId     NX transaction identifier used for identifying 
 *                          the transCb on expiry
 *
 * @return S16
 *  -# ROK
 *  -# RFAILED
*/
#ifdef ANSI
PUBLIC S16 nhStartLLyrCfgTmr
(
NhCellAndUeCb  *cellAndUeCb,
U32            nxTransId
)
#else /* ANSI */
PUBLIC S16 nhStartLLyrCfgTmr(cellAndUeCb, nxTransId)
NhCellAndUeCb  *cellAndUeCb;
U32            nxTransId;
#endif /* ANSI */
{
   U16               wait;
   CmTmrArg          arg;
   U8                tmrNum;
   NhLLyrCfgTmrCb    *tmrCb;

   TRC2(nhStartLLyrCfgTmr)

   RLOG0(L_DEBUG, "nhStartLLyrCfgTmr");

   wait = 0;
   tmrNum = 0;
   tmrCb = NULLP;

   if ((cellAndUeCb) && (cellAndUeCb->cellCb) && (cellAndUeCb->ueCb)
       && (nhCb.genCfg.cfgTmr.enb == TRUE))
   {

      /* Get the wait value from general configuration */
      wait = nhCb.genCfg.cfgTmr.val;

      /* allocate and fill the control block to be pushed into timer queue */
      NH_ALLOC(NH_REG, NH_POOL, &tmrCb, sizeof(NhLLyrCfgTmrCb));
      if (NULLP == tmrCb)
      {
         RLOG0(L_FATAL, "Failed to allocate memory for tmrCb.");
         RETVALUE(RFAILED);
      }

      tmrCb->ueCb = cellAndUeCb->ueCb;
      tmrCb->transId = nxTransId;

      /* fill the argument */
      arg.tq     = nhCb.nhLLyrCfgTq;
      arg.tqCp   = &(nhCb.nhLLyrCfgTqCp);
      arg.timers = cellAndUeCb->cellCb->cfgTimers;
      arg.max    = NH_MAX_CFG_TMRS;
      arg.cb     = (PTR)tmrCb;
      arg.evnt   = NH_TMR_UE_LLYR_CFGCFM;
      arg.wait   = wait;
      arg.tNum   = tmrNum;

      /* Start the Lower layer Cfg timer */
      cmPlcCbTq(&arg);
   }

   RETVALUE(ROK);
} /* End of nhStartLLyrCfgTmr */

/** 
 *
 * @brief
 * 
 * @details
 * 
 * This function stops the Lower Layer Configuration timer event.
 * This is used by RRC as a guard timer to configure RLC, PDCP and MAC layers.
 *
 * @param[in] cellAndUeCb   Cell and UE Control Blocks
 * @param[in] nxTransId     NX transaction identifier used for identifying 
 *                          the transCb on expiry
 *
 * @return S16
 *  -# ROK
 *  -# RFAILED
*/
#ifdef ANSI
PUBLIC S16 nhStopLLyrCfgTmr
(
NhCellAndUeCb  *cellAndUeCb,
U32            nxTransId
)
#else /* ANSI */
PUBLIC S16 nhStopLLyrCfgTmr(cellAndUeCb, nxTransId)
NhCellAndUeCb  *cellAndUeCb;
U32            nxTransId;
#endif /* ANSI */
{
   CmTmrArg          arg;
   U8                tmrNum;
   NhLLyrCfgTmrCb    *tmrCb;
   U16         wait;

   TRC2(nhStopLLyrCfgTmr)

   RLOG0(L_DEBUG, "nhStopLLyrCfgTmr");

   wait = 0;
   tmrNum = 0;

   if ((cellAndUeCb) && (cellAndUeCb->cellCb) && (cellAndUeCb->ueCb))
   {
      for (tmrNum = 0; tmrNum < NH_MAX_CFG_TMRS; tmrNum++)
      {
         tmrCb = (NhLLyrCfgTmrCb *)(cellAndUeCb->cellCb->cfgTimers[tmrNum].cb);
         if ((NULLP != tmrCb) && (tmrCb->ueCb == cellAndUeCb->ueCb) &&
            (tmrCb->transId == nxTransId))
         {
            break;
         }
      }

      if (tmrNum >= NH_MAX_CFG_TMRS)
      {
         RLOG0(L_DEBUG, "Cfg timer not running ");
         RETVALUE(ROK);
      }

      /* fill the argument */
      arg.tq     = nhCb.nhLLyrCfgTq;
      arg.tqCp   = &(nhCb.nhLLyrCfgTqCp);
      arg.timers = cellAndUeCb->cellCb->cfgTimers;
      arg.max    = NH_MAX_CFG_TMRS;
      arg.cb     = (PTR)tmrCb;
      arg.evnt   = NH_TMR_UE_LLYR_CFGCFM;
      arg.wait   = NOTUSED;
      arg.tNum   = tmrNum;

      /* Stop the Lower layer Cfg timer */
      cmRmvCbTq(&arg);

      /* Deallocate the lower layer config control block */
      NH_FREE(NH_REG, NH_POOL, tmrCb, sizeof(NhLLyrCfgTmrCb));
   }

   RETVALUE(ROK);
} /* End of nhStopLLyrCfgTmr */

/** 
 *
 * @brief
 * 
 * @details
 * 
 * This function handles the RRC LSAP bind timer expiry. If the
 * number of bind retries is less the maximum configured retry count, 
 * the function retries to bind again. If maximum retry count is reached, 
 * an alarm is sent to the layer manager.
 * 
 * @param[in] ctrlBlk     LSAP ctrlBlk or cell ctrlBlk depending on the 
 *                        timer event
 * @param[in] tmrEvnt     Timer event for which the timer has expired
 * 
 * @return Void
 *  -# RETVOID
*/
#ifdef ANSI
PUBLIC Void nhProcGenTmrExpiry
(
Ptr       ctrlBlk,       
S16       tmrEvnt 
)
#else /* ANSI */
PUBLIC Void nhProcGenTmrExpiry(ctrlBlk, tmrEvnt)
Ptr       ctrlBlk;
S16       tmrEvnt;
#endif /* ANSI */
{
   NhLSapCb       *lSapCb;   /* Lower SAP control block */
   NhCellCb       *cellCb;   /* Cell control block */
   NhUstaDgn      dgn;       /* Alarm Diagnostics */

   TRC2(nhProcGenTmrExpiry);

   RLOG0(L_DEBUG, "nhProcGenTmrExpiry");

   lSapCb = NULLP;

   switch(tmrEvnt)
   {
      case NH_TMR_WAIT_CPJ_BNDCFM:
      case NH_TMR_WAIT_CKW_BNDCFM:
      case NH_TMR_WAIT_CRG_BNDCFM:
      case NH_TMR_WAIT_PJU_BNDCFM:
      case NH_TMR_WAIT_KWU_BNDCFM:
#ifdef RRC_PHY_CFG_SUPP
      case NH_TMR_WAIT_CTF_BNDCFM:
#endif /* RRC_PHY_CFG_SUPP */
      {
         lSapCb = (NhLSapCb*)ctrlBlk;
         if (lSapCb == (NhLSapCb *)NULLP)
         {
            RLOG0(L_ERROR, "Null lSapCb in nhProcGenTmrExpiry");
            RETVOID;
         }
         if (lSapCb->state == LNH_SAP_BINDING)
         {
            if (lSapCb->bndRetryCnt < NH_BND_INTRETRY)
            {
               /* Try to Bind again */
               lSapCb->bndRetryCnt++;

               /* start timer to wait for bind confirm */
               (Void) nhSchedGenTmr((Void*)lSapCb, (S16)tmrEvnt, NH_TMR_START);

               /* issue bind request to the service provider */
               nhLIMSendBndReq(lSapCb->sapElmId, lSapCb);
            } /* if(lSapCb->bndRetryCnt < NH_BND_INTRETRY) */
            else
            {
               lSapCb->bndRetryCnt = 0;
               lSapCb->state = LNH_SAP_CFG;

               NH_SET_ZERO(&dgn, sizeof(NhUstaDgn));
               dgn.type = LNH_USTA_DGNVAL_SAP;
               dgn.u.sapInfo.sapId = lSapCb->suId;
               dgn.u.sapInfo.sapElmId = lSapCb->sapElmId;

               /* Send Alarm to the layer manager */
               nhSendLmAlarm(LCM_CATEGORY_INTERFACE, 
                             LCM_EVENT_BND_FAIL,
                             LCM_CAUSE_UNKNOWN,
                             &dgn);
            }
         } /* if (lSapCb->state == LNH_SAP_BINDING) */
         break;
      }
      case NH_TMR_CELL_CFGCFM:
      {
         cellCb = (NhCellCb*)ctrlBlk;
#if (ERRCLASS & ERRCLS_DEBUG)
         if (cellCb == (NhCellCb *)NULLP)
         {
            RLOG0(L_ERROR, "Null cellCb in nhProcGenTmrExpiry");
            RETVOID;
         }
#endif /* (ERRCLASS & ERRCLS_DEBUG) */
         if(NH_CFG_ACTN_RLS != cellCb->cellTransCb.cfgAction)
         {
            /*R9 upgrade: Kworks Error: ret value check is deleted since this
             * function always returns ROK. in future if somebody changes the
             * ret value then he has to add the check
             */
            (Void)nhLIMHndlRvrtCellCfg(cellCb);
         }
         
         /* Send Cfg Cfm Report */
         if (RFAILED == nhUIMSndCellCfgCfm(cellCb->cellTransCb.cfmStatus, 
                                        cellCb, NHU_CFGERRCAUSE_TMR_EXPIRED))
         {
             RLOG0(L_ERROR, "nhUIMSndCellCfgCfm failed");
         }

         /* In case of cell setup or cell release failure, delete cell control
          * block */
         if(NH_CFG_ACTN_MODIFY != cellCb->cellTransCb.cfgAction)
         {
            if(ROK != nhDBMDeleteCellCb(&cellCb))
            {
               RLOG0(L_ERROR, "nhDBMDeleteCellCb failed");
            }
         }

         break;
      }
      default:
         break;
   } /* End switch */

   RETVOID;
} /* End of nhProcGenTmrExpiry */

/** 
 *
 * @brief
 * 
 * @details
 * 
 * This function handles the RRC lower layer configuration timer expiry. If 
 * the timer expires, lower layer configurations are reverted back and config 
 * confirm is sent to RRC user.
 *
 * @param[in] ctrlBlk          UE control block 
 *
 * @return Void
 *  -# RETVOID
*/
#ifdef ANSI
PUBLIC S16 nhProcLLyrCfgTmrExpiry
(
Ptr       ctrlBlk
)
#else /* ANSI */
PUBLIC S16 nhProcLLyrCfgTmrExpiry(ctrlBlk)
Ptr       ctrlBlk;
#endif /* ANSI */
{
   NhLLyrCfgTmrCb *tmrCb=NULLP;
   NhActvUeCb     *ueCb;
   TransCb      *transCb;
   NhUstaDgn    dgn;       /* Alarm Diagnostics */
   NhId         nhIdentity;
   U8           cfgMask;
   U8           cfmStatus;
   NhTransId    transId;
   NhCellCb     *cellCb = NULLP;

   TRC2(nhProcLLyrCfgTmrExpiry);

   RLOG0(L_DEBUG, "nhProcLLyrCfgTmrExpiry");

   tmrCb = (NhLLyrCfgTmrCb *)ctrlBlk;

   if (NULLP == tmrCb)
   {
      RLOG0(L_ERROR, "Timer control block NULL!");
      RETVALUE(RFAILED);
   }

   transCb = nhUTLGetTransCbFrmNxTransId(tmrCb->ueCb, tmrCb->transId);
   if (transCb == NULLP)
   {
     /* De-allocate the memory allocated for tmrCb */
     NH_FREE(NH_REG, NH_POOL, tmrCb, sizeof(NhLLyrCfgTmrCb));
     NH_SET_ZERO(&dgn, sizeof(NhUstaDgn));
     dgn.type = LNH_USTA_DGNVAL_TIMER;
     RETVALUE(RFAILED);
   }

   ueCb = tmrCb->ueCb;

   /* De-allocate the memory allocated for tmrCb */
   NH_FREE(NH_REG, NH_POOL, tmrCb, sizeof(NhLLyrCfgTmrCb));

   /* ueId and cellId required to revert bk config */
   nhIdentity.ueId  = ueCb->ueCbKey.crnti;
   nhIdentity.cellId = ueCb->cellCb->cellId;
   
   /* Revert is required, Delete the new entry for UE if ueidChangereq
    * has been sent to lower layers */
   if(transCb->ordrdCfg.actionMask & NH_ORD_CFG_UEID &&
      transCb->ordrdCfg.ueInfo.crntiCfgType == NH_UE_INFO_MOD_CRNTI)
   {
      if (NULLP == (cellCb = nhDBMGetCellCb(nhIdentity.cellId)))
      {
         RLOG0(L_ERROR, "NULLP Cell Cb");
         RETVALUE(RFAILED);
      }
      if (ROK != nhHashListDelete(&(cellCb->newUeHlCp), (PTR)(ueCb)))
      {
          RLOG0(L_FATAL, "Could not delete from  list.");
          RETVALUE(RFAILED);
      }
   }
   
   /* Handle the timer expiry when the PDCP discard config after
    * Security mode cmd complete does not get a confirm from PDCP. */
   if ((transCb->cause == NH_MSGTYPE_SECMODCMD) &&
       (transCb->ordrdCfg.actionMask == NH_ORD_CFG_RB_MOD))
   {
      /*R9 upgrade: Kworks Error: ret value check is deleted since this
       * function always returns ROK. in future if somebody changes the
       * ret value then he has to add the check
       */
      (Void)nhDBMDelTransCb(ueCb, &transCb);

      RETVALUE(ROK);
   }

   /* Call LIM handler to revert back lower layer configurations */
   /*R9 upgrade: Kworks Error: ret value check is deleted since this
    * function always returns ROK. in future if somebody changes the
    * ret value then he has to add the check
    */
   (Void)nhLIMHndlRvrtCfg(&nhIdentity, ueCb, transCb);

   cfgMask   = transCb->cfgMask;
   cfmStatus = transCb->cfmStatus;
   transId   = transCb->nxTransId;

   /*R9 upgrade: Kworks Error: ret value check is deleted since this
    * function always returns ROK. in future if somebody changes the
    * ret value then he has to add the check
    */
   (Void)nhDBMDelTransCb(ueCb, &transCb);

   /* Send Cfg Cfm Report to UIM module */
   if (RFAILED == nhUIMSndCfgCfm(nhIdentity, transId,
                             cfgMask, cfmStatus, NHU_CFGERRCAUSE_TMR_EXPIRED))
   {
      RLOG0(L_ERROR, "nhUIMSndCfgCfm failed");
      RETVALUE(RFAILED);
   }

   RETVALUE(ROK);
} /* End of nhProcLLyrCfgTmrExpiry */

/** @} */
/**********************************************************************
  
         End of file:     ni_rtm.c@@/main/3 - Fri Jul  1 01:13:22 2011
   
**********************************************************************/
 
/**********************************************************************
        Revision History:

**********************************************************************/
/********************************************************************90**
 
     ver       pat    init                  description
------------ -------- ---- ----------------------------------------------
/main/1      ---      rer              1. LTE-RRC Initial Release.
/main/2      ---      ch               1. RRC Release 2.1.
/main/3      ---      sbalakrishna     1. Updated for Release of 3.1.
*********************************************************************91*/
