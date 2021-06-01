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

     Desc:     C source code for Round Robin functions

     File:     rg_sch_rr.c

     Sid:      gk_sch_rr.c@@/main/2 - Sat Jul 30 02:21:48 2011

     Prg:     rr

**********************************************************************/

/** @file rg_sch_rr.c
@brief This module handles the round robin scheduler functionality
*/

static const char* RLOG_MODULE_NAME="MAC";
static int RLOG_MODULE_ID=4096;
static int RLOG_FILE_ID=172;

/* header include files -- defines (.h) */
#include "envopt.h"        /* environment options */
#include "envdep.h"        /* environment dependent */
#include "envind.h"        /* environment independent */
#include "gen.h"           /* general layer */
#include "ssi.h"           /* system service interface */
#include "cm5.h"           /* common timers */
#include "cm_hash.h"       /* common hash list */
#include "cm_llist.h"      /* common linked list library */
#include "cm_err.h"        /* common error */
#include "cm_lte.h"        /* common LTE */
#include "lrg.h"
#include "rgr.h"
#include "rgm.h"
#include "tfu.h"
#include "rg_env.h"
#include "rg_sch_inf.h"
#include "rg_sch_err.h"
#include "rg_sch.h"
#include "rg_sch_cmn.h"
#include "rg_sch_rr.h"
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
#include "cm_lte.x"        /* common tokens */
#include "tfu.x"           /* RGU types */
#include "lrg.x"           /* layer management typedefs for MAC */
#include "rgr.x"           /* layer management typedefs for MAC */
#include "rgm.x"           /* layer management typedefs for MAC */
#include "rg_sch_inf.x"    /* typedefs for Scheduler */
#include "rg_sch.x"        /* typedefs for Scheduler */
#include "rg_sch_cmn.x"
#include "rg_sch_clist.x"        /* typedefs for RR Scheduler */
#include "rg_sch_rr.x"        /* typedefs for RR Scheduler */

/* local defines */
PRIVATE Void rgSCHRrRlsDlHqProc ARGS(( RgSchDlHqProcCb    *proc ));

/* LTE_ADV_FLAG_REMOVED_START */
PRIVATE Void rgSCHRrDlRetxOfPrioritizedUe ARGS(( RgSchCellCb            *cell,
                                                 RgSchCmnDlRbAllocInfo  *allocInfo,
                                                 U8                     feautre,
                                                 Bool                   vctmPrior));
PRIVATE Void rgSCHRrDlNewTxOfPrioritizedUe ARGS(( RgSchCellCb            *cell,
                                                  RgSchCmnDlRbAllocInfo  *allocInfo,
                                                  U8                     feautre,
                                                  Bool                   vctmPrior));
/* LTE_ADV_FLAG_REMOVED_END */

PRIVATE Void rgSCHRrDlScheduleUEs ARGS((
                                            RgSchCellCb           *cell,
                                            RgSchDlSf             *sf,
                                            RgSchCmnDlRbAllocInfo *allocInfo));

PRIVATE Void rgSCHRrDlAddUeToSchdLst ARGS(( RgSchRrCListCp *lcp,
                                            RgSchUeCb      *ueToBeInsrtd));

PUBLIC Void rgSCHRrDlScanUpdPdbPrio ARGS((RgSchCellCb *cell ));
PUBLIC S16 rgSCHRrDlFillFlowCntrlInfo ARGS(( RgSchCellCb    *cell,
                                             RgInfSfAlloc   *sfAlloc));
PRIVATE Void rgSCHRrDlUpdateSchedList ARGS(( RgSchCellCb     *cell,
                                             RgSchRrDlCell   *rrDlCell,
                                             RgSchDlSf       *dlSf,
                                             CmLteTimingInfo schdTime,
                                             RgSchRrCListCp  *txListCp));
PRIVATE Void rgSCHRrDlAddUeTxLst ARGS(( RgSchUeCb      *ue,
                                               RgSchRrDlCell  *rrDlCell,
                                               RgSchRrDlUe    *rrDlUe));

PRIVATE Void rgSCHRrUlAddUeTxLst ARGS(( RgSchUeCb      *ue,
                                               RgSchRrUlCell  *rrUlCell,
                                               RgSchRrUlUe    *rrUlUe));

PRIVATE Void rgSCHRrDlDelUeTxLst ARGS(( RgSchUeCb      *ue,
                                               RgSchRrDlCell  *rrDlCell,
                                               RgSchRrDlUe    *rrDlUe));

PRIVATE Void rgSCHRrUlDelUeTxLst ARGS(( RgSchUeCb      *ue,
                                               RgSchRrUlCell  *rrUlCell,
                                               RgSchRrUlUe    *rrUlUe));

PUBLIC S16 rgSCHRrUlLcgUpd ARGS  (( RgSchCellCb         *cell,
                                    RgSchUeCb           *ue,
                                    RgInfUeDatInd       *datInd));


/**
 * @brief Indication of DL CQI for the UE.
 *
 * @details
 *
 *     Function: rgSCHRrDlCqiInd
 *     Purpose:  RR does nothing
 *
 *  @param[in]  RgSchCellCb        *cell
 *  @param[in]  RgSchUeCb          *ue
 *  @param[in]  TfuDlCqiRpt        *dlCqiRpt
 *  @return  Void
 *
 **/
#ifdef ANSI
PUBLIC Void rgSCHRrDlCqiInd
(
RgSchCellCb        *cell,
RgSchUeCb          *ue,
TfuDlCqiRpt        *dlCqiRpt
)
#else
PUBLIC Void rgSCHRrDlCqiInd(cell, ue, dlCqiRpt)
RgSchCellCb        *cell;
RgSchUeCb          *ue;
TfuDlCqiRpt        *dlCqiRpt;
#endif
{
   TRC2(rgSCHRrDlCqiInd);

   RETVOID;
}

/**
 * @brief Indication of activation of an UE after meas gap or ack/nack rep.
 *
 * @details
 *
 *     Function : rgSCHRrDlActvtUe
 *     Purpose:  RR does nothing
 *
 *  @param[in]  RgSchCellCb         *cell
 *  @param[in]  RgSchUeCb           *ue
 *  @return  Void
 **/
#ifdef ANSI
PUBLIC Void rgSCHRrDlActvtUe
(
RgSchCellCb          *cell,
RgSchUeCb            *ue
)
#else
PUBLIC Void rgSCHRrDlActvtUe(cell, ue)
RgSchCellCb          *cell;
RgSchUeCb            *ue;
#endif
{
   TRC2(rgSCHRrDlActvtUe);

   RETVOID;
}

/**
 * @brief Indication of Dl Refresh for the UE.
 *
 * @details
 *
 *     Function: rgSCHRrDlUeRefresh
 *     Purpose:  RR does nothing.
 *
 *  @param[in]  RgSchCellCb        *cell
 *  @param[in]  RgSchUeCb          *ue
 *  @return  Void
 *
 **/
#ifdef ANSI
PUBLIC Void rgSCHRrDlUeRefresh
(
RgSchCellCb    *cell,
RgSchUeCb      *ue
)
#else
PUBLIC Void rgSCHRrDlUeRefresh(cell, ue)
RgSchCellCb    *cell;
RgSchUeCb      *ue;
#endif
{
   TRC2(rgSCHRrDlUeRefresh);
   RETVOID;
}

/**
 * @brief Cell configuration for Round Robin
 *
 * @details
 *
 *     Function : rgSCHRrDlCellCfg
 *
 *     Processing Steps:
 *      - Create Round Robin related information per cell
 *      - Initialize Tx and ReTx lists
 *
 *  @param[in]  RgSchCellCb         *cell
 *  @param[in]  RgrCellCfg          *cfg
 *  @param[in]  RgSchErrInfo        *err
 *  @return  S16
 *      -# ROK
 *      -# RFAILED
 **/
#ifdef ANSI
PUBLIC S16 rgSCHRrDlCellCfg
(
RgSchCellCb          *cell,
RgrCellCfg           *cfg,
RgSchErrInfo         *err
)
#else
PUBLIC S16 rgSCHRrDlCellCfg(cell, cfg, err)
RgSchCellCb          *cell;
RgrCellCfg           *cfg;
RgSchErrInfo         *err;
#endif
{
   RgSchCmnCell *schCmnCell = (RgSchCmnCell *)(cell->sc.sch);
   RgSchRrDlCell *rrDlCell;
   TRC2(rgSCHRrDlCellCfg);

   if(rgSCHUtlAllocSBuf(cell->instIdx,
      (Data**)&(schCmnCell->dl.schSpfc), (sizeof(RgSchRrDlCell))) != ROK)
   {
      RLOG_ARG0(L_ERROR,DBG_CELLID,cell->cellId, 
         "Memory allocation FAILED");
      RETVALUE(RFAILED);
   }

   rrDlCell = (RgSchRrDlCell *)(schCmnCell->dl.schSpfc);

   rgSCHRrCListInit(&(rrDlCell->prioUeTxLst));
   rgSCHRrCListInit(&(rrDlCell->normUeTxLst));
   rgSCHRrCListInit(&(rrDlCell->ueRetxLst));
   /* Initialize toBeSchedLst*/
   rgSCHRrCListInit(&(rrDlCell->toBeSchedLst));

   RETVALUE(ROK);
}



/**
 * @brief Cell reconfiguration for Round Robin
 *
 * @details
 *
 *     Function : rgSCHRrDlCellRecfg
 *
 *     Processing Steps:
 *      - RR does nothing.
 *
 *  @param[in]  RgSchCellCb       *cell
 *  @param[in]  RgrCellRecfg      *recfg
 *  @param[in]  RgSchErrInfo      *err
 *  @return  S16
 *      -# ROK
 *      -# RFAILED
 **/
#ifdef ANSI
PUBLIC S16 rgSCHRrDlCellRecfg
(
RgSchCellCb     *cell,
RgrCellRecfg    *recfg,
RgSchErrInfo     *err
)
#else
PUBLIC S16 rgSCHRrDlCellRecfg(cell, recfg, err)
RgSchCellCb     *cell;
RgrCellRecfg    *recfg;
RgSchErrInfo     *err;
#endif
{
   TRC2(rgSCHRrDlCellRecfg);

   RETVALUE(ROK);
}


/**
 * @brief Cell deletion for Round Robin
 *
 * @details
 *
 *     Function : rgSCHRrDlCellDel
 *
 *     Processing Steps:
 *      - Free Round Robin related information per cell.
 *
 *  @param[in]  RgSchCellCb       *cell
 *  @return  Void
 **/
#ifdef ANSI
PUBLIC Void rgSCHRrDlCellDel
(
RgSchCellCb       *cell
)
#else
PUBLIC Void rgSCHRrDlCellDel(cell)
RgSchCellCb       *cell;
#endif
{
   RgSchCmnCell *schCmnCell = (RgSchCmnCell *)(cell->sc.sch);
   TRC2(rgSCHRrDlCellDel);

   if(schCmnCell->dl.schSpfc)
   {
      /* ccpu00117052 - MOD - Passing double pointer
      for proper NULLP assignment*/
      rgSCHUtlFreeSBuf(cell->instIdx,
      (Data**)(&(schCmnCell->dl.schSpfc)), (sizeof(RgSchRrDlCell)));
   }

   RETVOID;
}

/**
 * @brief Dl Harq Entity initialization for Round Robin
 *
 * @details
 *
 *     Function : rgSCHRrDlUeHqEntInit
 *
 *     Processing Steps:
 *      - Create Round Robin related information per Harq Entity
 *
 *  @param[in]  RgrSchCellCb     *cell
 *  @param[in]  RgSchUeCb        *ue
 *  @return  S16
 *      -# ROK
 *      -# RFAILED
 **/
#ifdef ANSI
PUBLIC S16 rgSCHRrDlUeHqEntInit
(
RgSchCellCb      *cell,
RgSchUeCb        *ue
)
#else
PUBLIC S16 rgSCHRrDlUeHqEntInit(cell, ue)
RgSchCellCb      *cell;
RgSchUeCb        *ue;
#endif
{
   TRC2(rgSCHRrDlUeHqEntInit);
   if(rgSCHUtlAllocSBuf(cell->instIdx,
      (Data**)&(ue->dl.hqEnt->sch),
      (ue->dl.hqEnt->numHqPrcs * sizeof(RgSchRrDlHqProc))) != ROK)
   {
      RLOG_ARG1(L_ERROR,DBG_CELLID,cell->cellId,
         "Memory allocation FAILED CRNTI:%d",ue->ueId);
      RETVALUE(RFAILED);
   }
   RETVALUE(ROK);
}

/**
 * @brief Dl Harq Entity deletion for Round Robin
 *
 * @details
 *
 *     Function : rgSCHRrDlUeHqEntDeInit 
 *
 *     Processing Steps:
 *      - Free Round Robin related information per Harq Entity
 *
 *  @param[in]  RgrSchCellCb     *cell
 *  @param[in]  RgSchDlHqEnt     *hqE 
 *  @return  Void
 **/
#ifdef ANSI
PUBLIC S16 rgSCHRrDlUeHqEntDeInit 
(
RgSchCellCb      *cell,
RgSchDlHqEnt     *hqE
)
#else
PUBLIC S16 rgSCHRrDlUeHqEntDeInit(cell, hqE)
RgSchCellCb      *cell;
RgSchDlHqEnt     *hqE;
#endif
{
   TRC2(rgSCHRrDlUeHqEntDeInit);

   if(hqE->sch)
   {
      rgSCHUtlFreeSBuf(cell->instIdx,
      (Data**)(&(hqE->sch)),
      (hqE->numHqPrcs * sizeof(RgSchRrDlHqProc)));
   }
   RETVALUE(ROK);
}


/**
 * @brief UE configuration for Round Robin
 *
 * @details
 *
 *     Function : rgSCHRrDlUeCfg
 *
 *     Processing Steps:
 *      - Create Round Robin related information per UE.
 *      - Initialize signalling and data lists.
 *
 *  @param[in]  RgrSchCellCb     *cell
 *  @param[in]  RgSchUeCb        *ue
 *  @param[in]  RgrUeCfg         *cfg
 *  @param[in]  RgSchErrInfo     *err
 *  @return  S16
 *      -# ROK
 *      -# RFAILED
 **/
#ifdef ANSI
PUBLIC S16 rgSCHRrDlUeCfg
(
RgSchCellCb      *cell,
RgSchUeCb        *ue,
RgrUeCfg         *cfg,
RgSchErrInfo     *err
)
#else
PUBLIC S16 rgSCHRrDlUeCfg(cell, ue, cfg, err)
RgSchCellCb      *cell;
RgSchUeCb        *ue;
RgrUeCfg         *cfg;
RgSchErrInfo     *err;
#endif
{
   RgSchCmnCell   *schCmnCell = (RgSchCmnCell *)(cell->sc.sch);
   RgSchRrDlCell  *rrDlCell = (RgSchRrDlCell *)(schCmnCell->dl.schSpfc);
   RgSchCmnUe     *schCmnUe = (RgSchCmnUe *)(ue->sch);
   RgSchRrDlUe    *rrDlUe;
   TRC2(rgSCHRrDlUeCfg);

   if(rgSCHUtlAllocSBuf(cell->instIdx,
      (Data**)&(schCmnUe->dl.schSpfc), (sizeof(RgSchRrDlUe))) != ROK)
   {
      RLOG_ARG1(L_ERROR,DBG_CELLID,cell->cellId,
         "Memory allocation FAILED CRNTI:%d",ue->ueId);
      RETVALUE(RFAILED);
   }

   rrDlUe = (RgSchRrDlUe *)(schCmnUe->dl.schSpfc);

   rgSCHRrCListInit(&(rrDlUe->sigLst));
   rgSCHRrCListInit(&(rrDlUe->dataLst));
   rgSCHRrDlAddUeTxLst(ue,rrDlCell, rrDlUe);

   RETVALUE(ROK);
}

/**
 * @brief This function removes a HARQ process from the retx
 *
 * @details
 *
 *     Function: rgSCHRrDlProcRmvFrmRetx
 *     Purpose:  This function removes a HARQ process from retransmission
 *               queue. This may be performed when a HARQ ack is successful
 *               for a retransmission or during UE reset
 *
 *  @param[in]  RgSchCellCb      *cell
 *  @param[in]  RgSchDlHqProcCb  *hqP
 *  @return  Void
 *
 **/
#ifdef ANSI
PUBLIC Void rgSCHRrDlProcRmvFrmRetx
(
RgSchCellCb                *cell,
RgSchDlHqProcCb            *hqP
)
#else
PUBLIC Void rgSCHRrDlProcRmvFrmRetx(cell, hqP)
RgSchCellCb                *cell;
RgSchDlHqProcCb            *hqP;
#endif
{
   RgSchCmnCell *schCmnCell = (RgSchCmnCell *)(cell->sc.sch);
   RgSchRrDlCell *rrDlCell = (RgSchRrDlCell *)(schCmnCell->dl.schSpfc);
   RgSchRrDlHqProc *schRrDlHqProc =
      &((RgSchRrDlHqProc *)hqP->hqE->sch)[hqP->procId];
   TRC2(rgSCHRrDlProcRmvFrmRetx);

   if(schRrDlHqProc->retxLnk.node != NULLP)
   {
      rgSCHRrCListDelFrm(&(rrDlCell->ueRetxLst),
            &(schRrDlHqProc->retxLnk));
      schRrDlHqProc->retxLnk.node = NULLP;
   }

   RETVOID;
}

/**
 * @brief UE reconfiguration for Round Robin
 *
 * @details
 *
 *     Function : rgSCHRrDlUeRecfg
 *
 *     Processing Steps:
 *      - RR does nothing.
 *
 *  @param[in]   RgSchCellCb      *cell
 *  @param[in]   RgSchUeCb        *ue
 *  @param[in]   RgrUeRecfg       *recfg
 *  @param[in]   RgSchErrInfo     *err
 *  @return  S16
 *      -# ROK
 *      -# RFAILED
 **/
#ifdef ANSI
PUBLIC S16 rgSCHRrDlUeRecfg
(
RgSchCellCb      *cell,
RgSchUeCb        *ue,
RgrUeRecfg       *recfg,
RgSchErrInfo     *err
)
#else
PUBLIC S16 rgSCHRrDlUeRecfg(cell, ue, recfg, err)
RgSchCellCb      *cell;
RgSchUeCb        *ue;
RgrUeRecfg       *recfg;
RgSchErrInfo     *err;
#endif
{
   RgSchCmnCell   *schCmnCell = (RgSchCmnCell *)(cell->sc.sch);
   RgSchRrDlCell  *rrDlCell = (RgSchRrDlCell *)(schCmnCell->dl.schSpfc);
   RgSchCmnUe     *schCmnUe = (RgSchCmnUe *)(ue->sch);
   RgSchRrDlUe    *rrDlUe = (RgSchRrDlUe *)(schCmnUe->dl.schSpfc);

   TRC2(rgSCHRrDlUeRecfg)
   UNUSED(err);
   if((cell->csgAccsMode == RGR_CELL_ACCS_HYBRID) &&
      (recfg->ueRecfgTypes & RGR_UE_CSG_PARAM_RECFG))
   {
      if (rrDlUe->ueTxLnk.node != NULLP)
      {
         if (recfg->csgMmbrSta == FALSE)
         {
            rgSCHRrCListDelFrm(&(rrDlCell->prioUeTxLst), &(rrDlUe->ueTxLnk));
            rgSCHRrCListAdd2Tail(&(rrDlCell->normUeTxLst), &(rrDlUe->ueTxLnk));
         }
         else
         {
            rgSCHRrCListDelFrm(&(rrDlCell->normUeTxLst), &(rrDlUe->ueTxLnk));
            rgSCHRrCListAdd2Tail(&(rrDlCell->prioUeTxLst), &(rrDlUe->ueTxLnk));
         }
         rrDlUe->ueTxLnk.node = (PTR)ue;
      }
   }

   RETVALUE(ROK);
}
/**
 * @brief This function adds a HARQ process to the retx
 *
 * @details
 *
 *     Function: rgSCHRrDlProcAddToRetx
 *     Purpose:  This function adds a HARQ process to the retransmission
 *               queue. This may be performed when a HARQ nack is received
 *               and transmissions are not exceeded the max.
 *
 *  @param[in]  RgSchCellCb      *cell
 *  @param[in]  RgSchDlHqProcCb  *hqP
 *  @return  Void
 *
 **/
#ifdef ANSI
PUBLIC Void rgSCHRrDlProcAddToRetx
(
RgSchCellCb                *cell,
RgSchDlHqProcCb            *hqP
)
#else
PUBLIC Void rgSCHRrDlProcAddToRetx(cell, hqP)
RgSchCellCb                *cell;
RgSchDlHqProcCb            *hqP;
#endif
{
   RgSchCmnCell *schCmnCell = (RgSchCmnCell *)(cell->sc.sch);
   RgSchRrDlCell *rrDlCell = (RgSchRrDlCell *)(schCmnCell->dl.schSpfc);
   RgSchRrDlHqProc *schRrDlHqProc =
      &((RgSchRrDlHqProc *)hqP->hqE->sch)[hqP->procId];

   TRC2(rgSCHRrDlProcAddToRetx);


   rgSCHRrCListAdd2Tail(&(rrDlCell->ueRetxLst),
                     &(schRrDlHqProc->retxLnk));
   schRrDlHqProc->retxLnk.node = (PTR)hqP;

   RETVOID;
}

/* MS_WORKAROUND for ccpu00122892 : Temp fix for erroeneous RETX Harq release by rgSCHCmnDlAllocRetxRb */
#ifdef ANSI
PUBLIC Void rgSCHRrDlRmvProcFrmRetxLst
(
RgSchCellCb                *cell,
RgSchUeCb                  *ue,
RgSchDlHqProcCb            *hqP
)
#else
PUBLIC Void rgSCHRrDlRmvProcFrmRetxLst(cell, ue, hqP)
RgSchCellCb                *cell;
RgSchUeCb                  *ue;
RgSchDlHqProcCb            *hqP;
#endif
{
   UNUSED(ue);
   rgSCHRrDlProcRmvFrmRetx(cell, hqP);
   RETVOID;
}

/***********************************************************
 *
 *     Func : rgSCHRrDlScanUpdPdbPrio
 *
 *     Desc : Increment the pivot and reposition the LCs under the pivot to 
 *            new location according to thieir PDB and elapsed time.
 *
 *     Ret  : Void
 *
 *     Notes:
 *
 *     File :
 *
 **********************************************************/
#ifdef ANSI
PUBLIC Void rgSCHRrDlScanUpdPdbPrio
(
RgSchCellCb *cell
)
#else
PUBLIC Void rgSCHRrDlScanUpdPdbPrio (cell)
RgSchCellCb *cell;
#endif
{
   TRC2(rgSCHRrDlScanUpdPdbPrio);
   
   RETVOID;
}

/**
 * @brief Function to update Flow control information 
 *        to be sent to MAC.
 *
 * @details
 *
 *     Function: rgSCHRrDlFillFlowCntrlInfo
 *
 *              update Flow control information 
 *
 *     Invoked by: 
 *         SCHD
 *
 *     Processing Steps:
 *           
 *  @param[in] RgSchCellCb       *cell
               RgInfSfAlloc    *sfAlloc;
 *  @return  S16
 *      -# ROK 
 **/
#ifdef ANSI
PUBLIC S16 rgSCHRrDlFillFlowCntrlInfo
(
RgSchCellCb    *cell,
RgInfSfAlloc   *sfAlloc
)
#else
PUBLIC S16 rgSCHRrDlFillFlowCntrlInfo(cell,sfAlloc)
RgSchCellCb    *cell;
RgInfSfAlloc   *sfAlloc;
#endif
{
  TRC2(rgSCHRrDlFillFlowCntrlInfo);
  RETVALUE(ROK);
}

