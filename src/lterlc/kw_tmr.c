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
  
        Name:    RLC - TMR module file
    
        Type:    C source file
  
        Desc:    Source code for timer functions such as, 

                 - kwStartTmr
                 - kwStopTmr
                 - kwTmrExpiry
                 - kwBndTmrExpiry  
                  
        File:    gp_tmr.c

        Sid:      gp_tmr.c@@/main/TeNB_Main_BR/tenb_main_ccb/tenb_5.0_RIB/tenb_5.0_RIB_GA/6 - Mon Feb 15 12:52:41 2016
 
        Prg:     nm
  
*********************************************************************21*/
static const char* RLOG_MODULE_NAME="TMR";
static int RLOG_MODULE_ID=2048;
static int RLOG_FILE_ID=202;


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
#include "kw_env.h"        /* RLC environment options */

#include "kw.h"            /* RLC defines */
#include "kw_err.h"        /* Error defines */
#include "kw_ul.h"
#include "kw_udx.h"
#include "kw_dl.h"

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

/** 
 * @file gp_tmr.c
 * @brief RLC Timer Module
*/

/**
 * @def KW_TMR_CALCUATE_WAIT
 *
 *    This macro calculates and assigns wait time based on the value of the 
 *    timer and the timer resolution. Timer value of 0 signifies that the
 *    timer is not configured
 *
 * @param[out] _wait   Time for which to arm the timer changed to proper 
 *                     value according to the resolution
 * @param[in] _tmrVal   Value of the timer
 * @param[in] _timerRes   Resolution of the timer
 *
*/
#define KW_TMR_CALCUATE_WAIT(_wait, _tmrVal, _timerRes)       \
{                                                             \
   (_wait) = ((_tmrVal) * SS_TICKS_SEC)/((_timerRes) * 1000); \
   if((0 != (_tmrVal)) && (0 == (_wait)))                     \
   {                                                          \
      (_wait) = 1;                                            \
   }                                                          \
}

/* private function declarations */
PRIVATE Void kwBndTmrExpiry(PTR cb);

