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
 
    Name:   LTE MAC SC1 scheduler
 
    Type:   C include file
 
    Desc:   Defines required by SC1 scheduler
 
    File:   rg_sch_cmn.x
 
    Sid:      rg_sch_cmn.x@@/main/2 - Sat Jul 30 02:22:02 2011
 
    Prg:    sm
 
**********************************************************************/
/** @file rg_sch_cmn.x
@brief This file contains data structures for the common module of the scheuler.
*/
 

/*
*     The defines in this file correspond to structures and variables
*     used by the following TRILLIUM software:
*
*     part no.             description
*     --------     ----------------------------------------------
*     1000372       LTE-MAC
*
*/

#ifndef __RGSCHCMNX__
#define __RGSCHCMNX__

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/*-------------------------------------*
 * Common Scheduler DataStructure START
 *-------------------------------------*/
typedef struct _rgDlSchdApis   RgDlSchdApis;
typedef struct _rgUlSchdApis   RgUlSchdApis;
typedef struct _rgDlfsSchdApis RgDlfsSchdApis;
typedef Void (*RgDlSchdInits[RGSCH_NUM_SCHEDULERS]) ARGS((RgDlSchdApis *apis));
typedef Void (*RgUlSchdInits[RGSCH_NUM_SCHEDULERS]) ARGS((RgUlSchdApis *apis));
typedef Void (*RgDlfsSchdInits[RGSCH_NUM_SCHEDULERS]) ARGS((RgDlfsSchdApis *apis));
typedef struct rgSchCmnDlRbAllocInfo RgSchCmnDlRbAllocInfo;
typedef struct rgSchCmnUeUlAlloc RgSchCmnUeUlAlloc;
typedef struct rgSchCmnUlRbAllocInfo RgSchCmnUlRbAllocInfo;

/**
  * @brief
  * Uplink Scheduler APIs.
  */
struct _rgUlSchdApis
{
   S16 (*rgSCHRgrUlUeCfg) ARGS((RgSchCellCb *cell, RgSchUeCb *ue, RgrUeCfg *cfg, 
                                                     RgSchErrInfo *err));
   S16 (*rgSCHRgrUlUeRecfg) ARGS((RgSchCellCb *cell, RgSchUeCb *ue, RgrUeRecfg *recfg, 
                                                     RgSchErrInfo *err));
   Void (*rgSCHFreeUlUe) ARGS((RgSchCellCb *cell, RgSchUeCb *ue));
   S16 (*rgSCHRgrUlCellCfg) ARGS((RgSchCellCb *cell, RgrCellCfg *cfg, 
                                                     RgSchErrInfo *err));
   S16 (*rgSCHRgrUlCellRecfg) ARGS((RgSchCellCb *cell, RgrCellRecfg *recfg, 
                                                     RgSchErrInfo *err));
   Void (*rgSCHFreeUlCell) ARGS((RgSchCellCb *cell));
   S16 (*rgSCHRgrUlLcgCfg) ARGS((RgSchCellCb *cell, RgSchUeCb *ue,
         RgSchLcgCb *lcg, RgrLcgCfg *cfg, RgSchErrInfo *errInfo));
   S16 (*rgSCHRgrUlLcCfg) ARGS((RgSchCellCb *cell, RgSchUeCb *ue,
         RgrLchCfg *cfg, RgSchErrInfo *errInfo));
   S16 (*rgSCHRgrUlLcgRecfg) ARGS((RgSchCellCb *cell, RgSchUeCb *ue,
         RgSchLcgCb *lcg, RgrLcgRecfg *recfg, RgSchErrInfo *errInfo));
   S16 (*rgSCHRgrUlLcRecfg) ARGS((RgSchCellCb *cell, RgSchUeCb *ue,
         RgrLchRecfg *recfg, RgSchErrInfo *errInfo));
   Void (*rgSCHFreeUlLcg) ARGS((RgSchCellCb *cell, RgSchUeCb *ue, RgSchLcgCb *lcg));
   S16 (*rgSCHRgrUlLchDel) ARGS((RgSchCellCb *cell, RgSchUeCb *ue, CmLteLcId lcId, U8 lcgId));
   Void (*rgSCHUlActvtUe) ARGS((RgSchCellCb *cell, RgSchUeCb *ue));
   Void (*rgSCHUpdBsrShort) ARGS((RgSchCellCb *cell, RgSchUeCb *ue, RgSchLcgCb *ulLcg, U8 bsr));
   Void (*rgSCHUpdBsrTrunc) ARGS((RgSchCellCb *cell, RgSchUeCb *ue, RgSchLcgCb *ulLcg, U8 bsr));
   Void (*rgSCHUpdBsrLong) ARGS((RgSchCellCb *cell, RgSchUeCb *ue, U8 bsArr[]));
   Void (*rgSCHContResUlGrant) ARGS((RgSchCellCb *cell, RgSchUeCb *ue));
   Void (*rgSCHSrRcvd) ARGS((RgSchCellCb *cell, RgSchUeCb *ue));
   Void (*rgSCHUlSched) ARGS((RgSchCellCb *cell, RgSchCmnUlRbAllocInfo
            *allocInfo));
   Void (*rgSCHUlCqiInd) ARGS(( RgSchCellCb *cell, RgSchUeCb *ue, TfuUlCqiRpt *ulCqiInfo));
   S16 (*rgSCHRgrUlLcgUpd) ARGS((RgSchCellCb *cell, RgSchUeCb *ue, RgInfUeDatInd *datInd));
   Void (*rgSCHUlUeRefresh) ARGS((RgSchCellCb *cell, RgSchUeCb *ue));
   Void (*rgSCHUlUeReset) ARGS((RgSchCellCb *cell, RgSchUeCb *ue));
   Void (*rgSCHUlAllocFnlz) ARGS((RgSchCellCb *cell, RgSchCmnUlRbAllocInfo
                                 *allocInfo));
   Void (*rgSCHUlInactvtUes) ARGS((RgSchCellCb *cell, CmLListCp *lst));
};

/**
  * @brief
  * DL Scheduler APIs.
  */
struct _rgDlSchdApis
{
   S16 (*rgSCHRgrDlUeCfg) ARGS((RgSchCellCb *cell, RgSchUeCb *ue, RgrUeCfg *cfg, 
                                                     RgSchErrInfo *err));
   S16 (*rgSCHRgrDlUeRecfg) ARGS((RgSchCellCb *cell, RgSchUeCb *ue, RgrUeRecfg *recfg, 
                                                     RgSchErrInfo *err));
   Void (*rgSCHFreeDlUe) ARGS((RgSchCellCb *cell, RgSchUeCb *ue));
   S16 (*rgSCHRgrDlCellCfg) ARGS((RgSchCellCb *cell, RgrCellCfg *cfg, 
                                                     RgSchErrInfo *err));
   S16 (*rgSCHRgrDlCellRecfg) ARGS((RgSchCellCb *cell, RgrCellRecfg *recfg, 
                                                     RgSchErrInfo *err));
   Void (*rgSCHFreeDlCell) ARGS((RgSchCellCb *cell));
   S16 (*rgSCHRgrDlLcCfg) ARGS((RgSchCellCb *cell, RgSchUeCb *ue,
         RgSchDlLcCb *dl, RgrLchCfg *cfg, 
         RgSchErrInfo *errInfo));
   S16 (*rgSCHRgrDlLcRecfg) ARGS((RgSchCellCb *cell, RgSchUeCb *ue,
         RgSchDlLcCb *dl, RgrLchRecfg *recfg,
         RgSchErrInfo *errInfo));
   Void (*rgSCHFreeDlLc) ARGS((RgSchCellCb *cell, RgSchUeCb *ue, RgSchDlLcCb *dlLc));
   Void (*rgSCHDlActvtUe) ARGS((RgSchCellCb *cell, RgSchUeCb *ue));
   Void (*rgSCHDlSched) ARGS((RgSchCellCb *cell, RgSchCmnDlRbAllocInfo *allocInfo));
   Void (*rgSCHDlDedBoUpd) ARGS((RgSchCellCb *cell, RgSchUeCb *ue, RgSchDlLcCb *svc));
   Void (*rgSCHDlProcAddToRetx) ARGS((RgSchCellCb *cell,RgSchDlHqProcCb *hqP));
   Void (*rgSCHDlCqiInd) ARGS((RgSchCellCb *cell, RgSchUeCb *ue, TfuDlCqiRpt *dlCqiInd));
   #ifdef TFU_UPGRADE
   Void (*rgSCHSrsInd) ARGS((RgSchCellCb *cell, RgSchUeCb *ue, TfuSrsRpt*srsInd));
   #endif 
   Void (*rgSCHDlAllocFnlz) ARGS((RgSchCellCb *cell, RgSchCmnDlRbAllocInfo
                                 *allocInfo));
   Void (*rgSCHDlUeRefresh) ARGS((RgSchCellCb *cell, RgSchUeCb *ue));
   Void (*rgSCHDlUeReset) ARGS((RgSchCellCb *cell, RgSchUeCb *ue));
   Void (*rgSCHDlInactvtUes) ARGS((RgSchCellCb *cell, CmLListCp *lst));
   S16 (*rgSCHDlUeHqEntInit) ARGS((RgSchCellCb *cell, RgSchUeCb *ue));
   S16 (*rgSCHDlUeHqEntDeInit) ARGS((RgSchCellCb *cell, RgSchDlHqEnt *hqE));
   Void (*rgSCHDlProcRmvFrmRetx) ARGS((RgSchCellCb *cell, RgSchUeCb *ue, RgSchDlHqProcCb *hqP));
   Void (*rgSCHDlTickForPdbTrkng ) ARGS((RgSchCellCb *cell));
   S16  (*rgSCHDlFillFlwCtrlInfo) ARGS((RgSchCellCb *cell, RgInfSfAlloc *sfAlloc));
};

/**
  * @brief
  * DLFS Scheduler APIs.
  */
struct _rgDlfsSchdApis
{
   S16 (*rgSCHDlfsCellCfg) ARGS((RgSchCellCb *cell, RgrCellCfg *cfg, 
                                                     RgSchErrInfo *err));
   S16 (*rgSCHDlfsCellRecfg) ARGS((RgSchCellCb *cell, RgrCellRecfg *recfg, 
                                                     RgSchErrInfo *err));
   Void (*rgSCHDlfsCellDel) ARGS((RgSchCellCb *cell));
   S16 (*rgSCHDlfsUeCfg) ARGS((RgSchCellCb *cell, RgSchUeCb *ue,
                               RgrUeCfg *cfg, RgSchErrInfo *err));
   S16 (*rgSCHDlfsUeRecfg) ARGS((RgSchCellCb *cell, RgSchUeCb *ue,
                                 RgrUeRecfg *recfg, RgSchErrInfo *err));
   Void (*rgSCHDlfsUeDel) ARGS((RgSchCellCb *cell, RgSchUeCb *ue));
   Void (*rgSCHDlfsDlCqiInd) ARGS((RgSchCellCb *cell, RgSchUeCb *ue,
                                  TfuDlCqiRpt *dlCqiRpt, 
                                  CmLteTimingInfo timingInfo));
   Void (*rgSCHDlfsReinitSf) ARGS((RgSchCellCb *cell, RgSchDlSf *dlSf));
   Void (*rgSCHDlfsAllocRb) ARGS((RgSchCellCb *cell, RgSchCmnDlRbAllocInfo
                                 *dlRbAllocInfo));
   /* Added for BCCH/PCCH handling */
   Void (*rgSCHDlfsBcchPcchAllocRb) ARGS((RgSchCellCb *cell, RgSchCmnDlRbAllocInfo *dlRbAllocInfo));
   Void (*rgSCHDlfsAddUeToLst) ARGS((CmLListCp *lCp, RgSchDlHqProcCb *hqP));
};

typedef enum rgSchCmnTpcAccVal
{
   RG_SCH_CMN_TPC_ACC_NEG_1DB = 0,
   RG_SCH_CMN_TPC_ACC_0DB     = 1,
   RG_SCH_CMN_TPC_ACC_1DB     = 2,
   RG_SCH_CMN_TPC_ACC_3DB     = 3
} RgSchCmnTpcAccVal;

typedef enum rgSchCmnTpcAbsVal
{
   RG_SCH_CMN_TPC_ABS_NEG_4DB = 0,
   RG_SCH_CMN_TPC_ABS_NEG_1DB = 1,
   RG_SCH_CMN_TPC_ABS_1DB     = 2,
   RG_SCH_CMN_TPC_ABS_4DB     = 3
} RgSchCmnTpcAbsVal;
/* Added changes of TFU_UPGRADE */
#ifdef TFU_UPGRADE
typedef enum rgSchCmnRank
{
   RG_SCH_CMN_RANK_1 = 1,
   RG_SCH_CMN_RANK_2 = 2,
   RG_SCH_CMN_RANK_3 = 3,
   RG_SCH_CMN_RANK_4 = 4
} RgSchCmnRank;
#endif