/**
 * @brief UE delete for Round Robin
 *
 * @details
 *
 *     Function : rgSCHRrDlUeDel
 *
 *     Processing Steps:
 *     - Remove UE from retx queue if present
 *     - Remove UE from tx queue if present
 *     - Free Round Robin UE info
 *
 *  @param[in]  RgSchCellCb       *cell
 *  @param[in]  RgSchUeCb         *ue
 *  @return  Void
 **/
#ifdef ANSI
PUBLIC Void rgSCHRrDlUeDel
(
RgSchCellCb      *cell,
RgSchUeCb        *ue
)
#else
PUBLIC Void rgSCHRrDlUeDel(cell, ue)
RgSchCellCb      *cell;
RgSchUeCb        *ue;
#endif
{
   RgSchCmnCell *schCmnCell = (RgSchCmnCell *)(cell->sc.sch);
   RgSchRrDlCell *rrDlCell = (RgSchRrDlCell *)(schCmnCell->dl.schSpfc);
   RgSchCmnUe *schCmnUe = (RgSchCmnUe *)(ue->sch);
   RgSchRrDlUe  *rrDlUe = (RgSchRrDlUe *)(schCmnUe->dl.schSpfc);
   U8 i;
   U8 j;
   RgSchPdcch        *pdcchPrev = NULLP;
   TRC2(rgSCHRrDlUeDel);

   if(schCmnUe->dl.schSpfc == NULLP)
   {
      RETVOID;
   }

   if(rrDlUe->ueTxLnk.node != NULLP)
   {
      rgSCHRrDlDelUeTxLst(ue,rrDlCell,rrDlUe);
   }

   /* Fix : syed Assign hqEnt to UE only if msg4 is done */
   if (ue->dl.hqEnt)
   {
      /* remove all in use HARQ processes from the subframes */
      for(i = 0; i < ue->dl.hqEnt->numHqPrcs; i++)
      {
         RgSchDlHqProcCb  *hqP = &(ue->dl.hqEnt->procs[i]);
         rgSCHRrDlProcRmvFrmRetx(cell, hqP);
         for (j = 0; j < 2; j++)
         {
            if ((hqP->tbInfo[j].subFrm != NULLP) &&
                  (hqP->tbInfo[j].sfLnk.node != NULLP))
            { 
               /*
                * Since each TB can be assigned the same PDCCH structure incase of
                * Spatial multiplexing allocation of 2 TBs. Check if we have
                * already removed PDCCH as part of one TB, then no need to remove
                * as part of other TB processing 
                */
                
               if (hqP->tbInfo[j].pdcch && (hqP->tbInfo[j].pdcch != pdcchPrev))
               {
                  cmLListDelFrm(&hqP->tbInfo[j].subFrm->pdcchInfo.pdcchs,
                        &hqP->tbInfo[j].pdcch->lnk);
                  cmLListAdd2Tail(&cell->pdcchLst, &hqP->tbInfo[j].pdcch->lnk);
               }
               pdcchPrev = hqP->tbInfo[j].pdcch;
               rgSCHUtlDlHqPTbRmvFrmTx(hqP->tbInfo[j].subFrm, hqP, j, FALSE);
               rgSCHDhmRlsHqpTb(hqP, j, TRUE);
            }
         }
      }
   }

   rgSCHUtlFreeSBuf(cell->instIdx,
         (Data**)(&(schCmnUe->dl.schSpfc)), (sizeof(RgSchRrDlUe)));

   RETVOID;
}

/**
 * @brief Dl LC configuration for Round Robin
 *
 * @details
 *
 *     Function : rgSCHRrDlLcCfg
 *
 *     Processing Steps:
 *      - Create the round robin info
 *
 *  @param[in]  RgrSchCellCb     *cell
 *  @param[in]  RgSchUeCb        *ue
 *  @param[in]  RgSchDlLcCb      *dlLc
 *  @param[in]  RgrLchCfg        *cfg
 *  @param[in]  RgSchErrInfo     *err
 *  @return  S16
 *      -# ROK
 *      -# RFAILED
 **/
#ifdef ANSI
PUBLIC S16 rgSCHRrDlLcCfg
(
RgSchCellCb      *cell,
RgSchUeCb        *ue,
RgSchDlLcCb      *dlLc,
RgrLchCfg        *cfg,
RgSchErrInfo     *err
)
#else
PUBLIC S16 rgSCHRrDlLcCfg(cell, ue, dlLc, cfg, err)
RgSchCellCb      *cell;
RgSchUeCb        *ue;
RgSchDlLcCb      *dlLc;
RgrLchCfg        *cfg;
RgSchErrInfo     *err;
#endif
{
   RgSchCmnDlSvc *schCmnDlLc = (RgSchCmnDlSvc *)(dlLc->sch);
   TRC2(rgSCHRrDlLcCfg);

   if(rgSCHUtlAllocSBuf(cell->instIdx,
      (Data**)&(schCmnDlLc->schSpfc), (sizeof(RgSchRrDlLc))) != ROK)
   {
      RLOG_ARG2(L_ERROR,DBG_CELLID,cell->cellId,
         "Memory allocation FAILED CRNTI:%d LCID:%d",ue->ueId,cfg->lcId);
      RETVALUE(RFAILED);
   }
   RETVALUE(ROK);
}

/**
 * @brief Dl LC re-configuration for Round Robin
 *
 * @details
 *
 *     Function : rgSCHRrDlLcRecfg
 *
 *     Processing Steps:
 *      - RR does nothing
 *
 *  @param[in]  RgrSchCellCb     *cell
 *  @param[in]  RgSchUeCb        *ue
 *  @param[in]  RgSchDlLcCb      *dlLc
 *  @param[in]  RgrLchRecfg      *recfg
 *  @param[in]  RgSchErrInfo     *err
 *  @return  S16
 *      -# ROK
 *      -# RFAILED
 **/
#ifdef ANSI
PUBLIC S16 rgSCHRrDlLcRecfg
(
RgSchCellCb      *cell,
RgSchUeCb        *ue,
RgSchDlLcCb      *dlLc,
RgrLchRecfg      *recfg,
RgSchErrInfo     *err
)
#else
PUBLIC S16 rgSCHRrDlLcRecfg(cell, ue, dlLc, recfg, err)
RgSchCellCb      *cell;
RgSchUeCb        *ue;
RgSchDlLcCb      *dlLc;
RgrLchRecfg      *recfg;
RgSchErrInfo     *err;
#endif
{
   TRC2(rgSCHRrDlLcRecfg);

   RETVALUE(ROK);
}


/**
 * @brief Dl LC deletion for Round Robin
 *
 * @details
 *
 *     Function : rgSCHRrDlLcDel
 *
 *     Processing Steps:
 *      - Free round robin info associated with the LC
 *      - Remove UE from tx queue if there are no LCs with
 *        outstanding BO
 *
 *  @param[in]  RgrSchCellCb     *cell
 *  @param[in]  RgSchUeCb        *ue
 *  @param[in]  RgSchDlLcCb      *dlLc
 *  @return  Void
 **/
#ifdef ANSI
PUBLIC Void rgSCHRrDlLcDel
(
RgSchCellCb      *cell,
RgSchUeCb        *ue,
RgSchDlLcCb      *dlLc
)
#else
PUBLIC Void rgSCHRrDlLcDel(cell, ue, dlLc)
RgSchCellCb      *cell;
RgSchUeCb        *ue;
RgSchDlLcCb      *dlLc;
#endif
{
   RgSchCmnCell *schCmnCell = (RgSchCmnCell *)(cell->sc.sch);
   RgSchRrDlCell *rrDlCell = (RgSchRrDlCell *)(schCmnCell->dl.schSpfc);
   RgSchCmnUe *schCmnUe = (RgSchCmnUe *)(ue->sch);
   RgSchRrDlUe  *rrDlUe = (RgSchRrDlUe *)(schCmnUe->dl.schSpfc);
   RgSchCmnDlSvc *schCmnDlLc = (RgSchCmnDlSvc *)(dlLc->sch);
   RgSchRrDlLc *rrDlLc = (RgSchRrDlLc *)(schCmnDlLc->schSpfc);
   TRC2(rgSCHRrDlLcDel);

   if(schCmnDlLc->schSpfc == NULLP)
   {
      RETVOID;
   }

   if(rrDlLc->lnk.node)
   {
      if((dlLc->lcId >= RG_RR_MIN_DCCH_LCID) &&
         (dlLc->lcId <= RG_RR_MAX_DCCH_LCID))
      {
         rgSCHRrCListDelFrm(&(rrDlUe->sigLst), &(rrDlLc->lnk));
      }
      else
      {
         rgSCHRrCListDelFrm(&(rrDlUe->dataLst), &(rrDlLc->lnk));
      }
      rrDlLc->lnk.node = NULLP; /* Added for avoiding dangling pointer */

      if((rrDlUe->sigLst.first == NULLP) &&
            (rrDlUe->dataLst.first == NULLP))
      {
         if (rrDlUe->ueTxLnk.node != NULLP)
         {
            rgSCHRrDlDelUeTxLst(ue,rrDlCell,rrDlUe);
         }
      }
   }

   /* ccpu00117052 - MOD - Passing double pointer
   for proper NULLP assignment*/
   rgSCHUtlFreeSBuf(cell->instIdx,
   (Data**)(&(schCmnDlLc->schSpfc)), (sizeof(RgSchRrDlLc)));
   RETVOID;
}

/**
 * @brief BO update
 *
 * @details
 *
 *     Function : rgSCHRrDlDedBoUpd
 *
 *     Processing Steps:
 *     - If the UE is not part of transmission queue then
 *       append the UE to this queue if reported bo > 0.
 *       If reported bo is ZERO then remove the UE from
 *       transmission queue if there are no other LCs with
 *       outstanding bo.
 *
 *  @param[in]  RgSchCellCb            *cell
 *  @param[in]  RgSchUeCb              *ue
 *  @param[in]  RgSchDlLcCb            *lc
 *  @return  Void
 **/
#ifdef ANSI
PUBLIC Void rgSCHRrDlDedBoUpd
(
RgSchCellCb            *cell,
RgSchUeCb              *ue,
RgSchDlLcCb            *dlLc
)
#else
PUBLIC Void rgSCHRrDlDedBoUpd(cell, ue, dlLc)
RgSchCellCb            *cell;
RgSchUeCb              *ue;
RgSchDlLcCb            *dlLc;
#endif
{
   RgSchCmnCell *schCmnCell = (RgSchCmnCell *)(cell->sc.sch);
   RgSchRrDlCell *rrDlCell = (RgSchRrDlCell *)(schCmnCell->dl.schSpfc);
   RgSchCmnUe *schCmnUe = (RgSchCmnUe *)(ue->sch);
   RgSchRrDlUe  *rrDlUe = (RgSchRrDlUe *)(schCmnUe->dl.schSpfc);
   RgSchCmnDlSvc *schCmnDlLc = (RgSchCmnDlSvc *)(dlLc->sch);
   RgSchRrDlLc *rrDlLc = (RgSchRrDlLc *)(schCmnDlLc->schSpfc);
   TRC2(rgSCHRrDlDedBoUpd);

   if((dlLc->bo) && (rrDlLc->lnk.node == NULLP))
   {
      if((dlLc->lcId >= RG_RR_MIN_DCCH_LCID) &&
         (dlLc->lcId <= RG_RR_MAX_DCCH_LCID))
      {
         rgSCHRrCListAdd2Tail(&(rrDlUe->sigLst), &(rrDlLc->lnk));
         rrDlLc->lnk.node = (PTR)dlLc;
      }
      else
      {
         rgSCHRrCListAdd2Tail(&(rrDlUe->dataLst), &(rrDlLc->lnk));
         rrDlLc->lnk.node = (PTR)dlLc;
      }
      rgSCHRrDlAddUeTxLst(ue,rrDlCell,rrDlUe);
      RETVOID;
   }

   if((dlLc->bo == 0) && (rrDlLc->lnk.node))
   {
      if((dlLc->lcId >= RG_RR_MIN_DCCH_LCID) &&
         (dlLc->lcId <= RG_RR_MAX_DCCH_LCID))
      {
         rgSCHRrCListDelFrm(&(rrDlUe->sigLst), &(rrDlLc->lnk));
         rrDlLc->lnk.node = NULLP;
      }
      else
      {
         rgSCHRrCListDelFrm(&(rrDlUe->dataLst), &(rrDlLc->lnk));
         rrDlLc->lnk.node = NULLP;
      }

      if((rrDlUe->sigLst.first == NULLP) &&
            (rrDlUe->dataLst.first == NULLP))
      {
         rgSCHRrDlDelUeTxLst(ue,rrDlCell,rrDlUe);
      }
      RETVOID;
   }

   /* Stack crash problem for TRACE5 Changes. added the return below */
  RETVOID;
}

/**
 * @brief Indication of DL UE reset
 *
 * @details
 *
 *     Function : rgSCHRrDlUeReset
 *
 *     Processing Steps:
 *     - Remove the UE from re-transmission list
 *     - Remove the UE from transmission list
 *
 *  @param[in]  RgSchCellCb       *cell
 *  @param[in]  RgSchUeCb         *ue
 *  @return  Void
 **/
#ifdef ANSI
PUBLIC Void rgSCHRrDlUeReset
(
RgSchCellCb          *cell,
RgSchUeCb            *ue
)
#else
PUBLIC Void rgSCHRrDlUeReset(cell, ue)
RgSchCellCb          *cell;
RgSchUeCb            *ue;
#endif
{
   RgSchCmnCell *schCmnCell = (RgSchCmnCell *)(cell->sc.sch);
   RgSchRrDlCell *rrDlCell = (RgSchRrDlCell *)(schCmnCell->dl.schSpfc);
   RgSchCmnUe *schCmnUe = (RgSchCmnUe *)(ue->sch);
   RgSchRrDlUe  *rrDlUe = (RgSchRrDlUe *)(schCmnUe->dl.schSpfc);
   RgSchRrCList *node;
   RgSchDlLcCb *dlLc;
   RgSchCmnDlSvc *schCmnDlLc;
   RgSchRrDlLc *rrDlLc;
   U8 i;
   TRC2(rgSCHRrDlUeReset);

   while((node=rrDlUe->sigLst.first) != NULLP)
   {
      dlLc = (RgSchDlLcCb *)(node->node);
      schCmnDlLc = (RgSchCmnDlSvc *)(dlLc->sch);
      rrDlLc = (RgSchRrDlLc *)(schCmnDlLc->schSpfc);
      rgSCHRrCListDelFrm(&(rrDlUe->sigLst), &(rrDlLc->lnk));
      rrDlLc->lnk.node = NULLP;
      node = node->next;
   }
   rgSCHRrCListInit(&(rrDlUe->sigLst));

   while((node=rrDlUe->dataLst.first) != NULLP)
   {
      dlLc = (RgSchDlLcCb *)(node->node);
      schCmnDlLc = (RgSchCmnDlSvc *)(dlLc->sch);
      rrDlLc = (RgSchRrDlLc *)(schCmnDlLc->schSpfc);
      rgSCHRrCListDelFrm(&(rrDlUe->dataLst), &(rrDlLc->lnk));
      rrDlLc->lnk.node = NULLP;
      node = node->next;
   }
   rgSCHRrCListInit(&(rrDlUe->dataLst));
   if((!(ue->dl.dlInactvMask & RG_HQENT_INACTIVE)) && (ue->dl.hqEnt))
   {
      /* remove all in use HARQ processes from the subframes */
      for(i = 0; i < ue->dl.hqEnt->numHqPrcs; i++)
      {
         RgSchDlHqProcCb  *hqP = &(ue->dl.hqEnt->procs[i]);
         rgSCHRrDlProcRmvFrmRetx(cell, hqP);
         /*MS_WORKAROUND for ccpu00122892 */
         if ((hqP->tbInfo[0].subFrm != NULLP) &&
               (hqP->tbInfo[0].sfLnk.node != NULLP))
         {

            if (hqP->tbInfo[0].pdcch)
            {
               cmLListDelFrm(&hqP->tbInfo[0].subFrm->pdcchInfo.pdcchs,
                     &hqP->tbInfo[0].pdcch->lnk);
               cmLListAdd2Tail(&cell->pdcchLst, &hqP->tbInfo[0].pdcch->lnk);
            }
            /* Fix */
            rgSCHUtlDlHqPTbRmvFrmTx(hqP->tbInfo[0].subFrm, hqP, 0, FALSE);
         }
         if ((hqP->tbInfo[1].subFrm != NULLP) &&
               (hqP->tbInfo[1].sfLnk.node != NULLP))
         {
            if (hqP->tbInfo[1].pdcch && 
                  (hqP->tbInfo[1].pdcch != hqP->tbInfo[0].pdcch))
            {
               cmLListDelFrm(&hqP->tbInfo[1].subFrm->pdcchInfo.pdcchs,
                     &hqP->tbInfo[1].pdcch->lnk);
               cmLListAdd2Tail(&cell->pdcchLst, &hqP->tbInfo[1].pdcch->lnk);
            }
            /* Fix */
            rgSCHUtlDlHqPTbRmvFrmTx(hqP->tbInfo[1].subFrm, hqP, 1, FALSE);
         }
      }
   }

   if(rrDlUe->ueTxLnk.node != NULLP)
   {
      rgSCHRrDlDelUeTxLst(ue,rrDlCell,rrDlUe);
   }

   RETVOID;
}



/**
 * @brief Scheduling of DL re-transmissions
 *
 * @details
 *
 *     Function : rgSCHRrDlRetx
 *
 *     Processing Steps:
 *     - Traverse the re-transmission list and do resource allocation
 *       for the UEs.
 *
 *  @param[in]  RgSchCellCb            *cell
 *  @param[in]  RgSchCmnDlRbAllocInfo  *allocInfo
 *  @return  Void
 **/
#ifdef ANSI
PRIVATE Void rgSCHRrDlRetx
(
RgSchCellCb            *cell,
RgSchCmnDlRbAllocInfo  *allocInfo
)
#else
PRIVATE Void rgSCHRrDlRetx(cell, allocInfo)
RgSchCellCb            *cell;
RgSchCmnDlRbAllocInfo  *allocInfo;
#endif
{
   RgSchCmnCell *schCmnCell = (RgSchCmnCell *)(cell->sc.sch);
   RgSchRrDlCell *rrDlCell = (RgSchRrDlCell *)(schCmnCell->dl.schSpfc);
   RgSchUeCb    *ue;
   RgSchCmnUe   *schCmnUe;
   RgSchRrDlUe  *rrDlUe;
   RgSchDlHqProcCb *hqP;
   RgSchRrCList *node;
   U32 allocBo;
   /* Fix: MUE_PERTTI_DL*/
   U32 remUe = allocInfo->dedAlloc.dedDlSf->remUeCnt;
#ifdef LTEMAC_SPS
   CmLteTimingInfo schdTime;
#endif
#ifdef LTEMAC_HDFDD
   Bool                 dlAllowed = FALSE;
#endif
   TRC2(rgSCHRrDlRetx);

   if(rrDlCell->ueRetxLst.crnt == NULLP || !remUe)
   {
      RETVOID;
   }
#ifdef LTEMAC_SPS
   schdTime = cell->crntTime;

   /* Increment by DL DELTA to determine the time for which scheduling
    * is done */
   RGSCH_INCR_SUB_FRAME(schdTime, RG_SCH_CMN_DL_DELTA);
#endif

/* LTE_ADV_FLAG_REMOVED_START */

   /* if SFR and ABS both are enabled, SFR takes precedence */
   if(cell->lteAdvCb.sfrCfg.status)
   {
      rgSCHRrDlRetxOfPrioritizedUe(cell, allocInfo, RGR_SFR, TRUE);
      /* Recalculating number of UEs remaining to schedule
       * after ABS SF allocation for cell edge user*/
      remUe = allocInfo->dedAlloc.dedDlSf->remUeCnt;
      if(!remUe)
      {
         RETVOID;
      }
   }

   /** 
    * When ABS is enabled, schedule the ABS UE's ABS SF.
    */
   if(RG_SCH_ABS_ENABLED_ABS_SF == cell->lteAdvCb.absDlSfInfo)
   {
      rgSCHRrDlRetxOfPrioritizedUe(cell, allocInfo, RGR_ABS, TRUE);
      /* Recalculating number of UEs remaining to schedule 
       * after ABS SF allocation */
      remUe = allocInfo->dedAlloc.dedDlSf->remUeCnt;
      if(!remUe)
      {
         RETVOID;
      }
   }
/* LTE_ADV_FLAG_REMOVED_END */

   /* loop till the crnt node */
   node = rrDlCell->ueRetxLst.crnt;
   do
   {
      hqP = (RgSchDlHqProcCb *)(node->node);
      ue = hqP->hqE->ue;
      schCmnUe = (RgSchCmnUe *)(ue->sch);
      rrDlUe = (RgSchRrDlUe *)(schCmnUe->dl.schSpfc);
      node = node->next;

#ifdef LTEMAC_HDFDD
      if (ue->hdFddEnbld)
      {
         rgSCHCmnHdFddChkDlAllow ( cell, ue, &dlAllowed);
         if (dlAllowed == FALSE)
         {
            continue;
         }
      }
#endif
      if((ue->dl.dlInactvMask)
#ifdef LTEMAC_SPS
            ||(RG_SCH_CMN_IS_UE_SPS_SCHDLD(ue, schdTime))
#endif
        )
      {
         continue;
      }

      if(schCmnUe->dl.proc != NULLP)
      {
         /*UE is already selected for re-tx*/
         continue;
      }
      /* Extra check: indicate if there is furtherScope for NewTx
       * addition for a HqProc. This information will
       * be utilized by common scheduler, in case of SM
       * UEs with only one of the TBs retransmitting and the
       * other TB can be used for clubbing new TX. */
      if ((rrDlUe->sigLst.first != NULLP) || (rrDlUe->dataLst.first != NULLP))
      {
         ue->dl.dlAllocCb.mimoAllocInfo.hasNewTxData = TRUE;
      }
      /* 3.1 MIMO : last parameter changed */
      if(rgSCHCmnDlAllocRetxRb(cell, allocInfo->dedAlloc.dedDlSf,
               ue, 0, &allocBo, hqP, allocInfo) != ROK)
      {
         /* Fix: syed For RETX let Fnlz alone change the crnt by means of 
          * node deletions */
         /* Fix: syed Missing remUeCnt setting */
         rrDlCell->ueRetxLst.crnt = node->prev;
         break;
      }
      /* Fix: syed removing unnecessary check*/

      if(allocBo == 0)
      {
         continue;
      }
      schCmnUe->dl.proc = hqP;
      /* Adding Ue into Retx list is moved to cmn Scheduler */
      --remUe;
      if(!remUe)
      {
         rrDlCell->ueRetxLst.crnt = node;
         break;
      }

   }while(node != rrDlCell->ueRetxLst.crnt);

   allocInfo->dedAlloc.dedDlSf->remUeCnt = remUe;
   RETVOID;
}

/* LTE_ADV_FLAG_REMOVED_START */
/**
 * @brief Scheduling of DL re-transmissions of prioritized UE
 *
 * @details
 *
 *     Function : rgSCHRrDlRetxOfPrioritizedUe
 *
 *     Processing Steps:
 *     - Traverse the re-transmission list and do resource allocation
 *       for the prioritized UEs.
 *
 *  @param[in]  RgSchCellCb            *cell
 *  @param[in]  RgSchCmnDlRbAllocInfo  *allocInfo
 *  @param[in]  Bool                   cellEdgePrior
 *  @return  Void
 **/
#ifdef ANSI
PRIVATE Void rgSCHRrDlRetxOfPrioritizedUe
(
 RgSchCellCb            *cell,
 RgSchCmnDlRbAllocInfo  *allocInfo,
 U8                     feature,
 Bool                   vctmPrior
 )
#else
PRIVATE Void rgSCHRrDlRetxOfPrioritizedUe(cell, allocInfo, feature, vctmPrior)
   RgSchCellCb            *cell;
   RgSchCmnDlRbAllocInfo  *allocInfo;
   U8                     feature;
   Bool                   vctmPrior;