/**
 * @brief Handler to start timer
 *       
 * @param[in] gCb       Pointer to the RLC instance control block
 * @param[in] cb        Control block depending on the type of the timer event. 
 *                      It can be uplink/downlink rbCb or rgu sap control block
 * @param[in] tmrEvnt   Timer event to be started
 *
 * @return  Void
*/
#ifdef ANSI
PUBLIC Void kwStartTmr
(
KwCb  *gCb,
PTR   cb,          
S16   tmrEvnt     
)
#else
PUBLIC Void kwStartTmr (gCb,cb, tmrEvnt)
KwCb  *gCb;
PTR   cb;        
S16   tmrEvnt;  
#endif
{
/* kw005.201 added support for L2 Measurement */
#ifdef LTE_L2_MEAS
   KwL2MeasEvtCb *measEvtCb = NULLP;
#endif

   CmTmrArg arg;
   arg.wait = 0;

   TRC2(kwStartTmr)

   /* kw002.201 Adjusting the wait time as per timeRes configured by layer manager */
   switch (tmrEvnt)
   {
      case KW_EVT_UMUL_REORD_TMR:
      {
         KwUmUl* umUl = &(((KwUlRbCb *)cb)->m.umUl);
         /* kw005.201 Changed wait calculation ccpu00117634*/ 
         KW_TMR_CALCUATE_WAIT(arg.wait, umUl->reOrdTmrInt, gCb->genCfg.timeRes);

         arg.timers = &umUl->reOrdTmr;
         arg.max = KW_MAX_UM_TMR;
         break;
      }
      case KW_EVT_AMUL_REORD_TMR:
      {
         KwAmUl* amUl = &(((KwUlRbCb *)cb)->m.amUl);
         /* kw005.201 Changed wait calculation ccpu00117634*/ 
         KW_TMR_CALCUATE_WAIT(arg.wait, amUl->reOrdTmrInt, gCb->genCfg.timeRes);         

         arg.timers = &amUl->reOrdTmr;
         arg.max = KW_MAX_AM_TMR;
         break;
      }
      case KW_EVT_AMUL_STA_PROH_TMR:
      {
         KwAmUl* amUl = &(((KwUlRbCb *)cb)->m.amUl);
         /* kw005.201 Changed wait calculation ccpu00117634*/ 
         KW_TMR_CALCUATE_WAIT(arg.wait,
                              amUl->staProhTmrInt,
                              gCb->genCfg.timeRes);                  

         arg.timers = &amUl->staProhTmr;
         arg.max = KW_MAX_AM_TMR;
         break;
      } 
      case KW_EVT_AMDL_POLL_RETX_TMR:
      {
         KwAmDl* amDl = &(((KwDlRbCb *)cb)->m.amDl);
         /* kw005.201 Changed wait calculation ccpu00117634*/ 
         KW_TMR_CALCUATE_WAIT(arg.wait, 
                              amDl->pollRetxTmrInt, 
                              gCb->genCfg.timeRes);                  

         arg.timers = &amDl->pollRetxTmr;
         arg.max = KW_MAX_AM_TMR;
         break;
      } 
      case KW_EVT_WAIT_BNDCFM:
      {
         KwRguSapCb* rguSap = (KwRguSapCb *)cb;
         /* kw005.201 Changed wait calculation ccpu00117634*/ 
         KW_TMR_CALCUATE_WAIT(arg.wait, rguSap->bndTmrInt, gCb->genCfg.timeRes);                  

         arg.timers = &rguSap->bndTmr;
         arg.max = KW_MAX_RGUSAP_TMR;
         break;
      }
/* kw005.201 added support for L2 Measurement */
#ifdef LTE_L2_MEAS
      case KW_EVT_L2_TMR:
      {
         measEvtCb = (KwL2MeasEvtCb *)cb;
         /* kw005.201 Changed wait calculation ccpu00117634*/ 
         KW_TMR_CALCUATE_WAIT(arg.wait, 
                              measEvtCb->l2TmrCfg.val, 
                              gCb->genCfg.timeRes);                  

         arg.timers = &measEvtCb->l2Tmr;
         arg.max = KW_L2_MAX_TIMERS;
         break;
      }
#endif
      default:
      {
         RLOG0(L_ERROR, "Invalid tmr Evnt");
      }
   } 

   if(arg.wait != 0)
   {
      arg.tqCp   = &gCb->kwTqCp;
      arg.tq     = gCb->kwTq;
      arg.cb     = cb;
      arg.evnt   = tmrEvnt;
      arg.tNum   = 0;

      cmPlcCbTq(&arg);
   }

   RETVOID;
}

/**
 * @brief Handler to stop a timer
 *       
 * @param[in] gCb       Pointer to the RLC instance control block
 * @param[in] cb        Control block depending on the type of the timer event. 
 *                      It can be uplink/downlink rbCb or rgu sap control block
 * @param[in] tmrType   Timer event to be started
 *
 * @return  Void
*/
#ifdef ANSI
PUBLIC Void kwStopTmr
(
KwCb   *gCb,
PTR    cb,
U8     tmrType
)
#else
PUBLIC Void kwStopTmr (gCb, cb, tmrType)
KwCb   *gCb;
PTR    cb; 
U8     tmrType;
#endif
{
   CmTmrArg   arg;
/* kw005.201 added support for L2 Measurement */
#ifdef LTE_L2_MEAS
   KwL2MeasEvtCb *measEvtCb = NULLP;
#endif
   TRC2(kwStopTmr)

   arg.timers = NULLP;

   switch (tmrType)
   {
      case KW_EVT_UMUL_REORD_TMR:
      {
         arg.timers  = &((KwUlRbCb *)cb)->m.umUl.reOrdTmr;
         arg.max = KW_MAX_UM_TMR;
         break;
      }
      case KW_EVT_AMUL_REORD_TMR:
      {
         arg.timers = &((KwUlRbCb *)cb)->m.amUl.reOrdTmr;
         arg.max = KW_MAX_AM_TMR;
         break;
      }
      case KW_EVT_AMUL_STA_PROH_TMR:
      {
         arg.timers = &((KwUlRbCb *)cb)->m.amUl.staProhTmr;
         arg.max = KW_MAX_AM_TMR;
         break;
      } 
      case KW_EVT_AMDL_POLL_RETX_TMR:
      {
         arg.timers = &((KwDlRbCb *)cb)->m.amDl.pollRetxTmr;
         arg.max = KW_MAX_AM_TMR;
         break;
      } 
      case KW_EVT_WAIT_BNDCFM:
      {
         arg.timers = &((KwRguSapCb *)cb)->bndTmr;
         arg.max = KW_MAX_RGUSAP_TMR;
         break;
      }
/* kw005.201 added support for L2 Measurement */
#ifdef LTE_L2_MEAS
      case KW_EVT_L2_TMR:
      {
         measEvtCb = (KwL2MeasEvtCb *)cb;
         arg.timers   = &measEvtCb->l2Tmr;
         arg.max  = KW_L2_MAX_TIMERS;
         break;
      }
#endif
      default:
      {
         RLOG0(L_ERROR, "Invalid tmr Evnt");
      }
   } 
   if (tmrType != TMR0)
   {
      arg.tqCp   = &gCb->kwTqCp;
      arg.tq     = gCb->kwTq;
      arg.cb     = cb;
      arg.evnt   = tmrType;
      arg.wait   = 0;
      arg.tNum   = 0;
      cmRmvCbTq(&arg);
   }
   
   RETVOID;
}

