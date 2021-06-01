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
 
     Desc:     C source code for power control functionality
 
     File:     rg_sch_pwr.c
 
     Sid:      gk_sch_pwr.c@@/main/2 - Sat Jul 30 02:21:47 2011
 
     Prg:     sd
 
**********************************************************************/

/** @file rg_sch_pwr.c
@brief This module handles schedulers' power control functionality
*/

static const char* RLOG_MODULE_NAME="MAC";
static int RLOG_MODULE_ID=4096;
static int RLOG_FILE_ID=188;
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
#include "cm_math.h"       /* common MATH functions */
#include "lrg.h"
#include "rgr.h"
#include "rgm.h"
#include "tfu.h"
#include "rg_env.h"
#include "rg_sch_inf.h"
#include "rg_sch_err.h"
#include "rg_sch.h"
#include "rg_sch_cmn.h"

/* header/extern include files (.x) */
#include "gen.x"           /* general layer typedefs */
#include "ssi.x"           /* system services typedefs */
#include "cm5.x"           /* common timers */
#include "cm_hash.x"       /* common hash list */
#include "cm_lib.x"        /* common library */
#include "cm_llist.x"      /* common linked list */
#include "cm_mblk.x"       /* memory management */
#include "cm_tkns.x"       /* common tokens */
#include "cm_lte.x"        /* common tokens */
#include "cm_math.x"       /* common MATH functions */
#include "tfu.x"           /* RGU types */
#include "lrg.x"           /* layer management typedefs for MAC */
#include "rgr.x"           /* layer management typedefs for MAC */
#include "rgm.x"           /* layer management typedefs for MAC */
#include "rg_sch_inf.x"    /* typedefs for Scheduler */
#include "rg_sch.x"        /* typedefs for Scheduler */
#include "rg_sch_cmn.x"
#include "rl_interface.h"
#include "rl_common.h"


/* Current specs have 23 dBm as max tx power capability for UEs */
#define RG_SCH_PWR_UE_MAX_PWR    23


#define RG_SCH_PWR_GETUEPWR(ue) &((RgSchCmnUe *)((ue)->sch))->ul.ulPwrCb
#define RG_SCH_PWR_GETCELLPWR(cell) &((RgSchCmnCell *)((cell)->sc.sch))->ul.ulPwrCb


typedef S8 RgSchCmnUlPwrCqiToPwrTbl[RG_SCH_CMN_UL_NUM_CQI];

PRIVATE RgSchCmnUlPwrCqiToPwrTbl rgSchPwrCqiToPwrTbl;

/* This table maps a given number of RBs (given by array index)
 * to the power in dB that these many RBs map to. */
CONSTANT U8 rgSchPwrRbToPwrTbl[111] = { 0,    /* First entry is dummy */
   0,  3,  4,  6,  7,  7,  8,  9,  9,  10,
   10, 10, 11, 11, 11, 12, 12, 12, 12, 13,
   13, 13, 13, 13, 14, 14, 14, 14, 14, 14,
   15, 15, 15, 15, 15, 15, 15, 15, 16, 16,
   16, 16, 16, 16, 16, 16, 16, 16, 17, 17,
   17, 17, 17, 17, 17, 17, 17, 17, 17, 17,
   17, 18, 18, 18, 18, 18, 18, 18, 18, 18,
   18, 18, 18, 18, 18, 18, 18, 19, 19, 19,
   19, 19, 19, 19, 19, 19, 19, 19, 19, 19,
   19, 19, 19, 19, 19, 19, 19, 20, 20, 20,
   20, 20, 20, 20, 20, 20, 20, 20, 20, 20
};


/* This table maps power (in dB) to number of RBs */
/* The array size comes from max power in rgSchPwrRbToPwrTbl */
CONSTANT U8 rgSchPwrToRbTbl[20+1] = {
  1, 1, 2, 2, 3, 4, 5, 6, 7, 9, 11,
  13, 17, 21, 26, 33, 41, 52, 65, 82, 103 
};



PRIVATE S8 rgSCHPwrGetCqiPwr ARGS((
         U8                  cqi
         ));
PRIVATE S8 rgSCHPwrGetCqiPwrForUe ARGS((
         RgSchUeCb          *ue,
         U8                  cqi
         ));
PRIVATE S8 rgSCHPwrCalcEfficncyPwr ARGS((
         U32                 eff
         ));
PRIVATE S8 rgSCHPwrGetDelta2FrmCqi ARGS((
         U8                  crntCqi,
         U8                  trgCqi,
         RgSchUeCb           *ue,
         RgSchCellCb         *cell
         ));
PRIVATE Void rgSCHPwrGetPuschTpc ARGS((
         U8                  isAcc,
         S8                  delta,
         S8                  availPwr,
         U8                 *tpc,
         S8                 *tpcDelta
         ));
PRIVATE U8 rgSCHPwrGetMaxRb ARGS((
         RgSchCellCb          *cell,
         S8                  pwr
         ));
PRIVATE U8 rgSCHPwrRbToPwr ARGS((
         RgSchCellCb          *cell,
         U8                  numRb
         ));
PRIVATE Void rgSCHPwrSchedPucchRnti ARGS((
         RgSchCellCb          *cell,
         RgSchCmnTpcRntiCb    *cb,
         RgSchPdcch           *pdcch,
         RgSchDlSf            *dlSf,
         Bool                 *sched
         ));
PRIVATE Void rgSCHPwrPuschCntrl ARGS((
         RgSchCellCb          *cell,
         RgSchUeCb            *ue
         ));
PRIVATE Void rgSCHPwrPucchCntrl ARGS((
         RgSchCellCb *cell,
         RgSchUeCb   *ue
         ));
PRIVATE Void rgSCHPwrSchedPuschRnti ARGS((
         RgSchCellCb          *cell,
         RgSchCmnTpcRntiCb    *cb,
         RgSchPdcch           *pdcch,
         RgSchUlSf            *ulSf,
         Bool                 *sched
         ));
PRIVATE Void rgSCHPwrGetPucchFmt3TpcForUe ARGS((
         RgSchUeCb            *ue,
         U8                   *tpc,
         S8                   *delta
         ));
PRIVATE Void rgSCHPwrGetPucchFmt3aTpcForUe ARGS((
         RgSchUeCb            *ue,
         U8                   *tpc,
         S8                   *delta
         ));
PRIVATE Void rgSCHPwrGetPuschFmt3TpcForUe ARGS((
         RgSchUeCb            *ue,
         U8                   *tpc,
         S8                   *delta
         ));
PRIVATE Void rgSCHPwrGetPuschFmt3aTpcForUe ARGS((
         RgSchUeCb            *ue,
         U8                   *tpc,
         S8                   *delta
         ));
PRIVATE Void rgSCHPwrGetAcc1bitTpc ARGS((
         S8                    remPwr,
         U8                   *tpc,
         S8                   *delta
         ));
PRIVATE Void rgSCHPwrGetAcc2bitTpc ARGS((
         S8                   remPwr,
         U8                  *tpc,
         S8                  *delta
         ));
PRIVATE Void rgSCHPwrGetAbsTpc ARGS((
         S8                   remPwr,
         U8                  *tpc,
         S8                  *delta
         ));
PRIVATE Void rgSCHPwrOnPucchGrpPwrForUe  ARGS((
         RgSchCellCb          *cell,
         RgSchUeCb            *ue,
         S8                    delta
         ));
PRIVATE Void rgSCHPwrOnPuschGrpPwrForUe  ARGS((
         RgSchCellCb          *cell,
         RgSchUeCb            *ue,
         S8                    delta
         ));
PRIVATE Bool rgSCHPwrIsDlUeSched ARGS((
         RgSchUeCb            *ue,
         RgSchDlSf            *sf
         ));
PRIVATE Bool rgSCHPwrIsUlUeSched ARGS((
         RgSchCellCb          *cell,
         RgSchUeCb            *ue,
         RgSchUlSf            *sf
         ));
PRIVATE Void rgSCHPwrOnSchedPucchTpc ARGS((
         RgSchCellCb           *cell,
         RgSchUeCb             *ue,
         S8                     delta
         ));
PRIVATE Void rgSCHPwrOnSchedPuschTpc ARGS((
         RgSchCellCb           *cell,
         RgSchUeCb             *ue
         ));
PRIVATE S16 rgSCHPwrApplyUePwrCfg  ARGS((
         RgSchCellCb          *cell,
         RgSchUeCb            *ue,
         RgrUeUlPwrCfg        *pwrCfg
         ));
PRIVATE Void rgSCHPwrUeResetPucch ARGS((
         RgSchCellCb          *cell,
         RgSchUeCb            *ue
         ));
PRIVATE Void rgSCHPwrUeResetPusch ARGS((
         RgSchCellCb          *cell,
         RgSchUeCb            *ue
         ));
PRIVATE Void rgSCHPwrOnPuschPwrUpd ARGS((
         RgSchCellCb          *cell,
         RgSchUeCb            *ue
         ));
PRIVATE Void rgSCHPwrAddRntiToPucchRntiLst  ARGS((
         RgSchCellCb          *cell,
         CmLteRnti             rnti,
         Bool                  isFmt3a
         ));
PRIVATE Void rgSCHPwrAddRntiToPuschRntiLst  ARGS((
         RgSchCellCb          *cell,
         CmLteRnti             rnti,
         Bool                  isFmt3a
         ));
PRIVATE Void rgSCHPwrInitTpcRntiCb  ARGS((
         RgSchCmnTpcRntiCb    *cb,
         CmLteRnti             rnti,
         Bool                  isFmt3a
         ));
PRIVATE RgSchCmnTpcRntiCb* rgSCHPwrGetPucchRntiCb ARGS((
         RgSchCellCb *cell,
         CmLteRnti   tpcRnti
         ));
PRIVATE RgSchCmnTpcRntiCb* rgSCHPwrGetPuschRntiCb ARGS((
         RgSchCellCb *cell,
         CmLteRnti   tpcRnti
         ));
PRIVATE Void rgSCHPwrAddUeToPucchTpcRntiCb ARGS((
         RgSchCellCb           *cell,
         RgSchCmnTpcRntiCb     *cb,
         RgSchUeCb             *ue
         ));
PRIVATE Void rgSCHPwrDelUeFrmPucchTpcRntiCb ARGS((
         RgSchCellCb           *cell,
         RgSchCmnTpcRntiCb     *cb,
         RgSchUeCb             *ue
         ));
PRIVATE Void rgSCHPwrRmvSchdUeFrmPucchTpcRntiCb ARGS((
         RgSchCellCb           *cell,
         RgSchCmnTpcRntiCb     *cb,
         RgSchUeCb             *ue
         ));
PRIVATE Void rgSCHPwrRmvSchdUeOnlyFrmPucchTpcRntiCb ARGS((
         RgSchCellCb           *cell,
         RgSchCmnTpcRntiCb     *cb,
         RgSchUeCb             *ue
         ));
PRIVATE Void rgSCHPwrRmvSchdPucchTpcRntiCb ARGS((
         RgSchCellCb           *cell,
         RgSchCmnTpcRntiCb     *cb
         ));
PRIVATE Void rgSCHPwrAddSchdUeToPucchTpcRntiCb ARGS((
         RgSchCellCb           *cell,
         RgSchCmnTpcRntiCb     *cb,
         RgSchUeCb             *ue
         ));
PRIVATE Void rgSCHPwrAddSchdPucchTpcRntiCb ARGS((
         RgSchCellCb           *cell,
         RgSchCmnTpcRntiCb     *cb
         ));
PRIVATE Void rgSCHPwrAddUeToPuschTpcRntiCb ARGS((
         RgSchCmnTpcRntiCb     *cb,
         RgSchUeCb             *ue
         ));
PRIVATE Void rgSCHPwrAddSchdUeToPuschTpcRntiCb ARGS((
         RgSchCellCb           *cell,
         RgSchCmnTpcRntiCb     *cb,
         RgSchUeCb             *ue
         ));
PRIVATE Void rgSCHPwrDelUeFrmPuschTpcRntiCb ARGS((
         RgSchCellCb           *cell,
         RgSchCmnTpcRntiCb     *cb,
         RgSchUeCb             *ue
         ));
PRIVATE Void rgSCHPwrRmvSchdUeFrmPuschTpcRntiCb ARGS((
         RgSchCellCb           *cell,
         RgSchCmnTpcRntiCb     *cb,
         RgSchUeCb             *ue
         ));
PRIVATE Void rgSCHPwrRmvSchdUeOnlyFrmPuschTpcRntiCb ARGS((
         RgSchCellCb           *cell,
         RgSchCmnTpcRntiCb     *cb,
         RgSchUeCb             *ue
         ));
PRIVATE Void rgSCHPwrAddSchdPuschTpcRntiCb ARGS((
         RgSchCellCb           *cell,
         RgSchCmnTpcRntiCb     *cb
         ));
PRIVATE Void rgSCHPwrRmvSchdPuschTpcRntiCb ARGS((
         RgSchCellCb           *cell,
         RgSchCmnTpcRntiCb     *cb
         ));
PRIVATE S16 rgSCHPwrChkPucchTpcRntiIdx ARGS((
         RgSchCmnTpcRntiCb     *cb,
         U8                     idx
         ));
PRIVATE S16 rgSCHPwrChkPuschTpcRntiIdx ARGS((
         RgSchCmnTpcRntiCb     *cb,
         U8                     idx
         ));
PRIVATE S16 rgSCHPwrChkUniqPucchTpcRntiIdx ARGS((
         RgSchCmnTpcRntiCb     *cb,
         U8                     idx
         ));
PRIVATE S16 rgSCHPwrChkUniqPuschTpcRntiIdx ARGS((
         RgSchCmnTpcRntiCb     *cb,
         U8                     idx
         ));
PRIVATE S16 rgSCHPwrChkTpcRntiIdx ARGS((
         RgSchCmnTpcRntiCb     *cb,
         U8                     idx
         ));
PRIVATE S8 rgSCHPwrGetPhValFromPhr ARGS((
         U8                    phr
         ));


/* local defines */


/**
 * @brief Does power related initialisation (not cell specific).
 *        
 *
 * @details
 *
 *     Function : rgSCHPwrInit
 *
 *     Processing Steps:
 *      - This shall precompute coding efficiency to power
 *        mappings (assuming beta of 1).
 *
 *  @return  Void
 **/
#ifdef ANSI
PUBLIC Void rgSCHPwrInit
(
Void
)
#else
PUBLIC Void rgSCHPwrInit()
#endif
{
   U8             idx;
   TRC2(rgSCHPwrInit);

   rgSchPwrCqiToPwrTbl[0] = 0;  /* This should never be used anyway */
   for (idx = 1; idx < RG_SCH_CMN_UL_NUM_CQI; ++idx)
   {
      rgSchPwrCqiToPwrTbl[idx] = rgSCHPwrCalcEfficncyPwr(rgSchCmnUlCqiTbl[idx].eff);
   }
   RETVOID;
}

/***********************************************************
 *
 *     Func : rgSCHPwrGetCqiPwr
 *
 *     Desc : Returns power corresponding to coding efficiency
 *            when beta pusch is assumed 1.
 *
 *     Ret  : U8
 *
 *     Notes:
 *
 *     File :
 *
 **********************************************************/
#ifdef ANSI
PRIVATE S8 rgSCHPwrGetCqiPwr
(
U8                  cqi
)
#else
PRIVATE S8 rgSCHPwrGetCqiPwr(cqi)
U8                  cqi;
#endif
{
   TRC2(rgSCHPwrGetCqiPwr);

   RETVALUE(rgSchPwrCqiToPwrTbl[cqi]);
}  /* rgSCHPwrGetCqiPwr */

/***********************************************************
 *
 *     Func : rgSCHPwrGetCqiPwrForUe
 *
 *     Desc : If MCS control is enabled for UE, returns
 *            power corresponding to CQI, else 0.
 *
 *     Ret  : U8
 *
 *     Notes:
 *
 *     File :
 *
 **********************************************************/
#ifdef ANSI
PRIVATE S8 rgSCHPwrGetCqiPwrForUe
(
RgSchUeCb          *ue,
U8                  cqi
)
#else
PRIVATE S8 rgSCHPwrGetCqiPwrForUe(ue, cqi)
RgSchUeCb          *ue;
U8                  cqi;
#endif
{
   RgSchCmnUeUlPwrCb     *uePwr = RG_SCH_PWR_GETUEPWR(ue);
   TRC2(rgSCHPwrGetCqiPwrForUe);

   if (!uePwr->deltaMcsEnbld)
   {
      RETVALUE(0);
   }
   RETVALUE(rgSCHPwrGetCqiPwr(cqi));
}  /* rgSCHPwrGetCqiPwrForUe */

