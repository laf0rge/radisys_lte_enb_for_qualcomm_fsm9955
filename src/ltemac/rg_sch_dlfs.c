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
 
     Desc:     C source code for DLFS functions
 
     File:     rg_sch_dlfs.c
 
     Sid:      gk_sch_dlfs.c@@/main/2 - Sat Jul 30 02:21:40 2011
 
     Prg:     sd
 
**********************************************************************/

/** @file rg_sch_dlfs.c
@brief This module handles the downlink frequency selective scheduler functionality
*/

#ifdef TFU_UPGRADE
static const char* RLOG_MODULE_NAME="MAC";
static int RLOG_MODULE_ID=4096;
static int RLOG_FILE_ID=238;
#endif
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
#include "lrg.h"
#include "rgr.h"
#include "rgm.h"
#include "tfu.h"
#include "rg_env.h"
#include "rg_sch_inf.h"
#include "rg_sch_err.h"
#include "rg_sch.h"
#include "rg_sch_cmn.h"
#include "rg_sch_dlfs.h"
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
#include "rg_sch_dlfs.x"        /* typedefs for DLFS Scheduler */

#ifdef TFU_UPGRADE
/* Private functions */

/* 
 * DLFS specific functions 
 */

/* DLFS Initialization functions */
PRIVATE Void rgSCHDlfsType2RbAssgn ARGS((
        RgSchDlfsCellCb *, RgSchDlfsSf *,
        RgSchDlRbAlloc *, U32 *, U32 *, U32 *));
PRIVATE Void rgSCHDlfsType0RbAssgn ARGS((
        RgSchDlfsCellCb *, RgSchDlfsSf *, RgSchUeCb *, 
        RgSchDlRbAlloc *, U32 *, U32 *, U32 *));
PRIVATE Void rgSCHDlfsAdjstITbs ARGS((RgSchCellCb *,
        RgSchDlfsSf *, RgSchDlRbAlloc *, U32 )); 
PRIVATE U32 rgSCHDlfsGetEffiTbs ARGS((RgSchCellCb *,
        U32, U32, U32 )); 
PRIVATE Void rgSCHDlfsSetPbchOvrLapRbs ARGS((RgSchCellCb *, 
        RgSchDlfsSf *));
PRIVATE S16 rgSCHDlfsInitSf ARGS((RgSchCellCb *, RgSchDlSf *));
PRIVATE Void rgSCHDlfsResetSfSb ARGS((RgSchDlfsCellCb *, RgSchDlfsSf *));
PRIVATE S16 rgSCHDlfsInitCell ARGS((RgSchCellCb *, RgSchDlfsCellCb *));
PRIVATE S16 rgSCHDlfsCreateUe ARGS((RgSchCellCb *, RgSchUeCb *));
PRIVATE Void rgSCHDlfsInitUeBp ARGS((RgSchDlfsCellCb *, RgSchDlfsUeCb *));
PRIVATE Void rgSCHDlfsInitSfSbs ARGS((RgSchDlfsCellCb *, RgSchDlfsSf *));
PRIVATE S16 rgSCHDlfsInitPrdSBModes ARGS((Inst, RgSchDlfsCellCb *,
         RgSchDlfsUeCb *, RgrUePrdDlCqiCfg *));
PRIVATE S16 rgSCHDlfsUpdPrdModeCfg ARGS((RgSchCellCb *, RgSchUeCb *, 
         RgrUePrdDlCqiCfg *));
PRIVATE Void rgSCHDlfsUpdAprdModeCfg ARGS((RgSchCellCb *, RgSchUeCb *, 
         RgrUeAprdDlCqiCfg *));

/* DLFS Sub-band/hole handling functions */
PRIVATE Void rgSCHDlfsRlsAlloc ARGS((RgSchDlfsCellCb *, RgSchDlfsSf *, 
                                     U32 ));

/* DLFS CQI updation functions */
PRIVATE Void rgSCHDlfsUpdWbCqi ARGS((RgSchDlfsCellCb *, 
                                     RgSchDlfsUeCb *, U8 ));
PRIVATE Void rgSCHDlfsUpdPrdMode20SbCqi ARGS((RgSchDlfsCellCb *, 
         RgSchDlfsUeCb *, TfuCqiPucchMode20 *, CmLteTimingInfo ));
PRIVATE Void rgSCHDlfsUpdAprdMode20SbCqi ARGS(( RgSchDlfsCellCb *,
         RgSchDlfsUeCb *, TfuCqiPuschMode20 *));
PRIVATE Void rgSCHDlfsUpdAprdMode30SbCqi ARGS(( RgSchDlfsCellCb *,
         RgSchDlfsUeCb *, TfuCqiPuschMode30 *));
PRIVATE S16 rgSCHDlfsCalcBpIdx ARGS((CmLteTimingInfo , RgSchDlfsCellCb *,
                                     RgSchDlfsUeCb * ,U32 *bpIdx));
PRIVATE Void rgSCHDlfsUpdAprdMode22SbCqi ARGS((RgSchDlfsCellCb *, RgSchDlfsUeCb *, 
          TfuCqiPuschMode22 *));
PRIVATE Void rgSCHDlfsUpdAprdMode31SbCqi ARGS((RgSchDlfsCellCb *, RgSchDlfsUeCb *, 
          TfuCqiPuschMode31 *));

/* DLFS Allocation functions */
PRIVATE Void rgSCHDlfsTxUeRbAlloc ARGS(( RgSchCellCb *, RgSchDlSf *,
         CmLListCp *, CmLListCp *, CmLListCp *));
PRIVATE Void rgSCHDlfsSbAllocForTxUe ARGS((RgSchDlSf *, RgSchCellCb *,
         RgSchUeCb *, RgSchDlRbAlloc *));
PRIVATE Void rgSCHDlfsRetxUeRbAlloc ARGS(( RgSchCellCb *, RgSchDlSf *,
         CmLListCp *, CmLListCp *, CmLListCp *));
PRIVATE Void rgSCHDlfsSbAllocForRetxUe ARGS((RgSchDlSf *, RgSchCellCb *,
         RgSchUeCb *, RgSchDlRbAlloc *));
PRIVATE S16 rgSCHDlfsMsg4Alloc ARGS ((RgSchCellCb *,
         RgSchCmnDlRbAllocInfo *, U8 ));
#ifdef RGR_V1
PRIVATE S16 rgSCHDlfsCcchSduAlloc ARGS ((RgSchCellCb *,
         RgSchCmnDlRbAllocInfo *, U8 ));
#endif
PRIVATE S16 rgSCHDlfsCmnRbAlloc ARGS((RgSchCellCb *, 
         RgSchDlRbAlloc *));
PRIVATE Void rgSCHDlfsAllocSb ARGS((RgSchDlfsSf *, RgSchDlfsHole *, U32 *,
         U32 *));

/* DLFS Utility functions */
PRIVATE Void rgSCHDlfsFreeSf ARGS((RgSchDlfsCellCb *, Inst , 
                                   RgSchDlfsSf *));
PRIVATE Void rgSCHDlfsFreeUeBp ARGS((Inst , RgSchDlfsCellCb *,
                                     RgSchDlfsUeCb *));

PRIVATE Void rgSCHDlfsUpdPrdMode21WbCqi ARGS((
RgSchDlfsCellCb    *dlfsCell,
RgSchDlfsUeCb      *dlfsUe,
TfuCqiPucchMode21  *mode21CqiInfo
));
PRIVATE Void rgSCHDlfsUpdPrdMode21SbCqi ARGS((
RgSchDlfsCellCb    *dlfsCell,
RgSchDlfsUeCb      *dlfsUe,
TfuCqiPucchMode21  *mode21CqiInfo,
CmLteTimingInfo    timingInfo
));
PRIVATE Void rgSCHDlfsPrcSbCqiUpd ARGS((
RgSchDlfsCellCb    *dlfsCell,
RgSchDlfsUeCb      *dlfsUe,
RgSchDlfsSbCqi     *sbCb,
U32                sbCqi));
PRIVATE Void rgSCHDlfsUpdSbCqiDeviation ARGS((
RgSchDlfsCellCb    *dlfsCell,
RgSchDlfsUeCb      *dlfsUe,
RgSchDlfsSbCqi     *sbCb,
U32                 prevSbCqi
)); 
PRIVATE Void rgSCHDlfsUpdSbSortedLst ARGS((
RgSchDlfsUeCb      *dlfsUe,
RgSchDlfsSbCqi     *sbCb,
U32                prevSbCqi
)); 
PRIVATE Void rgSCHDlfsRmvSbFrmLst ARGS((
RgSchDlfsUeCb      *dlfsUe,
RgSchDlfsSbCqi     *sbCb
));
PRIVATE Void rgSCHDlfsMarkAllocdRbgs ARGS((
RgSchCellCb    *cell,
RgSchDlSf      *sf
));

/**
 * @brief This function initializes all the data for the DLFS scheduler
 *
 * @details
 *
 *     Function: rgSCHDlfsInit
 *     Purpose: This function registers the DLFS scheduler with the APIs exposed
 *     by it.
 *     
 *     Invoked by: MAC intialization code.
 *     
 *  @return  Void
 *
 **/
#ifdef ANSI
PUBLIC Void rgSCHDlfsInit
(
RgDlfsSchdApis *rgDlfsApis
)
#else
PUBLIC Void rgSCHDlfsInit(rgDlfsApis)
RgDlfsSchdApis *rgDlfsApis;
#endif 
{
   TRC2(rgSCHDlfsInit);
   /* Init the function pointers */
   rgDlfsApis->rgSCHDlfsCellCfg    = rgSCHDlfsCellCfg;
   rgDlfsApis->rgSCHDlfsCellRecfg  = rgSCHDlfsCellRecfg;
   rgDlfsApis->rgSCHDlfsCellDel    = rgSCHDlfsCellDel;
   rgDlfsApis->rgSCHDlfsUeCfg      = rgSCHDlfsUeCfg; 
   rgDlfsApis->rgSCHDlfsUeRecfg    = rgSCHDlfsUeRecfg;
   rgDlfsApis->rgSCHDlfsUeDel      = rgSCHDlfsUeDel;
   rgDlfsApis->rgSCHDlfsDlCqiInd   = rgSCHDlfsDlCqiIndHndlr;
   rgDlfsApis->rgSCHDlfsReinitSf   = rgSCHDlfsReinitSf;
   rgDlfsApis->rgSCHDlfsAllocRb    = rgSCHDlfsRbAlloc;
   rgDlfsApis->rgSCHDlfsBcchPcchAllocRb = rgSCHDlfsBcchPcchRbAlloc;
   rgDlfsApis->rgSCHDlfsAddUeToLst      = rgSCHDlfsOdrUeForRbAssgnmt;

   RETVOID;
}

/**
 * @brief Initializes the DLFS scheduler for the cell.
 *
 * @details
 *     Function : rgSCHDlfsCellCfg
 *
 *     Invoking Module Processing:
 *      - SCH shall invoke this API at cell configuration if downlink frequency
 *      selective is enabled.
 *       
 *     Processing Steps:
 *     - No DLFS specific validations are needed.
 *     - Update configured values.
 *     - Initialize the cell. Call rgSCHDlfsInitCell()
 *
 *   @param[in]  RgSchCellCb   *cell
 *   @param[in]  RgrCellCfg    *cellCfg
 *   @param[out] RgSchErrInfo  *err
 *  @return  S16
 *      -# ROK
 *      -# RFAILED
 **/
#ifdef ANSI
PUBLIC S16 rgSCHDlfsCellCfg 
(
RgSchCellCb        *cell,
RgrCellCfg         *cellCfg,
RgSchErrInfo       *err
)
#else
PUBLIC S16 rgSCHDlfsCellCfg (cell, cellCfg, err) 
RgSchCellCb        *cell;
RgrCellCfg         *cellCfg;
RgSchErrInfo       *err;
#endif
{
   S16             ret       = ROK;
   Inst            instIdx   = cell->instIdx;
   RgSchDlfsCellCb *dlfsCell;
   RgSchCmnCell    *cmnCell  = RG_SCH_CMN_GET_CELL(cell);
   TRC2(rgSCHDlfsCellCfg);


   ret = rgSCHUtlAllocSBuf(instIdx,
         (Data**)&((RgSchCmnCell*)((cell)->sc.sch))->dl.dlfsCell,
         (sizeof(RgSchDlfsCellCb)));
   if(ret != ROK)
   {
      RLOG_ARG0(L_ERROR,DBG_CELLID,cell->cellId, 
               "Memory allocation FAILED for DLFS cell");
      err->errCause = RGSCHERR_DLFS_MEMERR;
      RETVALUE(ret);
   }

   dlfsCell = RG_SCH_DLFS_GET_CELL(cell);

   /* Update DLFS specific configuration */
   dlfsCell->defaultCqi = cmnCell->dl.ccchCqi;
   dlfsCell->totDlBw = cellCfg->bwCfg.dlTotalBw;

   /* Initialize the cell */
   if ((ret = rgSCHDlfsInitCell(cell, dlfsCell))!= ROK)
   {
      RLOG_ARG1(L_ERROR,DBG_CELLID,cell->cellId, 
               "Memory allocation FAILED for DLFS cell %d", cell->cellId);
      err->errCause = RGSCHERR_DLFS_MEMERR;
      RETVALUE(ret);
   }


   RETVALUE(ret);
}

/**
 * @brief Re-initializes the DLFS scheduler at cell-reconfiguration
 *
 * @details
 *
 *     Function : rgSCHDlfsCellRecfg
 *
 *     Invoking Module Processing:
 *      - SCH shall invoke this at cell re-configuration.
 *      
 *     Processing Steps:
 *      - Validate the re-configured values
 *      - If validation failed, return RFAILED
 *      - If validation succeeded,
 *       - if isDlFreqSel variable has been toggled
 *        - if (cell->isDlFreqSel is TRUE) && (isDlFreqSel is FALSE)
 *          - clear all the list per ue and per cell associated with DL FS
 *        - else if (cell->isDlFreqSel is FALSE) && (isDlFreqSel is TRUE)
 *          - Initialize all related variables and lists for cell and UE
 *          - Set cell->isDlFreqSel = isDlFreqSel
 *          - Initialize sub-bands for the cell. Call  rgSCHDlfsInitSf().
 *
 *   @param[in]  RgSchCellCb   *cell,
 *   @param[in]  RgrCellRecfg  *cellRecfg
 *   @param[out] RgSchErrInfo  *err
 *  @return  S16
 *      -# ROK
 *      -# RFAILED
 **/
#ifdef ANSI
PUBLIC S16 rgSCHDlfsCellRecfg
(
RgSchCellCb        *cell,
RgrCellRecfg       *cellRecfg,
RgSchErrInfo       *err
)
#else
PUBLIC S16 rgSCHDlfsCellRecfg (cell, cellRecfg, err) 
RgSchCellCb        *cell;
RgrCellRecfg       *cellRecfg;
RgSchErrInfo       *err;
#endif
{
   S16             ret       = ROK;
   Inst            instIdx   = cell->instIdx;
   RgSchUeCb       *prvUe    = NULLP;
   RgSchUeCb       *ue       = NULLP;
   RgSchCmnCell    *cmnCell  = RG_SCH_CMN_GET_CELL(cell);
   RgSchDlfsCellCb *dlfsCell = NULLP;
   TRC2(rgSCHDlfsCellRecfg);


   if (!cellRecfg->dlfsRecfg.isDlFreqSel)
   {
      if (cmnCell->dl.isDlFreqSel) 
      {
         /* Frequency selective has been disabled for the cell 
          * Clear DLFS related information */

         /* Clear DLFS specific information for the UE */
         while ((ue = rgSCHDbmGetNextUeCb(cell, prvUe)) != NULLP)
         {
            rgSCHDlfsUeDel(cell, ue);
            prvUe = ue;
         }

         /* Free DLFS cell */
         rgSCHDlfsCellDel(cell);
      }
      RETVALUE(ROK);
   }

   /* Processing for frequency selective enabled */
   if (!(cmnCell->dl.isDlFreqSel)) 
   {
      /* Frequency selective has been enabled for the cell: create cell
       * specific data strutures */ 
      ret = rgSCHUtlAllocSBuf(instIdx,
            (Data**)&((RgSchCmnCell*)((cell)->sc.sch))->dl.dlfsCell,
            (sizeof(RgSchDlfsCellCb)));
      if(ret != ROK)
      {
         RLOG_ARG0(L_ERROR,DBG_INSTID,instIdx, 
                  "Memory allocation FAILED for DLFS cell");
         err->errCause = RGSCHERR_DLFS_MEMERR;
         RETVALUE(ret);
      }

      dlfsCell = RG_SCH_DLFS_GET_CELL(cell);
      dlfsCell->totDlBw = cell->bwCfg.dlTotalBw; 

      /* Initialize the cell */
      if ((ret = rgSCHDlfsInitCell(cell, dlfsCell))!= ROK)
      {
         RLOG_ARG0(L_ERROR,DBG_CELLID,cell->cellId,
                  "Memory allocation FAILED for DLFS cell");
         err->errCause = RGSCHERR_DLFS_MEMERR;
         RETVALUE(ret);
      }

      /* Create DLFS UE for the UEs in the cell */
      while ((ue = rgSCHDbmGetNextUeCb(cell, prvUe)) != NULLP)
      {
         ret = rgSCHDlfsCreateUe(cell, ue);
         if (ret != ROK)
         {
            RLOG_ARG0(L_ERROR,DBG_CELLID,cell->cellId,
                     "DLFS cell reconfig failed for cell");
            err->errCause = RGSCHERR_DLFS_MEMERR;
            RETVALUE(RFAILED);
         }
         prvUe = ue;
      }

   }

   /* Update DLFS specific configuration */
   if (!dlfsCell)
   {
      dlfsCell = RG_SCH_DLFS_GET_CELL(cell);
   }

   /* Update default and threshold CQI */
   dlfsCell->defaultCqi = cmnCell->dl.ccchCqi;

   RETVALUE (ROK);
}


/**
 * @brief Delete the DLFS scheduler context for the cell at cell delete
 *
 * @details
 *
 *     Function : rgSCHDlfsCellDel
 *     
 *     Invoking Module Processing:
 *      - SCH shall invoke this if downlink frequency selective is enabled for
 *      the cell at cell delete.
 *
 *     Processing Steps:
 *      - Free DLFS related information per DL subframe in the cell. 
 *        Call rgSCHDlfsFreeSf().
 *      - Free cell specific informationfor DLFS, if any.
 *      - Note: UE specific information is not freed over here since it is
 *      assumed that UE delete shall be invoked for all UEs prior to this call.
 *      - Return Void
 *
 *  
 *  @param[in]  RgSchCellCb *cell
 *  @return  Void
 **/
#ifdef ANSI
PUBLIC Void rgSCHDlfsCellDel 
(
RgSchCellCb        *cell
)
#else
PUBLIC Void rgSCHDlfsCellDel (cell) 
RgSchCellCb        *cell;
#endif
{
   RgSchDlfsCellCb *dlfsCell = RG_SCH_DLFS_GET_CELL(cell);
   U8              dlsfIdx   = 0;
   RgSchDlfsSf     *dlfsSf    = NULLP;  
   Inst            instIdx   = cell->instIdx;
   TRC2(rgSCHDlfsCellDel);

   
   /* Free DLFS subframe related information */
   /* Changing loop limit from
      RGSCH_NUM_SUB_FRAMES to RGSCH_NUM_DL_SUBFRAMES */
#ifndef LTE_TDD
   for (dlsfIdx = 0; dlsfIdx < RGSCH_NUM_DL_SUBFRAMES; ++dlsfIdx)
#else
   for (dlsfIdx = 0; dlsfIdx < cell->numDlSubfrms; ++dlsfIdx)
#endif
   {
      dlfsSf = RG_SCH_DLFS_GET_SF(cell->subFrms[dlsfIdx]);
      rgSCHDlfsFreeSf(dlfsCell, instIdx, dlfsSf);
   }

   if (dlfsCell)
   {
      /* ccpu00117052 - MOD - Passing double pointer
      for proper NULLP assignment*/
      rgSCHUtlFreeSBuf(instIdx, (Data**)(&(((RgSchCmnCell*)\
                     ((cell)->sc.sch))->dl.dlfsCell)),
            sizeof(*dlfsCell));
   }

   RETVOID;
}

