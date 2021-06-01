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
  
     File:     rg_tmr.c 
  
     Sid:      gk_tmr.c@@/main/3 - Sat Jul 30 02:21:54 2011
  
     Prg:      sd 
  
**********************************************************************/

/** @file rg_tmr.c 
@brief This module does processing related to timers
*/
/* header include files -- defines (.h) */
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
#include "cm_lte.h"       /* common tokens */
#include "rgu.h"           /* RGU defines */
#include "tfu.h"           /* RGU defines */
#include "lrg.h"           /* layer management defines for LTE-MAC */
#include "crg.h"           /* layer management defines for LTE-MAC */
#include "rg_sch_inf.h"           /* layer management defines for LTE-MAC */
#include "rg.h"            /* defines and macros for MAC */
#include "rg_env.h"            /* defines and macros for MAC */
#include "rg_err.h"            /* defines and macros for MAC */


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
#include "rgu.x"           /* RGU types */
#include "tfu.x"           /* RGU types */
#include "lrg.x"           /* layer management typedefs for MAC */
#include "crg.x"           /* layer management typedefs for MAC */
#include "rg_sch_inf.x"           /* layer management typedefs for MAC */
#include "rg.x"            /* typedefs for MAC */

 /** @brief This function is a utility function to start timers, it is a
  * generic function.
  *
  * @details
  *
  *     Function: rgTMRStartTmr
  *
  *         Processing steps:
  *         - Starts timer at MAC 
  *
  *   @param[in]  RgCellCb       *cell
  *   @param[in]  Ptr            cb
  *   @param[in]  S16            tmrEvnt
  *   @param[in]  U32            tmrVal
  *   @return  Void
  */
#ifdef ANSI
PUBLIC Void rgTMRStartTmr
(
 RgCellCb   *cellCb,
 Ptr         cb,
 S16         tmrEvnt,
 U32         tmrVal
 )
#else
PUBLIC Void rgTMRStartTmr (cellCb, cb, tmrEvnt, tmrVal)
 RgCellCb   *cellCb;
 Ptr        cb;
 S16         tmrEvnt;
 U32        tmrVal;
#endif
{
   CmTmrArg    arg;
   RgUeCb      *ueCb;

   TRC2(rgTMRStartTmr);

   ueCb = (RgUeCb*)cb;
   RLOG_ARG2(L_INFO,DBG_CELLID,cellCb->cellId,"starting timer for TmrEvnt:%d",
                                              "ueId:%d",
                                              tmrEvnt,
                                              ueCb->ueId);

   arg.tqCp = &(cellCb->tqCp);
   arg.tq   = cellCb->tq;
   arg.cb   = (PTR)cb;
   arg.evnt = tmrEvnt;
   arg.wait = tmrVal;
   arg.max  = 1;
   arg.timers = &(ueCb->taTmr);
   arg.tNum = NOTUSED;
   cmPlcCbTq(&arg);

   RETVOID;
} /* end of */ 

 /** @brief This function stops the timer.
  *
  * @details
  *
  *     Function: rgTMRStopTmr 
  *
  *         Processing steps:
  *         - Stops timer
  *
  *   @param[in]  RgCellCb       *cell
  *   @param[in]  S16            tmrEvnt
  *   @param[in]  Ptr            cb
  *   @return  Void
  */
#ifdef ANSI
PUBLIC Void rgTMRStopTmr
(
 RgCellCb   *cellCb,
 S16         tmrEvnt,
 Ptr        cb
 )
#else
PUBLIC Void rgTMRStopTmr (cellCb, tmrEvnt, cb)
 RgCellCb   *cellCb;
 S16         tmrEvnt;
 Ptr        cb;
#endif
{
   CmTmrArg    arg;
   RgUeCb      *ueCb;

   TRC2(rgTMRStopTmr);
   
   ueCb = (RgUeCb*)cb;

   RLOG_ARG2(L_INFO,DBG_CELLID,cellCb->cellId,"stoping timer for TmrEvnt:%d"
                                              "ueId:%d",
                                              tmrEvnt,
                                              ueCb->ueId);

   arg.tqCp = &(cellCb->tqCp);
   arg.tq   = cellCb->tq;
   arg.cb   = (PTR)cb;
   arg.evnt = tmrEvnt;
   arg.wait = NOTUSED;
   arg.max  = 0;
   arg.timers = &(ueCb->taTmr);
   arg.tNum = NOTUSED;
   cmRmvCbTq(&arg);
   RETVOID;
} /* end of */ 

 /** @brief This function handles timer expiry.
  *
  * @details
  *
  *     Function: rgTMRProcTmr 
  *
  *         Processing steps:
  *         - Handles timer processing on timer expiry
  *
  *   @param[in]  Ptr            cb
  *   @param[in]  S16            tmrEvnt
  *   @return  Void
  */
#ifdef ANSI
PUBLIC Void rgTMRProcTmr 
(
Ptr         cb,
S16          tmrEvnt
)
#else
PUBLIC Void rgTMRProcTmr (cb, tmrEvnt)
Ptr         cb;
S16          tmrEvnt;
#endif
{
   RgUeCb      *ueCb;

   TRC2(rgTMRProcTmr);

   ueCb = (RgUeCb*)cb;
   /* call DHM to handle timer Expiry */
   rgDHMProcTAExp (ueCb);
   RETVOID;
} /* end of */ 


/**********************************************************************
 
         End of file:     gk_tmr.c@@/main/3 - Sat Jul 30 02:21:54 2011
 
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
/main/1      ---     sd   1. Initial Release.
/main/2      ---     sd   1. LTE MAC 2.1 release
          rg003.201  ns   1. Trace added to functions.
/main/3      ---        gvj  1. Updated for LTE MAC Rel 3.1           
$SID$        ---     rt    1. LTE MAC 4.1 release
*********************************************************************91*/