/***********************************************************
 *
 *     Func : rgSCHPwrCalcEfficncyPwr
 *
 *     Desc : Computes power corresponding to a coding
 *            efficiency.
 *
 *     Ret  : U8
 *
 *     Notes: Assumes beta pusch to be 1
 *
 *     File :
 *
 **********************************************************/
#ifdef ANSI
PRIVATE S8 rgSCHPwrCalcEfficncyPwr
(
U32                 eff
)
#else
PRIVATE S8 rgSCHPwrCalcEfficncyPwr(eff)
U32                 eff;
#endif
{
   F64          ks = 1.25; /* or F64 */
   F64          tmp = cmPow(2, ks*eff/1024) - 1;
   TRC2(rgSCHPwrCalcEfficncyPwr);

   if (tmp <= 0)
      RETVALUE(0);
   RETVALUE((S8)(10 * cmLog10(tmp)));
}  /* rgSCHPwrCalcEfficncyPwr */


/**
 * @brief Returns TPC to be sent in UL allocation
 *
 * @details
 *
 *     Function : rgSCHPwrPuschTpcForUe
 *
 *     Invoking Module Processing:
 *      - After allocation for UE, this function shall
 *        be invoked to retrieve TPC.
 *      - This assumes that rgSCHPwrGetMaxUlRb() was
 *        invoked prior to final allocation for UE.
 *        
 *     Processing Steps:
 *     - Just return TPC that was determined
 *       earlier.
 *     - After this, do necessary updates.
 *
 *  @param[in]  RgSchCellCb  *cell
 *  @param[in]  RgSchUeCb    *ue
 *  @return  U8
 **/
#ifdef ANSI
PUBLIC U8 rgSCHPwrPuschTpcForUe
(
RgSchCellCb *cell,
RgSchUeCb   *ue
)
#else
PUBLIC U8 rgSCHPwrPuschTpcForUe(cell, ue)
RgSchCellCb *cell;
RgSchUeCb   *ue;
#endif
{
   RgSchCmnUeUlPwrCb     *uePwr = RG_SCH_PWR_GETUEPWR(ue);

   UNUSED(cell);
   TRC2(rgSCHPwrPuschTpcForUe);

   rgSCHPwrOnSchedPuschTpc(cell, ue);
   RETVALUE(uePwr->puschTpc);
}

/**
 * @brief Handles Pusch power control for DCI format 0
 *
 * @details
 *
 *     Function : rgSCHPwrGetMaxUlRb
 *
 *     Invoking Module Processing:
 *      - This shall be invoked to determine maximum
 *        number of UL RBs for scheduling.
 *      - This is expected to be invoked every time
 *        priority to attempt at UE allocation. Later
 *        TPC retrieval depends on it.
 *
 *     Processing Steps:
 *     - Returns maximum allowed UL RBs to be granted
 *       after invoking Pusch power control.
 *
 *  @param[in]  RgSchCellCb  *cell
 *  @param[in]  RgSchUeCb    *ue
 *  @return  Void
 **/
#ifdef ANSI
PUBLIC U8 rgSCHPwrGetMaxUlRb
(
RgSchCellCb *cell,
RgSchUeCb   *ue
)
#else
PUBLIC U8 rgSCHPwrGetMaxUlRb(cell, ue)
RgSchCellCb *cell;
RgSchUeCb   *ue;
#endif
{
   RgSchCmnUeUlPwrCb     *uePwr = RG_SCH_PWR_GETUEPWR(ue);
   TRC2(rgSCHPwrGetMaxUlRb);

   rgSCHPwrPuschCntrl(cell, ue); /* This stores tpc, delta and maxRb
                                  * in uePwr */
   RETVALUE(uePwr->maxUlRbs);
}

/**
 * @brief Handles Pusch power control for DCI format 0
 *
 * @details
 *
 *     Function : rgSCHPwrPuschCntrl
 *
 *     Invoking Module Processing:
 *      - This shall be invoked to determine TPC
 *      and maximum number of UL RBs for scheduling
 *      (through DCI format 0).
 *
 *     Processing Steps:
 *     - 'remPuschPwr' is the final delta power that the UE
 *       should apply to get to target CQI.
 *     - The available headroom (availPwr) is determined.
 *     - Power command is given by considering remPuschPwr and
 *       availPwr.
 *     - After factoring in the power command into availPwr, the
 *       maximum number of RBs that can be supported is determined
 *       assuming that UE is going to use transmission efficiency
 *       corresponding to current CQI.
 *     - The results determined in this function are stored
 *       in the UE power control block.
 *     - [Not doing anything of power control of msg3
 *        retransmissions now]
 *
 *  @param[in]  RgSchCellCb  *cell
 *  @param[in]  RgSchUeCb    *ue
 *  @return  Void
 **/
#ifdef ANSI
PRIVATE Void rgSCHPwrPuschCntrl
(
RgSchCellCb *cell,
RgSchUeCb   *ue
)
#else
PRIVATE Void rgSCHPwrPuschCntrl(cell, ue)
RgSchCellCb *cell;
RgSchUeCb   *ue;
#endif
{
   RgSchCmnUlUe       *ueUl    = RG_SCH_CMN_GET_UL_UE(ue);
   RgSchCmnUeUlPwrCb  *uePwr   = RG_SCH_PWR_GETUEPWR(ue);
   RgSchCmnUlCell *cellUl      = RG_SCH_CMN_GET_UL_CELL(cell);
   S8                  delta;
#ifdef TFU_UPGRADE
   U8                  cqi     = ueUl->validUlCqi;
   S32                 tmp;
#else
   U8                  cqi     = ueUl->crntUlCqi[0];
#endif
   Bool                isAcc   = uePwr->isAccumulated;
   U8                  tpc;
   S8                  availPwr;
   U8                  maxRb;

   UNUSED(cell);

   TRC2(rgSCHPwrPuschCntrl);

   if (!uePwr->isPhrAvail)
   {
      availPwr = 60; /* setting a large value so that availPwr does
                      * not constrain delta */
   }
   else
   {
      availPwr = uePwr->maxUePwr - uePwr->pwrPerRb;
      availPwr -= rgSCHPwrGetCqiPwrForUe(ue, cqi);
   }
   delta = uePwr->remPuschPwr;
   rgSCHPwrGetPuschTpc(isAcc, delta, availPwr, &tpc, &delta);
   availPwr -= delta;

   maxRb = rgSCHPwrGetMaxRb(cell,availPwr);

   /* Store the results in ue power control block to be used later */
   if(maxRb < cellUl->sbSize)
   {
       maxRb = cellUl->sbSize;
#ifdef TFU_UPGRADE
       if(uePwr->maxPwrDeltaByPhr < 0)
       {
          tmp = ueUl->validUlCqi;
          tmp = tmp + uePwr->maxPwrDeltaByPhr;
          if (tmp < 1 )
          {
              ueUl->validUlCqi = 1;
          }
          else
          {
              ueUl->validUlCqi = tmp;
          }
       }
#endif
   }
   RLOG_ARG4(L_UNUSED,DBG_CELLID,cell->cellId,
         "UEID:%d Output Max Rb (%d), phVal (%d) AvailPwr (%d) ",
         ue->ueId, maxRb, uePwr->phVal, availPwr);
   RLOG_ARG3(L_UNUSED,DBG_CELLID,cell->cellId,
         "UEID:%d pwrPerRb %d remPuschPwr %d", 
         ue->ueId,
         uePwr->pwrPerRb,
         uePwr->remPuschPwr);
   uePwr->delta   = delta;
   uePwr->maxUlRbs = maxRb;
   uePwr->puschTpc = tpc;
   RETVOID;
}

/**
 * @brief Returns TPC to be sent in DL allocation
 *
 * @details
 *
 *     Function : rgSCHPwrPucchTpcForUe
 *
 *     Invoking Module Processing:
 *      - After DL allocation for UE, this function shall
 *        be invoked to obtain TPC.
 *
 *     Processing Steps:
 *     - Do Pucch power control processing
 *       and return TPC
 *
 *  @param[in]  RgSchCellCb  *cell
 *  @param[in]  RgSchUeCb    *ue
 *  @return  U8
 **/
#ifdef ANSI
PUBLIC U8 rgSCHPwrPucchTpcForUe
(
RgSchCellCb *cell,
RgSchUeCb   *ue
)
#else
PUBLIC U8 rgSCHPwrPucchTpcForUe(cell, ue)
RgSchCellCb *cell;
RgSchUeCb   *ue;
#endif
{
   RgSchCmnUeUlPwrCb     *uePwr = RG_SCH_PWR_GETUEPWR(ue);
   TRC2(rgSCHPwrPucchTpcForUe);

   rgSCHPwrPucchCntrl(cell, ue);
   RETVALUE(uePwr->pucchTpc);
}

/***********************************************************
 *
 *     Func : rgSCHPwrGetDelta2FrmCqi
 *
 *     Desc : Get power to be applied to achieve
 *            target CQI (the power returned is
 *            twice is actual power)
 *
 *     Ret  : S8
 *
 *     Notes:
 *
 *     File :
 *
 **********************************************************/
#ifdef ANSI
PRIVATE S8 rgSCHPwrGetDelta2FrmCqi
(
U8                  crntCqi,
U8                  trgCqi,
RgSchUeCb           *ue,
RgSchCellCb         *cell

)
#else
PRIVATE S8 rgSCHPwrGetDelta2FrmCqi(crntCqi, trgCqi)
U8                  crntCqi;
U8                  trgCqi;
RgSchUeCb           *ue;
RgSchCellCb         *cell;
#endif
{
   RgSchCmnUeUlPwrCb     *uePwr = RG_SCH_PWR_GETUEPWR(ue);
   TRC2(rgSCHPwrGetDelta2FrmCqi);
 
   if (uePwr->isPhrAvail)
   {
      //uePwr->maxPwrDeltaByPhr = uePwr->maxPwrPerRb - uePwr->pwrPerRb - uePwr->remPuschPwr;
      uePwr->maxPwrDeltaByPhr = uePwr->maxPwrPerRb - uePwr->pwrPerRb;
   }
   else
   {
      uePwr->maxPwrDeltaByPhr = 0;      
   }
     if (uePwr->maxPwrDeltaByPhr < 0 && (trgCqi - crntCqi) *       
           RG_SCH_UL_CQI_DB_STEP_2 > 0)      
     {      
         RETVALUE(0);       
     }
   RETVALUE(RGSCH_MIN(uePwr->maxPwrDeltaByPhr, 
           (trgCqi - crntCqi) * RG_SCH_UL_CQI_DB_STEP_2));
}  /* rgSCHPwrGetDelta2FrmCqi */

/***********************************************************
 *
 *     Func : rgSCHPwrGetPuschTpc
 *
 *     Desc : Based on whether accumulation is enabled or
 *            not, this returns an applicable power delta
 *            to be applied based on the input delta.
 *
 *     Ret  : S8
 *
 *     Notes:
 *
 *     File :
 *
 **********************************************************/
#ifdef ANSI
PRIVATE Void rgSCHPwrGetPuschTpc
(
U8                  isAcc,
S8                  delta,
S8                  availPwr,
U8                 *tpc,
S8                 *tpcDelta
)
#else
PRIVATE Void rgSCHPwrGetPuschTpc(isAcc, delta, availPwr, tpc, tpcDelta)
U8                  isAcc;
S8                  delta;
S8                  availPwr;
U8                 *tpc;
S8                 *tpcDelta;
#endif
{
   TRC2(rgSCHPwrGetPuschTpc);

   delta = RGSCH_MIN(delta, availPwr);

   /* As of now, the functions below possibly cause delta
    * to be breached by 1 only. So calling these as is. */
   if (isAcc)
   {
      rgSCHPwrGetAcc2bitTpc(delta, tpc, tpcDelta);
   }
   else
   {
      rgSCHPwrGetAbsTpc(delta, tpc, tpcDelta);
   }
   RETVOID;
}  /* rgSCHPwrGetPuschTpc */

/***********************************************************
 *
 *     Func : rgSCHPwrGetMaxRb
 *
 *     Desc : Get the maximum number of RBs that can be
 *            expected to be supported by the passed
 *            power headroom.
 *
 *     Ret  : U8
 *
 *     Notes:
 *
 *     File :
 *
 **********************************************************/
#ifdef ANSI
PRIVATE U8 rgSCHPwrGetMaxRb
(
RgSchCellCb        *cell,
S8                  pwr
)
#else
PRIVATE U8 rgSCHPwrGetMaxRb(cell, pwr)
RgSchCellCb        *cell;
S8                  pwr;
#endif
{
   RgSchCmnUlCell  *cellUl;

   TRC2(rgSCHPwrGetMaxRb);
   
   cellUl    = RG_SCH_CMN_GET_UL_CELL(cell);
   if (pwr <= 0)
   {
      /* Give 4 RBS so that UE can report changed power status*/
      /* [ccpu00119916] Mod -return 0th index of rgSchPwrToRbTbl when pwr <=0
       *  Change the Macros from RGSCH_MAX_DL_BW to RGSCH_MAX_UL_BW*/
      RETVALUE(rgSchPwrToRbTbl[0]);
   }
   if (pwr > rgSchPwrRbToPwrTbl[cellUl->maxUlBwPerUe])
   {
      RETVALUE(cellUl->maxUlBwPerUe);
   }
   RETVALUE(RGSCH_MIN(cellUl->maxUlBwPerUe,rgSchPwrToRbTbl[(U8)pwr]));
}  /* rgSCHPwrGetMaxRb */

/***********************************************************
 *
 *     Func : rgSCHPwrRbToPwr
 *
 *     Desc : Get the power corresponding to number of RBs
 *
 *     Ret  : U8
 *
 *     Notes:
 *
 *     File :
 *
 **********************************************************/
#ifdef ANSI
PRIVATE U8 rgSCHPwrRbToPwr
(
RgSchCellCb          *cell,
U8                  numRb
)
#else
PRIVATE U8 rgSCHPwrRbToPwr(cell,numRb)
RgSchCellCb          *cell;
U8                  numRb;
#endif
{
#ifndef NO_ERRCLS
   RgSchCmnUlCell  *cellUl;
#endif
   TRC2(rgSCHPwrRbToPwr);
#if (ERRCLASS & ERRCLS_DEBUG)
   cellUl    = RG_SCH_CMN_GET_UL_CELL(cell);
   if (numRb > cellUl->maxUlBwPerUe)
   {
      numRb = cellUl->maxUlBwPerUe;
   }
#endif
   RETVALUE(rgSchPwrRbToPwrTbl[numRb]);
}  /* rgSCHPwrRbToPwr */


/**
 * @brief Handles Pucch power control for DCI formats 1A/1B/1D/1/2A/2 
 *
 * @details
 *
 *     Function : rgSCHPwrPucchCntrl
 *
 *     Processing Steps:
 *     - Determine 2 bit TPC to be sent using remPucchPwr.
 *     - Update remPucchPwr appropriately
 *
 *  @param[in]  RgSchCellCb  *cell
 *  @param[in]  RgSchUeCb    *ue
 *  @return  Void
 **/
#ifdef ANSI
PRIVATE Void rgSCHPwrPucchCntrl
(
RgSchCellCb *cell,
RgSchUeCb   *ue
)
#else
PRIVATE Void rgSCHPwrPucchCntrl(cell, ue)
RgSchCellCb *cell;
RgSchUeCb   *ue;
#endif
{
   S8                     delta;
   RgSchCmnUeUlPwrCb     *uePwr = RG_SCH_PWR_GETUEPWR(ue);
   TRC2(rgSCHPwrPucchCntrl);

   rgSCHPwrGetAcc2bitTpc(uePwr->remPucchPwr, &uePwr->pucchTpc, &delta);
   rgSCHPwrOnSchedPucchTpc(cell, ue, delta);
   RETVOID;
}

/**
 * @brief Handles group power control for DCI formats 3/3A for Pucch and Pusch
 *
 * @details
 *
 *     Function : rgSCHPwrGrpCntrlPucch
 *
 *     Invoking Module Processing:
 *      - This shall be invoked to do group power control for
 *        all TPC RNTIs for which it is deemed necessary to
 *        do the same (group power control).
 *      - This function should only be invoked after all UEs
 *        have been scheduled for uplink (re)transmissions
 *        requiring DL DCI format in the passed subframe.
 *
 *     Processing Steps:
 *     - For Pucch group power control
 *       - For each TPC-Pucch-RNTI in the pucchGrpPwr List and 
 *         TPC-Pusch-RNTI in the puschGrpPwr List,
 *          - Request for PDCCH, skip if not available
 *          - Form DCI format 3/3A information depending
 *            on the format type of the TPC-RNTI and add it to the sub-frame.
 *          - For each Ue in ueLst of TPC RNTI Cb
 *             - if (fmtType == 3A)
 *              - if((Ue not scheduled DL dci formats)
 *                     && (remPwr >= 2 || remPwr <= -2))
 *                  - Determine TPC. Set puschTpc/pucchTpc.
 *                  - remPwr -= TPC
 *                  - if (remPwr >= -1 && remPwr <= 1)
 *                    - If already added, remove from toBeSchdLst
 *              - else
 *                  - Toggle the remainig power value
 *             - else if (fmtType == 3)
 *              - if((Ue not scheduled DL dci formats)
 *                    && (remPwr))
 *                  - Determine TPC. Set puschTpc/pucchTpc.
 *                  - remPwr -= TPC
 *                  - if (!remPwr)
 *                    - If already added, remove from toBeSchdLst
 *          - if (!toBeSchdUeCnt)
 *             - Remove the tpcRntiCb frm pucchGrpPwr/puschGrpPwr List
 *          - else, Move the tpcRntiCb to end of the list (not doing
 *             this)
 *
 *  @param[in]  RgSchCellCb  *cell
 *  @param[in]  RgSchDlSf    *dlSf
 *  @return  Void
 **/