/**
 * @brief Handler to invoke events on expiry of timer.
 *
 * @details
 *    This function is used to handle expiry of timer,it invokes relevant 
 *    functions.
 *       
 * @param[in] cb        Control block depending on the type of the timer event. 
 *                      It can be uplink/downlink rbCb or rgu sap control block
 * @param[in] tmrEvnt   Timer event to be started
 *
 * @return  Void
*/
#ifdef ANSI
PUBLIC Void kwTmrExpiry
(
PTR   cb,
S16   tmrEvnt 
)
#else
PUBLIC Void kwTmrExpiry (cb, tmrEvnt)
PTR   cb;
S16   tmrEvnt;
#endif
{
/* kw005.201 added support for L2 Measurement */
   TRC2(kwTmrExpiry)

   switch (tmrEvnt)
   {
      case KW_EVT_UMUL_REORD_TMR:
      {
         KwUlRbCb *ulRbCb = (KwUlRbCb *)cb;
         kwUmmReOrdTmrExp(KW_GET_KWCB(ulRbCb->inst), ulRbCb);

         break;
      }
      case KW_EVT_AMUL_REORD_TMR:
      {
         KwUlRbCb *ulRbCb = (KwUlRbCb *)cb;
         kwAmmReOrdTmrExp(KW_GET_KWCB(ulRbCb->inst), ulRbCb);
         break;
      }
      case KW_EVT_AMUL_STA_PROH_TMR:
      {
         KwUlRbCb *ulRbCb = (KwUlRbCb *)cb;
         kwAmmStaProTmrExp(KW_GET_KWCB(ulRbCb->inst), ulRbCb);

         break;
      }
      case KW_EVT_AMDL_POLL_RETX_TMR:
      {
         KwDlRbCb *dlRbCb = (KwDlRbCb *)cb;
         KwCb *gCb = KW_GET_KWCB(dlRbCb->inst);
         
         kwAmmPollRetxTmrExp(gCb, dlRbCb);

         gCb->genSts.protTimeOut++;
         break;
      }
      case KW_EVT_WAIT_BNDCFM:
      {
         kwBndTmrExpiry(cb);
         break;
      }
      /* kw005.201 L2 Measurement support */
      default:
      {
         break;
      }
   }

   RETVOID;
}