#endif
{
   RgSchCmnCell    *schCmnCell = (RgSchCmnCell *)(cell->sc.sch);
   RgSchRrDlCell   *rrDlCell   = (RgSchRrDlCell *)(schCmnCell->dl.schSpfc);
   RgSchUeCb       *ue;
   RgSchCmnUe      *schCmnUe;
   RgSchRrDlUe     *rrDlUe;
   RgSchDlHqProcCb *hqP;
   RgSchRrCList    *node;
   U32             allocBo;
   RgSchRrDlHqProc *schRrDlHqProc; 
   RgSchRrCList    *crntNode = NULLP;
   Bool            isNodeSkipped = FALSE;
   U32             remUe = allocInfo->dedAlloc.dedDlSf->remUeCnt;
   /*rg008.201 - Added for SPS support*/
   TRC2(rgSCHRrDlRetxOfPrioritizedUe);

   /**
    * Start loop through transmission list from current node 
    */
   node = rrDlCell->ueRetxLst.crnt;

   /**
    * Loop till the current node of transmission list
    */
   crntNode = rrDlCell->ueRetxLst.crnt;
   do
   {
      hqP       = (RgSchDlHqProcCb *)(node->node);
      ue        = hqP->hqE->ue;
      schCmnUe  = (RgSchCmnUe *)(ue->sch);
      rrDlUe    = (RgSchRrDlUe *)(schCmnUe->dl.schSpfc);
      node      = node->next;

      /** 
       * While prioritizing CE/ABS UEs, skip scheduling CC/Non-ABS UEs in the list.
       * If any CC/Non-ABS UE is found in the list, then set current pointer to first skipped CC/Non-ABS UE and 
       * move all scheduled CE/ABS UEs after first skipped CC/Non-ABS UE behind the current pointer.
       */
      if(TRUE == vctmPrior)
      {
         if(((RGR_SFR == feature) && (FALSE == ue->lteAdvUeCb.rgrLteAdvUeCfg.isUeCellEdge))
               || ((RGR_ABS == feature) && (FALSE == ue->lteAdvUeCb.rgrLteAdvUeCfg.isAbsUe))
           )
         {
            /* Update the crnt pointer to first skipped node */
            if(FALSE == isNodeSkipped)
            {
               isNodeSkipped = TRUE;
               rrDlCell->ueRetxLst.crnt = node->prev;
            }
            continue;
         }
      }
      /** 
       * While prioritizing CC UEs, skip scheduling CE UEs in the list.
       * If any CE UE is found in the list, then set current pointer to first skipped CE UE and 
       * move all scheduled CC UEs after first skipped CE UE behind the current pointer
       */
      else 
      {
         if(((RGR_SFR == feature) && (TRUE == ue->lteAdvUeCb.rgrLteAdvUeCfg.isUeCellEdge))
               || ((RGR_ABS == feature) && (TRUE == ue->lteAdvUeCb.rgrLteAdvUeCfg.isAbsUe))
           )
         {  
            /* Update the crnt pointer to first skipped node */
            if(FALSE == isNodeSkipped)
            {
               isNodeSkipped = TRUE;
               rrDlCell->ueRetxLst.crnt = node->prev;
            }
            continue;
         }
      }

      /** 
       * If UE is in inactive state, skip the UE from scheduling
       */
      if(ue->dl.dlInactvMask)
      {
         continue;
      }

      /** 
       * If UE is already scheduled, then donot schedule the ReTx 
       */
      if(schCmnUe->dl.proc != NULLP)
      {
         continue;
      }


      /** 
       * Extra check: indicate if there is furtherScope for NewTx
       * addition for a HqProc. This information will
       * be utilized by common scheduler, in case of SM
       * UEs with only one of the TBs retransmitting and the
       * other TB can be used for clubbing new TX. 
       */
      if ((rrDlUe->sigLst.first != NULLP) || (rrDlUe->dataLst.first != NULLP))
      {
         ue->dl.dlAllocCb.mimoAllocInfo.hasNewTxData = TRUE;
      }

      /**
       * Schedule UE for retransmission
       */
      if(rgSCHCmnDlAllocRetxRb(cell, allocInfo->dedAlloc.dedDlSf,
               ue, 0, &allocBo, hqP, allocInfo) != ROK)
      {
         /**
          * No partiall allocation possible in case of ReTx.
          * If node is not skipped while prioritization, 
          * set current pointer as last scheduled UE 
          */
         if(FALSE == isNodeSkipped)
         {
            rrDlCell->ueRetxLst.crnt = node->prev;
         }
         break;
      }

      if(allocBo == 0)
      {
         if(FALSE == isNodeSkipped)
         {
            rrDlCell->ueRetxLst.crnt = node->prev;
         }
         continue;
      }

      schCmnUe->dl.proc = hqP;

      /**
       * If some nodes are skipped during prioritization, 
       * then move current scheduled retransmission behind the current pointer
       */
      if(TRUE == isNodeSkipped)
      {
         /* Remove cell edge UE from list and insert behind the crnt node */
         schRrDlHqProc =
            &((RgSchRrDlHqProc *)hqP->hqE->sch)[hqP->procId];

         rgSCHRrCListDelFrm(&(rrDlCell->ueRetxLst),
               &(schRrDlHqProc->retxLnk));
         rgSCHRrCListAdd2Crnt(&(rrDlCell->ueRetxLst),
               &(schRrDlHqProc->retxLnk));
         /* TODO:Optimization can be considered */
      }
      if((TRUE == vctmPrior) && (RGR_SFR == feature))
      {
         allocInfo->dedAlloc.dedDlSf->sfrTotalPoolInfo.isUeCellEdge = TRUE;
      }

      /**
       * If maximum UE can be scheduled in one subframe is reached, 
       * then skip scheduling retransmission. If none of the UEs are skipped 
       * during prioritization, then set the current pointer to next node in the list
       */
      if(0 == (--remUe))
      {
         if(FALSE == isNodeSkipped)
         {
            rrDlCell->ueRetxLst.crnt = node;
         }
         break;
      }

   }while(node != crntNode);

   allocInfo->dedAlloc.dedDlSf->remUeCnt = remUe;

   RETVOID;
}
/* LTE_ADV_FLAG_REMOVED_END */

/**
 * @brief Scheduling of DL new-transmission for UE
 *
 * @details
 *
 *     Function : rgSCHRrDlSchdUe
 *
 *     Processing Steps:
 *     - Schedule DCCHs if outstanding bo exist for them.
 *     - Schedule DTCHs if outstanding bo exist for them.
 *     - Limit the LCs scheduling based on max TB size supported.
 *
 *  @param[in]  RgSchCellCb         *cell
 *  @param[in]  RgSchUeCb           *ue
 *  @param[in]  RgSchDlSf           *sf
 *  @return  Void
 **/
#ifdef ANSI
PRIVATE S16 rgSCHRrDlSchdUe
(
RgSchCellCb             *cell,
RgSchUeCb               *ue,
RgSchDlSf               *sf,
RgSchCmnDlRbAllocInfo   *allocInfo
)
#else
PRIVATE S16 rgSCHRrDlSchdUe(cell, ue, sf, allocInfo)
RgSchCellCb             *cell;
RgSchUeCb               *ue;
RgSchDlSf               *sf;
RgSchCmnDlRbAllocInfo   *allocInfo;
#endif
{
   RgSchCmnUe *schCmnUe = (RgSchCmnUe *)(ue->sch);
   RgSchRrDlUe  *rrDlUe = (RgSchRrDlUe *)(schCmnUe->dl.schSpfc);
   RgSchDlLcCb *dlLc;
   RgSchCmnDlSvc *schCmnDlLc;
   RgSchRrDlLc *rrDlLc;
   RgSchRrCList *node;
   U32 bo;
   U16 rlcHdrEstmt;
   U32 allocBo;
   RgSchCmnDlHqProc *cmnHqDl;
   S16  ret;
   rgSCHRrCListInit(&(rrDlUe->schedLcs));

   /* Harq availability check is moved to rgSchRrDlCreateSchdLst function to ensure that Ue
    * will be added to the SchdLst if Harq proc is available else it will not be
    * Added to SchLst */

   cmnHqDl = RG_SCH_CMN_GET_DL_HQP(schCmnUe->dl.proc);
   /* Fix : syed Round Robin with in sigLst */
   node = rrDlUe->sigLst.crnt;
   while(node)
   {
      dlLc = (RgSchDlLcCb *)(node->node);
      schCmnDlLc = (RgSchCmnDlSvc *)(dlLc->sch);
      rrDlLc = (RgSchRrDlLc *)(schCmnDlLc->schSpfc);

      RG_SCH_CMN_DL_GET_HDR_EST(dlLc, rlcHdrEstmt);
      bo= dlLc->bo+rlcHdrEstmt;
      RGSCH_NULL_CHECK(cell->instIdx, schCmnUe->dl.proc);
      schCmnUe->dl.proc->hasDcch = TRUE;
      /* rg007.201 - Changes for MIMO feature addition */
      /* rg008.201 - Removed dependency on MIMO compile-time flag */
      /* rg003.301[ccpu00122038]-MOD- Check for return value */
      ret = rgSCHCmnDlAllocTxRb(cell, sf, ue, bo, &allocBo,schCmnUe->dl.proc,allocInfo);
      if((ret != ROK) || (allocBo == 0))
      {
         if (cmnHqDl->totBytes == 0)
         {
            /* Added the handling for freeing the
             * harq proc, removing UE from txHqPLst and resetting outStndAlloc.*/
            RgSchCmnDlUe *ueDl  = RG_SCH_CMN_GET_DL_UE(ue);
            if(schCmnUe->dl.proc != NULLP)
            {
               /* Fix [ccpu00126847]: Corrected the order of following if check
                  to avoid crash in case of RB allocation failure */
               if(schCmnUe->dl.proc->reqLnk.node != (PTR)NULLP)
               {
                  cmLListDelFrm(&allocInfo->dedAlloc.txHqPLst, 
                                &schCmnUe->dl.proc->reqLnk);
                  schCmnUe->dl.proc->reqLnk.node = (PTR)NULLP;
               }
               rgSCHRrRlsDlHqProc(schCmnUe->dl.proc);
               schCmnUe->dl.proc = NULLP;
            }
            
            /*Re-set the outstanding alloc information.*/
            ueDl->outStndAlloc = 0;
         }
         /* Fix : syed Round Robin with in sigLst */
         rrDlUe->sigLst.crnt = node;
         RETVALUE(ret);
      }

      rgSCHRrCListAdd2Tail(&(rrDlUe->schedLcs), &(rrDlLc->schdLnk));
      rrDlLc->schdLnk.node = (PTR)dlLc;
      rrDlLc->schdBo = allocBo;
      rrDlLc->rlcHdrEstmt = rlcHdrEstmt;
      cmnHqDl->totBytes += allocBo;

      /* Fix : syed Round Robin with in sigLst */
      node = node->next;
      if(node == rrDlUe->sigLst.crnt)
      {
         break;
      }
   }

   node = rrDlUe->dataLst.crnt;
   while(node)
   {
      dlLc = (RgSchDlLcCb *)(node->node);
      schCmnDlLc = (RgSchCmnDlSvc *)(dlLc->sch);
      rrDlLc = (RgSchRrDlLc *)(schCmnDlLc->schSpfc);

      RG_SCH_CMN_DL_GET_HDR_EST(dlLc, rlcHdrEstmt);
      bo= dlLc->bo+rlcHdrEstmt;
      ret = rgSCHCmnDlAllocTxRb(cell, sf, ue, bo, &allocBo, schCmnUe->dl.proc, \
                                allocInfo);
      if((ret != ROK) || (allocBo == 0))
      {
    /* Fix : syed Upon failure release HqP and UE from txHqPLst.
     * We use to just release hqP without removing it from txHqPLst,
     * which was leading to crash in Finalization. This piece of 
     * code can be made as a function instead. Check other scheduler 
     * implementation as well. */
         if (cmnHqDl->totBytes == 0)
         {
            RgSchCmnDlUe *ueDl  = RG_SCH_CMN_GET_DL_UE(ue);
            if(schCmnUe->dl.proc != NULLP)
            {
               /* Fix [ccpu00126847]: Corrected the order of following if
                  check to avoid crash in case of RB allocation failure */
               if(schCmnUe->dl.proc->reqLnk.node != (PTR)NULLP)
               {
                  cmLListDelFrm(&allocInfo->dedAlloc.txHqPLst, &schCmnUe->dl.proc->reqLnk);
                  schCmnUe->dl.proc->reqLnk.node = (PTR)NULLP;
               }
               rgSCHRrRlsDlHqProc(schCmnUe->dl.proc);
               schCmnUe->dl.proc = NULLP;
            }
            
            /*Re-set the outstanding alloc information.*/
            ueDl->outStndAlloc = 0;
         }
         /* Fix : syed Round Robin with in sigLst */
         rrDlUe->dataLst.crnt = node;
         RETVALUE(ret);
      }
      rgSCHRrCListAdd2Tail(&(rrDlUe->schedLcs), &(rrDlLc->schdLnk));
      rrDlLc->schdLnk.node = (PTR)dlLc;
      rrDlLc->schdBo = allocBo;
      rrDlLc->rlcHdrEstmt = rlcHdrEstmt;
      cmnHqDl->totBytes += allocBo;

      /* Fix : syed Round Robin with in sigLst */
      node = node->next;
      if(node == rrDlUe->dataLst.crnt)
      {
         break;
      }
   }

   if((rrDlUe->schedLcs.count == 0) && (rrDlUe->schdTa == FALSE))
   {
      if(schCmnUe->dl.proc != NULLP)
      {
         rgSCHRrRlsDlHqProc(schCmnUe->dl.proc);
         schCmnUe->dl.proc = NULLP;
      }
   }

   RETVALUE(ROK);
}
/**
 * @brief Release the DL HarqProc.
 *
 * @details
 *
 *     Function :  rgSCHRrRlsDlHqProc
 *
 *     Processing Steps:
 *               Toggle the NDI.
 *               Release the Hq Proc
 *
 *  @param[in]  RgSchCmnUe   *schCmnUe
 *  @return  Void
 **/
#ifdef ANSI
PRIVATE Void rgSCHRrRlsDlHqProc
(
RgSchDlHqProcCb    *proc
)
#else
PRIVATE Void rgSCHRrRlsDlHqProc(proc)
RgSchDlHqProcCb    *proc;
#endif
{
   TRC2(rgSCHRrRlsDlHqProc)
   rgSCHDhmRlsHqProc(proc);
   RETVOID;
}
/**
 * @brief Scheduling of TA for the UEs.
 *
 * @details
 *
 *     Function : rgSCHRrDlSchdTa
 *
 *     Processing Steps:
 *     - Traverse the ta list  and do resource allocation
 *       for the UEs.
 *
 *  @param[in]  RgSchCellCb         *cell
 *  @return  Void
 **/
 /* Fix [ccpu00126870]: MUE_PERTTI_DL Limiting number of TA scheduling */
#ifdef ANSI
PRIVATE S16 rgSCHRrDlSchdTa
(
RgSchCellCb             *cell,
RgSchCmnDlRbAllocInfo   *allocInfo
)
#else
PRIVATE S16 rgSCHRrDlSchdTa(cell, allocInfo)
RgSchCellCb             *cell;
RgSchCmnDlRbAllocInfo   *allocInfo;
#endif
{
   RgSchCmnCell *schCmnCell = (RgSchCmnCell *)(cell->sc.sch);
   RgSchDlSf    *dlSf = allocInfo->dedAlloc.dedDlSf;
   RgSchUeCb    *ue;
   RgSchCmnUe   *schCmnUe;
   RgSchRrDlUe  *rrDlUe;
   CmLList *taNode;
   U32 allocBo;
   RgSchCmnDlHqProc *cmnHqDl;
#ifdef LTEMAC_SPS
   CmLteTimingInfo schdTime;
#endif
#ifdef LTEMAC_HDFDD
   Bool                 dlAllowed = FALSE;
#endif
   RgSchRrDlCell  *rrDlCell = (RgSchRrDlCell *)(schCmnCell->dl.schSpfc);
   U32             prbRequrd;

   
   TRC2(rgSCHRrDlSchdTa);

#ifdef LTEMAC_SPS
   schdTime = cell->crntTime;

   /* Increment by DL DELTA to determine the time for which scheduling
    * is done */
   RGSCH_INCR_SUB_FRAME(schdTime, RG_SCH_CMN_DL_DELTA);
#endif
      
   taNode = schCmnCell->dl.taLst.first;
   /* Fix [ccpu00126870]: MUE_PERTTI_DL: Limiting number of TA scheduling */
   while(taNode && dlSf->remUeCnt)
   {
      ue = (RgSchUeCb *)(taNode->node);
      taNode = taNode->next;
#ifdef LTEMAC_HDFDD
      if (ue->hdFddEnbld)
      {
         rgSCHCmnHdFddChkDlAllow ( cell, ue, &dlAllowed);
         if (dlAllowed == FALSE)
         {
            continue;
         }
      }
#endif
      if((ue->dl.dlInactvMask)
#ifdef LTEMAC_SPS
            ||(RG_SCH_CMN_IS_UE_SPS_SCHDLD(ue, schdTime))
#endif
        )
      {
         continue;
      }
      schCmnUe = (RgSchCmnUe *)(ue->sch);
      rrDlUe = (RgSchRrDlUe *)(schCmnUe->dl.schSpfc);

      if(schCmnUe->dl.proc != NULLP)
      {
         continue;
      }

      if(rgSCHDhmGetAvlHqProc(ue,
               schCmnCell->dl.time, &(schCmnUe->dl.proc)) != ROK)
      {
         continue;
      }
      /*ccpu00131191 and ccpu00131317 - Fix for RRC Reconfig failure
       * issue for VoLTE call */
      schCmnUe->dl.proc->hasDcch = TRUE;
      cmnHqDl = RG_SCH_CMN_GET_DL_HQP(schCmnUe->dl.proc);
      cmnHqDl->totBytes = 0;
      if(rgSCHCmnDlAllocTxRb(cell, dlSf, ue,
               RGSCH_TA_SIZE, &allocBo, schCmnUe->dl.proc,\
               allocInfo) != ROK)
      {
         /* TODO: the following 9 lines of code for
          * hqP release is reteated. Can be made as a
          * function */
         rgSCHRrRlsDlHqProc(schCmnUe->dl.proc);
         schCmnUe->dl.proc = NULLP;
         RETVALUE(RFAILED);
      }
      if(allocBo)
      {
         rrDlUe->schdTa = TRUE;
         /* Added the handling for updating the totBtyes.*/
         cmnHqDl->totBytes += allocBo;
         /* Fix [ccpu00126870]: MUE_PERTTI_DL: Scheduling data and signalling
          * bearers as well, As part of 4UePerTti feature, adding Ue to 
          * toBeSchdLst and then scheduling will happen from this list */

         /* Derive total number of PRB required using wideband CQI for the UE*/
         rgSchUtlCalcTotalPrbReq(cell, ue, ue->totalBo, &prbRequrd);
         ue->totalPrbRequired = prbRequrd;

         /* Do not schdule TA Ue here. However add this UE to toBeSchedLst*/
         rgSCHRrDlAddUeToSchdLst(&(rrDlCell->toBeSchedLst), ue);

         /* Fix [ccpu00126870]: MUE_PERTTI_DL: Limiting number of TA scheduling. Please
          * check in remUeCnt handling in oher schedulers as well. */
         (dlSf->remUeCnt)--;
      }      /* Add to UeLst is moved to common Scheduler */
      else
      {
         rgSCHRrRlsDlHqProc(schCmnUe->dl.proc);
         schCmnUe->dl.proc = NULLP;
         continue;
      }
   }       

   RETVALUE(ROK);
}

/* LTE_ADV_FLAG_REMOVED_START */

/**
 * @brief Scheduling of DL new transmissions for prioritized UEs
 * 
 * @details
 *
 *    Function : rgSCHRrDlAddPrioritizedUeToSchLst
 *
 *     Processing Steps:
 *     - Schedule new transmissions for cell edge UEs.
 *
 *    @param[in]  RgSchCellCb            *cell
 *    @param[in]  RgSchCmnDlRbAllocInfo  *allocInfo
 *    @param[in]  Bool                   cellEdgePrior
 *    @return  Void
 **/


#ifdef ANSI
PRIVATE Void rgSCHRrDlAddPrioritizedUeToSchLst
(
 RgSchCellCb             *cell,
 RgSchCmnDlRbAllocInfo   *allocInfo,
 U8                      feature,
 Bool                    vctmPrior,
 RgSchRrCList            *currNode
 )
#else
PRIVATE Void rgSCHRrDlAddPrioritizedUeToSchLst(cell, allocInfo, feature, vctmPrior, currNode)
 RgSchCellCb             *cell;
 RgSchCmnDlRbAllocInfo   *allocInfo;
 U8                      feature;
 Bool                    vctmPrior;
 RgSchRrCList            *currNode;
#endif
{
   RgSchCmnCell     *schCmnCell = (RgSchCmnCell *)(cell->sc.sch);
   RgSchRrDlCell    *rrDlCell   = (RgSchRrDlCell *)(schCmnCell->dl.schSpfc);
   RgSchUeCb        *ue;
   RgSchCmnUe       *schCmnUe;
   RgSchRrDlUe      *rrDlUe;
   RgSchRrCList     *node;
   RgSchRrCList     *crntNode = NULLP;
   U32               prbRequrd;

   TRC2(rgSCHRrDlAddPrioritizedUeToSchLst)

   if (currNode == NULLP)
   {
      RETVOID;
   }
   /*
    * Start loop through transmission list from current node
    */
   node     = currNode;
   crntNode = currNode;
   do
   {
      ue   = (RgSchUeCb *)(node->node);
      node = node->next;

      if(((RGR_SFR == feature) && (ue->lteAdvUeCb.rgrLteAdvUeCfg.isUeCellEdge))
            || ((RGR_ABS == feature) && (ue->lteAdvUeCb.rgrLteAdvUeCfg.isAbsUe))
        )
      {
         /** 
          * If UE is in inactive state, skip the UE from scheduling
          */
         if(ue->dl.dlInactvMask)
         {
            continue;
         }

         schCmnUe  = (RgSchCmnUe *)(ue->sch);
         rrDlUe    = (RgSchRrDlUe *)(schCmnUe->dl.schSpfc);

         /**
          * If UE is already considered for ReTx or if It is Added in the
          * schedule list during TA processing then dont add it again
          */
         if(schCmnUe->dl.proc != NULLP)
         {
            continue;
         }

         /* Check if HQ Proc is available for this UE. If HQ proc is not
          * available then do not add it to scheduled Lst
          */
         if((rrDlUe->sigLst.count) ||
               (rrDlUe->dataLst.count))
         {
            RgSchCmnDlHqProc *cmnHqDl;
            if(rrDlUe->schdTa == FALSE)
            {
               if(rgSCHDhmGetAvlHqProc(ue,
                        schCmnCell->dl.time, &(schCmnUe->dl.proc)) != ROK)
               {
                  continue;
               }
               cmnHqDl = RG_SCH_CMN_GET_DL_HQP(schCmnUe->dl.proc);
               cmnHqDl->totBytes = 0;
            }
         }
         else
         {
            continue;
         }
         
         /* Derive total number of PRB required using wideband CQI for the UE*/
         rgSchUtlCalcTotalPrbReq(cell, ue, ue->totalBo, &prbRequrd);
         ue->totalPrbRequired = prbRequrd;

         /* Add UE to the scheduling list */
         rgSCHRrDlAddUeToSchdLst(&(rrDlCell->toBeSchedLst), ue);

         if(RGR_SFR == feature)
         {
            allocInfo->dedAlloc.dedDlSf->sfrTotalPoolInfo.isUeCellEdge = TRUE;
         }
         if (!(--allocInfo->dedAlloc.dedDlSf->remUeCnt))
         {
            break;
         }
      }
   }while(node != crntNode);

   RETVOID;
}
/* LTE_ADV_FLAG_REMOVED_END */