typedef struct rgSchCmnUlCqiInfo
{
   U8  qm;
   U16 eff;      /* Efficiency in terms of bits/RE */
} RgSchCmnUlCqiInfo;

EXTERN RgSchCmnUlCqiInfo rgSchCmnUlCqiTbl[RG_SCH_CMN_UL_NUM_CQI];
EXTERN S8 rgSchCmnDlCqiDiffOfst[8];
/* Added changes of TFU_UPGRADE */
#ifdef TFU_UPGRADE
EXTERN S8 rgSchCmnApUeSelDiffCqi[4];
EXTERN S8 rgSchCmnApEnbConfDiffCqi[4];
#endif 


EXTERN U8 rgSchCmnUlCqiToTbsTbl[RG_SCH_CMN_MAX_CP][RG_SCH_CMN_UL_NUM_CQI];

#if (LTEMAC_SPS & LTE_TDD)
/* subframe offset values to be used when twoIntervalsConfig is enabled in UL
 * SPS for a UE */
typedef S8 RgSchTddSfOffTbl[RGSCH_MAX_TDD_UL_DL_CFG][RGSCH_NUM_SUB_FRAMES];
EXTERN RgSchTddSfOffTbl rgSchTddSfOffTbl;

#endif /* LTEMAC_SPS & LTE_TDD */

/*--------------------------*
 * SPS specific declarations 
 *---------------------------*/
#ifdef LTEMAC_SPS

/**
  * @brief
  * Downlink SPS scheduling information per UE
  */
typedef struct  rgSchCmnSpsDlUeSchdInfo
{
   U8              scaledCqi;    /*!< Assumed value of CQI for transmission */
   U16             actvSfTblIdx; /*!< Index into cell-wide DL SPS sub-frame 
                                      table during activation */
   CmLteTimingInfo schdKey;      /*!< Key into the list of DL SPS active 
                                      UEs: next DL SPS ocassion */
   RgSchDlRbAlloc  spsAllocInfo; /*!< Allocation information for an SPS active
                                      UE */
   U8              allocN1PucchIdx;  /*!< Index value in UE's n1Pucch array 
                                      of the allocated n1Pucch */
   //Bool            pdcchPndng;      /*!< Indicates if the activaton/
    //                                     reactivation PDCCH needs to be sent 
    //                                     for this allocation */
} RgSchCmnSpsDlUeSchdInfo;

/**
  * @brief
  * Downlink stats information for SPS per UE
  */
typedef struct rgSchCmnDlUeSpsStatInfo
{
   U32      numSchedSPSRnti;  /*!< Number of SPS occasions sched using SPS RNTI*/
   U32      totalSPSSchedOcc; /*!< Number of SPS occasions sched 
                                     using SPS RNTI + CRNTI*/
   U32      numSpsReactv;     /*!< Number of Reactivations */
   U32      numSpsActv;       /*!< Number of activations */
   U32      numSpsRel;        /*!< Number of Deactivations */
}RgSchCmnDlUeSpsStatInfo;

/**
  * @brief
  * Downlink information for SPS per UE
  */
typedef struct rgSchCmnDlUeSpsInfo
{
   CmLteTimingInfo prevDlBoUpdTm; /*!< BO updation interval*/
   CmLList        zeroBOSvcUesEnt;    /*!< Linked list entity for zeroBOSvcUes lst */ 
   CmLList        actvUeLstEnt; /*!< Linked List entry for DL SPS
                                       active UE list*/
   CmLList        pndngUeLstEnt;/*!< Linked List entry for UE list with
                                      pending SPS action:
                                      activation/reactivation/release */
  /* Added handling to retrnasmit RelPDCCH in case no
                     feedback is received */
   CmLList        wtngForRelFdbkUeEnt;/*!< Linked list entry for UE who
                                          have a feedback pending for
                                          Release PDCCH */
   RgSchDlLcCb    *spsSvc;       /*!< Pointer to the SPS service of the 
                                      UE */
   CmLListCp      *spsList; /*!< Pointer to the SPS list of which
                                      UE is a part */
   U32             measGapMask[RG_SCH_CMN_SPS_DL_MEASGAP_32BITMASK_SIZE]; 
                                 /*!< Indicates the DL sub-frames with 
                                      ongoing measurement gap */
   U16            n1PucchIdx[RG_SCH_CMN_SPS_DL_MAX_N1PUCCH_IDX_PER_UE]; 
                                 /*!< N1Pucch indices configured for the UE */
   U8             actionPndng;   /*!< Indicates the action pending on the UE
                                      activation/re-activation/release */
   U8             dlSpsStatus;     /*!< Indicates the current status of DL SPS */
   U8             prdIdx;        /*!< DL SPS periodicity index for the
                                      configured peridicity */
   RgSchCmnSpsDlUeSchdInfo dlSpsUeSchdInfo; /*!< Scheduled info for DL SPS 
                                                 active UE */  
   Bool           isRelPdcchSent; /*!< Indicates if release PDCCH is sent for
                                       this UE. For TDD, Used while sending DAI
                                       in DCI formats 0/1/1A/1B/1D/2/2A. 
                                       For FDD, used to not repeat relPdcch 
                                       till the feddback is recieved */
   U8             numRelPdcchSent; /*!< Number of times RelPdcch has been sent. */

   RgSchCmnDlUeSpsStatInfo statInfo; /*!< SPS Metric Info */
   U8             dynSchedCount; /*!< To track num of consecutive times SPS BO
                                                            is sched dynamically */
   U8             reducedBoCount; /*!< To track num of consecutive times BO
                                                            is lesser than SPS BO */                                                         
   U32             maxChgdBo;    /* !< The Maximum of BO which is different from the 
                                                            BO for which SPS has been activated */
   U32            spsSchedBo;    /* !< BO for which SPS is activated */
   Bool           isDynSched;    /* !< BO is dynamically scheduled */
} RgSchCmnDlUeSpsInfo;

/**
  * @brief
  * Downlink information for SPS per Cell
  */
typedef struct rgSchCmnSpsDlSf
{
  U32 rbsAlloc;         /*!< Allocated BW for this subframe (in actual number of
                             RBs) */ 
  RgSchDlSfAllocInfo spsAllocInfo; /*!< Allocation information for SPS BW */
  U32 n1PucchMask[RG_SCH_CMN_SPS_DL_N1PUCCH_32BITMASK_SIZE]; 
                        /*!< N1Pucch allocation mask per Sub-frame */
  U8  numDlSpsActiveUes;   /*!<  number of DL SPS UEs that 
                                                                  have been activated */
} RgSchCmnSpsDlSf;

/**
  * @brief
  * SPS N1Pucch Database for the cell
  */
typedef struct rgSchCmnSpsDlN1Pucch RgSchCmnSpsDlN1Pucch;
struct rgSchCmnSpsDlN1Pucch
{
 U16   idx;        /*!< Index in the n1PucchLst */
 U16   n1PucchVal; /*!< Pucch Value corresponding to the index */
 U32   numUes;     /*!< Count of UEs with this N1Pucch value configured */
 U16   next;       /*!< Next available index */
};

/**
  * @brief
  * SPS N1Pucch Database for the cell
  */
typedef struct rgSchCmnSpsDlN1PucchDb
{
  U16                   numFreeN1Pucch; /*!< Number of free n1Pucch values */
  U16                   numInUseN1Pucch; /*!< Number of inUse n1Pucch values
                                          */
  RgSchCmnSpsDlN1Pucch  *freeN1PucchStart; /*!< Start for free n1Pucch list */
  RgSchCmnSpsDlN1Pucch  *inUseN1PucchStart;/*!< Start for in-use n1Pucch list
                                           */
  RgSchCmnSpsDlN1Pucch   n1PucchLst[RG_SCH_SPS_DL_MAX_N1PUCCH_PER_SF];
                                            /*!< List of cell wide n1Pucch
                                                 values*/
} RgSchCmnSpsDlN1PucchDb;

/**
  * @brief
  * Downlink information for SPS per Cell
  */
typedef struct rgSchCmnDlCellSpsInfo
{
  CmLListCp       zeroBOSvcUes;    /*!< List of SPS services which 
                                       are not sched at SPS Occasion due 
                                       to zero BO*//* REVANTH_SPS_FIX */
  CmLListCp       toBeSchdSvcs;    /*!< List of SPS services to be scheduled */
  CmLListCp       retxHqProcs;     /*!< List of SPS HARQ procs for 
                                        re-transmission: all the HARQ procs 
                                        with isSpsSvcSchd = TRUE shall be
                                        part of this list */
  CmLListCp       actvDlSpsUeLsts[RG_SCH_CMN_SPS_MAX_PRD];    
                                 /*!< Array of list of UE control blocks with
                                      DL SPS activated: index - next time of
                                      transmission */
  CmLListCp       toBeActvtdUes; /*!< List of DL SPS UEs with pending 
                                      activation/re-activation */
  CmLListCp       toBeRelUes;    /*!< List of DL SPS enabled UEs with release
                                      pending */
 /* Added handling when no feedback is received
                    for the Release PDCCH sent
  */
  CmLListCp       wtngForRelFdbkUeLst[RGSCH_NUM_SUB_FRAMES]; /*!< List of DL SPS
                                                             enabled UEs waiting
                                                             for feedback for 
                                                             Release PDCCH sent
                                                            */
  U16             spsPrdLcmVal;  /*!< LCM value for all configured
                                      SPS periodicities: maxVal = 640 for FDD
                                      and (640 * 3) for TDD */
  U8              lcmIdx;        /*!< Index value for computed LCM */
  RgSchCmnSpsDlSf *spsSfTbl;     /*!< DL sub-frame information for the cell*/
  RgSchCmnSpsDlN1PucchDb n1PucchDb; /*!< Database of configured n1Pucch values
                                     */  
} RgSchCmnDlCellSpsInfo;

/**
  * @brief
  * Uplink SPS scheduling information per UE
  */
typedef struct  rgSchCmnSpsUlUeSchdInfo
{
   U32             allocBwMask[RGSCH_SPS_ULBW_MASK_LEN]; /*!< Bitmask indicating the allocation info 
                                      for the UE. Bit set at position 'x'
                                      indicates subband 'x' is allocated */
   U8              scaledCqi;    /*!< Assumed value of CQI for transmission */
   U16             actvSfLstIdx; /*!< Index into cell-wide UL SPS sub-frame 
                                      table during activation */
   CmLteTimingInfo ulSpsActvOcc; /*!< Time at which SPS is activated/reactivated
                                      for the UE */
   CmLteTimingInfo crntUlSpsOcc;/*!< Key into the list of UL SPS active 
                                      UEs: next UL SPS ocassion */
   RgSchUlAlloc    allocInfo;    /*!< Allocation information of the UE */
   RgSchUlAlloc    *crntAlloc;   /*!< Allocation pointer in the subframe */
   RgSchUlSf       *crntAllocSf;          /*!< sf in which crntAlloc was made */
} RgSchCmnSpsUlUeSchdInfo;

/**
  * @brief
  * Uplink information for SPS per UE
  */
typedef struct rgSchCmnUlUeSpsInfo
{
   CmLList                 actvUeLstEnt; /*!< Linked List entry for UL SPS
                                              active UE list*/
   CmLList                 pndngUeLstEnt;/*!< Linked List entry for UE list with
                                              pending SPS action:
                                              activation/reactivation/release */
   Bool                    isUlSpsActv;   /*!< Indicates if UE's UL SPS is 
                                              active currently */
   U8                      measGapMask[RGSCH_SPS_MG_MASK_LEN];  /*!< Indicates the UL sub-frames with 
                                               ongoing measurement gap. Mask is
                                               of size (80/8) */
   U8                      state;         /*!< State of the UE: can be one of
                                               ACTV_REACTV_SENT (pdcch for 
                                               (re)activation sent),REL_SENT, 
                                               ULSPS_ACTV */
   Bool                    pdcchToRetx;   /*!< set to TRUE if DTX occurs at the
                                               (re)activated SPS occasion */
   RgSchCmnSpsUlUeSchdInfo ulSpsSchdInfo; /*!< Scheduled info for UL SPS 
                                               active UE */

   U8                      spsOccIdx;         /*!< N value for nxt sps occasion */

   U32                     packetSize;       /*!< Packet size for SPS - SPS allocation*/
   U32                     spsSduSize;       /*!< SDU Size recvd on SPS Occasion*/
   U32                     spsGrantSize;     /*!< Grant size for SPS allocation */

   CmLteTimingInfo         lastSpsDatRecvdTime; /*!< timing info on which the  
                                                     SPS data is recieved*/
   CmLListCp               *spsList; /*!< Pointer to the SPS list of which
                                      UE is a part */
   U32                     actPdcchCrcFailCount; /*!< Num of consecutive CRC fails for 
                                                   SPS Activation PDCCH */
   U32                     crcFailCntSpsOcc;     /*!< Consecutive CRC fail for SPS Occasion
                                         TX */
   U32                     relPdcchSntCnt;     /*!< Num ber Rel PDCCH sent to UE*/
#ifdef RGSCH_SPS_STATS
   U32                     crcFailStats;
   U32                     crcPassStats;
   U32                     totalBsr;
   U32                     numBsrRpt;
#endif
} RgSchCmnUlUeSpsInfo;

