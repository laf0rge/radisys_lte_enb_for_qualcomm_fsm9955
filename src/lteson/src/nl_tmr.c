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

     Name:     SON Application

     Type:     

     Desc:     This file contains 
               

     File:     nl_tmr.c

     Sid:      bk_tmr.c@@/main/tenb_main_ccb/1 - Thu Jan 21 17:51:24 2016

     Prg:      subhamay 

*********************************************************************21*/

static const char* RLOG_MODULE_NAME="SON";
static int RLOG_FILE_ID=255;
static int RLOG_MODULE_ID=256;


/* Header include files (.h) */
#include "envopt.h"        /* Environment options */
#include "envdep.h"        /* Environment dependent */
#include "envind.h"        /* Environment independent */
#include "gen.h"           /* General */
#include "ssi.h"           /* System services */
#include "cm5.h"

#include "cm_tkns.h"       /* Common Token defines */
#include "cm_mblk.h"       /* Common Mem Allocation defines */
#include "cm_llist.h"      /* Common Link List defines */
#include "cm_hash.h"       /* Common Hashlist defines */
#include "cm_pasn.h"       /* Common Pasn defines */
#include "cm_lte.h"        /* Common Pasn defines */
#include "rl_common.h"     /* RLOG defines */
#include "rl_interface.h"
#include "lnl.h"           /* NL Interface defines */
#include "nlu.h"
#include "nl.h"            /* SON defines */

/* Header/extern include files (.x) */
#include "gen.x"           /* General */
#include "ssi.x"           /* System services */
#include "cm5.x"
#include "cm_tkns.x"       /* Common Token definitions */
#include "cm_mblk.x"       /* Common Memory Allocation */
#include "cm_llist.x"      /* Common Link List definitions */
#include "cm_hash.x"       /* Common Hashlist definitions */
#include "cm_lib.x"        /* Common Library definitions */
#include "cm_pasn.x"       /* Common Hashlist definitions */
#include "cm_lte.x"       /* Common Pasn defines */

#include "lnl.x"           /* NL Interface includes */
#include "nlu.x"
#include "nl.x"            /* SON includes */

/* local defines */
/* local typedefs */
 
/* local externs */
 
/* forward references */

/** @brief This function handles timer expiry. 
 *
 * @details
 *
 *  This function based upon the timer event invokes specific 
 *  routine to handle timer expiry. 
 *
 * @param[in] Cb       : This holds approriate Control block for 
 *                        the timer event passed. 
 * @param[in] tmrEvent : One of the many possible timer types. 
 * @return S16
 *        -# Success : ROK
 *        -# Failure : RFAILED
 */
#ifdef ANSI
PUBLIC Void nlProcTmr
(
Ptr    cb,
S16    event
)
#else
PUBLIC Void nlProcTmr(cb, event)
(
Ptr    cb;
S16    event;
)
#endif
{
   RETVOID;
}

/** @brief This function is used to start a given timer. 
 *
 * @details
 *
 *   This function based upon the timer event, derives the TimerCB 
 *   and adds it to the timer queue.
 *
 * @param[in] Cb       : This holds approriate Control block 
 *                       for the timer event passed. 
 * @param[in] tmrEvent : One of the many possible timer types. 
 * @param[in] delay    : Duration of the timer in second.
 * @return Void
 */
#ifdef ANSI
PUBLIC Void nlStartTmr
(
PTR   cb,
S16   tmrEvnt,
U32   delay
)
#else
PUBLIC Void nlStartTmr(cb, tmrEvnt, delay)
(
PTR   cb;
S16   tmrEvnt;
U32   delay;
)
#endif
{
   delay = NL_CALC_WAIT_TIME(delay);
   RETVOID;
}

/** @brief This function is used to stop a previously running timer. 
 *
 * @details
 *
 *  This function based upon the timer event reterives relevant timerCb and 
 *  tries to determine whether timer is running or not.In case timer 
 *  was running, it is stopped .
 *
 * @param[in] Cb : This holds approriate Control block for the timer event passed. 
 * @param[in] tmrEvent : One of the many possible timer types. 
 * @return Void
 */
#ifdef ANSI
PUBLIC Void nlStopTmr
(
PTR   cb,
S16   event
)
#else
PUBLIC Void nlStopTmr(cb, event)
(
PTR   cb;
S16   event;
)
#endif
{
   RETVOID;
}



/** @brief Invoked by system services to activate a task with
 *              a timer tick.
 *
 * @details 
 *         activate the timer tick
 *
 *
 * @param[in] ent    : Entity Id od SON module
 * @param[in] inst   : Instance Id od SON module
 
 * @return
 -#                  : ROK 
 */

#ifdef ANSI
PUBLIC S16 nlActvTmr
(
Ent    ent,
Inst   inst
)
#else
PUBLIC S16 nlActvTmr(ent, inst)
(
Ent    ent;
Inst   inst;
)
#endif
{
   if(&nlCb.tqCp && nlCb.tq)
   {
      cmPrcTmr(&nlCb.tqCp, nlCb.tq, (PFV) nlProcTmr);
   }

   RETVALUE(ROK);
} 

/**
 * @brief This function registers and initializes the timer
 *         queue structures in SON during general config. 
 *
 * @details
 *
 *     This function will register the timer module for SON
 *
 *  @param[in]   Void
 *
 *  @return  S16
 *     -# Timer module registration success  : ROK
 *     -# Timer module registration failed   : RFAILED
 *
 **/
#ifdef ANSI
PUBLIC S16 nlRegInitTmr
(
Void
)
#else
PUBLIC S16 nlRegInitTmr()
(
Void;
)
#endif
{
   U16                       idx;
   S16                       ret;

   ret = SRegTmrMt((Ent)ENTNL, (Inst)0, nlCb.timeRes, nlActvTmr);
   if (ret != ROK)
   {
      RLOG0(L_ERROR, "SRegTmr Failed");
      RETVALUE(RFAILED);
   }

   /* initialize timing queues */
   nlCb.tqCp.tmrLen      = NL_NUM_TQENTRY;

   for (idx = 0; idx < NL_NUM_TQENTRY; idx++)
   {
      nlCb.tq[idx].first    = NULLP;
      nlCb.tq[idx].tail     = NULLP;
   }

   RETVALUE(ROK);
}

/**
 * @brief Stop BRCM-SON Timers
 *
 * @details
 *
 *     This function will stop all the timers started for BRCM-SON
 *
 *  @param[in]  Void
 *  @return  Void
 *
 **/
#ifdef ANSI
PUBLIC Void nlStopTimers
(
Void
)
#else
PUBLIC Void nlStopTimers(Void)
(
Void
)
#endif
{
}

/********************************************************************30**

           End of file:     bk_tmr.c@@/main/tenb_main_ccb/1 - Thu Jan 21 17:51:24 2016

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