#ifdef ANSI
PUBLIC Void rgSCHPwrGrpCntrlPucch
(
RgSchCellCb *cell,
RgSchDlSf   *dlSf
)
#else
PUBLIC Void rgSCHPwrGrpCntrlPucch(cell, dlSf)
RgSchCellCb *cell;
RgSchDlSf   *dlSf;
#endif
{
   RgSchCmnUlPwrCb       *cellPwr = RG_SCH_PWR_GETCELLPWR(cell);
   CmLListCp             *lst;
   CmLList               *lnk;
   RgSchPdcch            *pdcch;
   TRC2(rgSCHPwrGrpCntrlPucch);

   lst = &cellPwr->pucchGrpPwr;
   lnk = lst->first;
   while (lnk && ((pdcch = rgSCHCmnCmnPdcchAlloc(cell, dlSf)) != NULLP))
   {
      RgSchCmnTpcRntiCb      *cb = (RgSchCmnTpcRntiCb *)lnk->node;
      Bool                    sched;
      lnk = lnk->next;
      rgSCHPwrSchedPucchRnti(cell, cb, pdcch, dlSf, &sched);
      if (!sched)
      {
         rgSCHUtlPdcchPut(cell, &dlSf->pdcchInfo, pdcch);
      }
      /* TPC RNTI would not have been removed if needs to
       * be scheduled again */
   }

   RETVOID;
}

/**
 * @brief Handles group power control for DCI formats 3/3A for Pusch and Pusch
 *
 * @details
 *
 *     Function : rgSCHPwrGrpCntrlPusch
 *
 *     Invoking Module Processing:
 *      - This shall be invoked to do group power control for
 *        all TPC RNTIs for which it is deemed necessary to
 *        do the same (group power control).
 *      - This function should only be invoked after all UEs
 *        have been scheduled for uplink (re)transmissions
 *        requiring DCI format 0 in the passed subframe.
 *
 *     Processing Steps:
 *     - For Pusch group power control
 *       - For each TPC-Pusch-RNTI in the puschGrpPwr List and 
 *          - Request for PDCCH, skip if not available
 *          - Form DCI format 3/3A information depending
 *            on the format type of the TPC-RNTI and add it to the sub-frame.
 *          - For each Ue in ueLst of TPC RNTI Cb
 *             - if (fmtType == 3A)
 *              - if (Ue not scheduled for dci format 0) and
 *                (remPwr >= 2 || remPwr <= -2))
 *                  - Determine TPC. Set puschTpc/puschTpc.
 *                  - remPwr -= TPC
 *                  - if (remPwr >= -1 && remPwr <= 1)
 *                    - If already added, remove from toBeSchdLst
 *              - else
 *                  - Toggle the remainig power value
 *             - else if (fmtType == 3)
 *              - if((Ue not scheduled for dci format 0) && (remPwr))
 *                  - Determine TPC. Set puschTpc.
 *                  - remPwr -= TPC
 *                  - if (!remPwr)
 *                    - If already added, remove from toBeSchdLst
 *          - if (!toBeSchdUeCnt)
 *             - Remove the tpcRntiCb frm puschGrpPwr/puschGrpPwr List
 *          - else, Move the tpcRntiCb to end of the list (not doing
 *            this now)
 *
 *  @param[in]  RgSchCellCb  *cell
 *  @param[in]  RgSchDlSf    *sf
 *  @return  Void
 **/
#ifdef ANSI
PUBLIC Void rgSCHPwrGrpCntrlPusch
(
RgSchCellCb *cell,
RgSchDlSf   *dlSf,
RgSchUlSf   *ulSf
)
#else
PUBLIC Void rgSCHPwrGrpCntrlPusch(cell, dlSf, ulSf)
RgSchCellCb *cell;
RgSchDlSf   *dlSf;
RgSchUlSf   *ulSf;
#endif
{
   RgSchCmnUlPwrCb       *cellPwr = RG_SCH_PWR_GETCELLPWR(cell);
   CmLListCp             *lst;
   CmLList               *lnk;
   RgSchPdcch            *pdcch;
   TRC2(rgSCHPwrGrpCntrlPusch);

   lst = &cellPwr->puschGrpPwr;
   lnk = lst->first;
   while (lnk && ((pdcch = rgSCHCmnCmnPdcchAlloc(cell, dlSf)) != NULLP))
   {
      RgSchCmnTpcRntiCb      *cb = (RgSchCmnTpcRntiCb *)lnk->node;
      Bool                    sched;
      lnk = lnk->next;
      rgSCHPwrSchedPuschRnti(cell, cb, pdcch, ulSf, &sched);
      if (!sched)
      {
         rgSCHUtlPdcchPut(cell, &dlSf->pdcchInfo, pdcch);
      }
      /* TPC RNTI would not have been removed if needs to
       * be scheduled again */
   }

   RETVOID;
}

/***********************************************************
 *
 *     Func : rgSCHPwrSchedPucchRnti
 *
 *     Desc : Schedule TPC RNTI to be sent out
 *
 *     Ret  : Void
 *
 *     Notes:
 *
 *     File :
 *
 **********************************************************/
#ifdef ANSI
PRIVATE Void rgSCHPwrSchedPucchRnti
(
RgSchCellCb          *cell,
RgSchCmnTpcRntiCb    *cb,
RgSchPdcch           *pdcch,
RgSchDlSf            *dlSf,
Bool                 *sched
)
#else
PRIVATE Void rgSCHPwrSchedPucchRnti(cell, cb, pdcch, dlSf, sched)
RgSchCellCb          *cell;
RgSchCmnTpcRntiCb    *cb;
RgSchPdcch           *pdcch;
RgSchDlSf            *dlSf;
Bool                 *sched;
#endif
{
   CmLListCp         *lst;
   CmLList           *lnk;
   U8                *tpcCmds;
   U8                 tpc;
   S8                 delta;
   Bool               atleastOne;
   TRC2(rgSCHPwrSchedPucchRnti);

   pdcch->rnti = cb->tpcRnti;

   if (cb->isFmt3a)
   {
      /* Go through all UEs for format 3A case */
      lst = &cb->cfgdUes;
      pdcch->dci.dciFormat = TFU_DCI_FORMAT_3A;
      pdcch->dci.u.format3AInfo.isPucch = TRUE;

      tpcCmds = pdcch->dci.u.format3AInfo.tpcCmd;
      /* No need to memset zero initially as every TPC is going
       * to be filled up for every configured UE */
      for (atleastOne = FALSE, lnk = lst->first; lnk; lnk = lnk->next)
      {
         RgSchUeCb             *ue    = (RgSchUeCb *)lnk->node;
         RgSchCmnUeUlPwrCb     *uePwr = RG_SCH_PWR_GETUEPWR(ue);

         if ( ue->isDrxEnabled == TRUE && 
               !RG_SCH_DRX_DL_IS_UE_ACTIVE(ue->drxCb))
         {
            /* UE is in its DRX time. So we cannot give command
             * to this UE.
             */
            continue;
         }

         if (rgSCHPwrIsDlUeSched(ue, dlSf))
         {
            /* UE already scheduled in downlink with PDCCH
             * carrying PUCCH pwr cmd. So don't care about
             * giving command to this UE. */
            continue;
         }
         rgSCHPwrGetPucchFmt3aTpcForUe(ue, &tpc, &delta);
         tpcCmds[uePwr->pucchIdx] = tpc;
         atleastOne = TRUE;
         rgSCHPwrOnPucchGrpPwrForUe(cell, ue, delta);
      }
   }
   else
   {
      /* Go through to-be-scheduled UEs for format 3 case */
      lst = &cb->toBeSchdUes;
      pdcch->dci.dciFormat = TFU_DCI_FORMAT_3;
      tpcCmds = pdcch->dci.u.format3Info.tpcCmd;
      pdcch->dci.u.format3Info.isPucch = TRUE;

      /* Fill TPC 1 (corresponding to no power change) initially */
      cmMemset((U8 *)tpcCmds, 1, sizeof(pdcch->dci.u.format3Info.tpcCmd));

      for (atleastOne = FALSE, lnk = lst->first; lnk; lnk = lnk->next)
      {
         RgSchUeCb             *ue    = (RgSchUeCb *)lnk->node;
         RgSchCmnUeUlPwrCb     *uePwr = RG_SCH_PWR_GETUEPWR(ue);

         if ( ue->isDrxEnabled == TRUE && 
               !RG_SCH_DRX_DL_IS_UE_ACTIVE(ue->drxCb))
         {
            /* UE is in its DRX time. So we cannot give command
             * to this UE.
             */
            continue;
         }

         if (rgSCHPwrIsDlUeSched(ue, dlSf))
         {
            /* UE already scheduled in downlink with PDCCH
             * carrying PUCCH pwr cmd. So don't care about
             * giving command to this UE. */
            continue;
         }
         rgSCHPwrGetPucchFmt3TpcForUe(ue, &tpc, &delta);
         tpcCmds[uePwr->pucchIdx] = tpc;
         atleastOne = TRUE;
         rgSCHPwrOnPucchGrpPwrForUe(cell, ue, delta);
      }
   }

   *sched = atleastOne;

   /* Check if no more UEs in TPC RNTI, and then remove
    * this TPC RNTI from scheduled list */
    if (cb->toBeSchdUes.count == 0)
    {
       rgSCHPwrRmvSchdPucchTpcRntiCb(cell, cb);
    }

   RETVOID;
}  /* rgSCHPwrSchedPucchRnti */

/***********************************************************
 *
 *     Func : rgSCHPwrSchedPuschRnti
 *
 *     Desc : Schedule TPC RNTI to be sent out
 *
 *     Ret  : Void
 *
 *     Notes:
 *
 *     File :
 *
 **********************************************************/
#ifdef ANSI
PRIVATE Void rgSCHPwrSchedPuschRnti
(
RgSchCellCb          *cell,
RgSchCmnTpcRntiCb    *cb,
RgSchPdcch           *pdcch,
RgSchUlSf            *ulSf,
Bool                 *sched
)
#else
PRIVATE Void rgSCHPwrSchedPuschRnti(cell, cb, pdcch, ulSf, sched)
RgSchCellCb          *cell;
RgSchCmnTpcRntiCb    *cb;
RgSchPdcch           *pdcch;
RgSchUlSf            *ulSf;
Bool                 *sched;
#endif
{
   CmLListCp         *lst;
   CmLList           *lnk;
   U8                *tpcCmds;
   U8                 tpc;
   S8                 delta;
   Bool               atleastOne;
   TRC2(rgSCHPwrSchedPuschRnti);

   pdcch->rnti = cb->tpcRnti;

   if (cb->isFmt3a)
   {
      /* Go through all UEs for format 3A case */
      lst = &cb->cfgdUes;
      pdcch->dci.dciFormat = TFU_DCI_FORMAT_3A;
      pdcch->dci.u.format3AInfo.isPucch = FALSE;
      tpcCmds = pdcch->dci.u.format3AInfo.tpcCmd;
      /* No need to memset zero initially as every TPC is going
       * to be filled up for every configured UE */
      for (atleastOne = FALSE, lnk = lst->first; lnk; lnk = lnk->next)
      {
         RgSchUeCb             *ue    = (RgSchUeCb *)lnk->node;
         RgSchCmnUeUlPwrCb     *uePwr = RG_SCH_PWR_GETUEPWR(ue);
         if (rgSCHPwrIsUlUeSched(cell, ue, ulSf))
         {
            /* UE already scheduled in uplink with DCI
             * format 0. So don't care about giving
             * command to this UE. */
            continue;
         }

         if ( ue->isDrxEnabled == TRUE && 
               !RG_SCH_DRX_DL_IS_UE_ACTIVE(ue->drxCb))
         {
            /* UE is in its DRX time. So we cannot give command
             * to this UE.
             */
            continue;
         }

         rgSCHPwrGetPuschFmt3aTpcForUe(ue, &tpc, &delta);
         tpcCmds[uePwr->puschIdx] = tpc;
         atleastOne = TRUE;
         rgSCHPwrOnPuschGrpPwrForUe(cell, ue, delta);
      }
   }
   else
   {
      /* Go through to-be-scheduled UEs for format 3 case */
      lst = &cb->toBeSchdUes;
      pdcch->dci.dciFormat = TFU_DCI_FORMAT_3;
      pdcch->dci.u.format3Info.isPucch = FALSE;
      tpcCmds = pdcch->dci.u.format3Info.tpcCmd;

      /* Fill TPC 1 (corresponding to no power change) initially */
      cmMemset((U8 *)tpcCmds, 1, sizeof(pdcch->dci.u.format3Info.tpcCmd));

      for (atleastOne = FALSE, lnk = lst->first; lnk; lnk = lnk->next)
      {
         RgSchUeCb             *ue    = (RgSchUeCb *)lnk->node;
         RgSchCmnUeUlPwrCb     *uePwr = RG_SCH_PWR_GETUEPWR(ue);
         if (rgSCHPwrIsUlUeSched(cell, ue, ulSf))
         {
            /* UE already scheduled in uplink with DCI
             * format 0. So don't care about giving
             * command to this UE. */
            continue;
         }

         if ( ue->isDrxEnabled == TRUE && 
               !RG_SCH_DRX_DL_IS_UE_ACTIVE(ue->drxCb))
         {
            /* UE is in its DRX time. So we cannot give command
             * to this UE.
             */
            continue;
         }

         rgSCHPwrGetPuschFmt3TpcForUe(ue, &tpc, &delta);
         tpcCmds[uePwr->puschIdx] = tpc;
         atleastOne = TRUE;
         rgSCHPwrOnPuschGrpPwrForUe(cell, ue, delta);
      }
   }

   *sched = atleastOne;

   /* Check if no more UEs in TPC RNTI, and then remove
    * this TPC RNTI from scheduled list */
    if (cb->toBeSchdUes.count == 0)
    {
       rgSCHPwrRmvSchdPuschTpcRntiCb(cell, cb);
    }

   RETVOID;
}  /* rgSCHPwrSchedPuschRnti */

/***********************************************************
 *
 *     Func : rgSCHPwrGetPucchFmt3TpcForUe
 *
 *     Desc : Gets 2 bit TPC cmd for PUCCH
 *
 *     Ret  : Void
 *
 *     Notes:
 *
 *     File :
 *
 **********************************************************/
#ifdef ANSI
PRIVATE Void rgSCHPwrGetPucchFmt3TpcForUe
(
RgSchUeCb            *ue,
U8                   *tpc,
S8                   *delta
)
#else
PRIVATE Void rgSCHPwrGetPucchFmt3TpcForUe(ue, tpc, delta)
RgSchUeCb            *ue;
U8                   *tpc;
S8                   *delta;
#endif
{
   RgSchCmnUeUlPwrCb     *uePwr = RG_SCH_PWR_GETUEPWR(ue);
   TRC2(rgSCHPwrGetPucchFmt3TpcForUe);

   rgSCHPwrGetAcc2bitTpc(uePwr->remPucchPwr, tpc, delta);
   RETVOID;
}  /* rgSCHPwrGetPucchFmt3TpcForUe */

/***********************************************************
 *
 *     Func : rgSCHPwrGetPucchFmt3aTpcForUe
 *
 *     Desc : Gets 1 bit TPC cmd for PUCCH
 *
 *     Ret  : Void
 *
 *     Notes:
 *
 *     File :
 *
 **********************************************************/
#ifdef ANSI
PRIVATE Void rgSCHPwrGetPucchFmt3aTpcForUe
(
RgSchUeCb            *ue,
U8                   *tpc,
S8                   *delta
)
#else
PRIVATE Void rgSCHPwrGetPucchFmt3aTpcForUe(ue, tpc, delta)
RgSchUeCb            *ue;
U8                   *tpc;
S8                   *delta;
#endif
{
   RgSchCmnUeUlPwrCb     *uePwr = RG_SCH_PWR_GETUEPWR(ue);
   TRC2(rgSCHPwrGetPucchFmt3aTpcForUe);

   rgSCHPwrGetAcc1bitTpc(uePwr->remPucchPwr, tpc, delta);
   RETVOID;
}  /* rgSCHPwrGetPucchFmt3aTpcForUe */

