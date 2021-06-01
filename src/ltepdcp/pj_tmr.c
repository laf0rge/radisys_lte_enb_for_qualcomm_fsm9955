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
  
        Name:    PDCP - TMR module file
    
        Type:    C source file
  
        Desc:    Source code for timer functions such as, 

                 - pjStartTmr
                 - pjStopTmr
                 - pjTmrExpiry
                 - pjHdlDiscTmrExp
                 - pjHdlInitObdTmrExp 
                 - pjHdlUeDelWaitTmrExp
                  
        File:    gp_pj_tmr.c

        Sid:      eq_tmr.c@@/main/TeNB_Main_BR/tenb_brcm_4.0_tdd_dlfs_IT/3 - Wed Dec 10 12:46:47 2014
 
        Prg:     vp
  
*********************************************************************21*/
static const char* RLOG_MODULE_NAME="PDCP";
static int RLOG_MODULE_ID=1024;
static int RLOG_FILE_ID=223;


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
#include "cpj.h"           /* RRC layer */
#include "pju.h"           /* PDCP service user */
#include "kwu.h"
#include "lpj.h"           /* LPJ defines */
#include "pj_env.h"        /* RLC environment options */
#include "pj.h"            /* RLC defines */
#include "pj_err.h"        /* Error defines */
#include "pj_udx.h"

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
#include "cpj.x"           /* RRC layer */
#include "kwu.x"
#include "pju.x"           /* PDCP service user */
#include "lpj.x"           /* LPJ */
#include "pj.x"
#include "pj_udx.h"
#include "pj_udx.x"
#include "pj_ul.x"
#include "pj_dl.x"

/* local defines */

/* local externs */

/* forward references */
PRIVATE Void pjBndTmrExpiry ARGS ((S16 tmrEvnt, PTR cb));
/* public variable declarations */

/* This structure holds all the global structs we need. */

/* private variable declarations */

/* private function declarations */
/** @file gp_pj_tmr.c
@brief PDCP Timer Module
**/