/**
 * @brief Scheduling of DL new transmissions for prioritized UEs
 * 
 * @details
 *
 *    Function : rgSCHRrDlNewTxOfPrioritizedUe
 *
 *     Processing Steps:
 *     - Schedule new transmissions for cell edge UEs.
 *
 *    @param[in]  RgSchCellCb            *cell
 *    @param[in]  RgSchCmnDlRbAllocInfo  *allocInfo
 *    @param[in]  Bool                   cellEdgePrior
 *    @return  Void
 **/


#ifdef ANSI
PRIVATE Void rgSCHRrDlNewTxOfPrioritizedUe
(
 RgSchCellCb             *cell,
 RgSchCmnDlRbAllocInfo   *allocInfo,
 U8                      feature,
 Bool                    vctmPrior
 )
#else
PRIVATE Void rgSCHRrDlNewTxOfPrioritizedUe(cell, allocInfo, feature, vctmPrior)
   RgSchCellCb             *cell;
   RgSchCmnDlRbAllocInfo   *allocInfo;
   U8                      feature;
   Bool                    vctmPrior;
#endif
{
   RgSchCmnCell         *schCmnCell;
   RgSchRrDlCell        *rrDlCell;

   TRC2(rgSCHRrDlNewTxOfPrioritizedUe)

   /* Initialization of local variables */
   schCmnCell = (RgSchCmnCell *)(cell->sc.sch);
   rrDlCell = (RgSchRrDlCell *)(schCmnCell->dl.schSpfc);
   
   /* Schedule the UEs in both priority and non-Priority Lists */
   rgSCHRrDlAddPrioritizedUeToSchLst(cell, allocInfo, feature, vctmPrior, rrDlCell->prioUeTxLst.crnt);
   rgSCHRrDlAddPrioritizedUeToSchLst(cell, allocInfo, feature, vctmPrior, rrDlCell->normUeTxLst.crnt);

   RETVOID;
}
/* LTE_ADV_FLAG_REMOVED_END */

/**
 * @brief Distrubute PRBs euqally across UEs prsent in scheduled list and
 *        Schedule UEs from scheduled list
 *
 * @details
 *
 *     Function : rgSCHRrDlScheduleUEs
 *
 *     Processing Steps:
 *     - Calculate available number of PRBs 
 *     - Calculate Per UE share 
 *     - Loop through the UEs selected in the scheduled list
 *     - Get the minimum of maxRb and PerUEPRBShare
 *     - Schedule a UE
 *     - If Required RBs is less than PerUeShare then add remainig delta 
 *       between rest of the UEs
 *
 *  @param[in]  RgSchCellCb              *cell
 *  @param[in]  RgSchDlSf                *sf
 *  @param[in]  RgSchCmnDlRbAllocInfo    *allocInfo
 *  @return  Void
 **/
#ifdef ANSI
PRIVATE Void rgSCHRrDlScheduleUEs
(
RgSchCellCb             *cell,
RgSchDlSf               *sf,
RgSchCmnDlRbAllocInfo   *allocInfo
)
#else
PRIVATE Void rgSCHRrDlScheduleUEs(cell, sf, allocInfo)
RgSchCellCb             *cell;
RgSchDlSf               *sf;
RgSchCmnDlRbAllocInfo   *allocInfo;
#endif
{
   RgSchCmnCell         *schCmnCell;
   RgSchRrDlCell        *rrDlCell;
   RgSchUeCb            *ue;
   RgSchCmnUe           *cmnUe;
   RgSchRrDlUe          *rrUe;
   U32                   prbSharePerUe;
   U32                   availNumOfPrbs;
   U32                    tmpMaxRb=0;
   RgSchRrCList         *node;
   U8                    nmbOfUeToBeSchd;

   TRC2(rgSCHRrDlScheduleUEs);

   /* Initialization of local variables */
   schCmnCell = (RgSchCmnCell *)(cell->sc.sch);
   rrDlCell = (RgSchRrDlCell *)(schCmnCell->dl.schSpfc);
   nmbOfUeToBeSchd = rrDlCell->toBeSchedLst.count;

   /*-- Equal number of PRBs distribution and scheduling starts from here---*/
   /* Scan through the Scheduled UE list and schedule one by one*/
   node = rrDlCell->toBeSchedLst.first;
   do
   {
      ue = (RgSchUeCb *)(node->node);
      cmnUe = (RgSchCmnUe *)(ue->sch);
      rrUe = (RgSchRrDlUe *)(cmnUe->dl.schSpfc);
      node = node->next;

      availNumOfPrbs = (sf->bw - sf->bwAssigned);
      /* if not exactly divisible then the remaning will be considered
       * for the remaining*/
      prbSharePerUe = availNumOfPrbs / nmbOfUeToBeSchd;
      tmpMaxRb = cmnUe->dl.maxRb;
      cmnUe->dl.maxRb = RGSCH_MIN(tmpMaxRb, prbSharePerUe);
      /* Schedule the UE, No need to check return value of below func as 
       * we are making sure that it is invoked only when BW is there*/
      rgSCHRrDlSchdUe(cell, ue, sf, allocInfo);
      
      /* Scheduling is done for this Ue hence delete it from schedLst*/
      //rgSCHRrCListDelFrm(&(rrDlCell->toBeSchedLst), &(rrUe->toBeSchdLnk));
      rrUe->toBeSchdLnk.node = NULLP;

      cmnUe->dl.maxRb = tmpMaxRb;

      if(!(--nmbOfUeToBeSchd))
      {
         /*Break if no more UE to be Sched*/
         break;
      }
   }while(node != (rrDlCell->toBeSchedLst.first));

   RETVOID;
} /*rgSCHRrDlScheduleUEs */

/**
*
*       Fun:   rgSCHRrDlAddUeToSchdLst
*
*       Desc:  This function inserts an UE in ascending order of number of
*              PRBs required for that UE
*
*     Processing Steps:
*      - if first node is null then add node at front of lst
*      - if num of PRBs of ueToBeInsrtd is > lastNodeInLst then add node at 
*        the end of the lst
*      - if none of the above then scan through the lst and add node at 
*        the correct position in the lst
*
*  @param[in]  RgSchRrCListCp   *toBeSchedLst
*  @param[in]  RgSchUeCb        *ueToBeInsrtd
*  @return  Void
*/
#ifdef ANSI
PRIVATE Void rgSCHRrDlAddUeToSchdLst
(
RgSchRrCListCp *toBeSchedLst,
RgSchUeCb      *ueToBeInsrtd
)
#else 
PRIVATE Void rgSCHRrDlAddUeToSchdLst(toBeSchedLst, ueToBeInsrtd)
RgSchRrCListCp *toBeSchedLst;
RgSchUeCb      *ueToBeInsrtd;
#endif
{
   RgSchCmnUe    *schCmnUe          = (RgSchCmnUe *)(ueToBeInsrtd->sch);
   RgSchRrDlUe   *rrDlUe            = (RgSchRrDlUe *)(schCmnUe->dl.schSpfc);
   RgSchUeCb     *firstUeInLst;
   RgSchRrCList  *lastNode;
   RgSchUeCb     *lastUeInLst;

   TRC2(rgSCHRrDlAddUeToSchdLst);

   if(toBeSchedLst->first == NULLP)
   {
      /* Adding first node to the lst*/
      rgSCHRrCListAdd2Tail(toBeSchedLst, &(rrDlUe->toBeSchdLnk));
      rrDlUe->toBeSchdLnk.node = (PTR)ueToBeInsrtd;
      RETVOID;
   }
   lastNode     = (toBeSchedLst->first->prev);
   lastUeInLst  = (RgSchUeCb *)(lastNode->node);
   firstUeInLst = (RgSchUeCb *)(toBeSchedLst->first->node);
   if((ueToBeInsrtd->totalPrbRequired) >= (lastUeInLst->totalPrbRequired))
   {
      /* Inserting node at the end of the list*/
      rgSCHRrCListAdd2Tail(toBeSchedLst, &(rrDlUe->toBeSchdLnk));
      rrDlUe->toBeSchdLnk.node = (PTR)ueToBeInsrtd;
   }
   else if((ueToBeInsrtd->totalPrbRequired) <= (firstUeInLst->totalPrbRequired))
   {
      rgSCHRrCListInsrtAtCrnt(toBeSchedLst, &(rrDlUe->toBeSchdLnk));
      rrDlUe->toBeSchdLnk.node = (PTR)ueToBeInsrtd;
   }
   else
   {
      RgSchRrCList   *tmpNode;
      RgSchUeCb      *tmpUe;
      RgSchCmnUe     *tmpSchCmnUe;
      RgSchRrDlUe    *tmpRrDlUe;
      /*get second node and scan through the list till second last node of list
       * and insert node at right position*/
      tmpNode = toBeSchedLst->first->next;
      while(tmpNode != (toBeSchedLst->first))
      {
         tmpUe        = (RgSchUeCb *)(tmpNode->node);
         tmpSchCmnUe  = (RgSchCmnUe *)(tmpUe->sch);
         tmpRrDlUe    = (RgSchRrDlUe *)(tmpSchCmnUe->dl.schSpfc);
         toBeSchedLst->crnt = tmpNode;
         if((ueToBeInsrtd->totalPrbRequired) <= (tmpUe->totalPrbRequired))
         {
            rgSCHRrCListInsrtAtCrnt(toBeSchedLst, &(rrDlUe->toBeSchdLnk));
            rrDlUe->toBeSchdLnk.node = (PTR)ueToBeInsrtd;
            break;
         }
         tmpNode = tmpNode->next;
      }
   }
   RETVOID;
} /*rgSCHRrDlAddUeToSchdLst*/


#ifdef ANSI
PRIVATE Void rgSCHRrDlSelectDlUes
(
RgSchCellCb    *cell,
RgSchCmnCell   *schCmnCell,
RgSchDlSf      *dlSf
)
#else
PRIVATE Void rgSCHRrDlSelectDlUes(cell, schCmnCell, dlSf)
RgSchCellCb    *cell;
RgSchCmnCell   *schCmnCell;
RgSchDlSf      *dlSf;
#endif
{
   RgSchRrDlCell     *rrDlCell =  (RgSchRrDlCell *)(schCmnCell->dl.schSpfc);
   CmLteTimingInfo   schdTime = cell->crntTime;
   
   TRC2(rgSCHRrDlSelectDlUes)

#ifdef LTEMAC_SPS
   /* Increment by DL DELTA to determine the time for which scheduling
    * is done */
   RGSCH_INCR_SUB_FRAME(schdTime, RG_SCH_CMN_DL_DELTA);
#endif
   if (rgSCHCmnIsDlCsgPrio(cell) == TRUE)
   {
      rgSCHRrDlUpdateSchedList(cell, rrDlCell, dlSf, schdTime, &rrDlCell->prioUeTxLst);
      if (dlSf->totPrbReq < dlSf->bw)
      {
         /* No Further scheduling for UEs in the lower priority list */
         rgSCHRrDlUpdateSchedList(cell, rrDlCell, dlSf, schdTime, &rrDlCell->normUeTxLst);
      }
   }
   else
   {
      rgSCHRrDlUpdateSchedList(cell, rrDlCell, dlSf, schdTime, &rrDlCell->normUeTxLst);
      if (dlSf->totPrbReq < dlSf->bw)
      {
         /* No Further scheduling for UEs in the lower priority list */
         rgSCHRrDlUpdateSchedList(cell, rrDlCell, dlSf, schdTime, &rrDlCell->prioUeTxLst);
       }
   }

   RETVOID;
   
} /* End of rgSCHRrDlSelectDlUes */


/**
 * @brief RR DL Scheduling UE list creation
 *
 * @details
 *
 *     Function : rgSCHRrDlUpdateSchedList
 *
 *     Processing Steps:
 *     1)Derive the total number of PRBS required for the total BO using wideband CQI
 *     2)Add to the toBeSchedLst in sorted manner of required PRBs
 *     3)Increment the selected UE count
 *     4)Repeat above steps until sf->remUECnt becomes equal to remUECnt and then 
 *      cnrt=node->next
 *
 *  @param[in]  RgSchCellCb            *cell
 *  @param[in]  RgSchCmnDlRbAllocInfo  *allocInfo
 *  @return  Void
 **/
#ifdef ANSI
PRIVATE Void rgSCHRrDlUpdateSchedList
(
RgSchCellCb   *cell,
RgSchRrDlCell *rrDlCell,
RgSchDlSf         *dlSf,
CmLteTimingInfo   schdTime,
RgSchRrCListCp    *txListCp
)
#else
PRIVATE Void rgSCHRrDlUpdateSchedList(cell, rrDlCell, dlSf,schdTime,txListCp)
RgSchCellCb   *cell;
RgSchRrDlCell *rrDlCell;
RgSchDlSf     *dlSf;
CmLteTimingInfo   schdTime;
RgSchRrCListCp    *txListCp;
#endif
{
   RgSchCmnCell           *schCmnCell = (RgSchCmnCell *)(cell->sc.sch);
   RgSchUeCb              *ue;
   RgSchCmnUe             *schCmnUe;
   RgSchRrDlUe            *rrDlUe;
   RgSchRrCList           *node;
   RgrLteAdvancedUeConfig *rgrLteAdvUeCfg = NULLP;
   U32                     prbRequrd;
   U32                     availBw;
#ifdef LTEMAC_HDFDD
   Bool               dlAllowed = FALSE;
#endif

   TRC2(rgSCHRrDlUpdateSchedList);

   if(txListCp->crnt == NULLP || !(dlSf->remUeCnt))
   {
      RETVOID;
   }

   availBw = dlSf->bw - dlSf->bwAssigned;
   /* Scan through the prioUeTxLst and add Ue to toBeSchedLst if it belongs to
    * dynamic new transmission*/
   node = txListCp->crnt;
   do
   {
      ue = (RgSchUeCb *)(node->node);
      node = node->next;
#ifdef LTEMAC_HDFDD
      if (ue->hdFddEnbld)
      {
         rgSCHCmnHdFddChkDlAllow ( cell, ue, &dlAllowed);
         if (dlAllowed == FALSE)
         {
            continue;
         }
      }
#endif
      rgrLteAdvUeCfg = &(ue->lteAdvUeCb.rgrLteAdvUeCfg);
      /* LTE_ADV_FLAG_REMOVED_START */
      /** 
       * When ABS is enabled, donot schedule the ABS UE's in Non-ABS SF.
       */
      if(RG_SCH_ABS_ENABLED_NONABS_SF == cell->lteAdvCb.absDlSfInfo) 
      {
         if(cell->lteAdvCb.absCfg.absPatternType & RGR_ABS_TRANSMIT) 
         {
            if(rgrLteAdvUeCfg->isAbsUe) 
            {
               continue;
            }  
         }
      }
      /* LTE_ADV_FLAG_REMOVED_END */

      schCmnUe = (RgSchCmnUe *)(ue->sch);
      rrDlUe = (RgSchRrDlUe *)(schCmnUe->dl.schSpfc);
      
      /* If Ue is already added during prioritization of ABS and SFR then
       * ignore it here*/
      if(rrDlUe->toBeSchdLnk.node != NULLP)
      {
         if(rrDlCell->toBeSchedLst.count == 0)
             RLOG_ARG1(L_ERROR,DBG_CELLID,cell->cellId,
                        "UE already present count(ERROR) CRNTI:%d",ue->ueId);
         continue;
      }

      if((ue->dl.dlInactvMask)
#ifdef LTEMAC_SPS
            ||(RG_SCH_CMN_IS_UE_SPS_SCHDLD(ue, schdTime))
#endif
        )
      {
         continue;
      }

      /* Fix [ccpu00126870]: Consider TA as well for limiting number of UEs
       * per TTI, UE already selected for TA, also dont add if it is Retx Ue */
      if (schCmnUe->dl.proc) 
      {
         continue;
      }

      /* Check if HQ Proc is available for this UE. If HQ proc is not
       * available then do not add it to scheduled Lst
       */
      if((rrDlUe->sigLst.count) ||
            (rrDlUe->dataLst.count))
      {
         RgSchCmnDlHqProc *cmnHqDl;
         if(rrDlUe->schdTa == FALSE)
         {
            if(rgSCHDhmGetAvlHqProc(ue,
                     schCmnCell->dl.time, &(schCmnUe->dl.proc)) != ROK)
            {
               continue;
            }
            cmnHqDl = RG_SCH_CMN_GET_DL_HQP(schCmnUe->dl.proc);
            cmnHqDl->totBytes = 0;
         }
      }
      else
      {
         continue;
      }

      /* Derive total number of PRB required using wideband CQI for the UE*/
      rgSchUtlCalcTotalPrbReq(cell, ue, ue->totalBo, &prbRequrd);
      ue->totalPrbRequired = prbRequrd;
      dlSf->totPrbReq += prbRequrd;
#ifdef BEST_EFFORT_2_UE_PER_TTI 
      dlSf->beTotPrbReq += prbRequrd;
#endif
      /* Add node to the scheduled list*/
      rgSCHRrDlAddUeToSchdLst(&(rrDlCell->toBeSchedLst), ue);

      /* Adding UE into prioUeTxLst is done in cmn scheduler */
      /* Fix [ccpu00126870]: Consider TA as well for limiting number of 
       * UEs per TTI */
      if(!(--dlSf->remUeCnt))
      {
         txListCp->crnt = node;
         break;
      }
#ifdef BEST_EFFORT_2_UE_PER_TTI
      if(dlSf->beTotPrbReq >= dlSf->bw)
      {
         /* Falling back to Old 3.0 2 UE/TTI for Intel SoC */
         break;
      }
#endif

   }while( node != txListCp->crnt);
   RETVOID;

} /* rgSCHRrDlUpdateSchedList*/


/**
 * @brief Scheduling of DL new transmissions
 *
 * @details
 *
 *     Function : rgSCHRrDlNewTx
 *
 *     Processing Steps:
 *     - Schedule TA for UEs
 *     - Schedule new transmissions for UEs.
 *
 *  @param[in]  RgSchCellCb            *cell
 *  @param[in]  RgSchCmnDlRbAllocInfo  *allocInfo
 *  @return  Void
 **/
#ifdef ANSI
PRIVATE Void rgSCHRrDlNewTx
(
RgSchCellCb             *cell,
RgSchCmnDlRbAllocInfo   *allocInfo
)
#else
PRIVATE Void rgSCHRrDlNewTx(cell, allocInfo)
RgSchCellCb             *cell;
RgSchCmnDlRbAllocInfo   *allocInfo;
#endif
{
   RgSchCmnCell      *schCmnCell = (RgSchCmnCell *)(cell->sc.sch);
   RgSchRrDlCell     *rrDlCell = (RgSchRrDlCell *)(schCmnCell->dl.schSpfc);
   RgSchDlSf         *dlSf = allocInfo->dedAlloc.dedDlSf;
   U16               availableBW = (dlSf->bw - dlSf->bwAssigned);
   /* Fix: MUE_PERTTI_DL: cap off new tranmsisisons by total number of UEs per TTI */
   U8               excessUeCnt = 0;
#ifdef LTEMAC_HDFDD
   Bool             dlAllowed = FALSE;
#endif

   TRC2(rgSCHRrDlNewTx);

   dlSf->totPrbReq = dlSf->bwAssigned;
#ifdef BEST_EFFORT_2_UE_PER_TTI
   dlSf->beTotPrbReq =  0;
#endif
   dlSf->numDlActvUes  =  rrDlCell->prioUeTxLst.count + 
                          rrDlCell->normUeTxLst.count;  /* 4UE_TTI_DELTA */
   if ((!dlSf->remUeCnt) || (!dlSf->numDlActvUes) || (!availableBW))
   {
      /* Schedule the UE's which got added to the list
       * during TA scheduling. Scheduling resources need to be
       * released if BW is not availale. Hence calling this function
       * */
      if(rrDlCell->toBeSchedLst.count)
      {
         /* schedule all the UEs Present in the scheduling list*/
         rgSCHRrDlScheduleUEs(cell, dlSf, allocInfo);
      }

      /*Return as either remUeCnt is 0 or no Ue in Tx lst or Bandwidth is 0*/
      RETVOID;
   }
   if (schCmnCell->dl.maxUeNewTxPerTti < dlSf->remUeCnt)
   {
      excessUeCnt = dlSf->remUeCnt - schCmnCell->dl.maxUeNewTxPerTti;      
      dlSf->remUeCnt = schCmnCell->dl.maxUeNewTxPerTti;
   }
   /* If available bandwidth count(no of PRB)< remUeCnt then update remUeCnt*/
   if(availableBW < (dlSf->remUeCnt))
   {
      excessUeCnt += (dlSf->remUeCnt - availableBW);
      dlSf->remUeCnt = availableBW; 
   }
   
/* LTE_ADV_FLAG_REMOVED_START */
   /** 
    * When SFR enabled, schedule the cell edge UE's in reserved bandwidth and 
    * then schedule cell center UEs.
    */
   if(cell->lteAdvCb.sfrCfg.status)
   {
      rgSCHRrDlNewTxOfPrioritizedUe(cell, allocInfo, RGR_SFR, TRUE);
   }
/* LTE_ADV_FLAG_REMOVED_END */

/* LTE_ADV_FLAG_REMOVED_START */
   /** 
    * When ABS is enabled, schedule the ABS UE's ABS SF.
    */
   if(RG_SCH_ABS_ENABLED_ABS_SF == cell->lteAdvCb.absDlSfInfo)
   {
      if (dlSf->remUeCnt)
      {
         rgSCHRrDlNewTxOfPrioritizedUe(cell, allocInfo, RGR_ABS, TRUE);
      }
   }
/* LTE_ADV_FLAG_REMOVED_END */

   /* Create scheduling list in acensing order of PRB required for the UEs */
   if (dlSf->remUeCnt)
   {
      rgSCHRrDlSelectDlUes(cell, schCmnCell, dlSf);
   }

   if(rrDlCell->toBeSchedLst.count)
   {
      /* schedule all the UEs Present in the scheduling list*/
      rgSCHRrDlScheduleUEs(cell, dlSf, allocInfo);
   }

   /* Fix [ccpu00126870]: Updating the remUeCnt based on current allocation */
   dlSf->remUeCnt += excessUeCnt;
   RETVOID;
}

/**
 * @brief Round Robin related scheduling for a cell
 *
 * @details
 *
 *     Function : rgSCHRrDlSchd
 *
 *     Processing Steps:
 *     - Schedule the re-transmission UEs by invoking rgSCHRrDlRetx
 *     - Schedule the new transmission UEs by invoking rgSCHRrDlNewTx
 *
 *  @param[in]  RgSchCellCb            *cell
 *  @param[in]  RgSchCmnDlRbAllocInfo  *allocInfo
 *  @return  Void
 **/
#ifdef ANSI
PUBLIC Void rgSCHRrDlSchd
(
RgSchCellCb             *cell,
RgSchCmnDlRbAllocInfo   *allocInfo
)
#else
PUBLIC Void rgSCHRrDlSchd(cell, allocInfo)
RgSchCellCb             *cell;
RgSchCmnDlRbAllocInfo   *allocInfo;
#endif
{
   RgSchCmnCell *schCmnCell = (RgSchCmnCell *)(cell->sc.sch);
   RgSchRrDlCell *rrDlCell = (RgSchRrDlCell *)(schCmnCell->dl.schSpfc);

   TRC2(rgSCHRrDlSchd);

   /* Initialize Sched list in the start of the TTI*/
   rgSCHRrCListInit(&(rrDlCell->toBeSchedLst));
   rgSCHRrDlSchdTa(cell, allocInfo);
   rgSCHRrDlRetx(cell, allocInfo);
    /* Fix: If this is considered as a dummy TTI then the scheduling for new 
    * transmissions for all the UEs are blocked to improve the performance 
    * and pull the system from L2 processing going beyond the TTI boundary 
    * Please port this fix to other schedulers as well. 
    */
   if (!cell->isDummyTti)
   {
      rgSCHRrDlNewTx(cell, allocInfo);
   }
   RETVOID;
}
/* 3.1 MIMO Alloc distribution functions being performed
 * TB wise */