/***********************************************************
 *
 *     Func : rgSCHPwrGetPuschFmt3TpcForUe
 *
 *     Desc : Gets 2 bit TPC cmd for PUCCH
 *
 *     Ret  : Void
 *
 *     Notes:
 *
 *     File :
 *
 **********************************************************/
#ifdef ANSI
PRIVATE Void rgSCHPwrGetPuschFmt3TpcForUe
(
RgSchUeCb            *ue,
U8                   *tpc,
S8                   *delta
)
#else
PRIVATE Void rgSCHPwrGetPuschFmt3TpcForUe(ue, tpc, delta)
RgSchUeCb            *ue;
U8                   *tpc;
S8                   *delta;
#endif
{
   RgSchCmnUeUlPwrCb     *uePwr = RG_SCH_PWR_GETUEPWR(ue);
   S8                     adj = RGSCH_MIN(uePwr->remPuschPwr, uePwr->phVal);
   TRC2(rgSCHPwrGetPuschFmt3TpcForUe);

   rgSCHPwrGetAcc2bitTpc(adj, tpc, delta);
   RETVOID;
}  /* rgSCHPwrGetPuschFmt3TpcForUe */

/***********************************************************
 *
 *     Func : rgSCHPwrGetPuschFmt3aTpcForUe
 *
 *     Desc : Gets 1 bit TPC cmd for PUCCH
 *
 *     Ret  : Void
 *
 *     Notes:
 *
 *     File :
 *
 **********************************************************/
#ifdef ANSI
PRIVATE Void rgSCHPwrGetPuschFmt3aTpcForUe
(
RgSchUeCb            *ue,
U8                   *tpc,
S8                   *delta
)
#else
PRIVATE Void rgSCHPwrGetPuschFmt3aTpcForUe(ue, tpc, delta)
RgSchUeCb            *ue;
U8                   *tpc;
S8                   *delta;
#endif
{
   RgSchCmnUeUlPwrCb     *uePwr = RG_SCH_PWR_GETUEPWR(ue);
   TRC2(rgSCHPwrGetPuschFmt3aTpcForUe);

   /* Don't attempt to look at headroom now, power
    * adjustment is small anyway */
   rgSCHPwrGetAcc1bitTpc(uePwr->remPuschPwr, tpc, delta);
   RETVOID;
}  /* rgSCHPwrGetPuschFmt3aTpcForUe */

/***********************************************************
 *
 *     Func : rgSCHPwrGetAcc1bitTpc
 *
 *     Desc : Gets 1 bit TPC cmd
 *
 *     Ret  : Void
 *
 *     Notes:
 *
 *     File :
 *
 **********************************************************/
#ifdef ANSI
PRIVATE Void rgSCHPwrGetAcc1bitTpc
(
S8                    remPwr,
U8                   *tpc,
S8                   *delta
)
#else
PRIVATE Void rgSCHPwrGetAcc1bitTpc(remPwr, tpc, delta)
S8                    remPwr;
U8                   *tpc;
S8                   *delta;
#endif
{
   TRC2(rgSCHPwrGetAcc1bitTpc);
   /*
    * TPC   delta
    *  0     -1
    *  1      1
    */
   if (remPwr <= 0)
   {
      *delta = -1;
      *tpc = 0;
   }
   else
   {
      *delta = 1;
      *tpc = 1;
   }
   RETVOID;
}  /* rgSCHPwrGetAcc1bitTpc */

/***********************************************************
 *
 *     Func : rgSCHPwrGetAcc2bitTpc
 *
 *     Desc : Allocate PDCCH for group power control
 *
 *     Ret  : Void
 *
 *     Notes:
 *
 *     File :
 *
 **********************************************************/
#ifdef ANSI
PRIVATE Void rgSCHPwrGetAcc2bitTpc
(
S8                   remPwr,
U8                  *tpc,
S8                  *delta
)
#else
PRIVATE Void rgSCHPwrGetAcc2bitTpc(remPwr, tpc, delta)
S8                   remPwr;
U8                  *tpc;
S8                  *delta;
#endif
{
   /*
    * TPC   delta
    *  0     -1
    *  1      0
    *  2      1
    *  3      3
    */
   U8            tpcs[3]   = {1, 2, 2};
   U8            deltas[3] = {0, 1, 1};
   TRC2(rgSCHPwrGetAcc2bitTpc);
   if (remPwr <= -1)
   {
      *tpc   = 0;
      *delta = -1;
   }
   else if (remPwr >= 3)
   {
      *tpc   = 3;
      *delta = 3;
   }
   else
   {
      *tpc   = tpcs[(U8)remPwr];
      *delta = deltas[(U8)remPwr];
   }
   RETVOID;
}  /* rgSCHPwrGetAcc2bitTpc */

/***********************************************************
 *
 *     Func : rgSCHPwrGetAbsTpc
 *
 *     Desc : Allocate PDCCH for group power control
 *
 *     Ret  : Void
 *
 *     Notes:
 *
 *     File :
 *
 **********************************************************/
#ifdef ANSI
PRIVATE Void rgSCHPwrGetAbsTpc
(
S8                   remPwr,
U8                  *tpc,
S8                  *delta
)
#else
PRIVATE Void rgSCHPwrGetAbsTpc(remPwr, tpc, delta)
S8                   remPwr;
U8                  *tpc;
S8                  *delta;
#endif
{
   TRC2(rgSCHPwrGetAbsTpc);
   /*
    * TPC   delta
    *  0     -4
    *  1     -1
    *  2      1
    *  3      4
    */
   if (remPwr <= -3)
   {
      *tpc = 0;
      *delta = -4;
   }
   else if (remPwr < 1)
   {
      *tpc = 1;
      *delta = -1;
   }
   else if (remPwr < 4)
   {
      *tpc = 2;
      *delta = 1;
   }
   else
   {
      *tpc = 3;
      *delta = 4;
   }
   RETVOID;
}  /* rgSCHPwrGetAbsTpc */

/***********************************************************
 *
 *     Func : rgSCHPwrOnPucchGrpPwrForUe
 *
 *     Desc : Processing on sending TPC for UE through group power
 *            control. Apart from updating remPwr, this only takes
 *            care of possibly removing UE from scheduled
 *            list in TPC RNTI.
 *            It does not take care of possibly removing TPC RNTI
 *            from scheduled list in cell. This is done
 *            in the caller after TPC for all UEs has been
 *            determined. (This is where it differs
 *            from the usual OnSendingPu[cs]ch TPC]
 *
 *     Ret  : ROK/RFAILED
 *
 *     Notes:
 *
 *     File :
 *
 **********************************************************/
#ifdef ANSI
PRIVATE Void rgSCHPwrOnPucchGrpPwrForUe 
(
RgSchCellCb          *cell,
RgSchUeCb            *ue,
S8                    delta
)
#else
PRIVATE Void rgSCHPwrOnPucchGrpPwrForUe(cell, ue, delta)
RgSchCellCb          *cell;
RgSchUeCb            *ue;
S8                    delta;
#endif
{
   RgSchCmnUeUlPwrCb     *uePwr = RG_SCH_PWR_GETUEPWR(ue);
   Bool                   rmvUe = FALSE;

   UNUSED(cell);

   TRC2(rgSCHPwrOnPucchGrpPwrForUe);

   uePwr->remPucchPwr -= delta;

   /* UE was already scheduled for PUCCH group power
    * control which is why we came here. Don't
    * again check for this. */

   /* UE was scheduled for pucch grp pwr, sent TPC may
    * possibly cause it to be removed. */
      if (!uePwr->remPucchPwr)
      {
         rmvUe = TRUE;
      }
   if (rmvUe)
   {
      rgSCHPwrRmvSchdUeOnlyFrmPucchTpcRntiCb(cell, uePwr->tpcPucchRntiCb, ue);
      /* Not removing TPC RNTI from scheduled list,
       * this will happen in the caller once this
       * function is called for every UE scheduled. */
   }
   RETVOID;
}

/***********************************************************
 *
 *     Func : rgSCHPwrOnPuschGrpPwrForUe
 *
 *     Desc : Processing on sending TPC for UE through group power
 *            control. Apart from updating remPwr, this only takes
 *            care of possibly removing UE from scheduled
 *            list in TPC RNTI.
 *            It does not take care of possibly removing TPC RNTI
 *            from scheduled list in cell. This is done
 *            in the caller after TPC for all UEs has been
 *            determined. (This is where it differs
 *            from the usual OnSendingPu[cs]ch TPC]
 *
 *     Ret  : ROK/RFAILED
 *
 *     Notes:
 *
 *     File :
 *
 **********************************************************/
#ifdef ANSI
PRIVATE Void rgSCHPwrOnPuschGrpPwrForUe 
(
RgSchCellCb          *cell,
RgSchUeCb            *ue,
S8                    delta
)
#else
PRIVATE Void rgSCHPwrOnPuschGrpPwrForUe(cell, ue, delta)
RgSchCellCb          *cell;
RgSchUeCb            *ue;
S8                    delta;
#endif
{
   RgSchCmnUeUlPwrCb     *uePwr = RG_SCH_PWR_GETUEPWR(ue);
   Bool                   rmvUe = FALSE;

   UNUSED(cell);
   TRC2(rgSCHPwrOnPuschGrpPwrForUe);

   uePwr->delta = delta;
   uePwr->remPuschPwr -= delta;
   if (uePwr->isPhrAvail)
   {
      uePwr->phVal -= uePwr->delta;
      uePwr->phVal = RGSCH_MAX(-23, uePwr->phVal);
   }

   /* UE was already scheduled for PUSCH group power
    * control which is why we came here. Don't
    * again check for this. */

   /* UE was scheduled for pusch grp pwr, sent TPC may
    * possibly cause it to be removed. */

      if (!uePwr->remPuschPwr)
      {
         rmvUe = TRUE;
      }

   if (rmvUe)
   {
      rgSCHPwrRmvSchdUeOnlyFrmPuschTpcRntiCb(cell, uePwr->tpcPuschRntiCb, ue);
      /* Not removing TPC RNTI from scheduled list,
       * this will happen in the caller once this
       * function is called for every UE scheduled. */
   }
   RETVOID;
}

/***********************************************************
 *
 *     Func : rgSCHPwrIsDlUeSched
 *
 *     Desc : Check if UE is scheduled in the passed DL SF
 *
 *     Ret  : Void
 *
 *     Notes:
 *
 *     File :
 *
 **********************************************************/
#ifdef ANSI
PRIVATE Bool rgSCHPwrIsDlUeSched
(
RgSchUeCb            *ue,
RgSchDlSf            *sf
)
#else
PRIVATE Bool rgSCHPwrIsDlUeSched(ue, sf)
RgSchUeCb            *ue;
RgSchDlSf            *sf;
#endif
{
   RgSchDlHqProcCb      *proc    = rgSCHDhmLastSchedHqProc(ue->dl.hqEnt);

   TRC2(rgSCHPwrIsDlUeSched);

   if (proc == NULLP)
   {
      RETVALUE(FALSE);
   }

   /*
    * The following subframe check is assumed enough, since
    * scheduled procs stay for a short time (until feedback
    * arrives), which typically is expected to have a
    * turnaround time of less than 8 subframes. So
    * we are probably never going to come across cases
    * where a process stays in the list for more than
    * 10 subframes, which would have otherwise caused
    * the check to succeed for a possibly older process.
    */
   if ((proc->tbInfo[0].timingInfo.subframe == sf->sfNum) ||
       (proc->tbInfo[1].timingInfo.subframe == sf->sfNum))
   {
      /*
       * Later, if a proc can be scheduled without having an
       * associated PDCCH, need to also check if PDCCH exists.
       * This is because for power, what matters is whether
       * TPC is going out for UE at this time or not, at least
       * that is what this function was introduced for.
       * Checking for PDCCH would have to be in common proc
       * the way things are now.
       */
      RETVALUE(TRUE);
   }
   else
   {
      RETVALUE(FALSE);
   }
}  /* rgSCHPwrIsDlUeSched */

/***********************************************************
 *
 *     Func : rgSCHPwrIsUlUeSched
 *
 *     Desc : Check if UE is scheduled in the passed UL SF
 *
 *     Ret  : Void
 *
 *     Notes:
 *
 *     File :
 *
 **********************************************************/
#ifdef ANSI
PRIVATE Bool rgSCHPwrIsUlUeSched
(
RgSchCellCb          *cell,
RgSchUeCb            *ue,
RgSchUlSf            *sf
)
#else
PRIVATE Bool rgSCHPwrIsUlUeSched(cell, ue, sf)
RgSchCellCb          *cell;
RgSchUeCb            *ue;
RgSchUlSf            *sf;
#endif
{
   RgSchCmnUlCell      *cmnCell = RG_SCH_CMN_GET_UL_CELL(cell);
   RgSchUlHqProcCb     *proc = rgSCHUhmGetUlHqProc(ue, cmnCell->schdHqProcIdx);

   TRC2(rgSCHPwrIsUlUeSched);

   UNUSED(sf);

#if (ERRCLASS & ERRCLS_DEBUG)
   if( proc == NULLP )
   {
      RETVALUE (FALSE);
   }
#endif  

   if (proc->alloc)
   {
      RETVALUE(TRUE);
   }
   else
   {
      RETVALUE(FALSE);
   }
}  /* rgSCHPwrIsUlUeSched */

/**
 * @brief Handles Pucch power delta indication recieved from PHY
 *
 * @details
 *
 *     Function : rgSCHPwrPucchDeltaInd
 *
 *     Invoking Module Processing:
 *      - This shall be invoked on reception of Pucch power 
 *        delta indication from PHY.
 *        
 *     Processing Steps:
 *     - Update the remPucchPwr 
 *       ue->remPucchPwr = pwrDelta
 *     - If (ue->tpcPucchRntiCb)
 *       - If (fmtType = 3A)
 *          - if (remPucchPwr >= 2 || remPucchPwr <= -2 )
 *             - if (tpcPucchRntiCb is not in the pucchGrpPwr List)
 *               - Add tpcPucchRntiCb to the pucchGrpPwr list.
 *             - If not added, add to toBeSchdLst
 *          - else
 *             - If already added, remove from toBeSchdLst
 *       - else If (fmtType == 3) 
 *          - if (remPucchPwr)
 *             - if (tpcPucchRntiCb is not in the pucchGrpPwr List)
 *               - Add tpcPucchRntiCb to the pucchGrpPwr list.
 *             - If not added, add to toBeSchdLst
 *          - else
 *             - If already added, remove from toBeSchdLst
 *
 *  @param[in]  RgSchCellCb  *cell
 *  @param[in]  RgSchUeCb    *ue
 *  @param[in]  U8           pwrDelta
 *  @return  Void
 **/
#ifdef ANSI
PUBLIC Void rgSCHPwrPucchDeltaInd
(
RgSchCellCb *cell,
RgSchUeCb   *ue,
S8          pwrDelta
)
#else
PUBLIC Void rgSCHPwrPucchDeltaInd(cell, ue, pwrDelta)
RgSchCellCb *cell;
RgSchUeCb   *ue;
S8          pwrDelta;
#endif
{
   RgSchCmnUeUlPwrCb     *uePwr = RG_SCH_PWR_GETUEPWR(ue);
   RgSchCmnTpcRntiCb     *cb;
   Bool                   toAdd;
   TRC2(rgSCHPwrPucchDeltaInd);

   uePwr->remPucchPwr = pwrDelta;
   
   if ((cb = uePwr->tpcPucchRntiCb) == NULLP)
   {
      RETVOID;
   }

   toAdd = FALSE;

   if (0 != uePwr->remPucchPwr)
      {
         toAdd = TRUE;
      }


   if (toAdd)
   {
      rgSCHPwrAddSchdUeToPucchTpcRntiCb(cell, cb, ue);
   }
   else
   {
      rgSCHPwrRmvSchdUeFrmPucchTpcRntiCb(cell, cb, ue);
   }

   RETVOID;
}

/**
 * @brief Does processing after TPC for Pucch has been sent
 *
 * @details
 *
 *     Function : rgSCHPwrOnSchedPucchTpc
 *
 *     Invoking Module Processing:
 *      - It shall be invoked after it is determined that PDCCH for UE
 *        is finalised to go out, and thus TPC for PUCCH is being
 *        sent out.
 *
 *     Processing Steps:
 *     - Update remPucchPwr with the delta
 *     - Do group power control related processing
 *
 *  @param[in]  RgSchCellCb       *cell
 *  @param[in]  RgSchUeCb         *ue
 *  @param[in]  S8                 delta
 *  @return  Void
 **/