/**
 *
 * @brief Handler to start timer
 *       
 *
 * @b Description
 *        This function is invoked by DLM modules to start the discard timer for SDUs.
 *
 *
 *  @param[in] cb      Transmission Buffer control block 
 *  @param[in] tmrEvnt Timer event to be started
 *
 * 
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED 
*/
#ifdef ANSI
PUBLIC S16 pjStartTmr
(
PjCb      *gCb,
PTR       cb,           /* Parent control block */
S16       tmrEvnt       /* Timer event */
)
#else
PUBLIC S16 pjStartTmr (gCb, cb, tmrEvnt) 
PjCb      *gCb;
PTR       cb;           /* Parent control block */
S16       tmrEvnt;      /* Timer event */
#endif
{
   PjDlRbCb   *dlRbCb;
   PjUlRbCb   *ulRbCb;
   PjTxEnt  *txEnt;
   U16      wait;
   S16      waitTime;
   CmTimer  *tmr;
   U8       maxTmrs;
   U32      count;
   Ticks    curTime;
   CmTmrArg arg;
   /* kw005.201 added support for L2 Measurement */
#ifdef LTE_L2_MEAS_COMMENT
   PjL2MeasEvtCb *measEvtCb;
#endif

#ifdef LTE_L2_MEAS
    PjL2Cb *pjL2Cb = NULLP;
    PjCb   *tPjMeasCb = NULLP;
#endif

   TRC2(pjStartTmr)

   RLOG1(L_DEBUG, "pjStartTmr(cb, tmrEvnt (%d)) ", tmrEvnt);

   ulRbCb  = NULLP; 
   dlRbCb  = NULLP;
   txEnt   = NULLP;
   wait    = 0;
   waitTime = 0;
   tmr     = NULLP;
   maxTmrs = 0;
   count   = 0;
   curTime = 0;

   switch (tmrEvnt)
   {
      case PJ_EVT_WAIT_KWU_BNDCFM:
      {
         PjKwuSapCb *kwuSap = (PjKwuSapCb *)cb;
         wait   = (U16)((kwuSap->bndTmrInt * SS_TICKS_SEC)/(gCb->pjGenCfg.timeRes * 1000));  /*KW_FIX*/
         if( (0 != kwuSap->bndTmrInt) && (0 == wait) )
         {
            wait = 1;
         }
         tmr    = &kwuSap->bndTmr;
         maxTmrs = PJ_MAX_KWUSAP_TMR;
         break;
      }
      case PJ_EVT_WAIT_UDX_BNDCFM:
      {
         PjUdxUlSapCb *udxSap = (PjUdxUlSapCb *)cb;
         wait   = (U16)((udxSap->bndTmrInt * SS_TICKS_SEC)/(gCb->pjGenCfg.timeRes * 1000));  /*KW_FIX*/
         if( (0 != udxSap->bndTmrInt) && (0 == wait) )
         {
            wait = 1;
         }
         tmr    = &udxSap->bndTmr;
         maxTmrs = PJ_MAX_UDXSAP_TMR;
         break;
      }    
#if (defined(PJ_SEC_ASYNC) || defined(PJ_CMP_ASYNC))
      case PJ_EVT_DL_OBD_TMR:
      {
         dlRbCb = (PjDlRbCb *)cb;
         tmr=&(dlRbCb->dlCb.obdTmr);
         maxTmrs = PJ_MAX_OBD_TMR;
         txEnt = pjDbmGetTxEnt(gCb,&(dlRbCb->dlCb.txBuf), dlRbCb->dlCb.nxtToSub);
         if ( txEnt != NULLP )
         {
            wait = gCb->pjGenCfg.obdTmrInt;
            dlRbCb->dlCb.obdCount = txEnt->count;
         }

         break;
      }
      case PJ_EVT_UL_OBD_TMR:
      {
         ulRbCb = (PjUlRbCb *)cb;
         tmr=&(ulRbCb->ulCb.obdTmr);
         maxTmrs = PJ_MAX_OBD_TMR;
         PJ_ULM_GET_SUBCNT(ulRbCb,(ulRbCb->ulCb.obdPdu),gCb);
         if(pjDbmGetRxEnt(gCb,&(ulRbCb->ulCb.recBuf), ulRbCb->ulCb.obdPdu) != NULLP )
         {
            wait = gCb->pjGenCfg.obdTmrInt;
         }

         break;
      } 
      case PJ_EVT_OBD_WAIT_TMR:
      {
         if (gCb->pjGenCfg.mode == LPJ_MODE_PDCP_DL)
         {
            PjDlUeCb  *dlUeCb = (PjDlUeCb *)cb;
            tmr=&(dlUeCb->libInfo.obdTmr);
         }
         else
         {
            PjUlUeCb  *ulUeCb = (PjUlUeCb *)cb;
            tmr=&(ulUeCb->libInfo.obdTmr);
         }
         maxTmrs = PJ_MAX_OBD_TMR;
         wait = gCb->pjGenCfg.obdWtTmrInt;
         break;
      }

#endif
      /* kw005.201 added support for L2 Measurement */
#ifdef LTE_L2_MEAS_COMMENT
      case PJ_EVT_L2_TMR:
      {
         measEvtCb = (PjL2MeasEvtCb *)cb;
         tmr=&(measEvtCb->l2Tmr);
         maxTmrs = PJ_L2_MAX_TIMERS;
         wait = (measEvtCb->l2TmrCfg.val * SS_TICKS_SEC)/(pjCb.genCfg.timeRes * 1000);
         if((measEvtCb->l2TmrCfg.val > 0) && wait == 0)
         {
            wait = 1;
         }
         break;
      }
#endif
#ifdef LTE_L2_MEAS
      case PJ_EVT_L2_MEAS_TMR:
      {
         tPjMeasCb = (PjCb      *)cb;
         pjL2Cb = &tPjMeasCb->u.ulCb->pjL2Cb;
         tmr=&(pjL2Cb->measTmr);
         maxTmrs = PJ_L2_MAX_TIMERS;
         wait = (pjL2Cb->measTmrCfg.val * SS_TICKS_SEC)/(gCb->pjGenCfg.timeRes * 1000);
         wait = 500;
         if((pjL2Cb->measTmrCfg.val > 0) && wait == 0)
         {
            wait = 1;
         }
         break;
      }
#endif
      default:
      {
         RLOG0(L_ERROR, "Invalid Start tmr Evnt");
         RETVALUE(RFAILED);
      }
   } 

   if(wait != 0)
   {
      arg.tqCp   = &gCb->pjTqCp;
      arg.tq     = gCb->pjTq;
      arg.cb     = cb;
      arg.timers = tmr;
      arg.evnt   = tmrEvnt;
      arg.wait   = wait;
      arg.tNum   = 0;
      arg.max    = maxTmrs;

      cmPlcCbTq(&arg);
   }

   RETVALUE(ROK);
}