/**
  * @brief
  *  Information per uplink SPS allocation
  */
typedef struct rgSchCmnSpsUlAlloc
{
   U8             sbStart;        /*!< Starting subband of the alloc */
   U8             numSb;          /*!< Num of subbands in the alloc */
} RgSchCmnSpsUlAlloc;

/**
  * @brief
  * Uplink information for SPS per subframe
  */
typedef struct rgSchCmnSpsUlSf
{
   U32            ulBwBitMask[RGSCH_SPS_ULBW_MASK_LEN];    /*!< Bitmask indicating the alloc/hole info 
                                         for SPS BW. Bit set at position 'x'
                                         indicates subband 'x' is occupied */
   U8             maskLen;         /*!< Length of ulBwBitMask based on numSb */
   U8             numUlSpsActiveUes;    /*!< Number of UL SPS Active UEs in this Subframe */
   RgSchCmnSpsUlAlloc allocInfo;   /*!< Info per SPS Allocation - Used to mark
                                        previous allocations in a subframe */
} RgSchCmnSpsUlSf;

/**
  * @brief
  * Uplink information for SPS per Cell
  */
typedef struct rgSchCmnUlCellSpsInfo
{
   U8              spsSbStart;      /*!< Starting subband of SPS BW */
   U8              numSpsSb;        /*!< number of subbands for SPS */
   U16             spsPrdLcmVal;    /*!< LCM value for all configured UL
                                         SPS periodicities:maxVal = 640 for FDD
                                         and (640 * 3) for TDD */ 
   RgSchCmnSpsUlSf *spsSfLst;       /*!< UL subframe information for the cell*/
   CmLListCp       actvUlSpsUeLsts[RG_SCH_CMN_SPS_MAX_PRD];    
                                    /*!< Array of list of UeCbs with
                                         UL SPS activated: index - next time of
                                         transmission */
   CmLListCp       toBeActvtdUeLst; /*!< List of ULSPS enabled UEs with pending
                                         activation */
   CmLListCp       toBeRelUeLst;    /*!< List of ULSPS enabled UEs with release
                                         pending */
} RgSchCmnUlCellSpsInfo;


#endif
/*--------------------------*
 * SPS specific declarations End 
 *---------------------------*/
/**
  * @brief
  * Scheduler uplink scheduling parameters related to random access.
  */
typedef struct rgSchCmnUlCellRa
{
   U8  prmblANumSb; /*!< Number of msg3 RBs to allocate for preamble A */
   U8  prmblAIMcs;  /*!< Imcs for msg3 when preamble A was used */
   U8  prmblBNumSb; /*!< Number of msg3 RBs to allocate for preamble B */
   U8  prmblBIMcs;  /*!< Imcs for msg3 when preamble B was used */
} RgSchCmnUlCellRa;

typedef struct rgSchCmnCellClcITbs
{
   U8       iTbs2Rbs; /*!< iTbs value for 2 Rbs precomputed at cell cfg */ 
   U8       iTbs3Rbs; /*!< iTbs value for 3 Rbs precomputed at cell cfg */ 
}RgSchCmnCellClcITbs;

typedef struct rgSchCmnDlCell
{
   Bool                isDlFreqSel;  /*!< Bool indicating if cell is frequency 
                                          selective or not */
   U8                  maxUeNewTxPerTti; /*!< Max UEs to be considered for New Tx Alloc in DL */
   U8                  numRaSubFrms; /*!< Number of frames of RA transmission */
   U8                  iTbsCap;      /*!< Max value DL iTbs capped to */
   U16                 nCce;         /*!< Number of CCEs computed based on CFI */
   U8                  maxDlBwPerUe; /*!< Max DL B/W per UE */
   U8                  maxDlRetxBw;  /*!< Max DL retx B/W, as part of 256 */
   U8                  maxUePerDlSf;  /*!< Max UE to be considered for DL scheduling
                                       *   in a TTI */
   /*[ccpu00138609]-ADD- max Msg4/ DL CCCH UE configuration */                                    
   U8                  maxCcchPerDlSf; /*!< Max Msg4/DL CCCH UE sched in Dlsf */                                     
   U8                  msg4TxDelay;  /*!<  Max estimated time for HARQ tx
                                           of msg4 based on the Harq  RTT and 
                                           max Harq retries for msg4 */
   RgSchCmnCellClcITbs cmnChITbs;  /*!< iTbs value for 2 Rbs precomputed at cell cfg */ 
   CmLteAggrLvl        cmnChAggrLvl; /*!< Precomputed aggregation level for common channel */ 
   U8                  ccchCqi;      /*!< Default Cqi to be used for Msg4 and UE */
   CmLListCp           msg4RetxLst;  /*!< Queue to hold Msg4 procs for retransmission */
   /* Changes for CR timer */
#ifdef RGR_V1
   CmLListCp           ccchSduRetxLst;  /*!< Queue to hold CCCH SDU procs for retransmission */
#endif
   Void                *cqiToTbsTbl[RGSCH_MAX_NUM_LYR_PERCW][RG_SCH_CMN_MAX_CFI]; 
   /* cqi to Tbs tables for each 1 and 2 layer TbSz table */
                                        /*!< CQI to efficiency translation */
   Void                *cqiToEffTbl[RGSCH_MAX_NUM_LYR_PERCW][RG_SCH_CMN_MAX_CFI]; 
   U8                  newCfi;          /*!< New CFI value */
   U8                  currCfi;         /*!< Current CFI value */
                                        
   U16                 noResPerRb[RG_SCH_CMN_MAX_CFI]; /*!< Num REs per RB */   
   CmLteTimingInfo     time;         /*!< Timing info for current allocation */
   Void                *schSpfc;     /*!< Scheduler Specific Cell DL dereferencing */
   Void                *dlfsCell;    /*!< DLFS specific information per cell */
   CmLListCp           taLst;        /*!< TA queues, holds the UEs for which TA 
                                        has to be scheduled */
#ifdef LTEMAC_SPS
   RgSchCmnDlCellSpsInfo dlSpsInfo; /*!< DL SPS info for the cell */
#endif
   /* Member to store no. of Bits per RB */
   U32                 bitsPerRb;   /*!< Bits per RB calculated from
                                      BcchPcchRaRsp Code rate configured through
                                      RGR */
#ifdef LTE_TDD                                      
   U16                 numReDwPts[RG_SCH_CMN_MAX_CFI-1];  /*!< Num of RE in DwPTS RB */
   U8                  splSfCfg;    /*!<Stores the special subframe cfg */  
#endif   

   /* ccpu00132314-ADD-Tx power offsets for Common PDSCH transmissions */                                   
   U16                 bcchTxPwrOffset; /*!< Tx Pwr Offset for BCCH tx on PDSCH.
                                             Offset to the reference signal 
                                             power. Value: 0 -> 10000, 
                                             representing -6 dB to 4 dB in 0.001
                                             dB steps */                                    
   U16                 pcchTxPwrOffset; /*!< Tx Pwr Offset for PCCH tx.
                                             Offset to the reference signal 
                                             power. Value: 0 -> 10000, 
                                             representing -6 dB to 4 dB in 0.001
                                             dB steps */                                    
   U16                 rarTxPwrOffset; /*!< Tx Pwr Offset for RAR tx.
                                             Offset to the reference signal 
                                             power. Value: 0 -> 10000, 
                                             representing -6 dB to 4 dB in 0.001
                                             dB steps */                                    
  /* ccpu00138898 - Added Tx pwr offset for PHICH Tx*/
   U16                 phichTxPwrOffset; /*!< Tx Pwr Offset for PHICH tx.
                                             Offset to the reference signal 
                                             power. Value: 0 -> 10000, 
                                             representing -6 dB to 4 dB in 0.001
                                             dB steps */                                    
   U32          ncsgPrbCnt; /*!< Cumulative sum of PDSCH PRBs assigned to non-Csg UEs */
   U32          totPrbCnt; /*!< Cumulative sum of PDSCH PRBs assigned to all UEs */
   RgrUeDlPwrCntrlPaCfg msg4pAVal;      /*!< Default value (Enum) of PA that is 
                                          used by Scheduler for msg4 */
} RgSchCmnDlCell;

/**
  @brief Information related to TPC-PUCCH-RNTI/TPC-PUSCH-RNTI. */
typedef struct rgSchCmnTpcRntiCb
{
   CmLteRnti  tpcRnti;       /*!< TPC-PUCCH-RNTI/TPC-PUSCH-RNTI*/
   Bool       isFmt3a;       /*!< DCI format type: 3/3A */
   CmLListCp  toBeSchdUes;   /*!< List of UEs requiring power adjustment 
                               for this TPC-RNTI */
   CmLListCp  cfgdUes;         /*!< List of UEs */
   CmLList    schdLnk;     /*!< Link to the list of TPC RNTIs to be
                                  scheduled */
} RgSchCmnTpcRntiCb;

/**
  @brief Uplink Power control related information per cell. */
typedef struct rgSchCmnUlPwrCb 
{
   U8                tpcPucchRntiCnt;/*!< Count of TPC-PUCCH-RNTIs for the cell */
   RgSchCmnTpcRntiCb tpcPucchRntiLst[RG_SCH_CMN_MAX_NUM_TPC_PUCCH_RNTI];
                                     /*!< List of TPC-PUCCH-RNTIs */
   U8                tpcPuschRntiCnt;/*!< Count of TPC-PUSCH-RNTIs for the cell */
   RgSchCmnTpcRntiCb tpcPuschRntiLst[RG_SCH_CMN_MAX_NUM_TPC_PUSCH_RNTI];
                                     /*!< List of TPC-PUSCH-RNTIs */
   CmLListCp         pucchGrpPwr;    /*!< List of TPC-PUCCH-RNTIs for PUCCH group
                                       power control: 'RgSchCmnTpcRntiCb' */
   CmLListCp         puschGrpPwr;    /*!< List of  TPC-PUSCH-RNTIs for PUSCH group 
                                       power control: 'RgSchCmnTpcRntiCb' */
   S8                pMax;           /*!< Max allowed uplink power in cell */
   U8                trgUlCqi;       /*!< Default target CQI */
} RgSchCmnUlPwrCb;

/**
  * @brief
  * Cell specific uplink scheduling information for Scheduler type 1.
  */
typedef struct rgSchCmnUlCell
{
   U8               maxUeNewTxPerTti; /*!< Max UEs to be considered for New Tx Alloc in UL */
   /* Added new variable maxUlBwPerUe */
   U8               maxUlBwPerUe;      /*!< Max UL BW per UE */
   U8               maxSbPerUe;      /*!< Max subbands per UE */
   U8               dfltUlCqi;       /*!< Default uplink CQI assumed intitially */
   U8               max16qamCqi;     /*!< Highest CQI supporting 16 QAM */
   U8               maxUlSpsCqi;     /*!< Highest CQI supporting 16 QAM */
   U8               iTbsCap;         /*!< Max value UL iTbs capped to */
   U8               sbSize;          /*!< Subband size */
   U8               dmrsArrSize;     /*!< DMRS array size */
   U8               *dmrsArr;        /*!< DMRS array */
   RgSchCmnUlCellRa ra;          /*!< RA related info */
   U8               idx;             /*!< Current subframe - maps to HARQ process ID */
   U8               schdIdx;         /*!< Subframe to schedule for */
   U8               schdHqProcIdx;   /*!< Proc to schedule for */
   U8               msg3SchdIdx;     /*!< Subframe to schedule for msg3 */
   U8               msg3SchdHqProcIdx;/*!< Proc to schedule for */
   U8               rcpReqIdx;       /*!< Subframe to send reception req for */
   /* ccpu00130688 -MOD- for config-0 changes */
   U8               hqFdbkIdx[2];    /*!< In FDD only Idx 0 is used. 
                                          In TDD n+k value is updated at idx 0.
                                          For TDD Cfg 0 both indices are used */ 
   U8               reTxIdx[2];      /*!< Retransmission Index corresponding to 
                                          the hqFdbkIdx */
#ifdef LTEMAC_SPS
   U8               spsUlRsrvIdx;     /*!< Subframe to reserve UL SPS cfgd grant */
   U8               spsUlRsrvHqProcIdx;/*!< Proc for the cfgd UL SPS grant */
#endif
   CmLteTimingInfo  schdTime;
#ifdef LTE_TDD
   U8               numUlSubfrms;    /*!< Number of UL subframes */
   RgSchUlSf        *ulSfArr;        /*!< no msg3 alloc info here */
#else
   RgSchUlSf        ulSfArr[RG_SCH_CMN_UL_NUM_SF];      /*!< no msg3 alloc info here */
#endif
   Void             *schSpfc;        /*!< Scheduler Specific Cell UL dereferencing */
   RgSchCmnUlPwrCb  ulPwrCb;     /*!< Uplink power control block */
   U8               ulNumRePerRb;  /*!< Number of REs per RB in UL */
   /* Added support for non-adaptive retransmission in uplink */
   U8               maxAllocPerUlSf; /*!< Max Allocations in a given SF */
#ifdef RGR_V1
/* Added a param to limit msg3 allocations */
   U8               maxMsg3PerUlSf; /*!< Max msg3 alocs in a given SF */
#endif

#ifdef LTEMAC_SPS
   RgSchCmnUlCellSpsInfo ulSpsInfo; /*!< UL SPS info for the cell */
   U16                   schdTti;   /*< 0..1023, corresponding to scheduling time,
                                     * can theoretically used for non-SPS
                                     * purposes as well */
#endif
   U32          ncsgPrbCnt; /*!< Cumulative sum of PDSCH PRBs assigned to non-Csg UEs */
   U32          totPrbCnt; /*!< Cumulative sum of PDSCH PRBs assigned to all UEs */
} RgSchCmnUlCell;