#ifdef ANSI
PRIVATE Void rgSCHPwrOnSchedPucchTpc
(
RgSchCellCb           *cell,
RgSchUeCb             *ue,
S8                     delta
)
#else
PRIVATE Void rgSCHPwrOnSchedPucchTpc(cell, ue, delta)
RgSchCellCb           *cell;
RgSchUeCb             *ue;
S8                     delta;
#endif
{
   /* Similar to rgSCHPwrPucchDeltaInd.. not reusing
    * that since we use the fact that UE could only have
    * improved its remPwr as part of power control. */
   RgSchCmnUeUlPwrCb     *uePwr = RG_SCH_PWR_GETUEPWR(ue);
   Bool                   rmvUe = FALSE;
   TRC2(rgSCHPwrOnSchedPucchTpc);

   uePwr->remPucchPwr -= delta;

   if (uePwr->schdPucchGrpLnk.node == NULLP)
   {
      RETVOID;
   }

   /* UE was scheduled for TPC, sent TPC may
    * possibly cause it to be removed. */

      if (!uePwr->remPucchPwr)
      {
         rmvUe = TRUE;
      }

   if (rmvUe)
   {
      rgSCHPwrRmvSchdUeFrmPucchTpcRntiCb(cell, uePwr->tpcPucchRntiCb, ue);
      if (uePwr->tpcPucchRntiCb->toBeSchdUes.count == 0)
      {
         rgSCHPwrRmvSchdPucchTpcRntiCb(cell, uePwr->tpcPucchRntiCb);
      }
   }
   RETVOID;
}


/**
 * @brief Does processing after TPC for Pusch has been sent
 *
 * @details
 *
 *     Function : rgSCHPwrOnSchedPuschTpc
 *
 *     Processing Steps:
 *      - If accumulative
 *        - Update remPuschPwr with the delta
 *        - Do group power related processing if applicable
 *       
 *  @param[in]  RgSchCellCb       *cell
 *  @param[in]  RgSchUeCb         *ue
 *  @return  Void
 **/
#ifdef ANSI
PRIVATE Void rgSCHPwrOnSchedPuschTpc
(
RgSchCellCb           *cell,
RgSchUeCb             *ue
)
#else
PRIVATE Void rgSCHPwrOnSchedPuschTpc(cell, ue)
RgSchCellCb           *cell;
RgSchUeCb             *ue;
#endif
{
   RgSchCmnUeUlPwrCb     *uePwr = RG_SCH_PWR_GETUEPWR(ue);
   Bool                   rmvUe = FALSE;
   TRC2(rgSCHPwrOnSchedPuschTpc);

   /* Don't do anything for the case of absolute TPC commands */
   if (!uePwr->isAccumulated)
   {
      RETVOID;
   }

   uePwr->remPuschPwr -= uePwr->delta;
   if (uePwr->isPhrAvail)
   {
      uePwr->phVal -= uePwr->delta;
      uePwr->phVal = RGSCH_MAX(-23, uePwr->phVal);
   }

   if (uePwr->schdPuschGrpLnk.node == NULLP)
   {
      RETVOID;
   }

   /* UE was scheduled for pusch TPC, sent TPC may
    * possibly cause it to be removed. */

      if (!uePwr->remPuschPwr)
      {
         rmvUe = TRUE;
      }

   if (rmvUe)
   {
      rgSCHPwrRmvSchdUeFrmPuschTpcRntiCb(cell, uePwr->tpcPuschRntiCb, ue);
   }

   RETVOID;
}

/**
 * @brief Handles PHR updation for the UE
 *
 * @details
 *
 *     Function : rgSCHPwrUpdPhr
 *
 *     Invoking Module Processing:
 *      - This shall be invoked on reception of PHR from MAC to SCH. It shall
 *      pass the information of number of RBs, coding efficiency and TPC for 
 *      the Pusch transmission for which PHR has been reported.
 *
 *     Processing Steps:
 *     - Compute power per RB using the PHR report
 *        - ue_transmit_pwr = ue_max_pwr - PHR
 *        - if isDeltaMcs = TRUE
 *          - ue_transmit_pwr - 
 *            [10log(phr_num_rb) + 10log(2^ (1.25 * phr_coding_effeciency) -1)
 *            + phr_tpc(if absolute TPC)] = pwrPerRB
 *        - else
 *          - ue_transmit_pwr - [10log(phr_num_rb) + phr_tpc(if absolute TPC)]
 *            = pwrPerRB
 *        (Use the number of RBs and efficiency used by UE which caused the PHR
 *         report to happen)
 *     - Adjust PHR according to last allocation (take into account
 *       number of RBs granted in the last allocation)
 *     - Update the PHR report in the control block
 *     - Set isPhrAvail = TRUE
 *     - Do group power control related processing if applicable
 *
 *  @param[in]  RgSchCellCb         *cell
 *  @param[in]  RgSchUeCb           *ue
 *  @param[in]  U8                   phr
 *  @param[in]  RgSchCmnAllocRecord  allocInfo
 *  @return  Void
 **/
#ifdef ANSI
PUBLIC Void rgSCHPwrUpdPhr
(
RgSchCellCb           *cell,
RgSchUeCb             *ue,
U8                     phr,
RgSchCmnAllocRecord   *allocInfo
)
#else
PUBLIC Void rgSCHPwrUpdPhr(cell, ue, phr, allocInfo)
RgSchCellCb           *cell;
RgSchUeCb             *ue;
U8                     phr;
RgSchCmnAllocRecord   *allocInfo;
#endif
{
   RgSchCmnUeUlPwrCb     *uePwr   = RG_SCH_PWR_GETUEPWR(ue);
   U8                     rbPwr;
   U8                     effPwr;
   RgSchCmnUlCell *cellUl = RG_SCH_CMN_GET_UL_CELL(cell);

   TRC2(rgSCHPwrUpdPhr);

   uePwr->phVal = rgSCHPwrGetPhValFromPhr(phr);
   
   rbPwr = rgSCHPwrRbToPwr(cell,allocInfo->numRb);
   effPwr = rgSCHPwrGetCqiPwrForUe(ue, allocInfo->cqi);
   uePwr->pwrPerRb = uePwr->maxUePwr - uePwr->phVal - rbPwr - effPwr;
   /*if (!uePwr->isAccumulated)
   {
      uePwr->pwrPerRb -= rgSCHPwrGetDeltaFrmAbsTpc(allocInfo->tpc);
   }*/

   /* Let headroom reflect remaining power according to last
    * allocated number of RBs. Intermediate TPCs not yet
    * taken care of (for accumulated case, it is anyway
    * not applicable for absolute commands). */
   uePwr->phVal -= (rgSCHPwrRbToPwr(cell, cellUl->sbSize)) - rbPwr;
   uePwr->phVal = RGSCH_MAX(-23, uePwr->phVal);
   uePwr->isPhrAvail = TRUE;

   rgSCHPwrOnPuschPwrUpd(cell, ue);

   RLOG_ARG4(L_DEBUG,DBG_UEID,ue->ueId,
         "Output: Reported PHR[%d] cqi[%u] allocRb[%u] uePwr->pwrPerRb[%d]",
            uePwr->phVal,
            allocInfo->cqi,
            allocInfo->numRb,  
            uePwr->pwrPerRb); 
   RETVOID;
}

/**
 * @brief Handles UL CQI indication
 *
 * @details
 *
 *     Function : rgSCHPwrUlCqiInd
 *
 *     Invoking Module Processing:
 *      - This shall be invoked when uplink CQI indication
 *        is receiving from PHY for a UE.
 *
 *     Processing Steps:
 *     - Update remPuschPwr.
 *     - Possibly schedule for group power control.
 *
 *  @param[in]  RgSchCellCb         *cell
 *  @param[in]  RgSchUeCb           *ue
 *  @param[in]  U8                   numRb
 *  @return  Void
 **/
#ifdef ANSI
PUBLIC Void rgSCHPwrUlCqiInd
(
RgSchCellCb           *cell,
RgSchUeCb             *ue
)
#else
PUBLIC Void rgSCHPwrUlCqiInd(cell, ue)
RgSchCellCb           *cell;
RgSchUeCb             *ue;
#endif
{
   RgSchCmnUlUe          *ueUl    = RG_SCH_CMN_GET_UL_UE(ue);
   RgSchCmnUeUlPwrCb     *uePwr   = RG_SCH_PWR_GETUEPWR(ue);
#ifdef TFU_UPGRADE 
   S32  tmp;
#endif
   TRC2(rgSCHPwrUlCqiInd);

   /*
    * For absolute power cmd case, we could look at the time
    * at which CQI was received, determine if there was a
    * PUSCH TPC cmd for that time (this could come from
    * group power control too), and (if this
    * CQI report is indeed based on the the PUSCH tx)
    * then factor in that cmd here. Not doing
    * this as of now.
    */

   /* See how much power needs to be adjusted based on cqi
    * differential */
   uePwr->remPuschPwr = 
#ifdef TFU_UPGRADE
      rgSCHPwrGetDelta2FrmCqi(ueUl->validUlCqi, uePwr->trgCqi, ue, cell);
#else
      rgSCHPwrGetDelta2FrmCqi(ueUl->crntUlCqi[0], uePwr->trgCqi, ue, cell);
#endif

   rgSCHPwrOnPuschPwrUpd(cell, ue);
#ifdef TFU_UPGRADE
   if(uePwr->maxPwrDeltaByPhr < 0)
   {
      tmp = ueUl->validUlCqi;
      tmp = tmp + uePwr->maxPwrDeltaByPhr;
      if (tmp < 1 )
      {
         ueUl->validUlCqi = 1;
      }
      else
      {
         ueUl->validUlCqi = tmp;
      }
   }
#endif 

   RETVOID;
}

/**
 * @brief Updates the stored last number of RBs allocated
 *
 * @details
 *
 *     Function : rgSCHPwrRecordRbAlloc
 *
 *     Invoking Module Processing:
 *      - This shall be invoked when uplink allocation is made for
 *        a UE.
 *      - Note: If outstanding TPCs are considered at the time
 *        of PHR report, the last numRb would also be known
 *        and then this API would not be needed. 
 *
 *     Processing Steps:
 *     - Adjust PHR according to now allocated number of RBs
 *     - Store the number of RBs
 *
 *  @param[in]  RgSchCellCb         *cell
 *  @param[in]  RgSchUeCb           *ue
 *  @param[in]  U8                   numRb
 *  @return  Void
 **/
#ifdef ANSI
PUBLIC Void rgSCHPwrRecordRbAlloc
(
RgSchCellCb           *cell,
RgSchUeCb             *ue,
U8                     numRb
)
#else
PUBLIC Void rgSCHPwrRecordRbAlloc(cell, ue, numRb)
RgSchCellCb           *cell;
RgSchUeCb             *ue;
U8                     numRb;
#endif
{
   RgSchCmnUeUlPwrCb     *uePwr   = RG_SCH_PWR_GETUEPWR(ue);
   UNUSED(cell);
   TRC2(rgSCHPwrRecordRbAlloc);
   RETVOID; 

   if (uePwr->isPhrAvail)
   {
      uePwr->phVal += rgSCHPwrRbToPwr(cell,numRb) - rgSCHPwrRbToPwr(cell,uePwr->numRb);
      uePwr->phVal = RGSCH_MIN(40, uePwr->phVal);
   }
   uePwr->numRb = numRb;
   RETVOID;
}

/**
 * @brief Handles power related configuration for a cell
 *
 * @details
 *
 *     Function : rgSCHPwrCellCfg
 *
 *     Invoking Module Processing:
 *      - This shall be invoked during cell config
 *        
 *     Processing Steps:
 *      - Set pMax
 *      - Set target CQI
 *      - Update TPC-RNTI information for the cell for Pucch and Pusch.
 *      - For each TPC-Pucch-RNTI,
 *         - Call  rgSCHAddRntiToPucchRntiLst()
 *      - For each TPC-Pusch-RNTI,
 *         - Call  rgSCHAddRntiToPuschRntiLst()
 *      - Return ROK
 *
 *  @param[in]  RgSchCellCb  *cell
 *  @param[in]  RgrCellCfg   *cfg
 *  @return  S16
 *      -# ROK
 *      -# RFAILED
 **/
#ifdef ANSI
PUBLIC S16 rgSCHPwrCellCfg
(
RgSchCellCb          *cell,
RgrCellCfg           *cfg
)
#else
PUBLIC S16 rgSCHPwrCellCfg(cell, cfg)
RgSchCellCb          *cell;
RgrCellCfg           *cfg;
#endif
{
   RgSchCmnUlPwrCb       *cellPwr = RG_SCH_PWR_GETCELLPWR(cell);
   CmLteRnti              rnti;
   CmLteRnti              startRnti;
   U16                    size;
   Bool                   isFmt3a;
   TRC2(rgSCHPwrCellCfg);

   /* Right now, all UEs have fixed maximum power capability. So
    * we store cell wide pMax as minimum of configured pMax and
    * UE's max power */
   cellPwr->pMax = RGSCH_MIN(cfg->pMax, RG_SCH_PWR_UE_MAX_PWR);

   /* trgUlCqi already validated by common */
   cellPwr->trgUlCqi = cfg->trgUlCqi.trgCqi;

   /* Validate number of TPC RNTIs */
   if ((cfg->pwrCfg.pucchPwrFmt3.size + cfg->pwrCfg.pucchPwrFmt3a.size
            > RG_SCH_CMN_MAX_NUM_TPC_PUCCH_RNTI)
         || (cfg->pwrCfg.puschPwrFmt3.size + cfg->pwrCfg.puschPwrFmt3a.size
            > RG_SCH_CMN_MAX_NUM_TPC_PUSCH_RNTI))
   {
      RETVALUE(RFAILED);
   }

   /* Now initialise TPC RNTIs */

   /* Format 3 Pucch TPC RNTIs */
   isFmt3a = FALSE;
   startRnti = cfg->pwrCfg.pucchPwrFmt3.startTpcRnti;
   size = cfg->pwrCfg.pucchPwrFmt3.size;
   for (rnti = startRnti; (rnti < startRnti + size); ++rnti)
   {
      rgSCHPwrAddRntiToPucchRntiLst(cell, rnti, isFmt3a);
   }

   /* Format 3A Pucch TPC RNTIs */
   isFmt3a = TRUE;
   startRnti = cfg->pwrCfg.pucchPwrFmt3a.startTpcRnti;
   size = cfg->pwrCfg.pucchPwrFmt3a.size;
   for (rnti = startRnti; (rnti < startRnti + size); ++rnti)
   {
      rgSCHPwrAddRntiToPucchRntiLst(cell, rnti, isFmt3a);
   }

   /* Format 3 Pusch TPC RNTIs */
   isFmt3a = FALSE;
   startRnti = cfg->pwrCfg.puschPwrFmt3.startTpcRnti;
   size = cfg->pwrCfg.puschPwrFmt3.size;
   for (rnti = startRnti; (rnti < startRnti + size); ++rnti)
   {
      rgSCHPwrAddRntiToPuschRntiLst(cell, rnti, isFmt3a);
   }

   /* Format 3A Pusch TPC RNTIs */
   isFmt3a = TRUE;
   startRnti = cfg->pwrCfg.puschPwrFmt3a.startTpcRnti;
   size = cfg->pwrCfg.puschPwrFmt3a.size;
   for (rnti = startRnti; (rnti < startRnti + size); ++rnti)
   {
      rgSCHPwrAddRntiToPuschRntiLst(cell, rnti, isFmt3a);
   }

   RETVALUE(ROK);
}

/**
 * @brief Handles power related re-configuration for a cell
 *
 * @details
 *
 *     Function : rgSCHPwrCellRecfg
 *
 *     Processing Steps:
 *     - NONE
 *
 *  @param[in]  RgSchCellCb  *cell
 *  @param[in]  RgrCellRecfg *recfg
 *  @return  S16
 *      -# ROK
 **/
#ifdef ANSI
PUBLIC S16 rgSCHPwrCellRecfg
(
RgSchCellCb          *cell,
RgrCellRecfg         *recfg
)
#else
PUBLIC S16 rgSCHPwrCellRecfg(cell, recfg)
RgSchCellCb          *cell;
RgrCellRecfg         *recfg;
#endif
{
   UNUSED(cell);
   UNUSED(recfg);
   TRC2(rgSCHPwrCellRecfg);

   /* Not doing anything for power reconfig, so such structure
    * in RGR */
   RETVALUE(ROK);
}

/**
 * @brief Frees power related data structures in cell
 *
 * @details
 *
 *     Function : rgSCHPwrCellDel
 *
 *     Processing Steps:
 *      - NONE
 *
 *  @param[in]  RgSchCellCb  *cell
 *  @return  Void
 **/