/**
 *
 * @brief Handler to stop timer
 *       
 *
 * @b Description
 *        This function is used to stop protocol timer, based on the timer event.
 *
 *
 *  @param[in] cb      Transmission Buffer control block 
 *  @param[in] tmrEvnt Timer event to be started 
 *
 * 
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED 
*/
#ifdef ANSI
PUBLIC S16 pjStopTmr
(
PjCb   *gCb,
PTR    cb,              /* Parent control block */
U8     tmrEvnt          /* Timer */
)
#else
PUBLIC S16 pjStopTmr (gCb, cb, tmrEvnt) 
PjCb   *gCb;
PTR    cb;              /* Parent control block */
U8     tmrEvnt;         /* Timer */
#endif
{
   CmTmrArg     arg;
   CmTimer      *tmr;
   PjDlRbCb     *pjDlRbCb;
   PjUlRbCb     *pjUlRbCb;
   PjKwuSapCb   *kwuSap;
   PjUdxUlSapCb   *udxSap;
#if (defined(PJ_SEC_ASYNC) || defined(PJ_CMP_ASYNC))
   PjDlUeCb       *dlUeCb;
   PjUlUeCb       *ulUeCb;
#endif
   /* kw005.201 added support for L2 Measurement */
#ifdef LTE_L2_MEAS
   PjL2Cb *pjL2Cb = NULLP;
   PjCb   *tPjMeasCb = NULLP;
#endif
   U8           maxTimers;

   TRC2(pjStopTmr)
   
   RLOG1(L_DEBUG, "pjStopTmr(cb, tmrEvnt(%d)) ", tmrEvnt);
   
   tmr = NULLP;
   maxTimers = 0;
   pjUlRbCb    = NULLP; 
   pjDlRbCb    = NULLP;
#if (defined(PJ_SEC_ASYNC) || defined(PJ_CMP_ASYNC))
   /* ueCb   = NULLP; */
#endif

   switch (tmrEvnt)
   {
      case PJ_EVT_WAIT_KWU_BNDCFM:
      {
         kwuSap = (PjKwuSapCb *)cb;
         tmr = &kwuSap->bndTmr;
         arg.max = PJ_MAX_KWUSAP_TMR;
         break;
      }
      case PJ_EVT_WAIT_UDX_BNDCFM:
      {
         udxSap = (PjUdxUlSapCb *)cb;
         tmr = &udxSap->bndTmr;
         arg.max = PJ_MAX_UDXSAP_TMR;
         break;
      }
#if (defined(PJ_SEC_ASYNC) || defined(PJ_CMP_ASYNC))
      case PJ_EVT_DL_OBD_TMR:
      {
         pjDlRbCb = (PjDlRbCb *)cb;
         tmr = &(pjDlRbCb->dlCb.obdTmr);
         maxTimers = PJ_MAX_OBD_TMR;
         break;
      }
      case PJ_EVT_UL_OBD_TMR:
      {
         pjUlRbCb = (PjUlRbCb *)cb;
         tmr = &(pjUlRbCb->ulCb.obdTmr);
         maxTimers = PJ_MAX_OBD_TMR;
         break;
      }
      case PJ_EVT_OBD_WAIT_TMR:
      {
         /*if (gCb->pjGenCfg.mode == LPJ_PDCP_MODE_DL)*/
         if (gCb->pjGenCfg.mode == LPJ_MODE_PDCP_DL)
         {
            dlUeCb = (PjDlUeCb *)cb;
            tmr  = &(dlUeCb->libInfo.obdTmr);
         }
         else
         {
            ulUeCb = (PjUlUeCb *)cb;
            tmr  = &(ulUeCb->libInfo.obdTmr);
         }
         maxTimers = PJ_MAX_OBD_TMR;
         break;
      }

#endif
     /* kw005.201 added support for L2 Measurement */
#ifdef LTE_L2_MEAS_COMMENT
      case PJ_EVT_L2_TMR:
      {
         measEvtCb = (PjL2MeasEvtCb *)cb;
         tmr  = &(measEvtCb->l2Tmr);
         maxTimers = PJ_L2_MAX_TIMERS;
         break;
      }
#endif
#ifdef LTE_L2_MEAS
      case PJ_EVT_L2_MEAS_TMR:
      {
         tPjMeasCb = (PjCb      *)cb;
         pjL2Cb = &tPjMeasCb->u.ulCb->pjL2Cb;
         tmr  = &(pjL2Cb->measTmr);
         maxTimers = PJ_L2_MAX_TIMERS;
         break;
      }
#endif

      default:
      {
         RLOG0(L_ERROR, "Invalid Stop tmr Evnt");
         RETVALUE(RFAILED);
      }
   }
   
   
   if (tmrEvnt != TMR0)
   {
      arg.tqCp    = &gCb->pjTqCp;
      arg.tq      = gCb->pjTq;
      arg.cb      = (PTR)cb;
      arg.timers  = tmr;
      arg.evnt    = tmrEvnt;
      arg.wait    = 0;
      arg.max     = maxTimers;
      arg.tNum    = 0;
      cmRmvCbTq(&arg);

   }
   
   RETVALUE(ROK);
}