/**
  @brief ACK-NACK repetition related information per cell. */
typedef struct rgSchCmnAckNakRepCb 
{
#ifdef LTE_TDD
   CmLListCp  ackNakRepQ[2*RGSCH_NUM_SUB_FRAMES];  /*!< ACK NACK repetition queue */
#else
   CmLListCp  ackNakRepQ[RGSCH_NUM_SUB_FRAMES];  /*!< ACK NACK repetition queue */
#endif
} RgSchCmnAckNakRepCb;

/**
  @brief Measurement Gap related information per cell. */
typedef struct rgSchCmnMeasGapCb 
{
   CmLListCp  gapPrd40Q[RG_SCH_CMN_MEAS_GAPPRD40]; /*!< Measurement Gap queue
                                               for UEs with 40 ms gap period */
   CmLListCp  gapPrd80Q[RG_SCH_CMN_MEAS_GAPPRD80]; /*!< Measurement Gap queue 
                                               for UEs with 80 ms gap period */
} RgSchCmnMeasGapCb;

/**
  * @brief
  * common scheduler specific information for rapId to UE mapping.  */
typedef struct rgSchCmnRapIdMap
{
   U8            rapId;
   CmLListCp     assgndUes;   /*!< List of UEs for which this rapId is 
                               assigned. */
} RgSchCmnRapIdMap;

/**
  * @brief
  * common scheduler specific information for RACH Dedicated Preambles.  */
typedef struct rgSchCmnRachCfg
{
   U8                numDedPrm;   /*!< number of configured dedicated prmbls */
   U8                dedPrmStart; /*!< starting rapId Number */
   U8                remDedPrm;   /*!< remaining number of ded Prm available
                                    for the "applFrm" */
   CmLteTimingInfo   applFrm;     /*!< Frame under consideration for dedPrm
                                     distribution */
   U8                prachMskIndx;/*!< Prach Mask Idx corresponding to
                                      applFrm*/
   RgSchCmnRapIdMap  rapIdMap[RG_SCH_MAX_DED_PRMBLS]; /*!< mapping of RapId 
                                                       *   to assigned UEs */
   CmLListCp         hoUeLst;  /*!< List of UEs undergoing Handover */
   CmLListCp         pdcchOdrLst;  /*!< Pdcch Order Q, holds the UEs for which
                                       PO has to be generated. */
} RgSchCmnRachCfg;
/**
  * @brief
  * Cell specific common scheduler information for all Scheduler types.
  */
typedef struct rgSchCmnCell
{
   RgrCfiCfg           cfiCfg;         /*!< CFI for PDCCH */
   RgrUlTrgCqiCfg      trgUlCqi;       /*!< Target UL CQI */
   CmTqCp              tmrTqCp; /*!< Refresh Timer Task Queue
                                 * Control Point */
   CmTqType            tmrTq[RG_SCH_CMN_NUM_REFRESH_Q]; /*!< Timer Task Queue */
   RgrDlCmnCodeRateCfg dlCmnCodeRate;  /*!< Coding rate for common DL channels: 
                                            Expressed in multiples of 1024 */
   RgrPuschSubBandCfg  puschSubBand;   /*!< UL subband information */
   RgrUlCmnCodeRateCfg ulCmnCodeRate;  /*!< Coding rate for common UL channels: 
                                            Expressed in multiples of 1024 */
   RgSchCmnRachCfg     rachCfg;    /*!< Rach configuration for schCmn */
   RgSchCmnUlCell      ul;         /*!< Scheduler UL info */
   RgSchCmnDlCell      dl;         /*!< Scheduler DL info */
   RgUlSchdApis        *apisUl;    /*!< Specific UL Scheduler APIs */
   RgDlSchdApis        *apisDl;    /*!< Specific DL Scheduler APIs */
   RgDlfsSchdApis      *apisDlfs; /*!< APIs specific to DLFS scheduler */
   CmLteAggrLvl        dciAggrLvl[RG_SCH_CMN_MAX_CQI][10]; 
                                           /*!< Aggr Level for each CQI for 
                                            * each DCI Format */
                                           /*!< DCI Format , Indexed by the 
                                             DCI Format from the following TFU
                                             Enum: typedef enum
                                             {
                                                TFU_DCI_FORMAT_0,       
                                                TFU_DCI_FORMAT_1,       
                                                TFU_DCI_FORMAT_1A,      
                                                TFU_DCI_FORMAT_1B,      
                                                TFU_DCI_FORMAT_1C,      
                                                TFU_DCI_FORMAT_1D,     
                                                TFU_DCI_FORMAT_2,       
                                                TFU_DCI_FORMAT_2A,      
                                                TFU_DCI_FORMAT_3,       
                                                TFU_DCI_FORMAT_3A       
                                             } TfuDciFormat; */
}RgSchCmnCell;

/**
  @brief Uplink Power control related information per UE. */
typedef struct rgSchCmnUeUlPwrCb 
{
   Bool              isAccumulated;   /*!< Indicates if power is accumulative or not */
   Bool              deltaMcsEnbld;   /*!< Indicates if coding effeciency is 
                                       * considered or not for PUSCH power computation */
   U8                pucchIdx;        /*!< Index for TPC-PUCCH-RNTI */
   U8                puschIdx;        /*!< Index for TPC-PUSCH-RNTI */
   U8                isPhrAvail;      /*!< Indicates if PHR is recieved */
   S8                phVal;           /*!< Power headroom value in dB */
   S8                pwrPerRb;        /*!< UL power computed per RB */
   U8                maxUePwr;        /*!< Maximum power with which UE can transmit */
   U8                maxUlRbs;        /*!< Maximum number of UL Rbs for UL scheduling */
   S8                delta;           /*!< Delta corresponding to TPC, for PUSCH */
   U8                numRb;           /*!< Number of RBs used in last allocation */
   S8                remPuschPwr;     /*!< PUSCH power remaining to be adjusted
                                        (in db) */ /* chk if needed */
   S8                remPucchPwr;     /*!< PUCCH Power remaining to be adjusted (in db) */
   U8                pucchTpc;        /*!< TPC to be used for PUCCH power control */
   U8                puschTpc;        /*!< TPC to be used for PUSCH power control */
   U8                trgCqi;          /*!< Target CQI */
   RgSchCmnTpcRntiCb *tpcPucchRntiCb; /*!< Pointer to tpcPucchRntiCb for the UE */
   CmLList           pucchGrpLnk;     /*!< To link together UEs in
                                       * RgSchCmnTpcRntiCb */
   CmLList           schdPucchGrpLnk; /*!< To link together scheduled
                                       * UEs in RgSchCmnTpcRntiCb */
   RgSchCmnTpcRntiCb *tpcPuschRntiCb; /*!< Pointer to tpcPuschRntiCb for the UE */
   CmLList           puschGrpLnk;     /*!< To link together UEs in
                                       * RgSchCmnTpcRntiCb */
   CmLList           schdPuschGrpLnk; /*!< To link together scheduled
                                       * UEs in RgSchCmnTpcRntiCb */
   S8                p0UePusch;         /*!< P_0UE_PUSCH*/
   S8                p0UePucch;         /*!< P_0_PUCCH*/
   S8                maxPwrPerRb;
   S8                maxPwrDeltaByPhr;
} RgSchCmnUeUlPwrCb;

/**
  @brief Uplink RB allocation information. */
struct rgSchCmnUeUlAlloc
{
   /* Request */
   U32          reqBytes;     /*!< Requested bytes */

   /* Allocation to be filled by UL RB allocator module */
   U32          allocdBytes;  /*!< Allocated bytes */
   RgSchUlAlloc *alloc;       /*!< Alloc assgnd by Allocator */
   CmLList      reqLnk;       /*!< To link UL Tx UEs */
   CmLList      schdLstLnk;   /*!< To link scheduled/non-scheduled UL UEs */
};

typedef struct rgSchCmnAllocRecord
{
   U32             alloc;      /* allocation amount */
   CmLteTimingInfo allocTime;  /* Time at which allocation made */
   CmLList         lnk;        /* To link in ulAllocLst */
   U8              numRb;      /* Number of RBs */
   U8              cqi;        /* CQI assumed for allocation */
   U8              tpc;        /* TPC */
}RgSchCmnAllocRecord;


/**
  * @brief
  * Uplink Bler LA information for UE
  */
#ifdef UL_LA
typedef struct ueUlLaCb 
{
   S32             deltaiTbs;  
   U32             iTbsUpperCap;
   S32             cqiBasediTbs;
   Bool            lastiTbsIgnored;
} UeUlLaCb;
#endif 

/**
  * @brief
  * Uplink information for scheduler per UE
  */
typedef struct rgSchCmnUlUe
{
   U8                maxUlCqi;        /*!< CQI for which no better Imcs can be granted */
   U8                crntUlCqi[RG_SCH_MAX_UL_TX_ANT]; /*!< Current CQI */
/* Added changes of TFU_UPGRADE */
#ifdef TFU_UPGRADE
   U8                validUlCqi;
#endif 
   U8                lastCfi;          /* last CFI, updated in case of SPS */
   U32               maxBytesPerUePerTti;         /*!< Maximum bytes that can be allocated 
                                       *   in 1 scheduling instance per UE */
   U32               minReqBytes;     /*!< The buffer amount of lcg0 */
   U32               totalBsr;        /*!< Total BSR as reported by UE (in bytes) */
   U32               nonLcg0Bs;       /*!< Total BSR for LCG1/LCG2/LCG3 */
   U32               nonGbrLcgBs;     /*!< Total BSR as reported by UE (in bytes) for non-GBR LCG's*/
   U32               effBsr;          /*!< BSR yet to be satisfied in allocations */
   CmLListCp         ulAllocLst;      /*!< To track the outstanding Allocations 
                                       *   node type RgSchCmnAllocRecord */
   
   U32               cfgdAmbr;        /*!< Byte rate of UE Ambr per Refresh Cycle */
   U32               effAmbr;         /*!< Remaining Bytes of Byte rate available in a refresh Cycle */

   Void              *schSpfc;        /*!< scheduler specific UE DL Info */
   RgSchCmnUeUlPwrCb ulPwrCb;         /*!< Uplink power control block */
   RgSchCmnUeUlAlloc alloc;           /*!< Allocation info */
   CmLteTimingInfo   ulTransTime;     /*!< Timing info of the latest reception of 
                                     any UL data/signal from UE */
#ifdef LTEMAC_SPS
   RgSchCmnUlUeSpsInfo ulSpsInfo;     /*!< UL SPS information for UE */
#endif
#ifdef UL_LA
   UeUlLaCb       ulLaCb;  /*!< Uplink LA structure */
#endif
} RgSchCmnUlUe;

/**
  @brief Downlink RB allocation information for Msg4. */