#ifdef ANSI
PUBLIC Void rgSCHPwrCellDel
(
RgSchCellCb *cell
)
#else
PUBLIC Void rgSCHPwrCellDel(cell)
RgSchCellCb *cell;
#endif
{
   UNUSED(cell);
   TRC2(rgSCHPwrCellDel);

   /* There is no allocated memory, so do nothing */
   RETVOID;
}

/**
 * @brief Handles power related configuration for a UE
 *
 * @details
 *
 *     Function : rgSCHPwrUeCfg
 *
 *     Processing Steps:
 *      - If Pusch group power configuration exists && accumulation enabled,
 *        - Fetch the TPC-Pusch-RNTI control block for the configured
 *        TPC-Pusch-RNTI. Call rgSCHGetRntiFrmPuschRntiLst(). If it does not
 *        exist, return RFAILED.
 *        - Add Ue to the ueLst of TPC-Pusch-RNTI control block.
 *        - Update tpcPuschRntiCb pointer in UE.
 *        - Update the puschIdx value.
 *      - If Pucch group power configuration exists && accumulation enabled,
 *        - Fetch the TPC-Pucch-RNTI control block for the configured
 *        TPC-Pucch-RNTI. Call rgSCHGetRntiFrmPucchRntiLst(). If it does not
 *        exist, return RFAILED.
 *        - Add Ue to the ueLst of TPC-Pucch-RNTI control block.
 *        - Update tpcPucchRntiCb pointer in UE.
 *        - Update the pucchIdx value.
 *      - Update isAccumulated and isDeltaMcs variables.
 *      - maxUlRbs = configured maximum UL bandwidth value per UE.
 *      - trgUlCqi = configured value, if any, else cell-wide default trg CQI value.
 *      - If format type is format 3A, update remaining power to +1
 *      - Update TPC-RNTI information for the cell for Pucch and Pusch.
 *      - Return ROK
 *
 *  @param[in]  RgSchCellCb  *cell
 *  @param[in]  RgSchUeCb    *ue
 *  @param[in]  RgrUeCfg     *cfg
 *  @return  S16
 *      -# ROK
 *      -# RFAILED
 **/
#ifdef ANSI
PUBLIC S16 rgSCHPwrUeCfg
(
RgSchCellCb *cell,
RgSchUeCb   *ue,
RgrUeCfg    *cfg
)
#else
PUBLIC S16 rgSCHPwrUeCfg(cell, ue, cfg)
RgSchCellCb *cell;
RgSchUeCb   *ue;
RgrUeCfg    *cfg;
#endif
{
   S16                    ret;
   RgSchCmnUlPwrCb       *cellPwr = RG_SCH_PWR_GETCELLPWR(cell);
   RgSchCmnUeUlPwrCb     *uePwr = RG_SCH_PWR_GETUEPWR(ue);

   RgSchCmnUlCell *cellUl      = RG_SCH_CMN_GET_UL_CELL(cell);

   TRC2(rgSCHPwrUeCfg);

   uePwr->maxUePwr = cellPwr->pMax;
   uePwr->trgCqi   = cellPwr->trgUlCqi; /* Overriding with UE's happens later */
   uePwr->numRb    = 1;

   uePwr->maxPwrPerRb = uePwr->maxUePwr - rgSchPwrRbToPwrTbl[cellUl->sbSize];

   rgSCHPwrUeResetPucch(cell, ue);
   rgSCHPwrUeResetPusch(cell, ue);

   /* Rest of the vars update and group power control related
    * config happens in the function below */
   ret = rgSCHPwrApplyUePwrCfg(cell, ue, &cfg->ueUlPwrCfg);

   RETVALUE(ret);
}

/**
 * @brief Handles power related re-configuration for a UE
 *
 * @details
 *
 *     Function : rgSCHPwrUeRecfg
 *
 *     Invoking Module Processing:
 *      - This shall be invoked by SCH_GOM at UE re-configuration.
 *        
 *     Processing Steps:
 *      - If change in TPC-RNTI, update the pointer and the TPC RNTI Cb appropriately.
 *      - If accumulation disabled, remove the UE from TPC-RNTI lists of UE, if
 *      it exists.
 *      - If group power configuration disabled, remove the UE from TPC-RNTI lists of UE, if
 *      it exists.
 *
 *  @param[in]  RgSchCellCb  *cell
 *  @param[in]  RgSchUeCb    *ue
 *  @param[in]  RgrUeRecfg   *recfg
 *
 *  @return  S16
 *      -# ROK
 *      -# RFAILED
 **/
#ifdef ANSI
PUBLIC S16 rgSCHPwrUeRecfg
(
RgSchCellCb   *cell,
RgSchUeCb     *ue,
RgrUeRecfg    *recfg
)
#else
PUBLIC S16 rgSCHPwrUeRecfg(cell, ue, recfg)
RgSchCellCb *cell;
RgSchUeCb   *ue;
RgrUeRecfg  *recfg;
#endif
{
   S16                    ret;
   RgSchCmnUeUlPwrCb     *uePwr       = RG_SCH_PWR_GETUEPWR(ue);
   RgrUeUlPwrCfg         *pwrCfg = &recfg->ueUlPwrRecfg;
   TRC2(rgSCHPwrUeRecfg);

   if (pwrCfg->p0UePucch != uePwr->p0UePucch)
   {
      rgSCHPwrUeResetPucch(cell, ue);
   }
   if ((pwrCfg->isAccumulated != uePwr->isAccumulated)
         || (pwrCfg->p0UePusch != uePwr->p0UePusch))
   {
      rgSCHPwrUeResetPusch(cell, ue);
   }
   ret = rgSCHPwrApplyUePwrCfg(cell, ue, &recfg->ueUlPwrRecfg);

   RETVALUE(ret);
}

/***********************************************************
 *
 *     Func : rgSCHPwrApplyUePwrCfg
 *
 *     Desc : Applies power config for UE. Meants to be
 *            used during both power config and reconfig.
 *
 *     Ret  : ROK/RFAILED
 *
 *     Notes:
 *
 *     File :
 *
 **********************************************************/
#ifdef ANSI
PRIVATE S16 rgSCHPwrApplyUePwrCfg 
(
RgSchCellCb          *cell,
RgSchUeCb            *ue,
RgrUeUlPwrCfg        *pwrCfg
)
#else
PRIVATE S16 rgSCHPwrApplyUePwrCfg(cell, ue, pwrCfg)
RgSchCellCb          *cell;
RgSchUeCb            *ue;
RgrUeUlPwrCfg        *pwrCfg;
#endif
{
   S16                    ret;
   RgSchCmnUlUe          *ueUl        = RG_SCH_CMN_GET_UL_UE(ue);
   RgSchCmnUeUlPwrCb     *uePwr       = RG_SCH_PWR_GETUEPWR(ue);
   RgSchCmnTpcRntiCb     *pucchRntiCb = NULLP;
   RgSchCmnTpcRntiCb     *puschRntiCb = NULLP;
   U8                     pucchIdx    = 0;
   U8                     puschIdx    = 0;
   TRC2(rgSCHPwrApplyUePwrCfg);

   /* Validate Pucch group power control config */
   if (pwrCfg->uePucchPwr.pres)
   {
      pucchRntiCb =
         rgSCHPwrGetPucchRntiCb(cell, pwrCfg->uePucchPwr.tpcRnti);
      if (pucchRntiCb == NULLP)
      {
         RETVALUE(RFAILED);
      }
      pucchIdx = pwrCfg->uePucchPwr.idx;
      ret = rgSCHPwrChkPucchTpcRntiIdx(pucchRntiCb, pucchIdx);
      if (ret != ROK)
      {
         RETVALUE(ret);
      }
   }

   /* Validate Pusch group power control config */
   if (pwrCfg->uePuschPwr.pres)
   {
      puschRntiCb =
         rgSCHPwrGetPuschRntiCb(cell, pwrCfg->uePuschPwr.tpcRnti);
      if (puschRntiCb == NULLP)
      {
         RETVALUE(RFAILED);
      }
      puschIdx = pwrCfg->uePuschPwr.idx;
      ret = rgSCHPwrChkPuschTpcRntiIdx(puschRntiCb, puschIdx);
      if (ret != ROK)
      {
         RETVALUE(ret);
      }
   }

   /* Apply Pucch group power control config */
   if (pucchRntiCb)
   {
      if (uePwr->tpcPucchRntiCb != pucchRntiCb) /* This part for recfg */
      {
         if (uePwr->tpcPucchRntiCb)
         {
            rgSCHPwrDelUeFrmPucchTpcRntiCb(cell, uePwr->tpcPucchRntiCb, ue);
         }
         uePwr->tpcPucchRntiCb = pucchRntiCb;
         rgSCHPwrAddUeToPucchTpcRntiCb(cell, pucchRntiCb, ue);
      }
      uePwr->pucchIdx = pucchIdx;
      RLOG_ARG4(L_UNUSED,DBG_CELLID,cell->cellId,
               "<GRP_PWR>PucchRntiCb cfgdUes(%ld %lu %lu) UEID:%d",
               pucchRntiCb->cfgdUes.count,((U32)pucchRntiCb->cfgdUes.first),
               ((U32)pucchRntiCb->cfgdUes.last),ue->ueId);
      RLOG_ARG3(L_UNUSED,DBG_CELLID,cell->cellId,
               "UEID:%d isFmt3a(%u) ueNode(%ld)",
               ue->ueId,pucchRntiCb->isFmt3a,
               pucchRntiCb->schdLnk.node);
      RLOG_ARG4(L_UNUSED,DBG_CELLID,cell->cellId,
               "toBeSchdUes(%ld %lu %lu) tpcRnti(%u)", 
               pucchRntiCb->toBeSchdUes.count, 
               ((U32)pucchRntiCb->toBeSchdUes.first),
               ((U32)pucchRntiCb->toBeSchdUes.last), 
               pucchRntiCb->tpcRnti);
   }

   /* Apply Pusch group power control config */
   if (puschRntiCb)
   {
      if (uePwr->tpcPuschRntiCb != puschRntiCb) /* This part for recfg */
      {
         if (uePwr->tpcPuschRntiCb)
         {
            rgSCHPwrDelUeFrmPuschTpcRntiCb(cell, uePwr->tpcPuschRntiCb, ue);
         }
         uePwr->tpcPuschRntiCb = puschRntiCb;
         rgSCHPwrAddUeToPuschTpcRntiCb(puschRntiCb, ue);
      }
      uePwr->puschIdx = puschIdx;
   }

   /* Update vars */
   uePwr->isAccumulated = pwrCfg->isAccumulated;
   uePwr->deltaMcsEnbld = pwrCfg->isDeltaMCSEnabled;
   if (pwrCfg->trgCqi)
   {
      uePwr->trgCqi = pwrCfg->trgCqi;
   }
   if (ueUl->maxUlCqi < uePwr->trgCqi)
   {
      uePwr->trgCqi = ueUl->maxUlCqi;
   }
   uePwr->p0UePusch = pwrCfg->p0UePusch;
   uePwr->p0UePucch = pwrCfg->p0UePucch;

   RETVALUE(ROK);
}


/**
 * @brief Deletes power related information for UE
 *
 * @details
 *
 *     Function : rgSCHPwrUeDel
 *
 *     Invoking Module Processing:
 *      - This shall be invoked by at the time of UE deletion.
 *        
 *     Processing Steps:
 *     - if (ue->tpcPucchRntiCb)
 *       - delete UE from tpcPucchRntiCb->ueLst
 *       - ue->tpcPucchRntiCb = NULLP
 *       - If in (ue->tpcPucchRntiCb->toBeSchdLst) 
 *         - remove from the list.
 *     - if (ue->tpcPuschRntiCb)
 *       - delete UE from tpcPuschRntiCb->ueLst
 *       - ue->tpcPuschRntiCb = NULLP
 *       - If in (ue->tpcPuschRntiCb->toBeSchdLst) 
 *         - remove from the list.
 *
 *  @param[in]  RgSchCellCb     *cell
 *  @param[in]  RgSchUeCb       *ue
 *  @return  Void
 **/
#ifdef ANSI
PUBLIC Void rgSCHPwrUeDel
(
RgSchCellCb           *cell,
RgSchUeCb             *ue
)
#else
PUBLIC Void rgSCHPwrUeDel(cell, ue)
RgSchCellCb           *cell;
RgSchUeCb             *ue;
#endif
{
   RgSchCmnUeUlPwrCb     *uePwr = RG_SCH_PWR_GETUEPWR(ue);
   TRC2(rgSCHPwrUeDel);

   if (uePwr->tpcPucchRntiCb)
   {
      rgSCHPwrDelUeFrmPucchTpcRntiCb(cell, uePwr->tpcPucchRntiCb, ue);
      uePwr->tpcPucchRntiCb = NULLP;
   }
   if (uePwr->tpcPuschRntiCb)
   {
      rgSCHPwrDelUeFrmPuschTpcRntiCb(cell, uePwr->tpcPuschRntiCb, ue);
      uePwr->tpcPuschRntiCb = NULLP;
   }
   RETVOID;
}

/**
 * @brief Resets UE's power state
 *
 * @details
 *
 *     Function : rgSCHPwrUeReset
 *
 *     Invoking Module Processing:
 *      - This shall be invoked by at the time PDCCH order.
 *
 *     Processing Steps:
 *     - Reset PUSCH power state
 *     - Reset PUCCH power state
 *
 *  @param[in]  RgSchCellCb     *cell
 *  @param[in]  RgSchUeCb       *ue
 *  @return  Void
 **/
#ifdef ANSI
PUBLIC Void rgSCHPwrUeReset
(
RgSchCellCb           *cell,
RgSchUeCb             *ue
)
#else
PUBLIC Void rgSCHPwrUeReset(cell, ue)
RgSchCellCb           *cell;
RgSchUeCb             *ue;
#endif
{
   TRC2(rgSCHPwrUeReset);

   rgSCHPwrUeResetPucch(cell, ue);
   rgSCHPwrUeResetPusch(cell, ue);
   RETVOID;
}

/***********************************************************
 *
 *     Func : rgSCHPwrUeResetPucch
 *
 *     Desc : This function is called to reset UE
 *            to initial PUCCH power state.
 *
 *     Ret  : Void
 *
 *     Notes:
 *
 *     File :
 *
 **********************************************************/
#ifdef ANSI
PRIVATE Void rgSCHPwrUeResetPucch 
(
RgSchCellCb          *cell,
RgSchUeCb            *ue
)
#else
PRIVATE Void rgSCHPwrUeResetPucch(cell, ue)
RgSchCellCb          *cell;
RgSchUeCb            *ue;
#endif
{
   RgSchCmnUeUlPwrCb     *uePwr       = RG_SCH_PWR_GETUEPWR(ue);
   TRC2(rgSCHPwrUeResetPucch);

   uePwr->pucchTpc    = 1;
   uePwr->remPucchPwr = 0;
   if (uePwr->tpcPucchRntiCb)
   {
      rgSCHPwrRmvSchdUeFrmPucchTpcRntiCb(cell, uePwr->tpcPucchRntiCb, ue);
   }

   /* Stack Crash problem for TRACE5 changes. Added the line below */
  RETVOID;
 
}

/***********************************************************
 *
 *     Func : rgSCHPwrUeResetPusch
 *
 *     Desc : This function is called to reset UE
 *            to initial PUSCH power state.
 *
 *     Ret  : Void
 *
 *     Notes:
 *
 *     File :
 *
 **********************************************************/
#ifdef ANSI
PRIVATE Void rgSCHPwrUeResetPusch 
(
RgSchCellCb          *cell,
RgSchUeCb            *ue
)
#else
PRIVATE Void rgSCHPwrUeResetPusch(cell, ue)
RgSchCellCb          *cell;
RgSchUeCb            *ue;
#endif
{
   RgSchCmnUeUlPwrCb     *uePwr       = RG_SCH_PWR_GETUEPWR(ue);
   TRC2(rgSCHPwrUeResetPusch);

   uePwr->isPhrAvail  = FALSE;
   uePwr->phVal       = 40;
   uePwr->maxUlRbs    = RGSCH_MAX_DL_BW;
   uePwr->delta       = 0;
   uePwr->puschTpc    = 1;
   uePwr->remPuschPwr = 0;
   if (uePwr->tpcPuschRntiCb)
   {
      rgSCHPwrRmvSchdUeFrmPuschTpcRntiCb(cell, uePwr->tpcPuschRntiCb, ue);
   }
   RETVOID;
}

/***********************************************************
 *
 *     Func : rgSCHPwrOnPuschPwrUpd
 *
 *     Desc : This function is called whenever 'remPuschPwr'
 *            is updated
 *
 *     Ret  : Void
 *
 *     Notes:
 *
 *     File :
 *
 **********************************************************/