/**
 *
 * @brief Handler to invoke events on expiry of timer.
 *       
 *
 * @b Description
 *        This function is used to handle expiry of timer,it invokes relevant functions.
 *
 *
 *  @param[in] cb        Transmission Buffer control block 
 *  @param[in] tmrType   Type of the timer to be stopped
 *
 * 
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED 
*/
#ifdef ANSI
PUBLIC S16 pjTmrExpiry
(
PTR       cb,           /* Parent control block */
S16       tmrEvnt       /* Timer event */
)
#else
PUBLIC S16 pjTmrExpiry (cb, tmrEvnt)
PTR       cb;           /* Parent control block */
S16       tmrEvnt;      /* Timer event */
#endif
{
#if (defined(PJ_SEC_ASYNC) || defined(PJ_CMP_ASYNC))
   PjDlRbCb      *dlRbCb        = NULLP;
   PjUlRbCb      *ulRbCb        = NULLP; 
   PjDlUeCb      *dlUeCb          = NULLP;
   PjUlUeCb      *ulUeCb          = NULLP;
#endif 
#ifdef LTE_L2_MEAS
   PjL2Cb *pjL2Cb = NULLP;
   PjCb   *tPjMeasCb = NULLP;
#endif
   /* pj005.201 added support for L2 Measurement */
   TRC2(pjTmrExpiry)

  switch (tmrEvnt)
   {
      case PJ_EVT_WAIT_KWU_BNDCFM:
      {
         pjBndTmrExpiry(tmrEvnt,cb);
         /* kw005.201 added missing break statement */
         break;
      }
      case PJ_EVT_WAIT_UDX_BNDCFM:
      {
         pjBndTmrExpiry(tmrEvnt,cb);
         /* kw005.201 added missing break statement */
         break;
      }
#if (defined(PJ_SEC_ASYNC) || defined(PJ_CMP_ASYNC))
      case PJ_EVT_DL_OBD_TMR:
      {
         PjCb *tPjCb;
         dlRbCb = (PjDlRbCb *)cb;

         if (dlRbCb->ueCb->inst >= PJ_MAX_PDCP_INSTANCES)
         {
            RETVALUE(RFAILED);
         }
         tPjCb = PJ_GET_PJCB(dlRbCb->ueCb->inst);

         pjDlmObdTmrExp(tPjCb,dlRbCb);
         break;
      }
      case PJ_EVT_UL_OBD_TMR:
      {
         PjCb *tPjCb;
         ulRbCb = (PjUlRbCb *)cb;

         if (ulRbCb->ueCb->inst >= PJ_MAX_PDCP_INSTANCES)
         {
            RETVALUE(RFAILED);
         }
         tPjCb = PJ_GET_PJCB(ulRbCb->ueCb->inst);
          pjUlmHdlObdTmrExpiry(tPjCb,ulRbCb);
         break;
      }
      case PJ_EVT_OBD_WAIT_TMR:
      {
         PjCb *tPjCb;
/* ********************************************************** */         
         tPjCb = (PjCb*)cb;
         if (tPjCb->pjGenCfg.mode == LPJ_MODE_PDCP_UL)
         {
            ulUeCb = (PjUlUeCb *)cb;
            if (ulUeCb->inst >= PJ_MAX_PDCP_INSTANCES)
            {
                RETVALUE(RFAILED);
            }
            tPjCb = PJ_GET_PJCB(ulUeCb->inst);
            pjUtlUlHdlInitObdTmrExp(tPjCb,ulUeCb);
            
         }
         else
         {
             dlUeCb = (PjDlUeCb *)cb;
             if (dlUeCb->inst >= PJ_MAX_PDCP_INSTANCES)
             {
                 RETVALUE(RFAILED);
             }
             tPjCb = PJ_GET_PJCB(dlUeCb->inst);
             pjUtlDlHdlInitObdTmrExp(tPjCb,dlUeCb);
         }
         break;
      }

#endif
#ifdef LTE_L2_MEAS
      case PJ_EVT_L2_MEAS_TMR:
      {
         tPjMeasCb = (PjCb      *)cb;
         pjL2Cb = &tPjMeasCb->u.ulCb->pjL2Cb;
         pjUtlMemCpuHdlTmrExp((PjCb *)cb,pjL2Cb);
      }
      break;
#endif

     /* pj005.201 added support for L2 Measurement */
      default:
      {
         RETVALUE(RFAILED);
      }
   }

   RETVALUE(ROK);
}