/***********************************************************
 *
 * @brief Dl allocation finalization for the UE.
 *
 * @details
 *
 *     Function : rgSCHRrDlNewTxTbDstn
 *
 *     Processing Steps:
 *     - Finalize the UE allocation.
 *     - If TA scheduled then remove from TA list.
 *     - Update the schedule BO for the scheduled LCs.
 *
 *  @param[in]  RgSchCellCb       *cell
 *  @param[in]  RgSchUeCb         *ue
 *  @param[in]  RgSchDlHqTbCb  *tbInfo,
 *  @param[in]  U32                    *effAlloc,
 *  @param[in]  CmLList              **node
 *
 *     Ret  : Void
 *
 *     Notes:
 *
 *     File :
 *
 **********************************************************/
#ifdef ANSI
PRIVATE Void rgSCHRrDlNewTxTbDstn
(
RgSchCellCb    *cell,
RgSchUeCb      *ue,
RgSchDlHqTbCb  *tbInfo,
U32            *effAlloc,
RgSchRrCList        **node
)
#else
PRIVATE Void rgSCHRrDlNewTxTbDstn(cell, ue, tbInfo, effAlloc, node)
RgSchCellCb    *cell;
RgSchUeCb      *ue;
RgSchDlHqTbCb  *tbInfo;
U32            *effAlloc;
RgSchRrCList        **node;
#endif
{
   RgSchDlLcCb *dlLc;
   RgSchCmnDlSvc *schCmnDlLc;
   RgSchRrDlLc *rrDlLc;
   U32   bo;
   RgSchLchAllocInfo   lchSchdData;
   RgSchCmnUe *schCmnUe = (RgSchCmnUe *)(ue->sch);
   RgSchRrDlUe  *rrDlUe = (RgSchRrDlUe *)(schCmnUe->dl.schSpfc);
   RgSchRrCList *lnk = *node;
   RgSchRrCList *prev = NULLP;
   U32          lcTotalBo;


   TRC2(rgSCHRrDlNewTxTbDstn);

   if(!tbInfo->schdTa.pres)
   {
      if(rrDlUe->schdTa == TRUE)
      {
         if(*effAlloc >= RGSCH_TA_SIZE)
         {
            *effAlloc -= RGSCH_TA_SIZE;
            rgSCHCmnRmvFrmTaLst(cell, ue);
            rgSCHDhmSchdTa(ue, tbInfo);
            rrDlUe->schdTa = FALSE;
         }
         else if(rrDlUe->schedLcs.first == NULLP)
         {
            RETVOID;
         }
      }
   }

   while((*node) && (*effAlloc > 0))
   {
      dlLc = (RgSchDlLcCb *)(*node)->node;
      schCmnDlLc = (RgSchCmnDlSvc *)(dlLc->sch);
      rrDlLc = (RgSchRrDlLc *)(schCmnDlLc->schSpfc);
      prev = *node;
      *node = (*node)->next;
      /* Tb size can get increased or decresed during rb allocation phase
       * Inorder to reduce the padding, considering the actual bo instead
       * of initial estimated bo.
       * RGSCH_MAX will be usefull in case of RGU LC/LWLC*/
      lcTotalBo = (dlLc->bo) ? (dlLc->bo + rrDlLc->rlcHdrEstmt):(0);
      bo = RGSCH_MIN(RGSCH_MAX(lcTotalBo, rrDlLc->schdBo), *effAlloc);
      /* ccpu00133680: Minimum alloc required by RLC per LC*/
      if(bo < RGSCH_MIN_ALLOC_PER_LC)
      {
         break;
      }

      /* 3.1 MIMO updating the reqBytes field */
      rrDlLc->schdBo -= bo;

      /* Update the totalBo with the scheduled bo */
      (ue->totalBo <= bo-rrDlLc->rlcHdrEstmt)?\
        (ue->totalBo = 0):\
        (ue->totalBo -= bo-rrDlLc->rlcHdrEstmt);
      
      (dlLc->bo <= bo-rrDlLc->rlcHdrEstmt)?\
        (dlLc->bo = 0):\
        (dlLc->bo -= bo-rrDlLc->rlcHdrEstmt);

#ifdef LTE_L2_MEAS
      /* Increment qciActiveLCs once since this LCs buffer will be present
         in Harq process */
      if ((dlLc->bo != 0) && (dlLc->lcType == CM_LTE_LCH_DTCH))
      {
         ue->qciActiveLCs[dlLc->qciCb->qci]++;
      }
#endif

      *effAlloc -= bo;

      /* Update DHM for this SVC */
      lchSchdData.schdData = bo;
      lchSchdData.lcId = dlLc->lcId;

      if(rgSCHDhmAddLcData(cell->instIdx, &lchSchdData,
            tbInfo) != ROK)
      {
         RLOG_ARG2(L_ERROR,DBG_CELLID,cell->cellId,
            "Failed to update DHM CRNTI:%d LCID:%d",ue->ueId,dlLc->lcId);
      }

      if(dlLc->bo == 0)
      {
         if((dlLc->lcId >= RG_RR_MIN_DCCH_LCID) &&
            (dlLc->lcId <= RG_RR_MAX_DCCH_LCID))
         {
            rgSCHRrCListDelFrm(&(rrDlUe->sigLst), &(rrDlLc->lnk));
         }
         else
         {
            rgSCHRrCListDelFrm(&(rrDlUe->dataLst), &(rrDlLc->lnk));
         }
         rrDlLc->lnk.node = NULLP;
      }
      /* node is a circuler linked list. Starting address is stored in lnk,
       * when node reaches at the beining of CList,it should come out*/
      if (lnk == *node)
      {
         if(dlLc->bo == 0)
         {/* No further LC present to be distributed*/
           prev = NULLP;
         }
         break;
      }
   }
   /* make sure node points to the svc not completely
    * satisfied.
    * make sure if not served completely then
    * the other TB allocation accomodates the same */
   *node = prev;
   RETVOID;
}


/***********************************************************
 *
 * @brief Dl allocation finalization for the UE.
 *
 * @details
 *
 *     Function : rgSCHRrDlSprTxTbDstn
 *
 *     Desc : Perform Actual allocation distribution among
 *     UEs schd svcs and TA for a given spare TB "tbInfo" allocation.
 *     spare TB allocation is as a result of 1 RETX TB allocation, when
 *     conditions are favourable for 2 TB spatial multiplexing.
 *
 *  @param[in]  RgSchCellCb       *cell
 *  @param[in]  RgSchUeCb         *ue
 *  @param[in]  RgSchDlHqTbCb     *tbInfo,
 *  @param[in]  U32               *effAlloc,
 *  @param[in/out] RgSchRrCListCp *lcLst;
 *
 *     Ret  : Void
 *
 *     Notes:
 *
 *     File :
 *
 **********************************************************/
#ifdef ANSI
PRIVATE Void rgSCHRrDlSprTxTbDstn
(
RgSchCellCb    *cell,
RgSchUeCb      *ue,
RgSchDlHqTbCb  *tbInfo,
U32            *effAlloc,
RgSchRrCListCp *lcLst
)
#else
PRIVATE Void rgSCHRrDlSprTxTbDstn(cell, ue, tbInfo, effAlloc, lcLst)
RgSchCellCb    *cell;
RgSchUeCb      *ue;
RgSchDlHqTbCb  *tbInfo;
U32            *effAlloc;
RgSchRrCListCp *lcLst;
#endif
{
   RgSchDlLcCb *dlLc;
   RgSchCmnDlSvc *schCmnDlLc;
   RgSchRrDlLc *rrDlLc;
   U32   bo;
   RgSchLchAllocInfo   lchSchdData;
   U32     effBo;


   TRC2(rgSCHRrDlSprTxTbDstn);


   /* Fix: syed List Corruption issue */
   while(lcLst->crnt)
   {
      if(*effAlloc == 0)
      {
         break;
      }
      dlLc = (RgSchDlLcCb *)(lcLst->crnt)->node;
      schCmnDlLc = (RgSchCmnDlSvc *)(dlLc->sch);
      rrDlLc = (RgSchRrDlLc *)(schCmnDlLc->schSpfc);
      effBo= dlLc->bo + rrDlLc->rlcHdrEstmt;
      bo = (effBo < *effAlloc) ?
            effBo : *effAlloc;

      if(bo <= rrDlLc->rlcHdrEstmt)
      {
         break;
      }
      if((bo - rrDlLc->rlcHdrEstmt) > ue->totalBo)
      {
         ue->totalBo = 0;
      }
      else
      {
         ue->totalBo -= (bo - rrDlLc->rlcHdrEstmt);
      }

      if((bo - rrDlLc->rlcHdrEstmt) > dlLc->bo)
      {
         lchSchdData.schdData = bo;
         dlLc->bo = 0;
      }
      else
      {
         lchSchdData.schdData = bo;
         dlLc->bo -= (bo - rrDlLc->rlcHdrEstmt);
      }
      /* L2_COUNTERS */
#ifdef LTE_L2_MEAS
      /* Increment qciActiveLCs once since this LCs buffer will be present
         in Harq process */
      if (dlLc->lcType == CM_LTE_LCH_DTCH)
      {
         ue->qciActiveLCs[dlLc->qciCb->qci]++;
      }

      /* If LCs bo has become zero due to this allocation decrement qciActiveLCs once */
      if ((dlLc->bo == 0)&& (dlLc->lcType == CM_LTE_LCH_DTCH))
      {
         if (ue->qciActiveLCs[dlLc->qciCb->qci])
         {
            ue->qciActiveLCs[dlLc->qciCb->qci]--;
         }
         if (!(ue->qciActiveLCs[dlLc->qciCb->qci]))
         {
            dlLc->qciCb->dlUeCount--;
         }
      }
#endif

      *effAlloc -= bo;
      lchSchdData.lcId = dlLc->lcId;
      if(rgSCHDhmAddLcData(cell->instIdx, &lchSchdData,
            tbInfo) != ROK)
      {
         RLOG_ARG2(L_ERROR,DBG_CELLID,cell->cellId,
            "Failed to update DHM CRNTI:%d LCID:%d",ue->ueId,dlLc->lcId);
      }

      if(dlLc->bo == 0)
      {
         rgSCHRrCListDelFrm(lcLst, &(rrDlLc->lnk));
         rrDlLc->lnk.node = NULLP;
      }
   }
   RETVOID;
}

/**
 * @brief Dl allocation finalization for the UE.
 *
 * @details
 *
 *     Function : rgSCHRrDlUeAllocFnlz
 *
 *     Processing Steps:
 *     - Finalize the UE allocation.
 *     - If TA scheduled then remove from TA list.
 *     - Update the schedule BO for the scheduled LCs.
 *
 *  @param[in]  RgSchCellCb       *cell
 *  @param[in]  RgSchUeCb         *ue
 *  @return  S16
 *      -# ROK
 *      -# RFAILED
 **/
#ifdef ANSI
PRIVATE S16 rgSCHRrDlUeAllocFnlz
(
RgSchCellCb       *cell,
RgSchUeCb         *ue
)
#else
PRIVATE S16 rgSCHRrDlUeAllocFnlz(cell, ue)
RgSchCellCb       *cell;
RgSchUeCb         *ue;
#endif
{
   RgSchCmnCell  *schCmnCell = (RgSchCmnCell *)(cell->sc.sch);
   RgSchRrDlCell *rrDlCell = (RgSchRrDlCell *)(schCmnCell->dl.schSpfc);
   RgSchCmnUe    *schCmnUe = (RgSchCmnUe *)(ue->sch);
   RgSchRrDlUe   *rrDlUe = (RgSchRrDlUe *)(schCmnUe->dl.schSpfc);
   RgSchRrCList  *node;
   /* 3.1 MIMO Distribute data of each TB across services */
   U32                remTb1Bytes = ue->dl.dlAllocCb.tbInfo[0].bytesAlloc;
   U32                remTb2Bytes = ue->dl.dlAllocCb.tbInfo[1].bytesAlloc;
   U32                effAlloc = 0;
   /*ccpu00120365-ADD-added to code to check if second TB is utilized */
   U32                tb2Bytes = 0;

   TRC2(rgSCHRrDlUeAllocFnlz);



   /* 3.1 MIMO Consider the allocation of New TX TB for distribution */
   /* Handle schd services */
   node = rrDlUe->schedLcs.first;
   if (remTb1Bytes){
      effAlloc += remTb1Bytes;
      rgSCHRrDlNewTxTbDstn(cell, ue, &schCmnUe->dl.proc->tbInfo[0],\
                            &remTb1Bytes, &node);
      /* In the event that TB1 is not completely filled by the DL LCs
       * BO, consider the reducing the iMcs for increasing redundancy
       * and hence reception quality at UE */
      rgSCHCmnRdcImcsTxTb(&ue->dl.dlAllocCb, 0,
           ue->dl.dlAllocCb.tbInfo[0].bytesAlloc - remTb1Bytes);

   }


   /*ccpu00120365-ADD-assigning value of remTb2Bytes before utilization */
   tb2Bytes = remTb2Bytes;

   /* Extra check for a non SM UE allocation */
   if (remTb2Bytes){
      effAlloc += remTb2Bytes;
      rgSCHRrDlNewTxTbDstn(cell, ue, &schCmnUe->dl.proc->tbInfo[1],\
                            &remTb2Bytes, &node);
      /* In the event that TB2 is not completely filled by the DL LCs
       * BO, consider the reducing the iMcs for increasing redundancy
       * and hence reception quality at UE */
      rgSCHCmnRdcImcsTxTb(&ue->dl.dlAllocCb, 1,
           ue->dl.dlAllocCb.tbInfo[1].bytesAlloc - remTb2Bytes);

   }

   /* ccpu00120365-ADD-Disable the second TB as the second TB is not
    * utilized */
   if ((remTb2Bytes) && (tb2Bytes == remTb2Bytes))
   {
      /* If few byte allocation satisfied within TB1
       * then single layer Transmit Diversity is chosen
       * for TM3 and TM4 */
      ue->dl.dlAllocCb.mimoAllocInfo.precIdxInfo = 0;
      ue->dl.dlAllocCb.mimoAllocInfo.numTxLyrs   = 1;
      ue->dl.dlAllocCb.tbInfo[1].schdlngForTb    = FALSE;
      ue->dl.dlAllocCb.tbInfo[1].isDisabled      = TRUE;
   }

   if (effAlloc == (remTb1Bytes + remTb2Bytes))
   {
      RETVALUE(RFAILED);
   }

   if((rrDlUe->sigLst.count == 0) &&
      (rrDlUe->dataLst.count == 0))
   {
      rgSCHRrDlDelUeTxLst(ue,rrDlCell,rrDlUe);
   }
   RETVALUE(ROK);
}


/**
 * @brief Dl allocation finalization for re-transmission UEs.
 *
 * @details
 *
 *     Function : rgSCHRrDlRetxUesAllocFnlz
 *
 *     Processing Steps:
 *     - If allocation succeded then remove from re-transmission list
 *       For SM UEs consider harq state of both TBs in case of MIMO.
 *
 *  @param[in]  RgSchCellCb            *cell
 *  @param[in]  RgSchCmnDlRbAllocInfo  *allocInfo
 *  @return  Void
 **/
#ifdef ANSI
PRIVATE Void rgSCHRrDlRetxUesAllocFnlz
(
RgSchCellCb             *cell,
RgSchCmnDlRbAllocInfo   *allocInfo
)
#else
PRIVATE Void rgSCHRrDlRetxUesAllocFnlz(cell, allocInfo)
RgSchCellCb             *cell;
RgSchCmnDlRbAllocInfo   *allocInfo;
#endif
{
   RgSchUeCb       *ue;
   RgSchDlHqProcCb *hqP;
   RgSchDlRbAlloc  *dlRbAlloc;
   CmLList         *node;
   TRC2(rgSCHRrDlRetxUesAllocFnlz);

   node = allocInfo->dedAlloc.schdRetxHqPLst.first;
   while(node)
   {
      hqP = (RgSchDlHqProcCb *)(node->node);
      ue  = hqP->hqE->ue;
      dlRbAlloc = &(ue->dl.dlAllocCb);
      node = node->next;
      rgSCHCmnFillHqPPdcch(cell, dlRbAlloc, hqP);
      /* Extra Check: Retain the hqProc in the RETX Queue if one/more
       * TBs of the HqProc are yet to be scheduled for RETX */
      if ((hqP->tbInfo[0].state != HQ_TB_NACKED) &&
            (hqP->tbInfo[1].state != HQ_TB_NACKED))
      {
         rgSCHRrDlProcRmvFrmRetx(cell, hqP);
      }
#ifdef LTEMAC_HDFDD
      if (ue->hdFddEnbld)
      {
         rgSCHCmnHdFddUpdDLMark(cell, ue);
      }
#endif
      rgSCHCmnDlUeResetTemp(ue, hqP);
   }

   node = allocInfo->dedAlloc.nonSchdRetxHqPLst.first;
   while(node)
   {
      hqP = (RgSchDlHqProcCb *)(node->node);
      ue  = hqP->hqE->ue;
      node = node->next;
      /* Fix: syed To update Retx Alloc failure counter.
       * Need to do the same in other schedulers as well. */
      rgSCHDhmDlRetxAllocFail(ue, hqP);
      rgSCHCmnDlUeResetTemp(ue, hqP);
   }
   RETVOID;
}

/**
 * @brief Dl allocation finalizations for the new-transmission UEs.
 *
 * @details
 *
 *     Function : rgSCHRrDlUesAllocFnlz
 *
 *     Processing Steps:
 *     - If allocation succeded then link the harq in dl subframe.
 *     - else release the harq.
 *
 *  @param[in]  RgSchCellCb            *cell
 *  @param[in]  RgSchCmnDlRbAllocInfo  *allocInfo
 *  @return  Void
 **/
#ifdef ANSI
PRIVATE Void rgSCHRrDlUesAllocFnlz
(
RgSchCellCb             *cell,
RgSchCmnDlRbAllocInfo   *allocInfo
)
#else
PRIVATE Void rgSCHRrDlUesAllocFnlz(cell, allocInfo)
RgSchCellCb             *cell;
RgSchCmnDlRbAllocInfo   *allocInfo;
#endif
{
   RgSchUeCb    *ue;
   RgSchCmnUe   *schCmnUe;
   RgSchRrDlUe  *rrDlUe;
   RgSchDlHqProcCb *hqP;
   CmLList      *node;
   TRC2(rgSCHRrDlUesAllocFnlz);

   node = allocInfo->dedAlloc.schdTxHqPLst.first;
   while(node)
   {
      hqP = (RgSchDlHqProcCb *)(node->node);
      ue  = hqP->hqE->ue;

      node = node->next;
      if(rgSCHRrDlUeAllocFnlz(cell, ue) == ROK)
      {
         rgSCHCmnFillHqPPdcch(cell, &(ue->dl.dlAllocCb), hqP);
      }
      else
      {
         rgSCHUtlPdcchPut(cell, &(allocInfo->dedAlloc.dedDlSf->pdcchInfo),
                     ue->dl.dlAllocCb.pdcch);
         rgSCHRrRlsDlHqProc(hqP);
      }
#ifdef LTEMAC_HDFDD
      if (ue->hdFddEnbld)
      {
         rgSCHCmnHdFddUpdDLMark(cell, ue);
      }
#endif
      rgSCHCmnDlUeResetTemp(ue, hqP);
   }

   node = allocInfo->dedAlloc.nonSchdTxHqPLst.first;
   while(node)
   {
      hqP = (RgSchDlHqProcCb *)(node->node);
      ue  = hqP->hqE->ue;
      schCmnUe = (RgSchCmnUe *)(ue->sch);
      rrDlUe = (RgSchRrDlUe *)(schCmnUe->dl.schSpfc);
      node = node->next;

      rgSCHRrRlsDlHqProc(hqP);
      /* Fix :syed  Need to reset this field even in failure case.*/
      rrDlUe->schdTa = FALSE;
      rgSCHCmnDlUeResetTemp(ue, hqP);
   }
   RETVOID;
}
 
/* 3.1 Added new function to handle TX+RETX alloc fnlz'n */

/**
 * @brief This function Processes the Final Allocations
 *        made by the RB Allocator against the requested
 *        RETX+New TX allocations. The NewTx TB allocation
 *        is considered for distribution among LCs.
 *
 * @details
 *
 *     Function: rgSCHRrDlRetxNewTxAllocFnlz
 *     Purpose : 1. Reached here due to 1 RETX TB allocation for a
 *                  SM UE, which is capable to accomodate a newTX
 *                  in the other TB.
 *               2. Distribute NewTX TB allocation among the
 *                  SVCs present in lcsWithData list of UE.
 *
 *
 *     Invoked by: Common Scheduler
 *
 *  @param[in]  RgSchCellCb           *cell
 *  @param[in]  RgSchCmnDlRbAllocInfo *allocInfo
 *  @return  Void
 *
 **/
#ifdef ANSI
PRIVATE Void rgSCHRrDlRetxNewTxAllocFnlz
(
RgSchCellCb           *cell,
RgSchCmnDlRbAllocInfo *allocInfo
)
#else
PRIVATE Void rgSCHRrDlRetxNewTxAllocFnlz(cell, allocInfo)
RgSchCellCb           *cell;
RgSchCmnDlRbAllocInfo *allocInfo;
#endif
{
   RgSchUeCb    *ue;
   RgSchDlHqProcCb *hqP;
   RgSchDlRbAlloc *dlRbAlloc;
   RgSchCmnUe   *schCmnUe;
   RgSchRrDlUe  *rrDlUe;
   CmLList      *node;
   RgSchDlHqTbCb    *newTxTbInfo;
   U32              effAlloc;
   U32              remTbBytes;
   RgSchDlRbAlloc   *ueAllocInfo;
   RgSchCmnCell     *schCmnCell = (RgSchCmnCell *)(cell->sc.sch);
   RgSchRrDlCell    *rrDlCell = (RgSchRrDlCell *)(schCmnCell->dl.schSpfc);

   TRC2(rgSCHRrDlRetxNewTxAllocFnlz);

   node = allocInfo->dedAlloc.schdTxRetxHqPLst.first;
   while(node)
   {
      hqP = (RgSchDlHqProcCb *)(node->node);
      ue  = hqP->hqE->ue;
      dlRbAlloc = &(ue->dl.dlAllocCb);
      schCmnUe = (RgSchCmnUe *)(ue->sch);
      rrDlUe = (RgSchRrDlUe *)(schCmnUe->dl.schSpfc);
      node = node->next;
      ueAllocInfo = RG_SCH_CMN_GET_ALLOCCB_FRM_UE(ue);
      /* Index 0 of ueAllocInfo->tbInfo will always hold the
       * RETX TB and index 1 will hold the NewTX TB in case of
       * RETX+TX allocation. */
      newTxTbInfo = ueAllocInfo->tbInfo[1].tbCb;
      effAlloc = remTbBytes  =  ueAllocInfo->tbInfo[1].bytesAlloc;
      /* Fix: syed List Corruption issue */
      rgSCHRrDlSprTxTbDstn(cell, ue, newTxTbInfo,\
            &remTbBytes, &rrDlUe->sigLst);
      rgSCHRrDlSprTxTbDstn(cell, ue, newTxTbInfo,\
            &remTbBytes, &rrDlUe->dataLst);

      /* Fix : syed if UE has no new data to be scheduled as a result 
       * of rgSCHRrDlSprTxTbDstn, then remove UE from scheduling list */
      if((rrDlUe->sigLst.count == 0) &&
         (rrDlUe->dataLst.count == 0))
      {
         rgSCHRrDlDelUeTxLst(ue,rrDlCell,rrDlUe);
      }
      /* Trying to reduce mcs of TX TB to increase reception quality at UE.
       * In case of RETX+TX allocation, TX TB allocation was irrespective
       * of actual requirement by UE, hence in case if consumption is
       * less than allocation, consider reducing the iMcs of this TX TB. */
      rgSCHCmnRdcImcsTxTb(&ue->dl.dlAllocCb, 1, effAlloc - remTbBytes);
      rgSCHRrDlProcRmvFrmRetx(cell, hqP);

      rgSCHCmnFillHqPPdcch(cell, dlRbAlloc, hqP);
#ifdef LTEMAC_HDFDD
      if (ue->hdFddEnbld)
      {
         rgSCHCmnHdFddUpdDLMark(cell, ue);
      }
#endif

      rgSCHCmnDlUeResetTemp(ue, hqP);
   }

   node = allocInfo->dedAlloc.nonSchdTxRetxHqPLst.first;
   while(node)
   {
      hqP = (RgSchDlHqProcCb *)(node->node);
      ue  = hqP->hqE->ue;
      node = node->next;
      /* Fix: syed To update Retx Alloc failure counter.
       * Need to do the same in other schedulers as well. */
      rgSCHDhmDlRetxAllocFail(ue, hqP);
      rgSCHCmnDlUeResetTemp(ue, hqP);
   }
   RETVOID;

}