/**
 * @brief Initializes UE with the DLFS scheduler at UE Configuration.
 *
 * @details
 *
 *     Function : rgSCHDlfsUeCfg
 *
 *     Invoking Module Processing:
 *      - SCH shall invoke this if downlink frequency selective is enabled for
 *        the cell at UE configuration.
 *      - SCH shall validate the configured values
 *         - Validate the transmission mode
 *         - Validate the reporting mode for pucch and pusch (if configured)
 *         - Validate reporting mode against transmission mode.
 *
 *     Processing Steps:
 *      - Set ue->wbCqi = cell->defaultCqi; 
 *      - Initalize DL FS related variables per UE
 *      - Return ROK
 *
 *  @param[in]   RgSchCellCb   *cell
 *  @param[in]   RgSchUeCb     *ue
 *  @param[in]   RgrUeCfg      *ueCfg
 *   @param[out] RgSchErrInfo  *err
 *  @return  S16
 *      -# ROK
 **/
#ifdef ANSI
PUBLIC S16 rgSCHDlfsUeCfg 
(
RgSchCellCb        *cell,
RgSchUeCb          *ue,
RgrUeCfg           *ueCfg,
RgSchErrInfo       *err
)
#else
PUBLIC S16 rgSCHDlfsUeCfg (cell, ue, ueCfg, err) 
RgSchCellCb        *cell;
RgSchUeCb          *ue;
RgrUeCfg           *ueCfg;
RgSchErrInfo       *err;
#endif
{
   S16             ret       = ROK;
   TRC2(rgSCHDlfsUeCfg);

   ret = rgSCHDlfsCreateUe(cell, ue);
   if (ret != ROK)
   {
      err->errCause = RGSCHERR_DLFS_MEMERR;
      RETVALUE(RFAILED);
   }

   RETVALUE(ROK);
}

/**
 * @brief Re-initializes UE with the DLFS scheduler at UE re-configuration.
 *
 * @details
 *
 *     Function : rgSCHDlfsUeRecfg
 *
 *     Invoking Module Processing:
 *      - SCH shall invoke this if downlink frequency selective is enabled for
 *        the UE at UE recfg.
 *      - SCH shall validate the re-configured values
 *         - Validate the transmission mode
 *         - Validate the reporting mode for pucch and pusch (if configured)
 *         - Validate reporting mode against transmission mode.
 *         
 *     Processing Steps:
 *      - If CQI reporting is disabled,
 *        - The old CQIs are retained.
 *      - Return ROK
 *        
 *
 *  @param[in]     RgSchCellCb    *cell
 *  @param[in]     RgSchUeCb      *ue
 *  @param[in]     RgrUeRecfg     *ueRecfg
 *  @param[out]    RgrSchErrInfo  *err
 *  @return  S16
 *      -# ROK
 **/