typedef struct rgSchCmnMsg4RbAlloc
{
   RgSchDlSf     *msg4DlSf;         /*!< DL sub-frame for which allocation is to
                                         be done: filled in by RR/MAX C/I/PFS */ 
   CmLListCp     msg4TxLst;          /*!< List of RgSchDlRbAllocs for Msg4 Tx */ 
   CmLListCp     msg4RetxLst;        /*!< List of RgSchDlRbAllocs for Msg4 ReTx */ 
   CmLListCp     schdMsg4TxLst;      /*!< List of Msg4 Txs scheduled per TTI */
   CmLListCp     schdMsg4RetxLst;    /*!< List of Msg4 ReTxs scheduled in the TTI */
   CmLListCp     nonSchdMsg4TxLst;   /*!< List of transmitting MSG4 not scheduled in the TTI */
   CmLListCp     nonSchdMsg4RetxLst; /*!< List of re-transmitting MSG4 not 
                                          scheduled in the TTI */
} RgSchCmnMsg4RbAlloc;
#ifdef RGR_V1
/* Changes for CR timer implementation*/
typedef struct rgSchCmnCcchSduRbAlloc
{
   RgSchDlSf     *ccchSduDlSf;         /*!< DL sub-frame for which allocation is to
                                         be done: filled in by RR/MAX C/I/PFS */ 
   CmLListCp     ccchSduTxLst;          /*!< List of RgSchDlRbAllocs for CcchSdu Tx */ 
   CmLListCp     ccchSduRetxLst;        /*!< List of RgSchDlRbAllocs for CcchSdu ReTx */ 
   CmLListCp     schdCcchSduTxLst;      /*!< List of CcchSdu Txs scheduled per TTI */
   CmLListCp     schdCcchSduRetxLst;    /*!< List of CcchSdu ReTxs scheduled in the TTI */
   CmLListCp     nonSchdCcchSduTxLst;   /*!< List of transmitting MSG4 not scheduled in the TTI */
   CmLListCp     nonSchdCcchSduRetxLst; /*!< List of re-transmitting MSG4 not 
                                          scheduled in the TTI */
} RgSchCmnCcchSduRbAlloc;
#endif

/**
  @brief Downlink RB allocation information for UEs. */
typedef struct rgSchCmnUeRbAlloc
{
   RgSchDlSf     *dedDlSf;      /*!< DL sub-frame for which dedicated
                                     allocation is to be done: filled in
                                     by RR/MAX C/I/PFS */ 
   CmLListCp     txHqPLst;       /*!< List of HqPs to be scheduled for Tx per 
                                     TTI: RgSchUeCb list  */
   CmLListCp     retxHqPLst;     /*!< List of HqPs scheduled for ReTx per 
                                     TTI: RgSchUeCb list */
#ifdef LTEMAC_SPS
   CmLListCp     retxSpsHqPLst;   /*!< List of SPS HqPs scheduled for ReTx per 
                                     TTI: RgSchUeCb list */
   CmLListCp     txSpsHqPLst;     /*!< List of SPS HqPs scheduled for Tx per 
                                     TTI: RgSchUeCb list */
#endif
   CmLListCp     schdTxHqPLst;     /*!< List of transmitting HqPs scheduled per TTI */
   CmLListCp     schdRetxHqPLst;   /*!< List of re-transmitting HqPs scheduled per TTI */
   CmLListCp     nonSchdTxHqPLst;  /*!< List of transmitting HqPs not scheduled in the TTI */
   CmLListCp     nonSchdRetxHqPLst;/*!< List of re-transmitting HqPs not scheduled in the TTI */
/* Changes for MIMO feature addition */
   /* MIMO Tx+Retx hqProc scheduling handling */
   CmLListCp     txRetxHqPLst;   /*!< List of HqPs scheduled for tx and retx per 
                                     TTI(MIMO case): RgSchUeCb list */
   CmLListCp     schdTxRetxHqPLst; /*!< List of TX&RETXing(MIMO case) HqPs scheduled per TTI */
   CmLListCp     nonSchdTxRetxHqPLst; /*!< List of TX&RETXing(MIMO case) HqPs not scheduled in the TTI */
#ifdef LTEMAC_SPS
   CmLListCp     schdRetxSpsHqPLst;   /*!< List of re-transmitting SPS HqPs scheduled per TTI */
   CmLListCp     nonSchdRetxSpsHqPLst;/*!< List of re-transmitting SPS HqPs 
                                          not scheduled in the TTI */
   CmLListCp     schdTxSpsHqPLst;     /*!< List of transmitting SPS HqPs scheduled per TTI */
   CmLListCp     nonSchdTxSpsHqPLst; /*!< List of transmitting SPS HqPs not scheduled per TTI */
#endif
} RgSchCmnUeRbAlloc;

/**
  @brief Downlink RB allocation information. */
struct rgSchCmnDlRbAllocInfo
{
   RgSchDlRbAlloc      pcchAlloc;  /*!< Allocation for PCCH */
   RgSchDlRbAlloc      bcchAlloc;  /*!< Allocation for BCCH on DLSCH */
   RgSchDlRbAlloc      raRspAlloc[RG_SCH_CMN_MAX_CMN_PDCCH]; /*!< Allocation for RAR */
   RgSchCmnMsg4RbAlloc msg4Alloc;  /*!< Alloction for Msg4  */
#ifdef RGR_V1
   /* Changes for CR timer implementation*/
   RgSchCmnCcchSduRbAlloc ccchSduAlloc;  /*!< Alloction for ccchSdu  */
#endif
   RgSchCmnUeRbAlloc   dedAlloc;   /*!< Alloction information for UEs */
};

/**
  * @brief
  * RACHO information for scheduler per UE.
  */
typedef struct rgSchCmnDlUeRachInfo
{
   CmLList         inActUeLnk; /*!< Link UE to PO inactUeList */
   CmLList         poLnk;    /*!< To link UE to PDCCH Order Q */
   CmLList         hoLnk;    /*!< To link UE to HandOver UE lst */
   CmLList         rapIdLnk; /*!< Link to the list assgndUes */
   CmLteTimingInfo asgnOppr; /*!< PRACH oppurtunity time assgined to UE */
   U8              hoRapId;  /*!< RAPID assigned to UE for HandOver */
   U8              poRapId;  /*!< RAPID assigned to UE for PdcchOrder */
}RgSchCmnDlUeRachInfo;


/**
  * @brief
  * Downlink CodeWord information for scheduler per UE.
  */
typedef struct rgSchCmnDlUeCwInfo
{
   U8     cqi;     /*!< CQI reported for this CW */
   U8     iTbs[2]; /*!< [0]ITBS for CW for 1 Layer,
                        corresponding to this CW's cqi. */
                   /*!< [1]ITBS for CW for 2 Layer,
                        corresponding to this CW's cqi. */
   U32    eff[2];  /*!< [0]eff for CW for 1 Layer,
                        corresponding to this CW's cqi. */
                   /*!< [1]eff for CW for 2 Layer,
                        corresponding to this CW's cqi. */
   U8     noLyr;   /*!< No. of layers this CW shall be using 
                    *   for transmission */
   U16    dtxCnt;
   U16    ackCnt;
   U16    nackCnt;
}RgSchCmnDlUeCwInfo;
/**
  * @brief UE cmn scheduler specific MIMO Info.
  */
typedef struct rgSchCmnUeMimoInfo
{
   RgSchCmnDlUeCwInfo   cwInfo[RG_SCH_CMN_MAX_CW_PER_UE];/*!< Codeword related feddback Information */ 
   U8                   ri;       /*!< Maximum allowable number of TX layers for SM */ 
   U8                   pmi;      /*!< Precoding matrix indicator(if any) */
   U8                   btrCwIdx; /*!< Index of a better(efficient) CW (0 or 1) */ 
   U8                   forceTD;  /*!< Flag to indicate transmission scheme as TD
                                   *   beyond any other consideration */
}RgSchCmnUeMimoInfo;

typedef struct ueLaCb {
   S32                  deltaiTbs;  
   U32                  iTbsUpperCap;
   S32                  cqiBasediTbs;
   Bool                 lastiTbsIgnored;
   U8                   notFirstCqi;
   U8                   numLastiTbsIgnored;
} UeLaCb;

/**
  * @brief
  * Downlink information for scheduler per UE.
  */
typedef struct rgSchCmnDlUe
{
   U32                  ambrCfgd; /*!< UE's configured AMBR scaled to Refresh Cycle */
   U32                  maxSbSz;  /*!< Max soft channel bits per Hq proc per TTI */
   U32                  maxTbSz;  /*!< Max DLSCH TB bits per TB per TTI */ 
   U8                   maxRb;    /*!< updated based on SoftBuffer Limitation and MaxDlBwPerUE */
   U32                  maxTbBits;/*!< Max Transport Block Bits this UE can receive per TTI*/
   RgSchCmnUeMimoInfo   mimoInfo; /*!< UE cmn scheduler specific MIMO Info */
   RgSchDlHqProcCb      *proc;    /*!< Proc which is picked for Trans for this Subfrm,"dlSf" */
   Void                 *schSpfc; /*!< scheduler specific UE DL Info */
   Void                 *dlfsUe;  /*!< DLFS Specific information */
   U32                  outStndAlloc; /*!< UEs outstanding allocation, for a given TTI.
                                       * valid for a single scheduling index */
   RgSchCmnDlUeRachInfo rachInfo; /*!< Ue specific RACH HO Info */
#ifdef LTEMAC_SPS
   RgSchCmnDlUeSpsInfo  dlSpsInfo;/*!< DL SPS information for the UE */
#endif
#if defined(SCH_STATS) || defined(TENB_STATS)
   U32            schedOccns;
   U32            currPdbLvl;
   U32            prevOccnLvlUpd;
   U32            schedRetxOccns;
   U32            prbAlloc;
#endif
   UeLaCb         laCb[RG_SCH_CMN_MAX_CW_PER_UE];
   U8             cqiFlag;
   U8             lastCfi;

} RgSchCmnDlUe;

/**
  @brief Uplink RB allocation information. */
struct rgSchCmnUlRbAllocInfo
{
   RgSchUlSf     *sf;             /*!< Subframe to schedule for */
   CmLListCp     contResLst;      /*!< UEs to schedule for cnt resn */
   CmLListCp     schdContResLst; /*!< Final UEs scheduled for cnt resn */
   CmLListCp     nonSchdContResLst; /*!< UEs not scheduled for  cnt resn*/
   CmLListCp     ueLst;           /*!< UEs to schedule for data */
   CmLListCp     schdUeLst;      /*!< Final UEs scheduled for data */
   CmLListCp     nonSchdUeLst;   /*!< Final UEs not scheduled for data */
};

/**
  * @brief
  * Information common to DL and UL scheduler per UE.
  */
typedef struct rgSchCmnUeInfo
{
   U8                      ueCat;         /*!< UE category */
   CmTimer                 tmr;
} RgSchCmnUeInfo;
/**
  * @brief
  * Information for scheduler per UE. 
  */
typedef struct rgSchCmnUe
{
  RgSchCmnUeInfo  cmn; /*!< UE specific scheduler information common to
                                  uplink and downlink */ 
  RgSchCmnUlUe   ul;  /*!< UE specific UL scheduler information */
  RgSchCmnDlUe   dl;  /*!< UE specific DL scheduler informaion */ 
} RgSchCmnUe;

typedef struct rgSchCmnLcg
{
   U32      bs;            /*!< Effective Buffer Status */
   U32      cfgdGbr;       /*!< Configured GBR */
   U32      effGbr;        /*!< Effective GBR */
   U32      deltaMbr;      /*!< Configured MBR in excess of configured GBR */
   U32      effDeltaMbr;   /*!< Effective MBR */
   U32      reportedBs;    /*!< Latest Buffer Status */
   Void     *schSpfc;
}RgSchCmnLcg;

#ifdef LTEMAC_SPS
/**
  * @brief
  * SPS information for DL service 
  */
typedef struct rgSchCmnDlSvcSpsInfo
{
   CmLList   toBeSchdSvcEnt;    /*!< Linked list entity for toBeSchdSvcs lst */
   U16       zeroBoOcassionCnt; /*!< Number of contiguous SPS ocassions for 
                                     which BO=0 */
   U32       effSpsBo;          /*!< Effective BO of the SPS service */
   U32       bytesReq;          /*!< Bytes Requested for this SPS service */
   U8        hdrEst;            /*!< Header estimate for SPS service */
   
} RgSchCmnDlSvcSpsInfo;
#endif

typedef struct rgSchCmnDlSvc {
   U8           qci;    /*!< Prio computed against Qci */
   U8           prio;    /*!< Prio computed against Qci */
   U32          gbr;     /*!< scaled GBR as per Refresh time resolution */
   U32          mbr;     /*!< scaled MBR as per Refresh time resolution */
   Void         *schSpfc;/*!< Scheduler specific Info */
#ifdef LTEMAC_SPS
   RgSchCmnDlSvcSpsInfo dlSvcSpsInfo; /*!< SPS related information for DL
                                           service */
#endif
}RgSchCmnDlSvc;