/**
 * @brief Dl allocations finalizations for the UEs.
 *
 * @details
 *
 *     Function : rgSCHRrDlAllocFnlz
 *
 *     Processing Steps:
 *     - Finalize dl allocations for re-transmission UEs by invoking
 *     rgSCHRrDlRetxUesAllocFnlz
 *     - Finalize dl allocations for new transmission UEs by invoking
 *     rgSCHRrDlUesAllocFnlz
 *
 *  @param[in]  RgSchCellCb            *cell
 *  @param[in]  RgSchCmnDlRbAllocInfo  *allocInfo
 *  @return  Void
 **/
#ifdef ANSI
PUBLIC Void rgSCHRrDlAllocFnlz
(
RgSchCellCb             *cell,
RgSchCmnDlRbAllocInfo   *allocInfo
)
#else
PUBLIC Void rgSCHRrDlAllocFnlz(cell, allocInfo)
RgSchCellCb             *cell;
RgSchCmnDlRbAllocInfo   *allocInfo;
#endif
{
   TRC2(rgSCHRrDlAllocFnlz);

   rgSCHRrDlRetxUesAllocFnlz(cell, allocInfo);
   rgSCHRrDlUesAllocFnlz(cell, allocInfo);
   /*3.1 MIMO new Function added to handle TX+RETX
    * harq process scheduling finalization */
   rgSCHRrDlRetxNewTxAllocFnlz(cell, allocInfo);

   RETVOID;
}

/**
 * @brief Indication of list of inactive UEs
 *
 * @details
 *
 *     Function: rgSCHRrDlInactvtUes
 *     Purpose:  RR does nothing
 *
 *  @param[in]  RgSchCellCb        *cell
 *  @param[in]  CmLListCp          *lst
 *  @return  Void
 *
 **/
#ifdef ANSI
PUBLIC Void rgSCHRrDlInactvtUes
(
RgSchCellCb        *cell,
CmLListCp          *lst
)
#else
PUBLIC Void rgSCHRrDlInactvtUes(cell, lst)
RgSchCellCb        *cell;
CmLListCp          *lst;
#endif
{
   TRC2(rgSCHRrDlInactvtUes);
   RETVOID;
}


/**
 * @brief This function initializes all the data for the scheduler
 *
 * @details
 *
 *     Function: rgSCHRrDlInit
 *     Purpose:  This function initializes the DL RR SCH
 *
 *  @param[in]  RgDlSchdApis *rgSchDlApis
 *  @return  Void
 *
 **/
#ifdef ANSI
PUBLIC Void rgSCHRrDlInit
(
RgDlSchdApis *rgSchDlApis
)
#else
PUBLIC Void rgSCHRrDlInit(rgSchDlApis)
RgDlSchdApis *rgSchDlApis;
#endif
{
   TRC2(rgSCHRrDlInit);

   /* Init the function pointers */
   rgSchDlApis->rgSCHRgrDlUeCfg       = rgSCHRrDlUeCfg;
   rgSchDlApis->rgSCHRgrDlUeRecfg     = rgSCHRrDlUeRecfg;
   rgSchDlApis->rgSCHFreeDlUe         = rgSCHRrDlUeDel;
   rgSchDlApis->rgSCHRgrDlCellCfg     = rgSCHRrDlCellCfg;
   rgSchDlApis->rgSCHRgrDlCellRecfg   = rgSCHRrDlCellRecfg;
   rgSchDlApis->rgSCHFreeDlCell       = rgSCHRrDlCellDel;
   rgSchDlApis->rgSCHRgrDlLcCfg       = rgSCHRrDlLcCfg;
   rgSchDlApis->rgSCHRgrDlLcRecfg     = rgSCHRrDlLcRecfg;
   rgSchDlApis->rgSCHFreeDlLc         = rgSCHRrDlLcDel;
   rgSchDlApis->rgSCHDlSched          = rgSCHRrDlSchd;
   rgSchDlApis->rgSCHDlDedBoUpd       = rgSCHRrDlDedBoUpd;
   rgSchDlApis->rgSCHDlProcAddToRetx  = rgSCHRrDlProcAddToRetx;
   rgSchDlApis->rgSCHDlAllocFnlz      = rgSCHRrDlAllocFnlz;
   rgSchDlApis->rgSCHDlCqiInd         = rgSCHRrDlCqiInd;
   rgSchDlApis->rgSCHDlUeRefresh      = rgSCHRrDlUeRefresh;
   rgSchDlApis->rgSCHDlActvtUe        = rgSCHRrDlActvtUe;
   rgSchDlApis->rgSCHDlUeReset        = rgSCHRrDlUeReset;
   rgSchDlApis->rgSCHDlInactvtUes     = rgSCHRrDlInactvtUes;
   rgSchDlApis->rgSCHDlUeHqEntInit    = rgSCHRrDlUeHqEntInit;
   rgSchDlApis->rgSCHDlUeHqEntDeInit  = rgSCHRrDlUeHqEntDeInit;
   rgSchDlApis->rgSCHDlProcRmvFrmRetx = rgSCHRrDlRmvProcFrmRetxLst;
   rgSchDlApis->rgSCHDlTickForPdbTrkng = rgSCHRrDlScanUpdPdbPrio;
   rgSchDlApis->rgSCHDlFillFlwCtrlInfo = rgSCHRrDlFillFlowCntrlInfo; 

   RETVOID;
}

/**
 * @brief Indication of UL CQI for the UE
 *
 * @details
 *
 *     Function : rgSCHRrUlCqiInd
 *     Purpose:  RR does nothing
 *
 *  @param[in]  RgSchCellCb         *cell
 *  @param[in]  RgSchUeCb           *ue
 *  @param[in]  TfuUlCqiRpt         *ulCqiInfo
 *  @return  Void
 **/
#ifdef ANSI
PUBLIC Void rgSCHRrUlCqiInd
(
RgSchCellCb          *cell,
RgSchUeCb            *ue,
TfuUlCqiRpt          *ulCqiInfo
)
#else
PUBLIC Void rgSCHRrUlCqiInd(cell, ue, ulCqiInfo)
RgSchCellCb          *cell;
RgSchUeCb            *ue;
TfuUlCqiRpt          *ulCqiInfo;
#endif
{
   TRC2(rgSCHRrUlCqiInd);

   RETVOID;
}

/**
 * @brief Indication of activation of an UE after meas gap or ack/nack rep.
 *
 * @details
 *
 *     Function : rgSCHRrUlActvtUe
 *     Purpose:  RR does nothing
 *
 *  @param[in]  RgSchCellCb         *cell
 *  @param[in]  RgSchUeCb           *ue
 *  @return  Void
 **/
#ifdef ANSI
PUBLIC Void rgSCHRrUlActvtUe
(
RgSchCellCb          *cell,
RgSchUeCb            *ue
)
#else
PUBLIC Void rgSCHRrUlActvtUe(cell, ue)
RgSchCellCb          *cell;
RgSchUeCb            *ue;
#endif
{
   TRC2(rgSCHRrUlActvtUe);

   RETVOID;
}

/**
 * @brief Indication of Ul Refresh for the UE.
 *
 * @details
 *
 *     Function: rgSCHRrUlUeRefresh
 *     Purpose:  RR does nothing.
 *
 *  @param[in]  RgSchCellCb        *cell
 *  @param[in]  RgSchUeCb          *ue
 *  @return  Void
 *
 **/
#ifdef ANSI
PUBLIC Void rgSCHRrUlUeRefresh
(
RgSchCellCb    *cell,
RgSchUeCb      *ue
)
#else
PUBLIC Void rgSCHRrUlUeRefresh(cell, ue)
RgSchCellCb    *cell;
RgSchUeCb      *ue;
#endif
{
   TRC2(rgSCHRrUlUeRefresh);

   RETVOID;
}
/**
 * @brief UL Lcg received data updation
 *
 * @details
 *
 *     Function : rgSCHRrUlLcgUpd
 *
 *     Processing Steps:RR Does nothing
 *
 *  @param[in]  RgSchCellCb         *cell
 *  @param[in]  RgSchUeCb          *ue
 *  @param[in]  RgInfUeDatInd      *datInd
 *  @return  S16
 **/
#ifdef ANSI
PUBLIC S16 rgSCHRrUlLcgUpd
(
RgSchCellCb         *cell,
RgSchUeCb           *ue,
RgInfUeDatInd       *datInd
)
#else
PUBLIC S16 rgSCHRrUlLcgUpd(cell, ue)
RgSchCellCb         *cell;
RgSchUeCb           *ue;
RgInfUeDatInd       *datInd;
#endif
{

   TRC2(rgSCHRrUlLcgUpd);

   RETVALUE(ROK);  
}
/**
 * @brief Cell configuration for Round Robin
 *
 * @details
 *
 *     Function : rgSCHRrUlCellCfg
 *
 *     Processing Steps:
 *      - Create Round Robin related information per cell
 *      - Initialize ContRes and Tx lists
 *
 *  @param[in]  RgSchCellCb         *cell
 *  @param[in]  RgrCellCfg          *cfg
 *  @param[in]  RgSchErrInfo        *err
 *  @return  S16
 *      -# ROK
 *      -# RFAILED
 **/
#ifdef ANSI
PUBLIC S16 rgSCHRrUlCellCfg
(
RgSchCellCb          *cell,
RgrCellCfg           *cfg,
RgSchErrInfo         *err
)
#else
PUBLIC S16 rgSCHRrUlCellCfg(cell, cfg, err)
RgSchCellCb          *cell;
RgrCellCfg           *cfg;
RgSchErrInfo         *err;
#endif
{
   RgSchCmnCell *schCmnCell = (RgSchCmnCell *)(cell->sc.sch);
   RgSchRrUlCell *rrUlCell;
   TRC2(rgSCHRrUlCellCfg);

   if(rgSCHUtlAllocSBuf(cell->instIdx,
      (Data**)&(schCmnCell->ul.schSpfc), (sizeof(RgSchRrUlCell))) != ROK)
   {
      RLOG_ARG0(L_ERROR,DBG_CELLID,cell->cellId,
         "Memory allocation FAILED");
      RETVALUE(RFAILED);
   }

   rrUlCell = (RgSchRrUlCell *)(schCmnCell->ul.schSpfc);

   rgSCHRrCListInit(&(rrUlCell->contResLst));
   rgSCHRrCListInit(&(rrUlCell->prioUeTxLst));
   rgSCHRrCListInit(&(rrUlCell->normUeTxLst));

   RETVALUE(ROK);
}

/**
 * @brief Cell reconfiguration for Round Robin
 *
 * @details
 *
 *     Function : rgSCHRrUlCellRecfg
 *
 *     Processing Steps:
 *      - RR does nothing.
 *
 *  @param[in]  RgSchCellCb       *cell
 *  @param[in]  RgrCellRecfg      *recfg
 *  @param[in]  RgSchErrInfo      *err
 *  @return  S16
 *      -# ROK
 *      -# RFAILED
 **/
#ifdef ANSI
PUBLIC S16 rgSCHRrUlCellRecfg
(
RgSchCellCb     *cell,
RgrCellRecfg    *recfg,
RgSchErrInfo    *err
)
#else
PUBLIC S16 rgSCHRrUlCellRecfg(cell, recfg, err)
RgSchCellCb     *cell;
RgrCellRecfg    *recfg;
RgSchErrInfo    *err;
#endif
{
   TRC2(rgSCHRrUlCellRecfg);
   RETVALUE(ROK);
}


/**
 * @brief Cell deletion for Round Robin
 *
 * @details
 *
 *     Function : rgSCHRrUlCellDel
 *
 *     Processing Steps:
 *      - Free Round Robin related information per cell.
 *
 *  @param[in]  RgSchCellCb       *cell
 *  @return  Void
 **/
#ifdef ANSI
PUBLIC Void rgSCHRrUlCellDel
(
RgSchCellCb       *cell
)
#else
PUBLIC Void rgSCHRrUlCellDel(cell)
RgSchCellCb       *cell;
#endif
{
   RgSchCmnCell *schCmnCell = (RgSchCmnCell *)(cell->sc.sch);
   TRC2(rgSCHRrUlCellDel);

   if(schCmnCell->ul.schSpfc)
   {
      /* ccpu00117052 - MOD - Passing double pointer
      for proper NULLP assignment*/
      rgSCHUtlFreeSBuf(cell->instIdx,
      (Data**)(&(schCmnCell->ul.schSpfc)), (sizeof(RgSchRrUlCell)));
   }
   RETVOID;
}

/**
 * @brief UE configuration for Round Robin
 *
 * @details
 *
 *     Function : rgSCHRrUlUeCfg
 *
 *     Processing Steps:
 *      - Create Round Robin related information
 *
 *  @param[in]  RgrSchCellCb     *cell
 *  @param[in]  RgSchUeCb        *ue
 *  @param[in]  RgrUeCfg         *cfg
 *  @param[in]  RgSchErrInfo     *err
 *  @return  S16
 *      -# ROK
 *      -# RFAILED
 **/
#ifdef ANSI
PUBLIC S16 rgSCHRrUlUeCfg
(
RgSchCellCb      *cell,
RgSchUeCb        *ue,
RgrUeCfg         *cfg,
RgSchErrInfo     *err
)
#else
PUBLIC S16 rgSCHRrUlUeCfg(cell, ue, cfg, err)
RgSchCellCb      *cell;
RgSchUeCb        *ue;
RgrUeCfg         *cfg;
RgSchErrInfo     *err;
#endif
{
   RgSchCmnUe *schCmnUe = (RgSchCmnUe *)(ue->sch);
   RgSchRrUlUe  *rrUlUe;
   TRC2(rgSCHRrUlUeCfg);

   if(rgSCHUtlAllocSBuf(cell->instIdx,
      (Data**)&(schCmnUe->ul.schSpfc), (sizeof(RgSchRrUlUe))) != ROK)
   {
      RLOG_ARG0(L_ERROR,DBG_CELLID,cell->cellId,
         "Memory allocation FAILED");
      RETVALUE(RFAILED);
   }
   rrUlUe = (RgSchRrUlUe *)(schCmnUe->ul.schSpfc);
 
   RETVALUE(ROK);
}

/**
 * @brief UE reconfiguration for Round Robin
 *
 * @details
 *
 *     Function : rgSCHRrUlUeRecfg
 *
 *     Processing Steps:
 *      - RR does nothing.
 *
 *  @param[in]   RgSchCellCb      *cell
 *  @param[in]   RgSchUeCb        *ue
 *  @param[in]   RgrUeRecfg       *recfg
 *  @param[in]   RgSchErrInfo     *err
 *  @return  S16
 *      -# ROK
 *      -# RFAILED
 **/
#ifdef ANSI
PUBLIC S16 rgSCHRrUlUeRecfg
(
RgSchCellCb      *cell,
RgSchUeCb        *ue,
RgrUeRecfg       *recfg,
RgSchErrInfo     *err
)
#else
PUBLIC S16 rgSCHRrUlUeRecfg(cell, ue, recfg, err)
RgSchCellCb      *cell;
RgSchUeCb        *ue;
RgrUeRecfg       *recfg;
RgSchErrInfo     *err;
#endif
{
   RgSchCmnCell *schCmnCell = (RgSchCmnCell *)(cell->sc.sch);
   RgSchRrUlCell *rrUlCell = (RgSchRrUlCell *)(schCmnCell->ul.schSpfc);
   RgSchCmnUe  *schCmnUe = (RgSchCmnUe *)(ue->sch);
   RgSchRrUlUe *rrUlUe = (RgSchRrUlUe *)(schCmnUe->ul.schSpfc);

   TRC2(rgSCHRrUlUeRecfg);

   if((cell->csgAccsMode == RGR_CELL_ACCS_HYBRID) &&
      (recfg->ueRecfgTypes & RGR_UE_CSG_PARAM_RECFG))
   {
      if (rrUlUe->ueTxLnk.node != NULLP)
      {
         if (recfg->csgMmbrSta == FALSE)
         {
            rgSCHRrCListDelFrm(&(rrUlCell->prioUeTxLst), &(rrUlUe->ueTxLnk));
            rgSCHRrCListAdd2Tail(&(rrUlCell->normUeTxLst), &(rrUlUe->ueTxLnk));
         }
         else
         {
            rgSCHRrCListDelFrm(&(rrUlCell->normUeTxLst), &(rrUlUe->ueTxLnk));
            rgSCHRrCListAdd2Tail(&(rrUlCell->prioUeTxLst), &(rrUlUe->ueTxLnk));
         }
         rrUlUe->ueTxLnk.node = (PTR)ue;
      }
   }

   RETVALUE(ROK);
}

/**
 * @brief UE delete for Round Robin
 *
 * @details
 *
 *     Function : rgSCHRrUlUeDel
 *
 *     Processing Steps:
 *     - Remove UE from contres list if present
 *     - Remove UE from tx list if present
 *     - Free Round Robin UE info
 *
 *  @param[in]  RgSchCellCb       *cell
 *  @param[in]  RgSchUeCb         *ue
 *  @return  Void
 **/
#ifdef ANSI
PUBLIC Void rgSCHRrUlUeDel
(
RgSchCellCb      *cell,
RgSchUeCb        *ue
)
#else
PUBLIC Void rgSCHRrUlUeDel(cell, ue)
RgSchCellCb      *cell;
RgSchUeCb        *ue;
#endif
{
   RgSchCmnCell *schCmnCell = (RgSchCmnCell *)(cell->sc.sch);
   RgSchRrUlCell *rrUlCell = (RgSchRrUlCell *)(schCmnCell->ul.schSpfc);
   RgSchCmnUe *schCmnUe = (RgSchCmnUe *)(ue->sch);
   RgSchRrUlUe  *rrUlUe = (RgSchRrUlUe *)(schCmnUe->ul.schSpfc);
   TRC2(rgSCHRrUlUeDel);

   if(schCmnUe->ul.schSpfc == NULLP)
   {
      RETVOID;
   }

   if(rrUlUe->ueTxLnk.node)
   {
      rgSCHRrUlDelUeTxLst(ue,rrUlCell,rrUlUe);
   }
   if(rrUlUe->contResLnk.node)
   {
      rgSCHRrCListDelFrm(&(rrUlCell->contResLst), &(rrUlUe->contResLnk));
   }
   /* ccpu00117052 - MOD - Passing double pointer
   for proper NULLP assignment*/
   rgSCHUtlFreeSBuf(cell->instIdx,
   (Data**)(&(schCmnUe->ul.schSpfc)), (sizeof(RgSchRrUlUe)));
   RETVOID;
}

/**
 * @brief Ul LCG configuration for Round Robin
 *
 * @details
 *
 *     Function : rgSCHRrUlLcgCfg
 *
 *     Processing Steps:
 *      - RR does nothing.
 *
 *  @param[in]  RgrSchCellCb     *cell
 *  @param[in]  RgSchUeCb        *ue
 *  @param[in]  RgSchUlLcgCb     *ulLcg
 *  @param[in]  RgrLcgCfg        *cfg
 *  @param[in]  RgSchErrInfo     *err
 *  @return  S16
 *      -# ROK
 *      -# RFAILED
 **/
#ifdef ANSI
PUBLIC S16 rgSCHRrUlLcgCfg
(
RgSchCellCb      *cell,
RgSchUeCb        *ue,
RgSchLcgCb       *ulLcg,
RgrLcgCfg        *cfg,
RgSchErrInfo     *err
)
#else
PUBLIC S16 rgSCHRrUlLcgCfg(cell, ue, ulLcg, cfg, err)
RgSchCellCb      *cell;
RgSchUeCb        *ue;
RgSchLcgCb       *ulLcg;
RgrLcgCfg        *cfg;
RgSchErrInfo     *err;
#endif
{
   TRC2(rgSCHRrUlLcgCfg);

   RETVALUE(ROK);
}

/**
 * @brief Ul LC configuration for Round Robin
 *
 * @details
 *
 *     Function : rgSCHRrUlLcgRecfg
 *
 *     Processing Steps:
 *      - RR does nothing.
 *
 *  @param[in]  RgrSchCellCb     *cell
 *  @param[in]  RgSchUeCb        *ue
 *  @param[in]  RgSchUlLcgCb      *ulLcg
 *  @param[in]  RgrLcgRecfg      *cfg
 *  @param[in]  RgSchErrInfo     *err
 *  @return  S16
 *      -# ROK
 *      -# RFAILED
 **/
#ifdef ANSI
PUBLIC S16 rgSCHRrUlLcgRecfg
(
RgSchCellCb      *cell,
RgSchUeCb        *ue,
RgSchLcgCb       *ulLcg,
RgrLcgRecfg      *recfg,
RgSchErrInfo     *err
)
#else
PUBLIC S16 rgSCHRrUlLcgRecfg(cell, ue, ulLcg, recfg, err)
RgSchCellCb      *cell;
RgSchUeCb        *ue;
RgSchLcgCb       *ulLcg;
RgrLcgRecfg      *recfg;
RgSchErrInfo     *err;
#endif
{
   TRC2(rgSCHRrUlLcgRecfg);

   RETVALUE(ROK);
}

/**
 * @brief Check and Update UE scheduling status
 *
 * @details
 *
 *     Function : rgSCHRrUlUeChkAndUpdSchd
 *
 *     Processing Steps:
 *     - If the effective BSR > 0 and the UE is not already part of
 *       transmission queue then append the UE to the queue.
 *     - If the effective BSR is ZERO then remove the UE from the queue.
 *
 *  @param[in]  RgSchCellCb        *cell
 *  @param[in]  RgSchUeCb          *ue
 *  @return  Void
 **/
