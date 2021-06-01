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

     File:     rg_sch_cmn.c

     Sid:      gk_sch_cmn.c@@/main/2 - Sat Jul 30 02:21:36 2011

     Prg:      SY

**********************************************************************/

/** @file rg_sch_cmn.c
@brief This file implements the schedulers main access to MAC layer code.
*/

static const char* RLOG_MODULE_NAME="MAC";
static int RLOG_FILE_ID=187;
static int RLOG_MODULE_ID=4096;

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
#include "cm5.h"
#include "lrg.h"
#include "rgr.h"
#include "tfu.h"
#include "rgm.h"
#include "rg_env.h"
#include "rg_sch_err.h"
#include "rg_sch_inf.h"
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
#include "cm_mblk.x"       /* memory management */
#include "cm_tkns.x"       /* common tokens */
#include "cm_lte.x"       /* common tokens */
#include "tfu.x"           /* TFU types */
#include "lrg.x"           /* layer management typedefs for MAC */
#include "rgr.x"           /* layer management typedefs for MAC */
#include "rgm.x"           /* layer management typedefs for MAC */
#include "rg_sch_inf.x"    /* typedefs for Scheduler */
#include "rg_sch.x"        /* typedefs for Scheduler */
#include "rg_sch_cmn.x"    /* typedefs for Scheduler */
#ifdef MAC_SCH_STATS
#include "lrg.x"            /* Stats Structures */
#endif /* MAC_SCH_STATS */

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#define RG_ITBS_DIFF(_x, _y) ((_x) > (_y) ? (_x) - (_y) : (_y) - (_x))
EXTERN Void rgSCHSc1UlInit ARGS((RgUlSchdApis *apis));
#ifdef RG_PHASE2_SCHED
EXTERN Void rgSCHRrUlInit ARGS((RgUlSchdApis *apis));
EXTERN Void rgSCHMaxciUlInit ARGS((RgUlSchdApis *apis));
EXTERN Void rgSCHPfsUlInit ARGS((RgUlSchdApis *apis));
#endif
EXTERN Void rgSCHSc1DlInit ARGS((RgDlSchdApis *apis));
#ifdef RG_PHASE2_SCHED
EXTERN Void rgSCHRrDlInit ARGS((RgDlSchdApis *apis));
EXTERN Void rgSCHMaxciDlInit ARGS((RgDlSchdApis *apis));
EXTERN Void rgSCHPfsDlInit ARGS((RgDlSchdApis *apis));
#ifdef TFU_UPGRADE
EXTERN Void rgSCHDlfsInit ARGS((RgDlfsSchdApis *apis));
#endif
#endif

#ifdef RGR_SI_SCH
PRIVATE Void rgSCHDlSiSched ARGS((RgSchCellCb  *cell,
                      RgSchCmnDlRbAllocInfo *allocInfo,
                      RgInfSfAlloc  *subfrmAlloc));
PRIVATE Void rgSCHChkNUpdSiCfg ARGS((RgSchCellCb  *cell));
PRIVATE Void rgSCHSelectSi ARGS((RgSchCellCb *cell));
#endif /*RGR_SI_SCH*/
/* LTE_ADV_FLAG_REMOVED_START */
#ifndef LTE_TDD
PRIVATE S16 rgSCHCmnNonDlfsUpdDSFRTyp2Alloc
(
RgSchCellCb        *cell,
RgSchUeCb          *ue,
RgSchDlSf          *dlSf,
U8                 rbStrt,
U8                 numRb
);
PRIVATE S16 rgSCHCmnBuildRntpInfo (
RgSchCellCb        *cell,
U8                 *rntpPtr,
U8                  startRb,
U8                  nmbRb,
U16                 bw
);
#endif
/* LTE_ADV_FLAG_REMOVED_END */

PRIVATE Void rgSCHCmnNonDlfsBcchPcchRbAlloc ARGS((
RgSchCellCb           *cell,
RgSchCmnDlRbAllocInfo *allocInfo
));
PRIVATE Void rgSCHBcchPcchDlRbAlloc ARGS((
RgSchCellCb           *cell,
RgSchCmnDlRbAllocInfo *allocInfo
));
PRIVATE Void rgSCHCmnDlBcchPcchAlloc ARGS((
RgSchCellCb  *cell
));
#ifdef RGR_CQI_REPT
PRIVATE Void rgSCHCmnDlCqiOnPucchInd ARGS ((
 RgSchCellCb        *cell,
 RgSchUeCb          *ue,
 TfuDlCqiRpt        *dlCqiRpt,
 RgrUeCqiRept       *ueCqiRept,
 Bool               *isCqiAvail,
 Bool               *is2ndCwCqiAvail
 ));
PRIVATE Void rgSCHCmnDlCqiOnPuschInd ARGS ((
 RgSchCellCb        *cell,
 RgSchUeCb          *ue,
 TfuDlCqiRpt        *dlCqiRpt,
 RgrUeCqiRept       *ueCqiRept,
 Bool               *isCqiAvail,
 Bool               *is2ndCwCqiAvail
 ));
#else
PRIVATE Void rgSCHCmnDlCqiOnPucchInd ARGS ((
 RgSchCellCb        *cell,
 RgSchUeCb          *ue,
 TfuDlCqiRpt        *dlCqiRpt
 ));
PRIVATE Void rgSCHCmnDlCqiOnPuschInd ARGS ((
 RgSchCellCb        *cell,
 RgSchUeCb          *ue,
 TfuDlCqiRpt        *dlCqiRpt
 ));
#endif
/* ccpu00117452 - MOD - Changed macro name from
   RGR_RRM_DLPWR_CNTRL to RGR_CQI_REPT */
#ifdef RGR_CQI_REPT
PRIVATE S16 rgSCHCmnUeDlPwrCtColltCqiRept ARGS((
   RgSchCellCb        *cell,
   RgSchUeCb          *ue,
   RgrUeCqiRept        *ueCqiRept));
#endif /* End of RGR_CQI_REPT */
/* Fix: syed align multiple UEs to refresh at same time */
PRIVATE Void rgSCHCmnGetRefreshPer ARGS((
   RgSchCellCb  *cell,
   RgSchUeCb    *ue,
   U32          *waitPer));
PRIVATE S16 rgSCHCmnApplyUeRefresh ARGS((
RgSchCellCb     *cell,
RgSchUeCb       *ue));
#ifdef DL_LA
PRIVATE Void rgSCHCmnDlSetUeAllocLmtLa ARGS
((
RgSchCellCb   *cell,
RgSchUeCb     *ue
));
PRIVATE Void rgSCHCheckAndSetTxScheme ARGS
((
RgSchCellCb   *cell,
RgSchUeCb     *ue
));
#endif

#ifdef LTE_TDD
PRIVATE U32 rgSCHCmnCalcDwPtsTbSz ARGS
((
RgSchCellCb    *cell,
U32             bo,
U8             *rb,
U8             *iTbs,
U8              lyr,
U8              cfi
));

PRIVATE Void rgSCHCmnCalcDwPtsTbSz2Cw ARGS
((
RgSchCellCb    *cell,
U32             bo,
U8             *rb,
U8              maxRb,
U8             *iTbs1,
U8             *iTbs2,
U8              lyr1,
U8              lyr2,
U32            *tb1Sz, 
U32            *tb2Sz, 
U8              cfi
));

PRIVATE Bool rgSCHCmnRetxAvoidTdd ARGS 
((
RgSchDlSf                  *curSf,
RgSchCellCb                *cell,
RgSchDlHqProcCb            *proc
));

#endif

#ifdef __cplusplus
}
#endif /* __cplusplus */


/* local defines */
PUBLIC  RgSchdApis          rgSchCmnApis;
PRIVATE RgUlSchdApis        rgSchUlSchdTbl[RGSCH_NUM_SCHEDULERS];
PRIVATE RgDlSchdApis        rgSchDlSchdTbl[RGSCH_NUM_SCHEDULERS];
#ifdef RG_PHASE2_SCHED
PRIVATE RgDlfsSchdApis      rgSchDlfsSchdTbl[RGSCH_NUM_DLFS_SCHEDULERS];
#endif
PRIVATE RgUlSchdInits       rgSchUlSchdInits = RGSCH_ULSCHED_INITS;
PRIVATE RgDlSchdInits       rgSchDlSchdInits = RGSCH_DLSCHED_INITS;
#if (defined (RG_PHASE2_SCHED) && defined (TFU_UPGRADE))
PRIVATE RgDlfsSchdInits     rgSchDlfsSchdInits = RGSCH_DLFSSCHED_INITS;
#endif

typedef Void (*RgSchCmnDlAllocRbFunc) ARGS((RgSchCellCb *cell, RgSchDlSf *subFrm,
RgSchUeCb *ue, U32 bo, U32 *effBo, RgSchDlHqProcCb *proc,
RgSchCmnDlRbAllocInfo *cellWdAllocInfo));
typedef U8 (*RgSchCmnDlGetPrecInfFunc) ARGS((RgSchUeCb *ue, U8 numLyrs, Bool bothCwEnbld));

PRIVATE Void rgSCHCmnFillHqPPdcchDciFrmt1 ARGS((
RgSchCellCb                *cell,
RgSchDlRbAlloc             *rbAllocInfo,
RgSchDlHqProcCb            *hqP,
RgSchPdcch                 *pdcch,
U8                         tpc
));
PRIVATE Void rgSCHCmnFillHqPPdcchDciFrmt1A ARGS((
RgSchCellCb                *cell,
RgSchDlRbAlloc             *rbAllocInfo,
RgSchDlHqProcCb            *hqP,
RgSchPdcch                 *pdcch,
U8                         tpc
));
PRIVATE Void rgSCHCmnFillHqPPdcchDciFrmt1B ARGS((
RgSchCellCb                *cell,
RgSchDlRbAlloc             *rbAllocInfo,
RgSchDlHqProcCb            *hqP,
RgSchPdcch                 *pdcch,
U8                         tpc
));
PRIVATE Void rgSCHCmnFillHqPPdcchDciFrmt2 ARGS((
RgSchCellCb                *cell,
RgSchDlRbAlloc             *rbAllocInfo,
RgSchDlHqProcCb            *hqP,
RgSchPdcch                 *pdcch,
U8                         tpc
));
PRIVATE Void rgSCHCmnFillHqPPdcchDciFrmt2A ARGS((
RgSchCellCb                *cell,
RgSchDlRbAlloc             *rbAllocInfo,
RgSchDlHqProcCb            *hqP,
RgSchPdcch                 *pdcch,
U8                         tpc
));
PRIVATE Void rgSCHCmnDlAllocTxRbTM1 ARGS((
RgSchCellCb                *cell,
RgSchDlSf                  *subFrm,
RgSchUeCb                  *ue,
U32                        bo,
U32                        *effBo,
RgSchDlHqProcCb            *proc,
RgSchCmnDlRbAllocInfo      *cellWdAllocInfo
));
PRIVATE Void rgSCHCmnDlAllocTxRbTM2 ARGS((
RgSchCellCb                *cell,
RgSchDlSf                  *subFrm,
RgSchUeCb                  *ue,
U32                        bo,
U32                        *effBo,
RgSchDlHqProcCb            *proc,
RgSchCmnDlRbAllocInfo      *cellWdAllocInfo
));
PRIVATE Void rgSCHCmnDlAllocTxRbTM3 ARGS((
RgSchCellCb                *cell,
RgSchDlSf                  *subFrm,
RgSchUeCb                  *ue,
U32                        bo,
U32                        *effBo,
RgSchDlHqProcCb            *proc,
RgSchCmnDlRbAllocInfo      *cellWdAllocInfo
));
PRIVATE Void rgSCHCmnDlAllocTxRbTM4 ARGS((
RgSchCellCb                *cell,
RgSchDlSf                  *subFrm,
RgSchUeCb                  *ue,
U32                        bo,
U32                        *effBo,
RgSchDlHqProcCb            *proc,
RgSchCmnDlRbAllocInfo      *cellWdAllocInfo
));
#ifdef RG_UNUSED
PRIVATE Void rgSCHCmnDlAllocTxRbTM5 ARGS((
RgSchCellCb                *cell,
RgSchDlSf                  *subFrm,
RgSchUeCb                  *ue,
U32                        bo,
U32                        *effBo,
RgSchDlHqProcCb            *proc,
RgSchCmnDlRbAllocInfo      *cellWdAllocInfo
));
#endif
PRIVATE Void rgSCHCmnDlAllocTxRbTM6 ARGS((
RgSchCellCb                *cell,
RgSchDlSf                  *subFrm,
RgSchUeCb                  *ue,
U32                        bo,
U32                        *effBo,
RgSchDlHqProcCb            *proc,
RgSchCmnDlRbAllocInfo      *cellWdAllocInfo
));
PRIVATE Void rgSCHCmnDlAllocTxRbTM7 ARGS((
RgSchCellCb                *cell,
RgSchDlSf                  *subFrm,
RgSchUeCb                  *ue,
U32                        bo,
U32                        *effBo,
RgSchDlHqProcCb            *proc,
RgSchCmnDlRbAllocInfo      *cellWdAllocInfo
));
PRIVATE Void rgSCHCmnDlAllocRetxRbTM1 ARGS((
RgSchCellCb                *cell,
RgSchDlSf                  *subFrm,
RgSchUeCb                  *ue,
U32                        bo,
U32                        *effBo,
RgSchDlHqProcCb            *proc,
RgSchCmnDlRbAllocInfo      *cellWdAllocInfo
));
PRIVATE Void rgSCHCmnDlAllocRetxRbTM2 ARGS((
RgSchCellCb                *cell,
RgSchDlSf                  *subFrm,
RgSchUeCb                  *ue,
U32                        bo,
U32                        *effBo,
RgSchDlHqProcCb            *proc,
RgSchCmnDlRbAllocInfo      *cellWdAllocInfo
));
PRIVATE Void rgSCHCmnDlAllocRetxRbTM3 ARGS((
RgSchCellCb                *cell,
RgSchDlSf                  *subFrm,
RgSchUeCb                  *ue,
U32                        bo,
U32                        *effBo,
RgSchDlHqProcCb            *proc,
RgSchCmnDlRbAllocInfo      *cellWdAllocInfo
));
PRIVATE Void rgSCHCmnDlAllocRetxRbTM4 ARGS((
RgSchCellCb                *cell,
RgSchDlSf                  *subFrm,
RgSchUeCb                  *ue,
U32                        bo,
U32                        *effBo,
RgSchDlHqProcCb            *proc,
RgSchCmnDlRbAllocInfo      *cellWdAllocInfo
));
#ifdef RG_UNUSED
PRIVATE Void rgSCHCmnDlAllocRetxRbTM5 ARGS((
RgSchCellCb                *cell,
RgSchDlSf                  *subFrm,
RgSchUeCb                  *ue,
U32                        bo,
U32                        *effBo,
RgSchDlHqProcCb            *proc,
RgSchCmnDlRbAllocInfo      *cellWdAllocInfo
));
#endif
PRIVATE Void rgSCHCmnDlAllocRetxRbTM6 ARGS((
RgSchCellCb                *cell,
RgSchDlSf                  *subFrm,
RgSchUeCb                  *ue,
U32                        bo,
U32                        *effBo,
RgSchDlHqProcCb            *proc,
RgSchCmnDlRbAllocInfo      *cellWdAllocInfo
));
PRIVATE Void rgSCHCmnDlAllocRetxRbTM7 ARGS((
RgSchCellCb                *cell,
RgSchDlSf                  *subFrm,
RgSchUeCb                  *ue,
U32                        bo,
U32                        *effBo,
RgSchDlHqProcCb            *proc,
RgSchCmnDlRbAllocInfo      *cellWdAllocInfo
));
/* Functions specific to each transmission mode for DL Tx RB Allocation*/
RgSchCmnDlAllocRbFunc  dlAllocTxRbFunc[7] = {rgSCHCmnDlAllocTxRbTM1,
rgSCHCmnDlAllocTxRbTM2, rgSCHCmnDlAllocTxRbTM3, rgSCHCmnDlAllocTxRbTM4,
NULLP, rgSCHCmnDlAllocTxRbTM6, rgSCHCmnDlAllocTxRbTM7};

/* Functions specific to each transmission mode for DL Retx RB Allocation*/
RgSchCmnDlAllocRbFunc  dlAllocRetxRbFunc[7] = {rgSCHCmnDlAllocRetxRbTM1,
rgSCHCmnDlAllocRetxRbTM2, rgSCHCmnDlAllocRetxRbTM3, rgSCHCmnDlAllocRetxRbTM4,
NULLP, rgSCHCmnDlAllocRetxRbTM6, rgSCHCmnDlAllocRetxRbTM7};

PRIVATE U8 rgSCHCmnDlTM3PrecInf2 ARGS((
RgSchUeCb                  *ue,
U8                         numTxLyrs,
Bool                       bothCwEnbld
));
PRIVATE U8 rgSCHCmnDlTM3PrecInf4 ARGS((
RgSchUeCb                  *ue,
U8                         numTxLyrs,
Bool                       bothCwEnbld
));
PRIVATE U8 rgSCHCmnDlTM4PrecInf2 ARGS((
RgSchUeCb                  *ue,
U8                         numTxLyrs,
Bool                       bothCwEnbld
));
PRIVATE U8 rgSCHCmnDlTM4PrecInf4 ARGS((
RgSchUeCb                  *ue,
U8                         numTxLyrs,
Bool                       bothCwEnbld
));
/* Functions specific to each transmission mode for DL RB Allocation*/
RgSchCmnDlGetPrecInfFunc getPrecInfoFunc[2][2] = {
{rgSCHCmnDlTM3PrecInf2, rgSCHCmnDlTM3PrecInf4},
{rgSCHCmnDlTM4PrecInf2, rgSCHCmnDlTM4PrecInf4}
};

PRIVATE S16 rgSCHCmnDlAlloc1CwRetxRb ARGS((
RgSchCellCb                *cell,
RgSchDlSf                  *subFrm,
RgSchUeCb                  *ue,
RgSchDlHqTbCb              *tbInfo,
U8                         noLyr,
U8                         *numRb,
U32                        *effBo
));
PRIVATE S16 rgSCHCmnDlAlloc2CwRetxRb ARGS((
RgSchCellCb                *cell,
RgSchDlSf                  *subFrm,
RgSchUeCb                  *ue,
RgSchDlHqProcCb            *proc,
U8                         *numRb,
Bool                       *swpFlg,
U32                        *effBo
));
PRIVATE Void rgSCHCmnDlTM3TxTx ARGS((
RgSchCellCb                *cell,
RgSchDlSf                  *subFrm,
RgSchUeCb                  *ue,
U32                        bo,
U32                        *effBo,
RgSchDlHqProcCb            *proc,
RgSchCmnDlRbAllocInfo      *cellWdAllocInfo
));
PRIVATE Void rgSCHCmnDlTM3TxRetx ARGS((
RgSchCellCb                *cell,
RgSchDlSf                  *subFrm,
RgSchUeCb                  *ue,
U32                        bo,
U32                        *effBo,
RgSchDlHqProcCb            *proc,
RgSchCmnDlRbAllocInfo      *cellWdAllocInfo
));
PRIVATE Void rgSCHCmnDlTM3RetxRetx ARGS((
RgSchCellCb                *cell,
RgSchDlSf                  *subFrm,
RgSchUeCb                  *ue,
U32                        bo,
U32                        *effBo,
RgSchDlHqProcCb            *proc,
RgSchCmnDlRbAllocInfo      *cellWdAllocInfo
));
PRIVATE TfuDciFormat rgSCHCmnSlctPdcchFrmt ARGS((
RgSchCellCb                *cell,
RgSchUeCb                  *ue,
U8                         *raType
));
PRIVATE Void rgSCHCmnNonDlfsUpdTyp2Alloc ARGS((
RgSchCellCb        *cell,
RgSchDlSf          *dlSf,
U8                 rbStrt,
U8                 numRb
));
/* LTE_ADV_FLAG_REMOVED_START */
#ifndef LTE_TDD
PRIVATE Void rgSCHCmnNonDlfsSFRCmnChannelUpdTyp2Alloc ARGS((
RgSchCellCb        *cell,
RgSchDlSf          *dlSf,
U8                 rbStrt,
U8                 numRb
));
#endif
/* LTE_ADV_FLAG_REMOVED_END */
PRIVATE Void rgSCHCmnDlRbInfoAddUeTx ARGS((
RgSchCmnDlRbAllocInfo *allocInfo,
RgSchUeCb             *ue,
RgSchDlHqProcCb       *proc
));
PRIVATE Void rgSCHCmnDlRbInfoAddUeRetx ARGS((
RgSchCmnDlRbAllocInfo *allocInfo,
RgSchUeCb             *ue,
RgSchDlHqProcCb       *hqP
));
PRIVATE Void rgSCHCmnDlAdd2NonSchdRetxLst ARGS((
RgSchCmnDlRbAllocInfo *allocInfo,
RgSchUeCb             *ue,
RgSchDlHqProcCb       *proc
));
PRIVATE S16 rgSCHCmnDlAlloc2CwTxRetxRb ARGS((
RgSchCellCb                *cell,
RgSchDlSf                  *subFrm,
RgSchUeCb                  *ue,
RgSchDlHqTbCb              *reTxTb,
RgSchDlHqTbCb              *txTb,
U8                         *numRb,
U32                        *effBo
));
PRIVATE S16 rgSCHCmnDlAlloc2CwTxRb ARGS((
RgSchCellCb                *cell,
RgSchDlSf                  *subFrm,
RgSchUeCb                  *ue,
RgSchDlHqProcCb            *proc,
U32                        bo,
U8                         *numRb,
U32                        *effBo
));
PRIVATE S16 rgSCHCmnDlAlloc1CwTxRb ARGS((
RgSchCellCb                *cell,
RgSchDlSf                  *subFrm,
RgSchUeCb                  *ue,
RgSchDlHqTbCb              *tbInfo,
U32                        bo,
U8                         *numRb,
U32                        *effBo
));
#ifndef LTEMAC_SPS
PRIVATE Void rgSCHCmnFillHqPTb ARGS((
RgSchCellCb                *cell,
RgSchDlRbAlloc             *rbAllocInfo,
U8                         tbAllocIdx,
RgSchPdcch                 *pdcch
));
#endif
#ifdef LTEMAC_SPS
PRIVATE Void rgSCHCmnDlGetBestFitHole ARGS((
U32         *allocMask,
U8          numMaskRbs,
U32         *crntAllocMask,
U8          rbsReq,
U8          *allocStart,
U8          *allocNumRbs,
Bool        isPartialAlloc
));
#ifdef RGSCH_SPS_UNUSED
PRIVATE U32 rgSCHCmnGetRaType1Mask ARGS((
U8                rbIdx,
U8                rbgSize,
U8                *type1Subset
));
#endif
PRIVATE U32 rgSCHCmnGetRaType0Mask ARGS((
U8                rbIdx,
U8                rbgSize
));
PRIVATE U32 rgSCHCmnGetRaType2Mask ARGS((
U8                rbIdx,
U8                *maskIdx
));
#endif

PUBLIC Bool rgSCHCmnRetxAllocAvoid ARGS(( 
RgSchDlSf                  *subFrm,
RgSchCellCb                *cell,
RgSchDlHqProcCb            *proc
));

PRIVATE U16 rgSCHCmnGetSiSetId ARGS((
U16    sfn,
U8     sf,
U16    minPeriodicity
));

#ifdef TFU_UPGRADE
PRIVATE S16 rgSCHCmnUlMdfyGrntForCqi ARGS((
RgSchUeCb    *ue,
U32          maxRb,
U32          *numSb,
U8           *iTbs,
U32          hqSz,
U32          stepDownItbs,
U32          effTgt
));
#endif


/* CQI Offset Index to Beta CQI Offset value mapping,
 * stored as parts per 1000. Reserved is set to 0.
 * Refer 36.213 sec 8.6.3 Tbl 8.6.3-3 */
PUBLIC U32 rgSchCmnBetaCqiOffstTbl[16] = {0, 0, 1125,
   1250, 1375, 1625, 1750, 2000, 2250, 2500, 2875,
   3125, 3500, 4000, 5000, 6250};
PUBLIC U32 rgSchCmnBetaHqOffstTbl[16] =  {2000, 2500, 3125, 
   4000, 5000, 6250, 8000,10000, 12625, 15875, 20000, 
   31000, 50000,80000,126000,0};
PUBLIC U32 rgSchCmnBetaRiOffstTbl[16] = {1250, 1625, 2000, 
   2500, 3125, 4000, 5000, 6250, 8000, 10000,12625,
   15875,20000,0,0,0};
PUBLIC S8 rgSchCmnDlCqiDiffOfst[8] = {0, 1, 2, 3, -4, -3, -2, -1};

/* Include CRS REs while calculating Efficiency */
CONSTANT PRIVATE U8 rgSchCmnAntIdx[5] = {0,0,1,0,2};
CONSTANT PRIVATE U8 rgSchCmnNumResForCrs[5] = {0,6,12,0,16};
U32 cfiSwitchCnt ;
U32 cfiIncr ;
U32 cfiDecr ;


#ifdef TFU_UPGRADE
PUBLIC S8 rgSchCmnApUeSelDiffCqi[4] = {1, 2, 3, 4};
PUBLIC S8 rgSchCmnApEnbConfDiffCqi[4] = {0, 1, 2, -1};
#endif

typedef struct rgSchCmnDlUeDciFrmtOptns
{
  TfuDciFormat spfcDciFrmt;   /* TM(Transmission Mode) specific DCI format.
                               * Search space : UE Specific by C-RNTI only. */
  U8           spfcDciRAType; /* Resource Alloctn(RA) type for spfcDciFrmt */
  TfuDciFormat prfrdDciFrmt;  /* Preferred DCI format among the available
                               * options for TD (Transmit Diversity) */
  U8           prfrdDciRAType; /* Resource Alloctn(RA) type for prfrdDciFrmt */
}RgSchCmnDlUeDciFrmtOptns;
/* DCI Format options for each Transmission Mode */
RgSchCmnDlUeDciFrmtOptns rgSchCmnDciFrmtOptns[7] = {
   {TFU_DCI_FORMAT_1, RG_SCH_CMN_RA_TYPE0, TFU_DCI_FORMAT_1A, RG_SCH_CMN_RA_TYPE2},
   {TFU_DCI_FORMAT_1, RG_SCH_CMN_RA_TYPE0, TFU_DCI_FORMAT_1A, RG_SCH_CMN_RA_TYPE2},
   {TFU_DCI_FORMAT_2A,RG_SCH_CMN_RA_TYPE0, TFU_DCI_FORMAT_1A, RG_SCH_CMN_RA_TYPE2},
   {TFU_DCI_FORMAT_2, RG_SCH_CMN_RA_TYPE0, TFU_DCI_FORMAT_1A, RG_SCH_CMN_RA_TYPE2},
   {TFU_DCI_FORMAT_1D,RG_SCH_CMN_RA_TYPE2, TFU_DCI_FORMAT_1A, RG_SCH_CMN_RA_TYPE2},
   {TFU_DCI_FORMAT_1B,RG_SCH_CMN_RA_TYPE2, TFU_DCI_FORMAT_1A, RG_SCH_CMN_RA_TYPE2},
   {TFU_DCI_FORMAT_1, RG_SCH_CMN_RA_TYPE0, TFU_DCI_FORMAT_1A, RG_SCH_CMN_RA_TYPE2}
};

typedef struct rgSchCmnDlImcsTbl
{
  U8   modOdr; /* Modulation Order */
  U8   iTbs;   /* ITBS */
}RgSchCmnDlImcsTbl[29];

CONSTANT struct rgSchCmnMult235Info
{
   U8   match;    /* Closest number satisfying 2^a.3^b.5^c, with a bias
                  * towards the smaller number */
   U8   prvMatch; /* Closest number not greater than array index
                  * satisfying 2^a.3^b.5^c */
} rgSchCmnMult235Tbl[110+1] = {
   {0, 0},  /* dummy */
   {1, 1}, {2, 2}, {3, 3}, {4, 4}, {5, 5}, {6, 6}, {6, 6}, {8, 8},
   {9, 9}, {10, 10}, {10, 10}, {12, 12}, {12, 12}, {15, 12}, {15, 15},
   {16, 16}, {16, 16}, {18, 18}, {18, 18}, {20, 20}, {20, 20}, {20, 20},
   {24, 20}, {24, 24}, {25, 25}, {25, 25}, {27, 27}, {27, 27}, {30, 27},
   {30, 30}, {30, 30}, {32, 32}, {32, 32}, {32, 32}, {36, 32}, {36, 36},
   {36, 36}, {36, 36}, {40, 36}, {40, 40}, {40, 40}, {40, 40}, {45, 40},
   {45, 40}, {45, 45}, {45, 45}, {48, 45}, {48, 48}, {48, 48}, {50, 50},
   {50, 50}, {50, 50}, {54, 50}, {54, 54}, {54, 54}, {54, 54}, {54, 54},
   {60, 54}, {60, 54}, {60, 60}, {60, 60}, {60, 60}, {64, 60}, {64, 64},
   {64, 64}, {64, 64}, {64, 64}, {64, 64}, {72, 64}, {72, 64}, {72, 64},
   {72, 72}, {72, 72}, {75, 72}, {75, 75}, {75, 75}, {75, 75}, {80, 75},
   {80, 75}, {80, 80}, {81, 81}, {81, 81}, {81, 81}, {81, 81}, {81, 81},
   {90, 81}, {90, 81}, {90, 81}, {90, 81}, {90, 90}, {90, 90}, {90, 90},
   {90, 90}, {96, 90}, {96, 90}, {96, 96}, {96, 96}, {96, 96}, {100, 96},
   {100, 100}, {100, 100}, {100, 100}, {100, 100}, {100, 100}, {108, 100},
   {108, 100}, {108, 100}, {108, 108}, {108, 108}, {108, 108}
};

/* R8 Upgrade */
/* BI table from 36.321 Table 7.2.1 */
CONSTANT PRIVATE S16 rgSchCmnBiTbl[RG_SCH_CMN_NUM_BI_VAL] = {
      0, 10, 20, 30,40,60,80,120,160,240,320,480,960};
PUBLIC RgSchCmnUlCqiInfo rgSchCmnUlCqiTbl[RG_SCH_CMN_UL_NUM_CQI] = {
 {     0,                0              },
 {RGSCH_CMN_QM_CQI_1,RGSCH_CMN_UL_EFF_CQI_1 },
 {RGSCH_CMN_QM_CQI_2,RGSCH_CMN_UL_EFF_CQI_2 },
 {RGSCH_CMN_QM_CQI_3,RGSCH_CMN_UL_EFF_CQI_3 },
 {RGSCH_CMN_QM_CQI_4,RGSCH_CMN_UL_EFF_CQI_4 },
 {RGSCH_CMN_QM_CQI_5,RGSCH_CMN_UL_EFF_CQI_5 },
 {RGSCH_CMN_QM_CQI_6,RGSCH_CMN_UL_EFF_CQI_6 },
 {RGSCH_CMN_QM_CQI_7,RGSCH_CMN_UL_EFF_CQI_7 },
 {RGSCH_CMN_QM_CQI_8,RGSCH_CMN_UL_EFF_CQI_8 },
 {RGSCH_CMN_QM_CQI_9,RGSCH_CMN_UL_EFF_CQI_9 },
 {RGSCH_CMN_QM_CQI_10,RGSCH_CMN_UL_EFF_CQI_10 },
 {RGSCH_CMN_QM_CQI_11,RGSCH_CMN_UL_EFF_CQI_11 },
 {RGSCH_CMN_QM_CQI_12,RGSCH_CMN_UL_EFF_CQI_12 },
 {RGSCH_CMN_QM_CQI_13,RGSCH_CMN_UL_EFF_CQI_13 },
 {RGSCH_CMN_QM_CQI_14,RGSCH_CMN_UL_EFF_CQI_14 },
 {RGSCH_CMN_QM_CQI_15,RGSCH_CMN_UL_EFF_CQI_15 },
};

#ifdef RG_UNUSED
/* This table maps a (delta_offset * 2 + 2) to a (beta * 8)
 * where beta is 10^-(delta_offset/10) rounded off to nearest 1/8
 */
PRIVATE U16 rgSchCmnUlBeta8Tbl[29] = {
   6, RG_SCH_CMN_UL_INVALID_BETA8, 8, 9, 10, 11, 13, 14, 16, 18, 20, 23,
   25, 28, 32, RG_SCH_CMN_UL_INVALID_BETA8, 40, RG_SCH_CMN_UL_INVALID_BETA8,
   50, RG_SCH_CMN_UL_INVALID_BETA8, 64, RG_SCH_CMN_UL_INVALID_BETA8, 80,
   RG_SCH_CMN_UL_INVALID_BETA8, 101, RG_SCH_CMN_UL_INVALID_BETA8, 127,
   RG_SCH_CMN_UL_INVALID_BETA8, 160
};
#endif

/* QCI to SVC priority mapping. Index specifies the Qci*/
PRIVATE U8 rgSchCmnDlQciPrio[RG_SCH_CMN_MAX_QCI] = RG_SCH_CMN_QCI_TO_PRIO;

/* The configuration is efficiency measured per 1024 REs.  */
/* The first element stands for when CQI is not known      */
/* This table is used to translate CQI to its corrospoding */
/* allocation parameters. These are currently from 36.213  */
/* Just this talbe needs to be edited for modifying the    */
/* the resource allocation behaviour                       */

/* ADD CQI to MCS mapping correction
 * single dimensional array is replaced by 2 dimensions for different CFI*/
PRIVATE U16 rgSchCmnCqiPdschEff[4][16] = {RG_SCH_CMN_CQI_TO_PDSCH_EFF_CFI0 ,RG_SCH_CMN_CQI_TO_PDSCH_EFF_CFI1,
    RG_SCH_CMN_CQI_TO_PDSCH_EFF_CFI2,RG_SCH_CMN_CQI_TO_PDSCH_EFF_CFI3};

PRIVATE U16 rgSchCmn2LyrCqiPdschEff[4][16] = {RG_SCH_CMN_2LYR_CQI_TO_PDSCH_EFF_CFI0 ,RG_SCH_CMN_2LYR_CQI_TO_PDSCH_EFF_CFI1,
    RG_SCH_CMN_2LYR_CQI_TO_PDSCH_EFF_CFI2, RG_SCH_CMN_2LYR_CQI_TO_PDSCH_EFF_CFI3};

/* This configuration determines the transalation of a UEs CQI to its    */
/* PDCCH coding efficiency. This may be edited based on the installation */
PRIVATE U8 rgSchCmnDlRvTbl[4] = {0, 2, 3, 1}; /* RVIdx sequence is corrected*/

/* Indexed by [DciFrmt].
 * Considering the following definition in determining the dciFrmt index.
 * typedef enum
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
} TfuDciFormat;
*/
PRIVATE U16 rgSchCmnDciFrmtSizes[10];


PRIVATE U16 rgSchCmnCqiPdcchEff[16] = RG_SCH_CMN_CQI_TO_PDCCH_EFF;

#ifdef LTE_TDD

PUBLIC RgSchTddUlDlSubfrmTbl rgSchTddUlDlSubfrmTbl = {
   {RG_SCH_TDD_DL_SUBFRAME, RG_SCH_TDD_SPL_SUBFRAME, RG_SCH_TDD_UL_SUBFRAME,  RG_SCH_TDD_UL_SUBFRAME,  RG_SCH_TDD_UL_SUBFRAME,  RG_SCH_TDD_DL_SUBFRAME,  RG_SCH_TDD_SPL_SUBFRAME, RG_SCH_TDD_UL_SUBFRAME,  RG_SCH_TDD_UL_SUBFRAME,  RG_SCH_TDD_UL_SUBFRAME},
   {RG_SCH_TDD_DL_SUBFRAME, RG_SCH_TDD_SPL_SUBFRAME, RG_SCH_TDD_UL_SUBFRAME,  RG_SCH_TDD_UL_SUBFRAME,  RG_SCH_TDD_DL_SUBFRAME,  RG_SCH_TDD_DL_SUBFRAME,  RG_SCH_TDD_SPL_SUBFRAME, RG_SCH_TDD_UL_SUBFRAME,  RG_SCH_TDD_UL_SUBFRAME,  RG_SCH_TDD_DL_SUBFRAME},
   {RG_SCH_TDD_DL_SUBFRAME, RG_SCH_TDD_SPL_SUBFRAME, RG_SCH_TDD_UL_SUBFRAME,  RG_SCH_TDD_DL_SUBFRAME,  RG_SCH_TDD_DL_SUBFRAME,  RG_SCH_TDD_DL_SUBFRAME,  RG_SCH_TDD_SPL_SUBFRAME, RG_SCH_TDD_UL_SUBFRAME,  RG_SCH_TDD_DL_SUBFRAME,  RG_SCH_TDD_DL_SUBFRAME},
   {RG_SCH_TDD_DL_SUBFRAME, RG_SCH_TDD_SPL_SUBFRAME, RG_SCH_TDD_UL_SUBFRAME,  RG_SCH_TDD_UL_SUBFRAME, RG_SCH_TDD_UL_SUBFRAME,   RG_SCH_TDD_DL_SUBFRAME,  RG_SCH_TDD_DL_SUBFRAME,  RG_SCH_TDD_DL_SUBFRAME,  RG_SCH_TDD_DL_SUBFRAME,  RG_SCH_TDD_DL_SUBFRAME},
   {RG_SCH_TDD_DL_SUBFRAME, RG_SCH_TDD_SPL_SUBFRAME, RG_SCH_TDD_UL_SUBFRAME,  RG_SCH_TDD_UL_SUBFRAME, RG_SCH_TDD_DL_SUBFRAME,   RG_SCH_TDD_DL_SUBFRAME,  RG_SCH_TDD_DL_SUBFRAME,  RG_SCH_TDD_DL_SUBFRAME,  RG_SCH_TDD_DL_SUBFRAME,  RG_SCH_TDD_DL_SUBFRAME},
   {RG_SCH_TDD_DL_SUBFRAME, RG_SCH_TDD_SPL_SUBFRAME, RG_SCH_TDD_UL_SUBFRAME,  RG_SCH_TDD_DL_SUBFRAME, RG_SCH_TDD_DL_SUBFRAME,   RG_SCH_TDD_DL_SUBFRAME,  RG_SCH_TDD_DL_SUBFRAME,  RG_SCH_TDD_DL_SUBFRAME,  RG_SCH_TDD_DL_SUBFRAME,  RG_SCH_TDD_DL_SUBFRAME},
   {RG_SCH_TDD_DL_SUBFRAME, RG_SCH_TDD_SPL_SUBFRAME, RG_SCH_TDD_UL_SUBFRAME,  RG_SCH_TDD_UL_SUBFRAME,  RG_SCH_TDD_UL_SUBFRAME,  RG_SCH_TDD_DL_SUBFRAME,  RG_SCH_TDD_SPL_SUBFRAME, RG_SCH_TDD_UL_SUBFRAME,  RG_SCH_TDD_UL_SUBFRAME,  RG_SCH_TDD_DL_SUBFRAME}
};

/* SPS_INTG_FIX */
#ifdef LTEMAC_SPS
PUBLIC U8 rgSchTddSpsDlMaxRetxTbl[RGSCH_MAX_TDD_UL_DL_CFG] = {
   /* 0 */ 6,
   /* 1 */ 7,
   /* 2 */ 8,
   /* 3 */ 11,
   /* 4 */ 12,
   /* 5 */ 13,
   /* 6 */ 7};

#endif


/* Special Subframes in OFDM symbols */
/* ccpu00134197-MOD-Correct the number of symbols */
PUBLIC RgSchTddSplSubfrmInfoTbl rgSchTddSplSubfrmInfoTbl = {
        {3,  1, 1, 3,   1, 1},
        {9,  1, 1, 8,   1, 1},
        {10, 1, 1, 9,   1, 1},
        {11, 1, 1, 10,  1, 1},
        {12, 1, 1, 3,   2, 2},
        {3,  2, 2, 8,   2, 2},
        {9,  2, 2, 9,   2, 2},
        {10, 2, 2, 0,   0, 0},
        {11, 2, 2, 0,   0, 0}
};

/* PHICH 'm' value Table */
PUBLIC RgSchTddPhichMValTbl rgSchTddPhichMValTbl = {
        {2, 1, 0, 0, 0, 2, 1, 0, 0, 0},
        {0, 1, 0, 0, 1, 0, 1, 0, 0, 1},
        {0, 0, 0, 1, 0, 0, 0, 0, 1, 0},
        {1, 0, 0, 0, 0, 0, 0, 0, 1, 1},
        {0, 0, 0, 0, 0, 0, 0, 0, 1, 1},
        {0, 0, 0, 0, 0, 0, 0, 0, 1, 0},
        {1, 1, 0, 0, 0, 1, 1, 0, 0, 1}
};

/* PHICH 'K' value Table */
PUBLIC RgSchTddKPhichTbl rgSchTddKPhichTbl = {
        {0, 0, 4, 7, 6, 0, 0, 4, 7, 6},
        {0, 0, 4, 6, 0, 0, 0, 4, 6, 0},
        {0, 0, 6, 0, 0, 0, 0, 6, 0, 0},
        {0, 0, 6, 6, 6, 0, 0, 0, 0, 0},
        {0, 0, 6, 6, 0, 0, 0, 0, 0, 0},
        {0, 0, 6, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 4, 6, 6, 0, 0, 4, 7, 0}
};

/* Uplink association index 'K' value Table */
PUBLIC RgSchTddUlAscIdxKDashTbl rgSchTddUlAscIdxKDashTbl = {
        {0, 0, 6, 4, 0, 0, 0, 6, 4, 0},
        {0, 0, 4, 0, 0, 0, 0, 4, 0, 0},
        {0, 0, 4, 4, 4, 0, 0, 0, 0, 0},
        {0, 0, 4, 4, 0, 0, 0, 0, 0, 0},
        {0, 0, 4, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 7, 7, 5, 0, 0, 7, 7, 0}
};


/* PUSCH 'K' value Table */
PUBLIC RgSchTddPuschTxKTbl rgSchTddPuschTxKTbl = {
        {4, 6, 0, 0, 0, 4, 6, 0, 0, 0},
        {0, 6, 0, 0, 4, 0, 6, 0, 0, 4},
        {0, 0, 0, 4, 0, 0, 0, 0, 4, 0},
        {4, 0, 0, 0, 0, 0, 0, 0, 4, 4},
        {0, 0, 0, 0, 0, 0, 0, 0, 4, 4},
        {0, 0, 0, 0, 0, 0, 0, 0, 4, 0},
        {7, 7, 0, 0, 0, 7, 7, 0, 0, 5}
};

/* PDSCH to PUCCH Table for DL Harq Feed back. Based on the 
   Downlink association set index 'K' table */
PUBLIC U8 rgSchTddPucchTxTbl[7][10] = {
        {4,  6,  0, 0, 0, 4, 6, 0, 0,  0},
        {7,  6,  0, 0, 4, 7, 6, 0, 0,  4},
        {7,  6,  0, 4, 8, 7, 6, 0, 4,  8},
        {4,  11, 0, 0, 0, 7, 6, 6, 5,  5},
        {12, 11, 0, 0, 8, 7, 7, 6, 5,  4},
        {12, 11, 0, 9, 8, 7, 6, 5, 4, 13},
        {7,  7,  0, 0, 0, 7, 7, 0, 0,  5}
};

/* Table to fetch the next DL sf idx for applying the 
   new CFI. The next Dl sf Idx at which the new CFI 
   is applied is always the starting Sf of the next ACK/NACK
   Fdbk bundle. 
   
   Ex: In Cfg-2, sf4 and sf9 are the only subframes at which 
       a new ACK/NACK bundle of DL subframes can start
       
   D  S  U  D  D  D  S  U  D  D  D  S  U  D  D  D  S  U  D  D    
               4              9
   
   dlSf Array for Cfg-2:
   sfNum:  0  1  3  4  5  6  8  9  0  1   3  4  5  6  8  9 
   sfIdx:  0  1  2  3  4  5  6  7  8  9  10 11 12 12 14 15 
    
   If CFI changes at sf0,  nearest DL SF bundle >= 4 TTI is sf4
   So at sf4 the new CFI can be applied. To arrive at sf4 from
   sf0, the sfIdx has to be increased by 3 */  
                 
PUBLIC U8 rgSchTddPdcchSfIncTbl[7][10] = {
 /* A/N Bundl: 0,1,5,6*/   {2,  1,  0, 0, 0, 2, 1,  0,  0,  0},
 /* A/N Bundl: 0,4,5,9*/   {2,  2,  0, 0, 3, 2, 2,  0,  0,  3},
 /* A/N Bundl: 4,9*/       {3,  6,  0, 5, 4, 3, 6,  0,  5,  4},
 /* A/N Bundl: 1,7,9*/     {4,  3,  0, 0, 0, 4, 5,  4,  6,  5},
 /* A/N Bundl: 0,6*/       {4,  3,  0, 0, 6, 5, 4,  7,  6,  5},
 /* A/N Bundl: 9*/         {8,  7,  0, 6, 5, 4, 12, 11, 10, 9},
 /* A/N Bundl: 0,1,5,6,9*/ {2,  1,  0, 0, 0, 2, 2,  0,  0,  3}
};
   

/* combine compilation fixes */
#ifdef LTEMAC_SPS
/* subframe offset values to be used when twoIntervalsConfig is enabled in UL
 * SPS for a UE */
PUBLIC RgSchTddSfOffTbl rgSchTddSfOffTbl = {
        {0, 0, 0,  0,  0, 0, 0,  0,  0, 0},
        {0, 0, 1, -1,  0, 0, 0,  1, -1, 0},
        {0, 0, 5,  0,  0, 0, 0, -5,  0, 0},
        {0, 0, 1,  1, -2, 0, 0,  0,  0, 0},
        {0, 0, 1, -1,  0, 0, 0,  0,  0, 0},
        {0, 0, 0,  0,  0, 0, 0,  0,  0, 0},
        {0, 0, 0,  0,  0, 0, 0,  0,  0, 0}
};


/* Table to determine when uplink SPS configured grants should
 * explicitly be reserved in a subframe. When enries are same
 * as that of Msg3SubfrmTbl, indicates competition with msg3.
 * As of now, this is same as Msg3SubfrmTbl (leaving out uldlcfg 2),
 * except that all 255s are now zeros. */
PUBLIC RgSchTddSpsUlRsrvTbl rgSchTddSpsUlRsrvTbl = {
        {0,    0,  0,  6,  8,  0, 0,  0,  6,  8},
        {0,    0,  6,  9,  0,  0, 0,  6,  9,  0},
        {0,    0,  10,  0,  0,  0, 0,  10,  0,  0},
        {0,   0,  0,  0,  8,  0, 7,  7,  14,  0},
        {0,   0,  0,  9,  0,  0, 7,  15,  0, 0},
        {0,   0,  10,  0,  0,  0, 16,  0, 0, 0},
        {0,    0,  0,  0,  8,  0, 0,  0,  9,  0}
};

/* Inverse DL Assoc Set index Table */
PUBLIC RgSchTddInvDlAscSetIdxTbl rgSchTddInvDlAscSetIdxTbl = {
       {4,  6,  0, 0, 0, 4, 6, 0, 0,  0},
       {7,  6,  0, 0, 4, 7, 6, 0, 0,  4},
       {7,  6,  0, 4, 8, 7, 6, 0, 4,  8},
       {4,  11, 0, 0, 0, 7, 6, 6, 5,  5},
       {12, 11, 0, 0, 8, 7, 7, 6, 5,  4},
       {12, 11, 0, 9, 8, 7, 6, 5, 4, 13},
       {7,  7,  0, 0, 0, 7, 7, 0, 0,  5}
};

#endif /* (LTEMAC_SPS ) */

/* Number of Uplink subframes Table */
PRIVATE U8 rgSchTddNumUlSf[] = {6, 4, 2, 3, 2, 1, 5};

/* Downlink HARQ processes Table */
PUBLIC RgSchTddUlNumHarqProcTbl rgSchTddUlNumHarqProcTbl = { 7, 4, 2, 3, 2, 1, 6};

/* Uplink HARQ processes Table */
PUBLIC RgSchTddDlNumHarqProcTbl rgSchTddDlNumHarqProcTbl = { 4, 7, 10, 9, 12, 15, 6};

/* Downlink association index set 'K' value Table */
PUBLIC RgSchTddDlAscSetIdxKTbl rgSchTddDlAscSetIdxKTbl = {
        { {0, {0}}, {0, {0}}, {1, {6}}, {0, {0}}, {1, {4}}, {0, {0}}, {0, {0}}, {1, {6}}, {0, {0}}, {1, {4}} },

        { {0, {0}}, {0, {0}}, {2, {7, 6}}, {1, {4}}, {0, {0}}, {0, {0}}, {0, {0}}, {2, {7, 6}}, {1, {4}}, {0, {0}} },

        { {0, {0}}, {0, {0}}, {4, {8, 7, 4, 6}}, {0, {0}}, {0, {0}}, {0, {0}}, {0, {0}}, {4, {8, 7, 4, 6}}, {0, {0}}, {0, {0}} },

        { {0, {0}}, {0, {0}}, {3, {7, 6, 11}}, {2, {6, 5}}, {2, {5, 4}}, {0, {0}}, {0, {0}}, {0, {0}}, {0, {0}}, {0, {0}} },

        { {0, {0}}, {0, {0}}, {4, {12, 8, 7, 11}}, {4, {6, 5, 4, 7}}, {0, {0}}, {0, {0}}, {0, {0}}, {0, {0}}, {0, {0}}, {0, {0}} },

        { {0, {0}}, {0, {0}}, {9, {13, 12, 9, 8, 7, 5, 4, 11, 6}}, {0, {0}}, {0, {0}}, {0, {0}}, {0, {0}}, {0, {0}}, {0, {0}}, {0, {0}} },

        { {0, {0}}, {0, {0}}, {1, {7}}, {1, {7}}, {1, {5}}, {0, {0}}, {0, {0}}, {1, {7}}, {1, {7}}, {0, {0}} }
};

 /* ccpu132282-ADD-the table rgSchTddDlAscSetIdxKTbl is rearranged in 
  * decreasing order of Km, this is used to calculate the NCE used for 
  * calculating N1Pucch Resource for Harq*/
PUBLIC RgSchTddDlAscSetIdxKTbl rgSchTddDlHqPucchResCalTbl = {
        { {0, {0}}, {0, {0}}, {1, {6}}, {0, {0}}, {1, {4}}, {0, {0}}, {0, {0}}, {1, {6}}, {0, {0}}, {1, {4}} },

        { {0, {0}}, {0, {0}}, {2, {7, 6}}, {1, {4}}, {0, {0}}, {0, {0}}, {0, {0}}, {2, {7, 6}}, {1, {4}}, {0, {0}} },

        { {0, {0}}, {0, {0}}, {4, {8, 7, 6, 4}}, {0, {0}}, {0, {0}}, {0, {0}}, {0, {0}}, {4, {8, 7, 6, 4}}, {0, {0}}, {0, {0}} },

        { {0, {0}}, {0, {0}}, {3, {11, 7, 6}}, {2, {6, 5}}, {2, {5, 4}}, {0, {0}}, {0, {0}}, {0, {0}}, {0, {0}}, {0, {0}} },

        { {0, {0}}, {0, {0}}, {4, {12, 11, 8, 7}}, {4, {7, 6, 5, 4}}, {0, {0}}, {0, {0}}, {0, {0}}, {0, {0}}, {0, {0}}, {0, {0}} },

        { {0, {0}}, {0, {0}}, {9, {13, 12, 11, 9, 8, 7, 6, 5, 4}}, {0, {0}}, {0, {0}}, {0, {0}}, {0, {0}}, {0, {0}}, {0, {0}}, {0, {0}} },

        { {0, {0}}, {0, {0}}, {1, {7}}, {1, {7}}, {1, {5}}, {0, {0}}, {0, {0}}, {1, {7}}, {1, {7}}, {0, {0}} }
};

/* Minimum number of Ack/Nack feeback information to be
   stored for each UL-DL configuration */
PUBLIC RgSchTddANFdbkMapTbl rgSchTddANFdbkMapTbl = {4, 4, 2, 3, 2, 1, 5};

/* Uplink switch points and number of UL subframes Table */
PUBLIC RgSchTddMaxUlSubfrmTbl rgSchTddMaxUlSubfrmTbl = {
     {2,3,3}, {2,2,2}, {2,1,1}, {1,3,0}, {1,2,0}, {1,1,0}, {2,3,2}
};

/* Uplink switch points and number of DL subframes Table */
PUBLIC RgSchTddMaxDlSubfrmTbl rgSchTddMaxDlSubfrmTbl = {
     {2,2,2}, {2,3,3}, {2,4,4}, {1,7,0}, {1,8,0}, {1,9,0}, {2,2,3}
};

/* Number of UL subframes present before a particular subframe */
PUBLIC RgSchTddNumUlSubfrmTbl rgSchTddNumUlSubfrmTbl = {
        {0, 0, 1, 2, 3, 3, 3, 4, 5, 6},
        {0, 0, 1, 2, 2, 2, 2, 3, 4, 4},
        {0, 0, 1, 1, 1, 1, 1, 2, 2, 2},
        {0, 0, 1, 2, 3, 3, 3, 3, 3, 3},
        {0, 0, 1, 2, 2, 2, 2, 2, 2, 2},
        {0, 0, 1, 1, 1, 1, 1, 1, 1, 1},
        {0, 0, 1, 2, 3, 3, 3, 4, 5, 5}
};

/* Number of DL subframes present till a particular subframe */
PUBLIC RgSchTddNumDlSubfrmTbl rgSchTddNumDlSubfrmTbl = {
        {1, 2, 2, 2, 2, 3, 4, 4, 4, 4},
        {1, 2, 2, 2, 3, 4, 5, 5, 5, 6},
        {1, 2, 2, 3, 4, 5, 6, 6, 7, 8},
        {1, 2, 2, 2, 2, 3, 4, 5, 6, 7},
        {1, 2, 2, 2, 3, 4, 5, 6, 7, 8},
        {1, 2, 2, 3, 4, 5, 6, 7, 8, 9},
        {1, 2, 2, 2, 2, 3, 4, 4, 4, 5}
};


/* Nearest possible UL subframe Index from UL subframe
 * DL Index < UL Index */
PUBLIC RgSchTddLowDlSubfrmIdxTbl rgSchTddLowDlSubfrmIdxTbl = {
        {0, 1, 1, 1, 1, 5, 6, 6, 6, 6},
        {0, 1, 1, 1, 4, 5, 6, 6, 6, 9},
        {0, 1, 1, 3, 4, 5, 6, 6, 8, 9},
        {0, 1, 1, 1, 1, 5, 6, 7, 8, 9},
        {0, 1, 1, 1, 4, 5, 6, 7, 8, 9},
        {0, 1, 1, 3, 4, 5, 6, 7, 8, 9},
        {0, 1, 1, 1, 1, 5, 6, 6, 6, 9}
};

/* Nearest possible DL subframe Index from UL subframe
 * DL Index > UL Index
 * 10 represents Next SFN low DL Idx */
PUBLIC RgSchTddHighDlSubfrmIdxTbl rgSchTddHighDlSubfrmIdxTbl = {
        {0, 1, 5, 5, 5, 5, 6, 10, 10, 10},
        {0, 1, 4, 4, 4, 5, 6,  9,  9,  9},
        {0, 1, 3, 3, 4, 5, 6,  8,  8,  9},
        {0, 1, 5, 5, 5, 5, 6,  7,  8,  9},
        {0, 1, 4, 4, 4, 5, 6,  7,  8,  9},
        {0, 1, 3, 3, 4, 5, 6,  7,  8,  9},
        {0, 1, 5, 5, 5, 5, 6,  9,  9,  9}
};

/* RACH Message3 related information */
PUBLIC RgSchTddMsg3SubfrmTbl rgSchTddMsg3SubfrmTbl = {
        {7,      6,  255,  255,  255,  7,   6,  255,  255,  255},
        {7,      6,  255,  255,    8,  7,   6,  255,  255,    8},
        {7,      6,  255,  9,      8,  7,   6,  255,    9,    8},
        {12,    11,  255,  255,  255,  7,   6,    6,    6,   13},
        {12,    11,  255,  255,    8,  7,   6,    6,   14,   13},
        {12,    11,  255,    9,    8,  7,   6,   15,   14,   13},
        {7,      6,  255,  255,  255,  7,   6,  255,  255,    8}
};

/* ccpu00132341-DEL Removed rgSchTddRlsDlSubfrmTbl and used Kset table for 
 * releasing DL HARQs */

/* DwPTS Scheduling Changes Start */
/* Provides the number of Cell Reference Signals in DwPTS
 * region per RB */
PRIVATE U8  rgSchCmnDwptsCrs[2][3] = {/* [Spl Sf cfg][Ant Port] */
           {4, 8,  16}, /* Spl Sf cfg 1,2,3,6,7,8 */
           {6, 12, 20}, /* Spl Sf cfg 4 */
};

PRIVATE S8  rgSchCmnSplSfDeltaItbs[9] = RG_SCH_DWPTS_ITBS_ADJ;
/* DwPTS Scheduling Changes End */
#endif


PRIVATE U32 rgSchCmnBsrTbl[64] = {
   0, 10, 12, 14, 17, 19, 22, 26,
   31, 36, 42, 49, 57, 67, 78, 91,
   107, 125, 146, 171, 200, 234, 274, 321,
   376, 440, 515, 603, 706, 826, 967, 1132,
   1326, 1552, 1817, 2127, 2490, 2915, 3413, 3995,
   4677, 5476, 6411, 7505, 8787, 10287, 12043, 14099,
   16507, 19325, 22624, 26487, 31009, 36304, 42502, 49759,
   58255, 68201, 79846, 93479, 109439, 128125, 150000, 220000
};

PRIVATE U8 rgSchCmnUlRvIdxToIMcsTbl[4] = {32, 30, 31, 29};

PUBLIC U8 rgSchCmnUlCqiToTbsTbl[RG_SCH_CMN_MAX_CP][RG_SCH_CMN_UL_NUM_CQI];

PUBLIC RgSchTbSzTbl rgTbSzTbl = {
 {
   {16,    32,    56,    88,    120,   152,   176,   208,   224,   256,   288,   328,   344,   376,   392,   424,   456,   488,   504,   536,   568,   600,   616,   648,   680,   712,   744,   776,   776,   808,   840,   872,   904,   936,   968,   1000,  1032,  1032,  1064,  1096,  1128,  1160,  1192,  1224,  1256,  1256,  1288,  1320,  1352,  1384,  1416,  1416,  1480,  1480,  1544,  1544,  1608,  1608,  1608,  1672,  1672,  1736,  1736,  1800,  1800,  1800,  1864,  1864,  1928,  1928,  1992,  1992,  2024,  2088,  2088,  2088,  2152,  2152,  2216,  2216,  2280,  2280,  2280,  2344,  2344,  2408,  2408,  2472,  2472,  2536,  2536,  2536,  2600,  2600,  2664,  2664,  2728,  2728,  2728,  2792,  2792,  2856,  2856,  2856,  2984,  2984,  2984,  2984,  2984,  3112},
   {24,    56,    88,    144,   176,   208,   224,   256,   328,   344,   376,   424,   456,   488,   520,   568,   600,   632,   680,   712,   744,   776,   808,   872,   904,   936,   968,   1000,  1032,  1064,  1128,  1160,  1192,  1224,  1256,  1288,  1352,  1384,  1416,  1416,  1480,  1544,  1544,  1608,  1608,  1672,  1736,  1736,  1800,  1800,  1864,  1864,  1928,  1992,  1992,  2024,  2088,  2088,  2152,  2152,  2216,  2280,  2280,  2344,  2344,  2408,  2472,  2472,  2536,  2536,  2600,  2600,  2664,  2728,  2728,  2792,  2792,  2856,  2856,  2856,  2984,  2984,  2984,  3112,  3112,  3112,  3240,  3240,  3240,  3240,  3368,  3368,  3368,  3496,  3496,  3496,  3496,  3624,  3624,  3624,  3752,  3752,  3752,  3752,  3880,  3880,  3880,  4008,  4008,  4008},
   {32,    72,    144,   176,   208,   256,   296,   328,   376,   424,   472,   520,   568,   616,   648,   696,   744,   776,   840,   872,   936,   968,   1000,  1064,  1096,  1160,  1192,  1256,  1288,  1320,  1384,  1416,  1480,  1544,  1544,  1608,  1672,  1672,  1736,  1800,  1800,  1864,  1928,  1992,  2024,  2088,  2088,  2152,  2216,  2216,  2280,  2344,  2344,  2408,  2472,  2536,  2536,  2600,  2664,  2664,  2728,  2792,  2856,  2856,  2856,  2984,  2984,  3112,  3112,  3112,  3240,  3240,  3240,  3368,  3368,  3368,  3496,  3496,  3496,  3624,  3624,  3624,  3752,  3752,  3880,  3880,  3880,  4008,  4008,  4008,  4136,  4136,  4136,  4264,  4264,  4264,  4392,  4392,  4392,  4584,  4584,  4584,  4584,  4584,  4776,  4776,  4776,  4776,  4968,  4968},
   {40,    104,   176,   208,   256,   328,   392,   440,   504,   568,   616,   680,   744,   808,   872,   904,   968,   1032,  1096,  1160,  1224,  1256,  1320,  1384,  1416,  1480,  1544,  1608,  1672,  1736,  1800,  1864,  1928,  1992,  2024,  2088,  2152,  2216,  2280,  2344,  2408,  2472,  2536,  2536,  2600,  2664,  2728,  2792,  2856,  2856,  2984,  2984,  3112,  3112,  3240,  3240,  3368,  3368,  3496,  3496,  3624,  3624,  3624,  3752,  3752,  3880,  3880,  4008,  4008,  4136,  4136,  4264,  4264,  4392,  4392,  4392,  4584,  4584,  4584,  4776,  4776,  4776,  4776,  4968,  4968,  4968,  5160,  5160,  5160,  5352,  5352,  5352,  5352,  5544,  5544,  5544,  5736,  5736,  5736,  5736,  5992,  5992,  5992,  5992,  6200,  6200,  6200,  6200,  6456,  6456},
   {56,    120,   208,   256,   328,   408,   488,   552,   632,   696,   776,   840,   904,   1000,  1064,  1128,  1192,  1288,  1352,  1416,  1480,  1544,  1608,  1736,  1800,  1864,  1928,  1992,  2088,  2152,  2216,  2280,  2344,  2408,  2472,  2600,  2664,  2728,  2792,  2856,  2984,  2984,  3112,  3112,  3240,  3240,  3368,  3496,  3496,  3624,  3624,  3752,  3752,  3880,  4008,  4008,  4136,  4136,  4264,  4264,  4392,  4392,  4584,  4584,  4584,  4776,  4776,  4968,  4968,  4968,  5160,  5160,  5160,  5352,  5352,  5544,  5544,  5544,  5736,  5736,  5736,  5992,  5992,  5992,  5992,  6200,  6200,  6200,  6456,  6456,  6456,  6456,  6712,  6712,  6712,  6968,  6968,  6968,  6968,  7224,  7224,  7224,  7480,  7480,  7480,  7480,  7736,  7736,  7736,  7992},
   {72,    144,   224,   328,   424,   504,   600,   680,   776,   872,   968,   1032,  1128,  1224,  1320,  1384,  1480,  1544,  1672,  1736,  1864,  1928,  2024,  2088,  2216,  2280,  2344,  2472,  2536,  2664,  2728,  2792,  2856,  2984,  3112,  3112,  3240,  3368,  3496,  3496,  3624,  3752,  3752,  3880,  4008,  4008,  4136,  4264,  4392,  4392,  4584,  4584,  4776,  4776,  4776,  4968,  4968,  5160,  5160,  5352,  5352,  5544,  5544,  5736,  5736,  5736,  5992,  5992,  5992,  6200,  6200,  6200,  6456,  6456,  6712,  6712,  6712,  6968,  6968,  6968,  7224,  7224,  7224,  7480,  7480,  7480,  7736,  7736,  7736,  7992,  7992,  7992,  8248,  8248,  8248,  8504,  8504,  8760,  8760,  8760,  8760,  9144,  9144,  9144,  9144,  9528,  9528,  9528,  9528,  9528},
   {328,    176,   256,   392,   504,   600,   712,   808,   936,   1032,  1128,  1224,  1352,  1480,  1544,  1672,  1736,  1864,  1992,  2088,  2216,  2280,  2408,  2472,  2600,  2728,  2792,  2984,  2984,  3112,  3240,  3368,  3496,  3496,  3624,  3752,  3880,  4008,  4136,  4136,  4264,  4392,  4584,  4584,  4776,  4776,  4968,  4968,  5160,  5160,  5352,  5352,  5544,  5736,  5736,  5992,  5992,  5992,  6200,  6200,  6456,  6456,  6456,  6712,  6712,  6968,  6968,  6968,  7224,  7224,  7480,  7480,  7736,  7736,  7736,  7992,  7992,  8248,  8248,  8248,  8504,  8504,  8760,  8760,  8760,  9144,  9144,  9144,  9144,  9528,  9528,  9528,  9528,  9912,  9912,  9912,  10296, 10296, 10296, 10296, 10680, 10680, 10680, 10680, 11064, 11064, 11064, 11448, 11448, 11448},
   {104,   224,   328,   472,   584,   712,   840,   968,   1096,  1224,  1320,  1480,  1608,  1672,  1800,  1928,  2088,  2216,  2344,  2472,  2536,  2664,  2792,  2984,  3112,  3240,  3368,  3368,  3496,  3624,  3752,  3880,  4008,  4136,  4264,  4392,  4584,  4584,  4776,  4968,  4968,  5160,  5352,  5352,  5544,  5736,  5736,  5992,  5992,  6200,  6200,  6456,  6456,  6712,  6712,  6712,  6968,  6968,  7224,  7224,  7480,  7480,  7736,  7736,  7992,  7992,  8248,  8248,  8504,  8504,  8760,  8760,  8760,  9144,  9144,  9144,  9528,  9528,  9528,  9912,  9912,  9912,  10296, 10296, 10296, 10680, 10680, 10680, 11064, 11064, 11064, 11448, 11448, 11448, 11448, 11832, 11832, 11832, 12216, 12216, 12216, 12576, 12576, 12576, 12960, 12960, 12960, 12960, 13536, 13536},
   {120,   256,   392,   536,   680,   808,   968,   1096,  1256,  1384,  1544,  1672,  1800,  1928,  2088,  2216,  2344,  2536,  2664,  2792,  2984,  3112,  3240,  3368,  3496,  3624,  3752,  3880,  4008,  4264,  4392,  4584,  4584,  4776,  4968,  4968,  5160,  5352,  5544,  5544,  5736,  5992,  5992,  6200,  6200,  6456,  6456,  6712,  6968,  6968,  7224,  7224,  7480,  7480,  7736,  7736,  7992,  7992,  8248,  8504,  8504,  8760,  8760,  9144,  9144,  9144,  9528,  9528,  9528,  9912,  9912,  9912,  10296, 10296, 10680, 10680, 10680, 11064, 11064, 11064, 11448, 11448, 11448, 11832, 11832, 12216, 12216, 12216, 12576, 12576, 12576, 12960, 12960, 12960, 13536, 13536, 13536, 13536, 14112, 14112, 14112, 14112, 14688, 14688, 14688, 14688, 15264, 15264, 15264, 15264},
   {136,   296,   456,   616,   776,   936,   1096,  1256,  1416,  1544,  1736,  1864,  2024,  2216,  2344,  2536,  2664,  2856,  2984,  3112,  3368,  3496,  3624,  3752,  4008,  4136,  4264,  4392,  4584,  4776,  4968,  5160,  5160,  5352,  5544,  5736,  5736,  5992,  6200,  6200,  6456,  6712,  6712,  6968,  6968,  7224,  7480,  7480,  7736,  7992,  7992,  8248,  8248,  8504,  8760,  8760,  9144,  9144,  9144,  9528,  9528,  9912,  9912,  10296, 10296, 10296, 10680, 10680, 11064, 11064, 11064, 11448, 11448, 11832, 11832, 11832, 12216, 12216, 12576, 12576, 12960, 12960, 12960, 13536, 13536, 13536, 13536, 14112, 14112, 14112, 14112, 14688, 14688, 14688, 15264, 15264, 15264, 15264, 15840, 15840, 15840, 16416, 16416, 16416, 16416, 16992, 16992, 16992, 16992, 17568},
   {144,   328,   504,   680,   872,   1032,  1224,  1384,  1544,  1736,  1928,  2088,  2280,  2472,  2664,  2792,  2984,  3112,  3368,  3496,  3752,  3880,  4008,  4264,  4392,  4584,  4776,  4968,  5160,  5352,  5544,  5736,  5736,  5992,  6200,  6200,  6456,  6712,  6712,  6968,  7224,  7480,  7480,  7736,  7992,  7992,  8248,  8504,  8504,  8760,  9144,  9144,  9144,  9528,  9528,  9912,  9912,  10296, 10296, 10680, 10680, 11064, 11064, 11448, 11448, 11448, 11832, 11832, 12216, 12216, 12576, 12576, 12960, 12960, 12960, 13536, 13536, 13536, 14112, 14112, 14112, 14688, 14688, 14688, 14688, 15264, 15264, 15264, 15840, 15840, 15840, 16416, 16416, 16416, 16992, 16992, 16992, 16992, 17568, 17568, 17568, 18336, 18336, 18336, 18336, 18336, 19080, 19080, 19080, 19080},
   {176,   376,   584,   776,   1000,  1192,  1384,  1608,  1800,  2024,  2216,  2408,  2600,  2792,  2984,  3240,  3496,  3624,  3880,  4008,  4264,  4392,  4584,  4776,  4968,  5352,  5544,  5736,  5992,  5992,  6200,  6456,  6712,  6968,  6968,  7224,  7480,  7736,  7736,  7992,  8248,  8504,  8760,  8760,  9144,  9144,  9528,  9528,  9912,  9912,  10296, 10680, 10680, 11064, 11064, 11448, 11448, 11832, 11832, 12216, 12216, 12576, 12576, 12960, 12960, 13536, 13536, 13536, 14112, 14112, 14112, 14688, 14688, 14688, 15264, 15264, 15840, 15840, 15840, 16416, 16416, 16416, 16992, 16992, 16992, 17568, 17568, 17568, 18336, 18336, 18336, 18336, 19080, 19080, 19080, 19080, 19848, 19848, 19848, 19848, 20616, 20616, 20616, 21384, 21384, 21384, 21384, 22152, 22152, 22152},
   {208,   440,   680,   904,   1128,  1352,  1608,  1800,  2024,  2280,  2472,  2728,  2984,  3240,  3368,  3624,  3880,  4136,  4392,  4584,  4776,  4968,  5352,  5544,  5736,  5992,  6200,  6456,  6712,  6712,  6968,  7224,  7480,  7736,  7992,  8248,  8504,  8760,  8760,  9144,  9528,  9528,  9912,  9912,  10296, 10680, 10680, 11064, 11064, 11448, 11832, 11832, 12216, 12216, 12576, 12576, 12960, 12960, 13536, 13536, 14112, 14112, 14112, 14688, 14688, 15264, 15264, 15264, 15840, 15840, 16416, 16416, 16416, 16992, 16992, 17568, 17568, 17568, 18336, 18336, 18336, 19080, 19080, 19080, 19080, 19848, 19848, 19848, 20616, 20616, 20616, 21384, 21384, 21384, 21384, 22152, 22152, 22152, 22920, 22920, 22920, 23688, 23688, 23688, 23688, 24496, 24496, 24496, 24496, 25456},
   {224,   488,   744,   1000,  1256,  1544,  1800,  2024,  2280,  2536,  2856,  3112,  3368,  3624,  3880,  4136,  4392,  4584,  4968,  5160,  5352,  5736,  5992,  6200,  6456,  6712,  6968,  7224,  7480,  7736,  7992,  8248,  8504,  8760,  9144,  9144,  9528,  9912,  9912,  10296, 10680, 10680, 11064, 11448, 11448, 11832, 12216, 12216, 12576, 12960, 12960, 13536, 13536, 14112, 14112, 14688, 14688, 14688, 15264, 15264, 15840, 15840, 16416, 16416, 16992, 16992, 16992, 17568, 17568, 18336, 18336, 18336, 19080, 19080, 19080, 19848, 19848, 19848, 20616, 20616, 20616, 21384, 21384, 21384, 22152, 22152, 22152, 22920, 22920, 22920, 23688, 23688, 23688, 24496, 24496, 24496, 25456, 25456, 25456, 25456, 26416, 26416, 26416, 26416, 27376, 27376, 27376, 27376, 28336, 28336},
   {256,   552,   840,   1128,  1416,  1736,  1992,  2280,  2600,  2856,  3112,  3496,  3752,  4008,  4264,  4584,  4968,  5160,  5544,  5736,  5992,  6200,  6456,  6968,  7224,  7480,  7736,  7992,  8248,  8504,  8760,  9144,  9528,  9912,  9912,  10296, 10680, 11064, 11064, 11448, 11832, 12216, 12216, 12576, 12960, 12960, 13536, 13536, 14112, 14112, 14688, 14688, 15264, 15264, 15840, 15840, 16416, 16416, 16992, 16992, 17568, 17568, 18336, 18336, 18336, 19080, 19080, 19848, 19848, 19848, 20616, 20616, 20616, 21384, 21384, 22152, 22152, 22152, 22920, 22920, 22920, 23688, 23688, 24496, 24496, 24496, 25456, 25456, 25456, 25456, 26416, 26416, 26416, 27376, 27376, 27376, 28336, 28336, 28336, 28336, 29296, 29296, 29296, 29296, 30576, 30576, 30576, 30576, 31704, 31704},
   {280,   600,   904,   1224,  1544,  1800,  2152,  2472,  2728,  3112,  3368,  3624,  4008,  4264,  4584,  4968,  5160,  5544,  5736,  6200,  6456,  6712,  6968,  7224,  7736,  7992,  8248,  8504,  8760,  9144,  9528,  9912,  10296, 10296, 10680, 11064, 11448, 11832, 11832, 12216, 12576, 12960, 12960, 13536, 13536, 14112, 14688, 14688, 15264, 15264, 15840, 15840, 16416, 16416, 16992, 16992, 17568, 17568, 18336, 18336, 18336, 19080, 19080, 19848, 19848, 20616, 20616, 20616, 21384, 21384, 22152, 22152, 22152, 22920, 22920, 23688, 23688, 23688, 24496, 24496, 24496, 25456, 25456, 25456, 26416, 26416, 26416, 27376, 27376, 27376, 28336, 28336, 28336, 29296, 29296, 29296, 29296, 30576, 30576, 30576, 30576, 31704, 31704, 31704, 31704, 32856, 32856, 32856, 34008, 34008},
   {328,   632,   968,   1288,  1608,  1928,  2280,  2600,  2984,  3240,  3624,  3880,  4264,  4584,  4968,  5160,  5544,  5992,  6200,  6456,  6712,  7224,  7480,  7736,  7992,  8504,  8760,  9144,  9528,  9912,  9912,  10296, 10680, 11064, 11448, 11832, 12216, 12216, 12576, 12960, 13536, 13536, 14112, 14112, 14688, 14688, 15264, 15840, 15840, 16416, 16416, 16992, 16992, 17568, 17568, 18336, 18336, 19080, 19080, 19848, 19848, 19848, 20616, 20616, 21384, 21384, 22152, 22152, 22152, 22920, 22920, 23688, 23688, 24496, 24496, 24496, 25456, 25456, 25456, 26416, 26416, 26416, 27376, 27376, 27376, 28336, 28336, 28336, 29296, 29296, 29296, 30576, 30576, 30576, 30576, 31704, 31704, 31704, 31704, 32856, 32856, 32856, 34008, 34008, 34008, 34008, 35160, 35160, 35160, 35160},
   {336,   696,   1064,  1416,  1800,  2152,  2536,  2856,  3240,  3624,  4008,  4392,  4776,  5160,  5352,  5736,  6200,  6456,  6712,  7224,  7480,  7992,  8248,  8760,  9144,  9528,  9912,  10296, 10296, 10680, 11064, 11448, 11832, 12216, 12576, 12960, 13536, 13536, 14112, 14688, 14688, 15264, 15264, 15840, 16416, 16416, 16992, 17568, 17568, 18336, 18336, 19080, 19080, 19848, 19848, 20616, 20616, 20616, 21384, 21384, 22152, 22152, 22920, 22920, 23688, 23688, 24496, 24496, 24496, 25456, 25456, 26416, 26416, 26416, 27376, 27376, 27376, 28336, 28336, 29296, 29296, 29296, 30576, 30576, 30576, 30576, 31704, 31704, 31704, 32856, 32856, 32856, 34008, 34008, 34008, 35160, 35160, 35160, 35160, 36696, 36696, 36696, 36696, 37888, 37888, 37888, 39232, 39232, 39232, 39232},
   {376,   776,   1160,  1544,  1992,  2344,  2792,  3112,  3624,  4008,  4392,  4776,  5160,  5544,  5992,  6200,  6712,  7224,  7480,  7992,  8248,  8760,  9144,  9528,  9912,  10296, 10680, 11064, 11448, 11832, 12216, 12576, 12960, 13536, 14112, 14112, 14688, 15264, 15264, 15840, 16416, 16416, 16992, 17568, 17568, 18336, 18336, 19080, 19080, 19848, 19848, 20616, 21384, 21384, 22152, 22152, 22920, 22920, 23688, 23688, 24496, 24496, 24496, 25456, 25456, 26416, 26416, 27376, 27376, 27376, 28336, 28336, 29296, 29296, 29296, 30576, 30576, 30576, 31704, 31704, 31704, 32856, 32856, 32856, 34008, 34008, 34008, 35160, 35160, 35160, 36696, 36696, 36696, 37888, 37888, 37888, 37888, 39232, 39232, 39232, 40576, 40576, 40576, 40576, 42368, 42368, 42368, 42368, 43816, 43816},
   {408,   840,   1288,  1736,  2152,  2600,  2984,  3496,  3880,  4264,  4776,  5160,  5544,  5992,  6456,  6968,  7224,  7736,  8248,  8504,  9144,  9528,  9912,  10296, 10680, 11064, 11448, 12216, 12576, 12960, 13536, 13536, 14112, 14688, 15264, 15264, 15840, 16416, 16992, 16992, 17568, 18336, 18336, 19080, 19080, 19848, 20616, 20616, 21384, 21384, 22152, 22152, 22920, 22920, 23688, 24496, 24496, 25456, 25456, 25456, 26416, 26416, 27376, 27376, 28336, 28336, 29296, 29296, 29296, 30576, 30576, 30576, 31704, 31704, 32856, 32856, 32856, 34008, 34008, 34008, 35160, 35160, 35160, 36696, 36696, 36696, 37888, 37888, 37888, 39232, 39232, 39232, 40576, 40576, 40576, 40576, 42368, 42368, 42368, 43816, 43816, 43816, 43816, 45352, 45352, 45352, 46888, 46888, 46888, 46888},
   {440,   904,   1384,  1864,  2344,  2792,  3240,  3752,  4136,  4584,  5160,  5544,  5992,  6456,  6968,  7480,  7992,  8248,  8760,  9144,  9912,  10296, 10680, 11064, 11448, 12216, 12576, 12960, 13536, 14112, 14688, 14688, 15264, 15840, 16416, 16992, 16992, 17568, 18336, 18336, 19080, 19848, 19848, 20616, 20616, 21384, 22152, 22152, 22920, 22920, 23688, 24496, 24496, 25456, 25456, 26416, 26416, 27376, 27376, 28336, 28336, 29296, 29296, 29296, 30576, 30576, 31704, 31704, 31704, 32856, 32856, 34008, 34008, 34008, 35160, 35160, 35160, 36696, 36696, 36696, 37888, 37888, 39232, 39232, 39232, 40576, 40576, 40576, 42368, 42368, 42368, 42368, 43816, 43816, 43816, 45352, 45352, 45352, 46888, 46888, 46888, 46888, 48936, 48936, 48936, 48936, 48936, 51024, 51024, 51024},
   {488,   1000,  1480,  1992,  2472,  2984,  3496,  4008,  4584,  4968,  5544,  5992,  6456,  6968,  7480,  7992,  8504,  9144,  9528,  9912,  10680, 11064, 11448, 12216, 12576, 12960, 13536, 14112, 14688, 15264, 15840, 15840, 16416, 16992, 17568, 18336, 18336, 19080, 19848, 19848, 20616, 21384, 21384, 22152, 22920, 22920, 23688, 24496, 24496, 25456, 25456, 26416, 26416, 27376, 27376, 28336, 28336, 29296, 29296, 30576, 30576, 31704, 31704, 31704, 32856, 32856, 34008, 34008, 35160, 35160, 35160, 36696, 36696, 36696, 37888, 37888, 39232, 39232, 39232, 40576, 40576, 40576, 42368, 42368, 42368, 43816, 43816, 43816, 45352, 45352, 45352, 46888, 46888, 46888, 46888, 48936, 48936, 48936, 48936, 51024, 51024, 51024, 51024, 52752, 52752, 52752, 52752, 55056, 55056, 55056},
   {520,   1064,  1608,  2152,  2664,  3240,  3752,  4264,  4776,  5352,  5992,  6456,  6968,  7480,  7992,  8504,  9144,  9528,  10296, 10680, 11448, 11832, 12576, 12960, 13536, 14112, 14688, 15264, 15840, 16416, 16992, 16992, 17568, 18336, 19080, 19080, 19848, 20616, 21384, 21384, 22152, 22920, 22920, 23688, 24496, 24496, 25456, 25456, 26416, 27376, 27376, 28336, 28336, 29296, 29296, 30576, 30576, 31704, 31704, 32856, 32856, 34008, 34008, 34008, 35160, 35160, 36696, 36696, 36696, 37888, 37888, 39232, 39232, 40576, 40576, 40576, 42368, 42368, 42368, 43816, 43816, 43816, 45352, 45352, 45352, 46888, 46888, 46888, 48936, 48936, 48936, 48936, 51024, 51024, 51024, 51024, 52752, 52752, 52752, 55056, 55056, 55056, 55056, 57336, 57336, 57336, 57336, 59256, 59256, 59256},
   {552,   1128,  1736,  2280,  2856,  3496,  4008,  4584,  5160,  5736,  6200,  6968,  7480,  7992,  8504,  9144,  9912,  10296, 11064, 11448, 12216, 12576, 12960, 13536, 14112, 14688, 15264, 15840, 16416, 16992, 17568, 18336, 19080, 19848, 19848, 20616, 21384, 22152, 22152, 22920, 23688, 24496, 24496, 25456, 25456, 26416, 27376, 27376, 28336, 28336, 29296, 29296, 30576, 30576, 31704, 31704, 32856, 32856, 34008, 34008, 35160, 35160, 36696, 36696, 37888, 37888, 37888, 39232, 39232, 40576, 40576, 40576, 42368, 42368, 43816, 43816, 43816, 45352, 45352, 45352, 46888, 46888, 46888, 48936, 48936, 48936, 51024, 51024, 51024, 51024, 52752, 52752, 52752, 55056, 55056, 55056, 55056, 57336, 57336, 57336, 57336, 59256, 59256, 59256, 59256, 61664, 61664, 61664, 61664, 63776},
   {584,   1192,  1800,  2408,  2984,  3624,  4264,  4968,  5544,  5992,  6712,  7224,  7992,  8504,  9144,  9912,  10296, 11064, 11448, 12216, 12960, 13536, 14112, 14688, 15264, 15840, 16416, 16992, 17568, 18336, 19080, 19848, 19848, 20616, 21384, 22152, 22920, 22920, 23688, 24496, 25456, 25456, 26416, 26416, 27376, 28336, 28336, 29296, 29296, 30576, 31704, 31704, 32856, 32856, 34008, 34008, 35160, 35160, 36696, 36696, 36696, 37888, 37888, 39232, 39232, 40576, 40576, 42368, 42368, 42368, 43816, 43816, 45352, 45352, 45352, 46888, 46888, 46888, 48936, 48936, 48936, 51024, 51024, 51024, 52752, 52752, 52752, 52752, 55056, 55056, 55056, 57336, 57336, 57336, 57336, 59256, 59256, 59256, 61664, 61664, 61664, 61664, 63776, 63776, 63776, 63776, 66592, 66592, 66592, 66592},
   {616,   1256,  1864,  2536,  3112,  3752,  4392,  5160,  5736,  6200,  6968,  7480,  8248,  8760,  9528,  10296, 10680, 11448, 12216, 12576, 13536, 14112, 14688, 15264, 15840, 16416, 16992, 17568, 18336, 19080, 19848, 20616, 20616, 21384, 22152, 22920, 23688, 24496, 24496, 25456, 26416, 26416, 27376, 28336, 28336, 29296, 29296, 30576, 31704, 31704, 32856, 32856, 34008, 34008, 35160, 35160, 36696, 36696, 37888, 37888, 39232, 39232, 40576, 40576, 40576, 42368, 42368, 43816, 43816, 43816, 45352, 45352, 46888, 46888, 46888, 48936, 48936, 48936, 51024, 51024, 51024, 52752, 52752, 52752, 55056, 55056, 55056, 55056, 57336, 57336, 57336, 59256, 59256, 59256, 61664, 61664, 61664, 61664, 63776, 63776, 63776, 63776, 66592, 66592, 66592, 66592, 68808, 68808, 68808, 71112},
   {712,   1480,  2216,  2984,  3752,  4392,  5160,  5992,  6712,  7480,  8248,  8760,  9528,  10296, 11064, 11832, 12576, 13536, 14112, 14688, 15264, 16416, 16992, 17568, 18336, 19080, 19848, 20616, 21384, 22152, 22920, 23688, 24496, 25456, 25456, 26416, 27376, 28336, 29296, 29296, 30576, 30576, 31704, 32856, 32856, 34008, 35160, 35160, 36696, 36696, 37888, 37888, 39232, 40576, 40576, 40576, 42368, 42368, 43816, 43816, 45352, 45352, 46888, 46888, 48936, 48936, 48936, 51024, 51024, 52752, 52752, 52752, 55056, 55056, 55056, 55056, 57336, 57336, 57336, 59256, 59256, 59256, 61664, 61664, 61664, 63776, 63776, 63776, 66592, 66592, 66592, 68808, 68808, 68808, 71112, 71112, 71112, 73712, 73712, 75376, 75376, 75376, 75376, 75376, 75376, 75376, 75376, 75376, 75376, 75376}
 },
 {
   {32,    88,    152,   208,   256,   328,   376,   424,   488,   536,   600,   648,   712,   776,   808,   872,   936,   1000,  1032,  1096,  1160,  1224,  1256,  1320,  1384,  1416,  1480,  1544,  1608,  1672,  1736,  1800,  1800,  1864,  1928,  1992,  2088,  2088,  2152,  2216,  2280,  2344,  2408,  2472,  2536,  2536,  2600,  2664,  2728,  2792,  2856,  2856,  2984,  2984,  3112,  3112,  3240,  3240,  3240,  3368,  3368,  3496,  3496,  3624,  3624,  3624,  3752,  3752,  3880,  3880,  4008,  4008,  4008,  4136,  4136,  4136,  4264,  4264,  4392,  4392,  4584,  4584,  4584,  4776,  4776,  4776,  4776,  4968,  4968,  5160,  5160,  5160,  5160,  5160,  5352,  5352,  5544,  5544,  5544,  5544,  5544,  5736,  5736,  5736,  5992,  5992,  5992,  5992,  5992,  6200},
   {56,    144,   208,   256,   344,   424,   488,   568,   632,   712,   776,   872,   936,   1000,  1064,  1160,  1224,  1288,  1384,  1416,  1544,  1608,  1672,  1736,  1800,  1864,  1992,  2024,  2088,  2152,  2280,  2344,  2408,  2472,  2536,  2600,  2728,  2792,  2856,  2856,  2984,  3112,  3112,  3240,  3240,  3368,  3496,  3496,  3624,  3624,  3752,  3752,  3880,  4008,  4008,  4008,  4136,  4136,  4264,  4264,  4392,  4584,  4584,  4776,  4776,  4776,  4968,  4968,  5160,  5160,  5160,  5160,  5352,  5544,  5544,  5544,  5544,  5736,  5736,  5736,  5992,  5992,  5992,  6200,  6200,  6200,  6456,  6456,  6456,  6456,  6712,  6712,  6712,  6968,  6968,  6968,  6968,  7224,  7224,  7224,  7480,  7480,  7480,  7480,  7736,  7736,  7736,  7992,  7992,  7992},
   {72,    176,   256,   328,   424,   520,   616,   696,   776,   872,   968,   1064,  1160,  1256,  1320,  1416,  1544,  1608,  1672,  1800,  1864,  1992,  2088,  2152,  2216,  2344,  2408,  2536,  2600,  2664,  2792,  2856,  2984,  3112,  3112,  3240,  3368,  3368,  3496,  3624,  3624,  3752,  3880,  4008,  4008,  4136,  4264,  4264,  4392,  4584,  4584,  4584,  4776,  4776,  4968,  5160,  5160,  5160,  5352,  5352,  5544,  5544,  5736,  5736,  5736,  5992,  5992,  6200,  6200,  6200,  6456,  6456,  6456,  6712,  6712,  6712,  6968,  6968,  6968,  7224,  7224,  7224,  7480,  7480,  7736,  7736,  7736,  7992,  7992,  7992,  8248,  8248,  8248,  8504,  8504,  8504,  8760,  8760,  8760,  9144,  9144,  9144,  9144,  9144,  9528,  9528,  9528,  9528,  9912,  9912},
   {104,   208,   328,   440,   568,   680,   808,   904,   1032,  1160,  1256,  1384,  1480,  1608,  1736,  1864,  1992,  2088,  2216,  2344,  2472,  2536,  2664,  2792,  2856,  2984,  3112,  3240,  3368,  3496,  3624,  3752,  3880,  4008,  4136,  4264,  4392,  4392,  4584,  4776,  4776,  4968,  4968,  5160,  5352,  5352,  5544,  5544,  5736,  5736,  5992,  5992,  6200,  6200,  6456,  6456,  6712,  6712,  6968,  6968,  7224,  7224,  7224,  7480,  7480,  7736,  7736,  7992,  7992,  8248,  8248,  8504,  8504,  8760,  8760,  8760,  9144,  9144,  9144,  9528,  9528,  9528,  9528,  9912,  9912,  9912,  10296, 10296, 10296, 10680, 10680, 10680, 10680, 11064, 11064, 11064, 11448, 11448, 11448, 11448, 11832, 11832, 11832, 11832, 12576, 12576, 12576, 12576, 12960, 12960},
   {120,   256,   408,   552,   696,   840,   1000,  1128,  1288,  1416,  1544,  1736,  1864,  1992,  2152,  2280,  2408,  2600,  2728,  2856,  2984,  3112,  3240,  3496,  3624,  3752,  3880,  4008,  4136,  4264,  4392,  4584,  4776,  4968,  4968,  5160,  5352,  5544,  5544,  5736,  5992,  5992,  6200,  6200,  6456,  6456,  6712,  6968,  6968,  7224,  7224,  7480,  7480,  7736,  7992,  7992,  8248,  8248,  8504,  8504,  8760,  8760,  9144,  9144,  9144,  9528,  9528,  9912,  9912,  9912,  10296, 10296, 10296, 10680, 10680, 11064, 11064, 11064, 11448, 11448, 11448, 11832, 11832, 11832, 11832, 12576, 12576, 12576, 12960, 12960, 12960, 12960, 13536, 13536, 13536, 14112, 14112, 14112, 14112, 14688, 14688, 14688, 14688, 14688, 14688, 14688, 15264, 15264, 15264, 15840},
   {144,   328,   504,   680,   872,   1032,  1224,  1384,  1544,  1736,  1928,  2088,  2280,  2472,  2664,  2792,  2984,  3112,  3368,  3496,  3752,  3880,  4008,  4264,  4392,  4584,  4776,  4968,  5160,  5352,  5544,  5736,  5736,  5992,  6200,  6200,  6456,  6712,  6968,  6968,  7224,  7480,  7480,  7736,  7992,  7992,  8248,  8504,  8760,  8760,  9144,  9144,  9528,  9528,  9528,  9912,  9912,  10296, 10296, 10680, 10680, 11064, 11064, 11448, 11448, 11448, 11832, 11832,  11832,  12576,  12576,  12576,  12960, 12960, 13536, 13536, 13536, 14112, 14112, 14112, 14688, 14688, 14688, 14688, 14688, 14688, 15264, 15264, 15264, 15840, 15840, 15840, 16416, 16416, 16416, 16992,  16992,  17568,  17568,  17568,  17568,  18336,  18336,  18336,  18336,  19080,  19080,  19080,  19080,  19080},
   {176,   392,   600,   808,   1032,  1224,  1480,  1672,  1864,  2088,  2280,  2472,  2728,  2984,  3112,  3368,  3496,  3752,  4008,  4136,  4392,  4584,  4776,  4968,  5160,  5352,  5736,  5992,  5992,  6200,  6456,  6712,  6968,  6968,  7224,  7480,  7736,  7992,  8248,  8248,  8504,  8760,  9144,  9144,  9528,  9528,  9912,  9912,  10296, 10296, 10680, 10680, 11064, 11448, 11448, 11832, 11832, 11832, 12576, 12576, 12960, 12960, 12960, 13536, 13536, 14112, 14112, 14112,  14688,  14688,  14688,  14688,  15264, 15264, 15264, 15840, 15840, 16416, 16416, 16416, 16992, 16992, 17568, 17568, 17568, 18336, 18336, 18336, 18336, 19080, 19080, 19080, 19080, 19848, 19848, 19848,  20616, 20616, 20616, 20616, 21384, 21384, 21384, 21384, 24264, 24264, 24264, 22920, 22920, 22920},
   {224,   472,   712,   968,   1224,  1480,  1672,  1928,  2216,  2472,  2664,  2984,  3240,  3368,  3624,  3880,  4136,  4392,  4584,  4968,  5160,  5352,  5736,  5992,  6200,  6456,  6712,  6712,  6968,  7224,  7480,  7736,  7992,  8248,  8504,  8760,  9144,  9144,  9528,  9912,  9912,  10296, 10680, 10680, 11064, 11448, 11448, 11832, 11832, 12216, 12576, 12576, 12960, 12960, 13536, 13536, 14112, 14112, 14688, 14688, 14688, 14688, 15264, 15264, 15840, 15840, 16416, 16416, 16992, 16992, 17568, 17568, 17568, 18336, 18336, 18336, 19080, 19080, 19080, 19848, 19848, 19848, 20616, 20616, 20616, 21384, 21384, 21384, 24264, 24264, 24264, 22920, 22920, 22920, 22920, 23688, 23688, 23688, 24496, 24496, 24496, 25456, 25456, 25456, 25456, 25456, 25456, 25456, 27376, 27376},
   {256,   536,   808,   1096,  1384,  1672,  1928,  2216,  2536,  2792,  3112,  3368,  3624,  3880,  4264,  4584,  4776,  4968,  5352,  5544,  5992,  6200,  6456,  6712,  6968,  7224,  7480,  7736,  7992,  8504,  8760,  9144,  9144,  9528,  9912,  9912,  10296, 10680, 11064, 11064, 11448, 11832, 12216, 12216, 12576, 12960, 12960, 13536, 13536, 14112, 14112, 14688, 14688, 15264, 15264, 15264, 15840, 15840, 16416, 16992, 16992, 17568, 17568, 18336, 18336, 18336, 19080, 19080, 19080, 19848, 19848, 19848, 20616, 20616, 21384, 21384, 21384, 24264, 24264, 24264, 22920, 22920, 22920, 23688, 23688, 24496, 24496, 24496, 25456, 25456, 25456, 25456, 25456, 25456, 27376, 27376, 27376, 27376, 28336, 28336, 28336, 28336, 29296, 29296, 29296, 29296, 30576, 30576, 30576, 30576},
   {296,   616,   936,   1256,  1544,  1864,  2216,  2536,  2856,  3112,  3496,  3752,  4136,  4392,  4776,  5160,  5352,  5736,  5992,  6200,  6712,  6968,  7224,  7480,  7992,  8248,  8504,  8760,  9144,  9528,  9912,  10296, 10296, 10680, 11064, 11448, 11832, 11832, 12216, 12576, 12960, 13536, 13536, 14112, 14112, 14688, 14688, 15264, 15264, 15840, 16416, 16416, 16992, 16992, 17568, 17568, 18336, 18336, 18336, 19080, 19080, 19848, 19848, 20616, 20616, 20616, 21384, 21384, 24264, 24264, 24264, 22920, 22920, 23688, 23688, 23688, 24496, 24496, 25456, 25456, 25456, 25456, 25456, 27376, 27376, 27376, 27376, 28336, 28336, 28336, 28336, 29296, 29296, 29296, 30576, 30576, 30576, 30576, 31704, 31704, 31704, 32856, 32856, 32856, 32856, 34008, 34008, 34008, 34008, 35160},
   {328,   680,   1032,  1384,  1736,  2088,  2472,  2792,  3112,  3496,  3880,  4264,  4584,  4968,  5352,  5736,  5992,  6200,  6712,  6968,  7480,  7736,  7992,  8504,  8760,  9144,  9528,  9912,  10296, 10680, 11064, 11448, 11448, 11832, 12216, 12576, 12960, 13536, 13536, 14112, 14688, 14688, 15264, 15264, 15840, 16416, 16416, 16992, 16992, 17568, 18336, 18336, 18336, 19080, 19080, 19848, 19848, 20616, 20616, 21384, 21384, 24264, 24264, 22920, 22920, 22920, 23688, 23688, 24496, 24496, 25456, 25456, 25456, 25456, 25456, 27376, 27376, 27376, 28336, 28336, 28336, 29296, 29296, 29296, 29296, 30576, 30576, 30576, 31704, 31704, 31704, 32856, 32856, 32856, 34008, 34008, 34008, 34008, 35160, 35160, 35160, 36696, 36696, 36696, 36696, 36696, 37888, 37888, 37888, 37888},
   {376,   776,   1192,  1608,  2024,  2408,  2792,  3240,  3624,  4008,  4392,  4776,  5352,  5736,  5992,  6456,  6968,  7224,  7736,  7992,  8504,  8760,  9144,  9528,  9912,  10680, 11064, 11448, 11832, 12216, 12576, 12960, 13536, 13536, 14112, 14688, 14688, 15264, 15840, 16416, 16416, 16992, 17568, 17568, 18336, 18336, 19080, 19080, 19848, 19848, 20616, 21384, 21384, 22152, 22152, 22920, 22920, 23688, 23688, 24496, 24496, 25456, 25456, 25456, 25456, 27376, 27376, 27376, 28336, 28336, 28336, 29296, 29296, 29296, 30576, 30576, 31704, 31704, 31704, 32856, 32856, 32856, 34008, 34008, 34008, 35160, 35160, 35160, 36696, 36696, 36696, 36696, 37888, 37888, 37888, 37888, 39232, 39232, 39232, 39232, 40576, 40576, 40576, 42368, 42368, 42368, 42368, 43816, 43816, 43816},
   {440,   904,   1352,  1800,  2280,  2728,  3240,  3624,  4136,  4584,  4968,  5544,  5992,  6456,  6712,  7224,  7736,  8248,  8760,  9144,  9528,  9912,  10680, 11064, 11448, 11832, 12216, 12576, 12960, 13536, 14112, 14688, 15264, 15264, 15840, 16416, 16992, 17568, 17568, 18336, 19080, 19080, 19848, 19848, 20616, 21384, 21384, 22152, 22152, 22920, 23688, 23688, 24496, 24496, 25456, 25456, 25456, 25456, 27376, 27376, 28336, 28336, 28336, 29296, 29296, 30576, 30576, 30576, 31704, 31704, 32856, 32856, 32856, 34008, 34008, 35160, 35160, 35160, 36696, 36696, 36696, 37888, 37888, 37888, 37888, 39232, 39232, 39232, 40576, 40576, 40576, 42368, 42368, 42368, 42368, 43816, 43816, 43816, 45352, 45352, 45352, 46888, 46888, 46888, 46888, 48936, 48936, 48936, 48936, 51024},
   {488,   1000,  1544,  2024,  2536,  3112,  3624,  4136,  4584,  5160,  5736,  6200,  6712,  7224,  7736,  8248,  8760,  9144,  9912,  10296, 10680, 11448, 11832, 12216, 12960, 13536, 14112, 14688, 14688, 15264, 15840, 16416, 16992, 17568, 18336, 18336, 19080, 19848, 19848, 20616, 21384, 21384, 22152, 22920, 22920, 23688, 24496, 24496, 25456, 25456, 26416, 26416, 27376, 27376, 28336, 29296, 29296, 29296, 30576, 30576, 31704, 31704, 32856, 32856, 34008, 34008, 34008, 35160, 35160, 36696, 36696, 36696, 37888, 37888, 37888, 39232, 39232, 39232, 40576, 40576, 40576, 42368, 42368, 42368, 43816, 43816, 43816, 45352, 45352, 45352, 46888, 46888, 46888, 48936, 48936, 48936, 51024, 51024, 51024, 51024, 52752, 52752, 52752, 52752, 55056, 55056, 55056, 55056, 57336, 57336},
   {552,   1128,  1736,  2280,  2856,  3496,  4008,  4584,  5160,  5736,  6200,  6968,  7480,  7992,  8504,  9144,  9912,  10296, 11064, 11448, 12216, 12576, 12960, 13536, 14112, 14688, 15264, 15840, 16416, 16992, 17568, 18336, 19080, 19848, 19848, 20616, 21384, 22152, 22152, 22920, 23688, 24496, 24496, 25456, 25456, 26416, 27376, 27376, 28336, 28336, 29296, 29296, 30576, 30576, 31704, 31704, 32856, 32856, 34008, 34008, 35160, 35160, 36696, 36696, 36696, 37888, 37888, 39232, 39232, 39232, 40576, 40576, 40576, 42368, 42368, 43816, 43816, 43816, 45352, 45352, 45352, 46888, 46888, 48936, 48936, 48936, 51024, 51024, 51024, 51024, 52752, 52752, 52752, 55056, 55056, 55056, 57336, 57336, 57336, 57336, 59256, 59256, 59256, 59256, 61664, 61664, 61664, 61664, 63776, 63776},
   {600,   1224,  1800,  2472,  3112,  3624,  4264,  4968,  5544,  6200,  6712,  7224,  7992,  8504,  9144,  9912,  10296, 11064, 11832, 12216, 12960, 13536, 14112, 14688, 15264, 15840, 16416, 16992, 17568, 18336, 19080, 19848, 20616, 20616, 21384, 22152, 22920, 23688, 23688, 24496, 25456, 25456, 26416, 27376, 27376, 28336, 29296, 29296, 30576, 30576, 31704, 31704, 32856, 32856, 34008, 34008, 35160, 35160, 36696, 36696, 36696, 37888, 37888, 39232, 39232, 40576, 40576, 40576, 42368, 42368, 43816, 43816, 43816, 45352, 45352, 46888, 46888, 46888, 48936, 48936, 48936, 51024, 51024, 51024, 52752, 52752, 52752, 55056, 55056, 55056, 57336, 57336, 57336, 59256, 59256, 59256, 59256, 61664, 61664, 61664, 61664, 63776, 63776, 63776, 63776, 66592, 66592, 66592, 68808, 68808},
   {632,   1288,  1928,  2600,  3240,  3880,  4584,  5160,  5992,  6456,  7224,  7736,  8504,  9144,  9912,  10296, 11064, 11832, 12216, 12960, 13536, 14112, 14688, 15840, 16416, 16992, 17568, 18336, 19080, 19848, 19848, 20616, 21384, 22152, 22920, 23688, 24496, 24496, 25456, 26416, 26416, 27376, 28336, 28336, 29296, 30576, 30576, 31704, 31704, 32856, 32856, 34008, 34008, 35160, 35160, 36696, 36696, 37888, 37888, 39232, 39232, 39232, 40576, 40576, 42368, 42368, 43816, 43816, 43816, 45352, 45352, 46888, 46888, 48936, 48936, 48936, 51024, 51024, 51024, 52752, 52752, 52752, 55056, 55056, 55056, 57336, 57336, 57336, 59256, 59256, 59256, 61664, 61664, 61664, 61664, 63776, 63776, 63776, 63776, 66592, 66592, 66592, 68808, 68808, 68808, 68808, 71112, 71112, 71112, 71112},
   {696,   1416,  2152,  2856,  3624,  4392,  5160,  5736,  6456,  7224,  7992,  8760,  9528,  10296, 10680, 11448, 12216, 12960, 13536, 14688, 15264, 15840, 16416, 17568, 18336, 19080, 19848, 20616, 20616, 21384, 22152, 22920, 23688, 24496, 25456, 26416, 26416, 27376, 28336, 29296, 29296, 30576, 30576, 31704, 32856, 32856, 34008, 35160, 35160, 36696, 36696, 37888, 37888, 39232, 39232, 40576, 40576, 40576, 42368, 42368, 43816, 43816, 45352, 45352, 46888, 46888, 48936, 48936, 48936, 51024, 51024, 52752, 52752, 52752, 55056, 55056, 55056, 57336, 57336, 59256, 59256, 59256, 61664, 61664, 61664, 61664, 63776, 63776, 63776, 66592, 66592, 66592, 68808, 68808, 68808, 71112, 71112, 71112, 71112, 73712, 73712, 73712, 73712, 76208, 76208, 76208, 78704, 78704, 78704, 78704},
   {776,   1544,  2344,  3112,  4008,  4776,  5544,  6200,  7224,  7992,  8760,  9528,  10296, 11064, 11832, 12576, 13536, 14112, 15264, 15840, 16416, 17568, 18336, 19080, 19848, 20616, 21384, 22152, 22920, 23688, 24496, 25456, 26416, 27376, 27376, 28336, 29296, 30576, 30576, 31704, 32856, 32856, 34008, 35160, 35160, 36696, 37888, 37888, 39232, 39232, 40576, 40576, 42368, 42368, 43816, 43816, 45352, 45352, 46888, 46888, 48936, 48936, 48936, 51024, 51024, 52752, 52752, 55056, 55056, 55056, 57336, 57336, 59256, 59256, 59256, 61664, 61664, 61664, 63776, 63776, 63776, 66592, 66592, 66592, 68808, 68808, 68808, 71112, 71112, 71112, 73712, 73712, 73712, 76208, 76208, 76208, 76208, 78704, 78704, 78704, 81176, 81176, 81176, 81176, 84760, 84760, 84760, 84760, 87936, 87936},
   {840,   1736,  2600,  3496,  4264,  5160,  5992,  6968,  7736,  8504,  9528,  10296, 11064, 12216, 12960, 13536, 14688, 15264, 16416, 16992, 18336, 19080, 19848, 20616, 21384, 22152, 22920, 24496, 25456, 25456, 26416, 27376, 28336, 29296, 30576, 30576, 31704, 32856, 34008, 34008, 35160, 36696, 36696, 37888, 39232, 39232, 40576, 40576, 42368, 43816, 43816, 45352, 45352, 46888, 46888, 48936, 48936, 51024, 51024, 51024, 52752, 52752, 55056, 55056, 57336, 57336, 59256, 59256, 59256, 61664, 61664, 61664, 63776, 63776, 66592, 66592, 66592, 68808, 68808, 68808, 71112, 71112, 71112, 73712, 73712, 73712, 76208, 76208, 76208, 78704, 78704, 78704, 81176, 81176, 81176, 81176, 84760, 84760, 84760, 87936, 87936, 87936, 87936, 90816, 90816, 90816, 93800, 93800, 93800, 93800},
   {904,   1864,  2792,  3752,  4584,  5544,  6456,  7480,  8248,  9144,  10296, 11064, 12216, 12960, 14112, 14688, 15840, 16992, 17568, 18336, 19848, 20616, 21384, 22152, 22920, 24496, 25456, 26416, 27376, 28336, 29296, 29296, 30576, 31704, 32856, 34008, 34008, 35160, 36696, 36696, 37888, 39232, 40576, 40576, 42368, 42368, 43816, 45352, 45352, 46888, 46888, 48936, 48936, 51024, 51024, 52752, 52752, 55056, 55056, 57336, 57336, 59256, 59256, 59256, 61664, 61664, 63776, 63776, 63776, 66592, 66592, 68808, 68808, 68808, 71112, 71112, 71112, 73712, 73712, 73712, 76208, 76208, 78704, 78704, 78704, 81176, 81176, 81176, 84760, 84760, 84760, 84760, 87936, 87936, 87936, 90816, 90816, 90816, 93800, 93800, 93800, 93800, 97896, 97896, 97896, 97896, 97896, 101840, 101840, 101840},
   {1000,  1992,  2984,  4008,  4968,  5992,  6968,  7992,  9144,  9912,  11064, 12216, 12960, 14112, 15264, 15840, 16992, 18336, 19080, 19848, 21384, 22152, 22920, 24496, 25456, 26416, 27376, 28336, 29296, 30576, 31704, 31704, 32856, 34008, 35160, 36696, 36696, 37888, 39232, 40576, 40576, 42368, 43816, 43816, 45352, 46888, 46888, 48936, 48936, 51024, 51024, 52752, 52752, 55056, 55056, 57336, 57336, 59256, 59256, 61664, 61664, 63776, 63776, 63776, 66592, 66592, 68808, 68808, 71112, 71112, 71112, 73712, 73712, 73712, 76208, 76208, 78704, 78704, 78704, 81176, 81176, 81176, 84760, 84760, 84760, 87936, 87936, 87936, 90816, 90816, 90816, 93800, 93800, 93800, 93800, 97896, 97896, 97896, 97896, 101840, 101840, 101840, 101840, 105528, 105528, 105528, 105528, 110136, 110136, 110136},
   {1064,  2152,  3240,  4264,  5352,  6456,  7480,  8504,  9528,  10680, 11832, 12960, 14112, 15264, 16416, 16992, 18336, 19080, 20616, 21384, 22920, 23688, 24496, 25456, 27376, 28336, 29296, 30576, 31704, 32856, 34008, 34008, 35160, 36696, 37888, 39232, 40576, 40576, 42368, 43816, 43816, 45352, 46888, 46888, 48936, 48936, 51024, 51024, 52752, 55056, 55056, 57336, 57336, 59256, 59256, 61664, 61664, 63776, 63776, 66592, 66592, 68808, 68808, 68808, 71112, 71112, 73712, 73712, 73712, 76208, 76208, 78704, 78704, 81176, 81176, 81176, 84760, 84760, 84760, 87936, 87936, 87936, 90816, 90816, 90816, 93800, 93800, 93800, 97896, 97896, 97896, 97896, 101840, 101840, 101840, 101840, 105528, 105528, 105528, 110136, 110136, 110136, 110136, 115040, 115040, 115040, 115040, 119816, 119816, 119816},
   {1128,  2280,  3496,  4584,  5736,  6968,  7992,  9144,  10296, 11448, 12576, 13536, 14688, 15840, 16992, 18336, 19848, 20616, 22152, 22920, 24496, 25456, 26416, 27376, 28336, 29296, 30576, 31704, 32856, 34008, 35160, 36696, 37888, 39232, 40576, 40576, 42368, 43816, 45352, 45352, 46888, 48936, 48936, 51024, 51024, 52752, 55056, 55056, 57336, 57336, 59256, 59256, 61664, 61664, 63776, 63776, 66592, 66592, 68808, 68808, 71112, 71112, 73712, 73712, 76208, 76208, 76208, 78704, 78704, 81176, 81176, 81176, 84760, 84760, 87936, 87936, 87936, 90816, 90816, 90816, 93800, 93800, 93800, 97896, 97896, 97896, 101840,101840,101840,101840,105528, 105528, 105528, 110136, 110136, 110136, 110136, 115040, 115040, 115040, 115040, 119816, 119816, 119816, 119816, 124464, 124464, 124464, 124464, 128496},
   {1192,  2408,  3624,  4968,  5992,  7224,  8504,  9912,  11064, 12216, 13536, 14688, 15840, 16992, 18336, 19848, 20616, 22152, 22920, 24496, 25456, 26416, 28336, 29296, 30576, 31704, 32856, 34008, 35160, 36696, 37888, 39232, 40576, 42368, 42368, 43816, 45352, 46888, 46888, 48936, 51024, 51024, 52752, 52752, 55056, 57336, 57336, 59256, 59256, 61664, 61664, 63776, 63776, 66592, 66592, 68808, 71112, 71112, 73712, 73712, 73712, 76208, 76208, 78704, 78704, 81176, 81176, 84760, 84760, 84760, 87936, 87936, 90816, 90816, 90816, 93800, 93800, 93800, 97896, 97896, 97896, 101840, 101840, 101840, 105528, 105528, 105528, 105528, 110136, 110136, 110136, 115040, 115040, 115040, 115040, 119816, 119816, 119816, 124464, 124464, 124464, 124464, 128496, 128496, 128496, 128496, 133208, 133208, 133208, 133208},
   {1256,  2536,  3752,  5160,  6200,  7480,  8760,  10296, 11448, 12576, 14112, 15264, 16416, 17568, 19080, 20616, 21384, 22920, 24496, 25456, 26416, 28336, 29296, 30576, 31704, 32856, 34008, 35160, 36696, 37888, 39232, 40576, 42368, 43816, 43816, 45352, 46888, 48936, 48936, 51024, 52752, 52752, 55056, 55056, 57336, 59256, 59256, 61664, 61664, 63776, 63776, 66592, 66592, 68808, 71112, 71112, 73712, 73712, 76208, 76208, 78704, 78704, 81176, 81176, 81176, 84760, 84760, 87936, 87936, 87936, 90816, 90816, 93800, 93800, 93800, 97896, 97896, 97896, 101840, 101840, 101840, 105528, 105528, 105528, 110136, 110136, 110136, 110136, 115040,115040, 115040, 119816, 119816, 119816, 124464, 124464, 124464, 124464, 128496, 128496, 128496, 128496, 133208, 133208, 133208, 133208, 137792, 137792, 137792, 142248},
   {1480,  2984,  4392,  5992,  7480,  8760,  10296, 11832, 13536, 14688, 16416, 17568, 19080, 20616, 22152, 23688, 25456, 26416, 28336, 29296, 30576, 32856, 34008, 35160, 36696, 37888, 40576, 40576, 42368, 43816, 45352, 46888, 48936, 51024, 52752, 52752, 55056, 55056, 57336, 59256, 59256, 61664, 63776, 63776, 66592, 68808, 68808, 71112, 73712, 75376, 75376, 75376, 75376, 75376, 75376, 81176, 84760, 84760, 87936, 87936, 90816, 90816, 93800, 93800, 97896, 97896, 97896, 101840, 101840, 105528, 105528, 105528, 110136, 110136, 110136, 110136, 115040, 115040, 115040, 119816, 119816, 119816, 124464, 124464, 124464, 128496, 128496, 128496, 133208, 133208, 133208, 137792, 137792, 137792, 142248, 142248, 142248, 146856, 146856,149776, 149776, 149776, 149776, 149776, 149776, 149776, 149776, 149776, 149776, 149776}
 }
};
RgSchUlIMcsTbl rgUlIMcsTbl = {
   {2, 0}, {2, 1}, {2, 2}, {2, 3}, {2, 4}, {2, 5},
   {2, 6}, {2, 7}, {2, 8}, {2, 9}, {2, 10},
   {4, 10}, {4, 11}, {4, 12}, {4, 13}, {4, 14},
   {4, 15}, {4, 16}, {4, 17}, {4, 18}, {4, 19},
   {6, 19}, {6, 20}, {6, 21}, {6, 22}, {6, 23},
   {6, 24}, {6, 25}, {6, 26}
};
RgSchUeCatTbl rgUeCatTbl = {
   {5160,  10296,  250368,  FALSE, 10296,  1},
   {25456, 51024,  1237248, FALSE, 51024,  2},
   {51024, 75376,  1237248, FALSE, 102048, 2},
   {51024, 75376,  1827072, FALSE, 150752, 2},
   {75376, 149776, 3667200, TRUE,  299552, 4}
};

/* [ccpu00138532]-ADD-The below table stores the min HARQ RTT time
   in Downlink for TDD and FDD. Indices 0 to 6 map to tdd UL DL config 0-6. 
   Index 7 map to FDD */    
U8 rgSchCmnHarqRtt[8] = {4,7,10,9,12,15,6,8};
/* Number of CFI Switchover Index is equals to 7 TDD Indexes + 1 FDD index */
U8 rgSchCfiSwitchOvrWinLen[] = {7, 4, 2, 3, 2, 1, 6, 8};

/* EffTbl is calculated for single layer and two layers.
  * CqiToTbs is calculated for single layer and two layers */
RgSchCmnTbSzEff rgSchCmnNorCfi1Eff[RGSCH_MAX_NUM_LYR_PERCW], rgSchCmnNorCfi2Eff[RGSCH_MAX_NUM_LYR_PERCW];
RgSchCmnTbSzEff rgSchCmnNorCfi3Eff[RGSCH_MAX_NUM_LYR_PERCW], rgSchCmnNorCfi4Eff[RGSCH_MAX_NUM_LYR_PERCW];
/* New variable to store UL effiency values for normal and extended CP*/
RgSchCmnTbSzEff rgSchCmnNorUlEff[1],rgSchCmnExtUlEff[1];
RgSchCmnCqiToTbs rgSchCmnNorCfi1CqiToTbs[RGSCH_MAX_NUM_LYR_PERCW], rgSchCmnNorCfi2CqiToTbs[RGSCH_MAX_NUM_LYR_PERCW];
RgSchCmnCqiToTbs rgSchCmnNorCfi3CqiToTbs[RGSCH_MAX_NUM_LYR_PERCW], rgSchCmnNorCfi4CqiToTbs[RGSCH_MAX_NUM_LYR_PERCW];
RgSchCmnCqiToTbs *rgSchCmnCqiToTbs[RGSCH_MAX_NUM_LYR_PERCW][RG_SCH_CMN_MAX_CP][RG_SCH_CMN_MAX_CFI];
RgSchCmnTbSzEff rgSchCmnExtCfi1Eff[RGSCH_MAX_NUM_LYR_PERCW], rgSchCmnExtCfi2Eff[RGSCH_MAX_NUM_LYR_PERCW];
RgSchCmnTbSzEff rgSchCmnExtCfi3Eff[RGSCH_MAX_NUM_LYR_PERCW], rgSchCmnExtCfi4Eff[RGSCH_MAX_NUM_LYR_PERCW];
RgSchCmnCqiToTbs rgSchCmnExtCfi1CqiToTbs[RGSCH_MAX_NUM_LYR_PERCW], rgSchCmnExtCfi2CqiToTbs[RGSCH_MAX_NUM_LYR_PERCW];
RgSchCmnCqiToTbs rgSchCmnExtCfi3CqiToTbs[RGSCH_MAX_NUM_LYR_PERCW], rgSchCmnExtCfi4CqiToTbs[RGSCH_MAX_NUM_LYR_PERCW];
/* Include CRS REs while calculating Efficiency */
RgSchCmnTbSzEff *rgSchCmnEffTbl[RGSCH_MAX_NUM_LYR_PERCW][RG_SCH_CMN_MAX_CP][RG_SCH_CMN_MAX_ANT_CONF][RG_SCH_CMN_MAX_CFI];
RgSchCmnTbSzEff *rgSchCmnUlEffTbl[RG_SCH_CMN_MAX_CP];
#ifdef LTE_TDD
RgSchRaPrmblToRaFrmTbl rgRaPrmblToRaFrmTbl = {1, 2, 2, 3, 1};
#else
/* Added matrix 'rgRaPrmblToRaFrmTbl'for computation of RA sub-frames from RA preamble */
RgSchRaPrmblToRaFrmTbl rgRaPrmblToRaFrmTbl = {1, 2, 2, 3};
#endif

EXTERN  RgUlSchdInits        rgSchUlSchdInits;
EXTERN  RgDlSchdInits        rgSchDlSchdInits;
EXTERN  RgDlfsSchdInits      rgSchDlfsSchdInits;

/* RACHO : start */
PRIVATE S16 rgSCHCmnUeIdleExdThrsld ARGS((
RgSchCellCb     *cell,
RgSchUeCb       *ue
));
PRIVATE RgSchUeCb* rgSCHCmnGetHoUe ARGS((
RgSchCellCb           *cell,
RgSchRaReqInfo        *raReq
));
PRIVATE Void rgSCHCmnDelDedPreamble ARGS((
RgSchCellCb           *cell,
U8                    preambleId
));
PRIVATE RgSchUeCb* rgSCHCmnGetPoUe ARGS((
RgSchCellCb           *cell,
RgSchRaReqInfo        *raReq
));
PRIVATE Void rgSCHCmnDelRachInfo ARGS((
RgSchCellCb  *cell,
RgSchUeCb    *ue
));
PRIVATE S16 rgSCHCmnUlRbAllocForPoHoUe ARGS((
RgSchCellCb           *cell,
RgSchUlSf             *sf,
RgSchUeCb             *ue,
U8                    maxRb
));
PRIVATE Void rgSCHCmnHdlHoPo ARGS((
RgSchCellCb           *cell,
CmLListCp             *raRspLst,
RgSchRaReqInfo        *raReq
));
PRIVATE Void rgSCHCmnAllocPoHoGrnt ARGS((
RgSchCellCb           *cell,
CmLListCp             *raRspLst,
RgSchUeCb             *ue,
RgSchRaReqInfo        *raReq
));
PRIVATE Void rgSCHCmnFillPdcchOdr2Sf ARGS((
RgSchUeCb   *ue,
RgSchPdcch  *pdcc,
U8          rapId,
U8          prachMskIdx
));
PRIVATE Void rgSCHCmnDlAdd2PdcchOdrQ ARGS((
RgSchCellCb                *cell,
RgSchUeCb                  *ue
));
PRIVATE Void rgSCHCmnDlRmvFrmPdcchOdrQ ARGS((
RgSchCellCb                *cell,
RgSchUeCb                  *ue
));
PRIVATE Void rgSCHCmnUpdNxtPrchMskIdx ARGS((
RgSchCellCb  *cell
));
PRIVATE Void rgSCHCmnUpdRachParam ARGS((
RgSchCellCb  *cell
));
PRIVATE S16 rgSCHCmnAllocPOParam ARGS((
RgSchCellCb  *cell,
RgSchDlSf    *dlSf,
RgSchUeCb    *ue,
RgSchPdcch   **pdcch,
U8           *rapId,
U8           *prachMskIdx
));
PRIVATE Void rgSCHCmnGenPdcchOrder ARGS((
RgSchCellCb  *cell,
RgSchDlSf    *dlSf
));
PRIVATE Void rgSCHCmnCfgRachDedPrm ARGS((
RgSchCellCb   *cell
));
/* RACHO : end */

PRIVATE Void rgSCHCmnHdlUlInactUes ARGS((
RgSchCellCb  *cell
));
PRIVATE Void rgSCHCmnHdlDlInactUes ARGS((
RgSchCellCb  *cell
));
PRIVATE Void rgSCHCmnUlInit ARGS((Void
));
PRIVATE Void rgSCHCmnDlInit ARGS((Void
));
PRIVATE Void rgSCHCmnInitUlRbAllocInfo ARGS((
RgSchCmnUlRbAllocInfo  *allocInfo
));
PRIVATE Void rgSCHCmnInitDlRbAllocInfo ARGS((
RgSchCmnDlRbAllocInfo  *allocInfo
));
PRIVATE Void rgSCHCmnUpdUlCompEffBsr ARGS((
RgSchUeCb *ue
));
#if RG_UNUSED
PRIVATE Void rgSCHCmnUlSetAllUnSched  ARGS((
RgSchCmnUlRbAllocInfo *allocInfo
));
PRIVATE Void rgSCHCmnUlUpdSf ARGS((
         RgSchCellCb           *cell,
         RgSchCmnUlRbAllocInfo *allocInfo,
         RgSchUlSf     *sf
         ));
PRIVATE Void rgSCHCmnUlHndlAllocRetx ARGS((
         RgSchCellCb           *cell,
         RgSchCmnUlRbAllocInfo *allocInfo,
         RgSchUlSf     *sf,
         RgSchUlAlloc  *alloc
         ));
#endif
PRIVATE Void rgSCHCmnDlAllocFnlz ARGS((
RgSchCellCb           *cell,
RgSchCmnDlRbAllocInfo *allocInfo
));
PRIVATE Void rgSCHCmnGrpPwrCntrlPucch ARGS((
RgSchCellCb  *cell,
RgSchDlSf    *dlSf
));
PRIVATE Void rgSCHCmnGrpPwrCntrlPusch ARGS((
RgSchCellCb  *cell,
RgSchUlSf    *ulSf
));
PRIVATE Void rgSCHCmnDelUeFrmRefreshQ ARGS((
RgSchCellCb     *cell,
RgSchUeCb       *ue
));
PRIVATE S16 rgSCHCmnTmrExpiry ARGS((
PTR cb,               /* Pointer to timer control block */
S16 tmrEvnt           /* Timer Event */
));
PRIVATE S16 rgSCHCmnTmrProc ARGS((
RgSchCellCb *cell
));
PRIVATE Void rgSCHCmnAddUeToRefreshQ ARGS((
RgSchCellCb     *cell,
RgSchUeCb       *ue,
U32             wait
));
PRIVATE Void rgSCHCmnDlCcchRetx ARGS((
RgSchCellCb             *cell,
RgSchCmnDlRbAllocInfo   *allocInfo
));
PRIVATE Void rgSCHCmnUpdUeMimoInfo ARGS((
RgrUeCfg     *ueCfg,
RgSchCmnDlUe *ueDl,
RgSchCellCb  *cell,
RgSchCmnCell *cellSchd
));
PRIVATE Void rgSCHCmnUpdUeUlCqiInfo ARGS((
RgSchCellCb   *cell,
RgSchUeCb     *ue,
RgSchCmnUlUe  *ueUl,
RgSchCmnUe    *ueSchCmn,
RgSchCmnCell  *cellSchd,
Bool          isEcp 
));
#ifdef RGR_V1
PRIVATE Void rgSCHCmnDlCcchSduRetx ARGS((
RgSchCellCb             *cell,
RgSchCmnDlRbAllocInfo   *allocInfo
));
PRIVATE Void rgSCHCmnDlCcchSduTx ARGS((
RgSchCellCb             *cell,
RgSchCmnDlRbAllocInfo   *allocInfo
));
PRIVATE S16 rgSCHCmnCcchSduAlloc ARGS((
RgSchCellCb                *cell,
RgSchUeCb                  *ueCb,
RgSchCmnDlRbAllocInfo      *allocInfo
));
PRIVATE S16 rgSCHCmnCcchSduDedAlloc ARGS((
RgSchCellCb                *cell,
RgSchUeCb                  *ueCb
));
PRIVATE S16 rgSCHCmnNonDlfsCcchSduRbAlloc ARGS((
RgSchCellCb           *cell,
RgSchUeCb             *ueCb,
RgSchDlSf             *dlSf
));
#endif
PRIVATE Void rgSCHCmnInitVars ARGS((
         RgSchCellCb *cell
         ));

/*ccpu00117180 - DEL - Moved rgSCHCmnUpdVars to .x as its access is now PUBLIC */
PRIVATE Void rgSCHCmnUlRbAllocForLst ARGS((
         RgSchCellCb           *cell,
         RgSchUlSf             *sf,
         U32                   count,
         CmLListCp             *reqLst,
         CmLListCp             *schdLst,
         CmLListCp             *nonSchdLst,
         Bool                  isNewTx
         ));
PRIVATE S16 rgSCHCmnUlRbAllocForUe ARGS((
         RgSchCellCb           *cell,
         RgSchUlSf             *sf,
         RgSchUeCb             *ue,
         U8                    maxRb,
         RgSchUlHole           *hole
         ));
PRIVATE Void rgSCHCmnMsg3GrntReq ARGS((
         RgSchCellCb     *cell,
         CmLteRnti       rnti,
         Bool            preamGrpA,
         RgSchUlHqProcCb *hqProc,
         RgSchUlAlloc    **ulAllocRef,
         U8              *hqProcIdRef
         ));
PRIVATE Void rgSCHCmnUlNonadapRetx ARGS((
         RgSchCmnUlCell  *cellUl,
         RgSchUlAlloc    *alloc,
         U8               idx
         ));

PRIVATE Void rgSCHCmnDlCmnChAlloc ARGS((
RgSchCellCb             *cell,
RgSchCmnDlRbAllocInfo   *allocInfo
));
PRIVATE Void rgSCHCmnDlCcchTx ARGS((
RgSchCellCb             *cell,
RgSchCmnDlRbAllocInfo   *allocInfo
));
PRIVATE Void rgSCHCmnDlBcchPcch ARGS((
RgSchCellCb             *cell,
RgSchCmnDlRbAllocInfo   *allocInfo,
RgInfSfAlloc            *subfrmAlloc
));
PRIVATE Bool rgSCHCmnChkInWin ARGS((
CmLteTimingInfo   frm,
CmLteTimingInfo   start,
CmLteTimingInfo   end
));
PRIVATE Bool rgSCHCmnChkPastWin ARGS((
CmLteTimingInfo   frm,
CmLteTimingInfo   end
));
PRIVATE Void rgSCHCmnClcAlloc ARGS((
RgSchCellCb             *cell,
RgSchDlSf               *sf,
RgSchClcDlLcCb          *lch,
U16                     rnti,
RgSchCmnDlRbAllocInfo   *allocInfo
));
#ifndef LTEMAC_SPS
PRIVATE Void rgSCHCmnClcRbAlloc ARGS((
RgSchCellCb             *cell,
U32                     bo,
U8                      cqi,
U8                      *rb,
U32                     *tbs,
U8                      *mcs,
RgSchDlSf               *sf 
));
#endif

PRIVATE S16 rgSCHCmnMsg4Alloc ARGS((
RgSchCellCb                *cell,
RgSchRaCb                  *raCb,
RgSchCmnDlRbAllocInfo      *allocInfo
));
PRIVATE S16 rgSCHCmnMsg4DedAlloc ARGS((
RgSchCellCb                *cell,
RgSchRaCb                  *raCb
));
PRIVATE Void rgSCHCmnDlRaRsp ARGS((
RgSchCellCb                *cell,
RgSchCmnDlRbAllocInfo      *allocInfo
));
PRIVATE S16 rgSCHCmnRaRspAlloc ARGS((
RgSchCellCb             *cell,
RgSchDlSf               *subFrm,
U16                     rntiIdx,
U16                     rarnti,
U8                      noRaRnti,
RgSchCmnDlRbAllocInfo   *allocInfo
));
PRIVATE Void rgSCHCmnUlUeDelAllocs ARGS((
RgSchCellCb  *cell,
RgSchUeCb   *ue
));
PRIVATE Void rgSCHCmnDlSetUeAllocLmt ARGS((
RgSchCellCb   *cell,
RgSchCmnDlUe  *ueDl
));
PRIVATE S16 rgSCHCmnDlRgrCellCfg ARGS((
RgSchCellCb    *cell,
RgrCellCfg     *cfg,
RgSchErrInfo   *err
));
PRIVATE Void rgSCHCmnUlAdapRetx ARGS((
RgSchUlAlloc    *alloc,
RgSchUlHqProcCb *proc
));
PRIVATE Void rgSCHCmnUlUpdAllocRetx ARGS((
RgSchUlSf      *reTxSf,
RgSchUlAlloc   *alloc
));
PRIVATE Void rgSCHCmnUlSfReTxAllocs ARGS((
RgSchCellCb *cell,
RgSchUlSf   *sf
));
/* Fix: syed Adaptive Msg3 Retx crash. */
PRIVATE Void rgSCHCmnUlSfRlsRetxProcs ARGS((
RgSchCellCb *cell,
RgSchUlSf   *sf
));

#ifdef TFU_UPGRADE
PRIVATE Void rgSCHCmnDlHdlTxModeRecfg ARGS
((
RgSchUeCb    *ue,
RgrUeRecfg   *ueRecfg,
U8 numTxPorts
));
#else
PRIVATE Void rgSCHCmnDlHdlTxModeRecfg ARGS
((
RgSchUeCb    *ue,
RgrUeRecfg   *ueRecfg
));
#endif


/*
 * DL RB allocation specific functions
 */

PRIVATE Void rgSCHCmnDlRbAlloc ARGS((
RgSchCellCb           *cell,
RgSchCmnDlRbAllocInfo *allocInfo
));
PRIVATE Void rgSCHCmnNonDlfsRbAlloc ARGS((
RgSchCellCb           *cell,
RgSchCmnDlRbAllocInfo *allocInfo
));
PRIVATE S16 rgSCHCmnNonDlfsCmnRbAlloc ARGS((
RgSchCellCb           *cell,
RgSchDlRbAlloc        *cmnAllocInfo));

#ifndef LTE_TDD
PRIVATE Void rgSCHCmnNonDlfsPbchRbAllocAdj ARGS((
RgSchCellCb           *cell,
RgSchDlRbAlloc        *cmnAllocInfo,
U8                    pbchSsRsSym,
Bool                  isBcchPcch
));
/* Added function to adjust TBSize*/
PRIVATE Void rgSCHCmnNonDlfsPbchTbSizeAdj ARGS((
RgSchDlRbAlloc        *allocInfo,
U8                    numOvrlapgPbchRb,
U8                    pbchSsRsSym,
U8                    idx,
U32                   bytesReq
));

/* Added function to find num of overlapping PBCH rb*/
PRIVATE Void rgSCHCmnFindNumPbchOvrlapRbs ARGS((
RgSchCellCb           *cell,
RgSchDlSf             *dlSf,
RgSchDlRbAlloc        *allocInfo,
U8                    *numOvrlapgPbchRb
));

PRIVATE U8 rgSCHCmnFindNumAddtlRbsAvl ARGS((
RgSchCellCb           *cell,
RgSchDlSf             *dlSf,
RgSchDlRbAlloc        *allocInfo
));
#ifdef DEBUGP
PRIVATE Void rgSCHCmnFindCodeRate ARGS((
RgSchCellCb           *cell,
RgSchDlSf             *dlSf,
RgSchDlRbAlloc        *allocInfo,
U8                    idx
));
#endif
#endif
PRIVATE Void rgSCHCmnNonDlfsMsg4Alloc ARGS((
RgSchCellCb           *cell,
RgSchCmnMsg4RbAlloc   *msg4AllocInfo,
U8                    isRetx
));
PRIVATE S16 rgSCHCmnNonDlfsMsg4RbAlloc ARGS((
RgSchCellCb           *cell,
RgSchRaCb             *raCb,
RgSchDlSf             *dlSf
));
PRIVATE Void rgSCHCmnNonDlfsDedRbAlloc ARGS((
RgSchCellCb        *cell,
RgSchCmnUeRbAlloc  *allocInfo,
CmLListCp          *ueLst,
CmLListCp          *schdUeLst,
CmLListCp          *nonSchdUeLst
));
PRIVATE S16 rgSCHCmnNonDlfsUeRbAlloc ARGS((
RgSchCellCb           *cell,
RgSchUeCb             *ue,
RgSchDlSf             *dlSf,
U8                    *isDlBwAvail
));
#ifndef LTEMAC_SPS
PRIVATE U32 rgSCHCmnCalcRiv ARGS(( U8 bw,
         U8           rbStart,
         U8           numRb));
#endif

#ifdef LTE_TDD
PRIVATE Void rgSCHCmnUpdHqAndDai ARGS((
RgSchDlHqProcCb   *hqP,
RgSchDlSf         *subFrm,
RgSchDlHqTbCb     *tbCb,
U8                tbAllocIdx
));
PRIVATE S16 rgSCHCmnUlCalcAvailBw ARGS((
RgSchCellCb *cell,
RgrCellCfg  *cellCfg,
U8          cfi,
U8          *rbStartRef,
U8          *bwAvailRef
));
PRIVATE S16 rgSCHCmnDlKdashUlAscInit ARGS((
RgSchCellCb *cell
));
PRIVATE S16 rgSCHCmnDlANFdbkInit ARGS((
RgSchCellCb *cell
));
PRIVATE S16 rgSCHCmnDlNpValInit ARGS((
RgSchCellCb *cell
));
PRIVATE S16 rgSCHCmnDlCreateRachPrmLst ARGS((
RgSchCellCb *cell
));
PRIVATE S16 rgSCHCmnDlCpyRachInfo ARGS((
RgSchCellCb        *cell,
RgSchTddRachRspLst rachRspLst[][RGSCH_NUM_SUB_FRAMES],
U8                 raArrSz
));
PRIVATE S16 rgSCHCmnDlRachInfoInit ARGS((
RgSchCellCb *cell
));
PRIVATE S16 rgSCHCmnDlPhichOffsetInit ARGS((
RgSchCellCb *cell
));
#endif
#ifdef TFU_UPGRADE
PRIVATE Void rgSCHCmnFindUlCqiUlTxAnt ARGS
((
 RgSchCellCb          *cell,
 RgSchUeCb            *ue,
 U8                          wideCqi
 ));
 PRIVATE RgSchCmnRank rgSCHCmnComputeRank ARGS
((
 RgrTxMode    txMode,
 U32          *pmiBitMap,
 U8           numTxPorts
 ));

 PRIVATE RgSchCmnRank rgSCHCmnComp2TxMode3 ARGS
((
 U32 *pmiBitMap
 ));

  PRIVATE RgSchCmnRank rgSCHCmnComp4TxMode3 ARGS
((
 U32 *pmiBitMap
 ));

  PRIVATE RgSchCmnRank rgSCHCmnComp2TxMode4 ARGS
((
 U32 *pmiBitMap
 ));

  PRIVATE RgSchCmnRank rgSCHCmnComp4TxMode4 ARGS
((
 U32 *pmiBitMap
 ));

 PRIVATE U8 rgSCHCmnCalcWcqiFrmSnr ARGS
((
 RgSchCellCb        *cell,
 TfuSrsRpt        *srsRpt
 ));
#endif

/* comcodsepa : start */

/**
 * @brief This function computes efficiency and stores in a table.
 *
 * @details
 *
 *     Function: rgSCHCmnCompEff
 *     Purpose:  this function computes the efficiency as number of
 *               bytes per 1024 symbols. The CFI table is also filled
 *               with the same information such that comparison is valid
 *
 *     Invoked by: Scheduler
 *
 *  @param[in]  U8            noPdcchSym
 *  @param[in]  U8            cpType
 *  @param[in]  U8            txAntIdx
 *  @param[in]  RgSchCmnTbSzEff* effTbl
 *  @return  Void
 *
 **/
#ifdef ANSI
PRIVATE Void rgSCHCmnCompEff
(
U8                    noPdcchSym,
U8                    cpType,
U8                    txAntIdx,
RgSchCmnTbSzEff       *effTbl
)
#else
PRIVATE Void rgSCHCmnCompEff(noPdcchSym, cpType, txAntIdx, effTbl)
U8                    noPdcchSym;
U8                    cpType;
U8                    txAntIdx;
RgSchCmnTbSzEff       *effTbl;
#endif
{
   U8               noResPerRb;
   U8               noSymPerRb;
   U8               resOfCrs; /* Effective REs occupied by CRS */
   U8               i, j;

   TRC2(rgSCHCmnCompEff);

   switch (cpType)
   {
      case RG_SCH_CMN_NOR_CP:
         noSymPerRb = 14;
         break;
      case RG_SCH_CMN_EXT_CP:
         noSymPerRb = 12;
         break;
      default:
         /* Generate a log error. This case should never be executed */
         RETVOID;
   }

   /* Depending on the Tx Antenna Index, deduct the
    * Resource elements for the CRS */
   switch (txAntIdx)
   {
      case 0:
         resOfCrs = RG_SCH_CMN_EFF_CRS_ONE_ANT_PORT;
         break;
      case 1:
         resOfCrs = RG_SCH_CMN_EFF_CRS_TWO_ANT_PORT;
         break;
      case 2:
         resOfCrs = RG_SCH_CMN_EFF_CRS_FOUR_ANT_PORT;
         break;
      default:
         /* Generate a log error. This case should never be executed */
         RETVOID;
   }
   noResPerRb = ((noSymPerRb - noPdcchSym) * RB_SCH_CMN_NUM_SCS_PER_RB) - resOfCrs;
   for (i = 0; i < RG_SCH_CMN_NUM_TBS; i++)
   {
      (*effTbl)[i] = 0;
      for (j = 0; j < RG_SCH_CMN_NUM_RBS; j++)
      {
         /* This line computes the coding efficiency per 1024 REs */
         (*effTbl)[i] += (rgTbSzTbl[0][i][j] * 1024) / (noResPerRb * (j+1));
      }
      (*effTbl)[i] /= RG_SCH_CMN_NUM_RBS;
   }
   RETVOID;
}
/**
 * @brief This function computes efficiency and stores in a table.
 *
 * @details
 *
 *     Function: rgSCHCmnCompUlEff
 *     Purpose:  this function computes the efficiency as number of
 *               bytes per 1024 symbols. The CFI table is also filled
 *               with the same information such that comparison is valid
 *
 *     Invoked by: Scheduler
 *
 *  @param[in]  U8            noUlRsSym
 *  @param[in]  U8            cpType
 *  @param[in]  U8            txAntIdx
 *  @param[in]  RgSchCmnTbSzEff* effTbl
 *  @return  Void
 *
 **/
#ifdef ANSI
PRIVATE Void rgSCHCmnCompUlEff
(
U8                    noUlRsSym,
U8                    cpType,
RgSchCmnTbSzEff       *effTbl
)
#else
PRIVATE Void rgSCHCmnCompUlEff(noUlRsSym, cpType, effTbl)
U8                    noUlRsSym;
U8                    cpType;
RgSchCmnTbSzEff       *effTbl;
#endif
{
   U8               noResPerRb;
   U8               noSymPerRb;
   U8               i, j;

   TRC2(rgSCHCmnCompUlEff);

   switch (cpType)
   {
      case RG_SCH_CMN_NOR_CP:
         noSymPerRb = 14;
         break;
      case RG_SCH_CMN_EXT_CP:
         noSymPerRb = 12;
         break;
      default:
         /* Generate a log error. This case should never be executed */
         RETVOID;
   }

   noResPerRb = ((noSymPerRb - noUlRsSym) * RB_SCH_CMN_NUM_SCS_PER_RB);
   for (i = 0; i < RG_SCH_CMN_NUM_TBS; i++)
   {
      (*effTbl)[i] = 0;
      for (j = 0; j < RG_SCH_CMN_NUM_RBS; j++)
      {
         /* This line computes the coding efficiency per 1024 REs */
         (*effTbl)[i] += (rgTbSzTbl[0][i][j] * 1024) / (noResPerRb * (j+1));
      }
      (*effTbl)[i] /= RG_SCH_CMN_NUM_RBS;
   }
   RETVOID;
}

/**
 * @brief This function computes efficiency for 2 layers and stores in a table.
 *
 * @details
 *
 *     Function: rgSCHCmn2LyrCompEff
 *     Purpose:  this function computes the efficiency as number of
 *               bytes per 1024 symbols. The CFI table is also filled
 *               with the same information such that comparison is valid
 *
 *     Invoked by: Scheduler
 *
 *  @param[in]  U8            noPdcchSym
 *  @param[in]  U8            cpType
 *  @param[in]  U8            txAntIdx
 *  @param[in]  RgSchCmnTbSzEff* effTbl2Lyr
 *  @return  Void
 *
 **/
#ifdef ANSI
PRIVATE Void rgSCHCmn2LyrCompEff
(
U8                    noPdcchSym,
U8                    cpType,
U8                    txAntIdx,
RgSchCmnTbSzEff       *effTbl2Lyr
)
#else
PRIVATE Void rgSCHCmn2LyrCompEff(noPdcchSym, cpType, txAntIdx, effTbl2Lyr)
U8                    noPdcchSym;
U8                    cpType;
U8                    txAntIdx;
RgSchCmnTbSzEff       *effTbl2Lyr;
#endif
{
   U8               noResPerRb;
   U8               noSymPerRb;
   U8               resOfCrs; /* Effective REs occupied by CRS */
   U8               i, j;

   TRC2(rgSCHCmn2LyrCompEff);

   switch (cpType)
   {
      case RG_SCH_CMN_NOR_CP:
         noSymPerRb = 14;
         break;
      case RG_SCH_CMN_EXT_CP:
         noSymPerRb = 12;
         break;
      default:
         /* Generate a log error. This case should never be executed */
         RETVOID;
   }

   /* Depending on the Tx Antenna Index, deduct the
    * Resource elements for the CRS */
   switch (txAntIdx)
   {
      case 0:
         resOfCrs = RG_SCH_CMN_EFF_CRS_ONE_ANT_PORT;
         break;
      case 1:
         resOfCrs = RG_SCH_CMN_EFF_CRS_TWO_ANT_PORT;
         break;
      case 2:
         resOfCrs = RG_SCH_CMN_EFF_CRS_FOUR_ANT_PORT;
         break;
      default:
         /* Generate a log error. This case should never be executed */
         RETVOID;
   }

   noResPerRb = ((noSymPerRb - noPdcchSym) * RB_SCH_CMN_NUM_SCS_PER_RB) - resOfCrs;
   for (i = 0; i < RG_SCH_CMN_NUM_TBS; i++)
   {
      (*effTbl2Lyr)[i] = 0;
      for (j = 0; j < RG_SCH_CMN_NUM_RBS; j++)
      {
         /* This line computes the coding efficiency per 1024 REs */
         (*effTbl2Lyr)[i] += (rgTbSzTbl[1][i][j] * 1024) / (noResPerRb * (j+1));
      }
      (*effTbl2Lyr)[i] /= RG_SCH_CMN_NUM_RBS;
   }
   RETVOID;
}


/**
 * @brief This function initializes the rgSchCmnDciFrmtSizes table.
 *
 * @details
 *
 *     Function: rgSCHCmnGetDciFrmtSizes
 *     Purpose:  This function determines the sizes of all
 *               the available DCI Formats. The order of
 *               bits addition for each format is inaccordance
 *               with the specs.
 *     Invoked by: rgSCHCmnRgrCellCfg
 *
 *  @return  Void
 *
 **/
#ifdef ANSI
PRIVATE Void rgSCHCmnGetDciFrmtSizes
(
RgSchCellCb *cell
)
#else
PRIVATE Void rgSCHCmnGetDciFrmtSizes(cell)
RgSchCellCb *cell;
#endif
{

   TRC2(rgSCHCmnGetDciFrmtSizes);

   /* DCI Format 0 size determination */
   rgSchCmnDciFrmtSizes[0] = 1 +
                             1 +
                             rgSCHUtlLog32bitNbase2((cell->bwCfg.ulTotalBw * \
                             (cell->bwCfg.ulTotalBw + 1))/2) +
                             5 +
                             1 +
                             2 +
                             3 +
#ifdef LTE_TDD
                             2 +
                             2 +
#endif
                             1;
   /* DCI Format 1 size determination */
   rgSchCmnDciFrmtSizes[1] = 1 +
   RGSCH_CEIL(cell->bwCfg.dlTotalBw, cell->rbgSize) +
                             5 +
#ifndef LTE_TDD
                             3 +
#else
                             4 + 2 + /* HqProc Id and DAI */
#endif
                             1 +
                             2 +
                             2;

   /* DCI Format 1A size determination */
   rgSchCmnDciFrmtSizes[2] = 1 + /* Flag for format0/format1a differentiation */
               1 + /* Localized/distributed VRB assignment flag */
               5 + /* For mcs */
#ifndef LTE_TDD
               3 + /* Harq process Id */
#else
               4 + /* Harq process Id */
               2 + /* UL Index or DAI */
#endif
               1 + /* New Data Indicator */
               2 + /* For RV */
               2 + /* For tpc */
               1 + rgSCHUtlLog32bitNbase2((cell->bwCfg.dlTotalBw * \
                   (cell->bwCfg.dlTotalBw + 1))/2);
               /* Resource block assignment ceil[log2(bw(bw+1)/2)] : \
                  Since VRB is local */

   /* DCI Format 1B size determination */
   rgSchCmnDciFrmtSizes[3] = 1 +
                             rgSCHUtlLog32bitNbase2((cell->bwCfg.dlTotalBw * \
                             (cell->bwCfg.dlTotalBw + 1))/2) +
                             5 +
                             3 +
#ifdef LTE_TDD
                             1 + /* HqP */
                             2 + /* Dai */
#endif
                             1 +
                             2 +
                             2 +
                             ((cell->numTxAntPorts == 4)? 4:2) +
                             1;

   /* DCI Format 1C size determination */
   /* Approximation: NDLVrbGap1 ~= Nprb for DL */
   rgSchCmnDciFrmtSizes[4] = (cell->bwCfg.dlTotalBw < 50)? 0:1 +
                             (cell->bwCfg.dlTotalBw < 50)?
                             (rgSCHUtlLog32bitNbase2((cell->bwCfg.dlTotalBw/2 * \
                                (cell->bwCfg.dlTotalBw/2 + 1))/2)) :
                             (rgSCHUtlLog32bitNbase2((cell->bwCfg.dlTotalBw/4 * \
                                (cell->bwCfg.dlTotalBw/4 + 1))/2)) +
                             5;

   /* DCI Format 1D size determination */
   rgSchCmnDciFrmtSizes[5] = 1 +
                             rgSCHUtlLog32bitNbase2((cell->bwCfg.dlTotalBw * \
                             (cell->bwCfg.dlTotalBw + 1))/2) +
                             5 +
                             3 +
#ifdef LTE_TDD
                             1 + 2 +
#endif
                             1 +
                             2 +
                             2 +
                             ((cell->numTxAntPorts == 4)? 4:2) +
                             1;

   /* DCI Format 2 size determination */
   rgSchCmnDciFrmtSizes[6] = ((cell->bwCfg.dlTotalBw < 10)?0:1) +
                             RGSCH_CEIL(cell->bwCfg.dlTotalBw, cell->rbgSize) +
                             2 +
#ifdef LTE_TDD
                             2 + 1 +
#endif
                             3 +
                             1 +
                             (5 + 1 + 2)*2 +
                             ((cell->numTxAntPorts == 4)? 6:3);

   /* DCI Format 2A size determination */
   rgSchCmnDciFrmtSizes[7] = ((cell->bwCfg.dlTotalBw < 10)?0:1) +
                             RGSCH_CEIL(cell->bwCfg.dlTotalBw, cell->rbgSize) +
                             2 +
#ifdef LTE_TDD
                             2 + 1 +
#endif
                             3 +
                             1 +
                             (5 + 1 + 2)*2 +
                             ((cell->numTxAntPorts == 4)? 2:0);

   /* DCI Format 3 size determination */
   rgSchCmnDciFrmtSizes[8] = rgSchCmnDciFrmtSizes[0];

   /* DCI Format 3A size determination */
   rgSchCmnDciFrmtSizes[9] = rgSchCmnDciFrmtSizes[0];

   RETVOID;
}


/**
 * @brief This function initializes the cmnCell->dciAggrLvl table.
 *
 * @details
 *
 *     Function: rgSCHCmnGetCqiDciFrmt2AggrLvl
 *     Purpose:  This function determines the Aggregation level
 *               for each CQI level against each DCI format.
 *     Invoked by: rgSCHCmnRgrCellCfg
 *
 *  @return  Void
 *
 **/
#ifdef ANSI
PRIVATE Void rgSCHCmnGetCqiDciFrmt2AggrLvl
(
RgSchCellCb *cell
)
#else
PRIVATE Void rgSCHCmnGetCqiDciFrmt2AggrLvl(cell)
RgSchCellCb *cell;
#endif
{
   RgSchCmnCell  *cellSch = RG_SCH_CMN_GET_CELL(cell);
   U8            i;
   U8            j;

   TRC2(rgSCHCmnGetCqiDciFrmt2AggrLvl);

   for (i = 0; i < RG_SCH_CMN_MAX_CQI; i++)
   {
      for (j = 0; j < 10; j++)
      {
         U32 pdcchBits; /* Actual number of phy bits needed for a given DCI Format
               * for a given CQI Level */
         pdcchBits = (rgSchCmnDciFrmtSizes[j] * 1024)/rgSchCmnCqiPdcchEff[i];
         if (pdcchBits < 72)
         {
             if(j == 0)
             {
               cellSch->dciAggrLvl[i][j] = CM_LTE_AGGR_LVL1;
             }
             else
             {
               cellSch->dciAggrLvl[i][j] = CM_LTE_AGGR_LVL2;
             }
             continue;
         }
         if (pdcchBits < 144)
         {
             cellSch->dciAggrLvl[i][j] = CM_LTE_AGGR_LVL2;
             continue;
         }
         if (pdcchBits < 288)
         {
             cellSch->dciAggrLvl[i][j] = CM_LTE_AGGR_LVL4;
             continue;
         }
         cellSch->dciAggrLvl[i][j] = CM_LTE_AGGR_LVL8;
      }
   }
   RETVOID;
}

/**
 * @brief This function initializes all the data for the scheduler.
 *
 * @details
 *
 *     Function: rgSCHCmnDlInit
 *     Purpose:  This function initializes the following information:
 *               1. Efficiency table
 *               2. CQI to table index - It is one row for upto 3 RBs
 *                  and another row for greater than 3 RBs
 *                  currently extended prefix is compiled out.
 *     Invoked by: MAC intialization code..may be ActvInit
 *
 *  @return  Void
 *
 **/
#ifdef ANSI
PRIVATE Void rgSCHCmnDlInit
(
)
#else
PRIVATE Void rgSCHCmnDlInit()
#endif
{
   U8                   i;
   S16                  j;
   S16                  k;
   U8                   idx;
   RgSchCmnTbSzEff      *effTbl;
   RgSchCmnCqiToTbs     *tbsTbl;

   TRC2(rgSCHCmnDlInit);

   /* 0 corresponds to Single layer case, 1 corresponds to 2 layers case*/
   /* Init Efficiency table for normal cyclic prefix */
   /*Initialize Efficiency table for Layer Index 0 */
   /*Initialize Efficiency table for Tx Antenna Port Index 0 */
   /*Initialize Efficiency table for each of the CFI indices. The
    * 4th Dimension of the rgSCHCmnEffTbl table refers to the CFI Index*/
   rgSchCmnEffTbl[0][RG_SCH_CMN_NOR_CP][0][0] = &rgSchCmnNorCfi1Eff[0];
   rgSchCmnEffTbl[0][RG_SCH_CMN_NOR_CP][0][1] = &rgSchCmnNorCfi2Eff[0];
   rgSchCmnEffTbl[0][RG_SCH_CMN_NOR_CP][0][2] = &rgSchCmnNorCfi3Eff[0];
   rgSchCmnEffTbl[0][RG_SCH_CMN_NOR_CP][0][3] = &rgSchCmnNorCfi4Eff[0];
   /*Initialize Efficency table for Tx Antenna Port Index 1 */
   rgSchCmnEffTbl[0][RG_SCH_CMN_NOR_CP][1][0] = &rgSchCmnNorCfi1Eff[0];
   rgSchCmnEffTbl[0][RG_SCH_CMN_NOR_CP][1][1] = &rgSchCmnNorCfi2Eff[0];
   rgSchCmnEffTbl[0][RG_SCH_CMN_NOR_CP][1][2] = &rgSchCmnNorCfi3Eff[0];
   rgSchCmnEffTbl[0][RG_SCH_CMN_NOR_CP][1][3] = &rgSchCmnNorCfi4Eff[0];
   /*Initialize Efficency table for Tx Antenna Port Index 2 */
   rgSchCmnEffTbl[0][RG_SCH_CMN_NOR_CP][2][0] = &rgSchCmnNorCfi1Eff[0];
   rgSchCmnEffTbl[0][RG_SCH_CMN_NOR_CP][2][1] = &rgSchCmnNorCfi2Eff[0];
   rgSchCmnEffTbl[0][RG_SCH_CMN_NOR_CP][2][2] = &rgSchCmnNorCfi3Eff[0];
   rgSchCmnEffTbl[0][RG_SCH_CMN_NOR_CP][2][3] = &rgSchCmnNorCfi4Eff[0];

   /*Initialize CQI to TBS table for Layer Index 0 for Normal CP */
   rgSchCmnCqiToTbs[0][RG_SCH_CMN_NOR_CP][0] = &rgSchCmnNorCfi1CqiToTbs[0];
   rgSchCmnCqiToTbs[0][RG_SCH_CMN_NOR_CP][1] = &rgSchCmnNorCfi2CqiToTbs[0];
   rgSchCmnCqiToTbs[0][RG_SCH_CMN_NOR_CP][2] = &rgSchCmnNorCfi3CqiToTbs[0];
   rgSchCmnCqiToTbs[0][RG_SCH_CMN_NOR_CP][3] = &rgSchCmnNorCfi4CqiToTbs[0];

   /*Intialize Efficency table for Layer Index 1 */
   /*Initialize Efficiency table for Tx Antenna Port Index 0 */
   /*Initialize Efficiency table for each of the CFI indices. The
    * 4th Dimension of the rgSCHCmnEffTbl table refers to the CFI Index*/
   rgSchCmnEffTbl[1][RG_SCH_CMN_NOR_CP][0][0] = &rgSchCmnNorCfi1Eff[1];
   rgSchCmnEffTbl[1][RG_SCH_CMN_NOR_CP][0][1] = &rgSchCmnNorCfi2Eff[1];
   rgSchCmnEffTbl[1][RG_SCH_CMN_NOR_CP][0][2] = &rgSchCmnNorCfi3Eff[1];
   rgSchCmnEffTbl[1][RG_SCH_CMN_NOR_CP][0][3] = &rgSchCmnNorCfi4Eff[1];
   /*Initialize Efficiency table for Tx Antenna Port Index 1 */
   rgSchCmnEffTbl[1][RG_SCH_CMN_NOR_CP][1][0] = &rgSchCmnNorCfi1Eff[1];
   rgSchCmnEffTbl[1][RG_SCH_CMN_NOR_CP][1][1] = &rgSchCmnNorCfi2Eff[1];
   rgSchCmnEffTbl[1][RG_SCH_CMN_NOR_CP][1][2] = &rgSchCmnNorCfi3Eff[1];
   rgSchCmnEffTbl[1][RG_SCH_CMN_NOR_CP][1][3] = &rgSchCmnNorCfi4Eff[1];
   /*Initialize Efficiency table for Tx Antenna Port Index 2 */
   rgSchCmnEffTbl[1][RG_SCH_CMN_NOR_CP][2][0] = &rgSchCmnNorCfi1Eff[1];
   rgSchCmnEffTbl[1][RG_SCH_CMN_NOR_CP][2][1] = &rgSchCmnNorCfi2Eff[1];
   rgSchCmnEffTbl[1][RG_SCH_CMN_NOR_CP][2][2] = &rgSchCmnNorCfi3Eff[1];
   rgSchCmnEffTbl[1][RG_SCH_CMN_NOR_CP][2][3] = &rgSchCmnNorCfi4Eff[1];

   /*Initialize CQI to TBS table for Layer Index 1 for Normal CP */
   rgSchCmnCqiToTbs[1][RG_SCH_CMN_NOR_CP][0] = &rgSchCmnNorCfi1CqiToTbs[1];
   rgSchCmnCqiToTbs[1][RG_SCH_CMN_NOR_CP][1] = &rgSchCmnNorCfi2CqiToTbs[1];
   rgSchCmnCqiToTbs[1][RG_SCH_CMN_NOR_CP][2] = &rgSchCmnNorCfi3CqiToTbs[1];
   rgSchCmnCqiToTbs[1][RG_SCH_CMN_NOR_CP][3] = &rgSchCmnNorCfi4CqiToTbs[1];

   for (idx = 0; idx < RG_SCH_CMN_MAX_ANT_CONF; idx++)
   {
      for (i = 0; i < RG_SCH_CMN_MAX_CFI; i++)
      {
         /* EfficiencyTbl calculation incase of 2 layers for normal CP  */
         rgSCHCmnCompEff((U8)(i + 1), RG_SCH_CMN_NOR_CP, idx,\
               rgSchCmnEffTbl[0][RG_SCH_CMN_NOR_CP][idx][i]);
         rgSCHCmn2LyrCompEff((U8)(i + 1), RG_SCH_CMN_NOR_CP, idx, \
               rgSchCmnEffTbl[1][RG_SCH_CMN_NOR_CP][idx][i]);
      }
   }

   for (idx = 0; idx < RG_SCH_CMN_MAX_ANT_CONF; idx++)
   {
      for (i = 0; i < RG_SCH_CMN_MAX_CFI; i++)
      {
         effTbl = rgSchCmnEffTbl[0][RG_SCH_CMN_NOR_CP][idx][i];
         tbsTbl = rgSchCmnCqiToTbs[0][RG_SCH_CMN_NOR_CP][i];
         for (j = RG_SCH_CMN_NUM_TBS - 1, k = RG_SCH_CMN_MAX_CQI - 1;
               (j >= 0) && (k > 0); --j)
         {
            /* ADD CQI to MCS mapping correction
            * single dimensional array is replaced by 2 dimensions for different CFI*/
            if ((*effTbl)[j] <= rgSchCmnCqiPdschEff[i][k])
            {
               (*tbsTbl)[k--] = (U8)j;
            }
         }
         for (; k > 0; --k)
         {
            (*tbsTbl)[k] = 0;
         }
         /* effTbl,tbsTbl calculation incase of 2 layers for normal CP */
         effTbl = rgSchCmnEffTbl[1][RG_SCH_CMN_NOR_CP][idx][i];
         tbsTbl = rgSchCmnCqiToTbs[1][RG_SCH_CMN_NOR_CP][i];
         for (j = RG_SCH_CMN_NUM_TBS - 1, k = RG_SCH_CMN_MAX_CQI - 1;
               (j >= 0) && (k > 0); --j)
         {
            /* ADD CQI to MCS mapping correction
            * single dimensional array is replaced by 2 dimensions for different CFI*/
            if ((*effTbl)[j] <= rgSchCmn2LyrCqiPdschEff[i][k])
            {
               (*tbsTbl)[k--] = (U8)j;
            }
         }
         for (; k > 0; --k)
         {
            (*tbsTbl)[k] = 0;
         }
      }
   }

   /* Efficiency Table for Extended CP */
   /*Initialize Efficiency table for Layer Index 0 */
   /*Initialize Efficiency table for Tx Antenna Port Index 0 */
   /*Initialize Efficiency table for each of the CFI indices. The
    * 4th Dimension of the rgSCHCmnEffTbl table refers to the CFI Index*/
   rgSchCmnEffTbl[0][RG_SCH_CMN_EXT_CP][0][0] = &rgSchCmnExtCfi1Eff[0];
   rgSchCmnEffTbl[0][RG_SCH_CMN_EXT_CP][0][1] = &rgSchCmnExtCfi2Eff[0];
   rgSchCmnEffTbl[0][RG_SCH_CMN_EXT_CP][0][2] = &rgSchCmnExtCfi3Eff[0];
   rgSchCmnEffTbl[0][RG_SCH_CMN_EXT_CP][0][3] = &rgSchCmnExtCfi4Eff[0];
   /*Initialize Efficency table for Tx Antenna Port Index 1 */
   rgSchCmnEffTbl[0][RG_SCH_CMN_EXT_CP][1][0] = &rgSchCmnExtCfi1Eff[0];
   rgSchCmnEffTbl[0][RG_SCH_CMN_EXT_CP][1][1] = &rgSchCmnExtCfi2Eff[0];
   rgSchCmnEffTbl[0][RG_SCH_CMN_EXT_CP][1][2] = &rgSchCmnExtCfi3Eff[0];
   rgSchCmnEffTbl[0][RG_SCH_CMN_EXT_CP][1][3] = &rgSchCmnExtCfi4Eff[0];
   /*Initialize Efficency table for Tx Antenna Port Index 2 */
   rgSchCmnEffTbl[0][RG_SCH_CMN_EXT_CP][2][0] = &rgSchCmnExtCfi1Eff[0];
   rgSchCmnEffTbl[0][RG_SCH_CMN_EXT_CP][2][1] = &rgSchCmnExtCfi2Eff[0];
   rgSchCmnEffTbl[0][RG_SCH_CMN_EXT_CP][2][2] = &rgSchCmnExtCfi3Eff[0];
   rgSchCmnEffTbl[0][RG_SCH_CMN_EXT_CP][2][3] = &rgSchCmnExtCfi4Eff[0];

   /*Initialize CQI to TBS table for Layer Index 0 for Extended CP */
   rgSchCmnCqiToTbs[0][RG_SCH_CMN_EXT_CP][0] = &rgSchCmnExtCfi1CqiToTbs[0];
   rgSchCmnCqiToTbs[0][RG_SCH_CMN_EXT_CP][1] = &rgSchCmnExtCfi2CqiToTbs[0];
   rgSchCmnCqiToTbs[0][RG_SCH_CMN_EXT_CP][2] = &rgSchCmnExtCfi3CqiToTbs[0];
   rgSchCmnCqiToTbs[0][RG_SCH_CMN_EXT_CP][3] = &rgSchCmnExtCfi4CqiToTbs[0];

   /*Initialize Efficiency table for Layer Index 1 */
   /*Initialize Efficiency table for each of the CFI indices. The
    * 4th Dimension of the rgSCHCmnEffTbl table refers to the CFI Index*/
   /*Initialize Efficency table for Tx Antenna Port Index 0 */
   rgSchCmnEffTbl[1][RG_SCH_CMN_EXT_CP][0][0] = &rgSchCmnExtCfi1Eff[1];
   rgSchCmnEffTbl[1][RG_SCH_CMN_EXT_CP][0][1] = &rgSchCmnExtCfi2Eff[1];
   rgSchCmnEffTbl[1][RG_SCH_CMN_EXT_CP][0][2] = &rgSchCmnExtCfi3Eff[1];
   rgSchCmnEffTbl[1][RG_SCH_CMN_EXT_CP][0][3] = &rgSchCmnExtCfi4Eff[1];
   /*Initialize Efficency table for Tx Antenna Port Index 1 */
   rgSchCmnEffTbl[1][RG_SCH_CMN_EXT_CP][1][0] = &rgSchCmnExtCfi1Eff[1];
   rgSchCmnEffTbl[1][RG_SCH_CMN_EXT_CP][1][1] = &rgSchCmnExtCfi2Eff[1];
   rgSchCmnEffTbl[1][RG_SCH_CMN_EXT_CP][1][2] = &rgSchCmnExtCfi3Eff[1];
   rgSchCmnEffTbl[1][RG_SCH_CMN_EXT_CP][1][3] = &rgSchCmnExtCfi4Eff[1];
   /*Initialize Efficency table for Tx Antenna Port Index 2 */
   rgSchCmnEffTbl[1][RG_SCH_CMN_EXT_CP][2][0] = &rgSchCmnExtCfi1Eff[1];
   rgSchCmnEffTbl[1][RG_SCH_CMN_EXT_CP][2][1] = &rgSchCmnExtCfi2Eff[1];
   rgSchCmnEffTbl[1][RG_SCH_CMN_EXT_CP][2][2] = &rgSchCmnExtCfi3Eff[1];
   rgSchCmnEffTbl[1][RG_SCH_CMN_EXT_CP][2][3] = &rgSchCmnExtCfi4Eff[1];

   /*Initialize CQI to TBS table for Layer Index 1 for Extended CP */
   rgSchCmnCqiToTbs[1][RG_SCH_CMN_EXT_CP][0] = &rgSchCmnExtCfi1CqiToTbs[1];
   rgSchCmnCqiToTbs[1][RG_SCH_CMN_EXT_CP][1] = &rgSchCmnExtCfi2CqiToTbs[1];
   rgSchCmnCqiToTbs[1][RG_SCH_CMN_EXT_CP][2] = &rgSchCmnExtCfi3CqiToTbs[1];
   rgSchCmnCqiToTbs[1][RG_SCH_CMN_EXT_CP][3] = &rgSchCmnExtCfi4CqiToTbs[1];
   /* Activate this code when extended cp is supported */
   for (idx = 0; idx < RG_SCH_CMN_MAX_ANT_CONF; idx++)
   {
      for (i = 0; i < RG_SCH_CMN_MAX_CFI; i++)
      {
         /* EfficiencyTbl calculation incase of 2 layers for extendedl CP  */
         rgSCHCmnCompEff( (U8)(i + 1 ), (U8)RG_SCH_CMN_EXT_CP, idx,\
               rgSchCmnEffTbl[0][RG_SCH_CMN_EXT_CP][idx][i]);
         rgSCHCmn2LyrCompEff((U8)(i + 1), (U8) RG_SCH_CMN_EXT_CP,idx, \
               rgSchCmnEffTbl[1][RG_SCH_CMN_EXT_CP][idx][i]);
      }
   }

   for (idx = 0; idx < RG_SCH_CMN_MAX_ANT_CONF; idx++)
   {
      for (i = 0; i < RG_SCH_CMN_MAX_CFI; i++)
      {
         effTbl = rgSchCmnEffTbl[0][RG_SCH_CMN_EXT_CP][idx][i];
         tbsTbl = rgSchCmnCqiToTbs[0][RG_SCH_CMN_EXT_CP][i];
         for (j = RG_SCH_CMN_NUM_TBS - 1, k = RG_SCH_CMN_MAX_CQI - 1;
               (j >= 0) && (k > 0); --j)
         {
            /* ADD CQI to MCS mapping correction
            * single dimensional array is replaced by 2 dimensions for different CFI*/
            if ((*effTbl)[j] <= rgSchCmnCqiPdschEff[i][k])
            {
               (*tbsTbl)[k--] = (U8)j;
            }
         }
         for (; k > 0; --k)
         {
            (*tbsTbl)[k] = 0;
         }
         /* effTbl,tbsTbl calculation incase of 2 layers for extended CP */
         effTbl = rgSchCmnEffTbl[1][RG_SCH_CMN_EXT_CP][idx][i];
         tbsTbl = rgSchCmnCqiToTbs[1][RG_SCH_CMN_EXT_CP][i];
         for (j = RG_SCH_CMN_NUM_TBS - 1, k = RG_SCH_CMN_MAX_CQI - 1;
               (j >= 0) && (k > 0); --j)
         {
           /* ADD CQI to MCS mapping correction
            * single dimensional array is replaced by 2 dimensions for different CFI*/
            if ((*effTbl)[j] <= rgSchCmn2LyrCqiPdschEff[i][k])
            {
               (*tbsTbl)[k--] = (U8)j;
            }
         }
         for (; k > 0; --k)
         {
            (*tbsTbl)[k] = 0;
         }
      }
   }
   RETVOID;
}

/**
 * @brief This function initializes all the data for the scheduler.
 *
 * @details
 *
 *     Function: rgSCHCmnUlInit
 *     Purpose:  This function initializes the following information:
 *               1. Efficiency table
 *               2. CQI to table index - It is one row for upto 3 RBs
 *                  and another row for greater than 3 RBs
 *                  currently extended prefix is compiled out.
 *     Invoked by: MAC intialization code..may be ActvInit
 *
 *  @return  Void
 *
 **/
#ifdef ANSI
PRIVATE Void rgSCHCmnUlInit
(
)
#else
PRIVATE Void rgSCHCmnUlInit()
#endif
{
   U8              *mapTbl = &rgSchCmnUlCqiToTbsTbl[RG_SCH_CMN_NOR_CP][0];
   RgSchCmnTbSzEff    *effTbl    = &rgSchCmnNorUlEff[0];
   CONSTANT RgSchCmnUlCqiInfo *cqiTbl = &rgSchCmnUlCqiTbl[0];
   S16              i;
   S16              j;
   TRC2(rgSCHCmnUlInit);

   /* Initaializing new variable added for UL eff */
   rgSchCmnUlEffTbl[RG_SCH_CMN_NOR_CP] = &rgSchCmnNorUlEff[0];
   /* Reason behind using 3 as the number of symbols to rule out for
    * efficiency table computation would be that we are using 2 symbols for
    * DMRS(1 in each slot) and 1 symbol for SRS*/
   rgSCHCmnCompUlEff(RGSCH_UL_SYM_DMRS_SRS,RG_SCH_CMN_NOR_CP,rgSchCmnUlEffTbl[RG_SCH_CMN_NOR_CP]);

   for (i = RGSCH_NUM_ITBS - 1, j = RG_SCH_CMN_UL_NUM_CQI - 1;
         i >= 0 && j > 0; --i)
   {
      if ((*effTbl)[i] <= cqiTbl[j].eff)
      {
         mapTbl[j--] = (U8)i;
      }
   }
   for (; j > 0; --j)
   {
      mapTbl[j] = 0;
   }
   effTbl    = &rgSchCmnExtUlEff[0];
   mapTbl    = &rgSchCmnUlCqiToTbsTbl[RG_SCH_CMN_EXT_CP][0];

   /* Initaializing new variable added for UL eff */
   rgSchCmnUlEffTbl[RG_SCH_CMN_EXT_CP] = &rgSchCmnExtUlEff[0];
   /* Reason behind using 3 as the number of symbols to rule out for
    * efficiency table computation would be that we are using 2 symbols for
    * DMRS(1 in each slot) and 1 symbol for SRS*/
   rgSCHCmnCompUlEff(3,RG_SCH_CMN_EXT_CP,rgSchCmnUlEffTbl[RG_SCH_CMN_EXT_CP]);

   for (i = RGSCH_NUM_ITBS - 1, j = RG_SCH_CMN_UL_NUM_CQI - 1;
         i >= 0 && j > 0; --i)
   {
      if ((*effTbl)[i] <= cqiTbl[j].eff)
      {
         mapTbl[j--] = (U8)i;
      }
   }
   for (; j > 0; --j)
   {
      mapTbl[j] = 0;
   }
   rgSCHPwrInit();
   RETVOID;
}


/**
 * @brief This function initializes all the data for the scheduler.
 *
 * @details
 *
 *     Function: rgSCHCmnInit
 *     Purpose:  This function initializes the following information:
 *               1. Efficiency table
 *               2. CQI to table index - It is one row for upto 3 RBs
 *                  and another row for greater than 3 RBs
 *                  currently extended prefix is compiled out.
 *     Invoked by: MAC intialization code..may be ActvInit
 *
 *  @return  Void
 *
 **/
#ifdef ANSI
PUBLIC Void rgSCHCmnInit
(
)
#else
PUBLIC Void rgSCHCmnInit()
#endif
{
   U8   idx;
   TRC2(rgSCHCmnInit);

   rgSCHCmnDlInit();
   rgSCHCmnUlInit();
#ifdef LTEMAC_SPS
   rgSCHCmnSpsInit();
#endif

   /* Init the function pointers */
   rgSchCmnApis.rgSCHRgrUeCfg         = rgSCHCmnRgrUeCfg;
   rgSchCmnApis.rgSCHRgrUeRecfg       = rgSCHCmnRgrUeRecfg;
   rgSchCmnApis.rgSCHFreeUe           = rgSCHCmnUeDel;
   rgSchCmnApis.rgSCHRgrCellCfg       = rgSCHCmnRgrCellCfg;
   rgSchCmnApis.rgSCHRgrCellRecfg     = rgSCHCmnRgrCellRecfg;
   rgSchCmnApis.rgSCHFreeCell         = rgSCHCmnCellDel;
   rgSchCmnApis.rgSCHRgrLchCfg        = rgSCHCmnRgrLchCfg;
   rgSchCmnApis.rgSCHRgrLcgCfg        = rgSCHCmnRgrLcgCfg;
   rgSchCmnApis.rgSCHRgrLchRecfg      = rgSCHCmnRgrLchRecfg;
   rgSchCmnApis.rgSCHRgrLcgRecfg      = rgSCHCmnRgrLcgRecfg;
   rgSchCmnApis.rgSCHFreeDlLc         = rgSCHCmnFreeDlLc;
   rgSchCmnApis.rgSCHFreeLcg          = rgSCHCmnLcgDel;
   rgSchCmnApis.rgSCHRgrLchDel        = rgSCHCmnRgrLchDel;
   rgSchCmnApis.rgSCHTti              = rgSCHCmnTti;
   rgSchCmnApis.rgSCHActvtUlUe        = rgSCHCmnActvtUlUe;
   rgSchCmnApis.rgSCHActvtDlUe        = rgSCHCmnActvtDlUe;
   rgSchCmnApis.rgSCHHdlUlTransInd    = rgSCHCmnHdlUlTransInd;
   rgSchCmnApis.rgSCHDlDedBoUpd       = rgSCHCmnDlDedBoUpd;
   rgSchCmnApis.rgSCHUlRecMsg3Alloc   = rgSCHCmnUlRecMsg3Alloc;
   rgSchCmnApis.rgSCHUlCqiInd         = rgSCHCmnUlCqiInd;
   rgSchCmnApis.rgSCHPucchDeltaPwrInd = rgSCHPwrPucchDeltaInd;
   rgSchCmnApis.rgSCHUlHqProcForUe    = rgSCHCmnUlHqProcForUe;
#ifdef RG_UNUSED
   rgSchCmnApis.rgSCHUpdUlHqProc      = rgSCHCmnUpdUlHqProc;
#endif
   rgSchCmnApis.rgSCHUpdBsrShort      = rgSCHCmnUpdBsrShort;
   rgSchCmnApis.rgSCHUpdBsrTrunc      = rgSCHCmnUpdBsrTrunc;
   rgSchCmnApis.rgSCHUpdBsrLong       = rgSCHCmnUpdBsrLong;
   rgSchCmnApis.rgSCHUpdPhr           = rgSCHCmnUpdPhr;
   rgSchCmnApis.rgSCHContResUlGrant   = rgSCHCmnContResUlGrant;
   rgSchCmnApis.rgSCHSrRcvd           = rgSCHCmnSrRcvd;
   rgSchCmnApis.rgSCHFirstRcptnReq    = rgSCHCmnFirstRcptnReq;
   rgSchCmnApis.rgSCHNextRcptnReq     = rgSCHCmnNextRcptnReq;
   rgSchCmnApis.rgSCHFirstHqFdbkAlloc = rgSCHCmnFirstHqFdbkAlloc;
   rgSchCmnApis.rgSCHNextHqFdbkAlloc  = rgSCHCmnNextHqFdbkAlloc;
   rgSchCmnApis.rgSCHDlProcAddToRetx  = rgSCHCmnDlProcAddToRetx;
   rgSchCmnApis.rgSCHDlCqiInd         = rgSCHCmnDlCqiInd;
#ifdef TFU_UPGRADE
   rgSchCmnApis.rgSCHSrsInd           = rgSCHCmnSrsInd;
#endif
   rgSchCmnApis.rgSCHDlTARpt          = rgSCHCmnDlTARpt;
   rgSchCmnApis.rgSCHDlRlsSubFrm      = rgSCHCmnDlRlsSubFrm;
   rgSchCmnApis.rgSCHUeReset          = rgSCHCmnUeReset;
#ifdef LTEMAC_SPS
   rgSchCmnApis.rgSCHHdlCrntiCE         = rgSCHCmnHdlCrntiCE;
   rgSchCmnApis.rgSCHDlProcAck        = rgSCHCmnDlProcAck;
   rgSchCmnApis.rgSCHDlRelPdcchFbk    = rgSCHCmnDlRelPdcchFbk;
   rgSchCmnApis.rgSCHUlSpsRelInd      = rgSCHCmnUlSpsRelInd;
   rgSchCmnApis.rgSCHUlSpsActInd      = rgSCHCmnUlSpsActInd;
   rgSchCmnApis.rgSCHUlCrcFailInd     = rgSCHCmnUlCrcFailInd;
   rgSchCmnApis.rgSCHUlCrcInd     = rgSCHCmnUlCrcInd;
#endif
   rgSchCmnApis.rgSCHDrxStrtInActvTmrInUl = rgSCHCmnDrxStrtInActvTmrInUl;
   rgSchCmnApis.rgSCHUpdUeDataIndLcg      = rgSCHCmnUpdUeDataIndLcg;

   for (idx = 0; idx < RGSCH_NUM_SCHEDULERS; ++idx)
   {
      rgSchUlSchdInits[idx](&rgSchUlSchdTbl[idx]);
      rgSchDlSchdInits[idx](&rgSchDlSchdTbl[idx]);
   }

#if (defined (RG_PHASE2_SCHED) && defined(TFU_UPGRADE))
   for (idx = 0; idx < RGSCH_NUM_DLFS_SCHEDULERS; ++idx)
   {
      rgSchDlfsSchdInits[idx](&rgSchDlfsSchdTbl[idx]);
   }
#endif
   RETVOID;
}


/**
 * @brief This function is a wrapper to call scheduler specific API.
 *
 * @details
 *
 *     Function: rgSCHCmnDlRlsSubFrm
 *     Purpose:  Releases scheduler Information from DL SubFrm.
 *
 *     Invoked by: DHM
 *
 *  @param[in]   RgSchCellCb     *cell
 *  @param[out]  CmLteTimingInfo frm
 *  @return  Void
 *
 **/
#ifdef ANSI
PUBLIC Void rgSCHCmnDlRlsSubFrm
(
RgSchCellCb        *cell,
CmLteTimingInfo   frm
)
#else
PUBLIC Void rgSCHCmnDlRlsSubFrm(cell, frm)
RgSchCellCb        *cell;
CmLteTimingInfo    frm;
#endif
{
   RgSchCmnCell        *cellSch = RG_SCH_CMN_GET_CELL(cell);
   RgSchDlSf           *sf;

   TRC2(rgSCHCmnDlRlsSubFrm);

   /* Get the pointer to the subframe */
   sf = rgSCHUtlSubFrmGet(cell, frm);

   rgSCHUtlSubFrmPut(cell, sf);
   if (sf->dlfsSf)
   {
      /* Re-initialize DLFS specific information for the sub-frame */
      cellSch->apisDlfs->rgSCHDlfsReinitSf(cell, sf);
   }
   RETVOID;
}



/**
 * @brief This function is the starting function for DL allocation.
 *
 * @details
 *
 *     Function: rgSCHCmnDlCmnChAlloc
 *     Purpose:  Scheduling for downlink. It performs allocation in the order
 *               of priority wich BCCH/PCH first, CCCH, Random Access and TA.
 *
 *     Invoked by: Scheduler
 *
 *  @param[in]  RgSchCellCb*           cell
 *  @param[out] RgSchCmnDlRbAllocInfo* allocInfo
 *  @return  Void
 *
 **/
#ifdef ANSI
PRIVATE Void rgSCHCmnDlCmnChAlloc
(
RgSchCellCb             *cell,
RgSchCmnDlRbAllocInfo   *allocInfo
)
#else
PRIVATE Void rgSCHCmnDlCmnChAlloc(cell, allocInfo)
RgSchCellCb             *cell;
RgSchCmnDlRbAllocInfo   *allocInfo;
#endif
{
   RgSchCmnCell *cellSch = RG_SCH_CMN_GET_CELL(cell);

   TRC2(rgSCHCmnDlCmnChAlloc);

   rgSCHCmnDlCcchRetx(cell, allocInfo);
   /* LTE_ADV_FLAG_REMOVED_START */
   if(RG_SCH_ABS_ENABLED_ABS_SF == cell->lteAdvCb.absDlSfInfo)
   {
      if(cell->lteAdvCb.absCfg.absPatternType & RGR_ABS_MUTE)
      {
         /*eNodeB need to blank the subframe */
      }
      else
      {
         rgSCHCmnDlCcchTx(cell, allocInfo);
      }
   }
   else
   {
      rgSCHCmnDlCcchTx(cell, allocInfo);
   }
   /* LTE_ADV_FLAG_REMOVED_END */

#ifdef RGR_V1

   /*Added these function calls for processing CCCH SDU arriving
    * after guard timer expiry.Functions differ from above two functions
    * in using ueCb instead of raCb.*/
   rgSCHCmnDlCcchSduRetx(cell, allocInfo);
   /* LTE_ADV_FLAG_REMOVED_START */
   if(RG_SCH_ABS_ENABLED_ABS_SF == cell->lteAdvCb.absDlSfInfo)
   {
      if(cell->lteAdvCb.absCfg.absPatternType & RGR_ABS_MUTE)
      {
         /*eNodeB need to blank the subframe */
      }
      else
      {
         rgSCHCmnDlCcchSduTx(cell, allocInfo);
      }
   }
   else
   {
      rgSCHCmnDlCcchSduTx(cell, allocInfo);
   }
   /* LTE_ADV_FLAG_REMOVED_END */
#endif

#ifdef LTE_TDD
   if(cellSch->ul.msg3SchdIdx != RGSCH_INVALID_INFO)
   {
      /* Do not schedule msg3 if there is a CFI change ongoing */
      if (cellSch->dl.currCfi == cellSch->dl.newCfi)
      {
         rgSCHCmnDlRaRsp(cell, allocInfo);
      }
   }
#else
   /* LTE_ADV_FLAG_REMOVED_START */
   if(RG_SCH_ABS_ENABLED_ABS_SF == cell->lteAdvCb.absDlSfInfo)
   {
      if(cell->lteAdvCb.absCfg.absPatternType & RGR_ABS_MUTE)
      {
         /*eNodeB need to blank the subframe */
      }
      else
      {
         /* Do not schedule msg3 if there is a CFI change ongoing */
         if (cellSch->dl.currCfi == cellSch->dl.newCfi)
         {
            rgSCHCmnDlRaRsp(cell, allocInfo);
         }
      }
   }
   else
   {
      /* Do not schedule msg3 if there is a CFI change ongoing */
      if (cellSch->dl.currCfi == cellSch->dl.newCfi)
      {
         rgSCHCmnDlRaRsp(cell, allocInfo);
      }
   }
   /* LTE_ADV_FLAG_REMOVED_END */
#endif

   RETVOID;
}

#ifdef RGR_V1
/**
 * @brief Scheduling for CCCH SDU.
 *
 * @details
 *
 *     Function: rgSCHCmnCcchSduAlloc
 *     Purpose:  Scheduling for CCCH SDU
 *
 *     Invoked by: Scheduler
 *
 *  @param[in]  RgSchCellCb*          cell
 *  @param[in]  RgSchUeCb*            ueCb
 *  @param[out] RgSchCmnDlRbAllocInfo *allocInfo
 *  @return  S16
 *
 **/
#ifdef ANSI
PRIVATE S16 rgSCHCmnCcchSduAlloc
(
RgSchCellCb                *cell,
RgSchUeCb                  *ueCb,
RgSchCmnDlRbAllocInfo      *allocInfo
)
#else
PRIVATE S16 rgSCHCmnCcchSduAlloc(cell, ueCb, allocInfo)
RgSchCellCb                *cell;
RgSchUeCb                  *ueCb;
RgSchCmnDlRbAllocInfo      *allocInfo;
#endif
{
   RgSchCmnCell       *cellSch = RG_SCH_CMN_GET_CELL(cell);
   RgSchCmnDlUe       *ueDl = RG_SCH_CMN_GET_DL_UE(ueCb);

   TRC2(rgSCHCmnCcchSduAlloc);

   /* Return if subframe BW exhausted */
   if (allocInfo->ccchSduAlloc.ccchSduDlSf->bw <=
       allocInfo->ccchSduAlloc.ccchSduDlSf->bwAssigned)
   {
      RLOG_ARG1(L_ERROR,DBG_CELLID,cell->cellId,
         "bw<=bwAssigned for UEID:%d",ueCb->ueId);
      RETVALUE(RFAILED);
   }

   if (rgSCHDhmGetCcchSduHqProc(ueCb, cellSch->dl.time, &(ueDl->proc)) != ROK)
   {
      RLOG_ARG1(L_ERROR,DBG_CELLID,cell->cellId,
         "rgSCHDhmGetCcchSduHqProc failed UEID:%d",ueCb->ueId);
      RETVALUE(RFAILED);
   }

   ueCb->dl.dlAllocCb.dlSf = allocInfo->ccchSduAlloc.ccchSduDlSf;

   if (rgSCHCmnCcchSduDedAlloc(cell, ueCb) != ROK)
   {
      /* Fix : syed Minor failure handling, release hqP if Unsuccessful */    
      rgSCHDhmRlsHqpTb(ueDl->proc, 0, FALSE);
      RLOG_ARG1(L_ERROR,DBG_CELLID,cell->cellId,
         "rgSCHCmnCcchSduDedAlloc failed UEID:%d",ueCb->ueId);
      RETVALUE(RFAILED);
   }
   cmLListAdd2Tail(&allocInfo->ccchSduAlloc.ccchSduTxLst, &ueDl->proc->reqLnk);
   ueDl->proc->reqLnk.node = (PTR)ueDl->proc;
   allocInfo->ccchSduAlloc.ccchSduDlSf->schdCcchUe++;

   RETVALUE(ROK);
}
/**
 * @brief This function scheduler for downlink CCCH messages.
 *
 * @details
 *
 *     Function: rgSCHCmnDlCcchSduTx
 *     Purpose:  Scheduling for downlink CCCH
 *
 *     Invoked by: Scheduler
 *
 *  @param[in]  RgSchCellCb           *cell
 *  @param[out] RgSchCmnDlRbAllocInfo *allocInfo
 *  @return  Void
 *
 **/
#ifdef ANSI
PRIVATE Void rgSCHCmnDlCcchSduTx
(
RgSchCellCb             *cell,
RgSchCmnDlRbAllocInfo   *allocInfo
)
#else
PRIVATE Void rgSCHCmnDlCcchSduTx(cell, allocInfo)
RgSchCellCb             *cell;
RgSchCmnDlRbAllocInfo   *allocInfo;
#endif
{
   CmLList           *node;
   RgSchUeCb         *ueCb;
   RgSchCmnDlUe      *ueCmnDl;
   RgSchCmnCell      *cellSch = RG_SCH_CMN_GET_CELL(cell);

   RgSchDlSf         *dlSf = allocInfo->ccchSduAlloc.ccchSduDlSf;
   
   TRC2(rgSCHCmnDlCcchSduTx);

   node = cell->ccchSduUeLst.first;
   while(node)
   {
      if(cellSch->dl.maxCcchPerDlSf &&
            dlSf->schdCcchUe == cellSch->dl.maxCcchPerDlSf)
      {
         break;
      }
      else
      {
         ueCb = (RgSchUeCb *)(node->node);
         ueCmnDl  = RG_SCH_CMN_GET_DL_UE(ueCb);
         node = node->next;
         /* Fix : syed postpone scheduling for this
          * until msg4 is done */
         /* Fix : syed RLC can erroneously send CCCH SDU BO 
          * twice. Hence an extra guard to avoid if already 
          * scheduled for RETX */
         if ((!(ueCb->dl.dlInactvMask & RG_HQENT_INACTIVE)) &&
               (!ueCmnDl->proc))
         {
            if ((rgSCHCmnCcchSduAlloc(cell, ueCb, allocInfo)) != ROK)
            {
               break;
            }
         }
         else
         {
            RLOG_ARG1(L_ERROR,DBG_CELLID,cell->cellId,"ERROR!! THIS SHOULD "
                     "NEVER HAPPEN for UEID:%d", ueCb->ueId);
            continue;
         }
      }
   }
   RETVOID;
}
#endif

/**
 * @brief This function scheduler for downlink CCCH messages.
 *
 * @details
 *
 *     Function: rgSCHCmnDlCcchTx
 *     Purpose:  Scheduling for downlink CCCH
 *
 *     Invoked by: Scheduler
 *
 *  @param[in]  RgSchCellCb           *cell
 *  @param[out] RgSchCmnDlRbAllocInfo *allocInfo
 *  @return  Void
 *
 **/
#ifdef ANSI
PRIVATE Void rgSCHCmnDlCcchTx
(
RgSchCellCb             *cell,
RgSchCmnDlRbAllocInfo   *allocInfo
)
#else
PRIVATE Void rgSCHCmnDlCcchTx(cell, allocInfo)
RgSchCellCb             *cell;
RgSchCmnDlRbAllocInfo   *allocInfo;
#endif
{
   CmLList           *node;
   RgSchRaCb         *raCb;
   RgSchCmnCell      *cellSch = RG_SCH_CMN_GET_CELL(cell);
   RgSchDlSf         *dlSf = allocInfo->msg4Alloc.msg4DlSf;
   
   TRC2(rgSCHCmnDlCcchTx);

   node = cell->raInfo.toBeSchdLst.first;
   while(node)
   {
      if(cellSch->dl.maxCcchPerDlSf &&
            dlSf->schdCcchUe == cellSch->dl.maxCcchPerDlSf)
      {
         break;
      }
      else
      {

         raCb = (RgSchRaCb *)(node->node);
         node = node->next;
         /* Address allocation for this UE for MSG 4 */
         /* Allocation for Msg4 */
         if ((rgSCHCmnMsg4Alloc(cell, raCb, allocInfo)) != ROK)
         {
            break;
         }
      }
   }
   RETVOID;
}

#ifdef RGR_V1
/**
 * @brief This function scheduler for downlink CCCH messages.
 *
 * @details
 *
 *     Function: rgSCHCmnDlCcchSduRetx
 *     Purpose:  Scheduling for downlink CCCH
 *
 *     Invoked by: Scheduler
 *
 *  @param[in]  RgSchCellCb           *cell
 *  @param[out] RgSchCmnDlRbAllocInfo *allocInfo
 *  @return  Void
 *
 **/
#ifdef ANSI
PRIVATE Void rgSCHCmnDlCcchSduRetx
(
RgSchCellCb             *cell,
RgSchCmnDlRbAllocInfo   *allocInfo
)
#else
PRIVATE Void rgSCHCmnDlCcchSduRetx(cell, allocInfo)
RgSchCellCb             *cell;
RgSchCmnDlRbAllocInfo   *allocInfo;
#endif
{
   CmLList           *node;
   RgSchCmnCell      *cellSch = RG_SCH_CMN_GET_CELL(cell);
   RgSchUeCb         *ueCb;
   RgSchDlHqProcCb   *hqP;
   U8                retxBw = 0;
   RgSchCmnDlUe      *ueDl;
   RgSchDlSf         *dlSf = allocInfo->ccchSduAlloc.ccchSduDlSf;
   
   TRC2(rgSCHCmnDlCcchSduRetx);

   node = cellSch->dl.ccchSduRetxLst.first;
   while(node)
   {
      if(cellSch->dl.maxCcchPerDlSf &&
            dlSf->schdCcchUe == cellSch->dl.maxCcchPerDlSf)
      {
         break;
      }
      else
      {

         hqP = (RgSchDlHqProcCb *)(node->node);
         node = node->next;

         /* DwPts Scheduling Changes Start */      
#ifdef LTE_TDD
         if (rgSCHCmnRetxAvoidTdd(allocInfo->ccchSduAlloc.ccchSduDlSf, 
                  cell, hqP) == TRUE)
         {
            continue;  
         }
#endif
         /* DwPts Scheduling Changes End */     

         if (hqP->tbInfo[0].dlGrnt.numRb > (dlSf->bw - dlSf->bwAssigned))
         {
            break;
         }
         ueCb = (RgSchUeCb*)(hqP->hqE->ue);
         ueDl = RG_SCH_CMN_GET_DL_UE(ueCb);
         /* Fill RB Alloc Info */
         ueCb->dl.dlAllocCb.dlSf = dlSf;
         ueCb->dl.dlAllocCb.tbInfo[0].bytesReq =  hqP->tbInfo[0].ccchSchdInfo.totBytes;
         ueCb->dl.dlAllocCb.rbsReq = hqP->tbInfo[0].dlGrnt.numRb;
         /* Fix : syed iMcs setting did not correspond to RETX */
         RG_SCH_CMN_GET_MCS_FOR_RETX((&hqP->tbInfo[0]), 
               ueCb->dl.dlAllocCb.tbInfo[0].imcs);
         ueCb->dl.dlAllocCb.rnti = ueCb->ueId;
         ueCb->dl.dlAllocCb.tbInfo[0].noLyr = hqP->tbInfo[0].numLyrs;
         /* Fix : syed Copying info in entirety without depending on stale TX information */
         ueCb->dl.dlAllocCb.tbInfo[0].tbCb = &hqP->tbInfo[0];
         ueCb->dl.dlAllocCb.tbInfo[0].schdlngForTb = TRUE;
         /* Fix : syed Assigning proc to scratchpad */ 
         ueDl->proc = hqP;

         retxBw += ueCb->dl.dlAllocCb.rbsReq;

         cmLListAdd2Tail(&allocInfo->ccchSduAlloc.ccchSduRetxLst, \
               &hqP->reqLnk);
         hqP->reqLnk.node = (PTR)hqP;
         dlSf->schdCcchUe++;
      }
   }
   dlSf->bwAssigned += retxBw;
   RETVOID;
}
#endif

/**
 * @brief This function scheduler for downlink CCCH messages.
 *
 * @details
 *
 *     Function: rgSCHCmnDlCcchRetx
 *     Purpose:  Scheduling for downlink CCCH
 *
 *     Invoked by: Scheduler
 *
 *  @param[in]  RgSchCellCb           *cell
 *  @param[out] RgSchCmnDlRbAllocInfo *allocInfo
 *  @return  Void
 *
 **/
#ifdef ANSI
PRIVATE Void rgSCHCmnDlCcchRetx
(
RgSchCellCb             *cell,
RgSchCmnDlRbAllocInfo   *allocInfo
)
#else
PRIVATE Void rgSCHCmnDlCcchRetx(cell, allocInfo)
RgSchCellCb             *cell;
RgSchCmnDlRbAllocInfo   *allocInfo;
#endif
{
   CmLList           *node;
   RgSchCmnCell      *cellSch = RG_SCH_CMN_GET_CELL(cell);
   RgSchRaCb         *raCb;
   RgSchDlHqProcCb   *hqP;
   U8                retxBw = 0;
   RgSchDlSf         *dlSf = allocInfo->msg4Alloc.msg4DlSf;
        
   TRC2(rgSCHCmnDlCcchRetx);

   node = cellSch->dl.msg4RetxLst.first;
   while(node)
   {
      if(cellSch->dl.maxCcchPerDlSf &&
            dlSf->schdCcchUe == cellSch->dl.maxCcchPerDlSf)
      {
         break;
      }
      else
      {
         hqP = (RgSchDlHqProcCb *)(node->node);

         node = node->next;

         /* DwPts Scheduling Changes Start */     
#ifdef LTE_TDD      
         if (rgSCHCmnRetxAvoidTdd(allocInfo->msg4Alloc.msg4DlSf, 
                  cell, hqP) == TRUE)
         {
            continue;  
         }
#endif      
         /* DwPts Scheduling Changes End */      

         if (hqP->tbInfo[0].dlGrnt.numRb > (dlSf->bw - dlSf->bwAssigned))
         {
            break;
         }
         raCb = (RgSchRaCb*)(hqP->hqE->raCb);
         /* Fill RB Alloc Info */
         raCb->rbAllocInfo.dlSf = dlSf;
         raCb->rbAllocInfo.tbInfo[0].bytesReq =  hqP->tbInfo[0].ccchSchdInfo.totBytes;
         raCb->rbAllocInfo.rbsReq = hqP->tbInfo[0].dlGrnt.numRb;
         /* Fix : syed iMcs setting did not correspond to RETX */
         RG_SCH_CMN_GET_MCS_FOR_RETX((&hqP->tbInfo[0]), 
               raCb->rbAllocInfo.tbInfo[0].imcs);
         raCb->rbAllocInfo.rnti = raCb->tmpCrnti;
         raCb->rbAllocInfo.tbInfo[0].noLyr = hqP->tbInfo[0].numLyrs;
         /* Fix; syed Copying info in entirety without depending on stale TX information */
         raCb->rbAllocInfo.tbInfo[0].tbCb = &hqP->tbInfo[0];
         raCb->rbAllocInfo.tbInfo[0].schdlngForTb = TRUE;

         retxBw += raCb->rbAllocInfo.rbsReq;

         cmLListAdd2Tail(&allocInfo->msg4Alloc.msg4RetxLst, \
               &hqP->reqLnk);
         hqP->reqLnk.node = (PTR)hqP;
         dlSf->schdCcchUe++;
      }
   }
   dlSf->bwAssigned += retxBw;
   RETVOID;
}


/**
 * @brief This function implements scheduler DL allocation for
 *        for broadcast (on PDSCH) and paging.
 *
 * @details
 *
 *     Function: rgSCHCmnDlBcchPcch
 *     Purpose:  This function implements scheduler for DL allocation
 *               for broadcast (on PDSCH) and paging.
 *
 *     Invoked by: Scheduler
 *
 *  @param[in]  RgSchCellCb*     cell
 *  @return  S16
 *      -# ROK
 *      -# RFAILED
 **/
#ifdef ANSI
PRIVATE Void rgSCHCmnDlBcchPcch
(
RgSchCellCb             *cell,
RgSchCmnDlRbAllocInfo   *allocInfo,
RgInfSfAlloc            *subfrmAlloc
)
#else
PRIVATE Void rgSCHCmnDlBcchPcch(cell, allocInfo, subfrmAlloc)
RgSchCellCb             *cell;
RgSchCmnDlRbAllocInfo   *allocInfo;
RgInfSfAlloc            *subfrmAlloc;
#endif
{
   CmLteTimingInfo   frm;
   RgSchDlSf         *sf;
   RgSchClcDlLcCb    *pcch;
   RgSchClcBoRpt     *bo;
#ifndef RGR_SI_SCH
   Bool              valid;
   RgSchClcDlLcCb    *bcch, *bch;
#endif/*RGR_SI_SCH*/


   TRC2(rgSCHCmnDlBcchPcch);

   frm   = cell->crntTime;
#ifdef LTEMAC_HDFDD
   /* For HDFDD we need scheduling information at least RG_SCH_CMN_DL_DELTA
      + RG_SCH_CMN_HARQ_INTERVAL (7) subframes ahead */
   RGSCH_INCR_SUB_FRAME(frm, RG_SCH_CMN_DL_DELTA + RG_SCH_CMN_HARQ_INTERVAL);
#else
   RGSCH_INCR_SUB_FRAME(frm, RG_SCH_CMN_DL_DELTA);
#endif

   /* Compute the subframe for which allocation is being made        */
   /* essentially, we need pointer to the dl frame for this subframe */
   sf = rgSCHUtlSubFrmGet(cell, frm);

#ifdef LTEMAC_SPS
   /* Update subframe-wide allocation information with SPS allocation */
   rgSCHCmnSpsDlUpdDlSfAllocWithSps(cell, frm, sf);
#endif

#ifndef RGR_SI_SCH
   bch = rgSCHDbmGetBcchOnBch(cell);
#if (ERRCLASS & ERRCLS_DEBUG)
   if (bch == NULLP)
   {
      RLOG_ARG0(L_ERROR,DBG_CELLID,cell->cellId,"BCCH on BCH is not configured");
      RETVOID;
   }
#endif
   if (bch->boLst.first != NULLP)
   {
      bo = (RgSchClcBoRpt *)(bch->boLst.first->node);
      if (RGSCH_TIMEINFO_SAME(frm, bo->timeToTx))
      {
         sf->bch.tbSize = bo->bo;
         cmLListDelFrm(&bch->boLst, bch->boLst.first);
         /* ccpu00117052 - MOD - Passing double pointer
            for proper NULLP assignment*/
         rgSCHUtlFreeSBuf(cell->instIdx, (Data **)&bo, sizeof(*bo));
         rgSCHUtlFillRgInfCmnLcInfo(sf, subfrmAlloc, bch->lcId,TRUE);
      }
   }
   else
   {
      if ((frm.sfn % 4 == 0) && (frm.subframe == 0))
      {
      }
   }

   allocInfo->bcchAlloc.schdFirst = FALSE;
   bcch = rgSCHDbmGetFirstBcchOnDlsch(cell);
#if (ERRCLASS & ERRCLS_DEBUG)
   if (bcch == NULLP)
   {
      RLOG_ARG0(L_ERROR,DBG_CELLID,cell->cellId,"BCCH on DLSCH is not configured");
      RETVOID;
   }
#endif
   if (bcch->boLst.first != NULLP)
   {
      bo = (RgSchClcBoRpt *)(bcch->boLst.first->node);

      if (RGSCH_TIMEINFO_SAME(frm, bo->timeToTx))
      {
         allocInfo->bcchAlloc.schdFirst = TRUE;
         /* Time to perform allocation for this BCCH transmission */
         rgSCHCmnClcAlloc(cell, sf, bcch, RGSCH_SI_RNTI, allocInfo);
      }
   }

   if(!allocInfo->bcchAlloc.schdFirst)
   {
      CmLList   *lnk;
      bcch = rgSCHDbmGetSecondBcchOnDlsch(cell);
#if (ERRCLASS & ERRCLS_DEBUG)
      if (bcch == NULLP)
      {
         RLOG_ARG0(L_ERROR,DBG_CELLID,cell->cellId,"BCCH on DLSCH is not configured");
         RETVOID;
      }
#endif
      lnk = bcch->boLst.first;
      while (lnk != NULLP)
      {
         bo = (RgSchClcBoRpt *)(lnk->node);
         lnk = lnk->next;
         valid = rgSCHCmnChkInWin(frm, bo->timeToTx, bo->maxTimeToTx);

         if(valid)
         {
            bo->i = RGSCH_CALC_SF_DIFF(frm, bo->timeToTx);
            /* Time to perform allocation for this BCCH transmission */
            rgSCHCmnClcAlloc(cell, sf, bcch, RGSCH_SI_RNTI, allocInfo);
            break;
         }
         else
         {
            valid = rgSCHCmnChkPastWin(frm, bo->maxTimeToTx);
            if(valid)
            {
               cmLListDelFrm(&bcch->boLst, &bo->boLstEnt);
               /* ccpu00117052 - MOD - Passing double pointer
                  for proper NULLP assignment*/
               rgSCHUtlFreeSBuf(cell->instIdx, (Data **)&bo,
                     sizeof(RgSchClcBoRpt));
            }
         }
      }
   }
#else
   rgSCHDlSiSched(cell, allocInfo, subfrmAlloc);
#endif/*RGR_SI_SCH*/

   pcch = rgSCHDbmGetPcch(cell);
#ifdef ERRCLS_KW
   if (pcch == NULLP)
   {
      RLOG_ARG0(L_ERROR,DBG_CELLID,cell->cellId,"PCCH on DLSCH is not configured");
      RETVOID;
   }
#endif
   if (pcch->boLst.first != NULLP)
   {
      bo = (RgSchClcBoRpt *)(pcch->boLst.first->node);

      if (RGSCH_TIMEINFO_SAME(frm, bo->timeToTx))
      {
         /* Time to perform allocation for this PCCH transmission */
         rgSCHCmnClcAlloc(cell, sf, pcch, RGSCH_P_RNTI, allocInfo);
      }
   }
   RETVOID;
}

/*
*
*       Fun:   rgSCHCmnChkInWin
*
*       Desc:  This function checks if frm occurs in window
*
*       Ret:   TRUE      - if in window
*              FALSE     - otherwise
*
*       Notes: None
*
*       File:  rg_sch_cmn.c
*
*/
#ifdef ANSI
PRIVATE Bool rgSCHCmnChkInWin
(
CmLteTimingInfo   frm,
CmLteTimingInfo   start,
CmLteTimingInfo   end
)
#else
PRIVATE Bool rgSCHCmnChkInWin(frm, start, end)
CmLteTimingInfo   frm;
CmLteTimingInfo   start;
CmLteTimingInfo   end;
#endif
{
   Bool    inWin = FALSE;

   TRC2(rgSCHCmnChkInWin);

   if (end.sfn > start.sfn)
   {
      if (frm.sfn > start.sfn
            || (frm.sfn == start.sfn && frm.subframe >= start.subframe))
      {
         if (frm.sfn < end.sfn
               || (frm.sfn == end.sfn && frm.subframe <= start.subframe))
         {
            inWin = TRUE;
         }
      }
   }
   /* Testing for wrap around, sfn wraparound check should be enough */
   else if (end.sfn < start.sfn)
   {
      if (frm.sfn > start.sfn
            || (frm.sfn == start.sfn && frm.subframe >= start.subframe))
      {
         inWin = TRUE;
      }
      else
      {
         if (frm.sfn < end.sfn
               || (frm.sfn == end.sfn && frm.subframe <= end.subframe))
         {
            inWin = TRUE;
         }
      }
   }
   else  /* start.sfn == end.sfn */
   {
      if (frm.sfn == start.sfn
            && (frm.subframe >= start.subframe
               && frm.subframe <= end.subframe))
      {
         inWin = TRUE;
      }
   }

   RETVALUE(inWin);
} /* end of rgSCHCmnChkInWin*/

/*
*
*       Fun:   rgSCHCmnChkPastWin
*
*       Desc:  This function checks if frm has gone past window edge
*
*       Ret:   TRUE      - if past window edge
*              FALSE     - otherwise
*
*       Notes: None
*
*       File:  rg_sch_cmn.c
*
*/
#ifdef ANSI
PRIVATE Bool rgSCHCmnChkPastWin
(
CmLteTimingInfo   frm,
CmLteTimingInfo   end
)
#else
PRIVATE Bool rgSCHCmnChkPastWin(frm, end)
CmLteTimingInfo   frm;
CmLteTimingInfo   end;
#endif
{
   CmLteTimingInfo  refFrm = end;
   Bool             pastWin;

   TRC2(rgSCHCmnChkPastWin);

   RGSCH_INCR_FRAME(refFrm.sfn);
   RGSCH_INCR_SUB_FRAME(end, 1);
   pastWin = rgSCHCmnChkInWin(frm, end, refFrm);

   RETVALUE(pastWin);
} /* end of rgSCHCmnChkPastWin*/

/**
 * @brief This function implements allocation of the resources for common
 * channels BCCH, PCCH.
 *
 * @details
 *
 *     Function: rgSCHCmnClcAlloc
 *     Purpose:  This function implements selection of number of RBs based
 *               the allowed grant for the service. It is also responsible
 *               for selection of MCS for the transmission.
 *
 *     Invoked by: Scheduler
 *
 *  @param[in]  RgSchCellCb                *cell,
 *  @param[in]  RgSchDlSf                  *sf,
 *  @param[in]  RgSchClcDlLcCb             *lch,
 *  @param[in]  U16                        rnti,
 *  @param[out] RgSchCmnDlRbAllocInfo      *allocInfo
 *  @return     Void
 *
 **/
#ifdef ANSI
PRIVATE Void rgSCHCmnClcAlloc
(
RgSchCellCb             *cell,
RgSchDlSf               *sf,
RgSchClcDlLcCb          *lch,
U16                     rnti,
RgSchCmnDlRbAllocInfo   *allocInfo
)
#else
PRIVATE Void rgSCHCmnClcAlloc(cell, sf, lch, rnti, allocInfo)
RgSchCellCb             *cell;
RgSchDlSf               *sf;
RgSchClcDlLcCb          *lch;
U16                     rnti;
RgSchCmnDlRbAllocInfo   *allocInfo;
#endif
{
   RgSchCmnDlCell       *cellDl = RG_SCH_CMN_GET_DL_CELL(cell);
   RgSchClcBoRpt        *bo;
   U32                  rb=0;
   U8                   mcs;
   U32                  tbs;
#ifdef LTE_TDD   
   U8                   lostRe;
   U8                   cfi = cellDl->currCfi;  
#endif

   TRC2(rgSCHCmnClcAlloc);

   bo = (RgSchClcBoRpt *)(lch->boLst.first->node);

   mcs = bo->mcs;
   tbs = bo->bo;
   /* rgSCHCmnClcRbAllocForFxdTb(cell, bo->bo, cellDl->ccchCqi, &rb);*/
   if(cellDl->bitsPerRb==0)
   {
      while ((rgTbSzTbl[0][0][rb]) < (tbs*8))
      {
         rb++;
      }
      rb = rb+1;
   }
   else
   {
      rb = RGSCH_CEIL((tbs*8), cellDl->bitsPerRb);
   }
   /* DwPTS Scheduling Changes Start */   
#ifdef LTE_TDD
   if(sf->sfType == RG_SCH_SPL_SF_DATA) 
   {
      RGSCH_GET_SPS_SF_CFI(cell->bwCfg.dlTotalBw, cfi);

      /* Calculate the less RE's because of DwPTS */
      lostRe = rb * (cellDl->noResPerRb[cfi] - cellDl->numReDwPts[cfi]);

      /* Increase number of RBs in Spl SF to compensate for lost REs */
      rb += RGSCH_CEIL(lostRe, cellDl->numReDwPts[cfi]); 
   }
#endif
   /* DwPTS Scheduling Changes End */   
   /*ccpu00115595- end*/
   /* additional check to see if required RBs
    * exceeds the available */
   if (rb > sf->bw - sf->bwAssigned)
   {
      RLOG_ARG1(L_ERROR,DBG_CELLID,cell->cellId,"BW allocation "
                "failed for CRNTI:%d",rnti);
      RETVOID;
   }

   /* Update the subframe Allocated BW field */
   sf->bwAssigned = sf->bwAssigned + rb;
   /* Fill in the BCCH/PCCH transmission info to the RBAllocInfo struct */
   if (rnti == RGSCH_SI_RNTI)
   {
      allocInfo->bcchAlloc.rnti = rnti;
      allocInfo->bcchAlloc.dlSf = sf;
      allocInfo->bcchAlloc.tbInfo[0].bytesReq = tbs;
      allocInfo->bcchAlloc.rbsReq = rb;
      allocInfo->bcchAlloc.tbInfo[0].imcs = mcs;
      allocInfo->bcchAlloc.tbInfo[0].noLyr = 1;
      /* Nprb indication at PHY for common Ch */
      allocInfo->bcchAlloc.nPrb = bo->nPrb;
   }
   else
   {
      allocInfo->pcchAlloc.rnti = rnti;
      allocInfo->pcchAlloc.dlSf = sf;
      allocInfo->pcchAlloc.tbInfo[0].bytesReq = tbs;
      /* Qcom: Pagging RIV correction */
//      allocInfo->pcchAlloc.rbsReq = rb;
      allocInfo->pcchAlloc.rbsReq = bo->nPrb;
      allocInfo->pcchAlloc.tbInfo[0].imcs = mcs;
      allocInfo->pcchAlloc.tbInfo[0].noLyr = 1;
      allocInfo->pcchAlloc.nPrb = bo->nPrb;
   }
   RETVOID;
}


/**
 * @brief This function implements PDCCH allocation for common channels.
 *
 * @details
 *
 *     Function: rgSCHCmnCmnPdcchAlloc
 *     Purpose:  This function implements allocation of PDCCH for a UE.
 *               1. This uses index 0 of PDCCH table for efficiency.
 *               2. Uses he candidate PDCCH count for the aggr level.
 *               3. Look for availability for each candidate and choose
 *                  the first one available.
 *
 *     Invoked by: Scheduler
 *
 *  @param[in]  RgSchCellCb           *cell
 *  @param[in]  RgSchDlSf             *sf
 *  @return     RgSchPdcch *
 *               -# NULLP when unsuccessful
 *
 **/
#ifdef ANSI
PUBLIC RgSchPdcch *rgSCHCmnCmnPdcchAlloc
(
RgSchCellCb                *cell,
RgSchDlSf                  *subFrm
)
#else
PUBLIC RgSchPdcch *rgSCHCmnCmnPdcchAlloc(cell, subFrm)
RgSchCellCb                *cell;
RgSchDlSf                  *subFrm;
#endif
{
   U8                   i;
   CmLteAggrLvl         aggrLvl;
   U8                   pdcchCnt;
   RgSchPdcchInfo       *pdcchInfo;
   RgSchPdcch           *pdcch;
   RgSchCmnCell         *cellSch = RG_SCH_CMN_GET_CELL(cell);
   U8                   numCce;  /*store num CCEs based on 
                                  aggregation level */
   TRC2(rgSCHCmnCmnPdcchAlloc);

   aggrLvl   = cellSch->dl.cmnChAggrLvl;

   pdcchInfo = &(subFrm->pdcchInfo);

    /* Updating the no. of nCce in pdcchInfo, in case if CFI
    * was changed  */
#ifdef LTE_TDD
   if(subFrm->nCce != pdcchInfo->nCce)
   {   
      rgSCHUtlPdcchInit(cell, subFrm, subFrm->nCce);
   }
#else   
   if(cell->nCce != pdcchInfo->nCce)
   {
      rgSCHUtlPdcchInit(cell, subFrm, cell->nCce);
   }
#endif  

   switch (aggrLvl)
   {
      case CM_LTE_AGGR_LVL4:
        pdcchCnt = 4;
        numCce = 4;
        break;
      case CM_LTE_AGGR_LVL8:
        pdcchCnt = 2;
        numCce = 8;
        break;
      default:
        RETVALUE(NULLP);
   }

   /* Locate a PDCCH for the known aggregation level */
   for (i = 0; i < pdcchCnt; i++)
   {
      if (rgSCHUtlPdcchAvail(cell, pdcchInfo, i, aggrLvl, &pdcch) == TRUE)
      {
#ifdef LTEMAC_SPS
         pdcch->isSpsRnti = FALSE;
#endif
         /* Increment the CCE used counter in the current subframe */
         subFrm->cceCnt += numCce;

         RETVALUE(pdcch);
      }
   }

   /* PDCCH Allocation Failed, Mark cceFailure flag as TRUE */
   subFrm->isCceFailure = TRUE;

   RLOG_ARG1(L_DEBUG,DBG_CELLID,cell->cellId,
	    "PDCCH ERR: NO PDDCH AVAIL IN COMMON SEARCH SPACE aggr:%u", 
	    aggrLvl);
   RETVALUE(NULLP);
}


/**
 * @brief This function implements bandwidth allocation for common channels.
 *
 * @details
 *
 *     Function: rgSCHCmnClcRbAlloc
 *     Purpose:  This function implements bandwith allocation logic
 *               for common control channels.
 *
 *     Invoked by: Scheduler
 *
 *  @param[in]  RgSchCellCb*  cell
 *  @param[in]  U32           bo
 *  @param[in]  U8            cqi
 *  @param[in]  U8            *rb
 *  @param[in]  U32           *tbs
 *  @param[in]  U8            *mcs
 *  @param[in]  RgSchDlSf     *sf
 *  @return  Void
 *
 **/
#ifdef LTEMAC_SPS
#ifdef ANSI
PUBLIC Void rgSCHCmnClcRbAlloc
(
RgSchCellCb             *cell,
U32                     bo,
U8                      cqi,
U8                      *rb,
U32                     *tbs,
U8                      *mcs,
U8                      *iTbs,
Bool                    isSpsBo,
RgSchDlSf               *sf 
)
#else
PUBLIC Void rgSCHCmnClcRbAlloc(cell, bo, cqi, rb, tbs, mcs, iTbs, isSpsBo)
RgSchCellCb             *cell;
U32                     bo;
U8                      cqi;
U8                      *rb;
U32                     *tbs;
U8                      *mcs;
U8                      *iTbs;
Bool                    isSpsBo;
RgSchDlSf               *sf; 
#endif
#else
#ifdef ANSI
PRIVATE Void rgSCHCmnClcRbAlloc
(
RgSchCellCb             *cell,
U32                     bo,
U8                      cqi,
U8                      *rb,
U32                     *tbs,
U8                      *mcs,
RgSchDlSf               *sf 
)
#else
PRIVATE Void rgSCHCmnClcRbAlloc(cell, bo, cqi, rb, tbs, mcs, sf)
RgSchCellCb             *cell;
U32                     bo;
U8                      cqi;
U8                      *rb;
U32                     *tbs;
U8                      *mcs;
RgSchDlSf               *sf; 
#endif
#endif /* LTEMAC_SPS */
{
   U8                   iTbsVal;
   RgSchCmnTbSzEff      *effTbl;
   U32                  eff;
   U32                  noRes;
   RgSchCmnCell         *cellSch = RG_SCH_CMN_GET_CELL(cell);
   U8                   cfi = cellSch->dl.currCfi;
   U32                  tmpRb=0;

   TRC2(rgSCHCmnClcRbAlloc);

#ifdef LTEMAC_SPS   
   if(isSpsBo)
   {
     cfi = 3;
   }   
#endif   

#ifdef LTE_TDD   
   if(sf->sfType == RG_SCH_SPL_SF_DATA)
   {
      RGSCH_GET_SPS_SF_CFI(cell->bwCfg.dlTotalBw, cfi);
   }
#endif

   /* first get the CQI to MCS table and determine the number of RBs */
   effTbl = (RgSchCmnTbSzEff *)(cellSch->dl.cqiToEffTbl[0][cfi]);
   iTbsVal = (*(RgSchCmnCqiToTbs *)(cellSch->dl.cqiToTbsTbl[0][cfi]))[cqi];
   RG_SCH_CMN_DL_TBS_TO_MCS(iTbsVal, *mcs);
#ifdef LTEMAC_SPS
   if (isSpsBo)
   {
      if ( *mcs > RG_SCH_CMN_SPS_DL_MAX_MCS)
      {
         *mcs=15;
      }
      RG_SCH_CMN_DL_MCS_TO_TBS(*mcs,iTbsVal);
      *iTbs = iTbsVal;
   }
#endif

   /* Efficiency is number of bits per 1024 REs */
   eff  = (*effTbl)[iTbsVal];

   /* Get the number of REs needed for this bo  */
   noRes = ((bo * 8 * 1024) / eff );

   /* Get the number of RBs needed for this transmission */
   /* Number of RBs = No of REs / No of REs per RB       */
   tmpRb = RGSCH_CEIL(noRes, cellSch->dl.noResPerRb[cfi]);
   /* KWORK_FIX: added check to see if rb has crossed maxRb*/
   RGSCH_ARRAY_BOUND_CHECK_WITH_POS_IDX(cell->instIdx, rgTbSzTbl[0][0], (tmpRb-1));
   if (tmpRb > cellSch->dl.maxDlBwPerUe)
   {
      tmpRb = cellSch->dl.maxDlBwPerUe;
   }
   while ((rgTbSzTbl[0][iTbsVal][tmpRb-1]/8) < bo && 
           (tmpRb < cellSch->dl.maxDlBwPerUe))
   {
      tmpRb++;
      RGSCH_ARRAY_BOUND_CHECK_WITH_POS_IDX(cell->instIdx, rgTbSzTbl[0][0], (tmpRb-1));
   }
   *tbs =  rgTbSzTbl[0][iTbsVal][tmpRb-1]/8;
   *rb = (U8)tmpRb;
   /* DwPTS Scheduling Changes Start */   
#ifdef LTE_TDD 
   if(sf->sfType == RG_SCH_SPL_SF_DATA)
   {
      *tbs = rgSCHCmnCalcDwPtsTbSz(cell, bo, rb, &iTbsVal, 1, cfi);   
#ifdef LTEMAC_SPS
      *iTbs = iTbsVal;
#endif
   }
#endif
   /* DwPTS Scheduling Changes End */  
   RG_SCH_CMN_DL_TBS_TO_MCS(iTbsVal, *mcs);

   RETVOID;
}



/**
 * @brief Scheduling for MSG4.
 *
 * @details
 *
 *     Function: rgSCHCmnMsg4Alloc
 *     Purpose:  Scheduling for MSG4
 *
 *     Invoked by: Scheduler
 *
 *  @param[in]  RgSchCellCb*          cell
 *  @param[in]  RgSchRaCb*            raCb
 *  @param[out] RgSchCmnDlRbAllocInfo *allocInfo
 *  @return  S16
 *
 **/
#ifdef ANSI
PRIVATE S16 rgSCHCmnMsg4Alloc
(
RgSchCellCb                *cell,
RgSchRaCb                  *raCb,
RgSchCmnDlRbAllocInfo      *allocInfo
)
#else
PRIVATE S16 rgSCHCmnMsg4Alloc(cell, raCb, allocInfo)
RgSchCellCb                *cell;
RgSchRaCb                  *raCb;
RgSchCmnDlRbAllocInfo      *allocInfo;
#endif
{
   RgSchCmnCell       *cellSch = RG_SCH_CMN_GET_CELL(cell);

   TRC2(rgSCHCmnMsg4Alloc);

 /* SR_RACH_STATS : MSG4 TO BE TXED */
   rgNumMsg4ToBeTx++;
   /* Return if subframe BW exhausted */
   if (allocInfo->msg4Alloc.msg4DlSf->bw <=
       allocInfo->msg4Alloc.msg4DlSf->bwAssigned)
   {
      RLOG_ARG0(L_ERROR,DBG_CELLID,cell->cellId ,
         "bw<=bwAssigned");
      RETVALUE(RFAILED);
   }

   if (rgSCHDhmGetMsg4HqProc(raCb, cellSch->dl.time) != ROK)
   {
      RLOG_ARG0(L_ERROR,DBG_CELLID,cell->cellId,
         "rgSCHDhmGetMsg4HqProc failed");
      RETVALUE(RFAILED);
   }

   raCb->rbAllocInfo.dlSf = allocInfo->msg4Alloc.msg4DlSf;

   if (rgSCHCmnMsg4DedAlloc(cell, raCb) != ROK)
   {
      /* Fix : syed Minor failure handling, release hqP if Unsuccessful */    
      rgSCHDhmRlsHqpTb(raCb->dlHqE->msg4Proc, 0, FALSE);
      RLOG_ARG0(L_ERROR,DBG_CELLID,cell->cellId,
         "rgSCHCmnMsg4DedAlloc failed.");
      RETVALUE(RFAILED);
   }
   cmLListAdd2Tail(&allocInfo->msg4Alloc.msg4TxLst, &raCb->dlHqE->msg4Proc->reqLnk);
   raCb->dlHqE->msg4Proc->reqLnk.node = (PTR)raCb->dlHqE->msg4Proc;
   allocInfo->msg4Alloc.msg4DlSf->schdCcchUe++;

   RETVALUE(ROK);
}


/**
 * @brief This function implements PDCCH allocation for an UE.
 *
 * @details
 *
 *     Function: PdcchAlloc
 *     Purpose:  This function implements allocation of PDCCH for an UE.
 *               1. Get the aggregation level for the CQI of the UE.
 *               2. Get the candidate PDCCH count for the aggr level.
 *               3. Look for availability for each candidate and choose
 *                  the first one available.
 *
 *     Invoked by: Scheduler
 *
 *  @param[in]  cell
 *  @param[in]  subFrm
 *  @param[in]  y
 *  @param[in]  cqi
 *  @param[in]  dciFrmt
 *  @return  RgSchPdcch *
 *         -# NULLP when unsuccessful
 *
 **/
#ifdef ANSI
PUBLIC RgSchPdcch *rgSCHCmnPdcchAlloc
(
RgSchCellCb             *cell,
RgSchDlSf               *subFrm,
U32                     y,
U8                      cqi,
TfuDciFormat            dciFrmt,
Bool                    isDtx
)
#else
PUBLIC RgSchPdcch *rgSCHCmnPdcchAlloc(cell, subFrm, y, cqi, dciFrmt, isDtx)
RgSchCellCb             *cell;
RgSchDlSf               *subFrm;
U32                     y;
U8                      cqi;
TfuDciFormat            dciFrmt;
Bool                    isDtx;
#endif
{
   U8                   i;
   CmLteAggrLvl     aggrLvl;
   U16              loc;
   RgSchPdcchInfo   *pdcchInfo;
   U8               pdcchCnt;
   RgSchPdcch       *pdcch;
   RgSchCmnCell     *cellSch = RG_SCH_CMN_GET_CELL(cell);
   U8               numCce;

   TRC2(rgSCHCmnPdcchAlloc);

   /* 3.1 consider the selected DCI format size in determining the
    * aggregation level */
   aggrLvl   = cellSch->dciAggrLvl[cqi][dciFrmt];
   /* Incrementing aggrLvl by 1 if it not AGGR_LVL8(MAX SIZE)
    * inorder to increse the redudancy bits for better decoding of UE */
   if (isDtx)
   {
      if (aggrLvl != CM_LTE_AGGR_LVL8)
      {
         switch(aggrLvl)
         {
            case CM_LTE_AGGR_LVL1:
               aggrLvl = CM_LTE_AGGR_LVL2;
                break;
            case CM_LTE_AGGR_LVL2:
               aggrLvl = CM_LTE_AGGR_LVL4;
               break;
            case CM_LTE_AGGR_LVL4:
               aggrLvl = CM_LTE_AGGR_LVL8;
               break;
            default:
               break;
         }
         /* aggrLvl   += 1; */
      }
   }

   pdcchInfo = &subFrm->pdcchInfo;

   /* Updating the no. of nCce in pdcchInfo, in case if CFI
    * was changed  */
#ifdef LTE_TDD
   if(subFrm->nCce != pdcchInfo->nCce)
   {   
      rgSCHUtlPdcchInit(cell, subFrm, subFrm->nCce);
   }
#else   
   if(cell->nCce != pdcchInfo->nCce)
   {
      rgSCHUtlPdcchInit(cell, subFrm, cell->nCce);
   }
#endif       

   if (pdcchInfo->nCce < (1 << (aggrLvl - 1)))
   {
      /* PDCCH Allocation Failed, Mark cceFailure flag as TRUE */
      subFrm->isCceFailure = TRUE;
      RLOG_ARG1(L_DEBUG,DBG_CELLID,cell->cellId,
            "PDCCH ERR: NO PDDCH AVAIL IN UE SEARCH SPACE :aggr(%u)", 
            aggrLvl);

      RETVALUE(NULLP);
   }
   /* Get the aggregation level for this CQI */
   switch (aggrLvl)
   {
      case CM_LTE_AGGR_LVL1:
        pdcchCnt = 6;
        numCce = 1;
        break;
      case CM_LTE_AGGR_LVL2:
        pdcchCnt = 6;
        numCce = 2;
        break;
      case CM_LTE_AGGR_LVL4:
        pdcchCnt = 2;
        numCce = 4;
        break;
      case CM_LTE_AGGR_LVL8:
        pdcchCnt = 2;
        numCce = 8;
        break;
      default:
        RETVALUE(NULLP);
   }

   /* Locate a PDCCH for the known aggregation level */
   for (i = 0; i < pdcchCnt; i++)
   {
      /* Wrap around if y+i exceeds limit, 36.213 9.1.1*/
      loc = (y+i) % (pdcchInfo->nCce / (1 << (aggrLvl - 1)));

      if (rgSCHUtlPdcchAvail(cell, pdcchInfo, loc, aggrLvl, &pdcch) == TRUE)
      {
         /* SR_RACH_STATS : Reset isTBMsg4 */
         pdcch->dci.u.format1aInfo.t.pdschInfo.isTBMsg4= FALSE;         
         pdcch->dci.u.format0Info.isSrGrant = FALSE;
#ifdef LTEMAC_SPS
         pdcch->isSpsRnti = FALSE;
#endif
         /* Increment the CCE used counter in the current subframe */
         subFrm->cceCnt += numCce;

         RETVALUE(pdcch);
      }
   }

   /* PDCCH Allocation Failed, Mark cceFailure flag as TRUE */
   subFrm->isCceFailure = TRUE;

   RLOG_ARG1(L_DEBUG,DBG_CELLID,cell->cellId,
         "PDCCH ERR: NO PDDCH AVAIL IN UE SEARCH SPACE :aggr(%u)",
         aggrLvl);
   RETVALUE(NULLP);
}

#ifdef RGR_V1
/**
 * @brief This function implements BW allocation for CCCH SDU
 *
 * @details
 *
 *     Function: rgSCHCmnCcchSduDedAlloc
 *     Purpose:  Downlink bandwidth Allocation for CCCH SDU.
 *
 *     Invoked by: Scheduler
 *
 *  @param[in]  RgSchCellCb*     cell
 *  @param[out] RgSchUeCb        *ueCb
 *  @return S16
 *
 **/
#ifdef ANSI
PRIVATE S16 rgSCHCmnCcchSduDedAlloc
(
RgSchCellCb      *cell,
RgSchUeCb        *ueCb
)
#else
PRIVATE S16 rgSCHCmnCcchSduDedAlloc(cell, ueCb)
RgSchCellCb      *cell;
RgSchUeCb        *ueCb;
#endif
{
   U32                  effBo;
   RgSchDlRbAlloc       *rbAllocinfo = &ueCb->dl.dlAllocCb;
   RgSchCmnDlCell       *cellDl = RG_SCH_CMN_GET_DL_CELL(cell);
   U8                   iTbs;
   U8                   numRb;
#ifdef LTE_TDD
   U8                   cfi     = cellDl->currCfi;
#endif

   TRC2(rgSCHCmnCcchSduDedAlloc);

   effBo  =   ueCb->dlCcchInfo.bo + RGSCH_CCCH_SDU_HDRSIZE;

#ifndef LTEMAC_SPS
   rgSCHCmnClcRbAlloc(cell, effBo, cellDl->ccchCqi, &rbAllocinfo->rbsReq, \
                      &rbAllocinfo->tbInfo[0].bytesReq,
                      &rbAllocinfo->tbInfo[0].imcs, rbAllocinfo->dlSf);
#else /* LTEMAC_SPS */
   rgSCHCmnClcRbAlloc(cell, effBo, cellDl->ccchCqi, &rbAllocinfo->rbsReq, \
                      &rbAllocinfo->tbInfo[0].bytesReq,\
                      &rbAllocinfo->tbInfo[0].imcs, &iTbs, FALSE, 
                      rbAllocinfo->dlSf);
#endif /* LTEMAC_SPS */

   iTbs = 0;
   /* Cannot exceed the total number of RBs in the cell */
   if ((S16)rbAllocinfo->rbsReq > ((S16)(rbAllocinfo->dlSf->bw - \
                                   rbAllocinfo->dlSf->bwAssigned)))
   {
      /* Check if atleast one allocation was possible.
         This may be the case where the Bw is very less and
         with the configured CCCH CQI, CCCH SDU exceeds the min Bw */
      if (rbAllocinfo->dlSf->bwAssigned == 0)
      {
         numRb   = rbAllocinfo->dlSf->bw;
         RG_SCH_CMN_DL_MCS_TO_TBS(rbAllocinfo->tbInfo[0].imcs, iTbs);
         while (rgTbSzTbl[0][++iTbs][numRb-1]/8 < effBo)
         {
            iTbs++;
         }
         rbAllocinfo->rbsReq = numRb;
         rbAllocinfo->tbInfo[0].bytesReq = rgTbSzTbl[0][iTbs][numRb-1]/8;
         /* DwPTS Scheduling Changes Start */
#ifdef LTE_TDD
         if(rbAllocinfo->dlSf->sfType == RG_SCH_SPL_SF_DATA)
         {
            rbAllocinfo->tbInfo[0].bytesReq =
               rgSCHCmnCalcDwPtsTbSz(cell, effBo, &numRb, &iTbs, 1,cfi);
         }
#endif
         /* DwPTS Scheduling Changes End */
         RG_SCH_CMN_DL_TBS_TO_MCS(iTbs, rbAllocinfo->tbInfo[0].imcs);
      }
      else
      {
         RETVALUE(RFAILED);
      }
   }

   /* Update the subframe Allocated BW field */
   rbAllocinfo->dlSf->bwAssigned = rbAllocinfo->dlSf->bwAssigned + \
                                   rbAllocinfo->rbsReq;
   rbAllocinfo->tbInfo[0].tbCb = &ueCb->dl.hqEnt->ccchSduProc->tbInfo[0];
   rbAllocinfo->rnti = ueCb->ueId;
   rbAllocinfo->tbInfo[0].noLyr = 1;

   RETVALUE(ROK);
}
#endif

/**
 * @brief This function implements BW allocation for MSG4
 *
 * @details
 *
 *     Function: rgSCHCmnMsg4DedAlloc
 *     Purpose:  Downlink bandwidth Allocation for MSG4.
 *
 *     Invoked by: Scheduler
 *
 *  @param[in]  RgSchCellCb*     cell
 *  @param[out] RgSchRaCb        *raCb
 *  @return S16
 *
 **/
#ifdef ANSI
PRIVATE S16 rgSCHCmnMsg4DedAlloc
(
RgSchCellCb      *cell,
RgSchRaCb        *raCb
)
#else
PRIVATE S16 rgSCHCmnMsg4DedAlloc(cell, raCb)
RgSchCellCb      *cell;
RgSchRaCb        *raCb;
#endif
{
   U32                  effBo;
   RgSchDlRbAlloc       *rbAllocinfo = &raCb->rbAllocInfo;
   U8                   iTbs;
   U8                   numRb;
#ifdef LTE_TDD
   RgSchCmnDlCell       *cellDl = RG_SCH_CMN_GET_DL_CELL(cell);
   U8                   cfi     = cellDl->currCfi;
#endif

   TRC2(rgSCHCmnMsg4DedAlloc);

   effBo  = raCb->dlCcchInfo.bo + RGSCH_MSG4_HDRSIZE + RGSCH_CONT_RESID_SIZE;

#ifndef LTEMAC_SPS
   rgSCHCmnClcRbAlloc(cell, effBo, raCb->ccchCqi, &rbAllocinfo->rbsReq, \
         &rbAllocinfo->tbInfo[0].bytesReq,\
         &rbAllocinfo->tbInfo[0].imcs, rbAllocinfo->dlSf);
#else /* LTEMAC_SPS */
   rgSCHCmnClcRbAlloc(cell, effBo, raCb->ccchCqi, &rbAllocinfo->rbsReq, \
                      &rbAllocinfo->tbInfo[0].bytesReq,\
                      &rbAllocinfo->tbInfo[0].imcs, &iTbs, FALSE,
                      rbAllocinfo->dlSf);
#endif /* LTEMAC_SPS */

   iTbs = 0;
   /* Cannot exceed the total number of RBs in the cell */
   if ((S16)rbAllocinfo->rbsReq > ((S16)(rbAllocinfo->dlSf->bw - \
               rbAllocinfo->dlSf->bwAssigned)))
   {
      /* Check if atleast one allocation was possible.
         This may be the case where the Bw is very less and
         with the configured CCCH CQI, CCCH SDU exceeds the min Bw */
      if (rbAllocinfo->dlSf->bwAssigned == 0)
      {
         numRb   = rbAllocinfo->dlSf->bw;
         RG_SCH_CMN_DL_MCS_TO_TBS(rbAllocinfo->tbInfo[0].imcs, iTbs);
         while (rgTbSzTbl[0][++iTbs][numRb-1]/8 < effBo)
         {
            iTbs++;
         }
         rbAllocinfo->rbsReq = numRb;
         rbAllocinfo->tbInfo[0].bytesReq = rgTbSzTbl[0][iTbs][numRb-1]/8;
         /* DwPTS Scheduling Changes Start */
#ifdef LTE_TDD
         if(rbAllocinfo->dlSf->sfType == RG_SCH_SPL_SF_DATA)
         {
            rbAllocinfo->tbInfo[0].bytesReq =
               rgSCHCmnCalcDwPtsTbSz(cell, effBo, &numRb, &iTbs, 1, cfi);
         }
#endif
         /* DwPTS Scheduling Changes End */
         RG_SCH_CMN_DL_TBS_TO_MCS(iTbs, rbAllocinfo->tbInfo[0].imcs);
      }
      else
      {
         RETVALUE(RFAILED);
      }
   }

   /* Update the subframe Allocated BW field */
   rbAllocinfo->dlSf->bwAssigned = rbAllocinfo->dlSf->bwAssigned + \
                                   rbAllocinfo->rbsReq;
   rbAllocinfo->rnti = raCb->tmpCrnti;
   rbAllocinfo->tbInfo[0].tbCb = &raCb->dlHqE->msg4Proc->tbInfo[0];
   rbAllocinfo->tbInfo[0].schdlngForTb = TRUE;
   rbAllocinfo->tbInfo[0].noLyr = 1;

   RETVALUE(ROK);
}

#ifdef LTE_TDD
/**
 * @brief This function implements scheduling for RA Response.
 *
 * @details
 *
 *     Function: rgSCHCmnDlRaRsp
 *     Purpose:  Downlink scheduling for RA responses.
 *
 *     Invoked by: Scheduler
 *
 *  @param[in]  RgSchCellCb*     cell
 *  @return  Void
 *
 **/
#ifdef ANSI
PRIVATE Void rgSCHCmnDlRaRsp
(
RgSchCellCb                *cell,
RgSchCmnDlRbAllocInfo      *allocInfo
)
#else
PRIVATE Void rgSCHCmnDlRaRsp(cell, allocInfo)
RgSchCellCb                *cell;
RgSchCmnDlRbAllocInfo      *allocInfo;
#endif
{
   CmLteTimingInfo      frm;
   CmLteTimingInfo      schFrm;
   RgSchDlSf            *subFrm;
   U16                  rarnti;
   U8                   i;
   U8                   noRaRnti=0;
   U8                   raIdx;
   RgSchTddRachRspLst   *rachRsp;
   U8                   ulDlCfgIdx = cell->ulDlCfgIdx;
   U8                   sfnIdx;
   U8                   subfrmIdx;
   U16                  rntiIdx=0;
   TRC2(rgSCHCmnDlRaRsp);

   frm   = cell->crntTime;
   RGSCH_INCR_SUB_FRAME(frm, RG_SCH_CMN_DL_DELTA);

   /* Compute the subframe for which allocation is being made        */
   /* essentially, we need pointer to the dl frame for this subframe */
   subFrm  = rgSCHUtlSubFrmGet(cell, frm);

   /* Get the RACH Response scheduling related information
    * for the subframe with RA index */
   raIdx = rgSchTddNumDlSubfrmTbl[ulDlCfgIdx][frm.subframe]-1;

   rachRsp = &cell->rachRspLst[raIdx];

   for(sfnIdx = 0; sfnIdx < rachRsp->numRadiofrms; sfnIdx++)
   {
      /* For all scheduled RACH Responses in SFNs */
      schFrm = frm;
      RG_SCH_CMN_DECR_FRAME(schFrm.sfn, rachRsp->rachRsp[sfnIdx].sfnOffset);
      /* For all scheduled RACH Responses in subframes */
      for(subfrmIdx = 0;
            subfrmIdx < rachRsp->rachRsp[sfnIdx].numSubfrms; subfrmIdx++)
      {
         schFrm.subframe = rachRsp->rachRsp[sfnIdx].subframe[subfrmIdx];
         /* compute the last RA RNTI used in the previous subframe */
         raIdx = (((schFrm.sfn % cell->raInfo.maxRaSize) * \
                  RGSCH_NUM_SUB_FRAMES * RGSCH_MAX_RA_RNTI_PER_SUBFRM) \
                                    + schFrm.subframe);

         /* For all RA RNTIs within a subframe */

         for(i=0; (i < RGSCH_MAX_RA_RNTI_PER_SUBFRM) && \
               (noRaRnti < RGSCH_MAX_TDD_RA_RSP_ALLOC); i++)
         {
            rarnti = (schFrm.subframe + RGSCH_NUM_SUB_FRAMES*i + 1);
            rntiIdx = (raIdx + RGSCH_NUM_SUB_FRAMES*i);

            if (cell->raInfo.raReqLst[rntiIdx].first != NULLP)
            {
               /* compute the next RA RNTI */
               if (rgSCHCmnRaRspAlloc(cell, subFrm, rntiIdx,
                        rarnti, noRaRnti, allocInfo) != ROK)
               {
                  /* The resources are exhausted */
                  break;
               }
               noRaRnti++;
            }
         }
         noRaRnti=0;
      }
   }

   RETVOID;
}
#else
/**
 * @brief This function implements scheduling for RA Response.
 *
 * @details
 *
 *     Function: rgSCHCmnDlRaRsp
 *     Purpose:  Downlink scheduling for RA responses.
 *
 *     Invoked by: Scheduler
 *
 *  @param[in]  RgSchCellCb*          cell
 *  @param[out] RgSchCmnDlRbAllocInfo *allocInfo
 *  @return  Void
 *
 **/
#ifdef ANSI
PRIVATE Void rgSCHCmnDlRaRsp
(
RgSchCellCb                *cell,
RgSchCmnDlRbAllocInfo      *allocInfo
)
#else
PRIVATE Void rgSCHCmnDlRaRsp(cell, allocInfo)
RgSchCellCb                *cell;
RgSchCmnDlRbAllocInfo      *allocInfo;
#endif
{
   CmLteTimingInfo      frm;
   CmLteTimingInfo      winStartFrm;
   RgSchDlSf            *subFrm;
   U8                   winStartIdx;
   U8                   winGap;
   U8                   rarnti;
   U8                   raIdx;
   RgSchCmnCell         *sched;
   U8                   i,noRaRnti=0;
   TRC2(rgSCHCmnDlRaRsp);

   frm   = cell->crntTime;
   RGSCH_INCR_SUB_FRAME(frm, RG_SCH_CMN_DL_DELTA);

   /* Compute the subframe for which allocation is being made        */
   /* essentially, we need pointer to the dl frame for this subframe */
   subFrm  = rgSCHUtlSubFrmGet(cell, frm);
   sched   = RG_SCH_CMN_GET_CELL(cell);

   /* ccpu00132523 - Window Start calculated by considering RAR window size, 
    * RAR Wait period, Subframes occuppied for respective preamble format*/
   winGap = (sched->dl.numRaSubFrms-1) + (cell->rachCfg.raWinSize-1) 
             +RGSCH_RARSP_WAIT_PERIOD;

   /* Window starting occassion is retrieved using the gap and tried to 
    * fit to the size of raReqLst array*/ 
   RGSCHDECRFRMCRNTTIME(frm, winStartFrm, winGap);

   winStartIdx = (winStartFrm.sfn & 1) * RGSCH_MAX_RA_RNTI+ winStartFrm.subframe;

   for(i = 0; ((i < cell->rachCfg.raWinSize) && (noRaRnti < RG_SCH_CMN_MAX_CMN_PDCCH)); i++)
   {
      raIdx = (winStartIdx + i) % RGSCH_RAREQ_ARRAY_SIZE;

      if (cell->raInfo.raReqLst[raIdx].first != NULLP)
      {
         allocInfo->raRspAlloc[noRaRnti].biEstmt = \
                         (!i * RGSCH_ONE_BIHDR_SIZE);
         rarnti = raIdx % RGSCH_MAX_RA_RNTI+ 1;
         if (rgSCHCmnRaRspAlloc(cell, subFrm, raIdx,
                                 rarnti, noRaRnti, allocInfo) != ROK)
         {
            /* The resources are exhausted */
            break;
         }
         /* ccpu00132523- If all the RAP ids are not scheduled then need not 
          * proceed for next RA RNTIs*/
         if(allocInfo->raRspAlloc[noRaRnti].numRapids < cell->raInfo.raReqLst[raIdx].count)
         {
            break;
         }
         noRaRnti++; /* Max of RG_SCH_CMN_MAX_CMN_PDCCH RARNTIs
                        for response allocation */
      }
   }
   RETVOID;
}
#endif


/**
 * @brief This function allocates the resources for an RARNTI.
 *
 * @details
 *
 *     Function: rgSCHCmnRaRspAlloc
 *     Purpose:  Allocate resources to a RARNTI.
 *               0. Allocate PDCCH for sending the response.
 *               1. Locate the number of RA requests pending for the RARNTI.
 *               2. Compute the size of data to be built.
 *               3. Using common channel CQI, compute the number of RBs.
 *
 *     Invoked by: Scheduler
 *
 *  @param[in]  RgSchCellCb             *cell,
 *  @param[in]  RgSchDlSf               *subFrm,
 *  @param[in]  U16                     rarnti,
 *  @param[in]  U8                      noRaRnti
 *  @param[out] RgSchCmnDlRbAllocInfo   *allocInfo
 *  @return  S16
 *
 **/
#ifdef ANSI
PRIVATE S16 rgSCHCmnRaRspAlloc
(
RgSchCellCb             *cell,
RgSchDlSf               *subFrm,
U16                     raIndex,
U16                     rarnti,
U8                      noRaRnti,
RgSchCmnDlRbAllocInfo   *allocInfo
)
#else
PRIVATE S16 rgSCHCmnRaRspAlloc(cell,subFrm,raIndex,rarnti,noRaRnti,allocInfo)
RgSchCellCb             *cell;
RgSchDlSf               *subFrm;
U16                     raIndex;
U16                     rarnti;
U8                      noRaRnti;
RgSchCmnDlRbAllocInfo   *allocInfo;
#endif
{
   RgSchCmnDlCell       *cellDl = RG_SCH_CMN_GET_DL_CELL(cell);
   RgSchCmnUlCell       *cellUl = RG_SCH_CMN_GET_UL_CELL(cell);
   U16                  noBytes;
   U32                  rb = 0;
   U32                  tbs;
   /*ccpu00116700,ccpu00116708- Corrected the wrong type for mcs*/
   U8                   mcs;
   CmLListCp            *reqLst;
   /* RACH handling related changes */
   Bool                 isAlloc = FALSE;
   static U8            schdNumRapid = 0;
   U8                   remNumRapid = 0;
   U8                   nPrb = 0;
   S32                  allwdTbSz = 0;
#ifdef LTE_TDD   
   U16                  lostRe;  
   U8                   cfi = cellDl->currCfi;  
#endif   

   TRC2(rgSCHCmnRaRspAlloc);
#ifndef RGR_V1
   UNUSED(cellUl);
#endif

   /* ccpu00132523: Resetting the schdRap Id count in every scheduling subframe*/
   if(noRaRnti == 0)
   {
      schdNumRapid = 0;
   }


   if (subFrm->bw == subFrm->bwAssigned)
   {
      RLOG_ARG1(L_ERROR,DBG_CELLID,cell->cellId,
         "bw == bwAssigned RARNTI:%d",rarnti);
      RETVALUE(RFAILED);
   }

   reqLst = &cell->raInfo.raReqLst[raIndex];
   if (reqLst->count == 0)
   {
      RLOG_ARG1(L_ERROR,DBG_CELLID,cell->cellId,
         "reqLst Count=0 RARNTI:%d",rarnti);
      RETVALUE(RFAILED);
   }
   /* ccpu00132523: Deleting False dedicated preambles, if any*/
   else
   {
      RgSchRaReqInfo        *raReq;
      RgSchCmnCell          *cellSch = (RgSchCmnCell *)(cell->sc.sch);
      RgSchUeCb             *ue;
      CmLList           *node;

      node = reqLst->first;
      while(node)
      {
         raReq = (RgSchRaReqInfo *)(node->node);
         node  = raReq->raReqLstEnt.next;
         /* ccpu00133504 - checking if there is no earlier UE context */ 
         if(RGSCH_IS_DEDPRM(cell, raReq->raReq.rapId) && (raReq->ue == NULLP))
         { 
            /* Finding UE in handOver List */
            if ((raReq->raReq.rapId < cellSch->rachCfg.dedPrmStart) ||
                (raReq->raReq.rapId > cellSch->rachCfg.dedPrmStart +
                  cellSch->rachCfg.numDedPrm - 1))
            {
               /* This ded Preamble corresponds to handover */
               ue = rgSCHCmnGetHoUe(cell, raReq);
            }
            else/* Finding UE from PDCCH Order Mappings */
            {
               /* Get the UE which has transmitted this RaReq */
               ue = rgSCHCmnGetPoUe(cell, raReq);
            }
         
            /* ccpu00132523: Removing False detected dedicated preamble from 
             * RA list such that only valid RAPIDs will be scheduled */   
            if (ue == NULLP)
            {
               cmLListDelFrm(reqLst, &raReq->raReqLstEnt);
               /* ccpu00133557- Memory leak fixed. RaReq deleted from the list but not freed.
                * Hence adding freeing here*/
               rgSCHUtlFreeSBuf(cell->instIdx, (Data **)&raReq,
                     sizeof(RgSchRaReqInfo));
               
               /* ccpu00133504 - Decrementing Ded Preamble counter for false
                * detected preambles */ 
               /* SR_RACH_STATS : DED PREAMB*/
               rgNumDedPream--;
#ifdef LTE_L2_MEAS
               cell->raPrmbs.dedPream--;
#endif

            }
            else
            {
               raReq->ue = ue;
            }
         }
         /* ccpu00132523: Breaking out from loop because non-dedicated preamble i
          * will be iterated only when dedicated preambles are completed. */
         else
         {
            break;
         }
      }
   }

   remNumRapid = reqLst->count;

#ifdef RGR_V1
   /* Limit number of rach rsps to maxMsg3PerUlsf */
   if ( schdNumRapid+remNumRapid > cellUl->maxMsg3PerUlSf )
   {
      remNumRapid = cellUl->maxMsg3PerUlSf-schdNumRapid;
   }
#endif
 
   while (remNumRapid)
   {
      /* Try allocating for as many RAPIDs as possible */
      /* BI sub-header size to the tbSize requirement */
      noBytes  = RGSCH_GET_RAR_BYTES(remNumRapid) +\
                 allocInfo->raRspAlloc[noRaRnti].biEstmt;
      if ((allwdTbSz = rgSCHUtlGetAllwdCchTbSz(noBytes*8, &nPrb, &mcs)) == -1)
      {
         remNumRapid--;
         continue;
      }

      /* rgSCHCmnClcRbAllocForFxdTb(cell, allwdTbSz/8, cellDl->ccchCqi, &rb);*/
      if(cellDl->bitsPerRb==0)
      {
         while ((rgTbSzTbl[0][0][rb]) <(U32) allwdTbSz)
         {
            rb++;
         }
         rb = rb+1;
      }
      else
      {
         rb = RGSCH_CEIL(allwdTbSz, cellDl->bitsPerRb);
      }
      /* DwPTS Scheduling Changes Start */      
#ifdef LTE_TDD      
      if (subFrm->sfType == RG_SCH_SPL_SF_DATA)
      {
         RGSCH_GET_SPS_SF_CFI(cell->bwCfg.dlTotalBw, cfi);

         /* Calculate the less RE's because of DwPTS */
         lostRe = rb * (cellDl->noResPerRb[cfi] - 
                                  cellDl->numReDwPts[cfi]);
          
         /* Increase number of RBs in Spl SF to compensate for lost REs */
         rb += RGSCH_CEIL(lostRe, cellDl->numReDwPts[cfi]);
      }
#endif      
      /* DwPTS Scheduling Changes End */

      /*ccpu00115595- end*/
      if (rb > subFrm->bw - subFrm->bwAssigned)
      {
         remNumRapid--;
         continue;
      }
      /* Allocation succeeded for 'remNumRapid' */
      isAlloc = TRUE;
      tbs = allwdTbSz/8;
      break;
   }
   if (!isAlloc)
   {
      RLOG_ARG0(L_INFO,DBG_CELLID,cell->cellId,"BW alloc Failed");
      RETVALUE(RFAILED);
   }

   subFrm->bwAssigned = subFrm->bwAssigned + rb;

   /* Fill AllocInfo structure */
   allocInfo->raRspAlloc[noRaRnti].rnti = rarnti;
   allocInfo->raRspAlloc[noRaRnti].tbInfo[0].bytesReq = tbs;
   allocInfo->raRspAlloc[noRaRnti].rbsReq = rb;
   allocInfo->raRspAlloc[noRaRnti].dlSf = subFrm;
   allocInfo->raRspAlloc[noRaRnti].tbInfo[0].imcs = mcs;
   allocInfo->raRspAlloc[noRaRnti].raIndex = raIndex;
   /* RACH changes for multiple RAPID handling */
   allocInfo->raRspAlloc[noRaRnti].numRapids = remNumRapid;
   allocInfo->raRspAlloc[noRaRnti].nPrb = nPrb;
   allocInfo->raRspAlloc[noRaRnti].tbInfo[0].noLyr = 1;

   schdNumRapid += remNumRapid; 
   RETVALUE(ROK);
}

/***********************************************************
 *
 *     Func : rgSCHCmnUlAllocFillRbInfo
 *
 *     Desc : Fills the start RB and the number of RBs for
 *            uplink allocation.
 *
 *     Ret  : void
 *
 *     Notes:
 *
 *     File :
 *
 **********************************************************/
#ifdef ANSI
PUBLIC Void rgSCHCmnUlAllocFillRbInfo
(
RgSchCellCb   *cell,
RgSchUlSf      *sf,
RgSchUlAlloc  *alloc
)
#else
PUBLIC Void rgSCHCmnUlAllocFillRbInfo(cell, sf, alloc)
RgSchCellCb    *cell;
RgSchUlSf      *sf;
RgSchUlAlloc   *alloc;
#endif
{
    RgSchCmnUlCell *cellUl = RG_SCH_CMN_GET_UL_CELL(cell);
    RgSchCmnDlCell *cellDl = RG_SCH_CMN_GET_DL_CELL(cell);
    U8             cfi = cellDl->currCfi;


   TRC2(rgSCHCmnUlAllocFillRbInfo);

   alloc->grnt.rbStart = (alloc->sbStart * cellUl->sbSize) + 
                                    cell->dynCfiCb.bwInfo[cfi].startRb;

   /* Num RBs = numSbAllocated * sbSize - less RBs in the last SB */
   alloc->grnt.numRb = (alloc->numSb * cellUl->sbSize);

   RETVOID;
}

/**
 * @brief Grant request for Msg3.
 *
 * @details
 *
 *     Function : rgSCHCmnMsg3GrntReq
 *
 *     This is invoked by downlink scheduler to request allocation
 *     for msg3.
 *     Steps:
 *     - Attempt to allocate msg3 in the current msg3 subframe
 *       Allocation attempt based on whether preamble is from group A
 *       and the value of MESSAGE_SIZE_GROUP_A
 *     - Link allocation with passed RNTI and msg3 HARQ process
 *     - Set the HARQ process ID (*hqProcIdRef)
 *
 *  @param[in]  RgSchCellCb       *cell
 *  @param[in]  CmLteRnti         rnti
 *  @param[in]  Bool              preamGrpA
 *  @param[in]  RgSchUlHqProcCb   *hqProc
 *  @param[out] RgSchUlAlloc      **ulAllocRef
 *  @param[out] U8                *hqProcIdRef
 *  @return  Void
 **/
#ifdef ANSI
PRIVATE Void rgSCHCmnMsg3GrntReq
(
RgSchCellCb     *cell,
CmLteRnti       rnti,
Bool            preamGrpA,
RgSchUlHqProcCb *hqProc,
RgSchUlAlloc    **ulAllocRef,
U8              *hqProcIdRef
)
#else
PRIVATE Void rgSCHCmnMsg3GrntReq(cell, rnti, preamGrpA, hqProc,
                                 ulAllocRef, hqProcIdRef)
RgSchCellCb     *cell;
CmLteRnti       rnti;
Bool            preamGrpA;
RgSchUlHqProcCb *hqProc;
RgSchUlAlloc    **ulAllocRef;
U8              *hqProcIdRef;
#endif
{
   RgSchCmnUlCell  *cellUl = RG_SCH_CMN_GET_UL_CELL(cell);
   RgSchUlSf       *sf = &cellUl->ulSfArr[cellUl->msg3SchdIdx];
   RgSchUlHole     *hole;
   RgSchUlAlloc    *alloc;
   U8              iMcs;
   U8              numSb;

   TRC2(rgSCHCmnMsg3GrntReq);

   *ulAllocRef = NULLP;

   /* Fix: ccpu00120610 Use remAllocs from subframe during msg3 allocation */
   if (*sf->allocCountRef >= cellUl->maxAllocPerUlSf)
   {
      RETVOID;
   }
   if (preamGrpA == FALSE)
   {
      numSb = cellUl->ra.prmblBNumSb;
      iMcs  = cellUl->ra.prmblBIMcs;
   }
   else
   {
      numSb = cellUl->ra.prmblANumSb;
      iMcs  = cellUl->ra.prmblAIMcs;
   }

   if ((hole = rgSCHUtlUlHoleFirst(sf)) != NULLP)
   {
      if(*sf->allocCountRef == 0)
      {
         RgSchCmnDlCell  *cellDl = RG_SCH_CMN_GET_DL_CELL(cell);
         /* Reinitialize the hole */
         if (sf->holeDb->count == 1 && (hole->start == 0)) /* Sanity check of holeDb */
         {
            hole->num = cell->dynCfiCb.bwInfo[cellDl->currCfi].numSb;
            /* Re-Initialize available subbands because of CFI change*/
            hole->num = cell->dynCfiCb.bwInfo[cellDl->currCfi].numSb;   
         }
         else
         {
            RLOG_ARG1(L_ERROR,DBG_CELLID,cell->cellId,
                     "Error! holeDb sanity check failed RNTI:%d",rnti);
         } 
      }
      if (numSb <= hole->num)
      {
         U8 iTbs;
         alloc                = rgSCHUtlUlAllocGetHole(sf, numSb, hole);
         rgSCHCmnUlAllocFillRbInfo(cell, sf, alloc);
         alloc->grnt.iMcs     = iMcs;
         alloc->grnt.iMcsCrnt = iMcs;
         iTbs                 = rgSCHCmnUlGetITbsFrmIMcs(iMcs);
         RGSCH_ARRAY_BOUND_CHECK(cell->instIdx, rgTbSzTbl[0], iTbs); 
         /* To include the length and ModOrder in DataRecp Req.*/
         alloc->grnt.datSz = rgTbSzTbl[0][iTbs][alloc->grnt.numRb-1] / 8;
         RG_SCH_UL_MCS_TO_MODODR(iMcs, alloc->grnt.modOdr);
         /* RACHO : setting nDmrs to 0 and UlDelaybit to 0*/
         alloc->grnt.nDmrs    = 0;
         alloc->grnt.hop      = 0;
         alloc->grnt.delayBit = 0;
         alloc->grnt.isRtx    = FALSE;
         *ulAllocRef          = alloc;
         *hqProcIdRef         = (cellUl->msg3SchdHqProcIdx);
         hqProc->procId       = *hqProcIdRef;
         hqProc->ulSfIdx      = (cellUl->msg3SchdIdx);
         alloc->rnti          = rnti;
         alloc->ue            = NULLP;
         alloc->pdcch         = FALSE;
         alloc->forMsg3       = TRUE;
         alloc->hqProc        = hqProc;
         rgSCHUhmNewTx(hqProc, (U8)(cell->rachCfg.maxMsg3Tx - 1), alloc);
         RLOG_ARG4(L_DEBUG,DBG_CELLID,cell->cellId,
               "RNTI:%d MSG3 ALLOC proc(%p)procId(%d)schdIdx(%d)",
               alloc->rnti,
               ((U32)alloc->hqProc),
               alloc->hqProc->procId,
               alloc->hqProc->ulSfIdx);
         RLOG_ARG2(L_DEBUG,DBG_CELLID,cell->cellId,
               "alloc(%p)maxMsg3Tx(%d)",
               ((U32)alloc),
               cell->rachCfg.maxMsg3Tx);
      }
   }

   RETVOID;
}


/**
 * @brief This function determines the allocation limits and
 *        parameters that aid in DL scheduling.
 *
 * @details
 *
 *     Function: rgSCHCmnDlSetUeAllocLmt
 *     Purpose:  This function determines the Maximum RBs
 *               a UE is eligible to get based on softbuffer
 *               limitation and cell->>>maxDlBwPerUe. The Codeword
 *               specific parameters like iTbs, eff and noLyrs
 *               are also set in this function. This function
 *               is called while UE configuration and UeDlCqiInd.
 *
 *     Invoked by: Scheduler
 *
 *  @param[in]  RgSchCellCb   *cellCb
 *  @param[in]  RgSchCmnDlUe  *ueDl
 *  @return  Void
 *
 **/
#ifdef ANSI
PRIVATE Void rgSCHCmnDlSetUeAllocLmt
(
RgSchCellCb   *cell,
RgSchCmnDlUe  *ueDl
)
#else
PRIVATE Void rgSCHCmnDlSetUeAllocLmt(cell, ueDl)
RgSchCellCb   *cell;
RgSchCmnDlUe  *ueDl;
#endif
{
   U8            modOrder;
   U32           maxRb;
   RgSchCmnCell  *cellSch = RG_SCH_CMN_GET_CELL(cell);
   U8            cfi = cellSch->dl.currCfi;

   TRC2(rgSCHCmnDlSetUeAllocLmt);

  
   /* ITbs for CW0 for 1 Layer Tx */
   ueDl->mimoInfo.cwInfo[0].iTbs[0] = (*(RgSchCmnCqiToTbs *)(cellSch->dl.cqiToTbsTbl[0][cfi]))\
                           [ueDl->mimoInfo.cwInfo[0].cqi];
   /* ITbs for CW0 for 2 Layer Tx */
   ueDl->mimoInfo.cwInfo[0].iTbs[1] = (*(RgSchCmnCqiToTbs *)(cellSch->dl.cqiToTbsTbl[1][cfi]))\
                           [ueDl->mimoInfo.cwInfo[0].cqi];
   /* Eff for CW0 for 1 Layer Tx */
   ueDl->mimoInfo.cwInfo[0].eff[0] = (*(RgSchCmnTbSzEff *)(cellSch->dl.cqiToEffTbl[0][cfi]))\
                           [ueDl->mimoInfo.cwInfo[0].iTbs[0]];
   /* Eff for CW0 for 2 Layer Tx */
   ueDl->mimoInfo.cwInfo[0].eff[1] = (*(RgSchCmnTbSzEff *)(cellSch->dl.cqiToEffTbl[1][cfi]))\
                           [ueDl->mimoInfo.cwInfo[0].iTbs[1]];

   /* ITbs for CW1 for 1 Layer Tx */
   ueDl->mimoInfo.cwInfo[1].iTbs[0] = (*(RgSchCmnCqiToTbs *)(cellSch->dl.cqiToTbsTbl[0][cfi]))\
                           [ueDl->mimoInfo.cwInfo[1].cqi];
   /* ITbs for CW1 for 2 Layer Tx */
   ueDl->mimoInfo.cwInfo[1].iTbs[1] = (*(RgSchCmnCqiToTbs *)(cellSch->dl.cqiToTbsTbl[1][cfi]))\
                           [ueDl->mimoInfo.cwInfo[1].cqi];
   /* Eff for CW1 for 1 Layer Tx */
   ueDl->mimoInfo.cwInfo[1].eff[0] = (*(RgSchCmnTbSzEff *)(cellSch->dl.cqiToEffTbl[0][cfi]))\
                           [ueDl->mimoInfo.cwInfo[1].iTbs[0]];
   /* Eff for CW1 for 2 Layer Tx */
   ueDl->mimoInfo.cwInfo[1].eff[1] = (*(RgSchCmnTbSzEff *)(cellSch->dl.cqiToEffTbl[1][cfi]))\
                           [ueDl->mimoInfo.cwInfo[1].iTbs[1]];
//#ifdef DL_LA 
  // ueDl->laCb.cqiBasediTbs =  ueDl->mimoInfo.cwInfo[0].iTbs[0] * 100;
//#endif
   /* Assigning noLyrs to each CW assuming optimal Spatial multiplexing
    * capability */
   (ueDl->mimoInfo.ri/2 == 0)? (ueDl->mimoInfo.cwInfo[0].noLyr = 1) : \
              (ueDl->mimoInfo.cwInfo[0].noLyr = ueDl->mimoInfo.ri/2);
   ueDl->mimoInfo.cwInfo[1].noLyr = ueDl->mimoInfo.ri - ueDl->mimoInfo.cwInfo[0].noLyr;
   /* rg002.101:ccpu00102106: correcting DL harq softbuffer limitation logic.
    * The maxTbSz is the maximum number of PHY bits a harq process can
    * hold. Hence we limit our allocation per harq process based on this.
    * Earlier implementation we misinterpreted the maxTbSz to be per UE
    * per TTI, but in fact it is per Harq per TTI. */
   /* rg002.101:ccpu00102106: cannot exceed the harq Tb Size
    * and harq Soft Bits limit.*/

   /* Considering iTbs corresponding to 2 layer transmission for
    * codeword0(approximation) and the maxLayers supported by
    * this UE at this point of time. */
   RG_SCH_CMN_TBS_TO_MODODR(ueDl->mimoInfo.cwInfo[0].iTbs[1], modOrder);
   /* Bits/modOrder gives #REs, #REs/noResPerRb gives #RBs */
   /* rg001.301 -MOD- [ccpu00119213] : avoiding wraparound */
   maxRb = ((ueDl->maxSbSz)/(cellSch->dl.noResPerRb[cfi] * modOrder *\
                   ueDl->mimoInfo.ri));
   if (cellSch->dl.isDlFreqSel)
   {
      /* Rounding off to left nearest multiple of RBG size */
      maxRb -= maxRb % cell->rbgSize;
   }
   ueDl->maxRb = RGSCH_MIN(maxRb, cellSch->dl.maxDlBwPerUe);
   if (cellSch->dl.isDlFreqSel)
   {
      /* Rounding off to right nearest multiple of RBG size */
      if (ueDl->maxRb % cell->rbgSize)
      {
         ueDl->maxRb += (cell->rbgSize - 
                         (ueDl->maxRb % cell->rbgSize));
      }
   }

   /* Set the index of the cwInfo, which is better in terms of
    * efficiency. If RI<2, only 1 CW, hence btrCwIdx shall be 0 */
   if (ueDl->mimoInfo.ri < 2)
   {
      ueDl->mimoInfo.btrCwIdx = 0;
   }
   else
   {
      if (ueDl->mimoInfo.cwInfo[0].eff[ueDl->mimoInfo.cwInfo[0].noLyr-1] <\
          ueDl->mimoInfo.cwInfo[1].eff[ueDl->mimoInfo.cwInfo[1].noLyr-1])
      {
         ueDl->mimoInfo.btrCwIdx = 1;
      }
      else
      {
         ueDl->mimoInfo.btrCwIdx = 0;
      }
   }

   RETVOID;
   }

#ifdef DL_LA

/**
 * @brief This function updates TX Scheme.
 *
 * @details
 *
 *     Function: rgSCHCheckAndSetTxScheme 
 *     Purpose:  This function determines the Maximum RBs
 *               a UE is eligible to get based on softbuffer
 *               limitation and cell->>>maxDlBwPerUe. The Codeword
 *               specific parameters like iTbs, eff and noLyrs
 *               are also set in this function. This function
 *               is called while UE configuration and UeDlCqiInd.
 *
 *     Invoked by: Scheduler
 *
 *  @param[in]  RgSchCellCb   *cell
 *  @param[in]  RgSchUeCb     *ue
 *  @return  Void
 *
 **/
#ifdef ANSI
PRIVATE Void rgSCHCheckAndSetTxScheme 
(
RgSchCellCb   *cell,
RgSchUeCb     *ue
)
#else
PRIVATE Void rgSCHCheckAndSetTxScheme(cell, ue)
RgSchCellCb   *cell;
RgSchUeCb     *ue;
#endif
{
   RgSchCmnCell  *cellSch = RG_SCH_CMN_GET_CELL(cell);
   RgSchCmnDlUe  *ueDl =  RG_SCH_CMN_GET_DL_UE(ue);
   U8            cfi = cellSch->dl.currCfi;
   U8            maxiTbs;
   U8            cqiBasediTbs;
   U8            actualiTbs;

   TRC2(rgSCHCheckAndSetTxScheme);

   maxiTbs      = (*(RgSchCmnCqiToTbs*)(cellSch->dl.cqiToTbsTbl[0][cfi]))\
                [RG_SCH_CMN_MAX_CQI - 1];
   cqiBasediTbs = (ueDl->laCb[0].cqiBasediTbs)/100;
   actualiTbs   = ueDl->mimoInfo.cwInfo[0].iTbs[0];

   if((actualiTbs < RG_SCH_TXSCHEME_CHNG_ITBS_FACTOR) && (cqiBasediTbs >
     actualiTbs) && ((cqiBasediTbs - actualiTbs) > RG_SCH_TXSCHEME_CHNG_THRSHD)) 
   {
      RG_SCH_CMN_SET_FORCE_TD(ue, RG_SCH_CMN_TD_TXSCHEME_CHNG);
   }
   
   if(actualiTbs >= maxiTbs)
   {
      RG_SCH_CMN_UNSET_FORCE_TD(ue, RG_SCH_CMN_TD_TXSCHEME_CHNG);
   }

   RETVOID;
}

/**
 * @brief This function determines the allocation limits and
 *        parameters that aid in DL scheduling.
 *
 * @details
 *
 *     Function: rgSCHCmnDlSetUeAllocLmtLa
 *     Purpose:  This function determines the Maximum RBs
 *               a UE is eligible to get based on softbuffer
 *               limitation and cell->>>maxDlBwPerUe. The Codeword
 *               specific parameters like iTbs, eff and noLyrs
 *               are also set in this function. This function
 *               is called while UE configuration and UeDlCqiInd.
 *
 *     Invoked by: Scheduler
 *
 *  @param[in]  RgSchCellCb   *cell
 *  @param[in]  RgSchUeCb     *ue
 *  @return  Void
 *
 **/
#ifdef ANSI
PRIVATE Void rgSCHCmnDlSetUeAllocLmtLa
(
RgSchCellCb   *cell,
RgSchUeCb     *ue
)
#else
PRIVATE Void rgSCHCmnDlSetUeAllocLmtLa(cell, ueDl)
RgSchCellCb   *cell;
RgSchUeCb     *ue;
#endif
{
   U8            modOrder;
   U32           maxRb;
   U8            reportediTbs;
   RgSchCmnCell  *cellSch = RG_SCH_CMN_GET_CELL(cell);
   RgSchCmnDlUe  *ueDl =  RG_SCH_CMN_GET_DL_UE(ue);
   U8            cfi = cellSch->dl.currCfi;
   U8            maxiTbs;
   U8            cwIdx; 

   TRC2(rgSCHCmnDlSetUeAllocLmtLa);

   maxiTbs      = (*(RgSchCmnCqiToTbs *)(cellSch->dl.cqiToTbsTbl[0][cfi]))[RG_SCH_CMN_MAX_CQI - 1];
   if(ueDl->cqiFlag == TRUE)
   {
      /* Calcluating the reported iTbs for code word 0 */
      reportediTbs = (*(RgSchCmnCqiToTbs *)(cellSch->dl.cqiToTbsTbl[0][cfi]))\
                        [ueDl->mimoInfo.cwInfo[0].cqi];

      /* Invoke function for TM mode check & chnage if Auto config 
       * mode is on */
      if(TRUE == cell->isAutoCfgModeEnb)
      {
         rgSchCheckAndTriggerModeChange(cell, ue, reportediTbs, 
               ueDl->mimoInfo.cwInfo[0].iTbs[0], maxiTbs);
      }

      for(cwIdx=0; cwIdx < RG_SCH_CMN_MAX_CW_PER_UE; cwIdx++)
      {
         U8 iTbsNew;
         reportediTbs = (*(RgSchCmnCqiToTbs *)(cellSch->dl.cqiToTbsTbl[0][cfi]))\
                        [ueDl->mimoInfo.cwInfo[cwIdx].cqi];

         iTbsNew =  reportediTbs;

         if(!ueDl->laCb[cwIdx].notFirstCqi)
         {
            /* This is the first CQI report from UE */
            ueDl->laCb[cwIdx].cqiBasediTbs = (iTbsNew * 100);
            ueDl->laCb[cwIdx].notFirstCqi  =  TRUE;
         }
         else if ((RG_ITBS_DIFF(reportediTbs, ueDl->mimoInfo.cwInfo[cwIdx].iTbs[0])) > 5)
         {
            /* Ignore this iTBS report and mark that last iTBS report was */
            /* ignored so that subsequently we reset the LA algorithm     */
            ueDl->laCb[cwIdx].lastiTbsIgnored = TRUE;
            ueDl->laCb[cwIdx].numLastiTbsIgnored++;
            if( ueDl->laCb[cwIdx].numLastiTbsIgnored > 10)
            {
               /* CQI reported by UE is not catching up. Reset the LA algorithm */
               ueDl->laCb[cwIdx].cqiBasediTbs = (iTbsNew * 100);
               ueDl->laCb[cwIdx].deltaiTbs = 0;
               ueDl->laCb[cwIdx].lastiTbsIgnored = FALSE;
               ueDl->laCb[cwIdx].numLastiTbsIgnored = 0;
            }
         }
         else
         {
            if (ueDl->laCb[cwIdx].lastiTbsIgnored != TRUE)
            {
               ueDl->laCb[cwIdx].cqiBasediTbs = ((20 * iTbsNew * 100) +
                     (80 * ueDl->laCb[cwIdx].cqiBasediTbs))/100;
            }
            else
            {
               /* Reset the LA as iTbs in use caught up with the value   */
               /* reported by UE.                                        */
               ueDl->laCb[cwIdx].cqiBasediTbs = ((20 * iTbsNew * 100) +
                     (80 * ueDl->mimoInfo.cwInfo[cwIdx].iTbs[0] * 100))/100;
               ueDl->laCb[cwIdx].deltaiTbs = 0;
               ueDl->laCb[cwIdx].lastiTbsIgnored = FALSE;
            }
         }

         iTbsNew = (ueDl->laCb[cwIdx].cqiBasediTbs + ueDl->laCb[cwIdx].deltaiTbs)/100;

         RG_SCH_CHK_ITBS_RANGE(iTbsNew, maxiTbs); 	

         ueDl->mimoInfo.cwInfo[cwIdx].iTbs[0] = RGSCH_MIN(iTbsNew, cell->thresholds.maxDlItbs);
         ueDl->mimoInfo.cwInfo[cwIdx].iTbs[1] = ueDl->mimoInfo.cwInfo[cwIdx].iTbs[0];

         ueDl->mimoInfo.cwInfo[cwIdx].eff[0] = (*(RgSchCmnTbSzEff *)(cellSch->dl.cqiToEffTbl[0][cfi]))\
                                               [ueDl->mimoInfo.cwInfo[cwIdx].iTbs[0]];
         /* Eff for CW0 for 2 Layer Tx */
         ueDl->mimoInfo.cwInfo[cwIdx].eff[1] = (*(RgSchCmnTbSzEff *)(cellSch->dl.cqiToEffTbl[1][cfi]))\
                                               [ueDl->mimoInfo.cwInfo[cwIdx].iTbs[1]];
         if((ue->mimoInfo.txMode != RGR_UE_TM_3) && (ue->mimoInfo.txMode != RGR_UE_TM_4))
         {
            break; 
         }
      }
      ueDl->cqiFlag = FALSE;
   } 

   /* Assigning noLyrs to each CW assuming optimal Spatial multiplexing
    * capability */
   (ueDl->mimoInfo.ri/2 == 0)? (ueDl->mimoInfo.cwInfo[0].noLyr = 1) : \
              (ueDl->mimoInfo.cwInfo[0].noLyr = ueDl->mimoInfo.ri/2);
   ueDl->mimoInfo.cwInfo[1].noLyr = ueDl->mimoInfo.ri - ueDl->mimoInfo.cwInfo[0].noLyr;
   /* correcting DL harq softbuffer limitation logic.
    * The maxTbSz is the maximum number of PHY bits a harq process can
    * hold. Hence we limit our allocation per harq process based on this.
    * Earlier implementation we misinterpreted the maxTbSz to be per UE
    * per TTI, but in fact it is per Harq per TTI. */
   /* cannot exceed the harq Tb Size
    * and harq Soft Bits limit.*/


   /* Set the index of the cwInfo, which is better in terms of
    * efficiency. If RI<2, only 1 CW, hence btrCwIdx shall be 0 */
   if (ueDl->mimoInfo.ri < 2)
   {
      ueDl->mimoInfo.btrCwIdx = 0;
   }
   else
   {
      if (ueDl->mimoInfo.cwInfo[0].eff[ueDl->mimoInfo.cwInfo[0].noLyr-1] <\
          ueDl->mimoInfo.cwInfo[1].eff[ueDl->mimoInfo.cwInfo[1].noLyr-1])
      {
         ueDl->mimoInfo.btrCwIdx = 1;
      }
      else
      {
         ueDl->mimoInfo.btrCwIdx = 0;
      }
   }
   /* Considering iTbs corresponding to 2 layer transmission for
    * better codeword(approximation) and the maxLayers supported by
    * this UE at this point of time. */
   RG_SCH_CMN_TBS_TO_MODODR(ueDl->mimoInfo.cwInfo[ueDl->mimoInfo.btrCwIdx].iTbs[1], modOrder);
   /* Bits/modOrder gives #REs, #REs/noResPerRb gives #RBs */
   maxRb = ((ueDl->maxSbSz)/(cellSch->dl.noResPerRb[cfi] * modOrder *\
                   ueDl->mimoInfo.ri));
   if (cellSch->dl.isDlFreqSel)
   {
      /* Rounding off to left nearest multiple of RBG size */
      maxRb -= maxRb % cell->rbgSize;
   }
   ueDl->maxRb = RGSCH_MIN(maxRb, cellSch->dl.maxDlBwPerUe);
   if (cellSch->dl.isDlFreqSel)
   {
      /* Rounding off to right nearest multiple of RBG size */
      if (ueDl->maxRb % cell->rbgSize)
      {
         ueDl->maxRb += (cell->rbgSize - 
                         (ueDl->maxRb % cell->rbgSize));
      }
   }

   RETVOID;
}
#endif
/***********************************************************
 *
 *     Func : rgSCHCmnDlUeResetTemp
 *
 *     Desc : Reset whatever variables where temporarily used
 *            during UE scheduling.
 *
 *     Ret  : Void
 *
 *     Notes:
 *
 *     File :
 *
 **********************************************************/
#ifdef ANSI
PRIVATE Void rgSCHCmnDlHqPResetTemp 
(
RgSchDlHqProcCb         *hqP
)
#else
PRIVATE Void rgSCHCmnDlHqPResetTemp(hqP)
RgSchDlHqProcCb         *hqP;
#endif
{

   TRC2(rgSCHCmnDlHqPResetTemp);

   /* Fix: syed having a hqP added to Lists for RB assignment rather than
    * a UE, as adding UE was limiting handling some scenarios */ 
    hqP->reqLnk.node = (PTR)NULLP;
    hqP->schdLstLnk.node = (PTR)NULLP;

   RETVOID;
}  /* rgSCHCmnDlHqPResetTemp */

/***********************************************************
 *
 *     Func : rgSCHCmnDlUeResetTemp
 *
 *     Desc : Reset whatever variables where temporarily used
 *            during UE scheduling.
 *
 *     Ret  : Void
 *
 *     Notes:
 *
 *     File :
 *
 **********************************************************/
#ifdef ANSI
PUBLIC Void rgSCHCmnDlUeResetTemp
(
RgSchUeCb               *ue,
RgSchDlHqProcCb         *hqP
)
#else
PUBLIC Void rgSCHCmnDlUeResetTemp(ue, hqP)
RgSchUeCb               *ue;
RgSchDlHqProcCb         *hqP;
#endif
{
   RgSchCmnDlUe       *cmnUe = RG_SCH_CMN_GET_DL_UE(ue);

   TRC2(rgSCHCmnDlUeResetTemp);

   /* Fix : syed check for UE's existence was useless.
    * Instead we need to check that reset is done only for the 
    * information of a scheduled harq proc, which is cmnUe->proc.
    * Reset should not be done for non-scheduled hqP */
   if((cmnUe->proc == hqP) || (cmnUe->proc == NULLP))
   {
      cmnUe->proc = NULLP;
      cmMemset((U8 *)&ue->dl.dlAllocCb, (U8)0, sizeof(ue->dl.dlAllocCb));
      ue->dl.dlAllocCb.rnti = ue->ueId;
      /* Fix: syed moving this to a common function for both scheduled
       * and non-scheduled UEs */
      cmnUe->outStndAlloc = 0;
   }
   rgSCHCmnDlHqPResetTemp(hqP);

   RETVOID;
}  /* rgSCHCmnDlUeResetTemp */

/***********************************************************
 *
 *     Func : rgSCHCmnUlUeResetTemp
 *
 *     Desc : Reset whatever variables where temporarily used
 *            during UE scheduling.
 *
 *     Ret  : Void
 *
 *     Notes:
 *
 *     File :
 *
 **********************************************************/
#ifdef ANSI
PUBLIC Void rgSCHCmnUlUeResetTemp
(
RgSchUeCb               *ue
)
#else
PUBLIC Void rgSCHCmnUlUeResetTemp(ue)
RgSchUeCb               *ue;
#endif
{
   RgSchCmnUlUe       *cmnUlUe = RG_SCH_CMN_GET_UL_UE(ue);

   TRC2(rgSCHCmnUlUeResetTemp);

   cmMemset((U8 *)&cmnUlUe->alloc, (U8)0, sizeof(cmnUlUe->alloc));

   RETVOID;
}  /* rgSCHCmnUlUeResetTemp */



/**
 * @brief This function fills the PDCCH information from dlProc.
 *
 * @details
 *
 *     Function: rgSCHCmnFillPdcch
 *     Purpose:  This function fills in the PDCCH information
 *               obtained from the RgSchDlRbAlloc
 *               during common channel scheduling(P, SI, RA - RNTI's).
 *
 *     Invoked by: Downlink Scheduler
 *
 *  @param[out] RgSchPdcch*       pdcch
 *  @param[in]  RgSchDlRbAlloc*   rbAllocInfo
 *  @return  Void
 *
 **/
#ifdef ANSI
PUBLIC Void rgSCHCmnFillPdcch
(
RgSchCellCb                *cell,
RgSchPdcch                 *pdcch,
RgSchDlRbAlloc             *rbAllocInfo
)
#else
PUBLIC Void rgSCHCmnFillPdcch(cell, pdcch, rbAllocInfo)
RgSchCellCb                *cell;
RgSchPdcch                 *pdcch;
RgSchDlRbAlloc             *rbAllocInfo;
#endif
{

   TRC2(rgSCHCmnFillPdcch);

   /* common channel pdcch filling,
    * only 1A and Local is supported */
   pdcch->rnti                       = rbAllocInfo->rnti;
   pdcch->dci.dciFormat              = rbAllocInfo->dciFormat;
   switch(rbAllocInfo->dciFormat)
   {
      case TFU_DCI_FORMAT_1A:
         pdcch->dci.u.format1aInfo.isPdcchOrder = FALSE;

         /*Nprb indication at PHY for common Ch
          *setting least significant bit of tpc field to 1 if
          nPrb=3 and 0 otherwise. */
         if (rbAllocInfo->nPrb == 3)
         {
            pdcch->dci.u.format1aInfo.t.pdschInfo.tpcCmd  = 1;
         }
         else
         {
            pdcch->dci.u.format1aInfo.t.pdschInfo.tpcCmd  = 0;
         }
         pdcch->dci.u.format1aInfo.t.pdschInfo.allocInfo.nGap2.pres = NOTPRSNT;
         pdcch->dci.u.format1aInfo.t.pdschInfo.allocInfo.isLocal = TRUE;
         pdcch->dci.u.format1aInfo.t.pdschInfo.allocInfo.mcs     = \
                                                                   rbAllocInfo->tbInfo[0].imcs;
         pdcch->dci.u.format1aInfo.t.pdschInfo.allocInfo.ndi     = 0;
         pdcch->dci.u.format1aInfo.t.pdschInfo.allocInfo.rv      = 0;
         /* Qcom: Adding Payload Len for FAPI API */
         pdcch->dci.u.format1aInfo.t.pdschInfo.allocInfo.payloadLen      = rbAllocInfo->tbInfo[0].bytesReq;
         /* Add RIV CALC */
         pdcch->dci.u.format1aInfo.t.pdschInfo.allocInfo.alloc.type =
            TFU_ALLOC_TYPE_RIV;
         pdcch->dci.u.format1aInfo.t.pdschInfo.allocInfo.alloc.u.riv =
            rgSCHCmnCalcRiv (cell->bwCfg.dlTotalBw,
                  rbAllocInfo->allocInfo.raType2.rbStart,
                  rbAllocInfo->allocInfo.raType2.numRb);
#if 0
         QU_CRITLOG(QU_LOG_MESSAGE,
               "rgSCHCmnFillPdcch(): Format1A");
         QU_CRITLOG(QU_LOG_MESSAGE,
               "dcch info numRb=%d,mcs=%d,riv=%lu,tpccmd=%d",\
               rbAllocInfo->allocInfo.raType2.numRb,
               pdcch->dci.u.format1aInfo.t.pdschInfo.allocInfo.mcs,
               pdcch->dci.u.format1aInfo.t.pdschInfo.allocInfo.alloc.u.riv,
               pdcch->dci.u.format1aInfo.t.pdschInfo.tpcCmd);
#endif
#ifdef LTE_TDD
         pdcch->dci.u.format1aInfo.t.pdschInfo.allocInfo.harqProcId.pres = \
                                                                           FALSE;
#ifdef TFU_TDD
         pdcch->dci.u.format1aInfo.t.pdschInfo.dai.pres = TRUE;
         pdcch->dci.u.format1aInfo.t.pdschInfo.dai.val = 1;
#endif
#endif
         break; /* case TFU_DCI_FORMAT_1A: */
      case TFU_DCI_FORMAT_1:
         pdcch->dci.u.format1Info.tpcCmd = 0;
         /* Avoiding this check,as we dont support Type1 RA */
#ifdef RG_UNUSED
         if (rbAllocInfo->raType == RG_SCH_CMN_RA_TYPE0)
         {
#endif
            pdcch->dci.u.format1Info.allocInfo.isAllocType0 = TRUE;
            pdcch->dci.u.format1Info.allocInfo.resAllocMap[0] =
               ((rbAllocInfo->allocInfo.raType0.dlAllocBitMask >> 24)
                & 0xff);
            pdcch->dci.u.format1Info.allocInfo.resAllocMap[1] =
               ((rbAllocInfo->allocInfo.raType0.dlAllocBitMask >> 16)
                & 0x00ff);
            pdcch->dci.u.format1Info.allocInfo.resAllocMap[2] =
               ((rbAllocInfo->allocInfo.raType0.dlAllocBitMask >> 8)
                & 0x0000ff);
            pdcch->dci.u.format1Info.allocInfo.resAllocMap[3] =
               ((rbAllocInfo->allocInfo.raType0.dlAllocBitMask & 0x000000ff));
#ifdef RG_UNUSED
         }
#endif
         pdcch->dci.u.format1Info.allocInfo.harqProcId = 0;
         pdcch->dci.u.format1Info.allocInfo.ndi = 0;
         pdcch->dci.u.format1Info.allocInfo.mcs = rbAllocInfo->tbInfo[0].imcs;
         pdcch->dci.u.format1Info.allocInfo.rv = 0;
#ifdef TFU_TDD
         pdcch->dci.u.format1Info.dai = 1;
#endif
         break;
      default:
         RLOG_ARG1(L_ERROR,DBG_CELLID,cell->cellId,"Allocator's icorrect "
            "dciForamt Fill RNTI:%d",rbAllocInfo->rnti);
         break;
   }
   RETVOID;
}

#ifdef LTE_TDD
/**
 * @brief This function finds whether the subframe is special subframe or not.
 *
 * @details
 *
 *     Function: rgSCHCmnIsSplSubfrm
 *     Purpose:  This function finds the subframe index of the special subframe
 *               and finds whether the current DL index matches it or not.
 *
 *     Invoked by: Scheduler
 *
 *  @param[in] U8                   splfrmCnt
 *  @param[in] U8                   curSubfrmIdx
 *  @param[in] U8                   periodicity
 *  @param[in] RgSchTddSubfrmInfo   *subfrmInfo
 *  @return  Bool
 *
 **/
#ifdef ANSI
PRIVATE Bool rgSCHCmnIsSplSubfrm
(
U8                   splfrmCnt,
U8                   curSubfrmIdx,
U8                   periodicity,
RgSchTddSubfrmInfo   *subfrmInfo
)
#else
PRIVATE Bool rgSCHCmnIsSplSubfrm(splfrmCnt, curSubfrmIdx, periodicity, subfrmInfo)
U8                   splfrmCnt;
U8                   curSubfrmIdx;
U8                   periodicity;
RgSchTddSubfrmInfo   *subfrmInfo;
#endif
{
   U8 dlSfCnt = 0;
   U8 splfrmIdx  = 0;

   TRC2(rgSCHCmnIsSplSubfrm);

   if(splfrmCnt > 0)
   {
      if(periodicity == RG_SCH_CMN_5_MS_PRD)
      {
         if(splfrmCnt%2)
         {
            dlSfCnt = ((splfrmCnt-1)/2) *\
                      (subfrmInfo->numFrmHf1 + subfrmInfo->numFrmHf2);
            dlSfCnt = dlSfCnt + subfrmInfo->numFrmHf1;
         }
         else
         {
            dlSfCnt = (splfrmCnt/2) * \
                      (subfrmInfo->numFrmHf1 + subfrmInfo->numFrmHf2);
         }
      }
      else
      {
         dlSfCnt = splfrmCnt * subfrmInfo->numFrmHf1;
      }
      splfrmIdx = RG_SCH_CMN_SPL_SUBFRM_1 +\
                  (periodicity*splfrmCnt - dlSfCnt);
   }
   else
   {
      splfrmIdx = RG_SCH_CMN_SPL_SUBFRM_1;
   }

   if(splfrmIdx == curSubfrmIdx)
   {
      RETVALUE(TRUE);
   }

   RETVALUE(FALSE);
}

/**
 * @brief This function updates DAI or UL index.
 *
 * @details
 *
 *     Function: rgSCHCmnUpdHqAndDai
 *     Purpose:  Updates the DAI based on UL-DL Configuration
 *               index and UE. It also updates the HARQ feedback
 *               time and 'm' index.
 *
 *     Invoked by: TOM
 *
 *  @param[in]  RgDlHqProcCb  *hqP
 *  @param[in]  RgSchDlSf     *subFrm
 *  @param[in]  RgSchDlHqTbCb *tbCb
 *  @param[in]  U8            tbAllocIdx
 *  @return  Void
 *
 **/
#ifdef ANSI
PRIVATE Void rgSCHCmnUpdHqAndDai
(
RgSchDlHqProcCb   *hqP,
RgSchDlSf         *subFrm,
RgSchDlHqTbCb     *tbCb,
U8                tbAllocIdx
)
#else
PRIVATE Void rgSCHCmnUpdHqAndDai(hqP, subFrm, tbCb,tbAllocIdx)
RgSchDlHqProcCb   *hqP;
RgSchDlSf         *subFrm;
RgSchDlHqTbCb     *tbCb;
U8                tbAllocIdx;
#endif
{
   RgSchUeCb      *ue = hqP->hqE->ue;
   RgSchCmnUe     *schCmnUe;

   TRC2(rgSCHCmnUpdHqAndDai);

   if(subFrm != NULLP)
   {
      /* set the time at which UE shall send the feedback
       * for this process */
      tbCb->fdbkTime.sfn = (tbCb->timingInfo.sfn + \
            subFrm->dlFdbkInfo.sfnOffset) % RGSCH_MAX_SFN;
      tbCb->fdbkTime.subframe = subFrm->dlFdbkInfo.subframe;
      tbCb->m = subFrm->dlFdbkInfo.m;
   }
   else
   {
      schCmnUe = (RgSchCmnUe *)(ue->sch);
      /* set the time at which UE shall send the feedback
       * for this process */
      tbCb->fdbkTime.sfn = (tbCb->timingInfo.sfn + \
            tbCb->subFrm->dlFdbkInfo.sfnOffset) % RGSCH_MAX_SFN;
      tbCb->fdbkTime.subframe = tbCb->subFrm->dlFdbkInfo.subframe;
      tbCb->m = tbCb->subFrm->dlFdbkInfo.m;
   }

   /* ccpu00132340-MOD- DAI need to be updated for first TB only*/
   if(ue && !tbAllocIdx)
   {
      Bool   havePdcch = (tbCb->pdcch ? TRUE : FALSE);
      rgSCHCmnUpdDai(ue, &tbCb->fdbkTime, tbCb->m, havePdcch);
   }
   RETVOID;
}


/**
 * @brief This function updates DAI or UL index.
 *
 * @details
 *
 *     Function: rgSCHCmnUpdDai
 *     Purpose:  Updates the DAI in the ack-nack info, a valid
 *               ue should be passed
 *
 *     Invoked by: TOM
 *
 *  @param[in]  RgDlHqProcCb  *hqP
 *  @param[in]  RgSchDlSf     *subFrm
 *  @param[in]  RgSchDlHqTbCb *tbCb
 *  @return  Void
 *
 **/
#ifdef ANSI
PUBLIC Void rgSCHCmnUpdDai
(
RgSchUeCb         *ue,
CmLteTimingInfo   *fdbkTime,
U8                 m,
Bool               havePdcch
)
#else
PUBLIC Void rgSCHCmnUpdDai(ue, fdbkTime, m, havePdcch)
RgSchUeCb         *ue;
CmLteTimingInfo   *fdbkTime;
U8                 m;
Bool               havePdcch;
#endif
{
   RgSchTddANInfo *anInfo;

   TRC2(rgSCHCmnUpdDai);

   anInfo = rgSCHUtlGetUeANFdbkInfo(ue, fdbkTime);

   /* If no ACK/NACK feedback already present, create a new one */
   if(NULLP == anInfo)
   {
      anInfo = &ue->dl.anInfo[ue->dl.nextFreeANIdx];
      anInfo->sfn = fdbkTime->sfn;
      anInfo->subframe = fdbkTime->subframe;
      anInfo->latestMIdx = m;
      /* Fixing DAI value - ccpu00109162 */
      /* Handle TDD case as in MIMO definition of the function */
      anInfo->ulDai = 1;
      if (havePdcch)
      {
         anInfo->dlDai = 1;
      }

      /* set the free Index to store  Ack/Nack Information*/
      ue->dl.nextFreeANIdx = (ue->dl.nextFreeANIdx + 1) %
         ue->cell->ackNackFdbkArrSize;
   }
   else
   {
      anInfo->latestMIdx = m;
      /* Fixing DAI value - ccpu00109162 */
      /* Handle TDD case as in MIMO definition of the function */
      anInfo->ulDai = anInfo->ulDai + 1;
      if (havePdcch)
      {
         anInfo->dlDai = anInfo->dlDai + 1;
      }
   }
   RETVOID;
}
#endif /* ifdef LTE_TDD */

PUBLIC U32 rgHqRvRetxCnt[4][2];
PUBLIC U32 rgUlrate_grant;

/**
 * @brief This function fills the HqP TB with rbAllocInfo.
 *
 * @details
 *
 *     Function: rgSCHCmnFillHqPTb
 *     Purpose:  This function fills in the HqP TB with rbAllocInfo.
 *
 *     Invoked by: rgSCHCmnFillHqPPdcch
 *
 *  @param[in]  RgSchCellCb*      cell
 *  @param[in]  RgSchDlRbAlloc    *rbAllocInfo,
 *  @param[in]  U8                tbAllocIdx
 *  @param[in]  RgSchPdcch        *pdcch
 *  @return  Void
 *
 **/
#ifdef LTEMAC_SPS
#ifdef ANSI
PUBLIC Void rgSCHCmnFillHqPTb
(
RgSchCellCb                *cell,
RgSchDlRbAlloc             *rbAllocInfo,
U8                         tbAllocIdx,
RgSchPdcch                 *pdcch
)
#else
PUBLIC Void rgSCHCmnFillHqPTb(cell, rbAllocInfo, tbAllocIdx, pdcch)
RgSchCellCb                *cell;
RgSchDlRbAlloc             *rbAllocInfo;
U8                         tbAllocIdx;
RgSchPdcch                 *pdcch;
#endif
#else
#ifdef ANSI
PRIVATE Void rgSCHCmnFillHqPTb
(
RgSchCellCb                *cell,
RgSchDlRbAlloc             *rbAllocInfo,
U8                         tbAllocIdx,
RgSchPdcch                 *pdcch
)
#else
PRIVATE Void rgSCHCmnFillHqPTb(cell, rbAllocInfo, tbAllocIdx, pdcch)
RgSchCellCb                *cell;
RgSchDlRbAlloc             *rbAllocInfo;
U8                         tbAllocIdx;
RgSchPdcch                 *pdcch;
#endif
#endif /* LTEMAC_SPS */
{
   RgSchCmnDlCell     *cmnCellDl = RG_SCH_CMN_GET_DL_CELL(cell);
   RgSchDlTbAllocInfo *tbAllocInfo = &rbAllocInfo->tbInfo[tbAllocIdx];
   RgSchDlHqTbCb      *tbInfo = tbAllocInfo->tbCb;
   RgSchDlHqProcCb    *hqP = tbInfo->hqP;

   TRC2(rgSCHCmnFillHqPTb);

   /*ccpu00120365-ADD-if tb is disabled, set mcs=0,rv=1.
    * Relevant for DCI format 2 & 2A as per 36.213-7.1.7.2
    */
   if ( tbAllocInfo->isDisabled)
   {

      tbInfo->dlGrnt.iMcs = 0;
      tbInfo->dlGrnt.rv   = 1;
   }
   /* Fill for TB retransmission */
   else if (tbInfo->txCntr > 0)
   {

      tbInfo->timingInfo = cmnCellDl->time;
      /* Fix */
      if ((tbInfo->isAckNackDtx == TFU_HQFDB_DTX)) 
      {
         tbInfo->dlGrnt.iMcs = tbAllocInfo->imcs;         
         rgHqRvRetxCnt[tbInfo->dlGrnt.rv][tbInfo->tbIdx]++;
      }
      else
      {
         tbInfo->dlGrnt.rv = rgSchCmnDlRvTbl[++(tbInfo->ccchSchdInfo.rvIdx) & 0x03];
      }

      /* fill the scheduler information of hqProc */
      tbInfo->ccchSchdInfo.totBytes = tbAllocInfo->bytesAlloc;
      RGSCH_ARRAY_BOUND_CHECK(cell->instIdx,hqP->tbInfo,tbInfo->tbIdx );
      rgSCHDhmHqTbRetx(hqP->hqE, tbInfo->timingInfo, hqP, tbInfo->tbIdx);
   }
   /* Fill for TB transmission */
   else
   {
      /* Fill the HqProc */
      tbInfo->dlGrnt.iMcs = tbAllocInfo->imcs;
      tbInfo->tbSz = tbAllocInfo->bytesAlloc;
      tbInfo->timingInfo = cmnCellDl->time;
      tbInfo->dlGrnt.rv = rgSchCmnDlRvTbl[0];
      /* fill the scheduler information of hqProc */
      tbInfo->ccchSchdInfo.rvIdx = 0;
      tbInfo->ccchSchdInfo.totBytes = tbAllocInfo->bytesAlloc;
      /* DwPts Scheduling Changes Start */
#ifdef LTE_TDD     
      /* Update the SF type for the TB only for the first transmission */ 
      tbInfo->sfType = rbAllocInfo->dlSf->sfType;
#endif      
      /* DwPts Scheduling Changes End */ 
      cell->measurements.dlBytesCnt += tbAllocInfo->bytesAlloc;
   }

   /*ccpu00120365:-ADD-only add to subFrm list if tb is not disabled */
   if ( tbAllocInfo->isDisabled == FALSE )
   {
      /* Set the number of transmitting SM layers for this TB */
      tbInfo->numLyrs = tbAllocInfo->noLyr;
      /* Set the TB state as WAITING to indicate TB has been
       * considered for transmission */
      tbInfo->state  = HQ_TB_WAITING;
      tbInfo->subFrm = rbAllocInfo->dlSf;
      tbInfo->pdcch  = pdcch;
      tbInfo->dlGrnt.numRb = rbAllocInfo->rbsAlloc;
#ifdef LTE_TDD
      /* ccpu00132340-MOD- DAI needs to be updated per subframe not per TB*/
      rgSCHCmnUpdHqAndDai(hqP, tbInfo->subFrm, tbInfo, tbAllocIdx);
#endif
      rgSCHUtlDlHqPTbAddToTx(tbInfo->subFrm, hqP, tbInfo->tbIdx);
   }
   RETVOID;
}

/**
 * @brief This function fills the PDCCH information from dlProc.
 *
 * @details
 *
 *     Function: rgSCHCmnFillHqPPdcch
 *     Purpose:  This function fills in the PDCCH information
 *               obtained from the RgSchDlHqProcCb and RgSchDlRbAlloc
 *               for dedicated service scheduling. It also
 *               obtains TPC to be filled in from the power module.
 *               Assign the PDCCH to HQProc.
 *
 *     Invoked by: Downlink Scheduler
 *
 *  @param[in]  RgSchCellCb*      cell
 *  @param[in]  RgSchDlRbAlloc*   rbAllocInfo
 *  @param[in]  RgDlHqProc*       hqP
 *  @return  Void
 *
 **/
#ifdef ANSI
PUBLIC Void rgSCHCmnFillHqPPdcch
(
RgSchCellCb                *cell,
RgSchDlRbAlloc             *rbAllocInfo,
RgSchDlHqProcCb            *hqP
)
#else
PUBLIC Void rgSCHCmnFillHqPPdcch(cell, rbAllocInfo, hqP)
RgSchCellCb                *cell;
RgSchDlRbAlloc             *rbAllocInfo;
RgSchDlHqProcCb            *hqP;
#endif
{
   RgSchCmnDlCell     *cmnCell = RG_SCH_CMN_GET_DL_CELL(cell);
   RgSchPdcch         *pdcch = rbAllocInfo->pdcch;
   U8                 tpc = 1;

   TRC2(rgSCHCmnFillHqPPdcch);

   if (hqP->hqE->ue)
   {
      tpc = rgSCHPwrPucchTpcForUe(cell, hqP->hqE->ue);
      /* Fix: syed moving this to a common function for both scheduled
       * and non-scheduled UEs */
      if (hqP->hqE->ue->csgMmbrSta == FALSE) 
      {
         cmnCell->ncsgPrbCnt += rbAllocInfo->rbsAlloc;
      }
      cmnCell->totPrbCnt += rbAllocInfo->rbsAlloc;
#ifdef TENB_STATS
      hqP->hqE->ue->tenbStats->stats.sch.dlPrbUsg += 
         rbAllocInfo->rbsAlloc;
      hqP->hqE->ue->tenbStats->stats.sch.dlSumCw0iTbs += 
         rbAllocInfo->tbInfo[0].iTbs;
      hqP->hqE->ue->tenbStats->stats.sch.dlNumCw0iTbs ++; 
      hqP->hqE->ue->tenbStats->stats.sch.dlTpt +=
         (rbAllocInfo->tbInfo[0].bytesAlloc << 3);
      cell->tenbStats->sch.dlPrbUsage[0] += 
         rbAllocInfo->rbsAlloc;
      cell->tenbStats->sch.dlSumCw0iTbs += 
         rbAllocInfo->tbInfo[0].iTbs;
      cell->tenbStats->sch.dlNumCw0iTbs ++; 
      cell->tenbStats->sch.dlTtlTpt +=
         (rbAllocInfo->tbInfo[0].bytesAlloc << 3); 
      if (rbAllocInfo->tbInfo[1].schdlngForTb)
      {
         cell->tenbStats->sch.dlSumCw1iTbs += 
            rbAllocInfo->tbInfo[1].iTbs;
         cell->tenbStats->sch.dlNumCw1iTbs ++; 
         hqP->hqE->ue->tenbStats->stats.sch.dlSumCw1iTbs += 
            rbAllocInfo->tbInfo[1].iTbs;
         hqP->hqE->ue->tenbStats->stats.sch.dlNumCw1iTbs ++; 
         hqP->hqE->ue->tenbStats->stats.sch.dlTpt +=
            (rbAllocInfo->tbInfo[1].bytesAlloc << 3);
         cell->tenbStats->sch.dlTtlTpt +=
            (rbAllocInfo->tbInfo[1].bytesAlloc << 3);
      }
#endif
   }

   pdcch->rnti                       = rbAllocInfo->rnti;
   pdcch->dci.dciFormat              = rbAllocInfo->dciFormat;
   /* Update subframe and pdcch info in HqTb control block */
   switch(rbAllocInfo->dciFormat)
   {
      case TFU_DCI_FORMAT_1:
          {
              rgSCHCmnFillHqPPdcchDciFrmt1(cell, rbAllocInfo, hqP, \
                      pdcch, tpc);
              break;
          }
      case TFU_DCI_FORMAT_1A:
          {
              rgSCHCmnFillHqPPdcchDciFrmt1A(cell, rbAllocInfo, hqP, \
                      pdcch, tpc);
              break;
          }
      case TFU_DCI_FORMAT_1B:
          {
              rgSCHCmnFillHqPPdcchDciFrmt1B(cell, rbAllocInfo, hqP, \
                      pdcch, tpc);
              break;
          }
      case TFU_DCI_FORMAT_2:
          {
              rgSCHCmnFillHqPPdcchDciFrmt2(cell, rbAllocInfo, hqP, \
                      pdcch, tpc);
              break;
          }
      case TFU_DCI_FORMAT_2A:
          {
               rgSCHCmnFillHqPPdcchDciFrmt2A(cell, rbAllocInfo, hqP, \
                       pdcch, tpc);
               break;
          }
      default:
         RLOG_ARG1(L_ERROR,DBG_CELLID,cell->cellId,
            "Allocator's incorrect dciForamt Fill for RNTI:%d",rbAllocInfo->rnti);
         break;
   }
   RETVOID;
}

/**
 * @brief This function fills the PDCCH DCI format 1 information from dlProc.
 *
 * @details
 *
 *     Function: rgSCHCmnFillHqPPdcchDciFrmt1
 *     Purpose:  This function fills in the PDCCH information
 *               obtained from the RgSchDlHqProcCb and RgSchDlRbAlloc
 *               for dedicated service scheduling. It also
 *               obtains TPC to be filled in from the power module.
 *               Assign the PDCCH to HQProc.
 *
 *     Invoked by: Downlink Scheduler
 *
 *  @param[in]  RgSchCellCb*      cell
 *  @param[in]  RgSchDlRbAlloc*   rbAllocInfo
 *  @param[in]  RgDlHqProc*       hqP
 *  @param[out]  RgSchPdcch        *pdcch
 *  @param[in]   U8               tpc
 *  @return  Void
 *
 **/
#ifdef ANSI
PRIVATE Void rgSCHCmnFillHqPPdcchDciFrmt1
(
RgSchCellCb                *cell,
RgSchDlRbAlloc             *rbAllocInfo,
RgSchDlHqProcCb            *hqP,
RgSchPdcch                 *pdcch,
U8                         tpc
)
#else
PRIVATE Void rgSCHCmnFillHqPPdcchDciFrmt1(cell, rbAllocInfo, hqP, pdcch, tpc)
RgSchCellCb                *cell;
RgSchDlRbAlloc             *rbAllocInfo;
RgSchDlHqProcCb            *hqP;
RgSchPdcch                 *pdcch;
U8                         tpc;
#endif
{

#ifdef LTE_TDD
   RgSchTddANInfo     *anInfo;
#endif

#ifdef LTEMAC_SPS
/* For activation or reactivation,
 * Harq ProcId should be 0 */
   RgSchCmnDlHqProc *cmnHqDl = RG_SCH_CMN_GET_DL_HQP(hqP);
#endif

    TRC2(rgSCHCmnFillHqPPdcchDciFrmt1)

    rgSCHCmnFillHqPTb(cell, rbAllocInfo, 0, pdcch);
    pdcch->dci.u.format1Info.tpcCmd = tpc;
     /* Avoiding this check,as we dont support Type1 RA */
#ifdef RG_UNUSED
    if (rbAllocInfo->raType == RG_SCH_CMN_RA_TYPE0)
    {
#endif
       pdcch->dci.u.format1Info.allocInfo.isAllocType0 = TRUE;
       pdcch->dci.u.format1Info.allocInfo.resAllocMap[0] =
         ((rbAllocInfo->allocInfo.raType0.dlAllocBitMask >> 24)
               & 0xff);
       pdcch->dci.u.format1Info.allocInfo.resAllocMap[1] =
         ((rbAllocInfo->allocInfo.raType0.dlAllocBitMask >> 16)
               & 0x00ff);
       pdcch->dci.u.format1Info.allocInfo.resAllocMap[2] =
           ((rbAllocInfo->allocInfo.raType0.dlAllocBitMask >> 8)
               & 0x0000ff);
       pdcch->dci.u.format1Info.allocInfo.resAllocMap[3] =
           ((rbAllocInfo->allocInfo.raType0.dlAllocBitMask & 0x000000ff));
#ifdef RG_UNUSED
    }
#endif
#ifdef LTEMAC_SPS
    if ((!(hqP->tbInfo[0].txCntr)) &&
       (cmnHqDl != (RgSchCmnDlHqProc*)NULLP  &&
         ((cmnHqDl->spsAction & RG_SCH_CMN_SPS_DL_ACTV) ||
         (cmnHqDl->spsAction & RG_SCH_CMN_SPS_DL_REACTV)))
       )
    {
       pdcch->dci.u.format1Info.allocInfo.harqProcId = 0;
    }
    else
    {
      pdcch->dci.u.format1Info.allocInfo.harqProcId = hqP->procId;
    }
#else
    pdcch->dci.u.format1Info.allocInfo.harqProcId = hqP->procId;
#endif

    pdcch->dci.u.format1Info.allocInfo.ndi = 
                        rbAllocInfo->tbInfo[0].tbCb->ndi;
    pdcch->dci.u.format1Info.allocInfo.mcs = 
                        rbAllocInfo->tbInfo[0].imcs;
    pdcch->dci.u.format1Info.allocInfo.rv = 
                        rbAllocInfo->tbInfo[0].tbCb->dlGrnt.rv;
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
               /* Fixing DAI value - ccpu00109162 */
             pdcch->dci.u.format1Info.dai = RG_SCH_MAX_DAI_IDX;
          }
#endif
       }
       else
       {
            /* always 0 for RACH */
           pdcch->dci.u.format1Info.allocInfo.harqProcId = 0;
#ifdef TFU_TDD
            /* Fixing DAI value - ccpu00109162 */
           pdcch->dci.u.format1Info.dai = 1;
#endif
       }
#endif
 

       RETVOID;
}
/**
 * @brief This function fills the PDCCH DCI format 1A information from dlProc.
 *
 * @details
 *
 *     Function: rgSCHCmnFillHqPPdcchDciFrmt1A
 *     Purpose:  This function fills in the PDCCH information
 *               obtained from the RgSchDlHqProcCb and RgSchDlRbAlloc
 *               for dedicated service scheduling. It also
 *               obtains TPC to be filled in from the power module.
 *               Assign the PDCCH to HQProc.
 *
 *     Invoked by: Downlink Scheduler
 *
 *  @param[in]  RgSchCellCb*      cell
 *  @param[in]  RgSchDlRbAlloc*   rbAllocInfo
 *  @param[in]  RgDlHqProc*       hqP
 *  @param[out]  RgSchPdcch        *pdcch
 *  @param[in]   U8               tpc
 *  @return  Void
 *
 **/
#ifdef ANSI
PRIVATE Void rgSCHCmnFillHqPPdcchDciFrmt1A
(
RgSchCellCb                *cell,
RgSchDlRbAlloc             *rbAllocInfo,
RgSchDlHqProcCb            *hqP,
RgSchPdcch                 *pdcch,
U8                         tpc
)
#else
PRIVATE Void rgSCHCmnFillHqPPdcchDciFrmt1A(cell, rbAllocInfo, hqP, pdcch, tpc)
RgSchCellCb                *cell;
RgSchDlRbAlloc             *rbAllocInfo;
RgSchDlHqProcCb            *hqP;
RgSchPdcch                 *pdcch;
U8                         tpc;
#endif
{

#ifdef LTE_TDD
   RgSchTddANInfo     *anInfo;
#endif

#ifdef LTEMAC_SPS
   RgSchCmnDlHqProc *cmnHqDl = RG_SCH_CMN_GET_DL_HQP(hqP);
#endif

    TRC2(rgSCHCmnFillHqPPdcchDciFrmt1A)

    rgSCHCmnFillHqPTb(cell, rbAllocInfo, 0, pdcch);
    pdcch->dci.u.format1aInfo.isPdcchOrder = FALSE;
    pdcch->dci.u.format1aInfo.t.pdschInfo.tpcCmd  = tpc;
    pdcch->dci.u.format1aInfo.t.pdschInfo.allocInfo.mcs     = \
      rbAllocInfo->tbInfo[0].imcs;
    pdcch->dci.u.format1aInfo.t.pdschInfo.allocInfo.harqProcId.pres = TRUE;
#ifdef LTEMAC_SPS
    if ((!(hqP->tbInfo[0].txCntr)) &&
       ( cmnHqDl != (RgSchCmnDlHqProc*)NULLP  &&
         ((cmnHqDl->spsAction & RG_SCH_CMN_SPS_DL_ACTV) ||
         (cmnHqDl->spsAction & RG_SCH_CMN_SPS_DL_REACTV))
       ))
    {
       pdcch->dci.u.format1aInfo.t.pdschInfo.allocInfo.harqProcId.val = 0;
    }
    else
    {
      pdcch->dci.u.format1aInfo.t.pdschInfo.allocInfo.harqProcId.val
                                                = hqP->procId;
    }
#else
    pdcch->dci.u.format1aInfo.t.pdschInfo.allocInfo.harqProcId.val =
                                              hqP->procId;
#endif
    pdcch->dci.u.format1aInfo.t.pdschInfo.allocInfo.ndi     = \
       rbAllocInfo->tbInfo[0].tbCb->ndi;
    pdcch->dci.u.format1aInfo.t.pdschInfo.allocInfo.rv      = \
       rbAllocInfo->tbInfo[0].tbCb->dlGrnt.rv;
         /* As of now, we do not support Distributed allocations */
    pdcch->dci.u.format1aInfo.t.pdschInfo.allocInfo.isLocal = TRUE;
    pdcch->dci.u.format1aInfo.t.pdschInfo.allocInfo.nGap2.pres = NOTPRSNT;
    pdcch->dci.u.format1aInfo.t.pdschInfo.allocInfo.alloc.type =
            TFU_ALLOC_TYPE_RIV;
    pdcch->dci.u.format1aInfo.t.pdschInfo.allocInfo.alloc.u.riv =
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
          /* Fixing DAI value - ccpu00109162 */
          pdcch->dci.u.format1aInfo.t.pdschInfo.dai.val = RG_SCH_MAX_DAI_IDX;
          RLOG_ARG1(L_ERROR,DBG_CELLID,cell->cellId,
                   "PDCCH is been scheduled without updating anInfo RNTI:%d",
                    rbAllocInfo->rnti);
       }
#endif
    }
    else
    {
            /* always 0 for RACH */
       pdcch->dci.u.format1aInfo.t.pdschInfo.allocInfo.harqProcId.pres
                                                                     = FALSE;
#ifdef TFU_TDD
       pdcch->dci.u.format1aInfo.t.pdschInfo.dai.pres = TRUE;
            /* Fixing DAI value - ccpu00109162 */
       pdcch->dci.u.format1aInfo.t.pdschInfo.dai.val = 1;
#endif
    }
#endif
 
    RETVOID;
}
/**
 * @brief This function fills the PDCCH DCI format 1B information from dlProc.
 *
 * @details
 *
 *     Function: rgSCHCmnFillHqPPdcchDciFrmt1B
 *     Purpose:  This function fills in the PDCCH information
 *               obtained from the RgSchDlHqProcCb and RgSchDlRbAlloc
 *               for dedicated service scheduling. It also
 *               obtains TPC to be filled in from the power module.
 *               Assign the PDCCH to HQProc.
 *
 *     Invoked by: Downlink Scheduler
 *
 *  @param[in]  RgSchCellCb*      cell
 *  @param[in]  RgSchDlRbAlloc*   rbAllocInfo
 *  @param[in]  RgDlHqProc*       hqP
 *  @param[out]  RgSchPdcch        *pdcch
 *  @param[in]   U8               tpc
 *  @return  Void
 *
 **/
#ifdef ANSI
PRIVATE Void rgSCHCmnFillHqPPdcchDciFrmt1B
(
RgSchCellCb                *cell,
RgSchDlRbAlloc             *rbAllocInfo,
RgSchDlHqProcCb            *hqP,
RgSchPdcch                 *pdcch,
U8                         tpc
)
#else
PRIVATE Void rgSCHCmnFillHqPPdcchDciFrmt1B(cell, rbAllocInfo, hqP, pdcch, tpc)
RgSchCellCb                *cell;
RgSchDlRbAlloc             *rbAllocInfo;
RgSchDlHqProcCb            *hqP;
RgSchPdcch                 *pdcch;
U8                         tpc;
#endif
{

#ifdef LTE_TDD
   RgSchTddANInfo     *anInfo;
#endif

#ifdef LTEMAC_SPS
   RgSchCmnDlHqProc *cmnHqDl = RG_SCH_CMN_GET_DL_HQP(hqP);
#endif

    TRC2(rgSCHCmnFillHqPPdcchDciFrmt1B)

    rgSCHCmnFillHqPTb(cell, rbAllocInfo, 0, pdcch);
    pdcch->dci.u.format1bInfo.tpcCmd  = tpc;
    pdcch->dci.u.format1bInfo.allocInfo.mcs     = \
           rbAllocInfo->tbInfo[0].imcs;
#ifdef LTEMAC_SPS
    if ((!(hqP->tbInfo[0].txCntr)) &&
       ( cmnHqDl != (RgSchCmnDlHqProc*)NULLP  &&
         ((cmnHqDl->spsAction & RG_SCH_CMN_SPS_DL_ACTV) ||
         (cmnHqDl->spsAction & RG_SCH_CMN_SPS_DL_REACTV))
       ))
    {
       pdcch->dci.u.format1bInfo.allocInfo.harqProcId = 0;
    }
    else
    {
      pdcch->dci.u.format1bInfo.allocInfo.harqProcId = hqP->procId;
    }
#else
    pdcch->dci.u.format1bInfo.allocInfo.harqProcId = hqP->procId;
#endif
    pdcch->dci.u.format1bInfo.allocInfo.ndi     = \
          rbAllocInfo->tbInfo[0].tbCb->ndi;
    pdcch->dci.u.format1bInfo.allocInfo.rv      = \
           rbAllocInfo->tbInfo[0].tbCb->dlGrnt.rv;
         /* As of now, we do not support Distributed allocations */
    pdcch->dci.u.format1bInfo.allocInfo.isLocal = TRUE;
    pdcch->dci.u.format1bInfo.allocInfo.nGap2.pres = NOTPRSNT;
    pdcch->dci.u.format1bInfo.allocInfo.alloc.type =
            TFU_ALLOC_TYPE_RIV;
    pdcch->dci.u.format1bInfo.allocInfo.alloc.u.riv =
    rgSCHCmnCalcRiv (cell->bwCfg.dlTotalBw,
                  rbAllocInfo->allocInfo.raType2.rbStart,
                  rbAllocInfo->allocInfo.raType2.numRb);
         /* Fill precoding Info */
    pdcch->dci.u.format1bInfo.allocInfo.pmiCfm = \
               rbAllocInfo->mimoAllocInfo.precIdxInfo >> 4;
    pdcch->dci.u.format1bInfo.allocInfo.tPmi   = \
               rbAllocInfo->mimoAllocInfo.precIdxInfo & 0x0F;
#ifdef LTE_TDD
    if(hqP->hqE->ue != NULLP)
    {
       anInfo = rgSCHUtlGetUeANFdbkInfo(hqP->hqE->ue,
             &(rbAllocInfo->tbInfo[0].tbCb->fdbkTime));
#ifdef TFU_TDD
       if(anInfo)
       {
          pdcch->dci.u.format1bInfo.dai = 
                         RG_SCH_GET_DAI_VALUE(anInfo->dlDai);
       }
       else
       {
          pdcch->dci.u.format1bInfo.dai = RG_SCH_MAX_DAI_IDX;
          RLOG_ARG1(L_ERROR,DBG_CELLID,cell->cellId,
                   "PDCCH is been scheduled without updating anInfo RNTI:%d",
                   rbAllocInfo->rnti);
       }
#endif
    }
#endif
       
    RETVOID;

}
/**
 * @brief This function fills the PDCCH DCI format 2 information from dlProc.
 *
 * @details
 *
 *     Function: rgSCHCmnFillHqPPdcchDciFrmt2
 *     Purpose:  This function fills in the PDCCH information
 *               obtained from the RgSchDlHqProcCb and RgSchDlRbAlloc
 *               for dedicated service scheduling. It also
 *               obtains TPC to be filled in from the power module.
 *               Assign the PDCCH to HQProc.
 *
 *     Invoked by: Downlink Scheduler
 *
 *  @param[in]  RgSchCellCb*      cell
 *  @param[in]  RgSchDlRbAlloc*   rbAllocInfo
 *  @param[in]  RgDlHqProc*       hqP
 *  @param[out]  RgSchPdcch        *pdcch
 *  @param[in]   U8               tpc
 *  @return  Void
 *
 **/
#ifdef ANSI
PRIVATE Void rgSCHCmnFillHqPPdcchDciFrmt2
(
RgSchCellCb                *cell,
RgSchDlRbAlloc             *rbAllocInfo,
RgSchDlHqProcCb            *hqP,
RgSchPdcch                 *pdcch,
U8                         tpc
)
#else
PRIVATE Void rgSCHCmnFillHqPPdcchDciFrmt2(cell, rbAllocInfo, hqP, pdcch, tpc)
RgSchCellCb                *cell;
RgSchDlRbAlloc             *rbAllocInfo;
RgSchDlHqProcCb            *hqP;
RgSchPdcch                 *pdcch;
U8                         tpc;
#endif
{

#ifdef LTE_TDD
   RgSchTddANInfo     *anInfo;
#endif

#ifdef LTEMAC_SPS
/* ccpu00119023-ADD-For activation or reactivation,
 * Harq ProcId should be 0 */
   RgSchCmnDlHqProc *cmnHqDl = RG_SCH_CMN_GET_DL_HQP(hqP);
#endif

    TRC2(rgSCHCmnFillHqPPdcchDciFrmt2)

    rgSCHCmnFillHqPTb(cell, rbAllocInfo, 0, pdcch);
    /*ccpu00120365:-ADD-call also if tb is disabled */
    if (rbAllocInfo->tbInfo[1].schdlngForTb ||
        rbAllocInfo->tbInfo[1].isDisabled)
    {
        rgSCHCmnFillHqPTb(cell, rbAllocInfo, 1, pdcch);
    }
    pdcch->dci.u.format2Info.tpcCmd = tpc;
         /* Avoiding this check,as we dont support Type1 RA */
#ifdef RG_UNUSED
    if (rbAllocInfo->raType == RG_SCH_CMN_RA_TYPE0)
    {
#endif
        pdcch->dci.u.format2Info.allocInfo.isAllocType0 = TRUE;
        pdcch->dci.u.format2Info.allocInfo.resAllocMap[0] =
          ((rbAllocInfo->allocInfo.raType0.dlAllocBitMask >> 24)
               & 0xff);
        pdcch->dci.u.format2Info.allocInfo.resAllocMap[1] =
           ((rbAllocInfo->allocInfo.raType0.dlAllocBitMask >> 16)
               & 0x00ff);
        pdcch->dci.u.format2Info.allocInfo.resAllocMap[2] =
               ((rbAllocInfo->allocInfo.raType0.dlAllocBitMask >> 8)
                & 0x0000ff);
        pdcch->dci.u.format2Info.allocInfo.resAllocMap[3] =
               ((rbAllocInfo->allocInfo.raType0.dlAllocBitMask & 0x000000ff));
#ifdef RG_UNUSED
    }
#endif
#ifdef LTEMAC_SPS
    if ((!(hqP->tbInfo[0].txCntr)) &&
       ( cmnHqDl != (RgSchCmnDlHqProc*)NULLP  &&
         ((cmnHqDl->spsAction & RG_SCH_CMN_SPS_DL_ACTV) ||
         (cmnHqDl->spsAction & RG_SCH_CMN_SPS_DL_REACTV))
       ))
    {
       pdcch->dci.u.format2Info.allocInfo.harqProcId = 0;
    }
    else
    {
      pdcch->dci.u.format2Info.allocInfo.harqProcId = hqP->procId;
    }
#else
     pdcch->dci.u.format2Info.allocInfo.harqProcId = hqP->procId;
#endif
         /* Initialize the TB info for both the TBs */
     pdcch->dci.u.format2Info.allocInfo.tbInfo[0].mcs = 0;
     pdcch->dci.u.format2Info.allocInfo.tbInfo[0].rv  = 1;
     pdcch->dci.u.format2Info.allocInfo.tbInfo[0].ndi = 0;
     pdcch->dci.u.format2Info.allocInfo.tbInfo[1].mcs = 0;
     pdcch->dci.u.format2Info.allocInfo.tbInfo[1].rv  = 1;
     pdcch->dci.u.format2Info.allocInfo.tbInfo[1].ndi = 0;

         /* Fill tbInfo for scheduled TBs */
     pdcch->dci.u.format2Info.allocInfo.tbInfo[rbAllocInfo->tbInfo[0].\
        tbCb->tbIdx].ndi = rbAllocInfo->tbInfo[0].tbCb->ndi;
     pdcch->dci.u.format2Info.allocInfo.tbInfo[rbAllocInfo->tbInfo[0].\
        tbCb->tbIdx].mcs = rbAllocInfo->tbInfo[0].imcs;
     pdcch->dci.u.format2Info.allocInfo.tbInfo[rbAllocInfo->tbInfo[0].\
            tbCb->tbIdx].rv = rbAllocInfo->tbInfo[0].tbCb->dlGrnt.rv;
          /* If we reach this function. It is safely assumed that
           *  rbAllocInfo->tbInfo[0] always has non default valid values.
           *  rbAllocInfo->tbInfo[1]'s scheduling is optional */
     if (rbAllocInfo->tbInfo[1].schdlngForTb == TRUE)
     {
            pdcch->dci.u.format2Info.allocInfo.tbInfo[rbAllocInfo->tbInfo[1].\
                tbCb->tbIdx].ndi = rbAllocInfo->tbInfo[1].tbCb->ndi;
            pdcch->dci.u.format2Info.allocInfo.tbInfo[rbAllocInfo->tbInfo[1].\
                tbCb->tbIdx].mcs = rbAllocInfo->tbInfo[1].imcs;
            pdcch->dci.u.format2Info.allocInfo.tbInfo[rbAllocInfo->tbInfo[1].\
                tbCb->tbIdx].rv = rbAllocInfo->tbInfo[1].tbCb->dlGrnt.rv;
     }

     pdcch->dci.u.format2Info.allocInfo.transSwap =
             rbAllocInfo->mimoAllocInfo.swpFlg;
     pdcch->dci.u.format2Info.allocInfo.precoding =
             rbAllocInfo->mimoAllocInfo.precIdxInfo;
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
                    "PDCCH is been scheduled without updating anInfo RNTI:%d",
                    rbAllocInfo->rnti);
        }
#endif
     }
#endif

     RETVOID;
}
/**
 * @brief This function fills the PDCCH DCI format 2A information from dlProc.
 *
 * @details
 *
 *     Function: rgSCHCmnFillHqPPdcchDciFrmt2A
 *     Purpose:  This function fills in the PDCCH information
 *               obtained from the RgSchDlHqProcCb and RgSchDlRbAlloc
 *               for dedicated service scheduling. It also
 *               obtains TPC to be filled in from the power module.
 *               Assign the PDCCH to HQProc.
 *
 *     Invoked by: Downlink Scheduler
 *
 *  @param[in]  RgSchCellCb*      cell
 *  @param[in]  RgSchDlRbAlloc*   rbAllocInfo
 *  @param[in]  RgDlHqProc*       hqP
 *  @param[out]  RgSchPdcch        *pdcch
 *  @param[in]   U8               tpc
 *  @return  Void
 *
 **/
#ifdef ANSI
PRIVATE Void rgSCHCmnFillHqPPdcchDciFrmt2A
(
RgSchCellCb                *cell,
RgSchDlRbAlloc             *rbAllocInfo,
RgSchDlHqProcCb            *hqP,
RgSchPdcch                 *pdcch,
U8                         tpc
)
#else
PRIVATE Void rgSCHCmnFillHqPPdcchDciFrmt2A(cell, rbAllocInfo, hqP, pdcch, tpc)
RgSchCellCb                *cell;
RgSchDlRbAlloc             *rbAllocInfo;
RgSchDlHqProcCb            *hqP;
RgSchPdcch                 *pdcch;
U8                         tpc;
#endif
{
#ifdef LTE_TDD
   RgSchTddANInfo     *anInfo;
#endif

#ifdef LTEMAC_SPS
   RgSchCmnDlHqProc *cmnHqDl = RG_SCH_CMN_GET_DL_HQP(hqP);
#endif

    TRC2(rgSCHCmnFillHqPPdcchDciFrmt2A)

    rgSCHCmnFillHqPTb(cell, rbAllocInfo, 0, pdcch);
    /*ccpu00120365:-ADD-call also if tb is disabled */
    if (rbAllocInfo->tbInfo[1].schdlngForTb ||
          rbAllocInfo->tbInfo[1].isDisabled)
    {

        rgSCHCmnFillHqPTb(cell, rbAllocInfo, 1, pdcch);
    }

    pdcch->dci.u.format2AInfo.tpcCmd = tpc;
         /* Avoiding this check,as we dont support Type1 RA */
#ifdef RG_UNUSED
    if (rbAllocInfo->raType == RG_SCH_CMN_RA_TYPE0)
    {
#endif
        pdcch->dci.u.format2AInfo.allocInfo.isAllocType0 = TRUE;
        pdcch->dci.u.format2AInfo.allocInfo.resAllocMap[0] =
              ((rbAllocInfo->allocInfo.raType0.dlAllocBitMask >> 24)
               & 0xff);
        pdcch->dci.u.format2AInfo.allocInfo.resAllocMap[1] =
              ((rbAllocInfo->allocInfo.raType0.dlAllocBitMask >> 16)
               & 0x00ff);
        pdcch->dci.u.format2AInfo.allocInfo.resAllocMap[2] =
               ((rbAllocInfo->allocInfo.raType0.dlAllocBitMask >> 8)
                & 0x0000ff);
        pdcch->dci.u.format2AInfo.allocInfo.resAllocMap[3] =
               ((rbAllocInfo->allocInfo.raType0.dlAllocBitMask & 0x000000ff));
#ifdef RG_UNUSED
    }
#endif
#ifdef LTEMAC_SPS
    if ((!(hqP->tbInfo[0].txCntr)) &&
       ( cmnHqDl != (RgSchCmnDlHqProc*)NULLP  &&
         ((cmnHqDl->spsAction & RG_SCH_CMN_SPS_DL_ACTV) ||
         (cmnHqDl->spsAction & RG_SCH_CMN_SPS_DL_REACTV))
       ))
    {
       pdcch->dci.u.format2AInfo.allocInfo.harqProcId = 0;
    }
    else
    {
      pdcch->dci.u.format2AInfo.allocInfo.harqProcId = hqP->procId;
    }
#else
    pdcch->dci.u.format2AInfo.allocInfo.harqProcId = hqP->procId;
#endif
         /* Initialize the TB info for both the TBs */
    pdcch->dci.u.format2AInfo.allocInfo.tbInfo[0].mcs = 0;
    pdcch->dci.u.format2AInfo.allocInfo.tbInfo[0].rv  = 1;
    pdcch->dci.u.format2AInfo.allocInfo.tbInfo[1].mcs = 0;
    pdcch->dci.u.format2AInfo.allocInfo.tbInfo[1].rv  = 1;
         /* Fill tbInfo for scheduled TBs */
    pdcch->dci.u.format2AInfo.allocInfo.tbInfo[rbAllocInfo->tbInfo[0].\
            tbCb->tbIdx].ndi = rbAllocInfo->tbInfo[0].tbCb->ndi;
    pdcch->dci.u.format2AInfo.allocInfo.tbInfo[rbAllocInfo->tbInfo[0].\
            tbCb->tbIdx].mcs = rbAllocInfo->tbInfo[0].imcs;
    pdcch->dci.u.format2AInfo.allocInfo.tbInfo[rbAllocInfo->tbInfo[0].\
            tbCb->tbIdx].rv = rbAllocInfo->tbInfo[0].tbCb->dlGrnt.rv;
         /* If we reach this function. It is safely assumed that
          *  rbAllocInfo->tbInfo[0] always has non default valid values.
          *  rbAllocInfo->tbInfo[1]'s scheduling is optional */

    if (rbAllocInfo->tbInfo[1].schdlngForTb == TRUE)
    {
            pdcch->dci.u.format2AInfo.allocInfo.tbInfo[rbAllocInfo->tbInfo[1].\
               tbCb->tbIdx].ndi = rbAllocInfo->tbInfo[1].tbCb->ndi;
            pdcch->dci.u.format2AInfo.allocInfo.tbInfo[rbAllocInfo->tbInfo[1].\
               tbCb->tbIdx].mcs = rbAllocInfo->tbInfo[1].imcs;
            pdcch->dci.u.format2AInfo.allocInfo.tbInfo[rbAllocInfo->tbInfo[1].\
               tbCb->tbIdx].rv = rbAllocInfo->tbInfo[1].tbCb->dlGrnt.rv;

    }
    pdcch->dci.u.format2AInfo.allocInfo.transSwap =
            rbAllocInfo->mimoAllocInfo.swpFlg;
    pdcch->dci.u.format2AInfo.allocInfo.precoding =
            rbAllocInfo->mimoAllocInfo.precIdxInfo;
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
                   "PDCCH is been scheduled without updating anInfo RNTI:%d",
                   rbAllocInfo->rnti);
       }
#endif
     }
#endif


    RETVOID;
}

/**
 * @brief init of Sch vars.
 *
 * @details
 *
 *     Function: rgSCHCmnInitVars
       Purpose:  Initialization of various UL subframe indices
 *
 *  @param[in]  RgSchCellCb *cell
 *  @return  Void
 *
 **/
#ifdef ANSI
PRIVATE Void rgSCHCmnInitVars
(
RgSchCellCb *cell
)
#else
PRIVATE Void rgSCHCmnInitVars(cell)
RgSchCellCb *cell;
#endif
{
   RgSchCmnUlCell  *cellUl = RG_SCH_CMN_GET_UL_CELL(cell);

   TRC2(rgSCHCmnInitVars);

   cellUl->idx         = RGSCH_INVALID_INFO;
   cellUl->schdIdx     = RGSCH_INVALID_INFO;
   cellUl->schdHqProcIdx = RGSCH_INVALID_INFO;
   cellUl->msg3SchdIdx = RGSCH_INVALID_INFO;
   cellUl->msg3SchdHqProcIdx = RGSCH_INVALID_INFO;
   cellUl->rcpReqIdx   = RGSCH_INVALID_INFO;
   cellUl->hqFdbkIdx[0] = RGSCH_INVALID_INFO;
   cellUl->hqFdbkIdx[1] = RGSCH_INVALID_INFO;
   cellUl->reTxIdx[0]   = RGSCH_INVALID_INFO;
   cellUl->reTxIdx[1]   = RGSCH_INVALID_INFO;
  /* Stack Crash problem for TRACE5 Changes. Added the return below */
  RETVOID;

}

#ifndef LTE_TDD
/**
 * @brief Updation of Sch vars per TTI.
 *
 * @details
 *
 *     Function: rgSCHCmnUpdVars
 *     Purpose:  Updation of Sch vars per TTI.
 *
 *  @param[in]  RgSchCellCb *cell
 *  @return  Void
 *
 **/
#ifdef ANSI
PUBLIC Void rgSCHCmnUpdVars
(
RgSchCellCb *cell
)
#else
PUBLIC Void rgSCHCmnUpdVars(cell)
RgSchCellCb *cell;
#endif
{
   CmLteTimingInfo   timeInfo;
   RgSchCmnUlCell  *cellUl = RG_SCH_CMN_GET_UL_CELL(cell);
   U16 idx;

   TRC2(rgSCHCmnUpdVars);

   idx = (cell->crntTime.sfn * RGSCH_NUM_SUB_FRAMES + cell->crntTime.subframe);
   cellUl->idx     = ((idx) % (RG_SCH_CMN_UL_NUM_SF));

   /* Need to scheduler for after SCHED_DELTA */
   /* UL allocation has been advanced by 1 subframe
    * so that we do not wrap around and send feedback
    * before the data is even received by the PHY */
   /* Introduced timing delta for UL control */
   idx = (cellUl->idx + TFU_ULCNTRL_DLDELTA + RGSCH_PDCCH_PUSCH_DELTA);
   cellUl->schdIdx     = ((idx) % (RG_SCH_CMN_UL_NUM_SF));

   RGSCHCMNADDTOCRNTTIME(cell->crntTime,timeInfo,
            TFU_ULCNTRL_DLDELTA + RGSCH_PDCCH_PUSCH_DELTA)
   cellUl->schdHqProcIdx = rgSCHCmnGetUlHqProcIdx(&timeInfo, cell);

   /* ccpu00127193 filling schdTime for logging and enhancement purpose*/
   cellUl->schdTime = timeInfo;

   /* msg3 scheduling two subframes after general scheduling */
   idx = (cellUl->idx + RG_SCH_CMN_DL_DELTA + RGSCH_RARSP_MSG3_DELTA);
   cellUl->msg3SchdIdx = ((idx) % (RG_SCH_CMN_UL_NUM_SF));

   RGSCHCMNADDTOCRNTTIME(cell->crntTime,timeInfo,
            RG_SCH_CMN_DL_DELTA+ RGSCH_RARSP_MSG3_DELTA)
   cellUl->msg3SchdHqProcIdx = rgSCHCmnGetUlHqProcIdx(&timeInfo, cell);

   /* spsUlRsrvIdx is not being used now (for FDD).
    * For FDD was causing TCs to fail. */
#ifdef LTEMAC_SPS
   RGSCHCMNADDTOCRNTTIME(cell->crntTime,timeInfo,
            RG_SCH_CMN_DL_DELTA+ RGSCH_RARSP_MSG3_DELTA + 1);
   cellUl->spsUlRsrvIdx       = (cellUl->msg3SchdIdx+1) % RG_SCH_CMN_UL_NUM_SF;
   cellUl->spsUlRsrvHqProcIdx = cellUl->schdHqProcIdx;
#endif

   idx = (cellUl->idx + TFU_RECPREQ_DLDELTA);

   cellUl->rcpReqIdx   = ((idx) % (RG_SCH_CMN_UL_NUM_SF));

   /* Downlink harq feedback is sometime after data reception / harq failure */
   /* Since feedback happens prior to scheduling being called, we add 1 to   */
   /* take care of getting the correct subframe for feedback                 */
#ifdef RG_ULSCHED_AT_CRC
   idx = (cellUl->idx - TFU_CRCIND_ULDELTA + RG_SCH_CMN_UL_NUM_SF);
#else
   idx = (cellUl->idx - TFU_CRCIND_ULDELTA + 1 + RG_SCH_CMN_UL_NUM_SF);
#endif
   cellUl->hqFdbkIdx[0]   = (idx % (RG_SCH_CMN_UL_NUM_SF));

#ifdef RG_ULSCHED_AT_CRC
   idx = ((cellUl->schdIdx) % (RG_SCH_CMN_UL_NUM_SF));
#else
   idx = ((cellUl->schdIdx + 1) % (RG_SCH_CMN_UL_NUM_SF));
#endif

   cellUl->reTxIdx[0] = (U8) idx;

   /* RACHO: update cmn sched specific RACH variables,
    * mainly the prachMaskIndex */
   rgSCHCmnUpdRachParam(cell);

   RETVOID;
}
#endif

#ifdef LTE_TDD

/**
 * @brief To get uplink subframe index associated with current PHICH
 *        transmission.
 *
 * @details
 *
 *     Function: rgSCHCmnGetPhichUlSfIdx
 *     Purpose:  Gets uplink subframe index associated with current PHICH
 *               transmission based on SFN and subframe no
 *
 *  @param[in]  CmLteTimingInfo  *timeInfo
 *  @param[in]  RgSchCellCb              *cell
 *  @return U8
 *
 **/
#ifdef ANSI
PUBLIC U8  rgSCHCmnGetPhichUlSfIdx
(
CmLteTimingInfo *timeInfo,
RgSchCellCb *cell
)
#else
PUBLIC U8  rgSCHCmnGetPhichUlSfIdx(timeInfo, cell)
CmLteTimingInfo *timeInfo;
RgSchCellCb        *cell;
#endif
{
   RgSchCmnUlCell       *cellUl = RG_SCH_CMN_GET_UL_CELL(cell);
   RgSchDlSf            *dlsf;
   U8                   ulDlCfgIdx = cell->ulDlCfgIdx;
   U8                   idx;
   U16                  numUlSf;
   U16                  sfn;
   U8                   subframe;

   TRC2(rgSCHCmnGetPhichUlSfIdx);

   dlsf = rgSCHUtlSubFrmGet(cell, *timeInfo);

   if(dlsf->phichOffInfo.sfnOffset == RGSCH_INVALID_INFO)
   {
      RETVALUE(RGSCH_INVALID_INFO);
   }
   subframe = dlsf->phichOffInfo.subframe;

   sfn = (RGSCH_MAX_SFN + timeInfo->sfn -
                   dlsf->phichOffInfo.sfnOffset) % RGSCH_MAX_SFN;

   /* ccpu00130980: numUlSf(U16) parameter added to avoid integer
    * wrap case such that idx will be proper*/
   numUlSf = rgSchTddNumUlSubfrmTbl[ulDlCfgIdx][RGSCH_NUM_SUB_FRAMES-1];
   numUlSf = ((numUlSf * sfn) + rgSchTddNumUlSubfrmTbl[ulDlCfgIdx][subframe]) - 1;
   idx = numUlSf % (cellUl->numUlSubfrms);

   RETVALUE(idx);
}

/**
 * @brief To get uplink subframe index.
 *
 * @details
 *
 *
 *     Function: rgSCHCmnGetUlSfIdx
 *     Purpose:  Gets uplink subframe index based on SFN and subframe number.
 *
 *  @param[in]  CmLteTimingInfo  *timeInfo
 *  @param[in]  U8               ulDlCfgIdx
 *  @return U8
 *
 **/
#ifdef ANSI
PUBLIC U8  rgSCHCmnGetUlSfIdx
(
CmLteTimingInfo *timeInfo,
RgSchCellCb *cell
)
#else
PUBLIC U8  rgSCHCmnGetUlSfIdx(timeInfo, cell)
CmLteTimingInfo *timeInfo;
RgSchCellCb *cell;
#endif
{
   RgSchCmnUlCell    *cellUl = RG_SCH_CMN_GET_UL_CELL(cell);
   U8                ulDlCfgIdx = cell->ulDlCfgIdx;
   U8                idx = 0;
   U16               numUlSf;

   TRC2(rgSCHCmnGetUlSfIdx);

   /* ccpu00130980: numUlSf(U16) parameter added to avoid integer
    * wrap case such that idx will be proper*/
   numUlSf = rgSchTddNumUlSubfrmTbl[ulDlCfgIdx][RGSCH_NUM_SUB_FRAMES-1];
   numUlSf = ((numUlSf * timeInfo->sfn) + \
         rgSchTddNumUlSubfrmTbl[ulDlCfgIdx][timeInfo->subframe]) - 1;
   idx = numUlSf % (cellUl->numUlSubfrms);

   RETVALUE(idx);
}

#endif

/**
 * @brief To get uplink hq index.
 *
 * @details
 *
 *
 *     Function: rgSCHCmnGetUlHqProcIdx
 *     Purpose:  Gets uplink subframe index based on SFN and subframe number.
 *
 *  @param[in]  CmLteTimingInfo  *timeInfo
 *  @param[in]  U8               ulDlCfgIdx
 *  @return U8
 *
 **/
#ifdef ANSI
PUBLIC U8  rgSCHCmnGetUlHqProcIdx
(
CmLteTimingInfo *timeInfo,
RgSchCellCb *cell
)
#else
PUBLIC U8  rgSCHCmnGetUlHqProcIdx(timeInfo, cell)
CmLteTimingInfo *timeInfo;
RgSchCellCb *cell;
#endif
{
   U8            procId;
   U32           numUlSf;
  
#ifndef LTE_TDD
   numUlSf  = (timeInfo->sfn * RGSCH_NUM_SUB_FRAMES + timeInfo->subframe);
   procId   = numUlSf % RGSCH_NUM_UL_HQ_PROC;
#else
   U8            ulDlCfgIdx = cell->ulDlCfgIdx;
   /*ccpu00130639 - MOD - To get correct UL HARQ Proc IDs for all UL/DL Configs*/
   U8            numUlSfInSfn;
   S8            sfnCycle = cell->tddHqSfnCycle;
   U8            numUlHarq = rgSchTddUlNumHarqProcTbl[ulDlCfgIdx]

   /* TRACE 5 Changes */
   TRC2(rgSCHCmnGetUlHqProcIdx);

   /* Calculate the number of UL SF in one SFN */
   numUlSfInSfn = RGSCH_NUM_SUB_FRAMES -
               rgSchTddNumDlSubfrmTbl[ulDlCfgIdx][RGSCH_NUM_SUB_FRAMES-1];

   /* Check for the SFN wrap around case */
   if(cell->crntTime.sfn == 1023 && timeInfo->sfn == 0)
   {
      sfnCycle++;
   }
   else if(cell->crntTime.sfn == 0 && timeInfo->sfn == 1023)
   {
      /* sfnCycle decremented by 1 */
      sfnCycle = (sfnCycle + numUlHarq-1) % numUlHarq;
   }
   /* Calculate the total number of UL sf */
   /*  -1 is done since uplink sf are counted from 0 */
   numUlSf = numUlSfInSfn *  (timeInfo->sfn + (sfnCycle*1024)) +
                  rgSchTddNumUlSubfrmTbl[ulDlCfgIdx][timeInfo->subframe] - 1;

   procId = numUlSf % numUlHarq;   
#endif
   RETVALUE(procId);
}


/* UL_ALLOC_CHANGES */
/***********************************************************
 *
 *     Func : rgSCHCmnUlFreeAlloc
 *
 *     Desc : Free an allocation - invokes UHM and releases
 *            alloc for the scheduler
 *            Doest need subframe as argument
 *
 *     Ret  :
 *
 *     Notes:
 *
 *     File :
 *
 **********************************************************/
#ifdef ANSI
PUBLIC Void rgSCHCmnUlFreeAlloc
(
RgSchCellCb     *cell,
RgSchUlAlloc    *alloc
)
#else
PUBLIC Void rgSCHCmnUlFreeAlloc(cell, alloc)
RgSchCellCb     *cell;
RgSchUlAlloc    *alloc;
#endif
{
   RgSchUlHqProcCb *hqProc;
   TRC2(rgSCHCmnUlFreeAllocation);

   if (alloc->forMsg3)
   {
      /* Fix : Release RNTI upon MSG3 max TX failure for non-HO UEs */
      if ((alloc->hqProc->remTx == 0) &&
          (alloc->hqProc->rcvdCrcInd == FALSE) &&
          (alloc->raCb))
      {
         RgSchRaCb      *raCb = alloc->raCb;
#ifdef LTE_L2_MEAS
         rgSCHUhmFreeProc(alloc->hqProc, cell);
#else
         rgSCHUhmFreeProc(alloc->hqProc);
#endif
         rgSCHUtlUlAllocRelease(alloc);
         rgSCHRamDelRaCb(cell, raCb, TRUE);
         RETVOID;
      }
   }
   
   hqProc = alloc->hqProc;
   rgSCHUtlUlAllocRelease(alloc);
#ifdef LTE_L2_MEAS
   rgSCHUhmFreeProc(hqProc, cell);
#else
   rgSCHUhmFreeProc(hqProc);
#endif
   RETVOID;
}


/***********************************************************
 *
 *     Func : rgSCHCmnUlFreeAllocation
 *
 *     Desc : Free an allocation - invokes UHM and releases
 *            alloc for the scheduler
 *
 *     Ret  :
 *
 *     Notes:
 *
 *     File :
 *
 **********************************************************/
#ifdef ANSI
PUBLIC Void rgSCHCmnUlFreeAllocation
(
RgSchCellCb     *cell,
RgSchUlSf       *sf,
RgSchUlAlloc    *alloc
)
#else
PUBLIC Void rgSCHCmnUlFreeAllocation(cell, sf, alloc)
RgSchCellCb     *cell;
RgSchUlSf       *sf;
RgSchUlAlloc    *alloc;
#endif
{
   RgSchUlHqProcCb *hqProc;

   TRC2(rgSCHCmnUlFreeAllocation);

   if (alloc->forMsg3)
   {
      /* Fix : Release RNTI upon MSG3 max TX failure for non-HO UEs */
      if ((alloc->hqProc->remTx == 0) &&
          (alloc->hqProc->rcvdCrcInd == FALSE) &&
          (alloc->raCb))
      {
         RgSchRaCb      *raCb = alloc->raCb;
#ifdef LTE_L2_MEAS
         rgSCHUhmFreeProc(alloc->hqProc, cell);
#else
         rgSCHUhmFreeProc(alloc->hqProc);
#endif
         rgSCHUtlUlAllocRls(sf, alloc);
         rgSCHRamDelRaCb(cell, raCb, TRUE);
         RETVOID;
      }
   }
   
   hqProc = alloc->hqProc;
#ifdef LTE_L2_MEAS
   rgSCHUhmFreeProc(hqProc, cell);
   /* re-setting the PRB count while freeing the allocations */
   sf->totPrb = 0;

#else
   rgSCHUhmFreeProc(hqProc);
#endif
   rgSCHUtlUlAllocRls(sf, alloc);

   RETVOID;
}

/**
 * @brief This function implements PDCCH allocation for an UE
 *        in the currently running subframe.
 *
 * @details
 *
 *     Function: rgSCHCmnPdcchAllocCrntSf
 *     Purpose:  This function determines current DL subframe
 *               and UE DL CQI to call the actual pdcch allocator
 *               function.
 *               Note that this function is called only
 *               when PDCCH request needs to be made during
 *               uplink scheduling.
 *
 *     Invoked by: Scheduler
 *
 *  @param[in]  RgSchCellCb  *cell
 *  @param[in]  RgSchUeCb    *ue
 *  @return  RgSchPdcch *
 *         -# NULLP when unsuccessful
 **/
#ifdef ANSI
PUBLIC RgSchPdcch *rgSCHCmnPdcchAllocCrntSf
(
RgSchCellCb                *cell,
RgSchUeCb                  *ue
)
#else
PUBLIC RgSchPdcch *rgSCHCmnPdcchAllocCrntSf(cell, ue)
RgSchCellCb                *cell;
RgSchUeCb                  *ue;
#endif
{
   CmLteTimingInfo      frm = cell->crntTime;
   RgSchCmnDlUe         *ueDl = RG_SCH_CMN_GET_DL_UE(ue);
   RgSchDlSf            *sf;
   U32                  y;

   TRC2(rgSCHCmnPdcchAllocCrntSf);
   RGSCH_INCR_SUB_FRAME(frm, TFU_ULCNTRL_DLDELTA);
   sf = rgSCHUtlSubFrmGet(cell, frm);

   y = rgSCHUtlGetY(ue, sf->sfNum);

   RETVALUE(rgSCHCmnPdcchAlloc(cell, sf, y, ueDl->mimoInfo.cwInfo[0].cqi, TFU_DCI_FORMAT_0, FALSE));
}

/***********************************************************
 *
 *     Func : rgSCHCmnUlAllocFillNdmrs
 *
 *     Desc : Determines and fills N_dmrs for a UE uplink
 *            allocation.
 *
 *     Ret  :
 *
 *     Notes: N_dmrs determination is straightforward, so
 *            it is configured per subband
 *
 *     File :
 *
 **********************************************************/
#ifdef ANSI
PUBLIC Void rgSCHCmnUlAllocFillNdmrs
(
RgSchCmnUlCell *cellUl,
RgSchUlAlloc   *alloc
)
#else
PUBLIC Void rgSCHCmnUlAllocFillNdmrs(cellUl, alloc)
RgSchCmnUlCell *cellUl;
RgSchUlAlloc   *alloc;
#endif
{
   TRC2(rgSCHCmnUlAllocFillNdmrs);
   alloc->grnt.nDmrs = cellUl->dmrsArr[alloc->sbStart];
   RETVOID;
}

/***********************************************************
 *
 *     Func : rgSCHCmnUlAllocLnkHqProc
 *
 *     Desc : Links a new allocation for an UE with the
 *            appropriate HARQ process of the UE.
 *
 *     Ret  :
 *
 *     Notes:
 *
 *     File :
 *
 **********************************************************/
#ifdef ANSI
PUBLIC Void rgSCHCmnUlAllocLnkHqProc
(
RgSchUeCb       *ue,
RgSchUlAlloc    *alloc,
RgSchUlHqProcCb *proc,
Bool            isRetx
)
#else
PUBLIC Void rgSCHCmnUlAllocLnkHqProc(ue, alloc, proc, isRetx)
RgSchUeCb       *ue;
RgSchUlAlloc    *alloc;
RgSchUlHqProcCb *proc;
Bool            isRetx;
#endif
{
   TRC2(rgSCHCmnUlAllocLnkHqProc);

   if(TRUE == isRetx)
   {
      rgSCHCmnUlAdapRetx(alloc, proc);
   }
   else
   {
#ifdef LTE_L2_MEAS /* L2_COUNTERS */
      alloc->ue = ue;
#endif
      rgSCHUhmNewTx(proc, ue->ul.hqEnt.maxHqRetx, alloc);
   }
   RETVOID;
}

/**
 * @brief This function releases a PDCCH in the subframe that is
 *        currently being allocated for.
 *
 * @details
 *
 *     Function: rgSCHCmnPdcchRlsCrntSf
 *     Purpose:  This function determines current DL subframe
 *               which is considered for PDCCH allocation,
 *               and then calls the actual function that
 *               releases a PDCCH in a specific subframe.
 *               Note that this function is called only
 *               when PDCCH release needs to be made during
 *               uplink scheduling.
 *
 *     Invoked by: Scheduler
 *
 *  @param[in]  RgSchCellCb  *cell
 *  @param[in]  RgSchPdcch   *pdcch
 *  @return  Void
 **/
#ifdef ANSI
PUBLIC Void rgSCHCmnPdcchRlsCrntSf
(
RgSchCellCb                *cell,
RgSchPdcch                 *pdcch
)
#else
PUBLIC Void rgSCHCmnPdcchRlsCrntSf(cell, pdcch)
RgSchCellCb                *cell;
RgSchPdcch                 *pdcch;
#endif
{
   CmLteTimingInfo      frm = cell->crntTime;
   RgSchDlSf               *sf;

   TRC2(rgSCHCmnPdcchRlsCrntSf);

   RGSCH_INCR_SUB_FRAME(frm, TFU_ULCNTRL_DLDELTA);
   sf = rgSCHUtlSubFrmGet(cell, frm);
   rgSCHUtlPdcchPut(cell, &sf->pdcchInfo, pdcch);
   RETVOID;
}
/***********************************************************
 *
 *     Func : rgSCHCmnUlFillPdcchWithAlloc
 *
 *     Desc : Fills a PDCCH with format 0 information.
 *
 *     Ret  :
 *
 *     Notes:
 *
 *     File :
 *
 **********************************************************/
#ifdef ANSI
PUBLIC Void rgSCHCmnUlFillPdcchWithAlloc
(
RgSchPdcch      *pdcch,
RgSchUlAlloc    *alloc,
RgSchUeCb       *ue
)
#else
PUBLIC Void rgSCHCmnUlFillPdcchWithAlloc(pdcch, alloc, ue)
RgSchPdcch      *pdcch;
RgSchUlAlloc    *alloc;
RgSchUeCb       *ue;
#endif
{
#ifdef LTE_TDD
   RgSchTddANInfo    *anInfo;
   RgSchDlSf         *dlsf;
   /*ccpu00118273- DEL - removed assignment */
   CmLteTimingInfo   timeInfo;
#endif

   TRC2(rgSCHCmnUlFillPdcchWithAlloc);

   pdcch->rnti                           = alloc->rnti;
   pdcch->dci.dciFormat                  = TFU_DCI_FORMAT_0;
   pdcch->dci.u.format0Info.hoppingEnbld = alloc->grnt.hop;
   pdcch->dci.u.format0Info.rbStart      = alloc->grnt.rbStart;
   pdcch->dci.u.format0Info.numRb        = alloc->grnt.numRb;
   pdcch->dci.u.format0Info.mcs          = alloc->grnt.iMcsCrnt;
   pdcch->dci.u.format0Info.ndi          = alloc->hqProc->ndi;
   pdcch->dci.u.format0Info.nDmrs        = alloc->grnt.nDmrs;
   pdcch->dci.u.format0Info.tpcCmd       = alloc->grnt.tpc;

   /* SR_RACH_STATS  : SR*/

   pdcch->dci.u.format0Info.isSrGrant = ue->isSrGrant;      


   /* SR_RACH_STATS  : PDCCH for CRNTI Based contention Resolution*/
   pdcch->dci.u.format0Info.isMsg4PdcchWithCrnti = ue->isMsg4PdcchWithCrnti;
   ue->isMsg4PdcchWithCrnti = FALSE;

#ifdef TFU_UPGRADE
   /* If APER CQI is not cfgd, ue->dl.reqforCqi will
    * always be FALSE */
   if (ue->dl.acqiResGrntd)
   {
      pdcch->dci.u.format0Info.cqiReq = TRUE;
      /* Indicating aper Cqi has been requested */
      ue->dl.acqiResGrntd = FALSE;
      ue->dl.reqForCqi = FALSE;
   }
   else
   {
      pdcch->dci.u.format0Info.cqiReq = 0;
   }
#else
   pdcch->dci.u.format0Info.cqiReq = 0;
#endif

   /*ccpu00116502- Send HqProc ID*/
   pdcch->dci.u.format0Info.hqProcId     = alloc->hqProc->procId;
#ifdef TFU_UPGRADE
   if (ue->ul.ulTxAntSel.selType == RGR_UE_TX_ANT_CLOSEDLOOP)
   {
      pdcch->dci.u.format0Info.txAntenna = ue->validTxAnt;
   }
   /* [ccpu00139245]-ADD-case of Open loop initialize to 0 */ 
   else
   {
      pdcch->dci.u.format0Info.txAntenna = 0;
   }
#endif
#ifdef LTE_TDD
   /* Adjust with the scheduled time */
   /*ccpu00118273- MOD - replaced timeInfo.Subframe adjustment with ue->cell
    * adjustment  */
   /* ccpu00130400-MOD: corrected TFU delta for DL */
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
      //RLOG_ARG0(L_ERROR,DBG_INSTID,inst,  "UL Asc Subframe is Invalid");
      RETVOID;
   }
#ifdef TFU_TDD
   if(ue->cell->ulDlCfgIdx == 0)
   {
      pdcch->dci.u.format0Info.ulIdx = RG_SCH_ULIDX_MSB;
   }
   else
   {
      pdcch->dci.u.format0Info.ulIdx = 0;
      anInfo = rgSCHUtlGetUeANFdbkInfo(ue, &timeInfo);
      if(anInfo != NULLP)
      {
         pdcch->dci.u.format0Info.dai = RG_SCH_GET_DAI_VALUE(anInfo->ulDai);
      }
      else
      {
         /* Set the no of DL subframes to zero with  DAI=4 */
         pdcch->dci.u.format0Info.dai = RG_SCH_MAX_DAI_IDX;
      }
   }
#endif
#endif
   RETVOID;
}

/***********************************************************
 *
 *     Func : rgSCHCmnUlAllocFillTpc
 *
 *     Desc : Determines and fills TPC for an UE allocation.
 *
 *     Ret  :
 *
 *     Notes:
 *
 *     File :
 *
 **********************************************************/
#ifdef ANSI
PUBLIC Void rgSCHCmnUlAllocFillTpc
(
RgSchCellCb  *cell,
RgSchUeCb    *ue,
RgSchUlAlloc *alloc
)
#else
PUBLIC Void rgSCHCmnUlAllocFillTpc(cell, ue, alloc)
RgSchCellCb  *cell;
RgSchUeCb    *ue;
RgSchUlAlloc *alloc;
#endif
{
   TRC2(rgSCHCmnUlAllocFillTpc);
   alloc->grnt.tpc = rgSCHPwrPuschTpcForUe(cell, ue);
   RETVOID;
}


/***********************************************************
 *
 *     Func : rgSCHCmnAddUeToRefreshQ
 *
 *     Desc : Adds a UE to refresh queue, so that the UE is
 *            periodically triggered to refresh it's GBR and
 *            AMBR values.
 *
 *     Ret  :
 *
 *     Notes:
 *
 *     File :
 *
 **********************************************************/
#ifdef ANSI
PRIVATE Void rgSCHCmnAddUeToRefreshQ
(
RgSchCellCb     *cell,
RgSchUeCb       *ue,
U32             wait
)
#else
PRIVATE Void rgSCHCmnAddUeToRefreshQ(cell, ue, wait)
RgSchCellCb     *cell;
RgSchUeCb       *ue;
U32             wait;
#endif
{
   RgSchCmnCell   *sched  = RG_SCH_CMN_GET_CELL(cell);
   CmTmrArg       arg;
   RgSchCmnUeInfo *ueSchd = RG_SCH_CMN_GET_CMN_UE(ue);

   TRC2(rgSCHCmnAddUeToRefreshQ);
   UNUSED(cell);

   cmMemset((U8 *)&arg, 0, sizeof(arg));
   arg.tqCp   = &sched->tmrTqCp;
   arg.tq     = sched->tmrTq;
   arg.timers = &ueSchd->tmr;
   arg.cb     = (PTR)ue;
   arg.tNum   = 0;
   arg.max    = 1;
   arg.evnt   = RG_SCH_CMN_EVNT_UE_REFRESH;
   arg.wait   = wait;
   cmPlcCbTq(&arg);
   RETVOID;
}

/**
 * @brief Perform UE reset procedure.
 *
 * @details
 *
 *     Function : rgSCHCmnUlUeReset
 *
 *     This functions performs BSR resetting and
 *     triggers UL specific scheduler
 *     to Perform UE reset procedure.
 *
 *  @param[in]  RgSchCellCb  *cell
 *  @param[in]  RgSchUeCb    *ue
 *  @return  Void
 **/
#ifdef ANSI
PRIVATE Void rgSCHCmnUlUeReset
(
RgSchCellCb  *cell,
RgSchUeCb    *ue
)
#else
PRIVATE Void rgSCHCmnUlUeReset(cell, ue)
RgSchCellCb  *cell;
RgSchUeCb    *ue;
#endif
{
   RgSchCmnCell         *cellSchd = RG_SCH_CMN_GET_CELL(cell);
   RgSchCmnUlUe         *ueUl = RG_SCH_CMN_GET_UL_UE(ue);
   U8                   lcgCnt=0;
   RgSchCmnLcg          *lcgCmn;
   CmLList              *node;
   RgSchCmnAllocRecord  *allRcd;
   TRC2(rgSCHCmnUlUeReset);

   ueUl->minReqBytes = 0;
   ueUl->totalBsr = 0;
   ueUl->effBsr = 0;
   ueUl->nonGbrLcgBs = 0;
   ueUl->effAmbr = ueUl->cfgdAmbr;

   node = ueUl->ulAllocLst.first;
   while (node)
   {
      allRcd = (RgSchCmnAllocRecord *)node->node;
      allRcd->alloc = 0;
      node = node->next;
   }
   for(lcgCnt = 0; lcgCnt < RGSCH_MAX_LCG_PER_UE; lcgCnt++)
   {
      lcgCmn = RG_SCH_CMN_GET_UL_LCG(&ue->ul.lcgArr[lcgCnt]);
      lcgCmn->bs = 0;
      lcgCmn->reportedBs = 0;
      lcgCmn->effGbr = lcgCmn->cfgdGbr;
      lcgCmn->effDeltaMbr = lcgCmn->deltaMbr;
   }
   rgSCHCmnUlUeDelAllocs(cell, ue);
#ifdef LTEMAC_SPS
   if (ue->ul.ulSpsCfg.isUlSpsEnabled == TRUE)
   {
      rgSCHCmnSpsUlUeReset(cell, ue);
   }
#endif
   ue->isSrGrant = FALSE;
   cellSchd->apisUl->rgSCHUlUeReset(cell, ue);

   /* Stack Crash problem for TRACE5 changes. Added the return below */
   RETVOID;

}

/**
 * @brief RESET UL CQI and DL CQI&RI to conservative values
    * for a reestablishing UE.
 *
 * @details
 *
 *     Function : rgSCHCmnResetRiCqi 
 *     
 *     RESET UL CQI and DL CQI&RI to conservative values
 *     for a reestablishing UE
 *
 *  @param[in]  RgSchCellCb  *cell
 *  @param[in]  RgSchUeCb    *ue
 *  @return  Void
 **/
#ifdef ANSI
PRIVATE Void rgSCHCmnResetRiCqi 
(
RgSchCellCb  *cell,
RgSchUeCb    *ue
)
#else
PRIVATE Void rgSCHCmnResetRiCqi(cell, ue)
RgSchCellCb  *cell;
RgSchUeCb    *ue;
#endif
{
   RgSchCmnCell  *cellSchd = RG_SCH_CMN_GET_CELL(cell);
   RgSchCmnUe    *ueSchCmn = RG_SCH_CMN_GET_UE(ue);
   RgSchCmnDlUe  *ueDl = RG_SCH_CMN_GET_DL_UE(ue);
   RgSchCmnUlUe  *ueUl = RG_SCH_CMN_GET_UL_UE(ue);

   TRC2(rgSCHCmnResetRiCqi);

   rgSCHCmnUpdUeUlCqiInfo(cell, ue, ueUl, ueSchCmn, cellSchd, 
         cell->isCpUlExtend);

   ueDl->mimoInfo.cwInfo[0].cqi = cellSchd->dl.ccchCqi;
   ueDl->mimoInfo.cwInfo[1].cqi = cellSchd->dl.ccchCqi;
   ueDl->mimoInfo.ri = 1;
   if ((ue->mimoInfo.txMode == RGR_UE_TM_4) ||
          (ue->mimoInfo.txMode == RGR_UE_TM_6))
   {
      RG_SCH_CMN_SET_FORCE_TD(ue, RG_SCH_CMN_TD_NO_PMI);
   }
   if (ue->mimoInfo.txMode == RGR_UE_TM_3)
   {
      RG_SCH_CMN_SET_FORCE_TD(ue, RG_SCH_CMN_TD_RI_1);
   }
   rgSCHCmnDlSetUeAllocLmt(cell, ueDl);

#ifdef TFU_UPGRADE
   /* Request for an early Aper CQI in case of reest */
   if(ue->dl.ueDlCqiCfg.aprdCqiCfg.pres)
   {
      ue->dl.reqForCqi = TRUE;
      ue->acqiCb.aCqiTrigWt = 0;
   }
#endif   

   RETVOID;
}

/**
 * @brief Perform UE reset procedure.
 *
 * @details
 *
 *     Function : rgSCHCmnDlUeReset
 *
 *     This functions performs BO resetting and
 *     triggers DL specific scheduler
 *     to Perform UE reset procedure.
 *
 *  @param[in]  RgSchCellCb  *cell
 *  @param[in]  RgSchUeCb    *ue
 *  @return  Void
 **/
#ifdef ANSI
PRIVATE Void rgSCHCmnDlUeReset
(
RgSchCellCb  *cell,
RgSchUeCb    *ue
)
#else
PRIVATE Void rgSCHCmnDlUeReset(cell, ue)
RgSchCellCb  *cell;
RgSchUeCb    *ue;
#endif
{
   RgSchCmnCell         *cellSchd = RG_SCH_CMN_GET_CELL(cell);
   RgSchCmnDlCell       *cellCmnDl = RG_SCH_CMN_GET_DL_CELL(cell);
   RgSchCmnDlUe         *ueDl = RG_SCH_CMN_GET_DL_UE(ue);

   TRC2(rgSCHCmnDlUeReset);

   if (ueDl->rachInfo.poLnk.node != NULLP)
   {
      rgSCHCmnDlRmvFrmPdcchOdrQ(cell, ue);
   }

   /* Fix: syed Remove from TA List if this UE is there.
    * If TA Timer is running. Stop it */
   if (ue->dlTaLnk.node)
   {
      cmLListDelFrm(&cellCmnDl->taLst, &ue->dlTaLnk);
      ue->dlTaLnk.node = (PTR)NULLP;
   }
   else if (ue->taTmr.tmrEvnt != TMR_NONE)
   {
      rgSCHTmrStopTmr(cell, ue->taTmr.tmrEvnt, ue);
   }

#ifdef LTEMAC_SPS
   /* Reset SPS specific information for SPS enabled UE */
   if (ue->dl.dlSpsCfg.isDlSpsEnabled)
   {
      rgSCHCmnSpsDlUeReset(cell, ue);
   }
#endif
   cellSchd->apisDl->rgSCHDlUeReset(cell, ue);
}

/**
 * @brief Perform UE reset procedure.
 *
 * @details
 *
 *     Function : rgSCHCmnUeReset
 *
 *     This functions triggers specific scheduler
 *     to Perform UE reset procedure.
 *
 *  @param[in]  RgSchCellCb  *cell
 *  @param[in]  RgSchUeCb    *ue
 *  @return  S16
 *      -# ROK
 *      -# RFAILED
 **/
#ifdef ANSI
PUBLIC Void rgSCHCmnUeReset
(
RgSchCellCb  *cell,
RgSchUeCb    *ue
)
#else
PUBLIC Void rgSCHCmnUeReset(cell, ue)
RgSchCellCb  *cell;
RgSchUeCb    *ue;
#endif
{
   TRC2(rgSCHCmnUeReset);
   /* RACHO: remove UE from pdcch, handover and rapId assoc Qs */
   rgSCHCmnDelRachInfo(cell, ue);

   rgSCHPwrUeReset(cell, ue);

   rgSCHCmnUlUeReset(cell, ue);
   rgSCHCmnDlUeReset(cell, ue);

   /* Fix : syed RESET UL CQI and DL CQI&RI to conservative values
    * for a reestablishing UE */
   rgSCHCmnResetRiCqi(cell, ue);

   RETVOID;
}

/**
 * @brief UE out of MeasGap or AckNackReptn.
 *
 * @details
 *
 *     Function : rgSCHCmnActvtUlUe
 *
 *     This functions triggers specific scheduler
 *     to start considering it for scheduling.
 *
 *  @param[in]  RgSchCellCb  *cell
 *  @param[in]  RgSchUeCb    *ue
 *  @return  S16
 *      -# ROK
 *      -# RFAILED
 **/
#ifdef ANSI
PUBLIC Void rgSCHCmnActvtUlUe
(
RgSchCellCb  *cell,
RgSchUeCb    *ue
)
#else
PUBLIC Void rgSCHCmnActvtUlUe(cell, ue)
RgSchCellCb  *cell;
RgSchUeCb    *ue;
#endif
{
   RgSchCmnCell         *cellSchd = RG_SCH_CMN_GET_CELL(cell);
   TRC2(rgSCHCmnActvtUlUe);
   /* : take care of this in UL retransmission */
   cellSchd->apisUl->rgSCHUlActvtUe(cell, ue);
   RETVOID;
}

/**
 * @brief UE out of MeasGap or AckNackReptn.
 *
 * @details
 *
 *     Function : rgSCHCmnActvtDlUe
 *
 *     This functions triggers specific scheduler
 *     to start considering it for scheduling.
 *
 *  @param[in]  RgSchCellCb  *cell
 *  @param[in]  RgSchUeCb    *ue
 *  @return  S16
 *      -# ROK
 *      -# RFAILED
 **/
#ifdef ANSI
PUBLIC Void rgSCHCmnActvtDlUe
(
RgSchCellCb  *cell,
RgSchUeCb    *ue
)
#else
PUBLIC Void rgSCHCmnActvtDlUe(cell, ue)
RgSchCellCb  *cell;
RgSchUeCb    *ue;
#endif
{
   RgSchCmnCell         *cellSchd = RG_SCH_CMN_GET_CELL(cell);
   TRC2(rgSCHCmnActvtDlUe);

   cellSchd->apisDl->rgSCHDlActvtUe(cell, ue);
   RETVOID;
}

/**
 * @brief This API is invoked to indicate scheduler of a CRC indication.
 *
 * @details
 *
 *     Function : rgSCHCmnHdlUlTransInd
 *      This API is invoked to indicate scheduler of a CRC indication.
 *
 *  @param[in]  RgSchCellCb     *cell
 *  @param[in]  RgSchUeCb       *ue
 *  @param[in]  CmLteTimingInfo timingInfo
 *
 *  @return Void
 **/
#ifdef ANSI
PUBLIC Void rgSCHCmnHdlUlTransInd
(
RgSchCellCb     *cell,
RgSchUeCb       *ue,
CmLteTimingInfo timingInfo
)
#else
PUBLIC Void rgSCHCmnHdlUlTransInd(cell, ue, timingInfo)
RgSchCellCb     *cell;
RgSchUeCb       *ue;
CmLteTimingInfo timingInfo;
#endif
{
   RgSchCmnUlUe  *ueUl = RG_SCH_CMN_GET_UL_UE(ue);
   TRC2(rgSCHCmnHdlUlTransInd);

   /* Update the latest UL dat/sig transmission time */
   RGSCHCPYTIMEINFO(timingInfo, ueUl->ulTransTime);
   if (RG_SCH_CMN_IS_UE_PDCCHODR_INACTV(ue))
   {
      /* Some UL Transmission from this UE.
       * Activate this UE if it was inactive */
      RG_SCH_CMN_DL_UPDT_INACTV_MASK ( cell, ue, RG_PDCCHODR_INACTIVE);
      RG_SCH_CMN_UL_UPDT_INACTV_MASK ( cell, ue, RG_PDCCHODR_INACTIVE);
   }
   RETVOID;
}

#ifdef TFU_UPGRADE

/**
 * @brief Compute the minimum Rank based on Codebook subset
 *        restriction configuration for 4 Tx Ports and Tx Mode 4.
 *
 * @details
 *
 *     Function : rgSCHCmnComp4TxMode4
 *
 *     Depending on BitMap set at CBSR during Configuration
 *      - return the least possible Rank
 *
 *
 *  @param[in]  U32 *pmiBitMap
 *  @return  RgSchCmnRank
 **/
#ifdef ANSI
PRIVATE RgSchCmnRank rgSCHCmnComp4TxMode4
(
 U32    *pmiBitMap
 )
#else
PRIVATE RgSchCmnRank rgSCHCmnComp4TxMode4(pmiBitMap)
   U32  *pmiBitMap;
#endif
{
   U32 bitMap0, bitMap1;
   TRC2(rgSCHCmnComp4TxMode4);
   bitMap0 = pmiBitMap[0];
   bitMap1 = pmiBitMap[1];
   if((bitMap1) & 0xFFFF)
   {
      RETVALUE (RG_SCH_CMN_RANK_1);
   }
   else if((bitMap1>>16) & 0xFFFF)
   {
      RETVALUE (RG_SCH_CMN_RANK_2);
   }
   else if((bitMap0) & 0xFFFF)
   {
      RETVALUE (RG_SCH_CMN_RANK_3);
   }
   else if((bitMap0>>16) & 0xFFFF)
   {
      RETVALUE (RG_SCH_CMN_RANK_4);
   }
   else
   {
      RETVALUE (RG_SCH_CMN_RANK_1);
   }
}


/**
 * @brief Compute the minimum Rank based on Codebook subset
 *        restriction configuration for 2 Tx Ports and Tx Mode 4.
 *
 * @details
 *
 *     Function : rgSCHCmnComp2TxMode4
 *
 *     Depending on BitMap set at CBSR during Configuration
 *      - return the least possible Rank
 *
 *
 *  @param[in]  U32 *pmiBitMap
 *  @return  RgSchCmnRank
 **/
#ifdef ANSI
PRIVATE RgSchCmnRank rgSCHCmnComp2TxMode4
(
 U32    *pmiBitMap
 )
#else
PRIVATE RgSchCmnRank rgSCHCmnComp2TxMode4(pmiBitMap)
   U32  *pmiBitMap;
#endif
{
   U32 bitMap0;
   TRC2(rgSCHCmnComp2TxMode4);
   bitMap0 = pmiBitMap[0];
   if((bitMap0>>26)& 0x0F)
   {
      RETVALUE (RG_SCH_CMN_RANK_1);
   }
   else if((bitMap0>>30) & 3)
   {
      RETVALUE (RG_SCH_CMN_RANK_2);
   }
   else
   {
      RETVALUE (RG_SCH_CMN_RANK_1);
   }
}

/**
 * @brief Compute the minimum Rank based on Codebook subset
 *        restriction configuration for 4 Tx Ports and Tx Mode 3.
 *
 * @details
 *
 *     Function : rgSCHCmnComp4TxMode3
 *
 *     Depending on BitMap set at CBSR during Configuration
 *      - return the least possible Rank
 *
 *
 *  @param[in]  U32 *pmiBitMap
 *  @return  RgSchCmnRank
 **/
#ifdef ANSI
PRIVATE RgSchCmnRank rgSCHCmnComp4TxMode3
(
 U32    *pmiBitMap
 )
#else
PRIVATE RgSchCmnRank rgSCHCmnComp4TxMode3(pmiBitMap)
   U32  *pmiBitMap;
#endif
{
   U32 bitMap0;
   TRC2(rgSCHCmnComp4TxMode3);
   bitMap0 = pmiBitMap[0];
   if((bitMap0>>28)& 1)
   {
      RETVALUE (RG_SCH_CMN_RANK_1);
   }
   else if((bitMap0>>29) &1)
   {
      RETVALUE (RG_SCH_CMN_RANK_2);
   }
   else if((bitMap0>>30) &1)
   {
      RETVALUE (RG_SCH_CMN_RANK_3);
   }
   else if((bitMap0>>31) &1)
   {
      RETVALUE (RG_SCH_CMN_RANK_4);
   }
   else
   {
      RETVALUE (RG_SCH_CMN_RANK_1);
   }
}

/**
 * @brief Compute the minimum Rank based on Codebook subset
 *        restriction configuration for 2 Tx Ports and Tx Mode 3.
 *
 * @details
 *
 *     Function : rgSCHCmnComp2TxMode3
 *
 *     Depending on BitMap set at CBSR during Configuration
 *      - return the least possible Rank
 *
 *
 *  @param[in]  U32 *pmiBitMap
 *  @return  RgSchCmnRank
 **/
#ifdef ANSI
PRIVATE RgSchCmnRank rgSCHCmnComp2TxMode3
(
 U32 *pmiBitMap
 )
#else
PRIVATE RgSchCmnRank rgSCHCmnComp2TxMode3(pmiBitMap)
   U32 *pmiBitMap;
#endif
{
   U32 bitMap0;
   TRC2(rgSCHCmnComp2TxMode3);
   bitMap0 = pmiBitMap[0];
   if((bitMap0>>30)& 1)
   {
      RETVALUE (RG_SCH_CMN_RANK_1);
   }
   else if((bitMap0>>31) &1)
   {
      RETVALUE (RG_SCH_CMN_RANK_2);
   }
   else
   {
      RETVALUE (RG_SCH_CMN_RANK_1);
   }
}

/**
 * @brief Compute the minimum Rank based on Codebook subset
 *        restriction configuration.
 *
 * @details
 *
 *     Function : rgSCHCmnComputeRank
 *
 *     Depending on Num Tx Ports and Transmission mode
 *      - return the least possible Rank
 *
 *
 *  @param[in]  RgrTxMode txMode
 *  @param[in]  U32 *pmiBitMap
 *  @param[in]  U8 numTxPorts
 *  @return  RgSchCmnRank
 **/
#ifdef ANSI
PRIVATE RgSchCmnRank rgSCHCmnComputeRank
(
 RgrTxMode    txMode,
 U32          *pmiBitMap,
 U8           numTxPorts
 )
#else
PRIVATE RgSchCmnRank rgSCHCmnComputeRank(txMode, pmiBitMap, numTxPorts)
   RgrTxMode    txMode;
   U32          *pmiBitMap;
   U8           numTxPorts;
#endif
{
   TRC2(rgSCHCmnComputeRank);

   if (numTxPorts ==2 && txMode == RGR_UE_TM_3)
   {
      RETVALUE (rgSCHCmnComp2TxMode3(pmiBitMap));
   }
   else if (numTxPorts ==4 && txMode == RGR_UE_TM_3)
   {
      RETVALUE (rgSCHCmnComp4TxMode3(pmiBitMap));
   }
   else if (numTxPorts ==2 && txMode == RGR_UE_TM_4)
   {
      RETVALUE (rgSCHCmnComp2TxMode4(pmiBitMap));
   }
   else if (numTxPorts ==4 && txMode == RGR_UE_TM_4)
   {
      RETVALUE (rgSCHCmnComp4TxMode4(pmiBitMap));
   }
   else
   {
      RETVALUE (RG_SCH_CMN_RANK_1);
   }
}

#endif

/**
 * @brief Harq Entity Deinitialization for CMN SCH.
 *
 * @details
 *
 *     Function : rgSCHCmnDlDeInitHqEnt 
 *
 *     Harq Entity Deinitialization for CMN SCH 
 *
 *  @param[in]  RgSchCellCb  *cell
 *  @param[in]  RgSchDlHqEnt *hqE 
 *  @return  VOID
 **/
/*KWORK_FIX:Changed function return type to void */
#ifdef ANSI
PUBLIC Void rgSCHCmnDlDeInitHqEnt 
(
RgSchCellCb  *cell,
RgSchDlHqEnt *hqE
)
#else
PUBLIC Void rgSCHCmnDlDeInitHqEnt(cell, hqE)
RgSchCellCb  *cell;
RgSchDlHqEnt *hqE;
#endif
{
   RgSchCmnCell         *cellSchd = RG_SCH_CMN_GET_CELL(cell);
   RgSchDlHqProcCb      *hqP;
   U8                   cnt;
   S16                  ret;

   TRC2(rgSCHCmnDlDeInitHqEnt);
   ret = cellSchd->apisDl->rgSCHDlUeHqEntDeInit(cell, hqE);

   /* Free only If the Harq proc are created*/
   if(RFAILED == ret)
   {
   }

   for(cnt = 0; cnt < hqE->numHqPrcs; cnt++)
   {
      hqP = &hqE->procs[cnt];
      if ((RG_SCH_CMN_GET_DL_HQP(hqP)))
      {
         rgSCHUtlFreeSBuf(cell->instIdx,
              (Data**)(&(hqP->sch)), (sizeof(RgSchCmnDlHqProc)));
      }
   }
#ifdef LTE_TDD
   /* ACC-TDD */
   if (hqE->procs != NULLP)
   {
      /*To release the hqE->procs since it is dynamically allocated in TDD */
      rgSCHUtlFreeSBuf(cell->instIdx, (Data **)(&(hqE->procs)),
            sizeof(RgSchDlHqProcCb) * hqE->numHqPrcs);
   }
#endif

   RETVOID;
}

/**
 * @brief Harq Entity initialization for CMN SCH.
 *
 * @details
 *
 *     Function : rgSCHCmnDlInitHqEnt 
 *
 *     Harq Entity initialization for CMN SCH 
 *
 *  @param[in]  RgSchCellCb  *cell
 *  @param[in]  RgSchUeCb    *ue
 *  @return  S16
 *      -# ROK
 *      -# RFAILED
 **/
#ifdef ANSI
PUBLIC S16 rgSCHCmnDlInitHqEnt 
(
RgSchCellCb  *cell,
RgSchUeCb    *ue
)
#else
PUBLIC S16 rgSCHCmnDlInitHqEnt(cell, ue)
RgSchCellCb  *cell;
RgSchUeCb    *ue;
#endif
{
   RgSchDlHqProcCb      *hqP;
   U8                   cnt;

   RgSchCmnCell         *cellSchd = RG_SCH_CMN_GET_CELL(cell);
   TRC2(rgSCHCmnDlInitHqEnt);

   for(cnt = 0; cnt < ue->dl.hqEnt->numHqPrcs; cnt++)
   {
      hqP = &ue->dl.hqEnt->procs[cnt];
      if (rgSCHUtlAllocSBuf(cell->instIdx,
               (Data**)&(hqP->sch), (sizeof(RgSchCmnDlHqProc))) != ROK)
      {
         RETVALUE(RFAILED);
      }
   }

   if(ROK != cellSchd->apisDl->rgSCHDlUeHqEntInit(cell, ue))
   {
      RETVALUE(RFAILED);
   }

   RETVALUE(ROK);
}  /* rgSCHCmnDlInitHqEnt */

/**
 * @brief This function computes distribution of refresh period
 *
 * @details
 *
 *     Function: rgSCHCmnGetRefreshDist 
 *     Purpose: This function computes distribution of refresh period
 *              This is required to align set of UEs refresh
 *              around the different consecutive subframe.
 *               
 *     Invoked by: rgSCHCmnGetRefreshPerDist
 *
 *  @param[in]  RgSchCellCb        *cell
 *  @param[in]  RgSchUeCb          *ue
 *  @return  Void
 *
 **/
#ifdef ANSI
PRIVATE U8 rgSCHCmnGetRefreshDist 
(
RgSchCellCb        *cell,
RgSchUeCb          *ue
)
#else
PRIVATE U8 rgSCHCmnGetRefreshDist(cell, ue)
RgSchCellCb        *cell;
RgSchUeCb          *ue;
#endif
{
   U8   refOffst;
#ifdef DEBUGP
   Inst inst = cell->instIdx;
#endif
   TRC2(rgSCHCmnGetRefreshDist);

   for(refOffst = 0; refOffst < RGSCH_MAX_REFRESH_OFFSET; refOffst++)
   {
      if(cell->refreshUeCnt[refOffst] < RGSCH_MAX_REFRESH_GRPSZ)
      {
         cell->refreshUeCnt[refOffst]++;
         ue->refreshOffset = refOffst;
         /* printf("UE[%d] refresh offset[%d]. Cell refresh ue count[%d].\n", ue->ueId, refOffst,  cell->refreshUeCnt[refOffst]); */
         RETVALUE(refOffst);
      }
   }
  
   RGSCHDBGERRNEW(inst, (rgSchPBuf(inst), "Allocation of refresh distribution failed\n"));
   /* We should not enter here  normally, but incase of failure, allocating from  last offset*/
   cell->refreshUeCnt[refOffst-1]++;
   ue->refreshOffset = refOffst-1;

   RETVALUE(refOffst-1);
}
/**
 * @brief This function computes initial Refresh Wait Period.
 *
 * @details
 *
 *     Function: rgSCHCmnGetRefreshPer 
 *     Purpose: This function computes initial Refresh Wait Period.
 *              This is required to align multiple UEs refresh
 *              around the same time.
 *               
 *     Invoked by: rgSCHCmnRgrUeCfg 
 *
 *  @param[in]  RgSchCellCb        *cell
 *  @param[in]  RgSchUeCb          *ue
 *  @param[in]  U32                *waitPer 
 *  @return  Void
 *
 **/
#ifdef ANSI
PRIVATE Void rgSCHCmnGetRefreshPer 
(
RgSchCellCb        *cell,
RgSchUeCb          *ue,
U32                *waitPer
)
#else
PRIVATE Void rgSCHCmnGetRefreshPer(cell, ue, waitPer)
RgSchCellCb        *cell;
RgSchUeCb          *ue;
U32                *waitPer;
#endif
{
   U32       refreshPer;      
   U32       crntSubFrm;

   TRC2(rgSCHCmnGetRefreshPer);     

   refreshPer = RG_SCH_CMN_REFRESH_TIME * RG_SCH_CMN_REFRESH_TIMERES;
   crntSubFrm = cell->crntTime.sfn * 10 + cell->crntTime.subframe;
   /* Fix: syed align multiple UEs to refresh at same time */
   *waitPer = refreshPer - (crntSubFrm % refreshPer);
   *waitPer = RGSCH_CEIL(*waitPer, RG_SCH_CMN_REFRESH_TIMERES);
   *waitPer = *waitPer + rgSCHCmnGetRefreshDist(cell, ue);

   RETVOID;
}

/**
 * @brief UE initialisation for scheduler.
 *
 * @details
 *
 *     Function : rgSCHCmnRgrUeCfg
 *
 *     This functions intialises UE specific scheduler
 *     information
 *     0. Perform basic validations
 *     1. Allocate common sched UE cntrl blk
 *     2. Perform DL cfg (allocate Hq Procs Cmn sched cntrl blks)
 *     3. Perform UL cfg
 *     4. Perform DLFS cfg
 *
 *  @param[in]  RgSchCellCb  *cell
 *  @param[in]  RgSchUeCb    *ue
 *  @param[int] RgrUeCfg     *ueCfg
 *  @param[out] RgSchErrInfo *err
 *  @return  S16
 *      -# ROK
 *      -# RFAILED
 **/
#ifdef ANSI
PUBLIC S16 rgSCHCmnRgrUeCfg
(
RgSchCellCb  *cell,
RgSchUeCb    *ue,
RgrUeCfg     *ueCfg,
RgSchErrInfo *err
)
#else
PUBLIC S16 rgSCHCmnRgrUeCfg(cell, ue, ueCfg, err)
RgSchCellCb  *cell;
RgSchUeCb    *ue;
RgrUeCfg     *ueCfg;
RgSchErrInfo *err;
#endif
{
   S16                  ret;
   RgSchCmnCell         *cellSchd = RG_SCH_CMN_GET_CELL(cell);
   RgSchCmnUe           *ueSchCmn;
   RgSchCmnUlUe         *ueUl;
   RgSchCmnDlUe         *ueDl;
   U8                   cnt;
   RgSchCmnAllocRecord  *allRcd;
   U32                  waitPer;
   TRC2(rgSCHCmnRgrUeCfg);


   /* 1. Allocate Common sched control block */
   if((rgSCHUtlAllocSBuf(cell->instIdx,
               (Data**)&(ue->sch), (sizeof(RgSchCmnUe))) != ROK))
   {
      RLOG_ARG1(L_ERROR,DBG_CELLID,cell->cellId,
            "Memory allocation FAILED for CRNTI:%d",ueCfg->crnti);
      err->errCause = RGSCHERR_SCH_CFG;
      RETVALUE(RFAILED);
   }
   ueSchCmn   = RG_SCH_CMN_GET_UE(ue);
   ue->dl.ueDlCqiCfg = ueCfg->ueDlCqiCfg;
   if(ueCfg->ueCatEnum > 0 )
   {
     /*KWORK_FIX removed NULL chk for ueSchCmn*/
      ueSchCmn->cmn.ueCat = ueCfg->ueCatEnum - 1; 
   }
   else
   {
      ueSchCmn->cmn.ueCat = 0; /* Assuming enum values correctly set */
   }
   cmInitTimers(&ueSchCmn->cmn.tmr, 1);

   /*2.  Perform UEs downlink configuration */
   ueDl = &ueSchCmn->dl;
   /* RACHO : store the rapId assigned for HandOver UE.
    * Append UE to handover list of cmnCell */
   if (ueCfg->dedPreambleId.pres == PRSNT_NODEF)
   {
#if 1
      rgSCHCmnDelDedPreamble(cell, ueCfg->dedPreambleId.val);
#endif
      ueDl->rachInfo.hoRapId = ueCfg->dedPreambleId.val;
      cmLListAdd2Tail(&cellSchd->rachCfg.hoUeLst, &ueDl->rachInfo.hoLnk);
      ueDl->rachInfo.hoLnk.node = (PTR)ue;
   }

   rgSCHCmnUpdUeMimoInfo(ueCfg, ueDl, cell, cellSchd);

   if (ueCfg->txMode.pres == TRUE)
   {
      if ((ueCfg->txMode.txModeEnum == RGR_UE_TM_4) ||
            (ueCfg->txMode.txModeEnum == RGR_UE_TM_6))
      {
         RG_SCH_CMN_SET_FORCE_TD(ue, RG_SCH_CMN_TD_NO_PMI);
      }
      if (ueCfg->txMode.txModeEnum == RGR_UE_TM_3)
      {
         RG_SCH_CMN_SET_FORCE_TD(ue, RG_SCH_CMN_TD_RI_1);
      }
   }
   RGSCH_ARRAY_BOUND_CHECK(cell->instIdx, rgUeCatTbl, ueSchCmn->cmn.ueCat);
   ueDl->maxTbBits = rgUeCatTbl[ueSchCmn->cmn.ueCat].maxDlTbBits;
   ueDl->maxTbSz = rgUeCatTbl[ueSchCmn->cmn.ueCat].maxDlBits;
   /* Fix : syed Assign hqEnt to UE only if msg4 is done */
#ifdef LTE_TDD
   ueDl->maxSbSz = (rgUeCatTbl[ueSchCmn->cmn.ueCat].maxSftChBits/
         rgSchTddDlNumHarqProcTbl[cell->ulDlCfgIdx]);
#else
   ueDl->maxSbSz = (rgUeCatTbl[ueSchCmn->cmn.ueCat].maxSftChBits/
         RGSCH_NUM_DL_HQ_PROC);
#endif
   rgSCHCmnDlSetUeAllocLmt(cell, ueDl);
   if (ueCfg->ueQosCfg.ambrPres)
   {
      /* DL ambr */
      ueDl->ambrCfgd = (ueCfg->ueQosCfg.dlAmbr * RG_SCH_CMN_REFRESH_TIME)/100;
   }
   else
   {
      /* If AMBR not configured, consider default MAX AMBR */
      ueDl->ambrCfgd = 0;
   }
   ue->dl.dlAllocCb.rnti = ue->ueId;

   /* Initializing the lastCfi value to current cfi value */
   ueDl->lastCfi = cellSchd->dl.currCfi;

   if ((cellSchd->apisDl->rgSCHRgrDlUeCfg(cell, ue, ueCfg, err)) != ROK)
   {
      RLOG_ARG1(L_ERROR,DBG_CELLID,cell->cellId,
               "Spec Sched DL UE CFG FAILED for CRNTI:%d",ueCfg->crnti);
      RETVALUE(RFAILED);
   }



   /* 3. Initialize ul part */
   ueUl     = &ueSchCmn->ul;

   rgSCHCmnUpdUeUlCqiInfo(cell, ue, ueUl, ueSchCmn, cellSchd,
            cell->isCpUlExtend);

   ueUl->maxBytesPerUePerTti = rgUeCatTbl[ueSchCmn->cmn.ueCat].maxUlBits * \
                               RG_SCH_CMN_MAX_BITS_RATIO / (RG_SCH_CMN_UL_COM_DENOM*8);

   /* UL Byte Rate */
   if (ueCfg->ueQosCfg.ueBr == 0)
   {
      RLOG_ARG1(L_ERROR,DBG_CELLID,cell->cellId,"UeBr configured "
         "as 0 for CRNTI:%d",ueCfg->crnti);
      err->errCause = RGSCHERR_SCH_CFG;
      RETVALUE(RFAILED);
   }
   ueUl->cfgdAmbr = (ueCfg->ueQosCfg.ueBr * RG_SCH_CMN_REFRESH_TIME)/100;
   ueUl->effAmbr = ueUl->cfgdAmbr;
   RGSCHCPYTIMEINFO(cell->crntTime, ueUl->ulTransTime);

   /* Allocate UL BSR allocation tracking List */
   cmLListInit(&ueUl->ulAllocLst);

   for (cnt = 0; cnt < RG_SCH_CMN_MAX_ALLOC_TRACK; cnt++)
   {
      if((rgSCHUtlAllocSBuf(cell->instIdx,
                  (Data**)&(allRcd),sizeof(RgSchCmnAllocRecord)) != ROK))
      {
         RLOG_ARG1(L_ERROR,DBG_CELLID,cell->cellId,"Memory allocation FAILED"
                   "for CRNTI:%d",ueCfg->crnti);
         err->errCause = RGSCHERR_SCH_CFG;
         RETVALUE(RFAILED);
      }
      allRcd->allocTime = cell->crntTime;
      cmLListAdd2Tail(&ueUl->ulAllocLst, &allRcd->lnk);
      allRcd->lnk.node = (PTR)allRcd;
   }
   /* Allocate common sch cntrl blocks for LCGs */
   for (cnt=0; cnt<RGSCH_MAX_LCG_PER_UE; cnt++)
   {
      ret = rgSCHUtlAllocSBuf(cell->instIdx,
            (Data**)&(ue->ul.lcgArr[cnt].sch), (sizeof(RgSchCmnLcg)));
      if (ret != ROK)
      {
         RLOG_ARG1(L_ERROR,DBG_CELLID,cell->cellId,
            "SCH struct alloc failed for CRNTI:%d",ueCfg->crnti);
         err->errCause = RGSCHERR_SCH_CFG;
         RETVALUE(ret);
      }
   }
   /* After initialising UL part, do power related init */
   ret = rgSCHPwrUeCfg(cell, ue, ueCfg);
   if (ret != ROK)
   {
      RLOG_ARG1(L_ERROR,DBG_CELLID,cell->cellId, "Could not do "
         "power config for UE CRNTI:%d",ueCfg->crnti);
      RETVALUE(RFAILED);
   }
#ifdef LTEMAC_SPS
   ret = rgSCHCmnSpsUeCfg(cell, ue, ueCfg, err);
   if (ret != ROK)
   {
      RLOG_ARG1(L_ERROR,DBG_CELLID,cell->cellId, "Could not do "
         "SPS config for CRNTI:%d",ueCfg->crnti);
      RETVALUE(RFAILED);
   }
#endif /* LTEMAC_SPS */

   if ((cellSchd->apisUl->rgSCHRgrUlUeCfg(cell, ue, ueCfg, err)) != ROK)
   {
      RLOG_ARG1(L_ERROR,DBG_CELLID,cell->cellId, "Spec Sched UL UE CFG FAILED"
               "for CRNTI:%d",ueCfg->crnti);
      RETVALUE(RFAILED);
   }

   /* DLFS UE Config */
   if (cellSchd->dl.isDlFreqSel)
   {
      if ((cellSchd->apisDlfs->rgSCHDlfsUeCfg(cell, ue, ueCfg, err)) != ROK)
      {
         RLOG_ARG1(L_ERROR,DBG_CELLID,cell->cellId, "DLFS UE config FAILED"
                   "for CRNTI:%d",ueCfg->crnti);
         RETVALUE(RFAILED);
      }
   }

   /* Fix: syed align multiple UEs to refresh at same time */
   rgSCHCmnGetRefreshPer(cell, ue, &waitPer);
   /* Start UE Qos Refresh Timer */
   rgSCHCmnAddUeToRefreshQ(cell, ue, waitPer);

   RETVALUE(ROK);
}  /* rgSCHCmnRgrUeCfg */

/**
 * @brief UE TX mode reconfiguration handler.
 *
 * @details
 *
 *     Function : rgSCHCmnDlHdlTxModeRecfg
 *
 *     This functions updates UE specific scheduler
 *     information upon UE reconfiguration.
 *
 *  @param[in]  RgSchUeCb    *ue
 *  @param[in] RgrUeRecfg   *ueRecfg
 *  @return  Void
 **/
#ifdef TFU_UPGRADE
#ifdef ANSI
PRIVATE Void rgSCHCmnDlHdlTxModeRecfg
(
RgSchUeCb    *ue,
RgrUeRecfg   *ueRecfg,
U8 numTxPorts
)
#else
PRIVATE Void rgSCHCmnDlHdlTxModeRecfg(ue, ueRecfg, numTxPorts)
RgSchUeCb    *ue;
RgrUeRecfg   *ueRecfg;
U8 numTxPorts;
#endif
#else
#ifdef ANSI
PRIVATE Void rgSCHCmnDlHdlTxModeRecfg
(
RgSchUeCb    *ue,
RgrUeRecfg   *ueRecfg
)
#else
PRIVATE Void rgSCHCmnDlHdlTxModeRecfg(ue, ueRecfg)
RgSchUeCb    *ue;
RgrUeRecfg   *ueRecfg;
#endif
#endif
{
   RgSchCmnDlUe *ueDl = RG_SCH_CMN_GET_DL_UE(ue);
   TRC2(rgSCHCmnDlHdlTxModeRecfg);

   if (ueRecfg->txMode.pres != PRSNT_NODEF)
   {
      RETVOID;
   }
   /* ccpu00140894- Starting Timer for TxMode Transition Completion*/
   ue->txModeTransCmplt =FALSE;
   rgSCHTmrStartTmr (ue->cell, ue, RG_SCH_TMR_TXMODE_TRNSTN, RG_SCH_TXMODE_TRANS_TIMER);
   if (ueRecfg->txMode.tmTrnstnState == RGR_TXMODE_RECFG_CMPLT)
   {
      RG_SCH_CMN_UNSET_FORCE_TD(ue,
                                RG_SCH_CMN_TD_TXMODE_RECFG);
     /* MS_WORKAROUND for ccpu00123186 MIMO Fix Start: need to set FORCE TD bitmap based on TX mode */
     ueDl->mimoInfo.ri = 1;
     if ((ueRecfg->txMode.txModeEnum == RGR_UE_TM_4) ||
          (ueRecfg->txMode.txModeEnum == RGR_UE_TM_6))
      {
         RG_SCH_CMN_SET_FORCE_TD(ue, RG_SCH_CMN_TD_NO_PMI);
      }
      if (ueRecfg->txMode.txModeEnum == RGR_UE_TM_3)
      {
         RG_SCH_CMN_SET_FORCE_TD(ue, RG_SCH_CMN_TD_RI_1);
      }
      /* MIMO Fix End: need to set FORCE TD bitmap based on TX mode */
      RETVOID;
   }
   if (ueRecfg->txMode.tmTrnstnState == RGR_TXMODE_RECFG_START)
   {
      /* start afresh forceTD masking */
      RG_SCH_CMN_INIT_FORCE_TD(ue, 0);
      RG_SCH_CMN_SET_FORCE_TD(ue, RG_SCH_CMN_TD_TXMODE_RECFG);
      /* Intialize MIMO related parameters of UE */

#ifdef TFU_UPGRADE
      if(ueRecfg->txMode.pres)
      {
         if((ueRecfg->txMode.txModeEnum ==RGR_UE_TM_3) ||
               (ueRecfg->txMode.txModeEnum ==RGR_UE_TM_4))
         {
            if(ueRecfg->ueCodeBookRstRecfg.pres)
            {
               ueDl->mimoInfo.ri =
                  rgSCHCmnComputeRank(ueRecfg->txMode.txModeEnum,
                    ueRecfg->ueCodeBookRstRecfg.pmiBitMap, numTxPorts);
            }
            else
            {
               ueDl->mimoInfo.ri = 1;
            }
         }
         else
         {
            ueDl->mimoInfo.ri = 1;
         }
      }
      else
      {
         ueDl->mimoInfo.ri = 1;
      }
#else
      ueDl->mimoInfo.ri = 1;
#endif /* TFU_UPGRADE */
      if ((ueRecfg->txMode.txModeEnum == RGR_UE_TM_4) ||
          (ueRecfg->txMode.txModeEnum == RGR_UE_TM_6))
      {
         RG_SCH_CMN_SET_FORCE_TD(ue, RG_SCH_CMN_TD_NO_PMI);
      }
      if (ueRecfg->txMode.txModeEnum == RGR_UE_TM_3)
      {
         RG_SCH_CMN_SET_FORCE_TD(ue, RG_SCH_CMN_TD_RI_1);
      }
      RETVOID;
   }
}
/***********************************************************
 *
 *     Func : rgSCHCmnUpdUeMimoInfo
 *
 *     Desc : Updates UL and DL Ue Information
 *
 *     Ret  :
 *
 *     Notes:
 *
 *     File :
 *
 **********************************************************/
#ifdef ANSI
PRIVATE Void rgSCHCmnUpdUeMimoInfo
(
RgrUeCfg     *ueCfg,
RgSchCmnDlUe *ueDl,
RgSchCellCb  *cell,
RgSchCmnCell *cellSchd
)
#else
PRIVATE Void rgSCHCmnUpdUeMimoInfo(ueCfg, ueDl, cell, cellSchd)
RgrUeCfg     *ueCfg;
RgSchCmnDlUe *ueDl;
RgSchCellCb  *cell;
RgSchCmnCell *cellSchd;
#endif
{
   TRC2(rgSCHCmnUpdUeMimoInfo)
#ifdef TFU_UPGRADE
   if(ueCfg->txMode.pres)
   {
      if((ueCfg->txMode.txModeEnum ==RGR_UE_TM_3) ||
            (ueCfg->txMode.txModeEnum ==RGR_UE_TM_4))
      {
         if(ueCfg->ueCodeBookRstCfg.pres)
         {
            ueDl->mimoInfo.ri =
               rgSCHCmnComputeRank(ueCfg->txMode.txModeEnum,
                 ueCfg->ueCodeBookRstCfg.pmiBitMap, cell->numTxAntPorts);
         }
         else
         {
            ueDl->mimoInfo.ri = 1;
         }
      }
      else
      {
         ueDl->mimoInfo.ri = 1;
      }
   }
   else
   {
      ueDl->mimoInfo.ri = 1;
   }

#else
   ueDl->mimoInfo.ri = 1;
#endif /*TFU_UPGRADE */
   ueDl->mimoInfo.cwInfo[0].cqi = cellSchd->dl.ccchCqi;
   ueDl->mimoInfo.cwInfo[1].cqi = cellSchd->dl.ccchCqi;

   RETVOID;
}
/***********************************************************
 *
 *     Func : rgSCHCmnUpdUeUlCqiInfo
 *
 *     Desc : Updates UL and DL Ue Information
 *
 *     Ret  :
 *
 *     Notes:
 *
 *     File :
 *
 **********************************************************/
#ifdef ANSI
PRIVATE Void rgSCHCmnUpdUeUlCqiInfo
(
RgSchCellCb   *cell,
RgSchUeCb     *ue,
RgSchCmnUlUe  *ueUl,
RgSchCmnUe    *ueSchCmn,
RgSchCmnCell  *cellSchd,
Bool          isEcp
)
#else
PRIVATE Void rgSCHCmnUpdUeUlCqiInfo(cell, ue, ueUl, ueSchCmn, cellSchd, isEcp)
RgSchCellCb  *cell;
RgSchUeCb    *ue;
RgSchCmnUlUe *ueUl;
RgSchCmnUe   *ueSchCmn;
RgSchCmnCell *cellSchd;
Bool          isEcp;
#endif
{

   TRC2(rgSCHCmnUpdUeUlCqiInfo)

#ifdef TFU_UPGRADE
   if(ue->srsCb.srsCfg.type  ==  RGR_SCH_SRS_SETUP)
   {
     if(ue->ul.ulTxAntSel.pres)
     {
       ueUl->crntUlCqi[ue->srsCb.selectedAnt] = cellSchd->ul.dfltUlCqi;
       ueUl->validUlCqi = ueUl->crntUlCqi[ue->srsCb.selectedAnt];
     }
     else
     {
       ueUl->crntUlCqi[0] = cellSchd->ul.dfltUlCqi;
       ueUl->validUlCqi =  ueUl->crntUlCqi[0];
     }
      ue->validTxAnt = ue->srsCb.selectedAnt;
   }
   else
   {
      ueUl->validUlCqi = cellSchd->ul.dfltUlCqi;
      ue->validTxAnt = 0;
   }
#ifdef UL_LA
   ueUl->ulLaCb.cqiBasediTbs = rgSchCmnUlCqiToTbsTbl[isEcp]
                                                [ueUl->validUlCqi] * 100;   
   ueUl->ulLaCb.deltaiTbs = 0;
#endif

#else
   ueUl->crntUlCqi[0] = cellSchd->ul.dfltUlCqi;
#endif /*TFU_UPGRADE */
   RGSCH_ARRAY_BOUND_CHECK(cell->instIdx, rgUeCatTbl, ueSchCmn->cmn.ueCat);
   if (rgUeCatTbl[ueSchCmn->cmn.ueCat].ul64qamSup == FALSE)
   {
      ueUl->maxUlCqi = cellSchd->ul.max16qamCqi;
   }
   else
   {
      ueUl->maxUlCqi = RG_SCH_CMN_UL_NUM_CQI - 1;
   }

   RETVOID;
}
/***********************************************************
 *
 *     Func : rgSCHCmnUpdUeCatCfg
 *
 *     Desc : Updates UL and DL Ue Information
 *
 *     Ret  :
 *
 *     Notes:
 *
 *     File :
 *
 **********************************************************/
#ifdef ANSI
PRIVATE Void rgSCHCmnUpdUeCatCfg
(
RgSchUeCb    *ue,
RgSchCellCb  *cell
)
#else
PRIVATE Void rgSCHCmnUpdUeCatCfg(ue, cell)
RgSchUeCb    *ue;
RgSchCellCb  *cell;
#endif
{
   RgSchCmnUlUe *ueUl     = RG_SCH_CMN_GET_UL_UE(ue);
   RgSchCmnDlUe *ueDl     = RG_SCH_CMN_GET_DL_UE(ue);
   RgSchCmnUe   *ueSchCmn = RG_SCH_CMN_GET_UE(ue);
   RgSchCmnCell *cellSchd = RG_SCH_CMN_GET_CELL(cell);

   TRC2(rgSCHCmnUpdUeCatCfg)

   ueDl->maxTbBits = rgUeCatTbl[ueSchCmn->cmn.ueCat].maxDlTbBits;
   ueDl->maxTbSz = rgUeCatTbl[ueSchCmn->cmn.ueCat].maxDlBits;
   ueDl->maxSbSz = (rgUeCatTbl[ueSchCmn->cmn.ueCat].maxSftChBits/
                           ue->dl.hqEnt->numHqPrcs);
   if (rgUeCatTbl[ueSchCmn->cmn.ueCat].ul64qamSup == FALSE)
   {
      ueUl->maxUlCqi = cellSchd->ul.max16qamCqi;
   }
   else
   {
      ueUl->maxUlCqi = RG_SCH_CMN_UL_NUM_CQI - 1;
   }
   ueUl->maxBytesPerUePerTti = rgUeCatTbl[ueSchCmn->cmn.ueCat].maxUlBits * \
                   RG_SCH_CMN_MAX_BITS_RATIO / (RG_SCH_CMN_UL_COM_DENOM*8);
   RETVOID;
}

/**
 * @brief UE reconfiguration for scheduler.
 *
 * @details
 *
 *     Function : rgSChCmnRgrUeRecfg
 *
 *     This functions updates UE specific scheduler
 *     information upon UE reconfiguration.
 *
 *  @param[in]  RgSchCellCb  *cell
 *  @param[in]  RgSchUeCb    *ue
 *  @param[int] RgrUeRecfg   *ueRecfg
 *  @param[out] RgSchErrInfo *err
 *  @return  S16
 *      -# ROK
 *      -# RFAILED
 **/
#ifdef ANSI
PUBLIC S16 rgSCHCmnRgrUeRecfg
(
RgSchCellCb  *cell,
RgSchUeCb    *ue,
RgrUeRecfg   *ueRecfg,
RgSchErrInfo *err
)
#else
PUBLIC S16 rgSCHCmnRgrUeRecfg(cell, ue, ueRecfg, err)
RgSchCellCb  *cell;
RgSchUeCb    *ue;
RgrUeRecfg   *ueRecfg;
RgSchErrInfo *err;
#endif
{
   RgSchCmnCell *cellSchCmn = RG_SCH_CMN_GET_CELL(cell);
   RgSchCmnUlUe *ueUl     = RG_SCH_CMN_GET_UL_UE(ue);
   RgSchCmnDlUe *ueDl = RG_SCH_CMN_GET_DL_UE(ue);
   U32          waitPer;

   TRC2(rgSCHCmnRgrUeRecfg);
   /* Basic validations */
   if (ueRecfg->ueRecfgTypes & RGR_UE_TXMODE_RECFG)
   {
#ifdef TFU_UPGRADE
      rgSCHCmnDlHdlTxModeRecfg(ue, ueRecfg, cell->numTxAntPorts);
#else
      rgSCHCmnDlHdlTxModeRecfg(ue, ueRecfg);
#endif /* TFU_UPGRADE */
   }
   if(ueRecfg->ueRecfgTypes & RGR_UE_CSG_PARAM_RECFG)
   {
      ue->csgMmbrSta = ueRecfg->csgMmbrSta;
   }
   /* Changes for UE Category reconfiguration feature */
   if(ueRecfg->ueRecfgTypes & RGR_UE_UECAT_RECFG)
   {
       rgSCHCmnUpdUeCatCfg(ue, cell);
   }
   if (ueRecfg->ueRecfgTypes & RGR_UE_APRD_DLCQI_RECFG)
   {
      ue->dl.ueDlCqiCfg.aprdCqiCfg = ueRecfg->aprdDlCqiRecfg;
   }
#ifndef TFU_UPGRADE
   if (ueRecfg->ueRecfgTypes & RGR_UE_PRD_DLCQI_RECFG)
   {
      if ((ueRecfg->prdDlCqiRecfg.pres == TRUE)
            && (ueRecfg->prdDlCqiRecfg.prdModeEnum != RGR_PRD_CQI_MOD10)
            && (ueRecfg->prdDlCqiRecfg.prdModeEnum != RGR_PRD_CQI_MOD20))
      {
         RLOG_ARG2(L_ERROR,DBG_CELLID,cell->cellId,"Unsupported periodic CQI "
            "reporting mode %d for old CRNIT:%d", 
            (int)ueRecfg->prdDlCqiRecfg.prdModeEnum,ueRecfg->oldCrnti);
         err->errCause = RGSCHERR_SCH_CFG;
         RETVALUE(RFAILED);
      }
     ue->dl.ueDlCqiCfg.prdCqiCfg = ueRecfg->prdDlCqiRecfg;
   }
#endif

   if (ueRecfg->ueRecfgTypes & RGR_UE_ULPWR_RECFG)
   {
      if (rgSCHPwrUeRecfg(cell, ue, ueRecfg) != ROK)
      {
         RLOG_ARG1(L_ERROR,DBG_CELLID,cell->cellId,
               "Power Reconfiguration Failed for OLD CRNTI:%d",ueRecfg->oldCrnti);
         RETVALUE(RFAILED);
      }
   }

   if (ueRecfg->ueRecfgTypes & RGR_UE_QOS_RECFG)
   {
      /* Uplink Sched related Initialization */
      if (ueRecfg->ueQosRecfg.ueBr == 0)
      {
         RLOG_ARG1(L_ERROR,DBG_CELLID,cell->cellId,"UeBr "
            "configured as 0 for OLD CRNTI:%d",ueRecfg->oldCrnti);
         err->errCause = RGSCHERR_SCH_CFG;
         RETVALUE(RFAILED);
      }
      ueUl->cfgdAmbr = (ueRecfg->ueQosRecfg.ueBr * \
      RG_SCH_CMN_REFRESH_TIME)/100;
      /* Downlink Sched related Initialization */
      if (ueRecfg->ueQosRecfg.ambrPres)
      {
         ueDl->ambrCfgd = (ueRecfg->ueQosRecfg.dlAmbr * \
         RG_SCH_CMN_REFRESH_TIME)/100;
      }
      /* Fix: syed Update the effAmbr and effUeBR fields w.r.t the
       * new QOS configuration */
      rgSCHCmnDelUeFrmRefreshQ(cell, ue);
      /* Fix: syed align multiple UEs to refresh at same time */
      rgSCHCmnGetRefreshPer(cell, ue, &waitPer);
      rgSCHCmnApplyUeRefresh(cell, ue);
      rgSCHCmnAddUeToRefreshQ(cell, ue, waitPer);
   }

   if ((cellSchCmn->apisUl->rgSCHRgrUlUeRecfg(cell, ue, ueRecfg, err)) != ROK)
   {
      RLOG_ARG1(L_ERROR,DBG_CELLID,cell->cellId, 
            "Spec Sched UL UE ReCFG FAILED for CRNTI:%d",ue->ueId);
      RETVALUE(RFAILED);
   }
   if ((cellSchCmn->apisDl->rgSCHRgrDlUeRecfg(cell, ue, ueRecfg, err)) != ROK)
   {
      RLOG_ARG1(L_ERROR,DBG_CELLID,cell->cellId, 
            "Spec Sched DL UE ReCFG FAILED for CRNTI:%d",ue->ueId);
      RETVALUE(RFAILED);
   }
   /* DLFS UE Config */
   if (cellSchCmn->dl.isDlFreqSel)
   {
      if ((cellSchCmn->apisDlfs->rgSCHDlfsUeRecfg(cell, ue, \
         ueRecfg, err)) != ROK)
      {
         RLOG_ARG1(L_ERROR,DBG_CELLID,cell->cellId, 
               "DLFS UE re-config FAILED for CRNTI:%d",ue->ueId);
         RETVALUE(RFAILED);
      }
   }

#ifdef LTEMAC_SPS
   /* Invoke re-configuration on SPS module */
   if (rgSCHCmnSpsUeRecfg(cell, ue, ueRecfg, err) != ROK)
   {
      RLOG_ARG1(L_ERROR,DBG_CELLID,cell->cellId, 
              "DL SPS ReCFG FAILED for UE CRNTI:%d", ue->ueId);
      RETVALUE(RFAILED);
   }
#endif

   RETVALUE(ROK);
}  /* rgSCHCmnRgrUeRecfg*/

/***********************************************************
 *
 *     Func : rgSCHCmnUlUeDelAllocs
 *
 *     Desc : Deletion of all UE allocations.
 *
 *     Ret  :
 *
 *     Notes:
 *
 *     File :
 *
 **********************************************************/
#ifdef ANSI
PRIVATE Void rgSCHCmnUlUeDelAllocs
(
RgSchCellCb  *cell,
RgSchUeCb   *ue
)
#else
PRIVATE Void rgSCHCmnUlUeDelAllocs(cell, ue)
RgSchCellCb  *cell;
RgSchUeCb   *ue;
#endif
{
   RgSchCmnUlCell  *cellUl = RG_SCH_CMN_GET_UL_CELL(cell);
   U8 i;
#ifdef LTEMAC_SPS
   RgSchCmnUlUeSpsInfo   *ulSpsUe   = RG_SCH_CMN_GET_UL_SPS_UE(ue);
#endif
   TRC2(rgSCHCmnUlUeDelAllocs);

   for (i = 0; i < ue->ul.hqEnt.numHqPrcs; ++i)
   {
      RgSchUlHqProcCb *proc = rgSCHUhmGetUlHqProc(ue, i);
      /* R8 Upgrade */
      proc->ndi = 0;
      if (proc->alloc)
      {
 /* Added Insure Fixes Of reading Dangling memory.NULLed crntAlloc */
#ifdef LTEMAC_SPS
          if(proc->alloc == ulSpsUe->ulSpsSchdInfo.crntAlloc)
          {
               ulSpsUe->ulSpsSchdInfo.crntAlloc = NULLP;
             ulSpsUe->ulSpsSchdInfo.crntAllocSf = NULLP;
          }
#endif
          rgSCHCmnUlFreeAllocation(cell, &cellUl->ulSfArr[proc->ulSfIdx],
                                                         proc->alloc);
         /* PHY probably needn't be intimated since
          * whatever intimation it needs happens at the last minute
          */
      }
      /* Fix: syed Adaptive Msg3 Retx crash. Remove the harqProc
       * from adaptive retx List. */
      if (proc->reTxLnk.node)
      {
         cmLListDelFrm(proc->reTxAlloc.reTxLst, &proc->reTxLnk); 
         proc->reTxLnk.node = (PTR)NULLP;
      }
   }
   RETVOID;
}

/***********************************************************
 *
 *     Func : rgSCHCmnDelUeFrmRefreshQ
 *
 *     Desc : Adds a UE to refresh queue, so that the UE is
 *            periodically triggered to refresh it's GBR and
 *            AMBR values.
 *
 *     Ret  :
 *
 *     Notes:
 *
 *     File :
 *
 **********************************************************/
#ifdef ANSI
PRIVATE Void rgSCHCmnDelUeFrmRefreshQ
(
RgSchCellCb     *cell,
RgSchUeCb       *ue
)
#else
PRIVATE Void rgSCHCmnDelUeFrmRefreshQ(cell, ue)
RgSchCellCb     *cell;
RgSchUeCb       *ue;
#endif
{
   RgSchCmnCell   *sched  = RG_SCH_CMN_GET_CELL(cell);
   CmTmrArg       arg;
   RgSchCmnUeInfo *ueSchd = RG_SCH_CMN_GET_CMN_UE(ue);

   TRC2(rgSCHCmnDelUeFrmRefreshQ);

   cmMemset((U8 *)&arg, 0, sizeof(arg));
   arg.tqCp   = &sched->tmrTqCp;
   arg.tq     = sched->tmrTq;
   arg.timers = &ueSchd->tmr;
   arg.cb     = (PTR)ue;
   arg.tNum   = 0;
   arg.max    = 1;
   arg.evnt   = RG_SCH_CMN_EVNT_UE_REFRESH;

   cmRmvCbTq(&arg);
   RETVOID;
}

/***********************************************************
 *
 *     Func : rgSCHCmnUeCcchSduDel
 *
 *     Desc : Clear CCCH SDU scheduling context.
 *
 *     Ret  :
 *
 *     Notes:
 *
 *     File :
 *
 **********************************************************/
#ifdef ANSI
PRIVATE Void rgSCHCmnUeCcchSduDel
(
RgSchCellCb  *cell,
RgSchUeCb    *ueCb
)
#else
PRIVATE Void rgSCHCmnUeCcchSduDel(cell, ueCb)
RgSchCellCb  *cell;
RgSchUeCb    *ueCb;
#endif
{
   RgSchDlHqEnt      *hqE = ueCb->dl.hqEnt;
   RgSchDlHqProcCb   *ccchSduHqP = NULLP;
   RgSchCmnCell      *cellSch = RG_SCH_CMN_GET_CELL(cell);

   TRC2(rgSCHCmnUeCcchSduDel);

   if (hqE == NULLP)
   {
      RETVOID;
   }
   ccchSduHqP = hqE->ccchSduProc;
   if(ueCb->ccchSduLnk.node != NULLP)
   {
      /* Remove the ccchSduProc if it is in the Tx list */
      cmLListDelFrm(&(cell->ccchSduUeLst), &(ueCb->ccchSduLnk));
      ueCb->ccchSduLnk.node = NULLP;   
   }
   else if(ccchSduHqP != NULLP)
   {
      if(ccchSduHqP->tbInfo[0].ccchSchdInfo.retxLnk.node != NULLP)
      {
         /* Remove the ccchSduProc if it is in the retx list */
         cmLListDelFrm(&cellSch->dl.ccchSduRetxLst,
          &ccchSduHqP->tbInfo[0].ccchSchdInfo.retxLnk);
         /* ccchSduHqP->tbInfo[0].ccchSchdInfo.retxLnk.node = NULLP; */
         rgSCHDhmRlsHqpTb(ccchSduHqP, 0, TRUE);
      }
      else if ((ccchSduHqP->tbInfo[0].subFrm != NULLP) &&
       (ccchSduHqP->tbInfo[0].sfLnk.node != NULLP))
      {
         /* Remove the ccchSduProc if it waiting in sf->tbs list for harq feedback */
         cmLListDelFrm(&ccchSduHqP->tbInfo[0].subFrm->pdcchInfo.pdcchs,
               &ccchSduHqP->tbInfo[0].pdcch->lnk);
         cmLListAdd2Tail(&cell->pdcchLst, &ccchSduHqP->tbInfo[0].pdcch->lnk);
         rgSCHUtlDlHqPTbRmvFrmTx(ccchSduHqP->tbInfo[0].subFrm, 
               ccchSduHqP, 0, FALSE);
         rgSCHDhmRlsHqpTb(ccchSduHqP, 0, TRUE);
      }
   }   
   RETVOID;
}




/**
 * @brief UE deletion for scheduler.
 *
 * @details
 *
 *     Function : rgSCHCmnUeDel
 *
 *     This functions deletes all scheduler information
 *     pertaining to an UE.
 *
 *  @param[in]  RgSchCellCb  *cell
 *  @param[in]  RgSchUeCb    *ue
 *  @return  Void
 **/
#ifdef ANSI
PUBLIC Void rgSCHCmnUeDel
(
RgSchCellCb  *cell,
RgSchUeCb    *ue
)
#else
PUBLIC Void rgSCHCmnUeDel(cell, ue)
RgSchCellCb  *cell;
RgSchUeCb    *ue;
#endif
{
   RgSchCmnUlUe *ueUl = RG_SCH_CMN_GET_UL_UE(ue);
   CmLList              *node;
   RgSchCmnAllocRecord  *allRcd;
   U8                   cnt;
   RgSchCmnCell *cellSchCmn = RG_SCH_CMN_GET_CELL(cell);
   TRC2(rgSCHCmnUeDel);

   if (ue->sch == NULLP)
   {
      /* Common scheduler config has not happened yet */
      RETVOID;
   }
   if(ue->dl.hqEnt)
   {
      /* UE Free can be triggered before MSG4 done when dlHqE is not updated */
      rgSCHCmnUeCcchSduDel(cell, ue);
   }
   rgSCHCmnDelUeFrmRefreshQ(cell, ue);

   rgSCHCmnUlUeDelAllocs(cell, ue);

   rgSCHCmnDelRachInfo(cell, ue);

   cellSchCmn->apisUl->rgSCHFreeUlUe(cell, ue);
   cellSchCmn->apisDl->rgSCHFreeDlUe(cell, ue);

   rgSCHPwrUeDel(cell, ue);

#ifdef LTEMAC_SPS
   rgSCHCmnSpsUeDel(cell, ue);
#endif /* LTEMAC_SPS*/

   /* DLFS UE delete */
   if (cellSchCmn->dl.isDlFreqSel)
   {
      cellSchCmn->apisDlfs->rgSCHDlfsUeDel(cell, ue);
   }
   node = ueUl->ulAllocLst.first;

/* ccpu00117052 - MOD - Passing double pointer in all the places of
   rgSCHUtlFreeSBuf function call for proper NULLP assignment*/
   while(node)
   {
      allRcd = (RgSchCmnAllocRecord *)node->node;
      node = node->next;
      cmLListDelFrm(&ueUl->ulAllocLst, &allRcd->lnk);
      rgSCHUtlFreeSBuf(cell->instIdx,
         (Data**)(&allRcd), (sizeof(RgSchCmnAllocRecord)));
   }

   for(cnt = 0; cnt < RGSCH_MAX_LCG_PER_UE; cnt++)
   {
      if (ue->ul.lcgArr[cnt].sch != NULLP)
      {
         rgSCHUtlFreeSBuf(cell->instIdx,
            (Data**)(&(ue->ul.lcgArr[cnt].sch)), (sizeof(RgSchCmnLcg)));
      }
   }

   /* Fix : syed Moved hqEnt deinit to rgSCHCmnDlDeInitHqEnt */
   rgSCHUtlFreeSBuf(cell->instIdx,
         (Data**)(&(ue->sch)), (sizeof(RgSchCmnUe)));
   RETVOID;
}  /* rgSCHCmnUeDel */


/**
 * @brief This function handles the common code rate configurations
 *        done as part of RgrCellCfg/RgrCellRecfg.
 *
 * @details
 *
 *     Function: rgSCHCmnDlCnsdrCmnRt
 *     Purpose:  This function handles the common code rate configurations
 *        done as part of RgrCellCfg/RgrCellRecfg.
 *
 *     Invoked by: Scheduler
 *
 *  @param[in]  RgSchCellCb                *cell
 *  @param[in]  RgrDlCmnCodeRateCfg     *dlCmnCodeRate
 *  @return     S16
 *
 **/
#ifdef ANSI
PRIVATE S16 rgSCHCmnDlCnsdrCmnRt
(
RgSchCellCb             *cell,
RgrDlCmnCodeRateCfg     *dlCmnCodeRate
)
#else
PRIVATE S16 rgSCHCmnDlCnsdrCmnRt(cell, dlCmnCodeRate)
RgSchCellCb             *cell;
RgrDlCmnCodeRateCfg     *dlCmnCodeRate;
#endif
{
   RgSchCmnCell         *cellDl = RG_SCH_CMN_GET_CELL(cell);
   U32                  bitsPerRb;
   U32                  bitsPer2Rb;
   U32                  bitsPer3Rb;
   U8                   i, rbNum;
   U32                  pdcchBits;

   TRC2(rgSCHCmnDlCnsdrCmnRt);

   /* code rate is bits per 1024 phy bits, since modl'n scheme is 2. it is
    * bits per 1024/2 REs */
   if (dlCmnCodeRate->bcchPchRaCodeRate != 0)
   {
      bitsPerRb = ((dlCmnCodeRate->bcchPchRaCodeRate * 2) *
            cellDl->dl.noResPerRb[3])/1024;
   }
   else
   {
      bitsPerRb = ((RG_SCH_CMN_DEF_BCCHPCCH_CODERATE * 2) *
            cellDl->dl.noResPerRb[3])/1024;
   }
   /* Store bitsPerRb in cellDl->dl to use later to determine
    * Number of RBs for UEs with SI-RNTI, P-RNTI and RA-RNTI */
   cellDl->dl.bitsPerRb = bitsPerRb;
   /* ccpu00115595 end*/
   /* calculate the ITbs for 2 RBs. Initialize ITbs to MAX value */
   i = 0;
   rbNum = 2;
   bitsPer2Rb = bitsPerRb * rbNum;
   while ((i < 9) && (rgTbSzTbl[0][i][rbNum - 1] <= bitsPer2Rb))
      i++;

   (i <= 1)? (cellDl->dl.cmnChITbs.iTbs2Rbs = 0) :
      (cellDl->dl.cmnChITbs.iTbs2Rbs = i-1);

   /* calculate the ITbs for 3 RBs. Initialize ITbs to MAX value */
   i = 0;
   rbNum = 3;
   bitsPer3Rb = bitsPerRb * rbNum;
   while ((i < 9) && (rgTbSzTbl[0][i][rbNum - 1] <= bitsPer3Rb))
         i++;

   (i <= 1)? (cellDl->dl.cmnChITbs.iTbs3Rbs = 0) :
      (cellDl->dl.cmnChITbs.iTbs3Rbs = i-1);


   pdcchBits = 1 + /* Flag for format0/format1a differentiation */
      1 + /* Localized/distributed VRB assignment flag */
      5 + /* For mcs */
#ifndef LTE_TDD
      3 + /* Harq process Id */
#else
      4 + /* Harq process Id */
      2 + /* UL Index or DAI */
#endif
      1 + /* New Data Indicator */
      2 + /* For RV */
      2 + /* For tpc */
      1 + rgSCHUtlLog32bitNbase2((cell->bwCfg.dlTotalBw * \
               (cell->bwCfg.dlTotalBw + 1))/2);
   /* Resource block assignment ceil[log2(bw(bw+1)/2)] : \
      Since VRB is local */
   /* For TDD consider DAI */

   /* Convert the pdcchBits to actual pdcchBits required for transmission */
   if (dlCmnCodeRate->pdcchCodeRate != 0)
   {
      pdcchBits = (pdcchBits * 1024)/dlCmnCodeRate->pdcchCodeRate;
      if (pdcchBits <= 288) /* 288 : Num of pdcch bits for aggrLvl=4 */
      {
         cellDl->dl.cmnChAggrLvl = CM_LTE_AGGR_LVL4;
      }
      else                  /* 576 : Num of pdcch bits for aggrLvl=8 */
      {
         cellDl->dl.cmnChAggrLvl = CM_LTE_AGGR_LVL8;
      }
   }
   else
   {
      cellDl->dl.cmnChAggrLvl = CM_LTE_AGGR_LVL4;
   }
   if (dlCmnCodeRate->ccchCqi == 0)
   {
      RETVALUE(RFAILED);
   }
   else
   {
      cellDl->dl.ccchCqi = dlCmnCodeRate->ccchCqi;
   }
   RETVALUE(ROK);
}

#ifdef LTE_TDD
/**
 * @brief This function handles the configuration of cell for the first
 *        time by the scheduler.
 *
 * @details
 *
 *     Function: rgSCHCmnDlRgrCellCfg
 *     Purpose:  Configuration received is stored into the data structures
 *               Also, update the scheduler with the number of frames of
 *               RACH preamble transmission.
 *
 *     Invoked by: BO and Scheduler
 *
 *  @param[in]  RgSchCellCb*     cell
 *  @param[in]  RgrCellCfg*      cfg
 *  @return     S16
 *
 **/
#ifdef ANSI
PRIVATE S16 rgSCHCmnDlRgrCellCfg
(
RgSchCellCb    *cell,
RgrCellCfg     *cfg,
RgSchErrInfo   *err
)
#else
PRIVATE S16 rgSCHCmnDlRgrCellCfg(cell, cfg, err)
RgSchCellCb    *cell;
RgrCellCfg     *cfg;
RgSchErrInfo   *err;
#endif
{
   RgSchCmnCell         *cellSch;
   U8                   cp;
   U8                   sfCount;
   U8                   numPdcchSym;
   U8                   noSymPerSlot;
   U8                   maxDlSubfrms = cell->numDlSubfrms;
   U8                   splSubfrmIdx = cfg->spclSfCfgIdx;
   U8                   swPtCnt = 0;
   Bool                 isSplfrm;
   RgSchTddSubfrmInfo   subfrmInfo = rgSchTddMaxUlSubfrmTbl[cell->ulDlCfgIdx];
   S16                  ret;
   U8                   splSfIdx;
   U8                   antPortIdx;
   U8                   numCrs;
   U8                   cfi;  
   U8                   cfiIdx;
   RgSchDlSf            *sf;
   U8                   splSfCfi;
   U8                   mPhich;

   TRC2(rgSCHCmnDlRgrCellCfg);
   

   cellSch = RG_SCH_CMN_GET_CELL(cell);
   cellSch->dl.numRaSubFrms = rgRaPrmblToRaFrmTbl[cell->\
                                                  rachCfg.preambleFormat];
   /*[ccpu00138532]-ADD-fill the Msg4 Harq data */
   cell->dlHqCfg.maxMsg4HqTx = cfg->dlHqCfg.maxMsg4HqTx;                                                
   
   /* Msg4 Tx Delay = (HARQ_RTT * MAX_MSG4_HARQ_RETX)  + 
                       3 TTI (MAX L1+L2 processing delay at the UE) */
   cellSch->dl.msg4TxDelay = (cfg->dlHqCfg.maxMsg4HqTx-1) *
                                 rgSchCmnHarqRtt[cell->ulDlCfgIdx] + 3; 
   cellSch->dl.maxUePerDlSf = cfg->maxUePerDlSf;
   cellSch->dl.maxUeNewTxPerTti = cfg->maxDlUeNewTxPerTti;
   if (cfg->maxUePerDlSf == 0)
   {
      cellSch->dl.maxUePerDlSf = RG_SCH_CMN_MAX_UE_PER_DL_SF;
   }
   if (cellSch->dl.maxUePerDlSf < cellSch->dl.maxUeNewTxPerTti)
   {
      RETVALUE(RFAILED);
   }


   if (cell->bwCfg.dlTotalBw <= 10)
   {
      cfiIdx = 1;
      numPdcchSym = 2;
   }
   else
   {
      cfiIdx = 0;
      numPdcchSym = 1;
   }
   /* DwPTS Scheduling Changes Start */
   cellSch->dl.splSfCfg  = splSubfrmIdx;
 
   if (cfg->isCpDlExtend == TRUE)
   {
      if((0 == splSubfrmIdx) || (4 == splSubfrmIdx) ||
         (7 == splSubfrmIdx) || (8 == splSubfrmIdx)
        )
      {
         cell->splSubfrmCfg.isDlDataAllowed = FALSE; 
      }
      else
      {
         cell->splSubfrmCfg.isDlDataAllowed = TRUE; 
      }
   }
   else
   {
      /* Refer to 36.213 Section 7.1.7 */
      if((0 == splSubfrmIdx) || (5 == splSubfrmIdx))
      {
         cell->splSubfrmCfg.isDlDataAllowed = FALSE; 
      }
      else
      {
         cell->splSubfrmCfg.isDlDataAllowed = TRUE; 
      }
   }
   /* DwPTS Scheduling Changes End */  

   splSfCfi = RGSCH_MIN(cell->dynCfiCb.maxCfi, cellSch->cfiCfg.cfi);
   RGSCH_GET_SPS_SF_CFI(cell->bwCfg.dlTotalBw, splSfCfi);
   
   for (sfCount = 0; sfCount < maxDlSubfrms; sfCount++)
   {
      sf = cell->subFrms[sfCount];
      /* Sfcount matches the first special subframe occurs at Index 0
            * or subsequent special subframes */
      if(subfrmInfo.switchPoints == 1)
      {
         isSplfrm = rgSCHCmnIsSplSubfrm(swPtCnt, sfCount,
                                 RG_SCH_CMN_10_MS_PRD, &subfrmInfo);
      }
      else
      {
         isSplfrm = rgSCHCmnIsSplSubfrm(swPtCnt, sfCount,
                                 RG_SCH_CMN_5_MS_PRD, &subfrmInfo);
      }
      if(isSplfrm == TRUE)
      {
         swPtCnt++;
         /* DwPTS Scheduling Changes Start */        
         if (cell->splSubfrmCfg.isDlDataAllowed == TRUE)
         {
            sf->sfType = RG_SCH_SPL_SF_DATA;
         }
         else
         {
            sf->sfType = RG_SCH_SPL_SF_NO_DATA;
         }
         /* DwPTS Scheduling Changes End */
      }
      else
      {
         /* DwPTS Scheduling Changes Start */
         if (sf->sfNum != 0)
         {
            sf->sfType = RG_SCH_DL_SF;
         }
         else
         {
            sf->sfType = RG_SCH_DL_SF_0;
         }
         /* DwPTS Scheduling Changes End */
      }
      
      /* Calculate the number of CCEs per subframe in the cell */
      mPhich = rgSchTddPhichMValTbl[cell->ulDlCfgIdx][sf->sfNum];
      if(cell->dynCfiCb.isDynCfiEnb == TRUE)
      {   
         /* In case if Dynamic CFI feature is enabled, default CFI 
          * value 1 is used  */
         sf->nCce = cell->dynCfiCb.cfi2NCceTbl[mPhich][1];
      }
      else
      {
         if (sf->sfType == RG_SCH_SPL_SF_DATA)
         {
            sf->nCce = cell->dynCfiCb.cfi2NCceTbl[mPhich][splSfCfi];
         }
         else
         {
            sf->nCce = cell->dynCfiCb.cfi2NCceTbl[mPhich][RGSCH_MIN(cell->dynCfiCb.maxCfi, cellSch->cfiCfg.cfi)];
         }
      }   
   }

   /* Intialize the RACH response scheduling related infromation */
   if(rgSCHCmnDlRachInfoInit(cell) != ROK)
   {
     RETVALUE(RFAILED);
   }

   /* Allocate PRACH preamble list */
   rgSCHCmnDlCreateRachPrmLst(cell);

   /* Initialize PHICH offset information */
   rgSCHCmnDlPhichOffsetInit(cell);

   /* Update the size of HARQ ACK/NACK feedback table */
   /* The array size is increased by 2 to have enough free indices, where other
    * indices are busy waiting for HARQ feedback */
   cell->ackNackFdbkArrSize = rgSchTddANFdbkMapTbl[cell->ulDlCfgIdx] + 2; 

   /* Initialize expected HARQ ACK/NACK feedback time */
   rgSCHCmnDlANFdbkInit(cell);

   /* Initialize UL association set index */
   if(cell->ulDlCfgIdx != 0)
   {
      rgSCHCmnDlKdashUlAscInit(cell);
   }

   if (cfg->isCpDlExtend == TRUE)
   {
      cp = RG_SCH_CMN_EXT_CP;
      noSymPerSlot = 6;
      cell->splSubfrmCfg.dwPts =
          rgSchTddSplSubfrmInfoTbl[splSubfrmIdx].extDlDwPts;
   
      if ( cell->splSubfrmCfg.dwPts == 0 )
      {
         cell->isDwPtsCnted = FALSE;
      }
      else
      {
         cell->isDwPtsCnted = TRUE;
      }

      if(cfg->isCpUlExtend == TRUE)
      {
         cell->splSubfrmCfg.upPts =
            rgSchTddSplSubfrmInfoTbl[splSubfrmIdx].extDlExtUpPts;
      }
      else
      {
         cell->splSubfrmCfg.upPts =
            rgSchTddSplSubfrmInfoTbl[splSubfrmIdx].extDlNorUpPts;
      }
   }
   else
   {
      cp = RG_SCH_CMN_NOR_CP;
      noSymPerSlot = 7;
      cell->splSubfrmCfg.dwPts =
          rgSchTddSplSubfrmInfoTbl[splSubfrmIdx].norDlDwPts;
      cell->isDwPtsCnted = TRUE;

      if(cfg->isCpUlExtend == TRUE)
      {
         cell->splSubfrmCfg.upPts =
            rgSchTddSplSubfrmInfoTbl[splSubfrmIdx].norDlExtUpPts;
      }
      else
      {
         cell->splSubfrmCfg.upPts =
            rgSchTddSplSubfrmInfoTbl[splSubfrmIdx].norDlNorUpPts;
      }
   }

   /* Initializing the cqiToEffTbl and cqiToTbsTbl for every CFI value */
   for(cfi = 1; cfi < RG_SCH_CMN_MAX_CFI; cfi++,cfiIdx++)
   {   
      cellSch->dl.cqiToTbsTbl[0][cfi]   = rgSchCmnCqiToTbs[0][cp][cfiIdx];
      cellSch->dl.cqiToEffTbl[0][cfi]   = rgSchCmnEffTbl[0][cp][rgSchCmnAntIdx\
                                                 [cell->numTxAntPorts]][cfiIdx];
      cellSch->dl.cqiToTbsTbl[1][cfi]   = rgSchCmnCqiToTbs[1][cp][cfiIdx];
      cellSch->dl.cqiToEffTbl[1][cfi]   = rgSchCmnEffTbl[1][cp][rgSchCmnAntIdx\
                                                 [cell->numTxAntPorts]][cfiIdx];
   }

   /* Initializing the values of CFI parameters */
   if(cell->dynCfiCb.isDynCfiEnb)
   {   
      /* If DCFI is enabled, current CFI value will start from 1 */
      cellSch->dl.currCfi = cellSch->dl.newCfi = 1;
   }
   else
   {
      /* If DCFI is disabled, current CFI value is set as default max allowed CFI value */
      cellSch->dl.currCfi = RGSCH_MIN(cell->dynCfiCb.maxCfi, cellSch->cfiCfg.cfi);
      cellSch->dl.newCfi = cellSch->dl.currCfi;
   }   

   /* Include CRS REs while calculating Efficiency
    * The number of Resource Elements occupied by CRS depends on Number of
    * Antenna Ports. Please refer to Section 6.10.1 of 3GPP TS 36.211 V8.8.0.
    * Also, please refer to Figures 6.10.1.2-1 and 6.10.1.2-2 for diagrammatic
    * details of the same. Please note that PDCCH overlap symbols would not
    * considered in CRS REs deduction */
   for (cfi = 1; cfi < RG_SCH_CMN_MAX_CFI; cfi++, numPdcchSym++)
   {
      cellSch->dl.noResPerRb[cfi] = (((noSymPerSlot * RG_SCH_CMN_NUM_SLOTS_PER_SF)
            - numPdcchSym) *RB_SCH_CMN_NUM_SCS_PER_RB) - rgSchCmnNumResForCrs[cell->numTxAntPorts];
   }

   /* DwPTS Scheduling Changes Start */
   antPortIdx = (cell->numTxAntPorts == 1)? 0: 
      ((cell->numTxAntPorts == 2)? 1: 2);     

   if (cp == RG_SCH_CMN_NOR_CP)
   {
      splSfIdx = (splSubfrmIdx == 4)? 1: 0;   
   }
   else
   {
      splSfIdx = (splSubfrmIdx == 3)? 1: 0;
   }

   numCrs = rgSchCmnDwptsCrs[splSfIdx][antPortIdx];

   for (cfi = 1; cfi < RG_SCH_CMN_MAX_CFI-1; cfi++)
   { 
      /* If CFI is 2 and Ant Port is 4, don't consider the sym 1 CRS REs */  
      if (antPortIdx == 2 && cfi == 2)
      {
         numCrs -= 4;      
      }
      cellSch->dl.numReDwPts[cfi]  =  ((cell->splSubfrmCfg.dwPts - cfi)*
                                  RB_SCH_CMN_NUM_SCS_PER_RB) - numCrs;
   }
   /* DwPTS Scheduling Changes End */

   if (cfg->maxDlBwPerUe == 0)
   {
      cellSch->dl.maxDlBwPerUe = RG_SCH_CMN_MAX_DL_BW_PERUE;
   }
   else
   {
      cellSch->dl.maxDlBwPerUe = cfg->maxDlBwPerUe;
   }
   if (cfg->maxDlRetxBw == 0)
   {
      cellSch->dl.maxDlRetxBw = RG_SCH_CMN_MAX_DL_RETX_BW;
   }
   else
   {
      cellSch->dl.maxDlRetxBw = cfg->maxDlRetxBw;
   }
   /* Fix: MUE_PERTTI_DL*/
   cellSch->dl.maxUePerDlSf = cfg->maxUePerDlSf;
   cellSch->dl.maxUeNewTxPerTti = cfg->maxDlUeNewTxPerTti;
   if (cfg->maxUePerDlSf == 0)
   {
      cellSch->dl.maxUePerDlSf = RG_SCH_CMN_MAX_UE_PER_DL_SF;
   }
   RG_SCH_RESET_HCSG_DL_PRB_CNTR(&cellSch->dl);
   /*[ccpu00138609]-ADD- Configure the Max CCCH Counter */
   if (cfg->maxCcchPerDlSf > cfg->maxUePerDlSf)
   {
      RLOG_ARG2(L_ERROR,DBG_CELLID,cell->cellId, 
                      "Invalid configuration !: "
                      "maxCcchPerDlSf %u > maxUePerDlSf %u",
                   cfg->maxCcchPerDlSf, cfg->maxUePerDlSf );

      RETVALUE(RFAILED);
   }
   else if (!cfg->maxCcchPerDlSf)
   {
      /* ccpu00143032: maxCcchPerDlSf 0 means not configured by application
       * hence setting to maxUePerDlSf. If maxCcchPerDlSf is 0 then scheduler
       * does't consider CCCH allocation in MaxUePerTti cap. Hence more than
       * 4UEs getting schduled & SCH expects >16 Hq PDUs in a TTI which causes
       * FLE crash in PHY as PHY has limit of 16 max*/
      cellSch->dl.maxCcchPerDlSf = cfg->maxUePerDlSf;
   }
   else
   {
      cellSch->dl.maxCcchPerDlSf = cfg->maxCcchPerDlSf;
   }
   if (rgSCHCmnDlCnsdrCmnRt(cell, &cfg->dlCmnCodeRate) != ROK)
   {
      RETVALUE(RFAILED);
   }

   /*ccpu00118273 - ADD - start */
   cmLListInit(&cellSch->dl.msg4RetxLst);
#ifdef RGR_V1
   cmLListInit(&cellSch->dl.ccchSduRetxLst);
#endif

#ifdef RG_PHASE2_SCHED
   if (cellSch->apisDlfs == NULLP) /* DFLS specific initialization */
   {
      cellSch->apisDlfs = &rgSchDlfsSchdTbl[cfg->dlfsSchdType];
   }
   if (cfg->dlfsCfg.isDlFreqSel)
   {
      ret = cellSch->apisDlfs->rgSCHDlfsCellCfg(cell, cfg, err);
      if (ret != ROK)
      {
         RETVALUE(RFAILED);
      }
   }
   cellSch->dl.isDlFreqSel = cfg->dlfsCfg.isDlFreqSel;
#endif

   /* Power related configuration */
   ret = rgSCHPwrCellCfg(cell, cfg);
   if (ret != ROK)
   {
      RETVALUE(RFAILED);
   }

   cellSch->dl.bcchTxPwrOffset = cfg->bcchTxPwrOffset; 
   cellSch->dl.pcchTxPwrOffset = cfg->pcchTxPwrOffset; 
   cellSch->dl.rarTxPwrOffset  = cfg->rarTxPwrOffset; 
   cellSch->dl.phichTxPwrOffset  = cfg->phichTxPwrOffset; 
   cellSch->dl.msg4pAVal        = cfg->msg4pAVal;
   RETVALUE(ROK);
}
#else /* LTE_TDD */
/**
 * @brief This function handles the configuration of cell for the first
 *        time by the scheduler.
 *
 * @details
 *
 *     Function: rgSCHCmnDlRgrCellCfg
 *     Purpose:  Configuration received is stored into the data structures
 *               Also, update the scheduler with the number of frames of
 *               RACH preamble transmission.
 *
 *     Invoked by: BO and Scheduler
 *
 *  @param[in]  RgSchCellCb*   cell
 *  @param[in]  RgrCellCfg*    cfg
 *  @param[in]  RgSchErrInfo*  err
 *  @return     S16
 *
 **/
#ifdef ANSI
PRIVATE S16 rgSCHCmnDlRgrCellCfg
(
RgSchCellCb             *cell,
RgrCellCfg              *cfg,
RgSchErrInfo            *err
)
#else
PRIVATE S16 rgSCHCmnDlRgrCellCfg(cell, cfg, err)
RgSchCellCb             *cell;
RgrCellCfg              *cfg;
RgSchErrInfo            *err;
#endif
{
   S16                 ret;
   RgSchCmnCell        *cellSch;
   U8                   cp;
   U8                   numPdcchSym;
   U8                   noSymPerSlot;
   U8                   cfi;  
   U8                   cfiIdx;

   TRC2(rgSCHCmnDlRgrCellCfg);

   cellSch = RG_SCH_CMN_GET_CELL(cell);

   /* Initialize the parameters with the ones received in the */
   /* configuration.                                          */

   /* Added matrix 'rgRaPrmblToRaFrmTbl' for computation of RA
    * sub-frames from preamble format */
   cellSch->dl.numRaSubFrms = rgRaPrmblToRaFrmTbl[cell->rachCfg.preambleFormat];

   /*[ccpu00138532]-ADD-fill the Msg4 Harq data */
   cell->dlHqCfg.maxMsg4HqTx = cfg->dlHqCfg.maxMsg4HqTx;                                                
   
   /* Msg4 Tx Delay = (HARQ_RTT * MAX_MSG4_HARQ_RETX)  + 
                       3 TTI (MAX L1+L2 processing delay at the UE) */
   cellSch->dl.msg4TxDelay = (cfg->dlHqCfg.maxMsg4HqTx-1) *
                                 rgSchCmnHarqRtt[7] + 3; 

   if (cell->bwCfg.dlTotalBw <= 10)
   {
      cfiIdx = 1;
      numPdcchSym = 2;
   }
   else
   {
      cfiIdx = 0;
      numPdcchSym = 1;
   }

   if (cell->isCpDlExtend == TRUE)
   {
      cp = RG_SCH_CMN_EXT_CP;
      noSymPerSlot = 6;
   }
   else
   {
      cp = RG_SCH_CMN_NOR_CP;
      noSymPerSlot = 7;
   }

   /* Initializing the cqiToEffTbl and cqiToTbsTbl for every CFI value */
   for(cfi = 1; cfi < RG_SCH_CMN_MAX_CFI; cfi++, cfiIdx++)
   {   
      cellSch->dl.cqiToTbsTbl[0][cfi]   = rgSchCmnCqiToTbs[0][cp][cfiIdx];
      cellSch->dl.cqiToEffTbl[0][cfi]   = rgSchCmnEffTbl[0][cp][rgSchCmnAntIdx\
                                                 [cell->numTxAntPorts]][cfiIdx];
      cellSch->dl.cqiToTbsTbl[1][cfi]   = rgSchCmnCqiToTbs[1][cp][cfiIdx];
      cellSch->dl.cqiToEffTbl[1][cfi]   = rgSchCmnEffTbl[1][cp][rgSchCmnAntIdx\
                                                 [cell->numTxAntPorts]][cfiIdx];
   }

   /* Initializing the values of CFI parameters */
   if(cell->dynCfiCb.isDynCfiEnb)
   {   
      /* If DCFI is enabled, current CFI value will start from 1 */
      cellSch->dl.currCfi = cellSch->dl.newCfi = 1;
   }
   else
   {
      /* If DCFI is disabled, current CFI value is set as default CFI value */
      cellSch->dl.currCfi = cellSch->cfiCfg.cfi;
      cellSch->dl.newCfi = cellSch->dl.currCfi;
   }   

   /* Include CRS REs while calculating Efficiency
    * The number of Resource Elements occupied by CRS depends on Number of
    * Antenna Ports. Please refer to Section 6.10.1 of 3GPP TS 36.211 V8.8.0.
    * Also, please refer to Figures 6.10.1.2-1 and 6.10.1.2-2 for diagrammatic
    * details of the same. Please note that PDCCH overlap symbols would not
    * considered in CRS REs deduction */
   for (cfi = 1; cfi < RG_SCH_CMN_MAX_CFI; cfi++, numPdcchSym++)
   {
       cellSch->dl.noResPerRb[cfi]    = (((noSymPerSlot * RG_SCH_CMN_NUM_SLOTS_PER_SF)
            - numPdcchSym) * RB_SCH_CMN_NUM_SCS_PER_RB) - rgSchCmnNumResForCrs[cell->numTxAntPorts];
   }           

   if (cfg->maxDlBwPerUe == 0)
   {
      cellSch->dl.maxDlBwPerUe = RG_SCH_CMN_MAX_DL_BW_PERUE;
   }
   else
   {
      cellSch->dl.maxDlBwPerUe = cfg->maxDlBwPerUe;
   }
   if (cfg->maxDlRetxBw == 0)
   {
      cellSch->dl.maxDlRetxBw = RG_SCH_CMN_MAX_DL_RETX_BW;
   }
   else
   {
      cellSch->dl.maxDlRetxBw = cfg->maxDlRetxBw;
   }
   
   /* Fix: MUE_PERTTI_DL*/
   cellSch->dl.maxUePerDlSf = cfg->maxUePerDlSf;
   cellSch->dl.maxUeNewTxPerTti = cfg->maxDlUeNewTxPerTti;
   if (cfg->maxUePerDlSf == 0)
   {
      cellSch->dl.maxUePerDlSf = RG_SCH_CMN_MAX_UE_PER_DL_SF;
   }
   /* Fix: MUE_PERTTI_DL syed validating Cell Configuration */
   if (cellSch->dl.maxUePerDlSf < cellSch->dl.maxUeNewTxPerTti)
   {
      RLOG_ARG2(L_ERROR,DBG_CELLID,cell->cellId,
            "FAILED MaxUePerDlSf(%u) < MaxDlUeNewTxPerTti(%u)",
            cellSch->dl.maxUePerDlSf,
            cellSch->dl.maxUeNewTxPerTti);
      RETVALUE(RFAILED);
   }
   /*[ccpu00138609]-ADD- Configure the Max CCCH Counter */
   if (cfg->maxCcchPerDlSf > cfg->maxUePerDlSf)
   {
      RLOG_ARG2(L_ERROR,DBG_CELLID,cell->cellId,"Invalid configuration !: "
            "maxCcchPerDlSf %u > maxUePerDlSf %u",
            cfg->maxCcchPerDlSf, cfg->maxUePerDlSf );

      RETVALUE(RFAILED);
   }
   else if (!cfg->maxCcchPerDlSf)
   {
      /* ccpu00143032: maxCcchPerDlSf 0 means not configured by application
       * hence setting to maxUePerDlSf. If maxCcchPerDlSf is 0 then scheduler
       * does't consider CCCH allocation in MaxUePerTti cap. Hence more than
       * 4UEs getting schduled & SCH expects >16 Hq PDUs in a TTI which causes
       * FLE crash in PHY as PHY has limit of 16 max*/
      cellSch->dl.maxCcchPerDlSf = cfg->maxUePerDlSf;
   }
   else
   {
      cellSch->dl.maxCcchPerDlSf = cfg->maxCcchPerDlSf;
   }


   if (rgSCHCmnDlCnsdrCmnRt(cell, &cfg->dlCmnCodeRate) != ROK)
   {
      RETVALUE(RFAILED);
   }
   cmLListInit(&cellSch->dl.msg4RetxLst);
#ifdef RGR_V1
   cmLListInit(&cellSch->dl.ccchSduRetxLst);
#endif

#ifdef RG_PHASE2_SCHED
   if (cellSch->apisDlfs == NULLP) /* DFLS specific initialization */
   {
      cellSch->apisDlfs = &rgSchDlfsSchdTbl[cfg->dlfsSchdType];
   }
   if (cfg->dlfsCfg.isDlFreqSel)
   {
      ret = cellSch->apisDlfs->rgSCHDlfsCellCfg(cell, cfg, err);
      if (ret != ROK)
      {
         RETVALUE(RFAILED);
      }
   }
   cellSch->dl.isDlFreqSel = cfg->dlfsCfg.isDlFreqSel;
#endif

   /* Power related configuration */
   ret = rgSCHPwrCellCfg(cell, cfg);
   if (ret != ROK)
   {
      RETVALUE(RFAILED);
   }

   cellSch->dl.bcchTxPwrOffset = cfg->bcchTxPwrOffset; 
   cellSch->dl.pcchTxPwrOffset = cfg->pcchTxPwrOffset; 
   cellSch->dl.rarTxPwrOffset  = cfg->rarTxPwrOffset; 
   cellSch->dl.phichTxPwrOffset  = cfg->phichTxPwrOffset; 
   RG_SCH_RESET_HCSG_DL_PRB_CNTR(&cellSch->dl);
   RETVALUE(ROK);
}
#endif /* LTE_TDD */

/***********************************************************
 *
 *     Func : rgSCHCmnUlCalcReqRbCeil
 *
 *     Desc : Calculate RB required to satisfy 'bytes' for
 *            a given CQI.
 *            Returns number of RBs such that requirement
 *            is necessarily satisfied (does a 'ceiling'
 *            computation).
 *
 *     Ret  : Required RBs (U8)
 *
 *     Notes:
 *
 *     File :
 *
 **********************************************************/
#ifdef ANSI
PRIVATE U8 rgSCHCmnUlCalcReqRbCeil
(
U32            bytes,
U8             cqi,
RgSchCmnUlCell *cellUl
)
#else
PRIVATE U8 rgSCHCmnUlCalcReqRbCeil(bytes, cqi, cellUl)
U32            bytes;
U8             cqi;
RgSchCmnUlCell *cellUl;
#endif
{
   U32 numRe = RGSCH_CEIL((bytes * 8) * 1024, rgSchCmnUlCqiTbl[cqi].eff);
   TRC2(rgSCHCmnUlCalcReqRbCeil);
   RETVALUE((U8)RGSCH_CEIL(numRe, RG_SCH_CMN_UL_NUM_RE_PER_RB(cellUl)));
}

/***********************************************************
 *
 *     Func : rgSCHCmnPrecompMsg3Vars
 *
 *     Desc : Precomputes the following for msg3 allocation:
 *            1. numSb and Imcs for msg size A
 *            2. numSb and Imcs otherwise
 *
 *     Ret  :
 *
 *     Notes: The corresponding vars in cellUl struct is filled
 *            up
 *
 *     File :
 *
 **********************************************************/
#ifdef ANSI
PRIVATE S16 rgSCHCmnPrecompMsg3Vars
(
RgSchCmnUlCell *cellUl,
U8           ccchCqi,
U16          msgSzA,
U8           sbSize,
Bool         isEcp
)
#else
PRIVATE S16 rgSCHCmnPrecompMsg3Vars(cellUl, ccchCqi, msgSzA, sbSize, isEcp)
RgSchCmnUlCell *cellUl;
U8           ccchCqi;
U16          msgSzA;
U8           sbSize;
Bool         isEcp;
#endif
{
   U8 numSb;
   U8 ccchTbs;
   U8 ccchMcs;
   U8   numRb = 0;
   U8   iTbs = 0;
   U16  msg3GrntSz = 0;

   TRC2(rgSCHCmnPrecompMsg3Vars);

   if (ccchCqi > cellUl->max16qamCqi)
   {
      ccchCqi = cellUl->max16qamCqi;
   }
/* #ifndef RG_SCH_CMN_EXP_CP_SUP For ECP Pick the index 1 */
   /* Fix */
   ccchTbs = rgSchCmnUlCqiToTbsTbl[(U8)isEcp][ccchCqi];
   ccchMcs = rgSCHCmnUlGetIMcsFrmITbs(ccchTbs, CM_LTE_UE_CAT_1);
   /* MCS should fit in 4 bits in RAR */
   if (ccchMcs >= 15)
   {
      ccchMcs = 15;
   }

   /* Limit the ccchMcs to 15 as it
    * can be inferred from 36.213, section 6.2 that msg3 imcs
    * field is 4 bits.
    * Since, UE doesn't exist right now, we use CAT_1 for ue
    * category*/
   while((ccchMcs = (rgSCHCmnUlGetIMcsFrmITbs(
                      rgSchCmnUlCqiToTbsTbl[(U8)isEcp][ccchCqi],CM_LTE_UE_CAT_1))
                    ) >
                 RG_SCH_CMN_MAX_MSG3_IMCS)
   {
      ccchCqi--;
   }
   
   iTbs = rgSchCmnUlCqiToTbsTbl[(U8)isEcp][ccchCqi];
   
   if (msgSzA < RGSCH_MIN_MSG3_GRNT_SZ)
   {
      RETVALUE(RFAILED);
   }
   numSb = RGSCH_CEIL(rgSCHCmnUlCalcReqRbCeil(msgSzA, ccchCqi, cellUl), sbSize);
   
   numRb   = numSb * sbSize;
   msg3GrntSz = 8 * msgSzA;

   while( (rgTbSzTbl[0][iTbs][numRb - 1]) < msg3GrntSz)
   {
      ++numSb;
      numRb   = numSb * sbSize;
   }
   while (rgSchCmnMult235Tbl[numSb].match != numSb)
   {
      ++numSb;
   }
   /* Reversed(Corrected) the assignment for preamble-GrpA
    * Refer- TG36.321- section- 5.1.2*/
   cellUl->ra.prmblBNumSb = numSb;
   cellUl->ra.prmblBIMcs  = ccchMcs;
   numSb = RGSCH_CEIL(rgSCHCmnUlCalcReqRbCeil(RGSCH_MIN_MSG3_GRNT_SZ, \
                      ccchCqi, cellUl),
         sbSize);

   numRb   = numSb * sbSize;
   msg3GrntSz = 8 * RGSCH_MIN_MSG3_GRNT_SZ;
   while( (rgTbSzTbl[0][iTbs][numRb - 1]) < msg3GrntSz)
   {
      ++numSb;
      numRb   = numSb * sbSize;
   }
   while (rgSchCmnMult235Tbl[numSb].match != numSb)
   {
      ++numSb;
   }
   /* Reversed(Corrected) the assignment for preamble-GrpA
    * Refer- TG36.321- section- 5.1.2*/
   cellUl->ra.prmblANumSb = numSb;
   cellUl->ra.prmblAIMcs  = ccchMcs;
   RETVALUE(ROK);
}

#ifdef LTE_TDD
/***********************************************************
 *
 *     Func : rgSCHCmnUlCalcAvailBw
 *
 *     Desc : Calculates bandwidth available for PUSCH scheduling.
 *
 *     Ret  : S16 (ROK/RFAILED)
 *
 *     Notes:
 *
 *     File :
 *
 **********************************************************/
#ifdef ANSI
PRIVATE S16 rgSCHCmnUlCalcAvailBw
(
RgSchCellCb    *cell,
RgrCellCfg     *cellCfg,
U8              cfi,
U8             *rbStartRef,
U8             *bwAvailRef
)
#else
PRIVATE S16 rgSCHCmnUlCalcAvailBw(cell, cellCfg, cfi, rbStartRef, bwAvailRef)
RgSchCellCb   *cell;
RgrCellCfg    *cellCfg;
U8             cfi;  
U8            *rbStartRef;
U8            *bwAvailRef;
#endif
{
   U8  c        = 3;
   U8  ulBw     = cell->bwCfg.ulTotalBw;
   U8  n2Rb     = cell->pucchCfg.resourceSize;
   U8  pucchDeltaShft = cell->pucchCfg.deltaShift;
   U16 n1Pucch  = cell->pucchCfg.n1PucchAn;
   U8  n1Cs     = cell->pucchCfg.cyclicShift;

   U8  n1PerRb;
   U8  totalCce;
   U16 n1Max;
   U8  n1Rb;
   U32 mixedRb;
   U8  exclRb; /* RBs to exclude */
   U8  n1RbPart;
   U8  puschRbStart;
   /* To avoid PUCCH and PUSCH collision issue */
   U8  P;
   U8  n1PlusOne;
   U8  mi;
   /* Maximum value of M as per Table 10.1-1 */
   U8  M[RGSCH_MAX_TDD_UL_DL_CFG] = {1, 2, 4, 3, 4, 9, 1};

   TRC2(rgSCHCmnUlCalcAvailBw);

   if (cell->isCpUlExtend)
   {
      c = 2;
   }

   n1PerRb  = c * 12 / pucchDeltaShft; /* 12/18/36 */

   /* Considering the max no. of CCEs for PUSCH BW calculation 
    * based on min mi value */
   if (cell->ulDlCfgIdx == 0 || cell->ulDlCfgIdx == 6)
   {
      mi = 1;
   }
   else
   { 
      mi = 0;
   }
   
   totalCce = cell->dynCfiCb.cfi2NCceTbl[mi][cfi];

   P        = rgSCHCmnGetPValFrmCCE(cell, totalCce-1);
   n1PlusOne = cell->rgSchTddNpValTbl[P + 1];
   n1Max    = (M[cell->ulDlCfgIdx] - 1)*n1PlusOne + (totalCce-1) + n1Pucch;

   /* ccpu00129978- MOD- excluding RBs based on formula in section 5.4.3 in 
    * TS 36.211  */
   n1RbPart = (c*n1Cs)/pucchDeltaShft;
   n1Rb = (n1Max - n1RbPart)/ n1PerRb;
   mixedRb = RGSCH_CEIL(n1Cs, 8); /* same as 'mixedRb = n1Cs ? 1 : 0' */

   /* get the total Number of RB's to be excluded for PUSCH */
   /* ccpu00137339 */
   if(n1Pucch < n1RbPart)
   {
      exclRb = n2Rb;
   }
   else
   {
      exclRb = n2Rb + mixedRb + n1Rb; /* RBs to exclude */
   }
   puschRbStart = exclRb/2 + 1; 

   /* Num of PUCCH RBs = puschRbStart*2 */
   if (puschRbStart * 2 >= ulBw)
   {
      RLOG_ARG0(L_ERROR,DBG_CELLID,cell->cellId,"No bw available for PUSCH");
      RETVALUE(RFAILED);
   }

   *rbStartRef = puschRbStart;
   *bwAvailRef = ulBw -  puschRbStart * 2;
 
   if(cell->pucchCfg.maxPucchRb !=0 && 
         (puschRbStart * 2 > cell->pucchCfg.maxPucchRb))
   {
      cell->dynCfiCb.maxCfi = RGSCH_MIN(cfi-1, cell->dynCfiCb.maxCfi);
   }
    
   RETVALUE(ROK);
}
#else
/***********************************************************
 *
 *     Func : rgSCHCmnUlCalcAvailBw
 *
 *     Desc : Calculates bandwidth available for PUSCH scheduling.
 *
 *     Ret  : S16 (ROK/RFAILED)
 *
 *     Notes:
 *
 *     File :
 *
 **********************************************************/
#ifdef ANSI
PRIVATE S16 rgSCHCmnUlCalcAvailBw
(
RgSchCellCb    *cell,
RgrCellCfg     *cellCfg,
U8              cfi,
U8             *rbStartRef,
U8             *bwAvailRef
)
#else
PRIVATE S16 rgSCHCmnUlCalcAvailBw(cell, cellCfg, cfi, rbStartRef, bwAvailRef)
RgSchCellCb   *cell;
RgrCellCfg    *cellCfg;
U8             cfi;
U8            *rbStartRef;
U8            *bwAvailRef;
#endif
{
   U8  c        = 3;
   U8  ulBw     = cell->bwCfg.ulTotalBw;
   U8  n2Rb     = cell->pucchCfg.resourceSize;
   U8  pucchDeltaShft = cell->pucchCfg.deltaShift;
   U16 n1Pucch  = cell->pucchCfg.n1PucchAn;
   U8  n1Cs     = cell->pucchCfg.cyclicShift;
   U8  n1PerRb;
   U8  totalCce;
   U16 n1Max;
   U8  n1Rb;
   U32 mixedRb;
   U8  exclRb; /* RBs to exclude */
   U8  n1RbPart;
   U8  puschRbStart;
   
   TRC2(rgSCHCmnUlCalcAvailBw);

   if (cell->isCpUlExtend)
   {
      c = 2;
   }

   n1PerRb  = c * 12 / pucchDeltaShft; /* 12/18/36 */

   totalCce = cell->dynCfiCb.cfi2NCceTbl[0][cfi];

   n1Max    = n1Pucch + totalCce-1;

   /* ccpu00129978- MOD- excluding RBs based on formula in section 5.4.3 in 
    * TS 36.211  */
   n1RbPart = (c*n1Cs)/pucchDeltaShft;
   n1Rb = (U8)((n1Max - n1RbPart) / n1PerRb);
   mixedRb = RGSCH_CEIL(n1Cs, 8); /* same as 'mixedRb = n1Cs ? 1 : 0' */

   /* get the total Number of RB's to be excluded for PUSCH */
   /* ccpu00137339 */
   if(n1Pucch < n1RbPart)
   {
      exclRb = n2Rb;
   }
   else
   {
      exclRb = n2Rb + mixedRb + n1Rb; /* RBs to exclude */
   }
   puschRbStart = exclRb/2 + 1;

   if (puschRbStart*2 >= ulBw)
   {
      RLOG_ARG0(L_ERROR,DBG_CELLID,cell->cellId,"No bw available for PUSCH");
      RETVALUE(RFAILED);
   }

   *rbStartRef = puschRbStart;
   *bwAvailRef = ulBw - puschRbStart * 2;

   if(cell->pucchCfg.maxPucchRb !=0 && 
      (puschRbStart * 2 > cell->pucchCfg.maxPucchRb))
   {
      cell->dynCfiCb.maxCfi = RGSCH_MIN(cfi-1, cell->dynCfiCb.maxCfi);
   }
   
   RETVALUE(ROK);
}
#endif



/***********************************************************
 *
 *     Func : rgSCHCmnUlCellInit
 *
 *     Desc : Uplink scheduler initialisation for cell.
 *
 *     Ret  : S16
 *
 *     Notes:
 *
 *     File :
 *
 **********************************************************/
#ifdef ANSI
PRIVATE S16 rgSCHCmnUlCellInit
(
 RgSchCellCb  *cell,
 RgrCellCfg   *cellCfg
 )
#else
PRIVATE S16 rgSCHCmnUlCellInit(cell, cellCfg)
   RgSchCellCb *cell;
   RgrCellCfg  *cellCfg;
#endif
{
   S16            ret;
   RgSchCmnUlCell *cellUl      = RG_SCH_CMN_GET_UL_CELL(cell);
   U8             maxUePerUlSf = cellCfg->maxUePerUlSf;
#ifdef RGR_V1
   /* Added configuration for maximum number of MSG3s */
   U8             maxMsg3PerUlSf = cellCfg->maxMsg3PerUlSf;
#endif
   U8             maxUlBwPerUe = cellCfg->maxUlBwPerUe;
   U8             sbSize       = cellCfg->puschSubBand.size;
   U8             i;
   U8             rbStart;
   U8             bwAvail;
   U8             cfi;  
   U8             maxSbPerUe;
   U8             numSb;
#ifdef LTE_TDD
   U16            ulDlCfgIdx = cell->ulDlCfgIdx;
   /* [ccpu00127294]-MOD-Change the max Ul subfrms size in TDD */
   U8             maxSubfrms = 2 * rgSchTddNumUlSf[ulDlCfgIdx]; 
   U8             ulToDlMap[12] = {0}; /* maximum 6 Subframes in UL  * 2 */
   U8             maxUlsubfrms = rgSchTddNumUlSubfrmTbl[ulDlCfgIdx]\
                                           [RGSCH_NUM_SUB_FRAMES-1];
   U16             subfrm;
   S8             dlIdx;
#else
   U8             maxSubfrms = RG_SCH_CMN_UL_NUM_SF;
#endif
#ifdef LTE_L2_MEAS
   U8             idx;
#endif
   U8  iTbs;
#if (defined(LTE_L2_MEAS) )
   Inst           inst         = cell->instIdx;
#endif /* #if (defined(LTE_L2_MEAS) || defined(DEBUGP) */
   RgSchCmnCell      *cellSch =  (RgSchCmnCell *)(cell->sc.sch);
   
   TRC2(rgSCHCmnUlCellInit);

   cellUl->maxUeNewTxPerTti = cellCfg->maxUlUeNewTxPerTti;
   if (maxUePerUlSf == 0)
   {
      maxUePerUlSf = RG_SCH_CMN_MAX_UE_PER_UL_SF;
   }
#ifdef RGR_V1
   if (maxMsg3PerUlSf == 0)
   {
      maxMsg3PerUlSf = RG_SCH_CMN_MAX_MSG3_PER_UL_SF;
   }
   /*  fixed the problem while sending raRsp 
    * if maxMsg3PerUlSf is greater than 
    * RGSCH_MAX_RNTI_PER_RARNTI 
    * */
   if(maxMsg3PerUlSf > RGSCH_MAX_RNTI_PER_RARNTI)
   {
      maxMsg3PerUlSf = RGSCH_MAX_RNTI_PER_RARNTI; 
   } 

   if(maxMsg3PerUlSf > maxUePerUlSf)
   {
      maxMsg3PerUlSf =  maxUePerUlSf;   
   }
   
   /*cellUl->maxAllocPerUlSf = maxUePerUlSf + maxMsg3PerUlSf;*/
   /*Max MSG3 should be a subset of Max UEs*/
   cellUl->maxAllocPerUlSf = maxUePerUlSf;
   cellUl->maxMsg3PerUlSf = maxMsg3PerUlSf;
#else
   cellUl->maxAllocPerUlSf = maxUePerUlSf;
#endif
   /* Fix: MUE_PERTTI_UL syed validating Cell Configuration */
   if (cellUl->maxAllocPerUlSf < cellUl->maxUeNewTxPerTti)
   {
      RLOG_ARG2(L_ERROR,DBG_CELLID,cell->cellId,
            "FAILED: MaxUePerUlSf(%u) < MaxUlUeNewTxPerTti(%u)",
            cellUl->maxAllocPerUlSf,
            cellUl->maxUeNewTxPerTti);
      RETVALUE(RFAILED);
   }

#ifdef LTE_L2_MEAS
#ifdef LTE_TDD
   for(idx = 0; idx < RGSCH_SF_ALLOC_SIZE; idx++)
#else
   for(idx = 0; idx < RGSCH_NUM_SUB_FRAMES; idx++)
#endif
   {

      ret = rgSCHUtlAllocSBuf(inst,  (Data **)&(cell->sfAllocArr[idx].
              ulUeInfo.ulAllocInfo), (cellUl->maxAllocPerUlSf * sizeof(RgInfUeUlAlloc)));
      if (ret != ROK)
      {
            RLOG_ARG0(L_ERROR,DBG_CELLID,cell->cellId,"Memory allocation failed ");
            RETVALUE(ret);
      }
   }
#endif
   if (maxUlBwPerUe == 0)
   {
      /* ccpu00139362- Setting to configured UL BW instead of MAX BW(100)*/
      maxUlBwPerUe = cell->bwCfg.ulTotalBw;
   }
   cellUl->maxUlBwPerUe = maxUlBwPerUe;

   /* FOR RG_SCH_CMN_EXT_CP_SUP */
   if (!cellCfg->isCpUlExtend)
   {
      cellUl->ulNumRePerRb = 12 * (14 - RGSCH_UL_SYM_DMRS_SRS);
   }
   else
   {
      cellUl->ulNumRePerRb = 12 * (12 - RGSCH_UL_SYM_DMRS_SRS);
   }

   if (sbSize != rgSchCmnMult235Tbl[sbSize].match)
   {
      RLOG_ARG1(L_ERROR,DBG_CELLID,cell->cellId,"Invalid subband size %d", sbSize);
      RETVALUE(RFAILED);
   }
   maxSbPerUe = maxUlBwPerUe / sbSize;
   if (maxSbPerUe == 0)
   {
      RLOG_ARG0(L_ERROR,DBG_CELLID,cell->cellId, "rgSCHCmnUlCellInit(): "
         "maxUlBwPerUe/sbSize is zero");
      RETVALUE(RFAILED);
   }
   cellUl->maxSbPerUe = rgSchCmnMult235Tbl[maxSbPerUe].prvMatch;

   /* CQI related updations */
   if ((!RG_SCH_CMN_UL_IS_CQI_VALID(cellCfg->ulCmnCodeRate.ccchCqi))
         || (!RG_SCH_CMN_UL_IS_CQI_VALID(cellCfg->trgUlCqi.trgCqi)))
   {
      RLOG_ARG0(L_ERROR,DBG_CELLID,cell->cellId,"rgSCHCmnUlCellInit(): "
         "Invalid cqi");
      RETVALUE(RFAILED);
   }
   cellUl->dfltUlCqi = cellCfg->ulCmnCodeRate.ccchCqi;

   /* Changed the logic to determine maxUlCqi.
    * For a 16qam UE, maxUlCqi is the CQI Index at which
    * efficiency is as close as possible to RG_SCH_MAX_CODE_RATE_16QAM
    * Refer to 36.213-8.6.1 */
    for (i = RG_SCH_CMN_UL_NUM_CQI - 1;i > 0; --i)
   {
      RLOG_ARG2(L_INFO,DBG_CELLID,cell->cellId,
            "CQI %u:iTbs %u",
            i, 
            rgSchCmnUlCqiToTbsTbl[cell->isCpUlExtend][i]);
#ifdef MAC_SCH_STATS
      /* ccpu00128489 ADD Update mcs in hqFailStats here instead of at CRC 
       * since CQI to MCS mapping does not change. The only exception is for 
       * ITBS = 19 where the MCS can be 20 or 21 based on the UE cat. We 
       * choose 20, instead of 21, ie UE_CAT_3 */
      iTbs = rgSchCmnUlCqiToTbsTbl[cell->isCpUlExtend][i];
      RG_SCH_CMN_UL_TBS_TO_MCS(iTbs, hqFailStats.ulCqiStat[i - 1].mcs);
#endif
   }
   for (i = RG_SCH_CMN_UL_NUM_CQI - 1; i != 0; --i)
   {
      /* Fix for ccpu00123912*/
      iTbs = rgSchCmnUlCqiToTbsTbl[cell->isCpUlExtend][i];
      if (iTbs <= RGSCH_UL_16QAM_MAX_ITBS) /* corresponds to 16QAM */
      {
         RLOG_ARG1(L_INFO,DBG_CELLID,cell->cellId,
            		 "16 QAM CQI %u", i);
         cellUl->max16qamCqi = i;
         break;
      }
   }

   /* Precompute useful values for RA msg3 */
   ret = rgSCHCmnPrecompMsg3Vars(cellUl, cellCfg->ulCmnCodeRate.ccchCqi,
         cell->rachCfg.msgSizeGrpA, sbSize, cell->isCpUlExtend);
   if (ret != ROK)
   {
      RETVALUE(ret);
   }

   cellUl->sbSize  = sbSize;
   
#ifdef LTE_TDD  
   cellUl->numUlSubfrms = maxSubfrms;

   ret = rgSCHUtlAllocSBuf(cell->instIdx, (Data **)&cellUl->ulSfArr,
            cellUl->numUlSubfrms * sizeof(RgSchUlSf));

   if (ret != ROK)
   {
      cellUl->numUlSubfrms = 0;
      RETVALUE(ret);
   }

   /* store the DL subframe corresponding to the PUSCH offset
    * in their respective UL subframe */
   for(i=0; i < RGSCH_NUM_SUB_FRAMES; i++)
   {
      if(rgSchTddPuschTxKTbl[ulDlCfgIdx][i] != 0)
      {
         subfrm = (i + rgSchTddPuschTxKTbl[ulDlCfgIdx][i]) % \
                                 RGSCH_NUM_SUB_FRAMES;
         subfrm = rgSchTddNumUlSubfrmTbl[ulDlCfgIdx][subfrm]-1;
         dlIdx = rgSchTddNumDlSubfrmTbl[ulDlCfgIdx][i]-1;
         RGSCH_ARRAY_BOUND_CHECK( cell->instIdx, ulToDlMap, subfrm);
         ulToDlMap[subfrm] = dlIdx;
      }
   }
   /* Copy the information in the remaining UL subframes based
    * on number of HARQ processes */
   for(i=maxUlsubfrms; i < maxSubfrms; i++)
   {
      subfrm = i-maxUlsubfrms;
      RGSCH_ARRAY_BOUND_CHECK(cell->instIdx, ulToDlMap, i);
      RGSCH_ARRAY_BOUND_CHECK(cell->instIdx, ulToDlMap, subfrm)
      ulToDlMap[i] = ulToDlMap[subfrm];
   }
#endif

   for (cfi = 1; cfi < RG_SCH_CMN_MAX_CFI; cfi++)
   {
#ifdef LTE_TDD        
      ret = rgSCHCmnUlCalcAvailBw(cell, cellCfg, cfi, &rbStart, &bwAvail); 
#else
      ret = rgSCHCmnUlCalcAvailBw(cell, cellCfg, cfi, &rbStart, &bwAvail); 
#endif
      if (ret != ROK)
      {
         RETVALUE(ret);
      }

      if (cfi == 1)
      {
         cell->ulAvailBw = bwAvail;
      }

      numSb = bwAvail/sbSize; 

      cell->dynCfiCb.bwInfo[cfi].startRb  = rbStart;
      cell->dynCfiCb.bwInfo[cfi].numSb    = numSb;
   }

   if(0 == cell->dynCfiCb.maxCfi)
   {
      RLOG_ARG3(L_ERROR,DBG_CELLID,cell->cellId, 
               "Incorrect Default CFI(%u), maxCfi(%u), maxPucchRb(%d)",
               cellSch->cfiCfg.cfi, cell->dynCfiCb.maxCfi, 
               cell->pucchCfg.maxPucchRb);
            
      RETVALUE(RFAILED);
   }

   /* DMRS values */
   cellUl->dmrsArrSize = cell->dynCfiCb.bwInfo[1].numSb;
   ret = rgSCHUtlAllocSBuf(cell->instIdx, (Data **)&cellUl->dmrsArr,
         cellUl->dmrsArrSize * sizeof(*cellUl->dmrsArr));
   if (ret != ROK)
   {
      RETVALUE(ret);
   }
   for (i = 0; i < cellUl->dmrsArrSize; ++i)
   {
      cellUl->dmrsArr[i] = cellCfg->puschSubBand.dmrs[i];
   }
 
   /* Init subframes */
   for (i = 0; i < maxSubfrms; ++i)
   {
      ret = rgSCHUtlUlSfInit(cell, &cellUl->ulSfArr[i], i,
                             cellUl->maxAllocPerUlSf);
      if (ret != ROK)
      {
         for (; i != 0; --i)
         {
            rgSCHUtlUlSfDeinit(cell, &cellUl->ulSfArr[i-1]);
         }
         /* ccpu00117052 - MOD - Passing double pointer
            for proper NULLP assignment*/
         rgSCHUtlFreeSBuf(cell->instIdx, (Data **)(&(cellUl->dmrsArr)),
               cellUl->dmrsArrSize * sizeof(*cellUl->dmrsArr));
#ifdef LTE_TDD
         /* ccpu00117052 - MOD - Passing double pointer
            for proper NULLP assignment*/
         rgSCHUtlFreeSBuf(cell->instIdx,
            (Data **)(&(cellUl->ulSfArr)), maxSubfrms * sizeof(RgSchUlSf));
#endif
         RETVALUE(ret);
      }
   }
   RG_SCH_RESET_HCSG_UL_PRB_CNTR(cellUl);
   RETVALUE(ROK);
}

/**
 * @brief Scheduler processing on cell configuration.
 *
 * @details
 *
 *     Function : rgSCHCmnRgrCellCfg
 *
 *     This function does requisite initialisation
 *     and setup for scheduler1 when a cell is
 *     configured.
 *
 *  @param[in]  RgSchCellCb   *cell
 *  @param[in]  RgrCellCfg    *cellCfg
 *  @param[out] RgSchErrInfo  *err
 *  @return  S16
 *      -# ROK
 *      -# RFAILED
 **/
#ifdef ANSI
PUBLIC S16 rgSCHCmnRgrCellCfg
(
RgSchCellCb   *cell,
RgrCellCfg    *cellCfg,
RgSchErrInfo  *err
)
#else
PUBLIC S16 rgSCHCmnRgrCellCfg(cell, cellCfg, err)
RgSchCellCb   *cell;
RgrCellCfg    *cellCfg;
RgSchErrInfo  *err;
#endif
{
   S16       ret;
   RgSchCmnCell *cellSch;
   TRC2(rgSCHCmnRgrCellCfg);

   /* As part of RGR cell configuration, validate the CRGCellCfg
    * There is no trigger for crgCellCfg from SC1 */
   /* Removed failure check for Extended CP */

   if (((ret = rgSCHUtlAllocSBuf(cell->instIdx,
      (Data**)&(cell->sc.sch), (sizeof(RgSchCmnCell)))) != ROK))
   {
      RLOG_ARG0(L_ERROR,DBG_CELLID,cell->cellId,  
         "Memory allocation FAILED");
      err->errCause = RGSCHERR_SCH_CFG;
      RETVALUE(ret);
   }
   cellSch = (RgSchCmnCell *)(cell->sc.sch);
   cellSch->cfiCfg = cellCfg->cfiCfg;
   cellSch->trgUlCqi.trgCqi = cellCfg->trgUlCqi.trgCqi;
   /* Initialize the scheduler refresh timer queues */
   cellSch->tmrTqCp.nxtEnt = 0;
   cellSch->tmrTqCp.tmrLen = RG_SCH_CMN_NUM_REFRESH_Q;

   /* RACHO Intialize the RACH ded Preamble Information */
   rgSCHCmnCfgRachDedPrm(cell);
#ifdef LTE_TDD
   /* Initialize 'Np' value for each 'p' used for
    * HARQ ACK/NACK reception */
   rgSCHCmnDlNpValInit(cell);
#endif

   /* Initialize 'Np' value for each 'p' used for
    * HARQ ACK/NACK reception */
#ifdef LTE_TDD
   rgSCHCmnDlNpValInit(cell);
#endif

   /* Now perform uplink related initializations  */
   ret = rgSCHCmnUlCellInit(cell, cellCfg);
   if (ret != ROK)
   {
      /* There is no downlink deinit to be performed */
      err->errCause = RGSCHERR_SCH_CFG;
      RETVALUE(ret);
   }
   ret = rgSCHCmnDlRgrCellCfg(cell, cellCfg, err);
   if (ret != ROK)
   {
      err->errCause = RGSCHERR_SCH_CFG;
      RETVALUE(ret);
   }
   /* DL scheduler has no initializations to make */
   /* As of now DL scheduler always returns ROK   */

   rgSCHCmnGetDciFrmtSizes(cell);
   rgSCHCmnGetCqiDciFrmt2AggrLvl(cell);

   cellSch->apisUl = &rgSchUlSchdTbl[cellCfg->ulSchdType];
   ret = cellSch->apisUl->rgSCHRgrUlCellCfg(cell, cellCfg, err);
   if (ret != ROK)
   {
      RETVALUE(ret);
   }

   cellSch->apisDl = &rgSchDlSchdTbl[cellCfg->dlSchdType];
#ifdef LTEMAC_SPS
   /* Perform SPS specific initialization for the cell */
   ret = rgSCHCmnSpsCellCfg(cell, cellCfg, err);
   if (ret != ROK)
   {
      RETVALUE(ret);
   }
#endif
   ret = cellSch->apisDl->rgSCHRgrDlCellCfg(cell, cellCfg, err);
   if (ret != ROK)
   {
      RETVALUE(ret);
   }
   rgSCHCmnInitVars(cell);

   RETVALUE(ROK);
}  /* rgSCHCmnRgrCellCfg*/


/**
 * @brief This function handles the reconfiguration of cell.
 *
 * @details
 *
 *     Function: rgSCHCmnRgrCellRecfg
 *     Purpose:  Update the reconfiguration parameters.
 *
 *     Invoked by: Scheduler
 *
 *  @param[in]  RgSchCellCb*  cell
 *  @return  Void
 *
 **/
#ifdef ANSI
PUBLIC S16 rgSCHCmnRgrCellRecfg
(
RgSchCellCb             *cell,
RgrCellRecfg            *recfg,
RgSchErrInfo            *err
)
#else
PUBLIC S16 rgSCHCmnRgrCellRecfg(cell, recfg, err)
RgSchCellCb             *cell;
RgrCellRecfg            *recfg;
RgSchErrInfo            *err;
#endif
{
   S16                  ret;
   RgSchCmnCell         *cellSch = RG_SCH_CMN_GET_CELL(cell);
   RgSchCmnUlCell       *cellUl  = RG_SCH_CMN_GET_UL_CELL(cell);

   TRC2(rgSCHCmnRgrCellRecfg);

   if (recfg->recfgTypes & RGR_CELL_UL_CMNRATE_RECFG)
   {
      U8   oldCqi = cellUl->dfltUlCqi;
      if (!RG_SCH_CMN_UL_IS_CQI_VALID(recfg->ulCmnCodeRate.ccchCqi))
      {
         err->errCause = RGSCHERR_SCH_CFG;
         RLOG_ARG0(L_ERROR,DBG_CELLID,cell->cellId, "rgSCHCmnRgrCellRecfg(): "
            "Invalid cqi");
         RETVALUE(RFAILED);
      }
      cellUl->dfltUlCqi = recfg->ulCmnCodeRate.ccchCqi;
      ret = rgSCHCmnPrecompMsg3Vars(cellUl, recfg->ulCmnCodeRate.ccchCqi,
            cell->rachCfg.msgSizeGrpA, cellUl->sbSize, cell->isCpUlExtend);
      if (ret != ROK)
      {
         cellUl->dfltUlCqi = oldCqi;
         rgSCHCmnPrecompMsg3Vars(cellUl, recfg->ulCmnCodeRate.ccchCqi,
               cell->rachCfg.msgSizeGrpA, cellUl->sbSize, cell->isCpUlExtend);
         RETVALUE(ret);
      }
   }

   if (recfg->recfgTypes & RGR_CELL_DL_CMNRATE_RECFG)
   {
      if (rgSCHCmnDlCnsdrCmnRt(cell, &recfg->dlCmnCodeRate) != ROK)
      {
         err->errCause = RGSCHERR_SCH_CFG;
         RETVALUE(RFAILED);
      }
   }
 
   /* Invoke UL sched for cell Recfg */
   ret = cellSch->apisUl->rgSCHRgrUlCellRecfg(cell, recfg, err);
   if (ret != ROK)
   {
      RETVALUE(RFAILED);
   }

   /* Invoke DL sched for cell Recfg */
   ret = cellSch->apisDl->rgSCHRgrDlCellRecfg(cell, recfg, err);
   if (ret != ROK)
   {
      RETVALUE(RFAILED);
   }

   if (recfg->recfgTypes & RGR_CELL_DLFS_RECFG)
   {
      ret = cellSch->apisDlfs->rgSCHDlfsCellRecfg(cell, recfg, err);
      if (ret != ROK)
      {
         RETVALUE(RFAILED);
      }
      cellSch->dl.isDlFreqSel = recfg->dlfsRecfg.isDlFreqSel;
   }

   if (recfg->recfgTypes & RGR_CELL_PWR_RECFG)
   {
      ret = rgSCHPwrCellRecfg(cell, recfg);
      if (ret != ROK)
      {
         RETVALUE(RFAILED);
      }
   }

   RETVALUE(ROK);
}

/***********************************************************
 *
 *     Func : rgSCHCmnUlCellDeinit
 *
 *     Desc : Uplink scheduler de-initialisation for cell.
 *
 *     Ret  : S16
 *
 *     Notes:
 *
 *     File :
 *
 **********************************************************/
#ifdef ANSI
PRIVATE Void rgSCHCmnUlCellDeinit
(
RgSchCellCb *cell
)
#else
PRIVATE Void rgSCHCmnUlCellDeinit(cell)
RgSchCellCb *cell;
#endif
{
   RgSchCmnUlCell   *cellUl = RG_SCH_CMN_GET_UL_CELL(cell);
   U8               ulSfIdx;
#ifdef LTE_TDD
   U8        maxSubfrms = cellUl->numUlSubfrms;
#endif
#ifdef LTE_L2_MEAS
   CmLList       *lnk = NULLP;
   RgSchL2MeasCb *measCb;
#endif
   TRC2(rgSCHCmnUlCellDeinit);
#ifdef LTE_L2_MEAS
#ifdef LTE_TDD
   for(ulSfIdx = 0; ulSfIdx < RGSCH_SF_ALLOC_SIZE; ulSfIdx++)
#else
   for(ulSfIdx = 0; ulSfIdx < RGSCH_NUM_SUB_FRAMES; ulSfIdx++)
#endif
   {
      if(cell->sfAllocArr[ulSfIdx].ulUeInfo.ulAllocInfo != NULLP)
      {
         /* ccpu00117052 - MOD - Passing double pointer
            for proper NULLP assignment*/
         rgSCHUtlFreeSBuf(cell->instIdx,
         (Data **)(&(cell->sfAllocArr[ulSfIdx].ulUeInfo.ulAllocInfo)),
         cellUl->maxAllocPerUlSf * sizeof(RgInfUeUlAlloc));

         /* ccpu00117052 - DEL - removed explicit NULLP assignment
            as it is done in above utility function */
      }
   }
   /* Free the memory allocated to measCb */
   lnk = cell->l2mList.first;
   while(lnk != NULLP)
   {
      measCb = (RgSchL2MeasCb *)lnk->node;
      cmLListDelFrm(&cell->l2mList, lnk);
      lnk = lnk->next;
   /* ccpu00117052 - MOD - Passing double pointer
   for proper NULLP assignment*/
      rgSCHUtlFreeSBuf(cell->instIdx, (Data **)&measCb,\
                          sizeof(RgSchL2MeasCb));
   }
#endif
   if (cellUl->dmrsArr != NULLP)
   {
      /* ccpu00117052 - MOD - Passing double pointer
      for proper NULLP assignment*/
      rgSCHUtlFreeSBuf(cell->instIdx,(Data **)(&(cellUl->dmrsArr)),
               cellUl->dmrsArrSize * sizeof(*cellUl->dmrsArr));
   }
   /* De-init subframes */
#ifdef LTE_TDD
   for (ulSfIdx = 0; ulSfIdx < maxSubfrms; ++ulSfIdx)
#else
   for (ulSfIdx = 0; ulSfIdx < RG_SCH_CMN_UL_NUM_SF; ++ulSfIdx)
#endif
   {
      rgSCHUtlUlSfDeinit(cell, &cellUl->ulSfArr[ulSfIdx]);
   }

#ifdef LTE_TDD
   if (cellUl->ulSfArr != NULLP)
   {
      /* ccpu00117052 - MOD - Passing double pointer
      for proper NULLP assignment*/
      rgSCHUtlFreeSBuf(cell->instIdx,
         (Data **)(&(cellUl->ulSfArr)), maxSubfrms * sizeof(RgSchUlSf));
   }
#endif

   RETVOID;
}

/**
 * @brief Scheduler processing for cell delete.
 *
 * @details
 *
 *     Function : rgSCHCmnCellDel
 *
 *     This functions de-initialises and frees memory
 *     taken up by scheduler1 for the entire cell.
 *
 *  @param[in]  RgSchCellCb  *cell
 *  @return  Void
 **/
#ifdef ANSI
PUBLIC Void rgSCHCmnCellDel
(
RgSchCellCb  *cell
)
#else
PUBLIC Void rgSCHCmnCellDel(cell)
RgSchCellCb  *cell;
#endif
{
   RgSchCmnCell *cellSch = RG_SCH_CMN_GET_CELL(cell);
   TRC2(rgSCHCmnCellDel);

#ifdef LTE_L2_MEAS
   glblTtiCnt = 0;
#endif
   if (cellSch == NULLP)
   {
      RETVOID;
   }
   /* Perform the deinit for the UL scheduler */
   rgSCHCmnUlCellDeinit(cell);
   if (cellSch->apisUl)
   {
      /* api pointer checks added (here and below in
       * this function). pl check. - antriksh */
      cellSch->apisUl->rgSCHFreeUlCell(cell);
   }

   /* Perform the deinit for the DL scheduler */
   cmLListInit(&cellSch->dl.taLst);
   if (cellSch->apisDl)
   {
      cellSch->apisDl->rgSCHFreeDlCell(cell);
   }

   /* DLFS de-initialization */
   if (cellSch->dl.isDlFreqSel && cellSch->apisDlfs)
   {
      cellSch->apisDlfs->rgSCHDlfsCellDel(cell);
   }

   rgSCHPwrCellDel(cell);
#ifdef LTEMAC_SPS
   rgSCHCmnSpsCellDel(cell);
#endif

   /* ccpu00117052 - MOD - Passing double pointer
   for proper NULLP assignment*/
   rgSCHUtlFreeSBuf(cell->instIdx,
      (Data**)(&(cell->sc.sch)), (sizeof(RgSchCmnCell)));
   RETVOID;
}  /* rgSCHCmnCellDel */


/**
 * @brief This function validates QOS parameters for DL.
 *
 * @details
 *
 *     Function: rgSCHCmnValidateDlQos
 *     Purpose:  This function validates QOS parameters for DL.
 *
 *     Invoked by: Scheduler
 *
 *  @param[in] CrgLchQosCfg    *dlQos
 *  @return                    S16
 *
 **/
#ifdef ANSI
PRIVATE S16 rgSCHCmnValidateDlQos
(
RgrLchQosCfg            *dlQos
)
#else
PRIVATE S16 rgSCHCmnValidateDlQos(dlQos)
RgrLchQosCfg            *dlQos;
#endif
{
   U8 qci = dlQos->qci;

   TRC2(rgSCHCmnValidateDlQos);

   if ( qci < RG_SCH_CMN_MIN_QCI || qci > RG_SCH_CMN_MAX_QCI )
   {
      RETVALUE(RFAILED);
   }

   if ((qci >= RG_SCH_CMN_GBR_QCI_START) &&
       (qci <= RG_SCH_CMN_GBR_QCI_END))
   {
      if ((dlQos->mbr == 0) || (dlQos->mbr < dlQos->gbr))
      {
         RETVALUE(RFAILED);
      }
   }
   RETVALUE(ROK);
}

/**
 * @brief Scheduler invocation on logical channel addition.
 *
 * @details
 *
 *     Function : rgSCHCmnRgrLchCfg
 *
 *     This functions does required processing when a new
 *     (dedicated) logical channel is added. Assumes lcg
 *     pointer in ulLc is set.
 *
 *  @param[in]  RgSchCellCb  *cell
 *  @param[in]  RgSchUeCb    *ue
 *  @param[in]  RgSchDlLcCb  *dlLc
 *  @param[int] RgrLchCfg    *lcCfg
 *  @param[out] RgSchErrInfo *err
 *  @return  S16
 *      -# ROK
 *      -# RFAILED
 **/
#ifdef ANSI
PUBLIC S16 rgSCHCmnRgrLchCfg
(
RgSchCellCb  *cell,
RgSchUeCb    *ue,
RgSchDlLcCb  *dlLc,
RgrLchCfg *lcCfg,
RgSchErrInfo *err
)
#else
PUBLIC S16 rgSCHCmnRgrLchCfg(cell, ue, dlLc, lcCfg, err)
RgSchCellCb  *cell;
RgSchUeCb    *ue;
RgSchDlLcCb  *dlLc;
RgrLchCfg *lcCfg;
RgSchErrInfo *err;
#endif
{
   S16 ret;

   RgSchCmnCell *cellSch = RG_SCH_CMN_GET_CELL(cell);

   TRC2(rgSCHCmnRgrLchCfg);

   ret = rgSCHUtlAllocSBuf(cell->instIdx,
      (Data**)&((dlLc)->sch), (sizeof(RgSchCmnDlSvc)));
   if (ret != ROK)
   {
      RLOG_ARG2(L_ERROR,DBG_CELLID,cell->cellId,"rgSCHCmnRgrLchCfg(): "
         "SCH struct alloc failed for CRNTI:%d LCID:%d",ue->ueId,lcCfg->lcId);
      err->errCause = RGSCHERR_SCH_CFG;
      RETVALUE(ret);
   }
   if(lcCfg->lcType != CM_LTE_LCH_DCCH)
   {
      ret = rgSCHCmnValidateDlQos(&lcCfg->dlInfo.dlQos);
      if (ret != ROK)
      {
         RLOG_ARG2(L_ERROR,DBG_CELLID,cell->cellId,"rgSchCmnCrgLcCfg(): "
            "DlQos validation failed for CRNTI:%d LCID:%d",ue->ueId,lcCfg->lcId);
         err->errCause = RGSCHERR_SCH_CFG;
         RETVALUE(ret);
      }
      /* Perform DL service activation in the scheduler */
      ((RgSchCmnDlSvc *)(dlLc->sch))->qci = lcCfg->dlInfo.dlQos.qci;
      ((RgSchCmnDlSvc *)(dlLc->sch))->prio = rgSchCmnDlQciPrio[lcCfg->dlInfo.dlQos.qci - 1];
      ((RgSchCmnDlSvc *)(dlLc->sch))->gbr = (lcCfg->dlInfo.dlQos.gbr * \
      RG_SCH_CMN_REFRESH_TIME)/100;
      ((RgSchCmnDlSvc *)(dlLc->sch))->mbr = (lcCfg->dlInfo.dlQos.mbr * \
      RG_SCH_CMN_REFRESH_TIME)/100;
   }
   else
   {
     /*assigning highest priority to DCCH */
    ((RgSchCmnDlSvc *)(dlLc->sch))->prio=RG_SCH_CMN_DCCH_PRIO; 
   }   
   dlLc->ue = ue;
   dlLc->lcType=lcCfg->lcType;
   ret = cellSch->apisDl->rgSCHRgrDlLcCfg(cell, ue, dlLc, lcCfg, err);
   if (ret != ROK)
   {
      RETVALUE(RFAILED);
   }
   ret = cellSch->apisUl->rgSCHRgrUlLcCfg(cell, ue, lcCfg, err);
   if (ret != ROK)
   {
      RETVALUE(RFAILED);
   }

#ifdef LTEMAC_SPS
   if(lcCfg->dlInfo.dlSpsCfg.isSpsEnabled)
   {
      /* Invoke SPS module if SPS is enabled for the service */
      ret = rgSCHCmnSpsDlLcCfg(cell, ue, dlLc, lcCfg, err);
      if (ret != ROK)
      {
         RLOG_ARG2(L_ERROR,DBG_CELLID,cell->cellId,  "rgSchCmnRgrLchCfg(): "
            "SPS configuration failed for DL LC for CRNTI:%d LCID:%d",ue->ueId,lcCfg->lcId);
         err->errCause = RGSCHERR_SCH_CFG;
         RETVALUE(RFAILED);
      }
   }
#endif

   RETVALUE(ROK);
}

/**
 * @brief Scheduler invocation on logical channel addition.
 *
 * @details
 *
 *     Function : rgSCHCmnRgrLchRecfg
 *
 *     This functions does required processing when an existing
 *     (dedicated) logical channel is reconfigured. Assumes lcg
 *     pointer in ulLc is set to the old value.
 *     Independent of whether new LCG is meant to be configured,
 *     the new LCG scheduler information is accessed and possibly modified.
 *
 *  @param[in]  RgSchCellCb  *cell
 *  @param[in]  RgSchUeCb    *ue
 *  @param[in]  RgSchDlLcCb  *dlLc
 *  @param[int] RgrLchRecfg  *lcRecfg
 *  @param[out] RgSchErrInfo *err
 *  @return  S16
 *      -# ROK
 *      -# RFAILED
 **/
#ifdef ANSI
PUBLIC S16 rgSCHCmnRgrLchRecfg
(
RgSchCellCb  *cell,
RgSchUeCb    *ue,
RgSchDlLcCb  *dlLc,
RgrLchRecfg  *lcRecfg,
RgSchErrInfo *err
)
#else
PUBLIC S16 rgSCHCmnRgrLchRecfg(cell, ue, dlLc, lcRecfg, err)
RgSchCellCb  *cell;
RgSchUeCb    *ue;
RgSchDlLcCb  *dlLc;
RgrLchRecfg  *lcRecfg;
RgSchErrInfo *err;
#endif
{
   S16   ret;
   RgSchCmnCell *cellSch = RG_SCH_CMN_GET_CELL(cell);

   TRC2(rgSCHCmnRgrLchRecfg)

   if(dlLc->lcType != CM_LTE_LCH_DCCH)
   {
      ret = rgSCHCmnValidateDlQos(&lcRecfg->dlRecfg.dlQos);
   
      if (ret != ROK)
      {
         RLOG_ARG2(L_ERROR,DBG_CELLID,cell->cellId,
               "DlQos validation failed for CRNTI:%d LCID:%d",ue->ueId,lcRecfg->lcId);
         err->errCause = RGSCHERR_SCH_CFG;
         RETVALUE(ret);
      }
      if (((RgSchCmnDlSvc *)(dlLc->sch))->qci != lcRecfg->dlRecfg.dlQos.qci)
      {
         RLOG_ARG2(L_ERROR,DBG_CELLID,cell->cellId, "Qci, hence lc Priority change "
            "not supported for CRNTI:%d LCID:%d",ue->ueId,lcRecfg->lcId);
         err->errCause = RGSCHERR_SCH_CFG;
         RETVALUE(ret);
      }
      ((RgSchCmnDlSvc *)(dlLc->sch))->gbr = (lcRecfg->dlRecfg.dlQos.gbr * \
      RG_SCH_CMN_REFRESH_TIME)/100;
      ((RgSchCmnDlSvc *)(dlLc->sch))->mbr = (lcRecfg->dlRecfg.dlQos.mbr * \
      RG_SCH_CMN_REFRESH_TIME)/100;
   }
   else
   {
      /*assigning highest priority to DCCH */
      ((RgSchCmnDlSvc *)(dlLc->sch))->prio = RG_SCH_CMN_DCCH_PRIO; 
   }
   
   ret = cellSch->apisDl->rgSCHRgrDlLcRecfg(cell, ue, dlLc, lcRecfg, err);
   if (ret != ROK)
   {
      RETVALUE(RFAILED);
   }
   ret = cellSch->apisUl->rgSCHRgrUlLcRecfg(cell, ue, lcRecfg, err);
   if (ret != ROK)
   {
      RETVALUE(RFAILED);
   }
    
#ifdef LTEMAC_SPS
   if (lcRecfg->recfgTypes & RGR_DL_LC_SPS_RECFG)
   {
      /* Invoke SPS module if SPS is enabled for the service */
      if(lcRecfg->dlRecfg.dlSpsRecfg.isSpsEnabled)
      {
         ret = rgSCHCmnSpsDlLcRecfg(cell, ue, dlLc, lcRecfg, err);
         if (ret != ROK)
         {
            RLOG_ARG2(L_ERROR,DBG_CELLID,cell->cellId,"SPS re-configuration not "
                  "supported for dlLC Ignore this CRNTI:%d LCID:%d",ue->ueId,lcRecfg->lcId);
         }
      }
      RETVALUE(ROK);
   }
#endif

   RETVALUE(ROK);
}

/**
 * @brief Scheduler invocation on logical channel addition.
 *
 * @details
 *
 *     Function : rgSCHCmnRgrLcgCfg
 *
 *     This functions does required processing when a new
 *     (dedicated) logical channel is added. Assumes lcg
 *     pointer in ulLc is set.
 *
 *  @param[in]  RgSchCellCb  *cell,
 *  @param[in]  RgSchUeCb    *ue,
 *  @param[in]  RgSchLcgCb   *lcg,
 *  @param[in]  RgrLcgCfg    *lcgCfg,
 *  @param[out] RgSchErrInfo *err
 *  @return  S16
 *      -# ROK
 *      -# RFAILED
 **/
#ifdef ANSI
PUBLIC S16 rgSCHCmnRgrLcgCfg
(
RgSchCellCb  *cell,
RgSchUeCb    *ue,
RgSchLcgCb   *lcg,
RgrLcgCfg    *lcgCfg,
RgSchErrInfo *err
)
#else
PUBLIC S16 rgSCHCmnRgrLcgCfg(cell, ue, lcg, lcgCfg, err)
RgSchCellCb  *cell;
RgSchUeCb    *ue;
RgSchLcgCb   *lcg;
RgrLcgCfg    *lcgCfg;
RgSchErrInfo *err;
#endif
{
   S16 ret;
   RgSchCmnCell *cellSch = RG_SCH_CMN_GET_CELL(cell);
   RgSchCmnLcg  *ulLcg = ((RgSchCmnLcg *)(ue->ul.lcgArr[lcgCfg->ulInfo.lcgId].sch));

   TRC2(rgSCHCmnRgrLcgCfg);

   ulLcg->cfgdGbr = (lcgCfg->ulInfo.gbr * RG_SCH_CMN_REFRESH_TIME)/100;
   ulLcg->effGbr  = ulLcg->cfgdGbr;
   ulLcg->deltaMbr = ((lcgCfg->ulInfo.mbr - lcgCfg->ulInfo.gbr) * RG_SCH_CMN_REFRESH_TIME)/100;
   ulLcg->effDeltaMbr = ulLcg->deltaMbr;

   ret = cellSch->apisUl->rgSCHRgrUlLcgCfg(cell, ue, lcg, lcgCfg, err);
   if (ret != ROK)
   {
      RETVALUE(RFAILED);
   }
   if (RGSCH_IS_GBR_BEARER(ulLcg->cfgdGbr))
   {
      /* Indicate MAC that this LCG is GBR LCG */
      rgSCHUtlBuildNSendLcgReg(cell, ue->ueId, lcgCfg->ulInfo.lcgId, TRUE);
   }
   RETVALUE(ROK);
}

/**
 * @brief Scheduler invocation on logical channel addition.
 *
 * @details
 *
 *     Function : rgSCHCmnRgrLcgRecfg
 *
 *     This functions does required processing when a new
 *     (dedicated) logical channel is added. Assumes lcg
 *     pointer in ulLc is set.
 *
 *  @param[in]  RgSchCellCb  *cell,
 *  @param[in]  RgSchUeCb    *ue,
 *  @param[in]  RgSchLcgCb   *lcg,
 *  @param[in]  RgrLcgRecfg  *reCfg,
 *  @param[out] RgSchErrInfo *err
 *  @return  S16
 *      -# ROK
 *      -# RFAILED
 **/
#ifdef ANSI
PUBLIC S16 rgSCHCmnRgrLcgRecfg
(
RgSchCellCb  *cell,
RgSchUeCb    *ue,
RgSchLcgCb   *lcg,
RgrLcgRecfg  *reCfg,
RgSchErrInfo *err
)
#else
PUBLIC S16 rgSCHCmnRgrLcgRecfg(cell, ue, lcg, reCfg, err)
RgSchCellCb  *cell;
RgSchUeCb    *ue;
RgSchLcgCb   *lcg;
RgrLcgRecfg  *reCfg;
RgSchErrInfo *err;
#endif
{
   S16 ret;
   RgSchCmnCell *cellSch = RG_SCH_CMN_GET_CELL(cell);
   RgSchCmnLcg  *ulLcg = ((RgSchCmnLcg *)(ue->ul.lcgArr[reCfg->ulRecfg.lcgId].sch));
   
   TRC2(rgSCHCmnRgrLcgRecfg);

   ulLcg->cfgdGbr = (reCfg->ulRecfg.gbr * RG_SCH_CMN_REFRESH_TIME)/100;
   ulLcg->effGbr  = ulLcg->cfgdGbr;
   ulLcg->deltaMbr = ((reCfg->ulRecfg.mbr - reCfg->ulRecfg.gbr) * RG_SCH_CMN_REFRESH_TIME)/100;
   ulLcg->effDeltaMbr = ulLcg->deltaMbr;
 
   ret = cellSch->apisUl->rgSCHRgrUlLcgRecfg(cell, ue, lcg, reCfg, err);
   if (ret != ROK)
   {
      RETVALUE(RFAILED);
   }
   if (RGSCH_IS_GBR_BEARER(ulLcg->cfgdGbr))
   {
      /* Indicate MAC that this LCG is GBR LCG */
      rgSCHUtlBuildNSendLcgReg(cell, ue->ueId, reCfg->ulRecfg.lcgId, TRUE);
   }
   else
   {
      /* In case of RAB modification */
      rgSCHUtlBuildNSendLcgReg(cell, ue->ueId, reCfg->ulRecfg.lcgId, FALSE);
   }
   RETVALUE(ROK);
}

/***********************************************************
 *
 *     Func : rgSCHCmnRgrLchDel
 *
 *     Desc : Scheduler handling for a (dedicated)
 *             uplink logical channel being deleted.
 *
 *     Ret  :
 *
 *     Notes:
 *
 *     File :
 **********************************************************/
#ifdef ANSI
PUBLIC S16 rgSCHCmnRgrLchDel 
(
RgSchCellCb   *cell,
RgSchUeCb     *ue,
CmLteLcId     lcId,
U8            lcgId
)
#else
PUBLIC S16 rgSCHCmnRgrLchDel(cell, ue, lcId, lcgId)
RgSchCellCb   *cell;
RgSchUeCb     *ue;
CmLteLcId     lcId;
U8            lcgId;
#endif
{
   RgSchCmnCell *cellSch = RG_SCH_CMN_GET_CELL(cell);
   TRC2(rgSCHCmnRgrLchDel);
   cellSch->apisUl->rgSCHRgrUlLchDel(cell, ue, lcId, lcgId);
   RETVALUE(ROK);
}

/***********************************************************
 *
 *     Func : rgSCHCmnLcgDel
 *
 *     Desc : Scheduler handling for a (dedicated)
 *             uplink logical channel being deleted.
 *
 *     Ret  :
 *
 *     Notes:
 *
 *     File :
 *
 **********************************************************/
#ifdef ANSI
PUBLIC Void rgSCHCmnLcgDel
(
RgSchCellCb   *cell,
RgSchUeCb     *ue,
RgSchLcgCb    *lcg
)
#else
PUBLIC Void rgSCHCmnLcgDel(cell, ue, lcg)
RgSchCellCb   *cell;
RgSchUeCb     *ue;
RgSchLcgCb    *lcg;
#endif
{
   RgSchCmnCell *cellSch = RG_SCH_CMN_GET_CELL(cell);
   RgSchCmnLcg  *lcgCmn = RG_SCH_CMN_GET_UL_LCG(lcg);
   TRC2(rgSCHCmnLcgDel);

   if (lcgCmn == NULLP)
   {
      RETVOID;
   }

   if (RGSCH_IS_GBR_BEARER(lcgCmn->cfgdGbr))
   {
      /* Indicate MAC that this LCG is GBR LCG */
      rgSCHUtlBuildNSendLcgReg(cell, ue->ueId, lcg->lcgId, FALSE);
   }

#ifdef LTEMAC_SPS
   if (ue->ul.ulSpsCfg.isUlSpsEnabled == TRUE)
   {
      rgSCHCmnSpsUlLcgDel(cell, ue, lcg);
   }
#endif /* LTEMAC_SPS */

   lcgCmn->effGbr     = 0;
   lcgCmn->reportedBs = 0;
   lcgCmn->cfgdGbr    = 0;
   /* set lcg bs to 0. Deletion of control block happens
    * at the time of UE deletion. */
   lcgCmn->bs = 0;
   cellSch->apisUl->rgSCHFreeUlLcg(cell, ue, lcg);
   RETVOID;
}


/**
 * @brief This function deletes a service from scheduler.
 *
 * @details
 *
 *     Function: rgSCHCmnFreeDlLc
 *     Purpose:  This function is made available through a FP for
 *               making scheduler aware of a service being deleted from UE.
 *
 *     Invoked by: BO and Scheduler
 *
 *  @param[in]  RgSchCellCb*  cell
 *  @param[in]  RgSchUeCb*    ue
 *  @param[in]  RgSchDlLcCb*  svc
 *  @return  Void
 *
 **/
#ifdef ANSI
PUBLIC Void rgSCHCmnFreeDlLc
(
RgSchCellCb                *cell,
RgSchUeCb                  *ue,
RgSchDlLcCb                *svc
)
#else
PUBLIC Void rgSCHCmnFreeDlLc(cell, ue, svc)
RgSchCellCb                *cell;
RgSchUeCb                  *ue;
RgSchDlLcCb                *svc;
#endif
{
   RgSchCmnCell *cellSch = RG_SCH_CMN_GET_CELL(cell);
   TRC2(rgSCHCmnFreeDlLc);
   if (svc->sch == NULLP)
   {
      RETVOID;
   }
   cellSch->apisDl->rgSCHFreeDlLc(cell, ue, svc);
#ifdef LTEMAC_SPS
   /* If SPS service, invoke SPS module */
   if (svc->dlLcSpsCfg.isSpsEnabled)
   {
      rgSCHCmnSpsDlLcDel(cell, ue, svc);
   }
#endif

   /* ccpu00117052 - MOD - Passing double pointer
   for proper NULLP assignment*/
   rgSCHUtlFreeSBuf(cell->instIdx,
         (Data**)(&(svc->sch)), (sizeof(RgSchCmnDlSvc)));
   RETVOID;
}

#ifdef RGR_V1

/**
 * @brief This function Processes the Final Allocations
 *        made by the RB Allocator against the requested
 *        CCCH SDURetx Allocations.
 *
 * @details
 *
 *     Function: rgSCHCmnDlCcchSduRetxFnlz
 *     Purpose:  This function Processes the Final Allocations
 *               made by the RB Allocator against the requested
 *               CCCH Retx Allocations.
 *               Scans through the scheduled list of ccchSdu retrans
 *               fills the corresponding pdcch, adds the hqProc to
 *               the corresponding SubFrm and removes the hqP from
 *               cells retx List.
 *
 *     Invoked by: Common Scheduler
 *
 *  @param[in]  RgSchCellCb           *cell
 *  @param[in]  RgSchCmnDlRbAllocInfo *allocInfo
 *  @return  Void
 *
 **/
#ifdef ANSI
PRIVATE Void rgSCHCmnDlCcchSduRetxFnlz
(
RgSchCellCb           *cell,
RgSchCmnDlRbAllocInfo *allocInfo
)
#else
PRIVATE Void rgSCHCmnDlCcchSduRetxFnlz(cell, allocInfo)
RgSchCellCb           *cell;
RgSchCmnDlRbAllocInfo *allocInfo;
#endif
{
   CmLList           *node;
   RgSchCmnDlCell    *cmnCellDl = RG_SCH_CMN_GET_DL_CELL(cell);
   RgSchDlRbAlloc    *rbAllocInfo;
   RgSchDlHqProcCb   *hqP;
   RgSchUeCb         *ue;
   TRC2(rgSCHCmnDlCcchSduRetxFnlz);

   /* Traverse through the Scheduled Retx List */
   node = allocInfo->ccchSduAlloc.schdCcchSduRetxLst.first;
   while (node)
   {
      hqP = (RgSchDlHqProcCb *)(node->node);
      ue = hqP->hqE->ue;
      rbAllocInfo = &ue->dl.dlAllocCb;
      node = node->next;
      rgSCHCmnFillHqPPdcch(cell, rbAllocInfo, hqP);

      /* Remove the HqP from cell's ccchSduRetxLst */
      cmLListDelFrm(&cmnCellDl->ccchSduRetxLst, &hqP->tbInfo[0].ccchSchdInfo.retxLnk);
      hqP->tbInfo[0].ccchSchdInfo.retxLnk.node = (PTR)NULLP;

      /* Fix: syed dlAllocCb reset should be performed.
       * zombie info in dlAllocCb leading to crash rbNum wraparound */ 
      rgSCHCmnDlUeResetTemp(ue, hqP);
   }
   /* Fix: syed dlAllocCb reset should be performed.
    * zombie info in dlAllocCb leading to crash rbNum wraparound */ 
   node = allocInfo->ccchSduAlloc.nonSchdCcchSduRetxLst.first;
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
#endif
/**
 * @brief This function Processes the Final Allocations
 *        made by the RB Allocator against the requested
 *        CCCH Retx Allocations.
 *
 * @details
 *
 *     Function: rgSCHCmnDlCcchRetxFnlz
 *     Purpose:  This function Processes the Final Allocations
 *               made by the RB Allocator against the requested
 *               CCCH Retx Allocations.
 *               Scans through the scheduled list of msg4 retrans
 *               fills the corresponding pdcch, adds the hqProc to
 *               the corresponding SubFrm and removes the hqP from
 *               cells retx List.
 *
 *     Invoked by: Common Scheduler
 *
 *  @param[in]  RgSchCellCb           *cell
 *  @param[in]  RgSchCmnDlRbAllocInfo *allocInfo
 *  @return  Void
 *
 **/
#ifdef ANSI
PRIVATE Void rgSCHCmnDlCcchRetxFnlz
(
RgSchCellCb           *cell,
RgSchCmnDlRbAllocInfo *allocInfo
)
#else
PRIVATE Void rgSCHCmnDlCcchRetxFnlz(cell, allocInfo)
RgSchCellCb           *cell;
RgSchCmnDlRbAllocInfo *allocInfo;
#endif
{
   CmLList           *node;
   RgSchCmnDlCell    *cmnCellDl = RG_SCH_CMN_GET_DL_CELL(cell);
   RgSchDlRbAlloc    *rbAllocInfo;
   RgSchDlHqProcCb   *hqP;
   RgSchRaCb         *raCb;
   TRC2(rgSCHCmnDlCcchRetxFnlz);

   /* Traverse through the Scheduled Retx List */
   node = allocInfo->msg4Alloc.schdMsg4RetxLst.first;
   while (node)
   {
      hqP = (RgSchDlHqProcCb *)(node->node);
      raCb = hqP->hqE->raCb;
      rbAllocInfo = &raCb->rbAllocInfo;
      node = node->next;
      rgSCHCmnFillHqPPdcch(cell, rbAllocInfo, hqP);

      /* Remove the HqP from cell's msg4RetxLst */
      cmLListDelFrm(&cmnCellDl->msg4RetxLst, &hqP->tbInfo[0].ccchSchdInfo.retxLnk);
      hqP->tbInfo[0].ccchSchdInfo.retxLnk.node = (PTR)NULLP;
      /* Fix: syed dlAllocCb reset should be performed.
       * zombie info in dlAllocCb leading to crash rbNum wraparound */ 
      cmMemset((U8 *)rbAllocInfo, (U8)0, sizeof(*rbAllocInfo));
      rgSCHCmnDlHqPResetTemp(hqP);
   }
   /* Fix: syed dlAllocCb reset should be performed.
    * zombie info in dlAllocCb leading to crash rbNum wraparound */ 
   node = allocInfo->msg4Alloc.nonSchdMsg4RetxLst.first;
   while(node)
   {
      hqP = (RgSchDlHqProcCb *)(node->node);
      raCb = hqP->hqE->raCb;
      node = node->next;
      cmMemset((U8 *)&raCb->rbAllocInfo, (U8)0, sizeof(raCb->rbAllocInfo));
      rgSCHCmnDlHqPResetTemp(hqP);
   }
   RETVOID;
}

#ifdef RGR_V1
/**
 * @brief This function Processes the Final Allocations
 *        made by the RB Allocator against the requested
 *        CCCH SDU tx Allocations.
 *
 * @details
 *
 *     Function: rgSCHCmnDlCcchSduTxFnlz
 *     Purpose:  This function Processes the Final Allocations
 *               made by the RB Allocator against the requested
 *               CCCH tx Allocations.
 *               Scans through the scheduled list of CCCH SDU trans
 *               fills the corresponding pdcch, adds the hqProc to
 *               the corresponding SubFrm and removes the hqP from
 *               cells tx List.
 *
 *     Invoked by: Common Scheduler
 *
 *  @param[in]  RgSchCellCb           *cell
 *  @param[in]  RgSchCmnDlRbAllocInfo *allocInfo
 *  @return  Void
 *
 **/
#ifdef ANSI
PRIVATE Void rgSCHCmnDlCcchSduTxFnlz
(
RgSchCellCb           *cell,
RgSchCmnDlRbAllocInfo *allocInfo
)
#else
PRIVATE Void rgSCHCmnDlCcchSduTxFnlz(cell, allocInfo)
RgSchCellCb           *cell;
RgSchCmnDlRbAllocInfo *allocInfo;
#endif
{
   CmLList           *node;
   RgSchUeCb         *ueCb;
   RgSchDlRbAlloc    *rbAllocInfo;
   RgSchDlHqProcCb   *hqP;
   RgSchLchAllocInfo  lchSchdData;
   TRC2(rgSCHCmnDlCcchSduTxFnlz);

   /* Traverse through the Scheduled Retx List */
   node = allocInfo->ccchSduAlloc.schdCcchSduTxLst.first;
   while (node)
   {
      hqP = (RgSchDlHqProcCb *)(node->node);
      ueCb = hqP->hqE->ue;
      node = node->next;
      rbAllocInfo = &ueCb->dl.dlAllocCb;

      /* fill the pdcch and HqProc */
      rgSCHCmnFillHqPPdcch(cell, rbAllocInfo, hqP);

      /* Remove the raCb from cell's toBeSchdLst */
      cmLListDelFrm(&cell->ccchSduUeLst, &ueCb->ccchSduLnk);
      ueCb->ccchSduLnk.node = (PTR)NULLP;

      /* Fix : Resetting this required to avoid complication
       * in reestablishment case */
      ueCb->dlCcchInfo.bo = 0;

      /* Indicate DHM of the CCCH LC scheduling */
      hqP->tbInfo[0].contResCe = NOTPRSNT;
      lchSchdData.lcId     = 0;
      lchSchdData.schdData = hqP->tbInfo[0].ccchSchdInfo.totBytes -
                             (RGSCH_MSG4_HDRSIZE);
      rgSCHDhmAddLcData(cell->instIdx, &lchSchdData, &hqP->tbInfo[0]);

      /* Fix: syed dlAllocCb reset should be performed.
       * zombie info in dlAllocCb leading to crash rbNum wraparound */ 
      rgSCHCmnDlUeResetTemp(ueCb, hqP);
   }
   /* Fix: syed dlAllocCb reset should be performed.
    * zombie info in dlAllocCb leading to crash rbNum wraparound */ 
   node = allocInfo->ccchSduAlloc.nonSchdCcchSduTxLst.first;
   while(node)
   {
      hqP = (RgSchDlHqProcCb *)(node->node);
      ueCb = hqP->hqE->ue;
      node = node->next;
      /* Release HqProc */
      rgSCHDhmRlsHqpTb(hqP, 0, FALSE);
      /*Fix: Removing releasing of TB1 as it will not exist for CCCH SDU and hence caused a crash*/
      /*rgSCHDhmRlsHqpTb(hqP, 1, FALSE);*/
      /* reset the UE allocation Information */
      rgSCHCmnDlUeResetTemp(ueCb, hqP);
   }
   RETVOID;
}

#endif
/**
 * @brief This function Processes the Final Allocations
 *        made by the RB Allocator against the requested
 *        CCCH tx Allocations.
 *
 * @details
 *
 *     Function: rgSCHCmnDlCcchTxFnlz
 *     Purpose:  This function Processes the Final Allocations
 *               made by the RB Allocator against the requested
 *               CCCH tx Allocations.
 *               Scans through the scheduled list of msg4 trans
 *               fills the corresponding pdcch, adds the hqProc to
 *               the corresponding SubFrm and removes the hqP from
 *               cells tx List.
 *
 *     Invoked by: Common Scheduler
 *
 *  @param[in]  RgSchCellCb           *cell
 *  @param[in]  RgSchCmnDlRbAllocInfo *allocInfo
 *  @return  Void
 *
 **/
#ifdef ANSI
PRIVATE Void rgSCHCmnDlCcchTxFnlz
(
RgSchCellCb           *cell,
RgSchCmnDlRbAllocInfo *allocInfo
)
#else
PRIVATE Void rgSCHCmnDlCcchTxFnlz(cell, allocInfo)
RgSchCellCb           *cell;
RgSchCmnDlRbAllocInfo *allocInfo;
#endif
{
   CmLList           *node;
   RgSchRaCb         *raCb;
   RgSchDlRbAlloc    *rbAllocInfo;
   RgSchDlHqProcCb   *hqP;
   RgSchLchAllocInfo  lchSchdData;
   TRC2(rgSCHCmnDlCcchTxFnlz);

   /* Traverse through the Scheduled Retx List */
   node = allocInfo->msg4Alloc.schdMsg4TxLst.first;
   while (node)
   {
      hqP = (RgSchDlHqProcCb *)(node->node);
      raCb = hqP->hqE->raCb;
      node = node->next;
      rbAllocInfo = &raCb->rbAllocInfo;

      /* fill the pdcch and HqProc */
      rgSCHCmnFillHqPPdcch(cell, rbAllocInfo, hqP);
      /* MSG4 Fix Start */
     
      rgSCHRamRmvFrmRaInfoSchdLst(cell, raCb);
      /* MSG4 Fix End */     

      /* Indicate DHM of the CCCH LC scheduling */
      lchSchdData.lcId     = 0;
      lchSchdData.schdData = hqP->tbInfo[0].ccchSchdInfo.totBytes -
         (RGSCH_MSG4_HDRSIZE + RGSCH_CONT_RESID_SIZE);
      /* TRansmitting presence of cont Res CE across MAC-SCH interface to
       * identify CCCH SDU transmissions which need to be done
       * without the
       * contention resolution CE*/
      hqP->tbInfo[0].contResCe = PRSNT_NODEF;
      /*Dont add lc if only cont res CE is being transmitted*/
      if(raCb->dlCcchInfo.bo)
      {
         rgSCHDhmAddLcData(cell->instIdx, &lchSchdData, &hqP->tbInfo[0]);
      }
      else
      {
      }
      /* Fix: syed dlAllocCb reset should be performed.
       * zombie info in dlAllocCb leading to crash rbNum wraparound */ 
      cmMemset((U8 *)&raCb->rbAllocInfo, (U8)0, sizeof(raCb->rbAllocInfo));
      rgSCHCmnDlHqPResetTemp(hqP);
   }
   node = allocInfo->msg4Alloc.nonSchdMsg4TxLst.first;
   while(node)
   {
      hqP = (RgSchDlHqProcCb *)(node->node);
      raCb = hqP->hqE->raCb;
      node = node->next;
      rbAllocInfo = &raCb->rbAllocInfo;
      /* Release HqProc */
      rgSCHDhmRlsHqpTb(hqP, 0, FALSE);
      /*Fix: Removing releasing of TB1 as it will not exist for MSG4 and hence caused a crash*/
      /*      rgSCHDhmRlsHqpTb(hqP, 1, FALSE);*/
      /* reset the UE allocation Information */
      cmMemset((U8 *)rbAllocInfo, (U8)0, sizeof(*rbAllocInfo));
      rgSCHCmnDlHqPResetTemp(hqP);
   }

   RETVOID;
}
/* R8 Upgrade */
/**
 * @brief This function calculates the BI Index to be sent in the Bi header
 * field.
 *
 * @details
 *     Function: rgSCHCmnGetBiIndex
 *     Purpose:  This function Processes utilizes the previous BI time value
 *     calculated and the difference last BI sent time and current time. To
 *     calculate the latest BI Index. It also considers the how many UE's
 *     Unserved in this subframe.
 *
 *     Invoked by: Common Scheduler
 *
 *  @param[in]  RgSchCellCb           *cell
 *  @param[in]  U32                   ueCount
 *  @return  U8
 *
 **/
#ifdef ANSI
PRIVATE U8 rgSCHCmnGetBiIndex
(
RgSchCellCb   *cell,
U32           ueCount
)
#else
PRIVATE U8 rgSCHCmnGetBiIndex(cell, ueCount)
RgSchCellCb   *cell;
U32           ueCount;
#endif
{
   S16  prevVal = 0;      /* To Store Intermediate Value */
   U16  newBiVal = 0;     /* To store Bi Value in millisecond */
   U8   idx = 0;
   U16  timeDiff = 0;

   TRC2(rgSCHCmnGetBiIndex)

   if (cell->biInfo.prevBiTime != 0)
   {
      timeDiff =(RGSCH_CALC_SF_DIFF(cell->crntTime, cell->biInfo.biTime));
      prevVal = cell->biInfo.prevBiTime - timeDiff;
   }
   if (prevVal < 0)
   {
      prevVal = 0;
   }
   newBiVal = RG_SCH_CMN_GET_BI_VAL(prevVal,ueCount);
   /* To be used next time when BI is calculated */
   RGSCHCPYTIMEINFO(cell->crntTime, cell->biInfo.biTime)
  /* Search the actual BI Index from table Backoff Parameters Value  and
   * return that Index */
   do
   {
      if (rgSchCmnBiTbl[idx] > newBiVal)
      {
         break;
      }
      idx++;
   }while(idx < RG_SCH_CMN_NUM_BI_VAL-1);
   cell->biInfo.prevBiTime = rgSchCmnBiTbl[idx];
   /* For 16 Entries in Table 7.2.1 36.321.880 - 3 reserved so total 13 Entries */
   RETVALUE(idx); /* Returning reserved value from table UE treats it has 960 ms */
} /* rgSCHCmnGetBiIndex */

/**
 * @brief This function Processes the Final Allocations
 *        made by the RB Allocator against the requested
 *        RAR allocations. Assumption: The reuqested
 *        allocations are always satisfied completely.
 *        Hence no roll back.
 *
 * @details
 *
 *     Function: rgSCHCmnDlRaRspFnlz
 *     Purpose:  This function Processes the Final Allocations
 *               made by the RB Allocator against the requested.
 *               Takes care of PDCCH filling.
 *
 *     Invoked by: Common Scheduler
 *
 *  @param[in]  RgSchCellCb           *cell
 *  @param[in]  RgSchCmnDlRbAllocInfo *allocInfo
 *  @return  Void
 *
 **/
#ifdef ANSI
PRIVATE Void rgSCHCmnDlRaRspFnlz
(
RgSchCellCb           *cell,
RgSchCmnDlRbAllocInfo *allocInfo
)
#else
PRIVATE Void rgSCHCmnDlRaRspFnlz(cell, allocInfo)
RgSchCellCb           *cell;
RgSchCmnDlRbAllocInfo *allocInfo;
#endif
{
   U32            rarCnt = 0;
   RgSchDlRbAlloc *raRspAlloc;
   RgSchDlSf      *subFrm = NULLP;
   RgSchRaCb      *raCb;
   RgSchErrInfo   err;
   CmLListCp      *reqLst;
   RgSchRaReqInfo *raReq;
   Bool           preamGrpA;
   RgSchUlAlloc   *ulAllocRef=NULLP;
   RgSchCmnDlCell *cellDl = RG_SCH_CMN_GET_DL_CELL(cell);
   U8              allocRapidCnt = 0;
#ifdef LTE_TDD
   U32            msg3SchdIdx = 0;
   U8             ulDlCfgIdx = cell->ulDlCfgIdx;
   U8             msg3Subfrm;
#endif

   TRC2(rgSCHCmnDlRaRspFnlz);

   for (rarCnt=0; rarCnt<RG_SCH_CMN_MAX_CMN_PDCCH; rarCnt++)
   {
      raRspAlloc = &allocInfo->raRspAlloc[rarCnt];
      /* Having likely condition first for optimization */
      if (!raRspAlloc->pdcch)
      {
         continue;
      }
      else
      {
         subFrm = raRspAlloc->dlSf;
         reqLst = &cell->raInfo.raReqLst[raRspAlloc->raIndex];
         /* Corrected RACH handling for multiple RAPIDs per RARNTI */
         allocRapidCnt = raRspAlloc->numRapids;
         while (allocRapidCnt)
         {
            raReq = (RgSchRaReqInfo *)(reqLst->first->node);
            /* RACHO: If dedicated preamble, then allocate UL Grant
             * (consequence of handover/pdcchOrder) and continue */
            if (RGSCH_IS_DEDPRM(cell, raReq->raReq.rapId))
            {
               rgSCHCmnHdlHoPo(cell, &subFrm->raRsp[rarCnt].contFreeUeLst,
                     raReq);
               cmLListDelFrm(reqLst, reqLst->first);
               allocRapidCnt--;
               /* ccpu00117052 - MOD - Passing double pointer
               for proper NULLP assignment*/
               rgSCHUtlFreeSBuf(cell->instIdx, (Data **)&raReq,
                     sizeof(RgSchRaReqInfo));
               continue;
            }
            /* ccpu00139815 */
            if(cell->overLoadBackOffEnab)
            {/* rach Overlaod conrol is triggerd, Skipping this rach */
               cmLListDelFrm(reqLst, reqLst->first);
               allocRapidCnt--;
               rgSCHUtlFreeSBuf(cell->instIdx, (Data **)&raReq,
                     sizeof(RgSchRaReqInfo));
               continue;
            }
            /* Attempt to include each RA request into the RSP */
            /* Any failure in the procedure is considered to   */
            /* affect futher allocations in the same TTI. When */
            /* a failure happens, we break out and complete    */
            /* the processing for random access                */
            if (rgSCHRamCreateRaCb(cell, &raCb, &err) != ROK)
            {
               break;
            }
            /* Msg3 allocation request to USM */
            if (raReq->raReq.rapId < cell->rachCfg.sizeRaPreambleGrpA)
               preamGrpA = TRUE;
            else
               preamGrpA = FALSE;
            /*ccpu00128820 - MOD - Msg3 alloc double delete issue*/
            rgSCHCmnMsg3GrntReq(cell, raCb->tmpCrnti, preamGrpA, \
                  &(raCb->msg3HqProc), &ulAllocRef, &raCb->msg3HqProcId);
            if (ulAllocRef == NULLP)
            {
               rgSCHRamDelRaCb(cell, raCb, TRUE);
               break;
            }
            if (raReq->raReq.cqiPres)
            {
               raCb->ccchCqi = raReq->raReq.cqiIdx;
            }
            else
            {
               raCb->ccchCqi = cellDl->ccchCqi;
            }
            raCb->rapId = raReq->raReq.rapId;
            raCb->ta.pres    = TRUE;
            raCb->ta.val = raReq->raReq.ta;
            raCb->msg3Grnt = ulAllocRef->grnt;
            /* Populating the tpc value received */
            raCb->msg3Grnt.tpc = raReq->raReq.tpc;
            /* PHR handling for MSG3 */
            ulAllocRef->raCb = raCb;
#ifndef LTE_TDD
            /* To the crntTime, add the MIN time at which UE will
             * actually send MSG3 i.e DL_DELTA+6 */
            raCb->msg3AllocTime = cell->crntTime;
            RGSCH_INCR_SUB_FRAME(raCb->msg3AllocTime, RG_SCH_CMN_MIN_MSG3_RECP_INTRVL);
#else
            msg3SchdIdx = (cell->crntTime.subframe+RG_SCH_CMN_DL_DELTA) % 
                                 RGSCH_NUM_SUB_FRAMES;
            /*[ccpu00134666]-MOD-Modify the check to schedule the RAR in
              special subframe */                       
            if(rgSchTddUlDlSubfrmTbl[ulDlCfgIdx][msg3SchdIdx] != 
                        RG_SCH_TDD_UL_SUBFRAME)
            {
               RGSCHCMNADDTOCRNTTIME(cell->crntTime,raCb->msg3AllocTime,
                                       RG_SCH_CMN_DL_DELTA)
               msg3Subfrm = rgSchTddMsg3SubfrmTbl[ulDlCfgIdx][
                                       raCb->msg3AllocTime.subframe];
               RGSCHCMNADDTOCRNTTIME(raCb->msg3AllocTime, raCb->msg3AllocTime, 
                                 msg3Subfrm);
            }
#endif
            cmLListAdd2Tail(&subFrm->raRsp[rarCnt].raRspLst, &raCb->rspLnk);
            raCb->rspLnk.node = (PTR)raCb;
            cmLListDelFrm(reqLst, reqLst->first);
            allocRapidCnt--;
            /* ccpu00117052 - MOD - Passing double pointer
            for proper NULLP assignment*/
            rgSCHUtlFreeSBuf(cell->instIdx, (Data **)&raReq,
                  sizeof(RgSchRaReqInfo));

            /* SR_RACH_STATS : RAR scheduled */
            rgNumRarSched++;

         }
         /* R8 Upgrade */
         /* Fill subframe data members */
         subFrm->raRsp[rarCnt].raRnti = raRspAlloc->rnti;
         subFrm->raRsp[rarCnt].pdcch  = raRspAlloc->pdcch;
         subFrm->raRsp[rarCnt].tbSz   = raRspAlloc->tbInfo[0].bytesAlloc;
         /* Fill PDCCH data members */
         rgSCHCmnFillPdcch(cell, subFrm->raRsp[rarCnt].pdcch, raRspAlloc);

         /* ccpu00139815 */
         if(cell->overLoadBackOffEnab)
         {/* rach Overlaod conrol is triggerd, Skipping this rach */
            subFrm->raRsp[rarCnt].backOffInd.pres = PRSNT_NODEF;
            subFrm->raRsp[rarCnt].backOffInd.val  = cell->overLoadBackOffval;
            continue;
         }
         else
         {
            subFrm->raRsp[rarCnt].backOffInd.pres = NOTPRSNT;
         }

         /*[ccpu00125212] Avoiding sending of empty RAR in case of RAR window
           is short and UE is sending unauthorised preamble.*/
         reqLst = &cell->raInfo.raReqLst[raRspAlloc->raIndex];
         if ((raRspAlloc->biEstmt) && (reqLst->count))
         {
            subFrm->raRsp[0].backOffInd.pres = PRSNT_NODEF;
            /* Added as part of Upgrade */
            subFrm->raRsp[0].backOffInd.val =
            rgSCHCmnGetBiIndex(cell, reqLst->count);

            /* SR_RACH_STATS : Back Off Inds */
            rgNumBI++;

         }
         else if ((subFrm->raRsp[rarCnt].raRspLst.first == NULLP) &&
               (subFrm->raRsp[rarCnt].contFreeUeLst.first == NULLP))
         {
            /* Return the grabbed PDCCH */
            rgSCHUtlPdcchPut(cell, &subFrm->pdcchInfo, raRspAlloc->pdcch);
            subFrm->raRsp[rarCnt].pdcch = NULLP;
            RLOG_ARG0(L_ERROR,DBG_CELLID,cell->cellId,"rgSCHCmnRaRspAlloc(): "
                  "Not even one RaReq.");
            RETVOID;
         }
      }
      RLOG_ARG3(L_DEBUG,DBG_CELLID,cell->cellId, 
            "RNTI:%d Scheduled RAR @ (%u,%u) ",
            raRspAlloc->rnti, 
            cell->crntTime.sfn,
            cell->crntTime.subframe);
   }
   RETVOID;
}

/**
 * @brief This function computes rv.
 *
 * @details
 *
 *     Function: rgSCHCmnDlCalcRvForBcch
 *     Purpose:  This function computes rv.
 *
 *     Invoked by: Common Scheduler
 *
 *  @param[in]   RgSchCellCb     *cell
 *  @param[in]   Bool            si
 *  @param[in]   U16             i
 *  @return  U8
 *
 **/
#ifdef ANSI
PRIVATE U8 rgSCHCmnDlCalcRvForBcch
(
RgSchCellCb          *cell,
Bool                 si,
U16                  i
)
#else
PRIVATE U8 rgSCHCmnDlCalcRvForBcch(cell, si, i)
RgSchCellCb          *cell;
Bool                 si;
U16                  i;
#endif
{
   U8 k, rv;
   CmLteTimingInfo   frm;
   TRC2(rgSCHCmnDlCalcRvForBcch);

   frm   = cell->crntTime;
   RGSCH_INCR_SUB_FRAME(frm, RG_SCH_CMN_DL_DELTA);

   if(si)
   {
      k = i % 4;
   }
   else
   {
      k = (frm.sfn/2) % 4;
   }
   rv = RGSCH_CEIL(3*k, 2) % 4;
   RETVALUE(rv);
}

/**
 * @brief This function Processes the Final Allocations
 *        made by the RB Allocator against the requested
 *        BCCH/PCCH allocations. Assumption: The reuqested
 *        allocations are always satisfied completely.
 *        Hence no roll back.
 *
 * @details
 *
 *     Function: rgSCHCmnDlBcchPcchFnlz
 *     Purpose:  This function Processes the Final Allocations
 *               made by the RB Allocator against the requested.
 *               Takes care of PDCCH filling.
 *
 *     Invoked by: Common Scheduler
 *
 *  @param[in]  RgSchCellCb           *cell
 *  @param[in]  RgSchCmnDlRbAllocInfo *allocInfo
 *  @return  Void
 *
 **/
#ifdef ANSI
PRIVATE Void rgSCHCmnDlBcchPcchFnlz
(
RgSchCellCb           *cell,
RgSchCmnDlRbAllocInfo *allocInfo
)
#else
PRIVATE Void rgSCHCmnDlBcchPcchFnlz(cell, allocInfo)
RgSchCellCb           *cell;
RgSchCmnDlRbAllocInfo *allocInfo;
#endif
{
   RgSchDlRbAlloc *rbAllocInfo;
   RgSchDlSf      *subFrm;

#ifdef LTE_TDD
   U8             nextSfIdx = (cell->crntSfIdx) % RGSCH_SF_ALLOC_SIZE;
#else
#ifdef LTEMAC_HDFDD
   U8             nextSfIdx = (cell->crntSfIdx + RG_SCH_CMN_HARQ_INTERVAL) % RGSCH_NUM_SUB_FRAMES;
#else
   U8             nextSfIdx = (cell->crntSfIdx) % RGSCH_NUM_SUB_FRAMES;
#endif
#endif

   /*  Moving variables to available scope for optimization */
   RgSchClcDlLcCb *pcch;
   RgSchClcBoRpt  *bo;
#ifndef RGR_SI_SCH
   RgSchClcDlLcCb  *bcch;
   Bool           sendInd=TRUE;
#endif
   RgSchCmnDlCell       *cellDl = RG_SCH_CMN_GET_DL_CELL(cell);

   TRC2(rgSCHCmnDlBcchPcchFnlz);

   /* handle PCCH */
   rbAllocInfo = &allocInfo->pcchAlloc;
   if (rbAllocInfo->pdcch)
   {
      RgInfSfAlloc   *subfrmAlloc = &(cell->sfAllocArr[nextSfIdx]);

      /* Added sfIdx calculation for TDD as well */
#ifndef LTE_TDD
#ifdef LTEMAC_HDFDD
      nextSfIdx = (cell->crntSfIdx + RG_SCH_CMN_HARQ_INTERVAL) % RGSCH_NUM_SUB_FRAMES;
#else
      nextSfIdx = (cell->crntSfIdx) % RGSCH_NUM_SUB_FRAMES;
#endif
#endif
      subFrm = rbAllocInfo->dlSf;
      pcch = rgSCHDbmGetPcch(cell);
      if(pcch == NULLP)
      {
         RLOG_ARG0(L_ERROR,DBG_CELLID,cell->cellId,"rgSCHCmnDlBcchPcchFnlz( ): "
               "No Pcch Present");
         RETVOID;
      }

      /* Added Dl TB count for paging message transmission*/
#ifdef LTE_L2_MEAS
      cell->dlUlTbCnt.tbTransDlTotalCnt++;
#endif      
      bo = (RgSchClcBoRpt *)pcch->boLst.first->node;
      cmLListDelFrm(&pcch->boLst, &bo->boLstEnt);
      /* ccpu00117052 - MOD - Passing double pointer
         for proper NULLP assignment*/
      rgSCHUtlFreeSBuf(cell->instIdx, (Data **)&bo, sizeof(RgSchClcBoRpt));
      /* Fill subframe data members */
      subFrm->pcch.tbSize = rbAllocInfo->tbInfo[0].bytesAlloc;
      subFrm->pcch.pdcch  = rbAllocInfo->pdcch;
      /* Fill PDCCH data members */
      rgSCHCmnFillPdcch(cell, subFrm->pcch.pdcch, rbAllocInfo);
      rgSCHUtlFillRgInfCmnLcInfo(subFrm, subfrmAlloc, pcch->lcId, TRUE);
      /* ccpu00132314-ADD-Update the tx power allocation info  
         TODO-Need to add a check for max tx power per symbol */ 
      subfrmAlloc->cmnLcInfo.pcchInfo.txPwrOffset = cellDl->pcchTxPwrOffset;   
   }

   /* handle BCCH */
   rbAllocInfo = &allocInfo->bcchAlloc;
   if (rbAllocInfo->pdcch)
   {
      RgInfSfAlloc   *subfrmAlloc = &(cell->sfAllocArr[nextSfIdx]);
#ifndef LTE_TDD
#ifdef LTEMAC_HDFDD
      nextSfIdx = (cell->crntSfIdx + RG_SCH_CMN_HARQ_INTERVAL) % RGSCH_NUM_SUB_FRAMES;
#else
      nextSfIdx = (cell->crntSfIdx) % RGSCH_NUM_SUB_FRAMES;
#endif
#endif
      subFrm = rbAllocInfo->dlSf;

      /* Fill subframe data members */
      subFrm->bcch.tbSize = rbAllocInfo->tbInfo[0].bytesAlloc;
      subFrm->bcch.pdcch  = rbAllocInfo->pdcch;
      /* Fill PDCCH data members */
      rgSCHCmnFillPdcch(cell, subFrm->bcch.pdcch, rbAllocInfo);

      if(rbAllocInfo->schdFirst)
      {
#ifndef RGR_SI_SCH
         bcch = rgSCHDbmGetFirstBcchOnDlsch(cell);
         bo = (RgSchClcBoRpt *)bcch->boLst.first->node;
#else
         /*Copy the SIB1 msg buff into interface buffer */
         SCpyMsgMsg(cell->siCb.crntSiInfo.sib1Info.sib1,
               rgSchCb[cell->instIdx].rgSchInit.region,
               rgSchCb[cell->instIdx].rgSchInit.pool,
               &subfrmAlloc->cmnLcInfo.bcchInfo.pdu);
#endif/*RGR_SI_SCH*/
         subFrm->bcch.pdcch->dci.u.format1aInfo.t.pdschInfo.allocInfo.rv =
            rgSCHCmnDlCalcRvForBcch(cell, FALSE, 0);
      }
      else
      {
         U16   i;
#ifdef RGR_SI_SCH
         Buffer    *pdu;

         i = cell->siCb.siCtx.i;
         /*Decrement the retransmission count */
         cell->siCb.siCtx.retxCntRem--;

         /*Copy the SI msg buff into interface buffer */
         if(cell->siCb.siCtx.warningSiFlag == FALSE)
         {
            SCpyMsgMsg(cell->siCb.siArray[cell->siCb.siCtx.siId-1].si,
                  rgSchCb[cell->instIdx].rgSchInit.region,
                  rgSchCb[cell->instIdx].rgSchInit.pool,
                  &subfrmAlloc->cmnLcInfo.bcchInfo.pdu);
         }
         else
         {
            pdu = rgSCHUtlGetWarningSiPdu(cell);
            RGSCH_NULL_CHECK(cell->instIdx, pdu);
            SCpyMsgMsg(pdu,
                  rgSchCb[cell->instIdx].rgSchInit.region,
                  rgSchCb[cell->instIdx].rgSchInit.pool,
                  &subfrmAlloc->cmnLcInfo.bcchInfo.pdu);
            if(cell->siCb.siCtx.retxCntRem == 0)
            {  
               rgSCHUtlFreeWarningSiPdu(cell);
               cell->siCb.siCtx.warningSiFlag  = FALSE;

            }
         }
#else
         bcch = rgSCHDbmGetSecondBcchOnDlsch(cell);
         bo = (RgSchClcBoRpt *)bcch->boLst.first->node;
         bo->retxCnt--;
         if(bo->retxCnt != cell->siCfg.retxCnt-1)
         {
            sendInd=FALSE;
         }
         i = bo->i;
#endif/*RGR_SI_SCH*/
         subFrm->bcch.pdcch->dci.u.format1aInfo.t.pdschInfo.allocInfo.rv =
            rgSCHCmnDlCalcRvForBcch(cell, TRUE, i);
      }

      /* Added Dl TB count for SIB1 and SI messages transmission.
       * This counter will be incremented only for the first transmission
       * (with RV 0) of these messages*/
#ifdef LTE_L2_MEAS
      if(subFrm->bcch.pdcch->dci.u.format1aInfo.t.pdschInfo.allocInfo.rv == 0)
      {   
         cell->dlUlTbCnt.tbTransDlTotalCnt++;
      }
#endif      
#ifndef RGR_SI_SCH
      if(bo->retxCnt == 0)
      {
         cmLListDelFrm(&bcch->boLst, &bo->boLstEnt);
         /* ccpu00117052 - MOD - Passing double pointer
            for proper NULLP assignment*/
         rgSCHUtlFreeSBuf(cell->instIdx, (Data **)&bo, sizeof(RgSchClcBoRpt));
      }
      rgSCHUtlFillRgInfCmnLcInfo(subFrm, subfrmAlloc, bcch->lcId, sendInd);
#else
      /*Fill the interface info */
      rgSCHUtlFillRgInfCmnLcInfo(subFrm, subfrmAlloc, NULLD, NULLD);

      /* ccpu00132314-ADD-Update the tx power allocation info  
         TODO-Need to add a check for max tx power per symbol */ 
      subfrmAlloc->cmnLcInfo.bcchInfo.txPwrOffset = cellDl->bcchTxPwrOffset;   

      /*mBuf has been already copied above */
#endif/*RGR_SI_SCH*/
   }

   RETVOID;
}


#if RG_UNUSED
/**
 * @brief
 *
 * @details
 *
 *     Function: rgSCHCmnUlSetAllUnSched
 *     Purpose:
 *
 *     Invoked by: Common Scheduler
 *
 *  @param[out] RgSchCmnUlRbAllocInfo *allocInfo
 *  @return  Void
 *
 **/
#ifdef ANSI
PRIVATE Void rgSCHCmnUlSetAllUnSched
(
RgSchCmnUlRbAllocInfo *allocInfo
)
#else
PRIVATE Void rgSCHCmnUlSetAllUnSched(allocInfo)
RgSchCmnUlRbAllocInfo *allocInfo;
#endif
{
   CmLList            *node;

   TRC2(rgSCHCmnUlSetAllUnSched);

   node = allocInfo->contResLst.first;
   while (node)
   {
      rgSCHCmnUlMov2NonSchdCntResLst(allocInfo, (RgSchUeCb *)node->node);
      node = allocInfo->contResLst.first;
   }

   node = allocInfo->retxUeLst.first;
   while (node)
   {
      rgSCHCmnUlMov2NonSchdRetxUeLst(allocInfo, (RgSchUeCb *)node->node);
      node = allocInfo->retxUeLst.first;
   }

   node = allocInfo->ueLst.first;
   while (node)
   {
      rgSCHCmnUlMov2NonSchdUeLst(allocInfo, (RgSchUeCb *)node->node);
      node = allocInfo->ueLst.first;
   }

   RETVOID;
}
#endif

/**
 * @brief
 *
 * @details
 *
 *     Function: rgSCHCmnUlAdd2CntResLst
 *     Purpose:
 *
 *     Invoked by: Common Scheduler
 *
 *  @param[out] RgSchCmnUlRbAllocInfo *allocInfo
 *  @param[in]  RgSchUeCb             *ue
 *  @return  Void
 *
 **/
#ifdef ANSI
PUBLIC Void rgSCHCmnUlAdd2CntResLst
(
RgSchCmnUlRbAllocInfo *allocInfo,
RgSchUeCb             *ue
)
#else
PUBLIC Void rgSCHCmnUlAdd2CntResLst(allocInfo, ue)
RgSchCmnUlRbAllocInfo *allocInfo;
RgSchUeCb             *ue;
#endif
{
   RgSchCmnUeUlAlloc  *ulAllocInfo = &((RG_SCH_CMN_GET_UL_UE(ue))->alloc);
   TRC2(rgSCHCmnUlAdd2CntResLst);
   cmLListAdd2Tail(&allocInfo->contResLst, &ulAllocInfo->reqLnk);
   ulAllocInfo->reqLnk.node = (PTR)ue;
   RETVOID;
}

/**
 * @brief
 *
 * @details
 *
 *     Function: rgSCHCmnUlAdd2UeLst
 *     Purpose:
 *
 *     Invoked by: Common Scheduler
 *
 *  @param[out] RgSchCmnUlRbAllocInfo *allocInfo
 *  @param[in]  RgSchUeCb             *ue
 *  @return  Void
 *
 **/
#ifdef ANSI
PUBLIC Void rgSCHCmnUlAdd2UeLst
(
RgSchCmnUlRbAllocInfo *allocInfo,
RgSchUeCb             *ue
)
#else
PUBLIC Void rgSCHCmnUlAdd2UeLst(allocInfo, ue)
RgSchCmnUlRbAllocInfo *allocInfo;
RgSchUeCb             *ue;
#endif
{
   RgSchCmnUeUlAlloc  *ulAllocInfo = &((RG_SCH_CMN_GET_UL_UE(ue))->alloc);
   TRC2(rgSCHCmnUlAdd2UeLst);
   if (ulAllocInfo->reqLnk.node == NULLP)
   {
      cmLListAdd2Tail(&allocInfo->ueLst, &ulAllocInfo->reqLnk);
      ulAllocInfo->reqLnk.node = (PTR)ue;
   }
   RETVOID;
}

/**
 * @brief
 *
 * @details
 *
 *     Function: rgSCHCmnAllocUlRb
 *     Purpose:  To do RB allocations for uplink
 *
 *     Invoked by: Common Scheduler
 *
 *  @param[in]  RgSchCellCb           *cell
 *  @param[in]  RgSchCmnUlRbAllocInfo *allocInfo
 *  @return  Void
 **/
#ifdef ANSI
PUBLIC Void rgSCHCmnAllocUlRb
(
RgSchCellCb           *cell,
RgSchCmnUlRbAllocInfo *allocInfo
)
#else
PUBLIC Void rgSCHCmnAllocUlRb(cell, allocInfo)
RgSchCellCb           *cell;
RgSchCmnUlRbAllocInfo *allocInfo;
#endif
{
   RgSchUlSf         *sf = allocInfo->sf;
   TRC2(rgSCHCmnAllocUlRb);

   /* [ccpu00123238]-DEL- Move the adaptive retx from here */
   /* Schedule for contention resolution */
   rgSCHCmnUlRbAllocForLst(cell, sf, allocInfo->contResLst.count,
         &allocInfo->contResLst, &allocInfo->schdContResLst,
         &allocInfo->nonSchdContResLst, (Bool)FALSE);

   /* Schedule for new transmissions */
   rgSCHCmnUlRbAllocForLst(cell, sf, allocInfo->ueLst.count,
         &allocInfo->ueLst, &allocInfo->schdUeLst,
         &allocInfo->nonSchdUeLst, (Bool)TRUE);
   RETVOID;
}

/***********************************************************
 *
 *     Func : rgSCHCmnUlRbAllocForLst
 *
 *     Desc : Allocate for a list in cmn rb alloc information passed
 *            in a subframe.
 *
 *     Ret  :
 *
 *     Notes:
 *
 *     File :
 *
 **********************************************************/
#ifdef ANSI
PRIVATE Void rgSCHCmnUlRbAllocForLst
(
RgSchCellCb           *cell,
RgSchUlSf             *sf,
U32                   count,
CmLListCp             *reqLst,
CmLListCp             *schdLst,
CmLListCp             *nonSchdLst,
Bool                  isNewTx
)
#else
PRIVATE Void rgSCHCmnUlRbAllocForLst(cell, sf, count, reqLst, schdLst,
                                     nonSchdLst, isNewTx)
RgSchCellCb           *cell;
RgSchUlSf             *sf;
U32                   count;
CmLListCp             *reqLst;
CmLListCp             *schdLst;
CmLListCp             *nonSchdLst;
Bool                  isNewTx;
#endif
{
   RgSchCmnUlCell   *cellUl = RG_SCH_CMN_GET_UL_CELL(cell);
   CmLList          *lnk;
   U8               sbCount;
   U8               leftOverSb = 0;
   U8               leftOverSbPerUe = 0;
   U8               leftOverSbForNxtUe = 0;
   RgSchUlHole      *hole;
#ifdef LTE_L2_MEAS
#ifdef LTE_TDD
   U8               k;
   CmLteTimingInfo  timeInfo;
#endif    
#endif    
   TRC2(rgSCHCmnUlRbAllocForLst);

   if(schdLst->count == 0)
   {
      cmLListInit(schdLst);
   }

   cmLListInit(nonSchdLst);
#ifdef LTE_L2_MEAS
   if(isNewTx == TRUE)
   {
      cell->sfAllocArr[cell->crntSfIdx].ulUeInfo.numUes = (U8) count;
#ifdef LTE_TDD
      RG_SCH_ADD_TO_CRNT_TIME(cell->crntTime, timeInfo, TFU_ULCNTRL_DLDELTA);
      k = rgSchTddPuschTxKTbl[cell->ulDlCfgIdx][timeInfo.subframe];
      RG_SCH_ADD_TO_CRNT_TIME(timeInfo,
          cell->sfAllocArr[cell->crntSfIdx].ulUeInfo.timingInfo, k);
#else
      RG_SCH_ADD_TO_CRNT_TIME(cell->crntTime,cell->sfAllocArr[cell->crntSfIdx].ulUeInfo.timingInfo,
                            (TFU_ULCNTRL_DLDELTA + RGSCH_PDCCH_PUSCH_DELTA));
#endif
   }
#endif

   for (lnk = reqLst->first; count; lnk = lnk->next, --count)
   {
      RgSchUeCb             *ue = (RgSchUeCb *)lnk->node;
      S16                   ret;
      U8                    maxRb;
      U8                    maxSb=0;


      if ((hole = rgSCHUtlUlHoleFirst(sf)) == NULLP)
      {
         break;
      }


      if(isNewTx == TRUE)
      {
         if (cell->ulSchdType == RGSCH_TYPE_RR)
         {
#ifdef BEST_EFFORT_2_UE_PER_TTI
            ue->ul.subbandShare = sf->availSubbands;
#else
            if(sf->availSubbands < count)
            {/* As we dont have enough subbands available to satisfy all the
               * ues, giving 1 subband to each UE. Once all subbands are used
               * rgSCHUtlUlHoleFirst will return NULl and will return from above
               * */
               ue->ul.subbandShare = 1;
            }
            else
            {
               ue->ul.subbandShare = sf->availSubbands/count;
            }
#endif

         }
         else/* PFS case */
         {
            if(!ue->ul.subbandShare)
            {
               ue->ul.subbandShare = sf->availSubbands;
            }
            ue->ul.subbandShare += (leftOverSbPerUe + leftOverSbForNxtUe);
         }
         leftOverSbForNxtUe = 0;
         maxSb   = hole->num;
         
         sbCount = RGSCH_MIN(ue->ul.subbandShare, maxSb);
      }

      maxRb = rgSCHPwrGetMaxUlRb(cell, ue);
      if(isNewTx == TRUE)
      {
         maxRb = RGSCH_MIN((sbCount * cellUl->sbSize), maxRb);
      }

      ret = rgSCHCmnUlRbAllocForUe(cell, sf, ue, maxRb, hole);
      if (ret == ROK)
      {
         rgSCHCmnUlRbAllocAddUeToLst(cell, ue, schdLst);
         rgSCHCmnUlUeFillAllocInfo(cell, ue);
      }
      else
      {
         rgSCHCmnUlRbAllocAddUeToLst(cell, ue, nonSchdLst);
         ue->isMsg4PdcchWithCrnti = FALSE;
         ue->isSrGrant = FALSE;
      }
      if((isNewTx == TRUE) && 
         (cell->ulSchdType != RGSCH_TYPE_RR) &&
         (count > 1))
      {
         leftOverSb       = ue->ul.subbandShare - (maxSb - hole->num);
         /* Dividing Unused subbands from this UE 
          * to the remaining UE's equally*/
         leftOverSbPerUe += (leftOverSb / (count - 1));
         /* this is for hte next immendiate UE */
         leftOverSbForNxtUe = (leftOverSb % (count - 1));
      }
#ifdef LTE_L2_MEAS
      if(isNewTx == TRUE)
      {
         cell->sfAllocArr[cell->crntSfIdx].ulUeInfo.
         ulAllocInfo[count - 1].rnti   = ue->ueId;
         cell->sfAllocArr[cell->crntSfIdx].ulUeInfo.
         ulAllocInfo[count - 1].numPrb = ue->ul.nPrb;
      }
#endif
      ue->ul.subbandShare = 0; /* This reset will take care of
                                  * all scheduler types */
   }
   for (; count; lnk = lnk->next, --count)
   {
      RgSchUeCb             *ue = (RgSchUeCb *)lnk->node;
      rgSCHCmnUlRbAllocAddUeToLst(cell, ue, nonSchdLst);
      ue->isMsg4PdcchWithCrnti = FALSE;
   }
   RETVOID;
}

#ifdef TFU_UPGRADE
/***********************************************************
 *
 *     Func : rgSCHCmnUlMdfyGrntForCqi
 *
 *     Desc : Modify UL Grant to consider presence of 
 *            CQI along with PUSCH Data.
 *
 *     Ret  :
 *
 *     Notes: 
 *          -  Scale down iTbs based on betaOffset and
 *             size of Acqi Size.
 *          -  Optionally attempt to increase numSb by 1
 *             if input payload size does not fit in due 
 *             to reduced tbSz as a result of iTbsNew.
 *
 *     File :
 *
 **********************************************************/
#ifdef ANSI
PRIVATE S16 rgSCHCmnUlMdfyGrntForCqi
(
RgSchUeCb    *ue,
U32          maxRb,
U32          *numSb,
U8           *iTbs,
U32          hqSz,
U32          stepDownItbs,
U32          effTgt
)
#else
PRIVATE S16 rgSCHCmnUlMdfyGrntForCqi(ue, maxRb, numSb, iTbs, hqSz, stepDownItbs, effTgt)
RgSchUeCb    *ue;
U32          maxRb;
U32          *numSb;
U8           *iTbs;
U32          hqSz;
U32          stepDownItbs;
U32          effTgt;
#endif
{
   RgSchCmnUlCell *cellUl = RG_SCH_CMN_GET_UL_CELL(ue->cell);
   U32  nPrb;
   U32  totREs;
   U32  cqiRiREs;
   U32  hqREs;
   U32  remREsForPusch;
   U32  bitsPerRe;
   U32  tbSz;
   U32  betaOffVal = ue->ul.betaOffstVal;
   U32  cqiRiRptSz = ue->ul.cqiRiSz;
   U32  betaOffHqVal = rgSchCmnBetaHqOffstTbl[ue->ul.betaHqOffst];
   U32  resNumSb = *numSb;
   U32  puschEff = 1000;
   U8   modOdr;
   U8   iMcs;
   Bool mdfyiTbsFlg = FALSE;
   U8   resiTbs = *iTbs;

   TRC2(rgSCHCmnUlMdfyGrntForCqi)

   
   do
   {
      iMcs  = rgSCHCmnUlGetIMcsFrmITbs(resiTbs, RG_SCH_CMN_GET_UE_CTGY(ue));
      RG_SCH_UL_MCS_TO_MODODR(iMcs, modOdr);
      if (RG_SCH_CMN_GET_UE_CTGY(ue) != CM_LTE_UE_CAT_5)
      {
         modOdr = RGSCH_MIN(RGSCH_QM_QPSK, modOdr);
      }
      else
      {
         modOdr = RGSCH_MIN(RGSCH_QM_64QAM, modOdr);
      }
      nPrb = resNumSb * cellUl->sbSize;
      /* Restricting the minumum iTbs requried to modify to 10 */
      if ((nPrb >= maxRb) && (resiTbs <= 10))
      {
         /* Could not accomodate ACQI */
         RETVALUE(RFAILED);
      }
      totREs = nPrb * RG_SCH_CMN_UL_NUM_RE_PER_RB(cellUl);
      tbSz = rgTbSzTbl[0][resiTbs][nPrb-1];
      /*  totalREs/tbSz = num of bits perRE.  */
      cqiRiREs = (totREs * betaOffVal * cqiRiRptSz)/(1000 * tbSz); /* betaOffVal is represented 
                                                                   as parts per 1000 */
      hqREs = (totREs * betaOffHqVal * hqSz)/(1000 * tbSz);
      if ((cqiRiREs + hqREs) < totREs)
      {
         remREsForPusch = totREs - cqiRiREs - hqREs;
         bitsPerRe = (tbSz * 1000)/remREsForPusch; /* Multiplying by 1000 for Interger Oper */
         puschEff = bitsPerRe/modOdr;
      }
      if (puschEff < effTgt)
      {
          /* ensure resultant efficiency for PUSCH Data is within 0.93*/
          break;
      }
      else
      {
         /* Alternate between increasing SB or decreasing iTbs until eff is met */
         if (mdfyiTbsFlg == FALSE)
         {
            if (nPrb < maxRb)
            {
              resNumSb = resNumSb + 1;
            }
            mdfyiTbsFlg = TRUE;
         }
         else
         {
            if (resiTbs > 10)
            {
               resiTbs-= stepDownItbs;
            }
            mdfyiTbsFlg = FALSE;
         }
      }
   }while (1); /* Loop breaks if efficency is met 
                  or returns RFAILED if not able to meet the efficiency */
              
   *numSb = resNumSb;
   *iTbs = resiTbs;

   RETVALUE(ROK);
}
#endif
/***********************************************************
 *
 *     Func : rgSCHCmnUlRbAllocForUe
 *
 *     Desc : Do uplink RB allocation for an UE.
 *
 *     Ret  :
 *
 *     Notes: Note that as of now, for retx, maxRb
 *            is not considered. Alternatives, such
 *            as dropping retx if it crosses maxRb
 *            could be considered.
 *
 *     File :
 *
 **********************************************************/
#ifdef ANSI
PRIVATE S16 rgSCHCmnUlRbAllocForUe
(
RgSchCellCb           *cell,
RgSchUlSf             *sf,
RgSchUeCb             *ue,
U8                    maxRb,
RgSchUlHole           *hole
)
#else
PRIVATE S16 rgSCHCmnUlRbAllocForUe(cell, sf, ue, maxRb, hole)
RgSchCellCb           *cell;
RgSchUlSf             *sf;
RgSchUeCb             *ue;
U8                    maxRb;
RgSchUlHole           *hole;
#endif
{
   RgSchCmnUlCell  *cellUl = RG_SCH_CMN_GET_UL_CELL(cell);
   RgSchCmnUlUe    *ueUl    = RG_SCH_CMN_GET_UL_UE(ue);
   U8               sbSize  = cellUl->sbSize;
   U32              maxBits = ueUl->maxBytesPerUePerTti*8;
   U32              bits;
   RgSchUlAlloc     *alloc = NULLP;
   U32              nPrb = 0;
   U8               iTbs;
   U32              eff;
   U8               numSb;
   Bool             limit = FALSE;
   U8               iMcs;
   U8               iMcsCrnt;
   RgSchUlHqProcCb  *proc = &ue->ul.hqEnt.hqProcCb[cellUl->schdHqProcIdx];
   RgSchPdcch       *pdcch;
   U32              reqSb;
#ifdef TFU_UPGRADE
   CmLteUeCategory  ueCtg = (CmLteUeCategory)(RG_SCH_CMN_GET_UE_CTGY(ue));
   U32              hqSz = 0;
#endif
   TRC2(rgSCHCmnUlRbAllocForUe);

   pdcch = rgSCHCmnPdcchAllocCrntSf(cell, ue);
   if(pdcch == NULLP)
   {
      RLOG_ARG1(L_DEBUG,DBG_CELLID,cell->cellId, 
         "rgSCHCmnUlRbAllocForUe(): Could not get PDCCH for CRNTI:%d",ue->ueId);
      RETVALUE(RFAILED);
   }
   /* ccpu00106531: fixing incorrect Imcs derivation */
   iTbs  = rgSCHCmnUlGetITbs(ue, cell->isCpUlExtend);

   if (!cell->isCpUlExtend)
   {
      RGSCH_ARRAY_BOUND_CHECK(cell->instIdx, rgSchCmnNorUlEff[0], iTbs); 
      eff   = rgSchCmnNorUlEff[0][iTbs];
   }
   else
   {
      RGSCH_ARRAY_BOUND_CHECK(cell->instIdx, rgSchCmnExtUlEff[0], iTbs); 
      eff   = rgSchCmnExtUlEff[0][iTbs];
   }

   bits = ueUl->alloc.reqBytes * 8;
#if 0 /*Commenting out the VoLTE Fix*/
      /*volte fix: Trying to be conservative for smaller allocations.
       * Assuming subband size is 6 PRBs */
      if (ueUl->alloc.reqBytes <= 129)
      {
         iTbs = RGSCH_MIN(iTbs, 10);       
      }
#endif

#if (ERRCLASS & ERRCLS_DEBUG)
   if (!bits)
   {
      RETVALUE(RFAILED);
   }
#endif

   if (bits < rgSCHCmnUlMinTbBitsForITbs(cellUl, iTbs))
   {
      if (!maxRb)
      {
         limit = TRUE;
         numSb = 0;
      }
      else
      {
         numSb = 1;
         nPrb = numSb * sbSize; 
      }
   }
   else
   {
      if (bits > maxBits)
      {
         bits  = maxBits;
         nPrb  = bits * 1024 / eff / RG_SCH_CMN_UL_NUM_RE_PER_RB(cellUl);
         if (nPrb > maxRb)
         {
            limit = TRUE;
            nPrb = maxRb;
         }
         numSb = nPrb / sbSize;
      }
      else
      {
         nPrb = RGSCH_DIV_ROUND((RGSCH_DIV_ROUND(bits * 1024, eff)),
               RG_SCH_CMN_UL_NUM_RE_PER_RB(cellUl));
         if (nPrb > maxRb)
         {
            limit = TRUE;
            nPrb = maxRb;
         }
         numSb = RGSCH_DIV_ROUND(nPrb, sbSize);
      }
   }
   if (!numSb)
   {
      if (limit)
      {
         rgSCHCmnPdcchRlsCrntSf(cell, pdcch);
         RETVALUE(RFAILED);
      }
      else
      {
         ++numSb;
         /* [ccpu00123302]-ADD- Update the nPrb */
         nPrb = numSb * sbSize;
      }
   }
   /*  ADD cap Rbs allocated to maxRb */
   /* Fix for 2 UE not Getting Scheduled in Ul */
   if((numSb*sbSize) > maxRb)
   {   
     /* Fix for 2 UE not Getting Scheduled in Ul */ 
     numSb = (maxRb/sbSize);
     /* [ccpu00123302]-ADD- Update the nPrb */
     nPrb = numSb * sbSize;
   }

   if(!numSb)
   {
       /* Fix for 2 UE not Getting Scheduled in Ul */
       numSb = 1;
       /* [ccpu00123302]-ADD- Update the nPrb */
       nPrb = numSb * sbSize;
   }


   reqSb = numSb;
#ifdef TFU_UPGRADE
#ifdef LTE_TDD
   if (rgSCHUtlGetUeANFdbkInfo(ue, &cellUl->schdTime) != NULLP)
   {
      /* TODO: changes need to be done for Hq multiplexing */

        hqSz = 2;
   }
#else
/* TODO: FDD changes need to be done for hqSz. For now hqSz will go as 0 for FDD case. */
#endif
   /* Restricting the aperiodic cqi to 1 per tti */
   ue->dl.acqiResGrntd = FALSE;
   if((sf->aCqiReqCount < RG_SCH_MAX_ACQI_PER_ULSF) && (TRUE == ue->dl.reqForCqi)) 
   {
      if (rgSCHCmnUlMdfyGrntForCqi(ue, maxRb, &reqSb, &iTbs, hqSz, RG_SCH_ITBS_STEP_DOWN_ACQI, RG_SCH_ACQI_EFF_TGT) == ROK)
      { 
         sf->aCqiReqCount++;
         ue->dl.acqiResGrntd = TRUE;
      }
   }
   else if (ue->dl.pCqiPrsnt == TRUE)
   {
      if (ue->ul.cqiRiSz > 0)
      {
         rgSCHCmnUlMdfyGrntForCqi(ue, maxRb, &reqSb, &iTbs, hqSz, RG_SCH_ITBS_STEP_DOWN_PCQI_RI_HQ, RG_SCH_PCQI_RI_HQ_EFF_TGT);
      }    
   }
   else if (hqSz == 2)
   {
      ue->ul.cqiRiSz = 0;
      rgSCHCmnUlMdfyGrntForCqi(ue, maxRb, &reqSb, &iTbs, hqSz, RG_SCH_ITBS_STEP_DOWN_PCQI_RI_HQ, RG_SCH_PCQI_RI_HQ_EFF_TGT);
   }
   
   /* ccpu00106531: fixing incorrect Imcs derivation */
   iMcs  = rgSCHCmnUlGetIMcsFrmITbs(iTbs,ueCtg);
   iMcsCrnt = iMcs;
   numSb = reqSb;
#endif   

   if(numSb)
   {
      alloc = rgSCHCmnUlSbAlloc(sf, (U8) (RGSCH_MIN(numSb, cellUl->maxSbPerUe)),\
                                hole);
   }
   if (alloc == NULLP)
   {
      RLOG_ARG2(L_ERROR,DBG_CELLID,cell->cellId, 
         "rgSCHCmnUlRbAllocForUe(): Could not get UlAlloc %d CRNTI:%d",numSb,ue->ueId);
      rgSCHCmnPdcchRlsCrntSf(cell, pdcch);
      RETVALUE(RFAILED);
   }
   rgSCHCmnUlAllocFillRbInfo(cell, sf, alloc);
#ifdef LTE_L2_MEAS
   sf->totPrb  += alloc->grnt.numRb;
   ue->ul.nPrb = alloc->grnt.numRb;
#endif
   if (ue->csgMmbrSta != TRUE)
   {
      cellUl->ncsgPrbCnt += alloc->grnt.numRb;
   }
   cellUl->totPrbCnt += alloc->grnt.numRb;
   alloc->pdcch = pdcch;
   alloc->grnt.iMcs = iMcs;
   alloc->grnt.iMcsCrnt = iMcsCrnt;
   alloc->grnt.hop = 0;
   /* Initial Num RBs support for UCI on PUSCH */
#ifdef TFU_UPGRADE
   ue->initNumRbs = alloc->grnt.numRb;
#endif
   alloc->forMsg3 = FALSE;
   RGSCH_ARRAY_BOUND_CHECK(cell->instIdx, rgTbSzTbl[0], (iTbs)); 

   ueUl->alloc.allocdBytes = rgTbSzTbl[0][iTbs][alloc->grnt.numRb-1] / 8;

#ifdef BEST_EFFORT_2_UE_PER_TTI
   rgUlrate_grant += ueUl->alloc.allocdBytes;
#endif

   alloc->grnt.datSz = ueUl->alloc.allocdBytes;
   RG_SCH_UL_MCS_TO_MODODR(iMcs, alloc->grnt.modOdr);
   alloc->grnt.isRtx = FALSE;
   alloc->hqProc = proc;
   alloc->hqProc->ulSfIdx = cellUl->schdIdx;
   alloc->ue = ue;
   /*commenting to retain the rnti used for transmission SPS/c-rnti */
   alloc->rnti = ue->ueId;
   ueUl->alloc.alloc = alloc;
   /*rntiwari-Adding the debug for generating the graph.*/
   /* No grant attr recorded now */
   RETVALUE(ROK);
}

/***********************************************************
 *
 *     Func : rgSCHCmnUlRbAllocAddUeToLst
 *
 *     Desc : Add UE to list (scheduled/non-scheduled list)
 *            for UL RB allocation information.
 *
 *     Ret  :
 *
 *     Notes:
 *
 *     File :
 *
 **********************************************************/
#ifdef ANSI
PUBLIC Void rgSCHCmnUlRbAllocAddUeToLst
(
RgSchCellCb           *cell,
RgSchUeCb             *ue,
CmLListCp             *lst
)
#else
PUBLIC Void rgSCHCmnUlRbAllocAddUeToLst(cell, ue, lst)
RgSchCellCb           *cell;
RgSchUeCb             *ue;
CmLListCp             *lst;
#endif
{
   RgSchCmnUlUe   *ueUl   = RG_SCH_CMN_GET_UL_UE(ue);
   TRC2(rgSCHCmnUlRbAllocAddUeToLst);
   UNUSED(cell);

   cmLListAdd2Tail(lst, &ueUl->alloc.schdLstLnk);
   ueUl->alloc.schdLstLnk.node = (PTR)ue;
}


/**
 * @brief This function Processes the Final Allocations
 *        made by the RB Allocator against the requested.
 *
 * @details
 *
 *     Function: rgSCHCmnUlAllocFnlz
 *     Purpose:  This function Processes the Final Allocations
 *               made by the RB Allocator against the requested.
 *
 *     Invoked by: Common Scheduler
 *
 *  @param[in]  RgSchCellCb           *cell
 *  @param[in]  RgSchCmnUlRbAllocInfo *allocInfo
 *  @return  Void
 *
 **/
#ifdef ANSI
PRIVATE Void rgSCHCmnUlAllocFnlz
(
RgSchCellCb           *cell,
RgSchCmnUlRbAllocInfo *allocInfo
)
#else
PRIVATE Void rgSCHCmnUlAllocFnlz(cell, allocInfo)
RgSchCellCb           *cell;
RgSchCmnUlRbAllocInfo *allocInfo;
#endif
{
   RgSchCmnCell *cellSch = RG_SCH_CMN_GET_CELL(cell);
   TRC2(rgSCHCmnUlAllocFnlz);

   /* call scheduler specific Finalization */
   cellSch->apisUl->rgSCHUlAllocFnlz(cell, allocInfo);

   RETVOID;
}

/**
 * @brief This function Processes the Final Allocations
 *        made by the RB Allocator against the requested.
 *
 * @details
 *
 *     Function: rgSCHCmnDlAllocFnlz
 *     Purpose:  This function Processes the Final Allocations
 *               made by the RB Allocator against the requested.
 *
 *     Invoked by: Common Scheduler
 *
 *  @param[in]  RgSchCellCb           *cell
 *  @param[in]  RgSchCmnDlRbAllocInfo *allocInfo
 *  @return  Void
 *
 **/
#ifdef ANSI
PRIVATE Void rgSCHCmnDlAllocFnlz
(
RgSchCellCb           *cell,
RgSchCmnDlRbAllocInfo *allocInfo
)
#else
PRIVATE Void rgSCHCmnDlAllocFnlz(cell, allocInfo)
RgSchCellCb           *cell;
RgSchCmnDlRbAllocInfo *allocInfo;
#endif
{
   RgSchCmnCell           *cellSch = RG_SCH_CMN_GET_CELL(cell);
   TRC2(rgSCHCmnDlAllocFnlz);

   rgSCHCmnDlCcchRetxFnlz(cell, allocInfo);
   rgSCHCmnDlCcchTxFnlz(cell, allocInfo);
#ifdef RGR_V1
   /*Added below functions for handling CCCH SDU transmission received after
    * guard timer expiry*/
   rgSCHCmnDlCcchSduRetxFnlz(cell, allocInfo);
   rgSCHCmnDlCcchSduTxFnlz(cell, allocInfo);
#endif
   rgSCHCmnDlRaRspFnlz(cell, allocInfo);

#ifdef LTEMAC_SPS
   /* Invoke SPS module for SPS service finalization */
   rgSCHCmnSpsDlAllocFnlz(cell, allocInfo);
#endif /* LTEMAC_SPS */

   /* call scheduler specific Finalization */
   cellSch->apisDl->rgSCHDlAllocFnlz(cell, allocInfo);

   /* Stack Crash problem for TRACE5 Changes. Added the return below */
   RETVOID;

}

#ifdef RG_UNUSED
/**
 * @brief Update an uplink subframe.
 *
 * @details
 *
 *     Function : rgSCHCmnUlUpdSf
 *
 *     For each allocation
 *      - if no more tx needed
 *         - Release allocation
 *      - else
 *         - Perform retransmission
 *
 *  @param[in]  RgSchUlSf *sf
 *  @return  Void
 **/
#ifdef ANSI
PRIVATE Void rgSCHCmnUlUpdSf
(
RgSchCellCb           *cell,
RgSchCmnUlRbAllocInfo *allocInfo,
RgSchUlSf *sf
)
#else
PRIVATE Void rgSCHCmnUlUpdSf(cell, allocInfo, sf)
RgSchCellCb           *cell;
RgSchCmnUlRbAllocInfo *allocInfo;
RgSchUlSf *sf;
#endif
{
   CmLList        *lnk;
   TRC2(rgSCHCmnUlUpdSf);

   while ((lnk = sf->allocs.first))
   {
      RgSchUlAlloc  *alloc = (RgSchUlAlloc *)lnk->node;
      lnk = lnk->next;

      if ((alloc->hqProc->rcvdCrcInd) || (alloc->hqProc->remTx == 0))
      {
      }
      else
      {
         /* If need to handle all retx together, run another loop separately */
         rgSCHCmnUlHndlAllocRetx(cell, allocInfo, sf, alloc);
      }
      rgSCHCmnUlRlsUlAlloc(cell, sf, alloc);
   }

   /* By this time, all allocs would have been cleared and
    * SF is reset to be made ready for new allocations. */
   rgSCHCmnUlSfReset(cell, sf);
   /* In case there are timing problems due to msg3
    * allocations being done in advance, (which will
    * probably happen with the current FDD code that
    * handles 8 subframes) one solution
    * could be to hold the (recent) msg3 allocs in a separate
    * list, and then possibly add that to the actual
    * list later. So at this time while allocations are
    * traversed, the recent msg3 ones are not seen. Anytime after
    * this (a good time is when the usual allocations
    * are made), msg3 allocations could be transferred to the
    * normal list. Not doing this now as it is assumed
    * that incorporation of TDD shall take care of this.
    */


   RETVOID;
}

/**
 * @brief Handle uplink allocation for retransmission.
 *
 * @details
 *
 *     Function : rgSCHCmnUlHndlAllocRetx
 *
 *     Processing Steps:
 *     - Add to queue for retx.
 *     - Do not release here, release happends as part
 *       of the loop that calls this function.
 *
 *  @param[in]  RgSchCellCb           *cell
 *  @param[in]  RgSchCmnUlRbAllocInfo *allocInfo
 *  @param[in]  RgSchUlSf *sf
 *  @param[in]  RgSchUlAlloc  *alloc
 *  @return  Void
 **/
#ifdef ANSI
PRIVATE Void rgSCHCmnUlHndlAllocRetx
(
RgSchCellCb           *cell,
RgSchCmnUlRbAllocInfo *allocInfo,
RgSchUlSf     *sf,
RgSchUlAlloc  *alloc
)
#else
PRIVATE Void rgSCHCmnUlHndlAllocRetx(cell, allocInfo, sf, alloc)
RgSchCellCb           *cell;
RgSchCmnUlRbAllocInfo *allocInfo;
RgSchUlSf     *sf;
RgSchUlAlloc  *alloc;
#endif
{
   U32            bytes;
   RgSchCmnUlUe   *ueUl;
   TRC2(rgSCHCmnUlHndlAllocRetx);
   bytes = \
      rgTbSzTbl[0][rgSCHCmnUlGetITbsFrmIMcs(alloc->grnt.iMcs)]\
                                     [alloc->grnt.numRb-1]/8;
   if (!alloc->forMsg3)
   {
      ueUl = RG_SCH_CMN_GET_UL_UE(alloc->ue);
      ueUl->alloc.reqBytes = bytes;
      rgSCHUhmRetx(alloc->hqProc);
      rgSCHCmnUlAdd2RetxUeLst(allocInfo, alloc->ue);
   }
   else
   {
      /* RACHO msg3 retx handling. Part of RACH procedure changes. */
      retxAlloc = rgSCHCmnUlGetUlAlloc(cell, sf, alloc->numSb);
      if (retxAlloc == NULLP)
      {
         RLOG_ARG1(L_ERROR,DBG_CELLID,cell->cellId,
               "rgSCHCmnUlRbAllocForUe():Could not get UlAlloc for msg3Retx RNTI:%d",
               alloc->rnti);
         RETVOID;
      }
      retxAlloc->grnt.iMcs = alloc->grnt.iMcs;
      retxAlloc->grnt.iMcsCrnt = rgSchCmnUlRvIdxToIMcsTbl\
                                 [alloc->hqProc->rvIdx];
      retxAlloc->grnt.nDmrs    = 0;
      retxAlloc->grnt.hop      = 0;
      retxAlloc->grnt.delayBit = 0;
      retxAlloc->rnti          = alloc->rnti;
      retxAlloc->ue            = NULLP;
      retxAlloc->pdcch         = FALSE;
      retxAlloc->forMsg3       = TRUE;
      retxAlloc->raCb          = alloc->raCb;
      retxAlloc->hqProc        = alloc->hqProc;
      rgSCHUhmRetx(retxAlloc->hqProc);
   }
   RETVOID;
}
#endif

/**
 * @brief Uplink Scheduling Handler.
 *
 * @details
 *
 *     Function: rgSCHCmnUlAlloc
 *     Purpose:  This function Handles Uplink Scheduling.
 *
 *     Invoked by: Common Scheduler
 *
 *  @param[in]  RgSchCellCb *cell
 *  @return  Void
 **/
/* ccpu00132653- The definition of this function made common for TDD and FDD*/
#ifdef ANSI
PRIVATE Void rgSCHCmnUlAlloc
(
RgSchCellCb  *cell
)
#else
PRIVATE Void rgSCHCmnUlAlloc(cell)
RgSchCellCb  *cell;
#endif
{
   RgSchCmnCell           *cellSch = RG_SCH_CMN_GET_CELL(cell);
   RgSchCmnUlCell         *cellUl = RG_SCH_CMN_GET_UL_CELL(cell);
   RgSchCmnDlCell         *cellDl = RG_SCH_CMN_GET_DL_CELL(cell);
   RgSchCmnUlRbAllocInfo  allocInfo;
   RgSchCmnUlRbAllocInfo  *allocInfoRef = &allocInfo;

   TRC2(rgSCHCmnUlAlloc);

   /* Initializing RgSchCmnUlRbAllocInfo structure */
   rgSCHCmnInitUlRbAllocInfo(allocInfoRef);

   /* Get Uplink Subframe */
   allocInfoRef->sf = &cellUl->ulSfArr[cellUl->schdIdx];
#ifdef LTE_L2_MEAS
   /* initializing the UL PRB count */
   allocInfoRef->sf->totPrb = 0;
#endif

#ifdef LTEMAC_SPS
   rgSCHCmnSpsUlTti(cell, allocInfoRef);
#endif

   if(*allocInfoRef->sf->allocCountRef == 0)
   {            
      RgSchUlHole     *hole;

      if ((hole = rgSCHUtlUlHoleFirst(allocInfoRef->sf)) != NULLP)
      {
         /* Sanity check of holeDb */
         if (allocInfoRef->sf->holeDb->count == 1 && hole->start == 0) 
         {
            hole->num = cell->dynCfiCb.bwInfo[cellDl->currCfi].numSb;   
            /* Re-Initialize available subbands because of CFI change*/
            allocInfoRef->sf->availSubbands = cell->dynCfiCb.\
                                              bwInfo[cellDl->currCfi].numSb;
         }
         else
         {
            RLOG_ARG0(L_ERROR,DBG_CELLID,cell->cellId,
                     "Error! holeDb sanity check failed");
         }
      }
   }

   /* Fix: Adaptive re-transmissions prioritised over other transmissions */
   /* perform adaptive retransmissions */
   rgSCHCmnUlSfReTxAllocs(cell, allocInfoRef->sf);

   /* Fix: syed Adaptive Msg3 Retx crash. Release all
    Harq processes for which adap Retx failed, to avoid 
    blocking. This step should be done before New TX 
    scheduling to make hqProc available. Right now we
    dont check if proc is in adap Retx list for considering
    it to be available. But now with this release that 
    functionality would be correct. */
   rgSCHCmnUlSfRlsRetxProcs(cell, allocInfoRef->sf);  

   /* Specific UL scheduler to perform UE scheduling */
   cellSch->apisUl->rgSCHUlSched(cell, allocInfoRef);

   /* Call UL RB allocator module */
   rgSCHCmnAllocUlRb(cell, allocInfoRef);

   /* Do group power control for PUSCH */
   rgSCHCmnGrpPwrCntrlPusch(cell, allocInfoRef->sf);

   cell->sc.apis->rgSCHDrxStrtInActvTmrInUl(cell);

   rgSCHCmnUlAllocFnlz(cell, allocInfoRef);

   RETVOID;
}

/**
 * @brief Consolidate Subframe Allocations.
 *
 * @details
 *
 *     Function: rgSCHCmnCnsldtSfAlloc
 *     Purpose:  Consolidate Subframe Allocations. Send the scheduled
 *     allocations to MAC for StaInd generation to Higher layers and
 *     for MUXing. PST's RgInfSfAlloc to MAC instance.
 *
 *     Invoked by: Common Scheduler
 *
 *  @param[in]  RgSchCellCb *cell
 *  @return  Void
 **/
#ifdef ANSI
PRIVATE Void rgSCHCmnCnsldtSfAlloc
(
RgSchCellCb  *cell
)
#else
PRIVATE Void rgSCHCmnCnsldtSfAlloc(cell)
RgSchCellCb  *cell;
#endif
{
   RgInfSfAlloc    *subfrmAlloc;
   CmLteTimingInfo frm;
   Pst             pst;
   RgSchDlSf       *dlSf;
   CmLListCp       dlDrxInactvTmrLst;
	 RgSchCmnCell    *cellSch = RG_SCH_CMN_GET_CELL(cell);

   TRC2(rgSCHCmnCnsldtSfAlloc);

   cmLListInit(&dlDrxInactvTmrLst);
   subfrmAlloc = &(cell->sfAllocArr[cell->crntSfIdx]);
   rgSCHUtlResetSfAlloc(subfrmAlloc,FALSE,TRUE);
#ifdef LTE_TDD
		  RG_SCH_ADD_TO_CRNT_TIME(cell->crntTime, subfrmAlloc->timingInfo, RG_DL_DELTA - TFU_DELTA);
#else
		  /*
  	 		* TFU_DLDATA_DLDELTA is used in this calculation because the subfrmAlloc
	  		* timingInfo which is being calculated here will be used by MAC
	 	  	*/
		  RG_SCH_ADD_TO_CRNT_TIME(cell->crntTime, subfrmAlloc->timingInfo,
												RG_DL_DELTA - TFU_DLDATA_DLDELTA);
#endif

		  /* Get Downlink Subframe */
		  frm   = cell->crntTime;
		  RGSCH_INCR_SUB_FRAME(frm, RG_SCH_CMN_DL_DELTA);
		  dlSf = rgSCHUtlSubFrmGet(cell, frm);

		  /* Fill the allocation Info */
		  rgSCHUtlFillRgInfRarInfo(dlSf, subfrmAlloc, cell);
		  rgSCHUtlFillRgInfUeInfo(dlSf, subfrmAlloc, cell);

		  rgSCHUtlGetDrxSchdUesInDl(cell,subfrmAlloc,&dlDrxInactvTmrLst);
		  /*re/start DRX inactivity timer for the UEs*/
		  (Void)rgSCHDrxStrtInActvTmr(cell,&dlDrxInactvTmrLst,RG_SCH_DRX_DL);
		  /* Send the allocations to MAC for MUXing */
		  rgSCHUtlGetPstToLyr(&pst, &rgSchCb[cell->instIdx], cell->macInst);
		  subfrmAlloc->cellId = cell->cellId;
		  /* Populate the List of UEs needing PDB-based Flow control */
		  cellSch->apisDl->rgSCHDlFillFlwCtrlInfo(cell, subfrmAlloc);
#ifdef LTE_L2_MEAS
		  if((subfrmAlloc->rarInfo.numRaRntis) ||
					(subfrmAlloc->ueInfo.numUes)      ||
					(subfrmAlloc->cmnLcInfo.bitMask)  ||
					(subfrmAlloc->ulUeInfo.numUes)    ||
		 		  (subfrmAlloc->flowCntrlInfo.numUes))
#else
		  if((subfrmAlloc->rarInfo.numRaRntis) ||
		 		(subfrmAlloc->ueInfo.numUes)      ||
		 		(subfrmAlloc->cmnLcInfo.bitMask)  ||
		 		(subfrmAlloc->flowCntrlInfo.numUes))
#endif
   {
         RgSchMacSfAlloc(&pst, subfrmAlloc);
   }

   RETVOID;
}

/**
 * @brief Initialize the DL Allocation Information Structure.
 *
 * @details
 *
 *     Function: rgSCHCmnInitDlRbAllocInfo
 *     Purpose:  Initialize the DL Allocation Information Structure.
 *
 *     Invoked by: Common Scheduler
 *
 *  @param[out]  RgSchCmnDlRbAllocInfo  *allocInfo
 *  @return  Void
 **/
#ifdef ANSI
PRIVATE Void rgSCHCmnInitDlRbAllocInfo
(
RgSchCmnDlRbAllocInfo  *allocInfo
)
#else
PRIVATE Void rgSCHCmnInitDlRbAllocInfo(allocInfo)
RgSchCmnDlRbAllocInfo  *allocInfo;
#endif
{
   TRC2(rgSCHCmnInitDlRbAllocInfo);
   cmMemset((U8 *)&allocInfo->pcchAlloc, (U8)0, sizeof(RgSchDlRbAlloc));
   cmMemset((U8 *)&allocInfo->bcchAlloc, (U8)0, sizeof(RgSchDlRbAlloc));
   cmMemset((U8 *)allocInfo->raRspAlloc, (U8)0,
         RG_SCH_CMN_MAX_CMN_PDCCH*sizeof(RgSchDlRbAlloc));

   allocInfo->msg4Alloc.msg4DlSf = NULLP;
   cmLListInit(&allocInfo->msg4Alloc.msg4TxLst);
   cmLListInit(&allocInfo->msg4Alloc.msg4RetxLst);
   cmLListInit(&allocInfo->msg4Alloc.schdMsg4TxLst);
   cmLListInit(&allocInfo->msg4Alloc.schdMsg4RetxLst);
   cmLListInit(&allocInfo->msg4Alloc.nonSchdMsg4TxLst);
   cmLListInit(&allocInfo->msg4Alloc.nonSchdMsg4RetxLst);
#ifdef RGR_V1
   allocInfo->ccchSduAlloc.ccchSduDlSf = NULLP;
   cmLListInit(&allocInfo->ccchSduAlloc.ccchSduTxLst);
   cmLListInit(&allocInfo->ccchSduAlloc.ccchSduRetxLst);
   cmLListInit(&allocInfo->ccchSduAlloc.schdCcchSduTxLst);
   cmLListInit(&allocInfo->ccchSduAlloc.schdCcchSduRetxLst);
   cmLListInit(&allocInfo->ccchSduAlloc.nonSchdCcchSduTxLst);
   cmLListInit(&allocInfo->ccchSduAlloc.nonSchdCcchSduRetxLst);
#endif

   allocInfo->dedAlloc.dedDlSf = NULLP;
   cmLListInit(&allocInfo->dedAlloc.txHqPLst);
   cmLListInit(&allocInfo->dedAlloc.retxHqPLst);
   cmLListInit(&allocInfo->dedAlloc.schdTxHqPLst);
   cmLListInit(&allocInfo->dedAlloc.schdRetxHqPLst);
   cmLListInit(&allocInfo->dedAlloc.nonSchdTxHqPLst);
   cmLListInit(&allocInfo->dedAlloc.nonSchdRetxHqPLst);

   cmLListInit(&allocInfo->dedAlloc.txRetxHqPLst);
   cmLListInit(&allocInfo->dedAlloc.schdTxRetxHqPLst);
   cmLListInit(&allocInfo->dedAlloc.nonSchdTxRetxHqPLst);
#ifdef LTEMAC_SPS
   cmLListInit(&allocInfo->dedAlloc.txSpsHqPLst);
   cmLListInit(&allocInfo->dedAlloc.retxSpsHqPLst);
   cmLListInit(&allocInfo->dedAlloc.schdTxSpsHqPLst);
   cmLListInit(&allocInfo->dedAlloc.schdRetxSpsHqPLst);
   cmLListInit(&allocInfo->dedAlloc.nonSchdTxSpsHqPLst);
   cmLListInit(&allocInfo->dedAlloc.nonSchdRetxSpsHqPLst);
#endif
   RETVOID;
}

/**
 * @brief Initialize the UL Allocation Information Structure.
 *
 * @details
 *
 *     Function: rgSCHCmnInitUlRbAllocInfo
 *     Purpose:  Initialize the UL Allocation Information Structure.
 *
 *     Invoked by: Common Scheduler
 *
 *  @param[out]  RgSchCmnUlRbAllocInfo  *allocInfo
 *  @return  Void
 **/
#ifdef ANSI
PRIVATE Void rgSCHCmnInitUlRbAllocInfo
(
RgSchCmnUlRbAllocInfo  *allocInfo
)
#else
PRIVATE Void rgSCHCmnInitUlRbAllocInfo(allocInfo)
RgSchCmnUlRbAllocInfo  *allocInfo;
#endif
{
   TRC2(rgSCHCmnInitUlRbAllocInfo);
   allocInfo->sf = NULLP;
   cmLListInit(&allocInfo->contResLst);
   cmLListInit(&allocInfo->schdContResLst);
   cmLListInit(&allocInfo->nonSchdContResLst);
   cmLListInit(&allocInfo->ueLst);
   cmLListInit(&allocInfo->schdUeLst);
   cmLListInit(&allocInfo->nonSchdUeLst);

   RETVOID;
}

/**
 * @brief Dowlink Scheduling Handler.
 *
 * @details
 *
 *     Function: rgSCHCmnDlAlloc
 *     Purpose:  This function calls common scheduler APIs to
 *     schedule for BCCH/PCCH/RAR/CCCH and subsequently calls
 *     specific scheduler handler to allocate for UEs dedicated
 *     DL channels. It then invokes Allocator for actual RB
 *     allocations. It processes on the actual resources allocated
 *     against requested to the allocator module.
 *
 *     Invoked by: Common Scheduler
 *
 *  @param[in]  RgSchCellCb *cell
 *  @return  Void
 **/
#ifdef ANSI
PRIVATE Void rgSCHCmnDlAlloc
(
RgSchCellCb  *cell
)
#else
PRIVATE Void rgSCHCmnDlAlloc(cell)
RgSchCellCb  *cell;
#endif
{
   RgSchCmnCell           *cellSch = RG_SCH_CMN_GET_CELL(cell);
   CmLteTimingInfo        frm;
   RgSchDlSf              *dlSf;
   RgSchCmnDlRbAllocInfo  allocInfo;

   TRC2(rgSCHCmnDlAlloc);

   /* Initializing RgSchCmnUlRbAllocInfo structure.*/
   rgSCHCmnInitDlRbAllocInfo(&allocInfo);
   /* Get Downlink Subframe */
   frm   = cell->crntTime;
   RGSCH_INCR_SUB_FRAME(frm, RG_SCH_CMN_DL_DELTA);
   cellSch->dl.time = frm;

   dlSf = rgSCHUtlSubFrmGet(cell, frm);
   dlSf->schdAmbrNxt = FALSE;

   /* Updating the Subframe information in RBAllocInfo */
   allocInfo.dedAlloc.dedDlSf   = dlSf;
   allocInfo.msg4Alloc.msg4DlSf = dlSf;

   /* Done from rgSCHCmnDlBcchPcch in previous TTI */

   /* LTE_ADV_FLAG_REMOVED_START */
   /* Determine next scheduling subframe is ABS or not */
   if(RGR_ENABLE == cell->lteAdvCb.absCfg.status)
   {
      cell->lteAdvCb.absPatternDlIdx = 
         ((frm.sfn*RGSCH_NUM_SUB_FRAMES) + frm.subframe) % RGR_ABS_PATTERN_LEN;
      cell->lteAdvCb.absDlSfInfo = (RgSchAbsSfEnum)(cell->lteAdvCb.absCfg.absPattern[
            cell->lteAdvCb.absPatternDlIdx]);

   }
   else
   {
      cell->lteAdvCb.absDlSfInfo = RG_SCH_ABS_DISABLED;
   }
   /* LTE_ADV_FLAG_REMOVED_END */

#ifdef RGR_V1
   allocInfo.ccchSduAlloc.ccchSduDlSf = dlSf;
#endif

   /* This function performs RAR/CCCH scheduling */
   rgSCHCmnDlCmnChAlloc(cell, &allocInfo);

   dlSf->remUeCnt = cellSch->dl.maxUePerDlSf - 
                    RGSCH_MIN(dlSf->schdCcchUe, cellSch->dl.maxCcchPerDlSf);
#ifdef LTEMAC_SPS
   /* Schedule for SPS services and UEs */   
   rgSCHCmnSpsDlSched(cell, &allocInfo);    

   if ((dlSf->type2Start != 0) && \
         (dlSf->type2Start == (((cell->spsCellCfg.maxSpsDlBw + cell->rbgSize - 1)/cell->rbgSize) * cell->rbgSize)))
   {
      RgSchCmnDlCellSpsInfo *dlSpsCell = RG_SCH_CMN_SPS_GET_DL_CELL(cell);
      CmLteTimingInfo       schdTime = cell->crntTime;
      U32 idx1  = 0, count = 0, raType2Mask = 0, dlSpsSfIdx = 0;
      U8 oldType2Start = dlSf->type2Start;

      dlSf->type2Start = 0;
      dlSf->type2End = 0;
      RGSCH_INCR_SUB_FRAME(schdTime, RG_SCH_CMN_DL_DELTA);
      dlSpsSfIdx = (schdTime.sfn * 10 + schdTime.subframe) % dlSpsCell->spsPrdLcmVal;
      for (idx1 = RG_SCH_NUM_RATYPE2_32BIT_MASK; idx1 > 0; idx1--)
      {
         if (0 != (raType2Mask = dlSpsCell->spsSfTbl[dlSpsSfIdx].
                  spsAllocInfo.raType2Mask[idx1 - 1]))
         {
            while (!(raType2Mask & 0x01))
            {
               count++;
               raType2Mask >>=1;
            }
            dlSf->type2Start = (32 - count) + (( idx1 -1) * 32);
            break;
         }
      }

      dlSf->bwAlloced -= (oldType2Start - dlSf->type2Start);
      dlSf->spsAllocdBw += (oldType2Start - dlSf->type2Start);
      dlSf->type2End = RGSCH_CEIL(dlSf->type2Start, cell->rbgSize);
   }
#endif

   /* LTE_ADV_FLAG_REMOVED_START */
   if(RG_SCH_ABS_ENABLED_ABS_SF == cell->lteAdvCb.absDlSfInfo)
   {
      if(cell->lteAdvCb.absCfg.absPatternType & RGR_ABS_MUTE)
      {
         /* The pattern type is RGR_ABS_MUTE, then eNB need to blank the subframe */
      } 
      else
      {
         /* The pattern type is RGR_ABS_TRANSMIT, then eNB need to schedule the UE */
         cellSch->apisDl->rgSCHDlSched(cell, &allocInfo);         
      }
   }
   else
   {
      /* Specific DL scheduler to perform UE scheduling */
      cellSch->apisDl->rgSCHDlSched(cell, &allocInfo);      
   }
   /* LTE_ADV_FLAG_REMOVED_END */

   /* call common allocator for RB Allocation */
   rgSCHCmnDlRbAlloc(cell, &allocInfo);

   /* Perform Pdcch allocations for PDCCH Order Q.
    * As of now, giving this the least preference.
    * This func call could be moved above other allocations
    * as per need */
   rgSCHCmnGenPdcchOrder(cell, dlSf);

   /* Do group power control for PUCCH */
   rgSCHCmnGrpPwrCntrlPucch(cell, dlSf);

   /* Finalize the Allocations for reqested Against alloced */
   rgSCHCmnDlAllocFnlz(cell, &allocInfo);

   /* Call ACK NACK module to add to dlsf Queue */
   rgSCHAckNakRepAddToQ (cell, dlSf);

   RETVOID;
}

/**
 * @brief Scheduling for PUCCH group power control.
 *
 * @details
 *
 *     Function: rgSCHCmnGrpPwrCntrlPucch
 *     Purpose: This function does group power control for PUCCH
 *     corresponding to the subframe for which DL UE allocations
 *     have happended.
 *
 *     Invoked by: Common Scheduler
 *
 *  @param[in]  RgSchCellCb *cell
 *  @return  Void
 **/
#ifdef ANSI
PRIVATE Void rgSCHCmnGrpPwrCntrlPucch
(
RgSchCellCb            *cell,
RgSchDlSf              *dlSf
)
#else
PRIVATE Void rgSCHCmnGrpPwrCntrlPucch(cell, dlSf)
RgSchCellCb            *cell;
RgSchDlSf              *dlSf;
#endif
{
   TRC2(rgSCHCmnGrpPwrCntrlPucch);

   rgSCHPwrGrpCntrlPucch(cell, dlSf);

   RETVOID;
}

/**
 * @brief Scheduling for PUSCH group power control.
 *
 * @details
 *
 *     Function: rgSCHCmnGrpPwrCntrlPusch
 *     Purpose: This function does group power control, for
 *     the subframe for which UL allocation has (just) happened.
 *
 *     Invoked by: Common Scheduler
 *
 *  @param[in]  RgSchCellCb *cell
 *  @param[in]  RgSchUlSf   *ulSf
 *  @return  Void
 **/
#ifdef ANSI
PRIVATE Void rgSCHCmnGrpPwrCntrlPusch
(
RgSchCellCb            *cell,
RgSchUlSf              *ulSf
)
#else
PRIVATE Void rgSCHCmnGrpPwrCntrlPusch(cell, ulSf)
RgSchCellCb            *cell;
RgSchUlSf              *ulSf;
#endif
{
   RgSchCmnCell           *cellSch = RG_SCH_CMN_GET_CELL(cell);
   CmLteTimingInfo        frm;
   RgSchDlSf              *dlSf;

   TRC2(rgSCHCmnGrpPwrCntrlPusch);

   /* Got to pass DL SF corresponding to UL SF, so get that first.
    * There is no easy way of getting dlSf by having the RgSchUlSf*,
    * so use the UL delta from current time to get the DL SF. */
   frm   = cell->crntTime;

   RGSCH_INCR_SUB_FRAME(frm, TFU_DLCNTRL_DLDELTA);

   cellSch->dl.time = frm;
   dlSf = rgSCHUtlSubFrmGet(cell, frm);

   rgSCHPwrGrpCntrlPusch(cell, dlSf, ulSf);

   RETVOID;
}

/* Fix: syed align multiple UEs to refresh at same time */
/***********************************************************
 *
 *     Func : rgSCHCmnApplyUeRefresh 
 *
 *     Desc : Apply UE refresh in CMN and Specific 
 *     schedulers. Data rates and corresponding 
 *     scratchpad variables are updated.
 *
 *     Ret  :
 *
 *     Notes:
 *
 *     File :
 *
 **********************************************************/
#ifdef ANSI
PRIVATE S16 rgSCHCmnApplyUeRefresh 
(
RgSchCellCb     *cell,
RgSchUeCb       *ue
)
#else
PRIVATE S16 rgSCHCmnApplyUeRefresh(cell, ue)
RgSchCellCb     *cell;
RgSchUeCb       *ue;
#endif
{
   RgSchCmnCell    *cellSch     = RG_SCH_CMN_GET_CELL(cell);
   RgSchCmnUlUe    *ueUl        = RG_SCH_CMN_GET_UL_UE(ue);
   U32             effGbrBsr    = 0;
   U32             effNonGbrBsr = 0;
   U32             lcgId;

   TRC2(rgSCHCmnApplyUeRefresh);

   /* Reset the refresh cycle variableCAP */
   ueUl->effAmbr = ueUl->cfgdAmbr;

   for (lcgId = 1; lcgId < RGSCH_MAX_LCG_PER_UE; lcgId++)
   {
      if (RGSCH_LCG_ISCFGD(&ue->ul.lcgArr[lcgId]))
      {
         RgSchCmnLcg *cmnLcg = ((RgSchCmnLcg *)(ue->ul.lcgArr[lcgId].sch));

         if (RGSCH_IS_GBR_BEARER(cmnLcg->cfgdGbr))
         {
            cmnLcg->effGbr = cmnLcg->cfgdGbr;
            cmnLcg->effDeltaMbr = cmnLcg->deltaMbr;
            cmnLcg->bs = RGSCH_MIN(cmnLcg->reportedBs, cmnLcg->effGbr + cmnLcg->effDeltaMbr);
            /* Considering GBR LCG will be prioritised by UE */
            effGbrBsr += cmnLcg->bs;
         }/* Else no remaing BS so nonLcg0 will be updated when BSR will be received */
         else
         {
            effNonGbrBsr += cmnLcg->reportedBs;
            cmnLcg->bs = RGSCH_MIN(cmnLcg->reportedBs, ueUl->effAmbr);
         }
      }
   }
   effNonGbrBsr = RGSCH_MIN(effNonGbrBsr,ueUl->effAmbr);
   ueUl->nonGbrLcgBs = effNonGbrBsr;

   ueUl->nonLcg0Bs = effGbrBsr + effNonGbrBsr;
   ueUl->effBsr = ueUl->nonLcg0Bs +\
                  ((RgSchCmnLcg *)(ue->ul.lcgArr[0].sch))->bs;


   /* call scheduler specific event handlers
    * for refresh timer expiry */
   cellSch->apisUl->rgSCHUlUeRefresh(cell, ue);
   cellSch->apisDl->rgSCHDlUeRefresh(cell, ue);

   RETVALUE(ROK);
}

/***********************************************************
 *
 *     Func : rgSCHCmnTmrExpiry
 *
 *     Desc : Adds an UE to refresh queue, so that the UE is
 *            periodically triggered to refresh it's GBR and
 *            AMBR values.
 *
 *     Ret  :
 *
 *     Notes:
 *
 *     File :
 *
 **********************************************************/
#ifdef ANSI
PRIVATE S16 rgSCHCmnTmrExpiry
(
PTR cb,               /* Pointer to timer control block */
S16 tmrEvnt           /* Timer Event */
)
#else
PRIVATE S16 rgSCHCmnTmrExpiry(cb, tmrEvnt)
PTR cb;               /* Pointer to timer control block */
S16 tmrEvnt;           /* Timer Event */
#endif
{
   RgSchUeCb       *ue = (RgSchUeCb *)cb;
   RgSchCellCb     *cell = ue->cell;
#if (ERRCLASS & ERRCLS_DEBUG)
#endif

   TRC2(rgSCHCmnTmrExpiry);

#if (ERRCLASS & ERRCLS_DEBUG)
   if (tmrEvnt != RG_SCH_CMN_EVNT_UE_REFRESH)
   {
      RLOG_ARG1(L_ERROR,DBG_CELLID,cell->cellId,"rgSCHCmnTmrExpiry(): Invalid "
         "timer event CRNTI:%d",ue->ueId);
      RETVALUE(RFAILED);
   }
#else
   UNUSED(tmrEvnt);
#endif

   rgSCHCmnApplyUeRefresh(cell, ue);

   rgSCHCmnAddUeToRefreshQ(cell, ue, RG_SCH_CMN_REFRESH_TIME);

   RETVALUE(ROK);
}

/***********************************************************
 *
 *     Func : rgSCHCmnTmrProc
 *
 *     Desc : Timer entry point per cell. Timer
 *            processing is triggered at every frame boundary
 *            (every 10 ms).
 *
 *     Ret  :
 *
 *     Notes:
 *
 *     File :
 *
 **********************************************************/
#ifdef ANSI
PRIVATE S16 rgSCHCmnTmrProc
(
RgSchCellCb *cell
)
#else
PRIVATE S16 rgSCHCmnTmrProc(cell)
RgSchCellCb *cell;
#endif
{
   RgSchCmnDlCell *cmnDlCell = RG_SCH_CMN_GET_DL_CELL(cell);
   RgSchCmnUlCell *cmnUlCell = RG_SCH_CMN_GET_UL_CELL(cell);
   /* Moving the assignment of scheduler pointer
     to available scope for optimization */
   TRC2(rgSCHCmnTmrProc);

   if (cell->crntTime.subframe == 0)
   {
      /* Reset the counters periodically */
      if ((cell->crntTime.sfn % RG_SCH_CMN_CSG_REFRESH_TIME) == 0)
      {
         RG_SCH_RESET_HCSG_DL_PRB_CNTR(cmnDlCell);
         RG_SCH_RESET_HCSG_UL_PRB_CNTR(cmnUlCell);
      }
      if ((cell->crntTime.sfn % RG_SCH_CMN_OVRLDCTRL_REFRESH_TIME) == 0)
      {

         cell->measurements.ulTpt =  ((cell->measurements.ulTpt * 95) + ( cell->measurements.ulBytesCnt * 5))/100;
         cell->measurements.dlTpt =  ((cell->measurements.dlTpt * 95) + ( cell->measurements.dlBytesCnt * 5))/100;

         rgSCHUtlCpuOvrLdAdjItbsCap(cell);
         /* reset cell level tpt measurements for next cycle */
         cell->measurements.ulBytesCnt = 0;
         cell->measurements.dlBytesCnt = 0;
      }
      /* Comparing with Zero instead of % is being done for efficiency.
       * If Timer resolution changes then accordingly update the
       * macro RG_SCH_CMN_REFRESH_TIMERES */    
      RgSchCmnCell   *sched  = RG_SCH_CMN_GET_CELL(cell);
      cmPrcTmr(&sched->tmrTqCp, sched->tmrTq, (PFV)rgSCHCmnTmrExpiry);
   }

   RETVALUE(ROK);
}


/***********************************************************
 *
 *     Func : rgSchCmnUpdCfiVal 
 *
 *     Desc : Update the CFI value if CFI switch was done 
 *
 *     Ret  :
 *
 *     Notes:
 *
 *     File :
 *
 **********************************************************/
#ifdef ANSI
PRIVATE Void rgSchCmnUpdCfiVal
(
RgSchCellCb     *cell,
U8              delta
)
#else
PRIVATE Void rgSchCmnUpdCfiVal(cell, delta)
RgSchCellCb     *cell;
U8              delta;
#endif  
{
   RgSchDlSf        *dlSf;
   CmLteTimingInfo  pdsch;
   RgSchCmnDlCell  *cellCmnDl = RG_SCH_CMN_GET_DL_CELL(cell); 
   U8               dlIdx;
#ifdef LTE_TDD
   U8               mPhich;
   RgSchDlSf        *tddSf;
   U8               idx;
   U8               splSfCfi = 0;
#endif    

   TRC2(rgSchCmnUpdCfiVal);

   pdsch  = cell->crntTime;
   RGSCH_INCR_SUB_FRAME(pdsch, delta);
   dlSf = rgSCHUtlSubFrmGet(cell, pdsch);
   /* Fix for DCFI FLE issue: when DL delta is 1 and UL delta is 0 and CFI
    *change happens in that SF then UL PDCCH allocation happens with old CFI
    *but CFI in control Req goes updated one since it was stored in the CELL
    */
   dlSf->pdcchInfo.currCfi = cellCmnDl->currCfi;
   if(cell->dynCfiCb.pdcchSfIdx != 0xFF) 
   {
#ifdef LTE_TDD
      dlIdx = rgSCHUtlGetDlSfIdx(cell, &pdsch);
#else
      dlIdx = (((pdsch.sfn & 1) * RGSCH_NUM_SUB_FRAMES) + pdsch.subframe);
      RGSCH_ARRAY_BOUND_CHECK(cell->instIdx, cell->subFrms, dlIdx);
#endif  
      /* If current downlink subframe index is same as pdcch SF index,
       * perform the switching of CFI in this subframe */
      if(cell->dynCfiCb.pdcchSfIdx == dlIdx)
      {
         cellCmnDl->currCfi  = cellCmnDl->newCfi;
         cell->dynCfiCb.pdcchSfIdx = 0xFF;

         /* Updating the nCce value based on the new CFI */
#ifdef LTE_TDD
         splSfCfi = cellCmnDl->newCfi;
         for(idx = 0; idx < cell->numDlSubfrms; idx++)
         {   
            tddSf = cell->subFrms[idx];

            mPhich = rgSchTddPhichMValTbl[cell->ulDlCfgIdx][tddSf->sfNum];

            if(tddSf->sfType == RG_SCH_SPL_SF_DATA)
            {
               RGSCH_GET_SPS_SF_CFI(cell->bwCfg.dlTotalBw, splSfCfi);

               tddSf->nCce = cell->dynCfiCb.cfi2NCceTbl[mPhich][splSfCfi];
            }
            else
            {   
               tddSf->nCce = cell->dynCfiCb.cfi2NCceTbl[mPhich][cellCmnDl->currCfi];
            }
         }
         /* Setting the switch over window length based on config index.
          * During switch over period all the UL trnsmissions are Acked 
          * to UEs */
         cell->dynCfiCb.switchOvrWinLen = 
               rgSchCfiSwitchOvrWinLen[cell->ulDlCfgIdx];
#else
         cell->nCce = cell->dynCfiCb.cfi2NCceTbl[0][cellCmnDl->currCfi];
         /* Fix for DCFI FLE issue: when DL delta is 1 and UL delta is 0 and CFI
          *change happens in that SF then UL PDCCH allocation happens with old CFI
          *but CFI in control Req goes updated one since it was stored in the CELL
          */
         dlSf->pdcchInfo.currCfi = cellCmnDl->currCfi;
         cell->dynCfiCb.switchOvrWinLen = rgSchCfiSwitchOvrWinLen[7];
#endif
      }   
   }   

   RETVOID;
}

/***********************************************************
 *
 *     Func : rgSchCmnUpdtPdcchSfIdx 
 *
 *     Desc : Update the switch over window length
 *
 *     Ret  : void
 *
 *     Notes:
 *
 *     File :
 *
 **********************************************************/
#ifdef LTE_TDD
#ifdef ANSI
PRIVATE Void rgSchCmnUpdtPdcchSfIdx 
(
RgSchCellCb     *cell,
U8              dlIdx,
U8              sfNum
)
#else
PRIVATE Void rgSchCmnUpdtPdcchSfIdx(cell, dlIdx, sfNum)
RgSchCellCb     *cell;
U8              dlIdx;
U8              sfNum;
#endif   
#else
#ifdef ANSI
PRIVATE Void rgSchCmnUpdtPdcchSfIdx 
(
RgSchCellCb     *cell,
U8              dlIdx
)
#else
PRIVATE Void rgSchCmnUpdtPdcchSfIdx(cell, dlIdx)
RgSchCellCb     *cell;
U8              dlIdx;
#endif    
#endif
{
   U8         idx;

   TRC2(rgSchCmnUpdtPdcchSfIdx);

   /* Resetting the parameters on CFI switching */
   cell->dynCfiCb.cceUsed = 0;
   cell->dynCfiCb.lowCceCnt = 0;

   cell->dynCfiCb.cceFailSum = 0;
   cell->dynCfiCb.cceFailCnt = 0;
   cell->dynCfiCb.prevCceFailIdx = 0;

   cell->dynCfiCb.switchOvrInProgress = TRUE;

   for(idx = 0; idx < cell->dynCfiCb.numFailSamples; idx++)
   {
      cell->dynCfiCb.cceFailSamples[idx] = 0;
   }   

   cell->dynCfiCb.ttiCnt = 0;

   cell->dynCfiCb.cfiSwitches++;
   cfiSwitchCnt = cell->dynCfiCb.cfiSwitches;

#ifdef LTE_TDD 
   cell->dynCfiCb.pdcchSfIdx = (dlIdx + 
      rgSchTddPdcchSfIncTbl[cell->ulDlCfgIdx][sfNum]) % cell->numDlSubfrms;
#else
   cell->dynCfiCb.pdcchSfIdx = (dlIdx + RG_SCH_CFI_APPLY_DELTA) % \
        RGSCH_NUM_DL_SUBFRAMES;
#endif
}

/***********************************************************
 *
 *     Func : rgSchCmnUpdCfiDb 
 *
 *     Desc : Update the counters related to dynamic
 *            CFI feature in cellCb. 
 *
 *     Ret  :
 *
 *     Notes:
 *
 *     File :
 *
 **********************************************************/
#ifdef ANSI
PUBLIC Void rgSchCmnUpdCfiDb 
(
RgSchCellCb     *cell,
U8              delta 
)
#else
PUBLIC Void rgSchCmnUpdCfiDb(cell, delta)
RgSchCellCb     *cell;
U8              delta;
#endif 
{
   CmLteTimingInfo        frm;
   RgSchDlSf              *dlSf;
#ifdef LTE_TDD
   U8                     mPhich;
   Bool                   isHiDci0; 
#endif      
   RgSchCmnCell           *cellSch = RG_SCH_CMN_GET_CELL(cell); 
   U8                     nCceLowerCfi = 0;
   U8                     currCfi;
   U8                     cceFailIdx;
   U32                    totalCce;
   U8                     dlIdx;
   U16                    ttiMod;

   TRC2(rgSchCmnUpdCfiDb);

   /* Get Downlink Subframe */   
   frm   = cell->crntTime;
   RGSCH_INCR_SUB_FRAME(frm, delta);

#ifdef LTE_TDD
   dlIdx = rgSCHUtlGetDlSfIdx(cell, &frm);
   dlSf = cell->subFrms[dlIdx];
   isHiDci0 = rgSchTddPuschTxKTbl[cell->ulDlCfgIdx][dlSf->sfNum];
#else
   /* Changing the idexing
      so that proper subframe is selected */
   dlIdx = (((frm.sfn & 1) * RGSCH_NUM_SUB_FRAMES) + frm.subframe);
   RGSCH_ARRAY_BOUND_CHECK(cell->instIdx, cell->subFrms, dlIdx);
   dlSf = cell->subFrms[dlIdx];
#endif 

   currCfi = cellSch->dl.currCfi;

   if(!cell->dynCfiCb.switchOvrInProgress)
   {   
      do{
         if(!cell->dynCfiCb.isDynCfiEnb)
         {
            if(currCfi != cellSch->cfiCfg.cfi)
            {
               if(currCfi < cellSch->cfiCfg.cfi)
               {
                  RG_SCH_CFI_STEP_UP(cell, cellSch, currCfi)
                  cfiIncr = cell->dynCfiCb.cfiIncr;   
               }
               else
               {
                  RG_SCH_CFI_STEP_DOWN(cell, cellSch, currCfi)
                  cfiDecr = cell->dynCfiCb.cfiDecr;
               }
            }
            break;
         }

#ifdef LTE_TDD         
         /* Setting ttiMod to 0 for ttiCnt > 1000 in case if this 
          * function was not called in UL subframe*/
         if(cell->dynCfiCb.ttiCnt > RGSCH_CFI_TTI_MON_INTRVL)
         {   
            ttiMod = 0;
         }
         else
#endif
         {   
            ttiMod = cell->dynCfiCb.ttiCnt % RGSCH_CFI_TTI_MON_INTRVL;
         }

         dlSf->dlUlBothCmplt++;
#ifdef LTE_TDD      
         if((dlSf->dlUlBothCmplt == 2) || (!isHiDci0))
#else
         if(dlSf->dlUlBothCmplt == 2)
#endif         
         {
            /********************STEP UP CRITERIA********************/
            /* Updating the CCE failure count parameter */
            cell->dynCfiCb.cceFailCnt += dlSf->isCceFailure;
            cell->dynCfiCb.cceFailSum += dlSf->isCceFailure;

            /* Check if cfi step up can be performed */
            if(currCfi < cell->dynCfiCb.maxCfi)
            {
               if(cell->dynCfiCb.cceFailSum >= cell->dynCfiCb.cfiStepUpTtiCnt) 
               {
                  RG_SCH_CFI_STEP_UP(cell, cellSch, currCfi)
                  cfiIncr = cell->dynCfiCb.cfiIncr;   
                  break;
               }
            } 

            /********************STEP DOWN CRITERIA********************/

            /* Updating the no. of CCE used in this dl subframe */
            cell->dynCfiCb.cceUsed += dlSf->cceCnt;

            if(currCfi > RGSCH_MIN_CFI_VAL)
            {   
               /* calculating the number of CCE for next lower CFI */
#ifdef LTE_TDD      
               mPhich = rgSchTddPhichMValTbl[cell->ulDlCfgIdx][dlSf->sfNum];
               nCceLowerCfi = cell->dynCfiCb.cfi2NCceTbl[mPhich][currCfi-1];
#else
               nCceLowerCfi = cell->dynCfiCb.cfi2NCceTbl[0][currCfi-1];
#endif     
               if(dlSf->cceCnt < nCceLowerCfi)
               {
                  /* Updating the count of TTIs in which no. of CCEs
                   * used were less than the CCEs of next lower CFI */
                  cell->dynCfiCb.lowCceCnt++;
               }   

               if(ttiMod == 0)
               {
                  totalCce = (nCceLowerCfi * cell->dynCfiCb.cfiStepDownTtiCnt * 
                        RGSCH_CFI_CCE_PERCNTG)/100;

                  if((!cell->dynCfiCb.cceFailSum) && 
                        (cell->dynCfiCb.lowCceCnt >= 
                         cell->dynCfiCb.cfiStepDownTtiCnt) && 
                        (cell->dynCfiCb.cceUsed < totalCce))  
                  {
                     RG_SCH_CFI_STEP_DOWN(cell, cellSch, currCfi)
                     cfiDecr = cell->dynCfiCb.cfiDecr; 
                     break;
                  }
               }   
            }

            cceFailIdx = ttiMod/cell->dynCfiCb.failSamplePrd;

            if(cceFailIdx != cell->dynCfiCb.prevCceFailIdx)
            {   
               /* New sample period has started. Subtract the old count  
                * from the new sample period */
               cell->dynCfiCb.cceFailSum -= cell->dynCfiCb.cceFailSamples[cceFailIdx];

               /* Store the previous sample period data */
               cell->dynCfiCb.cceFailSamples[cell->dynCfiCb.prevCceFailIdx]
                  = cell->dynCfiCb.cceFailCnt;

               cell->dynCfiCb.prevCceFailIdx = cceFailIdx;

               /* Resetting the CCE failure count as zero for next sample period */
               cell->dynCfiCb.cceFailCnt = 0;  
            }

            if(ttiMod == 0)
            {   
               /* Restting the parametrs after Monitoring Interval expired */
               cell->dynCfiCb.cceUsed = 0;
               cell->dynCfiCb.lowCceCnt = 0;
               cell->dynCfiCb.ttiCnt = 0;
            }

            cell->dynCfiCb.ttiCnt++;
         }
      }while(0);

      if(cellSch->dl.newCfi != cellSch->dl.currCfi)
      {
#ifdef LTE_TDD      
         rgSchCmnUpdtPdcchSfIdx(cell, dlIdx, dlSf->sfNum);
#else
         rgSchCmnUpdtPdcchSfIdx(cell, dlIdx);
#endif      
      }  
   }
}   

/**
 * @brief Scheduler invocation per TTI.
 *
 * @details
 *
 *     Function: rgSCHCmnTti
 *     Purpose:  This function implements a scheduler for LTE. This is
 *               made available as a function pointer to be called
 *               at the time of TTI processing by the MAC.
 *               Common channel, RAR and Msg4 in case of DL and
 *               Retx in case of UL are considered
 *               here for scheduling. specific DL and UL schedulers
 *               are inturn invoked to perform scheduling of UEs
 *               dedicated channel data.
 *
 *     Invoked by: TOM (TTI processing)
 *
 *  @param[in]  RgSchCellCb *cell
 *  @param[out] RgSchErrInfo *err
 *  @return  S16
 *      -# ROK
 *      -# RFAILED
 **/
#ifdef ANSI
PUBLIC S16 rgSCHCmnTti
(
RgSchCellCb  *cell,
RgSchErrInfo *err
)
#else
PUBLIC S16 rgSCHCmnTti(cell, err)
RgSchCellCb  *cell;
RgSchErrInfo *err;
#endif
{
#ifdef LTE_TDD
   Bool           bcchPcchSched = FALSE;
   Bool           othrDlSched = FALSE;
   Bool           isDlDataAllowed;
#else

   /* LTE_ADV_FLAG_REMOVED_START */
   S16                 retVal;
   RgrLoadInfIndInfo   *rgrLoadInf;
#endif
   RgSchCmnCell   *cellSch = RG_SCH_CMN_GET_CELL(cell);
   /* LTE_ADV_FLAG_REMOVED_END */
   U8             idx;

   VOLATILE U32     startTime=0;
   
   TRC2(rgSCHCmnTti);

   /*starting Task*/
   SStartTask(&startTime, PID_L2_PROCESSING);

   UNUSED(err);

   cellSch->apisDl->rgSCHDlTickForPdbTrkng(cell);
   rgSchCmnUpdCfiVal(cell, RG_SCH_CMN_DL_DELTA);

   if(!cell->dlBlankSf)
   {
     /* handle Inactive UEs for DL */
     rgSCHCmnHdlDlInactUes(cell);

     /* Send a Tick to Refresh Timer */
     rgSCHCmnTmrProc(cell);
   }
#ifdef LTE_TDD
   /* Special subframes are not used for DL scheduling */

   /*Calculating idx for BCCH,PCCH scheduling */
   idx = (cell->crntTime.subframe + RG_SCH_CMN_DL_DELTA) % RGSCH_NUM_SUB_FRAMES;

   isDlDataAllowed = RG_SCH_CMN_CHK_DL_DATA_ALLOWED(cell, idx); 

   /* Donot perform any downlink scheduling.In case of TDD, this flag will be set by CL only in uplink 
    * subframe. So this actually not required. However, in case the algorithm changes later, this will
    * take care of that  
    */
   if(cell->dlBlankSf)
   {
     isDlDataAllowed = 0;
   }  

   /*Checking if BCCH,PCCH Data can be scheduled*/
   /* DwPTS Scheduling Changes Start */
   if(isDlDataAllowed)
   /* DwPTS Scheduling Changes End */   
   {
      /* Perform DL Scheduling of BCCH,PCCH EVERY TTI */
      rgSCHCmnDlBcchPcchAlloc(cell);
      bcchPcchSched = TRUE;
   }
   else
   {
      if(cell->siCb.inWindow != 0)
      {
         cell->siCb.inWindow--;
      }
   }

   /*Checking if Rest of the data, except BCCH,PCCH can be scheduled*/
   /* DwPTS Scheduling Changes Start */
   if(isDlDataAllowed)
   /* DwPTS Scheduling Changes End */   
   {
      /* Perform DL Scheduling EVERY TTI */
      if (cell->isDummyTti != TRUE)
      {
         rgSCHCmnDlAlloc(cell);
         othrDlSched = TRUE;
      }
   }

   /* ccpu00132653 Adde SCHED_AT_CRC support for TDD and reduced 
    * redundant code across TDD and FDD*/
#ifndef RG_ULSCHED_AT_CRC
   if(cellSch->ul.schdIdx != RGSCH_INVALID_INFO)
   {
      rgSCHCmnUlTti(cell, err);
   }
#ifdef LTEMAC_SPS
   else
   {
      rgSCHCmnSpsUlTti(cell, NULLP);
   }
#endif /* LTEMAC_SPS */
#endif /* RG_ULSCHED_AT_CRC */
   /* Consolidate the allocations done sofar and send it to
    * MAC instance */
   /* DwPTS Scheduling Changes Start */
   if(isDlDataAllowed)
   /* DwPTS Scheduling Changes End */   
   {
      rgSCHCmnCnsldtSfAlloc(cell);
   }
   /*If either of BCCH-PCCH or any other DATA (RACH, Ded)
     has been scheduled for DL, then increment the crntSfIdx.*/
   if(bcchPcchSched || othrDlSched)
   {
      ++cell->crntSfIdx;
      cell->crntSfIdx  = (cell->crntSfIdx) % RGSCH_SF_ALLOC_SIZE;
   }
#else
    /* 4UE_TTI_DELTA */
   if(cell->dlBlankSf)
   {
      RETVALUE(ROK);
   }
   /* 4UE_TTI_DELTA */

  /* Perform DL Scheduling of BCCH,PCCH EVERY TTI */
   rgSCHCmnDlBcchPcchAlloc(cell);
   /* Perform DL Scheduling EVERY TTI */
   if (cell->isDummyTti != TRUE)
   {
      rgSCHCmnDlAlloc(cell);
   }
   /*stoping Task*/
   SStopTask(startTime, PID_L2_PROCESSING);
   /* Consolidate the allocations done sofar and send it to
    * MAC instance */
   rgSCHCmnCnsldtSfAlloc(cell);
   /*Increment the Current SF Index */
   cell->crntSfIdx  = (cell->crntSfIdx + 1) % RGSCH_NUM_SUB_FRAMES;

   /* LTE_ADV_FLAG_REMOVED_START */
   /* Report ABS Load information to application periodically */
   if((RGR_ENABLE == cell->lteAdvCb.absCfg.status) &&
                    (cell->lteAdvCb.absCfg.absLoadPeriodicity))
   {
      cell->lteAdvCb.absLoadTtiCnt++;
      if(cell->lteAdvCb.absLoadTtiCnt >= cell->lteAdvCb.absCfg.absLoadPeriodicity)
      {
      
         /* ccpu00134492 */
         retVal = rgSCHUtlAllocSBuf (cell->instIdx,(Data**)&rgrLoadInf,
               sizeof(RgrLoadInfIndInfo));
         if (retVal != ROK)
         {
            RLOG_ARG0(L_ERROR,DBG_CELLID,cell->cellId,"Could not "
                     "allocate memory for sending LoadInfo");
            RETVALUE(retVal);
         }
         cell->lteAdvCb.absLoadTtiCnt = 0;
         rgrLoadInf->cellId = cell->cellId;
         rgrLoadInf->bw     = cell->bwCfg.dlTotalBw;
         rgrLoadInf->type   = RGR_ABS;
         for(idx= 0; idx<RGR_ABS_PATTERN_LEN; idx++)
         {
            rgrLoadInf->u.absLoadInfo[idx] = cell->lteAdvCb.absLoadInfo[idx];
            cell->lteAdvCb.absLoadInfo[idx] = 0;
         }
         rgSCHUtlRgrLoadInfInd(cell, rgrLoadInf);
      }
   }
   /* LTE_ADV_FLAG_REMOVED_END */
#endif
#ifdef LTE_L2_MEAS
   rgSCHL2Meas(cell,FALSE);
#endif /* LTE_L2_MEAS */

#ifdef LTE_TDD   
   if(isDlDataAllowed)
   {   
      /* Calling function to update CFI parameters*/
      rgSchCmnUpdCfiDb(cell, RG_SCH_CMN_DL_DELTA);   
   }
   else
   {
      /* Incrementing the ttiCnt in case of UL subframe */
      if(!cell->dynCfiCb.switchOvrInProgress)
      {   
         cell->dynCfiCb.ttiCnt++;
      }
   }   
#else
   rgSchCmnUpdCfiDb(cell, RG_SCH_CMN_DL_DELTA);   
#endif   

   RETVALUE(ROK);
}

/**
 * @brief Scheduler invocation per TTI.
 *
 * @details
 *
 *     Function: rgSCHCmnUlTti
 *     Purpose:  This function implements UL scheduler alone. This is to
 *               be able to perform scheduling with more flexibility.
 *
 *     Invoked by: TOM (TTI processing)
 *
 *  @param[in]  RgSchCellCb *cell
 *  @param[out] RgSchErrInfo *err
 *  @return  S16
 *      -# ROK
 *      -# RFAILED
 **/
#ifdef ANSI
PUBLIC S16 rgSCHCmnUlTti
(
RgSchCellCb  *cell,
RgSchErrInfo *err
)
#else
PUBLIC S16 rgSCHCmnUlTti(cell, err)
RgSchCellCb  *cell;
RgSchErrInfo *err;
#endif
{

   TRC2(rgSCHCmnUlTti);
   UNUSED(err);

   rgSchCmnUpdCfiVal(cell, TFU_ULCNTRL_DLDELTA);

   /* Handle Inactive UEs for UL */
   rgSCHCmnHdlUlInactUes(cell);
   /* Perform UL Scheduling EVERY TTI */
   rgSCHCmnUlAlloc(cell);

   /* Calling function to update CFI parameters*/
   rgSchCmnUpdCfiDb(cell, TFU_ULCNTRL_DLDELTA);   

   if(cell->dynCfiCb.switchOvrWinLen > 0)
   {
      /* Decrementing the switchover window length */
      cell->dynCfiCb.switchOvrWinLen--;

      if(!cell->dynCfiCb.switchOvrWinLen)
      {   
         if(cell->dynCfiCb.dynCfiRecfgPend)
         {  
            /* Toggling the Dynamic CFI enabling */
            cell->dynCfiCb.isDynCfiEnb ^= 1;
            rgSCHDynCfiReCfg(cell, cell->dynCfiCb.isDynCfiEnb); 
            cell->dynCfiCb.dynCfiRecfgPend = FALSE;
         }   
         cell->dynCfiCb.switchOvrInProgress = FALSE;
      }
   } 

   RETVALUE(ROK);
}


/**
 * @brief This function updates the scheduler with service for an UE.
 *
 * @details
 *
 *     Function: rgSCHCmnDlDedBoUpd
 *     Purpose:  This function should be called whenever there is a
 *               change BO for a service.
 *
 *     Invoked by: BO and Scheduler
 *
 *  @param[in]  RgSchCellCb*  cell
 *  @param[in]  RgSchUeCb*    ue
 *  @param[in]  RgSchDlLcCb*  svc
 *  @return  Void
 *
 **/
#ifdef ANSI
PUBLIC Void rgSCHCmnDlDedBoUpd
(
RgSchCellCb                *cell,
RgSchUeCb                  *ue,
RgSchDlLcCb                *svc
)
#else
PUBLIC Void rgSCHCmnDlDedBoUpd(cell, ue, svc)
RgSchCellCb                *cell;
RgSchUeCb                  *ue;
RgSchDlLcCb                *svc;
#endif
{
   RgSchCmnCell *cellSch = RG_SCH_CMN_GET_CELL(cell);
   TRC2(rgSCHCmnDlDedBoUpd);

   /* RACHO : if UEs idle time exceeded and a BO update
    * is received, then add UE to the pdcch Order Q */
   if (RG_SCH_CMN_IS_UE_PDCCHODR_INACTV(ue))
   {
      RgSchCmnDlUe  *ueDl = RG_SCH_CMN_GET_DL_UE(ue);
      /* If PDCCH order is already triggered and we are waiting for
       * RACH from UE then do not add to PdcchOdrQ. */
      if (ueDl->rachInfo.rapIdLnk.node == NULLP)
      {
         rgSCHCmnDlAdd2PdcchOdrQ(cell, ue);
      }
   }

#ifdef LTEMAC_SPS

   /* If SPS service, invoke SPS module */
   if (svc->dlLcSpsCfg.isSpsEnabled)
   {
      rgSCHCmnSpsDlDedBoUpd(cell, ue, svc);
      /* Note: Retrun from here, no update needed in other schedulers */
      RETVOID;
   }
#endif
   cellSch->apisDl->rgSCHDlDedBoUpd(cell, ue, svc);
   RETVOID;
}


/**
 * @brief Removes an UE from Cell's TA List.
 *
 * @details
 *
 *     Function: rgSCHCmnRmvFrmTaLst
 *     Purpose:  Removes an UE from Cell's TA List.
 *
 *     Invoked by: Specific Scheduler
 *
 *  @param[in]  RgSchCellCb*     cell
 *  @param[in]  RgSchUeCb*       ue
 *  @return  Void
 *
 **/
#ifdef ANSI
PUBLIC Void rgSCHCmnRmvFrmTaLst
(
RgSchCellCb                *cell,
RgSchUeCb                  *ue
)
#else
PUBLIC Void rgSCHCmnRmvFrmTaLst(cell, ue)
RgSchCellCb                *cell;
RgSchUeCb                  *ue;
#endif
{
   RgSchCmnDlCell *cellCmnDl = RG_SCH_CMN_GET_DL_CELL(cell);
   TRC2(rgSCHCmnRmvFrmTaLst);

   cmLListDelFrm(&cellCmnDl->taLst, &ue->dlTaLnk);
   ue->dlTaLnk.node = (PTR)NULLP;

   RETVOID;
}

/* Fix: syed Remove the msg4Proc from cell
 * msg4Retx Queue. I have used CMN scheduler function
 * directly. Please define a new API and call this
 * function through that. */        

/**
 * @brief This function removes MSG4 HARQ process from cell RETX Queues.
 *
 * @details
 *
 *     Function: rgSCHCmnDlMsg4ProcRmvFrmRetx
 *     Purpose:  This function removes MSG4 HARQ process from cell RETX Queues.
 *
 *     Invoked by: UE/RACB deletion. 
 *
 *  @param[in]  RgSchCellCb*     cell
 *  @param[in]  RgSchDlHqProc*   hqP
 *  @return  Void
 *
 **/
#ifdef ANSI
PUBLIC Void rgSCHCmnDlMsg4ProcRmvFrmRetx 
(
RgSchCellCb                *cell,
RgSchDlHqProcCb            *hqP
)
#else
PUBLIC Void rgSCHCmnDlMsg4ProcRmvFrmRetx(cell, hqP)
RgSchCellCb                *cell;
RgSchDlHqProcCb            *hqP;
#endif
{
   RgSchCmnCell *cellSch = RG_SCH_CMN_GET_CELL(cell);
   TRC2(rgSCHCmnDlMsg4ProcRmvFrmRetx);

   if (hqP->tbInfo[0].ccchSchdInfo.retxLnk.node)
   {
      if (hqP->hqE->msg4Proc == hqP)
      {
         cmLListDelFrm(&cellSch->dl.msg4RetxLst, \
               &hqP->tbInfo[0].ccchSchdInfo.retxLnk);
         hqP->tbInfo[0].ccchSchdInfo.retxLnk.node = (PTR)NULLP;
      }
#ifdef RGR_V1
      else if(hqP->hqE->ccchSduProc == hqP)
      {
         cmLListDelFrm(&cellSch->dl.ccchSduRetxLst,
               &hqP->tbInfo[0].ccchSchdInfo.retxLnk);
         hqP->tbInfo[0].ccchSchdInfo.retxLnk.node = (PTR)NULLP;
      }
#endif
   }
   RETVOID;
}


/**
 * @brief This function adds a HARQ process for retx.
 *
 * @details
 *
 *     Function: rgSCHCmnDlProcAddToRetx
 *     Purpose:  This function adds a HARQ process to retransmission
 *               queue. This may be performed when a HARQ ack is
 *               unsuccessful.
 *
 *     Invoked by: HARQ feedback processing
 *
 *  @param[in]  RgSchCellCb*     cell
 *  @param[in]  RgSchDlHqProc*   hqP
 *  @return  Void
 *
 **/
#ifdef ANSI
PUBLIC Void rgSCHCmnDlProcAddToRetx
(
RgSchCellCb                *cell,
RgSchDlHqProcCb            *hqP
)
#else
PUBLIC Void rgSCHCmnDlProcAddToRetx(cell, hqP)
RgSchCellCb                *cell;
RgSchDlHqProcCb            *hqP;
#endif
{
   RgSchCmnCell *cellSch = RG_SCH_CMN_GET_CELL(cell);
   TRC2(rgSCHCmnDlProcAddToRetx);

   if (hqP->hqE->msg4Proc == hqP) /* indicating msg4 transmission */
   {
      cmLListAdd2Tail(&cellSch->dl.msg4RetxLst, \
            &hqP->tbInfo[0].ccchSchdInfo.retxLnk);
      hqP->tbInfo[0].ccchSchdInfo.retxLnk.node = (PTR)hqP;
   }
#ifdef RGR_V1
   else if(hqP->hqE->ccchSduProc == hqP)
   {
      /*If CCCH SDU being transmitted without cont res CE*/
      cmLListAdd2Tail(&cellSch->dl.ccchSduRetxLst,
            &hqP->tbInfo[0].ccchSchdInfo.retxLnk);
      hqP->tbInfo[0].ccchSchdInfo.retxLnk.node = (PTR)hqP;
   }
#endif
   else
   {
#ifdef LTEMAC_SPS
      if (RG_SCH_CMN_SPS_DL_IS_SPS_HQP(hqP))
      {
         /* Invoke SPS module for SPS HARQ proc re-transmission handling */
         rgSCHCmnSpsDlProcAddToRetx(cell, hqP);
         RETVOID;
      }
#endif /* LTEMAC_SPS */
      cellSch->apisDl->rgSCHDlProcAddToRetx(cell, hqP);
   }
   RETVOID;
}


/**
 * @brief This function performs RI validation and
 *        updates it to the ueCb.
 *
 * @details
 *
 *     Function: rgSCHCmnDlSetUeRi
 *     Purpose:  This function performs RI validation and
 *        updates it to the ueCb.
 *
 *     Invoked by: rgSCHCmnDlCqiInd
 *
 *  @param[in]  RgSchCellCb        *cell
 *  @param[in]  RgSchUeCb          *ue
 *  @param[in]  U8                 ri
 *  @param[in]  Bool               isPeriodic
 *  @return  Void
 *
 **/
#ifdef ANSI
PRIVATE Void rgSCHCmnDlSetUeRi
(
RgSchCellCb        *cell,
RgSchUeCb          *ue,
U8                 ri,
Bool               isPer
)
#else
PRIVATE Void rgSCHCmnDlSetUeRi(cell, ue, ri, isPer)
RgSchCellCb        *cell;
RgSchUeCb          *ue;
U8                 ri;
Bool               isPer;
#endif
{
   RgSchCmnDlUe  *ueDl = RG_SCH_CMN_GET_DL_UE(ue);
   RgSchCmnUe    *ueSch = RG_SCH_CMN_GET_UE(ue);
   TRC2(rgSCHCmnDlSetUeRi);
   
#ifdef TFU_UPGRADE
   UNUSED(isPer);
#endif


   /* FIX for RRC Reconfiguration issue */
   /* ccpu00140894- During Tx Mode transition RI report will not entertained for 
    * specific during which SCH expecting UE can complete TX mode transition*/
   if (ue->txModeTransCmplt == FALSE)
   {
      RETVOID;
   }

   /* Restrict the Number of TX layers to cell->numTxAntPorts.
    * Protection from invalid RI values. */
   ri = RGSCH_MIN(ri, cell->numTxAntPorts);
   
   /* Special case of converting PMI to sane value when
    * there is a switch in RI from 1 to 2 and PMI reported 
    * for RI=1 is invalid for RI=2 */
   if ((cell->numTxAntPorts == 2) && (ue->mimoInfo.txMode == RGR_UE_TM_4))
   {
      if ((ri == 2) && ( ueDl->mimoInfo.ri == 1))
      {
         ueDl->mimoInfo.pmi = (ueDl->mimoInfo.pmi < 2)? 1:2;
      }
   }

   /* Restrict the Number of TX layers according to the UE Category */
   ueDl->mimoInfo.ri = RGSCH_MIN(ri, rgUeCatTbl[ueSch->cmn.ueCat].maxTxLyrs);

#ifdef TENB_STATS
   ue->tenbStats->stats.sch.riCnt[ueDl->mimoInfo.ri-1]++;
   cell->tenbStats->sch.riCnt[ueDl->mimoInfo.ri-1]++;
#endif

#ifdef TFU_UPGRADE
   if (isPer)
   {
      /* If RI is from Periodic CQI report */
      ue->cqiCb.perRiVal = ueDl->mimoInfo.ri;
      /* Reset at every Periodic RI Reception */ 
      ue->cqiCb.invalidateCqi = FALSE;
   }
   else
   {
      /* If RI is from Aperiodic CQI report */
      if (ue->cqiCb.perRiVal != ueDl->mimoInfo.ri)
      {
         /* if this aperRI is different from last reported
          * perRI then invalidate all CQI reports till next
          * perRI */
         ue->cqiCb.invalidateCqi = TRUE;
      }
      else
      {
         ue->cqiCb.invalidateCqi = FALSE;
      }
   }
#endif   

   if (ueDl->mimoInfo.ri > 1)
   {
      RG_SCH_CMN_UNSET_FORCE_TD(ue, RG_SCH_CMN_TD_RI_1);
   }
   else if (ue->mimoInfo.txMode == RGR_UE_TM_3) /* ri == 1 */
   {
      RG_SCH_CMN_SET_FORCE_TD(ue, RG_SCH_CMN_TD_RI_1);
   }

   RETVOID;
}


/**
 * @brief This function performs PMI validation and
 *        updates it to the ueCb.
 *
 * @details
 *
 *     Function: rgSCHCmnDlSetUePmi
 *     Purpose:  This function performs PMI validation and
 *        updates it to the ueCb.
 *
 *     Invoked by: rgSCHCmnDlCqiInd
 *
 *  @param[in]  RgSchCellCb        *cell
 *  @param[in]  RgSchUeCb          *ue
 *  @param[in]  U8                 pmi
 *  @return  Void
 *
 **/
#ifdef ANSI
PRIVATE S16 rgSCHCmnDlSetUePmi
(
RgSchCellCb        *cell,
RgSchUeCb          *ue,
U8                 pmi
)
#else
PRIVATE S16 rgSCHCmnDlSetUePmi(cell, ue, pmi)
RgSchCellCb        *cell;
RgSchUeCb          *ue;
U8                 pmi;
#endif
{
   RgSchCmnDlUe  *ueDl = RG_SCH_CMN_GET_DL_UE(ue);
   TRC2(rgSCHCmnDlSetUePmi);

   if(FALSE == ue->txModeTransCmplt)
   {
      RETVALUE(ROK);
   }

   if (cell->numTxAntPorts == 2)
   {
      if (pmi > 3)
      {
         RETVALUE(RFAILED);
      }
      if (ueDl->mimoInfo.ri == 2)
      {
         /*ccpu00118150 - MOD - changed pmi value validation from 0 to 2*/
         /* PMI 2 and 3 are invalid incase of 2 TxAnt and 2 Layered SM */
         if (pmi == 2 || pmi == 3)
         {
            RETVALUE(RFAILED);
         }
         ueDl->mimoInfo.pmi = pmi+1;
      }
      else
      {
         ueDl->mimoInfo.pmi = pmi;
      }
   }
   else if (cell->numTxAntPorts == 4)
   {
      if (pmi > 15)
      {
         RETVALUE(RFAILED);
      }
      ueDl->mimoInfo.pmi = pmi;
   }
   /* Reset the No PMI Flag in forceTD */
   RG_SCH_CMN_UNSET_FORCE_TD(ue, RG_SCH_CMN_TD_NO_PMI);
   RETVALUE(ROK);
}

/**
 * @brief This function Updates the DL CQI on PUCCH for the UE.
 *
 * @details
 *
 *     Function: rgSCHCmnDlProcCqiMode10
 *
 *     This function updates the DL CQI on PUCCH for the UE.
 *
 *     Invoked by: rgSCHCmnDlCqiOnPucchInd
 *
 *     Processing Steps:
 *
 *  @param[in] RgSchCellCb     *cell
 *  @param[in] RgSchUeCb       *ue
 *  @param[in] TfuDlCqiRpt     *dlCqiRpt
 *  @return  S16
 *      -# ROK
 *      -# RFAILED
 **/
#ifdef RGR_CQI_REPT
#ifdef ANSI
PRIVATE INLINE Void rgSCHCmnDlProcCqiMode10
(
 RgSchCellCb        *cell,
 RgSchUeCb          *ue,
 TfuDlCqiRpt        *dlCqiRpt,
 Bool               *isCqiAvail
 )
#else
PRIVATE INLINE Void rgSCHCmnDlProcCqiMode10(cell, ue, dlCqiRpt, isCqiAvail)
 RgSchCellCb        *cell;
 RgSchUeCb          *ue;
 TfuDlCqiRpt        *dlCqiRpt;
 Bool               *isCqiAvail;
#endif
#else
#ifdef ANSI
PRIVATE INLINE Void rgSCHCmnDlProcCqiMode10
(
 RgSchCellCb        *cell,
 RgSchUeCb          *ue,
 TfuDlCqiRpt        *dlCqiRpt
 )
#else
PRIVATE INLINE Void rgSCHCmnDlProcCqiMode10(cell, ue, dlCqiRpt)
 RgSchCellCb        *cell;
 RgSchUeCb          *ue;
 TfuDlCqiRpt        *dlCqiRpt;
#endif
#endif
{
   RgSchCmnDlUe  *ueDl = RG_SCH_CMN_GET_DL_UE(ue);
   TfuDlCqiPucch *pucchCqi = &dlCqiRpt->dlCqiInfo.pucchCqi;
   TRC2(rgSCHCmnDlProcCqiMode10);

   if (pucchCqi->u.mode10Info.type == TFU_RPT_CQI)
   {
      /*ccpu00109787 - ADD - Check for non-zero CQI*/
      /* Checking whether the decoded CQI is a value between 1 and 15*/
      if((pucchCqi->u.mode10Info.u.cqi) && (pucchCqi->u.mode10Info.u.cqi
               < RG_SCH_CMN_MAX_CQI))
      {
         ueDl->cqiFlag = TRUE;
#if 0 /* Commenting as this was a part of demo, not to be sent in production code */    
         /* LTE_ADV_FLAG_REMOVED_START */
         if(RGR_ENABLE == cell->lteAdvCb.absCfg.status)
         {
            if(cell->lteAdvCb.absCfg.absPatternType & RGR_ABS_TRANSMIT) /*PICO Case */
            {
               if(TRUE == ue->lteAdvUeCb.rgrLteAdvUeCfg.isUeCellEdge)
               {
                  pucchCqi->u.mode10Info.u.cqi = 8;   
               }
            } 
         }  
         /* LTE_ADV_FLAG_REMOVED_END */
#endif    
         ueDl->mimoInfo.cwInfo[0].cqi = pucchCqi->u.mode10Info.u.cqi;
         ueDl->mimoInfo.cwInfo[1].cqi = ueDl->mimoInfo.cwInfo[0].cqi;
         /* ccpu00117452 - MOD - Changed macro name from
            RGR_RRM_DLPWR_CNTRL to RGR_CQI_REPT */
#ifdef RGR_CQI_REPT
         *isCqiAvail = TRUE;
#endif
      }
      else
      {
         RETVOID;
      }
   }
   else if (pucchCqi->u.mode10Info.type == TFU_RPT_RI)
   {
      if ( RG_SCH_CMN_IS_RI_VALID(pucchCqi->u.mode10Info.u.ri) )
      {
         rgSCHCmnDlSetUeRi(cell, ue, pucchCqi->u.mode10Info.u.ri,
                           TRUE);
      }
      else
      {
         RLOG_ARG2(L_ERROR,DBG_CELLID,cell->cellId,"Invalid RI value(%x) CRNTI:%d",
            pucchCqi->u.mode10Info.u.ri,ue->ueId);
         RETVOID;
      }
   }
}

/**
 * @brief This function Updates the DL CQI on PUCCH for the UE.
 *
 * @details
 *
 *     Function: rgSCHCmnDlProcCqiMode11
 *
 *     This function updates the DL CQI on PUCCH for the UE.
 *
 *     Invoked by: rgSCHCmnDlCqiOnPucchInd
 *
 *     Processing Steps:
 *       Process CQI MODE 11
 *  @param[in] RgSchCellCb     *cell
 *  @param[in] RgSchUeCb       *ue
 *  @param[in] TfuDlCqiRpt     *dlCqiRpt
 *  @return  S16
 *      -# ROK
 *      -# RFAILED
 **/
#ifdef RGR_CQI_REPT
#ifdef ANSI
PRIVATE INLINE Void rgSCHCmnDlProcCqiMode11
(
 RgSchCellCb        *cell,
 RgSchUeCb          *ue,
 TfuDlCqiRpt        *dlCqiRpt,
 Bool               *isCqiAvail,
 Bool               *is2ndCwCqiAvail
 )
#else
PRIVATE INLINE Void rgSCHCmnDlProcCqiMode11(cell, ue, dlCqiRpt, isCqiAvail, is2ndCwCqiAvail)
 RgSchCellCb        *cell;
 RgSchUeCb          *ue;
 TfuDlCqiRpt        *dlCqiRpt;
 Bool               *isCqiAvail;
 Bool               *is2ndCwCqiAvail;
#endif
#else
#ifdef ANSI
PRIVATE INLINE Void rgSCHCmnDlProcCqiMode11
(
 RgSchCellCb        *cell,
 RgSchUeCb          *ue,
 TfuDlCqiRpt        *dlCqiRpt
 )
#else
PRIVATE INLINE Void rgSCHCmnDlProcCqiMode11(cell, ue, dlCqiRpt)
 RgSchCellCb        *cell;
 RgSchUeCb          *ue;
 TfuDlCqiRpt        *dlCqiRpt;
#endif
#endif
{
   RgSchCmnDlUe  *ueDl = RG_SCH_CMN_GET_DL_UE(ue);
   TfuDlCqiPucch *pucchCqi = &dlCqiRpt->dlCqiInfo.pucchCqi;
   TRC2(rgSCHCmnDlProcCqiMode11);

   if (pucchCqi->u.mode11Info.type == TFU_RPT_CQI)
   {
      ue->mimoInfo.puschFdbkVld  = FALSE;
      /*ccpu00109787 - ADD - Check for non-zero CQI*/
      if((pucchCqi->u.mode11Info.u.cqi.cqi) &&
            (pucchCqi->u.mode11Info.u.cqi.cqi < RG_SCH_CMN_MAX_CQI))
      {
         ueDl->cqiFlag = TRUE;
         /* ccpu00117452 - MOD - Changed macro name from
            RGR_RRM_DLPWR_CNTRL to RGR_CQI_REPT */
#ifdef RGR_CQI_REPT
         *isCqiAvail = TRUE;
#endif
         ueDl->mimoInfo.cwInfo[0].cqi = pucchCqi->u.mode11Info.u.cqi.cqi;
         if (pucchCqi->u.mode11Info.u.cqi.wideDiffCqi.pres)
         {
            RG_SCH_UPDT_CW2_CQI(ueDl->mimoInfo.cwInfo[0].cqi, \
                                     ueDl->mimoInfo.cwInfo[1].cqi, \
                                     pucchCqi->u.mode11Info.u.cqi.wideDiffCqi.val);
#ifdef RGR_CQI_REPT
            /* ccpu00117259 - ADD - Considering second codeword CQI info
               incase of MIMO for CQI Reporting */
            *is2ndCwCqiAvail = TRUE;
#endif
         }
      }
      else
      {
         RETVOID;
      }
      rgSCHCmnDlSetUePmi(cell, ue, \
            pucchCqi->u.mode11Info.u.cqi.pmi);
   }
   else if (pucchCqi->u.mode11Info.type == TFU_RPT_RI)
   {
      if( RG_SCH_CMN_IS_RI_VALID(pucchCqi->u.mode11Info.u.ri))
      {
         rgSCHCmnDlSetUeRi(cell, ue,  pucchCqi->u.mode11Info.u.ri,
                           TRUE);
      }
      else
      {
         RLOG_ARG2(L_ERROR,DBG_CELLID,cell->cellId,  "Invalid RI value(%x) CRNTI:%d",
            pucchCqi->u.mode11Info.u.ri,ue->ueId);
         RETVOID;
      }
   }
}

/**
 * @brief This function Updates the DL CQI on PUCCH for the UE.
 *
 * @details
 *
 *     Function: rgSCHCmnDlProcCqiMode20
 *
 *     This function updates the DL CQI on PUCCH for the UE.
 *
 *     Invoked by: rgSCHCmnDlCqiOnPucchInd
 *
 *     Processing Steps:
 *       Process CQI MODE 20
 *  @param[in] RgSchCellCb     *cell
 *  @param[in] RgSchUeCb       *ue
 *  @param[in] TfuDlCqiRpt     *dlCqiRpt
 *  @return  S16
 *      -# ROK
 *      -# RFAILED
 **/
#ifdef RGR_CQI_REPT
#ifdef ANSI
PRIVATE INLINE Void rgSCHCmnDlProcCqiMode20
(
 RgSchCellCb        *cell,
 RgSchUeCb          *ue,
 TfuDlCqiRpt        *dlCqiRpt,
 Bool               *isCqiAvail
 )
#else
PRIVATE INLINE Void rgSCHCmnDlProcCqiMode20(cell, ue, dlCqiRpt, isCqiAvail )
 RgSchCellCb        *cell;
 RgSchUeCb          *ue;
 TfuDlCqiRpt        *dlCqiRpt;
 Bool               *isCqiAvail;
#endif
#else
#ifdef ANSI
PRIVATE INLINE Void rgSCHCmnDlProcCqiMode20
(
 RgSchCellCb        *cell,
 RgSchUeCb          *ue,
 TfuDlCqiRpt        *dlCqiRpt
 )
#else
PRIVATE INLINE Void rgSCHCmnDlProcCqiMode20(cell, ue, dlCqiRpt)
 RgSchCellCb        *cell;
 RgSchUeCb          *ue;
 TfuDlCqiRpt        *dlCqiRpt;
#endif
#endif
{
   RgSchCmnDlUe  *ueDl = RG_SCH_CMN_GET_DL_UE(ue);
   TfuDlCqiPucch *pucchCqi = &dlCqiRpt->dlCqiInfo.pucchCqi;
   TRC2(rgSCHCmnDlProcCqiMode20);

   if (pucchCqi->u.mode20Info.type == TFU_RPT_CQI)
   {
      if (pucchCqi->u.mode20Info.u.cqi.isWideband)
      {
         /*ccpu00109787 - ADD - Check for non-zero CQI*/
         if((pucchCqi->u.mode20Info.u.cqi.u.wideCqi) &&
               (pucchCqi->u.mode20Info.u.cqi.u.wideCqi < RG_SCH_CMN_MAX_CQI))
         {
            ueDl->cqiFlag = TRUE;
            ueDl->mimoInfo.cwInfo[0].cqi = pucchCqi->u.mode20Info.u.cqi.\
                                           u.wideCqi;
            ueDl->mimoInfo.cwInfo[1].cqi = ueDl->mimoInfo.cwInfo[0].cqi;
            /* ccpu00117452 - MOD - Changed macro name from
               RGR_RRM_DLPWR_CNTRL to RGR_CQI_REPT */
#ifdef RGR_CQI_REPT
            *isCqiAvail = TRUE;
#endif
         }
         else
         {
            RETVOID;
         }
      }
   }
   else if (pucchCqi->u.mode20Info.type == TFU_RPT_RI)
   {
      if(RG_SCH_CMN_IS_RI_VALID(pucchCqi->u.mode20Info.u.ri))
      {
         rgSCHCmnDlSetUeRi(cell, ue, pucchCqi->u.mode20Info.u.ri, 
                           TRUE);
      }
      else
      {
         RLOG_ARG2(L_ERROR,DBG_CELLID,cell->cellId,"Invalid RI value(%x) CRNTI:%d",
            pucchCqi->u.mode20Info.u.ri,ue->ueId);
         RETVOID;
      }
   }
}


/**
 * @brief This function Updates the DL CQI on PUCCH for the UE.
 *
 * @details
 *
 *     Function: rgSCHCmnDlProcCqiMode21
 *
 *     This function updates the DL CQI on PUCCH for the UE.
 *
 *     Invoked by: rgSCHCmnDlCqiOnPucchInd
 *
 *     Processing Steps:
 *       Process CQI MODE 21
 *  @param[in] RgSchCellCb     *cell
 *  @param[in] RgSchUeCb       *ue
 *  @param[in] TfuDlCqiRpt     *dlCqiRpt
 *  @return  S16
 *      -# ROK
 *      -# RFAILED
 **/
#ifdef RGR_CQI_REPT
#ifdef ANSI
PRIVATE INLINE Void rgSCHCmnDlProcCqiMode21
(
 RgSchCellCb        *cell,
 RgSchUeCb          *ue,
 TfuDlCqiRpt        *dlCqiRpt,
 Bool               *isCqiAvail,
 Bool               *is2ndCwCqiAvail
 )
#else
PRIVATE INLINE Void rgSCHCmnDlProcCqiMode21(cell, ue, dlCqiRpt, isCqiAvail, is2ndCwCqiAvail)
   RgSchCellCb        *cell;
   RgSchUeCb          *ue;
   TfuDlCqiRpt        *dlCqiRpt;
   Bool               *isCqiAvail;
   Bool               *is2ndCwCqiAvail;
#endif
#else
#ifdef ANSI
PRIVATE INLINE Void rgSCHCmnDlProcCqiMode21
(
 RgSchCellCb        *cell,
 RgSchUeCb          *ue,
 TfuDlCqiRpt        *dlCqiRpt
 )
#else
PRIVATE INLINE Void rgSCHCmnDlProcCqiMode21(cell, ue, dlCqiRpt)
 RgSchCellCb        *cell;
 RgSchUeCb          *ue;
 TfuDlCqiRpt        *dlCqiRpt;
#endif
#endif
{
   RgSchCmnDlUe  *ueDl = RG_SCH_CMN_GET_DL_UE(ue);
   TfuDlCqiPucch *pucchCqi = &dlCqiRpt->dlCqiInfo.pucchCqi;
   TRC2(rgSCHCmnDlProcCqiMode21);

   if (pucchCqi->u.mode21Info.type == TFU_RPT_CQI)
   {
      ue->mimoInfo.puschFdbkVld  = FALSE;
      if (pucchCqi->u.mode21Info.u.cqi.isWideband)
      {
         /*ccpu00109787 - ADD - Check for non-zero CQI*/
         if((pucchCqi->u.mode21Info.u.cqi.u.wideCqi.cqi) &&
               (pucchCqi->u.mode21Info.u.cqi.u.wideCqi.cqi < RG_SCH_CMN_MAX_CQI))
         {
            ueDl->cqiFlag = TRUE;
            ueDl->mimoInfo.cwInfo[0].cqi = pucchCqi->u.mode21Info.u.cqi.\
                                           u.wideCqi.cqi;
            if (pucchCqi->u.mode21Info.u.cqi.u.wideCqi.diffCqi.pres)
            {
               RG_SCH_UPDT_CW2_CQI(ueDl->mimoInfo.cwInfo[0].cqi, \
                                     ueDl->mimoInfo.cwInfo[1].cqi, \
                                     pucchCqi->u.mode21Info.u.cqi.u.wideCqi.diffCqi.val);
#ifdef RGR_CQI_REPT
               /* ccpu00117259 - ADD - Considering second codeword CQI info
                  incase of MIMO for CQI Reporting */
               *is2ndCwCqiAvail = TRUE;
#endif
            }
            /* ccpu00117452 - MOD - Changed macro name from
               RGR_RRM_DLPWR_CNTRL to RGR_CQI_REPT */
#ifdef RGR_CQI_REPT
            *isCqiAvail = TRUE;
#endif
         }
         else
         {
            RETVOID;
         }
         rgSCHCmnDlSetUePmi(cell, ue, \
               pucchCqi->u.mode21Info.u.cqi.u.wideCqi.pmi);
      }
   }
   else if (pucchCqi->u.mode21Info.type == TFU_RPT_RI)
   {
      if(RG_SCH_CMN_IS_RI_VALID(pucchCqi->u.mode21Info.u.ri))
      {
         rgSCHCmnDlSetUeRi(cell, ue, pucchCqi->u.mode21Info.u.ri,
                           TRUE);
      }
      else
      {
         RLOG_ARG2(L_ERROR,DBG_CELLID,cell->cellId,  "Invalid RI value(%x) CRNTI:%d",
            pucchCqi->u.mode21Info.u.ri,ue->ueId);
         RETVOID;
      }
   }
}


/**
 * @brief This function Updates the DL CQI on PUCCH for the UE.
 *
 * @details
 *
 *     Function: rgSCHCmnDlCqiOnPucchInd
 *
 *     This function updates the DL CQI on PUCCH for the UE.
 *
 *     Invoked by: rgSCHCmnDlCqiInd
 *
 *     Processing Steps:
 *     - Depending on the reporting mode of the PUCCH, the CQI/PMI/RI values
 *       are updated and stored for each UE
 *
 *  @param[in] RgSchCellCb     *cell
 *  @param[in] RgSchUeCb       *ue
 *  @param[in] TfuDlCqiRpt     *dlCqiRpt
 *  @return  S16
 *      -# ROK
 *      -# RFAILED
 **/
#ifdef RGR_CQI_REPT
#ifdef ANSI
PRIVATE Void rgSCHCmnDlCqiOnPucchInd
(
 RgSchCellCb        *cell,
 RgSchUeCb          *ue,
 TfuDlCqiRpt        *dlCqiRpt,
 RgrUeCqiRept       *ueCqiRept,
 Bool               *isCqiAvail,
 Bool               *is2ndCwCqiAvail
 )
#else
PRIVATE Void rgSCHCmnDlCqiOnPucchInd(cell, ue, dlCqiRpt, ueCqiRept, isCqiAvail, is2ndCwCqiAvail)
 RgSchCellCb        *cell;
 RgSchUeCb          *ue;
 TfuDlCqiRpt        *dlCqiRpt;
 RgrUeCqiRept       *ueCqiRept;
 Bool               *isCqiAvail;
 Bool               *is2ndCwCqiAvail;
#endif
#else
#ifdef ANSI
PRIVATE Void rgSCHCmnDlCqiOnPucchInd
(
 RgSchCellCb        *cell,
 RgSchUeCb          *ue,
 TfuDlCqiRpt        *dlCqiRpt
 )
#else
PRIVATE Void rgSCHCmnDlCqiOnPucchInd(cell, ue, dlCqiRpt)
 RgSchCellCb        *cell;
 RgSchUeCb          *ue;
 TfuDlCqiRpt        *dlCqiRpt;
#endif
#endif
{
   RgSchCmnDlUe  *ueDl = RG_SCH_CMN_GET_DL_UE(ue);
   TfuDlCqiPucch *pucchCqi = &dlCqiRpt->dlCqiInfo.pucchCqi;
   TRC2(rgSCHCmnDlCqiOnPucchInd);

   /* ccpu00117452 - MOD - Changed
      RGR_RRM_DLPWR_CNTRL to RGR_CQI_REPT */
#ifdef RGR_CQI_REPT
   /* Save CQI mode information in the report */
   ueCqiRept->cqiMode = pucchCqi->mode;
#endif

   switch(pucchCqi->mode)
   {
      case TFU_PUCCH_CQI_MODE10:
#ifdef RGR_CQI_REPT
         rgSCHCmnDlProcCqiMode10(cell, ue, dlCqiRpt, isCqiAvail);
#else
         rgSCHCmnDlProcCqiMode10(cell, ue, dlCqiRpt);
#endif
         ueDl->cqiFlag = TRUE;
         break;
      case TFU_PUCCH_CQI_MODE11:
#ifdef RGR_CQI_REPT
         rgSCHCmnDlProcCqiMode11(cell, ue, dlCqiRpt, isCqiAvail,
                is2ndCwCqiAvail);
#else
         rgSCHCmnDlProcCqiMode11(cell, ue, dlCqiRpt);
#endif
         ueDl->cqiFlag = TRUE;
         break;
      case TFU_PUCCH_CQI_MODE20:
#ifdef RGR_CQI_REPT
         rgSCHCmnDlProcCqiMode20(cell, ue, dlCqiRpt, isCqiAvail);
#else
         rgSCHCmnDlProcCqiMode20(cell, ue, dlCqiRpt);
#endif
         ueDl->cqiFlag = TRUE;
         break;
      case TFU_PUCCH_CQI_MODE21:
#ifdef RGR_CQI_REPT
         rgSCHCmnDlProcCqiMode21(cell, ue, dlCqiRpt, isCqiAvail,
                is2ndCwCqiAvail);
#else
         rgSCHCmnDlProcCqiMode21(cell, ue, dlCqiRpt);
#endif
         ueDl->cqiFlag = TRUE;
         break;
      default:
         {
            RLOG_ARG2(L_ERROR,DBG_CELLID,cell->cellId,"Unknown CQI Mode %d",
               pucchCqi->mode,ue->ueId);
       /* ccpu00117452 - MOD - Changed macro name from
               RGR_RRM_DLPWR_CNTRL to RGR_CQI_REPT */
#ifdef RGR_CQI_REPT
            *isCqiAvail = FALSE;
#endif
         }
         break;
   }

  RETVOID;
}  /* rgSCHCmnDlCqiOnPucchInd */


/**
 * @brief This function Updates the DL CQI on PUSCH for the UE.
 *
 * @details
 *
 *     Function: rgSCHCmnDlCqiOnPuschInd
 *
 *     This function updates the DL CQI on PUSCH for the UE.
 *
 *     Invoked by: rgSCHCmnDlCqiInd
 *
 *     Processing Steps:
 *     - Depending on the reporting mode of the PUSCH, the CQI/PMI/RI values
 *       are updated and stored for each UE
 *
 *  @param[in] RgSchCellCb     *cell
 *  @param[in] RgSchUeCb       *ue
 *  @param[in] TfuDlCqiRpt     *dlCqiRpt
 *  @return  S16
 *      -# ROK
 *      -# RFAILED
 **/
#ifdef RGR_CQI_REPT
#ifdef ANSI
PRIVATE Void rgSCHCmnDlCqiOnPuschInd
(
 RgSchCellCb        *cell,
 RgSchUeCb          *ue,
 TfuDlCqiRpt        *dlCqiRpt,
 RgrUeCqiRept       *ueCqiRept,
 Bool               *isCqiAvail,
 Bool               *is2ndCwCqiAvail
 )
#else
PRIVATE Void rgSCHCmnDlCqiOnPuschInd(cell, ue, dlCqiRpt, ueCqiRept, isCqiAvail, is2ndCwCqiAvail)
 RgSchCellCb        *cell;
 RgSchUeCb          *ue;
 TfuDlCqiRpt        *dlCqiRpt;
 RgrUeCqiRept       *ueCqiRept;
 Bool               *isCqiAvail;
 Bool               *is2ndCwCqiAvail;
#endif
#else
#ifdef ANSI
PRIVATE Void rgSCHCmnDlCqiOnPuschInd
(
 RgSchCellCb        *cell,
 RgSchUeCb          *ue,
 TfuDlCqiRpt        *dlCqiRpt
 )
#else
PRIVATE Void rgSCHCmnDlCqiOnPuschInd(cell, ue, dlCqiRpt)
   RgSchCellCb        *cell;
   RgSchUeCb          *ue;
   TfuDlCqiRpt        *dlCqiRpt;
#endif
#endif
{
   RgSchCmnDlUe  *ueDl = RG_SCH_CMN_GET_DL_UE(ue);
   U32 prevRiVal = 0; 
   TfuDlCqiPusch *puschCqi = &dlCqiRpt->dlCqiInfo.puschCqi;
   TRC2(rgSCHCmnDlCqiOnPuschInd);
   if (puschCqi->ri.pres == PRSNT_NODEF)
   {
      if (RG_SCH_CMN_IS_RI_VALID(puschCqi->ri.val))
      {
         /* Saving the previous ri value to revert back
            in  case PMI update failed */
         if (RGR_UE_TM_4 == ue->mimoInfo.txMode ) /* Cheking for TM4. TM8 check later */
         {
            prevRiVal = ueDl->mimoInfo.ri;
         }
         rgSCHCmnDlSetUeRi(cell, ue, puschCqi->ri.val, FALSE);
      }
      else
      {
         RLOG_ARG2(L_ERROR,DBG_CELLID,cell->cellId,"Invalid RI value(%x) CRNTI:%d",
            puschCqi->ri.val,ue->ueId);
         RETVOID;
      }
   }
   ue->mimoInfo.puschFdbkVld  = FALSE;
   /* ccpu00117452 - MOD - Changed macro name from
      RGR_RRM_DLPWR_CNTRL to RGR_CQI_REPT */
#ifdef RGR_CQI_REPT
   /* Save CQI mode information in the report */
   ueCqiRept->cqiMode = puschCqi->mode;
   /* ccpu00117259 - DEL - removed default setting of isCqiAvail to TRUE */
#endif

   switch(puschCqi->mode)
   {
      case TFU_PUSCH_CQI_MODE_20:
         /*ccpu00109787 - ADD - Check for non-zero CQI*/
         /* Checking whether the decoded CQI is a value between 1 and 15*/
         if((puschCqi->u.mode20Info.wideBandCqi) &&
               (puschCqi->u.mode20Info.wideBandCqi < RG_SCH_CMN_MAX_CQI))
         {
            ueDl->mimoInfo.cwInfo[0].cqi = puschCqi->u.mode20Info.wideBandCqi;
            ueDl->mimoInfo.cwInfo[1].cqi = ueDl->mimoInfo.cwInfo[0].cqi;
            /* ccpu00117452 - MOD - Changed macro name from
               RGR_RRM_DLPWR_CNTRL to RGR_CQI_REPT */
#ifdef RGR_CQI_REPT
           *isCqiAvail = TRUE;
#endif
         }
         else
         {
            RETVOID;
         }
         break;
      case TFU_PUSCH_CQI_MODE_30:
         /*ccpu00109787 - ADD - Check for non-zero CQI*/
         if((puschCqi->u.mode30Info.wideBandCqi) &&
               (puschCqi->u.mode30Info.wideBandCqi < RG_SCH_CMN_MAX_CQI))
         {
            ueDl->mimoInfo.cwInfo[0].cqi = puschCqi->u.mode30Info.wideBandCqi;
            ueDl->mimoInfo.cwInfo[1].cqi = ueDl->mimoInfo.cwInfo[0].cqi;
            /* ccpu00117452 - MOD - Changed macro name from
               RGR_RRM_DLPWR_CNTRL to RGR_CQI_REPT */
#ifdef RGR_CQI_REPT
            *isCqiAvail = TRUE;
#endif
         }
         else
         {
            RETVOID;
         }
         break;
      case TFU_PUSCH_CQI_MODE_12:
         /*ccpu00109787 - ADD - Check for non-zero CQI*/
         if((puschCqi->u.mode12Info.cqiIdx[0]) &&
               (puschCqi->u.mode12Info.cqiIdx[0] < RG_SCH_CMN_MAX_CQI))
         {
            ueDl->mimoInfo.cwInfo[0].cqi = puschCqi->u.mode12Info.cqiIdx[0];
            /* ccpu00117452 - MOD - Changed macro name from
               RGR_RRM_DLPWR_CNTRL to RGR_CQI_REPT */
#ifdef RGR_CQI_REPT
            *isCqiAvail = TRUE;
#endif
         }
         else
         {
            RETVOID;
         }
         if((puschCqi->u.mode12Info.cqiIdx[1]) &&
               (puschCqi->u.mode12Info.cqiIdx[1] < RG_SCH_CMN_MAX_CQI))
         {
            ueDl->mimoInfo.cwInfo[1].cqi = puschCqi->u.mode12Info.cqiIdx[1];
            /* ccpu00117452 - MOD - Changed macro name from
               RGR_RRM_DLPWR_CNTRL to RGR_CQI_REPT */
#ifdef RGR_CQI_REPT
            /* ccpu00117259 - ADD - Considering second codeword CQI info
               incase of MIMO for CQI Reporting */
            *is2ndCwCqiAvail = TRUE;
#endif
         }
         else
         {
            RETVOID;
         }
         ue->mimoInfo.puschFdbkVld  = TRUE;
         ue->mimoInfo.puschPmiInfo.mode = TFU_PUSCH_CQI_MODE_12;
         ue->mimoInfo.puschPmiInfo.u.mode12Info = puschCqi->u.mode12Info;
         /*  : resetting this is time based. Make use of CQI reporting
          * periodicity, DELTA's in determining the exact time at which this
          * need to be reset. */
         break;
      case TFU_PUSCH_CQI_MODE_22:
         /*ccpu00109787 - ADD - Check for non-zero CQI*/
         if((puschCqi->u.mode22Info.wideBandCqi[0]) &&
               (puschCqi->u.mode22Info.wideBandCqi[0] < RG_SCH_CMN_MAX_CQI))
         {
            ueDl->mimoInfo.cwInfo[0].cqi = puschCqi->u.mode22Info.wideBandCqi[0];
            /* ccpu00117452 - MOD - Changed macro name from
               RGR_RRM_DLPWR_CNTRL to RGR_CQI_REPT */
#ifdef RGR_CQI_REPT
            *isCqiAvail = TRUE;
#endif
         }
         else
         {
            RETVOID;
         }
         if((puschCqi->u.mode22Info.wideBandCqi[1]) &&
               (puschCqi->u.mode22Info.wideBandCqi[1] < RG_SCH_CMN_MAX_CQI))
         {
            ueDl->mimoInfo.cwInfo[1].cqi = puschCqi->u.mode22Info.wideBandCqi[1];
            /* ccpu00117452 - MOD - Changed macro name from
               RGR_RRM_DLPWR_CNTRL to RGR_CQI_REPT */
#ifdef RGR_CQI_REPT
            /* ccpu00117259 - ADD - Considering second codeword CQI info
               incase of MIMO for CQI Reporting */
            *is2ndCwCqiAvail = TRUE;
#endif
         }
         else
         {
            RETVOID;
         }
         rgSCHCmnDlSetUePmi(cell, ue, puschCqi->u.mode22Info.wideBandPmi);
         ue->mimoInfo.puschFdbkVld  = TRUE;
         ue->mimoInfo.puschPmiInfo.mode = TFU_PUSCH_CQI_MODE_22;
         ue->mimoInfo.puschPmiInfo.u.mode22Info = puschCqi->u.mode22Info;
         break;
      case TFU_PUSCH_CQI_MODE_31:
         /*ccpu00109787 - ADD - Check for non-zero CQI*/
         if((puschCqi->u.mode31Info.wideBandCqi[0]) &&
               (puschCqi->u.mode31Info.wideBandCqi[0] < RG_SCH_CMN_MAX_CQI))
         {
            ueDl->mimoInfo.cwInfo[0].cqi = puschCqi->u.mode31Info.wideBandCqi[0];
            /* ccpu00117452 - MOD - Changed macro name from
               RGR_RRM_DLPWR_CNTRL to RGR_CQI_REPT */
#ifdef RGR_CQI_REPT
            *isCqiAvail = TRUE;
#endif
         }
         if (ueDl->mimoInfo.ri > 1)
         {
           if((puschCqi->u.mode31Info.wideBandCqi[1]) &&
               (puschCqi->u.mode31Info.wideBandCqi[1] < RG_SCH_CMN_MAX_CQI))
           {
             ueDl->mimoInfo.cwInfo[1].cqi = puschCqi->u.mode31Info.wideBandCqi[1];
            /* ccpu00117452 - MOD - Changed macro name from
               RGR_RRM_DLPWR_CNTRL to RGR_CQI_REPT */
#ifdef RGR_CQI_REPT
            /* ccpu00117259 - ADD - Considering second codeword CQI info
               incase of MIMO for CQI Reporting */
             *is2ndCwCqiAvail = TRUE;
#endif
           }
         }
         if (rgSCHCmnDlSetUePmi(cell, ue, puschCqi->u.mode31Info.pmi) != ROK)
         {
            /* To avoid Rank and PMI inconsistency */
            if ((puschCqi->ri.pres == PRSNT_NODEF) &&
                (RGR_UE_TM_4 == ue->mimoInfo.txMode)) /* checking for TM4. TM8 check later */
            {
               ueDl->mimoInfo.ri = prevRiVal;
            }
         }
         ue->mimoInfo.puschPmiInfo.mode = TFU_PUSCH_CQI_MODE_31;
         ue->mimoInfo.puschPmiInfo.u.mode31Info = puschCqi->u.mode31Info;
         break;
      default:
         {
            RLOG_ARG2(L_ERROR,DBG_CELLID,cell->cellId,  "Unknown CQI Mode %d CRNTI:%d",
               puschCqi->mode,ue->ueId);
            /*  CQI decoding failed revert the RI to previous value */
            if ((puschCqi->ri.pres == PRSNT_NODEF) &&
                (RGR_UE_TM_4 == ue->mimoInfo.txMode)) /* checking for TM4. TM8 check later */
            {
               ueDl->mimoInfo.ri = prevRiVal;
            }
            /* ccpu00117452 - MOD - Changed macro name from
               RGR_RRM_DLPWR_CNTRL to RGR_CQI_REPT */
#ifdef RGR_CQI_REPT
           *isCqiAvail = FALSE;
            /* ccpu00117259 - ADD - Considering second codeword CQI info
               incase of MIMO for CQI Reporting */
            *is2ndCwCqiAvail = FALSE;
#endif
         }
         break;
   }

   RETVOID;
}  /* rgSCHCmnDlCqiOnPuschInd */


/**
 * @brief This function Updates the DL CQI for the UE.
 *
 * @details
 *
 *     Function: rgSCHCmnDlCqiInd
 *     Purpose:  Updates the DL CQI for the UE
 *
 *     Invoked by: TOM
 *
 *  @param[in]  RgSchCellCb        *cell
 *  @param[in]  RgSchUeCb          *ue
 *  @param[in]  TfuDlCqiRpt        *dlCqiRpt
 *  @return  Void
 *
 **/
#ifdef ANSI
PUBLIC Void rgSCHCmnDlCqiInd
(
RgSchCellCb        *cell,
RgSchUeCb          *ue,
TfuDlCqiRpt        *dlCqiRpt,
CmLteTimingInfo    timingInfo
)
#else
PUBLIC Void rgSCHCmnDlCqiInd(cell, ue, dlCqiRpt, timingInfo)
RgSchCellCb        *cell;
RgSchUeCb          *ue;
TfuDlCqiRpt        *dlCqiRpt;
CmLteTimingInfo    timingInfo;
#endif
{
   RgSchCmnCell  *cellSch = RG_SCH_CMN_GET_CELL(cell);
/* ccpu00117452 - MOD - Changed macro name from
   RGR_RRM_DLPWR_CNTRL to RGR_CQI_REPT */
#ifdef RGR_CQI_REPT
   RgSchCmnDlUe  *ueDl = RG_SCH_CMN_GET_DL_UE(ue);
   RgrUeCqiRept   ueCqiRept = {{0}};
   Bool           isCqiAvail = FALSE;
   /* ccpu00117259 - ADD - Considering second codeword CQI info
      incase of MIMO for CQI Reporting */
   Bool           is2ndCwCqiAvail = FALSE;
#endif

   TRC2(rgSCHCmnDlCqiInd);

#ifdef RGR_CQI_REPT
   if (dlCqiRpt->isPucchInfo)
   {
      rgSCHCmnDlCqiOnPucchInd(cell, ue, dlCqiRpt,&ueCqiRept, &isCqiAvail, &is2ndCwCqiAvail);
   }
   else
   {
      rgSCHCmnDlCqiOnPuschInd(cell, ue, dlCqiRpt,&ueCqiRept,  &isCqiAvail, &is2ndCwCqiAvail);
   }
#else
   if (dlCqiRpt->isPucchInfo)
   {
      rgSCHCmnDlCqiOnPucchInd(cell, ue, dlCqiRpt);
   }
   else
   {
      rgSCHCmnDlCqiOnPuschInd(cell, ue, dlCqiRpt);
   }
#endif

/* ccpu00117452 - MOD - Changed macro name from
   RGR_RRM_DLPWR_CNTRL to RGR_CQI_REPT */
#ifdef RGR_CQI_REPT
   /* ccpu00117259 - ADD - Considering second codeword CQI info
      incase of MIMO for CQI Reporting - added is2ndCwCqiAvail\
      in 'if' condition*/
   if (RG_SCH_CQIR_IS_PUSHNCQI_ENBLE(ue) && (isCqiAvail || is2ndCwCqiAvail))
   {
      ueCqiRept.cqi[0] = ueDl->mimoInfo.cwInfo[0].cqi;

   /* ccpu00117259 - ADD - Considering second codeword CQI info
      incase of MIMO for CQI Reporting - added is2ndCwCqiAvail
      in 'if' condition*/
      ueCqiRept.cqi[1] = 0;
      if(is2ndCwCqiAvail)
      {
         ueCqiRept.cqi[1] = ueDl->mimoInfo.cwInfo[1].cqi;
      }
      rgSCHCmnUeDlPwrCtColltCqiRept(cell, ue, &ueCqiRept);

   }
#endif
#ifdef DL_LA
   rgSCHCmnDlSetUeAllocLmtLa(cell, ue);
   rgSCHCheckAndSetTxScheme(cell, ue);
#else
   rgSCHCmnDlSetUeAllocLmt(cell, RG_SCH_CMN_GET_DL_UE(ue));
#endif

   if (cellSch->dl.isDlFreqSel)
   {
      cellSch->apisDlfs->rgSCHDlfsDlCqiInd(cell, ue, dlCqiRpt, timingInfo);
   }
#ifdef LTEMAC_SPS
   /* Call SPS module to update CQI indication */
   rgSCHCmnSpsDlCqiIndHndlr(cell, ue, dlCqiRpt, timingInfo);
#endif
   /* Call Specific scheduler to process on dlCqiInd */
   cellSch->apisDl->rgSCHDlCqiInd(cell, ue, dlCqiRpt);

#ifdef TENB_STATS
   ue->tenbStats->stats.sch.dlSumCw0Cqi +=  ueDl->mimoInfo.cwInfo[0].cqi;
   ue->tenbStats->stats.sch.dlSumCw1Cqi +=  ueDl->mimoInfo.cwInfo[1].cqi;
   ue->tenbStats->stats.sch.dlNumCw0Cqi ++;
   ue->tenbStats->stats.sch.dlNumCw1Cqi ++;
   cell->tenbStats->sch.dlSumCw0Cqi +=  ueDl->mimoInfo.cwInfo[0].cqi;
   cell->tenbStats->sch.dlSumCw1Cqi +=  ueDl->mimoInfo.cwInfo[1].cqi;
   cell->tenbStats->sch.dlNumCw0Cqi ++;
   cell->tenbStats->sch.dlNumCw1Cqi ++;
#endif
   RETVOID;
}

#ifdef TFU_UPGRADE
/**
 * @brief This function calculates the wideband CQI from SNR
 *            reported for each RB.
 *
 * @details
 *
 *     Function: rgSCHCmnCalcWcqiFrmSnr
 *     Purpose:  Wideband CQI calculation from SNR
 *
 *     Invoked by: RG SCH
 *
 *  @param[in]  RgSchCellCb        *cell
 *  @param[in]  TfuSrsRpt        *srsRpt,
 *  @return  Wideband CQI
 *
 **/
#ifdef ANSI
PRIVATE U8 rgSCHCmnCalcWcqiFrmSnr
(
 RgSchCellCb        *cell,
 TfuSrsRpt        *srsRpt
 )
#else
PRIVATE U8 rgSCHCmnCalcWcqiFrmSnr(cell,srsRpt)
   RgSchCellCb        *cell;
   TfuSrsRpt            *srsRpt;
#endif
{
   U8 wideCqi=1; /*Calculated value from SNR*/
   TRC2(rgSCHCmnCalcWcqiFrmSnr);
   /*Need to map a certain SNR with a WideCQI value.
    * The CQI calculation is still primitive. Further, need to
    * use a improvized method for calculating WideCQI from SNR*/
       if (srsRpt->snr[0] <=50)
       {
           wideCqi=3;
       }
       else if (srsRpt->snr[0]>=51 && srsRpt->snr[0] <=100)
       {
           wideCqi=6;
       }
       else if (srsRpt->snr[0]>=101 && srsRpt->snr[0] <=150)
       {
           wideCqi=9;
       }
       else if (srsRpt->snr[0]>=151 && srsRpt->snr[0] <=200)
       {
           wideCqi=12;
       }
       else if (srsRpt->snr[0]>=201 && srsRpt->snr[0] <=250)
       {
           wideCqi=14;
       }
       else
       {
           wideCqi=15;
       }
   RETVALUE(wideCqi);
}/*rgSCHCmnCalcWcqiFrmSnr*/


/**
 * @brief This function Updates the SRS for the UE.
 *
 * @details
 *
 *     Function: rgSCHCmnSrsInd
 *     Purpose:  Updates the UL SRS for the UE
 *
 *     Invoked by: TOM
 *
 *  @param[in]  RgSchCellCb        *cell
 *  @param[in]  RgSchUeCb          *ue
 *  @param[in]  TfuSrsRpt        *srsRpt,
 *  @return  Void
 *
 **/
#ifdef ANSI
PUBLIC Void rgSCHCmnSrsInd
(
 RgSchCellCb        *cell,
 RgSchUeCb          *ue,
 TfuSrsRpt        *srsRpt,
 CmLteTimingInfo    timingInfo
 )
#else
PUBLIC Void rgSCHCmnSrsInd(cell, ue, srsRpt, timingInfo)
    RgSchCellCb        *cell;
    RgSchUeCb          *ue;
    TfuSrsRpt            *srsRpt;
    CmLteTimingInfo    timingInfo;
#endif
{
    U8 wideCqi; /*Calculated value from SNR*/
    U32 recReqTime; /*Received Time in TTI*/
    TRC2(rgSCHCmnSrsInd);

    recReqTime = (timingInfo.sfn * RGSCH_NUM_SUB_FRAMES) + timingInfo.subframe;
    ue->srsCb.selectedAnt = (recReqTime/ue->srsCb.peri)%2;
    if(srsRpt->wideCqiPres)
    {
        wideCqi = srsRpt->wideCqi;
    }
    else
    {
        wideCqi = rgSCHCmnCalcWcqiFrmSnr(cell, srsRpt);
    }
    rgSCHCmnFindUlCqiUlTxAnt(cell, ue, wideCqi);
    RETVOID;
}/*rgSCHCmnSrsInd*/
#endif


/**
 * @brief This function is a handler for TA report for an UE.
 *
 * @details
 *
 *     Function: rgSCHCmnDlTARpt
 *     Purpose:  Determine based on UE_IDLE_TIME threshold,
 *     whether UE needs to be Linked to the scheduler's TA list OR
 *     if it needs a PDCCH Order.
 *
 *
 *     Invoked by: TOM
 *
 *  @param[in]  RgSchCellCb        *cell
 *  @param[in]  RgSchUeCb          *ue
 *  @return  Void
 *
 **/
#ifdef ANSI
PUBLIC Void rgSCHCmnDlTARpt
(
RgSchCellCb        *cell,
RgSchUeCb          *ue
)
#else
PUBLIC Void rgSCHCmnDlTARpt(cell, ue)
RgSchCellCb        *cell;
RgSchUeCb          *ue;
#endif
{
   RgSchCmnCell    *cellSch = RG_SCH_CMN_GET_CELL(cell);
   RgSchCmnDlCell  *cellDl = RG_SCH_CMN_GET_DL_CELL(cell);
   RgSchCmnDlUe    *ueDl = RG_SCH_CMN_GET_DL_UE(ue);
   CmLListCp       poInactvLst;

   TRC2(rgSCHCmnDlTARpt);

   /* RACHO: If UE idle time is more than threshold, then
    * set its poInactv pdcch order inactivity  */
   /* Fix : syed Ignore if TaTmr is not configured */
   if ((ue->dl.taCb.cfgTaTmr) && (rgSCHCmnUeIdleExdThrsld(cell, ue) == ROK))
   {
      U32 prevDlMsk = ue->dl.dlInactvMask;
      U32 prevUlMsk = ue->ul.ulInactvMask;
      ue->dl.dlInactvMask |= RG_PDCCHODR_INACTIVE;
      ue->ul.ulInactvMask |= RG_PDCCHODR_INACTIVE;
      /* Indicate Specific scheduler for this UEs inactivity */
      cmLListInit(&poInactvLst);
      cmLListAdd2Tail(&poInactvLst, &ueDl->rachInfo.inActUeLnk);
      ueDl->rachInfo.inActUeLnk.node = (PTR)ue;
      /* Send inactivate ind only if not already sent */
      if (prevDlMsk == 0)
      {
         cellSch->apisDl->rgSCHDlInactvtUes(cell, &poInactvLst);
      }
      if (prevUlMsk == 0)
      {
         cellSch->apisUl->rgSCHUlInactvtUes(cell, &poInactvLst);
      }
   }
   else
   {
      /* Fix: ccpu00124009 Fix for loop in the linked list "cellDl->taLst" */
      if (!ue->dlTaLnk.node)
      {
         cmLListAdd2Tail(&cellDl->taLst, &ue->dlTaLnk);
         ue->dlTaLnk.node = (PTR)ue;
      }
      else
      {
         RLOG_ARG1(L_ERROR,DBG_CELLID,cell->cellId,
               "<TA>TA duplicate entry attempt failed: UEID:%u", 
               ue->ueId);
      }
   }
   RETVOID;
}

#ifdef TFU_UPGRADE
/**
 * @brief Indication of UL CQI.
 *
 * @details
 *
 *     Function : rgSCHCmnFindUlCqiUlTxAnt
 *
 *     - Finds the Best Tx Antenna amongst the CQIs received
 *         from Two Tx Antennas.
 *
 *  @param[in]  RgSchCellCb         *cell
 *  @param[in]  RgSchUeCb           *ue
 *  @param[in]   U8                 wideCqi
 *  @return  Void
 **/
#ifdef ANSI
PRIVATE Void rgSCHCmnFindUlCqiUlTxAnt
(
RgSchCellCb     *cell,
RgSchUeCb       *ue,
U8              wideCqi
)
#else
PRIVATE Void rgSCHCmnFindUlCqiUlTxAnt(cell, ue, wideCqi)
RgSchCellCb     *cell;
RgSchUeCb       *ue;
U8              wideCqi;
#endif
{
#if 0
#else
   ue->validTxAnt = 1;
#endif
   RETVOID;
}  /* rgSCHCmnFindUlCqiUlTxAnt */
#endif

/**
 * @brief Indication of UL CQI.
 *
 * @details
 *
 *     Function : rgSCHCmnUlCqiInd
 *
 *     - Updates uplink CQI information for the UE. Computes and
 *       stores the lowest CQI of CQIs reported in all subbands.
 *
 *  @param[in]  RgSchCellCb         *cell
 *  @param[in]  RgSchUeCb           *ue
 *  @param[in]  TfuUlCqiRpt         *ulCqiInfo
 *  @return  Void
 **/
#ifdef ANSI
PUBLIC Void rgSCHCmnUlCqiInd
(
RgSchCellCb          *cell,
RgSchUeCb            *ue,
TfuUlCqiRpt          *ulCqiInfo
)
#else
PUBLIC Void rgSCHCmnUlCqiInd(cell, ue, ulCqiInfo)
RgSchCellCb          *cell;
RgSchUeCb            *ue;
TfuUlCqiRpt          *ulCqiInfo;
#endif
{
   RgSchCmnUlUe  *ueUl = RG_SCH_CMN_GET_UL_UE(ue);
   RgSchCmnCell  *cellSch = RG_SCH_CMN_GET_CELL(cell);
#ifdef UL_LA
   U8            iTbsNew;
   S32           previTbs;
#endif
#ifdef TENB_STATS
   CmLteUeCategory ueCtg = (CmLteUeCategory)(RG_SCH_CMN_GET_UE_CTGY(ue));
#endif   
   TRC2(rgSCHCmnUlCqiInd);
   /*  consider inputs from SRS handlers about SRS occassions
    * in determining the UL TX Antenna selection */
   ueUl->crntUlCqi[0] = ulCqiInfo->wideCqi;
#ifdef TFU_UPGRADE
   ueUl->validUlCqi = ueUl->crntUlCqi[0];
   ue->validTxAnt = 0;
#ifdef UL_LA
   iTbsNew  =  rgSchCmnUlCqiToTbsTbl[cell->isCpUlExtend][ueUl->validUlCqi];
   previTbs =  (ueUl->ulLaCb.cqiBasediTbs + ueUl->ulLaCb.deltaiTbs)/100;

   if (RG_ITBS_DIFF(iTbsNew, previTbs) > 5)
   {
      /* Ignore this iTBS report and mark that last iTBS report was */
      /* ignored so that subsequently we reset the LA algorithm     */
      ueUl->ulLaCb.lastiTbsIgnored = TRUE;
   }
   else
   {
      if (ueUl->ulLaCb.lastiTbsIgnored != TRUE)
      {
         ueUl->ulLaCb.cqiBasediTbs = ((20 * iTbsNew * 100) +
                                       (80 * ueUl->ulLaCb.cqiBasediTbs))/100;
      }
      else
      {
         /* Reset the LA as iTbs in use caught up with the value   */
         /* reported by UE.                                        */
         ueUl->ulLaCb.cqiBasediTbs = ((20 * iTbsNew * 100) +
                                        (80 * previTbs * 100))/100;
         ueUl->ulLaCb.deltaiTbs = 0;
         ueUl->ulLaCb.lastiTbsIgnored = FALSE;
      }
   }
#endif 
#endif
   rgSCHPwrUlCqiInd(cell, ue);
#ifdef LTEMAC_SPS
   if (ue->ul.ulSpsCfg.isUlSpsEnabled == TRUE)
   {
      rgSCHCmnSpsUlCqiInd(cell, ue);
   }
#endif
   /* Applicable to only some schedulers */
   cellSch->apisUl->rgSCHUlCqiInd(cell, ue, ulCqiInfo);

#ifdef TENB_STATS
   {
      ue->tenbStats->stats.sch.ulSumCqi += rgSCHCmnUlGetCqi(ue, ueCtg);
      ue->tenbStats->stats.sch.ulNumCqi ++;
      cell->tenbStats->sch.ulSumCqi += rgSCHCmnUlGetCqi(ue, ueCtg);
      cell->tenbStats->sch.ulNumCqi ++;
   }
#endif

   RETVOID;
}  /* rgSCHCmnUlCqiInd */

/**
 * @brief Returns HARQ proc for which data expected now.
 *
 * @details
 *
 *     Function: rgSCHCmnUlHqProcForUe
 *     Purpose:  This function returns the harq process for
 *               which data is expected in the current subframe.
 *               It does not validate that the HARQ process
 *               has an allocation.
 *
 *     Invoked by: TOM
 *
 *  @param[in]  RgSchCellCb        *cell
 *  @param[in]  CmLteTimingInfo    frm
 *  @param[in]  RgSchUeCb          *ue
 *  @param[out] RgSchUlHqProcCb    **procRef
 *  @return  Void
 **/
#ifdef ANSI
PUBLIC Void rgSCHCmnUlHqProcForUe
(
RgSchCellCb         *cell,
CmLteTimingInfo     frm,
RgSchUeCb           *ue,
RgSchUlHqProcCb     **procRef
)
#else
PUBLIC Void rgSCHCmnUlHqProcForUe(cell, frm, ue, procRef)
RgSchCellCb         *cell;
CmLteTimingInfo     frm;
RgSchUeCb           *ue;
RgSchUlHqProcCb     **procRef;
#endif
{
   U8 procId = rgSCHCmnGetUlHqProcIdx(&frm, cell);
   TRC2(rgSCHCmnUlHqProcForUe);
   *procRef = rgSCHUhmGetUlHqProc(ue, procId);
   RETVOID;
}

#ifdef RG_UNUSED
/**
 * @brief Update harq process for allocation.
 *
 * @details
 *
 *     Function : rgSCHCmnUpdUlHqProc
 *
 *     This function is invoked when harq process
 *     control block is now in a new memory location
 *     thus requiring a pointer/reference update.
 *
 *  @param[in] RgSchCellCb      *cell
 *  @param[in] RgSchUlHqProcCb  *curProc
 *  @param[in] RgSchUlHqProcCb  *oldProc
 *  @return  S16
 *      -# ROK
 *      -# RFAILED
 **/
#ifdef ANSI
PUBLIC S16 rgSCHCmnUpdUlHqProc
(
RgSchCellCb      *cell,
RgSchUlHqProcCb  *curProc,
RgSchUlHqProcCb  *oldProc
)
#else
PUBLIC S16 rgSCHCmnUpdUlHqProc(cell, curProc, oldProc)
RgSchCellCb      *cell;
RgSchUlHqProcCb  *curProc;
RgSchUlHqProcCb  *oldProc;
#endif
{
   TRC2(rgSCHCmnUpdUlHqProc);

   UNUSED(cell);
   UNUSED(oldProc);
#if (ERRCLASS & ERRCLS_DEBUG)
   if (curProc->alloc == NULLP)
   {
      RETVALUE(RFAILED);
   }
#endif
   curProc->alloc->hqProc = curProc;
   RETVALUE(ROK);
}  /* rgSCHCmnUpdUlHqProc */
#endif

/*MS_WORKAROUND for CR FIXME */
/**
 * @brief Hsndles BSR timer expiry
 *
 * @details
 *
 *     Function : rgSCHCmnBsrTmrExpry
 *
 *     This function is invoked when periodic BSR timer expires for a UE.
 *
 *  @param[in] RgSchUeCb        *ue
 *  @return  S16
 *      -# ROK
 *      -# RFAILED
 **/
#ifdef ANSI
PUBLIC S16 rgSCHCmnBsrTmrExpry
(
RgSchUeCb  *ueCb
)
#else
PUBLIC S16 rgSCHCmnBsrTmrExpry(ueCb)
RgSchUeCb  *ueCb;
#endif
{
   RgSchCmnCell *cellSch = RG_SCH_CMN_GET_CELL(ueCb->cell);

   TRC2(rgSCHCmnBsrTmrExpry)

   ueCb->isSrGrant = TRUE;
   /* Schedule UE for token Allocation */
   cellSch->apisUl->rgSCHSrRcvd(ueCb->cell, ueCb);

   RETVALUE (ROK);
}

/**
 * @brief Short BSR update.
 *
 * @details
 *
 *     Function : rgSCHCmnUpdBsrShort
 *
 *     This functions does requisite updates to handle short BSR reporting.
 *
 *  @param[in]  RgSchCellCb  *cell
 *  @param[in]  RgSchUeCb    *ue
 *  @param[in]  RgSchLcgCb *ulLcg
 *  @param[in]  U8           bsr
 *  @param[out] RgSchErrInfo *err
 *  @return  S16
 *      -# ROK
 *      -# RFAILED
 **/
#ifdef ANSI
PUBLIC S16 rgSCHCmnUpdBsrShort
(
RgSchCellCb  *cell,
RgSchUeCb    *ue,
RgSchLcgCb *ulLcg,
U8           bsr,
RgSchErrInfo *err
)
#else
PUBLIC S16 rgSCHCmnUpdBsrShort(cell, ue, ulLcg, bsr, err)
RgSchCellCb  *cell;
RgSchUeCb    *ue;
RgSchLcgCb *ulLcg;
U8           bsr;
RgSchErrInfo *err;
#endif
{
   U8  lcgCnt;
   RgSchCmnUlUe *ueUl = RG_SCH_CMN_GET_UL_UE(ue);
   RgSchCmnCell *cellSch = RG_SCH_CMN_GET_CELL(cell);
   RgSchCmnLcg  *cmnLcg  = NULLP;

#ifdef LTE_L2_MEAS
   U8             idx;
#endif
   TRC2(rgSCHCmnUpdBsrShort);

   if (!RGSCH_LCG_ISCFGD(ulLcg))
   {
      err->errCause = RGSCHERR_SCH_LCG_NOT_CFGD;
      RETVALUE(RFAILED);
   }
   for (lcgCnt=0; lcgCnt<4; lcgCnt++)
   {
#ifdef LTE_L2_MEAS
      /* Set BS of all other LCGs to Zero.
         If Zero BSR is reported in Short BSR include this LCG too */
      if ((lcgCnt != ulLcg->lcgId) ||
            (!bsr && !ue->ul.hqEnt.numBusyHqProcs))
      {
         /* If old BO is zero do nothing */
         if(((RgSchCmnLcg *)(ue->ul.lcgArr[lcgCnt].sch))->bs != 0)
         {
            for(idx = 0; idx < ue->ul.lcgArr[lcgCnt].numLch; idx++)
            {
               if((ue->ul.lcgArr[lcgCnt].lcArray[idx]->qciCb->ulUeCount) &&
                 (ue->ulActiveLCs & (1 << 
                  (ue->ul.lcgArr[lcgCnt].lcArray[idx]->qciCb->qci -1))))
               {
          /* L2_COUNTER */
                 ue->ul.lcgArr[lcgCnt].lcArray[idx]->qciCb->ulUeCount--;
                 ue->ulActiveLCs &= ~(1 << 
                  (ue->ul.lcgArr[lcgCnt].lcArray[idx]->qciCb->qci -1));
               }
            }
         }
      }
#endif
      if (RGSCH_LCG_ISCFGD(&ue->ul.lcgArr[lcgCnt]))
      {
         ((RgSchCmnLcg *)(ue->ul.lcgArr[lcgCnt].sch))->bs = 0;
         ((RgSchCmnLcg *)(ue->ul.lcgArr[lcgCnt].sch))->reportedBs = 0;
      }
   }

#ifdef LTE_L2_MEAS
   if(ulLcg->lcgId && bsr && (((RgSchCmnLcg *)(ulLcg->sch))->bs == 0))
   {
      for(idx = 0; idx < ulLcg->numLch; idx++)
      {
          /* L2_COUNTER */
          if (!(ue->ulActiveLCs & (1 << (ulLcg->lcArray[idx]->qciCb->qci -1))))
          {
             ulLcg->lcArray[idx]->qciCb->ulUeCount++;
             ue->ulActiveLCs |= (1 << (ulLcg->lcArray[idx]->qciCb->qci -1));
          }
      }
   }
#endif
   /* Resetting the nonGbrLcgBs info here */
   ueUl->nonGbrLcgBs = 0;
   ueUl->nonLcg0Bs = 0;

   cmnLcg = ((RgSchCmnLcg *)(ulLcg->sch));
   cmnLcg->reportedBs = rgSchCmnBsrTbl[bsr];
   if (RGSCH_IS_GBR_BEARER(cmnLcg->cfgdGbr))
   {
      /* TBD check for effGbr != 0 */    
      cmnLcg->bs = RGSCH_MIN(cmnLcg->reportedBs, cmnLcg->effGbr + cmnLcg->effDeltaMbr);
   }
   else if (0 == ulLcg->lcgId) 
   {
      /* This is added for handling LCG0 */
      cmnLcg->bs = cmnLcg->reportedBs;
   }
   else 
   {
      /* Update non GBR LCG's BS*/
      ueUl->nonGbrLcgBs = RGSCH_MIN(cmnLcg->reportedBs,ueUl->effAmbr);
      cmnLcg->bs     = ueUl->nonGbrLcgBs;
   }
   ueUl->totalBsr = cmnLcg->bs;

#ifdef RGR_V1
   if ((ue->bsrTmr.tmrEvnt != TMR_NONE) && (bsr == 0))
   {
      rgSCHTmrStopTmr(cell, ue->bsrTmr.tmrEvnt, ue);
   }
#endif
#ifdef LTEMAC_SPS
   if (ue->ul.ulSpsCfg.isUlSpsEnabled == TRUE)
   {
      rgSCHCmnSpsBsrRpt(cell, ue, ulLcg);
   }
#endif
   rgSCHCmnUpdUlCompEffBsr(ue);

   cellSch->apisUl->rgSCHUpdBsrShort(cell, ue, ulLcg, bsr);

   RETVALUE(ROK);
}

/**
 * @brief Truncated BSR update.
 *
 * @details
 *
 *     Function : rgSCHCmnUpdBsrTrunc
 *
 *     This functions does required updates to handle truncated BSR report.
 *
 *
 *  @param[in]  RgSchCellCb  *cell
 *  @param[in]  RgSchUeCb    *ue
 *  @param[in]  RgSchLcgCb *ulLcg
 *  @param[in]  U8           bsr
 *  @param[out] RgSchErrInfo *err
 *  @return  S16
 *      -# ROK
 *      -# RFAILED
 **/
#ifdef ANSI
PUBLIC S16 rgSCHCmnUpdBsrTrunc
(
RgSchCellCb  *cell,
RgSchUeCb    *ue,
RgSchLcgCb *ulLcg,
U8           bsr,
RgSchErrInfo *err
)
#else
PUBLIC S16 rgSCHCmnUpdBsrTrunc(cell, ue, ulLcg, bsr, err)
RgSchCellCb  *cell;
RgSchUeCb    *ue;
RgSchLcgCb *ulLcg;
U8           bsr;
RgSchErrInfo *err;
#endif
{
   RgSchCmnCell *cellSch = RG_SCH_CMN_GET_CELL(cell);
   RgSchCmnUlUe *ueUl = RG_SCH_CMN_GET_UL_UE(ue);
   RgSchCmnLcg  *cmnLcg = NULLP;
   S32          cnt;
#ifdef LTE_L2_MEAS
   U8     idx;
#endif

   TRC2(rgSCHCmnUpdBsrTrunc);

   if (!RGSCH_LCG_ISCFGD(ulLcg))
   {
      err->errCause = RGSCHERR_SCH_LCG_NOT_CFGD;
      RETVALUE(RFAILED);
   }
   /* set all higher prio lcgs bs to 0 and update this lcgs bs and
      total bsr= sumofall lcgs bs */
   if (ulLcg->lcgId)
   {
      for (cnt = ulLcg->lcgId-1; cnt >= 0; cnt--)
      {
#ifdef LTE_L2_MEAS
         /* If Existing BO is zero the don't do anything */
         if(((RgSchCmnLcg *)(ue->ul.lcgArr[cnt].sch))->bs != 0)
         {
            for(idx = 0; idx < ue->ul.lcgArr[cnt].numLch; idx++)
            {
               /* L2_COUNTERS */
               if((ue->ul.lcgArr[cnt].lcArray[idx]->qciCb->ulUeCount) &&
                     (ue->ulActiveLCs & (1 << 
                                         (ue->ul.lcgArr[cnt].lcArray[idx]->qciCb->qci -1))))
               {
                  ue->ul.lcgArr[cnt].lcArray[idx]->qciCb->ulUeCount--;
                  ue->ulActiveLCs &= ~(1 << 
                        (ue->ul.lcgArr[cnt].lcArray[idx]->qciCb->qci -1));
               }
            }
         }
#endif
         ((RgSchCmnLcg *)(ue->ul.lcgArr[cnt].sch))->bs = 0;
         ((RgSchCmnLcg *)(ue->ul.lcgArr[cnt].sch))->reportedBs = 0;
      }
   }

#ifdef LTE_L2_MEAS
   for (cnt = ulLcg->lcgId; cnt < RGSCH_MAX_LCG_PER_UE; cnt++)
   {
      if (ulLcg->lcgId == 0)
      {
         continue;
      }
      /* If Existing BO is zero the don't do anything */
      if(((RgSchCmnLcg *)(ue->ul.lcgArr[cnt].sch))->bs == 0)
      {
         for(idx = 0; idx < ue->ul.lcgArr[cnt].numLch; idx++)
         {
            /* L2_COUNTERS */
            if (!(ue->ulActiveLCs & (1 << 
               (ue->ul.lcgArr[cnt].lcArray[idx]->qciCb->qci -1))))
            {
               ue->ul.lcgArr[cnt].lcArray[idx]->qciCb->ulUeCount++;
               ue->ulActiveLCs |= (1 << 
                     (ue->ul.lcgArr[cnt].lcArray[idx]->qciCb->qci -1));
            }
         }
      }
   }
#endif
   ueUl->nonGbrLcgBs = 0;
   ueUl->nonLcg0Bs = 0;
   cmnLcg = ((RgSchCmnLcg *)(ulLcg->sch));
   cmnLcg->reportedBs = rgSchCmnBsrTbl[bsr];
   if (RGSCH_IS_GBR_BEARER(cmnLcg->cfgdGbr))
   {
      cmnLcg->bs = RGSCH_MIN(cmnLcg->reportedBs, cmnLcg->effGbr + cmnLcg->effDeltaMbr);
   }
   else if(ulLcg->lcgId == 0)
   {
      /* This is for handeling LCG0 */
      cmnLcg->bs = cmnLcg->reportedBs;
   }
   else
   {
      ueUl->nonGbrLcgBs = RGSCH_MIN(cmnLcg->reportedBs, ueUl->effAmbr);
      cmnLcg->bs = ueUl->nonGbrLcgBs;
   }
   ueUl->totalBsr = cmnLcg->bs;

   for (cnt = ulLcg->lcgId+1; cnt < RGSCH_MAX_LCG_PER_UE; cnt++)
   {
      /* TODO: The bs for the other LCGs may be stale because some or all of
       * the part of bs may have been already scheduled/data received. Please 
       * consider this when truncated BSR is tested/implemented */
      ueUl->totalBsr += ((RgSchCmnLcg *)(ue->ul.lcgArr[cnt].sch))->bs;
   }

   rgSCHCmnUpdUlCompEffBsr(ue);

   cellSch->apisUl->rgSCHUpdBsrTrunc(cell, ue, ulLcg, bsr);

   RETVALUE(ROK);
}

/**
 * @brief Long BSR update.
 *
 * @details
 *
 *     Function : rgSCHCmnUpdBsrLong
 *
 *     - Update BSRs for all configured LCGs.
 *     - Update priority of LCGs if needed.
 *     - Update UE's position within/across uplink scheduling queues.
 *
 *
 *  @param[in]  RgSchCellCb  *cell
 *  @param[in]  RgSchUeCb    *ue
 *  @param[in]  U8 bsArr[]
 *  @param[out] RgSchErrInfo *err
 *  @return  S16
 *      -# ROK
 *      -# RFAILED
 **/
#ifdef ANSI
PUBLIC S16 rgSCHCmnUpdBsrLong
(
RgSchCellCb  *cell,
RgSchUeCb    *ue,
U8           *bsArr,
RgSchErrInfo *err
)
#else
PUBLIC S16 rgSCHCmnUpdBsrLong(cell, ue, bsArr, err)
RgSchCellCb  *cell;
RgSchUeCb    *ue;
U8           *bsArr;
RgSchErrInfo *err;
#endif
{
   RgSchCmnCell *cellSch = RG_SCH_CMN_GET_CELL(cell);
   RgSchCmnUlUe *ueUl = RG_SCH_CMN_GET_UL_UE(ue);
   U32           tmpBsArr[4] = {0, 0, 0, 0};
   U32           nonGbrBs = 0;
#ifdef LTE_L2_MEAS
   U8            idx1;
   U8            idx2;
#endif
   U32           lcgId;

   TRC2(rgSCHCmnUpdBsrLong);

#ifdef LTE_L2_MEAS
   for(idx1 = 1; idx1 < RGSCH_MAX_LCG_PER_UE; idx1++)
   {
     /* If Old BO is non zero then do nothing */
     if ((((RgSchCmnLcg *)(ue->ul.lcgArr[idx1].sch))->bs == 0)
        && bsArr[idx1] )
     {
       for(idx2 = 0; idx2 < ue->ul.lcgArr[idx1].numLch; idx2++)
       {
          /* L2_COUNTERS */
          if (!(ue->ulActiveLCs & (1 << 
             (ue->ul.lcgArr[idx1].lcArray[idx2]->qciCb->qci -1))))
          {
             ue->ul.lcgArr[idx1].lcArray[idx2]->qciCb->ulUeCount++;
             ue->ulActiveLCs |= (1 << 
               (ue->ul.lcgArr[idx1].lcArray[idx2]->qciCb->qci -1));
          }
       }
     }
   }
#endif
   ueUl->nonGbrLcgBs = 0;
   ueUl->nonLcg0Bs = 0;

   if (RGSCH_LCG_ISCFGD(&ue->ul.lcgArr[0]))
   {
      ((RgSchCmnLcg *)(ue->ul.lcgArr[0].sch))->bs = rgSchCmnBsrTbl[bsArr[0]];
      ((RgSchCmnLcg *)(ue->ul.lcgArr[0].sch))->reportedBs = rgSchCmnBsrTbl[bsArr[0]];
      tmpBsArr[0] = rgSchCmnBsrTbl[bsArr[0]];
   }
   for (lcgId = 1; lcgId < RGSCH_MAX_LCG_PER_UE; lcgId++)
   {
      if (RGSCH_LCG_ISCFGD(&ue->ul.lcgArr[lcgId]))
      {
         RgSchCmnLcg *cmnLcg = ((RgSchCmnLcg *)(ue->ul.lcgArr[lcgId].sch));

         cmnLcg->reportedBs = rgSchCmnBsrTbl[bsArr[lcgId]];
         if (RGSCH_IS_GBR_BEARER(cmnLcg->cfgdGbr))
         {
            cmnLcg->bs = RGSCH_MIN(cmnLcg->reportedBs, cmnLcg->effGbr + cmnLcg->effDeltaMbr);
            tmpBsArr[lcgId] = cmnLcg->bs;
         }
         else
         {
            nonGbrBs += cmnLcg->reportedBs;
            tmpBsArr[lcgId] = cmnLcg->reportedBs;
            cmnLcg->bs = RGSCH_MIN(cmnLcg->reportedBs,ueUl->effAmbr);
         }
      }
   }
   ueUl->nonGbrLcgBs = RGSCH_MIN(nonGbrBs,ueUl->effAmbr);

   ueUl->totalBsr = tmpBsArr[0] + tmpBsArr[1] + tmpBsArr[2] + tmpBsArr[3];
#ifdef RGR_V1
   if ((ue->bsrTmr.tmrEvnt != TMR_NONE) && (ueUl->totalBsr == 0))
   {
      rgSCHTmrStopTmr(cell, ue->bsrTmr.tmrEvnt, ue);
   }
#endif

#ifdef LTEMAC_SPS
   if (ue->ul.ulSpsCfg.isUlSpsEnabled == TRUE) /* SPS_FIX */
   {
     if(ueUl->totalBsr - tmpBsArr[1] == 0)
     {/* Updaing the BSR to SPS only if LCG1 BS is present in sps active state */
        rgSCHCmnSpsBsrRpt(cell, ue, &ue->ul.lcgArr[1]);
     }
   }
#endif
   rgSCHCmnUpdUlCompEffBsr(ue);

   cellSch->apisUl->rgSCHUpdBsrLong(cell, ue, bsArr);

   RETVALUE(ROK);
}

/**
 * @brief PHR update.
 *
 * @details
 *
 *     Function : rgSCHCmnUpdPhr
 *
 *     Updates power headroom information for an UE.
 *
 *  @param[in]  RgSchCellCb  *cell
 *  @param[in]  RgSchUeCb    *ue
 *  @param[in]  U8           phr
 *  @param[out] RgSchErrInfo *err
 *  @return  S16
 *      -# ROK
 *      -# RFAILED
 **/
#ifdef ANSI
PUBLIC S16 rgSCHCmnUpdPhr
(
RgSchCellCb    *cell,
RgSchUeCb      *ue,
U8             phr,
RgSchErrInfo   *err
)
#else
PUBLIC S16 rgSCHCmnUpdPhr(cell, ue, phr, err)
RgSchCellCb    *cell;
RgSchUeCb      *ue;
U8             phr;
RgSchErrInfo   *err;
#endif
{
   RgSchCmnUlUe        *ueUl = RG_SCH_CMN_GET_UL_UE(ue);
   RgSchCmnAllocRecord *allRcd;
   CmLList             *node = ueUl->ulAllocLst.last;

#ifdef LTEMAC_SPS
   RgSchCmnUlUeSpsInfo   *ulSpsUe = RG_SCH_CMN_GET_UL_SPS_UE(ue);
#endif
   TRC2(rgSCHCmnUpdPhr);

   UNUSED(err);

   while (node)
   {
      allRcd = (RgSchCmnAllocRecord *)node->node;
      node = node->prev;
      if (RGSCH_TIMEINFO_SAME(ue->macCeRptTime, allRcd->allocTime))
      {
         rgSCHPwrUpdPhr(cell, ue, phr, allRcd);
         break;
      }
   }
#ifdef LTEMAC_SPS
   if(ulSpsUe->isUlSpsActv)
   {
      rgSCHCmnSpsPhrInd(cell,ue);
   }
#endif

   RETVALUE(ROK);
}  /* rgSCHCmnUpdPhr */

/**
 * @brief UL grant for contention resolution.
 *
 * @details
 *
 *     Function : rgSCHCmnContResUlGrant
 *
 *     Add UE to another queue specifically for CRNTI based contention
 *     resolution.
 *
 *
 *  @param[in]  RgSchUeCb    *ue
 *  @param[out] RgSchErrInfo *err
 *  @return  S16
 *      -# ROK
 *      -# RFAILED
 **/
#ifdef ANSI
PUBLIC S16 rgSCHCmnContResUlGrant
(
RgSchCellCb  *cell,
RgSchUeCb    *ue,
RgSchErrInfo *err
)
#else
PUBLIC S16 rgSCHCmnContResUlGrant(cell, ue, err)
RgSchCellCb  *cell;
RgSchUeCb    *ue;
RgSchErrInfo *err;
#endif
{
   RgSchCmnCell *cellSch = RG_SCH_CMN_GET_CELL(cell);
   TRC2(rgSCHCmnContResUlGrant);

   cellSch->apisUl->rgSCHContResUlGrant(cell, ue);

   RETVALUE(ROK);
}

/**
 * @brief SR reception handling.
 *
 * @details
 *
 *     Function : rgSCHCmnSrRcvd
 *
 *     - Update UE's position within/across uplink scheduling queues
 *     - Update priority of LCGs if needed.
 *
 *  @param[in]  RgSchCellCb  *cell
 *  @param[in]  RgSchUeCb    *ue
 *  @param[in]  CmLteTimingInfo frm
 *  @param[out] RgSchErrInfo *err
 *  @return  S16
 *      -# ROK
 *      -# RFAILED
 **/
#ifdef ANSI
PUBLIC S16 rgSCHCmnSrRcvd
(
RgSchCellCb  *cell,
RgSchUeCb    *ue,
CmLteTimingInfo frm,
RgSchErrInfo *err
)
#else
PUBLIC S16 rgSCHCmnSrRcvd(cell, ue, frm, err)
RgSchCellCb  *cell;
RgSchUeCb    *ue;
CmLteTimingInfo frm;
RgSchErrInfo *err;
#endif
{
   RgSchCmnCell *cellSch = RG_SCH_CMN_GET_CELL(cell);
   RgSchCmnUlUe *ueUl    = RG_SCH_CMN_GET_UL_UE(ue);
   CmLList      *node    = ueUl->ulAllocLst.last;

   TRC2(rgSCHCmnSrRcvd);

   RGSCH_INCR_SUB_FRAME(frm, 1); /* 1 TTI after the time SR was sent */
   while (node)
   {
      RgSchCmnAllocRecord *allRcd = (RgSchCmnAllocRecord *)node->node;
      if (RGSCH_TIMEINFO_SAME(frm, allRcd->allocTime))
      {
         break;
      }
      node = node->prev;
   }
   if (node)
   {
      /* There was some allocation after SR was sent, so don't do
       * anything further. Not considering wrap-around (unlikely). */
      RETVALUE(ROK);
   }

   ue->isSrGrant = TRUE;
   cellSch->apisUl->rgSCHSrRcvd(cell, ue);
   
   RETVALUE(ROK);
}

/**
 * @brief Returns first uplink allocation to send reception
 *        request to PHY.
 *
 * @details
 *
 *     Function: rgSCHCmnFirstRcptnReq(cell)
 *     Purpose:  This function returns the first uplink allocation
 *               (or NULLP if there is none) in the subframe
 *               in which is expected to prepare and send reception
 *               request to PHY.
 *
 *     Invoked by: TOM
 *
 *  @param[in]  RgSchCellCb      *cell
 *  @return  RgSchUlAlloc*
 **/
#ifdef ANSI
PUBLIC RgSchUlAlloc *rgSCHCmnFirstRcptnReq
(
RgSchCellCb      *cell
)
#else
PUBLIC RgSchUlAlloc *rgSCHCmnFirstRcptnReq(cell)
RgSchCellCb      *cell;
#endif
{
   RgSchCmnUlCell *cellUl = RG_SCH_CMN_GET_UL_CELL(cell);
/* ACC_TDD */
#if 0
#else
   RgSchUlAlloc* alloc = NULLP;
#endif

   TRC2(rgSCHCmnFirstRcptnReq);

   if (cellUl->rcpReqIdx != RGSCH_INVALID_INFO)
   {
      RgSchUlSf* sf = &cellUl->ulSfArr[cellUl->rcpReqIdx];
      alloc = rgSCHUtlUlAllocFirst(sf);

      if (alloc && alloc->hqProc == NULLP)
      {
         alloc = rgSCHUtlUlAllocNxt(sf, alloc);
      }
   }

   RETVALUE(alloc);
}

/**
 * @brief Returns first uplink allocation to send reception
 *        request to PHY.
 *
 * @details
 *
 *     Function: rgSCHCmnNextRcptnReq(cell)
 *     Purpose:  This function returns the next uplink allocation
 *               (or NULLP if there is none) in the subframe
 *               in which is expected to prepare and send reception
 *               request to PHY.
 *
 *     Invoked by: TOM
 *
 *  @param[in]  RgSchCellCb      *cell
 *  @return  RgSchUlAlloc*
 **/
#ifdef ANSI
PUBLIC RgSchUlAlloc *rgSCHCmnNextRcptnReq
(
RgSchCellCb      *cell,
RgSchUlAlloc     *alloc
)
#else
PUBLIC RgSchUlAlloc *rgSCHCmnNextRcptnReq(cell, alloc)
RgSchCellCb      *cell;
RgSchUlAlloc     *alloc;
#endif
{
   RgSchCmnUlCell *cellUl = RG_SCH_CMN_GET_UL_CELL(cell);
/* ACC-TDD */
   //RgSchUlSf      *sf   = &cellUl->ulSfArr[cellUl->rcpReqIdx];

   TRC2(rgSCHCmnNextRcptnReq);
/* ACC-TDD */
   if (cellUl->rcpReqIdx != RGSCH_INVALID_INFO)
   {
      RgSchUlSf *sf = &cellUl->ulSfArr[cellUl->rcpReqIdx];

      alloc = rgSCHUtlUlAllocNxt(sf, alloc);
      if (alloc && alloc->hqProc == NULLP)
      {
         alloc = rgSCHUtlUlAllocNxt(sf, alloc);
      }
   }
   else
   {
      alloc = NULLP;
   }

   RETVALUE(alloc);
}
/**
 * @brief Collates DRX enabled UE's scheduled in this SF
 *
 * @details
 *
 *     Function: rgSCHCmnDrxStrtInActvTmrInUl(cell)
 *     Purpose:  This function collates the link
 *               of UE's scheduled in this SF who
 *               have drx enabled. It then calls
 *               DRX specific function to start/restart
 *               inactivity timer in Ul
 *
 *     Invoked by: TOM
 *
 *  @param[in]  RgSchCellCb      *cell
 *  @return Void
 **/
#ifdef ANSI
PUBLIC Void rgSCHCmnDrxStrtInActvTmrInUl
(
RgSchCellCb      *cell
)
#else
PUBLIC Void rgSCHCmnDrxStrtInActvTmrInUl(cell)
RgSchCellCb      *cell;
#endif
{
   RgSchCmnUlCell *cellUl = RG_SCH_CMN_GET_UL_CELL(cell);
   RgSchUlSf      *sf     = &(cellUl->ulSfArr[cellUl->schdIdx]);
   RgSchUlAlloc   *alloc  = rgSCHUtlUlAllocFirst(sf);
   CmLListCp       ulUeLst;
   RgSchUeCb       *ueCb;


   TRC2(rgSCHCmnDrxStrtInActvTmrInUl);

   cmLListInit(&ulUeLst);

   while(alloc)
   {
      ueCb = alloc->ue;

      if (ueCb)
      {
         if (!(alloc->grnt.isRtx) && ueCb->isDrxEnabled && !(ueCb->isSrGrant)
#ifdef LTEMAC_SPS
             /* ccpu00139513- DRX inactivity timer should not be started for 
              * UL SPS occasions */
             && (alloc->hqProc->isSpsOccnHqP == FALSE) 
#endif
             )
         {
            cmLListAdd2Tail(&ulUeLst,&(ueCb->ulDrxInactvTmrLnk));
            ueCb->ulDrxInactvTmrLnk.node = (PTR)ueCb;
         }
      }

      alloc = rgSCHUtlUlAllocNxt(sf, alloc);
   }/*while(alloc)*/

   (Void)rgSCHDrxStrtInActvTmr(cell,&ulUeLst,RG_SCH_DRX_UL);

   RETVOID;
}


/**
 * @brief Returns first uplink allocation to send HARQ feedback
 *        request to PHY.
 *
 * @details
 *
 *     Function: rgSCHCmnFirstHqFdbkAlloc
 *     Purpose:  This function returns the first uplink allocation
 *               (or NULLP if there is none) in the subframe
 *               for which it is expected to prepare and send HARQ
 *               feedback to PHY.
 *
 *     Invoked by: TOM
 *
 *  @param[in]  RgSchCellCb      *cell
 *  @param[in]  U8               idx
 *  @return  RgSchUlAlloc*
 **/
#ifdef ANSI
PUBLIC RgSchUlAlloc *rgSCHCmnFirstHqFdbkAlloc
(
RgSchCellCb      *cell,
U8               idx 
)
#else
PUBLIC RgSchUlAlloc *rgSCHCmnFirstHqFdbkAlloc(cell, idx)
RgSchCellCb      *cell;
U8               idx;
#endif
{
   RgSchCmnUlCell *cellUl = RG_SCH_CMN_GET_UL_CELL(cell);
/* ACC-TDD */
#if 0
#else
   RgSchUlAlloc  *alloc = NULLP;
#endif

   TRC2(rgSCHCmnFirstHqFdbkAlloc);

   if (cellUl->hqFdbkIdx[idx] != RGSCH_INVALID_INFO)
   {
     RgSchUlSf *sf = &cellUl->ulSfArr[cellUl->hqFdbkIdx[idx]];
     alloc    = rgSCHUtlUlAllocFirst(sf);

     while (alloc && (alloc->hqProc == NULLP))
     {
        alloc = rgSCHUtlUlAllocNxt(sf, alloc);
     }
   }

   RETVALUE(alloc);
}

/**
 * @brief Returns next allocation to send HARQ feedback for.
 *
 * @details
 *
 *     Function: rgSCHCmnNextHqFdbkAlloc(cell)
 *     Purpose:  This function returns the next uplink allocation
 *               (or NULLP if there is none) in the subframe
 *               for which HARQ feedback needs to be sent.
 *
 *     Invoked by: TOM
 *
 *  @param[in]  RgSchCellCb      *cell
 *  @return  RgSchUlAlloc*
 **/
#ifdef ANSI
PUBLIC RgSchUlAlloc *rgSCHCmnNextHqFdbkAlloc
(
RgSchCellCb      *cell,
RgSchUlAlloc     *alloc,
U8               idx 
)
#else
PUBLIC RgSchUlAlloc *rgSCHCmnNextHqFdbkAlloc(cell, alloc, idx)
RgSchCellCb      *cell;
RgSchUlAlloc     *alloc;
U8               idx; 
#endif
{
   RgSchCmnUlCell *cellUl = RG_SCH_CMN_GET_UL_CELL(cell);
   TRC2(rgSCHCmnNextHqFdbkAlloc);

   if (cellUl->hqFdbkIdx[idx] != RGSCH_INVALID_INFO)
   {
      RgSchUlSf *sf = &cellUl->ulSfArr[cellUl->hqFdbkIdx[idx]];

      alloc = rgSCHUtlUlAllocNxt(sf, alloc);
      while (alloc && (alloc->hqProc == NULLP))
      {
         alloc = rgSCHUtlUlAllocNxt(sf, alloc);
      }
   }
   else
   {
     alloc = NULLP;
   }
   RETVALUE(alloc);
}

/***********************************************************
 *
 *     Func : rgSCHCmnUlGetITbsFrmIMcs
 *
 *     Desc : Returns the Itbs that is mapped to an Imcs
 *            for the case of uplink.
 *
 *     Ret  :
 *
 *     Notes:
 *
 *     File :
 *
 **********************************************************/
#ifdef ANSI
PUBLIC U8 rgSCHCmnUlGetITbsFrmIMcs
(
U8          iMcs
)
#else
PUBLIC U8 rgSCHCmnUlGetITbsFrmIMcs(iMcs)
U8          iMcs;
#endif
{
   TRC2(rgSCHCmnUlGetITbsFrmIMcs);

   RETVALUE(rgUlIMcsTbl[iMcs].iTbs);
}

/***********************************************************
 *
 *     Func : rgSCHCmnUlGetIMcsFrmITbs
 *
 *     Desc : Returns the Imcs that is mapped to an Itbs
 *            for the case of uplink.
 *
 *     Ret  :
 *
 *     Notes: For iTbs 19, iMcs is dependant on modulation order.
 *            Refer to 36.213, Table 8.6.1-1 and 36.306 Table 4.1-2
 *            for UE capability information
 *
 *     File :
 *
 **********************************************************/
#ifdef ANSI
PUBLIC U8 rgSCHCmnUlGetIMcsFrmITbs
(
U8                iTbs,
CmLteUeCategory   ueCtg
)
#else
PUBLIC U8 rgSCHCmnUlGetIMcsFrmITbs(iTbs, ueCtg)
U8                iTbs;
CmLteUeCategory   ueCtg;
#endif
{
   U8 iMcs;
   TRC2(rgSCHCmnUlGetIMcsFrmITbs);

   if (iTbs <= 10)
   {
      iMcs = iTbs;
   }
   /*a higher layer can force a 64QAM UE to transmit at 16QAM.
    * We currently do not support this. Once the support for such
    * is added, ueCtg should be replaced by current transmit
    * modulation configuration.Refer to 36.213 -8.6.1
    */
   else if ( iTbs < 19 )
   {
      iMcs = iTbs + 1;
   }
   else if ((iTbs == 19) && (ueCtg != CM_LTE_UE_CAT_5))
   {
      iMcs = iTbs + 1;
   }
   else
   {
      iMcs = iTbs + 2;
   }

   RETVALUE(iMcs);
}

/***********************************************************
 *
 *     Func : rgSCHCmnUlMinTbBitsForITbs
 *
 *     Desc : Returns the minimum number of bits that can
 *            be given as grant for a specific CQI.
 *
 *     Ret  :
 *
 *     Notes:
 *
 *     File :
 *
 **********************************************************/
#ifdef ANSI
PUBLIC U32 rgSCHCmnUlMinTbBitsForITbs
(
RgSchCmnUlCell     *cellUl,
U8                 iTbs
)
#else
PUBLIC U32 rgSCHCmnUlMinTbBitsForITbs(cellUl, iTbs)
RgSchCmnUlCell   *cellUl;
U8               iTbs;
#endif
{
   TRC2(rgSCHCmnUlMinTbBitsForITbs);

   RGSCH_ARRAY_BOUND_CHECK(0, rgTbSzTbl[0], iTbs); 

   RETVALUE(rgTbSzTbl[0][iTbs][cellUl->sbSize-1]);
}

/***********************************************************
 *
 *     Func : rgSCHCmnUlSbAlloc
 *
 *     Desc : Given a required 'number of subbands' and a hole,
 *            returns a suitable alloc such that the subband
 *            allocation size is valid
 *
 *     Ret  :
 *
 *     Notes: Does not assume either passed numSb or hole size
 *            to be valid for allocation, and hence arrives at
 *            an acceptable value.
 *     File :
 *
 **********************************************************/
#ifdef ANSI
PUBLIC RgSchUlAlloc *rgSCHCmnUlSbAlloc
(
RgSchUlSf       *sf,
U8              numSb,
RgSchUlHole     *hole
)
#else
PUBLIC RgSchUlAlloc *rgSCHCmnUlSbAlloc(sf, numSb, hole)
RgSchUlSf       *sf;
U8              numSb;
RgSchUlHole     *hole;
#endif
{
   U8           holeSz; /* valid hole size */
   RgSchUlAlloc *alloc;
   TRC2(rgSCHCmnUlSbAlloc);

   if ((holeSz = rgSchCmnMult235Tbl[hole->num].prvMatch) == hole->num)
   {
      numSb = rgSchCmnMult235Tbl[numSb].match;
      if (numSb >= holeSz)
      {
         alloc = rgSCHUtlUlAllocGetCompHole(sf, hole);
      }
      else
      {
         alloc = rgSCHUtlUlAllocGetPartHole(sf, numSb, hole);
      }
   }
   else
   {
      if (numSb < holeSz)
      {
         numSb = rgSchCmnMult235Tbl[numSb].match;
      }
      else
      {
         numSb = rgSchCmnMult235Tbl[numSb].prvMatch;
      }

      if ( numSb >= holeSz )
      {
         numSb = holeSz;
      }
      alloc = rgSCHUtlUlAllocGetPartHole(sf, numSb, hole);
   }
   RETVALUE(alloc);
}

/**
 * @brief To fill the RgSchCmnUeUlAlloc structure of UeCb.
 *
 * @details
 *
 *     Function: rgSCHCmnUlUeFillAllocInfo
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
PUBLIC Void rgSCHCmnUlUeFillAllocInfo
(
RgSchCellCb      *cell,
RgSchUeCb        *ue
)
#else
PUBLIC Void rgSCHCmnUlUeFillAllocInfo(cell, ue)
RgSchCellCb      *cell;
RgSchUeCb        *ue;
#endif
{
   RgSchCmnUlCell     *cellUl = RG_SCH_CMN_GET_UL_CELL(cell);
   RgSchCmnUeUlAlloc  *ulAllocInfo;
   RgSchCmnUlUe       *ueUl;

   TRC2(rgSCHCmnUlUeFillAllocInfo);

   ueUl = RG_SCH_CMN_GET_UL_UE(ue);
   ulAllocInfo = &ueUl->alloc;

   /* Fill alloc structure */
   rgSCHCmnUlAllocFillTpc(cell, ue, ulAllocInfo->alloc);
   rgSCHCmnUlAllocFillNdmrs(cellUl, ulAllocInfo->alloc);
   rgSCHCmnUlAllocLnkHqProc(ue, ulAllocInfo->alloc, ulAllocInfo->alloc->hqProc,
                     ulAllocInfo->alloc->hqProc->isRetx);
   /* Fill PDCCH */
   rgSCHCmnUlFillPdcchWithAlloc(ulAllocInfo->alloc->pdcch,
         ulAllocInfo->alloc, ue);
#ifdef LTEMAC_SPS
      /* ndi to be set as 1 for adaptive retransmission of SPS */
   if ((ulAllocInfo->alloc->hqProc->isRetx == TRUE)  && (ulAllocInfo->alloc->ue) &&  
        (ulAllocInfo->alloc->rnti == ulAllocInfo->alloc->ue->spsRnti))
   {
      ulAllocInfo->alloc->pdcch->dci.u.format0Info.ndi  = 1;
   }
#endif
   /* Recording information about this allocation */
   rgSCHCmnUlRecordUeAlloc(cell, ue);

   /* Update the UE's outstanding allocation */
   if (!ulAllocInfo->alloc->hqProc->isRetx)
   {
      rgSCHCmnUlUpdOutStndAlloc(cell, ue, ulAllocInfo->allocdBytes);
   }

   RETVOID;
}

/**
 * @brief Update the UEs outstanding alloc based on the BSR report's timing.
 *
 *
 * @details
 *
 *     Function: rgSCHCmnUpdUlCompEffBsr
 *     Purpose:  Clear off all the allocations from outstanding allocation that
 *     are later than or equal to BSR timing information (stored in UEs datIndTime).
 *
 *     Invoked by: Scheduler
 *
 *  @param[in]  RgSchUeCb *ue
 *  @return  Void
 **/
#ifdef ANSI
PRIVATE Void rgSCHCmnUpdUlCompEffBsr
(
RgSchUeCb *ue
)
#else
PRIVATE Void rgSCHCmnUpdUlCompEffBsr(ue)
RgSchUeCb *ue;
#endif
{
   RgSchCmnUlUe *ueUl = RG_SCH_CMN_GET_UL_UE(ue);
   CmLList   *node = ueUl->ulAllocLst.last;
   RgSchCmnAllocRecord *allRcd;
   U32 outStndAlloc=0;
   U32 nonLcg0OutStndAllocBs=0;
   U32 nonLcg0Bsr=0;
   U8  lcgId;
   RgSchCmnLcg *cmnLcg = NULLP;
   TRC2(rgSCHCmnUpdUlCompEffBsr);

   while (node)
   {
      allRcd = (RgSchCmnAllocRecord *)node->node;
      if (RGSCH_TIMEINFO_SAME(ue->macCeRptTime, allRcd->allocTime))
      {
         node = node->next;
         break;
      }
      node = node->prev;
   }
   while (node)
   {
      allRcd = (RgSchCmnAllocRecord *)node->node;
      node = node->next;
      outStndAlloc += allRcd->alloc;
   }
 
   cmnLcg = (RgSchCmnLcg *)(ue->ul.lcgArr[0].sch);
   /* Update UEs LCG0's bs according to the total outstanding BSR allocation.*/
   if (cmnLcg->bs > outStndAlloc)
   {
      cmnLcg->bs -= outStndAlloc;
      ueUl->minReqBytes = cmnLcg->bs;
      outStndAlloc = 0;
   }
   else
   {
      nonLcg0OutStndAllocBs = outStndAlloc - cmnLcg->bs;
      cmnLcg->bs = 0;
   }

   for(lcgId = 1;lcgId < RGSCH_MAX_LCG_PER_UE; lcgId++)
   {
      if(RGSCH_LCG_ISCFGD(&ue->ul.lcgArr[lcgId]))
      {
         cmnLcg = ((RgSchCmnLcg *) (ue->ul.lcgArr[lcgId].sch));
         if (RGSCH_IS_GBR_BEARER(cmnLcg->cfgdGbr))
         {
            nonLcg0Bsr += cmnLcg->bs;
         }
      }
   }
   nonLcg0Bsr += ueUl->nonGbrLcgBs;  
   if (nonLcg0OutStndAllocBs > nonLcg0Bsr)
   {
      nonLcg0Bsr = 0;
   }
   else
   {
      nonLcg0Bsr -= nonLcg0OutStndAllocBs;
   }
   ueUl->nonLcg0Bs = nonLcg0Bsr;
   /* Cap effBsr with nonLcg0Bsr and append lcg0 bs.
    * nonLcg0Bsr limit applies only to lcg1,2,3 */
   /* better be handled in individual scheduler */
   ueUl->effBsr = nonLcg0Bsr +\
                  ((RgSchCmnLcg *)(ue->ul.lcgArr[0].sch))->bs;
   RETVOID;
}

/**
 * @brief  Records information about the current allocation.
 *
 * @details
 *
 *     Function: rgSCHCmnUlRecordUeAlloc
 *     Purpose:  Records information about the curent allocation.
 *               This includes the allocated bytes, as well
 *               as some power information.
 *
 *     Invoked by: Scheduler
 *
 *  @param[in]  RgSchCellCb *cell
 *  @param[in]  RgSchUeCb   *ue
 *  @return  Void
 **/
#ifdef ANSI
PUBLIC Void rgSCHCmnUlRecordUeAlloc
(
RgSchCellCb *cell,
RgSchUeCb   *ue
)
#else
PUBLIC Void rgSCHCmnUlRecordUeAlloc(cell, ue)
RgSchCellCb *cell;
RgSchUeCb   *ue;
#endif
{
#ifdef LTE_TDD
   RgSchCmnUlCell     *cellUl = RG_SCH_CMN_GET_UL_CELL(cell);
#endif
   RgSchCmnUlUe        *ueUl = RG_SCH_CMN_GET_UL_UE(ue);
   CmLListCp           *lst = &ueUl->ulAllocLst;
   CmLList             *node = ueUl->ulAllocLst.first;
   RgSchCmnAllocRecord *allRcd = (RgSchCmnAllocRecord *)(node->node);
   RgSchCmnUeUlAlloc  *ulAllocInfo = &ueUl->alloc;
   CmLteUeCategory ueCtg = (CmLteUeCategory)(RG_SCH_CMN_GET_UE_CTGY(ue));
   TRC2(rgSCHCmnUlRecordUeAlloc);

   cmLListDelFrm(lst, &allRcd->lnk);
#ifndef LTE_TDD
   /* To the crntTime, add the MIN time at which UE will
    * actually send the BSR i.e DELTA+4 */
   allRcd->allocTime = cell->crntTime;
   /*ccpu00116293 - Correcting relation between UL subframe and DL subframe based on RG_UL_DELTA*/
   RGSCH_INCR_SUB_FRAME(allRcd->allocTime,
                        (TFU_ULCNTRL_DLDELTA + RGSCH_PDCCH_PUSCH_DELTA));
#else
   allRcd->allocTime = cellUl->schdTime;
#endif
   cmLListAdd2Tail(lst, &allRcd->lnk);

   /* Filling in the parameters to be recorded */
   allRcd->alloc = ulAllocInfo->allocdBytes;
   allRcd->numRb = ulAllocInfo->alloc->grnt.numRb;
   /*Recording the UL CQI derived from the maxUlCqi */
   allRcd->cqi   = rgSCHCmnUlGetCqi(ue, ueCtg);
   allRcd->tpc   = ulAllocInfo->alloc->grnt.tpc;

   rgSCHPwrRecordRbAlloc(cell, ue, allRcd->numRb);

   cell->measurements.ulBytesCnt += ulAllocInfo->allocdBytes;

   RETVOID;
}

/** PHR handling for MSG3
 * @brief  Records allocation information of msg3 in the the UE.
 *
 * @details
 *
 *     Function: rgSCHCmnUlRecMsg3Alloc
 *     Purpose:  Records information about msg3 allocation.
 *               This includes the allocated bytes, as well
 *               as some power information.
 *
 *     Invoked by: Scheduler
 *
 *  @param[in]  RgSchCellCb *cell
 *  @param[in]  RgSchUeCb   *ue
 *  @param[in]  RgSchRaCb   *raCb
 *  @return  Void
 **/
#ifdef ANSI
PUBLIC Void rgSCHCmnUlRecMsg3Alloc
(
RgSchCellCb *cell,
RgSchUeCb   *ue,
RgSchRaCb   *raCb
)
#else
PUBLIC Void rgSCHCmnUlRecMsg3Alloc(cell, ue, raCb)
RgSchCellCb *cell;
RgSchUeCb   *ue;
RgSchRaCb   *raCb;
#endif
{
   RgSchCmnUlUe        *ueUl = RG_SCH_CMN_GET_UL_UE(ue);
   CmLListCp           *lst = &ueUl->ulAllocLst;
   CmLList             *node = ueUl->ulAllocLst.first;
   RgSchCmnAllocRecord *allRcd = (RgSchCmnAllocRecord *)(node->node);

   /* Stack Crash problem for TRACE5 changes */
   TRC2(rgSCHCmnUlRecMsg3Alloc);

   cmLListDelFrm(lst, node);
   allRcd->allocTime = raCb->msg3AllocTime;
   cmLListAdd2Tail(lst, node);

   /* Filling in the parameters to be recorded */
   allRcd->alloc = raCb->msg3Grnt.datSz;
   allRcd->numRb = raCb->msg3Grnt.numRb;
   allRcd->cqi   = raCb->ccchCqi;
   allRcd->tpc   = raCb->msg3Grnt.tpc;

   rgSCHPwrRecordRbAlloc(cell, ue, allRcd->numRb);

   RETVOID;
}
/**
 * @brief Keeps track of the most recent RG_SCH_CMN_MAX_ALLOC_TRACK
 * allocations to track. Adds this allocation to the ueUl's ulAllocLst.
 *
 *
 * @details
 *
 *     Function: rgSCHCmnUlUpdOutStndAlloc
 *     Purpose:  Recent Allocation shall be at First Pos'n.
 *               Remove the last node, update the fields
 *                with the new allocation and add at front.
 *
 *     Invoked by: Scheduler
 *
 *  @param[in]  RgSchCellCb *cell
 *  @param[in]  RgSchUeCb   *ue
 *  @param[in]  U32 alloc
 *  @return  Void
 **/
#ifdef ANSI
PUBLIC Void rgSCHCmnUlUpdOutStndAlloc
(
RgSchCellCb *cell,
RgSchUeCb   *ue,
U32 alloc
)
#else
PUBLIC Void rgSCHCmnUlUpdOutStndAlloc(cell, ue, alloc)
RgSchCellCb *cell;
RgSchUeCb   *ue;
U32 alloc;
#endif
{
   RgSchCmnUlUe        *ueUl = RG_SCH_CMN_GET_UL_UE(ue);
   U32                 nonLcg0Alloc=0;
   TRC2(rgSCHCmnUlUpdOutStndAlloc);

   /* Update UEs LCG0's bs according to the total outstanding BSR allocation.*/
   if (((RgSchCmnLcg *)(ue->ul.lcgArr[0].sch))->bs > alloc)
   {
      ((RgSchCmnLcg *)(ue->ul.lcgArr[0].sch))->bs -= alloc;
   }
   else
   {
      nonLcg0Alloc = alloc - ((RgSchCmnLcg *)(ue->ul.lcgArr[0].sch))->bs;
      ((RgSchCmnLcg *)(ue->ul.lcgArr[0].sch))->bs = 0;
   }

   if (nonLcg0Alloc >= ueUl->nonLcg0Bs)
   {
      ueUl->nonLcg0Bs  = 0;
   }
   else
   {
      ueUl->nonLcg0Bs  -= nonLcg0Alloc;
   }
   /* Cap effBsr with effAmbr and append lcg0 bs.
    * effAmbr limit applies only to lcg1,2,3 non GBR LCG's*/
   /* better be handled in individual scheduler */
   ueUl->effBsr = ueUl->nonLcg0Bs +\
                  ((RgSchCmnLcg *)(ue->ul.lcgArr[0].sch))->bs;
#ifdef RGR_V1
   if (ueUl->effBsr == 0)
   {
      if (ue->bsrTmr.tmrEvnt != TMR_NONE)
      {
         rgSCHTmrStopTmr(cell, ue->bsrTmr.tmrEvnt, ue);
      }
      /* ccpu00133008 */
      if (FALSE == ue->isSrGrant)
      {
         if (ue->ul.bsrTmrCfg.isPrdBsrTmrPres)
         {
            rgSCHTmrStartTmr(cell, ue, RG_SCH_TMR_BSR,
                  ue->ul.bsrTmrCfg.prdBsrTmr);
         }
      }
   }
#endif
   /* Resetting UEs lower Cap */
   ueUl->minReqBytes = 0;

   RETVOID;
}


/**
 * @brief Returns the "Itbs" for a given UE.
 *
 * @details
 *
 *     Function: rgSCHCmnUlGetITbs
 *     Purpose:  This function returns the "Itbs" for a given UE.
 *
 *     Invoked by: Scheduler
 *
 *  @param[in]  RgSchUeCb        *ue
 *  @return     U8
 **/
#ifdef ANSI
PUBLIC U8 rgSCHCmnUlGetITbs
(
RgSchUeCb        *ue,
Bool             isEcp
)
#else
PUBLIC U8 rgSCHCmnUlGetITbs(ue, isEcp)
RgSchUeCb        *ue;
Bool             isEcp;
#endif
{
   RgSchCmnUlUe *ueUl    = RG_SCH_CMN_GET_UL_UE(ue);
   /* CQI will be capped to maxUlCqi for 16qam UEs */
   CmLteUeCategory  ueCtgy = (CmLteUeCategory)(RG_SCH_CMN_GET_UE_CTGY(ue));
   U8            cqi;
#ifdef UL_LA
   U8            iTbs;
   U8            maxiTbs = rgSchCmnUlCqiToTbsTbl[(U8)isEcp][ueUl->maxUlCqi]; 
#endif

   TRC2(rgSCHCmnUlGetITbs);

   /* #ifdef RG_SCH_CMN_EXT_CP_SUP For ECP pick index 1 */
#ifdef TFU_UPGRADE
   if ( (ueCtgy != CM_LTE_UE_CAT_5) &&
        (ueUl->validUlCqi > ueUl->maxUlCqi)
      )
   {
      cqi = ueUl->maxUlCqi;
   }
   else
   {
      cqi = ueUl->validUlCqi;
   }

#ifdef UL_LA
   iTbs = (ueUl->ulLaCb.cqiBasediTbs + ueUl->ulLaCb.deltaiTbs)/100;

   RG_SCH_CHK_ITBS_RANGE(iTbs, maxiTbs); 

   iTbs = RGSCH_MIN(iTbs,  ue->cell->thresholds.maxUlItbs);

   RETVALUE(iTbs);
#endif 
#else
   if ( (ueCtgy != CM_LTE_UE_CAT_5) && (ueUl->crntUlCqi[0] > ueUl->maxUlCqi ))
   {
      cqi = ueUl->maxUlCqi;
   }
   else
   {
      cqi = ueUl->crntUlCqi[0];
   }
#endif
   RETVALUE(rgSchCmnUlCqiToTbsTbl[(U8)isEcp][cqi]);
}

/**
 * @brief This function adds the UE to DLRbAllocInfo TX lst.
 *
 * @details
 *
 *     Function: rgSCHCmnDlRbInfoAddUeTx
 *     Purpose:  This function adds the UE to DLRbAllocInfo TX lst.
 *
 *     Invoked by: Common Scheduler
 *
 *  @param[out] RgSchCmnDlRbAllocInfo *allocInfo
 *  @param[in]  RgSchUeCb             *ue
 *  @param[in]  RgSchDlHqProcCb       *hqP
 *  @return  Void
 *
 **/
#ifdef ANSI
PRIVATE Void rgSCHCmnDlRbInfoAddUeTx
(
RgSchCmnDlRbAllocInfo *allocInfo,
RgSchUeCb             *ue,
RgSchDlHqProcCb       *hqP
)
#else
PRIVATE Void rgSCHCmnDlRbInfoAddUeTx(allocInfo, ue, hqP)
RgSchCmnDlRbAllocInfo *allocInfo;
RgSchUeCb             *ue;
RgSchDlHqProcCb       *hqP;
#endif
{
   RgSchCmnCell      *cellSch = RG_SCH_CMN_GET_CELL(ue->cell);

   TRC2(rgSCHCmnDlRbInfoAddUeTx);

   if (hqP->reqLnk.node == NULLP)
   {
      if (cellSch->dl.isDlFreqSel)
      {
         cellSch->apisDlfs->rgSCHDlfsAddUeToLst(
           &allocInfo->dedAlloc.txHqPLst, hqP);
      }
      else
      {
         cmLListAdd2Tail(&allocInfo->dedAlloc.txHqPLst, &hqP->reqLnk);
         hqP->reqLnk.node = (PTR)hqP;
      }
   }
   RETVOID;
}

/**
 * @brief This function adds the UE to DLRbAllocInfo RETX lst.
 *
 * @details
 *
 *     Function: rgSCHCmnDlRbInfoAddUeRetx
 *     Purpose:  This function adds the UE to DLRbAllocInfo RETX lst.
 *
 *     Invoked by: Common Scheduler
 *
 *  @param[out] RgSchCmnDlRbAllocInfo *allocInfo
 *  @param[in]  RgSchUeCb             *ue
 *  @param[in]  RgSchDlHqProcCb       *hqP
 *  @return  Void
 *
 **/
#ifdef ANSI
PRIVATE Void rgSCHCmnDlRbInfoAddUeRetx
(
RgSchCmnDlRbAllocInfo *allocInfo,
RgSchUeCb             *ue,
RgSchDlHqProcCb       *hqP
)
#else
PRIVATE Void rgSCHCmnDlRbInfoAddUeRetx(allocInfo, ue, hqP)
RgSchCmnDlRbAllocInfo *allocInfo;
RgSchUeCb             *ue;
RgSchDlHqProcCb       *hqP;
#endif
{
   RgSchCmnCell      *cellSch = RG_SCH_CMN_GET_CELL(ue->cell);

   TRC2(rgSCHCmnDlRbInfoAddUeRetx);

   if (cellSch->dl.isDlFreqSel)
   {
      cellSch->apisDlfs->rgSCHDlfsAddUeToLst(
        &allocInfo->dedAlloc.retxHqPLst, hqP);
   }
   else
   {
      /* checking UE's presence in this lst is unnecessary */
      cmLListAdd2Tail(&allocInfo->dedAlloc.retxHqPLst, &hqP->reqLnk);
      hqP->reqLnk.node = (PTR)hqP;
   }
   RETVOID;
}

/**
 * @brief This function adds the UE to DLRbAllocInfo TX-RETX lst.
 *
 * @details
 *
 *     Function: rgSCHCmnDlRbInfoAddUeRetxTx
 *     Purpose:  This adds the UE to DLRbAllocInfo TX-RETX lst.
 *
 *     Invoked by: Common Scheduler
 *
 *  @param[out] RgSchCmnDlRbAllocInfo *allocInfo
 *  @param[in]  RgSchUeCb             *ue
 *  @param[in]  RgSchDlHqProcCb       *hqP
 *  @return  Void
 *
 **/
#ifdef ANSI
PRIVATE Void rgSCHCmnDlRbInfoAddUeRetxTx
(
RgSchCmnDlRbAllocInfo *allocInfo,
RgSchUeCb             *ue,
RgSchDlHqProcCb       *hqP
)
#else
PRIVATE Void rgSCHCmnDlRbInfoAddUeRetxTx(allocInfo, ue, hqP)
RgSchCmnDlRbAllocInfo *allocInfo;
RgSchUeCb             *ue;
RgSchDlHqProcCb       *hqP;
#endif
{
   RgSchCmnCell      *cellSch = RG_SCH_CMN_GET_CELL(ue->cell);

   TRC2(rgSCHCmnDlRbInfoAddUeRetxTx);

   if (cellSch->dl.isDlFreqSel)
   {
      cellSch->apisDlfs->rgSCHDlfsAddUeToLst(
        &allocInfo->dedAlloc.txRetxHqPLst, hqP);
   }
   else
   {
      cmLListAdd2Tail(&allocInfo->dedAlloc.txRetxHqPLst, &hqP->reqLnk);
      hqP->reqLnk.node = (PTR)hqP;
   }
   RETVOID;
}

/**
 * @brief This function adds the UE to DLRbAllocInfo NonSchdRetxLst.
 *
 * @details
 *
 *     Function: rgSCHCmnDlAdd2NonSchdRetxLst 
 *     Purpose:  During RB estimation for RETX, if allocation fails
 *               then appending it to NonSchdRetxLst, the further
 *               action is taken as part of Finalization in
 *               respective schedulers.
 *
 *     Invoked by: Common Scheduler
 *
 *  @param[out] RgSchCmnDlRbAllocInfo *allocInfo
 *  @param[in]  RgSchUeCb             *ue
 *  @param[in]  RgSchDlHqProcCb       *hqP
 *  @return  Void
 *
 **/
#ifdef ANSI
PRIVATE Void rgSCHCmnDlAdd2NonSchdRetxLst 
(
RgSchCmnDlRbAllocInfo *allocInfo,
RgSchUeCb             *ue,
RgSchDlHqProcCb       *hqP
)
#else
PRIVATE Void rgSCHCmnDlAdd2NonSchdRetxLst(allocInfo, ue, hqP)
RgSchCmnDlRbAllocInfo *allocInfo;
RgSchUeCb             *ue;
RgSchDlHqProcCb       *hqP;
#endif
{
   CmLList         *schdLnkNode;

   TRC2(rgSCHCmnDlAdd2NonSchdRetxLst);

#ifdef LTEMAC_SPS
   if ( (hqP->sch != (RgSchCmnDlHqProc *)NULLP) && 
         (RG_SCH_CMN_SPS_DL_IS_SPS_HQP(hqP)))
   {
      RETVOID;
   }
#endif

   schdLnkNode = &hqP->schdLstLnk;
   RG_SCH_CMN_INIT_SCHD_LNK(schdLnkNode, hqP);
   cmLListAdd2Tail(&allocInfo->dedAlloc.nonSchdRetxHqPLst, schdLnkNode);

   RETVOID;
}

/**
 * @brief This function adds the UE to DLRbAllocInfo NonSchdTxRetxLst.
 *
 * @details
 *
 *     Function: rgSCHCmnDlAdd2NonSchdTxRetxLst 
 *     Purpose:  During RB estimation for TXRETX, if allocation fails
 *               then appending it to NonSchdTxRetxLst, the further
 *               action is taken as part of Finalization in
 *               respective schedulers.
 *
 *     Invoked by: Common Scheduler
 *
 *  @param[out] RgSchCmnDlRbAllocInfo *allocInfo
 *  @param[in]  RgSchUeCb             *ue
 *  @param[in]  RgSchDlHqProcCb       *hqP
 *  @return  Void
 *
 **/
#ifdef LTE_TDD
/**
 * @brief This function handles the initialisation of DL HARQ/ACK feedback
 *        timing information for eaach DL subframe.
 *
 * @details
 *
 *     Function: rgSCHCmnDlANFdbkInit
 *     Purpose:  Each DL subframe stores the sfn and subframe
 *               information of UL subframe in which it expects
 *               HARQ ACK/NACK feedback for this subframe.It
 *               generates the information based on Downlink
 *               Association Set Index table.
 *
 *     Invoked by: Scheduler
 *
 *  @param[in]  RgSchCellCb*     cell
 *  @return     S16
 *
 **/
#ifdef ANSI
PRIVATE S16 rgSCHCmnDlANFdbkInit
(
RgSchCellCb                *cell
)
#else
PRIVATE S16 rgSCHCmnDlANFdbkInit(cell)
RgSchCellCb                *cell;
#endif
{
 U8                   sfCount;
 U8                   ulDlCfgIdx = cell->ulDlCfgIdx;
 U8                   maxDlSubfrms = cell->numDlSubfrms;
 U8                   sfNum;
 U8                   idx;
 U8                   dlIdx;
 U8                   calcSfnOffset;
 S8                   calcSfNum;
 U8                   ulSfCnt =0;
 RgSchTddSubfrmInfo   ulSubfrmInfo;
 U8                   maxUlSubfrms;

   TRC2(rgSCHCmnDlANFdbkInit);

   ulSubfrmInfo = rgSchTddMaxUlSubfrmTbl[ulDlCfgIdx];
   maxUlSubfrms = rgSchTddNumUlSubfrmTbl[ulDlCfgIdx][RGSCH_NUM_SUB_FRAMES-1];

   /* Generate HARQ ACK/NACK feedback information for each DL sf in a radio frame
    * Calculate this information based on DL Association set Index table */
   for (sfCount = 0, sfNum = 0; sfCount < maxUlSubfrms; sfCount++)
   {
      while(rgSchTddUlDlSubfrmTbl[ulDlCfgIdx][sfNum] !=
            RG_SCH_TDD_UL_SUBFRAME)
      {
         sfNum = (sfNum+1) % RGSCH_NUM_SUB_FRAMES;
      }
      ulSfCnt++;

      for(idx=0; idx < rgSchTddDlAscSetIdxKTbl[ulDlCfgIdx][sfNum].\
            numFdbkSubfrms; idx++)
      {
         calcSfNum = sfNum - rgSchTddDlAscSetIdxKTbl[ulDlCfgIdx][sfNum].\
                     subfrmNum[idx];
         if(calcSfNum < 0)
         {
            calcSfnOffset = RGSCH_CEIL(-calcSfNum, RGSCH_NUM_SUB_FRAMES);
         }
         else
         {
            calcSfnOffset = 0;
         }

         calcSfNum = ((RGSCH_NUM_SUB_FRAMES * calcSfnOffset) + calcSfNum)\
                     % RGSCH_NUM_SUB_FRAMES;

         if(calcSfNum <= RG_SCH_CMN_SPL_SUBFRM_1)
         {
            dlIdx = calcSfNum;
         }
         else if((ulSubfrmInfo.switchPoints == 2) && (calcSfNum <= \
                  RG_SCH_CMN_SPL_SUBFRM_6))
         {
            dlIdx = calcSfNum - ulSubfrmInfo.numFrmHf1;
         }
         else
         {
            dlIdx = calcSfNum - maxUlSubfrms;
         }

         cell->subFrms[dlIdx]->dlFdbkInfo.subframe = sfNum;
         cell->subFrms[dlIdx]->dlFdbkInfo.sfnOffset = calcSfnOffset;
         cell->subFrms[dlIdx]->dlFdbkInfo.m = idx;
      }
      sfNum = (sfNum+1) % RGSCH_NUM_SUB_FRAMES;
   }

   /* DL subframes in the subsequent radio frames are initialized
    * with the previous radio frames  */
   for(dlIdx = RGSCH_NUM_SUB_FRAMES - maxUlSubfrms; dlIdx < maxDlSubfrms;\
         dlIdx++)
   {
      sfNum = dlIdx - rgSchTddNumDlSubfrmTbl[ulDlCfgIdx]\
              [RGSCH_NUM_SUB_FRAMES-1];
      cell->subFrms[dlIdx]->dlFdbkInfo.subframe = \
                                                  cell->subFrms[sfNum]->dlFdbkInfo.subframe;
      cell->subFrms[dlIdx]->dlFdbkInfo.sfnOffset = \
                                                   cell->subFrms[sfNum]->dlFdbkInfo.sfnOffset;
      cell->subFrms[dlIdx]->dlFdbkInfo.m = cell->subFrms[sfNum]->dlFdbkInfo.m;
   }
   RETVALUE(ROK);
}

/**
 * @brief This function handles the initialization of uplink association
 *        set information for each DL subframe.
 *
 *
 * @details
 *
 *     Function: rgSCHCmnDlKdashUlAscInit
 *     Purpose:  Each DL sf stores the sfn and sf information of UL sf
 *               in which it expects HQ ACK/NACK trans. It generates the information
 *               based on k` in UL association set index table.
 *
 *     Invoked by: Scheduler
 *
 *  @param[in]  RgSchCellCb*     cell
 *  @return     S16
 *
 **/
#ifdef ANSI
PRIVATE S16 rgSCHCmnDlKdashUlAscInit
(
RgSchCellCb                *cell
)
#else
PRIVATE S16 rgSCHCmnDlKdashUlAscInit(cell)
RgSchCellCb                *cell;
#endif
{
 U8                   sfCount;
 U8                   ulDlCfgIdx = cell->ulDlCfgIdx;
 U8                   maxDlSubfrms = cell->numDlSubfrms;
 U8                   sfNum;
 U8                   dlIdx;
 S8                   calcSfnOffset;
 S8                   calcSfNum;
 U8                   ulSfCnt =0;
 RgSchTddSubfrmInfo   ulSubfrmInfo = rgSchTddMaxUlSubfrmTbl[ulDlCfgIdx];
 U8                   maxUlSubfrms = rgSchTddNumUlSubfrmTbl[ulDlCfgIdx]\
                                     [RGSCH_NUM_SUB_FRAMES-1];
 U8                   dlPres = 0;

   TRC2(rgSCHCmnDlKdashUlAscInit);

   /* Generate ACK/NACK offset information for each DL subframe in a radio frame
    * Calculate this information based on K` in UL Association Set table */
   for (sfCount = 0, sfNum = 0; sfCount < maxUlSubfrms; sfCount++)
   {
      while(rgSchTddUlDlSubfrmTbl[ulDlCfgIdx][sfNum] !=
            RG_SCH_TDD_UL_SUBFRAME)
      {
         sfNum = (sfNum+1) % RGSCH_NUM_SUB_FRAMES;
      }
      ulSfCnt++;

      calcSfNum = (sfNum - rgSchTddUlAscIdxKDashTbl[ulDlCfgIdx-1][sfNum] + \
            RGSCH_NUM_SUB_FRAMES) % RGSCH_NUM_SUB_FRAMES;
      calcSfnOffset = sfNum - rgSchTddUlAscIdxKDashTbl[ulDlCfgIdx-1][sfNum];
      if(calcSfnOffset < 0)
      {
         calcSfnOffset = RGSCH_CEIL(-calcSfnOffset, RGSCH_NUM_SUB_FRAMES);
      }
      else
      {
         calcSfnOffset = 0;
      }

      if(calcSfNum <= RG_SCH_CMN_SPL_SUBFRM_1)
      {
         dlIdx = calcSfNum;
      }
      else if((ulSubfrmInfo.switchPoints == 2) &&
            (calcSfNum <= RG_SCH_CMN_SPL_SUBFRM_6))
      {
         dlIdx = calcSfNum - ulSubfrmInfo.numFrmHf1;
      }
      else
      {
         dlIdx = calcSfNum - maxUlSubfrms;
      }

      cell->subFrms[dlIdx]->ulAscInfo.subframe = sfNum;
      cell->subFrms[dlIdx]->ulAscInfo.sfnOffset = calcSfnOffset;

      /* set dlIdx for which ulAscInfo is updated */
      dlPres = dlPres | (1 << dlIdx);
      sfNum = (sfNum+1) % RGSCH_NUM_SUB_FRAMES;
   }

   /* Set Invalid information for which ulAscInfo is not present */
   for (sfCount = 0;
         sfCount < rgSchTddNumDlSubfrmTbl[ulDlCfgIdx][RGSCH_NUM_SUB_FRAMES-1];
         sfCount++)
   {
      /* If dlPres is 0, ulAscInfo is not present in that DL index */
      if(! ((dlPres >> sfCount)&0x01))
      {
         cell->subFrms[sfCount]->ulAscInfo.sfnOffset =
            RGSCH_INVALID_INFO;
         cell->subFrms[sfCount]->ulAscInfo.subframe =
            RGSCH_INVALID_INFO;
      }
   }

   /* DL subframes in the subsequent radio frames are initialized
    * with the previous radio frames  */
   for(dlIdx = RGSCH_NUM_SUB_FRAMES - maxUlSubfrms; dlIdx < maxDlSubfrms;
         dlIdx++)
   {
      sfNum = dlIdx - \
              rgSchTddNumDlSubfrmTbl[ulDlCfgIdx][RGSCH_NUM_SUB_FRAMES-1];
      cell->subFrms[dlIdx]->ulAscInfo.subframe =
         cell->subFrms[sfNum]->ulAscInfo.subframe;
      cell->subFrms[dlIdx]->ulAscInfo.sfnOffset =
         cell->subFrms[sfNum]->ulAscInfo.sfnOffset;
   }
   RETVALUE(ROK);
}


/**
 * @brief This function initialises the 'Np' value for 'p'
 *
 * @details
 *
 *     Function: rgSCHCmnDlNpValInit
 *     Purpose:  To initialise the 'Np' value for each 'p'. It is used
 *               to find the mapping between nCCE and 'p' and used in
 *               HARQ ACK/NACK reception.
 *
 *     Invoked by: Scheduler
 *
 *  @param[in]  RgSchCellCb*     cell
 *  @return     S16
 *
 **/
#ifdef ANSI
PRIVATE S16 rgSCHCmnDlNpValInit
(
RgSchCellCb                *cell
)
#else
PRIVATE S16 rgSCHCmnDlNpValInit(cell)
RgSchCellCb                *cell;
#endif
{
   U8    idx;
   U16   np;
   TRC2(rgSCHCmnDlNpValInit);

   /* Always Np is 0 for p=0 */
   cell->rgSchTddNpValTbl[0] = 0;

   for(idx=1; idx < RGSCH_TDD_MAX_P_PLUS_ONE_VAL; idx++)
   {
      np = cell->bwCfg.dlTotalBw * (idx * RG_SCH_CMN_NUM_SUBCAR - 4);
      cell->rgSchTddNpValTbl[idx] = (U8) (np/36);
   }

   RETVALUE(ROK);
}

/**
 * @brief This function handles the creation of RACH preamble
 *        list to queue the preambles and process at the scheduled
 *        time.
 *
 * @details
 *
 *     Function: rgSCHCmnDlCreateRachPrmLst
 *     Purpose:  To create RACH preamble list based on RA window size.
 *               It is used to queue the preambles and process it at the
 *               scheduled time.
 *
 *     Invoked by: Scheduler
 *
 *  @param[in]  RgSchCellCb*     cell
 *  @return     S16
 *
 **/
#ifdef ANSI
PRIVATE S16 rgSCHCmnDlCreateRachPrmLst
(
RgSchCellCb                *cell
)
#else
PRIVATE S16 rgSCHCmnDlCreateRachPrmLst(cell)
RgSchCellCb                *cell;
#endif
{
 U8       raArrSz;
 S16       ret;
 U8       lstSize;

   TRC2(rgSCHCmnDlCreateRachPrmLst);

   RG_SCH_CMN_CALC_RARSPLST_SIZE(cell, raArrSz);

   lstSize = raArrSz * RGSCH_MAX_RA_RNTI_PER_SUBFRM * RGSCH_NUM_SUB_FRAMES;

   cell->raInfo.maxRaSize = raArrSz;
   ret = rgSCHUtlAllocSBuf(cell->instIdx,
         (Data **)(&cell->raInfo.raReqLst), (Size)(lstSize * sizeof(CmLListCp)));
   if (ret != ROK)
   {
      RETVALUE(ret);
   }

   cell->raInfo.lstSize = lstSize;

   RETVALUE(ROK);
}


/**
 * @brief This function handles the initialization of RACH Response
 *        information at each DL subframe.
 *
 * @details
 *
 *     Function: rgSCHCmnDlRachInfoInit
 *     Purpose:  Each DL subframe stores the sfn and subframe information of
 *               possible RACH response allowed for UL subframes. It generates
 *               the information based on PRACH configuration.
 *
 *     Invoked by: Scheduler
 *
 *  @param[in]  RgSchCellCb*     cell
 *  @return     S16
 *
 **/
#ifdef ANSI
PRIVATE S16 rgSCHCmnDlRachInfoInit
(
RgSchCellCb                *cell
)
#else
PRIVATE S16 rgSCHCmnDlRachInfoInit(cell)
RgSchCellCb                *cell;
#endif
{
   U8                   sfCount;
   U8                   ulDlCfgIdx = cell->ulDlCfgIdx;
   U8                   sfNum;
   U8                   ulSfCnt =0;
   U8                   maxUlSubfrms = rgSchTddNumUlSubfrmTbl[ulDlCfgIdx]\
                                       [RGSCH_NUM_SUB_FRAMES-1];
   U8                   raArrSz;
   RgSchTddRachRspLst   rachRspLst[3][RGSCH_NUM_SUB_FRAMES];
   U8                   startWin;
   U8                   endWin;
   U8                   sfnIdx;
   U8                   subfrmIdx;
   U8                   endSubfrmIdx;
   U8                   startSubfrmIdx;
   S16                   ret;
   RgSchTddRachDelInfo  *delInfo;
   S8                   sfnOffset;
   U8                   numSubfrms;

   TRC2(rgSCHCmnDlRachInfoInit);

   cmMemset((U8 *)rachRspLst, 0, sizeof(rachRspLst));

   RG_SCH_CMN_CALC_RARSPLST_SIZE(cell, raArrSz);

   /* Include Special subframes */
   maxUlSubfrms = maxUlSubfrms + \
                  rgSchTddMaxUlSubfrmTbl[ulDlCfgIdx].switchPoints;
   for (sfCount = 0, sfNum = 0; sfCount < maxUlSubfrms; sfCount++)
   {
      while(rgSchTddUlDlSubfrmTbl[ulDlCfgIdx][sfNum] ==
            RG_SCH_TDD_DL_SUBFRAME)
      {
         sfNum = (sfNum+1) % RGSCH_NUM_SUB_FRAMES;
      }
      ulSfCnt++;

      startWin = (sfNum + RG_SCH_CMN_RARSP_WAIT_PRD + \
            ((RgSchCmnCell *)cell->sc.sch)->dl.numRaSubFrms);
      endWin = (startWin + cell->rachCfg.raWinSize - 1);
      startSubfrmIdx =
         rgSchTddHighDlSubfrmIdxTbl[ulDlCfgIdx][startWin%RGSCH_NUM_SUB_FRAMES];
      /* Find the next DL subframe starting from Subframe 0 */
      if((startSubfrmIdx % RGSCH_NUM_SUB_FRAMES) == 0)
      {
         startWin = RGSCH_CEIL(startWin, RGSCH_NUM_SUB_FRAMES);
         startWin = startWin * RGSCH_NUM_SUB_FRAMES;
      }

      endSubfrmIdx =
         rgSchTddLowDlSubfrmIdxTbl[ulDlCfgIdx][endWin%RGSCH_NUM_SUB_FRAMES];
      endWin = (endWin/RGSCH_NUM_SUB_FRAMES) * RGSCH_NUM_SUB_FRAMES \
               + endSubfrmIdx;
      if(startWin > endWin)
      {
         continue;
      }
      /* Find all the possible RACH Response transmission
       * time within the RA window size */
      startSubfrmIdx = startWin%RGSCH_NUM_SUB_FRAMES;
      for(sfnIdx = startWin/RGSCH_NUM_SUB_FRAMES;
            sfnIdx <= endWin/RGSCH_NUM_SUB_FRAMES; sfnIdx++)
      {
         if(sfnIdx == endWin/RGSCH_NUM_SUB_FRAMES)
         {
            endSubfrmIdx = endWin%RGSCH_NUM_SUB_FRAMES;
         }
         else
         {
            endSubfrmIdx = RGSCH_NUM_SUB_FRAMES-1;
         }

         /* Find all the possible RACH Response transmission
          * time within radio frame */
         for(subfrmIdx = startSubfrmIdx;
               subfrmIdx <= endSubfrmIdx; subfrmIdx++)
         {
            if(rgSchTddUlDlSubfrmTbl[ulDlCfgIdx][subfrmIdx] ==
                  RG_SCH_TDD_UL_SUBFRAME)
            {
               continue;
            }
            subfrmIdx = rgSchTddHighDlSubfrmIdxTbl[ulDlCfgIdx][subfrmIdx];
            /* Find the next DL subframe starting from Subframe 0 */
            if(subfrmIdx == RGSCH_NUM_SUB_FRAMES)
            {
               break;
            }
            RGSCH_ARRAY_BOUND_CHECK(cell->instIdx, rachRspLst[sfnIdx], subfrmIdx);
            numSubfrms =
               rachRspLst[sfnIdx][subfrmIdx].rachRsp[0].numSubfrms;
            rachRspLst[sfnIdx][subfrmIdx].rachRsp[0].sfnOffset = sfnIdx;
            rachRspLst[sfnIdx][subfrmIdx].rachRsp[0].subframe[numSubfrms]
               = sfNum;
            rachRspLst[sfnIdx][subfrmIdx].rachRsp[0].numSubfrms++;
         }
         startSubfrmIdx = RG_SCH_CMN_SUBFRM_0;
      }
      /* Update the subframes to be deleted at this subframe */
      /* Get the subframe after the end of RA window size */
      endWin++;
      endSubfrmIdx++;
      sfnOffset = endWin/RGSCH_NUM_SUB_FRAMES;
      if(sfnOffset < 0)
      {
         sfnOffset += raArrSz;
      }
      sfnIdx = (endWin/RGSCH_NUM_SUB_FRAMES) % raArrSz;

      RGSCH_ARRAY_BOUND_CHECK(cell->instIdx, rgSchTddHighDlSubfrmIdxTbl[ulDlCfgIdx],endSubfrmIdx-1);
      if((endSubfrmIdx == RGSCH_NUM_SUB_FRAMES) ||
            (rgSchTddHighDlSubfrmIdxTbl[ulDlCfgIdx][endSubfrmIdx] ==
             RGSCH_NUM_SUB_FRAMES))
      {
         subfrmIdx =
            rgSchTddHighDlSubfrmIdxTbl[ulDlCfgIdx][RG_SCH_CMN_SUBFRM_0];
      }
      else
      {
         subfrmIdx = rgSchTddHighDlSubfrmIdxTbl[ulDlCfgIdx][endSubfrmIdx];
      }

      delInfo = &rachRspLst[sfnIdx][subfrmIdx].delInfo;
      delInfo->sfnOffset = sfnOffset;
      delInfo->subframe[delInfo->numSubfrms] = sfNum;
      delInfo->numSubfrms++;

      sfNum = (sfNum+1) % RGSCH_NUM_SUB_FRAMES;
   }

   ret = rgSCHCmnDlCpyRachInfo(cell, rachRspLst, raArrSz);
   if (ret != ROK)
   {
      RETVALUE(ret);
   }

   RETVALUE(ROK);
}

/**
 * @brief This function handles the initialization of PHICH information
 *        for each DL subframe based on PHICH table.
 *
 * @details
 *
 *     Function: rgSCHCmnDlPhichOffsetInit
 *     Purpose:  Each DL subf stores the sfn and subf information of UL subframe
 *               for which it trnsmts PHICH in this subframe. It generates the information
 *               based on PHICH table.
 *
 *     Invoked by: Scheduler
 *
 *  @param[in]  RgSchCellCb*     cell
 *  @return     S16
 *
 **/
#ifdef ANSI
PRIVATE S16 rgSCHCmnDlPhichOffsetInit
(
RgSchCellCb                *cell
)
#else
PRIVATE S16 rgSCHCmnDlPhichOffsetInit(cell)
RgSchCellCb                *cell;
#endif
{
   U8                   sfCount;
   U8                   ulDlCfgIdx = cell->ulDlCfgIdx;
   U8                   maxDlSubfrms = cell->numDlSubfrms;
   U8                   sfNum;
   U8                   dlIdx;
   U8                   dlPres = 0;
   U8                   calcSfnOffset;
   U8                   calcSfNum;
   U8                   ulSfCnt =0;
   RgSchTddSubfrmInfo   ulSubfrmInfo = rgSchTddMaxUlSubfrmTbl[ulDlCfgIdx];
   U8                   maxUlSubfrms = rgSchTddNumUlSubfrmTbl[ulDlCfgIdx]\
                                       [RGSCH_NUM_SUB_FRAMES-1];

   TRC2(rgSCHCmnDlPhichOffsetInit);

   /* Generate PHICH offset information for each DL subframe in a radio frame
    * Calculate this information based on K in PHICH table */
   for (sfCount = 0, sfNum = 0; sfCount < maxUlSubfrms; sfCount++)
   {
      while(rgSchTddUlDlSubfrmTbl[ulDlCfgIdx][sfNum] !=
            RG_SCH_TDD_UL_SUBFRAME)
      {
         sfNum = (sfNum+1) % RGSCH_NUM_SUB_FRAMES;
      }
      ulSfCnt++;

      calcSfNum = (rgSchTddKPhichTbl[ulDlCfgIdx][sfNum] + sfNum) % \
                  RGSCH_NUM_SUB_FRAMES;
      calcSfnOffset = (rgSchTddKPhichTbl[ulDlCfgIdx][sfNum] + sfNum) / \
                      RGSCH_NUM_SUB_FRAMES;

      if(calcSfNum <= RG_SCH_CMN_SPL_SUBFRM_1)
      {
         dlIdx = calcSfNum;
      }
      else if((ulSubfrmInfo.switchPoints == 2) &&
            (calcSfNum <= RG_SCH_CMN_SPL_SUBFRM_6))
      {
         dlIdx = calcSfNum - ulSubfrmInfo.numFrmHf1;
      }
      else
      {
         dlIdx = calcSfNum - maxUlSubfrms;
      }

      cell->subFrms[dlIdx]->phichOffInfo.subframe = sfNum;
      cell->subFrms[dlIdx]->phichOffInfo.numSubfrms = 1;

      cell->subFrms[dlIdx]->phichOffInfo.sfnOffset = calcSfnOffset;

      /* set dlIdx for which phich offset is updated */
      dlPres = dlPres | (1 << dlIdx);
      sfNum = (sfNum+1) % RGSCH_NUM_SUB_FRAMES;
   }

   /* Set Invalid information for which phich offset is not present */
   for (sfCount = 0;
         sfCount < rgSchTddNumDlSubfrmTbl[ulDlCfgIdx][RGSCH_NUM_SUB_FRAMES-1];
         sfCount++)
   {
      /* If dlPres is 0, phich offset is not present in that DL index */
      if(! ((dlPres >> sfCount)&0x01))
      {
         cell->subFrms[sfCount]->phichOffInfo.sfnOffset =
            RGSCH_INVALID_INFO;
         cell->subFrms[sfCount]->phichOffInfo.subframe =
            RGSCH_INVALID_INFO;
         cell->subFrms[sfCount]->phichOffInfo.numSubfrms = 0;
      }
   }

   /* DL subframes in the subsequent radio frames are
    * initialized with the previous radio frames  */
   for(dlIdx = RGSCH_NUM_SUB_FRAMES - maxUlSubfrms;
         dlIdx < maxDlSubfrms; dlIdx++)
   {
      sfNum = dlIdx - \
              rgSchTddNumDlSubfrmTbl[ulDlCfgIdx][RGSCH_NUM_SUB_FRAMES-1];

      cell->subFrms[dlIdx]->phichOffInfo.subframe =
         cell->subFrms[sfNum]->phichOffInfo.subframe;

      cell->subFrms[dlIdx]->phichOffInfo.sfnOffset =
         cell->subFrms[sfNum]->phichOffInfo.sfnOffset;
   }
   RETVALUE(ROK);
}


/**
 * @brief Updation of Sch vars per TTI.
 *
 * @details
 *
 *     Function: rgSCHCmnUpdVars
 *     Purpose:  Updation of Sch vars per TTI.
 *
 *  @param[in]  RgSchCellCb *cell
 *  @return  Void
 *
 **/
#ifdef ANSI
PUBLIC Void rgSCHCmnUpdVars
(
RgSchCellCb *cell
)
#else
PUBLIC Void rgSCHCmnUpdVars(cell)
RgSchCellCb *cell;
#endif
{
   RgSchCmnUlCell    *cellUl = RG_SCH_CMN_GET_UL_CELL(cell);
   CmLteTimingInfo   timeInfo;
   U8                idx;
   U8                ulSubframe;
   U8                ulDlCfgIdx = cell->ulDlCfgIdx;
   U8                msg3Subfrm;
   U8                Mval;
   TRC2(rgSCHCmnUpdVars);
 
   /* ccpu00132654-ADD- Initializing all the indices in every subframe*/ 
   rgSCHCmnInitVars(cell);

   idx = (cell->crntTime.subframe + TFU_ULCNTRL_DLDELTA) % RGSCH_NUM_SUB_FRAMES;
   /* Calculate the UL scheduling subframe idx based on the 
      Pusch k table */
   if(rgSchTddPuschTxKTbl[ulDlCfgIdx][idx] != 0)
   {
      /* PUSCH transmission is based on offset from DL
       * PDCCH scheduling */
      RGSCHCMNADDTOCRNTTIME(cell->crntTime,timeInfo, TFU_ULCNTRL_DLDELTA); 
      ulSubframe = rgSchTddPuschTxKTbl[ulDlCfgIdx][timeInfo.subframe];
      /* Add the DCI-0 to PUSCH time to get the time of UL subframe */
      RGSCHCMNADDTOCRNTTIME(timeInfo, timeInfo, ulSubframe);
#ifdef LTEMAC_SPS
      cellUl->schdTti = timeInfo.sfn * 10 + timeInfo.subframe;
#endif
      /* Fetch the corresponding  UL subframe Idx in UL sf array */ 
      cellUl->schdIdx = rgSCHCmnGetUlSfIdx(&timeInfo, cell);
      /* Fetch the corresponding  UL Harq Proc ID */ 
      cellUl->schdHqProcIdx = rgSCHCmnGetUlHqProcIdx(&timeInfo, cell);
      cellUl->schdTime = timeInfo;
   }
   Mval = rgSchTddPhichMValTbl[ulDlCfgIdx][idx]; 
   if(Mval)
   {
      /* Fetch the tx time for DL HIDCI-0 */
      RGSCHCMNADDTOCRNTTIME(cell->crntTime,timeInfo, TFU_ULCNTRL_DLDELTA);
      /* Fetch the corresponding n-k tx time of PUSCH */
      cellUl->hqFdbkIdx[0] = rgSCHCmnGetPhichUlSfIdx(&timeInfo, cell);
      /* Retx will happen according to the Pusch k table */
      cellUl->reTxIdx[0] = cellUl->schdIdx;
      
      if(ulDlCfgIdx == 0) 
      {
         /* Calculate the ReTxIdx corresponding to hqFdbkIdx[0] */
         cellUl->reTxIdx[0] = rgSchUtlCfg0ReTxIdx(cell,timeInfo,
                                                cellUl->hqFdbkIdx[0]);
         if(Mval == 2)
         {
            /* At Idx 1 store the UL SF adjacent(left) to the UL SF
               given at idx 0 */  
            cellUl->hqFdbkIdx[1] = (cellUl->hqFdbkIdx[0]-1 + 
                                   cellUl->numUlSubfrms) % cellUl->numUlSubfrms;
            /* Calculate the ReTxIdx corresponding to hqFdbkIdx[1] */
            cellUl->reTxIdx[1] = rgSchUtlCfg0ReTxIdx(cell,timeInfo,
                                                cellUl->hqFdbkIdx[1]);
         }                               
      }
   }

   idx = (cell->crntTime.subframe + TFU_RECPREQ_DLDELTA) % RGSCH_NUM_SUB_FRAMES;
   if (rgSchTddUlDlSubfrmTbl[ulDlCfgIdx][idx] == RG_SCH_TDD_UL_SUBFRAME)
   {
      RGSCHCMNADDTOCRNTTIME(cell->crntTime, timeInfo, TFU_RECPREQ_DLDELTA)
      cellUl->rcpReqIdx   = rgSCHCmnGetUlSfIdx(&timeInfo, cell);
   }
   idx = (cell->crntTime.subframe+RG_SCH_CMN_DL_DELTA) % RGSCH_NUM_SUB_FRAMES;
   
   /*[ccpu00134666]-MOD-Modify the check to schedule the RAR in
     special subframe */                       
   if(rgSchTddUlDlSubfrmTbl[ulDlCfgIdx][idx] != RG_SCH_TDD_UL_SUBFRAME)
   {
      RGSCHCMNADDTOCRNTTIME(cell->crntTime,timeInfo,RG_SCH_CMN_DL_DELTA)
      msg3Subfrm = rgSchTddMsg3SubfrmTbl[ulDlCfgIdx][timeInfo.subframe];
      RGSCHCMNADDTOCRNTTIME(timeInfo, timeInfo, msg3Subfrm);
      cellUl->msg3SchdIdx     = rgSCHCmnGetUlSfIdx(&timeInfo, cell);
      cellUl->msg3SchdHqProcIdx = rgSCHCmnGetUlHqProcIdx(&timeInfo, cell);
   }
#ifdef LTEMAC_SPS
   if(!rgSchTddSpsUlRsrvTbl[ulDlCfgIdx][idx])
   {
      cellUl->spsUlRsrvIdx = RGSCH_INVALID_INFO;
   }
   else
   {
      /* introduce some reuse with above code? */
      U8    offst;
      RGSCHCMNADDTOCRNTTIME(cell->crntTime,timeInfo,RG_SCH_CMN_DL_DELTA)
      //offst = rgSchTddMsg3SubfrmTbl[ulDlCfgIdx][timeInfo.subframe];
      offst = rgSchTddSpsUlRsrvTbl[ulDlCfgIdx][timeInfo.subframe];
      RGSCHCMNADDTOCRNTTIME(timeInfo, timeInfo, offst);
      cellUl->spsUlRsrvIdx     = rgSCHCmnGetUlSfIdx(&timeInfo, cell);
      /* The harq proc continues to be accessed and used the same delta before
       * actual data occurance, and hence use the same idx */
      cellUl->spsUlRsrvHqProcIdx = cellUl->schdHqProcIdx;
   }
#endif

   /* RACHO: update cmn sched specific RACH variables,
    * mainly the prachMaskIndex */
   rgSCHCmnUpdRachParam(cell);

   RETVOID;
}

/**
 * @brief To get 'p' value from nCCE.
 *
 * @details
 *
 *     Function: rgSCHCmnGetPValFrmCCE
 *     Purpose:  Gets 'p' value for HARQ ACK/NACK reception from CCE.
 *
 *  @param[in]  RgSchCellCb   *cell
 *  @param[in]  U8            cce
 *  @return U8
 *
 **/
#ifdef ANSI
PUBLIC U8  rgSCHCmnGetPValFrmCCE
(
RgSchCellCb *cell,
U8          cce
)
#else
PUBLIC U8  rgSCHCmnGetPValFrmCCE(cell, cce)
RgSchCellCb *cell;
U8          cce;
#endif
{
   U8 i;
   TRC2(rgSCHCmnGetPValFrmCCE);

   for(i=1; i < RGSCH_TDD_MAX_P_PLUS_ONE_VAL; i++)
   {
      if(cce < cell->rgSchTddNpValTbl[i])
      {
         RETVALUE(i-1);
      }
   }
   RETVALUE(0);
}
#endif

/***********************************************************
 *
 *     Func : rgSCHCmnUlAdapRetx
 *
 *     Desc : Adaptive retransmission for an allocation.
 *
 *     Ret  :
 *
 *     Notes:
 *
 *     File :
 *
 **********************************************************/
#ifdef ANSI
PRIVATE Void rgSCHCmnUlAdapRetx
(
RgSchUlAlloc    *alloc,
RgSchUlHqProcCb *proc
)
#else
PRIVATE Void rgSCHCmnUlAdapRetx(alloc, proc)
RgSchUlAlloc    *alloc;
RgSchUlHqProcCb *proc;
#endif
{
   TRC2(rgSCHCmnUlAdapRetx);

   rgSCHUhmRetx(proc, alloc);
   if (proc->rvIdx != 0)
   {
      alloc->grnt.iMcsCrnt = rgSchCmnUlRvIdxToIMcsTbl[proc->rvIdx];
   }
   else
   {
      alloc->grnt.iMcsCrnt = alloc->grnt.iMcs;
   }
   RETVOID;
}

/**
 * @brief Scheduler invocation per TTI.
 *
 * @details
 *
 *     Function: rgSCHCmnHdlUlInactUes
 *     Purpose:
 *
 *     Invoked by: Common Scheduler
 *
 *  @param[in]  RgSchCellCb *cell
 *  @return  Void
 **/
#ifdef ANSI
PRIVATE Void rgSCHCmnHdlUlInactUes
(
RgSchCellCb  *cell
)
#else
PRIVATE Void rgSCHCmnHdlUlInactUes(cell)
RgSchCellCb  *cell;
#endif
{
   RgSchCmnCell  *cellSch  = RG_SCH_CMN_GET_CELL(cell);
   CmLListCp     ulInactvLst;
   TRC2(rgSCHCmnHdlUlInactUes);
   /* Get a List of Inactv UEs for UL*/
   cmLListInit(&ulInactvLst);

   /* Trigger Spfc Schedulers with Inactive UEs */
   rgSCHMeasGapANRepGetUlInactvUe (cell, &ulInactvLst);
   /* take care of this in UL retransmission */
   cellSch->apisUl->rgSCHUlInactvtUes(cell, &ulInactvLst);

   RETVOID;
}

/**
 * @brief Scheduler invocation per TTI.
 *
 * @details
 *
 *     Function: rgSCHCmnHdlDlInactUes
 *     Purpose:
 *
 *     Invoked by: Common Scheduler
 *
 *  @param[in]  RgSchCellCb *cell
 *  @return  Void
 **/
#ifdef ANSI
PRIVATE Void rgSCHCmnHdlDlInactUes
(
RgSchCellCb  *cell
)
#else
PRIVATE Void rgSCHCmnHdlDlInactUes(cell)
RgSchCellCb  *cell;
#endif
{
   RgSchCmnCell *cellSch  = RG_SCH_CMN_GET_CELL(cell);
   CmLListCp    dlInactvLst;
   TRC2(rgSCHCmnHdlDlInactUes);
   /* Get a List of Inactv UEs for DL */
   cmLListInit(&dlInactvLst);

   /* Trigger Spfc Schedulers with Inactive UEs */
   rgSCHMeasGapANRepGetDlInactvUe (cell, &dlInactvLst);

   cellSch->apisDl->rgSCHDlInactvtUes(cell, &dlInactvLst);
   RETVOID;
}

/* RACHO: Rach handover functions start here */
/***********************************************************
 *
 *     Func : rgSCHCmnUeIdleExdThrsld
 *
 *     Desc : RETURN ROK if UE has been idle more
 *            than threshold.
 *
 *     Ret  :
 *
 *     Notes:
 *
 *     File :
 *
 **********************************************************/
#ifdef ANSI
PRIVATE S16 rgSCHCmnUeIdleExdThrsld
(
RgSchCellCb     *cell,
RgSchUeCb       *ue
)
#else
PRIVATE S16 rgSCHCmnUeIdleExdThrsld(cell, ue)
RgSchCellCb     *cell;
RgSchUeCb       *ue;
#endif
{
   RgSchCmnUlUe  *ueUl = RG_SCH_CMN_GET_UL_UE(ue);
   /* Time difference in subframes */
   U32 sfDiff = RGSCH_CALC_SF_DIFF(cell->crntTime, ueUl->ulTransTime);

   TRC2(rgSCHCmnUeIdleExdThrsld);

   if (sfDiff > (U32)RG_SCH_CMN_UE_IDLE_THRSLD(ue))
   {
      RETVALUE(ROK);
   }
   else
   {
      RETVALUE(RFAILED);
   }
}


/**
 * @brief Scheduler processing for Ded Preambles on cell configuration.
 *
 * @details
 *
 *     Function : rgSCHCmnCfgRachDedPrm
 *
 *     This function does requisite initialisation
 *     for RACH Ded Preambles.
 *
 *
 *  @param[in]  RgSchCellCb   *cell
 *  @return  Void
 **/
#ifdef ANSI
PRIVATE Void rgSCHCmnCfgRachDedPrm
(
RgSchCellCb   *cell
)
#else
PRIVATE Void rgSCHCmnCfgRachDedPrm(cell)
RgSchCellCb   *cell;
#endif
{
   RgSchCmnCell *cellSch = (RgSchCmnCell *)(cell->sc.sch);
   U32          gap = RG_SCH_CMN_MIN_PRACH_OPPR_GAP;
   U32          sfDiff;
   U8           cnt;
   TRC2(rgSCHCmnCfgRachDedPrm);

   if (cell->macPreambleSet.pres == NOTPRSNT)
   {
      RETVOID;
   }
   cellSch->rachCfg.numDedPrm = cell->macPreambleSet.size;
   cellSch->rachCfg.dedPrmStart = cell->macPreambleSet.start;
   /* Initialize handover List */
   cmLListInit(&cellSch->rachCfg.hoUeLst);
   /* Initialize pdcch Order List */
   cmLListInit(&cellSch->rachCfg.pdcchOdrLst);

   /* Intialize the rapId to UE mapping structure */
   for (cnt = 0; cnt<cellSch->rachCfg.numDedPrm; cnt++)
   {
      cellSch->rachCfg.rapIdMap[cnt].rapId = cellSch->rachCfg.dedPrmStart + \
                                             cnt;
      cmLListInit(&cellSch->rachCfg.rapIdMap[cnt].assgndUes);
   }
   /* Perform Prach Mask Idx, remDedPrm, applFrm initializations */
   /* Set remDedPrm as numDedPrm */
   cellSch->rachCfg.remDedPrm = cellSch->rachCfg.numDedPrm;
   /* Initialize applFrm */
   cellSch->rachCfg.prachMskIndx = 0;
   if (cell->rachCfg.raOccasion.sfnEnum == RGR_SFN_EVEN)
   {
      cellSch->rachCfg.applFrm.sfn = (cell->crntTime.sfn + \
            (cell->crntTime.sfn % 2)) % RGSCH_MAX_SFN;
   }
#ifdef LTE_TDD
   else if (cell->rachCfg.raOccasion.sfnEnum == RGR_SFN_ODD)
   {
      if((cell->crntTime.sfn%2) == 0)
      {
         cellSch->rachCfg.applFrm.sfn = (cell->crntTime.sfn + 1)\
                                        % RGSCH_MAX_SFN;
      }
   }
#endif
   else /* ANY sfn */
   {
      cellSch->rachCfg.applFrm.sfn = cell->crntTime.sfn;
   }
   /* Initialize cellSch->rachCfg.applFrm as >= crntTime.
    * This is because of RGSCH_CALC_SF_DIFF logic */
   if (cellSch->rachCfg.applFrm.sfn == cell->crntTime.sfn)
   {
      while (cellSch->rachCfg.prachMskIndx < cell->rachCfg.raOccasion.size)
      {
         if (cell->crntTime.subframe <\
               cell->rachCfg.raOccasion.subFrameNum[cellSch->rachCfg.prachMskIndx])
         {
            break;
         }
         cellSch->rachCfg.prachMskIndx++;
      }
      if (cellSch->rachCfg.prachMskIndx == cell->rachCfg.raOccasion.size)
      {
         if (cell->rachCfg.raOccasion.sfnEnum == RGR_SFN_ANY)
         {
            cellSch->rachCfg.applFrm.sfn = (cellSch->rachCfg.applFrm.sfn+1) %\
                                           RGSCH_MAX_SFN;
         }
         else
         {
            cellSch->rachCfg.applFrm.sfn = (cellSch->rachCfg.applFrm.sfn+2) %\
                                           RGSCH_MAX_SFN;
         }
         cellSch->rachCfg.prachMskIndx = 0;
      }
      cellSch->rachCfg.applFrm.subframe = \
                                          cell->rachCfg.raOccasion.subFrameNum[cellSch->rachCfg.prachMskIndx];
   }
   else
   {
      cellSch->rachCfg.applFrm.subframe = \
                                          cell->rachCfg.raOccasion.subFrameNum[cellSch->rachCfg.prachMskIndx];
   }

   /* Note first param to this macro should always be the latest in time */
   sfDiff = RGSCH_CALC_SF_DIFF(cellSch->rachCfg.applFrm, cell->crntTime);
   while (sfDiff <= gap)
   {
      rgSCHCmnUpdNxtPrchMskIdx(cell);
      sfDiff = RGSCH_CALC_SF_DIFF(cellSch->rachCfg.applFrm, cell->crntTime);
   }

   RETVOID;
}

/**
 * @brief Updates the PRACH MASK INDEX.
 *
 * @details
 *
 *     Function: rgSCHCmnUpdNxtPrchMskIdx
 *     Purpose:  Ensures the "applFrm" field of Cmn Sched RACH
 *     CFG is always >= "n"+"DELTA", where "n" is the crntTime
 *     of the cell. If not, applFrm is updated to the next avl
 *     PRACH oppurtunity as per the PRACH Cfg Index configuration.
 *
 *
 *     Invoked by: Common Scheduler
 *
 *  @param[in]  RgSchCellCb *cell
 *  @return  Void
 **/
#ifdef ANSI
PRIVATE Void rgSCHCmnUpdNxtPrchMskIdx
(
RgSchCellCb  *cell
)
#else
PRIVATE Void rgSCHCmnUpdNxtPrchMskIdx(cell)
RgSchCellCb  *cell;
#endif
{
   RgSchCmnCell    *cellSch = (RgSchCmnCell *)(cell->sc.sch);
   TRC2(rgSCHCmnUpdNxtPrchMskIdx);

   /* Determine the next prach mask Index */
   if (cellSch->rachCfg.prachMskIndx == cell->rachCfg.raOccasion.size - 1)
   {
      /* PRACH within applFrm.sfn are done, go to next AVL sfn */
      cellSch->rachCfg.prachMskIndx = 0;
      if (cell->rachCfg.raOccasion.sfnEnum == RGR_SFN_ANY)
      {
         cellSch->rachCfg.applFrm.sfn = (cellSch->rachCfg.applFrm.sfn+1) % \
                                        RGSCH_MAX_SFN;
      }
      else/* RGR_SFN_EVEN or RGR_SFN_ODD */
      {
         cellSch->rachCfg.applFrm.sfn = (cellSch->rachCfg.applFrm.sfn+2) % \
                                        RGSCH_MAX_SFN;
      }
      cellSch->rachCfg.applFrm.subframe = cell->rachCfg.raOccasion.\
                                          subFrameNum[0];
   }
   else /* applFrm.sfn is still valid */
   {
      cellSch->rachCfg.prachMskIndx += 1;
      if ( cellSch->rachCfg.prachMskIndx < RGR_MAX_SUBFRAME_NUM )
      {
         cellSch->rachCfg.applFrm.subframe = \
                                          cell->rachCfg.raOccasion.subFrameNum[cellSch->rachCfg.prachMskIndx];
      }
   }
   RETVOID;
}

/**
 * @brief Updates the Ded preamble RACH parameters
 *        every TTI.
 *
 * @details
 *
 *     Function: rgSCHCmnUpdRachParam
 *     Purpose:  Ensures the "applFrm" field of Cmn Sched RACH
 *     CFG is always >= "n"+"6"+"DELTA", where "n" is the crntTime
 *     of the cell. If not, applFrm is updated to the next avl
 *     PRACH oppurtunity as per the PRACH Cfg Index configuration,
 *     accordingly the "remDedPrm" is reset to "numDedPrm" and
 *     "prachMskIdx" field is updated as per "applFrm".
 *
 *
 *     Invoked by: Common Scheduler
 *
 *  @param[in]  RgSchCellCb *cell
 *  @return  Void
 **/
#ifdef ANSI
PRIVATE Void rgSCHCmnUpdRachParam
(
RgSchCellCb  *cell
)
#else
PRIVATE Void rgSCHCmnUpdRachParam(cell)
RgSchCellCb  *cell;
#endif
{

   RgSchCmnCell    *cellSch = (RgSchCmnCell *)(cell->sc.sch);
   U32             gap = RG_SCH_CMN_MIN_PRACH_OPPR_GAP;
   U32             sfDiff;
   TRC2(rgSCHCmnUpdRachParam);

   if (cell->macPreambleSet.pres == NOTPRSNT)
   {
      RETVOID;
   }
   sfDiff = RGSCH_CALC_SF_DIFF(cellSch->rachCfg.applFrm, \
         cell->crntTime);
   if (sfDiff > gap)
   {
      /* applFrm is still a valid next Prach Oppurtunity */
      RETVOID;
   }
   rgSCHCmnUpdNxtPrchMskIdx(cell);
   /* Reset remDedPrm as numDedPrm */
   cellSch->rachCfg.remDedPrm = cellSch->rachCfg.numDedPrm;

   RETVOID;
}

/**
 * @brief Dedicated Preamble allocation function.
 *
 * @details
 *
 *     Function: rgSCHCmnAllocPOParam
 *     Purpose:  Allocate pdcch, rapId and PrachMskIdx.
 *     Set mapping of UE with the allocated rapId.
 *
 *     Invoked by: Common Scheduler
 *
 *  @param[in]   RgSchCellCb *cell
 *  @param[in]   RgSchDlSf   *dlSf
 *  @param[in]   RgSchUeCb   *ue
 *  @param[out]  RgSchPdcch  **pdcch
 *  @param[out]  U8          *rapId
 *  @param[out]  U8          *prachMskIdx
 *  @return  Void
 **/
#ifdef ANSI
PRIVATE S16 rgSCHCmnAllocPOParam
(
RgSchCellCb  *cell,
RgSchDlSf    *dlSf,
RgSchUeCb    *ue,
RgSchPdcch   **pdcch,
U8           *rapId,
U8           *prachMskIdx
)
#else
PRIVATE S16 rgSCHCmnAllocPOParam(cell, dlSf, ue, pdcch, rapId, prachMskIdx)
RgSchCellCb  *cell;
RgSchDlSf    *dlSf;
RgSchUeCb    *ue;
RgSchPdcch   **pdcch;
U8           *rapId;
U8           *prachMskIdx;
#endif
{

   RgSchCmnCell    *cellSch = (RgSchCmnCell *)(cell->sc.sch);
   RgSchCmnDlUe    *ueDl = RG_SCH_CMN_GET_DL_UE(ue);
   U32             y;

   TRC2(rgSCHCmnAllocPOParam);

   if (cell->macPreambleSet.pres == PRSNT_NODEF)
   {
      if (cellSch->rachCfg.remDedPrm == 0)
      {
         RETVALUE(RFAILED);
      }
      y = rgSCHUtlGetY(ue, dlSf->sfNum);
      /* DTX Changes: One Variable is passed to check whether it is DTX or Not */
      if ((*pdcch = rgSCHCmnPdcchAlloc(cell, dlSf, y, ueDl->mimoInfo.cwInfo[0].cqi, TFU_DCI_FORMAT_1A, FALSE)) == NULLP)
      {
         RETVALUE(RFAILED);
      }
      /* The stored prachMskIdx is the index of PRACH Oppurtunities in
       * raOccasions.subframes[].
       * Converting the same to the actual PRACHMskIdx to be transmitted. */
      *prachMskIdx = cellSch->rachCfg.prachMskIndx + 1;
      /* Distribution starts from dedPrmStart till dedPrmStart + numDedPrm */
      *rapId =  cellSch->rachCfg.dedPrmStart +
         cellSch->rachCfg.numDedPrm - cellSch->rachCfg.remDedPrm;
      cellSch->rachCfg.remDedPrm--;
      /* Map UE with the allocated RapId */
      ueDl->rachInfo.asgnOppr = cellSch->rachCfg.applFrm;
      RGSCH_ARRAY_BOUND_CHECK_WITH_POS_IDX(cell->instIdx, cellSch->rachCfg.rapIdMap, (*rapId - cellSch->rachCfg.dedPrmStart));
      cmLListAdd2Tail(&cellSch->rachCfg.rapIdMap[*rapId - cellSch->rachCfg.dedPrmStart].assgndUes, 
             &ueDl->rachInfo.rapIdLnk);
      ueDl->rachInfo.rapIdLnk.node = (PTR)ue;
      ueDl->rachInfo.poRapId = *rapId;
   }
   else /* if dedicated preambles not configured */
   {
      y = rgSCHUtlGetY(ue, dlSf->sfNum);
      /* DTX Changes: One Variable is passed to check whether it is DTX or Not */
      if ((*pdcch = rgSCHCmnPdcchAlloc(cell, dlSf, y, ueDl->mimoInfo.cwInfo[0].cqi, TFU_DCI_FORMAT_1A, FALSE)) == NULLP)
      {
         RETVALUE(RFAILED);
      }
      *prachMskIdx = 0;
      *rapId       = 0;
   }

   RETVALUE(ROK);
}

/**
 * @brief Dowlink Scheduling Handler.
 *
 * @details
 *
 *     Function: rgSCHCmnGenPdcchOrder
 *     Purpose:  For each UE in PO Q, grab a PDCCH,
 *     get an available ded RapId and fill PDCCH
 *     with PO information.
 *
 *     Invoked by: Common Scheduler
 *
 *  @param[in]  RgSchCellCb *cell
 *  @param[in]  RgSchDlSf   *dlSf
 *  @return  Void
 **/
#ifdef ANSI
PRIVATE Void rgSCHCmnGenPdcchOrder
(
RgSchCellCb  *cell,
RgSchDlSf    *dlSf
)
#else
PRIVATE Void rgSCHCmnGenPdcchOrder(cell, dlSf)
RgSchCellCb  *cell;
RgSchDlSf    *dlSf;
#endif
{
   RgSchCmnCell      *cellSch = RG_SCH_CMN_GET_CELL(cell);
   CmLList           *node = cellSch->rachCfg.pdcchOdrLst.first;
   RgSchUeCb         *ue;
   U8                rapId;
   U8                prachMskIdx;
   RgSchPdcch        *pdcch = NULLP;

   TRC2(rgSCHCmnGenPdcchOrder);

   while (node)
   {
      ue = (RgSchUeCb *)node->node;
      node = node->next;
      /* Skip sending for this subframe is Measuring or inActive in UL due
       * to MeasGap or inactie due to DRX
       */
      if  ((ue->measGapCb.isMeasuring == TRUE) ||
           (ue->ul.ulInactvMask & RG_MEASGAP_INACTIVE) ||
           (ue->isDrxEnabled &&
             ue->dl.dlInactvMask & RG_DRX_INACTIVE)
           )
      {
         continue;
      }
      if (rgSCHCmnAllocPOParam(cell, dlSf, ue, &pdcch, &rapId,\
               &prachMskIdx) != ROK)
      {
         /* No More rapIds left for the valid next avl Oppurtunity.
          * Unsatisfied UEs here would be given a chance, when the
          * prach Mask Index changes as per rachUpd every TTI */

         /* PDDCH can also be ordered with rapId=0, prachMskIdx=0
          * so that UE triggers a RACH procedure with non-dedicated preamble.
          * But the implementation here does not do this. Instead, the "break"
          * here implies, that PDCCH Odr always given with valid rapId!=0,
          * prachMskIdx!=0 if dedicated preambles are configured.
          * If not configured, then trigger a PO with rapId=0,prchMskIdx=0*/
         break;
      }
      /* Fill pdcch with pdcch odr information */
      rgSCHCmnFillPdcchOdr2Sf(ue, pdcch, rapId, prachMskIdx);
      /* Remove this UE from the PDCCH ORDER QUEUE */
      rgSCHCmnDlRmvFrmPdcchOdrQ(cell, ue);
      /* Reset UE's power state */
      rgSCHPwrUeReset(cell, ue);
   }
   RETVOID;
}


/**
 * @brief This function add UE to PdcchOdr Q if not already present.
 *
 * @details
 *
 *     Function: rgSCHCmnDlAdd2PdcchOdrQ
 *     Purpose:
 *
 *     Invoked by: CMN Scheduler
 *
 *  @param[in]  RgSchCellCb*  cell
 *  @param[in]  RgSchUeCb*    ue
 *  @return  Void
 *
 **/
#ifdef ANSI
PRIVATE Void rgSCHCmnDlAdd2PdcchOdrQ
(
RgSchCellCb                *cell,
RgSchUeCb                  *ue
)
#else
PRIVATE Void rgSCHCmnDlAdd2PdcchOdrQ(cell, ue)
RgSchCellCb                *cell;
RgSchUeCb                  *ue;
#endif
{
   RgSchCmnCell *cellSch = RG_SCH_CMN_GET_CELL(cell);
   RgSchCmnDlUe *ueDl = RG_SCH_CMN_GET_DL_UE(ue);

   TRC2(rgSCHCmnDlAdd2PdcchOdrQ);

   if (ueDl->rachInfo.poLnk.node == NULLP)
   {
      cmLListAdd2Tail(&cellSch->rachCfg.pdcchOdrLst, &ueDl->rachInfo.poLnk);
      ueDl->rachInfo.poLnk.node = (PTR)ue;
   }
   RETVOID;
}


/**
 * @brief This function rmvs UE to PdcchOdr Q if not already present.
 *
 * @details
 *
 *     Function: rgSCHCmnDlRmvFrmPdcchOdrQ
 *     Purpose:
 *
 *     Invoked by: CMN Scheduler
 *
 *  @param[in]  RgSchCellCb*  cell
 *  @param[in]  RgSchUeCb*    ue
 *  @return  Void
 *
 **/
#ifdef ANSI
PRIVATE Void rgSCHCmnDlRmvFrmPdcchOdrQ
(
RgSchCellCb                *cell,
RgSchUeCb                  *ue
)
#else
PRIVATE Void rgSCHCmnDlRmvFrmPdcchOdrQ(cell, ue)
RgSchCellCb                *cell;
RgSchUeCb                  *ue;
#endif
{
   RgSchCmnCell *cellSch = RG_SCH_CMN_GET_CELL(cell);
   RgSchCmnDlUe *ueDl = RG_SCH_CMN_GET_DL_UE(ue);

   TRC2(rgSCHCmnDlRmvFrmPdcchOdrQ);

   cmLListDelFrm(&cellSch->rachCfg.pdcchOdrLst, &ueDl->rachInfo.poLnk);
   ueDl->rachInfo.poLnk.node = NULLP;
   RETVOID;
}

/**
 * @brief Fill pdcch with PDCCH order information.
 *
 * @details
 *
 *     Function: rgSCHCmnFillPdcchOdr2Sf
 *     Purpose:  Fill PDCCH with PDCCH order information,
 *
 *     Invoked by: Common Scheduler
 *
 *  @param[in]  RgSchUeCb   *ue
 *  @param[in]  RgSchPdcch  *pdcch
 *  @param[in]  U8          rapId
 *  @param[in]  U8          prachMskIdx
 *  @return  Void
 **/
#ifdef ANSI
PRIVATE Void rgSCHCmnFillPdcchOdr2Sf
(
RgSchUeCb   *ue,
RgSchPdcch  *pdcch,
U8          rapId,
U8          prachMskIdx
)
#else
PRIVATE Void rgSCHCmnFillPdcchOdr2Sf(ue, pdcch, rapId, prachMskIdx)
RgSchUeCb   *ue;
RgSchPdcch  *pdcch;
U8          rapId;
U8          prachMskIdx;
#endif
{

   TRC2(rgSCHCmnFillPdcchOdr2Sf);

   pdcch->rnti                                         = ue->ueId;
   pdcch->dci.dciFormat                                = TFU_DCI_FORMAT_1A;
   pdcch->dci.u.format1aInfo.isPdcchOrder = TRUE;
   pdcch->dci.u.format1aInfo.t.pdcchOrder.preambleIdx  = rapId;
   pdcch->dci.u.format1aInfo.t.pdcchOrder.prachMaskIdx = prachMskIdx;

   /* Request for APer CQI immediately after PDCCH Order */
   /* CR ccpu00144525 */
#ifdef TFU_UPGRADE
   if(ue->dl.ueDlCqiCfg.aprdCqiCfg.pres)
   {
      ue->dl.reqForCqi = TRUE;
      ue->acqiCb.aCqiTrigWt = 0;
   }
#endif   

   RETVOID;
}


/**
 * @brief UE deletion for scheduler.
 *
 * @details
 *
 *     Function : rgSCHCmnDelRachInfo
 *
 *     This functions deletes all scheduler information
 *     pertaining to an UE.
 *
 *  @param[in]  RgSchCellCb  *cell
 *  @param[in]  RgSchUeCb    *ue
 *  @return  Void
 **/
#ifdef ANSI
PRIVATE Void rgSCHCmnDelRachInfo
(
RgSchCellCb  *cell,
RgSchUeCb    *ue
)
#else
PRIVATE Void rgSCHCmnDelRachInfo(cell, ue)
RgSchCellCb  *cell;
RgSchUeCb    *ue;
#endif
{
   RgSchCmnCell *cellSch = RG_SCH_CMN_GET_CELL(cell);
   RgSchCmnDlUe *ueDl = RG_SCH_CMN_GET_DL_UE(ue);
   U8            rapIdIdx;

   TRC2(rgSCHCmnDelRachInfo);

   if (ueDl->rachInfo.poLnk.node)
   {
      rgSCHCmnDlRmvFrmPdcchOdrQ(cell, ue);
   }
   if (ueDl->rachInfo.hoLnk.node)
   {
      cmLListDelFrm(&cellSch->rachCfg.hoUeLst, &ueDl->rachInfo.hoLnk);
      ueDl->rachInfo.hoLnk.node = NULLP;
   }
   if (ueDl->rachInfo.rapIdLnk.node)
   {
      rapIdIdx = ueDl->rachInfo.poRapId - cellSch->rachCfg.dedPrmStart;
      cmLListDelFrm(&cellSch->rachCfg.rapIdMap[rapIdIdx].assgndUes, 
          &ueDl->rachInfo.rapIdLnk);
      ueDl->rachInfo.rapIdLnk.node = NULLP;
   }
   RETVOID;
}

/**
 * @brief This function retrieves the ue which has sent this raReq
 * and it allocates grant for UEs undergoing (for which RAR
 * is being generated) HandOver/PdcchOrder.
 *
 *
 * @details
 *
 *     Function: rgSCHCmnHdlHoPo
 *     Purpose:  This function  retrieves the ue which has sent this raReq
 *               and it allocates grant for UEs undergoing (for which RAR
 *               is being generated) HandOver/PdcchOrder.
 *
 *     Invoked by: Common Scheduler
 *
 *  @param[in]  RgSchCellCb           *cell
 *  @param[out] CmLListCp             *raRspLst
 *  @param[in]  RgSchRaReqInfo        *raReq
 *  @return  Void
 *
 **/
#ifdef ANSI
PRIVATE Void rgSCHCmnHdlHoPo
(
RgSchCellCb           *cell,
CmLListCp             *raRspLst,
RgSchRaReqInfo        *raReq
)
#else
PRIVATE Void rgSCHCmnHdlHoPo(cell, raRspLst, raReq)
RgSchCellCb           *cell;
CmLListCp             *raRspLst;
RgSchRaReqInfo        *raReq;
#endif
{
   RgSchUeCb             *ue = raReq->ue;
   TRC2(rgSCHCmnHdlHoPo);

   if ( ue->isDrxEnabled )
   {
      rgSCHDrxDedRa(cell,ue);
   }
   rgSCHCmnAllocPoHoGrnt(cell, raRspLst, ue, raReq);
   RETVOID;
}

/**
 * @brief This function retrieves the UE which has sent this raReq
 * for handover case.
 *
 *
 * @details
 *
 *     Function: rgSCHCmnGetHoUe
 *     Purpose:  This function retrieves the UE which has sent this raReq
 *     for handover case.
 *
 *     Invoked by: Common Scheduler
 *
 *  @param[in]  RgSchCellCb           *cell
 *  @param[in]  RgSchRaReqInfo        *raReq
 *  @return  RgSchUeCb*
 *
 **/
#ifdef ANSI
PRIVATE RgSchUeCb* rgSCHCmnGetHoUe
(
RgSchCellCb           *cell,
RgSchRaReqInfo        *raReq
)
#else
PRIVATE RgSchUeCb* rgSCHCmnGetHoUe(cell, raReq)
RgSchCellCb           *cell;
RgSchRaReqInfo        *raReq;
#endif
{
   RgSchCmnCell          *cellSch = (RgSchCmnCell *)(cell->sc.sch);
   CmLList               *node;
   CmLListCp             *ueLst;
   RgSchUeCb             *ue;
   RgSchCmnDlUe          *ueDl;
   TRC2(rgSCHCmnGetHoUe);

   ueLst = &cellSch->rachCfg.hoUeLst;
   node = ueLst->first;
   while (node)
   {
      ue = (RgSchUeCb *)node->node;
      node = node->next;
      ueDl = RG_SCH_CMN_GET_DL_UE(ue);
      if (ueDl->rachInfo.hoRapId == raReq->raReq.rapId)
      {
         RETVALUE(ue);
      }
   }
   RETVALUE(NULLP);
}

#if 1
#ifdef ANSI
PRIVATE Void rgSCHCmnDelDedPreamble
(
RgSchCellCb           *cell,
U8                    preambleId
)
#else
PRIVATE rgSCHCmnDelDedPreamble(cell, preambleId)
RgSchCellCb           *cell;
U8                    preambleId;
#endif
{
   RgSchCmnCell          *cellSch = (RgSchCmnCell *)(cell->sc.sch);
   CmLList               *node;
   CmLListCp             *ueLst;
   RgSchUeCb             *ue;
   RgSchCmnDlUe          *ueDl;
   TRC2(rgSCHCmnDelDedPreamble);

   ueLst = &cellSch->rachCfg.hoUeLst;
   node = ueLst->first;
   while (node)
   {
      ue = (RgSchUeCb *)node->node;
      node = node->next;
      ueDl = RG_SCH_CMN_GET_DL_UE(ue);
      if (ueDl->rachInfo.hoRapId == preambleId)
      {
         cmLListDelFrm(ueLst, &ueDl->rachInfo.hoLnk);
         ueDl->rachInfo.hoLnk.node = (PTR)NULLP;
      }
   }
}
#endif

/**
 * @brief This function retrieves the UE which has sent this raReq
 * for PDCCh Order case.
 *
 *
 * @details
 *
 *     Function: rgSCHCmnGetPoUe
 *     Purpose:  This function retrieves the UE which has sent this raReq
 *     for PDCCH Order case.
 *
 *     Invoked by: Common Scheduler
 *
 *  @param[in]  RgSchCellCb           *cell
 *  @param[in]  RgSchRaReqInfo        *raReq
 *  @return  RgSchUeCb*
 *
 **/
#ifdef ANSI
PRIVATE RgSchUeCb* rgSCHCmnGetPoUe
(
RgSchCellCb           *cell,
RgSchRaReqInfo        *raReq
)
#else
PRIVATE RgSchUeCb* rgSCHCmnGetPoUe(cell, raReq)
RgSchCellCb           *cell;
RgSchRaReqInfo        *raReq;
#endif
{
   RgSchCmnCell          *cellSch = (RgSchCmnCell *)(cell->sc.sch);
   CmLList               *node;
   CmLListCp             *ueLst;
   RgSchUeCb             *ue;
   RgSchCmnDlUe          *ueDl;
   U8                    rapIdIdx;
   TRC2(rgSCHCmnGetPoUe);

   rapIdIdx = raReq->raReq.rapId -cellSch->rachCfg.dedPrmStart;
   ueLst = &cellSch->rachCfg.rapIdMap[rapIdIdx].assgndUes;
   node = ueLst->first;
   while (node)
   {
      ue = (RgSchUeCb *)node->node;
      node = node->next;
      ueDl = RG_SCH_CMN_GET_DL_UE(ue);
      /* Remove UEs irrespective.
       * Old UE associations are removed.*/
      cmLListDelFrm(ueLst, &ueDl->rachInfo.rapIdLnk);
      ueDl->rachInfo.rapIdLnk.node = (PTR)NULLP;
      if (RGSCH_TIMEINFO_SAME(ueDl->rachInfo.asgnOppr, raReq->timingInfo))
      {
         RETVALUE(ue);
      }
   }

   RETVALUE(NULLP);
}


/**
 * @brief This function returns the valid UL cqi for a given UE.
 *
 * @details
 *
 *     Function: rgSCHCmnUlGetCqi
 *     Purpose:  This function returns the "valid UL cqi" for a given UE
 *               based on UE category
 *
 *     Invoked by: Scheduler
 *     
 *  @param[in]  RgSchUeCb        *ue
 *  @param[in]  U8               ueCtgy
 *  @return     U8 
 **/
#ifdef ANSI
PUBLIC U8 rgSCHCmnUlGetCqi
(
RgSchUeCb        *ue,
CmLteUeCategory  ueCtgy
)
#else
PUBLIC U8 rgSCHCmnUlGetCqi(ue, ueCtgy)
RgSchUeCb        *ue;
CmLteUeCategory  ueCtgy;
#endif
{
   RgSchCmnUlUe *ueUl    = RG_SCH_CMN_GET_UL_UE(ue);
   U8            cqi;

   TRC2(rgSCHCmnUlGetCqi);
   
   cqi = ueUl->maxUlCqi;
#ifdef TFU_UPGRADE
   if (!((ueCtgy != CM_LTE_UE_CAT_5) && 
        (ueUl->validUlCqi > ueUl->maxUlCqi)))
   {
      cqi = ueUl->validUlCqi;
   }
#else   
   if (!((ueCtgy != CM_LTE_UE_CAT_5) && 
         (ueUl->crntUlCqi[0] > ueUl->maxUlCqi )))
   {
      cqi = ueUl->crntUlCqi[0];
   }
#endif    
   RETVALUE(cqi);
}/* End of rgSCHCmnUlGetCqi */

/***********************************************************
 *
 *     Func : rgSCHCmnUlRbAllocForPoHoUe
 *
 *     Desc : Do uplink RB allocation for a HO/PO UE.
 *
 *     Ret  :
 *
 *     Notes: Note that as of now, for retx, maxRb
 *            is not considered. Alternatives, such
 *            as dropping retx if it crosses maxRb
 *            could be considered.
 *
 *     File :
 *
 **********************************************************/
#ifdef ANSI
PRIVATE S16 rgSCHCmnUlRbAllocForPoHoUe
(
RgSchCellCb           *cell,
RgSchUlSf             *sf,
RgSchUeCb             *ue,
U8                    maxRb
)
#else
PRIVATE S16 rgSCHCmnUlRbAllocForPoHoUe(cell, sf, ue, maxRb)
RgSchCellCb           *cell;
RgSchUlSf             *sf;
RgSchUeCb             *ue;
U8                    maxRb;
#endif
{
   RgSchCmnUlCell *cellUl = RG_SCH_CMN_GET_UL_CELL(cell);
   RgSchCmnUlUe *ueUl    = RG_SCH_CMN_GET_UL_UE(ue);
   U8           sbSize  = cellUl->sbSize;
   U32          maxBits = ueUl->maxBytesPerUePerTti*8;
   U32          bits;
   RgSchUlAlloc *alloc;
   U32          nPrb;
   U8           iTbs;
   U32          eff;
   U32          numSb;
   U8           iMcs;
   U8           iMcsCrnt;
   U8           cqi;
   U8           modOdr;
   RgSchUlHole      *hole;
   RgSchUlHqProcCb  *proc = &ue->ul.hqEnt.hqProcCb[cellUl->msg3SchdHqProcIdx];
   CmLteUeCategory ueCtg = (CmLteUeCategory)(RG_SCH_CMN_GET_UE_CTGY(ue));

   TRC2(rgSCHCmnUlRbAllocForPoHoUe);
   if ((hole = rgSCHUtlUlHoleFirst(sf)) == NULLP)
   {
      RETVALUE(RFAILED);
   }
   /*MS_WORKAROUND for HO ccpu00121116*/
   cqi   = rgSCHCmnUlGetCqi(ue, ueCtg);
   RGSCH_ARRAY_BOUND_CHECK(cell->instIdx, rgSchCmnUlCqiToTbsTbl[(U8)cell->isCpUlExtend], cqi);
   iTbs  = rgSchCmnUlCqiToTbsTbl[(U8)cell->isCpUlExtend][cqi];
   iMcs  = rgSCHCmnUlGetIMcsFrmITbs(iTbs,ueCtg);
   while(iMcs > RG_SCH_CMN_MAX_MSG3_IMCS)
   {
       cqi--;
       iTbs  = rgSchCmnUlCqiToTbsTbl[(U8)cell->isCpUlExtend][cqi];
       iMcs  = rgSCHCmnUlGetIMcsFrmITbs(iTbs, ueCtg);
   }
   /* Filling the modorder in the grant structure*/
   RG_SCH_UL_MCS_TO_MODODR(iMcs,modOdr);
   if (!cell->isCpUlExtend)
   {
      eff   = rgSchCmnNorUlEff[0][iTbs];
   }
   else
   {
      eff   = rgSchCmnExtUlEff[0][iTbs];
   }

   bits = ueUl->alloc.reqBytes * 8;

#if (ERRCLASS & ERRCLS_DEBUG)
   if (!bits)
   {
      RETVALUE(RFAILED);
   }
#endif

   if (bits < rgSCHCmnUlMinTbBitsForITbs(cellUl, iTbs))
   {
      numSb = 1;
      nPrb = numSb * sbSize;
   }
   else
   {
      if (bits > maxBits)
      {
         bits  = maxBits;
         nPrb  = bits * 1024 / eff / RG_SCH_CMN_UL_NUM_RE_PER_RB(cellUl);
         if (nPrb > maxRb)
         {
            nPrb = maxRb;
         }
         numSb = nPrb / sbSize;
      }
      else
      {
         /*ccpu00128775:MOD-Change to get upper threshold nPrb*/
         nPrb = RGSCH_CEIL((RGSCH_CEIL(bits * 1024, eff)),
                  RG_SCH_CMN_UL_NUM_RE_PER_RB(cellUl));
         if (nPrb > maxRb)
         {
            nPrb = maxRb;
         }
         numSb = RGSCH_DIV_ROUND(nPrb, sbSize);
      }
   }
   iMcsCrnt = iMcs;

   alloc = rgSCHCmnUlSbAlloc(sf, (U8)RGSCH_MIN(numSb, cellUl->maxSbPerUe),\
                             hole);
   if (alloc == NULLP)
   {
      RLOG_ARG0(L_ERROR,DBG_CELLID,cell->cellId,
         "rgSCHCmnUlRbAllocForPoHoUe(): Could not get UlAlloc");
      RETVALUE(RFAILED);
   }
   rgSCHCmnUlAllocFillRbInfo(cell, sf, alloc);
   
   /* Filling the modorder in the grant structure start*/
   alloc->grnt.modOdr = (TfuModScheme) modOdr;
   alloc->grnt.iMcs = iMcs;
   alloc->grnt.iMcsCrnt = iMcsCrnt;
   alloc->grnt.hop = 0;
   /* Fix for ccpu00123915*/
   alloc->forMsg3 = TRUE;
   alloc->hqProc = proc;
   alloc->hqProc->ulSfIdx = cellUl->msg3SchdIdx;
   alloc->ue = ue;
   alloc->rnti = ue->ueId;
   /* updating initNumRbs in case of HO */
#ifdef TFU_UPGRADE
   ue->initNumRbs = alloc->grnt.numRb;
#endif
   ueUl->alloc.alloc = alloc;
   iTbs = rgSCHCmnUlGetITbsFrmIMcs(iMcs);
   RGSCH_ARRAY_BOUND_CHECK(cell->instIdx, rgTbSzTbl[0], iTbs);
   alloc->grnt.datSz    = rgTbSzTbl[0][iTbs][alloc->grnt.numRb-1] / 8;
   /* MS_WORKAROUND for HO ccpu00121124*/
   /*[Adi temp change] Need to fil modOdr */
   RG_SCH_UL_MCS_TO_MODODR(alloc->grnt.iMcsCrnt,alloc->grnt.modOdr);
   rgSCHUhmNewTx(proc, ue->ul.hqEnt.maxHqRetx, alloc);
   /* No grant attr recorded now */
   RETVALUE(ROK);
}

/**
 * @brief This function allocates grant for UEs undergoing (for which RAR
 * is being generated) HandOver/PdcchOrder.
 *
 *
 * @details
 *
 *     Function: rgSCHCmnAllocPoHoGrnt
 *     Purpose:  This function allocates grant for UEs undergoing (for which RAR
 *               is being generated) HandOver/PdcchOrder.
 *
 *     Invoked by: Common Scheduler
 *
 *  @param[in]  RgSchCellCb           *cell
 *  @param[out] CmLListCp             *raRspLst,
 *  @param[in]  RgSchUeCb             *ue
 *  @param[in]  RgSchRaReqInfo        *raReq
 *  @return  Void
 *
 **/
#ifdef ANSI
PRIVATE Void rgSCHCmnAllocPoHoGrnt
(
RgSchCellCb           *cell,
CmLListCp             *raRspLst,
RgSchUeCb             *ue,
RgSchRaReqInfo        *raReq
)
#else
PRIVATE Void rgSCHCmnAllocPoHoGrnt(cell, raRspLst, ue, raReq)
RgSchCellCb           *cell;
CmLListCp             *raRspLst;
RgSchUeCb             *ue;
RgSchRaReqInfo        *raReq;
#endif
{
   RgSchCmnUlCell  *cellUl = RG_SCH_CMN_GET_UL_CELL(cell);
   RgSchCmnUlUe    *ueUl   = RG_SCH_CMN_GET_UL_UE(ue);
   RgSchUlGrnt     *grnt;
   RgSchUlSf       *sf = &cellUl->ulSfArr[cellUl->msg3SchdIdx];

   TRC2(rgSCHCmnAllocPoHoGrnt);

   /* Clearing previous allocs if any*/
   rgSCHCmnUlUeDelAllocs(cell, ue);
   /* Fix : syed allocs are limited */
   if (*sf->allocCountRef >= cellUl->maxAllocPerUlSf)
   {
      RETVOID;
   }
   ueUl->alloc.reqBytes = RG_SCH_MIN_GRNT_HOPO;
   if (rgSCHCmnUlRbAllocForPoHoUe(cell, sf, ue, RGSCH_MAX_UL_RB) != ROK)
   {
      RETVOID;
   }

   /* Fill grant information */
   grnt = &ueUl->alloc.alloc->grnt;

   /* KWork fix */
   if (grnt == NULLP)
   {
      RLOG_ARG1(L_ERROR,DBG_INSTID,cell->instIdx,  "Failed to get"
        "the grant for HO/PDCCH Order. CRNTI:%d",ue->ueId);
      RETVOID;
   }
   ue->ul.rarGrnt.rapId = raReq->raReq.rapId;
   ue->ul.rarGrnt.hop = grnt->hop;
   ue->ul.rarGrnt.rbStart = grnt->rbStart;
   ue->ul.rarGrnt.numRb = grnt->numRb;
   ue->ul.rarGrnt.tpc = grnt->tpc;
   ue->ul.rarGrnt.iMcsCrnt = grnt->iMcsCrnt;
   ue->ul.rarGrnt.ta.pres = TRUE;
   ue->ul.rarGrnt.ta.val = raReq->raReq.ta;
   ue->ul.rarGrnt.datSz = grnt->datSz;
   if((sf->aCqiReqCount < RG_SCH_MAX_ACQI_PER_ULSF) && (TRUE == ue->dl.reqForCqi)) 
   {
      ue->ul.rarGrnt.cqiReqBit = ue->dl.reqForCqi;
      ue->dl.reqForCqi = FALSE;
      sf->aCqiReqCount++;
   }
   else
   {
      ue->ul.rarGrnt.cqiReqBit = 0;
   }
   /* Attach Ho/Po allocation to RAR Rsp cont free Lst */
   cmLListAdd2Tail(raRspLst, &ue->ul.rarGrnt.raRspLnk);
   ue->ul.rarGrnt.raRspLnk.node = (PTR)ue;

   RETVOID;
}

/**
 * @brief This is a utility function to set the fields in
 * an UL harq proc which is identified for non-adaptive retx
 *
 * @details
 *
 *     Function: rgSCHCmnUlNonadapRetx 
 *     Purpose:  Sets the fields in UL Harq  proc for non-adaptive retx 
 *
 * @param[in]  RgSchCmnUlCell  *cellUl 
 * @param[out] RgSchUlAlloc    *alloc
 * @param[in]  U8              idx 
 * @return  Void
 *
 **/
#ifdef ANSI
PRIVATE Void rgSCHCmnUlNonadapRetx
(
RgSchCmnUlCell  *cellUl,
RgSchUlAlloc    *alloc,
U8              idx
)
#else
PRIVATE Void rgSCHCmnUlNonadapRetx(cellUl, alloc, idx)
RgSchCmnUlCell  *cellUl;
RgSchUlAlloc    *alloc;
U8              idx;
#endif
{
   TRC2(rgSCHCmnUlNonadapRetx);
   rgSCHUhmRetx(alloc->hqProc, alloc);

   /* Update alloc to retx */
   alloc->hqProc->isRetx = TRUE;
   alloc->hqProc->ulSfIdx = cellUl->reTxIdx[idx];

   if (alloc->hqProc->rvIdx != 0)
   {
      alloc->grnt.iMcsCrnt = rgSchCmnUlRvIdxToIMcsTbl[alloc->hqProc->rvIdx];
   }
   else
   {
      alloc->grnt.iMcsCrnt = alloc->grnt.iMcs;
   }
   alloc->grnt.isRtx = TRUE;
   alloc->pdcch = NULLP;
   RETVOID;
}

/**
 * @brief Check if 2 allocs overlap
 *
 * @details
 *
 *     Function : rgSCHCmnUlAllocsOvrLap
 *
 *      - Return TRUE if alloc1 and alloc2 overlap.
 *
 *  @param[in]  RgSchUlAlloc  *alloc1
 *  @param[in]  RgSchUlAlloc  *alloc2
 *  @return  Bool
 **/
#ifdef ANSI
PRIVATE Bool rgSCHCmnUlAllocsOvrLap
(
RgSchUlAlloc    *alloc1,
RgSchUlAlloc    *alloc2
)
#else
PRIVATE Bool rgSCHCmnUlAllocsOvrLap(alloc1, alloc2)
RgSchUlAlloc    *alloc1;
RgSchUlAlloc    *alloc2;
#endif
{

   TRC2(rgSCHCmnUlAllocsOvrLap);

   if (((alloc1->sbStart >= alloc2->sbStart) &&
         (alloc1->sbStart <= alloc2->sbStart + alloc2->numSb-1)) ||
        ((alloc2->sbStart >= alloc1->sbStart) &&
         (alloc2->sbStart <= alloc1->sbStart + alloc1->numSb-1)))
   {
      RETVALUE(TRUE);
   }
   RETVALUE(FALSE);
}

/**
 * @brief Copy allocation Info from src to dst.
 *
 * @details
 *
 *     Function : rgSCHCmnUlCpyAllocInfo
 *
 *      - Copy allocation Info from src to dst.
 *
 *  @param[in]  RgSchUlAlloc  *srcAlloc
 *  @param[in]  RgSchUlAlloc  *dstAlloc
 *  @return  Void
 **/
#ifdef ANSI
PRIVATE Void rgSCHCmnUlCpyAllocInfo
(
RgSchUlAlloc    *srcAlloc,
RgSchUlAlloc    *dstAlloc
)
#else
PRIVATE Void rgSCHCmnUlCpyAllocInfo(srcAlloc, dstAlloc)
RgSchUlAlloc    *srcAlloc;
RgSchUlAlloc    *dstAlloc;
#endif
{
   RgSchCmnUlUe *ueUl;
   TRC2(rgSCHCmnUlCpyAllocInfo);

   dstAlloc->grnt = srcAlloc->grnt;
   dstAlloc->hqProc = srcAlloc->hqProc;
   /* Fix : syed During UE context release, hqProc->alloc
    * was pointing to srcAlloc instead of dstAlloc and
    * freeing from incorrect sf->allocDb was
    * corrupting the list. */
    /* In case of SPS Occasion Allocation is done in advance and 
       at a later time Hq Proc is linked. Hence HqProc
       pointer in alloc shall be NULL */
#ifdef LTEMAC_SPS
   if (dstAlloc->hqProc)
#endif
   {
      dstAlloc->hqProc->alloc = dstAlloc;
   }
   dstAlloc->ue = srcAlloc->ue;
   dstAlloc->rnti = srcAlloc->rnti;
   dstAlloc->forMsg3 = srcAlloc->forMsg3;
   dstAlloc->raCb  = srcAlloc->raCb;
   dstAlloc->pdcch = srcAlloc->pdcch;
   /* Fix : syed HandIn Ue has forMsg3 and ue Set, but no RaCb */
   if (dstAlloc->ue)
   {
      ueUl = RG_SCH_CMN_GET_UL_UE(dstAlloc->ue);
      ueUl->alloc.alloc = dstAlloc;
#ifdef LTEMAC_SPS
      if (dstAlloc->ue->ul.ulSpsCfg.isUlSpsEnabled == TRUE)
      {
         if((ueUl->ulSpsInfo.ulSpsSchdInfo.crntAlloc != NULLP)
               && (ueUl->ulSpsInfo.ulSpsSchdInfo.crntAlloc == srcAlloc))
         {
            ueUl->ulSpsInfo.ulSpsSchdInfo.crntAlloc = dstAlloc;
         }
      }
#endif
   }

   RETVOID;
}


/**
 * @brief Update TX and RETX subframe's allocation
 *        markings.
 *
 * @details
 *
 *     Function : rgSCHCmnUlInsAllocFrmNewSf2OldSf
 *
 *      - Release all preassigned allocations of newSf and merge
 *        them to oldSf.
 *      - If alloc of newSf collide with one or more allocs of oldSf
 *        - mark all such allocs of oldSf for Adaptive Retx.
 *      - Swap the alloc and hole DB references of oldSf and newSf.
 *
 *  @param[in]  RgSchCellCb   *cell
 *  @param[in]  RgSchUlSf     *newSf
 *  @param[in]  RgSchUlSf     *oldSf
 *  @param[in]  RgSchUlAlloc  *srcAlloc
 *  @return  Void
 **/
#ifdef ANSI
PRIVATE Void rgSCHCmnUlInsAllocFrmNewSf2OldSf
(
RgSchCellCb     *cell,
RgSchUlSf       *newSf,
RgSchUlSf       *oldSf,
RgSchUlAlloc    *srcAlloc
)
#else
PRIVATE Void rgSCHCmnUlInsAllocFrmNewSf2OldSf(cell, newSf, oldSf, srcAlloc)
RgSchCellCb     *cell;
RgSchUlSf       *newSf;
RgSchUlSf       *oldSf;
RgSchUlAlloc    *srcAlloc;
#endif
{
   RgSchUlAlloc   *alloc, *dstAlloc, *nxtAlloc;

   /* MS_WORKAROUND ccpu00120827 */
   RgSchCmnCell *schCmnCell = (RgSchCmnCell *)(cell->sc.sch);
   U8 remAllocs;
   TRC2(rgSCHCmnUlInsAllocFrmNewSf2OldSf);

   if ((alloc = rgSCHUtlUlAllocFirst(oldSf)) != NULLP)
   {
      do
      {
         nxtAlloc = rgSCHUtlUlAllocNxt(oldSf, alloc);
         /* If there is an overlap between alloc and srcAlloc
          * then alloc is marked for Adaptive retx and it is released
          * from txSf */
         if (rgSCHCmnUlAllocsOvrLap(alloc, srcAlloc) == TRUE)
         {
            rgSCHCmnUlUpdAllocRetx(newSf, alloc);
            rgSCHUtlUlAllocRls(oldSf, alloc);
         }
         /* No further allocs spanning the srcAlloc subbands */
         if (srcAlloc->sbStart + srcAlloc->numSb - 1  <= alloc->sbStart)
         {
            break;
         }
      } while ((alloc = nxtAlloc) != NULLP);
   }

   /* After freeing all the colliding allocs, request for an allocation
    * specifying the start and numSb with in txSf. This function should
    * always return positively with a nonNULL dstAlloc */
    /* MS_WORKAROUND ccpu00120827 */
   remAllocs = schCmnCell->ul.maxAllocPerUlSf - *oldSf->allocCountRef;
   if (!remAllocs)
   {
      /* Fix : If oldSf already has max Allocs then release the
       * old RETX alloc to make space for new alloc of newSf.
       * newSf allocs(i.e new Msg3s) are given higher priority
       * over retx allocs. */       
      if ((alloc = rgSCHUtlUlAllocFirst(oldSf)) != NULLP)
      {
         do
         {
            nxtAlloc = rgSCHUtlUlAllocNxt(oldSf, alloc);      
            if (!alloc->mrgdNewTxAlloc)
            {
               /* If alloc is for RETX */          
               /* TODO: Incase of this ad also in case of choosing
                * and alloc for ADAP RETX, we need to send ACK for
                * the corresponding alloc in PHICH */          
               rgSCHCmnUlFreeAllocation(cell, oldSf, alloc);
               break;
            }          
         }while((alloc = nxtAlloc) != NULLP);
      }
   }
   dstAlloc = rgSCHUtlUlGetSpfcAlloc(oldSf, srcAlloc->sbStart, srcAlloc->numSb);
#ifdef ERRCLS_KW
   /* This should never happen */
   if (dstAlloc == NULLP)
   {
      RLOG_ARG1(L_ERROR,DBG_CELLID,cell->cellId,"CRNTI:%d "
         "rgSCHUtlUlGetSpfcAlloc failed in rgSCHCmnUlInsAllocFrmNewSf2OldSf",
         srcAlloc->rnti);
      RETVOID;
   }
#endif
   /* Copy the srcAlloc's state information in to dstAlloc */
   rgSCHCmnUlCpyAllocInfo(srcAlloc, dstAlloc);
   /* Set new Tx merged Alloc Flag to TRUE, indicating that this
    * alloc shall not be processed for non-adaptive retransmission */
   dstAlloc->mrgdNewTxAlloc = TRUE;
   RETVOID;
}

/**
 * @brief Merge all allocations of newSf to oldSf.
 *
 * @details
 *
 *     Function : rgSCHCmnUlMergeSfAllocs
 *
 *      - Merge all allocations of newSf to oldSf.
 *      - If newSf's alloc collides with oldSf's alloc
 *        then oldSf's alloc is marked for adaptive Retx
 *        and is released from oldSf to create space for
 *        newSf's alloc.
 *
 *  @param[in]  RgSchCellCb  *cell
 *  @param[in]  RgSchUlSf    *oldSf
 *  @param[in]  RgSchUlSf    *newSf
 *  @return  Void
 **/
#ifdef ANSI
PRIVATE Void rgSCHCmnUlMergeSfAllocs
(
RgSchCellCb  *cell,
RgSchUlSf    *oldSf,
RgSchUlSf    *newSf
)
#else
PRIVATE Void rgSCHCmnUlMergeSfAllocs(cell, oldSf, newSf)
RgSchCellCb  *cell;
RgSchUlSf    *oldSf;
RgSchUlSf    *newSf;
#endif
{
   RgSchUlAlloc    *alloc, *nxtAlloc;
   TRC2(rgSCHCmnUlMergeSfAllocs);
   UNUSED(cell);

   /* Merge each alloc of newSf in to oldSf
    * and release it from newSf */
   if ((alloc = rgSCHUtlUlAllocFirst(newSf)) != NULLP)
   {
      do
      {
         nxtAlloc = rgSCHUtlUlAllocNxt(newSf, alloc);
         rgSCHCmnUlInsAllocFrmNewSf2OldSf(cell, newSf, oldSf, alloc);
         rgSCHUtlUlAllocRls(newSf, alloc);
      } while((alloc = nxtAlloc) != NULLP);
   }
   RETVOID;
}

/**
 * @brief Swap Hole/Alloc DB context of newSf and oldSf.
 *
 * @details
 *
 *     Function : rgSCHCmnUlSwapSfAllocs
 *
 *      - Swap Hole/Alloc DB context of newSf and oldSf.
 *
 *  @param[in]  RgSchCellCb  *cell
 *  @param[in]  RgSchUlSf    *oldSf
 *  @param[in]  RgSchUlSf    *newSf
 *  @return  Void
 **/
#ifdef ANSI
PRIVATE Void rgSCHCmnUlSwapSfAllocs
(
RgSchCellCb  *cell,
RgSchUlSf    *oldSf,
RgSchUlSf    *newSf
)
#else
PRIVATE Void rgSCHCmnUlSwapSfAllocs(cell, oldSf, newSf)
RgSchCellCb  *cell;
RgSchUlSf    *oldSf;
RgSchUlSf    *newSf;
#endif
{
   RgSchUlAllocDb *tempAllocDb  = newSf->allocDb;
   RgSchUlHoleDb  *tempHoleDb   = newSf->holeDb;
   U8              tempAvailSbs = newSf->availSubbands;

   TRC2(rgSCHCmnUlSwapSfAllocs);
   UNUSED(cell);

   newSf->allocDb       = oldSf->allocDb;
   newSf->holeDb        = oldSf->holeDb;
   newSf->availSubbands = oldSf->availSubbands;

   oldSf->allocDb = tempAllocDb;
   oldSf->holeDb  = tempHoleDb;
   oldSf->availSubbands = tempAvailSbs;
      
   /* Fix ccpu00120610*/
   newSf->allocCountRef = &newSf->allocDb->count;
   oldSf->allocCountRef = &oldSf->allocDb->count;
   RETVOID;
}

/**
 * @brief Perform non-adaptive RETX for non-colliding allocs.
 *
 * @details
 *
 *     Function : rgSCHCmnUlPrcNonAdptRetx
 *
 *      - Perform non-adaptive RETX for non-colliding allocs.
 *
 *  @param[in]  RgSchCellCb  *cell
 *  @param[in]  RgSchUlSf    *newSf
 *  @param[in]  U8           idx
 *  @return  Void
 **/
#ifdef ANSI
PRIVATE Void rgSCHCmnUlPrcNonAdptRetx
(
RgSchCellCb  *cell,
RgSchUlSf    *newSf,
U8           idx
)
#else
PRIVATE Void rgSCHCmnUlPrcNonAdptRetx(cell, newSf, idx)
RgSchCellCb  *cell;
RgSchUlSf    *newSf;
U8           idx;
#endif
{
   RgSchCmnUlCell  *cellUl = RG_SCH_CMN_GET_UL_CELL(cell);
   RgSchUlAlloc    *alloc, *nxtAlloc;
   TRC2(rgSCHCmnUlPrcNonAdptRetx);

   /* perform non-adaptive retx allocation(adjustment) */
   if ((alloc = rgSCHUtlUlAllocFirst(newSf)) != NULLP)
   {
      do
      {
         nxtAlloc = rgSCHUtlUlAllocNxt(newSf, alloc);
         /* A merged new TX alloc, reset the state and skip */
         if (alloc->mrgdNewTxAlloc)
         {
            alloc->mrgdNewTxAlloc = FALSE;
            continue;
         }
         

         rgSCHCmnUlNonadapRetx(cellUl, alloc, idx);

      } while((alloc = nxtAlloc) != NULLP);
   }
   RETVOID;
}

/**
 * @brief Update TX and RETX subframe's allocation
 *        markings.
 *
 * @details
 *
 *     Function : rgSCHCmnUlPrfmSfMerge
 *
 *      - Release all preassigned allocations of newSf and merge
 *        them to oldSf.
 *      - If alloc of newSf collide with one or more allocs of oldSf
 *        - mark all such allocs of oldSf for Adaptive Retx.
 *      - Swap the alloc and hole DB references of oldSf and newSf.
 *      - The allocs which did not collide with pre-assigned msg3
 *        allocs are marked for non-adaptive RETX.
 *
 *  @param[in]  RgSchCellCb  *cell
 *  @param[in]  RgSchUlSf    *oldSf
 *  @param[in]  RgSchUlSf    *newSf
 *  @param[in]  U8           idx 
 *  @return  Void
 **/
#ifdef ANSI
PRIVATE Void rgSCHCmnUlPrfmSfMerge
(
RgSchCellCb  *cell,
RgSchUlSf    *oldSf,
RgSchUlSf    *newSf,
U8           idx
)
#else
PRIVATE Void rgSCHCmnUlPrfmSfMerge(cell, oldSf, newSf, idx)
RgSchCellCb  *cell;
RgSchUlSf    *oldSf;
RgSchUlSf    *newSf;
U8           idx;
#endif
{
   TRC2(rgSCHCmnUlPrfmSfMerge);
   /* Preassigned resources for msg3 in newSf.
    * Hence do adaptive retx for all NACKED TXs */
   rgSCHCmnUlMergeSfAllocs(cell, oldSf, newSf);
   /* swap alloc and hole DBs of oldSf and newSf. */
   rgSCHCmnUlSwapSfAllocs(cell, oldSf, newSf);
   /* Here newSf has the resultant merged allocs context */
   /* Perform non-adaptive RETX for non-colliding allocs */
   rgSCHCmnUlPrcNonAdptRetx(cell, newSf, idx);
   
   RETVOID;
}

/**
 * @brief Update TX and RETX subframe's allocation
 *        markings.
 *
 * @details
 *
 *     Function : rgSCHCmnUlRmvCmpltdAllocs
 *
 *      - Free all Transmission which are ACKED
 *        OR for which MAX retransmission have
 *        occurred.
 *
 *
 *  @param[in]  RgSchCellCb    *cell,
 *  @param[in]  RgSchUlSf      *sf
 *  @return  Void
 **/
#ifdef ANSI
PRIVATE Void rgSCHCmnUlRmvCmpltdAllocs
(
RgSchCellCb    *cell,
RgSchUlSf      *sf,
RgSchUlSf      *newSf
)
#else
PRIVATE Void rgSCHCmnUlRmvCmpltdAllocs(cell, sf, newSf)
RgSchCellCb    *cell;
RgSchUlSf      *sf;
RgSchUlSf      *newSf;
#endif
{
   RgSchUlAlloc    *alloc, *nxtAlloc;
   TRC2(rgSCHCmnUlRmvCmpltdAllocs);

   if ((alloc = rgSCHUtlUlAllocFirst(sf)) == NULLP)
   {
      RETVOID;
   }
   do
   {
      nxtAlloc = rgSCHUtlUlAllocNxt(sf, alloc);
      if ((alloc->hqProc->rcvdCrcInd) || (alloc->hqProc->remTx == 0))
      {

        /* SR_RACH_STATS : MSG 3 MAX RETX FAIL*/
         if ((alloc->forMsg3 == TRUE) && (alloc->hqProc->remTx == 0))
         {
            rgNumMsg3FailMaxRetx++;
#ifdef TENB_STATS
            cell->tenbStats->sch.msg3Fail++;
#endif
         }

#ifdef MAC_SCH_STATS
    if(alloc->ue != NULLP)
    {
       /* access from ulHarqProc*/
       RgSchUeCb       *ueCb  = alloc->ue;
       RgSchCmnUe      *cmnUe = (RgSchCmnUe*)ueCb->sch;
       RgSchCmnUlUe    *ulUe  = &(cmnUe->ul);
       U8              cqi    = ulUe->crntUlCqi[0];  
       U16             numUlRetx = ueCb->ul.hqEnt.maxHqRetx - alloc->hqProc->remTx;

       hqRetxStats.ulCqiStat[(cqi - 1)].mcs = alloc->grnt.iMcs;

       switch (numUlRetx)
       {
          case 1:
             hqRetxStats.ulCqiStat[(cqi - 1)].numOfHQ_1++;
             break;
          case 2:
             hqRetxStats.ulCqiStat[(cqi - 1)].numOfHQ_2++;
             break;
         case 3:
            hqRetxStats.ulCqiStat[(cqi - 1)].numOfHQ_3++;
            break;
         case 4:
            hqRetxStats.ulCqiStat[(cqi - 1)].numOfHQ_4++;
            break;
      }
      hqRetxStats.ulCqiStat[(cqi - 1)].totalTx = \
             hqRetxStats.ulCqiStat[(cqi - 1)].numOfHQ_1 + \
            (hqRetxStats.ulCqiStat[(cqi - 1)].numOfHQ_2 * 2) + \
            (hqRetxStats.ulCqiStat[(cqi - 1)].numOfHQ_3 * 3) + \
            (hqRetxStats.ulCqiStat[(cqi - 1)].numOfHQ_4 * 4);
   }

#endif /*MAC_SCH_STATS*/
         rgSCHCmnUlFreeAllocation(cell, sf, alloc);
      }
      /*ccpu00106104 MOD added check for AckNackRep */
      /*added check for acknack so that adaptive retx considers ue
       inactivity due to ack nack repetition*/
      else if((alloc->ue != NULLP) && ((TRUE != alloc->forMsg3) &&
                ((alloc->ue->measGapCb.isMeasuring == TRUE) ||
                        (alloc->ue->ackNakRepCb.isAckNakRep == TRUE))))
      {
        rgSCHCmnUlUpdAllocRetx(newSf, alloc);
        rgSCHUtlUlAllocRls(sf, alloc);
      }
   } while ((alloc = nxtAlloc) != NULLP);

   RETVOID;
}

/**
 * @brief Update an uplink subframe.
 *
 * @details
 *
 *     Function : rgSCHCmnRlsUlSf
 *
 *     For each allocation
 *      - if no more tx needed
 *         - Release allocation
 *      - else
 *         - Perform retransmission
 *
 *  @param[in]  RgSchUlSf *sf
 *  @return  Void
 **/
#ifdef LTE_TDD
#ifdef ANSI
PUBLIC Void rgSCHCmnRlsUlSf
(
RgSchCellCb    *cell,
U8              idx
)
#else
PUBLIC Void rgSCHCmnRlsUlSf(cell, idx)
RgSchCellCb    *cell;
U8              idx;
#endif
#else /* LTE_TDD */
#ifdef ANSI
PUBLIC Void rgSCHCmnRlsUlSf
(
RgSchCellCb    *cell
)
#else
PUBLIC Void rgSCHCmnRlsUlSf(cell)
RgSchCellCb    *cell;
#endif
#endif
{

/* ACC-TDD */
#ifndef LTE_TDD
   U8              idx =0;
#endif
   RgSchCmnUlCell  *cellUl = RG_SCH_CMN_GET_UL_CELL(cell);
/* ACC-TDD */
   if ((cellUl->hqFdbkIdx[idx] != RGSCH_INVALID_INFO) && (cellUl->reTxIdx[idx] != RGSCH_INVALID_INFO))
   {
      RgSchUlSf       *oldSf  = &cellUl->ulSfArr[cellUl->hqFdbkIdx[idx]];
      RgSchUlSf       *newSf = &cellUl->ulSfArr[cellUl->reTxIdx[idx]];

      TRC2(rgSCHCmnRlsUlSf);

      /* Initialize the reTxLst of UL HqProcs for RETX subframe */
      /*cmLListInit(&newSf->reTxLst);*/
      if (rgSCHUtlUlAllocFirst(oldSf) == NULLP)
      {
         RETVOID;
      }
      /* ccpu00107638: Added support for non-adaptive
      * retransmission in Uplink. */
      /* ccpu00107391: Added support for msg3
      * retransmission. */
      /* Release all completed TX allocs from sf */
      rgSCHCmnUlRmvCmpltdAllocs(cell, oldSf, newSf);
      /* Merge newSf allocs to sf allocDb and swap
      * hole/alloc context of sf and newSf */
      /* ACC-TDD */
      rgSCHCmnUlPrfmSfMerge(cell, oldSf, newSf,idx);

      oldSf->aCqiReqCount = 0;
   }
   RETVOID;
}

/**
 * @brief Handle uplink allocation for retransmission.
 *
 * @details
 *
 *     Function : rgSCHCmnUlUpdAllocRetx
 *
 *     - Perform adaptive retransmission
 *
 *  @param[in]  RgSchUlSf *sf
 *  @param[in]  RgSchUlAlloc  *alloc
 *  @return  Void
 **/
#ifdef ANSI
PRIVATE Void rgSCHCmnUlUpdAllocRetx
(
RgSchUlSf      *reTxSf,
RgSchUlAlloc   *alloc
)
#else
PRIVATE Void rgSCHCmnUlUpdAllocRetx(reTxSf, alloc)
RgSchUlSf      *reTxSf;
RgSchUlAlloc   *alloc;
#endif
{
   U8 iTbs;
   TRC2(rgSCHCmnUlUpdAllocRetx);
   alloc->hqProc->reTxAlloc.rnti    =  alloc->rnti;
   alloc->hqProc->reTxAlloc.numSb   =  alloc->numSb;
   alloc->hqProc->reTxAlloc.iMcs   =  alloc->grnt.iMcs;
   iTbs = rgSCHCmnUlGetITbsFrmIMcs(alloc->grnt.iMcs);
   RGSCH_ARRAY_BOUND_CHECK( 0, rgTbSzTbl[0], iTbs);
   alloc->hqProc->reTxAlloc.tbSz    =
      rgTbSzTbl[0][iTbs][alloc->grnt.numRb-1]/8;
   alloc->hqProc->reTxAlloc.ue      = alloc->ue;
   alloc->hqProc->reTxAlloc.forMsg3 = alloc->forMsg3;
   alloc->hqProc->reTxAlloc.raCb = alloc->raCb;

   /* Set as retransmission is pending */
   alloc->hqProc->isRetx = TRUE;
   alloc->hqProc->alloc = NULLP;
   alloc->hqProc->ulSfIdx = RGSCH_INVALID_INFO;

   cmLListAdd2Tail(&reTxSf->reTxLst, &alloc->hqProc->reTxLnk);
   alloc->hqProc->reTxLnk.node = (PTR)alloc->hqProc;
   /* Fix: syed Adaptive Msg3 Retx crash. */
   alloc->hqProc->reTxAlloc.reTxLst = &reTxSf->reTxLst;
   alloc->hqProc->alloc = NULLP;  /* SPS_INTG_FIX */
   RETVOID;
}

/**
 * @brief Attempts allocation for msg3s for which ADAP retransmissions
 *     are required.
 *
 * @details
 *
 *     Function : rgSCHCmnUlAdapRetxAlloc
 *
 *     Attempts allocation for msg3s for which ADAP retransmissions
 *     are required.
 *
 *  @param[in]  RgSchCellCb       *cell
 *  @param[in]  RgSchUlSf         *sf
 *  @param[in]  RgSchUlHqProcCb   *proc;
 *  @param[in]  RgSchUlHole       *hole;
 *  @return  U8
 **/
#ifdef ANSI
PRIVATE Bool rgSCHCmnUlAdapRetxAlloc
(
RgSchCellCb       *cell,
RgSchUlSf         *sf,
RgSchUlHqProcCb   *proc,
RgSchUlHole       *hole
)
#else
PRIVATE Bool rgSCHCmnUlAdapRetxAlloc(cell, sf, proc, hole)
RgSchCellCb       *cell;
RgSchUlSf         *sf;
RgSchUlHqProcCb   *proc;
RgSchUlHole       *hole;
#endif
{
   U8              numSb = proc->reTxAlloc.numSb;
   U8              iMcs  = proc->reTxAlloc.iMcs;
   CmLteTimingInfo frm = cell->crntTime;
   RgSchCmnUlCell  *cellUl = RG_SCH_CMN_GET_UL_CELL(cell);
   RgSchDlSf       *dlSf;
   RgSchPdcch      *pdcch;
   RgSchUlAlloc    *alloc;
   TRC2(rgSCHCmnUlAdapRetxAlloc);

   /* Fetch PDCCH for msg3 */
   /* ccpu00116293 - Correcting relation between UL subframe and DL subframe based on RG_UL_DELTA*/
   /* Introduced timing delta for UL control */
   RGSCH_INCR_SUB_FRAME(frm, TFU_ULCNTRL_DLDELTA);
   dlSf = rgSCHUtlSubFrmGet(cell, frm);
   pdcch = rgSCHCmnCmnPdcchAlloc(cell, dlSf);
   if (pdcch == NULLP)
   {
      RETVALUE(FALSE);
   }

   /* Fetch UL Alloc for msg3 */
   if (numSb <= hole->num)
   {
      alloc                = rgSCHUtlUlAllocGetHole(sf, numSb, hole);
      
      /* KWork fix */
         if(alloc == NULLP)
         {
            rgSCHUtlPdcchPut(cell, &dlSf->pdcchInfo, pdcch);
            RLOG_ARG1(L_ERROR,DBG_CELLID,cell->cellId,
                  "UL Alloc fail for msg3 retx for rnti: %d\n", 
                  proc->reTxAlloc.rnti);
            RETVALUE(FALSE);
         }

      rgSCHCmnUlAllocFillRbInfo(cell, sf, alloc);
      alloc->grnt.iMcs     = iMcs;
      alloc->grnt.datSz    = proc->reTxAlloc.tbSz;
      RG_SCH_UL_MCS_TO_MODODR(iMcs, alloc->grnt.modOdr);
      /* Fill UL Alloc for msg3 */
      /* RACHO : setting nDmrs to 0 and UlDelaybit to 0*/
      alloc->grnt.nDmrs    = 0;
      alloc->grnt.hop      = 0;
      alloc->grnt.delayBit = 0;
      alloc->grnt.isRtx    = TRUE;
      proc->ulSfIdx        = cellUl->schdIdx;
      alloc->rnti          = proc->reTxAlloc.rnti;
      /* Fix : syed HandIn Ue has forMsg3 and ue Set, but no RaCb */
      alloc->ue            = proc->reTxAlloc.ue;
      alloc->pdcch         = pdcch;
      alloc->forMsg3       = proc->reTxAlloc.forMsg3;
      alloc->raCb          = proc->reTxAlloc.raCb;
      alloc->hqProc        = proc;
      alloc->isAdaptive    = TRUE;
      /* FIX : syed HandIn Ue has forMsg3 and ue Set, but no RaCb */
      if (alloc->raCb)
      {
         alloc->raCb->msg3Grnt= alloc->grnt;
#ifndef LTE_TDD
         /* To the crntTime, add the time at which UE will
          * actually send MSG3 */
         alloc->raCb->msg3AllocTime = cell->crntTime;
         RGSCH_INCR_SUB_FRAME(alloc->raCb->msg3AllocTime, RG_SCH_CMN_MIN_RETXMSG3_RECP_INTRVL);
#else
         alloc->raCb->msg3AllocTime =  cellUl->schdTime;
#endif
         rgSCHCmnUlAdapRetx(alloc, proc);
         /* Fill PDCCH with alloc info */
         pdcch->rnti                           = alloc->rnti;
         pdcch->dci.dciFormat                  = TFU_DCI_FORMAT_0;
         pdcch->dci.u.format0Info.hoppingEnbld = alloc->grnt.hop;
         pdcch->dci.u.format0Info.rbStart      = alloc->grnt.rbStart;
         pdcch->dci.u.format0Info.numRb        = alloc->grnt.numRb;
         pdcch->dci.u.format0Info.mcs          = alloc->grnt.iMcsCrnt;
         pdcch->dci.u.format0Info.ndi          = alloc->hqProc->ndi;
         pdcch->dci.u.format0Info.nDmrs        = alloc->grnt.nDmrs;
         pdcch->dci.u.format0Info.tpcCmd       = alloc->grnt.tpc;

#ifdef LTE_TDD
#ifdef TFU_TDD
         /* ulIdx setting for cfg 0 shall be appropriately fixed thru ccpu00109015 */
         pdcch->dci.u.format0Info.ulIdx = RG_SCH_ULIDX_MSB;
         pdcch->dci.u.format0Info.dai = RG_SCH_MAX_DAI_IDX;
#endif
#endif
      }
      else
      {
         RgSchCmnUlUe *ueUl    = RG_SCH_CMN_GET_UL_UE(alloc->ue);
         ueUl->alloc.alloc = alloc;
         /* FIx: Removed the call to rgSCHCmnUlAdapRetx */
         rgSCHCmnUlUeFillAllocInfo(cell, alloc->ue);
      }
      /* Reset as retransmission is done */
      proc->isRetx = FALSE;
   }
   else /* Intg fix */
   {
      rgSCHUtlPdcchPut(cell, &dlSf->pdcchInfo, pdcch);
      RLOG_ARG1(L_DEBUG,DBG_CELLID,cell->cellId,
               "Num SB not suffiecient for adap retx for rnti: %d", 
               proc->reTxAlloc.rnti);
      RETVALUE(FALSE);
   }
   RETVALUE(TRUE);
}

/* Fix: syed Adaptive Msg3 Retx crash. */
/**
 * @brief Releases all Adaptive Retx HqProcs which failed for
 *        allocations in this scheduling occassion.
 *
 * @details
 *
 *     Function : rgSCHCmnUlSfRlsRetxProcs
 *
 *
 *  @param[in]  RgSchCellCb *cell
 *  @param[in]  RgSchUlSf   *sf
 *  @return  U8
 **/
#ifdef ANSI
PRIVATE Void rgSCHCmnUlSfRlsRetxProcs
(
RgSchCellCb *cell,
RgSchUlSf   *sf
)
#else
PRIVATE Void rgSCHCmnUlSfRlsRetxProcs(cell, sf)
RgSchCellCb *cell;
RgSchUlSf   *sf;
#endif
{
   CmLListCp         *cp;
   CmLList           *node;
   RgSchUlHqProcCb   *proc;

   TRC2(rgSCHCmnUlSfRlsRetxProcs);

   cp = &(sf->reTxLst);
   node = cp->first;
   while (node)
   {
      proc  = (RgSchUlHqProcCb *)node->node;
      node = node->next;
      /* ccpu00137834 : Deleting reTxLnk from the respective reTxLst */
      cmLListDelFrm(proc->reTxAlloc.reTxLst, &proc->reTxLnk);
      proc->reTxLnk.node = (PTR)NULLP;
      proc->reTxAlloc.reTxLst = (PTR)NULLP;
   }
   RETVOID;
}
   

/**
 * @brief Attempts allocation for UEs for which retransmissions
 *     are required.
 *
 * @details
 *
 *     Function : rgSCHCmnUlSfReTxAllocs
 *
 *     Attempts allocation for UEs for which retransmissions
 *     are required.
 *
 *  @param[in]  RgSchCellCb *cell
 *  @param[in]  RgSchUlSf   *sf
 *  @return  U8
 **/
#ifdef ANSI
PRIVATE Void rgSCHCmnUlSfReTxAllocs
(
RgSchCellCb *cell,
RgSchUlSf   *sf
)
#else
PRIVATE Void rgSCHCmnUlSfReTxAllocs(cell, sf)
RgSchCellCb *cell;
RgSchUlSf   *sf;
#endif
{
   CmLListCp         *cp;
   CmLList           *node;
   RgSchUlHqProcCb   *proc;
   RgSchUlHole       *hole;
   RgSchUeCb         *ue;
   RgSchCmnCell      *schCmnCell = (RgSchCmnCell *)(cell->sc.sch);
   TRC2(rgSCHCmnUlSfReTxAllocs);

   cp = &(sf->reTxLst);
   node = cp->first;
   while ((node))
   {
      proc  = (RgSchUlHqProcCb *)node->node;
      ue = proc->reTxAlloc.ue;
      node = node->next;
      /*ccpu00106104 MOD added check for AckNackRep */
      /*added check for acknack so that adaptive retx considers ue
       inactivity due to ack nack repetition*/
      if((ue != NULLP) &&
            ((ue->measGapCb.isMeasuring == TRUE)||
               (ue->ackNakRepCb.isAckNakRep == TRUE)))
      {
         continue;
      }
      /* Fix for ccpu00123917: Check if maximum allocs per UL sf have been exhausted */
      if (((hole = rgSCHUtlUlHoleFirst(sf)) == NULLP)
            || (sf->allocDb->count == schCmnCell->ul.maxAllocPerUlSf))
      {
         /* No more UL BW then return */
         break;
      }
      /* perform adaptive retx for UE's */
      if (rgSCHCmnUlAdapRetxAlloc(cell, sf, proc, hole) == FALSE)
      {
         continue;
      }
      /* ccpu00137834 : Deleting reTxLnk from the respective reTxLst */
      cmLListDelFrm(proc->reTxAlloc.reTxLst, &proc->reTxLnk);
      /* Fix: syed Adaptive Msg3 Retx crash. */
      proc->reTxLnk.node = (PTR)NULLP;
      proc->reTxAlloc.reTxLst = (PTR)NULLP;
   }
   RETVOID;
}

/**
 * @brief Handles RB allocation for downlink.
 *
 * @details
 *
 *     Function : rgSCHCmnDlRbAlloc
 *
 *     Invoking Module Processing:
 *     - This function is invoked for DL RB allocation
 *
 *     Processing Steps:
 *     - If cell is frequency selecive,
 *       - Call rgSCHDlfsAllocRb().
 *     - else,
 *       - Call rgSCHCmnNonDlfsRbAlloc().
 *
 *  @param[in]  RgSchCellCb        *cell
 *  @param[in]  RgSchDlRbAllocInfo *allocInfo
 *  @return  Void
 **/

#ifdef ANSI
PRIVATE Void rgSCHCmnDlRbAlloc
(
RgSchCellCb           *cell,
RgSchCmnDlRbAllocInfo *allocInfo
)
#else
PRIVATE Void rgSCHCmnDlRbAlloc(cell, allocInfo)
RgSchCellCb           *cell;
RgSchCmnDlRbAllocInfo *allocInfo;
#endif
{
   RgSchCmnCell      *cellSch = RG_SCH_CMN_GET_CELL(cell);
   TRC2(rgSCHCmnDlRbAlloc);

   if (cellSch->dl.isDlFreqSel)
   {
      cellSch->apisDlfs->rgSCHDlfsAllocRb(cell, allocInfo);
   }
   else
   {
      rgSCHCmnNonDlfsRbAlloc(cell, allocInfo);
   }

   /* MUE_PERTTI_DL */
/*
    if (allocInfo->dedAlloc.retxUeLst.count +
    allocInfo->dedAlloc.txRetxUeLst.count +
    allocInfo->dedAlloc.txUeLst.count >= cellSch->dl.maxUeNewTxPerTti)
    {
   }
   */
   RETVOID;
}

#ifdef LTEMAC_SPS

/**
 * @brief Determines number of RBGs and RBG subset sizes for the given DL
 * bandwidth and rbgSize
 *
 * @details
 *     Function : rgSCHCmnDlGetRbgInfo
 *
 *
 *     Processing Steps:
 *     - Fill-up rbgInfo data structure for given DL bandwidth and rbgSize
 *
 *   @param[in]  U8             dlTotalBw
 *   @param[in]  U8             dlSubsetBw
 *   @param[in]  U8             maxRaType1SubsetBw
 *   @param[in]  U8             rbgSize
 *   @param[out] RgSchBwRbgInfo *rbgInfo
 *  @return Void
 **/
#ifdef ANSI
PUBLIC Void rgSCHCmnDlGetRbgInfo
(
U8             dlTotalBw,
U8             dlSubsetBw,
U8             maxRaType1SubsetBw,
U8             rbgSize,
RgSchBwRbgInfo *rbgInfo
)
#else
PUBLIC Void rgSCHCmnDlGetRbgInfo(dlTotalBw, dlSubsetBw, maxRaType1SubsetBw,
      rbgSize, rbgInfo)
U8             dlTotalBw;
U8             dlSubsetBw;
U8             maxRaType1SubsetBw;
U8             rbgSize;
RgSchBwRbgInfo *rbgInfo;
#endif
{
#ifdef RGSCH_SPS_UNUSED
   U8    idx           = 0;
   U8    lastRbgIdx    = ((dlTotalBw + rbgSize - 1)/rbgSize) - 1;
   U8    currRbgSize   = rbgSize;
   U8    subsetSizeIdx = 0;
   U8    subsetSize[RG_SCH_NUM_RATYPE1_SUBSETS] = {0};
   U8    lastRbgSize = rbgSize - (dlTotalBw - ((dlTotalBw/rbgSize) * rbgSize));
   U8    numRaType1Rbgs = (maxRaType1SubsetBw + rbgSize - 1)/rbgSize;
#endif

   /* Compute maximum number of SPS RBGs for the cell */
   rbgInfo->numRbgs =  ((dlSubsetBw + rbgSize - 1)/rbgSize);

#ifdef RGSCH_SPS_UNUSED
   /* Distribute RBGs across subsets except last RBG */
   for (;idx < numRaType1Rbgs - 1; ++idx)
   {
      subsetSize[subsetSizeIdx] += currRbgSize;
      subsetSizeIdx = (subsetSizeIdx + 1) % rbgSize;
   }

   /* Computation for last RBG */
   if (idx == lastRbgIdx)
   {
      currRbgSize = lastRbgSize;
   }
   subsetSize[subsetSizeIdx] += currRbgSize;
   subsetSizeIdx = (subsetSizeIdx + 1) % rbgSize;
#endif

   /* Update the computed sizes */
#ifdef RGSCH_SPS_UNUSED
   rbgInfo->lastRbgSize = currRbgSize;
#endif
   rbgInfo->lastRbgSize = rbgSize - 
            (dlSubsetBw - ((dlSubsetBw/rbgSize) * rbgSize));
#ifdef RGSCH_SPS_UNUSED
   cmMemcpy((U8 *)rbgInfo->rbgSubsetSize, (U8 *) subsetSize, 4 * sizeof(U8));
#endif
   rbgInfo->numRbs = (rbgInfo->numRbgs * rbgSize > dlTotalBw) ?
      dlTotalBw:(rbgInfo->numRbgs * rbgSize);
   rbgInfo->rbgSize = rbgSize;
}

/**
 * @brief Handles RB allocation for Resource allocation type 0
 *
 * @details
 *
 *     Function : rgSCHCmnDlRaType0Alloc
 *
 *     Invoking Module Processing:
 *     - This function is invoked for DL RB allocation for resource allocation
 *     type 0
 *
 *     Processing Steps:
 *     - Determine the available positions in the rbgMask.
 *     - Allocate RBGs in the available positions.
 *     - Update RA Type 0, RA Type 1 and RA type 2 masks.
 *
 *  @param[in]   RgSchDlSfAllocInfo *allocedInfo
 *  @param[in]   U8             rbsReq
 *  @param[in]   RgSchBwRbgInfo *rbgInfo
 *  @param[out]  U8             *numAllocRbs
 *  @param[out]  RgSchDlSfAllocInfo *resAllocInfo
 *  @param[in]   Bool           isPartialAlloc
 *
 *  @return  Void
 **/

#ifdef ANSI
PUBLIC U8 rgSCHCmnDlRaType0Alloc
(
RgSchDlSfAllocInfo *allocedInfo,
U8                 rbsReq,
RgSchBwRbgInfo     *rbgInfo,
U8                 *numAllocRbs,
RgSchDlSfAllocInfo *resAllocInfo,
Bool               isPartialAlloc
)
#else
PUBLIC U8 rgSCHCmnDlRaType0Alloc(allocedInfo, rbsReq, rbgInfo,
      numAllocRbs, resAllocInfo, isPartialAlloc)
RgSchDlSfAllocInfo *allocedInfo;
U8                 rbsReq;
RgSchBwRbgInfo     *rbgInfo;
U8                 *numAllocRbs;
RgSchDlSfAllocInfo *resAllocInfo;
Bool               isPartialAlloc;
#endif
{
   /* Note: This function atttempts allocation only full allocation */
   U32      remNumRbs, rbgPosInRbgMask, ueRaType2Mask;
   U8       type2MaskIdx, cnt, rbIdx;
   U8       maskSize, rbg;
   U8       bestNumAvailRbs = 0;
   U8       usedRbs = 0;
   U8       numAllocRbgs = 0;
   U8       rbgSize = rbgInfo->rbgSize;
   U32      *rbgMask = &(resAllocInfo->raType0Mask);
#ifdef RGSCH_SPS_UNUSED
   U8       rbgSubset;
   U32      ueRaType1Mask;
   U32      *raType1Mask = resAllocInfo->raType1Mask;
   U32      *raType1UsedRbs = resAllocInfo->raType1UsedRbs;
#endif
   U32      *raType2Mask = resAllocInfo->raType2Mask;

   U32      allocedMask = allocedInfo->raType0Mask;

   maskSize = rbgInfo->numRbgs;

   *numAllocRbs = 0;
   RG_SCH_CMN_DL_COUNT_ONES(allocedMask, maskSize, &usedRbs);
   if (maskSize == usedRbs)
   {
      /* All RBGs are allocated, including the last one */
      remNumRbs = 0;
   }
   else
   {
      remNumRbs = (maskSize - usedRbs - 1) * rbgSize; /* vamsee: removed minus 1 */

      /* If last RBG is available, add last RBG size */
      if (!(allocedMask & (1 << RG_SCH_CMN_DL_GET_POS_FRM_LSB(maskSize - 1))))
      {
         remNumRbs += rbgInfo->lastRbgSize;
      }
   }

   /* If complete allocation is needed, check if total requested RBs are available else
    * check the best available RBs */
   if (!isPartialAlloc)
   {
      if (remNumRbs >= rbsReq)
      {
         bestNumAvailRbs = rbsReq;
      }
   }
   else
   {
      bestNumAvailRbs = remNumRbs > rbsReq ? rbsReq : remNumRbs;
   }

   /* Allocate for bestNumAvailRbs */
   if (bestNumAvailRbs)
   {
      for (rbg = 0; rbg < maskSize - 1; ++rbg)
      {
         rbgPosInRbgMask = 1 << RG_SCH_CMN_DL_GET_POS_FRM_LSB(rbg);
         if (!(allocedMask & rbgPosInRbgMask))
         {
            /* Update RBG mask */
            *rbgMask |= rbgPosInRbgMask;

            /* Compute RB index of the first RB of the RBG allocated */
            rbIdx = rbg * rbgSize;

            for (cnt = 0; cnt < rbgSize; ++cnt)
            {
#ifdef RGSCH_SPS_UNUSED
               ueRaType1Mask = rgSCHCmnGetRaType1Mask(rbIdx, rbgSize, &rbgSubset);
#endif
               ueRaType2Mask = rgSCHCmnGetRaType2Mask(rbIdx, &type2MaskIdx);
#ifdef RGSCH_SPS_UNUSED
               /* Update RBG mask for RA type 1 */
               raType1Mask[rbgSubset] |= ueRaType1Mask;
               raType1UsedRbs[rbgSubset]++;
#endif
               /* Update RA type 2 mask */
               raType2Mask[type2MaskIdx] |= ueRaType2Mask;
               rbIdx++;
            }
            *numAllocRbs += rbgSize;
            remNumRbs -= rbgSize;
            ++numAllocRbgs;
            if (*numAllocRbs >= bestNumAvailRbs)
            {
               break;
            }
         }
      }
      /* If last RBG available and allocation is not completed, allocate
       * last RBG */
      if (*numAllocRbs < bestNumAvailRbs)
      {
         rbgPosInRbgMask =  1 << RG_SCH_CMN_DL_GET_POS_FRM_LSB(rbg);
         *rbgMask |= rbgPosInRbgMask;
         *numAllocRbs += rbgInfo->lastRbgSize;

         /* Compute RB index of the first RB of the last RBG */
         rbIdx = ((rbgInfo->numRbgs - 1 ) * rbgSize ); /* removed minus 1  vamsee */

         for (cnt = 0; cnt < rbgInfo->lastRbgSize; ++cnt)
         {
#ifdef RGSCH_SPS_UNUSED
            ueRaType1Mask = rgSCHCmnGetRaType1Mask(rbIdx, rbgSize, &rbgSubset);
#endif
            ueRaType2Mask = rgSCHCmnGetRaType2Mask(rbIdx, &type2MaskIdx);
#ifdef RGSCH_SPS_UNUSED
            /* Update RBG mask for RA type 1 */
            raType1Mask[rbgSubset] |=  ueRaType1Mask;
            raType1UsedRbs[rbgSubset]++;
#endif
            /* Update RA type 2 mask */
            raType2Mask[type2MaskIdx] |= ueRaType2Mask;
            rbIdx++;
         }
         remNumRbs -= rbgInfo->lastRbgSize;
         ++numAllocRbgs;
      }
      /* Note: this should complete allocation, not checking for the
       * same */
   }

   RETVALUE(numAllocRbgs);
}

#ifdef RGSCH_SPS_UNUSED
/**
 * @brief Handles RB allocation for Resource allocation type 1
 *
 * @details
 *
 *     Function : rgSCHCmnDlRaType1Alloc
 *
 *     Invoking Module Processing:
 *     - This function is invoked for DL RB allocation for resource allocation
 *     type 1
 *
 *     Processing Steps:
 *     - Determine the available positions in the subsets.
 *     - Allocate RB in the available subset.
 *     - Update RA Type1, RA type 0 and RA type 2 masks.
 *
 *  @param[in]   RgSchDlSfAllocInfo *allocedInfo
 *  @param[in]   U8                 rbsReq
 *  @param[in]   RgSchBwRbgInfo     *rbgInfo
 *  @param[in]   U8                 startRbgSubset
 *  @param[in]   U8                 *allocRbgSubset
 *  @param[out]  rgSchDlSfAllocInfo *resAllocInfo
 *  @param[in]   Bool               isPartialAlloc
 *
 *  @return  U8
 *  Number of allocated RBs
 **/

#ifdef ANSI
PUBLIC U8 rgSCHCmnDlRaType1Alloc
(
RgSchDlSfAllocInfo *allocedInfo,
U8                 rbsReq,
RgSchBwRbgInfo     *rbgInfo,
U8                 startRbgSubset,
U8                 *allocRbgSubset,
RgSchDlSfAllocInfo *resAllocInfo,
Bool               isPartialAlloc
)
#else
PUBLIC U8 rgSCHCmnDlRaType1Alloc(allocedInfo, rbsReq,rbgInfo,startRbgSubset,
      allocRbgSubset, resAllocInfo, isPartialAlloc)
RgSchDlSfAllocInfo *allocedInfo;
U8                 rbsReq;
RgSchBwRbgInfo     *rbgInfo;
U8                 startRbgSubset;
U8                 *allocRbgSubset;
RgSchDlSfAllocInfo *resAllocInfo;
Bool               isPartialAlloc;
#endif
{
   /* Note: This function atttempts only full allocation */
   U8          *rbgSubsetSzArr;
   U8          type2MaskIdx, subsetIdx, rbIdx, rbInSubset, rbgInSubset;
   U8          offset, rbg, maskSize, bestSubsetIdx;
   U8          startPos = 0;
   U8          bestNumAvailRbs = 0;
   U8          numAllocRbs = 0;
   U32         ueRaType2Mask, ueRaType0Mask, rbPosInSubset;
   U32         remNumRbs, allocedMask;
   U8          usedRbs = 0;
   U8          rbgSize = rbgInfo->rbgSize;
   U8          rbgSubset = startRbgSubset;
   U32         *rbgMask = &resAllocInfo->raType0Mask;
   U32         *raType1Mask = resAllocInfo->raType1Mask;
   U32         *raType2Mask = resAllocInfo->raType2Mask;
   U32         *raType1UsedRbs = resAllocInfo->raType1UsedRbs;
   U32         *allocMask = allocedInfo->raType1Mask;

   /* Initialize the subset size Array */
   rbgSubsetSzArr = rbgInfo->rbgSubsetSize;

   /* Perform allocation for RA type 1 */
   for (subsetIdx = 0;subsetIdx < rbgSize; ++subsetIdx)
   {
      allocedMask = allocMask[rbgSubset];
      maskSize = rbgSubsetSzArr[rbgSubset];

      /* Determine number of available RBs in the subset */
      usedRbs = allocedInfo->raType1UsedRbs[subsetIdx];
      remNumRbs = maskSize - usedRbs;

      if (remNumRbs >= rbsReq)
      {
         bestNumAvailRbs = rbsReq;
         bestSubsetIdx = rbgSubset;
         break;
      }
      else if (isPartialAlloc && (remNumRbs > bestNumAvailRbs))
      {
         bestNumAvailRbs = remNumRbs;
         bestSubsetIdx = rbgSubset;
      }

      rbgSubset = (rbgSubset + 1) % rbgSize;
   } /* End of for (each rbgsubset) */

   if (bestNumAvailRbs)
   {
      /* Initialize alloced mask and subsetSize depending on the RBG
       * subset of allocation */
      U8        startIdx = 0;
      maskSize = rbgSubsetSzArr[bestSubsetIdx];
      allocedMask = allocMask[bestSubsetIdx];
      RG_SCH_CMN_DL_GET_START_POS(allocedMask, maskSize,
            &startPos);
      for (; startIdx < rbgSize; ++startIdx, ++startPos)
      {
         for (rbInSubset = startPos; rbInSubset < maskSize;
               rbInSubset = rbInSubset + rbgSize)
         {
            rbPosInSubset =  1 << RG_SCH_CMN_DL_GET_POS_FRM_LSB(rbInSubset);
            if (!(allocedMask & rbPosInSubset))
            {
               raType1Mask[bestSubsetIdx] |= rbPosInSubset;
               raType1UsedRbs[bestSubsetIdx]++;

               /* Compute RB index value for the RB being allocated */
               rbgInSubset = rbInSubset /rbgSize;
               offset = rbInSubset % rbgSize;
               rbg = (rbgInSubset * rbgSize) + bestSubsetIdx;
               rbIdx = (rbg * rbgSize) + offset;

               /* Update RBG mask for RA type 0 allocation */
               ueRaType0Mask = rgSCHCmnGetRaType0Mask(rbIdx, rbgSize);
               *rbgMask |= ueRaType0Mask;

               /* Update RA type 2 mask */
               ueRaType2Mask = rgSCHCmnGetRaType2Mask(rbIdx, &type2MaskIdx);
               raType2Mask[type2MaskIdx] |= ueRaType2Mask;

               /* Update the counters */
               numAllocRbs++;
               remNumRbs--;
               if (numAllocRbs == bestNumAvailRbs)
               {
                  break;
               }
            }
         } /* End of for (each position in the subset mask) */
         if (numAllocRbs == bestNumAvailRbs)
         {
            break;
         }
      } /* End of for startIdx = 0 to rbgSize */

      *allocRbgSubset = bestSubsetIdx;
   } /* End of if (bestNumAvailRbs) */

   RETVALUE(numAllocRbs);
}
#endif
/**
 * @brief Handles RB allocation for Resource allocation type 2
 *
 * @details
 *
 *     Function : rgSCHCmnDlRaType2Alloc
 *
 *     Invoking Module Processing:
 *     - This function is invoked for DL RB allocation for resource allocation
 *     type 2
 *
 *     Processing Steps:
 *     - Determine the available positions in the mask
 *     - Allocate best fit cosecutive RBs.
 *     - Update RA Type2, RA type 1 and RA type 0 masks.
 *
 *  @param[in]   RgSchDlSfAllocInfo *allocedInfo
 *  @param[in]   U8             rbsReq
 *  @param[in]   RgSchBwRbgInfo *rbgInfo
 *  @param[out]  U8             *rbStart
 *  @param[out]  rgSchDlSfAllocInfo *resAllocInfo
 *  @param[in]   Bool           isPartialAlloc
 *
 *  @return  U8
 *  Number of allocated RBs
 **/

#ifdef ANSI
PUBLIC U8 rgSCHCmnDlRaType2Alloc
(
RgSchDlSfAllocInfo *allocedInfo,
U8                 rbsReq,
RgSchBwRbgInfo     *rbgInfo,
U8                 *rbStart,
RgSchDlSfAllocInfo *resAllocInfo,
Bool               isPartialAlloc
)
#else
PUBLIC U8 rgSCHCmnDlRaType2Alloc(allocedInfo, rbsReq, rbgInfo, rbStart,
      resAllocInfo, isPartialAlloc)
RgSchDlSfAllocInfo *allocedInfo;
U8                 rbsReq;
RgSchBwRbgInfo     *rbgInfo;
U8                 *rbStart;
RgSchDlSfAllocInfo *resAllocInfo;
Bool               isPartialAlloc;
#endif
{
   U8          numAllocRbs = 0;
   U8          rbIdx;
   U8          rbgSize = rbgInfo->rbgSize;
   U32         *rbgMask = &resAllocInfo->raType0Mask;
#ifdef RGSCH_SPS_UNUSED
   U32         *raType1Mask = resAllocInfo->raType1Mask;
#endif
   U32         *raType2Mask = resAllocInfo->raType2Mask;
#ifdef RGSCH_SPS_UNUSED
   U32         *raType1UsedRbs = resAllocInfo->raType1UsedRbs;
#endif
   U32         *allocedMask = allocedInfo->raType2Mask;

   /* Note: This function atttempts only full allocation */
   rgSCHCmnDlGetBestFitHole(allocedMask, rbgInfo->numRbs,
         raType2Mask, rbsReq, rbStart, &numAllocRbs, isPartialAlloc);
   if (numAllocRbs)
   {
      /* Update the allocation in RA type 0 and RA type 1 masks */
      U8 rbCnt = numAllocRbs;
#ifdef RGSCH_SPS_UNUSED
      U8 rbgSubset;
      U32 ueRaType1Mask;
#endif
      U32 ueRaType0Mask;
      rbIdx = *rbStart;

      while(rbCnt)
      {
         /* Update RBG mask for RA type 0 allocation */
         ueRaType0Mask = rgSCHCmnGetRaType0Mask(rbIdx, rbgSize);
         *rbgMask |= ueRaType0Mask;

#ifdef RGSCH_SPS_UNUSED
         /* Update RBG mask for RA type 1 */
         ueRaType1Mask = rgSCHCmnGetRaType1Mask(rbIdx, rbgSize, &rbgSubset);
         raType1Mask[rbgSubset] |= ueRaType1Mask;
         raType1UsedRbs[rbgSubset]++;
#endif
         /* Update the counters */
         --rbCnt;
         rbIdx++;
      }
   }

   RETVALUE(numAllocRbs);
}

/**
 * @brief Determines RA type 0 mask from given RB index.
 *
 * @details
 *
 *     Function : rgSCHCmnGetRaType0Mask
 *
 *
 *     Processing Steps:
 *     - Determine RA Type 0 mask for given rbIdex and rbg size.
 *
 *  @param[in]  U8          rbIdx
 *  @param[in]  U8          rbgSize
 *  @return  U32 RA type 0 mask
 **/
#ifdef ANSI
PRIVATE U32 rgSCHCmnGetRaType0Mask
(
U8                rbIdx,
U8                rbgSize
)
#else
PRIVATE U32 rgSCHCmnGetRaType0Mask(rbIdx, rbgSize)
U8                rbIdx;
U8                rbgSize;
#endif
{
   U8 rbg;
   U32 rbgPosInRbgMask = 0;

   rbg = rbIdx/rbgSize;
   rbgPosInRbgMask = 1 << RG_SCH_CMN_DL_GET_POS_FRM_LSB(rbg);

   RETVALUE(rbgPosInRbgMask);
}

#ifdef RGSCH_SPS_UNUSED
/**
 * @brief Determines RA type 1 mask from given RB index.
 *
 * @details
 *
 *     Function : rgSCHCmnGetRaType1Mask
 *
 *
 *     Processing Steps:
 *     - Determine RA Type 1 mask for given rbIdex and rbg size.
 *
 *  @param[in]  U8          rbIdx
 *  @param[in]  U8          rbgSize
 *  @param[out] U8          *type1Subset
 *  @return  U32 RA type 1 mask
 **/
#ifdef ANSI
PRIVATE U32 rgSCHCmnGetRaType1Mask
(
U8                rbIdx,
U8                rbgSize,
U8                *type1Subset
)
#else
PRIVATE U32 rgSCHCmnGetRaType1Mask(rbIdx, rbgSize, type1Subset)
U8                rbIdx;
U8                rbgSize;
U8                *type1Subset;
#endif
{
   U8 rbg, rbgSubset, rbgInSubset, offset, rbInSubset;
   U32 rbPosInSubset;

   rbg = rbIdx/rbgSize;
   rbgSubset = rbg % rbgSize;
   rbgInSubset = rbg/rbgSize;
   offset = rbIdx % rbgSize;
   rbInSubset = rbgInSubset * rbgSize + offset;
   rbPosInSubset =  1 << RG_SCH_CMN_DL_GET_POS_FRM_LSB(rbInSubset);

   *type1Subset = rbgSubset;
   RETVALUE(rbPosInSubset);
} 
#endif /* RGSCH_SPS_UNUSED */
/**
 * @brief Determines RA type 2 mask from given RB index.
 *
 * @details
 *
 *     Function : rgSCHCmnGetRaType2Mask
 *
 *
 *     Processing Steps:
 *     - Determine RA Type 2 mask for given rbIdx and rbg size.
 *
 *  @param[in]  U8          rbIdx
 *  @param[out] U8          *maskIdx
 *  @return  U32 RA type 2 mask
 **/
#ifdef ANSI
PRIVATE U32 rgSCHCmnGetRaType2Mask
(
U8                rbIdx,
U8                *maskIdx
)
#else
PRIVATE U32 rgSCHCmnGetRaType2Mask(rbIdx, maskIdx)
U8                rbIdx;
U8                *maskIdx;
#endif
{
   U32 rbPosInType2;

   *maskIdx = rbIdx / 32;
   rbPosInType2 =  1 << RG_SCH_CMN_DL_GET_POS_FRM_LSB(rbIdx % 32);

   RETVALUE(rbPosInType2);
}

/**
 * @brief Performs resource allocation for a non-SPS UE in SPS bandwidth
 *
 * @details
 *
 *     Function : rgSCHCmnAllocUeInSpsBw
 *
 *
 *     Processing Steps:
 *       - Determine allocation for the UE.
 *       - Use resource allocation type 0, 1 and 2 for allocation
 *         within maximum SPS bandwidth.
 *
 *  @param[in]  RgSchDlSf       *dlSf
 *  @param[in]  RgSchCellCb     *cell
 *  @param[in]  RgSchUeCb       *ue
 *  @param[in]  RgSchDlRbAlloc  *rbAllocInfo
 *  @param[in]  Bool            isPartialAlloc
 *  @return  Bool
 *             ROK      success
 *             RFAILED  failed
 **/
#ifdef ANSI
PUBLIC Bool rgSCHCmnAllocUeInSpsBw
(
RgSchDlSf           *dlSf,
RgSchCellCb         *cell,
RgSchUeCb           *ue,
RgSchDlRbAlloc      *rbAllocInfo,
Bool                isPartialAlloc
)
#else
PUBLIC Bool rgSCHCmnAllocUeInSpsBw(dlSf, cell, ue, rbAllocInfo, isPartialAlloc)
RgSchDlSf           *dlSf;
RgSchCellCb         *cell;
RgSchUeCb           *ue;
RgSchDlRbAlloc      *rbAllocInfo;
Bool                isPartialAlloc;
#endif
{
   U8                  rbgSize = cell->rbgSize;
   U8                  numAllocRbs = 0;
   U8                  numAllocRbgs = 0;
   U8                  rbStart = 0;
   U8                  idx, noLyr, iTbs;
   RgSchCmnDlUe        *dlUe = RG_SCH_CMN_GET_DL_UE(ue);
   RgSchDlSfAllocInfo  *dlSfAlloc = &rbAllocInfo->dlSf->dlSfAllocInfo;
   RgSchBwRbgInfo      *spsRbgInfo = &cell->spsBwRbgInfo;
   U32    y;


   /* SPS_FIX : Check if this Hq proc is scheduled */
   if ((0 == rbAllocInfo->tbInfo[0].schdlngForTb) &&
         (0 == rbAllocInfo->tbInfo[1].schdlngForTb))
   {
      RETVALUE(TRUE);
   }

   /* Check if the requirement can be accomodated in SPS BW */
   if (dlSf->spsAllocdBw == spsRbgInfo->numRbs)
   {
      /* SPS Bandwidth has been exhausted: no further allocations possible */
      RETVALUE(FALSE);
   }
   if (!isPartialAlloc)
   {
      if((dlSf->spsAllocdBw + rbAllocInfo->rbsReq) > spsRbgInfo->numRbs)
      {
         RETVALUE(TRUE);
      }
   }

   /* Perform allocation for RA type 0 if rbsReq is multiple of RBG size (also
    * if RBG size = 1) */
   if (rbAllocInfo->raType == RG_SCH_CMN_RA_TYPE0)
   {
      rbAllocInfo->rbsReq += (rbgSize - rbAllocInfo->rbsReq % rbgSize);
      numAllocRbgs = rgSCHCmnDlRaType0Alloc(dlSfAlloc,
            rbAllocInfo->rbsReq, spsRbgInfo, &numAllocRbs,
            &rbAllocInfo->resAllocInfo, isPartialAlloc);
   }
#ifdef RGSCH_SPS_UNUSED
   else if (rbAllocInfo->raType == RG_SCH_CMN_RA_TYPE1)
   {
      /* If no RBS could be allocated, attempt RA TYPE 1 */

      numAllocRbs = rgSCHCmnDlRaType1Alloc(dlSfAlloc,
            rbAllocInfo->rbsReq, spsRbgInfo, (U8)dlSfAlloc->nxtRbgSubset,
            &rbAllocInfo->allocInfo.raType1.rbgSubset,
            &rbAllocInfo->resAllocInfo, isPartialAlloc);

      if(numAllocRbs)
      {
         dlSfAlloc->nxtRbgSubset =
            (rbAllocInfo->allocInfo.raType1.rbgSubset + 1 ) % rbgSize;
      }
   }
#endif
   else if (rbAllocInfo->raType == RG_SCH_CMN_RA_TYPE2)
   {
      numAllocRbs = rgSCHCmnDlRaType2Alloc(dlSfAlloc,
            rbAllocInfo->rbsReq, spsRbgInfo,
            &rbStart, &rbAllocInfo->resAllocInfo, isPartialAlloc);
   }
   if (!numAllocRbs)
   {
      RETVALUE(TRUE);
   }

   y = rgSCHUtlGetY(ue, dlSf->sfNum);
   if (!(rbAllocInfo->pdcch =
            rgSCHCmnPdcchAlloc(cell, dlSf, y, dlUe->mimoInfo.cwInfo[0].cqi,\
               rbAllocInfo->dciFormat, FALSE)))
   {
      /* Note: Returning TRUE since PDCCH might be available for another UE */
      RETVALUE(TRUE);
   }

   /* Update Tb info for each scheduled TB */
   iTbs = rbAllocInfo->tbInfo[0].iTbs;
   noLyr = rbAllocInfo->tbInfo[0].noLyr;
   rbAllocInfo->tbInfo[0].bytesAlloc =
      rgTbSzTbl[noLyr - 1][iTbs][numAllocRbs - 1]/8;

   if (rbAllocInfo->tbInfo[1].schdlngForTb)
   {
      iTbs = rbAllocInfo->tbInfo[1].iTbs;
      noLyr = rbAllocInfo->tbInfo[1].noLyr;
      rbAllocInfo->tbInfo[1].bytesAlloc =
         rgTbSzTbl[noLyr - 1][iTbs][numAllocRbs - 1]/8;;
   }

   /* Update rbAllocInfo with the allocation information */
   if (rbAllocInfo->raType == RG_SCH_CMN_RA_TYPE0)
   {
      rbAllocInfo->allocInfo.raType0.dlAllocBitMask =
         rbAllocInfo->resAllocInfo.raType0Mask;
      rbAllocInfo->allocInfo.raType0.numDlAlloc = numAllocRbgs;
   }
#ifdef RGSCH_SPS_UNUSED
   else if (rbAllocInfo->raType == RG_SCH_CMN_RA_TYPE1)
   {
      rbAllocInfo->allocInfo.raType1.dlAllocBitMask =
         rbAllocInfo->resAllocInfo.raType1Mask[rbAllocInfo->allocInfo.raType1.rbgSubset];
      rbAllocInfo->allocInfo.raType1.numDlAlloc = numAllocRbs;
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
   rbAllocInfo->tbInfo[0].schdlngForTb = TRUE;

   /* Update allocation masks for RA types 0, 1 and 2 in DL SF */

   /* Update type 0 allocation mask */
   dlSfAlloc->raType0Mask |= rbAllocInfo->resAllocInfo.raType0Mask;
#ifdef RGSCH_SPS_UNUSED
   /* Update type 1 allocation masks */
   for (idx = 0; idx < RG_SCH_NUM_RATYPE1_32BIT_MASK; ++idx)
   {
      dlSfAlloc->raType1Mask[idx] |= rbAllocInfo->resAllocInfo.raType1Mask[idx];
      dlSfAlloc->raType1UsedRbs[idx] +=
         rbAllocInfo->resAllocInfo.raType1UsedRbs[idx];
   }
#endif
   /* Update type 2 allocation masks */
   for (idx = 0; idx < RG_SCH_NUM_RATYPE2_32BIT_MASK; ++idx)
   {
      dlSfAlloc->raType2Mask[idx] |= rbAllocInfo->resAllocInfo.raType2Mask[idx];
   }

   dlSf->spsAllocdBw += numAllocRbs;
   RETVALUE(TRUE);
}

/***********************************************************
 *
 *     Func : rgSCHCmnDlGetBestFitHole
 *
 *
 *     Desc : Converts the best fit hole into allocation and returns the
 *     allocation information.
 *
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
PRIVATE Void rgSCHCmnDlGetBestFitHole
(
U32         *allocMask,
U8          numMaskRbs,
U32         *crntAllocMask,
U8          rbsReq,
U8          *allocStart,
U8          *allocNumRbs,
Bool        isPartialAlloc
)
#else
PRIVATE  Void rgSCHCmnDlGetBestFitHole (allocMask, numMaskRbs,
        crntAllocMask, rbsReq, allocStart, allocNumRbs, isPartialAlloc)
U32         *allocMask;
U8          numMaskRbs;
U32         *crntAllocMask;
U8          rbsReq;
U8          *allocStart;
U8          *allocNumRbs;
Bool        isPartialAlloc;
#endif
{
   U8 maskSz = (numMaskRbs + 31)/32;
   U8 maxMaskPos = (numMaskRbs % 32);
   U8 maskIdx, maskPos;
   U8 numAvailRbs = 0;
   U8 bestAvailNumRbs = 0;
   S8 bestStartPos = -1;
   S8 startPos = -1;
   U32 tmpMask[RG_SCH_NUM_RATYPE2_32BIT_MASK] = {0};
   U32 bestMask[RG_SCH_NUM_RATYPE2_32BIT_MASK] = {0};

   *allocNumRbs = numAvailRbs;
   *allocStart = 0;

   for (maskIdx = 0; maskIdx < maskSz; ++maskIdx)
   {
      maxMaskPos = 31;
      if (maskIdx == (maskSz - 1))
      {
         if (numMaskRbs % 32)
         {
            maxMaskPos = numMaskRbs % 32;
         }
      }
      for (maskPos = 0; maskPos < maxMaskPos; ++maskPos)
      {
         if (!(allocMask[maskIdx] & (1 << (31 - maskPos))))
         {
            tmpMask[maskIdx] |= (1 << (31 - maskPos));
            if (startPos == -1)
            {
               startPos = maskIdx * 32 + maskPos;
            }
            ++numAvailRbs;
            if (numAvailRbs == rbsReq)
            {
               *allocStart = (U8)startPos;
               *allocNumRbs = rbsReq;
               break;
            }
         }
         else
         {
            if (numAvailRbs > bestAvailNumRbs)
            {
               bestAvailNumRbs = numAvailRbs;
               bestStartPos = startPos;
               cmMemcpy((U8 *)bestMask, (U8 *) tmpMask, 4 * sizeof(U32));
            }
            numAvailRbs = 0;
            startPos = -1;
            cmMemset((U8 *)tmpMask, 0, 4 * sizeof(U32));
         }
      }
      if (*allocNumRbs == rbsReq)
      {
         break;
      }
   }

   if (*allocNumRbs == rbsReq)
   {
      /* Convert the hole into allocation */
      cmMemcpy((U8 *)crntAllocMask, (U8 *) tmpMask, 4 * sizeof(U32));
      RETVOID;
   }
   else
   {
      if (bestAvailNumRbs && isPartialAlloc)
      {
         /* Partial allocation could have been done */
         *allocStart = (U8)bestStartPos;
         *allocNumRbs = bestAvailNumRbs;
         /* Convert the hole into allocation */
         cmMemcpy((U8 *)crntAllocMask, (U8 *) bestMask, 4 * sizeof(U32));
      }
   }

   RETVOID;
}
#endif /* LTEMAC_SPS */

/***************************************************************************
 *
 * NON-DLFS Allocation functions
 *
 * *************************************************************************/
#ifndef LTE_TDD
#ifdef DEBUGP
/**
 * @brief Function to find out code rate
 *
 * @details
 *
 *     Function : rgSCHCmnFindCodeRate
 *
 *     Processing Steps:
 *
 *  @param[in]      RgSchCellCb     *cell
 *  @param[in]      RgSchDlSf       *dlSf
 *  @param[in,out]  RgSchDlRbAlloc  *allocInfo
 *  @return  void
 **/
#ifdef ANSI
PRIVATE Void rgSCHCmnFindCodeRate
(
RgSchCellCb           *cell,
RgSchDlSf             *dlSf,
RgSchDlRbAlloc        *allocInfo,
U8                    idx
)
#else
PRIVATE Void rgSCHCmnFindCodeRate(cell,dlSf,allocInfo,idx)
RgSchCellCb           *cell;
RgSchDlSf             *dlSf;
RgSchDlRbAlloc        *allocInfo;
U8                    idx;
#endif
{
    U8 numOvrlapgPbchRb = 0;
    U32  numPdschRe = 0;
    U32 code_rate;
    U8 numResvdRePerSpclSf = 0;
    RgSchCmnDlCell   *cellDl    = RG_SCH_CMN_GET_DL_CELL(cell); 

    U8 numResvdRePerSf = ((cellDl->currCfi * RGSCH_NUM_SC_IN_RB) + (cell->numCellRSPerSf - (1 << cell->numTxAntPorts)));

    TRC2(rgSCHCmnFindCodeRate)


    if(dlSf->sfNum == 5)
    {
        rgSCHCmnFindNumPbchOvrlapRbs(cell,dlSf,allocInfo,&numOvrlapgPbchRb);
        numResvdRePerSpclSf = (((cellDl->currCfi + RGSCH_NUM_PSS_SSS_SYM) *
                                  RGSCH_NUM_SC_IN_RB) + (cell->numCellRSPerSf - (1 << cell->numTxAntPorts)));
    }
    else if (dlSf->sfNum == 0)
    {
        rgSCHCmnFindNumPbchOvrlapRbs(cell,dlSf,allocInfo,&numOvrlapgPbchRb);
        numResvdRePerSpclSf = (((cellDl->currCfi + RGSCH_NUM_PSS_SSS_SYM + RGSCH_NUM_PBCH_SYM) *
                                  RGSCH_NUM_SC_IN_RB) + (cell->numCellRSPerSf - ((1 << cell->numTxAntPorts)*2)));
    }

    numPdschRe = (RGSCH_NUM_SC_IN_RB * 14 * allocInfo->rbsReq) - \
                 ((numResvdRePerSf * (allocInfo->rbsReq - numOvrlapgPbchRb)) + \
                  (numResvdRePerSpclSf * numOvrlapgPbchRb));

    code_rate = (((allocInfo->tbInfo[idx].bytesReq)*8*100)/(numPdschRe * 6));
#if 1
#else
    {
       EXTERN U32 gDbgCodeRate;
       if(code_rate > gDbgCodeRate)
       {
          gDbgCodeRate = code_rate;
       }

       if(code_rate > 93)
       {
          EXTERN U32 gDbgCodeRateExceedCnt;
#if 0
#else
          gDbgCodeRateExceedCnt++;
#endif
       }
    }
#endif
    RETVOID;
}
#endif

/* Adjust the Imcs and bytes allocated also with respect to the adjusted
   RBs - Here we will find out the Imcs by identifying first Highest
   number of bits compared to the original bytes allocated.  */
/**
 * @brief Adjust IMCS according to tbSize and ITBS
 *
 * @details
 *
 *     Function : rgSCHCmnNonDlfsPbchTbImcsAdj
 *
 *     Processing Steps:
 *      - Adjust Imcs according to tbSize and ITBS.
 *
 *  @param[in,out]  RgSchDlRbAlloc  *allocInfo
 *  @param[in]      U8              *idx
 *  @return  void
 **/
#ifdef ANSI
PRIVATE Void rgSCHCmnNonDlfsPbchTbImcsAdj
(
RgSchCellCb      *cell,
RgSchDlRbAlloc   *allocInfo,
U8               idx,
U8               rbsReq
)
#else
PRIVATE Void rgSCHCmnNonDlfsPbchTbImcsAdj(cell,allocInfo, idx, rbsReq)
RgSchCellCb      *cell;
RgSchDlRbAlloc   *allocInfo;
U8               idx;
U8               rbsReq;
#endif
{
   U8             noLyrs = 0;
   U8             tbs = 0;
   U32            origBytesReq;
   U8             noRbgs = 0;
   U8             noRbs = 0;
   RgSchDlSf     *dlSf = allocInfo->dlSf;

   RG_SCH_CMN_DL_MCS_TO_TBS(allocInfo->tbInfo[idx].imcs, tbs);
   noLyrs = allocInfo->tbInfo[idx].noLyr;

   if((allocInfo->raType == RG_SCH_CMN_RA_TYPE0))
   {
      noRbgs = RGSCH_CEIL((allocInfo->rbsReq + dlSf->lstRbgDfct), cell->rbgSize);
      noRbs = (noRbgs * cell->rbgSize) - dlSf->lstRbgDfct;
   }
   else
   {
       noRbs = allocInfo->rbsReq;
   }

   /* This line will help in case if tbs is zero and reduction in MCS is not possible */
   if (allocInfo->rbsReq == 0 )
   {
      RETVOID;
   }
   origBytesReq = rgTbSzTbl[noLyrs - 1][tbs][rbsReq - 1]/8;

   /* Find out the ITbs & Imcs by identifying first Highest
      number of bits compared to the original bytes allocated.*/
   if(tbs > 0)
   {
      if(((rgTbSzTbl[noLyrs - 1][0][noRbs - 1])/8) < origBytesReq)
      {
          RGSCH_ARRAY_BOUND_CHECK(cell->instIdx, rgTbSzTbl[noLyrs - 1], tbs);
          while(((rgTbSzTbl[noLyrs - 1][tbs][noRbs - 1])/8) > origBytesReq)
          {
              tbs--;
          }
      }
      else
      {
          tbs = 0;
      }
      allocInfo->tbInfo[idx].bytesReq = rgTbSzTbl[noLyrs - 1][tbs][noRbs - 1]/8;
      allocInfo->tbInfo[idx].iTbs = tbs;
      RG_SCH_CMN_DL_TBS_TO_MCS(tbs,allocInfo->tbInfo[idx].imcs);
   }

   RETVOID;
}
/* Added funcion to adjust TBSize*/
/**
 * @brief Function to adjust the tbsize in case of subframe 0 & 5 when
 * we were not able to do RB alloc adjustment by adding extra required Rbs
 *
 * @details
 *
 *     Function : rgSCHCmnNonDlfsPbchTbSizeAdj
 *
 *     Processing Steps:
 *
 *  @param[in,out]  RgSchDlRbAlloc  *allocInfo
 *  @param[in]      U8            numOvrlapgPbchRb
 *  @param[in]      U8            idx
 *  @param[in]      U8            pbchSsRsSym
 *  @return  void
 **/
#ifdef ANSI
PRIVATE Void rgSCHCmnNonDlfsPbchTbSizeAdj
(
RgSchDlRbAlloc        *allocInfo,
U8                    numOvrlapgPbchRb,
U8                    pbchSsRsSym,
U8                    idx,
U32                   bytesReq
)
#else
PRIVATE Void rgSCHCmnNonDlfsPbchTbSizeAdj(allocInfo,numOvrlapgPbchRb,pbchSsRsSym,idx,bytesReq)
RgSchDlRbAlloc        *allocInfo;
U8                    numOvrlapgPbchRb;
U8                    pbchSsRsSym;
U8                    idx;
U32                   bytesReq;
#endif
{
   U32             reducedTbs = 0;
   U8              noLyrs = 0;
   U8              tbs = 0;

   noLyrs = allocInfo->tbInfo[idx].noLyr;

   RG_SCH_CMN_DL_MCS_TO_TBS(allocInfo->tbInfo[idx].imcs, tbs);

   reducedTbs = bytesReq - (((U32)numOvrlapgPbchRb * (U32)pbchSsRsSym * 6)/8);

   /* find out the ITbs & Imcs by identifying first Highest
    number of bits compared with reduced bits considering the bits that are
    reserved for PBCH/PSS/SSS */
   if(((rgTbSzTbl[noLyrs - 1][0][allocInfo->rbsReq - 1])/8) < reducedTbs)
   {
       while(((rgTbSzTbl[noLyrs - 1][tbs][allocInfo->rbsReq - 1])/8) > reducedTbs)
       {
           tbs--;
       }
   }
   else
   {
       tbs = 0;
   }
   allocInfo->tbInfo[idx].bytesReq = rgTbSzTbl[noLyrs - 1][tbs][allocInfo->rbsReq - 1]/8;
   allocInfo->tbInfo[idx].iTbs = tbs;
   RG_SCH_CMN_DL_TBS_TO_MCS(tbs,allocInfo->tbInfo[idx].imcs);

   RETVOID;
}

/* Added this function to find num of ovrlapping PBCH rb*/
/**
 * @brief Function to find out how many additional rbs are available
 *    in the entire bw which can be allocated to a UE
 * @details
 *
 *     Function : rgSCHCmnFindNumAddtlRbsAvl
 *
 *     Processing Steps:
 *      - Calculates number of additinal rbs available
 *
 *  @param[in]      RgSchCellCb     *cell
 *  @param[in]      RgSchDlSf       *dlSf
 *  @param[in,out]  RgSchDlRbAlloc  *allocInfo
 *  @param[out]      U8            addtlRbsAvl
 *  @return  void
 **/
#ifdef ANSI
PRIVATE U8 rgSCHCmnFindNumAddtlRbsAvl
(
RgSchCellCb           *cell,
RgSchDlSf             *dlSf,
RgSchDlRbAlloc        *allocInfo
)
#else
PRIVATE U8 rgSCHCmnFindNumAddtlRbsAvl(cell,dlSf,allocInfo)
RgSchCellCb           *cell;
RgSchDlSf             *dlSf;
RgSchDlRbAlloc        *allocInfo;
#endif
{
    U8 addtlRbsAvl = 0;

    TRC2(rgSCHCmnFindNumAddtlRbsAvl)

    if (allocInfo->raType == RG_SCH_CMN_RA_TYPE0)
    {
         addtlRbsAvl = (((dlSf->type0End - dlSf->type2End + 1)*\
                        cell->rbgSize) - dlSf->lstRbgDfct) - allocInfo->rbsReq;
    }
    else if (allocInfo->raType == RG_SCH_CMN_RA_TYPE2)
    {
       addtlRbsAvl = (dlSf->bw - dlSf->bwAlloced) - allocInfo->rbsReq;
    }

    RETVALUE(addtlRbsAvl);

}
/* Added this function to find num of ovrlapping PBCH rb*/
/**
 * @brief Function to find out how many of the requested RBs are
 *        falling in the center 6 RBs of the downlink bandwidth.
 * @details
 *
 *     Function : rgSCHCmnFindNumPbchOvrlapRbs
 *
 *     Processing Steps:
 *      - Calculates number of overlapping rbs
 *
 *  @param[in]      RgSchCellCb     *cell
 *  @param[in]      RgSchDlSf       *dlSf
 *  @param[in,out]  RgSchDlRbAlloc  *allocInfo
 *  @param[out]      U8*            numOvrlapgPbchRb
 *  @return  void
 **/
#ifdef ANSI
PRIVATE Void rgSCHCmnFindNumPbchOvrlapRbs
(
RgSchCellCb           *cell,
RgSchDlSf             *dlSf,
RgSchDlRbAlloc        *allocInfo,
U8                    *numOvrlapgPbchRb
)
#else
PRIVATE Void rgSCHCmnFindNumPbchOvrlapRbs(cell,dlSf,allocInfo,numOvrlapgPbchRb)
RgSchCellCb           *cell;
RgSchDlSf             *dlSf;
RgSchDlRbAlloc        *allocInfo;
U8                    *numOvrlapgPbchRb;
#endif
{
    *numOvrlapgPbchRb = 0;
    TRC2(rgSCHCmnFindNumPbchOvrlapRbs)
   /*Find if we have already crossed the start boundary for PBCH 6 RBs,
    * if yes then lets find the number of RBs which are getting overlapped
    * with this allocation.*/
   if(dlSf->bwAlloced <= (cell->pbchRbStart))
   {
      /*We have not crossed the start boundary of PBCH RBs. Now we need
       * to know that if take this allocation then how much PBCH RBs
       * are overlapping with this allocation.*/
      /* Find out the overlapping RBs in the centre 6 RBs */
       if((dlSf->bwAlloced + allocInfo->rbsReq) > cell->pbchRbStart)
       {
           *numOvrlapgPbchRb = (dlSf->bwAlloced + allocInfo->rbsReq) - (cell->pbchRbStart);
           if(*numOvrlapgPbchRb > 6)
                *numOvrlapgPbchRb = 6;
       }
   }
   else if ((dlSf->bwAlloced > (cell->pbchRbStart)) &&
         (dlSf->bwAlloced < (cell->pbchRbEnd)))
   {
      /*We have already crossed the start boundary of PBCH RBs.We need to
       * find that if we take this allocation then how much of the RBs for
       * this allocation will overlap with PBCH RBs.*/
      /* Find out the overlapping RBs in the centre 6 RBs */
      if(dlSf->bwAlloced + allocInfo->rbsReq < (cell->pbchRbEnd))
      {
         /*If we take this allocation then also we are not crossing the
          * end boundary of PBCH 6 RBs.*/
         *numOvrlapgPbchRb = allocInfo->rbsReq;
      }
      else
      {
         /*If we take this allocation then we are crossing the
          * end boundary of PBCH 6 RBs.*/
         *numOvrlapgPbchRb = (cell->pbchRbEnd) - dlSf->bwAlloced;
      }
   }
    RETVOID;

}
/**
 * @brief Performs RB allocation adjustment if the requested RBs are
 *        falling in the center 6 RBs of the downlink bandwidth.
 * @details
 *
 *     Function : rgSCHCmnNonDlfsPbchRbAllocAdj
 *
 *     Processing Steps:
 *      - Allocate consecutively available RBs.
 *
 *  @param[in]      RgSchCellCb     *cell
 *  @param[in,out]  RgSchDlRbAlloc  *allocInfo
 *  @param[in]      U8               pbchSsRsSym
 *  @return  void
 **/
#ifdef ANSI
PRIVATE Void rgSCHCmnNonDlfsPbchRbAllocAdj
(
RgSchCellCb      *cell,
RgSchDlRbAlloc   *allocInfo,
U8               pbchSsRsSym,
Bool             isBcchPcch
)
#else
PRIVATE Void rgSCHCmnNonDlfsPbchRbAllocAdj(cell, allocInfo,pbchSsRsSym)
RgSchCellCb      *cell;
RgSchDlRbAlloc   *allocInfo;
U8               pbchSsRsSym;
Bool             isBcchPcch;
#endif
{
   RgSchDlSf     *dlSf = allocInfo->dlSf;
   U8             numOvrlapgPbchRb = 0;
   U8             numOvrlapgAdtlPbchRb = 0;
   U8             totSym;
   U8             addtlRbsReq = 0;
   U8             moreAddtlRbsReq = 0;
   U8             addtlRbsAdd = 0;
   U8             moreAddtlRbsAdd = 0;
   U8             tbs;
   U8             origRbsReq = 0;
   U32            bytesReq;
   U8             noLyr;
   U8             divResult;


   TRC2(rgSCHCmnNonDlfsPbchRbAllocAdj);


   origRbsReq = allocInfo->rbsReq;
   rgSCHCmnFindNumPbchOvrlapRbs(cell,dlSf,allocInfo,&numOvrlapgPbchRb);

  totSym =  (cell->isCpDlExtend) ? RGSCH_TOT_NUM_SYM_EXTCP : RGSCH_TOT_NUM_SYM_NORCP;

   /* Additional RBs are allocated by considering the loss due to
      the reserved symbols for CFICH, PBCH, PSS, SSS and cell specific RS */

   divResult = (numOvrlapgPbchRb * pbchSsRsSym)/totSym;
   if((numOvrlapgPbchRb * pbchSsRsSym) % totSym)
   {
      divResult++;
   }
   addtlRbsReq = divResult;

   RG_SCH_CMN_UPD_RBS_TO_ADD(cell, dlSf, allocInfo, addtlRbsReq, addtlRbsAdd)

   /*Now RBs requires is original requested RBs + these additional RBs to make
    * up for PSS/SSS/BCCH.*/
   allocInfo->rbsReq = allocInfo->rbsReq + addtlRbsAdd;

   /*Check if with these additional RBs we have taken up, these are also falling
    * under PBCH RBs range, if yes then we would need to account for
    * PSS/BSS/BCCH for these additional RBs too.*/
   if(addtlRbsAdd && ((dlSf->bwAlloced + allocInfo->rbsReq - addtlRbsAdd) < (cell->pbchRbEnd)))
   {
      if((dlSf->bwAlloced + allocInfo->rbsReq) <= (cell->pbchRbEnd))
      {
      /*With additional RBs taken into account, we are not crossing the
       * PBCH RB end boundary.Thus here we need to account just for
       * overlapping PBCH RBs for these additonal RBs.*/
          divResult = (addtlRbsAdd * pbchSsRsSym)/totSym;
          if((addtlRbsAdd * pbchSsRsSym) % totSym)
          {
            divResult++;
          }

          moreAddtlRbsReq = divResult;

          RG_SCH_CMN_UPD_RBS_TO_ADD(cell, dlSf, allocInfo, moreAddtlRbsReq, moreAddtlRbsAdd)

          allocInfo->rbsReq = allocInfo->rbsReq + moreAddtlRbsAdd;
      }
      else
      {

         /*Here we have crossed the PBCH RB end boundary, thus we need to take
          * into account the overlapping RBs for additional RBs which will be
          * subset of addtlRbs.*/
          numOvrlapgAdtlPbchRb = (cell->pbchRbEnd) - ((dlSf->bwAlloced + allocInfo->rbsReq) -  addtlRbsAdd);

          divResult = (numOvrlapgAdtlPbchRb * pbchSsRsSym)/totSym;
          if((numOvrlapgAdtlPbchRb * pbchSsRsSym) % totSym)
          {
             divResult++;
          }

          moreAddtlRbsReq =  divResult;

          RG_SCH_CMN_UPD_RBS_TO_ADD(cell, dlSf, allocInfo, moreAddtlRbsReq, moreAddtlRbsAdd)

          allocInfo->rbsReq = allocInfo->rbsReq + moreAddtlRbsAdd;
      }
   }
   if (isBcchPcch == TRUE)
   {
      RETVOID;
   }

   RG_SCH_CMN_DL_MCS_TO_TBS(allocInfo->tbInfo[0].imcs, tbs);
   if(tbs == 6)
   {
      /* This case might be for Imcs value 6 and NPrb = 1 case  - Not
         Adjusting either RBs or Imcs or Bytes Allocated */
      allocInfo->rbsReq = allocInfo->rbsReq - addtlRbsAdd - moreAddtlRbsAdd;
   }
   else if(tbs && ((0 == addtlRbsAdd) && (moreAddtlRbsAdd == 0)))
   {
       /*In case of a situation where we the entire bandwidth is already occupied
        * and we dont have room to add additional Rbs then in order to decrease the
        * code rate we reduce the tbsize such that we reduce the present calculated
        * tbsize by number of bytes that would be occupied by PBCH/PSS/SSS in overlapping
        * rbs and find the nearest tbsize which would be less than this deduced value*/

      rgSCHCmnFindNumPbchOvrlapRbs(cell,dlSf,allocInfo,&numOvrlapgPbchRb);

      noLyr = allocInfo->tbInfo[0].noLyr;
      RGSCH_ARRAY_BOUND_CHECK(cell->instIdx, rgTbSzTbl[noLyr - 1], tbs);
      bytesReq = rgTbSzTbl[noLyr - 1][tbs][allocInfo->rbsReq - 1]/8;

      rgSCHCmnNonDlfsPbchTbSizeAdj(allocInfo,numOvrlapgPbchRb,pbchSsRsSym,0,bytesReq);

      if(allocInfo->tbInfo[1].schdlngForTb == TRUE)
      {
          noLyr = allocInfo->tbInfo[1].noLyr;
          bytesReq = rgTbSzTbl[noLyr - 1][tbs][allocInfo->rbsReq - 1]/8;
          rgSCHCmnNonDlfsPbchTbSizeAdj(allocInfo,numOvrlapgPbchRb,pbchSsRsSym,1,bytesReq);
      }

   }
   else if(tbs && ((addtlRbsAdd != addtlRbsReq) ||
          (addtlRbsAdd && (moreAddtlRbsReq != moreAddtlRbsAdd))))
   {
       /*In case of a situation where we were not able to add required number of
        * additional RBs then we adjust the Imcs based on original RBs requested.
        * Doing this would comensate for the few extra Rbs we have added but inorder
        * to comensate for number of RBS we couldnt add we again do the TBSize adjustment*/

      rgSCHCmnNonDlfsPbchTbImcsAdj(cell, allocInfo, 0 , origRbsReq);

      if(allocInfo->tbInfo[1].schdlngForTb == TRUE)
      {
          rgSCHCmnNonDlfsPbchTbImcsAdj(cell, allocInfo, 1 , origRbsReq);
      }

      rgSCHCmnFindNumPbchOvrlapRbs(cell,dlSf,allocInfo,&numOvrlapgPbchRb);
      numOvrlapgPbchRb = numOvrlapgPbchRb - (addtlRbsAdd + moreAddtlRbsAdd);

      rgSCHCmnNonDlfsPbchTbSizeAdj(allocInfo,numOvrlapgPbchRb,pbchSsRsSym,0,allocInfo->tbInfo[0].bytesReq);

      if(allocInfo->tbInfo[1].schdlngForTb == TRUE)
      {
          rgSCHCmnNonDlfsPbchTbSizeAdj(allocInfo,numOvrlapgPbchRb,pbchSsRsSym,1,allocInfo->tbInfo[1].bytesReq);
      }

   }
   else
   {
       /*We hit this code when we were able to add the required additional RBS
        * hence we should adjust the IMcs based on orignals RBs requested*/

      rgSCHCmnNonDlfsPbchTbImcsAdj(cell, allocInfo, 0 , origRbsReq);

      if(allocInfo->tbInfo[1].schdlngForTb == TRUE)
      {
          rgSCHCmnNonDlfsPbchTbImcsAdj(cell, allocInfo, 1 , origRbsReq);
      }
   }

   RETVOID;
} /* end of rgSCHCmnNonDlfsPbchRbAllocAdj */
#endif

/**
 * @brief Performs RB allocation for frequency non-selective cell.
 *
 * @details
 *
 *     Function : rgSCHCmnNonDlfsCmnRbAlloc
 *
 *     Processing Steps:
 *      - Allocate consecutively available RBs for BCCH/PCCH/RAR.
 *
 *  @param[in]      RgSchCellCb     *cell
 *  @param[in, out] RgSchDlRbAlloc  *allocInfo
 *  @return  S16
 *      -# ROK
 *      -# RFAILED
 **/
#ifdef ANSI
PRIVATE S16 rgSCHCmnNonDlfsCmnRbAlloc
(
RgSchCellCb      *cell,
RgSchDlRbAlloc   *allocInfo
)
#else
PRIVATE S16 rgSCHCmnNonDlfsCmnRbAlloc(cell, allocInfo)
RgSchCellCb      *cell;
RgSchDlRbAlloc   *allocInfo;
#endif
{
#ifndef LTE_TDD
#ifdef LTEMAC_SPS
   U8               rbsAlloc = 0;
#endif
   U8 pbchSsRsSym = 0;
   U8 pbchFrame = 0;
   U8  tbs = 0;
   RgSchCmnDlCell   *cellDl    = RG_SCH_CMN_GET_DL_CELL(cell); 
#endif
   RgSchDlSf     *dlSf   = allocInfo->dlSf;
#ifdef LTEMAC_SPS
   U8                  rbStart = 0;
   U8                  spsRbsAlloc = 0;
   RgSchDlSfAllocInfo  *dlSfAlloc = &allocInfo->dlSf->dlSfAllocInfo;
#endif
   TRC2(rgSCHCmnNonDlfsCmnRbAlloc);

   allocInfo->tbInfo[0].noLyr = 1;

#ifdef LTEMAC_SPS
   /* Note: Initialize the masks to 0, this might not be needed since alloInfo
    * is initialized to 0 at the beginning of allcoation */
   allocInfo->resAllocInfo.raType0Mask = 0;
   cmMemset((U8*)allocInfo->resAllocInfo.raType1Mask, 0,
         RG_SCH_NUM_RATYPE1_32BIT_MASK * sizeof (U32));
   cmMemset((U8*)allocInfo->resAllocInfo.raType2Mask, 0,
         RG_SCH_NUM_RATYPE2_32BIT_MASK * sizeof (U32));

   if ((dlSf->spsAllocdBw >= cell->spsBwRbgInfo.numRbs) &&
         (dlSf->bwAlloced == dlSf->bw))
#else
   if(dlSf->bwAlloced == dlSf->bw)
#endif
   {
      RETVALUE(RFAILED);
   }
#ifndef LTE_TDD
   if (allocInfo->rbsReq > (dlSf->bw - dlSf->bwAlloced))
   {
#ifdef LTEMAC_SPS
      if ((allocInfo->tbInfo[0].imcs < 29) && (dlSf->bwAlloced < dlSf->bw))
#else
      if(allocInfo->tbInfo[0].imcs < 29)
#endif
      {
         /* set the remaining RBs for the requested UE */
         allocInfo->rbsReq = dlSf->bw - dlSf->bwAlloced;
         RG_SCH_CMN_DL_MCS_TO_TBS(allocInfo->tbInfo[0].imcs, tbs);
         allocInfo->tbInfo[0].bytesReq = rgTbSzTbl[0][tbs][allocInfo->rbsReq - 1]/8;
      }
      else
      {
#ifdef LTEMAC_SPS
         /* Attempt RA Type 2 allocation in SPS Bandwidth */
         if (dlSf->spsAllocdBw < cell->spsBwRbgInfo.numRbs) 
         {
            spsRbsAlloc =
               rgSCHCmnDlRaType2Alloc(dlSfAlloc,
                     allocInfo->rbsReq, &cell->spsBwRbgInfo, &rbStart,
                     &allocInfo->resAllocInfo, FALSE);
            /* rbsAlloc assignment moved from line 16671 to here to avoid
             * compilation error. Recheck */
            rbsAlloc = spsRbsAlloc;
            dlSf->spsAllocdBw += spsRbsAlloc;
         }
         if (!spsRbsAlloc)
#endif /* LTEMAC_SPS */
         {
            RETVALUE(RFAILED);
         }
      }
   }
#endif

   /* Update allocation information */
   allocInfo->pdcch = rgSCHCmnCmnPdcchAlloc(cell, dlSf);
   if (allocInfo->pdcch == NULLP)
   {
      RETVALUE(RFAILED);
   }
   allocInfo->dciFormat = TFU_DCI_FORMAT_1A;
   allocInfo->raType = RG_SCH_CMN_RA_TYPE2;
   allocInfo->allocInfo.raType2.isLocal = TRUE;
#ifdef LTEMAC_SPS
   if (spsRbsAlloc) 
   {
      allocInfo->allocInfo.raType2.rbStart = rbStart;
      allocInfo->allocInfo.raType2.numRb = allocInfo->rbsReq;
      allocInfo->rbsAlloc = allocInfo->rbsReq;
   }
#endif

#ifdef LTEMAC_SPS
   if (!spsRbsAlloc)
   {
#endif
#ifndef LTE_TDD
      if(dlSf->sfNum)
      {
         if(!(dlSf->sfNum == 5))
         {
            /* case for subframes 1 to 9 except 5 */
#ifdef LTEMAC_SPS
            allocInfo->allocInfo.raType2.rbStart = rbStart;
#else
            /*Fix for ccpu00123918*/
            allocInfo->allocInfo.raType2.rbStart = (U8)dlSf->type2Start;
#endif
         }
         else
         {
            pbchFrame = 1; /* case for subframe 5 */
            /* In subframe 5, symbols are reserved for PSS and SSS and CFICH
               and Cell Specific Reference Signals */
            pbchSsRsSym = (((cellDl->currCfi) + RGSCH_NUM_PSS_SSS_SYM) *
                  RGSCH_NUM_SC_IN_RB + cell->numCellRSPerSf);
         }
      }
      else
      {
         pbchFrame = 1;
         /* In subframe 0, symbols are reserved for PSS, SSS, PBCH, CFICH and
            and Cell Specific Reference signals */
         pbchSsRsSym = (((cellDl->currCfi) + RGSCH_NUM_PBCH_SYM +
                  RGSCH_NUM_PSS_SSS_SYM) * RGSCH_NUM_SC_IN_RB +
               cell->numCellRSPerSf);
      } /* end of outer else */

      if((pbchFrame) &&
            (((dlSf->bwAlloced + allocInfo->rbsReq) - cell->pbchRbStart) > 0)&&
            (dlSf->bwAlloced < cell->pbchRbEnd))
      {
         if(allocInfo->tbInfo[0].imcs < 29)
         {
            rgSCHCmnNonDlfsPbchRbAllocAdj(cell, allocInfo, pbchSsRsSym, TRUE);
         }
      }
#endif
#ifdef LTEMAC_SPS
   }
#endif

#ifdef LTEMAC_SPS
   if (!spsRbsAlloc)
   {  
#endif
      /*Fix for ccpu00123918*/
      allocInfo->allocInfo.raType2.rbStart = (U8)dlSf->type2Start;
      allocInfo->allocInfo.raType2.numRb = allocInfo->rbsReq;
      allocInfo->rbsAlloc = allocInfo->rbsReq;
#if 0
      QU_CRITLOG(QU_LOG_MESSAGE,
            "Allocating the raType2 rbStart(%d) numRB(%d)",
            allocInfo->allocInfo.raType2.rbStart,
            allocInfo->rbsReq);
#endif
      /* LTE_ADV_FLAG_REMOVED_START */
#ifndef LTE_TDD
      if (cell->lteAdvCb.sfrCfg.status == RGR_ENABLE)
      {
         rgSCHCmnNonDlfsSFRCmnChannelUpdTyp2Alloc(cell, dlSf, \
               allocInfo->allocInfo.raType2.rbStart, \
               allocInfo->allocInfo.raType2.numRb);
      }
      else
#endif
      {
         rgSCHCmnNonDlfsUpdTyp2Alloc(cell, dlSf, \
               allocInfo->allocInfo.raType2.rbStart, \
               allocInfo->allocInfo.raType2.numRb);
      }

#ifdef LTEMAC_SPS
   }
#endif
   /* LTE_ADV_FLAG_REMOVED_END */
   allocInfo->tbInfo[0].bytesAlloc = allocInfo->tbInfo[0].bytesReq;


#ifdef LTEMAC_SPS
   if (spsRbsAlloc)
   {
      U8    idx;
      /* Update type 0, 1 and 2 masks */
      dlSfAlloc->raType0Mask    |= allocInfo->resAllocInfo.raType0Mask;
#ifdef RGSCH_SPS_UNUSED
      for (idx = 0; idx < RG_SCH_NUM_RATYPE1_32BIT_MASK; ++idx)
      {
         dlSfAlloc->raType1Mask[idx] |=
            allocInfo->resAllocInfo.raType1Mask[idx];
         dlSfAlloc->raType1UsedRbs[idx] +=
            allocInfo->resAllocInfo.raType1UsedRbs[idx];
      }
#endif
      for (idx = 0; idx < RG_SCH_NUM_RATYPE2_32BIT_MASK; ++idx)
      {
         dlSfAlloc->raType2Mask[idx] |=
            allocInfo->resAllocInfo.raType2Mask[idx];
      }
   }
#endif

   RETVALUE(ROK);
}
/* LTE_ADV_FLAG_REMOVED_START */
#ifndef LTE_TDD
/**
 * @brief To check if DL BW available for non-DLFS allocation.
 *
 * @details
 *
 *     Function : rgSCHCmnNonDlfsBwAvlbl
 *
 *     Processing Steps:
 *      - Determine availability based on RA Type.
 *
 *  @param[in]  RgSchCellCb     *cell
 *  @param[in]  RgSchDlSf       *dlSf
 *  @param[in]  RgSchDlRbAlloc  *allocInfo
 *
 *  @return Bool
 *      -# TRUE
 *      -# FALSE
 **/
#ifdef ANSI
PRIVATE Bool rgSCHCmnNonDlfsSFRBwAvlbl
(
RgSchCellCb        *cell,
RgSchSFRPoolInfo   **sfrpoolInfo,
RgSchDlSf          *dlSf,
RgSchDlRbAlloc     *allocInfo,
Bool               isUeCellEdge
)
#else
PRIVATE Bool rgSCHCmnNonDlfsSFRBwAvlbl(cell, sfrpoolInfo, dlSf, allocInfo, isUeCellEdge)
RgSchCellCb        *cell;
RgSchSFRPoolInfo   **sfrpoolInfo;
RgSchDlSf          *dlSf;
RgSchDlRbAlloc     *allocInfo;
Bool               isUeCellEdge;
#endif
{
   CmLListCp   *l;
   CmLListCp   *l1;
   CmLList     *n;
   CmLList     *n1;
   RgSchSFRPoolInfo  *sfrPool;
   RgSchSFRPoolInfo  *sfrCEPool;

   U8 tbs;
   U8 noLyrs;
   RgSchSFRPoolInfo *poolWithMaxAvlblBw = NULLP;
   U32 bwAvlbl = 0;
   U32 addtnlPRBs = 0;

   if (dlSf->bw <= dlSf->bwAlloced)
   {
      RLOG_ARG2(L_ERROR,DBG_CELLID,cell->cellId, 
            "BW is fully allocated for subframe (%d) CRNTI:%d", dlSf->sfNum,allocInfo->rnti);
      return FALSE;
   }

   if (dlSf->sfrTotalPoolInfo.ccBwFull == TRUE)
   {
      RLOG_ARG1(L_ERROR,DBG_CELLID,cell->cellId, 
            "BW is fully allocated for CC Pool CRNTI:%d",allocInfo->rnti);
      return FALSE;
   }

   if ((dlSf->sfrTotalPoolInfo.ceBwFull == TRUE) && (isUeCellEdge))
   {
      RLOG_ARG1(L_ERROR,DBG_CELLID,cell->cellId, 
            "BW is fully allocated for CE Pool CRNTI:%d",allocInfo->rnti);
      return FALSE;
   }  

   /* We first check if the ue scheduled is a cell edge or cell centre and accordingly check the avaialble
      memory in their pool. If the cell centre UE doesnt have Bw available in its pool, then it will check
      Bw availability in cell edge pool but the other way around is NOT possible.   */
   if(isUeCellEdge)
   {   
      l = &dlSf->sfrTotalPoolInfo.cePool;
   }
   else
   {
      l = &dlSf->sfrTotalPoolInfo.ccPool; 
   }    

   n = cmLListFirst(l);

   while(n)     
   {
      if (allocInfo->raType == RG_SCH_CMN_RA_TYPE0)
      {
         sfrPool = (RgSchSFRPoolInfo*)(n->node);

         /* MS_FIX for ccpu00123919 : Number of RBs in case of RETX should be same as that of initial transmission. */
         if(allocInfo->tbInfo[0].tbCb->txCntr)
         {
            /* If RB assignment is being done for RETX. Then if reqRbs are   a multiple of rbgSize then ignore lstRbgDfct. If reqRbs is 
             * not a multiple of rbgSize then check if lsgRbgDfct exists */
            if (allocInfo->rbsReq % cell->rbgSize == 0)
            {
               if ((sfrPool->type2End == dlSf->type2End) && dlSf->lstRbgDfct)
               {
                  /* In this scenario we are wasting the last RBG for this dlSf */
                  sfrPool->type0End--;
                  sfrPool->bwAlloced += (cell->rbgSize - dlSf->lstRbgDfct);

                  dlSf->lstRbgDfct = 0;

                  /*ABHINAV To check if these variables need to be taken care of*/
                  dlSf->type0End--;
                  dlSf->bwAlloced += (cell->rbgSize - dlSf->lstRbgDfct);
               }
            }
            else
            {
               if (dlSf->lstRbgDfct)
               {
                  /* Check if type0 allocation can cater to this RETX requirement */
                  if ((allocInfo->rbsReq % cell->rbgSize) != (cell->rbgSize - dlSf->lstRbgDfct))
                  {
                     RETVALUE(FALSE);
                  }
                  else
                  {
                     if (sfrPool->type2End != dlSf->type2End)   /*Search again for some pool which has the END RBG of the BandWidth*/
                     {
                        continue;               
                     }   
                  }
               }
               else
               {
                  /* cannot allocate same number of required RBs */
                  RETVALUE(FALSE);           
               }
            }
         }

         /*rg002.301 ccpu00120391 MOD condition is modified approprialtely to find if rbsReq is less than available RBS*/
         if(allocInfo->rbsReq <= (((sfrPool->type0End - sfrPool->type2End + 1)*\
                     cell->rbgSize) - dlSf->lstRbgDfct))
         {
            *sfrpoolInfo = sfrPool;
            RETVALUE(TRUE);
         }
         else
         {
            if (sfrPool->bw <= sfrPool->bwAlloced + cell->rbgSize)
            {
               n = cmLListNext(l);
               /* If the ue is cell centre then it will simply check the memory available in next pool.
                  But if there are no more memory pools available, then cell centre Ue will try to look for memory in cell edge pool */

               if((!isUeCellEdge) && (!n->node))
               {
                  l = &dlSf->sfrTotalPoolInfo.cePool;
                  n = cmLListFirst(l);
               }

               continue; 
            }     

            /* MS_FIX: Number of RBs in case of RETX should be same as that of initial transmission */
            if(allocInfo->tbInfo[0].tbCb->txCntr == 0)
            {
               /*rg002.301 ccpu00120391 MOD setting the remaining RBs  for the requested UE*/
               allocInfo->rbsReq = (((sfrPool->type0End - sfrPool->type2End + 1)*\
                        cell->rbgSize) - dlSf->lstRbgDfct);
               RG_SCH_CMN_DL_MCS_TO_TBS(allocInfo->tbInfo[0].imcs, tbs);
               noLyrs = allocInfo->tbInfo[0].noLyr;
               allocInfo->tbInfo[0].bytesReq = rgTbSzTbl[noLyrs-1][tbs][allocInfo->rbsReq - 1]/8;
               *sfrpoolInfo = sfrPool;
               RETVALUE(TRUE);
            }
            else
            {
               n = cmLListNext(l);

               /* If the ue is cell centre then it will simply check the memory available in next pool.
                  But if there are no more memory pools available, then cell centre Ue will try to look for memory in cell edge pool */
               if((!isUeCellEdge) && (!n->node))
               {
                  l = &dlSf->sfrTotalPoolInfo.cePool;
                  n = cmLListFirst(l);
               }

               continue;
            }

         //   RETVALUE(FALSE);
         }
      }
      else if (allocInfo->raType == RG_SCH_CMN_RA_TYPE2)
      {
         sfrPool = (RgSchSFRPoolInfo*)(n->node);
         /* This is a Case where a UE was CC and had more RBs allocated than present in CE pool.
            In case this UE whn become CE with retx going on, then BW is not sufficient for Retx */
         if ((isUeCellEdge) &&
            (allocInfo->tbInfo[0].tbCb->txCntr != 0))
         {
            if(allocInfo->rbsReq > (sfrPool->bw - sfrPool->bwAlloced))
            {
               /* Adjust CE BW such that Retx alloc is successful */
               /* Check if merging CE with adjacent CC pool will be sufficient to process Retx */

               /* If no Type 0 allocations are made from this pool */
               if (sfrPool->type0End == (((sfrPool->poolendRB + 1) / cell->rbgSize) - 1))
               {
                  if (sfrPool->adjCCPool &&
                        (sfrPool->adjCCPool->type2Start == sfrPool->poolendRB + 1) &&
                        (allocInfo->rbsReq <= ((sfrPool->bw - sfrPool->bwAlloced) + 
                                               ((sfrPool->adjCCPool->bw - sfrPool->adjCCPool->bwAlloced)))))
                  {
                     addtnlPRBs = allocInfo->rbsReq - (sfrPool->bw - sfrPool->bwAlloced);

                     /* Adjusting CE Pool Info */
                     sfrPool->bw += addtnlPRBs;
                     sfrPool->type0End = ((sfrPool->poolendRB + addtnlPRBs + 1) /
                           cell->rbgSize) - 1;

                     /* Adjusting CC Pool Info */
                     sfrPool->adjCCPool->type2Start += addtnlPRBs;
                     sfrPool->adjCCPool->type2End = RGSCH_CEIL(sfrPool->adjCCPool->type2Start, 
                           cell->rbgSize);
                     sfrPool->adjCCPool->bw -= addtnlPRBs;
                     *sfrpoolInfo = sfrPool;
                     RETVALUE(TRUE);
                  }
               }
            }
         }

         /* Check if CC pool is one of the following:
          * 1. |CE| + |CC "CCPool2Exists" = TRUE|
          * 2. |CC "CCPool2Exists" = FALSE| + |CE| + |CC "CCPool2Exists" = TRUE|
          */ 
         if(TRUE == sfrPool->CCPool2Exists)
         {
            l1 = &dlSf->sfrTotalPoolInfo.cePool;
            n1 = cmLListFirst(l1); 
            sfrCEPool = (RgSchSFRPoolInfo*)(n1->node);
            if(allocInfo->rbsReq <= (sfrCEPool->bw - sfrCEPool->bwAlloced))
            {
               *sfrpoolInfo = sfrCEPool;
               RETVALUE(TRUE);
            }
            else if(allocInfo->rbsReq <= (sfrPool->bw - sfrPool->bwAlloced))  
            {
               *sfrpoolInfo = sfrPool;
               RETVALUE(TRUE);
            }
            /* Check if CE and CC boundary has unallocated prbs */
            else if ((sfrPool->poolstartRB == sfrPool->type2Start) &&
                  (sfrCEPool->type0End  == ((sfrCEPool->poolendRB + 1) / cell->rbgSize) - 1))
            {
               if(allocInfo->rbsReq <= (sfrCEPool->bw - sfrCEPool->bwAlloced) + 
                     (sfrPool->bw - sfrPool->bwAlloced))
               {
                  /* Checking if BW can be allocated partly from CE pool and partly
                   * from CC pool
                   */
                  addtnlPRBs = allocInfo->rbsReq - (sfrPool->bw - sfrPool->bwAlloced);
                  /* Updating CE and CC  type2 parametrs based on the RBs allocated
                   * from these pools*/
                  sfrPool->type2Start -= addtnlPRBs;
                  sfrPool->type2End = RGSCH_CEIL(sfrPool->type2Start, cell->rbgSize);
                  sfrPool->bw += addtnlPRBs;
                  if (addtnlPRBs == (sfrCEPool->bw - sfrCEPool->bwAlloced))
                  {
                     sfrCEPool->bwAlloced  = sfrCEPool->bw; 
                     dlSf->sfrTotalPoolInfo.ceBwFull = TRUE;
                  }
                  else
                  {
                     sfrCEPool->bw -= addtnlPRBs;
                     sfrCEPool->type0End = ((sfrCEPool->poolendRB + 1 - addtnlPRBs) / cell->rbgSize) - 1;
                  }
                  *sfrpoolInfo = sfrPool;
                  RETVALUE(TRUE);
               }
               else if ( bwAvlbl < 
                     ((sfrCEPool->bw - sfrCEPool->bwAlloced) +
                      (sfrPool->bw - sfrPool->bwAlloced)))
               {
                  /* All the Prbs from CE BW shall be allocated */
                  if(allocInfo->tbInfo[0].tbCb->txCntr == 0)
                  {
                     sfrPool->type2Start   = sfrCEPool->type2Start;
                     sfrPool->bw          += sfrCEPool->bw - sfrCEPool->bwAlloced;
                     sfrCEPool->type2Start = sfrCEPool->poolendRB + 1;
                     sfrCEPool->bwAlloced  = sfrCEPool->bw; 
                     dlSf->sfrTotalPoolInfo.ceBwFull = TRUE;

                     /* set the remaining RBs for the requested UE */
                     allocInfo->rbsReq = (sfrPool->bw - sfrPool->bwAlloced);
                     RG_SCH_CMN_DL_MCS_TO_TBS(allocInfo->tbInfo[0].imcs, tbs);
                     noLyrs = allocInfo->tbInfo[0].noLyr;
                     allocInfo->tbInfo[0].bytesReq = 
                        rgTbSzTbl[noLyrs-1][tbs][allocInfo->rbsReq - 1]/8;
                     *sfrpoolInfo = sfrPool;         
                     RETVALUE(TRUE);
                  }
                  else
                  {
                     RETVALUE(FALSE);
                  }
               }
            }
         } 

         /* Checking if no. of RBs required can be allocated from
          * SFR pool. 
          * 1. If available return the SFR pool.
          * 2. Else update the RBs required parameter based on the 
          *    BW available in the pool 
          * 3. Return FALSE if no B/W is available. 
          */
         if (allocInfo->rbsReq <= (sfrPool->bw - sfrPool->bwAlloced))
         {
            *sfrpoolInfo = sfrPool;
            RETVALUE(TRUE);
         }
         else
         {
            if(allocInfo->tbInfo[0].tbCb->txCntr == 0)
            {
               if (bwAvlbl < sfrPool->bw - sfrPool->bwAlloced)
               {
                  if (isUeCellEdge)
                  {
                     dlSf->sfrTotalPoolInfo.ceBwFull = TRUE; 
                  }
                  bwAvlbl = sfrPool->bw - sfrPool->bwAlloced;
                  poolWithMaxAvlblBw = sfrPool;
               }
               n = cmLListNext(l);

               if ((isUeCellEdge == FALSE) && (n == NULLP))
               {
                  if(l != &dlSf->sfrTotalPoolInfo.cePool)
                  {
                     l = &dlSf->sfrTotalPoolInfo.cePool;
                     n = cmLListFirst(l);            
                  }
               }

               if (n == NULLP)
               {
                  if (bwAvlbl == 0)
                  {                                      
                     if (isUeCellEdge)
                     {
                        dlSf->sfrTotalPoolInfo.ceBwFull = TRUE; 
                     }
                     else
                     {
                        dlSf->sfrTotalPoolInfo.ccBwFull = TRUE;  
                     }
                     RETVALUE(FALSE);
                  }
                  else
                  {
                     /* set the remaining RBs for the requested UE */
                     allocInfo->rbsReq = poolWithMaxAvlblBw->bw - 
                        poolWithMaxAvlblBw->bwAlloced;
                     RG_SCH_CMN_DL_MCS_TO_TBS(allocInfo->tbInfo[0].imcs, tbs);
                     noLyrs = allocInfo->tbInfo[0].noLyr;
                     allocInfo->tbInfo[0].bytesReq = 
                        rgTbSzTbl[noLyrs-1][tbs][allocInfo->rbsReq - 1]/8;
                     *sfrpoolInfo = poolWithMaxAvlblBw;         
                     RETVALUE(TRUE);
                  }
               }              
            }
            else
            {         
               n = cmLListNext(l);

               if ((isUeCellEdge == FALSE) && (n == NULLP))
               {
                  if(l != &dlSf->sfrTotalPoolInfo.cePool)
                  {
                     l = &dlSf->sfrTotalPoolInfo.cePool;
                     n = cmLListFirst(l);            
                  }
               }

               if (n == NULLP)
               {
                  RETVALUE(FALSE);
               }
            }

         }
      }   
   } 
   RETVALUE(FALSE);
}
#endif /* end of ifndef LTE_TDD*/
/* LTE_ADV_FLAG_REMOVED_END */

/**
 * @brief To check if DL BW available for non-DLFS allocation.
 *
 * @details
 *
 *     Function : rgSCHCmnNonDlfsUeRbAlloc
 *
 *     Processing Steps:
 *      - Determine availability based on RA Type.
 *
 *  @param[in]  RgSchCellCb     *cell
 *  @param[in]  RgSchDlSf       *dlSf
 *  @param[in]  RgSchDlRbAlloc  *allocInfo
 *
 *  @return Bool
 *      -# TRUE
 *      -# FALSE
 **/
#ifdef ANSI
PRIVATE Bool rgSCHCmnNonDlfsBwAvlbl
(
RgSchCellCb        *cell,
RgSchDlSf          *dlSf,
RgSchDlRbAlloc     *allocInfo
)
#else
PRIVATE Bool rgSCHCmnNonDlfsBwAvlbl(cell, dlSf, allocInfo)
RgSchCellCb        *cell;
RgSchDlSf          *dlSf;
RgSchDlRbAlloc     *allocInfo;
#endif
{
   U8 tbs;
   U8 noLyrs;
   U8 ignoredDfctRbg = FALSE;

   TRC2(rgSCHCmnNonDlfsBwAvlbl);
   if (dlSf->bw <= dlSf->bwAlloced)
   {
      RLOG_ARG3(L_DEBUG,DBG_CELLID,cell->cellId, "(%d:%d)FAILED CRNTI:%d",
         dlSf->bw, dlSf->bwAlloced,allocInfo->rnti);
      RETVALUE(FALSE);
   }
   if (allocInfo->raType == RG_SCH_CMN_RA_TYPE0)
   {
       /* Fix for ccpu00123919 : Number of RBs in case of RETX should be same as 
        * that of initial transmission. */
       if(allocInfo->tbInfo[0].tbCb->txCntr)
       {
          /* If RB assignment is being done for RETX. Then if reqRbs are 
           * a multiple of rbgSize then ignore lstRbgDfct. If reqRbs is 
           * not a multiple of rbgSize then check if lsgRbgDfct exists */
          if (allocInfo->rbsReq % cell->rbgSize == 0)
          {
             if (dlSf->lstRbgDfct)
             {
                /* In this scenario we are wasting the last RBG for this dlSf */
                
                dlSf->type0End--;
                dlSf->bwAlloced += (cell->rbgSize - dlSf->lstRbgDfct);
                /* Fix: MUE_PERTTI_DL */
                dlSf->lstRbgDfct = 0;
                ignoredDfctRbg = TRUE;
                
             }
          }
          else
          {
             if (dlSf->lstRbgDfct)
             {
                /* Check if type0 allocation can cater to this RETX requirement */
                if ((allocInfo->rbsReq % cell->rbgSize) != (cell->rbgSize - dlSf->lstRbgDfct))
                {
                   RETVALUE(FALSE);
                }
             }
             else
             {
                /* cannot allocate same number of required RBs */
                RETVALUE(FALSE);           
             }
          }
       }

       /* Condition is modified approprialtely to find
        * if rbsReq is less than available RBS*/
      if(allocInfo->rbsReq <= (((dlSf->type0End - dlSf->type2End + 1)*\
               cell->rbgSize) - dlSf->lstRbgDfct))
      {
         RETVALUE(TRUE);
      }
      /* ccpu00132358:MOD- Removing "ifndef LTE_TDD" for unblocking the RB 
       * allocation in TDD when requested RBs are more than available RBs*/
      else
      {
          /* MS_WORKAROUND for ccpu00122022 */
         if (dlSf->bw <= dlSf->bwAlloced + cell->rbgSize)
         {
            /* ccpu00132358- Re-assigning the values which were updated above 
             * if it is RETX and Last  RBG available*/
            if(ignoredDfctRbg == TRUE)
            {
               dlSf->type0End++;
               dlSf->bwAlloced -= (cell->rbgSize - dlSf->lstRbgDfct);
               dlSf->lstRbgDfct = 1;
            }


            RETVALUE(FALSE);
         }
         /* Fix: Number of RBs in case of RETX should be same as 
          * that of initial transmission. */
         if(allocInfo->tbInfo[0].tbCb->txCntr == 0)
         {
            /* Setting the remaining RBs for the requested UE*/
            allocInfo->rbsReq = (((dlSf->type0End - dlSf->type2End + 1)*\
                        cell->rbgSize) - dlSf->lstRbgDfct);
            RG_SCH_CMN_DL_MCS_TO_TBS(allocInfo->tbInfo[0].imcs, tbs);
            noLyrs = allocInfo->tbInfo[0].noLyr;
            allocInfo->tbInfo[0].bytesReq = rgTbSzTbl[noLyrs-1][tbs][allocInfo->rbsReq - 1]/8;
            /* DwPts Scheduling Changes Start */
#if LTE_TDD
            if (dlSf->sfType == RG_SCH_SPL_SF_DATA)
            {   
               allocInfo->tbInfo[0].bytesReq = 
                        rgTbSzTbl[noLyrs-1][tbs][RGSCH_MAX(allocInfo->rbsReq*3/4,1) - 1]/8; 
            }
#endif            
            /* DwPts Scheduling Changes End */
         }
         else
         {
          /* ccpu00132358- Re-assigning the values which were updated above 
             * if it is RETX and Last  RBG available*/
            if(ignoredDfctRbg == TRUE)
            {
               dlSf->type0End++;
               dlSf->bwAlloced -= (cell->rbgSize - dlSf->lstRbgDfct);
               dlSf->lstRbgDfct = 1;
            }

            RLOG_ARG1(L_ERROR,DBG_CELLID,cell->cellId, "FAILED for CRNTI:%d",
                  allocInfo->rnti);
            RETVALUE(FALSE);
         }
         RETVALUE(TRUE);
      }
   }
   else if (allocInfo->raType == RG_SCH_CMN_RA_TYPE2)
   {
      if (allocInfo->rbsReq <= (dlSf->bw - dlSf->bwAlloced))
      {
         RETVALUE(TRUE);
      }
      /* ccpu00132358:MOD- Removing "ifndef LTE_TDD" for unblocking the RB 
       * allocation in TDD when requested RBs are more than available RBs*/
      else
      {
         /* Fix: Number of RBs in case of RETX should be same as 
          * that of initial transmission. */
         if(allocInfo->tbInfo[0].tbCb->txCntr == 0)
         {
            /* set the remaining RBs for the requested UE */
            allocInfo->rbsReq = dlSf->bw - dlSf->bwAlloced;
            RG_SCH_CMN_DL_MCS_TO_TBS(allocInfo->tbInfo[0].imcs, tbs);
            noLyrs = allocInfo->tbInfo[0].noLyr;
            allocInfo->tbInfo[0].bytesReq = rgTbSzTbl[noLyrs-1][tbs][allocInfo->rbsReq - 1]/8;
            /* DwPts Scheduling Changes Start */
#ifdef LTE_TDD
            if (dlSf->sfType == RG_SCH_SPL_SF_DATA)
            {   
               allocInfo->tbInfo[0].bytesReq = 
                        rgTbSzTbl[noLyrs-1][tbs][RGSCH_MAX(allocInfo->rbsReq*3/4,1) - 1]/8; 
            }
#endif            
            /* DwPts Scheduling Changes End */
         }
         else
         {
            RLOG_ARG1(L_ERROR,DBG_CELLID,cell->cellId,"FAILED for CRNTI:%d",allocInfo->rnti);
            RETVALUE(FALSE);
         }
         /* Fix: Number of RBs in case of RETX should be same as 
          * that of initial transmission. */
         RETVALUE(TRUE);
      }
   }
   RLOG_ARG1(L_ERROR,DBG_CELLID,cell->cellId,"FAILED for CRNTI:%d",allocInfo->rnti);
   RETVALUE(FALSE);
}
/* LTE_ADV_FLAG_REMOVED_START */
#ifndef LTE_TDD
/**
 * @brief To update non-DLFS alloc'n parameters after TYPE2 Allocation.
 *
 * @details
 *
 *     Function : rgSCHCmnNonDlfsSFRCmnChannelUpdTyp2Alloc
 *
 *     Processing Steps:
 *
 *  @param[in]  RgSchCellCb     *cell
 *  @param[in]  RgSchDlSf       *dlSf
 *  @param[in]  U8              rbStrt
 *  @param[in]  U8              numRb
 *
 *  @return Void
 **/
#ifdef ANSI
PUBLIC Void rgSCHCmnNonDlfsSFRCmnChannelUpdTyp2Alloc
(
RgSchCellCb        *cell,
RgSchDlSf          *dlSf,
U8                 rbStrt,
U8                 numRb
)
#else
PUBLIC Void rgSCHCmnNonDlfsSFRCmnChannelUpdTyp2Alloc(cell, dlSf, rbStrt, numRb)
RgSchCellCb        *cell;
RgSchDlSf          *dlSf;
U8                 rbStrt;
U8                 numRb;
#endif
{ 
   CmLListCp   *l;
   CmLList     *n;
   RgSchSFRPoolInfo  *sfrPool;
   TRC2(rgSCHCmnNonDlfsSFRCmnChannelUpdTyp2Alloc);
   
   l = &dlSf->sfrTotalPoolInfo.ccPool;
     
   dlSf->type2End = RGSCH_CEIL((rbStrt+numRb), cell->rbgSize);
   dlSf->bwAlloced += numRb;
   dlSf->type2Start += numRb;
   n = cmLListFirst(l);
        
   while(n->node)
   {
      sfrPool = (RgSchSFRPoolInfo*)(n->node);
        n = cmLListNext(l);
    
    /* If the pool contains some RBs allocated in this allocation, e.g: Pool is [30.50]. Pool->type2Start is 40 , dlSf->type2Start is 45. then update the variables in pool   */
        if((sfrPool->poolendRB >= dlSf->type2Start) && (sfrPool->type2Start < dlSf->type2Start))
        {
             sfrPool->type2End   =  dlSf->type2End;
             sfrPool->bwAlloced  =  dlSf->type2Start - sfrPool->poolstartRB; 
             sfrPool->type2Start =  dlSf->type2Start;
        }        
        else 
        { 
           /* If the pool contains all RBs allocated in this allocation*/
             if(dlSf->type2Start > sfrPool->poolendRB)
             {        
               sfrPool->type2End   =  sfrPool->type0End + 1;
               sfrPool->bwAlloced  =  sfrPool->bw; 
               sfrPool->type2Start =  sfrPool->poolendRB + 1;         
             }  
        }
      if (!n)
      {   
         if (l != &dlSf->sfrTotalPoolInfo.cePool)
         {
            l = &dlSf->sfrTotalPoolInfo.cePool;   
            n = cmLListFirst(l);
         }
         else
            RETVOID;
      }
   }
   RETVOID;
}

/**
 * @brief To update non-DLFS alloc'n parameters after TYPE2 Allocation.
 *
 * @details
 *
 *     Function : rgSCHCmnNonDlfsUpdDSFRTyp2Alloc
 *
 *     Processing Steps:
 *
 *  @param[in]  RgSchCellCb     *cell
 *  @param[in]  RgSchDlSf       *dlSf
 *  @param[in]  U8              rbStrt
 *  @param[in]  U8              numRb
 *
 *  @return Void
 **/
#ifdef ANSI
PRIVATE S16 rgSCHCmnNonDlfsUpdDSFRTyp2Alloc
(
RgSchCellCb        *cell,
RgSchUeCb          *ue,
RgSchDlSf          *dlSf,
U8                 rbStrt,
U8                 numRb
)
#else
PRIVATE S16 rgSCHCmnNonDlfsUpdDSFRTyp2Alloc(cell, ue, dlSf, rbStrt, numRb)
RgSchCellCb        *cell;
RgSchUeCb          *ue;
RgSchDlSf          *dlSf;
U8                 rbStrt;
U8                 numRb;
#endif
{
   CmLListCp   *l;
   CmLList     *n;
   RgSchSFRPoolInfo  *sfrCCPool1 = NULL;
   RgSchSFRPoolInfo  *sfrCCPool2 = NULL;
   S16 ret = RFAILED;

   TRC2(rgSCHCmnNonDlfsUpdDSFRTyp2Alloc);
   /* Move the type2End pivot forward */
   
   
   l = &dlSf->sfrTotalPoolInfo.ccPool;
   n = cmLListFirst(l);
   while(n)
   {
      sfrCCPool1 = (RgSchSFRPoolInfo*)(n->node);
      /* KWork fix */
      if (sfrCCPool1 ==  NULLP)
            {
               RLOG_ARG1(L_ERROR,DBG_CELLID,cell->cellId,  "rgSCHCmnNonDlfsUpdDSFRTyp2Alloc():"
                        "sfrCCPool1 is NULL for CRNTI:%d",ue->ueId);
               RETVALUE(RFAILED);
            }
      n = cmLListNext(l);
      if(n)
      {
          sfrCCPool2 = (RgSchSFRPoolInfo*)(n->node);
          n = cmLListNext(l);
      }
      if((sfrCCPool1) && (sfrCCPool2))
      { 
          /* Based on RNTP info, the CC user is assigned high power per subframe basis */
          if(((dlSf->type2Start >= sfrCCPool1->pwrHiCCRange.startRb) &&
              (dlSf->type2Start + numRb < sfrCCPool1->pwrHiCCRange.endRb)) || 
             ((dlSf->type2Start >= sfrCCPool2->pwrHiCCRange.startRb) &&
              (dlSf->type2Start + numRb < sfrCCPool2->pwrHiCCRange.endRb)))
          {
               ue->lteAdvUeCb.isCCUePHigh = TRUE;

               /* Calling rgSCHCmnBuildRntpInfo function to update RNTP BitMap */
               ret = rgSCHCmnBuildRntpInfo(cell, dlSf->rntpInfo.val, dlSf->type2Start, numRb, dlSf->bw);
               if (ret != ROK)
               {
                    RLOG_ARG1(L_ERROR,DBG_CELLID,cell->cellId, "rgSCHCmnNonDlfsUpdDSFRTyp2Alloc():"
                      "rgSCHCmnBuildRntpInfo() function returned RFAILED for CRNTI:%d",ue->ueId);
                    RETVALUE(RFAILED);
               }
           }
      }
      else
      {
         if((dlSf->type2Start >= sfrCCPool1->pwrHiCCRange.startRb) &&
               (dlSf->type2Start + numRb < sfrCCPool1->pwrHiCCRange.endRb))
         {
            ue->lteAdvUeCb.isCCUePHigh = TRUE;

            /* Calling rgSCHCmnBuildRntpInfo function to update RNTP BitMap */
            ret = rgSCHCmnBuildRntpInfo(cell, dlSf->rntpInfo.val, dlSf->type2Start, numRb, dlSf->bw);
            if (ret != ROK)
            {
               RLOG_ARG1(L_ERROR,DBG_CELLID,cell->cellId,   "rgSCHCmnNonDlfsUpdDSFRTyp2Alloc():" 
                        "rgSCHCmnBuildRntpInfo() function returned RFAILED CRNTI:%d",ue->ueId);
               RETVALUE(RFAILED);
            }
         }
      }
   }
   dlSf->type2End = RGSCH_CEIL((rbStrt+numRb), cell->rbgSize);
#ifndef LTEMAC_SPS
   dlSf->bwAlloced += numRb;
   /*MS_FIX for ccpu00123918*/
   dlSf->type2Start += numRb;
#endif
   RETVALUE(ROK);
}
#endif /* end of ifndef LTE_TDD*/
/* LTE_ADV_FLAG_REMOVED_END */
/**
 * @brief To update non-DLFS alloc'n parameters after TYPE2 Allocation.
 *
 * @details
 *
 *     Function : rgSCHCmnNonDlfsUpdTyp2Alloc
 *
 *     Processing Steps:
 *
 *  @param[in]  RgSchCellCb     *cell
 *  @param[in]  RgSchDlSf       *dlSf
 *  @param[in]  U8              rbStrt
 *  @param[in]  U8              numRb
 *
 *  @return Void
 **/
#ifdef ANSI
PRIVATE Void rgSCHCmnNonDlfsUpdTyp2Alloc
(
RgSchCellCb        *cell,
RgSchDlSf          *dlSf,
U8                 rbStrt,
U8                 numRb
)
#else
PRIVATE Void rgSCHCmnNonDlfsUpdTyp2Alloc(cell, dlSf, rbStrt, numRb)
RgSchCellCb        *cell;
RgSchDlSf          *dlSf;
U8                 rbStrt;
U8                 numRb;
#endif
{
   TRC2(rgSCHCmnNonDlfsUpdTyp2Alloc);
   /* Move the type2End pivot forward */
   dlSf->type2End = RGSCH_CEIL((rbStrt+numRb), cell->rbgSize);
//#ifndef LTEMAC_SPS
   dlSf->bwAlloced += numRb;
   /*Fix for ccpu00123918*/
   dlSf->type2Start += numRb;
//#endif
   RETVOID;
}

/**
 * @brief To do DL allocation using TYPE0 RA.
 *
 * @details
 *
 *     Function : rgSCHCmnNonDlfsType0Alloc
 *
 *     Processing Steps:
 *      - Perform TYPE0 allocation using the RBGs between
 *        type0End and type2End.
 *      - Build the allocation mask as per RBG positioning.
 *      - Update the allocation parameters.
 *
 *  @param[in]  RgSchCellCb     *cell
 *  @param[in]  RgSchDlSf       *dlSf
 *  @param[in]  RgSchDlRbAlloc  *allocInfo
 *
 *  @return Void
 **/
#ifdef ANSI
PRIVATE Void rgSCHCmnNonDlfsType0Alloc
(
RgSchCellCb        *cell,
RgSchDlSf          *dlSf,
RgSchDlRbAlloc     *allocInfo,
RgSchCmnDlUe       *dlUe
)
#else
PRIVATE Void rgSCHCmnNonDlfsType0Alloc(cell, dlSf, allocInfo, dlUe)
RgSchCellCb        *cell;
RgSchDlSf          *dlSf;
RgSchDlRbAlloc     *allocInfo;
RgSchCmnDlUe       *dlUe;
#endif
{
   U32 dlAllocMsk = 0;
   U8  rbgFiller = dlSf->lstRbgDfct;
   U8  noRbgs = RGSCH_CEIL((allocInfo->rbsReq + rbgFiller), cell->rbgSize);
   //U8  noRbgs = (allocInfo->rbsReq + rbgFiller)/ cell->rbgSize;
   U8  noRbs;
   U8  noLyr;
   U8  iTbs;
   U32          tb1BytesAlloc;
   U32          tb2BytesAlloc;

   TRC2(rgSCHCmnNonDlfsType0Alloc);
   //if(noRbgs == 0) noRbgs = 1; /* Not required as ceilling is used above*/

   /* Fix for ccpu00123919*/
   noRbs = (noRbgs * cell->rbgSize) - rbgFiller;
   if (dlSf->bwAlloced + noRbs > dlSf->bw)
   {
      if (--noRbgs == 0)
      {
         RETVOID;
      }
      noRbs = (noRbgs * cell->rbgSize) - rbgFiller;
   }

   /* Fix for ccpu00138701: Ceilling is using to derive num of RBGs, Therefore, 
   *  after this operation,checking Max TB size and Max RBs are not crossed
   * if it is crossed then decrement num of RBGs. */
   //if((noRbs + rbgFiller) % cell->rbgSize)
   if((noRbs > allocInfo->rbsReq) &&
         (allocInfo->rbsReq + rbgFiller) % cell->rbgSize)
   {/* considering ue category limitation
     * due to ceiling */
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
                  >= dlUe->maxTbBits/8) ||
               (tb1BytesAlloc >= dlUe->maxTbSz/8) ||
               (tb2BytesAlloc >= dlUe->maxTbSz/8) ||
               (noRbs >= dlUe->maxRb))
         {
            if (--noRbgs == 0)
            {
               RETVOID;
            }
            noRbs = (noRbgs * cell->rbgSize) - rbgFiller;
         }
      }
      else
      {
         if ((noRbs >= dlUe->maxRb) ||
               (tb1BytesAlloc >= dlUe->maxTbSz/8))
         {
            if (--noRbgs == 0)
            {
               RETVOID;
            }
            noRbs = (noRbgs * cell->rbgSize) - rbgFiller;
         }
      }
   }
   /* type0End would have been initially (during subfrm Init) at the bit position
    * (cell->noOfRbgs - 1), 0 being the most significant.
    * Getting DlAllocMsk for noRbgs and at the appropriate position */
   dlAllocMsk |= (((1 << noRbgs) - 1) << (31 - dlSf->type0End));
   /* Move backwards the type0End pivot */
   dlSf->type0End -= noRbgs;
   /*Fix for ccpu00123919*/
   /*noRbs = (noRbgs * cell->rbgSize) - rbgFiller;*/
   /* Update the bwAlloced field accordingly */
//#ifndef LTEMAC_SPS    /* ccpu00129474*/
   dlSf->bwAlloced += noRbs;
//#endif
   /* Update Type0 Alloc Info */
   allocInfo->allocInfo.raType0.numDlAlloc = noRbgs;
   allocInfo->allocInfo.raType0.dlAllocBitMask |= dlAllocMsk;
   allocInfo->rbsAlloc = noRbs;

   /* Update Tb info for each scheduled TB */
   iTbs = allocInfo->tbInfo[0].iTbs;
   noLyr = allocInfo->tbInfo[0].noLyr;
   /* Fix for ccpu00123919: For a RETX TB the iTbs is irrelevant.
    * RETX TB Size is same as Init TX TB Size */
   if (allocInfo->tbInfo[0].tbCb->txCntr)
   {
      allocInfo->tbInfo[0].bytesAlloc =
         allocInfo->tbInfo[0].bytesReq;
   }
   else
   {
      allocInfo->tbInfo[0].bytesAlloc =
         rgTbSzTbl[noLyr - 1][iTbs][noRbs - 1]/8;
      /* DwPts Scheduling Changes Start */
#ifdef LTE_TDD
      if (dlSf->sfType == RG_SCH_SPL_SF_DATA)
      {
         allocInfo->tbInfo[0].bytesAlloc =
            rgTbSzTbl[noLyr - 1][iTbs][RGSCH_MAX(noRbs*3/4,1) - 1]/8;
      }
#endif      
      /* DwPts Scheduling Changes End */
   }

   if (allocInfo->tbInfo[1].schdlngForTb)
   {
      iTbs = allocInfo->tbInfo[1].iTbs;
      noLyr = allocInfo->tbInfo[1].noLyr;
      /* Fix for ccpu00123919: For a RETX TB the iTbs is irrelevant
       * RETX TB Size is same as Init TX TB Size */
      if (allocInfo->tbInfo[1].tbCb->txCntr)
      {
         allocInfo->tbInfo[1].bytesAlloc =
            allocInfo->tbInfo[1].bytesReq;
      }
      else
      {
         allocInfo->tbInfo[1].bytesAlloc =
            rgTbSzTbl[noLyr - 1][iTbs][noRbs - 1]/8;;
         /* DwPts Scheduling Changes Start */
#ifdef LTE_TDD
         if (dlSf->sfType == RG_SCH_SPL_SF_DATA)
         {
            allocInfo->tbInfo[1].bytesAlloc =
               rgTbSzTbl[noLyr - 1][iTbs][RGSCH_MAX(noRbs*3/4,1) - 1]/8;
         }
#endif      
         /* DwPts Scheduling Changes End */
      }
   }

   /* The last RBG which can be smaller than the RBG size is consedered
    * only for the first time allocation of TYPE0 UE */
   dlSf->lstRbgDfct = 0;
   RETVOID;
}
#ifndef LTE_TDD

#if 0 /* SFR_FIX: removing PBCH Adjustment Logic */
/*rg002.301 ccpu00119901 ADD added this function to find num of ovrlapping PBCH rb*/
/**
 * @brief Function to find out how many of the requested RBs are  falling in the center 6 RBs of the downlink bandwidth.
 * @details
 *
 *     Function : rgSCHCmnSFRFindNumPbchOvrlapRbs
 *
 *     Processing Steps:
 *      - Calculates number of overlapping pbch rbs in thal pool
 *
 *  @param[in]      RgSchCellCb     *cell
 *  @param[in]      RgSchSFRPoolInfo      *poolInfo,
 *  @param[in,out]  RgSchDlRbAlloc  *allocInfo
 *  @param[out]      U8*            numOvrlapgPbchRb
 *  @return  void
 **/
#ifdef ANSI
PRIVATE Void rgSCHCmnSFRFindNumPbchOvrlapRbs
(
RgSchCellCb           *cell,
RgSchSFRPoolInfo      *poolInfo,
RgSchDlRbAlloc        *allocInfo,
U8                    *numOvrlapgPbchRb
)
#else
PRIVATE Void rgSCHCmnSFRFindNumPbchOvrlapRbs(cell,poolInfo,allocInfo,numOvrlapgPbchRb)
(
RgSchCellCb           *cell;
RgSchSFRPoolInfo      *poolInfo;
RgSchDlRbAlloc        *allocInfo;
U8                    *numOvrlapgPbchRb
)
#endif
{

   U16 poolAllocPosition = poolInfo->poolstartRB + poolInfo->bwAlloced;
    *numOvrlapgPbchRb = 0;
    TRC2(rgSCHCmnSFRFindNumPbchOvrlapRbs)
   /*Find if we have already crossed the start boundary for PBCH 6 RBs,
    * if yes then lets find the number of RBs which are getting overlapped
    * with this allocation.*/
   if(poolAllocPosition <= (cell->pbchRbStart))
   {
      /*We have not crossed the start boundary of PBCH RBs. Now we need
       * to know that if take this allocation then how much PBCH RBs
       * are overlapping with this allocation.*/
      /* Find out the overlapping RBs in the centre 6 RBs */
       if((poolAllocPosition + allocInfo->rbsReq) > cell->pbchRbStart)
       {
           *numOvrlapgPbchRb = (poolAllocPosition + allocInfo->rbsReq) - (cell->pbchRbStart);
           if(*numOvrlapgPbchRb > 6)
                *numOvrlapgPbchRb = 6;
       }
   }
   else if ((poolAllocPosition > (cell->pbchRbStart)) &&
         (poolAllocPosition < (cell->pbchRbEnd)))
   {
      /*We have already crossed the start boundary of PBCH RBs.We need to
       * find that if we take this allocation then how much of the RBs for
       * this allocation will overlap with PBCH RBs.*/
      /* Find out the overlapping RBs in the centre 6 RBs */
      if(poolAllocPosition + allocInfo->rbsReq < (cell->pbchRbEnd))
      {
         /*If we take this allocation then also we are not crossing the
          * end boundary of PBCH 6 RBs.*/
         *numOvrlapgPbchRb = allocInfo->rbsReq;
      }
      else
      {
         /*If we take this allocation then we are crossing the
          * end boundary of PBCH 6 RBs.*/
         *numOvrlapgPbchRb = (cell->pbchRbEnd) - poolAllocPosition + 1;
      }
   }
    RETVOID;

}


/* Adjust the Imcs and bytes allocated also with respect to the adjusted
   RBs - Here we will find out the Imcs by identifying first Highest
   number of bits compared to the original bytes allocated.  */
/**
 * @brief Adjust IMCS according to tbSize and ITBS
 *
 * @details
 *
 *     Function : rgSCHCmnNonDlfsSFRPbchTbImcsAdj
 *
 *     Processing Steps:
 *      - Adjust Imcs according to tbSize and ITBS.
 *
 *  @param[in,out]  RgSchDlRbAlloc  *allocInfo
 *  @param[in]      U8              *idx
 *  @return  void
 **/
#ifdef ANSI
PRIVATE Void rgSCHCmnNonDlfsSFRPbchTbImcsAdj
(
RgSchCellCb      *cell,
RgSchSFRPoolInfo *poolInfo,
RgSchDlRbAlloc   *allocInfo,
U8               idx,
U8               rbsReq
)
#else
PRIVATE Void rgSCHCmnNonDlfsSFRPbchTbImcsAdj(cell,poolInfo,allocInfo, idx, rbsReq)
RgSchCellCb      *cell;
RgSchSFRPoolInfo *poolInfo;
RgSchDlRbAlloc   *allocInfo;
U8               idx;
U8               rbsReq;
#endif
{
   U8             noLyrs = 0;
   U8             tbs = 0;
   U32            origBytesReq;
   U8             noRbgs = 0;
   U8             noRbs = 0;
   RgSchDlSf     *dlSf = allocInfo->dlSf;
   U8 rgbFiller = 0;

   /*Find if last RBG has few RBS deficit*/
   if ((poolInfo->poolstartRB + poolInfo->bw == dlSf->bw) &&
         (poolInfo->type0End == dlSf->bw/4))
   {
         rgbFiller = dlSf->lstRbgDfct;
   }

   /*Find TBS from MCS*/
   RG_SCH_CMN_DL_MCS_TO_TBS(allocInfo->tbInfo[idx].imcs, tbs);
   noLyrs = allocInfo->tbInfo[idx].noLyr;

   /*If  RAType0,  then find RBs in terms of RBGs*/
   if((allocInfo->raType == RG_SCH_CMN_RA_TYPE0))
   {
      noRbgs = RGSCH_CEIL((allocInfo->rbsReq + rgbFiller), cell->rbgSize);
      noRbs = (noRbgs * cell->rbgSize) - rgbFiller;
   }
   /*Find noRBsRequested from AllocInfo*/
   else
   {
       noRbs = allocInfo->rbsReq;
   }

   /* This line will help in case if tbs is zero and reduction in MCS is not possible */
   if (allocInfo->rbsReq == 0 )
   {
      RETVOID;
   }
   origBytesReq = rgTbSzTbl[noLyrs - 1][tbs][rbsReq - 1]/8;

   /*rg002.301 ccpu00119901 find out the ITbs & Imcs by identifying first Highest
    number of bits compared to the original bytes allocated.*/
   if(tbs > 0)
   {
      if(((rgTbSzTbl[noLyrs - 1][0][noRbs - 1])/8) < origBytesReq)
      {
          while(((rgTbSzTbl[noLyrs - 1][tbs][noRbs - 1])/8) > origBytesReq)
          {
              tbs--;
          }
      }
      else
      {
          tbs = 0;
      }
      allocInfo->tbInfo[idx].bytesReq = rgTbSzTbl[noLyrs - 1][tbs][noRbs - 1]/8;
      allocInfo->tbInfo[idx].iTbs = tbs;
      /* Ashish: TODO: Changed from RG_SCH_CMN_TBS_TO_MCS */ 
      RG_SCH_CMN_DL_TBS_TO_MCS(tbs,allocInfo->tbInfo[idx].imcs);
   }

   RETVOID;
}
/*rg002.301 ccpu00119901 MOD modified handling of PBCH adjustment*/
/**
 * @brief Performs RB allocation adjustment if the requested RBs are
 *        falling in the center 6 RBs of the downlink bandwidth.
 * @details
 *
 *     Function : rgSCHCmnNonDlfsSFRPbchRbAllocAdj
 *
 *     Processing Steps:
 *      - Allocate consecutively available RBs.
 *
 *  @param[in]      RgSchCellCb     *cell
 *  @param[in,out]  RgSchDlRbAlloc  *allocInfo
 *  @param[in]      U8               pbchSsRsSym
 *  @return  void
 **/
#ifdef ANSI
PRIVATE Void rgSCHCmnNonDlfsSFRPbchRbAllocAdj
(
RgSchCellCb      *cell,
RgSchSFRPoolInfo *poolInfo,
RgSchDlRbAlloc   *allocInfo,
U8               pbchSsRsSym
)
#else
PRIVATE Void rgSCHCmnNonDlfsSFRPbchRbAllocAdj(cell, poolInfo, allocInfo,pbchSsRsSym)
RgSchCellCb      *cell;
RgSchSFRPoolInfo *poolInfo;
RgSchDlRbAlloc   *allocInfo;
U8               pbchSsRsSym;
#endif
{
   RgSchDlSf     *dlSf = allocInfo->dlSf;
   U8             numOvrlapgPbchRb = 0;
   U8             numOvrlapgAdtlPbchRb = 0;
   U8             totSym;
   U8             addtlRbsReq = 0;
   U8             moreAddtlRbsReq = 0;
   U8             addtlRbsAdd = 0;
   U8             moreAddtlRbsAdd = 0;
   U8             tbs;
   U8             origRbsReq = 0;
   U32            bytesReq;
   U8             noLyr;
   U8             divResult;


   TRC2(rgSCHCmnNonDlfsSFRPbchRbAllocAdj);


   origRbsReq = allocInfo->rbsReq;
   rgSCHCmnSFRFindNumPbchOvrlapRbs(cell, poolInfo, allocInfo,&numOvrlapgPbchRb);

  totSym =  (cell->isCpDlExtend) ? RGSCH_TOT_NUM_SYM_EXTCP : RGSCH_TOT_NUM_SYM_NORCP;

   /* Additional RBs are allocated by considering the loss due to
      the reserved symbols for CFICH, PBCH, PSS, SSS and cell specific RS */

   divResult = (numOvrlapgPbchRb * pbchSsRsSym)/totSym;
   if((numOvrlapgPbchRb * pbchSsRsSym) % totSym)
   {
      divResult++;
   }
   addtlRbsReq = divResult;

   RG_SCH_CMN_SFR_UPD_RBS_TO_ADD_IN_POOL(cell, dlSf, poolInfo, allocInfo, addtlRbsReq, addtlRbsAdd)

   /*Now RBs requires is original requested RBs + these additional RBs to make
    * up for PSS/SSS/BCCH.*/
   allocInfo->rbsReq = allocInfo->rbsReq + addtlRbsAdd;

   /*Check if with these additional RBs we have taken up, these are also falling
    * under PBCH RBs range, if yes then we would need to account for
    * PSS/BSS/BCCH for these additional RBs too.*/
   if(addtlRbsAdd && ((poolInfo->poolstartRB + poolInfo->bwAlloced + allocInfo->rbsReq - addtlRbsAdd) < (cell->pbchRbEnd)))
   {
      if((poolInfo->poolstartRB + poolInfo->bwAlloced + allocInfo->rbsReq) <= (cell->pbchRbEnd))
      {
      /*With additional RBs taken into account, we are not crossing the
       * PBCH RB end boundary.Thus here we need to account just for
       * overlapping PBCH RBs for these additonal RBs.*/
          divResult = (addtlRbsAdd * pbchSsRsSym)/totSym;
          if((addtlRbsAdd * pbchSsRsSym) % totSym)
          {
            divResult++;
          }

          moreAddtlRbsReq = divResult;

          RG_SCH_CMN_SFR_UPD_RBS_TO_ADD_IN_POOL(cell, dlSf, poolInfo, allocInfo, moreAddtlRbsReq, moreAddtlRbsAdd)

          allocInfo->rbsReq = allocInfo->rbsReq + moreAddtlRbsAdd;
      }
      else
      {

         /*Here we have crossed the PBCH RB end boundary, thus we need to take
          * into account the overlapping RBs for additional RBs which will be
          * subset of addtlRbs.*/
          numOvrlapgAdtlPbchRb = (cell->pbchRbEnd) - ((dlSf->bwAlloced + allocInfo->rbsReq) -  addtlRbsAdd);

          divResult = (numOvrlapgAdtlPbchRb * pbchSsRsSym)/totSym;
          if((numOvrlapgAdtlPbchRb * pbchSsRsSym) % totSym)
          {
             divResult++;
          }

          moreAddtlRbsReq =  divResult;

          RG_SCH_CMN_SFR_UPD_RBS_TO_ADD_IN_POOL(cell, dlSf, poolInfo, allocInfo, moreAddtlRbsReq, moreAddtlRbsAdd)

          allocInfo->rbsReq = allocInfo->rbsReq + moreAddtlRbsAdd;
      }
   }
   /* rg007.201 - Changes for MIMO feature addition */
   /* rg008.201 - Removed dependency on MIMO compile-time flag */
   RG_SCH_CMN_DL_MCS_TO_TBS(allocInfo->tbInfo[0].imcs, tbs);
   if(tbs == 6)
   {
      /* This case might be for Imcs value 6 and NPrb = 1 case  - Not
         Adjusting either RBs or Imcs or Bytes Allocated */
      allocInfo->rbsReq = allocInfo->rbsReq - addtlRbsAdd - moreAddtlRbsAdd;
   }
   else if(tbs && ((0 == addtlRbsAdd) && (moreAddtlRbsAdd == 0)))
   {
       /*In case of a situation where we the entire bandwidth is already occupied
        * and we dont have room to add additional Rbs then in order to decrease the
        * code rate we reduce the tbsize such that we reduce the present calculated
        * tbsize by number of bytes that would be occupied by PBCH/PSS/SSS in overlapping
        * rbs and find the nearest tbsize which would be less than this deduced value*/

      rgSCHCmnSFRFindNumPbchOvrlapRbs(cell,poolInfo,allocInfo,&numOvrlapgPbchRb);

      noLyr = allocInfo->tbInfo[0].noLyr;

      bytesReq = rgTbSzTbl[noLyr - 1][tbs][allocInfo->rbsReq - 1]/8;

      rgSCHCmnNonDlfsPbchTbSizeAdj(allocInfo,numOvrlapgPbchRb,pbchSsRsSym,0,bytesReq);

      if(allocInfo->tbInfo[1].schdlngForTb == TRUE)
      {
          noLyr = allocInfo->tbInfo[1].noLyr;
          bytesReq = rgTbSzTbl[noLyr - 1][tbs][allocInfo->rbsReq - 1]/8;
          rgSCHCmnNonDlfsPbchTbSizeAdj(allocInfo,numOvrlapgPbchRb,pbchSsRsSym,1,bytesReq);
      }

   }
   else if(tbs && ((addtlRbsAdd != addtlRbsReq) ||
          (addtlRbsAdd && (moreAddtlRbsReq != moreAddtlRbsAdd))))
   {
       /*In case of a situation where we were not able to add required number of
        * additional RBs then we adjust the Imcs based on original RBs requested.
        * Doing this would comensate for the few extra Rbs we have added but inorder
        * to comensate for number of RBS we couldnt add we again do the TBSize adjustment*/

      rgSCHCmnNonDlfsSFRPbchTbImcsAdj(cell, poolInfo, allocInfo, 0 , origRbsReq);

      if(allocInfo->tbInfo[1].schdlngForTb == TRUE)
      {
          rgSCHCmnNonDlfsSFRPbchTbImcsAdj(cell, poolInfo, allocInfo, 1 , origRbsReq);
      }

      rgSCHCmnFindNumPbchOvrlapRbs(cell,dlSf,allocInfo,&numOvrlapgPbchRb);
      numOvrlapgPbchRb = numOvrlapgPbchRb - (addtlRbsAdd + moreAddtlRbsAdd);

      rgSCHCmnNonDlfsPbchTbSizeAdj(allocInfo,numOvrlapgPbchRb,pbchSsRsSym,0,allocInfo->tbInfo[0].bytesReq);

      if(allocInfo->tbInfo[1].schdlngForTb == TRUE)
      {
          rgSCHCmnNonDlfsPbchTbSizeAdj(allocInfo,numOvrlapgPbchRb,pbchSsRsSym,1,allocInfo->tbInfo[1].bytesReq);
      }
   }
   else
   {
       /*We hit this code when we were able to add the required additional RBS
        * hence we should adjust the IMcs based on orignals RBs requested*/
      rgSCHCmnNonDlfsSFRPbchTbImcsAdj(cell, poolInfo, allocInfo, 0 , origRbsReq);

      if(allocInfo->tbInfo[1].schdlngForTb == TRUE)
      {
          rgSCHCmnNonDlfsSFRPbchTbImcsAdj(cell, poolInfo, allocInfo, 1 , origRbsReq);
      }
   }

   /* rg007.201 - Returning VOID */
   RETVOID;
} /* end of rgSCHCmnNonDlfsPbchRbAllocAdj */
#endif /* SFR_FIX */
/**
 * @brief To prepare RNTP value from the PRB allocation (P-High -> 1 and P-Low -> 0)
 *
 * @details
 *
 *     Function : rgSCHCmnBuildRntpInfo
 *
 *     Processing Steps:
 *
 *  @param[in]  U8                 *rntpPtr
 *  @param[in]  U8                 startRb
 *  @param[in]  U8                 numRb
 *
 *  @return Void
 **/
#ifdef ANSI
PRIVATE S16 rgSCHCmnBuildRntpInfo
(
RgSchCellCb        *cell,
U8                 *rntpPtr,
U8		              startRb,
U8                  nmbRb,
U16                 bw
)
#else
PRIVATE S16 rgSCHCmnBuildRntpInfo(cell, rntpPtr, startRb, nmbRb, bw)
RgSchCellCb        *cell;
U8                 *rntpPtr;
U8		              startRb;
U8                  nmbRb;
U16                 bw;
#endif
{
   U16 rbPtrStartIdx;              /* Start Index of Octete Buffer to be filled */
   U16 rbPtrEndIdx;                /* End Index of Octete Buffer to be filled */
   U16 rbBitLoc;                   /* Bit Location to be set as 1 in the current Byte */
   U16 nmbRbPerByte;               /* PRB's to be set in the current Byte (in case of multiple Bytes) */

   TRC2(rgSCHCmnBuildRntpInfo);

   rbPtrStartIdx = (startRb)/8;
   rbPtrEndIdx   = (startRb + nmbRb)/8;

   if (rntpPtr == NULLP)
   {
      RLOG_ARG0(L_ERROR,DBG_CELLID,cell->cellId,
               "rgSCHCmnBuildRntpInfo():"
               "rntpPtr can't be NULLP (Memory Allocation Failed)");
      RETVALUE(RFAILED);
   }

   while(rbPtrStartIdx <= rbPtrEndIdx)
   {
      rbBitLoc = (startRb)%8;

      /* case 1: startRb and endRb lies in same Byte */
      if (rbPtrStartIdx == rbPtrEndIdx)
      {
         rntpPtr[rbPtrStartIdx] = rntpPtr[rbPtrStartIdx]
                                     | (((1<<nmbRb)-1)<<rbBitLoc);
      }

      /* case 2: startRb and endRb lies in different Byte */
      if (rbPtrStartIdx != rbPtrEndIdx)
      {
         nmbRbPerByte = 8 - rbBitLoc;
         nmbRb        = nmbRb - nmbRbPerByte;
         rntpPtr[rbPtrStartIdx] = rntpPtr[rbPtrStartIdx]
                                     | (((1<<nmbRbPerByte)-1)<<rbBitLoc);
         startRb = startRb + nmbRbPerByte;
      }

      rbPtrStartIdx++;
   }

   /* dsfr_pal_fixes ** 21-March-2013 ** SKS ** Adding Debug logs */

   /* dsfr_pal_fixes ** 25-March-2013 ** SKS ** Adding Debug logs to print RNTP */

   RETVALUE(ROK);
}


/**
 * @brief To update non-DLFS alloc'n parameters after TYPE2 Allocation.
 *
 * @details
 *
 *     Function : rgSCHCmnNonDlfsUpdSFRPoolTyp2Alloc
 *
 *     Processing Steps:
 *
 *  @param[in]  RgSchCellCb     *cell
 *  @param[in]  RgSchDlSf       *dlSf
 *  @param[in]  U8              rbStrt
 *  @param[in]  U8              numRb
 *
 *  @return Void
 **/
#ifdef ANSI
PRIVATE S16 rgSCHCmnNonDlfsUpdSFRPoolTyp2Alloc
(
RgSchCellCb        *cell,
RgSchUeCb         *ue,
RgSchDlSf          *dlSf,
RgSchSFRPoolInfo   *sfrPool,
U8                 rbStrt,
U8                 numRb
)
#else
PRIVATE S16 rgSCHCmnNonDlfsUpdSFRPoolTyp2Alloc(cell, ue, dlSf, sfrPool, rbStrt, numRb)
RgSchCellCb        *cell;
RgSchUeCb          *ue;
RgSchDlSf          *dlSf;
RgSchSFRPoolInfo   *sfrPool;
U8                 rbStrt;
U8                 numRb;
#endif
{
#ifndef LTEMAC_SPS
   S16 ret;
#endif

   TRC2(rgSCHCmnNonDlfsUpdSFRPoolTyp2Alloc);
   dlSf->type2End = RGSCH_CEIL((rbStrt+numRb), cell->rbgSize);
   sfrPool->type2End = RGSCH_CEIL((rbStrt+numRb), cell->rbgSize);
   
#ifndef LTEMAC_SPS
   dlSf->type2Start += numRb;
   dlSf->bwAlloced += numRb;
   
   if(cell->lteAdvCb.dsfrCfg.status == RGR_ENABLE)
   {
      /* Based on RNTP info, the CC user is assigned high power per subframe basis */
      if(FALSE == ue->lteAdvUeCb.rgrLteAdvUeCfg.isUeCellEdge)
      {
         if((sfrPool->type2Start >= sfrPool->pwrHiCCRange.startRb) &&
               (sfrPool->type2Start + numRb < sfrPool->pwrHiCCRange.endRb))
         {
            ue->lteAdvUeCb.isCCUePHigh = TRUE;

            /* Calling rgSCHCmnBuildRntpInfo function to update RNTP BitMap */
            ret = rgSCHCmnBuildRntpInfo(cell, dlSf->rntpInfo.val, sfrPool->type2Start, numRb, dlSf->bw);
            if (ret != ROK)
            {
               RLOG_ARG1(L_ERROR,DBG_CELLID,cell->cellId,"rgSCHCmnNonDlfsUpdSFRPoolTyp2Alloc():"
                        "rgSCHCmnBuildRntpInfo() function returned RFAILED for CRNTI:%d",ue->ueId);
               RETVALUE(RFAILED);
            }
         }
      }
      else
      {
         /* Calling rgSCHCmnBuildRntpInfo function to update RNTP BitMap */
         ret = rgSCHCmnBuildRntpInfo(cell, dlSf->rntpInfo.val, sfrPool->type2Start, numRb, dlSf->bw);
         if (ret != ROK)
         {
            RLOG_ARG1(L_ERROR,DBG_CELLID,cell->cellId, "rgSCHCmnNonDlfsUpdSFRPoolTyp2Alloc():"
                     "rgSCHCmnBuildRntpInfo() function returned RFAILED for CRNTI:%d",ue->ueId);
            RETVALUE(RFAILED);
         }
      }
   }
   sfrPool->type2Start += numRb;
   sfrPool->bwAlloced += numRb;
#endif 

   RETVALUE(ROK);
}

/**
 * @brief To do DL allocation using TYPE0 RA.
 *
 * @details
 *
 *     Function : rgSCHCmnNonDlfsSFRPoolType0Alloc
 *
 *     Processing Steps:
 *      - Perform TYPE0 allocation using the RBGs between type0End and type2End.
 *      - Build the allocation mask as per RBG positioning.
 *      - Update the allocation parameters.
 *
 *  @param[in]  RgSchCellCb     *cell
 *  @param[in]  RgSchDlSf       *dlSf
 *  @param[in]  RgSchDlRbAlloc  *allocInfo
 *
 *  @return Void
 **/
#ifdef ANSI
PRIVATE Void rgSCHCmnNonDlfsSFRPoolType0Alloc
(
RgSchCellCb        *cell,
RgSchDlSf          *dlSf,
RgSchSFRPoolInfo   *poolInfo,
RgSchDlRbAlloc     *allocInfo
)
#else
PRIVATE Void rgSCHCmnNonDlfsSFRPoolType0Alloc(cell, dlSf, poolInfo, allocInfo)
RgSchCellCb        *cell;
RgSchDlSf          *dlSf;
RgSchSFRPoolInfo   *poolInfo;
RgSchDlRbAlloc     *allocInfo;
#endif
{
   U32 dlAllocMsk = 0;
   U8  rbgFiller = 0;
   U8  noRbgs = 0;
   U8  noRbs;
   U8  noLyr;
   U8  iTbs;

   TRC2(rgSCHCmnNonDlfsSFRPoolType0Alloc);

   if (poolInfo->poolstartRB + poolInfo->bw == dlSf->bw)
   {
      if (poolInfo->type0End == dlSf->bw/4)
      {
         rbgFiller = dlSf->lstRbgDfct;
         /* The last RBG which can be smaller than the RBG size is consedered
         * only for the first time allocation of TYPE0 UE */
         dlSf->lstRbgDfct = 0;
      }
   }

   noRbgs = RGSCH_CEIL((allocInfo->rbsReq + rbgFiller), cell->rbgSize);

   /* Abhinav to-do start */
   /* MS_FIX for ccpu00123919*/
   noRbs = (noRbgs * cell->rbgSize) - rbgFiller;
   if (dlSf->bwAlloced + noRbs > dlSf->bw)
   {
      if (--noRbgs == 0)
      {
         RETVOID;
      }
      noRbs = (noRbgs * cell->rbgSize) - rbgFiller;
   }
   /* Abhinav to-do end */


   
   /* type0End would have been initially (during subfrm Init) at the bit position
    * (cell->noOfRbgs - 1), 0 being the most significant.
    * Getting DlAllocMsk for noRbgs and at the appropriate position */
   dlAllocMsk |= (((1 << noRbgs) - 1) << (31 - poolInfo->type0End));
   /* Move backwards the type0End pivot */
   poolInfo->type0End -= noRbgs;
   /*MS_FIX for ccpu00123919*/
   /*noRbs = (noRbgs * cell->rbgSize) - rbgFiller;*/
   /* Update the bwAlloced field accordingly */
   poolInfo->bwAlloced += noRbs + dlSf->lstRbgDfct;
   dlSf->bwAlloced += noRbs + dlSf->lstRbgDfct;
   
   /* Update Type0 Alloc Info */
   allocInfo->allocInfo.raType0.numDlAlloc = noRbgs;
   allocInfo->allocInfo.raType0.dlAllocBitMask |= dlAllocMsk;
   allocInfo->rbsAlloc = noRbs;

   /* Update Tb info for each scheduled TB */
   iTbs = allocInfo->tbInfo[0].iTbs;
   noLyr = allocInfo->tbInfo[0].noLyr;
   /* Fix for ccpu00123919: For a RETX TB the iTbs is irrelevant.
    * RETX TB Size is same as Init TX TB Size */
   if (allocInfo->tbInfo[0].tbCb->txCntr)
   {
      allocInfo->tbInfo[0].bytesAlloc =
         allocInfo->tbInfo[0].bytesReq;
   }
   else
   {
      allocInfo->tbInfo[0].bytesAlloc =
         rgTbSzTbl[noLyr - 1][iTbs][noRbs - 1]/8;
   }

   if (allocInfo->tbInfo[1].schdlngForTb)
   {
      iTbs = allocInfo->tbInfo[1].iTbs;
      noLyr = allocInfo->tbInfo[1].noLyr;
      /* Fix for ccpu00123919: For a RETX TB the iTbs is irrelevant
       * RETX TB Size is same as Init TX TB Size */
      if (allocInfo->tbInfo[1].tbCb->txCntr)
      {
         allocInfo->tbInfo[1].bytesAlloc =
            allocInfo->tbInfo[1].bytesReq;
      }
      else
      {
         allocInfo->tbInfo[1].bytesAlloc =
            rgTbSzTbl[noLyr - 1][iTbs][noRbs - 1]/8;;
      }
   }

   /* The last RBG which can be smaller than the RBG size is consedered
    * only for the first time allocation of TYPE0 UE */
   dlSf->lstRbgDfct = 0;
   RETVOID;
}

/**
 * @brief Computes RNTP Info for a subframe.
 *
 * @details
 *
 *     Function :  rgSCHCmnNonDlfsDsfrRntpComp 
 *
 *     Processing Steps:
 *      - Computes RNTP info from individual pools.
 *
 *  @param[in]  RgSchDlSf       *dlSf
 *
 *  @return  void
 
 **/
#ifdef ANSI
PRIVATE void rgSCHCmnNonDlfsDsfrRntpComp
(
RgSchCellCb        *cell,
RgSchDlSf          *dlSf
)
#else
PRIVATE void rgSCHCmnNonDlfsDsfrRntpComp(cell, dlSf)
RgSchCellCb        *cell;
RgSchDlSf          *dlSf;
#endif
{
   PRIVATE U16 samples = 0;
   U16 i;
   U16 bwBytes = (dlSf->bw-1)/8;
   RgrLoadInfIndInfo *rgrLoadInf;
   U16 len;
   U16 ret     = ROK;

   TRC2(rgSCHCmnNonDlfsDsfrRntpComp);

   len = (dlSf->bw % 8 == 0) ? dlSf->bw/8 : dlSf->bw/8 + 1;

   /* RNTP info is ORed every TTI and the sample is stored in cell control block */ 
   for(i = 0; i <= bwBytes; i++)
   {
     cell->rntpAggrInfo.val[i] |= dlSf->rntpInfo.val[i];
   }
   samples = samples + 1;
   /* After every 1000 ms, the RNTP info will be sent to application to be further sent to all neighbouring eNB
         informing them about the load indication for cell edge users */
   if(RG_SCH_MAX_RNTP_SAMPLES == samples)
   {
      /* ccpu00134492 */
      ret = rgSCHUtlAllocSBuf (cell->instIdx,(Data**)&rgrLoadInf,
               sizeof(RgrLoadInfIndInfo));
      if (ret != ROK)
      {
         RLOG_ARG0(L_ERROR,DBG_CELLID,cell->cellId, "Could not "
            "allocate memory for sending LoadInfo");
         RETVOID;  
      }
     
      rgrLoadInf->u.rntpInfo.pres = cell->rntpAggrInfo.pres;
      /* dsfr_pal_fixes ** 21-March-2013 ** SKS */
      rgrLoadInf->u.rntpInfo.len  = len;

      /* dsfr_pal_fixes ** 21-March-2013 ** SKS */
      rgrLoadInf->u.rntpInfo.val = cell->rntpAggrInfo.val; 
      rgrLoadInf->cellId = cell->cellId;

      /* dsfr_pal_fixes ** 22-March-2013 ** SKS */
      rgrLoadInf->bw = dlSf->bw;
      rgrLoadInf->type = RGR_SFR;

      ret = rgSCHUtlRgrLoadInfInd(cell, rgrLoadInf);
      if(ret == RFAILED)
      {
         RLOG_ARG0(L_ERROR,DBG_CELLID,cell->cellId, "rgSCHCmnNonDlfsDsfrRntpComp():"
                  "rgSCHUtlRgrLoadInfInd() returned RFAILED");
      }

      cmMemset(cell->rntpAggrInfo.val,0,len);
      samples = 0;
   }
 } 
/* LTE_ADV_FLAG_REMOVED_END */

/* LTE_ADV_FLAG_REMOVED_START */
/**
 * @brief Performs RB allocation per UE from a pool.
 *
 * @details
 *
 *     Function : rgSCHCmnSFRNonDlfsUeRbAlloc
 *
 *     Processing Steps:
 *      - Allocate consecutively available RBs.
 *
 *  @param[in]  RgSchCellCb     *cell
 *  @param[in]  RgSchUeCb       *ue
 *  @param[in]  RgSchDlSf       *dlSf
 *  @param[out] U8              *isDlBwAvail
 *
 *  @return  S16
 *      -# ROK
 *      -# RFAILED
 **/
#ifdef ANSI
PRIVATE S16 rgSCHCmnSFRNonDlfsUeRbAlloc
(
RgSchCellCb        *cell,
RgSchUeCb          *ue,
RgSchDlSf          *dlSf,
U8                 *isDlBwAvail
)
#else
PRIVATE S16 rgSCHCmnSFRNonDlfsUeRbAlloc(cell, ue, dlSf, isDlBwAvail)
RgSchCellCb        *cell;
RgSchUeCb          *ue;
RgSchDlSf          *dlSf;
U8                 *isDlBwAvail;
#endif
{
   U32             y;
   RgSchDlRbAlloc  *allocInfo;
   RgSchCmnDlUe    *dlUe;
   Bool isUECellEdge;
   RgSchSFRPoolInfo *sfrpoolInfo = NULLP;

   TRC2(rgSCHCmnSFRNonDlfsUeRbAlloc);

   isUECellEdge = RG_SCH_CMN_IS_UE_CELL_EDGE(ue);

   dlUe = RG_SCH_CMN_GET_DL_UE(ue);
   allocInfo =  RG_SCH_CMN_GET_ALLOCCB_FRM_UE(ue);
   *isDlBwAvail = TRUE;

   /*Find which pool is available for this UE*/
   if (rgSCHCmnNonDlfsSFRBwAvlbl(cell,  &sfrpoolInfo, dlSf, allocInfo, isUECellEdge) != TRUE)
   {
      /* SFR_FIX - If this is CE UE there may be BW available in CC Pool
         So CC UEs will be scheduled */
      if (isUECellEdge)
      {
         *isDlBwAvail = TRUE;
      }
      else
      {
         *isDlBwAvail = FALSE;
      }
      RETVALUE(RFAILED);
   }

   /* Retrieve PDCCH */
   y = rgSCHUtlGetY(ue, dlSf->sfNum);

   if (dlUe->proc->tbInfo[0].isAckNackDtx == TFU_HQFDB_DTX || dlUe->proc->tbInfo[1].isAckNackDtx)
   {
      allocInfo->pdcch = rgSCHCmnPdcchAlloc(cell, dlSf, y, dlUe->mimoInfo.cwInfo[0].cqi, allocInfo->dciFormat, TRUE);
   }
   else
   {
      allocInfo->pdcch = rgSCHCmnPdcchAlloc(cell, dlSf, y, dlUe->mimoInfo.cwInfo[0].cqi, allocInfo->dciFormat,FALSE);
   }
   
   if (!(allocInfo->pdcch))
   {
      /* Returning ROK since PDCCH might be available for another UE and further allocations could be done */
      RETVALUE(RFAILED);
   }
   
#ifdef LTEMAC_SPS
   allocInfo->rnti = ue->ueId;
#endif

#if 0 /* SFR_FIX: Removed PBCH Adjustment logic */
   /* Update allocation information */
#ifndef LTE_TDD
      if(dlSf->sfNum)
      {
       /*rg002.301 ccpu00119901 MOD modified to correct subframe number*/
         if(!(dlSf->sfNum == 5))
         {
            /* case for subframes 1 to 9 except 5 */
             if (allocInfo->raType == RG_SCH_CMN_RA_TYPE2)
             {
                allocInfo->allocInfo.raType2.rbStart = (U8)sfrpoolInfo->type2Start;
             }
         }
         else
         {
            pbchFrame = 1; /* case for subframe 5 */
            /* In subframe 5, symbols are reserved for PSS and SSS and CFICH   and Cell Specific Reference Signals */
            pbchSsRsSym = (((cellDl->currCfi) + RGSCH_NUM_PSS_SSS_SYM) *
                  RGSCH_NUM_SC_IN_RB + cell->numCellRSPerSf);
         }
      }
      else
      {
         pbchFrame = 1;
         /* In subframe 0, symbols are reserved for PSS, SSS, PBCH, CFICH and  and Cell Specific Reference signals */
         pbchSsRsSym = (((cellDl->currCfi) + RGSCH_NUM_PBCH_SYM +
                  RGSCH_NUM_PSS_SSS_SYM) * RGSCH_NUM_SC_IN_RB +
               cell->numCellRSPerSf);
      } /* end of outer else */

      if((pbchFrame) &&
            (((sfrpoolInfo->poolstartRB + sfrpoolInfo->bwAlloced + allocInfo->rbsReq) - cell->pbchRbStart) > 0)&&
            (sfrpoolInfo->poolstartRB + sfrpoolInfo->bwAlloced < cell->pbchRbEnd))
      {
         if(allocInfo->tbInfo[1].schdlngForTb == TRUE)
         {
            /* MS_FIX for ccpu00123919: Avoid allocating RETX incase of collision with reserved resources for BCH/PSS/SSS occassions */
            if ((allocInfo->tbInfo[0].tbCb->txCntr == 0) &&
                (allocInfo->tbInfo[1].tbCb->txCntr == 0))
            {
               /* adjusting only for new transmissions */
               /* Adjusting IMCS and tbSize of TB1 */
               rgSCHCmnNonDlfsSFRPbchRbAllocAdj(cell, sfrpoolInfo, allocInfo, pbchSsRsSym);
            }
         }
         else if (allocInfo->tbInfo[0].tbCb->txCntr == 0)
         {
            /* adjusting only for new transmissions */
            rgSCHCmnNonDlfsSFRPbchRbAllocAdj(cell, sfrpoolInfo, allocInfo, pbchSsRsSym);
         }
      }
#endif
#endif /* SFR_FIX */
   if (allocInfo->raType == RG_SCH_CMN_RA_TYPE2)
   {
      allocInfo->allocInfo.raType2.isLocal = TRUE;
      /* rg004.201 patch - ccpu00109921 fix end */
      /* MS_FIX for ccpu00123918*/
      allocInfo->allocInfo.raType2.rbStart = (U8)sfrpoolInfo->type2Start;
      allocInfo->allocInfo.raType2.numRb = allocInfo->rbsReq;
      /* rg007.201 - Changes for MIMO feature addition */
      /* rg008.201 - Removed dependency on MIMO compile-time flag */
      rgSCHCmnNonDlfsUpdSFRPoolTyp2Alloc(cell, ue, dlSf, sfrpoolInfo, \
            allocInfo->allocInfo.raType2.rbStart, \
            allocInfo->allocInfo.raType2.numRb);
      allocInfo->rbsAlloc = allocInfo->rbsReq;
      allocInfo->tbInfo[0].bytesAlloc = allocInfo->tbInfo[0].bytesReq;
   }
   else if (allocInfo->raType == RG_SCH_CMN_RA_TYPE0)
   {
      rgSCHCmnNonDlfsSFRPoolType0Alloc(cell, dlSf, sfrpoolInfo, allocInfo);
   }
#ifndef LTE_TDD
#ifdef DEBUGP
   rgSCHCmnFindCodeRate(cell,dlSf,allocInfo,0);
   if(allocInfo->tbInfo[1].schdlngForTb == TRUE)
   {
      rgSCHCmnFindCodeRate(cell,dlSf,allocInfo,1);
   }
#endif
#endif

#if defined(LTEMAC_SPS)
   /* Update the sub-frame with new allocation */
   dlSf->bwAlloced += allocInfo->rbsReq;
#endif

   RETVALUE(ROK);
}
/* LTE_ADV_FLAG_REMOVED_END */
#endif /* LTE_TDD */

/**
 * @brief Performs RB allocation per UE for frequency non-selective cell.
 *
 * @details
 *
 *     Function : rgSCHCmnNonDlfsUeRbAlloc
 *
 *     Processing Steps:
 *      - Allocate consecutively available RBs.
 *
 *  @param[in]  RgSchCellCb     *cell
 *  @param[in]  RgSchUeCb       *ue
 *  @param[in]  RgSchDlSf       *dlSf
 *  @param[out] U8              *isDlBwAvail
 *
 *  @return  S16
 *      -# ROK
 *      -# RFAILED
 **/
#ifdef ANSI
PRIVATE S16 rgSCHCmnNonDlfsUeRbAlloc
(
RgSchCellCb        *cell,
RgSchUeCb          *ue,
RgSchDlSf          *dlSf,
U8                 *isDlBwAvail
)
#else
PRIVATE S16 rgSCHCmnNonDlfsUeRbAlloc(cell, ue, dlSf, isDlBwAvail)
RgSchCellCb        *cell;
RgSchUeCb          *ue;
RgSchDlSf          *dlSf;
U8                 *isDlBwAvail;
#endif
{
   U32             y;
   RgSchDlRbAlloc  *allocInfo;
   RgSchCmnDlUe    *dlUe;
   TRC2(rgSCHCmnNonDlfsUeRbAlloc);

   dlUe = RG_SCH_CMN_GET_DL_UE(ue);
   allocInfo =  RG_SCH_CMN_GET_ALLOCCB_FRM_UE(ue);
   *isDlBwAvail = TRUE;
   if (rgSCHCmnNonDlfsBwAvlbl(cell, dlSf, allocInfo) != TRUE)
   {
      RLOG_ARG1(L_DEBUG,DBG_CELLID,cell->cellId,  "FAILED for CRNTI:%d",
          ue->ueId);
      *isDlBwAvail = FALSE;
      RETVALUE(RFAILED);
   }
#ifndef LTE_TDD
#endif
   /* Retrieve PDCCH */
   y = rgSCHUtlGetY(ue, dlSf->sfNum);
   if (dlUe->proc->tbInfo[0].isAckNackDtx == TFU_HQFDB_DTX 
       || dlUe->proc->tbInfo[1].isAckNackDtx)
   {
      allocInfo->pdcch = rgSCHCmnPdcchAlloc(cell, dlSf, y, dlUe->mimoInfo.cwInfo[0].cqi, allocInfo->dciFormat, TRUE);
   }
   else
   {
      allocInfo->pdcch = rgSCHCmnPdcchAlloc(cell, dlSf, y, dlUe->mimoInfo.cwInfo[0].cqi, allocInfo->dciFormat,FALSE);
   }
   if (!(allocInfo->pdcch))
   {
      /* Returning ROK since PDCCH might be available for another UE and
       * further allocations could be done */
      RETVALUE(RFAILED);
   }
#ifdef LTEMAC_SPS
   allocInfo->rnti = ue->ueId;
#endif
   /* Update allocation information */
   /*ccpu00131191 and ccpu00131317 - Fix for RRC Reconfig failure
    * issue for VoLTE call.
    * Commenting out the below block*/
   if (allocInfo->raType == RG_SCH_CMN_RA_TYPE2)
   {
      allocInfo->allocInfo.raType2.isLocal = TRUE;
      allocInfo->allocInfo.raType2.rbStart = (U8)dlSf->type2Start;
      allocInfo->allocInfo.raType2.numRb = allocInfo->rbsReq;
     
      /* LTE_ADV_FLAG_REMOVED_START */
#ifndef LTE_TDD
      if ((cell->lteAdvCb.dsfrCfg.status == RGR_ENABLE) && (cell->lteAdvCb.sfrCfg.status == RGR_ENABLE))
      {
         rgSCHCmnNonDlfsUpdDSFRTyp2Alloc(cell, ue, dlSf, \
               allocInfo->allocInfo.raType2.rbStart, \
               allocInfo->allocInfo.raType2.numRb);
      }
      else
#endif /* end of ifndef LTE_TDD*/
      {
         rgSCHCmnNonDlfsUpdTyp2Alloc(cell, dlSf, \
               allocInfo->allocInfo.raType2.rbStart, \
               allocInfo->allocInfo.raType2.numRb);
      }
      /* LTE_ADV_FLAG_REMOVED_END */
      allocInfo->rbsAlloc = allocInfo->rbsReq;
      allocInfo->tbInfo[0].bytesAlloc = allocInfo->tbInfo[0].bytesReq;
   }
   else if (allocInfo->raType == RG_SCH_CMN_RA_TYPE0)
   {
      rgSCHCmnNonDlfsType0Alloc(cell, dlSf, allocInfo, dlUe);
   }
#ifndef LTE_TDD
#ifdef DEBUGP
   rgSCHCmnFindCodeRate(cell,dlSf,allocInfo,0);
   if(allocInfo->tbInfo[1].schdlngForTb == TRUE)
   {
      rgSCHCmnFindCodeRate(cell,dlSf,allocInfo,1);
   }
#endif
#endif

#if defined(LTEMAC_SPS)
#endif

   RETVALUE(ROK);
}

#ifdef RGR_V1
/**
 * @brief Performs RB allocation for Msg4 for frequency non-selective cell.
 *
 * @details
 *
 *     Function : rgSCHCmnNonDlfsCcchSduAlloc
 *
 *     Processing Steps:
 *     - For each element in the list, Call rgSCHCmnNonDlfsCcchSduRbAlloc().
 *        - If allocation is successful, add the ueCb to scheduled list of CCCH
 *        SDU.
 *        - else, add UeCb to non-scheduled list.
 *
 *  @param[in]      RgSchCellCb         *cell
 *  @param[in, out] RgSchCmnCcchSduRbAlloc *allocInfo
 *  @param[in]      U8                  isRetx
 *
 *  @return  Void
 **/
#ifdef ANSI
PRIVATE Void rgSCHCmnNonDlfsCcchSduAlloc
(
RgSchCellCb         *cell,
RgSchCmnCcchSduRbAlloc *allocInfo,
U8                  isRetx
)
#else
PRIVATE Void rgSCHCmnNonDlfsCcchSduAlloc(cell, allocInfo, isRetx)
RgSchCellCb         *cell;
RgSchCmnCcchSduRbAlloc *allocInfo;
U8                  isRetx;
#endif
{
   S16             ret;
   CmLListCp       *ccchSduLst        = NULLP;
   CmLListCp       *schdCcchSduLst    = NULLP;
   CmLListCp       *nonSchdCcchSduLst = NULLP;
   CmLList         *schdLnkNode    = NULLP;
   CmLList         *toBeSchdLnk    = NULLP;
   RgSchDlSf       *dlSf           = allocInfo->ccchSduDlSf;
   RgSchUeCb       *ueCb           = NULLP;
   RgSchDlHqProcCb *hqP            = NULLP;
   TRC2(rgSCHCmnNonDlfsCcchSduAlloc);

   if (isRetx)
   {
      /* Initialize re-transmitting lists */
      ccchSduLst = &(allocInfo->ccchSduRetxLst);
      schdCcchSduLst = &(allocInfo->schdCcchSduRetxLst);
      nonSchdCcchSduLst = &(allocInfo->nonSchdCcchSduRetxLst);
   }
   else
   {
      /* Initialize transmitting lists */
      ccchSduLst = &(allocInfo->ccchSduTxLst);
      schdCcchSduLst = &(allocInfo->schdCcchSduTxLst);
      nonSchdCcchSduLst = &(allocInfo->nonSchdCcchSduTxLst);
   }

   /* Perform allocaations  for the list */
   toBeSchdLnk = cmLListFirst(ccchSduLst);
   for (; toBeSchdLnk; toBeSchdLnk = toBeSchdLnk->next)
   {
      hqP = (RgSchDlHqProcCb *)(toBeSchdLnk->node);
      ueCb = hqP->hqE->ue;
      schdLnkNode = &hqP->schdLstLnk;
      RG_SCH_CMN_INIT_SCHD_LNK(schdLnkNode, hqP);
      ret = rgSCHCmnNonDlfsCcchSduRbAlloc(cell, ueCb, dlSf);
      if (ret != ROK)
      {
         /* Allocation failed: Add remaining MSG4 nodes to non-scheduled
          * list and return */
         do
         {
            hqP = (RgSchDlHqProcCb *)(toBeSchdLnk->node);
            ueCb = hqP->hqE->ue;
            schdLnkNode = &hqP->schdLstLnk;
            RG_SCH_CMN_INIT_SCHD_LNK(schdLnkNode, hqP);
            cmLListAdd2Tail(nonSchdCcchSduLst, schdLnkNode);
            toBeSchdLnk = toBeSchdLnk->next;
         } while(toBeSchdLnk);
         RETVOID;
      }

      /* Allocation successful: Add UE to the scheduled list */
      cmLListAdd2Tail(schdCcchSduLst, schdLnkNode);
   }


   RETVOID;
}

/**
 * @brief Performs RB allocation for CcchSdu for frequency non-selective cell.
 *
 * @details
 *
 *     Function : rgSCHCmnNonDlfsCcchSduRbAlloc
 *
 *     Processing Steps:
 *     - Fetch PDCCH
 *     - Allocate consecutively available RBs
 *
 *  @param[in] RgSchCellCb     *cell
 *  @param[in] RgSchUeCb       *ueCb
 *  @param[in] RgSchDlSf       *dlSf
 *  @return  S16
 *      -# ROK
 *      -# RFAILED
 **/
#ifdef ANSI
PRIVATE S16 rgSCHCmnNonDlfsCcchSduRbAlloc
(
RgSchCellCb        *cell,
RgSchUeCb          *ueCb,
RgSchDlSf          *dlSf
)
#else
PRIVATE S16 rgSCHCmnNonDlfsCcchSduRbAlloc(cell, ueCb, dlSf)
RgSchCellCb        *cell;
RgSchUeCb          *ueCb;
RgSchDlSf          *dlSf;
#endif
{
   U32             y;
   RgSchDlRbAlloc  *allocInfo;
   RgSchCmnDlUe         *ueDl = RG_SCH_CMN_GET_DL_UE(ueCb);

   TRC2(rgSCHCmnNonDlfsCcchSduRbAlloc);


   allocInfo =  RG_SCH_CMN_GET_ALLOCCB_FRM_UE(ueCb);

   /* [ccpu00138802]-MOD-If Bw is less than required, return fail
      It will be allocated in next TTI */
#ifdef LTEMAC_SPS
   if ((dlSf->spsAllocdBw >= cell->spsBwRbgInfo.numRbs) &&
         (dlSf->bwAlloced == dlSf->bw))
#else
   if((dlSf->bwAlloced == dlSf->bw) ||
      (allocInfo->rbsReq > (dlSf->bw - dlSf->bwAlloced)))
#endif
   {
      RETVALUE(RFAILED);
   }
   /* Retrieve PDCCH */
   y = ueCb->y[dlSf->sfNum];
   /* DTX Changes: One Variable is passed to check whether it is DTX or Not */
   if (ueDl->proc->tbInfo[0].isAckNackDtx == TFU_HQFDB_DTX)
   {
      /*      allocInfo->pdcch = rgSCHCmnPdcchAlloc(cell, dlSf, y, ueDl->cqi,
       *      TFU_DCI_FORMAT_1A, TRUE);*/
      allocInfo->pdcch = rgSCHCmnPdcchAlloc(cell, dlSf, y, ueDl->mimoInfo.cwInfo[0].cqi, TFU_DCI_FORMAT_1A, TRUE);
   }
   else
   {
      allocInfo->pdcch = rgSCHCmnPdcchAlloc(cell, dlSf, y, ueDl->mimoInfo.cwInfo[0].cqi, TFU_DCI_FORMAT_1A, FALSE);
   }
   if (!(allocInfo->pdcch))
   {
      /* Returning RFAILED since PDCCH not available for any CCCH allocations */
      RETVALUE(RFAILED);
   }

   /* Update allocation information */
   allocInfo->dciFormat = TFU_DCI_FORMAT_1A;
   allocInfo->raType = RG_SCH_CMN_RA_TYPE2;
   allocInfo->allocInfo.raType2.isLocal = TRUE;

      /*Fix for ccpu00123918*/
      /* Push this harq process back to the free queue */
      allocInfo->allocInfo.raType2.rbStart = (U8)dlSf->type2Start;
      allocInfo->allocInfo.raType2.numRb = allocInfo->rbsReq;
      allocInfo->rbsAlloc = allocInfo->rbsReq;
      allocInfo->tbInfo[0].bytesAlloc = allocInfo->tbInfo[0].bytesReq;
      /* Update the sub-frame with new allocation */
      /* ccpu00129469 */
      /* LTE_ADV_FLAG_REMOVED_START */
#ifndef LTE_TDD
      if (cell->lteAdvCb.sfrCfg.status == RGR_ENABLE)
      {
         rgSCHCmnNonDlfsSFRCmnChannelUpdTyp2Alloc(cell, dlSf,
               allocInfo->allocInfo.raType2.rbStart,
               allocInfo->allocInfo.raType2.numRb);
      }
      else
#endif /* end of ifndef LTE_TDD*/
      {
         rgSCHCmnNonDlfsUpdTyp2Alloc(cell, dlSf, 
               allocInfo->allocInfo.raType2.rbStart, 
               allocInfo->allocInfo.raType2.numRb);
      }

   /* LTE_ADV_FLAG_REMOVED_END */
   /* ccpu00131941 - bwAlloced is updated from SPS bandwidth */  


   RETVALUE(ROK);
}
#endif

/**
 * @brief Performs RB allocation for Msg4 for frequency non-selective cell.
 *
 * @details
 *
 *     Function : rgSCHCmnNonDlfsMsg4RbAlloc
 *
 *     Processing Steps:
 *     - Fetch PDCCH
 *     - Allocate consecutively available RBs
 *
 *  @param[in] RgSchCellCb     *cell
 *  @param[in] RgSchRaCb       *raCb
 *  @param[in] RgSchDlSf       *dlSf
 *  @return  S16
 *      -# ROK
 *      -# RFAILED
 **/
#ifdef ANSI
PRIVATE S16 rgSCHCmnNonDlfsMsg4RbAlloc
(
RgSchCellCb        *cell,
RgSchRaCb          *raCb,
RgSchDlSf          *dlSf
)
#else
PRIVATE S16 rgSCHCmnNonDlfsMsg4RbAlloc(cell, raCb, dlSf)
RgSchCellCb        *cell;
RgSchRaCb          *raCb;
RgSchDlSf          *dlSf;
#endif
{
   U32             y;
   RgSchDlRbAlloc  *allocInfo;
   TRC2(rgSCHCmnNonDlfsMsg4RbAlloc);


   allocInfo =  RG_SCH_CMN_GET_ALLOCCB_FRM_RACB(raCb);

#ifdef LTEMAC_SPS
   if ((dlSf->spsAllocdBw >= cell->spsBwRbgInfo.numRbs) &&
         (dlSf->bwAlloced == dlSf->bw))
#else
   if((dlSf->bwAlloced == dlSf->bw) ||
            (allocInfo->rbsReq > (dlSf->bw - dlSf->bwAlloced)))
#endif
   {

      RETVALUE(RFAILED);
   }

   /* Retrieve PDCCH */
   y = raCb->y[dlSf->sfNum];
   /* DTX Changes: One Variable is passed to check whether it is DTX or Not */
   if (raCb->dlHqE->msg4Proc->tbInfo[0].isAckNackDtx == TFU_HQFDB_DTX)
   {
      allocInfo->pdcch = rgSCHCmnPdcchAlloc(cell, dlSf, y, raCb->ccchCqi, TFU_DCI_FORMAT_1A, TRUE);
   }
   else
   {
      allocInfo->pdcch = rgSCHCmnPdcchAlloc(cell, dlSf, y, raCb->ccchCqi, TFU_DCI_FORMAT_1A, FALSE);
   }
   if (!(allocInfo->pdcch))
   {
      /* Returning RFAILED since PDCCH not available for any CCCH allocations */
      RETVALUE(RFAILED);
   }
   
 /* SR_RACH_STATS : MSG4 TX Failed */
   allocInfo->pdcch->dci.u.format1aInfo.t.pdschInfo.isTBMsg4 = TRUE;

   /* Update allocation information */
   allocInfo->dciFormat = TFU_DCI_FORMAT_1A;
   allocInfo->raType = RG_SCH_CMN_RA_TYPE2;
   allocInfo->allocInfo.raType2.isLocal = TRUE;


        /*Fix for ccpu00123918*/
        allocInfo->allocInfo.raType2.rbStart = (U8)dlSf->type2Start;
        allocInfo->allocInfo.raType2.numRb = allocInfo->rbsReq;
        /* LTE_ADV_FLAG_REMOVED_START */
#ifndef LTE_TDD
        if (cell->lteAdvCb.sfrCfg.status == RGR_ENABLE)
        {
           rgSCHCmnNonDlfsSFRCmnChannelUpdTyp2Alloc(cell, dlSf, \
                 allocInfo->allocInfo.raType2.rbStart, \
                 allocInfo->allocInfo.raType2.numRb);
        }
        else
#endif /* end of ifndef LTE_TDD */
        {
           rgSCHCmnNonDlfsUpdTyp2Alloc(cell, dlSf, \
                 allocInfo->allocInfo.raType2.rbStart, \
                 allocInfo->allocInfo.raType2.numRb);
        }
        /* LTE_ADV_FLAG_REMOVED_END */


   allocInfo->rbsAlloc = allocInfo->rbsReq;
   allocInfo->tbInfo[0].bytesAlloc = allocInfo->tbInfo[0].bytesReq;




   RETVALUE(ROK);
}

/**
 * @brief Performs RB allocation for Msg4 lists of frequency non-selective cell.
 *
 * @details
 *
 *     Function : rgSCHCmnNonDlfsMsg4Alloc
 *
 *     Processing Steps:
 *     - For each element in the list, Call rgSCHCmnNonDlfsMsg4RbAlloc().
 *        - If allocation is successful, add the raCb to scheduled list of MSG4.
 *        - else, add RaCb to non-scheduled list.
 *
 *  @param[in]      RgSchCellCb         *cell
 *  @param[in, out] RgSchCmnMsg4RbAlloc *allocInfo
 *  @param[in]      U8                  isRetx
 *
 *  @return  Void
 **/
#ifdef ANSI
PRIVATE Void rgSCHCmnNonDlfsMsg4Alloc
(
RgSchCellCb         *cell,
RgSchCmnMsg4RbAlloc *allocInfo,
U8                  isRetx
)
#else
PRIVATE Void rgSCHCmnNonDlfsMsg4Alloc(cell, allocInfo, isRetx)
RgSchCellCb         *cell;
RgSchCmnMsg4RbAlloc *allocInfo;
U8                  isRetx;
#endif
{
   S16             ret;
   CmLListCp       *msg4Lst        = NULLP;
   CmLListCp       *schdMsg4Lst    = NULLP;
   CmLListCp       *nonSchdMsg4Lst = NULLP;
   CmLList         *schdLnkNode    = NULLP;
   CmLList         *toBeSchdLnk    = NULLP;
   RgSchDlSf       *dlSf           = allocInfo->msg4DlSf;
   RgSchRaCb       *raCb           = NULLP;
   RgSchDlHqProcCb *hqP            = NULLP;
   TRC2(rgSCHCmnNonDlfsMsg4Alloc);

   if (isRetx)
   {
      /* Initialize re-transmitting lists */
      msg4Lst = &(allocInfo->msg4RetxLst);
      schdMsg4Lst = &(allocInfo->schdMsg4RetxLst);
      nonSchdMsg4Lst = &(allocInfo->nonSchdMsg4RetxLst);
   }
   else
   {
      /* Initialize transmitting lists */
      msg4Lst = &(allocInfo->msg4TxLst);
      schdMsg4Lst = &(allocInfo->schdMsg4TxLst);
      nonSchdMsg4Lst = &(allocInfo->nonSchdMsg4TxLst);
   }

   /* Perform allocaations  for the list */
   toBeSchdLnk = cmLListFirst(msg4Lst);
   for (; toBeSchdLnk; toBeSchdLnk = toBeSchdLnk->next)
   {
      hqP = (RgSchDlHqProcCb *)(toBeSchdLnk->node);
      raCb = hqP->hqE->raCb;
      schdLnkNode = &hqP->schdLstLnk;
      RG_SCH_CMN_INIT_SCHD_LNK(schdLnkNode, hqP);
      ret = rgSCHCmnNonDlfsMsg4RbAlloc(cell, raCb, dlSf);
      if (ret != ROK)
      {
         /* Allocation failed: Add remaining MSG4 nodes to non-scheduled
          * list and return */
         do
         {
            hqP = (RgSchDlHqProcCb *)(toBeSchdLnk->node);
            raCb = hqP->hqE->raCb;
            schdLnkNode = &hqP->schdLstLnk;
            RG_SCH_CMN_INIT_SCHD_LNK(schdLnkNode, hqP);
            cmLListAdd2Tail(nonSchdMsg4Lst, schdLnkNode);
            toBeSchdLnk = toBeSchdLnk->next;
         } while(toBeSchdLnk);
         RETVOID;
      }

      /* Allocation successful: Add UE to the scheduled list */
      cmLListAdd2Tail(schdMsg4Lst, schdLnkNode);
      if (isRetx)
      {
      }
   }


   RETVOID;
}

/**
 * @brief Performs RB allocation for the list of UEs of a frequency
 * non-selective cell.
 *
 * @details
 *
 *     Function : rgSCHCmnNonDlfsDedRbAlloc
 *
 *     Processing Steps:
 *     - For each element in the list, Call rgSCHCmnNonDlfsUeRbAlloc().
 *        - If allocation is successful, add the ueCb to scheduled list of UEs.
 *        - else, add ueCb to non-scheduled list of UEs.
 *
 *  @param[in]      RgSchCellCb        *cell
 *  @param[in, out] RgSchCmnUeRbAlloc  *allocInfo
 *  @param[in]      CmLListCp          *ueLst,
 *  @param[in, out] CmLListCp          *schdHqPLst,
 *  @param[in, out] CmLListCp          *nonSchdHqPLst
 *
 *  @return  Void
 **/
#ifdef ANSI
PRIVATE Void rgSCHCmnNonDlfsDedRbAlloc
(
RgSchCellCb        *cell,
RgSchCmnUeRbAlloc  *allocInfo,
CmLListCp          *ueLst,
CmLListCp          *schdHqPLst,
CmLListCp          *nonSchdHqPLst
)
#else
PRIVATE Void rgSCHCmnNonDlfsDedRbAlloc(cell, allocInfo, ueLst,
        schdHqPLst, nonSchdHqPLst)
RgSchCellCb        *cell;
RgSchCmnUeRbAlloc  *allocInfo;
CmLListCp          *ueLst;
CmLListCp          *schdHqPLst;
CmLListCp          *nonSchdHqPLst;
#endif
{
   S16             ret;
#ifdef LTEMAC_SPS
   CmLList         *nonSchdLstPtr = NULLP;
   CmLList         *prevLstPtr = NULLP;
   Bool            isSpsBwAvail   = TRUE;
#endif
   CmLList         *schdLnkNode  = NULLP;
   CmLList         *toBeSchdLnk  = NULLP;
   RgSchDlRbAlloc  *dlAlloc      = NULLP;
   RgSchDlSf       *dlSf         = allocInfo->dedDlSf;
   RgSchUeCb       *ue           = NULLP;
   RgSchDlHqProcCb *hqP          = NULLP;
   U8              isDlBwAvail;
   TRC2(rgSCHCmnNonDlfsDedRbAlloc);


   /* Perform allocaations  for the list */
   toBeSchdLnk = cmLListFirst(ueLst);
   for (; toBeSchdLnk; toBeSchdLnk = toBeSchdLnk->next)
   {
      hqP = (RgSchDlHqProcCb *)(toBeSchdLnk->node);
      ue = hqP->hqE->ue;
      dlAlloc = RG_SCH_CMN_GET_ALLOCCB_FRM_UE(ue);
      schdLnkNode = &hqP->schdLstLnk;
      RG_SCH_CMN_INIT_SCHD_LNK(schdLnkNode, hqP);
#ifndef LTE_TDD      
      /* LTE_ADV_FLAG_REMOVED_START */
      if ((cell->lteAdvCb.sfrCfg.status == RGR_ENABLE) && (TRUE == dlSf->sfrTotalPoolInfo.isUeCellEdge))
      {
         ret = rgSCHCmnSFRNonDlfsUeRbAlloc(cell, ue, dlSf, &isDlBwAvail);
      }
      else
      /* LTE_ADV_FLAG_REMOVED_END */
#endif /* ifndef LTE_TDD */          
      ret = rgSCHCmnNonDlfsUeRbAlloc(cell, ue, dlSf, &isDlBwAvail);
      if (!isDlBwAvail)
      {
         /* Allocation failed: Add remaining UEs to non-scheduled
          * list and return */
         do
         {
            hqP = (RgSchDlHqProcCb *)(toBeSchdLnk->node);
            ue = hqP->hqE->ue;
            dlAlloc = RG_SCH_CMN_GET_ALLOCCB_FRM_UE(ue);
            schdLnkNode = &hqP->schdLstLnk;
            RG_SCH_CMN_INIT_SCHD_LNK(schdLnkNode, hqP);
            cmLListAdd2Tail(nonSchdHqPLst, schdLnkNode);
            toBeSchdLnk = toBeSchdLnk->next;
         } while(toBeSchdLnk);
         break; 
      }

      if (ret == ROK)
      {
         /* Allocation successful: Add UE to the scheduled list */
         cmLListAdd2Tail(schdHqPLst, schdLnkNode);
         {
            if (ueLst == &allocInfo->retxHqPLst)
            {
               static U32 retxCnt = 0;
               ++retxCnt;
               if ((cell->crntTime.sfn % 10 == 0)&& (cell->crntTime.subframe == 0))
               {
               }
            }
         }
      }
      else
      {
         /* Allocation failed : Add UE to the non-scheduled list */
         cmLListAdd2Tail(nonSchdHqPLst, schdLnkNode);
      }
   }
#ifdef LTEMAC_SPS

   if (!((ueLst == &allocInfo->retxHqPLst) ||
         (ueLst == &allocInfo->retxSpsHqPLst) || 
         (ueLst == &allocInfo->txRetxHqPLst)))
   {
   /* Attempt Scheduling for non-scheduled UEs in SPS BW */
   nonSchdLstPtr = cmLListFirst(nonSchdHqPLst);
   while (isSpsBwAvail && nonSchdLstPtr)
   {
      hqP = (RgSchDlHqProcCb *)(nonSchdLstPtr->node); 
      ue = hqP->hqE->ue;
      dlAlloc = RG_SCH_CMN_GET_ALLOCCB_FRM_UE(ue);
         if (dlAlloc->rbsAlloc)
         {
            /* UE already scheduled */
            prevLstPtr = nonSchdLstPtr;
            nonSchdLstPtr = nonSchdLstPtr->next;
            continue;
         }

      isSpsBwAvail =
               rgSCHCmnAllocUeInSpsBw(dlSf, cell, ue, dlAlloc, TRUE);

      if (!isSpsBwAvail)
      {
         break;
      }
      prevLstPtr = nonSchdLstPtr;
      nonSchdLstPtr = nonSchdLstPtr->next;
      if (dlAlloc->rbsAlloc)
      {
         /* Remove the UE from the non-scheduled list */
         cmLListDelFrm(nonSchdHqPLst, prevLstPtr);
         /* Add UE to scheduled UE list */
         cmLListAdd2Tail(schdHqPLst, prevLstPtr);

         }
      }
   }
#endif

   RETVOID;
}

/**
 * @brief Handles RB allocation for frequency non-selective cell.
 *
 * @details
 *
 *     Function : rgSCHCmnNonDlfsRbAlloc
 *
 *     Invoking Module Processing:
 *      - SCH shall invoke this if downlink frequency selective is disabled for
 *        the cell for RB allocation.
 *      - MAX C/I/PFS/RR shall provide the requiredBytes, required RBs
 *        estimate and subframe for each allocation to be made to SCH.
 *
 *     Processing Steps:
 *     - Allocate sequentially for common channels.
 *     - For transmitting and re-transmitting UE list.
 *      - For each UE:
 *       - Perform wide-band allocations for UE in increasing order of
 *         frequency.
 *       - Determine Imcs for the allocation.
 *       - Determine RA type.
 *       - Determine DCI format.
 *
 *  @param[in]  RgSchCellCb        *cell
 *  @param[in]  RgSchCmnDlRbAllocInfo *allocInfo
 *  @return  Void
 **/

#ifdef ANSI
PUBLIC Void rgSCHCmnNonDlfsRbAlloc
(
RgSchCellCb           *cell,
RgSchCmnDlRbAllocInfo *allocInfo
)
#else
PUBLIC Void rgSCHCmnNonDlfsRbAlloc(cell, allocInfo)
RgSchCellCb           *cell;
RgSchCmnDlRbAllocInfo *allocInfo;
#endif
{
   U8                 raRspCnt = 0;
   RgSchDlRbAlloc     *reqAllocInfo;
   TRC2(rgSCHCmnNonDlfsRbAlloc);

   /* Allocate for BCCH on DLSCH */
   reqAllocInfo = &(allocInfo->bcchAlloc);
   if (reqAllocInfo->rbsReq)
   {
      rgSCHCmnNonDlfsCmnRbAlloc(cell, reqAllocInfo);
   }

   /* Allocate for PCCH */
   reqAllocInfo = &(allocInfo->pcchAlloc);
   if (reqAllocInfo->rbsReq)
   {
      rgSCHCmnNonDlfsCmnRbAlloc(cell, reqAllocInfo);
   }

   /* Allocate for MSG4 retransmissions */
   if (allocInfo->msg4Alloc.msg4RetxLst.count)
   {
      rgSCHCmnNonDlfsMsg4Alloc(cell, &(allocInfo->msg4Alloc), TRUE);
   }

   /* Allocate for MSG4 transmissions */
   /* Assuming all the nodes in the list need allocations: rbsReq is valid */
   if (allocInfo->msg4Alloc.msg4TxLst.count)
   {
      rgSCHCmnNonDlfsMsg4Alloc(cell, &(allocInfo->msg4Alloc), FALSE);
   }
#ifdef RGR_V1
   /* Allocate for CCCH SDU (received after guard timer expiry)
    * retransmissions */
   if (allocInfo->ccchSduAlloc.ccchSduRetxLst.count)
   {
      rgSCHCmnNonDlfsCcchSduAlloc(cell, &(allocInfo->ccchSduAlloc), TRUE);
   }

   /* Allocate for CCCD SDU transmissions */
   /* Allocate for CCCH SDU (received after guard timer expiry) transmissions */
   if (allocInfo->ccchSduAlloc.ccchSduTxLst.count)
   {
      rgSCHCmnNonDlfsCcchSduAlloc(cell, &(allocInfo->ccchSduAlloc), FALSE);
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
      if ((rgSCHCmnNonDlfsCmnRbAlloc(cell, reqAllocInfo)) != ROK)
      {
         break;
      }
   }

   /* Allocate for re-transmitting UEs */
   /* ccpu00109625: Incorrect dlsf pointer
    * assignment correction.*/
#ifdef LTEMAC_SPS
   /* Allocate for re-transmitting SPS services */

    if((allocInfo->dedAlloc.retxSpsHqPLst.count))
   {
      rgSCHCmnNonDlfsDedRbAlloc(cell, &(allocInfo->dedAlloc),
            &(allocInfo->dedAlloc.retxSpsHqPLst),
            &(allocInfo->dedAlloc.schdRetxSpsHqPLst),
            &(allocInfo->dedAlloc.nonSchdRetxSpsHqPLst));
   }
   /* Allocate for transmitting SPS services */
     if(     (allocInfo->dedAlloc.txSpsHqPLst.count))
   {
      rgSCHCmnNonDlfsDedRbAlloc(cell, &(allocInfo->dedAlloc),
            &(allocInfo->dedAlloc.txSpsHqPLst),
            &(allocInfo->dedAlloc.schdTxSpsHqPLst),
            &(allocInfo->dedAlloc.nonSchdTxSpsHqPLst));
   }

#endif /* LTEMAC_SPS */
   /* Allocate for RETX+TX UEs */
   if(allocInfo->dedAlloc.txRetxHqPLst.count)
   {
      rgSCHCmnNonDlfsDedRbAlloc(cell, &(allocInfo->dedAlloc),
            &(allocInfo->dedAlloc.txRetxHqPLst),
            &(allocInfo->dedAlloc.schdTxRetxHqPLst),
            &(allocInfo->dedAlloc.nonSchdTxRetxHqPLst));
   }

    if(      (allocInfo->dedAlloc.retxHqPLst.count))
   {
      rgSCHCmnNonDlfsDedRbAlloc(cell, &(allocInfo->dedAlloc),
            &(allocInfo->dedAlloc.retxHqPLst),
            &(allocInfo->dedAlloc.schdRetxHqPLst),
            &(allocInfo->dedAlloc.nonSchdRetxHqPLst));
   }

   /* Allocate for transmitting UEs */
    if(      (allocInfo->dedAlloc.txHqPLst.count))
   {
      rgSCHCmnNonDlfsDedRbAlloc(cell, &(allocInfo->dedAlloc),
            &(allocInfo->dedAlloc.txHqPLst),
            &(allocInfo->dedAlloc.schdTxHqPLst),
            &(allocInfo->dedAlloc.nonSchdTxHqPLst));
   }
  {
      RgSchCmnCell  *cmnCell = RG_SCH_CMN_GET_CELL(cell);
     if ((allocInfo->dedAlloc.txRetxHqPLst.count + 
              allocInfo->dedAlloc.retxHqPLst.count + 
              allocInfo->dedAlloc.txHqPLst.count) > 
           cmnCell->dl.maxUePerDlSf)
     {
        RLOG_ARG4(L_ERROR,DBG_CELLID,cell->cellId,"UEs selected by"
           " scheduler exceed maximumUePerDlSf(%u)tx-retx %ld retx %ld tx %ld",
           cmnCell->dl.maxUePerDlSf, allocInfo->dedAlloc.txRetxHqPLst.count,
           allocInfo->dedAlloc.retxHqPLst.count,
           allocInfo->dedAlloc.txHqPLst.count);
     }
  }
#ifndef LTE_TDD
      /* LTE_ADV_FLAG_REMOVED_START */
      if(cell->lteAdvCb.dsfrCfg.status == RGR_ENABLE)
      {    
           rgSCHCmnNonDlfsDsfrRntpComp(cell, allocInfo->dedAlloc.dedDlSf); 
      }  
      /* LTE_ADV_FLAG_REMOVED_END */
#endif /* LTE_TDD */
   RETVOID;
}

/***********************************************************
 *
 *     Func : rgSCHCmnCalcRiv
 *
 *     Desc : This function calculates RIV.
 *
 *     Ret  : None.
 *
 *     Notes: None.
 *
 *     File : rg_sch_utl.c
 *
 **********************************************************/
#ifdef LTEMAC_SPS
#ifdef ANSI
PUBLIC U32 rgSCHCmnCalcRiv
(
U8           bw,
U8           rbStart,
U8           numRb
)
#else
PUBLIC U32 rgSCHCmnCalcRiv(bw, rbStart, numRb)
U8           bw;
U8           rbStart;
U8           numRb;
#endif
#else
#ifdef ANSI
PUBLIC U32 rgSCHCmnCalcRiv
(
U8           bw,
U8           rbStart,
U8           numRb
)
#else
PUBLIC U32 rgSCHCmnCalcRiv(bw, rbStart, numRb)
U8           bw;
U8           rbStart;
U8           numRb;
#endif
#endif
{
   U8           numRbMinus1 = numRb - 1;
   U32          riv;

   TRC2(rgSCHCmnCalcRiv);

   if (numRbMinus1 <= bw/2)
   {
      riv = bw * numRbMinus1 + rbStart;
   }
   else
   {
      riv = bw * (bw - numRbMinus1) + (bw - rbStart - 1);
   }
   RETVALUE(riv);
} /* rgSCHCmnCalcRiv */

#ifdef LTE_TDD
/**
 * @brief This function allocates and copies the RACH response scheduling
 *        related information into cell control block.
 *
 * @details
 *
 *     Function: rgSCHCmnDlCpyRachInfo
 *     Purpose:  This function allocates and copies the RACH response
 *               scheduling related information into cell control block
 *               for each DL subframe.
 *
 *
 *     Invoked by: Scheduler
 *
 *  @param[in]  RgSchCellCb*           cell
 *  @param[in]  RgSchTddRachRspLst     rachRspLst[][RGSCH_NUM_SUB_FRAMES]
 *  @param[in]  U8                     raArrSz
 *  @return     S16
 *
 **/
#ifdef ANSI
PRIVATE S16 rgSCHCmnDlCpyRachInfo
(
RgSchCellCb                *cell,
RgSchTddRachRspLst         rachRspLst[][RGSCH_NUM_SUB_FRAMES],
U8                         raArrSz
)
#else
PRIVATE S16 rgSCHCmnDlCpyRachInfo(cell, rachRspLst, raArrSz)
RgSchCellCb                *cell;
RgSchTddRachRspLst         rachRspLst[][RGSCH_NUM_SUB_FRAMES];
U8                         raArrSz;
#endif
{
   U8                   ulDlCfgIdx = cell->ulDlCfgIdx;
   U8                   sfNum;
   S16                  sfnIdx;
   U16                  subfrmIdx;
   U8                   numRfs;
   U8                   numSubfrms;
   U8                   sfcount;
   S16                   ret;

   TRC2(rgSCHCmnDlCpyRachInfo);

   /* Allocate RACH response information for each DL
    * subframe in a radio frame */
   ret = rgSCHUtlAllocSBuf(cell->instIdx, (Data **)&cell->rachRspLst,
         rgSchTddNumDlSubfrmTbl[ulDlCfgIdx][RGSCH_NUM_SUB_FRAMES-1] *
         sizeof(RgSchTddRachRspLst));
   if (ret != ROK)
   {
      RETVALUE(ret);
   }

   for(sfnIdx=raArrSz-1; sfnIdx>=0; sfnIdx--)
   {
      for(subfrmIdx=0; subfrmIdx < RGSCH_NUM_SUB_FRAMES; subfrmIdx++)
      {
         subfrmIdx = rgSchTddHighDlSubfrmIdxTbl[ulDlCfgIdx][subfrmIdx];
         if(subfrmIdx == RGSCH_NUM_SUB_FRAMES)
         {
            break;
         }

         RGSCH_ARRAY_BOUND_CHECK(cell->instIdx, rachRspLst[sfnIdx],subfrmIdx);
         numSubfrms =
            rachRspLst[sfnIdx][subfrmIdx].rachRsp[0].numSubfrms;

         RGSCH_ARRAY_BOUND_CHECK(cell->instIdx, rgSchTddNumDlSubfrmTbl[ulDlCfgIdx],subfrmIdx);
         sfNum = rgSchTddNumDlSubfrmTbl[ulDlCfgIdx][subfrmIdx]-1;
         numRfs = cell->rachRspLst[sfNum].numRadiofrms;
         /* For each DL subframe in which RACH response can
          * be sent is updated */
         if(numSubfrms > 0)
         {
            cell->rachRspLst[sfNum].rachRsp[numRfs].sfnOffset =
               rachRspLst[sfnIdx][subfrmIdx].rachRsp[0].sfnOffset;
            for(sfcount=0; sfcount < numSubfrms; sfcount++)
            {
               cell->rachRspLst[sfNum].rachRsp[numRfs].\
                  subframe[sfcount] =
                  rachRspLst[sfnIdx][subfrmIdx].rachRsp[0].\
                  subframe[sfcount];
            }
            cell->rachRspLst[sfNum].rachRsp[numRfs].numSubfrms =
               rachRspLst[sfnIdx][subfrmIdx].rachRsp[0].numSubfrms;
            cell->rachRspLst[sfNum].numRadiofrms++;
         }

         /* Copy the subframes to be deleted at ths subframe */
         numSubfrms =
            rachRspLst[sfnIdx][subfrmIdx].delInfo.numSubfrms;
         if(numSubfrms > 0)
         {
            cell->rachRspLst[sfNum].delInfo.sfnOffset =
               rachRspLst[sfnIdx][subfrmIdx].delInfo.sfnOffset;
            for(sfcount=0; sfcount < numSubfrms; sfcount++)
            {
               cell->rachRspLst[sfNum].delInfo.subframe[sfcount] =
                  rachRspLst[sfnIdx][subfrmIdx].delInfo.subframe[sfcount];
            }
            cell->rachRspLst[sfNum].delInfo.numSubfrms =
               rachRspLst[sfnIdx][subfrmIdx].delInfo.numSubfrms;
         }
      }
   }
   RETVALUE(ROK);
}
#endif
/**
 * @brief This function determines the iTbs based on the new CFI, 
 *        CQI and BLER based delta iTbs 
 *
 * @details
 *
 *     Function: rgSchCmnFetchItbs
 *     Purpose:  Fetch the new iTbs when CFI changes.
 *
 *  @param[in]  RgSchCellCb           *cell
 *  @param[in]  RgSchCmnDlUe          *ueDl
 *  @param[in]  U8                    cqi
 *
 *  @return S32 iTbs
 *
 **/
#ifdef LTE_TDD
#ifdef ANSI
PRIVATE S32 rgSchCmnFetchItbs 
(
RgSchCellCb        *cell,
RgSchCmnDlUe       *ueDl,
RgSchDlSf          *subFrm,
U8                 cqi,
U8                 cfi,
U8                 cwIdx,
U8                 noLyr
)
#else
PRIVATE S32 rgSchCmnFetchItbs (cell, ueDl, subFrm, cqi, cfi, cwIdx, noLyr)
RgSchCellCb        *cell;
RgSchCmnDlUe       *ueDl; 
RgSchDlSf          *subFrm;
U8                 cqi;
U8                 cfi;
U8                 cwIdx;
U8                 noLyr;
#endif
#else
#ifdef ANSI
PRIVATE S32 rgSchCmnFetchItbs 
(
RgSchCellCb        *cell,
RgSchCmnDlUe       *ueDl,
U8                 cqi,
U8                 cfi,
U8                 cwIdx,
U8                 noLyr
)
#else
PRIVATE S32 rgSchCmnFetchItbs (cell, ueDl, cqi, cfi, cwIdx, noLyr)
RgSchCellCb        *cell;
RgSchCmnDlUe       *ueDl; 
U8                 cqi;
U8                 cfi;
U8                 cwIdx;
U8                 noLyr;
#endif 
#endif
{

   RgSchCmnDlCell *cellDl = RG_SCH_CMN_GET_DL_CELL(cell);
   S32 iTbs = 0;

   TRC2(rgSchCmnFetchItbs);

#ifdef LTE_TDD      
   /* Special Handling for Spl Sf when CFI is 3 as 
    * CFI in Spl Sf will be max 2 */
   if(subFrm->sfType == RG_SCH_SPL_SF_DATA) 
   {
      if((cellDl->currCfi == 3) || 
            ((cell->bwCfg.dlTotalBw <= 10) && (cellDl->currCfi == 1)))
      {    
         /* Use CFI 2 in this case */
         iTbs = (ueDl->laCb[cwIdx].deltaiTbs + 
               ((*(RgSchCmnCqiToTbs *)(cellDl->cqiToTbsTbl[0][2]))[cqi])* 100)/100;

         RG_SCH_CHK_ITBS_RANGE(iTbs, RGSCH_NUM_ITBS - 1);
      }
      else
      {
         iTbs = ueDl->mimoInfo.cwInfo[cwIdx].iTbs[noLyr - 1];
      }
      iTbs = RGSCH_MIN(iTbs, cell->thresholds.maxDlItbs);
   }   
   else /* CFI Changed. Update with new iTbs Reset the BLER*/
#endif         
   {
      S32 tmpiTbs  = (*(RgSchCmnCqiToTbs *)(cellDl->cqiToTbsTbl[0][cfi]))[cqi];
      
      iTbs = (ueDl->laCb[cwIdx].deltaiTbs + tmpiTbs*100)/100;

      RG_SCH_CHK_ITBS_RANGE(iTbs, tmpiTbs); 	

      iTbs = RGSCH_MIN(iTbs, cell->thresholds.maxDlItbs);

      ueDl->mimoInfo.cwInfo[cwIdx].iTbs[noLyr - 1] = iTbs;

      ueDl->lastCfi = cfi;
      ueDl->laCb[cwIdx].deltaiTbs = 0;
   }

   RETVALUE(iTbs);
} 

/**
 * @brief This function determines the RBs and Bytes required for BO
 *        transmission for UEs configured with TM 1/2/6/7.
 *
 * @details
 *
 *     Function: rgSCHCmnDlAllocTxRb1Tb1Cw
 *     Purpose:  Allocate TB1 on CW1.
 *
 *               Reference Parameter effBo is filled with alloced bytes.
 *               Returns RFAILED if BO not satisfied at all.
 *
 *     Invoked by: rgSCHCmnDlAllocTxRbTM1/2/6/7
 *
 *  @param[in]  RgSchCellCb           *cell
 *  @param[in]  RgSchDlSf             *subFrm
 *  @param[in]  RgSchUeCb             *ue
 *  @param[in]  U32                   bo
 *  @param[out] U32                   *effBo
 *  @param[in]  RgSchDlHqProcCb       *proc
 *  @param[out] RgSchCmnDlRbAllocInfo *cellWdAllocInfo
 *  @return  Void
 *
 **/
#ifdef ANSI
PRIVATE Void rgSCHCmnDlAllocTxRb1Tb1Cw
(
RgSchCellCb                *cell,
RgSchDlSf                  *subFrm,
RgSchUeCb                  *ue,
U32                        bo,
U32                        *effBo,
RgSchDlHqProcCb            *proc,
RgSchCmnDlRbAllocInfo      *cellWdAllocInfo
)
#else
PRIVATE Void rgSCHCmnDlAllocTxRb1Tb1Cw(cell, subFrm, ue, bo, effBo, proc, cellWdAllocInfo)
RgSchCellCb                *cell;
RgSchDlSf                  *subFrm;
RgSchUeCb                  *ue;
U32                        bo;
U32                        *effBo;
RgSchDlHqProcCb            *proc;
RgSchCmnDlRbAllocInfo      *cellWdAllocInfo;
#endif
{
   RgSchDlRbAlloc   *allocInfo;
   S16              ret;
   U8               numRb;
   TRC2(rgSCHCmnDlAllocTxRb1Tb1Cw);

   ret = ROK;
   allocInfo = RG_SCH_CMN_GET_ALLOCCB_FRM_UE(ue);

   allocInfo->dciFormat = rgSCHCmnSlctPdcchFrmt(cell, ue, \
         &allocInfo->raType);
   ret = rgSCHCmnDlAlloc1CwTxRb(cell, subFrm, ue, &proc->tbInfo[0],\
         bo, &numRb, effBo);
   if (ret == RFAILED)
   {
      /* If allocation couldn't be made then return */
      RETVOID;
   }
   /* Adding UE to RbAllocInfo TX Lst */
#ifdef LTEMAC_SPS
   if (!RG_SCH_CMN_SPS_DL_IS_SPS_HQP(proc))
#endif
   {
      rgSCHCmnDlRbInfoAddUeTx(cellWdAllocInfo, ue, proc);
   }
   /* Fill UE alloc Info */
   allocInfo->rbsReq = numRb;
   allocInfo->dlSf   = subFrm;

   RETVOID;
}


/**
 * @brief This function determines the RBs and Bytes required for BO
 *        retransmission for UEs configured with TM 1/2/6/7.
 *
 * @details
 *
 *     Function: rgSCHCmnDlAllocRetxRb1Tb1Cw
 *     Purpose:  Allocate TB1 on CW1.
 *
 *               Reference Parameter effBo is filled with alloced bytes.
 *               Returns RFAILED if BO not satisfied at all.
 *
 *     Invoked by: rgSCHCmnDlAllocRetxRbTM1/2/6/7
 *
 *  @param[in]  RgSchCellCb           *cell
 *  @param[in]  RgSchDlSf             *subFrm
 *  @param[in]  RgSchUeCb             *ue
 *  @param[in]  U32                   bo
 *  @param[out] U32                   *effBo
 *  @param[in]  RgSchDlHqProcCb       *proc
 *  @param[out] RgSchCmnDlRbAllocInfo *cellWdAllocInfo
 *  @return  Void
 *
 **/
#ifdef ANSI
PRIVATE Void rgSCHCmnDlAllocRetxRb1Tb1Cw
(
RgSchCellCb                *cell,
RgSchDlSf                  *subFrm,
RgSchUeCb                  *ue,
U32                        bo,
U32                        *effBo,
RgSchDlHqProcCb            *proc,
RgSchCmnDlRbAllocInfo      *cellWdAllocInfo
)
#else
PRIVATE Void rgSCHCmnDlAllocRetxRb1Tb1Cw(cell, subFrm, ue, bo, effBo, proc, cellWdAllocInfo)
RgSchCellCb                *cell;
RgSchDlSf                  *subFrm;
RgSchUeCb                  *ue;
U32                        bo;
U32                        *effBo;
RgSchDlHqProcCb            *proc;
RgSchCmnDlRbAllocInfo      *cellWdAllocInfo;
#endif
{
   RgSchDlRbAlloc   *allocInfo;
   S16              ret;
   U8               numRb;
   TRC2(rgSCHCmnDlAllocRetxRb1Tb1Cw);

   ret = ROK;
   allocInfo = RG_SCH_CMN_GET_ALLOCCB_FRM_UE(ue);

   allocInfo->dciFormat = rgSCHCmnSlctPdcchFrmt(cell, ue, \
         &allocInfo->raType);
   /* Get the Allocation in terms of RBs that are required for
    * this retx of TB1 */
   ret = rgSCHCmnDlAlloc1CwRetxRb(cell, subFrm, ue, &proc->tbInfo[0],
         1, &numRb, effBo);
   if (ret == RFAILED)
   {
      /* Allocation couldn't be made for Retx */
      /* Fix : syed If TxRetx allocation failed then add the UE along with the proc
       * to the nonSchdTxRetxUeLst and let spfc scheduler take care of it during
       * finalization. */        
      rgSCHCmnDlAdd2NonSchdRetxLst(cellWdAllocInfo, ue, proc);
      RETVOID;
   }
   /* Adding UE to allocInfo RETX Lst */
#ifdef LTEMAC_SPS
   if (!RG_SCH_CMN_SPS_DL_IS_SPS_HQP(proc))
#endif
   {
      rgSCHCmnDlRbInfoAddUeRetx(cellWdAllocInfo, ue, proc);
   }
   /* Fill UE alloc Info */
   allocInfo->rbsReq = numRb;
   allocInfo->dlSf   = subFrm;

   RETVOID;
}


/**
 * @brief This function determines the RBs and Bytes required for BO
 *        transmission for UEs configured with TM 2.
 *
 * @details
 *
 *     Function: rgSCHCmnDlAllocTxRbTM1
 *     Purpose:
 *
 *               Reference Parameter effBo is filled with alloced bytes.
 *               Returns RFAILED if BO not satisfied at all.
 *
 *     Invoked by: rgSCHCmnDlAllocTxRb
 *
 *  @param[in]  RgSchCellCb           *cell
 *  @param[in]  RgSchDlSf             *subFrm
 *  @param[in]  RgSchUeCb             *ue
 *  @param[in]  U32                   bo
 *  @param[out] U32                   *effBo
 *  @param[in]  RgSchDlHqProcCb       *proc
 *  @param[out] RgSchCmnDlRbAllocInfo *cellWdAllocInfo
 *  @return Void
 *
 **/
#ifdef ANSI
PRIVATE Void rgSCHCmnDlAllocTxRbTM1
(
RgSchCellCb                *cell,
RgSchDlSf                  *subFrm,
RgSchUeCb                  *ue,
U32                        bo,
U32                        *effBo,
RgSchDlHqProcCb            *proc,
RgSchCmnDlRbAllocInfo      *cellWdAllocInfo
)
#else
PRIVATE Void rgSCHCmnDlAllocTxRbTM1(cell, subFrm, ue, bo, effBo, proc, cellWdAllocInfo)
RgSchCellCb                *cell;
RgSchDlSf                  *subFrm;
RgSchUeCb                  *ue;
U32                        bo;
U32                        *effBo;
RgSchDlHqProcCb            *proc;
RgSchCmnDlRbAllocInfo      *cellWdAllocInfo;
#endif
{
   TRC2(rgSCHCmnDlAllocTxRbTM1);
   rgSCHCmnDlAllocTxRb1Tb1Cw(cell, subFrm, ue, bo, effBo, proc, cellWdAllocInfo);
   RETVOID;
}


/**
 * @brief This function determines the RBs and Bytes required for BO
 *        retransmission for UEs configured with TM 2.
 *
 * @details
 *
 *     Function: rgSCHCmnDlAllocRetxRbTM1
 *     Purpose:
 *
 *               Reference Parameter effBo is filled with alloced bytes.
 *               Returns RFAILED if BO not satisfied at all.
 *
 *     Invoked by: rgSCHCmnDlAllocRetxRb
 *
 *  @param[in]  RgSchCellCb           *cell
 *  @param[in]  RgSchDlSf             *subFrm
 *  @param[in]  RgSchUeCb             *ue
 *  @param[in]  U32                   bo
 *  @param[out] U32                   *effBo
 *  @param[in]  RgSchDlHqProcCb       *proc
 *  @param[out] RgSchCmnDlRbAllocInfo *cellWdAllocInfo
 *  @return Void
 *
 **/
#ifdef ANSI
PRIVATE Void rgSCHCmnDlAllocRetxRbTM1
(
RgSchCellCb                *cell,
RgSchDlSf                  *subFrm,
RgSchUeCb                  *ue,
U32                        bo,
U32                        *effBo,
RgSchDlHqProcCb            *proc,
RgSchCmnDlRbAllocInfo      *cellWdAllocInfo
)
#else
PRIVATE Void rgSCHCmnDlAllocRetxRbTM1(cell, subFrm, ue, bo, effBo, proc, cellWdAllocInfo)
RgSchCellCb                *cell;
RgSchDlSf                  *subFrm;
RgSchUeCb                  *ue;
U32                        bo;
U32                        *effBo;
RgSchDlHqProcCb            *proc;
RgSchCmnDlRbAllocInfo      *cellWdAllocInfo;
#endif
{
   TRC2(rgSCHCmnDlAllocRetxRbTM1);
   rgSCHCmnDlAllocRetxRb1Tb1Cw(cell, subFrm, ue, bo, effBo, proc, cellWdAllocInfo);
   RETVOID;
}


/**
 * @brief This function determines the RBs and Bytes required for BO
 *        transmission for UEs configured with TM 2.
 *
 * @details
 *
 *     Function: rgSCHCmnDlAllocTxRbTM2
 *     Purpose:
 *
 *               Reference Parameter effBo is filled with alloced bytes.
 *               Returns RFAILED if BO not satisfied at all.
 *
 *     Invoked by: rgSCHCmnDlAllocTxRb
 *
 *  @param[in]  RgSchCellCb           *cell
 *  @param[in]  RgSchDlSf             *subFrm
 *  @param[in]  RgSchUeCb             *ue
 *  @param[in]  U32                   bo
 *  @param[out] U32                   *effBo
 *  @param[in]  RgSchDlHqProcCb       *proc
 *  @param[out] RgSchCmnDlRbAllocInfo *cellWdAllocInfo
 *  @return Void
 *
 **/
#ifdef ANSI
PRIVATE Void rgSCHCmnDlAllocTxRbTM2
(
RgSchCellCb                *cell,
RgSchDlSf                  *subFrm,
RgSchUeCb                  *ue,
U32                        bo,
U32                        *effBo,
RgSchDlHqProcCb            *proc,
RgSchCmnDlRbAllocInfo      *cellWdAllocInfo
)
#else
PRIVATE Void rgSCHCmnDlAllocTxRbTM2(cell, subFrm, ue, bo, effBo, proc, cellWdAllocInfo)
RgSchCellCb                *cell;
RgSchDlSf                  *subFrm;
RgSchUeCb                  *ue;
U32                        bo;
U32                        *effBo;
RgSchDlHqProcCb            *proc;
RgSchCmnDlRbAllocInfo      *cellWdAllocInfo;
#endif
{
   TRC2(rgSCHCmnDlAllocTxRbTM2);
   rgSCHCmnDlAllocTxRb1Tb1Cw(cell, subFrm, ue, bo, effBo, proc, cellWdAllocInfo);
   RETVOID;
}


/**
 * @brief This function determines the RBs and Bytes required for BO
 *        retransmission for UEs configured with TM 2.
 *
 * @details
 *
 *     Function: rgSCHCmnDlAllocRetxRbTM2
 *     Purpose:
 *
 *               Reference Parameter effBo is filled with alloced bytes.
 *               Returns RFAILED if BO not satisfied at all.
 *
 *     Invoked by: rgSCHCmnDlAllocRetxRb
 *
 *  @param[in]  RgSchCellCb           *cell
 *  @param[in]  RgSchDlSf             *subFrm
 *  @param[in]  RgSchUeCb             *ue
 *  @param[in]  U32                   bo
 *  @param[out] U32                   *effBo
 *  @param[in]  RgSchDlHqProcCb       *proc
 *  @param[out] RgSchCmnDlRbAllocInfo *cellWdAllocInfo
 *  @return Void
 *
 **/
#ifdef ANSI
PRIVATE Void rgSCHCmnDlAllocRetxRbTM2
(
RgSchCellCb                *cell,
RgSchDlSf                  *subFrm,
RgSchUeCb                  *ue,
U32                        bo,
U32                        *effBo,
RgSchDlHqProcCb            *proc,
RgSchCmnDlRbAllocInfo      *cellWdAllocInfo
)
#else
PRIVATE Void rgSCHCmnDlAllocRetxRbTM2(cell, subFrm, ue, bo, effBo, proc, cellWdAllocInfo)
RgSchCellCb                *cell;
RgSchDlSf                  *subFrm;
RgSchUeCb                  *ue;
U32                        bo;
U32                        *effBo;
RgSchDlHqProcCb            *proc;
RgSchCmnDlRbAllocInfo      *cellWdAllocInfo;
#endif
{
   TRC2(rgSCHCmnDlAllocRetxRbTM2);
   rgSCHCmnDlAllocRetxRb1Tb1Cw(cell, subFrm, ue, bo, effBo, proc, cellWdAllocInfo);
   RETVOID;
}


/**
 * @brief This function determines the RBs and Bytes required for BO
 *        transmission for UEs configured with TM 3.
 *
 * @details
 *
 *     Function: rgSCHCmnDlAllocTxRbTM3
 *     Purpose:
 *
 *               Reference Parameter effBo is filled with alloced bytes.
 *               Returns RFAILED if BO not satisfied at all.
 *
 *     Invoked by: rgSCHCmnDlAllocTxRb
 *
 *  @param[in]  RgSchCellCb           *cell
 *  @param[in]  RgSchDlSf             *subFrm
 *  @param[in]  RgSchUeCb             *ue
 *  @param[in]  U32                   bo
 *  @param[out] U32                   *effBo
 *  @param[in]  RgSchDlHqProcCb       *proc
 *  @param[out] RgSchCmnDlRbAllocInfo *cellWdAllocInfo
 *  @return Void
 *
 **/
#ifdef ANSI
PRIVATE Void rgSCHCmnDlAllocTxRbTM3
(
RgSchCellCb                *cell,
RgSchDlSf                  *subFrm,
RgSchUeCb                  *ue,
U32                        bo,
U32                        *effBo,
RgSchDlHqProcCb            *proc,
RgSchCmnDlRbAllocInfo      *cellWdAllocInfo
)
#else
PRIVATE Void rgSCHCmnDlAllocTxRbTM3(cell, subFrm, ue, bo, effBo, proc, cellWdAllocInfo)
RgSchCellCb                *cell;
RgSchDlSf                  *subFrm;
RgSchUeCb                  *ue;
U32                        bo;
U32                        *effBo;
RgSchDlHqProcCb            *proc;
RgSchCmnDlRbAllocInfo      *cellWdAllocInfo;
#endif
{

   TRC2(rgSCHCmnDlAllocTxRbTM3);

   /* Both TBs free for TX allocation */
   rgSCHCmnDlTM3TxTx(cell, subFrm, ue, bo, effBo,\
         proc, cellWdAllocInfo);

   RETVOID;
}


/**
 * @brief This function determines the RBs and Bytes required for BO
 *        retransmission for UEs configured with TM 3.
 *
 * @details
 *
 *     Function: rgSCHCmnDlAllocRetxRbTM3
 *     Purpose:
 *
 *               Reference Parameter effBo is filled with alloced bytes.
 *               Returns RFAILED if BO not satisfied at all.
 *
 *     Invoked by: rgSCHCmnDlAllocRetxRb
 *
 *  @param[in]  RgSchCellCb           *cell
 *  @param[in]  RgSchDlSf             *subFrm
 *  @param[in]  RgSchUeCb             *ue
 *  @param[in]  U32                   bo
 *  @param[out] U32                   *effBo
 *  @param[in]  RgSchDlHqProcCb       *proc
 *  @param[out] RgSchCmnDlRbAllocInfo *cellWdAllocInfo
 *  @return Void
 *
 **/
#ifdef ANSI
PRIVATE Void rgSCHCmnDlAllocRetxRbTM3
(
RgSchCellCb                *cell,
RgSchDlSf                  *subFrm,
RgSchUeCb                  *ue,
U32                        bo,
U32                        *effBo,
RgSchDlHqProcCb            *proc,
RgSchCmnDlRbAllocInfo      *cellWdAllocInfo
)
#else
PRIVATE Void rgSCHCmnDlAllocRetxRbTM3(cell, subFrm, ue, bo, effBo, proc, cellWdAllocInfo)
RgSchCellCb                *cell;
RgSchDlSf                  *subFrm;
RgSchUeCb                  *ue;
U32                        bo;
U32                        *effBo;
RgSchDlHqProcCb            *proc;
RgSchCmnDlRbAllocInfo      *cellWdAllocInfo;
#endif
{

   TRC2(rgSCHCmnDlAllocRetxRbTM3);

   if ((proc->tbInfo[0].state == HQ_TB_NACKED) &&
         (proc->tbInfo[1].state == HQ_TB_NACKED))
   {
      /* Both TBs require RETX allocation */
      rgSCHCmnDlTM3RetxRetx(cell, subFrm, ue, bo, effBo,\
            proc, cellWdAllocInfo);
   }
   else
   {
      /* One of the TBs need RETX allocation. Other TB may/maynot
       * be available for new TX allocation. */
      rgSCHCmnDlTM3TxRetx(cell, subFrm, ue, bo, effBo,\
            proc, cellWdAllocInfo);
   }

   RETVOID;
}


/**
 * @brief This function performs the DCI format selection in case of
 *        Transmit Diversity scheme where there can be more
 *        than 1 option for DCI format selection.
 *
 * @details
 *
 *     Function: rgSCHCmnSlctPdcchFrmt
 *     Purpose:  1. If DLFS is enabled, then choose TM specific
 *                  DCI format for Transmit diversity. All the
 *                  TM Specific DCI Formats support Type0 and/or
 *                  Type1 resource allocation scheme. DLFS
 *                  supports only Type-0&1 Resource allocation.
 *               2. If DLFS is not enabled, select a DCI format
 *                  which is of smaller size. Since Non-DLFS
 *                  scheduler supports all Resource allocation
 *                  schemes, selection is based on efficiency.
 *
 *     Invoked by: DL UE Allocation by Common Scheduler.
 *
 *  @param[in]  RgSchCellCb      *cell
 *  @param[in]  RgSchUeCb        *ue
 *  @param[out] U8               *raType
 *  @return  TfuDciFormat
 *
 **/
#ifdef ANSI
PRIVATE TfuDciFormat rgSCHCmnSlctPdcchFrmt
(
RgSchCellCb                *cell,
RgSchUeCb                  *ue,
U8                         *raType
)
#else
PRIVATE TfuDciFormat rgSCHCmnSlctPdcchFrmt(cell, ue, raType)
RgSchCellCb                *cell;
RgSchUeCb                  *ue;
U8                         *raType;
#endif
{
   RgSchCmnCell   *cellSch = RG_SCH_CMN_GET_CELL(cell);

   TRC2(rgSCHCmnSlctPdcchFrmt);

   /* ccpu00140894- Selective DCI Format and RA type should be selected only 
    * after TX Mode transition is completed*/
   if ((cellSch->dl.isDlFreqSel) && (ue->txModeTransCmplt))
   {
      *raType = rgSchCmnDciFrmtOptns[ue->mimoInfo.txMode-1].spfcDciRAType;
      RETVALUE(rgSchCmnDciFrmtOptns[ue->mimoInfo.txMode-1].spfcDciFrmt);
   }
   else
   {
      *raType = rgSchCmnDciFrmtOptns[ue->mimoInfo.txMode-1].prfrdDciRAType;
      RETVALUE(rgSchCmnDciFrmtOptns[ue->mimoInfo.txMode-1].prfrdDciFrmt);
   }
}


/**
 * @brief This function handles Retx allocation in case of TM3 UEs
 *        where both the TBs were NACKED previously.
 *
 * @details
 *
 *     Function: rgSCHCmnDlTM3RetxRetx
 *     Purpose:  If forceTD flag enabled
 *                  TD for TB1 on CW1.
 *               Else
 *                  DCI Frmt 2A and RA Type 0
 *                  RI layered SM of both TBs on 2 CWs
 *               Add UE to cell Alloc Info.
 *               Fill UE alloc Info.
 *
 *
 *               Successful allocation is indicated by non-zero effBo value.
 *
 *     Invoked by: rgSCHCmnDlAllocRbTM3
 *
 *  @param[in]  RgSchCellCb           *cell
 *  @param[in]  RgSchDlSf             *subFrm
 *  @param[in]  RgSchUeCb             *ue
 *  @param[in]  U32                   bo
 *  @param[out] U32                   *effBo
 *  @param[in]  RgSchDlHqProcCb       *proc
 *  @param[out] RgSchCmnDlRbAllocInfo *cellWdAllocInfo
 *  @return  Void
 *
 **/
#ifdef ANSI
PRIVATE Void rgSCHCmnDlTM3RetxRetx
(
RgSchCellCb                *cell,
RgSchDlSf                  *subFrm,
RgSchUeCb                  *ue,
U32                        bo,
U32                        *effBo,
RgSchDlHqProcCb            *proc,
RgSchCmnDlRbAllocInfo      *cellWdAllocInfo
)
#else
PRIVATE Void rgSCHCmnDlTM3RetxRetx(cell, subFrm, ue, bo, effBo, proc, cellWdAllocInfo)
RgSchCellCb                *cell;
RgSchDlSf                  *subFrm;
RgSchUeCb                  *ue;
U32                        bo;
U32                        *effBo;
RgSchDlHqProcCb            *proc;
RgSchCmnDlRbAllocInfo      *cellWdAllocInfo;
#endif
{
   S16           ret;
   RgSchDlRbAlloc *allocInfo;
   U8            numRb;
   Bool          swpFlg;
   U8            precInfo;
   U8            noTxLyrs;
   U8            precInfoAntIdx;

   TRC2(rgSCHCmnDlTM3RetxRetx);

   ret = ROK;
   allocInfo = RG_SCH_CMN_GET_ALLOCCB_FRM_UE(ue);
   swpFlg = FALSE;
/* Fix for ccpu00123927: Retransmit 2 codewords irrespective of current rank */
   {
      allocInfo->dciFormat = TFU_DCI_FORMAT_2A;
      allocInfo->raType    = RG_SCH_CMN_RA_TYPE0;

      ret = rgSCHCmnDlAlloc2CwRetxRb(cell, subFrm, ue, proc, &numRb, &swpFlg,\
            effBo);
      if (ret == RFAILED)
      {
         /* Allocation couldn't be made for Retx */
         rgSCHCmnDlAdd2NonSchdRetxLst(cellWdAllocInfo, ue, proc);
         RETVOID;
      }
      /* Fix for ccpu00123927: Retransmit 2 codewords irrespective of current rank */
      noTxLyrs = proc->tbInfo[0].numLyrs + proc->tbInfo[1].numLyrs;
      precInfoAntIdx = cell->numTxAntPorts/2 - 1;
      precInfo = (getPrecInfoFunc[0][precInfoAntIdx])(ue, noTxLyrs, TRUE);
   }

#ifdef LTEMAC_SPS
   if (!RG_SCH_CMN_SPS_DL_IS_SPS_HQP(proc))
#endif
   {
      /* Adding UE to allocInfo RETX Lst */
      rgSCHCmnDlRbInfoAddUeRetx(cellWdAllocInfo, ue, proc);
   }
   /* Fill UE alloc Info scratch pad */
   RG_SCH_CMN_FILL_DL_TXINFO(allocInfo, numRb, swpFlg, \
         precInfo, noTxLyrs, subFrm);

   RETVOID;
}


/**
 * @brief This function handles Retx allocation in case of TM4 UEs
 *        where both the TBs were NACKED previously.
 *
 * @details
 *
 *     Function: rgSCHCmnDlTM4RetxRetx
 *     Purpose:  If forceTD flag enabled
 *                  TD for TB1 on CW1.
 *               Else
 *                  DCI Frmt 2 and RA Type 0
 *                  If RI == 1
 *                     1 layer SM of TB1 on CW1.
 *                  Else
 *                     RI layered SM of both TBs on 2 CWs
 *               Add UE to cell Alloc Info.
 *               Fill UE alloc Info.
 *
 *
 *               Successful allocation is indicated by non-zero effBo value.
 *
 *     Invoked by: rgSCHCmnDlAllocRbTM4
 *
 *  @param[in]  RgSchCellCb           *cell
 *  @param[in]  RgSchDlSf             *subFrm
 *  @param[in]  RgSchUeCb             *ue
 *  @param[in]  U32                   bo
 *  @param[out] U32                   *effBo
 *  @param[in]  RgSchDlHqProcCb       *proc
 *  @param[out] RgSchCmnDlRbAllocInfo *cellWdAllocInfo
 *  @return  Void
 *
 **/
#ifdef ANSI
PRIVATE Void rgSCHCmnDlTM4RetxRetx
(
RgSchCellCb                *cell,
RgSchDlSf                  *subFrm,
RgSchUeCb                  *ue,
U32                        bo,
U32                        *effBo,
RgSchDlHqProcCb            *proc,
RgSchCmnDlRbAllocInfo      *cellWdAllocInfo
)
#else
PRIVATE Void rgSCHCmnDlTM4RetxRetx(cell, subFrm, ue, bo, effBo, proc, cellWdAllocInfo)
RgSchCellCb                *cell;
RgSchDlSf                  *subFrm;
RgSchUeCb                  *ue;
U32                        bo;
U32                        *effBo;
RgSchDlHqProcCb            *proc;
RgSchCmnDlRbAllocInfo      *cellWdAllocInfo;
#endif
{
   S16            ret;
   RgSchCmnDlUe   *ueDl;
   RgSchDlRbAlloc *allocInfo;
   U8             numRb;
   Bool           swpFlg = FALSE;
   U8             precInfo;
   //U8             precInfoAntIdx;
   U8             noTxLyrs;

   TRC2(rgSCHCmnDlTM4RetxRetx);

   ret = ROK;
   ueDl =  RG_SCH_CMN_GET_DL_UE(ue);
   allocInfo = RG_SCH_CMN_GET_ALLOCCB_FRM_UE(ue);
                       
   /* Irrespective of RI Schedule both CWs */
   allocInfo->dciFormat = TFU_DCI_FORMAT_2;
   allocInfo->raType    = RG_SCH_CMN_RA_TYPE0;

   ret = rgSCHCmnDlAlloc2CwRetxRb(cell, subFrm, ue, proc, &numRb, &swpFlg,\
         effBo);
   if (ret == RFAILED)
   {
      /* Allocation couldn't be made for Retx */
      rgSCHCmnDlAdd2NonSchdRetxLst(cellWdAllocInfo, ue, proc);
      RETVOID;
   }
   noTxLyrs = proc->tbInfo[0].numLyrs + proc->tbInfo[1].numLyrs;
   precInfo = 0; 
#ifdef LTEMAC_SPS
   if (!RG_SCH_CMN_SPS_DL_IS_SPS_HQP(proc))
#endif
   {
      /* Adding UE to allocInfo RETX Lst */
      rgSCHCmnDlRbInfoAddUeRetx(cellWdAllocInfo, ue, proc);
   }
   /* Fill UE alloc Info scratch pad */
   RG_SCH_CMN_FILL_DL_TXINFO(allocInfo, numRb, swpFlg, \
         precInfo, noTxLyrs, subFrm);

   RETVOID;
}



/**
 * @brief This function determines Transmission attributes
 *        incase of Spatial multiplexing for TX and RETX TBs.
 *
 * @details
 *
 *     Function: rgSCHCmnDlSMGetAttrForTxRetx
 *     Purpose:  1. Reached here for a TM3/4 UE's HqP whose one of the TBs is
 *                  NACKED and the other TB is either NACKED or WAITING.
 *               2. Select the NACKED TB for RETX allocation.
 *               3. Allocation preference for RETX TB by mapping it to a better
 *                  CW (better in terms of efficiency).
 *               4. Determine the state of the other TB.
 *                  Determine if swapFlag were to be set.
 *                  Swap flag would be set if Retx TB is cross
 *                  mapped to a CW.
 *               5. If UE has new data available for TX and if the other TB's state
 *                  is ACKED then set furtherScope as TRUE.
 *
 *     Invoked by: rgSCHCmnDlTM3[4]TxRetx
 *
 *  @param[in]  RgSchUeCb        *ue
 *  @param[in]  RgSchDlHqProcCb  *proc
 *  @param[out] RgSchDlHqTbCb    **retxTb
 *  @param[out] RgSchDlHqTbCb    **txTb
 *  @param[out] Bool             *frthrScp
 *  @param[out] Bool             *swpFlg
 *  @return  Void
 *
 **/
#ifdef ANSI
PRIVATE Void rgSCHCmnDlSMGetAttrForTxRetx
(
RgSchUeCb                  *ue,
RgSchDlHqProcCb            *proc,
RgSchDlHqTbCb              **retxTb,
RgSchDlHqTbCb              **txTb,
Bool                       *frthrScp,
Bool                       *swpFlg
)
#else
PRIVATE Void rgSCHCmnDlSMGetAttrForTxRetx(ue, proc, retxTb, txTb, frthrScp,\
        swpFlg)
RgSchUeCb                  *ue;
RgSchDlHqProcCb            *proc;
RgSchDlHqTbCb              **retxTb;
RgSchDlHqTbCb              **txTb;
Bool                       *frthrScp;
Bool                       *swpFlg;
#endif
{
   RgSchCmnDlUe  *ueDl = RG_SCH_CMN_GET_DL_UE(ue);

   TRC2(rgSCHCmnDlSMGetAttrForTxRetx);

   if (proc->tbInfo[0].state == HQ_TB_NACKED)
   {
      *retxTb = &proc->tbInfo[0];
      *txTb = &proc->tbInfo[1];
      /* TENB_BRDCM_TM4- Currently disabling swapflag for TM3/TM4, since 
       * HqFeedback processing does not consider a swapped hq feedback */
      if ((ue->mimoInfo.txMode == RGR_UE_TM_4) && (ueDl->mimoInfo.btrCwIdx == 1))
      {
         *swpFlg = TRUE;
         proc->cwSwpEnabled = TRUE;
      }
      if (proc->tbInfo[1].state == HQ_TB_ACKED)
      {
         *frthrScp = ue->dl.dlAllocCb.mimoAllocInfo.hasNewTxData;
      }
   }
   else
   {
      *retxTb = &proc->tbInfo[1];
      *txTb = &proc->tbInfo[0];
      /* TENB_BRDCM_TM4 - Currently disabling swapflag for TM3/TM4, since 
       * HqFeedback processing does not consider a swapped hq feedback */
      if ((ue->mimoInfo.txMode == RGR_UE_TM_4) && (ueDl->mimoInfo.btrCwIdx == 0))
      {
         *swpFlg = TRUE;
         proc->cwSwpEnabled = TRUE;
      }
      if (proc->tbInfo[0].state == HQ_TB_ACKED)
      {
         *frthrScp = ue->dl.dlAllocCb.mimoAllocInfo.hasNewTxData;
      }
   }
   RETVOID;
}


/**
 * @brief Determine Precoding information for TM3 2 TX Antenna.
 *
 * @details
 *
 *     Function: rgSCHCmnDlTM3PrecInf2
 *     Purpose:
 *
 *     Invoked by: rgSCHCmnDlGetAttrForTM3
 *
 *  @param[in]  RgSchUeCb        *ue
 *  @param[in]  U8               numTxLyrs
 *  @param[in]  Bool             bothCwEnbld
 *  @return  U8
 *
 **/
#ifdef ANSI
PRIVATE U8 rgSCHCmnDlTM3PrecInf2
(
RgSchUeCb                  *ue,
U8                         numTxLyrs,
Bool                       bothCwEnbld
)
#else
PRIVATE U8 rgSCHCmnDlTM3PrecInf2(ue, numTxLyrs, bothCwEnbld)
RgSchUeCb                  *ue;
U8                         numTxLyrs;
Bool                       bothCwEnbld;
#endif
{
   TRC2(rgSCHCmnDlTM3PrecInf2);

   RETVALUE(0);
}


/**
 * @brief Determine Precoding information for TM4 2 TX Antenna.
 *
 * @details
 *
 *     Function: rgSCHCmnDlTM4PrecInf2
 *     Purpose:  To determine a logic of deriving precoding index
 *               information from 36.212 table 5.3.3.1.5-4
 *
 *     Invoked by: rgSCHCmnDlGetAttrForTM4
 *
 *  @param[in]  RgSchUeCb        *ue
 *  @param[in]  U8               numTxLyrs
 *  @param[in]  Bool             bothCwEnbld
 *  @return  U8
 *
 **/
#ifdef ANSI
PRIVATE U8 rgSCHCmnDlTM4PrecInf2
(
RgSchUeCb                  *ue,
U8                         numTxLyrs,
Bool                       bothCwEnbld
)
#else
PRIVATE U8 rgSCHCmnDlTM4PrecInf2(ue, numTxLyrs, bothCwEnbld)
RgSchUeCb                  *ue;
U8                         numTxLyrs;
Bool                       bothCwEnbld;
#endif
{
   RgSchCmnDlUe  *ueDl = RG_SCH_CMN_GET_DL_UE(ue);
   U8            precIdx;

   TRC2(rgSCHCmnDlTM4PrecInf2);

   if (ueDl->mimoInfo.ri == numTxLyrs)
   {
      if (ueDl->mimoInfo.ri == 2)
      {
         /* PrecInfo corresponding to 2 CW
           Transmission */
         if (ue->mimoInfo.puschFdbkVld)
         {
            precIdx = 2;
         }
         else 
         {
            precIdx = ueDl->mimoInfo.pmi - 1;
         }
      }
      else
      {
         /* PrecInfo corresponding to 1 CW
          * Transmission */
         if (ue->mimoInfo.puschFdbkVld)
         {
            precIdx =  5;
         }
         else
         {
            precIdx =  ueDl->mimoInfo.pmi + 1;
         }
      }
   }
   else if (ueDl->mimoInfo.ri > numTxLyrs)
   {
      /* In case of choosing among the columns of a
       * precoding matrix, choose the column corresponding
       * to the MAX-CQI */
      if (ue->mimoInfo.puschFdbkVld)
      {
         precIdx = 5;
      }
      else
      {
         precIdx = (ueDl->mimoInfo.pmi- 1)* 2  + 1;
      }
   }
   else /* if RI < numTxLyrs */
   {
      precIdx = (ueDl->mimoInfo.pmi < 2)? 0:1;
   }
   RETVALUE(precIdx);
}


/**
 * @brief Determine Precoding information for TM3 4 TX Antenna.
 *
 * @details
 *
 *     Function: rgSCHCmnDlTM3PrecInf4
 *     Purpose:  To determine a logic of deriving precoding index
 *               information from 36.212 table 5.3.3.1.5A-2
 *
 *     Invoked by: rgSCHCmnDlGetAttrForTM3
 *
 *  @param[in]  RgSchUeCb        *ue
 *  @param[in]  U8               numTxLyrs
 *  @param[in]  Bool             bothCwEnbld
 *  @return  U8
 *
 **/
#ifdef ANSI
PRIVATE U8 rgSCHCmnDlTM3PrecInf4
(
RgSchUeCb                  *ue,
U8                         numTxLyrs,
Bool                       bothCwEnbld
)
#else
PRIVATE U8 rgSCHCmnDlTM3PrecInf4(ue, numTxLyrs, bothCwEnbld)
RgSchUeCb                  *ue;
U8                         numTxLyrs;
Bool                       bothCwEnbld;
#endif
{
   U8            precIdx;

   TRC2(rgSCHCmnDlTM3PrecInf4);

   if (bothCwEnbld)
   {
      precIdx = numTxLyrs - 2;
   }
   else /* one 1 CW transmission */
   {
      precIdx = 1;
   }
   RETVALUE(precIdx);
}


/**
 * @brief Determine Precoding information for TM4 4 TX Antenna.
 *
 * @details
 *
 *     Function: rgSCHCmnDlTM4PrecInf4
 *     Purpose:  To determine a logic of deriving precoding index
 *               information from 36.212 table 5.3.3.1.5-5
 *
 *     Invoked by: rgSCHCmnDlGetAttrForTM4
 *
 *  @param[in]  RgSchUeCb        *ue
 *  @param[in]  U8               numTxLyrs
 *  @param[in]  Bool             bothCwEnbld
 *  @return  U8
 *
 **/
#ifdef ANSI
PRIVATE U8 rgSCHCmnDlTM4PrecInf4
(
RgSchUeCb                  *ue,
U8                         numTxLyrs,
Bool                       bothCwEnbld
)
#else
PRIVATE U8 rgSCHCmnDlTM4PrecInf4(ue, numTxLyrs, bothCwEnbld)
RgSchUeCb                  *ue;
U8                         numTxLyrs;
Bool                       bothCwEnbld;
#endif
{
   RgSchCmnDlUe  *ueDl = RG_SCH_CMN_GET_DL_UE(ue);
   U8            precInfoBaseIdx, precIdx;

   TRC2(rgSCHCmnDlTM4PrecInf4);

   precInfoBaseIdx  = (ue->mimoInfo.puschFdbkVld)? (16):
      (ueDl->mimoInfo.pmi);
   if (bothCwEnbld)
   {
      precIdx = precInfoBaseIdx + (numTxLyrs-2)*17;
   }
   else /* one 1 CW transmission */
   {
      precInfoBaseIdx += 1;
      precIdx = precInfoBaseIdx + (numTxLyrs-1)*17;
   }
   RETVALUE(precIdx);
}


/**
 * @brief This function determines Transmission attributes
 *        incase of TM3 scheduling.
 *
 * @details
 *
 *     Function: rgSCHCmnDlGetAttrForTM3
 *     Purpose:  Determine retx TB and tx TB based on TB states.
 *               If forceTD enabled
 *                  perform only retx TB allocation.
 *                  If retxTB == TB2 then DCI Frmt = 2A, RA Type = 0.
 *                  Else DCI Frmt and RA Type based on cell->isDlfsEnbld
 *               If RI == 1
 *                  perform retxTB allocation on CW1.
 *               Else if RI > 1
 *                  Determine further Scope and Swap Flag attributes
 *                  assuming a 2 CW transmission of RetxTB and new Tx TB.
 *                  If no further scope for new TX allocation
 *                     Allocate only retx TB using 2 layers if
 *                     this TB was previously transmitted using 2 layers AND
 *                     number of Tx antenna ports == 4.
 *                     otherwise do single layer precoding.
 *
 *     Invoked by: rgSCHCmnDlTM3TxRetx
 *
 *  @param[in]  RgSchUeCb        *ue
 *  @param[in]  RgSchDlHqProcCb  *proc
 *  @param[out] U8               *numTxLyrs
 *  @param[out] Bool             *isTraDiv
 *  @param[out] U8               *prcdngInf
 *  @param[out] U8               *raType
 *  @return  Void
 *
 **/
#ifdef ANSI
PRIVATE Void rgSCHCmnDlGetAttrForTM3
(
RgSchCellCb                *cell,
RgSchUeCb                  *ue,
RgSchDlHqProcCb            *proc,
U8                         *numTxLyrs,
TfuDciFormat               *dciFrmt,
U8                         *prcdngInf,
RgSchDlHqTbCb              **retxTb,
RgSchDlHqTbCb              **txTb,
Bool                       *frthrScp,
Bool                       *swpFlg,
U8                         *raType
)
#else
PRIVATE Void rgSCHCmnDlGetAttrForTM3(cell, ue, proc, numTxLyrs, dciFrmt,\
        prcdngInf, retxTb, txTb, frthrScp, swpFlg, raType)
RgSchCellCb                *cell;
RgSchUeCb                  *ue;
RgSchDlHqProcCb            *proc;
U8                         *numTxLyrs;
TfuDciFormat               *dciFrmt;
U8                         *prcdngInf;
RgSchDlHqTbCb              **retxTb;
RgSchDlHqTbCb              **txTb;
Bool                       *frthrScp;
Bool                       *swpFlg;
U8                         *raType;
#endif
{
   RgSchCmnDlUe  *ueDl = RG_SCH_CMN_GET_DL_UE(ue);
   U8            precInfoAntIdx;

   TRC2(rgSCHCmnDlGetAttrForTM3);

   /* Integration_fix: SPS Proc shall always have only one Cw */
#ifdef LTEMAC_SPS
   if ((RG_SCH_CMN_SPS_DL_IS_SPS_HQP(proc)) ||
         (ueDl->mimoInfo.forceTD))
#else
   if (ueDl->mimoInfo.forceTD)
#endif
   {
      /* Transmit Diversity. Format based on dlfsEnabled
       * No further scope */
      if (proc->tbInfo[0].state == HQ_TB_NACKED)
      {
         *retxTb = &proc->tbInfo[0];
         *dciFrmt = rgSCHCmnSlctPdcchFrmt(cell, ue, raType);
      }
      else
      {
         *retxTb = &proc->tbInfo[1];
         *dciFrmt = TFU_DCI_FORMAT_2A;
         *raType = RG_SCH_CMN_RA_TYPE0;
      }
      *numTxLyrs = 1;
      *frthrScp = FALSE;
      *prcdngInf = 0;
      RETVOID;
   }

   /* Determine the 2 TB transmission attributes */
   rgSCHCmnDlSMGetAttrForTxRetx(ue, proc, retxTb, txTb, \
         frthrScp, swpFlg);
   if (*frthrScp)
   {
      /* Prefer allocation of RETX TB over 2 layers rather than combining
       * it with a new TX. */
      if ((ueDl->mimoInfo.ri == 2)
            && ((*retxTb)->numLyrs == 2) && (cell->numTxAntPorts == 4))
      {
         /* Allocate TB on CW1, using 2 Lyrs,
          * Format 2, precoding accordingly */
         *numTxLyrs = 2;
         *frthrScp = FALSE;
      }
      precInfoAntIdx = cell->numTxAntPorts/2 - 1; 
      *prcdngInf = (getPrecInfoFunc[0][precInfoAntIdx])\
                   (ue, ueDl->mimoInfo.ri, *frthrScp);
      *dciFrmt = TFU_DCI_FORMAT_2A;
      *raType = RG_SCH_CMN_RA_TYPE0;
   }
   else /* frthrScp == FALSE */
   {
      if (cell->numTxAntPorts == 2)
      {
         /*  Transmit Diversity  */
         *numTxLyrs = 1;
         if ((*retxTb)->tbIdx == 0)
         {
            *dciFrmt = rgSCHCmnSlctPdcchFrmt(cell, ue, raType);
         }
         else
         {
            /* If retxTB is TB2 then use format 2A */
            *dciFrmt = TFU_DCI_FORMAT_2A;
            *raType = RG_SCH_CMN_RA_TYPE0;
         }
         *prcdngInf = 0;
         RETVOID;
      }
      else /* NumAntPorts == 4 */
      {
         if ((*retxTb)->numLyrs == 2)
         {
            /* Allocate TB on CW1, using 2 Lyrs,
             * Format 2A, precoding accordingly */
            *numTxLyrs = 2;
            *dciFrmt = TFU_DCI_FORMAT_2A;
            *raType = RG_SCH_CMN_RA_TYPE0;
            precInfoAntIdx = cell->numTxAntPorts/2 - 1;
            *prcdngInf = (getPrecInfoFunc[0][precInfoAntIdx])(ue, *numTxLyrs, *frthrScp);
            RETVOID;
         }
         else
         {
            /*  Transmit Diversity  */
            *numTxLyrs = 1;
            if ((*retxTb)->tbIdx == 0)
            {
               *dciFrmt = rgSCHCmnSlctPdcchFrmt(cell, ue, raType);
            }
            else
            {
               /* If retxTB is TB2 then use format 2A */
               *dciFrmt = TFU_DCI_FORMAT_2A;
               *raType = RG_SCH_CMN_RA_TYPE0;
            }
            *prcdngInf = 0;
            RETVOID;
         }
      }
   }

   RETVOID;
}



/**
 * @brief This function determines Transmission attributes
 *        incase of TM4 scheduling.
 *
 * @details
 *
 *     Function: rgSCHCmnDlGetAttrForTM4
 *     Purpose:  Determine retx TB and tx TB based on TB states.
 *               If forceTD enabled
 *                  perform only retx TB allocation.
 *                  If retxTB == TB2 then DCI Frmt = 2, RA Type = 0.
 *                  Else DCI Frmt and RA Type based on cell->isDlfsEnbld
 *               If RI == 1
 *                  perform retxTB allocation on CW1.
 *               Else if RI > 1
 *                  Determine further Scope and Swap Flag attributes
 *                  assuming a 2 CW transmission of RetxTB and new Tx TB.
 *                  If no further scope for new TX allocation
 *                     Allocate only retx TB using 2 layers if
 *                     this TB was previously transmitted using 2 layers AND
 *                     number of Tx antenna ports == 4.
 *                     otherwise do single layer precoding.
 *
 *     Invoked by: rgSCHCmnDlTM4TxRetx
 *
 *  @param[in]  RgSchUeCb        *ue
 *  @param[in]  RgSchDlHqProcCb  *proc
 *  @param[out] U8               *numTxLyrs
 *  @param[out] Bool             *isTraDiv
 *  @param[out] U8               *prcdngInf
 *  @param[out] U8               *raType
 *  @return  Void
 *
 **/
#ifdef ANSI
PRIVATE Void rgSCHCmnDlGetAttrForTM4
(
RgSchCellCb                *cell,
RgSchUeCb                  *ue,
RgSchDlHqProcCb            *proc,
U8                         *numTxLyrs,
TfuDciFormat               *dciFrmt,
U8                         *prcdngInf,
RgSchDlHqTbCb              **retxTb,
RgSchDlHqTbCb              **txTb,
Bool                       *frthrScp,
Bool                       *swpFlg,
U8                         *raType
)
#else
PRIVATE Void rgSCHCmnDlGetAttrForTM4(cell, ue, proc, numTxLyrs, dciFrmt,\
        prcdngInf, retxTb, txTb, frthrScp, swpFlg, raType)
RgSchCellCb                *cell;
RgSchUeCb                  *ue;
RgSchDlHqProcCb            *proc;
U8                         *numTxLyrs;
TfuDciFormat               *dciFrmt;
U8                         *prcdngInf;
RgSchDlHqTbCb              **retxTb;
RgSchDlHqTbCb              **txTb;
Bool                       *frthrScp;
Bool                       *swpFlg;
U8                         *raType;
#endif
{
   RgSchCmnDlUe  *ueDl = RG_SCH_CMN_GET_DL_UE(ue);
   U8 precInfoAntIdx;

   TRC2(rgSCHCmnDlGetAttrForTM4);

   *frthrScp = FALSE;
   /* Integration_fix: SPS Proc shall always have only one Cw */
#ifdef LTEMAC_SPS
   if ((RG_SCH_CMN_SPS_DL_IS_SPS_HQP(proc)) ||
         (ueDl->mimoInfo.forceTD))
#else
   if (ueDl->mimoInfo.forceTD)
#endif
   {
      /* Transmit Diversity. Format based on dlfsEnabled
       * No further scope */
      if (proc->tbInfo[0].state == HQ_TB_NACKED)
      {
         *retxTb = &proc->tbInfo[0];
         *dciFrmt = rgSCHCmnSlctPdcchFrmt(cell, ue, raType);
      }
      else
      {
         *retxTb = &proc->tbInfo[1];
         *dciFrmt = TFU_DCI_FORMAT_2;
         *raType = RG_SCH_CMN_RA_TYPE0;
      }
      *numTxLyrs = 1;
      *frthrScp = FALSE;
      *prcdngInf = 0;
      RETVOID;
   }

   if (ueDl->mimoInfo.ri == 1)
   {
      /* single layer precoding. Format 2.
       * No further scope */
      if (proc->tbInfo[0].state == HQ_TB_NACKED)
      {
         *retxTb = &proc->tbInfo[0];
      }
      else
      {
         *retxTb = &proc->tbInfo[1];
      }
      *numTxLyrs = 1;
      *dciFrmt = TFU_DCI_FORMAT_2;
      *raType = RG_SCH_CMN_RA_TYPE0;
      *frthrScp = FALSE;
      *prcdngInf = 0; /*When RI= 1*/
      RETVOID;
   }

   /* Determine the 2 TB transmission attributes */
   rgSCHCmnDlSMGetAttrForTxRetx(ue, proc, retxTb, txTb, \
         frthrScp, swpFlg);
   *dciFrmt = TFU_DCI_FORMAT_2;
   *raType = RG_SCH_CMN_RA_TYPE0;
   if (*frthrScp)
   {
      /* Prefer allocation of RETX TB over 2 layers rather than combining
       * it with a new TX. */
      if ((ueDl->mimoInfo.ri == 2)
            && ((*retxTb)->numLyrs == 2) && (cell->numTxAntPorts == 4))
      {
         /* Allocate TB on CW1, using 2 Lyrs,
          * Format 2, precoding accordingly */
         *numTxLyrs = 2;
         *frthrScp = FALSE;
      }
      precInfoAntIdx = cell->numTxAntPorts/2 - 1;
      *prcdngInf = (getPrecInfoFunc[1][precInfoAntIdx])
                   (ue, ueDl->mimoInfo.ri, *frthrScp);
   }
   else /* frthrScp == FALSE */
   {
      if (cell->numTxAntPorts == 2)
      {
         /* single layer precoding. Format 2. */
         *numTxLyrs = 1;
         *prcdngInf = (getPrecInfoFunc[1][cell->numTxAntPorts/2 - 1])\
                      (ue, *numTxLyrs, *frthrScp);
         RETVOID;
      }
      else /* NumAntPorts == 4 */
      {
         if ((*retxTb)->numLyrs == 2)
         {
            /* Allocate TB on CW1, using 2 Lyrs,
             * Format 2, precoding accordingly */
            *numTxLyrs = 2;
            precInfoAntIdx = cell->numTxAntPorts/2 - 1;
            *prcdngInf = (getPrecInfoFunc[1][precInfoAntIdx])\
                         (ue, *numTxLyrs, *frthrScp);
            RETVOID;
         }
         else
         {
            /* Allocate TB with 1 lyr precoding,
             * Format 2, precoding info accordingly */
            *numTxLyrs = 1;
            precInfoAntIdx = cell->numTxAntPorts/2 - 1;
            *prcdngInf = (getPrecInfoFunc[1][precInfoAntIdx])\
                         (ue, *numTxLyrs, *frthrScp);
            RETVOID;
         }
      }
   }

   RETVOID;
}


/**
 * @brief This function handles Retx allocation in case of TM3 UEs
 *        where previously one of the TBs was NACKED and the other
 *        TB is either ACKED/WAITING.
 *
 * @details
 *
 *     Function: rgSCHCmnDlTM3TxRetx
 *     Purpose:  Determine the TX attributes for TM3 TxRetx Allocation.
 *               If futher Scope for New Tx Allocation on other TB
 *                  Perform RETX alloc'n on 1 CW and TX alloc'n on other.
 *                  Add UE to cell wide RetxTx List.
 *               Else
 *                  Perform only RETX alloc'n on CW1.
 *                  Add UE to cell wide Retx List.
 *
 *               effBo is set to a non-zero value if allocation is
 *               successful.
 *
 *     Invoked by: rgSCHCmnDlAllocRbTM3
 *
 *  @param[in]  RgSchCellCb           *cell
 *  @param[in]  RgSchDlSf             *subFrm
 *  @param[in]  RgSchUeCb             *ue
 *  @param[in]  U32                   bo
 *  @param[out] U32                   *effBo
 *  @param[in]  RgSchDlHqProcCb       *proc
 *  @param[out] RgSchCmnDlRbAllocInfo *cellWdAllocInfo
 *  @return  Void
 *
 **/
#ifdef ANSI
PRIVATE Void rgSCHCmnDlTM3TxRetx
(
RgSchCellCb                *cell,
RgSchDlSf                  *subFrm,
RgSchUeCb                  *ue,
U32                        bo,
U32                        *effBo,
RgSchDlHqProcCb            *proc,
RgSchCmnDlRbAllocInfo      *cellWdAllocInfo
)
#else
PRIVATE Void rgSCHCmnDlTM3TxRetx(cell, subFrm, ue, bo, effBo, proc, cellWdAllocInfo)
RgSchCellCb                *cell;
RgSchDlSf                  *subFrm;
RgSchUeCb                  *ue;
U32                        bo;
U32                        *effBo;
RgSchDlHqProcCb            *proc;
RgSchCmnDlRbAllocInfo      *cellWdAllocInfo;
#endif
{
   S16              ret;
   RgSchDlRbAlloc   *allocInfo;
   U8               numRb;
   RgSchDlHqTbCb    *retxTb, *txTb;
   Bool             frthrScp;
   Bool             swpFlg;
   U8               prcdngInf;
   U8               numTxLyrs;

   TRC2(rgSCHCmnDlTM3TxRetx);
   frthrScp = FALSE;

   ret = ROK;
   allocInfo = RG_SCH_CMN_GET_ALLOCCB_FRM_UE(ue);
   swpFlg = FALSE;

   /* Determine the transmission attributes */
   rgSCHCmnDlGetAttrForTM3(cell, ue, proc, &numTxLyrs, &allocInfo->dciFormat,\
         &prcdngInf, &retxTb, &txTb, &frthrScp, &swpFlg,\
         &allocInfo->raType);
   if (frthrScp)
   {
      ret = rgSCHCmnDlAlloc2CwTxRetxRb(cell, subFrm, ue, retxTb, txTb,\
            &numRb, effBo);
      if (ret == RFAILED)
      {
         /* Allocation couldn't be made for Retx */
         rgSCHCmnDlAdd2NonSchdRetxLst(cellWdAllocInfo, ue, proc);
         RETVOID;
      }
      /* Adding UE to RbAllocInfo RETX-TX Lst */
      rgSCHCmnDlRbInfoAddUeRetxTx(cellWdAllocInfo, ue, proc);
   }
   else
   {
      ret = rgSCHCmnDlAlloc1CwRetxRb(cell, subFrm, ue, retxTb,
            numTxLyrs, &numRb, effBo);
      if (ret == RFAILED)
      {
         /* Allocation couldn't be made for Retx */
         rgSCHCmnDlAdd2NonSchdRetxLst(cellWdAllocInfo, ue, proc);
         RETVOID;
      }
#ifdef LTEMAC_SPS
      if (!RG_SCH_CMN_SPS_DL_IS_SPS_HQP(proc))
#endif
      {
         /* Adding UE to allocInfo RETX Lst */
         rgSCHCmnDlRbInfoAddUeRetx(cellWdAllocInfo, ue, proc);
      }
   }
   RG_SCH_CMN_FILL_DL_TXINFO(allocInfo, numRb, swpFlg, \
         prcdngInf, numTxLyrs, subFrm);

   RETVOID;
}


/**
 * @brief This function handles Retx allocation in case of TM4 UEs
 *        where previously one of the TBs was NACKED and the other
 *        TB is either ACKED/WAITING.
 *
 * @details
 *
 *     Function: rgSCHCmnDlTM4TxRetx
 *     Purpose:  Determine the TX attributes for TM4 TxRetx Allocation.
 *               If futher Scope for New Tx Allocation on other TB
 *                  Perform RETX alloc'n on 1 CW and TX alloc'n on other.
 *                  Add UE to cell wide RetxTx List.
 *               Else
 *                  Perform only RETX alloc'n on CW1.
 *                  Add UE to cell wide Retx List.
 *
 *               effBo is set to a non-zero value if allocation is
 *               successful.
 *
 *     Invoked by: rgSCHCmnDlAllocRbTM4
 *
 *  @param[in]  RgSchCellCb           *cell
 *  @param[in]  RgSchDlSf             *subFrm
 *  @param[in]  RgSchUeCb             *ue
 *  @param[in]  U32                   bo
 *  @param[out] U32                   *effBo
 *  @param[in]  RgSchDlHqProcCb       *proc
 *  @param[out] RgSchCmnDlRbAllocInfo *cellWdAllocInfo
 *  @return  Void
 *
 **/
#ifdef ANSI
PRIVATE Void rgSCHCmnDlTM4TxRetx
(
RgSchCellCb                *cell,
RgSchDlSf                  *subFrm,
RgSchUeCb                  *ue,
U32                        bo,
U32                        *effBo,
RgSchDlHqProcCb            *proc,
RgSchCmnDlRbAllocInfo      *cellWdAllocInfo
)
#else
PRIVATE Void rgSCHCmnDlTM4TxRetx(cell, subFrm, ue, bo, effBo, proc, cellWdAllocInfo)
RgSchCellCb                *cell;
RgSchDlSf                  *subFrm;
RgSchUeCb                  *ue;
U32                        bo;
U32                        *effBo;
RgSchDlHqProcCb            *proc;
RgSchCmnDlRbAllocInfo      *cellWdAllocInfo;
#endif
{
   S16              ret;
   RgSchDlRbAlloc   *allocInfo;
   U8               numRb;
   RgSchDlHqTbCb    *retxTb, *txTb;
   Bool             frthrScp;
   Bool             swpFlg;
   U8               prcdngInf;
   U8               numTxLyrs;

   TRC2(rgSCHCmnDlTM4TxRetx);

   ret = ROK;
   allocInfo = RG_SCH_CMN_GET_ALLOCCB_FRM_UE(ue);
   swpFlg = FALSE;

   /* Determine the transmission attributes */
   rgSCHCmnDlGetAttrForTM4(cell, ue, proc, &numTxLyrs, &allocInfo->dciFormat,\
         &prcdngInf, &retxTb, &txTb, &frthrScp, &swpFlg,\
         &allocInfo->raType);
   if (frthrScp)
   {
      ret = rgSCHCmnDlAlloc2CwTxRetxRb(cell, subFrm, ue, retxTb, txTb,\
            &numRb, effBo);
      if (ret == RFAILED)
      {
         /* Fix : syed If TxRetx allocation failed then add the UE along 
          * with the proc to the nonSchdTxRetxUeLst and let spfc scheduler
          *  take care of it during finalization. */       
         rgSCHCmnDlAdd2NonSchdRetxLst(cellWdAllocInfo, ue, proc);
         RETVOID;
      }
      /* Adding UE to RbAllocInfo RETX-TX Lst */
      rgSCHCmnDlRbInfoAddUeRetxTx(cellWdAllocInfo, ue, proc);
   }
   else
   {
      ret = rgSCHCmnDlAlloc1CwRetxRb(cell, subFrm, ue, retxTb,
            numTxLyrs, &numRb, effBo);
      if (ret == RFAILED)
      {
         /* Allocation couldn't be made for Retx */
         rgSCHCmnDlAdd2NonSchdRetxLst(cellWdAllocInfo, ue, proc);
         RETVOID;
      }
#ifdef LTEMAC_SPS
      if (!RG_SCH_CMN_SPS_DL_IS_SPS_HQP(proc))
#endif
      {
         /* Adding UE to allocInfo RETX Lst */
         rgSCHCmnDlRbInfoAddUeRetx(cellWdAllocInfo, ue, proc);
      }
   }
   RG_SCH_CMN_FILL_DL_TXINFO(allocInfo, numRb, swpFlg, \
         prcdngInf, numTxLyrs, subFrm)

      RETVOID;
}


/**
 * @brief This function handles Retx allocation in case of TM4 UEs
 *        where previously both the TBs were ACKED and ACKED
 *        respectively.
 *
 * @details
 *
 *     Function: rgSCHCmnDlTM3TxTx
 *     Purpose:  Reached here for a TM3 UE's HqP's fresh allocation
 *                  where both the TBs are free for TX scheduling.
 *               If forceTD flag is set
 *                  perform TD on CW1 with TB1.
 *                  precInfo = 0
 *               else
 *                  DCI Format = 2A.
 *                  RA Type = Type0.
 *                  RI layered precoding 2 TB on 2 CW.
 *                  Set precoding info.
 *               Add UE to cellAllocInfo.
 *               Fill ueAllocInfo.
 *
 *              effBo is set to a non-zero value if allocation is
 *              successful.
 *
 *     Invoked by: rgSCHCmnDlAllocRbTM3
 *
 *  @param[in]  RgSchCellCb           *cell
 *  @param[in]  RgSchDlSf             *subFrm
 *  @param[in]  RgSchUeCb             *ue
 *  @param[in]  U32                   bo
 *  @param[out] U32                   *effBo
 *  @param[in]  RgSchDlHqProcCb       *proc
 *  @param[out] RgSchCmnDlRbAllocInfo *cellWdAllocInfo
 *  @return  Void
 *
 **/
#ifdef ANSI
PRIVATE Void rgSCHCmnDlTM3TxTx
(
RgSchCellCb                *cell,
RgSchDlSf                  *subFrm,
RgSchUeCb                  *ue,
U32                        bo,
U32                        *effBo,
RgSchDlHqProcCb            *proc,
RgSchCmnDlRbAllocInfo      *cellWdAllocInfo
)
#else
PRIVATE Void rgSCHCmnDlTM3TxTx(cell, subFrm, ue, bo, effBo, proc, cellWdAllocInfo)
RgSchCellCb                *cell;
RgSchDlSf                  *subFrm;
RgSchUeCb                  *ue;
U32                        bo;
U32                        *effBo;
RgSchDlHqProcCb            *proc;
RgSchCmnDlRbAllocInfo      *cellWdAllocInfo;
#endif
{
   RgSchCmnDlUe     *ueDl;
   RgSchDlRbAlloc   *allocInfo;
   U8               numRb;
   U8               noTxLyrs;
   U8               precInfo;
   S16              ret;
   U8               precInfoAntIdx;

   TRC2(rgSCHCmnDlTM3TxTx);

   ret = ROK;
   ueDl = RG_SCH_CMN_GET_DL_UE(ue);
   allocInfo = RG_SCH_CMN_GET_ALLOCCB_FRM_UE(ue);

   /* Integration_fix: SPS Proc shall always have only one Cw */
#ifdef LTEMAC_SPS
   if ((RG_SCH_CMN_SPS_DL_IS_SPS_HQP(proc)) ||
         (ueDl->mimoInfo.forceTD))
#else
   if (ueDl->mimoInfo.forceTD) /* Transmit Diversity (TD) */
#endif
   {
      allocInfo->dciFormat = rgSCHCmnSlctPdcchFrmt(cell, ue, \
            &allocInfo->raType);
      ret = rgSCHCmnDlAlloc1CwTxRb(cell, subFrm, ue, &proc->tbInfo[0],\
            bo, &numRb, effBo);
      if (ret == RFAILED)
      {
         /* If allocation couldn't be made then return */
         RETVOID;
      }
      noTxLyrs = 1;
      precInfo = 0; /* TD */
   }
   else /* Precoding */
   {
      allocInfo->dciFormat = TFU_DCI_FORMAT_2A;
      allocInfo->raType    = RG_SCH_CMN_RA_TYPE0;

      /* Spatial Multiplexing using 2 CWs */
      ret = rgSCHCmnDlAlloc2CwTxRb(cell, subFrm, ue, proc, bo, &numRb, effBo);
      if (ret == RFAILED)
      {
         /* If allocation couldn't be made then return */
         RETVOID;
      }
      noTxLyrs = ueDl->mimoInfo.ri;
      precInfoAntIdx = cell->numTxAntPorts/2 - 1;
      RGSCH_ARRAY_BOUND_CHECK(cell->instIdx, getPrecInfoFunc[0], precInfoAntIdx);
      precInfo = (getPrecInfoFunc[0][precInfoAntIdx])(ue, noTxLyrs, TRUE);
   }

#ifdef LTEMAC_SPS
   if (!RG_SCH_CMN_SPS_DL_IS_SPS_HQP(proc))
#endif
   {
      /* Adding UE to RbAllocInfo TX Lst */
      rgSCHCmnDlRbInfoAddUeTx(cellWdAllocInfo, ue, proc);
   }
   /* Fill UE allocInfo scrath pad */
   RG_SCH_CMN_FILL_DL_TXINFO(allocInfo, numRb, FALSE, \
         precInfo, noTxLyrs, subFrm);

   RETVOID;
}


/**
 * @brief This function handles Retx allocation in case of TM4 UEs
 *        where previously both the TBs were ACKED and ACKED
 *        respectively.
 *
 * @details
 *
 *     Function: rgSCHCmnDlTM4TxTx
 *     Purpose:  Reached here for a TM4 UE's HqP's fresh allocation
 *                  where both the TBs are free for TX scheduling.
 *               If forceTD flag is set
 *                  perform TD on CW1 with TB1.
 *                  precInfo = 0
 *               else
 *                  DCI Format = 2.
 *                  RA Type = Type0.
 *                  If Rank == 1
 *                     Single layer precoding of TB1 on CW1.
 *                     Set precoding info.
 *                  else
 *                     RI layered precoding 2 TB on 2 CW.
 *                     Set precoding info.
 *               Add UE to cellAllocInfo.
 *               Fill ueAllocInfo.
 *
 *              effBo is set to a non-zero value if allocation is
 *              successful.
 *
 *     Invoked by: rgSCHCmnDlAllocRbTM4
 *
 *  @param[in]  RgSchCellCb           *cell
 *  @param[in]  RgSchDlSf             *subFrm
 *  @param[in]  RgSchUeCb             *ue
 *  @param[in]  U32                   bo
 *  @param[out] U32                   *effBo
 *  @param[in]  RgSchDlHqProcCb       *proc
 *  @param[out] RgSchCmnDlRbAllocInfo *cellWdAllocInfo
 *  @return  Void
 *
 **/
#ifdef ANSI
PRIVATE Void rgSCHCmnDlTM4TxTx
(
RgSchCellCb                *cell,
RgSchDlSf                  *subFrm,
RgSchUeCb                  *ue,
U32                        bo,
U32                        *effBo,
RgSchDlHqProcCb            *proc,
RgSchCmnDlRbAllocInfo      *cellWdAllocInfo
)
#else
PRIVATE Void rgSCHCmnDlTM4TxTx(cell, subFrm, ue, bo, effBo, proc, cellWdAllocInfo)
RgSchCellCb                *cell;
RgSchDlSf                  *subFrm;
RgSchUeCb                  *ue;
U32                        bo;
U32                        *effBo;
RgSchDlHqProcCb            *proc;
RgSchCmnDlRbAllocInfo      *cellWdAllocInfo;
#endif
{
   RgSchCmnDlUe     *ueDl;
   RgSchDlRbAlloc   *allocInfo;
   U8               numRb;
   U8               precInfo;
   U8               noTxLyrs;
   U8               precInfoAntIdx;
   S16              ret;

   TRC2(rgSCHCmnDlTM4TxTx);

   ret       = ROK;
   ueDl      = RG_SCH_CMN_GET_DL_UE(ue);
   allocInfo = RG_SCH_CMN_GET_ALLOCCB_FRM_UE(ue);

   /* Integration_fix: SPS Proc shall always have only one Cw */
#ifdef LTEMAC_SPS
   if ((RG_SCH_CMN_SPS_DL_IS_SPS_HQP(proc)) ||
         (ueDl->mimoInfo.forceTD))
#else
   if (ueDl->mimoInfo.forceTD) /* Transmit Diversity (TD) */
#endif
   {
      allocInfo->dciFormat = rgSCHCmnSlctPdcchFrmt(cell, ue, \
            &allocInfo->raType);

      ret = rgSCHCmnDlAlloc1CwTxRb(cell, subFrm, ue, &proc->tbInfo[0],\
            bo, &numRb, effBo);
      if (ret == RFAILED)
      {
         /* If allocation couldn't be made then return */
         RETVOID;
      }
      noTxLyrs = 1;
      precInfo = 0; /* TD */
   }
   else /* Precoding */
   {
      allocInfo->dciFormat = TFU_DCI_FORMAT_2;
      allocInfo->raType    = RG_SCH_CMN_RA_TYPE0;

      if (ueDl->mimoInfo.ri == 1)
      {
         /* Single Layer SM using FORMAT 2 */
         ret = rgSCHCmnDlAlloc1CwTxRb(cell, subFrm, ue, &proc->tbInfo[0],\
               bo, &numRb, effBo);
         if (ret == RFAILED)
         {
            /* If allocation couldn't be made then return */
            RETVOID;
         }
         noTxLyrs = 1;
         precInfo = 0; /* PrecInfo as 0 for RI=1*/
      }
      else
      {
         /* Spatial Multiplexing using 2 CWs */
         ret = rgSCHCmnDlAlloc2CwTxRb(cell, subFrm, ue, proc, bo, &numRb, effBo);
         if (ret == RFAILED)
         {
            /* If allocation couldn't be made then return */
            RETVOID;
         }
         noTxLyrs = ueDl->mimoInfo.ri;
         precInfoAntIdx = cell->numTxAntPorts/2 - 1; 
         precInfo = (getPrecInfoFunc[1][precInfoAntIdx])(ue, noTxLyrs, TRUE);
      }
   }

   
#ifdef LTEMAC_SPS
   if (!RG_SCH_CMN_SPS_DL_IS_SPS_HQP(proc))
#endif
   {
      /* Adding UE to RbAllocInfo TX Lst */
      rgSCHCmnDlRbInfoAddUeTx(cellWdAllocInfo, ue, proc);
   }

   /* Fill UE allocInfo scrath pad */
   RG_SCH_CMN_FILL_DL_TXINFO(allocInfo, numRb, FALSE, \
         precInfo, noTxLyrs, subFrm);

   RETVOID;
}


/**
 * @brief This function determines the RBs and Bytes required for BO
 *        transmission for UEs configured with TM 4.
 *
 * @details
 *
 *     Function: rgSCHCmnDlAllocTxRbTM4
 *     Purpose:  Invokes the functionality particular to the
 *               current state of the TBs of the "proc".
 *
 *               Reference Parameter effBo is filled with alloced bytes.
 *               Returns RFAILED if BO not satisfied at all.
 *
 *     Invoked by: rgSCHCmnDlAllocTxRb
 *
 *  @param[in]  RgSchCellCb           *cell
 *  @param[in]  RgSchDlSf             *subFrm
 *  @param[in]  RgSchUeCb             *ue
 *  @param[in]  U32                   bo
 *  @param[out] U32                   *effBo
 *  @param[in]  RgSchDlHqProcCb       *proc
 *  @param[out] RgSchCmnDlRbAllocInfo *cellWdAllocInfo
 *  @return  Void
 *
 **/
#ifdef ANSI
PRIVATE Void rgSCHCmnDlAllocTxRbTM4
(
RgSchCellCb                *cell,
RgSchDlSf                  *subFrm,
RgSchUeCb                  *ue,
U32                        bo,
U32                        *effBo,
RgSchDlHqProcCb            *proc,
RgSchCmnDlRbAllocInfo      *cellWdAllocInfo
)
#else
PRIVATE Void rgSCHCmnDlAllocTxRbTM4(cell, subFrm, ue, bo, effBo, proc, cellWdAllocInfo)
RgSchCellCb                *cell;
RgSchDlSf                  *subFrm;
RgSchUeCb                  *ue;
U32                        bo;
U32                        *effBo;
RgSchDlHqProcCb            *proc;
RgSchCmnDlRbAllocInfo      *cellWdAllocInfo;
#endif
{
   TRC2(rgSCHCmnDlAllocTxRbTM4);

   /* Both TBs free for TX allocation */
   rgSCHCmnDlTM4TxTx(cell, subFrm, ue, bo, effBo,\
         proc, cellWdAllocInfo);

   RETVOID;
}


/**
 * @brief This function determines the RBs and Bytes required for BO
 *        retransmission for UEs configured with TM 4.
 *
 * @details
 *
 *     Function: rgSCHCmnDlAllocRetxRbTM4
 *     Purpose:  Invokes the functionality particular to the
 *               current state of the TBs of the "proc".
 *
 *               Reference Parameter effBo is filled with alloced bytes.
 *               Returns RFAILED if BO not satisfied at all.
 *
 *     Invoked by: rgSCHCmnDlAllocRetxRb
 *
 *  @param[in]  RgSchCellCb           *cell
 *  @param[in]  RgSchDlSf             *subFrm
 *  @param[in]  RgSchUeCb             *ue
 *  @param[in]  U32                   bo
 *  @param[out] U32                   *effBo
 *  @param[in]  RgSchDlHqProcCb       *proc
 *  @param[out] RgSchCmnDlRbAllocInfo *cellWdAllocInfo
 *  @return  Void
 *
 **/
#ifdef ANSI
PRIVATE Void rgSCHCmnDlAllocRetxRbTM4
(
RgSchCellCb                *cell,
RgSchDlSf                  *subFrm,
RgSchUeCb                  *ue,
U32                        bo,
U32                        *effBo,
RgSchDlHqProcCb            *proc,
RgSchCmnDlRbAllocInfo      *cellWdAllocInfo
)
#else
PRIVATE Void rgSCHCmnDlAllocRetxRbTM4(cell, subFrm, ue, bo, effBo, proc, cellWdAllocInfo)
RgSchCellCb                *cell;
RgSchDlSf                  *subFrm;
RgSchUeCb                  *ue;
U32                        bo;
U32                        *effBo;
RgSchDlHqProcCb            *proc;
RgSchCmnDlRbAllocInfo      *cellWdAllocInfo;
#endif
{
   TRC2(rgSCHCmnDlAllocRetxRbTM4);

   if ((proc->tbInfo[0].state == HQ_TB_NACKED) &&
         (proc->tbInfo[1].state == HQ_TB_NACKED))
   {
      /* Both TBs require RETX allocation */
      rgSCHCmnDlTM4RetxRetx(cell, subFrm, ue, bo, effBo,\
            proc, cellWdAllocInfo);
   }
   else
   {
      /* One of the TBs need RETX allocation. Other TB may/maynot
       * be available for new TX allocation. */
      rgSCHCmnDlTM4TxRetx(cell, subFrm, ue, bo, effBo,\
            proc, cellWdAllocInfo);
   }

   RETVOID;
}

#ifdef RG_UNUSED

/**
 * @brief This function determines the RBs and Bytes required for BO
 *        transmission for UEs configured with TM 5.
 *
 * @details
 *
 *     Function: rgSCHCmnDlAllocTxRbTM5
 *     Purpose:
 *
 *               Reference Parameter effBo is filled with alloced bytes.
 *               Returns RFAILED if BO not satisfied at all.
 *
 *     Invoked by: rgSCHCmnDlAllocTxRb
 *
 *  @param[in]  RgSchCellCb           *cell
 *  @param[in]  RgSchDlSf             *subFrm
 *  @param[in]  RgSchUeCb             *ue
 *  @param[in]  U32                   bo
 *  @param[out] U32                   *effBo
 *  @param[in]  RgSchDlHqProcCb       *proc
 *  @param[out] RgSchCmnDlRbAllocInfo *cellWdAllocInfo
 *  @return Void
 *
 **/
#ifdef ANSI
PRIVATE Void rgSCHCmnDlAllocTxRbTM5
(
RgSchCellCb                *cell,
RgSchDlSf                  *subFrm,
RgSchUeCb                  *ue,
U32                        bo,
U32                        *effBo,
RgSchDlHqProcCb            *proc,
RgSchCmnDlRbAllocInfo      *cellWdAllocInfo
)
#else
PRIVATE Void rgSCHCmnDlAllocTxRbTM5(cell, subFrm, ue, bo, effBo, proc, cellWdAllocInfo)
RgSchCellCb                *cell;
RgSchDlSf                  *subFrm;
RgSchUeCb                  *ue;
U32                        bo;
U32                        *effBo;
RgSchDlHqProcCb            *proc;
RgSchCmnDlRbAllocInfo      *cellWdAllocInfo;
#endif
{
   TRC2(rgSCHCmnDlAllocTxRbTM5);
#if (ERRCLASS & ERRCLS_DEBUG)
   RLOG_ARG1(L_ERROR,DBG_CELLID,cell->cellId, "Invalid TM 5 for CRNTI:%d",ue->ueId);
#endif
   RETVOID;
}


/**
 * @brief This function determines the RBs and Bytes required for BO
 *        retransmission for UEs configured with TM 5.
 *
 * @details
 *
 *     Function: rgSCHCmnDlAllocRetxRbTM5
 *     Purpose:
 *
 *               Reference Parameter effBo is filled with alloced bytes.
 *               Returns RFAILED if BO not satisfied at all.
 *
 *     Invoked by: rgSCHCmnDlAllocRetxRb
 *
 *  @param[in]  RgSchCellCb           *cell
 *  @param[in]  RgSchDlSf             *subFrm
 *  @param[in]  RgSchUeCb             *ue
 *  @param[in]  U32                   bo
 *  @param[out] U32                   *effBo
 *  @param[in]  RgSchDlHqProcCb       *proc
 *  @param[out] RgSchCmnDlRbAllocInfo *cellWdAllocInfo
 *  @return Void
 *
 **/
#ifdef ANSI
PRIVATE Void rgSCHCmnDlAllocRetxRbTM5
(
RgSchCellCb                *cell,
RgSchDlSf                  *subFrm,
RgSchUeCb                  *ue,
U32                        bo,
U32                        *effBo,
RgSchDlHqProcCb            *proc,
RgSchCmnDlRbAllocInfo      *cellWdAllocInfo
)
#else
PRIVATE Void rgSCHCmnDlAllocRetxRbTM5(cell, subFrm, ue, bo, effBo, proc, cellWdAllocInfo)
RgSchCellCb                *cell;
RgSchDlSf                  *subFrm;
RgSchUeCb                  *ue;
U32                        bo;
U32                        *effBo;
RgSchDlHqProcCb            *proc;
RgSchCmnDlRbAllocInfo      *cellWdAllocInfo;
#endif
{
   TRC2(rgSCHCmnDlAllocRetxRbTM5);
#if (ERRCLASS & ERRCLS_DEBUG)
   RLOG_ARG1(L_ERROR,DBG_CELLID,cell->cellId, "Invalid TM 5 for CRNTI:%d",ue->ueId);
#endif
   RETVOID;
}
#endif


/**
 * @brief This function determines the RBs and Bytes required for BO
 *        transmission for UEs configured with TM 6.
 *
 * @details
 *
 *     Function: rgSCHCmnDlAllocTxRbTM6
 *     Purpose:
 *
 *               Reference Parameter effBo is filled with alloced bytes.
 *               Returns RFAILED if BO not satisfied at all.
 *
 *     Invoked by: rgSCHCmnDlAllocTxRb
 *
 *  @param[in]  RgSchCellCb           *cell
 *  @param[in]  RgSchDlSf             *subFrm
 *  @param[in]  RgSchUeCb             *ue
 *  @param[in]  U32                   bo
 *  @param[out] U32                   *effBo
 *  @param[in]  RgSchDlHqProcCb       *proc
 *  @param[out] RgSchCmnDlRbAllocInfo *cellWdAllocInfo
 *  @return Void
 *
 **/
#ifdef ANSI
PRIVATE Void rgSCHCmnDlAllocTxRbTM6
(
RgSchCellCb                *cell,
RgSchDlSf                  *subFrm,
RgSchUeCb                  *ue,
U32                        bo,
U32                        *effBo,
RgSchDlHqProcCb            *proc,
RgSchCmnDlRbAllocInfo      *cellWdAllocInfo
)
#else
PRIVATE Void rgSCHCmnDlAllocTxRbTM6(cell, subFrm, ue, bo, effBo, proc, cellWdAllocInfo)
RgSchCellCb                *cell;
RgSchDlSf                  *subFrm;
RgSchUeCb                  *ue;
U32                        bo;
U32                        *effBo;
RgSchDlHqProcCb            *proc;
RgSchCmnDlRbAllocInfo      *cellWdAllocInfo;
#endif
{
   RgSchDlRbAlloc *allocInfo;
   RgSchCmnDlUe   *ueDl;
   S16            ret;
   U8             numRb;

   TRC2(rgSCHCmnDlAllocTxRbTM6);

   ret       = ROK;
   ueDl      = RG_SCH_CMN_GET_DL_UE(ue);
   allocInfo = RG_SCH_CMN_GET_ALLOCCB_FRM_UE(ue);

   if (ueDl->mimoInfo.forceTD)
   {
      allocInfo->dciFormat = TFU_DCI_FORMAT_1A;
      allocInfo->raType    = RG_SCH_CMN_RA_TYPE2;
   }
   else
   {
      allocInfo->dciFormat = TFU_DCI_FORMAT_1B;
      allocInfo->raType    = RG_SCH_CMN_RA_TYPE2;
      /* Fill precoding information for FORMAT 1B */
      /* First 4 least significant bits to indicate PMI.
       * 4th most significant corresponds to pmi Confirmation.
       */
      allocInfo->mimoAllocInfo.precIdxInfo |= ue->mimoInfo.puschFdbkVld << 4;
      allocInfo->mimoAllocInfo.precIdxInfo |= ueDl->mimoInfo.pmi;
   }
   ret = rgSCHCmnDlAlloc1CwTxRb(cell, subFrm, ue, &proc->tbInfo[0],\
         bo, &numRb, effBo);
   if (ret == RFAILED)
   {
      /* If allocation couldn't be made then return */
      RETVOID;
   }
   
#ifdef LTEMAC_SPS
   if (!RG_SCH_CMN_SPS_DL_IS_SPS_HQP(proc))
#endif
   {
      /* Adding UE to RbAllocInfo TX Lst */
      rgSCHCmnDlRbInfoAddUeTx(cellWdAllocInfo, ue, proc);
   }
   /* Fill UE alloc Info */
   allocInfo->rbsReq = numRb;
   allocInfo->dlSf   = subFrm;
   RETVOID;
}


/**
 * @brief This function determines the RBs and Bytes required for BO
 *        retransmission for UEs configured with TM 6.
 *
 * @details
 *
 *     Function: rgSCHCmnDlAllocRetxRbTM6
 *     Purpose:
 *
 *               Reference Parameter effBo is filled with alloced bytes.
 *               Returns RFAILED if BO not satisfied at all.
 *
 *     Invoked by: rgSCHCmnDlAllocRetxRb
 *
 *  @param[in]  RgSchCellCb           *cell
 *  @param[in]  RgSchDlSf             *subFrm
 *  @param[in]  RgSchUeCb             *ue
 *  @param[in]  U32                   bo
 *  @param[out] U32                   *effBo
 *  @param[in]  RgSchDlHqProcCb       *proc
 *  @param[out] RgSchCmnDlRbAllocInfo *cellWdAllocInfo
 *  @return Void
 *
 **/
#ifdef ANSI
PRIVATE Void rgSCHCmnDlAllocRetxRbTM6
(
RgSchCellCb                *cell,
RgSchDlSf                  *subFrm,
RgSchUeCb                  *ue,
U32                        bo,
U32                        *effBo,
RgSchDlHqProcCb            *proc,
RgSchCmnDlRbAllocInfo      *cellWdAllocInfo
)
#else
PRIVATE Void rgSCHCmnDlAllocRetxRbTM6(cell, subFrm, ue, bo, effBo, proc, cellWdAllocInfo)
RgSchCellCb                *cell;
RgSchDlSf                  *subFrm;
RgSchUeCb                  *ue;
U32                        bo;
U32                        *effBo;
RgSchDlHqProcCb            *proc;
RgSchCmnDlRbAllocInfo      *cellWdAllocInfo;
#endif
{
   RgSchDlRbAlloc *allocInfo;
   RgSchCmnDlUe   *ueDl;
   S16            ret;
   U8             numRb;

   TRC2(rgSCHCmnDlAllocRetxRbTM6);

   ret       = ROK;
   allocInfo = RG_SCH_CMN_GET_ALLOCCB_FRM_UE(ue);
   ueDl      = RG_SCH_CMN_GET_DL_UE(ue);

   if (ueDl->mimoInfo.forceTD)
   {
      allocInfo->dciFormat = TFU_DCI_FORMAT_1A;
      allocInfo->raType    = RG_SCH_CMN_RA_TYPE2;
   }
   else
   {
      allocInfo->dciFormat = TFU_DCI_FORMAT_1B;
      allocInfo->raType    = RG_SCH_CMN_RA_TYPE2;
      /* Fill precoding information for FORMAT 1B */
      /* First 4 least significant bits to indicate PMI.
       * 4th most significant corresponds to pmi Confirmation.
       */
      allocInfo->mimoAllocInfo.precIdxInfo |= ue->mimoInfo.puschFdbkVld << 4;
      allocInfo->mimoAllocInfo.precIdxInfo |= ueDl->mimoInfo.pmi;
   }

   /* Get the Allocation in terms of RBs that are required for
    * this retx of TB1 */
   ret = rgSCHCmnDlAlloc1CwRetxRb(cell, subFrm, ue, &proc->tbInfo[0],
         1, &numRb, effBo);
   if (ret == RFAILED)
   {
      /* Allocation couldn't be made for Retx */
      rgSCHCmnDlAdd2NonSchdRetxLst(cellWdAllocInfo, ue, proc);
      RETVOID;
   }
   /* Adding UE to allocInfo RETX Lst */
   rgSCHCmnDlRbInfoAddUeRetx(cellWdAllocInfo, ue, proc);
   /* Fill UE alloc Info */
   allocInfo->rbsReq = numRb;
   allocInfo->dlSf   = subFrm;
   RETVOID;
}


/**
 * @brief This function determines the RBs and Bytes required for BO
 *        transmission for UEs configured with TM 7.
 *
 * @details
 *
 *     Function: rgSCHCmnDlAllocTxRbTM7
 *     Purpose:
 *
 *               Reference Parameter effBo is filled with alloced bytes.
 *               Returns RFAILED if BO not satisfied at all.
 *
 *     Invoked by: rgSCHCmnDlAllocTxRb
 *
 *  @param[in]  RgSchCellCb           *cell
 *  @param[in]  RgSchDlSf             *subFrm
 *  @param[in]  RgSchUeCb             *ue
 *  @param[in]  U32                   bo
 *  @param[out] U32                   *effBo
 *  @param[in]  RgSchDlHqProcCb       *proc
 *  @param[out] RgSchCmnDlRbAllocInfo *cellWdAllocInfo
 *  @return Void
 *
 **/
#ifdef ANSI
PRIVATE Void rgSCHCmnDlAllocTxRbTM7
(
RgSchCellCb                *cell,
RgSchDlSf                  *subFrm,
RgSchUeCb                  *ue,
U32                        bo,
U32                        *effBo,
RgSchDlHqProcCb            *proc,
RgSchCmnDlRbAllocInfo      *cellWdAllocInfo
)
#else
PRIVATE Void rgSCHCmnDlAllocTxRbTM7(cell, subFrm, ue, bo, effBo, proc, cellWdAllocInfo)
RgSchCellCb                *cell;
RgSchDlSf                  *subFrm;
RgSchUeCb                  *ue;
U32                        bo;
U32                        *effBo;
RgSchDlHqProcCb            *proc;
RgSchCmnDlRbAllocInfo      *cellWdAllocInfo;
#endif
{
   TRC2(rgSCHCmnDlAllocTxRbTM7);
   rgSCHCmnDlAllocTxRb1Tb1Cw(cell, subFrm, ue, bo, effBo, proc, cellWdAllocInfo);
   RETVOID;
}


/**
 * @brief This function determines the RBs and Bytes required for BO
 *        retransmission for UEs configured with TM 7.
 *
 * @details
 *
 *     Function: rgSCHCmnDlAllocRetxRbTM7
 *     Purpose:
 *
 *               Reference Parameter effBo is filled with alloced bytes.
 *               Returns RFAILED if BO not satisfied at all.
 *
 *     Invoked by: rgSCHCmnDlAllocRetxRb
 *
 *  @param[in]  RgSchCellCb           *cell
 *  @param[in]  RgSchDlSf             *subFrm
 *  @param[in]  RgSchUeCb             *ue
 *  @param[in]  U32                   bo
 *  @param[out] U32                   *effBo
 *  @param[in]  RgSchDlHqProcCb       *proc
 *  @param[out] RgSchCmnDlRbAllocInfo *cellWdAllocInfo
 *  @return Void
 *
 **/
#ifdef ANSI
PRIVATE Void rgSCHCmnDlAllocRetxRbTM7
(
RgSchCellCb                *cell,
RgSchDlSf                  *subFrm,
RgSchUeCb                  *ue,
U32                        bo,
U32                        *effBo,
RgSchDlHqProcCb            *proc,
RgSchCmnDlRbAllocInfo      *cellWdAllocInfo
)
#else
PRIVATE Void rgSCHCmnDlAllocRetxRbTM7(cell, subFrm, ue, bo, effBo, proc, cellWdAllocInfo)
RgSchCellCb                *cell;
RgSchDlSf                  *subFrm;
RgSchUeCb                  *ue;
U32                        bo;
U32                        *effBo;
RgSchDlHqProcCb            *proc;
RgSchCmnDlRbAllocInfo      *cellWdAllocInfo;
#endif
{
   TRC2(rgSCHCmnDlAllocRetxRbTM7);
   rgSCHCmnDlAllocRetxRb1Tb1Cw(cell, subFrm, ue, bo, effBo, proc, cellWdAllocInfo);
   RETVOID;
}


/**
 * @brief This function invokes the TM specific DL TX RB Allocation routine.
 *
 * @details
 *
 *     Function: rgSCHCmnDlAllocTxRb
 *     Purpose:  This function invokes the TM specific
 *               DL TX RB Allocation routine.
 *
 *     Invoked by: Specific Schedulers
 *
 *  @param[in]  RgSchCellCb           *cell
 *  @param[in]  RgSchDlSf             *subFrm
 *  @param[in]  RgSchUeCb             *ue
 *  @param[in]  U32                   bo
 *  @param[out] U32                   *effBo
 *  @param[in]  RgSchDlHqProcCb       *proc
 *  @param[out] RgSchCmnDlRbAllocInfo *cellWdAllocInfo
 *  @return  S16
 *
 **/
#ifdef ANSI
PUBLIC S16 rgSCHCmnDlAllocTxRb
(
RgSchCellCb                *cell,
RgSchDlSf                  *subFrm,
RgSchUeCb                  *ue,
U32                        bo,
U32                        *effBo,
RgSchDlHqProcCb            *proc,
RgSchCmnDlRbAllocInfo      *cellWdAllocInfo
)
#else
PUBLIC S16 rgSCHCmnDlAllocTxRb(cell, subFrm, ue, bo, effBo, proc, cellWdAllocInfo)
RgSchCellCb                *cell;
RgSchDlSf                  *subFrm;
RgSchUeCb                  *ue;
U32                        bo;
U32                        *effBo;
RgSchDlHqProcCb            *proc;
RgSchCmnDlRbAllocInfo      *cellWdAllocInfo;
#endif
{
   TRC2(rgSCHCmnDlAllocTxRb);

   *effBo = 0;
   /* Check for DL BW exhaustion */
   /* Modified the conditional below to bring in the
    * dimension for returning failure if bo specified is 0.*/
   if ((subFrm->bw <= subFrm->bwAssigned) || (0 == bo))
   {
      RETVALUE(RFAILED);
   }

   /* LTE_ADV_FLAG_REMOVED_START */
   /* If cell-edge pool has no availability, then no need to go inside. For Cell-centred, I assume the above case will take care*/
   if(cell->lteAdvCb.sfrCfg.status)
   {
     if(ue->lteAdvUeCb.rgrLteAdvUeCfg.isUeCellEdge)
     {
         if ((subFrm->sfrTotalPoolInfo.CEPoolBwAvlbl <= 0) || (0 == bo))
          {
           RETVALUE(RFAILED);
          }
     }
   }
   /* LTE_ADV_FLAG_REMOVED_END */

   /* Call TM specific RB allocation routine */
   (dlAllocTxRbFunc[ue->mimoInfo.txMode - 1])(cell, subFrm, ue, bo, effBo, \
         proc, cellWdAllocInfo);

   RETVALUE(ROK);
}

/* DwPTS Scheduling Changes Start */
#ifdef LTE_TDD
/**
 * @brief Retransmit decision for TDD. Retx is avoided in below cases
 *        1) DL Sf       -> Spl Sf
 *        2) DL SF       -> DL SF 0 
 *
 * @details
 *
 *     Function: rgSCHCmnRetxAvoidTdd 
 *     Purpose: Avoid allocating RETX for cases 1, 2 
 * 
 *     Invoked by: rgSCHCmnRetxAvoidTdd 
 *
 *  @param[in]  RgSchDlSf             *curSf
 *  @param[in]  RgSchCellCb           *cell
 *  @param[in]  RgSchDlHqProcCb       *proc
 *  @return  Bool 
 *
 **/
#ifdef ANSI
PRIVATE Bool rgSCHCmnRetxAvoidTdd 
(
RgSchDlSf                  *curSf,
RgSchCellCb                *cell,
RgSchDlHqProcCb            *proc
)
#else
PRIVATE Bool rgSCHCmnRetxAvoidTdd(curSf, cell, proc)
RgSchDlSf                  *curSf;
RgSchCellCb                *cell;
RgSchDlHqProcCb            *proc;
#endif
{
   RgSchTddSfType   txSfType = 0;

   TRC2(rgSCHCmnRetxAvoidTdd);

   /* Get the RBs of TB that will be retransmitted */
   if (proc->tbInfo[0].state == HQ_TB_NACKED)
   {
      txSfType = proc->tbInfo[0].sfType;
   }
   if (proc->tbInfo[1].state == HQ_TB_NACKED) 
   {
      /* Select the TxSf with the highest num of possible REs 
       * In ascending order -> 1) SPL SF 2) DL_SF_0 3) DL_SF */
      txSfType = RGSCH_MAX(txSfType, proc->tbInfo[1].sfType);
   }

   if (txSfType > curSf->sfType)
   {
      /* Avoid retx */
      RETVALUE(TRUE);
   }
   
   /* Allow Retx */
   RETVALUE(FALSE);
}

#else
/* DwPTS Scheduling Changes End */

/**
 * @brief Avoid allocating RETX incase of collision
 * with reserved resources for BCH/PSS/SSS occassions.
 *
 * @details
 *
 *     Function: rgSCHCmnRetxAllocAvoid 
 *     Purpose: Avoid allocating RETX incase of collision
 * with reserved resources for BCH/PSS/SSS occassions 
 *
 *     Invoked by: rgSCHCmnDlAllocRetxRb 
 *
 *  @param[in]  RgSchDlSf             *subFrm
 *  @param[in]  RgSchUeCb             *ue
 *  @param[in]  RgSchDlHqProcCb       *proc
 *  @return  Bool 
 *
 **/
#ifdef ANSI
PUBLIC Bool rgSCHCmnRetxAllocAvoid 
(
RgSchDlSf                  *subFrm,
RgSchCellCb                *cell,
RgSchDlHqProcCb            *proc
)
#else
PUBLIC Bool rgSCHCmnRetxAllocAvoid(subFrm, cell, proc)
RgSchDlSf                  *subFrm;
RgSchCellCb                *cell;
RgSchDlHqProcCb            *proc;
#endif
{
   U8          reqRbs;

   TRC2(rgSCHCmnRetxAllocAvoid);

   if (proc->tbInfo[0].state == HQ_TB_NACKED)
   {
      reqRbs = proc->tbInfo[0].dlGrnt.numRb;    
   }
   else
   {
      reqRbs = proc->tbInfo[1].dlGrnt.numRb;    
   }
   /* Consider the dlGrnt.numRb of the Retransmitting proc->tbInfo
    * and current available RBs to determine if this RETX TB
    * will collide with the BCH/PSS/SSS occassion */
   if (subFrm->sfNum % 5 == 0)
   {
      if ((subFrm->bwAssigned < cell->pbchRbEnd) &&
          (((subFrm->bwAssigned + reqRbs) - cell->pbchRbStart) > 0))
      {
         RETVALUE(TRUE);
      }
   }
   RETVALUE(FALSE);
}

#endif


/**
 * @brief This function invokes the TM specific DL RETX RB Allocation routine.
 *
 * @details
 *
 *     Function: rgSCHCmnDlAllocRetxRb
 *     Purpose:  This function invokes the TM specific
 *               DL RETX RB Allocation routine.
 *
 *     Invoked by: Specific Schedulers
 *
 *  @param[in]  RgSchCellCb           *cell
 *  @param[in]  RgSchDlSf             *subFrm
 *  @param[in]  RgSchUeCb             *ue
 *  @param[in]  U32                   bo
 *  @param[out] U32                   *effBo
 *  @param[in]  RgSchDlHqProcCb       *proc
 *  @param[out] RgSchCmnDlRbAllocInfo *cellWdAllocInfo
 *  @return  S16
 *
 **/
#ifdef ANSI
PUBLIC S16 rgSCHCmnDlAllocRetxRb
(
RgSchCellCb                *cell,
RgSchDlSf                  *subFrm,
RgSchUeCb                  *ue,
U32                        bo,
U32                        *effBo,
RgSchDlHqProcCb            *proc,
RgSchCmnDlRbAllocInfo      *cellWdAllocInfo
)
#else
PUBLIC S16 rgSCHCmnDlAllocRetxRb(cell, subFrm, ue, bo, effBo, proc, cellWdAllocInfo)
RgSchCellCb                *cell;
RgSchDlSf                  *subFrm;
RgSchUeCb                  *ue;
U32                        bo;
U32                        *effBo;
RgSchDlHqProcCb            *proc;
RgSchCmnDlRbAllocInfo      *cellWdAllocInfo;
#endif
{
   TRC2(rgSCHCmnDlAllocRetxRb);

   *effBo = 0;
   /* Check for DL BW exhaustion */
   if (subFrm->bw <= subFrm->bwAssigned)
   {
      RETVALUE(RFAILED);
   }
   /* DwPTS Scheduling Changes Start */   
#ifdef  LTE_TDD
   if (rgSCHCmnRetxAvoidTdd(subFrm, cell, proc) == TRUE)
   {
      RETVALUE(ROK);
   }
#else
   /* DwPTS Scheduling Changes End */   
   /* LTE_ADV_FLAG_REMOVED_START */
   /* If cell-edge pool has no availability, then no need to go inside. For Cell-centred, I assume the above case will take care*/
   if(cell->lteAdvCb.sfrCfg.status)
   {
      if(ue->lteAdvUeCb.rgrLteAdvUeCfg.isUeCellEdge)
      {
         if (subFrm->sfrTotalPoolInfo.CEPoolBwAvlbl <= 0)
         {
            RETVALUE(RFAILED);
         }
      }
   }
   /* LTE_ADV_FLAG_REMOVED_END */

   /* Fix for ccpu00123919: Avoid allocating RETX in BCH/PSS/SSS occassions */
   if (rgSCHCmnRetxAllocAvoid(subFrm, cell, proc) == TRUE)
   {
      RETVALUE(ROK);    
   }
#endif
   /* Call TM specific RB allocation routine */
   (dlAllocRetxRbFunc[ue->mimoInfo.txMode - 1])(cell, subFrm, ue, bo, effBo, \
         proc, cellWdAllocInfo);

   RETVALUE(ROK);
}


/**
 * @brief This function determines the RBs and Bytes required for
 *        Transmission on 1 CW.
 *
 * @details
 *
 *     Function: rgSCHCmnDlAlloc1CwTxRb
 *     Purpose:  This function determines the RBs and Bytes required
 *               for Transmission of DL SVC BO on 1 CW.
 *               Also, takes care of SVC by SVC allocation by tracking
 *               previous SVCs allocations.
 *               Returns RFAILED if BO not satisfied at all.
 *
 *     Invoked by: DL UE Allocation
 *
 *  @param[in]  RgSchCellCb      *cell
 *  @param[in]  RgSchDlSf        *subFrm
 *  @param[in]  RgSchUeCb        *ue
 *  @param[in]  RgSchDlHqTbCb    *tbInfo
 *  @param[in]  U32              bo
 *  @param[out] U8               *numRb
 *  @param[out] U32              *effBo
 *  @return  S16
 *
 **/
#ifdef ANSI
PRIVATE S16 rgSCHCmnDlAlloc1CwTxRb
(
RgSchCellCb                *cell,
RgSchDlSf                  *subFrm,
RgSchUeCb                  *ue,
RgSchDlHqTbCb              *tbInfo,
U32                        bo,
U8                         *numRb,
U32                        *effBo
)
#else
PRIVATE S16 rgSCHCmnDlAlloc1CwTxRb(cell, subFrm, ue, tbInfo, bo, numRb, effBo)
RgSchCellCb                *cell;
RgSchDlSf                  *subFrm;
RgSchUeCb                  *ue;
RgSchDlHqTbCb              *tbInfo;
U32                        bo;
U8                         *numRb;
U32                        *effBo;
#endif
{
   U32                noRes;
   U32                eff;
   U32                tbSz;
   U8                 imcs;
   U8                 iTbs;
   RgSchCmnDlCell     *cellDl;
   RgSchCmnDlUe       *ueDl;
   RgSchDlRbAlloc     *allocInfo;
   U32                oldReq;
   U32                reqBytes;
   /* Correcting wrap around issue.
    * This change has been done at mutliple places in this function.*/
   U32                tempNumRb;
   RgSchCmnCell       *cellSch = RG_SCH_CMN_GET_CELL(cell);
   U8                cfi;               
   S16                availBw; 
#ifdef LTE_TDD      
   U8                 temp;
#endif
   TRC2(rgSCHCmnDlAlloc1CwTxRb);

   reqBytes  = bo;
   cellDl    = RG_SCH_CMN_GET_DL_CELL(cell);
   ueDl      = RG_SCH_CMN_GET_DL_UE(ue);
   allocInfo = RG_SCH_CMN_GET_ALLOCCB_FRM_UE(ue);
   oldReq    = ueDl->outStndAlloc;
   cfi       = cellDl->currCfi;               
#ifdef DEBUGP


#endif

   /* check if we can further allocate to this UE */
   if ((allocInfo->rbsReq >= ueDl->maxRb) ||
         (allocInfo->tbInfo[0].bytesReq >= ueDl->maxTbSz/8))
   {
      RETVALUE(RFAILED);
   }
  
   /* If there is no CFI change, continue to use the BLER based
    * iTBS value */
   if (ueDl->lastCfi == cfi)
   {   
      iTbs = ueDl->mimoInfo.cwInfo[0].iTbs[0];
   }
   else
   {  
      U8 cqi = ueDl->mimoInfo.cwInfo[0].cqi;
#ifdef LTE_TDD      
      iTbs = (U8) rgSchCmnFetchItbs(cell, ueDl, subFrm, cqi, cfi, 0, 1);
#else      
      iTbs = (U8) rgSchCmnFetchItbs(cell, ueDl, cqi, cfi, 0, 1);
#endif         
   }
   /*ccpu00131191 and ccpu00131317 - Fix for RRC Reconfig failure
    * issue for VoLTE call */
   /*ccpu00131626 - iTbs setting to zero was improper */
   if (tbInfo->hqP->hasDcch)
   {
      if (iTbs > 5)
      {
         iTbs  = iTbs - 5;
      }
      else
      {
         iTbs  = 0;
      }
   }

   else if (!cellSch->dl.isDlFreqSel)
   {
#ifdef LTE_TDD
    /* for Tdd reduce iTbs only for SF0 due to PBCH */
      if (subFrm->sfNum == 0)
      {
         (iTbs > 1)? (iTbs -= 1) : (iTbs = 0);
      }
#else   
      if (subFrm->sfNum % 5 == 0)
      {
         (iTbs > 1)? (iTbs -= 2) : (iTbs = 0);
      }
#endif
   }

#ifdef LTE_TDD
   if(subFrm->sfType == RG_SCH_SPL_SF_DATA)
   {
      RGSCH_GET_SPS_SF_CFI(cell->bwCfg.dlTotalBw, cfi);
   }
#endif 

   eff   = (*(RgSchCmnTbSzEff *)(cellSch->dl.cqiToEffTbl[0][cfi]))[iTbs];
   
   if (tbInfo->hqP->hasDcch)
   {
   }

   ueDl->outStndAlloc += bo;
   /* consider Cumulative amount of this BO and bytes so far allocated */
   bo = RGSCH_MIN(ueDl->outStndAlloc, ueDl->maxTbSz/8);
   /* Get the number of REs needed for this bo. */
   noRes = ((bo * 8 * 1024) / eff);

   /* Get the number of RBs needed for this transmission */
   /* Number of RBs = No of REs / No of REs per RB       */
   tempNumRb = RGSCH_CEIL(noRes, cellDl->noResPerRb[cfi]);
   /* Cannot exceed the maximum number of RBs per UE */
   if (tempNumRb > ueDl->maxRb)
   {
      tempNumRb = ueDl->maxRb;
   }
   else
   {
      while ((tempNumRb < ueDl->maxRb) &&
            ((rgTbSzTbl[0][iTbs][tempNumRb-1]/8) < bo))
      {
         (tempNumRb)++;
      }
   }
   availBw = subFrm->bw - subFrm->bwAssigned;
   /* Cannot exceed the total number of RBs in the cell */
   if ((S16)(tempNumRb - allocInfo->rbsReq) > availBw)
   {
      tempNumRb = availBw + allocInfo->rbsReq;
   }

   tbSz = rgTbSzTbl[0][iTbs][tempNumRb-1]/8;
   /* DwPts Scheduling Changes Start */
#ifdef LTE_TDD
   if(subFrm->sfType == RG_SCH_SPL_SF_DATA)
   {
      tbSz = rgSCHCmnCalcDwPtsTbSz(cell, bo, (U8*)&tempNumRb, &iTbs, 1, cfi);   
      /* Check for available Bw */
      if ((S16)tempNumRb - allocInfo->rbsReq > availBw)
      {
         tempNumRb = availBw + allocInfo->rbsReq;
         tbSz = rgTbSzTbl[0][iTbs][MAX(tempNumRb*3/4,1)-1]/8;
      }
      /* The below check of 4/3 is an approximation for the maxRb
       * For the UE in Special Sf. */
      if (tempNumRb > ueDl->maxRb*4/3)
      {
         tempNumRb = ueDl->maxRb*4/3;
         temp = MAX(tempNumRb*3/4,1)-1;
         RGSCH_ARRAY_BOUND_CHECK(cell->instIdx, rgTbSzTbl[0][iTbs],temp);
         tbSz = rgTbSzTbl[0][iTbs][temp]/8;
      }
   }
#endif  
   /* DwPts Scheduling Changes End */
   *effBo = RGSCH_MIN(tbSz - oldReq, reqBytes);
   RG_SCH_CMN_DL_TBS_TO_MCS(iTbs, imcs);

   RG_SCH_CMN_FILL_DL_TBINFO(&allocInfo->tbInfo[0], tbSz, \
         iTbs, imcs, tbInfo, 1);
   *numRb = (U8) tempNumRb;
   
   /* Update the subframe Allocated BW field */
   subFrm->bwAssigned = subFrm->bwAssigned + tempNumRb - allocInfo->rbsReq;
   
   RETVALUE(ROK);
}


/**
 * @brief This function is invoked in the event of any TB's allocation
 *  being underutilized by the specific scheduler. Here we reduce iMcs
 *  to increase redundancy and hence increase reception quality at UE.
 *
 * @details
 *
 *     Function: rgSCHCmnRdcImcsTxTb
 *     Purpose:  This function shall reduce the iMcs in accordance with
 *               the total consumed bytes by the UE at allocation
 *               finalization.
 *
 *     Invoked by: UE DL Allocation finalization routine
 *                 of specific scheduler.
 *
 *  @param[in]  RgSchDlRbAlloc   *allocInfo
 *  @param[in]  U8               tbInfoIdx
 *  @param[in]  U32              cnsmdBytes
 *  @return  Void
 *
 **/
#ifdef ANSI
PUBLIC Void rgSCHCmnRdcImcsTxTb
(
RgSchDlRbAlloc   *allocInfo,
U8               tbInfoIdx,
U32              cnsmdBytes
)
#else
PUBLIC Void rgSCHCmnRdcImcsTxTb(allocInfo, tbInfoIdx, cnsmdBytes)
RgSchDlRbAlloc   *allocInfo;
U8               tbInfoIdx;
U32              cnsmdBytes;
#endif
{
   RETVOID;
   /*The below functionality is not needed.*/
   U8                 noLyr;
   U8                 iTbs;
   U16                numRb;

   TRC2(rgSCHCmnRdcImcsTxTb);

   iTbs = allocInfo->tbInfo[tbInfoIdx].iTbs;
   noLyr = allocInfo->tbInfo[tbInfoIdx].noLyr;
   numRb = allocInfo->rbsAlloc;
   if ( numRb > 0)
   {
      if ((rgTbSzTbl[noLyr-1][iTbs][numRb-1]/8) == cnsmdBytes)
      {
         RETVOID;
      }
   }
   /* Get iTbs as suitable for the consumed bytes */
   while((rgTbSzTbl[noLyr-1][iTbs][numRb-1]/8) > cnsmdBytes)
   {
      if (iTbs == 0)
      {
         RG_SCH_CMN_DL_TBS_TO_MCS(iTbs, allocInfo->tbInfo[tbInfoIdx].\
               tbCb->dlGrnt.iMcs);
         RETVOID;
      }
      iTbs--;
   }
   iTbs++;
   RG_SCH_CMN_DL_TBS_TO_MCS(iTbs, allocInfo->tbInfo[tbInfoIdx].tbCb->dlGrnt.iMcs);

   RETVOID;
}


/**
 * @brief This function determines the RBs and Bytes required for
 *        Transmission on 2 CWs.
 *
 * @details
 *
 *     Function: rgSCHCmnDlAlloc2CwTxRb
 *     Purpose:  This function determines the RBs and Bytes required
 *               for Transmission of DL SVC BO on 2 CWs.
 *               Also, takes care of SVC by SVC allocation by tracking
 *               previous SVCs allocations.
 *               Returns RFAILED if BO not satisfied at all.
 *
 *     Invoked by: TM3 and TM4 DL UE Allocation
 *
 *  @param[in]  RgSchCellCb      *cell
 *  @param[in]  RgSchDlSf        *subFrm
 *  @param[in]  RgSchUeCb        *ue
 *  @param[in]  RgSchDlHqProcCb  *proc
 *  @param[in]  RgSchDlHqProcCb  bo
 *  @param[out] U8               *numRb
 *  @param[out] U32              *effBo
 *  @return  Void
 *
 **/
#ifdef ANSI
PRIVATE S16 rgSCHCmnDlAlloc2CwTxRb
(
RgSchCellCb                *cell,
RgSchDlSf                  *subFrm,
RgSchUeCb                  *ue,
RgSchDlHqProcCb            *proc,
U32                        bo,
U8                         *numRbRef,
U32                        *effBo
)
#else
PRIVATE S16 rgSCHCmnDlAlloc2CwTxRb(cell, subFrm, ue, proc, bo, numRbRef, effBo)
RgSchCellCb                *cell;
RgSchDlSf                  *subFrm;
RgSchUeCb                  *ue;
RgSchDlHqProcCb            *proc;
U32                        bo;
U8                         *numRbRef;
U32                        *effBo;
#endif
{
   U32                noRes;
   U32                eff1, eff2;
   U32                tb1Sz, tb2Sz;
   U8                 imcs1, imcs2;
   U8                 noLyr1, noLyr2;
   U8                 iTbs1, iTbs2;
   RgSchCmnDlCell     *cellDl;
   RgSchCmnDlUe       *ueDl;
   RgSchDlRbAlloc     *allocInfo;
   U32                oldReq;
   U32                reqBytes;
   /* Fix: MUE_PERTTI_DL */
   U32                numRb;
   RgSchCmnCell       *cellSch = RG_SCH_CMN_GET_CELL(cell);
   U8                 cfi = cellSch->dl.currCfi;
   S16                availBw; 

   TRC2(rgSCHCmnDlAlloc2CwTxRb);

   reqBytes  = bo;
   cellDl    = RG_SCH_CMN_GET_DL_CELL(cell);
   ueDl      = RG_SCH_CMN_GET_DL_UE(ue);
   allocInfo = RG_SCH_CMN_GET_ALLOCCB_FRM_UE(ue);
   oldReq    = ueDl->outStndAlloc;


   /* check if we can further allocate to this UE */
   if ((allocInfo->tbInfo[0].bytesReq + allocInfo->tbInfo[1].bytesReq \
            >= ueDl->maxTbBits/8) ||
         (allocInfo->tbInfo[0].bytesReq >= ueDl->maxTbSz/8) ||
         (allocInfo->tbInfo[1].bytesReq >= ueDl->maxTbSz/8) ||
         (allocInfo->rbsReq >= ueDl->maxRb))
   {
      RLOG_ARG0(L_DEBUG,DBG_CELLID,cell->cellId,
            "rgSCHCmnDlAllocRb(): UEs max allocation exceed");
      RETVALUE(RFAILED);
   }

   noLyr1 = ueDl->mimoInfo.cwInfo[0].noLyr;
   noLyr2 = ueDl->mimoInfo.cwInfo[1].noLyr;

   /* If there is no CFI change, continue to use the BLER based
    * iTBS value */
   if (ueDl->lastCfi == cfi)
   {   
      iTbs1  = ueDl->mimoInfo.cwInfo[0].iTbs[noLyr1 - 1];
      iTbs2  = ueDl->mimoInfo.cwInfo[1].iTbs[noLyr2 - 1];
   }
   else
   {  
      U8 cqi = ueDl->mimoInfo.cwInfo[0].cqi;
#ifdef LTE_TDD      
      iTbs1 = (U8) rgSchCmnFetchItbs(cell, ueDl, subFrm, cqi, cfi, 0, noLyr1);
#else      
      iTbs1 = (U8) rgSchCmnFetchItbs(cell, ueDl, cqi, cfi, 0, noLyr1);
#endif         

      cqi = ueDl->mimoInfo.cwInfo[1].cqi;
#ifdef LTE_TDD      
      iTbs2 = (U8) rgSchCmnFetchItbs(cell, ueDl, subFrm, cqi, cfi, 1, noLyr2);
#else      
      iTbs2 = (U8) rgSchCmnFetchItbs(cell, ueDl, cqi, cfi, 1, noLyr2);
#endif         
   } 

   /*ccpu00131191 and ccpu00131317 - Fix for RRC Reconfig failure
    * issue for VoLTE call */
   if (proc->hasDcch)
   {
      if (iTbs1 > 5)
      {
         iTbs1  = iTbs1 - 5;
      }
     else
     {
         iTbs1  = 0; 
     }
      if (iTbs2 > 5)
      {
         iTbs2  = iTbs2 - 5;
      }
      else
     {
         iTbs2  = 0; 
     }
   }
   else if(!cellSch->dl.isDlFreqSel)
   {
#ifdef LTE_TDD
      /* for Tdd reduce iTbs only for SF0. SF5 contains only 
       * SSS and can be ignored */
      if (subFrm->sfNum == 0)
      {
         (iTbs1 > 1)? (iTbs1 -= 1) : (iTbs1 = 0);
         (iTbs2 > 1)? (iTbs2 -= 1) : (iTbs2 = 0);
      }
#else   
      if (subFrm->sfNum % 5 == 0)
      {
         (iTbs1 > 1)? (iTbs1 -= 2) : (iTbs1 = 0);
         (iTbs2 > 1)? (iTbs2 -= 2) : (iTbs2 = 0);
      }
#endif
   }

#ifdef LTE_TDD
   if(subFrm->sfType == RG_SCH_SPL_SF_DATA)
   {
      RGSCH_GET_SPS_SF_CFI(cell->bwCfg.dlTotalBw, cfi);
   }
#endif 

   eff1 = (*(RgSchCmnTbSzEff *)(cellSch->dl.cqiToEffTbl[noLyr1 - 1][cfi]))[iTbs1];
   eff2 = (*(RgSchCmnTbSzEff *)(cellSch->dl.cqiToEffTbl[noLyr2 - 1][cfi]))[iTbs2];


   ueDl->outStndAlloc += bo;
   /* consider Cumulative amount of this BO and bytes so far allocated */
   bo = RGSCH_MIN(ueDl->outStndAlloc, ueDl->maxTbBits/8);
   bo = RGSCH_MIN(RGSCH_MAX(RGSCH_CMN_MIN_GRNT_HDR, (bo*eff1)/(eff1+eff2)), 
        ueDl->maxTbSz/8) +
        RGSCH_MIN(RGSCH_MAX(RGSCH_CMN_MIN_GRNT_HDR, (bo*eff2)/(eff1+eff2)), 
        (ueDl->maxTbSz)/8) +
        1; /* Add 1 to adjust the truncation at weighted averaging */
   /* Get the number of REs needed for this bo. */
   noRes = ((bo * 8 * 1024) / (eff1 + eff2));

   /* Get the number of RBs needed for this transmission */
   /* Number of RBs = No of REs / No of REs per RB       */
   numRb = RGSCH_CEIL(noRes, cellDl->noResPerRb[cfi]);
   /* Cannot exceed the maximum number of RBs per UE */
   if (numRb > ueDl->maxRb)
   {
      numRb = ueDl->maxRb;
   }
   else
   {
#if 0
#else
      //Bool  inc = FALSE;
      while ((numRb <= ueDl->maxRb) &&
              (rgTbSzTbl[noLyr1 - 1][iTbs1][numRb-1] <= ueDl->maxTbSz) &&
              (rgTbSzTbl[noLyr2 - 1][iTbs2][numRb-1] <= ueDl->maxTbSz) &&
              ((rgTbSzTbl[noLyr1 - 1][iTbs1][numRb-1]/8 +
                rgTbSzTbl[noLyr2 - 1][iTbs2][numRb-1]/8) <= bo))
      {
         //inc = TRUE;
         (numRb)++;
      }
#endif

   }
   availBw = subFrm->bw - subFrm->bwAssigned;
   /* Cannot exceed the total number of RBs in the cell */
   if ((S16)(numRb - allocInfo->rbsReq) > availBw)
   {
      numRb = availBw + allocInfo->rbsReq;
   }
   tb1Sz = rgTbSzTbl[noLyr1 - 1][iTbs1][numRb-1]/8;
   tb2Sz = rgTbSzTbl[noLyr2 - 1][iTbs2][numRb-1]/8;
   /* DwPts Scheduling Changes Start */
#ifdef LTE_TDD
   if(subFrm->sfType == RG_SCH_SPL_SF_DATA)
   { 
      /* Max Rb for Special Sf is approximated as 4/3 of maxRb */
      rgSCHCmnCalcDwPtsTbSz2Cw(cell, bo, (U8*)&numRb,  ueDl->maxRb*4/3, 
                                &iTbs1, &iTbs2, noLyr1, 
                                noLyr2, &tb1Sz, &tb2Sz, cfi);   
      /* Check for available Bw */
      if ((S16)numRb - allocInfo->rbsReq > availBw)
      {
         numRb = availBw + allocInfo->rbsReq;
         tb1Sz = rgTbSzTbl[noLyr1-1][iTbs1][RGSCH_MAX(numRb*3/4,1)-1]/8;
         tb2Sz = rgTbSzTbl[noLyr2-1][iTbs2][RGSCH_MAX(numRb*3/4,1)-1]/8;
      }
   }
#endif
   /* DwPts Scheduling Changes End */
   /* Update the subframe Allocated BW field */
   subFrm->bwAssigned = subFrm->bwAssigned + numRb - \
                        allocInfo->rbsReq;

   *effBo = RGSCH_MIN((tb1Sz + tb2Sz) - oldReq, reqBytes);
   RG_SCH_CMN_DL_TBS_TO_MCS(iTbs1, imcs1);
   RG_SCH_CMN_DL_TBS_TO_MCS(iTbs2, imcs2);
   RG_SCH_CMN_FILL_DL_TBINFO(&allocInfo->tbInfo[0], tb1Sz, \
         iTbs1, imcs1, &proc->tbInfo[0], noLyr1);
   RG_SCH_CMN_FILL_DL_TBINFO(&allocInfo->tbInfo[1], tb2Sz, \
         iTbs2, imcs2, &proc->tbInfo[1], noLyr2);
   *numRbRef = (U8)numRb;


   RETVALUE(ROK);
}


/**
 * @brief This function determines the RBs and Bytes required for
 *        Transmission & Retransmission on 2 CWs.
 *
 * @details
 *
 *     Function: rgSCHCmnDlAlloc2CwTxRetxRb
 *     Purpose:  This function determines the RBs and Bytes required
 *               for Transmission & Retransmission on 2 CWs. Allocate
 *               RETX TB on a better CW and restrict new TX TB by
 *               RETX allocation.
 *               Returns RFAILED if BO not satisfied at all.
 *
 *     Invoked by: TM3 and TM4 DL UE Allocation
 *
 *  @param[in]  RgSchCellCb      *cell
 *  @param[in]  RgSchDlSf        *subFrm
 *  @param[in]  RgSchUeCb        *ue
 *  @param[in]  RgSchDlHqTbCb    *reTxTb
 *  @param[in]  RgSchDlHqTbCb    *txTb
 *  @param[out] U8               *numRb
 *  @param[out] U32              *effBo
 *  @return  Void
 *
 **/
#ifdef ANSI
PRIVATE S16 rgSCHCmnDlAlloc2CwTxRetxRb
(
RgSchCellCb                *cell,
RgSchDlSf                  *subFrm,
RgSchUeCb                  *ue,
RgSchDlHqTbCb              *reTxTb,
RgSchDlHqTbCb              *txTb,
U8                         *numRb,
U32                        *effBo
)
#else
PRIVATE S16 rgSCHCmnDlAlloc2CwTxRetxRb(cell, subFrm, ue, reTxTb, txTb, numRb,\
        effBo)
RgSchCellCb                *cell;
RgSchDlSf                  *subFrm;
RgSchUeCb                  *ue;
RgSchDlHqTbCb              *reTxTb;
RgSchDlHqTbCb              *txTb;
U8                         *numRb;
U32                        *effBo;
#endif
{
   RgSchCmnDlUe       *ueDl;
   RgSchDlRbAlloc     *allocInfo;
   U8                 imcs1, imcs2;
   U8                  noLyr2;
   U16                 tb2Sz;
   RgSchCmnDlUeCwInfo *otherCw;
   S16                 availBw;
   RgSchCmnDlCell     *cellDl = RG_SCH_CMN_GET_DL_CELL(cell);
   U8                 cfi = cellDl->currCfi; 
   U8                 iTbs;

   TRC2(rgSCHCmnDlAlloc2CwTxRetxRb);

   ueDl      = RG_SCH_CMN_GET_DL_UE(ue);
   allocInfo = RG_SCH_CMN_GET_ALLOCCB_FRM_UE(ue);
   otherCw   = &ueDl->mimoInfo.cwInfo[!(ueDl->mimoInfo.btrCwIdx)];


   /* Fix for ccpu00123919: In case of RETX TB scheduling avoiding recomputation of RB
    * and Tbs. Set all parameters same as Init TX except RV(only for NACKED) and
    * MCS.  */
   availBw = subFrm->bw - subFrm->bwAssigned; 
   *numRb = reTxTb->dlGrnt.numRb;
   if ((S16)*numRb > availBw)
   {
      RETVALUE(RFAILED);
   }
   /* Update the subframe Allocated BW field */
   subFrm->bwAssigned += *numRb;
   noLyr2 = otherCw->noLyr;
   RG_SCH_CMN_GET_MCS_FOR_RETX(reTxTb, imcs1);

   /* If there is no CFI change, continue to use the BLER based
    * iTBS value */
   if (ueDl->lastCfi == cfi)
   {   
      iTbs = otherCw->iTbs[noLyr2-1];
   }
   else
   {  
#ifdef LTE_TDD      
      iTbs = (U8) rgSchCmnFetchItbs(cell, ueDl, subFrm, otherCw->cqi, cfi, 
                                      !(ueDl->mimoInfo.btrCwIdx), noLyr2);
#else      
      iTbs = (U8) rgSchCmnFetchItbs(cell, ueDl, otherCw->cqi, cfi, 
                                      !(ueDl->mimoInfo.btrCwIdx), noLyr2);
#endif 
   } 
   tb2Sz = rgTbSzTbl[noLyr2-1][iTbs][*numRb-1]/8;
   /* DwPts Scheduling Changes Start */
#ifdef LTE_TDD
   if (subFrm->sfType == RG_SCH_SPL_SF_DATA)
   {
      U8 *iTbs2 = &iTbs;
      RGSCH_GET_SPS_SF_CFI(cell->bwCfg.dlTotalBw, cfi);

      tb2Sz = rgSCHCmnCalcDwPtsTbSz(cell, 0, numRb, iTbs2, noLyr2, cfi);
      /* Check for available Bw */
      if ((S16)*numRb - allocInfo->rbsReq > availBw)
      {
         *numRb = availBw + allocInfo->rbsReq;
         tb2Sz = rgTbSzTbl[noLyr2-1][*iTbs2][RGSCH_MAX(*numRb*3/4,1)-1]/8;
      }
   }     
#endif
   /* DwPts Scheduling Changes End */
   RG_SCH_CMN_DL_TBS_TO_MCS(iTbs, imcs2);
   
   RG_SCH_CMN_FILL_DL_TBINFO(&allocInfo->tbInfo[0], reTxTb->tbSz, \
                              0, imcs1, reTxTb, reTxTb->numLyrs);
   
   RG_SCH_CMN_FILL_DL_TBINFO(&allocInfo->tbInfo[1], tb2Sz, \
                              iTbs, imcs2, txTb, noLyr2);
   
   *effBo = reTxTb->tbSz + tb2Sz;

   RETVALUE(ROK);
}


/**
 * @brief This function determines the RBs and Bytes required for BO
 *        Retransmission on 2 CWs.
 *
 * @details
 *
 *     Function: rgSCHCmnDlAlloc2CwRetxRb
 *     Purpose:  This function determines the RBs and Bytes required
 *               for BO Retransmission on 2 CWs. Allocate larger TB
 *               on a better CW and check if the smaller TB can be
 *               accomodated on the other CW.
 *               Returns RFAILED if BO not satisfied at all.
 *
 *     Invoked by: Common Scheduler
 *
 *  @param[in]  RgSchCellCb      *cell
 *  @param[in]  RgSchDlSf        *subFrm
 *  @param[in]  RgSchUeCb        *ue
 *  @param[in]  RgSchDlHqProcCb  *proc
 *  @param[out] U8               *numRb
 *  @param[out] Bool             *swpFlg
 *  @param[out] U32              *effBo
 *  @return  Void
 *
 **/
#ifdef ANSI
PRIVATE S16 rgSCHCmnDlAlloc2CwRetxRb
(
RgSchCellCb                *cell,
RgSchDlSf                  *subFrm,
RgSchUeCb                  *ue,
RgSchDlHqProcCb            *proc,
U8                         *numRb,
Bool                       *swpFlg,
U32                        *effBo
)
#else
PRIVATE S16 rgSCHCmnDlAlloc2CwRetxRb(cell, subFrm, ue, proc,\
        numRb, swpFlg, effBo)
RgSchCellCb                *cell;
RgSchDlSf                  *subFrm;
RgSchUeCb                  *ue;
RgSchDlHqProcCb            *proc;
U8                         *numRb;
Bool                       *swpFlg;
U32                        *effBo;
#endif
{
   RgSchDlRbAlloc     *allocInfo;
   U8                 imcs1;
   U8                 imcs2;
   RgSchDlHqTbCb      *lrgTbInfo, *othrTbInfo;

   TRC2(rgSCHCmnDlAlloc2CwRetxRb);

   allocInfo = RG_SCH_CMN_GET_ALLOCCB_FRM_UE(ue);


   /* Fix for ccpu00123919: In case of RETX TB scheduling avoiding recomputation of RB
    * and Tbs. Set all parameters same as Init TX except RV(only for NACKED) and
    * MCS.  */
   lrgTbInfo  = &proc->tbInfo[0];
   othrTbInfo = &proc->tbInfo[1];
   *numRb = lrgTbInfo->dlGrnt.numRb;
   if ((S16)*numRb > (S16)(subFrm->bw - subFrm->bwAssigned))
   {
      RETVALUE(RFAILED);
   }
   /* Update the subframe Allocated BW field */
   subFrm->bwAssigned += *numRb;
   RG_SCH_CMN_GET_MCS_FOR_RETX(lrgTbInfo, imcs1);
   RG_SCH_CMN_GET_MCS_FOR_RETX(othrTbInfo, imcs2);
   RG_SCH_CMN_FILL_DL_TBINFO(&allocInfo->tbInfo[0], lrgTbInfo->tbSz, \
         0, imcs1, lrgTbInfo, lrgTbInfo->numLyrs);
   RG_SCH_CMN_FILL_DL_TBINFO(&allocInfo->tbInfo[1], othrTbInfo->tbSz, \
         0, imcs2, othrTbInfo, othrTbInfo->numLyrs);
   *effBo = lrgTbInfo->tbSz + othrTbInfo->tbSz;



   RETVALUE(ROK);
}


/**
 * @brief This function determines the RBs and Bytes required for BO
 *        Retransmission on 1 CW.
 *
 * @details
 *
 *     Function: rgSCHCmnDlAlloc1CwRetxRb
 *     Purpose:  This function determines the RBs and Bytes required
 *               for BO Retransmission on 1 CW, the first CW.
 *               Returns RFAILED if BO not satisfied at all.
 *
 *     Invoked by: Common Scheduler
 *
 *  @param[in]  RgSchCellCb      *cell
 *  @param[in]  RgSchDlSf        *subFrm
 *  @param[in]  RgSchUeCb        *ue
 *  @param[in]  RgSchDlHqTbCb    *tbInfo
 *  @param[in]  U8               noLyr
 *  @param[out] U8               *numRb
 *  @param[out] U32              *effBo
 *  @return  S16
 *
 **/
#ifdef ANSI
PRIVATE S16 rgSCHCmnDlAlloc1CwRetxRb
(
RgSchCellCb                *cell,
RgSchDlSf                  *subFrm,
RgSchUeCb                  *ue,
RgSchDlHqTbCb              *tbInfo,
U8                         noLyr,
U8                         *numRb,
U32                        *effBo
)
#else
PRIVATE S16 rgSCHCmnDlAlloc1CwRetxRb(cell, subFrm, ue, tbInfo, noLyr,\
        numRb, effBo)
RgSchCellCb                *cell;
RgSchDlSf                  *subFrm;
RgSchUeCb                  *ue;
RgSchDlHqTbCb              *tbInfo;
U8                         noLyr;
U8                         *numRb;
U32                        *effBo;
#endif
{
   RgSchDlRbAlloc  *allocInfo;
   U8              imcs;

   TRC2(rgSCHCmnDlAlloc1CwRetxRb);

   allocInfo = RG_SCH_CMN_GET_ALLOCCB_FRM_UE(ue);


   /* Fix for ccpu00123919: In case of RETX TB scheduling avoiding recomputation of RB
    * and Tbs. Set all parameters same as Init TX except RV(only for NACKED) and
    * MCS.  */
   *numRb = tbInfo->dlGrnt.numRb;
   if ((S16)*numRb > (S16)(subFrm->bw - subFrm->bwAssigned))
   {
      RETVALUE(RFAILED);
   }
   /* Update the subframe Allocated BW field */
   subFrm->bwAssigned += *numRb;
   RG_SCH_CMN_GET_MCS_FOR_RETX(tbInfo, imcs);
   /* Fix: For a RETX TB the iTbs is irrelevant, hence setting 0 */
   RG_SCH_CMN_FILL_DL_TBINFO(&allocInfo->tbInfo[0], tbInfo->tbSz, \
         0, imcs, tbInfo, tbInfo->numLyrs);
   *effBo = tbInfo->tbSz;

   RETVALUE(ROK);
}

#ifdef LTEMAC_SPS

/**
 * @brief This function is called to handle Release PDCCH feedback for SPS UE
 *
 * @details
 *
 *     Function: rgSCHCmnDlRelPdcchFbk
 *     Purpose:  Invokes SPS module to handle release PDCCH feedback
 *
 *     Invoked by: DHM
 *
 *  @param[in]   RgSchCellCb     *cell
 *  @param[in]   RgSchUeCb       *ue
 *  @param[in]   Bool            isAck
 *  @return  Void
 *
 **/
#ifdef ANSI
PUBLIC Void rgSCHCmnDlRelPdcchFbk
(
RgSchCellCb        *cell,
RgSchUeCb          *ue,
Bool               isAck
)
#else
PUBLIC Void rgSCHCmnDlRelPdcchFbk(cell, ue, isAck)
RgSchCellCb        *cell;
RgSchUeCb          *ue;
Bool               isAck;
#endif
{

   TRC2(rgSCHCmnDlRelPdcchFbk);
   rgSCHCmnSpsDlRelPdcchFbk(cell, ue, isAck);
   RETVOID;

}


/**
 * @brief This function is invoked to handle Ack processing for a HARQ proc.
 *
 * @details
 *
 *     Function: rgSCHCmnDlProcAck
 *     Purpose:  DTX processing for HARQ proc
 *
 *     Invoked by: DHM
 *
 *  @param[in]   RgSchCellCb     *cell
 *  @param[in]   RgSchDlHqProcCb *hqP
 *  @return  Void
 *
 **/
#ifdef ANSI
PUBLIC Void rgSCHCmnDlProcAck
(
RgSchCellCb        *cell,
RgSchDlHqProcCb    *hqP
)
#else
PUBLIC Void rgSCHCmnDlProcAck(cell, hqP)
RgSchCellCb        *cell;
RgSchDlHqProcCb    *hqP;
#endif
{

   TRC2(rgSCHCmnDlProcAck);

   if (RG_SCH_CMN_SPS_DL_IS_SPS_HQP(hqP))
   {
      /* Invoke SPS module if SPS service was scheduled for this HARQ proc */
      rgSCHCmnSpsDlProcAck(cell, hqP);
   }
   RETVOID;
}
#ifdef RGSCH_SPS_STATS
extern U32 rgSchStatCrntiCeRcvCnt;
#endif
/**
 * @brief This function is invoked to handle CRNTI CE reception for an UE
 *
 * @details
 *
 *     Function: rgSCHCmnHdlCrntiCE
 *     Purpose:  Handle CRNTI CE reception
 *
 *     Invoked by: DHM
 *
 *  @param[in]   RgSchCellCb     *cell
 *  @param[in]   RgSchDlHqProcCb *hqP
 *  @return  Void
 *
 **/
#ifdef ANSI
PUBLIC Void rgSCHCmnHdlCrntiCE
(
RgSchCellCb        *cell,
RgSchUeCb          *ue
)
#else
PUBLIC Void rgSCHCmnHdlCrntiCE(cell, ue)
RgSchCellCb        *cell;
RgSchUeCb          *ue;
#endif
{

   TRC2(rgSCHCmnHdlCrntiCE);
#ifdef RGSCH_SPS_STATS   
   rgSchStatCrntiCeRcvCnt++;
#endif

   /* When UL sync lost happened due to TA timer expiry UE is being moved to 
      PDCCH order inactivity list.But when CRNTI CE received in msg3 from UE
      we are not moving UE into active state due to that RRC Reconfiguration is
      not happening.
      So here we are moving UE to active list whenever we receive the CRNTI CE and
      UE is inactive */
   /* CR ccpu00144525 */      
   if (RG_SCH_CMN_IS_UE_PDCCHODR_INACTV(ue))
   {
       /* Activate this UE if it was inactive */
       RG_SCH_CMN_DL_UPDT_INACTV_MASK ( cell, ue, RG_PDCCHODR_INACTIVE);
       RG_SCH_CMN_UL_UPDT_INACTV_MASK ( cell, ue, RG_PDCCHODR_INACTIVE);
   }

   /* Handling is same as reception of UE RESET for both DL and UL */
   if (ue->dl.dlSpsCfg.isDlSpsEnabled)
   {
      rgSCHCmnSpsDlUeReset(cell, ue);
   }
   if (ue->ul.ulSpsCfg.isUlSpsEnabled == TRUE)
   {
      rgSCHCmnSpsUlUeReset(cell, ue);
   }
   
   RETVOID;
}


/**
 * @brief This function is called to handle relInd from MAC for a UE
 *
 * @details
 *
 *     Function: rgSCHCmnUlSpsRelInd
 *     Purpose:  Invokes SPS module to handle UL SPS release for a UE
 *
 *     Invoked by: SCH_UTL
 *
 *  @param[in]   RgSchCellCb        *cell
 *  @param[in]   RgSchUeCb          *ue
 *  @param[in]   Bool               isExplRel
 *  @return  Void
 *
 **/
#ifdef ANSI
PUBLIC Void rgSCHCmnUlSpsRelInd
(
RgSchCellCb        *cell,
RgSchUeCb          *ue,
Bool               isExplRel
)
#else
PUBLIC Void rgSCHCmnUlSpsRelInd(cell, ue, isExplRel)
RgSchCellCb        *cell;
RgSchUeCb          *ue;
Bool               isExplRel;
#endif
{

   TRC2(rgSCHCmnUlSpsRelInd);
   rgSCHCmnSpsUlProcRelInd(cell, ue, isExplRel);
   RETVOID;

} /* end of rgSCHCmnUlSpsRelInd */

/**
 * @brief This function is called to handle SPS Activate Ind from MAC for a UE
 *
 * @details
 *
 *     Function: rgSCHCmnUlSpsActInd
 *     Purpose:  Invokes SPS module to handle UL SPS activate for a UE
 *
 *     Invoked by: SCH_UTL
 *
 *  @param[in]   RgSchCellCb        *cell
 *  @param[in]   RgSchUeCb          *ue
 *  @return  Void
 *
 **/
#ifdef ANSI
PUBLIC Void rgSCHCmnUlSpsActInd
(
RgSchCellCb        *cell,
RgSchUeCb          *ue,
U16                spsSduSize
)
#else
PUBLIC Void rgSCHCmnUlSpsActInd(cell, ue,spsSduSize)
RgSchCellCb        *cell;
RgSchUeCb          *ue;
U16                spsSduSize;
#endif
{

   TRC2(rgSCHCmnUlSpsActInd);

   if (ue->ul.ulSpsCfg.isUlSpsEnabled == TRUE)
   {
      rgSCHCmnSpsUlProcActInd(cell, ue,spsSduSize);
   }
   RETVOID;

} /* end of rgSCHCmnUlSpsActInd */

/**
 * @brief This function is called to handle CRC in UL for UEs
 * undergoing SPS release
 *
 * @details
 *
 *     Function: rgSCHCmnUlCrcInd
 *     Purpose:  Invokes SPS module to handle CRC in UL for SPS UE
 *
 *     Invoked by: SCH_UTL
 *
 *  @param[in]   RgSchCellCb        *cell
 *  @param[in]   RgSchUeCb          *ue
 *  @param[in]   CmLteTimingInfo    crcTime
 *  @return  Void
 *
 **/
#ifdef ANSI
PUBLIC Void rgSCHCmnUlCrcInd
(
RgSchCellCb        *cell,
RgSchUeCb          *ue,
CmLteTimingInfo    crcTime
)
#else
PUBLIC Void rgSCHCmnUlCrcInd(cell, ue, crcTime)
RgSchCellCb        *cell;
RgSchUeCb          *ue;
CmLteTimingInfo    crcTime;
#endif
{

   TRC2(rgSCHCmnUlCrcInd);
   if (ue->ul.ulSpsCfg.isUlSpsEnabled == TRUE)
   {
      rgSCHCmnSpsUlProcCrcInd(cell, ue, crcTime);
   }
   RETVOID;

} /* end of rgSCHCmnUlCrcFailInd */

/**
 * @brief This function is called to handle CRC failure in UL
 *
 * @details
 *
 *     Function: rgSCHCmnUlCrcFailInd
 *     Purpose:  Invokes SPS module to handle CRC failure in UL for SPS UE
 *
 *     Invoked by: SCH_UTL
 *
 *  @param[in]   RgSchCellCb        *cell
 *  @param[in]   RgSchUeCb          *ue
 *  @param[in]   CmLteTimingInfo    crcTime
 *  @return  Void
 *
 **/
#ifdef ANSI
PUBLIC Void rgSCHCmnUlCrcFailInd
(
RgSchCellCb        *cell,
RgSchUeCb          *ue,
CmLteTimingInfo    crcTime
)
#else
PUBLIC Void rgSCHCmnUlCrcFailInd(cell, ue, crcTime)
RgSchCellCb        *cell;
RgSchUeCb          *ue;
CmLteTimingInfo    crcTime;
#endif
{

   TRC2(rgSCHCmnUlCrcFailInd);
   if (ue->ul.ulSpsCfg.isUlSpsEnabled == TRUE)
   {
      rgSCHCmnSpsUlProcDtxInd(cell, ue, crcTime);
   }
   RETVOID;

} /* end of rgSCHCmnUlCrcFailInd */

#endif /* LTEMAC_SPS */

/**
 * @brief BCH,BCCH,PCCH Dowlink Scheduling Handler.
 *
 * @details
 *
 *     Function: rgSCHCmnDlBcchPcchAlloc
 *     Purpose:  This function calls common scheduler APIs to
 *     schedule for BCCH/PCCH.
 *     It then invokes Allocator for actual RB
 *     allocations. It processes on the actual resources allocated
 *     against requested to the allocator module.
 *
 *     Invoked by: Common Scheduler
 *
 *  @param[in]  RgSchCellCb *cell
 *  @return  Void
 **/
#ifdef ANSI
PRIVATE Void rgSCHCmnDlBcchPcchAlloc
(
RgSchCellCb  *cell
)
#else
PRIVATE Void rgSCHCmnDlBcchPcchAlloc(cell)
RgSchCellCb  *cell;
#endif
{
   RgSchCmnDlRbAllocInfo  allocInfo;
#ifdef LTE_TDD
   U8           nextSfIdx = (cell->crntSfIdx) % RGSCH_SF_ALLOC_SIZE;
#else
#ifdef LTEMAC_HDFDD
   U8           nextSfIdx = (cell->crntSfIdx + RG_SCH_CMN_HARQ_INTERVAL) % RGSCH_NUM_SUB_FRAMES;
#else
   U8           nextSfIdx = (cell->crntSfIdx) % RGSCH_NUM_SUB_FRAMES;
#endif
#endif
   RgInfSfAlloc *nextsfAlloc = &(cell->sfAllocArr[nextSfIdx]);

   TRC2(rgSCHCmnDlBcchPcchAlloc);


   /* Initializing RgSchCmnUlRbAllocInfo structure for BCCH/PCCH.*/
   cmMemset((U8 *)&allocInfo.pcchAlloc, (U8)0, sizeof(RgSchDlRbAlloc));
   cmMemset((U8 *)&allocInfo.bcchAlloc, (U8)0, sizeof(RgSchDlRbAlloc));

   /*Reset the bitmask for BCCH/PCCH*/
   rgSCHUtlResetSfAlloc(nextsfAlloc,TRUE,FALSE);

#ifdef RGR_SI_SCH
   rgSCHChkNUpdSiCfg(cell);
   rgSCHSelectSi(cell);
#endif

   /*Perform the scheduling for BCCH,PCCH*/
   rgSCHCmnDlBcchPcch(cell, &allocInfo, nextsfAlloc);

   /* Call common allocator for RB Allocation */
   rgSCHBcchPcchDlRbAlloc(cell, &allocInfo);

   /* Finalize the Allocations for reqested Against alloced */
   rgSCHCmnDlBcchPcchFnlz(cell, &allocInfo);

   RETVOID;
}

/**
 * @brief Handles RB allocation for BCCH/PCCH for downlink.
 *
 * @details
 *
 *     Function : rgSCHBcchPcchDlRbAlloc
 *
 *     Invoking Module Processing:
 *     - This function is invoked for DL RB allocation of BCCH/PCCH
 *
 *     Processing Steps:
 *     - If cell is frequency selecive,
 *       - Call rgSCHDlfsBcchPcchAllocRb().
 *     - else,
 *       - Do the processing
 *
 *  @param[in]  RgSchCellCb        *cell
 *  @param[in]  RgSchDlRbAllocInfo *allocInfo
 *  @return  Void
 **/

#ifdef ANSI
PRIVATE Void rgSCHBcchPcchDlRbAlloc
(
RgSchCellCb           *cell,
RgSchCmnDlRbAllocInfo *allocInfo
)
#else
PRIVATE Void rgSCHBcchPcchDlRbAlloc(cell, allocInfo)
RgSchCellCb           *cell;
RgSchCmnDlRbAllocInfo *allocInfo;
#endif
{
   RgSchCmnCell      *cellSch = RG_SCH_CMN_GET_CELL(cell);

   TRC2(rgSCHBcchPcchDlRbAlloc);


   if (cellSch->dl.isDlFreqSel)
   {
      cellSch->apisDlfs->rgSCHDlfsBcchPcchAllocRb(cell, allocInfo);
   }
   else
   {
      rgSCHCmnNonDlfsBcchPcchRbAlloc(cell, allocInfo);
   }

   RETVOID;
}

/**
 * @brief Handles RB allocation for BCCH,PCCH for frequency
 *  non-selective cell.
 *
 * @details
 *
 *     Function : rgSCHCmnNonDlfsBcchPcchRbAlloc
 *
 *     Invoking Module Processing:
 *      - SCH shall invoke this if downlink frequency selective is disabled for
 *        the cell for RB allocation.
 *      - MAX C/I/PFS/RR shall provide the requiredBytes, required RBs
 *        estimate and subframe for each allocation to be made to SCH.
 *
 *     Processing Steps:
 *     - Allocate sequentially for BCCH,PCCH common channels.
 *
 *  @param[in]  RgSchCellCb        *cell
 *  @param[in]  RgSchCmnDlRbAllocInfo *allocInfo
 *  @return  Void
 **/

#ifdef ANSI
PRIVATE Void rgSCHCmnNonDlfsBcchPcchRbAlloc
(
RgSchCellCb           *cell,
RgSchCmnDlRbAllocInfo *allocInfo
)
#else
PRIVATE Void rgSCHCmnNonDlfsBcchPcchRbAlloc(cell, allocInfo)
RgSchCellCb           *cell;
RgSchCmnDlRbAllocInfo *allocInfo;
#endif
{
   RgSchDlRbAlloc     *reqAllocInfo;

   TRC2(rgSCHCmnNonDlfsBcchPcchRbAlloc);

   /* 143473 */
   /* Allocate for PCCH */
   reqAllocInfo = &(allocInfo->pcchAlloc);
   if (reqAllocInfo->rbsReq)
   {
      rgSCHCmnNonDlfsCmnRbAlloc(cell, reqAllocInfo);
   }
   /* Allocate for BCCH on DLSCH */
   reqAllocInfo = &(allocInfo->bcchAlloc);
   if (reqAllocInfo->rbsReq)
   {
      rgSCHCmnNonDlfsCmnRbAlloc(cell, reqAllocInfo);
   }
   RETVOID;
}


#ifdef RGR_SI_SCH
/**
 * @brief This function implements the handling to check and
 *        update the SI cfg at the start of the modificiation period.
 *
 * @details
 *
 *     Function: rgSCHChkNUpdSiCfg
 *     Purpose:  This function implements handling for update of SI Cfg
 *               at the start of modification period.
 *
 *     Invoked by: Scheduler
 *
 *  @param[in]  RgSchCellCb*     cell
 *  @return  S16
 *      -# ROK
 *      -# RFAILED
 **/
#ifdef ANSI
PRIVATE Void rgSCHChkNUpdSiCfg
(
RgSchCellCb             *cell
)
#else
PRIVATE Void rgSCHChkNUpdSiCfg(cell)
RgSchCellCb             *cell;
#endif
{
   CmLteTimingInfo   pdSchTmInfo;

   TRC2(rgSCHChkNUpdSiCfg);


   pdSchTmInfo   = cell->crntTime;
#ifdef LTEMAC_HDFDD
   /* For HDFDD we need scheduling information at least RG_SCH_CMN_DL_DELTA
      + RG_SCH_CMN_HARQ_INTERVAL (7) subframes ahead */
   RGSCH_INCR_SUB_FRAME(pdSchTmInfo, RG_SCH_CMN_DL_DELTA + RG_SCH_CMN_HARQ_INTERVAL);
#else
   RGSCH_INCR_SUB_FRAME(pdSchTmInfo, RG_SCH_CMN_DL_DELTA);
#endif


   /* Updating the SIB1 for Warning SI message immediately after it is received 
    * from application. No need to wait for next modification period.
    */
   if((pdSchTmInfo.sfn % RGSCH_SIB1_RPT_PERIODICITY == 0)
         && (RGSCH_SIB1_TX_SF_NUM == pdSchTmInfo.subframe))
   {   
      /*Check whether SIB1 with PWS has been updated*/
      if(cell->siCb.siBitMask & RGSCH_SI_SIB1_PWS_UPD)
      {
         RGSCH_SET_SI_INFO(cell->siCb.crntSiInfo.sib1Info.sib1,
               cell->siCb.newSiInfo.sib1Info.sib1);
         cell->siCb.crntSiInfo.sib1Info.mcs = 
            cell->siCb.newSiInfo.sib1Info.mcs;
         cell->siCb.crntSiInfo.sib1Info.nPrb = 
             cell->siCb.newSiInfo.sib1Info.nPrb;
         cell->siCb.crntSiInfo.sib1Info.msgLen = 
            cell->siCb.newSiInfo.sib1Info.msgLen;
         cell->siCb.siBitMask &= ~RGSCH_SI_SIB1_PWS_UPD;
      }
   }

   /*Check if this SFN and SF No marks the start of next modification
     period. If current SFN,SF No doesn't marks the start of next
     modification period, then return. */
   if(!((pdSchTmInfo.sfn % cell->siCfg.modPrd == 0)
            && (0 == pdSchTmInfo.subframe)))
   {
      RETVOID;
   }

   /*Check whether MIB has been updated*/
   if(cell->siCb.siBitMask & RGSCH_SI_MIB_UPD)
   {
      RGSCH_SET_SI_INFO(cell->siCb.crntSiInfo.mib,
            cell->siCb.newSiInfo.mib);
      cell->siCb.siBitMask &= ~RGSCH_SI_MIB_UPD;
   }

   /*Check whether SIB1 has been updated*/
   if(cell->siCb.siBitMask & RGSCH_SI_SIB1_UPD)
   {
      RGSCH_SET_SI_INFO(cell->siCb.crntSiInfo.sib1Info.sib1,
            cell->siCb.newSiInfo.sib1Info.sib1);
      cell->siCb.crntSiInfo.sib1Info.mcs = cell->siCb.newSiInfo.sib1Info.mcs;
      cell->siCb.crntSiInfo.sib1Info.nPrb = cell->siCb.newSiInfo.sib1Info.nPrb;
      cell->siCb.crntSiInfo.sib1Info.msgLen = 
         cell->siCb.newSiInfo.sib1Info.msgLen;
      cell->siCb.siBitMask &= ~RGSCH_SI_SIB1_UPD;
   }

   /*Check whether SIs have been updated*/
   if(cell->siCb.siBitMask & RGSCH_SI_SI_UPD)
   {
      U8  idx;

      /*Check if SI cfg have been modified And Check if numSi have
        been changed, if yes then we would need to update the
        pointers for all the SIs */
      if((cell->siCb.siBitMask & RGSCH_SI_SICFG_UPD) &&
            (cell->siCfg.numSi != cell->siCb.newSiCfg.numSi))
      {
         for(idx = 0;idx < cell->siCb.newSiCfg.numSi;idx++)
         {
            RGSCH_SET_SI_INFO(cell->siCb.crntSiInfo.siInfo[idx].si,
                  cell->siCb.newSiInfo.siInfo[idx].si);
            cell->siCb.siArray[idx].si = cell->siCb.crntSiInfo.siInfo[idx].si;
            cell->siCb.siArray[idx].isWarningSi = FALSE;

            cell->siCb.crntSiInfo.siInfo[idx].mcs = cell->siCb.newSiInfo.siInfo[idx].mcs;
            cell->siCb.crntSiInfo.siInfo[idx].nPrb = cell->siCb.newSiInfo.siInfo[idx].nPrb;
            cell->siCb.crntSiInfo.siInfo[idx].msgLen = cell->siCb.newSiInfo.siInfo[idx].msgLen;
         }

         /*If numSi have been reduced then we need to free the
           pointers at the indexes in crntSiInfo which haven't
           been exercised. If numSi has increased then nothing
           additional is requires as above handling has taken
           care.*/
         if(cell->siCfg.numSi > cell->siCb.newSiCfg.numSi)
         {
            for(idx = cell->siCb.newSiCfg.numSi;
                  idx < cell->siCfg.numSi;idx++)
            {
               RGSCH_FREE_MSG(cell->siCb.crntSiInfo.siInfo[idx].si);
               cell->siCb.siArray[idx].si = NULLP;
            }
         }
      }
      else
      {
         /*numSi has not been updated, we just need to update the
           pointers for the SIs which are set to NON NULLP */
         /*ccpu00118260 - Correct Update of SIB2 */
         for(idx = 0;idx < cell->siCfg.numSi;idx++)
         {
            if(NULLP != cell->siCb.newSiInfo.siInfo[idx].si)
            {
               RGSCH_SET_SI_INFO(cell->siCb.crntSiInfo.siInfo[idx].si,
                     cell->siCb.newSiInfo.siInfo[idx].si);

               cell->siCb.siArray[idx].si = cell->siCb.crntSiInfo.siInfo[idx].si;
               cell->siCb.siArray[idx].isWarningSi = FALSE;
               cell->siCb.crntSiInfo.siInfo[idx].mcs = cell->siCb.newSiInfo.siInfo[idx].mcs;
               cell->siCb.crntSiInfo.siInfo[idx].nPrb = cell->siCb.newSiInfo.siInfo[idx].nPrb;
               cell->siCb.crntSiInfo.siInfo[idx].msgLen = cell->siCb.newSiInfo.siInfo[idx].msgLen;
            }
         }
      }
      cell->siCb.siBitMask &= ~RGSCH_SI_SI_UPD;
   }

   /*Check whether SI cfg have been updated*/
   if(cell->siCb.siBitMask & RGSCH_SI_SICFG_UPD)
   {
      cell->siCfg = cell->siCb.newSiCfg;
      cell->siCb.siBitMask &= ~RGSCH_SI_SICFG_UPD;
   }

   RETVOID;
}


/**
 * @brief This function implements the selection of the SI
 *        that is to be scheduled.
 *
 * @details
 *
 *     Function: rgSCHSelectSi
 *     Purpose:  This function implements the selection of SI
 *               that is to be scheduled.
 *
 *     Invoked by: Scheduler
 *
 *  @param[in]  RgSchCellCb*     cell
 *  @return  S16
 *      -# ROK
 *      -# RFAILED
 **/
#ifdef ANSI
PRIVATE Void rgSCHSelectSi
(
RgSchCellCb             *cell
)
#else
PRIVATE Void rgSCHSelectSi(cell)
RgSchCellCb             *cell;
#endif
{
   CmLteTimingInfo        crntTmInfo;
   U8                     siWinSize;
   U16                    x; 
   U16                    windowId; 

   TRC2(rgSCHSelectSi);


   crntTmInfo  = cell->crntTime;
#ifdef LTEMAC_HDFDD
   /* For HDFDD we need scheduling information at least RG_SCH_CMN_DL_DELTA
      + RG_SCH_CMN_HARQ_INTERVAL (7) subframes ahead */
   RGSCH_INCR_SUB_FRAME(crntTmInfo, RG_SCH_CMN_DL_DELTA + RG_SCH_CMN_HARQ_INTERVAL);
#else
   RGSCH_INCR_SUB_FRAME(crntTmInfo, RG_SCH_CMN_DL_DELTA);
#endif

   siWinSize    = cell->siCfg.siWinSize;

   /* Select SI only once at the starting of the new window */
   if(cell->siCb.inWindow != 0)
   {
      if ((crntTmInfo.sfn % cell->siCfg.minPeriodicity) == 0 && 
          crntTmInfo.subframe == 0)
      {
         cell->siCb.inWindow = siWinSize - 1;
      }
      else
      {
         cell->siCb.inWindow--;
         RETVOID;
      }
   }
   else /* New window. Re-init the winSize counter with the window length */
   {
      if((cell->siCb.siArray[cell->siCb.siCtx.siId - 1].isWarningSi == TRUE)&&
            (cell->siCb.siCtx.retxCntRem != 0))   
      {
         rgSCHUtlFreeWarningSiPdu(cell);
         cell->siCb.siCtx.warningSiFlag  = FALSE;
      }

      cell->siCb.inWindow = siWinSize - 1;
   }

   x = rgSCHCmnGetSiSetId(crntTmInfo.sfn, crntTmInfo.subframe, 
                                  cell->siCfg.minPeriodicity); 

   /* Window Id within a SI set. This window Id directly maps to a
    * unique SI Id */
   windowId = (((crntTmInfo.sfn * 10) + 
            crntTmInfo.subframe) - (x * (cell->siCfg.minPeriodicity * 10))) 
                                                               / siWinSize;

   if(windowId >= RGR_MAX_NUM_SI)
      RETVOID;

   /* Update the siCtx if there is a valid SI and its periodicity
    * has occurred */
   if (NULLP != cell->siCb.siArray[windowId].si)
   {
      /* Warning SI Periodicity is same as SIB2 Periodicity */
      if(((cell->siCb.siArray[windowId].isWarningSi == FALSE) && 
               (x % (cell->siCfg.siPeriodicity[windowId]
                     /cell->siCfg.minPeriodicity) == 0)) || 
            ((cell->siCb.siArray[windowId].isWarningSi == TRUE) &&
             (x % (cell->siCfg.siPeriodicity[0]
                   /cell->siCfg.minPeriodicity) == 0)))
      {
         cell->siCb.siCtx.siId = windowId+1;
         cell->siCb.siCtx.retxCntRem = cell->siCfg.retxCnt;
         cell->siCb.siCtx.warningSiFlag = cell->siCb.siArray[windowId].
                                                           isWarningSi;
         cell->siCb.siCtx.timeToTx.sfn = crntTmInfo.sfn;
         cell->siCb.siCtx.timeToTx.subframe = crntTmInfo.subframe;

         RG_SCH_ADD_TO_CRNT_TIME(cell->siCb.siCtx.timeToTx,
               cell->siCb.siCtx.maxTimeToTx, (siWinSize - 1))
      }
   }
   else
   {/* Update the siCtx with invalid si Id */
      cell->siCb.siCtx.siId = 0;
   }

   RETVOID;
}


/**
 * @brief This function implements scheduler DL allocation for
 *        SI.
 *
 * @details
 *
 *     Function: rgSCHDlSiSched
 *     Purpose:  This function implements scheduler for DL allocation
 *               for SI.
 *
 *     Invoked by: Scheduler
 *
 *  @param[in]  RgSchCellCb*     cell
 *  @return  S16
 *      -# ROK
 *      -# RFAILED
 **/
#ifdef ANSI
PRIVATE Void rgSCHDlSiSched
(
RgSchCellCb             *cell,
RgSchCmnDlRbAllocInfo   *allocInfo,
RgInfSfAlloc            *subfrmAlloc
)
#else
PRIVATE Void rgSCHDlSiSched(cell, allocInfo, subfrmAlloc)
RgSchCellCb             *cell;
RgSchCmnDlRbAllocInfo   *allocInfo;
RgInfSfAlloc            *subfrmAlloc;
#endif
{
   CmLteTimingInfo   crntTimInfo;
   RgSchDlSf         *sf;
   U8                nPrb = 0;
   U8                mcs  = 0;
   MsgLen            msgLen = 0;
   U32               rb=0;
   RgSchCmnDlCell    *cellDl = RG_SCH_CMN_GET_DL_CELL(cell);
   /* DwPTS Scheduling Changes Start */
#ifdef LTE_TDD   
   U16                lostRe;  
   U8                 cfi = cellDl->currCfi;      
#endif
   /* DwPTS Scheduling Changes End */

   TRC2(rgSCHDlSiSched);


   crntTimInfo   = cell->crntTime;
#ifdef LTEMAC_HDFDD
   /* For HDFDD we need scheduling information at least RG_SCH_CMN_DL_DELTA
      + RG_SCH_CMN_HARQ_INTERVAL (7) subframes ahead */
   RGSCH_INCR_SUB_FRAME(crntTimInfo, RG_SCH_CMN_DL_DELTA + RG_SCH_CMN_HARQ_INTERVAL);
#else
   RGSCH_INCR_SUB_FRAME(crntTimInfo, RG_SCH_CMN_DL_DELTA);
#endif

   /* Compute the subframe for which allocation is being made.
      Essentially, we need pointer to the dl frame for this subframe */
   sf = rgSCHUtlSubFrmGet(cell, crntTimInfo);

   /*Check if scheduling of MIB is required */
   if((crntTimInfo.sfn % RGSCH_MIB_PERIODICITY == 0)
         && (RGSCH_MIB_TX_SF_NUM == crntTimInfo.subframe))
   {
      MsgLen  mibLen = 0;
      U8      sfnOctet, mibOct2 = 0;
      U8      mibOct1 = 0;
      /*If MIB has not been yet setup by Application, return*/
      if(NULLP == cell->siCb.crntSiInfo.mib)
         RETVOID;

      SFndLenMsg(cell->siCb.crntSiInfo.mib, &mibLen);
      sf->bch.tbSize = mibLen;
      /*Fill the interface information */
      rgSCHUtlFillRgInfCmnLcInfo(sf, subfrmAlloc, NULLD, NULLD);

      /*Set the bits of MIB to reflect SFN */
      /*First get the Most signficant 8 bits of SFN */
      sfnOctet = (U8)(crntTimInfo.sfn >> 2);
      /*Get the first two octets of MIB, and then update them
        using the SFN octet value obtained above.*/
      if(ROK != SExamMsg((Data *)(&mibOct1),
               cell->siCb.crntSiInfo.mib, 0))
         RETVOID;

      if(ROK != SExamMsg((Data *)(&mibOct2),
               cell->siCb.crntSiInfo.mib, 1))
         RETVOID;

      /* ccpu00114572- Fix for improper way of MIB Octet setting for SFN */
      mibOct1 = (mibOct1 & 0xFC) | (sfnOctet >> 6);
      mibOct2 = (mibOct2 & 0x03) | (sfnOctet << 2);
      /* ccpu00114572- Fix ends*/

      /*Now, replace the two octets in MIB */
      if(ROK != SRepMsg((Data)(mibOct1),
               cell->siCb.crntSiInfo.mib, 0))
         RETVOID;

      if(ROK != SRepMsg((Data)(mibOct2),
               cell->siCb.crntSiInfo.mib, 1))
         RETVOID;

      /*Copy the MIB msg buff into interface buffer */
      SCpyMsgMsg(cell->siCb.crntSiInfo.mib,
            rgSchCb[cell->instIdx].rgSchInit.region,
            rgSchCb[cell->instIdx].rgSchInit.pool,
            &subfrmAlloc->cmnLcInfo.bchInfo.pdu);
      /* Added Dl TB count for MIB message transmission
       * This counter is incremented 4 times to consider 
       * the retransmission at the PHY level on PBCH channel*/
#ifdef LTE_L2_MEAS
      cell->dlUlTbCnt.tbTransDlTotalCnt += RG_SCH_MIB_CNT;
#endif      
   }

   allocInfo->bcchAlloc.schdFirst = FALSE;
   /*Check if scheduling of SIB1 is required.
     Check of (crntTimInfo.sfn % RGSCH_SIB1_PERIODICITY == 0)
     is not required here since the below check takes care
     of SFNs applicable for this one too.*/
   if((crntTimInfo.sfn % RGSCH_SIB1_RPT_PERIODICITY == 0)
         && (RGSCH_SIB1_TX_SF_NUM == crntTimInfo.subframe))
   {
      /*If SIB1 has not been yet setup by Application, return*/
      if(NULLP == (cell->siCb.crntSiInfo.sib1Info.sib1))
      {
         RETVOID;
      }

      allocInfo->bcchAlloc.schdFirst = TRUE;
      mcs =  cell->siCb.crntSiInfo.sib1Info.mcs;
      nPrb =  cell->siCb.crntSiInfo.sib1Info.nPrb;
      msgLen =  cell->siCb.crntSiInfo.sib1Info.msgLen;
   }
   else
   {
      /*Check if scheduling of SI can be performed.*/
      Bool    invalid = FALSE;

      if(cell->siCb.siCtx.siId == 0)
         RETVOID;

      /*Check if the Si-Window for the current Si-Context is completed*/
      invalid = rgSCHCmnChkPastWin(crntTimInfo, cell->siCb.siCtx.maxTimeToTx);
      if(invalid)
      {
         /* LTE_ADV_FLAG_REMOVED_START */
         if(cell->siCb.siCtx.retxCntRem)
         { 
            RGSCHLOGERROR(cell->instIdx,ERRCLS_INT_PAR,ERG011,(ErrVal)cell->siCb.siCtx.siId,
                                "rgSCHDlSiSched(): SI not scheduled and window expired");
         }
         /* LTE_ADV_FLAG_REMOVED_END */
         if(cell->siCb.siCtx.warningSiFlag == TRUE)
         {
            rgSCHUtlFreeWarningSiPdu(cell);
            cell->siCb.siCtx.warningSiFlag  = FALSE;
         }
         RETVOID;
      }

      /*Check the timinginfo of the current SI-Context to see if its
        transmission can be scheduled. */
      if(FALSE == (rgSCHCmnChkInWin(crntTimInfo,
                  cell->siCb.siCtx.timeToTx,
                  cell->siCb.siCtx.maxTimeToTx)))
      {
         RETVOID;

      }
      /*Check if retransmission count has become 0*/
      if(0 == cell->siCb.siCtx.retxCntRem)
      {
         RETVOID;
      }

      /* LTE_ADV_FLAG_REMOVED_START */
      /* Check if ABS is enabled/configured  */
      if(RGR_ENABLE == cell->lteAdvCb.absCfg.status)
      {
         /* The pattern type is RGR_ABS_MUTE, then eNB need to blank the subframe */
         if(cell->lteAdvCb.absCfg.absPatternType & RGR_ABS_MUTE)
         {
            /* Determine next scheduling subframe is ABS or not */
            if(RG_SCH_ABS_ENABLED_ABS_SF == (RgSchAbsSfEnum)(cell->lteAdvCb.absCfg.absPattern
                  [((crntTimInfo.sfn*RGSCH_NUM_SUB_FRAMES) + crntTimInfo.subframe) % RGR_ABS_PATTERN_LEN]))
            {
               /* Skip the SI scheduling to next tti */
               RETVOID;
            }
         }
      }
      /* LTE_ADV_FLAG_REMOVED_END */

      /*Schedule the transmission of the current SI-Context */
      /*Find out the messg length for the SI message */
      /* warningSiFlag is to differentiate between Warning SI
       * and Other SI */
        if((rgSCHUtlGetMcsAndNPrb(cell, &nPrb, &mcs, &msgLen)) != ROK)
        {
           RETVOID; 
        }

      cell->siCb.siCtx.i = RGSCH_CALC_SF_DIFF(crntTimInfo,
            cell->siCb.siCtx.timeToTx);
   } 


   /*Get the number of rb required */
   /*rgSCHCmnClcRbAllocForFxdTb(cell, msgLen, cellDl->ccchCqi, &rb);*/
   if(cellDl->bitsPerRb==0)
   {
      while ((rgTbSzTbl[0][0][rb]) < (U32) (msgLen*8))
      {
         rb++;
      }
      rb = rb+1;
   }
   else
   {
      rb = RGSCH_CEIL((msgLen*8), cellDl->bitsPerRb);
   }
   /* DwPTS Scheduling Changes Start */   
#ifdef LTE_TDD
   if (sf->sfType == RG_SCH_SPL_SF_DATA) 
   {
      RGSCH_GET_SPS_SF_CFI(cell->bwCfg.dlTotalBw, cfi);

      /* Calculate the less RE's because of DwPTS */
       lostRe = rb * (cellDl->noResPerRb[cfi] - cellDl->numReDwPts[cfi]);

       /* Increase number of RBs in Spl SF to compensate for lost REs */
       rb += RGSCH_CEIL(lostRe, cellDl->numReDwPts[cfi]); 
   }
#endif
   /* DwPTS Scheduling Changes End */   
   /*ccpu00115595- end*/
   /* Additional check to see if required RBs
    * exceeds the available */
   if (rb > sf->bw - sf->bwAssigned)
   {
      RLOG_ARG1(L_ERROR,DBG_CELLID,cell->cellId,  "rgSCHDlSiSched(): "
         "BW allocation failed CRNTI:%d",RGSCH_SI_RNTI);
      RETVOID;
   }

   /* Update the subframe Allocated BW field */
   sf->bwAssigned = sf->bwAssigned + rb;

   /*Fill the parameters in allocInfo */
   allocInfo->bcchAlloc.rnti = RGSCH_SI_RNTI;
   allocInfo->bcchAlloc.dlSf = sf;
   /* Qcom: SI Number of RIV correction */
//   allocInfo->bcchAlloc.rbsReq = nPrb;
   allocInfo->bcchAlloc.rbsReq = rb;
#if 0
   if(rb != nPrb)
   {
      QU_CRITLOG(QU_LOG_MESSAGE,"(%d, %d):rb=(%d),nPrb=(%d),msgLen(%d)",
            crntTimInfo.sfn, crntTimInfo.subframe, rb, nPrb, msgLen);
   }
#endif
   /*ccpu00116710- MCS is not getting assigned */
   allocInfo->bcchAlloc.tbInfo[0].imcs = mcs;

   /* ccpu00117510 - ADD - Assignment of nPrb and other information */
   allocInfo->bcchAlloc.nPrb = nPrb;
   allocInfo->bcchAlloc.tbInfo[0].bytesReq = msgLen;
   allocInfo->bcchAlloc.tbInfo[0].noLyr = 1;
   RETVOID;
}
#endif /*RGR_SI_SCH*/


/* ccpu00117452 - MOD - Changed macro name from
   RGR_RRM_DLPWR_CNTRL to RGR_CQI_REPT */
#ifdef RGR_CQI_REPT
/**
 * @brief This function Updates the DL CQI for the UE.
 *
 * @details
 *
 *     Function: rgSCHCmnUeDlPwrCtColltCqiRept
 *     Purpose:  Manages PUSH N CQI reporting
 *         Step 1: Store the CQI in collation array
 *         Step 2: Increament the tracking count
 *         Step 3: Check is it time to to send the report
 *         Step 4: if yes, Send StaInd to RRM
 *         Step 4.1: Fill StaInd for sending collated N CQI rpeorts
 *         Step 4.2: Call utility function (rgSCHUtlRgrStaInd) to send rpts to RRM
 *         Step 4.2.1: If sending was not sucessful, return RFAILED
 *         Step 4.2.2: If sending was sucessful, return ROK
 *         Step 5: If no, return
 *     Invoked by: rgSCHCmnDlCqiInd
 *
 *  @param[in]  RgSchCellCb        *cell
 *  @param[in]  RgSchUeCb          *ue
 *  @param[in]  RgrUeCqiRept        *ueCqiRpt
 *  @return  Void
 *
 **/
#ifdef ANSI
PRIVATE S16 rgSCHCmnUeDlPwrCtColltCqiRept
(
RgSchCellCb        *cell,
RgSchUeCb          *ue,
RgrUeCqiRept        *ueCqiRpt
)
#else
PRIVATE S16 rgSCHCmnUeDlPwrCtColltCqiRept(cell, ue, ueCqiRpt)
RgSchCellCb        *cell;
RgSchUeCb          *ue;
RgrUeCqiRept        *ueCqiRpt;
#endif
{
   U8    *cqiCount = NULLP;
   S16   retVal;
   RgrStaIndInfo *staInfo = NULLP;

   TRC2(rgSCHCmnUeDlPwrCtColltCqiRept)

   /* Step 1: Store the CQI in collation array */
   /* Step 2: Increament the tracking count */
   cqiCount = &(ue->schCqiInfo.cqiCount);
   ue->schCqiInfo.cqiRept[(*cqiCount)++] =
                  *ueCqiRpt;


   /* Step 3: Check is it time to to send the report */
   if(RG_SCH_CQIR_IS_TIMTOSEND_CQIREPT(ue))
   {
   /* Step 4: if yes, Send StaInd to RRM */
      retVal = rgSCHUtlAllocSBuf (cell->instIdx,(Data**)&staInfo,
               sizeof(RgrStaIndInfo));
      if (retVal != ROK)
      {
         RLOG_ARG1(L_ERROR,DBG_CELLID,cell->cellId, "Could not "
            "allocate memory for sending StaInd CRNTI:%d",ue->ueId);
         RETVALUE(retVal);
      }

   /* Step 4.1: Fill StaInd for sending collated N CQI rpeorts */
      retVal = rgSCHUtlFillSndStaInd(cell, ue, staInfo,
            ue->cqiReptCfgInfo.numColltdCqiRept);
      RETVALUE(retVal);

   }

   RETVALUE(ROK);
} /* End of rgSCHCmnUeDlPwrCtColltCqiRept */

#endif /* End of RGR_CQI_REPT */

/**
 * @brief This function checks for the retransmisson
 *        for a DTX scenario.
 * @details
 *
 *     Function:
 *     Purpose:
 *     Invoked by:
 *
 *  @param[in]  RgSchCellCb        *cell
 *  @param[in]  RgSchUeCb          *ue
 *  @param[in]
 *  @return  Void
 *
 **/
#ifdef ANSI
PUBLIC Void rgSCHCmnChkRetxAllowDtx
(
RgSchCellCb        *cell,
RgSchUeCb          *ueCb,
RgSchDlHqProcCb    *proc,
Bool               *reTxAllwd
)
#else
PUBLIC Void rgSCHCmnChkRetxAllowDtx(cell, ueCb, proc, reTxAllwd)
RgSchCellCb        *cell;
RgSchUeCb          *ueCb;
RgSchDlHqProcCb    *proc;
Bool               *reTxAllwd;
#endif
{
   TRC3(rgSCHCmnChkRetxAllowDtx)


   *reTxAllwd = TRUE;
   /* Fix */
   if ((proc->tbInfo[0].isAckNackDtx == TFU_HQFDB_DTX))
   {
       *reTxAllwd = FALSE;
   }

   RETVOID;
}

/**
 * @brief API for calculating the SI Set Id 
 *
 * @details
 *
 *     Function: rgSCHCmnGetSiSetId
 *
 *     This API is used for calculating the SI Set Id, as shown below
 *     
 *          siSetId = 0        siSetId = 1
 *     |******************|******************|---------------->
 *   (0,0)              (8,0)              (16,0)          (SFN, SF)
 *    
 *
 *  @param[in]  U16     sfn                   
 *  @param[in]  U8      sf
 *  @return     U16     siSetId
 **/
#ifdef ANSI
PRIVATE U16 rgSCHCmnGetSiSetId
(
U16    sfn,
U8     sf,
U16    minPeriodicity
)
#else
PRIVATE U16 rgSCHCmnGetSiSetId(sfn, sf, minPeriodicity)
U16    sfn;
U8     sf
U16    minPeriodicity;
#endif
{
   /* 80 is the minimum SI periodicity in sf. Also
    * all other SI periodicities are multiples of 80 */
    RETVALUE (((sfn * 10) + sf) / (minPeriodicity * 10));
}
#ifdef LTE_TDD
/**
 * @brief API for calculating the DwPts Rb, Itbs and  tbSz 
 *
 * @details
 *
 *     Function: rgSCHCmnCalcDwPtsTbSz
 *
 *  @param[in]     RgSchCellCb    *cell                   
 *  @param[in]     U32             bo
 *  @param[in/out] U8             *rb
 *  @param[in/out] U8             *iTbs
 *  @param[in]     U8              lyr
 *  @param[in]     U8              cfi
 *  @return        U32             tbSz
 **/
#ifdef ANSI
PRIVATE U32 rgSCHCmnCalcDwPtsTbSz
(
RgSchCellCb    *cell,
U32             bo,
U8             *rb,
U8             *iTbs,
U8              lyr,
U8              cfi
)
#else
PRIVATE U32 rgSCHCmnCalcDwPtsTbSz(cell, bo, rb, iTbs, lyr, cfi)
RgSchCellCb    *cell;
U32             bo;
U8             *rb;
U8             *iTbs;
U8              lyr;
U8              cfi;
#endif
{
    U32             tbSz;
    RgSchCmnDlCell *cellDl     = RG_SCH_CMN_GET_DL_CELL(cell);
    U32             numRE      = *rb * cellDl->noResPerRb[cfi];
    U32             numDwPtsRb = RGSCH_CEIL(numRE, cellDl->numReDwPts[cfi]);   

    TRC2(rgSCHCmnCalcDwPtsTbSz);

    /* DwPts Rb cannot exceed the cell Bw */
    numDwPtsRb = RGSCH_MIN(numDwPtsRb, cellDl->maxDlBwPerUe);
    
    /* Adjust the iTbs for optimum usage of the DwPts region. 
     * Using the same iTbs adjustment will not work for all 
     * special subframe configurations and iTbs levels. Hence use the 
     * static iTbs Delta table for adjusting the iTbs  */
    RG_SCH_CMN_ADJ_DWPTS_ITBS(cellDl, *iTbs);
    
    if (bo)
    {
       while(rgTbSzTbl[lyr-1][*iTbs][RGSCH_MAX(numDwPtsRb*3/4,1)-1] < bo*8 &&
             numDwPtsRb < cellDl->maxDlBwPerUe) 
       {
          (numDwPtsRb)++;
       }

       tbSz = rgTbSzTbl[lyr-1][*iTbs][RGSCH_MAX(numDwPtsRb*3/4,1)-1];
    }
    else
    {
       tbSz = rgTbSzTbl[lyr-1][*iTbs][RGSCH_MAX(numDwPtsRb*3/4,1)-1];
    }
    *rb = numDwPtsRb;

    RETVALUE(tbSz/8);
}

/**
 * @brief API for calculating the DwPts Rb, Itbs and  tbSz 
 *
 * @details
 *
 *     Function: rgSCHCmnCalcDwPtsTbSz2Cw
 *
 *  @param[in]      RgSchCellCb    *cell                   
 *  @param[in]      U32             bo
 *  @param[in/out]  U8             *rb
 *  @param[in]      U8              maxRb
 *  @param[in/out]  U8             *iTbs1
 *  @param[in/out]  U8             *iTbs2
 *  @param[in]      U8              lyr1
 *  @param[in]      U8              lyr2
 *  @return[in/out] U32            *tb1Sz
 *  @return[in/out] U32            *tb2Sz
 *  @param[in]      U8              cfi 
 **/
#ifdef ANSI
PRIVATE Void rgSCHCmnCalcDwPtsTbSz2Cw
(
RgSchCellCb    *cell,
U32             bo,
U8             *rb,
U8              maxRb,
U8             *iTbs1,
U8             *iTbs2,
U8              lyr1,
U8              lyr2,
U32            *tb1Sz, 
U32            *tb2Sz,
U8              cfi
)
#else
PRIVATE Void rgSCHCmnCalcDwPtsTbSz2Cw(cell, bo, rb, maxRb, iTbs1, iTbs2, 
      lyr1, lyr2, tb1Sz, tb2Sz, cfi)
RgSchCellCb    *cell;
U32             bo;
U8             *rb;
U8              maxRb;
U8             *iTbs1;
U8             *iTbs2;
U8              lyr1;
U8              lyr2;
U32            *tb1Sz; 
U32            *tb2Sz;
U8              cfi;
#endif
{
    RgSchCmnDlCell *cellDl     = RG_SCH_CMN_GET_DL_CELL(cell);
    U32             numRE      = *rb * cellDl->noResPerRb[cfi];
    U32             numDwPtsRb = RGSCH_CEIL(numRE, cellDl->numReDwPts[cfi]);   

    TRC2(rgSCHCmnCalcDwPtsTbSz2Cw);

    /* DwPts Rb cannot exceed the cell Bw */
    numDwPtsRb = RGSCH_MIN(numDwPtsRb, maxRb);
    
    /* Adjust the iTbs for optimum usage of the DwPts region. 
     * Using the same iTbs adjustment will not work for all 
     * special subframe configurations and iTbs levels. Hence use the 
     * static iTbs Delta table for adjusting the iTbs  */
    RG_SCH_CMN_ADJ_DWPTS_ITBS(cellDl, *iTbs1);
    RG_SCH_CMN_ADJ_DWPTS_ITBS(cellDl, *iTbs2);
    
    while((rgTbSzTbl[lyr1-1][*iTbs1][RGSCH_MAX(numDwPtsRb*3/4,1)-1] +
           rgTbSzTbl[lyr2-1][*iTbs2][RGSCH_MAX(numDwPtsRb*3/4,1)-1])< bo*8 &&
          numDwPtsRb < maxRb) 
    {
       (numDwPtsRb)++;
    }

    *tb1Sz = rgTbSzTbl[lyr1-1][*iTbs1][RGSCH_MAX(numDwPtsRb*3/4,1)-1]/8;
    *tb2Sz = rgTbSzTbl[lyr2-1][*iTbs2][RGSCH_MAX(numDwPtsRb*3/4,1)-1]/8;

    *rb = numDwPtsRb;

    RETVOID;    
}

#endif

/**
 * @brief Updates the GBR LCGs when datInd is received from MAC
 * 
 * @details
 *
 *     Function: rgSCHCmnUpdUeDataIndLcg(cell, ue, datInd)
 *     Purpose:  This function updates the GBR LCGs 
 *               when datInd is received from MAC.
 *
 *     Invoked by: TOM
 *
 *  @param[in]  RgSchCellCb      *cell
 *  @param[in]  RgSchUeCb        *ue
 *  @param[in]  RgInfUeDatInd    *datInd
 *  @return Void
 **/
#ifdef ANSI
PUBLIC Void rgSCHCmnUpdUeDataIndLcg 
(
RgSchCellCb    *cell,
RgSchUeCb      *ue,
RgInfUeDatInd  *datInd
)
#else
PUBLIC Void rgSCHCmnUpdUeDataIndLcg(cell, ue, datInd)
RgSchCellCb    *cell;
RgSchUeCb      *ue;
RgInfUeDatInd  *datInd;
#endif
{
   U32 idx = 0;
   RgSchCmnCell *cellSch = RG_SCH_CMN_GET_CELL(cell);
   RgSchCmnUlUe *ueUl    = RG_SCH_CMN_GET_UL_UE(ue);

   TRC2(rgSCHCmnUpdUeDataIndLcg);

   for (idx = 0; (idx < RGINF_MAX_LCG_PER_UE - 1); idx++)
   {
      if (datInd->lcgInfo[idx].bytesRcvd != 0)
      {
         U8  lcgId     = datInd->lcgInfo[idx].lcgId;
         U32 bytesRcvd = datInd->lcgInfo[idx].bytesRcvd;

         if (RGSCH_LCG_ISCFGD(&ue->ul.lcgArr[lcgId]))
         {
            RgSchCmnLcg *cmnLcg = ((RgSchCmnLcg *)(ue->ul.lcgArr[lcgId].sch));
            if (RGSCH_IS_GBR_BEARER(cmnLcg->cfgdGbr))
            {
               if(bytesRcvd > cmnLcg->effGbr)
               {
                  bytesRcvd -= cmnLcg->effGbr;
                  cmnLcg->effDeltaMbr = (cmnLcg->effDeltaMbr > bytesRcvd) ? \
                                   (cmnLcg->effDeltaMbr - bytesRcvd) : (0);
                  cmnLcg->effGbr = 0;
               }
               else
               {
                  cmnLcg->effGbr -= bytesRcvd;
               }
	       /* To keep BS updated with the amount of data received for the GBR */
	       cmnLcg->reportedBs = (cmnLcg->reportedBs > datInd->lcgInfo[idx].bytesRcvd) ? \
				    (cmnLcg->reportedBs - datInd->lcgInfo[idx].bytesRcvd) : (0);
	       cmnLcg->bs = RGSCH_MIN(cmnLcg->reportedBs, cmnLcg->effGbr+cmnLcg->effDeltaMbr);
	    }
            else if(lcgId != 0)
            {
               ueUl->effAmbr = (ueUl->effAmbr > datInd->lcgInfo[idx].bytesRcvd) ? \
                               (ueUl->effAmbr - datInd->lcgInfo[idx].bytesRcvd) : (0);
	       cmnLcg->reportedBs = (cmnLcg->reportedBs > datInd->lcgInfo[idx].bytesRcvd) ? \
				    (cmnLcg->reportedBs - datInd->lcgInfo[idx].bytesRcvd) : (0);
	       cmnLcg->bs = RGSCH_MIN(cmnLcg->reportedBs, ueUl->effAmbr);
               ueUl->nonGbrLcgBs = (ueUl->nonGbrLcgBs > datInd->lcgInfo[idx].bytesRcvd) ? \
                                   (ueUl->nonGbrLcgBs - datInd->lcgInfo[idx].bytesRcvd) : (0);
            }
            ueUl->nonLcg0Bs = (ueUl->nonLcg0Bs > datInd->lcgInfo[idx].bytesRcvd) ? \
                              (ueUl->nonLcg0Bs - datInd->lcgInfo[idx].bytesRcvd) : (0);
         }
      }
      else
      {
         break;
      }
   }
   if (cellSch->apisUl->rgSCHRgrUlLcgUpd(cell, ue, datInd) != ROK)
   {
      RLOG_ARG0(L_ERROR,DBG_CELLID,cell->cellId," rgSCHCmnUpdUeDataIndLcg(): rgSCHRgrUlLcgUpd returned failure");
   }
}

#ifdef DL_LA
/**
 * @brief Check & Updates the TM Mode chnage threashold based on cqiiTbs and
 * actual iTbs
 * 
 * @details
 *
 *     Function: rgSCHCmnSendTxModeInd(cell, ueUl, newTxMode)
 *     Purpose:  This function sends the TX mode Change 
 *               indication to RRM
 *     change
 *
 *     Invoked by: CMN
 *
 *  @param[in]  RgSchCellCb      *cell
 *  @param[in]  RgSchUeCb        *ue
 *  @param[in]  U8               newTxMode
 *  @return Void
 **/
#ifdef ANSI
PRIVATE Void rgSCHCmnSendTxModeInd 
(
RgSchCellCb    *cell,
RgSchUeCb      *ue,
U8             newTxMode
)
#else
PRIVATE Void rgSCHCmnSendTxModeInd(cell, ue, newTxMode)
RgSchCellCb    *cell;
RgSchUeCb      *ue;
U8             newTxMode;
#endif
{ 
   RgmTransModeInd   *txModeChgInd;
   RgSchCmnDlUe      *ueDl =  RG_SCH_CMN_GET_DL_UE(ue);

   TRC2(rgSCHCmnSendTxModeInd);

   if(!(ueDl->mimoInfo.forceTD & RG_SCH_CMN_TD_TXMODE_RECFG))
   {
      /* Mem Alloc */
      if(SGetSBuf(cell->rgmSap->sapCfg.sapPst.region,
               cell->rgmSap->sapCfg.sapPst.pool, (Data**)&txModeChgInd,
               sizeof(RgmTransModeInd)) != ROK)
      {
         RETVOID;
      }
      RG_SCH_FILL_RGM_TRANSMODE_IND(ue->ueId, cell->cellId, newTxMode, txModeChgInd);
      RgUiRgmChangeTransModeInd(&(cell->rgmSap->sapCfg.sapPst),
            cell->rgmSap->sapCfg.suId, txModeChgInd);
   }

   ue->mimoInfo.txModUpChgFactor = 0;
   ue->mimoInfo.txModDownChgFactor = 0;
   ueDl->laCb[0].deltaiTbs = 0;

   RETVOID;
}

/**
 * @brief Check & Updates the TM Mode chnage threashold based on cqiiTbs and
 * actual iTbs
 * 
 * @details
 *
 *     Function: rgSchCheckAndTriggerModeChange(cell, ueUl, iTbsNew)
 *     Purpose:  This function update and check for threashold for TM mode
 *     change
 *
 *     Invoked by: CMN
 *
 *  @param[in]  RgSchCellCb      *cell
 *  @param[in]  RgSchUeCb        *ue
 *  @param[in]  U8               iTbs
 *  @return Void
 **/
#ifdef ANSI
PUBLIC Void rgSchCheckAndTriggerModeChange
(
RgSchCellCb    *cell,
RgSchUeCb      *ue,
U8             reportediTbs,
U8             previTbs,
U8             maxiTbs
)
#else
PUBLIC Void rgSchCheckAndTriggerModeChange(cell, ue, reportediTbs, previTbs, maxiTbs)
RgSchCellCb    *cell;
RgSchUeCb      *ue;
U8             reportediTbs;
U8             previTbs;
U8             maxiTbs;
#endif
{
   RgrTxMode          txMode;       /*!< UE's Transmission Mode */
   RgrTxMode          modTxMode;       /*!< UE's Transmission Mode */

   TRC2(rgSchCheckAndTriggerModeChange);

   txMode = ue->mimoInfo.txMode;

   /* Check for Step down */
   /* Step down only when TM4 is configured. */
   if(RGR_UE_TM_4 == txMode)
   {
      if((previTbs <= reportediTbs) && ((reportediTbs - previTbs) >= RG_SCH_MODE_CHNG_STEPDOWN_CHECK_FACTOR))
      {
         ue->mimoInfo.txModDownChgFactor += RG_SCH_MODE_CHNG_STEPUP_FACTOR;
      }
      else
      {
         ue->mimoInfo.txModDownChgFactor -= RG_SCH_MODE_CHNG_STEPDOWN_FACTOR;
      }

      ue->mimoInfo.txModDownChgFactor =  
         RGSCH_MAX(ue->mimoInfo.txModDownChgFactor, -(RG_SCH_MODE_CHNG_STEPDOWN_THRSHD));

      if(ue->mimoInfo.txModDownChgFactor >= RG_SCH_MODE_CHNG_STEPDOWN_THRSHD)
      {
         /* Trigger Mode step down */
         modTxMode = RGR_UE_TM_3;
         rgSCHCmnSendTxModeInd(cell, ue, modTxMode);
      }
   }

   /* Check for Setup up */
   /* Step Up only when TM3 is configured, Max possible Mode is TM4*/
   if(RGR_UE_TM_3 == txMode)
   {
      if((previTbs > reportediTbs) || (maxiTbs == previTbs))
      {
         ue->mimoInfo.txModUpChgFactor += RG_SCH_MODE_CHNG_STEPUP_FACTOR;
      }
      else
      {
         ue->mimoInfo.txModUpChgFactor -= RG_SCH_MODE_CHNG_STEPDOWN_FACTOR;
      }

      ue->mimoInfo.txModUpChgFactor = 
         RGSCH_MAX(ue->mimoInfo.txModUpChgFactor, -(RG_SCH_MODE_CHNG_STEPUP_THRSHD));

      /* Check if TM step up need to be triggered */
      if(ue->mimoInfo.txModUpChgFactor >= RG_SCH_MODE_CHNG_STEPUP_THRSHD)
      {
         /* Trigger mode chnage */
         modTxMode =  RGR_UE_TM_4;
         rgSCHCmnSendTxModeInd(cell, ue, modTxMode);
      }
   }

   RETVOID;
}
#endif

/**
* @brief Updates the GBR LCGs when datInd is received from MAC
 * 
 * @details
 *
 *     Function: rgSCHCmnIsDlCsgPrio (cell)
 *     Purpose:  This function returns if csg UEs are
 *               having priority at current time
 *
 *     Invoked by: Scheduler
 *
 *  @param[in]  RgSchCellCb      *cell
 *  @param[in]  RgSchUeCb        *ue
 *  @param[in]  RgInfUeDatInd    *datInd
 *  @return Void
 **/
#ifdef ANSI
PUBLIC Bool rgSCHCmnIsDlCsgPrio
(
RgSchCellCb    *cell
)
#else
PUBLIC Bool rgSCHCmnIsDlCsgPrio(cell)
RgSchCellCb    *cell;
#endif
{
  
   RgSchCmnDlCell *cmnDlCell = RG_SCH_CMN_GET_DL_CELL(cell);
 
   TRC2(rgSCHCmnIsDlCsgPrio)
   /* Calculating the percentage resource allocated */
   if(RGR_CELL_ACCS_HYBRID != cell->csgAccsMode)
   {
      RETVALUE(FALSE);
   }
   else
   {
      if(((cmnDlCell->ncsgPrbCnt * 100) / cmnDlCell->totPrbCnt) < cell->minDlResNonCsg)
      {
         RETVALUE(FALSE);
      }
      else
      {
         RETVALUE(TRUE);
      }
   }
}

/**
* @brief Updates the GBR LCGs when datInd is received from MAC
 * 
 * @details
 *
 *     Function: rgSCHCmnIsUlCsgPrio (cell)
 *     Purpose:  This function returns if csg UEs are
 *               having priority at current time
 *
 *     Invoked by: Scheduler
 *
 *  @param[in]  RgSchCellCb      *cell
 *  @param[in]  RgSchUeCb        *ue
 *  @param[in]  RgInfUeDatInd    *datInd
 *  @return Void
 **/
#ifdef ANSI
PUBLIC Bool rgSCHCmnIsUlCsgPrio
(
RgSchCellCb    *cell
)
#else
PUBLIC Bool rgSCHCmnIsUlCsgPrio(cell)
RgSchCellCb    *cell;
#endif
{
   RgSchCmnUlCell *cmnUlCell = RG_SCH_CMN_GET_UL_CELL(cell);
 
   TRC2(rgSCHCmnIsUlCsgPrio)

   /* Calculating the percentage resource allocated */
   if(RGR_CELL_ACCS_HYBRID != cell->csgAccsMode)
   {
      RETVALUE(FALSE);
   }
   else
   {
      if (((cmnUlCell->ncsgPrbCnt * 100) /cmnUlCell->totPrbCnt) < cell->minUlResNonCsg)
      {
         RETVALUE(FALSE);
      }
      else
      {
         RETVALUE(TRUE);
      }
   }
}
/**********************************************************************

  End of file:     gk_sch_cmn.c@@/main/2 - Sat Jul 30 02:21:36 2011

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
/main/1   ---    apany         1. LTE MAC 2.1 release
       rg001.201   sm          1. ccpu00106531: fixing incorrect
                                  Imcs derivation. Inappropriate name
                                  of CQI to ITbs table for DL has been
                                  corrected.
                               2. ccpu00107387: To include the length
                                  and ModOrder in DataRecp Req.
                               3. ccpu00107637: Nprb indication at
                                  PHY for common Channel.
                   sd          4. ccpu00107640: Corrected RACH handling for
                                  multiple RAPIDs per RARNTI
       rg002.201   sd          1. ccpu00108814: Corrected handling for SIB2
                               2. ccpu00108815: Corrected timing for SIB2 handling
                               3. ccpu00108811: Corrected allocation for
                                  common channels
                               4. ccpu00108813: Added rnti filling up for
                                  BCCH/PCCH
                               5. ccpu00108809: Added sfIdx calculation for
                                  TDD as well
                               6. ccpu00108812: Corrected the computation
                                  of RV for BCCH
      rg003.201    sm          1. ccpu00109625: Incorrect dlsf pointer
                                  assignment correction.
                               2. ccpu00107638: Added support for non-adaptive
                                  retransmission in Uplink.
                               3. ccpu00107391: Added support for msg3
                                  retransmission.
                               4. Trace added for functions.
      rg004.201    sd          1. ccpu00105914: PHR handling for MSG3
                               2. ccpu00109160: Np calculation correction
                               3. ccpu00109162: DAI value correction
                   ms          4. DL DTX Handling
                               5. Cell Configuration - Bandwidth allocation
                   pk          6. ccpu00109921: PBCH Allocation
      rg004.201    rnt         1. Modified for SI Enhancement
      rg005.201    sm          1. Number of Transmissions - 1 should be
                                  passed to UHM.
                               2. Renamed rcvdDatInd to rcvdCrcInd.
      rg006.201    ms          1. ccpu00110949: DL-SCH TB Size Change
      rg006.201    sd          1. ccpu00112398: Added periodic BSR timer
                               2. ccpu000111706, ccpu00112394:
                                  TFU_DELTA dependency removed and added
                                  dependency on RG_UL_DELTA dependency
                               3. [ccpu000111706, ccpu00112394]: Separated TTI
                                  processing for UL and DL.
                               4. [ccpu00112789]: Added configuration for
                                  maximum number of MSG3s.
      rg007.201    ap          1. Added support for MIMO
      rg008.201    sd          1. Removed dependency on MIMO compile-time flag
                   dv          2. ccpu00114572: Fix for Improper SFN setting in MIB Octet.
      rg009.201    rsharon     1. [ccpu00112372]:Changes for CR timer.
                               2. [ccpu00115756]:Removed DTX from
                                  TFU_UPGRADE flag.
                               3. ccpu00115239: Fix for harq proc rls in CCCH
                                   if allocation fails
                               4. Replaced direct access of subFrms for rgSchCellCb with
                                  access through function rgSCHUtlSubFrmGet.[ccpu00115595]
                                  if allocation fails.
                               5. ccpu00115595: Use bcchpcchrarspcoderate
                                  for num rb calculation.
                               6. Added changes of TFU_UPGRADE
                               7. ccpu00116520- Guarding Cavium Specific code under
                                  CAV_SUPPORT flag.
/main/2  ---       gvj         1. Updated for LTE MAC Rel 3.1
      rg001.301   nudupi       1. ccpu00119023: Filling TFU params for RETX.
                               2. ccpu00118643: Adding the changes for the Init num Rbs.
                               3. Added the changes for the ticket ccpu00118350 .
                               4. ccpu00119213 : avoiding wraparound.
                               5. ccpu00119227 : Generalized Cavium
      rg002.301  asehgal       1. ccpu00120274: added validation checks for RI.
                               2. ccpu00120092: added check to limit numSb to
                                  holeSz.
                               3. ccpu00120703: added check to limit msg3 mcs
                                  to 15.
                               4. ccpu00120621: fixed wrong validation of
                                  non-GBR qci.
                               5. ccpu00119118 Added the changes for the warnings
                                arising out of cross compilation.
                               6. ccpu00119227 : Generalized Cavium
                               7. ccpu00119333/34:Correcting wrap around issue.
                               8. ccpu00119227 : Generalized Cavium
                               9.ccpu00117606:inclusion of CRS
                               10.ccpu00119901:coderate exceeding 0.93
                               11.ccpu00118423:rb allocated exceeding max limit
                               12.ccpu00109543:cqi to mcs violation
                               13.ccpu00120391:PDSCH allocations overlapping
                               14.ccpu00118351: seperate UL & DL deltas.
      rg003.301   hsingh       1. ccpu00120794-Freeing up the HARQ proc blocked
                                  for indefinite time in case of Retx
$SID$  ---        rt           1. LTE MAC 4.1 release
*********************************************************************91*/