#ifdef ANSI
PRIVATE Void rgSCHPwrOnPuschPwrUpd 
(
RgSchCellCb          *cell,
RgSchUeCb            *ue
)
#else
PRIVATE Void rgSCHPwrOnPuschPwrUpd(cell, ue)
RgSchCellCb          *cell;
RgSchUeCb            *ue;
#endif
{
   RgSchCmnUeUlPwrCb     *uePwr = RG_SCH_PWR_GETUEPWR(ue);
   RgSchCmnTpcRntiCb     *cb;
   Bool                   toAdd;
   TRC2(rgSCHPwrOnPuschPwrUpd);

   if ((cb = uePwr->tpcPuschRntiCb) == NULLP)
   {
      RETVOID;
   }

   /* Not checking for uwPwr->isPhrAvail as uePwr->phVal
    * is set to a large value initially */
   toAdd = FALSE;


   if ((uePwr->phVal != 0) && (uePwr->remPuschPwr != 0))
      {
         toAdd = TRUE;
      }


   if (toAdd)
   {
      rgSCHPwrAddSchdUeToPuschTpcRntiCb(cell, cb, ue);
   }
   else
   {
      rgSCHPwrRmvSchdUeFrmPuschTpcRntiCb(cell, cb, ue);
   }

   RETVOID;
}


/***********************************************************
 *
 *     Func : rgSCHPwrAddRntiToPucchRntiLst
 *
 *
 *     Desc : Adds RNTI to Pucch Rnti list and updates requisite
 *            information. 
 *
 *     Ret  : Void
 *
 *     Notes: Assumed that array bounds are checked
 *            in caller before adding.
 *
 *     File :
 *
 **********************************************************/
#ifdef ANSI
PRIVATE Void rgSCHPwrAddRntiToPucchRntiLst 
(
RgSchCellCb          *cell,
CmLteRnti             rnti,
Bool                  isFmt3a
)
#else
PRIVATE Void rgSCHPwrAddRntiToPucchRntiLst(cell, rnti, isFmt3a)
RgSchCellCb          *cell;
CmLteRnti             rnti;
Bool                  isFmt3a;
#endif
{
   RgSchCmnUlPwrCb       *cellPwr = RG_SCH_PWR_GETCELLPWR(cell);
   TRC2(rgSCHPwrAddRntiToPucchRntiLst);

   rgSCHPwrInitTpcRntiCb(&cellPwr->tpcPucchRntiLst[cellPwr->tpcPucchRntiCnt++],
       rnti, isFmt3a);
   RETVOID;
}

/***********************************************************
 *
 *     Func : rgSCHPwrAddRntiToPuschRntiLst
 *
 *
 *     Desc : Adds RNTI to Pusch Rnti list and updates requisite
 *            information. 
 *
 *     Ret  : Void
 *
 *     Notes: Assumed that array bounds are checked
 *            in caller before adding.
 *
 *     File :
 *
 **********************************************************/
#ifdef ANSI
PRIVATE Void rgSCHPwrAddRntiToPuschRntiLst 
(
RgSchCellCb          *cell,
CmLteRnti             rnti,
Bool                  isFmt3a
)
#else
PRIVATE Void rgSCHPwrAddRntiToPuschRntiLst(cell, rnti, isFmt3a)
RgSchCellCb          *cell;
CmLteRnti             rnti;
Bool                  isFmt3a;
#endif
{
   RgSchCmnUlPwrCb       *cellPwr = RG_SCH_PWR_GETCELLPWR(cell);
   TRC2(rgSCHPwrAddRntiToPuschRntiLst);

   rgSCHPwrInitTpcRntiCb(&cellPwr->tpcPuschRntiLst[cellPwr->tpcPuschRntiCnt++], 
       rnti, isFmt3a);
   RETVOID;
}

/***********************************************************
 *
 *     Func : rgSCHPwrInitTpcRntiCb
 *
 *
 *     Desc : Initialises a TPC RNTI CB
 *
 *     Ret  : Void
 *
 *     Notes:
 *
 *     File :
 *
 **********************************************************/
#ifdef ANSI
PRIVATE Void rgSCHPwrInitTpcRntiCb 
(
RgSchCmnTpcRntiCb    *cb,
CmLteRnti             rnti,
Bool                  isFmt3a
)
#else
PRIVATE Void rgSCHPwrInitTpcRntiCb(cb, rnti, isFmt3a)
RgSchCmnTpcRntiCb    *cb;
CmLteRnti             rnti;
Bool                  isFmt3a;
#endif
{
   TRC2(rgSCHPwrInitTpcRntiCb);

   cmMemset((U8 *)cb, 0, sizeof(*cb));
   cb->tpcRnti = rnti;
   cb->isFmt3a = isFmt3a;
   /* Not initialising lists as memset 0 takes care of it */
   /* cb->schdLnk.node is set when this rnti is to be scheduled */
   RETVOID;
}

/***********************************************************
 *
 *     Func : rgSCHPwrGetPucchRntiCb
 *
 *
 *     Desc : Gets TPC RNTI control block from Pucch rnti list 
 *
 *     Ret  : RgSchCmnTpcRntiCb * - Success
 *            NULLP - Fail
 *
 *     Notes:
 *
 *     File :
 *
 **********************************************************/
#ifdef ANSI
PRIVATE RgSchCmnTpcRntiCb* rgSCHPwrGetPucchRntiCb
(
RgSchCellCb *cell,
CmLteRnti   tpcRnti
)
#else
PRIVATE RgSchCmnTpcRntiCb* rgSCHPwrGetPucchRntiCb(cell, tpcRnti)
RgSchCellCb *cell;
CmLteRnti   tpcRnti;
#endif
{
   RgSchCmnUlPwrCb       *cellPwr = RG_SCH_PWR_GETCELLPWR(cell);
   U16                    idx;
   TRC2(rgSCHPwrGetPucchRntiCb);

   if (!cellPwr->tpcPucchRntiCnt)
   {
      RETVALUE(NULLP);
   }
   for (idx = 0; idx < cellPwr->tpcPucchRntiCnt; ++idx)
   {
      if (cellPwr->tpcPucchRntiLst[idx].tpcRnti == tpcRnti)
      {
         RETVALUE(&cellPwr->tpcPucchRntiLst[idx]);
      }
   }
   RETVALUE(NULLP);
}

/***********************************************************
 *
 *     Func : rgSCHPwrGetPuschRntiCb
 *
 *
 *     Desc : Gets TPC RNTI control block from Pusch rnti list 
 *
 *     Ret  : RgSchCmnTpcRntiCb * - Success
 *            NULLP - Fail
 *
 *     Notes:
 *
 *     File :
 *
 **********************************************************/
#ifdef ANSI
PRIVATE RgSchCmnTpcRntiCb* rgSCHPwrGetPuschRntiCb
(
RgSchCellCb *cell,
CmLteRnti   tpcRnti
)
#else
PRIVATE RgSchCmnTpcRntiCb* rgSCHPwrGetPuschRntiCb(cell, tpcRnti)
RgSchCellCb *cell;
CmLteRnti   tpcRnti;
#endif
{
   RgSchCmnUlPwrCb       *cellPwr = RG_SCH_PWR_GETCELLPWR(cell);
   U16                    idx;
   TRC2(rgSCHPwrGetPuschRntiCb);

   if (!cellPwr->tpcPuschRntiCnt)
   {
      RETVALUE(NULLP);
   }
   for (idx = 0; idx < cellPwr->tpcPuschRntiCnt; ++idx)
   {
      if (cellPwr->tpcPuschRntiLst[idx].tpcRnti == tpcRnti)
      {
         RETVALUE(&cellPwr->tpcPuschRntiLst[idx]);
      }
   }
   RETVALUE(NULLP);
}


/***********************************************************
 *
 *     Func : rgSCHPwrAddUeToPucchTpcRntiCb
 *
 *
 *     Desc : Add UE to cfgd list of UEs in rnti cb
 *
 *     Ret  :
 *
 *     Notes:
 *
 *     File :
 *
 **********************************************************/
#ifdef ANSI
PRIVATE Void rgSCHPwrAddUeToPucchTpcRntiCb
(
RgSchCellCb           *cell,
RgSchCmnTpcRntiCb     *cb,
RgSchUeCb             *ue
)
#else
PRIVATE Void rgSCHPwrAddUeToPucchTpcRntiCb(cell, cb, ue)
RgSchCellCb           *cell;
RgSchCmnTpcRntiCb     *cb;
RgSchUeCb             *ue;
#endif
{
   RgSchCmnUeUlPwrCb     *uePwr = RG_SCH_PWR_GETUEPWR(ue);
   UNUSED(cell);
   TRC2(rgSCHPwrAddUeToPucchTpcRntiCb);

   cmLListAdd2Tail(&cb->cfgdUes, &uePwr->pucchGrpLnk);
   uePwr->pucchGrpLnk.node = (PTR)ue;
   RETVOID;
}

/***********************************************************
 *
 *     Func : rgSCHPwrDelUeFrmPucchTpcRntiCb
 *
 *
 *     Desc : Remove UE from Pucch TPC RNTI CB
 *
 *     Ret  :
 *
 *     Notes:
 *
 *     File :
 *
 **********************************************************/
#ifdef ANSI
PRIVATE Void rgSCHPwrDelUeFrmPucchTpcRntiCb
(
RgSchCellCb           *cell,
RgSchCmnTpcRntiCb     *cb,
RgSchUeCb             *ue
)
#else
PRIVATE Void rgSCHPwrDelUeFrmPucchTpcRntiCb(cell, cb, ue)
RgSchCellCb           *cell;
RgSchCmnTpcRntiCb     *cb;
RgSchUeCb             *ue;
#endif
{
   RgSchCmnUeUlPwrCb     *uePwr = RG_SCH_PWR_GETUEPWR(ue);
   TRC2(rgSCHPwrDelUeFrmPucchTpcRntiCb);

   rgSCHPwrRmvSchdUeFrmPucchTpcRntiCb(cell, cb, ue);
   cmLListDelFrm(&cb->cfgdUes, &uePwr->pucchGrpLnk);
   uePwr->pucchGrpLnk.node = NULLP;
   RETVOID;
}

/***********************************************************
 *
 *     Func : rgSCHPwrRmvSchdUeFrmPucchTpcRntiCb
 *
 *
 *     Desc : Remove UE from to-be-scheduled list of UEs
 *            in Pusch RNTI CB
 *
 *     Ret  :
 *
 *     Notes:
 *
 *     File :
 *
 **********************************************************/
#ifdef ANSI
PRIVATE Void rgSCHPwrRmvSchdUeFrmPucchTpcRntiCb
(
RgSchCellCb           *cell,
RgSchCmnTpcRntiCb     *cb,
RgSchUeCb             *ue
)
#else
PRIVATE Void rgSCHPwrRmvSchdUeFrmPucchTpcRntiCb(cell, cb, ue)
RgSchCellCb           *cell;
RgSchCmnTpcRntiCb     *cb;
RgSchUeCb             *ue;
#endif
{
   RgSchCmnUeUlPwrCb     *uePwr = RG_SCH_PWR_GETUEPWR(ue);
   TRC2(rgSCHPwrRmvSchdUeFrmPucchTpcRntiCb);

   if (uePwr->schdPucchGrpLnk.node == NULLP)
   {
      RETVOID;
   }
   rgSCHPwrRmvSchdUeOnlyFrmPucchTpcRntiCb(cell, cb, ue);
   if (!cb->toBeSchdUes.count)
   {
      rgSCHPwrRmvSchdPucchTpcRntiCb(cell, cb);
   }
   RETVOID;
}

/***********************************************************
 *
 *     Func : rgSCHPwrRmvSchdUeOnlyFrmPucchTpcRntiCb
 *
 *     Desc : Remove UE from to-be-scheduled list of UEs
 *            in Pucch RNTI CB. Do not both about
 *            possibly removing Pucch RNTI CB from
 *            the cell wide to-be-scheduled list.
 *
 *     Ret  :
 *
 *     Notes:
 *
 *     File :
 *
 **********************************************************/
#ifdef ANSI
PRIVATE Void rgSCHPwrRmvSchdUeOnlyFrmPucchTpcRntiCb
(
RgSchCellCb           *cell,
RgSchCmnTpcRntiCb     *cb,
RgSchUeCb             *ue
)
#else
PRIVATE Void rgSCHPwrRmvSchdUeOnlyFrmPucchTpcRntiCb(cell, cb, ue)
RgSchCellCb           *cell;
RgSchCmnTpcRntiCb     *cb;
RgSchUeCb             *ue;
#endif
{
   RgSchCmnUeUlPwrCb     *uePwr = RG_SCH_PWR_GETUEPWR(ue);
   TRC2(rgSCHPwrRmvSchdUeOnlyFrmPucchTpcRntiCb);

   if (uePwr->schdPucchGrpLnk.node != NULLP)
   {
      cmLListDelFrm(&cb->toBeSchdUes, &uePwr->schdPucchGrpLnk);
      uePwr->schdPucchGrpLnk.node = NULLP;
   }
   RETVOID;
}

/***********************************************************
 *
 *     Func : rgSCHPwrRmvSchdPucchTpcRntiCb
 *
 *     Desc : Remove Pucch TPC RNTI CB from to-be-scheduled
 *            list in cell
 *
 *     Ret  : Void
 *
 *     Notes:
 *
 *     File :
 *
 **********************************************************/
#ifdef ANSI
PRIVATE Void rgSCHPwrRmvSchdPucchTpcRntiCb
(
RgSchCellCb           *cell,
RgSchCmnTpcRntiCb     *cb
)
#else
PRIVATE Void rgSCHPwrRmvSchdPucchTpcRntiCb(cell, cb)
RgSchCellCb           *cell;
RgSchCmnTpcRntiCb     *cb;
#endif
{
   RgSchCmnUlPwrCb       *cellPwr = RG_SCH_PWR_GETCELLPWR(cell);
   TRC2(rgSCHPwrRmvSchdPucchTpcRntiCb);

   if (cb->schdLnk.node == NULLP)
   {
      RETVOID;
   }
   cmLListDelFrm(&cellPwr->pucchGrpPwr, &cb->schdLnk);
   cb->schdLnk.node = NULLP;
   RETVOID;
}

/***********************************************************
 *
 *     Func : rgSCHPwrAddSchdUeToPucchTpcRntiCb
 *
 *     Desc : Add UE to to-be-scheduled list of UEs
 *            in Pucch RNTI CB
 *
 *     Ret  :
 *
 *     Notes:
 *
 *     File :
 *
 **********************************************************/
#ifdef ANSI
PRIVATE Void rgSCHPwrAddSchdUeToPucchTpcRntiCb
(
RgSchCellCb           *cell,
RgSchCmnTpcRntiCb     *cb,
RgSchUeCb             *ue
)
#else
PRIVATE Void rgSCHPwrAddSchdUeToPucchTpcRntiCb(cell, cb, ue)
RgSchCellCb           *cell;
RgSchCmnTpcRntiCb     *cb;
RgSchUeCb             *ue;
#endif
{
   RgSchCmnUeUlPwrCb     *uePwr = RG_SCH_PWR_GETUEPWR(ue);
   TRC2(rgSCHPwrAddSchdUeToPucchTpcRntiCb);

   if (uePwr->schdPucchGrpLnk.node != NULLP)
   {
      /* UE is already in the list */
      RETVOID;
   }
   cmLListAdd2Tail(&cb->toBeSchdUes, &uePwr->schdPucchGrpLnk);
   uePwr->schdPucchGrpLnk.node = (PTR)ue;
   if (cb->toBeSchdUes.count == 1)
   {
      /* This is first UE, so queue up this TPC RNTI
       * for scheduling */
      rgSCHPwrAddSchdPucchTpcRntiCb(cell, cb);
   }
   RETVOID;
}

/***********************************************************
 *
 *     Func : rgSCHPwrAddSchdPucchTpcRntiCb
 *
 *     Desc : Add Pucch TPC RNTI CB from to-be-scheduled
 *            list in cell
 *
 *     Ret  : Void
 *
 *     Notes:
 *
 *     File :
 *
 **********************************************************/
#ifdef ANSI
PRIVATE Void rgSCHPwrAddSchdPucchTpcRntiCb
(
RgSchCellCb           *cell,
RgSchCmnTpcRntiCb     *cb
)
#else
PRIVATE Void rgSCHPwrAddSchdPucchTpcRntiCb(cell, cb)
RgSchCellCb           *cell;
RgSchCmnTpcRntiCb     *cb;
#endif
{
   RgSchCmnUlPwrCb       *cellPwr = RG_SCH_PWR_GETCELLPWR(cell);
   TRC2(rgSCHPwrAddSchdPucchTpcRntiCb);

   cmLListAdd2Tail(&cellPwr->pucchGrpPwr, &cb->schdLnk);
   cb->schdLnk.node = (PTR)cb;
   RETVOID;
}