#ifdef ANSI
PRIVATE Void rgSCHRrUlUeChkAndUpdSchd
(
RgSchCellCb         *cell,
RgSchUeCb           *ue
)
#else
PRIVATE Void rgSCHRrUlUeChkAndUpdSchd(cell, ue)
RgSchCellCb         *cell;
RgSchUeCb           *ue;
#endif
{
   RgSchCmnCell *schCmnCell = (RgSchCmnCell *)(cell->sc.sch);
   RgSchRrUlCell *rrUlCell = (RgSchRrUlCell *)(schCmnCell->ul.schSpfc);
   RgSchCmnUe *schCmnUe = (RgSchCmnUe *)(ue->sch);
   RgSchCmnUeUlAlloc *schCmnUeUlAlloc = &(schCmnUe->ul.alloc);
   RgSchRrUlUe  *rrUlUe = (RgSchRrUlUe *)(schCmnUe->ul.schSpfc);
   TRC2(rgSCHRrUlUeChkAndUpdSchd);

   /* ccpu00130982: If CRNTI MAC CE and Short BSR are came together then
    * not adding UE to UlTxLst and the UE will be processed from ContResLst*/
   if((schCmnUe->ul.effBsr) && (rrUlUe->contResLnk.node == NULLP))
   {
      if(rrUlUe->ueTxLnk.node == NULLP)
      {
         rgSCHRrUlAddUeTxLst(ue,rrUlCell,rrUlUe);
         schCmnUeUlAlloc->reqLnk.node = NULLP;
      }
      else if(
               (RGSCH_LCG_ISCFGD(&ue->ul.lcgArr[0]) == FALSE) &&
               (RGSCH_LCG_ISCFGD(&ue->ul.lcgArr[1]) == FALSE) &&
               (RGSCH_LCG_ISCFGD(&ue->ul.lcgArr[2]) == FALSE) &&
               (RGSCH_LCG_ISCFGD(&ue->ul.lcgArr[3]) == FALSE)
             )
      {
         rgSCHRrUlDelUeTxLst(ue,rrUlCell,rrUlUe);
      }
   }
   else
   {
      if (rrUlUe->srRcvd == FALSE)
      {
         rgSCHRrUlDelUeTxLst(ue,rrUlCell,rrUlUe);
      }
#ifdef RGR_V1
      if(schCmnUe->ul.totalBsr != 0)
      {
         if (ue->bsrTmr.tmrEvnt != TMR_NONE)
         {
            rgSCHTmrStopTmr(cell, ue->bsrTmr.tmrEvnt, ue);
         }
         if (ue->ul.bsrTmrCfg.isPrdBsrTmrPres)
         {
            rgSCHTmrStartTmr(cell, ue, RG_SCH_TMR_BSR,
                  ue->ul.bsrTmrCfg.prdBsrTmr);
         }
      }
#endif
   }

   RETVOID;
}


/**
 * @brief Ul LCG deletion for Round Robin
 *
 * @details
 *
 *     Function : rgSCHRrUlLcgDel
 *     - RR does nothing.
 *
 *     Processing Steps:
 *
 *  @param[in]  RgrSchCellCb     *cell
 *  @param[in]  RgSchUeCb        *ue
 *  @param[in]  RgSchUlLcgCb     *ulLcg
 *  @return  Void
 **/
#ifdef ANSI
PUBLIC Void rgSCHRrUlLcgDel
(
RgSchCellCb      *cell,
RgSchUeCb        *ue,
RgSchLcgCb       *ulLcg
)
#else
PUBLIC Void rgSCHRrUlLcgDel(cell, ue, ulLcg)
RgSchCellCb      *cell;
RgSchUeCb        *ue;
RgSchLcgCb       *ulLcg;
#endif
{
   TRC2(rgSCHRrUlLcgDel);

   rgSCHRrUlUeChkAndUpdSchd(cell, ue);
   RETVOID;
}

/**
 * @brief Short BSR reception handling
 *
 * @details
 *
 *     Function : rgSCHRrUpdShortBsr
 *
 *     Processing Steps:
 *     - Check and update the UE scheduling status
 *
 *  @param[in]  RgSchCellCb        *cell
 *  @param[in]  RgSchUeCb          *ue
 *  @param[in]  RgSchLcgCb         *ulLcg
 *  @param[in]  U8                 bs
 *  @return  Void
 **/
#ifdef ANSI
PUBLIC Void rgSCHRrUpdShortBsr
(
RgSchCellCb         *cell,
RgSchUeCb           *ue,
RgSchLcgCb          *ulLcg,
U8                  bs
)
#else
PUBLIC Void rgSCHRrUpdShortBsr(cell, ue, ulLcg, bs)
RgSchCellCb         *cell;
RgSchUeCb           *ue;
RgSchLcgCb          *ulLcg;
U8                  bs;
#endif
{
   TRC2(rgSCHRrUpdShortBsr);

   rgSCHRrUlUeChkAndUpdSchd(cell, ue);
   RETVOID;
}

/**
 * @brief Truncated BSR reception handling
 *
 * @details
 *
 *     Function : rgSCHRrUpdTruncBsr
 *
 *     Processing Steps:
 *     - Check and update the UE scheduling status
 *
 *  @param[in]  RgSchCellCb        *cell
 *  @param[in]  RgSchUeCb          *ue
 *  @param[in]  RgSchLcgCb         *ulLcg
 *  @param[in]  U8                 bs
 *  @return  Void
 **/
#ifdef ANSI
PUBLIC Void rgSCHRrUpdTruncBsr
(
RgSchCellCb         *cell,
RgSchUeCb           *ue,
RgSchLcgCb          *ulLcg,
U8                  bs
)
#else
PUBLIC Void rgSCHRrUpdTruncBsr(cell, ue, ulLcg, bs)
RgSchCellCb         *cell;
RgSchUeCb           *ue;
RgSchLcgCb          *ulLcg;
U8                  bs;
#endif
{
   TRC2(rgSCHRrUpdTruncBsr);

   rgSCHRrUlUeChkAndUpdSchd(cell, ue);
   RETVOID;
}

/**
 * @brief Long BSR reception handling
 *
 * @details
 *
 *     Function : rgSCHRrUpdLongBsr
 *
 *     Processing Steps:
 *     - Check and update the UE scheduling status
 *
 *  @param[in]  RgSchCellCb        *cell
 *  @param[in]  RgSchUeCb          *ue
 *  @param[in]  U8                 *bsArr
 *  @return  Void
 **/
#ifdef ANSI
PUBLIC Void rgSCHRrUpdLongBsr
(
RgSchCellCb         *cell,
RgSchUeCb           *ue,
U8                  *bsArr
)
#else
PUBLIC Void rgSCHRrUpdLongBsr(cell, ue, bsArr)
RgSchCellCb         *cell;
RgSchUeCb           *ue;
U8                  *bsArr;
#endif
{
   TRC2(rgSCHRrUpdLongBsr);

   rgSCHRrUlUeChkAndUpdSchd(cell, ue);
   RETVOID;
}

/**
 * @brief Indication of UL UE reset
 *
 * @details
 *
 *     Function : rgSCHRrUlUeReset
 *
 *     Processing Steps:
 *     - Remove the UE from contres list
 *     - Remove the UE from transmission list
 *
 *  @param[in]  RgSchCellCb       *cell
 *  @param[in]  RgSchUeCb         *ue
 *  @return  Void
 **/
#ifdef ANSI
PUBLIC Void rgSCHRrUlUeReset
(
RgSchCellCb          *cell,
RgSchUeCb            *ue
)
#else
PUBLIC Void rgSCHRrUlUeReset(cell, ue)
RgSchCellCb          *cell;
RgSchUeCb            *ue;
#endif
{
   RgSchCmnCell *schCmnCell = (RgSchCmnCell *)(cell->sc.sch);
   RgSchRrUlCell *rrUlCell = (RgSchRrUlCell *)(schCmnCell->ul.schSpfc);
   RgSchCmnUe *schCmnUe = (RgSchCmnUe *)(ue->sch);
   RgSchRrUlUe  *rrUlUe = (RgSchRrUlUe *)(schCmnUe->ul.schSpfc);
   TRC2(rgSCHRrUlUeReset);

   /*Removing from re-transmission should
     be handled in CMN SCH
   rgSCHCmnUlUeDelAllocs(cell, ue);
   */

   rrUlUe->srRcvd = FALSE;

   if(rrUlUe->contResLnk.node != NULLP)
   {
      rgSCHRrCListDelFrm(&(rrUlCell->contResLst), &(rrUlUe->contResLnk));
      rrUlUe->contResLnk.node = NULLP;
      RETVOID;
   }

   rgSCHRrUlDelUeTxLst(ue,rrUlCell,rrUlUe);

   /* Stack crash problem for TRACE5 Changes. added the return below */
  RETVOID;
}

/**
 * @brief SR reception handling
 *
 * @details
 *
 *     Function : rgSCHRrSrInd
 *
 *     Processing Steps:
 *     - If UE is not part of transmission list then
 *       append the UE to the transmission list.
 *
 *  @param[in]  RgSchCellCb        *cell
 *  @param[in]  RgSchUeCb          *ue
 *  @return  Void
 **/
#ifdef ANSI
PUBLIC Void rgSCHRrSrInd
(
RgSchCellCb         *cell,
RgSchUeCb           *ue
)
#else
PUBLIC Void rgSCHRrSrInd(cell, ue)
RgSchCellCb         *cell;
RgSchUeCb           *ue;
#endif
{
   RgSchCmnCell *schCmnCell = (RgSchCmnCell *)(cell->sc.sch);
   RgSchRrUlCell *rrUlCell = (RgSchRrUlCell *)(schCmnCell->ul.schSpfc);
   RgSchCmnUe *schCmnUe = (RgSchCmnUe *)(ue->sch);
   RgSchCmnUeUlAlloc *schCmnUeUlAlloc = &(schCmnUe->ul.alloc);
   RgSchRrUlUe  *rrUlUe = (RgSchRrUlUe *)(schCmnUe->ul.schSpfc);
   TRC2(rgSCHRrSrInd);

   rrUlUe->srRcvd = TRUE;

   if(rrUlUe->ueTxLnk.node == NULLP)
   {
      /* Fix: Remove from contResLst if UE already exists there. 
       * Without which we were scheduling twice for the same UE in a 
       * given TTI, which renders system unstable and finally to a crash
       * during UE DEL. Please check other schedulers for this. */
      if(rrUlUe->contResLnk.node != NULLP)
      {
         rgSCHRrCListDelFrm(&(rrUlCell->contResLst), &(rrUlUe->contResLnk));
         rrUlUe->contResLnk.node = NULLP;
      }
      rgSCHRrUlAddUeTxLst(ue,rrUlCell,rrUlUe);
      schCmnUeUlAlloc->reqLnk.node = NULLP;
   }
   RETVOID;
}

/**
 * @brief Contention Resolution allocation request
 *
 * @details
 *
 *     Function : rgSCHRrContResUlGrant
 *
 *     Processing Steps:
 *     - Append the UE to the contention resolution queue.
 *
 *  @param[in]  RgSchCellCb        *cell
 *  @param[in]  RgSchUeCb          *ue
 *  @return  Void
 **/
#ifdef ANSI
PUBLIC Void rgSCHRrContResUlGrant
(
RgSchCellCb         *cell,
RgSchUeCb           *ue
)
#else
PUBLIC Void rgSCHRrContResUlGrant(cell, ue)
RgSchCellCb         *cell;
RgSchUeCb           *ue;
#endif
{
   RgSchCmnCell *schCmnCell = (RgSchCmnCell *)(cell->sc.sch);
   RgSchRrUlCell *rrUlCell = (RgSchRrUlCell *)(schCmnCell->ul.schSpfc);
   RgSchCmnUe *schCmnUe = (RgSchCmnUe *)(ue->sch);
   RgSchCmnUeUlAlloc *schCmnUeUlAlloc = &(schCmnUe->ul.alloc);
   RgSchRrUlUe  *rrUlUe = (RgSchRrUlUe *)(schCmnUe->ul.schSpfc);
   TRC2(rgSCHRrContResUlGrant);

   ue->isSrGrant = FALSE;
   /* Fix: syed Adding a check before adding to list. This
    * however did not create any issue. Just a precautionary measure.
    *  Please check other schedulers for this. */
   if (rrUlUe->contResLnk.node == NULLP)
   {
      rgSCHRrCListAdd2Tail(&(rrUlCell->contResLst), &(rrUlUe->contResLnk));
      rrUlUe->contResLnk.node = (PTR)ue;
      schCmnUeUlAlloc->reqLnk.node = NULLP;

      rgSCHRrUlDelUeTxLst(ue,rrUlCell,rrUlUe);
   }
   RETVOID;
}

/**
 * @brief Scheduling of contention resolution UEs in the cell.
 *
 * @details
 *
 *     Function : rgSCHRrUlSchdContResUes
 *
 *     Processing Steps:
 *     - Traverse the contention resolution list and do resource
 *       allocation for the UEs.
 *
 *  @param[in]  RgSchCellCb            *cell
 *  @param[in]  RgSchCmnUlRbAllocInfo  *allocInfo
 *  @param[in]  U8                     *remUe
 *  @return  Void
 **/
#ifdef ANSI
PRIVATE Void rgSCHRrUlSchdContResUes
(
RgSchCellCb            *cell,
RgSchCmnUlRbAllocInfo  *allocInfo,
U8                     *remUe
)
#else
PRIVATE Void rgSCHRrUlSchdContResUes(cell, allocInfo, remUe)
RgSchCellCb            *cell;
RgSchCmnUlRbAllocInfo  *allocInfo;
U8                     *remUe;
#endif
{
   RgSchCmnCell *schCmnCell = (RgSchCmnCell *)(cell->sc.sch);
   RgSchRrUlCell *rrUlCell = (RgSchRrUlCell *)(schCmnCell->ul.schSpfc);
   RgSchUeCb  *ue;
   RgSchCmnUe *schCmnUe;
   RgSchCmnUeUlAlloc *schCmnUeUlAlloc;
   RgSchRrCList *node;
#ifdef LTEMAC_HDFDD
   Bool                 ulAllowed = FALSE;
#endif
   TRC2(rgSCHRrUlSchdContResUes);
   /* Fix:ccpu00120610 Fix to avaoid incorrect allocation */
   if(rrUlCell->contResLst.crnt == NULLP || !*remUe)
   {
      RETVOID;
   }

   node = rrUlCell->contResLst.crnt;
   do
   {
      ue = (RgSchUeCb *)(node->node);
      node = node->next;
#ifdef LTEMAC_HDFDD
      if (ue->hdFddEnbld)
      {
         rgSCHCmnHdFddChkUlAllow (cell, ue,  &ulAllowed);
         if (ulAllowed == FALSE)
         {
            continue;
         }
      }
#endif
      /* rg005.201 Use the macro to check if UE already picked for Retx */
      /* Fix : syed ccpu00129054. Added check for inactivity. */ 
      if ((ue->ul.ulInactvMask) ||
          (RG_SCH_CMN_IS_UL_UE_RETX(ue)))
      {
         continue;
      }
      schCmnUe = (RgSchCmnUe *)(ue->sch);
      schCmnUeUlAlloc = &(schCmnUe->ul.alloc);
      /* ccpu00130982: Grant will be given according to the BSR , if any,
       * along with CRNTI MAC CE*/
      if(!schCmnUe->ul.effBsr)
      {
         schCmnUeUlAlloc->reqBytes = RG_SCH_CMN_MAX_UL_CONTRES_GRNT;
      }
      else
      {
         schCmnUeUlAlloc->reqBytes =  schCmnUe->ul.effBsr;
      }
      cmLListAdd2Tail(&(allocInfo->contResLst),
                        &(schCmnUeUlAlloc->reqLnk));
      schCmnUeUlAlloc->reqLnk.node = (PTR)ue;
      ue->isSrGrant = FALSE;
      (*remUe)--;
      if(*remUe == 0)
      {
         rrUlCell->contResLst.crnt = node;
         break;
      }

   }while(node != rrUlCell->contResLst.crnt);
   RETVOID;
}

/**
 * @brief Round Robin related scheduling of new-transmission for the UE
 *
 * @details
 *
 *     Function : rgSCHRrUlSchdUe
 *
 *     Processing Steps:
 *     - Schedule the UE and do resource allocation.
 *
 *  @param[in]  RgSchCellCb        *cell
 *  @return  Void
 **/
#ifdef ANSI
PRIVATE Void rgSCHRrUlSchdUe
(
RgSchCellCb       *cell,
RgSchUeCb         *ue
)
#else
PRIVATE Void rgSCHRrUlSchdUe(cell, ue)
RgSchCellCb       *cell;
RgSchUeCb         *ue;
#endif
{
   RgSchCmnUe *schCmnUe = (RgSchCmnUe *)(ue->sch);
   RgSchCmnUeUlAlloc *schCmnUeUlAlloc = &(schCmnUe->ul.alloc);
   RgSchRrUlUe  *rrUlUe = (RgSchRrUlUe *)(schCmnUe->ul.schSpfc);
   TRC2(rgSCHRrUlSchdUe);

   if(rrUlUe->srRcvd == TRUE)
   {
      if(schCmnUe->ul.effBsr == 0)
      {
         schCmnUeUlAlloc->reqBytes = RG_RR_SR_GRNT;
      }
      else
      {
         schCmnUeUlAlloc->reqBytes = schCmnUe->ul.effBsr;
      }
   }
   else
   {
      schCmnUeUlAlloc->reqBytes = schCmnUe->ul.effBsr;
   }

   rgSchUtlGetNumSbs(cell, ue, &ue->ul.subbandRequired);

   RETVOID;
}

/**
 * @brief Round Robin related scheduling of new-transmission for the UEs
 *
 * @details
 *
 *     Function : rgSCHRrUlSelUes
 *
 *     Processing Steps:
 *     - Selecting the UEs from priority queue and non priority queue
 *
 *  @param[in]  RgSchCellCb            *cell
 *  @return  Void
 **/
#ifdef ANSI
PRIVATE Void rgSCHRrUlSelUes
(
RgSchCellCb             *cell,
RgSchCmnUlRbAllocInfo   *allocInfo,
RgSchRrCListCp          *listCp,
U8                      *remUe

)
#else
PRIVATE Void rgSCHRrUlSelUes(cell, allocInfo, listCp, remUe)
RgSchCellCb             *cell;
RgSchCmnUlRbAllocInfo   *allocInfo;
RgSchRrCListCp          *listCp;
U8                      *remUe;
#endif
{
   RgSchCmnCell       *schCmnCell = (RgSchCmnCell *)(cell->sc.sch);
   RgSchUeCb          *ue;
   RgSchCmnUe         *schCmnUe;
   RgSchCmnUeUlAlloc  *schCmnUeUlAlloc;
   RgSchRrCList       *node;
   RgSchUlHqProcCb    *proc;
   U32                totSbReq = 0;
#ifdef LTEMAC_HDFDD
   Bool              ulAllowed = FALSE;
#endif
   TRC2(rgSCHRrUlSchdUes);
   /* Fix:ccpu00120610 Fix to avaoid incorrect allocation */
   if(listCp->crnt == NULLP || !(*remUe))
   {
      RETVOID;
   }
   node = listCp->crnt;
   do
   {
      ue = (RgSchUeCb *)(node->node);
      node = node->next;
#ifdef LTEMAC_HDFDD
      if (ue->hdFddEnbld)
      {
         rgSCHCmnHdFddChkUlAllow (cell, ue, &ulAllowed);
         if (ulAllowed == FALSE)
         {
            continue;
         }
      }
#endif
      schCmnUe = (RgSchCmnUe *)(ue->sch);
      schCmnUeUlAlloc = &(schCmnUe->ul.alloc);
      proc = &(ue->ul.hqEnt.hqProcCb[schCmnCell->ul.schdHqProcIdx]);
#ifdef LTEMAC_SPS
      if ((ue->ul.ulInactvMask) || RG_SCH_CMN_IS_UL_UE_RETX(ue) || RG_SCH_CMN_IS_SPS_SCHD(ue))
#else
      if((ue->ul.ulInactvMask) ||
         (RG_SCH_CMN_IS_UL_UE_RETX(ue)))
#endif
      {
         continue;
      }
      rgSCHRrUlSchdUe(cell, ue);
      /* adding the Ue to the sorted list */
      rgSchUtlSortInsUeLst(&(allocInfo->ueLst), &schCmnUeUlAlloc->reqLnk,
            ue->ul.subbandRequired);
      totSbReq += ue->ul.subbandRequired;
      schCmnUeUlAlloc->reqLnk.node = (PTR)ue;

      (*remUe)--;
      if(*remUe == 0)
      {
         /* Fix [ccpu00126846]: Taking care of RR behaviour during finalization */
         /* rrUlCell->prioUeTxLst.crnt = node; */
         break;
      }

   }while(node != listCp->crnt);
   /* Stop further shecudling of UEs in low priority list */
   if (totSbReq >= allocInfo->sf->availSubbands)
   {
      *remUe = 0;
   } 
   RETVOID;
}/* End of rgSchRrUlSelUes */

/**
 * @brief Round Robin related scheduling of new-transmission for the UEs
 *
 * @details
 *
 *     Function : rgSCHRrUlSchdUes
 *
 *     Processing Steps:
 *     - Traverse the new transmission list and do resource allocation
 *       for the UEs.
 *
 *  @param[in]  RgSchCellCb            *cell
 *  @param[in]  RgSchCmnUlRbAllocInfo  *allocInfo
 *  @param[in]  U8                     remUe
 *  @return  Void
 **/
#ifdef ANSI
PRIVATE Void rgSCHRrUlSchdUes
(
RgSchCellCb             *cell,
RgSchCmnUlRbAllocInfo   *allocInfo,
U8                      *remUe
)
#else
PRIVATE Void rgSCHRrUlSchdUes(cell, allocInfo, remUe)
RgSchCellCb             *cell;
RgSchCmnUlRbAllocInfo   *allocInfo;
U8                      *remUe;
#endif
{
   RgSchCmnCell *schCmnCell = (RgSchCmnCell *)(cell->sc.sch);
   RgSchRrUlCell *rrUlCell = (RgSchRrUlCell *)(schCmnCell->ul.schSpfc);
   TRC2(rgSCHRrUlSchdUes);
   /* Select UEs from Priority list first, if cell periodicity 
    * is lesser than the configured periodicity, else select UEs
    * from non Priority list first */
   if (rgSCHCmnIsUlCsgPrio(cell) == TRUE)
   {
      rgSCHRrUlSelUes(cell, allocInfo, &rrUlCell->prioUeTxLst, remUe);
      rgSCHRrUlSelUes(cell, allocInfo, &rrUlCell->normUeTxLst, remUe);
   }
   else
   {
      rgSCHRrUlSelUes(cell, allocInfo, &rrUlCell->normUeTxLst, remUe);
      rgSCHRrUlSelUes(cell, allocInfo, &rrUlCell->prioUeTxLst, remUe);
   }
   RETVOID;
}

/**
 * @brief Round Robin scheduling for a cell
 *
 * @details
 *
 *     Function : rgSCHRrUlNewTx
 *
 *     Processing Steps:
 *     - Schedule the contention resolution UEs by invoking
 *       rgSCHRrUlSchdContResUes
 *     - Schedule the new-transmission UEs by invoking
 *       rgSCHRrUlSchdUes
 *
 *  @param[in]  RgSchCellCb            *cell
 *  @param[in]  RgSchCmnUlRbAllocInfo  *allocInfo
 *  @return  Void
 **/
#ifdef ANSI
PRIVATE Void rgSCHRrUlNewTx
(
RgSchCellCb             *cell,
RgSchCmnUlRbAllocInfo   *allocInfo
)
#else
PRIVATE Void rgSCHRrUlNewTx(cell, allocInfo)
RgSchCellCb             *cell;
RgSchCmnUlRbAllocInfo   *allocInfo;
#endif
{
   RgSchCmnCell *schCmnCell = (RgSchCmnCell *)(cell->sc.sch);
   U8 remUe = schCmnCell->ul.maxUeNewTxPerTti;
   /* Fix ccpu00120610*/
   U8 remAllocs = schCmnCell->ul.maxAllocPerUlSf - *allocInfo->sf->allocCountRef;
   TRC2(rgSCHRrUlNewTx);
   /* Fix:ccpu00120610 consider remaining allocs in a subframe, because allocs may
    * have been consumed earlier as part of msg3 allocation.
    * Note: this needs to be carried over to other schedulers. */
   if (remUe > remAllocs)
   {
      remUe = remAllocs;
   }
   /* MS_WORKAROUND : Limiting one PUSCH per UL SF */
/*Removing the WA as fixed to control from configuration*/
   rgSCHRrUlSchdContResUes(cell, allocInfo, &remUe);
   rgSCHRrUlSchdUes(cell, allocInfo, &remUe);
   if (*allocInfo->sf->allocCountRef > schCmnCell->ul.maxAllocPerUlSf)
   {
      RLOG_ARG2(L_DEBUG,DBG_CELLID,cell->cellId,
               "More than expecte UEs per TTI allocated in UL exp(%u)"
               "allocated(%u)",
               schCmnCell->ul.maxAllocPerUlSf, 
               *allocInfo->sf->allocCountRef);
   }
   RETVOID;
}