#ifdef ANSI
PUBLIC S16 rgSCHDlfsUeRecfg 
(
RgSchCellCb        *cell,
RgSchUeCb          *ue,
RgrUeRecfg         *ueRecfg,
RgSchErrInfo       *err
)
#else
PUBLIC S16 rgSCHDlfsUeRecfg (cell, ue, ueRecfg, err) 
RgSchCellCb        *cell;
RgSchUeCb          *ue;
RgrUeRecfg         *ueRecfg;
RgSchErrInfo       *err;
#endif
{
   S16             ret       = ROK;
   Inst            instIdx   = cell->instIdx;
   RgSchDlfsUeCb   *dlfsUe   = RG_SCH_DLFS_GET_UE(ue);
   RgSchDlfsCellCb *dlfsCell = RG_SCH_DLFS_GET_CELL(cell);

   TRC2(rgSCHDlfsUeRecfg);


   /* Depending on the reporting mode, free/create BP information */

   if (ueRecfg->ueRecfgTypes & RGR_UE_PCQI_RECFG)
   {
#ifndef TFU_UPGRADE
      if (ueRecfg->prdDlCqiRecfg.pres)
      {
         ret = rgSCHDlfsUpdPrdModeCfg(cell, ue, &ueRecfg->prdDlCqiRecfg);
#else
      if (ueRecfg->cqiCfg.type)
      {

         ret = rgSCHDlfsUpdPrdModeCfg(cell, ue, &ueRecfg->cqiCfg);

#endif /* TFU_UPGRADE */
         if (ret != ROK)
         {
            err->errCause = RGSCHERR_DLFS_MEMERR;
            RETVALUE(RFAILED);
         }
      }
      else
      {
         /* Free bandwidth parts if allocated for the UE */
         if (dlfsUe->bpCbs)
         {
            rgSCHDlfsFreeUeBp(instIdx, dlfsCell, dlfsUe);
         }
         /* Re-setting prdSbSize */
         dlfsUe->prdSbSize = 0;
      }
   }

   /* Update aperiodic sub-band size */
   if (ueRecfg->ueRecfgTypes & RGR_UE_APRD_DLCQI_RECFG)
   {
      if (ueRecfg->aprdDlCqiRecfg.pres)
      {
         rgSCHDlfsUpdAprdModeCfg(cell, ue, &ueRecfg->aprdDlCqiRecfg);
      }
      else
      {
         /* Re-setting aprdSbSize */
         dlfsUe->aprdSbSize = 0;
      }
   }

   RETVALUE(ROK);
}

/**
 * @brief Clears UE context with the DLFS scheduler at UE delete.
 *
 * @details
 *
 *     Function : rgSCHDlfsUeDel
 *
 *     Invoking Module Processing:
 *      - SCH shall invoke this if downlink frequency selective is enabled for
 *        the cell at UE delete.
 *
 *     Processing Steps:
 *     - Free Ue specific information 
 *     - Return ROK
 *
 * @param[in]  RgSchCellCb *cell
 * @param[in]  RgSchUeCb   *ue
 * @return  Void
 **/
#ifdef ANSI
PUBLIC Void rgSCHDlfsUeDel 
(
RgSchCellCb      *cell,
RgSchUeCb        *ue
)
#else
PUBLIC Void rgSCHDlfsUeDel (cell, ue) 
RgSchCellCb      *cell;
RgSchUeCb        *ue;
#endif
{
   RgSchDlfsUeCb   *dlfsUe = RG_SCH_DLFS_GET_UE(ue);
   RgSchDlfsCellCb *dlfsCell = RG_SCH_DLFS_GET_CELL(cell);
   Inst            instIdx = cell->instIdx;
   TRC2(rgSCHDlfsUeDel);


   /* Clear ue specific DLFS information */
   if (dlfsUe)
   {
      /* Free the list of sub-band CQIs allocated for the UE */
      if (dlfsUe->sbCqiLst)
      {
         /* ccpu00117052 - MOD - Passing double pointer
         for proper NULLP assignment*/
         rgSCHUtlFreeSBuf(instIdx, (Data**)(&(dlfsUe->sbCqiLst)),
               (dlfsCell->numDlSb * sizeof(RgSchDlfsSbCqi)));
      }

      /* Free bandwidth part information */
      if (dlfsUe->bpCbs)
      {
         rgSCHDlfsFreeUeBp(instIdx, dlfsCell, dlfsUe);
      }

         /* ccpu00117052 - MOD - Passing double pointer
         for proper NULLP assignment*/
      rgSCHUtlFreeSBuf(instIdx,
            (Data **)(&(((RgSchCmnUe *)((ue)->sch))->dl.dlfsUe)),
            sizeof(RgSchDlfsUeCb));
   }

   RETVOID;
}

/**
 * @brief Downlink CQI Indication handler 
 *
 * @details
 *
 *     Function : rgSCHDlfsDlCqiIndHndlr
 *
 *     Invoking Module Processing:
 *      - SCH shall invoke this if downlink frequency selective is enabled for
 *        the cell at downlink CQI indication from PHY for the UE.
 *        
 *     Processing Steps:
 *     - It is assumed that the reporting of the CQI shall be strictly on the
 *     basis of the configured mode of the UE.
 *     - Per reporting mode, 
 *        - Update wideband CQI.
 *        - Update sub-band CQI, if reported.
 *     - If sub-band CQI has been reported, sort the sub-band CQI list in 
 *       desceding order of CQIs. Call rgSCHDlfsSortSbCqiLst().
 *     - Compute the deviation of sub-band CQI from average CQI. Call
 *       rgSCHDlfsCalcSbCqiDeviation().
 *
 *  @param[in]  RgSchCellCb     *cell
 *  @param[in]  RgSchUeCb       *ue
 *  @param[in]  TfuDlCqiRpt     *cqiIndInfo
 *  @param[in]  CmLteTimingInfo timingInfo
 *  @return  Void
 **/

#ifdef ANSI
PUBLIC Void rgSCHDlfsDlCqiIndHndlr
(
RgSchCellCb          *cell,
RgSchUeCb            *ue,
TfuDlCqiRpt          *cqiIndInfo,
CmLteTimingInfo      timingInfo
)
#else
PUBLIC Void rgSCHDlfsDlCqiIndHndlr(cell, ue, cqiIndInfo, timingInfo)
RgSchCellCb          *cell;
RgSchUeCb            *ue;
TfuDlCqiRpt          *cqiIndInfo;
CmLteTimingInfo      timingInfo;
#endif
{
   RgSchDlfsCellCb   *dlfsCell     = RG_SCH_DLFS_GET_CELL(cell);
   RgSchDlfsUeCb     *dlfsUe       = RG_SCH_DLFS_GET_UE(ue);
   TfuDlCqiPucch     *pucchCqiInfo = NULLP;
   TfuDlCqiPusch     *puschCqiInfo = NULLP; 
   TRC2(rgSCHDlfsDlCqiIndHndlr);


   if (cqiIndInfo->isPucchInfo)
   {
      /* Periodic CQI report recieved */
      pucchCqiInfo = &(cqiIndInfo->dlCqiInfo.pucchCqi);
      switch (pucchCqiInfo->mode)
      {
         case TFU_PUCCH_CQI_MODE10:
            {
               if (pucchCqiInfo->u.mode10Info.type != TFU_RPT_CQI)
               {
                  RETVOID;
               }
               /* Only wideband CQI is recieved */
               rgSCHDlfsUpdWbCqi(dlfsCell, dlfsUe, 
                    pucchCqiInfo->u.mode10Info.u.cqi);
               break;
            }
         case TFU_PUCCH_CQI_MODE20:
            {
               if (pucchCqiInfo->u.mode20Info.type != TFU_RPT_CQI)
               {
                  RETVOID;
               }
               if (pucchCqiInfo->u.mode20Info.u.cqi.isWideband)
               {
                  /* Wideband CQI is recieved */
                  rgSCHDlfsUpdWbCqi(dlfsCell, dlfsUe, 
                        pucchCqiInfo->u.mode20Info.u.cqi.u.wideCqi);
               }
               else
               {
                  /* Sub-band CQI received for preferred sub-band in a BP */
                  rgSCHDlfsUpdPrdMode20SbCqi(dlfsCell, dlfsUe, 
                        &pucchCqiInfo->u.mode20Info, timingInfo);
               }
               break;
            }
         case TFU_PUCCH_CQI_MODE11:
            {
               U8 avgCqi, cw1Cqi, cw2Cqi;
               if (pucchCqiInfo->u.mode11Info.type != TFU_RPT_CQI)
               {
                  RETVOID;
               }
               /* Only wideband CQI is recieved */
               /* Considering Wideband CQI as an average of both
                * CW1 and CW2 CQIs */
               avgCqi = cw1Cqi = pucchCqiInfo->u.mode11Info.u.cqi.cqi;
               if (pucchCqiInfo->u.mode11Info.u.cqi.wideDiffCqi.pres)
               {
                  RG_SCH_UPDT_CW2_CQI(cw1Cqi,cw2Cqi,pucchCqiInfo->u.mode11Info.u.cqi.wideDiffCqi.val);
                  avgCqi = (cw1Cqi+cw2Cqi)/2;
               }
               rgSCHDlfsUpdWbCqi(dlfsCell, dlfsUe, avgCqi);
               break;
            }
         case TFU_PUCCH_CQI_MODE21:
            {
               if (pucchCqiInfo->u.mode21Info.type != TFU_RPT_CQI)
               {
                  RETVOID;
               }
               if (pucchCqiInfo->u.mode21Info.u.cqi.isWideband)
               {
                  /* Wideband CQI is recieved */
                  rgSCHDlfsUpdPrdMode21WbCqi(dlfsCell, dlfsUe, 
                        &pucchCqiInfo->u.mode21Info);
               }
               else
               {
                  /* Sub-band CQI received for preferred sub-band in a BP */
                  rgSCHDlfsUpdPrdMode21SbCqi(dlfsCell, dlfsUe, 
                        &pucchCqiInfo->u.mode21Info, timingInfo);
               }
               break;
            }
         default:
            {
               RLOG_ARG2(L_ERROR,DBG_CELLID,cell->cellId,
                        "Invalid periodic CQI reporting mode config %d CRNTI:%d"
                        ,(int)pucchCqiInfo->mode,ue->ueId);
               break;
            }
      }
   }
   else
   {
      /* Aperiodic CQI report recieved */
      puschCqiInfo = &(cqiIndInfo->dlCqiInfo.puschCqi); 
      switch (puschCqiInfo->mode)
      {
         case TFU_PUSCH_CQI_MODE_20:
            {
               /* Update wideband CQI */
               rgSCHDlfsUpdWbCqi(dlfsCell, dlfsUe,
                     puschCqiInfo->u.mode20Info.wideBandCqi);

               /* Update Sub-band CQI recieved for preferred sub-bands */
               rgSCHDlfsUpdAprdMode20SbCqi(dlfsCell, dlfsUe,
                     &(puschCqiInfo->u.mode20Info));
               break;
            }
         case TFU_PUSCH_CQI_MODE_30:
            {
               /* Update wideband CQI */
               rgSCHDlfsUpdWbCqi(dlfsCell, dlfsUe, 
                     puschCqiInfo->u.mode30Info.wideBandCqi);

               /* Update Sub-band CQI recieved for all sub-bands */
               rgSCHDlfsUpdAprdMode30SbCqi(dlfsCell, dlfsUe,
                     &puschCqiInfo->u.mode30Info);
               break;
            }
         case TFU_PUSCH_CQI_MODE_12:
            {
               /* Update wideband CQI as an average of 
                * CW1 and CW2's CQIs */
               rgSCHDlfsUpdWbCqi(dlfsCell, dlfsUe, 
                     (U8)((puschCqiInfo->u.mode12Info.cqiIdx[0] +\
                      puschCqiInfo->u.mode12Info.cqiIdx[1])/2));
               break;
            }
         case TFU_PUSCH_CQI_MODE_22:
            {
               /* Update wideband CQI as an average of 
                * CW1 and CW2's CQIs */
               rgSCHDlfsUpdWbCqi(dlfsCell, dlfsUe, 
                     (U8) ((puschCqiInfo->u.mode22Info.cqi[0] +\
                      puschCqiInfo->u.mode22Info.cqi[1])/2));
               /* Update Sub-band CQI recieved for all sub-bands */
               rgSCHDlfsUpdAprdMode22SbCqi(dlfsCell, dlfsUe,
                     &puschCqiInfo->u.mode22Info);
               break;
            }
         case TFU_PUSCH_CQI_MODE_31:
            {
               /* Update wideband CQI as an average of 
                * CW1 and CW2's CQIs */
               rgSCHDlfsUpdWbCqi(dlfsCell, dlfsUe, 
                    (U8) ((puschCqiInfo->u.mode31Info.wideBandCqi[0] +\
                      puschCqiInfo->u.mode31Info.wideBandCqi[1])/2));
               /* Update Sub-band CQI recieved for all sub-bands */
               rgSCHDlfsUpdAprdMode31SbCqi(dlfsCell, dlfsUe,
                     &puschCqiInfo->u.mode31Info);
               break;
            }            
         default:
            {
               RLOG_ARG2(L_ERROR,DBG_CELLID,cell->cellId,
                        "Invalid aperiodic CQI reporting mode config %d CRNTI:%d"
                        ,(int)puschCqiInfo->mode,ue->ueId);
               break;
            }
      }
   }

   RETVOID;
}


/**
 * @brief Handles DLFS based RB allocation
 *
 * @details
 *
 *     Function : rgSCHDlfsRbAlloc
 *
 *     Invoking Module Processing:
 *      - SCH shall invoke this if downlink frequency selective is enabled for
 *        the cell for RB allocation.
 *      - MAX C/I/PFS/RR shall provide the requiredBytes, required RBs
 *        estimate and subframe for each allocation to be made to SCH.
 *        
 *     Processing Steps:
 *     - Allocate for common channel CCCH,
 *       RaRsp. Call  rgSCHDlfsCmnRbAlloc(). Call rgSCHDlfsMsg4Alloc().
 *     - For transmiting and re-transmiting UE list,
 *       - Call rgSCHDlfsTxUeRbAlloc().
 *       - Call rgSCHDlfsRetxUeRbAlloc().
 *
 *  @param[in]  RgSchCellCb           *cell
 *  @param[in]  RgSchCmnDlRbAllocInfo *allocInfo
 *  @return  Void
 **/

#ifdef ANSI
PUBLIC Void rgSCHDlfsRbAlloc
(
RgSchCellCb           *cell,
RgSchCmnDlRbAllocInfo *allocInfo
)
#else
PUBLIC Void rgSCHDlfsRbAlloc(cell, allocInfo)
RgSchCellCb           *cell;
RgSchCmnDlRbAllocInfo *allocInfo;
#endif
{
   S16                ret            = ROK;
   U8                 raRspCnt       = 0;
   RgSchDlRbAlloc     *reqAllocInfo;
   TRC2(rgSCHDlfsRbAlloc);

   /* Allocate for MSG4 retransmissions */
   /* Assuming all the nodes in the list need allocations: rbsReq is valid */
   if (allocInfo->msg4Alloc.msg4RetxLst.count)
   {
      ret = rgSCHDlfsMsg4Alloc(cell, allocInfo, RG_SCH_DLFS_RETX_TRUE);
   }

   /* If bandwidth remains, Allocate for MSG4 transmissions */
   /* Assuming all the nodes in the list need allocations: rbsReq is valid */
   if ((ret == ROK) && (allocInfo->msg4Alloc.msg4TxLst.count))
   {
      rgSCHDlfsMsg4Alloc(cell, allocInfo, RG_SCH_DLFS_RETX_FALSE);
   }

   /* Allocate for CCCH SDU (received after guard timer expiry) retransmissions */
#ifdef RGR_V1
   if (allocInfo->ccchSduAlloc.ccchSduRetxLst.count)
   {
      ret = rgSCHDlfsCcchSduAlloc(cell, allocInfo, RG_SCH_DLFS_RETX_TRUE);
   }
   
   /* If bandwidth remains, Allocate for CCCH SDU transmissions */
   /* Allocate for CCCH SDU (received after guard timer expiry) transmissions */
   if ((ret == ROK) && (allocInfo->ccchSduAlloc.ccchSduTxLst.count))
   {
      rgSCHDlfsCcchSduAlloc(cell, allocInfo, RG_SCH_DLFS_RETX_FALSE);
   }
#endif
   /* Allocate for Random access response */
   for (raRspCnt = 0; raRspCnt < RG_SCH_CMN_MAX_CMN_PDCCH; ++raRspCnt)
   {
      /* Assuming that the requests will be filled in sequentially */
      reqAllocInfo = &(allocInfo->raRspAlloc[raRspCnt]);
      if (!reqAllocInfo->rbsReq)
      {
         break;
      }
      if ((ret = rgSCHDlfsCmnRbAlloc(cell, reqAllocInfo)) != ROK)
      {
         break;
      }
   }

   /* Re-set the hole list's start if common allocations are done
    * for the dedicated sub-frame */
   rgSCHDlfsMarkAllocdRbgs(cell, allocInfo->dedAlloc.dedDlSf); 
#ifdef LTEMAC_SPS
   /* Allocate for re-transmitting SPS UEs */
   if (allocInfo->dedAlloc.retxSpsHqPLst.count)
   {
      rgSCHDlfsRetxUeRbAlloc(cell, allocInfo->dedAlloc.dedDlSf, 
            &(allocInfo->dedAlloc.retxSpsHqPLst),
            &(allocInfo->dedAlloc.schdRetxSpsHqPLst),
            &(allocInfo->dedAlloc.nonSchdRetxSpsHqPLst));
   }

   /* Allocate for transmitting SPS UEs */
   if (allocInfo->dedAlloc.txSpsHqPLst.count)
   {
      rgSCHDlfsTxUeRbAlloc(cell, allocInfo->dedAlloc.dedDlSf, 
            &(allocInfo->dedAlloc.txSpsHqPLst),
            &(allocInfo->dedAlloc.schdTxSpsHqPLst), 
            &(allocInfo->dedAlloc.nonSchdTxSpsHqPLst));
   }
#endif


   if (allocInfo->dedAlloc.txRetxHqPLst.count)
   {
      rgSCHDlfsRetxUeRbAlloc(cell, allocInfo->dedAlloc.dedDlSf, 
            &(allocInfo->dedAlloc.txRetxHqPLst),
            &(allocInfo->dedAlloc.schdTxRetxHqPLst), 
            &(allocInfo->dedAlloc.nonSchdTxRetxHqPLst));
   }
   /* Allocate for re-transmitting UEs */
   if (allocInfo->dedAlloc.retxHqPLst.count)
   {
      rgSCHDlfsRetxUeRbAlloc(cell, allocInfo->dedAlloc.dedDlSf, 
            &(allocInfo->dedAlloc.retxHqPLst),
            &(allocInfo->dedAlloc.schdRetxHqPLst), 
            &(allocInfo->dedAlloc.nonSchdRetxHqPLst));
   }

   /* Allocate for transmitting UEs */
   if (allocInfo->dedAlloc.txHqPLst.count)
   {
      rgSCHDlfsTxUeRbAlloc(cell, allocInfo->dedAlloc.dedDlSf, 
            &(allocInfo->dedAlloc.txHqPLst),
            &(allocInfo->dedAlloc.schdTxHqPLst), 
            &(allocInfo->dedAlloc.nonSchdTxHqPLst));
   }
   RETVOID;
}

/**
 * @brief Updates configuration for periodic mode
 *
 * @details
 *
 *     Function : rgSCHDlfsUpdPrdModeCfg
 *
 *     Processing Steps:
 *
 *  @param[in]  RgSchCellCb       *cell
 *  @param[in]  RgSchUeCb         *ue
 *  @param[in]  RgrUePrdDlCqiCfg  *prdCqiCfg
 *  @return  S16
 *      -# ROK
 *      -# RFAILED
 **/

#ifdef ANSI
PRIVATE S16 rgSCHDlfsUpdPrdModeCfg
(
RgSchCellCb            *cell,
RgSchUeCb              *ue,
RgrUePrdDlCqiCfg       *prdCqiCfg
)
#else
PRIVATE S16 rgSCHDlfsUpdPrdModeCfg(cell, ue, prdCqiCfg)
RgSchCellCb            *cell;
RgSchUeCb              *ue;
RgrUePrdDlCqiCfg       *prdCqiCfg;
#endif
{
   S16                 ret         = ROK;
   U32                 mode;
   Inst                instIdx     = cell->instIdx;
   RgSchDlfsCellCb     *dlfsCell   = RG_SCH_DLFS_GET_CELL(cell);
   RgSchDlfsUeCb       *dlfsUe     = RG_SCH_DLFS_GET_UE(ue);
   TRC2(rgSCHDlfsUpdPrdModeCfg);

#ifndef TFU_UPGRADE
    mode = prdCqiCfg->prdModeEnum;
#else
    mode = prdCqiCfg->cqiSetup.prdModeEnum;
#endif

   switch (mode)
   {
      case RGR_PRD_CQI_MOD20:
      case RGR_PRD_CQI_MOD21:
         {

            /* Initialize periodic Mode 20 and 21 information */
            ret = rgSCHDlfsInitPrdSBModes(instIdx, dlfsCell, dlfsUe, prdCqiCfg);
            if (ret != ROK)
            {
               RETVALUE(ret);
            }
            break;
         }
      default: /* RGR_PRD_CQI_MOD10 11 */
         {
            /* If allocated, de-allocate the Bandwidth parts */
            if (dlfsUe->bpCbs)
            {
               rgSCHDlfsFreeUeBp(instIdx, dlfsCell, dlfsUe);
            }
            /* Re-setting prdSbSize */
            dlfsUe->prdSbSize = 0;
            break;
         }
   }
   RETVALUE(ROK);
}

/**
 * @brief Updates configuration for aperiodic mode 
 *
 * @details
 *
 *     Function : rgSCHDlfsUpdAprdModeCfg
 *
 *     Processing Steps:
 *
 *  @param[in]  RgSchCellCb        *cell
 *  @param[in]  RgSchUeCb          *ue
 *  @param[in]  RgrUeAprdDlCqiCfg  *aprdCqiCfg
 *  @return  S16
 *      -# ROK
 *      -# RFAILED
 **/

#ifdef ANSI
PRIVATE Void rgSCHDlfsUpdAprdModeCfg
(
RgSchCellCb            *cell,
RgSchUeCb              *ue,
RgrUeAprdDlCqiCfg      *aprdCqiCfg
)
#else
PRIVATE Void rgSCHDlfsUpdAprdModeCfg(cell, ue, aprdCqiCfg)
RgSchCellCb            *cell;
RgSchUeCb              *ue;
RgrUeAprdDlCqiCfg      *aprdCqiCfg;
#endif
{
   RgSchDlfsCellCb     *dlfsCell   = RG_SCH_DLFS_GET_CELL(cell);
   RgSchDlfsUeCb       *dlfsUe     = RG_SCH_DLFS_GET_UE(ue);
   TRC2(rgSCHDlfsUpdAprdModeCfg);

   switch (aprdCqiCfg->aprdModeEnum)
   {
      case RGR_APRD_CQI_MOD12:
         {
            /* As there is no SB CQI for mode1-2, setting this to 0.
             * Note: However mode1-2 has SB PMI. This field is used
             * in rgSCHDlfsUpdWbCqi() to indicate if UE is cfgd to
             * send Aper SB CQI reports. */
            dlfsUe->aprdSbSize = 0; 
            break;
         }
      case RGR_APRD_CQI_MOD20:
      case RGR_APRD_CQI_MOD22:
         {
            RG_SCH_DLFS_GET_APRD_MODE20_SBSIZE(&(dlfsUe->aprdSbSize),
                  dlfsCell->totDlBw);
            break;
         }
      case RGR_APRD_CQI_MOD30:
      case RGR_APRD_CQI_MOD31:
         {
            RG_SCH_DLFS_GET_APRD_MODE30_SBSIZE(&(dlfsUe->aprdSbSize),
                  dlfsCell->totDlBw);
            break;
         }
   }
   RETVOID;
}

/**
 * @brief Initializes the cell for DLFS
 *
 * @details
 *
 *     Function : rgSCHDlfsInitCell
 *
 *     Processing Steps:
 *     - Compute rbgSize and number of bandwidth parts based on 
 *       DL bandwidth configuration.
 *     - For each downlink subframe, 
 *       - Initialize sub-bands and holes. Call  rgSCHDlfsInitSf().
 *
 *  @param[in]  RgSchCellCb     *cell
 *  @param[in]  RgSchDlfsCellCb *dlfsCell
 *  @return  S16
 *      -# ROK
 *      -# RFAILED
 **/

#ifdef ANSI
PRIVATE S16 rgSCHDlfsInitCell
(
RgSchCellCb         *cell,
RgSchDlfsCellCb     *dlfsCell
)
#else
PRIVATE S16 rgSCHDlfsInitCell (cell, dlfsCell)
RgSchCellCb         *cell;
RgSchDlfsCellCb     *dlfsCell;
#endif
{
   S16              ret      = ROK;
   U8               dlsfIdx  = 0;

   TRC2(rgSCHDlfsInitCell);

   if (dlfsCell->totDlBw > 63)
   {
      dlfsCell->rbgSize = 4;
      dlfsCell->numBp = 4;
   }
   else if (dlfsCell->totDlBw > 26)
   {
      dlfsCell->rbgSize = 3;
      dlfsCell->numBp = 3;
   }
   else if (dlfsCell->totDlBw > 10)
   {
      dlfsCell->rbgSize = 2;
      dlfsCell->numBp = 2;
   }
   else
   {
      dlfsCell->rbgSize = 1;
      if (dlfsCell->totDlBw > 7)
      {
         dlfsCell->numBp = 1;
      }
      else
      {
         dlfsCell->numBp = 0;
      }
   }

   dlfsCell->numDlSb =  (dlfsCell->totDlBw + dlfsCell->rbgSize - 1)/ 
      (dlfsCell->rbgSize); 


   /* Changing loop limit from
      RGSCH_NUM_SUB_FRAMES to RGSCH_NUM_DL_SUBFRAMES */
#ifndef LTE_TDD
   for (dlsfIdx = 0; dlsfIdx < RGSCH_NUM_DL_SUBFRAMES; ++dlsfIdx)
#else
   for (dlsfIdx = 0; dlsfIdx < cell->numDlSubfrms; ++dlsfIdx)
#endif
   {
      if ((ret = rgSCHDlfsInitSf(cell, cell->subFrms[dlsfIdx])) != ROK)
      {
         RLOG_ARG1(L_ERROR,DBG_CELLID,cell->cellId,
                  "Memory allocation FAILED for sub-bands %d", dlsfIdx);
         /* Note: Failure shall be handled by invoking cellDel on DLFS by the
          * caller of DLFS */ 
         RETVALUE(ret);
      }
   }


   RETVALUE(ROK);
}

/***********************************************************
 *
 *     Func : rgSCHDlfsCreateUe
 *
 *     Desc : Creates DLFS UE for a UE 
 *
 *     Ret  : S16
 *
 *     Notes:
 *
 *     File :
 *
 **********************************************************/

#ifdef ANSI
PRIVATE S16 rgSCHDlfsCreateUe
(
RgSchCellCb         *cell,
RgSchUeCb           *ue
)
#else
PRIVATE S16 rgSCHDlfsCreateUe(cell, ue)
RgSchCellCb         *cell;
RgSchUeCb           *ue;
#endif
{
   S16             ret       = ROK;
   Inst            instIdx   = cell->instIdx;
   RgSchDlfsCellCb *dlfsCell = RG_SCH_DLFS_GET_CELL(cell);
   U8              numSbCqi  = dlfsCell->numDlSb;
   RgSchCmnDlUe    *cmnDlUe  = RG_SCH_CMN_GET_DL_UE(ue);
   RgSchDlfsUeCb   *dlfsUe;
   TRC2(rgSCHDlfsCreateUe);

   /* Allocate DFLS UE */
   ret = rgSCHUtlAllocSBuf(instIdx, 
         (Data**)&((RgSchCmnUe *)((ue)->sch))->dl.dlfsUe,
         (sizeof(RgSchDlfsUeCb)));
   if(ret != ROK)
   {
      RLOG_ARG1(L_ERROR,DBG_CELLID,cell->cellId,
               "Memory allocation FAILED for DLFS CRNTI:%d",ue->ueId);
      RETVALUE(ret);
   }

   dlfsUe   = RG_SCH_DLFS_GET_UE(ue);

   /* Allocate the sub-bands for the UE */
   if((ret = rgSCHUtlAllocSBuf(cell->instIdx, 
               (Data**)&(dlfsUe->sbCqiLst), 
               numSbCqi * (sizeof(RgSchDlfsSbCqi)))) != ROK)
   {
      RLOG_ARG1(L_ERROR,DBG_CELLID,cell->cellId,
               "Allocation for sub-band cqi list failed for the CRNTI:%d",
               ue->ueId);
      RETVALUE(ret);
   }

   /* Update WB cqi for DLFS UE.
      Note: This should be called prior to CQI Mode configuration. */
   rgSCHDlfsUpdWbCqi(dlfsCell, dlfsUe, cmnDlUe->mimoInfo.cwInfo[0].cqi);

#ifndef  TFU_UPGRADE
   if (ue->dl.ueDlCqiCfg.prdCqiCfg.pres)
   {
      ret = rgSCHDlfsUpdPrdModeCfg(cell, ue, &ue->dl.ueDlCqiCfg.prdCqiCfg);
      if (ret != ROK)
      {
         RETVALUE(RFAILED);
      }
   }
#else
   if (ue->dl.ueDlCqiCfg.prdCqiCfg.type)
   {
      ret = rgSCHDlfsUpdPrdModeCfg(cell, ue, &ue->dl.ueDlCqiCfg.prdCqiCfg);
      if (ret != ROK)
      {
         RETVALUE(RFAILED);
      }
   }
#endif
   if (ue->dl.ueDlCqiCfg.aprdCqiCfg.pres)
   {
      rgSCHDlfsUpdAprdModeCfg(cell, ue, &ue->dl.ueDlCqiCfg.aprdCqiCfg);
   }


   RETVALUE(ROK);
}

/**
 * @brief Reduce ITbs if 0.93 efficiency is exceeded.
 *
 * @details
 *
 *     Function : rgSCHDlfsGetEffiTbs
 *
 *     Processing Steps:
 *     - Check whether the efficiency has not exceeded
 *        0.93 and progressively reduce the iTbs.
 *
 *  @param[in]  RgSchCellCb  *cell
 *  @param[in]  U32          iTbs
 *  @param[in]  U32          totPdschRes
 *  @param[in]  U32          totPbchRes
 *  @return  U32
 **/
#ifdef ANSI
PRIVATE U32 rgSCHDlfsGetEffiTbs
(
 RgSchCellCb     *cell,
 U32             iTbs,
 U32             totPdschRes,
 U32             totPbchRes
 )
#else
PRIVATE U32 rgSCHDlfsGetEffiTbs(cell, iTbs, totPdschRes, totPbchRes)
   RgSchCellCb    *cell;
   U32            iTbs;
   U32            totPdschRes;
   U32            totPbchRes;
#endif
{
   RgSchCmnCell *cellSch = RG_SCH_CMN_GET_CELL(cell);
   U8           cfi = cellSch->dl.currCfi;
   U32          eff;
   U32          bitsPerRe;
   U32          bitsPer1024Re;
   U32          modOdr; 
   U32          finalEff; /* finalEff is with mult factor 1000 */ 

   TRC2(rgSCHDlfsGetEffiTbs)

   do
   {
      bitsPer1024Re =  (*(RgSchCmnTbSzEff *)(cellSch->dl.cqiToEffTbl[0][cfi]))\
                       [iTbs];
      bitsPerRe = (bitsPer1024Re * 1000) / 1024; /* Multiplying by 1000 
                                             to avoid float computations */
      RG_SCH_CMN_TBS_TO_MODODR(iTbs, modOdr);

      eff = bitsPerRe/modOdr; /* eff is with mult factor 1000 */
      finalEff = (eff * totPdschRes) /(totPdschRes - totPbchRes);
      iTbs--;
   } while (finalEff > 930);
   iTbs++;

   RETVALUE(iTbs);
}

/**
 * @brief Adjust ITbs if RB assignment has overlapping PBCH RBs.
 *
 * @details
 *
 *     Function : rgSCHDlfsAdjstITbs
 *
 *     Processing Steps:
 *     - Use the preprocessed information of number of
 *       overlapping SYM and RBs and scale down iTbs 
 *       proportionate to amount of overlap.
 *
 *  @param[in]  RgSchCellCb     *cell
 *  @param[in]  RgSchDlfsSf     *dlfsSf
 *  @param[in]  RgSchDlRbAlloc  *allocInfo,
 *  @param[in]  U32             pbchRbs
 *  @return  Void
 **/
#ifdef ANSI
PRIVATE Void rgSCHDlfsAdjstITbs
(
RgSchCellCb        *cell,
RgSchDlfsSf        *dlfsSf,
RgSchDlRbAlloc     *allocInfo,
U32                pbchRbs
)
#else
PRIVATE Void rgSCHDlfsAdjstITbs(cell, dlfsSf, allocInfo, pbchRbs)
RgSchCellCb        *cell;
RgSchDlfsSf        *dlfsSf;
RgSchDlRbAlloc     *allocInfo;
U32                pbchRbs;
#endif
{
   U32 iTbsOrig = 0;
   U32 totPbchRes = 0;
   U32 totPdschRes = 0;
   RgSchCmnDlCell  *cellDl = RG_SCH_CMN_GET_DL_CELL(cell);

   TRC2(rgSCHDlfsAdjstITbs)

   totPbchRes = pbchRbs * dlfsSf->numPbchRePerRb; 
   totPdschRes = allocInfo->rbsAlloc * cellDl->noResPerRb[cellDl->currCfi];

   /* Adjust iTbs only for New Transmissions */
   if (allocInfo->tbInfo[0].tbCb->txCntr == 0) 
   {
      iTbsOrig = allocInfo->tbInfo[0].iTbs;
      allocInfo->tbInfo[0].iTbs = rgSCHDlfsGetEffiTbs(cell, iTbsOrig, totPdschRes, totPbchRes);
      RG_SCH_CMN_DL_TBS_TO_MCS(allocInfo->tbInfo[0].iTbs,
                               allocInfo->tbInfo[0].imcs);
   }

   if (allocInfo->tbInfo[1].schdlngForTb)
   {
      if (allocInfo->tbInfo[1].tbCb->txCntr == 0) 
      {
         iTbsOrig = allocInfo->tbInfo[1].iTbs;
         allocInfo->tbInfo[1].iTbs = rgSCHDlfsGetEffiTbs(cell, iTbsOrig, totPdschRes, totPbchRes);
         RG_SCH_CMN_DL_TBS_TO_MCS(allocInfo->tbInfo[1].iTbs,
               allocInfo->tbInfo[1].imcs);
      }
   }

   RETVOID;
}

/**
 * @brief Update the count of RBs overlapping 
 * with the PBCH RBs.
 *
 * @details
 *
 *     Function : rgSCHDlfsSetPbchOvrLapRbs
 *
 *     Processing Steps:
 *      - Find the SBs overlapping with PBCH and
 *        update the corresponding overlapping RB
 *        count in those SBs.
 *
 *  @param[in]  RgSchCellCb     *cell
 *  @param[in]  RgSchDlfsSf     *dlfsSf
 *  @return  Void
 **/
#ifdef ANSI
PRIVATE Void rgSCHDlfsSetPbchOvrLapRbs
(
RgSchCellCb        *cell,
RgSchDlfsSf        *dlfsSf
)
#else
PRIVATE Void rgSCHDlfsSetPbchOvrLapRbs(cell, dlfsSf)
RgSchCellCb        *cell;
RgSchDlfsSf        *dlfsSf;
#endif
{
   RgSchDlfsCellCb *dlfsCell = RG_SCH_DLFS_GET_CELL(cell);
   U32 sbIdx = 0;

   TRC2(rgSCHDlfsSetPbchOvrLapRbs)

   sbIdx = cell->pbchRbStart/dlfsCell->rbgSize;

   dlfsSf->dlSb[sbIdx].numOvrlpPbchRb = 
                          (dlfsSf->dlSb[sbIdx].start +
                           dlfsSf->dlSb[sbIdx].sbSize) -
                           cell->pbchRbStart;
   sbIdx++;
   while ((dlfsSf->dlSb[sbIdx].start + dlfsSf->dlSb[sbIdx].sbSize - 1) <
          (cell->pbchRbEnd))
   {
      dlfsSf->dlSb[sbIdx].numOvrlpPbchRb = dlfsSf->dlSb[sbIdx].sbSize;
      sbIdx++;
   }
   dlfsSf->dlSb[sbIdx].numOvrlpPbchRb = cell->pbchRbEnd - 
                                       dlfsSf->dlSb[sbIdx].start + 1;
   RETVOID;
}

/**
 * @brief Initializes the subframe for DLFS
 *
 * @details
 *
 *     Function : rgSCHDlfsInitSf
 *
 *     Processing Steps:
 *      - Allocate DL sub-bands
 *      - Initialize DL sub-bands for each DL subframe.
 *        Call rgSCHDlfsInitSfSbs().
 *      - Initialize DL holes for each DL subframe. Call rgSCHDlfsResetSfSb().
 *
 *  @param[in]  RgSchCellCb     *cell
 *  @param[in]  RgSchDlSf       *sf
 *  @return  S16
 *      -# ROK
 *      -# RFAILED
 **/

#ifdef ANSI
PRIVATE S16 rgSCHDlfsInitSf
(
RgSchCellCb        *cell,
RgSchDlSf          *sf
)
#else
PRIVATE S16 rgSCHDlfsInitSf (cell, sf)
RgSchCellCb        *cell;
RgSchDlSf          *sf;
#endif
{
   S16             ret       = ROK;
   Inst            instIdx   = cell->instIdx;
   RgSchDlfsCellCb *dlfsCell = RG_SCH_DLFS_GET_CELL(cell);
   RgSchDlfsSf     *dlfsSf;
   TRC2(rgSCHDlfsInitSf);
   
   if((ret = rgSCHUtlAllocSBuf(instIdx, 
               (Data**)&(sf->dlfsSf), 
               (sizeof(RgSchDlfsSf)))) != ROK)
   {
      RLOG_ARG0(L_ERROR,DBG_CELLID,cell->cellId,
               "Memory allocation FAILED for DLFS sub-frame");
      /* Note: Failure shall be handled by invoking cellDel on DLFS by the
       * caller of DLFS */ 
      RETVALUE(ret);
   }

   dlfsSf = RG_SCH_DLFS_GET_SF(sf);
   /* Allocate DFLS Sub-bands */
   if((ret = rgSCHUtlAllocSBuf(instIdx, 
               (Data**)&(dlfsSf->dlSb), 
             dlfsCell->numDlSb * sizeof(RgSchDlfsSb))) != ROK)
   {
      RLOG_ARG0(L_ERROR,DBG_CELLID,cell->cellId,
               "Memory allocation FAILED for sub-bands");
      RETVALUE(ret);
   }
   
   /* Initialize DL sub-bands */
   rgSCHDlfsInitSfSbs(dlfsCell, dlfsSf);

   /* Initialize Hole control block */
   rgSCHDlfsResetSfSb(dlfsCell, dlfsSf);

#ifdef LTEMAC_SPS
   /* Setting allocated bandwidth to SPS bandwidth for non-SPS RB allocator */
   dlfsSf->allocDlBw = cell->spsCellCfg.maxSpsDlBw;
#else
   dlfsSf->allocDlBw = 0;
#endif
   dlfsSf->availDlBw = dlfsCell->totDlBw - dlfsSf->allocDlBw;
   
   /* Since Reference Signals and CFI impact all the RBs,
    * hence assuming its impact to be mininal. However
    * PBCH, PSS and SSS effect specifically some UEs
    * hence considering their impact and adjusting 
    * accordingly */
#ifdef LTE_TDD
   /* for Tdd reduce iTbs only for SF0 due to PBCH */
   if (sf->sfNum == 0)
   {
      dlfsSf->numPbchRePerRb = RGSCH_NUM_PBCH_SYM *
                               RGSCH_NUM_SC_IN_RB;
      rgSCHDlfsSetPbchOvrLapRbs(cell, dlfsSf);
   }
#else
   if (sf->sfNum == 0)
   {
      dlfsSf->numPbchRePerRb = (RGSCH_NUM_PBCH_SYM + RGSCH_NUM_PSS_SSS_SYM) *
                               RGSCH_NUM_SC_IN_RB;
      rgSCHDlfsSetPbchOvrLapRbs(cell, dlfsSf);
   }
   else if (sf->sfNum % 5 == 0)
   {
      dlfsSf->numPbchRePerRb = RGSCH_NUM_PSS_SSS_SYM *
                               RGSCH_NUM_SC_IN_RB;
      rgSCHDlfsSetPbchOvrLapRbs(cell, dlfsSf);
   }
#endif
   RETVALUE(ret);
}

/**
 * @brief Re-initializes the subframe for dlfs
 *
 * @details
 *
 *     Function : rgSCHDlfsReinitSf
 *
 *     Processing Steps:
 *      - Re-initialize DL holes. Call rgSCHDlfsResetSfSb().
 *
 *  @param[in]  RgSchDlfsCellCb *cell
 *  @param[in]  RgSchDlSf       *dlSf
 *  @return  Void
 **/
#ifdef ANSI
PUBLIC Void rgSCHDlfsReinitSf
(
RgSchCellCb        *cell,
RgSchDlSf          *dlSf
)
#else
PUBLIC Void rgSCHDlfsReinitSf(cell, dlSf)
RgSchCellCb        *cell;
RgSchDlSf          *dlSf;
#endif
{
   RgSchDlfsSf     *dlfsSf   = RG_SCH_DLFS_GET_SF(dlSf);
   RgSchDlfsCellCb *dlfsCell = RG_SCH_DLFS_GET_CELL(cell);
   TRC2(rgSCHDlfsReinitSf);

   dlSf->bwAlloced   = 0;
   rgSCHDlfsResetSfSb(dlfsCell, dlfsSf);
#ifdef LTEMAC_SPS
   /* Setting allocated bandwidth to SPS bandwidth for non-SPS RB allocator */
   dlfsSf->allocDlBw = cell->spsCellCfg.maxSpsDlBw;
#else
   dlfsSf->allocDlBw = 0;
#endif
   /* Naw*/
   dlfsSf->availDlBw = dlfsCell->totDlBw - dlfsSf->allocDlBw;
   RETVOID;
}

/**
 * @brief Initializes the Bandwidth part information for a UE
 *
 * @details
 *
 *     Function : rgSCHDlfsInitUeBp
 *
 *     Processing Steps:
 *     - Initialize the bandwidth parts for the UE.
 *
 *  @param[in]  RgSchDlfsCellCb *dlfsCell
 *  @param[in]  RgSchDlfsUe     *dlfsUe
 *  @return  Void
 **/

#ifdef ANSI
PRIVATE Void rgSCHDlfsInitUeBp
(
RgSchDlfsCellCb    *dlfsCell,
RgSchDlfsUeCb      *dlfsUe
)
#else
PRIVATE Void rgSCHDlfsInitUeBp (dlfsCell, dlfsUe)
RgSchDlfsCellCb    *dlfsCell;
RgSchDlfsUeCb      *dlfsUe;
#endif
{
   U8              bpIdx             = 0;
   U8              numDlfsSbPerPrdSb = dlfsUe->prdSbSize/dlfsCell->rbgSize;
   U8              numPrdSbPerCell   = ((dlfsCell->totDlBw) +
         (dlfsUe->prdSbSize - 1))/(dlfsUe->prdSbSize);
   U8              numPrdSbPerBp     = numPrdSbPerCell/dlfsCell->numBp;
   U8              numLargeBp        = numPrdSbPerCell % dlfsCell->numBp;
   U8              bpStart           = 0;
   RgSchDlfsUeBpCb *bpCb             = NULLP;
   TRC2(rgSCHDlfsInitUeBp);

   /* Update larger bandwidth parts with 
    * numPrdSbs = cieling(N_DL_RB/prdSbSize/numBps) */
   for (bpIdx = 0; bpIdx < numLargeBp; ++bpIdx)
   {
      bpCb = &(dlfsUe->bpCbs[bpIdx]);
      bpCb->sbStartIdx = bpStart;
      bpCb->sbEndIdx = bpStart + ((numPrdSbPerBp + 1) * numDlfsSbPerPrdSb) - 1;
      bpStart = bpCb->sbEndIdx + 1;
   }
   
   /* Update smaller bandwidth parts with
    * numPrdSbs = cieling(N_DL_RB/prdSbSize/numBps) - 1 */
   for (bpIdx = numLargeBp; bpIdx < dlfsCell->numBp; ++bpIdx)
   {
      bpCb = &(dlfsUe->bpCbs[bpIdx]);
      bpCb->sbStartIdx = bpStart;
      bpCb->sbEndIdx = bpStart + (numPrdSbPerBp * numDlfsSbPerPrdSb) - 1;
      bpStart = bpCb->sbEndIdx + 1;
   }

   /* If the last BP goes beyond the last DLFS sub-band, update 
    * bpCb->sbEndIdx appropriately */
   if ( (bpCb) && (bpCb->sbEndIdx > (dlfsCell->numDlSb - 1)) )
   {
      bpCb->sbEndIdx = dlfsCell->numDlSb - 1;
   }

   RETVOID;
}

/**
 * @brief Free the Bandwidth part information for a UE
 *
 * @details
 *
 *     Function : rgSCHDlfsFreeUeBp
 *
 *     Processing Steps:
 *     - Free the bandwidth parts for the UE.
 *
 *  @param[in]  Inst            instIdx
 *  @param[in]  RgSchDlfsCellCb *dlfsCell
 *  @param[in]  RgSchDlfsUe     *dlfsUe
 *  @return  Void
 **/
#ifdef ANSI
PRIVATE Void rgSCHDlfsFreeUeBp
(
Inst               instIdx,
RgSchDlfsCellCb    *dlfsCell,
RgSchDlfsUeCb      *dlfsUe
)
#else
PRIVATE Void rgSCHDlfsFreeUeBp (instIdx, dlfsCell, dlfsUe)
Inst               instIdx;
RgSchDlfsCellCb    *dlfsCell;
RgSchDlfsUeCb      *dlfsUe;
#endif
{
   TRC2(rgSCHDlfsFreeUeBp);
   /* ccpu00117052 - MOD - Passing double pointer
   for proper NULLP assignment*/
   rgSCHUtlFreeSBuf(instIdx, (Data**)(&(dlfsUe->bpCbs)), 
         (dlfsCell->numBp * sizeof(RgSchDlfsUeBpCb)));

   RETVOID;
}

/**
 * @brief Initializes the subframe for DLFS
 *
 * @details
 *
 *     Function : rgSCHDlfsInitPrdSBModes
 *
 *     Processing Steps:
 *
 *  @param[in]  Inst              instIdx
 *  @param[in]  RgSchDlfsCellCb   *dlfsCell
 *  @param[in]  RgSchDlfsUeCb     *dlfsUe
 *  @return  S16
 *      -# ROK
 *      -# RFAILED
 **/

#ifdef ANSI
PRIVATE S16 rgSCHDlfsInitPrdSBModes
(
Inst                   instIdx,
RgSchDlfsCellCb        *dlfsCell,
RgSchDlfsUeCb          *dlfsUe,
RgrUePrdDlCqiCfg       *prdCqiCfg
)
#else
PRIVATE S16 rgSCHDlfsInitPrdSBModes(instIdx, dlfsCell, dlfsUe, prdCqiCfg)
Inst                   instIdx;
RgSchDlfsCellCb        *dlfsCell;
RgSchDlfsUeCb          *dlfsUe;
RgrUePrdDlCqiCfg       *prdCqiCfg;
#endif
{
   S16                 ret = ROK;
   TRC2(rgSCHDlfsInitPrdSBModes);

   /* Initialize periodic sub-band size for the UE */
   RG_SCH_DLFS_GET_PRD_SBSIZE(&(dlfsUe->prdSbSize), 
         dlfsCell->totDlBw);
#ifndef TFU_UPGRADE
   /* Update k and I_cqi_pmi value */
   dlfsUe->k           = prdCqiCfg->k;

   /* Get periodicity value */
   RG_SCH_DLFS_GET_PRDICITY_AND_OFFST(prdCqiCfg->cqiPmiCfgIdx,
         &dlfsUe->periodicity, &dlfsUe->cqiRptOffst);
#else
   /* Update k and I_cqi_pmi value */
   dlfsUe->k           = prdCqiCfg->cqiSetup.k;

   /* Get periodicity value */
   RG_SCH_DLFS_GET_PRDICITY_AND_OFFST(prdCqiCfg->cqiSetup.cqiPCfgIdx,
         &dlfsUe->periodicity, &dlfsUe->cqiRptOffst);
#endif
   /*if for ccpu00143880  */
   if (dlfsUe->bpCbs)
   {
      /* Reconfig case */
      RETVALUE(ROK);
   }
   if (dlfsCell->numBp)
   {
      if((ret = rgSCHUtlAllocSBuf(instIdx, 
                  (Data**)&(dlfsUe->bpCbs), 
                  dlfsCell->numBp * (sizeof(RgSchDlfsUeBpCb)))) 
            != ROK)
      {
         RLOG_ARG0(L_ERROR,DBG_INSTID,instIdx,
                  "Allocation for bandwidth parts failed");
         RETVALUE(ret);
      }
      /* Initialize bandwidth part of the UE */
      rgSCHDlfsInitUeBp(dlfsCell, dlfsUe);
   }
   RETVALUE(ret);
}

/**
 * @brief Resets all DL SBs in a SF as available 
 *
 * @details
 *
 *     Function : RGsCHDlfsResetSfSb
 *
 *     Processing Steps:
 *      - Initializes DL SBs for the subframe.
 *
 *  @param[in]  RgSchDlfsCellCb   *dlfsCell
 *  @param[in]  RgSchDlfsSf       *dlfsSf
 *  @return  Void
 **/
#ifdef ANSI
PRIVATE Void rgSCHDlfsResetSfSb
(
RgSchDlfsCellCb  *dlfsCell,
RgSchDlfsSf      *dlfsSf
)
#else
PRIVATE Void rgSCHDlfsResetSfSb(dlfsCell, dlfsSf)
RgSchDlfsCellCb  *dlfsCell;
RgSchDlfsSf      *dlfsSf;
#endif
{
   U32            sbIdx;
   TRC2(rgSCHDlfsResetSfSb);

   for (sbIdx = 0; sbIdx < dlfsCell->numDlSb; ++sbIdx)
   {
      dlfsSf->dlSb[sbIdx].isAvail = TRUE;
   }
   dlfsSf->numAvailSb = dlfsCell->numDlSb;

   RETVOID;
}

/**
 * @brief Initializes the sub-bands for the subframe
 *
 * @details
 *
 *     Function : rgSCHDlfsInitSfSbs
 *
 *     Processing Steps:
 *      - Initializes DL sub-bands for the subframe.
 *
 *  @param[in]  RgSchDlfsCellCb *dlfsCell
 *  @param[in]  RgSchDlfsSf     *dlfsSf
 *  @return  Void
 **/
#ifdef ANSI
PRIVATE Void rgSCHDlfsInitSfSbs
(
RgSchDlfsCellCb *dlfsCell,
RgSchDlfsSf     *dlfsSf
)
#else
PRIVATE Void rgSCHDlfsInitSfSbs (dlfsCell, dlfsSf)
RgSchDlfsCellCb *dlfsCell;
RgSchDlfsSf     *dlfsSf;
#endif
{
   U8           sbIdx = 0;
   TRC2(rgSCHDlfsInitSfSbs);

   for (sbIdx = 0; sbIdx < dlfsCell->numDlSb - 1; ++sbIdx)
   {
      dlfsSf->dlSb[sbIdx].start = sbIdx * dlfsCell->rbgSize;
      dlfsSf->dlSb[sbIdx].sbIdx = sbIdx;
      dlfsSf->dlSb[sbIdx].sbSize = dlfsCell->rbgSize;
   }

   /* Initialize the last sub-band */
   dlfsSf->dlSb[sbIdx].start = sbIdx * dlfsCell->rbgSize;
   dlfsSf->dlSb[sbIdx].sbSize = (dlfsCell->totDlBw % dlfsCell->rbgSize) ? 
      dlfsCell->totDlBw - (dlfsCell->rbgSize * 
            (dlfsCell->totDlBw/dlfsCell->rbgSize)):dlfsCell->rbgSize;
   dlfsSf->dlSb[sbIdx].sbIdx = sbIdx;

   RETVOID;
}

/**
 * @brief Mark the RBGs which are already allocated for
 *        common channels and SPS.
 *
 * @details
 *
 *     Function : rgSCHDlfsMarkAllocdRbgs
 *
 *     Processing Steps: 
 *     - CMN and SPS allocations
 *       are spread from PRB=0 to PRB=dlfsSf->allocDlBw.
 *     - There could be some free RBGs within the reserved
 *       SPS PRBs.
 *     - Considering the above mark the unavailable RBGs.
 *
 *  @param[in]  RgSchCellCb    *cell
 *  @param[in]  RgSchDlSf      *sf
 *  @return  Void
 **/

#ifdef ANSI
PRIVATE Void rgSCHDlfsMarkAllocdRbgs
(
RgSchCellCb    *cell,
RgSchDlSf      *sf
)
#else
PRIVATE Void rgSCHDlfsMarkAllocdRbgs(cell, sf)
RgSchCellCb    *cell;
RgSchDlSf      *sf;
#endif
{
   RgSchDlfsCellCb    *dlfsCell = RG_SCH_DLFS_GET_CELL(cell);
   RgSchDlfsSf        *dlfsSf = RG_SCH_DLFS_GET_SF(sf);
#ifdef LTEMAC_SPS
   RgSchDlSfAllocInfo *dlSfAlloc = &sf->dlSfAllocInfo;
#endif
   U32                numAllocdRbgs = 0;
   U32                sbIdx = 0;
   TRC2(rgSCHDlfsMarkAllocdRbgs);

#ifdef LTEMAC_SPS
   /* Number of RBGs comprising allocDlBw */
   numAllocdRbgs = (cell->spsCellCfg.maxSpsDlBw + 
         (dlfsCell->rbgSize - 1))/dlfsCell->rbgSize;
   
   for (sbIdx = 0; sbIdx < numAllocdRbgs; ++sbIdx)
   {
      /* Set the sub-bands as used: assuming that the allocation for common
       * channels starts from RB 0 */
      U32 spsRbgMask = 1 << RG_SCH_CMN_DL_GET_POS_FRM_LSB(sbIdx);
      /* Check if this RBG is allocated from SPS BW */
      if ((dlSfAlloc->raType0Mask & spsRbgMask))
      {
         dlfsSf->dlSb[sbIdx].isAvail = FALSE;
         dlfsSf->numAvailSb--;
      }
   }
#endif

   numAllocdRbgs = (dlfsSf->allocDlBw + (dlfsCell->rbgSize - 1))
                   /dlfsCell->rbgSize;
   for (; sbIdx < numAllocdRbgs; ++sbIdx)
   {
      dlfsSf->dlSb[sbIdx].isAvail = FALSE;
      dlfsSf->numAvailSb--;
   }
   
   RETVOID;
}

#ifdef RGR_V1
/**
 * @brief Performs RB allocation for MSG4 list
 * cell
 *
 * @details
 *
 *     Function : rgSCHDlfsCcchSduAlloc
 *
 *     Processing Steps:
 *      - Perform allocations in consecutive holes.
 *
 *  @param[in]      RgSchCellCb         *cell
 *  @param[in, out] RgSchCmnDlRbAllocInfo *allocInfo
 *  @param[in]      U8                    isRetx
 *  @return  S16
 *      -# ROK
 *      -# RFAILED
 **/
#ifdef ANSI
PRIVATE S16 rgSCHDlfsCcchSduAlloc
(
RgSchCellCb           *cell,
RgSchCmnDlRbAllocInfo *allocInfo,
U8                    isRetx
)
#else
PRIVATE S16 rgSCHDlfsCcchSduAlloc(cell, allocInfo, isRetx) 
RgSchCellCb           *cell;
RgSchCmnDlRbAllocInfo *allocInfo;
U8                    isRetx;
#endif
{
   S16                ret            = ROK;
   CmLList            *ccchSduAllocNode = NULLP;
   CmLListCp          *ccchSduLst;
   CmLListCp          *allocLst;
   CmLListCp          *nonAllocLst;
   RgSchDlRbAlloc     *reqAllocInfo;
   RgSchDlHqProcCb    *hqP = NULLP;
   TRC2(rgSCHDlfsCcchSduAlloc);

   if (isRetx)
   {
      ccchSduLst = &(allocInfo->ccchSduAlloc.ccchSduRetxLst);
      allocLst = &(allocInfo->ccchSduAlloc.schdCcchSduRetxLst);
      nonAllocLst = &(allocInfo->ccchSduAlloc.nonSchdCcchSduRetxLst);
   }
   else
   {
      ccchSduLst = &(allocInfo->ccchSduAlloc.ccchSduTxLst);
      allocLst = &(allocInfo->ccchSduAlloc.schdCcchSduTxLst);
      nonAllocLst = &(allocInfo->ccchSduAlloc.nonSchdCcchSduTxLst);
   }
   

   ccchSduAllocNode = cmLListFirst(ccchSduLst);
   while (ccchSduAllocNode)
   {
      hqP = ((RgSchDlHqProcCb *)(ccchSduAllocNode->node));      
      reqAllocInfo = RG_SCH_CMN_GET_ALLOCCB_FRM_UE
         (hqP->hqE->ue);
      RG_SCH_CMN_INIT_SCHD_LNK(&(hqP->schdLstLnk), hqP);
      reqAllocInfo->dlSf = allocInfo->ccchSduAlloc.ccchSduDlSf;
      if ((ret = rgSCHDlfsCmnRbAlloc(cell, reqAllocInfo)) != ROK)
      {
         break;
      }
      cmLListAdd2Tail(allocLst, &(hqP->schdLstLnk));
      ccchSduAllocNode = ccchSduAllocNode->next;
   }

   /* Add the remaining ones to non-scheduled list */
   while (ccchSduAllocNode)
   { 
      hqP = ((RgSchDlHqProcCb *)(ccchSduAllocNode->node));      
      RG_SCH_CMN_INIT_SCHD_LNK(&(hqP->schdLstLnk), hqP);
      cmLListAdd2Tail(nonAllocLst, &(hqP->schdLstLnk));
      ccchSduAllocNode = ccchSduAllocNode->next;
   }

   RETVALUE(ret);
}
#endif
/**
 * @brief Performs RB allocation for MSG4 list
 * cell
 *
 * @details
 *
 *     Function : rgSCHDlfsMsg4Alloc
 *
 *     Processing Steps:
 *      - Perform allocations in consecutive holes.
 *
 *  @param[in]      RgSchCellCb         *cell
 *  @param[in, out] RgSchCmnDlRbAllocInfo *allocInfo
 *  @param[in]      U8                    isRetx
 *  @return  S16
 *      -# ROK
 *      -# RFAILED
 **/
#ifdef ANSI
PRIVATE S16 rgSCHDlfsMsg4Alloc
(
RgSchCellCb           *cell,
RgSchCmnDlRbAllocInfo *allocInfo,
U8                    isRetx
)
#else
PRIVATE S16 rgSCHDlfsMsg4Alloc(cell, allocInfo, isRetx) 
RgSchCellCb           *cell;
RgSchCmnDlRbAllocInfo *allocInfo;
U8                    isRetx;
#endif
{
   S16                ret            = ROK;
   CmLList            *msg4AllocNode = NULLP;
   CmLListCp          *msg4Lst;
   CmLListCp          *allocLst;
   CmLListCp          *nonAllocLst;
   RgSchDlRbAlloc     *reqAllocInfo;
   RgSchDlHqProcCb    *hqP = NULLP;
   TRC2(rgSCHDlfsMsg4Alloc);

   if (isRetx)
   {
      msg4Lst = &(allocInfo->msg4Alloc.msg4RetxLst);
      allocLst = &(allocInfo->msg4Alloc.schdMsg4RetxLst);
      nonAllocLst = &(allocInfo->msg4Alloc.nonSchdMsg4RetxLst);
   }
   else
   {
      msg4Lst = &(allocInfo->msg4Alloc.msg4TxLst);
      allocLst = &(allocInfo->msg4Alloc.schdMsg4TxLst);
      nonAllocLst = &(allocInfo->msg4Alloc.nonSchdMsg4TxLst);
   }
   

   msg4AllocNode = cmLListFirst(msg4Lst);
   while (msg4AllocNode)
   {
      hqP = (RgSchDlHqProcCb *)(msg4AllocNode->node);      
      reqAllocInfo = RG_SCH_CMN_GET_ALLOCCB_FRM_RACB
         (hqP->hqE->raCb);
      RG_SCH_CMN_INIT_SCHD_LNK(&(hqP->schdLstLnk), hqP);
      reqAllocInfo->dlSf = allocInfo->msg4Alloc.msg4DlSf;
      if ((ret = rgSCHDlfsCmnRbAlloc(cell, reqAllocInfo)) != ROK)
      {
         break;
      }
      cmLListAdd2Tail(allocLst, &(hqP->schdLstLnk));
      msg4AllocNode = msg4AllocNode->next;
   }

   /* Add the remaining ones to non-scheduled list */
   while (msg4AllocNode)
   { 
      hqP = (RgSchDlHqProcCb *)(msg4AllocNode->node);      
      RG_SCH_CMN_INIT_SCHD_LNK(&(hqP->schdLstLnk), hqP);
      cmLListAdd2Tail(nonAllocLst, &(hqP->schdLstLnk));
      msg4AllocNode = msg4AllocNode->next;
   }

   RETVALUE(ret);
}

/**
 * @brief Performs RB allocation for common channels for a frequency selective
 * cell
 *
 * @details
 *
 *     Function : rgSCHDlfsCmnRbAlloc
 *
 *     Processing Steps:
 *      - If SPS is enabled, attempt to allocate icompletely from 
 *        left over SPS DL BW.
 *      - If unable to allocate from SPS BW then attempt to allocate
 *        completely from the rest of the DL BW.
 *      
 *
 *  @param[in]      RgSchCellCb *cell
 *  @param[in, out] RgSchDlRbAlloc  *cmnAllocInfo
 *  @return  S16
 *      -# ROK
 *      -# RFAILED
 **/
#ifdef ANSI
PRIVATE S16 rgSCHDlfsCmnRbAlloc
(
RgSchCellCb     *cell,
RgSchDlRbAlloc  *cmnAllocInfo
)
#else
PRIVATE S16 rgSCHDlfsCmnRbAlloc(cell, cmnAllocInfo)
RgSchCellCb     *cell;
RgSchDlRbAlloc  *cmnAllocInfo;
#endif
{
   U8                  rbStart;
   U8                  rbsAlloc = 0;
#ifdef LTEMAC_SPS
   U8                  spsRbsAlloc = 0;
   RgSchDlSfAllocInfo  *dlSfAlloc = &cmnAllocInfo->dlSf->dlSfAllocInfo;
#endif
   RgSchDlfsSf *dlfsSf = RG_SCH_DLFS_GET_SF(cmnAllocInfo->dlSf);
   TRC2(rgSCHDlfsCmnRbAlloc);

#ifdef LTEMAC_SPS
   /* Attempt RA Type 2 allocation in SPS Bandwidth */
   spsRbsAlloc = 
      rgSCHCmnDlRaType2Alloc(dlSfAlloc,
            cmnAllocInfo->rbsReq, &cell->spsBwRbgInfo, &rbStart, 
            &cmnAllocInfo->resAllocInfo, FALSE);
   if (spsRbsAlloc)
   {
      rbsAlloc = spsRbsAlloc;
   }
   else
   {
      if (cmnAllocInfo->rbsReq > dlfsSf->availDlBw)
      {
         RLOG_ARG1(L_ERROR,DBG_CELLID,cell->cellId, "DL BW exhausted for cell:" 
                  " CRNTI:%d", cmnAllocInfo->rnti);
         RETVALUE(RFAILED);
      }
      else
      {
         rbStart = dlfsSf->allocDlBw;
         rbsAlloc = cmnAllocInfo->rbsReq;
      }
   }
#else
   if (cmnAllocInfo->rbsReq > dlfsSf->availDlBw)
   {
      RLOG_ARG1(L_ERROR,DBG_CELLID,cell->cellId, "DL BW exhausted for cell:" 
               " CRNTI:%d",cmnAllocInfo->rnti);
      RETVALUE(RFAILED);
   }
   else
   {
      rbStart = dlfsSf->allocDlBw;
      rbsAlloc = cmnAllocInfo->rbsReq;
   }
#endif

   /* Update allocation information */
   /* Retrieve PDCCH */
   cmnAllocInfo->pdcch = rgSCHCmnCmnPdcchAlloc(cell, cmnAllocInfo->dlSf);
   if (cmnAllocInfo->pdcch == NULLP)
   {
      RLOG_ARG1(L_ERROR,DBG_CELLID,cell->cellId, "Commom PDCCH exhausted for cell:"
               "CRNTI:%d",cmnAllocInfo->rnti);
      RETVALUE(RFAILED);
   }

   cmnAllocInfo->dciFormat = TFU_DCI_FORMAT_1A;
   cmnAllocInfo->raType = RG_SCH_CMN_RA_TYPE2;
   cmnAllocInfo->allocInfo.raType2.isLocal = TRUE;
   cmnAllocInfo->allocInfo.raType2.rbStart = rbStart;
   cmnAllocInfo->allocInfo.raType2.numRb = rbsAlloc;
   cmnAllocInfo->rbsAlloc = rbsAlloc;
   cmnAllocInfo->tbInfo[0].bytesAlloc = cmnAllocInfo->tbInfo[0].bytesReq;

#ifdef LTEMAC_SPS
   if (spsRbsAlloc)
   {
      U8    idx;
      /* Update type 0, 1 and 2 masks in DL SF */
      dlSfAlloc->raType0Mask    |= cmnAllocInfo->resAllocInfo.raType0Mask;
#ifdef RGSCH_SPS_UNUSED
      for (idx = 0; idx < RG_SCH_NUM_RATYPE1_32BIT_MASK; ++idx)
      {
         dlSfAlloc->raType1Mask[idx] |=
            cmnAllocInfo->resAllocInfo.raType1Mask[idx];
         dlSfAlloc->raType1UsedRbs[idx] +=
            cmnAllocInfo->resAllocInfo.raType1UsedRbs[idx];
      }
#endif
      for (idx = 0; idx < RG_SCH_NUM_RATYPE2_32BIT_MASK; ++idx)
      {
         dlSfAlloc->raType2Mask[idx] |=
            cmnAllocInfo->resAllocInfo.raType2Mask[idx];
      }
   }
   else 
   {
      /* Update the sub-frame with new allocation */
      cmnAllocInfo->dlSf->bwAlloced += rbsAlloc;
      dlfsSf->allocDlBw += rbsAlloc;
      dlfsSf->availDlBw -= rbsAlloc;
   }
#else
   /* Update the sub-frame with new allocation */
   cmnAllocInfo->dlSf->bwAlloced += rbsAlloc;
   dlfsSf->allocDlBw += rbsAlloc;
   dlfsSf->availDlBw -= rbsAlloc;
#endif

   RETVALUE(ROK);
}

/**
 * @brief Performs RB allocation for dedicated channels for a frequency 
 * selective
 * cell for transmitting Ues 
 *
 * @details
 *
 *     Function : rgSCHDlfsTxUeRbAlloc
 *
 *     Processing Steps:
 *     - Assumption: Each UE has one entry in transmitting or re-transmitting
 *       UE list.
 *     - For each UE,
 *         - Perform sub-band allocations for UE. Call rgSCHDlfsSbAllocForTxUe().
 *         - Subbands in prioritized order for a given UE are checked for 
 *           availability and assigned to UE.
 *     - MIMO: Allocate as much as requested by TbInfo[0], tbInfo[1]'s allocation is
 *             a result of tbInfo[0]'s allocation.
 *
 *  @param[in]      RgSchCellCb        *cell
 *  @param[in]      RgSchDlSf          *dlSf,
 *  @param[in]      CmLListCp          *ueLst,
 *  @param[in, out] CmLListCp          *schdUeLst,
 *  @param[in, out] CmLListCp          *nonSchdUeLst
 *  @return  Void
 **/
#ifdef ANSI
PRIVATE Void rgSCHDlfsTxUeRbAlloc
(
RgSchCellCb        *cell,
RgSchDlSf          *dlSf,
CmLListCp          *ueLst,
CmLListCp          *schdUeLst,
CmLListCp          *nonSchdUeLst
)
#else
PRIVATE Void rgSCHDlfsTxUeRbAlloc(cell, dlSf, ueLst, schdUeLst, nonSchdUeLst)
RgSchCellCb        *cell;
RgSchDlSf          *dlSf;
CmLListCp          *ueLst;
CmLListCp          *schdUeLst;
CmLListCp          *nonSchdUeLst;
#endif
{
   CmLList           *lstPtr       = NULLP;
   CmLList           *prevLstPtr   = NULLP;
   RgSchDlRbAlloc    *dlAllocCb    = NULLP;
   RgSchUeCb         *ue           = NULLP;
   RgSchDlfsSf       *dlfsSf       = RG_SCH_DLFS_GET_SF(dlSf);
   RgSchDlHqProcCb    *hqP = NULLP;
   TRC2(rgSCHDlfsTxUeRbAlloc);


   lstPtr = cmLListFirst(ueLst);
   while (dlfsSf->numAvailSb && lstPtr)
   {
      hqP = (RgSchDlHqProcCb *)(lstPtr->node);
      ue = hqP->hqE->ue;
      dlAllocCb = RG_SCH_CMN_GET_ALLOCCB_FRM_UE(ue);

      rgSCHDlfsSbAllocForTxUe(dlSf, cell, ue, dlAllocCb);

      prevLstPtr = lstPtr;
      lstPtr = lstPtr->next;
      /* Remove the UE from the SCH Request List*/
      cmLListDelFrm(ueLst, prevLstPtr);
      if (!dlAllocCb->rbsAlloc)
      {
         /* Add UE to not scheduled UE list */
         cmLListAdd2Tail(nonSchdUeLst, prevLstPtr);
      }
      else
      {
         /* Add UE to scheduled UE list */
         cmLListAdd2Tail(schdUeLst, prevLstPtr);
      }
   }

   /* Add remaining UEs to non-scheduled list */
   while (lstPtr)
   {
      prevLstPtr = lstPtr; 
      lstPtr = lstPtr->next;
      cmLListDelFrm(ueLst, prevLstPtr);
      cmLListAdd2Tail(nonSchdUeLst, prevLstPtr);
   }
   

   RETVOID;
}

/**
 * @brief Performs RB allocation for dedicated channels for a frequency 
 * selective cell for re-transmitting Ues 
 *
 * @details
 *
 *     Function : rgSCHDlfsRetxUeRbAlloc
 *
 *     Processing Steps:
 *     - Assumption: Each UE has one entry in transmitting or re-transmitting
 *       UE list.
 *     - For each UE,
 *         - Perform sub-band allocations for UE. Call rgSCHDlfsSbAllocForRetxUe().
 *         - Subbands in prioritized order for a given UE are checked for 
 *           availability and assigned to UE.
 *         - Partial RB# allocation for RETX is considered as FAILURE.
 *
 *  @param[in]      RgSchCellCb        *cell
 *  @param[in]      RgSchDlSf          *dlSf,
 *  @param[in]      CmLListCp          *ueLst,
 *  @param[in, out] CmLListCp          *schdUeLst,
 *  @param[in, out] CmLListCp          *nonSchdUeLst
 *  @return  Void
 **/
#ifdef ANSI
PRIVATE Void rgSCHDlfsRetxUeRbAlloc
(
RgSchCellCb        *cell,
RgSchDlSf          *dlSf,
CmLListCp          *ueLst,
CmLListCp          *schdUeLst,
CmLListCp          *nonSchdUeLst
)
#else
PRIVATE Void rgSCHDlfsRetxUeRbAlloc(cell, dlSf, ueLst, schdUeLst, nonSchdUeLst)
RgSchCellCb        *cell;
RgSchDlSf          *dlSf;
CmLListCp          *ueLst;
CmLListCp          *schdUeLst;
CmLListCp          *nonSchdUeLst;
#endif
{
   CmLList         *lstPtr       = NULLP;
   CmLList         *prevLstPtr   = NULLP;
   RgSchDlRbAlloc  *dlAllocCb    = NULLP;
   RgSchUeCb       *ue           = NULLP;
   RgSchDlfsSf     *dlfsSf       = RG_SCH_DLFS_GET_SF(dlSf);
   RgSchDlHqProcCb    *hqP = NULLP;
   TRC2(rgSCHDlfsRetxUeRbAlloc);

   /* Initialize re-transmitting lists */


   lstPtr = cmLListFirst(ueLst);
   while (dlfsSf->numAvailSb && lstPtr)
   {
      hqP = (RgSchDlHqProcCb *)(lstPtr->node);
      ue = hqP->hqE->ue;
      dlAllocCb = RG_SCH_CMN_GET_ALLOCCB_FRM_UE(ue);
      rgSCHDlfsSbAllocForRetxUe(dlSf, cell, ue, dlAllocCb);
      prevLstPtr = lstPtr;
      lstPtr = lstPtr->next;
      /* Remove the UE from the SCH Request List*/
      cmLListDelFrm(ueLst, prevLstPtr);
      if (!dlAllocCb->rbsAlloc)
      {
         /* Add UE to not scheduled UE list */
         cmLListAdd2Tail(nonSchdUeLst, prevLstPtr);
      }
      else
      {
         /* Add UE to scheduled UE list */
         cmLListAdd2Tail(schdUeLst, prevLstPtr);
      }
   }

   /* Add remaining UEs to non-scheduled list */
   while (lstPtr)
   {
      prevLstPtr = lstPtr; 
      lstPtr = lstPtr->next;
      cmLListDelFrm(ueLst, prevLstPtr);
      cmLListAdd2Tail(nonSchdUeLst, prevLstPtr);
   }


   RETVOID;
}

/**
 * @brief Updates the reported Wideband CQI for the UE.
 *
 * @details
 *
 *     Function : rgSCHDlfsUpdWbCqi
 *
 *     Processing Steps:
 *      - Update the wideband CQI value for the UE.
 *
 *  @param[in]  RgSchDlfsUeCb   *dlfsUe
 *  @param[in]  U8              wbCqi
 *  @return  Void
 **/
#ifdef ANSI
PRIVATE Void rgSCHDlfsUpdWbCqi
(
RgSchDlfsCellCb *dlfsCell,
RgSchDlfsUeCb   *dlfsUe,
U8              wbCqi
)
#else
PRIVATE Void rgSCHDlfsUpdWbCqi(dlfsCell, dlfsUe, wbCqi)
RgSchDlfsCellCb   *dlfsCell;
RgSchDlfsUeCb     *dlfsUe;
U8                wbCqi;
#endif
{
   U32           sbCqiIdx = 0; 
   U32           numSbCqi = dlfsCell->numDlSb;
   TRC2(rgSCHDlfsUpdWbCqi);
   
   dlfsUe->wbCqi = wbCqi;

   if (dlfsUe->aprdSbSize)
   {
      /* If this UE is configured to report Aperiodic SB CQI
         Reports then do not reset with WB CQI */
      RETVOID;
   }

   /* Update the first entry */
   dlfsUe->sbCqiLst[0].cqi = wbCqi; 
   dlfsUe->sbCqiLst[0].sbIdx = 0;
   dlfsUe->sbCqiLst[0].next = &(dlfsUe->sbCqiLst[1]);
   dlfsUe->sbCqiLst[0].prev = NULLP;

   for (sbCqiIdx = 1; sbCqiIdx < (numSbCqi - 1); sbCqiIdx++)
   {
      dlfsUe->sbCqiLst[sbCqiIdx].cqi = wbCqi; 
      dlfsUe->sbCqiLst[sbCqiIdx].sbIdx = sbCqiIdx;
      dlfsUe->sbCqiLst[sbCqiIdx].next = &(dlfsUe->sbCqiLst[sbCqiIdx+1]);
      dlfsUe->sbCqiLst[sbCqiIdx].prev = &(dlfsUe->sbCqiLst[sbCqiIdx-1]);
   }
   
   /* Update the last entry */
   dlfsUe->sbCqiLst[sbCqiIdx].cqi = wbCqi; 
   dlfsUe->sbCqiLst[sbCqiIdx].sbIdx = sbCqiIdx;
   dlfsUe->sbCqiLst[sbCqiIdx].next = NULLP;
   dlfsUe->sbCqiLst[sbCqiIdx].prev = &(dlfsUe->sbCqiLst[sbCqiIdx - 1]);

   /* Update the sorted list pointer */
   dlfsUe->sortSbCqiLst = &(dlfsUe->sbCqiLst[0]);
   
   /* Update average CQI and CQI deviation for the UE */
   dlfsUe->avgSbCqiVal = wbCqi*1024;
   dlfsUe->avgCqiDeviation = 0;

   RETVOID;
}

/**
 * @brief Updates the aperiodic CQI report for Mode 2-0
 *
 * @details
 *
 *     Function : rgSCHDlfsUpdAprdMode20SbCqi
 *
 *     Processing Steps:
 *     - Assumption: The reported sub-bands are within the limts of sub-band
 *     size of the periodic reporting mode 2-0.
 *     - Update the sub-band CQIs of the UE with the reported values.
 *
 *  @param[in]  RgSchDlfsCellCb    *dlfsCell
 *  @param[in]  RgSchDlfsUeCb      *dlfsUe
 *  @param[in]  TfuCqiPuschMode20  *mode20CqiInfo
 *  @return  Void
 **/
#ifdef ANSI
PRIVATE Void rgSCHDlfsUpdAprdMode20SbCqi
(
RgSchDlfsCellCb    *dlfsCell,
RgSchDlfsUeCb      *dlfsUe,
TfuCqiPuschMode20  *mode20CqiInfo
)
#else
PRIVATE Void rgSCHDlfsUpdAprdMode20SbCqi(dlfsCell, dlfsUe, mode20CqiInfo)
RgSchDlfsCellCb    *dlfsCell;
RgSchDlfsUeCb      *dlfsUe;
TfuCqiPuschMode20  *mode20CqiInfo;
#endif
{
   U32              sbIdx    = 0;
   U32              startIdx = 0;
   U32              remRbs   = 0;
   U32              sbCqi    = mode20CqiInfo->cqiIdx;
   TRC2(rgSCHDlfsUpdAprdMode20SbCqi);

   if (!dlfsUe->aprdSbSize)
   {
      /* Incorrect report: sub-band report not applicable */
      RETVOID;
   }

   for (sbIdx = 0; sbIdx < mode20CqiInfo->numSubband; ++sbIdx)
   {
      startIdx = 
         ((mode20CqiInfo->subbandArr[sbIdx].rbStart)/dlfsCell->rbgSize);
      remRbs = mode20CqiInfo->subbandArr[sbIdx].numRb;
      while (remRbs >= dlfsCell->rbgSize)
      {
         rgSCHDlfsPrcSbCqiUpd(dlfsCell, dlfsUe, &dlfsUe->sbCqiLst[startIdx], sbCqi);
         remRbs -= dlfsCell->rbgSize;
         startIdx += 1; 
      }
      /* Updation for the last small-sized sub-band */
      if (remRbs)
      {
         rgSCHDlfsPrcSbCqiUpd(dlfsCell, dlfsUe, &dlfsUe->sbCqiLst[startIdx], sbCqi);
      }
   }

   RETVOID;
}


/**
 * @brief Updates the aperiodic CQI report for Mode 3-1
 *
 * @details
 *
 *     Function : rgSCHDlfsUpdAprdMode31SbCqi
 *
 *     Processing Steps:
 *     - Assumption: The reported sub-bands are within the limts of sub-band
 *     size of the aperiodic reporting mode 3-1.
 *     - Update the sub-band CQIs of the UE with the reported values.
 *
 *  @param[in]  RgSchDlfsCellCb    *dlfsCell
 *  @param[in]  RgSchDlfsUeCb      *dlfsUe
 *  @param[in]  TfuCqiPuschMode31  *mode31CqiInfo
 *  @return  Void
 **/
#ifdef ANSI
PRIVATE Void rgSCHDlfsUpdAprdMode31SbCqi
(
RgSchDlfsCellCb     *dlfsCell,
RgSchDlfsUeCb       *dlfsUe,
TfuCqiPuschMode31   *mode31CqiInfo
)
#else
PRIVATE Void rgSCHDlfsUpdAprdMode31SbCqi(dlfsCell, dlfsUe, mode31CqiInfo)
RgSchDlfsCellCb     *dlfsCell;
RgSchDlfsUeCb       *dlfsUe;
TfuCqiPuschMode31   *mode31CqiInfo;
#endif
{
   U32               sbIdx      = 0;
   U32               startIdx   = 0;
   U32               remRbs     = 0;
   U32               sbCqi;
   TfuSubbandDlCqiInfo *sbCqiInfo = NULLP;
   TRC2(rgSCHDlfsUpdAprdMode31SbCqi);

   if (!dlfsUe->aprdSbSize)
   {
      /* Incorrect report: sub-band report not applicable */
      RETVOID;
   }

   for (sbIdx = 0; sbIdx < mode31CqiInfo->numSubband; ++sbIdx)
   {
      sbCqiInfo = &(mode31CqiInfo->subbandCqiArr[sbIdx]);
      startIdx = ((sbCqiInfo->subband.rbStart)/dlfsCell->rbgSize);
      remRbs = sbCqiInfo->subband.numRb;
      sbCqi = sbCqiInfo->cqiIdx[0];
      RG_SCH_DLFS_WEIGH_AVG_SBCQI(sbCqi,sbCqiInfo->cqiIdx[1]); 
      while (remRbs >= dlfsCell->rbgSize)
      {
         rgSCHDlfsPrcSbCqiUpd(dlfsCell, dlfsUe, &dlfsUe->sbCqiLst[startIdx], sbCqi);
         remRbs -= dlfsCell->rbgSize;
         startIdx += 1; 
      }
      /* Updation for the last small-sized sub-band */
      if (remRbs)
      {
         rgSCHDlfsPrcSbCqiUpd(dlfsCell, dlfsUe, &dlfsUe->sbCqiLst[startIdx], sbCqi);
      }
   }

   RETVOID;
}

/**
 * @brief Updates the aperiodic CQI report for Mode 2-2 
 *
 * @details
 *
 *     Function : rgSCHDlfsUpdAprdMode22SbCqi
 *
 *     Processing Steps:
 *     - Assumption: The reported sub-bands are within the limts of sub-band
 *     size of the aperiodic reporting mode 22.
 *     - Update the sub-band CQIs of the UE with the reported values.
 *
 *  @param[in]  RgSchDlfsCellCb    *dlfsCell
 *  @param[in]  RgSchDlfsUeCb      *dlfsUe
 *  @param[in]  TfuCqiPuschMode22  *mode22CqiInfo
 *  @return  Void
 **/
#ifdef ANSI
PRIVATE Void rgSCHDlfsUpdAprdMode22SbCqi
(
RgSchDlfsCellCb     *dlfsCell,
RgSchDlfsUeCb       *dlfsUe,
TfuCqiPuschMode22   *mode22CqiInfo
)
#else
PRIVATE Void rgSCHDlfsUpdAprdMode22SbCqi(dlfsCell, dlfsUe, mode22CqiInfo)
RgSchDlfsCellCb     *dlfsCell;
RgSchDlfsUeCb       *dlfsUe;
TfuCqiPuschMode22   *mode22CqiInfo;
#endif
{
   U32               sbIdx      = 0;
   U32               startIdx   = 0;
   U32               remRbs     = 0;
   U32               sbCqi;
   TfuSubbandInfo   *sbCqiInfo = NULLP;
   TRC2(rgSCHDlfsUpdAprdMode22SbCqi);

   if (!dlfsUe->aprdSbSize)
   {
      /* Incorrect report: sub-band report not applicable */
      RETVOID;
   }

   /* subband CQI is considered as the average of the two CW
    * CQIs reported over M-preferred subbands. */
   sbCqi = (mode22CqiInfo->cqi[0] + mode22CqiInfo->cqi[1])/2;
   for (sbIdx = 0; sbIdx < mode22CqiInfo->numSubband; ++sbIdx)
   {
      sbCqiInfo =(TfuSubbandInfo*) &(mode22CqiInfo->subbandArr[sbIdx]);
      startIdx = ((sbCqiInfo->rbStart)/dlfsCell->rbgSize);
      remRbs = sbCqiInfo->numRb;
      while (remRbs >= dlfsCell->rbgSize)
      {
         rgSCHDlfsPrcSbCqiUpd(dlfsCell, dlfsUe, 
           &dlfsUe->sbCqiLst[startIdx], sbCqi);
         remRbs -= dlfsCell->rbgSize;
         startIdx += 1; 
      }
      /* Updation for the last small-sized sub-band */
      if (remRbs)
      {
         rgSCHDlfsPrcSbCqiUpd(dlfsCell, dlfsUe, 
            &dlfsUe->sbCqiLst[startIdx], sbCqi);
      }
   }

   RETVOID;
}
/**
 * @brief Updates the aperiodic CQI report for Mode 3-0
 *
 * @details
 *
 *     Function : rgSCHDlfsUpdAprdMode30SbCqi
 *
 *     Processing Steps:
 *     - Assumption: The reported sub-bands are within the limts of sub-band
 *     size of the aperiodic reporting mode 3-0.
 *     - Update the sub-band CQIs of the UE with the reported values.
 *
 *  @param[in]  RgSchDlfsCellCb    *dlfsCell
 *  @param[in]  RgSchDlfsUeCb      *dlfsUe
 *  @param[in]  TfuCqiPuschMode30  *mode30CqiInfo
 *  @return  Void
 **/
#ifdef ANSI
PRIVATE Void rgSCHDlfsUpdAprdMode30SbCqi
(
RgSchDlfsCellCb     *dlfsCell,
RgSchDlfsUeCb       *dlfsUe,
TfuCqiPuschMode30   *mode30CqiInfo
)
#else
PRIVATE Void rgSCHDlfsUpdAprdMode30SbCqi(dlfsCell, dlfsUe, mode30CqiInfo)
RgSchDlfsCellCb     *dlfsCell;
RgSchDlfsUeCb       *dlfsUe;
TfuCqiPuschMode30   *mode30CqiInfo;
#endif
{
   U32               sbIdx      = 0;
   U32               startIdx   = 0;
   U32               remRbs     = 0;
   U32               sbCqi;
   TfuSubbandMode30 *sbCqiInfo = NULLP;
   TRC2(rgSCHDlfsUpdAprdMode30SbCqi);

   if (!dlfsUe->aprdSbSize)
   {
      /* Incorrect report: sub-band report not applicable */
      RETVOID;
   }

   for (sbIdx = 0; sbIdx < mode30CqiInfo->numSubband; ++sbIdx)
   {
      sbCqiInfo = &(mode30CqiInfo->subbandArr[sbIdx]);
      startIdx = ((sbCqiInfo->subBand.rbStart)/dlfsCell->rbgSize);
      remRbs = sbCqiInfo->subBand.numRb;
      sbCqi = sbCqiInfo->cqi;
      while (remRbs >= dlfsCell->rbgSize)
      {
         rgSCHDlfsPrcSbCqiUpd(dlfsCell, dlfsUe, 
            &dlfsUe->sbCqiLst[startIdx], sbCqi);
         remRbs -= dlfsCell->rbgSize;
         startIdx += 1; 
      }
      /* Updation for the last small-sized sub-band */
      if (remRbs)
      {
         rgSCHDlfsPrcSbCqiUpd(dlfsCell, dlfsUe, 
            &dlfsUe->sbCqiLst[startIdx], sbCqi);
      }
   }

   RETVOID;
}

/**
 * @brief Updates the periodic CQI sub-band report for Mode 2-0
 *
 * @details
 *
 *     Function : rgSCHDlfsUpdPrdMode20SbCqi
 *
 *     Processing Steps:
 *     - Assumption: The reported sub-bands are within the limts of sub-band
 *     size of the periodic reporting mode 2-0.
 *     - Update the sub-band CQIs of the UE with the reported values.
 *     - Update only the sub-bands of the reported bandwidth part.
 *
 *  @param[in]  RgSchDlfsCellCb    *dlfsCell
 *  @param[in]  RgSchDlfsUeCb      *dlfsUe
 *  @param[in]  TfuCqiPucchMode20  *mode20CqiInfo
 *  @param[in]  CmLteTimingInfo    timingInfo
 *  @return  Void
 **/
#ifdef ANSI
PRIVATE Void rgSCHDlfsUpdPrdMode20SbCqi
(
RgSchDlfsCellCb    *dlfsCell,
RgSchDlfsUeCb      *dlfsUe,
TfuCqiPucchMode20  *mode20CqiInfo,
CmLteTimingInfo    timingInfo
)
#else
PRIVATE Void rgSCHDlfsUpdPrdMode20SbCqi(dlfsCell, dlfsUe, mode20CqiInfo, timingInfo)
RgSchDlfsCellCb    *dlfsCell;
RgSchDlfsUeCb      *dlfsUe;
TfuCqiPucchMode20  *mode20CqiInfo;
CmLteTimingInfo    timingInfo;
#endif
{
   U32              sbLabel;
   U32              rbgCount = 0;
   U32              rbgStartIdx = 0;
   U32              numDlfsSbPerPrdSb = (dlfsUe->prdSbSize/dlfsCell->rbgSize);
   U32              sbCqi             = mode20CqiInfo->u.cqi.u.subCqi.cqi;
   U32              rcvdBpIdx;
   RgSchDlfsUeBpCb *bpCb; 
#ifndef NO_ERRCLS
   S16             ret = ROK;
#endif   
   TRC2(rgSCHDlfsUpdPrdMode20SbCqi);

   if (!dlfsUe->prdSbSize)
   {
      /* Incorrect report: sub-band report not applicable */
      RETVOID;
   }

#if (ERRCLASS & ERRCLS_DEBUG)
   /* Determine the BP index for which the report is recieved */
   ret = rgSCHDlfsCalcBpIdx(timingInfo, dlfsCell, dlfsUe, &rcvdBpIdx);
   
   if ( ret != ROK)
   {
      /* Report recieved at wrong timing */
      RETVOID;
   }
#else 
   /* Determine the BP index for which the report is recieved */
   rgSCHDlfsCalcBpIdx(timingInfo, dlfsCell, dlfsUe, &rcvdBpIdx);
#endif 

   sbLabel = mode20CqiInfo->u.cqi.u.subCqi.l;
   /* Update the value for reported BP */
   bpCb = &(dlfsUe->bpCbs[rcvdBpIdx]);

   rbgStartIdx = bpCb->sbStartIdx + (sbLabel * numDlfsSbPerPrdSb);

   if(rbgStartIdx > bpCb->sbEndIdx)
   {/* This is invalid label .,hence skipping it */
      RETVOID;
   }
   while (rbgCount < numDlfsSbPerPrdSb)
   {
      rgSCHDlfsPrcSbCqiUpd(dlfsCell, dlfsUe, 
         &dlfsUe->sbCqiLst[rbgStartIdx], sbCqi);
      rbgStartIdx++;
      rbgCount++;
      if (rbgStartIdx == dlfsCell->numDlSb)
      {
         break;
      }

   }

   RETVOID;
}

/**
 * @brief Updates the periodic CQI sub-band report for Mode 2-1
 *
 * @details
 *
 *     Function : rgSCHDlfsUpdPrdMode21SbCqi
 *
 *     Processing Steps:
 *     - Assumption: The reported sub-bands are within the limts of sub-band
 *     size of the periodic reporting mode 2-1.
 *     - Update the sub-band CQIs of the UE with the reported values.
 *     - Update only the sub-bands of the reported bandwidth part.
 *     - In case of 2 CW CQI reporting. Consider the average of both CQIs
 *       as sbCqi.
 *
 *  @param[in]  RgSchDlfsCellCb    *dlfsCell
 *  @param[in]  RgSchDlfsUeCb      *dlfsUe
 *  @param[in]  TfuCqiPucchMode21  *mode21CqiInfo
 *  @param[in]  CmLteTimingInfo    timingInfo
 *  @return  Void
 **/
#ifdef ANSI
PRIVATE Void rgSCHDlfsUpdPrdMode21SbCqi
(
RgSchDlfsCellCb    *dlfsCell,
RgSchDlfsUeCb      *dlfsUe,
TfuCqiPucchMode21  *mode21CqiInfo,
CmLteTimingInfo    timingInfo
)
#else
PRIVATE Void rgSCHDlfsUpdPrdMode21SbCqi(dlfsCell, dlfsUe, mode21CqiInfo, timingInfo)
RgSchDlfsCellCb    *dlfsCell;
RgSchDlfsUeCb      *dlfsUe;
TfuCqiPucchMode21  *mode21CqiInfo;
CmLteTimingInfo    timingInfo;
#endif
{
   U32              sbLabel;
   U32              rbgCount = 0;
   U32              rbgStartIdx = 0;
   U32              numDlfsSbPerPrdSb = (dlfsUe->prdSbSize/dlfsCell->rbgSize);
   U32              sbCqiCw1          = mode21CqiInfo->u.cqi.u.subCqi.cqi;
   U32              sbCqiCw2, avgSbCqi = sbCqiCw1;
   U32              rcvdBpIdx;
   RgSchDlfsUeBpCb *bpCb; 
#ifndef NO_ERRCLS
   S16             ret = ROK;
#endif
   if (!dlfsUe->prdSbSize)
   {
      /* Incorrect report: sub-band report not applicable */
      RETVOID;
   }

   if (mode21CqiInfo->u.cqi.u.subCqi.diffCqi.pres)
   {
      RG_SCH_UPDT_CW2_CQI(sbCqiCw1,sbCqiCw2,mode21CqiInfo->u.cqi.u.subCqi.diffCqi.val);
      RG_SCH_DLFS_WEIGH_AVG_SBCQI(avgSbCqi,sbCqiCw2); 
   }
#if (ERRCLASS & ERRCLS_DEBUG)
   /* Determine the BP index for which the report is recieved */
   ret = rgSCHDlfsCalcBpIdx(timingInfo, dlfsCell, dlfsUe, &rcvdBpIdx);
   
   if ( ret != ROK)
   {
      /* Report recieved at wrong timing */
      RETVOID;
   }
#else 
   /* Determine the BP index for which the report is recieved */
   rgSCHDlfsCalcBpIdx(timingInfo, dlfsCell, dlfsUe, &rcvdBpIdx);
#endif 

   sbLabel = mode21CqiInfo->u.cqi.u.subCqi.l;
   /* Update the value for reported BP */
   bpCb = &(dlfsUe->bpCbs[rcvdBpIdx]);

   rbgStartIdx = bpCb->sbStartIdx + (sbLabel * numDlfsSbPerPrdSb);

   if(rbgStartIdx > bpCb->sbEndIdx)
   {/* This is invalid label .,hence skipping it */
      RETVOID;
   }

   while (rbgCount < numDlfsSbPerPrdSb)
   {
      rgSCHDlfsPrcSbCqiUpd(dlfsCell, dlfsUe, 
         &dlfsUe->sbCqiLst[rbgStartIdx], avgSbCqi);
      rbgStartIdx++;
      rbgCount++;
      if (rbgStartIdx == dlfsCell->numDlSb)
      {
         break;
      }

   }

   RETVOID;
}

/**
 * @brief Updates the periodic CQI wideband report for Mode 2-1
 *
 * @details
 *
 *     Function : rgSCHDlfsUpdPrdMode21WbCqi
 *
 *     Processing Steps:
 *     - Assumption: The reported sub-bands are within the limts of sub-band
 *     size of the periodic reporting mode 2-1.
 *     - Update the sub-band CQIs of the UE with the reported values.
 *     - Update only the sub-bands of the reported bandwidth part.
 *     - In case of 2 CW CQI reporting. Consider the average of both CQIs
 *       as wbCqi.
 *
 *  @param[in]  RgSchDlfsCellCb    *dlfsCell
 *  @param[in]  RgSchDlfsUeCb      *dlfsUe
 *  @param[in]  TfuCqiPucchMode21  *mode21CqiInfo
 *  @return  Void
 **/
#ifdef ANSI
PRIVATE Void rgSCHDlfsUpdPrdMode21WbCqi
(
RgSchDlfsCellCb    *dlfsCell,
RgSchDlfsUeCb      *dlfsUe,
TfuCqiPucchMode21  *mode21CqiInfo
)
#else
PRIVATE Void rgSCHDlfsUpdPrdMode21WbCqi(dlfsCell, dlfsUe, mode21CqiInfo)
RgSchDlfsCellCb    *dlfsCell;
RgSchDlfsUeCb      *dlfsUe;
TfuCqiPucchMode21  *mode21CqiInfo;
#endif
{
   U32  cw1Cqi = mode21CqiInfo->u.cqi.u.wideCqi.cqi;
   U32  cw2Cqi;
   U32  avgCqi = cw1Cqi;

   if (mode21CqiInfo->u.cqi.u.wideCqi.diffCqi.pres)
   {
      RG_SCH_UPDT_CW2_CQI(cw1Cqi,cw2Cqi,mode21CqiInfo->u.cqi.u.wideCqi.diffCqi.val);
      avgCqi = (cw1Cqi + cw2Cqi)/2;
   }
   /* Update wideband CQI for all the bands */
   rgSCHDlfsUpdWbCqi(dlfsCell, dlfsUe, avgCqi);

   RETVOID;
}

/**
 * @brief  RB allocation for the UE
 *
 * @details
 *
 *     Function : rgSCHDlfsAllocSb
 *
 *     Processing Steps:
 *      - RBG allocation for the UE from the hole.
 *      - Decrement dlfsSf->numAvailSb
 *
 *  @param[in]   RgSchDlfsSf     *dlfsSf
 *  @param[in]   RgSchDlfsHole   *hole
 *  @param[out]  U8              *allocIdx 
 *  @param[out]  U8              *numAllocRbs 
 *  
 *  @return  Void
 **/
#ifdef ANSI
PRIVATE Void rgSCHDlfsAllocSb
(
RgSchDlfsSf     *dlfsSf,
RgSchDlfsHole   *hole,
U32             *allocIdx,
U32             *numAllocRbs
)
#else
PRIVATE Void rgSCHDlfsAllocSb(dlfsSf, hole, allocIdx, numAllocRbs)
RgSchDlfsSf     *dlfsSf;
RgSchDlfsHole   *hole;
U32             *allocIdx;
U32             *numAllocRbs;
#endif
{
   TRC2(rgSCHDlfsAllocSb);

   *allocIdx = hole->sbIdx;
   *numAllocRbs = hole->sbSize;
   hole->isAvail = FALSE;
   dlfsSf->numAvailSb--;

   RETVOID;
}

/**
 * @brief RB Assignment for RA TYPE-2
 *
 * @details
 *
 *     Function : rgSCHDlfsType2RbAssgn
 *
 *     Processing Steps:
 *     - Traverse through the available SBs in the
 *       subframe. 
 *     - Assign as many adjacent RBGs as rbsReq.
 *
 *  @param[in]  RgSchDlfsCellCb *dlfsCell,
 *  @param[in]  RgSchDlfsSf     *dlfsSf,
 *  @param[out] RgSchDlRbAlloc  *dlRbAlloc,
 *  @param[out] U32             *totAllocRbs,
 *  @param[out] U32             *pbchOvrlpRbs,
 *  @param[out] U32             *dlAllocBitMask
 *
 *  @return  Void
 **/
#ifdef ANSI
PRIVATE Void rgSCHDlfsType2RbAssgn
(
RgSchDlfsCellCb *dlfsCell,
RgSchDlfsSf     *dlfsSf,
RgSchDlRbAlloc  *dlRbAlloc,
U32             *totAllocRbs,
U32             *pbchOvrlpRbs,
U32             *dlAllocBitMask
)
#else
PRIVATE Void rgSCHDlfsType2RbAssgn(dlfsCell, dlfsSf, dlRbAlloc, 
                totAllocRbs, pbchOvrlpRbs, dlAllocBitMask)
RgSchDlfsCellCb *dlfsCell;
RgSchDlfsSf     *dlfsSf;
RgSchDlRbAlloc  *dlRbAlloc;
U32             *totAllocRbs;
U32             *pbchOvrlpRbs;
U32             *dlAllocBitMask;
#endif
{
   RgSchDlfsSb     *dlSb;
   U32             allocIdx = 0;
   U32             raType2RbStart = 0;
   U32             rbAlloc = 0;

   TRC2(rgSCHDlfsType2RbAssgn)

   /* Get first Available Subband */
   allocIdx = 0;
   while (allocIdx < dlfsCell->numDlSb)
   {
      dlSb = &(dlfsSf->dlSb[allocIdx]);
      if (dlSb->isAvail)
      {
         rgSCHDlfsAllocSb (dlfsSf, dlSb, &allocIdx, &rbAlloc);
         raType2RbStart = dlSb->start;
         *totAllocRbs += rbAlloc;
         *pbchOvrlpRbs += dlSb->numOvrlpPbchRb;
         *dlAllocBitMask |= 1 << (31 - allocIdx); 
         break;
      }
      allocIdx++;
   }
   if (allocIdx == dlfsCell->numDlSb)
   {
      /* Not even one RBG is free */
      RETVOID;
   }
   /* Add as many adjacent RBGs as rbsReq */
   allocIdx++;
   while((*totAllocRbs < dlRbAlloc->rbsReq) &&
         (allocIdx < dlfsCell->numDlSb))
   {
      dlSb = &(dlfsSf->dlSb[allocIdx]);
      if (!dlSb->isAvail)
      {
         /* Adjacent RBG isn't available, stop RB assignment */
         break;
      }
      rgSCHDlfsAllocSb (dlfsSf, dlSb, &allocIdx, &rbAlloc);
      *totAllocRbs += rbAlloc;
      *pbchOvrlpRbs += dlSb->numOvrlpPbchRb;
      *dlAllocBitMask |= 1 << (31 - allocIdx); 
      allocIdx++;
   }

   dlRbAlloc->allocInfo.raType2.isLocal = TRUE;
   dlRbAlloc->allocInfo.raType2.rbStart = raType2RbStart;
   dlRbAlloc->allocInfo.raType2.numRb = *totAllocRbs;

   RETVOID;
}
/**
 * @brief RB Assignment for RA TYPE-0 
 *
 * @details
 *
 *     Function : rgSCHDlfsType0RbAssgn
 *
 *     Processing Steps:
 *     - Traverse through the prioritized list of sub-band CQIs. 
 *       - For each sub-band,
 *         - If available, allocate to the UE.
 *         - else, go to the next sub-band.
 *         - Break if reqRbs are allocated.
 *
 *  @param[in]  RgSchDlfsCellCb *dlfsCell,
 *  @param[in]  RgSchDlfsSf     *dlfsSf,
 *  @param[in]  RgSchUeCb   *ue,
 *  @param[out] RgSchDlRbAlloc  *dlRbAlloc,
 *  @param[out] U32             *totAllocRbs,
 *  @param[out] U32             *pbchOvrlpRbs,
 *  @param[out] U32             *dlAllocBitMask
 *
 *  @return  Void
 **/
#ifdef ANSI
PRIVATE Void rgSCHDlfsType0RbAssgn
(
RgSchDlfsCellCb *dlfsCell,
RgSchDlfsSf     *dlfsSf,
RgSchUeCb       *ue,
RgSchDlRbAlloc  *allocInfo,
U32             *totAllocRbs,
U32             *pbchOvrlpRbs,
U32             *dlAllocBitMask
)
#else
PRIVATE Void rgSCHDlfsType0RbAssgn(dlfsCell, dlfsSf, ue, allocInfo, 
                totAllocRbs, pbchOvrlpRbs, dlAllocBitMask)
RgSchDlfsCellCb *dlfsCell;
RgSchDlfsSf     *dlfsSf;
RgSchUeCb       *ue;
RgSchDlRbAlloc  *allocInfo;
U32             *totAllocRbs;
U32             *pbchOvrlpRbs;
U32             *dlAllocBitMask;
#endif
{
   RgSchDlfsSbCqi  *sbCqiPtr;
   RgSchDlfsSb     *dlSb;
   U32         allocIdx = 0;
   U32         numSbAlloc = 0;
   U32         rbAlloc = 0;
   U32         noRbs = allocInfo->rbsReq;
   U32         noRbgs = 0;
   U32         noLyr;
   U32         iTbs;
   U32         tb1BytesAlloc;
   U32         tb2BytesAlloc;
   RgSchCmnDlUe    *dlUe = RG_SCH_CMN_GET_DL_UE(ue);
   RgSchDlfsUeCb   *dlfsUe   = RG_SCH_DLFS_GET_UE(ue);

   TRC2(rgSCHDlfsType0RbAssgn)

   noRbgs = RGSCH_CEIL(noRbs,dlfsCell->rbgSize);  
   noRbs = noRbgs * dlfsCell->rbgSize;
   if (noRbs > dlfsCell->totDlBw)
   {
      noRbs = dlfsCell->totDlBw;
   }
   iTbs = allocInfo->tbInfo[0].iTbs;
   noLyr = allocInfo->tbInfo[0].noLyr;

   if (allocInfo->tbInfo[0].tbCb->txCntr)
   {
      tb1BytesAlloc = allocInfo->tbInfo[0].bytesReq;
   }
   else
   {
      tb1BytesAlloc = rgTbSzTbl[noLyr - 1][iTbs][noRbs - 1]/8;
   }

   if (allocInfo->tbInfo[1].schdlngForTb)
   {
      iTbs = allocInfo->tbInfo[1].iTbs;
      noLyr = allocInfo->tbInfo[1].noLyr;
      /* Fix for ccpu00123919: For a RETX TB the iTbs is irrelevant
       * RETX TB Size is same as Init TX TB Size */
      if (allocInfo->tbInfo[1].tbCb->txCntr)
      {
        tb2BytesAlloc = allocInfo->tbInfo[1].bytesReq;
      }
      else
      {
        tb2BytesAlloc = rgTbSzTbl[noLyr - 1][iTbs][noRbs - 1]/8;;
      }
      if (( tb1BytesAlloc + tb2BytesAlloc \
               > dlUe->maxTbBits/8) ||
            (tb1BytesAlloc > dlUe->maxTbSz/8) ||
            (tb2BytesAlloc > dlUe->maxTbSz/8) ||
            (noRbs > dlUe->maxRb))
      {
         if (--noRbgs == 0)
         {
            RETVOID;
         }
         noRbs = (noRbgs * dlfsCell->rbgSize);
      }
   }
   else
   {
#if 0
#else
      if ((noRbs > dlUe->maxRb) ||
            (tb1BytesAlloc > dlUe->maxTbSz/8))
#endif
      {
         if (--noRbgs == 0)
         {
            RETVOID;
         }
         noRbs = (noRbgs * dlfsCell->rbgSize);
      }
   }

   sbCqiPtr = dlfsUe->sortSbCqiLst;

   while(dlfsSf->numAvailSb && sbCqiPtr) /* sbCqiPtr observed NULL in FDD */
   {
      dlSb = &(dlfsSf->dlSb[sbCqiPtr->sbIdx]);
      if (dlSb->isAvail)
      {
         /* Allocate the available sub-band */
         rgSCHDlfsAllocSb (dlfsSf, dlSb, &allocIdx, &rbAlloc);
         *totAllocRbs += rbAlloc; 
         *pbchOvrlpRbs += dlSb->numOvrlpPbchRb;
         *dlAllocBitMask |= 1 << (31 - allocIdx); 
         numSbAlloc++;
         if (noRbs <= *totAllocRbs)
         {
            break;
         }
      }
      sbCqiPtr = sbCqiPtr->next;
   }

   allocInfo->allocInfo.raType0.numDlAlloc += numSbAlloc; 
   allocInfo->allocInfo.raType0.dlAllocBitMask |= *dlAllocBitMask;

   RETVOID;
}

/**
 * @brief  RB allocation for the UE based on sub-band CQI
 *
 * @details
 *
 *     Function :rgSCHDlfsSbAllocForTxUe
 *
 *     Processing Steps:
 *     - Traverse through the prioritized list of sub-band CQIs. 
 *       - For each sub-band,
 *         - If available, allocate to the UE.
 *         - else, go to the next sub-band.
 *         - Break if reqRbs are allocated.
 *     - If atleast one SB is allocated successfully,
 *       - Retrieve PDCCH
 *       - Update Allocation information
 *
 *  @param[in]  RgSchDlSf          *dlSf
 *  @param[in]  RgSchCellCb        *cell
 *  @param[in]  RgSchUeCb          *ue
 *  @param[in, out] RgSchDlRbAlloc *dlRbAlloc
 *
 *  @return  Void
 **/
#ifdef ANSI
PRIVATE Void rgSCHDlfsSbAllocForTxUe
(
RgSchDlSf          *dlSf,
RgSchCellCb        *cell,
RgSchUeCb          *ue,
RgSchDlRbAlloc     *dlRbAlloc
)
#else
PRIVATE Void rgSCHDlfsSbAllocForTxUe(dlSf, cell, ue, dlRbAlloc)
RgSchDlSf          *dlSf;
RgSchCellCb        *cell;
RgSchUeCb          *ue;
RgSchDlRbAlloc     *dlRbAlloc;
#endif
{
   U32             totAllocRbs    = 0;
   U32             pbchOvrlpRbs   = 0;
   U32             dlAllocBitMask = 0;
   RgSchCmnDlUe    *dlUe     = RG_SCH_CMN_GET_DL_UE(ue);
   RgSchDlfsCellCb *dlfsCell = RG_SCH_DLFS_GET_CELL(cell);
   RgSchDlfsSf     *dlfsSf   = RG_SCH_DLFS_GET_SF(dlSf);

   TRC2(rgSCHDlfsSbAllocForTxUe);

   if (dlRbAlloc->raType == RG_SCH_CMN_RA_TYPE2)
   {
      rgSCHDlfsType2RbAssgn(dlfsCell, dlfsSf, dlRbAlloc,
              &totAllocRbs, &pbchOvrlpRbs, &dlAllocBitMask);
   }
   else /* TYPE0 */
   {
      rgSCHDlfsType0RbAssgn(dlfsCell, dlfsSf, ue, dlRbAlloc, 
              &totAllocRbs, &pbchOvrlpRbs, &dlAllocBitMask);
   }

   /* Update allocation Info */
   if (totAllocRbs) 
   {
      U32     y = 0;
      U32     iTbs = 0;
      U32     noLyr = 0;

      /* Either dlRbAlloc->tbInfo[0] represents TX TB or both
       * dlRbAlloc->tbInfo[0] and tbInfo[1] represents TX TBs */
      
      /* Retrieve the PDCCH */
      y = rgSCHUtlGetY(ue, dlSf->sfNum);
      dlRbAlloc->pdcch = 
         rgSCHCmnPdcchAlloc(cell, dlSf, y,
               dlUe->mimoInfo.cwInfo[0].cqi, 
               dlRbAlloc->dciFormat, FALSE);
      if (!(dlRbAlloc->pdcch)) 
      {
         rgSCHDlfsRlsAlloc(dlfsCell, dlfsSf, dlAllocBitMask);
         RETVOID;
      }

      /* Update the sub-frame with new allocation */
      dlSf->bwAlloced += totAllocRbs;
      dlfsSf->allocDlBw += totAllocRbs;
      dlfsSf->availDlBw -= totAllocRbs;

      dlRbAlloc->rbsAlloc = totAllocRbs;

      if (pbchOvrlpRbs)
      {
         rgSCHDlfsAdjstITbs(cell, dlfsSf, dlRbAlloc, pbchOvrlpRbs);
      }

      /* Update Tb info for each scheduled TB */
      iTbs = dlRbAlloc->tbInfo[0].iTbs;
      noLyr = dlRbAlloc->tbInfo[0].noLyr;

      dlRbAlloc->tbInfo[0].bytesAlloc =
         rgTbSzTbl[noLyr - 1][iTbs][totAllocRbs - 1]/8;
      /* DwPts Scheduling Changes Start */
#ifdef LTE_TDD
      if (dlSf->sfType == RG_SCH_SPL_SF_DATA)
      {
         dlRbAlloc->tbInfo[0].bytesAlloc =
            rgTbSzTbl[noLyr - 1][iTbs]
                     [RGSCH_MAX(totAllocRbs*3/4,1) - 1]/8;
      }
#endif      
      /* DwPts Scheduling Changes End */

      if (dlRbAlloc->tbInfo[1].schdlngForTb)
      {
         iTbs = dlRbAlloc->tbInfo[1].iTbs;
         noLyr = dlRbAlloc->tbInfo[1].noLyr;
         dlRbAlloc->tbInfo[1].bytesAlloc =
            rgTbSzTbl[noLyr - 1][iTbs][totAllocRbs - 1]/8;;
         /* DwPts Scheduling Changes Start */
#ifdef LTE_TDD
         if (dlSf->sfType == RG_SCH_SPL_SF_DATA)
         {
            dlRbAlloc->tbInfo[1].bytesAlloc =
               rgTbSzTbl[noLyr - 1][iTbs]
                        [RGSCH_MAX(totAllocRbs*3/4,1) - 1]/8;
         }
#endif      
         /* DwPts Scheduling Changes End */
      }
   }

   RETVOID;
}

/**
 * @brief  RB allocation for the UE based on sub-band CQI
 *
 * @details
 *
 *     Function :rgSCHDlfsSbAllocForRetxUe
 *
 *     Processing Steps:
 *     - Traverse through the prioritized list of sub-band CQIs. 
 *       - For each sub-band,
 *         - If available, allocate to the UE.
 *         - else, go to the next sub-band.
 *         - Break if reqRbs are allocated.
 *     - If totalRbs allocated is == rbsReq,
 *       - Retrieve PDCCH
 *       - Update Allocation information
 *     - Else FAIL this RETX allocation.
 *
 *  @param[in]  RgSchDlSf          *dlSf
 *  @param[in]  RgSchCellCb        *cell
 *  @param[in]  RgSchUeCb          *ue
 *  @param[in, out] RgSchDlRbAlloc *dlRbAlloc
 *
 *  @return  Void
 **/
#ifdef ANSI
PRIVATE Void rgSCHDlfsSbAllocForRetxUe
(
RgSchDlSf          *dlSf,
RgSchCellCb        *cell,
RgSchUeCb          *ue,
RgSchDlRbAlloc     *dlRbAlloc
)
#else
PRIVATE Void rgSCHDlfsSbAllocForRetxUe(dlSf, cell, ue, dlRbAlloc)
RgSchDlSf          *dlSf;
RgSchCellCb        *cell;
RgSchUeCb          *ue;
RgSchDlRbAlloc     *dlRbAlloc;
#endif
{
   U32             totAllocRbs    = 0;
   U32             pbchOvrlpRbs   = 0;
   U32             dlAllocBitMask = 0;
   RgSchCmnDlUe    *dlUe     = RG_SCH_CMN_GET_DL_UE(ue);
   RgSchDlfsCellCb *dlfsCell = RG_SCH_DLFS_GET_CELL(cell);
   RgSchDlfsSf     *dlfsSf   = RG_SCH_DLFS_GET_SF(dlSf);

   TRC2(rgSCHDlfsSbAllocForRetxUe);

   if (dlRbAlloc->raType == RG_SCH_CMN_RA_TYPE2)
   {
      rgSCHDlfsType2RbAssgn(dlfsCell, dlfsSf, dlRbAlloc,
              &totAllocRbs, &pbchOvrlpRbs, &dlAllocBitMask);
   }
   else /* TYPE0 */
   {
      rgSCHDlfsType0RbAssgn(dlfsCell, dlfsSf, ue, dlRbAlloc,
              &totAllocRbs, &pbchOvrlpRbs, &dlAllocBitMask);
   }

   /* In case of RETX, partial allocation is
    * regarded as FAILURE. And initial TX TB size
    * should be same as RETX TB size. */
   if (dlRbAlloc->rbsReq != totAllocRbs)
   {
      rgSCHDlfsRlsAlloc(dlfsCell, dlfsSf, dlAllocBitMask);
   }
   else
   {
      U32     y = 0;
      U32     iTbs = 0;
      U32     noLyr = 0;
      Bool    allocForHqTb2Retx = FALSE;
      Bool    allocForHqDtx = FALSE;

      /* Note: Incase of RETX+TX or RETX+RETX, dlRbAlloc->tbInfo[0]
       * always represents the  RETX TB info. dlRbAlloc->tbInfo[1]
       * represents the other TX or RETX TB info respectively. */
      allocForHqDtx = (dlRbAlloc->tbInfo[0].tbCb->isAckNackDtx == 
                       TFU_HQFDB_DTX)? TRUE : FALSE;
      
      /* Retrieve the PDCCH */
      y = rgSCHUtlGetY(ue, dlSf->sfNum);
      dlRbAlloc->pdcch = 
         rgSCHCmnPdcchAlloc(cell, dlSf, y,
               dlUe->mimoInfo.cwInfo[0].cqi, 
               dlRbAlloc->dciFormat, allocForHqDtx);
      if (!(dlRbAlloc->pdcch)) 
      {
         rgSCHDlfsRlsAlloc(dlfsCell, dlfsSf, dlAllocBitMask);
         RETVOID;
      }

      /* Update the sub-frame with new allocation */
      dlSf->bwAlloced += totAllocRbs;
      dlfsSf->allocDlBw += totAllocRbs;
      dlfsSf->availDlBw -= totAllocRbs;

      dlRbAlloc->rbsAlloc = totAllocRbs;

      if (pbchOvrlpRbs)
      {
         rgSCHDlfsAdjstITbs(cell, dlfsSf, dlRbAlloc, pbchOvrlpRbs);
      }

      /* RETX TB Size is same as Init TX TB Size */
      dlRbAlloc->tbInfo[0].bytesAlloc =
         dlRbAlloc->tbInfo[0].bytesReq;

      if (dlRbAlloc->tbInfo[1].schdlngForTb)
      {
         iTbs = dlRbAlloc->tbInfo[1].iTbs;
         noLyr = dlRbAlloc->tbInfo[1].noLyr;
         allocForHqTb2Retx = (dlRbAlloc->tbInfo[1].tbCb->txCntr)? 
                              TRUE : FALSE;
          /* RETX TB Size is same as Init TX TB Size */
         if (allocForHqTb2Retx)
         {
            dlRbAlloc->tbInfo[1].bytesAlloc =
               dlRbAlloc->tbInfo[1].bytesReq;
         }
         else
         {
            dlRbAlloc->tbInfo[1].bytesAlloc =
               rgTbSzTbl[noLyr - 1][iTbs][totAllocRbs - 1]/8;;
            /* DwPts Scheduling Changes Start */
#ifdef LTE_TDD
            if (dlSf->sfType == RG_SCH_SPL_SF_DATA)
            {
               dlRbAlloc->tbInfo[1].bytesAlloc =
                  rgTbSzTbl[noLyr - 1][iTbs]
                           [RGSCH_MAX(totAllocRbs*3/4,1) - 1]/8;
            }
#endif      
            /* DwPts Scheduling Changes End */
         }
      }
   }

   RETVOID;
}

/***********************************************************
 *
 *     Func : rgSCHDlfsCalcBpIdx
 *
 *
 *     Desc : Determines the BP index from the timing info
 *
 *     Ret  : S16
 *            ROK - Success
 *
 *     Notes:
 *
 *     File :
 *
 **********************************************************/
#ifdef ANSI
PRIVATE S16 rgSCHDlfsCalcBpIdx
(
CmLteTimingInfo timingInfo,
RgSchDlfsCellCb *dlfsCell,
RgSchDlfsUeCb   *dlfsUe,
U32             *bpIdx
)
#else
PRIVATE S16 rgSCHDlfsCalcBpIdx(timingInfo, dlfsCell, dlfsUe, bpIdx)
CmLteTimingInfo timingInfo;
RgSchDlfsCellCb *dlfsCell;
RgSchDlfsUeCb   *dlfsUe;
U32             *bpIdx;
#endif
{
   U32 sfNum = (timingInfo.sfn * 10 + timingInfo.subframe);
   U32  hVal   = (dlfsCell->numBp * dlfsUe->k) + 1;
   U32  prdNum = sfNum/dlfsUe->periodicity;
   TRC2(rgSCHDlfsCalcBpIdx);

#if (ERRCLASS & ERRCLS_DEBUG)
   if (!(prdNum % hVal))
   {
      /* Report recieved at wrong timing */
      RETVALUE(RFAILED);
   }
#endif

   *bpIdx = ((prdNum % hVal) - 1) % dlfsCell->numBp;  

   RETVALUE(ROK);
}

/**
 * @brief  Releases allocation made through allocation bitmask
 *
 * @details
 *
 *     Function :rgSCHDlfsRlsAlloc
 *
 *     Processing Steps:
 *     - Create holes at the positions specified by the bitmask
 *
 *  @param[in]  RgSchDlfsCellCb *dlfsCell
 *  @param[in]  RgSchDlfsSf     *dlfsSf
 *  @param[in]  U32             allocBitMask 
 *  @return  Void
 **/
#ifdef ANSI
PRIVATE Void rgSCHDlfsRlsAlloc
(
RgSchDlfsCellCb  *dlfsCell,
RgSchDlfsSf      *dlfsSf,
U32              allocBitMask
)
#else
PRIVATE Void rgSCHDlfsRlsAlloc(dlfsCell, dlfsSf, allocBitMask)
RgSchDlfsCellCb  *dlfsCell;
RgSchDlfsSf      *dlfsSf;
U32              allocBitMask;
#endif
{
   U32 allocIdx = 0;
   TRC2(rgSCHDlfsRlsAlloc);

   while (allocIdx < (dlfsCell->numDlSb))
   {
      if (allocBitMask & (1 << (31 - allocIdx)))
      {
         dlfsSf->dlSb[allocIdx].isAvail = TRUE;
         dlfsSf->numAvailSb++;
      } 
      allocIdx++;
   }
   RETVOID;
}

/**
 * @brief  Frees subframe related information at DLFS
 *
 * @details
 *
 *     Function :rgSCHDlfsFreeSf
 *
 *     Processing Steps:
 *     - Free the sub-bands allocated for the sub-frame.
 *     - Re-initialize the hole list.
 *
 *  @param[in]  RgSchDlfsCellCb *dlfsCell
 *  @param[in]  Inst            instIdx  
 *  @param[in]  RgSchDlfsSf     *dlfsSf
 *  @return  Void
 **/
#ifdef ANSI
PRIVATE Void rgSCHDlfsFreeSf
(
RgSchDlfsCellCb  *dlfsCell,
Inst             instIdx,
RgSchDlfsSf      *dlfsSf
)
#else
PRIVATE Void rgSCHDlfsFreeSf(dlfsCell, instIdx, dlfsSf)
RgSchDlfsCellCb  *dlfsCell,
Inst             instIdx;
RgSchDlfsSf      *dlfsSf;
#endif
{
   TRC2(rgSCHDlfsFreeSf);
   /* Deallocate the sub-bands */
   if (dlfsSf)
   {
      if (dlfsSf->dlSb)
      {
         /* ccpu00117052 - MOD - Passing double pointer
         for proper NULLP assignment*/
         rgSCHUtlFreeSBuf(instIdx, (Data **)(&(dlfsSf->dlSb)),
               (dlfsCell->numDlSb * sizeof(RgSchDlfsSb)));
      }
         /* ccpu00117052 - MOD - Passing double pointer
         for proper NULLP assignment*/
      rgSCHUtlFreeSBuf(instIdx, (Data **)&dlfsSf, sizeof(RgSchDlfsSf));
   }

   RETVOID;
}

/**
 * @brief Orders Scheduling candidates in desc order of
 *        Average CQI Deviation.
 *
 * @details
 *
 *     Function : rgSCHDlfsOdrUeForRbAssgnmt
 *
 *     Invoking Module Processing:
 *     - Specific SCH to perform UE selection.
 *     - CMN SCH to perform resource estimation.
 *     - Then calls this funtion to add in DLFS request Q.
 *       - Prioritize RA Type-2 Allocations First.
 *       - Proritize based on CQI Deviation.
 *        
 *     Processing Steps:
 *
 *  @param[in]  CmLListCp          *lCp
 *  @param[in]  RgSchDlHqProcCb    *hqP
 *  @return  Void
 **/

#ifdef ANSI
PUBLIC Void rgSCHDlfsOdrUeForRbAssgnmt
(
CmLListCp          *lCp, 
RgSchDlHqProcCb    *hqP
)
#else
PUBLIC Void rgSCHDlfsOdrUeForRbAssgnmt(lCp, hqP)
CmLListCp          *lCp;
RgSchDlHqProcCb    *hqP;
#endif
{
   RgSchDlfsUeCb    *dlfsUe = RG_SCH_DLFS_GET_UE(hqP->hqE->ue);
   CmLList          *lstNode = NULLP;
   RgSchDlfsUeCb    *lstDlfsUe = NULLP;
   RgSchDlHqProcCb  *lstHqP = NULLP;
   RgSchDlRbAlloc   *allocInfo;

   TRC2(rgSCHDlfsOdrUeForRbAssgnmt);

   lstNode = cmLListFirst(lCp);
   while(lstNode)
   {
      lstHqP = (RgSchDlHqProcCb *)lstNode->node;
      lstDlfsUe = RG_SCH_DLFS_GET_UE(lstHqP->hqE->ue);
      allocInfo = RG_SCH_CMN_GET_ALLOCCB_FRM_UE(lstHqP->hqE->ue);
      if (allocInfo->raType == RG_SCH_CMN_RA_TYPE2)
      {
         break;
      }
      if (lstDlfsUe->avgCqiDeviation <= dlfsUe->avgCqiDeviation)
      {
         break;
      }
      lstNode = lstNode->next;
   }
   if (lstNode == NULLP)
   {
      /* We have come to the end of the queue, so Append */
      cmLListAdd2Tail(lCp, &hqP->reqLnk);
      hqP->reqLnk.node = (PTR)hqP;
   }
   else
   {
      lCp->crnt = lstNode;
      cmLListInsCrnt(lCp, &hqP->reqLnk);
      hqP->reqLnk.node = (PTR)hqP;
   }
   RETVOID;
}
/**
 * @brief Handles DLFS based RB allocation for BCCH,PCCH
 *
 * @details
 *
 *     Function : rgSCHDlfsBcchPcchRbAlloc
 *
 *     Invoking Module Processing:
 *      - SCH shall invoke this if downlink frequency selective is enabled for
 *        the cell for RB allocation.
 *      - MAX C/I/PFS/RR shall provide the requiredBytes, required RBs
 *        estimate and subframe for each allocation to be made to SCH.
 *        
 *     Processing Steps:
 *     - Allocate for common channels BCCH on DLSCH, PCCH. 
 *       Call  rgSCHDlfsCmnRbAlloc().
 *
 *  @param[in]  RgSchCellCb           *cell
 *  @param[in]  RgSchCmnDlRbAllocInfo *allocInfo
 *  @return  Void
 **/

#ifdef ANSI
PUBLIC Void rgSCHDlfsBcchPcchRbAlloc
(
RgSchCellCb           *cell,
RgSchCmnDlRbAllocInfo *allocInfo
)
#else
PUBLIC Void rgSCHDlfsBcchPcchRbAlloc(cell, allocInfo)
RgSchCellCb           *cell;
RgSchCmnDlRbAllocInfo *allocInfo;
#endif
{
   RgSchDlRbAlloc     *reqAllocInfo;

   TRC2(rgSCHDlfsBcchPcchRbAlloc);

   /* Allocate for PCCH */
   reqAllocInfo = &(allocInfo->pcchAlloc);
   if (reqAllocInfo->rbsReq)
   {
      rgSCHDlfsCmnRbAlloc(cell, reqAllocInfo);
   }

   /* Allocate for BCCH on DLSCH */
   reqAllocInfo = &(allocInfo->bcchAlloc);
   if (reqAllocInfo->rbsReq)
   {
      rgSCHDlfsCmnRbAlloc(cell, reqAllocInfo);
   }

   RETVOID;
}

/**
 * @brief Handles the processing for SB CQI updation. 
 *
 * @details
 *
 *     Function : rgSCHDlfsPrcSbCqiUpd
 *
 *     Invoking Module Processing:
 *      - Identifies which Sub-band CQIs have to be updated and 
 *        calls this funtion with the identified SB and the CQI.
 *        
 *     Processing Steps:
 *     - if current reported CQI is different from the 
 *       previosly reported CQI
 *        - Update the SB CQI
 *        - Call rgSCHDlfsUpdSbCqiDeviation
 *        - Call rgSCHDlfsUpdSbSortedlst
 *     - else return
 *
 *  @param[in]  RgSchDlfsCellCb    *dlfsCell
 *  @param[in]  RgSchDlfsUeCb      *dlfsUe
 *  @param[in]  RgSchDlfsSbCqi     *sbCb
 *  @param[in]  U32                avgSbCqi
 *  @return  Void
 **/

#ifdef ANSI
PUBLIC Void rgSCHDlfsPrcSbCqiUpd
(
RgSchDlfsCellCb    *dlfsCell,
RgSchDlfsUeCb      *dlfsUe,
RgSchDlfsSbCqi     *sbCb,
U32                sbCqi
)
#else
PUBLIC Void rgSCHDlfsPrcSbCqiUpd(dlfsCell, dlfsUe, sbCb, sbCqi)
RgSchDlfsCellCb    *dlfsCell;
RgSchDlfsUeCb      *dlfsUe;
RgSchDlfsSbCqi     *sbCb;
U32                sbCqi;
#endif
{
   U32 prevSbCqi = sbCb->cqi;

   TRC2(rgSCHDlfsPrcSbCqiUpd);

   if (prevSbCqi == sbCqi)
   {
      /* SB CQI remains unchanged, skip procecssing */
      RETVOID;
   }

   sbCb->cqi = sbCqi;
   /* Update Avg CQI and Avg CQI Deviation for the UE */
   rgSCHDlfsUpdSbCqiDeviation(dlfsCell, dlfsUe, sbCb, prevSbCqi);
   /* Update RBGs position in the sorted RBG Queue */
   rgSCHDlfsUpdSbSortedLst(dlfsUe, sbCb, prevSbCqi);

   RETVOID;
}

/**
 * @brief Updated Average CQI and Average CQI Deviation
 *
 * @details
 *
 *     Function : rgSCHDlfsUpdSbCqiDeviation
 *
 *     Processing Steps:
 *      - Called whenever there is a change in one of the SB CQIs
 *        Recalculates Avg CQI and AvgCqiDeviation using the 
 *        previous and current SB CQI values.
 *
 *  @param[in] RgSchDlfsCellCb  *dlfsCell
 *  @param[in] RgSchDlfsUeCb    *dlfsUe
 *  @param[in] RgSchDlfssbCqi   *sbCb
 *  @param[in] U32               prevSbCqi 
 *  @return  Void
 **/
#ifdef ANSI
PRIVATE Void rgSCHDlfsUpdSbCqiDeviation
(
RgSchDlfsCellCb    *dlfsCell,
RgSchDlfsUeCb      *dlfsUe,
RgSchDlfsSbCqi     *sbCb,
U32                prevSbCqi
)
#else
PRIVATE Void rgSCHDlfsUpdSbCqiDeviation(dlfsCell, dlfsUe, sbCb, prevSbCqi)
RgSchDlfsCellCb    *dlfsCell;
RgSchDlfsUeCb      *dlfsUe;
RgSchDlfsSbCqi     *sbCb;
U32                prevSbCqi;
#endif
{
   U32 prevAvgCqiDev = dlfsUe->avgCqiDeviation;
   U32 prevAvgCqiVal       = dlfsUe->avgSbCqiVal;
   U32 newCqiVal           = sbCb->cqi * 1024;
   U32 prevCqiVal          = prevSbCqi * 1024;

   TRC2(rgSCHDlfsUpdSbCqiDeviation);
   
   /* Note: This approximate avgSbCqiVal is near accurate 
    to the actual Avg */
   dlfsUe->avgSbCqiVal = ((prevAvgCqiVal * dlfsCell->numDlSb) 
                          - prevCqiVal + newCqiVal) / 
                         (dlfsCell->numDlSb); 

   /* Note: To get exact avg CQI deviation value, we need to loop
    * for all subbands. */
   /* An approximate value of avg CQI deviation is derived 
    * without looping across all subbands. Eventhough this is 
    * error prone(specially when most of the SBs CQIs are different
    * from one another). However, in this tradeoff situation, we chose
    * to reduce CPU cycles than accuracy for this calculation */
   if  (prevAvgCqiDev < RG_ABSLT_DIFF(prevAvgCqiVal, prevCqiVal))
   {
      dlfsUe->avgCqiDeviation = RG_ABSLT_DIFF(dlfsUe->avgSbCqiVal, newCqiVal);
   }
   else
   {
      dlfsUe->avgCqiDeviation = prevAvgCqiDev -
                             RG_ABSLT_DIFF(prevAvgCqiVal, prevCqiVal) +
                             RG_ABSLT_DIFF(dlfsUe->avgSbCqiVal, newCqiVal);
   }

   RETVOID;
}

/**
 * @brief Remove Sub-band from the Sorted Subband Queue.
 *
 * @details
 *
 *     Function : rgSCHDlfsRmvSbFrmLst
 *
 *     Processing Steps:
 *     - Removed sbCb from the sortlst and if sbCb is head of the
 *       list then updates the head of the list.
 *
 *  @param[in] RgSchDlfsUeCb  *dlfsUe
 *  @param[in] RgSchDlfssbCqi *sbCb
 *  @return  Void
 **/
#ifdef ANSI
PRIVATE Void rgSCHDlfsRmvSbFrmLst
(
RgSchDlfsUeCb      *dlfsUe,
RgSchDlfsSbCqi     *sbCb
)
#else
PRIVATE Void rgSCHDlfsRmvSbFrmLst(dlfsUe, sbCb)
RgSchDlfsUeCb      *dlfsUe;
RgSchDlfsSbCqi     *sbCb;
#endif
{
   TRC2(rgSCHDlfsRmvSbFrmLst);

   if (sbCb->next)
   {
      sbCb->next->prev = sbCb->prev;
   }
   if (sbCb->prev)
   {
      sbCb->prev->next = sbCb->next;
   }
   else
   {
      /* sbCb is the head of the sorted List */
      dlfsUe->sortSbCqiLst = sbCb->next;
   }
   RETVOID;
}

/**
 * @brief Re-positions Sub-band in the Sorted Subband Queue.
 *
 * @details
 *
 *     Function : rgSCHDlfsUpdSbSortedLst
 *
 *     Processing Steps:
 *     - if reported SB CQI is less than prevSbCqi then reposition
 *       this subband right to the current position.
 *     - else reposition this subband left to the current position.
 *
 *  @param[in] RgSchDlfsUeCb  *dlfsUe
 *  @param[in] RgSchDlfssbCqi *sbCb
 *  @param[in] U32            prevSbCqi 
 *  @return  Void
 **/
#ifdef ANSI
PRIVATE Void rgSCHDlfsUpdSbSortedLst
(
RgSchDlfsUeCb      *dlfsUe,
RgSchDlfsSbCqi     *sbCb,
U32                prevSbCqi
)
#else
PRIVATE Void rgSCHDlfsUpdSbSortedLst(dlfsUe, sbCb, prevSbCqi)
RgSchDlfsUeCb      *dlfsUe;
RgSchDlfsSbCqi     *sbCb;
U32                prevSbCqi;
#endif
{
   RgSchDlfsSbCqi *currPos = NULLP;/* Traversing Node */
   RgSchDlfsSbCqi *insPos = NULLP;/* Node after which sbCb to be inserted */

   TRC2(rgSCHDlfsUpdSbSortedLst);

   /* sortSbCqiLst is in desc order of SB CQIs */
   if (sbCb->cqi < prevSbCqi)
   {
      /* Traverse and insert to the right of sbCb */
      currPos = sbCb->next;

      /* Inserting sbCb in its new position */
      while(currPos)
      {
         if (sbCb->cqi >= currPos->cqi)
         {
            break;
         }
         insPos = currPos;
         currPos = currPos->next;
      }
      /* insPos = NULLP indicates sbCb is already at 
       * correct position */
      if (insPos)
      {
         /* Remove sbCb from existing position */
         rgSCHDlfsRmvSbFrmLst(dlfsUe, sbCb);

         /* Insert next to insPos */
         sbCb->next = insPos->next;
         sbCb->prev = insPos;
         if (insPos->next)
         {
            insPos->next->prev = sbCb;
         }
         insPos->next = sbCb;
      }
   }
   else
   {
      /* Traverse and insert to the left of sbCb */
      currPos = sbCb->prev;
      /* Inserting sbCb in its new position */
      while(currPos)
      {
         if (sbCb->cqi <= currPos->cqi)
         {
            break;
         }
         insPos = currPos;
         currPos = currPos->prev;
      }
      /* insPos = NULLP indicates sbCb is already at 
       * correct position */
      if (insPos)
      {
         /* Remove sbCb from existing position */
         rgSCHDlfsRmvSbFrmLst(dlfsUe, sbCb);
         /* Insert prev to insPos */
         sbCb->prev = insPos->prev;
         sbCb->next = insPos;
         if (insPos->prev)
         {
            insPos->prev->next = sbCb;
         }
         else
         {
            /* insPos is the head of the sorted List */
            dlfsUe->sortSbCqiLst = sbCb;
         }
         insPos->prev = sbCb;
      }
   }

   RETVOID;
}
#endif

/**********************************************************************
 
         End of file:     gk_sch_dlfs.c@@/main/2 - Sat Jul 30 02:21:40 2011
 
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
           rg001.201 sm   1. Inappropriate name 
                             of CQI to ITbs table for DL has been 
                             corrected.
           rg003.201 ns   1. Trace added for functions.
           rg004.201 ms   1. DL DTX Handling
           rg004.201 rnt  1. Modified for SI Enhancement
           rg005.201 sm   1. Moved the variable declaration.
           rg007.201   ap  1. Added support for MIMO
           rg008.201 sd  1. Removed dependency on MIMO compile-time flag
           rg009.201 rsharon  1.[ccpu00112372]:Changes for CR timer.
                     gj       2. Changing loop limit from RGSCH_NUM_SUB_FRAMES
                                 to RGSCH_NUM_DL_SUBFRAMES
           rg009.201 dv 1. Added Changes of TFU_UPGRADE
/main/2      ---         gvj 1. Updated for LTE MAC Rel 3.1
           rg002.301 dvasisht 1.[cppu00120694]: DLSF Index calculation support for TDD
$SID$        ---     rt       1. LTE MAC 4.1 release
*********************************************************************91*/