/**
 *
 * @brief Handler to check if the timer is running
 *       
 *
 * @b Description
 *        This function is used to check if the timer is running at the given
 *        moment.
 *
 *
 *  @param[in] cb      Transmission Buffer control block 
 *  @param[in] tmrType   Type of the timer to be stopped 
 *
 * 
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED 
 */
#ifdef ANSI
PUBLIC Bool pjChkTmr
(
PjCb      *gCb,
PTR       cb,           /* Parent control block */
S16       tmrEvnt       /* Timer event */
)
#else
PUBLIC Bool pjChkTmr(gCb, cb, tmrEvnt) 
PjCb      *gCb;
PTR       cb;           /* Parent control block */
S16       tmrEvnt;      /* Timer event */
#endif
{
#if (defined(PJ_SEC_ASYNC) || defined(PJ_CMP_ASYNC))
   PjUlRbCb  *ulRbCb         = NULLP;
   PjDlUeCb  *dlUeCb         = NULLP;
   PjUlUeCb  *ulUeCb         = NULLP;
#endif
#ifdef LTE_L2_MEAS
   PjL2Cb *pjL2Cb = NULLP;
   PjCb   *tPjMeasCb = NULLP;
#endif
   Bool tmrRunning = FALSE;

   TRC2(pjChkTmr)

   RLOG1(L_UNUSED, "pjChkTmr(cb, tmrEvnt(%d)) ", tmrEvnt);

   switch (tmrEvnt)
   {
#if (defined(PJ_SEC_ASYNC) || defined(PJ_CMP_ASYNC))
      case PJ_EVT_DL_OBD_TMR:
      {
         dlRbCb = (PjDlRbCb *)cb;
         if (dlRbCb->dlCb.obdTmr.tmrEvnt == PJ_EVT_DL_OBD_TMR)
         {
            tmrRunning = TRUE;
         }
         break;
      }
      case PJ_EVT_UL_OBD_TMR:
      {
          ulRbCb = (PjUlRbCb *)cb;
          if (ulRbCb->ulCb.obdTmr.tmrEvnt == PJ_EVT_UL_OBD_TMR)
          {
              tmrRunning = TRUE;
          }
          break;
      }
      case PJ_EVT_OBD_WAIT_TMR:
      {
          if(gCb->pjGenCfg.mode == LPJ_MODE_PDCP_UL)
          {
              ulUeCb = (PjUlUeCb *)cb;
              if (ulUeCb->inst >= PJ_MAX_PDCP_INSTANCES)
              {
                  RETVALUE(RFAILED);
              }
              if (ulUeCb->libInfo.obdTmr.tmrEvnt == PJ_EVT_OBD_WAIT_TMR)
              {
                  tmrRunning = TRUE;
              }
          }
          else
          {
              dlUeCb = (PjDlUeCb *)cb;
              if (dlUeCb->inst >= PJ_MAX_PDCP_INSTANCES)
              {
                  RETVALUE(RFAILED);
              }
              if (dlUeCb->libInfo.obdTmr.tmrEvnt == PJ_EVT_OBD_WAIT_TMR)
              {
                  tmrRunning =  TRUE;
              }
          }
          break;
      }
#endif
#ifdef LTE_L2_MEAS
      case PJ_EVT_L2_MEAS_TMR:
      {
         tPjMeasCb = (PjCb      *)cb;
         pjL2Cb = &tPjMeasCb->u.ulCb->pjL2Cb;
         if (pjL2Cb->measTmr.tmrEvnt == PJ_EVT_L2_MEAS_TMR)
         {
             tmrRunning = TRUE;
         }
         break;
      }
#endif
      default:
      {

          RLOG0(L_ERROR, "Invalid Chk tmr Evnt");
          RETVALUE(RFAILED);
      }
   } 

   RETVALUE(tmrRunning);
}
/**
 *
 * @brief Private handler to invoke an event for bind timer expiry
 *       
 *
 * @b Description
 *        This function processes the RLC bind timer expiry. If the number of retry is
 *        less than the maximum retry counter, bind request is sent again, else an 
 *        alarm is raised to the layer manager.
 *
 *
 *  @param[in] cb      RB control block 
 *
 * 
 *   @return Void
 *      -# RETVOID
*/
#ifdef ANSI
PRIVATE Void pjBndTmrExpiry
(
S16       tmrEvnt,
PTR       cb            /* Parent control block */
)
#else
PRIVATE Void pjBndTmrExpiry(tmrEvnt, cb) 
S16       tmrEvnt;
PTR       cb;           /* Parent control block */
#endif
{
   PjCb          *tPjCb;
   PjUdxUlSapCb  *udxSap;
   PjKwuSapCb *kwuSapCb; 

   TRC2(pjBndTmrExpiry)

   if (tmrEvnt == PJ_EVT_WAIT_KWU_BNDCFM)
   {
      kwuSapCb = (PjKwuSapCb *) cb;

      tPjCb = PJ_GET_PJCB(kwuSapCb->pst.srcInst);
      if (kwuSapCb->state == PJ_SAP_BINDING)
      {
         if (kwuSapCb->retryCnt < PJ_MAX_SAP_BND_RETRY)
         {
            /* Send bind request */
            kwuSapCb->retryCnt++;

            /* start timer to wait for bind confirm */
            pjStartTmr(tPjCb, (PTR)kwuSapCb, PJ_EVT_WAIT_KWU_BNDCFM);

            PjLiKwuBndReq (&kwuSapCb->pst, kwuSapCb->suId, kwuSapCb->spId);
         }
         else
         {
            kwuSapCb->retryCnt = 0;
            kwuSapCb->state = PJ_SAP_CFG;

            /* Send alarm to the layer manager */
#ifdef LTE_L2_MEAS
            pjLmmSendAlarm(tPjCb,LCM_CATEGORY_INTERFACE, LCM_EVENT_BND_FAIL,
                           LCM_CAUSE_TMR_EXPIRED, 0, 0, 0);
#else
            pjLmmSendAlarm(tPjCb,LCM_CATEGORY_INTERFACE, LCM_EVENT_BND_FAIL,
                           LCM_CAUSE_TMR_EXPIRED, 0, 0);
#endif
         }
      }
   }
   else
   {
      udxSap = (PjUdxUlSapCb *)cb;

      tPjCb  = PJ_GET_PJCB (udxSap->pst.srcInst);

      if (udxSap->state == PJ_SAP_BINDING)
      {
         if (udxSap->retryCnt < PJ_MAX_SAP_BND_RETRY)
         {
            /* Send bind request */
            udxSap->retryCnt++;

            /* start timer to wait for bind confirm */
            pjStartTmr(tPjCb, (PTR)udxSap, PJ_EVT_WAIT_UDX_BNDCFM);

            PjUlUdxBndReq (&udxSap->pst, udxSap->suId, udxSap->spId);
         }
         else
         {
            udxSap->retryCnt = 0;
            udxSap->state = PJ_SAP_CFG;

            /* Send alarm to the layer manager */
#ifdef LTE_L2_MEAS
            pjLmmSendAlarm(tPjCb,LCM_CATEGORY_INTERFACE, LCM_EVENT_BND_FAIL,
                           LCM_CAUSE_TMR_EXPIRED, 0, 0, 0);
#else
            pjLmmSendAlarm(tPjCb,LCM_CATEGORY_INTERFACE, LCM_EVENT_BND_FAIL,
                           LCM_CAUSE_TMR_EXPIRED, 0, 0);
#endif
         }
      }

   }

   RETVOID;
}


  
/********************************************************************30**
  
         End of file:     eq_tmr.c@@/main/TeNB_Main_BR/tenb_brcm_4.0_tdd_dlfs_IT/3 - Wed Dec 10 12:46:47 2014
  
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
/main/1      ---      vp   1. LTERLC Release 2.1
/main/2     kw005.201 ap   1. Added support for L2 Measurement.
*********************************************************************91*/
