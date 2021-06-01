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
 
     Desc:     C source code for SPS functions
 
     File:     rg_sch_sps.c
 
     Sid:      gk_sch_sps.c@@/main/2 - Sat Jul 30 02:21:50 2011
 
     Prg:     sd
 
**********************************************************************/

/** @file rg_sch_sps.c
@brief This module handles the semi-persistent scheduling functionality in both
downlink and uplink
*/

#ifdef LTEMAC_SPS
static const char* RLOG_MODULE_NAME="MAC";
static int RLOG_MODULE_ID=4096;
static int RLOG_FILE_ID=174;
#endif
/* header include files -- defines (.h) */
#include "envopt.h"        /* environment options */
#include "envdep.h"        /* environment dependent */
#include "envind.h"        /* environment independent */
#include "gen.h"           /* general layer */
#include "ssi.h"           /* system service interface */
#include "cm_hash.h"       /* common hash list */
#include "cm_llist.h"      /* common linked list library */
#include "cm_math.h"      /* common math library */
#include "cm_err.h"        /* common error */
#include "cm_lte.h"        /* common LTE */
#include "cm_math.h"       /* common MATH */
#include "lrg.h"
#include "rgr.h"
#include "rgm.h"
#include "tfu.h"
#include "rg_env.h"
#include "rg_sch_inf.h"
#include "rg_sch_err.h"
#include "rg_sch.h"
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
#include "cm_math.x"       /* common math library */
#include "cm_mblk.x"       /* memory management */
#include "cm_tkns.x"       /* common tokens */
#include "cm_lte.x"        /* common tokens */
#include "cm_math.x"       /* common MATH */
#include "tfu.x"           /* RGU types */
#include "lrg.x"           /* layer management typedefs for MAC */
#include "rgr.x"           /* layer management typedefs for MAC */
#include "rgm.x"           /* layer management typedefs for MAC */
#include "rg_sch_inf.x"    /* typedefs for Scheduler */
#include "rg_sch.x"        /* typedefs for Scheduler */
#include "rg_sch_cmn.x"


#ifdef RGSCH_SPS_STATS

#define RG_SCH_ABORT()\
   {\
      U32 * nullP = NULL;\
      U32  null = *nullP;\
      null = null;\
   }

#define RGSCH_SPS_MAX_HISTORY 60
typedef struct rgSchSpsDlStateHist{
   U32 ueId;
   U32 state;
   U32 line;
   U64 ttiCnt;
} rgSchSpsDlStateHist_t;

typedef struct rgSchSpsDlClrAllcHist{
   U32 ueId;
   U32 line;
}rgSchSpsDlClrAllcHist_t;

rgSchSpsDlStateHist_t rgSchSpsDlSpsStateHistory[RGSCH_SPS_MAX_HISTORY];
U32   rgSchSpsDlHistoryCount;

rgSchSpsDlClrAllcHist_t   myClearAllocHist[RGSCH_SPS_MAX_HISTORY];
U32   myClearAllocHistCnt;

#define RGSCH_SPS_SET_CLEAR_ALLC_HIST(ue) \
 myClearAllocHist[myClearAllocHistCnt%RGSCH_SPS_MAX_HISTORY].line = __LINE__;\
 myClearAllocHist[myClearAllocHistCnt%RGSCH_SPS_MAX_HISTORY].ueId = ue->ueId;\
 myClearAllocHistCnt++;

#define RGSCH_SPS_SET_DL_SPS_STATE(ue, stateVal) \
(((RgSchCmnUe *)((ue)->sch))->dl.dlSpsInfo).dlSpsStatus = stateVal;\
 rgSchSpsDlSpsStateHistory[rgSchSpsDlHistoryCount%RGSCH_SPS_MAX_HISTORY].state = stateVal; \
 rgSchSpsDlSpsStateHistory[rgSchSpsDlHistoryCount%RGSCH_SPS_MAX_HISTORY].ueId = ue->ueId; \
 rgSchSpsDlSpsStateHistory[rgSchSpsDlHistoryCount%RGSCH_SPS_MAX_HISTORY].line = __LINE__; \
 rgSchSpsDlSpsStateHistory[rgSchSpsDlHistoryCount%RGSCH_SPS_MAX_HISTORY].ttiCnt = glblTtiCnt; \
 rgSchSpsDlHistoryCount++;

#else
#define RGSCH_SPS_SET_CLEAR_ALLC_HIST(ue) 

#define RGSCH_SPS_SET_DL_SPS_STATE(ue, state) \
(((RgSchCmnUe *)((ue)->sch))->dl.dlSpsInfo).dlSpsStatus = state;
#endif


#ifdef RGSCH_SPS_STATS
#define RGSCHDBGSPSSTATS(...) printf(__VA_ARGS__)

#define RGSCH_SPS_MAX_BO_STATS      100
#define RGSCH_SPS_MAX_BSR_STATS     30
#define RGSCH_SPS_MAX_RBALLC_FAIL   20
#define RGSCH_SPS_MAX_STATS         20

U32 rgSchStatCrntiCeRcvCnt;
U32 rgSchSpsRelPdcchAllocd;
U32 rgSchSpsRelSentToTf;
U32 rgSchSpsRelAtTf;

U32 rgSchSpsRetxAllcFailCnt;

U32                  rgSchSpsStatTotalBsr;
U32                  rgSchSpsStatNumBsrRpt;
U32                  rgSchSpsNumActCrcPass;
U32                  rgSchSpsNumUlSpsActSent;
U32                  rgSchSpsNumUlReact;
U32                  rgSchSpsNumImplRel;
U32                  rgSchSpsNumExplRel;
CmLteTimingInfo      rgSchSpsUlSpsActDciTime[RGSCH_SPS_MAX_STATS];
CmLteTimingInfo      rgSchSpsUlSpsImplRelTime[RGSCH_SPS_MAX_STATS];
CmLteTimingInfo      rgSchSpsUlSpsExplRelTime[RGSCH_SPS_MAX_STATS];
CmLteTimingInfo      rgSchSpsUlSpsRelPdcchAllocTime[RGSCH_SPS_MAX_STATS];
U32                  rgSchSpsNumExplRelPdcchAlloc;
U32      rgSchSpsUlSpsRelPdcchAllocUE[RGSCH_SPS_MAX_STATS];



U32 ulSpsBsSizeCnt;
U32 ulSpsBsSizeStats[RGSCH_SPS_MAX_BSR_STATS];


U32 spsBsrUpdState[7];
U32 spsResetState[7];
U32 spsBoIntrvStats[21];
U32 spsBoSizeStats[20];
U32 spsBoUpdState[6];
U32 spsBoValueStats[RGSCH_SPS_MAX_BO_STATS];
U32 spsBoIntrValueStats[RGSCH_SPS_MAX_BO_STATS];
U32 numSPSBoUpd;    
U32 dlRelPdcchRetx;
U32 crcFailOccCountStat;
U32 crcFailActCountStat;
U32 crcPassCount;
U32 crcFailExplRelStat;
U32 ulExplRelSucc;
U32 improperItbsCnt;
U32 numSPSSchedAcked;
U32 numDynSched;
U32 numSPSSched;
U32 rgNumSPSSchedDropMaxRetx;
U32 numActvations;
U32 numActSuccess;
U32 rgNumActDtx;
U32 numActAckOrNack;
U32 numReactivations;
U32 numReactivationsBo;
U32 numReactivationsCqi;
U32 numdlSpsRel;
U32 numdlSpsRelSentToTf;
U32 numdlSpsRelRcvdAtCl;
U32 numSpsActResetDtx;
U32 ulSpsOccSched;
/* Failure stats */
U32 dlSpsFailToActUeAlreadySched;
U32 dlSpsFailToActUeCollInTime;
U32 dlSpsFailToActUeCollInFreq;
U32 dlSpsFailToActUeAlloc;
U32 dlSpsFailToActUeAllocFnlz;
U32 spsRbAllocFailReqRbs[RGSCH_SPS_MAX_RBALLC_FAIL];
U32 spsRbAllocFailCnt;
#else
#define RGSCHDBGSPSSTATS(...) 
#endif
#ifdef LTEMAC_SPS
/* Private functions */
typedef enum rgSchCmnSpsPrdEnum
{
   RG_SCH_CMN_SPS_PRD_2SF,
   RG_SCH_CMN_SPS_PRD_5SF,
   RG_SCH_CMN_SPS_PRD_10SF,
   RG_SCH_CMN_SPS_PRD_20SF,
   RG_SCH_CMN_SPS_PRD_30SF,
   RG_SCH_CMN_SPS_PRD_32SF,
   RG_SCH_CMN_SPS_PRD_40SF,
   RG_SCH_CMN_SPS_PRD_60SF,
   RG_SCH_CMN_SPS_PRD_64SF,
   RG_SCH_CMN_SPS_PRD_80SF,
   RG_SCH_CMN_SPS_PRD_120SF,
   RG_SCH_CMN_SPS_PRD_128SF,
   RG_SCH_CMN_SPS_PRD_160SF,
   RG_SCH_CMN_SPS_PRD_256SF,
   RG_SCH_CMN_SPS_PRD_320SF,
   RG_SCH_CMN_SPS_PRD_512SF,
   RG_SCH_CMN_SPS_PRD_640SF,
   RG_SCH_CMN_SPS_PRD_1024SF,
   RG_SCH_CMN_SPS_PRD_1280SF,
   RG_SCH_CMN_SPS_PRD_2048SF,
   RG_SCH_CMN_SPS_PRD_2560SF,
   RG_SCH_CMN_SPS_PRD_INVALID
} RgSchCmnSpsPrdEnum; 

/* DCI Format options for each Transmission Mode and selected RA TYPE 
 * for UE specific search-space using SPS-RNTI only */

TfuDciFormat rgSchCmnSpsRaTypeToDciFrmt[7][3] = {
   {TFU_DCI_FORMAT_1, TFU_DCI_FORMAT_1, TFU_DCI_FORMAT_1A},
   {TFU_DCI_FORMAT_1, TFU_DCI_FORMAT_1, TFU_DCI_FORMAT_1A},
   {TFU_DCI_FORMAT_2A,TFU_DCI_FORMAT_2A, TFU_DCI_FORMAT_1A},
   {TFU_DCI_FORMAT_2, TFU_DCI_FORMAT_2, TFU_DCI_FORMAT_1A},
   {TFU_DCI_FORMAT_INVALID,TFU_DCI_FORMAT_INVALID, TFU_DCI_FORMAT_1A},
   {TFU_DCI_FORMAT_INVALID,TFU_DCI_FORMAT_INVALID, TFU_DCI_FORMAT_1A},
   {TFU_DCI_FORMAT_1, TFU_DCI_FORMAT_1, TFU_DCI_FORMAT_1A}
};

/* LCM table for the possible combination of periodicities */
PRIVATE U16
 rgSchCmnSpsLcmTbl[RG_SCH_CMN_SPS_MAX_NUM_PRD][RG_SCH_CMN_SPS_MAX_NUM_PRD];

PRIVATE U16 rgSchCmnSpsPrdVal[RG_SCH_CMN_SPS_MAX_NUM_PRD] 
= {2, 5, 10, 20, 30, 32, 40, 60, 64, 80, 
   120, 128, 160, 256, 320, 512, 640, 1024, 1280, 2048, 2560};

#ifdef LTE_TDD
/* Harq interval values for various TDD Configurations for SPS_TDD */
/* Array initialization to 0 */
PRIVATE U8 rgSchHqIntTbl[RGSCH_MAX_TDD_UL_DL_CFG] =
 {0};

#endif /* LTE_TDD */
/* DL SPS private functions */

/* DL SPS Configuration/initialization functions */
PRIVATE Void rgSCHCmnSpsHandleDlMaxRetx ARGS((
RgSchCellCb           *cell,
RgSchDlHqProcCb       *hqP,
Bool                  *isMaxRetxHit
));

PRIVATE Void rgSCHCmnSpsRelHqProc ARGS((
RgSchCellCb           *cell,
RgSchDlHqProcCb       *hqP
));

PRIVATE Void rgSCHCmnSpsCompLcmTbl ARGS((Void));
PRIVATE U16 rgSCHCmnSpsCompLcm ARGS((
 U16      prd1,
 U16      prd2
));
PRIVATE Void rgSCHCmnSpsUtlDlDelFrmZeroBOSvcUes
(
RgSchCmnDlCellSpsInfo   *dlSpsCell,
RgSchUeCb               *ue
);
PRIVATE S16 rgSCHCmnSpsVldtDlCellCfg ARGS((
RgSchCellCb        *cell,
RgrCellCfg         *cellCfg
));
PRIVATE S16 rgSCHCmnSpsDlCellCfg ARGS((
RgSchCellCb        *cell,
RgrCellCfg         *cellCfg,
RgSchErrInfo       *err
));
PRIVATE S16 rgSCHCmnSpsDlSfInit ARGS((
RgSchCellCb        *cell,
RgrCellCfg         *cellCfg
));
PRIVATE Void rgSCHCmnSpsDlN1PucchInit ARGS((
RgSchCellCb        *cell,
RgrCellCfg         *cellCfg
));
PRIVATE S16 rgSCHCmnSpsVldtDlUeCfg ARGS((
RgSchCellCb        *cell,
RgSchUeCb          *ue,
RgrUeCfg           *ueCfg
));
PRIVATE S16 rgSCHCmnSpsDlUeCfg ARGS((
RgSchCellCb        *cell,
RgSchUeCb          *ue,
RgrUeCfg           *ueCfg,
RgSchErrInfo       *err
));
PRIVATE S16 rgSCHCmnSpsVldtDlUeRecfg ARGS((
RgSchCellCb        *cell,
RgSchUeCb          *ue,
RgrUeRecfg         *ueRecfg
));
PRIVATE S16 rgSCHCmnSpsDlUeRecfg ARGS((
RgSchCellCb        *cell,
RgSchUeCb          *ue,
RgrUeRecfg         *ueRecfg,
RgSchErrInfo       *err
));
PUBLIC Void rgSCHCmnSpsDlCellDel ARGS((
RgSchCellCb        *cell
));
PRIVATE Void rgSCHCmnSpsDlUeSuspend ARGS((
RgSchCellCb        *cell,
RgSchUeCb          *ue,
Bool                isSpsDisab
));
PRIVATE Void rgSCHCmnSpsDlCellInit ARGS((
RgSchCellCb        *cell,
RgrCellCfg         *cellCfg
));
PRIVATE S16 rgSCHCmnSpsDlUeInit ARGS((
RgSchCellCb        *cell,
RgSchUeCb          *ue,
RgrUeSpsCfg        *ueSpsCfg,
RgrUeMeasGapCfg    *ueMeasGapCfg
));


/* DL SPS Scheduling functions */

PRIVATE RgSchCmnSpsDlN1Pucch *rgSCHCmnSpsDlGetN1PucchCb ARGS((
RgSchCmnSpsDlN1PucchDb  *n1PucchDb,
U16                     n1PucchVal
));
#ifdef RGSCH_SPS_UNUSED
PRIVATE Void rgSCHCmnDlSpsRstMaxSpsBw ARGS((
U8             rbgSize,
U8             dlTotalBw,
U8             *spsBw
));
#endif
PRIVATE Void rgSCHCmnSpsDlProcRetxUes ARGS((
RgSchCellCb           *cell,
CmLteTimingInfo       frmTime,
RgSchDlSf             *dlSf,
RgSchCmnDlRbAllocInfo *allocInfo
));
PRIVATE Void rgSCHCmnSpsDlProcToBeSchdSvcs ARGS((
RgSchCellCb           *cell,
CmLteTimingInfo       schdTime,
RgSchDlSf             *dlSf,
RgSchCmnDlRbAllocInfo *allocInfo
));
PRIVATE S16 rgSCHCmnSpsDlUpdSf ARGS((
RgSchCellCb        *cell,
U32                prdVal
));
PRIVATE U8 rgSCHCmnSpsDlGetScaledCqi ARGS((
RgSchUeCb             *ue,
U8                    curCqi
));
PRIVATE S16 rgSCHCmnSpsDlUeClearAlloc ARGS((
RgSchCellCb         *cell,
RgSchUeCb           *ue
));
PRIVATE S16 rgSCHCmnSpsDlUeClearN1PucchAlloc ARGS((
RgSchCellCb         *cell,
RgSchUeCb           *ue,
Bool                 clearDbAlloc
));
PRIVATE Void rgSCHCmnSpsDlProcActvtdUes ARGS((
RgSchCellCb           *cell,
CmLteTimingInfo       crntTime,
RgSchDlSf             *dlSf,
RgSchCmnDlRbAllocInfo *allocInfo
));
PRIVATE Void rgSCHCmnSpsDlProcToBeRelUes ARGS((
RgSchCellCb           *cell,
CmLteTimingInfo       crntTime,
RgSchDlSf             *dlSf
));
PRIVATE S16 rgSCHCmnSpsDlProcToBeActvtdUes ARGS((
RgSchCellCb            *cell,
CmLteTimingInfo        crntTime,
RgSchDlSf              *dlSf
));
PRIVATE Bool rgSCHCmnSpsDlChkUeInActv ARGS((
RgSchCellCb            *cell,
RgSchUeCb              *ue,
CmLteTimingInfo        timingInfo
));
PRIVATE S16 rgSCHCmnSpsDlAllocForSvc ARGS((
RgSchCellCb            *cell,
CmLteTimingInfo        schdTime,
RgSchDlSf              *subFrm,
RgSchDlLcCb            *svc,
U32                    bo,
RgSchCmnDlRbAllocInfo  *allocInfo,
Bool 				   *bwExp
));
PRIVATE S16 rgSCHCmnSpsDlProcToBeReactvtdUe ARGS((
RgSchCellCb            *cell,
RgSchUeCb              *ue,
CmLteTimingInfo        schdTime,
RgSchDlSf              *dlSf
));
PRIVATE S16 rgSCHCmnSpsDlUeAllocFnlz ARGS((
RgSchCellCb         *cell,
RgSchUeCb           *ue,
CmLteTimingInfo     schdTime,
RgSchDlSf           *dlSf,
U8                  n1PucchIdx,
U8                  isActvOrReactv
));
PRIVATE Bool rgSCHCmnSpsDlUeCollidesInTime ARGS((
RgSchCellCb            *cell,
RgSchUeCb              *ue,
CmLteTimingInfo        timingInfo,
RgrUeMeasGapCfg      *measGapCfg
));
PRIVATE Bool rgSCHCmnSpsDlUeCollidesInFreqOrN1Pucch ARGS((
RgSchCellCb         *cell,
RgSchUeCb           *ue,
CmLteTimingInfo     timingInfo,
RgSchDlSfAllocInfo  *allocInfo,
U8                  *n1PucchIdx,
Bool                 ignoreUe
));

PRIVATE Bool rgSCHCmnChkMaxSpsUePerSf ARGS((
RgSchCellCb           *cell,
RgSchUeCb             *ue,
CmLteTimingInfo       timeToTx
));

PRIVATE S16 rgSCHCmnSpsDlUeAlloc ARGS((
RgSchCellCb         *cell,
RgSchUeCb           *ue,
CmLteTimingInfo     timingInfo,
RgSchDlSf           *dlSf,
RgSchDlSfAllocInfo  *allocInfo
));
PRIVATE Bool rgSCHCmnSpsDlChkMeasGapCollision ARGS((
U8                  spsPrdIdx,
U16                 startIdx,
U16                 measGapPrd,
U16                 spsPrd,
U32                  *measGapMask
));
PRIVATE S16 rgSCHCmnSpsDlUeAllocRbs ARGS((
RgSchCellCb         *cell,
RgSchUeCb           *ue,
CmLteTimingInfo     timingInfo,
RgSchDlSf           *dlSf,
RgSchDlSfAllocInfo  *allocInfo
));
PRIVATE Void rgSCHCmnSpsDlGetDciFmt ARGS((
RgSchUeCb           *ue,
U8                  raType,
TfuDciFormat        *dciFmt
));
PRIVATE S16 rgSCHCmnSpsDlUpdSfOnAlloc ARGS((
RgSchCellCb         *cell,
RgSchUeCb           *ue,
CmLteTimingInfo     schdTime,
RgSchDlSf           *dlSf,
U8                  n1PucchIdx
));
PRIVATE S16 rgSCHCmnSpsDlUpdUeOnAlloc ARGS((
RgSchCellCb         *cell,
RgSchUeCb           *ue,
CmLteTimingInfo     schdTime,
U8                  n1PucchIdx
));
PRIVATE Void rgSCHCmnSpsDlFillActvPdcch ARGS((
RgSchCellCb         *cell,
RgSchUeCb           *ue,
RgSchPdcch          *pdcch,
Bool                isSpsOcc,
U8                  hqPId
));
PRIVATE Void rgSCHCmnSpsDlTxAllocFnlz ARGS((
RgSchCellCb           *cell,
RgSchCmnDlRbAllocInfo *allocInfo
));
PRIVATE Void rgSCHCmnSpsDlRetxAllocFnlz ARGS((
RgSchCellCb           *cell,
RgSchCmnDlRbAllocInfo *allocInfo
));

#ifdef RG_UNUSED
PRIVATE Bool rgSCHCmnSpsDlChkCollision ARGS((
U8                    spsPrdIdx,
U16                   startIdx,
U16                   prd,
U16                   spsPrd,
U16                   offset
));
#endif /* RG_UNUSED */

PRIVATE Void rgSCHCmnSpsDlFillRelPdcch ARGS((
RgSchUeCb       *ue,
RgSchPdcch      *relPdcch
));
PRIVATE Void rgSCHCmnSpsDlNewTxUeFnlz ARGS((
RgSchCellCb    *cell,
RgSchDlRbAlloc *ueAllocInfo, 
RgSchUeCb      *ue
));
PRIVATE Void rgSCHCmnSpsDlFreeN1Pucch ARGS((
RgSchCmnSpsDlN1PucchDb  *n1PucchDb,
RgSchCmnSpsDlN1Pucch    *n1PucchCb
));
PRIVATE S16 rgSCHCmnSpsDlInitMeasGapMask ARGS((
RgSchCellCb          *cell,
RgrUeMeasGapCfg      *measGapCfg,
RgSchCmnDlUeSpsInfo  *dlSpsUe
));
PRIVATE Void rgSCHCmnSpsDlUpdDlSfAllocWithZeroBoSvcs ARGS((
RgSchCellCb           *cell,
RgSchDlSf             *dlSf
)); 

PRIVATE Void rgSCHCmnSpsDlUpdSpsWithDlSfAlloc ARGS((
RgSchDlSf             *dlSf,
RgSchDlSfAllocInfo    *allocInfo
));

/* DL SPS Utility functions */
PRIVATE Void rgSCHCmnSpsUtlDlAdd2ZeroBOSvcUes ARGS((
RgSchCmnDlCellSpsInfo   *dlSpsCell,
RgSchUeCb               *ue
)); 

PRIVATE Void rgSCHCmnSpsUtlDlAdd2ToBeActvtdUes ARGS((
RgSchCmnDlCellSpsInfo   *dlSpsCell,
RgSchUeCb               *ue
));
PRIVATE Void rgSCHCmnSpsUtlDlDelFrmToBeActvtdUes ARGS((
RgSchCmnDlCellSpsInfo   *dlSpsCell,
RgSchUeCb               *ue
));
PRIVATE Void rgSCHCmnSpsUtlDlAdd2ActvUes ARGS((
RgSchCmnDlCellSpsInfo   *dlSpsCell,
RgSchUeCb               *ue,
CmLteTimingInfo         actvTime
));
PRIVATE Void rgSCHCmnSpsUtlDlDelFrmActvUes ARGS((
RgSchCmnDlCellSpsInfo   *dlSpsCell,
RgSchUeCb               *ue,
CmLteTimingInfo         actvTime
));
PRIVATE Void rgSCHCmnSpsUtlDlAdd2ToBeRelUes ARGS((
RgSchCmnDlCellSpsInfo   *dlSpsCell,
RgSchUeCb               *ue
));
PRIVATE Void rgSCHCmnSpsUtlDlDelFrmToBeRelUes ARGS((
RgSchCmnDlCellSpsInfo   *dlSpsCell,
RgSchUeCb               *ue
));
PRIVATE Void rgSCHCmnSpsUtlDlAdd2RetxHqProcs ARGS((
RgSchCmnDlCellSpsInfo   *dlSpsCell,
RgSchDlHqProcCb         *hqP
));
PRIVATE Void rgSCHCmnSpsUtlDlDelFrmRetxHqProcs ARGS((
RgSchCmnDlCellSpsInfo   *dlSpsCell,
RgSchDlHqProcCb         *hqP
));
PRIVATE Void rgSCHCmnSpsUtlDlAdd2ToBeSchdSvcs ARGS((
RgSchCmnDlCellSpsInfo   *dlSpsCell,
RgSchDlLcCb             *dlSvc
));
PRIVATE Void rgSCHCmnSpsUtlDlDelFrmToBeSchdSvcs ARGS((
RgSchCmnDlCellSpsInfo   *dlSpsCell,
RgSchDlLcCb             *dlSvc
));
PRIVATE Void rgSCHCmnSpsUtlDlAdd2TxSpsUeLst ARGS((
RgSchCmnDlRbAllocInfo *allocInfo,
RgSchDlHqProcCb      *hqP
));
PRIVATE Void rgSCHCmnSpsUtlDlAdd2RetxSpsUeLst ARGS((
RgSchCmnDlRbAllocInfo *allocInfo,
RgSchDlHqProcCb      *hqP
));

PRIVATE S16 rgSCHCmnSpsUlFreeDlSpsLsts ARGS((
RgSchCellCb           *cell
));

#ifdef RGSCH_SPS_STATS
/* DL SPS Stats funcs */
PRIVATE S16 rgSCHCmnSpsPrintStats ARGS((
RgSchCellCb           *cell
));
#endif


/* UL SPS functions */
PRIVATE S16 rgSCHCmnSpsUlVldtCellCfg ARGS((
RgSchCellCb    *cell,
RgrCellCfg     *cellCfg
));

PRIVATE S16 rgSCHCmnSpsUlCellCfg ARGS((
RgSchCellCb    *cell,
RgrCellCfg     *cellCfg
));

PRIVATE S16 rgSCHCmnSpsUlCellDel ARGS((
RgSchCellCb    *cell
));

PRIVATE S16 rgSCHCmnSpsUlVldtUeCfg ARGS((
RgSchCellCb  *cell,
RgrUeCfg     *ueCfg
));

PRIVATE S16 rgSCHCmnSpsUlUeCfg ARGS((
RgSchCellCb  *cell,
RgSchUeCb    *ue,
RgrUeCfg     *ueCfg
));

PRIVATE S16 rgSCHCmnSpsUlVldtUeRecfg ARGS((
RgSchCellCb  *cell,
RgSchUeCb    *ue,
RgrUeRecfg   *ueRecfg
));

PRIVATE S16 rgSCHCmnSpsUlUeRecfg ARGS((
RgSchCellCb  *cell,
RgSchUeCb    *ue,
RgrUeRecfg   *ueRecfg
));

PRIVATE Void rgSCHCmnSpsUlUeDel ARGS((
RgSchCellCb      *cell,
RgSchUeCb        *ue
));

PRIVATE Void rgSCHCmnSpsUlUeMeasGapMaskInit ARGS((
RgSchCellCb        *cell,
RgSchUeCb          *ue,
RgrUeMeasGapCfg    ueMesGapCfg
));

PRIVATE Void rgSCHCmnSpsUlUeInit ARGS((
RgSchCellCb     *cell,
RgSchUeCb       *ue,
RgrUeSpsCfg     spsCfg,
RgrUeMeasGapCfg measGapCfg
));

PRIVATE S16 rgSCHCmnSpsUlBwMaskInit ARGS((
RgSchCellCb     *cell,
U16             lstSz
));

PRIVATE Void rgSCHCmnSpsUlInitSpsSf ARGS((
RgSchCellCb     *cell
));

PRIVATE S16 rgSCHCmnSpsUlUpdSpsSf ARGS((
RgSchCellCb     *cell,
U16             crntLcm,
U16             nextLcm
));

PRIVATE U8 rgSCHCmnSpsScaleUlCqi ARGS((
RgSchCellCb     *cell,
RgSchUeCb       *ue
));

PRIVATE U8 rgSCHCmnSpsUlCalcSbReqd ARGS((
RgSchCellCb          *cell,
RgSchUeCb            *ue,
U8                   cqi,
U8                   *iMcs,
U8                   *iTbs
));

PRIVATE RgSchCmnSpsUlSf* rgSCHCmnSpsGetNextNthSpsUlSf ARGS((
RgSchCellCb     *cell,
RgSchUeCb       *ue,
CmLteTimingInfo ulSpsActvOcc,
U8              prsntidx,
CmLteTimingInfo  *nxtOcc
));

PRIVATE U8 rgSCHCmnSpsUlGetITbs ARGS((
RgSchUeCb        *ue,
Bool             isEcp
));

PRIVATE RgSchUlAlloc* rgSCHCmnSpsUlSbAlloc ARGS((
RgSchCellCb           *cell,
RgSchUeCb             *ue,
RgSchUlSf             *sf,
U8                    numSb,
CmLteTimingInfo       timeToTx
));

PRIVATE S16 rgSCHCmnSpsUlSbAllocForUe ARGS((
RgSchCellCb           *cell,
RgSchUeCb             *ue,
RgSchUlSf             *sf,
CmLteTimingInfo       timeToTx
));

PRIVATE S16 rgSCHCmnUlSpsClrAlloc ARGS((
RgSchCellCb             *cell,
RgSchUeCb               *ue,
RgSchCmnSpsUlSf         *spsSf,
RgSchUlAlloc            *alloc
));

PRIVATE S16 rgSCHCmnUlSpsMarkAlloc ARGS((
RgSchCellCb             *cell,
RgSchUeCb               *ue,
RgSchCmnSpsUlSf         *spsSf,
CmLteTimingInfo         timeToTx
));

PRIVATE S16 rgSCHCmnSpsUlUeAlloc ARGS((
RgSchCellCb             *cell,
RgSchUeCb               *ue,
RgSchCmnSpsUlSf         *spsSf,
RgSchCmnUlRbAllocInfo   *alloc,
RgSchUlAlloc            *ueSpsAlloc,
CmLteTimingInfo         timeToTx
));

PRIVATE Bool rgSCHCmnSpsUlChkMesGapColl ARGS((
RgSchCellCb             *cell,
RgSchUeCb               *ue,
CmLteTimingInfo         timeToTx,
U8                      gapPrd
));

PRIVATE S16 rgSCHCmnSpsUlFetchAllocInUlSf ARGS((
RgSchCellCb           *cell,
RgSchUeCb             *ue,
RgSchUlSf             *ulSf
));

PRIVATE S16 rgSCHCmnSpsUlProcActvUeInCrntSf ARGS((
RgSchCellCb             *cell
));

PRIVATE S16 rgSCHCmnSpsUlProcActvUeNxtOcc ARGS((
RgSchCellCb             *cell,
RgSchCmnUlRbAllocInfo   *allocInfo
));

PRIVATE S16 rgSCHCmnSpsUlMarkActvUeAllocs ARGS((
RgSchCellCb             *cell
));

PRIVATE S16 rgSCHCmnSpsUlProcActvtdUes ARGS((
RgSchCellCb             *cell,
RgSchCmnUlRbAllocInfo   *allocInfo
));

PRIVATE S16 rgSCHCmnSpsUlProcToBeRelUe ARGS((
RgSchCellCb             *cell
));

PRIVATE S16 rgSCHCmnSpsUlProcToBeReactvtdUe ARGS((
RgSchCellCb             *cell,
RgSchUeCb               *ue,
RgSchCmnUlRbAllocInfo   *alloc
));

PRIVATE S16 rgSCHCmnSpsUlProcToBeActvtdUe ARGS((
RgSchCellCb             *cell,
RgSchCmnUlRbAllocInfo   *alloc
));

PRIVATE Bool rgSCHCmnSpsUlChkUeInActv ARGS((
RgSchCellCb             *cell,
RgSchUeCb               *ue
));

PRIVATE Void rgSCHCmnSpsUlUtlFreeSpsUe ARGS((
RgSchCellCb     *cell,
RgSchUeCb       *ue
));

PRIVATE Void rgSCHCmnSpsUlUtlRmvSpsAlloc ARGS((
RgSchCellCb     *cell,
RgSchUeCb       *ue
));
PRIVATE RgSchUlAlloc *rgSCHCmnSpsUtlUlGetSpfcAlloc ARGS((
RgSchCellCb     *cell,         
RgSchCmnUlUe    *ueUl,
RgSchUlSf       *sf,
U8              startSb,
U8              numSb,
U8              cfi  
));

PRIVATE S16 rgSCHCmnSpsUlUtlCpyAlloc ARGS((
RgSchUlAlloc         *srcAlloc,
RgSchUlAlloc         *dstAlloc 
));

PRIVATE S16 rgSCHCmnSpsUlUtlFillRelPdcch ARGS((
RgSchCellCb             *cell,
RgSchUeCb               *ue,
RgSchPdcch              *pdcch
));

PRIVATE S16 rgSCHCmnSpsUlUtlInsUeInToBeRelLst ARGS((
RgSchCellCb           *cell,
RgSchUeCb             *ue
));

PRIVATE S16 rgSCHCmnSpsUlUtlInsUeInToBeActvLst ARGS((
RgSchCellCb           *cell,
RgSchUeCb             *ue
));

PRIVATE S16 rgSCHCmnSpsUlInsUeInActvLst ARGS((
RgSchCellCb           *cell,
RgSchUeCb             *ue,
CmLteTimingInfo       spsOcc
));

PRIVATE S16 rgSCHCmnSpsUlDelUeFrmToBeActvLst ARGS((
RgSchCellCb           *cell,
RgSchUeCb             *ue
));

PRIVATE S16 rgSCHCmnSpsUlDelUeFrmToBeRelLst ARGS((
RgSchCellCb           *cell,
RgSchUeCb             *ue
));

PRIVATE S16 rgSCHCmnSpsUlDelUeFrmActvLst ARGS((
RgSchCellCb           *cell,
RgSchUeCb             *ue,
CmLteTimingInfo       spsOcc
));

PRIVATE S16 rgSCHCmnSpsUlFreeToBeActvUeLst ARGS((
RgSchCellCb           *cell
));

PRIVATE S16 rgSCHCmnSpsUlFreeToBeRelUeLst ARGS((
RgSchCellCb           *cell
));

PRIVATE S16 rgSCHCmnSpsUlFreeActvUeLst ARGS((
RgSchCellCb           *cell
));

PRIVATE Void rgSCHCmnSpsUlFillPdcchWithAlloc ARGS((
RgSchPdcch      *pdcch,
RgSchUlAlloc    *alloc,
RgSchUeCb       *ue
));
PRIVATE Void rgSCHCmnSpsUlUeFillAllocInfo ARGS((
RgSchCellCb      *cell,
RgSchUeCb        *ue
));
PRIVATE Void rgSCHCmnSpsDlFillActvPdcchDciFrmt2A ARGS((
RgSchCellCb                *cell,
RgSchCmnDlUeSpsInfo        *dlSpsUe,
RgSchDlRbAlloc             *rbAllocInfo,
RgSchPdcch                 *pdcch,
Bool                       isSpsOcc,
U8                         hqProcId
));
PRIVATE Void rgSCHCmnSpsDlFillActvPdcchDciFrmt1 ARGS((
RgSchCellCb                *cell,
RgSchCmnDlUeSpsInfo        *dlSpsUe,
RgSchDlRbAlloc             *rbAllocInfo,
RgSchPdcch                 *pdcch,
Bool                       isSpsOcc,
U8                         hqProcId
));
PRIVATE Void rgSCHCmnSpsDlFillActvPdcchDciFrmt2 ARGS((
RgSchCellCb                *cell,
RgSchCmnDlUeSpsInfo        *dlSpsUe,
RgSchDlRbAlloc             *rbAllocInfo,
RgSchPdcch                 *pdcch,
Bool                       isSpsOcc,
U8                         hqProcId
));
PRIVATE Void rgSCHCmnSpsDlFillActvPdcchDciFrmt1A ARGS((
RgSchCellCb                *cell,
RgSchCmnDlUeSpsInfo        *dlSpsUe,
RgSchDlRbAlloc             *rbAllocInfo,
RgSchPdcch                 *pdcch,
Bool                       isSpsOcc,
U8                         hqProcId
));

PRIVATE Bool rgSCHCmnSpsUlChkUlSf ARGS((
RgSchCellCb *cell, 
RgSchUlSf   *ulSf
));
/**
 * @brief Initializes the SPS related global data structures
 *
 * @details
 *     Function : rgSCHCmnSpsInit
 *
 *     Invoking Module Processing:
 *      - Common SCH shall invoke this API at initialization.
 *       
 *     Processing Steps:
 *     - Compute LCM table for SPS.
 *     
 *  @param[in] Void
 *
 *  @return Void 
 **/
#ifdef ANSI
PUBLIC Void rgSCHCmnSpsInit
(
)
#else /* ANSI */
PUBLIC Void rgSCHCmnSpsInit () 
#endif /* ANSI */
{
   TRC2(rgSCHCmnSpsInit);
   /* Compute the LCM table for SPS operation */
   rgSCHCmnSpsCompLcmTbl();

   RETVOID;
}

/* 
 * SPS specific functions 
 */

/**
 * @brief Initializes the SPS scheduler for the cell.
 *
 * @details
 *     Function : rgSCHCmnSpsCellCfg
 *
 *     Invoking Module Processing:
 *      - Common SCH shall invoke this API at cell configuration.
 *       
 *     Processing Steps:
 *     - Invoke rgSCHCmnSpsDlCellCfg for DL SPS
 *     - Invoke rgSCHCmnSpsUlCellCfg for UL SPS
 *
 *   @param[in]  RgSchCellCb   *cell
 *   @param[in]  RgrCellCfg    *cellCfg
 *   @param[out] RgSchErrInfo  *err
 *  @return  S16
 *      -# ROK
 *      -# RFAILED
 **/
#ifdef ANSI
PUBLIC S16 rgSCHCmnSpsCellCfg
(
RgSchCellCb        *cell,
RgrCellCfg         *cellCfg,
RgSchErrInfo       *err
)
#else /* ANSI */
PUBLIC S16 rgSCHCmnSpsCellCfg (cell, cellCfg, err) 
RgSchCellCb        *cell;
RgrCellCfg         *cellCfg;
RgSchErrInfo       *err;
#endif /* ANSI */
{
   S16             ret       = ROK;

   
   TRC2(rgSCHCmnSpsCellCfg);    

   /* Validate DL SPS configuration for the cell */
   if ((ret = rgSCHCmnSpsVldtDlCellCfg(cell, cellCfg)) != ROK)
   {
      RLOG_ARG0(L_ERROR,DBG_CELLID,cell->cellId, 
               "rgSCHCmnSpsDlCellCfg: Validation failed for cell");
      RETVALUE(RFAILED);
   }

   /* Validate UL SPS configuration for the cell */
   if ((ret = rgSCHCmnSpsUlVldtCellCfg(cell, cellCfg)) != ROK)
   {
      RLOG_ARG0(L_ERROR,DBG_CELLID,cell->cellId,
               "rgSCHCmnSpsUlVldtCellCfg: Validation failed for cell%d");
      RETVALUE(RFAILED);
   }

   /* Initialize DL SPS specific information for the cell */
   if ((ret = rgSCHCmnSpsDlCellCfg(cell, cellCfg, err)) != ROK)
   {
      RLOG_ARG0(L_ERROR,DBG_CELLID,cell->cellId,
             "rgSCHCmnSpsDlCellCfg failed for cell");
      RETVALUE(RFAILED);
   }

   /* Initialize UL SPS specific information for the cell */
   if ((ret = rgSCHCmnSpsUlCellCfg(cell, cellCfg)) != ROK)
   {
      RLOG_ARG0(L_ERROR,DBG_CELLID,cell->cellId,
         "rgSCHCmnSpsUlCellCfg failed for cell");
      RETVALUE(RFAILED);
   }

   RETVALUE(ret);
}

/**
 * @brief Delete the SPS context for the cell at cell delete
 *
 * @details
 *
 *     Function : rgSCHCmnSpsCellDel
 *     
 *     Invoking Module Processing:
 *      - SCH shall invoke this at cell delete.
 *  
 *
 *     Processing Steps:
 *      - Invoke rgSCHCmnSpsDlCellDel
 *      - Invoke rgSCHCmnSpsUlCellDel
 *      - Return Void
 *
 *  
 *  @param[in]  RgSchCellCb *cell
 *  @return  Void
 **/
#ifdef ANSI
PUBLIC Void rgSCHCmnSpsCellDel 
(
RgSchCellCb        *cell
)
#else /* ANSI */
PUBLIC Void rgSCHCmnSpsCellDel (cell) 
RgSchCellCb        *cell;
#endif /* ANSI */
{

   TRC2(rgSCHCmnSpsCellDel);
       

   /* Delete DL SPS specific information */
   rgSCHCmnSpsDlCellDel(cell);
   
   /* Delete UL SPS specific information */
   rgSCHCmnSpsUlCellDel(cell);

   RETVOID;
}

/**
 * @brief Initializes UE with SPS specific information.
 *
 * @details
 *
 *     Function : rgSCHCmnSpsUeCfg
 *
 *     Invoking Module Processing:
 *      - Common SCH shall invoke this if DL SPS is enabled for the UE
 *        at UE configuration.
 *      - SCH CMN shall validate the range for the configured values
 *      - SCH CMN shall invoke this after the configuration for measurement
 *      gap, DRX, SRS, CQI and SR periodicity and offset has been updated in 
 *      the UeCb.
 *        
 *
 *     Processing Steps:
 *      - If DL SPS is enabled,
 *        - Validate the configuration with respect to SPS.
 *          Call rgSCHCmnSpsVldtDlUeCfg().
 *        - If validation fails
 *           - Return RFAILED.
 *        - else
 *           - Invoke ggSCHCmnSpsDlUeCfg
 *      - If (ueCfg->ueSpsCfg.ulSpsCfg.isUlSpsEnabled == TRUE)
 *        - Call rgSCHCmnSpsUlVldtUeCfg()
 *        - if validation fails
 *           - Return RFAILED
 *        - else
 *           - Invoke rgSCHCmnSpsUlUeCfg
 *
 *  @param[in]   RgSchCellCb   *cell
 *  @param[in]   RgSchUeCb     *ue
 *  @param[in]   RgrUeCfg      *ueCfg
 *   @param[out] RgSchErrInfo  *err
 *  @return  S16
 *      -# ROK
 **/
#ifdef ANSI
PUBLIC S16 rgSCHCmnSpsUeCfg 
(
RgSchCellCb        *cell,
RgSchUeCb          *ue,
RgrUeCfg           *ueCfg,
RgSchErrInfo       *err
)
#else /* ANSI */
PUBLIC S16 rgSCHCmnSpsUeCfg (cell, ue, ueCfg, err) 
RgSchCellCb        *cell;
RgSchUeCb          *ue;
RgrUeCfg           *ueCfg;
RgSchErrInfo       *err;
#endif /* ANSI */
{

   TRC2(rgSCHCmnSpsUeCfg);
       
   /* Note: Assumed that the ue is memset to 0 before call to this function */
   /* Perform validations for DL SPS if DL SPS is enabled */
   if (ueCfg->ueSpsCfg.dlSpsCfg.isDlSpsEnabled)
   {


      if (ROK != rgSCHCmnSpsVldtDlUeCfg(cell, ue, ueCfg))
      {
         RLOG_ARG1(L_ERROR,DBG_CELLID,cell->cellId,
                  "Validation failed for DL SPS UE CRNTI:%d", 
                  (int)ueCfg->crnti);
         RETVALUE(RFAILED);
      }
      /* Perform initialization for DL SPS enabled UE */
      rgSCHCmnSpsDlUeCfg(cell, ue, ueCfg, err);

   }

   /* Perform validations for UL SPS if UL SPS is enabled */
   if (ueCfg->ueSpsCfg.ulSpsCfg.isUlSpsEnabled)
   {

      if (ROK != rgSCHCmnSpsUlVldtUeCfg(cell, ueCfg))
      {
         RLOG_ARG1(L_ERROR,DBG_CELLID,cell->cellId,
                  "Validation failed for UL SPS UE config CRNTI:%d", 
                  (int)ueCfg->crnti);
         RETVALUE(RFAILED);
      }
      /* Perform initialization for UL SPS enabled UE */
      if (ROK != rgSCHCmnSpsUlUeCfg(cell, ue, ueCfg))
      {
         RLOG_ARG1(L_ERROR,DBG_CELLID,cell->cellId,
                  "Initialization failed for UL SPS UE config CRNTI:%d", 
                  (int)ueCfg->crnti);
         RETVALUE(RFAILED);
      }
   }

   RETVALUE(ROK);
}

/**
 * @brief Re-initializes SPS context for the UE at UE reconfiguration.
 *
 * @details
 *
 *     Function : rgSCHCmnSpsUeRecfg
 *
 *     Invoking Module Processing:
 *      - Common SCH shall invoke this at UE reconfiguration.
 *         
 *     Processing Steps:
 *      - If DL SPS has been reconfigured, call rgSCHCmnSpsDlUeRecfg.
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
PUBLIC S16 rgSCHCmnSpsUeRecfg 
(
RgSchCellCb        *cell,
RgSchUeCb          *ue,
RgrUeRecfg         *ueRecfg,
RgSchErrInfo       *err
)
#else /* ANSI */
PUBLIC S16 rgSCHCmnSpsUeRecfg (cell, ue, ueRecfg, err) 
RgSchCellCb        *cell;
RgSchUeCb          *ue;
RgrUeRecfg         *ueRecfg;
RgSchErrInfo       *err;
#endif /* ANSI */
{

   S16             ret;
   
   TRC2(rgSCHCmnSpsUeRecfg);


   /* DL SPS reconfigured */
   ret = rgSCHCmnSpsDlUeRecfg(cell, ue, ueRecfg, err);
   if (ret != ROK)
   {
      RLOG_ARG1(L_ERROR,DBG_CELLID,cell->cellId,
               "rgSCHCmnSpsDlUeRecfg failed: cell CRNTI:%d",ue->ueId);
      RETVALUE(RFAILED);
   }

   /* UL SPS reconfigured */
   if ((rgSCHCmnSpsUlUeRecfg(cell, ue, ueRecfg)) != ROK)
   {
      RLOG_ARG1(L_ERROR,DBG_CELLID,cell->cellId,
               "rgSCHCmnSpsUlUeRecfg failed: cellId CRNTI:%d",ue->ueId);
      RETVALUE(RFAILED);
   }

   if ((ue->dl.dlSpsCfg.isDlSpsEnabled == FALSE) &&
      (ue->ul.ulSpsCfg.isUlSpsEnabled == FALSE))
   {
      if (NULLP  != rgSCHDbmGetSpsUeCb (cell, ue->spsRnti))
      {
         rgSCHDbmDelSpsUeCb(cell, ue);
      }
   }

   RETVALUE(ROK);
}

/**
 * @brief Clears SPS context of the UE.
 *
 * @details
 *
 *     Function : rgSCHCmnSpsUeDel
 *
 *     Invoking Module Processing:
 *      - Common SCH shall invoke this at delete of SPS enabled UE.
 *
 *     Processing Steps:
 *     - Check the configured n1Pucch and if this is the last UE on the
 *     configured n1Pucch, free the corresponding index
 *     - Remove UE from the lists, if it exists
 *     - Free Ue specific information for SPS 
 *     - Return ROK
 *
 * @param[in]  RgSchCellCb *cell
 * @param[in]  RgSchUeCb   *ue
 * @return  Void
 **/
#ifdef ANSI
PUBLIC Void rgSCHCmnSpsUeDel 
(
RgSchCellCb      *cell,
RgSchUeCb        *ue
)
#else /* ANSI */
PUBLIC Void rgSCHCmnSpsUeDel (cell, ue) 
RgSchCellCb      *cell;
RgSchUeCb        *ue;
#endif /* ANSI */
{

   RgSchCmnDlUeSpsInfo *dlSpsUe = RG_SCH_CMN_SPS_GET_DL_UE(ue);

   TRC2(rgSCHCmnSpsUeDel);
       

   if (NULLP  != rgSCHDbmGetSpsUeCb (cell, ue->spsRnti))
   {
      rgSCHDbmDelSpsUeCb(cell, ue);
   }

   /* Clear DL SPS specific information for a DL SPS enabled UE */
   if (ue->dl.dlSpsCfg.isDlSpsEnabled)
   {
      rgSCHCmnSpsDlUeSuspend(cell, ue, TRUE);
      cmMemset((U8 *)dlSpsUe, 0, sizeof(*dlSpsUe));
   }

   /* Delete UL SPS specific information */
   if (ue->ul.ulSpsCfg.isUlSpsEnabled)
   {
      rgSCHCmnSpsUlUeDel(cell, ue);
   }

   RETVOID;
}

/**
 * @brief Downlink CQI Indication handler 
 *
 * @details
 *
 *     Function : rgSCHCmnSpsDlCqiIndHndlr
 *
 *     Invoking Module Processing:
 *      - Common SCH shall invoke this if UE is DL SPS enabled upon recieving
 *      DL CQI indication from PHY.
 *        
 *     Processing Steps:
 *     -  If (!ue->isSpsActv), return ROK
 *     - else
 *       - if (reported_cqi != old_cqi) 
 *         - newCQi  = RgSchCmnSpsGetDlCqi(reported_cqi)
 *         - Determine number of RBs needed at new CQi for the
 *         configured SPS TB Size.
 *         - If ue->isSpsActv
 *            - If new_rbs_needed != ue->spsAllocInfo->rbsAlloc, mark UE to be
 *              reactivated.
 *              - Set ue->actionPndng |= RG_SCH_CMN_SPS_DL_REACTV_FREQ
 *
 *  @param[in]  RgSchCellCb     *cell
 *  @param[in]  RgSchUeCb       *ue
 *  @param[in]  TfuDlCqiRpt     *cqiIndInfo
 *  @param[in]  CmLteTimingInfo timingInfo
 *  @return  Void
 **/

#ifdef ANSI
PUBLIC Void rgSCHCmnSpsDlCqiIndHndlr
(
RgSchCellCb          *cell,
RgSchUeCb            *ue,
TfuDlCqiRpt          *cqiIndInfo,
CmLteTimingInfo      timingInfo
)
#else /* ANSI */
PUBLIC Void rgSCHCmnSpsDlCqiIndHndlr(cell, ue, cqiIndInfo, timingInfo)
RgSchCellCb          *cell;
RgSchUeCb            *ue;
TfuDlCqiRpt          *cqiIndInfo;
CmLteTimingInfo      timingInfo;
#endif /* ANSI */
{

   RgSchCmnDlUe        *cmnUeDl = RG_SCH_CMN_GET_DL_UE(ue);
   RgSchCmnDlUeSpsInfo *dlSpsUe = RG_SCH_CMN_SPS_GET_DL_UE(ue);
   U8                  scCqi;
   RgSchCmnDlSvcSpsInfo  *dlSpsSvc = NULLP;
   U32                 spsBo;
   U16                 rlcHdrEst;
   RgSchCmnSpsDlUeSchdInfo *dlSpsUeSchdInfo = &(dlSpsUe->dlSpsUeSchdInfo);
   
   TRC2(rgSCHCmnSpsDlCqiIndHndlr);
   /* Update the values only if ue is SPS active */
   if (dlSpsUe->dlSpsStatus == RGSCH_DL_SPS_ACTV)
   {
      
      /* Note: Assumed CQI is updated before invoking this API */
      scCqi = rgSCHCmnSpsDlGetScaledCqi(ue, cmnUeDl->mimoInfo.cwInfo[0].cqi);

      /* scaledCqi should be updated value for SPS active UE */
      if (scCqi != dlSpsUe->dlSpsUeSchdInfo.scaledCqi)
      {
         U8 rb, mcs, itbs;
         U32 tbs;
         
         /* Compute number of RBs needed for new CQI */
         dlSpsSvc = RG_SCH_CMN_SPS_GET_DL_SVC(dlSpsUe->spsSvc); /* SPS_KW_FIX */

         spsBo = dlSpsUe->spsSchedBo + RG_SCH_DL_SPS_ADDTL_BO;
         RG_SCH_CMN_DL_GET_HDR_EST(dlSpsUe->spsSvc, rlcHdrEst);
         spsBo += rlcHdrEst;

         rgSCHCmnClcRbAlloc(cell, spsBo,
               scCqi, &rb, &tbs, &mcs, &itbs, TRUE, 
               rgSCHUtlSubFrmGet(cell, dlSpsUeSchdInfo->schdKey));
         if (rb != dlSpsUe->dlSpsUeSchdInfo.spsAllocInfo.rbsAlloc)
         {
            /* Re-activation needed due to change in number of required Rbs */
            /* Mark UE for reactivation */
            dlSpsUe->actionPndng |= RG_SCH_CMN_SPS_DL_REACTV_FREQ;

#ifdef RGSCH_SPS_STATS
            numReactivationsCqi++;
            numReactivations++;
#endif
         }
         dlSpsUe->dlSpsUeSchdInfo.scaledCqi = scCqi;
      } 
   }
   RETVOID;
}

/**
 * @brief Configuration for SPS service
 * @details
 *
 *     Function : rgSCHCmnSpsDlLcRecfg
 *
 *     Invoking Module Processing:
 *     - Common SCH shall invoke this at a DL LC Reconfiguration for sps enabled
 *     service
 *
 *     Processing Steps:
 *      - Do validations:
 *          - Validate if the UE has SPS enabled
 *          - Validate if this is the first SPS service for the UE 
 *          - If validation fails, return RFAILED.
 *      - Update the configured values for the LC
 *      - Initialize SPS specific values
 *      - Assign the SPS LC pointer to the UE
 *
 *  @param[in]  RgrSchCellCb     *cell
 *  @param[in]  RgSchUeCb        *ue
 *  @param[in]  RgSchDlLcCb      *dlLc
 *  @param[in]  RgrLchRecfg        *lcCfg
 *  @param[in]  RgSchErrInfo     *err
 *  @return  S16
 *      -# ROK
 *      -# RFAILED
 **/
#ifdef ANSI
PUBLIC S16 rgSCHCmnSpsDlLcRecfg
(
RgSchCellCb        *cell,
RgSchUeCb          *ue,
RgSchDlLcCb        *dlLc,
RgrLchRecfg        *lcRecfg,
RgSchErrInfo       *err
)
#else /* ANSI */
PUBLIC S16 rgSCHCmnSpsDlLcRecfg(cell, ue, dlLc, lcRecfg, err)
RgSchCellCb        *cell;
RgSchUeCb          *ue;
RgSchDlLcCb        *dlLc;
RgrLchRecfg        *lcRecfg;
RgSchErrInfo       *err;
#endif /* ANSI */
{
   RgSchCmnDlUeSpsInfo  *dlSpsUe   = RG_SCH_CMN_SPS_GET_DL_UE(ue);

   TRC2(rgSCHCmnSpsDlLcRecfg);
       


   /* Check if one SPS service is already existing for the UE */
   if (dlSpsUe->spsSvc)
   {
      RLOG_ARG2(L_ERROR,DBG_CELLID,cell->cellId,
               "Only one SPS service is supported: attempt to configure more"
               " than 1 SPS service: CRNTI:%d, LCID:%d", (int)ue->ueId, 
               (int)lcRecfg->lcId);
      RETVALUE(RFAILED);
   }

   /* Note: Assumed that the variables shall be memset to 0
    * during allocation */

   dlSpsUe->spsSvc = dlLc;

   /* Update the configured values */
   dlLc->dlLcSpsCfg = lcRecfg->dlRecfg.dlSpsRecfg;

   RETVALUE(ROK);
}


/**
 * @brief Configuration for SPS service
 * @details
 *
 *     Function : rgSCHCmnSpsDlLcCfg
 *
 *     Invoking Module Processing:
 *     - Common SCH shall invoke this at a DL LC configuration for sps enabled
 *     service
 *
 *     Processing Steps:
 *      - Do validations:
 *          - Validate if the UE has SPS enabled
 *          - Validate if this is the first SPS service for the UE 
 *          - If validation fails, return RFAILED.
 *      - Update the configured values for the LC
 *      - Initialize SPS specific values
 *      - Assign the SPS LC pointer to the UE
 *
 *  @param[in]  RgrSchCellCb     *cell
 *  @param[in]  RgSchUeCb        *ue
 *  @param[in]  RgSchDlLcCb      *dlLc
 *  @param[in]  RgrLchCfg        *lcCfg
 *  @param[in]  RgSchErrInfo     *err
 *  @return  S16
 *      -# ROK
 *      -# RFAILED
 **/
#ifdef ANSI
PUBLIC S16 rgSCHCmnSpsDlLcCfg
(
RgSchCellCb        *cell,
RgSchUeCb          *ue,
RgSchDlLcCb        *dlLc,
RgrLchCfg          *lcCfg,
RgSchErrInfo       *err
)
#else /* ANSI */
PUBLIC S16 rgSCHCmnSpsDlLcCfg(cell, ue, dlLc, lcCfg, err)
RgSchCellCb        *cell;
RgSchUeCb          *ue;
RgSchDlLcCb        *dlLc;
RgrLchCfg          *lcCfg;
RgSchErrInfo       *err;
#endif /* ANSI */
{
   RgSchCmnDlUeSpsInfo  *dlSpsUe   = RG_SCH_CMN_SPS_GET_DL_UE(ue);

   TRC2(rgSCHCmnSpsDlLcCfg);
       

   

   /* Check if one SPS service is already existing for the UE */
   if (dlSpsUe->spsSvc)
   {
      RLOG_ARG2(L_ERROR,DBG_CELLID,cell->cellId,
               "Only one SPS service is supported: attempt to configure more"
               " than 1 SPS service: CRNTI:%d, LCID:%d", (int)ue->ueId, 
               (int)lcCfg->lcId);
      RETVALUE(RFAILED);
   }

   /* Note: Assumed that the variables shall be memset to 0
    * during allocation */

   dlSpsUe->spsSvc = dlLc;

   /* Update the configured values */
   dlLc->dlLcSpsCfg = lcCfg->dlInfo.dlSpsCfg;

   RETVALUE(ROK);
} 

/**
 * @brief Deletion for SPS service
 *
 * @details
 *
 *     Function : rgSCHCmnSpsDlLcDel
 *
 *     Processing Steps:
 *     - If LC is SPS enabled,
 *       - if (ue->isSpsActv)
 *         - Add UE to toBeRelUes queue for SPS Release
 *         - Remove UE from active list of UEs 
 *       - Remove UE from activation/reactivation SPS Queues
 *       - Set ue->spsSvc = NULLP
 *
 *  @param[in]  RgrSchCellCb     *cell
 *  @param[in]  RgSchUeCb        *ue
 *  @param[in]  RgSchDlLcCb      *dlLc
 *  @return  Void
 **/
#ifdef ANSI
PUBLIC Void rgSCHCmnSpsDlLcDel
(
RgSchCellCb        *cell,
RgSchUeCb          *ue,
RgSchDlLcCb        *dlLc
)
#else /* ANSI */
PUBLIC Void rgSCHCmnSpsDlLcDel(cell, ue, dlLc)
RgSchCellCb        *cell;
RgSchUeCb          *ue;
RgSchDlLcCb        *dlLc;
#endif /* ANSI */
{

   RgSchCmnDlCellSpsInfo *dlSpsCell = RG_SCH_CMN_SPS_GET_DL_CELL(cell);
   RgSchCmnDlUeSpsInfo   *dlSpsUe   = RG_SCH_CMN_SPS_GET_DL_UE(ue);
   RgSchCmnDlSvcSpsInfo  *dlSpsSvc  = RG_SCH_CMN_SPS_GET_DL_SVC(dlLc);

   TRC2(rgSCHCmnSpsDlLcDel);

   if (dlSpsUe->spsSvc != dlLc)
   {
      /* Non-SPS service cannot be deleted by SPS module */
      RETVOID;
   }

   /* Delete SPS service from DL SPS UE */
   if (dlSpsUe->spsSvc)
   {
      if (dlSpsSvc->toBeSchdSvcEnt.node)
      {
         rgSCHCmnSpsUtlDlDelFrmToBeSchdSvcs(dlSpsCell, dlSpsUe->spsSvc);
      }
      if (dlSpsUe->actvUeLstEnt.node)
      {
         rgSCHCmnSpsUtlDlDelFrmActvUes(dlSpsCell, ue, 
                                dlSpsUe->dlSpsUeSchdInfo.schdKey);
      }
   }

   /* Remove UE from toBeActvtdUes or toBeRelUes list */
   if (dlSpsUe->spsList)
   {
      if (dlSpsUe->spsList == &dlSpsCell->toBeActvtdUes)
      {
         rgSCHCmnSpsUtlDlDelFrmToBeActvtdUes(dlSpsCell, ue);
      }
      else
      {
         rgSCHCmnSpsUtlDlDelFrmToBeRelUes(dlSpsCell, ue);
      }
   }

   /* If UE is SPS active, clear the allocation of the UE from the cell-wide
    * subframes */
   if (dlSpsUe->dlSpsStatus == RGSCH_DL_SPS_ACTV)
   {
      rgSCHCmnSpsDlUeClearN1PucchAlloc(cell, ue, FALSE);
      rgSCHCmnSpsDlUeClearAlloc(cell, ue);
      RGSCH_SPS_SET_CLEAR_ALLC_HIST(ue);
      rgSCHCmnSpsUtlDlAdd2ToBeRelUes(dlSpsCell,ue);      
      RGSCH_SPS_SET_DL_SPS_STATE(ue, RGSCH_DL_SPS_RELEASE_TRIGGERED);

   }
   else
   {

      RGSCH_SPS_SET_DL_SPS_STATE(ue, RGSCH_DL_SPS_IDLE);
   }

   cmMemset((U8 *)dlSpsSvc, 0, sizeof(*dlSpsSvc));
   dlSpsUe->spsSvc = NULLP;

   RETVOID;
} 

/**
 * @brief BO update trigger for SPS
 *
 * @details
 *
 *     Function : rgSCHCmnSpsDlDedBoUpd
 *
 *     Invoking Module Processing:
 *     - Common SCH shall invoke this on reception of BO update from the UE
 *     for an SPS enabled service.
 *     - It is expected that the Common SCH shall update the bo in lcCb before
 *     invoking this API
 *     - if (svc->isSpsEnabled), Invoke this API
 *     - else, common SCH shall call bo update on other schedulers
 * 
 *     Processing Steps:
 *     - If (bo)
 *       - If svc does not exist in toBeSchdSvcs list && !(ue->isSpsActv)
 *          - Add to the toBeSchdSvcs list
 *       - If (bo <= svc->packetSize  && !(ue->isSpsActv))
 *          - Add UE to the toBeActvtdUes list
 *     -else
 *        - If svc exist in toBeSchdSvcs list, remove from the list 
 *     
 *
 *  @param[in]  RgSchCellCb            *cell
 *  @param[in]  RgSchUeCb              *ue
 *  @param[in]  RgSchDlLcCb            *dlLc
 *  @return  Void
 **/
#ifdef ANSI
PUBLIC Void rgSCHCmnSpsDlDedBoUpd
(
RgSchCellCb              *cell,
RgSchUeCb                *ue,
RgSchDlLcCb              *dlLc
)
#else /* ANSI */
PUBLIC Void rgSCHCmnSpsDlDedBoUpd(cell, ue, dlLc)
RgSchCellCb              *cell;
RgSchUeCb                *ue;
RgSchDlLcCb              *dlLc;
#endif /* ANSI */
{
   RgSchCmnDlCellSpsInfo *dlSpsCell = RG_SCH_CMN_SPS_GET_DL_CELL(cell);
   RgSchCmnDlSvcSpsInfo  *dlSpsSvc = RG_SCH_CMN_SPS_GET_DL_SVC(dlLc);
   RgSchCmnDlUeSpsInfo   *dlSpsUe  = RG_SCH_CMN_SPS_GET_DL_UE(ue);
   U16 rlcHdrEst = 0;
#ifdef RGSCH_SPS_STATS
   U32 statBoIndex = 0;
#endif

   /* Check if BO update is recieved for UE's SPS service only: this check is
    * important if SPS is disabled for UE but still enabled for LC */
   TRC2(rgSCHCmnSpsDlDedBoUpd);
       
#if (ERRCLASS & ERRCLS_DEBUG)
   if (dlSpsUe->spsSvc != dlLc)
   {
      RLOG_ARG2(L_ERROR,DBG_CELLID,cell->cellId,
               "Bo update recieved for Non-Sps service CRNTI:%d LCID:%d", 
                ue->ueId,dlLc->lcId);
      RETVOID;
   }
#endif /*(ERRCLASS & ERRCLS_DEBUG)*/
   
   /* Update effective BO of SPS service */
   dlSpsSvc->effSpsBo = dlLc->bo;
   
   /* Handling for BO != 0 */
   if (dlSpsSvc->effSpsBo)
   { 
#ifdef RGSCH_SPS_STATS     
      ue->lastSpsLcBoRptTime = cell->crntTime;
      ue->absTimeBo = glblTtiCnt;


      spsBoValueStats[numSPSBoUpd % RGSCH_SPS_MAX_BO_STATS] = dlSpsSvc->effSpsBo;
      /* SPS STATS */
      statBoIndex = dlLc->bo/100;
      if (statBoIndex < 20)
      { 
         spsBoSizeStats[statBoIndex]++;
      }

      if (dlSpsUe->prevDlBoUpdTm.subframe != RGSCH_INVALID_SUBFRAME)
      {
         statBoIndex = ((RGSCH_CALC_SF_DIFF(cell->crntTime, dlSpsUe->prevDlBoUpdTm))/10);

         spsBoIntrValueStats[numSPSBoUpd % RGSCH_SPS_MAX_BO_STATS] = 
            RGSCH_CALC_SF_DIFF(cell->crntTime, dlSpsUe->prevDlBoUpdTm);

         if (statBoIndex < 20)
         { 
            spsBoIntrvStats[statBoIndex]++;
         }
         else
         {
            spsBoIntrvStats[20]++;
         }
      }
      if (dlSpsUe->dlSpsStatus < 6)
      {
         spsBoUpdState[dlSpsUe->dlSpsStatus]++;
      }
      numSPSBoUpd++;    
#endif      

      RG_SCH_CMN_DL_GET_HDR_EST(dlSpsUe->spsSvc, rlcHdrEst);

      switch (dlSpsUe->dlSpsStatus)
      {
         case RGSCH_DL_SPS_WAITING_FOR_ACTV_CFM:
         case RGSCH_DL_SPS_RELEASE_TRIGGERED:
         {
            /* If Activation PDCCH is already sent but Harq Ack is 
               yet to be received for the PDCCH sent and BO upd is recvd
               for SPS LC
               OR
               If SPS Release is already triggered wait for the process
               to finish, Until then 
               Then: 
                  Schedule the BO by dynamic scheduling */
            rgSCHCmnSpsUtlDlAdd2ToBeSchdSvcs(dlSpsCell, dlLc); 
            break;
         }
         case RGSCH_DL_SPS_ACTVTN_PENDING:
         {
            if (((dlSpsUe->prevDlBoUpdTm.sfn != cell->crntTime.sfn) &&
                  (dlSpsUe->prevDlBoUpdTm.subframe != cell->crntTime.subframe))||
                  (dlLc->bo > dlSpsUe->spsSchedBo))
            {
               /* Scheduler was unable to activate DL SPS for this UE before the
                  reception of next BO update for SPS Svc. Hence pending BO
                  needs to be scheduled dynamically. Since all the remaining BO shall
                  be scheduled dynamically, remove the UE's entry from to be actvd list */
               rgSCHCmnSpsUtlDlAdd2ToBeSchdSvcs(dlSpsCell, dlLc);
               rgSCHCmnSpsUtlDlDelFrmToBeActvtdUes(dlSpsCell, ue);
               RGSCH_SPS_SET_DL_SPS_STATE(ue, RGSCH_DL_SPS_IDLE);
            }
            break;
         }
         case RGSCH_DL_SPS_ACTV:
         {
            if ((dlSpsSvc->effSpsBo + RG_SCH_DL_SPS_ADDTL_BO + rlcHdrEst) > 
                  dlSpsUe->dlSpsUeSchdInfo.spsAllocInfo.tbInfo[0].bytesAlloc)
            {
               /* If BO received is greater than SPS scheduled TB size */
               dlSpsUe->dynSchedCount++;
               dlSpsUe->reducedBoCount = 0;

               if (dlSpsSvc->effSpsBo > dlSpsUe->maxChgdBo)
               {
                  dlSpsUe->maxChgdBo = dlSpsSvc->effSpsBo;
               }

               if (dlSpsUe->dynSchedCount == RG_SCH_SPS_CONS_DYN_SCHD)
               {
                  /* Reactivate SPS with Max of changed BO */
                  dlSpsUe->actionPndng |= RG_SCH_CMN_SPS_DL_REACTV_FREQ;
                  dlSpsUe->spsSchedBo = dlSpsUe->maxChgdBo;
                  dlSpsUe->dynSchedCount = 0;
                  dlSpsUe->maxChgdBo = 0;

#ifdef RGSCH_SPS_STATS
                  numReactivations++;
                  numReactivationsBo++;
#endif
               }
               else
               {  /* dynSchedCount < RG_SCH_SPS_CONS_DYN_SCHD
                     Hence, Schedule BO dynamically */
                  rgSCHCmnSpsUtlDlAdd2ToBeSchdSvcs(dlSpsCell, dlLc);
                  dlSpsUe->isDynSched = TRUE;
               }
            }
            else if (dlSpsSvc->effSpsBo == dlSpsUe->spsSchedBo)
            {
               dlSpsUe->dynSchedCount = 0;
               dlSpsUe->reducedBoCount = 0;
               dlSpsUe->maxChgdBo = 0;
            }
            else
            {
               /* BO scheduled using SPS Occasion */
               dlSpsUe->dynSchedCount = 0;

               if ((!dlSpsUe->reducedBoCount) ||
                     (dlSpsSvc->effSpsBo > dlSpsUe->maxChgdBo))
               {
                  dlSpsUe->maxChgdBo = dlSpsSvc->effSpsBo;
               }
               dlSpsUe->reducedBoCount++;

               if (dlSpsUe->reducedBoCount == RG_SCH_SPS_CONS_RED_BO)
               {
                  /* Reactivate SPS with Max of changed BO */
                  dlSpsUe->actionPndng |= RG_SCH_CMN_SPS_DL_REACTV_FREQ;
                  dlSpsSvc->effSpsBo = dlSpsUe->maxChgdBo;
                  dlSpsUe->reducedBoCount= 0;
                  dlSpsUe->maxChgdBo = 0;
#ifdef RGSCH_SPS_STATS
                  numReactivationsBo++;
                  numReactivations++;
#endif
               }            
            }         
            break;
         }
         case RGSCH_DL_SPS_IDLE:
         {
            /* Else if SPS is not activated already */
            if ((NULLP == dlSpsSvc->toBeSchdSvcEnt.node) &&
               (dlSpsUe->prevDlBoUpdTm.subframe != RGSCH_INVALID_SUBFRAME))
            {            
               /* stub to disable dl sps */
               if ((dlSpsSvc->effSpsBo < RGSCH_SPS_MAX_SPS_ACTVN_BO) &&
                     (ue->dl.dlSpsCfg.dlSpsPrdctyEnum < 
                      RG_SCH_SPS_SID_INTERVAL) && 
                     ((RGSCH_CALC_SF_DIFF(cell->crntTime, 
                         dlSpsUe->prevDlBoUpdTm)) < RG_SCH_SPS_SID_INTERVAL))
               {
                  /* Speech Good Packet */
                  dlSpsUe->actionPndng |= RG_SCH_CMN_SPS_DL_ACTV; 
                  ue->dl.isSpsHqPInUse = TRUE;
                  /* Set UE status to be SPS Pending Activation */ 
                  RGSCH_SPS_SET_DL_SPS_STATE(ue, RGSCH_DL_SPS_ACTVTN_PENDING);
                  rgSCHCmnSpsUtlDlAdd2ToBeActvtdUes(dlSpsCell, ue);
                  dlSpsUe->spsSchedBo = dlLc->bo;

                  RLOG_ARG3(L_DEBUG,DBG_CELLID,cell->cellId,
                           "DL SPS Activation criteria satisfied for "
                           "UEID:%d sfn %d sf %d",
                           ue->ueId, 
                           cell->crntTime.sfn, 
                           cell->crntTime.subframe );
               }
               else
               {
                  /* SID packet*/
                  rgSCHCmnSpsUtlDlAdd2ToBeSchdSvcs(dlSpsCell, dlLc);
               }
            }
            else
            {
               /* First BO update */
               rgSCHCmnSpsUtlDlAdd2ToBeSchdSvcs(dlSpsCell, dlLc);
            }
            break;
         }
         default:
         RLOG_ARG2(L_ERROR,DBG_CELLID,cell->cellId,
                  " CRNTI:%d Invalid State !! LCID:%d", ue->ueId,dlLc->lcId);
         /*Invalid State */
      }
      dlSpsUe->prevDlBoUpdTm.sfn = cell->crntTime.sfn;
      dlSpsUe->prevDlBoUpdTm.subframe = cell->crntTime.subframe;
   }
   else
   {
      /* Handling for BO == 0 */
      if(dlSpsSvc->toBeSchdSvcEnt.node)
      {
         rgSCHCmnSpsUtlDlDelFrmToBeSchdSvcs(dlSpsCell, dlLc); 
      } 
   }
   RETVOID;
}

/**
 * @brief UE Reset
 *
 * @details
 *
 *     Function : rgSCHCmnSpsDlUeReset
 *
 *     Processing Steps:
 *     - Clear the configured grant for the UE.
 *     - Reset bo for SPS service of UE.
 *     - Remove UE and SPS service from all queues.
 *
 *  @param[in]  RgSchCellCb        *cell
 *  @param[in]  RgSchUeCb          *ue
 *  @return  Void
 **/
#ifdef ANSI
PUBLIC Void rgSCHCmnSpsDlUeReset
(
RgSchCellCb         *cell,
RgSchUeCb           *ue
)
#else /* ANSI */
PUBLIC Void rgSCHCmnSpsDlUeReset(cell, ue)
RgSchCellCb         *cell;
RgSchUeCb           *ue;
#endif /* ANSI */
{
   /* Note:Invoked for DL SPS enabled UE only and before invoking other
    * schedulers to reset the UE */
   TRC2(rgSCHCmnSpsDlUeReset);
       
   rgSCHCmnSpsDlUeSuspend(cell, ue, FALSE);
   RETVOID;
}

/**
 * @brief Handler for handling release of sps hq proc
 *
 * @details
 *
 *     Function : rgSCHCmnSpsRelHqProc
 *
 *
 *  @param[in]  RgSchCellCb              *cell
 *  @param[in]  RgSchDlHqProcCb          *hqP
 *  @return  Void
 **/
#ifdef ANSI
PRIVATE Void rgSCHCmnSpsRelHqProc
(
RgSchCellCb           *cell,
RgSchDlHqProcCb       *hqP
)
#else /* ANSI */
PRIVATE Void rgSCHCmnSpsRelHqProc(cell, hqP)
RgSchCellCb           *cell;
RgSchDlHqProcCb       *hqP;
#endif /* ANSI */
{
   RgSchUeCb            *ue = hqP->hqE->ue;
   Pst             pst;
   RgInfRlsHqInfo  *rlsHqBufs = &(cell->rlsHqArr[cell->crntHqIdx]); 
   TRC2(rgSCHCmnSpsRelHqProc);

   hqP->tbInfo[0].cntrRetxAllocFail = RG_SCH_MAX_RETX_ALLOC_FAIL;
   rgSCHDhmRlsHqpTb(hqP, 0, TRUE);
   if (hqP->tbInfo[1].txCntr)
   {
      hqP->tbInfo[1].cntrRetxAllocFail = RG_SCH_MAX_RETX_ALLOC_FAIL;
      rgSCHDhmRlsHqpTb(hqP, 1, TRUE);
   }

   /* Fill Release harq sent to MAC to free SPS Hq procs */
   rlsHqBufs->ueHqInfo[0].status[0] = TRUE;
   rlsHqBufs->ueHqInfo[0].tbId[0] = 1;
   rlsHqBufs->ueHqInfo[0].numOfTBs = 1;
   rlsHqBufs->ueHqInfo[0].rnti = ue->ueId;
   rlsHqBufs->ueHqInfo[0].hqProcId = hqP->procId;
   rlsHqBufs->numUes = 1;

   rgSCHUtlGetPstToLyr(&pst, &rgSchCb[cell->instIdx], cell->macInst);
   RgSchMacRlsHq (&pst, rlsHqBufs);

   RETVOID;
}


/**
 * @brief Handler for proc max retx case for DL SPS hq proc
 *
 * @details
 *
 *     Function : rgSCHCmnSpsHandleDlMaxRetx
 *
 *
 *  @param[in]  RgSchCellCb              *cell
 *  @param[in]  RgSchDlHqProcCb          *hqP
 *  @return  Void
 **/
#ifdef ANSI
PRIVATE Void rgSCHCmnSpsHandleDlMaxRetx
(
RgSchCellCb           *cell,
RgSchDlHqProcCb       *hqP,
Bool                  *isMaxRetxHit
)
#else /* ANSI */
PRIVATE Void rgSCHCmnSpsHandleDlMaxRetx(cell, hqP, isMaxRetxHit)
RgSchCellCb           *cell;
RgSchDlHqProcCb       *hqP;
Bool                  *isMaxRetxHit;
#endif /* ANSI */
{
   CmLteTimingInfo       schdTime;
   RgSchCmnDlHqProc     *cmnHqDl = RG_SCH_CMN_GET_DL_HQP(hqP);
   RgSchUeCb            *ue = hqP->hqE->ue;
   S8       ret;
   RgSchCmnDlCellSpsInfo *dlSpsCell;
   RgSchCmnDlUeSpsInfo   *dlSpsUe  = RG_SCH_CMN_SPS_GET_DL_UE(hqP->hqE->ue);
   TRC2(rgSCHCmnSpsHandleDlMaxRetx);

   dlSpsCell = RG_SCH_CMN_SPS_GET_DL_CELL(cell);
   RG_SCH_ADD_TO_CRNT_TIME(cell->crntTime, schdTime, RG_SCH_CMN_DL_DELTA);
   *isMaxRetxHit = FALSE;

   /* If HARQ proc corresponds to SPS transmission, check for the 
    * maximum time for re-transmission */
   if (cmnHqDl->isSpsActv)
   {         
      RGSCH_TIMEINFO_CMP(schdTime, cmnHqDl->maxRetxTime, &ret);
      if ((ret >= 0) ||
            (hqP->tbInfo[0].txCntr == RGSCH_SPS_MAX_DL_HQP_TX))
      {
         /* Current time is greater than maximum time of re-transmission: */
         /* Max Retx hit for HqP with which Activation PDCCH was sent
            Hence Consider SPS Activation unsuccessful */
         if (dlSpsUe->dlSpsStatus == RGSCH_DL_SPS_WAITING_FOR_ACTV_CFM)
         {
            if ((cmnHqDl->spsAction & RG_SCH_CMN_SPS_DL_ACTV) ||
                  (cmnHqDl->spsAction & RG_SCH_CMN_SPS_DL_REACTV))
            {               

#ifdef RGSCH_SPS_STATS
               rgNumSPSSchedDropMaxRetx++;
               rgNumActDtx++;
#endif

               rgSCHCmnSpsDlUeClearN1PucchAlloc(cell, ue, FALSE);
               rgSCHCmnSpsDlUeClearAlloc(cell, ue);
               RGSCH_SPS_SET_CLEAR_ALLC_HIST(ue);
               /*Intg fix */
               rgSCHCmnSpsUtlDlAdd2ToBeRelUes(dlSpsCell,ue);      
               RGSCH_SPS_SET_DL_SPS_STATE(ue, RGSCH_DL_SPS_RELEASE_TRIGGERED);
               RLOG_ARG1(L_DEBUG,DBG_CELLID,cell->cellId,
                        "Max retx... Nack recvd for SPS Actv Hq "
                        "prc moving state to IDLE UEID:%d",
                        ue->ueId);
            }
         }
         /* Remove the HARQ proc from retxHqProcs list */
         rgSCHCmnSpsUtlDlDelFrmRetxHqProcs(dlSpsCell, hqP);
         rgSCHCmnSpsRelHqProc(cell, hqP);
         *isMaxRetxHit = TRUE;
      }
   }

   RETVOID;
}


/**
 * @brief Handler for proc requiring re-transmission.
 *
 * @details
 *
 *     Function : rgSCHCmnSpsDlProcAddToRetx
 *
 *     Processing Steps:
 *     - Handles HARQ procs pending for re-transmission with SPS data.
 *     - Add hqP to spsCell->retxHqProcs
 *
 *  @param[in]  RgSchCellCb              *cell
 *  @param[in]  RgSchDlHqProcCb          *hqP
 *  @return  Void
 **/
#ifdef ANSI
PUBLIC Void rgSCHCmnSpsDlProcAddToRetx
(
RgSchCellCb           *cell,
RgSchDlHqProcCb       *hqP
)
#else /* ANSI */
PUBLIC Void rgSCHCmnSpsDlProcAddToRetx(cell, hqP)
RgSchCellCb           *cell;
RgSchDlHqProcCb       *hqP;
#endif /* ANSI */
{
   RgSchCmnDlCellSpsInfo *dlSpsCell;
   Bool isMaxRetxHit = FALSE;
   TRC2(rgSCHCmnSpsDlProcAddToRetx);
   dlSpsCell = RG_SCH_CMN_SPS_GET_DL_CELL(cell);

   /* Integration_fix */   
   /* Setting cntrRetxAllocFail to MAX value here */
   /* Since the hqP entry is already deleted from inUse list of HqEntity
      setting the value here will ensure the entry is not deleted
      again during release harq proc */
   hqP->tbInfo[0].cntrRetxAllocFail = RG_SCH_MAX_RETX_ALLOC_FAIL;
   if (hqP->tbInfo[1].txCntr)
   {
      hqP->tbInfo[1].cntrRetxAllocFail = RG_SCH_MAX_RETX_ALLOC_FAIL;
   }

   rgSCHCmnSpsHandleDlMaxRetx(cell, hqP, &isMaxRetxHit);

   if (FALSE == isMaxRetxHit)
   {
      /* Add to the list of re-transmitting HARQ SPS procs in the cell */
      rgSCHCmnSpsUtlDlAdd2RetxHqProcs(dlSpsCell, hqP);
   }

   RETVOID;
}
/**
 * @brief Handler for Releasing DL SPS resources and making SPS IDLE
 *
 * @details
 *
 *     Function : rgSCHCmnSpsRelDlSpsActHqP
 *      
 *  @param[in]  RgSchCellCb              *cell
 *  @param[in]  RgSchDlHqProcCb          *hqP
 *  @return  Void
 **/
#ifdef ANSI
PUBLIC Void rgSCHCmnSpsRelDlSpsActHqP
(
RgSchCellCb            *cell,
RgSchDlHqProcCb        *hqP
)
#else /* ANSI */
PUBLIC Void rgSCHCmnSpsRelDlSpsActHqP(cell, hqP)
RgSchCellCb            *cell;
RgSchDlHqProcCb        *hqP;
#endif /* ANSI */
{
   RgSchUeCb             *ue = hqP->hqE->ue;
   RgSchCmnDlUeSpsInfo *dlSpsUe = RG_SCH_CMN_SPS_GET_DL_UE(ue);
   RgSchCmnDlCellSpsInfo *dlSpsCell = RG_SCH_CMN_SPS_GET_DL_CELL(cell);
   TRC2(rgSCHCmnSpsRelDlSpsActHqP);

#ifdef RGSCH_SPS_STATS
   rgNumActDtx++;
#endif
   if (dlSpsUe->dlSpsStatus == RGSCH_DL_SPS_WAITING_FOR_ACTV_CFM)
   {
      rgSCHCmnSpsDlUeClearN1PucchAlloc(cell, ue, FALSE);
      rgSCHCmnSpsDlUeClearAlloc(cell, ue);
      RGSCH_SPS_SET_CLEAR_ALLC_HIST(ue);
      rgSCHCmnSpsUtlDlDelFrmRetxHqProcs(dlSpsCell, hqP);
      /* Intg_fix */
      rgSCHCmnSpsUtlDlAdd2ToBeRelUes(dlSpsCell,ue);      
      RGSCH_SPS_SET_DL_SPS_STATE(ue, RGSCH_DL_SPS_RELEASE_TRIGGERED);
   }

   RETVOID;
}


/**
 * @brief Handler for processing Ack recieved for a HARQ process
 *
 * @details
 *
 *     Function : rgSCHCmnSpsDlProcAck
 *
 *     Processing Steps:
 *     - If dlSpsStatus is ACTV PDCCH SENT
 *          set dlSpsStatus as SPS ACTIVE      
 *  @param[in]  RgSchCellCb              *cell
 *  @param[in]  RgSchDlHqProcCb          *hqP
 *  @return  Void
 **/
#ifdef ANSI
PUBLIC Void rgSCHCmnSpsDlProcAck
(
RgSchCellCb            *cell,
RgSchDlHqProcCb        *hqP
)
#else /* ANSI */
PUBLIC Void rgSCHCmnSpsDlProcAck(cell, hqP)
RgSchCellCb            *cell;
RgSchDlHqProcCb        *hqP;
#endif /* ANSI */
{
   RgSchUeCb             *ue = hqP->hqE->ue;
   RgSchCmnDlHqProc      *cmnHqDl = RG_SCH_CMN_GET_DL_HQP(hqP);

   TRC2(rgSCHCmnSpsDlProcAck);
   
   if ((RG_SCH_CMN_SPS_DL_IS_SPS_TX_HQP(hqP)))
   {
      if ((cmnHqDl->spsAction & RG_SCH_CMN_SPS_DL_ACTV) ||
            (cmnHqDl->spsAction & RG_SCH_CMN_SPS_DL_REACTV))
      {
         RGSCH_SPS_SET_DL_SPS_STATE(ue, RGSCH_DL_SPS_ACTV);
         RLOG_ARG4(L_DEBUG,DBG_CELLID,cell->cellId,
                  "DL SPS ACTIVATED succesfully for UEID:%d Ack on hqP %d "
                  "at SFN(%d) subframe(%d)",
                  ue->ueId,
                  hqP->procId,
                  cell->crntTime.sfn, 
                  cell->crntTime.subframe);
#ifdef RGSCH_SPS_STATS
         numActSuccess++;
#endif
      }
#ifdef RGSCH_SPS_STATS
      numSPSSchedAcked++;
#endif
   }

   RETVOID;
}



/**
 * @brief Handler for processing feedback recieved for release PDCCH
 *
 * @details
 *
 *     Function : rgSCHCmnSpsDlRelPdcchFbk
 *
 *     Processing Steps:
 *     - If ACK
 *       - Remove UE from toBeRelUes list
 *       - Clear UE specific SPS allocation information
 *        
 *  @param[in]  RgSchCellCb   *cell
 *  @param[in]  RgSchUeCb     *ue       
 *  @param[in]  Bool          isAck
 *
 *  @return  Void
 **/
#ifdef ANSI
PUBLIC Void rgSCHCmnSpsDlRelPdcchFbk
(
RgSchCellCb           *cell,
RgSchUeCb             *ue,
Bool                  isAck
)
#else /* ANSI */
PUBLIC Void rgSCHCmnSpsDlRelPdcchFbk(cell, ue, isAck)
RgSchCellCb           *cell;
RgSchUeCb             *ue;
Bool                  isAck;
#endif /* ANSI */
{
   /* Assumed that feedback for release PDCCH shall be sent at index 0. 
    * from PHY */
   RgSchCmnDlCellSpsInfo *dlSpsCell = RG_SCH_CMN_SPS_GET_DL_CELL(cell);
   RgSchCmnDlUeSpsInfo   *dlSpsUe = RG_SCH_CMN_SPS_GET_DL_UE(ue);
   
   CmLteTimingInfo       relFdbkTime;

   TRC2(rgSCHCmnSpsDlRelPdcchFbk);
  
    
   if ((isAck == TFU_HQFDB_ACK) ||
           (dlSpsUe->numRelPdcchSent >= cell->dlHqCfg.maxDlHqTx)) /* SPS_REVIEW */
   {
      /* Release successful for SPS UE */

      /* Remove UE from to be released UEs list */
      rgSCHCmnSpsUtlDlDelFrmToBeRelUes(dlSpsCell, ue);
      
      /* Set the state to SPS IDLE */ 
      RGSCH_SPS_SET_DL_SPS_STATE(ue, RGSCH_DL_SPS_IDLE);
     

       RLOG_ARG4(L_DEBUG,DBG_CELLID,cell->cellId,
               "DL SPS Ack received for Release PDCCH or maxhqtx "
               "sent %d max %d at SFN(%d), sunframe(%d) UEID:%d",
               dlSpsUe->numRelPdcchSent ,
               cell->dlHqCfg.maxDlHqTx, 
               cell->crntTime.sfn, 
               ue->ueId);
       RLOG_ARG2(L_DEBUG,DBG_CELLID,cell->cellId,
               "subframe(%d) UEID:%d",
               cell->crntTime.subframe,
               ue->ueId);
      /* If Ue's SPS service has BO, add to toBeSchdSvcs list */
      if (dlSpsUe->spsSvc)
      {
         RgSchCmnDlSvc  *spsSvcCmn = RG_SCH_CMN_GET_DL_SVC(dlSpsUe->spsSvc);
         if(spsSvcCmn->dlSvcSpsInfo.effSpsBo)
         {
            rgSCHCmnSpsUtlDlAdd2ToBeSchdSvcs(dlSpsCell, dlSpsUe->spsSvc);
         }
      }
      
   }

   /* SPS_INTG_FIX */
   /* Note: No special handling needed for NACK and DTX: re-transmission of
    * relPdcch taken care through usual toBeRel UE list */
   dlSpsUe->isRelPdcchSent = FALSE;

   
   /* UE is added to wtngForRelFdbkUeLst at 
    * expected fdb time + TFU_HQFBKIND_ULDELTA time.
    * At this time if the UE still
    * exists in wtngForRelFdbkUeLst, we know for sure
    * UE has not received any feedback
    * crntTime is expected fdbk time + delta that PHY takes
    * to deliver the Harq. However, we add to wtngForRelFdbkUeLst
    * 1 tti after this time. 
    * Therefore,we add 1 to determine the correct index of insertion */
   //RG_SCH_ADD_TO_CRNT_TIME(cell->crntTime,relFdbkTime,1);
   /* Integration_fix */
   if (dlSpsUe->wtngForRelFdbkUeEnt.node)
   {
      RG_SCH_ADD_TO_CRNT_TIME(ue->relPdcchFbkTiming,relFdbkTime,TFU_HQFBKIND_ULDELTA); 
      cmLListDelFrm(&(dlSpsCell->wtngForRelFdbkUeLst[relFdbkTime.subframe]),
            &dlSpsUe->wtngForRelFdbkUeEnt);
      dlSpsUe->wtngForRelFdbkUeEnt.node = NULLP;
   }

   /* Reset the transmission time of relPdcch */
   ue->relPdcchTxTime.sfn = RGSCH_MAX_SFN + 1;
   /* Reset release PDCCH feedback timing */
   ue->relPdcchFbkTiming.sfn = RGSCH_MAX_SFN + 1;

   RETVOID;
}

#ifdef RG_UNUSED
/* Invoked by HD-FDD module to get list of DL SPS active UEs in a TTI */
/**
 * @brief Determines the list of UEs Active for this sub-frame
 *
 * @details
 *
 *     Function : rgSCHCmnSpsGetDlActvUe
 *
 *     Invoking Module Processing:
 *     - This API shall be invoked to get list of UEs active for DL SPS
 *        
 *     Processing Steps:
 *     - Compute the index (idx) into cell->actvDlSpsUeLsts
 *     - Set *dlSpsActvUeLst = cell->actvDlSpsUeLsts[idx]
 *     - Return ROK
 *
 *  @param[in]  RgSchCellCb  *cell
 *  @param[in]  CmLteTimingInfo  timingInfo
 *  @param[out] CmLListCp    *dlSpsActvUeLst
 *  @return  S16
 *      -# ROK
 **/
#ifdef ANSI
PUBLIC S16 rgSCHCmnSpsGetDlActvUe
(
RgSchCellCb     *cell,
CmLteTimingInfo *timingInfo,
CmLListCp       *dlSpsActvUeLst
)
#else /* ANSI */
PUBLIC S16 rgSCHCmnSpsGetDlActvUe(cell, timingInfo, dlSpsActvUeLst)
RgSchCellCb     *cell;
CmLteTimingInfo *timingInfo;
CmLListCp       *dlSpsActvUeLst;
#endif /* ANSI */
{
   U16                   idx;
   RgSchCmnDlCellSpsInfo *dlSpsCell = RG_SCH_CMN_SPS_GET_DL_CELL(cell);
   
   TRC2(rgSCHCmnSpsGetDlActvUe);
   /* Note: Assumed that the list will be used for read-only purpose */

   idx = (timingInfo->sfn * 10 + timingInfo->subframe) % 
      RG_SCH_CMN_SPS_MAX_PRD;

   *dlSpsActvUeLst = dlSpsCell->actvDlSpsUeLsts[idx];

   RETVALUE(ROK);
}
#endif /* RG_UNUSED */

/**
 * @brief Trigger for DL scheduling for SPS UEs.
 *
 * @details
 *
 *     Function : rgSCHCmnSpsDlSched
 *
 *     Invoking Module Processing:
 *      - Common SCH shall invoke this at TTI for SPS scheduling in DL
 *      sub-frame.
 *      - This API shall be invoked before invoking TTI processing API for
 *      other schedulers
 *        
 *     Processing Steps:
 *     - Process UEs with configured grant for the target DL subframe. Call
 *       rgSCHCmnSpsDlProcActvtdUes()
 *     - Process UEs with release pending for the target DL subframe.Call 
 *       rgSCHCmnSpsDlProcToBeRelUes()
 *     - Process UEs with activation pending for the target DL subframe.Call 
 *       rgSCHCmnSpsDlProcToBeActvtdUes()
 *     - Process UEs with pending re-transmissions. Call
 *       rgSCHCmnSpsDlProcRetxUes()
 *     - Process UEs with services pending for Non-SPS scheduling.Call 
 *       rgSCHCmnSpsDlProcToBeSchdSvcs()
 *
 *  @param[in]  RgSchCellCb *cell
 *  @param[out] RgSchCmnDlRbAllocInfo *allocInfo,
 *  @return  Void
 **/
#ifdef ANSI
PUBLIC Void rgSCHCmnSpsDlSched
(
RgSchCellCb           *cell,
RgSchCmnDlRbAllocInfo *allocInfo
)
#else /* ANSI */
PUBLIC Void rgSCHCmnSpsDlSched(cell, allocInfo)
RgSchCellCb           *cell;
RgSchCmnDlRbAllocInfo *allocInfo;
#endif /* ANSI */
{

   CmLteTimingInfo       crntTime;
   RgSchDlSf             *dlSf;
   CmLteTimingInfo       schdTime;
   
   TRC2(rgSCHCmnSpsDlSched);

#ifdef RGSCH_SPS_STATS
   rgSCHCmnSpsPrintStats(cell);
#endif
   if (0 == cell->spsCellCfg.maxSpsUePerDlSf)
   {
      RETVOID;
   }
   /* Get the current time for the cell */ 
   crntTime = cell->crntTime;

   /* Get Downlink Subframe */
   dlSf = allocInfo->dedAlloc.dedDlSf;

   dlSf->isSPSOcc = FALSE;


   /* Handle re-transmitting UEs */
   rgSCHCmnSpsDlProcRetxUes(cell, crntTime, dlSf, allocInfo);

   /* Handle already activated UEs*/
   rgSCHCmnSpsDlProcActvtdUes(cell, crntTime, dlSf, allocInfo);

   /* Handle to be released UEs */
   rgSCHCmnSpsDlProcToBeRelUes(cell, crntTime, dlSf);

   /* Handle to be activated UEs */
   rgSCHCmnSpsDlProcToBeActvtdUes(cell, crntTime, dlSf);

   /* Handle to be scheduled services */
   rgSCHCmnSpsDlProcToBeSchdSvcs(cell, crntTime, dlSf, allocInfo);


   /* Increment by DL DELTA to determine the time for which scheduling
    * is done */
   RG_SCH_ADD_TO_CRNT_TIME(crntTime, schdTime, RG_SCH_CMN_DL_DELTA);

   /* Update subframe-wide allocation information with SPS allocation */
   rgSCHCmnSpsDlUpdDlSfAllocWithSps(cell, schdTime, dlSf);

   /* Update subframe-wide allocation information with SPS allocation
      not Utilized in this Subf due to zero BO */
   rgSCHCmnSpsDlUpdDlSfAllocWithZeroBoSvcs(cell, dlSf);
 

   RETVOID;
}


/**
 * @brief Update DL SF with SPS allocation information in the sub-frame
 *
 * @details
 *
 *     Function : rgSCHCmnSpsDlUpdDlSfAllocWithZeroBoSvcs
 *
 *
 *     Processing Steps:
 *     
 *  @param[in]  RgSchCellCb     *cell
 *  @param[in]  RgSchDlSf       *dlSf
 *  @return  Void
 **/
#ifdef ANSI
PRIVATE Void rgSCHCmnSpsDlUpdDlSfAllocWithZeroBoSvcs
(
RgSchCellCb           *cell,
RgSchDlSf             *dlSf
)
#else /* ANSI */
PRIVATE Void rgSCHCmnSpsDlUpdDlSfAllocWithZeroBoSvcs(cell, crntTime, dlSf)
RgSchCellCb           *cell;
RgSchDlSf             *dlSf;
#endif /* ANSI */
{
   RgSchCmnDlCellSpsInfo *dlSpsCell = RG_SCH_CMN_SPS_GET_DL_CELL(cell);
   CmLList               *listNode  = dlSpsCell->zeroBOSvcUes.first;
   RgSchDlRbAlloc        *spsAllocInfo = NULLP;
   RgSchUeCb             *ue;
   RgSchCmnDlUeSpsInfo   *dlSpsUe;
   U8                    idx;   

   TRC2(rgSCHCmnSpsDlUpdDlSfAllocWithZeroBoSvcs);


   while (listNode)
   {
      ue       = (RgSchUeCb *)(listNode->node);
      dlSpsUe = RG_SCH_CMN_SPS_GET_DL_UE(ue);
      spsAllocInfo = &dlSpsUe->dlSpsUeSchdInfo.spsAllocInfo;
      listNode = listNode->next;

      /* Update allocation from DL SPS SF to dlSf */
      dlSf->dlSfAllocInfo.raType0Mask  ^= 
               spsAllocInfo->resAllocInfo.raType0Mask;
#ifdef RGSCH_SPS_UNUSED
      for (idx = 0; idx < RG_SCH_NUM_RATYPE1_32BIT_MASK; idx++)
      {
         dlSf->dlSfAllocInfo.raType1Mask[idx] ^= 
            spsAllocInfo->resAllocInfo.raType1Mask[idx];
         dlSf->dlSfAllocInfo.raType1UsedRbs[idx] ^= 
            spsAllocInfo->resAllocInfo.raType1UsedRbs[idx];
      }
#endif
      for (idx = 0; idx < RG_SCH_NUM_RATYPE2_32BIT_MASK; idx++)
      {
         dlSf->dlSfAllocInfo.raType2Mask[idx] ^= 
            spsAllocInfo->resAllocInfo.raType2Mask[idx];
      }

      dlSf->spsAllocdBw -= spsAllocInfo->rbsAlloc;

      rgSCHCmnSpsUtlDlDelFrmZeroBOSvcUes(dlSpsCell,ue);
   }

   RETVOID;
}

/**
 * @brief Update DL SF with SPS allocation information in the sub-frame
 *
 * @details
 *
 *     Function : rgSCHCmnSpsDlUpdDlSfAllocWithSps
 *
 *
 *     Processing Steps:
 *     
 *  @param[in]  RgSchCellCb     *cell
 *  @parama[in] CmLteTimingInfo crntTime
 *  @param[in]  RgSchDlSf       *dlSf
 *  @return  Void
 **/
#ifdef ANSI
PUBLIC Void rgSCHCmnSpsDlUpdDlSfAllocWithSps
(
RgSchCellCb           *cell,
CmLteTimingInfo       schdTime,
RgSchDlSf             *dlSf
)
#else /* ANSI */
PUBLIC Void rgSCHCmnSpsDlUpdDlSfAllocWithSps(cell, schdTime, dlSf)
RgSchCellCb           *cell;
CmLteTimingInfo       schdTime;
RgSchDlSf             *dlSf;
#endif /* ANSI */
{
   U16                   dlSpsSfIdx;   
   RgSchCmnDlCellSpsInfo *dlSpsCell = RG_SCH_CMN_SPS_GET_DL_CELL(cell);
   RgSchCmnSpsDlSf       *spsSf;
   U8                    idx;   

   TRC2(rgSCHCmnSpsDlUpdDlSfAllocWithSps);


   /* Determine DL SPS subframe index */
   dlSpsSfIdx = (schdTime.sfn * 10 + schdTime.subframe) %
      dlSpsCell->spsPrdLcmVal;
   spsSf = &dlSpsCell->spsSfTbl[dlSpsSfIdx];

   /* Update allocation from DL SPS SF to dlSf */
   dlSf->dlSfAllocInfo.raType0Mask        |= spsSf->spsAllocInfo.raType0Mask;
#ifdef RGSCH_SPS_UNUSED
   for (idx = 0; idx < RG_SCH_NUM_RATYPE1_32BIT_MASK; idx++)
   {
      dlSf->dlSfAllocInfo.raType1Mask[idx] |= 
         spsSf->spsAllocInfo.raType1Mask[idx];
      dlSf->dlSfAllocInfo.raType1UsedRbs[idx] |= 
         spsSf->spsAllocInfo.raType1UsedRbs[idx];
   }
   dlSf->dlSfAllocInfo.nxtRbgSubset = spsSf->spsAllocInfo.nxtRbgSubset;
#endif
   for (idx = 0; idx < RG_SCH_NUM_RATYPE2_32BIT_MASK; idx++)
   {
      dlSf->dlSfAllocInfo.raType2Mask[idx] |= 
         spsSf->spsAllocInfo.raType2Mask[idx];
   }

   dlSf->spsAllocdBw = spsSf->rbsAlloc;
   RETVOID;
}


/**
 * @brief Handles Retransmitting SPS UEs
 *
 * @details
 *
 *     Function : rgSCHCmnSpsDlProcRetxUes
 *
 *
 *     Processing Steps:
 *     - This API handles retransmitting HARQ SPS procs. Retransmitting SPS HARQ
 *     procs include all HARQ procs with isSpsSvcSchd = TRUE.
 *     - For each hqProc in retxHqProcs,
 *        - if (current_time > hqP->maxRetxTime)
 *           - Delete the HARQ proc from retxHqProcs list
 *           - Free the HARQ proc
 *           - continue
 *        - Determine the RBs needed for the re-transmission based on the tbSize
 *        - Add to the re-transmitting SPS queue in allocInfo 
 *     
 *  @param[in]  RgSchCellCb     *cell
 *  @parama[in] CmLteTimingInfo crntTime
 *  @param[in]  RgSchDlSf       *dlSf
 *  @param[out] RgSchCmnDlRbAllocInfo *allocInfo
 *  @return  Void
 **/
#ifdef ANSI
PRIVATE Void rgSCHCmnSpsDlProcRetxUes
(
RgSchCellCb           *cell,
CmLteTimingInfo       crntTime,
RgSchDlSf             *dlSf,
RgSchCmnDlRbAllocInfo *allocInfo
)
#else /* ANSI */
PRIVATE Void rgSCHCmnSpsDlProcRetxUes(cell, crntTime, dlSf, allocInfo)
RgSchCellCb           *cell;
CmLteTimingInfo       crntTime;
RgSchDlSf             *dlSf;
RgSchCmnDlRbAllocInfo *allocInfo;
#endif /* ANSI */
{
   Bool                 isMaxRetxHit = FALSE;
   CmLListCp            *retxLst;
   CmLList              *node;
   RgSchDlHqProcCb      *hqP;
   RgSchCmnDlUe         *cmnUeDl;
   RgSchCmnDlHqProc     *cmnHqDl;
   U32                  effBo;
   RgSchUeCb            *ue = NULLP;
   RgSchCmnDlCellSpsInfo *dlSpsCell = RG_SCH_CMN_SPS_GET_DL_CELL(cell);
   CmLteTimingInfo       schdTime;
   RgSchCmnDlUeSpsInfo   *dlSpsUe;
   RgSchCmnSpsDlUeSchdInfo *dlSpsUeSchdInfo;
   S8       ret;
   S16      numRb;

   TRC2(rgSCHCmnSpsDlProcRetxUes);


   /* Increment by DL DELTA to determine the time for which scheduling
    * is done */
   RG_SCH_ADD_TO_CRNT_TIME(crntTime, schdTime, RG_SCH_CMN_DL_DELTA);

   retxLst = &dlSpsCell->retxHqProcs;
   /* allocate bw for the retransmission..should be same are previous */
   /* If CQI gets worse, as we cannot find same TB size for another   */
   /* MCS, we just remove this from the retransmission queue          */
   node = retxLst->first;
   while (dlSf->remUeCnt && (node != NULLP))
   {
      hqP = (RgSchDlHqProcCb *)node->node;
      cmnHqDl = RG_SCH_CMN_GET_DL_HQP(hqP);
      node = node->next;
      ue = hqP->hqE->ue;

      /* If this is an SPS Occasion then New transmission is given priority
              over retx */
      dlSpsUe = RG_SCH_CMN_SPS_GET_DL_UE(ue);
      dlSpsUeSchdInfo = &(dlSpsUe->dlSpsUeSchdInfo);


      /* If HARQ proc corresponds to SPS transmission, check for the 
       * maximum time for re-transmission */
      rgSCHCmnSpsHandleDlMaxRetx(cell, hqP, &isMaxRetxHit);
      
      if (TRUE == isMaxRetxHit)
      {
         continue;
      }

      RGSCH_TIMEINFO_CMP(schdTime, dlSpsUeSchdInfo->schdKey, &ret);
      if (ret == 0)
      {
         continue;
      }
      /* Check if this UE is already scheduled for transmission/
       * active to be scheduled */
      if (rgSCHCmnSpsDlChkUeInActv(cell, ue, schdTime))
      {
         continue;
      }
      cmnUeDl = RG_SCH_CMN_GET_DL_UE(ue);

      numRb = hqP->tbInfo[0].dlGrnt.numRb;
      if ( numRb > (S16)(dlSf->bw - dlSf->bwAssigned))
      {
         /* If its not possible to alloc RB for SPS RETX then it shall 
            end the retx */
         rgSCHCmnSpsUtlDlDelFrmRetxHqProcs(dlSpsCell, hqP);
         rgSCHCmnSpsRelHqProc(cell, hqP);
#ifdef RGSCH_SPS_STATS
         rgSchSpsRetxAllcFailCnt++;
#endif
         continue;

      } 

      effBo = 0;

      if (rgSCHCmnDlAllocRetxRb(cell, dlSf, ue, 0, &effBo, hqP, allocInfo) 
            != ROK)
      {
         /* SF/RETX Bandwidth expired */
         RETVOID;
      }
      if (effBo == 0)
      {
         continue;
      }
      /* Mark HARQ Proc to have invalid SPS n1Pucch resource */
      hqP->spsN1PucchRes.pres = FALSE;
      cmnUeDl->proc = hqP;
      
      /* Mark UE to be scheduled by SPS module */
      ue->dl.dlAllocCb.spsSchdTime = schdTime;
      rgSCHCmnSpsUtlDlAdd2RetxSpsUeLst(allocInfo, hqP);
      dlSf->remUeCnt--;
   }
   RETVOID;
}


/**
 * @brief Handles Activated SPS UEs
 *
 * @details
 *
 *     Function : rgSCHCmnSpsDlProcActvtdUes
 *
 *
 *     Processing Steps:
 *     - If UE is HD-FDD, 
 *       - Check if the subframe corrsponding to 'frm' is DL subframe 
 *         else return. Invoke API/MACRO on HD-FDD.
 *     - If UE has measurement gap enabled, 
 *       - Check if UE can be scheduled in DL else continue.
 *         Invoke API/MACRO on measurement gap module.
 *     - If UE has DRX enabled, 
 *       - Check if UE can be scheduled in DL else continue.
 *         Invoke API/MACRO on DRX module.
 *     - Determine the index into the cell->actvDlSpsUeLsts using following
 *       - Determine frm for (cell->crntTime + DL_DELTA)
 *       - (frm.sfn * 10 + frm.subframe) % RG_SCH_CMN_SPS_MAX_PRD
 *     - for each Ue in the list,
 *       - if (ue->spsSvc == NULL)
 *        {
 *           - Service might have been deleted and ue should be awaiting
 *             release
 *           - Remove UE from toBeActvtdUes list
 *           - Continue to next UE
 *        }
 *       - if (ue->spsSvc->bo == 0)
 *        {
 *           - ue->spsSvc->zeroBoOcassionCnt++
 *           - if (ue->spsSvc->zeroBoOcassionCnt == svc->explicitRelCnt)
 *             - Add UE to toBeRelUes list
 *             - Remove UE from toBeActvtdUes list
 *           - return
 *        }
 *       - Reset zeroBoOcassionCnt
 *       - if (ue->spsSvc->bo > ue->spsSvc->packetSize)
 *        {
 *           - Schedule UE using C-RNTI
 *           - Call rgSCHCmnSpsDlAllocForSvc()
 *        }
 *       - if (ue->spsSvc->bo <= ue->spsSvc->packetSize)
 *        {
 *           - Schedule UE using SPS-RNTI
 *           - If successful,
 *             - Get SPS harq Proc. Call rgSCHDhmSpsDlGetHqProc()
 *             - Check if (ue->actionPndng  == RG_SCH_CMN_SPS_DL_REACTV_FREQ)
 *             {
 *                - Reactivate UE with change in number and position of RBs
 *                - Call rgSCHCmnSpsDlProcToBeReactvtdUe() 
 *                - continue 
 *             }
 *             else
 *             {
 *                - Check if (ue->pdcchPndng), fetch PDCCH and update the
 *                  information in pdcch
 *             }
 *             - Update DHM with logical channel's data information
 *             - Update the HARQ proc with the allocation information
 *             - Set hqP->isSpsActv = TRUE
 *             - Add HARQ proc to the subframe.
 *             - Update the maximumRetxTime in the hqP
#ifdef LTE_TDD
 *             - increment numCfgGrntTx in the UE's dlSpsInfo
#endif
 *        }
 *  @param[in]  RgSchCellCb           *cell
 *  @parama[in] CmLteTimingInfo       crntTime
 *  @param[in]  RgSchDlSf             *dlSf
 *  @param[out] RgSchCmnDlRbAllocInfo *allocInfo
 *  @return  Void
 **/
#ifdef ANSI
PRIVATE Void rgSCHCmnSpsDlProcActvtdUes
(
RgSchCellCb           *cell,
CmLteTimingInfo       crntTime,
RgSchDlSf             *dlSf,
RgSchCmnDlRbAllocInfo *allocInfo
)
#else /* ANSI */
PRIVATE Void rgSCHCmnSpsDlProcActvtdUes(cell, crntTime, dlSf, allocInfo)
RgSchCellCb           *cell;
CmLteTimingInfo       crntTime;
RgSchDlSf             *dlSf;
RgSchCmnDlRbAllocInfo *allocInfo;
#endif /* ANSI */
{
   RgSchCmnDlCellSpsInfo *dlSpsCell = RG_SCH_CMN_SPS_GET_DL_CELL(cell);
   RgSchCmnDlUeSpsInfo   *dlSpsUe;
   RgSchCmnSpsDlUeSchdInfo *dlSpsUeSchdInfo;
   RgSchCmnDlSvcSpsInfo  *dlSpsSvc;
   U32                   spsPrd;
   CmLList               *listNode;
   CmLteTimingInfo       schdTime;
   U16                   actvtdLstIdx;
   RgSchUeCb             *ue;
   U8                    n1PucchIdx;
   Bool                  isActvOrReactv = FALSE;
#ifdef LTE_TDD
   Bool                  forceSched = FALSE;
#endif
   TRC2(rgSCHCmnSpsDlProcActvtdUes);
   

   /* Increment by DL DELTA to determine the time for which scheduling
    * is done */
   RG_SCH_ADD_TO_CRNT_TIME(crntTime, schdTime, RG_SCH_CMN_DL_DELTA);
   actvtdLstIdx = (schdTime.sfn * 10 + schdTime.subframe) % 
      RG_SCH_CMN_SPS_MAX_PRD;
   listNode = dlSpsCell->actvDlSpsUeLsts[actvtdLstIdx].first;

   while (dlSf->remUeCnt && listNode)
   {
#ifdef LTE_TDD
      forceSched = FALSE;
#endif      
      ue       = (RgSchUeCb *)(listNode->node);
      dlSpsUe = RG_SCH_CMN_SPS_GET_DL_UE(ue);
      dlSpsUeSchdInfo = &(dlSpsUe->dlSpsUeSchdInfo);
      spsPrd = (U32)ue->dl.dlSpsCfg.dlSpsPrdctyEnum;
      listNode = listNode->next;

      /* Check if SPS is successfully activated for UE
              Until then any BO for SPS svc shall be sched dynamically */
      /* Check if UE can be scheduled in downlink */
      if ((dlSpsUe->dlSpsStatus != RGSCH_DL_SPS_ACTV) 
            && (dlSpsUe->dlSpsStatus != RGSCH_DL_SPS_WAITING_FOR_ACTV_CFM)) 
      {
         rgSCHCmnSpsUtlDlDelFrmActvUes(dlSpsCell, ue, schdTime);
         RG_SCH_ADD_TO_CRNT_TIME(schdTime, dlSpsUeSchdInfo->schdKey , spsPrd);
         rgSCHCmnSpsUtlDlAdd2ActvUes(dlSpsCell, ue, dlSpsUeSchdInfo->schdKey);
         /* Free the SPS reserved resources for this DL SF */
         rgSCHCmnSpsUtlDlAdd2ZeroBOSvcUes(dlSpsCell, ue); 
         continue;
      }

#if (ERRCLASS & ERRCLS_DEBUG)
      if (!dlSpsUe->spsSvc)
      {
         /* SPS Service does not exist: this condition might not occur since
                * service deletion should cause removal from actvUesLst as well */
         RLOG_ARG3(L_ERROR,DBG_CELLID,cell->cellId, 
                  " CRNTI:%d has NULL SPS service at SPS ocassion: "
                  " sfn %d subframe %d", ue->ueId, schdTime.sfn,
                  schdTime.subframe);
         rgSCHCmnSpsUtlDlDelFrmActvUes(dlSpsCell, ue, schdTime);
         continue;
      }
#endif /*(ERRCLASS & ERRCLS_DEBUG)*/

      dlSpsSvc = RG_SCH_CMN_SPS_GET_DL_SVC(dlSpsUe->spsSvc);

      /* Zero BO SPS ocassion */
      if (!dlSpsSvc->effSpsBo)
      {
         if (FALSE == dlSpsUe->isDynSched)
         {
            dlSpsSvc->zeroBoOcassionCnt++;
         }
         dlSpsUe->isDynSched = FALSE;

         if (dlSpsSvc->zeroBoOcassionCnt >= 
               ue->dl.dlSpsCfg.explicitRelCnt)
         {
            RLOG_ARG3(L_DEBUG,DBG_CELLID,cell->cellId,
                  "UEID:%d DL Explict release at SPS ocassion: sfn %d "
                  "subframe %d", 
                  ue->ueId, 
                  schdTime.sfn, 
                  schdTime.subframe);
            /* Remove from toBeActvtdUes, if UE exists in the queue for
                     * re-activation */
            rgSCHCmnSpsUtlDlDelFrmToBeActvtdUes(dlSpsCell, ue);
            /* Explicit release needed */
            rgSCHCmnSpsUtlDlAdd2ToBeRelUes(dlSpsCell, ue);
            rgSCHCmnSpsUtlDlDelFrmActvUes(dlSpsCell, ue, schdTime);
            /* Free the SPS reserved resources for this DL SF */
            rgSCHCmnSpsUtlDlAdd2ZeroBOSvcUes(dlSpsCell, ue); 
            dlSpsSvc->zeroBoOcassionCnt = 0;     
            
            /* Clear UE's allocation from the subframes */
            rgSCHCmnSpsDlUeClearN1PucchAlloc(cell, ue, FALSE);
            rgSCHCmnSpsDlUeClearAlloc(cell, ue);
            RGSCH_SPS_SET_CLEAR_ALLC_HIST(ue);

            RGSCH_SPS_SET_DL_SPS_STATE(ue, RGSCH_DL_SPS_RELEASE_TRIGGERED);
            dlSpsUe->actionPndng = 0; /* TDD SPS INTG FIX */
            continue;
         }
         else
         {
  #ifdef LTE_TDD
            /* Incase of TDD, not sending any data on SPS occasion
                        will result in UE sending NACK for the SPS occasion harq
                        proc. This will severly impact other harq procs in case 
                        harq feedback mode is BUNDLING */
            if (RGR_TDD_ACKNACK_MODE_BUNDL == ue->dl.ackNackMode)
            {
                  forceSched = TRUE;
            }
            else
  #endif
            {
               /* Free the SPS reserved resources for this DL SF */
               rgSCHCmnSpsUtlDlAdd2ZeroBOSvcUes(dlSpsCell, ue); 
            }

            rgSCHCmnSpsUtlDlDelFrmActvUes(dlSpsCell, ue, schdTime);
            RG_SCH_ADD_TO_CRNT_TIME(schdTime, dlSpsUeSchdInfo->schdKey , spsPrd);
            rgSCHCmnSpsUtlDlAdd2ActvUes(dlSpsCell, ue, 
                                  dlSpsUeSchdInfo->schdKey);
         }

#ifdef LTE_TDD
         if (FALSE == forceSched)
#endif
         {
            continue;
         }
      }
      dlSpsUe->isDynSched = FALSE;

#ifdef LTE_TDD
      if (FALSE == forceSched)
#endif
      {
         /* If non-zero BO is recieved, reset zeroBoOcassionCnt */
         dlSpsSvc->zeroBoOcassionCnt = 0;
      }

      /* BO within packet size limits, use SPS ocassion and SPS RBs */
      /* If the UE is been re-activated for change in number of RBs */
      if (dlSpsUe->actionPndng & RG_SCH_CMN_SPS_DL_REACTV_FREQ)
      {
         rgSCHCmnSpsDlUeClearN1PucchAlloc(cell, ue, FALSE);
         rgSCHCmnSpsDlUeClearAlloc(cell, ue);
         RGSCH_SPS_SET_CLEAR_ALLC_HIST(ue);
         if ((rgSCHCmnSpsDlProcToBeReactvtdUe(cell, ue, schdTime, dlSf) != ROK))
         {
            RLOG_ARG3(L_DEBUG,DBG_CELLID,cell->cellId,
                     "Re-activation failed for UEID:%d, re-activation "
                     "time: sfn %d subframe %d", 
                     ue->ueId,
                     schdTime.sfn, 
                     schdTime.subframe);
            /* Add UE to toBeActvtdUes list for re-activation */
            rgSCHCmnSpsUtlDlAdd2ToBeRelUes(dlSpsCell,ue);      
            RGSCH_SPS_SET_DL_SPS_STATE(ue, RGSCH_DL_SPS_RELEASE_TRIGGERED);
            if (dlSpsUe->spsSvc->bo)
            {
               rgSCHCmnSpsUtlDlAdd2ToBeSchdSvcs(dlSpsCell, dlSpsUe->spsSvc);
            }
         }
         else
         {
            /* PDCCH for REACTIVATION shall be sent */
            RGSCH_SPS_SET_DL_SPS_STATE(ue, RGSCH_DL_SPS_WAITING_FOR_ACTV_CFM);
            dlSf->remUeCnt--;
         }
      }
      else
      {
         /* Update allocCb of UE with the scheduled SPS allocation */
         ue->dl.dlAllocCb = dlSpsUeSchdInfo->spsAllocInfo;
         n1PucchIdx = dlSpsUeSchdInfo->allocN1PucchIdx;
            
         /* Allocation finalization for an SPS UE */
         if (rgSCHCmnSpsDlUeAllocFnlz(cell, ue, schdTime, dlSf,
                  n1PucchIdx, isActvOrReactv) != ROK)
         {
            RLOG_ARG3(L_ERROR,DBG_CELLID,cell->cellId,
                     "RB allocation failed for activated CRNTI:%d, "
                     "SPS ocasssion : sfn %d subframe %d ", ue->ueId, schdTime.sfn, 
                     schdTime.subframe);
         }
         else
         {
            dlSf->isSPSOcc = TRUE;
            dlSf->remUeCnt--;
         }
      }

   } /* End of while (listNode) */

   RETVOID;
}

/**
 * @brief Handles to-be activated SPS UEs
 *
 * @details
 *
 *     Function : rgSCHCmnSpsDlProcToBeActvtdUes
 *
 *
 *     Processing Steps:
 *     - Determine the index into the cell->actvDlSpsUeLsts using following
 *       - Determine frm for (cell->crntTime + DL_DELTA)
 *       - (frm.sfn * 10 + frm.subframe) % RG_SCH_CMN_SPS_MAX_PRD
 *     - For each UE in toBeActvtdUes UE list,
 *       - Check collision in time domain. Call rgSCHCmnSpsDlUeCollidesInTime.
 *       - Check collision in frequency domain or n1Pucch resources. 
 *       Call rgSCHCmnSpsDlUeCollidesInFreqOrN1Pucch.
 *       - Determine allocation for the UE. Use resource allocation type 1 and
 *       0 for allocation within maximum SPS bandwidth.Call
 *       rgSCHCmnSpsDlUeAllocRbs. 
 *       - Finalize allocation. Call rgSCHCmnSpsDlUeAllocFnlz.
 *       - If failed,
 *         - Move UE to the end of toBeActvtdUes list
 *         - break from the loop
 *       
 *
 *  @param[in]  RgSchCellCb           *cell
 *  @parama[in] CmLteTimingInfo       crntTime
 *  @param[in]  RgSchDlSf             *dlSf
 *  @return  Void
 **/
#ifdef ANSI
PRIVATE S16 rgSCHCmnSpsDlProcToBeActvtdUes
(
RgSchCellCb           *cell,
CmLteTimingInfo       crntTime,
RgSchDlSf             *dlSf
)
#else/* ANSI */
PRIVATE S16 rgSCHCmnSpsDlProcToBeActvtdUes(cell, crntTime, dlSf)
RgSchCellCb           *cell;
CmLteTimingInfo       crntTime;
RgSchDlSf             *dlSf;
#endif /* ANSI */
{
   RgSchCmnDlCellSpsInfo *dlSpsCell = RG_SCH_CMN_SPS_GET_DL_CELL(cell);
   CmLList               *listNode  = dlSpsCell->toBeActvtdUes.first;
   RgSchUeCb             *ue;
   RgSchDlSfAllocInfo    allocInfo;
   CmLteTimingInfo       schdTime = crntTime;
   Bool                  isActvOrReactv = TRUE;
   U8                    n1PucchIdx;
   RgSchCmnDlUeSpsInfo   *dlSpsUe;
   RgSchCmnDlSvcSpsInfo  *dlSpsSvc;
#ifdef DEBUGP   
   CmLteTimingInfo       frm = schdTime;
   UNUSED(frm);
#endif

   TRC2(rgSCHCmnSpsDlProcToBeActvtdUes);

   /* Increment by DL DELTA to determine the time for which scheduling
    * is done */
   RGSCH_INCR_SUB_FRAME(schdTime, RG_SCH_CMN_DL_DELTA);

   while (dlSf->remUeCnt && listNode)
   {
      ue       = (RgSchUeCb *)(listNode->node);
      dlSpsUe = RG_SCH_CMN_SPS_GET_DL_UE(ue);
      listNode = listNode->next;

      /* Check if UE is already scheduled by SPS module */
      if (RG_SCH_CMN_IS_UE_SPS_SCHDLD(ue, schdTime))
      {
#ifdef RGSCH_SPS_STATS
         dlSpsFailToActUeAlreadySched++;
#endif
         continue;
      }

      /* Check if UE collides in time domain and if the current activation
       * offset needs to be changed */
      if (rgSCHCmnSpsDlUeCollidesInTime(cell, ue, schdTime,NULLP))
      {
#ifdef RGSCH_SPS_STATS
         dlSpsFailToActUeCollInTime++;
#endif
         RLOG_ARG3(L_ERROR,DBG_CELLID,cell->cellId,
                  "SPS Collision in time domain for CRNTI:%d, activation "
                  "time: sfn %d subframe %d", ue->ueId, schdTime.sfn, 
                  schdTime.subframe);
         continue;
      }

      /* Check if UE collides in frequency domain or in n1Pucch resources: 
       * no resources available at this offset */
      /* Set n1PucchIdx to an invalid value */
      n1PucchIdx = RG_SCH_CMN_SPS_DL_MAX_N1PUCCH_IDX_PER_UE + 1;
      if (rgSCHCmnSpsDlUeCollidesInFreqOrN1Pucch(cell, ue, schdTime, 
               &allocInfo, &n1PucchIdx, FALSE))
      {
#ifdef RGSCH_SPS_STATS
         dlSpsFailToActUeCollInFreq++;
#endif

         RLOG_ARG3(L_ERROR,DBG_CELLID,cell->cellId,
             "SPS Collision in frequency domain for CRNTI:%d, activation "
             "time: sfn %d subframe %d ", ue->ueId, frm.sfn, frm.subframe);
         continue;
      }

      /* Attempt allocation for SPS UE */
      if ((rgSCHCmnSpsDlUeAlloc(cell, ue, schdTime, dlSf, &allocInfo)) != ROK)
      {

#ifdef RGSCH_SPS_STATS
         dlSpsFailToActUeAlloc++;
#endif
         if (ue->dl.dlAllocCb.rbsReq > cell->spsCellCfg.maxSpsDlBw)
         {
            rgSCHCmnSpsUtlDlDelFrmToBeActvtdUes(dlSpsCell, ue);
            RGSCH_SPS_SET_DL_SPS_STATE(ue, RGSCH_DL_SPS_IDLE);
            if (dlSpsUe->spsSvc->bo)
            {
               rgSCHCmnSpsUtlDlAdd2ToBeSchdSvcs(dlSpsCell, dlSpsUe->spsSvc);
            }
         }
         RLOG_ARG3(L_ERROR,DBG_CELLID,cell->cellId,
                  "RB allocation failed for UE: CRNTI:%d, "
                  "activation time: sfn %d subframe %d", ue->ueId, frm.sfn, 
                  frm.subframe);
         continue;
      }

      /* Allocation finalization for an SPS UE */
      if (rgSCHCmnSpsDlUeAllocFnlz(cell, ue, schdTime, dlSf, 
               n1PucchIdx, isActvOrReactv) != ROK)
      {
#ifdef RGSCH_SPS_STATS
         dlSpsFailToActUeAllocFnlz++;
#endif
        /*As part of purify fixes frm variable is included in case only DEBUGP is enabled */ 
         RLOG_ARG3(L_ERROR,DBG_CELLID,cell->cellId,
                "RB allocation failed for UE: UE AllcFnlz CRNTI:%d, "
                "activation time: sfn %d subframe %d", ue->ueId, frm.sfn, 
                frm.subframe);
         continue;
      }

      /* Remove UE from toBeActvtd list of UEs */
      rgSCHCmnSpsUtlDlDelFrmToBeActvtdUes(dlSpsCell, ue);

      /* Remove service from toBeSchdSvcs list, if present */
      dlSpsSvc = RG_SCH_CMN_SPS_GET_DL_SVC(dlSpsUe->spsSvc);
      if(dlSpsSvc->toBeSchdSvcEnt.node)
      {
         rgSCHCmnSpsUtlDlDelFrmToBeSchdSvcs(dlSpsCell, dlSpsUe->spsSvc);
      }

      /* Unmark the pending action */
      dlSpsUe->actionPndng = 0;


      RGSCH_SPS_SET_DL_SPS_STATE(ue, RGSCH_DL_SPS_WAITING_FOR_ACTV_CFM);

      dlSpsUe->statInfo.numSpsActv++;
      dlSf->remUeCnt--;
#ifdef RGSCH_SPS_STATS
      numActvations++;
#endif
      /* Breaking from the loop since activation for only one UE is attempted
       * per TTI */
      break;
   }


   RETVALUE(ROK);
}

/**
 * @brief Handles to-be re-activated SPS UEs for change in number of RBs
 *
 * @details
 *
 *     Function : rgSCHCmnSpsDlProcToBeReactvtdUe
 *
 *
 *     Processing Steps:
 *       - Check collision in frequency domain or n1Pucch resources. 
 *       Call rgSCHCmnSpsDlUeCollidesInFreqOrN1Pucch.
 *       - Determine allocation for the UE. Use resource allocation type 1 and
 *       0 for allocation within maximum SPS bandwidth.Call
 *       rgSCHCmnSpsDlUeAllocRbs. 
 *       - Finalize allocation. Call rgSCHCmnSpsDlUeAllocFnlz.
 *       
 *
 *  @param[in]  RgSchCellCb           *cell
 *  @param[in]  RgSchUeCb             *ue
 *  @param[in]  CmLteTimingInfo       *schdTime
 *  @param[in]  RgSchDlSf             *dlSf
 *  @return  S16
 **/
#ifdef ANSI
PRIVATE S16 rgSCHCmnSpsDlProcToBeReactvtdUe
(
RgSchCellCb           *cell,
RgSchUeCb             *ue,
CmLteTimingInfo       schdTime,
RgSchDlSf             *dlSf
)
#else/* ANSI */
PRIVATE S16 rgSCHCmnSpsDlProcToBeReactvtdUe(cell, ue, schdTime, dlSf)
RgSchCellCb           *cell;
RgSchUeCb             *ue;
CmLteTimingInfo       schdTime;
RgSchDlSf             *dlSf;
#endif /* ANSI */
{
   RgSchCmnDlUeSpsInfo   *dlSpsUe = RG_SCH_CMN_SPS_GET_DL_UE(ue);
   RgSchCmnSpsDlUeSchdInfo *dlSpsUeSchdInfo = &(dlSpsUe->dlSpsUeSchdInfo);
   RgSchDlSfAllocInfo    allocInfo;
   U8                    n1PucchIdx;
   Bool                  isActvOrReactv = TRUE;
#ifdef DEBUGP
   CmLteTimingInfo       frm = schdTime;
   UNUSED(frm);
#endif

   TRC2(rgSCHCmnSpsDlProcToBeReactvtdUe);

   /* Note: It is assumed in this function that dlSpsUe->isSpsActv = TRUE and
    * dlSpsUe has valid SPS scheduling information  */

   /* Check if UE collides in frequency domain or in n1Pucch resources: 
    * no resources available at this offset */
   n1PucchIdx = dlSpsUeSchdInfo->allocN1PucchIdx; 
   if (rgSCHCmnSpsDlUeCollidesInFreqOrN1Pucch(cell, ue, schdTime, 
            &allocInfo, &n1PucchIdx, FALSE))
   {
      RLOG_ARG3(L_ERROR,DBG_CELLID,cell->cellId,
               "SPS Collision in frequency domain for CRNTI:%d, Reactivation "
               "time: sfn %d subframe %d ", ue->ueId, frm.sfn, frm.subframe);
      RETVALUE(RFAILED);
   }
   /* ccpu00139527 */
   /*Check if UE is on DRX Active period*/
   if(!RG_SCH_CMN_DL_IS_UE_ACTIVE(ue))
   {

      RETVALUE(RFAILED);
   }

   /* Attempt allocation for SPS UE */
   if ((rgSCHCmnSpsDlUeAlloc(cell, ue, schdTime, dlSf, &allocInfo)) != ROK)
   {
      RLOG_ARG3(L_ERROR,DBG_CELLID,cell->cellId,
               "RB allocation failed for CRNTI:%d, "
               "activation time: sfn %d subframe %d", ue->ueId, frm.sfn, 
               frm.subframe);
      RETVALUE(RFAILED);
   }

   /* Allocation finalization for an SPS UE */
   if (rgSCHCmnSpsDlUeAllocFnlz(cell, ue, schdTime, dlSf, 
            n1PucchIdx, isActvOrReactv) != ROK)
   {
      RLOG_ARG3(L_ERROR,DBG_CELLID,cell->cellId,
               "RB allocation failed for CRNTI:%d, "
               "activation time: sfn %d subframe %d", ue->ueId, frm.sfn, 
               frm.subframe);
      RETVALUE(RFAILED);
   }

   dlSpsUe->statInfo.numSpsReactv++;
   /* Reset the pending action for the UE */
   dlSpsUe->actionPndng = 0;

#ifdef RGSCH_SPS_STATS
   numActvations++;
#endif
   RETVALUE(ROK);
}
/**
 * @brief Checks collision of allocation in time domain
 *
 * @details
 *
 *     Function : rgSCHCmnSpsDlUeCollidesInTime
 *
 *
 *     Processing Steps:
 *       - If UE is in HD-FDD mode, check collision with SR, CQI, SRS for the
 *       'frm' for current and future SPS ocassions.
 *       - If UE has measurement gap enabled, check collision with measurement
 *       gap using measGapMask for current and future SPS ocassions.
 *       - If DRX is enabled, the selected offset should lie within the
 *       on-Duration timer value for the UE
 *
 *  @param[in]  RgSchCellCb           *cell
 *  @param[in]  RgSchUeCb             *ueCb
 *  @param[in]  CmLteTimingInfo       timingInfo
 *  @param[in]  RgrUeMeasGapCfg      *measGapCfg
 *  @return  S16
 *             ROK      success 
 *             RFAILED  failed
 **/
#ifdef ANSI
PRIVATE Bool rgSCHCmnSpsDlUeCollidesInTime
(
RgSchCellCb            *cell,
RgSchUeCb              *ue,
CmLteTimingInfo        timingInfo,
RgrUeMeasGapCfg      *measGapCfg
)
#else/* ANSI */
PRIVATE Bool rgSCHCmnSpsDlUeCollidesInTime(cell, ue, timingInfo)
RgSchCellCb            *cell;
RgSchUeCb              *ue;
CmLteTimingInfo        timingInfo;
RgrUeMeasGapCfg      *measGapCfg;
#endif /* ANSI */
{
    U16              startIdx;
    U16              prd;
    U16              spsPrd;
    U8               spsPrdIdx;
    Bool             collides = FALSE;
    RgSchCmnDlUeSpsInfo   *dlSpsUe = RG_SCH_CMN_SPS_GET_DL_UE(ue);
    Bool             isMesGapEnabled = FALSE;

    TRC2(rgSCHCmnSpsDlUeCollidesInTime);

    spsPrdIdx = dlSpsUe->prdIdx;
    if(spsPrdIdx >= RG_SCH_CMN_SPS_PRD_INVALID ) /* SPS_REVIEW */
    {
       RETVALUE(TRUE);
    }

    spsPrd    = (U16)ue->dl.dlSpsCfg.dlSpsPrdctyEnum;

    /* If HD-FDD UE, check collision with SR, SRS and CQI */
#ifdef RG_UNUSED
    startIdx = (timingInfo.sfn * 10 + timingInfo.subframe) % prd;
    if (collides = 
            rgSCHCmnSpsDlChkCollision(spsPrdIdx, startIdx, prd, spsPrd, offset))
    {
        RETVALUE(collides);
    }
    /* For DRX active UE, check if the scheduling offset
     * falls during 'OnDuration' period for the UE */
#endif /*RG_UNUSED*/

   if (measGapCfg)
   {
      isMesGapEnabled = measGapCfg->isMesGapEnabled;
      prd = measGapCfg->gapPrd;
   }
   else
   {
      isMesGapEnabled = ue->measGapCb.isMesGapEnabled;
      prd = ue->measGapCb.gapPrd;
   }
   /* If measurement gap enabled, check collision for measurement gap */
   if (isMesGapEnabled)
   {
      startIdx = (timingInfo.sfn * 10 + timingInfo.subframe) % prd;
      collides =
            rgSCHCmnSpsDlChkMeasGapCollision(spsPrdIdx , startIdx, prd, 
               spsPrd, (U32 *)dlSpsUe->measGapMask);
      if (collides)
      {
         RETVALUE(collides);
      }
   }
      /*  For DRX active UE, check if the scheduling offset
       * falls during 'OnDuration' period for the UE .
       *  we checked for SPS periodicity being multiple of DRX cycle at 
       *  configuration so that SPS occassions fall on on-duration*/
   if(ue->isDrxEnabled)
   {
       /*if UE is not on onduration then dont schedule it*/
       if(RG_SCH_DRX_DL_IS_UE_ONDUR_INACTIVE(ue->drxCb))
       {
          RETVALUE(TRUE);
       }
   }

    RETVALUE(collides);
}

/**
 * @brief Checks collision of allocation with measurement gap
 *
 * @details
 *
 *     Function : rgSCHCmnSpsDlChkMeasGapCollision
 *
 *
 *     Processing Steps:
 *       - If UE has measurement gap enabled, check collision with measurement
 *       gap using measGapMask for current and future SPS ocassions.
 *       - It checks collision using the measurement gap bitmask for the UE
 *       using the time at which scheduling is attempted.
 *
 *  @param[in]  U8                    spsPrdIdx
 *  @param[in]  U16                   startIdx
 *  @param[in]  U16                   measGapPrd
 *  @param[in]  U16                   spsPrd
 *  @param[in]  U8                    *measGapMask
 *  @return  Bool
 *             TRUE     Collision detected
 *             FALSE    No collision
 **/
#ifdef ANSI
PRIVATE Bool rgSCHCmnSpsDlChkMeasGapCollision
(
U8                     spsPrdIdx,
U16                    startIdx,
U16                    measGapPrd,
U16                    spsPrd,
U32                     *measGapMask
)
#else /* ANSI */
PRIVATE Bool rgSCHCmnSpsDlChkMeasGapCollision(spsPrdIdx, startIdx, 
      measGapPrd, spsPrd, measGapMask)
U8                     spsPrdIdx;
U16                    startIdx;
U16                    measGapPrd;
U16                    spsPrd;
U32                     *measGapMask;
#endif /* ANSI */
{
   U8                  measGapPrdIdx;
   U16                 cycleCnt;
   U16                 lcm;
   U8                  idx;
   U8                  maskIdx;
   U8                  posInMask;

   TRC2(rgSCHCmnSpsDlChkMeasGapCollision);

   /* If measurement gap enabled, check collision for measurement gap */
   RG_SCH_CMN_SPS_GET_PRD_IDX (measGapPrd, &measGapPrdIdx);

   if (measGapPrdIdx == RG_SCH_CMN_SPS_PRD_INVALID)
   {
      RETVALUE(TRUE);
   }
   lcm = rgSchCmnSpsLcmTbl[spsPrdIdx][measGapPrdIdx];
   cycleCnt = lcm/spsPrd;
   idx = (U8)startIdx;

   while (cycleCnt)
   {
      maskIdx = idx / 32;
      posInMask = idx % 32;
      if ((measGapMask[maskIdx]) & 
            (1 << RG_SCH_CMN_DL_GET_POS_FRM_LSB(posInMask)))
      {
         break;
      }
      idx = (idx + spsPrd) % measGapPrd;
      cycleCnt--;
   }
   if (cycleCnt)
   {
      RETVALUE(TRUE);
   }
   else
   {
      RETVALUE(FALSE);
   }
}

#ifdef RG_UNUSED
/* This function to be used after introduction of HD_FDD module */
/**
 * @brief Checks collision of allocation with a given periodicity and offset
 *
 * @details
 *
 *     Function : rgSCHCmnSpsDlChkCollision
 *
 *
 *     Processing Steps:
 *       - It checks collision of allocation for given offset and periodicity of
 *       the SPS.
 *
 *  @param[in]  U8                    spsPrdIdx
 *  @param[in]  U16                   startIdx
 *  @param[in]  U16                   prd
 *  @param[in]  U16                   spsPrd
 *  @param[in]  U16                   offset
 *  @return  Bool
 *             TRUE     Collision detected
 *             FALSE    No collision
 **/
#ifdef ANSI
PRIVATE Bool rgSCHCmnSpsDlChkCollision
(
U8                     spsPrdIdx,
U16                    startIdx,
U16                    prd,
U16                    spsPrd,
U16                    offset
)
#else /* ANSI */
PRIVATE Bool rgSCHCmnSpsDlChkCollision(spsPrdIdx, startIdx, 
      prd, spsPrd, offset)
U8                     spsPrdIdx;
U16                    startIdx;
U16                    prd;
U16                    spsPrd;
U16                    offset;
#endif /* ANSI */
{
   U8                  prdIdx;
   U16                 cycleCnt;
   U16                 lcm;
   U8                  idx;

   TRC2(rgSCHCmnSpsDlChkCollision);

   /* Get the index for the periodicity of the feature */
   RG_SCH_CMN_SPS_GET_PRD_IDX (prd, &prdIdx);
   lcm = rgSchCmnSpsLcmTbl[spsPrdIdx][prdIdx];
   cycleCnt = lcm/spsPrd;
   idx = startIdx;

   while (cycleCnt)
   {
      if (offset == idx)
         break;
      idx = (idx + spsPrd) % prd;
      cycleCnt--;
   }
   if (cycleCnt)
   {
      RETVALUE(TRUE);
   }
   else
   {
      RETVALUE(FALSE);
   }
}
#endif /*RG_UNUSED*/

/**
 * @brief Checks collision of allocation in frequency domain
 *
 * @details
 *
 *     Function : rgSCHCmnSpsDlUeCollidesInFreqOrN1Pucch
 *
 *
 *     Processing Steps:
 *     - Assumed that this function is called with invalid value of n1Pucch if
 *     n1Pucch needs to be determined by the function.
 *     - Check the collision in frequency domain for the computed sfIdx and
 *     returns allocation mask indicating frequency resources available for UE.
 *     - Check the collision in n1Pucch values and returns the n1PucchMask
 *     indicating n1Pucch available resources.
 *
 *  @param[in]  RgSchCellCb        *cellCb
 *  @param[in]  RgSchUeCb          *ueCb
 *  @param[in]  CmLteTimingInfo    timingInfo
 *  @param[in]  Bool               ignoreUe
 *  @param[out] RgSchDlSfAllocInfo *allocInfo
 *  @param[out] U8                 *n1PucchIdx
 *  @return  Bool
 *             TRUE     Collision detected in n1Pucch resources
 *             FALSE    No collision detected in n1Pucch values
 **/
#ifdef ANSI
PRIVATE Bool rgSCHCmnSpsDlUeCollidesInFreqOrN1Pucch
(
RgSchCellCb         *cell,
RgSchUeCb           *ue,
CmLteTimingInfo     timingInfo,
RgSchDlSfAllocInfo  *allocInfo,
U8                  *n1PucchIdx,
Bool                 ignoreUe
)
#else /* ANSI */
PRIVATE Bool rgSCHCmnSpsDlUeCollidesInFreqOrN1Pucch(cell, ue, timingInfo,
      allocInfo, n1PucchIdx)
RgSchCellCb         *cell;
RgSchUeCb           *ue;
CmLteTimingInfo     timingInfo;
RgSchDlSfAllocInfo  *allocInfo;
U8                  *n1PucchIdx;
Bool                 ignoreUe;
#endif /* ANSI */
{
   RgSchCmnDlCellSpsInfo *dlSpsCell = RG_SCH_CMN_SPS_GET_DL_CELL(cell);
   RgSchCmnDlUeSpsInfo   *dlSpsUe = RG_SCH_CMN_SPS_GET_DL_UE(ue);
   U16                   sfIdx;
   U16                   cycleCnt;
   U16                   spsPrd;  
   U16                   n1PucchCnt;
   U32                   n1PucchMask[RG_SCH_CMN_SPS_DL_N1PUCCH_32BITMASK_SIZE];
   U8                    maskIdx;
   U8                    maskPos;
   U16                   crntLcm = dlSpsCell->spsPrdLcmVal;
   U8                    ueDecCnt = 0;
   RgSchCmnSpsDlUeSchdInfo *dlSpsUeSchdInfo = &(dlSpsUe->dlSpsUeSchdInfo);
   U16                  n1PucchSfIdx;

   TRC2(rgSCHCmnSpsDlUeCollidesInFreqOrN1Pucch);

   /* Memset the input masks to 0 */
   cmMemset((U8 *)allocInfo, 0, sizeof(*allocInfo));
   cmMemset((U8 *)n1PucchMask, 0, 
         (sizeof(U32) * RG_SCH_CMN_SPS_DL_N1PUCCH_32BITMASK_SIZE) );

   /* Initialize the variables */
   spsPrd    = (U16)ue->dl.dlSpsCfg.dlSpsPrdctyEnum;
   sfIdx = (timingInfo.sfn * 10 + timingInfo.subframe) % crntLcm;
   cycleCnt = crntLcm/spsPrd; 

   if (ignoreUe)
   {
      /* Ignore the active UE count for this UE */
      ueDecCnt = 1;
   }

   /* Determine the allocation mask and n1Pucch mask for the current offset */
   while(cycleCnt)
   {
      RgSchDlSfAllocInfo *spsSfAlloc = 
         &(dlSpsCell->spsSfTbl[sfIdx].spsAllocInfo);

      /* SPS can't be activated in this subframe due to max SPS UEs Per TTI check*/
      if ((dlSpsCell->spsSfTbl[sfIdx].numDlSpsActiveUes - ueDecCnt)== 
               cell->spsCellCfg.maxSpsUePerDlSf)
      {
          RLOG_ARG3(L_ERROR,DBG_CELLID,cell->cellId,
        "rgSCHCmnSpsDlUeCollidesInFreqOrN1Pucch max sps ue reached "
        " numact %d max sps %d CRNTI:%d",
         dlSpsCell->spsSfTbl[sfIdx].numDlSpsActiveUes,
         cell->spsCellCfg.maxSpsUePerDlSf,ue->ueId);
         RETVALUE(TRUE);
      }
 #ifdef RGSCH_SPS_UNUSED     
      for (maskIdx = 0; maskIdx < RG_SCH_NUM_RATYPE1_32BIT_MASK; ++maskIdx)
      {
         allocInfo->raType1Mask[maskIdx] |= spsSfAlloc->raType1Mask[maskIdx];
         allocInfo->raType1UsedRbs[maskIdx] += 
            spsSfAlloc->raType1UsedRbs[maskIdx];
      }
 #endif
      allocInfo->raType0Mask |= spsSfAlloc->raType0Mask;
      for (maskIdx = 0; maskIdx < RG_SCH_NUM_RATYPE2_32BIT_MASK; ++maskIdx)
      {
         allocInfo->raType2Mask[maskIdx] |= spsSfAlloc->raType2Mask[maskIdx];
      }
      
#ifdef LTE_TDD      
      n1PucchSfIdx = (sfIdx + 
         rgSchTddInvDlAscSetIdxTbl[cell->ulDlCfgIdx][sfIdx]) % crntLcm;
#else
      n1PucchSfIdx = sfIdx;
#endif

      for (n1PucchCnt = 0;n1PucchCnt < RG_SCH_CMN_SPS_DL_N1PUCCH_32BITMASK_SIZE;
            ++n1PucchCnt)
      {
         n1PucchMask[n1PucchCnt] |= 
            dlSpsCell->spsSfTbl[n1PucchSfIdx].n1PucchMask[n1PucchCnt];
      } 
      sfIdx = (sfIdx + spsPrd) % crntLcm;
      --cycleCnt;
   }

   if (ignoreUe)
   {
      /* ignore N1 Pucch already allocated for this UE */
      maskIdx = dlSpsUe->n1PucchIdx[dlSpsUeSchdInfo->allocN1PucchIdx]/32;
      maskPos = dlSpsUe->n1PucchIdx[dlSpsUeSchdInfo->allocN1PucchIdx] % 32;
      n1PucchMask[maskIdx] &= ~(1 << RG_SCH_CMN_DL_GET_POS_FRM_LSB(maskPos));
   }

   /* Check if n1Pucch value is available */
   if (*n1PucchIdx == RG_SCH_CMN_SPS_DL_MAX_N1PUCCH_IDX_PER_UE + 1)
   {
      for (n1PucchCnt = 0; n1PucchCnt < ue->dl.dlSpsCfg.numPucchVal; 
              ++n1PucchCnt)
      {
         maskIdx = dlSpsUe->n1PucchIdx[n1PucchCnt]/32;
         maskPos = dlSpsUe->n1PucchIdx[n1PucchCnt] % 32;
         if (!((n1PucchMask[maskIdx]) & 
               (1 << RG_SCH_CMN_DL_GET_POS_FRM_LSB(maskPos))))
         {
            *n1PucchIdx = (U8)n1PucchCnt;
            break;
         }
      }
   }

   /* If n1PucchIdx is invalid value, no n1Pucch value is available at this
    * offset: Collision in n1Pucch resources detected */
   if (*n1PucchIdx == RG_SCH_CMN_SPS_DL_MAX_N1PUCCH_IDX_PER_UE + 1)
   {
       RLOG_ARG3(L_ERROR,DBG_CELLID,cell->cellId,
          "no free n1pucch resources found sfn %d sf %d CRNTI:%d",cell->crntTime.sfn, 
            cell->crntTime.subframe,ue->ueId);
#ifdef RG_UNUSED
       RLOG_ARG4(L_ERROR,DBG_CELLID,cell->cellId,
          "N1 Mask [0] 0x%x [1] 0x%x [2] 0x%x CRNTI:%d",
          n1PucchMask[0],n1PucchMask[1],n1PucchMask[2],ue->ueId);
#endif

      RETVALUE(TRUE);
   }
   else
   {
      RETVALUE(FALSE);
   }
}


/**
 * @brief Performs resource allocation for an SPS UE
 *
 * @details
 *
 *     Function : rgSCHCmnSpsDlUeAlloc
 *
 *
 *     Processing Steps:
 *       - Determine the scaled CQI value for the UE.Call
 *       rgSCHCmnSpsDlGetScaledCqi.
 *       - Determine the number of RBs needed for the SPS transmission.
 *       - Determine the allocation and allocation type.
 *       - Determine the dci format for transmission.
 *       - Check if the PDCCH for the UE is available.
 *       - Update allocInfo with information.
 *
 *  @param[in]  RgSchCellCb     *cellCb
 *  @param[in]  RgSchUeCb       *ueCb
 *  @param[in]  CmLteTimingInfo timingInfo
 *  @param[in]  RgSchDlSf       *dlSf
 *  @param[in]  RgSchDlSfAllocInfo  *allocInfo
 *  @return  S16
 *             ROK      success 
 *             RFAILED  failed
 **/
#ifdef ANSI
PRIVATE S16 rgSCHCmnSpsDlUeAlloc
(
RgSchCellCb         *cell,
RgSchUeCb           *ue,
CmLteTimingInfo     timingInfo,
RgSchDlSf           *dlSf,
RgSchDlSfAllocInfo  *allocInfo
)
#else /* ANSI */
PRIVATE S16 rgSCHCmnSpsDlUeAlloc(cell, ue, timingInfo, dlSf, allocInfo)
RgSchCellCb         *cell;
RgSchUeCb           *ue;
CmLteTimingInfo     timingInfo;
RgSchDlSf           *dlSf;
RgSchDlSfAllocInfo  *allocInfo;
#endif /* ANSI */
{

   RgSchCmnDlUe        *cmnUeDl = RG_SCH_CMN_GET_DL_UE(ue);
   RgSchCmnDlUeSpsInfo *dlSpsUe = RG_SCH_CMN_SPS_GET_DL_UE(ue);
   RgSchCmnDlSvcSpsInfo  *dlSpsSvc = NULLP;
   U8                  scCqi;
   RgSchDlRbAlloc      *rbAllocInfo = &(ue->dl.dlAllocCb);
   U32                 spsBo;
   U16                 rlcHdrEst;

   TRC2(rgSCHCmnSpsDlUeAlloc);

   /* Note: Assumed allocation information for UE is initialized to 0 while
    * memset for ueCb */

   /* Determine the scaled CQI value for SPS transmission */
   dlSpsUe->dlSpsUeSchdInfo.scaledCqi = 
      rgSCHCmnSpsDlGetScaledCqi(ue, cmnUeDl->mimoInfo.cwInfo[0].cqi);
   scCqi = dlSpsUe->dlSpsUeSchdInfo.scaledCqi;

   /* Determine number of RBs needed by the UE for SPS allocation */
   dlSpsSvc = RG_SCH_CMN_SPS_GET_DL_SVC(dlSpsUe->spsSvc);
   spsBo = dlSpsUe->spsSchedBo + RG_SCH_DL_SPS_ADDTL_BO;
   RG_SCH_CMN_DL_GET_HDR_EST(dlSpsUe->spsSvc, rlcHdrEst);
   spsBo += rlcHdrEst;

   rgSCHCmnClcRbAlloc(cell, spsBo, scCqi, &rbAllocInfo->rbsReq, \
         &rbAllocInfo->tbInfo[0].bytesReq,\
         &rbAllocInfo->tbInfo[0].imcs, &rbAllocInfo->tbInfo[0].iTbs, TRUE,dlSf);

   if (!rbAllocInfo->rbsReq)
   {
      RLOG_ARG1(L_ERROR,DBG_CELLID,cell->cellId,
               "Zero RBs needed for SPS UE %d", ue->ueId);
      RETVALUE(RFAILED);
   }

   /* Determine the allocation for the UE */
   rgSCHCmnSpsDlUeAllocRbs(cell, ue, timingInfo, dlSf, allocInfo);
   if (!rbAllocInfo->rbsAlloc)
   {
#ifdef RG_UNUSED
    RLOG_ARG4(L_ERROR,DBG_CELLID,cell->cellId,
       "SPS ACT or REACT CQI %d ScCQI %d iMcs %d spsBo %d",
         cmnUeDl->mimoInfo.cwInfo[0].cqi,
         scCqi,
         rbAllocInfo->tbInfo[0].imcs,
         spsBo);
    RLOG_ARG2(L_ERROR,DBG_CELLID,cell->cellId, 
       "SPS ACT or REACT CQI rbsReq %d CRNTI:%d",
         rbAllocInfo->rbsReq,ue->ueId);
#endif
#ifdef RGSCH_SPS_STATS
    spsRbAllocFailReqRbs[spsRbAllocFailCnt++% RGSCH_SPS_MAX_RBALLC_FAIL] = 
       rbAllocInfo->rbsReq;
#endif
      RLOG_ARG2(L_ERROR,DBG_CELLID,cell->cellId,
               "RB allocation failed for SPS CRNTI:%d at CQI %d", ue->ueId, 
               scCqi);
      RETVALUE(RFAILED);
   }

   /* Determine dciFormat for the UE for SPS transmission */
   rgSCHCmnSpsDlGetDciFmt(ue, rbAllocInfo->raType, 
         &rbAllocInfo->dciFormat);


   /* Update scaled CQI value in rbAllocInfo */
   rbAllocInfo->cqiForTx = scCqi;

   RETVALUE(ROK);
}

/**
 * @brief Checks if UE is active for DL scheduling 
 *
 * @details
 *
 *     Function : rgSCHCmnSpsDlChkUeInActv
 *
 *
 *     Processing Steps:
 *       - If UE is in HD-FDD mode, check collision with SR, CQI, SRS for the
 *       scheduling time.
 *       - If UE has measurement gap enabled, check collision with measurement
 *       gap.
 *       - If DRX is enabled, check if DL scheduling can be done
 *
 *  @param[in]  RgSchCellCb           *cell
 *  @param[in]  RgSchUeCb             *ueCb
 *  @param[in]  CmLteTimingInfo       timingInfo
 *  @return  Bool
 *             TRUE     inactive
 *             FALSE    active
 **/
#ifdef ANSI
PRIVATE Bool rgSCHCmnSpsDlChkUeInActv
(
RgSchCellCb            *cell,
RgSchUeCb              *ue,
CmLteTimingInfo        timingInfo
)
#else /* ANSI */
PRIVATE Bool rgSCHCmnSpsDlChkUeInActv(cell, ue, timingInfo)
RgSchCellCb            *cell;
RgSchUeCb              *ue;
CmLteTimingInfo        timingInfo;
#endif /* ANSI */
{
   RgSchCmnDlUeSpsInfo   *dlSpsUe = RG_SCH_CMN_SPS_GET_DL_UE(ue);
   U32 *measGapMask = dlSpsUe->measGapMask;
   U8 idx = 0;
   U8                  maskIdx;
   U8                  posInMask;

    TRC2(rgSCHCmnSpsDlChkUeInActv);

   /* Call APIs exposed by measurement gap, DRX and HD-FDD module */
         

   if (ue->measGapCb.isMesGapEnabled)
   {
      idx = (timingInfo.sfn * 10 + timingInfo.subframe) %
         ue->measGapCb.gapPrd;
      maskIdx = idx / 32;
      posInMask = idx % 32;
      if ((measGapMask[maskIdx]) & 
            (1 << RG_SCH_CMN_DL_GET_POS_FRM_LSB(posInMask)))
      {
         RETVALUE(TRUE);
      }
   }

   if (RG_SCH_CMN_IS_UE_SPS_SCHDLD(ue, timingInfo))
   {
      RETVALUE(TRUE);
   }

   /* Check if the UE is already scheduled by cmn scheduler */
   if (((RgSchCmnUe *)(ue->sch))->dl.proc)
   {
      RETVALUE(TRUE);
   }
   /*Check if UE is on DRX Active period*/
   if(!RG_SCH_CMN_DL_IS_UE_ACTIVE(ue))
   {

      RETVALUE(TRUE);
   }
   RETVALUE(FALSE);
}

/***********************************************************
 *
 *     Func : rgSCHCmnSpsDlGetDciFmt
 *
 *
 *     Desc :Determines DCI format based on the resource allocation type for 
 *     SPS UE
 *
 *     Ret  : DCI format
 *            
 *
 *     Notes:
 *
 *     File :
 *
 **********************************************************/
#ifdef ANSI
PRIVATE Void rgSCHCmnSpsDlGetDciFmt
(
RgSchUeCb    *ue,
U8           raType,
TfuDciFormat *dciFmt
)
#else /* ANSI */
PRIVATE  Void rgSCHCmnSpsDlGetDciFmt(ue, raType, dciFmt) 
RgSchUeCb    *ue;
U8           raType;
TfuDciFormat *dciFmt;
#endif /* ANSI */
{
   TRC2(rgSCHCmnSpsDlGetDciFmt);
   *dciFmt =  rgSchCmnSpsRaTypeToDciFrmt[ue->mimoInfo.txMode - 1][raType];
   RETVOID;
}

/**
 * @brief Update allocation mask for SPS with DL SF allocation 
 *
 * @details
 *
 *     Function : rgSCHCmnSpsDlUpdSpsWithDlSfAlloc
 *
 *
 *     Processing Steps:
 *     
 *  @param[in]  RgSchDlSf       *dlSf
 *  @param[out]  RgSchDlSfAllocInfo  *allocInfo
 *  @return  Void
 **/
#ifdef ANSI
PRIVATE Void rgSCHCmnSpsDlUpdSpsWithDlSfAlloc
(
RgSchDlSf             *dlSf,
RgSchDlSfAllocInfo    *allocInfo
)
#else /* ANSI */
PRIVATE Void rgSCHCmnSpsDlUpdSpsWithDlSfAlloc(dlSf, allocInfo)
RgSchDlSf             *dlSf;
RgSchDlSfAllocInfo    *allocInfo;
#endif /* ANSI */
{
   U8                 idx;

   TRC2(rgSCHCmnSpsDlUpdSpsWithDlSfAlloc);
#ifdef RGSCH_SPS_UNUSED
   /* Update allocation mask with DL SF allocation  */
   for (idx = 0; idx < RG_SCH_NUM_RATYPE1_32BIT_MASK; ++idx)
   {
      allocInfo->raType1Mask[idx] |= dlSf->dlSfAllocInfo.raType1Mask[idx];
      allocInfo->raType1UsedRbs[idx] += 
         dlSf->dlSfAllocInfo.raType1UsedRbs[idx];
   }
#endif
   allocInfo->raType0Mask |= dlSf->dlSfAllocInfo.raType0Mask;
   for (idx = 0; idx < RG_SCH_NUM_RATYPE2_32BIT_MASK; idx++)
   {
      allocInfo->raType2Mask[idx] |= dlSf->dlSfAllocInfo.raType2Mask[idx];
   }

   RETVOID;
}

/**
 * @brief Performs resource allocation for an SPS UE
 *
 * @details
 *
 *     Function : rgSCHCmnSpsDlUeAllocRbs
 *
 *
 *     Processing Steps:
 *       - Determine allocation for the UE. Use resource allocation type 1 and
 *       0 for allocation within maximum SPS bandwidth.
 *
 *  @param[in]  RgSchCellCb     *cellCb
 *  @param[in]  RgSchUeCb       *ueCb
 *  @param[in]  CmLteTimingInfo timingInfo
 *  @param[in]  RgSchDlSf       *dlSf
 *  @param[in]  RgSchDlSfAllocInfo  *allocInfo
 *  @return  S16
 *             ROK      success 
 *             RFAILED  failed
 **/
#ifdef ANSI
PRIVATE S16 rgSCHCmnSpsDlUeAllocRbs
(
RgSchCellCb         *cell,
RgSchUeCb           *ue,
CmLteTimingInfo     timingInfo,
RgSchDlSf           *dlSf,
RgSchDlSfAllocInfo  *allocInfo
)
#else /* ANSI */
PRIVATE S16 rgSCHCmnSpsDlUeAllocRbs(cell, ue, timingInfo, dlSf, allocInfo)
RgSchCellCb         *cell;
RgSchUeCb           *ue;
CmLteTimingInfo     timingInfo;
RgSchDlSf           *dlSf;
RgSchDlSfAllocInfo  *allocInfo;
#endif /* ANSI */
{

   U16                 frmIdx;
#ifdef RGSCH_SPS_UNUSED
   U8                  rbgSize = cell->rbgSize;
#endif
   U8                  numAllocRbs = 0;
   U8                  numAllocRbgs = 0;
   U8                  rbStart=0;
   RgSchCmnDlCellSpsInfo *dlSpsCell = RG_SCH_CMN_SPS_GET_DL_CELL(cell);
   RgSchDlRbAlloc      *rbAllocInfo = &(ue->dl.dlAllocCb);
   RgSchCmnSpsDlSf     *dlSpsSf = NULLP;
   RgSchBwRbgInfo      *spsRbgInfo = &(cell->spsBwRbgInfo);
#ifdef RGSCH_SPS_UNUSED
   U8                  temprbg;
#endif


   TRC2(rgSCHCmnSpsDlUeAllocRbs);

   /* Note: This function atttempts allocation only if RBs sufficient for SPS
    * allocation are available */

   /* Memset the resAllocInfo to 0 */
   cmMemset((U8 *)&rbAllocInfo->resAllocInfo, 0, sizeof(RgSchDlSfAllocInfo));

   /* Determine the DL SPS sub-frame for which allocation is being done */
   frmIdx = (timingInfo.sfn * 10 + timingInfo.subframe) % 
      dlSpsCell->spsPrdLcmVal;
   dlSpsSf = &dlSpsCell->spsSfTbl[frmIdx];

   /* Update allocMask with DL SF allocation */
   rgSCHCmnSpsDlUpdSpsWithDlSfAlloc(dlSf, allocInfo);

   /* Perform allocation for RA type 0 if rbsReq is multiple of RBG size (also
    * if RBG size = 1) */
#ifdef RGSCH_SPS_UNUSED
   if ((ue->mimoInfo.txMode != RGR_UE_TM_5) && 
         (ue->mimoInfo.txMode != RGR_UE_TM_6))
   {
      if (rbAllocInfo->rbsReq % rbgSize == 0)
      {
         numAllocRbgs = rgSCHCmnDlRaType0Alloc(allocInfo, 
               rbAllocInfo->rbsReq, spsRbgInfo, &numAllocRbs,
               &rbAllocInfo->resAllocInfo, FALSE);
         if (numAllocRbs)
         {
            rbAllocInfo->raType = RG_SCH_CMN_RA_TYPE0;
         }
      }
      /* If no RBS could be allocated, attempt RA TYPE 1 */
      if (!numAllocRbs)
      {
         numAllocRbs = rgSCHCmnDlRaType1Alloc(allocInfo, rbAllocInfo->rbsReq,
               spsRbgInfo, (U8)dlSpsSf->spsAllocInfo.nxtRbgSubset,
               &rbAllocInfo->allocInfo.raType1.rbgSubset,
               &rbAllocInfo->resAllocInfo, FALSE);

         if(numAllocRbs)
         {
            rbAllocInfo->raType = RG_SCH_CMN_RA_TYPE1;
            dlSpsSf->spsAllocInfo.nxtRbgSubset = 
               (rbAllocInfo->allocInfo.raType1.rbgSubset + 1 ) % rbgSize;
         }
      }
   }
#endif

   /* RA type 2 allocation for SPS UE */
   if (!numAllocRbs)
   {
      /* RA type 2 needs to be attempted for SPS UE */
      numAllocRbs = rgSCHCmnDlRaType2Alloc(allocInfo,rbAllocInfo->rbsReq,
            spsRbgInfo, &rbStart, &rbAllocInfo->resAllocInfo, FALSE);
      if (numAllocRbs)
      {
         rbAllocInfo->raType = RG_SCH_CMN_RA_TYPE2;
      }
   }
   if (!numAllocRbs)
   {
      RETVALUE(RFAILED);
   }


   /* Update rbAllocInfo with the allocation information */
   rbAllocInfo->rnti = ue->spsRnti;

   if (rbAllocInfo->raType == RG_SCH_CMN_RA_TYPE0)
   {
      rbAllocInfo->allocInfo.raType0.dlAllocBitMask = 
         rbAllocInfo->resAllocInfo.raType0Mask;
      rbAllocInfo->allocInfo.raType0.numDlAlloc = numAllocRbgs;
   }
#ifdef RGSCH_SPS_UNUSED
   else if (rbAllocInfo->raType == RG_SCH_CMN_RA_TYPE1)
   {
      temprbg = rbAllocInfo->allocInfo.raType1.rbgSubset;

      rbAllocInfo->allocInfo.raType1.dlAllocBitMask =
       rbAllocInfo->resAllocInfo.raType1Mask[temprbg];

      rbAllocInfo->allocInfo.raType1.numDlAlloc = numAllocRbs;
      /* Note: shift currently set to 0 since SPS BW is not more than half of
       * total BW, need to determine based on the allocations */
      rbAllocInfo->allocInfo.raType1.shift = 0;
   }
#endif
   else if (rbAllocInfo->raType == RG_SCH_CMN_RA_TYPE2)
   {
      rbAllocInfo->allocInfo.raType2.isLocal = TRUE;
      rbAllocInfo->allocInfo.raType2.rbStart = rbStart;
      rbAllocInfo->allocInfo.raType2.numRb = numAllocRbs;
   }

   rbAllocInfo->rbsAlloc = numAllocRbs;
   rbAllocInfo->tbInfo[0].bytesAlloc = rbAllocInfo->tbInfo[0].bytesReq;
   rbAllocInfo->tbInfo[0].schdlngForTb = TRUE;


   RETVALUE(ROK);
}


/**
 * @brief Performs allocation finalization for active UEs
 *
 * @details
 *
 *     Function : rgSCHCmnSpsDlUeAllocFnlz
 *
 *
 *     Processing Steps:
 *      - Get SPS harq Proc. Call rgSCHDhmSpsDlGetHqProc()
 *        - If HqP not available
 *          - return ROK
 *      - Update DHM with logical channel's data information
 *      - Update the HARQ proc with the allocation information
 *      - Set hqP->isSpsActv = TRUE
 *      - Add HARQ proc to the subframe.
 *      - If successful, 
 *        - if (ue->actionPndng | RG_SCH_CMN_SPS_DL_REACTV)
 *        {
 *          - Call rgSCHCmnSpsDlUeClearAlloc
 *        }
 *        - Update UE's new allocation information in cell-wide sub-frames.
 *          Call rgSCHCmnSpsDlUpdSfOnAlloc.
 *        - Update UE's new allocation information in dlSpsUe.
 *          Call rgSCHCmnSpsDlUpdUeOnAlloc.
 *        - Mark HARQ proc as activation/reactivation HARQ proc 
 *           - hqP->spsAction = ue->actionPndng
 *        - ue->actionPndng = 0
 *        - Remove UE from toBeActvtdUes list
 *        - Remove SPS service from toBeSchdSvcs list 
 *
 *  @param[in]  RgSchCellCb     *cellCb
 *  @param[in]  RgSchUeCb       *ueCb
 *  @param[in]  CmLteTimingInfo schdTime
 *  @param[in]  RgSchDlSf       *dlSf
 *  @param[in]  U8              n1PucchIdx
 *  @param[in]  U8              isActvOrReactv
 *  @return  S16
 *             ROK      success 
 *             RFAILED  failed
 **/
#ifdef ANSI
PRIVATE S16 rgSCHCmnSpsDlUeAllocFnlz
(
RgSchCellCb         *cell,
RgSchUeCb           *ue,
CmLteTimingInfo     schdTime,
RgSchDlSf           *dlSf,
U8                  n1PucchIdx,
U8                  isActvOrReactv
)
#else /* ANSI */
PRIVATE S16 rgSCHCmnSpsDlUeAllocFnlz(cell, ue, schdTime, dlSf, n1PucchIdx,
      isActvOrReactv)
RgSchCellCb         *cell;
RgSchUeCb           *ue;
CmLteTimingInfo     schdTime;
RgSchDlSf           *dlSf;
U8                  n1PucchIdx;
U8                  isActvOrReactv;
#endif /* ANSI */
{
   RgSchCmnDlHqProc        *cmnHqDl;
   RgSchDlHqProcCb         *spsProc;
   RgSchCmnDlUe            *ueDl;

   RgSchLchAllocInfo       lchSchdData;
   RgSchDlRbAlloc          *rbAllocInfo = &ue->dl.dlAllocCb;
   RgSchCmnDlCellSpsInfo   *dlSpsCell = RG_SCH_CMN_SPS_GET_DL_CELL(cell);
   RgSchCmnDlUeSpsInfo     *dlSpsUe   = RG_SCH_CMN_SPS_GET_DL_UE(ue);
   U32                     y;

   TRC2(rgSCHCmnSpsDlUeAllocFnlz);
   /* Retrieve PDCCH */
   rbAllocInfo->pdcch = NULLP;

   /* If dl sps status is still waiting for Activation confirm
    * this means neither ACK or NACK is received for DL SPS
    * ACTVN PDCCH hence there is need to trasmit ACTVN PDCCH again */
   if ((isActvOrReactv)
         || (dlSpsUe->dlSpsStatus == RGSCH_DL_SPS_WAITING_FOR_ACTV_CFM)) 
   {
      y = rgSCHUtlGetY(ue, dlSf->sfNum);
      rbAllocInfo->pdcch = rgSCHCmnPdcchAlloc(cell, dlSf, y, 
                  rbAllocInfo->cqiForTx, rbAllocInfo->dciFormat, FALSE);
      if (!(rbAllocInfo->pdcch))
      {
         RETVALUE(RFAILED);
      }
#ifdef RGSCH_SPS_STATS
      if (dlSpsUe->dlSpsStatus == RGSCH_DL_SPS_WAITING_FOR_ACTV_CFM)
      {
         numSpsActResetDtx++;
      }
#endif
   }

   /* Fetch SPS HARQ proc for activation */
   spsProc = rgSCHDhmSpsDlGetHqProc(ue, schdTime);
   if (!spsProc)
   {
      RLOG_ARG1(L_ERROR,DBG_CELLID,cell->cellId, "rgSCHCmnSpsDlProcToBeActvtdUe: "
               "No SPS Hq Proc Avl.CRNTI:%d",ue->ueId);
      if (rbAllocInfo->pdcch)
      {
         rgSCHUtlPdcchPut(cell, &dlSf->pdcchInfo, rbAllocInfo->pdcch);
         rbAllocInfo->pdcch = NULLP;
      }
      RETVALUE(RFAILED);
   }

   /* If the UE is been re-activated, clear the old allocation for the UE */
   if ((isActvOrReactv) && (dlSpsUe->actionPndng & RG_SCH_CMN_SPS_DL_REACTV))
   {
      /* Except for N1PUCCH resources all other resources are already freed */
      rgSCHCmnSpsDlUeClearN1PucchAlloc(cell, ue, FALSE);
   }

   /* Update UE allocation in cell-wide subframes: replicate the allocation across
    * sub-frames, if activation/re-activation */
   if (isActvOrReactv)
   {
      rgSCHCmnSpsDlUpdSfOnAlloc(cell, ue, schdTime, dlSf, n1PucchIdx);
   }

   /* Update allocation information in dlSpsUe */
   rgSCHCmnSpsDlUpdUeOnAlloc(cell, ue, schdTime, n1PucchIdx);

   /* Update values in the hqProc */
   /* Note: SPS Service data shall not be multiplexed with any other lc Data */
   if (!rbAllocInfo->pdcch)
   {
      /* Allocation without PDCCH, set the n1Pucch resource */
      U8 n1PucchIdxVal = (U8)dlSpsUe->n1PucchIdx[n1PucchIdx];

      spsProc->spsN1PucchRes.pres = TRUE;
      spsProc->spsN1PucchRes.val =
         dlSpsCell->n1PucchDb.n1PucchLst[n1PucchIdxVal].n1PucchVal;
   }
   cmnHqDl = RG_SCH_CMN_GET_DL_HQP(spsProc);
   ueDl = RG_SCH_CMN_GET_DL_UE(ue);
   ueDl->proc = spsProc;
   cmnHqDl->totBytes = rbAllocInfo->tbInfo[0].bytesAlloc;
   cmnHqDl->isSpsSvcSchd = TRUE;
   cmnHqDl->isSpsActv = TRUE;
   cmnHqDl->spsAction = dlSpsUe->actionPndng; 
   rbAllocInfo->tbInfo[0].tbCb = &spsProc->tbInfo[0];
   rbAllocInfo->tbInfo[0].noLyr = 1;
   rbAllocInfo->dlSf = dlSf;
   spsProc->tbInfo[0].subFrm = dlSf; 


   /* Update maximum re-transmission time for this SPS HARQ proc */
#ifdef LTE_TDD
   if ((ue->dl.dlSpsCfg.dlSpsPrdctyEnum * ue->dl.dlSpsCfg.numSpsHqProc) >
         (RG_SCH_CMN_HARQ_INTERVAL + TFU_HQFBKIND_ULDELTA + 
          rgSchTddSpsDlMaxRetxTbl[cell->ulDlCfgIdx]))
   {
      RG_SCH_ADD_TO_CRNT_TIME(schdTime, cmnHqDl->maxRetxTime , 
            (ue->dl.dlSpsCfg.dlSpsPrdctyEnum * ue->dl.dlSpsCfg.numSpsHqProc)
            - (RG_SCH_CMN_HARQ_INTERVAL + TFU_HQFBKIND_ULDELTA + 
               rgSchTddSpsDlMaxRetxTbl[cell->ulDlCfgIdx])); 
   }
   else
   {
      cmnHqDl->maxRetxTime = schdTime;
   }
#else
   RG_SCH_ADD_TO_CRNT_TIME(schdTime, cmnHqDl->maxRetxTime , 
         (ue->dl.dlSpsCfg.dlSpsPrdctyEnum * ue->dl.dlSpsCfg.numSpsHqProc)
         - (RG_SCH_CMN_HARQ_INTERVAL + RG_SCH_CMN_DL_DELTA + 
         TFU_HQFBKIND_ULDELTA)); 
#endif
   /* Update DHM for the sps service*/
   lchSchdData.lcId     = dlSpsUe->spsSvc->lcId;
   lchSchdData.schdData = rbAllocInfo->tbInfo[0].bytesAlloc;
   rgSCHDhmAddLcData(cell->instIdx, &lchSchdData, &spsProc->tbInfo[0]);

   /* Schedule TA if pending for the UE */
   if (ue->dlTaLnk.node)
   {
      rgSCHCmnRmvFrmTaLst(cell, ue);
      rgSCHDhmSchdTa(ue, &spsProc->tbInfo[0]);
   }
   if (rbAllocInfo->pdcch)
   {
      /* Fill Activation/re-activation PDCCH and add the HARQ proc to sub-frame
       * for transmission */
      rgSCHCmnSpsDlFillActvPdcch(cell, ue, rbAllocInfo->pdcch, FALSE, 0);
   }
   else
   {
      /* Fill up the TB of HARQ process */
      /* Add the HARQ process to the sub-frame */

      rgSCHCmnSpsDlFillActvPdcch(cell, ue, &ue->dl.spsOccPdcch, TRUE,
            spsProc->procId);
   }

   /* Reset UE but update spsSchdTime correctly */
   rgSCHCmnDlUeResetTemp(ue, spsProc);
   rbAllocInfo->spsSchdTime = schdTime;

   /* Update stats */
   dlSpsUe->statInfo.numSchedSPSRnti++;
   dlSpsUe->statInfo.totalSPSSchedOcc++;
#ifdef RGSCH_SPS_STATS
   ue->lastSpsLcSchedTime = cell->crntTime;
   ue->absTimeSched = glblTtiCnt;

   numSPSSched++;
#endif
   RETVALUE(ROK);

} /* rgSCHCmnSpsDlUeAllocFnlz */


/**
 * @brief Clear allocation of a particular SPS UE
 *
 * @details
 *
 *     Function : rgSCHCmnSpsDlUeClearN1PucchAlloc
 *
 *
 *     Processing Steps:
 *     - Remove n1Pucch allocation corresponding to the UE from the
 *       subframes
 *
 *  @param[in]  RgSchCellCb     *cellCb
 *  @param[in]  RgSchUeCb       *ueCb
 *  @return  S16
 *             ROK      success 
 *             RFAILED  failed
 **/
#ifdef ANSI
PRIVATE S16 rgSCHCmnSpsDlUeClearN1PucchAlloc
(
RgSchCellCb         *cell,
RgSchUeCb           *ue,
Bool                 clearDbAlloc
)
#else /* ANSI */
PRIVATE S16 rgSCHCmnSpsDlUeClearN1PucchAlloc(cell, ue)
RgSchCellCb         *cell;
RgSchUeCb           *ue;
Bool                 clearDbAlloc;
#endif /* ANSI */
{
   RgSchCmnDlCellSpsInfo *dlSpsCell = RG_SCH_CMN_SPS_GET_DL_CELL(cell);
   RgSchCmnDlUeSpsInfo   *dlSpsUe   = RG_SCH_CMN_SPS_GET_DL_UE(ue);
   RgSchCmnSpsDlUeSchdInfo *dlSpsUeSchdInfo = &(dlSpsUe->dlSpsUeSchdInfo);
   U32                    maskIdx;
   U32                    maskPos;
   U32                   n1PucchIdxVal;
   RgSchCmnSpsDlN1Pucch  *n1PucchCb;
   U32                   n1PucchMask;
   U32                   repCnt; 
   U32                   sfIdx; 
   U16                   idx;   
   U32                   spsPrd = (U32)ue->dl.dlSpsCfg.dlSpsPrdctyEnum;
   RgSchCmnSpsDlSf       *dlSpsSfArr = NULLP;     
   U32                   lcmVal = dlSpsCell->spsPrdLcmVal;
   U16                  n1PucchSfIdx;

   TRC2(rgSCHCmnSpsDlUeClearN1PucchAlloc);

   /* Releasing of N1PUCCH resources and setting of isSpsHqPInUse is
         separated from clearing of SPS allocations "rgSCHCmnSpsDlUeClearAlloc".
         Because in the case of reactivation, until reactivation is successful
         allocation N1PUCCH to other UEs and allocations of SPS Hq procs for 
         dynamic scheduling shall be avoided */

   /* Compute n1Pucch mask idx and mask position */
   n1PucchIdxVal = dlSpsUe->n1PucchIdx[dlSpsUeSchdInfo->allocN1PucchIdx];
   maskIdx = n1PucchIdxVal/32;
   maskPos = n1PucchIdxVal % 32;
   n1PucchMask = 1 << (31 - maskPos);
   n1PucchCb = &dlSpsCell->n1PucchDb.n1PucchLst[n1PucchIdxVal];

   sfIdx = dlSpsUeSchdInfo->actvSfTblIdx;
   dlSpsSfArr = dlSpsCell->spsSfTbl;
   repCnt =  lcmVal/spsPrd;
#ifdef LTE_TDD      
      n1PucchSfIdx = (sfIdx + 
         rgSchTddInvDlAscSetIdxTbl[cell->ulDlCfgIdx][sfIdx]) % lcmVal;
#else
      n1PucchSfIdx = sfIdx;
#endif
    
   /* Clear allocation information from SPS sub-frames occuring at the UE's
    * periodicity */
   for (idx = 0; idx < repCnt; ++idx)
   {      
      if (dlSpsSfArr[n1PucchSfIdx].n1PucchMask[maskIdx] & n1PucchMask)
      {
         dlSpsSfArr[n1PucchSfIdx].n1PucchMask[maskIdx] ^= n1PucchMask;
      }
      sfIdx = (sfIdx + spsPrd) % lcmVal;
#ifdef LTE_TDD      
      n1PucchSfIdx = (sfIdx + 
         rgSchTddInvDlAscSetIdxTbl[cell->ulDlCfgIdx][sfIdx]) % lcmVal;
#else
      n1PucchSfIdx = sfIdx;
#endif      
   }

   /* Free reserved n1Pucch value */
   if (clearDbAlloc)
   {
      rgSCHCmnSpsDlFreeN1Pucch(&dlSpsCell->n1PucchDb, n1PucchCb);
   }

   ue->dl.isSpsHqPInUse = FALSE;
   
   RETVALUE(ROK);
}


/**
 * @brief Clear allocation of a particular SPS UE
 *
 * @details
 *
 *     Function : rgSCHCmnSpsDlUeClearAlloc
 *
 *
 *     Processing Steps:
 *     - Remove UE from the old index in actvDlSpsUeLsts
 *     - Remove UE's old allocation from the subframes
 *
 *  @param[in]  RgSchCellCb     *cellCb
 *  @param[in]  RgSchUeCb       *ueCb
 *  @return  S16
 *             ROK      success 
 *             RFAILED  failed
 **/
#ifdef ANSI
PRIVATE S16 rgSCHCmnSpsDlUeClearAlloc
(
RgSchCellCb         *cell,
RgSchUeCb           *ue
)
#else /* ANSI */
PRIVATE S16 rgSCHCmnSpsDlUeClearAlloc(cell, ue)
RgSchCellCb         *cell;
RgSchUeCb           *ue;
#endif /* ANSI */
{
   RgSchCmnDlCellSpsInfo *dlSpsCell = RG_SCH_CMN_SPS_GET_DL_CELL(cell);
   RgSchCmnDlUeSpsInfo   *dlSpsUe   = RG_SCH_CMN_SPS_GET_DL_UE(ue);
   U16                   lcmVal = dlSpsCell->spsPrdLcmVal;
   U16                   dlSpsSfIdx;
   U16                   repCnt; 
   U16                   sfIdx; 
   U16                   idx, idx1;
   RgSchCmnSpsDlUeSchdInfo *dlSpsUeSchdInfo = &(dlSpsUe->dlSpsUeSchdInfo);
   U32                   spsPrd = (U32)ue->dl.dlSpsCfg.dlSpsPrdctyEnum;
   RgSchCmnSpsDlSf       *dlSpsSfArr = NULLP;
   RgSchDlRbAlloc        *spsAllocInfo = &dlSpsUeSchdInfo->spsAllocInfo;
   RgSchDlSfAllocInfo    *sfResAllocInfo;
   RgSchDlSf              *dlSf;
   CmLteTimingInfo        frm;
   TRC2(rgSCHCmnSpsDlUeClearAlloc);
#ifdef RGSCH_SPS_STATS 
   ue->lastSpsLcBoRptTime.subframe = 0xFF;
#endif
   /* Compute the indices to be updated */
   dlSpsSfIdx = dlSpsUeSchdInfo->actvSfTblIdx;
   dlSpsSfArr = dlSpsCell->spsSfTbl;
   repCnt =  lcmVal/spsPrd;

   sfIdx = dlSpsSfIdx;

   /* Get Downlink Subframe */
   frm   = cell->crntTime;
   RGSCH_INCR_SUB_FRAME(frm, RG_SCH_CMN_DL_DELTA);
   dlSf = rgSCHUtlSubFrmGet(cell, frm);

   /* Clear the allocation information from current DLSF*/
   sfResAllocInfo = &(dlSf->dlSfAllocInfo);
   sfResAllocInfo->raType0Mask ^=
      spsAllocInfo->resAllocInfo.raType0Mask;
#ifdef RGSCH_SPS_UNUSED
   for (idx1 = 0; idx1 < RG_SCH_NUM_RATYPE1_32BIT_MASK; ++idx1)
   {
      sfResAllocInfo->raType1Mask[idx1] ^= 
         spsAllocInfo->resAllocInfo.raType1Mask[idx1];
      sfResAllocInfo->raType1UsedRbs[idx1] -= 
         spsAllocInfo->resAllocInfo.raType1UsedRbs[idx1];
   }
#endif
   for (idx1 = 0; idx1 < RG_SCH_NUM_RATYPE2_32BIT_MASK; ++idx1)
   {
      sfResAllocInfo->raType2Mask[idx1] ^= 
         spsAllocInfo->resAllocInfo.raType2Mask[idx1];
   }

   /* Clear allocation information from SPS sub-frames occuring at the UE's
    * periodicity */
   for (idx = 0; idx < repCnt; ++idx)
   {
      sfResAllocInfo = &(dlSpsSfArr[sfIdx].spsAllocInfo);
      dlSpsSfArr[sfIdx].numDlSpsActiveUes--;
      sfResAllocInfo->raType0Mask ^=
         spsAllocInfo->resAllocInfo.raType0Mask;
#ifdef RGSCH_SPS_UNUSED
      for (idx1 = 0; idx1 < RG_SCH_NUM_RATYPE1_32BIT_MASK; ++idx1)
      {
         sfResAllocInfo->raType1Mask[idx1] ^= 
            spsAllocInfo->resAllocInfo.raType1Mask[idx1];
         sfResAllocInfo->raType1UsedRbs[idx1] -= 
            spsAllocInfo->resAllocInfo.raType1UsedRbs[idx1];
      }
#endif
      for (idx1 = 0; idx1 < RG_SCH_NUM_RATYPE2_32BIT_MASK; ++idx1)
      {
         sfResAllocInfo->raType2Mask[idx1] ^= 
            spsAllocInfo->resAllocInfo.raType2Mask[idx1];
      }
      dlSpsSfArr[sfIdx].rbsAlloc -= spsAllocInfo->rbsAlloc;
      sfIdx = (sfIdx + spsPrd) % lcmVal;
   }

   /* Remove UE from active UE list */
   rgSCHCmnSpsUtlDlDelFrmActvUes(dlSpsCell, ue, dlSpsUeSchdInfo->schdKey);

   /* Free reserved n1Pucch value */

   /* Clear the scheduling information of the UE */
   cmMemset((U8 *)(dlSpsUeSchdInfo), 0, sizeof(*dlSpsUeSchdInfo));

   /* Mark UE to have invalid SPS scheduling information */
   RGSCH_SPS_SET_DL_SPS_STATE(ue, RGSCH_DL_SPS_IDLE);
   dlSpsUe->prevDlBoUpdTm.subframe = RGSCH_INVALID_SUBFRAME;
   dlSpsUe->maxChgdBo = 0;
   dlSpsUe->reducedBoCount = 0;
   dlSpsUe->dynSchedCount = 0;
   dlSpsUe->numRelPdcchSent = 0;
   dlSpsUe->isDynSched = FALSE;
   dlSpsUeSchdInfo->actvSfTblIdx = dlSpsSfIdx;
   ue->relPdcchTxTime.sfn    = RGSCH_MAX_SFN + 1;
   ue->relPdcchFbkTiming.sfn = RGSCH_MAX_SFN + 1;

   RETVALUE(ROK);
}

/**
 * @brief Update allocation of a UE in the cell-wide subframes.
 *
 * @details
 *
 *     Function : rgSCHCmnSpsDlUpdSfOnAlloc
 *
 *
 *     Processing Steps:
 *        - Replicate the n1Pucch and frequency domain information for the SPS
 *          periodicity over the sub-frames
 *        - Mark UE as ue->isSpsActv = TRUE
 *        - Add UE to actvDlSpsUeLsts at computed index.
 *
 *  @param[in]  RgSchCellCb     *cellCb
 *  @param[in]  RgSchUeCb       *ueCb
 *  @param[in]  CmLteTimingInfo schdTime
 *  @param[in]  RgSchDlSf       *dlSf
 *  @param[in]  U8              n1PucchIdx
 *  @return  S16
 *             ROK      success 
 *             RFAILED  failed
 **/
#ifdef ANSI
PRIVATE S16 rgSCHCmnSpsDlUpdSfOnAlloc
(
RgSchCellCb         *cell,
RgSchUeCb           *ue,
CmLteTimingInfo     schdTime,
RgSchDlSf           *dlSf,
U8                  n1PucchIdx
)
#else /* ANSI */
PRIVATE S16 rgSCHCmnSpsDlUpdSfOnAlloc(cell, ue, schdTime, dlSf, n1PucchIdx)
RgSchCellCb         *cell;
RgSchUeCb           *ue;
CmLteTimingInfo     schdTime;
RgSchDlSf           *dlSf;
U8                  n1PucchIdx;
#endif /* ANSI */
{
   RgSchCmnDlCellSpsInfo *dlSpsCell = RG_SCH_CMN_SPS_GET_DL_CELL(cell);
   RgSchCmnDlUeSpsInfo   *dlSpsUe   = RG_SCH_CMN_SPS_GET_DL_UE(ue);
   U32                   lcmVal = dlSpsCell->spsPrdLcmVal;
   U16                   dlSpsSfIdx;
   U32                   repCnt; 
   U16                   sfIdx; 
   U16                   idx, idx1;
   U32                   spsPrd = (U32)ue->dl.dlSpsCfg.dlSpsPrdctyEnum;
   RgSchCmnSpsDlSf       *dlSpsSfArr = NULLP;
   RgSchDlRbAlloc        *rbAllocInfo = &ue->dl.dlAllocCb;
   U32                    maskIdx;
   U32                    maskPos;
   U32                   n1PucchIdxVal;
   U32                   n1PucchMask;
   RgSchDlSfAllocInfo    *sfResAllocInfo;
   U16                   n1PucchSfIdx; 
   TRC2(rgSCHCmnSpsDlUpdSfOnAlloc);

   /* Compute n1Pucch mask idx and mask position */
   n1PucchIdxVal = dlSpsUe->n1PucchIdx[n1PucchIdx];
   maskIdx = n1PucchIdxVal/32;
   maskPos = n1PucchIdxVal % 32;
   n1PucchMask = 1 << (31 - maskPos);

   /* Compute the indices to be updated */
   dlSpsSfIdx = (schdTime.sfn * 10 + schdTime.subframe) % lcmVal;
   dlSpsSfArr = dlSpsCell->spsSfTbl;
   repCnt =  lcmVal/spsPrd;

   sfIdx = dlSpsSfIdx; 
   for (idx = 0; idx < repCnt; ++idx)
   {
      sfResAllocInfo = &(dlSpsSfArr[sfIdx].spsAllocInfo);

      if (cell->spsCellCfg.maxSpsUePerDlSf > dlSpsSfArr[sfIdx].numDlSpsActiveUes)
      {
         dlSpsSfArr[sfIdx].numDlSpsActiveUes++;
      }
      sfResAllocInfo->raType0Mask |=
         rbAllocInfo->resAllocInfo.raType0Mask;
#ifdef RGSCH_SPS_UNUSED
      for (idx1 = 0; idx1 < RG_SCH_NUM_RATYPE1_32BIT_MASK; ++idx1)
      {
         sfResAllocInfo->raType1Mask[idx1] |= 
            rbAllocInfo->resAllocInfo.raType1Mask[idx1];
         sfResAllocInfo->raType1UsedRbs[idx1] += 
            rbAllocInfo->resAllocInfo.raType1UsedRbs[idx1];
      }
#endif
      for (idx1 = 0; idx1 < RG_SCH_NUM_RATYPE2_32BIT_MASK; ++idx1)
      {
         sfResAllocInfo->raType2Mask[idx1] |= 
            rbAllocInfo->resAllocInfo.raType2Mask[idx1];
      }

#ifdef LTE_TDD      
      n1PucchSfIdx = (sfIdx + 
         rgSchTddInvDlAscSetIdxTbl[cell->ulDlCfgIdx][sfIdx]) % lcmVal;
#else
      n1PucchSfIdx = sfIdx;
#endif
      
      dlSpsSfArr[n1PucchSfIdx].n1PucchMask[maskIdx] |= n1PucchMask;
      dlSpsSfArr[sfIdx].rbsAlloc += rbAllocInfo->rbsAlloc;
      sfIdx = (sfIdx + spsPrd) % lcmVal;
   }

   RETVALUE(ROK);
}

/**
 * @brief Update scheduling information in DL SPS UE on allocation
 *
 * @details
 *
 *     Function : rgSCHCmnSpsDlUpdUeOnAlloc
 *
 *
 *     Processing Steps:
 *     - Update SPS scheduling information in the UE from  rbAllocInfo
 *     - Mark UE as ue->isSpsActv = TRUE
 *     - Add UE to actvDlSpsUeLsts at computed index.
 *
 *  @param[in]  RgSchCellCb     *cellCb
 *  @param[in]  RgSchUeCb       *ueCb
 *  @param[in]  CmLteTimingInfo schdTime
 *  @param[in]  U8              n1PucchIdx
 *  @return  S16
 *             ROK      success 
 *             RFAILED  failed
 **/
#ifdef ANSI
PRIVATE S16 rgSCHCmnSpsDlUpdUeOnAlloc
(
RgSchCellCb         *cell,
RgSchUeCb           *ue,
CmLteTimingInfo     schdTime,
U8                  n1PucchIdx
)
#else /* ANSI */
PRIVATE S16 rgSCHCmnSpsDlUpdUeOnAlloc(cell, ue, schdTime, n1PucchIdx)
RgSchCellCb         *cell;
RgSchUeCb           *ue;
CmLteTimingInfo     schdTime;
U8                  n1PucchIdx;
#endif /* ANSI */
{
   RgSchCmnDlCellSpsInfo *dlSpsCell = RG_SCH_CMN_SPS_GET_DL_CELL(cell);
   RgSchCmnDlUeSpsInfo   *dlSpsUe   = RG_SCH_CMN_SPS_GET_DL_UE(ue);
   U16                   dlSpsSfIdx;
   RgSchCmnSpsDlUeSchdInfo *dlSpsUeSchdInfo = &(dlSpsUe->dlSpsUeSchdInfo);
   U32                   spsPrd = (U32)ue->dl.dlSpsCfg.dlSpsPrdctyEnum;
   RgSchDlRbAlloc        *rbAllocInfo = &ue->dl.dlAllocCb;
   U16                   lcmVal = dlSpsCell->spsPrdLcmVal;

   TRC2(rgSCHCmnSpsDlUpdUeOnAlloc);

   /* Compute the indices to be updated */
   dlSpsSfIdx = (schdTime.sfn * 10 + schdTime.subframe) % lcmVal;

   /* Update the scheduling information in the UE */
   dlSpsUeSchdInfo->spsAllocInfo = *rbAllocInfo;
   dlSpsUeSchdInfo->allocN1PucchIdx = n1PucchIdx; 
   dlSpsUeSchdInfo->actvSfTblIdx =  dlSpsSfIdx;

   /* If UE exists in active list, remove from current index */
   if (dlSpsUe->actvUeLstEnt.node)
   {
      rgSCHCmnSpsUtlDlDelFrmActvUes(dlSpsCell, ue, dlSpsUeSchdInfo->schdKey);
   }

   /* Add UE to the active list of UEs at new index */
   RG_SCH_ADD_TO_CRNT_TIME(schdTime, dlSpsUeSchdInfo->schdKey , spsPrd);
   rgSCHCmnSpsUtlDlAdd2ActvUes(dlSpsCell, ue, dlSpsUeSchdInfo->schdKey);

   /* Update scaledCqi and pdcchPndg variable */
   dlSpsUeSchdInfo->scaledCqi = rbAllocInfo->cqiForTx;

   /* Mark UE to have valid SPS scheduling information */
   ue->dl.isSpsHqPInUse = TRUE;

   RETVALUE(ROK);
}

/**
 * @brief Handles services to be scheduled at non-SPS ocassions
 *
 * @details
 *
 *     Function : rgSCHCmnSpsDlProcToBeSchdSvcs
 *
 *
 *     Processing Steps:
 *     - For each service in the toBeSchdSvcs,
 *      - Schedule the service using CRNTI
 *      - Call rgSCHCmnSpsDlAllocForSvc()
 *
 *  @param[in]  RgSchCellCb     *cell
 *  @param[in]  CmLteTimingInfo crntTime
 *  @param[in]  RgSchDlSf       *dlSf
 *  @param[out] RgSchCmnDlRbAllocInfo *allocInfo
 *  @return  Void
 **/
#ifdef ANSI
PRIVATE Void rgSCHCmnSpsDlProcToBeSchdSvcs
(
RgSchCellCb           *cell,
CmLteTimingInfo       crntTime,
RgSchDlSf             *dlSf,
RgSchCmnDlRbAllocInfo *allocInfo
)
#else /* ANSI */
PRIVATE Void rgSCHCmnSpsDlProcToBeSchdSvcs(cell, crntTime, dlSf, allocInfo)
RgSchCellCb           *cell;
CmLteTimingInfo       crntTime;
RgSchDlSf             *dlSf;
RgSchCmnDlRbAllocInfo *allocInfo;
#endif /* ANSI */
{
   RgSchCmnDlCellSpsInfo *dlSpsCell = RG_SCH_CMN_SPS_GET_DL_CELL(cell);
   RgSchDlLcCb           *dlLcCb;
   RgSchCmnDlSvcSpsInfo  *dlSpsSvc;
   CmLList               *schdSvcNode;
   CmLteTimingInfo       schdTime;
   Bool                  bwExp = FALSE;

   TRC2(rgSCHCmnSpsDlProcToBeSchdSvcs);

   /* Increment by DL DELTA to determine the time for which scheduling
    * is done */
   RG_SCH_ADD_TO_CRNT_TIME(crntTime, schdTime, RG_SCH_CMN_DL_DELTA);

   CM_LLIST_FIRST_NODE(&dlSpsCell->toBeSchdSvcs, schdSvcNode);
   while (dlSf->remUeCnt && schdSvcNode)
   {
      dlLcCb = (RgSchDlLcCb *)schdSvcNode->node;
      dlSpsSvc = RG_SCH_CMN_SPS_GET_DL_SVC(dlLcCb);
      if (dlLcCb->bo)
      {
         if (rgSCHCmnSpsDlAllocForSvc(cell, 
                  schdTime, dlSf, dlLcCb,
                  dlLcCb->bo, allocInfo, &bwExp) != ROK)
         {
            if (bwExp == TRUE)
            {
               RETVOID;
            }
         }
         else
         {
            dlSf->remUeCnt--;
         }
      }
      else
      {
         rgSCHCmnSpsUtlDlDelFrmToBeSchdSvcs(dlSpsCell, dlLcCb);
      }
      CM_LLIST_NEXT_NODE(&dlSpsCell->toBeSchdSvcs, schdSvcNode);      
   }


   RETVOID;
}

/**
 * @brief Handles to be released SPS UEs
 *
 * @details
 *
 *     Function : rgSCHCmnSpsDlProcToBeRelUes
 *
 *
 *     Processing Steps:
 *     -For  UE in the list of toBeRelUes,
 *       - Fetch PDCCH for the UE
 *       - If successful, 
 *          - Set the bits according to the one needed for releasing
 *          - Update the feedback timing information in the PDCCH
 *          - Add PDCCH to the subframe to the relPdcchs list 
 *          - set ue->isRelPdcchSent = TRUE
 *        else
 *          - Move UE to the end of the queue
 *
 *
 *  @param[in]  RgSchCellCb           *cell
 *  @parama[in] CmLteTimingInfo       crntTime
 *  @param[in]  RgSchDlSf             *dlSf
 *  @return  Void
 **/
#ifdef ANSI
PRIVATE Void rgSCHCmnSpsDlProcToBeRelUes
(
RgSchCellCb           *cell,
CmLteTimingInfo       crntTime,
RgSchDlSf             *dlSf
)
#else /* ANSI */
PRIVATE Void rgSCHCmnSpsDlProcToBeRelUes(cell, crntTime, dlSf)
RgSchCellCb           *cell;
CmLteTimingInfo       crntTime;
RgSchDlSf             *dlSf;
#endif /* ANSI */
{
   RgSchCmnDlUe          *cmnUeDl = NULLP;
   RgSchCmnDlCellSpsInfo *dlSpsCell = RG_SCH_CMN_SPS_GET_DL_CELL(cell);
   CmLList               *listNode  = dlSpsCell->toBeRelUes.first;
   RgSchUeCb             *ue;
   CmLteTimingInfo       schdTime;
   U8                    scCqi;
   U32                   y;
   RgSchPdcch            *relPdcch;
   RgSchCmnDlUeSpsInfo   *dlSpsUe;
   Bool                 isNackDtx = FALSE;

   CmLteTimingInfo       relFdbkTime = {0};
   
   TRC2(rgSCHCmnSpsDlProcToBeRelUes);
   
   /* Increment by DL DELTA to determine the time for which scheduling
    * is done */
   RG_SCH_ADD_TO_CRNT_TIME(crntTime, schdTime, RG_SCH_CMN_DL_DELTA);

   while (listNode)
   {
      ue       = (RgSchUeCb *)(listNode->node);
      cmnUeDl  = RG_SCH_CMN_GET_DL_UE(ue);
      dlSpsUe = RG_SCH_CMN_SPS_GET_DL_UE(ue);
      listNode = listNode->next;

      if (!dlSpsUe->isRelPdcchSent)
      {
         if ( (dlSpsUe->numRelPdcchSent >= cell->dlHqCfg.maxDlHqTx))
         {
            /* Remove UE from to be released UEs list */
            rgSCHCmnSpsUtlDlDelFrmToBeRelUes(dlSpsCell, ue);

            /* Set the state to SPS IDLE */ 
            RGSCH_SPS_SET_DL_SPS_STATE(ue, RGSCH_DL_SPS_IDLE);

            RLOG_ARG4(L_DEBUG,DBG_CELLID,cell->cellId,
                     "DL SPS MAX RETX for Release PDCCH due to DTX"
                     "sent %d max %d at SFN(%d), UEID:%d",
                     dlSpsUe->numRelPdcchSent ,cell->dlHqCfg.maxDlHqTx, 
                     cell->crntTime.sfn, ue->ueId);
            RLOG_ARG2(L_DEBUG,DBG_CELLID,cell->cellId,
                     "subframe(%d) UEID:%d",
                     cell->crntTime.subframe,ue->ueId);
            /* If Ue's SPS service has BO, add to toBeSchdSvcs list */
            if (dlSpsUe->spsSvc)
            {
               RgSchCmnDlSvc  *spsSvcCmn = RG_SCH_CMN_GET_DL_SVC(dlSpsUe->spsSvc);
               if(spsSvcCmn->dlSvcSpsInfo.effSpsBo)
               {
                  rgSCHCmnSpsUtlDlAdd2ToBeSchdSvcs(dlSpsCell, dlSpsUe->spsSvc);
               }
            }

            continue;
         }

         /* Check if UE can be scheduled in DL */
         if (rgSCHCmnSpsDlChkUeInActv(cell, ue, schdTime))
         {
            
            continue;
         }

         /* Determine the scaled CQI value for SPS transmission */
         scCqi = dlSpsUe->dlSpsUeSchdInfo.scaledCqi;
            
         if (dlSpsUe->numRelPdcchSent)
         {
            isNackDtx = TRUE;
         }
         /* Retrieve PDCCH using DCI format 1A */
         y = rgSCHUtlGetY(ue, dlSf->sfNum);
         relPdcch = rgSCHCmnPdcchAlloc(cell, dlSf, y, scCqi,\
                     TFU_DCI_FORMAT_1A, isNackDtx);
         if (!relPdcch)
         {
           
            continue;
         }

         /* Update the transmission time of relPdcch */
         ue->relPdcchTxTime = schdTime;

         /* Update the expected feedback timing for release PDCCH */
#ifndef LTE_TDD
         RG_SCH_ADD_TO_CRNT_TIME(schdTime, ue->relPdcchFbkTiming, 
               RG_SCH_CMN_HARQ_INTERVAL);
#else
         /* Doing equivalent of what is done in rgSCHCmnUpdHqAndDai() */
          /* for timingInfo in tbCb, equivalent of which should be use in place
          * of 'schdTime' below. */
         ue->relPdcchFbkTiming.sfn = (schdTime.sfn + \
               dlSf->dlFdbkInfo.sfnOffset) % RGSCH_MAX_SFN;
         ue->relPdcchFbkTiming.subframe = dlSf->dlFdbkInfo.subframe;
         ue->relPdcchValm = dlSf->dlFdbkInfo.m;
         rgSCHCmnUpdDai(ue, &ue->relPdcchFbkTiming, ue->relPdcchValm, TRUE);
#endif

         /* UE is added to wtngForRelFdbkUeLst at 
          * expected fdb time + TFU_HQFBKIND_ULDELTA tti.
          * At fdb+HQ_IND_DELTA  tti if the UE still
          * exists in wtngForRelFdbkUeLst, we know for sure
          * UE has not received any feedback*/
         RG_SCH_ADD_TO_CRNT_TIME(ue->relPdcchFbkTiming,relFdbkTime,
                            TFU_HQFBKIND_ULDELTA ); 
         cmLListAdd2Tail(&(dlSpsCell->wtngForRelFdbkUeLst[relFdbkTime.subframe]),
                         &(dlSpsUe->wtngForRelFdbkUeEnt));
         dlSpsUe->wtngForRelFdbkUeEnt.node = (PTR)ue;

         /* Note: Add UE's normal RNTI so that the feedback for the same  is
          * recieved with normal RNTI itself and UE can be fetched using the
          * same */
         /* Fill up release PDCCH info */
         rgSCHCmnSpsDlFillRelPdcch(ue, relPdcch);

         /* Note: Currently no list maintained in dlSf since only one release 
          * PDCCH is scheduled per sub-frame. Add release PDCCH to the subframe */
         dlSf->relPdcch                 = relPdcch;
#ifdef LTE_TDD
         dlSf->relPdcchInfo.crnti       = ue->ueId;
         dlSf->relPdcchInfo.ackNackMode = ue->dl.ackNackMode;
#endif /*LTE_TDD*/

         /* Mark UE to have release PDCCH sent */
         dlSpsUe->isRelPdcchSent = TRUE;
         dlSpsUe->numRelPdcchSent++;

         dlSpsUe->statInfo.numSpsRel++;

#ifdef RGSCH_SPS_STATS
         numdlSpsRel++;
#endif

         /* Mark UE to be scheduled by SPS */
         ue->dl.dlAllocCb.spsSchdTime = schdTime;

         /* Breaking from the loop since only 1 Release PDCCH to be scheduled per
          * sub-frame */
         break;
      }
   }

   RETVOID;
}

/**
 * @brief This function Processes the Final Allocations
 *        made by the RB Allocator against the requested. 
 *
 * @details
 *
 *     Function: rgSCHCmnSpsDlAllocFnlz
 *     Purpose:  This function Processes the Final Allocations
 *               made by the RB Allocator against the requested. 
 *               1. Loop through scheduled TX and RETX lists.
 *                  Fill in the corresponding PDCCH and HqProcs.
 *                  Remove the satisfied services from the toBeSchdSvcs list
 *                  Remove the satisfied retx Hq Procs from retxHqProcs list
 *               2. Loop through UnScheduled RETX Lists.
 *                  Release grabbed HqProcs.
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
PUBLIC Void rgSCHCmnSpsDlAllocFnlz
(
RgSchCellCb           *cell,
RgSchCmnDlRbAllocInfo *allocInfo
)
#else /* ANSI */
PUBLIC Void rgSCHCmnSpsDlAllocFnlz(cell, allocInfo)
RgSchCellCb           *cell;
RgSchCmnDlRbAllocInfo *allocInfo;
#endif /* ANSI */
{

   TRC2(rgSCHCmnSpsDlAllocFnlz);

   
   rgSCHCmnSpsDlRetxAllocFnlz(cell, allocInfo);

   /* Tx Allocations would include allocation for SPS service during 
    * non-SPS ocassions  and SPS ocassions ( when BO does not satisfy packet
    * size constraints) */
   rgSCHCmnSpsDlTxAllocFnlz(cell, allocInfo);


   RETVOID;
}

/**
 * @brief This function Processes the Final Allocations
 *        made by the RB Allocator against the requested
 *        RETX allocations. 
 *
 * @details
 *
 *     Function: rgSCHCmnSpsDlRetxAllocFnlz 
 *     Purpose : Remove the Retx Hq Proc from the Cell's
 *     Retx list, if allocation is successful.
 *     Fill the HqProc and PDCCH and append it to the SubFrm.
 *     
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
PRIVATE Void rgSCHCmnSpsDlRetxAllocFnlz 
(
RgSchCellCb           *cell,
RgSchCmnDlRbAllocInfo *allocInfo
)
#else /* ANSI */
PRIVATE Void rgSCHCmnSpsDlRetxAllocFnlz(cell, allocInfo)
RgSchCellCb           *cell;
RgSchCmnDlRbAllocInfo *allocInfo;
#endif /* ANSI */
{
   CmLList          *node;
   RgSchCmnDlCellSpsInfo *dlSpsCell = RG_SCH_CMN_SPS_GET_DL_CELL(cell);
   RgSchUeCb        *ue;
   RgSchDlHqProcCb  *hqP;
   RgSchCmnDlHqProc *cmnHqDl;


   TRC2(rgSCHCmnSpsDlRetxAllocFnlz);

   node = allocInfo->dedAlloc.schdRetxSpsHqPLst.first;
   while(node)
   {
      hqP = (RgSchDlHqProcCb *)(node->node);
      ue = hqP->hqE->ue;
      node = node->next;
      /* Fill PDCCH and assign it to HqP */
      cmnHqDl = RG_SCH_CMN_GET_DL_HQP(hqP);
      if (cmnHqDl->isSpsActv)
      {
         /* If HARQ proc used for SPS ocassion, set NDI = 1, use SPS rnti for
          * retx */
         ue->dl.dlAllocCb.tbInfo[0].tbCb->ndi = 1;
         ue->dl.dlAllocCb.rnti = ue->spsRnti;         
      }
      rgSCHCmnFillHqPPdcch(cell, &ue->dl.dlAllocCb, hqP);
      
      /* Integration_fix */
      /* Reseting cntrRetxAllocFail since hqP entry is added to
         Harq Entity's inUse list */
      hqP->tbInfo[0].cntrRetxAllocFail = 0;
      if (hqP->tbInfo[1].txCntr)
      {
         hqP->tbInfo[1].cntrRetxAllocFail = 0;
      }

      /* Extra Check: Retain the hqProc in the RETX Queue if one/more
       * TBs of the HqProc are yet to be scheduled for RETX.
       * Note: Here we are not tracking at TB Level, the priority Q
       * to which it belongs to. The retx prio of transmission is still
       * being maintained at hqProc level, rather than at TB level */
      if ((hqP->tbInfo[0].state != HQ_TB_NACKED) && 
          (hqP->tbInfo[1].state != HQ_TB_NACKED)) {
         rgSCHCmnSpsUtlDlDelFrmRetxHqProcs(dlSpsCell, hqP);
      }
      /* reset the UE allocation Information */
      rgSCHCmnDlUeResetTemp(ue, hqP);
   }

   /* Traverse the nonSchdTxUeLst and reset the UE allocation Info */
   node = allocInfo->dedAlloc.nonSchdRetxSpsHqPLst.first;
   while(node)
   {
      hqP = (RgSchDlHqProcCb *)(node->node);
      ue = hqP->hqE->ue;
      node = node->next;
      /* reset the UE allocation Information */
      rgSCHCmnDlUeResetTemp(ue, hqP);
   }
   RETVOID;
}

/**
 * @brief This function Processes the Final Allocations
 *        made by the RB Allocator against the requested
 *        New TX allocations for SPS services. 
 *
 * @details
 *
 *     Function: rgSCHCmnSpsDlTxAllocFnlz 
 *     Purpose :
 *     Fill pdcch and HqP for UEs will allocations.
 *     Release HqP for UE with no allocation.
 *
 *     Invoked by: Common Scheduler 
 *     
 *  @param[in]  RgSchCellCb           *cell
 *  @param[in]  RgSchCmnDlRbAllocInfo *allocInfo
 *  @return  Void
 *
 **/
#ifdef ANSI
PRIVATE Void rgSCHCmnSpsDlTxAllocFnlz 
(
RgSchCellCb           *cell,
RgSchCmnDlRbAllocInfo *allocInfo
)
#else /* ANSI */
PRIVATE Void rgSCHCmnSpsDlTxAllocFnlz(cell, allocInfo)
RgSchCellCb           *cell;
RgSchCmnDlRbAllocInfo *allocInfo;
#endif /* ANSI */
{
   CmLList          *node;
   RgSchUeCb        *ue;
   RgSchDlHqProcCb  *hqP;
   RgSchDlRbAlloc   *ueAllocInfo; 

   TRC2(rgSCHCmnSpsDlTxAllocFnlz);

   node = allocInfo->dedAlloc.schdTxSpsHqPLst.first;
   while(node)
   {
      hqP = (RgSchDlHqProcCb *)(node->node);
      ue = hqP->hqE->ue;
      node = node->next;
      ueAllocInfo = &ue->dl.dlAllocCb;
      rgSCHCmnSpsDlNewTxUeFnlz(cell, ueAllocInfo, ue);
      rgSCHCmnDlUeResetTemp(ue, hqP);
   }

   /* Traverse the nonSchdTxUeLst and reset the UE allocation Info */
   node = allocInfo->dedAlloc.nonSchdTxSpsHqPLst.first;
   while(node)
   {
      hqP = (RgSchDlHqProcCb *)(node->node);
      ue = hqP->hqE->ue;
      node = node->next;

      /* Release HqProc */
      rgSCHDhmRlsHqpTb(hqP, 0, FALSE);
      if (hqP->tbInfo[1].txCntr)
      {
         rgSCHDhmRlsHqpTb(hqP, 1, FALSE);
      }
      /* reset the UE allocation Information */
      rgSCHCmnDlUeResetTemp(ue, hqP);
   }
   RETVOID;
}

/***********************************************************
 *
 *     Func : rgSCHCmnSpsDlNewTxUeFnlz 
 *        
 *     Desc : Perform UE finalization for allocated bytes.
 *
 *     Ret  : Void 
 *
 *     Notes:
 *
 *     File : 
 *
 **********************************************************/
#ifdef ANSI
PRIVATE Void rgSCHCmnSpsDlNewTxUeFnlz 
(
RgSchCellCb    *cell,
RgSchDlRbAlloc *ueAllocInfo, 
RgSchUeCb      *ue
)
#else /* ANSI */
PRIVATE Void rgSCHCmnSpsDlNewTxUeFnlz(cell, ueAllocInfo, ue)
RgSchCellCb    *cell;
RgSchDlRbAlloc *ueAllocInfo;
RgSchUeCb      *ue;
#endif /* ANSI */
{
   RgSchCmnDlCellSpsInfo *dlSpsCell = RG_SCH_CMN_SPS_GET_DL_CELL(cell);
   RgSchCmnDlUe        *cmnUeDl = RG_SCH_CMN_GET_DL_UE(ue);
   RgSchCmnDlUeSpsInfo *dlSpsUe = RG_SCH_CMN_SPS_GET_DL_UE(ue);
   U32                 bytesAlloc = ueAllocInfo->tbInfo[0].bytesAlloc;
   RgSchCmnDlSvc       *spsSvcCmn = RG_SCH_CMN_GET_DL_SVC(dlSpsUe->spsSvc);
   U32                 remBytes;
   U32                 effBo = spsSvcCmn->dlSvcSpsInfo.effSpsBo;
   RgSchLchAllocInfo   lchSchdData;

   TRC2(rgSCHCmnSpsDlNewTxUeFnlz);

   /* Handle SPS service */
   if (bytesAlloc)
   {
      /* Determine how much BO is satisfied */
      if (bytesAlloc <= spsSvcCmn->dlSvcSpsInfo.hdrEst)
      {
         /* Allocation such that Nothing could be satisfied */
         /* Return the grabbed PDCCH */
         rgSCHUtlPdcchPut(cell, &ueAllocInfo->dlSf->pdcchInfo, 
               ueAllocInfo->pdcch);
         rgSCHDhmRlsHqpTb(cmnUeDl->proc, 0, FALSE);
         if (cmnUeDl->proc->tbInfo[1].txCntr)
         {
            rgSCHDhmRlsHqpTb(cmnUeDl->proc, 1, FALSE);
         }
         RETVOID;
      }
      remBytes = bytesAlloc - spsSvcCmn->dlSvcSpsInfo.hdrEst;
      effBo =  effBo > remBytes ? (effBo - remBytes) : 0;

      /* Remove UE from toBeSchdSvcs list, if present */
      if (spsSvcCmn->dlSvcSpsInfo.toBeSchdSvcEnt.node)
      {
         rgSCHCmnSpsUtlDlDelFrmToBeSchdSvcs(dlSpsCell, dlSpsUe->spsSvc);
      }
      if (effBo)
      {
         /* Add to tail to give round robin effect */
         rgSCHCmnSpsUtlDlAdd2ToBeSchdSvcs(dlSpsCell, dlSpsUe->spsSvc); 
      }
      spsSvcCmn->dlSvcSpsInfo.effSpsBo = effBo;

      /* Update DHM for this SVC */
      lchSchdData.lcId     = dlSpsUe->spsSvc->lcId;
      lchSchdData.schdData = bytesAlloc;
      rgSCHDhmAddLcData(cell->instIdx, &lchSchdData, 
            &(cmnUeDl->proc->tbInfo[0]));
      /* Fill PDCCH and assign it to HqP */
      rgSCHCmnFillHqPPdcch(cell, ueAllocInfo, cmnUeDl->proc);
      
      /* Update stats */
      dlSpsUe->statInfo.totalSPSSchedOcc++;
#ifdef RGSCH_SPS_STATS
      ue->lastSpsLcSchedTime = cell->crntTime;
      ue->absTimeSched = glblTtiCnt;
      numDynSched++;
#endif
   }

   RETVOID;
}


/**
 * @brief Handles allocation for an SPS service of a UE
 *
 * @details
 *
 *     Function : rgSCHCmnSpsDlAllocForSvc
 *
 *
 *     Processing Steps:
 *     - Get non-SPS harq Proc. Call rgSCHDhmGetAvlHqProc()
 *     - Determine header size estimate for the service.
 *     - Determine RB requirement for the service's BO. Call rgSCHCmnDlAllocRb()
 *     - Update the HARQ proc with the allocation
 *     - Add UE to the txSpsUeLst in allocInfo structure. 
 *       Call rgSCHCmnDlRbInfoAddSpsUeTx().
 *
 *  @param[in]  RgSchCellCb            *cell
 *  @param[in]  CmLteTimingInfo        schdTime
 *  @param[in]  RgSchDlSf              *subFrm
 *  @param[in]  RgSchDlLcCb            *svc
 *  @param[in]  U32                    bo
 *  @param[in]  RgSchCmnDlRbAllocInfo  *allocInfo
 *  @return  S16 
 **/
#ifdef ANSI
PRIVATE S16 rgSCHCmnSpsDlAllocForSvc
(
RgSchCellCb                *cell,
CmLteTimingInfo            schdTime,
RgSchDlSf                  *subFrm,
RgSchDlLcCb                *svc,
U32                        bo,
RgSchCmnDlRbAllocInfo      *allocInfo,
Bool                       *bwExpired
)
#else /* ANSI */
PRIVATE S16 rgSCHCmnSpsDlAllocForSvc(cell, schdTime, subFrm, svc, bo,
      allocInfo, bwExpired)
RgSchCellCb                *cell;
CmLteTimingInfo            schdTime;
RgSchDlSf                  *subFrm;
RgSchDlLcCb                *svc;
U32                        bo;
RgSchCmnDlRbAllocInfo      *allocInfo;
Bool                       *bwExpired;
#endif /* ANSI */
{
   RgSchUeCb               *ue;
   RgSchDlHqProcCb         *proc;
   U16                     rlcHdrEstmt;
   U32                     effBo;
   RgSchCmnDlCell          *cmnCellDl = RG_SCH_CMN_GET_DL_CELL(cell);
   RgSchCmnDlSvc           *svcCmn = RG_SCH_CMN_GET_DL_SVC(svc);
   RgSchCmnDlUe            *ueDl;
   RgSchCmnDlHqProc        *cmnHqDl;
   RgSchCmnDlUeSpsInfo     *dlSpsUe  = NULLP;

   S16                     ret;
   TRC2(rgSCHCmnSpsDlAllocForSvc);

   *bwExpired = FALSE;
   /* Get the UE to which this service belongs to */
   ue = svc->ue;
   ueDl = RG_SCH_CMN_GET_DL_UE(ue);
   dlSpsUe  = RG_SCH_CMN_SPS_GET_DL_UE(ue);
   
   /* Check if this UE is already scheduled for transmission/
    * active to be scheduled */
   if (rgSCHCmnSpsDlChkUeInActv(cell, ue, schdTime))
   {
      
      RETVALUE(RFAILED);
   }

   if (rgSCHDhmGetAvlHqProc(ue, cmnCellDl->time, &proc) != ROK)
   {
      RLOG_ARG1(L_ERROR,DBG_CELLID,cell->cellId,  "rgSCHCmnSpsDlAllocForSvc: "
               "No Hq Proc Avl. CRNTI:%d",ue->ueId);
      
      RETVALUE(RFAILED);
   }
   proc->spsN1PucchRes.pres = FALSE;
   cmnHqDl = RG_SCH_CMN_GET_DL_HQP(proc);
   cmnHqDl->isSpsSvcSchd = TRUE;
   cmnHqDl->isSpsActv = FALSE;
   cmnHqDl->spsAction = 0;
   ue->dl.dlAllocCb.rbsReq = 0;
 
   /* Including each SDU's header size */
   RG_SCH_CMN_DL_GET_HDR_EST(svc, rlcHdrEstmt);
   bo += rlcHdrEstmt;
   
   /* Schedule TA if pending for the UE */
   if (ue->dlTaLnk.node)
   {
      rgSCHCmnRmvFrmTaLst(cell, ue);
      rgSCHDhmSchdTa(ue, &proc->tbInfo[0]);
      bo += RGSCH_TA_SIZE;
   }
   
   effBo = 0;
   ret = rgSCHCmnDlAllocTxRb(cell, subFrm, ue, bo, &effBo, proc, allocInfo); 
   if ((ret != ROK) || (effBo == 0))
   {
      /* If no allocations so far, meaning proc obtained now */
      rgSCHDhmRlsHqpTb(proc, 0, FALSE);

      /*ccpu00135985 - Modify - To avod segmentation fault*/
      if(proc->tbInfo[1].txCntr)
      {
         rgSCHDhmRlsHqpTb(proc, 1, FALSE);
      }
      cmnHqDl->isSpsSvcSchd = FALSE;
      *bwExpired = TRUE;

      RETVALUE(RFAILED);
   }

   ueDl->proc = proc;
   cmnHqDl->totBytes = effBo;
   svcCmn->dlSvcSpsInfo.bytesReq = effBo;
   svcCmn->dlSvcSpsInfo.hdrEst = (U8)rlcHdrEstmt;
   ue->dl.dlAllocCb.spsSchdTime = schdTime;
   if (dlSpsUe->dlSpsStatus == RGSCH_DL_SPS_ACTV)
   {
      dlSpsUe->isDynSched = TRUE;
   }

   rgSCHCmnSpsUtlDlAdd2TxSpsUeLst(allocInfo, proc);
   RETVALUE(ROK);
}

/**
 * @brief Validates the UE configuration for SPS UE.
 *
 * @details
 *
 *     Function : rgSCHCmnSpsVldtDlCellCfg
 *
 *
 *     Processing Steps:
 *      - Validate the configuration with respect to SPS.
 *      - Validate that SPS BW <= 50% total DL BW
 *
 *  @param[in]   RgSchCellCb   *cell
 *  @param[in]   RgrCellCfg    *cellCfg
 *  @return  S16
 *      -# ROK
 **/
#ifdef ANSI
PRIVATE S16 rgSCHCmnSpsVldtDlCellCfg 
(
RgSchCellCb        *cell,
RgrCellCfg         *cellCfg
)
#else /* ANSI */
PRIVATE S16 rgSCHCmnSpsVldtDlCellCfg (cell, cellCfg) 
RgSchCellCb        *cell;
RgrCellCfg         *cellCfg;
#endif /* ANSI */
{
   TRC2(rgSCHCmnSpsVldtDlCellCfg);
   
   if (cellCfg->spsCfg.maxSpsDlBw > cell->bwCfg.dlTotalBw)
   {
      RLOG_ARG0(L_ERROR,DBG_CELLID,cell->cellId,
            "rgSCHCmnSpsVldtDlCellCfg:Invalid BW configuration");
      return RFAILED;
   }
   RETVALUE(ROK);
}

/**
 * @brief Validates the UE configuration for SPS UE.
 *
 * @details
 *
 *     Function : rgSCHCmnSpsVldtDlUeCfg
 *
 *
 *     Processing Steps:
 *      - Validate the configuration with respect to SPS.
 *      - If DL SPS is eanbled, 
 *        - If ack-nack Repetion is enabled, return RFAILED.
 *        - If measGap is enabled, 
 *          - If SPS periodicity is multiple of measGap period (40/80 ms),
 *          numOfAvailSpsOffset = SPS periodicity - (periodicity/MGP * 10); 
 *        - If DRX is enabled,
 *          - Validate that SPS periodicity is multiple of long_drx_cylce
 *          - Validate that maxTxCount * HARQ_RTT < (numOfSpsHarqProc * periodicity)
 *        - If DL SPS periodicity is multiple of 8, the activation is done
 *        such that it never collides with UL retransmissions of UEs for HD
 *        FDD UE.
 *
 *  @param[in]   RgSchCellCb   *cell
 *  @param[in]   RgSchUeCb     *ue
 *  @param[in]   RgrUeCfg      *ueCfg
 *  @return  S16
 *      -# ROK
 **/
#ifdef ANSI
PRIVATE S16 rgSCHCmnSpsVldtDlUeCfg 
(
RgSchCellCb        *cell,
RgSchUeCb          *ue,
RgrUeCfg           *ueCfg
)
#else /* ANSI */
PRIVATE S16 rgSCHCmnSpsVldtDlUeCfg (cell, ue, ueCfg) 
RgSchCellCb        *cell;
RgSchUeCb          *ue;
RgrUeCfg           *ueCfg;
#endif /* ANSI */
{

   U16             prdIdx;

   TRC2(rgSCHCmnSpsVldtDlUeCfg);
   
   /* Validate that ack-nack repetition is not enabled for DL SPS UE */
   if (ueCfg->ueAckNackCfg.isAckNackEnabled)
   {
      RLOG_ARG1(L_ERROR,DBG_CELLID,cell->cellId,
               "Ack-nack repetition not supported DL SPS enabled CRNTI:%d", 
               (int)ue->ueId);
      RETVALUE(RFAILED);
   }

   RG_SCH_CMN_SPS_GET_PRD_IDX ((int)ueCfg->ueSpsCfg.dlSpsCfg.dlSpsPrdctyEnum, 
      &prdIdx);

   /* Note: prdIdx shall not be invalid since all possible periodicity
    * configurations have been taken care of */
   if (prdIdx == RG_SCH_CMN_SPS_PRD_INVALID)
   {
      RLOG_ARG2(L_ERROR,DBG_CELLID,cell->cellId,
               "Invalid SPS periodicity for CRNTI:%d, periodicity %d", 
               (int)ue->ueId, (int)ueCfg->ueSpsCfg.dlSpsCfg.dlSpsPrdctyEnum);
      RETVALUE(RFAILED);
   }
   
#ifdef LTE_TDD
   /* Only supporting Periodicity values that are multiple of 10 */
   if (ueCfg->ueSpsCfg.dlSpsCfg.dlSpsPrdctyEnum % 10)
   {
      RLOG_ARG1(L_ERROR,DBG_CELLID,cell->cellId,
         "rgSCHCmnSpsVldtDlUeCfg: Invalid Periodicity CRNTI:%d",ue->ueId);
      return RFAILED;
   }
#endif

   RETVALUE(ROK);

}

/**
 * @brief  UE reconfiguration for DL SPS
 *
 * @details
 *
 *     Function : rgSCHCmnSpsDlUeRecfg
 *
 *
 *     Processing Steps:
 *        -  If (ue->dlSpsEnabled && ueRecfg->ueSpsRecfg.dlSpsCfg.dlSpsEnabled)
 *           - If both the configurations do not match, return RFAILED
 *           - if (ue->isSpsActv) and collision of the offset is 
 *              detected with new configuration,
 *              - Set (ue->actionPndng |= RG_SCH_CMN_SPS_DL_REACTV_TIME)
 *        - If (ue->dlSpsEnabled && !ueRecfg->ueSpsRecfg.dlSpsCfg.dlSpsEnabled)
 *           - if (ue->isSpsActv) 
 *             - Add UE to toBeRelUes queue for SPS Release
 *           - Remove it from the SPS Queues
 *        -  If (!ue->dlSpsEnabled && ueRecfg->ueSpsRecfg.dlSpsCfg.dlSpsEnabled)
 *           - Perform validations. Call rgSCHCmnSpsVldtDlUeRecfg()
 *           - Update cell-wide DL SF data structure based on the configured
 *             periodicity of DL SPS. Call rgSCHCmnSpsDlUpdSf.
 *           - Perform initialization of UE specifc information for 
 *             DL SPS enabled UE. Call rgSCHCmnSpsDlUeInit.
 *           - Compute Y table for SPS-RNTI. Call rgSCHUtlUpdY().
 *           - Return ROK. 
 *     - Return ROK. 
 *
 *  @param[in]   RgSchCellCb   *cell
 *  @param[in]   RgSchUeCb     *ue
 *  @param[in]   RgrUeRecfg    *ueRecfg
 *   @param[out] RgSchErrInfo  *err
 *  @return  S16
 *      -# ROK
 **/
#ifdef ANSI
PRIVATE S16 rgSCHCmnSpsDlUeRecfg 
(
RgSchCellCb        *cell,
RgSchUeCb          *ue,
RgrUeRecfg         *ueRecfg,
RgSchErrInfo       *err
)
#else /* ANSI */
PRIVATE S16 rgSCHCmnSpsDlUeRecfg (cell, ue, ueRecfg, err) 
RgSchCellCb        *cell;
RgSchUeCb          *ue;
RgrUeRecfg         *ueRecfg;
RgSchErrInfo       *err;
#endif /* ANSI */
{

   RgrUeSpsDlCfg         *dlSpsCfg = &ueRecfg->ueSpsRecfg.dlSpsCfg;
   RgrUeSpsDlCfg         *ueSpsCfg = &ue->dl.dlSpsCfg;
   RgSchCmnDlUeSpsInfo   *dlSpsUe = RG_SCH_CMN_SPS_GET_DL_UE(ue);
   RgSchCmnDlCellSpsInfo *dlSpsCell = RG_SCH_CMN_SPS_GET_DL_CELL(cell);
   Bool                  isDlSpsEnabled;
   Bool                  isSpsRecfg = FALSE;

   TRC2(rgSCHCmnSpsDlUeRecfg);
   /* Validate reconfigured values if either DL SPS is getting enabled or is
    * already enabled */
   if (ueRecfg->ueRecfgTypes & RGR_UE_DLSPS_RECFG)
   {
      isDlSpsEnabled = dlSpsCfg->isDlSpsEnabled;
      isSpsRecfg     = TRUE;
   }
   else
   {
      isDlSpsEnabled = ueSpsCfg->isDlSpsEnabled;
   }

   if (isDlSpsEnabled)
   {
      /* Validate the re-configured values of measGap and DRX */
      if (ROK != rgSCHCmnSpsVldtDlUeRecfg(cell, ue, ueRecfg))
      {
         RLOG_ARG2(L_ERROR,DBG_CELLID,cell->cellId,
                  "DL SPS sub-frame update failed %d CRNTI:%d", 
                  (int)dlSpsCfg->dlSpsPrdctyEnum,ue->ueId);
         RETVALUE(RFAILED);
      }

      /* Update measurement Gap mask of the UE if measurement gap is
       * re-configured, if UE was previously SPS enabled */
      if (ueSpsCfg->isDlSpsEnabled)
      {
         if (ueRecfg->ueRecfgTypes & RGR_UE_MEASGAP_RECFG)
         {
            /* Note: Latest Measurement gap configuration being read from the 
             * ueRecfg rather than from ueCb */
            rgSCHCmnSpsDlInitMeasGapMask(cell, &ueRecfg->ueMeasGapRecfg,
                  dlSpsUe);
         } 

         if ((dlSpsUe->dlSpsStatus == RGSCH_DL_SPS_ACTV) ||
               (dlSpsUe->dlSpsStatus == RGSCH_DL_SPS_WAITING_FOR_ACTV_CFM))
         {
            /* Check collision with existing activation */
            if (rgSCHCmnSpsDlUeCollidesInTime(cell, ue, 
                     dlSpsUe->dlSpsUeSchdInfo.schdKey,
                     &ueRecfg->ueMeasGapRecfg))
            {
               /* Existing activation collides, mark UE to be reacitvated */
               /* Add to ToBeActvated list only when BO is available for SPS Svc*/
               rgSCHCmnSpsDlUeClearN1PucchAlloc(cell, ue, FALSE);
               rgSCHCmnSpsDlUeClearAlloc(cell, ue);
               RGSCH_SPS_SET_CLEAR_ALLC_HIST(ue);
               rgSCHCmnSpsUtlDlAdd2ToBeRelUes(dlSpsCell,ue);      
               RGSCH_SPS_SET_DL_SPS_STATE(ue, RGSCH_DL_SPS_RELEASE_TRIGGERED);
               if (dlSpsUe->spsSvc->bo)
               {
                  rgSCHCmnSpsUtlDlAdd2ToBeSchdSvcs(dlSpsCell, dlSpsUe->spsSvc);
               }
            }
         }
      }
   }

   if (isSpsRecfg)
   {
      if (dlSpsCfg->isDlSpsEnabled && ueSpsCfg->isDlSpsEnabled)
      {
         /* Note: Not handling any re-configured values here */
         /* Currently re-configuration of values is not supported for SPS enabled
          * UE */
         RLOG_ARG1(L_ERROR,DBG_CELLID,cell->cellId,
                  "DL SPS values' reconfiguration for UE not supported:"
                  " CRNTI:%d", (int)ue->ueId);

         /* Could think of re-activation here - design needs to be thought
          * through, if done then UE shall never have temporary disabling of the
          * SPS service */
      }
      else if (!dlSpsCfg->isDlSpsEnabled && ueSpsCfg->isDlSpsEnabled)
      {
         /* DL SPS getting disabled */
         rgSCHCmnSpsDlUeSuspend(cell, ue, TRUE);
         cmMemset((U8 *)dlSpsUe, 0, sizeof(*dlSpsUe));
         ue->dl.dlSpsCfg = *dlSpsCfg;

         /* Note: Not sending explicit release for this  UE since SPS is getting
          * disabled for the UE, it shall release implicitly [36.321, Sec 5.10] */
         /* Note: If temporarily getting disabled, then it shall be re-configured
          * by RRC for enabling SPS and UE shall expect data for very short period
          * of time for SPS ocassions which could be ignored */
      }
      else if (dlSpsCfg->isDlSpsEnabled && !ueSpsCfg->isDlSpsEnabled)
      {
         /* DL SPS enabled right now */

         /* Update the subframe information based on the configured periodicity value */
         if (ROK != rgSCHCmnSpsDlUpdSf(cell, dlSpsCfg->dlSpsPrdctyEnum))
         {
            RLOG_ARG2(L_ERROR,DBG_CELLID,cell->cellId, 
                     "DL SPS sub-frame update failed %d CRNTI:%d", 
                     (int)dlSpsCfg->dlSpsPrdctyEnum,ue->ueId);
            RETVALUE(RFAILED);
         }

         /* Initialize SPS related information in UeCb */
         if (ROK != rgSCHCmnSpsDlUeInit(cell, ue, &ueRecfg->ueSpsRecfg, 
                     &ueRecfg->ueMeasGapRecfg))
         {
            RLOG_ARG1(L_ERROR,DBG_CELLID,cell->cellId,
                     "Ue Initialization failed: n1Pucch not available CRNTI:%d",ue->ueId);
            RETVALUE(RFAILED);
         }
      }
   }

   RETVALUE(ROK);
}

/**
 * @brief  UE configuration for DL SPS enabled UE.
 *
 * @details
 *
 *     Function : rgSCHCmnSpsDlUeCfg
 *
 *
 *     Processing Steps:
 *     - Update cell-wide DL SF data structure based on the configured
      periodicity of DL SPS. Call rgSCHCmnSpsDlUpdSf.
 *     - Perform initialization of UE specifc information for DL SPS enabled UE.
 *     Call rgSCHCmnSpsDlUeInit.
 *     - Compute Y table for SPS-RNTI. Call rgSCHUtlUpdY().
 *     - Return ROK. 
 *
 *  @param[in]   RgSchCellCb   *cell
 *  @param[in]   RgSchUeCb     *ue
 *  @param[in]   RgrUeCfg      *ueCfg
 *   @param[out] RgSchErrInfo  *err
 *  @return  S16
 *      -# ROK
 **/
#ifdef ANSI
PRIVATE S16 rgSCHCmnSpsDlUeCfg 
(
RgSchCellCb        *cell,
RgSchUeCb          *ue,
RgrUeCfg           *ueCfg,
RgSchErrInfo       *err
)
#else /* ANSI */
PRIVATE S16 rgSCHCmnSpsDlUeCfg (cell, ue, ueCfg, err) 
RgSchCellCb        *cell;
RgSchUeCb          *ue;
RgrUeCfg           *ueCfg;
RgSchErrInfo       *err;
#endif /* ANSI */
{

   RgrUeSpsDlCfg   *dlSpsCfg = &ueCfg->ueSpsCfg.dlSpsCfg;

   TRC2(rgSCHCmnSpsDlUeCfg);
   /* Update the subframe information based on the configured periodicity value */
   if (ROK != rgSCHCmnSpsDlUpdSf(cell, dlSpsCfg->dlSpsPrdctyEnum))
   {
      RLOG_ARG2(L_ERROR,DBG_CELLID,cell->cellId,
         "DL SPS sub-frame update failed %d CRNTI:%d", 
               (int)dlSpsCfg->dlSpsPrdctyEnum,ueCfg->crnti);
      RETVALUE(RFAILED);
   }

   /* Initialize SPS related information in UeCb */
  if (ROK != rgSCHCmnSpsDlUeInit(cell, ue, &ueCfg->ueSpsCfg,
              &ueCfg->ueMesGapCfg))
  {
      RLOG_ARG1(L_ERROR,DBG_CELLID,cell->cellId, 
              "Ue Initialization failed: n1Pucch not available CRNTI:%d",
              ueCfg->crnti);
      RETVALUE(RFAILED);
  }

   RETVALUE(ROK);

}

/**
 * @brief Validates the UE re-configuration for SPS UE.
 *
 * @details
 *
 *     Function : rgSCHCmnSpsVldtDlUeRecfg
 *
 *
 *     Processing Steps:
 *      - Validate the re-configuration with respect to SPS. 
 *
 *  @param[in]   RgSchCellCb   *cell
 *  @param[in]   RgSchUeCb     *ue
 *  @param[in]   RgrUeRecfg    *ueRecfg
 *  @return  S16
 *      -# ROK
 **/
#ifdef ANSI
PRIVATE S16 rgSCHCmnSpsVldtDlUeRecfg 
(
RgSchCellCb        *cell,
RgSchUeCb          *ue,
RgrUeRecfg         *ueRecfg
)
#else /* ANSI */
PRIVATE S16 rgSCHCmnSpsVldtDlUeRecfg (cell, ue, ueRecfg) 
RgSchCellCb        *cell;
RgSchUeCb          *ue;
RgrUeRecfg         *ueRecfg;
#endif /* ANSI */
{

   Bool                isAckNackEnabled;
#ifdef RG_UNUSED
   /* Enable this if DRX implementation is completed */
   Bool                isDrxEnabled;
   U16                 longDrxCycle;
   U16                 dlSpsPrd = 0;
#endif
   U16                  prdIdx;
   TRC2(rgSCHCmnSpsVldtDlUeRecfg);
   /* Check if ack-nack repetition is reconfigured and use the re-configured
    * value */
   if (ueRecfg->ueRecfgTypes & RGR_UE_ACKNACK_RECFG)
   {
      isAckNackEnabled = ueRecfg->ueAckNackRecfg.isAckNackEnabled;  
   }
   else
   {
      isAckNackEnabled = ue->ackNakRepCb.isAckNackEnabled;
   }
   /* Validate that ack-nack repetition is not enabled for DL SPS UE */
   if (isAckNackEnabled)
   {
      RLOG_ARG1(L_ERROR,DBG_CELLID,cell->cellId,
               "Ack-nack repetition not supported DL SPS enabled CRNTI:%d", 
               (int)ue->ueId);
      RETVALUE(RFAILED);
   }

   if (ueRecfg->ueRecfgTypes & RGR_UE_DLSPS_RECFG)
   {
      RG_SCH_CMN_SPS_GET_PRD_IDX ((int)ueRecfg->ueSpsRecfg.dlSpsCfg.dlSpsPrdctyEnum,
            &prdIdx);

      if (prdIdx == RG_SCH_CMN_SPS_PRD_INVALID)
      {
         RLOG_ARG2(L_ERROR,DBG_CELLID,cell->cellId,
                  "Invalid SPS periodicity for CRNTI:%d, periodicity %d", 
                  (int)ue->ueId, (int)ueRecfg->ueSpsRecfg.dlSpsCfg.dlSpsPrdctyEnum);
         RETVALUE(RFAILED);
      }
#ifdef LTE_TDD
   /* Only supporting Periodicity values that are multiple of 10 */
   if (ueRecfg->ueSpsRecfg.dlSpsCfg.dlSpsPrdctyEnum % 10)
   {
      RLOG_ARG1(L_ERROR,DBG_CELLID,cell->cellId,
         "rgSCHCmnSpsVldtDlUeRecfg: Invalid Periodicity CRNTI:%d",ue->ueId);
      return RFAILED;
   }
#endif
   }
#ifdef RG_UNUSED
   /* Enable this if DRX implementation is completed */
   /* Get SPS periodicity for the new configuration */
   if (ueRecfg->recfgTypes & RGR_UE_DLSPS_RECFG)
   {
      if (ueRecfg->ueSpsRecfg.dlSpsCfg.isDlSpsEnabled)
      {
         dlSpsPrd = ueRecfg->ueSpsRecfg.dlSpsCfg.dlSpsPrdctyEnum;
      }
   }
   else
   {
      if (ue->dl.dlSpsCfg.isDlSpsEnabled)
      { 
         dlSpsPrd = ue->dl.dlSpsCfg.dlSpsPrdctyEnum;
      }
   }
   
   /* Get DRX values post re-configuration */
   if (ueRecfg->recfgTypes & RGR_UE_DRX_RECFG)
   {
      isDrxEnabled = ueRecfg->ueDrxRecfg.isDrxEnabled;  
      longDrxCycle  = ueRecfg->ueDrxRecfg.drxLongCycleOffst.longDrxCycle;
   }
   else
   {
      isDrxEnabled = ue->drxCb->isDrxEnabled;
      longDrxCycle = ue->drxCb->longDrxCycle;
   }

   /* Validate if DRX is enabled for the UE, SPS periodicity should be multiple
    * of long DRX cycle */
   if ((isDrxEnabled) && ((longDrxCycle > dlSpsPrd) ||
            (dlSpsPrd % longDrxCycle)))
   {

      RLOG_ARG3(L_ERROR,DBG_CELLID,cell->cellId,
               "Invalid configuration for DRX long cycle/DL SPS periodicity"
               "CRNTI: %d, DRX long cycle: %d, DL SPS Prd: %d", 
               (int)ue->ueId, (int)longDrxCycle, (int)dlSpsPrd);
      RETVALUE(RFAILED);
   }
#endif /*ifdef RG_UNUSED */
   
   RETVALUE(ROK);
}


/**
 * @brief Handler to free DL SPS Lists
 *
 * @details
 *
 *     Function : rgSCHCmnSpsUlFreeDlSpsLsts
 *        
 *     Processing Steps:
 *       Delete  all the UEs from all Lists for DL SPS
 *       return ROK
 *
 *  @param[in]  RgSchCellCb      *cell
 *  @return  S16
 **/

#ifdef ANSI
PRIVATE S16 rgSCHCmnSpsUlFreeDlSpsLsts
(
RgSchCellCb           *cell
)
#else /* ANSI */
PRIVATE S16 rgSCHCmnSpsUlFreeDlSpsLsts(cell)
RgSchCellCb           *cell;
#endif /* ANSI */
{
   RgSchCmnDlCellSpsInfo *dlSpsCell = RG_SCH_CMN_SPS_GET_DL_CELL(cell);
   CmLList               *ueLnk;
   U16                    idx = 0;

   TRC2 (rgSCHCmnSpsUlFreeDlSpsLsts);
   
   while ((ueLnk = cmLListFirst(&dlSpsCell->zeroBOSvcUes)) != NULLP)
   {
      cmLListDelFrm(&dlSpsCell->zeroBOSvcUes, ueLnk);
   }
   while ((ueLnk = cmLListFirst(&dlSpsCell->toBeSchdSvcs)) != NULLP)
   {
      cmLListDelFrm(&dlSpsCell->toBeSchdSvcs, ueLnk);
   }
   while ((ueLnk = cmLListFirst(&dlSpsCell->retxHqProcs)) != NULLP)
   {
      cmLListDelFrm(&dlSpsCell->retxHqProcs, ueLnk);
   }
   while ((ueLnk = cmLListFirst(&dlSpsCell->toBeActvtdUes)) != NULLP)
   {
      cmLListDelFrm(&dlSpsCell->toBeActvtdUes, ueLnk);
   }
   while ((ueLnk = cmLListFirst(&dlSpsCell->toBeRelUes)) != NULLP)
   {
      cmLListDelFrm(&dlSpsCell->toBeRelUes, ueLnk);
   }

   for (idx = 0; idx < RG_SCH_CMN_SPS_MAX_PRD; idx++)
   {
      while ((ueLnk = cmLListFirst(&dlSpsCell->actvDlSpsUeLsts[idx]))
         != NULLP)
      {
         cmLListDelFrm(&dlSpsCell->actvDlSpsUeLsts[idx], ueLnk);
      }
   }
   for (idx = 0; idx < RGSCH_NUM_SUB_FRAMES; idx++)
   {
      while ((ueLnk = cmLListFirst(&dlSpsCell->wtngForRelFdbkUeLst[idx]))
         != NULLP)
      {
         cmLListDelFrm(&dlSpsCell->wtngForRelFdbkUeLst[idx], ueLnk);
      }
   }
   RETVALUE(ROK);
} /* end of rgSCHCmnSpsUlFreeDlSpsLsts */

/***********************************************************
 *
 *     Func : rgSCHCmnSpsDlFillRelPdcch
 *
 *
 *     Desc : Fills up the release PDCCH for SPS UE.
 *
 *     Ret  : Void
 *            
 *
 *     Notes:
 *
 *     File :
 *
 **********************************************************/
#ifdef ANSI
PRIVATE Void rgSCHCmnSpsDlFillRelPdcch
(
RgSchUeCb       *ue,
RgSchPdcch      *relPdcch
)
#else /* ANSI */
PRIVATE  Void rgSCHCmnSpsDlFillRelPdcch(ue, relPdcch) 
RgSchUeCb       *ue;
RgSchPdcch      *relPdcch;
#endif /* ANSI */
{
   TfuDciFormat1aInfo *dci1aInfo = NULLP;
#ifdef LTE_TDD
   RgSchTddANInfo         *anInfo;
#endif
   TRC2(rgSCHCmnSpsDlFillRelPdcch);
   
   relPdcch->rnti = ue->spsRnti; 
   relPdcch->dci.dciFormat = TFU_DCI_FORMAT_1A; 

   dci1aInfo = &relPdcch->dci.u.format1aInfo;
   dci1aInfo->isPdcchOrder = FALSE;

   /* Release PDCCH is sent using DCI format 1A */
   dci1aInfo->t.pdschInfo.allocInfo.mcs = 0x1F;
   dci1aInfo->t.pdschInfo.allocInfo.harqProcId.pres = TRUE;
   dci1aInfo->t.pdschInfo.allocInfo.harqProcId.val = 0;
   dci1aInfo->t.pdschInfo.allocInfo.ndi = 0;
   dci1aInfo->t.pdschInfo.allocInfo.rv = 0;

#ifdef LTE_TDD
   anInfo = rgSCHUtlGetUeANFdbkInfo(ue, 
                     &ue->relPdcchFbkTiming);
   if(anInfo)
   {
      dci1aInfo->t.pdschInfo.dai.pres= TRUE;
      dci1aInfo->t.pdschInfo.dai.val= anInfo->dlDai;
   }
   else
   {
      dci1aInfo->t.pdschInfo.dai.pres= TRUE;
      dci1aInfo->t.pdschInfo.dai.val= RG_SCH_MAX_DAI_IDX;
   }
#endif

   /* Setting allocation information to all 1's */
   dci1aInfo->t.pdschInfo.allocInfo.alloc.type = TFU_ALLOC_TYPE_RIV;
   dci1aInfo->t.pdschInfo.allocInfo.alloc.u.riv = 0xFFFFFFFF; 

   RETVOID;
}

/***********************************************************
 *
 *     Func : rgSCHCmnSpsDlFillActvPdcch
 *
 *
 *     Desc :Fills up the activation/re-activation PDCCH for SPS UE.
 *
 *     Ret  : Void
 *            
 *
 *     Notes:
 *
 *     File :
 *
 **********************************************************/
#ifdef ANSI
PRIVATE Void rgSCHCmnSpsDlFillActvPdcch
(
RgSchCellCb                *cell,
RgSchUeCb                  *ue,
RgSchPdcch                 *pdcch,
Bool                       isSpsOcc,
U8                         hqProcId
)
#else /* ANSI */
PRIVATE  Void rgSCHCmnSpsDlFillActvPdcch(cell, ue, pdcch, isSpsOcc, hqProcId)
RgSchCellCb                *cell;
RgSchUeCb                  *ue;
RgSchPdcch                 *pdcch;
Bool                       isSpsOcc;
U8                         hqProcId;
#endif /* ANSI */
{
   RgSchCmnDlUeSpsInfo *dlSpsUe = RG_SCH_CMN_SPS_GET_DL_UE(ue);
   RgSchDlRbAlloc      *rbAllocInfo = &ue->dl.dlAllocCb;
   TRC2(rgSCHCmnSpsDlFillActvPdcch);

   pdcch->dci.dciFormat              = rbAllocInfo->dciFormat;
   pdcch->rnti = ue->spsRnti;
   pdcch->isSpsRnti = TRUE;
   pdcch->crnti = ue->ueId;

   /* Update subframe and pdcch info in HqTb control block */
   switch(rbAllocInfo->dciFormat)
   {
      case TFU_DCI_FORMAT_1:
         {
            rgSCHCmnSpsDlFillActvPdcchDciFrmt1(cell,dlSpsUe,rbAllocInfo,\
                    pdcch,isSpsOcc,hqProcId);
            break;
         }
      case TFU_DCI_FORMAT_1A:
         {
            rgSCHCmnSpsDlFillActvPdcchDciFrmt1A(cell,dlSpsUe,rbAllocInfo,\
                    pdcch,isSpsOcc,hqProcId);
            break;
         }
      case TFU_DCI_FORMAT_2:
         {
            rgSCHCmnSpsDlFillActvPdcchDciFrmt2(cell,dlSpsUe,rbAllocInfo,\
                    pdcch,isSpsOcc,hqProcId);
            break;
         }
      case TFU_DCI_FORMAT_2A:
         {
            rgSCHCmnSpsDlFillActvPdcchDciFrmt2A(cell,dlSpsUe,rbAllocInfo,\
                    pdcch,isSpsOcc,hqProcId);
            break;
         }
      default:
         {
            RLOG_ARG1(L_ERROR,DBG_CELLID,cell->cellId,"Allocator's"
                     " incorrect dciForamt Fill CRNTI:%d",ue->ueId);
            break;
         }
   }
   RETVOID;
}

/***********************************************************
 *
 *     Func : rgSCHCmnSpsCompLcmTbl
 *
 *
 *     Desc :Computes the LCM table for the feasible set of periodicity values
 *
 *     Ret  : Void
 *            
 *
 *     Notes:
 *
 *     File :
 *
 **********************************************************/
#ifdef ANSI
PRIVATE Void rgSCHCmnSpsCompLcmTbl
(
)
#else /* ANSI */
PRIVATE  Void rgSCHCmnSpsCompLcmTbl() 
#endif /* ANSI */
{
   U8 idx1, idx2;

   TRC2(rgSCHCmnSpsCompLcmTbl);
   /* Memset the memory */
   cmMemset((Data *)rgSchCmnSpsLcmTbl, 0, 
         RG_SCH_CMN_SPS_MAX_NUM_PRD * RG_SCH_CMN_SPS_MAX_NUM_PRD * 
         sizeof(U16));

   /* Compute LCM values */
   for (idx1 = 0; idx1 <= RG_SCH_CMN_SPS_MAX_NUM_PRD - 1; ++idx1)
   {
      for (idx2 = 0; idx2 <= RG_SCH_CMN_SPS_MAX_NUM_PRD - 1; ++idx2)
      {
         U16 prd1 = rgSchCmnSpsPrdVal[idx1];
         U16 prd2 = rgSchCmnSpsPrdVal[idx2];
         rgSchCmnSpsLcmTbl[idx1][idx2] = rgSCHCmnSpsCompLcm(prd1, prd2);
      }
   }

   RETVOID;
}

/***********************************************************
 *
 *     Func : rgSCHCmnSpsCompLcm
 *
 *
 *     Desc :Computes the LCM of 2 given periodicity values
 *           Assumption: Both the periodicities can be represented as 
 *           2^a * 3^b * 5^c
 *
 *     Ret  : U16
 *            
 *
 *     Notes:
 *
 *     File :
 *
 **********************************************************/
#ifdef ANSI
PRIVATE U16 rgSCHCmnSpsCompLcm
(
 U16      prd1,
 U16      prd2
)
#else /* ANSI */
PRIVATE  U16 rgSCHCmnSpsCompLcm(prd1, prd2) 
U16       prd1;
U16       prd2;
#endif /* ANSI */
{
   U8     cntFactors = 0;
   U16    tmpN1      = prd1;
   U16    tmpN2      = prd2;
   U16    lcm        = 1;
   U8     num        = 2;
   U8     incr       = 0;

   TRC2(rgSCHCmnSpsCompLcm);
   /* Return same value if periodicities are equal */
   if (prd1 == prd2)
   {
      RETVALUE(prd1);
   }

   /* If periodicities are multiple of each other, return the greater value */
   if ((prd1 > prd2) && !(prd1 % prd2))
   {
      RETVALUE(prd1);
   }
   else if ((prd2 > prd1) && !(prd2 % prd1))
   {
      RETVALUE(prd2);
   }

   /* Compute LCM by computing factors for 2, 3 and 5 */
   do
   {
      while (!(tmpN1 % num) && !(tmpN2 % num))
      {
         tmpN1 = tmpN1/num;
         tmpN2 = tmpN2/num;
         ++cntFactors;
      }
      while (!(tmpN1 % num))
      {
         tmpN1 = tmpN1/num;
         ++cntFactors;
      }
      while (!(tmpN2 % num))
      {
         tmpN2 = tmpN2/num;
         ++cntFactors;
      }

      lcm *= (U32)cmPow((F64)num, (F64)cntFactors);
      cntFactors = 0;
      ++incr;
      num = num + incr;
   } while (num <= 5);
   
   RETVALUE(lcm);
}

/**
 * @brief Initializes the DL SPS specific data structures for the cell.
 *
 * @details
 *     Function : rgSCHCmnSpsDlCellCfg
 *
 *       
 *     Processing Steps:
 *     - Initialize the SPS lists maintained at the cell level.
 *     - Initialize the subframe information for the cell. Call 
 *       rgSCHCmnSpsDlSfInit 
 *     - Initialize the cell-wide n1Pucch data base. Call 
 *       rgSCHCmnSpsDlN1PucchInit
 *
 *   @param[in]  RgSchCellCb   *cell
 *   @param[in]  RgrCellCfg    *cellCfg
 *   @param[out] RgSchErrInfo  *err
 *  @return  S16
 *      -# ROK
 *      -# RFAILED
 **/
#ifdef ANSI
PRIVATE S16 rgSCHCmnSpsDlCellCfg
(
RgSchCellCb        *cell,
RgrCellCfg         *cellCfg,
RgSchErrInfo       *err
)
#else /* ANSI */
PRIVATE S16 rgSCHCmnSpsDlCellCfg (cell, cellCfg, err) 
RgSchCellCb        *cell;
RgrCellCfg         *cellCfg;
RgSchErrInfo       *err;
#endif /* ANSI */
{
   RgSchCmnDlCellSpsInfo *dlSpsCell = RG_SCH_CMN_SPS_GET_DL_CELL(cell);
   U16 idx;


   TRC2(rgSCHCmnSpsDlCellCfg);
   
   /* Initialize the SPS lists maintained at the cell level: Assumed the cell
    * shall be memset to zero by the caller */

   /* Initialize the cell */
   rgSCHCmnSpsDlCellInit(cell, cellCfg);

   /* Initialize subframe related information per cell for DL SPS */
   if (ROK != rgSCHCmnSpsDlSfInit(cell, cellCfg))
   {
        RLOG_ARG0(L_ERROR,DBG_CELLID,cell->cellId,
                 "Memory allocation FAILED for SPS sub-frame table");
        RETVALUE(RFAILED);
   }
   
   cell->spsCellCfg = cellCfg->spsCfg;

   
   /* Initialize n1Pucch database per cell for DL SPS */
   rgSCHCmnSpsDlN1PucchInit(cell, cellCfg);

   for(idx=0; idx < RG_SCH_CMN_SPS_MAX_PRD; idx++)
   {
      cmLListInit (&(dlSpsCell->actvDlSpsUeLsts[idx]));
   }
   for(idx=0; idx < RGSCH_NUM_SUB_FRAMES; idx++)
   {
      cmLListInit (&(dlSpsCell->wtngForRelFdbkUeLst[idx]));
   }
   cmLListInit (&dlSpsCell->zeroBOSvcUes);
   cmLListInit (&dlSpsCell->toBeSchdSvcs);
   cmLListInit (&dlSpsCell->retxHqProcs);
   cmLListInit (&dlSpsCell->toBeActvtdUes);
   cmLListInit (&dlSpsCell->toBeRelUes);
   
   RETVALUE(ROK);
}

/**
 * @brief Initializes DL SPS specific information for the cell.
 *
 * @details
 *     Function : rgSCHCmnSpsDlCellInit
 *
 *       
 *     Processing Steps:
 *     - Compute rbgSubset sizes for SPS 
 *     - Update the configured values in CellCb
 *
 *   @param[in]  RgSchCellCb   *cell
 *   @param[in]  RgrCellCfg    *cellCfg
 *  @return  Void
 **/
#ifdef ANSI
PRIVATE Void rgSCHCmnSpsDlCellInit
(
RgSchCellCb        *cell,
RgrCellCfg         *cellCfg
)
#else /* ANSI */
PRIVATE Void rgSCHCmnSpsDlCellInit (cell, cellCfg) 
RgSchCellCb        *cell;
RgrCellCfg         *cellCfg;
#endif /* ANSI */
{
   U8              maxRaType1SpsBw = 0;

   TRC2(rgSCHCmnSpsDlCellInit);
   /* Note: Assumed that the bwRbgInfo has been updated in the cell before call to
    * this API for configured total DL bandwidth */
   /* Note: Also assumed that SPS bandwidth starts from lowest frequency i.e.
    * RB0 onwards */
   /* Note: Assuming that cell->rbgSize shall be set correctly by common
    * module */
   /* Setting maxSpsDlBw to a value such that offset=1 is not needed for SPS BW 
    * for RA type 1 */

   if (cellCfg->spsCfg.maxSpsDlBw < cell->bwCfg.dlTotalBw)
   {
      cellCfg->spsCfg.maxSpsDlBw = 
         ((cellCfg->spsCfg.maxSpsDlBw + cell->rbgSize -1)
         /cell->rbgSize)*cell->rbgSize;
   }
#ifdef RGSCH_SPS_UNUSED
   maxRaType1SpsBw = cellCfg->spsCfg.maxSpsDlBw;
   rgSCHCmnDlSpsRstMaxSpsBw(cell->rbgSize, cell->bwCfg.dlTotalBw, 
         &maxRaType1SpsBw);
#endif
   /* Determine the RBG information for the configured SPS BW */
   rgSCHCmnDlGetRbgInfo (cell->bwCfg.dlTotalBw, cellCfg->spsCfg.maxSpsDlBw,
         maxRaType1SpsBw, cell->rbgSize, &cell->spsBwRbgInfo); 

   cell->spsCellCfg.maxSpsDlBw =  cell->spsBwRbgInfo.numRbs;

   RETVOID;
}


/**
 * @brief Initializes subframe related information for the cell.
 *
 * @details
 *     Function : rgSCHCmnSpsDlSfInit
 *
 *       
 *     Processing Steps:
 *     - Set spsPrdLcm  = default value
 *     - Initialize the subframes for DL SPS 
 *
 *   @param[in]  RgSchCellCb   *cell
 *   @param[in]  RgrCellCfg    *cellCfg
 *  @return  S16
 *      -# ROK
 *      -# RFAILED
 **/
#ifdef ANSI
PRIVATE S16 rgSCHCmnSpsDlSfInit
(
RgSchCellCb        *cell,
RgrCellCfg         *cellCfg
)
#else /* ANSI */
PRIVATE S16 rgSCHCmnSpsDlSfInit (cell, cellCfg) 
RgSchCellCb        *cell;
RgrCellCfg         *cellCfg;
#endif /* ANSI */
{
   S16                   ret        = ROK;
   RgSchCmnDlCellSpsInfo *dlSpsCell = RG_SCH_CMN_SPS_GET_DL_CELL(cell);

   TRC2(rgSCHCmnSpsDlSfInit);
   dlSpsCell->spsPrdLcmVal =  RG_SCH_SPS_DFLT_PRD;
   RG_SCH_CMN_SPS_GET_PRD_IDX (dlSpsCell->spsPrdLcmVal, &dlSpsCell->lcmIdx);
   dlSpsCell->spsSfTbl = NULLP;
  
   if (dlSpsCell->spsPrdLcmVal > 1)
   {
      /* Allocate DL SPS subframes for the LCM value */
      ret = rgSCHUtlAllocSBuf(cell->instIdx, 
           (Data**)(&dlSpsCell->spsSfTbl),
           (dlSpsCell->spsPrdLcmVal * sizeof(RgSchCmnSpsDlSf)));
      if(ret != ROK)
      {
         RLOG_ARG0(L_ERROR,DBG_CELLID,cell->cellId,
                 "Memory allocation FAILED for SPS sub-frame table");
         RETVALUE(ret);
      }
   } 

   RETVALUE(ret);
}

#ifdef RGSCH_SPS_UNUSED
/**
 * @brief Resets the maximum SPS bandwidth to ensure that offset=1 is not needed
 * for RA type1 allocations
 *
 * @details
 *     Function : rgSCHCmnDlSpsRstMaxSpsBw
 *
 *       
 *     Processing Steps:
 *     - Reset SPS BW to avoid offset = 1 allocations for RA type 1
 *
 *   @param[in]  U8            rbgSize
 *   @param[in]  U8            dlTotalBw
 *   @param[out] U8            *spsBw  
 *  @return Void
 **/
#ifdef ANSI
PRIVATE Void rgSCHCmnDlSpsRstMaxSpsBw
(
U8             rbgSize,
U8             dlTotalBw,
U8             *spsBw
)
#else /* ANSI */
PRIVATE Void rgSCHCmnDlSpsRstMaxSpsBw(rbgSize, dlTotalBw, spsBw) 
U8             rbgSize;
U8             dlTotalBw;
U8             *spsBw;
#endif /* ANSI */
{
  
   U8       maxRaType1Offset0Bw = dlTotalBw - (rbgSize * rbgSize);
   U8       maxSpsBw = ((*spsBw + rbgSize - 1)/rbgSize) * rbgSize;

   TRC2(rgSCHCmnDlSpsRstMaxSpsBw);
   
   if (maxSpsBw > dlTotalBw)
   {
      maxSpsBw = dlTotalBw;
   }
   if (maxRaType1Offset0Bw % rbgSize)
   {
      maxRaType1Offset0Bw -= maxRaType1Offset0Bw % rbgSize; 
   }
   if (maxSpsBw > maxRaType1Offset0Bw)
   {
      *spsBw = maxRaType1Offset0Bw;
   }

   RETVOID;
}
#endif /* RGSCH_SPS_UNUSED */
/**
 * @brief Initializes the DL SPS specific data structures for the cell.
 *
 * @details
 *     Function : rgSCHCmnSpsDlN1PucchInit
 *
 *       
 *     Processing Steps:
 *     - Initialize the cell-wide n1Pucch data base
 *
 *   @param[in]  RgSchCellCb   *cell
 *   @param[in]  RgrCellCfg    *cellCfg
 *  @return  Void
 **/
#ifdef ANSI
PRIVATE Void rgSCHCmnSpsDlN1PucchInit
(
RgSchCellCb        *cell,
RgrCellCfg         *cellCfg
)
#else /* ANSI */
PRIVATE Void rgSCHCmnSpsDlN1PucchInit (cell, cellCfg) 
RgSchCellCb        *cell;
RgrCellCfg         *cellCfg;
#endif /* ANSI */
{

   U16                   sfIdx;
   RgSchCmnDlCellSpsInfo *dlSpsCell = RG_SCH_CMN_SPS_GET_DL_CELL(cell);

   TRC2(rgSCHCmnSpsDlN1PucchInit);
   

   dlSpsCell->n1PucchDb.numFreeN1Pucch = RG_SCH_SPS_DL_MAX_N1PUCCH_PER_SF;
   dlSpsCell->n1PucchDb.numInUseN1Pucch = 0;
   dlSpsCell->n1PucchDb.freeN1PucchStart = dlSpsCell->n1PucchDb.n1PucchLst;
   dlSpsCell->n1PucchDb.inUseN1PucchStart = NULLP;

   for (sfIdx = 0; sfIdx <  RG_SCH_SPS_DL_MAX_N1PUCCH_PER_SF - 1; ++sfIdx)
   {
      RgSchCmnSpsDlN1Pucch *n1Pucch = &(dlSpsCell->n1PucchDb.n1PucchLst[sfIdx]);
      n1Pucch->idx  = sfIdx;
      n1Pucch->next = sfIdx + 1;
   }
   dlSpsCell->n1PucchDb.n1PucchLst[sfIdx].idx = sfIdx;

   RETVOID;
}


/***********************************************************
 *
 *     Func : rgSCHCmnSpsDlGetN1PucchCb
 *
 *
 *     Desc : Determines the n1Pucch Cb for a given n1Pucch value
 *             - It tries to fetch an already existing CB for given value 
 *               from inUse list
 *             - If not available, it fetches from free list
 *
 *     Ret  : RgSchCmnSpsDlN1Pucch control block
 *            
 *
 *     Notes:
 *
 *     File :
 *
 **********************************************************/
#ifdef ANSI
PRIVATE RgSchCmnSpsDlN1Pucch *rgSCHCmnSpsDlGetN1PucchCb
(
RgSchCmnSpsDlN1PucchDb  *n1PucchDb,
U16                     n1PucchVal
)
#else /* ANSI */
PRIVATE  RgSchCmnSpsDlN1Pucch *rgSCHCmnSpsDlGetN1PucchCb(n1PucchDb, n1PucchVal) 
RgSchCmnSpsDlN1PucchDb  *n1PucchDb;
U16                     n1PucchVal;
#endif /* ANSI */
{
   U16                  cnt;
   RgSchCmnSpsDlN1Pucch *tmpCb = n1PucchDb->inUseN1PucchStart;

   TRC2(rgSCHCmnSpsDlGetN1PucchCb);
   /* Check if n1PucchCb for the n1Pucch value already exists */
   for (cnt = 0; cnt < n1PucchDb->numInUseN1Pucch; ++cnt)
   {
      if (tmpCb->n1PucchVal == n1PucchVal)
      {
         tmpCb->numUes++;
         RETVALUE(tmpCb);
      }
      tmpCb = &n1PucchDb->n1PucchLst[tmpCb->next];
   }

   /* Fetch new n1PucchCb from the free list and update the values */
   if (!n1PucchDb->numFreeN1Pucch)
   {
      RETVALUE(NULLP);
   }

   /* Remove from free list */
   tmpCb = n1PucchDb->freeN1PucchStart;
   tmpCb->n1PucchVal = n1PucchVal;
   tmpCb->numUes++;
   n1PucchDb->freeN1PucchStart = &n1PucchDb->n1PucchLst[tmpCb->next];
   n1PucchDb->numFreeN1Pucch--;

   /* Add to the beginning of inUse list */
   if (n1PucchDb->inUseN1PucchStart)
   {
      tmpCb->next = n1PucchDb->inUseN1PucchStart->idx;
   }
   n1PucchDb->inUseN1PucchStart = tmpCb;
   n1PucchDb->numInUseN1Pucch++;

   RETVALUE(tmpCb);
}

/***********************************************************
 *
 *     Func : rgSCHCmnSpsDlFreeN1Pucch
 *
 *
 *     Desc : Frees the n1Pucch value
 *            - Decrements the count for the number of UEs using the value
 *            - If count == 0, 
 *                - Remove from inUse list and 
 *                - Add the corresponding n1PucchCb to free list
 *
 *     Ret  : Void 
 *
 *     Notes:
 *
 *     File :
 *
 **********************************************************/
#ifdef ANSI
PRIVATE Void rgSCHCmnSpsDlFreeN1Pucch
(
RgSchCmnSpsDlN1PucchDb  *n1PucchDb,
RgSchCmnSpsDlN1Pucch    *n1PucchCb
)
#else /* ANSI */
PRIVATE  Void rgSCHCmnSpsDlFreeN1Pucch(n1PucchDb, n1PucchCb) 
RgSchCmnSpsDlN1PucchDb  *n1PucchDb;
RgSchCmnSpsDlN1Pucch    *n1PucchCb;
#endif /* ANSI */
{
   TRC2(rgSCHCmnSpsDlFreeN1Pucch);
   /* Decrement number of UEs using the n1Pucch vaue */
   --n1PucchCb->numUes;

   if (!n1PucchCb->numUes)
   {
      /* Remove from beginning of inUse list */
      if (n1PucchCb == n1PucchDb->inUseN1PucchStart)
      {
         n1PucchDb->inUseN1PucchStart = \
                            &n1PucchDb->n1PucchLst[n1PucchCb->next];
      }
      else
      {
         RgSchCmnSpsDlN1Pucch *tmpCb;
         RgSchCmnSpsDlN1Pucch *tmpCb1;
         U16                  cnt = n1PucchDb->numInUseN1Pucch - 1;
         tmpCb =  n1PucchDb->inUseN1PucchStart;
         tmpCb1 = &n1PucchDb->n1PucchLst[tmpCb->next];
         while (cnt && tmpCb1 != n1PucchCb)
         {
            tmpCb = &n1PucchDb->n1PucchLst[tmpCb->next];
            tmpCb1 = &n1PucchDb->n1PucchLst[tmpCb->next];
            cnt--;
         }
         /* Note: Assuming that the n1PucchCb definitely exists in inUse
          * list: Not checkining if cnt == 0 */
         /* Control reaches here only if tmpCb == n1PucchCb */
         /* Note: Could think of removing following check */
         if (tmpCb1 == n1PucchCb)
         {
            tmpCb->next = tmpCb1->next;
         }
      }
      n1PucchDb->numInUseN1Pucch--;

      /* Add to the beginning of the free list */
      if (n1PucchDb->freeN1PucchStart)
      {
         n1PucchCb->next = n1PucchDb->freeN1PucchStart->idx;
      }
      n1PucchDb->freeN1PucchStart = n1PucchCb;
      n1PucchDb->numFreeN1Pucch++;
   }

   RETVOID;
}

/**
 * @brief Delete the SPS context for the cell at cell delete for DL SPS
 *
 * @details
 *
 *     Function : rgSCHCmnSpsDlCellDel
 *     
 *     Processing Steps:
 *      - Free the DL SPS related data structures at cell level 
 *      - Return Void
 *
 *  
 *  @param[in]  RgSchCellCb *cell
 *  @return  Void
 **/
#ifdef ANSI
PUBLIC Void rgSCHCmnSpsDlCellDel 
(
RgSchCellCb        *cell
)
#else /* ANSI */
PUBLIC Void rgSCHCmnSpsDlCellDel (cell) 
RgSchCellCb        *cell;
#endif /* ANSI */
{

   RgSchCmnDlCellSpsInfo *dlSpsCell = RG_SCH_CMN_SPS_GET_DL_CELL(cell);

   TRC2(rgSCHCmnSpsDlCellDel);

   /* Free the sub-frame information for DL SPS */
   if (dlSpsCell->spsSfTbl)
   {
      /* ccpu00117052 - MOD - Passing double pointer
      for proper NULLP assignment*/
      if (dlSpsCell->spsPrdLcmVal > 1)
      {
         rgSCHUtlFreeSBuf(cell->instIdx, 
               (Data**)(&(dlSpsCell->spsSfTbl)),
               //sizeof(*dlSpsCell->spsSfTbl));
            ((dlSpsCell->spsPrdLcmVal) * sizeof(RgSchCmnSpsDlSf)));
      }
   }
   rgSCHCmnSpsUlFreeDlSpsLsts (cell);
   
   cmMemset((U8 *)dlSpsCell, 0, sizeof(*dlSpsCell));

   RETVOID;
}

/**
 * @brief Initializes DL SPS specific information for the UE.
 *
 * @details
 *     Function : rgSCHCmnSpsDlUeInit
 *
 *       
 *     Processing Steps:
 *     - Determine the index for the configured n1Pucch value.Update the
 *     index in the UeCb
 *     - Initialize ueCb->measGapMask with the measurement gap ocassions.
 *     - Update the configured values in UeCb
 *     - Update ue->spsRnti
 *
 *   @param[in]  RgSchCellCb     *cell
 *   @param[in]  RgSchUeCb       *ue
 *   @param[in]  RgrUeSpsCfg     *ueSpsCfg
 *   @param[in]  RgrUeMeasGapCfg *ueMeasGapCfg
 *  @return  S16
 *      -# ROK
 *      -# RFAILED
 **/
#ifdef ANSI
PRIVATE S16 rgSCHCmnSpsDlUeInit
(
RgSchCellCb        *cell,
RgSchUeCb          *ue,
RgrUeSpsCfg        *ueSpsCfg,
RgrUeMeasGapCfg    *ueMeasGapCfg
)
#else /* ANSI */
PRIVATE S16 rgSCHCmnSpsDlUeInit (cell, ue, ueSpsCfg, ueMeasGapCfg) 
RgSchCellCb        *cell;
RgSchUeCb          *ue;
RgrUeSpsCfg        *ueSpsCfg;
RgrUeMeasGapCfg    *ueMeasGapCfg;
#endif /* ANSI */
{

   S16                   ret        = ROK;
   U8                    n1PucchIdx;
   RgSchCmnDlCellSpsInfo *dlSpsCell = RG_SCH_CMN_SPS_GET_DL_CELL(cell);
   RgSchCmnDlUeSpsInfo   *dlSpsUe   = RG_SCH_CMN_SPS_GET_DL_UE(ue);
   RgSchCmnSpsDlN1Pucch  *n1PucchCb = NULLP;
   RgrUeSpsDlCfg         *dlSpsUeCfg  = &ueSpsCfg->dlSpsCfg;

   TRC2(rgSCHCmnSpsDlUeInit);

   
   dlSpsUe->prevDlBoUpdTm.subframe = RGSCH_INVALID_SUBFRAME;

   /* Update the indices for n1Pucch values */
   for (n1PucchIdx = 0; n1PucchIdx < dlSpsUeCfg->numPucchVal; ++n1PucchIdx)
   {
      n1PucchCb = rgSCHCmnSpsDlGetN1PucchCb(&dlSpsCell->n1PucchDb, 
            (U16)dlSpsUeCfg->n1PucchVal[n1PucchIdx]);
      if (n1PucchCb == NULLP)
      {
         /* No free n1PucchCb is available */
         RLOG_ARG2(L_ERROR,DBG_CELLID,cell->cellId,
                  "No free n1PucchCb available: in Use n1Pucch Count: %d CRNTI:%d",
                  dlSpsCell->n1PucchDb.numInUseN1Pucch,ue->ueId);
         RETVALUE(RFAILED);
      }
      dlSpsUe->n1PucchIdx[n1PucchIdx] = n1PucchCb->idx;
   }

   /* If measurement gap is enabled for the UE, update the mask */
   /* Indices corresponding to the positions at which scheduling 
      cannot be done due to meas gap are set 1*/
   if (ueMeasGapCfg->isMesGapEnabled)
   {
      rgSCHCmnSpsDlInitMeasGapMask(cell, ueMeasGapCfg, dlSpsUe);
   }

   /* Initialize DL SPS UE pointer */
   RG_SCH_CMN_SPS_GET_PRD_IDX ((int)dlSpsUeCfg->dlSpsPrdctyEnum, &dlSpsUe->prdIdx);

   /* Update the configuration information in the ueCb */
   /* Note: Not validating the uniqueness of SPS RNTI at scheduler */
   ue->spsRnti = ueSpsCfg->spsRnti;
   ue->dl.dlSpsCfg = *dlSpsUeCfg;

#ifdef LTE_TDD
   /* In case of TDD, SPS periodicity shall be rounded off to an integer. For
    * ex: 128 is considered to be 120 */
   RGSCH_SPS_GET_PRDCTY (ue->dl.dlSpsCfg.dlSpsPrdctyEnum,
                                   ue->dl.dlSpsCfg.dlSpsPrdctyEnum);
#endif /*LTE_TDD*/

   dlSpsUe->numRelPdcchSent = 0;
   /* Initialize feedback timing with invalid value */
   ue->relPdcchFbkTiming.sfn = RGSCH_MAX_SFN + 1;

   /* Initialize Statistic info for this UE */
   dlSpsUe->statInfo.numSchedSPSRnti = 0;
   dlSpsUe->statInfo.totalSPSSchedOcc = 0;
   dlSpsUe->statInfo.numSpsActv = 0;
   dlSpsUe->statInfo.numSpsRel = 0;
   dlSpsUe->statInfo.numSpsReactv = 0;
#ifdef RGSCH_SPS_STATS 
   ue->lastSpsLcBoRptTime.subframe = 0xFF;
   ue->absTimeBo = 0;
   ue->absTimeSched = 0;
#endif
   dlSpsUe->dlSpsUeSchdInfo.scaledCqi = RG_SCH_CMN_SPS_DL_INVALIDCQI_VAL;
  
   if (NULLP  == rgSCHDbmGetSpsUeCb (cell, ue->spsRnti))
   {
      rgSCHDbmInsSpsUeCb(cell, ue);
   }

   RETVALUE(ret);
}

/**
 * @brief Clear the SPS context for the UE
 *
 * @details
 *
 *     Function : rgSCHCmnSpsDlUeSuspend
 *     
 *     Processing Steps:
 *      - Free the DL SPS related data structures at UE level 
 *      - Return Void
 *
 *  
 *  @param[in]  RgSchCellCb *cell
 *  @param[in]  RgSchUeCb   *ue
 *  @return  Void
 **/
#ifdef ANSI
PRIVATE Void rgSCHCmnSpsDlUeSuspend 
(
RgSchCellCb        *cell,
RgSchUeCb          *ue,
Bool                isSpsDisab
)
#else /* ANSI */
PRIVATE Void rgSCHCmnSpsDlUeSuspend (cell, ue) 
RgSchCellCb        *cell;
RgSchUeCb          *ue;
Bool                isSpsDisab;
#endif /* ANSI */
{

   RgSchDlSf             *dlSf;
   RgSchCmnDlCellSpsInfo *dlSpsCell = RG_SCH_CMN_SPS_GET_DL_CELL(cell);
   RgSchCmnDlUeSpsInfo   *dlSpsUe   = RG_SCH_CMN_SPS_GET_DL_UE(ue);
   RgSchCmnDlSvcSpsInfo  *dlSpsSvc;
   U8                    hqPIdx;
   RgSchDlHqProcCb       *hqP;
   CmLteTimingInfo fdbkTime;
   RgSchCmnDlHqProc *cmnHqDl;   
   Pst             pst;
   RgInfRlsHqInfo  *rlsHqBufs = &(cell->rlsHqArr[cell->crntHqIdx]); 
   RgSchDrxDlHqProcCb  *drxHq   =NULLP;

   TRC2(rgSCHCmnSpsDlUeSuspend);
   
#ifdef RGSCH_SPS_STATS
   ue->absTimeBo = 0;
   ue->absTimeSched = 0;
#endif

   rlsHqBufs->numUes = 0;
   /* Remove HARQ Proc from re-transmitting queues, if present */
   for(hqPIdx = 0; hqPIdx < ue->dl.hqEnt->numHqPrcs; hqPIdx++)
   {
      hqP = &ue->dl.hqEnt->procs[hqPIdx];
      rgSCHCmnSpsUtlDlDelFrmRetxHqProcs(dlSpsCell, hqP);

      /* Freeing the Harq Proc being used for SPS */
      cmnHqDl = RG_SCH_CMN_GET_DL_HQP(hqP);
      
      if (cmnHqDl->isSpsSvcSchd)
      {
         /* integration_fix */

         if (ue->isDrxEnabled == TRUE)
         {
            drxHq   = RG_SCH_DRX_GET_DL_HQ(hqP);

            if(drxHq->rttIndx != DRX_INVALID)
            {
               cmLListDelFrm (&(cell->drxCb->drxQ[drxHq->rttIndx].harqRTTQ),
                     &(drxHq->harqRTTEnt));

               drxHq->rttIndx = DRX_INVALID;
            }

            if(drxHq->reTxIndx != DRX_INVALID)
            {
               cmLListDelFrm (&(cell->drxCb->drxQ[drxHq->reTxIndx].harqRetxQ),
                     &(drxHq->harqRetxEnt));

               drxHq->reTxIndx = DRX_INVALID;
            }
         } 
         if ((hqP->tbInfo[0].subFrm != NULLP) &&
               (hqP->tbInfo[0].sfLnk.node != NULLP))
         {
            rgSCHUtlDlHqPTbRmvFrmTx(hqP->tbInfo[0].subFrm, hqP, 0, FALSE);

            /* Fill Release harq sent to MAC to free SPS Hq procs */
            rlsHqBufs->ueHqInfo[rlsHqBufs->numUes].status[0] = TRUE;
            rlsHqBufs->ueHqInfo[rlsHqBufs->numUes].tbId[0] = 1;
            rlsHqBufs->ueHqInfo[rlsHqBufs->numUes].numOfTBs = 1;
            rlsHqBufs->ueHqInfo[rlsHqBufs->numUes].rnti = ue->ueId;
            rlsHqBufs->ueHqInfo[rlsHqBufs->numUes].hqProcId = hqP->procId;
            rlsHqBufs->numUes++;
         }
         rgSCHDhmRlsHqpTb(hqP, 0, TRUE);
      }
   }
 
   /* Free SPS Hq procs at MAC */ 
   if(rlsHqBufs->numUes)
   {
      rgSCHUtlGetPstToLyr(&pst, &rgSchCb[cell->instIdx], cell->macInst);
      RgSchMacRlsHq (&pst, rlsHqBufs);
   } 
   ue->dl.isSpsHqPInUse = FALSE;

  
   dlSpsUe->prevDlBoUpdTm.subframe = RGSCH_INVALID_SUBFRAME;

   /* Remove from wtngForRelFdbkUeLst in case Release PDCCH is sent but
    * UE DelReq is received before feedback could arrive */
   if (dlSpsUe->isRelPdcchSent)
   {
      RG_SCH_ADD_TO_CRNT_TIME(ue->relPdcchFbkTiming,fdbkTime,TFU_HQFBKIND_ULDELTA); 
      cmLListDelFrm(&(dlSpsCell->wtngForRelFdbkUeLst[fdbkTime.subframe]),
            &(dlSpsUe->wtngForRelFdbkUeEnt));
      dlSpsUe->wtngForRelFdbkUeEnt.node = NULLP; /* SPS INTG FIX */
      dlSpsUe->isRelPdcchSent = FALSE; /* SPS INTG FIX */

      dlSf = rgSCHUtlSubFrmGet(cell, ue->relPdcchTxTime);
      if (dlSf->relPdcch)
      {
         rgSCHUtlPdcchPut(cell,&dlSf->pdcchInfo, dlSf->relPdcch);
         dlSf->relPdcch = NULLP;
      }

      ue->relPdcchTxTime.sfn    = RGSCH_MAX_SFN + 1;
      ue->relPdcchFbkTiming.sfn = RGSCH_MAX_SFN + 1;
   }

   /* Remove UE from toBeActvtdUes or toBeRelUes list */
   if (dlSpsUe->spsList)
   {
      if (dlSpsUe->spsList == &dlSpsCell->toBeActvtdUes)
      {
         rgSCHCmnSpsUtlDlDelFrmToBeActvtdUes(dlSpsCell, ue);
      }
      else
      {
         rgSCHCmnSpsUtlDlDelFrmToBeRelUes(dlSpsCell, ue);
      }
   }

   /* If UE is SPS active, clear the allocation of the UE from the cell-wide
    * subframes */
   if ((dlSpsUe->dlSpsStatus == RGSCH_DL_SPS_ACTV) ||
      (dlSpsUe->dlSpsStatus == RGSCH_DL_SPS_WAITING_FOR_ACTV_CFM))
   {
      rgSCHCmnSpsDlUeClearN1PucchAlloc(cell, ue, isSpsDisab);
      rgSCHCmnSpsDlUeClearAlloc(cell, ue);
      RGSCH_SPS_SET_CLEAR_ALLC_HIST(ue);
   }

   RGSCH_SPS_SET_DL_SPS_STATE(ue, RGSCH_DL_SPS_IDLE);

   if (dlSpsUe->spsSvc)
   {
      dlSpsSvc = RG_SCH_CMN_SPS_GET_DL_SVC(dlSpsUe->spsSvc);
      if(dlSpsSvc->toBeSchdSvcEnt.node)
      {
         rgSCHCmnSpsUtlDlDelFrmToBeSchdSvcs(dlSpsCell, dlSpsUe->spsSvc);
      }
      cmMemset((U8 *)dlSpsSvc, 0, sizeof(*dlSpsSvc));
   }

   RETVOID;
}

/**
 * @brief Update the cell-wide sub-frame information for DL SPS.
 *
 * @details
 *     Function : rgSCHCmnSpsDlUpdSf
 *
 *       
 *     Processing Steps:
 *     - Compute the new LCM value with the DL SPS periodicity value
 *     - if (!cell->spsSfTbl)
 *       -  Allocate for spsSfTbl with new_lcm size
 *     - else if (cell->spsSfTbl && new_lcm > old_lcm) 
 *       -  Re-allocate for spsSfTbl with new_lcm
 *       -  Copy the old data in newly allocated memory. Replicate it for
 *         (new_lcm/old_Lcm) times in newly allocated memory.
 *
 *
 *   @param[in]  RgSchCellCb   *cell
 *   @param[in]  U32           prdVal
 *  @return  S16
 *      -# ROK
 *      -# RFAILED
 **/
#ifdef ANSI
PRIVATE S16 rgSCHCmnSpsDlUpdSf
(
RgSchCellCb        *cell,
U32                prdVal
)
#else /* ANSI */
PRIVATE S16 rgSCHCmnSpsDlUpdSf (cell, prdVal) 
RgSchCellCb        *cell;
U32                prdVal;
#endif /* ANSI */
{
   S16                   ret        = ROK;

   U8                    prdIdx;
   U8                    lcmIdx=0;
   U16                   oldLcm     = 1;
   U16                   newLcm     = 1;
   RgSchCmnDlCellSpsInfo *dlSpsCell = RG_SCH_CMN_SPS_GET_DL_CELL(cell);
   RgSchCmnSpsDlSf       *tmpSpsSfTbl = NULLP;

   TRC2(rgSCHCmnSpsDlUpdSf);
   
   oldLcm = dlSpsCell->spsPrdLcmVal;
   lcmIdx = dlSpsCell->lcmIdx;

   /* Get index for new periodicity */
   RG_SCH_CMN_SPS_GET_PRD_IDX (prdVal, &prdIdx);

   /* Note: prdIdx shall not be invalid since all possible periodicity
    * configurations have been taken care of */
   if (lcmIdx >= RG_SCH_CMN_SPS_PRD_INVALID)
   {
      newLcm = rgSCHCmnSpsCompLcm(oldLcm, (U16)prdVal);
   }
   else
   {
      /*KW_FIX:Prd check already done in during cfg validation */
      if (prdIdx < RG_SCH_CMN_SPS_PRD_INVALID)
      {
         newLcm = rgSchCmnSpsLcmTbl[lcmIdx][prdIdx];
      }
   }

   /* Replicate the data structure information if the newLcm > oldLcm for
    * newLcm/oldLcm number of times */
   /* Note: newLcm/oldLcm is always an integer */
   if (newLcm > oldLcm)
   {
      ret = rgSCHUtlAllocSBuf(cell->instIdx, 
            (Data**)(&tmpSpsSfTbl),
            (newLcm * sizeof(RgSchCmnSpsDlSf)));
      if(ret != ROK)
      {
         RLOG_ARG1(L_ERROR,DBG_CELLID,cell->cellId,
                  "Memory allocation FAILED for SPS sub-frame table: "
                  "lcmVal: %d", (int)newLcm);
         RETVALUE(ret);
      }

      if (oldLcm > 1)
      {
         /* If old subframe info exists, replicate old sub-frame info across 
          * new subframes */
         U16   iterCntr = newLcm/oldLcm;
         U16   idx;
         for (idx = 0; idx < iterCntr; ++idx)
         {
            cmMemcpy((U8 *)(tmpSpsSfTbl + (idx * oldLcm)),
                  (U8 *)dlSpsCell->spsSfTbl, oldLcm * sizeof(RgSchCmnSpsDlSf));
         }
         /* ccpu00117052 - MOD - Passing double pointer
         for proper NULLP assignment*/
         rgSCHUtlFreeSBuf(cell->instIdx, (Data**)(&(dlSpsCell->spsSfTbl)),
               oldLcm * sizeof(RgSchCmnSpsDlSf));
      }

      /* Update the cell-wide data structure with new information */
      dlSpsCell->spsSfTbl =  tmpSpsSfTbl;
      dlSpsCell->spsPrdLcmVal =  newLcm;
      RG_SCH_CMN_SPS_GET_PRD_IDX (dlSpsCell->spsPrdLcmVal, &dlSpsCell->lcmIdx);
   }

   RETVALUE(ret);
}

/***********************************************************
 *
 *     Func : rgSCHCmnSpsDlInitMeasGapMask
 *
 *
 *     Desc : Initializes measurement gap mask of the UE based on the
 *     measurement gap configuration 
 *
 *     Ret  : U8
 *            
 *
 *     Notes:
 *
 *     File :
 *
 **********************************************************/
#ifdef ANSI
PRIVATE S16 rgSCHCmnSpsDlInitMeasGapMask
(
RgSchCellCb          *cell,
RgrUeMeasGapCfg      *measGapCfg,
RgSchCmnDlUeSpsInfo  *dlSpsUe
)
#else /* ANSI */
PRIVATE S16 rgSCHCmnSpsDlInitMeasGapMask (cell, measGapCfg, dlSpsUe)
RgSchCellCb          *cell;
RgrUeMeasGapCfg      *measGapCfg;
RgSchCmnDlUeSpsInfo  *dlSpsUe;
#endif /* ANSI */
{
   U8 idx = 0;
   S8 startOffset = measGapCfg->gapOffst;
   U8 hqFbkOffst;
   U8 maskIdx;
   U8 posInMask;
   S8 tempOff;
   
   TRC2(rgSCHCmnSpsDlInitMeasGapMask);
   
#ifndef LTE_TDD
   UNUSED(cell);
#endif /*LTE_TDD*/

   /* Init measGapMask before setting it */
   cmMemset ((U8 *)dlSpsUe->measGapMask, 0, (sizeof(U32) *
            RG_SCH_CMN_SPS_DL_MEASGAP_32BITMASK_SIZE));

   /* If measurement gap is enabled for the UE, update the mask */
   /* Indices corresponding to the positions at which scheduling 
      cannot be done are set 1*/
   if (measGapCfg->isMesGapEnabled)
   {

#ifdef LTE_TDD
      hqFbkOffst =  rgSchHqIntTbl[cell->ulDlCfgIdx];
#else /* LTE_TDD */
      hqFbkOffst = RG_SCH_CMN_HARQ_INTERVAL;
#endif /*LTE_TDD*/
      startOffset -= hqFbkOffst;
      if (startOffset < 0)
      {
         startOffset = startOffset + measGapCfg->gapPrd;
      } 
      
      for (;idx < RG_SCH_MEAS_GAP_LEN + hqFbkOffst; ++idx)
      {
         maskIdx = startOffset/32;
         posInMask = startOffset%32;
         dlSpsUe->measGapMask[maskIdx] |= 
            1 << RG_SCH_CMN_DL_GET_POS_FRM_LSB(posInMask);
         tempOff = ++startOffset % measGapCfg->gapPrd;
         startOffset = tempOff;
      }
   }

   RETVALUE(ROK);
}

/***********************************************************
 *
 *     Func : rgSCHCmnSpsDlGetScaledCqi
 *
 *
 *     Desc : Determines a scaled down CQI value for SPS transmission
 *
 *     Ret  : U8
 *
 *
 *     Notes:
 *
 *     File :
 *
 **********************************************************/
#ifdef ANSI
PRIVATE U8 rgSCHCmnSpsDlGetScaledCqi
(
RgSchUeCb             *ue,
U8                    curCqi
)
#else /* ANSI */
PRIVATE  U8 rgSCHCmnSpsDlGetScaledCqi(ue, curCqi)
RgSchUeCb             *ue;
U8                    curCqi;
#endif /* ANSI */
{
   TRC2(rgSCHCmnSpsDlGetScaledCqi);

   /* If CQI is between 1 to 5 : Scaled CQI is 1 */                                                                                                                          
   if (curCqi <= 5)
   {
      RETVALUE(1);
   }
   else if (curCqi <= 10)
   {
      /* If CQI is between 5 to 10 : Scaled CQI is 5 */
      RETVALUE(5);
   }
   else
   {
 
      /* If CQI is between 10 to 15 : Scaled CQI is 10 */
      RETVALUE(10);
   }
}
                                                                                                                             


/***************************************************************
                         DL SPS Utility Functions Start
****************************************************************/


/***********************************************************
 *
 *     Func : rgSCHCmnSpsUtlDlAdd2ZeroBOSvcUes
 *
 *
 *     Desc : Adds UE to the ZeroBOSvcUes list
 *
 *     Ret  : Void
 *            
 *
 *     Notes:
 *
 *     File :
 *
 **********************************************************/
#ifdef ANSI
PRIVATE Void rgSCHCmnSpsUtlDlAdd2ZeroBOSvcUes
(
RgSchCmnDlCellSpsInfo   *dlSpsCell,
RgSchUeCb               *ue
)
#else /* ANSI */
PRIVATE  Void rgSCHCmnSpsUtlDlAdd2ZeroBOSvcUes(dlSpsCell, ue) 
RgSchCmnDlCellSpsInfo   *dlSpsCell;
RgSchUeCb               *ue;
#endif /* ANSI */
{
   RgSchCmnDlUeSpsInfo  *dlSpsUe = RG_SCH_CMN_SPS_GET_DL_UE(ue);
   
   TRC2(rgSCHCmnSpsUtlDlAdd2ZeroBOSvcUes);
   
   if (!dlSpsUe->zeroBOSvcUesEnt.node)
   {
      dlSpsUe->zeroBOSvcUesEnt.node = (PTR)ue;
      cmLListAdd2Tail(&dlSpsCell->zeroBOSvcUes, &dlSpsUe->zeroBOSvcUesEnt);
   }
   RETVOID;
}

/***********************************************************
 *
 *     Func : rgSCHCmnSpsUtlDlDelFrmZeroBOSvcUes
 *
 *
 *     Desc : Deletes UE from ZeroBOSvcUes list
 *
 *     Ret  : Void
 *            
 *
 *     Notes:
 *
 *     File :
 *
 **********************************************************/
#ifdef ANSI
PRIVATE Void rgSCHCmnSpsUtlDlDelFrmZeroBOSvcUes
(
RgSchCmnDlCellSpsInfo   *dlSpsCell,
RgSchUeCb               *ue
)
#else /* ANSI */
PRIVATE  Void rgSCHCmnSpsUtlDlDelFrmZeroBOSvcUes(dlSpsCell, ue) 
RgSchCmnDlCellSpsInfo   *dlSpsCell;
RgSchUeCb               *ue;
#endif /* ANSI */
{
   RgSchCmnDlUeSpsInfo  *dlSpsUe = RG_SCH_CMN_SPS_GET_DL_UE(ue);
   
   TRC2(rgSCHCmnSpsUtlDlDelFrmZeroBOSvcUes);
   
   if (dlSpsUe->zeroBOSvcUesEnt.node)
   {
      cmLListDelFrm(&dlSpsCell->zeroBOSvcUes, &dlSpsUe->zeroBOSvcUesEnt);
      dlSpsUe->zeroBOSvcUesEnt.node = NULLP;
   }
   RETVOID;
}


/***********************************************************
 *
 *     Func : rgSCHCmnSpsUtlDlAdd2ToBeActvtdUes
 *
 *
 *     Desc : Adds UE to the toBeActvtdUes list
 *
 *     Ret  : Void
 *            
 *
 *     Notes:
 *
 *     File :
 *
 **********************************************************/
#ifdef ANSI
PRIVATE Void rgSCHCmnSpsUtlDlAdd2ToBeActvtdUes
(
RgSchCmnDlCellSpsInfo   *dlSpsCell,
RgSchUeCb               *ue
)
#else /* ANSI */
PRIVATE  Void rgSCHCmnSpsUtlDlAdd2ToBeActvtdUes(dlSpsCell, ue) 
RgSchCmnDlCellSpsInfo   *dlSpsCell;
RgSchUeCb               *ue;
#endif /* ANSI */
{
   RgSchCmnDlUeSpsInfo  *dlSpsUe = RG_SCH_CMN_SPS_GET_DL_UE(ue);
   
   TRC2(rgSCHCmnSpsUtlDlAdd2ToBeActvtdUes);
   
#ifdef RGSCH_SPS_STATS
   if (dlSpsUe->spsList  && dlSpsUe->pndngUeLstEnt.node && 
         (dlSpsUe->spsList != &dlSpsCell->toBeActvtdUes))
   {
      RG_SCH_ABORT();
   }
#endif 

   if (!dlSpsUe->pndngUeLstEnt.node)
   {
      dlSpsUe->pndngUeLstEnt.node = (PTR)ue;
      cmLListAdd2Tail(&dlSpsCell->toBeActvtdUes, &dlSpsUe->pndngUeLstEnt);
      dlSpsUe->spsList = &dlSpsCell->toBeActvtdUes;
   }
   RETVOID;
}

/***********************************************************
 *
 *     Func : rgSCHCmnSpsUtlDlDelFrmToBeActvtdUes
 *
 *
 *     Desc : Deletes UE from toBeActvtdUes list
 *
 *     Ret  : Void
 *            
 *
 *     Notes:
 *
 *     File :
 *
 **********************************************************/
#ifdef ANSI
PRIVATE Void rgSCHCmnSpsUtlDlDelFrmToBeActvtdUes
(
RgSchCmnDlCellSpsInfo   *dlSpsCell,
RgSchUeCb               *ue
)
#else /* ANSI */
PRIVATE  Void rgSCHCmnSpsUtlDlDelFrmToBeActvtdUes(dlSpsCell, ue) 
RgSchCmnDlCellSpsInfo   *dlSpsCell;
RgSchUeCb               *ue;
#endif /* ANSI */
{
   RgSchCmnDlUeSpsInfo  *dlSpsUe = RG_SCH_CMN_SPS_GET_DL_UE(ue);
   
   TRC2(rgSCHCmnSpsUtlDlDelFrmToBeActvtdUes);
   
   if (dlSpsUe->pndngUeLstEnt.node)
   {
//#if (ERRCLASS & ERRCLS_DEBUG)
#ifdef RGSCH_SPS_STATS
      if (dlSpsUe->spsList != &dlSpsCell->toBeActvtdUes)
      {
         RG_SCH_ABORT();
      }
#endif 
//#endif
      cmLListDelFrm(&dlSpsCell->toBeActvtdUes, &dlSpsUe->pndngUeLstEnt);
   }
   dlSpsUe->pndngUeLstEnt.node = NULLP;
   dlSpsUe->spsList = NULLP;
   RETVOID;
}

/***********************************************************
 *     
 *     Func : rgSCHCmnSpsUtlDlAdd2ActvUes
 *     
 *     
 *     Desc : Adds UE to the actvDlSpsUeLsts at the index corresponding to the
 *     time of activation
 *  
 *     Ret  : Void 
 *            
 *
 *     Notes:
 *
 *     File :
 *
 **********************************************************/
#ifdef ANSI
PRIVATE Void rgSCHCmnSpsUtlDlAdd2ActvUes
(
RgSchCmnDlCellSpsInfo   *dlSpsCell,
RgSchUeCb               *ue,
CmLteTimingInfo         actvTime
)
#else/* ANSI */
PRIVATE  Void rgSCHCmnSpsUtlDlAdd2ActvUes(dlSpsCell, ue, actvTime)
RgSchCmnDlCellSpsInfo   *dlSpsCell;
RgSchUeCb               *ue;
CmLteTimingInfo         actvTime;
#endif /* ANSI */
{
   RgSchCmnDlUeSpsInfo  *dlSpsUe = RG_SCH_CMN_SPS_GET_DL_UE(ue);
   U16                  idx;

   TRC2(rgSCHCmnSpsUtlDlAdd2ActvUes);
   
   idx =   (actvTime.sfn * 10 + actvTime.subframe) % RG_SCH_CMN_SPS_MAX_PRD;

   if (!dlSpsUe->actvUeLstEnt.node)
   {
      dlSpsUe->actvUeLstEnt.node = (PTR)ue;
      cmLListAdd2Tail(&dlSpsCell->actvDlSpsUeLsts[idx], 
              &dlSpsUe->actvUeLstEnt);
   }
   RETVOID;
}

/***********************************************************
 *
 *     Func : rgSCHCmnSpsUtlDlDelFrmActvUes
 *
 *
 *     Desc : Deletes UE from actvDlSpsUeLsts list
 *     Ret  : Void
 *
 *
 *     Notes:
 *
 *     File :
 *
 **********************************************************/
#ifdef ANSI
PRIVATE Void rgSCHCmnSpsUtlDlDelFrmActvUes
(
RgSchCmnDlCellSpsInfo   *dlSpsCell,
RgSchUeCb               *ue,
CmLteTimingInfo         actvTime
)
#else /* ANSI */
PRIVATE  Void rgSCHCmnSpsUtlDlDelFrmActvUes(dlSpsCell, ue, actvTime)
RgSchCmnDlCellSpsInfo   *dlSpsCell;
RgSchUeCb               *ue;
CmLteTimingInfo         actvTime;
#endif /* ANSI */
{
   RgSchCmnDlUeSpsInfo  *dlSpsUe = RG_SCH_CMN_SPS_GET_DL_UE(ue);
   U16   idx = (actvTime.sfn * 10 + actvTime.subframe) % RG_SCH_CMN_SPS_MAX_PRD;

   TRC2(rgSCHCmnSpsUtlDlDelFrmActvUes);
   
   if (dlSpsUe->actvUeLstEnt.node)
   { 
      cmLListDelFrm(&dlSpsCell->actvDlSpsUeLsts[idx], &dlSpsUe->actvUeLstEnt);
   }
   dlSpsUe->actvUeLstEnt.node = NULLP;

   RETVOID;
}

/***********************************************************
 *
 *     Func : rgSCHCmnSpsUtlDlAdd2ToBeRelUes
 *
 *
 *     Desc : Adds UE to the toBeRelUes list
 *
 *     Ret  : Void
 *            
 *
 *     Notes:
 *
 *     File :
 *
 **********************************************************/
#ifdef ANSI
PRIVATE Void rgSCHCmnSpsUtlDlAdd2ToBeRelUes
(
RgSchCmnDlCellSpsInfo   *dlSpsCell,
RgSchUeCb               *ue
)
#else /* ANSI */
PRIVATE  Void rgSCHCmnSpsUtlDlAdd2ToBeRelUes(dlSpsCell, ue) 
RgSchCmnDlCellSpsInfo   *dlSpsCell;
RgSchUeCb               *ue;
#endif /* ANSI */
{
   RgSchCmnDlUeSpsInfo  *dlSpsUe = RG_SCH_CMN_SPS_GET_DL_UE(ue);
   
   TRC2(rgSCHCmnSpsUtlDlAdd2ToBeRelUes);

#ifdef RGSCH_SPS_STATS
   if (dlSpsUe->spsList &&  dlSpsUe->pndngUeLstEnt.node &&
         (dlSpsUe->spsList != &dlSpsCell->toBeRelUes))
   {
      RG_SCH_ABORT();
   }
#endif

   if (!dlSpsUe->pndngUeLstEnt.node)
   {
      dlSpsUe->pndngUeLstEnt.node = (PTR)ue;
      cmLListAdd2Tail(&dlSpsCell->toBeRelUes, &dlSpsUe->pndngUeLstEnt);
      dlSpsUe->spsList = &dlSpsCell->toBeRelUes;
   }
   RETVOID;
}

/***********************************************************
 *
 *     Func : rgSCHCmnSpsUtlDlDelFrmToBeRelUes
 *
 *
 *     Desc : Deletes UE from toBeRelUes list
 *
 *     Ret  : Void
 *            
 *
 *     Notes:
 *
 *     File :
 *
 **********************************************************/
#ifdef ANSI
PRIVATE Void rgSCHCmnSpsUtlDlDelFrmToBeRelUes
(
RgSchCmnDlCellSpsInfo   *dlSpsCell,
RgSchUeCb               *ue
)
#else /* ANSI */
PRIVATE  Void rgSCHCmnSpsUtlDlDelFrmToBeRelUes(dlSpsCell, ue) 
RgSchCmnDlCellSpsInfo   *dlSpsCell;
RgSchUeCb               *ue;
#endif /* ANSI */
{
   RgSchCmnDlUeSpsInfo  *dlSpsUe = RG_SCH_CMN_SPS_GET_DL_UE(ue);
   
   TRC2(rgSCHCmnSpsUtlDlDelFrmToBeRelUes);
   
   if (dlSpsUe->pndngUeLstEnt.node)
   { 
//#if (ERRCLASS & ERRCLS_DEBUG)
#ifdef RGSCH_SPS_STATS
      if (dlSpsUe->spsList != &dlSpsCell->toBeRelUes)
      {
         RG_SCH_ABORT();
      }
#endif
//#endif
      cmLListDelFrm(&dlSpsCell->toBeRelUes, &dlSpsUe->pndngUeLstEnt);
   }
   dlSpsUe->pndngUeLstEnt.node = NULLP;
   dlSpsUe->spsList = NULLP;
   RETVOID;
}

/***********************************************************
 *
 *     Func : rgSCHCmnSpsUtlDlAdd2RetxHqProcs
 *
 *
 *     Desc : Adds HARQ proc to the list of re-transmitting SPS harq procs.
 *
 *     Ret  : Void
 *            
 *
 *     Notes:
 *
 *     File :
 *
 **********************************************************/
#ifdef ANSI
PRIVATE Void rgSCHCmnSpsUtlDlAdd2RetxHqProcs
(
RgSchCmnDlCellSpsInfo   *dlSpsCell,
RgSchDlHqProcCb         *hqP
)
#else /* ANSI */
PRIVATE  Void rgSCHCmnSpsUtlDlAdd2RetxHqProcs(dlSpsCell, hqP) 
RgSchCmnDlCellSpsInfo   *dlSpsCell;
RgSchDlHqProcCb         *hqP;
#endif /* ANSI */
{
   RgSchCmnDlHqProc     *cmnHqProcDl = RG_SCH_CMN_GET_DL_HQP(hqP);

   TRC2(rgSCHCmnSpsUtlDlAdd2RetxHqProcs);
   
   cmnHqProcDl->retxLnk.node = (PTR)hqP;

   cmLListAdd2Tail(&dlSpsCell->retxHqProcs, &cmnHqProcDl->retxLnk);
   RETVOID;
}

/***********************************************************
 *
 *     Func : rgSCHCmnSpsUtlDlDelFrmRetxHqProcs
 *
 *
 *     Desc : Deletes HARQ proc from the list of re-transmitting SPS harq 
 *     procs.
 *
 *     Ret  : Void
 *            
 *
 *     Notes:
 *
 *     File :
 *
 **********************************************************/
#ifdef ANSI
PRIVATE Void rgSCHCmnSpsUtlDlDelFrmRetxHqProcs
(
RgSchCmnDlCellSpsInfo   *dlSpsCell,
RgSchDlHqProcCb         *hqP
)
#else /* ANSI */
PRIVATE  Void rgSCHCmnSpsUtlDlDelFrmRetxHqProcs(dlSpsCell, hqP) 
RgSchCmnDlCellSpsInfo   *dlSpsCell;
RgSchDlHqProcCb         *hqP;
#endif /* ANSI */
{
   RgSchCmnDlHqProc     *cmnHqProcDl = RG_SCH_CMN_GET_DL_HQP(hqP);

   TRC2(rgSCHCmnSpsUtlDlDelFrmRetxHqProcs);
   if (cmnHqProcDl->retxLnk.node && cmnHqProcDl->isSpsSvcSchd) 
   {
      cmLListDelFrm(&dlSpsCell->retxHqProcs, &cmnHqProcDl->retxLnk);
      cmnHqProcDl->retxLnk.node = NULLP;
   }
   RETVOID;
}

/***********************************************************
 *
 *     Func : rgSCHCmnSpsUtlDlAdd2ToBeSchdSvcs
 *
 *
 *     Desc : Adds a service to toBeSchdSvcs list
 *
 *     Ret  : Void
 *            
 *
 *     Notes:
 *
 *     File :
 *
 **********************************************************/
#ifdef ANSI
PRIVATE Void rgSCHCmnSpsUtlDlAdd2ToBeSchdSvcs
(
RgSchCmnDlCellSpsInfo   *dlSpsCell,
RgSchDlLcCb             *dlSvc
)
#else /* ANSI */
PRIVATE  Void rgSCHCmnSpsUtlDlAdd2ToBeSchdSvcs(dlSpsCell, dlSvc) 
RgSchCmnDlCellSpsInfo   *dlSpsCell;
RgSchDlLcCb             *dlSvc;
#endif /* ANSI */
{
   RgSchCmnDlSvcSpsInfo *dlSpsSvc = RG_SCH_CMN_SPS_GET_DL_SVC(dlSvc);
   
   TRC2(rgSCHCmnSpsUtlDlAdd2ToBeSchdSvcs);
   
   if (!dlSpsSvc->toBeSchdSvcEnt.node)
   {
      dlSpsSvc->toBeSchdSvcEnt.node = (PTR)dlSvc;

      cmLListAdd2Tail(&dlSpsCell->toBeSchdSvcs, &dlSpsSvc->toBeSchdSvcEnt);
   }
   RETVOID;
}

/***********************************************************
 *
 *     Func : rgSCHCmnSpsUtlDlDelFrmToBeSchdSvcs
 *
 *
 *     Desc : Deletes a service from toBeSchdSvcs list
 *
 *     Ret  : Void
 *            
 *
 *     Notes:
 *
 *     File :
 *
 **********************************************************/
#ifdef ANSI
PRIVATE Void rgSCHCmnSpsUtlDlDelFrmToBeSchdSvcs
(
RgSchCmnDlCellSpsInfo   *dlSpsCell,
RgSchDlLcCb             *dlSvc
)
#else /* ANSI */
PRIVATE  Void rgSCHCmnSpsUtlDlDelFrmToBeSchdSvcs(dlSpsCell, dlSvc) 
RgSchCmnDlCellSpsInfo   *dlSpsCell;
RgSchDlLcCb             *dlSvc;
#endif /* ANSI */
{
   RgSchCmnDlSvcSpsInfo *dlSpsSvc = RG_SCH_CMN_SPS_GET_DL_SVC(dlSvc);

   TRC2(rgSCHCmnSpsUtlDlDelFrmToBeSchdSvcs);
   
   if (dlSpsSvc->toBeSchdSvcEnt.node)
   {
      cmLListDelFrm(&dlSpsCell->toBeSchdSvcs, &dlSpsSvc->toBeSchdSvcEnt);
      dlSpsSvc->toBeSchdSvcEnt.node = NULLP;
   }
   RETVOID;
}

/***********************************************************
 *
 *     Func : rgSCHCmnSpsUtlDlAdd2TxSpsUeLst
 *
 *
 *     Desc : This function adds the UE to alloc info's TX SPS UE list.
 *
 *     Ret  : Void
 *            
 *
 *     Notes:
 *
 *     File :
 *
 **********************************************************/
#ifdef ANSI
PRIVATE Void rgSCHCmnSpsUtlDlAdd2TxSpsUeLst
(
RgSchCmnDlRbAllocInfo *allocInfo,
RgSchDlHqProcCb      *hqP
)
#else /* ANSI */
PRIVATE Void rgSCHCmnSpsUtlDlAdd2TxSpsUeLst(allocInfo, hqP)
RgSchCmnDlRbAllocInfo *allocInfo;
RgSchDlHqProcCb      *hqP;
#endif /* ANSI */
{

   TRC2(rgSCHCmnSpsUtlDlAdd2TxSpsUeLst);

   cmLListAdd2Tail(&allocInfo->dedAlloc.txSpsHqPLst, &hqP->reqLnk);
   hqP->reqLnk.node = (PTR)hqP;
   RETVOID;
}

/***********************************************************
 *
 *     Func : rgSCHCmnSpsUtlDlAdd2RetxSpsUeLst
 *
 *
 *     Desc : This function adds the UE to alloc info's Retx SPS UE list.
 *
 *     Ret  : Void
 *            
 *
 *     Notes:
 *
 *     File :
 *
 **********************************************************/
#ifdef ANSI
PRIVATE Void rgSCHCmnSpsUtlDlAdd2RetxSpsUeLst
(
RgSchCmnDlRbAllocInfo *allocInfo,
RgSchDlHqProcCb      *hqP
)
#else /* ANSI */
PRIVATE Void rgSCHCmnSpsUtlDlAdd2RetxSpsUeLst(allocInfo, hqP)
RgSchCmnDlRbAllocInfo *allocInfo;
RgSchDlHqProcCb      *hqP;
#endif /* ANSI */
{

   TRC2(rgSCHCmnSpsUtlDlAdd2RetxSpsUeLst);
   cmLListAdd2Tail(&allocInfo->dedAlloc.retxSpsHqPLst, &hqP->reqLnk);
   hqP->reqLnk.node = (PTR)hqP;

   RETVOID;
}

/***************************************************************
                         DL SPS Utility Functions End
****************************************************************/

/*!< Changes needed in other modules and functions 
 *  tfu.x: 
 *
 *  Change the bitmask used for allocation from an array to a U32 value
 *  Corresponding changes in the code and the test engine also needs to be
 *  taken care of
 *  
 *  rg_sch_dhm.c :
 *  
 *  On reception of positive feedback, a trigger could be possibly sent to SPS
 *  to unset isSpsActv variable on SPS HARQ proc.
 * */
/***************************************************************
                         UL SPS Functions
****************************************************************/


/**
 * @brief Handler for validating Cell configuration request for UL SPS
 *
 * @details
 *
 *     Function: rgSCHCmnSpsUlVldtCellCfg
 *
 *     Invoked by: 
 *         rgSCHCmnSpsCellCfg as part of Cell configuration handling
 *
 *     Processing Steps for UL SPS:
 *       if(cellCfg->puschCfg.hopOffst == cellUl->puschBW)
 *         No bandwidth left for SPS
 *         return RFAILED
 *       return ROK
 *           
 *  @param[in] RgSchCellCb    *cell
 *  @param[in] RgrCellCfg     *cellCfg
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED 
 **/
#ifdef ANSI
PRIVATE S16 rgSCHCmnSpsUlVldtCellCfg
(
RgSchCellCb    *cell,
RgrCellCfg     *cellCfg
)
#else /* ANSI */
PRIVATE S16 rgSCHCmnSpsUlVldtCellCfg(cell, cellCfg)
RgSchCellCb    *cell;
RgrCellCfg     *cellCfg;
#endif /* ANSI */
{
   TRC2(rgSCHCmnSpsUlVldtCellCfg);
 
   U8   puschBw = cell->dynCfiCb.bwInfo[cell->dynCfiCb.maxCfi].numSb * 
                           cellCfg->puschSubBand.size;
     

   if (cellCfg->puschCfg.hopOffst == puschBw)
   {
      RLOG_ARG0(L_ERROR,DBG_CELLID,cell->cellId,
            "rgSCHCmnSpsUlVldtCellCfg:No bandwidth for SPS allocations");
      return RFAILED;
   }

   RETVALUE(ROK);
} /* end of rgSCHCmnSpsUlVldtCellCfg */

/**
 * @brief Handler for processing Cell configuration request 
 *
 * @details
 *
 *     Function: rgSCHCmnSpsUlCellCfg
 *
 *     Invoked by: 
 *         rgSCHCmnSpsCellCfg as part of Cell configuration handling
 *
 *     Processing Steps for UL SPS:
 *       Store the puschCfg in rgSchCellCb
 *       Calculate SPS subband information using the pusch hopping offset
 *       if(cellCfg->puschCfg.hopOffst%2)
 *         spsRbs = cellCfg->bwCfg.ulTotalBw - cellCfg->puschCfg.hopOffst - 1
 *       else
 *         spsRbs = cellCfg->bwCfg.ulTotalBw - cellCfg->puschCfg.hopOffst
 *       cmnCell->ul.ulSpsInfo.spsSbStart = 
 *                     CEIL((cellCfg->bwCfg.ulTotalBw - spsRbs)/2/numSbs)
 *       cmnCell->ul.ulSpsInfo.spsSbEnd = 
 *                     (cmnCell->ul.ulSpsInfo.spsSbStart + (spsRbs)/sbSize) -1
 *       cmnCell->ul.ulSpsInfo.spsPrdLcmVal = 1
 *       cmnCell->ul.ulSpsInfo.spsSfArr = NULL
 *       Initialize actvUlSpsUeLsts, toBeActvtdUeLst, 
 *       toBeRelUeLst
 *           
 *  @param[in] RgSchCellCb    *cell
 *  @param[in] RgrCellCfg     *cellCfg
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED 
 **/
#ifdef ANSI
PRIVATE S16 rgSCHCmnSpsUlCellCfg
(
RgSchCellCb    *cell,
RgrCellCfg    *cellCfg
)
#else /* ANSI */
PRIVATE S16 rgSCHCmnSpsUlCellCfg(cell, cellCfg)
RgSchCellCb    *cell;
RgrCellCfg    *cellCfg;
#endif /* ANSI */
{
   RgSchCmnUlCellSpsInfo *ulSpsCell = RG_SCH_CMN_GET_UL_SPS_CELL(cell);
   U8                    spsRbStart; /* Starting RB for SPS */
   U16                   idx;
   U8 numSpsSb=0;

   TRC2(rgSCHCmnSpsUlCellCfg);


   /* Calculate SPS subband information using the pusch hopping offset */
   if (!cellCfg->puschCfg.hopOffst)
   {
      ulSpsCell->spsSbStart = 0; 
      ulSpsCell->numSpsSb = cell->dynCfiCb.bwInfo[cell->dynCfiCb.maxCfi].numSb;
   }
   else
   {
      U8   hopOffst;
      U8   puschRbStart;
      hopOffst = cellCfg->puschCfg.hopOffst;
      
      spsRbStart = RGSCH_CEIL(hopOffst,2);
      puschRbStart = (cell->bwCfg.ulTotalBw - spsRbStart)/2;
      ulSpsCell->spsSbStart = RGSCH_CEIL(spsRbStart - puschRbStart, cellCfg->puschSubBand.size);
      numSpsSb =
             (cell->bwCfg.ulTotalBw - spsRbStart - puschRbStart) / cellCfg->puschSubBand.size;
      if(numSpsSb)
      {
      ulSpsCell->numSpsSb=numSpsSb- ulSpsCell->spsSbStart;
      }
       
   }

   rgSCHCmnSpsUlInitSpsSf (cell);

   for(idx=0; idx < RG_SCH_CMN_SPS_MAX_PRD; idx++)
   {
      cmLListInit (&(ulSpsCell->actvUlSpsUeLsts[idx]));
   }
   cmLListInit (&ulSpsCell->toBeActvtdUeLst);
   cmLListInit (&ulSpsCell->toBeRelUeLst);

   RETVALUE(ROK);
} /* end of rgSCHCmnSpsUlCellCfg */


/**
 * @brief Handler for processing Cell deletion request 
 *
 * @details
 *
 *     Function: rgSCHCmnSpsUlCellDel
 *
 *     Invoked by: 
 *         rgSCHCmnSpsCellDel as part of Cell deletion handling
 *
 *     Processing Steps for UL SPS:
 *       Deallocate cell->ulSpsInfo.spsSfLst
 *       toBeRelUeLst
 *           
 *  @param[in] RgSchCellCb    *cell
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED 
 **/
#ifdef ANSI
PRIVATE S16 rgSCHCmnSpsUlCellDel
(
RgSchCellCb    *cell
)
#else /* ANSI */
PRIVATE S16 rgSCHCmnSpsUlCellDel(cell)
RgSchCellCb    *cell;
#endif /* ANSI */
{
   RgSchCmnUlCellSpsInfo *ulSpsCell = RG_SCH_CMN_GET_UL_SPS_CELL(cell);

   TRC2(rgSCHCmnSpsUlCellDel);


   /* Free cell specific sps information */
   if (ulSpsCell->spsSfLst)
   {
      /* ccpu00117052 - MOD - Passing double pointer
      for proper NULLP assignment*/
      rgSCHUtlFreeSBuf(cell->instIdx, (Data **)(&(ulSpsCell->spsSfLst)),
                    (ulSpsCell->spsPrdLcmVal* sizeof(RgSchCmnSpsUlSf)));
   }

   /* Free SPS lists */
   rgSCHCmnSpsUlFreeToBeActvUeLst(cell);
   rgSCHCmnSpsUlFreeToBeRelUeLst(cell);
   rgSCHCmnSpsUlFreeActvUeLst(cell);

   /* CellDel shall invoke ueDel which will take care of SPS UE deletion */

   RETVALUE(ROK);
} /* end of rgSCHCmnSpsUlCellDel */


/**
 * @brief Handler for validating UL SPS configuration received as part of UE
 * configuration request 
 *
 * @details
 *
 *     Function: rgSCHCmnSpsUlVldtUeCfg
 *
 *     Handler for validating SPS configuration that comes as part of 
 *     UE Configuration request 
 *
 *     Invoked by: 
 *         rgSCHCmnSpsUeCfg as part of UE configuration handling
 *
 *     Processing Steps: 
 *       if ((ueCfg->ueSpsCfg.ulSpsCfg.isUlSpsEnabled == TRUE) && 
 *           (ueCfg->ueAckNackCfg.isAckNackEnabled == TRUE))
 *            return RFAILED as both are not supported for a UE simultaneously
 *       return ROK
 *           
 *  @param[in] RgSchCellCb  *cell
 *  @param[in] RgrUeCfg     *ueCfg
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED 
 **/
#ifdef ANSI
PRIVATE S16 rgSCHCmnSpsUlVldtUeCfg
(
RgSchCellCb  *cell,
RgrUeCfg     *ueCfg
)
#else /* ANSI */
PRIVATE S16 rgSCHCmnSpsUlVldtUeCfg(cell, ueCfg)
RgSchCellCb  *cell;
RgrUeCfg     *ueCfg;
#endif /* ANSI */
{

   U16 prdIdx = 0;

   TRC2(rgSCHCmnSpsUlVldtUeCfg);


   if (ueCfg->ueAckNackCfg.isAckNackEnabled == TRUE)
   {
      /* SPS is not supported when ACK-NACK repetition is enabled for a UE */
      RLOG_ARG1(L_ERROR,DBG_CELLID,cell->cellId,
            "rgSCHCmnSpsUlVldtUeCfg:SPS is not supported when ACK-NACK"
            " repetition is enabled for a UE new CRNTI:%d",ueCfg->crnti);
      return RFAILED;
   }

   /* Check if num SPS LCs is more than one
      Presently only one SPS LC is supported */
   if (ueCfg->ueSpsCfg.ulSpsCfg.lcCnt > 1)
   {
      /* SPS is not supported when ACK-NACK repetition is enabled for a UE */
      RLOG_ARG1(L_ERROR,DBG_CELLID,cell->cellId,
            "rgSCHCmnSpsUlVldtUeCfg: Invalid SPS LC Count CRNTI:%d",ueCfg->crnti);
      return RFAILED;
   }

#ifdef LTE_TDD
   /* Only supporting Periodicity values that are multiple of 10 */
   if (ueCfg->ueSpsCfg.ulSpsCfg.ulSpsPrdctyEnum % 10)
   {
      RLOG_ARG1(L_ERROR,DBG_CELLID,cell->cellId,
         "rgSCHCmnSpsUlVldtUeCfg: Invalid Periodicity CRNTI:%d",ueCfg->crnti);
      return RFAILED;
   }
#endif

   RG_SCH_CMN_SPS_GET_PRD_IDX ((int)ueCfg->ueSpsCfg.ulSpsCfg.ulSpsPrdctyEnum,
         &prdIdx);

   /* Note: prdIdx shall not be invalid since all possible periodicity
    * configurations have been taken care of */
   if (prdIdx >= RG_SCH_CMN_SPS_PRD_INVALID)
   {
      RLOG_ARG2(L_ERROR,DBG_CELLID,cell->cellId,
               "Invalid SPS periodicity for CRNTI:%d, periodicity %d", 
               (int)ueCfg->crnti, (int)ueCfg->ueSpsCfg.ulSpsCfg.ulSpsPrdctyEnum);
      RETVALUE(RFAILED);
   }
   RETVALUE(ROK);
} /* end of rgSCHCmnSpsUlVldtUeCfg */

/**
 * @brief Handler for processing UL SPS configuration of a UE 
 *
 * @details
 *
 *     Function: rgSCHCmnSpsUlUeCfg
 *
 *     Handler for processing UL SPS configuration that comes as part of
 *     UE Configuration request 
 *
 *     Invoked by: 
 *         rgSCHCmnSpsUeCfg 
 *
 *     Processing Steps: 
 *        - Store the given configuration in ue->ul.ulSpsInfo
 *        - for TDD
 *          ue->ul.ulSpsCfg.spsPrdcty = ueCfg->spsPrdcty - (ueCfg->spsPrdcty % 10)
 *        - for FDD
 *          ue->ul.ulSpsCfg.spsPrdcty = ueCfg->spsPrdcty
 *        - Compute newLCM value using oldLCM value and the SPS
 *          periodicity given in the configuration.
 *            - if (newLcm > oldLcm) 
 *                 if (spsSfLst == NULLP)
 *                    allocate spsSfLst upto newLcm value
 *                    initialize spsAllocDb and spsHoleDb according to SPS BW
 *                 if (spsSfLst)
 *                    reallocate the spsSfLst
 *                    replicate the subframe information till the newLcm
 *        - if (ueCfg->ueMesGapCfg.isMesGapEnabled == TRUE)
 *            - allocate ueCmnUlCb->ulSpsInfo.measGapMask with
 *                        size= (((ueCfg->ueMesGapCfg.gapPrd)) * sizeof(U8))
 *            - set indices in ueUlCb->ulSpsInfo.measGapMask based on the
 *              inactivity subframes due to measurement gap.
 *           
 *  @param[in] RgSchCellCb  *cell
 *  @param[in] RgSchUeCb    *ue
 *  @param[in] RgrUeCfg     *ueCfg
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED 
 **/
#ifdef ANSI
PRIVATE S16 rgSCHCmnSpsUlUeCfg
(
RgSchCellCb  *cell,
RgSchUeCb    *ue,
RgrUeCfg     *ueCfg
)
#else /* ANSI */
PRIVATE S16 rgSCHCmnSpsUlUeCfg(cell, ue, ueCfg)
RgSchCellCb  *cell;
RgSchUeCb    *ue;
RgrUeCfg     *ueCfg;
#endif /* ANSI */
{
   RgSchCmnUlCellSpsInfo *ulSpsCell = RG_SCH_CMN_GET_UL_SPS_CELL(cell);
   U16                   crntLcm = ulSpsCell->spsPrdLcmVal;
   U16                   newLcm;

   TRC2(rgSCHCmnSpsUlUeCfg);
 

   /* Init Ue with given configuration */
   rgSCHCmnSpsUlUeInit(cell, ue, ueCfg->ueSpsCfg,
                             ueCfg->ueMesGapCfg);

   /* Calculate new LCM using the existing value and the UE's periodicity */
#ifdef LTE_TDD
   /* In case Two interval config is TRUE then SPS occasion change based 
      on subframe offset. Hence spsPrdLcmVal should be a multiple and 
      greater than "Ul SPS Periodicity" */
   if (ue->ul.ulSpsCfg.twoIntervalCfg == TRUE)
   {
      newLcm = rgSCHCmnSpsCompLcm (crntLcm, (2 * ue->ul.spsPrdcty));
   }
   else
   {
      newLcm = rgSCHCmnSpsCompLcm (crntLcm, ue->ul.spsPrdcty);
   }
#else
   newLcm = rgSCHCmnSpsCompLcm (crntLcm, ue->ul.spsPrdcty);
#endif

   if (newLcm > crntLcm)
   {
      /* Create or update the cell-wide spsSfLst */
      if ((rgSCHCmnSpsUlUpdSpsSf(cell, crntLcm, newLcm)) != ROK)
      {
         RLOG_ARG1(L_ERROR,DBG_CELLID,cell->cellId,
               "SPS subframe update failed while configuring CRNTI:%d", 
               ue->ueId);
         RETVALUE(RFAILED);
      }
      /* Store the new LCM as the current LCM */
      ulSpsCell->spsPrdLcmVal = newLcm;
   } /* end of newLcm > crntLcm */


   RETVALUE(ROK);
} /* end of rgSCHCmnSpsUlUeCfg */

/**
 * @brief Handler for validating UL SPS configuration received as part of UE
 * reconfiguration request 
 *
 * @details
 *
 *     Function: rgSCHCmnSpsUlVldtUeRecfg
 *
 *     Handler for validating SPS configuration that comes as part of 
 *     UE Reconfiguration request 
 *
 *     Invoked by: 
 *         rgSCHCmnSpsUeRecfg as part of UE reconfiguration handling
 *
 *     Processing Steps: 
 *       if ((ueRecfg->ueSpsCfg.ulSpsCfg.isUlSpsEnabled == TRUE) && 
 *           (ueRecfg->ueAckNackCfg.isAckNackEnabled == TRUE))
 *            return RFAILED as both are not supported for a UE simultaneously
 *       if ((ue->isUlSpsActv == TRUE) && 
 *           (ueRecfg->ueAckNackCfg.isAckNackEnabled == TRUE))
 *            return RFAILED
 *       return ROK
 *           
 *  @param[in] RgSchCellCb  *cell
 *  @param[in] RgSchUeCb    *ue
 *  @param[in] RgrUeCfg     *ueCfg
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED 
 **/
#ifdef ANSI
PRIVATE S16 rgSCHCmnSpsUlVldtUeRecfg
(
RgSchCellCb  *cell,
RgSchUeCb    *ue,
RgrUeRecfg   *ueRecfg
)
#else /* ANSI */
PRIVATE S16 rgSCHCmnSpsUlVldtUeRecfg(cell, ue, ueRecfg)
RgSchCellCb  *cell;
RgSchUeCb    *ue;
RgrUeRecfg   *ueRecfg;
#endif /* ANSI */
{
   U16 prdIdx = 0;
   TRC2(rgSCHCmnSpsUlVldtUeRecfg);
 

   if (((ue->ackNakRepCb.isAckNackEnabled == TRUE) &&
        (ueRecfg->ueSpsRecfg.ulSpsCfg.isUlSpsEnabled == TRUE)) ||
       ((ueRecfg->ueAckNackRecfg.isAckNackEnabled == TRUE) &&
        ((ue->ul.ulSpsCfg.isUlSpsEnabled == TRUE) ||
         (ueRecfg->ueSpsRecfg.ulSpsCfg.isUlSpsEnabled == TRUE))))
   {
      /* SPS is not supported when ACK-NACK repetition is enabled for a UE */
      RLOG_ARG1(L_ERROR,DBG_CELLID,cell->cellId,
            "rgSCHCmnSpsUlVldtUeCfg:SPS is not supported when ACK-NACK"
            " repetition is enabled for a CRNTI:%d",ue->ueId);
      RETVALUE(RFAILED);
   }


   /* Check if num SPS LCs is more than one
      Presently only one SPS LC is supported */
   if (ueRecfg->ueSpsRecfg.ulSpsCfg.lcCnt > 1)
   {
      /* SPS is not supported when ACK-NACK repetition is enabled for a UE */
      RLOG_ARG1(L_ERROR,DBG_CELLID,cell->cellId,
            "rgSCHCmnSpsUlVldtUeRecfg: Invalid SPS LC Count CRNTI:%d",ue->ueId);
      return RFAILED;
   }
   
#ifdef LTE_TDD
   /* Only supporting Periodicity values that are multiple of 10 */
   if (ueRecfg->ueSpsRecfg.ulSpsCfg.ulSpsPrdctyEnum % 10)
   {
      RLOG_ARG1(L_ERROR,DBG_CELLID,cell->cellId,
         "rgSCHCmnSpsUlVldtUeRecfg: Invalid SPS Periodicity CRNTI:%d",ue->ueId);
      return RFAILED;
   }
#endif

   RG_SCH_CMN_SPS_GET_PRD_IDX ((int)ueRecfg->ueSpsRecfg.ulSpsCfg.ulSpsPrdctyEnum,
         &prdIdx);

   /* Note: prdIdx shall not be invalid since all possible periodicity
    * configurations have been taken care of */
   if (prdIdx >= RG_SCH_CMN_SPS_PRD_INVALID)
   {
      RLOG_ARG2(L_ERROR,DBG_CELLID,cell->cellId,
               "Invalid SPS periodicity for CRNTI:%d, periodicity %d", 
               (int)ue->ueId, (int)ueRecfg->ueSpsRecfg.ulSpsCfg.ulSpsPrdctyEnum);
      RETVALUE(RFAILED);
   }

   RETVALUE(ROK);
} /* end of rgSCHCmnSpsUlVldtUeRecfg */

/**
 * @brief Handler for processing UE reconfiguration request 
 *
 * @details
 *
 *     Function: rgSCHCmnSpsUlUeRecfg
 *
 *     Handler for processing UL SPS configuration/reconfiguration that 
 *     comes as part of UE reconfiguration request 
 *
 *     Invoked by: 
 *         rgSCHCmnRgrUeRecfg as part of UE reconfiguration handling
 *
 *     Processing Steps:
 *        -  invoke rgSCHCmnSpsUlVldtUeRecfg
 *        - if validation is unsuccessful,
 *             return RFAILED 
 *        -  if (ue->isUlSpsEnabled == TRUE) 
 *              if (ueRecfg->ueSpsRecfg.ulSpsCfg.isUlSpsEnabled == TRUE)
 *                 return RFAILED
 *              if (ueRecfg->ueSpsRecfg.ulSpsCfg.isUlSpsEnabled == FALSE))
 *                 if (ue->isUlSpsActv) 
 *                   add UE to toBeRelUes queue for UL SPS Release
 *        -  If (ue->isUlSpsEnabled ==FALSE)
 *              if (ueRecfg->ueSpsRecfg.ulSpsCfg.isUlSpsEnabled == TRUE)
 *                 perform the same actions as in ueCfg handling
 *                 return ROK. 
 *           
 *  @param[in] RgSchCellCb  *cell
 *  @param[in] RgSchUeCb    *ue
 *  @param[in] RgrUeRecfg   *ueRecfg
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED 
 **/
#ifdef ANSI
PRIVATE S16 rgSCHCmnSpsUlUeRecfg
(
RgSchCellCb  *cell,
RgSchUeCb    *ue,
RgrUeRecfg   *ueRecfg
)
#else /* ANSI */
PRIVATE S16 rgSCHCmnSpsUlUeRecfg(cell, ue, ueRecfg)
RgSchCellCb  *cell;
RgSchUeCb    *ue;
RgrUeRecfg   *ueRecfg;
#endif /* ANSI */
{
   RgSchCmnUlCellSpsInfo *ulSpsCell = RG_SCH_CMN_GET_UL_SPS_CELL(cell);
   RgSchCmnUlUeSpsInfo   *ulSpsUe = RG_SCH_CMN_GET_UL_SPS_UE(ue);
   U16                   newLcm;
   Bool                  isSpsRecfgPres = FALSE;
   Bool                  isSpsEnabInRecfg = FALSE;

   TRC2(rgSCHCmnSpsUlUeRecfg);
   

   /* Validate the input reconfiguration parameters */
   if (ueRecfg->ueRecfgTypes & RGR_UE_ULSPS_RECFG)
   {
      isSpsRecfgPres = TRUE;
      if (ueRecfg->ueSpsRecfg.ulSpsCfg.isUlSpsEnabled == TRUE) 
      {
         if (rgSCHCmnSpsUlVldtUeRecfg(cell, ue, ueRecfg) != ROK)
         {
            RLOG_ARG1(L_ERROR,DBG_CELLID,cell->cellId,
                     "UE recfg validation failed for OLD CRNTI:%d", ueRecfg->oldCrnti);
            RETVALUE(RFAILED);
         }
         isSpsEnabInRecfg = TRUE;
      }
   }

   /* Reconfiguration of UL SPS params is not supported. However, config/ 
    * reconfig of measurement gap shall be considered */ 
   if ((ue->ul.ulSpsCfg.isUlSpsEnabled == TRUE) &&
        ((FALSE == isSpsRecfgPres) ||
       ((TRUE == isSpsRecfgPres) &&  (isSpsEnabInRecfg == TRUE))))
   {
     /* Update measurement Gap mask of the UE if measurement gap is
       * re-configured */
      if (ueRecfg->ueRecfgTypes & RGR_UE_MEASGAP_RECFG)
      {
         rgSCHCmnSpsUlUeMeasGapMaskInit(cell, ue, ueRecfg->ueMeasGapRecfg);

         if (ulSpsUe->isUlSpsActv)
         {
           /* Check collision with existing activation */
            if (TRUE == rgSCHCmnSpsUlChkMesGapColl(cell, ue,
                ulSpsUe->ulSpsSchdInfo.crntUlSpsOcc,
                ueRecfg->ueMeasGapRecfg.gapPrd))
            {
               if ((ulSpsUe->state != RGSCH_SPS_TO_BE_REL) ||
                     (ulSpsUe->state != RGSCH_SPS_REL_SENT))
               {
                  /* Existing activation collides, mark UE to be re - acitvated */
                  /* Since current Allocation collides with Meas Gap release the SPS 
                     resources for this UE and Mark UE for Activation */
                  rgSCHCmnSpsUlUtlRmvSpsAlloc(cell, ue);
                  ulSpsUe->state = RGSCH_SPS_TO_BE_ACTV;
                  RLOG_ARG2(L_DEBUG,DBG_CELLID,cell->cellId,
                           "UL SPS needs to be re-activated as new config "
                           "collides with measurement gap for UEID:%d, spsRnti %d", 
                           ue->ueId, 
                           ue->spsRnti);
                  rgSCHCmnSpsUlUtlInsUeInToBeActvLst(cell, ue);
               }
            }
         }
         RETVALUE(ROK);
      }
   }

   if ((ue->ul.ulSpsCfg.isUlSpsEnabled == TRUE) &&
         (TRUE == isSpsRecfgPres) && (isSpsEnabInRecfg == FALSE))
   {
      /* SPS is getting disabled for the UE. Free UE's SPS resources */
      rgSCHCmnSpsUlUtlFreeSpsUe (cell, ue);
   }
   else if ((ue->ul.ulSpsCfg.isUlSpsEnabled == FALSE) &&
       (TRUE == isSpsRecfgPres) &&  (isSpsEnabInRecfg == TRUE))
   {
      /* UL SPS is getting enabled here. Init Ue with given configuration */
      rgSCHCmnSpsUlUeInit(cell, ue, ueRecfg->ueSpsRecfg,
            ueRecfg->ueMeasGapRecfg);
      /* Calculate new LCM using the existing value and the UE's periodicity */
#ifdef LTE_TDD
      /* ccpu00139528- In case Two interval config is TRUE then SPS occasion change based 
         on subframe offset. Hence spsPrdLcmVal should be a multiple and 
         greater than "Ul SPS Periodicity" */
      if (ue->ul.ulSpsCfg.twoIntervalCfg == TRUE)
      {
         newLcm = rgSCHCmnSpsCompLcm (ulSpsCell->spsPrdLcmVal, (2*ue->ul.spsPrdcty));
      }
      else
#endif
      {
         newLcm = rgSCHCmnSpsCompLcm (ulSpsCell->spsPrdLcmVal, ue->ul.spsPrdcty);
      }

      if (newLcm > ulSpsCell->spsPrdLcmVal)
      {
         
         /* Update the cell-wide spsSfLst */
         if ((rgSCHCmnSpsUlUpdSpsSf(cell, ulSpsCell->spsPrdLcmVal, newLcm)) 
                                                                      != ROK)
         {
            RLOG_ARG1(L_ERROR,DBG_CELLID,cell->cellId,
                "SPS subframe update failed while configuring CRNTI:%d", 
                     ue->ueId);
            RETVALUE(RFAILED);
         }
        
        /* OK first update ulSpsCell with the new LCM that we just computed in the case of reconfiguring the UE */
        ulSpsCell->spsPrdLcmVal = newLcm;
      }
   }
   
   RETVALUE(ROK);
} /* end of rgSCHCmnSpsUlUeRecfg */


/**
 * @brief  Handler for processing UE delete request
 *
 * @details
 *
 *     Function : rgSCHCmnSpsUlUeDel
 *
 *     Invoked by:
 *      rgSCHCmnUeDel if(ue->isUlSpsEnabled == TRUE)
 *
 *     Processing Steps:
 *       mark the UE allocation in the corr subframes to be free
 *       free ue->measGapMask if it exists
 *       remove UE from its existing lists
 *       return ROK
 *
 * @param[in]  RgSchCellCb *cell
 * @param[in]  RgSchUeCb   *ue
 * @return  Void
 **/
#ifdef ANSI
PRIVATE Void rgSCHCmnSpsUlUeDel 
(
RgSchCellCb      *cell,
RgSchUeCb        *ue
)
#else /* ANSI */
PRIVATE Void rgSCHCmnSpsUlUeDel (cell, ue) 
RgSchCellCb      *cell;
RgSchUeCb        *ue;
#endif /* ANSI */
{

   TRC2(rgSCHCmnSpsUlUeDel); 
 

   /* Free SPS specific information of the UE */
   rgSCHCmnSpsUlUtlFreeSpsUe (cell, ue);
  
   /* NOTE: No dereg Req to MAC as Ue shall be deleted in MAC too */

   RETVOID;
} /* end of rgSCHCmnSpsUlUeDel */

/**
 * @brief  Handler for processing SPS LCG delete request
 *
 * @details
 *
 *     Function : rgSCHCmnSpsUlLcgDel
 *
 *     Invoked by:
 *      rgSCHCmnLcgDel for UL SPS enabled UE 
 *
 *     Processing Steps:
 *       free the allocation of the UE at the scheduled offset and
 *       in the subframes corr to the UE's UL SPS periodicity
 *       remove UE from the actvUlSpsUeLsts
 *       Deregister with MAC
 *       return ROK
 *
 * @param[in]  RgSchCellCb *cell
 * @param[in]  RgSchUeCb   *ue
 * @param[in]  RgSchLcgCb  *lcg
 * @return  Void
 **/
#ifdef ANSI
PUBLIC Void rgSCHCmnSpsUlLcgDel 
(
RgSchCellCb      *cell,
RgSchUeCb        *ue,
RgSchLcgCb       *lcg
)
#else /* ANSI */
PUBLIC Void rgSCHCmnSpsUlLcgDel (cell, ue, lcg) 
RgSchCellCb      *cell;
RgSchUeCb        *ue;
RgSchLcgCb       *lcg;
#endif /* ANSI */
{
   RgSchCmnUlUeSpsInfo   *ulSpsUe = RG_SCH_CMN_GET_UL_SPS_UE(ue);
  
   TRC2(rgSCHCmnSpsUlLcgDel); 


   if ((lcg->lcgId != RG_SCH_SPS_LCG_ID) ||
      (RGSCH_SPS_IDLE == ulSpsUe->state) ||
      (RGSCH_SPS_REL_SENT == ulSpsUe->state) ||
      (RGSCH_SPS_TO_BE_REL == ulSpsUe->state))
   {
      /* need not do anything */
      RETVOID;
   }
    /* Remove Ue from other lists, if it exists */
    if ((ulSpsUe->state == RGSCH_SPS_TO_BE_ACTV) ||
       (ulSpsUe->state == RGSCH_SPS_TO_BE_REACTV))
    {
        rgSCHCmnSpsUlDelUeFrmToBeActvLst(cell, ue);
        RETVOID;
    }
   /* Send an explicit release and when release is successful, then delete
    * the SPS information of the UE */ 
    rgSCHCmnSpsUlProcRelInd(cell, ue, TRUE);

   RETVOID;
} /* end of rgSCHCmnSpsUlLcgDel */


/**
 * @brief Handler for processing UE reset request
 *
 * @details
 *
 *     Function : rgSCHCmnSpsUlUeReset
 *
 *     Invoked By:
 *       rgSCHCmnUlUeReset if (ue->isUlSpsEnabled == TRUE)
 *
 *     Processing Steps:
 *       if (ue->isUlSpsActv == TRUE)
 *          remove the configured grants allocated for the UE.
 *       remove Ue from lists if it exists
 *
 *  @param[in]  RgSchCellCb        *cell
 *  @param[in]  RgSchUeCb          *ue
 *  @return  Void
 **/
#ifdef ANSI
PUBLIC Void rgSCHCmnSpsUlUeReset
(
RgSchCellCb         *cell,
RgSchUeCb           *ue
)
#else /* ANSI */
PUBLIC Void rgSCHCmnSpsUlUeReset(cell, ue)
RgSchCellCb         *cell;
RgSchUeCb           *ue;
#endif /* ANSI */
{

    TRC2(rgSCHCmnSpsUlUeReset);


#ifdef RGSCH_SPS_STATS
    {
       RgSchCmnUlUeSpsInfo   *ulSpsUe = RG_SCH_CMN_GET_UL_SPS_UE(ue);
       spsResetState[ulSpsUe->state]++;
    }
#endif
    rgSCHCmnSpsUlUtlRmvSpsAlloc(cell, ue);


    RETVOID;
} /* end of rgSCHCmnSpsUlUeReset */

/**
 * @brief Handler for processing SPS release for a UE
 *
 * @details
 *
 *     Function : rgSCHCmnSpsUlProcRelInd
 *
 *     Invoked By:
 *       rgSCHCmnUlSpsRelInd
 *
 *     Processing Steps:
 *       rgSCHCmnUlSpsRelInd
 *
 *  @param[in]  RgSchCellCb        *cell
 *  @param[in]  RgSchUeCb          *ue
 *  @param[in]  Bool               isExplRel
 *  @return  Void
 **/
#ifdef ANSI
PUBLIC Void rgSCHCmnSpsUlProcRelInd
(
RgSchCellCb         *cell,
RgSchUeCb           *ue,
Bool                isExplRel
)
#else /* ANSI */
PUBLIC Void rgSCHCmnSpsUlProcRelInd(cell, ue, isExplRel)
RgSchCellCb         *cell;
RgSchUeCb           *ue;
Bool                isExplRel;
#endif /* ANSI */
{
   RgSchCmnUlUeSpsInfo   *ulSpsUe = RG_SCH_CMN_GET_UL_SPS_UE(ue);

   TRC2(rgSCHCmnSpsUlProcRelInd);


   if (isExplRel == TRUE)
   {

      if ((ulSpsUe->state == RGSCH_SPS_IDLE) ||
            (ulSpsUe->state == RGSCH_SPS_TO_BE_REL) ||
            (ulSpsUe->state == RGSCH_SPS_REL_SENT))
      {
         RETVOID;
      }

#ifdef RGSCH_SPS_STATS
      rgSchSpsUlSpsExplRelTime[rgSchSpsNumExplRel % RGSCH_SPS_MAX_STATS]
         =  cell->crntTime;
      rgSchSpsNumExplRel++;
#endif

      /* Send an explicit release and when release is successful, then delete
       * the SPS information of the UE */ 
      /* Remove Ue from other lists, if it exists */
      if ((ulSpsUe->state == RGSCH_SPS_TO_BE_ACTV) ||
            (ulSpsUe->state == RGSCH_SPS_TO_BE_REACTV))
      {
         rgSCHCmnSpsUlDelUeFrmToBeActvLst(cell, ue);
      }
      rgSCHCmnSpsUlUtlRmvSpsAlloc(cell, ue);
      ulSpsUe->state = RGSCH_SPS_TO_BE_REL;
      ue->ul.explicitRelCnt=0;
      /* Filling last recieveed SPS time to detect SID*/
      RGSCHDECRFRMCRNTTIME(cell->crntTime, ulSpsUe->lastSpsDatRecvdTime,
            ((ue->ul.ulSpsCfg.implicitRelCnt+1) * ue->ul.spsPrdcty));   
      rgSCHCmnSpsUlUtlInsUeInToBeRelLst (cell, ue);
      RLOG_ARG2(L_DEBUG,DBG_CELLID,cell->cellId,
               "UL SPS Explicit Release indication recieved for UEID:%d, "
               "SpsRnti %d", 
               ue->ueId, 
               ue->spsRnti);
   }
   else
   {
#ifdef RGSCH_SPS_STATS
      rgSchSpsUlSpsImplRelTime[rgSchSpsNumImplRel % RGSCH_SPS_MAX_STATS]
         =  cell->crntTime;
      rgSchSpsNumImplRel++;
#endif

      if (ulSpsUe->state == RGSCH_SPS_IDLE)
      {
         RETVOID;
      }

      /* Filling last recieveed SPS time to detect SID*/
      RGSCHDECRFRMCRNTTIME(cell->crntTime, ulSpsUe->lastSpsDatRecvdTime,
          ((ue->ul.ulSpsCfg.implicitRelCnt) * ue->ul.spsPrdcty));   
      /* Handle implicit release of the UE */
      RLOG_ARG2(L_DEBUG,DBG_CELLID,cell->cellId,
               "UL SPS Implicit Release indication recieved for UE %d, "
               "SpsRnti %d", 
               ue->ueId, 
               ue->spsRnti);
      rgSCHCmnSpsUlUtlRmvSpsAlloc(cell, ue);
   }

   RETVOID;
} /* end of rgSCHCmnSpsUlProcRelInd */


/**
 * @brief Handler for processing SPS activation for a UE
 *
 * @details
 *
 *     Function : rgSCHCmnSpsUlProcActInd
 *
 *     Invoked By:
 *       rgSCHCmnUlSpsActInd
 *
 *     Processing Steps:
 *       rgSCHCmnUlSpsActInd
 *
 *  @param[in]  RgSchCellCb        *cell
 *  @param[in]  RgSchUeCb          *ue
 *  @return  Void
 **/
#ifdef ANSI
PUBLIC Void rgSCHCmnSpsUlProcActInd
(
RgSchCellCb         *cell,
RgSchUeCb           *ue,
U16                 spsSduSize
)
#else /* ANSI */
PUBLIC Void rgSCHCmnSpsUlProcActInd(cell, ue,spsSduSize)
RgSchCellCb         *cell;
RgSchUeCb           *ue;
U16                spsSduSize;
#endif /* ANSI */
{
   RgSchCmnUlUeSpsInfo   *ulSpsUe = RG_SCH_CMN_GET_UL_SPS_UE(ue);

   TRC2(rgSCHCmnSpsUlProcActInd);


   ulSpsUe->spsSduSize = spsSduSize;
   RETVOID;
} /* end of rgSCHCmnSpsUlProcActInd */


/**
 * @brief Handler to initialize the measurement gap mask of the UE
 *
 * @details
 *
 *     Function : rgSCHCmnSpsUlUeMeasGapMaskInit
 *
 *     Invoked by:
 *      -rgSCHCmnSpsUlUeCfg
 *        
 *     Processing Steps:
 *
 *  @param[in]  RgSchCellCb     *cell
 *  @param[in]  RgSchUeCb       *ue
 *  @param[in]  RgrUeCfg        *ueCfg
 *  @return  Void
 **/

#ifdef ANSI
PRIVATE Void rgSCHCmnSpsUlUeMeasGapMaskInit
(
RgSchCellCb        *cell,
RgSchUeCb          *ue,
RgrUeMeasGapCfg    ueMesGapCfg
)
#else /* ANSI */
PRIVATE Void rgSCHCmnSpsUlUeMeasGapMaskInit(cell, ue, ueMesGapCfg)
RgSchCellCb        *cell;
RgSchUeCb          *ue;
RgrUeMeasGapCfg    ueMesGapCfg;
#endif /* ANSI */
{
    RgSchCmnUlUeSpsInfo   *ulSpsUe = RG_SCH_CMN_GET_UL_SPS_UE(ue);
    U8                    idx;
    S8                    startPos;   

    TRC2(rgSCHCmnSpsUlUeMeasGapMaskInit);


    /* Reset the existing mask before initializing */
    for (idx=0; idx < RGSCH_SPS_MG_MASK_LEN; idx++)
    {
        ulSpsUe->measGapMask[idx] = 0;
    }

    if (TRUE == ueMesGapCfg.isMesGapEnabled)
    {
    /* Mark UL Subframes in which Measurement takes place */
    /* Calculate the starting position to be set and set n indices from there
     * starting pos =  ue's measurement offset - pdcch_pusch_gap.
     * n = no of subframes UE remains inactive in UL due to MG */
    /* We need not account for harq feedback in the mask
     * so mark only 6 subframes*/
    startPos = ueMesGapCfg.gapOffst; 
    for (idx=0; idx < (RG_SCH_MEAS_GAP_LEN + RG_SCH_CMN_HARQ_INTERVAL); idx++)
    {
        /* Divide by 8*/
        ulSpsUe->measGapMask[(((startPos + idx) % ueMesGapCfg.gapPrd)  >> 3 )] |= 
            (1 << ((startPos + idx) % 8));
    }
    }
    RETVOID;
} /* end of rgSCHCmnSpsUlUeMeasGapMaskInit */

   /**
    * @brief Handler to initialize the SPS info of a UE at config/reconfig
    *
    * @details
    *
    *     Function : rgSCHCmnSpsUlUeInit
    *
    *     Invoked by:
    *      -rgSCHCmnSpsUlUeCfg
    *      -rgSCHCmnSpsUlUeRecfg
    *        
    *     Processing Steps:
    *           return 
    *
    *  @param[in]  RgSchUeCb      *ue
    *  @return  Void
    **/

#ifdef ANSI
PRIVATE Void rgSCHCmnSpsUlUeInit
(
RgSchCellCb     *cell,
RgSchUeCb       *ue,
RgrUeSpsCfg     spsCfg,
RgrUeMeasGapCfg measGapCfg
)
#else
PRIVATE Void rgSCHCmnSpsUlUeInit(cell, ue, spsCfg, measGapCfg)
RgSchCellCb     *cell;
RgSchUeCb       *ue;
RgrUeSpsCfg     spsCfg;
RgrUeMeasGapCfg measGapCfg;
#endif
{
   RgSchCmnUlUeSpsInfo   *ulSpsUe = RG_SCH_CMN_GET_UL_SPS_UE(ue);
   Pst                   pst;
   RgInfSpsLcInfo        lcInfo;
   U16                   idx;

   TRC2(rgSCHCmnSpsUlUeInit);

   
   ue->spsRnti = spsCfg.spsRnti;
   ue->ul.ulSpsCfg = spsCfg.ulSpsCfg;
   ue->ul.spsPrdcty= spsCfg.ulSpsCfg.ulSpsPrdctyEnum;
#ifdef LTE_TDD
   /* In case of TDD, SPS periodicity shall be rounded off to an integer. For
    * ex: 128 is considered to be 120 */
   RGSCH_SPS_GET_PRDCTY(ue->ul.ulSpsCfg.ulSpsPrdctyEnum, ue->ul.spsPrdcty);
#endif /* LTE_TDD */
   /* Calculate scaled Cqi based on the current UL Cqi */
   ulSpsUe->ulSpsSchdInfo.scaledCqi = rgSCHCmnSpsScaleUlCqi(cell, ue);
   ulSpsUe->state = RGSCH_SPS_IDLE;
   ulSpsUe->actPdcchCrcFailCount = 0;
   /* Initializing last recieved SPS data time to invalid*/
   ulSpsUe->lastSpsDatRecvdTime.sfn = RGSCH_INVALID_SFN;
   ulSpsUe->lastSpsDatRecvdTime.subframe = RGSCH_INVALID_SUBFRAME;
#ifdef RGSCH_SPS_STATS
   ulSpsUe->crcPassStats = 0;
   ulSpsUe->crcFailStats = 0;
#endif
   
   if (NULLP  == rgSCHDbmGetSpsUeCb (cell, ue->spsRnti))
   {
      rgSCHDbmInsSpsUeCb(cell, ue);
   }

   if (measGapCfg.isMesGapEnabled == TRUE)
   {
      rgSCHCmnSpsUlUeMeasGapMaskInit(cell, ue, measGapCfg);
   }


   /* Register the SPS-Rnti and the set of lcIds given as SPS LCs with MAC */
   lcInfo.cellId = cell->cellId;
   lcInfo.crnti = ue->ueId;
   lcInfo.spsRnti = ue->spsRnti;
   lcInfo.spsLcCnt = ue->ul.ulSpsCfg.lcCnt;
   for (idx=0; idx<lcInfo.spsLcCnt; idx++)
   {
      if (TRUE == ue->ul.ulSpsCfg.spsLcInfo[idx].isSpsEnabled)
      {
         lcInfo.spsLcId[idx] = ue->ul.ulSpsCfg.spsLcInfo[idx].lcId;
      }
   }
   lcInfo.implRelCnt = ue->ul.ulSpsCfg.implicitRelCnt;
   lcInfo.spsPrd = ue->ul.spsPrdcty;
   rgSCHUtlGetPstToLyr(&pst, &rgSchCb[cell->instIdx], cell->macInst);
   /* code Coverage portion of the test case */
   RgSchMacSpsLcReg(&pst, &lcInfo);

   RETVOID;

} /* end of rgSCHCmnSpsUlUeInit */

/**
 * @brief Handler to initialize the UL bandwidth bitmask in SPS subframes 
 *
 * @details
 *
 *     Function : rgSCHCmnSpsUlBwMaskInit
 *
 *     Invoked by:
 *      -rgSCHCmnSpsUlUpdSpsSf
 *        
 *     Processing Steps:
 *           for each subframe in cell->spsSfLst
 *              if (non-SPS bandwidth != 0)
 *                 Mark non-SPS bandwidth bits as 1
 *           return 
 *
 *  @param[in]  RgSchCellCb     *cell
 *  @return  Void
 **/

#ifdef ANSI
PRIVATE S16 rgSCHCmnSpsUlBwMaskInit
(
RgSchCellCb     *cell,
U16             lstSz
)
#else /* ANSI */
PRIVATE S16 rgSCHCmnSpsUlBwMaskInit(cell, lstSz)
RgSchCellCb     *cell;
U16             lstSz;
#endif /* ANSI */
{
   U16                   idx;
   U16                   idx1;
   U8                    maskIdx;
   RgSchCmnSpsUlSf       *spsSf;
   RgSchCmnUlCell        *cellUl    = RG_SCH_CMN_GET_UL_CELL(cell);
   RgSchCmnUlCellSpsInfo *ulSpsCell = RG_SCH_CMN_GET_UL_SPS_CELL(cell);
   RgSchCmnSpsUlSf       *ulSpsSfLst = ulSpsCell->spsSfLst;
   U32                    initMask = 0xFFFFFFFF;
   U8 i=0;
   U8                     numSb = cell->dynCfiCb.bwInfo[cell->dynCfiCb.maxCfi].
                                                                        numSb;

   TRC2(rgSCHCmnSpsUlBwMaskInit);


   initMask = (initMask << (ulSpsCell->spsSbStart +  ulSpsCell->numSpsSb));
   for (idx=0; idx < lstSz; idx++)
   {
      spsSf = &ulSpsSfLst[idx];
      spsSf->maskLen = RGSCH_CEIL(numSb, 32);

      /* Mark the bits corr to non-SPS BW, if exists, as occupied */
      if (ulSpsCell->numSpsSb != numSb)
      {
         U8  nonSpsSbToSet;
         U8  numSpsSb;
         U32 tmpMask;

         if(!ulSpsCell->numSpsSb)
         {
            for (i=0; i < RGSCH_SPS_ULBW_MASK_LEN; i++)
            {
               spsSf->ulBwBitMask[i] = 0xffffffff;
            }
            RETVALUE(RFAILED);
         }
         nonSpsSbToSet = (numSb - ulSpsCell->numSpsSb)/2;

         /* Mark non-SPS BW at the end of the BW as occupied */
         spsSf->ulBwBitMask[0] = (U32)(((1<<nonSpsSbToSet))-1);
         if (nonSpsSbToSet > 32)
         {
            /* nonSpsSbToSet at one end of the bandwidth can't go beyond
             * 49 and hence lie within Mask[1] */
            spsSf->ulBwBitMask[1] = spsSf->ulBwBitMask[0] | 
               (U32)((1<<(nonSpsSbToSet-32))-1);
         }
         /* Mark non-SPS BW at the start of the BW as occupied */
         maskIdx = (ulSpsCell->numSpsSb + nonSpsSbToSet) / 32;
         numSpsSb = ((ulSpsCell->numSpsSb + nonSpsSbToSet)
               - (32 * maskIdx));
         tmpMask = spsSf->ulBwBitMask[maskIdx];
         tmpMask = tmpMask >> numSpsSb;
         tmpMask = tmpMask | (U32)(((1<<nonSpsSbToSet))-1);
         tmpMask = tmpMask << numSpsSb;
         spsSf->ulBwBitMask[maskIdx] = spsSf->ulBwBitMask[maskIdx]|tmpMask;
         if ((32 - numSpsSb) < nonSpsSbToSet)
         {
            spsSf->ulBwBitMask[maskIdx+1] = spsSf->ulBwBitMask[maskIdx+1]|
               (U32)((1<<(nonSpsSbToSet-(32 - numSpsSb)))-1);
         }
      } /* end of numSpsSb != numSb */ 

      /* Also mark the subbands with non-zero DMRS as occupied for SPS
       * allocations */
      for (idx1=cellUl->ulSpsInfo.spsSbStart; 
            idx1 < cellUl->ulSpsInfo.numSpsSb; idx1++)
      {
         if (cellUl->dmrsArr[idx1] != 0)
         {
            /* Set the bit corr. to the subband as 1 */
            maskIdx = idx1 >> 5;
            spsSf->ulBwBitMask[maskIdx] |= (1 << (idx1 % 32));
         }
      }
      /* Set the bits out of SPS Subband Range */
      for (i=0; i < RGSCH_SPS_ULBW_MASK_LEN; i++)
      {
         spsSf->ulBwBitMask[i] |= initMask;
      }
   }
   RETVALUE(ROK);
} /* end of rgSCHCmnSpsUlBwMaskInit */


/**
 * @brief Handler to create cell wide sps subframe list
 *
 * @details
 *
 *     Function : rgSCHCmnSpsUlInitSpsSf
 *
 *     Invoked by:
 *      -rgSCHCmnSpsUlCellCfg
 *        
 *     Processing Steps:
 *              create spsSfLst of default size
 *           return 
 *
 *  @param[in]  RgSchCellCb       *cell
 *  @return  Void
 **/

#ifdef ANSI
PRIVATE Void rgSCHCmnSpsUlInitSpsSf
(
RgSchCellCb     *cell
)
#else /* ANSI */
PRIVATE Void rgSCHCmnSpsUlInitSpsSf(cell)
RgSchCellCb     *cell;
#endif /* ANSI */
{
   RgSchCmnUlCellSpsInfo *ulSpsCell = RG_SCH_CMN_GET_UL_SPS_CELL(cell);
   RgSchCmnSpsUlSf       **ulSpsSfLst = &ulSpsCell->spsSfLst;

   TRC2(rgSCHCmnSpsUlInitSpsSf);


   /* Allocate sps sf table */
   if ((rgSCHUtlAllocSBuf (cell->instIdx, 
                         (Data **)(ulSpsSfLst), 
                         (sizeof(RgSchCmnSpsUlSf) * RGSCH_SPS_UL_LCM))) !=ROK)
   {
      RLOG_ARG0(L_ERROR,DBG_CELLID,cell->cellId,
                 "Memory allocation FAILED for ulSpsSfLst");
      RETVOID;
   }

   /* Store the new LCM as the current LCM */
   ulSpsCell->spsPrdLcmVal = RGSCH_SPS_UL_LCM;

   /* Initialize the BW bitmask in the subframes */
   if ((rgSCHCmnSpsUlBwMaskInit(cell, RGSCH_SPS_UL_LCM)) == RFAILED)
   {
      /*rgSCHUtlFreeSBuf (cell->instIdx, (Data *)(*ulSpsSfLst),
                        (sizeof(RgSchCmnSpsUlSf) * RGSCH_SPS_UL_LCM));*/
      RETVOID;
   }
   

   RETVOID;
} /* end of rgSCHCmnSpsUlInitSpsSf */

/**
 * @brief Handler to create or update the cell wide sps subframe list
 *
 * @details
 *
 *     Function : rgSCHCmnSpsUlUpdSpsSf
 *
 *     Invoked by:
 *      -rgSCHCmnSpsUlUeCfg
 *        
 *     Processing Steps:
 *           if(cell->spsSfLst == NULLP)
 *              create spsSfLst of size = Ue's periodicity
 *           else
 *              realloc spsSflst
 *              replicate the sps information across the newly allocated memory
 *           return 
 *
 *  @param[in]  RgSchUeCb       *ue
 *  @return  Void
 **/

#ifdef ANSI
PRIVATE S16 rgSCHCmnSpsUlUpdSpsSf
(
RgSchCellCb     *cell,
U16             crntLcm,
U16             newLcm
)
#else /* ANSI */
PRIVATE S16 rgSCHCmnSpsUlUpdSpsSf(cell, crntLcm, newLcm)
RgSchCellCb     *cell;
U16             crntLcm;
U16             newLcm;
#endif /* ANSI */
{
   RgSchCmnUlCellSpsInfo *ulSpsCell = RG_SCH_CMN_GET_UL_SPS_CELL(cell);
   RgSchCmnSpsUlSf       **ulSpsSfLst = &ulSpsCell->spsSfLst;
   RgSchCmnSpsUlSf       *spsSf = NULLP;
   RgSchCmnSpsUlSf       *tmpUlSpsSfLst;
   U8                    idx;

   TRC2(rgSCHCmnSpsUlUpdSpsSf);

   /* Reallocate spsSfLst to the new LCM size */
   if ((rgSCHUtlAllocSBuf (cell->instIdx, 
                            (Data **)(&tmpUlSpsSfLst), 
                            (sizeof(RgSchCmnSpsUlSf) * newLcm))) !=ROK)
   {
      RLOG_ARG0(L_ERROR,DBG_CELLID,cell->cellId,
                 "Memory reallocation FAILED for ulSpsSfLst");
      RETVALUE(RFAILED);
   }
   /* Replicate the sps subframe information upto newLcm */
   for(idx=0; idx < (newLcm/crntLcm); idx++)
   {
      spsSf = &tmpUlSpsSfLst[(idx * crntLcm)];
      cmMemcpy((U8 *)spsSf, (U8 *)(*ulSpsSfLst),
                                 (sizeof(RgSchCmnSpsUlSf) * crntLcm));
   }
   /* Deallocate old spsSf list and assign the newl created list */
   /* ccpu00117052 - MOD - Passing double pointer
   for proper NULLP assignment*/
   rgSCHUtlFreeSBuf(cell->instIdx, (Data **)(ulSpsSfLst),
                              (sizeof(RgSchCmnSpsUlSf) * crntLcm));
   *ulSpsSfLst = tmpUlSpsSfLst;

   RETVALUE(ROK);
} /* end of rgSCHCmnSpsUlUpdSpsSf */

/**
 * @brief Function to handle BSR report 
 *
 * @details
 *
 *     Function: rgSCHCmnSpsBsrRpt
 *
 *     Handler for processing BSR reported by UE 
 *
 *     Invoked by: 
 *         - rgSCHCmnUpdBsrShort
 *         - rgSCHCmnUpdBsrTrunc
 *         - rgSCHCmnUpdBsrLong 
 *       
 *
 *     Processing Steps: 
 *         if(ulLcg->lcgId == RG_SCH_SPS_LCG_ID) && (ue->isUlSpsEnabled == TRUE)
 *            if(ue->isUlSpsActv == FALSE)
 *               add to the "tobeActvtd" SPS UE List
 *         else
 *            return
 *           
 *  @param[in] RgSchCellCb  *cell
 *  @param[in] RgSchUeCb    *ue
 *  @param[in] RgSchLcgCb   *ulLcg
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED 
 **/
#ifdef ANSI
PUBLIC S16 rgSCHCmnSpsBsrRpt
(
RgSchCellCb  *cell,
RgSchUeCb    *ue,
RgSchLcgCb   *ulLcg
)
#else /* ANSI */
PUBLIC S16 rgSCHCmnSpsBsrRpt(cell, ue, ulLcg)
RgSchCellCb  *cell;
RgSchUeCb    *ue;
RgSchLcgCb   *ulLcg;
#endif /* ANSI */
{
   RgSchCmnUlUeSpsInfo   *ulSpsUe = RG_SCH_CMN_GET_UL_SPS_UE(ue);
   RgSchCmnUlUe          *ueUl = RG_SCH_CMN_GET_UL_UE(ue);
   U32 bufferSize;

   TRC2(rgSCHCmnSpsBsrRpt);


     
   if (ulLcg->lcgId != RG_SCH_SPS_LCG_ID)
   {
      /* need not do anything */
      RETVALUE(ROK);
   }
   else
   {
      bufferSize= ((RgSchCmnLcg *)(ulLcg->sch))->bs;
#ifdef RGSCH_SPS_STATS
      if (bufferSize)
      {
         ulSpsBsSizeStats[ulSpsBsSizeCnt++ % RGSCH_SPS_MAX_BSR_STATS] = 
            bufferSize;
         ulSpsUe->totalBsr += bufferSize;
         ulSpsUe->numBsrRpt++;
         rgSchSpsStatTotalBsr += bufferSize;
         rgSchSpsStatNumBsrRpt++;
      }
#endif
      /*
      if((bufferSize > RG_SCH_CMN_SID_PACKET_SIZE) &&
            (bufferSize < RGSCH_SPS_MAX_SPS_ACTVN_BO)) 
         */
      if(bufferSize > RG_SCH_CMN_SID_PACKET_SIZE)
      {
#ifdef RGSCH_SPS_STATS
         if (ulSpsUe->state < 7)
         {
            spsBsrUpdState[ulSpsUe->state]++;
         }
#endif
         if ((ulSpsUe->state == RGSCH_SPS_IDLE) &&
               ((bufferSize < RGSCH_SPS_MAX_SPS_ACTVN_BO)))
         {
            /* Add to toBeActvtd Q, if not already added, if SPS is not yet activated */
            ulSpsUe->state = RGSCH_SPS_TO_BE_ACTV;
            ulSpsUe->packetSize          = bufferSize + RG_SCH_CMN_SPS_BSR_HEADROOM;
            /* add UE to the "tobeActvtd" SPS UE List */
            rgSCHCmnSpsUlUtlInsUeInToBeActvLst (cell, ue);
         }
         else if ((ulSpsUe->state == RGSCH_SPS_ACTV) ||
               (ulSpsUe->state == RGSCH_SPS_TO_BE_REACTV))
         {
            /* Reactivatin the SPS if the new BSR reported is greater than the 
             * previous*/
            if (((bufferSize + ulSpsUe->spsSduSize)> ulSpsUe->spsGrantSize) &&
                  ((bufferSize +ulSpsUe->spsSduSize)>= RGSCH_SPS_MAX_SPS_ACTVN_BO))
            {
               /*
               ueUl->totalBsr -= ulSpsUe->spsGrantSize;

               if (((RgSchCmnLcg *)(ulLcg->sch))->bs > ulSpsUe->spsGrantSize)
               {
                  ((RgSchCmnLcg *)(ulLcg->sch))->bs -= ulSpsUe->spsGrantSize;
               }
               else
               {
                  ((RgSchCmnLcg *)(ulLcg->sch))->bs = 0;
               } */
               rgSCHCmnSpsUlProcRelInd(cell, ue, TRUE);
            }
            else if (((bufferSize +  ulSpsUe->spsSduSize) > ulSpsUe->spsGrantSize) &&
                  ((bufferSize +  ulSpsUe->spsSduSize) < RGSCH_SPS_MAX_SPS_ACTVN_BO))
            {
               ulSpsUe->state = RGSCH_SPS_TO_BE_REACTV;
               ulSpsUe->packetSize =  ulSpsUe->spsSduSize + bufferSize + RG_SCH_CMN_SPS_BSR_HEADROOM;
            }
            else
            {
               ueUl->totalBsr -= bufferSize;
               ((RgSchCmnLcg *)(ulLcg->sch))->bs = 0;
            }
         }
      }
   }
   ulSpsUe->spsSduSize = 0;
   RETVALUE(ROK);
} /* end of rgSCHCmnSpsBsrRpt */

/**
 * @brief Handler to process Uplink CQI Indication for a UL-SPS UE
 *
 * @details
 *
 *     Function : rgSCHCmnSpsScaleUlCqi
 *
 *     Invoked by:
 *      -rgSCHCmnSpsUlCqiInd
 *      -rgSCHCmnSpsUlUeCfg 
 *        
 *     Processing Steps:
 *           ue->scaledCqi = cqi - RG_SCH_SPS_CQI_SCALE_FACTOR
 *           return 
 *
 *  @param[in]  RgSchUeCb       *ue
 *  @param[in]  U8              cqi
 *  @return  Void
 **/

#ifdef ANSI
PRIVATE U8 rgSCHCmnSpsScaleUlCqi
(
RgSchCellCb          *cell,
RgSchUeCb            *ue
)
#else /* ANSI */
PRIVATE U8 rgSCHCmnSpsScaleUlCqi(cell, ue)
RgSchCellCb          *cell;
RgSchUeCb            *ue;
#endif /* ANSI */
{
  RgSchCmnUlUe      *ueUl = RG_SCH_CMN_GET_UL_UE(ue);
  U8                cqi;

  TRC2(rgSCHCmnSpsScaleUlCqi);
#ifdef TFU_UPGRADE
  cqi = ueUl->validUlCqi;
#else
  cqi = ueUl->crntUlCqi[0];
#endif
   /* If CQI is between 1 to 5 : Scaled CQI is 1 */                                                                                                                          
   if (!cqi)
   {
      RETVALUE(1);
   }
   if (cqi <= 5)
   {
      RETVALUE(cqi==1?1:cqi-1);
   }
   else if (cqi <= 7)
   {
      /* If CQI is between 5 to 10 : Scaled CQI is 5 */
      RETVALUE(5);
   }
   else if (cqi <= 10)
   {
      /* If CQI is between 5 to 10 : Scaled CQI is 5 */
      RETVALUE(7);
   }
   else
   { 
      /* If CQI is between 10 to 15 : Scaled CQI is 10 */
      RETVALUE(10);
   }
} /* end of rgSCHCmnSpsScaleUlCqi */

/**
 * @brief Handler to calculate the number of subbands required for given CQI and
 *     packet size
 *
 * @details
 *
 *     Function : rgSCHCmnSpsUlCalcSbReqd
 *
 *     Invoked by:
 *      -rgSCHCmnSpsUlCqiInd
 *      -rgSCHCmnSpsUlSbAllocForUe
 *        
 *     Processing Steps:
 *       Calculate the number of subbands required for UE taking the scaled CQI
 *       into consideration  
 *
 *  @param[in]  RgSchUeCb       *ue
 *  @param[in]  U8              cqi
 *  @return  Void
 **/

#ifdef ANSI
PRIVATE U8 rgSCHCmnSpsUlCalcSbReqd
(
RgSchCellCb          *cell,
RgSchUeCb            *ue,
U8                   cqi,
U8                   *iMcs,
U8                   *iTbs
)
#else /* ANSI */
PRIVATE U8 rgSCHCmnSpsUlCalcSbReqd(cell, ue, cqi, iMcs, iTbs)
RgSchCellCb          *cell;
RgSchUeCb            *ue;
U8                   cqi;
U8                   *iMcs;
U8                   *iTbs;
#endif /* ANSI */
{
   RgSchCmnUlCell    *cellUl = RG_SCH_CMN_GET_UL_CELL(cell);
   RgSchCmnUlUeSpsInfo   *ulSpsUe = RG_SCH_CMN_GET_UL_SPS_UE(ue);
   U32               bits;
   U32               nPrb;
   U8                sbSize  = cellUl->sbSize;
   U8                maxRb;
   U32               eff;
   U8                numSb;
   U32               iTbsLa;
   Bool              limit = FALSE;
   CmLteUeCategory   ueCtg = (CmLteUeCategory) (RG_SCH_CMN_GET_UE_CTGY(ue));

   TRC2(rgSCHCmnSpsUlCalcSbReqd);


   *iTbs  = rgSCHCmnSpsUlGetITbs(ue, cell->isCpUlExtend);
   iTbsLa = rgSCHCmnUlGetITbs(ue, cell->isCpUlExtend);

   if (iTbsLa < *iTbs)
   {
      *iTbs = iTbsLa;
   }
   bits = ulSpsUe->packetSize * 8;
   *iMcs  = rgSCHCmnUlGetIMcsFrmITbs(*iTbs,ueCtg);

   /* Limit max Imcs to 15 */ 
   if ( *iMcs > RG_SCH_CMN_SPS_DL_MAX_MCS)
   {
      /* Set iMcs to 15 and iTbs to 14 as per Table 8.6.1-1, 36.213*/
      *iMcs=15;
      *iTbs = 14;
   }

   /* SPS_FIX */
   if (!cell->isCpUlExtend)
   {
      eff   = rgSchCmnNorUlEff[0][*iTbs]; 
   }
   else
   {
      eff   = rgSchCmnExtUlEff[0][*iTbs]; 
   }

   maxRb = rgSCHPwrGetMaxUlRb(cell, ue);

   if (bits < rgSCHCmnUlMinTbBitsForITbs(cellUl, *iTbs))
   {
      if (!maxRb)
      {
         limit = TRUE;
         numSb = 0;
      }
      else 
      {
         numSb = 1;
      }
   }
   else
   {
      nPrb = RGSCH_CEIL((RGSCH_CEIL(bits * 1024, eff)),
            RG_SCH_CMN_UL_NUM_RE_PER_RB(cellUl));
      if (nPrb > maxRb)
      {
         limit = TRUE;
         nPrb = maxRb;
      }
      numSb = RGSCH_CEIL(nPrb, sbSize);
   }
   if (!numSb)
   {
      if (limit)
      {
         RETVALUE(numSb);
      }
      else
      {
         ++numSb;
      }
   }

   RETVALUE(numSb);
} /* end of rgSCHCmnSpsUlCalcSbReqd */

/**
 * @brief Handler to process Uplink CQI Indication for a UL-SPS UE
 *
 * @details
 *
 *     Function : rgSCHCmnSpsUlCqiInd
 *
 *     Invoked by:
 *      rgSCHCmnUlCqiInd when (ue->isUlSpsEnabled == TRUE)
 *        
 *     Processing Steps:
 *       if (ue->isUlSpsActv == FALSE),
 *           invoke rgSCHCmnSpsScaleUlCqi(ulCqi)
 *           return ROK
 *       else
 *           newScaledCqi = rgSCHCmnSpsScaleUlCqi(ulCqi)
 *           calculate number of RBs needed at ue->newScaledCqi for the
 *           configured packet size.
 *             if rbsReqd > ue->rbsAllocd 
 *                ue->sbsReqd = rbsReqd/sbSize
 *                change ue state to RGSCH_SPS_TO_BE_REACTV
 *             else do nothing
 *
 *  Notes: CQI is assumed to be updated in ue before invoking this function
 *
 *  @param[in]  RgSchCellCb     *cell
 *  @param[in]  RgSchUeCb       *ue
 *  @return  Void
 **/

#ifdef ANSI
PUBLIC S16 rgSCHCmnSpsUlCqiInd
(
RgSchCellCb          *cell,
RgSchUeCb            *ue
)
#else /* ANSI */
PUBLIC S16 rgSCHCmnSpsUlCqiInd(cell, ue)
RgSchCellCb          *cell;
RgSchUeCb            *ue;
#endif /* ANSI */
{
   RgSchCmnUlUeSpsInfo   *ulSpsUe = RG_SCH_CMN_GET_UL_SPS_UE(ue);
   RgSchCmnUlCell        *cellUl = RG_SCH_CMN_GET_UL_CELL(cell);
   U8                    iMcs;
   U8                    iTbs;
   U8                    newSb;
   
   TRC2(rgSCHCmnSpsUlCqiInd);


   /* Scale CQI and store irrespective of whether UE is SPS active or not */
   ulSpsUe->ulSpsSchdInfo.scaledCqi = rgSCHCmnSpsScaleUlCqi(cell, ue);

   if (ulSpsUe->state == RGSCH_SPS_ACTV)
   {

      /* Calculate the new SBs required and add it to reactvUeLst if needed */
      newSb = rgSCHCmnSpsUlCalcSbReqd(cell, ue, 
            ulSpsUe->ulSpsSchdInfo.scaledCqi, &iMcs, &iTbs);
      if ((newSb != ulSpsUe->ulSpsSchdInfo.allocInfo.numSb) ||
            (iMcs < ulSpsUe->ulSpsSchdInfo.allocInfo.grnt.iMcs))
      {
         /* restricting NUM SB to max SPS BW */
         if ((cellUl->sbSize * newSb) > cell->spsCellCfg.maxSpsDlBw)
         {
            rgSCHCmnSpsUlProcRelInd(cell, ue, TRUE);
         }
         else 
         {
            /* change the state of the UE to "TO_BE_REACTV" so that reactivation
             * can be attempted in frequency domain */
            ulSpsUe->state = RGSCH_SPS_TO_BE_REACTV;
            RLOG_ARG4(L_DEBUG,DBG_CELLID,cell->cellId,
                     "UL SPS needs to be Re-Activated as ULCQI is changed for"
                     " UEID:%d, SPS-RNTI %d, already allocated subband %d, newly required"
                     " subband %d ", 
                     ue->ueId, 
                     ue->spsRnti, 
                     ulSpsUe->ulSpsSchdInfo.allocInfo.numSb, 
                     newSb);
         }
      } 
   }
   
   RETVALUE(ROK);
} /* end of rgSCHCmnSpsUlCqiInd */

/**
 * @brief Handler to process the DTX indication received for a SPS UE
 *
 * @details
 *
 *     Function : rgSCHCmnSpsUlProcDtxInd
 *
 *     Invoked by:
 *       rgSCHCmnUlCrcFailInd for a UE whose UL SPS is enabled
 *        
 *     Processing Steps:
 *       if ((ue->isUlSpsActv == FALSE) || (dtxTime != ue->actvUlSpsLstIdx))
 *          return ROK;
 *       else if (ue->ulSpsInfo.state == RGSCH_ACTV_REACTV_SENT)
 *          set ue->pdcchToRetx = TRUE;
 *         
 *
 *  @param[in]  RgSchCellCb        *cell
 *  @param[in]  RgSchUeCb          *ue
 *  @param[in]  CmLteTimingInfo    dtxTime
 *  @return  S16
 *
 *  Notes: Right now, dtxInd is invoked on receiving CRC failure for UL data.
 *         Can also be invoked from DtxInd, if introduced on TFU.
 **/

#ifdef ANSI
PUBLIC S16 rgSCHCmnSpsUlProcDtxInd
(
RgSchCellCb          *cell,
RgSchUeCb            *ue,
CmLteTimingInfo      dtxTime
)
#else /* ANSI */
PUBLIC S16 rgSCHCmnSpsUlProcDtxInd(cell, ue, dtxTime)
RgSchCellCb          *cell;
RgSchUeCb            *ue;
CmLteTimingInfo      dtxTime;
#endif /* ANSI */
{
   RgSchCmnUlUeSpsInfo   *ulSpsUe = RG_SCH_CMN_GET_UL_SPS_UE(ue);
   RgSchUlHqProcCb   *ulHqProc = NULLP;

   TRC2(rgSCHCmnSpsUlProcDtxInd);



   rgSCHUtlUlHqProcForUe(cell, dtxTime, ue, &ulHqProc);

#ifdef RGSCH_SPS_STATS
   if (ulHqProc && (ulHqProc->isSpsActvnHqP == TRUE))
   {
       crcFailActCountStat++;
   }
#endif
   if (ulHqProc && (ulHqProc->isSpsOccnHqP == TRUE))
   { 


#ifdef RGSCH_SPS_STATS
         crcFailOccCountStat++;
#endif

      if ((ulSpsUe->state == RGSCH_SPS_ACTV) ||
               (ulSpsUe->state == RGSCH_SPS_TO_BE_REACTV))
      {
         /* checking first TX success */
         if(ue->ul.hqEnt.maxHqRetx == ulHqProc->remTx)
         {
            ulSpsUe->crcFailCntSpsOcc++;
         }

#ifdef RGSCH_SPS_STATS
         ulSpsUe->crcFailStats++;
#endif

         if (ulSpsUe->crcFailCntSpsOcc >= RGSCH_SPS_MAX_UL_SPS_OCC_CRC_FAIL)
         {
            /* CRC Fail is received RGSCH_SPS_MAX_UL_SPS_OCC_CRC_FAIL 
             * number of consecutive occasions then relese UL
             * SPS explicitly */
            rgSCHCmnSpsUlProcRelInd(cell, ue, TRUE);

#ifdef RGSCH_SPS_STATS
            crcFailExplRelStat++;
#endif
         }
      }
      else if (ulSpsUe->state == RGSCH_SPS_REL_SENT)
      {
         ue->ul.explicitRelCnt++;
         if( ue->ul.explicitRelCnt == RG_SCH_NUM_SPS_OCC_AFTR_EXP_REL)
         {
            /* Explicit release is successful. Release SPS resources */
            rgSCHCmnSpsUlUtlRmvSpsAlloc(cell, ue);
            ue->ul.explicitRelCnt = 0;

#ifdef RGSCH_SPS_STATS
            ulExplRelSucc++;
#endif
         }
      }
   }

   RETVALUE(ROK);
} /* end of rgSCHCmnSpsUlProcDtxInd */

/**
 * @brief Handler to process the CRC indication received for a SPS UE
 *
 * @details
 *
 *     Function : rgSCHCmnSpsUlProcCrcInd
 *
 *     Invoked by:
 *       rgSCHCmnUlCrcFailInd for a UE whose UL SPS is enabled
 *        
 *     Processing Steps:
 *       if ((ue->isUlSpsActv == FALSE) || (dtxTime != ue->actvUlSpsLstIdx))
 *          return ROK;
 *       else if (ue->ulSpsInfo.state == RGSCH_SPS_REL_SENT)
 *          Add Ue to TobeRel List
 *         
 *
 *  @param[in]  RgSchCellCb        *cell
 *  @param[in]  RgSchUeCb          *ue
 *  @param[in]  CmLteTimingInfo    dtxTime
 *  @return  S16
 *
 **/

#ifdef ANSI
PUBLIC S16 rgSCHCmnSpsUlProcCrcInd
(
RgSchCellCb          *cell,
RgSchUeCb            *ue,
CmLteTimingInfo      crcTime
)
#else /* ANSI */
PUBLIC S16 rgSCHCmnSpsUlProcCrcInd(cell, ue, crcTime)
RgSchCellCb          *cell;
RgSchUeCb            *ue;
CmLteTimingInfo      crcTime;
#endif /* ANSI */
{
   RgSchCmnUlUeSpsInfo   *ulSpsUe = RG_SCH_CMN_GET_UL_SPS_UE(ue);
   RgSchUlHqProcCb   *ulHqProc = NULLP;

   TRC2(rgSCHCmnSpsUlProcCrcInd);

   
   rgSCHUtlUlHqProcForUe(cell, crcTime, ue, &ulHqProc);
   /* If CRC success is received after ACTVN PDCCH sent 
      then change UE state to UL SPS ACTIVE */
   if (ulHqProc && (ulHqProc->isSpsActvnHqP == TRUE))
   {
      ulSpsUe->state = RGSCH_SPS_ACTV;
      ulSpsUe->crcFailCntSpsOcc = 0;
#ifdef RGSCH_SPS_STATS
      crcPassCount++;
      rgSchSpsNumActCrcPass++;
#endif

      RLOG_ARG3(L_DEBUG,DBG_CELLID,cell->cellId,
               "UL SPS ACTIVTD succesfully for UEID:%d crnttime %d:%d",
               ue->ueId,
               cell->crntTime.sfn,
               cell->crntTime.subframe);
   }   
   else if (ulHqProc && (ulHqProc->isSpsOccnHqP == TRUE))
   {
      if (ulSpsUe->state == RGSCH_SPS_ACTV)
      {
         /* Reset on the successful first tx of SPS occ */
         if(ue->ul.hqEnt.maxHqRetx == ulHqProc->remTx)
         {
            ulSpsUe->crcFailCntSpsOcc = 0;
         }
      }

#ifdef RGSCH_SPS_STATS
      ulSpsUe->crcPassStats++;
      crcPassCount++;
#endif
   }

   RETVALUE(ROK);
} /* end of rgSCHCmnSpsUlProcCrcInd */

/**
 * @brief Handler to get the next nth SPS occasion's subframe
 *
 * @details
 *
 *     Function : rgSCHCmnSpsGetNextNthSpsUlSf
 *
 *     Invoked by:
 *        rgSCHCmnSpsUlSbAlloc
 *        rgSCHCmnUlSpsMarkAlloc
 *        
 *     Processing Steps:
#ifdef LTE_TDD
 *           if(ue->twoIntervalCfg == TRUE)
 *              sfOffset = rgSchTddSfOffTbl[cell->UlDlCfgIdx][(cell->crntTime +
 *              UL_SCHD_DELTA)]
 *              futureSpsUlSf = spsSfLst[(actvOffst + (prsntidx * ue->spsPrdcty)+
 *                                          sfOffset) % cell->spsPrdLcmVal];
 *           else
 *              futureSpsUlSf = spsSfLst[(actvOffst + (prsntidx * ue->spsPrdcty)) %
 *                                                       cell->spsPrdLcmVal];
#else
 *           futureSpsUlSf = spsSfLst[(actvOffst + (prsntidx * ue->spsPrdcty)) %
 *                                                       cell->spsPrdLcmVal]
#endif
 *          
 *  @param[in]  RgSchCellCb   *cell
 *  @param[in]  RgSchUeCb     *ue
 *  @param[in]  CmLteTimingInfo ulSpsActvOcc
 *  @param[in]  U8            prsntidx
 *  @param[out] CmLteTimingInfo  *nxtOcc
 *  @return  RgSchCmnSpsUlSf*
 **/

#ifdef ANSI
PRIVATE RgSchCmnSpsUlSf* rgSCHCmnSpsGetNextNthSpsUlSf
(
RgSchCellCb     *cell,
RgSchUeCb       *ue,
CmLteTimingInfo ulSpsActvOcc,
U8              prsntidx,
CmLteTimingInfo  *nxtOcc
)
#else /* ANSI */
PRIVATE RgSchCmnSpsUlSf* rgSCHCmnSpsGetNextNthSpsUlSf(cell, ue, ulSpsActvOcc, prsntidx, nxtOcc)
RgSchCellCb     *cell;
RgSchUeCb       *ue;
CmLteTimingInfo ulSpsActvOcc;
U8              prsntidx;
CmLteTimingInfo  *nxtOcc;
#endif /* ANSI */
{
   RgSchCmnUlCellSpsInfo *ulSpsCell = RG_SCH_CMN_GET_UL_SPS_CELL(cell);
   RgSchCmnSpsUlSf       *futSpsSf  = NULLP;
   U32                    actvOffst;
   U16 nxtOffst;

   TRC2(rgSCHCmnSpsGetNextNthSpsUlSf);

   actvOffst = (ulSpsActvOcc.sfn*RGSCH_NUM_SUB_FRAMES) + ulSpsActvOcc.subframe;

#ifdef LTE_TDD
   if(ue->ul.ulSpsCfg.twoIntervalCfg == TRUE)
   {
      S8                sfOffset;
      U8                nMod2 = (prsntidx%2);

      sfOffset = rgSchTddSfOffTbl[cell->ulDlCfgIdx][ulSpsActvOcc.subframe];
      futSpsSf = &(ulSpsCell->spsSfLst[(actvOffst + 
                     (prsntidx * ue->ul.spsPrdcty)+ (sfOffset * nMod2)) %
                                            ulSpsCell->spsPrdLcmVal]);
      nxtOffst = (actvOffst + 
                     (prsntidx * ue->ul.spsPrdcty)+ (sfOffset * nMod2));
   }
   else
#endif /*LTE_TDD*/
   {
      futSpsSf = &(ulSpsCell->spsSfLst[(actvOffst + 
                     (prsntidx * ue->ul.spsPrdcty)) % ulSpsCell->spsPrdLcmVal]);
      nxtOffst = (actvOffst + (prsntidx * ue->ul.spsPrdcty));
   }
   nxtOcc->sfn = (nxtOffst / RGSCH_NUM_SUB_FRAMES);
   nxtOcc->subframe = (nxtOffst % RGSCH_NUM_SUB_FRAMES);
   
   RETVALUE(futSpsSf);
} /* end of rgSCHCmnSpsGetNextNthSpsUlSf */


/**
 * @brief Returns the "Itbs" for a given UE.
 *
 * @details
 *
 *     Function: rgSCHCmnSpsUlGetITbs
 *     Purpose:  This function returns the "Itbs" for a given UE.
 *
 *     Invoked by: Scheduler
 *     
 *  @param[in]  RgSchUeCb        *ue
 *  @return     U8 
 **/
#ifdef ANSI
PRIVATE U8 rgSCHCmnSpsUlGetITbs 
(
RgSchUeCb        *ue,
Bool             isEcp
)
#else /* ANSI */
PRIVATE U8 rgSCHCmnSpsUlGetITbs(ue, isEcp)
RgSchUeCb        *ue;
Bool             isEcp;
#endif /* ANSI */
{
   RgSchCmnUlUe *ueUl    = RG_SCH_CMN_GET_UL_UE(ue);
   U8               cqi    = 0;
   CmLteUeCategory  ueCtgy = (CmLteUeCategory) (RG_SCH_CMN_GET_UE_CTGY(ue));

   TRC2(rgSCHCmnSpsUlGetITbs);

   if ( (ueCtgy != CM_LTE_UE_CAT_5) && 
        (ueUl->ulSpsInfo.ulSpsSchdInfo.scaledCqi > ueUl->maxUlCqi)
      )
   {
      cqi = ueUl->maxUlCqi;
   }
   else
   {
      cqi = ueUl->ulSpsInfo.ulSpsSchdInfo.scaledCqi;
   }

   /* Consider the scaled CQI in case of SPS activated UEs */
   RETVALUE(rgSchCmnUlCqiToTbsTbl[(U8)isEcp][cqi]);
} /* end of rgSchCmnSpsUlGetITbs */

/**
 * @brief Handler for SPS SB allocation
 *
 * @details
 *
 *     Function : rgSCHCmnSpsSbAlloc
 *
 *     Invoked by:
 *      - rgSCHCmnSpsUlSbAllocForUe 
 *        
 *     Processing Steps:
 *        fetch the spsUlSf using actvOffst as index into spsSfLst
 *        allocMask = spsUlSf->ulBwBitMask
 *        for (i=0; i <  ((spsPrdLcmVal/ue->spsPrdcty) -1); i++)
 *           futureSpsUlSf = rgSCHCmnSpsGetNextNthSpsUlSf (ue, actvOffst, i);
 *           allocMask = allocMask|futureSpsUlSf->ulBwbitMask 
 *        fetch an allocation that can accommodate UE's subband requirement
 *        using allocMask
 *        return allocation
 *          
 *  @param[in]  RgSchCellCb   *cell
 *  @param[in]  RgSchUeCb     *ue
 *  @param[in]  U8            actvOffst
 *  @return  S16
 **/

#ifdef ANSI
PRIVATE RgSchUlAlloc* rgSCHCmnSpsUlSbAlloc
(
RgSchCellCb           *cell,
RgSchUeCb             *ue,
RgSchUlSf             *sf,
U8                    numSb,
CmLteTimingInfo       timeToTx
)
#else /* ANSI */
PRIVATE RgSchUlAlloc* rgSCHCmnSpsUlSbAlloc(cell, ue, sf, numSb, timeToTx)
RgSchCellCb           *cell;
RgSchUeCb             *ue;
RgSchUlSf             *sf;
U8                    numSb;
CmLteTimingInfo       timeToTx;
#endif /* ANSI */
{
   RgSchCmnUlUe           *ueUl = RG_SCH_CMN_GET_UL_UE(ue);
   RgSchCmnUlCellSpsInfo  *ulSpsCell = RG_SCH_CMN_GET_UL_SPS_CELL(cell);
   RgSchCmnSpsUlSf        *spsSf = NULLP;
   U32                    allocMask[4] = {0}; /* to store the
                                       previous allocations across sps Sfs */
   U32                    tmpMask;
   U8                     idx;
   U8                     idx1;
   U8                     startSb = ulSpsCell->spsSbStart;
   U8                     numAvlSb=0;
   U16                    spsPrdcty = (U16)ue->ul.spsPrdcty;
   Bool                   bwAvl = FALSE;
   RgSchUlAlloc           *alloc = NULLP;
   CmLteTimingInfo       nxtOcc;
   RgSchCmnDlCell         *cellDl = RG_SCH_CMN_GET_DL_CELL(cell);
   U8                     cfi = cellDl->currCfi; 

   TRC2(rgSCHCmnSpsUlSbAlloc);
  

 
   /* Or the BW bitmasks of UE's SPS occasions */
   for (idx=0; idx < (ulSpsCell->spsPrdLcmVal/spsPrdcty); idx++)
   {
      spsSf = rgSCHCmnSpsGetNextNthSpsUlSf(cell, ue, 
            timeToTx, idx, &nxtOcc);
      for(idx1=0; idx1<spsSf->maskLen; idx1++)
      {
         allocMask[idx1] = allocMask[idx1]|spsSf->ulBwBitMask[idx1];
      }
   }

   if (spsSf)
   {
      /* Fetch an allocation based on allocMask now */
      for (idx=0; idx < spsSf->maskLen; idx++)
      {
         tmpMask = allocMask[idx];
         for (idx1=0; idx1<32; idx1++)
         {
            if ((tmpMask & 0x01) == 0)
            {
               if (numAvlSb == 0)
                  startSb = ((idx << 5) + idx1);
               numAvlSb++;
               if (numAvlSb >= numSb)
               {
                  bwAvl = TRUE;
                  break;
               }
            }
            else
            {
               numAvlSb = 0;
            }
            tmpMask = tmpMask >> 1;            
         }
      }

      if (bwAvl == TRUE)
      {
         /* Check for ongoing cfi change */
         if (cellDl->currCfi != cellDl->newCfi) /* CFI is going to change */
         {
            if (rgSCHCmnSpsUlChkUlSf(cell, sf) == TRUE)
            {
               RgSchCmnUlUeSpsInfo *ulSpsUe = RG_SCH_CMN_GET_UL_SPS_UE(ue);
               ulSpsUe->state = RGSCH_SPS_TO_BE_REACTV;
               cfi = cellDl->newCfi;
            }
         }

         /* get the alloc as numSbs are available */
         if((alloc = rgSCHCmnSpsUtlUlGetSpfcAlloc(cell, ueUl, sf, startSb, 
                                       numSb, cfi)) != NULLP)
         {
#ifdef RGSCH_SPS_STATS
            ulSpsOccSched++;
#endif
            /* Mark the allocation in Ue's allocBwMask */
            for (idx=0; idx < RGSCH_SPS_ULBW_MASK_LEN; idx++)
            {
               /* Reset before marking. Needed if it comes for reactv here */
               ueUl->ulSpsInfo.ulSpsSchdInfo.allocBwMask[idx] = 0;
            }

            for(idx=0; idx < numSb; idx++)
            {
               ueUl->ulSpsInfo.ulSpsSchdInfo.allocBwMask[((startSb+idx)>>5)] |=
                  (1 << ((startSb+idx) % 32));
            }
         }

         RETVALUE(alloc);
      } 
   }
   RETVALUE(NULLP);
} /* end of rgSCHCmnSpsUlSbAlloc */


/**
 * @brief Handler to control Num UL SPS Activation Per SF
 *
 * @details
 *
 *     Function : rgSCHCmnChkMaxSpsUePerSf
 *
 *     Invoked by:
 *      - rgSCHCmnSpsUlSbAllocForUe 
 *        
 *     Processing Steps:
 *        fetch the spsUlSf using actvOffst as index into spsSfLst
 *        allocMask = spsUlSf->ulBwBitMask
 *        for (i=0; i <  ((spsPrdLcmVal/ue->spsPrdcty) -1); i++)
 *           futureSpsUlSf = rgSCHCmnSpsGetNextNthSpsUlSf (ue, actvOffst, i);
 *           If numSpsActive Ues ==  Max SPS UEs Per SF
 *               Return TRUE
 *           Else
 *               Return FALSE
 *          
 *  @param[in]  RgSchCellCb   *cell
 *  @param[in]  RgSchUeCb     *ue
 *  @param[in]  CmLteTimingInfo timeToTx
 *  @return  Bool
 **/

#ifdef ANSI
PRIVATE Bool rgSCHCmnChkMaxSpsUePerSf
(
RgSchCellCb           *cell,
RgSchUeCb             *ue,
CmLteTimingInfo       timeToTx
)
#else /* ANSI */
PRIVATE Bool rgSCHCmnChkMaxSpsUePerSf(cell, ue,timeToTx)
RgSchCellCb           *cell;
RgSchUeCb             *ue;
CmLteTimingInfo       timeToTx;
#endif /* ANSI */
{
   RgSchCmnUlCellSpsInfo  *ulSpsCell = RG_SCH_CMN_GET_UL_SPS_CELL(cell);
   RgSchCmnSpsUlSf        *spsSf = NULLP;
   U8                     idx;
   U16                    spsPrdcty = (U16)ue->ul.spsPrdcty;
   CmLteTimingInfo       nxtOcc;

   TRC2(rgSCHCmnChkMaxSpsUePerSf);
  

   
   for (idx=0; idx < (ulSpsCell->spsPrdLcmVal/spsPrdcty); idx++)
   {
      spsSf = rgSCHCmnSpsGetNextNthSpsUlSf(cell, ue, 
                    timeToTx, idx, &nxtOcc);
      if (spsSf->numUlSpsActiveUes == cell->spsCellCfg.maxSpsUePerUlSf)
      {
         RETVALUE(TRUE);
      }
   }
   RETVALUE(FALSE);
} /* end of rgSCHCmnChkMaxSpsUePerSf */


/**
 * @brief Handler for SPS RB allocation
 *
 * @details
 *
 *     Function : rgSCHCmnSpsUlSbAllocForUe
 *
 *     Invoked by:
 *      - rgSCHCmnSpsUlProcToBeActvtdUe when a UE needs to be
 *        activated/reactivated. This shall pick the subbands based on the UE's
 *        requirement and the available bandwidth in the considered subframe.
 *        This shall also take care of the collision in the future subframes
 *        based on the UE's periodicity.
 *        
 *     Processing Steps:
 *        fetch the spsUlSf using actvOffst as index into spsSfLst
 *        fetch a hole that can accommodate UE's subband requirement
 *        if (hole == NULLP)
 *           return RFAILED
 *        set as many LSBs as the number of subbands allocated in a temporary U32
 *        for (i=0; i <  ((spsPrdLcmVal/ue->spsPrdcty) -1); i++)
 *           futureSpsUlSf = rgSCHCmnSpsGetNextNthSpsUlSf (ue, actvOffst, i);
 *           mask = futureSpsUlSf->ulBwBitmask >> (totalSubbands - allocation's
 *           end subband)
 *           if (mask & allocU32) 
 *              return the allocation to holeDb of spsUlSf
 *              return RFAILED
 *          
 *  @param[in]  RgSchCellCb   *cell
 *  @param[in]  RgSchUeCb     *ue
 *  @param[in]  U8            actvOffst
 *  @return  S16
 **/

#ifdef ANSI
PRIVATE S16 rgSCHCmnSpsUlSbAllocForUe
(
RgSchCellCb           *cell,
RgSchUeCb             *ue,
RgSchUlSf             *sf,
CmLteTimingInfo       timeToTx
)
#else /* ANSI */
PRIVATE S16 rgSCHCmnSpsUlSbAllocForUe(cell, ue, sf, timeToTx)
RgSchCellCb           *cell;
RgSchUeCb             *ue;
RgSchUlSf             *sf;
CmLteTimingInfo       timeToTx;
#endif /* ANSI */
{
   RgSchCmnUlUeSpsInfo   *ulSpsUe = RG_SCH_CMN_GET_UL_SPS_UE(ue);
   RgSchCmnUlCell        *cellUl = RG_SCH_CMN_GET_UL_CELL(cell);
   RgSchCmnUlUe          *ueUl    = RG_SCH_CMN_GET_UL_UE(ue);
   RgSchUlAlloc          *alloc=NULLP;
   RgSchPdcch            *pdcch;
   U32                   allocdBytes;
   U8                    iTbs;
   U8                    iMcs;
   U8                    numSb;

   TRC2(rgSCHCmnSpsUlSbAllocForUe);

   pdcch = rgSCHCmnPdcchAllocCrntSf(cell, ue);
   if(pdcch == NULLP)
   {
      RLOG_ARG1(L_DEBUG,DBG_CELLID,cell->cellId,
               "rgSCHCmnUlRbAllocForUe(): Could not get PDCCH CRNTI:%d",
               ue->ueId);
      RETVALUE(RFAILED);
   }

   ulSpsUe->ulSpsSchdInfo.scaledCqi = rgSCHCmnSpsScaleUlCqi(cell, ue);
   numSb = rgSCHCmnSpsUlCalcSbReqd(cell, ue, ulSpsUe->ulSpsSchdInfo.scaledCqi,
                           &iMcs, &iTbs);


   /* restricting NUM SB to max SPS BW */
   if ((cellUl->sbSize * numSb) > cell->spsCellCfg.maxSpsDlBw)
   {
      rgSCHCmnPdcchRlsCrntSf(cell, pdcch);
      RETVALUE(RFAILED);
   }
      

   if(numSb)
   {
      allocdBytes = rgTbSzTbl[0][iTbs][(U8)RGSCH_MIN(numSb, cellUl->maxSbPerUe)*cellUl->sbSize] / 8;


      if(allocdBytes >= ulSpsUe->packetSize)
      {/* Dropping the activation if enough resource is not available */
         alloc = rgSCHCmnSpsUlSbAlloc(cell, ue, sf, 
                      (U8)RGSCH_MIN(numSb, cellUl->maxSbPerUe), timeToTx);
      }
   }
                             
   if (alloc == NULLP)
   {
      RLOG_ARG1(L_ERROR,DBG_CELLID,cell->cellId,
               "rgSCHCmnUlRbAllocForUe(): Could not get UlAlloc CRNTI:%d",
               ue->ueId);
      rgSCHCmnPdcchRlsCrntSf(cell, pdcch);
      RETVALUE(RFAILED);
   }


   rgSCHCmnUlAllocFillRbInfo(cell, sf, alloc);
   alloc->pdcch = pdcch;
   alloc->grnt.iMcs = iMcs;
   alloc->grnt.iMcsCrnt = iMcs;
   alloc->grnt.hop = 0;
   alloc->grnt.nDmrs = 0; /* SPS_INTG_FIX */
   alloc->forMsg3 = FALSE;
   ueUl->alloc.allocdBytes = rgTbSzTbl[0][iTbs][alloc->grnt.numRb-1] / 8;
   /* Setting packet size to total allocbytes
      Since allocdBytes is now the configured grant */
   ulSpsUe->spsGrantSize = ueUl->alloc.allocdBytes;
   alloc->grnt.datSz = ueUl->alloc.allocdBytes;
   RG_SCH_UL_MCS_TO_MODODR(iMcs, alloc->grnt.modOdr);
   alloc->grnt.isRtx = FALSE;
   alloc->hqProc = &ue->ul.hqEnt.hqProcCb[cellUl->schdHqProcIdx];
   if(alloc->hqProc->alloc)
   {      
      /*
      rgSCHUtlUlAllocRls(
            &cellUl->ulSfArr[alloc->hqProc->ulSfIdx],
            alloc->hqProc->alloc);
       */
      rgSCHUtlUlAllocRelease(alloc->hqProc->alloc); 
   }
   alloc->hqProc->alloc = alloc; 
   alloc->hqProc->isSpsActvnHqP = TRUE; 
   /*alloc->hqProc->remTx = alloc; */
   alloc->hqProc->ulSfIdx = cellUl->schdIdx;
   alloc->ue = ue;
   alloc->rnti = ue->spsRnti;
   ueUl->alloc.alloc = alloc;
   ulSpsUe->ulSpsSchdInfo.crntAlloc = alloc;
   ulSpsUe->ulSpsSchdInfo.crntAllocSf = sf;
   ulSpsUe->spsOccIdx = 0;

   RLOG_ARG4(L_DEBUG,DBG_CELLID,cell->cellId,
            "UL SPS activation for UEID:%u, SPS-C-RNTI %u"
            "timeToTx [%u:%u]",
            ue->ueId, 
            ue->spsRnti, 
            timeToTx.sfn, 
            timeToTx.subframe);
   RLOG_ARG2(L_DEBUG,DBG_CELLID,cell->cellId,
            "UEID:%d scaledCqi %u", 
            ue->ueId, 
            ulSpsUe->ulSpsSchdInfo.scaledCqi);
   RLOG_ARG4(L_DEBUG,DBG_CELLID,cell->cellId,
            "UEID:%d iMcs %u, allocdBytes %lu, numRb %u",
            ue->ueId, 
            iMcs, 
            ueUl->alloc.allocdBytes, 
            alloc->grnt.numRb); 
   RLOG_ARG4(L_DEBUG,DBG_CELLID,cell->cellId,
            " rbStart %u, tpc %u nDmrs %u procId %d", 
            alloc->grnt.rbStart, 
            alloc->grnt.tpc, 
            alloc->grnt.nDmrs,
            cellUl->schdHqProcIdx);
  RETVALUE(ROK);
} /* end of rgSCHCmnSpsUlSbAllocForUe */

/**
 * @brief Handler to clear SPS allocations of the UE
 *
 * @details
 *
 *     Function : rgSCHCmnUlSpsClrAlloc
 *
 *     Invoked by:
 *            rgSCHCmnSpsUlProcToBeReactvtdUe
 *        
 *     Processing Steps:
 *      for (idx=0; idx < spsPrdLcmVal/ue->spsPrdcty; idx++) 
 *        fetch spsSf corr to the "idx"th SPS occasion of the UE
 *        unmark the allocation in the spsSf's bandwidth mask
 *      return ROK
 *
 *  @param[in]  RgSchCellCb             *cell
 *  @param[in]  RgSchUeCb               *ue
 *  @param[in]  RgSchCmnSpsUlSf         *spsSf
 *  @param[in]  RgSchUlAlloc            *alloc
 *  @return  S16
 **/

#ifdef ANSI
PRIVATE S16 rgSCHCmnUlSpsClrAlloc
(
RgSchCellCb             *cell,
RgSchUeCb               *ue,
RgSchCmnSpsUlSf         *spsSf,
RgSchUlAlloc            *alloc
)
#else /* ANSI */
PRIVATE S16 rgSCHCmnUlSpsClrAlloc(cell, ue, spsSf, alloc)
RgSchCellCb             *cell;
RgSchUeCb               *ue;
RgSchCmnSpsUlSf         *spsSf;
RgSchUlAlloc            *alloc;
#endif /* ANSI */
{
   RgSchCmnUlCellSpsInfo *ulSpsCell = RG_SCH_CMN_GET_UL_SPS_CELL(cell);
   RgSchCmnSpsUlSf       *crntSpsSf = spsSf;
   RgSchCmnUlUe          *ueUl    = RG_SCH_CMN_GET_UL_UE(ue);
   U8                    idx;
   U8                    idx1;
   U16                   spsPrdcty = ue->ul.spsPrdcty;
   CmLteTimingInfo       nxtOcc;

   TRC2(rgSCHCmnUlSpsClrAlloc);
   
  
   /* Reset bits corr to allocated subbands in all spsSfs corr to UE's 
    * SPS transmissions */

   for (idx=1; idx <= (ulSpsCell->spsPrdLcmVal/spsPrdcty); idx++)
   {

      for(idx1=0; idx1 < RGSCH_SPS_ULBW_MASK_LEN; idx1++)
      {
         crntSpsSf->ulBwBitMask[idx1] ^=
               ueUl->ulSpsInfo.ulSpsSchdInfo.allocBwMask[idx1];
      }

      if (crntSpsSf->numUlSpsActiveUes)
      {
         crntSpsSf->numUlSpsActiveUes--;
      }
      crntSpsSf = rgSCHCmnSpsGetNextNthSpsUlSf(cell, ue, 
                    ueUl->ulSpsInfo.ulSpsSchdInfo.ulSpsActvOcc, idx,
                    &nxtOcc);
   } /* End of for loop for spsSf */

   RETVALUE(ROK);
} /* end of rgSCHCmnUlSpsClrAlloc */

/**
 * @brief Handler to mark SPS allocation of the UE across SPS subframes
 *
 * @details
 *
 *     Function : rgSCHCmnUlSpsMarkAlloc
 *
 *     Invoked by:
 *       rgSCHCmnSpsUlUeAlloc
 *        
 *     Processing Steps:
 *      for (idx=0; idx < spsPrdLcmVal/ue->spsPrdcty; idx++) 
 *        fetch spsSf corr to the "idx"th SPS occasion of the UE
 *        if (ue's alloc collides with spsSf's existing BW allocations)
 *            return RFAILED
 *      mark the allocation in all the spsSfs that corr to UE's activation and
 *      further SPS occasions
 *      return ROK
 *
 *  @param[in]  RgSchCellCb             *cell
 *  @param[in]  RgSchUeCb               *ue
 *  @param[in]  RgSchCmnSpsUlSf         *spsSf
 *  @return  S16
 **/

#ifdef ANSI
PRIVATE S16 rgSCHCmnUlSpsMarkAlloc
(
RgSchCellCb             *cell,
RgSchUeCb               *ue,
RgSchCmnSpsUlSf         *spsSf,
CmLteTimingInfo         timeToTx
)
#else /* ANSI */
PRIVATE S16 rgSCHCmnUlSpsMarkAlloc(cell, ue, spsSf, timeToTx)
RgSchCellCb             *cell;
RgSchUeCb               *ue;
RgSchCmnSpsUlSf         *spsSf;
CmLteTimingInfo         timeToTx;
#endif /* ANSI */
{
   RgSchCmnUlCellSpsInfo *ulSpsCell = RG_SCH_CMN_GET_UL_SPS_CELL(cell);
   RgSchCmnUlUe          *ueUl = RG_SCH_CMN_GET_UL_UE(ue);
   RgSchCmnSpsUlSf       *crntSpsSf = spsSf;
   U8                    idx;
   U8                    idx1;
   U16                   spsPrdcty = ue->ul.spsPrdcty;
   CmLteTimingInfo       nxtOcc;

   TRC2(rgSCHCmnUlSpsMarkAlloc);
   
   /* Set bits corr to allocated subbands in all spsSfs corr to UE's activation
    * and further SPS transmissions */
   for (idx=1; idx <= (ulSpsCell->spsPrdLcmVal/spsPrdcty); idx++)
   {
      crntSpsSf->numUlSpsActiveUes++;
      for (idx1=0; idx1 < RGSCH_SPS_ULBW_MASK_LEN; idx1++)
      {
         crntSpsSf->ulBwBitMask[idx1] |=
               ueUl->ulSpsInfo.ulSpsSchdInfo.allocBwMask[idx1];
      }
      crntSpsSf = rgSCHCmnSpsGetNextNthSpsUlSf(cell, ue, 
                    timeToTx, idx, &nxtOcc);
   } /* End of for loop for spsSf */

   RETVALUE(ROK);
} /* end of rgSCHCmnUlSpsMarkAlloc */

/***********************************************************
 *
 *     Func : rgSCHCmnSpsUlFillPdcchWithAlloc
 *        
 *     Desc : Fills a PDCCH with format 0 SPS Activation 
 *     information.
 *
 *     Ret  : 
 *
 *     Notes: 
 *
 *     File : 
 *
 **********************************************************/
#ifdef ANSI
PRIVATE Void rgSCHCmnSpsUlFillPdcchWithAlloc
(
RgSchPdcch      *pdcch,
RgSchUlAlloc    *alloc,
RgSchUeCb       *ue
)
#else /* ANSI */
PRIVATE Void rgSCHCmnSpsUlFillPdcchWithAlloc(pdcch, alloc, ue)
RgSchPdcch      *pdcch;
RgSchUlAlloc    *alloc;
RgSchUeCb       *ue;
#endif /* ANSI */
{
#ifdef LTE_TDD
   RgSchTddANInfo    *anInfo;
   RgSchDlSf         *dlsf;
   CmLteTimingInfo   timeInfo = ue->cell->crntTime;
#endif /*LTE_TDD*/
   RgSchCmnUlCell         *cellUl = RG_SCH_CMN_GET_UL_CELL(ue->cell);
   RgSchUlSf              *sf = &cellUl->ulSfArr[cellUl->schdIdx];

   TRC2(rgSCHCmnSpsUlFillPdcchWithAlloc);

   pdcch->isSpsRnti = TRUE;
   pdcch->crnti = ue->ueId;
   pdcch->rnti = ue->spsRnti;
   pdcch->dci.dciFormat                  = TFU_DCI_FORMAT_0;
   pdcch->dci.u.format0Info.hoppingEnbld = alloc->grnt.hop;
   pdcch->dci.u.format0Info.rbStart      = alloc->grnt.rbStart;
   pdcch->dci.u.format0Info.numRb        = alloc->grnt.numRb;
   pdcch->dci.u.format0Info.mcs          = alloc->grnt.iMcsCrnt;
   pdcch->dci.u.format0Info.ndi          = 0;
   pdcch->dci.u.format0Info.isSrGrant = FALSE;
   /* SPS RNTI in pdcch */
   pdcch->dci.u.format0Info.tpcCmd = 0;
   pdcch->dci.u.format0Info.nDmrs = 0;
   /* MSB of mcs is set to 0 */
   /* Indicating aper Cqi has been requested */
   if((sf->aCqiReqCount < RG_SCH_MAX_ACQI_PER_ULSF) && (TRUE == ue->dl.reqForCqi)) 
   {

      pdcch->dci.u.format0Info.cqiReq       = ue->dl.reqForCqi;
      ue->dl.reqForCqi = FALSE;
      sf->aCqiReqCount++;
   }
   else
   {
      pdcch->dci.u.format0Info.cqiReq       = 0;
   }
#ifdef LTE_TDD
   RGSCHCMNADDTOCRNTTIME(ue->cell->crntTime, timeInfo, TFU_ULCNTRL_DLDELTA)

   /* Get DL subframe for the current time*/
   dlsf = rgSCHUtlSubFrmGet(ue->cell, timeInfo);

   /* Get ACK/NACK feedback time with respect to this current time */

   /* ccpu00132272 -MOD- for all the UL-DL configs 1-6, the uplink-association 
    * index timing should be used for calculating dai.*/
   if(dlsf->ulAscInfo.subframe != RGSCH_INVALID_INFO)
   {
      timeInfo.sfn = 
         (timeInfo.sfn + dlsf->ulAscInfo.sfnOffset) % RGSCH_MAX_SFN;
      timeInfo.subframe = dlsf->ulAscInfo.subframe;
   }
   else
   {
      RETVOID;
   }
   if(ue->cell->ulDlCfgIdx == 0)
   {
      pdcch->dci.u.format0Info.ulIdx = RG_SCH_ULIDX_MSB;
   }
   else
   {
      anInfo = rgSCHUtlGetUeANFdbkInfo(ue, &timeInfo); 
      if(anInfo != NULLP)
      {
         pdcch->dci.u.format0Info.dai = RG_SCH_GET_DAI_VALUE(anInfo->ulDai);
      }
      else
      {
         /* Fixing DAI value - ccpu00109162 */
         /* Set the no of DL subframes to zero with  DAI=3 */
         pdcch->dci.u.format0Info.dai = RG_SCH_MAX_DAI_IDX;
      }
   }
#endif /*LTE_TDD*/
   RETVOID;
}


/**
 * @brief To fill the RgSchCmnUeUlAlloc structure of UeCb.
 *
 * @details
 *
 *     Function: rgSCHCmnSpsUlUeFillAllocInfo 
 *     Purpose:  Specific scheduler to call this API to fill the alloc
 *               information.
 *
 *     Invoked by: Scheduler
 *     
 *  @param[in]  RgSchCellCb      *cell
 *  @param[out] RgSchUeCb        *ue
 *  @return   Void 
 **/
#ifdef ANSI
PRIVATE Void rgSCHCmnSpsUlUeFillAllocInfo 
(
RgSchCellCb      *cell,
RgSchUeCb        *ue
)
#else /* ANSI */
PRIVATE Void rgSCHCmnSpsUlUeFillAllocInfo(cell, ue)
RgSchCellCb      *cell;
RgSchUeCb        *ue;
#endif /* ANSI */
{
   RgSchCmnUeUlAlloc  *ulAllocInfo;
   RgSchCmnUlUe       *ueUl;
   RgSchCmnUlUeSpsInfo   *ulSpsUe = RG_SCH_CMN_GET_UL_SPS_UE(ue);

   TRC2(rgSCHCmnSpsUlUeFillAllocInfo);

   ueUl = RG_SCH_CMN_GET_UL_UE(ue);
   ulAllocInfo = &ueUl->alloc;

   /* Fill alloc structure */
   /* Fill PDCCH */
   rgSCHCmnSpsUlFillPdcchWithAlloc(ulAllocInfo->alloc->pdcch, 
                                                   ulAllocInfo->alloc, ue);
   /* invoked after filling pdcch so that in case of adaptive retx of SPS, ndi
    * can be overwritten as 1 */
   rgSCHCmnUlAllocLnkHqProc(ue, ulAllocInfo->alloc, ulAllocInfo->alloc->hqProc, 
                              ulAllocInfo->alloc->hqProc->isRetx);

   /* Recording information about this allocation */
   rgSCHCmnUlRecordUeAlloc(cell, ue);

   /* Update the UE's outstanding allocation */
   if (!ulAllocInfo->alloc->hqProc->isRetx)
   {
      rgSCHCmnUlUpdOutStndAlloc(cell, ue, 
            (ulSpsUe->packetSize - RG_SCH_CMN_SPS_BSR_HEADROOM));
   }
   RETVOID;
}

/**
 * @brief Handler to allocate UE during UL SPS activation of the UE
 *
 * @details
 *
 *     Function : rgSCHCmnSpsUlUeAlloc
 *
 *     Invoked by:
 *       rgSCHCmnSpsUlProcToBeActvtdUe
 *       rgSCHCmnSpsUlProcToBeReactvtdUe
 *        
 *     Processing Steps:
 *      if (ue has pending retransmission)
 *         return RFAILED
 *      if(hole satisfying UE's requirement doesn't exist in the subframe)
 *         return RFIALED
 *      else
 *         Check for collision of SPS occasions across sps subframes 
 *         reserve hole 
 *         grab pdcch
 *         mark bits corr to the reserved subbands in ulBwMask maintained in
 *         spsSf
 *      return ROK
 *
 *  @param[in]  RgSchCellCb             *cell
 *  @param[in]  RgSchUeCb               *ue
 *  @param[in]  RgSchCmnSpsUlSf         *spsSf
 *  @param[in]  RgSchCmnUlRbAllocInfo   *alloc
 *  @return  S16
 **/

#ifdef ANSI
PRIVATE S16 rgSCHCmnSpsUlUeAlloc
(
RgSchCellCb             *cell,
RgSchUeCb               *ue,
RgSchCmnSpsUlSf         *spsSf,
RgSchCmnUlRbAllocInfo   *alloc,
RgSchUlAlloc            *ueSpsAlloc,
CmLteTimingInfo         timeToTx
)
#else /* ANSI */
PRIVATE S16 rgSCHCmnSpsUlUeAlloc(cell, ue, spsSf, alloc, ueSpsAlloc, timeToTx)
RgSchCellCb             *cell;
RgSchUeCb               *ue;
RgSchCmnSpsUlSf         *spsSf;
RgSchCmnUlRbAllocInfo   *alloc;
RgSchUlAlloc            *ueSpsAlloc;
CmLteTimingInfo         timeToTx;
#endif /* ANSI */
{
   RgSchCmnUlCell        *cellUl = RG_SCH_CMN_GET_UL_CELL(cell);
   RgSchCmnUlUeSpsInfo   *ulSpsUe = RG_SCH_CMN_GET_UL_SPS_UE(ue);
   RgSchCmnUlUe          *cmnUlUe;
   RgSchUlHqProcCb       *hqProc;
   S16                   ret;

   TRC2(rgSCHCmnSpsUlUeAlloc);


   hqProc = &ue->ul.hqEnt.hqProcCb[cellUl->schdHqProcIdx];

   /* Check if Ue's hqProc is awaiting retransmission, if so, no SPS 
    * (re)activation till retransmission completes */
   if ((hqProc->isRetx) ||
       (RG_SCH_CMN_IS_UL_UE_RETX(ue)))
   {
      RETVALUE(RFAILED);
   }

   cmnUlUe = RG_SCH_CMN_GET_UL_UE(ue);
   cmnUlUe->alloc.reqBytes = ulSpsUe->packetSize;

   ret = rgSCHCmnSpsUlSbAllocForUe(cell, ue, alloc->sf, 
                              timeToTx);
   if (ret == ROK)
   {
      /* Mark this allocation for further sps occasions */
      rgSCHCmnUlSpsMarkAlloc(cell, ue, spsSf, timeToTx);

      /* Store scheduled alloc information for the UE */
      rgSCHCmnSpsUlUtlCpyAlloc(cmnUlUe->alloc.alloc, ueSpsAlloc);
                                            
      /* Add UE to the scheduled list of UEs in the subframe */
      rgSCHCmnUlRbAllocAddUeToLst(cell, ue, &alloc->schdUeLst);
      rgSCHCmnSpsUlUeFillAllocInfo(cell, ue);
      /* Fill activation specific params in pdcch */
   } /* Allocation for a UE succeeded */
   else
   {
      RETVALUE(RFAILED);
   }
   RETVALUE(ROK);
} /* end of rgSCHCmnSpsUlUeAlloc */

/**
 * @brief Handler to check collision of SPS occasions with measurement gap.
 *
 * @details
 *
 *     Function : rgSCHCmnSpsUlChkMesGapColl
 *
 *     Invoked by:
 *       rgSCHCmnSpsUlUeRecfg
 *       rgSCHCmnSpsUlChkUeInActv
 *        
 *     Processing Steps:
 *      for (i=0; i<lcmVal/spsPrd; i++)
 *         get the index in measGap mask corr to i'th SPS occasion
 *         if mask is set
 *         return TRUE
 *      return FALSE
 *
 *  @param[in]  RgSchCellCb             *cell
 *  @param[in]  RgSchUeCb               *ue
 *  @return  Bool
 **/

#ifdef ANSI
PRIVATE Bool rgSCHCmnSpsUlChkMesGapColl
(
RgSchCellCb             *cell,
RgSchUeCb               *ue,
CmLteTimingInfo         timeToTx,
U8                      gapPrd
)
#else /* ANSI */
PRIVATE Bool rgSCHCmnSpsUlChkMesGapColl(cell, ue, timeToTx, gapPrd)
RgSchCellCb             *cell;
RgSchUeCb               *ue;
CmLteTimingInfo         timeToTx;
U8                      gapPrd;
#endif /* ANSI */
{
   RgSchCmnUlUeSpsInfo   *ulSpsUe = RG_SCH_CMN_GET_UL_SPS_UE(ue);
   U16                   spsPrdcty = ue->ul.spsPrdcty;
   U8                    tmpIdx;
   U8                    idx;
   U16                   lcm;
   CmLteTimingInfo       timeToTx1 = timeToTx;
   TRC2(rgSCHCmnSpsUlChkMesGapColl);
   
   /* Calculate the starting bit position to be checked for */
   tmpIdx = ((timeToTx1.sfn * RGSCH_NUM_SUB_FRAMES) + 
                   (timeToTx1.subframe)) % (gapPrd);

#ifdef LTE_TDD
   if (ue->ul.ulSpsCfg.twoIntervalCfg == TRUE)
   {
      spsPrdcty *= 2;
   }
#endif

   lcm = rgSCHCmnSpsCompLcm(spsPrdcty, gapPrd);

   for (idx=1; idx <= (lcm/spsPrdcty); idx++)
   {
      if (ulSpsUe->measGapMask[tmpIdx >> 3] & (1 << (tmpIdx % 8)))
      {
         RLOG_ARG2(L_DEBUG,DBG_CELLID,cell->cellId,
            "(%d)th transmission is colliding due to measurement gap for"
            " UEID:%d", idx+1, ue->ueId); 
         RETVALUE(TRUE);
      }
      rgSCHCmnSpsGetNextNthSpsUlSf(cell, ue, 
                          timeToTx, idx, &timeToTx1);
      tmpIdx = ((timeToTx1.sfn * RGSCH_NUM_SUB_FRAMES) + 
                   (timeToTx1.subframe)) % (gapPrd);
   }
   RETVALUE(FALSE);
} /* end of rgSCHCmnSpsUlChkMesGapColl */

/**
 * @brief Handler to fetch an allocation for an active UE in the uplink subframe
 *
 * @details
 *
 *     Function : rgSCHCmnSpsUlFetchAllocInUlSf
 *
 *     Invoked by:
 *         rgSCHCmnSpsUlMarkActvUeAllocs
 *         rgSCHCmnSpsUlProcToBeActvtdUe
 *         rgSCHCmnSpsUlProcToBeReactvtdUe
 *        
 *     Processing Steps:
 *            Fetch an uplink allocation in the given subframe based on
 *             the SPS grant allocated for the UE
 *            Fill the fetched allocation with SPS grant information
 *
 *  @param[in]  RgSchCellCb             *cell
 *  @param[in]  RgSchUeCb               *ue
 *  @return  S16
 **/

#ifdef ANSI
PRIVATE S16 rgSCHCmnSpsUlFetchAllocInUlSf
(
RgSchCellCb           *cell,
RgSchUeCb             *ue,
RgSchUlSf             *ulSf
)
#else /* ANSI */
PRIVATE S16 rgSCHCmnSpsUlFetchAllocInUlSf(cell, ue, ulSf)
RgSchCellCb           *cell;
RgSchUeCb             *ue;
RgSchUlSf             *ulSf;
#endif /* ANSI */
{
   RgSchCmnUlUe          *ueUl = RG_SCH_CMN_GET_UL_UE(ue);
   RgSchCmnDlCell        *cellDl = RG_SCH_CMN_GET_DL_CELL(cell);
   RgSchUlAlloc          *alloc;
   U8                    cfi = cellDl->currCfi;
   
   TRC2(rgSCHCmnSpsUlFetchAllocInUlSf);
   

   /* create allocation for the configured grant of the UE in the ulSf. No
    * need to check for collision as this will be the first marking on the
    * subframe */
   /* TODO: Add logic for SPS release */

   /* In case if current CFI value does not match with the last cfi 
    * value with which SPS allocation was done for this UE, UE is
    * reactivated with the grant allocated according to new CFI value */
   if(ueUl->lastCfi != cellDl->currCfi)
   {
      RgSchCmnUlUeSpsInfo *ulSpsUe = RG_SCH_CMN_GET_UL_SPS_UE(ue);
      ulSpsUe->state = RGSCH_SPS_TO_BE_REACTV;
   } 
   if (cellDl->currCfi != cellDl->newCfi) /* CFI is going to change */
   {
      if (rgSCHCmnSpsUlChkUlSf(cell, ulSf) == TRUE)
      {
         RgSchCmnUlUeSpsInfo *ulSpsUe = RG_SCH_CMN_GET_UL_SPS_UE(ue);
         ulSpsUe->state = RGSCH_SPS_TO_BE_REACTV;
         cfi = cellDl->newCfi;
      }
   }   

   alloc = rgSCHCmnSpsUtlUlGetSpfcAlloc(cell, ueUl, ulSf, 
                       ueUl->ulSpsInfo.ulSpsSchdInfo.allocInfo.sbStart,
                       ueUl->ulSpsInfo.ulSpsSchdInfo.allocInfo.numSb, cfi);
#ifdef ERRCLS_KW
   /* should never come here */
   if (alloc == NULLP)
   {
      RGSCHLOGERROR(cell->instIdx, ERRCLS_DEBUG, ERG012, 0, "Unable to find alloc"
                                " for the UE's configured UL grant");
      RETVALUE(RFAILED);
   }
#endif /*(ERRCLASS & ERRCLS_DEBUG)*/

#ifdef RGSCH_SPS_STATS
   ulSpsOccSched++;
#endif

   /* Copy previous allocation information into this and store */
   {
       RgSchUlAlloc    *prvAlloc = &ueUl->ulSpsInfo.ulSpsSchdInfo.allocInfo;

       /*rgSCHCmnSpsUlUtlCpyAlloc (prvAlloc, alloc);*/
       ueUl->ulSpsInfo.ulSpsSchdInfo.crntAlloc = alloc;
       ueUl->ulSpsInfo.ulSpsSchdInfo.crntAllocSf = ulSf;
       rgSCHCmnUlAllocFillRbInfo(cell, ulSf, alloc);
       alloc->grnt.hop = 0;
       alloc->grnt.nDmrs = 0; /* SPS_INTG_FIX */
       alloc->forMsg3 = FALSE;
       alloc->grnt.isRtx = FALSE;
       alloc->grnt.iMcs = prvAlloc->grnt.iMcs;
       alloc->grnt.iMcsCrnt = prvAlloc->grnt.iMcsCrnt;
       alloc->grnt.modOdr = prvAlloc->grnt.modOdr;
       alloc->grnt.datSz = prvAlloc->grnt.datSz;
       /*   RG_SCH_UL_MCS_TO_MODODR(alloc->grnt.iMcs, alloc->grnt.modOdr);*/
       alloc->ue = ue;
       alloc->rnti = ue->spsRnti;
       /* No pdcch for configured UL grant */
       alloc->pdcch = NULLP;
       alloc->hqProc=NULLP;
       rgSCHCmnSpsUlUtlCpyAlloc (alloc, prvAlloc);
   }
   RETVALUE(ROK);
} /* end of rgSCHCmnSpsUlFetchAllocInUlSf */

/**
 * @brief Handler to process UL SPS activated UEs whose SPS occasion is in
 *        the current subframe 
 *
 * @details
 *
 *     Function : rgSCHCmnSpsUlProcActvUeInCrntSf
 *
 *     Invoked by:
 *      - rgSCHCmnSpsUlProcActvtdUes
 *        
 *     Processing Steps:
 *      Calculate crntIndex into the spsSfLst maintained in ulCellCb as
 *      (10*crntSfn + crntSubframe)%(cell->ul.ulSpsInfo.spsPrdLcmVal)
 *      fetch the list of UEs whose SPS Occasion is the current subframe
 *       (crntIndex - 1)%(RG_SCH_CMN_SPS_MAX_PRD) from actvUlSpsUeLsts
 *      for each UE
 *       calculate its next SPS Occasion and move it in actvLst
#ifdef LTE_TDD
 *       toggle nMod2 value
#endif
 *      return ROK              
 *
 *  @param[in]  RgSchCellCb     *cell
 *  @return  S16
 **/

#ifdef ANSI
PRIVATE S16 rgSCHCmnSpsUlProcActvUeInCrntSf
(
RgSchCellCb             *cell
)
#else /* ANSI */
PRIVATE S16 rgSCHCmnSpsUlProcActvUeInCrntSf(cell)
RgSchCellCb             *cell;
#endif /* ANSI */
{
   RgSchCmnUlCellSpsInfo *ulSpsCell = RG_SCH_CMN_GET_UL_SPS_CELL(cell);
   RgSchCmnUlUeSpsInfo   *ulSpsUe;
   U16                   actvUeLstIdx;
   CmLListCp             *actvUeLstCp;
   RgSchUeCb             *ue;
   CmLList               *ueLnk;

   TRC2(rgSCHCmnSpsUlProcActvUeInCrntSf);


   /* Fetch the list of UEs whose SPS occasion is the current subframe */
   actvUeLstIdx = (((cell->crntTime.sfn * RGSCH_NUM_SUB_FRAMES) + 
              cell->crntTime.subframe)) % (RG_SCH_CMN_SPS_MAX_PRD);

   actvUeLstCp = &ulSpsCell->actvUlSpsUeLsts[actvUeLstIdx];

   ueLnk = actvUeLstCp->first;

   for(;ueLnk; ueLnk=actvUeLstCp->first)
   {
      ue = (RgSchUeCb *)(ueLnk->node);
      ulSpsUe = RG_SCH_CMN_GET_UL_SPS_UE(ue);

      ulSpsUe->spsOccIdx += 1;

      /*The current allocation is used up at the SPS allocation
       * so set it it to NULLP*/
      ulSpsUe->ulSpsSchdInfo.crntAlloc = NULLP;
      ulSpsUe->ulSpsSchdInfo.crntAllocSf = NULLP;
      /* compute the UE's next SPS occasion and move UE to the corr
         offset in actvdUeLst. Delete from previous occasion and insert
         in next SPS occasion */

      rgSCHCmnSpsUlDelUeFrmActvLst(cell, ue, 
                                     ulSpsUe->ulSpsSchdInfo.crntUlSpsOcc);
      rgSCHCmnSpsGetNextNthSpsUlSf(cell, ue, 
                    ulSpsUe->ulSpsSchdInfo.ulSpsActvOcc, ulSpsUe->spsOccIdx, 
                    &ulSpsUe->ulSpsSchdInfo.crntUlSpsOcc);
      rgSCHCmnSpsUlInsUeInActvLst (cell, ue, 
                                     ulSpsUe->ulSpsSchdInfo.crntUlSpsOcc);
   }
   RETVALUE(ROK);
} /* end of rgSCHCmnSpsUlProcActvUeInCrntSf */

/**
 * @brief Handler to process  the next SPS occasion of UL SPS activated UEs
 *
 * @details
 *
 *     Function : rgSCHCmnSpsUlProcActvUeNxtOcc
 *
 *     Invoked by:
 *      - rgSCHCmnSpsUlProcActvtdUes
 *        
 *     Processing Steps:
 *      Calculate crntIndex into the spsSfLst maintained in ulCellCb as
 *      (10*crntSfn + crntSubframe)%(cell->ul.ulSpsInfo.spsPrdLcmVal)
 *      fetch the list of UEs whose SPS occasion is at the current calculated 
 *      index + UL_SCHD_DELTA by indexing actvUeLst with
 *            ((crntIndex + UL_SCHD_DELTA) % (RG_SCH_CMN_SPS_MAX_PRD))
 *      for each UE in the list
 *          if (ue->state == REACTV)
 *             invoke rgSCHCmnSpsUlProcToBeReactvtdUe
 *          if (ue->pdcchToRetx == TRUE)
 *          invoke rgSCHCmnPdcchAllocCrntSf to get a pdcch 
 *          fill the allocation information same as the previous one sent
 *          set the pdcch members as per activation/reactivation 
 *          set ue->pdcchToRetx = FALSE
 *          ue->state = RGSCH_ULSPS_ACTV
 *      return               
 *
 *  @param[in]  RgSchCellCb             *cell
 *  @param[in]  RgSchCmnUlRbAllocInfo   *allocInfo
 *  @return  S16
 **/

#ifdef ANSI
PRIVATE S16 rgSCHCmnSpsUlProcActvUeNxtOcc
(
RgSchCellCb             *cell,
RgSchCmnUlRbAllocInfo   *allocInfo
)
#else /* ANSI */
PRIVATE S16 rgSCHCmnSpsUlProcActvUeNxtOcc(cell, allocInfo)
RgSchCellCb             *cell;
RgSchCmnUlRbAllocInfo   *allocInfo;
#endif /* ANSI */
{
   Pst                   pst;
   RgInfUlSpsReset       ulSpsResetInfo;
   RgSchCmnUlCellSpsInfo *ulSpsCell = RG_SCH_CMN_GET_UL_SPS_CELL(cell);
   RgSchCmnUlUeSpsInfo   *ulSpsUe;
   U16                   actvUeLstIdx;
   CmLListCp             actvUeLstCp;
   RgSchUeCb             *ue;
   RgSchCmnUlUe          *ueUl;
   CmLList               *ueLnk;
   U8              hqProcIdx;
   RgSchUlAlloc    *alloc;    /*!< Allocation information of the UE */
   RgSchUlSf      *reTxSf;
   RgSchCmnUlCell        *cellUl = RG_SCH_CMN_GET_UL_CELL(cell);
   
   TRC2(rgSCHCmnSpsUlProcActvUeNxtOcc);


   /* Fetch the list of UEs whose SPS occasion is at crntTime + uplink 
    * scheduling delta */
#ifndef LTE_TDD
   actvUeLstIdx = (((cell->crntTime.sfn * RGSCH_NUM_SUB_FRAMES) + 
              cell->crntTime.subframe + RGSCH_SPS_UL_DELTA))
                                                   % (RG_SCH_CMN_SPS_MAX_PRD);
#else
   actvUeLstIdx = cellUl->schdTti % RG_SCH_CMN_SPS_MAX_PRD;
#endif

   actvUeLstCp = ulSpsCell->actvUlSpsUeLsts[actvUeLstIdx];

   ueLnk = actvUeLstCp.first;

   for(; ueLnk; ueLnk=ueLnk->next)
   {
       ue = (RgSchUeCb *)(ueLnk->node);
       ueUl = RG_SCH_CMN_GET_UL_UE(ue);
       ulSpsUe = RG_SCH_CMN_GET_UL_SPS_UE(ue);

       /*Link the Harq Proc Start*/
       alloc = ulSpsUe->ulSpsSchdInfo.crntAlloc;
       hqProcIdx = cellUl->schdHqProcIdx;
       alloc->hqProc =  &ue->ul.hqEnt.hqProcCb[hqProcIdx];

       if (alloc->hqProc->alloc)
       {
         rgSCHCmnUlFreeAlloc(cell, alloc->hqProc->alloc); 
       }
         /* SPS_INTG_FIX */
       if(alloc->hqProc->reTxLnk.node)
       {
          reTxSf = &cellUl->ulSfArr[cellUl->schdIdx]; 
          cmLListDelFrm(alloc->hqProc->reTxAlloc.reTxLst, &alloc->hqProc->reTxLnk);
          alloc->hqProc->reTxLnk.node = NULLP;
#ifdef LTE_L2_MEAS
          rgSCHUhmFreeProc(alloc->hqProc, cell);
#else
          rgSCHUhmFreeProc(alloc->hqProc);
#endif
       }
       if (ulSpsUe->state == RGSCH_SPS_TO_BE_REACTV)
       {
          /* ccpu00139522- Do not schedule if Ue is inactive at the next SPS 
           * occasion. This can happen because of reconfiguration in between 
           * the SPS occasions of the UE */
          if ((rgSCHCmnSpsUlChkUeInActv(cell, ue)) == FALSE)
          {
             rgSCHCmnSpsUlProcToBeReactvtdUe (cell, ue, allocInfo);
             continue;
          }
       }
       rgSCHCmnUlAllocLnkHqProc(ue, alloc, alloc->hqProc, FALSE);
       /*alloc->hqProc->alloc = alloc;*/
       alloc->hqProc->ulSfIdx = (cellUl->schdIdx);/* % (RG_SCH_CMN_UL_NUM_SF);*/
       ulSpsUe->ulSpsSchdInfo.allocInfo.hqProc = alloc->hqProc;
       ueUl->alloc.alloc = alloc;
       ueUl->alloc.allocdBytes = alloc->grnt.datSz;
       /*Link the Harq Proc end*/
 #if 0
          ulSpsUe->ulSpsSchdInfo.ulSpsActvOcc = 
             ulSpsUe->ulSpsSchdInfo.crntUlSpsOcc;
 #endif

       if (ulSpsUe->pdcchToRetx == TRUE)
       {
          /* Do nothing if Ue is inactive at the next SPS occasion. This can
           * happen because of reconfiguration in between the SPS occasions 
           * of the UE */
          if ((rgSCHCmnSpsUlChkUeInActv(cell, ue)) == TRUE)
          {
             alloc->hqProc->isSpsOccnHqP = TRUE; 
             continue;
          }

          RLOG_ARG1(L_DEBUG,DBG_CELLID,cell->cellId,
                  "UL SPS ACT PDCCH to be TXed again due to max crcF UEID:%d",
                  ue->ueId);
          /* Fetch a pdcch and send out the release */
          RgSchPdcch *pdcch = rgSCHCmnPdcchAllocCrntSf(cell, ue);
          if (pdcch == NULLP)
          {
             RLOG_ARG1(L_ERROR,DBG_CELLID,cell->cellId,
                      "rgSCHCmnSpsUlProcActvtdUes:PDCCH unavailable to retransmit"
                      " for CRNTI:%d", ue->ueId);
             RETVALUE(RFAILED);
          }
          /* Fill PDCCH params same as the ones sent before */
          rgSCHCmnUlFillPdcchWithAlloc (pdcch, 
                &ulSpsUe->ulSpsSchdInfo.allocInfo, ue);
          ueUl->alloc.alloc->pdcch = pdcch;
          ueUl->alloc.alloc->forMsg3 = FALSE; 
          ulSpsUe->pdcchToRetx = FALSE;
          alloc->hqProc->isSpsActvnHqP = TRUE; 

          ulSpsResetInfo.crnti = ue->ueId;
          ulSpsResetInfo.cellId = cell->cellId;
          rgSCHUtlGetPstToLyr(&pst, &rgSchCb[cell->instIdx], cell->macInst);
          RgSchMacUlSpsReset(&pst, &ulSpsResetInfo);
       }
       else
       {
          alloc->hqProc->isSpsOccnHqP = TRUE; 
       }

   } /* end of while */

   RETVALUE(ROK);
} /* end of rgSCHCmnSpsUlProcActvUeNxtOcc */

/**
 * @brief Handler to mark SPS allocations of the activated UEs in uplink
 * subframe to be scheduled
 *
 * @details
 *
 *     Function : rgSCHCmnSpsUlMarkActvUeAllocs
 *
 *     Invoked by:
 *      - rgSCHCmnSpsUlProcActvtdUes
 *        
 *     Processing Steps:
 *      Calculate crntIndex into the spsSfLst maintained in ulCellCb as
 *      (10*crntSfn + crntSubframe)%(cell->ul.ulSpsInfo.spsPrdLcmVal)
 *      fetch the list of UEs whose SPS Occasion is at the current calculated
 *      index + TFU_DELTA + PDCCH_PUSCH_DELTA +DL_DELTA + 1 by indexing  
 *            actvUeLst with ((crntIndex + 10) % (RG_SCH_CMN_SPS_MAX_PRD))
 *      for each UE in the list
 *          create allocation for the configured grant of the UE in the ulSf
 *          This shall take care of the collision of msg3 allocations/non-adaptive
 *          retx with configured grants, if any. This shall also take care 
 *          of the reception requests being sent to PHY at the expected time
 *           for the configured UL grants
 *      return        
 *
 *  @param[in]  RgSchCellCb     *cell
 *  @return  S16
 **/

#ifdef ANSI
PRIVATE S16 rgSCHCmnSpsUlMarkActvUeAllocs
(
RgSchCellCb             *cell
)
#else /* ANSI */
PRIVATE S16 rgSCHCmnSpsUlMarkActvUeAllocs(cell)
RgSchCellCb             *cell;
#endif /* ANSI */
{
   RgSchCmnUlUeSpsInfo   *ulSpsUe = NULLP;
   RgSchCmnUlCell        *cellUl = RG_SCH_CMN_GET_UL_CELL(cell);
   RgSchCmnUlCellSpsInfo *ulSpsCell = RG_SCH_CMN_GET_UL_SPS_CELL(cell);
   U16                   actvUeLstIdx;
   CmLListCp             actvUeLstCp;
   RgSchUeCb             *ue;
   CmLList               *ueLnk;
   RgSchUlSf             *ulSf;
#ifdef LTE_TDD
   U8                    offst;
   CmLteTimingInfo       timeInfo;
#else /*LTE_TDD*/
   U8                    ulSfIdx;
#endif/*LTE_TDD*/

   TRC2(rgSCHCmnSpsUlMarkActvUeAllocs);


#ifndef LTE_TDD
   /* Fetch the list of UEs whose SPS Occasion is at current time +
      TFU_DELTA + PDCCH_PUSCH_DELTA +DL_DELTA + 1,i.e., 10 subframes ahead so
      that collision of msg3 allocations/non-adaptive retx with configured 
      grants, if any and the reception requests to be sent to PHY for the
      configured grants can be taken care of */
   actvUeLstIdx = ((cell->crntTime.sfn * RGSCH_NUM_SUB_FRAMES) + 
              cell->crntTime.subframe + RGSCH_RARSP_MSG3_DELTA
               + RG_SCH_CMN_DL_DELTA + 1) % RG_SCH_CMN_SPS_MAX_PRD;
#else /* LTE_TDD */
   RG_SCH_ADD_TO_CRNT_TIME(cell->crntTime, timeInfo, RG_SCH_CMN_DL_DELTA);
   if (!(offst = rgSchTddSpsUlRsrvTbl[cell->ulDlCfgIdx][timeInfo.subframe]))
   {
      RETVALUE(ROK);
   }
   actvUeLstIdx = ((timeInfo.sfn * RGSCH_NUM_SUB_FRAMES) + 
              timeInfo.subframe + offst) % RG_SCH_CMN_SPS_MAX_PRD;
#endif /* LTE_TDD */

   actvUeLstCp = ulSpsCell->actvUlSpsUeLsts[actvUeLstIdx];

   ueLnk = actvUeLstCp.first;

   for(; ueLnk; ueLnk = ueLnk->next)
   {
      ue = (RgSchUeCb *)(ueLnk->node);

      /* later remove this to use sps idx computed every TTI once
       * it works fine */
#ifndef LTE_TDD
      /* Fetch the UL subframe that shall be scheduled 10 subframes later */
      ulSfIdx = (cellUl->msg3SchdIdx +  1) % (RG_SCH_CMN_UL_NUM_SF);
      ulSf = &cellUl->ulSfArr[ulSfIdx];
#else /*LTE_TDD*/

      ulSf = &cellUl->ulSfArr[cellUl->spsUlRsrvIdx];
#endif /*LTE_TDD*/

      ulSpsUe = RG_SCH_CMN_GET_UL_SPS_UE(ue);
      if (ulSpsUe->actPdcchCrcFailCount == RGSCH_SPS_MAX_UL_ACT_CRC_FAIL)
      {
         rgSCHCmnSpsUlProcRelInd(cell, ue, TRUE);
      }
      else
      {
         rgSCHCmnSpsUlFetchAllocInUlSf(cell, ue, ulSf);
      }
   }

   RETVALUE(ROK);
} /* end of rgSCHCmnSpsUlMarkActvUeAllocs */

/**
 * @brief Handler to process UL SPS activated UEs at the time of TTI
 *
 * @details
 *
 *     Function : rgSCHCmnSpsUlProcActvtdUes
 *
 *     Invoked by:
 *      - rgSCHCmnSpsUlTti
 *        
 *     Processing Steps:
 *        Invoke rgSCHCmnSpsUlProcActvUeInCrntSf to process the UEs whose 
 *        SPS occasion is in current subframe
 *        Invoke rgSCHCmnSpsUlProcActvUeNxtOcc to process the next SPS 
 *        occasion of the UL SPS activated UEs
 *        Invoke rgSCHCmnSpsUlMarkActvUeAllocs to mark the allocations in 
 *        ulSf for the activated UEs
 *        return       
 *
 *  @param[in]  RgSchCellCb     *cell
 *  @return  S16
 **/

#ifdef ANSI
PRIVATE S16 rgSCHCmnSpsUlProcActvtdUes
(
RgSchCellCb             *cell,
RgSchCmnUlRbAllocInfo   *allocInfo
)
#else /* ANSI */
PRIVATE S16 rgSCHCmnSpsUlProcActvtdUes(cell, ulSched, allocInfo)
RgSchCellCb             *cell;
RgSchCmnUlRbAllocInfo   *allocInfo;
#endif /* ANSI */
{

   TRC2(rgSCHCmnSpsUlProcActvtdUes);


   /* Process the UEs whose SPS occasion is in current subframe */
   rgSCHCmnSpsUlProcActvUeInCrntSf(cell);

   /* Process the next SPS occasion of the UL SPS activated UEs */
   if (NULLP != allocInfo)
   {
      rgSCHCmnSpsUlProcActvUeNxtOcc(cell, allocInfo);
   }

   /* Mark the allocations in ulSf for the activated UEs */
   rgSCHCmnSpsUlMarkActvUeAllocs(cell);

   RETVALUE(ROK);
} /* end of rgSCHCmnSpsUlProcActvtdUes */


/**
 * @brief Handler to process UEs which are waiting for UL SPS release
 *
 * @details
 *
 *     Function : rgSCHCmnSpsUlProcToBeRelUe
 *
 *     Invoked by:
 *      - rgSCHCmnSpsUlTti
 *        
 *     Processing Steps:
 *       fetch the first UE in the "toBeRelUeLst"
 *       invoke rgSCHCmnPdcchAllocCrntSf to fetch a PDCCH to send release
 *       if (pdcch == NULLP)
 *          return RFAILED;
 *       else
 *          fill the pdcch with release specific information
 *          set ue->ulSpsInfo.state = REL_SENT
 *       
 *
 *  @param[in]  RgSchCellCb             *cell
 *  @return  S16
 **/

#ifdef ANSI
PRIVATE S16 rgSCHCmnSpsUlProcToBeRelUe
(
RgSchCellCb             *cell
)
#else /* ANSI */
PRIVATE S16 rgSCHCmnSpsUlProcToBeRelUe(cell)
RgSchCellCb             *cell;
#endif /* ANSI */
{
   RgSchCmnUlCellSpsInfo *ulSpsCell = RG_SCH_CMN_GET_UL_SPS_CELL(cell);
   RgSchCmnUlCell        *cellUl = RG_SCH_CMN_GET_UL_CELL(cell);
   RgSchCmnUlUeSpsInfo   *ulSpsUe = NULLP;
   CmLList               *ueLnk;
   RgSchUeCb             *ue;
   RgSchPdcch            *pdcch;
   RgSchCmnUlUe          *ueUl; 
   RgSchUlHqProcCb       *hqProc;

   TRC2(rgSCHCmnSpsUlProcToBeRelUe);


   /* Fetch the first UE waiting for UL SPS activation */
   ueLnk = cmLListFirst(&ulSpsCell->toBeRelUeLst);
   if ((ueLnk == NULLP) || ((ue = (RgSchUeCb *)(ueLnk->node)) == NULLP))
   {
      RETVALUE(ROK);
   }


   for(;ueLnk; ueLnk= ueLnk->next)
   {
      /* Check if Ue is inactive for UL scheduling */
      if (ue->ul.ulInactvMask)
      {
         continue;
      } /* end of (ue->inactvmask) */

      hqProc = &ue->ul.hqEnt.hqProcCb[cellUl->schdHqProcIdx];

      /* Fetch a pdcch and send out the release */
      pdcch = rgSCHCmnPdcchAllocCrntSf(cell, ue);
      if (pdcch == NULLP)
      {
         RLOG_ARG1(L_ERROR,DBG_CELLID,cell->cellId,
           "rgSCHCmnSpsUlProcToBeRelUe:PDCCH unavailable to send SPS release"
           " for CRNTI:%d", ue->ueId);
         /* Move Ue to the end of the Q */
         RETVALUE(RFAILED);
      }
      /* Fill release PDCCH */
      rgSCHCmnSpsUlUtlFillRelPdcch(cell, ue, pdcch);
#ifdef RGSCH_SPS_STATS     
      rgSchSpsRelPdcchAllocd++;
#endif
      ueUl = RG_SCH_CMN_GET_UL_UE(ue);
      ueUl->ulSpsInfo.state = RGSCH_SPS_REL_SENT;
      RLOG_ARG3(L_DEBUG,DBG_CELLID,cell->cellId,
               "UL SPS REL PDCCH sent for UEID:%d sfn %d sf %d ",
               ue->ueId, cell->crntTime.sfn,
               cell->crntTime.subframe);
      rgSCHCmnSpsUlDelUeFrmToBeRelLst(cell, ue);

      ulSpsUe = RG_SCH_CMN_GET_UL_SPS_UE(ue);
      ulSpsUe->relPdcchSntCnt++;
      ue->ul.relPdcchSchdTime = cell->crntTime;
      if (ulSpsUe->relPdcchSntCnt == RGSCH_SPS_MAX_UL_REL_PDCCH)
      {
         ueUl->ulSpsInfo.state = RGSCH_SPS_IDLE;
         ue->ul.explicitRelCnt = 0;
         ulSpsUe->relPdcchSntCnt = 0;
         ue->ul.relPdcchSchdTime.subframe = 0xFF;
#ifdef RGSCH_SPS_STATS
         ulExplRelSucc++;
#endif
      }
      else
      {
         /*Add to end of the list to give other UEs opportunity in
           the next TTI */
         rgSCHCmnSpsUlUtlInsUeInToBeRelLst (cell, ue);
      }
      /*Breaking here to process only one UE per TTI*/
      break;
   }
   RETVALUE(ROK);
} /* end of rgSCHCmnSpsUlProcToBeRelUe */

/**
 * @brief Handler to process UEs which are waiting for UL SPS reactivation
 *
 * @details
 *
 *     Function : rgSCHCmnSpsUlProcToBeReactvtdUe
 *
 *     Invoked by:
 *       rgSCHCmnSpsUlTti
 *        
 *     Processing Steps:
 *      fetch the subframe information from the "spsSfLst" with index as 
 *      ((crntSfn*10)+crntSubframe + UL_SCHD_DELTA)%spsPrdLcmVal
 *      invoke rgSCHCmnRbAllocForUe to fetch  an allocation from subframe's 
 *      spsAllocDb for ueUlSpsInfo.sbsReqd
 *      if (alloc == NULLP)
 *         ue->state = TO_BE_REACTV
 *         add Ue to cellUlSpsInfo.toBeActvtdUeLst
 *         return RFAILED
 *      ue->rbsallocd = alloc->numRbs
 *      remove UE's  old allocation from the subframes
 *      copy the allocation in all the subframes corresponding to the UE's SPS
 *      periodicity
 *      return ROK
 *
 *  @param[in]  RgSchCellCb             *cell
 *  @param[in]  RgSchCmnUlRbAllocInfo   *alloc
 *  @return  S16
 **/

#ifdef ANSI
PRIVATE S16 rgSCHCmnSpsUlProcToBeReactvtdUe
(
RgSchCellCb             *cell,
RgSchUeCb               *ue,
RgSchCmnUlRbAllocInfo   *alloc
)
#else /* ANSI */
PRIVATE S16 rgSCHCmnSpsUlProcToBeReactvtdUe(cell, ue, alloc)
RgSchCellCb             *cell;
RgSchUeCb               *ue;
RgSchCmnUlRbAllocInfo   *alloc;
#endif /* ANSI */
{
   RgSchUlAlloc           *reAlloc = NULLP;
   RgSchCmnDlCell         *cellDl = RG_SCH_CMN_GET_DL_CELL(cell);
   RgSchCmnUlUe          *ueUl = NULLP;
   Pst                   pst;
   RgInfUlSpsReset       ulSpsResetInfo;
   RgSchCmnUlCellSpsInfo *ulSpsCell = RG_SCH_CMN_GET_UL_SPS_CELL(cell);
   RgSchCmnUlUeSpsInfo   *ulSpsUe = RG_SCH_CMN_GET_UL_SPS_UE(ue);
   CmLteTimingInfo       timeToTx;
   U16                   spsSfIdx; /* Index into the SPS Subframes Array */
   RgSchCmnSpsUlSf       *spsSf;   /* UL SPS information of a subframe */
   RgSchUlAlloc          tmpUeAlloc;
   RgSchUlAlloc          allocCpy;
   Bool                  isCpyValid = FALSE;
   U32                   idx =0;
   U32                   tmpAllocBwMask[RGSCH_SPS_ULBW_MASK_LEN] = {0};
#ifdef LTE_TDD
   RgSchCmnUlCell  *cellUl = RG_SCH_CMN_GET_UL_CELL(cell);
#endif


   TRC2(rgSCHCmnSpsUlProcToBeReactvtdUe);


   /* Calculate timingInfo that UE needs to transmit in, if Ue reactivation
    * succeeds */
#ifdef LTE_TDD
   timeToTx = cellUl->schdTime;
#else
   RG_SCH_ADD_TO_CRNT_TIME(cell->crntTime, timeToTx, RGSCH_SPS_UL_DELTA);
#endif

   /* Fetch the sps subframe corresponding to the UE's SPS occasion which is
    * away by 6 (UL scheduling delta) subframes from the current subframe */
   spsSfIdx = ((timeToTx.sfn * RGSCH_NUM_SUB_FRAMES) + (timeToTx.subframe)) %
                                         ulSpsCell->spsPrdLcmVal;
   spsSf = &(ulSpsCell->spsSfLst[spsSfIdx]); 

   /* Remove previous allocation */
   if(ulSpsUe->ulSpsSchdInfo.crntAlloc)
   {
      cmMemcpy((U8*)&allocCpy, (U8*)ulSpsUe->ulSpsSchdInfo.crntAlloc, sizeof(RgSchUlAlloc));
      rgSCHCmnUlFreeAlloc(cell, ulSpsUe->ulSpsSchdInfo.crntAlloc);
      ulSpsUe->ulSpsSchdInfo.crntAlloc = NULLP;
      isCpyValid = TRUE;
   }
   /* Free the allocations of the UE in cell wide sps sf tbl */
   rgSCHCmnUlSpsClrAlloc (cell, ue, &(ulSpsCell->spsSfLst[
            ulSpsUe->ulSpsSchdInfo.actvSfLstIdx]),
         &ulSpsUe->ulSpsSchdInfo.allocInfo);

   for (idx=0; idx < RGSCH_SPS_ULBW_MASK_LEN; idx++)
   {
      tmpAllocBwMask[idx] = ulSpsUe->ulSpsSchdInfo.allocBwMask[idx];
      ulSpsUe->ulSpsSchdInfo.allocBwMask[idx] = 0;
   }

   ulSpsUe->crcFailCntSpsOcc = 0;
   /* Fetch an allocation for the new RB requirement of Ue in the subframe */
   if ((rgSCHCmnSpsUlUeAlloc (cell, ue, spsSf, alloc, &tmpUeAlloc, timeToTx))!=
                                                         ROK)
   {
      RLOG_ARG1(L_ERROR,DBG_CELLID,cell->cellId,
         "CRNTI:%d allocation failed and hence reactivation at the same offset "
         "failed", ue->ueId);

      ueUl = RG_SCH_CMN_GET_UL_UE(ue);
      reAlloc = rgSCHCmnSpsUtlUlGetSpfcAlloc(cell, ueUl, ulSpsUe->ulSpsSchdInfo.crntAllocSf,
                       ueUl->ulSpsInfo.ulSpsSchdInfo.allocInfo.sbStart,
                       ueUl->ulSpsInfo.ulSpsSchdInfo.allocInfo.numSb, cellDl->currCfi);

      ulSpsUe->ulSpsSchdInfo.crntAlloc = reAlloc;
     
      if (reAlloc && (TRUE == isCpyValid))
      {
         reAlloc->grnt.hop = 0;
         reAlloc->grnt.nDmrs = 0; /* SPS_INTG_FIX */
         reAlloc->forMsg3 = FALSE;
         reAlloc->grnt.isRtx = FALSE;
         reAlloc->grnt.iMcs = allocCpy.grnt.iMcs;
         reAlloc->grnt.iMcsCrnt = allocCpy.grnt.iMcsCrnt;
         reAlloc->grnt.modOdr = allocCpy.grnt.modOdr;
         reAlloc->grnt.datSz = allocCpy.grnt.datSz;
         reAlloc->ue = ue;
         reAlloc->rnti = ue->spsRnti;
         /* No pdcch for configured UL grant */
         reAlloc->pdcch = allocCpy.pdcch;
         reAlloc->hqProc= allocCpy.hqProc;
      }
      /* Set the alloced variables and trigger expl rel */
      for (idx=0; idx < RGSCH_SPS_ULBW_MASK_LEN; idx++)
      {
         ulSpsUe->ulSpsSchdInfo.allocBwMask[idx] = tmpAllocBwMask[idx];
      }
      rgSCHCmnUlSpsMarkAlloc(cell, ue, 
            &(ulSpsCell->spsSfLst[ ulSpsUe->ulSpsSchdInfo.actvSfLstIdx]),
             ulSpsUe->ulSpsSchdInfo.ulSpsActvOcc);

      /* Trigger Explicit Release */
      rgSCHCmnSpsUlProcRelInd(cell, ue, TRUE);

      RETVALUE(RFAILED);
   }     

   ulSpsResetInfo.crnti = ue->ueId;
   ulSpsResetInfo.cellId = cell->cellId;
   rgSCHUtlGetPstToLyr(&pst, &rgSchCb[cell->instIdx], cell->macInst);
   RgSchMacUlSpsReset(&pst, &ulSpsResetInfo);

   ulSpsUe->state = RGSCH_SPS_ACTV_PDCCH_SENT;
   ulSpsUe->ulSpsSchdInfo.ulSpsActvOcc = 
      ulSpsUe->ulSpsSchdInfo.crntUlSpsOcc;
   ulSpsUe->ulSpsSchdInfo.actvSfLstIdx = spsSfIdx;

#ifdef RGSCH_SPS_STATS
      rgSchSpsUlSpsActDciTime[rgSchSpsNumUlSpsActSent% RGSCH_SPS_MAX_STATS]
         =  timeToTx;
      rgSchSpsNumUlSpsActSent++;
      rgSchSpsNumUlReact++;
#endif

   /* Remove the previous existing allocation of the Ue from the SPS subframe
    * and the subframe which is getting scheduled in this tti */
   /*rgSCHCmnUlSpsClrAlloc (cell, ue, spsSf,&ulSpsUe->ulSpsSchdInfo.allocInfo);*/
   /* Release previous allocation from the scheduling subframe's allocDb */
  /*rgSCHUtlUlAllocRls(alloc->sf,ulSpsUe->ulSpsSchdInfo.allocInfo.hqProc->alloc);*/
   /* Update the allocation into ueCb */
   rgSCHCmnSpsUlUtlCpyAlloc (&tmpUeAlloc, &ulSpsUe->ulSpsSchdInfo.allocInfo);

   RETVALUE(ROK);
} /* end of rgSCHCmnSpsUlProcToBeReactvtdUe */

/**
 * @brief Handler to process UEs which are waiting for UL SPS activation
 *
 * @details
 *
 *     Function : rgSCHCmnSpsUlProcToBeActvtdUe
 *
 *     Invoked by:
 *       rgSCHCmnSpsUlTti. For TDD, this shall be invoked only when the subframe
 *       to be scheduled is an UL subframe
 *        
 *     Processing Steps:
 *       fetch the first UE in toBeActvtdUeLst of cell
 *         if Ue is inactive due to DRX, 
 *             add UE to the end of toBeActvtdUes list
 *             return RFAILED
 *         if (ue->isHdFdd == TRUE)
 *           query HD-FDD module to check if UE is eligible for UL transmission
 *           at subframe = crntTime + UL_SCHD_DELTA
 *           if (retVal == FALSE)
 *             add UE to the end of toBeActvtdUes list
 *             return RFAILED
 *       fetch the subframe information from the "spsSfLst" with index as 
 *       ((crntSfn*10) + crntSubframe + UL_SCHD_DELTA)%spsPrdLcmVal
 *         if (ue->is measGapEnabled == TRUE)
 *           check collision with measurement gap using measGapMask for 
 *           current and future SPS ocassions
 *           if (collision == TRUE)
 *             add UE to the end of toBeActvtdUes list
 *             return RFAILED
 *       invoke rgSCHCmnSpsUlUeAlloc to fetch an allocation from subframe's
 *       spsAllocDb for ueUlSpsInfo.sbsReqd
 *       if (alloc == NULLP)
 *          add UE to the end of toBeActvtdUes list
 *          return RFAILED 
 *       Mark the allocation in the current and future SPS occaisons using the
 *       UE's SPS periodicity
 *       ue->isUlSpsActv = TRUE
 *       ue->state = RGSCH_ACTV_REACTV_SENT
 *       add UE to actvUlSpsUeLsts at the selected index.
 *       remove UE from toBeActvtdUes list
 *       if (ue->state == TO_BE_REACTV)
 *         remove UE from the old index in actvUlSpsUeLsts
 *         remove UE's old allocation from the subframes corr to its SPS
 *         occasions
 *       else
 *         register the SPS-Rnti and the set of lcIds given as SPS LCs with
 *         MAC. Invoke RgSchMacSpsLcRegReq() for this
 *       return ROK
 *
 *  Notes: Only one UE's activation shall be attempted per TTI
 *
 *  @param[in]  RgSchCellCb     *cell
 *  @return     S16
 **/

#ifdef ANSI
PRIVATE S16 rgSCHCmnSpsUlProcToBeActvtdUe
(
RgSchCellCb             *cell,
RgSchCmnUlRbAllocInfo   *allocInfo
)
#else /* ANSI */
PRIVATE S16 rgSCHCmnSpsUlProcToBeActvtdUe(cell, alloc)
RgSchCellCb             *cell;
RgSchCmnUlRbAllocInfo   *allocInfo;
#endif /* ANSI */
{
   Pst                   pst;
   RgInfUlSpsReset       ulSpsResetInfo;
   RgSchCmnUlUe          *ueUl = NULLP;
   RgSchCmnUlUeSpsInfo   *ulSpsUe;
   RgSchCmnUlCellSpsInfo *ulSpsCell = RG_SCH_CMN_GET_UL_SPS_CELL(cell);
   RgSchCmnUlCell  *cellUl = RG_SCH_CMN_GET_UL_CELL(cell);
   CmLList               *ueLnk;
   RgSchUeCb             *ue;
   RgSchCmnSpsUlSf       *spsSf;   /* UL SPS information of a subframe */
   CmLteTimingInfo       timeToTx; /* initial UL SPS transmission time */
   U16                   spsSfIdx; /* Index into the SPS Subframes Array */
   RgSchUlAlloc          tmpUeAlloc;

   TRC2(rgSCHCmnSpsUlProcToBeActvtdUe);

   cmMemset((U8*)&tmpUeAlloc, 0, sizeof(RgSchUlAlloc));

   /* Fetch the first UE waiting for UL SPS activation */
   ueLnk = cmLListFirst(&ulSpsCell->toBeActvtdUeLst);
   if ((ueLnk == NULLP) || ((ue = (RgSchUeCb *)(ueLnk->node)) == NULLP))
   {
      RETVALUE(ROK);
   }
   
   /* calculate the time to Transmit */
   timeToTx = cellUl->schdTime;

   for(;ueLnk; ueLnk= ueLnk->next)
   {
      if(cellUl->maxAllocPerUlSf <= *allocInfo->sf->allocCountRef)
      {
         RETVALUE(ROK);
      } 

      if((ue = (RgSchUeCb *)(ueLnk->node)) == NULLP)
      {
         continue;
      }

      ulSpsUe = RG_SCH_CMN_GET_UL_SPS_UE(ue);

       /* Check for Max SPS Activations for UL SF */
      if ((rgSCHCmnChkMaxSpsUePerSf (cell, ue, timeToTx)) == TRUE)
      {
         RLOG_ARG1(L_DEBUG,DBG_CELLID,cell->cellId,
                  "UEID:%d is Max UL SPS UE Per SF hit ",
                  ue->ueId);
         continue;
      }
      
      /* Check if UE is inactive for UL scheduling */
      if ((rgSCHCmnSpsUlChkUeInActv (cell, ue)) == TRUE)
      {
         continue;
      }
     
      /* Removed DRX ONDUR_ACTIVE check, as the DRX inactivity is already verified
       * in above rgSCHCmnSpsUlChkUeInActv() function call */      
 
      /* Fetch the SPS information corresponding to the subframe in which UL
       * transmission shall happen for the UE, if activation succeeds */
      spsSfIdx = ((timeToTx.sfn * RGSCH_NUM_SUB_FRAMES) + 
            (timeToTx.subframe)) % ulSpsCell->spsPrdLcmVal;
      spsSf = &(ulSpsCell->spsSfLst[spsSfIdx]);

      /* Try to allocate UE in this subframe */
      if ((rgSCHCmnSpsUlUeAlloc (cell, 
                      ue, spsSf, allocInfo, &tmpUeAlloc, timeToTx)) != ROK)
      {
 
         /* When allocation fails the accumulated BO should be
            cleared through dynamic Scheduling */
         ueUl = RG_SCH_CMN_GET_UL_UE(ue);

         ulSpsUe->state = RGSCH_SPS_IDLE;
         ulSpsUe->packetSize = 0;
         rgSCHCmnSpsUlDelUeFrmToBeActvLst(cell, ue);
         continue;
      }

      ulSpsResetInfo.crnti = ue->ueId;
      ulSpsResetInfo.cellId = cell->cellId;
      rgSCHUtlGetPstToLyr(&pst, &rgSchCb[cell->instIdx], cell->macInst);
      RgSchMacUlSpsReset(&pst, &ulSpsResetInfo);

      /* Mark the state of the UE to ACTV_SENT so that if no data comes, pdcch 
       * can be retransmitted at the next SPS occasion */
      ulSpsUe->state = RGSCH_SPS_ACTV_PDCCH_SENT;
      ulSpsUe->ulSpsSchdInfo.actvSfLstIdx = spsSfIdx;
      ulSpsUe->ulSpsSchdInfo.crntUlSpsOcc = timeToTx;
      ulSpsUe->ulSpsSchdInfo.ulSpsActvOcc = timeToTx;
      ulSpsUe->isUlSpsActv = TRUE;
      ulSpsUe->relPdcchSntCnt = 0;
      ue->ul.relPdcchSchdTime.subframe = 0xFF;
      rgSCHCmnSpsUlUtlCpyAlloc(&tmpUeAlloc, &ulSpsUe->ulSpsSchdInfo.allocInfo);

#ifdef RGSCH_SPS_STATS
      rgSchSpsUlSpsActDciTime[rgSchSpsNumUlSpsActSent% RGSCH_SPS_MAX_STATS]
         =  timeToTx;
      rgSchSpsNumUlSpsActSent++;
#endif

      /* Add UE to the cell's sps List */

      /* calculate the time to Transmit */
      /* Add UE to actvUlSpsUeLsts */
      rgSCHCmnSpsUlInsUeInActvLst(cell, ue, timeToTx);
      rgSCHCmnSpsUlDelUeFrmToBeActvLst(cell, ue);


      /*breaking here currently to process only one UE per TTI*/
      break;
   }

   RETVALUE(ROK);
} /* end of rgSCHCmnSpsUlProcToBeActvtdUe */

/**
 * @brief Handler to check if Ue is inactive for UL scheduling
 *
 * @details
 *
 *     Function : rgSCHCmnSpsUlChkUeInActv
 *
 *     Invoked by:
 *      - rgSCHCmnSpsUlProcToBeActvtdUe
 *        
 *     Processing Steps:
 *           if (ue->ulInactvMask)
 *              return TRUE
 *           if (ue->hdfdd)
 *              invoke isUlSchedUE() to check if UE can be scheduled in UL
 *           invoke ChkMeasGapColl if ue->measGap is enabled
 *              return TRUE if collides
 *  @param[in]  RgSchCellCb     *cell
 *  @param[in]  RgSchUeCb       *ue
 *  @return  Bool
 *       TRUE if UE is inactive
 *       FALSE otherwise
 **/

#ifdef ANSI
PRIVATE Bool rgSCHCmnSpsUlChkUeInActv
(
RgSchCellCb             *cell,
RgSchUeCb               *ue
)
#else /* ANSI */
PRIVATE Bool rgSCHCmnSpsUlChkUeInActv(cell, ue)
RgSchCellCb             *cell;
RgSchUeCb               *ue;
#endif /* ANSI */
{
   CmLteTimingInfo timeToTx;
#ifdef LTE_TDD
   RgSchCmnUlCell  *cellUl = RG_SCH_CMN_GET_UL_CELL(cell);
#endif

   TRC2(rgSCHCmnSpsUlChkUeInActv);


   /* Deleting the check for cfi change. If the UE is inactive
      and reactivation is pending, it will trigger activation PDCCH
      without considering the UE's state */

   /* calculate the time to Transmit */
#ifdef LTE_TDD
   timeToTx = cellUl->schdTime;
#else
   RG_SCH_ADD_TO_CRNT_TIME(cell->crntTime, timeToTx, RGSCH_SPS_UL_DELTA);
#endif

   /* Check if Ue is inactive for UL scheduling */
   if (ue->ul.ulInactvMask)
   {
      RETVALUE(TRUE);
   } /* end of (ue->inactvmask) */
 
   
   /* Check if measuring collides with UL SPS transmissions if activated */
   if ((ue->measGapCb.isMesGapEnabled == TRUE) &&
       (rgSCHCmnSpsUlChkMesGapColl(cell, ue, timeToTx,
               ue->measGapCb.gapPrd) == TRUE))
   {
      RETVALUE(TRUE);
   }

   RETVALUE(FALSE);   
} /* end of rgSCHCmnSpsUlChkUeInActv */

/**
 * @brief Handler to process TTI for  Uplink SPS 
 *
 * @details
 *
 *     Function : rgSCHCmnSpsUlTti
 *
 *     Invoked by:
 *      - rgSCHCmnTti before calling normal uplink scheduling 
 *        
 *     Processing Steps:
 *      invoke rgSCHCmnSpsUlProcActvtdUes to process all the UL-SPS activated
 *        UEs
 *      invoke rgSCHCmnSpsUlProcToBeRelUe to process the UE in the
 *        toBeRelUeLst
 *      invoke rgSCHCmnSpsUlProcToBeActvtdUe to process the UE in the
 *        toBeActvtdUeLst
 *      return ROK
 *  @param[in]  RgSchCellCb     *cell
 *  @return  Void
 **/

#ifdef ANSI
PUBLIC S16 rgSCHCmnSpsUlTti
(
RgSchCellCb             *cell,
RgSchCmnUlRbAllocInfo   *allocInfo
)
#else /* ANSI */
PUBLIC S16 rgSCHCmnSpsUlTti(cell, allocInfo)
RgSchCellCb             *cell;
RgSchCmnUlRbAllocInfo   *allocInfo;
#endif /* ANSI */
{

   TRC2(rgSCHCmnSpsUlTti);


   if (0 == cell->spsCellCfg.maxSpsUePerUlSf)
   {
      RETVALUE(ROK);
   }
   /* Process UL SPS activated UEs */ 
   rgSCHCmnSpsUlProcActvtdUes(cell, allocInfo);

   /* Process UE waiting for UL SPS Activation */ 
   if (NULLP != allocInfo)
   {
      /* Scheduling ULSPS Release only if it is a HIDCI0 subframe*/
      rgSCHCmnSpsUlProcToBeRelUe(cell);
      rgSCHCmnSpsUlProcToBeActvtdUe(cell, allocInfo);
   }


   RETVALUE(ROK);
} /* end of rgSCHCmnSpsUlTti */

#ifdef RG_UNUSED
/**
 * @brief Handler to send the list of UL SPS active UEs for a given time
 *
 * @details
 *
 *     Function : rgSCHCmnSpsUlGetActvUeLst
 *
 *     Invoked by:
 *      - HD-FDD module 
 *        
 *     Processing Steps:
 *       Fetch the actvUeLstIdx for the given timingInfo
 *          return (actvUeLst = &actvueLsts[actvUeLstIdx])
 *       return ROK
 *  @param[in]  RgSchCellCb     *cell
 *  @return  Void
 **/

#ifdef ANSI
PUBLIC S16 rgSCHCmnSpsUlGetActvUeLst 
(
RgSchCellCb     *cell,
CmLteTimingInfo timingInfo,
CmLListCp       *ulSpsActvUeLst
)
#else /* ANSI */
PUBLIC S16 rgSCHCmnSpsUlGetActvUeLst(cell, timingInfo, ulSpsActvUeLst)
RgSchCellCb     *cell;
CmLteTimingInfo timingInfo;
CmLListCp       *ulSpsActvUeLst;
#endif /* ANSI */
{
   RgSchCmnUlCellSpsInfo *ulSpsCell = RG_SCH_CMN_GET_UL_SPS_CELL(cell);
   U16                   actvUeLstIdx;

   TRC2(rgSCHCmnSpsUlGetActvUeLst);
  

   /* Fetch the list of UL SPS active UEs for the given timingInfo */
   actvUeLstIdx = ((timingInfo.sfn * RGSCH_NUM_SUB_FRAMES) + 
                      timingInfo.subframe) % RG_SCH_CMN_SPS_MAX_PRD;

   ulSpsActvUeLst = &ulSpsCell->actvUlSpsUeLsts[actvUeLstIdx];

   RETVALUE(ROK);
} /* end of rgSCHCmnSpsUlGetActvUeLst */
#endif /*RG_UNUSED*/

/************** UL SPS Utility Functions *************/

/**
 * @brief Handler to remove the SPS specific information in the UE
 *
 * @details
 *
 *     Function : rgSCHCmnSpsUlUtlFreeSpsUe
 *
 *     Invoked by:
 *      -rgSCHCmnSpsUlUeDel
 *      -rgSCHCmnSpsUlLcgDel
 *        
 *     Processing Steps: 
 *        Free measGapMask if allocated
 *        clear sps allocations of the UE
 *        Delete from lists
 *
 *  @param[in]  RgSchCellCb     *cell
 *  @param[in]  RgSchUeCb       *ue
 *  @return  Void
 **/

#ifdef ANSI
PRIVATE Void rgSCHCmnSpsUlUtlFreeSpsUe
(
RgSchCellCb     *cell,
RgSchUeCb       *ue
)
#else /* ANSI */
PRIVATE Void rgSCHCmnSpsUlUtlFreeSpsUe(cell, ue)
RgSchCellCb     *cell;
RgSchUeCb       *ue;
#endif /* ANSI */
{
    RgSchCmnUlCellSpsInfo *ulSpsCell = RG_SCH_CMN_GET_UL_SPS_CELL(cell);
    RgSchCmnUlUeSpsInfo   *ulSpsUe   = RG_SCH_CMN_GET_UL_SPS_UE(ue);
#ifndef LTE_TDD
    U8                    maxSubfrms = RG_SCH_CMN_UL_NUM_SF;
#endif
    U16                   actvOffst  = ulSpsUe->ulSpsSchdInfo.actvSfLstIdx;
    U16                   ulSfIdx;
    Pst                   pst;

    TRC2(rgSCHCmnSpsUlUtlFreeSpsUe);


    /* deregister the SPS LCs with MAC */
    rgSCHUtlGetPstToLyr(&pst, &rgSchCb[cell->instIdx], cell->macInst);
    RgSchMacSpsLcDereg(&pst, cell->cellId, ue->ueId);

    if (ulSpsUe->state == RGSCH_SPS_IDLE)
    {
        /* Nothing needs to be done */
        ue->ul.ulSpsCfg.isUlSpsEnabled = FALSE;
        RETVOID;
    }

    if (ulSpsUe->state == RGSCH_SPS_TO_BE_ACTV)
    {
        rgSCHCmnSpsUlDelUeFrmToBeActvLst(cell, ue);
        ue->ul.ulSpsCfg.isUlSpsEnabled = FALSE;
        RETVOID;
    }
    if(ulSpsUe->ulSpsSchdInfo.crntAlloc)
    {
        /*ccpu00139461 - Retrieving ulSfIdx is different for TDD and FDD*/
#ifndef LTE_TDD
        ulSfIdx = (ulSpsUe->ulSpsSchdInfo.crntUlSpsOcc.sfn * 
                    RGSCH_NUM_SUB_FRAMES + 
                    ulSpsUe->ulSpsSchdInfo.crntUlSpsOcc.subframe) % maxSubfrms;
#else
        ulSfIdx = rgSCHCmnGetUlSfIdx (&ulSpsUe->ulSpsSchdInfo.crntUlSpsOcc, cell);
#endif
        rgSCHCmnUlFreeAlloc(cell, ulSpsUe->ulSpsSchdInfo.crntAlloc);
        ulSpsUe->ulSpsSchdInfo.crntAlloc = NULLP;
        ulSpsUe->ulSpsSchdInfo.crntAllocSf = NULLP;
    }
    /* Free the allocations of the UE in cell wide sps sf tbl */
    rgSCHCmnUlSpsClrAlloc (cell, ue, &(ulSpsCell->spsSfLst[actvOffst]),
            &ulSpsUe->ulSpsSchdInfo.allocInfo);

    /* Remove Ue from sps list */
    /* Remove UE from actvtdUeLst */
    rgSCHCmnSpsUlDelUeFrmActvLst(cell, ue, ulSpsUe->ulSpsSchdInfo.crntUlSpsOcc);

    /* Remove Ue from other lists, if it exists */
    if (ulSpsUe->state == RGSCH_SPS_TO_BE_REACTV)
    {
        rgSCHCmnSpsUlDelUeFrmToBeActvLst(cell, ue);
    }
    else if (ulSpsUe->state == RGSCH_SPS_TO_BE_REL)
    {
        rgSCHCmnSpsUlDelUeFrmToBeRelLst(cell, ue);
    }
    ue->ul.ulSpsCfg.isUlSpsEnabled = FALSE;
    RETVOID;
} /* end of rgSCHCmnSpsUlUtlFreeSpsUe */

/**
 * @brief Handler to remove the SPS alloc information in the UE
 *
 * @details
 *
 *     Function : rgSCHCmnSpsUlUtlRmvSpsAlloc
 *
 *     Invoked by:
 *      -rgSCHCmnSpsUlUeDel
 *      -rgSCHCmnSpsUlLcgDel
 *        
 *     Processing Steps: 
 *        Free measGapMask if allocated
 *        clear sps allocations of the UE
 *        Delete from lists
 *
 *  @param[in]  RgSchCellCb     *cell
 *  @param[in]  RgSchUeCb       *ue
 *  @return  Void
 **/

#ifdef ANSI
PRIVATE Void rgSCHCmnSpsUlUtlRmvSpsAlloc
(
RgSchCellCb     *cell,
RgSchUeCb       *ue
)
#else /* ANSI */
PRIVATE Void rgSCHCmnSpsUlUtlRmvSpsAlloc(cell, ue)
RgSchCellCb     *cell;
RgSchUeCb       *ue;
#endif /* ANSI */
{
    Pst                   pst;
    RgInfUlSpsReset       ulSpsResetInfo;
    U32                    idx = 0;
    RgSchCmnUlCellSpsInfo *ulSpsCell = RG_SCH_CMN_GET_UL_SPS_CELL(cell);
    RgSchCmnUlUeSpsInfo   *ulSpsUe   = RG_SCH_CMN_GET_UL_SPS_UE(ue);
#ifndef LTE_TDD
    U8                    maxSubfrms = RG_SCH_CMN_UL_NUM_SF;
#endif
    U16                   actvOffst  = ulSpsUe->ulSpsSchdInfo.actvSfLstIdx;
    U16                   ulSfIdx ;

    TRC2(rgSCHCmnSpsUlUtlRmvSpsAlloc);

    ulSpsUe->pdcchToRetx = FALSE;
    ulSpsUe->actPdcchCrcFailCount = 0;
    ulSpsUe->spsOccIdx = 0;
    if (ulSpsUe->state == RGSCH_SPS_IDLE)
    {
        RETVOID;
    }

    if (ulSpsUe->state == RGSCH_SPS_TO_BE_ACTV)
    {
        rgSCHCmnSpsUlDelUeFrmToBeActvLst(cell, ue);
        ulSpsUe->state = RGSCH_SPS_IDLE;
        RETVOID;
    }

    ulSpsResetInfo.crnti = ue->ueId;
    ulSpsResetInfo.cellId = cell->cellId;
    rgSCHUtlGetPstToLyr(&pst, &rgSchCb[cell->instIdx], cell->macInst);
    RgSchMacUlSpsReset(&pst, &ulSpsResetInfo);

    if(ulSpsUe->ulSpsSchdInfo.crntAlloc)
    {
        /*ccpu00139461 - Retrieving ulSfIdx is different for TDD and FDD*/
#ifndef LTE_TDD
        ulSfIdx = (ulSpsUe->ulSpsSchdInfo.crntUlSpsOcc.sfn * 
                    RGSCH_NUM_SUB_FRAMES + 
                    ulSpsUe->ulSpsSchdInfo.crntUlSpsOcc.subframe) % maxSubfrms;
#else
        ulSfIdx = rgSCHCmnGetUlSfIdx (&ulSpsUe->ulSpsSchdInfo.crntUlSpsOcc, cell);
#endif
        rgSCHCmnUlFreeAlloc(cell, ulSpsUe->ulSpsSchdInfo.crntAlloc);
        ulSpsUe->ulSpsSchdInfo.crntAlloc = NULLP;
        ulSpsUe->ulSpsSchdInfo.crntAllocSf = NULLP;
    }
    /* Free the allocations of the UE in cell wide sps sf tbl */
    rgSCHCmnUlSpsClrAlloc (cell, ue, &(ulSpsCell->spsSfLst[actvOffst]),
          &ulSpsUe->ulSpsSchdInfo.allocInfo);

    for (idx=0; idx < RGSCH_SPS_ULBW_MASK_LEN; idx++)
    {
       ulSpsUe->ulSpsSchdInfo.allocBwMask[idx] = 0;
    }

    /* Remove UE from actvtdUeLst */
    rgSCHCmnSpsUlDelUeFrmActvLst(cell, ue, ulSpsUe->ulSpsSchdInfo.crntUlSpsOcc);

    /* Remove Ue from other lists, if it exists */
    if (ulSpsUe->state == RGSCH_SPS_TO_BE_REACTV)
    {
        rgSCHCmnSpsUlDelUeFrmToBeActvLst(cell, ue);
    }
    else if (ulSpsUe->state == RGSCH_SPS_TO_BE_REL)
    {
        rgSCHCmnSpsUlDelUeFrmToBeRelLst(cell, ue);
    }

   ulSpsUe->state = RGSCH_SPS_IDLE;
   ulSpsUe->isUlSpsActv = FALSE;
   ulSpsUe->crcFailCntSpsOcc = 0;
   ulSpsUe->relPdcchSntCnt = 0;
   ue->ul.relPdcchSchdTime.subframe = 0xFF;

    RETVOID;
} /* end of rgSCHCmnSpsUlUtlRmvSpsAlloc */


/**
 * @brief Get an alloc from the specified position in the BW. 
 *
 * @details
 *
 *     Function : rgSCHCmnSpsUtlUlGetSpfcAlloc
 *
 *      - Return an alloc from the specified position in the BW.
 *
 *      @param[in]  RgSchCellCb     *cell,
 *      @param[in]  RgSchCmnUlUe    *ueUl,
 *      @param[in]  RgSchUlSf       *sf,
 *      @param[in]  U8              startSb,
 *      @param[in]  U8              numSb,
 *      @param[in]  U8              cfi
 *
 *  @return  RgSchUlAlloc*
 **/
#ifdef ANSI
PRIVATE RgSchUlAlloc *rgSCHCmnSpsUtlUlGetSpfcAlloc
(
RgSchCellCb     *cell,
RgSchCmnUlUe    *ueUl,
RgSchUlSf       *sf,
U8              startSb,
U8              numSb,
U8              cfi
)
#else /* ANSI */
PRIVATE RgSchUlAlloc *rgSCHCmnSpsUtlUlGetSpfcAlloc(cell, ueUl, sf, startSb, 
                        numSb, cfi)
RgSchCellCb     *cell;
RgSchCmnUlUe    *ueUl;
RgSchUlSf       *sf;
U8              startSb;
U8              numSb;
U8              cfi;
#endif 
{
   RgSchUlHole     *hole, *nxtHole;
   RgSchUlAlloc    *alloc = NULLP;
   
   TRC2(rgSCHCmnSpsUtlUlGetSpfcAlloc);

   if (!(hole = rgSCHUtlUlHoleFirst(sf)))
   {
      RETVALUE(NULLP);
   }

   if(*sf->allocCountRef == 0)
   {  
      /* Sanity check of holeDb */
      if (sf->holeDb->count == 1 && hole->start == 0)
      {
         hole->num = cell->dynCfiCb.bwInfo[cfi].numSb;   
      }
      else
      {
         RLOG_ARG0(L_ERROR,DBG_CELLID,cell->cellId,
                  "Error! holeDb sanity check failed");
      }
   } 

   do
   {
      nxtHole = rgSCHUtlUlHoleNxt(sf, hole);
      if ((startSb >= hole->start) &&
          (startSb+numSb <= hole->start+hole->num))
      {
         if (startSb != hole->start)
         {
            /* Create a new hole to accomodate Subbands between
             * hole start and req alloc start */
            RgSchUlHole *newHole = rgSCHUtlUlHoleMemGet(&(sf->holeDb->mem));

#if (ERRCLASS & ERRCLS_DEBUG)
            if (newHole == NULLP )
               RETVALUE(NULLP);
#endif
            newHole->start = hole->start;
            newHole->num = startSb - hole->start;
            hole->start = startSb;
            hole->num -= newHole->num;
            ++(sf->holeDb->count);
            rgSCHUtlUlHoleIns(sf->holeDb, newHole);
            newHole->prvAlloc = hole->prvAlloc;
            if (newHole->prvAlloc) 
            {
               newHole->prvAlloc->nxtHole = newHole;
            }
            alloc = rgSCHCmnUlSbAlloc(sf, numSb, hole);
            alloc->prvHole = newHole;
            newHole->nxtAlloc = alloc;
         }
         else /* Hole start and req alloc start are same */
         {
            alloc = rgSCHCmnUlSbAlloc(sf, numSb, hole);
         }
         ueUl->lastCfi = cfi;
         break;
      }
   } while ((hole = nxtHole));
   RETVALUE(alloc);
} /* end of rgSCHCmnSpsUtlUlGetSpfcAlloc */
/**
 * @brief Handler to copy source allocation to destination allocation
 *
 * @details
 *
 *     Function : rgSCHCmnSpsUlUtlCpyAlloc
 *
 *     Invoked by:
 *       rgSCHCmnSpsUlProcActvtdUes
 *       rgSCHCmnSpsUlProcToBeReactvtdUe 
 *       rgSCHCmnSpsUlProcToBeActvtdUe 
 *        
 *     Processing Steps:
 *        Copy the entire source allocation to destination allocation
 *         
 *
 *  @param[in]  RgSchUlAlloc         *srcAlloc
 *  @param[in]  RgSchUlAlloc         *dstAlloc
 *  @return  S16
 **/

#ifdef ANSI
PRIVATE S16 rgSCHCmnSpsUlUtlCpyAlloc
(
RgSchUlAlloc         *srcAlloc,
RgSchUlAlloc         *dstAlloc 
)
#else /* ANSI */
PRIVATE S16 rgSCHCmnSpsUlUtlCpyAlloc(srcAlloc, dstAlloc)
RgSchUlAlloc         *srcAlloc;
RgSchUlAlloc         *dstAlloc;
#endif /* ANSI */
{
   TRC2(rgSCHCmnSpsUlUtlCpyAlloc);
   cmMemcpy((U8*)dstAlloc, (U8*)srcAlloc, sizeof(RgSchUlAlloc));
   dstAlloc->pdcch          = NULLP;
  
   RETVALUE(ROK); 
} /* end of rgSCHCmnSpsUlUtlCpyAlloc */


/**
 * @brief Handler to fill release pdcch to be sent for a UE 
 *
 * @details
 *
 *     Function : rgSCHCmnSpsUlUtlFillRelPdcch
 *
 *     Invoked by:
 *           rgSCHCmnSpsUlProcToBeRelUe
 *        
 *     Processing Steps:
 *       Fill pdcch to reflect SPS release
 *       return ROK
 *
 *  @param[in]  RgSchCellCb             *cell
 *  @param[in]  RgSchPdcch              *pdcch
 *  @return  S16
 **/

#ifdef ANSI
PRIVATE S16 rgSCHCmnSpsUlUtlFillRelPdcch
(
RgSchCellCb             *cell,
RgSchUeCb               *ue,
RgSchPdcch              *pdcch
)
#else /* ANSI */
PRIVATE S16 rgSCHCmnSpsUlUtlFillRelPdcch(cell, ue, pdcch)
RgSchCellCb             *cell;
RgSchUeCb               *ue;
RgSchPdcch              *pdcch;
#endif /* ANSI */
{
#ifdef LTE_TDD
   RgSchTddANInfo    *anInfo;
   RgSchDlSf         *dlsf;
   CmLteTimingInfo   timeInfo = ue->cell->crntTime;
#endif /*LTE_TDD*/
   
   TRC2(rgSCHCmnSpsUlUtlFillRelPdcch);

#ifdef RGSCH_SPS_STATS
   rgSchSpsUlSpsRelPdcchAllocTime[rgSchSpsNumExplRelPdcchAlloc % RGSCH_SPS_MAX_STATS]
      =  cell->crntTime;
   rgSchSpsUlSpsRelPdcchAllocUE[rgSchSpsNumExplRelPdcchAlloc % RGSCH_SPS_MAX_STATS]
      = ue->ueId;
   rgSchSpsNumExplRelPdcchAlloc++;
#endif

   /*_intg_fix_*/
   pdcch->dci.u.format0Info.hoppingEnbld = 0;
   pdcch->dci.dciFormat = TFU_DCI_FORMAT_0;
   /* SPS RNTI in pdcch */
   pdcch->rnti = ue->spsRnti;
   pdcch->dci.u.format0Info.tpcCmd = 0;
   pdcch->dci.u.format0Info.ndi    = 0;
   pdcch->dci.u.format0Info.nDmrs  = 0;
   /* MCS set to all 1's */
   pdcch->dci.u.format0Info.mcs    = 0x1F;
   /* Set all bits of RIV to 1's */
#ifdef TFU_UPGRADE
   pdcch->dci.u.format0Info.riv    = 0xFFFFFFFF;
#endif /*TFU_UPGRADE*/
   pdcch->dci.u.format0Info.rbStart = 0xFF;

   RLOG_ARG2(L_DEBUG,DBG_CELLID,cell->cellId,
            "UL SPS EXPLICIT RELEASE Pdcch is scheduled for UEID:%d "
            "SPS-RNTI:%d", 
            ue->ueId, 
            ue->spsRnti);

#ifdef LTE_TDD

   RGSCHCMNADDTOCRNTTIME(ue->cell->crntTime, timeInfo, TFU_ULCNTRL_DLDELTA)

   /* Get DL subframe for the current time*/
   dlsf = rgSCHUtlSubFrmGet(ue->cell, timeInfo);

   /* Get ACK/NACK feedback time with respect to this current time */

   /* ccpu00132272 -MOD- for all the UL-DL configs 1-6, the uplink-association 
    * index timing should be used for calculating dai.*/
   if(dlsf->ulAscInfo.subframe != RGSCH_INVALID_INFO)
   {
      timeInfo.sfn = 
         (timeInfo.sfn + dlsf->ulAscInfo.sfnOffset) % RGSCH_MAX_SFN;
      timeInfo.subframe = dlsf->ulAscInfo.subframe;
   }
   else
   {
      RETVALUE(ROK);
   }
   if(ue->cell->ulDlCfgIdx == 0)
   {
      pdcch->dci.u.format0Info.ulIdx = RG_SCH_ULIDX_MSB;
   }
   else
   {
      anInfo = rgSCHUtlGetUeANFdbkInfo(ue, &timeInfo); 
      if(anInfo != NULLP)
      {
         pdcch->dci.u.format0Info.dai = RG_SCH_GET_DAI_VALUE(anInfo->ulDai);
      }
      else
      {
         /* Fixing DAI value - ccpu00109162 */
         /* Set the no of DL subframes to zero with  DAI=3 */
         pdcch->dci.u.format0Info.dai = RG_SCH_MAX_DAI_IDX;
      }
   }
#endif /*LTE_TDD*/

   RETVALUE(ROK);
} /* end of rgSCHCmnSpsUlUtlFillRelPdcch */

/**
 * @brief Handler to insert Ue into ToBeRelUeLst
 *
 * @details
 *
 *     Function : rgSCHCmnSpsUlUtlInsUeInToBeRelLst
 *
 *     Invoked by:
 *           rgSCHCmnSpsUlProcActvtdUes
 *           rgSCHCmnSpsUlProcToBeRelUe
 *        
 *     Processing Steps:
 *       Insert UE into tobeRelUeLst
 *       return ROK
 *
 *  @param[in]  RgSchCellCb      *cell
 *  @param[in]  RgSchUeCb        *ue
 *  @return  S16
 **/

#ifdef ANSI
PRIVATE S16 rgSCHCmnSpsUlUtlInsUeInToBeRelLst
(
RgSchCellCb           *cell,
RgSchUeCb             *ue
)
#else /* ANSI */
PRIVATE S16 rgSCHCmnSpsUlUtlInsUeInToBeRelLst(cell, ue)
RgSchCellCb           *cell;
RgSchUeCb             *ue;
#endif /* ANSI */
{
   RgSchCmnUlCellSpsInfo *ulSpsCell = RG_SCH_CMN_GET_UL_SPS_CELL(cell);
   RgSchCmnUlUeSpsInfo   *ulSpsUe  = RG_SCH_CMN_GET_UL_SPS_UE(ue);
   
   TRC2(rgSCHCmnSpsUlUtlInsUeInToBeRelLst);
   
//#if (ERRCLASS & ERRCLS_DEBUG)
#ifdef RGSCH_SPS_STATS
   if ((PTR)ulSpsUe->pndngUeLstEnt.node != NULLP)
   {
      RG_SCH_ABORT();
   }
#endif
   if (ulSpsUe->pndngUeLstEnt.node == NULLP)
   {   
      ulSpsUe->pndngUeLstEnt.node = (PTR)ue;
      cmLListAdd2Tail(&ulSpsCell->toBeRelUeLst, &ulSpsUe->pndngUeLstEnt);
      ulSpsUe->spsList = &ulSpsCell->toBeRelUeLst;

   }

   RETVALUE(ROK);
} /* end of rgSCHCmnSpsUlUtlInsUeInToBeRelLst */

/**
 * @brief Handler to insert Ue into ToBeActvUeLst
 *
 * @details
 *
 *     Function : rgSCHCmnSpsUlUtlInsUeInToBeActvLst
 *
 *     Invoked by:
 *           rgSCHCmnSpsBsrRpt
 *           rgSCHCmnSpsUlProcToBeReactvtdUe
 *           rgSCHCmnSpsUlProcToBeActvtdUe
 *        
 *     Processing Steps:
 *       Insert UE into tobeActvUelst
 *       return ROK
 *
 *  @param[in]  RgSchCellCb      *cell
 *  @param[in]  RgSchUeCb        *ue
 *  @return  S16
 **/

#ifdef ANSI
PRIVATE S16 rgSCHCmnSpsUlUtlInsUeInToBeActvLst
(
RgSchCellCb           *cell,
RgSchUeCb             *ue
)
#else /* ANSI */
PRIVATE S16 rgSCHCmnSpsUlUtlInsUeInToBeActvLst(cell, ue)
RgSchCellCb           *cell;
RgSchUeCb             *ue;
#endif /* ANSI */
{
   RgSchCmnUlCellSpsInfo *ulSpsCell = RG_SCH_CMN_GET_UL_SPS_CELL(cell);
   RgSchCmnUlUeSpsInfo   *ulSpsUe  = RG_SCH_CMN_GET_UL_SPS_UE(ue);

   TRC2 (rgSCHCmnSpsUlUtlInsUeInToBeActvLst);

//#if (ERRCLASS & ERRCLS_DEBUG)
#ifdef RGSCH_SPS_STATS
   if (ulSpsUe->pndngUeLstEnt.node != NULLP)
   {
      RG_SCH_ABORT();
   }
#endif

   if (ulSpsUe->pndngUeLstEnt.node == NULLP)
   {
      ulSpsUe->pndngUeLstEnt.node = (PTR)ue;
      cmLListAdd2Tail (&ulSpsCell->toBeActvtdUeLst, &ulSpsUe->pndngUeLstEnt); 
      ulSpsUe->spsList = &ulSpsCell->toBeActvtdUeLst;
   }

   RETVALUE(ROK);   
} /* end of rgSCHCmnSpsUlUtlInsUeInToBeActvLst */

/**
 * @brief Handler to insert Ue into ActvUeLst
 *
 * @details
 *
 *     Function : rgSCHCmnSpsUlInsUeInActvLst
 *
 *     Invoked by:
 *           rgSCHCmnSpsUlProcActvtdUes
 *           rgSCHCmnSpsUlProcToBeActvtdUe
 *        
 *     Processing Steps:
 *       Insert UE into ActvUelst based on time
 *       return ROK
 *
 *  @param[in]  RgSchCellCb      *cell
 *  @param[in]  RgSchUeCb        *ue
 *  @param[in]  CmLteTimingInfo  spsOcc
 *  @return  S16
 **/

#ifdef ANSI
PRIVATE S16 rgSCHCmnSpsUlInsUeInActvLst
(
RgSchCellCb           *cell,
RgSchUeCb             *ue,
CmLteTimingInfo       spsOcc
)
#else /* ANSI */
PRIVATE S16 rgSCHCmnSpsUlInsUeInActvLst(cell, ue, spsOcc)
RgSchCellCb           *cell;
RgSchUeCb             *ue;
CmLteTimingInfo       spsOcc;
#endif /* ANSI */
{
   RgSchCmnUlCellSpsInfo *ulSpsCell = RG_SCH_CMN_GET_UL_SPS_CELL(cell);
   RgSchCmnUlUeSpsInfo   *ulSpsUe = RG_SCH_CMN_GET_UL_SPS_UE(ue);
   U16                   actvUeLstIdx;

   TRC2 (rgSCHCmnSpsUlInsUeInActvLst);

   /* Compute actvlstidx and insert */
   actvUeLstIdx = (((spsOcc.sfn * RGSCH_NUM_SUB_FRAMES) + spsOcc.subframe)) % 
                                           (RG_SCH_CMN_SPS_MAX_PRD);

   ulSpsUe->actvUeLstEnt.node = (PTR)ue;
   /* Add UE to actvUlSpsUeLsts at the selected index */
   cmLListAdd2Tail (&ulSpsCell->actvUlSpsUeLsts[actvUeLstIdx],
                                                &ulSpsUe->actvUeLstEnt);
   RETVALUE(ROK);
} /* end of rgSCHCmnSpsUlInsUeInActvLst */

/**
 * @brief Handler to delete Ue from ToBeActvUeLst
 *
 * @details
 *
 *     Function : rgSCHCmnSpsUlDelUeFrmToBeActvLst
 *
 *     Invoked by:
 *           rgSCHCmnSpsUlUtlFreeSpsUe
 *        
 *     Processing Steps:
 *       Delete UE from  ToBeActvUelst
 *       return ROK
 *
 *  @param[in]  RgSchCellCb      *cell
 *  @param[in]  RgSchUeCb        *ue
 *  @return  S16
 **/

#ifdef ANSI
PRIVATE S16 rgSCHCmnSpsUlDelUeFrmToBeActvLst
(
RgSchCellCb           *cell,
RgSchUeCb             *ue
)
#else /* ANSI */
PRIVATE S16 rgSCHCmnSpsUlDelUeFrmToBeActvLst(cell, ue)
RgSchCellCb           *cell;
RgSchUeCb             *ue;
#endif /* ANSI */
{
   RgSchCmnUlCellSpsInfo *ulSpsCell = RG_SCH_CMN_GET_UL_SPS_CELL(cell);
   RgSchCmnUlUeSpsInfo   *ulSpsUe = RG_SCH_CMN_GET_UL_SPS_UE(ue);
   
   TRC2(rgSCHCmnSpsUlDelUeFrmToBeActvLst);

   if (ulSpsUe->pndngUeLstEnt.node)
   {
//#if (ERRCLASS & ERRCLS_DEBUG)
#ifdef RGSCH_SPS_STATS
      if (ulSpsUe->spsList != &ulSpsCell->toBeActvtdUeLst)
      {
         RG_SCH_ABORT();
      }
#endif
      cmLListDelFrm (&ulSpsCell->toBeActvtdUeLst, &ulSpsUe->pndngUeLstEnt);
      ulSpsUe->pndngUeLstEnt.node = NULLP;
      ulSpsUe->spsList = NULLP;
   }

   RETVALUE(ROK);
} /* end of rgSCHCmnSpsUlDelUeFrmToBeActvLst */

/**
 * @brief Handler to delete Ue from ToBeRelUeLst
 *
 * @details
 *
 *     Function : rgSCHCmnSpsUlDelUeFrmToBeRelLst
 *
 *     Invoked by:
 *           rgSCHCmnSpsUlUtlFreeSpsUe
 *        
 *     Processing Steps:
 *       Delete UE from  ToBeRelUelst
 *       return ROK
 *
 *  @param[in]  RgSchCellCb      *cell
 *  @param[in]  RgSchUeCb        *ue
 *  @return  S16
 **/

#ifdef ANSI
PRIVATE S16 rgSCHCmnSpsUlDelUeFrmToBeRelLst
(
RgSchCellCb           *cell,
RgSchUeCb             *ue
)
#else /* ANSI */
PRIVATE S16 rgSCHCmnSpsUlDelUeFrmToBeRelLst(cell, ue)
RgSchCellCb           *cell;
RgSchUeCb             *ue;
#endif /* ANSI */
{
   RgSchCmnUlCellSpsInfo *ulSpsCell = RG_SCH_CMN_GET_UL_SPS_CELL(cell);
   RgSchCmnUlUeSpsInfo   *ulSpsUe = RG_SCH_CMN_GET_UL_SPS_UE(ue);
   
   TRC2(rgSCHCmnSpsUlDelUeFrmToBeRelLst);

   if (ulSpsUe->pndngUeLstEnt.node)
   {
//#if (ERRCLASS & ERRCLS_DEBUG)
#ifdef RGSCH_SPS_STATS
      if (ulSpsUe->spsList != &ulSpsCell->toBeRelUeLst)
      {
         RG_SCH_ABORT();
      }
#endif
      cmLListDelFrm (&ulSpsCell->toBeRelUeLst, &ulSpsUe->pndngUeLstEnt);
      ulSpsUe->pndngUeLstEnt.node = NULLP;
      ulSpsUe->spsList = NULLP;
   }

   RETVALUE(ROK);
} /* end of rgSCHCmnSpsUlDelUeFrmToBeRelLst */

/**
 * @brief Handler to delete Ue from actvLst
 *
 * @details
 *
 *     Function : rgSCHCmnSpsUlDelUeFrmActvLst
 *
 *     Invoked by:
 *           rgSCHCmnSpsUlProcActvtdUes
 *           rgSCHCmnSpsUlProcToBeActvtdUe
 *           rgSCHCmnSpsUlUtlFreeSpsUe
 *        
 *     Processing Steps:
 *       Delete UE from ActvUelst based on time
 *       return ROK
 *
 *  @param[in]  RgSchCellCb      *cell
 *  @param[in]  RgSchUeCb        *ue
 *  @param[in]  CmLteTimingInfo  spsOcc
 *  @return  S16
 **/

#ifdef ANSI
PRIVATE S16 rgSCHCmnSpsUlDelUeFrmActvLst
(
RgSchCellCb           *cell,
RgSchUeCb             *ue,
CmLteTimingInfo       spsOcc
)
#else /* ANSI */
PRIVATE S16 rgSCHCmnSpsUlDelUeFrmActvLst(cell, ue, spsOcc)
RgSchCellCb           *cell;
RgSchUeCb             *ue;
CmLteTimingInfo       spsOcc;
#endif /* ANSI */
{
   RgSchCmnUlCellSpsInfo *ulSpsCell = RG_SCH_CMN_GET_UL_SPS_CELL(cell);
   RgSchCmnUlUeSpsInfo   *ulSpsUe = RG_SCH_CMN_GET_UL_SPS_UE(ue);
   U16                   actvUeLstIdx;

   TRC2 (rgSCHCmnSpsUlDelUeFrmActvLst);

   /* Compute actvlstidx and delete */
   actvUeLstIdx = (((spsOcc.sfn * RGSCH_NUM_SUB_FRAMES) + spsOcc.subframe)) % 
                                           (RG_SCH_CMN_SPS_MAX_PRD);

/*   ulSpsUe->actvUeLstEnt.node = (PTR)ue;*/
   /* Delete UE from actvUlSpsUeLsts at the selected index */
   if (ulSpsUe->actvUeLstEnt.node)
   {
      cmLListDelFrm (&ulSpsCell->actvUlSpsUeLsts[actvUeLstIdx], 
            &ulSpsUe->actvUeLstEnt);
      ulSpsUe->actvUeLstEnt.node = NULLP;
   }
   RETVALUE(ROK);
} /* end of rgSCHCmnSpsUlDelUeFrmActvLst */

/**
 * @brief Handler to free tobeActvUeLst
 *
 * @details
 *
 *     Function : rgSCHCmnSpsUlFreeToBeActvUeLst
 *
 *     Invoked by:
 *           rgSCHCmnSpsUlCellDel
 *        
 *     Processing Steps:
 *       Delete  all the UEs from ToBeActvUelst
 *       return ROK
 *
 *  @param[in]  RgSchCellCb      *cell
 *  @return  S16
 **/

#ifdef ANSI
PRIVATE S16 rgSCHCmnSpsUlFreeToBeActvUeLst
(
RgSchCellCb           *cell
)
#else /* ANSI */
PRIVATE S16 rgSCHCmnSpsUlFreeToBeActvUeLst(cell)
RgSchCellCb           *cell;
#endif /* ANSI */
{
   RgSchCmnUlCellSpsInfo *ulSpsCell = RG_SCH_CMN_GET_UL_SPS_CELL(cell);
   CmLList               *ueLnk;

   TRC2 (rgSCHCmnSpsUlFreeToBeActvUeLst);

   /* Delete all the UEs from toBeActvtdueLst */
   while ((ueLnk = cmLListFirst(&ulSpsCell->toBeActvtdUeLst)) != NULLP)
   {
      cmLListDelFrm(&ulSpsCell->toBeActvtdUeLst, ueLnk);
   }

   RETVALUE(ROK);
} /* end of rgSCHCmnSpsUlFreeToBeActvUeLst */


/**
 * @brief Handler to free tobeRelUeLst
 *
 * @details
 *
 *     Function : rgSCHCmnSpsUlFreeToBeRelUeLst
 *
 *     Invoked by:
 *           rgSCHCmnSpsUlCellDel
 *        
 *     Processing Steps:
 *       Delete all the UEs from ToBeRelUeLst
 *       return ROK
 *
 *  @param[in]  RgSchCellCb      *cell
 *  @return  S16
 **/

#ifdef ANSI
PRIVATE S16 rgSCHCmnSpsUlFreeToBeRelUeLst
(
RgSchCellCb           *cell
)
#else /* ANSI */
PRIVATE S16 rgSCHCmnSpsUlFreeToBeRelUeLst(cell)
RgSchCellCb           *cell;
#endif /* ANSI */
{
   RgSchCmnUlCellSpsInfo *ulSpsCell = RG_SCH_CMN_GET_UL_SPS_CELL(cell);
   CmLList               *ueLnk;

   TRC2 (rgSCHCmnSpsUlFreeToBeRelUeLst);

   /* delete all the UEs from toBeRelUeLst */
   while ((ueLnk = cmLListFirst(&ulSpsCell->toBeRelUeLst)) != NULLP)
   {
      cmLListDelFrm(&ulSpsCell->toBeRelUeLst, ueLnk);
   }

   RETVALUE(ROK);
} /* end of rgSCHCmnSpsUlFreeToBeRelUeLst */

/**
 * @brief Handler to free actvUeLsts
 *
 * @details
 *
 *     Function : rgSCHCmnSpsUlFreeActvUeLst
 *
 *     Invoked by:
 *            rgSCHCmnSpsUlCellDel
 *        
 *     Processing Steps:
 *       Delete all the UEs from all ActvUeLsts in the cell
 *       return ROK
 *
 *  @param[in]  RgSchCellCb      *cell
 *  @return  S16
 **/

#ifdef ANSI
PRIVATE S16 rgSCHCmnSpsUlFreeActvUeLst
(
RgSchCellCb           *cell
)
#else /* ANSI */
PRIVATE S16 rgSCHCmnSpsUlFreeActvUeLst(cell)
RgSchCellCb           *cell;
#endif /* ANSI */
{
   RgSchCmnUlCellSpsInfo *ulSpsCell = RG_SCH_CMN_GET_UL_SPS_CELL(cell);
   CmLList               *ueLnk;
   U16                   idx;

   TRC2 (rgSCHCmnSpsUlFreeActvUeLst);

   /* Delete all the Ues from actvUlSpsUeLsts */
   for (idx=0; idx<RG_SCH_CMN_SPS_MAX_PRD; idx++)
   {
      while ((ueLnk = cmLListFirst(&ulSpsCell->actvUlSpsUeLsts[idx])) != NULLP)
      {
         cmLListDelFrm (&ulSpsCell->actvUlSpsUeLsts[idx], ueLnk);
         ueLnk->node = NULLP;  
      }
   }

   RETVALUE(ROK);
} /* end of rgSCHCmnSpsUlFreeActvUeLst */

/** @brief This function is called to fill activation PDCCH for DCI format 1
 *  
 *  @details
 *  Invoked by - rgSCHCmnSpsDlFillActvPdcch
 *  
 *  Function:  rgSCHCmnSpsDlFillActvPdcchDciFrmt1
 *       
 *  Processing steps:
 *  Fills up the activation/re-activation PDCCH for SPS UE.
 *
 *  @param  RgSchCellCb         *cellCb
 *  @param  RgSchCmnDlUeSpsInfo *dlSpsUe    
 *  @param  RgSchDlRbAlloc      *rbAllocInfo
 *  @param  RgSchPdcch           *pdcch
 *  @param  Bool                 isSpsOcc
 *  @param  U8                   hqProcId
 *  @return Void 
 *  
 **/
#ifdef ANSI
PRIVATE Void rgSCHCmnSpsDlFillActvPdcchDciFrmt1
(
RgSchCellCb                *cell,
RgSchCmnDlUeSpsInfo        *dlSpsUe,
RgSchDlRbAlloc             *rbAllocInfo,
RgSchPdcch                 *pdcch,
Bool                       isSpsOcc,
U8                         hqProcId
)
#else /* ANSI */
PRIVATE  Void rgSCHCmnSpsDlFillActvPdcchDciFrmt1(cell, dlSpsUe, rbAllocInfo,
        pdcch, isSpsOcc, hqProcId)
RgSchCellCb                *cell;
RgSchCmnDlUeSpsInfo        *dlSpsUe;
RgSchDlRbAlloc             *rbAllocInfo;
RgSchPdcch                 *pdcch;
Bool                       isSpsOcc;
U8                         hqProcId;
#endif /* ANSI */
{
   U8 tpc;
   U32 allocBitMask;
#ifdef LTE_TDD
   RgSchTddANInfo     *anInfo;
   RgSchDlHqProcCb    *hqP = rbAllocInfo->tbInfo[0].tbCb->hqP;
#endif
   
   tpc = dlSpsUe->dlSpsUeSchdInfo.allocN1PucchIdx;
   allocBitMask = 0;

   TRC2(rgSCHCmnSpsDlFillActvPdcchDciFrmt1)
   if (!isSpsOcc)
   {
       rgSCHCmnFillHqPTb(cell, rbAllocInfo, 0, pdcch);
   }
   else
   {
       rgSCHCmnFillHqPTb(cell, rbAllocInfo, 0, NULLP);
   }

   pdcch->dci.u.format1Info.tpcCmd = tpc;
   if (rbAllocInfo->raType == RG_SCH_CMN_RA_TYPE0)
   {
       /* Fill-up for resource allocation type 0 */
       pdcch->dci.u.format1Info.allocInfo.isAllocType0 = TRUE;
       allocBitMask =  rbAllocInfo->allocInfo.raType0.dlAllocBitMask;
   }
   else
   {
       /* Fill-up for resource allocation type 1 */
       pdcch->dci.u.format1Info.allocInfo.isAllocType0 = FALSE;
       allocBitMask =  rbAllocInfo->allocInfo.raType1.dlAllocBitMask;
   }
   pdcch->dci.u.format1Info.allocInfo.resAllocMap[0] = 
               ((allocBitMask >> 24) & 0xff);
   pdcch->dci.u.format1Info.allocInfo.resAllocMap[1] = 
               ((allocBitMask >> 16) & 0x00ff);
   pdcch->dci.u.format1Info.allocInfo.resAllocMap[2] = 
               ((allocBitMask >> 8) & 0x0000ff);
   pdcch->dci.u.format1Info.allocInfo.resAllocMap[3] = 
               ((allocBitMask & 0x000000ff));
   pdcch->dci.u.format1Info.allocInfo.harqProcId = hqProcId;
   pdcch->dci.u.format1Info.allocInfo.ndi = 0;
   pdcch->dci.u.format1Info.allocInfo.mcs =
                                rbAllocInfo->tbInfo[0].tbCb->dlGrnt.iMcs;
   pdcch->dci.u.format1Info.allocInfo.rv = 0;


#ifdef LTE_TDD
   if(hqP->hqE->ue != NULLP)
   {
      anInfo = rgSCHUtlGetUeANFdbkInfo(hqP->hqE->ue,
            &(rbAllocInfo->tbInfo[0].tbCb->fdbkTime));
#ifdef TFU_TDD
      if(anInfo)
      {
         pdcch->dci.u.format1Info.dai = RG_SCH_GET_DAI_VALUE(anInfo->dlDai);
      }
      else
      {
         pdcch->dci.u.format1Info.dai = RG_SCH_MAX_DAI_IDX;
         RLOG_ARG1(L_ERROR,DBG_CELLID,cell->cellId,
                  "PDCCH is been scheduled without updating anInfo CRNTI:%d",
                  pdcch->rnti);
      }
#endif
   }
#endif

   RETVOID;
}
/** @brief This function is called to fill activation PDCCH for DCI format 1A
 *  
 *  @details
 *  Invoked by - rgSCHCmnSpsDlFillActvPdcch
 *  
 *  Function:  rgSCHCmnSpsDlFillActvPdcchDciFrmt1A
 *       
 *  Processing steps:
 *  Fills up the activation/re-activation PDCCH for SPS UE.
 *
 *  @param  RgSchCellCb         *cellCb
 *  @param  RgSchCmnDlUeSpsInfo *dlSpsUe    
 *  @param  RgSchDlRbAlloc      *rbAllocInfo
 *  @param  RgSchPdcch           *pdcch
 *  @param  Bool                 isSpsOcc
 *  @param  U8                   HqProcId
 *   @return Void
 **/
#ifdef ANSI
PRIVATE Void rgSCHCmnSpsDlFillActvPdcchDciFrmt1A
(
RgSchCellCb                *cell,
RgSchCmnDlUeSpsInfo        *dlSpsUe,
RgSchDlRbAlloc             *rbAllocInfo,
RgSchPdcch                 *pdcch,
Bool                       isSpsOcc,
U8                         hqProcId
)
#else /* ANSI */
PRIVATE  Void rgSCHCmnSpsDlFillActvPdcchDciFrmt1A(cell, dlSpsUe, rbAllocInfo,
        pdcch,isSpsOcc, hqProcId)
RgSchCellCb                *cell;
RgSchCmnDlUeSpsInfo        *dlSpsUe;
RgSchDlRbAlloc             *rbAllocInfo;
RgSchPdcch                 *pdcch;
Bool                       isSpsOcc;
U8                         hqProcId;
#endif /* ANSI */
{
   U8 tpc;
#ifdef LTE_TDD
   RgSchTddANInfo     *anInfo;
   RgSchDlHqProcCb    *hqP = rbAllocInfo->tbInfo[0].tbCb->hqP;
#endif

   tpc = dlSpsUe->dlSpsUeSchdInfo.allocN1PucchIdx;

   TRC2(rgSCHCmnSpsDlFillActvPdcchDciFrmt1A)
    if (!isSpsOcc)
    {
        rgSCHCmnFillHqPTb(cell, rbAllocInfo, 0, pdcch);
    }
    else
    {
        rgSCHCmnFillHqPTb(cell, rbAllocInfo, 0, NULLP);
    }
   pdcch->dci.u.format1aInfo.isPdcchOrder = FALSE;
   pdcch->dci.u.format1aInfo.t.pdschInfo.tpcCmd  = tpc;
   pdcch->dci.u.format1aInfo.t.pdschInfo.allocInfo.mcs     = \
                         rbAllocInfo->tbInfo[0].tbCb->dlGrnt.iMcs;
   pdcch->dci.u.format1aInfo.t.pdschInfo.allocInfo.harqProcId.pres = \
                         TRUE;
   pdcch->dci.u.format1aInfo.t.pdschInfo.allocInfo.harqProcId.val = \
                         hqProcId;
   pdcch->dci.u.format1aInfo.t.pdschInfo.allocInfo.ndi = 0;
   pdcch->dci.u.format1aInfo.t.pdschInfo.allocInfo.rv = 0;
   /* As of now, we do not support Distributed allocations */
   pdcch->dci.u.format1aInfo.t.pdschInfo.allocInfo.isLocal = TRUE;
   pdcch->dci.u.format1aInfo.t.pdschInfo.allocInfo.nGap2.pres = \
                        NOTPRSNT;
   pdcch->dci.u.format1aInfo.t.pdschInfo.allocInfo.alloc.type = \
               TFU_ALLOC_TYPE_RIV;
   pdcch->dci.u.format1aInfo.t.pdschInfo.allocInfo.alloc.u.riv = \
    rgSCHCmnCalcRiv (cell->bwCfg.dlTotalBw, 
                     rbAllocInfo->allocInfo.raType2.rbStart,
                     rbAllocInfo->allocInfo.raType2.numRb);
#ifdef LTE_TDD
   if(hqP->hqE->ue != NULLP)
   {
      anInfo = rgSCHUtlGetUeANFdbkInfo(hqP->hqE->ue,
            &(rbAllocInfo->tbInfo[0].tbCb->fdbkTime));
#ifdef TFU_TDD
      pdcch->dci.u.format1aInfo.t.pdschInfo.dai.pres = TRUE;
      if(anInfo)
      {
         pdcch->dci.u.format1aInfo.t.pdschInfo.dai.val = 
            RG_SCH_GET_DAI_VALUE(anInfo->dlDai);
      }
      else
      {
         pdcch->dci.u.format1aInfo.t.pdschInfo.dai.val = RG_SCH_MAX_DAI_IDX;
         RLOG_ARG1(L_ERROR,DBG_CELLID,cell->cellId,
                  "PDCCH is been scheduled without updating anInfo CRNTI:%d",
                  pdcch->rnti);
      }
#endif
   }
#endif


   RETVOID;
}
/** @brief This function is called to fill activation PDCCH for DCI format 2
 *  
 *  @details
 *  Invoked by - rgSCHCmnSpsDlFillActvPdcch
 *  
 *  Function:  rgSCHCmnSpsDlFillActvPdcchDciFrmt2
 *       
 *  Processing steps:
 *  Fills up the activation/re-activation PDCCH for SPS UE.
 *
 *  @param  RgSchCellCb         *cellCb
 *  @param  RgSchCmnDlUeSpsInfo *dlSpsUe    
 *  @param  RgSchDlRbAlloc      *rbAllocInfo
 *  @param  RgSchPdcch           *pdcch
 *  @param  Bool                 isSpsOcc
 *  @param  U8                   HqProcId
 *   @return Void
 **/
#ifdef ANSI
PRIVATE Void rgSCHCmnSpsDlFillActvPdcchDciFrmt2
(
RgSchCellCb                *cell,
RgSchCmnDlUeSpsInfo        *dlSpsUe,
RgSchDlRbAlloc             *rbAllocInfo,
RgSchPdcch                 *pdcch,
Bool                       isSpsOcc,
U8                         hqProcId
)
#else /* ANSI */
PRIVATE  Void rgSCHCmnSpsDlFillActvPdcchDciFrmt2(cell, dlSpsUe, rbAllocInfo,
        pdcch, isSpsOcc, hqProcId)
RgSchCellCb                *cell;
RgSchCmnDlUeSpsInfo        *dlSpsUe;
RgSchDlRbAlloc             *rbAllocInfo;
RgSchPdcch                 *pdcch;
Bool                       isSpsOcc;
U8                         hqProcId;
#endif /* ANSI */
{
   U8 tpc;
   U32 allocBitMask;
#ifdef LTE_TDD
   RgSchTddANInfo     *anInfo;
   RgSchDlHqProcCb    *hqP = rbAllocInfo->tbInfo[0].tbCb->hqP;
#endif

   tpc = dlSpsUe->dlSpsUeSchdInfo.allocN1PucchIdx;
   allocBitMask = 0;

   TRC2(rgSCHCmnSpsDlFillActvPdcchDciFrmt2)

   if (!isSpsOcc)
   {
       rgSCHCmnFillHqPTb(cell, rbAllocInfo, 0, pdcch);
   }
   else
   {
       rgSCHCmnFillHqPTb(cell, rbAllocInfo, 0, NULLP);
   }
   
   pdcch->dci.u.format2Info.tpcCmd = tpc;
    /*ccpu00117078 - MOD - typo correction for format2Info*/
   if (rbAllocInfo->raType == RG_SCH_CMN_RA_TYPE0)
   {
        /* Fill-up for resource allocation type 0 */
       pdcch->dci.u.format2Info.allocInfo.isAllocType0 = TRUE;
       allocBitMask =  rbAllocInfo->allocInfo.raType0.dlAllocBitMask;
   }
   else
   {
       /* Fill-up for resource allocation type 1 */
       pdcch->dci.u.format2Info.allocInfo.isAllocType0 = FALSE;
       allocBitMask =  rbAllocInfo->allocInfo.raType1.dlAllocBitMask;
   }
   pdcch->dci.u.format2Info.allocInfo.resAllocMap[0] = \
               ((allocBitMask >> 24) & 0xff);
   pdcch->dci.u.format2Info.allocInfo.resAllocMap[1] = \
               ((allocBitMask >> 16) & 0x00ff);
   pdcch->dci.u.format2Info.allocInfo.resAllocMap[2] = \
               ((allocBitMask >> 8) & 0x0000ff);
   pdcch->dci.u.format2Info.allocInfo.resAllocMap[3] = \
               ((allocBitMask & 0x000000ff));
   
   pdcch->dci.u.format2Info.allocInfo.harqProcId = hqProcId;
   pdcch->dci.u.format2Info.allocInfo.precoding  = 0;
   pdcch->dci.u.format2Info.allocInfo.transSwap = 0;
   pdcch->dci.u.format2Info.allocInfo.tbInfo[0].mcs = 0;
   pdcch->dci.u.format2Info.allocInfo.tbInfo[0].rv  = 1;

   /* Fill tbInfo for scheduled TBs */

   pdcch->dci.u.format2Info.allocInfo.tbInfo[rbAllocInfo->tbInfo[0].\
               tbCb->tbIdx].ndi = 0;
   pdcch->dci.u.format2Info.allocInfo.tbInfo[rbAllocInfo->tbInfo[0].\
               tbCb->tbIdx].mcs = rbAllocInfo->tbInfo[0].tbCb->dlGrnt.iMcs;
   pdcch->dci.u.format2Info.allocInfo.tbInfo[rbAllocInfo->tbInfo[0].\
               tbCb->tbIdx].rv = 0;

#ifdef LTE_TDD
   if(hqP->hqE->ue != NULLP)
   {
      anInfo = rgSCHUtlGetUeANFdbkInfo(hqP->hqE->ue,
            &(rbAllocInfo->tbInfo[0].tbCb->fdbkTime));
#ifdef TFU_TDD
      if(anInfo)
      {
         pdcch->dci.u.format2Info.dai = RG_SCH_GET_DAI_VALUE(anInfo->dlDai);
      }
      else
      {
         pdcch->dci.u.format2Info.dai = RG_SCH_MAX_DAI_IDX;
         RLOG_ARG1(L_ERROR,DBG_CELLID,cell->cellId,
                  "PDCCH is been scheduled without updating anInfo CRNTI:%d",
                  pdcch->rnti);
      }
#endif
   }
#endif


   RETVOID;
}
/** @brief This function is called to fill activation PDCCH for DCI format 2A
 *  
 *  @details
 *  Invoked by - rgSCHCmnSpsDlFillActvPdcch
 *  
 *  Function:  rgSCHCmnSpsDlFillActvPdcchDciFrmt2A
 *       
 *  Processing steps:
 *  Fills up the activation/re-activation PDCCH for SPS UE.
 *
 *  @param  RgSchCellCb         *cellCb
 *  @param  RgSchCmnDlUeSpsInfo *dlSpsUe    
 *  @param  RgSchDlRbAlloc      *rbAllocInfo
 *  @param  RgSchPdcch           *pdcch
 *  @param  Bool                 isSpsOcc
 *  @param  U8                   HqProcId
 *   @return Void
 **/
#ifdef ANSI
PRIVATE Void rgSCHCmnSpsDlFillActvPdcchDciFrmt2A
(
RgSchCellCb                *cell,
RgSchCmnDlUeSpsInfo        *dlSpsUe,
RgSchDlRbAlloc             *rbAllocInfo,
RgSchPdcch                 *pdcch,
Bool                       isSpsOcc,
U8                         hqProcId
)
#else /* ANSI */
PRIVATE  Void rgSCHCmnSpsDlFillActvPdcchDciFrmt2A(cell, dlSpsUe, rbAllocInfo,
        pdcch, isSpsOcc, hqProcId)
RgSchCellCb                *cell;
RgSchCmnDlUeSpsInfo        *dlSpsUe;
RgSchDlRbAlloc             *rbAllocInfo;
RgSchPdcch                 *pdcch;
Bool                       isSpsOcc;
U8                         hqProcId;
#endif /* ANSI */
{
   U8 tpc;
   U32 allocBitMask;
#ifdef LTE_TDD
   RgSchTddANInfo     *anInfo;
   RgSchDlHqProcCb    *hqP = rbAllocInfo->tbInfo[0].tbCb->hqP;
#endif
   
   tpc = dlSpsUe->dlSpsUeSchdInfo.allocN1PucchIdx;
   allocBitMask = 0;

   TRC2(rgSCHCmnSpsDlFillActvPdcchDciFrmt2A)

   if (!isSpsOcc)
   {
       rgSCHCmnFillHqPTb(cell, rbAllocInfo, 0, pdcch);
   }
   else
   {
       rgSCHCmnFillHqPTb(cell, rbAllocInfo, 0, NULLP);
   }
   pdcch->dci.u.format2AInfo.tpcCmd = tpc;
   /*ccpu00117078 - MOD - typo correction for format2AInfo*/
   if (rbAllocInfo->raType == RG_SCH_CMN_RA_TYPE0)
   {
       /* Fill-up for resource allocation type 0 */
       pdcch->dci.u.format2AInfo.allocInfo.isAllocType0 = TRUE;
       allocBitMask =  rbAllocInfo->allocInfo.raType0.dlAllocBitMask;
   }
   else
   {
       /* Fill-up for resource allocation type 1 */
       pdcch->dci.u.format2AInfo.allocInfo.isAllocType0 = FALSE;
       allocBitMask =  rbAllocInfo->allocInfo.raType1.dlAllocBitMask;
   }
   pdcch->dci.u.format2AInfo.allocInfo.resAllocMap[0] = 
               ((allocBitMask >> 24) & 0xff);
   pdcch->dci.u.format2AInfo.allocInfo.resAllocMap[1] = 
               ((allocBitMask >> 16) & 0x00ff);
   pdcch->dci.u.format2AInfo.allocInfo.resAllocMap[2] = 
               ((allocBitMask >> 8) & 0x0000ff);
   pdcch->dci.u.format2AInfo.allocInfo.resAllocMap[3] = 
               ((allocBitMask & 0x000000ff));
   pdcch->dci.u.format2AInfo.allocInfo.harqProcId = hqProcId;
   pdcch->dci.u.format2AInfo.allocInfo.precoding  = 0;
   pdcch->dci.u.format2AInfo.allocInfo.transSwap  = 0;
   /* Initialize the TB info for TB 0 only */
   pdcch->dci.u.format2AInfo.allocInfo.tbInfo[0].mcs = 0;
   pdcch->dci.u.format2AInfo.allocInfo.tbInfo[0].rv  = 1;
   /* Fill tbInfo for scheduled TBs */
   pdcch->dci.u.format2AInfo.allocInfo.tbInfo[rbAllocInfo->tbInfo[0].\
               tbCb->tbIdx].ndi = 0;
   pdcch->dci.u.format2AInfo.allocInfo.tbInfo[rbAllocInfo->tbInfo[0].\
               tbCb->tbIdx].mcs = rbAllocInfo->tbInfo[0].tbCb->dlGrnt.iMcs;
   pdcch->dci.u.format2AInfo.allocInfo.tbInfo[rbAllocInfo->tbInfo[0].\
               tbCb->tbIdx].rv = 0;

#ifdef LTE_TDD
   if(hqP->hqE->ue != NULLP)
   {
      anInfo = rgSCHUtlGetUeANFdbkInfo(hqP->hqE->ue,
            &(rbAllocInfo->tbInfo[0].tbCb->fdbkTime));
#ifdef TFU_TDD
      if(anInfo)
      {
         pdcch->dci.u.format2AInfo.dai = RG_SCH_GET_DAI_VALUE(anInfo->dlDai);
      }
      else
      {
         pdcch->dci.u.format2AInfo.dai = RG_SCH_MAX_DAI_IDX;
         RLOG_ARG1(L_ERROR,DBG_CELLID,cell->cellId,
                  "PDCCH is been scheduled without updating anInfo CRNTI:%d",
                  pdcch->rnti);
      }
#endif
   }
#endif

   RETVOID;
}

/* Handler function to handle UE's who 
 * were expecting a ACK for Release PDCCH but received
 * no feedback
 */
/** @brief Handler to retransmit Release PDCCHs 
 *  
 *  @details
 *  Invoked by - rgSCHTomTtiInd
 *  
 *  Function:  rgSCHCmnSpsDlRetransRelPdcch
 *       
 *  Processing steps:
 *  a) Taking current subframe as index, traverse
 *     through wtngForRelFdbkUeLst.
 *  b) If any entry is found, a retranmission is
 *     scheduled
 *
 *  @param  RgSchCellCb         *cellCb
 *  @return Void
 **/
#ifdef ANSI
PUBLIC Void rgSCHCmnSpsDlReTxRelPdcch
(
RgSchCellCb                *cell
)
#else /* ANSI */
PUBLIC  Void rgSCHCmnSpsDlReTxRelPdcch(cell)
RgSchCellCb                *cell;
#endif /* ANSI */
{
   RgSchCmnDlCellSpsInfo *dlSpsCell;
   U8                     idx;
   CmLList               *listNode;

   RgSchUeCb             *ue;
   RgSchCmnDlUeSpsInfo   *dlSpsUe;
   RgSchDlSf             *dlSf;
   CmLteTimingInfo       actulTxTime; /*the timingInfo of feedback */
#ifdef LTE_TDD
   CmLteTimingInfo       actulRelTime;
#endif
   


   dlSpsCell = RG_SCH_CMN_SPS_GET_DL_CELL(cell);
   idx       = cell->crntTime.subframe;

   /*initialize listNode */ 
   CM_LLIST_FIRST_NODE(&(dlSpsCell->wtngForRelFdbkUeLst[idx]),listNode);
   
   while ( listNode )
   {
      ue      = (RgSchUeCb *)(listNode->node);
      dlSpsUe = RG_SCH_CMN_SPS_GET_DL_UE(ue);
#ifdef LTE_TDD
      /* wtngForRelFdbkUeLst is an array of CmLListCp of size
       * RGSCH_NUM_SUB_FRAMES and the subframe acts as an idx
       * to this array.However, in TDD for some cfgs like 5,
       * a feedback can be expected 13 subframes after RelPDCCH Tx
       * which would cause a cycle in the array. To ensure, we 
       * don't delete the UE too early from this queue, we
       * compare the Release time with the current time - TFU_HQFBKIND_ULDELTA 
       * . If they do not match, we do not delete
       * */
      /* ccpu00133109: Removed RGSCHSUBFRMCRNTTIME as it is not giving proper 
       * output if diff is more than 10. Instead using RGSCHDECRFRMCRNTTIME() 
       * as it is serving the purpose */
      RGSCHDECRFRMCRNTTIME(cell->crntTime,actulRelTime,(TFU_HQFBKIND_ULDELTA));
      if ( !(RGSCH_TIMEINFO_SAME(actulRelTime, ue->relPdcchFbkTiming)) )
      {
         CM_LLIST_NEXT_NODE(&(dlSpsCell->wtngForRelFdbkUeLst[idx]),listNode);
         continue;
      }
#endif
      RLOG_ARG2(L_DEBUG,DBG_CELLID,cell->cellId,
			      "DL SPS DTX for Release PDCCH sfn:sf %d:%d", 
               cell->crntTime.sfn,
               cell->crntTime.subframe);
      dlSpsUe->isRelPdcchSent = FALSE;

#ifdef RGSCH_SPS_STATS
      dlRelPdcchRetx++;
#endif
      
      if (dlSpsUe->wtngForRelFdbkUeEnt.node)
      {
         cmLListDelFrm(&(dlSpsCell->wtngForRelFdbkUeLst[idx]),
               &(dlSpsUe->wtngForRelFdbkUeEnt));
         dlSpsUe->wtngForRelFdbkUeEnt.node = NULLP; /* SPS INTG FIX */
      }

      /*reset relPdcch in dlSf */
      actulTxTime = ue->relPdcchTxTime;
      dlSf = rgSCHUtlSubFrmGet(cell,actulTxTime);

      rgSCHUtlPdcchPut(cell,&dlSf->pdcchInfo, dlSf->relPdcch);
      dlSf->relPdcch = NULLP;
      ue->relPdcchTxTime.sfn    = RGSCH_MAX_SFN + 1;
      ue->relPdcchFbkTiming.sfn = RGSCH_MAX_SFN + 1;

      CM_LLIST_NEXT_NODE(&(dlSpsCell->wtngForRelFdbkUeLst[idx]),listNode);
   }/*while (listNode) */

   RETVOID;

}/*rgSCHCmnSpsDlReTxRelPdcch*/

#ifdef RGSCH_SPS_STATS
/**
 * @brief Print Stats for a UE
 *
 * @details
 *
 *     Function : rgSCHCmnSpsPrintStats
 *
 *     Invoked by:
 *        
 *     Processing Steps:
 *
 *  @param[in]  RgSchUeCb        *ue
 *  @return  S16
 **/

#ifdef ANSI
PRIVATE S16 rgSCHCmnSpsPrintStats
(
RgSchCellCb                *cell
)
#else /* ANSI */
PRIVATE S16 rgSCHCmnSpsPrintStats(cell)
RgSchCellCb                *cell;
#endif /* ANSI */
{
   RgSchCmnDlCellSpsInfo *dlSpsCell = RG_SCH_CMN_SPS_GET_DL_CELL(cell);
   RgSchUeCb           *ue = NULLP;
   RgSchCmnDlUe        *cmnUeDl = NULLP;
   RgSchCmnDlUeSpsInfo *dlSpsUe = NULLP;
   RgSchCmnUlUeSpsInfo   *ulSpsUe = NULLP;
   static U32          wrapCount = 0;
   static U32          statPrintDist = 0;
   U32                 avgBsr = 0;
   RgSchCmnSpsDlSf       *dlSpsSfArr = NULLP;
   dlSpsSfArr = dlSpsCell->spsSfTbl;
   static U32 winUlSpsSchd;
   static U32 winUlCrcPass;
   static U32 winUlCrcFail;
   static U32 winUlActFail;
   static U32 winUlOccFail;
   static U32 winDlSpsSchd;
   static U32 winDlMaxRetxFail;

   TRC2 (rgSCHCmnSpsPrintStats);
   
   if (cell->crntTime.sfn == 0)
   {
      if (wrapCount++ % 60)
      {
         RETVALUE(ROK);
      }
      statPrintDist++;

      RGSCHDBGSPSSTATS("============ SPS STATS STARTS ============\n");
      //if ((statPrintDist%2) == 0)
      RGSCHDBGSPSSTATS ("+++++++++UE SPS STATS+++++++++\n");
      {
         ue = rgSCHDbmGetNextSpsUeCb(cell, NULLP);
         if (ue)
         {
            do {
               U8                  scCqi;
               RgSchCmnUlUe      *ueUl = RG_SCH_CMN_GET_UL_UE(ue);
               dlSpsUe = RG_SCH_CMN_SPS_GET_DL_UE(ue);
               ulSpsUe = RG_SCH_CMN_GET_UL_SPS_UE(ue);
               cmnUeDl = RG_SCH_CMN_GET_DL_UE(ue);
               scCqi = rgSCHCmnSpsDlGetScaledCqi(ue, cmnUeDl->mimoInfo.cwInfo[0].cqi);

            
               {
                  if (ue->absTimeBo > ue->absTimeSched)
                  {
                     if (ue->absTimeBo -  ue->absTimeSched > 100)
                     {
                        RG_SCH_ABORT();
                     }
                  }
                  else
                  {
                     if (ue->absTimeSched -  ue->absTimeBo > 100)
                     {
                        RG_SCH_ABORT();
                     }
                  }
               }

               /* Print Statistics */
               RGSCHDBGSPSSTATS("DL SPS:CRNTI %d SPSRNTI %d STATE %u:Cqi/ScCqi[%d/%d]"
                     " SpsSchd %lu DynSchd %lu Act %lu Rel %lu React %lu tot:act:sps:dyn(%lu:%lu:%lu:%lu) Ack:max %lu %lu [%lu:%lu:%lu:%lu]\n",
                     ue->ueId,
                     ue->spsRnti, 
                     dlSpsUe->dlSpsStatus,
                     cmnUeDl->mimoInfo.cwInfo[0].cqi,
                     scCqi,
                     dlSpsUe->statInfo.numSchedSPSRnti,
                     dlSpsUe->statInfo.totalSPSSchedOcc - dlSpsUe->statInfo.numSchedSPSRnti,
                     dlSpsUe->statInfo.numSpsActv,
                     dlSpsUe->statInfo.numSpsRel,
                     dlSpsUe->statInfo.numSpsReactv);

               avgBsr = 0;
               if (ulSpsUe->numBsrRpt)
                  avgBsr = ulSpsUe->totalBsr/ulSpsUe->numBsrRpt;

               RGSCHDBGSPSSTATS("UL SPS Cqi/ScCqi[%d/%d]: STATE %d CRC Pass %lu Fail %lu Avg Bsr %lu\n",
                     ueUl->validUlCqi,
                     ulSpsUe->ulSpsSchdInfo.scaledCqi,
                     ulSpsUe->state,
                     ulSpsUe->crcPassStats,
                     ulSpsUe->crcFailStats,
                     avgBsr);

            } while (NULLP != (ue = rgSCHDbmGetNextSpsUeCb(cell, ue)));
         }
      }
      RGSCHDBGSPSSTATS ("+++++++++TOTAL STATS+++++++++\n");
      RGSCHDBGSPSSTATS("DL: DynSchd %lu SpsSchd %lu Act %lu ActSucc %lu ActMaxTxFail %lu SpsMaxRetx %lu  RelPdcchRetx %lu\n",
            numDynSched,
            numSPSSched,
            numActvations,
            numActSuccess,
            rgNumActDtx,
            rgNumSPSSchedDropMaxRetx,
            dlRelPdcchRetx);

      RGSCHDBGSPSSTATS ("DL: Rel sent %lu toCl %lu atCl %lu \n",
            numdlSpsRel,
            numdlSpsRelSentToTf,
            numdlSpsRelRcvdAtCl);

      RGSCHDBGSPSSTATS("UL: ActSent %lu ActSucc %lu CrcFailAct %lu NumReact %lu ImplRel %lu \n",
            rgSchSpsNumUlSpsActSent,
            rgSchSpsNumActCrcPass,
            crcFailActCountStat,
            rgSchSpsNumUlReact,
            rgSchSpsNumImplRel);

      RGSCHDBGSPSSTATS("UL REL: totExplRel %lu Succ %lu dueToCrcF %lu relPdcch %lu sentoTf %lu atTf %lu \n",
      rgSchSpsNumExplRel,
      ulExplRelSucc,
      crcFailExplRelStat,
      rgSchSpsRelPdcchAllocd,
      rgSchSpsRelSentToTf,
      rgSchSpsRelAtTf);


      avgBsr = 0;
      if (rgSchSpsStatNumBsrRpt)
           avgBsr = rgSchSpsStatTotalBsr/rgSchSpsStatNumBsrRpt;
      RGSCHDBGSPSSTATS("UL: TotSpsSchd %lu CrcPass %lu crcFail %lu :Act %lu Occ %lu Avg BSR %lu \n",
            ulSpsOccSched,
            crcPassCount,
            crcFailActCountStat+
            crcFailOccCountStat,
            crcFailActCountStat,
            crcFailOccCountStat,
            avgBsr);
      RGSCHDBGSPSSTATS ("+++++++++WINDOW STATS+++++++++\n");

      RGSCHDBGSPSSTATS ("DL:TotSpsSchd %lu MaxRetx %lu \n",
            numSPSSched - winDlSpsSchd,
            rgNumSPSSchedDropMaxRetx - winDlMaxRetxFail);

      RGSCHDBGSPSSTATS ("UL:TotSpsSchd %lu CrcPass %lu crcFail %lu :Act %lu Occ %lu \n",
            ulSpsOccSched - winUlSpsSchd,
            crcPassCount - winUlCrcPass,
            (crcFailActCountStat + crcFailOccCountStat) - winUlCrcFail,
            crcFailActCountStat - winUlActFail,
            crcFailOccCountStat - winUlOccFail);

      winUlSpsSchd = ulSpsOccSched;
      winUlCrcPass = crcPassCount;
      winUlCrcFail = (crcFailActCountStat + crcFailOccCountStat);
      winUlActFail = crcFailActCountStat;
      winUlOccFail = crcFailOccCountStat;

      winDlSpsSchd = numSPSSched; 
      winDlMaxRetxFail = rgNumSPSSchedDropMaxRetx;

      RGSCHDBGSPSSTATS("============ SPS STATS ENDS ============\n");
   }
   RETVALUE(ROK);   
} /* end of rgSCHCmnSpsPrintStats */

#endif /* RGSCH_SPS_STATS */

/**
 * @brief Handler for processing PHR
 *
 * @details
 *
 *     Function : rgSCHCmnSpsPhrInd
 *
 *     Invoked By:
 *       rgSCHCmnUpdPhr
 *
 *     Processing Steps:
 *          Get the maxUlRb left out
 *          Add the UE to rlsLst if the remaingRb is less than the required RB's
 *
 *  @param[in]  RgSchCellCb        *cell
 *  @param[in]  RgSchUeCb          *ue
 *  @return  Void
 **/
#ifdef ANSI
PUBLIC Void rgSCHCmnSpsPhrInd
(
RgSchCellCb         *cell,
RgSchUeCb           *ue
)
#else /* ANSI */
PUBLIC Void rgSCHCmnSpsPhrInd(cell, ue)
RgSchCellCb         *cell;
RgSchUeCb           *ue;
#endif /* ANSI */
{
   RgSchCmnUlUeSpsInfo   *ulSpsUe = RG_SCH_CMN_GET_UL_SPS_UE(ue);
   U8                maxRb;

   TRC2(rgSCHCmnSpsPhrInd);


   maxRb = rgSCHPwrGetMaxUlRb(cell, ue);
   if (ulSpsUe->state == RGSCH_SPS_ACTV)
   {
      if(ulSpsUe->ulSpsSchdInfo.allocInfo.grnt.numRb > maxRb)
      {
         /* Add the UE to toBeRlsLst as the required Rb is less than the 
          * Max Rb due to PHR
          */
         rgSCHCmnSpsUlProcRelInd(cell, ue, TRUE);
      }
   }
   RETVOID;
} /* end of rgSCHCmnSpsPhrInd */

/**
 * @brief Checks if the CFI changes for the UL Sf
 *
 * @details
 *
 *     Function : rgSCHCmnSpsUlChkUlSf
 *
 *     Invoked By:
 *
 *     Processing Steps:
 *          Fetch the Ul corresponding to the dlSf at which the new CFI
 *          is going to change.  
 *          Check if the ul Sf passed is same as the ul Sf in above step
 *
 *  @param[in]  RgSchCellCb        *cell
 *  @param[in]  RgSchUlSf          *ulSf
 *  @return  Bool
 **/
#ifdef ANSI
PRIVATE Bool rgSCHCmnSpsUlChkUlSf
(
RgSchCellCb           *cell,
RgSchUlSf             *ulSf
)
#else 
PRIVATE Bool rgSCHCmnSpsUlChkUlSf(cell, ulSf)
RgSchCellCb           *cell;
RgSchUlSf             *ulSf;
#endif 
{
   RgSchCmnUlCell  *cellUl = RG_SCH_CMN_GET_UL_CELL(cell);
   RgSchDlSf       *dlSf;
   RgSchUlSf       *newCfiUlSf; 
   CmLteTimingInfo ulSfTime;
   U8              ulSfIdx;
#ifdef LTE_TDD
   U8              m;
   U8              sfnOffset;
#endif

   TRC2(rgSCHCmnSpsUlChkUlSf);
   
   if (cell->dynCfiCb.pdcchSfIdx == 0xFF)
   {
      RETVALUE(FALSE);
   }
      
   dlSf = cell->subFrms[cell->dynCfiCb.pdcchSfIdx];
#ifdef LTE_TDD  
   m = rgSchTddPucchTxTbl[cell->ulDlCfgIdx][dlSf->sfNum];
 
   ulSfTime.subframe = (dlSf->sfNum + m) % RGSCH_NUM_SUB_FRAMES;
   sfnOffset = (dlSf->sfNum + m) / RGSCH_NUM_SUB_FRAMES;
   ulSfTime.sfn = (cell->crntTime.sfn + sfnOffset) % RGSCH_MAX_SFN;   
   ulSfIdx      = rgSCHCmnGetUlSfIdx(&ulSfTime, cell);
#else
   ulSfTime.subframe = (dlSf->sfNum + RGSCH_PDCCH_PUSCH_DELTA) % 
                        RGSCH_NUM_SUB_FRAMES;  
  
   if (dlSf->sfNum + RGSCH_PDCCH_PUSCH_DELTA >= RGSCH_NUM_SUB_FRAMES)
   { 
      ulSfTime.sfn = (cell->crntTime.sfn + 1) % RGSCH_MAX_SFN;
   }
   else
   {
      ulSfTime.sfn = cell->crntTime.sfn;
   }
 
   ulSfIdx = (ulSfTime.sfn * RGSCH_NUM_SUB_FRAMES + ulSfTime.subframe) % 
                        RG_SCH_CMN_UL_NUM_SF; 
#endif
   newCfiUlSf      = &cellUl->ulSfArr[ulSfIdx];
   
   if (newCfiUlSf == ulSf)
   {
      RETVALUE(TRUE);
   }

   RETVALUE(FALSE);
}


#endif /* LTEMAC_SPS */

/**********************************************************************
 
         End of file:     gk_sch_sps.c@@/main/2 - Sat Jul 30 02:21:50 2011
 
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
/main/1      ---     sd   1. LTE MAC 3.1 release
/main/1   rg009.201  rsharon   1. [ccpu00115756]:Removed TFU_UPGRADE flag for
                                   DTX.
                    nudupi 2.Pending Insure fixes accomodated
/main/1    3. Added changes of TFU_UPGRADE. 
/main/2      ---        gvj  1. Updated for LTE MAC Rel 3.1           
  rg001.301  nudupi  1.Added the changes for the CR ccpu00117270 
                     2. ccpu00118350:Correcting NDI manipulation of Harq.
          rg002.301 asehgal 1. ccpu00119494: added code to handle retx
                               of release PDCCH.
$SID$        ---      rt     1. LTE MAC 4.1 release
*********************************************************************91*/