typedef struct rgSchCmnDlHqProc {
   CmLList        retxLnk; /*!< To link retransmitting HARQ processes in cell */
   U32            totBytes;/*!< This maintains total allocation */ 
#ifdef LTEMAC_SPS
   Bool          isSpsSvcSchd;/*!< Indicates if this HARQ process is having SPS
                                   service scheduled: TRUE for SPS and non-SPS
                                   ocassions */
   Bool          isSpsActv;   /*!< Indicates if this HARQ proc
                                   is in-use for SPS transmission: TRUE only for
                                   SPS ocassions */
   U8            spsAction;   /*!< SPS action associated with this HARQ proc:
                                   activation/reactivation */
   CmLteTimingInfo maxRetxTime; /*!< Maximum retransmission time for SPS HARQ
                                  proc */
#endif 
   Void           *schSpfc;/*!< Scheduler specific Info */
}RgSchCmnDlHqProc;

/*--------------------------*
 * UL specific declarations END
 *---------------------------*/

/* Inappropriate name of CQI to ITbs table for DL. */
typedef U8 RgSchCmnCqiToTbs[16];
/* The following data type is used to store computed efficiency */
/* for each MCS and consequently, will be used to derive MCS    */
/* for a CQI. The last row is used for storing the average   */
typedef U32 RgSchCmnTbSzEff[RG_SCH_CMN_NUM_TBS];

/* Inappropriate name of CQI to ITbs table for DL. */
/* Changes for MIMO feature addition */
EXTERN RgSchCmnTbSzEff rgSchCmnNorCfi1Eff[RGSCH_MAX_NUM_LYR_PERCW], rgSchCmnNorCfi2Eff[RGSCH_MAX_NUM_LYR_PERCW];
EXTERN RgSchCmnTbSzEff rgSchCmnNorCfi3Eff[RGSCH_MAX_NUM_LYR_PERCW], rgSchCmnNorCfi4Eff[RGSCH_MAX_NUM_LYR_PERCW];
/* Added new variable for Ul eff */
EXTERN RgSchCmnTbSzEff rgSchCmnNorUlEff[1],rgSchCmnExtUlEff[1];
EXTERN RgSchCmnCqiToTbs rgSchCmnNorCfi1CqiToTbs[RGSCH_MAX_NUM_LYR_PERCW], rgSchCmnNorCfi2CqiToTbs[RGSCH_MAX_NUM_LYR_PERCW];
EXTERN RgSchCmnCqiToTbs rgSchCmnNorCfi3CqiToTbs[RGSCH_MAX_NUM_LYR_PERCW], rgSchCmnNorCfi4CqiToTbs[RGSCH_MAX_NUM_LYR_PERCW];
EXTERN RgSchCmnCqiToTbs *rgSchCmnCqiToTbs[RGSCH_MAX_NUM_LYR_PERCW][RG_SCH_CMN_MAX_CP][RG_SCH_CMN_MAX_CFI]; 
EXTERN RgSchCmnTbSzEff rgSchCmnExtCfi1Eff[RGSCH_MAX_NUM_LYR_PERCW], rgSchCmnExtCfi2Eff[RGSCH_MAX_NUM_LYR_PERCW];
EXTERN RgSchCmnTbSzEff rgSchCmnExtCfi3Eff[RGSCH_MAX_NUM_LYR_PERCW], rgSchCmnExtCfi4Eff[RGSCH_MAX_NUM_LYR_PERCW];
EXTERN RgSchCmnCqiToTbs rgSchCmnExtCfi1CqiToTbs[RGSCH_MAX_NUM_LYR_PERCW], rgSchCmnExtCfi2CqiToTbs[RGSCH_MAX_NUM_LYR_PERCW];
EXTERN RgSchCmnCqiToTbs rgSchCmnExtCfi3CqiToTbs[RGSCH_MAX_NUM_LYR_PERCW], rgSchCmnExtCfi4CqiToTbs[RGSCH_MAX_NUM_LYR_PERCW];
/* Include CRS REs while calculating Efficiency */
EXTERN RgSchCmnTbSzEff
*rgSchCmnEffTbl[RGSCH_MAX_NUM_LYR_PERCW][RG_SCH_CMN_MAX_CP][RG_SCH_CMN_MAX_ANT_CONF][RG_SCH_CMN_MAX_CFI];
/* Added new variable for Ul eff */
EXTERN RgSchCmnTbSzEff *rgSchCmnUlEffTbl[RG_SCH_CMN_MAX_CP];

EXTERN RgSchTbSzTbl rgTbSzTbl;

EXTERN Void rgSCHCmnInit ARGS((Void
));
EXTERN S16 rgSCHCmnRgrCellCfg ARGS((
RgSchCellCb   *cell,
RgrCellCfg    *cellCfg,
RgSchErrInfo  *err
));
EXTERN S16 rgSCHCmnRgrCellRecfg ARGS((
RgSchCellCb             *cell,
RgrCellRecfg            *recfg,
RgSchErrInfo            *err
));
EXTERN Void rgSCHCmnFreeDlLc ARGS((
RgSchCellCb                *cell,
RgSchUeCb                  *ue,
RgSchDlLcCb                *svc
));
EXTERN Void rgSCHCmnCellDel ARGS((
RgSchCellCb     *cell
));
EXTERN Void rgSCHCmnDlRlsSubFrm ARGS((
RgSchCellCb        *cell,
CmLteTimingInfo   frm
));
EXTERN S16 rgSCHCmnRgrUeCfg ARGS((
RgSchCellCb  *cell,
RgSchUeCb    *ue,
RgrUeCfg     *ueCfg,
RgSchErrInfo *err
));
EXTERN S16 rgSCHCmnRgrUeRecfg ARGS((
RgSchCellCb  *cell,
RgSchUeCb    *ue,
RgrUeRecfg   *ueRecfg,
RgSchErrInfo *err
));
EXTERN Void rgSCHCmnUeDel ARGS((
RgSchCellCb  *cell,
RgSchUeCb    *ue
));
EXTERN Void rgSCHCmnUeReset ARGS((
RgSchCellCb  *cell,
RgSchUeCb    *ue
));
EXTERN S16 rgSCHCmnRgrLcgRecfg ARGS((
RgSchCellCb  *cell,
RgSchUeCb    *ue,
RgSchLcgCb   *lcg,
RgrLcgRecfg  *reCfg,
RgSchErrInfo *err
));
EXTERN S16 rgSCHCmnRgrLcgCfg ARGS((
RgSchCellCb  *cell,
RgSchUeCb    *ue,
RgSchLcgCb   *lcg,
RgrLcgCfg    *lcgCfg,
RgSchErrInfo *err
));
EXTERN S16 rgSCHCmnRgrLchCfg ARGS((
RgSchCellCb  *cell,
RgSchUeCb    *ue,
RgSchDlLcCb  *dlLc,
RgrLchCfg *lcCfg,
RgSchErrInfo *err
));
EXTERN S16 rgSCHCmnRgrLchDel ARGS((
RgSchCellCb   *cell,
RgSchUeCb     *ue,
CmLteLcId     lcId,    
U8            lcgId
));
EXTERN S16 rgSCHCmnRgrLchRecfg ARGS((
RgSchCellCb  *cell,
RgSchUeCb    *ue,
RgSchDlLcCb  *dlLc,
RgrLchRecfg  *lcRecfg,
RgSchErrInfo *err
));
EXTERN Void rgSCHCmnLcgDel ARGS((
RgSchCellCb   *cell,
RgSchUeCb     *ue,
RgSchLcgCb    *lcg
));
EXTERN S16 rgSCHCmnUpdBsrShort ARGS((
RgSchCellCb  *cell,
RgSchUeCb    *ue,
RgSchLcgCb   *ulLcg,
U8           bsr,
RgSchErrInfo *err
));
EXTERN S16 rgSCHCmnUpdBsrTrunc ARGS((
RgSchCellCb  *cell,
RgSchUeCb    *ue,
RgSchLcgCb   *ulLcg,
U8           bsr,
RgSchErrInfo *err
));
EXTERN S16 rgSCHCmnUpdBsrLong ARGS((
RgSchCellCb  *cell,
RgSchUeCb    *ue,
U8           bsArr[],
RgSchErrInfo *err
));
EXTERN S16 rgSCHCmnDataRcvd ARGS((
RgSchCellCb  *cell,
RgSchUeCb    *ue,
U8           numLc,
RgSchUlLcCb     *lcArr[],
U16          bytesArr[],
RgSchErrInfo *err
));
EXTERN Void rgSCHCmnUlCqiInd ARGS((
RgSchCellCb          *cell,
RgSchUeCb            *ue,
TfuUlCqiRpt          *ulCqiInfo
));
EXTERN S16 rgSCHCmnUpdPhr ARGS((
RgSchCellCb  *cell,
RgSchUeCb    *ue,
U8           phr,
RgSchErrInfo *err
));
EXTERN S16 rgSCHCmnUpdUlHqProc ARGS((
RgSchCellCb      *cell,
RgSchUlHqProcCb  *curProc,
RgSchUlHqProcCb  *oldProc
));
EXTERN S16 rgSCHCmnContResUlGrant ARGS((
RgSchCellCb  *cell,
RgSchUeCb    *ue,
RgSchErrInfo *err
));
EXTERN Void rgSCHCmnActvtUlUe ARGS((
RgSchCellCb  *cell,
RgSchUeCb    *ue
));
EXTERN Void rgSCHCmnActvtDlUe ARGS((
RgSchCellCb  *cell,
RgSchUeCb    *ue
));
EXTERN Void rgSCHCmnHdlUlTransInd ARGS((
RgSchCellCb     *cell,
RgSchUeCb       *ue,
CmLteTimingInfo timingInfo
));
EXTERN S16 rgSCHCmnSrRcvd ARGS((
RgSchCellCb  *cell,
RgSchUeCb    *ue,
CmLteTimingInfo frm,
RgSchErrInfo *err
));
EXTERN Void rgSCHCmnUlRbAllocAddUeToLst ARGS((
RgSchCellCb           *cell,
RgSchUeCb             *ue,
CmLListCp             *lst
));
EXTERN S16 rgSCHCmnTti ARGS((
RgSchCellCb  *cell,
RgSchErrInfo *err
));
EXTERN Void rgSCHCmnUlHqProcForUe ARGS((
RgSchCellCb         *cell,
CmLteTimingInfo     frm,
RgSchUeCb           *ue,
RgSchUlHqProcCb     **procRef
));
EXTERN RgSchUlAlloc *rgSCHCmnFirstRcptnReq ARGS((
RgSchCellCb      *cell
));
EXTERN RgSchUlAlloc *rgSCHCmnNextRcptnReq ARGS((
RgSchCellCb      *cell,
RgSchUlAlloc     *alloc
));
EXTERN RgSchUlAlloc *rgSCHCmnFirstHqFdbkAlloc ARGS((
RgSchCellCb      *cell,
U8                idx
));
EXTERN RgSchUlAlloc *rgSCHCmnNextHqFdbkAlloc ARGS((
RgSchCellCb      *cell,
RgSchUlAlloc     *alloc,
U8                idx
));
EXTERN Void rgSCHCmnDlDedBoUpd ARGS((
RgSchCellCb                *cell,
RgSchUeCb                  *ue,
RgSchDlLcCb                *svc
));
/* Fix: syed Remove the msg4Proc from cell
 * msg4Retx Queue. I have used CMN scheduler function
 * directly. Please define a new API and call this
 * function through that. */     
EXTERN Void rgSCHCmnDlMsg4ProcRmvFrmRetx ARGS((
RgSchCellCb                *cell,
RgSchDlHqProcCb            *hqP
));
EXTERN Void rgSCHCmnDlProcAddToRetx ARGS((
RgSchCellCb                *cell,
RgSchDlHqProcCb            *hqP
));
EXTERN Void rgSCHCmnDlCqiInd ARGS((
RgSchCellCb        *cell,
RgSchUeCb          *ue,
TfuDlCqiRpt        *dlCqiRpt,
CmLteTimingInfo    timingInfo
));
/* Added changes of TFU_UPGRADE */
#ifdef TFU_UPGRADE
EXTERN  Void rgSCHCmnRawCqiInd ARGS
((
RgSchCellCb        *cellCb,
RgSchUeCb          *ueCb,
TfuRawCqiRpt      *rawCqiRpt,
CmLteTimingInfo    timingInfo
));

EXTERN  Void rgSCHCmnSrsInd ARGS
((
RgSchCellCb        *cellCb,
RgSchUeCb          *ueCb,
TfuSrsRpt      *srsRpt,
CmLteTimingInfo    timingInfo
));
#endif /* TFU_UPGRADE */ 