/***********************************************************
 *
 *     Func : rgSCHPwrAddUeToPuschTpcRntiCb
 *
 *
 *     Desc : Add UE to cfgd list of UEs in rnti cb
 *
 *     Ret  :
 *
 *     Notes:
 *
 *     File :
 *
 **********************************************************/
#ifdef ANSI
PRIVATE Void rgSCHPwrAddUeToPuschTpcRntiCb
(
RgSchCmnTpcRntiCb     *cb,
RgSchUeCb             *ue
)
#else
PRIVATE Void rgSCHPwrAddUeToPuschTpcRntiCb(cb, ue)
RgSchCmnTpcRntiCb     *cb;
RgSchUeCb             *ue;
#endif
{
   RgSchCmnUeUlPwrCb     *uePwr = RG_SCH_PWR_GETUEPWR(ue);
   TRC2(rgSCHPwrAddUeToPuschTpcRntiCb);

   cmLListAdd2Tail(&cb->cfgdUes, &uePwr->puschGrpLnk);
   uePwr->puschGrpLnk.node = (PTR)ue;
   RETVOID;
}

/***********************************************************
 *
 *     Func : rgSCHPwrAddSchdUeToPuschTpcRntiCb
 *
 *     Desc : Add UE to to-be-scheduled list of UEs
 *            in Pusch RNTI CB
 *
 *     Ret  :
 *
 *     Notes:
 *
 *     File :
 *
 **********************************************************/
#ifdef ANSI
PRIVATE Void rgSCHPwrAddSchdUeToPuschTpcRntiCb
(
RgSchCellCb           *cell,
RgSchCmnTpcRntiCb     *cb,
RgSchUeCb             *ue
)
#else
PRIVATE Void rgSCHPwrAddSchdUeToPuschTpcRntiCb(cell, cb, ue)
RgSchCellCb           *cell;
RgSchCmnTpcRntiCb     *cb;
RgSchUeCb             *ue;
#endif
{
   RgSchCmnUeUlPwrCb     *uePwr = RG_SCH_PWR_GETUEPWR(ue);
   TRC2(rgSCHPwrAddSchdUeToPuschTpcRntiCb);

   if (uePwr->schdPuschGrpLnk.node != NULLP)
   {
      /* UE is already in the list */
      RETVOID;
   }
   cmLListAdd2Tail(&cb->toBeSchdUes, &uePwr->schdPuschGrpLnk);
   uePwr->schdPuschGrpLnk.node = (PTR)ue;
   if (cb->toBeSchdUes.count == 1)
   {
      /* This is first UE, so queue up this TPC RNTI
       * for scheduling */
      rgSCHPwrAddSchdPuschTpcRntiCb(cell, cb);
   }
   RETVOID;
}

/***********************************************************
 *
 *     Func : rgSCHPwrDelUeFrmPuschTpcRntiCb
 *
 *
 *     Desc : Add UE to cfgd list of UEs in rnti cb
 *
 *     Ret  :
 *
 *     Notes:
 *
 *     File :
 *
 **********************************************************/
#ifdef ANSI
PRIVATE Void rgSCHPwrDelUeFrmPuschTpcRntiCb
(
RgSchCellCb           *cell,
RgSchCmnTpcRntiCb     *cb,
RgSchUeCb             *ue
)
#else
PRIVATE Void rgSCHPwrDelUeFrmPuschTpcRntiCb(cell, cb, ue)
RgSchCellCb           *cell;
RgSchCmnTpcRntiCb     *cb;
RgSchUeCb             *ue;
#endif
{
   RgSchCmnUeUlPwrCb     *uePwr = RG_SCH_PWR_GETUEPWR(ue);
   TRC2(rgSCHPwrDelUeFrmPuschTpcRntiCb);

   rgSCHPwrRmvSchdUeFrmPuschTpcRntiCb(cell, cb, ue);
   cmLListDelFrm(&cb->cfgdUes, &uePwr->puschGrpLnk);
   uePwr->puschGrpLnk.node = NULLP;
   RETVOID;
}

/***********************************************************
 *
 *     Func : rgSCHPwrRmvSchdUeFrmPuschTpcRntiCb
 *
 *     Desc : Remove UE from to-be-scheduled list of UEs
 *            in Pusch RNTI CB
 *
 *     Ret  :
 *
 *     Notes:
 *
 *     File :
 *
 **********************************************************/
#ifdef ANSI
PRIVATE Void rgSCHPwrRmvSchdUeFrmPuschTpcRntiCb
(
RgSchCellCb           *cell,
RgSchCmnTpcRntiCb     *cb,
RgSchUeCb             *ue
)
#else
PRIVATE Void rgSCHPwrRmvSchdUeFrmPuschTpcRntiCb(cell, cb, ue)
RgSchCellCb           *cell;
RgSchCmnTpcRntiCb     *cb;
RgSchUeCb             *ue;
#endif
{
   RgSchCmnUeUlPwrCb     *uePwr       = RG_SCH_PWR_GETUEPWR(ue);
   TRC2(rgSCHPwrRmvSchdUeFrmPuschTpcRntiCb);

   if (uePwr->schdPuschGrpLnk.node == NULLP)
   {
      RETVOID;
   }
   rgSCHPwrRmvSchdUeOnlyFrmPuschTpcRntiCb(cell, cb, ue);
   if (!cb->toBeSchdUes.count)
   {
      rgSCHPwrRmvSchdPuschTpcRntiCb(cell, cb);
   }
   RETVOID;
}

/***********************************************************
 *
 *     Func : rgSCHPwrRmvSchdUeOnlyFrmPuschTpcRntiCb
 *
 *     Desc : Remove UE from to-be-scheduled list of UEs
 *            in Pusch RNTI CB. Do not both about
 *            possibly removing Pusch RNTI CB from
 *            the cell wide to-be-scheduled list.
 *
 *     Ret  :
 *
 *     Notes:
 *
 *     File :
 *
 **********************************************************/
#ifdef ANSI
PRIVATE Void rgSCHPwrRmvSchdUeOnlyFrmPuschTpcRntiCb
(
RgSchCellCb           *cell,
RgSchCmnTpcRntiCb     *cb,
RgSchUeCb             *ue
)
#else
PRIVATE Void rgSCHPwrRmvSchdUeOnlyFrmPuschTpcRntiCb(cell, cb, ue)
RgSchCellCb           *cell;
RgSchCmnTpcRntiCb     *cb;
RgSchUeCb             *ue;
#endif
{
   RgSchCmnUeUlPwrCb     *uePwr = RG_SCH_PWR_GETUEPWR(ue);
   TRC2(rgSCHPwrRmvSchdUeOnlyFrmPuschTpcRntiCb);

   if (uePwr->schdPuschGrpLnk.node != NULLP)
   {
      cmLListDelFrm(&cb->toBeSchdUes, &uePwr->schdPuschGrpLnk);
      uePwr->schdPuschGrpLnk.node = NULLP;
   }
   RETVOID;
}

/***********************************************************
 *
 *     Func : rgSCHPwrAddSchdPuschTpcRntiCb
 *
 *     Desc : Add Pusch TPC RNTI CB from to-be-scheduled
 *            list in cell
 *
 *     Ret  : Void
 *
 *     Notes:
 *
 *     File :
 *
 **********************************************************/
#ifdef ANSI
PRIVATE Void rgSCHPwrAddSchdPuschTpcRntiCb
(
RgSchCellCb           *cell,
RgSchCmnTpcRntiCb     *cb
)
#else
PRIVATE Void rgSCHPwrAddSchdPuschTpcRntiCb(cell, cb)
RgSchCellCb           *cell;
RgSchCmnTpcRntiCb     *cb;
#endif
{
   RgSchCmnUlPwrCb       *cellPwr = RG_SCH_PWR_GETCELLPWR(cell);
   TRC2(rgSCHPwrAddSchdPuschTpcRntiCb);

   cmLListAdd2Tail(&cellPwr->puschGrpPwr, &cb->schdLnk);
   cb->schdLnk.node = (PTR)cb;
   RETVOID;
}

/***********************************************************
 *
 *     Func : rgSCHPwrRmvSchdPuschTpcRntiCb
 *
 *     Desc : Remove Pusch TPC RNTI CB from to-be-scheduled
 *            list in cell
 *
 *     Ret  : Void
 *
 *     Notes:
 *
 *     File :
 *
 **********************************************************/
#ifdef ANSI
PRIVATE Void rgSCHPwrRmvSchdPuschTpcRntiCb
(
RgSchCellCb           *cell,
RgSchCmnTpcRntiCb     *cb
)
#else
PRIVATE Void rgSCHPwrRmvSchdPuschTpcRntiCb(cell, cb)
RgSchCellCb           *cell;
RgSchCmnTpcRntiCb     *cb;
#endif
{
   RgSchCmnUlPwrCb       *cellPwr = RG_SCH_PWR_GETCELLPWR(cell);
   TRC2(rgSCHPwrRmvSchdPuschTpcRntiCb);

   if (cb->schdLnk.node == NULLP)
   {
      RETVOID;
   }
   cmLListDelFrm(&cellPwr->puschGrpPwr, &cb->schdLnk);
   cb->schdLnk.node = NULLP;
   RETVOID;
}

/***********************************************************
 *
 *     Func : rgSCHPwrChkPucchTpcRntiIdx
 *
 *     Desc : Validate that the given index is OK to
 *            be assigned to a new UE for the Pucch TPC
 *            RNTI
 *
 *     Ret  :
 *
 *     Notes:
 *
 *     File :
 *
 **********************************************************/
#ifdef ANSI
PRIVATE S16 rgSCHPwrChkPucchTpcRntiIdx
(
RgSchCmnTpcRntiCb     *cb,
U8                     idx
)
#else
PRIVATE S16 rgSCHPwrChkPucchTpcRntiIdx(cb, idx)
RgSchCmnTpcRntiCb     *cb;
U8                     idx;
#endif
{
   TRC2(rgSCHPwrChkPucchTpcRntiIdx);

   if (rgSCHPwrChkTpcRntiIdx(cb, idx) != ROK)
   {
      RETVALUE(RFAILED);
   }
   if (rgSCHPwrChkUniqPucchTpcRntiIdx(cb, idx) != ROK)
   {
      RETVALUE(RFAILED);
   }
   RETVALUE(ROK);
}

/***********************************************************
 *
 *     Func : rgSCHPwrChkPuschTpcRntiIdx
 *
 *     Desc : Validate that the given index is OK to
 *            be assigned to a new UE for the Pusch TPC
 *            RNTI
 *
 *     Ret  :
 *
 *     Notes:
 *
 *     File :
 *
 **********************************************************/
#ifdef ANSI
PRIVATE S16 rgSCHPwrChkPuschTpcRntiIdx
(
RgSchCmnTpcRntiCb     *cb,
U8                     idx
)
#else
PRIVATE S16 rgSCHPwrChkPuschTpcRntiIdx(cb, idx)
RgSchCmnTpcRntiCb     *cb;
U8                     idx;
#endif
{
   TRC2(rgSCHPwrChkPuschTpcRntiIdx);

   if (rgSCHPwrChkTpcRntiIdx(cb, idx) != ROK)
   {
      RETVALUE(RFAILED);
   }
   if (rgSCHPwrChkUniqPuschTpcRntiIdx(cb, idx) != ROK)
   {
      RETVALUE(RFAILED);
   }
   RETVALUE(ROK);
}

/***********************************************************
 *
 *     Func : rgSCHPwrChkUniqPucchTpcRntiIdx
 *
 *     Desc : Validate index against format type of TPC RNTI
 *
 *     Ret  :
 *
 *     Notes:
 *
 *     File :
 *
 **********************************************************/
#ifdef ANSI
PRIVATE S16 rgSCHPwrChkUniqPucchTpcRntiIdx
(
RgSchCmnTpcRntiCb     *cb,
U8                     idx
)
#else
PRIVATE S16 rgSCHPwrChkUniqPucchTpcRntiIdx(cb, idx)
RgSchCmnTpcRntiCb     *cb;
U8                     idx;
#endif
{
   CmLList           *lnk;
   TRC2(rgSCHPwrChkUniqPucchTpcRntiIdx);

   for (lnk = cb->cfgdUes.first; lnk; lnk = lnk->next)
   {
      RgSchUeCb             *ue = (RgSchUeCb *)lnk->node;
      RgSchCmnUeUlPwrCb     *uePwr = RG_SCH_PWR_GETUEPWR(ue);
      if (uePwr->pucchIdx == idx)
      {
         RETVALUE(RFAILED);
      }
   }
   RETVALUE(ROK);
}

/***********************************************************
 *
 *     Func : rgSCHPwrChkUniqPuschTpcRntiIdx
 *
 *     Desc : Validate index against format type of TPC RNTI
 *
 *     Ret  :
 *
 *     Notes:
 *
 *     File :
 *
 **********************************************************/
#ifdef ANSI
PRIVATE S16 rgSCHPwrChkUniqPuschTpcRntiIdx
(
RgSchCmnTpcRntiCb     *cb,
U8                     idx
)
#else
PRIVATE S16 rgSCHPwrChkUniqPuschTpcRntiIdx(cb, idx)
RgSchCmnTpcRntiCb     *cb;
U8                     idx;
#endif
{
   CmLList           *lnk;
   TRC2(rgSCHPwrChkUniqPuschTpcRntiIdx);

   for (lnk = cb->cfgdUes.first; lnk; lnk = lnk->next)
   {
      RgSchUeCb             *ue = (RgSchUeCb *)lnk->node;
      RgSchCmnUeUlPwrCb     *uePwr = RG_SCH_PWR_GETUEPWR(ue);
      if (uePwr->puschIdx == idx)
      {
         RETVALUE(RFAILED);
      }
   }
   RETVALUE(ROK);
}

/***********************************************************
 *
 *     Func : rgSCHPwrChkTpcRntiIdx
 *
 *     Desc : Validate index against format type of TPC RNTI.
 *
 *     Ret  :
 *
 *     Notes:
 *
 *     File :
 *
 **********************************************************/
#ifdef ANSI
PRIVATE S16 rgSCHPwrChkTpcRntiIdx
(
RgSchCmnTpcRntiCb     *cb,
U8                     idx
)
#else
PRIVATE S16 rgSCHPwrChkTpcRntiIdx(cb, idx)
RgSchCmnTpcRntiCb     *cb;
U8                     idx;
#endif
{
   TRC2(rgSCHPwrChkTpcRntiIdx);

   if (cb->isFmt3a)
   {
      if (idx >= TFU_MAX_1BIT_TPC)
      {
         RETVALUE(RFAILED);
      }
   }
   else
   {
      if (idx >= TFU_MAX_2BIT_TPC)
      {
         RETVALUE(RFAILED);
      }
   }
   RETVALUE(ROK);
}
/* Warning Fix: Commenting out as not used */

/***********************************************************
 *
 *     Func : rgSCHPwrGetPhValFromPhr
 *
 *     Desc : Returns the power headroom in dB
 *            corresponding to a power headroom
 *            report
 *
 *     Ret  : S8
 *
 *     Notes:
 *
 *     File :
 *
 **********************************************************/
#ifdef ANSI
PRIVATE S8 rgSCHPwrGetPhValFromPhr
(
U8                    phr
)
#else
PRIVATE S8 rgSCHPwrGetPhValFromPhr(phr)
U8                    phr;
#endif
{
   TRC2(rgSCHPwrGetPhValFromPhr);
   RETVALUE((phr & 63) - 23);
}



/**********************************************************************
 
         End of file:     gk_sch_pwr.c@@/main/2 - Sat Jul 30 02:21:47 2011
 
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
/main/1      ---     sd   1. LTE MAC 2.1 release
             rg001.201 sm 1. Removed extra comments.
             rg003.201 ns 1. Trace added to functions.
             rg005.201 sm 1. Removed unused linked list
           rg007.201   ap  1. Added support for MIMO
           rg008.201   sd  1. Removed dependency on MIMO compile-time flag
           rg009.201   dv  1. Added Changes of TFU_UPGRADE. 
/main/2      ---        gvj  1. Updated for LTE MAC Rel 3.1           
/main/2    rg001.301  nudupi  1. Updated to fix the correction pertaining 
                                to the CR ccpu00118788
           rg002.301  dvasisht 1. ccpu00119916: Return 0th index of 
                                 rgSchPwrToRbTbl when pwr <=0    
                               2.ccpu00118423 fix for rb alloced exceeding
                                 max limit
$SID$        ---      rt   1. LTE MAC 4.1 release
*********************************************************************91*/
