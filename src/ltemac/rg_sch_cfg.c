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
 
     Desc:     C source code for Entry point functions.
 
     File:     rg_sch_cfg.c
 
     Sid:      gk_sch_cfg.c@@/main/2 - Sat Jul 30 02:21:35 2011
 
     Prg:     ns
 
**********************************************************************/

/** @file rg_sch_cfg.c
@brief This module handles the configuration of SCH by RRC and RRM.
*/

static const char* RLOG_MODULE_NAME="MAC";
static int RLOG_FILE_ID=186;
static int RLOG_MODULE_ID=4096;

/* header include files -- defines (.h) */
#include "envopt.h"        /* environment options */
#include "envdep.h"        /* environment dependent */
#include "envind.h"        /* environment independent */
#include "gen.h"           /* general layer */
#include "ssi.h"           /* system service interface */
#include "cm_hash.h"       /* common hash list */
#include "cm_mblk.h"       /* common memory link list library */
#include "cm_llist.h"      /* common linked list library */
#include "cm_err.h"        /* common error */
#include "cm_lte.h"        /* common LTE */
#include "lrg.h"
#include "rgr.h"
#include "rgm.h"
#include "tfu.h"
#include "rg_env.h"
#include "rg_sch_inf.h"
#include "rg_sch.h"
#include "rg_sch_err.h"
#include "rg_sch_cmn.h"
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
#include "cm_lte.x"       /* common tokens */
#include "tfu.x"           /* TFU types */
#include "lrg.x"           /* layer management typedefs for MAC */
#include "rgr.x"           /* layer management typedefs for MAC */
#include "rgm.x"           /* layer management typedefs for MAC */
#include "rg_sch_inf.x"            /* typedefs for MAC */
#include "rg_sch.x"            /* typedefs for MAC */
/* [ccpu00124018]-MOD- Retrieving CQI value from cell config*/
#include "rg_sch_cmn.x"   

/* LTE-MAC Scheduler instance control block structures */
PUBLIC RgSchCb rgSchCb[RGSCH_MAX_INST];

#ifdef PHY_ERROR_LOGING
PUBLIC RgSchUlAllocCntr rgSchUlAllocCntr;
#endif

/* local defines */
PRIVATE S16 rgSCHCfgRgrUeRecfgRntiChg ARGS (( RgSchCellCb *cell,
      RgSchUeCb *ue, RgrUeRecfg *ueRecfg, RgSchErrInfo *errInfo));
PRIVATE S16 rgSCHCfgVldtUePwrCfg ARGS((RgSchCellCb *cell,
         RgrUeUlPwrCfg *pwrCfg));
PRIVATE S16 rgSCHCfgVldtUeGrpPwrCfg ARGS((RgSchCellCb *cell,
         RgrUeGrpPwrCfg *grpPwrCfg));
#ifdef LTEMAC_SPS
PRIVATE S16 rgSCHCfgVldtUeDlSpsCfg ARGS((RgSchCellCb *cell,
         RgrUeSpsDlCfg *dlSpsCfg));
PRIVATE S16 rgSCHCfgVldtSpsReCfg ARGS ((RgSchCellCb *cell,
         RgSchUeCb *ue, RgrUeRecfg *ueRecfg));
#endif /*LTEMAC_SPS*/
PRIVATE S16 rgSCHCfgVldtUeCqiModeCfg ARGS((RgSchCellCb *cell,RgrUeCfg *ueCfg));
PRIVATE S16 rgSCHCfgVldtUeMeasGapAckNakRepCfg ARGS ((RgSchCellCb *cell,
         RgrUeCfg *ueCfg));
PRIVATE S16 rgSCHCfgVldtUeMeasGapAckNakRepRecfg ARGS ((RgSchCellCb *cell,
         RgrUeRecfg *ueRecfg));
PRIVATE Void rgSCHCfgFreeDlDedLcCb ARGS((RgSchCellCb *cell, RgSchUeCb *ue,
         RgSchDlLcCb *dlLc));
PRIVATE Void rgSCHCfgFreeDlCmnLcCb ARGS((RgSchClcDlLcCb *cmnDlLc));
PRIVATE Void rgSCHCfgFreeUeCb ARGS((RgSchCellCb *cell, RgSchUeCb *ue));
PRIVATE Void rgSCHCfgFreeRgrCfgLst ARGS((RgSchCellCb *cell));
PRIVATE Void rgSCHCfgFreeCmnLcLst ARGS((RgSchCellCb *cell));
PRIVATE Void rgSCHCfgFreeUeLst ARGS((RgSchCellCb *cell));
#ifdef LTEMAC_SPS
PRIVATE Void rgSCHCfgFreeSpsUeLst ARGS((RgSchCellCb *cell));
#endif
PRIVATE S16 rgSCHCfgVldtRgrCmnLcCfg ARGS((Inst inst, RgrCellCfg *cellCfg,
                         RgSchErrInfo *errInfo));
PRIVATE S16 rgSCHCfgVldtRgrCellPwrCfg ARGS((Inst inst, RgrCellCfg *cellCfg,
         RgSchErrInfo *errInfo));
PRIVATE S16 rgSCHCfgVldtRgrCellSchCfg ARGS((Inst inst, RgrCellCfg *cellCfg));
PRIVATE S16 rgSCHCfgVldtRgrCellRACfg ARGS((Inst inst, RgrCellCfg *cellCfg));
PRIVATE Void rgSCHCfgRgrUePhrMsg3 ARGS(( RgSchCellCb *cell,
             RgSchRaCb *raCb,RgSchUeCb *ue, RgSchErrInfo *errInfo));
PRIVATE S16 rgSCHCfgRgrCmnLcCfg ARGS((RgSchCellCb *cell, RgrCmnLchCfg *lcCfg,
                         RgSchErrInfo *errInfo));
#ifdef RGR_SI_SCH
PRIVATE S16 rgSCHCfgVldtRgrCellSiCfg ARGS(( Inst inst, RgrSiCfg  *siCfg));
#endif/*RGR_SI_SCH */

/* LTE_ADV_FLAG_REMOVED_START */
PRIVATE S16 rgSCHCfgVldtRgrCellLteAdvCfg ARGS(( Inst inst, 
                                 RgrLteAdvancedCellConfig *lteAdvCfg, U8 dlTotalBw));
/* LTE_ADV_FLAG_REMOVED_END */

PRIVATE S16 rgSCHCfgVldtDrxUeCfg ARGS ((RgSchCellCb *cell,
                                       RgrUeDrxCfg *ueDrxCfg));
PRIVATE S16 rgSCHCfgVldtDrxOnDurCfg ARGS((U8 onDurTmr));
PRIVATE S16 rgSCHCfgVldtDrxInActvCfg ARGS((U16 inActvTmr));
PRIVATE S16 rgSCHCfgVldtDrxReTxCfg ARGS((U8 reTxTmr));
PRIVATE S16 rgSCHCfgVldtDrxLngCycCfg ARGS((RgrDrxLongCycleOffst lngCycleOffst));
PRIVATE S16 rgSCHCfgVldtDrxLngCyclTmrs ARGS((U16 val));
PRIVATE S16 rgSCHCfgVldtDrxShrtCycCfg ARGS((RgrDrxShortDrx shrtCycCfg));
PRIVATE S16 rgSCHCfgVldtRgrCellCsgParamCfg ARGS((Inst inst,
                          RgrCellCsgParamCfg   *csgParam));
#ifdef TFU_UPGRADE

PUBLIC S16 rgSCHCfgACqiUeCfg ARGS(( RgSchCellCb *cellCb, RgSchUeCb  *ueCb, 
                     RgrUeAprdDlCqiCfg *aCqiCfg, CmLteUeCategory ueCat ));

PUBLIC S16 rgSCHCfgAcqiUeReCfg ARGS(( RgSchCellCb *cellCb, RgSchUeCb *ueCb, 
 RgrUeAprdDlCqiCfg   *acqiCfg, CmLteUeCategory  ueCat ));

PUBLIC S16 rgSCHUtlGetCfgPerOff ARGS(( RgSchPerTbl tbl, U16  cfgIdx, 
         U16 *peri, U16 *offset ));

PUBLIC S16 rgSCHCfgRiUeCfg ARGS((RgSchCellCb  *cellCb, RgSchUeCb    *ueCb, 
                   RgrUePrdDlCqiCfg *cqiCfg, CmLteUeCategory ueCat ));

PUBLIC S16 rgSCHCfgPCqiUeCfg ARGS((RgSchCellCb  *cellCb, RgSchUeCb    *ueCb, 
                     RgrUePrdDlCqiCfg *cqiCfg,CmLteUeCategory ueCat ));

PUBLIC S16 rgSCHCfgSrsUeCfg ARGS((RgSchCellCb   *cellCb,RgSchUeCb     *ueCb,
                     RgrUeUlSrsCfg *srsCfg ));

PUBLIC S16 rgSCHCfgSrUeCfg ARGS((RgSchCellCb   *cellCb,RgSchUeCb     *ueCb,
                     RgrUeSrCfg    *srCfg));

PUBLIC S16 rgSCHCfgPCqiUeReCfg ARGS((RgSchCellCb  *cellCb, RgSchUeCb *ueCb, 
                     RgrUePrdDlCqiCfg *cqiCfg,CmLteUeCategory ueCat));

PUBLIC S16 rgSCHCfgSrsUeReCfg ARGS((RgSchCellCb  *cellCb, RgSchUeCb *ueCb, 
                     RgrUeUlSrsCfg *srsCfg));

PUBLIC S16 rgSCHCfgSrUeReCfg ARGS((RgSchCellCb  *cellCb, RgSchUeCb *ueCb, 
                      RgrUeSrCfg   *srCfg));

PUBLIC S16 rgSCHCfgVldtRgrTxmodePuschMode ARGS((RgSchCellCb  *cellCb,
         RgrTxMode  txMde, RgrAprdCqiMode puschMode,RgSchErrInfo *errInfo));

PUBLIC S16 rgSCHCfgVldtRgrUeACqiCfg ARGS(( RgSchCellCb *cellCb,
         CmLteRnti  crnti, RgrUeAprdDlCqiCfg *acqiCfg, RgrUeTxModeCfg txMode,
         RgSchErrInfo  *errInfo ));

PUBLIC S16 rgSCHCfgVldtRgrTxmodePucchMode ARGS((RgSchCellCb  *cellCb,
         RgrTxMode  txMde, RgrPrdCqiMode pucchMode,RgSchErrInfo *errInfo));

#ifdef LTEMAC_HDFDD
PUBLIC S16 rgSCHCfgVldtRgrUePCqiCfg ARGS(( RgSchCellCb *cellCb,
         CmLteRnti  crnti, RgrUePrdDlCqiCfg *cqiCfg, Bool hdFdd,
         RgrUeTxModeCfg txMode, RgSchErrInfo *errInfo));
#else
PUBLIC S16 rgSCHCfgVldtRgrUePCqiCfg ARGS(( RgSchCellCb *cellCb,
         CmLteRnti  crnti, RgrUePrdDlCqiCfg *cqiCfg, RgrUeTxModeCfg txMode,
         RgSchErrInfo *errInfo));         
#endif
#ifdef LTEMAC_HDFDD
PUBLIC S16 rgSCHCfgVldtRgrUeUlSrsCfg  ARGS ((RgSchCellCb *cellCb,
             CmLteRnti crnti, RgrUeUlSrsCfg *srsCfg, Bool hdFdd, 
             RgSchErrInfo *errInfo));
#else
PUBLIC S16 rgSCHCfgVldtRgrUeUlSrsCfg  ARGS ((RgSchCellCb *cellCb,
         CmLteRnti crnti, RgrUeUlSrsCfg *srsCfg, 
         RgSchErrInfo *errInfo));
#endif

#ifdef LTEMAC_HDFDD
PUBLIC S16 rgSCHCfgVldtRgrUeSrCfg  ARGS((RgSchCellCb *cellCb, CmLteRnti crnti,  
         RgrUeSrCfg *srCfg, Bool hdFdd, RgSchErrInfo *errInfo));
#else
PUBLIC S16 rgSCHCfgVldtRgrUeSrCfg  ARGS((RgSchCellCb *cellCb, CmLteRnti crnti,  
         RgrUeSrCfg *srCfg, RgSchErrInfo *errInfo));
#endif
PRIVATE S16 rgSCHCfgVldtCqiSrSrsUeCfg ARGS (( RgSchCellCb       *cellCb,
                                             RgrUeCfg          *ueCfg,  
                                             RgSchErrInfo      *errInfo));

PRIVATE S16 rgSCHCfgVldtCqiSrSrsUeReCfg ARGS (( RgSchCellCb  *cellCb,
                                               RgSchUeCb    *ueCb,
                                               RgrUeRecfg   *ueReCfg,  
                                               RgSchErrInfo *errInfo ));

PUBLIC S16 rgSCHCfgPCqiSrsSrUeDel ARGS ((RgSchCellCb *cellCb, 
                                         RgSchUeCb *ueCb));

PRIVATE Void rgSCHCfgUtlFetchAcqiBitSz ARGS ((RgSchUeCb* ueCb, U8  numTxAnt,
                     U8* cqiPmiSzR1,U8* cqiPmiSzRn1 ));

/*  Added the function to be used instead of the 
 *  MACRO RG_SCH_GET_PERIODICITY_TBL */
PRIVATE CONSTANT RgSchUePCqiSrsSrCfgIdxTbl*  rgSCHCfgUtlGetPcqiSrsSrRiTbl ARGS ((
                                           RgSchPerTbl tblType, 
                                           U8 * min, 
                                           U8 * max));

#endif /* TFU_UPGRADE */ 
PRIVATE Void rgSCHCfgUeTaRecfg ARGS (( RgSchCellCb  *cellCb,
                                               RgSchUeCb    *ueCb,
                                               RgrUeRecfg   *ueReCfg,  
                                               RgSchErrInfo *errInfo ));
/* ccpu00117452 - MOD - Changed macro name from
   RGR_RRM_DLPWR_CNTRL to RGR_CQI_REPT */
#if ((defined (RGR_CQI_REPT)) && (defined (RGR_V2)))
PRIVATE S16 rgSCHCfgUeCqiReptReCfg ARGS ((
RgSchCellCb    *cell,
RgSchUeCb      *ue,
RgrUeRecfg     *ueRecfg
));

PRIVATE S16 rgSCHCfgVldtCqiReptReCfg ARGS ((
RgSchCellCb       *cell,
RgrUeRecfg     *ueRecfg
));
#endif
/*LTE_L2_MEAS_PHASE2*/
#ifdef LTE_L2_MEAS
PRIVATE S16 rgSchAddToL2Meas ARGS ((RgSchCellCb *cellCb,RgSchDlLcCb *dlLc));
#endif
/* local typedefs */
CONSTANT RgSchSrsTxOffst rgSrsTxOffstTbl[RGSCH_MAX_SRS_SFCFG_IDX+1]=
                                  {{1,{0,0,0,0,0,0,0,0}},
                                   {1,{0,0,0,0,0,0,0,0}},
                                   {1,{1,0,0,0,0,0,0,0}},
                                   {1,{0,0,0,0,0,0,0,0}},
                                   {1,{1,0,0,0,0,0,0,0}},
                                   {1,{2,0,0,0,0,0,0,0}},
                                   {1,{3,0,0,0,0,0,0,0}},
                                   {2,{0,1,0,0,0,0,0,0}},
                                   {2,{2,3,0,0,0,0,0,0}},
                                   {1,{0,0,0,0,0,0,0,0}},
                                   {1,{1,0,0,0,0,0,0,0}},
                                   {1,{2,0,0,0,0,0,0,0}},
                                   {1,{3,0,0,0,0,0,0,0}},
                                   {7,{0,1,2,3,4,6,8,0}},
                                   {8,{0,1,2,3,4,5,6,8}},
                                   {0,{0,0,0,0,0,0,0,0}}};
 
/* local externs */

#ifdef TFU_UPGRADE
/* SRSCfg Table: Ref 36.213, Table: 8.2.1  */
CONSTANT RgSchUePCqiSrsSrCfgIdxTbl  
rgSchUeSrsCfgIdxFddTbl[RG_SCH_SRS_ISRS_INDX_MAX_FDD]=
{
 { 0,1,    2,  0 },
 { 2,6,    5,  2 },
 { 7,16,   10, 7 },
 { 17,36,  20, 17},
 { 37,76,  40, 37},
 { 77,156, 80, 77},
 { 157,316,160,157},
 { 317,636,320,317 }
};

/* Reference : 36.213 Table 8.2-2 */
CONSTANT RgSchUePCqiSrsSrCfgIdxTbl 
rgSchUeSrsCfgIdxTddTbl[RG_SCH_SRS_ISRS_INDX_MAX_TDD]=
{
 { 10,14,  5,  10 },
 { 15,24,  10, 15 },
 { 25,44,  20, 25 },
 { 45,84,  40, 45 },
 { 85,164, 80, 85 },
 { 165,324,160, 165 },
 { 325,644,320, 325 }
 /* RESERVED: Configuration Module should not allow Res values */
};

/*Reference: 36.213 Table:7.2.2-1A */
CONSTANT RgSchUePCqiSrsSrCfgIdxTbl 
rgSchUePCqiCfgIdxFddTbl[RG_SCH_CQIPMI_CFGIDX_MAX_FDD]=
{
 { 0,1,   2, 0 },
 { 2,6,   5, 2 },
 { 7,16,  10,7 },
 {17,36,  20,17},
 {37,76,  40, 37},
 {77,156, 80, 77},
 {157,316,160,157},
 {318,349,32,318}, 
 {350,413,64, 350},
 {414,541,128,414 }
 /* RESERVED: Configuration should not allow Res values */
};

/* Reference: 36.213 Table:7.2.2-1C */
CONSTANT RgSchUePCqiSrsSrCfgIdxTbl 
rgSchUeCqiPmiCfgIdxTddTbl[RG_SCH_CQIPMI_CFGIDX_MAX_TDD]=
{
 { 0,0,   1, 0 },
 { 1,5,   5, 1 },
 { 6,15,  10,6 },
 {16,35,  20,16},
 {36,75,  40,36},
 {76,155, 80,76},
 {156,315,160,156}
 /* RESERVED: Configuration should not allow Res values */
};

/* Note: RI table is same for FDD and TDD */
/*Reference: 36.213 Table:7.2.2-1B */
CONSTANT RgSchUePCqiSrsSrCfgIdxTbl 
rgSchUeRiCfgIdxTbl[RG_SCH_RI_CFGIDX_MAX]=
{
 { 0,160,    1, 0 },
 { 161,321,  2, 161 },
 { 322,482,  4, 322 },
 { 483,643,  8, 483},
 { 644,804,  16,644},
 { 805,965,  32,805 }
 /* RESERVED: Configuration should not allow Res values */
};

/*Reference: 36.213 Table:7.2.2-2 */
CONSTANT RgSchUeBwSubSzBwParts 
rgSchUeBwSubSzBwPartsTbl[RG_SCH_BW_SUBSZ_BWPARTS_MAX]=
{
 {6,7, 0, 0},  /*TODO: 6,7, NA, NA */
 {8, 10, 4,1},
 {11, 26, 4,2},
 {27, 63, 6,3},
 {64, 110, 8,4}
};


/* Reference : 36.213 Table 10.1-5 */
/* Note: SR is same table for TDD and FDD */
CONSTANT RgSchUePCqiSrsSrCfgIdxTbl rgSchUeSrCfgIdxTbl[RG_SCH_ISR_INDX_MAX]=
{
 { 0,4,   5, 0 },
 { 5,14,   10, 5 },
 { 15,34,  20,15 },
 { 35,74, 40,35},
 { 75,154, 80, 75}
 /* RESERVED: Configuration should not allow Res values */
};

/*Reference:36.213: Derived from Table: 7.2.1-5 for Label L.
 The Label L is CEIL(log2(BinCoe(N and M)))*/
CONSTANT U8 RgSCHUeAcqi2022LBitWidth[6][28] ={
{0,1,2,2,3,3,3,3,4,4,4,4,4,4,4,4,5,5,5,5,5,5,5,5,5,5,5,5},
{0,0,2,3,4,4,5,5,6,6,6,7,7,7,7,7,8,8,8,8,8,8,8,9,9,9,9,9},
{0,0,0,2,4,5,6,6,7,7,8,8,9,9,9,10,10,10,10,11,11,11,11,11,12,12,12,12},
{0,0,0,0,3,4,6,7,7,8,9,9,10,10,11,11,12,12,12,13,13,13,14,14,14,14,15,15},
{0,0,0,0,0,3,5,6,7,8,9,10,11,11,12,13,13,14,14,14,15,15,16,16,16,17,17,17},
{0,0,0,0,0,0,3,5,7,8,9,10,11,12,13,13,14,15,15,16,16,17,17,18,18,18,19,19}
};

#endif 
 
EXTERN U8 rgSchCmnHarqRtt[];
 
/* forward references */



/**
 * @brief Validates the cell configuration request from RRM to MAC.
 *
 * @details
 *
 *     Function : rgSCHCfgVldtRgrCellCfg
 *
 *     Processing Steps:
 *      - Retrieve the cell control block.
 *      - If successful,
 *        - Validate the range of configured values recieved in
 *          configuration request.
 *        - If validated successfully,
 *          - Return ROK and pointer to the cell.
 *        - Else 
 *          - Return RFAILED.
 *      - Else return RFAILED.
 *  @param[in]  Inst         inst
 *  @param[in]  RgrCellCfg   *cellCfg
 *  @param[out] RgSchCellCb  **cell
 *  @param[out] RgSchErrInfo *errInfo
 *  @return  S16
 *      -# ROK
 *      -# RFAILED
 **/
#ifdef ANSI
PUBLIC S16 rgSCHCfgVldtRgrCellCfg
(
Inst         inst,
RgrCellCfg   *cellCfg,
RgSchCellCb  *cell,
RgSchErrInfo *errInfo
)
#else
PUBLIC S16 rgSCHCfgVldtRgrCellCfg(inst, cellCfg, cell, errInfo)
Inst         inst;
RgrCellCfg   *cellCfg;
RgSchCellCb  *cell;
RgSchErrInfo *errInfo;
#endif
{
   TRC2(rgSCHCfgVldtRgrCellCfg);

   errInfo->errCause = RGSCHERR_CFG_INVALID_RGR_CELL_CFG;

   /* check if cell exists already */
   if ((U8 *)cell != NULLP)
   {
      RLOG_ARG0(L_ERROR,DBG_CELLID,cellCfg->cellId,"Cell Id already exists");
      RETVALUE(RFAILED);
   }
   
   if(cellCfg->macInst >= RGSCH_INST_START)
   {
      RLOG_ARG1(L_ERROR,DBG_CELLID,cellCfg->cellId,  "Invalid MAC Instance %d ",
               cellCfg->macInst);
      RETVALUE(RFAILED);
   }
   
   if (cellCfg->macRnti.startRnti < RGSCH_MIN_MAC_RNTI )
   {
      RLOG_ARG1(L_ERROR,DBG_CELLID,cellCfg->cellId,  "Invalid start RNTI %d for cell ",
               cellCfg->macRnti.startRnti);
      RETVALUE(RFAILED);
   }

   if ((rgSCHCfgVldtRgrCellSchCfg(inst, cellCfg)) != ROK)
   {
      RLOG_ARG0(L_ERROR,DBG_CELLID,cellCfg->cellId,  "Validation for scheduler related "
                          "config failed");
      RETVALUE(RFAILED);
   }

   if ((cellCfg->dlHqCfg.maxDlHqTx < RGSCH_MIN_HQ_TX) ||
         (cellCfg->dlHqCfg.maxMsg4HqTx < RGSCH_MIN_HQ_TX))
   {
      RLOG_ARG2(L_ERROR,DBG_CELLID,cellCfg->cellId, "Invalid Downlink HARQ configuration:"
               " maxDlHqTx %d maxMsg4HqTx %d", cellCfg->dlHqCfg.maxDlHqTx,
               cellCfg->dlHqCfg.maxMsg4HqTx);
      RETVALUE(RFAILED);
   }
   if ((cellCfg->cfiCfg.cfi < RGSCH_MIN_CFI_VAL) ||
         (cellCfg->cfiCfg.cfi > RGSCH_MAX_CFI_VAL))
   {
      RLOG_ARG1(L_ERROR,DBG_CELLID,cellCfg->cellId,  "Invalid CFI configuration %d",
           cellCfg->cfiCfg.cfi);
      RETVALUE(RFAILED);
   }
   if (((cellCfg->puschSubBand.subbandStart) + 
        ((cellCfg->puschSubBand.numSubbands -1) * cellCfg->puschSubBand.size))
         > (cellCfg->bwCfg.ulTotalBw - 1))
   {
      RLOG_ARG4(L_ERROR,DBG_CELLID,cellCfg->cellId, "Invalid PUSCH subband configuration:"
               " subBandStart %d numSubBands %d subBandSize %d ulTotBw %d",
               cellCfg->puschSubBand.subbandStart, 
               cellCfg->puschSubBand.numSubbands, cellCfg->puschSubBand.size,
               cellCfg->bwCfg.ulTotalBw);
      RETVALUE(RFAILED);
   }

   /* Validation for the cell parameters */
   if ((cellCfg->numTxAntPorts == 0) || (cellCfg->numTxAntPorts > 4))
   {
      RLOG_ARG1(L_ERROR,DBG_CELLID,cellCfg->cellId, "Invalid number of transmit antenna" 
            " ports %d", cellCfg->numTxAntPorts);
      RETVALUE(RFAILED);
   }
   if (((cellCfg->bwCfg.dlTotalBw < RGSCH_MIN_DL_BW) ||
            (cellCfg->bwCfg.dlTotalBw > RGSCH_MAX_DL_BW)) ||
         ((cellCfg->bwCfg.ulTotalBw < RGSCH_MIN_UL_BW) || 
          (cellCfg->bwCfg.ulTotalBw > RGSCH_MAX_UL_BW)))
   {
      RLOG_ARG2(L_ERROR,DBG_CELLID,cellCfg->cellId,  "Invalid Bandwidth configuration:"
            " ul %d dl %d",cellCfg->bwCfg.ulTotalBw,
            cellCfg->bwCfg.dlTotalBw);
      RETVALUE(RFAILED);
   }
   if (cellCfg->phichCfg.ngEnum > RGR_NG_TWO)
   {
      RLOG_ARG1(L_ERROR,DBG_CELLID,cellCfg->cellId,  "Invalid PHICH Ng configuration %d",
               (U8)cellCfg->phichCfg.ngEnum);
      RETVALUE(RFAILED);
   }
   /* Validation for extended PHICH Duration */
   if ((cellCfg->phichCfg.isDurExtend == TRUE) && 
         (cellCfg->bwCfg.dlTotalBw <= 10) && (cellCfg->cfiCfg.cfi < 2))
   {
      RLOG_ARG2(L_ERROR,DBG_CELLID,cellCfg->cellId,  "Invalid cfi value for"
               "Extended PHICH duration cfi:%d dlBw:%d",
               (U8)cellCfg->cfiCfg.cfi, cellCfg->bwCfg.dlTotalBw);
      RETVALUE(RFAILED);
   }
   if ((cellCfg->phichCfg.isDurExtend == TRUE) &&
         (cellCfg->bwCfg.dlTotalBw > 10) && (cellCfg->cfiCfg.cfi < 3))
   {
      RLOG_ARG2(L_ERROR,DBG_CELLID,cellCfg->cellId,  "Invalid cfi value for"
               "Extended PHICH duration cfi:%d dlBw:%d",
               (U8)cellCfg->cfiCfg.cfi, cellCfg->bwCfg.dlTotalBw);
      RETVALUE(RFAILED);
   }
   /* ccpu00138567- Removing validation check for resourceSize as 0.
    * From the spec, n2RB value 0 is a valid config. */
   if ((cellCfg->pucchCfg.resourceSize >= cellCfg->bwCfg.ulTotalBw/2) ||
         (cellCfg->pucchCfg.n1PucchAn == 0) ||
         (cellCfg->pucchCfg.deltaShift < RGSCH_PUCCH_MINVAL_DS) ||
         (cellCfg->pucchCfg.deltaShift > RGSCH_PUCCH_MAXVAL_DS) ||
         (cellCfg->pucchCfg.cyclicShift > RGSCH_PUCCH_MAXVAL_CS))
   {
      RLOG_ARG4(L_ERROR,DBG_CELLID,cellCfg->cellId,  "Invalid PUCCH configuration:"
              " N2_RB %d N1_PUCCH %d deltaShift %d cyclicShift %d",
               cellCfg->pucchCfg.resourceSize,
               cellCfg->pucchCfg.n1PucchAn,
               cellCfg->pucchCfg.deltaShift,
               cellCfg->pucchCfg.cyclicShift);
      RETVALUE(RFAILED);
   }
   if (cellCfg->srsCfg.isSrsCfgSetup && cellCfg->srsCfg.srsBwEnum > RGR_SRS_BWCFG_7)
   {
      RLOG_ARG1(L_ERROR,DBG_CELLID,cellCfg->cellId,  "Invalid SRS configuration: "
               " srsBw %d", (U8)cellCfg->srsCfg.srsBwEnum);
      RETVALUE(RFAILED);
   }

   if ((rgSCHCfgVldtRgrCellRACfg(inst, cellCfg)) != ROK)
   {
      RLOG_ARG0(L_ERROR,DBG_CELLID,cellCfg->cellId,  "Validation for Random access related"
                          "config failed");
      RETVALUE(RFAILED);
   }

   if ((rgSCHCfgVldtRgrCellPwrCfg(inst, cellCfg, errInfo)) != ROK)
   {
      RLOG_ARG0(L_ERROR,DBG_CELLID,cellCfg->cellId,  "Validation for cell power "
                          "config failed");
      RETVALUE(RFAILED);
   }

   /* Validate the common logical channel configuration */
   if( (cellCfg->numCmnLcs < RGR_MIN_CMN_LC_PER_CELL)||
       (cellCfg->numCmnLcs > RGR_MAX_CMN_LC_PER_CELL))
   {
      RLOG_ARG1(L_ERROR,DBG_CELLID,cellCfg->cellId,  "Invalid number(%d) of common logical"
              "channels in cell config", cellCfg->numCmnLcs);
      RETVALUE(RFAILED);
   }
   if ((rgSCHCfgVldtRgrCmnLcCfg(inst, cellCfg, errInfo)) != ROK)
   {
      RLOG_ARG0(L_ERROR,DBG_CELLID,cellCfg->cellId,  "Validation for common logical"
                          "channels failed");
      RETVALUE(RFAILED);
   }

   /* Added 0 as a valid value for number of TICKs RRM
    * 0 implies no ticks i.e. shutting off the feature.*/

#ifdef RGR_SI_SCH
   if ((rgSCHCfgVldtRgrCellSiCfg(inst, &(cellCfg->siCfg))) != ROK)
   {
      RLOG_ARG0(L_ERROR,DBG_CELLID,cellCfg->cellId,  "Validation for SI"
                          "configuration failed");
      RETVALUE(RFAILED);
   }
#endif /*RGR_SI_SCH */

   /*ccpu00116923 - ADD - Srs Present support - Start*/
#ifdef TFU_UPGRADE
#ifdef LTE_TDD
   if(cellCfg->srsCfg.isSrsCfgSetup && cellCfg->srsCfg.srsSubFrameCfg > 13)
#else
      if(cellCfg->srsCfg.isSrsCfgSetup && cellCfg->srsCfg.srsSubFrameCfg > 14)
#endif
      {
         RLOG_ARG0(L_ERROR,DBG_CELLID,cellCfg->cellId,  "Invalid"
                  "Subframe configuration");
         RETVALUE(RFAILED);
      }
#endif
   /*ccpu00116923 - ADD - Srs Present support - End*/
  
  if ((cellCfg->bcchTxPwrOffset > 10000) ||
       (cellCfg->pcchTxPwrOffset > 10000) ||
       (cellCfg->rarTxPwrOffset > 10000) ||
       (cellCfg->phichTxPwrOffset > 10000) 
      )
   {
      RLOG_ARG0(L_ERROR,DBG_CELLID,cellCfg->cellId, 
            "Invalid txPower offset ");
      RETVALUE(RFAILED);
   }

/* LTE_ADV_FLAG_REMOVED_START */
   /* Checking Whether DSFR is enabled without enabling SFR */
   if(((cellCfg->rgrLteAdvCfg.pres & RGR_DSFR) && 
       (RGR_ENABLE == cellCfg->rgrLteAdvCfg.dsfrCfg.status)) &&
      (!((cellCfg->rgrLteAdvCfg.pres & RGR_SFR) &&
         (RGR_ENABLE == cellCfg->rgrLteAdvCfg.sfrCfg.status))))
   {
      RLOG_ARG0(L_ERROR,DBG_CELLID,cellCfg->cellId,  "DSFR is enbaled"
               "Without enabling SFR");
      RETVALUE(RFAILED);
   }

   if ((rgSCHCfgVldtRgrCellLteAdvCfg(inst, &(cellCfg->rgrLteAdvCfg), 
                                      cellCfg->bwCfg.dlTotalBw)) != ROK)
   {
      RLOG_ARG0(L_ERROR,DBG_CELLID,cellCfg->cellId,  "Validation for LTE Adv"
               "configuration failed");
      RETVALUE(RFAILED);
   }
/* LTE_ADV_FLAG_REMOVED_END */
   if (cellCfg->msg4pAVal > RGRUE_DLPWRCNTRL_PA_DB3)
   {
      RLOG_ARG1(L_ERROR,DBG_CELLID,cellCfg->cellId,  "Invalid"
               "msg4pAVal %u", cellCfg->msg4pAVal);
      RETVALUE(RFAILED);
   }
  /* Validate csg access mode */
   if((cellCfg->accsMode < RGR_CELL_ACCS_OPEN) ||
      (cellCfg->accsMode > RGR_CELL_ACCS_HYBRID))
   {
      RGSCHDBGERR(inst, (rgSchPBuf(inst), "Invalid CSG Access mode\n"));
      RETVALUE(RFAILED);
   }
   /* Validate RgrCellCsgParamCfg */
   if(cellCfg->accsMode == RGR_CELL_ACCS_HYBRID) 
   {
   if((rgSCHCfgVldtRgrCellCsgParamCfg(inst, 
                                         &(cellCfg->csgParamCfg)) != ROK))
   {
      RGSCHDBGERR(inst, (rgSchPBuf(inst), "Validation failed for \n"
               "Access CSG parameter failed\n"));
      RETVALUE(RFAILED);
   }
   }
   errInfo->errCause = RGSCHERR_NONE;
   RETVALUE(ROK);
}  /* rgSCHCfgVldtRgrCellCfg */

/**
 * @brief Validates the scheduler related configuration request from RRM to MAC.
 *
 * @details
 *
 *     Function : rgSCHCfgVldtRgrCellSchCfg
 *
 *     Processing Steps:
 *      - Validate the scheduler related configuration request from RRC to MAC at CFG:
 *        validate the value range for the configured values.
 *      - If validated successfully,
 *        - Return ROK and pointer to the cell of UE.
 *      - Else 
 *        - Return RFAILED.
 *
 *  @param[in]  Inst         inst
 *  @param[out] RgSchCellCfg  *cellCfg
 *  @return  S16
 *      -# ROK
 *      -# RFAILED
 **/
#ifdef ANSI
PUBLIC S16 rgSCHCfgVldtRgrCellSchCfg
(
Inst         inst,
RgrCellCfg   *cellCfg
)
#else
PUBLIC S16 rgSCHCfgVldtRgrCellSchCfg(inst, cellCfg)
Inst         inst;
RgrCellCfg   *cellCfg;
#endif
{
   TRC2(rgSCHCfgVldtRgrCellSchCfg);

   if (cellCfg->ulSchdType > (RGSCH_NUM_SCHEDULERS - 1))
   {
      RLOG_ARG1(L_ERROR,DBG_CELLID,cellCfg->cellId,"Invalid UL scheduler type %d for cell",
                cellCfg->ulSchdType);
      RETVALUE(RFAILED);
   }
   if (cellCfg->dlSchdType > (RGSCH_NUM_SCHEDULERS - 1))
   {
      RLOG_ARG1(L_ERROR,DBG_CELLID,cellCfg->cellId,"Invalid DL scheduler type %d for cell",
                cellCfg->dlSchdType);
      RETVALUE(RFAILED);
   }
#if RGSCH_NUM_DLFS_SCHEDULERS
   if (cellCfg->dlfsSchdType > RGSCH_NUM_DLFS_SCHEDULERS - 1)
   {
      RLOG_ARG1(L_ERROR,DBG_CELLID,cellCfg->cellId,"Invalid dlfs scheduler type %d for cell",
                cellCfg->dlfsSchdType);
      RETVALUE(RFAILED);
   }
#endif
   RETVALUE(ROK);
}
/**
 * @brief Validates the RACH related configuration request from RRM to MAC.
 *
 * @details
 *
 *     Function : rgSCHCfgVldtRgrCellRACfg
 *
 *     Processing Steps:
 *      - Validate the RA configuration request from RRC to MAC at CFG:
 *        validate the value range for the configured values.
 *      - If validated successfully,
 *        - Return ROK and pointer to the cell of UE.
 *      - Else 
 *        - Return RFAILED.
 *
 *  @param[in]  Inst         inst
 *  @param[out] RgSchCellCfg  *cellCfg
 *  @return  S16
 *      -# ROK
 *      -# RFAILED
 **/
#ifdef ANSI
PUBLIC S16 rgSCHCfgVldtRgrCellRACfg
(
Inst         inst,
RgrCellCfg   *cellCfg
)
#else
PUBLIC S16 rgSCHCfgVldtRgrCellRACfg(inst, cellCfg)
Inst         inst;
RgrCellCfg   *cellCfg;
#endif
{
   TRC2(rgSCHCfgVldtRgrCellRACfg);


#ifdef LTE_TDD
   if ((cellCfg->rachCfg.preambleFormat > RGSCH_MAX_TDD_RA_PREAMBLE_FMT) ||
#else
   if ((cellCfg->rachCfg.preambleFormat > RGSCH_MAX_RA_PREAMBLE_FMT) ||
#endif
         (cellCfg->rachCfg.raWinSize < RGSCH_MIN_RA_WINSIZE)  ||
         (cellCfg->rachCfg.raWinSize > RGSCH_MAX_RA_WINSIZE) ||
         (cellCfg->rachCfg.maxMsg3Tx < RGSCH_MIN_HQ_TX) ||
         (cellCfg->rachCfg.numRaPreamble < RGSCH_MIN_NUM_RA_PREAMBLE) ||
         (cellCfg->rachCfg.numRaPreamble > RGSCH_MAX_NUM_RA_PREAMBLE) ||
         (cellCfg->rachCfg.sizeRaPreambleGrpA >
          cellCfg->rachCfg.numRaPreamble) || 
         (cellCfg->rachCfg.prachResource > 
          (cellCfg->bwCfg.ulTotalBw - RGSCH_NUM_RA_RB)))
   {
      RLOG_ARG3(L_ERROR,DBG_CELLID,cellCfg->cellId,"Invalid RACH configuration:"
                "preamble Fmt %d raWinSize %d maxMsg3Tx %d",
                cellCfg->rachCfg.preambleFormat, cellCfg->rachCfg.raWinSize,
                cellCfg->rachCfg.maxMsg3Tx);
      RLOG_ARG2(L_ERROR,DBG_CELLID,cellCfg->cellId,"Invalid numRaPreamble %d sizeRaPreambleGrpA %d",
                cellCfg->rachCfg.numRaPreamble,
                cellCfg->rachCfg.sizeRaPreambleGrpA);
      RETVALUE(RFAILED);
   }
   /* RACHO */
   /* verify that the ded Preambles cfgd for Pdcch Order
    * do not collide with that of non-dedicated  and validates against
    * the configuration index and number of RACH
    * ded-preambles. For non-zero ded preamble cfg, 
    * the config index is expected to be != NA.*/
   if ((cellCfg->macPreambleSet.pres) && 
       ((cellCfg->macPreambleSet.start < cellCfg->rachCfg.numRaPreamble) ||
       (cellCfg->macPreambleSet.start >= RGSCH_MAX_NUM_RA_PREAMBLE) ||
       (cellCfg->macPreambleSet.size < 1) ||
       (cellCfg->macPreambleSet.size > RGSCH_MAX_NUM_RA_PREAMBLE-
                                       cellCfg->rachCfg.numRaPreamble) ||
       (cellCfg->rachCfg.raOccasion.sfnEnum == RGR_SFN_NA)))
   {
      RLOG_ARG2(L_ERROR,DBG_CELLID,cellCfg->cellId,"Invalid RACH Preambleset conf:"
                "preambleSet Start %d preambleSet Size %d",
                cellCfg->macPreambleSet.start, cellCfg->macPreambleSet.size);
      RETVALUE(RFAILED);
   }
#ifdef RGR_V1
   if(cellCfg->rachCfg.contResTmr)
   {
      U8 idx;
#ifdef LTE_TDD
      idx = cellCfg->ulDlCfgIdx;
#else
      idx = 7; /* FDD */
#endif
      /* maxMsg4TxDelay = (HARQ_RTT * MAX_MSG4_HARQ_RETX)  + 
         3 TTI (MAX L1+L2 processing delay at the UE) */
      U8 maxMsg4TxDelay = (cellCfg->dlHqCfg.maxMsg4HqTx-1) * 
         rgSchCmnHarqRtt[idx] + 3; 


      if(maxMsg4TxDelay >= cellCfg->rachCfg.contResTmr)
      {
         RLOG_ARG2(L_WARNING,DBG_CELLID,cellCfg->cellId ,
               "Warining !: Contention Resolution timer not greater than the "
               "guard timer. Conte Res timer %d Guard timer %d",
               cellCfg->rachCfg.contResTmr, 
               maxMsg4TxDelay );
         /* [ccpu00138532]-DEL- removed return fail here as it is ok if the 
            max Msg4 Tx delay is more than the contension Resolution timer. 
            In such case, the CRI CE will be scheduled immediately once 
            msg3 is received */ 
      }
   }
   else
   {
      /* ccpu00128575 ADD - If contention resolution timer is configured as 0,
         Then return fail*/
      RLOG_ARG0(L_ERROR,DBG_CELLID,cellCfg->cellId, 
               "Contention Resolution timer is configured as '0'");
      RETVALUE(RFAILED);
   }
#endif

   RETVALUE(ROK);
}

/**
 * @brief Validates the UE configuration request from RRM to MAC.
 *
 * @details
 *
 *     Function : rgSCHCfgVldtRgrUeCfg
 *
 *     Processing Steps:
 *      - Validate the UE configuration request from RRC to MAC at CFG:
 *        validate the value range for the configured values.
 *      - If validated successfully,
 *        - Return ROK and pointer to the cell of UE.
 *      - Else 
 *        - Return RFAILED.
 *
 *  @param[in]  Inst         inst
 *  @param[in]  RgrUeCfg     *ueCfg
 *  @param[out] RgSchCellCb  **cell
 *  @param[out] RgSchErrInfo *errInfo
 *  @return  S16
 *      -# ROK
 *      -# RFAILED
 **/
#ifdef ANSI
PUBLIC S16 rgSCHCfgVldtRgrUeCfg
(
Inst         inst,
RgrUeCfg     *ueCfg,
RgSchCellCb  **cell,
RgSchErrInfo *errInfo
)
#else
PUBLIC S16 rgSCHCfgVldtRgrUeCfg(inst, ueCfg, cell, errInfo)
Inst         inst;
RgrUeCfg     *ueCfg;
RgSchCellCb  **cell;
RgSchErrInfo *errInfo;
#endif
{
   TRC2(rgSCHCfgVldtRgrUeCfg);

   errInfo->errCause = RGSCHERR_CFG_INVALID_RGR_UE_CFG;

   if (((*cell) == NULLP) || 
       ((*cell)->cellId != ueCfg->cellId))
   {
      RLOG_ARG1(L_ERROR,DBG_CRNTI,ueCfg->crnti,"Cell does not exist %d", 
           ueCfg->cellId);
      RETVALUE(RFAILED);
   }
   /* RACHO: 
    * Check configured preamble id not colliding with non dedicated or PDCCH
    * order preamble sets. When valid preamble id given check that C-RNTI given
    * in configuration is not amongst the C-RNTI'smanaged by scheduler */
   if ((rgSCHRamVldtUeCfg(*cell, ueCfg)) != ROK)
   {
      RLOG_ARG1(L_ERROR,DBG_CRNTI,ueCfg->crnti,"CELLID:%d Preamble Id configuration"
               "failed ",ueCfg->cellId);
      RETVALUE(RFAILED);
   }
   /* Check if Ue already configured */
   if (rgSCHDbmGetUeCb(*cell, ueCfg->crnti) != NULLP)
   {
      RLOG_ARG1(L_ERROR,DBG_CRNTI,ueCfg->crnti,"CELLID:%d UEID already exists",
             ueCfg->cellId);
      RETVALUE(RFAILED);
   }
   /* Validate Transmission UE modes  */
   if ((ueCfg->txMode.pres == TRUE) && ((ueCfg->txMode.txModeEnum < RGR_UE_TM_1)
            || (ueCfg->txMode.txModeEnum > RGR_UE_TM_7)))
   {
      RLOG_ARG2(L_ERROR,DBG_CRNTI,ueCfg->crnti,"CELLID:%d Invalid transmission mode for"
               " UE is %d", ueCfg->cellId,(U8)ueCfg->txMode.txModeEnum);
      RETVALUE(RFAILED);
   }
  
   /* Validate UE Category */
   if (ueCfg->ueCatEnum > CM_LTE_UE_CAT_5)
   {
      RLOG_ARG2(L_ERROR,DBG_CRNTI,ueCfg->crnti,"CELLID:%d Invalid category for UE %d",
               ueCfg->cellId,(U8)ueCfg->ueCatEnum);
      RETVALUE(RFAILED);
   }
  
   if ((*cell)->numTxAntPorts == 1)
   {
      if ((ueCfg->txMode.pres == TRUE) && 
            (ueCfg->txMode.txModeEnum > RGR_UE_TM_1))
      {
         RLOG_ARG2(L_ERROR,DBG_CRNTI,ueCfg->crnti,"CELLID:%d Invalid transmission mode for"
                  " UE (%d) for the configured Cell Antenna Ports",ueCfg->cellId,
                  (U8)ueCfg->txMode.txModeEnum);
         RETVALUE(RFAILED);
      }
   }
   
   if ((rgSCHCfgVldtUeCqiModeCfg(*cell, ueCfg)) != ROK)
   {
      RLOG_ARG1(L_ERROR,DBG_CRNTI,ueCfg->crnti, "CELLID:%d Invalid CQI Mode configuration",
                ueCfg->cellId);
      RETVALUE(RFAILED);
   }

   /* Validate Max Uplink HARQ transmission value */
   if (ueCfg->ueUlHqCfg.maxUlHqTx < RGSCH_MIN_HQ_TX)
   {
      RLOG_ARG2(L_ERROR,DBG_CRNTI,ueCfg->crnti,"CELLID:%d Invalid Uplink HARQ config for "
            "UE %d",ueCfg->cellId,ueCfg->ueUlHqCfg.maxUlHqTx);
      RETVALUE(RFAILED);
   }

   if (rgSCHCfgVldtUePwrCfg(*cell, &ueCfg->ueUlPwrCfg) != ROK)
   {
      RLOG_ARG1(L_ERROR,DBG_CRNTI,ueCfg->crnti,"CELLID:%d Invalid PUSCH Group power"
           " configuration",ueCfg->cellId);
      RETVALUE(RFAILED);
   }
 
   if (rgSCHCfgVldtUeMeasGapAckNakRepCfg(*cell, ueCfg) != ROK)
   {
      RLOG_ARG1(L_ERROR,DBG_CRNTI,ueCfg->crnti,"CELLID:%d Invalid MeasGap/AckNackRep"
            " configuration",ueCfg->cellId);
      RETVALUE(RFAILED);
   }
  
#ifdef LTEMAC_SPS
   /* Validating SPS RNTI */
   if (((ueCfg->ueSpsCfg.spsRnti >=  (*cell)->rntiDb.rntiStart) && 
       (ueCfg->ueSpsCfg.spsRnti<=((*cell)->rntiDb.rntiStart+(*cell)->rntiDb.maxRntis)))
       ||(ueCfg->ueSpsCfg.spsRnti == RGSCH_SI_RNTI) 
       ||(ueCfg->ueSpsCfg.spsRnti == RGSCH_P_RNTI))
   {
      RLOG_ARG1(L_ERROR,DBG_CRNTI,ueCfg->crnti,"CELLID:%d Invalid SPS RNTI "
               " in DL SPS Config",ueCfg->cellId);
      RETVALUE(RFAILED);
   }

   if (ueCfg->ueSpsCfg.dlSpsCfg.isDlSpsEnabled) 
   {
      if (rgSCHCfgVldtUeDlSpsCfg(*cell, &ueCfg->ueSpsCfg.dlSpsCfg) != ROK)
      {
         RLOG_ARG1(L_ERROR,DBG_CRNTI,ueCfg->crnti,"CELLID:%d Invalid DL SPS configuration"
                  " for the UE",ueCfg->cellId);
         RETVALUE(RFAILED);
      }
   }
#endif 
#ifdef TFU_UPGRADE
   /* Validated Periodic CQI/PMI, RI , SRS and SR related UeCfg */
   if ( ROK != rgSCHCfgVldtCqiSrSrsUeCfg(*cell, ueCfg, errInfo))
   {
      RLOG_ARG1(L_ERROR,DBG_CRNTI,ueCfg->crnti,"CELLID:%d Invalid Periodic CQI/SR/SRS"
               "configuration",ueCfg->cellId);
      RETVALUE(RFAILED);
   }
#endif

   /* Validate DRX specific parameters */
   if ( ROK != rgSCHCfgVldtDrxUeCfg(*cell, &(ueCfg->ueDrxCfg)))
   {
      RLOG_ARG1(L_ERROR,DBG_CRNTI,ueCfg->crnti,"CELLID:%d Invalid DRX configuration",
                ueCfg->cellId);
      RETVALUE(RFAILED);
   }

/* ccpu00117452 - MOD - Changed macro name from
   RGR_RRM_DLPWR_CNTRL to RGR_CQI_REPT */
#ifdef RGR_CQI_REPT
   if (ueCfg->ueCqiReptCfg.numColltdCqiRept > RGR_CQIRPTS_MAXN)
   {
      RLOG_ARG2(L_ERROR,DBG_CRNTI,ueCfg->crnti,"CELLID:%d Invalid numColltdCqiRept,"
               "MAX supported %d",RGR_CQIRPTS_MAXN,ueCfg->cellId);
      RETVALUE(RFAILED);
   }
#endif /* End of RGR_CQI_REPT */

   errInfo->errCause = RGSCHERR_NONE;
   RETVALUE(ROK);
}  /* rgSCHCfgVldtRgrUeCfg */


/**
 * @brief Validates the cell reconfiguration request from RRM to MAC.
 *
 * @details
 *
 *     Function : rgSCHCfgVldtRgrCellRecfg
 *
 *     Processing Steps:
 *      - Retrieve the cell control block.
 *      - If successful,
 *        - Validate the range of reconfigured values recieved in
 *          re-configuration request.
 *        - If validated successfully,
 *          - Return ROK and pointer to the cell.
 *        - Else 
 *          - Return RFAILED.
 *      - Else return RFAILED.
 *
 *  @param[in]  Inst          inst
 *  @param[in]  RgrCellRecfg  *cellRecfg
 *  @param[out] RgSchCellCb   **cell
 *  @param[out] RgSchErrInfo  *errInfo
 *  @return  S16
 *      -# ROK
 *      -# RFAILED
 **/
#ifdef ANSI
PUBLIC S16 rgSCHCfgVldtRgrCellRecfg
(
Inst          inst,
RgrCellRecfg  *cellRecfg,
RgSchCellCb   **cell,
RgSchErrInfo  *errInfo
)
#else
PUBLIC S16 rgSCHCfgVldtRgrCellRecfg(inst, cellRecfg, cell, errInfo)
Inst          inst;
RgrCellRecfg  *cellRecfg;
RgSchCellCb   **cell;
RgSchErrInfo  *errInfo;
#endif
{
   TRC2(rgSCHCfgVldtRgrCellRecfg);


   errInfo->errCause = RGSCHERR_CFG_INVALID_RGR_CELL_RECFG;
   
   /* Fetch cell and validate cell Id with the cell control block*/
   if (((*cell) == NULLP) ||
       ((*cell)->cellId != cellRecfg->cellId)) 
   {
      RLOG_ARG0(L_ERROR,DBG_CELLID,cellRecfg->cellId,"Cell control block does not exist");
      RETVALUE(RFAILED);
   }

   /* Validate recieved values */
   if ((cellRecfg->recfgTypes & RGR_CELL_DL_HARQ_RECFG) &&
         ((cellRecfg->dlHqRecfg.maxDlHqTx < RGSCH_MIN_HQ_TX) ||
          (cellRecfg->dlHqRecfg.maxMsg4HqTx < RGSCH_MIN_HQ_TX)))
   {
      RLOG_ARG2(L_ERROR,DBG_CELLID,cellRecfg->cellId,"Invalid Downlink HARQ configuration:"
           " maxDlHqTx %d maxMsg4HqTx %d", cellRecfg->dlHqRecfg.maxDlHqTx,
            cellRecfg->dlHqRecfg.maxMsg4HqTx);
      RETVALUE(RFAILED);
   }
   if ((cellRecfg->recfgTypes & RGR_CELL_CFI_RECFG) && 
         ((cellRecfg->cfiRecfg.cfi < RGSCH_MIN_CFI_VAL) ||
          (cellRecfg->cfiRecfg.cfi > RGSCH_MAX_CFI_VAL)))
   {
      RLOG_ARG1(L_ERROR,DBG_CELLID,cellRecfg->cellId,"Invalid CFI configuration %d",
               cellRecfg->cfiRecfg.cfi);
      RETVALUE(RFAILED);
   }
   if (cellRecfg->recfgTypes & RGR_CELL_PUCCH_RECFG)
   {
      /* ccpu00138567- Removing validation check for resourceSize as 0.
       * From the spec, n2RB value 0 is a valid config. */
      if ((cellRecfg->pucchRecfg.n1PucchAn == 0) ||
          (cellRecfg->pucchRecfg.resourceSize >= (*cell)->bwCfg.ulTotalBw/2)||
          ((cellRecfg->pucchRecfg.deltaShift < RGSCH_PUCCH_MINVAL_DS) || 
           (cellRecfg->pucchRecfg.deltaShift > RGSCH_PUCCH_MAXVAL_DS)) ||
          (cellRecfg->pucchRecfg.cyclicShift > RGSCH_PUCCH_MAXVAL_CS))
      {
         RLOG_ARG4(L_ERROR,DBG_CELLID,cellRecfg->cellId,"Invalid PUCCH configuration: "
               "N2_RB %d N1_PUCCH %d deltaShift %d cyclicShift %d",
               cellRecfg->pucchRecfg.resourceSize,
               cellRecfg->pucchRecfg.n1PucchAn,
               cellRecfg->pucchRecfg.deltaShift,
               cellRecfg->pucchRecfg.cyclicShift);
         RETVALUE(RFAILED);
      }
   }
   if (cellRecfg->recfgTypes & RGR_CELL_SRS_RECFG)
   {
      if (cellRecfg->srsRecfg.isSrsCfgSetup && cellRecfg->srsRecfg.srsBwEnum > RGR_SRS_BWCFG_7)
      {
         RLOG_ARG1(L_ERROR,DBG_CELLID,cellRecfg->cellId,"Invalid SRS configuration: "
               "srsBw %d", (U8)cellRecfg->srsRecfg.srsBwEnum);
         RETVALUE(RFAILED);
      }

      /*ccpu00116923 - ADD - Srs Present support - Start*/
#ifdef TFU_UPGRADE
#ifdef LTE_TDD
      if(cellRecfg->srsRecfg.isSrsCfgSetup && cellRecfg->srsRecfg.srsSubFrameCfg > 13)
#else
         if(cellRecfg->srsRecfg.isSrsCfgSetup && cellRecfg->srsRecfg.srsSubFrameCfg > 14)
#endif
         {
            RLOG_ARG0(L_ERROR,DBG_CELLID,cellRecfg->cellId,"Invalid Subframe configuration ");
            RETVALUE(RFAILED);
         }
#endif
      /*ccpu00116923 - ADD - Srs Present support - End*/
   }
   if (cellRecfg->recfgTypes & RGR_CELL_RACH_RECFG)
   {
      if ((cellRecfg->rachRecfg.preambleFormat > RGSCH_MAX_RA_PREAMBLE_FMT) ||
            ((cellRecfg->rachRecfg.raWinSize < RGSCH_MIN_RA_WINSIZE) ||
             (cellRecfg->rachRecfg.raWinSize > RGSCH_MAX_RA_WINSIZE)) ||
            (cellRecfg->rachRecfg.maxMsg3Tx < RGSCH_MIN_HQ_TX) ||
            ((cellRecfg->rachRecfg.numRaPreamble < RGSCH_MIN_NUM_RA_PREAMBLE)
            || (cellRecfg->rachRecfg.numRaPreamble > RGSCH_MAX_NUM_RA_PREAMBLE))
            || (cellRecfg->rachRecfg.sizeRaPreambleGrpA >
               cellRecfg->rachRecfg.numRaPreamble) ||
            (cellRecfg->rachRecfg.prachResource >
               (*cell)->bwCfg.ulTotalBw - RGSCH_NUM_RA_RB))
      {
         RLOG_ARG3(L_ERROR,DBG_CELLID,cellRecfg->cellId,"Invalid RACH configuration:"
               " preamble Fmt %d raWinSize %d maxMsg3Tx %d",
               cellRecfg->rachRecfg.preambleFormat,
               cellRecfg->rachRecfg.raWinSize,
               cellRecfg->rachRecfg.maxMsg3Tx);
         RLOG_ARG2(L_ERROR,DBG_CELLID,cellRecfg->cellId,"Invalid RACH configuration:"
               "numRaPreamble %d sizeRaPreambleGrpA %d",
               cellRecfg->rachRecfg.numRaPreamble,
               cellRecfg->rachRecfg.sizeRaPreambleGrpA);
         RETVALUE(RFAILED);
      }
   }

#ifdef RGR_SI_SCH
   if (cellRecfg->recfgTypes & RGR_CELL_SI_RECFG)
   {
      if ((rgSCHCfgVldtRgrCellSiCfg(inst, &(cellRecfg->siReCfg))) != ROK)
      {
         RLOG_ARG0(L_ERROR,DBG_CELLID,cellRecfg->cellId,"Validation for SI"
                          "Re-configuration failed");
         RETVALUE(RFAILED);
      }
   }
#endif /*RGR_SI_SCH */

/* LTE_ADV_FLAG_REMOVED_START */
   if (cellRecfg->recfgTypes & RGR_CELL_LTEA_FEATURE_RECFG)
   {
      /* Checkin whether DSFR is enbaled without enabling SFR.
       * So we need to check if SFR is enabled along with DSFR
       * in the same reconfiguration or it is already enabled earlier*/
      if((cellRecfg->rgrLteAdvCfg.pres & RGR_DSFR) && 
         (RGR_ENABLE == cellRecfg->rgrLteAdvCfg.dsfrCfg.status)) 
      {   
         if(!(((cellRecfg->rgrLteAdvCfg.pres & RGR_SFR) &&
               (RGR_ENABLE == cellRecfg->rgrLteAdvCfg.sfrCfg.status)) || 
               ((*cell)->lteAdvCb.sfrCfg.status == RGR_ENABLE)))
         {
            RLOG_ARG0(L_ERROR,DBG_CELLID,cellRecfg->cellId,"DSFR is enbaled"
                     "Without enabling SFR");
            RETVALUE(RFAILED);
         }
      }
      if ((rgSCHCfgVldtRgrCellLteAdvCfg(inst, &(cellRecfg->rgrLteAdvCfg),
                                       (*cell)->bwCfg.dlTotalBw)) != ROK)
      {
         RLOG_ARG0(L_ERROR,DBG_CELLID,cellRecfg->cellId,"Validation for Lte Adv"
                  "Re-configuration failed");
         RETVALUE(RFAILED);
      }
   }
/* LTE_ADV_FLAG_REMOVED_END */

   /* Validating minimum resource for non-CSG users */
   if (cellRecfg->recfgTypes & RGR_CELL_CSG_PARAM_RECFG)
   {
     if (cellRecfg->csgParamCfg.minDlResNonCsg > 100)
     {
         RLOG_ARG0(L_ERROR,DBG_CELLID,cellRecfg->cellId,"Invalid Configuration "
               "of minimum DL resources for NON-CSG");
         RETVALUE(RFAILED);
     }
     if (cellRecfg->csgParamCfg.minUlResNonCsg > 100)
     {
         RLOG_ARG0(L_ERROR,DBG_CELLID,cellRecfg->cellId,"Invalid Configuration "
               "of minimum UL resources for NON-CSG");
         RETVALUE(RFAILED);
     }
   }
   errInfo->errCause = RGSCHERR_NONE;
   RETVALUE(ROK);
}  /* rgSCHCfgVldtRgrCellRecfg */

/**
 * @brief Validates the UE reconfiguration request from RRC to MAC.
 *
 * @details
 *
 *     Function : rgSCHCfgVldtRgrUeRecfg
 *
 *     Processing Steps:
 *      - Retrieve the UE control block.
 *      - If successful,
 *        - Validate the range of reconfigured values recieved in
 *          re-configuration request.
 *        - If validated successfully,
 *          - Return ROK and pointer to the cell and ue.
 *        - Else 
 *          - Return RFAILED.
 *      - Else return RFAILED.
 *
 *  @param[in]  Inst          inst
 *  @param[in]  RgrUeRecfg    *ueRecfg
 *  @param[out] RgSchCellCb   **cell
 *  @param[out] RgSchUeCb     **ue
 *  @param[out] RgSchErrInfo  *errInfo
 *  @return  S16
 *      -# ROK
 *      -# RFAILED
 **/
#ifdef ANSI
PUBLIC S16 rgSCHCfgVldtRgrUeRecfg
(
Inst           inst,
RgrUeRecfg     *ueRecfg,
RgSchCellCb    **cell,
RgSchUeCb      **ue,
RgSchErrInfo   *errInfo
)
#else
PUBLIC S16 rgSCHCfgVldtRgrUeRecfg(inst, ueRecfg, cell, ue, errInfo)
Inst           inst;
RgrUeRecfg     *ueRecfg;
RgSchCellCb    **cell;
RgSchUeCb      **ue;
RgSchErrInfo   *errInfo;
#endif
{

   TRC2(rgSCHCfgVldtRgrUeRecfg);

   errInfo->errCause = RGSCHERR_CFG_INVALID_RGR_UE_RECFG;
   
   if (((*cell) == NULLP) ||
       ((*cell)->cellId != ueRecfg->cellId))
   {
      RLOG_ARG1(L_ERROR,DBG_CELLID,ueRecfg->cellId,
                "Cell does not exist for OLD CRNTI:%d",ueRecfg->oldCrnti);
      RETVALUE(RFAILED);
   }

   /* Fetch the Old Ue */
   if ((*ue = rgSCHDbmGetUeCb(*cell, ueRecfg->oldCrnti)) == NULLP)
   {
      RLOG_ARG1(L_ERROR,DBG_CELLID,ueRecfg->cellId,"OLD CRNTI:%d does not exist", 
            ueRecfg->oldCrnti);
      RETVALUE(RFAILED);
   }

   if (ueRecfg->oldCrnti != ueRecfg->newCrnti)
   {
      if (rgSCHDbmGetUeCb(*cell, ueRecfg->newCrnti) != NULLP)
      {
         RLOG_ARG1(L_ERROR,DBG_CELLID,ueRecfg->cellId,"NEW CRNTI:%d already exists", 
              ueRecfg->newCrnti);
         RETVALUE(RFAILED);
      }
   }

   if ((ueRecfg->ueRecfgTypes & RGR_UE_CSG_PARAM_RECFG) &&\
       ((*ue)->csgMmbrSta == ueRecfg->csgMmbrSta))
   {
      RLOG_ARG2(L_ERROR,DBG_CELLID,ueRecfg->cellId,"UE ID [%d] invalid CSG Membership reconfig :%d ", 
              ueRecfg->newCrnti, (U8)ueRecfg->csgMmbrSta);
      RETVALUE(RFAILED);
   }
   /* Validate values */
   if ((ueRecfg->ueRecfgTypes & RGR_UE_TXMODE_RECFG) &&
         (ueRecfg->txMode.pres == TRUE) &&
         ((ueRecfg->txMode.txModeEnum < RGR_UE_TM_1) ||
          (ueRecfg->txMode.txModeEnum > RGR_UE_TM_7)))
   {
      RLOG_ARG2(L_ERROR,DBG_CELLID,ueRecfg->cellId,"Invalid transmission mode %d"
            "for NEW CRNTI:%d", (U8)ueRecfg->txMode.txModeEnum,ueRecfg->newCrnti);
      RETVALUE(RFAILED);
   }
#ifndef TFU_UPGRADE
   if ((ueRecfg->ueRecfgTypes & RGR_UE_PRD_DLCQI_RECFG) &&
         (((ueRecfg->prdDlCqiRecfg.k < 1) || (ueRecfg->prdDlCqiRecfg.k > 4)) ||
         ((ueRecfg->prdDlCqiRecfg.cqiPmiCfgIdx < 1) || 
         (ueRecfg->prdDlCqiRecfg.cqiPmiCfgIdx > 1024))))
   {
      RLOG_ARG2(L_ERROR,DBG_CELLID,ueRecfg->cellId,"Invalid Periodic CQI INFO"
               "OLD CRNTI:%d NEW CRNTI:%d",(U8)ueRecfg->oldCrnti,ueRecfg->newCrnti);
      RETVALUE(RFAILED);
   }
#endif
   if ((ueRecfg->ueRecfgTypes & RGR_UE_ULHARQ_RECFG) &&
         (ueRecfg->ueUlHqRecfg.maxUlHqTx < RGSCH_MIN_HQ_TX))
   {
      RLOG_ARG2(L_ERROR,DBG_CELLID,ueRecfg->cellId,"Invalid Uplink HARQ config %d"
          "for NEW CRNTI:%d", ueRecfg->ueUlHqRecfg.maxUlHqTx,ueRecfg->newCrnti);
      RETVALUE(RFAILED);
   }
#ifndef TFU_UPGRADE
   if ((ueRecfg->ueRecfgTypes & RGR_UE_PRD_DLCQI_RECFG) &&
         (ueRecfg->prdDlCqiRecfg.prdModeEnum > RGR_PRD_CQI_MOD21))
   {
      RLOG_ARG2(L_ERROR,DBG_CELLID,ueRecfg->cellId,"Invalid periodic mode config for"
          " DL CQI %d NEW CRNTI:%d", (U8)ueRecfg->prdDlCqiRecfg.prdModeEnum,ueRecfg->newCrnti);
      RETVALUE(RFAILED);
   }
#else
   if ((ueRecfg->ueRecfgTypes & RGR_UE_PCQI_RECFG) &&
       (ueRecfg->cqiCfg.cqiSetup.prdModeEnum > RGR_PRD_CQI_MOD21))
   {
     RLOG_ARG2(L_ERROR,DBG_CELLID,ueRecfg->cellId,"Invalid periodic mode config for "
            "DL CQI %d for NEW CRNTI:%d",(U8)ueRecfg->cqiCfg.cqiSetup.prdModeEnum,ueRecfg->newCrnti);
     RETVALUE(RFAILED);
   }
#endif /* TFU_UPGRADE */
   /* Validate UE Category */
   if (ueRecfg->ueCatEnum > CM_LTE_UE_CAT_5)
   {
      RLOG_ARG2(L_ERROR,DBG_CELLID,ueRecfg->cellId,"Invalid category %d for NEW CRNTI:%d",
               (U8)ueRecfg->ueCatEnum,ueRecfg->newCrnti);
      RETVALUE(RFAILED);
   }

   if ((ueRecfg->ueRecfgTypes & RGR_UE_APRD_DLCQI_RECFG) && 
         ((ueRecfg->aprdDlCqiRecfg.pres == TRUE) &&
          ((ueRecfg->aprdDlCqiRecfg.aprdModeEnum > RGR_APRD_CQI_MOD31) ||
          (*cell)->bwCfg.dlTotalBw <= 7)))
   {
      RLOG_ARG2(L_ERROR,DBG_CELLID,ueRecfg->cellId,"Invalid aperiodic mode config for"
          " DL CQI %d for NEW CRNTI:%d", (U8)ueRecfg->aprdDlCqiRecfg.aprdModeEnum,ueRecfg->newCrnti);
      RETVALUE(RFAILED);
   }
   if ((ueRecfg->ueRecfgTypes & RGR_UE_ULPWR_RECFG) &&
         (rgSCHCfgVldtUePwrCfg(*cell, &ueRecfg->ueUlPwrRecfg) != ROK))
   {
      RLOG_ARG1(L_ERROR,DBG_CELLID,ueRecfg->cellId,"Invalid PUSCH Group power"
           " Reconfiguration for NEW CRNTI:%d",ueRecfg->newCrnti);
      RETVALUE(RFAILED);
   }
 
   
   if ((ueRecfg->ueRecfgTypes & RGR_UE_ACKNACK_MEASGAP_RECFG) &&
      (rgSCHCfgVldtUeMeasGapAckNakRepRecfg(*cell, ueRecfg) != ROK))
   {
      RLOG_ARG1(L_ERROR,DBG_CELLID,ueRecfg->cellId,"Invalid MeasGap/AckNackRep"
            " Reconfiguration for NEW CRNTI:%d",ueRecfg->newCrnti);
      RETVALUE(RFAILED);
   }
#ifdef LTEMAC_SPS
   if(rgSCHCfgVldtSpsReCfg(*cell, *ue, ueRecfg)!= ROK)
   {
      RLOG_ARG1(L_ERROR,DBG_CELLID,ueRecfg->cellId,"Invalid SPS"
           " Reconfiguration for NEW CRNTI:%d",ueRecfg->newCrnti);
      RETVALUE(RFAILED);
   }
#endif
#ifdef TFU_UPGRADE
   /* Validated Periodic CQI/PMI, RI , SRS and SR related UeCfg */
   if ( ROK != rgSCHCfgVldtCqiSrSrsUeReCfg(*cell, *ue, ueRecfg,  errInfo))
   {
      RLOG_ARG1(L_ERROR,DBG_CELLID,ueRecfg->cellId,"Invalid ACQI, PCQI/SR/SRS "
               "Re-configuration for NEW CRNTI:%d",ueRecfg->newCrnti);
      RETVALUE(RFAILED);
   }
#endif
   if ((ueRecfg->ueRecfgTypes & RGR_UE_DRX_RECFG) &&
      (rgSCHCfgVldtDrxUeCfg(*cell, &(ueRecfg->ueDrxRecfg)) != ROK))
   {
      RLOG_ARG1(L_ERROR,DBG_CELLID,ueRecfg->cellId,"Invalid drxParams"
            " Reconfiguration for NEW CRNTI:%d",ueRecfg->newCrnti);
      RETVALUE(RFAILED);
   }

/* ccpu00117452 - MOD - Changed macro name from
   RGR_RRM_DLPWR_CNTRL to RGR_CQI_REPT */
#ifdef RGR_CQI_REPT
   /* Validate DL Power Control Config parameters */
   if(rgSCHCfgVldtCqiReptReCfg(*cell, ueRecfg)!= ROK)
   {
      RLOG_ARG1(L_ERROR,DBG_CELLID,ueRecfg->cellId,"Invalid DL Power Control"
           " Reconfiguration for NEW CRNTI:%d",ueRecfg->newCrnti);
      RETVALUE(RFAILED);
   }
#endif /* End of RGR_CQI_REPT */
   errInfo->errCause = RGSCHERR_NONE;
   RETVALUE(ROK);
}  /* rgSCHCfgVldtRgrUeRecfg */


/**
 * @brief Validates the logical channel reconfiguration request from
 * RRC to MAC.
 *
 * @details
 *
 *     Function : rgSCHCfgVldtRgrLchRecfg
 *
 *     Processing Steps:
 *      - Retrieve the uplink and downlink logical channel control block.
 *      - If successful,
 *        - Validate the range of reconfigured values recieved in
 *          re-configuration request.
 *        - If validated successfully,
 *          - Return ROK and pointer to the cell, UE and logical channel.
 *        - Else 
 *          - Return RFAILED.
 *      - Else return RFAILED.
 *
 *  @param[in]  RgrLchRecfg     *lcRecfg
 *  @param[out] RgSchCellCb     **cell
 *  @param[out] RgSchUeCb       **ue
 *  @param[out] RgSchUlLcCb     **ulLc
 *  @param[out] RgSchDlLcCb     **dlLc
 *  @param[out] RgSchErrInfo       *errInfo
 *  @return  S16
 *      -# ROK
 *      -# RFAILED
 **/
#ifdef ANSI
PUBLIC S16 rgSCHCfgVldtRgrLchRecfg
(
Inst           inst,
RgrLchRecfg    *lcRecfg,
RgSchCellCb    **cell,
RgSchUeCb      **ue,
RgSchDlLcCb    **dlLc,
RgSchErrInfo   *errInfo
)
#else
PUBLIC S16 rgSCHCfgVldtRgrLchRecfg(inst, lcRecfg, cell, ue, dlLc, errInfo)
Inst            inst;
RgrLchRecfg     *lcRecfg;
RgSchCellCb     **cell;
RgSchUeCb       **ue;
RgSchDlLcCb     **dlLc;
RgSchErrInfo    *errInfo;
#endif
{
   TRC2(rgSCHCfgVldtRgrLchRecfg);

   errInfo->errCause = RGSCHERR_CFG_INVALID_RGR_LC_RECFG;

   if (((*cell) == NULLP) ||
       ((*cell)->cellId != lcRecfg->cellId))
   {
      RLOG_ARG2(L_ERROR,DBG_CELLID,lcRecfg->cellId,"Cell does not exist " 
                "for CRNTI:%d LCID:%d",lcRecfg->crnti,lcRecfg->lcId);
      RETVALUE(RFAILED);
   }

   /* Fetch the Ue for dedicated channels */
   if ((*ue = rgSCHDbmGetUeCb(*cell, lcRecfg->crnti)) == NULLP)
   {
      RLOG_ARG2(L_ERROR,DBG_CELLID,lcRecfg->cellId,"UEID does not exist"
         "dedicated logical channel for CRNTI:%d LCID:%d",lcRecfg->crnti,lcRecfg->lcId);
      RETVALUE(RFAILED);
   }
   
   if ((*dlLc = rgSCHDbmGetDlDedLcCb((*ue), lcRecfg->lcId)) == NULLP)
   {
      RLOG_ARG2(L_ERROR,DBG_CELLID,lcRecfg->cellId,"Dedicated DL LC does not "
               "exist for CRNTI:%d LCID:%d",lcRecfg->crnti,lcRecfg->lcId);
      RETVALUE(RFAILED);
   }

   errInfo->errCause = RGSCHERR_NONE;
   RETVALUE(ROK);
}  /* rgSCHCfgVldtRgrLchRecfg */

/**
 * @brief Validates the UE Reset request from RRM to MAC.
 *
 * @details
 *
 *     Function : rgSCHCfgVldtRgrUeReset
 *
 *     Processing Steps:
 *      - Retrieve the CELL control block
 *      - If cell does not exist return RFAILED
 *      - Retrieve UE Control block
 *      - If UE does not exist return RFAILED
 *      - Return ROK
 *
 *  @param[in]  Inst        inst
 *  @param[in]  RgrRst      *reset
 *  @param[out] RgSchCellCb **cell
 *  @param[out] RgSchUeCb      **ue
 *  @param[out] RgErrInfo   *errInfo
 *  @return  S16
 *      -# ROK
 *      -# RFAILED
 **/
#ifdef ANSI
PUBLIC S16 rgSCHCfgVldtRgrUeReset
(
Inst         inst,
RgrRst       *reset,
RgSchCellCb  *cell,
RgSchUeCb    **ue,
RgSchErrInfo *errInfo
)
#else
PUBLIC S16 rgSCHCfgVldtRgrUeReset(inst, reset, cell, ue, errInfo)
Inst         inst;
RgrRst       *reset;
RgSchCellCb  *cell;
RgSchUeCb    **ue;
RgSchErrInfo *errInfo;
#endif
{
   TRC2(rgSCHCfgVldtRgrUeReset);
   

   errInfo->errCause = RGSCHERR_CFG_INVALID_RGR_UE_RESET;
   
   if ((cell == NULLP) || (cell->cellId != reset->cellId))
   {
      RLOG_ARG1(L_ERROR,DBG_CELLID,reset->cellId,"CELL does not exist for CRNTI:%d",
               reset->crnti);
      RETVALUE(RFAILED);
   }
   /* Fetch the Ue  */
   if ((*ue = rgSCHDbmGetUeCb(&(*cell), reset->crnti)) == NULLP)
   {
      RLOG_ARG1(L_ERROR,DBG_CELLID,reset->cellId,"UE does not exist for CRNTI:%d",
               reset->crnti);
      RETVALUE(RFAILED);
   }

   errInfo->errCause = RGSCHERR_NONE;

   RETVALUE(ROK);
}  /* rgSCHCfgVldtRgrUeReset */
   

/**
 * @brief Validates the logical channel reconfiguration request from
 * RRC to MAC.
 *
 * @details
 *
 *     Function : rgSCHCfgVldtRgrLcgRecfg
 *
 *     Processing Steps:
 *      - Retrieve the uplink and downlink logical channel control block.
 *      - If successful,
 *        - Validate the range of reconfigured values recieved in
 *          re-configuration request.
 *        - If validated successfully,
 *          - Return ROK and pointer to the cell, UE and logical channel.
 *        - Else 
 *          - Return RFAILED.
 *      - Else return RFAILED.
 *
 *  @param[in]  RgrLchRecfg     *lcRecfg
 *  @param[out] RgSchCellCb     **cell
 *  @param[out] RgSchUeCb       **ue
 *  @param[out] RgSchErrInfo    *errInfo
 *  @return  S16
 *      -# ROK
 *      -# RFAILED
 **/
#ifdef ANSI
PUBLIC S16 rgSCHCfgVldtRgrLcgRecfg
(
Inst           inst,
RgrLcgRecfg    *lcgRecfg,
RgSchCellCb    *cell,
RgSchUeCb      **ue,
RgSchErrInfo   *errInfo
)
#else
PUBLIC S16 rgSCHCfgVldtRgrLcgRecfg(inst, lcgRecfg, cell, ue, errInfo)
Inst            inst;
RgrLcgRecfg     *lcgRecfg;
RgSchCellCb     *cell;
RgSchUeCb       **ue;
RgSchErrInfo    *errInfo;
#endif
{
   TRC2(rgSCHCfgVldtRgrLcgRecfg);

   errInfo->errCause = RGSCHERR_CFG_INVALID_RGR_LCG_RECFG;

   if (((cell) == NULLP) ||
       ((cell)->cellId != lcgRecfg->cellId))
   {
      RLOG_ARG2(L_ERROR,DBG_CELLID,lcgRecfg->cellId,"Cell does not exist for" 
                "CRNTI:%d LCGID:%d",lcgRecfg->crnti,lcgRecfg->ulRecfg.lcgId);
      RETVALUE(RFAILED);
   }

   /* Fetch the Ue for dedicated channels */
   if ((*ue = rgSCHDbmGetUeCb(&(*cell), lcgRecfg->crnti)) == NULLP)
   {
      RLOG_ARG2(L_ERROR,DBG_CELLID,lcgRecfg->cellId,"UE does not exist for "
                "dedicated logical channel group CRNTI:%d LCGID:%d",
                lcgRecfg->crnti,lcgRecfg->ulRecfg.lcgId);
      RETVALUE(RFAILED);
   }
   if (lcgRecfg->ulRecfg.lcgId > (RGSCH_MAX_LCG_PER_UE - 1))
   {
      RLOG_ARG2(L_ERROR,DBG_CELLID,lcgRecfg->cellId,"Invalid lcgId for uplink logical"
                "channel CRNTI:%d LCGID:%d",
                lcgRecfg->crnti,lcgRecfg->ulRecfg.lcgId);
      RETVALUE(RFAILED);
   }
   
   if ((lcgRecfg->ulRecfg.gbr != 0) && (lcgRecfg->ulRecfg.mbr < lcgRecfg->ulRecfg.gbr))
   {
      RGSCHDBGINFO(inst, (rgSchPBuf(inst), "Dedicated Logical Group %d validation failed"
               " for ue %d for cell %d\n", lcgCfg->ulInfo.lcgId, lcgCfg->crnti, lcgCfg->cellId));
      RETVALUE(RFAILED);
   }
   errInfo->errCause = RGSCHERR_NONE;
   RETVALUE(ROK);
}  /* rgSCHCfgVldtRgrLcgRecfg */

/**
 *
 * @details
 *
 *     Function : rgSCHDynCfiCfg
 *
 *  @param[in]  RgSchCellCb   *cell
 *              RgrCellCfg    *cellCfg
 *  @return  S16
 *      -# ROK
 *      -# RFAILED
 **/
#ifdef ANSI
PRIVATE S16 rgSCHDynCfiCfg
(
RgSchCellCb   *cell,
RgrCellCfg    *cellCfg
)
#else
PRIVATE S16 rgSCHDynCfiCfg(cell, cellCfg)
RgSchCellCb   *cell;
RgrCellCfg    *cellCfg;
#endif  
{
   U8        cfi; 
#ifdef LTE_TDD
   U8        ulDlCfgIdx = cellCfg->ulDlCfgIdx;
   U8        mphIdx;
   U8        maxMPhich;
   U16       numDlSf;
#endif    

   TRC2(rgSCHDynCfiCfg);

   cell->dynCfiCb.isDynCfiEnb = cellCfg->isDynCfiEnb;

   /* Initializing Failure Sample Period */
   cell->dynCfiCb.failSamplePrd = (RGSCH_CFI_TTI_MON_INTRVL *
                     RGSCH_CFI_STEP_UP_TTI_PRCNTG)/100;
   /* Initializing Number of Failure Samples */
   cell->dynCfiCb.numFailSamples = (RGSCH_CFI_TTI_MON_INTRVL/
                                    cell->dynCfiCb.failSamplePrd);
   cell->dynCfiCb.maxCfi = RGSCH_MAX_CFI_VAL;
   /* Allocating memory for CCE failure average array based on 
    * monitoring interval and CCE failure sample period */
   if((rgSCHUtlAllocSBuf(cell->instIdx, (Data**)&(cell->dynCfiCb.cceFailSamples),
               (cell->dynCfiCb.numFailSamples * sizeof(U16)))) != ROK)
   {
      RLOG_ARG0(L_ERROR,DBG_CELLID,cellCfg->cellId,"Memory allocation FAILED for cell");
      RETVALUE(RFAILED);
   }

   /* Setting the Invalid value 0xFF to pdcchSfIdx, it will be assigned 
    * a valid value during CFI swithing is done */
   cell->dynCfiCb.pdcchSfIdx = 0xFF;

#ifdef LTE_TDD
   /* In case of config index 0, the mphich index can be upto 2 
    * in other config index cases, it will always be set as 1*/
   if(ulDlCfgIdx == 0)
   {
      maxMPhich = RG_SCH_MAX_MPHICH;
   }
   else
   {
      maxMPhich = RG_SCH_MAX_MPHICH -1;
   }    
   /* Calculate the number of CCEs in the cell */
   for(cfi = 1; cfi < RG_SCH_CMN_MAX_CFI; cfi++)
   {   
      for(mphIdx = 0; mphIdx < maxMPhich; mphIdx++)
      {   
         cell->dynCfiCb.cfi2NCceTbl[mphIdx][cfi] = 
            rgSCHUtlCalcNCce(cell->bwCfg.dlTotalBw,
               cell->phichCfg.ngEnum, cfi, mphIdx,
               cell->numTxAntPorts,
               cell->isCpDlExtend);
      }
   }
#else   
   /* Calculate the number of CCEs in the cell */
   for(cfi = 1; cfi < RG_SCH_CMN_MAX_CFI; cfi++)
   {   
      /* CFI Index starts from 1 so that there can be a direct mapping from 
         actual CFI value to cfi Index. mPhich index will always be set 
         as 0 for FDD */
      cell->dynCfiCb.cfi2NCceTbl[0][cfi] = 
            rgSCHUtlCalcNCce(cell->bwCfg.dlTotalBw, cell->phichCfg.ngEnum,
                  cfi, cell->numTxAntPorts, cell->isCpDlExtend);
   }

   /* Calculate the number of CCEs in the cell */
   if(cell->dynCfiCb.isDynCfiEnb == TRUE)
   {   
      /* In case if Dynamic CFI feature is enabled, default CFI 
       * value 1 is used  */
      cell->nCce = cell->dynCfiCb.cfi2NCceTbl[0][1];
   }   
   else
   {
      cell->nCce = cell->dynCfiCb.cfi2NCceTbl[0][cellCfg->cfiCfg.cfi];
   }    
#endif  

#ifdef LTE_TDD   
   numDlSf = rgSchTddNumDlSubfrmTbl[ulDlCfgIdx][9] * 
               (RGSCH_CFI_TTI_MON_INTRVL/10);
   cell->dynCfiCb.cfiStepUpTtiCnt = 
               (RGSCH_CFI_STEP_UP_TTI_PRCNTG * numDlSf)/100;
   cell->dynCfiCb.cfiStepDownTtiCnt = 
               (RGSCH_CFI_STEP_DOWN_TTI_PERCNTG * numDlSf)/100;
#else
   cell->dynCfiCb.cfiStepUpTtiCnt = (RGSCH_CFI_STEP_UP_TTI_PRCNTG * 
                                     RGSCH_CFI_TTI_MON_INTRVL)/100;
   cell->dynCfiCb.cfiStepDownTtiCnt = (RGSCH_CFI_STEP_DOWN_TTI_PERCNTG * 
                                     RGSCH_CFI_TTI_MON_INTRVL)/100;
#endif   

   RETVALUE(ROK);
}

/**
 * @brief Handler for the cell configuration request from RRM to MAC.
 *
 * @details
 *
 *     Function : rgSCHCfgRgrCellCfg
 *
 *     Processing Steps:
 *      - Invoke SCH with cell control block to update
 *        scheduler specific information.
 *      - Update cell control block with the values recieved in the
 *        configuration.
 *      - Add to the active list of cells if cell becomes ACTIVE.
 *      - If successful, return ROK else RFAILED.
 *      
 *  @param[in]  RgSchCb      *instCb
 *  @param[in]  SpId         spId
 *  @param[in]  RgSchCellCfg    *cellCfg
 *  @param[out] RgSchErrInfo *errInfo
 *  @return  S16
 *      -# ROK
 *      -# RFAILED
 **/
#ifdef ANSI
PUBLIC S16 rgSCHCfgRgrCellCfg
(
RgSchCb       *instCb,
SpId          spId,
RgrCellCfg    *cellCfg,
RgSchErrInfo  *errInfo
)
#else
PUBLIC S16 rgSCHCfgRgrCellCfg(instCb, spId, cellCfg, errInfo)
RgSchCb       *instCb;
SpId          spId;
RgrCellCfg    *cellCfg;
RgSchErrInfo  *errInfo;
#endif
{
   S16          ret;
   U8           idx;
   Pst          pst;
   RgInfCellReg cellRegReq;
   RgSchCellCb  *cell = NULLP;
   Inst         inst = instCb->rgSchInit.inst;

   TRC2(rgSCHCfgRgrCellCfg);

   errInfo->errCause = RGSCHERR_CFG_RGR_CELL_CFG;
   
   cmMemset((U8*)&pst, (U8)0, sizeof(Pst));
    
   /* Allocate the scheduler's cell control block */
   if((ret = rgSCHUtlAllocSBuf(inst, (Data**)&cell, sizeof(RgSchCellCb))) 
         != ROK)
   {
      RLOG_ARG0(L_ERROR,DBG_CELLID,cellCfg->cellId,"Memory allocation FAILED for cell");
      RETVALUE(RFAILED);
   }
   if ((U8 *)cell == NULLP)
   {
      RLOG_ARG0(L_ERROR,DBG_CELLID,cellCfg->cellId,"Memory allocation FAILED for cell");
      RETVALUE(RFAILED);
   }


   /* Initialize the lists of the cell */
   ret = rgSCHDbmInitCell(cell);
   if (ret != ROK)
   {
      RLOG_ARG0(L_ERROR,DBG_CELLID,cellCfg->cellId,"DBM initialization FAILED for cell");
      rgSCHCfgFreeCellCb(cell);
      RETVALUE(RFAILED);
   }
/* LTE_ADV_FLAG_REMOVED_START */
   if(cellCfg->rgrLteAdvCfg.pres & RGR_ABS)
   {
      cell->lteAdvCb.absCfg =
         cellCfg->rgrLteAdvCfg.absCfg;
      cmMemset((U8*)cell->lteAdvCb.absLoadInfo, 0, sizeof(U32)*RGR_ABS_PATTERN_LEN);
      cell->lteAdvCb.absLoadTtiCnt = 0;
   }

   if(cellCfg->rgrLteAdvCfg.pres & RGR_SFR)
   {
      cell->lteAdvCb.sfrCfg =
         cellCfg->rgrLteAdvCfg.sfrCfg;
   }
   if(cellCfg->rgrLteAdvCfg.pres & RGR_DSFR)
   {
      cell->lteAdvCb.dsfrCfg =
         cellCfg->rgrLteAdvCfg.dsfrCfg;
   }
/* LTE_ADV_FLAG_REMOVED_END */

   /* Initialize the cell */
   cell->cellId = cellCfg->cellId;
   cell->instIdx = inst;
   cell->macInst = cellCfg->macInst;
   cell->isCpUlExtend = cellCfg->isCpUlExtend;
   cell->isCpDlExtend = cellCfg->isCpDlExtend;
   cell->numTxAntPorts = cellCfg->numTxAntPorts;
   if(cell->numTxAntPorts == 1)
     cell->numCellRSPerSf = RGSCH_NUM_CELL_RS_ONE_ANT_PORT;
   else if(cell->numTxAntPorts == 2)
     cell->numCellRSPerSf = RGSCH_NUM_CELL_RS_TWO_ANT_PORT;
   else
     cell->numCellRSPerSf = RGSCH_NUM_CELL_RS_FOUR_ANT_PORT;
   cell->bwCfg = cellCfg->bwCfg;
   cell->pbchRbStart = ((((cell->bwCfg.dlTotalBw * 12)/2) - 36)/12); /* Ref section 6.6 in 36.211 */ 
   cell->pbchRbEnd = cell->pbchRbStart + 5;
   cell->pucchCfg = cellCfg->pucchCfg;
   cell->rachCfg = cellCfg->rachCfg;
   cell->siCfg = cellCfg->siCfg;
   cell->t300TmrVal = cellCfg->t300TmrVal;
#ifdef RGR_SI_SCH
   /*Initialize the SI CB in Cell CB */
   cmMemset((U8 *)&cell->siCb, 0, sizeof(RgSchSiCb));
#endif
   /*Fix: Added Guard Pool for RNTI which will contain RNTIs 
       *for UEs deleted from Scheduler but not yet from MAC*/
   cmLListInit(&cell->rntiDb.rntiGuardPool);

   /* Initialize the inWindow to sync with scheduler time when ticks starts */
#ifdef LTEMAC_HDFDD
      cell->siCb.inWindow = (cellCfg->siCfg.siWinSize - 
            (RG_SCH_CMN_DL_DELTA + RG_SCH_CMN_HARQ_INTERVAL));
#else
      cell->siCb.inWindow = (cellCfg->siCfg.siWinSize - 
            (RG_SCH_CMN_DL_DELTA));
#endif
     
      if(cell->siCb.inWindow < 0)
      { 
         cell->siCb.inWindow = 0;
      }
   cell->macPreambleSet = cellCfg->macPreambleSet;
   cell->phichCfg = cellCfg->phichCfg;

   /* Initialize UL and DL CCCH logical channels */
   cell->ulCcchId = RGSCH_INVALID_LC_ID;
   cell->dlCcchId = RGSCH_INVALID_LC_ID;

   /* Update SRS configuration */
   cell->srsCfg.isSrsCfgPres = cellCfg->srsCfg.isSrsCfgSetup;
   if(cellCfg->srsCfg.isSrsCfgSetup)
   {
      cell->srsCfg.srsCfgPrdEnum = cellCfg->srsCfg.srsCfgPrdEnum;
      cell->srsCfg.srsBwEnum = cellCfg->srsCfg.srsBwEnum;
      cell->srsCfg.srsTxOffst =
         rgSrsTxOffstTbl[cellCfg->srsCfg.srsSubFrameCfg];
      /*ccpu00116923 - ADD - Srs Present support */
#ifdef TFU_UPGRADE
      cell->srsCfg.srsSubFrameCfg = cellCfg->srsCfg.srsSubFrameCfg;
#endif
   }
 
   /* Update scheduler type */
   cell->ulSchdType = cellCfg->ulSchdType;
   cell->dlSchdType = cellCfg->dlSchdType;

   /* Configure all the common logical channels for the cell */
   for(idx = 0; idx < cellCfg->numCmnLcs; idx++)
   {
      /* This never returns failure and hence not checked for */
      rgSCHCfgRgrCmnLcCfg(cell, &(cellCfg->cmnLcCfg[idx]), errInfo);
   }

   /* Invoke the MeasGap and ACK NACK Rep handler for cell cfg */

   /* Dynamic CFI cell configuration */
   ret = rgSCHDynCfiCfg(cell, cellCfg);
   if(ret != ROK)
   {
      RLOG_ARG0(L_ERROR,DBG_CELLID,cellCfg->cellId,"Rgr cell Config failed at "
       "Scheduler for cell");
      rgSCHCfgFreeCellCb(cell);
      RETVALUE(RFAILED);
   }   
   /* Updating Auto TM Mode enable/diable flag */
   cell->isAutoCfgModeEnb = cellCfg->isAutoCfgModeEnb;
   /* CPU OvrLoad State Initialization */
   cell->thresholds.maxDlItbs = RG_SCH_DL_MAX_ITBS;
   cell->thresholds.maxUlItbs = RG_SCH_UL_MAX_ITBS;
   cell->measurements.dlTpt = 0;
   cell->measurements.ulTpt = 0;
   cell->measurements.dlBytesCnt = 0;
   cell->measurements.ulBytesCnt = 0;
   cell->cpuOvrLdCntrl.cpuOvrLdIns = 0; /* 0 - No command */
   cell->cpuOvrLdCntrl.dlNxtIndxDecNumUeTti = 0;
   cell->cpuOvrLdCntrl.ulNxtIndxDecNumUeTti = 0;
   for ( idx = 0; idx < 10; idx++ )
   {
      cell->cpuOvrLdCntrl.maxUeNewTxPerTti[idx] = cellCfg->maxDlUeNewTxPerTti;
      cell->cpuOvrLdCntrl.maxUeNewRxPerTti[idx] = cellCfg->maxUlUeNewTxPerTti;
   }

   /* Invoke scheduler to update scheduler specific information */
   ret =  rgSCHUtlRgrCellCfg(cell, cellCfg, errInfo);
   if (ret != ROK)
   {
      RLOG_ARG0(L_ERROR,DBG_CELLID,cellCfg->cellId,"Rgr cell Config failed at "
       "Scheduler for cell ");
      rgSCHCfgFreeCellCb(cell);
      RETVALUE(RFAILED);
   }

   /* Invoke DHM to update DHM specific information */
   rgSCHDhmRgrCellCfg(cell, cellCfg, errInfo);
   
   /* Initialize RNTI DB */
   ret = rgSCHDbmRntiDbInit(cell, cellCfg->macRnti.startRnti,
         cellCfg->macRnti.size);
   if (ret != ROK)
   {
      RLOG_ARG0(L_ERROR,DBG_CELLID,cellCfg->cellId,"Rgr Cell Config failed at"
          " RNTI DB init for cell");
      rgSCHCfgFreeCellCb(cell);
      RETVALUE(RFAILED);
   }

   /* Update the cell with recieved configuration */
   cell->dlHqCfg  =  cellCfg->dlHqCfg;

   cell->crntSfIdx = 0; 
   /* Allocate the subframe allocation information */
   if((ret = rgSCHUtlGetSfAlloc(cell)) != ROK)
   {
      RLOG_ARG0(L_ERROR,DBG_CELLID,cellCfg->cellId,"Memory allocation FAILED for "
               "cell");
      rgSCHCfgFreeCellCb(cell);
      RETVALUE(RFAILED);
   }
   /* Update RACH Related information 
    * XXX: Below function yet to be written in RAM
    * To store the preambles given in  the configuration for PDCCH order in the
    * scheduler cell control block. Initialize the PRACH Mask Index allocated
    * for these preambles to invalid values */

   cell->crntHqIdx = 0; 
   /* Allocate the subframe allocation information */
   if((ret = rgSCHUtlGetRlsHqAlloc(cell)) != ROK)
   {
      RLOG_ARG0(L_ERROR,DBG_CELLID,cellCfg->cellId,"Memory allocation FAILED for"
               "cell");
      rgSCHCfgFreeCellCb(cell);
      RETVALUE(RFAILED);
   }

   /* Associate a pair of upper and lower sapCbs with this cell */
   instCb->rgrSap[spId].cell = cell;
   instCb->tfuSap[spId].cell = cell;
   instCb->rgmSap[spId].cell = cell;
   cell->tfuSap = &(instCb->tfuSap[spId]);
   /* rg001.201: Added for sending TTI tick to RRM */
#if (defined(RGR_RRM_TICK) || defined(RGR_CQI_REPT))
   /* Associate the RGR SAP as well utilized while sending TTI
    * Ticks to RGR User. */
   cell->rgrSap = &(instCb->rgrSap[spId]);
#endif
   cell->rgmSap = &(instCb->rgmSap[spId]);
#ifdef RGR_RRM_TICK
   /* Store the periodicity configured */
   cell->rrmTtiIndPrd = cellCfg->rrmTtiIndPrd;
#endif

#ifdef LTE_L2_MEAS
   cmLListInit(&cell->l2mList);
#endif

   if (rgSCHDrxCellCfg(cell,cellCfg) != ROK)
   {
      RLOG_ARG0(L_ERROR,DBG_CELLID,cellCfg->cellId,"Drx Memory allocation FAILED for"
         " cell");
      rgSCHCfgFreeCellCb(cell);
      RETVALUE(RFAILED);
   }
   cell->overLoadBackOffEnab = FALSE;/* Disabling RachOverload by default */
   /* Updating CSG Parameters */
   cell->csgAccsMode    = cellCfg->accsMode;
   cell->minDlResNonCsg = cellCfg->csgParamCfg.minDlResNonCsg;
   cell->minUlResNonCsg = cellCfg->csgParamCfg.minUlResNonCsg;

   /* Register the cell with MAC */ 
   rgSCHUtlGetPstToLyr(&pst, instCb, cell->macInst);
   cellRegReq.cellId = cell->cellId;
   cellRegReq.cellSapId = spId;
#ifdef LTE_TDD
   cellRegReq.maxDlHqProcPerUe = rgSchTddDlNumHarqProcTbl[cellCfg->ulDlCfgIdx];
#else
   cellRegReq.maxDlHqProcPerUe = RGSCH_MAX_DL_HQ_PROC;
#endif
   RgSchMacCellReg(&pst, &cellRegReq);

#ifdef TENB_STATS
   cell->tenbStats = TSL2AllocCellStatsBlk(cell->cellId);
#endif

   errInfo->errCause = RGSCHERR_NONE;
   RETVALUE(ROK);
}  /* rgSCHCfgRgrCellCfg */

/**
 * @brief Handler for the UE configuration request from RRC to MAC.
 *
 * @details
 *
 *     Function : rgSCHCfgRgrUeCfg
 *
 *     Processing Steps:
 *      - Allocate and create UE control block.
 *      - Update UE control block with the values recieved in the
 *        configuration.
 *      - Invoke RAM, SCH, UHM and DHM with created UE control block, to
 *        update random access, scheduler, uplink harq and downlink harq
 *        specific information respectively.
 *      - If successful, add the control block to hash list of UEs for the cell
 *        else Rollback and FAIL.
 *
 *  @param[in]  RgSchCellCb  *cell
 *  @param[in]  RgrUeCfg     *ueCfg
 *  @param[out] RgSchErrInfo *errInfo
 *  @return  S16
 *      -# ROK
 *      -# RFAILED
 **/
#ifdef ANSI
PUBLIC S16 rgSCHCfgRgrUeCfg
(
RgSchCellCb  *cell,
RgrUeCfg     *ueCfg,
RgSchErrInfo *errInfo
)
#else
PUBLIC S16 rgSCHCfgRgrUeCfg(cell, ueCfg, errInfo)
RgSchCellCb  *cell;
RgrUeCfg     *ueCfg;
RgSchErrInfo *errInfo;
#endif
{
   S16          ret;
   RgSchRaCb    *raCb=NULLP;
   RgSchUeCb    *ue = NULLP;
   Inst         inst = cell->instIdx;
   U32          lcgCnt;

   TRC2(rgSCHCfgRgrUeCfg);

  do {
     errInfo->errCause = RGSCHERR_CFG_RGR_UE_CFG;
     /* RACHO : Check for raCb only if preamble Id not provded */
     if (ueCfg->dedPreambleId.pres == NOTPRSNT)
     {
        if ((raCb = rgSCHDbmGetRaCb(cell, ueCfg->crnti)) == NULLP)
        {
           RLOG_ARG1(L_ERROR,DBG_CELLID,ueCfg->cellId,"No RaCb exists for"
                    "CRNTI:%d ",ueCfg->crnti);
           break;
        }
     }

     /* Allocate the Ue control block */
     if (((rgSCHUtlAllocSBuf(inst, (Data **)&ue, sizeof(RgSchUeCb))) != ROK) ||
            ((U8 *)ue == NULLP))
     {
        RLOG_ARG1(L_ERROR,DBG_CELLID,ueCfg->cellId, "Memory allocation"
           " FAILED for CRNTI:%d", ueCfg->crnti);
        break;
     }

     /* Inititialize Ue control block */
     ue->ueId = ueCfg->crnti;
     ue->cell = cell;
     /*ccpu00117778- Initialize Transmission Indices upon UE CB creation */
#ifdef TFU_UPGRADE
     ue->cqiCb.nCqiTrIdx = RG_SCH_INVALID_IDX;
     ue->cqiCb.nRiTrIdx = RG_SCH_INVALID_IDX;
     ue->srsCb.nSrsTrIdx = RG_SCH_INVALID_IDX;
     ue->srCb.nSrTrIdx = RG_SCH_INVALID_IDX;
#endif
#ifdef LA
   ue->lastRprdAckNackTime.sfn = cell->crntTime.sfn;
   ue->lastRprdAckNackTime.subframe = cell->crntTime.subframe;
   ue->ueIdle = FALSE; 
#endif

   /* LTE_ADV_FLAG_REMOVED_START */
   /* While doing UE configuration for SFR at SCH, by default 
    * CC UE power is configured as LOW */
   ue->lteAdvUeCb.isCCUePHigh = FALSE;
   /* LTE_ADV_FLAG_REMOVED_END */

     /* Initialize the lists of the UE */
     if((rgSCHDbmInitUe(ue)) != ROK)
     {
        RLOG_ARG1(L_ERROR,DBG_CELLID,ueCfg->cellId,"DBM initialization "
           "failed for CRNTI:%d", ueCfg->crnti);
        break;
     }
     ret = rgSCHUhmHqEntInit(ue);
     if (ret != ROK)
     {
        RLOG_ARG1(L_ERROR,DBG_CELLID,ueCfg->cellId,"UHM HARQ Ent Init "
           "Failed for CRNTI:%d", ueCfg->crnti);
        break;
     }
     /* Initialize RAM related information for UE 
      * RACHO: if preamble Id is present in ueCfg then raCb will be NULL
      * so rgSCHRamRgrUeCfg should take care of creating raCb */
   if ((ueCfg->dedPreambleId.pres == NOTPRSNT) && (NULLP != raCb) )
     {
        if((rgSCHRamRgrUeCfg(cell, ue, raCb, errInfo)) != ROK)
        {
           RLOG_ARG1(L_ERROR,DBG_CELLID,ueCfg->cellId,"Random access "
              "handling config failed for CRNTI:%d", ueCfg->crnti);
           break;
        }
     }
     else /* if HO Ue */
     {
        ue->dl.hqEnt = rgSCHDhmHqEntInit(cell);
        if (ue->dl.hqEnt == NULLP)
        {
           RLOG_ARG1(L_ERROR,DBG_CELLID,ueCfg->cellId,"Hq Entity Initialization "
                    "failed in config for CRNTI:%d", ueCfg->crnti);
           break;
        }
        ue->dl.hqEnt->ue = ue;
        /* Fix : syed Assign hqEnt to UE only if msg4 is done */
        rgSCHCmnDlInitHqEnt(cell, ue);
        /* For Hand-In UE Request Aper CQI report
         * immediately */
        if (ueCfg->ueDlCqiCfg.aprdCqiCfg.pres)
        {
           ue->dl.reqForCqi = TRUE;
        }
     }

     /* Initialize lcgIds to Invalid */
     for (lcgCnt = 0; lcgCnt < RGSCH_MAX_LCG_PER_UE; lcgCnt++)
     {
        ue->ul.lcgArr[lcgCnt].lcgId = RGSCH_INVALID_LCG_ID;
     }
     /* Initialize scheduler related information for UE */
     if(rgSCHUtlRgrUeCfg(cell, ue, ueCfg, errInfo) != ROK)
     {
        RLOG_ARG1(L_ERROR,DBG_CELLID,ueCfg->cellId,"Scheduler handling "
                 "failed in config for CRNTI:%d", ueCfg->crnti);
        break;
     }

     if(raCb != NULLP)
     {
        rgSCHCfgRgrUePhrMsg3(cell,raCb,ue,errInfo);
        /* Moved this code out of rgSCHCfgRgrUePhrMsg3()
         * as it was not the appropriate place to 
         * do this. */
        if (raCb->raState == RGSCH_RA_MSG4_DONE)
        {
           RLOG_ARG1(L_DEBUG,DBG_CELLID,cell->cellId,
                 "RNTI:%d RaCb deleted as Msg4 transmission is done",
                 raCb->tmpCrnti);
           rgSCHRamDelRaCb(cell, raCb, FALSE);
        }
     }
     /* Initialize uplink HARQ related information for UE */
     rgSCHUhmRgrUeCfg(cell, ue, ueCfg);
     cmInitTimers(&ue->bsrTmr, 1);
#ifdef RGR_V1
     /* Added periodic BSR timer */
     cmInitTimers(&ue->bsrTmr, 1);

     /* Fix - Added proper configuration from U-ARM */
     if(ueCfg->ueBsrTmrCfg.isPrdBsrTmrPres == TRUE)
     {
        ue->ul.bsrTmrCfg.isPrdBsrTmrPres = TRUE;
        ue->ul.bsrTmrCfg.prdBsrTmr       = ueCfg->ueBsrTmrCfg.prdBsrTmr;
        ue->ul.bsrTmrCfg.retxBsrTmr      = ueCfg->ueBsrTmrCfg.retxBsrTmr;
     }
#endif
     /* Initialize downlink HARQ related information for UE */
     rgSCHDhmRgrUeCfg(cell, ue, ueCfg, errInfo);

     /* Initialize MeasureGap and Acknack Rep Information for UE */
     if((rgSCHMeasGapANRepUeCfg(cell, ue, ueCfg)) != ROK)
     {
        RLOG_ARG1(L_ERROR,DBG_CELLID,ueCfg->cellId,"Measurement Gap and"
                 " AckNack Rep failed in Config for CRNTI:%d", ueCfg->crnti);
        break;
     }


#ifdef LTE_TDD
     if((rgSCHUtlAllocUeANFdbkInfo(ue)) != ROK)
     {
        RLOG_ARG1(L_ERROR,DBG_CELLID,ueCfg->cellId,"Memomy allocation "
                 "Failed while UE related Ack Nack Information for CRNTI:%d",
                 ueCfg->crnti);
        break;
     }
     ue->dl.ackNackMode = ueCfg->ackNackModeEnum;
#endif /* LTE_TDD */

     /* Insert Ue */
     rgSCHDbmInsUeCb(cell, ue);

#ifdef TFU_UPGRADE
     /*  Intialize APeriodic CQI/PMI/RI Information for UE */
     ret = rgSCHCfgACqiUeCfg(cell, ue, &ueCfg->ueDlCqiCfg.aprdCqiCfg, 
           ue->ueCatEnum);

     ue->cqiRiWritIdx = 0;
     ue->cqiRiReadIdx = 0;
     /*   Initialize Periodic CQI/PMI, RI Information for UE  */
     ret = rgSCHCfgPCqiUeCfg(cell, ue, &ueCfg->ueDlCqiCfg.prdCqiCfg, 
           ue->ueCatEnum);

     /*   Initialize UL SRS Information for UE  */
     ret = rgSCHCfgSrsUeCfg(cell, ue, &ueCfg->srsCfg);

     /*   Initialize SR Information for UE  */
     ret = rgSCHCfgSrUeCfg(cell, ue, &ueCfg->srCfg);
#endif 

#ifdef LTEMAC_HDFDD
     if (rgSCHHdFddUeCfg(cell, ue, ueCfg->isHdFddEnbld) != ROK)
     {
        RLOG_ARG1(L_ERROR,DBG_CELLID,ueCfg->cellId,
                 "Could not do HD-FDD config for CRNTI:%d",ueCfg->crnti);
        break;
     }

#endif /* LTEMAC_HDFDD */
     /* ccpu00117452 - MOD - Changed macro name from
        RGR_RRM_DLPWR_CNTRL to RGR_CQI_REPT */
#ifdef RGR_CQI_REPT
     ue->cqiReptCfgInfo.numColltdCqiRept =
        ueCfg->ueCqiReptCfg.numColltdCqiRept;
#endif /* End of RGR_CQI_REPT */
#ifdef TFU_UPGRADE
     ue->pA.pres = FALSE;
     if (RG_SCH_UE_CFG_ISPAPRSNT(ueCfg->uePdschDedCfg.uepACfg))
     {
        ue->pA.pres = TRUE;
        ue->pA.val  = ueCfg->uePdschDedCfg.uepACfg.pA;
     }
#endif
     ue->isDrxEnabled = ueCfg->ueDrxCfg.isDrxEnabled;

     if ( ue->isDrxEnabled )
     {
        if((rgSCHDrxUeCfg(cell,ue,ueCfg)) != ROK )
        {
           RLOG_ARG1(L_ERROR,DBG_CELLID,ueCfg->cellId,"DRX configuration failed",
                     ueCfg->crnti);
           break;
        }
     }

     /* LTE_ADV_FLAG_REMOVED_START */
     if ((cell->lteAdvCb.sfrCfg.status == RGR_ENABLE) ||    \
        (cell->lteAdvCb.absCfg.status == RGR_ENABLE))
     {
        ue->lteAdvUeCb.rgrLteAdvUeCfg = ueCfg->ueLteAdvCfg;
     }   
     /* LTE_ADV_FLAG_REMOVED_END */

#ifdef TENB_STATS
     ue->tenbStats = TSL2AllocUeStatsBlk(ue->ueId);
     ue->tenbStats->stats.rnti = ue->ueId;
#endif

     errInfo->errCause = RGSCHERR_NONE;
     RETVALUE(ROK);
  }while(0);

  if (ue)
  {
     rgSCHCfgFreeUeCb(cell, ue);
  }
  RETVALUE(RFAILED);
}  /* rgSCHCfgRgrUeCfg */

/**
 * @brief Handler for PHR for MSG3.
 *
 * @details
 *
 *     Function : rgSCHCfgRgrUePhrMsg3
 *
 *     Processing Steps:
 *       Handle PHR related config for MSG3
 *
 *  @param[in]  RgSchCellCb  *cell
 *  @param[in]  RgrUeCb     *ueCb
 *  @param[in]  RgSchRaCb    *raCb
 *  @param[out] RgSchErrInfo *errInfo
 **/
#ifdef ANSI
PRIVATE Void rgSCHCfgRgrUePhrMsg3
(
RgSchCellCb  *cell,
RgSchRaCb    *raCb,
RgSchUeCb    *ue,
RgSchErrInfo *errInfo
)
#else
PRIVATE Void rgSCHCfgRgrUePhrMsg3(cell, raCb, ue, errInfo)
RgSchCellCb  *cell;
RgSchRaCb    *raCb;
RgSchUeCb    *ue;
RgSchErrInfo *errInfo;
#endif
{

   TRC2(rgSCHCfgRgrUePhrMsg3);

   /* Record msg3 allocation in the UE */
   rgSCHUtlRecMsg3Alloc(cell, ue, raCb);

   /* If raCb received PHR, update scheduler */
   if(raCb->phr.pres == TRUE)
   {
      ue->macCeRptTime = raCb->msg3AllocTime; 
      rgSCHUtlUpdPhr(cell, ue, raCb->phr.val, errInfo);
   }

   RETVOID;
}

/**
 *
 * @details
 *
 *     Function : rgSCHDynCfiReCfg
 *
 *  @param[in]  RgSchCellCb   *cell
 *              Bool          isDynCfiEnb 
 *  @return  S16
 *      -# ROK
 *      -# RFAILED
 **/
#ifdef ANSI
PUBLIC Void rgSCHDynCfiReCfg
(
RgSchCellCb   *cell,
Bool          isDynCfiEnb
)
#else
PUBLIC Void rgSCHDynCfiReCfg(cell, isDynCfiEnb)
RgSchCellCb   *cell;
Bool          isDynCfiEnb;
#endif 
{            
   U8                idx;
   RgSchCmnDlCell    *cellSchDl = RG_SCH_CMN_GET_DL_CELL(cell);

   TRC2(rgSCHDynCfiReCfg);

   if(isDynCfiEnb)
   {
      cell->dynCfiCb.ttiCnt = 0;
      cellSchDl->newCfi = cellSchDl->currCfi;
   }   
   else
   {
      /* Resetting the parameters*/
      cell->dynCfiCb.cceFailCnt          = 0;
      cell->dynCfiCb.cceFailSum          = 0;
      cell->dynCfiCb.prevCceFailIdx      = 0;

      for(idx = 0; idx < cell->dynCfiCb.numFailSamples; idx++)
      {
         cell->dynCfiCb.cceFailSamples[idx] = 0;
      }   

      cell->dynCfiCb.cceUsed             = 0;
      cell->dynCfiCb.lowCceCnt           = 0;
      cell->dynCfiCb.ttiCnt = 0;
   }   
}
/**
 * @brief Handler for the cell reconfiguration request from RRM to MAC.
 *
 * @details
 *
 *     Function : rgSCHCfgRgrCellRecfg
 *
 *     Processing Steps:
 *      - Invoke SCH with cell control block to update
 *        scheduler specific information.
 *      - Update cell control block with the values recieved in the
 *        configuration.
 *      - If successful, return ROK else RFAILED.
 *
 *  @param[in]  RgSchCellCb   *cell
 *  @param[in]  RgrCellRecfg  *cellRecfg
 *  @param[out] RgSchErrInfo  *errInfo
 *  @return  S16
 *      -# ROK
 *      -# RFAILED
 **/
#ifdef ANSI
PUBLIC S16 rgSCHCfgRgrCellRecfg
(
RgSchCellCb   *cell,
RgrCellRecfg  *cellRecfg,
RgSchErrInfo  *errInfo
)
#else
PUBLIC S16 rgSCHCfgRgrCellRecfg(cell, cellRecfg, errInfo)
RgSchCellCb   *cell;
RgrCellRecfg  *cellRecfg;
RgSchErrInfo  *errInfo;
#endif
{
   S16      ret;
   Inst    inst = cell->instIdx;
/* LTE_ADV_FLAG_REMOVED_START */
   U8      i = 0;
   U16     len; /* dsfr_pal_fixes ** 21-March-2013 ** SKS */
/* LTE_ADV_FLAG_REMOVED_END */
   
   TRC2(rgSCHCfgRgrCellRecfg);


   errInfo->errCause = RGSCHERR_CFG_RGR_CELL_RECFG;
   
   /* Invoke scheduler to update scheduler specific information */
   ret =  rgSCHUtlRgrCellRecfg(cell, cellRecfg, errInfo);
   if (ret != ROK)
   {
     RLOG_ARG0(L_ERROR,DBG_CELLID,cellRecfg->cellId, "RGR Cell re-configuration failed "
          "at Scheduler ");
      RETVALUE(RFAILED);
   }
   
   /* Invoke DHM to update DHM specific information */
   rgSCHDhmRgrCellRecfg(cell, cellRecfg, errInfo);

      /* PUCCH Reconfiguration */
   if (cellRecfg->recfgTypes & RGR_CELL_PUCCH_RECFG)
   {
      cell->pucchCfg = cellRecfg->pucchRecfg;
   }
 
   /* SRS Reconfiguration */
   if (cellRecfg->recfgTypes & RGR_CELL_SRS_RECFG)
   {
      cell->srsCfg.isSrsCfgPres = cellRecfg->srsRecfg.isSrsCfgSetup;
      if(cellRecfg->srsRecfg.isSrsCfgSetup)
      {
         cell->srsCfg.srsCfgPrdEnum = cellRecfg->srsRecfg.srsCfgPrdEnum;
         cell->srsCfg.srsBwEnum = cellRecfg->srsRecfg.srsBwEnum;
         cell->srsCfg.srsTxOffst =
            rgSrsTxOffstTbl[cellRecfg->srsRecfg.srsSubFrameCfg];
         /*ccpu00116923 - ADD - Srs Present support */
#ifdef TFU_UPGRADE
         cell->srsCfg.srsSubFrameCfg = cellRecfg->srsRecfg.srsSubFrameCfg;
#endif
      }
   }

   /* RACH Reconfiguration */
   if (cellRecfg->recfgTypes & RGR_CELL_RACH_RECFG)
   {
      cell->rachCfg = cellRecfg->rachRecfg;
   }

   /* ccpu00132256:MOD: Moved this assignment from Validation to here.*/
   if (cellRecfg->recfgTypes & RGR_CELL_TMRS_RECFG)
   {
      cell->t300TmrVal = cellRecfg->t300TmrVal;
   }
#ifdef RGR_SI_SCH
   /* SI Reconfiguration */
   if (cellRecfg->recfgTypes & RGR_CELL_SI_RECFG)
   {
      /*Set the specified SI configuration. */
      cell->siCb.newSiCfg = cellRecfg->siReCfg;
      /* Set the Bit mask for SI re-configuration */
      cell->siCb.siBitMask |= RGSCH_SI_SICFG_UPD; 
   }
#endif /*RGR_SI_SCH */

   /* Overload RACH Control changes */
   if (cellRecfg->recfgTypes & RGR_CELL_CNTRL_CMD_RECFG)
   {
      if (cellRecfg->cntrlCmdCfg.cmdType == RGR_CNTRL_CMD_RACH_OVRLD)
      {
         cell->overLoadBackOffEnab = cellRecfg->cntrlCmdCfg.cmdDesc.rachOvrLd.backOffEnb;
         cell->overLoadBackOffval = cellRecfg->cntrlCmdCfg.cmdDesc.rachOvrLd.backOffVal;
      }
      else if (cellRecfg->cntrlCmdCfg.cmdType == RGR_CNTRL_CMD_CPU_OVRLD)
      {
         if( ROK != rgSCHUtlResetCpuOvrLdState(cell, cellRecfg->cntrlCmdCfg.cmdDesc.\
                                          cpuOvrLd.instruction))
         {
            RLOG_ARG1(L_ERROR,DBG_CELLID, cellRecfg->cellId,
               "Invalid CPU OvrLd Ins %d for cell", 
	       cellRecfg->cntrlCmdCfg.cmdDesc.cpuOvrLd.instruction);
            RETVALUE(RFAILED);
         }
      }
   }

/* LTE_ADV_FLAG_REMOVED_START */
   if (cellRecfg->recfgTypes & RGR_CELL_LTEA_FEATURE_RECFG)
   {
      if(cellRecfg->rgrLteAdvCfg.pres & RGR_ABS)
      {
         cell->lteAdvCb.absCfg =
            cellRecfg->rgrLteAdvCfg.absCfg;         
      }
      if(cellRecfg->rgrLteAdvCfg.pres & RGR_SFR)
      {
         cmMemcpy((U8 *)&cell->lteAdvCb.sfrCfg, (U8 *)&cellRecfg->rgrLteAdvCfg.sfrCfg,
               sizeof(RgrSfrConfig));
         /* dsfr_pal_fixes ** 21-March-2013 ** SKS */
         if (cellRecfg->rgrLteAdvCfg.sfrCfg.status == RGR_ENABLE)
         {
            for(i = 0; i < RGSCH_NUM_DL_SUBFRAMES; i++)
            {
               /*initialise the pools of CC and CE*/
               if(rgSchSFRTotalPoolInit(cell, cell->subFrms[i]))
               {
                  RETVALUE(RFAILED);
               }
            }
         }
         else
         {
            for(i = 0; i < RGSCH_NUM_DL_SUBFRAMES; i++)
            {
               /*initialise the pools of CC and CE*/
               rgSchSFRTotalPoolFree(&cell->subFrms[i]->sfrTotalPoolInfo, cell);
            }

            if (cell->lteAdvCb.dsfrCfg.status == RGR_ENABLE)
            {
               /* releasing rntp info val from each subframe */ 
               for(i = 0; i < RGSCH_NUM_DL_SUBFRAMES; i++)
               {
                  rgSchDSFRRntpInfoFree(&cell->subFrms[i]->rntpInfo, cell, cell->bwCfg.dlTotalBw);
               }

               /* releasing RNTP Aggregation Info from CellCb*/
               rgSchDSFRRntpInfoFree(&cell->rntpAggrInfo, cell, cell->bwCfg.dlTotalBw); 

               cell->lteAdvCb.dsfrCfg.status = RGR_DISABLE;
            }
         }
      }
      /* dsfr_pal_fixes ** 21-March-2013 ** SKS ** Start */
      if(cellRecfg->rgrLteAdvCfg.pres & RGR_DSFR)
      {
         cell->lteAdvCb.dsfrCfg =
            cellRecfg->rgrLteAdvCfg.dsfrCfg;
         if (cell->lteAdvCb.dsfrCfg.status == RGR_ENABLE)
         {
            for(i = 0; i < RGSCH_NUM_DL_SUBFRAMES; i++)
            {
               /*initialise the pools of CC and CE*/
               if(rgSchDSFRRntpInfoInit(&cell->subFrms[i]->rntpInfo,cell,cell->bwCfg.dlTotalBw))
               {
                  RETVALUE(RFAILED);
               }
            }
            /*Calculating the length of RNTP array based on Dl Bandwidth */
            len = (U16)((cell->bwCfg.dlTotalBw % 8 == 0) ? (cell->bwCfg.dlTotalBw/8) : (cell->bwCfg.dlTotalBw/8 + 1)); /* KW fix for LTE_ADV */
            if(cell->rntpAggrInfo.pres == NOTPRSNT)
            {   
               if((rgSCHUtlAllocSBuf(inst, (Data**)&(cell->rntpAggrInfo.val),
                           (len * sizeof(U8)))) != ROK)
               {
                  RLOG_ARG0(L_ERROR,DBG_CELLID,cellRecfg->cellId,
                            "Memory allocation FAILED for RNTP Alloc");
                  RETVALUE(RFAILED);
               }
               cell->rntpAggrInfo.pres = PRSNT_NODEF;
               cell->rntpAggrInfo.len  = len; 
            }
         }
         /* in case if DSFR is disabled, need to free RNTP pattern val*/
         else
         {
            /* releasing rntp info val from each subframe */ 
            for(i = 0; i < RGSCH_NUM_DL_SUBFRAMES; i++)
            {
               rgSchDSFRRntpInfoFree(&cell->subFrms[i]->rntpInfo, cell, cell->bwCfg.dlTotalBw);
            }

            /* releasing RNTP Aggregation Info from CellCb*/
            rgSchDSFRRntpInfoFree(&cell->rntpAggrInfo, cell, cell->bwCfg.dlTotalBw);
         }   
      }
      /* dsfr_pal_fixes ** 21-March-2013 ** SKS ** End */
   }
/* LTE_ADV_FLAG_REMOVED_END */

   /* Dynamic CFI cell Reconfiguration */
   if(cellRecfg->recfgTypes & RGR_CELL_DYN_CFI_RECFG)
   {
      if(cell->dynCfiCb.isDynCfiEnb != cellRecfg->isDynCfiEnb)
      {
         if(cell->dynCfiCb.switchOvrInProgress)  
         {
            cell->dynCfiCb.dynCfiRecfgPend = TRUE;
         }  
         else
         { 
            cell->dynCfiCb.isDynCfiEnb = cellRecfg->isDynCfiEnb;
            rgSCHDynCfiReCfg(cell, cellRecfg->isDynCfiEnb); 
         }
      }   
      else
      {
         /* To hanlde the case where reconfiguration comes for disabling 
          * and then enabling before switchover period expires */
         cell->dynCfiCb.dynCfiRecfgPend = FALSE;
      }   
   }   
   /* Dynamic config of AUTO chnage flag */
   if(cellRecfg->recfgTypes & RGR_CELL_AUTO_CFG_MODE_RECFG)
   {
      if(cell->isAutoCfgModeEnb != cellRecfg->isAutoCfgModeEnb)
      {
         cell->isAutoCfgModeEnb = cellRecfg->isAutoCfgModeEnb;
      }
   }

   if (cellRecfg->recfgTypes & RGR_CELL_CSG_PARAM_RECFG)
   {
      cell->minDlResNonCsg = cellRecfg->csgParamCfg.minDlResNonCsg;
      cell->minUlResNonCsg = cellRecfg->csgParamCfg.minUlResNonCsg;
   }
  
   errInfo->errCause = RGSCHERR_NONE;
   RETVALUE(ROK);
}  /* rgSCHCfgRgrCellRecfg */

/**
 * @brief Handler for the UE reconfiguration request from RRC to MAC.
 *
 * @details
 *
 *     Function : rgSCHCfgRgrUeRecfgRntiChg
 *
 *     Processing Steps:
 *      - If rnti changes,
 *        - Invoke RAM for UE reconfiguration.
 *        - Delete old UE from the list.
 *        - Update the new rnti and re-insert the UE in the list.
 *      - If successful, return ROK else RFAILED.
 *
 *  @param[in]  RgSchCellCb  *cell
 *  @param[in]  RgSchUeCb    *ue
 *  @param[in]  RgrUeRecfg   *ueRecfg
 *  @param[out] RgSchErrInfo *errInfo
 *  @return  S16
 *      -# ROK
 *      -# RFAILED
 **/
#ifdef ANSI
PRIVATE S16 rgSCHCfgRgrUeRecfgRntiChg
(
RgSchCellCb    *cell,
RgSchUeCb      *ue,
RgrUeRecfg     *ueRecfg,
RgSchErrInfo   *errInfo
)
#else
PRIVATE S16 rgSCHCfgRgrUeRecfgRntiChg(cell, ue, ueRecfg, errInfo)
RgSchCellCb    *cell;
RgSchUeCb      *ue;
RgrUeRecfg     *ueRecfg;
RgSchErrInfo   *errInfo;
#endif
{
   S16          ret;
   RgSchRaCb    *raCb;
   RgSchRaCb    *oldRaCb;
   
   TRC2(rgSCHCfgRgrUeRecfgRntiChg);

   /* Handle CRNTI change in reconfiguration */
   if (ueRecfg->oldCrnti != ueRecfg->newCrnti)
   {
      RgSchRntiLnk   *oldRntiLnk=NULLP;
      CmLteRnti      oldRnti = 0;
      if ((raCb = rgSCHDbmGetRaCb(cell, ueRecfg->newCrnti)) == NULLP)
      {
         RLOG_ARG2(L_ERROR,DBG_CELLID,ueRecfg->cellId,"UEID:No RaCb exists while"
              "Reconfig for OLD CRNTI:%d NEW CRNTI:%d",ueRecfg->oldCrnti,ueRecfg->newCrnti);
         RETVALUE(RFAILED);
      }

      /* rntiLnk does not exist for a HandIn UE. Hence this check. */
      if(ue->rntiLnk)
      {
         oldRntiLnk = ue->rntiLnk;
      }
      else
      {
        /* Fix : syed HO UE does not have a valid ue->rntiLnk */
         oldRnti = ue->ueId;
      }
      
      /* Fix : syed Deleting Old DL HqEnt. It would be assigned after
       * reest RACH(msg4) is completed. */
      rgSCHDhmDelHqEnt(cell, &ue->dl.hqEnt);

      /* Initialize RAM related information for UE */
      ret = rgSCHRamRgrUeCfg(cell, ue, raCb, errInfo);
      if (ret != ROK)
      {
         RLOG_ARG2(L_ERROR,DBG_CELLID,ueRecfg->cellId,"RAM Handling for UE Reconfig failed"
              "for OLD CRNTI:%d NEW CRNTI:%d",ueRecfg->oldCrnti,ueRecfg->newCrnti);
         RETVALUE(RFAILED);
      }
      /* Delete Ue from the ue list */
      rgSCHDbmDelUeCb(cell, ue);

      /* Inititialize Ue control block */
      ue->ueId = ueRecfg->newCrnti;
      RLOG_ARG2(L_DEBUG,DBG_CELLID,cell->cellId,
            "Changing RNTI from %d to %d",
            ueRecfg->oldCrnti, 
            ueRecfg->newCrnti);
      rgSCHUtlUpdY(ue->ueId, ue->y);
      /* Fix ccpu00122631 */
      ue->dl.dlAllocCb.rnti = ueRecfg->newCrnti;

      rgSCHUtlRecMsg3Alloc(cell, ue, raCb);

      /* If raCb received PHR, update scheduler */
      if(raCb->phr.pres == TRUE)
      {
         ue->macCeRptTime = raCb->msg3AllocTime; 
         rgSCHUtlUpdPhr(cell, ue, raCb->phr.val, errInfo);
      }

#ifdef RGR_V2  /* Acc Fix */
     if(TRUE == ue->isDrxEnabled)
     {
         ueRecfg->ueDrxRecfg.isDrxEnabled = TRUE;
         ret = rgSCHDrxUeReCfg(cell,ue,ueRecfg);  

         if ( ret != ROK )
         {
             RLOG_ARG2(L_ERROR,DBG_CELLID,ueRecfg->cellId,"UE DRX re-est failed"
              "for OLD CRNTI:%d NEW CRNTI:%d",ueRecfg->oldCrnti,ueRecfg->newCrnti);
             RETVALUE(RFAILED);
         }
     }
#endif /* Acc Fix */

      /* Re-insert updated Ue */
      rgSCHDbmInsUeCb(cell, ue);
#ifdef TENB_STATS
      ue->tenbStats->stats.rnti = ue->ueId;
#endif

      /* Fix : syed If MSG4 is done, since corresponding ueCb
       * is ready, the raCb should be cleared immediately.
       * Otherwise it would remain in the cell until timed out
       * and till then the hq Feedbacks will be assumed to be
       * for msg4 */
      if (raCb->raState == RGSCH_RA_MSG4_DONE)
      {
         RLOG_ARG1(L_DEBUG,DBG_CELLID,ueRecfg->cellId,
               "RNTI:%d with RaCb deleted as Msg4 transmission is done", 
               raCb->tmpCrnti);
         rgSCHRamDelRaCb(cell, raCb, FALSE);
      }
      /* Fix : syed moving the UL CQI initialization to UERESET */

      /* Release Older rnti */
      if(oldRntiLnk)
      {
         /* This is the rare case in which back to back reestablishment is 
          * happening and previous re-est was not done completely (MSG4 was 
          * not done) for an UE, and again re-est is triggered for the same 
          * UE. We are deleting the old RA CB for the previous re-est which 
          * still exist due to MSG4 not transmitted successfully */
         if ((oldRaCb = rgSCHDbmGetRaCb(cell, oldRntiLnk->rnti)) != NULLP)
         {
            rgSCHRamDelRaCb(cell, oldRaCb, FALSE);
         }

         rgSCHUtlRlsRnti(cell, oldRntiLnk, TRUE, ueRecfg->newCrnti);
      }
      else
      {
        /* Fix : syed HO UE does not have a valid ue->rntiLnk */
        /* Just indicate to MAC, no need to release at SCH */
         RLOG_ARG2(L_DEBUG,DBG_CELLID,cell->cellId,
               "HO OldRnti:%d RLS and NewRnti:%d CHNG IND TO MAC",
               oldRnti, ueRecfg->newCrnti);
         rgSCHUtlIndRntiRls2Mac(cell, oldRnti, TRUE, ueRecfg->newCrnti);
      }
   }
   RETVALUE(ROK);
}
/**
 * @brief Handler for the UE reconfiguration request from RRC to MAC.
 *
 * @details
 *
 *     Function : rgSCHCfgRgrUeRecfg
 *
 *     Processing Steps:
 *      - If rnti changes,
 *        - Invoke RAM for UE reconfiguration.
 *        - Delete old UE from the list.
 *        - Update the new rnti and re-insert the UE in the list.
 *      - Update the UE control block with the reconfigured values.
 *      - Invoke SCH, UHM and DHM with updated UE control block to 
 *        update scheduler, uplink HARQ and downlink HARQ specific
 *        parameters.
 *      - If successful, return ROK else RFAILED.
 *
 *  @param[in]  RgSchCellCb  *cell
 *  @param[in]  RgSchUeCb    *ue
 *  @param[in]  RgrUeRecfg   *ueRecfg
 *  @param[out] RgSchErrInfo *errInfo
 *  @return  S16
 *      -# ROK
 *      -# RFAILED
 **/
#ifdef ANSI
PUBLIC S16 rgSCHCfgRgrUeRecfg
(
RgSchCellCb    *cell,
RgSchUeCb      *ue,
RgrUeRecfg     *ueRecfg,
RgSchErrInfo   *errInfo
)
#else
PUBLIC S16 rgSCHCfgRgrUeRecfg(cell, ue, ueRecfg, errInfo)
RgSchCellCb    *cell;
RgSchUeCb      *ue;
RgrUeRecfg     *ueRecfg;
RgSchErrInfo   *errInfo;
#endif
{
   S16          ret;
   
   TRC2(rgSCHCfgRgrUeRecfg);


   errInfo->errCause = RGSCHERR_CFG_RGR_UE_RECFG;
   if (ueRecfg->ueRecfgTypes)
   {
      /* Update scheduler related information for UE */
      ret = rgSCHUtlRgrUeRecfg(cell, ue, ueRecfg, errInfo);
      if (ret != ROK)
      {
         RLOG_ARG2(L_ERROR,DBG_CELLID,ueRecfg->cellId,
             "Scheduler handling while reconfig failed"
              "for OLD CRNTI:%d NEW CRNTI:%d",ueRecfg->oldCrnti,ueRecfg->newCrnti);
         RETVALUE(RFAILED);
      }

      /* Update uplink HARQ related information for UE */
      rgSCHUhmRgrUeRecfg(cell, ue, ueRecfg);

      /* Update TA related information for UE */
      if (ueRecfg->ueRecfgTypes & RGR_UE_TATMR_RECFG)
      {
         rgSCHCfgUeTaRecfg(cell, ue, ueRecfg, errInfo);
      }

      /*Update Measurement Gap and AckNack Details */
      /* After merging from 2.2 */
      if (ueRecfg->ueRecfgTypes & RGR_UE_ACKNACK_MEASGAP_RECFG)
      {
         ret = rgSCHMeasGapANRepUeRecfg(cell, ue, ueRecfg);
         if (ret != ROK)
         {
            RLOG_ARG2(L_ERROR,DBG_CELLID,ueRecfg->cellId,"Measurement Gap and"
                     "AckNack Rep Recfg failed for OLD CRNTI:%d NEW CRNTI:%d",
                      ueRecfg->oldCrnti,ueRecfg->newCrnti);
            RETVALUE(RFAILED);
         }
      }
      if (ueRecfg->ueRecfgTypes & RGR_UE_BSRTMR_RECFG)
      {
         cmInitTimers(&ue->bsrTmr, 1);
         ue->ul.bsrTmrCfg = ueRecfg->ueBsrTmrRecfg;
         if ((ue->ul.bsrTmrCfg.isPrdBsrTmrPres) && 
               (ue->ul.bsrTmrCfg.prdBsrTmr == 0xFFFF))
         {
            ue->ul.bsrTmrCfg.isPrdBsrTmrPres = FALSE;
         }
      }
   }

   if (RFAILED == rgSCHCfgRgrUeRecfgRntiChg (cell, ue, ueRecfg, errInfo))
   {
       RLOG_ARG2(L_ERROR,DBG_CELLID,ueRecfg->cellId,"RNTI change "
               "failed for OLD CRNTI:%d NEW CRNTI:%d",
                ueRecfg->oldCrnti,ueRecfg->newCrnti);
       RETVALUE(RFAILED);
   }

#ifdef TFU_UPGRADE
   /* Re-Initialize Aperiodic CQI Information for UE*/
   if ( ueRecfg->ueRecfgTypes & RGR_UE_APRD_DLCQI_RECFG ) 
   {
      ret = rgSCHCfgAcqiUeReCfg(cell, ue, &ueRecfg->aprdDlCqiRecfg, 
            ue->ueCatEnum);
   }
   /*   Re-Initialize Periodic CQI/PMI, RI Information for UE  */
   if ( ueRecfg->ueRecfgTypes & RGR_UE_PCQI_RECFG) 
   {
      ret = rgSCHCfgPCqiUeReCfg(cell, ue, &ueRecfg->cqiCfg, 
            ue->ueCatEnum);
   }
   /*   Re-Initialize UL SRS Information for UE  */
   if ( ueRecfg->ueRecfgTypes & RGR_UE_SRS_RECFG) 
   {
      ret = rgSCHCfgSrsUeReCfg(cell, ue, &ueRecfg->srsCfg);
   }
   /*   Re-Initialize SR Information for UE  */
   if ( ueRecfg->ueRecfgTypes & RGR_UE_SR_RECFG) 
   {
      ret = rgSCHCfgSrUeReCfg(cell, ue, &ueRecfg->srCfg);
   }
#endif 

#ifdef LTEMAC_HDFDD  
   if(ueRecfg->isHdFddEnbld)
   {
        ret = rgSCHHdFddUeCfg(cell, ue, ueRecfg->isHdFddEnbld);
        if (ret != ROK)
        {
           errInfo->errCause = RGSCHERR_HDFDD_SPSCFGRD;
           RETVALUE(ret);
        }
   }
#endif /* LTEMAC_HDFDD */
#ifdef RGR_V2
  if ( ueRecfg->ueRecfgTypes & RGR_UE_DRX_RECFG)
  {
     ret = rgSCHDrxUeReCfg(cell,ue,ueRecfg);  

     if ( ret != ROK )
     {
        RLOG_ARG2(L_ERROR,DBG_CELLID,ueRecfg->cellId,"UE DRX reconfig failed"
               "failed for OLD CRNTI:%d NEW CRNTI:%d",
                ueRecfg->oldCrnti,ueRecfg->newCrnti);
        RETVALUE(RFAILED);
     }
  }
#endif  
/* ccpu00117452 - MOD - Changed macro name from
   RGR_RRM_DLPWR_CNTRL to RGR_CQI_REPT */
#ifdef RGR_CQI_REPT
   /* CQI Reporting (N)  Re-configuration */
   if(ueRecfg->ueRecfgTypes & RGR_UE_CQIREPT_RECFG)
   {
      ret = rgSCHCfgUeCqiReptReCfg(cell, ue, ueRecfg);
      if(ret != OK)
      {
         errInfo->errCause = RGSCHERR_CQIREPT;
         RETVALUE(ret);
      }
   }
#endif /* End of RGR_CQI_REPT */
#ifdef TFU_UPGRADE
   /* pA Re-configuration */
   if((ueRecfg->ueRecfgTypes & RGR_UE_PA_RECFG) &&
         RG_SCH_UE_CFG_ISPAPRSNT(ueRecfg->uePdschDedCfg.uepACfg))
   {
      ue->pA.pres = TRUE;
      ue->pA.val  = ueRecfg->uePdschDedCfg.uepACfg.pA;
   }
#endif

/* LTE_ADV_FLAG_REMOVED_START */
   if(ueRecfg->ueRecfgTypes & RGR_UE_LTEA_RECFG)
   {
      if(ueRecfg->ueLteAdvCfg.pres & RGR_ABS)
      {
         ue->lteAdvUeCb.rgrLteAdvUeCfg.isAbsUe = ueRecfg->ueLteAdvCfg.isAbsUe;
      }

      if(ueRecfg->ueLteAdvCfg.pres & RGR_SFR)
      {
         ue->lteAdvUeCb.rgrLteAdvUeCfg.isUeCellEdge = ueRecfg->ueLteAdvCfg.isUeCellEdge;
      }
   }
/* LTE_ADV_FLAG_REMOVED_END */

   errInfo->errCause = RGSCHERR_NONE;

   RETVALUE(ROK);
}  /* rgSCHCfgRgrUeRecfg */


/**
 * @brief Handler for the logical channel reconfiguration request from
 * RRC to MAC.
 *
 * @details
 *
 *     Function : rgSCHCfgRgrLchRecfg
 *
 *     Processing Steps:
 *      - Invoke scheduler to update scheduler specific information.
 *      - Update the dedicated logical channel Cb with the reconfigured
 *        values.
 *      - If successful, return ROK else RFAILED.
 *
 *  @param[in]  RgUlCellCb  *cell
 *  @param[in]  RgUlUeCb    *ue
 *  @param[in]  RgSchUlLcCb    *ulLc
 *  @param[in]  RgSchDlLcCb    *dlLc
 *  @param[in]  RgrLchRecfg *lcRecfg
 *  @param[out] RgSchErrInfo   *errInfo
 *  @return  S16
 *      -# ROK
 *      -# RFAILED
 **/
#ifdef ANSI
PUBLIC S16 rgSCHCfgRgrLchRecfg
(
RgSchCellCb    *cell,
RgSchUeCb      *ue,
RgSchDlLcCb    *dlLc,
RgrLchRecfg    *lcRecfg,
RgSchErrInfo   *errInfo
)
#else
PUBLIC S16 rgSCHCfgRgrLchRecfg(cell, ue, dlLc, lcRecfg, errInfo)
RgSchCellCb    *cell;
RgSchUeCb      *ue;
RgSchDlLcCb    *dlLc;
RgrLchRecfg    *lcRecfg;
RgSchErrInfo   *errInfo;
#endif
{
   S16     ret = ROK;

   TRC2(rgSCHCfgRgrLchRecfg);

   errInfo->errCause = RGSCHERR_CFG_RGR_LC_RECFG;
   /* Invoke Scheduler to update the new configuration */
   ret = rgSCHUtlRgrLcRecfg(cell, ue, dlLc, lcRecfg, errInfo);
   if (ret != ROK)
   {
      RLOG_ARG2(L_ERROR,DBG_CELLID,lcRecfg->cellId,"Scheduler handling for LC Recfg"
              " failed for CRNTI:%d LCID:%d",lcRecfg->crnti,lcRecfg->lcId);
      RETVALUE(RFAILED);
   }

   errInfo->errCause = RGSCHERR_NONE;
   RETVALUE(ROK);
}  /* rgSCHCfgRgrLchRecfg */
/**
 * @brief Handler for the logical channel reconfiguration request from
 * RRC to MAC.
 *
 * @details
 *
 *     Function : rgSCHCfgRgrLcgRecfg
 *
 *     Processing Steps:
 *      - Invoke scheduler to update scheduler specific information.
 *      - Update the dedicated logical channel Cb with the re-configured
 *        values.
 *      - If successful, return ROK else RFAILED.
 *
 *  @param[in]  RgUlCellCb   *cell
 *  @param[in]  RgUlUeCb     *ue
 *  @param[in]  RgrLcgRecfg  *lcgRecfg
 *  @param[out] RgSchErrInfo *errInfo
 *  @return  S16
 *      -# ROK
 *      -# RFAILED
 **/
#ifdef ANSI
PUBLIC S16 rgSCHCfgRgrLcgRecfg
(
RgSchCellCb    *cell,
RgSchUeCb      *ue,
RgrLcgRecfg    *lcgRecfg,
RgSchErrInfo   *errInfo
)
#else
PUBLIC S16 rgSCHCfgRgrLcgRecfg(cell, ue, lcgRecfg, errInfo)
RgSchCellCb    *cell;
RgSchUeCb      *ue;
RgrLcgRecfg    *lcgRecfg;
RgSchErrInfo   *errInfo;
#endif
{
   S16     ret = ROK;

   TRC2(rgSCHCfgRgrLcgRecfg);

   errInfo->errCause = RGSCHERR_CFG_RGR_LCG_RECFG;
  
   /*Added for handling LCG ReConfig if the LCG was deleted */
   ue->ul.lcgArr[lcgRecfg->ulRecfg.lcgId].lcgId = lcgRecfg->ulRecfg.lcgId;

   /* Invoke Scheduler to update the new configuration */
   ret = rgSCHUtlRgrLcgRecfg(cell, ue, lcgRecfg, errInfo);
   if (ret != ROK)
   {
      RLOG_ARG2(L_ERROR,DBG_CELLID,lcgRecfg->cellId,"Scheduler handling for LCG Recfg"
              " failed for CRNTI:%d LCGID:%d",lcgRecfg->crnti,lcgRecfg->ulRecfg.lcgId);
      RETVALUE(RFAILED);
   }

   errInfo->errCause = RGSCHERR_NONE;
   RETVALUE(ROK);
}  /* rgSCHCfgRgrLcgRecfg */

/**
 * @brief Handler for the UE Reset request from RRM to MAC.
 *
 * @details
 *
 *     Function : rgSCHCfgRgrUeReset
 *
 *     Processing Steps:
 *     - Call Measument Gap Module and Ack/Nack Module for resetting UE.
 *     - Call Common Schduler UE rest API which inturn will call scheduler
 *     specific UE Reset APis to reset UE.
 *
 *  @param[in]  RgSchCellCb    *cell
 *  @param[in]  RgSchUeCb      *ue
 *  @param[in]  RgrRst         *reset
 *  @param[out] RgSchErrInfo   *errInfo
 *  @return  S16
 *      -# ROK
 *      -# RFAILED
 **/
#ifdef ANSI
PUBLIC S16 rgSCHCfgRgrUeReset
(
RgSchCellCb    *cell,
RgSchUeCb      *ue,
RgrRst         *reset,
RgSchErrInfo   *errInfo
)
#else
PUBLIC S16 rgSCHCfgRgrUeReset(cell, ue, reset, errInfo)
RgSchCellCb    *cell;
RgSchUeCb      *ue;
RgrRst         *reset;
RgSchErrInfo   *errInfo;
#endif
{
   U32     idx;
   RgSchRaCb *raCb;

   TRC2(rgSCHCfgRgrUeReset);


   errInfo->errCause = RGSCHERR_CFG_RGR_UE_RESET;
  
   /* Setting BO of Each Logical Channel of the UE to 0 */
   for (idx = 0; idx < RGSCH_MAX_LC_PER_UE; idx++)
   {
      if(ue->dl.lcCb[idx] != NULLP)
         ue->dl.lcCb[idx]->bo = 0;
   }

   /* Reset the totalBo */
   ue->totalBo = 0;
   /* Call DRX module to stop all DRX timers */
   /* ccpu00129899 */
   if(ue->drxCb != NULLP)
   {
     (Void)rgSCHDrxUeDel(cell,ue);
   }

   /* ccpu00140894- Stop TXMode transiition timer if it is running*/
   if (ue->txModeTransTmr.tmrEvnt != TMR_NONE)
   {
      rgSCHTmrStopTmr(cell, RG_SCH_TMR_TXMODE_TRNSTN, ue);
      ue->txModeTransCmplt =TRUE;
   }

   /* ccpu00133470- Meas Gap should be released during RRC re-establishment */
   rgSCHMeasGapANRepUeDel(cell, ue, FALSE);

   /* Call Common scheduler which in turn will call specific scheduler for UE
    * Reset*/
   rgSCHUtlUeReset(cell, ue);
  
   /* Release the hqProcs if hqE is valid*/
   if(ue->dl.hqEnt != NULLP) 
   {
      rgSCHDhmHqEntReset(ue->dl.hqEnt);
   }
   else
   {
      RLOG_ARG2(L_ERROR,DBG_CELLID,reset->cellId,  
         "NPB: Found invalid DL HQ entity during UEID:%d CRNTI:%d",
         ue->ueId,reset->crnti);
   }

   /* In case of back to back reestablishments, when this UE's
    * previous ReEst is still in progress and has got RESET
    * as part of new ReEst */
   if((raCb = rgSCHDbmGetRaCb(cell, ue->ueId)) != NULLP)
   {
      rgSCHRamDelRaCb(cell, raCb, FALSE);
   }
 
   /* Fix : syed set UE inactive in DL until UE is reinitialization completed */
   ue->dl.dlInactvMask |= RG_HQENT_INACTIVE;
   ue->ul.ulInactvMask |= RG_HQENT_INACTIVE;
   /* [ccpu00127141] Resetting TA related parameters */
   ue->dl.taCb.ta = RGSCH_NO_TA_RQD;
   ue->dl.taCb.state = RGSCH_TA_IDLE;
   
   /*[ccpu00121813]-ADD-Initializing outstanding TA value */
   ue->dl.taCb.outStndngTa = FALSE;
   ue->dl.taCb.outStndngTaval = RGSCH_NO_TA_RQD;
   
   if (ue->dl.taCb.cfgTaTmr)
   {
      rgSCHTmrStartTmr (cell, ue, RG_SCH_TMR_TA, ue->dl.taCb.cfgTaTmr);
   }

#ifdef DL_LA
   /* Resetting the Tx mode change factor on UE reset */
   ue->mimoInfo.txModUpChgFactor = 0;
   ue->mimoInfo.txModDownChgFactor = 0;
#endif

   errInfo->errCause = RGSCHERR_NONE;
   RETVALUE(ROK);
}  /* rgSCHCfgRgrUeReset */

/**
 * @brief Handler for the cell delete request from RRM to MAC.
 *
 * @details
 *
 *     Function : rgSCHCfgRgrCellDel
 *
 *     Processing Steps:
 *      - Fetch the cell control block.
 *      - Remove the cell control block from the hash list of cells.
 *      - Free the cell control block.
 *      - If successful, return ROK else return RFAILED.
 *
 *  @param[in]  RgSchCellCb    *cell
 *  @param[in]  RgrDel         *cellDelInfo
 *  @param[out] RgSchErrInfo   *errInfo
 *  @return  S16
 *      -# ROK
 *      -# RFAILED
 **/
#ifdef ANSI
PUBLIC S16 rgSCHCfgRgrCellDel
(
RgSchCellCb    *cell,
RgrDel         *cellDelInfo,
RgSchErrInfo   *errInfo
)
#else
PUBLIC S16 rgSCHCfgRgrCellDel(cell, cellDelInfo, errInfo)
RgSchCellCb    *cell;
RgrDel         *cellDelInfo;
RgSchErrInfo   *errInfo;
#endif
{ 
   TRC2(rgSCHCfgRgrCellDel);


   errInfo->errCause = RGSCHERR_CFG_RGR_CELL_DEL;

   if (cell->cellId != cellDelInfo->u.cellDel.cellId)
   {
      RLOG_ARG0(L_ERROR,DBG_CELLID,cellDelInfo->u.cellDel.cellId, 
                "Cell does not exist");
      RETVALUE(RFAILED);
   }

   /* Free the active cell */
   rgSCHCfgFreeCellCb(cell);

   errInfo->errCause = RGSCHERR_NONE;
   RETVALUE(ROK);
}  /* rgSCHCfgRgrCellDel */


/**
 * @brief Handler for the UE delete request from RRM to MAC.
 *
 * @details
 *
 *     Function : rgSCHCfgRgrUeDel
 *
 *     Processing Steps:
 *      - Fetch the UE control block.
 *      - Remove the UE control block from the hash list of UEs for the cell.
 *      - Free the UE control block.
 *      - If successful, return ROK else return RFAILED.
 *
 *  @param[in]  RgSchCellCb    *cell
 *  @param[in]  RgrDel         *ueDelInfo
 *  @param[out] RgSchErrInfo   *errInfo
 *  @return  S16
 *      -# ROK
 *      -# RFAILED
 **/
#ifdef ANSI
PUBLIC S16 rgSCHCfgRgrUeDel
(
RgSchCellCb         *cell,
RgrDel              *ueDelInfo,
RgSchErrInfo        *errInfo
)
#else
PUBLIC S16 rgSCHCfgRgrUeDel(cell, ueDelInfo, errInfo)
RgSchCellCb         *cell;
RgrDel              *ueDelInfo;
RgSchErrInfo        *errInfo;
#endif
{
   RgSchUeCb        *ue;
   RgSchRaCb        *raCb;

   TRC2(rgSCHCfgRgrUeDel);
   
   errInfo->errCause = RGSCHERR_CFG_RGR_UE_DEL;

   if (cell->cellId != ueDelInfo->u.ueDel.cellId)
   {
      RLOG_ARG1(L_ERROR,DBG_CELLID,ueDelInfo->u.ueDel.cellId, 
                "Cell does not exist CRNTI:%d",
                ueDelInfo->u.ueDel.crnti);
      RETVALUE(RFAILED);
   }
   if ((ue = rgSCHDbmGetUeCb(cell, ueDelInfo->u.ueDel.crnti)) == NULLP)
   {
      if((raCb = rgSCHDbmGetRaCb(cell, ueDelInfo->u.ueDel.crnti)) == NULLP)
      {
         RLOG_ARG1(L_ERROR,DBG_CELLID,ueDelInfo->u.ueDel.cellId,
                  "RaCb does not exist for CRNTI:%d",ueDelInfo->u.ueDel.crnti);
         RETVALUE(RFAILED);
      }
      else
      {
         /* This happens in case of Msg4 rejection */
         raCb->toDel = TRUE;
         RETVALUE(ROK);
      }
   }
   else
   {
      /* Delete Ue from the UE list of CELL*/
      rgSCHDbmDelUeCb(cell, ue);

#ifdef LTE_L2_MEAS
      rgSCHDbmDelL2MUe(cell, ue);
#endif

      /* Call MeasGap and AckNakRep processing module */
      rgSCHMeasGapANRepUeDel(cell, ue, TRUE);

      /* ccpu00140894- Stop TXMode transiition timer if it is running*/
      if (ue->txModeTransTmr.tmrEvnt != TMR_NONE)
      {
         rgSCHTmrStopTmr(cell, RG_SCH_TMR_TXMODE_TRNSTN, ue);
      }

      /* Call DRX module to remove UEs from various
       * lists it maintain 
       */
      /* ccpu00129899 */
      if(ue->drxCb != NULLP)
      {
        (Void)rgSCHDrxUeDel(cell,ue);
        /* Free Ue */
      }
      /*Fix: If RA CB exists, delete it*/
      if((raCb = rgSCHDbmGetRaCb(cell, ueDelInfo->u.ueDel.crnti)) != NULLP)
      {
         /* Fix : syed RNTI was getting released twice, once by racb del 
          * and subsequently by ueDel. Let it get released by ueDel alone */       
         rgSCHRamDelRaCb(cell, raCb, FALSE);
      }
     
      rgSCHCfgFreeUeCb(cell, ue);

      errInfo->errCause = RGSCHERR_NONE;

      RETVALUE(ROK);
   }
}  /* rgSCHCfgRgrUeDel */


/**
 * @brief Handler for the logical channel delete request from
 * RRM to MAC.
 *
 * @details
 *
 *     Function : rgSCHCfgRgrLcDel
 *
 *     Processing Steps:
 *      - Fetch the logical channel control block.
 *      - Free the logical channel control block.
 *      - If successful, return ROK else return RFAILED.
 *
 *  @param[in]  RgrDel      *lcDelInfo
 *  @param[out] RgSchErrInfo   *errInfo
 *  @return  S16
 *      -# ROK
 *      -# RFAILED
 **/
#ifdef ANSI
PUBLIC S16 rgSCHCfgRgrLcDel
(
RgSchCellCb    *cell,
RgrDel         *lcDelInfo,
RgSchErrInfo   *errInfo
)
#else
PUBLIC S16 rgSCHCfgRgrLcDel(cell, lcDelInfo, errInfo)
RgSchCellCb    *cell;
RgrDel         *lcDelInfo;
RgSchErrInfo   *errInfo;
#endif
{
   RgSchUeCb    *ue;
   RgSchDlLcCb  *dlLc;
#ifdef LTE_L2_MEAS
   U8          lcId;
   U8          idx;
   RgSchUlLcCb  *ulLc; 
#endif

   TRC2(rgSCHCfgRgrLcDel);

   errInfo->errCause = RGSCHERR_CFG_RGR_LC_DEL;

   /* Fetch the Active cell */
   if (cell->cellId != lcDelInfo->u.lchDel.cellId)
   {
      RLOG_ARG1(L_ERROR,DBG_CELLID,cell->cellId,  "Cell does not exist %d",
                  lcDelInfo->u.lchDel.cellId);
      RETVALUE(RFAILED);
   }

   /* Fetch the Ue */
   if ((ue = rgSCHDbmGetUeCb(cell, lcDelInfo->u.lchDel.crnti)) == NULLP)
   {
      RLOG_ARG2(L_ERROR,DBG_CELLID,lcDelInfo->u.lchDel.cellId, 
                "UE does not exist for CRNTI:%d LCID:%d",
               lcDelInfo->u.lchDel.crnti,lcDelInfo->u.lchDel.lcId);
      RETVALUE(RFAILED);
   }
   if (lcDelInfo->u.lchDel.lcgId > 3)
   {
      RLOG_ARG3(L_ERROR,DBG_CELLID,lcDelInfo->u.lchDel.cellId, 
                "[%d]UEID:For LC %d, LCGid %d is invalid",
               lcDelInfo->u.lchDel.crnti,lcDelInfo->u.lchDel.lcId,
               lcDelInfo->u.lchDel.lcgId);
      RETVALUE(RFAILED);
   }
   if ((dlLc = rgSCHDbmGetDlDedLcCb(ue, lcDelInfo->u.lchDel.lcId))
         == NULLP)
   {
      RLOG_ARG2(L_ERROR,DBG_CELLID,lcDelInfo->u.lchDel.cellId,
                "LC does not exist for CRNTI:%d LCID:%d",
               lcDelInfo->u.lchDel.crnti, lcDelInfo->u.lchDel.lcId);
      RETVALUE(RFAILED);
   }
   rgSCHUtlRgrLcDel(cell, ue, lcDelInfo->u.lchDel.lcId,lcDelInfo->u.lchDel.lcgId); 

   /* Reduce any pending bo from this LC(if any) 
    * from the UE's total BO */
   if(dlLc->bo)
   {
      if(ue->totalBo >= dlLc->bo)
      {
         ue->totalBo -= dlLc->bo;
      }
      else
      {
        ue->totalBo = 0; /* this scenario should not occur */
      }
   }
   rgSCHDbmDelDlDedLcCb(ue, dlLc);
   rgSCHCfgFreeDlDedLcCb(cell, ue, dlLc);

#ifdef LTE_L2_MEAS
   lcId = lcDelInfo->u.lchDel.lcId;
   if (TRUE == ue->ul.lcCb[lcId -1].isValid)
   {
      ulLc = &(ue->ul.lcCb[lcId -1]);
      ue->ul.lcCb[lcId -1].isValid = FALSE;

      if((ulLc->qciCb->ulUeCount) &&
            (ue->ulActiveLCs & (1 << (ulLc->qciCb->qci -1))))
      {
         ulLc->qciCb->ulUeCount--;
         ue->ulActiveLCs &= ~(1 << (ulLc->qciCb->qci -1));
      }
      /* Shifting LCs in LCG Array because of LC deletion */
      for (idx = ulLc->lcgArrIdx +1; idx < ulLc->lcg->numLch;
               idx++)
      {
         ulLc->lcg->lcArray[idx -1] =
            ulLc->lcg->lcArray[idx];
         ulLc->lcg->lcArray[idx -1]->lcgArrIdx = idx -1;
      }
      ulLc->lcg->numLch--;
      ulLc->lcg->lcArray[idx -1] = NULLP;
   }
#endif /* LTE_L2_MEAS */


   errInfo->errCause = RGSCHERR_NONE;
   RETVALUE(ROK);
}  /* rgSCHCfgRgrLcDel */



/**
 * @brief Handler for the logical channel delete request from
 * RRM to MAC.
 *
 * @details
 *
 *     Function : rgSCHCfgRgrLcgDel
 *
 *     Processing Steps:
 *      - Fetch the logical channel control block.
 *      - Free the logical channel control block.
 *      - If successful, return ROK else return RFAILED.
 *
 *  @param[in]  RgrDel      *lcDelInfo
 *  @param[out] RgSchErrInfo   *errInfo
 *  @return  S16
 *      -# ROK
 *      -# RFAILED
 **/
#ifdef ANSI
PUBLIC S16 rgSCHCfgRgrLcgDel
(
RgSchCellCb    *cell,
RgrDel         *lcDelInfo,
RgSchErrInfo   *errInfo
)
#else
PUBLIC S16 rgSCHCfgRgrLcgDel(cell, lcDelInfo, errInfo)
RgSchCellCb    *cell;
RgrDel         *lcDelInfo;
RgSchErrInfo   *errInfo;
#endif
{
   RgSchUeCb    *ue = NULLP;
#ifdef LTE_L2_MEAS
   U8           lcCount = 0;
#endif
   U8           lcgId = 0;

   TRC2(rgSCHCfgRgrLcgDel);


   lcgId = lcDelInfo->u.lcgDel.lcgId;

   errInfo->errCause = RGSCHERR_CFG_RGR_LCG_DEL;

   /* Fetch the Active cell */
   if (cell->cellId != lcDelInfo->u.lcgDel.cellId)
   {
      RLOG_ARG2(L_ERROR,DBG_CELLID,lcDelInfo->u.lchDel.cellId,
                "CELL does not exist for CRNTI:%d LCGID:%d",
               lcDelInfo->u.lchDel.crnti,lcDelInfo->u.lchDel.lcId);
      RETVALUE(RFAILED);
   }

   /* Fetch the Ue */
   if ((ue = rgSCHDbmGetUeCb(cell, lcDelInfo->u.lcgDel.crnti)) == NULLP)
   {
      RLOG_ARG2(L_ERROR,DBG_CELLID,lcDelInfo->u.lchDel.cellId,
                "UE does not exist for CRNTI:%d LCGID:%d",
               lcDelInfo->u.lchDel.crnti,lcDelInfo->u.lchDel.lcId);
      RETVALUE(RFAILED);
   }

   /* set lcgId in UEs lcg cntrl blk to invalid */
   rgSCHUtlRgrLcgDel(cell, ue, lcgId);
   ue->ul.lcgArr[lcgId].lcgId = RGSCH_INVALID_LCG_ID;

#ifdef LTE_L2_MEAS
   /* Since LCs are being deleted, if any of them are contributing
      to Active UE count for a QCI, decrease the count */
   for (lcCount =0; (lcCount < RGSCH_MAX_LC_PER_UE) && 
         (lcCount < ue->ul.lcgArr[lcgId].numLch); lcCount++)
   {
      if (ue->ul.lcgArr[lcgId].lcArray[lcCount])
      {
         if((ue->ul.lcgArr[lcgId].
                  lcArray[lcCount]->qciCb->ulUeCount) &&
            (ue->ulActiveLCs &
             (1 << ((ue->ul.lcgArr[lcgId].
                    lcArray[lcCount])->qciCb->qci -1))))
            {
               /* L2_COUNTERS */
               ue->ul.lcgArr[lcgId].
                  lcArray[lcCount]->qciCb->ulUeCount--;
               ue->ulActiveLCs &= ~(1 << 
                     (ue->ul.lcgArr[lcgId].
                      lcArray[lcCount]->qciCb->qci -1));
            }
      }
   }
#endif
   
   errInfo->errCause = RGSCHERR_NONE;
   RETVALUE(ROK);
}  /* rgSCHCfgRgrLcgDel */



/***********************************************************
 *
 *     Func : rgSCHCfgVldtRgrLcCfg
 *
 *
 *     Desc : Validates dedicated logical channel configuration recieved from RRM.
 *
 *     Ret  : S16
 *            ROK - Success
 *            RFAILED - Failed
 *
 *     Notes:
 *
 *     File :
 *
 **********************************************************/
#ifdef ANSI
PUBLIC S16 rgSCHCfgVldtRgrLcCfg
(
Inst             inst,
RgrLchCfg     *lcCfg,
RgSchCellCb      **cell,
RgSchUeCb        **ue,
RgSchErrInfo     *errInfo
)
#else
PUBLIC S16 rgSCHCfgVldtRgrLcCfg(inst, lcCfg, cell, ue, errInfo)
Inst             inst;
RgrLchCfg     *lcCfg;
RgSchCellCb      **cell;
RgSchUeCb        **ue;
RgSchErrInfo     *errInfo;
#endif
{
   TRC2(rgSCHCfgVldtRgrLcCfg);


   errInfo->errCause = RGSCHERR_CFG_INVALID_RGR_DED_LC_CFG;

   if (((*cell) == NULLP) ||
       ((*cell)->cellId != lcCfg->cellId))
   {
      RLOG_ARG2(L_ERROR,DBG_CELLID,lcCfg->cellId,"Cell does not existi for "
               "CRNTI:%d LCID:%d",lcCfg->crnti, lcCfg->lcId);
      RETVALUE(RFAILED);
   }

   /* Fetch the Ue */
   if ((*ue = rgSCHDbmGetUeCb(*cell, lcCfg->crnti)) == NULLP)
   {
      RLOG_ARG2(L_ERROR,DBG_CELLID,lcCfg->cellId,"UE does not exist for dedicated"
         " logical channel CRNTI:%d LCID:%d", lcCfg->crnti, lcCfg->lcId);
      RETVALUE(RFAILED);
   }

   /* Validate logical channel Id */
   if ((lcCfg->lcId < RGSCH_DEDLC_MIN_LCID)
            ||(lcCfg->lcId > RGSCH_DEDLC_MAX_LCID))
   {
      RLOG_ARG2(L_ERROR,DBG_CELLID,lcCfg->cellId,"Invalid logical channel Id:%d"
               "for CRNTI:%d",lcCfg->lcId,lcCfg->crnti);
      RETVALUE(RFAILED);
   }

   if (lcCfg->lcType != CM_LTE_LCH_DTCH && lcCfg->lcType != CM_LTE_LCH_DCCH)
   {
      RLOG_ARG3(L_ERROR,DBG_CELLID,lcCfg->cellId,"Invalid logical channel Type %d"
               "CRNTI:%d LCID:%d",lcCfg->lcType,lcCfg->crnti, lcCfg->lcId);
      RETVALUE(RFAILED);
   }

   RETVALUE(ROK);
}  /* rgSCHCfgVldtRgrLcCfg */

/***********************************************************
 *
 *     Func : rgSCHCfgVldtRgrLcgCfg
 *
 *
 *     Desc : Validates dedicated logical channel group configuration recieved from RRM.
 *
 *     Ret  : S16
 *            ROK - Success
 *            RFAILED - Failed
 *
 *     Notes:
 *
 *     File :
 *
 **********************************************************/
#ifdef ANSI
PUBLIC S16 rgSCHCfgVldtRgrLcgCfg
(
Inst             inst,
RgrLcgCfg        *lcgCfg,
RgSchCellCb      **cell,
RgSchUeCb        **ue,
RgSchErrInfo     *errInfo
)
#else
PUBLIC S16 rgSCHCfgVldtRgrLcgCfg(inst, lcgCfg, cell, ue, errInfo)
Inst             inst;
RgrLcgCfg        *lcgCfg;
RgSchCellCb      **cell;
RgSchUeCb        **ue;
RgSchErrInfo     *errInfo;
#endif
{
   TRC2(rgSCHCfgVldtRgrLcgCfg);


   errInfo->errCause = RGSCHERR_CFG_INVALID_RGR_DED_LCG_CFG;

   if (((*cell) == NULLP) ||
       ((*cell)->cellId != lcgCfg->cellId))
   {
      RLOG_ARG2(L_ERROR,DBG_CELLID,lcgCfg->cellId,"Cell does not exist for"
               "CRNTI:%d LCGID:%d",lcgCfg->crnti,lcgCfg->ulInfo.lcgId);
      RETVALUE(RFAILED);
   }

   /* Fetch the Ue */
   if ((*ue = rgSCHDbmGetUeCb(*cell, lcgCfg->crnti)) == NULLP)
   {
      RLOG_ARG2(L_ERROR,DBG_CELLID,lcgCfg->cellId,"UE does not exist for "
        "dedicated logical channel CRNTI:%d LCGID:%d", lcgCfg->crnti, lcgCfg->ulInfo.lcgId);
      RETVALUE(RFAILED);
   }
   
   if ((lcgCfg->ulInfo.gbr != 0) && (lcgCfg->ulInfo.mbr < lcgCfg->ulInfo.gbr))
   {
     RGSCHDBGINFO(inst, (rgSchPBuf(inst), "Dedicated Logical Group %d validation failed"
      " for ue %d for cell %d\n", lcgCfg->ulInfo.lcgId, lcgCfg->crnti, lcgCfg->cellId));
     RETVALUE(RFAILED);
   }
   
   RETVALUE(ROK);
}  /* rgSCHCfgVldtRgrLcgCfg */


/***********************************************************
 *
 *     Func : rgSCHCfgVldtRgrCellPwrCfg
 *
 *     Desc : Validates cell power configuration.
 *
 *     Ret  : S16
 *            ROK - Success
 *            RFAILED - Failed
 *
 *     Notes:
 *
 *     File :
 *
 **********************************************************/
#ifdef ANSI
PRIVATE S16 rgSCHCfgVldtRgrCellPwrCfg
(
Inst             inst,
RgrCellCfg       *cellCfg,
RgSchErrInfo     *errInfo
)
#else
PRIVATE S16 rgSCHCfgVldtRgrCellPwrCfg(inst, cellCfg, errInfo)
Inst             inst;
RgrCellCfg       *cellCfg;
RgSchErrInfo     *errInfo;
#endif
{
   UNUSED(inst);
   UNUSED(cellCfg);
   UNUSED(errInfo);

   TRC2(rgSCHCfgVldtRgrCellPwrCfg);

   /* This function does nothing now, placeholder for
    * subsequent power config validations that may be needed */


   RETVALUE(ROK);
}  /* rgSCHCfgVldtRgrCellPwrCfg */


/***********************************************************
 *
 *     Func : rgSCHCfgVldtRgrCmnLcCfg
 *
 *
 *     Desc : Validates common logical channel configuration recieved from RRM.
 *
 *  @param[in]  Inst         inst
 *  @param[in]  RgrCellCfg   *cellCfg
 *  @param[out] RgSchErrInfo *errInfo
 *     Ret  : S16
 *            ROK - Success
 *            RFAILED - Failed
 *
 *     Notes:
 *
 *     File :
 *
 **********************************************************/
#ifdef ANSI
PRIVATE S16 rgSCHCfgVldtRgrCmnLcCfg
(
Inst             inst,
RgrCellCfg       *cellCfg,
RgSchErrInfo     *errInfo
)
#else
PRIVATE S16 rgSCHCfgVldtRgrCmnLcCfg(inst, cellCfg, errInfo)
Inst             inst;
RgrCellCfg       *cellCfg;
RgSchErrInfo     *errInfo;
#endif
{
   U8            idx;
   RgrCmnLchCfg  *lcCfg; 
   U8            dirVld  = FALSE;
   U8            bitMask = 0x00;
   U8            cnt=0;

   TRC2(rgSCHCfgVldtRgrCmnLcCfg);

   errInfo->errCause = RGSCHERR_CFG_INVALID_RGR_CMN_LC_CFG;
   
   for (idx = 0; idx < cellCfg->numCmnLcs; idx++)
   {
      lcCfg = &(cellCfg->cmnLcCfg[idx]);
      /* Validate downlink info */
      if (lcCfg->dir & RGR_DIR_TX)
      {
         if (lcCfg->lcType == CM_LTE_LCH_BCCH)
         {
            if (lcCfg->dlTrchType == CM_LTE_TRCH_DL_SCH)
            {
               if(cnt == 0)
               {
                  bitMask |= RGSCH_BCCH_DLSCH_CFG1;
                  cnt++;
               }
               else
               {

                  if((
                     (cellCfg->siCfg.siWinSize == 1)  ||
                     (cellCfg->siCfg.siWinSize == 2)  ||
                     (cellCfg->siCfg.siWinSize == 5)  ||
                     (cellCfg->siCfg.siWinSize == 10) ||
                     (cellCfg->siCfg.siWinSize == 15) ||
                     (cellCfg->siCfg.siWinSize == 20) ||
                     (cellCfg->siCfg.siWinSize == 40)) &&
                     (cellCfg->siCfg.retxCnt>0)
                    )
                  {
                     bitMask |= RGSCH_BCCH_DLSCH_CFG2;
                  }
                  else
                  {
                     RLOG_ARG0(L_ERROR,DBG_CELLID,cellCfg->cellId,
                           "Invalid si config for cell");
                     RETVALUE(RFAILED);
                  }
               }
            }
            else if (lcCfg->dlTrchType == CM_LTE_TRCH_BCH)
            {
               bitMask |= RGSCH_BCCH_BCH_CFG;
            }
            else
            {
               RLOG_ARG1(L_ERROR,DBG_CELLID, cellCfg->cellId,
                     "Invalid transport channel %d for cell", lcCfg->dlTrchType);
               RETVALUE(RFAILED);
            }
         }
         else if (lcCfg->lcType == CM_LTE_LCH_PCCH)
         {
            bitMask |= RGSCH_PCCH_CFG;
         }
         else if (lcCfg->lcType == CM_LTE_LCH_CCCH)
         {
            bitMask |= RGSCH_DL_CCCH_CFG;
         }
         dirVld = TRUE;
      }

      /* Validate uplink info */
      if (lcCfg->dir & RGR_DIR_RX)
      {
         /* Uplink CCCH */
         if (lcCfg->lcType != CM_LTE_LCH_CCCH)
         {
            RLOG_ARG1(L_ERROR,DBG_CELLID, cellCfg->cellId,"Invalid UL common lcType %d "
             "for cell", lcCfg->lcType);
            RETVALUE(RFAILED);
         }
         else
         {
            bitMask |= RGSCH_UL_CCCH_CFG;
         }
         dirVld = TRUE;
      }

      /* Invalid direction */
      if (!dirVld)
      {
         RLOG_ARG1(L_ERROR,DBG_CELLID, cellCfg->cellId,"Invalid Direction %d", 
            lcCfg->dir);
         RETVALUE(RFAILED);
      }
   }
   if (bitMask != RGSCH_CELL_ACTIVE_CFG)
   {
      RLOG_ARG0(L_ERROR,DBG_CELLID, cellCfg->cellId,
                "Invalid Common channel config for cell");
      RETVALUE(RFAILED);
   }

   RETVALUE(ROK);
}  /* rgSCHCfgVldtRgrCmnLcCfg */


/***********************************************************
 *
 *     Func : rgSCHCfgVldtUeCqiModeCfg
 *
 *
 *     Desc : Validates UE CQI modes Configuration recieved from RRC.
 *
 *     Ret  : S16
 *            ROK - Success
 *            RFAILED - Failed
 *
 *     Notes:
 *
 *     File :
 *
 **********************************************************/
#ifdef ANSI
PRIVATE S16 rgSCHCfgVldtUeCqiModeCfg
(
RgSchCellCb       *cell,
RgrUeCfg          *ueCfg
)
#else
PRIVATE S16 rgSCHCfgVldtUeCqiModeCfg(cell, ueCfg)
RgSchCellCb       *cell;
RgrUeCfg          *ueCfg;
#endif
{
   
    TRC2(rgSCHCfgVldtUeCqiModeCfg)

#ifndef TFU_UPGRADE
   if((ueCfg->ueDlCqiCfg.prdCqiCfg.cqiPmiCfgIdx < 1) || 
         (ueCfg->ueDlCqiCfg.prdCqiCfg.cqiPmiCfgIdx > 1024))
   {
      RLOG_ARG1(L_ERROR,DBG_CELLID,cell->cellId,
                  "Invalid Periodic CQI Info for CRNTI:%d",
               (U8)ueCfg->crnti);
      RETVALUE(RFAILED);
   }
#endif
   /* Validate UE Aperiodic CQI mode */
   if ((ueCfg->ueDlCqiCfg.aprdCqiCfg.pres == TRUE) &&
       ((ueCfg->ueDlCqiCfg.aprdCqiCfg.aprdModeEnum > RGR_APRD_CQI_MOD31) ||
        (cell->bwCfg.dlTotalBw <= 7)))
   {
      RLOG_ARG2(L_ERROR,DBG_CELLID,cell->cellId,
                  "Invalid Aperiodic mode config for DL CQI for CRNTI:%d",
            ueCfg->ueDlCqiCfg.aprdCqiCfg.aprdModeEnum,ueCfg->crnti);
      RETVALUE(RFAILED);
   }
#ifndef TFU_UPGRADE
   /* Validate UE Periodic CQI mode */ 
   if (ueCfg->ueDlCqiCfg.prdCqiCfg.prdModeEnum > RGR_PRD_CQI_MOD21)
   {
      RLOG_ARG2(L_ERROR,DBG_CELLID,cell->cellId,
                  "Invalid periodic mode config for DL CQI for UE %d",
            ueCfg->ueDlCqiCfg.prdCqiCfg.prdModeEnum,ueCfg->crnti);
      RETVALUE(RFAILED);
   }
   /* Validate K value in periodic CQI Config */
   if(((ueCfg->ueDlCqiCfg.prdCqiCfg.prdModeEnum == RGR_PRD_CQI_MOD20) ||
            (ueCfg->ueDlCqiCfg.prdCqiCfg.prdModeEnum == RGR_PRD_CQI_MOD21)) && 
         ((ueCfg->ueDlCqiCfg.prdCqiCfg.k < 1)||
            (ueCfg->ueDlCqiCfg.prdCqiCfg.k > 4)))
   {
      RLOG_ARG1(L_ERROR,DBG_CELLID,cell->cellId,
                  "Invalid K for Subband CQI reporting for CRNTI:%d",
               (U8)ueCfg->crnti);
      RETVALUE(RFAILED);
   }
#else
   if (ueCfg->ueDlCqiCfg.prdCqiCfg.cqiSetup.prdModeEnum > RGR_PRD_CQI_MOD21)
   {
     RLOG_ARG2(L_ERROR,DBG_CELLID,cell->cellId,
                 "Invalid periodic mode config for DL CQI for CRNTI:%d",
                 ueCfg->ueDlCqiCfg.prdCqiCfg.cqiSetup.prdModeEnum,ueCfg->crnti);
     RETVALUE(RFAILED);
   }

#endif
   
   RETVALUE(ROK);
      
}
/***********************************************************
 *
 *     Func : rgSCHCfgVldtUeMeasGapAckNakRepCfg
 *
 *
 *     Desc : Validates UE Measurement Gap and Ack Nack Repetition Configuration recieved from RRC.
 *
 *     Ret  : S16
 *            ROK - Success
 *            RFAILED - Failed
 *
 *     Notes:
 *
 *     File :
 *
 **********************************************************/
#ifdef ANSI
PRIVATE S16 rgSCHCfgVldtUeMeasGapAckNakRepCfg
(
RgSchCellCb       *cell,
RgrUeCfg          *ueCfg
)
#else
PRIVATE S16 rgSCHCfgVldtUeMeasGapAckNakRepCfg(cell, ueCfg)
RgSchCellCb       *cell;
RgrUeCfg          *ueCfg;
#endif
{

   TRC2(rgSCHCfgVldtUeMeasGapAckNakRepCfg)
      
#ifdef LTE_TDD
   if ((ueCfg->ackNackModeEnum == RGR_TDD_ACKNACK_MODE_MULT) &&
         (ueCfg->ueAckNackCfg.isAckNackEnabled == TRUE))
   {
      RLOG_ARG2(L_ERROR,DBG_CELLID,ueCfg->cellId,"TDD ACK NACK Multiplexing Mode"
      "is not allowed when Ack/Nack is Enabled: %d CRNTI:%d",
       ueCfg->ueAckNackCfg.ackNackRepFactor,ueCfg->crnti);
      RETVALUE(RFAILED);
   }
#endif /* LTE_TDD */
   /* Validate AckNackRep Factor */
   if((ueCfg->ueAckNackCfg.isAckNackEnabled == FALSE) &&
         (!ueCfg->ueMesGapCfg.isMesGapEnabled))
   {
      RETVALUE(ROK);
   }

   if(ueCfg->ueAckNackCfg.isAckNackEnabled)
   {
   if ( (ueCfg->ueAckNackCfg.ackNackRepFactor < RGR_ACKNACK_REPFACT_N2)
         || (ueCfg->ueAckNackCfg.ackNackRepFactor > RGR_ACKNACK_REPFACT_N6))
   {
      RLOG_ARG2(L_ERROR,DBG_CELLID,ueCfg->cellId, "Invalid ACK NACK REP Factor:%d CRNTI:%d",
               ueCfg->ueAckNackCfg.ackNackRepFactor,ueCfg->crnti);
      RETVALUE(RFAILED);
   }
   }
   if(ueCfg->ueMesGapCfg.isMesGapEnabled)
   {
   switch(ueCfg->ueMesGapCfg.gapPrd)
   {
      case RG_MEAS_GAPPRD_40:
         if(ueCfg->ueMesGapCfg.gapOffst >= RG_MEAS_GAPPRD_40)
         {
            RLOG_ARG2(L_ERROR,DBG_CELLID,ueCfg->cellId,"Invalid GAP Offset:%d CRNTI:%d",
                     ueCfg->ueMesGapCfg.gapOffst,ueCfg->crnti);
            RETVALUE(RFAILED);
         }
         break;
      case RG_MEAS_GAPPRD_80:
         if(ueCfg->ueMesGapCfg.gapOffst >= RG_MEAS_GAPPRD_80)
         {
            RLOG_ARG2(L_ERROR,DBG_CELLID,ueCfg->cellId,"Invalid GAP Offset:%d CRNTI:%d",
                     ueCfg->ueMesGapCfg.gapOffst,ueCfg->crnti);
            RETVALUE(RFAILED);
         }
         break;
         default:
         { 
            RLOG_ARG2(L_ERROR,DBG_CELLID,ueCfg->cellId,"Invalid GAP Periodicity Settings:%d"
                      "CRNTI:%d", ueCfg->ueMesGapCfg.gapPrd,ueCfg->crnti);
            RETVALUE(RFAILED);
         }
      }
   }

   RETVALUE(ROK);
}  /* rgSCHCfgVldtUeMeasGapAckNakRepCfg*/


/***********************************************************
 *
 *     Func : rgSCHCfgVldtUeMeasGapAckNakRepRecfg
 *
 *
 *     Desc : Validates UE Measurement Gap and Ack Nack Repetition Configuration recieved from RRC.
 *
 *     Ret  : S16
 *            ROK - Success
 *            RFAILED - Failed
 *
 *     Notes:
 *
 *     File :
 *
 **********************************************************/
#ifdef ANSI
PRIVATE S16 rgSCHCfgVldtUeMeasGapAckNakRepRecfg
(
RgSchCellCb       *cell,
RgrUeRecfg        *ueRecfg
)
#else
PRIVATE S16 rgSCHCfgVldtUeMeasGapAckNakRepRecfg(cell, ueRecfg)
RgSchCellCb       *cell;
RgrUeRecfg        *ueRecfg;
#endif
{

   TRC2(rgSCHCfgVldtUeMeasGapAckNakRepRecfg)
   if((ueRecfg->ueAckNackRecfg.isAckNackEnabled == FALSE) &&
          (!ueRecfg->ueMeasGapRecfg.isMesGapEnabled))
   {
      RETVALUE(ROK);
   }

   if(ueRecfg->ueAckNackRecfg.isAckNackEnabled )
   {
   /* Validate AckNackRep Factor */
   if ( (ueRecfg->ueAckNackRecfg.ackNackRepFactor < RGR_ACKNACK_REPFACT_N2)
         || (ueRecfg->ueAckNackRecfg.ackNackRepFactor > RGR_ACKNACK_REPFACT_N6))
   {
      RLOG_ARG2(L_ERROR,DBG_CELLID,ueRecfg->cellId,"Invalid ACK NACK REP Factor:%d"
               "NEW CRNTI:%d",ueRecfg->ueAckNackRecfg.ackNackRepFactor,ueRecfg->newCrnti);
      RETVALUE(RFAILED);
   }
   }
   if(ueRecfg->ueMeasGapRecfg.isMesGapEnabled)
   {
   switch(ueRecfg->ueMeasGapRecfg.gapPrd)
   {
      case RG_MEAS_GAPPRD_40:
         if(ueRecfg->ueMeasGapRecfg.gapOffst >= RG_MEAS_GAPPRD_40)
         {
            RLOG_ARG2(L_ERROR,DBG_CELLID,ueRecfg->cellId,"Invalid GAP Offset:%d"
                     "NEW CRNTI:%d",ueRecfg->ueMeasGapRecfg.gapOffst,ueRecfg->newCrnti);
            RETVALUE(RFAILED);
         }
         break;
      case RG_MEAS_GAPPRD_80:
         if(ueRecfg->ueMeasGapRecfg.gapOffst >= RG_MEAS_GAPPRD_80)
         {
            RLOG_ARG2(L_ERROR,DBG_CELLID,ueRecfg->cellId,"Invalid GAP Offset:%d"
                     "NEW CRNTI:%d",ueRecfg->ueMeasGapRecfg.gapOffst,ueRecfg->newCrnti);
            RETVALUE(RFAILED);
         }
         break;
         default:
         { 
            RLOG_ARG2(L_ERROR,DBG_CELLID,ueRecfg->cellId,"Invalid GAP Periodicity Settings:%d"
                     "NEW CRNTI:%d",ueRecfg->ueMeasGapRecfg.gapPrd,ueRecfg->newCrnti);
            RETVALUE(RFAILED);
         }
      }
   }

   RETVALUE(ROK);
}  /* rgSCHCfgVldtUeMeasGapAckNakRepRecfg*/

#ifdef LTEMAC_SPS
/***********************************************************
 *
 *     Func : rgSCHCfgVldtUeDlSpsCfg
 *
 *
 *     Desc : Validates UE's DL SPS configuration recieved from RRC.
 *
 *     Ret  : S16
 *            ROK - Success
 *            RFAILED - Failed
 *
 *     Notes:
 *
 *     File :
 *
 **********************************************************/
#ifdef ANSI
PRIVATE S16 rgSCHCfgVldtUeDlSpsCfg
(
RgSchCellCb       *cell,
RgrUeSpsDlCfg     *dlSpsCfg
)
#else
PRIVATE S16 rgSCHCfgVldtUeDlSpsCfg(cell, dlSpsCfg)
RgSchCellCb       *cell;
RgrUeSpsDlCfg     *dlSpsCfg;
#endif
{

   U8                 idx = 0;

   TRC2(rgSCHCfgVldtUeDlSpsCfg);

   /* peridicity validation done in SPS module */
   if ((dlSpsCfg->numPucchVal > RG_SCH_MAX_NUM_N1PUCCH_PER_UE) ||
         (dlSpsCfg->numPucchVal == 0))
   {
      RLOG_ARG0(L_ERROR,DBG_CELLID,cell->cellId,"Invalid number of n1Pucch values"
               " in DL SPS Config");
      RETVALUE(RFAILED);
   }

   for (idx = 0; idx < dlSpsCfg->numPucchVal; ++idx)
   {
      if (dlSpsCfg->n1PucchVal[idx] > RG_SCH_MAX_N1PUCCH_VAL)
      {
#ifdef ALIGN_64BIT
         RLOG_ARG1(L_ERROR,DBG_CELLID,cell->cellId,"Invalid N1Pucch value"
                  " in DL SPS Config %u", dlSpsCfg->n1PucchVal[idx]);
#else
         RLOG_ARG1(L_ERROR,DBG_CELLID,cell->cellId,"Invalid N1Pucch value"
                  " in DL SPS Config %lu", dlSpsCfg->n1PucchVal[idx]);
#endif
         RETVALUE(RFAILED);
      }
   }
   /* SPS_TODO: check will change for TDD */
   if ((dlSpsCfg->numSpsHqProc == 0) ||
         (dlSpsCfg->numSpsHqProc > RGSCH_MAX_DL_HQ_PROC))
   {
      RLOG_ARG0(L_ERROR,DBG_CELLID,cell->cellId,"Invalid number of SPS HARQ procs"
               " in DL SPS Config");
      RETVALUE(RFAILED);
   }

   RETVALUE(ROK);
}  /* rgSCHCfgVldtDlSpsCfg */
#endif /* LTEMAC_SPS */

/***********************************************************
 *
 *     Func : rgSCHCfgVldtUePwrCfg
 *
 *
 *     Desc : Validates UE Group power configuration recieved from RRC.
 *
 *     Ret  : S16
 *            ROK - Success
 *            RFAILED - Failed
 *
 *     Notes:
 *
 *     File :
 *
 **********************************************************/
#ifdef ANSI
PRIVATE S16 rgSCHCfgVldtUePwrCfg
(
RgSchCellCb       *cell,
RgrUeUlPwrCfg     *pwrCfg
)
#else
PRIVATE S16 rgSCHCfgVldtUePwrCfg(cell, pwrCfg)
RgSchCellCb       *cell;
RgrUeUlPwrCfg     *pwrCfg;
#endif
{

   TRC2(rgSCHCfgVldtUePwrCfg);

   /* Group power control works only in accumulated mode */
   if (!pwrCfg->isAccumulated)
   {
      /* Fix */
      if (pwrCfg->uePuschPwr.pres)
      {
         RLOG_ARG0(L_ERROR,DBG_CELLID,cell->cellId,"Accumulation configutation"
              " not in sync with group power configuration");
         RETVALUE(RFAILED);
      }
   }

   if (rgSCHCfgVldtUeGrpPwrCfg(cell, &pwrCfg->uePuschPwr) != ROK)
   {
      RLOG_ARG0(L_ERROR,DBG_CELLID,cell->cellId,"Invalid PUSCH Group power"
           " configuration");
      RETVALUE(RFAILED);
   }
   if (rgSCHCfgVldtUeGrpPwrCfg(cell, &pwrCfg->uePucchPwr) != ROK)
   {
      RLOG_ARG0(L_ERROR,DBG_CELLID,cell->cellId,"Invalid PUSCH Group power"
           " configuration");
      RETVALUE(RFAILED);
   }

   RETVALUE(ROK);
}  /* rgSCHCfgVldtUePwrCfg */

/***********************************************************
 *
 *     Func : rgSCHCfgVldtUeGrpPwrCfg
 *
 *
 *     Desc : Validates UE Group power configuration recieved from RRC.
 *
 *     Ret  : S16
 *            ROK - Success
 *            RFAILED - Failed
 *
 *     Notes:
 *
 *     File :
 *
 **********************************************************/
#ifdef ANSI
PRIVATE S16 rgSCHCfgVldtUeGrpPwrCfg
(
RgSchCellCb       *cell,
RgrUeGrpPwrCfg *grpPwrCfg
)
#else
PRIVATE S16 rgSCHCfgVldtUeGrpPwrCfg(cell, grpPwrCfg)
RgSchCellCb       *cell;
RgrUeGrpPwrCfg *grpPwrCfg;
#endif
{

   TRC2(rgSCHCfgVldtUeGrpPwrCfg);

   if ((grpPwrCfg->pres) &&
       (((grpPwrCfg->tpcRnti > cell->rntiDb.rntiStart) &&
        ((grpPwrCfg->tpcRnti <
                     (cell->rntiDb.rntiStart + cell->rntiDb.maxRntis))))))
   {
      RLOG_ARG0(L_ERROR,DBG_CELLID,cell->cellId,"Invalid Uplink Group power " 
             "configuration");
      RETVALUE(RFAILED);
   }

   RETVALUE(ROK);
}  /* rgSCHCfgVldtUeGrpPwrCfg */

#ifdef LTEMAC_SPS
/***********************************************************
 *
 *     Func : rgSCHCfgVldtSpsReCfg
 *
 *
 *     Desc : Validates UE SPS and other SPS dependent 
 *     configuration recieved from RRC.
 *
 *     Ret  : S16
 *            ROK - Success
 *            RFAILED - Failed
 *
 *     Notes:
 *
 *     File :
 *
 **********************************************************/
#ifdef ANSI
PRIVATE S16 rgSCHCfgVldtSpsReCfg
(
RgSchCellCb       *cell,
RgSchUeCb         *ue,
RgrUeRecfg        *ueRecfg
)
#else
PRIVATE S16 rgSCHCfgVldtSpsReCfg(cell, ue, ueRecfg)
RgSchCellCb       *cell;
RgSchUeCb         *ue;
RgrUeRecfg     *ueRecfg;
#endif
{

   TRC2(rgSCHCfgVldtSpsReCfg);
   if ((ueRecfg->ueRecfgTypes & RGR_UE_DLSPS_RECFG) &&
         (ueRecfg->ueSpsRecfg.dlSpsCfg.isDlSpsEnabled)) 
   {
      /* Validating SPS RNTI */ 
      if (((ueRecfg->ueSpsRecfg.spsRnti >=  cell->rntiDb.rntiStart) && 
          (ueRecfg->ueSpsRecfg.spsRnti<=
          (cell->rntiDb.rntiStart+cell->rntiDb.maxRntis)))||
          (ueRecfg->ueSpsRecfg.spsRnti == RGSCH_SI_RNTI) ||
          (ueRecfg->ueSpsRecfg.spsRnti == RGSCH_P_RNTI))
      {
         RLOG_ARG2(L_ERROR,DBG_CELLID,ueRecfg->cellId,"Invalid SPS RNTI "
                  " in DL SPS Recfg OLD CRNTI:%d NEW CCRNTI:%d",
                  ueRecfg->oldCrnti,ueRecfg->newCrnti);
         RETVALUE(RFAILED);
      }
      if (rgSCHCfgVldtUeDlSpsCfg(cell, &ueRecfg->ueSpsRecfg.dlSpsCfg) != ROK)
      {
         RLOG_ARG2(L_ERROR,DBG_CELLID,ueRecfg->cellId,"Invalid DL SPS configuration"
                  " for the OLD CRNTI:%d NEW CRNTI:%d",
                  ueRecfg->oldCrnti,ueRecfg->newCrnti);
         RETVALUE(RFAILED);
      }
   }

#if RG_SPS_UNUSED 
   if(ueRecfg->ueSpsRecfg.dlSpsCfg.isDlSpsEnabled)
   {
     if (ueRecfg->ueRecfgTypes & RGR_UE_DRX_RECFG)
     {
     /* ccpu00117035 - MOD - changed instIdx to inst */
     /* ccpu00117035 - MOD - changed ueID to oldCrnti*/
      RLOG_ARG2(L_ERROR,DBG_CELLID,ueRecfg->cellId, 
               " DRX reconfig not supported DL SPS enabled for OLD CRNTI:%d NEW CRNTI:%d", 
                  ueRecfg->oldCrnti,ueRecfg->newCrnti);
      RETVALUE(RFAILED);
     }
   }
#endif
/* ccpu00117627 - ADD - SPS recfg validation against HDFDD */
#ifdef LTEMAC_HDFDD
   if(ueRecfg->ueSpsRecfg.dlSpsCfg.isDlSpsEnabled)
   {
      if(ue->hdFddEnbld == TRUE)
      {
         RLOG_ARG2(L_ERROR,DBG_CELLID,ueRecfg->cellId,
                  "DL SPS is not supported for HDFDD enabled for OLD CRNTI:%d NEW CRNTI:%d",
                  ueRecfg->oldCrnti,ueRecfg->newCrnti);
         RETVALUE(RFAILED);
      }
   }
   if(ueRecfg->ueSpsRecfg.ulSpsCfg.isUlSpsEnabled)
   {
      if(ue->hdFddEnbld == TRUE)
      {
         RLOG_ARG2(L_ERROR,DBG_CELLID,ueRecfg->cellId,
                  "UL SPS is not supported for HDFDD enabled for OLD CRNTI:%d NEW CRNTI:%d",
                  ueRecfg->oldCrnti,ueRecfg->newCrnti);
         RETVALUE(RFAILED);
      }
   }
#endif

   RETVALUE(ROK);
}  /*rgSCHCfgVldtSpsReCfg*/
#endif

#if ((defined (RGR_CQI_REPT)) && (defined (RGR_V2)))
/***********************************************************
 *
 *     Func : rgSCHCfgVldtCqiReptReCfg
 *
 *
 *     Desc : Validates UE CQI report for DL Power control
 *     configuration recieved from RRC.
 *
 *     Ret  : S16
 *            ROK - Success
 *            RFAILED - Failed
 *
 *     Notes:
 *
 *     File :
 *
 **********************************************************/
#ifdef ANSI
PRIVATE S16 rgSCHCfgVldtCqiReptReCfg
(
RgSchCellCb       *cell,
RgrUeRecfg     *ueRecfg
)
#else
PRIVATE S16 rgSCHCfgVldtCqiReptReCfg(cell, ueRecfg)
RgSchCellCb       *cell;
RgrUeRecfg     *ueRecfg;
#endif
{

   TRC2(rgSCHCfgVldtCqiReptReCfg);
   /* Validate DL Power Control Config parameters */
   if (ueRecfg->ueCqiReptCfg.numColltdCqiRept > RGR_CQIRPTS_MAXN)
   {
      RLOG_ARG3(L_ERROR,DBG_CELLID,ueRecfg->cellId,"Invalide numColltdCqiRept,"
               "MAX supported %d for OLD CRNTI:%d NEW CRNTI:%d",RGR_CQIRPTS_MAXN,
                  ueRecfg->oldCrnti,ueRecfg->newCrnti);
      RETVALUE(RFAILED);
   }

   RETVALUE(ROK);
}  /*rgSCHCfgVldtCqiReptReCfg*/
#endif

/***********************************************************
 *
 *     Func : rgSCHCfgRgrLcChfg
 *
 *
 *     Desc : Handles dedicated logical channel configuration 
 *     recieved from RRC.
 *
 *     Ret  : S16
 *            ROK - Success
 *            RFAILED - Failed
 *
 *     Notes:
 *
 *     File :
 *
 **********************************************************/
#ifdef ANSI
PUBLIC S16 rgSCHCfgRgrLchCfg
(
RgSchCellCb      *cell,
RgSchUeCb        *ue,
RgrLchCfg     *lcCfg,
RgSchErrInfo     *errInfo
)
#else
PUBLIC S16 rgSCHCfgRgrLchCfg(cell, ue, lcCfg, errInfo)
RgSchCellCb      *cell;
RgSchUeCb        *ue;
RgrLchCfg     *lcCfg;
RgSchErrInfo     *errInfo;
#endif
{
   S16       ret;
   RgSchDlLcCb  *dlLc = NULLP;
   Inst    inst = cell->instIdx;
#ifdef LTE_L2_MEAS
   RgSchUlLcCb  *ulLc; 
#endif

   TRC2(rgSCHCfgRgrLchCfg);

   errInfo->errCause = RGSCHERR_CFG_RGR_DED_LC_CFG;

   /* Allocate the downlink logical channel control block */
   if((ret = rgSCHUtlAllocSBuf(inst, (Data**)&dlLc, 
               sizeof(RgSchDlLcCb))) != ROK)
   {
      RLOG_ARG2(L_ERROR,DBG_CELLID,lcCfg->cellId,"Memory allocation FAILED for "
               "Downlink LCId:%d CRNTI:%d", lcCfg->lcId,lcCfg->crnti);
      RETVALUE(RFAILED);
   }
   if ((U8 *)dlLc == NULLP)
   {
      RLOG_ARG2(L_ERROR,DBG_CELLID,lcCfg->cellId,"Memory allocation FAILED for "
               "Downlink LCID:%d CRNTI:%d", lcCfg->lcId,lcCfg->crnti);
      RETVALUE(RFAILED);
   }
   dlLc->lcId   = lcCfg->lcId;
   rgSCHDbmInsDlDedLcCb(ue, dlLc);
   
   ret = rgSCHUtlRgrLcCfg(cell, ue, dlLc, lcCfg, errInfo);

   if (ret != ROK)
   {
      /* ROLLBACK */
      if (dlLc)
      {
         rgSCHDbmDelDlDedLcCb(ue, dlLc);
         rgSCHCfgFreeDlDedLcCb(cell, ue, dlLc);
      }

      RLOG_ARG3(L_ERROR,DBG_CELLID,cell->cellId,"Dedicated logical channel "
        "configuration failed at SCH:UEID:%d LCID:%d CRNTI:%d",
        ue->ueId, lcCfg->lcId,lcCfg->crnti);
      RETVALUE(RFAILED);
   }
#ifdef LTE_L2_MEAS
   RGSCH_ARRAY_BOUND_CHECK(inst, ue->ul.lcCb, (lcCfg->lcId -1));
   if ( !lcCfg->lcId || 
         (TRUE == ue->ul.lcCb[lcCfg->lcId -1].isValid))
   {
      /* ROLLBACK */
      if (dlLc)
      {
         rgSCHDbmDelDlDedLcCb(ue, dlLc);
         rgSCHCfgFreeDlDedLcCb(cell, ue, dlLc);
      }

      RLOG_ARG3(L_ERROR,DBG_CELLID,cell->cellId,"Dedicated logical channel "
        "configuration failed at SCH: UL LC CB already existing"
        " UEID:%d LCID:%d CRNTI:%d",
        ue->ueId, lcCfg->lcId,lcCfg->crnti);
      RETVALUE(RFAILED);
   }

   /* Create UL LC context to maintain LCG to LC mapping and 
      LC and QCI mapping, this is for L2 Counters :UL ACTIVE UE 
      PER QCI */
   ue->ul.lcCb[lcCfg->lcId -1].isValid = TRUE;
   ulLc = &(ue->ul.lcCb[lcCfg->lcId -1]);

   ulLc->lcId = lcCfg->lcId;
   ulLc->qciCb = &(cell->qciArray[lcCfg->dlInfo.dlQos.qci]);
   ulLc->qciCb->qci = lcCfg->dlInfo.dlQos.qci;
   ue->ul.lcgArr[lcCfg->lcgId].lcArray[ue->ul.lcgArr[lcCfg->lcgId].numLch] = ulLc;
   ulLc->lcg = &ue->ul.lcgArr[lcCfg->lcgId];
   ulLc->lcgArrIdx = ue->ul.lcgArr[lcCfg->lcgId].numLch;
   ue->ul.lcgArr[lcCfg->lcgId].numLch++;

   dlLc->qciCb = &(cell->qciArray[lcCfg->dlInfo.dlQos.qci]);
   dlLc->qciCb->qci = lcCfg->dlInfo.dlQos.qci;
   if(lcCfg->lcType == CM_LTE_LCH_DTCH)
   {
     rgSchAddToL2Meas(cell,dlLc); /*LTE_L2_MEAS_PHASE2*/
   }
#endif /* LTE_L2_MEAS */

   RETVALUE(ROK);
}  /* rgSCHCfgRgrLchCfg */

/***********************************************************
 *
 *     Func : rgSCHCfgRgrLcgCfg
 *
 *
 *     Desc : Handles dedicated logical channel group configuration
 *     recieved from RRM.
 *
 *     Ret  : S16
 *            ROK - Success
 *            RFAILED - Failed
 *
 *     Notes:
 *
 *     File :
 *
 **********************************************************/
#ifdef ANSI
PUBLIC S16 rgSCHCfgRgrLcgCfg
(
RgSchCellCb      *cell,
RgSchUeCb        *ue,
RgrLcgCfg        *lcgCfg,
RgSchErrInfo     *errInfo
)
#else
PUBLIC S16 rgSCHCfgRgrLcgCfg(cell, ue, lcgCfg, errInfo)
RgSchCellCb      *cell;
RgSchUeCb        *ue;
RgrLcgCfg        *lcgCfg;
RgSchErrInfo     *errInfo;
#endif
{
   S16          ret = ROK;

#ifdef RG_UNUSED
//#ifdef LTE_L2_MEAS
   U32          idx;
   RgSchUlLcCb  *ulLc; 
#endif
   TRC2(rgSCHCfgRgrLcgCfg);

   errInfo->errCause = RGSCHERR_CFG_RGR_DED_LCG_CFG;

   ue->ul.lcgArr[lcgCfg->ulInfo.lcgId].lcgId = lcgCfg->ulInfo.lcgId;

   ret = rgSCHUtlRgrLcgCfg(cell, ue, lcgCfg, errInfo);
   if (ret != ROK)
   {
      RLOG_ARG3(L_ERROR,DBG_CELLID,cell->cellId,"Dedicated logical channel "
        "configuration failed at SCH: UEID:%d LCGID:%d CRNTI:%d",
        ue->ueId, lcgCfg->ulInfo.lcgId,lcgCfg->crnti);
      /* Roll back lcgCfg */
      ue->ul.lcgArr[lcgCfg->ulInfo.lcgId].lcgId = RGSCH_INVALID_LCG_ID;
      rgSCHUtlRgrLcgDel(cell, ue, lcgCfg->ulInfo.lcgId);
      RETVALUE(RFAILED);
   }
#ifdef RG_UNUSED
//#ifdef LTE_L2_MEAS
   /* Copy all info of UL LCH in cfg to ulLcgCb */
   for (idx = 0; idx < lcgCfg->ulInfo.numLch; idx++)
   {
      /* Allocate the uplink logical channel control block */
      if((ret = rgSCHUtlAllocSBuf(cell->instIdx, (Data**)&ulLc, 
                  sizeof(RgSchUlLcCb))) != ROK)
      {
         RLOG_ARG0(L_ERROR,DBG_CELLID,cell->cellId,  "Memory allocation FAILED for ");
         RETVALUE(RFAILED);
      }
      if ((U8 *)ulLc == NULLP)
      {
         RLOG_ARG0(L_ERROR,DBG_CELLID,cell->cellId,  "Memory allocation FAILED for ");
         RETVALUE(RFAILED);
      }
      /* Create UL LC context to maintain LCG to LC mapping and 
         LC and QCI mapping, this is for L2 Counters :UL ACTIVE UE 
         PER QCI */
      ulLc->lcId = lcgCfg->ulInfo.lchUlCfg[idx].lcId;
      ulLc->qciCb = &(cell->qciArray[lcgCfg->ulInfo.lchUlCfg[idx].qci]);
      ulLc->qciCb->qci = lcgCfg->ulInfo.lchUlCfg[idx].qci;
      ue->ul.lcgArr[lcgCfg->ulInfo.lcgId].lcArray[idx] = ulLc;
      /* L2_COUNTERS */
      ue->ul.lcCb[ulLc->lcId -1] = ulLc;
      ulLc->lcg = &ue->ul.lcgArr[lcgCfg->ulInfo.lcgId];
      ulLc->lcgArrIdx = idx;
   }
   ue->ul.lcgArr[lcgCfg->ulInfo.lcgId].numLch = lcgCfg->ulInfo.numLch;
#endif /* LTE_L2_MEAS */

   RETVALUE(ROK);
}  /* rgSCHCfgRgrLcgCfg */



/***********************************************************
 *
 *     Func : rgSCHCfgRgrCmnLcCfg
 *
 *
 *     Desc : Handles dedicated logical channel configuration 
 *     recieved from RRC.
 *
 *     Ret  : S16
 *            ROK - Success
 *            RFAILED - Failed
 *
 *     Notes:
 *
 *     File :
 *
 **********************************************************/
#ifdef ANSI
PRIVATE S16 rgSCHCfgRgrCmnLcCfg
(
RgSchCellCb           *cell,
RgrCmnLchCfg          *lcCfg,
RgSchErrInfo          *errInfo
)
#else
PRIVATE S16 rgSCHCfgRgrCmnLcCfg(cell, lcCfg, errInfo)
RgSchCellCb         *cell;
RgrCmnLchCfg        *lcCfg;
RgSchErrInfo        *errInfo;
#endif
{
   RgSchClcDlLcCb cmnLcCb;
   TRC2(rgSCHCfgRgrCmnLcCfg);

   errInfo->errCause = RGSCHERR_CFG_RGR_CMN_LC_CFG;

   cmMemset((U8 *)&cmnLcCb, 0, sizeof(cmnLcCb));

   /* Handle configuration for CCCH/BCCH/PCCH */
   if (lcCfg->lcType == CM_LTE_LCH_CCCH)
   {
      /* UL and DL CCCH configuration */
      if (lcCfg->dir & RGR_DIR_TX)
      {
         cell->dlCcchId = lcCfg->lcId;
      }

      if (lcCfg->dir & RGR_DIR_RX)
      {
         cell->ulCcchId = lcCfg->lcId;
      }
   }
   else
   {
      cmnLcCb.lcId = lcCfg->lcId;
      rgSCHDbmInitCmnLcBoLst(&cmnLcCb);
      if (lcCfg->lcType == CM_LTE_LCH_BCCH)
      {
         /* BCCH on BCH and DLSCH configuration */
         if (lcCfg->dlTrchType == CM_LTE_TRCH_DL_SCH)
         {
            rgSCHDbmInsBcchOnDlsch(cell, &cmnLcCb);
         }
         else
         {
            rgSCHDbmInsBcchOnBch(cell, &cmnLcCb);
         }
      }
      else  /* PCCH configuration */
      {
         rgSCHDbmInsPcch(cell, &cmnLcCb);
      }
   }

   RETVALUE(ROK);
}  /* rgSCHCfgRgrCmnLcCfg */



/***********************************************************
 *
 *     Func : rgSCHCfgFreeDlDedLcCb
 *
 *
 *     Desc :
 *     - Processing Steps:
 *        - Frees downlink dedicated logical channel control block.
 *
 *     Ret  : Void
 *
 *     Notes:
 *
 *     File :
 *
 **********************************************************/
#ifdef ANSI
PRIVATE Void rgSCHCfgFreeDlDedLcCb
(
RgSchCellCb      *cell,
RgSchUeCb        *ue,
RgSchDlLcCb      *dlLc
)
#else
PRIVATE Void rgSCHCfgFreeDlDedLcCb(cell, ue, dlLc)
RgSchCellCb      *cell;
RgSchUeCb        *ue;
RgSchDlLcCb      *dlLc;
#endif
{
   Inst    inst = cell->instIdx;
   TRC2(rgSCHCfgFreeDlDedLcCb);

   rgSCHUtlFreeDlLc(cell, ue, dlLc);

   /* De-allocate the Cb */
   /* ccpu00117052 - MOD - Passing double pointer
      for proper NULLP assignment*/
   rgSCHUtlFreeSBuf(inst, (Data **)&dlLc, sizeof(*dlLc));


  /* Stack Crash problem for TRACE5 changes. Added the return below */
  RETVOID;

}  /* rgSCHCfgFreeDlDedLcCb */


/***********************************************************
 *
 *     Func : rgSCHCfgFreeDlCmnLcCb
 *
 *
 *     Desc :
 *     - Processing Steps:
 *        - Frees downlink common logical channel control block.
 *
 *     Ret  : Void
 *
 *     Notes:
 *
 *     File :
 *
 **********************************************************/
#ifdef ANSI
PRIVATE Void rgSCHCfgFreeDlCmnLcCb
(
RgSchClcDlLcCb   *cmnDlLc
)
#else
PRIVATE Void rgSCHCfgFreeDlCmnLcCb(cmnDlLc)
RgSchClcDlLcCb      *cmnDlLc;
#endif
{
   TRC2(rgSCHCfgFreeDlCmnLcCb);
 
   cmMemset((U8*)cmnDlLc, 0, sizeof(*cmnDlLc));
   cmnDlLc->lcId = RGSCH_INVALID_LC_ID;
   RETVOID;
}  /* rgSCHCfgFreeDlCmnLcCb */


/***********************************************************
 *
 *     Func : rgSCHCfgFreeCellCb
 *
 *
 *     Desc :
 *     - Processing Steps:
 *        - Frees scheduler cell control block.
 *
 *     Ret  : Void
 *
 *     Notes:
 *
 *     File :
 *
 **********************************************************/
#ifdef ANSI
PUBLIC Void rgSCHCfgFreeCellCb
(
RgSchCellCb      *cell
)
#else
PUBLIC Void rgSCHCfgFreeCellCb(cell)
RgSchCellCb      *cell;
#endif
{
   Inst    inst = cell->instIdx;
   CmLList            *node;
   Buffer             *pdu;
   RgSchWarningSiInfo *warningSi;
   RgSchWarningSiPdu  *warningSiPdu;
   U8                  idx;
 
   TRC2(rgSCHCfgFreeCellCb);
   /* ccpu00132385- SI Warning PDUs which are not processed need to be deleted */
   /* Search for used index in WarningSi */
   for(idx = 0; idx < RGR_MAX_NUM_WARNING_SI; idx++)
   {
      if(cell->siCb.warningSi[idx].siId == 0)
        continue;
      cell->siCb.siCtx.siId = cell->siCb.warningSi[idx].siId;
      warningSi = (RgSchWarningSiInfo *) cell->siCb.
                  siArray[cell->siCb.siCtx.siId-1].si; 
      if(warningSi != NULLP)
      {
         /* ccpu00136659: CMAS ETWS design change */
         while (CM_LLIST_FIRST_NODE(&warningSi->warningSiMsg.segLstCp, node))
         {
            warningSiPdu = (RgSchWarningSiPdu *)node->node;
            pdu = warningSiPdu->pdu;
            /* ccpu00136659: CMAS ETWS design change */
            cmLListDelFrm(&warningSi->warningSiMsg.segLstCp, node);
            RGSCH_FREE_MSG(pdu);
         }
         cell->siCb.siArray[cell->siCb.siCtx.siId-1].si = NULLP;
      }
   }
   /* Free lists of the cell */
   rgSCHCfgFreeUeLst(cell);
#ifdef LTEMAC_SPS
   rgSCHCfgFreeSpsUeLst(cell);
#endif /* LTEMAC_SPS */
   rgSCHRamFreeCell(cell);

   rgSCHDbmRntiDbDeInit(cell);
   /* Deallocate the subframe allocation information */
   rgSCHUtlPutSfAlloc(cell);
   rgSCHUtlFreeCell(cell);

   rgSCHCfgFreeRgrCfgLst(cell);
   rgSCHCfgFreeCmnLcLst(cell);

   rgSCHUtlPutRlsHqAlloc(cell);

#ifdef LTE_TDD
   rgSCHDbmDeInitUeTfuPendLst(cell);
#endif /* LTE_TDD */

#ifdef RGR_SI_SCH
   rgSCHUtlPutSiInfo(cell);
#endif/*RGR_SI_SCH*/

   (Void)rgSCHDrxCellDel(cell);

   rgSCHUtlFreeSBuf(inst, (Data**)&(cell->dynCfiCb.cceFailSamples),
               (cell->dynCfiCb.numFailSamples * sizeof(U16)));

#ifdef TENB_STATS
   TSL2DeallocCellStatsBlk(cell->cellId); 
#endif
   /* De-allocate the Cell */
   /* ccpu00117052 - MOD - Passing double pointer
   for proper NULLP assignment*/
   rgSCHUtlFreeSBuf(inst, (Data **)&cell, sizeof(*cell));
 

  /* Stack Crash problem for TRACE5 changes. Added the return below */
  RETVOID;

}  /* rgSCHCfgFreeCellCb */


/***********************************************************
 *
 *     Func : rgSCHCfgFreeUeCb
 *
 *
 *     Desc :
 *     - Processing Steps:
 *        - Frees UE control block.
 *
 *     Ret  : Void
 *
 *     Notes:
 *
 *     File :
 *
 **********************************************************/
#ifdef ANSI
PRIVATE Void rgSCHCfgFreeUeCb
(
RgSchCellCb    *cell,
RgSchUeCb      *ue
)
#else
PRIVATE Void rgSCHCfgFreeUeCb(cell, ue)
RgSchCellCb    *cell;
RgSchUeCb      *ue;
#endif
{
   RgSchDlLcCb       *dlLc;
   Inst    inst = cell->instIdx;
   U8 lcCnt; 
   U8 lcgId; 
   TRC2(rgSCHCfgFreeUeCb);

   /* Free all logical channel info per UE */
   while((dlLc = rgSCHDbmGetNextDlDedLcCb(ue, NULLP)) != NULLP)
   {
      rgSCHDbmDelDlDedLcCb(ue, dlLc);
      rgSCHCfgFreeDlDedLcCb(cell, ue, dlLc);
   }
   for (lcCnt =0; lcCnt<RGSCH_MAX_LC_PER_UE; lcCnt++)
   {
      if(ue->ul.lcCb[lcCnt].isValid == TRUE) 
      {
         lcgId = ue->ul.lcCb[lcCnt].lcg->lcgId;
         if (lcgId <=3)
         {
            rgSCHUtlRgrLcDel(cell, ue, ue->ul.lcCb[lcCnt].lcId,lcgId); 
            ue->ul.lcCb[lcCnt].isValid = FALSE;
         }
      }
   }

   /* Free Scheduler specific information per UE */
   rgSCHUtlFreeUe(cell, ue);
   /* Free Uplink HARQ specific information per UE */
   rgSCHUhmFreeUe(ue);
   if ( ue->drxCb != NULLP)
   {
      /* free drxCb */
   /* ccpu00117052 - MOD - Passing double pointer
   for proper NULLP assignment*/
      rgSCHUtlFreeSBuf(cell->instIdx,
            (Data**)(&(ue->drxCb)),
            sizeof(RgSchDrxUeCb));
   }

   ue->drxCb = (RgSchDrxUeCb *)NULLP;
   /* Free Downlink HARQ specific information per UE */
   rgSCHDhmFreeUe(ue);
   /* Release the RNTI */
   if (ue->rntiLnk)
   {
      rgSCHUtlRlsRnti(cell, ue->rntiLnk, FALSE, 0);
   }
   else
   {
      /* Fix : syed HO UE does not have a valid ue->rntiLnk */
      /* Just indicate to MAC, no need to release at SCH */
      rgSCHUtlIndRntiRls2Mac(cell, ue->ueId, FALSE, 0);
   }
/* rg009.201. Added changes of TFU_UPGRADE */
#ifdef TFU_UPGRADE
    rgSCHCfgPCqiSrsSrUeDel(cell,ue); 
#endif 
#ifdef LTEMAC_HDFDD
    rgSCHHdFddUeDel(cell, ue);
#endif
#ifdef TENB_STATS
    if (ue->tenbStats)
    {
       TSL2DeallocUeStatsBlk(ue->ueId, ue->tenbStats); 
    }
#endif
   /* De-allocate the Ue */
   /* ccpu00117052 - MOD - Passing double pointer
   for proper NULLP assignment*/
   rgSCHUtlFreeSBuf(inst, (Data **)&ue, sizeof(*ue));



  /* Stack Crash problem for TRACE5 changes. Added the return below */
  RETVOID;

}  /* rgSCHCfgFreeUeCb */

/***********************************************************
 *
 *     Func : rgSCHCfgFreeRgrCfgLst
 *
 *
 *     Desc :
 *     - Processing Steps:
 *        - Frees configuration lists in cell control block.
 *
 *     Ret  : Void
 *
 *     Notes:
 *
 *     File :
 *
 **********************************************************/
#ifdef ANSI
PRIVATE Void rgSCHCfgFreeRgrCfgLst
(
RgSchCellCb      *cell
)
#else
PRIVATE Void rgSCHCfgFreeRgrCfgLst(cell)
RgSchCellCb      *cell;
#endif
{
   RgSchCfgElem  *rgCfgElem;
   Inst    inst = cell->instIdx;

   TRC2(rgSCHCfgFreeRgrCfgLst);

   /* Free CURRENT RGR cfg list */
   while ((rgCfgElem = rgSCHDbmGetNextCrntRgrCfgElem(cell, NULLP)) != NULLP)
   {
      rgSCHDbmDelCrntRgrCfgElem(cell, rgCfgElem);
   /* ccpu00117052 - MOD - Passing double pointer
   for proper NULLP assignment*/
      rgSCHUtlFreeSBuf(inst, (Data **)&rgCfgElem, sizeof(*rgCfgElem));
   }

   /* Free PENDING RGR cfg list */
   while ((rgCfgElem = rgSCHDbmGetNextPndngRgrCfgElem(cell, NULLP)) != NULLP)
   {
      rgSCHDbmDelPndngRgrCfgElem(cell, rgCfgElem);
   /* ccpu00117052 - MOD - Passing double pointer
   for proper NULLP assignment*/
      rgSCHUtlFreeSBuf(inst, (Data **)&rgCfgElem, sizeof(*rgCfgElem));
   }


  /* Stack Crash problem for TRACE5 changes. Added the return below */
  RETVOID;

}  /* rgSCHCfgFreeRgrCfgLst */


/***********************************************************
 *
 *     Func : rgSCHCfgFreeCmnLcLst
 *
 *
 *     Desc :
 *     - Processing Steps:
 *        - Frees common logical channels in cell control block.
 *
 *     Ret  : Void
 *
 *     Notes:
 *
 *     File :
 *
 **********************************************************/
#ifdef ANSI
PRIVATE Void rgSCHCfgFreeCmnLcLst
(
RgSchCellCb      *cell
)
#else
PRIVATE Void rgSCHCfgFreeCmnLcLst(cell)
RgSchCellCb      *cell;
#endif
{
   RgSchClcDlLcCb *dlCmnLc;

   TRC2(rgSCHCfgFreeCmnLcLst);

   if ((dlCmnLc = rgSCHDbmGetBcchOnBch(cell)) != NULLP)
   {
      rgSCHCfgFreeDlCmnLcCb(dlCmnLc);
   }
   if ((dlCmnLc = rgSCHDbmGetFirstBcchOnDlsch(cell)) != NULLP)
   {
      rgSCHCfgFreeDlCmnLcCb(dlCmnLc);
   }
   if ((dlCmnLc = rgSCHDbmGetSecondBcchOnDlsch(cell)) != NULLP)
   {
      rgSCHCfgFreeDlCmnLcCb(dlCmnLc);
   }
   if ((dlCmnLc = rgSCHDbmGetPcch(cell)) != NULLP)
   {
      rgSCHCfgFreeDlCmnLcCb(dlCmnLc);
   }


  /* Stack Crash problem for TRACE5 changes. Added the return below */
  RETVOID;

}  /* rgSCHCfgFreeCmnLcLst */


/***********************************************************
 *
 *     Func : rgSCHCfgFreeUeLst
 *
 *
 *     Desc :
 *     - Processing Steps:
 *        - Frees UE list in cell control block.
 *
 *     Ret  : Void
 *
 *     Notes:
 *
 *     File :
 *
 **********************************************************/
#ifdef ANSI
PRIVATE Void rgSCHCfgFreeUeLst
(
RgSchCellCb      *cell
)
#else
PRIVATE Void rgSCHCfgFreeUeLst(cell)
RgSchCellCb      *cell;
#endif
{
   RgSchUeCb     *ue;
   TRC2(rgSCHCfgFreeUeLst);

   /* Free Ues in the list */
   while ((ue = rgSCHDbmGetNextUeCb(cell, NULLP)) != NULLP)
   {
      rgSCHDbmDelUeCb(cell, ue);
      
      /* Call MeasGap and AckNakRep processing module */
      rgSCHMeasGapANRepUeDel(cell, ue, TRUE);

      rgSCHCfgFreeUeCb(cell, ue);
   }

   /* De-initialize the Ue list */
   rgSCHDbmDeInitUeCbLst(cell);


  /* Stack Crash problem for TRACE5 changes. Added the return below */
  RETVOID;

}  /* rgSCHCfgFreeUeLst */

#ifdef LTEMAC_SPS
/***********************************************************
 *
 *     Func : rgSCHCfgFreeSpsUeLst
 *
 *
 *     Desc :
 *     - Processing Steps:
 *        - Frees Sps UE list in cell control block.
 *
 *     Ret  : Void
 *
 *     Notes:
 *
 *     File :
 *
 **********************************************************/
#ifdef ANSI
PRIVATE Void rgSCHCfgFreeSpsUeLst
(
RgSchCellCb      *cell
)
#else
PRIVATE Void rgSCHCfgFreeSpsUeLst(cell)
RgSchCellCb      *cell;
#endif
{
   RgSchUeCb     *ue;

   TRC2(rgSCHCfgFreeSpsUeLst);

   /* Free Ues in the list */
   while ((ue = rgSCHDbmGetNextSpsUeCb(cell, NULLP)))
   {
      rgSCHDbmDelSpsUeCb(cell, ue);
   }

   /* De-initialize the Ue list */
   rgSCHDbmDeInitSpsUeCbLst(cell);

}  /* rgSCHCfgFreeSpsUeLst */

#endif /* LTEMAC_SPS */

#ifdef RGR_SI_SCH
/***********************************************************
 *
 *     Func : rgSCHCfgVldtRgrCellSiCfg
 *
 *     Desc : Validates SI Configuration for SI
 *
 *     Ret  : S16
 *            ROK - Success
 *            RFAILED - Failed
 *
 *     Notes:
 *
 *     File :
 *
 **********************************************************/
#ifdef ANSI
PRIVATE S16 rgSCHCfgVldtRgrCellSiCfg
(
Inst             inst,
RgrSiCfg         *siCfg
)
#else
PRIVATE S16 rgSCHCfgVldtRgrCellSiCfg(inst, siCfg)
Inst             inst;
RgrSiCfg       *siCfg;
#endif
{
   U8   idx; /* idx for iteration */

   UNUSED(inst);

   TRC2(rgSCHCfgVldtRgrCellSiCfg);


#ifndef LTE_TDD
   /* Check that retxCnt value should be <= value of siWinSize.
   This validation is only applicable for FDD mode. */
   if(siCfg->retxCnt > siCfg->siWinSize) 
   {
      RLOG0(L_ERROR,"retxCnt is greater than siWinSize, validation failed");
      RETVALUE(RFAILED);
   }
#endif

   /* Validate that a valid value for numSi has been specified */
   if(siCfg->numSi > RGR_MAX_NUM_SI) 
   {
      RLOG0(L_ERROR,"Validation for numSi in SI CFG failed");
      RETVALUE(RFAILED);
   }

   /* MinPeriodicity will have the least configured periodicity
    * Hence initializing with Max periodicity */
   siCfg->minPeriodicity = RGR_SI_PERD_512;

   /*Validate the value of periodicity specified for SIs */
   for(idx = 0;idx < siCfg->numSi;idx++)
   {
      siCfg->minPeriodicity =     RGSCH_MIN(siCfg->minPeriodicity, 
            siCfg->siPeriodicity[idx]);
         /* Set the siPeriodicity as a multiple of 80 subframes */
         switch(siCfg->siPeriodicity[idx])
         {
            case RGR_SI_PERD_8:
            case RGR_SI_PERD_16:
            case RGR_SI_PERD_32:
            case RGR_SI_PERD_64:
            case RGR_SI_PERD_128:
            case RGR_SI_PERD_256:
            case RGR_SI_PERD_512:
               continue;

         default:
            RLOG0(L_ERROR,"Validation for SI Periodicity in SI-CFG failed");
            RETVALUE(RFAILED);
      }
   }

   RETVALUE(ROK);
}  /* rgSCHCfgVldtRgrCellSiCfg */

/* LTE_ADV_FLAG_REMOVED_START */
/***********************************************************
 *
 *     Func : rgSCHCfgVldtRgrCellLtrAdvCfg
 *
 *     Desc : Validates Lte Adv Configuration
 *
 *     Ret  : S16
 *            ROK - Success
 *            RFAILED - Failed
 *
 *     Notes:
 *
 *     File :
 *
 **********************************************************/
#ifdef ANSI
PRIVATE S16 rgSCHCfgVldtRgrCellLteAdvCfg
(
 Inst                     inst,
 RgrLteAdvancedCellConfig *lteAdvCfg,
 U8                       dlTotalBw  
 )
#else
PRIVATE S16 rgSCHCfgVldtRgrCellLteAdvCfg(inst, lteAdvCfg, dlTotalBw)
   Inst                     inst;
   RgrLteAdvancedCellConfig *lteAdvCfg;
   U8                       dlTotalBw;  
#endif
{
   U8 temp[RGR_ABS_PATTERN_LEN];
   U32 idx;
   UNUSED(inst);

   TRC2(rgSCHCfgVldtRgrCellLteAdvCfg);


   if((lteAdvCfg->pres & RGR_SFR) && (RGR_ENABLE == lteAdvCfg->sfrCfg.status))
   {
      if(lteAdvCfg->sfrCfg.cellEdgeRbRange.startRb > lteAdvCfg->sfrCfg.cellEdgeRbRange.endRb)
      {
         RLOG0(L_ERROR,"Invalid configuration of cell edge bandwidth for SFR feature");
         RETVALUE(RFAILED);
      }

      if(lteAdvCfg->sfrCfg.cellEdgeRbRange.endRb >= dlTotalBw)
      {
         RLOG0(L_ERROR,"Invalid configuration of cell edge end RB for SFR feature");
         RETVALUE(RFAILED);
      }
         
#ifdef TFU_UPGRADE      
      if(lteAdvCfg->sfrCfg.pwrThreshold.pLow >= lteAdvCfg->sfrCfg.pwrThreshold.pHigh)
      {
         RLOG0(L_ERROR,"Invalid configuration of power threshold for SFR feature");
         RETVALUE(RFAILED);
      }
#endif      
   }

   if((lteAdvCfg->pres & RGR_ABS) && (RGR_ENABLE == lteAdvCfg->absCfg.status))
   {
      if((RGR_ABS_MUTE != lteAdvCfg->absCfg.absPatternType) && 
            (RGR_ABS_TRANSMIT != lteAdvCfg->absCfg.absPatternType)) 
      {
         RLOG0(L_ERROR,"Invalid configuration of ABS pattern type");
         RETVALUE(RFAILED);
      }

       cmMemcpy(temp,  (U8 *) lteAdvCfg->absCfg.absPattern,RGR_ABS_PATTERN_LEN);
      
       /* Added validation for ABS pattern len */
      for(idx = 0; idx < RGR_ABS_PATTERN_LEN; idx++)
      {   
         if((temp[idx] != 1) && (temp[idx] != 0))
        {
         RLOG0(L_ERROR,"Invalid configuration of ABS pattern type");
         RETVALUE(RFAILED);
        }
      }
   }

   RETVALUE(ROK);
}
/* LTE_ADV_FLAG_REMOVED_END */
/***********************************************************
 *
 *     Func : rgSCHCfgVldtRgrCellCsgParamCfg
 *
 *     Desc : Validates CSG Parameter Configuration
 *
 *     Ret  : S16
 *            ROK - Success
 *            RFAILED - Failed
 *
 *     Notes:
 *
 *     File :
 *
 **********************************************************/
#ifdef ANSI
PRIVATE S16 rgSCHCfgVldtRgrCellCsgParamCfg
(
Inst                     inst,
RgrCellCsgParamCfg       *csgParam
)
#else
PRIVATE S16 rgSCHCfgVldtRgrCellCsgParamCfg(inst, csgParam)
Inst                     inst;
RgrCellCsgParamCfg       *csgParam;
#endif
{

   TRC2(rgSCHCfgVldtRgrCellCsgParamCfg);

   RGSCHDBGPRM(inst, (rgSchPBuf(inst), "Validating CSG Parameters \n"));

   if(csgParam->minDlResNonCsg > 100)
   {
      RLOG0(L_ERROR,"Invalid Configuration of minimum DL resources "
            "for NON-CSG");
      RETVALUE(RFAILED);
   }
   if(csgParam->minUlResNonCsg > 100)
   {
      RLOG0(L_ERROR,"Invalid Configuration of minimum UL resources "
            "for NON-CSG");
      RETVALUE(RFAILED);
   }
   RETVALUE(ROK);
}

/**
 * @brief Validates the SI configuration request from RRM to MAC.
 *
 * @details
 *
 *     Function : rgSCHCfgVldtRgrSiCfg
 *
 *     Processing Steps:
 *        - Validate the range of configured values recieved in
 *          configuration request.
 *        - If validated successfully,
 *          - Return ROK
 *        - Else 
 *          - Return RFAILED.
 *      - Else return RFAILED.
 *  @param[in]  Inst         inst
 *  @param[in]  RgrCellCfg   *siCfg
 *  @param[out] RgSchCellCb  *cell
 *  @param[out] RgSchErrInfo *errInfo
 *  @return  S16
 *      -# ROK
 *      -# RFAILED
 **/
#ifdef ANSI
PUBLIC S16 rgSCHCfgVldtRgrSiCfg
(
Inst         inst,
RgrSiCfgReqInfo *siCfg,
RgSchCellCb      *cell,
RgSchErrInfo *errInfo
)
#else
PUBLIC S16 rgSCHCfgVldtRgrSiCfg(inst, siCfg, cell, errInfo)
Inst         inst;
RgrSiCfgReqInfo *siCfg;
RgSchCellCb      *cell;
RgSchErrInfo *errInfo;
#endif
{
   MsgLen  msgLen = 0;
   U8      numSi;

   TRC2(rgSCHCfgVldtRgrSiCfg);


   errInfo->errCause = RGSCHERR_CFG_INVALID_RGR_SI_CFG;

   /*Validate the cfgType parameter */
   switch(siCfg->cfgType)
   {
      /*ccpu00140789*/
      case RGR_SI_STOP:
         numSi = (cell->siCb.siBitMask & RGSCH_SI_SICFG_UPD) ?
              cell->siCb.newSiCfg.numSi : cell->siCfg.numSi;
         if((siCfg->siId < RGSCH_SI_SIID_LOWER_LMT) ||
                 (siCfg->siId > numSi))
         {
            RLOG_ARG0(L_ERROR,DBG_CELLID,cell->cellId,  "Invalid SI Id value"
                     " specified");
            RETVALUE(RFAILED);
         }
         errInfo->errCause = RGSCHERR_NONE;
         RETVALUE(ROK);
      break;
      case RGR_SI_CFG_TYPE_MIB:      /* SI CFG Type MIB */
      case RGR_SI_CFG_TYPE_SIB1:     /* SI CFG TYPE SIB1 */
      case RGR_SI_CFG_TYPE_SIB1_PWS: /* SI CFG TYPE SIB1_PWS */
      case RGR_SI_CFG_TYPE_SIB8_CDMA:    /* SI CFG TYPE SIB 8 CDMA */
         break;

      case RGR_SI_CFG_TYPE_SI:    /* SI CFG TYPE SI */
         /*Check that value of SI should be less than equal 
           to configured numSi parameter value */
         /* Added siId validation for lower limit */
         numSi = (cell->siCb.siBitMask & RGSCH_SI_SICFG_UPD) ? 
            cell->siCb.newSiCfg.numSi : cell->siCfg.numSi;
         if((siCfg->siId < RGSCH_SI_SIID_LOWER_LMT) || 
               (siCfg->siId > numSi))
         {
            RLOG_ARG0(L_ERROR,DBG_CELLID,cell->cellId,  "Invalid SI Id value"
                     " specified");
            RETVALUE(RFAILED);
         }

         if(siCfg->siId > ((cell->siCfg.minPeriodicity * 10)/cell->siCfg.siWinSize))
         {
            RLOG_ARG0(L_ERROR,DBG_CELLID,cell->cellId, "SiId can not be scheduled ");
            RETVALUE(RFAILED);
         }
         break;

      default:
         RLOG_ARG0(L_ERROR,DBG_CELLID,cell->cellId,  "Invalid cfgType "
                      "parameter value");
         RETVALUE(RFAILED);
   }

   /*Validate the specified pdu */
   if(NULLP == siCfg->pdu)
   {
      RLOG_ARG0(L_ERROR,DBG_CELLID,cell->cellId,  "Invalid NULLP pdu "
                   "specified");
      RETVALUE(RFAILED);
   }

   /*Check if PDU is of 0 length*/
   SFndLenMsg(siCfg->pdu, &msgLen);
   if(0 == msgLen)
   {
      RLOG_ARG0(L_ERROR,DBG_CELLID,cell->cellId,  "Invalid pdu "
                   "specified");
      RETVALUE(RFAILED);
   }

   errInfo->errCause = RGSCHERR_NONE;


   RETVALUE(ROK);
}  /* rgSCHCfgVldtRgrSiCfg */
#endif /*RGR_SI_SCH*/

/* LTE_ADV_FLAG_REMOVED_START */
/**
 * @brief Validates the RNTP INF request from RRM to MAC.
 *
 * @details
 *
 *     Function : rgSCHCfgVldtRgrLoadInf
 *
 *     Processing Steps:
 *        - Validate the range of configured values recieved in
 *          LOAD INF request.
 *        - If validated successfully,
 *          - Return ROK
 *        - Else
 *          - Return RFAILED.
 *      - Else return RFAILED.
 *  @param[in]  Inst                inst
 *  @param[in]  RgrLoadInfReqInfo   *loadInfReq
 *  @param[out] RgSchCellCb         *cell
 *  @param[out] RgSchErrInfo        *errInfo
 *  @return  S16
 *      -# ROK
 *      -# RFAILED
 **/
#ifdef ANSI
PUBLIC S16 rgSCHCfgVldtRgrLoadInf
(
 Inst              inst,
 RgrLoadInfReqInfo *loadInfReq,
 RgSchCellCb       *cell,
 RgSchErrInfo      *errInfo
 )
#else
PUBLIC S16 rgSCHCfgVldtRgrLoadInf(inst, loadInfReq, cell, errInfo)
   Inst              inst;
   RgrLoadInfReqInfo *loadInfReq;
   RgSchCellCb       *cell;
   RgSchErrInfo      *errInfo;
#endif
{

   TRC2(rgSCHCfgVldtRgrLoadInf);


   errInfo->errCause = RGSCHERR_CFG_INVALID_RGR_LOAD_INF;

   /* Validate if the CC startRb which we have received from DSFR lies in CE sub-band of the receiving eNB */
   if((loadInfReq->rgrCcPHighStartRb >= cell->lteAdvCb.sfrCfg.cellEdgeRbRange.startRb) && 
         (loadInfReq->rgrCcPHighStartRb <= cell->lteAdvCb.sfrCfg.cellEdgeRbRange.endRb))
   {
      RLOG_ARG0(L_ERROR,DBG_CELLID,cell->cellId,"Invalid rgrCcPHighStartRb received specified");
      RETVALUE(RFAILED);
   }

   /* Validate if the CC endRb which we have received from DSFR lies in CE sub-band of the receiving eNB */
   if((loadInfReq->rgrCcPHighEndRb >= cell->lteAdvCb.sfrCfg.cellEdgeRbRange.startRb) && 
         (loadInfReq->rgrCcPHighEndRb <= cell->lteAdvCb.sfrCfg.cellEdgeRbRange.endRb))
   {
      RLOG_ARG0(L_ERROR,DBG_CELLID,cell->cellId,  "Invalid rgrCcPHighEndRb received specified");
      RETVALUE(RFAILED);
   }

   errInfo->errCause = RGSCHERR_NONE;


   RETVALUE(ROK);
}  /* rgSCHCfgVldtRgrLoadInf */
/* LTE_ADV_FLAG_REMOVED_END */

#ifdef TFU_UPGRADE

/********************************************************************
 *   UE ACQI, PCQI, RI, SRS and SR Re/Configuration  Validation Functions  *
 *                                                                   *
 *********************************************************************/

 /*
 * @brief Validates the Tx Mode and PUSCH Mode configuration.
 *
 * @details
 *
 *     Function : rgSCHCfgVldtRgrTxmodePuschMode  
 *
 *     Processing Steps:
 *      - Validate whether the configured PUSCH Mode and the
 *         Configured Tx Mode are in the right combination
 *      - If validated successfully,
 *        - Return ROK.
 *      - Else 
 *        - Return RFAILED.
 *
 *  @param[in]  RgSchCellCb     *cellCb
 *  @param[in]  RgrTxMode     txMde
 *  @param[in]  RgrAprdCqiMode puschMode
 *  @param[out] RgSchErrInfo *errInfo
 *  @return  S16
 *      -# ROK
 *      -# RFAILED
 */
#ifdef ANSI
PUBLIC S16 rgSCHCfgVldtRgrTxmodePuschMode 
(
 RgSchCellCb     *cellCb,
 RgrTxMode       txMde,
 RgrAprdCqiMode  puschMode,
 RgSchErrInfo    *errInfo
 )
#else
PUBLIC S16 rgSCHCfgVldtRgrTxmodePuschMode(cellCb, txMde, puschMode,errInfo)
   RgSchCellCb     *cellCb;
   RgrTxMode       txMde;
   RgrAprdCqiMode  puschMode;
   RgSchErrInfo    *errInfo;
#endif
{
   TRC2(rgSCHCfgVldtRgrTxmodePuschMode);


   if (txMde == RGR_UE_TM_1 ||  txMde == RGR_UE_TM_2 ||
         txMde == RGR_UE_TM_3 || txMde == RGR_UE_TM_7)
   {
      if (puschMode == RGR_APRD_CQI_MOD12 ||
            puschMode == RGR_APRD_CQI_MOD22  ||
            puschMode == RGR_APRD_CQI_MOD31)
      {
         RETVALUE(RFAILED);
      }
   }

   if (txMde == RGR_UE_TM_4 ||  txMde == RGR_UE_TM_6)
   {
      if (puschMode == RGR_APRD_CQI_MOD20 ||
            puschMode == RGR_APRD_CQI_MOD30)
      {
         RETVALUE(RFAILED);
      }
   }

   if (txMde == RGR_UE_TM_5 )
   {
      if (puschMode != RGR_APRD_CQI_MOD31)
      {
         RETVALUE(RFAILED);
      }
   }
   RETVALUE(ROK);
}/*rgSCHCfgVldtRgrTxmodePuschMode ends*/

 /*
 * @brief Validates the UE ACQI configuration request from RRM to MAC.
 *
 * @details
 *
 *     Function : rgSCHCfgVldtRgrUeACqiCfg  
 *
 *     Processing Steps:
 *      - Validate the UE configuration request from RRC to MAC at CFG:
 *        validate the ACQI Configuration 
 *      - If validated successfully,
 *        - Return ROK.
 *      - Else 
 *        - Return RFAILED.
 *
 *  @param[in] RgSchCellCb        *cellCb,
 *  @param[in] CmLteRnti          crnti,  
 *  @param[in] RgrUeAprdDlCqiCfg   *acqiCfg,  
 *  @param[in] RgrUeTxModeCfg     txMode,
 *  @param[out] RgSchErrInfo       *errInfo

 *  @return  S16
 *      -# ROK
 *      -# RFAILED
 */
#ifdef ANSI
PUBLIC S16 rgSCHCfgVldtRgrUeACqiCfg 
(
 RgSchCellCb        *cellCb,
 CmLteRnti          crnti,  
 RgrUeAprdDlCqiCfg  *acqiCfg,  
 RgrUeTxModeCfg     txMode,
 RgSchErrInfo       *errInfo
 )
#else
PUBLIC S16 rgSCHCfgVldtRgrUeACqiCfg(cellCb, crnti, acqiCfg,txMode, errInfo)
   RgSchCellCb        *cellCb;
   CmLteRnti          crnti; 
   RgrUeAprdDlCqiCfg  *acqiCfg;
   RgrUeTxModeCfg     txMode;
   RgSchErrInfo       *errInfo;
#endif
{
   RgrTxMode      txMde;
   RgrAprdCqiMode puschMode; 
   TRC2(rgSCHCfgVldtRgrUeACqiCfg);


   if(acqiCfg->pres)
   {
      if(txMode.pres == TRUE)
      {
         txMde = txMode.txModeEnum;
         puschMode = acqiCfg->aprdModeEnum;
         if ( ROK != rgSCHCfgVldtRgrTxmodePuschMode(cellCb, txMde,
                  puschMode, errInfo))
         {
            RLOG_ARG1(L_ERROR,DBG_CELLID,cellCb->cellId,
                     "Invalid Aperiodic CQI configuration CRNTI:%d",crnti);
            RETVALUE(RFAILED);
         } 
      }
   }
   RETVALUE(ROK);
}

 /*
 * @brief Validates the Tx Mode and PUCCH Mode configuration.
 *
 * @details
 *
 *     Function : rgSCHCfgVldtRgrTxmodePucchMode  
 *
 *     Processing Steps:
 *      - Validate whether the configured PUCCH Mode and the
 *         Configured Tx Mode are in the right combination
 *      - If validated successfully,
 *        - Return ROK.
 *      - Else 
 *        - Return RFAILED.
 *
 *  @param[in]  RgSchCellCb     *cellCb
 *  @param[in]  RgrTxMode     txMde
 *  @param[in]  RgrPrdCqiMode     pucchMode
 *  @param[out] RgSchErrInfo *errInfo
 *  @return  S16
 *      -# ROK
 *      -# RFAILED
 */
#ifdef ANSI
PUBLIC S16 rgSCHCfgVldtRgrTxmodePucchMode 
(
   RgSchCellCb     *cellCb,
   RgrTxMode       txMde,
   RgrPrdCqiMode   pucchMode,
   RgSchErrInfo    *errInfo
 )
#else
PUBLIC S16 rgSCHCfgVldtRgrTxmodePucchMode(cellCb, txMde, pucchMode,errInfo)
   RgSchCellCb     *cellCb;
   RgrTxMode       txMde;
   RgrPrdCqiMode   pucchMode;
   RgSchErrInfo    *errInfo;
#endif
{
   TRC2(rgSCHCfgVldtRgrTxmodePucchMode);
   

  if (pucchMode == RGR_PRD_CQI_MOD10 || pucchMode == RGR_PRD_CQI_MOD20 )
  {
   if (txMde ==RGR_UE_TM_4 || txMde ==RGR_UE_TM_5 || txMde ==RGR_UE_TM_6)
   {
      RETVALUE(RFAILED);
   }
  }
  else if (pucchMode == RGR_PRD_CQI_MOD11 || pucchMode == RGR_PRD_CQI_MOD21)
  {
     if (txMde ==RGR_UE_TM_1 || txMde ==RGR_UE_TM_2 || txMde ==RGR_UE_TM_3 \
             || txMde ==RGR_UE_TM_7)
   {
      RETVALUE(RFAILED);
   }
  }
   RETVALUE(ROK);
}

/*
 * @brief Validates the UE Periodic CQI, PMI, RI, re/configuration request from RRM to MAC.
 *
 * @details
 *
 *     Function : rgSCHCfgVldtRgrUePCqiCfg  
 *
 *     Processing Steps:
 *      - Validate the UE configuration request from RRC to MAC at CFG:
 *        validate the value range for Periodic CQI, PMI, RI values.
 *      - If validated successfully,
 *        - Return ROK.
 *      - Else 
 *        - Return RFAILED.
 *
 *  @param[in] RgSchCellCb        *cellCb,
 *  @param[in] CmLteRnti          crnti,  
 *  @param[in] RgrUePrdDlCqiCfg   *cqiCfg,  
 *  @param[in] RgrUeTxModeCfg     txMode,
 *  @param[out] RgSchErrInfo       *errInfo
 *  @return  S16
 *      -# ROK
 *      -# RFAILED
 */
#ifdef LTEMAC_HDFDD
#ifdef ANSI
PUBLIC S16 rgSCHCfgVldtRgrUePCqiCfg 
(
 RgSchCellCb        *cellCb,
 CmLteRnti          crnti,  
 RgrUePrdDlCqiCfg   *cqiCfg,
 Bool               hdFdd,
 RgrUeTxModeCfg     txMode,
 RgSchErrInfo       *errInfo
 )
#else
PUBLIC S16 rgSCHCfgVldtRgrUePCqiCfg(cellCb, crnti, cqiCfg, hdFdd, 
       txMode, errInfo)
   RgSchCellCb        *cellCb;
   CmLteRnti          crnti; 
   RgrUePrdDlCqiCfg   *cqiCfg;
   Bool               hdFdd;
   RgrUeTxModeCfg     txMode;
   RgSchErrInfo       *errInfo;
#endif
#else
#ifdef ANSI
PUBLIC S16 rgSCHCfgVldtRgrUePCqiCfg 
   (
    RgSchCellCb        *cellCb,
    CmLteRnti          crnti,  
    RgrUePrdDlCqiCfg   *cqiCfg,  
    RgrUeTxModeCfg     txMode,
    RgSchErrInfo       *errInfo
   )
#else
PUBLIC S16 rgSCHCfgVldtRgrUePCqiCfg(cellCb, crnti, cqiCfg,  txMode, errInfo)
   RgSchCellCb        *cellCb;
   CmLteRnti          crnti; 
   RgrUePrdDlCqiCfg   *cqiCfg;
   RgrUeTxModeCfg     txMode;
   RgSchErrInfo       *errInfo;
#endif
#endif
{
   RgrTxMode     txMde; 
   RgrPrdCqiMode pucchMode; 

   TRC2(rgSCHCfgVldtRgrUePCqiCfg);
   
   txMde = RGR_UE_TM_1;
   pucchMode = RGR_PRD_CQI_MOD20;
   if ( RGR_SCH_PCQI_SETUP == cqiCfg->type )
   {
      /*1. Validate for Tx Mode and PUCCH Mode combination*/
      if(txMode.pres == TRUE)
      {
         txMde = txMode.txModeEnum;
         pucchMode = cqiCfg->cqiSetup.prdModeEnum;
         if ( ROK != rgSCHCfgVldtRgrTxmodePucchMode(cellCb, txMde,
                  pucchMode, errInfo))
         {
            RLOG_ARG1(L_ERROR,DBG_CELLID,cellCb->cellId,
                     "Invalid Tx Mode-PUCCH Mode combination CRNTI:%d",crnti);
            RETVALUE(RFAILED);
         } 
      }

      /*2. Validate for PCQI Reporting Type and PUCCH Mode combination*/
      if((cqiCfg->cqiSetup.cqiRepType==1) && 
              ((pucchMode == RGR_PRD_CQI_MOD20) ||
               (pucchMode == RGR_PRD_CQI_MOD21)))
      {
            RLOG_ARG1(L_ERROR,DBG_CELLID,cellCb->cellId,
                     "Invalid Reporting Type-PUCCH Mode combination CRNTI:%d",crnti);
            RETVALUE(RFAILED);
      }

      if((cqiCfg->cqiSetup.cqiRepType==2) && 
              ((pucchMode == RGR_PRD_CQI_MOD10) ||
               (pucchMode == RGR_PRD_CQI_MOD11)))
      {
            RLOG_ARG1(L_ERROR,DBG_CELLID,cellCb->cellId,
                     "Invalid Reporting Type-PUCCH Mode combination CRNTI:%d",crnti);
            RETVALUE(RFAILED);
      }

      /*3. Validate CQI/PMI and RI Configuration related parameter values */
      /*TODO- To be compared with configured n2Pucch Index*/
      if (cqiCfg->cqiSetup.cqiPResIdx > RG_SCH_PUCCH_RES_MAX_SUPP ) 
      {
         RLOG_ARG3(L_ERROR,DBG_CELLID,cellCb->cellId,  
                  "Not Supported or Invalid CQI-PUCCH resourceIndex=%d Cfg Val=%d CRNTI:%d", 
                  RG_SCH_PUCCH_RES_MAX_SUPP, cqiCfg->type,crnti);
         RETVALUE(RFAILED);
      } 

#ifdef LTEMAC_HDFDD
      if(hdFdd)
      {
         if((cqiCfg->cqiSetup.cqiPCfgIdx > RG_SCH_ICQI_MAX_SUPP)||
               (cqiCfg->cqiSetup.cqiPCfgIdx < 7) ||
               (cqiCfg->cqiSetup.cqiPCfgIdx == 317))
         {
            RLOG_ARG4(L_ERROR,DBG_CELLID,cellCb->cellId,  
                     "Not Supported or Invalid Cfg CQI Min Index Sup =%d"
                     "Max Index Sup=%d  Cfg Val=%d CRNTI:%d", RG_SCH_ICQI_MIN_SUPP, 
                     RG_SCH_ICQI_MAX_SUPP, cqiCfg->type,crnti);
            RETVALUE(RFAILED);
         }
      }
      else 
      {
         if((cqiCfg->cqiSetup.cqiPCfgIdx > RG_SCH_ICQI_MAX_SUPP)||
               (cqiCfg->cqiSetup.cqiPCfgIdx == 317))
         {
            RLOG_ARG4(L_ERROR,DBG_CELLID,cellCb->cellId,  
                     "Not Supported or Invalid Cfg CQI Min Index Sup =%d "
                     "Max Index Sup=%d  Cfg Val=%d CRNTI:%d", RG_SCH_ICQI_MIN_SUPP, 
                     RG_SCH_ICQI_MAX_SUPP, cqiCfg->type,crnti);
            RETVALUE(RFAILED);
         }
      }
#else
      if((cqiCfg->cqiSetup.cqiPCfgIdx > RG_SCH_ICQI_MAX_SUPP)||
            (cqiCfg->cqiSetup.cqiPCfgIdx == 317))
      {
         RLOG_ARG4(L_ERROR,DBG_CELLID,cellCb->cellId,  
                  "Not Supported or Invalid Cfg CQI Min Index Sup =%d"
                  "Max Index Sup=%d  Cfg Val=%d CRNTI;%d", RG_SCH_ICQI_MIN_SUPP, 
                  RG_SCH_ICQI_MAX_SUPP, cqiCfg->type,crnti);
         RETVALUE(RFAILED);
      }
#endif

      if((cqiCfg->cqiSetup.cqiRepType < RGR_UE_PCQI_WB_REP) ||
            (cqiCfg->cqiSetup.cqiRepType > RGR_UE_PCQI_SB_REP))
      {
         RLOG_ARG2(L_ERROR,DBG_CELLID,cellCb->cellId,  
                  "Invalid Cfg CQI Report"
                  "ModeCfg Val=%d CRNTI:%d", cqiCfg->type,crnti);
         RETVALUE(RFAILED);
      }

      if((cqiCfg->cqiSetup.cqiRepType == RGR_UE_PCQI_SB_REP) && 
            ((cqiCfg->cqiSetup.k < RG_SCH_CQI_K_MIN) ||
            (cqiCfg->cqiSetup.k > RG_SCH_CQI_K_MAX)))
      {
         RLOG_ARG2(L_ERROR,DBG_CELLID,cellCb->cellId,  
                  "Invalid CQI Cfg K Cfg Val=%d CRNTI:%d", cqiCfg->type,crnti);
         RETVALUE(RFAILED);
      }

      if((cqiCfg->cqiSetup.cqiRepType == RGR_UE_PCQI_SB_REP) && 
            (cellCb->bwCfg.dlTotalBw <= 7))
      {
         RLOG_ARG2(L_ERROR,DBG_CELLID,cellCb->cellId,  
                  "Invalid Periodic CQI mode Cfg for dlTotalBw (%d) for CRNTI:%d", 
                  cellCb->bwCfg.dlTotalBw, crnti);
         RETVALUE(RFAILED);
      }

#ifndef LTE_TDD
      if (cqiCfg->cqiSetup.cqiPCfgIdx == RG_SCH_ICQI_RESV_FDD ) 
      {  
         RLOG_ARG2(L_ERROR,DBG_CELLID,cellCb->cellId, 
                  "Reserved value Cfg =%d CRNTI:%d", 
                  cqiCfg->cqiSetup.cqiPResIdx,crnti);
         RETVALUE(RFAILED);
      }
#endif

      /* 4. Check RI Configuration values */
       if(cqiCfg->cqiSetup.riEna == TRUE)
      {
         if(txMode.pres == TRUE)
         {
            if(((txMde != RGR_UE_TM_3)  && (txMde != RGR_UE_TM_4)))
              {
                  RLOG_ARG2(L_ERROR,DBG_CELLID,cellCb->cellId, 
                  "Invalid Transmission Mode =%d CRNTI:%d",
                  txMde,crnti);
                  RETVALUE(RFAILED);
              }
         }
         
         if(cqiCfg->cqiSetup.riCfgIdx > RG_SCH_IRI_MAX_SUPP)
         {
          RLOG_ARG2(L_ERROR,DBG_CELLID,cellCb->cellId, 
                  "Invalid Index RI value Cfg =%d CRNTI:%d",
                  cqiCfg->cqiSetup.riCfgIdx,crnti);
          RETVALUE(RFAILED);
         }
      }
   }

  
   RETVALUE(ROK);
}

/*
 * @brief Validates the UE SRS Re/Configuation request from RRM to MAC.
 *
 * @details
 *
 *     Function : rgSCHCfgVldtRgrUeUlSrsCfg 
 *
 *     Processing Steps:
 *      - Validate the UE configuration request from RRC to MAC at CFG:
 *        validate the value range for SRS values.
 *      - If validated successfully,
 *        - Return ROK.
 *      - Else 
 *        - Return RFAILED.
 *
 *  @param[in] RgSchCellCb       *cellCb,
 *  @param[in] CmLteRnti         crnti,  
 *  @param[in] RgrUeUlSrsCfg     *srsCfg,  
 *  @param[out] RgSchErrInfo     *errInfo
 *  @return  S16
 *      -# ROK
 *      -# RFAILED
 */
#ifdef LTEMAC_HDFDD
#ifdef ANSI
PUBLIC S16 rgSCHCfgVldtRgrUeUlSrsCfg 
(
 RgSchCellCb       *cellCb,
 CmLteRnti         crnti,  
 RgrUeUlSrsCfg     *srsCfg,
 Bool              hdFdd,
 RgSchErrInfo      *errInfo
 )
#else
PUBLIC S16 rgSCHCfgVldtRgrUeUlSrsCfg(cellCb, crnti, srsCfg, hdFdd, errInfo)
   RgSchCellCb     *cellCb;
   CmLteRnti       crnti; 
   RgrUeUlSrsCfg   *srsCfg;
   Bool            hdFdd;
   RgSchErrInfo    *errInfo;
#endif
#else
#ifdef ANSI
PUBLIC S16 rgSCHCfgVldtRgrUeUlSrsCfg 
(
 RgSchCellCb       *cellCb,
 CmLteRnti         crnti,  
 RgrUeUlSrsCfg     *srsCfg,  
 RgSchErrInfo      *errInfo
 )
#else
PUBLIC S16 rgSCHCfgVldtRgrUeUlSrsCfg(cellCb, crnti, srsCfg,  errInfo)
   RgSchCellCb     *cellCb;
   CmLteRnti       crnti; 
   RgrUeUlSrsCfg   *srsCfg;
   RgSchErrInfo    *errInfo;
#endif
#endif
{
   U16    srsPeri=0;
   U16    srsOffset=0;
   U8     srsSubframe=0;
   
   TRC2(rgSCHCfgVldtRgrUeUlSrsCfg);


   if ( RGR_SCH_SRS_SETUP == srsCfg->type )
   {

      /*ccpu00130768 - ADD - if cell specific SRS is not configured*/
      if(cellCb->srsCfg.isSrsCfgPres == FALSE)
      {
         RLOG_ARG1(L_ERROR,DBG_CELLID,cellCb->cellId, 
              "Cell specific SRS is not configured CRNTI:%d",crnti);
         RETVALUE(RFAILED);
      }

      /*  1. Validate SRS Configuration related parameter values  */
      /* 1.1 iSRS should be 0-636; Ref: 36.213. Table 8.2-1 */
#ifdef LTEMAC_HDFDD
      if(hdFdd)
      {
         if ( (srsCfg->srsSetup.srsCfgIdx < 7) || 
               (srsCfg->srsSetup.srsCfgIdx > RG_SCH_ISRS_MAX_SUPP) )
         {
            RLOG_ARG4(L_ERROR,DBG_CELLID,cellCb->cellId, 
                     "Not Supported or Invalid Cfg "
                     "SRS Min Index Sup =%d   Max Index Sup=%d  Cfg Val=%d CRNTI:%d",
                     RG_SCH_ISRS_MIN_SUPP, RG_SCH_ISRS_MAX_SUPP,
                     srsCfg->srsSetup.srsCfgIdx,crnti);
            RETVALUE(RFAILED);
         }
      }
      else
      {
         if ( srsCfg->srsSetup.srsCfgIdx > RG_SCH_ISRS_MAX_SUPP )
         {
            RLOG_ARG4(L_ERROR,DBG_CELLID,cellCb->cellId, 
                     "Not Supported or Invalid Cfg"
                     "SRS Min Index Sup =%d   Max Index Sup=%d  Cfg Val=%d CRNTI:%d",
                     RG_SCH_ISRS_MIN_SUPP, RG_SCH_ISRS_MAX_SUPP,
                     srsCfg->srsSetup.srsCfgIdx,crnti);
            RETVALUE(RFAILED);
         }

      }
#else
      if ( srsCfg->srsSetup.srsCfgIdx > RG_SCH_ISRS_MAX_SUPP )
      {
         RLOG_ARG4(L_ERROR,DBG_CELLID,cellCb->cellId, 
                  "Not Supported or Invalid Cfg" 
                  "SRS Min Index Sup =%d   Max Index Sup=%d  Cfg Val=%d CRNTI:%d",
                  RG_SCH_ISRS_MIN_SUPP, RG_SCH_ISRS_MAX_SUPP,
                  srsCfg->srsSetup.srsCfgIdx,crnti);
         RETVALUE(RFAILED);
      }
#endif
#ifdef LTE_TDD
      /* Compute SRS Offset and Periodicity */
      rgSCHUtlGetCfgPerOff( RG_SCH_TDD_SRS_TBL, 
            srsCfg->srsSetup.srsCfgIdx,
            &srsPeri, &srsOffset);  
#else
      rgSCHUtlGetCfgPerOff( RG_SCH_FDD_SRS_TBL, 
            srsCfg->srsSetup.srsCfgIdx,
            &srsPeri, &srsOffset);     
#endif
       srsSubframe = srsOffset%RGSCH_NUM_SUB_FRAMES;
#ifdef LTE_TDD   
      if(rgSchTddCellSpSrsSubfrmTbl[cellCb->srsCfg.srsSubFrameCfg][srsSubframe] == FALSE)   {      
#else   
      if(rgSchFddCellSpSrsSubfrmTbl[cellCb->srsCfg.srsSubFrameCfg][srsSubframe] == FALSE)   {
#endif
         RLOG_ARG2(L_ERROR,DBG_CELLID,cellCb->cellId, 
               "UE specific SRS is not occuring in Cell specific SRS subframe"
               "srs Cfg Idx =%d CRNTI:%d",
               srsCfg->srsSetup.srsCfgIdx,crnti);
         RETVALUE(RFAILED);
      }


      if ( srsCfg->srsSetup.fDomPosi > RG_SCH_SRS_FREQDOM_POS_MAX )
      {
         RLOG_ARG4(L_ERROR,DBG_CELLID,cellCb->cellId, 
                  "Not Supported or Invalid Cfg" 
                  "SRS Min Freq Domain Position =%d"   
                  "Max Freq Domain Position =%d  Cfg Val=%d CRNTI:%d",
                  RG_SCH_SRS_FREQDOM_POS_MIN, RG_SCH_SRS_FREQDOM_POS_MAX,
                  srsCfg->srsSetup.srsCfgIdx,crnti);
         RETVALUE(RFAILED);
      }

      if ( srsCfg->srsSetup.txComb > RG_SCH_SRS_TXCOMB_MAX )
      {
         RLOG_ARG4(L_ERROR,DBG_CELLID,cellCb->cellId, 
                  "Not Supported or Invalid Cfg" 
                  "SRS Min TX Comb =%d   Max TX Comb =%d  Cfg Val=%d CRNTI:%d",
                  RG_SCH_SRS_TXCOMB_MIN, RG_SCH_SRS_TXCOMB_MAX,
                  srsCfg->srsSetup.srsCfgIdx,crnti);
         RETVALUE(RFAILED);
      }
   }
   RETVALUE(ROK);
}

/*
 * @brief Validates the UE SR Re/configuration request from RRM to MAC.
 *
 * @details
 *
 *     Function : rgSCHCfgVldtRgrUeSrCfg 
 *
 *     Processing Steps:
 *      - Validate the UE re/configuration request from RRC to MAC at CFG:
 *        validate the value range for SR values.
 *      - If validated successfully,
 *        - Return ROK.
 *      - Else 
 *        - Return RFAILED.
 *
 *  @param[in] RgSchCellCb      *cellCb,
 *  @param[in] CmLteRnti        crnti,  
 *  @param[in] RgrUeSrCfg       *srCfg,  
 *  @param[out] RgSchErrInfo    *errInfo
 *  @return  S16
 *      -# ROK
 *      -# RFAILED
 */
#ifdef LTEMAC_HDFDD 
#ifdef ANSI
PUBLIC S16 rgSCHCfgVldtRgrUeSrCfg 
(
 RgSchCellCb      *cellCb,
 CmLteRnti        crnti,  
 RgrUeSrCfg       *srCfg,
 Bool             hdFdd,
 RgSchErrInfo     *errInfo
 )
#else
PUBLIC S16 rgSCHCfgVldtRgrUeSrCfg(cellCb, crnti, srCfg, hdFdd, errInfo)
   RgSchCellCb    *cellCb;
   CmLteRnti      crnti; 
   RgrUeSrCfg     *srCfg;
   Bool           hdFdd;
   RgSchErrInfo   *errInfo;
#endif
#else
#ifdef ANSI
PUBLIC S16 rgSCHCfgVldtRgrUeSrCfg 
(
 RgSchCellCb      *cellCb,
 CmLteRnti        crnti,  
 RgrUeSrCfg       *srCfg,  
 RgSchErrInfo     *errInfo
 )
#else
PUBLIC S16 rgSCHCfgVldtRgrUeSrCfg(cellCb, crnti, srCfg, errInfo)
   RgSchCellCb    *cellCb;
   CmLteRnti      crnti; 
   RgrUeSrCfg     *srCfg;
   RgSchErrInfo   *errInfo;
#endif
#endif
{

   TRC2(rgSCHCfgVldtRgrUeSrCfg);



   if ( RGR_SCH_SR_SETUP == srCfg->type )
   {
      /* 1. Validate SR Configuration related parameter values  */
#ifdef LTEMAC_HDFDD
      if(hdFdd)
      {
         if (( srCfg->srSetup.srResIdx > RG_SCH_SR_RES_IDX   ) ||
               ( srCfg->srSetup.srCfgIdx < 5 ) ||
               ( srCfg->srSetup.srCfgIdx > RG_SCH_ISR_MAX_SUPP )) 
         {
            RLOG_ARG4(L_ERROR,DBG_CELLID,cellCb->cellId,  
                     "Invalid SR Index Cfg =%d"
                      "Max Supp=%d,Min Supp=%d CRNTI:%d", srCfg->srSetup.srCfgIdx, 
                     RG_SCH_ISR_MAX_SUPP, RG_SCH_ISR_MIN_SUPP,crnti);

            RETVALUE(RFAILED);
         }
      }
      else
      {
         if (( srCfg->srSetup.srResIdx > RG_SCH_SR_RES_IDX   ) ||
               ( srCfg->srSetup.srCfgIdx > RG_SCH_ISR_MAX_SUPP )) 
         {
            RLOG_ARG4(L_ERROR,DBG_CELLID,cellCb->cellId,  
                     "Invalid SR Index Cfg =%d" 
                      "Max Supp=%d,Min Supp=%d CRNTI:%d", srCfg->srSetup.srCfgIdx, 
                     RG_SCH_ISR_MAX_SUPP, RG_SCH_ISR_MIN_SUPP,crnti);

            RETVALUE(RFAILED);
         }
      }
#else
      if (( srCfg->srSetup.srResIdx > RG_SCH_SR_RES_IDX   ) ||
            ( srCfg->srSetup.srCfgIdx > RG_SCH_ISR_MAX_SUPP )) 
      {
         RLOG_ARG4(L_ERROR,DBG_CELLID,cellCb->cellId,  
                  "Invalid SR Index Cfg =%d"
                  "Max Supp=%d,Min Supp=%d CRNTI:%d", srCfg->srSetup.srCfgIdx, 
                  RG_SCH_ISR_MAX_SUPP, RG_SCH_ISR_MIN_SUPP,crnti);

         RETVALUE(RFAILED);
      }
#endif
   } 
   RETVALUE(ROK);
}


/*
 * @brief Validates the UE Aperiodic & Periodic  CQI, PMI, RI, SRS and SR Configuration 
 *             request from RRM to MAC.
 *
 * @details
 *
 *     Function :rgSCHCfgVldtCqiSrSrsUeCfg 
 *
 *     Processing Steps:
 *      - Validate the UE configuration request from RRC to MAC at CFG:
 *        validate the value range for Aperiodic & Periodic CQI, PMI, RI , SRS and SR values.
 *      - If validated successfully,
 *        - Return ROK.
 *      - Else 
 *        - Return RFAILED.
 *
 *  @param[in]  RgSchCellCb  *cellCb
 *  @param[in]  RgrUeCfg     *ueCfg
 *  @param[out] RgSchErrInfo *errInfo
 *  @return  S16
 *      -# ROK
 *      -# RFAILED
 */
#ifdef ANSI
PRIVATE S16 rgSCHCfgVldtCqiSrSrsUeCfg 
(
 RgSchCellCb       *cellCb,  
 RgrUeCfg          *ueCfg,  
 RgSchErrInfo      *errInfo
 )
#else
PRIVATE S16  rgSCHCfgVldtCqiSrSrsUeCfg(cellCb, ueCfg,  errInfo)
   RgSchCellCb     *cellCb;  
   RgrUeCfg        *ueCfg;
   RgSchErrInfo    *errInfo;
#endif
{

   TRC2(rgSCHCfgVldtCqiSrSrsUeCfg);


   /*  1. Validate UE Aperiodic CQI related parameters */
   if ( ROK != rgSCHCfgVldtRgrUeACqiCfg (cellCb, ueCfg->crnti,
            &ueCfg->ueDlCqiCfg.aprdCqiCfg, ueCfg->txMode, errInfo ))
   {
      RLOG_ARG1(L_ERROR,DBG_CELLID,cellCb->cellId, 
       "Invalid Aperiodic CQI configuration CRNTI:%d",ueCfg->crnti);
      RETVALUE(RFAILED);
   }

   /*  1. Validate UE Periodic CQI/PMI, SRS and SR related parameters */
#ifdef LTEMAC_HDFDD
   if ( ROK != rgSCHCfgVldtRgrUePCqiCfg (cellCb, ueCfg->crnti,
            &ueCfg->ueDlCqiCfg.prdCqiCfg, ueCfg->isHdFddEnbld, 
            ueCfg->txMode, errInfo ))
#else
      if ( ROK != rgSCHCfgVldtRgrUePCqiCfg (cellCb, ueCfg->crnti,
             &ueCfg->ueDlCqiCfg.prdCqiCfg, ueCfg->txMode, errInfo ))
#endif
      {
         RLOG_ARG1(L_ERROR,DBG_CELLID,cellCb->cellId, 
          "Invalid Periodic CQI configuration CRNTI:%d",ueCfg->crnti);
         RETVALUE(RFAILED);
      }

   /*  2. Validate SRS Configuration related parameter values  */
#ifdef LTEMAC_HDFDD
   if ( ROK != rgSCHCfgVldtRgrUeUlSrsCfg(cellCb, ueCfg->crnti, &ueCfg->srsCfg,
            ueCfg->isHdFddEnbld, errInfo ))
#else
      if ( ROK != rgSCHCfgVldtRgrUeUlSrsCfg(cellCb, ueCfg->crnti, 
                  &ueCfg->srsCfg, errInfo ))
#endif
      {
         RLOG_ARG1(L_ERROR,DBG_CELLID,cellCb->cellId, 
           "Invalid SRS  configuration CRNTI:%d",ueCfg->crnti);
         RETVALUE(RFAILED);
      } 

   /*  3. Validate SR Configuration related parameter values  */
#ifdef LTEMAC_HDFDD
   if ( ROK != rgSCHCfgVldtRgrUeSrCfg (cellCb, ueCfg->crnti, &ueCfg->srCfg,
            ueCfg->isHdFddEnbld, errInfo))
#else
      if ( ROK != rgSCHCfgVldtRgrUeSrCfg (cellCb, ueCfg->crnti, 
                  &ueCfg->srCfg, errInfo))
#endif
      {
         RLOG_ARG1(L_ERROR,DBG_CELLID,cellCb->cellId, 
            "Invalid SR  configuration CRNTI:%d",ueCfg->crnti);
         RETVALUE(RFAILED);
      }

   RETVALUE(ROK);

}



/*****************************************************************
 *   UE PCQI, RI, SRS and SR Re Configuration  Validation Functions  *
 *                                                                *
 ******************************************************************/
/*
 * @brief Validates the UE Periodic CQI, PMI, RI, SRS and SR 
 * Re-configuration request from RRM to MAC.
 *
 * @details
 *
 *     Function : rgSCHCfgVldtCqiSrSrsUeReCfg 
 *
 *     Processing Steps:
 *      - Validate the UE Re configuration request from RRC to MAC at CFG:
 *        validate the value range for Periodic CQI, PMI, RI, SRS and SR values.
 *      - If validated successfully,
 *        - Return ROK.
 *      - Else 
 *        - Return RFAILED.
 *
 *  @param[in]   RgSchCellCb *cellCb
 *  @param[in]   RgSchUeCb   *ueCb
 *  @param[in]  RgrUeCfg     *ueCfg
 *  @param[out] RgSchErrInfo *errInfo
 *  @return  S16
 *      -# ROK
 *      -# RFAILED
 */
#ifdef ANSI
PRIVATE S16 rgSCHCfgVldtCqiSrSrsUeReCfg 
(
 RgSchCellCb          *cellCb,
 RgSchUeCb            *ueCb,
 RgrUeRecfg           *ueReCfg,  
 RgSchErrInfo         *errInfo
 )
#else
PRIVATE S16 rgSCHCfgVldtCqiSrSrsUeReCfg(cellCb, ueCb, ueReCfg, errInfo)
   RgSchCellCb          *cellCb;  
   RgSchUeCb            *ueCb;
   RgrUeRecfg           *ueReCfg;
   RgSchErrInfo         *errInfo;
#endif
{
   RgrUeTxModeCfg     txMode;
   TRC3(rgSCHCfgVldtCqiSrSrsUeReCfg);

   txMode.pres = TRUE;
   txMode.tmTrnstnState = RGR_TXMODE_RECFG_CMPLT;
   if ((ueReCfg->ueRecfgTypes & RGR_UE_TXMODE_RECFG) &&
       (ueReCfg->txMode.pres == TRUE)) 
   {
      txMode.txModeEnum = ueReCfg->txMode.txModeEnum;
   }
   else
   {
      txMode.txModeEnum = ueCb->mimoInfo.txMode;
   }

   /*    1. Validate UE CQI/PMI, SRS and SR related parameters */

   if ( ueReCfg->ueRecfgTypes & RGR_UE_APRD_DLCQI_RECFG ) 
   { 
      if ( ROK != rgSCHCfgVldtRgrUeACqiCfg (cellCb, ueReCfg->oldCrnti,     
               &ueReCfg->aprdDlCqiRecfg, txMode, errInfo ))
      {
         RLOG_ARG1(L_ERROR,DBG_CELLID,cellCb->cellId, 
         "Invalid Aperiodic CQI configuration OLD CRNTI:%d",ueReCfg->oldCrnti);
         RETVALUE(RFAILED);
      }
   }


   /*    2. Validate UE CQI/PMI, SRS and SR related parameters */

   if ( ueReCfg->ueRecfgTypes & RGR_UE_PCQI_RECFG ) 
   { 
#ifdef LTEMAC_HDFDD   
      if ( ROK != rgSCHCfgVldtRgrUePCqiCfg (cellCb, ueReCfg->oldCrnti,     
               &ueReCfg->cqiCfg, ueReCfg->isHdFddEnbld, txMode, errInfo ))
#else
         if ( ROK != rgSCHCfgVldtRgrUePCqiCfg (cellCb, ueReCfg->oldCrnti,     
                  &ueReCfg->cqiCfg, txMode, errInfo ))
#endif

         {
            RLOG_ARG1(L_ERROR,DBG_CELLID,cellCb->cellId, 
            "Invalid Periodic CQI configuration OLD CRNTI:%d",ueReCfg->oldCrnti);
            RETVALUE(RFAILED);
         }
   }

   if(ueReCfg->ueRecfgTypes & RGR_UE_SRS_RECFG )
   {
#ifdef LTEMAC_HDFDD
      if ( ROK != rgSCHCfgVldtRgrUeUlSrsCfg(cellCb, ueReCfg->oldCrnti,
               &ueReCfg->srsCfg, ueReCfg->isHdFddEnbld, errInfo ))
#else
         if ( ROK != rgSCHCfgVldtRgrUeUlSrsCfg(cellCb, ueReCfg->oldCrnti,
                  &ueReCfg->srsCfg, errInfo ))
#endif
         {
            RLOG_ARG1(L_ERROR,DBG_CELLID,cellCb->cellId, 
              "Invalid SRS  configuration OLD CRNTI:%d",ueReCfg->oldCrnti);
            RETVALUE(RFAILED);
         } 

   }

   if ( ueReCfg->ueRecfgTypes & RGR_UE_SR_RECFG )
   {
#ifdef LTEMAC_HDFDD
      if ( ROK != rgSCHCfgVldtRgrUeSrCfg (cellCb, ueReCfg->oldCrnti, 
            &ueReCfg->srCfg, ueReCfg->isHdFddEnbld, errInfo))
#else
         if ( ROK != rgSCHCfgVldtRgrUeSrCfg (cellCb, ueReCfg->oldCrnti, 
            &ueReCfg->srCfg, errInfo))
#endif
         {
            RLOG_ARG1(L_ERROR,DBG_CELLID,cellCb->cellId, 
                     "Invalid SR configuration OLD CRNTI:%d",ueReCfg->oldCrnti);
            RETVALUE(RFAILED);
         }
   }

   RETVALUE(ROK);
}


/*****************************************************************
 *   UE ACQI, PCQI, RI, SRS SR Configuration Functions      *
 *                                                                *
 ******************************************************************/
   /**
    * @brief Handles Aperiodic CQI , PMI, RI  configuration for a UE.
    *
    * @details
    *
    *     Function : rgSCHCfgACqiUeCfg 
    *
    *     Invoking Module Processing:
    *      - This shall be invoked by SCH_GOM at UE configuration. It 
    *      shall do the validations for the spec-defined values.
    *        
    *     Processing Steps:
    *     
    *     - Return ROK
    *
    *  @param[in]  RgSchCellCb  *cell
    *  @param[in]  RgSchUeCb    *ue
    *  @param[in]  RgrUeAprdDlCqiCfg *aCqiCfg
    *  @param[in]  CmLteUeCategory        ueCat
    *  @return  S16
    *      -# ROK
    *
    **/
#ifdef ANSI
PUBLIC S16 rgSCHCfgACqiUeCfg
(
RgSchCellCb         *cellCb, 
RgSchUeCb           *ueCb, 
RgrUeAprdDlCqiCfg   *aCqiCfg,
CmLteUeCategory     ueCat
)
#else
PUBLIC S16 rgSCHCfgACqiUeCfg(cellCb, ueCb, aCqiCfg, ueCat)
RgSchCellCb       *cellCb; 
RgSchUeCb         *ueCb; 
RgrUeAprdDlCqiCfg *aCqiCfg;
CmLteUeCategory   ueCat;
#endif
{
   U8     M;  /*Num of Subbands -- Applicable only for Mode 2-0 and 2-2*/
   U8     k;  /*SubBand Size (RB)  -- 
              Holds different values depending on Mode*/
   U8     cqiPmiSzR1;   /*CQIPMI Size for Rank =1*/
   U8     cqiPmiSzRn1; /*CQIPMI Size for Rank > 1*/

   TRC3(rgSCHCfgACqiUeCfg);

   cqiPmiSzR1 = 0;
   cqiPmiSzRn1 = 0;
   

   ueCb->acqiCb.aCqiCfg.pres = aCqiCfg->pres;
   ueCb->acqiCb.aCqiCfg.aprdModeEnum = aCqiCfg->aprdModeEnum;

   if(aCqiCfg->pres)
   {           
      switch(aCqiCfg->aprdModeEnum)
      {
         case RGR_APRD_CQI_MOD12:
         case RGR_APRD_CQI_MOD30:
         case RGR_APRD_CQI_MOD31:
            RG_SCH_GET_CQI_K_VAL(cellCb->bwCfg.dlTotalBw, k);
            ueCb->acqiCb.N = RGSCH_CEIL(cellCb->bwCfg.dlTotalBw, k);
            ueCb->acqiCb.k = k;
            break;

         case RGR_APRD_CQI_MOD20:
         case RGR_APRD_CQI_MOD22:
            RG_SCH_GET_SBCQI_M_K_VAL(cellCb->bwCfg.dlTotalBw, M, k);
            ueCb->acqiCb.N = RGSCH_CEIL(cellCb->bwCfg.dlTotalBw, k);
            ueCb->acqiCb.k = k;
            ueCb->acqiCb.M = M;
            ueCb->acqiCb.L = RgSCHUeAcqi2022LBitWidth[M-1][ueCb->acqiCb.N-1];
            break;

         default:
            break; 
      }
      if(ueCb->mimoInfo.txMode == RGR_UE_TM_3 || 
            ueCb->mimoInfo.txMode == RGR_UE_TM_4)
      {
         if(cellCb->numTxAntPorts ==2)
         {
            ueCb->acqiCb.riNumBits = 1;
         }
         else if(cellCb->numTxAntPorts ==4)
         {
            if(ueCat == CM_LTE_UE_CAT_5)
            {
               ueCb->acqiCb.riNumBits = 2;
            }
            else
            {
               ueCb->acqiCb.riNumBits = 1;
            }
         }
      }
      rgSCHCfgUtlFetchAcqiBitSz(ueCb, cellCb->numTxAntPorts, 
            &cqiPmiSzR1, &cqiPmiSzRn1);
      ueCb->acqiCb.cqiPmiSzR1 = cqiPmiSzR1;
      ueCb->acqiCb.cqiPmiSzRn1 = cqiPmiSzRn1;
   }
   RETVALUE(ROK);
}

/**
 * @brief Handles Periodic CQI , PMI, RI  configuration for a UE.
 *
 * @details
 *
 *     Function : rgSCHCfgPCqiUeCfg 
 *
 *     Invoking Module Processing:
 *      - This shall be invoked by SCH_GOM at UE configuration. It shall do the
 *      validations for the spec-defined values.
 *        
 *     Processing Steps:
 *     - For UE-specific Periodic CQI  related configuration, 
 *      - If Periodic CQI/PMI is configured,
 *        - Update UE with the configured values.
 - Compute and Update next occurance of CQI/PMI or RI Tranmission instance.
 - Update the CQI offset and CQI perodicity information
 - Add Ue to cell's list  
 *        
 *
 *     - For UE-specific Periodic RI  related configuration, 
 *      - If Periodic RI is configured,
 *        - Update UE with the configured values.
 - Compute and Update next occurance of RI Tranmission instance.
 - Update the  RI offset and RI perodicity information
 *      
 *
 *     - Return ROK
 *
 *  @param[in]  RgSchCellCb  *cell
 *  @param[in]  RgSchUeCb    *ue
 *  @param[in]  RgrUePrdDlCqiCfg *cqiCfg
 *  @param[in]  CmLteUeCategory   ueCat
 *  @return  S16
 *      -# ROK
 *
 **/
#ifdef ANSI
PUBLIC S16 rgSCHCfgPCqiUeCfg
(
 RgSchCellCb         *cellCb, 
 RgSchUeCb           *ueCb, 
 RgrUePrdDlCqiCfg    *cqiCfg,
 CmLteUeCategory     ueCat
 )
#else
PUBLIC S16 rgSCHCfgPCqiUeCfg(cellCb, ueCb, cqiCfg, ueCat)
   RgSchCellCb       *cellCb; 
   RgSchUeCb         *ueCb; 
   RgrUePrdDlCqiCfg  *cqiCfg;
   CmLteUeCategory   ueCat;
#endif
{  
   U16    crntTime;           
   U16    cqiTrInstTime; 
   U8     j;  /*Bandwidth Parts*/
   U8     temp; 
   U8     loop;
   TRC3(rgSCHCfgPCqiUeCfg);

   crntTime = (cellCb->crntTime.sfn * RGSCH_NUM_SUB_FRAMES)+
                  (cellCb->crntTime.subframe);
   /* Periodic CQI is setup  */
   if (cqiCfg->type == RGR_SCH_PCQI_SETUP)
   {   
      for(loop = 0; loop < MAX_CQI_RI_RPT_BUFF;loop++)
      {
         ueCb->rawCqiBitW[loop].type = TFU_RECP_REQ_INVLD; /* setting invalid  type*/
      }
      /*  1. Copy the Received CQI Cfg parameters to ueCb  */
      cmMemcpy((U8 *)&ueCb->cqiCb.cqiCfg, (U8 *)cqiCfg, 
            sizeof(RgrUePrdDlCqiCfg));

      /*  2. Compute Periodic CQI Periodicity and subframe offset   */
#ifndef LTE_TDD           
      rgSCHUtlGetCfgPerOff(RG_SCH_FDD_PCQI_TBL, cqiCfg->cqiSetup.cqiPCfgIdx,
            &ueCb->cqiCb.cqiPeri, &ueCb->cqiCb.cqiOffset);      
#else
      rgSCHUtlGetCfgPerOff( RG_SCH_TDD_PCQI_TBL, 
            cqiCfg->cqiSetup.cqiPCfgIdx,
            &ueCb->cqiCb.cqiPeri, &ueCb->cqiCb.cqiOffset);    
#endif
      RLOG_ARG3(L_DEBUG,DBG_CELLID,cellCb->cellId,
            "rgSCHCfgPCqiUeCfg(): UEID:%d CQI Peri=%d, CQI Offset=%d",
            ueCb->ueId,
            ueCb->cqiCb.cqiPeri,
            ueCb->cqiCb.cqiOffset);


      cqiTrInstTime = ((ueCb->cqiCb.cqiPeri+crntTime) - ueCb->cqiCb.cqiOffset)
         %ueCb->cqiCb.cqiPeri;
      ueCb->cqiCb.nCqiTrIdx = (crntTime + 
            (ueCb->cqiCb.cqiPeri - cqiTrInstTime));
      /* Introduced timing delta for reception req
       * in FDD*/
      if(ueCb->cqiCb.nCqiTrIdx  <= (crntTime + TFU_RECPREQ_DLDELTA))
      {
         ueCb->cqiCb.nCqiTrIdx = ueCb->cqiCb.nCqiTrIdx + ueCb->cqiCb.cqiPeri;
      }
      ueCb->cqiCb.nCqiTrIdx = ueCb->cqiCb.nCqiTrIdx
         %RG_SCH_PCQI_SRS_SR_TRINS_SIZE;
      if(RGR_UE_PCQI_SB_REP == cqiCfg->cqiSetup.cqiRepType)
      {
         U8     k;  /*SubBand Size (RB) */
         RG_SCH_GET_CQI_J_VAL(cellCb->bwCfg.dlTotalBw, j);
         RG_SCH_GET_CQI_K_VAL(cellCb->bwCfg.dlTotalBw, k);
         ueCb->cqiCb.J = j; /*Number of Bandwidth Parts*/
         /*h: reporting instances required for a complete CQI/PMI report */
         /*j:Number of bandwidth parts; k: Subband Size*/
         ueCb->cqiCb.h = (ueCb->cqiCb.cqiCfg.cqiSetup.k *j )+1; 
         /* ccpu00140905- L-size is coming as 3 for 100Rbs where it should be 2*/
         temp = RGSCH_CEIL(cellCb->bwCfg.dlTotalBw, (j*k));
         ueCb->cqiCb.label = (temp & (temp-1)) ?
            (1+ rgSCHUtlLog32bitNbase2(temp)) : rgSCHUtlLog32bitNbase2(temp);
      }
      /*    Place the UE in cellCb->tIUeLstCp  */
      ueCb->cqiCb.cqiLstEnt.node=(PTR) ueCb;


      cmLListAdd2Tail(&cellCb->pCqiSrsSrLst[ueCb->cqiCb.nCqiTrIdx].cqiLst,
            &ueCb->cqiCb.cqiLstEnt);  


      /* 4. Rank Indicator Cfg handler */
      rgSCHCfgRiUeCfg(cellCb, ueCb, cqiCfg, ueCat);
   }
   else
   { 
      ueCb->cqiCb.cqiCfg.type =  RGR_SCH_PCQI_REL;
      if(ueCb->cqiCb.nCqiTrIdx != RG_SCH_INVALID_IDX)
      {
         cmLListDelFrm(&cellCb->pCqiSrsSrLst[ueCb->cqiCb.nCqiTrIdx].cqiLst,\
               &ueCb->cqiCb.cqiLstEnt);
      } 
      if(ueCb->cqiCb.nRiTrIdx != RG_SCH_INVALID_IDX)
      {
         cmLListDelFrm(&cellCb->pCqiSrsSrLst[ueCb->cqiCb.nRiTrIdx].riLst, \
               &ueCb->cqiCb.riLstEnt);
      }
      ueCb->cqiCb.nCqiTrIdx = RG_SCH_INVALID_IDX;
      ueCb->cqiCb.nRiTrIdx   = RG_SCH_INVALID_IDX;
      ueCb->cqiCb.riDist   = RG_SCH_INVALID_IDX; 
   } 
   RETVALUE(ROK);
}

/**
 * @brief Handles Periodic  RI  configuration for a UE.
 *
 * @details
 *
 *     Function : rgSCHCfgRiUeCfg 
 *
 *     Invoking Module Processing:
 *      - This shall be invoked by SCH_GOM at UE configuration. It shall do the
 *      validations for the spec-defined values.
 *        
 *     Processing Steps:
 *     - For UE-specific Periodic RI  related configuration, 
 *      - If Periodic RI is configured,
 *        - Update UE with the configured values.
 - Compute and Update next occurance of RI Tranmission instance.
 - Update the  RI offset and RI perodicity information
 *
 *     - Return ROK
 *
 *  @param[in]  RgSchCellCb  *cell
 *  @param[in]  RgSchUeCb    *ue
 *  @param[in]  RgrUePrdDlCqiCfg *cqiCfg
 *  @param[in]  CmLteUeCategory   ueCat
 *  @return  S16
 *      -# ROK
 *
 **/
#ifdef ANSI
PUBLIC S16 rgSCHCfgRiUeCfg
(
 RgSchCellCb        *cellCb, 
 RgSchUeCb          *ueCb, 
 RgrUePrdDlCqiCfg   *cqiCfg,
 CmLteUeCategory    ueCat
 )
#else
PUBLIC S16 rgSCHCfgRiUeCfg(cellCb, ueCb, cqiCfg, ueCat)
   RgSchCellCb       *cellCb; 
   RgSchUeCb         *ueCb; 
   RgrUePrdDlCqiCfg  *cqiCfg;
   CmLteUeCategory   ueCat;
#endif
{
   U16     crntTime;  
   U16     riTrInsTime;  
   U8      j; /*Bandwidth parts. Valid for Modes 2-0, 2-1*/         
   U16     periodicity; 
   U16    tempIdx; 

   TRC3(rgSCHCfgRiUeCfg);



   crntTime = (cellCb->crntTime.sfn * RGSCH_NUM_SUB_FRAMES)
      +(cellCb->crntTime.subframe);
   /* 1. Rank Indicator is enabled  */
   if(cqiCfg->cqiSetup.riEna)
   {

      rgSCHUtlGetCfgPerOff(RG_SCH_RI_TBL, 
            cqiCfg->cqiSetup.riCfgIdx,
            &ueCb->cqiCb.riPeri, &ueCb->cqiCb.riOffset);

      RLOG_ARG3(L_DEBUG,DBG_CELLID,cellCb->cellId,
            "rgSCHCfgRiUeCfg(): RI Peri=%d, RI Offset=%d UEID:%d", 
            ueCb->cqiCb.riPeri,ueCb->cqiCb.riOffset,ueCb->ueId);

      ueCb->cqiCb.perRiVal = 1;
      ueCb->cqiCb.invalidateCqi = FALSE;

      if(RGR_UE_PCQI_WB_REP == cqiCfg->cqiSetup.cqiRepType)
      {
         /* 
            1. wideband RI reporting is configured 
            (Mode 1-0 or 1-1)
            (10*sfn+floor(subframe)-Noffsetcqi-NoffsetRI )Mod(NCqiperiod
          *MriPeriod)=0  
          */ 
         periodicity = ueCb->cqiCb.cqiPeri * ueCb->cqiCb.riPeri; 
      }
      else
      {
         /*
         *  Where Widesband and Subband RI reporting is configured
         *   (Mode 2-0 or 2-1 )
         *   (10*sfn+floor(subframe)-Noffsetcqi-NoffsetRI )
         *   Mod(H. NCqiperiod *MriPeriod )=0 
         *   where H= J * K +1;  J=Number of bandwidth parts(BW/subsize). 
         *   K is RGR interf input 
         */

         RG_SCH_GET_CQI_J_VAL(cellCb->bwCfg.dlTotalBw, j);
         ueCb->cqiCb.h = (ueCb->cqiCb.cqiCfg.cqiSetup.k *j )+1;  
         periodicity = ueCb->cqiCb.h * ueCb->cqiCb.cqiPeri * 
                                  ueCb->cqiCb.riPeri; 
      }

      /* In case of SFN wraparound, the SB CQI reporting cycle breaks
       * and RI->WB CQI->SBCQI.. should resume. RI is repositioned 
       * accordingly. WBCQI handling is naturally accomplished */
      if (periodicity >= RGSCH_MAX_SUBFRM)
      {
         periodicity = ueCb->cqiCb.cqiOffset - ueCb->cqiCb.riOffset + 
                       RGSCH_MAX_SUBFRM - (crntTime);
         tempIdx = crntTime + periodicity;
      }
      else
      {
         if ((crntTime + TFU_RECPREQ_DLDELTA + periodicity) > 
                            (RGSCH_MAX_SUBFRM - 1))
         {
            riTrInsTime = (periodicity - ueCb->cqiCb.cqiOffset + ueCb->cqiCb.riOffset) % periodicity; 
            tempIdx = RGSCH_MAX_SUBFRM + (periodicity - riTrInsTime);
         }
         else
         {
            riTrInsTime = ((periodicity +crntTime )- \
               ueCb->cqiCb.cqiOffset + ueCb->cqiCb.riOffset)\
                       % periodicity;
            tempIdx = (crntTime + (periodicity -riTrInsTime));
         }
      }
      if (tempIdx <= (crntTime + TFU_RECPREQ_DLDELTA))
      {
         tempIdx = tempIdx + periodicity; 
      }
      ueCb->cqiCb.nRiTrIdx = tempIdx 
         % RG_SCH_PCQI_SRS_SR_TRINS_SIZE;
      if(periodicity >= RG_SCH_PCQI_SRS_SR_TRINS_SIZE)
      {  
         ueCb->cqiCb.riDist = rgSCHUtlFindDist((U16)(crntTime + TFU_RECPREQ_DLDELTA),
                (U16) tempIdx);
      }
      else
      {
         ueCb->cqiCb.riDist =0; 
      }
      if(ueCb->mimoInfo.txMode == RGR_UE_TM_3 
            || ueCb->mimoInfo.txMode == RGR_UE_TM_4)
      {
         if (cellCb->numTxAntPorts ==2)
         {
            ueCb->cqiCb.riNumBits = 1;
         }
         else if(cellCb->numTxAntPorts ==4)
         {
            if(ueCat == CM_LTE_UE_CAT_5)
            {
               ueCb->cqiCb.riNumBits = 2;
            }
            else
            {
               ueCb->cqiCb.riNumBits = 1;
            }
         }
      }
      /*    Place the UE in cellCb->tIUeLstCp  */
      ueCb->cqiCb.riLstEnt.node=(PTR) ueCb;

      cmLListAdd2Tail(&cellCb->pCqiSrsSrLst[ueCb->cqiCb.nRiTrIdx].riLst,
            &ueCb->cqiCb.riLstEnt);  
     

   }
   else
   {
      ueCb->cqiCb.nRiTrIdx = RG_SCH_INVALID_IDX;
      ueCb->cqiCb.riDist   = RG_SCH_INVALID_IDX; 
   }

   RETVALUE(ROK);

}

/* @brief Handles SRS configuration for a UE.
 *
 * @details
 *
 *     Function : rgSCHCfgSrsUeCfg 
 *
 *     Invoking Module Processing:
 *      - This shall be invoked by SCH_GOM at UE configuration. It shall do the
 *      validations for the spec-defined values.
 *        
 *     Processing Steps:
 *     - For UE-specific SRS  related configuration, 
 *      - If SRS is configured,
 *        - Update UE with the configured values.
 - Compute and Update next occurance of SRS Tranmission instance.
 - Update the  SRS offset and SRS perodicity information
 - Add Ue to cell's srs list
 *      - else
 *        - next occurance transmission instance of SRS  = RG_SCH_INVALID_IDX
 *
 *     - Return ROK
 *
 *  @param[in]  RgSchCellCb  *cell
 *  @param[in]  RgSchUeCb    *ue
 *  @param[in]  RgrUeUlSrsCfg   *srsCfg
 *
 *  @return  S16
 *      -# ROK
 *      -# RFAILED
 */
#ifdef ANSI
PUBLIC S16 rgSCHCfgSrsUeCfg
(
 RgSchCellCb     *cellCb,
 RgSchUeCb       *ueCb,
 RgrUeUlSrsCfg   *srsCfg 
 )
#else
PUBLIC S16 rgSCHCfgSrsUeCfg(cellCb, ueCb, srsCfg)
   RgSchCellCb     *cellCb;
   RgSchUeCb       *ueCb;
   RgrUeUlSrsCfg   *srsCfg;
#endif
{
   U16      srsTrInsTime;
   U16      crntTime;  
   U16      tempIdx;

   TRC3(rgSCHCfgSrsUeCfg);


   crntTime = (cellCb->crntTime.sfn * RGSCH_NUM_SUB_FRAMES)
      +(cellCb->crntTime.subframe);

   if(RGR_SCH_SRS_SETUP == srsCfg->type) 
   {
      /*  1. Copy the Received Cfg parameters to local cb  */
      cmMemcpy((U8 *)&ueCb->srsCb.srsCfg, (U8 *)srsCfg, sizeof(RgrUeUlSrsCfg));

#ifndef LTE_TDD
      /* 2.  Compute SRS Offset and Periodicity */
      rgSCHUtlGetCfgPerOff( RG_SCH_FDD_SRS_TBL, 
            srsCfg->srsSetup.srsCfgIdx,
            &ueCb->srsCb.peri, &ueCb->srsCb.offset);      
#else
      rgSCHUtlGetCfgPerOff( RG_SCH_TDD_SRS_TBL, 
            srsCfg->srsSetup.srsCfgIdx,
            &ueCb->srsCb.peri, &ueCb->srsCb.offset);      
#endif       

      RLOG_ARG3(L_DEBUG,DBG_CELLID,cellCb->cellId,
            "rgSCHCfgSrsUeCfg(): SRS Peri=%d, SRS Offset=%d UEID:%d", 
            ueCb->srsCb.peri,ueCb->srsCb.offset,ueCb->ueId);

      /*  3. Compute next Tranmission index for SRS */ 
      /*     Referenence: 36.213 Section:8.2
             i. SRS transmission instances for TDD with  period > 2 and for FDD are 
             ((10*sfn +Ksrs-suframeoffset))/mod(periodicity))  
             FDD: Ksrs is 0...9
             TDD: Ksrs Table 8.2-3: 
             ii.The SRS transmission instances for TDD (periodicity == 2)
             (Ksrs-Toffset)mod(5)==0.  Note: This is not supported now  
      */ 

      srsTrInsTime = ((ueCb->srsCb.peri+crntTime) - ueCb->srsCb.offset)
         %ueCb->srsCb.peri;
     tempIdx = (crntTime + (ueCb->srsCb.peri - srsTrInsTime));
#ifdef LTE_TDD
      if (tempIdx  <=  (crntTime + TFU_DELTA))
#else
      if (tempIdx  <=  (crntTime + TFU_RECPREQ_DLDELTA))
#endif
      {
         tempIdx = tempIdx + ueCb->srsCb.peri;
      }
      ueCb->srsCb.nSrsTrIdx =(U16) (tempIdx 
         % RG_SCH_PCQI_SRS_SR_TRINS_SIZE);
      if(ueCb->srsCb.peri >= RG_SCH_PCQI_SRS_SR_TRINS_SIZE)
      {
#ifdef LTE_TDD
         ueCb->srsCb.srsDist = rgSCHUtlFindDist((U8)(crntTime+TFU_DELTA), 
                                                    (U16)tempIdx); 
#else
         ueCb->srsCb.srsDist = rgSCHUtlFindDist((U8)(crntTime + TFU_RECPREQ_DLDELTA), 
                                                    (U16)tempIdx); 
#endif
      }
      else
      {
         ueCb->srsCb.srsDist = 0; 
      }

      /*UE Tx Antenna Selection - START*/
      if(ueCb->ul.ulTxAntSel.pres == TRUE )
      {
          /*for both partial and full sounding bandwidth, 
           and when frequency hopping is disabled */
         ueCb->srsCb.selectedAnt = (crntTime/ueCb->srsCb.peri)%2; 
      }
      else
      {
         /* TS 36.213 specifies that if Tx Antenna Selection is 
            disabled/not supported then its Port 0*/
         ueCb->srsCb.selectedAnt=0; 
      }
      ueCb->validTxAnt = ueCb->srsCb.selectedAnt; 
      /*UE Tx Antenna Selection - ENDS*/

      ueCb->srsCb.srsLstEnt.node=(PTR)ueCb;
      cmLListAdd2Tail(&cellCb->pCqiSrsSrLst[ueCb->srsCb.nSrsTrIdx].srsLst,
            &ueCb->srsCb.srsLstEnt);  


   }
   else
   {
      /*  SRS Release / Not configured */
      ueCb->srsCb.srsCfg.type = RGR_SCH_SRS_REL;
      if(ueCb->srsCb.nSrsTrIdx != RG_SCH_INVALID_IDX)
      {
         cmLListDelFrm(&cellCb->pCqiSrsSrLst[ueCb->srsCb.nSrsTrIdx].srsLst,
               &ueCb->srsCb.srsLstEnt);
      }
      ueCb->srsCb.nSrsTrIdx = RG_SCH_INVALID_IDX; 
      ueCb->srsCb.srsLstEnt.node =(PTR) NULLP;
   }

   RETVALUE(ROK);
}


/* * @brief Handles SR configuration for a UE.
 *
 * @details
 *
 *     Function : rgSCHCfgSrUeCfg 
 *
 *     Invoking Module Processing:
 *      - This shall be invoked by SCH_GOM at UE configuration. It shall do the
 *      validations for the spec-defined values.
 *        
 *      - If SR is configured,
 *        - Update UE with the configured values.
 - Compute and Update next occurance of SR Tranmission instance.
 - Update the  SR offset and SR perodicity information
 - Add Ue to cell->tIUeLstCp[n]  
 *      - else
 *        - next occurance transmission instance of SR = RG_INVALID_SR_ID
 *
 *
 *     - Return ROK
 *
 *  @param[in]  RgSchCellCb  *cell
 *  @param[in]  RgSchUeCb    *ue
 *  @param[in]  RgrUeSrCfg       *srCfg
 *
 *  @return  S16
 *      -# ROK
 *      -# RFAILED
 */
#ifdef ANSI
PUBLIC S16 rgSCHCfgSrUeCfg
(
 RgSchCellCb      *cellCb,
 RgSchUeCb        *ueCb,
 RgrUeSrCfg       *srCfg
 )
#else
PUBLIC S16 rgSCHCfgSrUeCfg(cellCb, ueCb, srCfg)
   RgSchCellCb    *cellCb;
   RgSchUeCb      *ueCb;
   RgrUeSrCfg     *srCfg;
#endif
{
   U16     srTrInsTime;
   U16     crntTime;

   TRC3(rgSCHCfgSrUeCfg);


   crntTime = (cellCb->crntTime.sfn * RGSCH_NUM_SUB_FRAMES)
      +(cellCb->crntTime.subframe);
   if(srCfg->type == RGR_SCH_SR_SETUP) 
   {
      /*  1. Copy the Received Cfg parameters to local cb  */
      cmMemcpy((U8 *)&ueCb->srCb.srCfg, (U8 *)srCfg, sizeof(RgrUeSrCfg));


      /*  2. Compute SR periodicity and offset */
      rgSCHUtlGetCfgPerOff( RG_SCH_SR_TBL, 
            srCfg->srSetup.srCfgIdx,
            &ueCb->srCb.peri, &ueCb->srCb.offset);
    
      RLOG_ARG4(L_DEBUG,DBG_CELLID,cellCb->cellId ,
            "SR Config: idx(%u), period (%u) offset (%u) UEID:%d",
            srCfg->srSetup.srCfgIdx,
            ueCb->srCb.peri, 
            ueCb->srCb.offset,
            ueCb->ueId);

      /*   3. Compute Next Transmission Instance */

      srTrInsTime = ((ueCb->srCb.peri+crntTime) - ueCb->srCb.offset)
         %ueCb->srCb.peri;
      ueCb->srCb.nSrTrIdx = (crntTime + (ueCb->srCb.peri- srTrInsTime)); 
#ifdef LTE_TDD
      if (ueCb->srCb.nSrTrIdx  <= (crntTime + TFU_DELTA))
#else
      if (ueCb->srCb.nSrTrIdx  <= (crntTime + TFU_RECPREQ_DLDELTA))
#endif
      {
         ueCb->srCb.nSrTrIdx = ueCb->srCb.nSrTrIdx +  ueCb->srCb.peri;
      }
      ueCb->srCb.nSrTrIdx = ueCb->srCb.nSrTrIdx 
         % RG_SCH_PCQI_SRS_SR_TRINS_SIZE;
      ueCb->srCb.srLstEnt.node= (PTR) ueCb;

      /* 4. Place UE in Cell SR Tranmisison Instance List  */
      cmLListAdd2Tail(&cellCb->pCqiSrsSrLst[ueCb->srCb.nSrTrIdx].srLst,
            &ueCb->srCb.srLstEnt);  
   }
   else
   {
      ueCb->srCb.srCfg.type =  RGR_SCH_SR_REL;

      if(ueCb->srCb.nSrTrIdx != RG_SCH_INVALID_IDX)
      {
         cmLListDelFrm(&cellCb->pCqiSrsSrLst[ueCb->srCb.nSrTrIdx].srLst,
               &ueCb->srCb.srLstEnt);
      }
      ueCb->srCb.nSrTrIdx = RG_SCH_INVALID_IDX; 
      ueCb->srCb.srLstEnt.node = (PTR)NULLP;
   }

   RETVALUE(ROK);
}


/*****************************************************************
 *    UE PCQI, RI, SRS and SR       Re Configuration Functions    *
 *                                                                *
 ******************************************************************/


/* * @brief Handles Periodic CQI, PMI, RI  Re-configuration for a UE.
 *
 * @details
 *
 *     Function : rgSCHCfgPCqiUeReCfg 
 *
 *     Invoking Module Processing:
 *      - This shall be invoked by SCH_GOM at UE Re configuration. It shall do the
 *      validations for the spec-defined values.
 *        
 *     Processing Steps:
 *     - For UE-specific Periodic CQI  related configuration, 
 *      - If Periodic CQI/PMI is re configured(first time enabled),
 *          - Update UE with the configured values.
 *         - Compute and Update next occurance of CQI/PMI or RI Tranmission 
 *         instance.
 *         - Update the CQI offset and CQI perodicity information
 *         - Add Ue to cell's list 
 *      - If Periodic CQI/PMI is re configured(modify),
 *         - Update UE with the configured values.
 *         - Del Ue from cell->tIUeLstCp list 
 *          - Compute and Update next occurance of CQI/PMI or RI Tranmission
 *          instance.
 *         - Update the CQI offset and CQI perodicity information
 *         - Add Ue to cell's list
 *      - If Periodic CQI/PMI is re configured(disabled),
 *        - Update UE with the configured values.
 *         - Del Ue from cell's list  
 *         - Update next occurance of CQI/PMI or RI Tranmission instance.
 *        - next occurance of CQI/PMI = RG_INVALID_CQIPMI_ID
 *
 *     - For UE-specific Periodic RI  related configuration, 
 *      - If Periodic RI is configured(first time enabled),
 *        - Update UE with the configured values.
 *          - Compute and Update next occurance of RI Tranmission instance.
 *         - Update the  RI offset and RI perodicity information
 *      - If Periodic RI is configured(modify),
 *        - Update UE with the configured values.
 *         - Compute and Update next occurance of RI Tranmission instance.
 *         - Update the  RI offset and RI perodicity information
 *      - else
 *        - next occurance of RI  = RG_INVALID_CQIPMI_ID
 *
 *     - Return ROK
 *
 *  @param[in]  RgSchCellCb  *cell
 *  @param[in]  RgSchUeCb    *ue
 *  @param[in]  RgrUeCfg     *ueCfg
 *
 *  @return  S16
 *      -# ROK
 *
 */
#ifdef ANSI
PUBLIC S16 rgSCHCfgPCqiUeReCfg
(
 RgSchCellCb        *cellCb, 
 RgSchUeCb          *ueCb, 
 RgrUePrdDlCqiCfg   *cqiCfg,
 CmLteUeCategory      ueCat
 )
#else
PUBLIC S16  rgSCHCfgPCqiUeReCfg(cellCb, ueCb, cqiCfg, ueCat)
   RgSchCellCb       *cellCb; 
   RgSchUeCb         *ueCb; 
   RgrUePrdDlCqiCfg  *cqiCfg;
   CmLteUeCategory  ueCat;
#endif
{

   TRC3(rgSCHCfgPCqiUeReCfg);

   /* Fix: ccpu00124008 Fix for incorrect check causing missed CQI reception instance */
   if((cqiCfg->type == RGR_SCH_PCQI_SETUP) &&
         (ueCb->cqiCb.cqiCfg.type != RGR_SCH_PCQI_SETUP))
   {
      /* 1. cqi is in Release (Disable) state,  Recfg is allowing Setup (Enable)
      */
      rgSCHCfgPCqiUeCfg(cellCb, ueCb, cqiCfg, ueCat);

   }
   else if((cqiCfg->type == RGR_SCH_PCQI_SETUP) &&
         (ueCb->cqiCb.cqiCfg.type == RGR_SCH_PCQI_SETUP ))
   {

      /*
         2. Present is SETUP(Enable) state,  Recfg is modifying SETUP(Enable)

         2.1 Delete UE from the cqiList 
         2.2 Set next occurance Transmission instace to "INVALID"
         2.3 Compute Next occurance Transmission instace 
         2.4 Placed ue in Transmission instance list.
         */    
      if(ueCb->cqiCb.nCqiTrIdx != RG_SCH_INVALID_IDX )
         cmLListDelFrm(&cellCb->pCqiSrsSrLst[ueCb->cqiCb.nCqiTrIdx].cqiLst,
               &ueCb->cqiCb.cqiLstEnt); 

      if(ueCb->cqiCb.nRiTrIdx != RG_SCH_INVALID_IDX )
         cmLListDelFrm(&cellCb->pCqiSrsSrLst[ueCb->cqiCb.nRiTrIdx].riLst,
               &ueCb->cqiCb.riLstEnt); 


      ueCb->cqiCb.cqiLstEnt.next = NULLP;
      ueCb->cqiCb.cqiLstEnt.prev  = NULLP;
      ueCb->cqiCb.nCqiTrIdx = RG_SCH_INVALID_IDX;
      ueCb->cqiCb.nRiTrIdx  = RG_SCH_INVALID_IDX;
      ueCb->cqiCb.riDist    = RG_SCH_INVALID_IDX;

      rgSCHCfgPCqiUeCfg(cellCb, ueCb, cqiCfg, ueCat);
   }
   else
   {
      /* Present is SETUP(Enable) state,  Recfg is Release(Disable)  */
      rgSCHCfgPCqiUeCfg(cellCb, ueCb, cqiCfg, ueCat);
   }

   /* ccpu00140578:: */
   ueCb->cqiCb.riRecpPrcsd = FALSE;
   RETVALUE(ROK);
}


/* * @brief Handles SRS  Re-configuration for a UE.
 *
 * @details
 *
 *     Function : rgSCHCfgSrsUeReCfg 
 *
 *     Invoking Module Processing:
 *      - This shall be invoked by SCH_GOM at UE Re configuration. It shall do the
 *      validations for the spec-defined values.
 *        
 *     Processing Steps:
 *     - For UE-specific SRS  related re configuration, 
 *      - If SRS is configured modified(First time Enabled),
 *        - Update UE with the configured values.
 *         - Compute and Update next occurance of SRS Tranmission instance.
 *        - Update the  SRS offset and SRS perodicity information
 *        - Add Ue to cell's list  
 *      - If SRS is configured modified(Changed offset or index ),
 *      - Delete UE from cell->tIUeLstCp[n] if present  
 *      - Update UE with the configured values.
 *      - Compute and Update next occurance of SRS Tranmission instance.
 *      - Update the  SRS offset and SRS perodicity information
 *      - Add Ue to cell's list  
 *      - If SRS is configured modified(disabled),
 *      - Delete UE from cell->tIUeLstCp[n] if present  
 *        - Update next occurance of SRS Tranmission instance to "INVALID".
 *        - Update the  SRS offset and SRS perodicity information "INVALID"
 *      - else
 *        - ROK 
 *
 *     - Return ROK
 *
 *  @param[in]  RgSchCellCb  *cell
 *  @param[in]  RgSchUeCb    *ue
 *  @param[in]  RgrUeUlSrsCfg    *srsCfg
 *
 *  @return  S16
 *      -# ROK
 *      -# RFAILED
 */
#ifdef ANSI
PUBLIC S16 rgSCHCfgSrsUeReCfg
(
 RgSchCellCb        *cellCb, 
 RgSchUeCb          *ueCb, 
 RgrUeUlSrsCfg      *srsCfg
 )
#else
PUBLIC S16 rgSCHCfgSrsUeReCfg(cellCb, ueCb, srsCfg)
   RgSchCellCb      *cellCb; 
   RgSchUeCb        *ueCb; 
   RgrUeUlSrsCfg    *srsCfg;
#endif
{

   TRC3(rgSCHCfgSrsUeReCfg);



   if(( RGR_SCH_SRS_SETUP == srsCfg->type) ||
         ( RGR_SCH_SRS_SETUP != ueCb->srsCb.srsCfg.type ))
   {
      /*     1. Present is Release(Disable) state,  Recfg is allowing
       *     SETUP(Enable)  */
      rgSCHCfgSrsUeCfg(cellCb, ueCb, srsCfg);
   } 
   else if (( RGR_SCH_SRS_SETUP == srsCfg->type ) &&
         ( RGR_SCH_SRS_SETUP  == ueCb->srsCb.srsCfg.type))
   {

      /*     2. Present is SETUP(Eanble) state,  Recfg is modifying SETUP(Enable) 

             2.1 Delete UE from the cqiList 
             2.2 Set next occurance Transmission instance to "INVALID"
             2.3 Compute Next occurance Transmission instance 
             2.4 Placed ue in Transmission instance list.
             */ 
      if (ueCb->srsCb.nSrsTrIdx != RG_SCH_INVALID_IDX)
      {
         cmLListDelFrm(&cellCb->pCqiSrsSrLst[ueCb->srsCb.nSrsTrIdx].srsLst,
               &ueCb->srsCb.srsLstEnt);
         ueCb->srsCb.nSrsTrIdx = RG_SCH_INVALID_IDX;
      }

      rgSCHCfgSrsUeCfg(cellCb, ueCb, srsCfg);
   }
   else
   {
      /* 3. Present is SETUP(Enable) state,  Recfg is Release(Disable)*/ 
      rgSCHCfgSrsUeCfg(cellCb, ueCb, srsCfg);
   }
   /* ccpu00140578:: */
   ueCb->srsCb.srsRecpPrcsd = FALSE;

   RETVALUE(ROK);
}

/* @brief Handles  SR Re-configuration for a UE.
 *
 * @details
 *
 *     Function : rgSCHCfgSrUeReCfg 
 *
 *     Invoking Module Processing:
 *      - This shall be invoked by SCH_GOM at UE Re configuration. 
 *      It shall do the validations for the spec-defined values.
 *        
 *     Processing Steps:
 *     - For UE-specific SR  related re configuration, 
 *      - If SR is configured modified(First time Enabled),
 *        - Update UE with the configured values.
 *          - Compute and Update next occurance of SR Tranmission instance.
 *         - Update the  SR offset and SR perodicity information
 *         - Add Ue to cell->tIUeLstCp[n]  
 *      - If SR is configured modified(Changed offset or index ),
 *      - Delete UE from cell->tIUeLstCp[n] if present  
 *       - Update UE with the configured values.
 *       - Compute and Update next occurance of SR Tranmission instance.
 *       - Update the  SR offset and SR perodicity information
 *       - Add Ue to cell->tIUeLstCp[n]  
 *      - If SR is configured modified(disabled),
 *      - Delete UE from cell->tIUeLstCp[n] if present  
 *         - Update next occurance of SR Tranmission instance to "INVALID".
 *         - Update the  SR offset and SR perodicity information "INVALID"
 *      - else
 *        - ROK 
 *
 *
 *
 *     - Return ROK
 *
 *  @param[in]  RgSchCellCb  *cell
 *  @param[in]  RgSchUeCb    *ue
 *  @param[in]  RgrUeCfg     *ueCfg
 *
 *  @return  S16
 *      -# ROK
 *      -# RFAILED
 */
#ifdef ANSI
PUBLIC S16 rgSCHCfgSrUeReCfg
(
 RgSchCellCb    *cellCb, 
 RgSchUeCb      *ueCb, 
 RgrUeSrCfg     *srCfg
 )
#else
PUBLIC S16 rgSCHCfgSrUeReCfg(cellCb, ueCb, srCfg)
   RgSchCellCb   *cellCb; 
   RgSchUeCb     *ueCb; 
   RgrUeSrCfg    *srCfg;
#endif
{
   TRC3(rgSCHCfgSrUeReCfg);


   /* Fix : syed Incorrect check for SR RECFG */
   if((srCfg->type == RGR_SCH_SR_SETUP) &&
         (ueCb->srCb.srCfg.type != RGR_SCH_SR_SETUP))
   {
      /*
         1. Present is Release(Disable) state,  Recfg is allowing SETUP(Enable) 
         */
      rgSCHCfgSrUeCfg(cellCb, ueCb, srCfg);
   } 
   else if((srCfg->type == RGR_SCH_SR_SETUP) &&
         (ueCb->srCb.srCfg.type == RGR_SCH_SR_SETUP))
   {

      /*   2. Present is SETUP(Eanble) state,  Recfg is modifying SETUP(Enable) 

           2.1 Delete UE from the cqiList 
           2.2 Compute Next occurance Transmission instace 
           */      
      if(ueCb->srCb.nSrTrIdx != RG_SCH_INVALID_IDX )
      {
         cmLListDelFrm(&cellCb->pCqiSrsSrLst[ueCb->srCb.nSrTrIdx].srLst, 
               &ueCb->srCb.srLstEnt);
         ueCb->srCb.nSrTrIdx = RG_SCH_INVALID_IDX;
      }
      rgSCHCfgSrUeCfg(cellCb, ueCb, srCfg);

   }
   else
   {
      /* 3. Present is SETUP(Enable) state,  Recfg is Release(Disable) */
      rgSCHCfgSrUeCfg(cellCb, ueCb, srCfg);
   }

   RETVALUE(ROK);
}

/* @brief Handles  ACQI Re-configuration for a UE.
 *
 * @details
 *
 *     Function : rgSCHCfgAcqiUeReCfg 
 *
 *     Invoking Module Processing:
 *      - This shall be invoked by SCH_GOM at UE Re configuration. 
 *      It shall do the validations for the spec-defined values.
 *        
 *     Processing Steps:
 *     - For UE-specific ACQI  related re configuration, 
 *          - Check if the ACQI Mode has been changed from the
 *             existing Configuration.
 *     - If the configuration has been changed,
 *          - Call Aperiodic Config function to change the config
 *      - else
 *        - ROK 
 *
 *
 *     - Return ROK
 *
 *  @param[in]  RgSchCellCb  *cell
 *  @param[in]  RgSchUeCb    *ue
 *  @param[in]  RgrUeAprdDlCqiCfg   *acqiCfg
 *
 *  @return  S16
 *      -# ROK
 *      -# RFAILED
 */
#ifdef ANSI
PUBLIC S16 rgSCHCfgAcqiUeReCfg
(
 RgSchCellCb    *cellCb, 
 RgSchUeCb      *ueCb, 
 RgrUeAprdDlCqiCfg   *acqiCfg,
 CmLteUeCategory   ueCat
 )
#else
PUBLIC S16 rgSCHCfgAcqiUeReCfg(cellCb, ueCb, acqiCfg, ueCat)
   RgSchCellCb   *cellCb; 
   RgSchUeCb     *ueCb; 
   RgrUeAprdDlCqiCfg   *acqiCfg;
   CmLteUeCategory   ueCat;
#endif
{
   TRC3(rgSCHCfgAcqiUeReCfg);


   rgSCHCfgACqiUeCfg(cellCb, ueCb, acqiCfg, ueCat);

   RETVALUE(ROK);
}

/*****************************************************************
 *   UE PCQI, RI, SRS and SR Configuration  Delete                *
 *                                                                *
 *****************************************************************/

/* @brief Free Periodic CQI/PMI/RI, SRS and SR transmission instance 
 * related data structures of this UE from CellCb 
 *
 * @details
 *
 *     Function : rgSCHCfgPCqiSrsSrUeDel
 *
 *     Invoking Module Processing:
 *      - This shall be invoked by SCH_GOM at Ue deletion.
 *
 *     Processing Steps:
 *      - For SRS Transmission Instance
 *       - if (srsTxInst!= RG_INVALID)
 *        - Remove from the cellCb->tIUeLstCp[srsTxInst*3+2]
 *       - else
 *        - Nothing to do
 *      - For SR Transmission Instance
 *       - if (srTxInst!= RG_INVALID)
 *        - Remove from the cellCb->tIUeLstCp[srTxInst*3+1]
 *       - else
 *        - Nothing to do
 *      - For Periodic CQI/PMI RI  Transmission Instance
 *       - if (pCqiTxInst!= RG_INVALID)
 *        - Remove from the cellCb->tIUeLstCp[srTxInst*3+0]
 *       - else
 *        - Nothing to do
 *      - Return ROK
 *  @param[in]  RgSchCellCb  *cell
 *  @param[in]  RgSchUeCb    *ue
 *
 *  @return  S16
 *      -# ROK
 */
#ifdef ANSI
PUBLIC S16 rgSCHCfgPCqiSrsSrUeDel 
(
 RgSchCellCb      *cellCb,
 RgSchUeCb        *ueCb
 )
#else
PUBLIC S16 rgSCHCfgPCqiSrsSrUeDel(cellCb, ueCb)
   RgSchCellCb    *cellCb;
   RgSchUeCb      *ueCb;
#endif
{

   TRC3(rgSCHCfgPCqiSrsSrUeDel);




   /* Delete SRS Transmission Instance  */
   if (ueCb->srsCb.nSrsTrIdx != RG_SCH_INVALID_IDX)
   {
      cmLListDelFrm(&cellCb->pCqiSrsSrLst[ueCb->srsCb.nSrsTrIdx].srsLst,
            &ueCb->srsCb.srsLstEnt); 
      ueCb->srsCb.nSrsTrIdx = RG_SCH_INVALID_IDX;
   }

   /* Delete SR Transmission Instance  */
   if (ueCb->srCb.nSrTrIdx != RG_SCH_INVALID_IDX)
   {
      cmLListDelFrm(&cellCb->pCqiSrsSrLst[ueCb->srCb.nSrTrIdx].srLst,
            &ueCb->srCb.srLstEnt); 
      ueCb->srCb.nSrTrIdx = RG_SCH_INVALID_IDX;
   }


   /* Delete Periodic CQI/PMI  Transmission Instance  */
   if (ueCb->cqiCb.nCqiTrIdx != RG_SCH_INVALID_IDX)
   {
      cmLListDelFrm(&cellCb->pCqiSrsSrLst[ueCb->cqiCb.nCqiTrIdx].cqiLst, 
            &ueCb->cqiCb.cqiLstEnt); 
      ueCb->cqiCb.nCqiTrIdx = RG_SCH_INVALID_IDX;

      /* Delete Periodic  RI  Transmission Instance  */

      if (ueCb->cqiCb.nRiTrIdx != RG_SCH_INVALID_IDX)
      {
         cmLListDelFrm(&cellCb->pCqiSrsSrLst[ueCb->cqiCb.nRiTrIdx].riLst, 
               &ueCb->cqiCb.riLstEnt); 
         ueCb->cqiCb.nRiTrIdx = RG_SCH_INVALID_IDX;
      }
   }

   RETVALUE(ROK);
}


/* @brief Search the cfgIdx in given table and retrive periodicity & offset
 * @details
 *
 *     Function : rgSCHUtlGetCfgPerOff 
 *
 *     Invoking Module Processing:
 *      - This shall be invoked by Cfg Module 
 *
 *     Processing Steps:
 *          binary search for given entry in table
 *          find the periodicty, offset for a given config index from the table
 *      - Return ROK
 *  @param[in]  RgSchPerTbl tbl
 *  @param[in]  U16      cfgIdx
 *  @param[out] U16     *peri 
 *  @param[out] U16     *offset 
 *
 *  @return  S16
 *      -# ROK
 *
 */
#ifdef ANSI
PUBLIC S16 rgSCHUtlGetCfgPerOff
(
 RgSchPerTbl     tbl,
 U16             cfgIdx, 
 U16             *peri, 
 U16             *offset
 )
#else
PUBLIC S16 rgSCHUtlGetCfgPerOff ( tbl, cfgIdx, peri, offset)
   RgSchPerTbl   tbl;
   U16           cfgIdx;
   U16           *peri; 
   U16           *offset;
#endif
{
   U8     mid;
   U8     min = 0;
   U8     max = 0;
   CONSTANT RgSchUePCqiSrsSrCfgIdxTbl* table;
   TRC3(rgSCHUtlGetCfgPerOff);
  
   /* Added the function instead of the MACRO to get the 
    * periodicity table */
   table = rgSCHCfgUtlGetPcqiSrsSrRiTbl ( tbl,&min,&max );
   do{
      mid = (min+max)/2;
      if (( cfgIdx >= table[mid].min) &&
            ( cfgIdx <= table[mid].max))
      {
         *peri = table[mid].peri;
         *offset = cfgIdx - table[mid].offset;
         break;
      } 

      if ( cfgIdx > table[mid].min)
      {
         min = mid+1;
      }
      else
      {
         max = mid-1;
      }

   }while( min <= max );

   RETVALUE(ROK);
} 


/***********************************************************
 *
 *     Func : rgSCHCfgUtlFetchAcqiBitSz
 *
 *
 *     Desc : Fetch the CQI/PMI bits for a UE based on the mode and store them 
 *                for decoding. 
 *
 *     Ret  : Void
 *            ROK - RETVOID
 *
 *     Notes:
 *
 *     File :
 *
 **********************************************************/
#ifdef ANSI
PRIVATE Void rgSCHCfgUtlFetchAcqiBitSz
(
 RgSchUeCb         *ueCb, 
 U8                numTxAnt,
 U8*               cqiPmiSzR1,
 U8*               cqiPmiSzRn1
 )
#else
PRIVATE  Void rgSCHCfgUtlFetchAcqiBitSz(ueCb, numTxAnt, cqiPmiSzR1, cqiPmiSzRn1)
 RgSchUeCb         *ueCb;
 U8                numTxAnt;
 U8*               cqiPmiSzR1;
 U8*               cqiPmiSzRn1;
#endif
{
   U32 confRepMode;

   TRC3(rgSCHCfgUtlFetchAcqiBitSz);

   confRepMode = ueCb->dl.ueDlCqiCfg.aprdCqiCfg.aprdModeEnum;

   switch(confRepMode)
   {
      case RGR_APRD_CQI_MOD12:
         {
            if(numTxAnt == 2)
            {
               *cqiPmiSzR1 = 4 + 2*ueCb->acqiCb.N;
               *cqiPmiSzRn1 = 8+ ueCb->acqiCb.N;
            }
            else if(numTxAnt == 4)
            {
               *cqiPmiSzR1 = 4 + 4*ueCb->acqiCb.N;
               *cqiPmiSzRn1 = 8 + 4*ueCb->acqiCb.N;
            }
         }
         break;

      case RGR_APRD_CQI_MOD20:
         {
            *cqiPmiSzR1 = 6 + ueCb->acqiCb.L;
            *cqiPmiSzRn1 = 6  + ueCb->acqiCb.L;
         }
         break;

      case RGR_APRD_CQI_MOD22:
         {
            if(numTxAnt == 2)
            {
               *cqiPmiSzR1 = 10 + ueCb->acqiCb.L;
               *cqiPmiSzRn1 = 14 + ueCb->acqiCb.L;
            }
            else if(numTxAnt == 4)
            {
               *cqiPmiSzR1 = 14 + ueCb->acqiCb.L;
               *cqiPmiSzRn1 = 20 + ueCb->acqiCb.L;
            }
         }
         break;

      case RGR_APRD_CQI_MOD30:
         {
            *cqiPmiSzR1 = 4 + 2*ueCb->acqiCb.N;
            *cqiPmiSzRn1 = 4 + 2*ueCb->acqiCb.N;
         }
         break;

      case RGR_APRD_CQI_MOD31:
         {
            if(numTxAnt == 2)
            {
               *cqiPmiSzR1 = 6 + 2*ueCb->acqiCb.N;
               *cqiPmiSzRn1 = 9 + 4*ueCb->acqiCb.N;
            }
            else if(numTxAnt == 4)
            {
               *cqiPmiSzR1 = 8 + 2*ueCb->acqiCb.N;
               *cqiPmiSzRn1 = 12 + 4*ueCb->acqiCb.N;
            }
         }
         break;
      default:
         break;
   }
   RETVOID;
}
/* Added the function rgSCHCfgUtlGetPcqiCrsSrRiTbl to be used
 * instead of the MACRO RG_SCH_GET_PERIODICITY_TBL */
/***********************************************************
 *
 *     Func : rgSCHCfgUtlGetPcqiCrsSrRiTbl
 *
 *
 *     Desc :  Get the Srs Cqi Crs Ri Table 
 *
 *     Ret  : Void
 *            ROK - RETVOID
 *
 *     Notes:
 *
 *     File :
 *
 **********************************************************/

#ifdef ANSI
PRIVATE CONSTANT RgSchUePCqiSrsSrCfgIdxTbl *  rgSCHCfgUtlGetPcqiSrsSrRiTbl
(
   RgSchPerTbl tblType, 
   U8* min, 
   U8* max
)
#else
PRIVATE CONSTANT RgSchUePCqiSrsSrCfgIdxTbl *  rgSCHCfgUtlGetPcqiSrsSrRiTbl(tblType, min, max)
   RgSchPerTbl tblType; 
   U8* min; 
   U8* max;
#endif
{
    CONSTANT RgSchUePCqiSrsSrCfgIdxTbl * table;
    TRC3(rgSCHCfgUtlGetPcqiCrsSrRiTbl);
    
    switch (tblType)
    {

        case RG_SCH_FDD_PCQI_TBL:
        {
            table = rgSchUePCqiCfgIdxFddTbl;
            * min = 0;
            * max=RG_SCH_CQIPMI_CFGIDX_MAX_FDD;
            break;
        }
        case RG_SCH_TDD_PCQI_TBL:
        {
            table = rgSchUeCqiPmiCfgIdxTddTbl;
            * min = 0;
            * max=RG_SCH_CQIPMI_CFGIDX_MAX_TDD;
            break;
        }
        case RG_SCH_RI_TBL:
        {
            table = rgSchUeRiCfgIdxTbl;
            * min = 0;
            * max=RG_SCH_RI_CFGIDX_MAX;
            break;
        }
        case RG_SCH_FDD_SRS_TBL:
        {
            table = rgSchUeSrsCfgIdxFddTbl;
            * min = 0;
            * max=RG_SCH_SRS_ISRS_INDX_MAX_FDD;
            break;
        }
        case RG_SCH_TDD_SRS_TBL:
        {
            table = rgSchUeSrsCfgIdxTddTbl;
            * min = 0;
            * max=RG_SCH_SRS_ISRS_INDX_MAX_TDD;
            break;
        }
        case RG_SCH_SR_TBL:
        {
            table = rgSchUeSrCfgIdxTbl;
            * min = 0;
            * max=RG_SCH_ISR_INDX_MAX;
            break;
        }
        default:
        {
            table = (CONSTANT RgSchUePCqiSrsSrCfgIdxTbl *) 0;
            * min = 0;
            * max = 0;
            break;
        }

    }
    RETVALUE ( table );
}
/* #endif */
#endif /* TFU_UPGRADE */

/***********************************************************
 *
 *     Func : rgSCHCfgVldtDrxUeCfg
 *
 *
 *     Desc : Validates UE DRX Timers Configuration recieved from RRC.
 *
 *     Ret  : S16
 *            ROK - Success
 *            RFAILED - Failed
 *
 *     Notes:
 *
 *     File :
 *
 **********************************************************/
#ifdef ANSI
PRIVATE S16 rgSCHCfgVldtDrxUeCfg
(
RgSchCellCb       *cell,
RgrUeDrxCfg       *ueDrxCfg
)
#else
PRIVATE S16 rgSCHCfgVldtDrxUeCfg(cell, ueDrxCfg)
RgSchCellCb       *cell;
RgrUeDrxCfg       *ueDrxCfg;
#endif
{
   TRC2(rgSCHCfgVldtDrxUeCfg)


   if (ueDrxCfg->isDrxEnabled == FALSE)
   {
      RETVALUE(ROK);
   }

#ifdef LTEMAC_R9
   if ( ueDrxCfg->cqiMask.pres )
   {
      if ( ueDrxCfg->cqiMask.val != RGR_DRX_SETUP )
      {
#ifdef ALIGN_64BIT
         RLOG_ARG1(L_ERROR,DBG_CELLID,cell->cellId,"Invalid cqiMask configuration (%d)",
                  ueDrxCfg->cqiMask.val);
#else
         RLOG_ARG1(L_ERROR,DBG_CELLID,cell->cellId,"Invalid cqiMask configuration(%ld)",
                  ueDrxCfg->cqiMask.val);
#endif
         RETVALUE(RFAILED);
      }
   }
#endif /*LTEMAC_R9*/

   if ( rgSCHCfgVldtDrxOnDurCfg(ueDrxCfg->drxOnDurTmr) != ROK )
   {
      RLOG_ARG1(L_ERROR,DBG_CELLID,cell->cellId,"Invalid onDurTimer configuration(%d)",
               ueDrxCfg->drxOnDurTmr);
      RETVALUE(RFAILED);
   }

   if ( rgSCHCfgVldtDrxInActvCfg(ueDrxCfg->drxInactvTmr) != ROK )
   {
      RLOG_ARG1(L_ERROR,DBG_CELLID,cell->cellId,"Invalid Inactivity configuration(%d)",
               ueDrxCfg->drxInactvTmr);
      RETVALUE(RFAILED);
   }

   if ( rgSCHCfgVldtDrxReTxCfg(ueDrxCfg->drxRetxTmr) != ROK )
   {
      RLOG_ARG1(L_ERROR,DBG_CELLID,cell->cellId,"Invalid DrxReTX configuration(%d)",
               ueDrxCfg->drxRetxTmr);
      RETVALUE(RFAILED);
   }

   if ( rgSCHCfgVldtDrxLngCycCfg(ueDrxCfg->drxLongCycleOffst) != ROK )
   {
      RLOG_ARG0(L_ERROR,DBG_CELLID,cell->cellId,"Invalid LongCycle configuration");
      RETVALUE(RFAILED);
   }

   if ( ueDrxCfg->drxLongCycleOffst.longDrxCycle < ueDrxCfg->drxOnDurTmr ) 
   {
      RLOG_ARG0(L_ERROR,DBG_CELLID,cell->cellId,"Invalid combination of Long DRX Cycle " 
              " and onDuration timer values");
      RETVALUE(RFAILED);
   }

#ifdef LTE_TDD
   if( rgSCHCfgVldtTddDrxCycCfg(cell, ueDrxCfg->drxLongCycleOffst.longDrxCycle, 
       ueDrxCfg->drxOnDurTmr, ueDrxCfg->drxLongCycleOffst.drxStartOffst) != ROK)
   {
      RLOG_ARG0(L_ERROR,DBG_CELLID,cell->cellId,"Invalid combination of Long DRX Cycle " 
              " and onDuration timer values");
      RETVALUE(RFAILED);
   }
#endif
  
   if( TRUE == ueDrxCfg->drxShortDrx.pres )
   { 
      if ( ueDrxCfg->drxShortDrx.shortDrxCycle < ueDrxCfg->drxOnDurTmr )
      {
         RLOG_ARG0(L_ERROR,DBG_CELLID,cell->cellId,"Invalid combination of short DRX "
                 "Cycle and onDuration timer values");
         RETVALUE(RFAILED);
      }

      if ( (ueDrxCfg->drxLongCycleOffst.longDrxCycle %
               ueDrxCfg->drxShortDrx.shortDrxCycle) != 0)
      {
         RLOG_ARG0(L_ERROR,DBG_CELLID,cell->cellId," Long DRX cycle is not multiple of "
                 "short DRX cycle");
         RETVALUE(RFAILED);
      }

      if ( rgSCHCfgVldtDrxShrtCycCfg(ueDrxCfg->drxShortDrx) != ROK )
      {
         RLOG_ARG0(L_ERROR,DBG_CELLID,cell->cellId,"Invalid Short Cycle configuration");
         RETVALUE(RFAILED);
      }

#ifdef LTE_TDD
      if( rgSCHCfgVldtTddDrxCycCfg(cell, ueDrxCfg->drxShortDrx.shortDrxCycle, 
             ueDrxCfg->drxOnDurTmr, 
             ueDrxCfg->drxLongCycleOffst.drxStartOffst % 
             ueDrxCfg->drxShortDrx.shortDrxCycle) != ROK)
      { 
         RLOG_ARG0(L_ERROR,DBG_CELLID,cell->cellId,"Invalid combination of Long DRX Cycle " 
                 " and onDuration timer values");
         RETVALUE(RFAILED);
      }
#endif
   }
   
   RETVALUE(ROK);
}/*rgSCHCfgVldtDrxUeCfg*/
      
/***********************************************************
 *
 *     Func : rgSCHCfgVldtDrxOnDurCfg
 *
 *
 *     Desc : Validates UE DRX On Duration configuration 
 *            recieved from RRC.
 *
 *     Ret  : S16
 *            ROK - Success
 *            RFAILED - Failed
 *
 *     Notes:
 *
 *     File :
 *
 **********************************************************/
#ifdef ANSI
PRIVATE S16 rgSCHCfgVldtDrxOnDurCfg
(
U8      onDurTmr 
)
#else
PRIVATE S16 rgSCHCfgVldtDrxOnDurCfg(onDurTmr)
U8      onDurTmr;
#endif
{
   TRC2(rgSCHCfgVldtDrxOnDurCfg)

   switch ( onDurTmr )
   {
      case RGR_DRX_PRD_1PSF:
      case RGR_DRX_PRD_2PSF:
      case RGR_DRX_PRD_3PSF:
      case RGR_DRX_PRD_4PSF:
      case RGR_DRX_PRD_5PSF:
      case RGR_DRX_PRD_6PSF:
      case RGR_DRX_PRD_8PSF:
      case RGR_DRX_PRD_10PSF:
      case RGR_DRX_PRD_20PSF:
      case RGR_DRX_PRD_30PSF:
      case RGR_DRX_PRD_40PSF:
      case RGR_DRX_PRD_50PSF:
      case RGR_DRX_PRD_60PSF:
      case RGR_DRX_PRD_80PSF:
      case RGR_DRX_PRD_100PSF:
      case RGR_DRX_PRD_200PSF:
         break;

      default:
         {
            RETVALUE(RFAILED);
         }
   }/*switch(onDurTmr)*/

   RETVALUE(ROK);
}/*rgSCHCfgVldtOnDurCfg*/

/***********************************************************
 *
 *     Func : rgSCHCfgVldtDrxInActvCfg
 *
 *
 *     Desc : Validates UE DRX InActivity configuration 
 *            recieved from RRC.
 *
 *     Ret  : S16
 *            ROK - Success
 *            RFAILED - Failed
 *
 *     Notes:
 *
 *     File :
 *
 **********************************************************/
#ifdef ANSI
PRIVATE S16 rgSCHCfgVldtDrxInActvCfg
(
U16      inActvTmr
)
#else
PRIVATE S16 rgSCHCfgVldtDrxInActvCfg(inActvTmr)
U16      inActvTmr;
#endif
{
   TRC2(rgSCHCfgVldtDrxInActvCfg)

   switch ( inActvTmr )
   {
      case RGR_DRX_PRD_1PSF:
      case RGR_DRX_PRD_2PSF:
      case RGR_DRX_PRD_3PSF:
      case RGR_DRX_PRD_4PSF:
      case RGR_DRX_PRD_5PSF:
      case RGR_DRX_PRD_6PSF:
      case RGR_DRX_PRD_8PSF:
      case RGR_DRX_PRD_10PSF:
      case RGR_DRX_PRD_20PSF:
      case RGR_DRX_PRD_30PSF:
      case RGR_DRX_PRD_40PSF:
      case RGR_DRX_PRD_50PSF:
      case RGR_DRX_PRD_60PSF:
      case RGR_DRX_PRD_80PSF:
      case RGR_DRX_PRD_100PSF:
      case RGR_DRX_PRD_200PSF:
      case RGR_DRX_PRD_300PSF:
      case RGR_DRX_PRD_500PSF:
      case RGR_DRX_PRD_750PSF:
      case RGR_DRX_PRD_1280PSF:
      case RGR_DRX_PRD_1920PSF:
      case RGR_DRX_PRD_2560PSF:
         break;

      default:
         {
            RETVALUE(RFAILED);
         }
   }/*switch(InactvTmr)*/

   RETVALUE(ROK);
}/*rgSCHCfgVldtDrxInActvCfg*/

/***********************************************************
 *
 *     Func : rgSCHCfgVldtDrxReTxCfg
 *
 *
 *     Desc : Validates DRX ReTx timer  configuration 
 *            recieved from RRC.
 *
 *     Ret  : S16
 *            ROK - Success
 *            RFAILED - Failed
 *
 *     Notes:
 *
 *     File :
 *
 **********************************************************/
#ifdef ANSI
PRIVATE S16 rgSCHCfgVldtDrxReTxCfg
(
U8       reTxTmr
)
#else
PRIVATE S16 rgSCHCfgVldtDrxReTxCfg(reTxTmr)
U8       reTxTmr;
#endif
{
   TRC2(rgSCHCfgVldtDrxReTxCfg)

   switch ( reTxTmr )
   {
      case RGR_DRX_PRD_1PSF:
      case RGR_DRX_PRD_2PSF:
      case RGR_DRX_PRD_4PSF:
      case RGR_DRX_PRD_6PSF:
      case RGR_DRX_PRD_8PSF:
      case RGR_DRX_PRD_16PSF:
      case RGR_DRX_PRD_24PSF:
      case RGR_DRX_PRD_33PSF:
         break;

      default:
         {
            RETVALUE(RFAILED);
         }
   }/*switch(drxRetxTmr)*/

   RETVALUE(ROK);
}/*rgSCHCfgVldtDrxReTxCfg*/

/***********************************************************
 *
 *     Func : rgSCHCfgVldtDrxShrtCycCfg
 *
 *
 *     Desc : Validates DRX Short Cycle timer  configuration 
 *            recieved from RRC.
 *
 *     Ret  : S16
 *            ROK - Success
 *
 *            RFAILED - Failed
 *
 *     Notes:
 *
 *     File :
 *
 **********************************************************/
#ifdef ANSI
PRIVATE S16 rgSCHCfgVldtDrxLngCycCfg
(
RgrDrxLongCycleOffst       lngCycleOffst
)
#else
PRIVATE S16 rgSCHCfgVldtDrxLngCycCfg(lngCycleOffst)
RgrDrxLongCycleOffst       lngCycleOffst;
#endif
{
   TRC2(rgSCHCfgVldtDrxLngCycCfg)

   if ( rgSCHCfgVldtDrxLngCyclTmrs(lngCycleOffst.longDrxCycle) != ROK )
   {
      RETVALUE(RFAILED);
   }

   if ( lngCycleOffst.drxStartOffst >= lngCycleOffst.longDrxCycle ) 
   {
      RETVALUE(RFAILED);  
   }

   RETVALUE(ROK);
}/*rgSCHCfgVldtDrxLngCycCfg*/

/***********************************************************
 *
 *     Func : rgSCHCfgVldtDrxLngCyclTmrs
 *
 *
 *     Desc : Validates DRX Long Cycle timer values 
 *            recieved from RRC.
 *
 *     Ret  : S16
 *            ROK - Success
 *
 *            RFAILED - Failed
 *
 *     Notes:
 *
 *     File :
 *
 **********************************************************/
#ifdef ANSI
PRIVATE S16 rgSCHCfgVldtDrxLngCyclTmrs
(
U16      val 
)
#else
PRIVATE S16 rgSCHCfgVldtDrxLngCyclTmrs(val)
U16      val;
#endif
{
   TRC2(rgSCHCfgVldtDrxLngCyclTmrs)

   switch ( val )
   {
      case RGR_DRX_PRD_10SF:
      case RGR_DRX_PRD_20SF:
      case RGR_DRX_PRD_32SF:
      case RGR_DRX_PRD_40SF:
      case RGR_DRX_PRD_64SF:
      case RGR_DRX_PRD_80SF:
      case RGR_DRX_PRD_128SF:
      case RGR_DRX_PRD_160SF:
      case RGR_DRX_PRD_256SF:
      case RGR_DRX_PRD_320SF:
      case RGR_DRX_PRD_512SF:
      case RGR_DRX_PRD_640SF:
      case RGR_DRX_PRD_1024SF:
      case RGR_DRX_PRD_1280SF:
      case RGR_DRX_PRD_2048SF:
      case RGR_DRX_PRD_2560SF:
         break;

      default:
         {
            RETVALUE(RFAILED);
         }
   }/*switch(longDrxCycle)*/

   RETVALUE(ROK);
}/*rgSCHCfgVldtDrxLngCyclTmrs*/

/***********************************************************
 *
 *     Func : rgSCHCfgVldtDrxShrtCycCfg
 *
 *
 *     Desc : Validates DRX Short Cycle timer  configuration 
 *            recieved from RRC.
 *
 *     Ret  : S16
 *            ROK - Success
 *
 *            RFAILED - Failed
 *
 *     Notes:
 *
 *     File :
 *
 **********************************************************/
#ifdef ANSI
PRIVATE S16 rgSCHCfgVldtDrxShrtCycCfg
(
RgrDrxShortDrx      shrtCycCfg 
)
#else
PRIVATE S16 rgSCHCfgVldtDrxShrtCycCfg(shrtCycCfg)
RgrDrxShortDrx      shrtCycCfg;
#endif
{
   TRC2(rgSCHCfgVldtDrxShrtCycCfg)

   switch(shrtCycCfg.shortDrxCycle)
   {
      case RGR_DRX_PRD_2SF:
      case RGR_DRX_PRD_5SF:
      case RGR_DRX_PRD_8SF:
      case RGR_DRX_PRD_10SF:
      case RGR_DRX_PRD_16SF:
      case RGR_DRX_PRD_20SF:
      case RGR_DRX_PRD_32SF:
      case RGR_DRX_PRD_40SF:
      case RGR_DRX_PRD_64SF:
      case RGR_DRX_PRD_80SF:
      case RGR_DRX_PRD_128SF:
      case RGR_DRX_PRD_160SF:
      case RGR_DRX_PRD_256SF:
      case RGR_DRX_PRD_320SF:
      case RGR_DRX_PRD_640SF:
         break;

      default:
         {
            RETVALUE(RFAILED);
         }

   }/*switch(shortDrxCycle)*/

   if ( (shrtCycCfg.drxShortCycleTmr < RGR_DRX_SHRTCYCLE_MIN) ||
         (shrtCycCfg.drxShortCycleTmr > RGR_DRX_SHRTCYCLE_MAX)
      )
   {
      RETVALUE(RFAILED);
   }

   RETVALUE(ROK);
}

/**
 * @brief Handler for TA related UE Reconfiguration.
 *
 * @details
 *
 *     Function : rgSCHCfgUeTaRecfg
 *     
 *     This function shall fetch the TA timer related information into the
 *     respective ueCb from the UE configuration as provided by the 
 *     upper layers. 
 *           
 *  @param[in]  RgSchCellCb   *cell 
 *  @param[in]  RgSchUeCb     *ueCb 
 *  @param[in]  RgrUeCfg      *ueCfg 
 *  @param[out] RgSchErrInfo  *errInfo 
 *  @return     S16
 *      -# ROK 
 *      -# RFAILED 
 **/
#ifdef ANSI
PRIVATE Void rgSCHCfgUeTaRecfg
(
RgSchCellCb       *cell,
RgSchUeCb         *ueCb,
RgrUeRecfg        *ueCfg,
RgSchErrInfo      *err
)
#else
PRIVATE Void rgSCHCfgUeTaRecfg(cell, ueCb, ueCfg, err)
RgSchCellCb       *cell;
RgSchUeCb         *ueCb;
RgrUeRecfg        *ueCfg;
RgSchErrInfo      *err;
#endif
{
   TRC2(rgSCHCfgUeTaRecfg)

   UNUSED(err);

   /* Update the TA related information */
   
   if (ueCfg->ueTaTmrRecfg.pres)
   {
      /* Configuring taTmr with 30 deficit, to enable eNodeB sending
       * TA command before the expiry of TA at UE. Also considering for 
       * possible retx for this TA command */
      /* [ccpu00121813]-ADD-Added chk if tatmr val > 30 */
      if(ueCfg->ueTaTmrRecfg.taTmr > 30)
      {
         ueCb->dl.taCb.cfgTaTmr = ueCfg->ueTaTmrRecfg.taTmr - 30;
      }
       /* If TA Timer is running. Stop it and then start it*/
      if (ueCb->taTmr.tmrEvnt != TMR_NONE)
      {
         rgSCHTmrStopTmr(cell, ueCb->taTmr.tmrEvnt, ueCb);
         rgSCHTmrStartTmr(cell, ueCb, RG_SCH_TMR_TA, ueCb->dl.taCb.cfgTaTmr);
      }
      else
      {
         rgSCHTmrStartTmr(cell, ueCb, RG_SCH_TMR_TA, ueCb->dl.taCb.cfgTaTmr);
      }
   }
   RETVOID;
}  /* rgSCHCfgUeTaRecfg */

/* ccpu00117452 - MOD - Changed macro name from
   RGR_RRM_DLPWR_CNTRL to RGR_CQI_REPT */
#if ((defined (RGR_CQI_REPT)) && (defined (RGR_V2)))
/***********************************************************
 *
 *     Func : rgSCHCfgUeCqiReptReCfg
 *
 *
 *     Desc : Reconfiguration of PUSH N CQI Reporting 
 *
 *     Ret  : RFAILED in case of failure
 *            ROK if success
 *
 *     Notes:
 *
 *     File :
 *
 **********************************************************/
#ifdef ANSI
PRIVATE S16 rgSCHCfgUeCqiReptReCfg
(
RgSchCellCb    *cell,
RgSchUeCb      *ue,
RgrUeRecfg     *ueRecfg
)
#else
PRIVATE  S16 rgSCHCfgUeCqiReptReCfg(cell, ue, ueRecfg)
RgSchCellCb    *cell;
RgSchUeCb      *ue;
RgrUeRecfg     *ueRecfg;
#endif
{
   S16 retVal;
   TRC3(rgSCHCfgUeCqiReptReCfg)

   /* Check has it been disabled */
   if(ueRecfg->ueCqiReptCfg.numColltdCqiRept)
   {
    /* Check if we need to send CQI reports collagted so far and send if so */
      if(ue->schCqiInfo.cqiCount >=
              ueRecfg->ueCqiReptCfg.numColltdCqiRept)
      {
         RgrStaIndInfo *staInfo;
      /* if yes, Send StaInd to RRM */
         retVal = rgSCHUtlAllocSBuf (cell->instIdx,(Data**)&staInfo,
                  sizeof(RgrStaIndInfo));
         if(retVal != ROK)
         {
            RLOG_ARG1(L_ERROR,DBG_CELLID,cell->cellId,
               "Could not allocate memory for sending StaInd OLD CRNTI:%d",ueRecfg->oldCrnti);
            RETVALUE(retVal);
         }

      /* Fill StaInd for sending collated N CQI rpeort                        */
         rgSCHUtlFillSndStaInd(cell, ue, staInfo,
               ueRecfg->ueCqiReptCfg.numColltdCqiRept);
      }
   }
   else
   {
      ue->schCqiInfo.cqiCount = 0;
   }

   ue->cqiReptCfgInfo.numColltdCqiRept =
      ueRecfg->ueCqiReptCfg.numColltdCqiRept;
   RETVALUE(ROK);
} /* End of rgSCHCfgUeCqiReptReCfg */
#endif /* End of RGR_CQI_REPT */
/*This function Added Ue in ongoing L2 Meas*/
/*LTE_L2_MEAS_PHASE2*/
#ifdef LTE_L2_MEAS
PRIVATE S16 rgSchAddToL2Meas(RgSchCellCb *cellCb,RgSchDlLcCb *dlLc)
{
   CmLList             *lnk;
   U16       idx;
   RgSchL2MeasCb       *measCb = NULLP;
   lnk = cellCb->l2mList.first;

   while(lnk != NULLP)
   {
      /* Get the MeasCb : RgSchL2MeasCb */
      measCb = (RgSchL2MeasCb *)lnk->node;
      if (measCb->measReq.measType & LRG_L2MEAS_AVG_PRB_PER_QCI_DL)
      {
         for(idx = 0;idx < measCb->measReq.avgPrbQciDl.numQci;idx++)
         {
            if(measCb->measReq.avgPrbQciDl.qci[idx] == dlLc->qciCb->qci)
            {
               break; /*exit from for loop*/
            } 
         }   
         if(idx  == measCb->measReq.avgPrbQciDl.numQci)
         {
            measCb->measReq.avgPrbQciDl.qci[measCb->measReq.avgPrbQciDl.numQci++] = dlLc->qciCb->qci;
         }      
      }
      lnk = lnk->next;
   }/*End of while loop*/

   RETVALUE(ROK);
}
#endif
/**********************************************************************
 
         End of file:     gk_sch_cfg.c@@/main/2 - Sat Jul 30 02:21:35 2011
 
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
/main/1      ---       sd   1. LTE MAC 2.1 release
             rg001.201 sm   1. Added TTI indication towards 
                                        RGR user.
             rg002.201 sm   1. Added 0 as a valid value for number of TICKs
                               RRM.0 implies no ticks i.e. shutting off the 
                               feature.
             rg003.201 sm   1. TDD flag added  
             rg004.201 sd   1. ccpu00105914: PHR handling for MSG3
                       pk   2. ccpu00109921: PBCH allocation      
                       rt   3. Added for SI Enhancement
             rg006.201 sd  1. ccpu00112398: Added periodic BSR timer
             rg007.201 ap  1. Added support for MIMO
             rg008.201 sd  1. Removed dependncy on MIMO compile-time flag
             rg009.201 rsharon  1.[ccpu00112372]Changes for CR timer.
             rg009.201 gj 1. [ccpu00116647] - Added siId validation for lower limit
/main/2      ---        gvj  1. Updated for LTE MAC Rel 3.1           
           rg002.301 nudupi 1. [ccpu00119118 ] - Warning removal for cross 
                                   compilation
                            2. Added the function rgSCHCfgUtlGetPcqiSrsSrRiTbl
                               and removed the MACRO RG_SCH_GET_PERIODICITY_TBL
                               as a part of coding standard improvement.
                            3.ccpu00118351: seperate UL & DL deltas.
$SID$        ---        rt   1. LTE MAC 4.1 release
*********************************************************************91*/