EXTERN Void rgSCHCmnDlTARpt ARGS((
RgSchCellCb        *cell,
RgSchUeCb          *ue
));
EXTERN RgSchPdcch *rgSCHCmnCmnPdcchAlloc ARGS((
RgSchCellCb                *cell,
RgSchDlSf                  *subFrm
));
EXTERN RgSchUlAlloc *rgSCHCmnUlSbAlloc ARGS((
RgSchUlSf       *sf,
U8              numSb,
RgSchUlHole     *hole
));
#ifdef LTE_TDD
EXTERN Void rgSCHCmnRlsUlSf ARGS((
RgSchCellCb    *cell,
U8 idx
));
#else
EXTERN Void rgSCHCmnRlsUlSf ARGS((
RgSchCellCb    *cell
));
#endif

/* PHR handling for MSG3 */
EXTERN Void rgSCHCmnUlRecMsg3Alloc ARGS((
RgSchCellCb                *cell,
RgSchUeCb                  *ue,
RgSchRaCb                  *raCb
));

/* Added periodic BSR timer */

/*ccpu00117180 - ADD - Added Prototype in .x since the function access is now PUBLIC */
PUBLIC Void rgSCHCmnUpdVars ARGS((
RgSchCellCb *cell
));

#ifdef LTEMAC_SPS
EXTERN Void rgSCHCmnFillHqPTb ARGS((
RgSchCellCb                *cell,
RgSchDlRbAlloc             *rbAllocInfo,
U8                         tbAllocIdx,
RgSchPdcch                 *pdcch
));

EXTERN Void rgSCHCmnDlProcAck ARGS((
RgSchCellCb        *cell,
RgSchDlHqProcCb    *hqP
));
EXTERN Void rgSCHCmnHdlCrntiCE ARGS((
RgSchCellCb        *cell,
RgSchUeCb          *ue
));
EXTERN Void rgSCHCmnDlRelPdcchFbk ARGS((
RgSchCellCb        *cell,
RgSchUeCb          *ue,
Bool               isAck
));
EXTERN Void rgSCHCmnDlGetRbgInfo ARGS((
U8             dlTotalBw,
U8             dlSubsetBw,
U8             maxRaType1SubsetBw,
U8             rbgSize,
RgSchBwRbgInfo *rbgInfo
));
EXTERN U8 rgSCHCmnDlRaType0Alloc ARGS((
RgSchDlSfAllocInfo *allocedInfo, 
U8                 rbsReq,
RgSchBwRbgInfo     *rbgInfo,
U8                 *numAllocRbs,
RgSchDlSfAllocInfo *resAllocInfo, 
Bool               isPartialAlloc
));
#ifdef RGSCH_SPS_UNUSED
EXTERN U8 rgSCHCmnDlRaType1Alloc ARGS((
RgSchDlSfAllocInfo *allocedInfo, 
U8                 rbsReq,
RgSchBwRbgInfo     *rbgInfo,
U8                 startRbgSubset,
U8                 *allocRbgSubset,
RgSchDlSfAllocInfo *resAllocInfo, 
Bool               isPartialAlloc
));
#endif
EXTERN U8 rgSCHCmnDlRaType2Alloc ARGS((
RgSchDlSfAllocInfo *allocedInfo, 
U8                 rbsReq,
RgSchBwRbgInfo     *rbgInfo,
U8                 *rbStart,
RgSchDlSfAllocInfo *resAllocInfo, 
Bool               isPartialAlloc
));
EXTERN Bool rgSCHCmnAllocUeInSpsBw ARGS((
RgSchDlSf           *dlSf,
RgSchCellCb         *cell,
RgSchUeCb           *ue,
RgSchDlRbAlloc      *rbAllocInfo,
Bool                isPartialAlloc
));
#endif
EXTERN Void rgSCHCmnDrxStrtInActvTmrInUl ARGS((RgSchCellCb *cell));
EXTERN Void rgSCHCmnUpdUeDataIndLcg ARGS((RgSchCellCb *cell, RgSchUeCb *ue, RgInfUeDatInd  *datInd));
#ifdef LTE_TDD
EXTERN U8  rgSCHCmnGetPhichUlSfIdx ARGS((CmLteTimingInfo *timeInfo, RgSchCellCb *cell));
EXTERN U8  rgSCHCmnGetUlSfIdx ARGS((CmLteTimingInfo *timeInfo, RgSchCellCb *cell));
EXTERN U8 rgSCHCmnGetPValFrmCCE ARGS((RgSchCellCb *cell, U8 cce));
#endif
EXTERN U8  rgSCHCmnGetUlHqProcIdx ARGS((CmLteTimingInfo *timeInfo, RgSchCellCb *cell));

/* APIs exposed by COMMON SCHEDULER to
 * SPECIFIC SCHEDULER */
/* UL_ALLOC_CHANGES */
EXTERN Void rgSCHCmnUlFreeAlloc ARGS((
RgSchCellCb     *cell,
RgSchUlAlloc    *alloc  
));

EXTERN Void rgSCHCmnUlFreeAllocation ARGS((
RgSchCellCb     *cell,
RgSchUlSf       *sf,    
RgSchUlAlloc    *alloc  
));
/* APIs exposed by DL RB allocation module */
EXTERN S16 rgSCHCmnAllocDlRb ARGS((
RgSchCellCb         *cell, 
RgSchCmnDlRbAllocInfo  *dlRbAllocInfo
));

/* APIs exposed by UL RB allocation module */
EXTERN Void rgSCHCmnAllocUlRb ARGS((
RgSchCellCb            *cell, 
RgSchCmnUlRbAllocInfo  *ulRbAllocInfo
));

/* APIs Exposed to Specific Scheduler */
EXTERN RgSchPdcch *rgSCHCmnPdcchAllocCrntSf ARGS((
RgSchCellCb                *cell,
RgSchUeCb                  *ue
));
EXTERN Void rgSCHCmnPdcchRlsCrntSf ARGS((
RgSchCellCb                *cell,
RgSchPdcch                 *pdcch
));
EXTERN Void rgSCHCmnUlFillPdcchWithAlloc ARGS((
RgSchPdcch      *pdcch,
RgSchUlAlloc    *alloc,
RgSchUeCb       *ue
));
EXTERN Void rgSCHCmnUlAllocFillTpc ARGS((
RgSchCellCb  *cell,
RgSchUeCb    *ue,
RgSchUlAlloc *alloc
));
EXTERN Void rgSCHCmnUlAllocFillNdmrs ARGS((
RgSchCmnUlCell *cellUl,
RgSchUlAlloc   *alloc
));
EXTERN Void rgSCHCmnUlAllocLnkHqProc ARGS((
RgSchUeCb       *ue,
RgSchUlAlloc    *alloc,
RgSchUlHqProcCb *proc,
Bool            isReTx
));
EXTERN RgSchPdcch *rgSCHCmnPdcchAlloc ARGS((
RgSchCellCb                *cell,
RgSchDlSf                  *subFrm,
U32                        y,
U8                         cqi,
TfuDciFormat               dciFrmt,
Bool                       isDtx 
));
EXTERN Void rgSCHCmnRdcImcsTxTb ARGS((
RgSchDlRbAlloc   *allocInfo,
U8               tbInfoIdx,
U32              cnsmdBytes
));
EXTERN Void rgSCHCmnFillPdcch ARGS((
RgSchCellCb                *cell,
RgSchPdcch                 *pdcch,
RgSchDlRbAlloc             *rbAllocInfo
));
EXTERN Void rgSCHCmnUpdDai ARGS((
RgSchUeCb         *ue,
CmLteTimingInfo   *fdbkTime,
U8                 m,
Bool               havePdcch
));
EXTERN Void rgSCHCmnFillHqPPdcch ARGS((
RgSchCellCb                *cell,
RgSchDlRbAlloc             *rbAllocInfo,
RgSchDlHqProcCb            *hqP
));
EXTERN S16 rgSCHCmnDlChkResAvl ARGS((
RgSchCellCb                *cell,
RgSchDlSf                  *subFrm,
RgSchUeCb                  *ue,
RgSchDlHqProcCb            *proc,
U32                        *bo,
U8                         *iTbs,
U32                        *maxRb
));
EXTERN S16 rgSCHCmnDlDedAlloc ARGS((
RgSchCellCb                *cell,
RgSchDlSf                  *subFrm,
RgSchUeCb                  *ue,
RgSchDlHqProcCb            *proc,
U32                        bo,
U8                         iTbs,
U32                        maxRb,
U32                        *bytes
));
EXTERN Void rgSCHCmnUlUeFillAllocInfo  ARGS((
RgSchCellCb      *cell,
RgSchUeCb        *ue
));
/* Fixing incorrect Imcs derivation */
EXTERN U8 rgSCHCmnUlGetITbsFrmIMcs ARGS((
U8          iMcs
));
/* Adding ueCtg to argument list */
EXTERN U8 rgSCHCmnUlGetIMcsFrmITbs ARGS((
U8              iTbs,
CmLteUeCategory ueCtg
));
EXTERN U32 rgSCHCmnUlMinTbBitsForITbs ARGS((
RgSchCmnUlCell     *cellUl,
U8                 iTbs
));
EXTERN U8 rgSCHCmnUlGetITbs ARGS((
RgSchUeCb        *ue,
Bool              isEcp
));
EXTERN Void rgSCHCmnUlAllocFillRbInfo ARGS((
RgSchCellCb    *cell,
RgSchUlSf      *sf,
RgSchUlAlloc   *alloc
));
EXTERN Void rgSCHCmnDlUeResetTemp ARGS((
RgSchUeCb               *ue,
RgSchDlHqProcCb         *hqP
));
EXTERN Void rgSCHCmnUlUeResetTemp ARGS((
RgSchUeCb               *ue
));
/* proc is added for DTX support */
/* DL per UE RB allocation API */
EXTERN S16 rgSCHCmnDlAllocTxRb ARGS((
RgSchCellCb                *cell,
RgSchDlSf                  *subFrm,
RgSchUeCb                  *ue,
U32                        bo,
U32                        *effBo,
RgSchDlHqProcCb            *proc,
RgSchCmnDlRbAllocInfo      *cellWdAllocInfo
));
EXTERN PUBLIC Bool rgSCHCmnIsDlCsgPrio ARGS((
RgSchCellCb    *cell
));
EXTERN PUBLIC Bool rgSCHCmnIsUlCsgPrio ARGS((
RgSchCellCb    *cell
));
EXTERN S16 rgSCHCmnDlAllocRetxRb ARGS((
RgSchCellCb                *cell,
RgSchDlSf                  *subFrm,
RgSchUeCb                  *ue,
U32                        bo,
U32                        *effBo,
RgSchDlHqProcCb            *proc,
RgSchCmnDlRbAllocInfo      *cellWdAllocInfo
));
#ifdef LTEMAC_SPS
EXTERN Void rgSCHCmnClcRbAlloc ARGS((
RgSchCellCb             *cell,
U32                     bo,
U8                      cqi,
U8                      *rb,
U32                     *tbs,
U8                      *mcs,
U8                      *iTbs,
Bool                    isSpsBo,
RgSchDlSf               *sf 
));
PUBLIC U32 rgSCHCmnCalcRiv ARGS((
U8           bw,
U8           rbStart,
U8           numRb
));
#endif /* LTEMAC_SPS */

/* end: Apis to add Ues in to DlRbAllocInfo Lists */
/* start: Apis to add Ues in to UlRbAllocInfo Lists */
EXTERN Void rgSCHCmnUlAdd2UeLst ARGS((
RgSchCmnUlRbAllocInfo *allocInfo,
RgSchUeCb             *ue
));
EXTERN Void rgSCHCmnUlAdd2CntResLst ARGS((
RgSchCmnUlRbAllocInfo *allocInfo,
RgSchUeCb             *ue
));
EXTERN Void rgSCHCmnRmvFrmTaLst ARGS((
RgSchCellCb                *cell,
RgSchUeCb                  *ue
));
/* end: Apis to add Ues in to UlRbAllocInfo Lists */
EXTERN Void rgSCHCmnUlUpdOutStndAlloc ARGS((
RgSchCellCb *cell,
RgSchUeCb   *ue,
U32 alloc
));

EXTERN Void rgSCHCmnUlRecordUeAlloc ARGS((
RgSchCellCb *cell,
RgSchUeCb   *ue
));



/* APIs exposed by common power module */
EXTERN Void rgSCHPwrInit ARGS((
         Void));
EXTERN U8 rgSCHPwrPuschTpcForUe ARGS((
         RgSchCellCb *cell,
         RgSchUeCb   *ue));
EXTERN U8 rgSCHPwrGetMaxUlRb ARGS((
         RgSchCellCb *cell,
         RgSchUeCb   *ue));
EXTERN U8 rgSCHPwrPucchTpcForUe ARGS((
         RgSchCellCb *cell,
         RgSchUeCb   *ue));
EXTERN Void rgSCHPwrGrpCntrlPucch ARGS((
         RgSchCellCb *cell,
         RgSchDlSf   *dlSf));