/**
 * @brief Handler to check if the timer is running
 *       
 * @param[in] gCb       Pointer to the RLC instance control block
 * @param[in] cb        Control block depending on the type of the timer event. 
 *                      It can be uplink/downlink rbCb or rgu sap control block
 * @param[in] tmrEvnt   Timer event to be started
 *
 * @return  Bool indicating whether the timer is running or not
 *      -# ROK 
 *      -# RFAILED 
*/
#ifdef ANSI
PUBLIC Bool kwChkTmr
(
KwCb   *gCb,
PTR    cb,
S16    tmrEvnt
)
#else
PUBLIC Bool kwChkTmr(gCb,cb, tmrEvnt)
KwCb   *gCb;
PTR    cb;
S16    tmrEvnt;
#endif
{
   TRC2(kwChkTmr)

   switch (tmrEvnt)
   {
      case KW_EVT_UMUL_REORD_TMR:
      {
         return (((KwUlRbCb *)cb)->m.umUl.reOrdTmr.tmrEvnt == 
                  KW_EVT_UMUL_REORD_TMR);
      }
      case KW_EVT_AMUL_REORD_TMR:
      {
         return (((KwUlRbCb *)cb)->m.amUl.reOrdTmr.tmrEvnt == 
                  KW_EVT_AMUL_REORD_TMR);
      }
      case KW_EVT_AMUL_STA_PROH_TMR:
      {
         return (((KwUlRbCb *)cb)->m.amUl.staProhTmr.tmrEvnt == 
                  KW_EVT_AMUL_STA_PROH_TMR);
      } 
      case KW_EVT_AMDL_POLL_RETX_TMR:
      {
         return (((KwDlRbCb *)cb)->m.amDl.pollRetxTmr.tmrEvnt == 
                  KW_EVT_AMDL_POLL_RETX_TMR);
      } 
      case KW_EVT_WAIT_BNDCFM:
      {
         return (((KwRguSapCb *)cb)->bndTmr.tmrEvnt == KW_EVT_WAIT_BNDCFM);
      }
      default:
      {
         RLOG0(L_ERROR, "Invalid tmr Evnt");
      }
   } 

   RETVALUE(FALSE);
}

/**
 * @brief Handler to do processing on expiry of the bind timer
 *
 * @details
 *    This function processes the RLC bind timer expiry. If the number of 
 *    retries is less than the maximum retry counter, bind request is sent 
 *    again, else an alarm is raised to the layer manager.
 *       
 * @param[in] cb  Pointer to the Rgu sap
 *
 * @return  Void
*/
#ifdef ANSI
PRIVATE Void kwBndTmrExpiry
(
PTR cb
)
#else
PRIVATE Void kwBndTmrExpiry(cb)
PTR cb;
#endif
{
   KwRguSapCb *rguSapCb; 

   TRC2(kwBndTmrExpiry)

   rguSapCb = (KwRguSapCb *) cb;

   if (rguSapCb->state == KW_SAP_BINDING)
   {
      if (rguSapCb->retryCnt < KW_MAX_SAP_BND_RETRY)
      {
         /* start timer to wait for bind confirm */
         kwStartTmr(KW_GET_KWCB(rguSapCb->pst.srcInst),
                    (PTR)rguSapCb, 
                    KW_EVT_WAIT_BNDCFM);
         
         /* Send bind request */
         rguSapCb->retryCnt++;
         KwLiRguBndReq (&rguSapCb->pst, rguSapCb->suId, rguSapCb->spId);
      }
      else
      {
         rguSapCb->retryCnt = 0;
         rguSapCb->state = KW_SAP_CFG;

         /* Send alarm to the layer manager */
#ifdef LTE_L2_MEAS
         kwLmmSendAlarm(KW_GET_KWCB(rguSapCb->pst.srcInst),
                        LCM_CATEGORY_INTERFACE, 
                        LCM_EVENT_BND_FAIL,
                        LCM_CAUSE_TMR_EXPIRED, 
                        0, 
                        0, 
                        0);
#else
         kwLmmSendAlarm(KW_GET_KWCB(rguSapCb->pst.srcInst),
                        LCM_CATEGORY_INTERFACE, 
                        LCM_EVENT_BND_FAIL,
                        LCM_CAUSE_TMR_EXPIRED, 
                        0, /* suId */
                        0 /* ueId */);
#endif
      }
   }

   RETVOID;
}


  
/********************************************************************30**
  
         End of file:     gp_tmr.c@@/main/TeNB_Main_BR/tenb_main_ccb/tenb_5.0_RIB/tenb_5.0_RIB_GA/6 - Mon Feb 15 12:52:41 2016
  
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
/main/1      ---      vp   1. initial release.
*********************************************************************91*/