/**
 * @brief Round Robin scheduling for a cell
 *
 * @details
 *
 *     Function : rgSCHRrUlSchd
 *
 *     Processing Steps:
 *     - Schedule the UEs and do resource allocation.
 *
 *  @param[in]  RgSchCellCb            *cell
 *  @param[in]  RgSchCmnUlRbAllocInfo  *allocInfo
 *  @return  Void
 **/
#ifdef ANSI
PUBLIC Void rgSCHRrUlSchd
(
RgSchCellCb             *cell,
RgSchCmnUlRbAllocInfo   *allocInfo
)
#else
PUBLIC Void rgSCHRrUlSchd(cell, allocInfo)
RgSchCellCb             *cell;
RgSchCmnUlRbAllocInfo   *allocInfo;
#endif
{
   TRC2(rgSCHRrUlSchd);

   rgSCHRrUlNewTx(cell, allocInfo);

   RETVOID;
}

/**
 * @brief Ul allocation finalization for the contresolution UEs.
 *
 * @details
 *
 *     Function : rgSCHRrUlContResUesAllocFnlz
 *
 *     Processing Steps:
 *     - If the allocation succeded then remove from contres list.
 *
 *  @param[in]  RgSchCellCb            *cell
 *  @param[in]  RgSchCmnUlRbAllocInfo  *allocInfo
 *  @return  Void
 **/
#ifdef ANSI
PRIVATE Void rgSCHRrUlContResUesAllocFnlz
(
RgSchCellCb             *cell,
RgSchCmnUlRbAllocInfo   *allocInfo
)
#else
PRIVATE Void rgSCHRrUlContResUesAllocFnlz(cell, allocInfo)
RgSchCellCb             *cell;
RgSchCmnUlRbAllocInfo   *allocInfo;
#endif
{
   RgSchCmnCell *schCmnCell = (RgSchCmnCell *)(cell->sc.sch);
   RgSchRrUlCell *rrUlCell = (RgSchRrUlCell *)(schCmnCell->ul.schSpfc);
   RgSchUeCb    *ue;
   RgSchCmnUe *schCmnUe;
   RgSchCmnUeUlAlloc *schCmnUeUlAlloc;
   RgSchRrUlUe  *rrUlUe;
   CmLList      *node;
   TRC2(rgSCHRrUlContResUesAllocFnlz);

   node = allocInfo->schdContResLst.first;
   while(node)
   {
      ue = (RgSchUeCb *)(node->node);
#ifdef LTEMAC_HDFDD
      if (ue->hdFddEnbld)
      {
         rgSCHCmnHdFddUpdULMark (cell,ue);
      }
#endif
      schCmnUe = (RgSchCmnUe *)(ue->sch);
      schCmnUeUlAlloc = &(schCmnUe->ul.alloc);
      rrUlUe = (RgSchRrUlUe *)(schCmnUe->ul.schSpfc);
      node = node->next;
      rgSCHRrCListDelFrm(&(rrUlCell->contResLst), &(rrUlUe->contResLnk));
      rrUlUe->contResLnk.node = NULLP;
      schCmnUeUlAlloc->reqLnk.node = NULLP;
   }
   node = allocInfo->nonSchdContResLst.first;
   while(node)
   {
      ue = (RgSchUeCb *)(node->node);
      node = node->next;
      schCmnUe = (RgSchCmnUe *)(ue->sch);
      schCmnUeUlAlloc = &(schCmnUe->ul.alloc);
      schCmnUeUlAlloc->reqLnk.node = NULLP;
   }
   RETVOID;
}


/**
 * @brief Ul allocation finalization for the new-transmission UE.
 *
 * @details
 *
 *     Function : rgSCHRrUlUeAllocFnlz
 *
 *     Processing Steps:
 *     - If allocation succeded and effective BSR is ZERO
 *       then remove UE from the transmission list.
 *
 *  @param[in]  RgSchCellCb      *cell
 *  @param[in]  RgSchUeCb        *ue
 *  @return  Void
 **/
#ifdef ANSI
PRIVATE Void rgSCHRrUlUeAllocFnlz
(
RgSchCellCb       *cell,
RgSchUeCb         *ue,
CmLListCp         *ulInactvLst
)
#else
PRIVATE Void rgSCHRrUlUeAllocFnlz(cell, ue, ulInactvLst)
RgSchCellCb       *cell;
RgSchUeCb         *ue;
CmLListCp         *ulInactvLst;
#endif
{
   RgSchCmnCell *schCmnCell = (RgSchCmnCell *)(cell->sc.sch);
   RgSchRrUlCell *rrUlCell = (RgSchRrUlCell *)(schCmnCell->ul.schSpfc);
   RgSchCmnUe *schCmnUe = (RgSchCmnUe *)(ue->sch);
   RgSchRrUlUe  *rrUlUe = (RgSchRrUlUe *)(schCmnUe->ul.schSpfc);
   RgSchDrxUeCb      *drxUe    = NULLP;
   TRC2(rgSCHRrUlUeAllocFnlz);

   /* The srRcvd is set to TRUE in case of BSR timer expiry case also. 
    * In such case, the DRX mask should not be changed. Hence replacing with 
    * the srGrant flag. 
    * TODO: We should remove the duplicate flag srRcvd from both RR and PFS 
    *       schedulers 
    */   
   if(ue->isSrGrant == TRUE)
   {
      if (ue->isDrxEnabled)
      {
         drxUe = RG_SCH_DRX_GET_UE(ue);
         drxUe->drxUlInactvMask  |= RG_SCH_DRX_SR_BITMASK;
         if(!RG_SCH_DRX_UL_IS_UE_ACTIVE(drxUe))
         {
            ue->ul.ulInactvMask |= RG_DRX_INACTIVE;
            /* Add to Ul inactive List */
            ue->ulDrxInactvLnk.node  = (PTR)ue;
            cmLListAdd2Tail(ulInactvLst,&(ue->ulDrxInactvLnk));
         }
         drxUe->srRcvd = FALSE;
      }
      rrUlUe->srRcvd = FALSE;
      ue->isSrGrant = FALSE;
   }

   if(schCmnUe->ul.effBsr == 0)
   {
      rgSCHRrUlDelUeTxLst(ue,rrUlCell,rrUlUe);
   }
   /* Fix [ccpu00126846]: Taking care of RR behaviour during finalization to 
      avoid starvation of previously non-scheduled UEs */
   else
   {
      /* Remove the UE from the Tx list and add to tail */
      if(ue->csgMmbrSta == TRUE)
      {
         rgSCHRrCListDelFrm(&(rrUlCell->prioUeTxLst), &(rrUlUe->ueTxLnk));
         rgSCHRrCListAdd2Tail(&(rrUlCell->prioUeTxLst), &(rrUlUe->ueTxLnk));
      }
      else
      {
         rgSCHRrCListDelFrm(&(rrUlCell->normUeTxLst), &(rrUlUe->ueTxLnk));
         rgSCHRrCListAdd2Tail(&(rrUlCell->normUeTxLst), &(rrUlUe->ueTxLnk));
      }
   }
   RETVOID;
}

/**
 * @brief Ul allocation finalization for the new-transmission UEs.
 *
 * @details
 *
 *     Function : rgSCHRrUlUesAllocFnlz
 *
 *     Processing Steps:
 *     - Finalize the Ul allocations for the UEs by invoking
 *       rgSCHRrUlUeAllocFnlz
 *
 *  @param[in]  RgSchCellCb            *cell
 *  @param[in]  RgSchCmnUlRbAllocInfo  *allocInfo
 *  @return  Void
 **/
#ifdef ANSI
PRIVATE Void rgSCHRrUlUesAllocFnlz
(
RgSchCellCb             *cell,
RgSchCmnUlRbAllocInfo   *allocInfo
)
#else
PRIVATE Void rgSCHRrUlUesAllocFnlz(cell, allocInfo)
RgSchCellCb             *cell;
RgSchCmnUlRbAllocInfo   *allocInfo;
#endif
{
   RgSchUeCb    *ue;
   RgSchCmnUe *schCmnUe;
   RgSchCmnUeUlAlloc *schCmnUeUlAlloc;
   CmLList      *node;

   CmLListCp         ulInactvLst; /* list of UE's becoming UL-inactive */
   TRC2(rgSCHRrUlUesAllocFnlz);

   cmLListInit(&ulInactvLst);
   node = allocInfo->schdUeLst.first;
   while(node)
   {
      ue = (RgSchUeCb *)(node->node);
      node = node->next;
#ifdef LTEMAC_HDFDD
      if (ue->hdFddEnbld)
      {
         rgSCHCmnHdFddUpdULMark (cell,ue);
      }
#endif
      rgSCHRrUlUeAllocFnlz(cell, ue, &ulInactvLst);
      schCmnUe = (RgSchCmnUe *)(ue->sch);
      schCmnUeUlAlloc = &(schCmnUe->ul.alloc);
      schCmnUeUlAlloc->reqLnk.node = NULLP;
   }
   rgSCHRrUlInactvtUes(cell, &ulInactvLst);
   node = allocInfo->nonSchdUeLst.first;
   while(node)
   {
      ue = (RgSchUeCb *)(node->node);
      node = node->next;
      schCmnUe = (RgSchCmnUe *)(ue->sch);
      schCmnUeUlAlloc = &(schCmnUe->ul.alloc);
      schCmnUeUlAlloc->reqLnk.node = NULLP;
   }
   RETVOID;
}

/**
 * @brief Ul allocation finalization for the new-transmission UEs.
 *
 * @details
 *
 *     Function : rgSCHRrUlAllocFnlz
 *
 *     Processing Steps:
 *     - Finalize the Ul allocations for the contres UEs by invoking
 *       rgSCHRrUlContResUesAllocFnlz
 *     - Finalize the Ul allocations for the new-transmission UEs by
 *       invoking rgSCHRrUlUesAllocFnlz
 *
 *  @param[in]  RgSchCellCb            *cell
 *  @param[in]  RgSchCmnUlRbAllocInfo  *allocInfo
 *  @return  Void
 **/
#ifdef ANSI
PUBLIC Void rgSCHRrUlAllocFnlz
(
RgSchCellCb             *cell,
RgSchCmnUlRbAllocInfo   *allocInfo
)
#else
PUBLIC Void rgSCHRrUlAllocFnlz(cell, allocInfo)
RgSchCellCb             *cell;
RgSchCmnUlRbAllocInfo   *allocInfo;
#endif
{
   TRC2(rgSCHRrUlAllocFnlz);

   rgSCHRrUlContResUesAllocFnlz(cell, allocInfo);
   rgSCHRrUlUesAllocFnlz(cell, allocInfo);

   RETVOID;
}

/**
 * @brief Indication of list of inactive UEs
 *
 * @details
 *
 *     Function: rgSCHRrUlInactvtUes
 *     Purpose:  RR does nothing
 *
 *  @param[in]  RgSchCellCb        *cell
 *  @param[in]  CmLListCp          *lst
 *  @return  Void
 *
 **/
#ifdef ANSI
PUBLIC Void rgSCHRrUlInactvtUes
(
RgSchCellCb        *cell,
CmLListCp          *lst
)
#else
PUBLIC Void rgSCHRrUlInactvtUes(cell, lst)
RgSchCellCb        *cell;
CmLListCp          *lst;
#endif
{
   TRC2(rgSCHRrUlInactvtUes);

   RETVOID;
}


/**
 * @brief UE Lc Config for RR 
 *
 * @details
 *
 *     Function : rgSCHRrUlLchCfg
 *
 *     Processing Steps:  Dummy function
 *
 *  @param[in]  RgrSchCellCb     *cell
 *  @param[in]  RgSchUeCb        *ue
 *  @param[in]  RgrLchCfg        *cfg 
 *  @param[in]  RgSchErrInfo     *err 
 *  @return  S16
 *      -# ROK
 **/
PUBLIC S16 rgSCHRrUlLchCfg 
(
RgSchCellCb      *cell,
RgSchUeCb        *ue,
RgrLchCfg        *cfg,
RgSchErrInfo     *err
)
{
   RETVALUE(ROK);
}
/**
 * @brief UE Lc Reconfig for RR 
 *
 * @details
 *
 *     Function : rgSCHRrUlLchRecfg
 *
 *     Processing Steps:  Dummy function
 *
 *  @param[in]  RgrSchCellCb     *cell
 *  @param[in]  RgSchUeCb        *ue
 *  @param[in]  RgrLchRecfg      *recfg 
 *  @param[in]  RgSchErrInfo     *err 
 *  @return  S16
 *      -# ROK
 **/
PUBLIC S16 rgSCHRrUlLchRecfg 
(
RgSchCellCb      *cell,
RgSchUeCb        *ue,
RgrLchRecfg      *recfg,
RgSchErrInfo     *err
)
{
   RETVALUE(ROK);
}

/**
 * @brief LC deletion for PFS
 *
 * @details
 *
 *     Function : rgSCHRrUlLchDel
 *
 *     Processing Steps: Dummy function
 *
 *  @param[in]  RgrSchCellCb     *cell
 *  @param[in]  RgSchUeCb        *ue
 *  @param[in]  CmLteLcId        lcId
 *  @return  S16
 *      -# ROK
 **/
#ifdef ANSI
PUBLIC S16 rgSCHRrUlLchDel
(
RgSchCellCb      *cell,
RgSchUeCb        *ue,
CmLteLcId        lcId,
U8               lcgId
)
#else
PUBLIC S16 rgSCHRrUlLchDel(cell, ue, lcId, lcgId)
RgSchCellCb      *cell;
RgSchUeCb        *ue;
CmLteLcId        lcId;
U8               lcgId;
#endif
{
  RETVALUE (ROK);
}
/**
 * @brief This function initializes all the data for the scheduler
 *
 * @details
 *
 *     Function: rgSCHRrUlInit
 *     Purpose:  This function initializes the UL RR SCH
 *
 *  @param[in]  RgDlSchdApis *rgSchDlApis
 *  @return  Void
 *
 **/
#ifdef ANSI
PUBLIC Void rgSCHRrUlInit
(
RgUlSchdApis *rgSchUlApis
)
#else
PUBLIC Void rgSCHRrUlInit(rgSchUlApis)
RgUlSchdApis *rgSchUlApis;
#endif
{
   TRC2(rgSCHRrUlInit);

   /* Init the function pointers */
   rgSchUlApis->rgSCHRgrUlUeCfg     = rgSCHRrUlUeCfg;
   rgSchUlApis->rgSCHRgrUlUeRecfg   = rgSCHRrUlUeRecfg;
   rgSchUlApis->rgSCHFreeUlUe       = rgSCHRrUlUeDel;
   rgSchUlApis->rgSCHRgrUlCellCfg   = rgSCHRrUlCellCfg;
   rgSchUlApis->rgSCHRgrUlCellRecfg = rgSCHRrUlCellRecfg;
   rgSchUlApis->rgSCHFreeUlCell     = rgSCHRrUlCellDel;
   rgSchUlApis->rgSCHRgrUlLcCfg     = rgSCHRrUlLchCfg;
   rgSchUlApis->rgSCHRgrUlLcRecfg   = rgSCHRrUlLchRecfg;
   rgSchUlApis->rgSCHRgrUlLcgCfg    = rgSCHRrUlLcgCfg;
   rgSchUlApis->rgSCHRgrUlLcgRecfg  = rgSCHRrUlLcgRecfg;
   rgSchUlApis->rgSCHFreeUlLcg      = rgSCHRrUlLcgDel;
   rgSchUlApis->rgSCHRgrUlLchDel    = rgSCHRrUlLchDel;
   rgSchUlApis->rgSCHUlSched        = rgSCHRrUlSchd;
   rgSchUlApis->rgSCHUpdBsrShort    = rgSCHRrUpdShortBsr;
   rgSchUlApis->rgSCHUpdBsrTrunc    = rgSCHRrUpdTruncBsr;
   rgSchUlApis->rgSCHUpdBsrLong     = rgSCHRrUpdLongBsr;
   rgSchUlApis->rgSCHContResUlGrant = rgSCHRrContResUlGrant;
   rgSchUlApis->rgSCHSrRcvd         = rgSCHRrSrInd;
   rgSchUlApis->rgSCHUlCqiInd       = rgSCHRrUlCqiInd;
   rgSchUlApis->rgSCHUlUeRefresh    = rgSCHRrUlUeRefresh;
   rgSchUlApis->rgSCHUlAllocFnlz    = rgSCHRrUlAllocFnlz;
   rgSchUlApis->rgSCHUlActvtUe      = rgSCHRrUlActvtUe;
   rgSchUlApis->rgSCHUlUeReset      = rgSCHRrUlUeReset;
   rgSchUlApis->rgSCHUlInactvtUes   = rgSCHRrUlInactvtUes;
   rgSchUlApis->rgSCHRgrUlLcgUpd    = rgSCHRrUlLcgUpd;

   RETVOID;
}


/**
 * @brief 
 *
 * @details
 *
 *     Function : 
 *
 *     Processing Steps:
 *     
 * 
 *
 *  @param[in]  RgSchUeCb      *ueCb
 *  @param[in]  RgSchRrDlCell  *rrDlCell
 *  @param[in]  RgSchRrDlUe    *rrDlUe
 *  @return  Void
 **/
#ifdef ANSI
PRIVATE Void rgSCHRrDlAddUeTxLst
(
RgSchUeCb      *ue,
RgSchRrDlCell  *rrDlCell,
RgSchRrDlUe    *rrDlUe
)
#else
PRIVATE Void rgSCHRrDlAddUeTxLst(ueCb, rrDlCell, rrDlUe)
RgSchUeCb      *ue;
RgSchRrDlCell  *rrDlCell;
RgSchRrDlUe    *rrDlUe;
#endif
{
   TRC2(rgSCHRrDlAddUeTxLst)

   if (rrDlUe->ueTxLnk.node == NULLP)
   {
      if(ue->csgMmbrSta == TRUE)
      {
         rgSCHRrCListAdd2Tail(&(rrDlCell->prioUeTxLst), &(rrDlUe->ueTxLnk));
      }
      else
      {
         rgSCHRrCListAdd2Tail(&(rrDlCell->normUeTxLst), &(rrDlUe->ueTxLnk));
      }
      rrDlUe->ueTxLnk.node = (PTR)ue;
   }
}


/**
 * @brief 
 *
 * @details
 *
 *     Function : 
 *
 *     Processing Steps:
 *     
 * 
 *
 *  @param[in]  RgSchUeCb      *ueCb
 *  @param[in]  RgSchRrUlCell  *rrUlCell
 *  @param[in]  RgSchRrUlUe    *rrUlUe
 *  @return  Void
 **/
#ifdef ANSI
PRIVATE Void rgSCHRrUlAddUeTxLst
(
RgSchUeCb      *ue,
RgSchRrUlCell  *rrUlCell,
RgSchRrUlUe    *rrUlUe
)
#else
PRIVATE Void rgSCHRrUlAddUeTxLst(ue, rrUlCell, rrUlUe)
RgSchUeCb      *ue;
RgSchRrUlCell  *rrUlCell;
RgSchRrUlUe    *rrUlUe;
#endif
{
   TRC2(rgSCHRrUlAddUeTxLst)

   if (rrUlUe->ueTxLnk.node == NULLP)
   {
      if(ue->csgMmbrSta == TRUE)
      {
         rgSCHRrCListAdd2Tail(&(rrUlCell->prioUeTxLst), &(rrUlUe->ueTxLnk));
      }
      else
      {
         rgSCHRrCListAdd2Tail(&(rrUlCell->normUeTxLst), &(rrUlUe->ueTxLnk));
      }
      rrUlUe->ueTxLnk.node = (PTR)ue;
   }
}


/**
 * @brief 
 *
 * @details
 *
 *     Function : 
 *
 *     Processing Steps:
 *     
 * 
 *
 *  @param[in]  RgSchUeCb      *ueCb
 *  @param[in]  RgSchRrDlCell  *rrDlCell
 *  @param[in]  RgSchRrDlUe    *rrDlUe
 *  @return  Void
 **/
#ifdef ANSI
PRIVATE Void rgSCHRrDlDelUeTxLst
(
RgSchUeCb      *ue,
RgSchRrDlCell  *rrDlCell,
RgSchRrDlUe    *rrDlUe
)
#else
PRIVATE Void rgSCHRrDlDelUeTxLst(ue, rrDlCell, rrDlUe)
RgSchUeCb      *ue;
RgSchRrDlCell  *rrDlCell;
RgSchRrDlUe    *rrDlUe;
#endif
{
   TRC2(rgSCHRrDlDelUeTxLst)

   if (rrDlUe->ueTxLnk.node != NULLP)
   {
      if(ue->csgMmbrSta == TRUE)
      {
         rgSCHRrCListDelFrm(&(rrDlCell->prioUeTxLst), &(rrDlUe->ueTxLnk));
      }
      else
      {
         rgSCHRrCListDelFrm(&(rrDlCell->normUeTxLst), &(rrDlUe->ueTxLnk));
      }
      rrDlUe->ueTxLnk.node = NULLP;
   }
}



/**
 * @brief 
 *
 * @details
 *
 *     Function : 
 *
 *     Processing Steps:
 *     
 * 
 *
 *  @param[in]  RgSchUeCb      *ueCb
 *  @param[in]  RgSchRrUlCell  *rrUlCell
 *  @param[in]  RgSchRrUlUe    *rrUlUe
 *  @return  Void
 **/
#ifdef ANSI
PRIVATE Void rgSCHRrUlDelUeTxLst
(
RgSchUeCb      *ue,
RgSchRrUlCell  *rrUlCell,
RgSchRrUlUe    *rrUlUe
)
#else
PRIVATE Void rgSCHRrUlDelUeTxLst(ue, rrUlCell, rrUlUe)
RgSchUeCb      *ue;
RgSchRrUlCell  *rrUlCell;
RgSchRrUlUe    *rrUlUe;
#endif
{
   TRC2(rgSCHRrUlAddUeTxLst)

   if (rrUlUe->ueTxLnk.node != NULLP)
   {
      if(ue->csgMmbrSta == TRUE)
      {
         rgSCHRrCListDelFrm(&(rrUlCell->prioUeTxLst), &(rrUlUe->ueTxLnk));
      }
      else
      {
         rgSCHRrCListDelFrm(&(rrUlCell->normUeTxLst), &(rrUlUe->ueTxLnk));
      }
      rrUlUe->ueTxLnk.node = NULLP;
   }
}


/**********************************************************************

         End of file:     gk_sch_rr.c@@/main/2 - Sat Jul 30 02:21:48 2011

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
/main/1      ---     rr   1. LTE MAC 2.1 release
             rg001.201 sm 1. Removed extra comments.
             rg003.201 ns 1. Trace added to functions.
             rg004.201 ms 1. DL DTX Handling
                          2. Functions and Datatypes starting
                             with rr are renamed as rgSchRr
           rg005.201 sm      1. Added Macro to check if UE is picked for Retx
           rg006.201 sd   1. ccpu00110358: Removed ndi toggling post release
                             of HARQ proc.
                          2. ccpu00112398: Added periodic BSR timer
           rg007.201   ap  1. Added support for MIMO
           rg008.201 sd   1. Removed dependency on MIMO compile-time flag
              rsharon 2. Added support for SPS.
/main/2      ---        gvj  1. Updated for LTE MAC Rel 3.1
           rg001.301 nudupi  1. ccpu00118350:Correcting NDI manipulation of Harq.
           rg002.301  rntiwari 1.ccpu00120941: Added handling for releasing HARQ
                                 proc, deleting UE from txUeLst and resetting
                                 outStndAlloc.
$SID$        ---      rt     1. LTE MAC 4.1 release
*********************************************************************91*/