EXTERN Void rgSCHPwrGrpCntrlPusch ARGS((
         RgSchCellCb *cell,
         RgSchDlSf   *dlSf,
         RgSchUlSf   *ulSf));
EXTERN Void rgSCHPwrPucchDeltaInd ARGS((
         RgSchCellCb *cell,
         RgSchUeCb   *ue,
         S8          pwrDelta));
EXTERN Void rgSCHPwrUpdPhr ARGS((
         RgSchCellCb           *cell,
         RgSchUeCb             *ue,
         U8                     phr,
         RgSchCmnAllocRecord   *allocInfo));
EXTERN Void rgSCHPwrUlCqiInd ARGS((
         RgSchCellCb           *cell,
         RgSchUeCb             *ue
         ));
EXTERN Void rgSCHPwrRecordRbAlloc ARGS((
         RgSchCellCb           *cell,
         RgSchUeCb             *ue,
         U8                     numRb
         ));
EXTERN S16 rgSCHPwrCellCfg ARGS((
         RgSchCellCb          *cell,
         RgrCellCfg           *cfg));
EXTERN S16 rgSCHPwrCellRecfg ARGS((
         RgSchCellCb          *cell,
         RgrCellRecfg         *recfg));
EXTERN Void rgSCHPwrCellDel ARGS((
         RgSchCellCb *cell));
EXTERN S16 rgSCHPwrUeCfg ARGS((
         RgSchCellCb *cell,
         RgSchUeCb   *ue,
         RgrUeCfg    *cfg));
EXTERN S16 rgSCHPwrUeRecfg ARGS((
         RgSchCellCb   *cell,
         RgSchUeCb     *ue,
         RgrUeRecfg    *recfg));
EXTERN Void rgSCHPwrUeDel ARGS((
         RgSchCellCb   *cell,
         RgSchUeCb     *ue));
EXTERN Void rgSCHPwrUeReset ARGS((
         RgSchCellCb   *cell,
         RgSchUeCb     *ue));

#ifdef LTEMAC_SPS
EXTERN S16 rgSCHCmnSpsUlProcCrcInd ARGS((
RgSchCellCb          *cell,
RgSchUeCb            *ue,
CmLteTimingInfo      crcTime
));
EXTERN Void rgSCHCmnSpsInit ARGS((Void));

EXTERN Void rgSCHCmnSpsRelDlSpsActHqP ARGS((
RgSchCellCb        *cell,
RgSchDlHqProcCb    *hqP));

EXTERN S16 rgSCHCmnSpsCellCfg ARGS((
RgSchCellCb        *cell,
RgrCellCfg         *cellCfg,
RgSchErrInfo       *err));
EXTERN Void rgSCHCmnSpsCellDel ARGS((
RgSchCellCb        *cell
));
EXTERN S16 rgSCHCmnSpsUeCfg ARGS((
RgSchCellCb        *cell,
RgSchUeCb          *ue,
RgrUeCfg           *ueCfg,
RgSchErrInfo       *err
));
EXTERN S16 rgSCHCmnSpsUeRecfg ARGS((
RgSchCellCb        *cell,
RgSchUeCb          *ue,
RgrUeRecfg         *ueRecfg,
RgSchErrInfo       *err
));
EXTERN Void rgSCHCmnSpsUeDel ARGS((
RgSchCellCb      *cell,
RgSchUeCb        *ue
));
EXTERN S16 rgSCHCmnSpsDlLcRecfg ARGS((
RgSchCellCb        *cell,
RgSchUeCb          *ue,
RgSchDlLcCb        *dlLc,
RgrLchRecfg        *lcRecfg,
RgSchErrInfo       *err
));
EXTERN S16 rgSCHCmnSpsDlLcCfg ARGS((
RgSchCellCb        *cell,
RgSchUeCb          *ue,
RgSchDlLcCb        *dlLc,
RgrLchCfg          *lcCfg,
RgSchErrInfo       *err
));
EXTERN Void rgSCHCmnSpsDlLcDel ARGS((
RgSchCellCb        *cell,
RgSchUeCb          *ue,
RgSchDlLcCb        *dlLc
));
EXTERN Void rgSCHCmnSpsDlCqiIndHndlr ARGS((
RgSchCellCb          *cell,
RgSchUeCb            *ue,
TfuDlCqiRpt          *cqiIndInfo,
CmLteTimingInfo      timingInfo
));
EXTERN Void rgSCHCmnSpsDlDedBoUpd ARGS((
RgSchCellCb            *cell,
RgSchUeCb              *ue,
RgSchDlLcCb            *dlLc
));
EXTERN Void rgSCHCmnSpsDlUeReset ARGS((
RgSchCellCb         *cell,
RgSchUeCb           *ue
));
EXTERN Void rgSCHCmnSpsDlProcAddToRetx ARGS((
RgSchCellCb           *cell,
RgSchDlHqProcCb       *hqP
));
EXTERN Void rgSCHCmnSpsDlProcAck ARGS((
RgSchCellCb           *cell,
RgSchDlHqProcCb       *hqP
));

EXTERN Void rgSCHCmnSpsDlRelPdcchFbk ARGS((
RgSchCellCb           *cell,
RgSchUeCb             *ue,
Bool                  isAck
));
EXTERN Void rgSCHCmnSpsDlSched ARGS((
RgSchCellCb           *cell,
RgSchCmnDlRbAllocInfo *allocInfo
));
#ifdef RG_UNUSED
EXTERN S16 rgSCHCmnSpsGetDlActvUe ARGS((
RgSchCellCb     *cell,
CmLteTimingInfo *timingInfo,
CmLListCp       *dlSpsActvUeLst
));
#endif
EXTERN Void rgSCHCmnSpsDlAllocFnlz ARGS((
RgSchCellCb           *cell,
RgSchCmnDlRbAllocInfo *allocInfo
));

EXTERN Void rgSCHCmnSpsDlUpdDlSfAllocWithSps ARGS((
RgSchCellCb           *cell,
CmLteTimingInfo       schdTime,
RgSchDlSf             *dlSf
));


/* APIs exposed by UL SPS */
EXTERN Void rgSCHCmnSpsUlLcgDel ARGS(( 
RgSchCellCb      *cell,
RgSchUeCb        *ue,
RgSchLcgCb       *lcg
));
EXTERN Void rgSCHCmnSpsUlUeReset ARGS((
RgSchCellCb         *cell,
RgSchUeCb           *ue
));
EXTERN Void rgSCHCmnSpsUlProcRelInd ARGS((
RgSchCellCb         *cell,
RgSchUeCb           *ue,
Bool                isExplRel
));

EXTERN Void rgSCHCmnSpsUlProcActInd ARGS((
RgSchCellCb         *cell,
RgSchUeCb           *ue,
U16                 spsSduSize
));
EXTERN Void rgSCHCmnSpsPhrInd ARGS((
RgSchCellCb         *cell,
RgSchUeCb           *ue
));


EXTERN S16 rgSCHCmnSpsBsrRpt ARGS((
RgSchCellCb  *cell,
RgSchUeCb    *ue,
RgSchLcgCb   *ulLcg
));


EXTERN S16 rgSCHCmnSpsUlCqiInd ARGS((
RgSchCellCb          *cell,
RgSchUeCb            *ue
));
EXTERN S16 rgSCHCmnSpsUlProcDtxInd ARGS((
RgSchCellCb          *cell,
RgSchUeCb            *ue,
CmLteTimingInfo      dtxTime
));
EXTERN S16 rgSCHCmnSpsUlTti ARGS((
RgSchCellCb             *cell,
RgSchCmnUlRbAllocInfo   *allocInfo
));
#ifdef RG_UNUSED
EXTERN S16 rgSCHCmnSpsUlGetActvUeLst  ARGS((
RgSchCellCb     *cell,
CmLteTimingInfo timingInfo,
CmLListCp       *ulSpsActvUeLst
));
#endif
EXTERN Void rgSCHCmnUlSpsRelInd ARGS((
RgSchCellCb        *cell,
RgSchUeCb          *ue,
Bool               isExplRel
));


EXTERN Void rgSCHCmnUlSpsActInd ARGS((
RgSchCellCb        *cell,
RgSchUeCb          *ue,
U16                spsSduSize
));

EXTERN Void rgSCHCmnUlCrcFailInd ARGS((
RgSchCellCb        *cell,
RgSchUeCb          *ue,
CmLteTimingInfo    crcTime
));
EXTERN Void rgSCHCmnUlCrcInd ARGS((
RgSchCellCb        *cell,
RgSchUeCb          *ue,
CmLteTimingInfo    crcTime
));

/* Added handling to retrnasmit RelPDCCH in case no
                             feedback is received */
EXTERN Void rgSCHCmnSpsDlReTxRelPdcch ARGS((
RgSchCellCb        *cell
));
#endif

EXTERN Void rgSCHCmnChkRetxAllowDtx
ARGS((
RgSchCellCb        *cell,
RgSchUeCb          *ueCb,
RgSchDlHqProcCb    *proc,
Bool               *reTxAllwd
));

EXTERN S16 PtUiRgmBndCfm ARGS((Pst* pst, SuId suId, U8 status));

EXTERN  S16 rgSCHCmnDlInitHqEnt 
ARGS((
RgSchCellCb  *cell,
RgSchUeCb    *ue
));

EXTERN PUBLIC U8 rgSCHCmnUlGetCqi
         ARGS ((
         RgSchUeCb *ue,
         CmLteUeCategory  ueCtgy
         ));

EXTERN PUBLIC Void rgSCHCmnDlDeInitHqEnt 
ARGS((
RgSchCellCb  *cell,
RgSchDlHqEnt *hqE
));
EXTERN PUBLIC U8 rgSCHCmnUlGetCqi
ARGS ((
RgSchUeCb *ue,
CmLteUeCategory  ueCtgy
));
#ifdef DL_LA
EXTERN S16 rgSCHDhmUpdBlerBasediTbsEff ARGS((
         RgSchCmnCell            *cellSch,
         RgSchUeCb               *ueCb,
         U8                      tbCnt
         ));
#endif

EXTERN Void rgSchCmnUpdCfiDb ARGS((
         RgSchCellCb     *cell,
         U8              delta                
         ));
EXTERN S16 RgUiRgmChangeTransModeInd ARGS((
         Pst               *pst, 
         SuId              suId, 
         RgmTransModeInd   *transModeInd));

EXTERN Void rgSchCheckAndTriggerModeChange ARGS((
RgSchCellCb    *cell,
RgSchUeCb      *ue,
U8             reportediTbs,
U8             previTbs,
U8             maxiTbs
));
EXTERN Void rgSCHRrDlProcRmvFrmRetx ARGS((
RgSchCellCb                *cell,
RgSchDlHqProcCb            *hqP
));
#ifdef __cplusplus
}
#endif /* __cplusplus */
#endif /* __RGSCHCMNX__ */


/**********************************************************************
         End of file:     rg_sch_cmn.x@@/main/2 - Sat Jul 30 02:22:02 2011
**********************************************************************/
/**********************************************************************
 
        Revision history:
 
**********************************************************************/
/********************************************************************90**
 
     ver       pat    init                  description
------------ -------- ---- ----------------------------------------------
/main/1      ---      sm       1. LTE MAC 2.1 release
           rg001.201  sm       1. ccpu00106531: fixing incorrect 
                                  Imcs derivation.
                      sd       2. Removed redundant declaration for 
                                  function "rgSCHCmnDlSfFinalize"
                               3. Removed extra comments.
           rg002.201  sm       1. Fixed the wrap around condition for maxRb.
           rg003.201  sm       1. ccpu00107638: Added support for non-adaptive
                                  retransmission in uplink
           rg004.201  sd       1. ccpu00105914: PHR handling for MSG3 
                      ms       2. Handling of DTX                
           rg004.201  rnt      1. Modified for SI Enhancement
           rg006.201  sd       1. ccpu00112398: Added periodic BSR timer
           rg007.201  ap       1. Added support for MIMO
           rg008.201  sd       1. Removed dependency on MIMO compile-time flag.
           rg009.201  dv       1. Added birsPerRb as a member in rgSchCmn Dl
                                  struct derived from bcchpchrarsp coderate
           rg009.201 rsharon   1. [ccpu00112372]Changes for CR timer.
           rg009.201 dv   1. Added Changes of TFU_UPGRADE. 
/main/2      ---         gvj 1. Updated for LTE MAC Rel 3.1
           rg001.301 asehgal 1. Changes for ccpu00117270.
           rg002.301 asehgal 1.changes for ccpu00117606,ccpu00118423
                             2. Changes for ccpu00119494: Introduced a queue
                                to handle Release PDCCH retransmission in SPS.
$SID$        ---     rt      1. LTE MAC 4.1 Release.                           
*********************************************************************91*/
