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

     Name:     EnodeB Application

     Type:     C source file

     Desc:     This file contains 
               

     File:     wr_smm_enbapp_cmnplatutils.c

     Sid:      $SID$ 


**********************************************************************/

static const char* RLOG_MODULE_NAME="SMM";
static int RLOG_MODULE_ID=2;
static int RLOG_FILE_ID=39;
#include<stdlib.h>
#include "wr.h"
#include "wr_emm.h"
#include "wr_lwr.h"
#include "wr_cmn.h"
#include "wr_smm_smallcell.h"
#include "wr_smm_init.h"
#include "wr_utils.h"
#include "wr_msm_common.h" 
#include "lrm.x"
#include "wr_umm.h"
#include "wr_smm_watchdog_tmr.h"
#include <math.h>
#include "wr_smm_qcom_oam_init.h"
Void smStaticCfgFailAlarm(Void);
PUBLIC S16  MsmSmmDynCfgReq(Void *cfg, U32 cfgTyp, U8 priority);
EXTERN Void MsmConfigComplete(Void);
EXTERN Void MsmConfigFailure(Void);
EXTERN Void SmUpdateSwVersion(const char *swver);
EXTERN Void SmUpdateN1pucch(U16 n1Pucch);
EXTERN S16  smRegInitTmr ARGS((Void));
EXTERN MsmLteeNodeBparams lteeNodeBparams;
EXTERN MsmWcdmaNodeBparams wcdmaNodeBparams;
EXTERN MsmFaultMgmtParams faultMgmtParams;
EXTERN Bool sendKick;
#define SCTP_TIME_FROM_MS_TO_SEC 100
PUBLIC Void smBuildShutRestart(Void);
//PRIVATE S16 smHdlDynMmeCfg(MsmLteeNodeBIPparams *enbIpCfg);
PRIVATE Void smBldSib1Cfg(LwrMngmt *lwrMng,U8 cfgGrpIndex, U8 accessMode);
PRIVATE Void smBldSib2Cfg(LwrMngmt *lwrMng,U8 cfgGrpIndex);
PRIVATE Void smBldSib3Cfg(LwrMngmt *lwrMng,U8 cfgGrpIndex);
PRIVATE Void smBldRabPolicy(LwrMngmt *lwrMng,U8 cfgGrpIndex);
PRIVATE Void smBldMeasCellCfg(LwrMngmt   *lwrMng,U8 cfgGrpIndex);
PRIVATE S16 smInitNghHashList(Void);
PRIVATE S16 smInitDbgCfg(Void);
PRIVATE Void wrSmmSetBwFrmBwIdx  (U32  bwIdx);
PUBLIC Void smFillNghUtraTddFreqParam(LwrNeighUtraFreq *lwrNghUtraFreq, MsmLteNeighUtraTddFreq *msmNghUtraFreq, U32 reCfgType);
PUBLIC Void smFillNghUtraTddCellParam(LwrUtraNeighCell *lwrNghUtraTddCell, MsmUtraTddNeighCell *msmNghUtraTddCell, U32 reCfgType);
EXTERN U16 wrSmSctpPort;
EXTERN Bool sendKick;
#ifdef RM_INTF
PUBLIC S16 smBuildRmuProtMmeCfg(U16 mmeId, LrmEnbConfiguration *enbCfg, U32 action);
EXTERN S16 smBuildRmuProtCellCfg(LrmCellConfiguration *cellCfg);
EXTERN S16 smBuildRmuProtEnbCfg(LrmEnbConfiguration *enbCfg);
EXTERN S16 SmHdlNghEurtaAddCellCfg(LwrEUTRANeighCell *eutraCella);
EXTERN S16 SmHdlNghUrtaAddCellCfg(LwrUtraNeighCell *utranCell);
EXTERN Void SmUpdateMibParms(U32 msgType, U32 action, Void* Cfg);
EXTERN S16 wrFillSectorId ARGS((
   TknStrOSXL  *ptr,
   U8          len,
   Data        *val
));
#ifdef TENB_STATS
EXTERN S16 TSInfInitStatsFrmWork ARGS((
   Bool   tenbSocketStats,
   Bool   tenbFileStats,
   Bool   tenbConsoleStats,
   U32    tenbStatsPeriodicity,
   U32    tendStatsSocketPort
));
EXTERN Void TSInfSetEnbIp ARGS((
   U32 enbIpAddr
));
#endif
/*
*
*       Fun:   smBuildRmuProtCellCfg
*
*       Desc:  This function is used for filling cell configuration in RRM
*
*       Ret:   ROK
*
*       Notes: None
*              
*       File:  wr_sm_wr_cntrl.c 
*
*/
#ifdef ANSI 
PUBLIC S16 smBuildRmuProtCellCfg(LrmCellConfiguration *cellCfg)
#else
PUBLIC S16 smBuildRmuProtCellCfg(cellCfg)
LrmCellConfiguration *cellCfg;
#endif
{

   U16 usNumSpsUePerTti;
   U16 usTotalSpsN1PucchRes;
#ifdef LTE_TDD
   U8              maxUeSchdPerUlSf[LRM_UL_DL_CONFIG_MAX] = {1,2,4,5,7,9,2};
#endif   
   U8 acbMask = 0x00;
 
    TRC2(smBuildRmuProtCellCfg);

    cellCfg->enCellAddModDel = LRM_PROTOCFG_ADD_CELL;

    cellCfg->stCellCfgInfo.bCellId              = 1; /*currently we support only one cell*/
    cellCfg->stCellCfgInfo.enCellDlBandWidth    = lteeNodeBparams.lteCellMibCfgGrp.dlBw[0];
    cellCfg->stCellCfgInfo.usMaxUlBroadbandBw   = 10; /*TODO*/
    cellCfg->stCellCfgInfo.usMaxDlBroadbandBw   = 10; /*TODO*/
    cellCfg->stCellCfgInfo.bNRbCqi              = lteeNodeBparams.ltePucchCfgCommon.nRBCqi;
    cellCfg->stCellCfgInfo.bNCsAn               = lteeNodeBparams.ltePucchCfgCommon.nCSAN;
    cellCfg->stCellCfgInfo.bMaxActiveUesServed  = lteeNodeBparams.lteSmCellCfg.maxUeSupp;
    cellCfg->stCellCfgInfo.bMaxSigUesServed     = lteeNodeBparams.lteSmCellCfg.maxUeSupp + WR_MAX_SR_CQI_NOT_CONFIGURED_UES;  /*TODO*/

    /* HCSG_DEV */
    /* adding 25% delta over max UE supported */
    cellCfg->stCellCfgInfo.usMaxExtraUes    = 
       ceil(cellCfg->stCellCfgInfo.bMaxActiveUesServed * 0.25);

    cellCfg->stCellCfgInfo.flagDlfsScheduling   = lteeNodeBparams.lteCellDlSchdCfgGrp.dlfsSchd;
    cellCfg->stCellCfgInfo.bWaitTime            = 3;  /*TODO: Pick from OAM*/
    cellCfg->stCellCfgInfo.stAcbInfo.bAcBarringEmergency = lteeNodeBparams.lteAcBarringInfo.acBarringForEmergency;
    cellCfg->stCellCfgInfo.bRcmAlgoType = 1; /* TODO */
    /* RRM_SP1_START */
	  cellCfg->stCellCfgInfo.bMaxAvgGbrPrbUsage = lteeNodeBparams.lteSmCellCfg.maxAvgGbrPrbUsage; 
	  cellCfg->stCellCfgInfo.bRbcQci1TmngReq = lteeNodeBparams.lteSmCellCfg.rrmQci1TmngReq; 
	  cellCfg->stCellCfgInfo.bRbcQci2TmngReq = lteeNodeBparams.lteSmCellCfg.rrmQci2TmngReq; 
	  cellCfg->stCellCfgInfo.bRbcQci3TmngReq = lteeNodeBparams.lteSmCellCfg.rrmQci3TmngReq; 
	  cellCfg->stCellCfgInfo.bRbcQci4TmngReq = lteeNodeBparams.lteSmCellCfg.rrmQci4TmngReq; 
	  cellCfg->stCellCfgInfo.bRbcQci4TmngReqEnable = lteeNodeBparams.lteSmCellCfg.rrmQci4TmngReqEnable; 
     cellCfg->stCellCfgInfo.bDuplexMode = lteeNodeBparams.lteAddCellCfg.duplexMode;
     cellCfg->stCellCfgInfo.bNumDlNewUesPerTti = lteeNodeBparams.lteCellDlSchdCfgGrp.maxUePerDlSf;
     cellCfg->stCellCfgInfo.bNumUlNewUesPerTti = lteeNodeBparams.lteCellUlSchdCfgGrp.maxUePerUlSf;
     cellCfg->stCellCfgInfo.bArpForEmergencyBearer = lteeNodeBparams.lteAddCellCfg.emergencyServicePriorityLevel;
     /* reserving default number of DL re-Tx UEs per TTI to 1 */
     cellCfg->stCellCfgInfo.bNumDlReTxUesPerTti = wrSmDfltMinDlReTxUesPerTti;
     cellCfg->stCellCfgInfo.bNumUlReTxUesPerTti = wrSmDfltMinUlReTxUesPerTti;
    /* RRM_SP1_END */

    /*RBC_SP1_X*/
	 cellCfg->stCellCfgInfo.utNumOfPrbRpts         = lteeNodeBparams.lteSmCellCfg.numOfPrbRpts; 
	 cellCfg->stCellCfgInfo.utPrbRptInterval       = lteeNodeBparams.lteSmCellCfg.prbRptIntvl; 
    /*RBC_SP1_Y*/

    /*Filling ACB params*/
    cellCfg->stCellCfgInfo.stAcbInfo.bIsacBarringMoSig = 
       lteeNodeBparams.lteAcBarringInfo.barringSignalUsage;
    cellCfg->stCellCfgInfo.stAcbInfo.bIsacBarringMoData = 
       lteeNodeBparams.lteAcBarringInfo.barringDataUsage;
    cellCfg->stCellCfgInfo.stAcbInfo.stAcBarringMoSig.enAcbTime = 
       (CmRrmAcBarringTime)lteeNodeBparams.lteAcBarringInfo.acBarringForMOSignal.acBarringTime;

    if(lteeNodeBparams.lteAcBarringInfo.acBarringForMOSignal.acBarringAC11)
    {
       acbMask |= LRM_ACB_SPECIAL_AC_11; 
    }
    if(lteeNodeBparams.lteAcBarringInfo.acBarringForMOSignal.acBarringAC12)
    {
       acbMask |= LRM_ACB_SPECIAL_AC_12;
    }
    if(lteeNodeBparams.lteAcBarringInfo.acBarringForMOSignal.acBarringAC13)
    {
       acbMask |= LRM_ACB_SPECIAL_AC_13;
    }
    if(lteeNodeBparams.lteAcBarringInfo.acBarringForMOSignal.acBarringAC14)
    {
       acbMask |= LRM_ACB_SPECIAL_AC_14;
    }
    if(lteeNodeBparams.lteAcBarringInfo.acBarringForMOSignal.acBarringAC15)
    {
       acbMask |= LRM_ACB_SPECIAL_AC_15;
    }

    cellCfg->stCellCfgInfo.stAcbInfo.stAcBarringMoSig.bAcbForSpac = acbMask; 

    if(acbMask && 
          (lteeNodeBparams.lteAcBarringInfo.acBarringForMOSignal.acBarringFactor != 0))
    {
       cellCfg->stCellCfgInfo.stAcbInfo.stAcBarringMoSig.enAcbFactor = RM_ACB_FACTOR_P00; 
    }
    else
    {
       cellCfg->stCellCfgInfo.stAcbInfo.stAcBarringMoSig.enAcbFactor = (CmRrmAcBarringFactor) 
         lteeNodeBparams.lteAcBarringInfo.acBarringForMOSignal.acBarringFactor;
    }

    acbMask = 0x00;

    cellCfg->stCellCfgInfo.stAcbInfo.stAcBarringMoData.enAcbTime = (CmRrmAcBarringTime)
       lteeNodeBparams.lteAcBarringInfo.acBarringForMOData.acBarringTime;

    if(lteeNodeBparams.lteAcBarringInfo.acBarringForMOData.acBarringAC11)
    {
       acbMask |= LRM_ACB_SPECIAL_AC_11; 
    }
    if(lteeNodeBparams.lteAcBarringInfo.acBarringForMOData.acBarringAC12)
    {
       acbMask |= LRM_ACB_SPECIAL_AC_12;
    }
    if(lteeNodeBparams.lteAcBarringInfo.acBarringForMOData.acBarringAC13)
    {
       acbMask |= LRM_ACB_SPECIAL_AC_13;
    }
    if(lteeNodeBparams.lteAcBarringInfo.acBarringForMOData.acBarringAC14)
    {
       acbMask |= LRM_ACB_SPECIAL_AC_14;
    }
    if(lteeNodeBparams.lteAcBarringInfo.acBarringForMOData.acBarringAC15)
    {
       acbMask |= LRM_ACB_SPECIAL_AC_15;
    }

    cellCfg->stCellCfgInfo.stAcbInfo.stAcBarringMoData.bAcbForSpac = acbMask; 

    if(acbMask && 
          (lteeNodeBparams.lteAcBarringInfo.acBarringForMOData.acBarringFactor != 0))
    {
       cellCfg->stCellCfgInfo.stAcbInfo.stAcBarringMoData.enAcbFactor = RM_ACB_FACTOR_P00; 
    }
    else
    {
       cellCfg->stCellCfgInfo.stAcbInfo.stAcBarringMoData.enAcbFactor = (CmRrmAcBarringFactor) 
         lteeNodeBparams.lteAcBarringInfo.acBarringForMOData.acBarringFactor;
    }

    cellCfg->stCellCfgInfo.flagTransMode        = lteeNodeBparams.lteTransModeCellCfg.transModeUse;
    RLOG1(L_INFO,"DLFSS is %d",cellCfg->stCellCfgInfo.flagDlfsScheduling);
#ifdef INTEL_FDD_NODLFSS
    if(cellCfg->stCellCfgInfo.flagDlfsScheduling == 1)
    {
       RLOG0(L_ERROR,"DLFSS is not supported in INTEL FDD, so disabling it");
       cellCfg->stCellCfgInfo.flagDlfsScheduling = 0;
    }
#endif
    /* CR Fix :- ccpu00136285 and ccpu00137294*/
    if (MSM_LWR_AP_CNT_1 == lteeNodeBparams.lteAntenna_Common_Config.numAntPorts)
    {
       cellCfg->stCellCfgInfo.bNumTxAnt = 1;
    }
    else if(MSM_LWR_AP_CNT_2 == lteeNodeBparams.lteAntenna_Common_Config.numAntPorts)
    {
       cellCfg->stCellCfgInfo.bNumTxAnt = 2;
    }
    else if(MSM_LWR_AP_CNT_4 == lteeNodeBparams.lteAntenna_Common_Config.numAntPorts)
    {
       cellCfg->stCellCfgInfo.bNumTxAnt = 4;
    }
    else
    {
       WR_DBGP(WR_ERROR,(WR_PRNT_BUF,
         "Number of Antenna ports %d configured from OAM is invalid\n",
         lteeNodeBparams.lteAntenna_Common_Config.numAntPorts));
    }
	 cellCfg->stCellCfgInfo.enSrPrdcty           = lteeNodeBparams.lteRrmTddParam.srPrdcty;
	 cellCfg->stCellCfgInfo.enCqiPrdcty          = lteeNodeBparams.lteRrmTddParam.cqiPrdcty;
	 cellCfg->stCellCfgInfo.usNumSrResPerTti     = lteeNodeBparams.lteRrmTddParam.numSrPerTti;
	 cellCfg->stCellCfgInfo.usNumCqiResPerTti    = lteeNodeBparams.lteRrmTddParam.numCqiPerTti;
	 cellCfg->stCellCfgInfo.bSimulCqiAckNack     = lteeNodeBparams.lteRrmTddParam.simulAckNackEnb;
#ifdef LTE_TDD 
    cellCfg->stCellCfgInfo.enUlDlConfig         = lteeNodeBparams.lteTddParam.subframeAssignment; /* TODO 1*/
    cellCfg->stCellCfgInfo.enSplSfCfg           = lteeNodeBparams.lteTddParam.specialSubFramePattern; /* TODO 7*/
#endif
    cellCfg->stCellCfgInfo.enDsrTransMax        = wrSmDfltDsrTransMax; /* TODO 7*/

    cellCfg->stCellCfgInfo.enSrPrdcty           = lteeNodeBparams.lteRrmTddParam.srPrdcty;
    cellCfg->stCellCfgInfo.enCqiPrdcty          = lteeNodeBparams.lteRrmTddParam.cqiPrdcty;
    cellCfg->stCellCfgInfo.usNumSrResPerTti     = lteeNodeBparams.lteRrmTddParam.numSrPerTti;
    cellCfg->stCellCfgInfo.usNumCqiResPerTti    = lteeNodeBparams.lteRrmTddParam.numCqiPerTti;

    cellCfg->stCellCfgInfo.enUlDlConfig         = lteeNodeBparams.lteTddParam.subframeAssignment; /* TODO 1*/
    cellCfg->stCellCfgInfo.enSplSfCfg           = lteeNodeBparams.lteTddParam.specialSubFramePattern; /* TODO 7*/
    cellCfg->stCellCfgInfo.usN1Pucch            = lteeNodeBparams.ltePucchCfgCommon.n1PucchAn;
    cellCfg->stCellCfgInfo.enDsrTransMax        = wrSmDfltDsrTransMax; /* TODO 7*/
    /*Removed DRX Flag */
    cellCfg->stCellCfgInfo.stRmCellDrxCfg.bIsDrxEnabled = lteeNodeBparams.lteDrxCfgParams.isDrxEnabled;
    cellCfg->stCellCfgInfo.stRmCellDrxCfg.usDrxInactvTmr = lteeNodeBparams.lteDrxCfgParams.drxInactvTmr;
    cellCfg->stCellCfgInfo.stRmCellDrxCfg.bDrxRetxTmr = (U8)lteeNodeBparams.lteDrxCfgParams.drxRetxTmr;

    cellCfg->stCellCfgInfo.stRmCellDrxCfg.usLongDrxCycleGbr    = lteeNodeBparams.lteDrxCfgParams.drxLongCycleOffst.longDrxCycleGbr; 
    cellCfg->stCellCfgInfo.stRmCellDrxCfg.usLongDrxCycleNonGbr = lteeNodeBparams.lteDrxCfgParams.drxLongCycleOffst.longDrxCycleNonGbr; 
   /*DRX QCI*/
   cellCfg->stCellCfgInfo.stRmCellDrxCfg.bIsGbrDrxDis = lteeNodeBparams.lteDrxCfgParams.isDRXQCIEnabled;
 
   /* SPS changes starts */
   cellCfg->stCellCfgInfo.stSpsCellCfg.bIsSpsEnabled = lteeNodeBparams.stSpsCellCfg.bIsSpsEnabled;
   if (cellCfg->stCellCfgInfo.stSpsCellCfg.bIsSpsEnabled == TRUE)
   {
      cellCfg->stCellCfgInfo.stSpsCellCfg.usMaxDlSpsUePerTti = lteeNodeBparams.stSpsCellCfg.maxDlSpsUePerTti;
      cellCfg->stCellCfgInfo.stSpsCellCfg.usMaxUlSpsUePerTti = lteeNodeBparams.stSpsCellCfg.maxUlSpsUePerTti;
   }
   /* SPS changes ends */


      if(cellCfg->stCellCfgInfo.stSpsCellCfg.usMaxDlSpsUePerTti <
            cellCfg->stCellCfgInfo.stSpsCellCfg.usMaxUlSpsUePerTti)
      {
         usNumSpsUePerTti = cellCfg->stCellCfgInfo.stSpsCellCfg.usMaxUlSpsUePerTti;
      }
      else
      {
         usNumSpsUePerTti = cellCfg->stCellCfgInfo.stSpsCellCfg.usMaxDlSpsUePerTti;
      }
#ifdef LTE_TDD 
      usTotalSpsN1PucchRes = usNumSpsUePerTti * \
                   maxUeSchdPerUlSf[cellCfg->stCellCfgInfo.enUlDlConfig];
#else
      usTotalSpsN1PucchRes = usNumSpsUePerTti;
#endif 

      if( lteeNodeBparams.ltePucchCfgCommon.n1PucchAn <
            (cellCfg->stCellCfgInfo.usNumSrResPerTti + usTotalSpsN1PucchRes))
      {
         RLOG3(L_ERROR,"N1 PUCCH resources configured are not enough."
               "So Changing it. "
               "usN1Pucch(%d), bNumSrPerTti(%d), usTotalSpsN1PucchRes(%d)\n",
               lteeNodeBparams.ltePucchCfgCommon.n1PucchAn,
               cellCfg->stCellCfgInfo.usNumSrResPerTti, usTotalSpsN1PucchRes);
         lteeNodeBparams.ltePucchCfgCommon.n1PucchAn =
            cellCfg->stCellCfgInfo.usNumSrResPerTti + usTotalSpsN1PucchRes;
         SmUpdateN1pucch(lteeNodeBparams.ltePucchCfgCommon.n1PucchAn);
      }
      cellCfg->stCellCfgInfo.usN1Pucch  = lteeNodeBparams.ltePucchCfgCommon.n1PucchAn;

     /* HCSG_Dev Start */
     if(lteeNodeBparams.lteFapAccessParams.accessMode == WR_ENB_HYBRID_ACCESS)
     {
        cellCfg->stCellCfgInfo.usMaxNonCsgUes = 
           lteeNodeBparams.lteFapAccessParams.maxNonCSGMembers;
        cellCfg->stCellCfgInfo.usMaxCsgUes    = 
           lteeNodeBparams.lteFapAccessParams.maxCSGMembers;
        /* adding 25% delta over max UE supported */
        cellCfg->stCellCfgInfo.usMaxExtraUes    = 
           ceil(cellCfg->stCellCfgInfo.bMaxActiveUesServed * 0.25);
     }
     else
     {
        cellCfg->stCellCfgInfo.usMaxNonCsgUes = 0;
        cellCfg->stCellCfgInfo.usMaxCsgUes    = lteeNodeBparams.lteSmCellCfg.maxUeSupp;
        cellCfg->stCellCfgInfo.usMaxExtraUes  =  0;
     }
        cellCfg->stCellCfgInfo.usAccessMode   = 
           lteeNodeBparams.lteFapAccessParams.accessMode;
     /* HCSG_Dev End */

    RETVALUE(ROK);
}


/* IPSec Recovery */

/*
*
*       Fun:   smBuildRmuProtCellDel
*
*       Desc:  This function is used for filling cell configuration in RRM
*
*       Ret:   ROK
*
*       Notes: None
*              
*       File:  wr_sm_wr_cntrl.c 
*
*/
#ifdef ANSI 
PUBLIC S16 smBuildRmuProtCellDel(LrmCellConfiguration *cellCfg)
#else
PUBLIC S16 smBuildRmuProtCellDel(cellCfg)
LrmCellConfiguration *cellCfg;
#endif
{
    TRC2(smBuildRmuProtCellDel);

    cellCfg->enCellAddModDel = LRM_PROTOCFG_DEL_CELL;

    cellCfg->stCellCfgInfo.bCellId              = 1; /*currently we support only one cell*/
#if 1
    cellCfg->stCellCfgInfo.enCellDlBandWidth    = wrSmDfltCellDlBW;
    cellCfg->stCellCfgInfo.usMaxUlBroadbandBw   = 10; /*TODO*/
    cellCfg->stCellCfgInfo.usMaxDlBroadbandBw   = 10; /*TODO*/
    cellCfg->stCellCfgInfo.bNRbCqi              = lteeNodeBparams.ltePucchCfgCommon.nRBCqi;
    cellCfg->stCellCfgInfo.bNCsAn               = lteeNodeBparams.ltePucchCfgCommon.nCSAN;
    cellCfg->stCellCfgInfo.bMaxActiveUesServed  = lteeNodeBparams.lteSmCellCfg.maxUeSupp;
    cellCfg->stCellCfgInfo.bMaxSigUesServed     = lteeNodeBparams.lteSmCellCfg.maxUeSupp + WR_MAX_SR_CQI_NOT_CONFIGURED_UES; /*TODO*/
    /* RRM_SP1_START */
	  cellCfg->stCellCfgInfo.bMaxAvgGbrPrbUsage = lteeNodeBparams.lteSmCellCfg.maxAvgGbrPrbUsage; 
	  cellCfg->stCellCfgInfo.bRbcQci1TmngReq = lteeNodeBparams.lteSmCellCfg.rrmQci1TmngReq; 
	  cellCfg->stCellCfgInfo.bRbcQci2TmngReq = lteeNodeBparams.lteSmCellCfg.rrmQci2TmngReq; 
	  cellCfg->stCellCfgInfo.bRbcQci3TmngReq = lteeNodeBparams.lteSmCellCfg.rrmQci3TmngReq; 
	  cellCfg->stCellCfgInfo.bRbcQci4TmngReq = lteeNodeBparams.lteSmCellCfg.rrmQci4TmngReq; 
	  cellCfg->stCellCfgInfo.bRbcQci4TmngReqEnable = lteeNodeBparams.lteSmCellCfg.rrmQci4TmngReqEnable; 
     cellCfg->stCellCfgInfo.bDuplexMode = lteeNodeBparams.lteAddCellCfg.duplexMode;
     cellCfg->stCellCfgInfo.bNumDlNewUesPerTti = lteeNodeBparams.lteCellDlSchdCfgGrp.maxUePerDlSf;
     cellCfg->stCellCfgInfo.bNumUlNewUesPerTti = lteeNodeBparams.lteCellUlSchdCfgGrp.maxUePerUlSf;
     cellCfg->stCellCfgInfo.bArpForEmergencyBearer = lteeNodeBparams.lteAddCellCfg.emergencyServicePriorityLevel;

     /* reserving default number of DL re-Tx UEs per TTI to 1 */
     cellCfg->stCellCfgInfo.bNumDlReTxUesPerTti = wrSmDfltMinDlReTxUesPerTti;
     cellCfg->stCellCfgInfo.bNumUlReTxUesPerTti = wrSmDfltMinUlReTxUesPerTti;
    /* RRM_SP1_END */
    cellCfg->stCellCfgInfo.flagDlfsScheduling   = lteeNodeBparams.lteCellDlSchdCfgGrp.dlfsSchd;
    cellCfg->stCellCfgInfo.bWaitTime            = 3;  /*TODO: Pick from OAM*/
    cellCfg->stCellCfgInfo.stAcbInfo.bAcBarringEmergency = lteeNodeBparams.lteAcBarringInfo.acBarringForEmergency;
      cellCfg->stCellCfgInfo.stAcbInfo.bIsacBarringMoData = lteeNodeBparams.lteAcBarringInfo.barringDataUsage;
      cellCfg->stCellCfgInfo.stAcbInfo.bIsacBarringMoSig = lteeNodeBparams.lteAcBarringInfo.barringSignalUsage;
    cellCfg->stCellCfgInfo.flagTransMode        = lteeNodeBparams.lteTransModeCellCfg.transModeUse;
    RLOG1(L_INFO,"DLFSS is %d",cellCfg->stCellCfgInfo.flagDlfsScheduling);
#ifdef INTEL_FDD_NODLFSS
    if(cellCfg->stCellCfgInfo.flagDlfsScheduling == 1)
    {
       RLOG0(L_ERROR,"DLFSS is not supported in INTEL FDD, so disabling it");
       cellCfg->stCellCfgInfo.flagDlfsScheduling = 0;
    }
#endif
    cellCfg->stCellCfgInfo.bRcmAlgoType = 1; /* TODO */
    /* CR Fix :- ccpu00136285 and ccpu00137294*/
    if (MSM_LWR_AP_CNT_1 == lteeNodeBparams.lteAntenna_Common_Config.numAntPorts)
    {
       cellCfg->stCellCfgInfo.bNumTxAnt = 1;
    }
    else if(MSM_LWR_AP_CNT_2 == lteeNodeBparams.lteAntenna_Common_Config.numAntPorts)
    {
       cellCfg->stCellCfgInfo.bNumTxAnt = 2;
    }
    else if(MSM_LWR_AP_CNT_4 == lteeNodeBparams.lteAntenna_Common_Config.numAntPorts)
    {
       cellCfg->stCellCfgInfo.bNumTxAnt = 4;
    }
    else
    {
       WR_DBGP(WR_ERROR,(WR_PRNT_BUF,
      " The Default value of lteeNodeBparams.lteAntenna_Common_Config.?numAntPort ?%d is wrong\n",
      lteeNodeBparams.lteAntenna_Common_Config.numAntPorts));
    }

    cellCfg->stCellCfgInfo.stRmCellDrxCfg.bIsDrxEnabled = lteeNodeBparams.lteDrxCfgParams.isDrxEnabled;
    cellCfg->stCellCfgInfo.stRmCellDrxCfg.usDrxInactvTmr = lteeNodeBparams.lteDrxCfgParams.drxInactvTmr;
    cellCfg->stCellCfgInfo.stRmCellDrxCfg.bDrxRetxTmr = (U8)lteeNodeBparams.lteDrxCfgParams.drxRetxTmr;

    cellCfg->stCellCfgInfo.stRmCellDrxCfg.usLongDrxCycleGbr    = lteeNodeBparams.lteDrxCfgParams.drxLongCycleOffst.longDrxCycleGbr; 
    cellCfg->stCellCfgInfo.stRmCellDrxCfg.usLongDrxCycleNonGbr = lteeNodeBparams.lteDrxCfgParams.drxLongCycleOffst.longDrxCycleNonGbr; 
#endif 
    RETVALUE(ROK);
}


/* IPSec Recovery */


/*
*
*       Fun:   smBuildRmuProtEnbCfg
*
*       Desc:  This function is used for filling Enb configuration in RRM
*
*       Ret:   ROK
*
*       Notes: None
*              
*       File:  wr_sm_wr_cntrl.c 
*
*/
#ifdef ANSI 
PUBLIC S16 smBuildRmuProtEnbCfg(LrmEnbConfiguration *enbCfg)
#else
PUBLIC S16 smBuildRmuProtEnbCfg(enbCfg)
LrmEnbConfiguration *enbCfg;
#endif
{
   TRC2(smBuildRmuProtEnbCfg);

   enbCfg->bCfgType = RRM_ENB_CFG_SYS_CFG;
   enbCfg->u.bMaxNumCells = 1;

   RETVALUE(ROK);
}

/* ccpu00132596 */

/** @brief This function is responsible for retrieving the Enum
 *         based on the default pciRange and Mapping it to the 
 *         exact PCI Range Value.
 *
 * @details
 *
 *     Function: wrSmmPciRangeMapping
 *
 *         Processing steps:
 *         - Get the default pciRange.
 *         - Based on the Enum Value return the exact PCI Range Value. 
 *
 * @param[in] pciRange     : pointer to the default pciRange
 * 
 * @return U16
 *     -# Exact PCI Range Value
 */
PRIVATE U16 wrSmmPciRangeMapping
(
LwrSib4CsgInfo            *pciRange
)
{
   U16   mappedPci = 0;

   if(pciRange->isCsgPciCfgd == WR_PRSNT)
   {
      switch(pciRange->csgPciRange)
      {
         case NhuPhysCellIdRangerangen4Enum : 
            mappedPci = 4;
            break;
         case NhuPhysCellIdRangerangen8Enum :
            mappedPci = 8;
            break;
         case NhuPhysCellIdRangerangen12Enum :
            mappedPci = 12;
            break;
         case NhuPhysCellIdRangerangen16Enum :
            mappedPci = 16;
            break;
         case NhuPhysCellIdRangerangen24Enum :
            mappedPci = 24;
            break;
         case NhuPhysCellIdRangerangen32Enum :    
            mappedPci = 32;
            break;
         case NhuPhysCellIdRangerangen48Enum :
            mappedPci = 48;
            break;
         case NhuPhysCellIdRangerangen64Enum :
            mappedPci = 64;
            break;
         case NhuPhysCellIdRangerangen84Enum :
            mappedPci = 84;
            break;
         case NhuPhysCellIdRangerangen96Enum :
            mappedPci = 96;
            break;
         case NhuPhysCellIdRangerangen128Enum :
            mappedPci = 128;
            break;
         case NhuPhysCellIdRangerangen168Enum :
            mappedPci = 168;
            break;
         case NhuPhysCellIdRangerangen252Enum :
            mappedPci = 252;
            break;
         case NhuPhysCellIdRangerangen504Enum :
            mappedPci = 504;
            break;
         default :
            RLOG1(L_ERROR, "PCI Range value not found for PCI entry %d",
               pciRange->csgPciRange);
                    break;
      }
   }
   RETVALUE(mappedPci);
}

/** @brief This function is responsible for retrieving the pciRange
 *         and checking whether the final PCI is in the valid region
 *         of 0 to 503. If not set to default values.
 *
 * @details
 *
 *     Function: wrSmmIsPciRangeValid
 *
 *         Processing steps:
 *         - Get the default pciRange.
 *         - Based on the Enum Value return the exact PCI Range Value. 
 *         - Add the startPci & PciRange. Check whether this added value is b/n 0-503
 *         - If not in 0 - 503 range then set to default values.
 *
 * @param[in] pciRange     : pointer to the default pciRange
 * 
 */
PRIVATE Void wrSmmIsPciRangeValid
( 
LwrSib4CsgInfo            *pciRange
)
{
   U16        mappedPciRange = 0;
   U16        rangeofPci= 0;

   mappedPciRange = wrSmmPciRangeMapping(pciRange);


   if(mappedPciRange != 0)
   {
      rangeofPci = pciRange->csgStartPhyCellId + (mappedPciRange - 1);
   }
   else
   {
      rangeofPci = pciRange->csgStartPhyCellId + mappedPciRange; 
   }

   if(rangeofPci >= 504)
   {
      RLOG2(L_ERROR, "For CSG start PhyCellIdID %d and PCI range %d, the PCI is "
         "exceeding 504, so setting PCI range to default values ",
         pciRange->csgStartPhyCellId, pciRange->csgPciRange);

      pciRange->csgStartPhyCellId = 1;
      pciRange->csgPciRange = 5;
   }
}


/* ccpu00132596 */

/*
*
*       Fun:   smBuildRmuProtMmeCfg
*
*       Desc:  This function is used for filling Mme configuration in RRM
*
*       Ret:   ROK
*
*       Notes: None
*              
*       File:  wr_sm_wr_cntrl.c 
*
*/
#ifdef ANSI 
PUBLIC S16 smBuildRmuProtMmeCfg(U16 mmeId, LrmEnbConfiguration *enbCfg, U32 action)
#else
PUBLIC S16 smBuildRmuProtMmeCfg(mmeId, enbCfg, action)
U16                 mmeId;
LrmEnbConfiguration *enbCfg;
U32                 action;
#endif
{
   TRC2(smBuildRmuProtMmeCfg);

   enbCfg->bCfgType = RRM_ENB_CFG_MME_CFG;
   enbCfg->u.stMmeCfg.enMmeAddModOrDel = action;
   enbCfg->u.stMmeCfg.usMmeId        = mmeId;

   RETVALUE(ROK);
}

#endif

/*
* 
*       Fun:   smGetEnbType
*
*       Desc:  This function is get EnbType from phyCellId, smallCellPci and smallCellPciRange 
*
*       Ret:   EnbType [WR_ENODEB_TYPE_HOME / WR_ENODEB_TYPE_MACRO]
*
*       Notes: None
*              
*       File:  wr_smm_enbapp_cmnplatutils.c
*
*/
#ifdef ANSI 
PUBLIC U8 smGetEnbType(U16 phyCellId)
#else
PUBLIC U8 smGetEnbType(phyCellId)
U16 phyCellId;
#endif
{
   U16 smallCellStartPci = lteeNodeBparams.lteAddCellCfg.startPci;
   U16 smallCellPciRange = lteeNodeBparams.lteAddCellCfg.pciRange;
   if ((phyCellId >= smallCellStartPci) && (phyCellId <= (smallCellStartPci + smallCellPciRange)))
   {
      RETVALUE(WR_ENODEB_TYPE_HOME);
   }
   else
   {
      RETVALUE(WR_ENODEB_TYPE_MACRO);
   }
}/*end of smGetEnbType*/

/*
*
*       Fun:  smInitFreePeerIdList
*
*       Desc:  This function is used to intialize free peerId list
*                          
*
*       Ret:   Void
*
*       Notes: None
*              
*       File:  fc_smm_enbapp_cmnplatutils.c 
*
*/
#ifdef ANSI 
PUBLIC Void smInitFreePeerIdList()
#else
PUBLIC Void smInitFreePeerIdList()
#endif
{
   U16 idx;

   for(idx = 0; idx < WR_SMM_MAX_PEERID_SUPPORTED; idx++)
   {
      smCfgCb.freePeeIdList.peerId[idx] = idx+1; 
   }
   smCfgCb.freePeeIdList.freePeerIdIdx = 0;
   RETVOID;
}/*end of smInitFreePeerIdList*/
/*
*
*       Fun: smGetIsX2Enabl 
*
*       Desc:  This function is used  x2enable  flag
*                          
*
*       Ret:   U8 
*
*       Notes: None
*              
*       File:  fc_smm_enbapp_cmnplatutils.c 
*
*/
#ifdef ANSI 
PUBLIC U8 smGetIsX2Enabl(LwrEnbType enbType)
#else
PUBLIC U8 smGetIsX2Enabl(LwrEnbType enbType)
#endif
{
   if ((enbType == WR_ENODEB_TYPE_MACRO) &&
       ((lteeNodeBparams.lteNeighCellCfg.isX2Enabled == WR_X2_CON_ENBL_ALL )||
        (lteeNodeBparams.lteNeighCellCfg.isX2Enabled == WR_X2_CON_ENBL_MACRO)))
   {
      RETVALUE(TRUE);
   }
   else if((enbType == WR_ENODEB_TYPE_HOME) &&
         ((lteeNodeBparams.lteNeighCellCfg.isX2Enabled == WR_X2_CON_ENBL_ALL)||
          (lteeNodeBparams.lteNeighCellCfg.isX2Enabled == WR_X2_CON_ENBL_FEMTO)))
   {
      RETVALUE(TRUE);
   }
   else
   {
      RETVALUE(FALSE);
   }
}/*end of smGetIsX2Enabl */

/*
*
*       Fun:  smGetPeerId 
*
*       Desc:  This function is used get peerId from free list 
*                          
*
*       Ret:   U8 peerId
*
*       Notes: None
*              
*       File:  fc_smm_enbapp_cmnplatutils.c 
*
*/
PUBLIC U8 smGetPeerId(U32 enbId)
{
  U8 peerId = 0;
  
  WR_GET_X2AP_PEER_ID(peerId, enbId);  
  RETVALUE(peerId);
}/*end of smGetPeerId*/
/*
*
*       Fun:  smReturnPeerIdToFreeList
*
*       Desc:  This function is to return peerId back to free list
*               once neighbor deleted 
*                          
*
*       Ret:   Void 
*
*       Notes: None
*              
*       File:  fc_smm_enbapp_cmnplatutils.c 
*
*/
#ifdef ANSI 
PUBLIC Void smReturnPeerIdToFreeList(U8 peerId)
#else
PUBLIC Void smReturnPeerIdToFreeList(U8 peerId)
#endif
{
  WR_CLEAR_X2AP_PEER_ID(peerId);

  RETVOID;
}/*end of smReturnPeerIdToFreeList*/

/*
*
*       Fun:   smFillNghUtraFreqParam
*
*       Desc:  This function is used for filling Neighbor 
*              utra Freq parameter              
*
*       Ret:   Void
*
*       Notes: None
*              
*       File:  fc_smm_enbapp_cmnplatutils.c 
*
*/
#ifdef ANSI 
PUBLIC Void smFillNghUtraFreqParam(LwrNeighUtraFreq *lwrNghUtraFreq, 
                                   MsmLteNeighUtraFreq *msmNghUtraFreq, U32 reCfgType)
#else
PUBLIC Void smFillNghUtraFreqParam(lwrNghUtraFreq,msmNghUtraFreq, reCfgType)
LwrNeighUtraFreq *lwrNghUtraFreq;
MsmLteNeighUtraFreq *msmNghUtraFreq;
U32 reCfgType;
#endif
{
   TRC2(smFillNghUtraFreqParam);

   lwrNghUtraFreq->reCfgType = reCfgType; 
   lwrNghUtraFreq->freqIndex = msmNghUtraFreq->freqIndex; 
   lwrNghUtraFreq->status    = wrSmDfltStatus; 
   lwrNghUtraFreq->t.utraFdd.arfcnDl = msmNghUtraFreq->arfcnDl;
   lwrNghUtraFreq->t.utraFdd.arfcnUl = msmNghUtraFreq->arfcnUl;

   lwrNghUtraFreq->priority.pres = PRSNT_NODEF;
   lwrNghUtraFreq->priority.val = msmNghUtraFreq->priority; 
   lwrNghUtraFreq->threshXhigh  = msmNghUtraFreq->threshXhigh; 
   lwrNghUtraFreq->threshXlow   = msmNghUtraFreq->threshXlow; 
   lwrNghUtraFreq->qRxLevMin    = msmNghUtraFreq->qRxLevMin;
   lwrNghUtraFreq->pMaxUtra     = msmNghUtraFreq->pMaxUtra;
   lwrNghUtraFreq->t.utraFdd.pQualMin     = msmNghUtraFreq->pQualMin;  
   lwrNghUtraFreq->offsetFreq   = msmNghUtraFreq->offsetFreq;
   lwrNghUtraFreq->duplexMode	 = MSM_UTRA_DUPLEX_MODE_FDD;
   lwrNghUtraFreq->t.utraFdd.thresR9.pres = PRSNT_NODEF;
   lwrNghUtraFreq->t.utraFdd.thresR9.thresXHighQR9 = msmNghUtraFreq->threshXHighQR9;
   lwrNghUtraFreq->t.utraFdd.thresR9.thresXLowQR9 = msmNghUtraFreq->threshXLowQR9;

}/*end of smFillNghUtraFreqParam*/

/*
*
*       Fun:   smFillNghUtraTddFreqParam
*
*       Desc:  This function is used for filling Neighbor 
*              utra Tdd Freq parameter              
*
*       Ret:   Void
*
*       Notes: None
*              
*       File:  wr_smm_enbapp_cmnplatutils.c 
*
*/
#ifdef ANSI 
PUBLIC Void smFillNghUtraTddFreqParam(LwrNeighUtraFreq *lwrNghUtraFreq, 
                                   MsmLteNeighUtraTddFreq *msmNghUtraFreq, U32 reCfgType)
#else
PUBLIC Void smFillNghUtraTddFreqParam(lwrNghUtraFreq,msmNghUtraFreq, reCfgType)
LwrNeighUtraFreq *lwrNghUtraFreq;
MsmLteNeighUtraTddFreq *msmNghUtraFreq;
U32 reCfgType;
#endif
{
   TRC2(smFillNghUtraTddFreqParam);

   lwrNghUtraFreq->reCfgType = reCfgType; 
   lwrNghUtraFreq->freqIndex = msmNghUtraFreq->freqIndex; 
   lwrNghUtraFreq->status    = wrSmDfltStatus; 
   lwrNghUtraFreq->t.utraTdd.arfcn = msmNghUtraFreq->arfcn;
   lwrNghUtraFreq->priority.pres = PRSNT_NODEF;
   lwrNghUtraFreq->priority.val = msmNghUtraFreq->priority; 
   lwrNghUtraFreq->threshXhigh  = msmNghUtraFreq->threshXhigh; 
   lwrNghUtraFreq->threshXlow   = msmNghUtraFreq->threshXlow; 
   lwrNghUtraFreq->qRxLevMin    = msmNghUtraFreq->qRxLevMin;
   lwrNghUtraFreq->pMaxUtra     = msmNghUtraFreq->pMaxUtra;
   lwrNghUtraFreq->offsetFreq   = msmNghUtraFreq->offsetFreq;
   lwrNghUtraFreq->duplexMode	 = MSM_UTRA_DUPLEX_MODE_TDD;
   lwrNghUtraFreq->t.utraTdd.tddMode = msmNghUtraFreq->tddMode;
   lwrNghUtraFreq->t.utraTdd.bandIndicator = msmNghUtraFreq->bandIndicator;
}/*end of smFillNghUtraTddFreqParam*/

/*
*
*       Fun:   smFillNghEutraFreqParam
*
*       Desc:  This function is used for filling Neighbor 
*              eutra freq parameter              
*
*       Ret:   Void
*
*       Notes: None
*              
*       File:  fc_smm_enbapp_cmnplatutils.c 
*
*/
#ifdef ANSI 
PUBLIC Void smFillNghEutraFreqParam(LwrNeighEutraFreq *lwrNghEutraFreq, 
                                   MsmLteNeighEutraFreq *msmNghEutraFreq,
                                   U32 reCfgType)
#else
PUBLIC Void smFillNghEutraFreqParam(lwrNghEutraFreq, msmNghEutraFreq, reCfgType)
LwrNeighEutraFreq *lwrNghEutraFreq;
MsmLteNeighEutraFreq *msmNghEutraFreq;
U32 reCfgType;
#endif
{
   TRC2(smFillNghEutraFreqParam);
   lwrNghEutraFreq->reCfgType    = reCfgType;
   lwrNghEutraFreq->freqIndex    = msmNghEutraFreq->freqIndex;
   lwrNghEutraFreq->status       = wrSmDfltStatus;
   lwrNghEutraFreq->earfcnUl     = msmNghEutraFreq->earfcnUl;
   lwrNghEutraFreq->earfcnDl     = msmNghEutraFreq->earfcnDl;
   lwrNghEutraFreq->priority     = msmNghEutraFreq->priority;
   lwrNghEutraFreq->qRxLevMin    = msmNghEutraFreq->qRxLevMin;
   lwrNghEutraFreq->pMaxUsage    = wrSmDfltPMaxUsage;
   lwrNghEutraFreq->pMax         = msmNghEutraFreq->pMax;
   lwrNghEutraFreq->tReselection = msmNghEutraFreq->tReselection;
   lwrNghEutraFreq->sfUsage      = wrSmDfltSfUsage;
   lwrNghEutraFreq->tReselectionSFMed  = msmNghEutraFreq->tReselectionSFMed;
   lwrNghEutraFreq->tReselectionSFHigh = msmNghEutraFreq->tReselectionSFHigh;
   lwrNghEutraFreq->sIntraSearch.pres  = PRSNT_NODEF;
   lwrNghEutraFreq->sIntraSearch.val   = msmNghEutraFreq->sIntraSearch;
   lwrNghEutraFreq->sNonIntraSearch.pres = PRSNT_NODEF;
   lwrNghEutraFreq->sNonIntraSearch.val  = msmNghEutraFreq->sNonIntraSearch;
   lwrNghEutraFreq->servThresholdLow     = msmNghEutraFreq->servThresholdLow;
   lwrNghEutraFreq->threshXHigh  = msmNghEutraFreq->threshXHigh;
   lwrNghEutraFreq->threshXLow   = msmNghEutraFreq->threshXLow;
   lwrNghEutraFreq->measBandwidth.pres =  PRSNT_NODEF;
   lwrNghEutraFreq->measBandwidth.val  = msmNghEutraFreq->measBandwidth;
   lwrNghEutraFreq->presAntennaPort1   = msmNghEutraFreq->presAntennaPort1;
   lwrNghEutraFreq->neighCellCfg       = msmNghEutraFreq->neighCellCfg;
   lwrNghEutraFreq->qOffsetFreq        = msmNghEutraFreq->qOffsetFreq;
   lwrNghEutraFreq->offsetFreq         = msmNghEutraFreq->offsetFreq;
   lwrNghEutraFreq->sIntraSearchRel9.pres =  PRSNT_NODEF;
   lwrNghEutraFreq->sIntraSearchRel9.sSearchPRel9 = wrSmDfltsSearchPRel9;
   lwrNghEutraFreq->sIntraSearchRel9.sSearchQRel9 =  wrSmDfltsSearchQRel9;
   lwrNghEutraFreq->sInterSearchRel9.pres         = PRSNT_NODEF;
   lwrNghEutraFreq->sInterSearchRel9.sSearchPRel9 = wrSmDfltsSearchPRel9;
   lwrNghEutraFreq->sInterSearchRel9.sSearchQRel9 = wrSmDfltsSearchQRel9;
   lwrNghEutraFreq->qQualMinRel9.pres = PRSNT_NODEF;
   lwrNghEutraFreq->qQualMinRel9.val  = wrSmDfltDualMinRel9;
   lwrNghEutraFreq->reselecThreshQRel9.pres = PRSNT_NODEF;
   lwrNghEutraFreq->reselecThreshQRel9.val  =  wrSmDfltReselecThreshQRel9;
   lwrNghEutraFreq->threshXQRel9.threshXHighQR9 =  msmNghEutraFreq->threshXHighQR9;
   lwrNghEutraFreq->threshXQRel9.threshXLowQR9  =  msmNghEutraFreq->threshXLowQR9;

}/*end of smFillNghEutraFreqParam*/
 
/*
*
*       Fun:   smFillNghGeranFreqParam
*
*       Desc:  This function is used for filling Neighbor 
*              Geran freq parameter              
*
*       Ret:   Void
*
*       Notes: None
*              
*       File:  fc_smm_enbapp_cmnplatutils.c 
*
*/
#ifdef ANSI 
PUBLIC Void smFillNghGeranFreqParam(LwrNeighGERANFreq *lwrNghGeranFreq, 
                                   MsmLteNeighGERANFreq *msmNghGeranFreq,
                                   U32 reCfgType, U32 idx)
#else
PUBLIC Void smFillNghGeranFreqParam(lwrNghGERANFreq, msmNghGeranFreq, reCfgType, idx)
LwrNeighGERANFreq *lwrNghGeranFreq;
MsmLteNeighGERANFreq *msmNghGeranFreq;
U32 reCfgType;
U32 idx;
#endif
{
   TRC2(smFillNghGeranFreqParam);

   lwrNghGeranFreq->reCfgType          = reCfgType;
   lwrNghGeranFreq->freqIndex          = idx + 1;
   lwrNghGeranFreq->status             = wrSmDfltStatus;
   lwrNghGeranFreq->bandIndicator      = msmNghGeranFreq->bandIndicator;
   lwrNghGeranFreq->bcchArfcn          = msmNghGeranFreq->bcchArfcn;
   lwrNghGeranFreq->qRxLevMin          = msmNghGeranFreq->qRxLevMin;
   lwrNghGeranFreq->cellReselePri = msmNghGeranFreq->cellReselectionPriority;
   lwrNghGeranFreq->threshXHigh        = msmNghGeranFreq->threshXHigh;
   lwrNghGeranFreq->threshXLow         = msmNghGeranFreq->threshXLow;
   lwrNghGeranFreq->ncc_Permitted      = msmNghGeranFreq->ncc_Permitted;
   lwrNghGeranFreq->pMaxGeran.pres     = msmNghGeranFreq->pMaxPres;
   lwrNghGeranFreq->pMaxGeran.val      = msmNghGeranFreq->pMax;

   RETVOID;
}/*end of smFillNghGeranFreqParam*/
 
/*
 *
 *       Fun: smBuildMobParams - Fill Mobility parameters
 *
 *       Desc:   
 *
 *       Ret:    Void
 *
 *       Notes:  None
 *
 *      File:    wr_smm_enbapp_cmnplatutils.c
 *
 */

PRIVATE S16 smBuildMobParams
(
  LwrCdma2k1xMobParams *mobParams
)
{
   MsmCdmaMobilityParam *mobParam;

   mobParam = &(lteeNodeBparams.cdmaMobilityParam);

   mobParams->sidIncl = mobParam->sidIncl; 
   mobParams->sid = mobParam->sid;
   mobParams->nidIncl = mobParam->nidIncl;
   mobParams->nid = mobParam->nid;
   mobParams->multSidsIncl = mobParam->multSidsIncl;
   mobParams->multSids = mobParam->multSids;
   mobParams->multNidsIncl = mobParam->multNidsIncl;
   mobParams->multNids = mobParam->multNids;
   mobParams->regZoneIncl = mobParam->regZoneIncl;
   mobParams->regZone = mobParam->regZone;
   mobParams->totalZonesIncl = mobParam->totalZonesIncl;
   mobParams->totalZones = mobParam->totalZones;
   mobParams->zoneTimerIncl = mobParam->zoneTimerIncl;
   mobParams->zoneTimer = mobParam->zoneTimer;
   mobParams->pktZoneIdIncl = mobParam->pktZoneIdIncl;
   mobParams->pktZoneId = mobParam->pktZoneId;
   mobParams->pzIdHysParamIncl = mobParam->pzIdHysParamIncl;
   mobParams->pzHystEnabled = mobParam->pzHystEnabled;
   mobParams->pzHystInfoIncl = mobParam->pzHystInfoIncl;
   mobParams->pzHystListLen = mobParam->pzHystListLen;
   mobParams->pzHystActTimer = mobParam->pzHystActTimer;
   mobParams->pzHystTimerMul = mobParam->pzHystTimerMul;
   mobParams->pzHystTimerExp = mobParam->pzHystTimerExp;
   mobParams->pRevIncl = mobParam->pRevIncl;
   mobParams->pRev = mobParam->pRev;
   mobParams->minPRevIncl = mobParam->minPRevIncl;
   mobParams->minPRev = mobParam->minPRev;
   mobParams->negSlotCycIndxSupIncl = mobParam->negSlotCycIndxSupIncl;
   mobParams->negSlotCycIndxSup = mobParam->negSlotCycIndxSup;
   mobParams->encModeIncl = mobParam->encModeIncl;
   mobParams->encMode = mobParam->encMode;
   mobParams->encSupIncl = mobParam->encSupIncl;
   mobParams->encSup = mobParam->encSup;
   mobParams->sigEncSupIncl = mobParam->sigEncSupIncl;
   mobParams->sigEncSup = mobParam->sigEncSup;
   mobParams->msgIntSupIncl = mobParam->msgIntSupIncl;
   mobParams->msgIntSup = mobParam->msgIntSup;
   mobParams->sigIntSupINCLIncl = mobParam->sigIntSupINCLIncl;
   mobParams->sigIntSupINCL = mobParam->sigIntSupINCL;
   mobParams->sigIntSupIncl = mobParam->sigIntSupIncl;
   mobParams->sigIntSup = mobParam->sigIntSup;
   mobParams->authIncl = mobParam->authIncl;
   mobParams->auth = mobParam->auth;
   mobParams->maxNumAltoSoIncl = mobParam->maxNumAltoSoIncl;
   mobParams->maxNumAltoSo = mobParam->maxNumAltoSo;
   mobParams->useSyncIdIncl = mobParam->useSyncIdIncl;
   mobParams->useSyncId = mobParam->useSyncId;
   mobParams->msInitPosLocSupIndIncl = mobParam->msInitPosLocSupIndIncl;
   mobParams->msInitPosLocSupInd = mobParam->msInitPosLocSupInd;
   mobParams->mobQosIncl = mobParam->mobQosIncl;
   mobParams->mobQos = mobParam->mobQos;
   mobParams->bandClsInfoReqIncl = mobParam->bandClsInfoReqIncl;
   mobParams->bandClsInfoReq = mobParam->bandClsInfoReq;
   mobParams->bandClsIncl = mobParam->bandClsIncl;
   mobParams->bandCls = mobParam->bandCls;
   mobParams->bypassRegIndIncl = mobParam->bypassRegIndIncl;
   mobParams->bypassRegInd = mobParam->bypassRegInd;
   mobParams->altBandClsIncl = mobParam->altBandClsIncl;
   mobParams->altBandCls = mobParam->altBandCls;
   mobParams->maxAddServInstIncl = mobParam->maxAddServInstIncl;
   mobParams->maxAddServInst = mobParam->maxAddServInst;
   mobParams->homeRegIncl = mobParam->homeRegIncl;
   mobParams->homeReg = mobParam->homeReg;
   mobParams->forSidRegIncl = mobParam->forSidRegIncl;
   mobParams->forSidReg = mobParam->forSidReg;
   mobParams->forNidRegIncl = mobParam->forNidRegIncl;
   mobParams->forNidReg = mobParam->forNidReg;
   mobParams->powerUpRegIncl = mobParam->powerUpRegIncl;
   mobParams->powerUpReg = mobParam->powerUpReg;
   mobParams->powerDownRegIncl = mobParam->powerDownRegIncl;
   mobParams->powerDownReg = mobParam->powerDownReg;
   mobParams->parmRegIncl = mobParam->parmRegIncl;
   mobParams->parmReg = mobParam->parmReg;
   mobParams->regPrdIncl = mobParam->regPrdIncl;
   mobParams->regPrd = mobParam->regPrd;
   mobParams->regDistIncl = mobParam->regDistIncl;
   mobParams->regDist = mobParam->regDist;
   mobParams->prefMsIdTypeIncl = mobParam->prefMsIdTypeIncl;
   mobParams->prefMsIdType = mobParam->prefMsIdType;
   mobParams->extPrefMsIdTypeIncl = mobParam->extPrefMsIdTypeIncl;
   mobParams->extPrefMsIdType = mobParam->extPrefMsIdType;
   mobParams->meIdReqdIncl = mobParam->meIdReqdIncl;
   mobParams->meIdReqd = mobParam->meIdReqd;
   mobParams->mccIncl = mobParam->mccIncl;
   mobParams->mcc = mobParam->mcc;
   mobParams->imsi1112Incl = mobParam->imsi1112Incl;
   mobParams->imsi1112 = mobParam->imsi1112;
   mobParams->imsiTSupIncl = mobParam->imsiTSupIncl;
   mobParams->imsiTSup = mobParam->imsiTSup;
   mobParams->recntMsgIndIncl = mobParam->recntMsgIndIncl;
   mobParams->recntMsgInd = mobParam->recntMsgInd;
   mobParams->rerModeSupIncl = mobParam->rerModeSupIncl;
   mobParams->rerModeSup = mobParam->rerModeSup;
   mobParams->tzkModeSupIncl = mobParam->tzkModeSupIncl;
   mobParams->tzkModeSup = mobParam->tzkModeSup;
   mobParams->tzkIdIncl = mobParam->tzkIdIncl;
   mobParams->tzkId = mobParam->tzkId;
   mobParams->pilRepIncl = mobParam->pilRepIncl;
   mobParams->pilRep = mobParam->pilRep;
   mobParams->sdbSupIncl = mobParam->sdbSupIncl;
   mobParams->sdbSup = mobParam->sdbSup;
   mobParams->autoFcsoAllIncl = mobParam->autoFcsoAllIncl;
   mobParams->autoFcsoAll = mobParam->autoFcsoAll;
   mobParams->sdbInRcnmIncl = mobParam->sdbInRcnmIncl;
   mobParams->sdbInRcnm = mobParam->sdbInRcnm;
   mobParams->fpcFchIncl = mobParam->fpcFchIncl;
   mobParams->fpcFchInitSetptRC3 = mobParam->fpcFchInitSetptRC3;
   mobParams->fpcFchInitSetptRC4 = mobParam->fpcFchInitSetptRC4;
   mobParams->fpcFchInitSetptRC5 = mobParam->fpcFchInitSetptRC5;
   mobParams->fpcFchInitSetptRC11 = mobParam->fpcFchInitSetptRC11;
   mobParams->fpcFchInitSetptRC12 = mobParam->fpcFchInitSetptRC12;
   mobParams->tAddIncl = mobParam->tAddIncl;
   mobParams->tAdd = mobParam->tAdd;
   mobParams->pilotIncIncl = mobParam->pilotIncIncl;
   mobParams->PilotInc = mobParam->pilotInc;
   mobParams->randIncl = mobParam->randIncl;
   mobParams->rand = mobParam->rand;
   mobParams->lpSecIncl = mobParam->lpSecIncl;
   mobParams->lpSec = mobParam->lpSec;
   mobParams->ltmOffIncl = mobParam->ltmOffIncl;
   mobParams->ltmOff = mobParam->ltmOff;
   mobParams->dayltIncl = mobParam->dayltIncl;
   mobParams->daylt = mobParam->daylt;
   mobParams->gcsnaL2ActTimerIncl = mobParam->gcsnaL2ActTimerIncl;
   mobParams->gcsnaL2ActTimer = mobParam->gcsnaL2ActTimer;
   mobParams->gcsnaSeqCtxtTimerIncl = mobParam->gcsnaSeqCtxtTimerIncl;
   mobParams->gcsnaSeqCtxtTimer = mobParam->gcsnaSeqCtxtTimer;
   //mobParams->reserved = wrSmDfltCdma1xReservedreserved;
   RETVALUE(ROK);
} /* end of smBuildMobParams */ 
 
/*
*
*       Fun:   smFillNghCdma1xBandParam
*
*       Desc:  This function for filling the Cell Band Class Configuration
*
*       Ret:   ROK
*
*       Notes: None
*              
*       File:  wr_sm_wr_cntrl.c 
*
*/
#ifdef ANSI
PUBLIC S16 smFillNghCdma1xBandParam
(
 LwrNeighCdmaBandClassCfg *bandClassCfg,
 MsmCdmaBandClassCfg      *bandClass,
 U32                      reCfgType,
 U8                       idx
)
#else /* if ANSI not defined*/
PUBLIC S16 smFillNghCdma1xBandParam(bandClassCfg, bandClass, reCfgType, idx)
(
 LwrNeighCdmaBandClassCfg *bandClassCfg;
 MsmCdmaBandClassCfg      *bandClass;
 U32                      reCfgType;
 U8                       idx;
)
#endif /* end of ANSI*/
{
   bandClassCfg->reCfgType = reCfgType;
   bandClassCfg->bandClassIndex  = idx+1;
   bandClassCfg->status = wrSmDfltBandClsStatus;
   bandClassCfg->bandClass = bandClass->bandClass;
   bandClassCfg->cellReselectPrior = bandClass->cellReselectPrior;
   bandClassCfg->threshXHigh = bandClass->threshXHigh;
   bandClassCfg->threshXLow = bandClass->threshXLow;
   RETVALUE(ROK);
} /* end of smFillNghCdma1xBandParam */
 
/*
*
*       Fun:   smFillNghCdma1xCellParam
*
*       Desc:  This function is used for filling Neighbor 
*              cdma 1xrtt cell parameter              
*
*       Ret:   Void
*
*       Notes: None
*              
*       File:  fc_smm_enbapp_cmnplatutils.c 
*
*/
#ifdef ANSI 
PUBLIC S16 smFillNghCdma1xCellParam(LwrCDMA1xNeighCell *lwrNghCdma1xCell, 
                                   MsmCDMA1xNeighCell *msmNghCdma1xCell,
                                   LwrNeighCdma1xFreq *lwrNghCdma1xFreq,
                                   U32 reCfgType, U16 cnt)
#else
PUBLIC S16 smFillNghCdma1xCellParam(lwrNghCdma1xCell, msmNghCdma1xCell,
      reCfgType)
LwrCDMA1xNeighCell *lwrNghCdma1xCell;
MsmCDMA1xNeighCell *msmNghCdma1xCell;
LwrNeighCdma1xFreq *lwrNghCdma1xFreq;
U32 reCfgType;
U16 cnt;
#endif
{
   U8  ret     = RFAILED;
   U8  idx     = 0;
   U64 tmpCid  = 0;
   U16 sId     = 0;
   U16 nId     = 0;
   U16 baseId  = 0;
   U8  *tmp = NULLP;

   TRC2(smFillNghCdma1xCellParam);

   for(idx = 0; idx<lteeNodeBparams.cdmaBandClassParams.numBandClass; idx++)
   {
      if(lteeNodeBparams.cdmaBandClassParams.t.bandClassAddCfg[idx].t.
            bandClass1XRtt.bandClass == msmNghCdma1xCell->bandClass)
      {
         WR_ALLOC(&tmp,6);
         if(NULLP == tmp)
         {
            RLOG0(L_FATAL, "Memory allocation failed while filling Neighbour "
               "CDMA1X cell");
            RETVALUE(RFAILED);
         }
         cmMemcpy((U8*)tmp, (U8*)msmNghCdma1xCell->CID, 6);
         tmpCid = (U64)atoi((S8*)tmp);
         tmpCid = tmpCid & 0x00007fffffffffff;
         wrGetCdmaParamFromCID(&sId, &nId, &baseId, CDMA2000_1XRTT, tmpCid);

         lwrNghCdma1xCell->cellNum       = 1;
         lwrNghCdma1xCell->nrIndex       = cnt;
         lwrNghCdma1xCell->status        = wrSmDfltStatus;
         lwrNghCdma1xCell->sId           = sId;
         lwrNghCdma1xCell->nId           = nId;
         lwrNghCdma1xCell->baseId        = baseId;
         lwrNghCdma1xCell->bandClass     = msmNghCdma1xCell->bandClass;
         lwrNghCdma1xCell->arfcn         = msmNghCdma1xCell->arfcn;
         lwrNghCdma1xCell->pnOffset      = msmNghCdma1xCell->pnOffset;
         lwrNghCdma1xCell->curRank       = msmNghCdma1xCell->curRank;
         lwrNghCdma1xCell->systemType    = msmNghCdma1xCell->systemType;
         lwrNghCdma1xCell->channelNum    = msmNghCdma1xCell->channelNum;       
         lwrNghCdma1xCell->secId.cdmaType= msmNghCdma1xCell->cellType;
         lwrNghCdma1xCell->secId.isOctStrPres = NOTPRSNT;  // OAM does not send in Octet form
         lwrNghCdma1xCell->secId.s.cdmaSectorId.marketId= msmNghCdma1xCell->tgtCell.marketId;
         lwrNghCdma1xCell->secId.s.cdmaSectorId.switchNum= msmNghCdma1xCell->tgtCell.switchNum;
         lwrNghCdma1xCell->secId.s.cdmaSectorId.sectorNum= msmNghCdma1xCell->tgtCell.sectorNum;
         lwrNghCdma1xCell->secId.s.cdmaSectorId.cellId= msmNghCdma1xCell->tgtCell.cellId;       
         lwrNghCdma1xCell->secId.cdmaType = msmNghCdma1xCell->cellType;
         lwrNghCdma1xCell->neighCellFlag = FALSE;
         smBuildMobParams(&lwrNghCdma1xCell->mobParams);
         lwrNghCdma1xCell->mobParams.sid = sId;
         lwrNghCdma1xCell->mobParams.nid = nId;

         smFillNghCdma1xFreqParam(lwrNghCdma1xFreq, 
               lwrNghCdma1xCell, wrSmDfltReCfgType,idx);
         ret = ROK;
      }
   }
   RETVALUE(ret);
}/*end of smFillNghCdma1xCellParam*/
 
/*
*
*       Fun:   smFillNghCdma1xFreqParam
*
*       Desc:  This function is used for filling Neighbor 
*              cdma 1xrtt freq parameter              
*
*       Ret:   Void
*
*       Notes: None
*              
*       File:  fc_smm_enbapp_cmnplatutils.c 
*
*/
#ifdef ANSI 
PUBLIC Void smFillNghCdma1xFreqParam(LwrNeighCdma1xFreq *lwrNghCdma1xFreq, 
                                   LwrCDMA1xNeighCell *nghCdma1xCell,
                                   U32 reCfgType, U8 idx)
#else
PUBLIC Void smFillNghCdma1xFreqParam(lwrNghCdma1xFreq, nghCdma1xCell, reCfgType, idx)
LwrNeighCdma1xFreq *lwrNghCdma1xFreq;
LwrCDMA1xNeighCell *nghCdma1xCell;
U32 reCfgType;
U8 idx;
#endif
{
   TRC2(smFillNghCdma1xFreqParam);

   lwrNghCdma1xFreq->reCfgType    = reCfgType;
   lwrNghCdma1xFreq->freqIndex    = idx + 1;
   lwrNghCdma1xFreq->status       = wrSmDfltStatus;
   lwrNghCdma1xFreq->bandClass    = nghCdma1xCell->bandClass;
   lwrNghCdma1xFreq->arfcn        = nghCdma1xCell->arfcn;
   lwrNghCdma1xFreq->searchRate   = lteeNodeBparams.csfbCfgGrp.searchRate;
   lwrNghCdma1xFreq->offsetFreq   = wrSmDfltCdma1xOffsetFreq;
   lwrNghCdma1xFreq->freqPriority = 1;

}/*end of smFillNghCdma1xFreqParam*/
 
/*
*
*       Fun:   smFillNghUtraCellParam
*
*       Desc:  This function is used for filling Neighbor 
*              utra cell parameter              
*
*       Ret:   Void
*
*       Notes: None
*              
*       File:  fc_smm_enbapp_cmnplatutils.c 
*
*/
#ifdef ANSI 
PUBLIC Void smFillNghUtraCellParam(LwrUtraNeighCell *lwrNghUtraCell, 
                                   MsmUtraNeighCell *msmNghUtraCell,
                                   U32 reCfgType)
#else
PUBLIC Void smFillNghUtraCellParam(lwrNghEutraCell,msmNghUtraCell, reCfgType)
LwrUtraNeighCell *lwrNghUtraCell;
MsmUtraNeighCell *msmNghUtraCell;
U32 reCfgType;
#endif
{
   U32 plmnIdx = 0;
   TRC2(smFillNghUtraCellParam);
 
   lwrNghUtraCell->reCfgType = reCfgType;
   lwrNghUtraCell->nrIndex = msmNghUtraCell->nrIndex;
   lwrNghUtraCell->rncId   = msmNghUtraCell->rncId;
   lwrNghUtraCell->cId     = msmNghUtraCell->cId;

   lwrNghUtraCell->numPlmn = msmNghUtraCell->numPlmn;
   for(plmnIdx = 0; plmnIdx < lwrNghUtraCell->numPlmn; plmnIdx++)
   {
     lwrNghUtraCell->plmnId[plmnIdx].mcc[0] = msmNghUtraCell->plmnId[plmnIdx].mcc[0];
     lwrNghUtraCell->plmnId[plmnIdx].mcc[1] = msmNghUtraCell->plmnId[plmnIdx].mcc[1];
     lwrNghUtraCell->plmnId[plmnIdx].mcc[2] = msmNghUtraCell->plmnId[plmnIdx].mcc[2];

     lwrNghUtraCell->plmnId[plmnIdx].numMncDigits = 
                              msmNghUtraCell->plmnId[plmnIdx].numMncDigits;

     lwrNghUtraCell->plmnId[plmnIdx].mnc[0] = msmNghUtraCell->plmnId[plmnIdx].mnc[0];
     lwrNghUtraCell->plmnId[plmnIdx].mnc[1] = msmNghUtraCell->plmnId[plmnIdx].mnc[1];

     if(3 == lwrNghUtraCell->plmnId[plmnIdx].numMncDigits)
        lwrNghUtraCell->plmnId[plmnIdx].mnc[2] = msmNghUtraCell->plmnId[plmnIdx].mnc[2];
   } 
   lwrNghUtraCell->t.utraFdd.psc = msmNghUtraCell->psc;
   lwrNghUtraCell->lac = msmNghUtraCell->lac;
   lwrNghUtraCell->rac = msmNghUtraCell->rac;

   lwrNghUtraCell->duplexMode = MSM_UTRA_DUPLEX_MODE_FDD;

   lwrNghUtraCell->t.utraFdd.arfcnDl = msmNghUtraCell->arfcnDl;
   lwrNghUtraCell->t.utraFdd.arfcnUl = msmNghUtraCell->arfcnUl;
   lwrNghUtraCell->isRimSupported = msmNghUtraCell->isRimSupported;
   lwrNghUtraCell->isVoipCapable = msmNghUtraCell->isVoipCapable;
   lwrNghUtraCell->isPsHOCapable = msmNghUtraCell->isPsHOCapable;

}/*end of smFillNghUtraCellParam*/

/*
*
*       Fun:   smFillNghUtraTddCellParam
*
*       Desc:  This function is used for filling Neighbor 
*              utra tdd cell parameter              
*
*       Ret:   Void
*
*       Notes: None
*              
*       File:  fc_smm_enbapp_cmnplatutils.c 
*
*/
#ifdef ANSI 
PUBLIC Void smFillNghUtraTddCellParam(LwrUtraNeighCell *lwrNghUtraTddCell, 
                                   MsmUtraTddNeighCell *msmNghUtraTddCell,
                                   U32 reCfgType)
#else
PUBLIC Void smFillNghUtraTddCellParam(lwrNghUtraTddCell,msmNghUtraTddCell, reCfgType)
LwrUtraNeighCell *lwrNghUtraTddCell;
MsmUtraTddNeighCell *msmNghUtraTddCell;
U32 reCfgType;
#endif
{
   U32 plmnIdx = 0;

   TRC2(smFillNghUtraTddCellParam);
 
   lwrNghUtraTddCell->reCfgType = reCfgType;
   lwrNghUtraTddCell->nrIndex = msmNghUtraTddCell->nrIndex;
   lwrNghUtraTddCell->rncId   = msmNghUtraTddCell->rncId;
   lwrNghUtraTddCell->cId     = msmNghUtraTddCell->cId;

   lwrNghUtraTddCell->numPlmn = msmNghUtraTddCell->numPlmn;
   for(plmnIdx = 0; plmnIdx < lwrNghUtraTddCell->numPlmn; plmnIdx++)
   {
     lwrNghUtraTddCell->plmnId[plmnIdx].mcc[0] = msmNghUtraTddCell->plmnId[plmnIdx].mcc[0];
     lwrNghUtraTddCell->plmnId[plmnIdx].mcc[1] = msmNghUtraTddCell->plmnId[plmnIdx].mcc[1];
     lwrNghUtraTddCell->plmnId[plmnIdx].mcc[2] = msmNghUtraTddCell->plmnId[plmnIdx].mcc[2];

     lwrNghUtraTddCell->plmnId[plmnIdx].numMncDigits = 
                              msmNghUtraTddCell->plmnId[plmnIdx].numMncDigits;

     lwrNghUtraTddCell->plmnId[plmnIdx].mnc[0] = msmNghUtraTddCell->plmnId[plmnIdx].mnc[0];
     lwrNghUtraTddCell->plmnId[plmnIdx].mnc[1] = msmNghUtraTddCell->plmnId[plmnIdx].mnc[1];

     if(3 == lwrNghUtraTddCell->plmnId[plmnIdx].numMncDigits)
        lwrNghUtraTddCell->plmnId[plmnIdx].mnc[2] = msmNghUtraTddCell->plmnId[plmnIdx].mnc[2];
   } 
   lwrNghUtraTddCell->lac = msmNghUtraTddCell->lac;
   lwrNghUtraTddCell->rac = msmNghUtraTddCell->rac;

   lwrNghUtraTddCell->t.utraTdd.cellParamId = msmNghUtraTddCell->cellParamId;
   lwrNghUtraTddCell->duplexMode = MSM_UTRA_DUPLEX_MODE_TDD;

   lwrNghUtraTddCell->t.utraTdd.arfcn = msmNghUtraTddCell->arfcn;
   lwrNghUtraTddCell->isRimSupported = msmNghUtraTddCell->isRimSupported;
}/*end of smFillNghUtraTddCellParam*/

/*
*
*       Fun:  smFillNghEnbParam 
*
*       Desc:  This function is used for filling Neighbor 
*              Eutra cell parameter              
*
*       Ret:   Void
*
*       Notes: None
*              
*       File:  fc_smm_enbapp_cmnplatutils.c 
*
*/
#ifdef ANSI 
PUBLIC Void smFillNghEnbParam(SmNeigEnbList *nghEnbCfg, 
                              MsmEUTRANeighCell *msmNghEutraCell)
#else
PUBLIC Void smFillNghEnbParam(nghEnbCfg, msmNghEutraCell)
SmNeigEnbList *nghEnbCfg;
MsmEUTRANeighCell *msmNghEutraCell;
#endif
{
   TRC2(smFillNghEnbParam);

   nghEnbCfg->numCell++;
   nghEnbCfg->nghEnbCfg.enbType = smGetEnbType(msmNghEutraCell->pci); 

/*ccpu00138869 start*/
   if(nghEnbCfg->nghEnbCfg.enbType == WR_ENODEB_TYPE_MACRO)
   {
      nghEnbCfg->nghEnbCfg.enbId   = ((msmNghEutraCell->cid) >>8); 
   }
   else
   {
      nghEnbCfg->nghEnbCfg.enbId   = msmNghEutraCell->cid; 
   }
   RLOG2(L_DEBUG,"Filling Neighbour ENB parameters, ENB type : %lu,enbId : %lu",
     nghEnbCfg->nghEnbCfg.enbType,nghEnbCfg->nghEnbCfg.enbId);

   nghEnbCfg->nghEnbCfg.peerId  = smGetPeerId(nghEnbCfg->nghEnbCfg.enbId); 
   nghEnbCfg->nghEnbCfg.plmn.numMncDigits = msmNghEutraCell->plmnId[0].numMncDigits; 
   nghEnbCfg->nghEnbCfg.plmn.mcc[0] = msmNghEutraCell->plmnId[0].mcc[0]; 
   nghEnbCfg->nghEnbCfg.plmn.mcc[1] = msmNghEutraCell->plmnId[0].mcc[1];
   nghEnbCfg->nghEnbCfg.plmn.mcc[2] = msmNghEutraCell->plmnId[0].mcc[2];
   nghEnbCfg->nghEnbCfg.plmn.mnc[0] = msmNghEutraCell->plmnId[0].mnc[0]; 
   nghEnbCfg->nghEnbCfg.plmn.mnc[1] = msmNghEutraCell->plmnId[0].mnc[1]; 
   if(msmNghEutraCell->plmnId[0].numMncDigits == 3)
   {
      nghEnbCfg->nghEnbCfg.plmn.mnc[2] = msmNghEutraCell->plmnId[0].mnc[2]; 
   }
   else
   {
      nghEnbCfg->nghEnbCfg.plmn.mnc[2] = 0; 
   }
   nghEnbCfg->nghEnbCfg.ipAddr.u.ipv4TptAddr.port       = 0; 
   nghEnbCfg->nghEnbCfg.ipAddr.u.ipv4TptAddr.address    = 0; 
   nghEnbCfg->nghEnbCfg.secIpAddr.u.ipv4TptAddr.port    = 0;
   nghEnbCfg->nghEnbCfg.secIpAddr.u.ipv4TptAddr.address = 0;
   nghEnbCfg->nghEnbCfg.state     = wrSmDfltNeEnbState; 

   nghEnbCfg->nghEnbCfg.isX2Reqd  = smGetIsX2Enabl(nghEnbCfg->nghEnbCfg.enbType);
   nghEnbCfg->nghEnbCfg.alarmVal.maxErrIndCnt = faultMgmtParams.frequentFaultMgmtParameters.maxX2ErrIndCnt;
   nghEnbCfg->nghEnbCfg.alarmVal.minErrIndTime = faultMgmtParams.frequentFaultMgmtParameters.minX2ErrIndTime;
   nghEnbCfg->nghEnbCfg.alarmVal.maxResetCnt = faultMgmtParams.frequentFaultMgmtParameters.maxX2ResetCnt;
   nghEnbCfg->nghEnbCfg.alarmVal.minResetTime = faultMgmtParams.frequentFaultMgmtParameters.minX2ResetTime;
}/*end of smFillNghEnbParam*/

/*
*
*       Fun:   smFillNghEutraCellPara
*
*       Desc:  This function is used for filling Neighbor 
*              Eutra cell parameter              
*
*       Ret:   Void
*
*       Notes: None
*              
*       File:  fc_smm_enbapp_cmnplatutils.c 
*
*/
#ifdef ANSI 
PUBLIC Void smFillNghEutraCellParam(LwrEUTRANeighCell *lwrNghEutraCell, 
                                   MsmEUTRANeighCell *msmNghEutraCell,
                                   U32 reCfgType)
#else
PUBLIC Void smFillNghEutraCellParam(lwrNghEutraCell,msmNghEutraCell, reCfgType)
LwrEUTRANeighCell *lwrNghEutraCell;
MsmEUTRANeighCell *msmNghEutraCell;
U32                reCfgType;
#endif
{
   TRC2(smFillNghEutraCellParam);
   lwrNghEutraCell->reCfgType = reCfgType;
   lwrNghEutraCell->nrIndex = msmNghEutraCell->nrIndex; 
   lwrNghEutraCell->enbType = smGetEnbType(msmNghEutraCell->pci);
   /*Do not do right shift by 8 for MACRO as it is done in EMM module*/
   lwrNghEutraCell->enbId   = msmNghEutraCell->cid; 
   /*Do not do right shift by 8 for MACRO as it is done in EMM module*/
   RLOG3(L_DEBUG,"Filling Neighbour EUTRA cell parameters, ENB type : %lu, "
      "cid : %lu, enbid : %lu",
      lwrNghEutraCell->enbType,msmNghEutraCell->cid, lwrNghEutraCell->enbId);

   lwrNghEutraCell->cio     = msmNghEutraCell->cio;
   lwrNghEutraCell->enbPlmn.numMncDigits = msmNghEutraCell->plmnId[0].numMncDigits;
   lwrNghEutraCell->enbPlmn.mcc[0] = msmNghEutraCell->plmnId[0].mcc[0];
   lwrNghEutraCell->enbPlmn.mcc[1] = msmNghEutraCell->plmnId[0].mcc[1];
   lwrNghEutraCell->enbPlmn.mcc[2] = msmNghEutraCell->plmnId[0].mcc[2];

   lwrNghEutraCell->enbPlmn.mnc[0] = msmNghEutraCell->plmnId[0].mnc[0];
   lwrNghEutraCell->enbPlmn.mnc[1] = msmNghEutraCell->plmnId[0].mnc[1];
   lwrNghEutraCell->enbPlmn.mnc[2] = msmNghEutraCell->plmnId[0].mnc[2];

   lwrNghEutraCell->plmnId[0].numMncDigits = msmNghEutraCell->plmnId[0].numMncDigits;
   lwrNghEutraCell->plmnId[0].mcc[0] = msmNghEutraCell->plmnId[0].mcc[0];
   lwrNghEutraCell->plmnId[0].mcc[1] = msmNghEutraCell->plmnId[0].mcc[1];
   lwrNghEutraCell->plmnId[0].mcc[2] = msmNghEutraCell->plmnId[0].mcc[2];
               
   lwrNghEutraCell->plmnId[0].mnc[0] = msmNghEutraCell->plmnId[0].mnc[0];
   lwrNghEutraCell->plmnId[0].mnc[1] = msmNghEutraCell->plmnId[0].mnc[1];
   lwrNghEutraCell->plmnId[0].mnc[2] = msmNghEutraCell->plmnId[0].mnc[2];

   lwrNghEutraCell->numPlmn = 1;
   lwrNghEutraCell->pci     = msmNghEutraCell->pci;
   lwrNghEutraCell->qoffset = msmNghEutraCell->qoffset;
   lwrNghEutraCell->dlBw    = msmNghEutraCell->dlBw; /*msmNghEutraCell->dlBw; wrSmDfltCellDlBW;*/
  /*ccpu00136251-Mod-TAC should be from OAM not from SIB1*/
   lwrNghEutraCell->tac     = msmNghEutraCell->tac;
   lwrNghEutraCell->ulBw    = msmNghEutraCell->ulBw; /*msmNghEutraCell->ulBw; wrSmDfltCellUlBW;*/
   lwrNghEutraCell->earfcnDl= msmNghEutraCell->earfcnDl;
   lwrNghEutraCell->earfcnUl= msmNghEutraCell->earfcnUl;
   lwrNghEutraCell->blackListed = msmNghEutraCell->BlackListed;
   lwrNghEutraCell->refSigPower = msmNghEutraCell->RSTxPower;
   /* CNM_DEV start */
   lwrNghEutraCell->numAntenna = msmNghEutraCell->numAntenna;
   lwrNghEutraCell->ulDlSubFrameCfg = msmNghEutraCell->ulDlSubFrameCfg;
   lwrNghEutraCell->splSfCfg = msmNghEutraCell->splSfCfg;
   lwrNghEutraCell->cpDl = msmNghEutraCell->cpDl;
   lwrNghEutraCell->cpUl = msmNghEutraCell->cpUl;
   lwrNghEutraCell->rsrp = msmNghEutraCell->rsrp;
   lwrNghEutraCell->rssi = msmNghEutraCell->rssi;
   /* CNM_DEV end */
  /* CSG_DEV start */
   lwrNghEutraCell->isCsgIdResolved = WR_EMM_NBR_CSG_ID_DISCOVERED;
   lwrNghEutraCell->isCsgAccessModeResolved = WR_EMM_CELL_ACCESS_MODE_DETERMINED;
   lwrNghEutraCell->csgAccessMode = msmNghEutraCell->csgAccessMode;
   lwrNghEutraCell->csgId = msmNghEutraCell->csgId;
   /* CSG_DEV end */
   lwrNghEutraCell->numAntenna = msmNghEutraCell->numAntenna;
   lwrNghEutraCell->ulDlSubFrameCfg = msmNghEutraCell->ulDlSubFrameCfg;
   lwrNghEutraCell->splSfCfg = msmNghEutraCell->splSfCfg;
   lwrNghEutraCell->cpDl = msmNghEutraCell->cpDl;
   lwrNghEutraCell->cpUl = msmNghEutraCell->cpUl;
   lwrNghEutraCell->rsrp = msmNghEutraCell->rsrp;
   lwrNghEutraCell->rssi = msmNghEutraCell->rssi;
   lwrNghEutraCell->isHeMsCell = msmNghEutraCell->isHeMsCell;
}/*end of smFillNghEutraCellParam*/
 
/*
*
*       Fun:   smFillNghGeranCellParam
*
*       Desc:  This function is used for filling Neighbor 
*              Geran cell parameter              
*
*       Ret:   Void
*
*       Notes: None
*              
*       File:  wr_smm_enbapp_cmnplatutils.c 
*
*/
#ifdef ANSI 
PUBLIC Void smFillNghGeranCellParam(LwrGERANNeighCell *lwrNghGeranCell, 
                                   MsmLteGERANNeighCell *msmNghGeranCell,
                                   U32 reCfgType, U32 idx)
#else
PUBLIC Void smFillNghGeranCellParam(lwrNghGERANCell,msmNghGeranCell, reCfgType, idx)
LwrGERANNeighCell *lwrNghGeranCell;
MsmLteGERANNeighCell *msmNghGeranCell;
U32               reCfgType;
U32               idx;
#endif
{
   TRC2(smFillNghGeranCellParam);

   lwrNghGeranCell->reCfgType = reCfgType;
   lwrNghGeranCell->status = wrSmDfltStatus;
   lwrNghGeranCell->nrIndex = idx + 1; 

   lwrNghGeranCell->plmnId.numMncDigits   = msmNghGeranCell->plmnId.numMncDigits;
   lwrNghGeranCell->plmnId.mcc[0]         = msmNghGeranCell->plmnId.mcc[0];
   lwrNghGeranCell->plmnId.mcc[1]         = msmNghGeranCell->plmnId.mcc[1];
   lwrNghGeranCell->plmnId.mcc[2]         = msmNghGeranCell->plmnId.mcc[2];

   lwrNghGeranCell->plmnId.mnc[0]         = msmNghGeranCell->plmnId.mnc[0];
   lwrNghGeranCell->plmnId.mnc[1]         = msmNghGeranCell->plmnId.mnc[1];
   lwrNghGeranCell->plmnId.mnc[2]         = msmNghGeranCell->plmnId.mnc[2];

   lwrNghGeranCell->lac                   = msmNghGeranCell->lac;
   lwrNghGeranCell->rac                   = msmNghGeranCell->rac;
   lwrNghGeranCell->bsic                  = msmNghGeranCell->bsic;
   lwrNghGeranCell->ci                    = msmNghGeranCell->ci;
   lwrNghGeranCell->bandIndicator         = msmNghGeranCell->bandIndicator;
   lwrNghGeranCell->bcchArfcn             = msmNghGeranCell->bcchArfcn;
   lwrNghGeranCell->nccPermittedMeas      = msmNghGeranCell->ncc_PermittedMeas;
   lwrNghGeranCell->isDTMCapable          = msmNghGeranCell->isDTMCapable;
   lwrNghGeranCell->ncoVal               = msmNghGeranCell->networkControlOrder;
   lwrNghGeranCell->ncoPres              = msmNghGeranCell->networkControlOrderPres;
   lwrNghGeranCell->rimSupportForGeranNgh = msmNghGeranCell->rimSupportForGeranNgh;
   lwrNghGeranCell->offsetFreq =msmNghGeranCell->qoffsetGERAN;
   RETVOID;
}/*end of smFillNghGeranCellParam*/


/*
*       Fun:   SmDeInitNghHashList
*
*       Desc:  This function is used for Free The Hash List Created for Neighbor configuration
*
*       Ret:   Void
*
*       Notes: None
*
*       File:  fc_smm_enbapp_cmnplatutils.c
*/
#ifdef ANSI
PRIVATE S16 SmDeInitNghHashList(void)
#else
PRIVATE S16 SmDeInitNghHashList(void)
#endif
{

   SmNeigEutraFreqList *nghEutraFreqList = NULLP;
   SmNeigCdma1xBandList *nghCdms1xBandList = NULLP;
   SmNeigCdma1xCellList *nghCdms1xCellList = NULLP;
   SmNeigCdma1xFreqList *nghCdms1xFreqList = NULLP;
   SmNeigUtraFreqList  *nghUtraFreqList = NULLP;
   SmNeigUtraTddFreqList *nghUtraTddFreqList = NULLP;
   SmNeigEnbList       *nghEnbCfg = NULLP;
   SmNeigEutraCellList *nghEutraCellList = NULLP;
   SmNeigUtraCellList  *nghUtraCellList = NULLP;
   SmNeigUtraTddCellList *nghUtraTddCellList = NULLP;
   SmNeigGeranFreqList *nghGeranFreqList = NULLP;
   SmNeigGeranCellList *nghGeranCellList = NULLP;

   while (ROK == cmHashListGetNext(&(smCfgCb.neighEutraFreqLstCp),NULLP, (PTR *) &nghEutraFreqList))
   {

      cmHashListDelete(&(smCfgCb.neighEutraFreqLstCp), (PTR )nghEutraFreqList);
      if(NULLP != nghEutraFreqList)
      {
         WR_FREE(nghEutraFreqList,sizeof(SmNeigEutraFreqList));  
      }
   }
   cmHashListDeinit(&(smCfgCb.neighEutraFreqLstCp));

   while (ROK == cmHashListGetNext(&(smCfgCb.neighUtraTddFreqLstCp),NULLP, (PTR *) &nghUtraTddFreqList))
   {
      cmHashListDelete(&(smCfgCb.neighUtraTddFreqLstCp),(PTR)nghUtraTddFreqList);

      if(NULLP != nghUtraTddFreqList)
         WR_FREE(nghUtraTddFreqList,sizeof(SmNeigUtraTddFreqList));  
   }
   cmHashListDeinit(&(smCfgCb.neighUtraTddFreqLstCp));


   while (ROK == cmHashListGetNext(&(smCfgCb.neighUtraFreqLstCp),NULLP, (PTR *) &nghUtraFreqList))
   {
      cmHashListDelete(&(smCfgCb.neighUtraFreqLstCp),(PTR) nghUtraFreqList); 
      if(NULLP != nghUtraFreqList)
         WR_FREE(nghUtraFreqList,sizeof(SmNeigUtraFreqList));  
   }
   cmHashListDeinit(&(smCfgCb.neighUtraFreqLstCp));

   while (ROK == cmHashListGetNext(&(smCfgCb.neighEnbLstCp),NULLP, (PTR *) &nghEnbCfg ))
   {

      cmHashListDelete(&(smCfgCb.neighEnbLstCp),(PTR) nghEnbCfg );
      if(NULLP != nghEnbCfg)
         WR_FREE(nghEnbCfg ,sizeof(SmNeigEnbList));  
   }
   cmHashListDeinit(&(smCfgCb.neighEnbLstCp));
   while (ROK == cmHashListGetNext(&(smCfgCb.neighEutraCellLstCp),NULLP, (PTR *) &nghEutraCellList ))
   {
      cmHashListDelete(&(smCfgCb.neighEutraCellLstCp),(PTR ) nghEutraCellList );
      if(NULLP != nghEutraCellList)
         WR_FREE(nghEutraCellList ,sizeof(SmNeigEutraCellList));  
   }
   cmHashListDeinit(&(smCfgCb.neighEutraCellLstCp));
   while (ROK == cmHashListGetNext(&(smCfgCb.neighUtraTddCellLstCp),NULLP, (PTR *) &nghUtraTddCellList ))
   {
      cmHashListDelete(&(smCfgCb.neighUtraTddCellLstCp),(PTR) nghUtraTddCellList );
      if(NULLP != nghUtraTddCellList)
         WR_FREE(nghUtraTddCellList ,sizeof(SmNeigUtraTddCellList ));  
   }
   cmHashListDeinit(&(smCfgCb.neighUtraTddCellLstCp));

   while (ROK == cmHashListGetNext(&(smCfgCb.neighUtraCellLstCp),NULLP, (PTR *) &nghUtraCellList ))
   {
      cmHashListDelete(&(smCfgCb.neighUtraCellLstCp),(PTR ) nghUtraCellList );

      if(NULLP != nghUtraCellList)
         WR_FREE(nghUtraCellList ,sizeof(SmNeigUtraCellList));  
   }
   cmHashListDeinit(&(smCfgCb.neighUtraCellLstCp));
   while (ROK == cmHashListGetNext(&(smCfgCb.neighCdma1xBandLstCp),NULLP, (PTR *) &nghCdms1xBandList ))
   {
      cmHashListDelete(&(smCfgCb.neighCdma1xBandLstCp),(PTR ) nghCdms1xBandList );
      if(NULLP != nghCdms1xBandList)
         WR_FREE(nghCdms1xBandList ,sizeof(SmNeigCdma1xBandList));  
   }
   cmHashListDeinit(&(smCfgCb.neighCdma1xBandLstCp));

   while (ROK == cmHashListGetNext(&(smCfgCb.neighCdma1xFreqLstCp),NULLP, (PTR *) &nghCdms1xFreqList ))
   {
      cmHashListDelete(&(smCfgCb.neighCdma1xFreqLstCp), (PTR ) nghCdms1xFreqList );

      if(NULLP != nghCdms1xFreqList)
         WR_FREE(nghCdms1xFreqList ,sizeof(SmNeigCdma1xFreqList ));  
   }
   cmHashListDeinit(&(smCfgCb.neighCdma1xFreqLstCp));
   while (ROK == cmHashListGetNext(&(smCfgCb.neighCdma1xCellLstCp),NULLP, (PTR *) &nghCdms1xCellList ))
   {
      cmHashListDelete(&(smCfgCb.neighCdma1xCellLstCp), (PTR ) nghCdms1xCellList );

      if(NULLP != nghCdms1xCellList)
         WR_FREE(nghCdms1xCellList ,sizeof(SmNeigCdma1xCellList));  
   }
   cmHashListDeinit(&(smCfgCb.neighCdma1xCellLstCp));
   while (ROK == cmHashListGetNext(&(smCfgCb.neighGeranFreqLstCp),NULLP, (PTR *) &nghGeranFreqList ))
   {

      cmHashListDelete(&(smCfgCb.neighGeranFreqLstCp), (PTR )nghGeranFreqList);
      if(NULLP != nghGeranFreqList)
      {
         WR_FREE(nghGeranFreqList,sizeof(SmNeigGeranFreqList));  

      }
   }
   cmHashListDeinit(&(smCfgCb.neighGeranFreqLstCp));

   while (ROK == cmHashListGetNext(&(smCfgCb.neighGeranCellLstCp),NULLP, (PTR *) &nghGeranCellList ))
   {
      cmHashListDelete(&(smCfgCb.neighGeranCellLstCp),(PTR )nghGeranCellList );

      if(NULLP != nghGeranCellList)
         WR_FREE(nghGeranCellList ,sizeof(SmNeigGeranCellList ));  
   }
   cmHashListDeinit(&(smCfgCb.neighGeranCellLstCp));
   RETVALUE(ROK);
}
/*
*
*       Fun:   smInitNghHashList
*
*       Desc:  This function is used for filling Neighbor configuration 
*
*       Ret:   Void
*
*       Notes: None
*              
*       File:  fc_smm_enbapp_cmnplatutils.c 
*
*/
#ifdef ANSI 
PRIVATE S16 smInitNghHashList()
#else
PRIVATE S16 smInitNghHashList()
#endif
{
   U32 idx;
   U32 enbId = 0;
   SmNeigEutraFreqList *nghEutraFreqList = NULLP;
   SmNeigCdma1xBandList *nghCdms1xBandList = NULLP;
   SmNeigCdma1xCellList *nghCdms1xCellList = NULLP;
   SmNeigCdma1xFreqList *nghCdms1xFreqList = NULLP;
   SmNeigUtraFreqList  *nghUtraFreqList = NULLP;
   SmNeigUtraTddFreqList *nghUtraTddFreqList = NULLP;
   SmNeigEnbList       *nghEnbCfg = NULLP;
   SmNeigEutraCellList *nghEutraCellList = NULLP;
   SmNeigUtraCellList  *nghUtraCellList = NULLP;
   SmNeigUtraTddCellList *nghUtraTddCellList = NULLP;
   MsmEUTRANeighCell   *msmNghEutraCell = NULLP;
   MsmUtraNeighCell    *msmNghUtraCell = NULLP;
   MsmUtraTddNeighCell   *msmNghUtraTddCell = NULLP;
   SmNeigGeranFreqList *nghGeranFreqList = NULLP;
   SmNeigGeranCellList *nghGeranCellList = NULLP;
   MsmLteGERANNeighCell   *msmNghGeranCell = NULLP;
   U16                 cnt = 0;

   TRC2(smInitNghHashList);

   if(ROK != cmHashListInit(&(smCfgCb.neighEutraFreqLstCp), WR_SMM_EUTRAN_FREQ_HASH_NO_BINS,0,
           FALSE,7, smCb.init.region, smCb.init.pool))
   {
      RLOG0(L_ERROR, "Initialisation of hash list for Neighbour EUTRA "
         "frequency failed");
      RETVALUE(RFAILED);
   }
   if(ROK != cmHashListInit(&(smCfgCb.neighUtraTddFreqLstCp), WR_SMM_UTRAN_TDD_FREQ_HASH_NO_BINS,0,
            FALSE,CM_HASH_KEYTYPE_ANY, smCb.init.region, smCb.init.pool))
   {
      RLOG0(L_ERROR, "Initialisation of hash list for Neighbour UTRA TDD "
         "frequency failed");
      RETVALUE(RFAILED);
   }
   if(ROK != cmHashListInit(&(smCfgCb.neighUtraFreqLstCp), WR_SMM_UTRAN_FREQ_HASH_NO_BINS,0,
           FALSE,CM_HASH_KEYTYPE_ANY, smCb.init.region, smCb.init.pool))
   {
      RLOG0(L_ERROR, "Initialisation of hash list for Neighbour UTRA frequency "
         "failed");
      RETVALUE(RFAILED);
   }
   if(ROK != cmHashListInit(&(smCfgCb.neighEnbLstCp), WR_SMM_NGH_ENB_HASH_NO_BINS, 0,
           FALSE,CM_HASH_KEYTYPE_ANY, smCb.init.region, smCb.init.pool))
   {
      RLOG0(L_ERROR, "Initialisation of hash list for Neighbour ENB failed");
      RETVALUE(RFAILED);
    }

   if(ROK != cmHashListInit(&(smCfgCb.neighEutraCellLstCp), WR_SMM_EUTRAN_CELL_HASH_NO_BINS,0,
           FALSE,CM_HASH_KEYTYPE_ANY, smCb.init.region, smCb.init.pool))
   {
      RLOG0(L_ERROR, "Initialisation of hash list for Neighbour EUTRA cell "
         "failed");
      RETVALUE(RFAILED);
   }
   if(ROK != cmHashListInit(&(smCfgCb.neighUtraTddCellLstCp), WR_SMM_UTRAN_TDD_CELL_HASH_NO_BINS,0,
            FALSE,CM_HASH_KEYTYPE_ANY, smCb.init.region, smCb.init.pool))
   {
      RLOG0(L_ERROR, "Initialisation of hash list for Neighbour UTRA TDD cell "
         "failed");
      RETVALUE(RFAILED);
   }
   if(ROK != cmHashListInit(&(smCfgCb.neighUtraCellLstCp), WR_SMM_UTRAN_CELL_HASH_NO_BINS,0,
           FALSE,CM_HASH_KEYTYPE_ANY, smCb.init.region, smCb.init.pool))
   {
      RLOG0(L_ERROR, "Initialisation of hash list for Neighbour UTRA cell "
         "failed");
      RETVALUE(RFAILED);
   }
   if(ROK != cmHashListInit(&(smCfgCb.neighCdma1xBandLstCp), WR_SMM_CDMA1X_CELL_HASH_NO_BINS,0,
           FALSE,7, smCb.init.region, smCb.init.pool))
   {
      RLOG0(L_ERROR, "Initialisation of hash list for Neighbour CDMA1X band "
         "failed");
      RETVALUE(RFAILED);
   }
   if(ROK != cmHashListInit(&(smCfgCb.neighCdma1xFreqLstCp), WR_SMM_CDMA1X_CELL_HASH_NO_BINS,0,
           FALSE,7, smCb.init.region, smCb.init.pool))
   {
      RLOG0(L_ERROR, "Initialisation of hash list for Neighbour CDMA1X "
         "frequency failed");
      RETVALUE(RFAILED);
   }
   if(ROK != cmHashListInit(&(smCfgCb.neighCdma1xCellLstCp), WR_SMM_CDMA1X_CELL_HASH_NO_BINS,0,
           FALSE,7, smCb.init.region, smCb.init.pool))
   {
      RLOG0(L_ERROR, "Initialisation of hash list for Neighbour CDMA1X cell "
         "failed");
      RETVALUE(RFAILED);
   }
   if(ROK != cmHashListInit(&(smCfgCb.neighGeranFreqLstCp), WR_SMM_GERAN_FREQ_HASH_NO_BINS,0,
           FALSE,7, smCb.init.region, smCb.init.pool))
   {
      RLOG0(L_ERROR, "Initialisation of hash list for Neighbour GERAN "
         "frequency failed");
      RETVALUE(RFAILED);
   }
   if(ROK != cmHashListInit(&(smCfgCb.neighGeranCellLstCp), WR_SMM_GERAN_CELL_HASH_NO_BINS,0,
           FALSE,CM_HASH_KEYTYPE_ANY, smCb.init.region, smCb.init.pool))
   {
      RLOG0(L_ERROR, "Initialisation of hash list for Neighbour GERAN cell "
         "failed");
      RETVALUE(RFAILED);
   }
 
   /*adding Eutra freq list to hash table  */
   for(idx = 0; idx < lteeNodeBparams.lteNeighFreqCfg.numEutraFreq; idx++)
   {
      WR_ALLOC(&nghEutraFreqList,sizeof(SmNeigEutraFreqList));
      if(NULLP == nghEutraFreqList)
      {
        RLOG0(L_FATAL, "Memory allocation failed while creating Neighbour "
           "EUTRA frequency");
        RETVALUE(RFAILED);
      }

      smFillNghEutraFreqParam(&nghEutraFreqList->nghEutraFreqCfg, 
            &lteeNodeBparams.lteNeighFreqCfg.t.addNeighFreq[idx].eutranFreq,
            wrSmDfltReCfgType);
      if(idx == 0)
      {
         /*Fix for CR ccpu00148373*/  
         if(lteeNodeBparams.lteNeighFreqCfg.t.addNeighFreq[idx].eutranFreq.measBandwidth > lteeNodeBparams.lteCellMibCfgGrp.dlBw[0])
         {
            nghEutraFreqList->nghEutraFreqCfg.measBandwidth.pres = PRSNT_NODEF;
            nghEutraFreqList->nghEutraFreqCfg.measBandwidth.val = lteeNodeBparams.lteCellMibCfgGrp.dlBw[0];
         }
         else
         {
            nghEutraFreqList->nghEutraFreqCfg.measBandwidth.pres = PRSNT_NODEF;
            nghEutraFreqList->nghEutraFreqCfg.measBandwidth.val = lteeNodeBparams.lteNeighFreqCfg.t.addNeighFreq[idx].eutranFreq.measBandwidth;
         }
      }

       /*Adding the  neighbour CB into hash list*/       
      if(cmHashListInsert(&(smCfgCb.neighEutraFreqLstCp),
               (PTR)nghEutraFreqList, (U8 *)&nghEutraFreqList->nghEutraFreqCfg.earfcnDl, 
                   sizeof(nghEutraFreqList->nghEutraFreqCfg.earfcnDl)) != ROK)
      {
         RLOG1(L_ERROR, "Adding Neighbour EUTRA frequency into hashlist failed"
            " for EARFCN : %d", nghEutraFreqList->nghEutraFreqCfg.earfcnDl);
         WR_FREE(nghEutraFreqList, sizeof(SmNeigEutraFreqList));
         continue;/*can't configure same freq two times*/
      }
      RLOG1(L_INFO,"Added neighbour frequency EARFCN DL %d into hashlist",
         nghEutraFreqList->nghEutraFreqCfg.earfcnDl);
   } 

   /*adding Utra freq list to hash table  */
   for(idx = 0; idx < lteeNodeBparams.lteNeighFreqCfg.numUtraFreq; idx++)
   {
      WR_ALLOC(&nghUtraFreqList, sizeof(SmNeigUtraFreqList));
      if(NULLP == nghUtraFreqList)
      {
         RLOG0(L_FATAL, "Memory allocation failed while creating Neighbour "
            "UTRA frequency");
         RETVALUE(RFAILED);
      }

      smFillNghUtraFreqParam(&nghUtraFreqList->nghUtraFreqCfg, 
                           &lteeNodeBparams.lteNeighFreqCfg.t.addNeighFreq[idx].utranFreq,
                           wrSmDfltReCfgType);

      if(cmHashListInsert(&(smCfgCb.neighUtraFreqLstCp),
               (PTR)nghUtraFreqList, (U8 *)&nghUtraFreqList->nghUtraFreqCfg.t.utraFdd.arfcnDl,
               sizeof(nghUtraFreqList->nghUtraFreqCfg.t.utraFdd.arfcnDl)) != ROK)
      {
         RLOG1(L_ERROR, "Adding Neighbour UTRA frequency into hashlist failed "
            "for ARFCN : %d",nghUtraFreqList->nghUtraFreqCfg.t.utraFdd.arfcnDl);
         WR_FREE(nghUtraFreqList, sizeof(SmNeigUtraFreqList));
         continue;/*can't configure same freq two times*/
      }
   }

   /*adding Utra Tdd freq list to hash table  */
   for(idx = 0; idx < lteeNodeBparams.lteNeighFreqCfg.numUtraTddFreq; idx++)
   {
      WR_ALLOC(&nghUtraTddFreqList, sizeof(SmNeigUtraTddFreqList));
      if(NULLP == nghUtraTddFreqList)
      {
         RLOG0(L_FATAL, "Memory allocation failed while creating UTRA TDD "
            "frequency");
         RETVALUE(RFAILED);
      }

      smFillNghUtraTddFreqParam(&nghUtraTddFreqList->nghUtraTddFreqCfg, 
            &lteeNodeBparams.lteNeighFreqCfg.t.addNeighFreq[idx].utranTddFreq,
            wrSmDfltReCfgType);

      if(cmHashListInsert(&(smCfgCb.neighUtraTddFreqLstCp),
               (PTR)nghUtraTddFreqList, (U8 *)&nghUtraTddFreqList->nghUtraTddFreqCfg.t.utraTdd.arfcn,
               sizeof(nghUtraTddFreqList->nghUtraTddFreqCfg.t.utraTdd.arfcn)) != ROK)
      {
         RLOG1(L_ERROR, "Adding Neighbour UTRA TDD frequency into hashlist "
            "failed for ARFCN : %d",
            nghUtraTddFreqList->nghUtraTddFreqCfg.t.utraTdd.arfcn);
         WR_FREE(nghUtraTddFreqList, sizeof(SmNeigUtraTddFreqList));
         continue;/*can't configure same freq two times*/
      }
   }

   /*adding Neghbor enodeb to list to hash table  */
   for(idx = 0; idx < lteeNodeBparams.lteNeighCellCfg.numEutraCells; idx++)
   {
      msmNghEutraCell = &lteeNodeBparams.lteNeighCellCfg.t.neighAddCfg[idx].eutraCell;
      RLOG2(L_INFO,"EUTRA Neighbor cell %d added with DL frequency %d", 
         idx+1, msmNghEutraCell->earfcnDl);
      RLOG3(L_INFO,"PLMN: MCC[0], MCC[1],MCC[2] %d %d %d",
         msmNghEutraCell->plmnId[0].mcc[0], msmNghEutraCell->plmnId[0].mcc[1],
         msmNghEutraCell->plmnId[0].mcc[2]);
      RLOG2(L_INFO,"PLMN: MNC[0], MNC[1] %d %d",
         msmNghEutraCell->plmnId[0].mnc[0], msmNghEutraCell->plmnId[0].mnc[1]);
      if(ROK != cmHashListFind(&(smCfgCb.neighEutraFreqLstCp), (U8*)&msmNghEutraCell->earfcnDl,
               sizeof(msmNghEutraCell->earfcnDl), 0,(PTR*) &nghEutraFreqList))
      {
         RLOG1(L_ERROR, "Neighbour eNodeB cannot be added because corresponding"
            " frequency for EARFCN %d is not configured", 
            msmNghEutraCell->earfcnDl);
         continue;
      }

      if(WR_ENODEB_TYPE_MACRO == smGetEnbType(msmNghEutraCell->pci))
      {
        enbId = ((msmNghEutraCell->cid) >>8);
      }
      else
      {
        enbId = msmNghEutraCell->cid;
      }
      if(ROK == cmHashListFind(&(smCfgCb.neighEnbLstCp), (U8*)&enbId,
               sizeof(enbId), 0,(PTR*) &nghEnbCfg))
      {
         nghEnbCfg->numCell++;
         continue;
      }

      WR_ALLOC(&nghEnbCfg, sizeof(SmNeigEnbList));
      if(NULLP == nghEnbCfg)
      {
        RLOG0(L_FATAL, "Memory allocation failed while creating Neighbour "
           "eNodeB");
        RETVALUE(RFAILED);
      }
      smFillNghEnbParam(nghEnbCfg,
            &lteeNodeBparams.lteNeighCellCfg.t.neighAddCfg[idx].eutraCell);

      if(cmHashListInsert(&(smCfgCb.neighEnbLstCp),
               (PTR)nghEnbCfg, (U8 *)&nghEnbCfg->nghEnbCfg.enbId,
                      sizeof(nghEnbCfg->nghEnbCfg.enbId)) != ROK)
      {
         RLOG1(L_ERROR, "Adding Neighbour eNodeB %ld into hashlist failed",
            nghEnbCfg->nghEnbCfg.enbId);
         WR_FREE(nghEnbCfg, sizeof(SmNeigEnbList));
         RETVALUE(RFAILED);
      }
   }

   /*adding Eutra Cell list to hash table  */
   for(idx = 0; idx < lteeNodeBparams.lteNeighCellCfg.numEutraCells; idx++)
   {
      msmNghEutraCell = &lteeNodeBparams.lteNeighCellCfg.t.neighAddCfg[idx].eutraCell;

      if(ROK != cmHashListFind(&(smCfgCb.neighEutraFreqLstCp), (U8*)&msmNghEutraCell->earfcnDl,
               sizeof(msmNghEutraCell->earfcnDl), 0,(PTR*) &nghEutraFreqList))
      {
         RLOG1(L_ERROR, "EUTRAN cell cannot be added because corresponding "
            "frequency for EARFCN %d is not configured", 
            msmNghEutraCell->earfcnDl);
         continue;
      }

      WR_ALLOC(&nghEutraCellList, sizeof(SmNeigEutraCellList));
      if(NULLP == nghEutraCellList)
      {
        RLOG0(L_FATAL, "Memory allocation failed while creating neighbour "
           "EUTRA cell");
        RETVALUE(RFAILED);
      }

      smFillNghEutraCellParam(&nghEutraCellList->nghEutraCellCfg,
                               &lteeNodeBparams.lteNeighCellCfg.t.neighAddCfg[idx].eutraCell,
                               wrSmDfltReCfgType);
      /*add all SON related parameter*/
      nghEutraCellList->sonNghCellCfg.nghCellCfg = 
                      &nghEutraCellList->nghEutraCellCfg;
      nghEutraCellList->sonNghCellCfg.isPrachConfig = 
                      msmNghEutraCell->isPrachConfig;
      nghEutraCellList->sonNghCellCfg.rootSeqIndex = 
                      msmNghEutraCell->prachCfgInfo.rootSeqIndex;
      nghEutraCellList->sonNghCellCfg.zeroCorrelationZoneCfg = 
                      msmNghEutraCell->prachCfgInfo.zeroCorrelationZoneCfg;
      nghEutraCellList->sonNghCellCfg.highSpeedFlag = 
                      msmNghEutraCell->prachCfgInfo.highSpeedFlag;
      nghEutraCellList->sonNghCellCfg.prachCfgIdx = 
                      msmNghEutraCell->prachCfgInfo.prachCfgIdx;
      nghEutraCellList->sonNghCellCfg.prachFreqOffset = 
                      msmNghEutraCell->prachCfgInfo.prachFreqOffset;
      nghEutraCellList->sonNghCellCfg.lteBand =
         msmNghEutraCell->lteBand;
      nghEutraCellList->sonNghCellCfg.isCellBarred =
         msmNghEutraCell->isCellBarred;
      nghEutraCellList->sonNghCellCfg.numNghNgh = 
                      msmNghEutraCell->numIntraFreqNeighbors;
      cmMemcpy((U8 *) &(nghEutraCellList->sonNghCellCfg.nghNghcellPci[0]), 
             (U8 *) &(msmNghEutraCell->intraFreqNghPci[0]), 
              msmNghEutraCell->numIntraFreqNeighbors * sizeof(msmNghEutraCell->intraFreqNghPci[0]));
   
      /*hash key */
      smFillNghHashKeyInit(&nghEutraCellList->hashKey, 
                    (LwrPlmnId *)&lteeNodeBparams.lteNeighCellCfg.t.neighAddCfg[idx].eutraCell.plmnId[0],
                     (U32)(lteeNodeBparams.lteNeighCellCfg.t.neighAddCfg[idx].eutraCell.cid));
      
      if(cmHashListInsert(&(smCfgCb.neighEutraCellLstCp),
               (PTR)nghEutraCellList, (U8 *)&nghEutraCellList->hashKey, sizeof(smNghCellHasKey)) != ROK)
      {
         RLOG1(L_ERROR, "Insertion of EUTRA cell %ld into hash list failed "
            "because of duplicate entry", 
            (U32)lteeNodeBparams.lteNeighCellCfg.t.neighAddCfg[idx].eutraCell.cid);
         WR_FREE(nghEutraCellList, sizeof(SmNeigEutraCellList));
         continue;
      }
   }
   
   /*adding Utra Cell list to hash table  */
   for(idx = 0; idx < lteeNodeBparams.lteNeighCellCfg.numUtraCells; idx++)
   {
      msmNghUtraCell = &lteeNodeBparams.lteNeighCellCfg.t.neighAddCfg[idx].utranCell;

      if(ROK != cmHashListFind(&(smCfgCb.neighUtraFreqLstCp), (U8*)&msmNghUtraCell->arfcnDl,
               sizeof(msmNghUtraCell->arfcnDl), 0,(PTR*) &nghUtraFreqList))
      {
         RLOG1(L_ERROR, "UTRAN cell cannot be added because corresponding "
            "frequency for ARFCN %d is not configured", 
            msmNghUtraCell->arfcnDl);
         continue;
      }

      WR_ALLOC(&nghUtraCellList, sizeof(SmNeigUtraCellList));
      if(NULLP == nghUtraCellList)
      {
        RLOG0(L_FATAL, "Memory allocation failed while creating Neighbour "
           "UTRA cell");
        RETVALUE(RFAILED);
      }
      smFillNghUtraCellParam(&nghUtraCellList->nghUtraCellCfg, msmNghUtraCell, wrSmDfltReCfgType);

      smFillNghHashKeyInit(&nghUtraCellList->hashKey, (LwrPlmnId *)&msmNghUtraCell->plmnId, (U32)msmNghUtraCell->cId);

      if(cmHashListInsert(&(smCfgCb.neighUtraCellLstCp),
               (PTR)nghUtraCellList, (U8 *)&nghUtraCellList->hashKey, sizeof(smNghCellHasKey)) != ROK)
      {
         RLOG1(L_ERROR, "Insertion of UTRA cell %ld into hash list failed since "
            "because of duplicate entry",msmNghUtraCell->cId);
         WR_FREE(nghUtraCellList, sizeof(SmNeigUtraCellList));
      }
   }

   /*adding Utra Tdd Cell list to hash table  */
   for(idx = 0; idx < lteeNodeBparams.lteNeighCellCfg.numUtraTddCells; idx++)
   {
      msmNghUtraTddCell = &lteeNodeBparams.lteNeighCellCfg.t.neighAddCfg[idx].utranTddCell;

      if(ROK != cmHashListFind(&(smCfgCb.neighUtraTddFreqLstCp), (U8*)&msmNghUtraTddCell->arfcn,
               sizeof(msmNghUtraTddCell->arfcn), 0,(PTR*) &nghUtraTddFreqList))
      {
         RLOG1(L_ERROR, "UTRAN TDD cell cannot be added because corresponding "
            "frequency for ARFCN %d is not configured",msmNghUtraTddCell->arfcn);
         continue;
      }

      WR_ALLOC(&nghUtraTddCellList, sizeof(SmNeigUtraTddCellList));
      if(NULLP == nghUtraTddCellList)
      {
         RLOG0(L_FATAL, "Memory allocation failed while creating UTRA TDD cell");
         RETVALUE(RFAILED);
      }
      smFillNghUtraTddCellParam(&nghUtraTddCellList->nghUtraTddCellCfg, msmNghUtraTddCell, wrSmDfltReCfgType);

      smFillNghHashKeyInit(&nghUtraTddCellList->hashKey, (LwrPlmnId *)&msmNghUtraTddCell->plmnId, (U32)msmNghUtraTddCell->cId);

      if(cmHashListInsert(&(smCfgCb.neighUtraTddCellLstCp),
               (PTR)nghUtraTddCellList, (U8 *)&nghUtraTddCellList->hashKey, sizeof(smNghCellHasKey)) != ROK)
      {
         RLOG1(L_ERROR, "Insertion of UTRA TDD cell %d into hash list failed "
            "because of duplicate entry",msmNghUtraCell->cId);
         WR_FREE(nghUtraCellList, sizeof(SmNeigUtraCellList));
      }
   }

   /* adding band list */
   for(idx = 0; idx < lteeNodeBparams.cdmaBandClassParams.numBandClass; idx++)
   {
      WR_ALLOC(&nghCdms1xBandList,sizeof(SmNeigCdma1xBandList));
      if(NULLP == nghCdms1xBandList)
      {
         RLOG0(L_FATAL, "Memory allocation failed while creating CDMA1x Band "
            "class");
         RETVALUE(RFAILED);
      }

      smFillNghCdma1xBandParam(&nghCdms1xBandList->nghCdma1xBandCfg,
           &lteeNodeBparams.cdmaBandClassParams.t.bandClassAddCfg[idx].t.bandClass1XRtt,
           wrSmDfltReCfgType, idx);

       /*Adding the  neighbour CB into hash list*/       
      if(cmHashListInsert(&(smCfgCb.neighCdma1xBandLstCp),
               (PTR)nghCdms1xBandList, (U8 *)&nghCdms1xBandList->nghCdma1xBandCfg.bandClass, 
                   sizeof(nghCdms1xBandList->nghCdma1xBandCfg.bandClass)) != ROK)
      {
         RLOG1(L_ERROR, "Insertion of CDMA1x band %ld into hash list failed "
            "because of duplicate entry",
             nghCdms1xBandList->nghCdma1xBandCfg.bandClass);
         WR_FREE(nghCdms1xBandList, sizeof(SmNeigCdma1xBandList));
         continue;/*can't configure same Band two times*/
      }
   } 

   /*adding CDMA 1XRTT cell list to hash table  */
   for(idx = 0; idx < lteeNodeBparams.lteNeighCellCfg.numCdma1xCells; idx++)
   {
      WR_ALLOC(&nghCdms1xCellList,sizeof(SmNeigCdma1xCellList));
      if(NULLP == nghCdms1xCellList)
      {
         RLOG0(L_FATAL, "Memory allocation failed while creating CDMA1x cell");
         RETVALUE(RFAILED);
      }
      WR_ALLOC(&nghCdms1xFreqList,sizeof(SmNeigCdma1xFreqList));
      if(NULLP == nghCdms1xFreqList)
      {
         RLOG0(L_FATAL, "Memory allocation failed while creating CDMA1x "
            "frequency");
         RETVALUE(RFAILED);
      }
 
      if(ROK == smFillNghCdma1xCellParam(&nghCdms1xCellList->nghCdma1xCellCfg,
           &lteeNodeBparams.lteNeighCellCfg.t.neighAddCfg[idx].cdma1xCell,
           &nghCdms1xFreqList->nghCdma1xFreqCfg, wrSmDfltReCfgType,cnt))
      {
         cnt++;
      }
  
       /*Adding the  neighbour CB into hash list*/       
      if(cmHashListInsert(&(smCfgCb.neighCdma1xCellLstCp),
               (PTR)nghCdms1xCellList, (U8 *)&nghCdms1xCellList->nghCdma1xCellCfg.nrIndex, 
                   sizeof(nghCdms1xCellList->nghCdma1xCellCfg.nrIndex)) != ROK)
      {
         RLOG1(L_ERROR, "Insertion of CDMA1x cell %ld into hash list failed "
            "because of duplicate entry",
            nghCdms1xCellList->nghCdma1xCellCfg.nrIndex);
         WR_FREE(nghCdms1xCellList, sizeof(SmNeigCdma1xCellList));
         continue;/*can't configure same Cell two times*/
      }
      /*Adding the  neighbour CB into hash list*/       
      if(cmHashListInsert(&(smCfgCb.neighCdma1xFreqLstCp),
               (PTR)nghCdms1xFreqList, (U8 *)&nghCdms1xFreqList->nghCdma1xFreqCfg.arfcn, 
                   sizeof(nghCdms1xFreqList->nghCdma1xFreqCfg.arfcn)) != ROK)
      {
         RLOG1(L_ERROR, "Insertion of CDMA1x frequency %ld into hash list "
            "failed because of duplicate entry",
            nghCdms1xFreqList->nghCdma1xFreqCfg.arfcn);
         WR_FREE(nghCdms1xFreqList, sizeof(SmNeigCdma1xFreqList));
         continue;/*can't configure same freq two times*/
      }
 
   }

   /*adding GERAN freq list to hash table  */
   for(idx = 0; idx < lteeNodeBparams.lteNeighFreqCfg.numGeranFreq; idx++)
   {
      WR_ALLOC(&nghGeranFreqList,sizeof(SmNeigGeranFreqList));
      if(NULLP == nghGeranFreqList)
      {
        RLOG0(L_FATAL, "Memory allocation failed while creating GERAN frequency");
        RETVALUE(RFAILED);
      }

      smFillNghGeranFreqParam(&nghGeranFreqList->nghGeranFreqCfg, 
            &lteeNodeBparams.lteNeighFreqCfg.t.addNeighFreq[idx].geranFreq,
            wrSmDfltReCfgType, idx);
      /*Adding the  neighbour CB into hash list*/       
      if(cmHashListInsert(&(smCfgCb.neighGeranFreqLstCp),
               (PTR)nghGeranFreqList, (U8 *)&nghGeranFreqList->nghGeranFreqCfg.bcchArfcn, 
                   sizeof(nghGeranFreqList->nghGeranFreqCfg.bcchArfcn)) != ROK)
      {
         RLOG1(L_ERROR, "Insertion of GERAN frequency %ld into hash list "
            "failed because of duplicate entry",
            nghGeranFreqList->nghGeranFreqCfg.bcchArfcn);
         WR_FREE(nghGeranFreqList, sizeof(SmNeigGeranFreqList));
         continue;/*can't configure same freq two times*/
      }
   }

   /*adding Geran Cell list to hash table  */
   for(idx = 0; idx < lteeNodeBparams.lteNeighCellCfg.numGeranCells; idx++)
   {
      msmNghGeranCell = &lteeNodeBparams.lteNeighCellCfg.t.neighAddCfg[idx].geranCell;

      if(ROK != cmHashListFind(&(smCfgCb.neighGeranFreqLstCp), (U8*)&msmNghGeranCell->bcchArfcn,
               sizeof(msmNghGeranCell->bcchArfcn), 0,(PTR*) &nghGeranFreqList))
      {
         RLOG1(L_ERROR, "GERAN cell cannot be added because corresponding "
            "frequency for ARFCN %d is not configured", msmNghGeranCell->bcchArfcn);
         continue;
      }

      WR_ALLOC(&nghGeranCellList, sizeof(SmNeigGeranCellList));
      if(NULLP == nghGeranCellList)
      {
        RLOG0(L_FATAL, "Memory allocation failed while creating GERAN cell");
        RETVALUE(RFAILED);
      }

      smFillNghGeranCellParam(&nghGeranCellList->nghGeranCellCfg,
                               &lteeNodeBparams.lteNeighCellCfg.t.neighAddCfg[idx].geranCell,
                               wrSmDfltReCfgType, idx);
      /*hash key */
      smFillNghHashKeyInit(&nghGeranCellList->hashKey, 
                    (LwrPlmnId *)&lteeNodeBparams.lteNeighCellCfg.t.neighAddCfg[idx].geranCell.plmnId,
                     (U32)lteeNodeBparams.lteNeighCellCfg.t.neighAddCfg[idx].geranCell.ci);
      
      if(cmHashListInsert(&(smCfgCb.neighGeranCellLstCp),
               (PTR)nghGeranCellList, (U8 *)&nghGeranCellList->hashKey, sizeof(smNghCellHasKey)) != ROK)
      {
         RLOG1(L_ERROR, "Insertion of GERAN cell %ld into hash list failed "
            "because of duplicate entry",
            (U32)lteeNodeBparams.lteNeighCellCfg.t.neighAddCfg[idx].geranCell.ci);
         WR_FREE(nghGeranCellList, sizeof(SmNeigGeranCellList));
         continue;
      }
   }
   RETVALUE(ROK);
} /*end of smInitNghHashList*/
 
/*
*
*       Fun:  smValidateFillMmeIp 
*
*       Desc:  validate for duplicacy and fill mme Ip
*
*       Ret:   ROK
*
*       Notes: None
*              
*       File:  wr_smm_enbapp_cmnplatutils.c 
*
*/

PUBLIC S16 smValidateFillMmeIp
(
 LwrSmMmeCfg *mmeCfg,
 U8          *idx,
 Mmeiptype   ip
)
{
   U8 idx1 = 0;
   U8 ret = ROK;
   U8 idx2 = *idx;
   U8 emtpyIpv6Addr[WR_IPV6_ADDR_LEN] = {0};

   for(idx1 = 0; idx1 < idx2; idx1++)
   {
      if((0 == ip.mmeipv4) && 
         (ROK == cmMemcmp((U8*)&ip.mmeipv6, (U8*)&emtpyIpv6Addr,
          sizeof(CmInetIpAddr6))))
      {
         ret = RFAILED;
         RLOG0(L_INFO, "Secondary MME IP is 0");
         break;
      }
      else  
      {
         /* Check if IPv4 address is same as received */
         if((mmeCfg->mmeAddr[idx1].type == CM_TPTADDR_IPV4) && (ip.ipv4pres == TRUE) 
            && (mmeCfg->mmeAddr[idx1].u.ipv4TptAddr.address == ip.mmeipv4))
         {
            ret = ROKDNA;
            RLOG0(L_ERROR, "MME IPv4 is duplicate");
            break;
         }
         /* Check if IPv6 address is same as received */
         else if((mmeCfg->mmeAddr[idx1].type == CM_TPTADDR_IPV6) && (ip.ipv6pres == TRUE) 
            && ( ROK == cmMemcmp((U8*)&mmeCfg->mmeAddr[idx1].u.ipv6TptAddr.ipv6NetAddr, 
                 (U8*)&ip.mmeipv6,sizeof(CmInetIpAddr6))))
         {
            ret = ROKDNA;
            RLOG0(L_ERROR, "MME IPv6 is duplicate");
            break;
         }
      }
   }
   if(ROK == ret)
   {
      if(ip.ipv4pres == TRUE)
      {
         mmeCfg->mmeAddr[*idx].type = CM_TPTADDR_IPV4;
         mmeCfg->mmeAddr[*idx].u.ipv4TptAddr.address = ip.mmeipv4;
      }
      if(ip.ipv6pres == TRUE)
      {
         mmeCfg->mmeAddr[*idx].type = CM_TPTADDR_IPV6;
         cmMemcpy((U8*)&mmeCfg->mmeAddr[*idx].u.ipv6TptAddr.ipv6NetAddr,
                 (U8*)&ip.mmeipv6,sizeof(CmInetIpAddr6));
      }
      *idx = *idx + 1;
   }

   RETVALUE(ret);
} /* end of smValidateFillMmeIp */
 
/*
*
*       Fun:  smInitMmeCfg 
*
*       Desc:  This function is used for filling mme configuration in smCb
*
*       Ret:   Void
*
*       Notes: None
*              
*       File:  fc_smm_enbapp_cmnplatutils.c 
*
*/
#ifdef ANSI 
PUBLIC S16 smInitMmeCfg()
#else
PUBLIC S16 smInitMmeCfg()
#endif
{ 
   U32                  idx = 0;
   U8                   idx1 = 0;
   U8                   idx2 = 0;
   U8                   idx3 = 0;
   U8 emtpyIpv6Addr[WR_IPV6_ADDR_LEN] = {0};
   Mmeiptype            *mmeIpType = NULLP;
   CmTptAddr            addr;

   U8                   ret = ROK;
   LwrSmMmeCfg          *mmeCfg = NULLP;
   MsmLteeNodeBIPparams *mmeIpParam = NULLP;

   TRC2(smInitMmeCfg);

   mmeIpParam = &lteeNodeBparams.lteeNodeBIPparams;
   smCfgCb.freeMmeIdx  = 0;
   smCfgCb.numMmeAdded = 0;
   smCfgCb.noMmeAddCfm = 0;
   smCfgCb.noMmeDelCfm = 0;

   smCfgCb.noOfCfg = mmeIpParam->lteS1connmode;
   if((smCfgCb.noOfCfg <= 0) || (smCfgCb.noOfCfg > LWR_SM_MAX_MMES))
   {
      RLOG1(L_ERROR, "Error in number of MMEs configured %d",smCfgCb.noOfCfg);
      RETVALUE(RFAILED);
   }
      idx1 = 0;
   for(idx = 0; idx < smCfgCb.noOfCfg && idx < MAX_NUM_OF_IP; idx++)
   {
      idx3 = 0;
      WR_ALLOC(&mmeCfg, sizeof(LwrSmMmeCfg));
      if(mmeCfg == NULLP)
      {
         RLOG1(L_FATAL, "Memory allocation failed for MME %d configuration",
            smCfgCb.noOfCfg);
         RETVALUE(RFAILED);
      }
      mmeCfg->mmeId = idx +1;
      /* check for duplicate MME primary ip */
      for(idx2 = 0; idx2 < idx; idx2++)
      {
         /*Check If MME ip is IPV4*/
         if(mmeIpParam->lteSiglinkserverlist[idx].ipv4pres == TRUE)
         {
            if((0 == mmeIpParam->lteSiglinkserverlist[idx].mmeipv4) || (idx1 < LWR_MAX_IP_PER_MME && (smCfgCb.mmeCfg[idx2]->mmeAddr[idx1].u.ipv4TptAddr.address == mmeIpParam->lteSiglinkserverlist[idx].mmeipv4)))
            {
                ret = RFAILED;
                 break;
            }
            mmeIpType = &mmeIpParam->lteSiglinkserverlist[idx];
            if((mmeIpType->ipv4pres == TRUE) && (mmeIpType->mmeipv4 == 0))
            {
                ret = RFAILED;
                break;
            }
         }
         
         /*Check If MME ip is IPV6*/
         if(mmeIpParam->lteSiglinkserverlist[idx].ipv6pres == TRUE)
         {
            if((ROK == cmMemcmp((U8*)&mmeIpParam->lteSiglinkserverlist[idx].mmeipv6,(U8*)&emtpyIpv6Addr, sizeof(CmInetIpAddr6)))||(ROK == cmMemcmp((U8*)&smCfgCb.mmeCfg[idx2]->mmeAddr[idx1].u.ipv6TptAddr.ipv6NetAddr,(U8*)&mmeIpParam->lteSiglinkserverlist[idx].mmeipv6,sizeof(CmInetIpAddr6))))
            {
                ret = RFAILED;
                 break;
            }
            mmeIpType = &mmeIpParam->lteSiglinkserverlist[idx];
            if((mmeIpType->ipv6pres == TRUE) && (ROK == cmMemcmp((U8*)&mmeIpType->mmeipv6,(U8*)&emtpyIpv6Addr, sizeof(CmInetIpAddr6))))
            {
                ret = RFAILED;
                break;
            }
         }
      }
      if(ROK == ret)
      {
         if(idx1 < MAX_NUM_OF_IP && lteeNodeBparams.lteeNodeBIPparams.lteSiglinkserverlist[idx1].ipv4pres == TRUE)
         {   
            addr.type = CM_TPTADDR_IPV4;
            addr.u.ipv4TptAddr.address =lteeNodeBparams.lteeNodeBIPparams.lteSiglinkserverlist[idx1].mmeipv4;
            wrCpyCmTptAddr(&mmeCfg->mmeAddr[idx3++],&addr);
         }
         if(lteeNodeBparams.lteeNodeBIPparams.lteSiglinkserverlist[idx1].ipv6pres == TRUE)
         {
             addr.type = CM_TPTADDR_IPV6;
             cmMemcpy((U8*)addr.u.ipv6TptAddr.ipv6NetAddr,(U8*)lteeNodeBparams.lteeNodeBIPparams.lteSiglinkserverlist[idx1].mmeipv6, sizeof(CmInetIpAddr6));

            wrCpyCmTptAddr(&mmeCfg->mmeAddr[idx3++],&addr);
         }
         idx1++;
      }
      if(RFAILED != smValidateFillMmeIp(mmeCfg,&idx3,mmeIpParam->lteSecIp1[idx]))
      {
         if(RFAILED != smValidateFillMmeIp(mmeCfg,&idx3,mmeIpParam->lteSecIp2[idx]))
         {
            if(RFAILED != smValidateFillMmeIp(mmeCfg,&idx3,mmeIpParam->lteSecIp3[idx]))
            {
               smValidateFillMmeIp(mmeCfg,&idx3,mmeIpParam->lteSecIp4[idx]);
            }
         }
      }
      mmeCfg->noOfIps = idx3;
      smCfgCb.mmeCfg[idx] = mmeCfg;
   }
   RETVALUE(ROK);
}/*end of smInitMmeCfg*/

/*
*
*       Fun:  wrSmConfigComplete 
*
*       Desc:  This function is to 
*
*       Ret:   None
*
*       Notes: <NONE>
*
*       File:  wr_enb_cmnplt_enbapp.c
*
*/
PUBLIC Void wrSmConfigComplete()
{

   TRC2(wrMsmConfigComplete);

   MsmConfigComplete();
   SmUpdateSwVersion(wrGetSwVersion());

   smCb.smState = WR_SM_STATE_OAM_DYN_START;
   RETVOID;
}

PUBLIC Void smConfigFailure()
{

   TRC2(wrMsmConfigComplete);

   MsmConfigFailure();

   RETVOID;
}

/*
*
*       Fun:  smInitDbgCfg
*
*       Desc:  This function is used for filling Debug options in smCb
*
*       Ret:   Void
*
*       Notes: None
*
*       File:  wr_smm_enbapp_cmnplatutils.c
*
*/
#ifdef ANSI
PRIVATE S16 smInitDbgCfg()
#else
PRIVATE S16 smInitDbgCfg()
#endif
{

   TRC2(smInitDbgCfg);

   smCfgCb.hiDbg = lteeNodeBparams.lteDbgParams.hiDbg;
   smCfgCb.sbDbg = lteeNodeBparams.lteDbgParams.sbDbg;
   smCfgCb.szDbg = lteeNodeBparams.lteDbgParams.szDbg;
   smCfgCb.egDbg = lteeNodeBparams.lteDbgParams.egDbg;
   smCfgCb.wrDbg = lteeNodeBparams.lteDbgParams.wrDbg;
   smCfgCb.nhDbg = lteeNodeBparams.lteDbgParams.nhDbg;
   smCfgCb.kwDbg = lteeNodeBparams.lteDbgParams.kwDbg;
   smCfgCb.rgDbg = lteeNodeBparams.lteDbgParams.rgDbg;
   smCfgCb.ysDbg = lteeNodeBparams.lteDbgParams.ysDbg;
   smCfgCb.smDbg = lteeNodeBparams.lteDbgParams.smDbg;
   smCfgCb.czDbg = lteeNodeBparams.lteDbgParams.czDbg;

   RETVALUE(ROK);
}/*end of smInitDbgCfg*/

/* CSG_DEV start */
/*
 *
 *       Fun:   smWrValidateConfigParams
 *
 *       Desc:  Validates the values configured from OAM in MsmLteeNodeBparams.
 *
 *       Ret:   ROK, RFAILED
 *
 *       File:  wr_smm_cfg.c
 *
 */
#ifdef ANSI
PUBLIC S16 smWrValidateConfigParams
(
)
#else
PUBLIC S16 smWrValidateConfigParams (Void)
#endif
{
   U16        maxCsgPci = 0;
   U16        maxSmallCellPci = 0;
   U16        csgPciRange = 0;
   S16        retVal = ROK;
   U16        csgStartPci = lteeNodeBparams.lteSib4CsgInfo.csgStartPhyCellId;
   U16        smallCellStartPci = lteeNodeBparams.lteAddCellCfg.startPci;
   U16        servingCellPci = lteeNodeBparams.lteAddCellCfg.physCellId;
   U32        csgId = lteeNodeBparams.lteCellSib1CfgGrp.cellAccessInfo.csgIdentity;
   U8         accessMode = lteeNodeBparams.lteFapAccessParams.accessMode;
   U8         isCsgPciCfgd = lteeNodeBparams.lteSib4CsgInfo.isCsgPciCfgd;
   U8         isSmallCellPciCfgd = lteeNodeBparams.lteAddCellCfg.isSmallCellPciCfgd;
   U8         eNBAccessMode[4][32] = {"NA","WR_ENB_OPEN_ACCESS","WR_ENB_CLOSED_ACCESS","WR_ENB_HYBRID_ACCESS"}; 
   U16        maxCsgUes = 0;
   U16        maxNonCsgUes = 0;
   U8         minDlResNonCsg = 0;
   U8         minUlResNonCsg = 0;

   /* For serving cell validation EnbType is required */
   wrSmDfltEnbType = smGetEnbType(servingCellPci);

   /* validate serving cell PCI */
   /* Removed one part of the check to make warning free */
   /*if (servingCellPci <  WR_PCI_MIN_VAL || servingCellPci > WR_PCI_MAX_VAL)*/

   if(WR_ENB_OPEN_ACCESS != accessMode && WR_ENODEB_TYPE_MACRO == wrSmDfltEnbType)
   {
      RLOG0(L_ERROR, "Mismatch in AccessMode and PCI Configuration:");
      RLOG_STR(L_ERROR,"AccessMode %s",(PSTR)eNBAccessMode[accessMode]);
      RLOG3(L_ERROR, "servingCellPci %d and it should be in the range from "
         "%d to %d",servingCellPci, lteeNodeBparams.lteAddCellCfg.startPci,
         (lteeNodeBparams.lteAddCellCfg.startPci+lteeNodeBparams.lteAddCellCfg.pciRange-1));

      RETVALUE(RFAILED);
   }

   if(TRUE == isSmallCellPciCfgd)
   {
      maxSmallCellPci = smallCellStartPci +
         lteeNodeBparams.lteAddCellCfg.pciRange;
      /* Removed one part of check to make warning free */   
      /*if(WR_PCI_MIN_VAL > smallCellStartPci || WR_PCI_MAX_VAL < maxSmallCellPci)*/
      if(WR_PCI_MAX_VAL < maxSmallCellPci)
      {
         RLOG2(L_ERROR, "Invalid SmallCell PCI "
            "configuration. SmallCell Start PCI %d Max %d",
            smallCellStartPci, maxSmallCellPci);
         RETVALUE(RFAILED);
      }
   }
   else
   {
      RLOG1(L_ERROR, "Missing SmallCell PCI configuration AccesMode %d",
            accessMode);
      RETVALUE(RFAILED);
   }

   if(TRUE == lteeNodeBparams.lteSib4CsgInfo.isCsgPciCfgd)
   {
      WR_EMM_GET_PCI_RANGE(lteeNodeBparams.lteSib4CsgInfo.csgPciRange,
            csgPciRange);
      maxCsgPci = csgStartPci + csgPciRange;
      /* Removed one part of check to make warning free */
      /*if(WR_PCI_MIN_VAL > csgStartPci || WR_PCI_MAX_VAL < maxCsgPci)*/
      if(WR_PCI_MAX_VAL < maxCsgPci)
      {
         lteeNodeBparams.lteSib4CsgInfo.isCsgPciCfgd = FALSE;
         RLOG2(L_ERROR, "Ingnoring invalid CSG PCI OPEN Access "
           "configuration CSG start PCI %d Max %d",
           csgStartPci, maxCsgPci);
      }
   }

   /* CSG configuration parameter validation */
   switch(accessMode)
   {
      case WR_ENB_OPEN_ACCESS:
      {
         if(WR_ENODEB_TYPE_HOME == wrSmDfltEnbType && 
               (((TRUE == isCsgPciCfgd) && 
                (servingCellPci >= csgStartPci &&
                 servingCellPci <= maxCsgPci)) ||
               (servingCellPci < smallCellStartPci || 
                servingCellPci > maxSmallCellPci)))
         {
            RLOG0(L_ERROR, "Invalid PCI Configuration in OPEN Access");
            RLOG4(L_ERROR, "eNB Type %d PCI %d CSG start PCI %d Max %d",
                  wrSmDfltEnbType, servingCellPci, csgStartPci, maxCsgPci);
            RLOG2(L_ERROR, "SmallCell Start PCI %d Max %d",
                  smallCellStartPci, maxSmallCellPci);
            retVal = RFAILED;
         }
         else if (WR_ENODEB_TYPE_MACRO == wrSmDfltEnbType && 
               (((TRUE == isCsgPciCfgd) && 
                (servingCellPci >= csgStartPci &&
                 servingCellPci <= maxCsgPci)) ||
               (servingCellPci > smallCellStartPci &&
                servingCellPci < maxSmallCellPci)))
         {
            RLOG0(L_ERROR, "Invalid PCI Configuration in OPEN Access");
            RLOG4(L_ERROR, "eNB type %d PCI %d CSG Start PCI %d Max %d",
                  wrSmDfltEnbType, servingCellPci, csgStartPci, maxCsgPci);
            RLOG2(L_ERROR, "SmallCell start PCI %d Max %d",
                  smallCellStartPci, maxSmallCellPci);
            retVal = RFAILED;
         }
      }
      break;
      case WR_ENB_CLOSED_ACCESS:
      {
         if(FALSE == isCsgPciCfgd)
         {
            RLOG0(L_ERROR, "Incomplete Closed Mode CSG configuration");
            RLOG3(L_ERROR, "CSG ID %d CSG PCI configured %d SmallCell PCI "
                  "configured %d", csgId, isCsgPciCfgd, isSmallCellPciCfgd);
            retVal = RFAILED;
         }
         else
         {
            if(0 > csgId || WR_CSG_ID_MAX_VAL < csgId)
            {
               RLOG1(L_ERROR, "CSG ID %d not in range", csgId);
               retVal = RFAILED;
            }
            if(servingCellPci < csgStartPci || 
                    servingCellPci > maxCsgPci)
            {
               RLOG0(L_ERROR, "Invalid PCI Configuration in closed access");
               RLOG4(L_ERROR, "eNB type %d PCI %d CSG start PCI %d "
                     "Max %d", wrSmDfltEnbType, servingCellPci, csgStartPci,
                     maxCsgPci);
               RLOG2(L_ERROR, "SmallCell start PCI %d Max %d",
                     smallCellStartPci, maxSmallCellPci);
               retVal = RFAILED;
            }
         }
      }
      break;
      case WR_ENB_HYBRID_ACCESS:
      /* Validate Max number of CSG and Non CSG UEs configured */
      maxCsgUes = lteeNodeBparams.lteFapAccessParams.maxCSGMembers;
      maxNonCsgUes = lteeNodeBparams.lteFapAccessParams.maxNonCSGMembers;
      minDlResNonCsg = lteeNodeBparams.lteAddCellCfg.minDlResNonCsg;
      minUlResNonCsg = lteeNodeBparams.lteAddCellCfg.minUlResNonCsg;
      if((maxCsgUes > smCfgCb.maxUeSupp) || 
         (maxNonCsgUes > smCfgCb.maxUeSupp) ||
         ((maxNonCsgUes == maxCsgUes) && 
          (maxCsgUes < smCfgCb.maxUeSupp) && 
          ((maxCsgUes + maxNonCsgUes) != smCfgCb.maxUeSupp)) ||
         ((maxCsgUes != maxNonCsgUes) &&
          ((maxCsgUes + maxNonCsgUes) != smCfgCb.maxUeSupp))) 
      {
            RLOG0(L_ERROR, "Invalid number of CSG or Non CSG UEs configured");
            RLOG3(L_ERROR, "Non CSG UEs configured %d, CSG UEs configured "
                  "%d, Max UEs Supported %d", maxNonCsgUes,
                  maxCsgUes, smCfgCb.maxUeSupp);
            RETVALUE(RFAILED);
      }
      /* Validate minimum number of UL and DL resources configured  */
      if(minDlResNonCsg > 100)
      {
         RLOG1(L_ERROR, "Invalid number of Minimum DL Resources %d",
                  minDlResNonCsg);
         RETVALUE(RFAILED);
      }
      if(minUlResNonCsg > 100)
      {
         RLOG1(L_ERROR, "Invalid number of Minimum UL Resources %d",
                  minUlResNonCsg);
         RETVALUE(RFAILED);
      }
      break;
      default:
      {
         RLOG1(L_ERROR, "Invalid Access Mode configuration %d",
               accessMode);
         retVal = RFAILED;
      }
      break;
   }
   RETVALUE(retVal);
} /* end of smWrValidateConfigParams */
/* CSG_DEV end */

/*
*
*       Fun:   MsmEnodeBinitialCfgComplete
*
*       Desc:  This function is to conform MIB is populated we can start cfg start layere
*
*       Ret:   None
*
*       Notes: <NONE>
*
*       File:  wr_smm_enbapp_cmnplatutils.c
*
**/

PUBLIC S16 MsmEnodeBinitialCfgComplete(Void)
{
   Pst    smPst;
   Buffer *mBuf;          /* message buffer */

   TRC2(MsmEnodeBinitialCfgComplete);

   RLOG0(L_ALWAYS," Received eNodeB initial configuration complete from OAM\n");
   SM_SET_ZERO(&smPst, sizeof(Pst));

   smPst.dstProcId = SM_SM_PROC;
   smPst.srcProcId = SM_SM_PROC;
   smPst.dstEnt = ENTSM;
   smPst.srcEnt = ENTWR;
   smPst.dstInst = 0;
   smPst.srcInst = 0;
   smPst.event  = EVTSMSTATICCFG;
   smPst.pool   = WR_POOL;
   smPst.selector= SM_SELECTOR_LC;


   if(SGetMsg(DFLT_REGION, WR_POOL, &mBuf) != ROK)
   {
      RLOG0(L_FATAL, "Memory allocation failed while creating mBuf");
      RETVALUE(RFAILED);
   } /* end of if statement */

   SPstTsk(&smPst, mBuf); 
   RETVALUE(ROK);
}/* end of MsmEnodeBinitialCfgComplete */

/*
*
*       Fun:   smStaticCfgReq
*
*       Desc:  This function is to conform MIB is populated we can start cfg start layere
*
*       Ret:   None
*
*       Notes: <NONE>
*
*       File:  wr_enb_cmnplt_enbapp.c
*
*/
PUBLIC S16 smStaticCfgReq(Void)
{
   U16 smEvent;
#ifdef TENB_AS_SECURITY
   U32 loopCnt = 0;
#endif
   CmTptAddr addr;

   TRC2(smStaticCfgReq);

#ifndef NL
   if(ROK != smWrValidateConfigParams())
   {
      RLOG0(L_ERROR, "Validation of configuration parameters failed");
      RETVALUE(RFAILED);
   }
#endif

   if(lteeNodeBparams.lteEnodebIpAddr.ipv6pres == TRUE)
   {

      addr.type = CM_TPTADDR_IPV6;
      cmMemcpy((U8*)addr.u.ipv6TptAddr.ipv6NetAddr,(U8*)lteeNodeBparams.lteEnodebIpAddr.enbIpv6Addr, sizeof(CmInetIpAddr6));
      wrCpyCmTptAddr(&smCfgCb.enbIpAddr[WR_IPV6_IDX], &addr);
      wrCpyCmTptAddr(&wrSmDfltEnbIpAddr[WR_IPV6_IDX], &addr);
      wrCpyCmTptAddr(&smCfgCb.sctpIpAddr[WR_IPV6_IDX], &addr);

   /* Do not fill enb ipv4 address : TBD */
   addr.type = 0;
   wrCpyCmTptAddr(&smCfgCb.enbIpAddr[WR_IPV4_IDX], &addr);
   wrCpyCmTptAddr(&wrSmDfltEnbIpAddr[WR_IPV4_IDX], &addr);
   wrCpyCmTptAddr(&smCfgCb.sctpIpAddr[WR_IPV4_IDX], &addr);
   }
   else if(lteeNodeBparams.lteEnodebIpAddr.ipv4pres == TRUE)
  {
   addr.type = CM_TPTADDR_IPV4;
   addr.u.ipv4TptAddr.address = lteeNodeBparams.lteEnodebIpAddr.enbIpAddr;

   wrCpyCmTptAddr(&smCfgCb.enbIpAddr[WR_IPV4_IDX], &addr);
   wrCpyCmTptAddr(&wrSmDfltEnbIpAddr[WR_IPV4_IDX], &addr);
   wrCpyCmTptAddr(&smCfgCb.sctpIpAddr[WR_IPV4_IDX], &addr);
   /* Do not fill enb ipv6 address : TBD */
   addr.type = 0;
   wrCpyCmTptAddr(&smCfgCb.enbIpAddr[WR_IPV6_IDX], &addr);
   wrCpyCmTptAddr(&wrSmDfltEnbIpAddr[WR_IPV6_IDX], &addr);
   wrCpyCmTptAddr(&smCfgCb.sctpIpAddr[WR_IPV6_IDX], &addr);
  }
   wrSmSctpPort       = lteeNodeBparams.lteEnbProtoCfg.sctpPortNumS1;
   smInitFreePeerIdList(); /*initialize free peerId list*/
   if(ROK != smInitNghHashList())
   {
      RLOG0(L_ERROR, "Initialisation of Neighbour hash lists failed");
      RETVALUE(RFAILED);
   }

   if(ROK != smInitMmeCfg())
   {
      RLOG0(L_ERROR, "Initialisation of MME configuration failed");
      RETVALUE(RFAILED);
   }

#ifdef TENB_AS_SECURITY
   for(loopCnt = 0; loopCnt < WR_MAX_AS_SEC_ALGOS; loopCnt++)
   {
      wreNodBPriIntgAlgo[loopCnt].pres = FALSE;
      wreNodBPriCiphAlgo[loopCnt].pres = FALSE;
   }
   for(loopCnt = 0; loopCnt < WR_MAX_AS_SEC_ALGOS; loopCnt++)
   {
      if((lteeNodeBparams.lteSmCellCfg.genUeCfg.intgAlgo[loopCnt] < WR_MAX_AS_SEC_ALGOS) && (lteeNodeBparams.lteSmCellCfg.genUeCfg.intgAlgo[loopCnt] >= 0))
      {
         wreNodBPriIntgAlgo[loopCnt].pres= TRUE;
         wreNodBPriIntgAlgo[loopCnt].val = lteeNodeBparams.lteSmCellCfg.genUeCfg.intgAlgo[loopCnt];
      }
   }
   for(loopCnt = 0; loopCnt < WR_MAX_AS_SEC_ALGOS; loopCnt++)
   {
      if((lteeNodeBparams.lteSmCellCfg.genUeCfg.ciphAlgo[loopCnt] < WR_MAX_AS_SEC_ALGOS) && (lteeNodeBparams.lteSmCellCfg.genUeCfg.ciphAlgo[loopCnt] >= 0))
      {
         wreNodBPriCiphAlgo[loopCnt].pres= TRUE;
         wreNodBPriCiphAlgo[loopCnt].val = lteeNodeBparams.lteSmCellCfg.genUeCfg.ciphAlgo[loopCnt];
      }
   }
#endif


   smCfgCb.maxUeSupp = lteeNodeBparams.lteSmCellCfg.maxUeSupp;
   if(smCfgCb.maxUeSupp > TENB_MAX_UE_SUPPORTED)
   {
      smCfgCb.maxUeSupp = TENB_MAX_UE_SUPPORTED;
   }
   wrSmDfltMaxNoUE = smCfgCb.maxUeSupp;

/* ccpu00135220 */
   smCfgCb.x2PrepTimerVal = lteeNodeBparams.lteProcTimerCfg.x2PrepTimerVal;
   smCfgCb.x2OvrAllTimerVal = lteeNodeBparams.lteProcTimerCfg.x2OvrAllTimerVal;
/* ccpu00135220 */

   smCfgCb.x2TimeToWaitTimerVal = lteeNodeBparams.lteX2ConfigParams.x2TimeToWaitTimerVal;
#ifdef LTE_HO_SUPPORT
   smCfgCb.rrmNumDedPrm = lteeNodeBparams.lteSmCellCfg.maxRrmPrmbls;
   smCfgCb.rrmDedPrmStart = lteeNodeBparams.lteSmCellCfg.rrmPrmblStart;
#endif  
   smCfgCb.lwrSctpParamCfg.heartBeatInterval = lteeNodeBparams.lteSctpCfgParams.heartBeatInterval/SCTP_TIME_FROM_MS_TO_SEC;
   smCfgCb.lwrSctpParamCfg.rtoInitial = lteeNodeBparams.lteSctpCfgParams.rtoInitial/SCTP_TIME_FROM_MS_TO_SEC;
   smCfgCb.lwrSctpParamCfg.rtoMin = lteeNodeBparams.lteSctpCfgParams.rtoMin/SCTP_TIME_FROM_MS_TO_SEC;
   smCfgCb.lwrSctpParamCfg.rtoMax = lteeNodeBparams.lteSctpCfgParams.rtoMax/SCTP_TIME_FROM_MS_TO_SEC;

   smCfgCb.maxExpires =  lteeNodeBparams.lteEnbProtoCfg.maxExpires;

   //smCfgCb.enbIpAddr = lteeNodeBparams.lteEnbProtoCfg.enbIpAddr;
   //smCfgCb.sctpIpAddr = lteeNodeBparams.lteEnbProtoCfg.enbIpAddr;

   if(ROK != smInitDbgCfg())
   {
      RLOG0(L_ERROR, "Initialisation of debug configuration failed");
      RETVALUE(RFAILED);
   }

#ifdef PHY_ERROR_LOGING
   smUlAllocInfoCb.mcs = lteeNodeBparams.lteUlAllocInfoCbParams.mcs;
   smUlAllocInfoCb.numOfRb = lteeNodeBparams.lteUlAllocInfoCbParams.numOfRb;
   smUlAllocInfoCb.rbStart = lteeNodeBparams.lteUlAllocInfoCbParams.rbStart;
   smUlAllocInfoCb.logTime = lteeNodeBparams.lteUlAllocInfoCbParams.logTime;
#endif

#ifndef LTE_ENB_PERF
   smEvent = EVTWRSONCFG;
#else /* if not defined LTE_ENB_PERF */
   smEvent = EVTWRAPPCFG;
#endif /* end of LTE_ENB_PERF */
   /* Invoke the state machine */
   smCb.smState = WR_SM_STATE_INIT;
   smCb.portNumber=lteeNodeBparams.lteGostLog.portNumber;
   smCb.rrcLogEnable=lteeNodeBparams.lteGostLog.rrcLogEnable;
   cmMemcpy((U8*)smCb.ipAddress,(U8*)lteeNodeBparams.lteGostLog.ipAddress, sizeof(lteeNodeBparams.lteGostLog.ipAddress));
  
   /* Register and Initiallize Timer */   

   if (smRegInitTmr() != ROK)
   {
      RLOG0(L_ERROR, "Registartion and initialisation of timers failed");
      RETVALUE(RFAILED);
   }

   cmInitTimers(&(smCb.cfgCb.guardCfgTmr), 1); /* Guard (Static/Dynamic Cfg) Timer  */

   smWrProcSm((U8)smEvent);

#ifdef TENB_STATS
   if (TSInfInitStatsFrmWork(lteeNodeBparams.tenbStatsCfg.tenbSocketStats,
            lteeNodeBparams.tenbStatsCfg.tenbFileStats,
            lteeNodeBparams.tenbStatsCfg.tenbConsoleStats,
            lteeNodeBparams.tenbStatsCfg.tenbStatsPeriodicity,
            lteeNodeBparams.tenbStatsCfg.tendStatsSocketPort) != ROK)
   {
      RLOG0(L_ERROR, "Wrong configuration of TENB STATS");
      RETVALUE(RFAILED);
   }
   TSInfSetEnbIp(lteeNodeBparams.lteEnodebIpAddr.enbIpAddr);
#endif

   RETVALUE(ROK);
}/*smStaticCfgReq*/
 
/*
*
*       Fun:   smPackSendAdminChange
*
*       Desc:  packing and sending dynamic configuration to SM
*
*       Ret:   ROK
*
*       Notes: None
*
*       File:  wr_smm_dyncfg.c
*
*/ 

PRIVATE S16 smPackStartStopStackReq(U8 isPerdREMReq,U8 isStackStart,U8 isImmediate)
{
   Pst    smPst;
   Buffer *mBuf;          /* message buffer */

   TRC2(smPackStartStopStackReq);

   SM_SET_ZERO(&smPst, sizeof(Pst));

   smPst.dstProcId = SM_SM_PROC;
   smPst.srcProcId = SM_SM_PROC;
   smPst.dstEnt = ENTSM;
   smPst.srcEnt = ENTWR;
   smPst.dstInst = 0;
   smPst.srcInst = 0;
   smPst.event  = EVTSTKSTRSTPREQ;
   smPst.pool   = WR_POOL;
   smPst.selector= SM_SELECTOR_LC;

   if(SGetMsg(DFLT_REGION, WR_POOL, &mBuf) != ROK)
   {
      RLOG0(L_FATAL, "Memory allocation failed while packing Admin change");
      RETVALUE(RFAILED);
   } /* end of if statement */

   SAddPstMsgMult((Data *)&(isImmediate), (MsgLen) sizeof(U8), mBuf);
   SAddPstMsgMult((Data *)&(isStackStart), (MsgLen) sizeof(U8), mBuf);
   SAddPstMsgMult((Data *)&(isPerdREMReq), (MsgLen) sizeof(U8), mBuf);

   RETVALUE(SPstTsk(&smPst, mBuf));
} /* end of smPackSendAdminChange */
 
/*
*
*       Fun:   MsmStartStopStackReq.
*
*       Desc:  While stack is running anf HeMS changed the admin state 
*              this function will handle the eNodeB state
*
*       Ret:   ROK
*
*       Notes: 
*              If true Unlocked with permission to serve traffic (and enable RF transmitter(s)).
*              If false Locked. Transition FAP to state where it is NOT permitted to serve traffic. RF transmitter is disabled.
*
*       File:  wr_sm_wr_cntrl.c
*
*/
PUBLIC Void MsmStartStopStackReq(U8 isPerdREMReq,U8 isStackStart,U8 isImmediate)
{
   TRC2(MsmStartStopStackReq);

   smPackStartStopStackReq(isPerdREMReq,isStackStart,isImmediate);

   RETVOID;
} /*end of MsmAdminStateChanged*/
 
/*
*
*       Fun:   smStartStopStack.
*
*       Desc:  While stack is running anf HeMS changed the admin state 
*              this function will handle the eNodeB state
*
*       Ret:   ROK
*
*       Notes: 
*              If true Unlocked with permission to serve traffic (and enable RF transmitter(s)).
*              If false Locked. Transition FAP to state where it is NOT permitted to serve traffic. RF transmitter is disabled.
*
*       File:  wr_smm_enbapp_cmnplatutils.c
*
*/
PUBLIC S16 smStartStopStack(U8 isPerdREMReq,U8 isStackStart,U8 isImmediate)
{
   TRC2(smStartStopStack);
 
#ifdef LTE_QCOM 
   S16 ret;
   /* Updating admin state to be sent to Qualcomm OAM Server */
    lteeNodeBparams.lteRsysQcomCfgParam.adminState = isStackStart;

   /* If OAM Dynamic configuration has started then construct record dynamicall and update in the Qualcomm server */
    RLOG1(L_ALWAYS, "AdminState changed dynamically %d",lteeNodeBparams.lteRsysQcomCfgParam.adminState);
    ret = wrQcomRfCarrierCommonDynamicCfgReq(isStackStart); 

    if ( ret == RFAILED)
    {
       smStaticCfgFailAlarm();
    }
#endif
   /*  CmTptAddr addr; */
   if(TRUE == isStackStart)
   {
      if(lteeNodeBparams.lteEnodebIpAddr.ipv4pres == TRUE)
      {
         smCfgCb.enbIpAddr[0].type = CM_TPTADDR_IPV4;
         smCfgCb.enbIpAddr[0].u.ipv4TptAddr.address = lteeNodeBparams.lteEnodebIpAddr.enbIpAddr;
         wrSmDfltEnbIpAddr[0].type = CM_TPTADDR_IPV4;
         wrSmDfltEnbIpAddr[0].u.ipv4TptAddr.address = lteeNodeBparams.lteEnodebIpAddr.enbIpAddr;
         smCfgCb.sctpIpAddr[0].type = CM_TPTADDR_IPV4;
         smCfgCb.sctpIpAddr[0].u.ipv4TptAddr.address = lteeNodeBparams.lteEnodebIpAddr.enbIpAddr;

         smCb.smState = WR_SM_STATE_INIT;
         if (smCb.reCfg == TRUE)
         {
            wrCpyCmTptAddr(&wrEmmCb.datAppAddr[WR_IPV4_IDX], &smCfgCb.enbIpAddr[WR_IPV4_IDX]);
         }
      }
      if(lteeNodeBparams.lteEnodebIpAddr.ipv6pres == TRUE)
      {
         CmTptAddr addr;
         addr.type = CM_TPTADDR_IPV6;
         cmMemcpy((U8*)addr.u.ipv6TptAddr.ipv6NetAddr,(U8*)lteeNodeBparams.lteEnodebIpAddr.enbIpv6Addr, sizeof(CmInetIpAddr6));
         wrCpyCmTptAddr(&smCfgCb.enbIpAddr[WR_IPV6_IDX], &addr);
         wrCpyCmTptAddr(&wrSmDfltEnbIpAddr[WR_IPV6_IDX], &addr);
         wrCpyCmTptAddr(&smCfgCb.sctpIpAddr[WR_IPV6_IDX], &addr);

         smCb.smState = WR_SM_STATE_INIT;
         if (smCb.reCfg == TRUE)
         {
            wrCpyCmTptAddr(&wrEmmCb.datAppAddr[WR_IPV6_IDX], &smCfgCb.enbIpAddr[WR_IPV6_IDX]);
         }

      }

      if(ROK != smInitMmeCfg())
      {
         RLOG0(L_ERROR, "Initialisation of MME configuration failed");
         RETVALUE(RFAILED);
      }
      smInitNghHashList();
      smWrProcSm(EVTWRSONCFG);
      sendKick = TRUE;
   }
   else
   {
#ifdef TENB_T2K3K_SPECIFIC_CHANGES
      if(smCb.isPciMod == TRUE)
#endif
      {
         smCb.reCfg = TRUE;
         smCb.syncState = FALSE; 
         SmDeInitNghHashList();
      }
      if( (isPerdREMReq == TRUE) &&(isImmediate == TRUE))
      {
         smCb.prem = TRUE;
         wrWatchdogSendRemovePid(0); 
         smBuildShutRestart();
      }
      else if ((isPerdREMReq == TRUE) && (isImmediate == FALSE))
      {
         smCb.prem = TRUE;
      }
      else
      {
         wrWatchdogSendRemovePid(0);
         smBuildShutRestart();
      }
      
   }
   RETVALUE(ROK);
} /*end of MsmAdminStateChanged*/

/*
*
*       Fun:   smBuildCdmaHrpdaBandClass
*
*       Desc:  This function for filling the Cell Band Class Configuration
*
*       Ret:   ROK
*
*       Notes: None
*              
*       File:  wr_sm_wr_cntrl.c 
*
*/
#ifdef ANSI
PUBLIC S16 smBuildCdmaHrpdaBandClass
(
LwrMngmt *lwrMng
)
#else /* if ANSI not defined*/
PUBLIC S16 smBuildCdmaHrpdaBandClass(*lwrMng)
LwrMngmt *lwrMng
#endif /* end of ANSI*/
{
   RETVALUE(ROK);
} /* end of smBuildCdmaHrpdaBandClass */


/*
*
*       Fun:   smBuildWrEnbCfg
*
*       Desc:  This function for filling eNodeB Configuration
*
*       Ret:   ROK
*
*       Notes: None
*              
*       File:  wr_sm_wr_cntrl.c 
*
*/
PUBLIC S16 smBuildWrEnbCfg
(
LwrMngmt *lwrMng
)
{
   LwrENodeBCfg *enbCfg = NULLP;
   TknStrOSXL   *enbName;
   MsmLtePlmnId *priPlmn;
   MsmLtePlmnId *plmn;
   U32          idx = 0;
   U32          idx1 = 1;
   U32          itr = 0;
   
   TRC2(smBuildWrEnbCfg);

   lwrMng->t.protoCfg.type = WR_CFG_TYPE_ENB;
   lwrMng->t.protoCfg.t.eNbCfg.action = WR_ACT_ADD;
   
   enbCfg =&(lwrMng->t.protoCfg.t.eNbCfg.t.eNodeBCfg);

   /* add enb name */
   enbName = &(enbCfg->protoCfg.enbName);
   enbName->len = (U16)(strlen((S8 *)&(lteeNodeBparams.lteEnbProtoCfg.henbName)));
   WR_ALLOC(&(enbName->val), enbName->len);
   if(NULLP == enbName->val)
   {
      RLOG0(L_FATAL, "Memory allocation failed while allocating eNBName");
      RETVALUE(RFAILED);
   }
   cmMemcpy(enbName->val, lteeNodeBparams.lteEnbProtoCfg.henbName, enbName->len);

   enbCfg->maxNumOfUe = wrSmDfltMaxNoUE;
   if (MSM_LWR_AP_CNT_1 == lteeNodeBparams.lteAntenna_Common_Config.numAntPorts)
   {
      enbCfg->numTxAntenna = 1;
   }
   else if(MSM_LWR_AP_CNT_2 == lteeNodeBparams.lteAntenna_Common_Config.numAntPorts)
   {
      enbCfg->numTxAntenna = 2;
   }
   else if(MSM_LWR_AP_CNT_4 == lteeNodeBparams.lteAntenna_Common_Config.numAntPorts)
   {
      enbCfg->numTxAntenna = 4;
   }
   enbCfg->enbCfgTransTmrVal = wrSmDfltEnbCfgTransTmrVal;
   enbCfg->pwsEtwsCmas = (U8)lteeNodeBparams.lteEnbProtoCfg.pwsEtwsCmas; /* Enable ETWS or CMAS or disable both*/
   enbCfg->avgFreeCpuRqd = (U8)lteeNodeBparams.lteEnbProtoCfg.avgFreeCpuRqd; /* Configure free cpu req*/
   if(WR_ENODEB_TYPE_HOME == wrSmDfltEnbType)
   {
      enbCfg->protoCfg.enbId = lteeNodeBparams.lteCellSib1CfgGrp.cellAccessInfo.cellId;
     /* in home enb case eNodeB Id and cellId are same wrSmDfltEnbID; */
   }
   else
   {
      /* in macro enb case eNodeB Id is MSB 20 bits of cellId */
      enbCfg->protoCfg.enbId = ((lteeNodeBparams.lteCellSib1CfgGrp.cellAccessInfo.cellId) >> 8);
   }
   enbCfg->protoCfg.enbType = wrSmDfltEnbType;
   RLOG2(L_DEBUG, "enbCfg->protoCfg.enbType : %lu,enbCfg->protoCfg.enbId : %lu",
      enbCfg->protoCfg.enbType,enbCfg->protoCfg.enbId);
  
  if(lteeNodeBparams.lteEnodebIpAddr.ipv4pres == TRUE)
  {   
   enbCfg->protoCfg.enbIpAddr[WR_IPV4_IDX].type = CM_TPTADDR_IPV4;
   enbCfg->protoCfg.enbIpAddr[WR_IPV4_IDX].u.ipv4TptAddr.address = lteeNodeBparams.lteEnodebIpAddr.enbIpAddr;

   enbCfg->protoCfg.enbIpAddr[WR_IPV4_IDX].u.ipv4TptAddr.port = 
       lteeNodeBparams.lteEnbProtoCfg.sctpPortNumS1;
   }

  if(lteeNodeBparams.lteEnodebIpAddr.ipv6pres == TRUE)
  {
     enbCfg->protoCfg.enbIpAddr[WR_IPV6_IDX].type = CM_TPTADDR_IPV6;
     cmMemcpy((U8*)enbCfg->protoCfg.enbIpAddr[WR_IPV6_IDX].u.ipv6TptAddr.ipv6NetAddr,(U8*)lteeNodeBparams.lteEnodebIpAddr.enbIpv6Addr, sizeof(CmInetIpAddr6));

     enbCfg->protoCfg.enbIpAddr[WR_IPV6_IDX].u.ipv6TptAddr.port = lteeNodeBparams.lteEnbProtoCfg.sctpPortNumS1;
   }

   enbCfg->protoCfg.numPlmns = lteeNodeBparams.lteCellSib1CfgGrp.cellAccessInfo.numOfPlmns;

   for(idx =0; idx < lteeNodeBparams.lteCellSib1CfgGrp.cellAccessInfo.numOfPlmns; idx++ )
   {
      if(lteeNodeBparams.lteCellSib1CfgGrp.cellAccessInfo.plmnInfo[idx].isPrimary == TRUE)
      {
         priPlmn = &lteeNodeBparams.lteCellSib1CfgGrp.cellAccessInfo.plmnInfo[idx].plmnId;
         enbCfg->protoCfg.plmns[0].plmnId.numMncDigits  = priPlmn->numMncDigits;
         enbCfg->protoCfg.plmns[0].plmnId.mcc[0] =priPlmn->mcc[0];
         enbCfg->protoCfg.plmns[0].plmnId.mcc[1] =priPlmn->mcc[1];
         enbCfg->protoCfg.plmns[0].plmnId.mcc[2] =priPlmn->mcc[2];
      
         enbCfg->protoCfg.plmns[0].plmnId.mnc[0]=priPlmn->mnc[0];
         enbCfg->protoCfg.plmns[0].plmnId.mnc[1]=priPlmn->mnc[1];
         enbCfg->protoCfg.plmns[0].plmnId.mnc[2]=priPlmn->mnc[2];
         enbCfg->protoCfg.plmns[0].plmnRatPriority.numberOfRats = lteeNodeBparams.lteEnbProtoCfg.plmnRatPriority[idx].numberOfRats;
         for(itr = 0; itr < enbCfg->protoCfg.plmns[0].plmnRatPriority.numberOfRats; itr++)
         {
            enbCfg->protoCfg.plmns[0].plmnRatPriority.rat[itr] = lteeNodeBparams.lteEnbProtoCfg.plmnRatPriority[idx].rat[itr];
         }

         enbCfg->protoCfg.plmns[0].isRedirSupport  = lteeNodeBparams.lteEnbProtoCfg.isRedirectionSupport[idx];
      }
      else
      {
         plmn = &lteeNodeBparams.lteCellSib1CfgGrp.cellAccessInfo.plmnInfo[idx].plmnId;
         enbCfg->protoCfg.plmns[idx1].plmnId.numMncDigits  = plmn->numMncDigits;
         enbCfg->protoCfg.plmns[idx1].plmnId.mcc[0] =plmn->mcc[0];
         enbCfg->protoCfg.plmns[idx1].plmnId.mcc[1] =plmn->mcc[1];
         enbCfg->protoCfg.plmns[idx1].plmnId.mcc[2] =plmn->mcc[2];
      
         enbCfg->protoCfg.plmns[idx1].plmnId.mnc[0]=plmn->mnc[0];
         enbCfg->protoCfg.plmns[idx1].plmnId.mnc[1]=plmn->mnc[1];
         enbCfg->protoCfg.plmns[idx1].plmnId.mnc[2]=plmn->mnc[2];
         enbCfg->protoCfg.plmns[idx1].plmnRatPriority.numberOfRats = lteeNodeBparams.lteEnbProtoCfg.plmnRatPriority[idx].numberOfRats;
         for(itr = 0; itr < enbCfg->protoCfg.plmns[idx1].plmnRatPriority.numberOfRats; itr++)
         {
            enbCfg->protoCfg.plmns[idx1].plmnRatPriority.rat[itr] = lteeNodeBparams.lteEnbProtoCfg.plmnRatPriority[idx].rat[itr];
         }

         enbCfg->protoCfg.plmns[idx1].isRedirSupport  = lteeNodeBparams.lteEnbProtoCfg.isRedirectionSupport[idx];
         idx1++;
      }
   }


#ifdef MME_LDBAL
   enbCfg->numOfMmePriGrps                 = wrSmDfltNumOfPriorityGrps;
#endif /* end of MME_LDBAL */

    enbCfg->macMainCfg.timeAlignmentTimerDedicated = lteeNodeBparams.macMainConfig.timeAlignmentTimerDedicated; 
    enbCfg->macMainCfg.ulSchCfg.maxHARQ_Tx = lteeNodeBparams.lteMAC_SCH_Config.maxHARQ_Tx;
    enbCfg->macMainCfg.ulSchCfg.periodicBSR_Timer = lteeNodeBparams.lteMAC_SCH_Config.periodicBSR_Timer;
    enbCfg->macMainCfg.ulSchCfg.retxBSR_Timer = lteeNodeBparams.lteMAC_SCH_Config.retxBSR_Timer;
    enbCfg->macMainCfg.ulSchCfg.ttiBundling = lteeNodeBparams.lteMAC_SCH_Config.ttiBundling;
    enbCfg->macMainCfg.ulSchCfg.deltaHqOffset = wrSmDfltDeltaHqOffst;
    enbCfg->macMainCfg.phr_Config.periodicPHR_Timer = wrSmDfltMacCfgPeriodicPHR_Timer;
    enbCfg->macMainCfg.phr_Config.prohibitPHR_Timer = wrSmDfltMacCfgProhibitPHRTimer;
    enbCfg->macMainCfg.phr_Config.dl_PathlossChange = wrSmDfltMacCfgDlPathlossChange; 
   
    enbCfg->pdcpInfo.umSnSize = NHU_PDCP_SN_SIZE_7BITS; 
    enbCfg->pdcpInfo.statRprtReqrd = wrSmDfltStatusReportReqd;
    enbCfg->pdcpInfo.discardTimer = wrSmDfltDiscardTimer;

    enbCfg->schReqCfg.srPucchRsrcIdx = lteeNodeBparams.lteCQIPeriodicReportCfg.cqi_PUCCH_ResourceIdx; 
    enbCfg->schReqCfg.dsrTransMax = wrSmDfltDsrTransMax;

    enbCfg->puschDedicated.ackIdx = wrSmDfltBetaOffACKIdx;
    enbCfg->puschDedicated.riIdx = wrSmDfltBetaOffRIIdx;
    enbCfg->puschDedicated.cqiIdx = wrSmDfltBetaOffCQIIdx;
    enbCfg->puschDedicated.puschPwrCfg.pres = lteeNodeBparams.ltecellSchdPwrCfg.isUlGrpPwrCntrlPuschEnable;
    enbCfg->puschDedicated.puschPwrCfg.isFmt3a = lteeNodeBparams.ltecellSchdPwrCfg.puschPwrFmt3aEnable;

    enbCfg->pucchPwrCfg.pres = lteeNodeBparams.ltecellSchdPwrCfg.isUlGrpPwrCntrlPucchEnable;
    enbCfg->pucchPwrCfg.isFmt3a = lteeNodeBparams.ltecellSchdPwrCfg.pucchPwrFmt3aEnable;

    enbCfg->pdschCfgDedicated.pA = lteeNodeBparams.ltePdschCfgDedicated.pA[0];

    enbCfg->protoCfg.sctpPortNumS1 = lteeNodeBparams.lteEnbProtoCfg.sctpPortNumS1;
    enbCfg->protoCfg.sctpPortNumX2 = lteeNodeBparams.lteEnbProtoCfg.sctpPortNumX2;
    enbCfg->qosCfg.ueBr = wrSmDfltUeBr;
    enbCfg->cqiRptCfg.aperCqiEnbld = FALSE;
    enbCfg->cqiRptCfg.cqiAperiodicMode = wrSmDfltCqiRprtModeAperiodic;
    enbCfg->cqiRptCfg.nomPdschRsEpreOff = (S8)wrSmDfltCqiNomPDSCHRSEPREoff;
    enbCfg->cqiRptCfg.periodRptSetupPres = FALSE;
    enbCfg->cqiRptCfg.cqiPucchRsrcIdx = lteeNodeBparams.lteCQIPeriodicReportCfg.cqi_PUCCH_ResourceIdx;
    enbCfg->cqiRptCfg.cqipmiCfgIdx = WR_UMM_CQI_PMI_CONFIG_IDX;
    enbCfg->cqiRptCfg.subbndCqik = lteeNodeBparams.lteCQIPeriodicReportCfg.subBandCQI.k;
    enbCfg->cqiRptCfg.fmtInd = WR_UE_CQI_FMT_WB;/*129311*/
    enbCfg->cqiRptCfg.riCfgIdx = 0;
    enbCfg->cqiRptCfg.simulAckNackAndCQI = wrSmDfltSimultAckNackAndCQI;
    enbCfg->cqiRptCfg.periodicmode = wrSmDfltPrdModeEnum;
    /*ccpu00129641*/
    enbCfg->cqiRptCfg.riPres = FALSE;
    enbCfg->x2ConFlg =  lteeNodeBparams.lteNeighCellCfg.isX2Enabled;
    /* CNM_DEV */
    enbCfg->isCnmEnabled = lteeNodeBparams.lteCnmCfg.cnmEnable;
    enbCfg->csfbToGeranCfg.isGERANMeasAllowedForCCO = 
            lteeNodeBparams.csfbToGeranCfg.isGERANMeasAllowedForCCO;
    enbCfg->csfbToGeranCfg.isCCOAllowed = 
            lteeNodeBparams.csfbToGeranCfg.isCCOAllowed;
    enbCfg->csfbToGeranCfg.rrcDatCfmWaitTmr = 
            lteeNodeBparams.csfbToGeranCfg.rrcDatCfmWaitTmr;
    enbCfg->csfbToGeranCfg.GERANMeasWaitTmr = 
            lteeNodeBparams.csfbToGeranCfg.GERANMeasWaitTmr;
    enbCfg->csfbToGeranCfg.T304expWaitTmrVal = 
            lteeNodeBparams.csfbToGeranCfg.T304expWaitTmrVal;

   RETVALUE(ROK);
} /* smBuildWrEnbCfg */

/*
*
*       Fun:   smBuildCdma1xRTTBandClass
*
*       Desc:  This function for filling the Cell Band Class Configuration
*
*       Ret:   ROK
*
*       Notes: None
*              
*       File:  wr_sm_wr_cntrl.c 
*
*/
#ifdef ANSI
PUBLIC S16 smBuildCdma1xRTTBandClass
(
LwrMngmt *lwrMng
)
#else /* if ANSI not defined*/
PUBLIC S16 smBuildCdma1xRTTBandClass(*lwrMng)
LwrMngmt *lwrMng
#endif /* end of ANSI*/
{
   LwrNeighCdmaBandClassCfg       *cdma1xNeghBand = NULLP;
   LwrNeighCdmaBandClassCfg       *nrNeghBand = NULLP;
   SmNeigCdma1xBandList           *cdma1xBandList = NULLP; 
   PTR                            prevEntry = NULLP;
   U32                            numBandAdded = 0;
   S16                            retVal = ROK;

   TRC2(smBuildWrNr1xCdmaBandCfg);

   lwrMng->t.protoCfg.type = WR_CFG_TYPE_BAND_CLASS;
   lwrMng->t.protoCfg.t.bandClassCfg.action = WR_ACT_ADD;
   lwrMng->t.protoCfg.t.bandClassCfg.cellId = 1; /* this is ownBandID in case of PLD, jst a logical entity */
  
   while((retVal = cmHashListGetNext(&(smCfgCb.neighCdma1xBandLstCp), prevEntry, (PTR *) &cdma1xBandList)) == ROK)
   {
     cdma1xNeghBand  = &cdma1xBandList->nghCdma1xBandCfg;
     lwrMng->t.protoCfg.t.bandClassCfg.t.bcAddCfg[numBandAdded].bandType = 0;//WR_NEIGH_BAND_TYPE_CDMA;
     nrNeghBand =&(lwrMng->t.protoCfg.t.bandClassCfg.t.bcAddCfg[numBandAdded].t.bandClass1xRtt);

     *nrNeghBand = *cdma1xNeghBand;
     cdma1xBandList->dynCfgInd = WR_CFG_DONE;  
     prevEntry = (PTR)cdma1xBandList;
     numBandAdded++;
   }
   lwrMng->t.protoCfg.t.bandClassCfg.numBandClass = numBandAdded; 
   RETVALUE(ROK);
} /* end of smBuildCdma1xRTTBandClass */
 
/*
*
*       Fun:   smBuildModCellCfg
*
*       Desc:  fill in cellCfg for WR
*
*       Ret:   ROK
*
*       Notes: None
*              
*       File:  wr_sm_wr_cntrl.c 
*/
#ifdef ANSI
PUBLIC S16 smBuildModCellCfg
(
 LwrMngmt *lwrMng 
)
#else /* if ANSI not defined*/
PUBLIC S16 smBuildModCellCfg(lwrMng)
LwrMngmt lwrMng
#endif /* end of ANSI*/
{
   LwrCellMibCfgGrp *lwrMib = NULLP;
   LwrCellSib1CfgGrp *sib1 =NULLP;
   U8                cfgGrpIndex = 0;

   TRC2(smBuildModCellCfg);

   lwrMng->t.protoCfg.type = WR_CFG_TYPE_CELL;
   lwrMng->t.protoCfg.t.cellCfg.action = WR_ACT_MOD;

   lwrMng->t.protoCfg.t.cellCfg.t.modCell.cellId = 1; /* this ownCellID in case of PLD based, jst logic cell */
   lwrMng->t.protoCfg.t.cellCfg.t.modCell.cellIndex = WR_DFLT_CELL_IDX;
   
   /*Filling MIB params*/
      WR_ALLOC(&(lwrMng->t.protoCfg.t.cellCfg.t.modCell.cellCfgGrp[cfgGrpIndex]), \
            (sizeof(LwrCellCfgGrp)));
      if(NULLP == lwrMng->t.protoCfg.t.cellCfg.t.modCell.cellCfgGrp[cfgGrpIndex])
      {
         RLOG0(L_FATAL, "Memory allocation failed. for cell configuration ");
         RETVALUE(RFAILED);
      }
   lwrMng->t.protoCfg.t.cellCfg.t.modCell.cellCfgGrp[cfgGrpIndex]->grpType = WR_CELL_CFG_GRP_SIB1;
   lwrMib = &(lwrMng->t.protoCfg.t.cellCfg.t.modCell.cellCfgGrp[cfgGrpIndex]->t.mibCfgGrp);
   lwrMng->t.protoCfg.t.cellCfg.t.modCell.reCfgType = LWR_CELL_CFG_GRP;  
   lwrMib->dlBw = wrSmDfltCellDlBW;
   lwrMib->phichCfg.phichDur = wrSmDfltPhichDur;
   lwrMib->phichCfg.phichRes = wrSmDfltPhichRes;
   cfgGrpIndex++;
   /*Filling sib1 params*/
   lwrMng->t.protoCfg.t.cellCfg.t.modCell.cellCfgGrp[cfgGrpIndex]->grpType 
                                                        = WR_CELL_CFG_GRP_SIB1;
                                                          
   sib1 =&(lwrMng->t.protoCfg.t.cellCfg.t.modCell.cellCfgGrp[cfgGrpIndex]->t.sib1CfgGrp);

   sib1->reCfgType = LWR_CELL_SIB1_CFG_CELL_ACCESS_INFO;

   sib1->cellAccessInfo.plmnInfo[0].rsrvdFrOperUse =wrSmDfltCellRsrvdForOperUse;

   /* Modify tac */
   sib1->cellAccessInfo.tac = (U16)(wrSmDfltTac + 1);

   /*sib1->cellAccessInfo.cellId = wrSmDfltCellId;*/ /* wrSmDfltCellIdentity */
   sib1->cellAccessInfo.cellId = lteeNodeBparams.lteCellSib1CfgGrp.cellAccessInfo.cellId; /* wrSmDfltCellIdentity */
   /* CSG_DEV start */
   if(WR_ENB_OPEN_ACCESS == lteeNodeBparams.lteFapAccessParams.accessMode)
   {
      sib1->cellAccessInfo.csgIndication = FALSE;
      sib1->cellAccessInfo.csgIdentity.pres = NOTPRSNT;
   }
   else if(WR_ENB_CLOSED_ACCESS == lteeNodeBparams.lteFapAccessParams.accessMode)
   {
      sib1->cellAccessInfo.csgIndication = TRUE;
      wrFillTknU32(&sib1->cellAccessInfo.csgIdentity,
            lteeNodeBparams.lteCellSib1CfgGrp.cellAccessInfo.csgIdentity);
   }
   else /* hybrid access */
   {
      sib1->cellAccessInfo.csgIndication = FALSE;
      wrFillTknU32(&sib1->cellAccessInfo.csgIdentity,
            lteeNodeBparams.lteCellSib1CfgGrp.cellAccessInfo.csgIdentity);
   }
   /* CSG_DEV end */
   sib1->cellAccessInfo.cellBarred = (Bool)wrSmDfltCellBarred;
   sib1->cellAccessInfo.intraFreqReSelection = (Bool)wrSmDfltIntraFreqResel;

   sib1->cellSelectInfo.qRxLevMin = (S8)wrSmDlftqRxLevMin;
   /* Start - Fix for ccpu00123651 */
   wrFillTknU8(&(sib1->cellSelectInfo.qRxLevMinOffset), (U8)wrSmDfltqRxlevminoffset);
   /* End - Fix for ccpu00123651 */

   sib1->pMax = (S8)wrSmDfltPMax;
   sib1->siWindowSize = wrSmDfltSiWinSize;
   
   /*TODO:*/
#ifdef LTE_TDD
   sib1->tddCfg.specialSubframeCfg = lteeNodeBparams.lteTddParam.specialSubFramePattern;
   sib1->tddCfg.subframeCfg = lteeNodeBparams.lteTddParam.subframeAssignment;
#else
   sib1->tddCfg.specialSubframeCfg = 0;
   sib1->tddCfg.subframeCfg = 0;
#endif
   cfgGrpIndex++;

   lwrMng->t.protoCfg.t.cellCfg.t.modCell.numOfGrps = cfgGrpIndex;

   RETVALUE(ROK);
} /* end of smBuildModCellCfg */

/*
*
*       Fun:   smBuildWrNehUtraFddCellCfg
*
*       Desc:  This function for filling neighbour cell configuration
*
*       Ret:   ROK
*
*       Notes: None
*              
*       File:  wr_smm_enbapp_cmnplatutils.c 
*
*/
#ifdef ANSI
PUBLIC S16 smBuildWrNehUtraFddCellCfg
(
LwrMngmt                     *lwrMng
)
#else /* if ANSI not defined*/
PUBLIC S16 smBuildWrNehUtraFddCellCfg(*lwrMng)
LwrMngmt                     *lwrMng;
#endif /* end of ANSI*/
{
   LwrUtraNeighCell          *utraNeghCell = NULLP;
   LwrUtraNeighCell   *nrCellCfg =NULLP;
   SmNeigUtraCellList *utraCellList = NULLP;
   PTR               prevEntry = NULLP;
   U8                utraFddIdx = 0;
   S16               retVal = ROK; 

   TRC2(smBuildWrNehUtraFddCellCfg);

   lwrMng->t.protoCfg.type = WR_CFG_TYPE_NEIGH_CELL;
   lwrMng->t.protoCfg.t.neighCellCfg.action = WR_ACT_ADD;
   lwrMng->t.protoCfg.t.neighCellCfg.cellId = 1;  /* this is ownCellID in case of PLD based conf, jst a logic entity */

   while((retVal = cmHashListGetNext(&(smCfgCb.neighUtraCellLstCp), prevEntry, (PTR *)&utraCellList)) == ROK) 
   {
      lwrMng->t.protoCfg.t.neighCellCfg.t.neighAddCfg[utraFddIdx].type = WR_NEIGH_CELL_UTRA_FDD;
      utraNeghCell = &utraCellList->nghUtraCellCfg;
      nrCellCfg =&lwrMng->t.protoCfg.t.neighCellCfg.t.neighAddCfg[utraFddIdx].t.utranCell;

      *nrCellCfg = *utraNeghCell;
      utraCellList->dynCfgInd = WR_CFG_DONE;
      prevEntry = (PTR )utraCellList;
      (utraFddIdx)++;
   }

   lwrMng->t.protoCfg.t.neighCellCfg.numCells = utraFddIdx;
   RETVALUE(ROK);
} /* end of smBuildWrNehUtraFddCellCfg */
 
/*
*
*       Fun:   smBuildWrNehUtraTddCellCfg
*
*       Desc:  This function for filling neighbour Utra Tdd cell configuration
*
*       Ret:   ROK
*
*       Notes: None
*              
*       File:  wr_smm_enbapp_cmnplatutils.c 
*
*/
#ifdef ANSI
PUBLIC S16 smBuildWrNehUtraTddCellCfg
(
LwrMngmt                     *lwrMng
)
#else /* if ANSI not defined*/
PUBLIC S16 smBuildWrNehUtraTddCellCfg(*lwrMng)
LwrMngmt                     *lwrMng;
#endif /* end of ANSI*/
{
   LwrUtraNeighCell      *utraNeghCell = NULLP;
   LwrUtraNeighCell      *nrCellCfg =NULLP;
   SmNeigUtraTddCellList *utraCellList = NULLP;
   PTR                   prevEntry = NULLP;
   U8                    utraTddIdx = 0;
   S16                   retVal = ROK; 

   TRC2(smBuildWrNehUtraTddCellCfg);

   lwrMng->t.protoCfg.type = WR_CFG_TYPE_NEIGH_CELL;
   lwrMng->t.protoCfg.t.neighCellCfg.action = WR_ACT_ADD;
   lwrMng->t.protoCfg.t.neighCellCfg.cellId = 1;  /* this is ownCellID in case of PLD based conf, jst a logic entity */

   while((retVal = cmHashListGetNext(&(smCfgCb.neighUtraTddCellLstCp), prevEntry, (PTR *)&utraCellList)) == ROK) 
   {
      lwrMng->t.protoCfg.t.neighCellCfg.t.neighAddCfg[utraTddIdx].type = WR_NEIGH_CELL_UTRA_TDD;
      utraNeghCell = &utraCellList->nghUtraTddCellCfg;
      nrCellCfg =&lwrMng->t.protoCfg.t.neighCellCfg.t.neighAddCfg[utraTddIdx].t.utranCell;

      *nrCellCfg = *utraNeghCell;
      utraCellList->dynCfgInd = WR_CFG_DONE;
      prevEntry = (PTR )utraCellList;
      (utraTddIdx)++;
   }

   lwrMng->t.protoCfg.t.neighCellCfg.numCells = utraTddIdx;
   RETVALUE(ROK);
} /* end of smBuildWrNehUtraTddCellCfg */
 
 /*
 *
 *       Fun:  smBuildWrNrCdmaHrpdFreqCfg- Build Neighbour frequency
 *
 *       Desc:   
 *
 *       Ret:    Void
 *
 *       Notes:  None
 *
 *      File:    wr_smm_enbapp.c
 *
 */

PUBLIC S16 smBuildWrNrCdmaHrpdFreqCfg
(
 LwrMngmt *lwrMng
)
{
   LwrNeighFreqAddModCfg *nrFreqCfg =NULLP;

   TRC2(smBuildWrNrCdmaHrpdFreqCfg);

   lwrMng->t.protoCfg.type = WR_CFG_TYPE_NEIGH_FREQ;
   lwrMng->t.protoCfg.t.neighFreqCfg.action = WR_ACT_ADD;
   lwrMng->t.protoCfg.t.neighFreqCfg.cellId = 1;  /* this is ownCellID in case of PLD based conf, jst a logic entity */
   lwrMng->t.protoCfg.t.neighFreqCfg.numFreq = 1;/* we are jst filling one frequency */
   nrFreqCfg =lwrMng->t.protoCfg.t.neighFreqCfg.t.addNeighFreq;
   nrFreqCfg->type = WR_NEIGH_CELL_CDMA2K_HRPD;
   nrFreqCfg->t.cdmaHrpdFreq.reCfgType = (U8)wrSmDfltReCfgType;
   nrFreqCfg->t.cdmaHrpdFreq.status = wrSmDfltStatus;
   nrFreqCfg->t.cdmaHrpdFreq.bandClass = wrSmDfltBandClass;
   nrFreqCfg->t.cdmaHrpdFreq.arfcn = wrSmDfltArfcn;
   nrFreqCfg->t.cdmaHrpdFreq.searchRate = wrSmDfltSearchRate;
   nrFreqCfg->t.cdmaHrpdFreq.offsetFreq = wrSmDfltOffsetFreq;
   RETVALUE(ROK);
} /* end of smBuildWrNrCdmaHrpdFreqCfg */
 
/*
 *
 *       Fun: smBuildWrNr1xCdmaFreqCfg- Build Neighbour frequency
 *
 *       Desc:   
 *
 *       Ret:    Void
 *
 *       Notes:  None
 *
 *      File:    wr_smm_enbapp_cmnplatutils.c
 *
 */

PUBLIC S16 smBuildWrNr1xCdmaFreqCfg
(
 LwrMngmt *lwrMng
)
{
   LwrNeighCdma1xFreq     *nrFreqCfg =NULLP;
   LwrNeighCdma1xFreq     *cdma1xFeqCfg = NULLP;
   SmNeigCdma1xFreqList   *cdma1xreqList = NULLP; 
   PTR                    prevEntry = NULLP;
   U32                    numFreqAdded = 0;
   S16                    retVal = ROK;

   TRC2(smBuildWrNr1xCdmaFreqCfg);

   lwrMng->t.protoCfg.type = WR_CFG_TYPE_NEIGH_FREQ;
   lwrMng->t.protoCfg.t.neighFreqCfg.action = WR_ACT_ADD;
   lwrMng->t.protoCfg.t.neighFreqCfg.cellId = 1; /* this is ownCellID in case of PLD based, jst a logic entity */

   while((retVal = cmHashListGetNext(&(smCfgCb.neighCdma1xFreqLstCp), prevEntry, (PTR *)&cdma1xreqList)) == ROK)
   {
     cdma1xFeqCfg = &cdma1xreqList->nghCdma1xFreqCfg;
     lwrMng->t.protoCfg.t.neighFreqCfg.t.addNeighFreq[numFreqAdded].type = WR_NEIGH_CELL_CDMA2K_1XRTT;
     nrFreqCfg = &lwrMng->t.protoCfg.t.neighFreqCfg.t.addNeighFreq[numFreqAdded].t.cdma1xFreq;

     *nrFreqCfg = *cdma1xFeqCfg;
     cdma1xreqList->dynCfgInd = WR_CFG_DONE; 
     prevEntry = (PTR)cdma1xreqList;
     numFreqAdded++;
   }
   lwrMng->t.protoCfg.t.neighFreqCfg.numFreq = numFreqAdded;
   RETVALUE(ROK); 
} /* end of smBuildWrNr1xCdmaFreqCfg */
                
 /*
 *
 *       Fun: smBuildWrNr1xCdmaCellCfg- Build Neighbour frequency
 *
 *       Desc:   
 *
 *       Ret:    Void
 *
 *       Notes:  None
 *
 *      File:    wr_smm_enbapp_cmnplatutils.c
 *
 */

PUBLIC S16 smBuildWrNr1xCdmaCellCfg
(
 LwrMngmt *lwrMng
)
{
   LwrCDMA1xNeighCell       *cdma1xNeghCell = NULLP;
   LwrCDMA1xNeighCell       *nrNeghCell = NULLP;
   SmNeigCdma1xCellList     *cdma1xCellList = NULLP; 
   PTR                      prevEntry = NULLP;
   U32                      numCellAdded = 0;
   S16                      retVal = ROK;

   TRC2(smBuildWrNr1xCdmaCellCfg);

   lwrMng->t.protoCfg.type = WR_CFG_TYPE_NEIGH_CELL;
   lwrMng->t.protoCfg.t.neighCellCfg.action = WR_ACT_ADD;
   lwrMng->t.protoCfg.t.neighCellCfg.cellId = 1; /* this is ownCellID in case of PLD, jst a logical entity */
  
   while((retVal = cmHashListGetNext(&(smCfgCb.neighCdma1xCellLstCp), prevEntry, (PTR *) &cdma1xCellList)) == ROK)
   {
     cdma1xNeghCell  = &cdma1xCellList->nghCdma1xCellCfg;
     lwrMng->t.protoCfg.t.neighCellCfg.t.neighAddCfg[numCellAdded].type = WR_NEIGH_CELL_CDMA2K_1XRTT;
     nrNeghCell =&(lwrMng->t.protoCfg.t.neighCellCfg.t.neighAddCfg[numCellAdded].t.cdma1xCell);

     *nrNeghCell = *cdma1xNeghCell;
     cdma1xCellList->dynCfgInd = WR_CFG_DONE;  
     prevEntry = (PTR)cdma1xCellList;
     numCellAdded++;
   }
   lwrMng->t.protoCfg.t.neighCellCfg.numCells = numCellAdded;
   RETVALUE(ROK); 
} /* end of smBuildWrNr1xCdmaCellCfg */
 
/*
 *
 *       Fun:    smBuildWrNrUtraFddFreqCfg - Build Neighbour frequency
 *
 *       Desc:   
 *
 *       Ret:    Void
 *
 *       Notes:  None
 *
 *      File:    wr_smm_enbapp_cmnplatutils.c
 *
 */
PUBLIC S16 smBuildWrNrUtraFddFreqCfg
(
 LwrMngmt *lwrMng
)
{
   LwrNeighUtraFreq   *nrFreqCfg =NULLP;
   LwrNeighUtraFreq   *utraFeqCfg = NULLP;
   SmNeigUtraFreqList *utrafreqList = NULLP;
   PTR                prevEntry = NULLP;
   U32                utraFddIdx = 0;
   S16               retVal = ROK;


   TRC2(smBuildWrNrUtraFddFreqCfg);

   lwrMng->t.protoCfg.type = WR_CFG_TYPE_NEIGH_FREQ;
   lwrMng->t.protoCfg.t.neighFreqCfg.action = WR_ACT_ADD;
   lwrMng->t.protoCfg.t.neighFreqCfg.cellId = 1; /* this is ownCellID in case of PLD based conf, jst a logic entity */

   while((retVal = cmHashListGetNext(&(smCfgCb.neighUtraFreqLstCp), prevEntry, (PTR *) &utrafreqList)) == ROK)
   {
     utraFeqCfg = &utrafreqList->nghUtraFreqCfg;
      lwrMng->t.protoCfg.t.neighFreqCfg.t.addNeighFreq[utraFddIdx].type = WR_NEIGH_CELL_UTRA_FDD;
      nrFreqCfg = &lwrMng->t.protoCfg.t.neighFreqCfg.t.addNeighFreq[utraFddIdx].t.utranFreq;

     *nrFreqCfg = *utraFeqCfg;
     utrafreqList->dynCfgInd = WR_CFG_DONE;  
     prevEntry = (PTR )utrafreqList; 
      (utraFddIdx)++;
   }
   lwrMng->t.protoCfg.t.neighFreqCfg.numFreq = utraFddIdx;
   RETVALUE(ROK);
} /* end of smBuildWrNrUtraFddFreqCfg */
 
/*
 *
 *       Fun:    smBuildWrNrUtraTddFreqCfg - Build Neighbour frequency
 *
 *       Desc:   
 *
 *       Ret:    Void
 *
 *       Notes:  None
 *
 *      File:    wr_smm_enbapp_cmnplatutils.c
 *
 */
PUBLIC S16 smBuildWrNrUtraTddFreqCfg
(
 LwrMngmt *lwrMng
)
{
   LwrNeighUtraFreq      *nrFreqCfg =NULLP;
   LwrNeighUtraFreq      *utraFeqCfg = NULLP;
   SmNeigUtraTddFreqList *utrafreqList = NULLP;
   PTR                   prevEntry = NULLP;
   U32                   utraTddIdx = 0;
   S16                   retVal = ROK;

   TRC2(smBuildWrNrUtraTddFreqCfg);

   lwrMng->t.protoCfg.type = WR_CFG_TYPE_NEIGH_FREQ;
   lwrMng->t.protoCfg.t.neighFreqCfg.action = WR_ACT_ADD;
   lwrMng->t.protoCfg.t.neighFreqCfg.cellId = 1; /* this is ownCellID in case of PLD based conf, jst a logic entity */

   while((retVal = cmHashListGetNext(&(smCfgCb.neighUtraTddFreqLstCp), prevEntry, (PTR *) &utrafreqList)) == ROK)
   {
      utraFeqCfg = &utrafreqList->nghUtraTddFreqCfg;
      lwrMng->t.protoCfg.t.neighFreqCfg.t.addNeighFreq[utraTddIdx].type = WR_NEIGH_CELL_UTRA_TDD;
      nrFreqCfg = &lwrMng->t.protoCfg.t.neighFreqCfg.t.addNeighFreq[utraTddIdx].t.utranFreq;

      *nrFreqCfg = *utraFeqCfg;
      utrafreqList->dynCfgInd = WR_CFG_DONE;  
      prevEntry = (PTR )utrafreqList; 
      (utraTddIdx)++;
   }
   lwrMng->t.protoCfg.t.neighFreqCfg.numFreq = utraTddIdx;

   RETVALUE(ROK);
} /* end of smBuildWrNrUtraTddFreqCfg */

/*
 *
 *       Fun:    smBuildWrNrGeranFreqCfg - Build Neighbour frequency
 *
 *       Desc:   LwrMngmt structure filled for GERAN Freq config
 *
 *       Ret:    S16
 *
 *       Notes:  None
 *
 *      File:    wr_smm_enbapp_cmnplatutils.c
 *
 */
PUBLIC S16 smBuildWrNrGeranFreqCfg
(
 LwrMngmt *lwrMng
)
{
   LwrNeighGERANFreq     *nrFreqCfg =NULLP;
   LwrNeighGERANFreq     *geranFeqCfg = NULLP;
   SmNeigGeranFreqList   *eutrFreqList = NULLP; 
   PTR                   prevEntry = NULLP;
   U32                   numFreqAdded = 0;
   S16                   retVal = ROK;

   TRC2(smBuildWrNrGeranFreqCfg);

   lwrMng->t.protoCfg.type = WR_CFG_TYPE_NEIGH_FREQ;
   lwrMng->t.protoCfg.t.neighFreqCfg.action = WR_ACT_ADD;
   lwrMng->t.protoCfg.t.neighFreqCfg.cellId = 1;

   while((retVal = cmHashListGetNext(&(smCfgCb.neighGeranFreqLstCp), prevEntry, (PTR *)&eutrFreqList)) == ROK)
   {
      geranFeqCfg = &eutrFreqList->nghGeranFreqCfg;
      lwrMng->t.protoCfg.t.neighFreqCfg.t.addNeighFreq[numFreqAdded].type = WR_NEIGH_CELL_GERAN;
      nrFreqCfg = &lwrMng->t.protoCfg.t.neighFreqCfg.t.addNeighFreq[numFreqAdded].t.geranFreq;

      *nrFreqCfg = *geranFeqCfg;
      prevEntry = (PTR)eutrFreqList;
      numFreqAdded++;
   }
   lwrMng->t.protoCfg.t.neighFreqCfg.numFreq = numFreqAdded;

   RETVALUE(ROK);
} /* end of smBuildWrNrGeranFreqCfg*/


/*
 *
 *       Fun:    smBuildWrNrGeranCellCfg - Build Neighbour cell cfg
 *
 *       Desc:   LwrMngmt structure filled with GERAN cell config
 *
 *       Ret:    S16
 *
 *       Notes:  None
 *
 *      File:    wr_smm_enbapp_cmnplatutils.c
 *
 */
PUBLIC S16 smBuildWrNrGeranCellCfg
(
 LwrMngmt *lwrMng
)
{
   LwrGERANNeighCell   *geranNeghCell = NULLP;
   LwrGERANNeighCell   *nrNeghCell = NULLP;
   SmNeigGeranCellList *geranCellList = NULLP; 
   PTR                prevEntry = NULLP;
   U32                numCellAdded = 0;
   S16                retVal = ROK;

   TRC2(smBuildWrNrGeranCellCfg);

   lwrMng->t.protoCfg.type = WR_CFG_TYPE_NEIGH_CELL;
   lwrMng->t.protoCfg.t.neighCellCfg.action = WR_ACT_ADD;
   lwrMng->t.protoCfg.t.neighCellCfg.cellId = 1;
  
   while((retVal = cmHashListGetNext(&(smCfgCb.neighGeranCellLstCp), prevEntry, (PTR *) &geranCellList)) == ROK)
   {
     geranNeghCell  = &geranCellList->nghGeranCellCfg;
     lwrMng->t.protoCfg.t.neighCellCfg.t.neighAddCfg[numCellAdded].type = WR_NEIGH_CELL_GERAN;
     nrNeghCell =&(lwrMng->t.protoCfg.t.neighCellCfg.t.neighAddCfg[numCellAdded].t.geranCell);

     *nrNeghCell = *geranNeghCell;
     prevEntry = (PTR)geranCellList;
     numCellAdded++;
   }
   lwrMng->t.protoCfg.t.neighCellCfg.numCells = numCellAdded; 

   RETVALUE(ROK);
} /* end of smBuildWrNrGeranCellCfg */

/*
 *
 *       Fun:    smBuildWrNrEutraFreqCfg - Build Neighbour frequency
 *
 *       Desc:   
 *
 *       Ret:    Void
 *
 *       Notes:  None
 *
 *      File:    wr_smm_enbapp.c
 *
 */

PUBLIC S16 smBuildWrNrEutraFreqCfg
(
 LwrMngmt *lwrMng
)
{
   LwrNeighEutraFreq     *nrFreqCfg =NULLP;
   LwrNeighEutraFreq     *eutraFeqCfg = NULLP;
   SmNeigEutraFreqList   *eutrFreqList = NULLP; 
   PTR                    prevEntry = NULLP;
   U32                    numFreqAdded = 0;
   S16                    retVal = ROK;

   TRC2(smBuildWrNrEutraFreqCfg);

   lwrMng->t.protoCfg.type = WR_CFG_TYPE_NEIGH_FREQ;
   lwrMng->t.protoCfg.t.neighFreqCfg.action = WR_ACT_ADD;
   lwrMng->t.protoCfg.t.neighFreqCfg.cellId = 1; /* this is ownCellID in case of PLD based, jst a logic entity */

   while((retVal = cmHashListGetNext(&(smCfgCb.neighEutraFreqLstCp), prevEntry, (PTR *)&eutrFreqList)) == ROK)
   {
     eutraFeqCfg = &eutrFreqList->nghEutraFreqCfg;
     RLOG1(L_INFO,"Sending Neighbour frequency of EARFCN %d to eNBAPP",
        eutraFeqCfg->earfcnDl);
     lwrMng->t.protoCfg.t.neighFreqCfg.t.addNeighFreq[numFreqAdded].type = WR_NEIGH_CELL_EUTRA;
     nrFreqCfg = &lwrMng->t.protoCfg.t.neighFreqCfg.t.addNeighFreq[numFreqAdded].t.eutranFreq;

     *nrFreqCfg = *eutraFeqCfg;
     eutrFreqList->dynCfgInd = WR_CFG_DONE; 
     prevEntry = (PTR)eutrFreqList;
     numFreqAdded++;
   }
   lwrMng->t.protoCfg.t.neighFreqCfg.numFreq = numFreqAdded;
   RETVALUE(ROK);
} /* end of smBuildWrNrEutraFreqCfg */
 
/* Commented as this is unused function as of now. Need to maintain for future purpose 
PRIVATE S16 smUtlGetNumNghEnbCfg
(
Void
)
{
   S16 indx1;
   S16 indx2;
   U32 enbId = 0;
   S16 numEnb = 0;
   Bool newEnb = TRUE; *//* it much be always true for the first enb */

/*   TRC2(smUtlGetNumNghEnbCfg);

   for(indx1 = 0; indx1 < smCfgCb.noOfNghCfg; indx1++)
   {
      enbId = smCfgCb.wrNghCellCfg[indx1]->enbId;
      for(indx2 = 0; indx2 <= indx1; indx2++)
      {
         if(enbId == smCfgCb.wrNghCellCfg[indx2]->enbId)
         {
            newEnb = TRUE;
         }
      }
      if(newEnb == TRUE)
         numEnb++;

      newEnb = FALSE;
   }
   RETVALUE(numEnb);
} *//* end of smUtlGetNumNghEnbCfg */


/*PRIVATE S16 smUtlGetNumIntraFreqNghCellCfg
(
)
{
   S32   lpCnt;
   S16   numCells = 0;

   TRC2(smUtlGetNumIntraFreqNghCellCfg);

   for (lpCnt = 0; lpCnt < smCfgCb.noOfNghCfg; lpCnt++)
   {
*/      /* not including intra - frequency neighbors */
/*      if(smCfgCb.wrNghCellCfg[lpCnt]->dlEarfcn == wrSmDfltServingCellDLCarrierFreq)
      {
         numCells++;
      }
   }
   RETVALUE(numCells);
}
*/


/*
*
*       Fun:   smBuildWrNehCellCfg
*
*       Desc:  This function for filling neighbour cell configuration
*
*       Ret:   ROK
*
*       Notes: None
*              
*       File:  wr_sm_wr_cntrl.c 
*
*/
#ifdef ANSI
PUBLIC S16 smBuildWrNehCellCfg
(
LwrMngmt *lwrMng
)
#else /* if ANSI not defined*/
PUBLIC S16 smBuildWrNehCellCfg(*lwrMng)
LwrMngmt *lwrMng;
#endif /* end of ANSI*/
{
   LwrEUTRANeighCell  *eutraNeghCell = NULLP;
   LwrEUTRANeighCell   *nrNeghCell = NULLP;
   SmNeigEutraCellList *eutraCellList = NULLP; 
   PTR               prevEntry = NULLP;
   U32               numCellAdded = 0;
   S16               retVal = ROK;

   TRC2(smBuildWrNehCellCfg);

   lwrMng->t.protoCfg.type = WR_CFG_TYPE_NEIGH_CELL;
   lwrMng->t.protoCfg.t.neighCellCfg.action = WR_ACT_ADD;
   lwrMng->t.protoCfg.t.neighCellCfg.cellId = 1; /* this is ownCellID in case of PLD, jst a logical entity */
  
   while((retVal = cmHashListGetNext(&(smCfgCb.neighEutraCellLstCp), prevEntry, (PTR *) &eutraCellList)) == ROK)
   {
     eutraNeghCell  = &eutraCellList->nghEutraCellCfg;
     lwrMng->t.protoCfg.t.neighCellCfg.t.neighAddCfg[numCellAdded].type = WR_NEIGH_CELL_EUTRA;
     lwrMng->t.protoCfg.t.neighCellCfg.t.neighAddCfg[numCellAdded].cfgType = WR_NEIGH_CELL_CFG_INITIAL;
     nrNeghCell =&(lwrMng->t.protoCfg.t.neighCellCfg.t.neighAddCfg[numCellAdded].t.eutraCell);

     *nrNeghCell = *eutraNeghCell;
     eutraCellList->dynCfgInd = WR_CFG_DONE;  
     prevEntry = (PTR)eutraCellList;
     numCellAdded++;
   }
   lwrMng->t.protoCfg.t.neighCellCfg.numCells = numCellAdded;
   RETVALUE(ROK);
} /* end of smBuildWrNehCellCfg */

/*
*
*       Fun:   smBuildWrMmeCfg
*
*       Desc:  fill in neighbour Cfg for WR
*
*       Ret:   ROK
*
*       Notes: None
*              
*       File:  wr_sm_wr_cntrl.c 
*
*/
#ifdef ANSI
PUBLIC S16 smBuildWrMmeCfg
(
LwrMngmt *lwrMng,
LwrSmMmeCfg *mmeSmCfg
)
#else /* if ANSI not defined*/
PUBLIC S16 smBuildWrMmeCfg(*lwrMng,*mmeCfg)
LwrMngmt *lwrMng;
LwrSmMmeCfg *mmeSmCfg;
#endif /* end of ANSI*/
{
   LwrAddModMmeCfg *mmeCfg =NULLP;
   U8              idx = 0;
   
   TRC2(smBuildWrMmeCfg);

   lwrMng->t.protoCfg.type = WR_CFG_TYPE_MME;
   lwrMng->t.protoCfg.t.mmeCfg.action = WR_ACT_ADD;

   mmeCfg =&(lwrMng->t.protoCfg.t.mmeCfg.t.addMme);

   mmeCfg->mmeIndex = (U8)(mmeSmCfg->mmeId);

   /*Filling IP Address*/
   for(idx = 0; idx < mmeSmCfg->noOfIps; idx++)
   {
      /*Filling IP Address*/
      wrCpyCmTptAddr(&mmeCfg->ipAddr[idx], &mmeSmCfg->mmeAddr[idx]);
      if(CM_TPTADDR_IPV4 == mmeCfg->ipAddr[idx].type)
      {
         mmeCfg->ipAddr[idx].u.ipv4TptAddr.port = wrSmDfltMmePort;
      }
      else if(CM_TPTADDR_IPV6 == mmeCfg->ipAddr[idx].type)
      {
         mmeCfg->ipAddr[idx].u.ipv6TptAddr.port = wrSmDfltMmePort;
      }
   }
   mmeCfg->noOfIps = idx;

   /*Filling MME state*/
   mmeCfg->mmeState =wrSmDfltMmeState;
   mmeCfg->adminState =wrSmDfltMmeAdminState;

   mmeCfg->alarmVal.maxErrIndCnt = faultMgmtParams.frequentFaultMgmtParameters.maxS1ErrIndCnt;
   mmeCfg->alarmVal.maxResetCnt = faultMgmtParams.frequentFaultMgmtParameters.maxS1ResetCnt;
   mmeCfg->alarmVal.minErrIndTime = faultMgmtParams.frequentFaultMgmtParameters.minS1ErrIndTime;
   mmeCfg->alarmVal.minResetTime = faultMgmtParams.frequentFaultMgmtParameters.minS1ResetTime;
   RETVALUE(ROK);
} /* end of smBuildWrMmeCfg */



/*
*
*       Fun:   smBuildWrNghCfg
*
*       Desc:  fill in neighbour Cfg for WR
*
*       Ret:   ROK
*
*       Notes: None
*              
*       File:  wr_sm_wr_cntrl.c 
*
*/
#ifdef ANSI
PUBLIC S16 smBuildWrNghCfg
(
LwrMngmt *lwrMng
)
#else /* if ANSI not defined*/
PUBLIC S16 smBuildWrNghCfg(*lwrMng)
LwrMngmt *lwrMng;
#endif /* end of ANSI*/
{
   LwrNeighAddModCfg  *neighCfg = NULLP;
   LwrNeighAddModCfg *neighEnbCfg =NULLP;
   SmNeigEnbList      *smNeigEnbCfg = NULLP;
   PTR               prevEntry = NULLP;
   U32               nubEnbAdded = 0;
   S16               retVal = ROK;


   TRC2(smBuildWrNghCfg);

   lwrMng->t.protoCfg.type = WR_CFG_TYPE_NEIGH;
   lwrMng->t.protoCfg.t.neighCfg.action = WR_ACT_ADD;
   lwrMng->t.protoCfg.t.neighCfg.cellId = 1; /* this ownCellID in case of PLD based, jst a logic entity */

   while((retVal = cmHashListGetNext(&(smCfgCb.neighEnbLstCp), prevEntry, (PTR *) &smNeigEnbCfg)) == ROK)
   {
     neighEnbCfg = &smNeigEnbCfg->nghEnbCfg;
     neighCfg = &lwrMng->t.protoCfg.t.neighCfg.t.neighAdd[nubEnbAdded];  

     *neighCfg = *neighEnbCfg;
     smNeigEnbCfg->dynCfgInd = WR_CFG_DONE; 
     prevEntry = (PTR)smNeigEnbCfg;
     nubEnbAdded++;
   }
   lwrMng->t.protoCfg.t.neighCfg.numEnb = nubEnbAdded; 
   RETVALUE(ROK);
} /* end of smBuildWrNghCfg */

/*
*
*       Fun:   smBuildDynWrNbrEnbCfg
*
*       Desc:  fill in neighbour Cfg for WR
*
*       Ret:   ROK
*
*       Notes: None
*              
*       File:  wr_sm_wr_cntrl.c 
*
*/
#ifdef ANSI
PUBLIC S16 smBuildDynWrNbrEnbCfg
(
U32    enbId,
LwrMngmt *lwrMng
)
#else /* if ANSI not defined*/
PUBLIC S16 smBuildDynWrNbrEnbCfg(enbId,lwrMng)
U32   enbId;
LwrMngmt *lwrMng;
#endif /* end of ANSI*/
{
   SmNeigEnbList  *nghEnbList = NULLP;
   TRC2(smBuildDynWrNbrEnbCfg);

   if(ROK != cmHashListFind(&(smCfgCb.neighEnbLstCp), (U8*)&enbId, sizeof(enbId), 0,(PTR*) &nghEnbList))
   {
      RLOG1(L_ERROR, "Neighbour eNodeB %ld not configurured", enbId);
      RETVALUE(RFAILED);
   }
   lwrMng->t.nbrAddCfm.enbId  = nghEnbList->nghEnbCfg.enbId;
   lwrMng->t.nbrAddCfm.peerId = nghEnbList->nghEnbCfg.peerId;
   wrCpyCmTptAddr(&lwrMng->t.nbrAddCfm.ipAddr,&nghEnbList->nghEnbCfg.ipAddr);

   RETVALUE(ROK);
}/*smBuildDynWrNbrEnbCfg*/
 
/*
*
*       Fun:   smBldSib1Cfg
*
*       Desc:  fill in sib1 Cfg for WR
*
*       Ret:   ROK
*
*       Notes: None
*              
*       File:  wr_smm_enbapp_cmnplatutils.c 
*
*/

PRIVATE Void smBldSib1Cfg
(
 LwrMngmt   *lwrMng,
 U8         cfgGrpIndex,
 U8         accessMode
)
{
   LwrCellSib1CfgGrp    *sib1 =NULLP;
   MsmLteCellSib1CfgGrp *sib1cfg = NULLP;
   MsmLtePlmnId  *priPlmn;
   MsmLtePlmnId  *plmn;
   U32 idx; 
   U32 idx1 = 1; 
   TRC2(smBldSib1Cfg);

   /*Filling sib1 params*/
   lwrMng->t.protoCfg.t.cellCfg.t.addCell.cellCfgGrp[cfgGrpIndex]->grpType 
      = WR_CELL_CFG_GRP_SIB1;

   sib1 =&(lwrMng->t.protoCfg.t.cellCfg.t.addCell.cellCfgGrp[cfgGrpIndex]->t.sib1CfgGrp);

   sib1cfg  =&(lteeNodeBparams.lteCellSib1CfgGrp);
   sib1->freqBandInd = sib1cfg->freqBandInd;
   sib1->cellAccessInfo.numOfPlmns = lteeNodeBparams.lteCellSib1CfgGrp.cellAccessInfo.numOfPlmns;
   for(idx =0; idx < sib1cfg->cellAccessInfo.numOfPlmns; idx++ )
   {
      if(lteeNodeBparams.lteCellSib1CfgGrp.cellAccessInfo.plmnInfo[idx].isPrimary == TRUE)
      {
         priPlmn = &sib1cfg->cellAccessInfo.plmnInfo[idx].plmnId;
         sib1->cellAccessInfo.plmnInfo[0].plmnId.numMncDigits  = priPlmn->numMncDigits;
         sib1->cellAccessInfo.plmnInfo[0].plmnId.mcc[0] =priPlmn->mcc[0];
         sib1->cellAccessInfo.plmnInfo[0].plmnId.mcc[1] =priPlmn->mcc[1];
         sib1->cellAccessInfo.plmnInfo[0].plmnId.mcc[2] =priPlmn->mcc[2];
      
         sib1->cellAccessInfo.plmnInfo[0].plmnId.mnc[0]=priPlmn->mnc[0];
         sib1->cellAccessInfo.plmnInfo[0].plmnId.mnc[1]=priPlmn->mnc[1];
         sib1->cellAccessInfo.plmnInfo[0].plmnId.mnc[2]=priPlmn->mnc[2];
         sib1->cellAccessInfo.plmnInfo[0].rsrvdFrOperUse = 
         sib1cfg->cellAccessInfo.plmnInfo[idx].rsrvdFrOperUse;
      }
      else
      {
         plmn = &sib1cfg->cellAccessInfo.plmnInfo[idx].plmnId;
         sib1->cellAccessInfo.plmnInfo[idx1].plmnId.numMncDigits  = plmn->numMncDigits;
         sib1->cellAccessInfo.plmnInfo[idx1].plmnId.mcc[0] =plmn->mcc[0];
         sib1->cellAccessInfo.plmnInfo[idx1].plmnId.mcc[1] =plmn->mcc[1];
         sib1->cellAccessInfo.plmnInfo[idx1].plmnId.mcc[2] =plmn->mcc[2];

         sib1->cellAccessInfo.plmnInfo[idx1].plmnId.mnc[0]=plmn->mnc[0];
         sib1->cellAccessInfo.plmnInfo[idx1].plmnId.mnc[1]=plmn->mnc[1];
         sib1->cellAccessInfo.plmnInfo[idx1].plmnId.mnc[2]=plmn->mnc[2];
         sib1->cellAccessInfo.plmnInfo[idx1].rsrvdFrOperUse = 
         sib1cfg->cellAccessInfo.plmnInfo[idx].rsrvdFrOperUse;
         idx1++;
      }
   }


   sib1->cellAccessInfo.tac = sib1cfg->cellAccessInfo.tac;
   sib1->cellAccessInfo.cellId = sib1cfg->cellAccessInfo.cellId;
   /* CSG_DEV start */
   if(WR_ENB_OPEN_ACCESS == accessMode)
   {
      sib1->cellAccessInfo.csgIndication = FALSE;
      sib1->cellAccessInfo.csgIdentity.pres = NOTPRSNT;
   }
   else if(WR_ENB_CLOSED_ACCESS == accessMode)
   {
      sib1->cellAccessInfo.csgIndication = TRUE;
      wrFillTknU32(&sib1->cellAccessInfo.csgIdentity,
            sib1cfg->cellAccessInfo.csgIdentity);
   }
   else /* hybrid access */
   {
      sib1->cellAccessInfo.csgIndication = FALSE;
      wrFillTknU32(&sib1->cellAccessInfo.csgIdentity,
            sib1cfg->cellAccessInfo.csgIdentity);
   }
   /* CSG_DEV end */
   sib1->cellAccessInfo.cellBarred = sib1cfg->cellAccessInfo.cellBarred;
   sib1->cellAccessInfo.intraFreqReSelection = (Bool)wrSmDfltIntraFreqResel;
   sib1->cellSelectInfo.qRxLevMin = sib1cfg->cellSelectInfo.qRxLevMin;
   /* Start - Fix for ccpu00123651 */
   wrFillTknU8(&(sib1->cellSelectInfo.qRxLevMinOffset), sib1cfg->cellSelectInfo.qRxLevMinOffset);
   /* End - Fix for ccpu00123651 */
   sib1->pMax = sib1cfg->pMax;

   sib1->siWindowSize = wrSmDfltSiWinSize;

   /*TODO:*/
#ifdef LTE_TDD
   sib1->tddCfg.specialSubframeCfg = lteeNodeBparams.lteTddParam.specialSubFramePattern;
   sib1->tddCfg.subframeCfg = lteeNodeBparams.lteTddParam.subframeAssignment;
#else
   sib1->tddCfg.specialSubframeCfg = 8;
   sib1->tddCfg.subframeCfg = 2;
#endif
   RETVOID;
} /* end of smBldSib1Cfg */  

/*
*
*       Fun:   smBldSib2Cfg
*
*       Desc:  fill in sib2 Cfg for WR
*
*       Ret:   ROK
*
*       Notes: None
*              
*       File:  wr_smm_enbapp_cmnplatutils.c 
*
*/

PRIVATE Void smBldSib2Cfg
(
 LwrMngmt   *lwrMng,
 U8         cfgGrpIndex
)
{
   LwrCellSib2CfgGrp *sib2 = NULLP;
   LwrPuschCfgCommon *puschCfgCmn = NULLP;
   LwrPucchCfgCommon *pucchCfgCmn = NULLP;
   LwrSRSCfgCommon   *srsCfgCmn = NULLP;
   LwrPrachCfgCommon *prachCmn = NULLP;
   LwrRachCfgCommon  *rachCmn = NULLP;
   LwrULPwrCtrlCommon *uLPwrCtrlCmn = NULLP;
   LwrUeTimerConstants *ueTmr = NULLP;

   MsmLtePreambleCfgParams *lteRachPreambleCfg = NULLP;
   MsmLteDeltaPUCCHLst *deltaFLst = NULLP;
   MsmLteULPwrCtrlCommon *msmUlPwrCtrlCmn = NULLP;
   MsmPrachCfgInfo *prachCfgInfo = NULLP;
   MsmLteRachCfgCommonParams *lteRachCfgComnParams = NULLP;

   TRC2(smBldSib2Cfg);

   lwrMng->t.protoCfg.t.cellCfg.t.addCell.cellCfgGrp[cfgGrpIndex]->grpType = WR_CELL_CFG_GRP_SIB2;

   sib2 =&(lwrMng->t.protoCfg.t.cellCfg.t.addCell.cellCfgGrp[cfgGrpIndex]->t.sib2CfgGrp);

   lteRachCfgComnParams = &lteeNodeBparams.lteRachCfgCommonParams;  
   lteRachPreambleCfg = &lteRachCfgComnParams->ltePreambleCfg;
   msmUlPwrCtrlCmn = &(lteeNodeBparams.lteULPwrCtrlCommon);
   deltaFLst = &(msmUlPwrCtrlCmn->deltaFLst);
   prachCfgInfo = &(lteeNodeBparams.ltePrachCfgCommon.prachCfgInfo);


   puschCfgCmn = &(sib2->radioResourceCommon.puschCfgCommon);
   pucchCfgCmn = &(sib2->radioResourceCommon.pucchCfgCommon);
   srsCfgCmn = &(sib2->radioResourceCommon.srsCfgCommon);
   prachCmn = &(sib2->radioResourceCommon.prachCfgCommon);
   rachCmn = &(sib2->radioResourceCommon.rachCfgCommon);
   uLPwrCtrlCmn = &(sib2->radioResourceCommon.ulPwrCtrlCommon);
   ueTmr = &(sib2->ueTimerConstants);

   sib2->ueTimerConstants.t302 = 9;
   wrFillTknU8(&(sib2->acBarringInfo.pres), wrSmDfltACBarrInfoPres);
   sib2->acBarringInfo.acBarringForEmergency =  lteeNodeBparams.lteAcBarringInfo.acBarringForEmergency;
  
   wrFillTknU8(&(sib2->acBarringInfo.acBarringForMOData.pres), lteeNodeBparams.lteAcBarringInfo.barringDataUsage);

   sib2->ulFreqInfo.ulBandwidth = lteeNodeBparams.lteULFrequencyInfo.ulBandwidth;
   sib2->ulFreqInfo.eARFCN =  lteeNodeBparams.lteULFrequencyInfo.eARFCN; 

   sib2->ulFreqInfo.addSpectEmission =(U8)wrSmDfltAddSpectEmn;

   sib2->acBarringInfo.acBarringForMOData.acBarringAC11 = lteeNodeBparams.lteAcBarringInfo.acBarringForMOData.acBarringAC11;
   sib2->acBarringInfo.acBarringForMOData.acBarringAC12 = lteeNodeBparams.lteAcBarringInfo.acBarringForMOData.acBarringAC12;
   sib2->acBarringInfo.acBarringForMOData.acBarringAC13 = lteeNodeBparams.lteAcBarringInfo.acBarringForMOData.acBarringAC13;
   sib2->acBarringInfo.acBarringForMOData.acBarringAC14 = lteeNodeBparams.lteAcBarringInfo.acBarringForMOData.acBarringAC14;
   sib2->acBarringInfo.acBarringForMOData.acBarringAC15 = lteeNodeBparams.lteAcBarringInfo.acBarringForMOData.acBarringAC15;

   /* RRC 3GPP TS 36.331:Sec 6.3.1: 
    * ac-BarringFactor Values other than p00 can only be set if all 
    * bits of the corresponding ac-BarringForSpecialAC are set to 0.
    * Originaly these changes has to be performed in OAM. But these
    * check added for 3rd party 
    */ 
   if((sib2->acBarringInfo.acBarringForMOData.acBarringAC11    | 
         sib2->acBarringInfo.acBarringForMOData.acBarringAC12 |
         sib2->acBarringInfo.acBarringForMOData.acBarringAC13 |
         sib2->acBarringInfo.acBarringForMOData.acBarringAC14 |
         sib2->acBarringInfo.acBarringForMOData.acBarringAC15) &&
         (NhuAC_BarringConfigac_BarringFactorp00Enum != sib2->acBarringInfo.acBarringForMOData.acBarringFactor))
   {
      sib2->acBarringInfo.acBarringForMOData.acBarringFactor = NhuAC_BarringConfigac_BarringFactorp00Enum;
   }
   else
   {
      sib2->acBarringInfo.acBarringForMOData.acBarringFactor = lteeNodeBparams.lteAcBarringInfo.acBarringForMOData.acBarringFactor;
   }

   sib2->acBarringInfo.acBarringForMOData.acBarringTime = lteeNodeBparams.lteAcBarringInfo.acBarringForMOData.acBarringTime;

   wrFillTknU8(&(sib2->acBarringInfo.acBarringForMOSignal.pres), lteeNodeBparams.lteAcBarringInfo.barringSignalUsage);

   sib2->acBarringInfo.acBarringForMOSignal.acBarringAC11 = lteeNodeBparams.lteAcBarringInfo.acBarringForMOSignal.acBarringAC11;
   sib2->acBarringInfo.acBarringForMOSignal.acBarringAC12 = lteeNodeBparams.lteAcBarringInfo.acBarringForMOSignal.acBarringAC12;
   sib2->acBarringInfo.acBarringForMOSignal.acBarringAC13 = lteeNodeBparams.lteAcBarringInfo.acBarringForMOSignal.acBarringAC13;
   sib2->acBarringInfo.acBarringForMOSignal.acBarringAC14 = lteeNodeBparams.lteAcBarringInfo.acBarringForMOSignal.acBarringAC14;
   sib2->acBarringInfo.acBarringForMOSignal.acBarringAC15 = lteeNodeBparams.lteAcBarringInfo.acBarringForMOSignal.acBarringAC15;

    /* RRC 3GPP TS 36.331:Sec 6.3.1: 
    * ac-BarringFactor Values other than p00 can only be set if all 
    * bits of the corresponding ac-BarringForSpecialAC are set to 0.
    * Originaly these changes has to be performed in OAM. But these
    * check added for 3rd party 
    */  
   if((sib2->acBarringInfo.acBarringForMOSignal.acBarringAC11    |
         sib2->acBarringInfo.acBarringForMOSignal.acBarringAC12 |
         sib2->acBarringInfo.acBarringForMOSignal.acBarringAC13 |
         sib2->acBarringInfo.acBarringForMOSignal.acBarringAC14 |
         sib2->acBarringInfo.acBarringForMOSignal.acBarringAC15) &&
         (NhuAC_BarringConfigac_BarringFactorp00Enum != sib2->acBarringInfo.acBarringForMOSignal.acBarringFactor))
   {
      sib2->acBarringInfo.acBarringForMOSignal.acBarringFactor = NhuAC_BarringConfigac_BarringFactorp00Enum;
   }
   else
   {
      sib2->acBarringInfo.acBarringForMOSignal.acBarringFactor = lteeNodeBparams.lteAcBarringInfo.acBarringForMOSignal.acBarringFactor;
   }

   sib2->acBarringInfo.acBarringForMOSignal.acBarringTime = lteeNodeBparams.lteAcBarringInfo.acBarringForMOSignal.acBarringTime;
   wrFillTknU8(&(sib2->acBarringVideo.pres),wrSmDfltVDACBarrInfoPres);

   sib2->acBarringVideo.acBarringAC11 = wrSmDfltacBarringAC11;

   sib2->acBarringVideo.acBarringAC12 = wrSmDfltacBarringAC12;
   sib2->acBarringVideo.acBarringAC13 = wrSmDfltacBarringAC13;
   sib2->acBarringVideo.acBarringAC14 = wrSmDfltacBarringAC14;
   sib2->acBarringVideo.acBarringAC15 = wrSmDfltacBarringAC15;
   sib2->acBarringVideo.acBarringFactor = wrSmDfltacBarringFactor;
   sib2->acBarringVideo.acBarringTime = wrSmDfltacBarringTime;

   wrFillTknU8(&(sib2->acBarringVideo.pres),wrSmDfltVOACBarrInfoPres);

   sib2->acBarringVoice.acBarringAC11 = wrSmDfltacBarringAC11;
   sib2->acBarringVoice.acBarringAC12 = wrSmDfltacBarringAC12;
   sib2->acBarringVoice.acBarringAC13 = wrSmDfltacBarringAC13;
   sib2->acBarringVoice.acBarringAC14 = wrSmDfltacBarringAC14;
   sib2->acBarringVoice.acBarringAC15 = wrSmDfltacBarringAC15;
   sib2->acBarringVoice.acBarringFactor = wrSmDfltacBarringFactor;
   sib2->acBarringVoice.acBarringTime = wrSmDfltacBarringTime;

   /*Filling BCCH*/
   sib2->radioResourceCommon.bcchCfgCommon.modPeriodCoeff 
      =(U8)wrSmDfltModPeriodCoeff;
   /*Filling PCCH*/
   sib2->radioResourceCommon.pcchCfgCommon.defaultPagingCycle 
      =lteeNodeBparams.ltePCCHCfgCommon.defaultPagingCycle;
   sib2->radioResourceCommon.pcchCfgCommon.nB =lteeNodeBparams.ltePCCHCfgCommon.nB;

   /*Filling PDSCH*/

   sib2->radioResourceCommon.pdschCfgCommon.pB = lteeNodeBparams.ltePdschCfgCommon.pB[0];
   sib2->radioResourceCommon.pdschCfgCommon.referenceSignalPwr = 
      lteeNodeBparams.ltePdschCfgCommon.referenceSignalPwr[0];
   /*Filling PRACH*/

   prachCmn->rootSequenceIndx = lteeNodeBparams.ltePrachCfgCommon.rootSequenceIndx;
   prachCmn->prachCfgInfo.highSpeedFlag = prachCfgInfo->highSpeedFlag;
   prachCmn->prachCfgInfo.prachCfgIdx = prachCfgInfo->prachCfgIdx;
   prachCmn->prachCfgInfo.prachFreqOffset = prachCfgInfo->prachFreqOffset;
   prachCmn->prachCfgInfo.zeroCorrelationZoneCfg= prachCfgInfo->zeroCorrelationZoneCfg;
   /*Filling PUCCH*/

   /*Filling RACH*/
   rachCmn->maxHARQMsg3 = lteRachCfgComnParams->maxHARQMsg3;
   rachCmn->powerRamping.initRcvdTrgtPwr = lteRachCfgComnParams->ltePowerRamping.initRcvdTrgtPwr;
   rachCmn->powerRamping.powerRampingstep= lteRachCfgComnParams->ltePowerRamping.powerRampingstep;
   rachCmn->preambleCfg.groupASize = lteRachPreambleCfg->groupASize;
   rachCmn->preambleCfg.numOfRAPreambles= lteRachPreambleCfg->numOfRAPreambles;
   rachCmn->preambleCfg.msgSizeGroupA= lteRachPreambleCfg->msgSizeGroupA;  
   rachCmn->preambleCfg.pwrOffstGrpB= lteRachPreambleCfg->pwrOffstGrpB;
   rachCmn->raSupervisionCfg.macContResTimer= lteRachCfgComnParams->lteRaSupervisionCfg.macContResTimer;
   rachCmn->raSupervisionCfg.preambleTransMax= lteRachCfgComnParams->lteRaSupervisionCfg.preambleTransMax;
   rachCmn->raSupervisionCfg.rarWindowSize= lteRachCfgComnParams->lteRaSupervisionCfg.rarWindowSize;

   /*Filling SRC */

   sib2->radioResourceCommon.srsCfgCommon.subframeCfg =(U8)wrSmDfltSrsSubfrmCfg;

   /*Filling UL Power Control*/
   sib2->radioResourceCommon.ulCyclicPrefix =wrSmDfltUlCyclicPrefixLength;

   uLPwrCtrlCmn->alpha = msmUlPwrCtrlCmn->alpha;
   uLPwrCtrlCmn->deltaMsg3 = msmUlPwrCtrlCmn->deltaMsg3;
   uLPwrCtrlCmn->pNomialPucch= msmUlPwrCtrlCmn->pNomialPucch;
   uLPwrCtrlCmn->pNomialPusch= msmUlPwrCtrlCmn->pNomialPusch;
   uLPwrCtrlCmn->deltaFLst.deltaPucchFormat1= deltaFLst->deltaPucchFormat1;
   uLPwrCtrlCmn->deltaFLst.deltaPucchFormat1b= deltaFLst->deltaPucchFormat1b;
   uLPwrCtrlCmn->deltaFLst.deltaPucchFormat2= deltaFLst->deltaPucchFormat2;
   uLPwrCtrlCmn->deltaFLst.deltaPucchFormat2a= deltaFLst->deltaPucchFormat2a;
   uLPwrCtrlCmn->deltaFLst.deltaPucchFormat2b= deltaFLst->deltaPucchFormat2b;

   wrSmEnableSrs = lteeNodeBparams.lteSRSCfgCommon.enable;
   srsCfgCmn->cSrs = lteeNodeBparams.lteSRSCfgCommon.cSrs;
   srsCfgCmn->simulAckNackSrs = lteeNodeBparams.lteSRSCfgCommon.simulAckNackSrs;
   srsCfgCmn->srsMaxUpts = lteeNodeBparams.lteSRSCfgCommon.srsMaxUpts;

   pucchCfgCmn->deltaShift = lteeNodeBparams.ltePucchCfgCommon.deltaShift;
   pucchCfgCmn->nRBCqi = lteeNodeBparams.ltePucchCfgCommon.nRBCqi;
   pucchCfgCmn->nCSAN = lteeNodeBparams.ltePucchCfgCommon.nCSAN;
   pucchCfgCmn->n1PucchAn = lteeNodeBparams.ltePucchCfgCommon.n1PucchAn;

   puschCfgCmn->puschRefSigCfg.grpHopEnabled = lteeNodeBparams.ltePuschRefSignalCfg.grpHopEnabled;
   puschCfgCmn->puschRefSigCfg.grpAsgnPusch = lteeNodeBparams.ltePuschRefSignalCfg.grpAsgnPusch;
   puschCfgCmn->puschRefSigCfg.seqHopEnabled = lteeNodeBparams.ltePuschRefSignalCfg.seqHopEnabled;
   puschCfgCmn->puschRefSigCfg.cyclicShift = lteeNodeBparams.ltePuschRefSignalCfg.cyclicShift;

   puschCfgCmn->puschBasicCfg.nSb = lteeNodeBparams.ltePuschBasicCfgCommon.nSb;
   puschCfgCmn->puschBasicCfg.hoppingMode = lteeNodeBparams.ltePuschBasicCfgCommon.hoppingMode;
   puschCfgCmn->puschBasicCfg.hoppingOffset = lteeNodeBparams.ltePuschBasicCfgCommon.hoppingOffset;
   puschCfgCmn->puschBasicCfg.enable64QAM = lteeNodeBparams.ltePuschBasicCfgCommon.enable64QAM;

   ueTmr->t300 = lteeNodeBparams.lteUeTimerConstants.t300; 
   ueTmr->t301 = lteeNodeBparams.lteUeTimerConstants.t301; 
   ueTmr->t302 = lteeNodeBparams.lteUeTimerConstants.t302; 
   ueTmr->t310 = lteeNodeBparams.lteUeTimerConstants.t310; 
   ueTmr->n310 = lteeNodeBparams.lteUeTimerConstants.n310; 
   ueTmr->t311 = lteeNodeBparams.lteUeTimerConstants.t311; 
   ueTmr->n311 = lteeNodeBparams.lteUeTimerConstants.n311; 
   ueTmr->t320 = lteeNodeBparams.lteUeTimerConstants.t320; 
   /* ccpu00125578 T304 value set to ms500 so that
    * RRC Connection Reestablishment will be delayed during HO */
   ueTmr->t304 = lteeNodeBparams.lteUeTimerConstants.t304; 
   ueTmr->ccoT304 = lteeNodeBparams.lteUeTimerConstants.ccoT304; 
   sib2->timeAlignmentCommon = lteeNodeBparams.sib2Cfg.timeAlignmentCommon;

   RETVOID;
} /* end of smBldSib2Cfg */

/*
*
*       Fun:   smBldSib3Cfg
*
*       Desc:  fill in sib3 Cfg for WR
*
*       Ret:   ROK
*
*       Notes: None
*              
*       File:  wr_smm_enbapp_cmnplatutils.c 
*
*/

PRIVATE Void smBldSib3Cfg
(
 LwrMngmt   *lwrMng,
 U8         cfgGrpIndex
)
{
   LwrCellSib3CfgGrp *sib3 =NULLP;
   MsmLteMeasMobilityStatParams  *mobStateParams = NULLP;
   MsmLteCellSib3CfgGrp          *dfltSib3 = NULLP;
 
   TRC2(smBldSib3Cfg);

   dfltSib3 = &lteeNodeBparams.lteCellSib3CfgGrp;
   mobStateParams = &dfltSib3->mobStateParams;
   lwrMng->t.protoCfg.t.cellCfg.t.addCell.cellCfgGrp[cfgGrpIndex]->grpType = WR_CELL_CFG_GRP_SIB3;
   sib3 = &(lwrMng->t.protoCfg.t.cellCfg.t.addCell.cellCfgGrp[cfgGrpIndex]->t.sib3CfgGrp);

   /*Updating the SIB3 parameters*/
   sib3->qHyst                               = lteeNodeBparams.lteCellSib3CfgGrp.qHyst;
   sib3->speedStateReselPres                 = WR_PRSNT;
   sib3->sfMedium                            = lteeNodeBparams.lteCellSib3CfgGrp.sfMedium;
   sib3->sfHigh                              = lteeNodeBparams.lteCellSib3CfgGrp.sfHigh;
   /*Updating the mobility state parameters*/
   sib3->mobStateParams.tEvalulation         = mobStateParams->tEvalulation;
   sib3->mobStateParams.tHystNormal          = mobStateParams->tHystNormal;
   sib3->mobStateParams.nCellChangeMedium    = mobStateParams->nCellChangeMedium;
   sib3->mobStateParams.nCellChangeHigh      = mobStateParams->nCellChangeHigh;

   /*  commenting out as these info will be filled from neighbour 
    *  configuration not from here */
   RETVOID;
} /* end of smBldSib3Cfg */
 
PRIVATE void smBldSib8Cfg
(
 LwrMngmt   *lwrMng,
 U8         cfgGrpIndex
)
{
   LwrCellSib8CfgGrp *sib8Cfg = NULLP;

   lwrMng->t.protoCfg.t.cellCfg.t.addCell.cellCfgGrp[cfgGrpIndex]->grpType =
      WR_CELL_CFG_GRP_SIB8;

   sib8Cfg = &(lwrMng->t.protoCfg.t.cellCfg.t.addCell.cellCfgGrp[cfgGrpIndex]->t.sib8CfgGrp);

   sib8Cfg->cdmaEutraTimeSync = lteeNodeBparams.cellSib8CfgGrp.cdmaEutraTimeSync;
   sib8Cfg->tReselection = lteeNodeBparams.cellSib8CfgGrp.tReselection;
   sib8Cfg->speedStatePres = lteeNodeBparams.cellSib8CfgGrp.speedStatePres;
   sib8Cfg->sfMedium = lteeNodeBparams.cellSib8CfgGrp.sfMedium;
   sib8Cfg->sfHigh = lteeNodeBparams.cellSib8CfgGrp.sfHigh;

   sib8Cfg->cdma1xrttParams.cdma1xrttRegParams.pres = wrSmDfltCdma1xrttRegSup;/*RRC Rel10 Upgrade*/
   /*wrSmDfltCdma1xrttRegSup is set to TRUE by default. configurability to be supported
	 in the future*/
   sib8Cfg->cdma1xrttParams.cdma1xrttRegParams.sid = lteeNodeBparams.cellSib8CfgGrp.cdma1xrttRegParams.sid;
   sib8Cfg->cdma1xrttParams.cdma1xrttRegParams.nid = lteeNodeBparams.cellSib8CfgGrp.cdma1xrttRegParams.nid;
   sib8Cfg->cdma1xrttParams.cdma1xrttRegParams.mulSID = lteeNodeBparams.cellSib8CfgGrp.cdma1xrttRegParams.mulSID;
   sib8Cfg->cdma1xrttParams.cdma1xrttRegParams.mulNID = lteeNodeBparams.cellSib8CfgGrp.cdma1xrttRegParams.mulNID;
   sib8Cfg->cdma1xrttParams.cdma1xrttRegParams.homeReg = lteeNodeBparams.cellSib8CfgGrp.cdma1xrttRegParams.homeReg;
   sib8Cfg->cdma1xrttParams.cdma1xrttRegParams.foreignSIDReg = lteeNodeBparams.cellSib8CfgGrp.cdma1xrttRegParams.foreignSIDReg;
   sib8Cfg->cdma1xrttParams.cdma1xrttRegParams.foreignNIDReg = lteeNodeBparams.cellSib8CfgGrp.cdma1xrttRegParams.foreignNIDReg;
   sib8Cfg->cdma1xrttParams.cdma1xrttRegParams.paramReg = lteeNodeBparams.cellSib8CfgGrp.cdma1xrttRegParams.paramReg;
   sib8Cfg->cdma1xrttParams.cdma1xrttRegParams.powerUpReg = lteeNodeBparams.cellSib8CfgGrp.cdma1xrttRegParams.powerUpReg;
   sib8Cfg->cdma1xrttParams.cdma1xrttRegParams.regPeriod = lteeNodeBparams.cellSib8CfgGrp.cdma1xrttRegParams.regPeriod;
   sib8Cfg->cdma1xrttParams.cdma1xrttRegParams.regZone = lteeNodeBparams.cellSib8CfgGrp.cdma1xrttRegParams.regZone;
   sib8Cfg->cdma1xrttParams.cdma1xrttRegParams.totalZone = lteeNodeBparams.cellSib8CfgGrp.cdma1xrttRegParams.totalZone;
   sib8Cfg->cdma1xrttParams.cdma1xrttRegParams.zoneTimer = lteeNodeBparams.cellSib8CfgGrp.cdma1xrttRegParams.zoneTimer;
   sib8Cfg->cdma1xrttParams.cdma1xrttRegParams.powerDownRegPres = lteeNodeBparams.cellSib8CfgGrp.cdma1xrttRegParams.powerDownReg;

   sib8Cfg->cdma1xrttParams.cdma1xrttACBarringCfg.pres = PRSNT_NODEF;
   sib8Cfg->cdma1xrttParams.cdma1xrttACBarringCfg.acBarring0to9_r9 = lteeNodeBparams.barringConfig1xRTTCfg.ac_Barring0to9_r9;
   sib8Cfg->cdma1xrttParams.cdma1xrttACBarringCfg.acBarring10_r9 = lteeNodeBparams.barringConfig1xRTTCfg.ac_Barring10_r9;
   sib8Cfg->cdma1xrttParams.cdma1xrttACBarringCfg.acBarring11_r9 = lteeNodeBparams.barringConfig1xRTTCfg.ac_Barring11_r9;
   sib8Cfg->cdma1xrttParams.cdma1xrttACBarringCfg.acBarring12_r9 = lteeNodeBparams.barringConfig1xRTTCfg.ac_Barring12_r9;
   sib8Cfg->cdma1xrttParams.cdma1xrttACBarringCfg.acBarring13_r9 = lteeNodeBparams.barringConfig1xRTTCfg.ac_Barring13_r9;
   sib8Cfg->cdma1xrttParams.cdma1xrttACBarringCfg.acBarring14_r9 = lteeNodeBparams.barringConfig1xRTTCfg.ac_Barring14_r9;
   sib8Cfg->cdma1xrttParams.cdma1xrttACBarringCfg.acBarring15_r9 = lteeNodeBparams.barringConfig1xRTTCfg.ac_Barring15_r9;
   sib8Cfg->cdma1xrttParams.cdma1xrttACBarringCfg.acBarringMsg_r9 = lteeNodeBparams.barringConfig1xRTTCfg.ac_BarringMsg_r9;
   sib8Cfg->cdma1xrttParams.cdma1xrttACBarringCfg.acBarringReg_r9 = lteeNodeBparams.barringConfig1xRTTCfg.ac_BarringReg_r9;
   sib8Cfg->cdma1xrttParams.cdma1xrttACBarringCfg.acBarringEmg_r9 = lteeNodeBparams.barringConfig1xRTTCfg.ac_BarringEmg_r9;

} /* end of smBldSib8Cfg */
 
/*
*
*       Fun:   smBldCellAnrCfg
*
*       Desc:  fill in cell anr Cfg for WR
*
*       Ret:   ROK
*
*       Notes: None
*              
*       File:  wr_smm_enbapp_cmnplatutils.c 
*
*/

PUBLIC void smBldCellAnrCfg
(
 LwrMngmt   *lwrMng,
 U8         cfgGrpIndex
)
{
   LwrCellAnrCfgGrp          *anrCfg;
   TRC2(smBldCellAnrCfg);

   anrCfg = &lwrMng->t.protoCfg.t.cellCfg.t.addCell.cellCfgGrp[cfgGrpIndex]->t.anrCfgGrp;

   lwrMng->t.protoCfg.t.cellCfg.t.addCell.cellCfgGrp[cfgGrpIndex]->grpType = WR_CELL_CFG_GRP_ANR;
   anrCfg->csfbTransTmrVal = wrSmDfltCsfbTmrVal;

   wrFillTknU32(&(anrCfg->epocTimerVal), lteeNodeBparams.lteFapControlParams.anrEpochPeriod);

   anrCfg->tricePeriodIntv    = lteeNodeBparams.lteFapControlParams.anrTricePeriod;
   anrCfg->anrCellValidAge = 
      lteeNodeBparams.lteFapControlParams.anrCellValidAge;
   wrSmDfltAnrCelValidAge = anrCfg->anrCellValidAge; 


    /*Removed DRX Flag */
   anrCfg->ueAnrDrxCfg.longDrxCycle =  lteeNodeBparams.lteFapControlParams.anrDrxLongCycle;  
   anrCfg->ueAnrDrxCfg.drxOnDurTmr  =  lteeNodeBparams.lteFapControlParams.anrDrxOnDurTmr;
   /* Fix for ccpu00132334 - To read DRX Inactivity timer for ANR*/
   anrCfg->ueAnrDrxCfg.inActivTmr   =  lteeNodeBparams.lteFapControlParams.anrDrxInactvTmr; 
   RETVOID;
} /* end of smBldCellAnrCfg */
 
/*
*
*       Fun:   smBldRabPolicy
*
*       Desc:  fill in rab policy for WR
*
*       Ret:   ROK
*
*       Notes: None
*              
*       File:  wr_smm_enbapp_cmnplatutils.c 
*
*/

PRIVATE Void smBldRabPolicy
(
 LwrMngmt   *lwrMng,
 U8         cfgGrpIndex
)
{
   LwrSrbPolicy        *srbPolicy = NULLP;
   MsmLteSrbPolicy     *msmSrbPolicy = NULLP;
   LwrRabPolicyCfgGrp  *rabPolicyCfg = NULLP;
   U8                  idx = 0;
   U8                  qciIdx = 0;

   TRC2(smBldRabPolicy);

   lwrMng->t.protoCfg.t.cellCfg.t.addCell.cellCfgGrp[cfgGrpIndex]->grpType =
      WR_CELL_CFG_GRP_RAB_PARAMS;
   lwrMng->t.protoCfg.t.cellCfg.t.addCell.cellCfgGrp[cfgGrpIndex]->t.rabPolicyCfgGrp.numSrbPolicies = 2; 
   for(idx = 1; idx < MSM_MAX_SRBS; idx++)
   {
      srbPolicy = &(lwrMng->t.protoCfg.t.cellCfg.t.addCell.cellCfgGrp[cfgGrpIndex]->t.rabPolicyCfgGrp.srbPolicy[idx-1]);
      msmSrbPolicy = &(lteeNodeBparams.lteRabPolicyCfgGrp.srbPolicy[idx]);

      srbPolicy->srbId = msmSrbPolicy->srbId;
      srbPolicy->rlcInfo.srbId = msmSrbPolicy->rlcInfo.srbId;
      srbPolicy->rlcInfo.tpRetransmit = msmSrbPolicy->rlcInfo.tpRetransmit;
      srbPolicy->rlcInfo.pollPdu = msmSrbPolicy->rlcInfo.pollPdu;
      srbPolicy->rlcInfo.pollByte = msmSrbPolicy->rlcInfo.pollByte;
      srbPolicy->rlcInfo.maxRetransThresh = msmSrbPolicy->rlcInfo.maxRetransThresh;
      srbPolicy->rlcInfo.timerReorderingDl = msmSrbPolicy->rlcInfo.timerReordering;
      srbPolicy->rlcInfo.timerReorderingUl = msmSrbPolicy->rlcInfo.timerReordering;
      srbPolicy->rlcInfo.timerStatusProhibit = msmSrbPolicy->rlcInfo.timerStatusProhibit;
      srbPolicy->lchInfo.srbId = msmSrbPolicy->srbId; 
      srbPolicy->lchInfo.pbr                = wrSmDfltPrioInfoTbl.srb[idx-1].pbr;
      srbPolicy->lchInfo.bucketSizeDuration = wrSmDfltPrioInfoTbl.srb[idx-1].bucketSizeDur;
      srbPolicy->lchInfo.lchGrpId           = wrSmDfltPrioInfoTbl.srb[idx-1].lchGrp;
      srbPolicy->lchInfo.priority           = wrSmDfltPrioInfoTbl.srb[idx-1].lchPriority;
   }
      rabPolicyCfg= &(lwrMng->t.protoCfg.t.cellCfg.t.addCell.cellCfgGrp[cfgGrpIndex]->t.rabPolicyCfgGrp);
      rabPolicyCfg->arpForEmergencyServ = lteeNodeBparams.lteAddCellCfg.emergencyServicePriorityLevel;
      rabPolicyCfg->numDrbPolicies=WR_MAX_QCI;
      smCfgCb.isDscpEnable = lteeNodeBparams.lteRabPolicyCfgGrp.isDscpEnable; 
     for(qciIdx=0; qciIdx<rabPolicyCfg->numDrbPolicies; qciIdx++)
     {
        if(smCfgCb.isDscpEnable==TRUE)
        {
           rabPolicyCfg->drbPolicy[qciIdx].qciInfo.qciSupported=TRUE;
           rabPolicyCfg->drbPolicy[qciIdx].qci=(qciIdx+1);
           rabPolicyCfg->drbPolicy[qciIdx].dscpInfo.qci=(qciIdx+1);
           rabPolicyCfg->drbPolicy[qciIdx].dscpInfo.dscp= lteeNodeBparams.lteRabPolicyCfgGrp.drbPolicy[qciIdx].dscpInfo.dscp;
        }
        else
        {
           rabPolicyCfg->drbPolicy[qciIdx].qciInfo.qciSupported=TRUE;
           rabPolicyCfg->drbPolicy[qciIdx].qci=(qciIdx+1);
           rabPolicyCfg->drbPolicy[qciIdx].dscpInfo.qci=(qciIdx+1);
           rabPolicyCfg->drbPolicy[qciIdx].dscpInfo.dscp= 0;
        } 
     }

      rabPolicyCfg->drbPolicy[0].rlcInfo.rlcMode = WR_RLC_UM_MODE;
      rabPolicyCfg->drbPolicy[0].rlcInfo.ulSnFieldLength = NhuSN_FieldLengthsize5Enum;
      rabPolicyCfg->drbPolicy[0].rlcInfo.dlSnFieldLength = NhuSN_FieldLengthsize10Enum;
      rabPolicyCfg->drbPolicy[0].rlcInfo.ulTimerReordering = wrSmDfltUlDrbtReordring;
      rabPolicyCfg->drbPolicy[0].rlcInfo.dlTimerReordering = wrSmDfltDlDrbtReordring;
      rabPolicyCfg->drbPolicy[0].pdcpInfo.umSnSize = NhuPDCP_Configrlc_UMpdcp_SN_Sizelen7bitsEnum;
      rabPolicyCfg->drbPolicy[0].pdcpInfo.discardTimer = NhuPDCP_ConfigdiscardTimerms100Enum;
      rabPolicyCfg->drbPolicy[0].pdcpInfo.statRprtReqrd = TRUE;
   rabPolicyCfg->drbPolicy[0].lchInfo.lchPriority        = wrSmDfltPrioInfoTbl.drb[1].lchPriority;
   rabPolicyCfg->drbPolicy[0].lchInfo.prioritisedBitRate = wrSmDfltPrioInfoTbl.drb[1].pbr;
   rabPolicyCfg->drbPolicy[0].lchInfo.bcketSizeDurn      = wrSmDfltPrioInfoTbl.drb[1].bucketSizeDur;

      rabPolicyCfg->drbPolicy[1].rlcInfo.rlcMode = WR_RLC_UM_MODE;
      rabPolicyCfg->drbPolicy[1].rlcInfo.ulSnFieldLength = NhuSN_FieldLengthsize5Enum;
      rabPolicyCfg->drbPolicy[1].rlcInfo.dlSnFieldLength = NhuSN_FieldLengthsize10Enum;
      rabPolicyCfg->drbPolicy[1].rlcInfo.ulTimerReordering = wrSmDfltUlDrbtReordring;
      rabPolicyCfg->drbPolicy[1].rlcInfo.dlTimerReordering = wrSmDfltDlDrbtReordring;
      rabPolicyCfg->drbPolicy[1].pdcpInfo.umSnSize = NhuPDCP_Configrlc_UMpdcp_SN_Sizelen7bitsEnum;
      rabPolicyCfg->drbPolicy[1].pdcpInfo.discardTimer = NhuPDCP_ConfigdiscardTimerms150Enum;
      rabPolicyCfg->drbPolicy[1].pdcpInfo.statRprtReqrd = TRUE;
   rabPolicyCfg->drbPolicy[1].lchInfo.lchPriority        = wrSmDfltPrioInfoTbl.drb[2].lchPriority;
   rabPolicyCfg->drbPolicy[1].lchInfo.prioritisedBitRate = wrSmDfltPrioInfoTbl.drb[2].pbr;
   rabPolicyCfg->drbPolicy[1].lchInfo.bcketSizeDurn      = wrSmDfltPrioInfoTbl.drb[2].bucketSizeDur;

      rabPolicyCfg->drbPolicy[2].rlcInfo.rlcMode = WR_RLC_UM_MODE;
      rabPolicyCfg->drbPolicy[2].rlcInfo.ulSnFieldLength = NhuSN_FieldLengthsize5Enum;
      rabPolicyCfg->drbPolicy[2].rlcInfo.dlSnFieldLength = NhuSN_FieldLengthsize10Enum;
      rabPolicyCfg->drbPolicy[2].rlcInfo.ulTimerReordering = wrSmDfltUlDrbtReordring;
      rabPolicyCfg->drbPolicy[2].rlcInfo.dlTimerReordering = wrSmDfltDlDrbtReordring;
      rabPolicyCfg->drbPolicy[2].pdcpInfo.umSnSize = NhuPDCP_Configrlc_UMpdcp_SN_Sizelen7bitsEnum;
      rabPolicyCfg->drbPolicy[2].pdcpInfo.discardTimer = NhuPDCP_ConfigdiscardTimerms50Enum;
      rabPolicyCfg->drbPolicy[2].pdcpInfo.statRprtReqrd = TRUE;
   rabPolicyCfg->drbPolicy[2].lchInfo.lchPriority        = wrSmDfltPrioInfoTbl.drb[3].lchPriority;
   rabPolicyCfg->drbPolicy[2].lchInfo.prioritisedBitRate = wrSmDfltPrioInfoTbl.drb[3].pbr;
   rabPolicyCfg->drbPolicy[2].lchInfo.bcketSizeDurn      = wrSmDfltPrioInfoTbl.drb[3].bucketSizeDur;
   
      rabPolicyCfg->drbPolicy[3].rlcInfo.rlcMode = WR_RLC_UM_MODE;
      rabPolicyCfg->drbPolicy[3].rlcInfo.ulSnFieldLength = NhuSN_FieldLengthsize5Enum;
      rabPolicyCfg->drbPolicy[3].rlcInfo.dlSnFieldLength = NhuSN_FieldLengthsize10Enum;
      rabPolicyCfg->drbPolicy[3].rlcInfo.ulTimerReordering = wrSmDfltUlDrbtReordring;
      rabPolicyCfg->drbPolicy[3].rlcInfo.dlTimerReordering = wrSmDfltDlDrbtReordring;
      rabPolicyCfg->drbPolicy[3].pdcpInfo.umSnSize = NhuPDCP_Configrlc_UMpdcp_SN_Sizelen7bitsEnum;
      rabPolicyCfg->drbPolicy[3].pdcpInfo.discardTimer = NhuPDCP_ConfigdiscardTimerms300Enum;
      rabPolicyCfg->drbPolicy[3].pdcpInfo.statRprtReqrd = TRUE;
   rabPolicyCfg->drbPolicy[3].lchInfo.lchPriority        = wrSmDfltPrioInfoTbl.drb[4].lchPriority;
   rabPolicyCfg->drbPolicy[3].lchInfo.prioritisedBitRate = wrSmDfltPrioInfoTbl.drb[4].pbr;
   rabPolicyCfg->drbPolicy[3].lchInfo.bcketSizeDurn      = wrSmDfltPrioInfoTbl.drb[4].bucketSizeDur;


      rabPolicyCfg->drbPolicy[4].rlcInfo.rlcMode = WR_RLC_AM_MODE;
      rabPolicyCfg->drbPolicy[4].rlcInfo.ulTimerStatusProhibit = NhuT_StatusProhibitms10Enum; 
      rabPolicyCfg->drbPolicy[4].rlcInfo.ulTimerReordering = wrSmDfltUlDrbtReordring;
      rabPolicyCfg->drbPolicy[4].rlcInfo.ulPollByte = NhuPollBytekBinfinityEnum;
      rabPolicyCfg->drbPolicy[4].rlcInfo.ulPollPdu = NhuPollPDUp16Enum; 
      rabPolicyCfg->drbPolicy[4].rlcInfo.ulTpRetransmit = NhuT_PollRetransmitms400Enum;
      rabPolicyCfg->drbPolicy[4].rlcInfo.ulMaxRetransThresh = NhuUL_AM_RLCmaxRetxThresholdt8Enum;
      rabPolicyCfg->drbPolicy[4].rlcInfo.dlTimerStatusProhibit = NhuT_StatusProhibitms35Enum;
      rabPolicyCfg->drbPolicy[4].rlcInfo.dlTimerReordering = wrSmDfltDlDrbtReordring; 
      rabPolicyCfg->drbPolicy[4].rlcInfo.dlPollByte = NhuPollBytekBinfinityEnum; 
      rabPolicyCfg->drbPolicy[4].rlcInfo.dlPollPdu = NhuPollPDUp8Enum; 
      rabPolicyCfg->drbPolicy[4].rlcInfo.dlTpRetransmit = NhuT_PollRetransmitms400Enum;
      rabPolicyCfg->drbPolicy[4].rlcInfo.dlMaxRetransThresh = NhuPDCP_ConfigdiscardTimerinfinityEnum; 
      rabPolicyCfg->drbPolicy[4].pdcpInfo.discardTimer = NhuPDCP_ConfigdiscardTimerinfinityEnum;
      rabPolicyCfg->drbPolicy[4].pdcpInfo.statRprtReqrd = TRUE;
   rabPolicyCfg->drbPolicy[4].lchInfo.lchPriority        = wrSmDfltPrioInfoTbl.drb[5].lchPriority;
   rabPolicyCfg->drbPolicy[4].lchInfo.prioritisedBitRate = wrSmDfltPrioInfoTbl.drb[5].pbr;
   rabPolicyCfg->drbPolicy[4].lchInfo.bcketSizeDurn      = wrSmDfltPrioInfoTbl.drb[5].bucketSizeDur;

      rabPolicyCfg->drbPolicy[5].rlcInfo.rlcMode = WR_RLC_AM_MODE;
      rabPolicyCfg->drbPolicy[5].rlcInfo.ulTimerStatusProhibit = NhuT_StatusProhibitms10Enum; 
      rabPolicyCfg->drbPolicy[5].rlcInfo.ulTimerReordering = NhuT_Reorderingms30Enum;
      rabPolicyCfg->drbPolicy[5].rlcInfo.ulPollByte = NhuPollBytekBinfinityEnum;
      rabPolicyCfg->drbPolicy[5].rlcInfo.ulPollPdu = NhuPollPDUp16Enum; 
      rabPolicyCfg->drbPolicy[5].rlcInfo.ulTpRetransmit = NhuT_PollRetransmitms400Enum;
      rabPolicyCfg->drbPolicy[5].rlcInfo.ulMaxRetransThresh = NhuUL_AM_RLCmaxRetxThresholdt8Enum;
      rabPolicyCfg->drbPolicy[5].rlcInfo.dlTimerStatusProhibit = NhuT_StatusProhibitms40Enum;
      rabPolicyCfg->drbPolicy[5].rlcInfo.dlTimerReordering = wrSmDfltDlDrbtReordring; 
      rabPolicyCfg->drbPolicy[5].rlcInfo.dlPollByte = NhuPollBytekBinfinityEnum; 
      rabPolicyCfg->drbPolicy[5].rlcInfo.dlPollPdu = NhuPollPDUp16Enum; 
      rabPolicyCfg->drbPolicy[5].rlcInfo.dlTpRetransmit = NhuT_PollRetransmitms400Enum;
      rabPolicyCfg->drbPolicy[5].rlcInfo.dlMaxRetransThresh = NhuPDCP_ConfigdiscardTimerinfinityEnum; 
      rabPolicyCfg->drbPolicy[5].pdcpInfo.discardTimer = NhuPDCP_ConfigdiscardTimerinfinityEnum;
      rabPolicyCfg->drbPolicy[5].pdcpInfo.statRprtReqrd = TRUE;
   rabPolicyCfg->drbPolicy[5].lchInfo.lchPriority        = wrSmDfltPrioInfoTbl.drb[6].lchPriority;
   rabPolicyCfg->drbPolicy[5].lchInfo.prioritisedBitRate = wrSmDfltPrioInfoTbl.drb[6].pbr;
   rabPolicyCfg->drbPolicy[5].lchInfo.bcketSizeDurn      = wrSmDfltPrioInfoTbl.drb[6].bucketSizeDur;

      rabPolicyCfg->drbPolicy[6].rlcInfo.rlcMode = WR_RLC_AM_MODE;
      rabPolicyCfg->drbPolicy[6].rlcInfo.ulTimerStatusProhibit = NhuT_StatusProhibitms35Enum; 
      rabPolicyCfg->drbPolicy[6].rlcInfo.ulTimerReordering = wrSmDfltUlDrbtReordring;
      rabPolicyCfg->drbPolicy[6].rlcInfo.ulPollByte = NhuPollBytekBinfinityEnum;
      rabPolicyCfg->drbPolicy[6].rlcInfo.ulPollPdu = NhuPollPDUp16Enum; 
      rabPolicyCfg->drbPolicy[6].rlcInfo.ulTpRetransmit = NhuT_PollRetransmitms400Enum;
      rabPolicyCfg->drbPolicy[6].rlcInfo.ulMaxRetransThresh = NhuUL_AM_RLCmaxRetxThresholdt8Enum;
      rabPolicyCfg->drbPolicy[6].rlcInfo.dlTimerStatusProhibit = NhuT_StatusProhibitms5Enum;
      rabPolicyCfg->drbPolicy[6].rlcInfo.dlTimerReordering = wrSmDfltDlDrbtReordring; 
      rabPolicyCfg->drbPolicy[6].rlcInfo.dlPollByte = NhuPollBytekBinfinityEnum; 
      rabPolicyCfg->drbPolicy[6].rlcInfo.dlPollPdu = NhuPollPDUp16Enum; 
      rabPolicyCfg->drbPolicy[6].rlcInfo.dlTpRetransmit = NhuT_PollRetransmitms400Enum;
      rabPolicyCfg->drbPolicy[6].rlcInfo.dlMaxRetransThresh = NhuUL_AM_RLCmaxRetxThresholdt8Enum; 
      rabPolicyCfg->drbPolicy[6].pdcpInfo.discardTimer = NhuPDCP_ConfigdiscardTimerinfinityEnum;
      rabPolicyCfg->drbPolicy[6].pdcpInfo.statRprtReqrd = TRUE;
   rabPolicyCfg->drbPolicy[6].lchInfo.lchPriority        = wrSmDfltPrioInfoTbl.drb[7].lchPriority;
   rabPolicyCfg->drbPolicy[6].lchInfo.prioritisedBitRate = wrSmDfltPrioInfoTbl.drb[7].pbr;
   rabPolicyCfg->drbPolicy[6].lchInfo.bcketSizeDurn      = wrSmDfltPrioInfoTbl.drb[7].bucketSizeDur;

      rabPolicyCfg->drbPolicy[7].rlcInfo.rlcMode = WR_RLC_AM_MODE;
      rabPolicyCfg->drbPolicy[7].rlcInfo.ulTimerStatusProhibit = NhuT_StatusProhibitms35Enum; 
      rabPolicyCfg->drbPolicy[7].rlcInfo.ulTimerReordering = wrSmDfltUlDrbtReordring;
      rabPolicyCfg->drbPolicy[7].rlcInfo.ulPollByte = NhuPollBytekBinfinityEnum;
      rabPolicyCfg->drbPolicy[7].rlcInfo.ulPollPdu = NhuPollPDUp16Enum; 
      rabPolicyCfg->drbPolicy[7].rlcInfo.ulTpRetransmit = NhuT_PollRetransmitms400Enum;
      rabPolicyCfg->drbPolicy[7].rlcInfo.ulMaxRetransThresh = NhuUL_AM_RLCmaxRetxThresholdt8Enum;
      rabPolicyCfg->drbPolicy[7].rlcInfo.dlTimerStatusProhibit = NhuT_StatusProhibitms35Enum;
      rabPolicyCfg->drbPolicy[7].rlcInfo.dlTimerReordering = wrSmDfltDlDrbtReordring; 
      rabPolicyCfg->drbPolicy[7].rlcInfo.dlPollByte = NhuPollBytekBinfinityEnum; 
      rabPolicyCfg->drbPolicy[7].rlcInfo.dlPollPdu = NhuPollPDUp16Enum; 
      rabPolicyCfg->drbPolicy[7].rlcInfo.dlTpRetransmit = NhuT_PollRetransmitms400Enum;
      rabPolicyCfg->drbPolicy[7].rlcInfo.dlMaxRetransThresh = NhuUL_AM_RLCmaxRetxThresholdt8Enum; 
      rabPolicyCfg->drbPolicy[7].pdcpInfo.discardTimer = NhuPDCP_ConfigdiscardTimerinfinityEnum;
      rabPolicyCfg->drbPolicy[7].pdcpInfo.statRprtReqrd = TRUE;
   rabPolicyCfg->drbPolicy[7].lchInfo.lchPriority        = wrSmDfltPrioInfoTbl.drb[8].lchPriority;
   rabPolicyCfg->drbPolicy[7].lchInfo.prioritisedBitRate = wrSmDfltPrioInfoTbl.drb[8].pbr;
   rabPolicyCfg->drbPolicy[7].lchInfo.bcketSizeDurn      = wrSmDfltPrioInfoTbl.drb[8].bucketSizeDur;

      rabPolicyCfg->drbPolicy[8].rlcInfo.rlcMode = WR_RLC_AM_MODE;
      rabPolicyCfg->drbPolicy[8].rlcInfo.ulTimerStatusProhibit = NhuT_StatusProhibitms35Enum; 
      rabPolicyCfg->drbPolicy[8].rlcInfo.ulTimerReordering = wrSmDfltUlDrbtReordring;
      rabPolicyCfg->drbPolicy[8].rlcInfo.ulPollByte = NhuPollBytekBinfinityEnum;
      rabPolicyCfg->drbPolicy[8].rlcInfo.ulPollPdu = NhuPollPDUp16Enum; 
      rabPolicyCfg->drbPolicy[8].rlcInfo.ulTpRetransmit = NhuT_PollRetransmitms400Enum;
      rabPolicyCfg->drbPolicy[8].rlcInfo.ulMaxRetransThresh = NhuUL_AM_RLCmaxRetxThresholdt8Enum;
      rabPolicyCfg->drbPolicy[8].rlcInfo.dlTimerStatusProhibit = NhuT_StatusProhibitms35Enum;
      rabPolicyCfg->drbPolicy[8].rlcInfo.dlTimerReordering = wrSmDfltDlDrbtReordring; 
      rabPolicyCfg->drbPolicy[8].rlcInfo.dlPollByte = NhuPollBytekBinfinityEnum; 
      rabPolicyCfg->drbPolicy[8].rlcInfo.dlPollPdu = NhuPollPDUp16Enum; 
      rabPolicyCfg->drbPolicy[8].rlcInfo.dlTpRetransmit = NhuT_PollRetransmitms400Enum;
      rabPolicyCfg->drbPolicy[8].rlcInfo.dlMaxRetransThresh = NhuUL_AM_RLCmaxRetxThresholdt8Enum; 
      rabPolicyCfg->drbPolicy[8].pdcpInfo.discardTimer = NhuPDCP_ConfigdiscardTimerinfinityEnum;
      rabPolicyCfg->drbPolicy[8].pdcpInfo.statRprtReqrd = TRUE;
   rabPolicyCfg->drbPolicy[8].lchInfo.lchPriority        = wrSmDfltPrioInfoTbl.drb[9].lchPriority;
   rabPolicyCfg->drbPolicy[8].lchInfo.prioritisedBitRate = wrSmDfltPrioInfoTbl.drb[9].pbr;
   rabPolicyCfg->drbPolicy[8].lchInfo.bcketSizeDurn      = wrSmDfltPrioInfoTbl.drb[9].bucketSizeDur;

   RETVOID;
} /* end of smBldRabPolicy */
 
/*
*
*       Fun:   smBldCellSibSchCfg
*
*       Desc:  fill in sib scheduling cfg for WR
*
*       Ret:   ROK
*
*       Notes: None
*              
*       File:  wr_smm_enbapp_cmnplatutils.c 
*
*/

PUBLIC void smBldCellSibSchCfg
(
 LwrMngmt   *lwrMng,
 U8         cfgGrpIndex
)
{
   LwrCellSibSchedCfgGrp *sibSchCfg = NULLP;

   TRC2(smBldCellSibSchCfg);

   lwrMng->t.protoCfg.t.cellCfg.t.addCell.cellCfgGrp[cfgGrpIndex]->grpType =
      WR_CELL_CFG_GRP_SIB_SCHED;

   sibSchCfg = &(lwrMng->t.protoCfg.t.cellCfg.t.addCell.cellCfgGrp[cfgGrpIndex]->t.sibSchedCfgGrp);
   /* Populate the SIB periodicites */
   sibSchCfg->sib2Period = WR_SIB2_PERIOD; 
   sibSchCfg->sib3Period = WR_SIB3_PERIOD;
   sibSchCfg->sib4Period = WR_SIB4_PERIOD; 
   /* If the number of Frequency entries is >1(the first entry is alwys Intra frequency), 
    */
   sibSchCfg->sib5Period = WR_SIB5_PERIOD; 

   if ((lteeNodeBparams.lteNeighFreqCfg.numUtraFreq > 0) || (lteeNodeBparams.lteNeighFreqCfg.numUtraTddFreq > 0))
   {
      sibSchCfg->sib6PeriodValid = TRUE;   
      sibSchCfg->sib6Period = WR_SIB6_PERIOD; 
   }
   else
   {
      sibSchCfg->sib6PeriodValid = FALSE;
   }
   /* Fix for CR ccpu00142792 */
   if(lteeNodeBparams.lteNeighFreqCfg.numGeranFreq > 0)
   {
      sibSchCfg->sib7PeriodValid = TRUE;
      sibSchCfg->sib7Period = WR_SIB7_PERIOD;
   }
   else
   {
      sibSchCfg->sib7PeriodValid = FALSE;
   }
   sibSchCfg->sib9Period = WR_SIB9_PERIOD; 
   if(lteeNodeBparams.lteNeighCellCfg.numCdma1xCells > 0)
   {
      sibSchCfg->sib8PeriodValid = TRUE;
      sibSchCfg->sib8Period = WR_SIB8_PERIOD;
   }
   else
   {
      sibSchCfg->sib8PeriodValid = FALSE;
   }
   sibSchCfg->siWindow = wrSmDfltSiWinSize;
   sibSchCfg->sib10ManualPrdPres = TRUE;
   sibSchCfg->sib10RepetPeriod = wrSmDfltSib10RptPrd;
   sibSchCfg->sib10NumBroadCast = wrSmDfltSib10NumBroadCast;
   sibSchCfg->sib11WarnSegSize = wrSmDfltWarnSegSize;
   RETVOID;
} /* end of smBldCellSibSchCfg */

/*
*
*       Fun:   smBldCellEaidCfg
*
*       Desc:  fill in EAID cfg for WR
*
*       Ret:   ROK
*
*       Notes: None
*              
*       File:  wr_smm_enbapp_cmnplatutils.c 
*
*/

PUBLIC void smBldCellEaidCfg
(
 LwrMngmt   *lwrMng,
 U8         cfgGrpIndex
)
{
   LwrCellEAIDCfgGrp         *eaidCfgGrp = NULLP;
   U8                        idx = 0; 

   TRC2(smBldCellEaidCfg);

   lwrMng->t.protoCfg.t.cellCfg.t.addCell.cellCfgGrp[cfgGrpIndex]->grpType =
      WR_CELL_CFG_GRP_EAID;

   eaidCfgGrp = &(lwrMng->t.protoCfg.t.cellCfg.t.addCell.cellCfgGrp[cfgGrpIndex]->t.eaidCfgGrp);

   for (idx = 0; idx < LWR_SM_MAX_EAIDS; idx++)
   {
      eaidCfgGrp->eaid[idx].pres = TRUE;
      eaidCfgGrp->eaid[idx].val = lteeNodeBparams.lteCellEAIDCfgGrp.eaid[idx];
      /* eaidCfgGrp->eaid[idx].val = tempEaid++; */
   }
   RETVOID;
} /* end of smBldCellEaidCfG */

/*
*
*       Fun:  smBldMeasCellCfg 
*
*       Desc:  fill in measurement cfg for WR
*
*       Ret:   ROK
*
*       Notes: None
*              
*       File:  wr_smm_enbapp_cmnplatutils.c 
*
*/

PRIVATE Void smBldMeasCellCfg
(
 LwrMngmt   *lwrMng,
 U8         cfgGrpIndex
)
{
   LwrCellMeasCfgGrp          *measCfg = NULLP;
   MsmLteMeasEvntRptCfg       *dflMeasCfg = NULLP;
   MsmLteMeasEvntA3RptCfg     *measEvtA3Cfg = NULLP;
   MsmLteMeasUtraEvntB2RptCfg *b2UtraMeasCfg = NULLP; 
   MsmLteMeasUtraEvntB2RptCfg *b2UtraTddMeasCfg = NULLP; 
   MsmLteMeasGeranEvntB2RptCfg *b2GeranMeasCfg = NULLP; 
   MsmMeasEutraPeridRptCfg    *eutraMeasPeridCfg = NULLP;
   MsmMeasUtraPeriodRptCfg    *utraMeasPeridCfg  = NULLP;
   MsmLteMeasEvntA5RptCfg     *measEvtA5Cfg = NULLP;
   LwrMeasEvntA3RptCfg       *a3Cfg;
   LwrMeasEvntA5RptCfg       *a5Cfg;
   LwrMeasEvntRptCfg         *eventCfg;
   LwrMeasEutraPeridRptCfg   *eutraPerd;
   LwrMeasUtraPeriodRptCfg   *utraPerd;
   LwrMeasUtraEvntB2RptCfg   *b2UtraCfg; 
   LwrMeasUtraEvntB2RptCfg   *b2UtraTddCfg; 
   LwrMeasGeranEvntB2RptCfg   *b2GeranCfg = NULLP; 
   LwrMeasGERANPeridRptCfg   *geranPrdCfg;
   /*S8                         offset;*/
   /* Fix for CR - ccpu00137183  */
   MsmLteMeasMobilityStatParams  *mobStateParams = NULLP;


   TRC2(smBldMeasCellCfg);

   /* Fix for CR - ccpu00137183  */
   mobStateParams = &lteeNodeBparams.lteCellSib3CfgGrp.mobStateParams;


   lwrMng->t.protoCfg.t.cellCfg.t.addCell.cellCfgGrp[cfgGrpIndex]->grpType =
      WR_CELL_CFG_GRP_MEAS;

   measCfg = &(lwrMng->t.protoCfg.t.cellCfg.t.addCell.cellCfgGrp[cfgGrpIndex]->t.measCfgGrp);
   /*For all variants in every event report configuraitons i.e. RSRP or
    * RSRQ, currently OAM is filling the same values.*/
   eventCfg = &measCfg->measA1EvtRptCfg[WR_MEAS_EVENTA1_HO_RSRP];

   dflMeasCfg = &lteeNodeBparams.lteCellMeasCfgGrp.measA1EvtRptCfg[0];
   /* configure Event A1 */
   eventCfg->isPres.pres    = PRSNT_NODEF;
   eventCfg->reason         = wrSmDfltReason;
   eventCfg->threshold_RSRP =  dflMeasCfg->threshold_RSRP;
   eventCfg->threshold_RSRQ =  dflMeasCfg->threshold_RSRQ;
   eventCfg->hysteresis     =  dflMeasCfg->hysteresis;
   eventCfg->timeToTrigger  =  dflMeasCfg->timeToTrigger;
   eventCfg->triggerQty     =  dflMeasCfg->triggerQty;
   eventCfg->reportQty      =  dflMeasCfg->reportQty;
   eventCfg->maxRptCells    =  dflMeasCfg->maxRptCells;
   eventCfg->reportInterval =  dflMeasCfg->reportInterval;
   eventCfg->reportAmount   =  dflMeasCfg->reportAmount;

   /* configure Event A2 */
   dflMeasCfg = &lteeNodeBparams.lteCellMeasCfgGrp.measA2EvtRptCfg[0];
   eventCfg = &measCfg->measA2EvtRptCfg[WR_MEAS_EVENTA2_HO_RSRP];

   eventCfg->isPres.pres = PRSNT_NODEF;
   eventCfg->reason = wrSmDfltReason;
   eventCfg->threshold_RSRP = dflMeasCfg->threshold_RSRP;
   eventCfg->threshold_RSRQ = dflMeasCfg->threshold_RSRQ;
   eventCfg->hysteresis     = dflMeasCfg->hysteresis;
   eventCfg->timeToTrigger  = dflMeasCfg->timeToTrigger;
   eventCfg->triggerQty     = dflMeasCfg->triggerQty;
   eventCfg->reportQty      = dflMeasCfg->reportQty;
   eventCfg->maxRptCells    = dflMeasCfg->maxRptCells;
   eventCfg->reportInterval = dflMeasCfg->reportInterval;
   eventCfg->reportAmount   = dflMeasCfg->reportAmount;

   /* configure Event A4 */
   eventCfg = &measCfg->measA4EvtRptCfg[WR_MEAS_EVENTA4_ANR_SPECIFIC];

   eventCfg->isPres.pres = PRSNT_NODEF;
   eventCfg->reason = wrSmDfltReason;
   eventCfg->threshold_RSRP = wrSmDfltThresRSRP1;
   eventCfg->threshold_RSRQ = wrSmDfltThresRSRQ1;
   eventCfg->hysteresis = wrSmDfltHysteresis;
   eventCfg->timeToTrigger = wrSmDfltTimeToTrig;
   eventCfg->triggerQty = wrSmDfltTrigQty;
   eventCfg->reportQty = wrSmDfltRptQty;
   eventCfg->maxRptCells = wrSmDfltMaxRptCells;
   eventCfg->reportInterval = wrSmDfltRptInterval;
   eventCfg->reportAmount = wrSmDfltRptAmount;

   /* configure Event A3 - intra */
   measEvtA3Cfg = &lteeNodeBparams.lteCellMeasCfgGrp.measEvtA3Cfg[MSM_MEAS_EVENTA3_INTRA_FREQ_HO];
   a3Cfg = &measCfg->measEvtA3Cfg[WR_MEAS_EVENTA3_INTRA_FREQ_HO];
   a3Cfg->isPres.pres = PRSNT_NODEF;
   a3Cfg->reason = wrSmDfltReason;
   a3Cfg->offset = measEvtA3Cfg->offset;
   a3Cfg->reportOnLeave = wrSmDfltRptOnLeave;
   a3Cfg->hysteresis = measEvtA3Cfg->hysteresis;
   a3Cfg->timeToTrigger = measEvtA3Cfg->timeToTrigger;
   a3Cfg->triggerQty = measEvtA3Cfg->triggerQty;
   a3Cfg->reportQty = measEvtA3Cfg->reportQty;
   a3Cfg->maxRptCells = measEvtA3Cfg->maxRptCells;
   a3Cfg->reportInterval = measEvtA3Cfg->reportInterval;
   a3Cfg->reportAmount = measEvtA3Cfg->reportAmount;
   
   /* configure Event A3 - inter */
   /* ccpu00133426 - inter freq ho requirement - start*/
   measEvtA3Cfg = &lteeNodeBparams.lteCellMeasCfgGrp.measEvtA3Cfg[MSM_MEAS_EVENTA3_INTER_FREQ_HO_RSRP];
   a3Cfg = &measCfg->measEvtA3Cfg[WR_MEAS_EVENTA3_INTER_FREQ_HO_RSRP];
   a3Cfg->isPres.pres = PRSNT_NODEF;
   a3Cfg->reason = wrSmDfltReason;
   a3Cfg->offset = measEvtA3Cfg->offset;
   a3Cfg->reportOnLeave = wrSmDfltRptOnLeave;
   a3Cfg->hysteresis = measEvtA3Cfg->hysteresis;
   a3Cfg->timeToTrigger = measEvtA3Cfg->timeToTrigger;
   a3Cfg->triggerQty = measEvtA3Cfg->triggerQty;
   a3Cfg->reportQty = measEvtA3Cfg->reportQty;
   a3Cfg->maxRptCells = measEvtA3Cfg->maxRptCells;
   a3Cfg->reportInterval = measEvtA3Cfg->reportInterval;
   a3Cfg->reportAmount = measEvtA3Cfg->reportAmount;
   /* ccpu00133426 - inter freq ho requirement - end*/

   /*Event A3 for ANR */
   measEvtA3Cfg = &lteeNodeBparams.lteCellMeasCfgGrp.measEvtA3Cfg[MSM_MEAS_EVENTA3_INTRA_FREQ_ANR];
   a3Cfg = &measCfg->measEvtA3Cfg[WR_MEAS_EVENTA3_INTRA_FREQ_ANR];
   a3Cfg->isPres.pres = PRSNT_NODEF;
   a3Cfg->reason = wrSmDfltReason;
   a3Cfg->offset = measEvtA3Cfg->offset;
   a3Cfg->reportOnLeave = wrSmDfltRptOnLeave;
   a3Cfg->hysteresis = measEvtA3Cfg->hysteresis;
   a3Cfg->timeToTrigger = measEvtA3Cfg->timeToTrigger;
   a3Cfg->triggerQty = measEvtA3Cfg->triggerQty;
   a3Cfg->reportQty = measEvtA3Cfg->reportQty;
   a3Cfg->maxRptCells = measEvtA3Cfg->maxRptCells;
   a3Cfg->reportInterval = measEvtA3Cfg->reportInterval;
   a3Cfg->reportAmount = measEvtA3Cfg->reportAmount;

   /* configure Event A5 */
   a5Cfg = &measCfg->measEvtA5Cfg[WR_MEAS_EVENTA5_INTRA_FREQ_HO];
   measEvtA5Cfg = &lteeNodeBparams.lteCellMeasCfgGrp.measEvtA5Cfg[MSM_MEAS_EVENTA5_INTRA_FREQ_HO];
   /* configure Intra Event A5 */
   a5Cfg->isPres.pres = PRSNT_NODEF;
   a5Cfg->reason = wrSmDfltReason;
   a5Cfg->threshold1_RSRP = lteeNodeBparams.lteCellMeasCfgGrp.measEvtA5Cfg[MSM_MEAS_EVENTA5_INTRA_FREQ_HO].threshold1_RSRP;
   a5Cfg->threshold1_RSRQ = wrSmDfltThresRSRQ1;
   a5Cfg->threshold2_RSRP =  lteeNodeBparams.lteCellMeasCfgGrp.measEvtA5Cfg[MSM_MEAS_EVENTA5_INTRA_FREQ_HO].threshold2_RSRP;
   a5Cfg->threshold2_RSRQ = wrSmDfltThresRSRQ2;
   a5Cfg->hysteresis = measEvtA5Cfg->hysteresis;
   a5Cfg->timeToTrigger = measEvtA5Cfg->timeToTrigger;
   a5Cfg->triggerQty = measEvtA5Cfg->triggerQty;
   a5Cfg->reportQty = measEvtA5Cfg->reportQty;
   a5Cfg->maxRptCells = measEvtA5Cfg->maxRptCells;
   a5Cfg->reportInterval = measEvtA5Cfg->reportInterval;
   a5Cfg->reportAmount = measEvtA5Cfg->reportAmount;

   a5Cfg = &measCfg->measEvtA5Cfg[WR_MEAS_EVENTA5_INTER_FREQ_HO_RSRP];
   measEvtA5Cfg = &lteeNodeBparams.lteCellMeasCfgGrp.measEvtA5Cfg[MSM_MEAS_EVENTA5_INTER_FREQ_HO_RSRP];
   /* configure Inter Event A5 */
   a5Cfg->isPres.pres = PRSNT_NODEF;
   a5Cfg->reason = wrSmDfltReason;
   a5Cfg->threshold1_RSRP = lteeNodeBparams.lteCellMeasCfgGrp.measEvtA5Cfg[MSM_MEAS_EVENTA5_INTER_FREQ_HO_RSRP].threshold1_RSRP;
   a5Cfg->threshold1_RSRQ = wrSmDfltThresRSRQ1;
   a5Cfg->threshold2_RSRP = lteeNodeBparams.lteCellMeasCfgGrp.measEvtA5Cfg[MSM_MEAS_EVENTA5_INTER_FREQ_HO_RSRP].threshold2_RSRP;
   a5Cfg->threshold2_RSRQ = wrSmDfltThresRSRQ2;
   a5Cfg->hysteresis = measEvtA5Cfg->hysteresis;
   a5Cfg->timeToTrigger = measEvtA5Cfg->timeToTrigger;
   a5Cfg->triggerQty = measEvtA5Cfg->triggerQty;
   a5Cfg->reportQty = measEvtA5Cfg->reportQty;
   a5Cfg->maxRptCells = measEvtA5Cfg->maxRptCells;
   a5Cfg->reportInterval = measEvtA5Cfg->reportInterval;
   a5Cfg->reportAmount = measEvtA5Cfg->reportAmount;

   a5Cfg = &measCfg->measEvtA5Cfg[WR_MEAS_EVENTA5_INTER_FREQ_ANR];
   measEvtA5Cfg = &lteeNodeBparams.lteCellMeasCfgGrp.measEvtA5Cfg[MSM_MEAS_EVENTA5_INTER_FREQ_ANR];
   /* configure Inter Event A5 */
   a5Cfg->isPres.pres = PRSNT_NODEF;
   a5Cfg->reason = wrSmDfltReason;
   a5Cfg->threshold1_RSRP = lteeNodeBparams.lteCellMeasCfgGrp.measEvtA5Cfg[MSM_MEAS_EVENTA5_INTER_FREQ_ANR].threshold1_RSRP;
   a5Cfg->threshold1_RSRQ = wrSmDfltThresRSRQ1;
   a5Cfg->threshold2_RSRP = lteeNodeBparams.lteCellMeasCfgGrp.measEvtA5Cfg[MSM_MEAS_EVENTA5_INTER_FREQ_ANR].threshold2_RSRP;
   a5Cfg->threshold2_RSRQ = wrSmDfltThresRSRQ2;
   a5Cfg->hysteresis = measEvtA5Cfg->hysteresis;
   a5Cfg->timeToTrigger = measEvtA5Cfg->timeToTrigger;
   a5Cfg->triggerQty = measEvtA5Cfg->triggerQty;
   a5Cfg->reportQty = measEvtA5Cfg->reportQty;
   a5Cfg->maxRptCells = measEvtA5Cfg->maxRptCells;
   a5Cfg->reportInterval = measEvtA5Cfg->reportInterval;
   a5Cfg->reportAmount = measEvtA5Cfg->reportAmount;
 
  /* configure Event Periodic */
   eutraPerd = &measCfg->measEutraPerdRptCfg[WR_MEAS_EUTRA_PERIODIC_REPORT_STRNG_CELLS];
   eutraMeasPeridCfg = &lteeNodeBparams.lteCellMeasCfgGrp.measEutraPeridRptCfg[MSM_MEAS_EUTRA_PERIODIC_REPORT_STRNG_CELLS];
   eutraPerd->isPres.pres = PRSNT_NODEF;
   eutraPerd->reason = wrSmDfltReason;
   eutraPerd->triggerQty = eutraMeasPeridCfg->triggerQty;
   eutraPerd->reportQty = eutraMeasPeridCfg->reportQty;
   eutraPerd->maxRptCells = eutraMeasPeridCfg->maxRptCells;
   eutraPerd->reportInterval = eutraMeasPeridCfg->reportInterval;
   eutraPerd->reportAmount = eutraMeasPeridCfg->reportAmount;
   eutraPerd->sonCfgRptInterval = eutraMeasPeridCfg->sonCfgRptInterval;
   eutraPerd->isPerdStrongCellCfgRptEnable = eutraMeasPeridCfg->isPerdStrongCellCfgRptEnable;

/* configure Event B2 for GERAN */
   b2GeranCfg = &measCfg->measGeranEventB2RptCfg[WR_MEAS_GERAN_EVENTB2_INTER_RAT_REDIR];
   b2GeranMeasCfg = &lteeNodeBparams.lteCellMeasCfgGrp.\
                    measGeranEventB2RptCfg[WR_MEAS_GERAN_EVENTB2_INTER_RAT_REDIR]; 
   b2GeranCfg->b2Threshold1RSRP = b2GeranMeasCfg->b2Threshold1RSRP;
   b2GeranCfg->b2Threshold1RSRQ = b2GeranMeasCfg->b2Threshold1RSRQ;
   b2GeranCfg->b2Threshold2Geran = b2GeranMeasCfg->b2Threshold2Geran;
   b2GeranCfg->hysteresis = b2GeranMeasCfg->hysteresis;
   b2GeranCfg->maxReportCell = b2GeranMeasCfg->maxReportCell;
   b2GeranCfg->reportAmount = b2GeranMeasCfg->reportAmount;
   b2GeranCfg->reportInterval = b2GeranMeasCfg->reportInterval;
   b2GeranCfg->timeToTrigger = b2GeranMeasCfg->timeToTrigger;

   /* configure Event B2 for GERAN */
   b2GeranCfg = &measCfg->measGeranEventB2RptCfg[WR_MEAS_GERAN_EVENTB2_INTER_RAT_HO];
   b2GeranMeasCfg = &lteeNodeBparams.lteCellMeasCfgGrp.\
                    measGeranEventB2RptCfg[WR_MEAS_GERAN_EVENTB2_INTER_RAT_HO];  
   b2GeranCfg->b2Threshold1RSRP = b2GeranMeasCfg->b2Threshold1RSRP;
   b2GeranCfg->b2Threshold1RSRQ = b2GeranMeasCfg->b2Threshold1RSRQ;
   b2GeranCfg->b2Threshold2Geran = b2GeranMeasCfg->b2Threshold2Geran;
   b2GeranCfg->hysteresis = b2GeranMeasCfg->hysteresis;
   b2GeranCfg->maxReportCell = b2GeranMeasCfg->maxReportCell;
   b2GeranCfg->reportAmount = b2GeranMeasCfg->reportAmount;
   b2GeranCfg->reportInterval = b2GeranMeasCfg->reportInterval;
   b2GeranCfg->timeToTrigger = b2GeranMeasCfg->timeToTrigger;


   /* configure Event B2 */
   b2UtraCfg = &measCfg->measUtraEventB2RptCfg[WR_MEAS_UTRA_EVENTB2_HO]; 
   b2UtraMeasCfg = &lteeNodeBparams.lteCellMeasCfgGrp.measUtraEventB2RptCfg[WR_MEAS_UTRA_EVENTB2_HO];
   b2UtraCfg->b2Threshold1RSRP = b2UtraMeasCfg->b2Threshold1RSRP;
   b2UtraCfg->b2Threshold1RSRQ = b2UtraMeasCfg->b2Threshold1RSRQ;
   b2UtraCfg->b2Threshold2UtraEcNo = b2UtraMeasCfg->b2Threshold2UtraEcNo;
   b2UtraCfg->b2Threshold2UtraRSCP = b2UtraMeasCfg->b2Threshold2UtraRSCP;
   b2UtraCfg->hysteresis = b2UtraMeasCfg->hysteresis;
   b2UtraCfg->maxReportCell = b2UtraMeasCfg->maxReportCell;
   b2UtraCfg->reportAmount = b2UtraMeasCfg->reportAmount;
   b2UtraCfg->reportInterval = b2UtraMeasCfg->reportInterval;
   b2UtraCfg->timeToTrigger = b2UtraMeasCfg->timeToTrigger;

   /* Configure UTRA Event B2 for Redirection to TD-SCDMA */
   b2UtraTddCfg = &measCfg->measUtraEventB2RptCfg[WR_MEAS_UTRA_EVENTB2_REDIR]; 
   b2UtraTddMeasCfg = &lteeNodeBparams.lteCellMeasCfgGrp.measUtraEventB2RptCfg[WR_MEAS_UTRA_EVENTB2_REDIR];
   b2UtraTddCfg->b2Threshold1RSRP = b2UtraTddMeasCfg->b2Threshold1RSRP;
   b2UtraTddCfg->b2Threshold1RSRQ = b2UtraTddMeasCfg->b2Threshold1RSRQ;
   b2UtraTddCfg->b2Threshold2UtraEcNo = b2UtraTddMeasCfg->b2Threshold2UtraEcNo;
   b2UtraTddCfg->b2Threshold2UtraRSCP = b2UtraTddMeasCfg->b2Threshold2UtraRSCP;
   b2UtraTddCfg->hysteresis = b2UtraTddMeasCfg->hysteresis;
   b2UtraTddCfg->maxReportCell = b2UtraTddMeasCfg->maxReportCell;
   b2UtraTddCfg->reportAmount = b2UtraTddMeasCfg->reportAmount;
   b2UtraTddCfg->reportInterval = b2UtraTddMeasCfg->reportInterval;
   b2UtraTddCfg->timeToTrigger = b2UtraTddMeasCfg->timeToTrigger;
   geranPrdCfg = &measCfg->measGERANPeridRptCfg[LWR_MEAS_GERAN_REPORT_STRNG_CELLS];
   geranPrdCfg->isPres.pres = PRSNT_NODEF;
   geranPrdCfg->reason = wrSmDfltUtraReason;
   geranPrdCfg->maxRptCells = 
         lteeNodeBparams.lteCellMeasCfgGrp.measGERANPeridRptCfg[0].maxRptCells;
   geranPrdCfg->reportInterval = 
         lteeNodeBparams.lteCellMeasCfgGrp.measGERANPeridRptCfg[0].reportInterval;
   geranPrdCfg->reportAmount = 
         lteeNodeBparams.lteCellMeasCfgGrp.measGERANPeridRptCfg[0].reportAmount;

   /*config for UTRA*/
   /* Below Report Amount parametrs hardcoded because current CSFB design 
    * support for only one measurement report per frequency. 
    * This will be resolved soon*/
   utraPerd = &measCfg->measUtraPerdRptCfg[WR_MEAS_UTRA_PRDRPT_STRGCELLS_CSFB];
   utraMeasPeridCfg = &lteeNodeBparams.lteCellMeasCfgGrp.measUtraPeriodRptCfg[MSM_MEAS_UTRA_PRDRPT_STRGCELLS_CSFB];

   utraPerd->reason = wrSmDfltUtraReason;
   utraPerd->maxReportCell= utraMeasPeridCfg->maxReportCell;
   utraPerd->reportInterval= utraMeasPeridCfg->reportInterval;
   utraPerd->reportAmount= utraMeasPeridCfg->reportAmount;

   utraPerd = &measCfg->measUtraPerdRptCfg[WR_MEAS_UTRA_PRDRPT_STRGCELLS_ANR];
   utraMeasPeridCfg = &lteeNodeBparams.lteCellMeasCfgGrp.measUtraPeriodRptCfg[MSM_MEAS_UTRA_PRDRPT_STRGCELLS_ANR];

   utraPerd->reason = wrSmDfltUtraReason;
   utraPerd->maxReportCell= utraMeasPeridCfg->maxReportCell;
   utraPerd->reportInterval= utraMeasPeridCfg->reportInterval;
   utraPerd->reportAmount= utraMeasPeridCfg->reportAmount;


   /* configure 1xCdma Event B1 */
   measCfg->meas1xCDMAEventB1RptCfg[0].isPres.pres = PRSNT_NODEF;
   measCfg->meas1xCDMAEventB1RptCfg[0].reason = wrSmDfltReason;
   measCfg->meas1xCDMAEventB1RptCfg[0].cellType = wrSmDfltCellType;
   measCfg->meas1xCDMAEventB1RptCfg[0].threshold = wrSmDfltCdmaThres1;
   measCfg->meas1xCDMAEventB1RptCfg[0].hysteresis = wrSmDfltCdmaHysteresis;
   measCfg->meas1xCDMAEventB1RptCfg[0].timeToTrigger = wrSmDfltCdmaTimeToTrig;
   measCfg->meas1xCDMAEventB1RptCfg[0].maxRptCells = wrSmDfltCdmaMaxRptCells;
   measCfg->meas1xCDMAEventB1RptCfg[0].reportInterval =
      wrSmDfltCdmaRptInterval;
   measCfg->meas1xCDMAEventB1RptCfg[0].reportAmount = wrSmDfltCdmaRptAmount;

   /* configure 1xCdma Event B2 */
   measCfg->meas1xCDMAEventB2RptCfg[0].isPres.pres = PRSNT_NODEF;
   measCfg->meas1xCDMAEventB2RptCfg[0].reason = wrSmDfltReason;
   measCfg->meas1xCDMAEventB2RptCfg[0].cellType = wrSmDfltCellType;
   measCfg->meas1xCDMAEventB2RptCfg[0].threshold1Rsrp = wrSmDfltCdmaThresRSRP1;
   measCfg->meas1xCDMAEventB2RptCfg[0].threshold1Rsrq = wrSmDfltCdmaThresRSRQ1;
   measCfg->meas1xCDMAEventB2RptCfg[0].threshold2 = wrSmDfltCdmaThres1;
   measCfg->meas1xCDMAEventB2RptCfg[0].hysteresis = wrSmDfltCdmaHysteresis;
   measCfg->meas1xCDMAEventB2RptCfg[0].timeToTrigger = wrSmDfltCdmaTimeToTrig;
   measCfg->meas1xCDMAEventB2RptCfg[0].maxRptCells = wrSmDfltCdmaMaxRptCells;
   measCfg->meas1xCDMAEventB2RptCfg[0].trigQtyEutra = wrSmDfltTrigQty;
   measCfg->meas1xCDMAEventB2RptCfg[0].reportInterval =
      wrSmDfltCdmaRptInterval;
   measCfg->meas1xCDMAEventB2RptCfg[0].reportAmount = wrSmDfltCdmaRptAmount;


   /* configure Cdma Hrpd Event B1 */
   measCfg->measCDMAHrpdEventB1RptCfg[0].isPres.pres = PRSNT_NODEF;
   measCfg->measCDMAHrpdEventB1RptCfg[0].reason = wrSmDfltReason;
   measCfg->measCDMAHrpdEventB1RptCfg[0].cellType = wrSmDfltCellType;
   measCfg->measCDMAHrpdEventB1RptCfg[0].threshold = wrSmDfltCdmaThres1;
   measCfg->measCDMAHrpdEventB1RptCfg[0].hysteresis = wrSmDfltCdmaHysteresis;
   measCfg->measCDMAHrpdEventB1RptCfg[0].timeToTrigger =
      wrSmDfltCdmaTimeToTrig;
   measCfg->measCDMAHrpdEventB1RptCfg[0].maxRptCells = wrSmDfltCdmaMaxRptCells;
   measCfg->measCDMAHrpdEventB1RptCfg[0].reportInterval =
      wrSmDfltCdmaRptInterval;
   measCfg->measCDMAHrpdEventB1RptCfg[0].reportAmount = wrSmDfltCdmaRptAmount;

   /* configure Cdma Hrpd Event B2 */
   measCfg->measCDMAHrpdEventB2RptCfg[0].isPres.pres = PRSNT_NODEF;
   measCfg->measCDMAHrpdEventB2RptCfg[0].reason = wrSmDfltReason;
   measCfg->measCDMAHrpdEventB2RptCfg[0].cellType = wrSmDfltCellType;
   measCfg->measCDMAHrpdEventB2RptCfg[0].threshold1Rsrp =
      wrSmDfltCdmaThresRSRP1;
   measCfg->measCDMAHrpdEventB2RptCfg[0].threshold1Rsrq =
      wrSmDfltCdmaThresRSRQ1;
   measCfg->measCDMAHrpdEventB2RptCfg[0].threshold2 = wrSmDfltCdmaThres1;
   measCfg->measCDMAHrpdEventB2RptCfg[0].hysteresis = wrSmDfltCdmaHysteresis;
   measCfg->measCDMAHrpdEventB2RptCfg[0].timeToTrigger =
      wrSmDfltCdmaTimeToTrig;
   measCfg->measCDMAHrpdEventB2RptCfg[0].maxRptCells = wrSmDfltCdmaMaxRptCells;
   measCfg->measCDMAHrpdEventB2RptCfg[0].trigQtyEutra = wrSmDfltTrigQty;
   measCfg->measCDMAHrpdEventB2RptCfg[0].reportInterval =
      wrSmDfltCdmaRptInterval;
   measCfg->measCDMAHrpdEventB2RptCfg[0].reportAmount = wrSmDfltCdmaRptAmount;

   /* configure 1xCdma Periodic */
   measCfg->meas1xCDMAPeridRptCfg[0].isPres.pres = PRSNT_NODEF;
   measCfg->meas1xCDMAPeridRptCfg[0].reason = wrSmDfltReason;
   measCfg->meas1xCDMAPeridRptCfg[0].cellType = wrSmDfltCellType;
   measCfg->meas1xCDMAPeridRptCfg[0].maxRptCells = lteeNodeBparams.lteCellMeasCfgGrp.measCDMA1xPeridRptCfg[0].maxReportCell;
   measCfg->meas1xCDMAPeridRptCfg[0].reportInterval = lteeNodeBparams.lteCellMeasCfgGrp.measCDMA1xPeridRptCfg[0].reportInterval;
   measCfg->meas1xCDMAPeridRptCfg[0].reportAmount = lteeNodeBparams.lteCellMeasCfgGrp.measCDMA1xPeridRptCfg[0].reportAmount;

   /* configure Cdma HRPD Periodic */
   measCfg->measCDMAHrpdPeridRptCfg[0].isPres.pres = PRSNT_NODEF;
   measCfg->measCDMAHrpdPeridRptCfg[0].reason = wrSmDfltReason;
   measCfg->measCDMAHrpdPeridRptCfg[0].cellType = wrSmDfltCellType;
   measCfg->measCDMAHrpdPeridRptCfg[0].maxRptCells = wrSmDfltCdmaMaxRptCells;
   measCfg->measCDMAHrpdPeridRptCfg[0].reportInterval =
      wrSmDfltCdmaRptInterval;
   measCfg->measCDMAHrpdPeridRptCfg[0].reportAmount = wrSmDfltCdmaRptAmount;

   /* configure Speed scaling Factors */
   measCfg->measSpdSclFact.sfUsagePres.pres =  PRSNT_NODEF;
   measCfg->measSpdSclFact.sMeasure = lteeNodeBparams.lteCellMeasCfgGrp.measSpeedCfg.sMeasure;
   measCfg->measSpdSclFact.timeToTriggerSfMedium = lteeNodeBparams.lteCellMeasCfgGrp.measSpeedCfg.timeToTriggerSfMedium;
   measCfg->measSpdSclFact.timeToTriggerSfHigh = lteeNodeBparams.lteCellMeasCfgGrp.measSpeedCfg.timeToTriggerSfHigh;

   /* configure Mobility state Params */
   measCfg->mobStatParams.speedStatPres.pres =  PRSNT_NODEF;
   /* Fix for CR - ccpu00137183  */
   measCfg->mobStatParams.tEvalulation       = mobStateParams->tEvalulation;
   measCfg->mobStatParams.tHystNormal        = mobStateParams->tHystNormal; 
   measCfg->mobStatParams.nCellChangeMedium  = mobStateParams->nCellChangeMedium;
   measCfg->mobStatParams.nCellChangeHigh    = mobStateParams->nCellChangeHigh;  

   /* configure Quantity params */
   measCfg->qtyCfg.rsrpFltrCoeff  = lteeNodeBparams.lteCellMeasCfgGrp.qtyCfg.rsrpFltrCoeff; 
   measCfg->qtyCfg.rsrqFltrCoeff  = lteeNodeBparams.lteCellMeasCfgGrp.qtyCfg.rsrqFltrCoeff; 
   measCfg->qtyCfg.measQtyCDMA2k  = lteeNodeBparams.lteCellMeasCfgGrp.qtyCfg.measQtyCDMA2k;
   measCfg->qtyCfg.measQtyUtraTdd   = lteeNodeBparams.lteCellMeasCfgGrp.qtyCfg.measQtyUtraTdd;
   measCfg->qtyCfg.fltrCoeffGERAN = lteeNodeBparams.lteCellMeasCfgGrp.qtyCfg.fltrCoeffGERAN;
   measCfg->qtyCfg.ecnoFltrCoeff  = lteeNodeBparams.lteCellMeasCfgGrp.qtyCfg.ecnoFltrCoeff; 
   measCfg->qtyCfg.measQtyUtraFdd = lteeNodeBparams.lteCellMeasCfgGrp.qtyCfg.measQtyUtraFdd; 
   measCfg->qtyCfg.filterCoefficient2FDDr10  = lteeNodeBparams.lteCellMeasCfgGrp.qtyCfg.filterCoefficient2FDDr10;/*RRC Rel10 Upgrade*/ 

   /* configure Measurement Gap params */
   measCfg->measGap.gapForANRPres.pres =  PRSNT_NODEF;
   measCfg->measGap.gapPatternForInterFA = lteeNodeBparams.lteCellMeasCfgGrp.measGap.gapPatternForInterFA;
   measCfg->measGap.gapPatternForInterRAT = lteeNodeBparams.lteCellMeasCfgGrp.measGap.gapPatternForInterRAT;
   measCfg->measGap.gapPatternForANR = lteeNodeBparams.lteCellMeasCfgGrp.measGap.gapPatternForANR;;

   /* configure PreRegistration HRPD */
   measCfg->preRegHrpd.preRegAllowed = wrSmDfltPreRegAllowed;
   measCfg->preRegHrpd.preRegZoneId = wrSmDfltPreRegZoneId;
   measCfg->preRegHrpd.NumOf2ndPreRegZoneIdList =
      wrSmDfltNumOf2ndPreRegZoneIdList;
   measCfg->preRegHrpd.secondaryPreRegZoneId1 = wrSmDfltSecondaryPreRegZoneId1;
   measCfg->preRegHrpd.secondaryPreRegZoneId2 = wrSmDfltSecondaryPreRegZoneId2;

   measCfg->anrRptCfg = lteeNodeBparams.lteCellMeasCfgGrp.anrRptCfg;
   measCfg->hoRptCfg = lteeNodeBparams.lteCellMeasCfgGrp.hoRptCfg;
   /*rsrq_ho*/
   measCfg->rsrqHoThrshld = wrSmDfltThresRSRQ;

   RETVOID;
} /* end of smBldMeasCellCfg */

/* Populate the SIB periodicites */
/*
*
*       Fun:   smBuildGenCfg
*
*       Desc:  fill in cellCfg for WR
*
*       Ret:   ROK
*
*       Notes: None
*              
*       File:  wr_smm_enbapp_cmnplatutils.c  
*/
#ifdef ANSI
PUBLIC S16 smBuildGenCfg
(
 LwrMngmt *lwrMng 
)
#else /* if ANSI not defined*/
PUBLIC S16 smBuildGenCfg(lwrMng)
LwrMngmt *lwrMng
#endif /* end of ANSI*/
{
   U8 loopCnt = 0;
   LwrGenCfg    *cfg = NULLP;
   cfg   = &(lwrMng->t.cfg.t.genCfg);
   /*----------- Fill General Configuration Parameters ---------*/
#ifdef WR_RELAY
   /* CELL Configuration  */
   cfg->cellCfg.cellId       =  smCfgCb.cellId;
   cfg->cellCfg.modType      =  smCfgCb.modType;
   cfg->cellCfg.duplexMode   =  smCfgCb.duplexMode;
   cfg->cellCfg.maxUeSupp    =  smCfgCb.maxUeSupp;
   cfg->cellCfg.mcc[0].pres  =  smCfgCb.mcc[0].pres;
   cfg->cellCfg.mcc[0].val   =  smCfgCb.mcc[0].val;
   cfg->cellCfg.mcc[1].pres  =  smCfgCb.mcc[1].pres;
   cfg->cellCfg.mcc[1].val   =  smCfgCb.mcc[1].val;
   cfg->cellCfg.mcc[2].pres  =  smCfgCb.mcc[2].pres;
   cfg->cellCfg.mcc[2].val   =  smCfgCb.mcc[2].val;
   cfg->cellCfg.mnc[0].pres  =  smCfgCb.mnc[0].pres;
   /* wr003.102   : CR ID- ccpu00116764-IOT changes*/ 
   cfg->cellCfg.mnc[0].val   =  smCfgCb.mnc[0].val;
   cfg->cellCfg.mnc[1].pres  =  smCfgCb.mnc[1].pres;
   /* wr003.102   : CR ID- ccpu00116764-IOT changes*/ 
   cfg->cellCfg.mnc[1].val   =  smCfgCb.mnc[1].val;
   cfg->cellCfg.mnc[2].pres  =  smCfgCb.mnc[2].pres;
   cfg->cellCfg.mnc[2].val   =  smCfgCb.mnc[2].val;
   cfg->cellCfg.trackAreaCode=  smCfgCb.trackAreaCode;
   cfg->cellCfg.freqBandInd  =  smCfgCb.freqBandInd;
#ifdef LTE_HO_SUPPORT
   cfg->cellCfg.rrmRntiStart = lteeNodeBparams.lteSmCellCfg.rrmRntiStart;
   cfg->cellCfg.maxRrmRntis = lteeNodeBparams.lteSmCellCfg.maxRrmRntis;
   cfg->cellCfg.rrmPrmblStart = smCfgCb.rrmDedPrmStart;
   cfg->cellCfg.maxRrmPrmbls = smCfgCb.rrmNumDedPrm;
   cfg->cellCfg.macRntiStart = lteeNodeBparams.lteSmCellCfg.macRntiStart;
   cfg->cellCfg.maxMacRntis = lteeNodeBparams.lteSmCellCfg.maxMacRntis;

   cfg->cellCfg.numOfMacPrm = lteeNodeBparams.lteSmCellCfg.numOfMacPrm;
   cfg->cellCfg.rsrpTrshldRange = smCfgCb.rsrpTrshldRange;
#endif
   /* Filling PLMN configuration */
   cfg->cellCfg.noOfBrdCstPlmn = smCfgCb.noOfBrdCstPlmn;
   for (loopCnt = 0; loopCnt < smCfgCb.noOfBrdCstPlmn; loopCnt++)
   {
      strcpy((S8 *)cfg->cellCfg.plmnId[loopCnt], (S8 *)smCfgCb.plmnId[loopCnt]);
   }
   /* Filling the ENB Type */
   cfg->eNBType = wrSmDfltEnbType;
   printf("\n smBuildWrGenCfg: cfg->enblSIAndPagngLog %d  smCb.rrcLogEnable %d\n", cfg->enblSIAndPagngLog, smCb.rrcLogEnable);
   cfg->enblSIAndPagngLog = smCb.rrcLogEnable;
   cfg->noOfNghInfo = smCfgCb.noOfNghInfo;
   /*cfg->nghInfo = smCfgCb.nghInfo;*/
   cfg->noOfguGrp = smCfgCb.noOfguGrp;
   /*cfg->guGrpId = smCfgCb.guGrpId;*/

   /* Filling UE HO Attach Timer */
   cfg->ueHoAttachTimerVal = smCfgCb.ueHoAttachTimerVal;


   /* Filling Physical Cell Id */
   cfg->cellCfg.physCellId = smCfgCb.pci;

   wrCpyCmTptAddr(&cfg->dataAppAddr, &smCfgCb.enbIpAddr[WR_IPV4_IDX]);
   if(CM_TPTADDR_IPV4 ==  cfg->dataAppAddr.type)
   {
      cfg->dataAppAddr.u.ipv4TptAddr.port = WR_DFLT_EGTP_PORT;
   }   
   else if(CM_TPTADDR_IPV6 ==  cfg->dataAppAddr.type)
   {
      cfg->dataAppAddr.u.ipv6TptAddr.port = WR_DFLT_EGTP_PORT;
   }   
  
#endif /* end of WR_RELAY */

   /* Filling UE Inactivity Timer */

   cfg->inactvTmrVal = lteeNodeBparams.lteEnbProtoCfg.inactvTmrVal;
   cfg->expiryCnt   = lteeNodeBparams.lteEnbProtoCfg.maxExpires;
   cfg->endMrkTmrVal = lteeNodeBparams.lteEnbProtoCfg.endMrkTmrVal;
   /* Fill UL SRS Periodicity */ 
   cfg->ulSrsPeriod  = wrSmUlSrsPeriod; 
#ifdef ENB_OVRLOAD
   cfg->eNBOvldParams.critThrldVal = WR_CRIT_THRLD_VAL;
   cfg->eNBOvldParams.majThrldVal  = WR_MAX_THRLD_VAL; 
   cfg->eNBOvldParams.minThrldVal  = WR_MIN_THRLD_VAL;
   cfg->eNBOvldParams.critRcvryVal = WR_CRIT_RCVRY_VAL;
   cfg->eNBOvldParams.majRcvryVal  = WR_MAX_RCVRY_VAL;
   cfg->eNBOvldParams.minRcvryVal  = WR_MIN_RCVRY_VAL;
   cfg->eNBOvldParams.eNBMemCritThresVal= WR_ENB_MEM_CRIT_THRES_VAL;
   cfg->eNBOvldParams.eNBMemMajThresVal = WR_ENB_MEM_MAX_THRES_VAL;
   cfg->eNBOvldParams.eNBMemMinThresVal = WR_ENB_MEM_MIN_THRES_VAL;
   cfg->eNBOvldParams.eNBOvldMonTimerVal= WR_ENB_OVLD_MON_TMR_VAL;
   cfg->eNBOvldParams.eNBOvldMonfreq   = WR_ENB_OVLD_MON_FREQ;
   cfg->eNBOvldParams.maxCntval        = WR_MAX_CNT_VAL;
   cfg->eNBOvldParams.eNbOvrLdFlag     = WR_ENB_OVLD_FLAG;
#endif  

   RETVALUE(ROK);
}/*end of smBuildGenCfg*/
/*
*
*       Fun:   smBuildCellCfg
*
*       Desc:  fill in cellCfg for WR
*
*       Ret:   ROK
*
*       Notes: None
*              
*       File:  wr_smm_enbapp_cmnplatutils.c  
*/

#ifdef ANSI
PUBLIC S16 smBuildCellCfg
(
 LwrMngmt *lwrMng 
)
#else /* if ANSI not defined*/
PUBLIC S16 smBuildCellCfg(lwrMng)
LwrMngmt *lwrMng
#endif /* end of ANSI*/
{
   LwrCellMibCfgGrp    *lwrMib = NULLP;
   LwrCellSib4CfgGrp   *sib4 =NULLP;
   LwrProcTimerCfgGrp  *tmrCfg = NULLP;
   U8                  cfgGrpIndex = 0;
   LwrCellSib6CfgGrp   *sib6 = NULLP;
   LwrCellSib7CfgGrp   *sib7 = NULLP;
   LwrCellSib9CfgGrp   *sib9 = NULLP;      
   LwrAddCellCfg       *enbCfg = NULLP;
   LwrRgrCfgGrp        *rgrCfg = NULLP;
   LwrCtfCfgGrp        *ctfCfg = NULLP;
   LwrRlcCfgGrp        *rlcCfg = NULLP;
   LwrCellSchdCfg      *schdCfg = NULLP;
   LwrCsfbCfgGrp       *csfbGrp = NULLP;
   U16                 dfltDrxCycle;
   U8                  dfltModPrdCoeff;
   LwrCellUlSchdCfgGrp *ulSchdCfgGrp = NULLP;
   LwrCellDlSchdCfgGrp *dlSchdCfgGrp = NULLP;
   U8 wrHoDedPreamCount = 0;
   U32 grpIdx,freingIdx;

   TRC2(smBuildCellCfg);

   enbCfg = &(lwrMng->t.protoCfg.t.cellCfg.t.addCell);
   lwrMng->t.protoCfg.type = WR_CFG_TYPE_CELL;
   lwrMng->t.protoCfg.t.cellCfg.action = WR_ACT_ADD;

   lwrMng->t.protoCfg.t.cellCfg.t.addCell.dlCarrierFreq = lteeNodeBparams.lteAddCellCfg.earfdnDl; /*ccpu00127813 changes*/
   lwrMng->t.protoCfg.t.cellCfg.t.addCell.cellId = 1; 
   lwrMng->t.protoCfg.t.cellCfg.t.addCell.cellIndex     = WR_DFLT_CELL_IDX; 
   /*DRX QCI */
   lwrMng->t.protoCfg.t.cellCfg.t.addCell.isDRXQCIEnabled = lteeNodeBparams.lteDrxCfgParams.isDRXQCIEnabled;

   lwrMng->t.protoCfg.t.cellCfg.t.addCell.cellSizeType      = lteeNodeBparams.lteAddCellCfg.cellSizeType;
   lwrMng->t.protoCfg.t.cellCfg.t.addCell.physCellId    = lteeNodeBparams.lteAddCellCfg.physCellId; 
   lwrMng->t.protoCfg.t.cellCfg.t.addCell.duplexMode    = lteeNodeBparams.lteAddCellCfg.duplexMode; 
	   
   /* CSG_DEV start */
   lwrMng->t.protoCfg.t.cellCfg.t.addCell.csgAccessMode = lteeNodeBparams.lteFapAccessParams.accessMode;
   /* because there will be value for startPci and pciRange always, setting 
    * pciRangeInfoPres to PRSNT_NODEF */
   lwrMng->t.protoCfg.t.cellCfg.t.addCell.smallCellCsgPciRange.pciRangeInfoPres = PRSNT_NODEF;
   lwrMng->t.protoCfg.t.cellCfg.t.addCell.smallCellCsgPciRange.startPci = lteeNodeBparams.lteAddCellCfg.startPci;
   lwrMng->t.protoCfg.t.cellCfg.t.addCell.smallCellCsgPciRange.pciRange = lteeNodeBparams.lteAddCellCfg.pciRange;
   if(lteeNodeBparams.lteFapAccessParams.accessMode == WR_ENB_HYBRID_ACCESS)
   {
      lwrMng->t.protoCfg.t.cellCfg.t.addCell.hcsgParam.minDlResNonCsg  = 
                     lteeNodeBparams.lteAddCellCfg.minDlResNonCsg;
      lwrMng->t.protoCfg.t.cellCfg.t.addCell.hcsgParam.minUlResNonCsg  = 
                     lteeNodeBparams.lteAddCellCfg.minUlResNonCsg;
   }
   /* CSG_DEV end */

   /*Filling MIB params*/
   /* mem_issue_fix: Allocating memory for all the cell group */
   for(grpIdx=0 ;grpIdx<WR_CELL_CFG_MAX_GRPS;grpIdx++)
   {
      WR_ALLOC(&(lwrMng->t.protoCfg.t.cellCfg.t.addCell.cellCfgGrp[grpIdx]), \
            (sizeof(LwrCellCfgGrp)));
      if(NULLP == lwrMng->t.protoCfg.t.cellCfg.t.addCell.cellCfgGrp[grpIdx])
      {
         for (freingIdx=0 ;freingIdx< grpIdx;freingIdx++ )
         {
            WR_FREE((lwrMng->t.protoCfg.t.cellCfg.t.addCell.cellCfgGrp[freingIdx]), \
                  (sizeof(LwrCellCfgGrp)));
         }
         RLOG0(L_FATAL, "Memory allocation failed while filling cell configuration");
         RETVALUE(RFAILED);
      }
   }
   lwrMng->t.protoCfg.t.cellCfg.t.addCell.cellCfgGrp[cfgGrpIndex]->grpType 
      = WR_CELL_CFG_GRP_MIB;
   lwrMib = &(lwrMng->t.protoCfg.t.cellCfg.t.addCell.cellCfgGrp[cfgGrpIndex]->t.mibCfgGrp);

   lwrMib->dlBw              = lteeNodeBparams.lteCellMibCfgGrp.dlBw[0];
   wrSmmSetBwFrmBwIdx ( lwrMib->dlBw );
   lwrMib->phichCfg.phichDur = wrSmDfltPhichDur;
   lwrMib->phichCfg.phichRes = wrSmDfltPhichRes;
   cfgGrpIndex++;

   /* Build the sib1 configuration parameter */
   smBldSib1Cfg(lwrMng, cfgGrpIndex, lteeNodeBparams.lteFapAccessParams.accessMode);
   cfgGrpIndex++; 

   /* Build the sib2 configuration parameter */
   smBldSib2Cfg(lwrMng, cfgGrpIndex);
   cfgGrpIndex++; 

   /* Build the sib2 configuration parameter */
   smBldSib3Cfg(lwrMng, cfgGrpIndex);
   cfgGrpIndex++; 

   /* Configure the SIB4 parameters*/
   lwrMng->t.protoCfg.t.cellCfg.t.addCell.cellCfgGrp[cfgGrpIndex]->grpType =
      WR_CELL_CFG_GRP_SIB4;
   sib4 = &(lwrMng->t.protoCfg.t.cellCfg.t.addCell.cellCfgGrp[cfgGrpIndex]->t.sib4CfgGrp);

   /*Updating the CSG Split Info parameters */
   if(TRUE == lteeNodeBparams.lteSib4CsgInfo.isCsgPciCfgd)
   {
      sib4->csgInfo.isCsgPciCfgd      = TRUE;
      sib4->csgInfo.csgStartPhyCellId = lteeNodeBparams.lteSib4CsgInfo.csgStartPhyCellId;
      sib4->csgInfo.csgPciRange       = (U8)lteeNodeBparams.lteSib4CsgInfo.csgPciRange;
   }

   /* ccpu00132596 */
   wrSmmIsPciRangeValid(&sib4->csgInfo);
   /* ccpu00132596 */

   cfgGrpIndex++;

   lwrMng->t.protoCfg.t.cellCfg.t.addCell.cellCfgGrp[cfgGrpIndex]->grpType =
      WR_CELL_CFG_GRP_TIMER;
   /* Filling Emm (S1/X2) Timer values */
   /* TODO: Do the same for all other timers */
   tmrCfg = &(lwrMng->t.protoCfg.t.cellCfg.t.addCell.cellCfgGrp[cfgGrpIndex]->
         t.procTimerCfgGrp);

   tmrCfg->s1SetupTmr                       = wrSmDfltS1SetupTmr;
   tmrCfg->s1UpdateTmr                      = wrSmDfltUpdateTmr;
   tmrCfg->s1RetryCount                     = wrSmDfltS1RtryCntTmr;
   tmrCfg->s1TimeToWait                     = wrSmDfltS1TimeToWait;
   tmrCfg->s1MmeConfigurationTransferTmr    = wrSmDfltS1MmeCfgTrfrTmr;
   tmrCfg->s1ResetTmr                       = wrSmDfltX2ResetRetryTmr; 
   tmrCfg->x2SetupTmr                       = wrSmDfltX2SetupTmr;
   tmrCfg->x2SetTimeToWait                  = wrSmDfltX2SetTimeToWait;
   tmrCfg->x2UpdateTmr                      = wrSmDfltX2UpdateTmr;
   tmrCfg->x2UpdTimeToWait                  = wrSmDfltX2UpdateTimeToWait;
   tmrCfg->x2ResetTmr                       = wrSmDfltS1ResetRetryTmr; 
   tmrCfg->X2ResetRetryCount                = wrSmDfltX2RstRtryCntTmr;
   tmrCfg->X2SetupRetryCount                = wrSmDfltX2SetupRtryCntTmr;
   tmrCfg->x2UpdRetryCount                  = wrSmDfltX2UpdateRetryCount;
   tmrCfg->internalNoWaitRetryTmr           = wrSmDfltIntNoWaitRetryTmr;

   /* Fill RRC Timer Values */
   tmrCfg->rrcConnectionSetupTmr           = wrSmDfltRrcConSetupTmr;
   tmrCfg->rrcConnectionReconfigTmr        = wrSmDfltRrcConReCfgTmr;
   tmrCfg->rrcConnectionReEstablishTmr     = wrSmDfltRrcConReEstTmr;
   tmrCfg->rrcSecurityModeCommandTmr       = wrSmDfltRrcSecModCmdTmr;
   tmrCfg->rrcUeCapabilityEnquiryTmr       = wrSmDfltRrcUeCapEnqTmr;
   tmrCfg->rrcConnectionReleaseTmr         = wrSmDfltRrcConRelTmr;
   tmrCfg->rrcHandoverPrepTmr              = wrSmDfltRrcHandoverPrepTmr;
   tmrCfg->rrcUeInformationRequestTmr      = wrSmDfltRrcUeInfReqTmr;

   tmrCfg->s1InitialContextSetupTmr        = wrSmDflts1InitialContextSetupTmr;
   tmrCfg->s1ErabReleaseIndicationTmr      =wrSmDflts1ErabReleaseIndicationTmr;
   tmrCfg->s1UeContextReleaseTmr           = wrSmDflts1UeContextReleaseTmr;
   tmrCfg->s1PathSwitchTmr                 = wrSmDflts1PathSwitchTmr;
   tmrCfg->s1HandoverCancelTmr           = lteeNodeBparams.lteProcTimerCfg.s1HandoverCancelTmr;
   tmrCfg->x2HandoverPrepTmr               = wrSmDfltx2HandoverPrepTmr;
   tmrCfg->x2RelocOverallTmr               = wrSmDfltx2RelocOverallTmr;
   tmrCfg->x2StoreUeContextTmr             = wrSmDfltx2StoreUeContextTmr;
   tmrCfg->x2SnStatusTransferTmr           = wrSmDfltx2SnStatusTransferTmr;
   tmrCfg->internalSubCellChangeTmr        = wrSmDfltinternalSubCellChangeTmr;
   tmrCfg->internalSubCellPdcpControlTmr   =
      wrSmDfltinternalSubCellPdcpControlTmr;
   tmrCfg->ehrpdOptHoCompleteTmr          = wrSmDfltehrpdOptHoCompleteTmr;
   tmrCfg->intraEnbHoCompleteTmr          = wrSmDfltintraEnbHoCompleteTmr;
   tmrCfg->measurementReportToWaitTmr     = wrSmDfltmeasurementReportToWaitTmr;
   tmrCfg->internalResourceSetupTmr       = wrSmDfltinternalResourceSetupTmr;
   tmrCfg->internalResourceReleaseTmr     = wrSmDfltinternalResourceReleaseTmr;
   tmrCfg->internalSecurityControlTmr     = wrSmDfltinternalSecurityControlTmr;
   tmrCfg->internalRrcDataTransferTmr     = wrSmDfltinternalRrcDataTransferTmr;
   tmrCfg->internalForwardingPathSetupTmr =
      wrSmDfltinternalForwardingPathSetupTmr;
   tmrCfg->internalReestablishControlTmr   =
      wrSmDfltinternalReestablishControlTmr;
   tmrCfg->internalBufferFlushTmr          = wrSmDfltinternalBufferFlushTmr;
   tmrCfg->internalDataTransferStartTmr    =
      wrSmDfltinternalDataTransferStartTmr;
   tmrCfg->internalDataForwardEndIndTmr    =
      wrSmDfltinternalDataForwardEndIndTmr;
   tmrCfg->internalReestablishTimeToWaitTmr =
      wrSmDfltinternalReestablishTimeToWaitTmr;
   tmrCfg->internalUserInactivityTmr       = wrSmDfltinternalUserInactivityTmr;
   tmrCfg->internalSignalingInactivityTmr  =
      wrSmDfltinternalSignalingInactivityTmr;
   tmrCfg->internalUnassignedTmr           = wrSmDfltinternalUnassignedTmr;
   tmrCfg->internalNoWaitRetryTmr          = wrSmDfltinternalNoWaitRetryTmr;
   tmrCfg->internalRrcResetTmr             = wrSmDfltinternalRrcResetTmr;
   tmrCfg->internalEnbResetTmr             = wrSmDfltinternalEnbResetTmr;
   tmrCfg->internalSolicitMeasurementTmr   =
      wrSmDfltinternalSolicitMeasurementTmr;
   tmrCfg->internalRtdInfoReqTmr           = wrSmDfltinternalRtdInfoReqTmr;
   tmrCfg->s1HandoverPrepTmr = lteeNodeBparams.lteProcTimerCfg.s1PrepTimerVal;
   tmrCfg->s1RelocOverallTmr = lteeNodeBparams.lteProcTimerCfg.s1OvrAllTimerVal;
   tmrCfg->s1IRatHandoverPrepTmr = lteeNodeBparams.lteProcTimerCfg.s1RATPrepTimerVal;
   tmrCfg->s1IRatRelocOverallTmr = lteeNodeBparams.lteProcTimerCfg.s1RATOvrAllTimerVal;
   tmrCfg->x2HandoverPrepTmr = lteeNodeBparams.lteProcTimerCfg.x2PrepTimerVal;
   tmrCfg->x2RelocOverallTmr = lteeNodeBparams.lteProcTimerCfg.x2OvrAllTimerVal;

   //RIM parameters
   MsmLteRimParams *lteRimParams = &(lteeNodeBparams.lteRimParams);
   tmrCfg->rimOvrlTmr = lteRimParams->rimOvrlTmrVal;
   tmrCfg->rimRirTmr = lteRimParams->rimRirTmr;
   tmrCfg->rimRiTmr = lteRimParams->rimRiTmr;
   tmrCfg->rimRiaeTmr = lteRimParams->rimRiaeTmr;
   tmrCfg->rimMaxRetryCnt = lteRimParams->rimMaxRetryCnt;

   /*Config SIB6 Params*/
   cfgGrpIndex++; 



   lwrMng->t.protoCfg.t.cellCfg.t.addCell.cellCfgGrp[cfgGrpIndex]->grpType =
      WR_CELL_CFG_GRP_SIB6;

   sib6 = &(lwrMng->t.protoCfg.t.cellCfg.t.addCell.cellCfgGrp[cfgGrpIndex]->t.sib6CfgGrp);

   /*Updating the SIB6 parameters*/
   sib6->reCfgType            = wrSmDfltUtraReCfgType;
   sib6->sfUsagePres.pres     = PRSNT_NODEF;
   sib6->tReselection         = wrSmDfltReslct; 
   sib6->tReselection         = lteeNodeBparams.lteCellSib6CfgGrp.tReselection;
   sib6->tReselectionSfMedium = lteeNodeBparams.lteCellSib6CfgGrp.tReselectionSfMedium;
   sib6->tReselectionSfHigh   = lteeNodeBparams.lteCellSib6CfgGrp.tReselectionSfHigh;

   /*Config SIB7 Params*/
   cfgGrpIndex++; 

   lwrMng->t.protoCfg.t.cellCfg.t.addCell.cellCfgGrp[cfgGrpIndex]->grpType =
                                                          WR_CELL_CFG_GRP_SIB7;
   
   sib7 = &(lwrMng->t.protoCfg.t.cellCfg.t.addCell.cellCfgGrp[cfgGrpIndex]->t.sib7CfgGrp);

   sib7->reCfgType = wrSmDfltGeranReCfgType;
   sib7->tReselectionGERAN                = lteeNodeBparams.sib7CfgGrp.tReselectionGERAN;
   sib7->pres_SpeedStateScaleFactors.pres = lteeNodeBparams.sib7CfgGrp.pres_SpeedStateScaleFactors;
   sib7->tReselectionGERANSFMedium        = lteeNodeBparams.sib7CfgGrp.tReselectionGERANSFMedium;
   sib7->tReselectionGERANSFHigh          = lteeNodeBparams.sib7CfgGrp.tReselectionGERANSFHigh;
   /*Config SIB7 Params*/

   cfgGrpIndex++; 

   if(lteeNodeBparams.lteNeighCellCfg.numCdma1xCells > 0)
   {
      /*Config SIB8 Params*/
      smBldSib8Cfg(lwrMng, cfgGrpIndex);
      cfgGrpIndex++;
   }

   /*Config SIB9 Params*/
   lwrMng->t.protoCfg.t.cellCfg.t.addCell.cellCfgGrp[cfgGrpIndex]->grpType =
      WR_CELL_CFG_GRP_SIB9;
   sib9 = &(lwrMng->t.protoCfg.t.cellCfg.t.addCell.cellCfgGrp[cfgGrpIndex]->t.sib9CfgGrp);
   /*cmMemcpy(sib9->henbName, "RADISYS", 8);*/
   cmMemcpy(sib9->henbName, lteeNodeBparams.lteCellSib9CfgGrp.henbName,
         strlen((S8 *)lteeNodeBparams.lteCellSib9CfgGrp.henbName));
   sib9->henbName[strlen((S8 *)lteeNodeBparams.lteCellSib9CfgGrp.henbName)] = '\0';
   cfgGrpIndex++;

   /* Build the SIB Scheduling confuration parameters */
   smBldCellSibSchCfg(lwrMng, cfgGrpIndex);
   cfgGrpIndex++; 

   smBldCellEaidCfg(lwrMng, cfgGrpIndex);
   cfgGrpIndex++; 

   /* Build the Measurement  configuration parameters */
   smBldMeasCellCfg(lwrMng, cfgGrpIndex);
   cfgGrpIndex++; 

   /* Build the ANR confuration parameters */
   smBldCellAnrCfg(lwrMng, cfgGrpIndex); 
   cfgGrpIndex++; 

   /* Build the rab policy configuration parameter */
   smBldRabPolicy(lwrMng, cfgGrpIndex);
   cfgGrpIndex++;

   
   tmrCfg->s1ResetTmr                       = lteeNodeBparams.lteProcTimerCfg.s1ResetTmr; 
   tmrCfg->x2ResetTmr                       = lteeNodeBparams.lteProcTimerCfg.x2ResetTmr; 
   /* Fill the RRM config */
   lwrMng->t.protoCfg.t.cellCfg.t.addCell.cellCfgGrp[cfgGrpIndex]->grpType = 
      WR_CELL_CFG_GRP_RRM;
   lwrMng->t.protoCfg.t.cellCfg.t.addCell.cellCfgGrp[cfgGrpIndex]->
      t.rrmCfgGrp.enbCacInfo.maxEnbCallCount = smCfgCb.maxUeSupp;
   
   lwrMng->t.protoCfg.t.cellCfg.t.addCell.cellCfgGrp[cfgGrpIndex]->
       t.rrmCfgGrp.enbCacInfo.dfltRrmRntiSize = lteeNodeBparams.lteSmCellCfg.maxRrmRntis;
   lwrMng->t.protoCfg.t.cellCfg.t.addCell.cellCfgGrp[cfgGrpIndex]->
      t.rrmCfgGrp.enbCacInfo.dfltRrmStartPreamble = lteeNodeBparams.lteSmCellCfg.rrmPrmblStart;
   /* Using 2/3rd of dedicated preambles for HO and reamining for PDCCH 
    * order RACH purpose at MAC*/
   wrHoDedPreamCount = (lteeNodeBparams.lteSmCellCfg.maxRrmPrmbls * 2)/3;
   if(wrHoDedPreamCount)
   {
      lwrMng->t.protoCfg.t.cellCfg.t.addCell.cellCfgGrp[cfgGrpIndex]->
         t.rrmCfgGrp.enbCacInfo.dfltRrmNumPreamble = wrHoDedPreamCount;
   }
   else
   {/* Very less preambles are configure. Assigning complete 
       * for HO and none for pdcch order */
      lwrMng->t.protoCfg.t.cellCfg.t.addCell.cellCfgGrp[cfgGrpIndex]->
         t.rrmCfgGrp.enbCacInfo.dfltRrmNumPreamble = lteeNodeBparams.lteSmCellCfg.maxRrmPrmbls;
   }

   cfgGrpIndex++; 
   /*UL and DL scheduler config*/
   lwrMng->t.protoCfg.t.cellCfg.t.addCell.cellCfgGrp[cfgGrpIndex]->grpType = WR_CELL_CFG_GRP_ULSCHD;
   ulSchdCfgGrp = &lwrMng->t.protoCfg.t.cellCfg.t.addCell.cellCfgGrp[cfgGrpIndex]->t.ulSchdCfgGrp;
   ulSchdCfgGrp->ulSchedulingAlg = lteeNodeBparams.lteCellUlSchdCfgGrp.ulSchedulingAlg;
   ulSchdCfgGrp->pfsUlTptCoefficient = (U32)lteeNodeBparams.lteCellUlSchdCfgGrp.pfsUlTptCoefficient;
   ulSchdCfgGrp->pfsUlFairCoefficient = (U32)lteeNodeBparams.lteCellUlSchdCfgGrp.pfsUlFairCoefficient;
   ulSchdCfgGrp->maxUePerUlSf = lteeNodeBparams.lteCellUlSchdCfgGrp.maxUePerUlSf;
   ulSchdCfgGrp->maxUlUeNewTxPerTti = lteeNodeBparams.lteCellUlSchdCfgGrp.maxUlUeNewTxPerTti;
   
   for(U8 idx = 0; idx < MAX_NUM_QCI; idx++)
   {
      ulSchdCfgGrp->ulQciWgtList[idx] = lteeNodeBparams.lteCellUlSchdCfgGrp.ulQciWgtList[idx];
   }

   cfgGrpIndex++; 

   lwrMng->t.protoCfg.t.cellCfg.t.addCell.cellCfgGrp[cfgGrpIndex]->grpType = WR_CELL_CFG_GRP_DLSCHD;
   dlSchdCfgGrp = &lwrMng->t.protoCfg.t.cellCfg.t.addCell.cellCfgGrp[cfgGrpIndex]->t.dlSchdCfgGrp;
   dlSchdCfgGrp->dlSchedulingAlg = lteeNodeBparams.lteCellDlSchdCfgGrp.dlSchedulingAlg;
   dlSchdCfgGrp->pfsDlTptCoefficient = (U32)lteeNodeBparams.lteCellDlSchdCfgGrp.pfsDlTptCoefficient;
   dlSchdCfgGrp->pfsDlFairCoefficient = (U32)lteeNodeBparams.lteCellDlSchdCfgGrp.pfsDlFairCoefficient;
   dlSchdCfgGrp->maxUePerDlSf = lteeNodeBparams.lteCellDlSchdCfgGrp.maxUePerDlSf;
   dlSchdCfgGrp->dlfsSchdType = lteeNodeBparams.lteCellDlSchdCfgGrp.dlfsSchd;
   wrSmDfltMaxUePerDlSf = lteeNodeBparams.lteCellDlSchdCfgGrp.maxUePerDlSf;
   dlSchdCfgGrp->maxDlUeNewTxPerTti = lteeNodeBparams.lteCellDlSchdCfgGrp.maxDlUeNewTxPerTti;
   dlSchdCfgGrp->dlfsSchd = lteeNodeBparams.lteCellDlSchdCfgGrp.dlfsSchd;

   for(U8 idx = 0; idx < MAX_NUM_QCI; idx++)
   {
      dlSchdCfgGrp->dlQciWgtList[idx] = lteeNodeBparams.lteCellDlSchdCfgGrp.dlQciWgtList[idx];
   }
#ifdef INTEL_FDD_NODLFSS
    if(dlSchdCfgGrp->dlfsSchdType == 1)
    {
       RLOG0(L_ERROR,"DLFSS is not supported in INTEL FDD, so disabling it for "
          "cell configuration");
       dlSchdCfgGrp->dlfsSchdType = 0;
       dlSchdCfgGrp->dlfsSchd = 0;
    }
#endif
   cfgGrpIndex++; 
 

   wrSmDfltPucchPwrFmt3RntiSt = lteeNodeBparams.ltecellSchdPwrCfg.pucchPwrFmt3Rnti;
   wrSmDfltPucchPwrFmt3RntiSz = lteeNodeBparams.ltecellSchdPwrCfg.pucchPwrFmt3Siz;

   wrSmDfltPucchPwrFmt3aRntiSt = lteeNodeBparams.ltecellSchdPwrCfg.pucchPwrFmt3aRnti;
   wrSmDfltPucchPwrFmt3aRntiSz = lteeNodeBparams.ltecellSchdPwrCfg.pucchPwrFmt3aSiz;

   wrSmDfltPuschPwrFmt3RntiSt  = lteeNodeBparams.ltecellSchdPwrCfg.puschPwrFmt3Rnti;
   wrSmDfltPuschPwrFmt3RntiSz  = lteeNodeBparams.ltecellSchdPwrCfg.puschPwrFmt3Siz;

   wrSmDfltPuschPwrFmt3aRntiSt = lteeNodeBparams.ltecellSchdPwrCfg.puschPwrFmt3aRnti;
   wrSmDfltPuschPwrFmt3aRntiSz = lteeNodeBparams.ltecellSchdPwrCfg.pucchPwrFmt3aSiz;

   wrSmDfltPucchPwrFmt3AEnable = lteeNodeBparams.ltecellSchdPwrCfg.pucchPwrFmt3aEnable;
   wrSmDfltPuschPwrFmt3AEnable = lteeNodeBparams.ltecellSchdPwrCfg.puschPwrFmt3aEnable;

   wrSmDfltisUlGrpPwrCntrlPucchEnable = lteeNodeBparams.ltecellSchdPwrCfg.isUlGrpPwrCntrlPucchEnable;
   wrSmDfltisUlGrpPwrCntrlPuschEnable = lteeNodeBparams.ltecellSchdPwrCfg.isUlGrpPwrCntrlPuschEnable;
  

   lwrMng->t.protoCfg.t.cellCfg.t.addCell.cellCfgGrp[cfgGrpIndex]->grpType =
                        WR_CELL_CFG_GRP_ULPWRCNTRL;
   lwrMng->t.protoCfg.t.cellCfg.t.addCell.cellCfgGrp[cfgGrpIndex]->
            t.uLPwrCtrl.p0UePUSCH = wrSmDfltP0UEPUSCH;
   lwrMng->t.protoCfg.t.cellCfg.t.addCell.cellCfgGrp[cfgGrpIndex]->
               t.uLPwrCtrl.accumulationEnabled = wrSmDfltAccumulationEnabled;
   lwrMng->t.protoCfg.t.cellCfg.t.addCell.cellCfgGrp[cfgGrpIndex]->
                  t.uLPwrCtrl.deltaMCSenabled = wrSmDfltDeltaMCSEnabled;
   lwrMng->t.protoCfg.t.cellCfg.t.addCell.cellCfgGrp[cfgGrpIndex]->
                     t.uLPwrCtrl.p0UePUCCH = wrSmDfltP0UEPUCCH;
   lwrMng->t.protoCfg.t.cellCfg.t.addCell.cellCfgGrp[cfgGrpIndex]->
                        t.uLPwrCtrl.pSRSoffset = wrSmDfltpSRSOffset;
   lwrMng->t.protoCfg.t.cellCfg.t.addCell.cellCfgGrp[cfgGrpIndex]->
                           t.uLPwrCtrl.filterCoefficient = wrSmDfltFilterCoef;
   cfgGrpIndex++;
   /* Fill MAC config*/
   lwrMng->t.protoCfg.t.cellCfg.t.addCell.cellCfgGrp[cfgGrpIndex]->grpType =
            WR_CELL_CFG_GRP_RGR;
   rgrCfg = &(lwrMng->t.protoCfg.t.cellCfg.t.addCell.cellCfgGrp[cfgGrpIndex]->t.rgrCfgGrp);
   
   rgrCfg->dfltStartRnti = lteeNodeBparams.lteSmCellCfg.macRntiStart;
   rgrCfg->dfltRntiSize = lteeNodeBparams.lteSmCellCfg.maxMacRntis;
   rgrCfg->dfltCfiCfg = lteeNodeBparams.lteSmCellCfg.dfltCfiCfg;
   rgrCfg->dfltTrgUlCqi = wrSmDfltTrgUlCqi;
   rgrCfg->dfltBcchPchRaCodeRate = wrSmDfltBcchPchRaCodeRate;
   rgrCfg->dfltPdcchCodeRate = wrSmDfltPdcchCodeRate;
   rgrCfg->dfltPuschSubBndStrt = wrSmDfltPuschSubBndStrt;
   rgrCfg->dfltPuschNoSubbnds = wrSmDfltPuschNoSubbnds;
   rgrCfg->dfltULCmnCodRateCcchCqi = wrSmDfltULCmnCodRateCcchCqi;
   rgrCfg->dfltDLCmnCodRateCcchCqi = wrSmDfltDlCmnCodRateCcchCqi;
   rgrCfg->dfltIsDlFreqSel = lteeNodeBparams.lteCellDlSchdCfgGrp.dlfsSchd;
#ifdef INTEL_FDD_NODLFSS
    if(rgrCfg->dfltIsDlFreqSel == 1)
    {
       RLOG0(L_ERROR,"DLFSS is not supported in INTEL FDD, so disabling it for "
          "MAC configuration");
       rgrCfg->dfltIsDlFreqSel = 0;
    }
#endif
   rgrCfg->dfltThresholdCqi = wrSmDfltThresholdCqi;
   rgrCfg->dfltMaxDlHqTx = wrSmDfltMaxDlHqTx;
   rgrCfg->dfltMaxMsg4HqTx = wrSmDfltMaxMsg4HqTx;
   rgrCfg->dfltPuschSubBandSz = wrSmDfltPuschSubBandSz;
   rgrCfg->dfltPuschSubBandDmrs = wrSmDfltPuschSubBandDmrs;
   rgrCfg->dfltMaxPucchRbs = wrSmDfltMaxPucchRbs;
   cfgGrpIndex++;

   /* Fill CTF configuration */
   lwrMng->t.protoCfg.t.cellCfg.t.addCell.cellCfgGrp[cfgGrpIndex]->grpType =
               WR_CELL_CFG_GRP_CTF;
   ctfCfg = &(lwrMng->t.protoCfg.t.cellCfg.t.addCell.cellCfgGrp[cfgGrpIndex]->t.ctfCfgGrp); 
   ctfCfg->dfltScSpacing = wrSmDfltScSpacing;
   ctfCfg->dfltOpMode = wrSmDfltOpMode;
   ctfCfg->dfltTagPeriod = wrSmDfltTagPeriod;
   cfgGrpIndex++;

   /* Fill RLC configuration */
   lwrMng->t.protoCfg.t.cellCfg.t.addCell.cellCfgGrp[cfgGrpIndex]->grpType = 
      WR_CELL_CFG_GRP_RLC;
   rlcCfg = &(lwrMng->t.protoCfg.t.cellCfg.t.addCell.cellCfgGrp[cfgGrpIndex]->t.rlcCfgGrp);
   rlcCfg->ulModeInfo.dfltAmRlcSrbtStatusProh = wrSmDfltAmRlcSrbtStatusProh;
   rlcCfg->ulModeInfo.dfltAmRlcDrbtStatusProh = wrSmDfltAmRlcDrbtStatusProh;
   rlcCfg->ulModeInfo.dfltUmRlcsnFieldLen = wrSmDfltUmRlcsnFieldLen;
   rlcCfg->ulModeInfo.drbtReordring = wrSmDfltUlDrbtReordring;
   rlcCfg->ulModeInfo.srbtReordring = wrSmDfltUlSrbtReordring;

   rlcCfg->dlModeInfo.dfltAmRlcSrbmaxRetxThresh = wrSmDfltAmRlcSrbmaxRetxThresh;
   rlcCfg->dlModeInfo.dfltAmRlcSrbmaxpollByte = wrSmDfltAmRlcSrbmaxpollByte;
   rlcCfg->dlModeInfo.dfltAmRlcSrbpollPdu = wrSmDfltAmRlcSrbpollPdu;
   rlcCfg->dlModeInfo.dfltAmRlcSrbtPollRetx = wrSmDfltAmRlcSrbtPollRetx;
   rlcCfg->dlModeInfo.dfltAmRlcDrbmaxRetxThresh = wrSmDfltAmRlcDrbmaxRetxThresh;
   rlcCfg->dlModeInfo.dfltAmRlcDrbmaxpollByte = wrSmDfltAmRlcDrbmaxpollByte;
   rlcCfg->dlModeInfo.dfltAmRlcDrbpollPdu = wrSmDfltAmRlcDrbpollPdu;
   rlcCfg->dlModeInfo.dfltAmRlcDrbtPollRetx = wrSmDfltAmRlcDrbtPollRetx;
   rlcCfg->dlModeInfo.dfltUmRlcsnFieldLen = wrSmDfltUmRlcsnFieldLen;
   rlcCfg->dlModeInfo.srbtReordring = wrSmDfltDlSrbtReordring;
   rlcCfg->dlModeInfo.drbtReordring = wrSmDfltDlDrbtReordring;
   
   cfgGrpIndex++;

   schdCfg = &(lwrMng->t.protoCfg.t.cellCfg.t.addCell.cellCfgGrp[cfgGrpIndex]->t.schdCfgGrp);
   lwrMng->t.protoCfg.t.cellCfg.t.addCell.cellCfgGrp[cfgGrpIndex]->grpType = 
      WR_CELL_CFG_GRP_SCHD;
   schdCfg->prdEnum = wrSmDfltSrsCfgPrdEnum;
   schdCfg->retxCnt = wrSmDfltRetxCnt;
   WR_GET_PAGING_CYCLE(wrSmDfltDefaultPagCycle, dfltDrxCycle);
   WR_GET_MODIFI_COEFF(wrSmDfltModPeriodCoeff, dfltModPrdCoeff);
   schdCfg->modPrd = dfltModPrdCoeff * dfltDrxCycle;
   schdCfg->pwrCfg.pucchPwrFmt3Rnti = wrSmDfltPucchPwrFmt3RntiSt;
   schdCfg->pwrCfg.pucchPwrFmt3Siz = wrSmDfltPucchPwrFmt3RntiSz;
   schdCfg->pwrCfg.pucchPwrFmt3aRnti = wrSmDfltPucchPwrFmt3aRntiSt;
   schdCfg->pwrCfg.pucchPwrFmt3aSiz = wrSmDfltPucchPwrFmt3aRntiSz;
   schdCfg->pwrCfg.puschPwrFmt3Rnti = wrSmDfltPuschPwrFmt3RntiSt;
   schdCfg->pwrCfg.puschPwrFmt3Siz = wrSmDfltPuschPwrFmt3RntiSz;
   schdCfg->pwrCfg.puschPwrFmt3aRnti = wrSmDfltPuschPwrFmt3aRntiSt;
   schdCfg->pwrCfg.puschPwrFmt3aSiz = wrSmDfltPuschPwrFmt3aRntiSz;
   if((wrHoDedPreamCount) && 
      (lteeNodeBparams.lteSmCellCfg.maxRrmPrmbls - wrHoDedPreamCount))   
   {/* Assigning the remaining ded preambles to MAC for
       * PDCCH order RACH */
      schdCfg->preamPres  = TRUE;
      schdCfg->preamStart = lteeNodeBparams.lteSmCellCfg.rrmPrmblStart + 
                            wrHoDedPreamCount;
      schdCfg->preamSize  = lteeNodeBparams.lteSmCellCfg.maxRrmPrmbls - 
                            wrHoDedPreamCount;
   }
   else
   {
      schdCfg->preamPres  = FALSE;
      schdCfg->preamStart = 0;
      schdCfg->preamSize  = 0;
   }

   schdCfg->rachCfg.preambleFormat = wrSmDfltPreambleFrmt;
   schdCfg->rachCfg.raOccSize = wrSmDfltRaOccSz;
   schdCfg->rachCfg.raOccSfnEnum = wrSmDfltRaOccSfnEnum;
   schdCfg->rachCfg.prachResource = wrSmDfltPrachResource;
   schdCfg->genCfg.macInst = wrSmDfltMacInst;
   schdCfg->genCfg.maxUePerUlSf = lteeNodeBparams.lteCellUlSchdCfgGrp.maxUePerUlSf;
   schdCfg->genCfg.maxUePerDlSf = lteeNodeBparams.lteCellDlSchdCfgGrp.maxUePerDlSf;
   schdCfg->genCfg.maxUlBwPerUe = wrSmDfltMaxUlBwPerUe;
   schdCfg->genCfg.maxDlBwPerUe = wrSmDfltMaxDlBwPerUe;
   schdCfg->genCfg.maxDlRetxBw = wrSmDfltMaxDlRetxBw;
   schdCfg->genCfg.maxDlUeNewTxPerTti = lteeNodeBparams.lteCellDlSchdCfgGrp.maxDlUeNewTxPerTti;
   schdCfg->genCfg.maxUlUeNewTxPerTti = lteeNodeBparams.lteCellUlSchdCfgGrp.maxUlUeNewTxPerTti;
   schdCfg->genCfg.cellModSchm = wrSmDfltCellModSchm;
   /* SPS changes starts */
   schdCfg->spsCfg.bIsSpsEnabled = lteeNodeBparams.stSpsCellCfg.bIsSpsEnabled;
   if (schdCfg->spsCfg.bIsSpsEnabled == TRUE)
   {
      schdCfg->spsCfg.maxDlSpsUePerTti = lteeNodeBparams.stSpsCellCfg.maxDlSpsUePerTti;
      schdCfg->spsCfg.maxUlSpsUePerTti = lteeNodeBparams.stSpsCellCfg.maxUlSpsUePerTti;
      schdCfg->spsCfg.maxDlBwInRbs = lteeNodeBparams.stSpsCellCfg.maxDlBwInRbs;
   }
   /* SPS changes ends */
   
   schdCfg->isDynCfiEnb = lteeNodeBparams.lteDynCfiParams.isDynCfiEnb;

   cfgGrpIndex++; 

   csfbGrp = &(lwrMng->t.protoCfg.t.cellCfg.t.addCell.cellCfgGrp[cfgGrpIndex]->t.csfbCfgGrp);
   lwrMng->t.protoCfg.t.cellCfg.t.addCell.cellCfgGrp[cfgGrpIndex]->grpType = WR_CELL_CFG_GRP_CSFB;
   csfbGrp->eCsfbMeasSup = lteeNodeBparams.csfbCfgGrp.ecsfbSupport;
   csfbGrp->dualRxSup = lteeNodeBparams.csfbCfgGrp.dualRxSup;
   RLOG1(L_INFO, "eCSFB support configuration from OAM : %d",
      lteeNodeBparams.csfbCfgGrp.ecsfbSupport);
/*RRC Rel10 Upgrade - start*/
   if (TRUE == lteeNodeBparams.csfbCfgGrp.dualRxTxSup)
   {
      if(LWR_ECSFB_NOT_SUPPORTED == csfbGrp->eCsfbMeasSup) 
      {
         RLOG2(L_ERROR, "Invalid configuration for parameter dualRxTxSup." 
            "eCSFB should be supported (currently %d) inorder to support "
	    "DualRxTx configuration(%d)",csfbGrp->eCsfbMeasSup,
            lteeNodeBparams.csfbCfgGrp.dualRxTxSup);
	 		
	  RETVALUE(RFAILED);
       }
   }

   RLOG1(L_INFO, "dualRxTx support fom oam : %d",
      lteeNodeBparams.csfbCfgGrp.dualRxTxSup);
   csfbGrp->dualRxTxSup = lteeNodeBparams.csfbCfgGrp.dualRxTxSup;
   csfbGrp->cdma1xRttSup = wrSmDfltCdma1xrttRegSup;

/*RRC Rel10 Upgrade - end*/
   csfbGrp->csfbToUtraMeasSup = lteeNodeBparams.csfbCfgGrp.csfbToUtranWithMeasEnabled;
   csfbGrp->csfbToCdmaMeasSup = lteeNodeBparams.csfbCfgGrp.csfbSupport;
   csfbGrp->cdma1xrttCsfbCfg.secId.isOctStrPres = NOTPRSNT;
   csfbGrp->cdma1xrttCsfbCfg.secId.cdmaType = wrSmDfltCellType;
   if(FALSE == csfbGrp->cdma1xrttCsfbCfg.secId.isOctStrPres)
   {
      csfbGrp->cdma1xrttCsfbCfg.secId.s.cdmaSectorId.marketId = lteeNodeBparams.csfbCfgGrp.marketId;
      csfbGrp->cdma1xrttCsfbCfg.secId.s.cdmaSectorId.switchNum = lteeNodeBparams.csfbCfgGrp.switchNum;
      csfbGrp->cdma1xrttCsfbCfg.secId.s.cdmaSectorId.cellId = wrSmDfltCdmaCellId;
      csfbGrp->cdma1xrttCsfbCfg.secId.s.cdmaSectorId.sectorNum = lteeNodeBparams.csfbCfgGrp.sectorNum;
   }
   else
   {
      wrFillSectorId(&csfbGrp->cdma1xrttCsfbCfg.secId.s.sectorIdPtr, 7,
            (Data *)&wrSmDfltSecId);
   }
   smBuildMobParams(&csfbGrp->cdma1xrttCsfbCfg.mobParams);
   cfgGrpIndex++; 

   lwrMng->t.protoCfg.t.cellCfg.t.addCell.numOfGrps = cfgGrpIndex;
   enbCfg->txPrefMode = lteeNodeBparams.lteTransModeCellCfg.transModeUse;
   enbCfg->isAutoCfgModeEnb = lteeNodeBparams.lteTransModeCellCfg.bIsAutoConfigEnabled;

   /*PHY parameters*/
    enbCfg->shutdownPwrCfg.lwPwrCfg.lowRefSigPwr = wrSmDfltLowRefSigPwr;
    enbCfg->shutdownPwrCfg.lwPwrCfg.lowPilotSigPwr = wrSmDfltLowPilotSigPwr;
    enbCfg->shutdownPwrCfg.lwPwrCfg.lowPriSigPwr = wrSmDfltLowPriSigPwr;
    enbCfg->shutdownPwrCfg.lwPwrCfg.lowSecSigPwr = wrSmDfltLowSecSigPwr;
    enbCfg->shutdownPwrCfg.hiPwrCfg.highRefSigPwr = wrSmDfltRefSigPwr; 
    enbCfg->shutdownPwrCfg.hiPwrCfg.highPilotSigPwr = wrSmDfltPilotSigPwr;
    enbCfg->shutdownPwrCfg.hiPwrCfg.highPriSigPwr = wrSmDfltPriSigPwr;
    enbCfg->shutdownPwrCfg.hiPwrCfg.highSecSigPwr = wrSmDfltSecSigPwr;
    
   RETVALUE(ROK);
} /* end of smBuildCellCfg */

/*
 * 
 *        Fun:   smFillX2InitCntrl
 * 
 *        Desc:  fill in cellCfg for WR
 * 
 *        Ret:   ROK
 * 
 *        Notes: None
 *               
 *        File:  wr_sm_wr_cntrl.c 
 */
#ifdef ANSI
PUBLIC S16 smFillX2InitCntrl
(
LwrMngmt *cntrl 
)
#else /* if ANSI not defined*/
PUBLIC S16 smFillX2InitCntrl(lwrMng)
LwrMngmt *cntrl
#endif /* end of ANSI*/
{
   U8       noOfCfg; 

   TRC2(smFillX2InitCntrl)

   for ( noOfCfg=0; noOfCfg<smCfgCb.noOfNghCfg; noOfCfg++)
   {
      cntrl->t.cntrl.idLst[noOfCfg] = smCfgCb.wrNghCellCfg[noOfCfg]->peerId;
   }
   cntrl->t.cntrl.numConn = noOfCfg;
 
   RETVALUE(ROK);
} /* end of smFillX2InitCntrl */

/*
 * 
 *       Fun:    SmMiLwrAddNeighCfgReq - Neighbor Config Request
 * 
 *       Desc:   Process the Neighbor Config Request
 *
 *       Ret:    ROK     - succeeded
 *               RFAILED - failed
 *
 *       Notes:  None
 *
 *       File:   wr_sm_wr_ptmi.c
 *
 */

#ifdef ANSI
PUBLIC S16 SmMiLwrAddNeighCfgReq
(
Pst          *pst,
LwrMngmt     *cfgReq
)
#else
PUBLIC S16 SmMiLwrAddNeighCfgReq(pst, cfgReq)
Pst          *pst;
LwrMngmt     *cfgReq;
#endif /* ANSI */
{
   SmNeigEnbList  *nghEnbCfg = NULLP;

   TRC2(SmMiLwrAddNeighCfgReq)
   if(cfgReq == NULLP)
   {
      RETVALUE(RFAILED);
   }

   if(ROK != cmHashListFind(&(smCfgCb.neighEnbLstCp), (U8*)&cfgReq->t.nbrAddReq.enbId,
            sizeof(cfgReq->t.nbrAddReq.enbId), 0,(PTR*) &nghEnbCfg))
   {
      RLOG1(L_ERROR, "Neighbour eNodeB %ld not configurured",
         cfgReq->t.nbrAddReq.enbId);
      RETVALUE(RFAILED);
   }
   wrCpyCmTptAddr(&nghEnbCfg->nghEnbCfg.ipAddr, &cfgReq->t.nbrAddReq.nbIpAddress);
   WR_GET_PEERID_FROM_ENBID(cfgReq->t.nbrAddReq.enbId,nghEnbCfg->nghEnbCfg.peerId);
   WR_FREE(cfgReq, sizeof(LwrMngmt));

   smBuildDynCzPeerCfg(&nghEnbCfg->nghEnbCfg);

   RETVALUE(ROK);
} /* end of SmMiLwrAddNeighCfgReq() */
PRIVATE S16 fillMsmGeranCell(LwrGERANNeighCell *ipCell, MsmLteGERANNeighCell *opCell)
{
   cmMemset((U8*)opCell,0,sizeof(MsmLteGERANNeighCell));

   opCell->lac = ipCell->lac;
   opCell->rac = ipCell->rac;
   opCell->bsic = ipCell->bsic;
   opCell->ci = ipCell->ci;
   opCell->qoffsetGERAN = ipCell->offsetFreq;
   opCell->bandIndicator = ipCell->bandIndicator;
   opCell->bcchArfcn = ipCell->bcchArfcn;
   opCell->ncc_PermittedMeas = ipCell->nccPermittedMeas;
   opCell->isDTMCapable = ipCell->isDTMCapable;
   opCell->rimSupportForGeranNgh = ipCell->rimSupportForGeranNgh;
   opCell->networkControlOrder = ipCell->ncoVal;
   opCell->networkControlOrderPres = ipCell->ncoPres;
   opCell->qoffsetGERAN = ipCell->offsetFreq;

   opCell->plmnId.numMncDigits = ipCell->plmnId.numMncDigits;
   opCell->plmnId.mcc[0] = ipCell->plmnId.mcc[0];
   opCell->plmnId.mcc[1] = ipCell->plmnId.mcc[1];
   opCell->plmnId.mcc[2] = ipCell->plmnId.mcc[2];
   opCell->plmnId.mnc[0] = ipCell->plmnId.mnc[0];
   opCell->plmnId.mnc[1] = ipCell->plmnId.mnc[1];
   opCell->plmnId.mnc[2] = ipCell->plmnId.mnc[2];

   RETVALUE(ROK);
}

PRIVATE S16 fillMsmEutraCell(LwrEUTRANeighCell *ipCell, MsmEUTRANeighCell *opCell)
{
   cmMemset((U8*)opCell,0,sizeof(MsmEUTRANeighCell));

   opCell->cid = ipCell->enbId;
   opCell->pci = ipCell->pci;
   opCell->tac = ipCell->tac;

   opCell->numPlmn = ipCell->numPlmn;
   opCell->plmnId[0].numMncDigits = ipCell->enbPlmn.numMncDigits;
   opCell->plmnId[0].mcc[0] = ipCell->enbPlmn.mcc[0];
   opCell->plmnId[0].mcc[1] = ipCell->enbPlmn.mcc[1];
   opCell->plmnId[0].mcc[2] = ipCell->enbPlmn.mcc[2];
   opCell->plmnId[0].mnc[0] = ipCell->enbPlmn.mnc[0];
   opCell->plmnId[0].mnc[1] = ipCell->enbPlmn.mnc[1];
   opCell->plmnId[0].mnc[2] = ipCell->enbPlmn.mnc[2];
   opCell->earfcnDl = ipCell->earfcnDl;
   opCell->earfcnUl = ipCell->earfcnUl;
   opCell->cio = ipCell->cio;
   opCell->qoffset = ipCell->qoffset;
   opCell->RSTxPower = ipCell->refSigPower;
   opCell->BlackListed = ipCell->blackListed;
   opCell->csgAccessMode = ipCell->csgAccessMode;
   opCell->csgId = ipCell->csgId;
   opCell->numAntenna = ipCell->numAntenna;
   opCell->ulDlSubFrameCfg = ipCell->ulDlSubFrameCfg;
   opCell->splSfCfg = ipCell->splSfCfg;
   opCell->cpDl = ipCell->cpDl;
   opCell->cpUl = ipCell->cpUl;
   opCell->rsrp = ipCell->rsrp;
   opCell->rssi = ipCell->rssi;
   opCell->ulBw = ipCell->ulBw;
   opCell->dlBw = ipCell->dlBw;
   //opCell->lteBand = ipCell->
   //opCell->isCellBarred = ipCell->
   //opCell->isPrachConfig = ipCell->
   //opCell->prachCfgInfo = ipCell->
   //opCell->numIntraFreqNeighbors = ipCell-> 
   //opCell->intraFreqNghPci = ipCell->
   
   RETVALUE(ROK);
}

PRIVATE S16 fillMsmUtraCell(LwrUtraNeighCell *ipCell, MsmUtraNeighCell *opCell)
{
   U32 idx;

   cmMemset((U8*)opCell,0,sizeof(MsmUtraNeighCell));

   opCell->rncId = ipCell->rncId;
   opCell->cId = ipCell->cId;
   opCell->lac = ipCell->lac;
   opCell->rac = ipCell->rac;

   opCell->numPlmn = ipCell->numPlmn;
   for (idx=0; idx < ipCell->numPlmn; idx++)
   {
      opCell->plmnId[idx].numMncDigits = ipCell->plmnId[idx].numMncDigits;
      opCell->plmnId[idx].mcc[0] = ipCell->plmnId[idx].mcc[0];
      opCell->plmnId[idx].mcc[1] = ipCell->plmnId[idx].mcc[1];
      opCell->plmnId[idx].mcc[2] = ipCell->plmnId[idx].mcc[2];
      opCell->plmnId[idx].mnc[0] = ipCell->plmnId[idx].mnc[0];
      opCell->plmnId[idx].mnc[1] = ipCell->plmnId[idx].mnc[1];
      opCell->plmnId[idx].mnc[2] = ipCell->plmnId[idx].mnc[2];
   }

   opCell->psc = ipCell->t.utraFdd.psc;
   opCell->arfcnUl = ipCell->t.utraFdd.arfcnUl;
   opCell->arfcnDl = ipCell->t.utraFdd.arfcnDl;
   opCell->duplexMode = MSM_UTRA_DUPLEX_MODE_FDD;
   opCell->isRimSupported = ipCell->isRimSupported;
   opCell->isVoipCapable = ipCell->isVoipCapable;
   opCell->isPsHOCapable = ipCell->isPsHOCapable;

   RETVALUE(ROK);
}

/*
 *
 *       Fun:    SmMiLwrHdlNghCellInd- Neighbor Cell Indication
 *
 *       Desc:   Process the Neighbor cell indication from application
 *              
 *
 *       Ret:    ROK     - succeeded
 *               RFAILED - failed
 *
 *       Notes:  None
 *
 *       File:   wr_sm_wr_ptmi.c
 *
 */

#ifdef ANSI
PUBLIC S16 SmMiLwrHdlNghCellInd 
(
Pst          *pst,
LwrNghCellInd *nghCellInfo
)
#else
PUBLIC S16 SmMiLwrHdlNghCellInd(pst, nghCellInfo)
Pst          *pst;
LwrNghCellInd     *nghCellInfo;
#endif /* ANSI */
{
   U32 action;
   MsmEUTRANeighCell msmEutranCell;
   MsmUtraNeighCell  msmUtranCell;
   MsmLteGERANNeighCell msmGeranCell;

   action = nghCellInfo->action;

   if (action == WR_ACT_ADD)
   {
      if (nghCellInfo->t.neighAddCfg.type == WR_NEIGH_CELL_EUTRA)
      {
         fillMsmEutraCell(&nghCellInfo->t.neighAddCfg.t.eutraCell, &msmEutranCell);
         SmUpdateMibParms(MSM_LTE_EUTRAN_NEIGHBORLIST_IN_USE_PARAMETERS, MSM_ACT_ADD,
           (void*)&msmEutranCell);
      }
      else if (nghCellInfo->t.neighAddCfg.type == WR_NEIGH_CELL_UTRA_FDD)
      {
         fillMsmUtraCell(&nghCellInfo->t.neighAddCfg.t.utranCell, &msmUtranCell);
         SmUpdateMibParms(MSM_LTE_UTRA_NEIGHBORLIST_IN_USE_PARAMETERS, MSM_ACT_ADD,
               (void*)&msmUtranCell);
      }
      else if(nghCellInfo->t.neighAddCfg.type == WR_NEIGH_CELL_GERAN)
      {
         fillMsmGeranCell(&nghCellInfo->t.neighAddCfg.t.geranCell, &msmGeranCell);
         SmUpdateMibParms(MSM_GERAN_NEIGHBOR_CELL_IN_USE_PARAMETERS, MSM_ACT_ADD, (void*)&msmGeranCell);
      }
   }
   else if (action == WR_ACT_MOD)
   {
      if (nghCellInfo->t.neighModCfg.type == WR_NEIGH_CELL_EUTRA)
      {
         fillMsmEutraCell(&nghCellInfo->t.neighModCfg.t.eutraCell, &msmEutranCell);
         SmUpdateMibParms(MSM_LTE_EUTRAN_NEIGHBORLIST_IN_USE_PARAMETERS, MSM_ACT_MOD,
            (void*)&msmEutranCell);
      }
      else if (nghCellInfo->t.neighModCfg.type == WR_NEIGH_CELL_UTRA_FDD)
      {
         fillMsmUtraCell(&nghCellInfo->t.neighModCfg.t.utranCell, &msmUtranCell);
         SmUpdateMibParms(MSM_LTE_UTRA_NEIGHBORLIST_IN_USE_PARAMETERS, MSM_ACT_MOD,
            (void*)&msmUtranCell);
      }
   }
   else if (action == WR_ACT_DEL)
   {
      if (nghCellInfo->t.neighDelCfg.type == WR_NEIGH_CELL_EUTRA)
      {
         msmEutranCell.cid = nghCellInfo->t.neighDelCfg.t.eutranCell.cid;
         msmEutranCell.plmnId[0].numMncDigits = nghCellInfo->t.neighDelCfg.t.eutranCell.plmnId.numMncDigits;
         msmEutranCell.plmnId[0].mcc[0] = nghCellInfo->t.neighDelCfg.t.eutranCell.plmnId.mcc[0];
         msmEutranCell.plmnId[0].mcc[1] = nghCellInfo->t.neighDelCfg.t.eutranCell.plmnId.mcc[1];
         msmEutranCell.plmnId[0].mcc[2] = nghCellInfo->t.neighDelCfg.t.eutranCell.plmnId.mcc[2];

         msmEutranCell.plmnId[0].mnc[0] = nghCellInfo->t.neighDelCfg.t.eutranCell.plmnId.mnc[0];
         msmEutranCell.plmnId[0].mnc[1] = nghCellInfo->t.neighDelCfg.t.eutranCell.plmnId.mnc[1];
         msmEutranCell.plmnId[0].mnc[2] = nghCellInfo->t.neighDelCfg.t.eutranCell.plmnId.mnc[2];

         SmUpdateMibParms(MSM_LTE_EUTRAN_NEIGHBORLIST_IN_USE_PARAMETERS, MSM_ACT_DEL,
            (void*)&msmEutranCell);
      }
      else if (nghCellInfo->t.neighDelCfg.type == WR_NEIGH_CELL_UTRA_FDD)
      {
         msmUtranCell.cId = nghCellInfo->t.neighDelCfg.t.utranCell.cid;
         msmUtranCell.plmnId[0].numMncDigits = nghCellInfo->t.neighDelCfg.t.utranCell.plmnId.numMncDigits;
         msmUtranCell.plmnId[0].mcc[0] = nghCellInfo->t.neighDelCfg.t.utranCell.plmnId.mcc[0];
         msmUtranCell.plmnId[0].mcc[1] = nghCellInfo->t.neighDelCfg.t.utranCell.plmnId.mcc[1];
         msmUtranCell.plmnId[0].mcc[2] = nghCellInfo->t.neighDelCfg.t.utranCell.plmnId.mcc[2];

         msmUtranCell.plmnId[0].mnc[0] = nghCellInfo->t.neighDelCfg.t.utranCell.plmnId.mnc[0];
         msmUtranCell.plmnId[0].mnc[1] = nghCellInfo->t.neighDelCfg.t.utranCell.plmnId.mnc[1];
         msmUtranCell.plmnId[0].mnc[2] = nghCellInfo->t.neighDelCfg.t.utranCell.plmnId.mnc[2];

         SmUpdateMibParms(MSM_LTE_UTRA_NEIGHBORLIST_IN_USE_PARAMETERS, MSM_ACT_DEL,
            (void*)&msmUtranCell);
      }
   }

   WR_FREE(nghCellInfo,sizeof(LwrNghCellInd));
   RETVALUE(ROK);
}



/*
 *
 *       Fun:    SmMiLwrUpDateNhgCellCfgReq - Neighbor Config Request
 *
 *       Desc:   Process the Neighbor Config Request
 *               new cell is detected through ANR and same cell info is updateing in MIB
 *
 *       Ret:    ROK     - succeeded
 *               RFAILED - failed
 *
 *       Notes:  None
 *
 *       File:   wr_sm_wr_ptmi.c
 *
 */

#ifdef ANSI
PUBLIC S16 SmMiLwrUpDateNhgCellCfgReq 
(
Pst          *pst,
LwrMngmt     *nghCellCfgReq
)
#else
PUBLIC S16 SmMiLwrUpDateNhgCellCfgReq(pst, nghCellCfgReq)
Pst          *pst;
LwrMngmt     *nghCellCfgReq;
#endif /* ANSI */
{
  LwrEUTRANeighCell *lwrEutraCell = NULLP;
  LwrUtraNeighCell   *lwrUtraCell = NULLP;
  MsmEUTRANeighCell  msmEutraCell;
  MsmUtraNeighCell   msmUtraCell; 
  U16                idx;

  TRC2(SmMiLwrUpDateNhgCellCfgReq)

  if(nghCellCfgReq->t.protoCfg.t.neighCellCfg.t.neighAddCfg[0].type == WR_NEIGH_CELL_EUTRA)
  {
     SmHdlNghEurtaAddCellCfg(&nghCellCfgReq->t.protoCfg.t.neighCellCfg.t.neighAddCfg[0].t.eutraCell);

     lwrEutraCell = &nghCellCfgReq->t.protoCfg.t.neighCellCfg.t.neighAddCfg[0].t.eutraCell;

     msmEutraCell.nrIndex     = lwrEutraCell->nrIndex;
     msmEutraCell.cid         = lwrEutraCell->enbId;
     msmEutraCell.pci         = lwrEutraCell->pci;
     msmEutraCell.numPlmn     = 1;
     msmEutraCell.plmnId[0].numMncDigits = lwrEutraCell->enbPlmn.numMncDigits;
     msmEutraCell.plmnId[0].mcc[0] = lwrEutraCell->enbPlmn.mcc[0];
     msmEutraCell.plmnId[0].mcc[1] = lwrEutraCell->enbPlmn.mcc[1];
     msmEutraCell.plmnId[0].mcc[2] = lwrEutraCell->enbPlmn.mcc[2];
     msmEutraCell.plmnId[0].mnc[0] = lwrEutraCell->enbPlmn.mnc[0];
     msmEutraCell.plmnId[0].mnc[1] = lwrEutraCell->enbPlmn.mnc[1];
     msmEutraCell.plmnId[0].mnc[2] = lwrEutraCell->enbPlmn.mnc[2];
     /*ccpu00132852*/
     msmEutraCell.tac         = lwrEutraCell->tac;
     msmEutraCell.earfcnDl    = lwrEutraCell->earfcnDl;
     msmEutraCell.earfcnUl    = lwrEutraCell->earfcnUl;
     msmEutraCell.cio         = lwrEutraCell->cio;
     msmEutraCell.qoffset     = lwrEutraCell->qoffset;
     msmEutraCell.RSTxPower   = lwrEutraCell->refSigPower;
     msmEutraCell.BlackListed = FALSE;
     /* CSG_DEV start */
     msmEutraCell.csgAccessMode = lwrEutraCell->csgAccessMode;
     msmEutraCell.csgId         = lwrEutraCell->csgId;
     /* CSG_DEV end */
  }  
  else if(nghCellCfgReq->t.protoCfg.t.neighCellCfg.t.neighAddCfg[0].type == WR_NEIGH_CELL_UTRA_FDD)
  {
     SmHdlNghUrtaAddCellCfg(&nghCellCfgReq->t.protoCfg.t.neighCellCfg.t.neighAddCfg[0].t.utranCell);

     lwrUtraCell = &nghCellCfgReq->t.protoCfg.t.neighCellCfg.t.neighAddCfg[0].t.utranCell;
     msmUtraCell.nrIndex = lwrUtraCell->nrIndex;
     msmUtraCell.rncId = lwrUtraCell->rncId;
     msmUtraCell.cId = lwrUtraCell->cId;
     msmUtraCell.lac = lwrUtraCell->lac;
     msmUtraCell.rac = lwrUtraCell->rac;
     msmUtraCell.numPlmn = lwrUtraCell->numPlmn;
     for(idx = 0; idx <lwrUtraCell->numPlmn; idx++)
     {
       msmUtraCell.plmnId[idx].numMncDigits = lwrUtraCell->plmnId[idx].numMncDigits;
       msmUtraCell.plmnId[idx].mcc[0] = lwrUtraCell->plmnId[idx].mcc[0];
       msmUtraCell.plmnId[idx].mcc[2] = lwrUtraCell->plmnId[idx].mcc[1];
       msmUtraCell.plmnId[idx].mcc[2] = lwrUtraCell->plmnId[idx].mcc[2];
       msmUtraCell.plmnId[idx].mnc[0] = lwrUtraCell->plmnId[idx].mnc[0];
       msmUtraCell.plmnId[idx].mnc[1] = lwrUtraCell->plmnId[idx].mnc[1];
       msmUtraCell.plmnId[idx].mnc[2] = lwrUtraCell->plmnId[idx].mnc[2];
     }
     msmUtraCell.psc = lwrUtraCell->t.utraFdd.psc;
     msmUtraCell.arfcnUl = lwrUtraCell->t.utraFdd.arfcnUl;
     msmUtraCell.arfcnDl = lwrUtraCell->t.utraFdd.arfcnDl;
     msmUtraCell.duplexMode = lwrUtraCell->duplexMode;
  }
  else
  {
     RLOG1(L_ERROR, "Addition of cell is not supported for neighbor cell of "
        "type %d", 
        nghCellCfgReq->t.protoCfg.t.neighCellCfg.t.neighAddCfg[0].type);
     /* Memory leak fix ccpu00145573 */
     WR_FREE(nghCellCfgReq, sizeof(LwrMngmt));
     RETVALUE(RFAILED);
  }
  /* Memory leak fix ccpu00145573 */
  WR_FREE(nghCellCfgReq, sizeof(LwrMngmt));
  /*NEED to update in MIB */
  RETVALUE(ROK);
}/*end of SmMiLwrUpDateNhgCellCfgReq*/

/*
 *
 *       Fun:    SmHdlNghEurtaAddCellCfg - Neighbor Config Request
 *
 *       Desc:   Process the Neighbor Config Request
 *               new cell is detected through ANR and same cell info is updateing in MIB
 *
 *       Ret:    ROK     - succeeded
 *               RFAILED - failed
 *
 *       Notes:  None
 *
 *       File:   wr_sm_wr_ptmi.c
 *
 */
#ifdef ANSI
PUBLIC S16 SmHdlNghEurtaAddCellCfg 
(
LwrEUTRANeighCell *eutraCell
)
#else
PUBLIC S16 SmHdlNghEurtaAddCellCfg(eutraCell)
LwrEUTRANeighCell  *eutraCell;
#endif /* ANSI */
{
   SmNeigEutraCellList *nghEutraCellList = NULLP;
   SmNeigEnbList       *nghEnbCfgList = NULLP;
   U32                 enbId;

   TRC2(SmHdlNghEurtaAddCellCfg);

   if(WR_ENODEB_TYPE_MACRO == eutraCell->enbType)
   {
      enbId = eutraCell->enbId;
      enbId >>= 8;
      eutraCell->enbId = enbId;
   }

   if(ROK == cmHashListFind(&(smCfgCb.neighEnbLstCp), (U8*)&eutraCell->enbId,
               sizeof(eutraCell->enbId), 0,(PTR*) &nghEnbCfgList))
   {
         nghEnbCfgList->numCell++;
   }
   else
   { 
      WR_ALLOC(&nghEnbCfgList, sizeof(SmNeigEnbList));
      if(NULLP == nghEnbCfgList)
      {
        RLOG0(L_FATAL, "Memory allocation failed while creating memory for "
           "neighbour eNodeB");
        RETVALUE(RFAILED);
      }
      nghEnbCfgList->numCell++;
      /* nghEnbCfgList->nghEnbCfg.peerId  = smGetPeerId(eutraCell->enbId); */
      WR_GET_PEERID_FROM_ENBID(eutraCell->enbId,nghEnbCfgList->nghEnbCfg.peerId); 
      nghEnbCfgList->nghEnbCfg.enbType = eutraCell->enbType;
      /*DO NOT do right shift for macro as its already done while filling*/ 
      /*DO NOT do right shift for macro as its already done while filling*/
      nghEnbCfgList->nghEnbCfg.enbId   = eutraCell->enbId;
      RLOG2(L_DEBUG,"enb type : %lu, enbId : %lu",
         nghEnbCfgList->nghEnbCfg.enbType,nghEnbCfgList->nghEnbCfg.enbId);

      nghEnbCfgList->nghEnbCfg.plmn    = eutraCell->plmnId[0];
      nghEnbCfgList->nghEnbCfg.ipAddr.u.ipv4TptAddr.port       = 0; 
      nghEnbCfgList->nghEnbCfg.ipAddr.u.ipv4TptAddr.address    = 0; 
      nghEnbCfgList->nghEnbCfg.secIpAddr.u.ipv4TptAddr.port    = 0;
      nghEnbCfgList->nghEnbCfg.secIpAddr.u.ipv4TptAddr.address = 0;
      nghEnbCfgList->nghEnbCfg.state     = wrSmDfltNeEnbState; 
      nghEnbCfgList->nghEnbCfg.isX2Reqd  = smGetIsX2Enabl(eutraCell->enbType);

      if(cmHashListInsert(&(smCfgCb.neighEnbLstCp),
               (PTR)nghEnbCfgList, (U8 *)&nghEnbCfgList->nghEnbCfg.enbId,
                      sizeof(nghEnbCfgList->nghEnbCfg.enbId)) != ROK)
      {
         RLOG0(L_ERROR, "Insertion of Neighbour eNodeB in hash list failed");
         WR_FREE(nghEnbCfgList, sizeof(SmNeigEnbList));
         RETVALUE(RFAILED);
      }
    }
    WR_ALLOC(&nghEutraCellList, sizeof(SmNeigEutraCellList));
    if(NULLP == nghEutraCellList)
    {
      RLOG0(L_FATAL, "Memory allocation failed for while creating neighbour "
         "EUTRA cell");
      RETVALUE(RFAILED);
    }
    nghEutraCellList->nghEutraCellCfg= *eutraCell;

    smFillNghHashKeyInit(&nghEutraCellList->hashKey, 
                 &nghEutraCellList->nghEutraCellCfg.enbPlmn,
                  nghEutraCellList->nghEutraCellCfg.enbId);
    nghEutraCellList->dynCfgInd = WR_CFG_DONE;
    if(cmHashListInsert(&(smCfgCb.neighEutraCellLstCp),
                        (PTR)nghEutraCellList, (U8 *)&nghEutraCellList->hashKey,
                        sizeof(smNghCellHasKey)) != ROK)
    {
       RLOG1(L_ERROR, "Insertion of EUTRA cell %ld into hash list failed since "
          "because of duplicate entry", 
          nghEutraCellList->nghEutraCellCfg.enbId);
       WR_FREE(nghEutraCellList, sizeof(SmNeigEutraCellList));
       RETVALUE(RFAILED);
    }

   RETVALUE(ROK);
}/*end of SmHdlNghEurtaAddCellCfg*/
/*
 *
 *       Fun:    SmHdlNghUrtaAddCellCfg - Neighbor Config Request
 *
 *       Desc:   Process the Neighbor Config Request
 *               new cell is detected through ANR and same cell info is updateing in MIB
 *
 *       Ret:    ROK     - succeeded
 *               RFAILED - failed
 *
 *       Notes:  None
 *
 *       File:   wr_sm_wr_ptmi.c
 *
 */
#ifdef ANSI
PUBLIC S16 SmHdlNghUrtaAddCellCfg 
(
LwrUtraNeighCell    *utranCell
)
#else
PUBLIC S16 SmHdlNghUrtaAddCellCfg(utranCell)
LwrUtraNeighCell    *utranCell;
#endif /* ANSI */
{
   SmNeigUtraCellList *nghUtraCellList = NULLP;

   TRC2(SmHdlNghUrtaAddCellCfg);

   WR_ALLOC(&nghUtraCellList, sizeof(SmNeigUtraCellList));
   if(NULLP == nghUtraCellList)
   {
      RLOG0(L_FATAL, "Memory allocation failed while creating UTRA cell");
      RETVALUE(RFAILED);
   }
   nghUtraCellList->nghUtraCellCfg = *utranCell;

   smFillNghHashKeyInit(&nghUtraCellList->hashKey, 
         &nghUtraCellList->nghUtraCellCfg.plmnId[0],
         nghUtraCellList->nghUtraCellCfg.cId);
   if(cmHashListInsert(&(smCfgCb.neighUtraCellLstCp),(PTR)nghUtraCellList,
            (U8 *)&nghUtraCellList->hashKey, sizeof(nghUtraCellList->hashKey)) != ROK)
   {
      RLOG1(L_ERROR, "Insertion of UTRA cell %ld into hash list failed "
         "because of duplicate entry",
         utranCell->cId);
      WR_FREE(nghUtraCellList, sizeof(SmNeigUtraCellList));
      RETVALUE(RFAILED);
   }
   smHdlNeighUtraCellCfgAdd(&nghUtraCellList->nghUtraCellCfg);
   nghUtraCellList->dynCfgInd = WR_CFG_DONE;
   RETVALUE(ROK);
}/*end of SmHdlNghUrtaAddCellCfg*/

/*
 *
 *       Fun:    SmHdlNghUrtaCellUpDateMib - Neighbor Config Request
 *
 *       Desc:   Process the Neighbor Config Request
 *               new cell is detected through ANR and same cell info is updateing in MIB
 *
 *       Ret:    ROK     - succeeded
 *               RFAILED - failed
 *
 *       Notes:  None
 *
 *       File:   wr_sm_wr_ptmi.c
 *
 */
#ifdef ANSI
PUBLIC S16 SmHdlNghUrtaCellUpDateMib 
(
LwrUtraNeighCell    *lwrUtranCell
)
#else
PUBLIC S16 SmHdlNghUrtaCellUpDateMib(lwrUtranCell)
LwrUtraNeighCell    *lwrUtranCell;
#endif /* ANSI */
{
   MsmUtraNeighCell  utranCell;
   U16               plmnIdx = 0;

   TRC2(SmHdlNghUrtaCellUpDateMib);

   utranCell.nrIndex = lwrUtranCell->nrIndex;
   utranCell.rncId   = lwrUtranCell->rncId;
   utranCell.cId     = lwrUtranCell->cId;
   utranCell.lac     = lwrUtranCell->lac;
   utranCell.rac     = lwrUtranCell->rac;
   /*   utranCell->ura     = lwrUtranCell.ura;*/
   utranCell.numPlmn = lwrUtranCell->numPlmn;
   for(plmnIdx = 0; plmnIdx < lwrUtranCell->numPlmn; plmnIdx++)
   {
      utranCell.plmnId[plmnIdx].mcc[0] = lwrUtranCell->plmnId[plmnIdx].mcc[0];
      utranCell.plmnId[plmnIdx].mcc[1] = lwrUtranCell->plmnId[plmnIdx].mcc[1];
      utranCell.plmnId[plmnIdx].mcc[2] = lwrUtranCell->plmnId[plmnIdx].mcc[2];

      utranCell.plmnId[plmnIdx].numMncDigits = 
         lwrUtranCell->plmnId[plmnIdx].numMncDigits;

      utranCell.plmnId[plmnIdx].mnc[0] = lwrUtranCell->plmnId[plmnIdx].mnc[0];
      utranCell.plmnId[plmnIdx].mnc[1] = lwrUtranCell->plmnId[plmnIdx].mnc[1];

      if(3 == utranCell.plmnId[plmnIdx].numMncDigits)
         utranCell.plmnId[plmnIdx].mnc[2] = lwrUtranCell->plmnId[plmnIdx].mnc[2];
   } 
   utranCell.psc       = lwrUtranCell->t.utraFdd.psc;
   /*   utranCell.pcpichTxPower = lwrUtranCell.;*/
   utranCell.arfcnUl   = lwrUtranCell->t.utraFdd.arfcnUl;
   utranCell.arfcnDl   = lwrUtranCell->t.utraFdd.arfcnDl;
   SmUpdateMibParms(MSM_LTE_UTRA_NEIGHBORLIST_IN_USE_PARAMETERS, MSM_ACT_ADD, &utranCell);
   RETVALUE(ROK);
}/*end of SmHdlNghUrtaCellUpDateMib*/

/*
 *
 *       Fun:    SmHdlNghEurtaCellUpDateMib - Neighbor Config Request
 *
 *       Desc:   Process the Neighbor Config Request
 *               new cell is detected through ANR and same cell info is updateing in MIB
 *
 *       Ret:    ROK     - succeeded
 *               RFAILED - failed
 *
 *       Notes:  None
 *
 *       File:   wr_sm_wr_ptmi.c
 *
 */
#ifdef ANSI
PUBLIC S16 SmHdlNghEurtaCellUpDateMib 
(
LwrEUTRANeighCell *lwrEutranCell
)
#else
PUBLIC S16 SmHdlNghEurtaCellUpDateMib(lwrEutranCell)
LwrEUTRANeighCell *lwrEutranCell;
#endif /* ANSI */
{
   MsmEUTRANeighCell           eutraCell;

   TRC2(SmHdlNghEurtaCellUpDateMib);

   eutraCell.nrIndex = lwrEutranCell->nrIndex;
   eutraCell.cid     = lwrEutranCell->enbId;
   eutraCell.plmnId[0].numMncDigits = lwrEutranCell->plmnId[0].numMncDigits;
   eutraCell.plmnId[0].mcc[0] = lwrEutranCell->plmnId[0].mcc[0];
   eutraCell.plmnId[0].mcc[1] = lwrEutranCell->plmnId[0].mcc[1];
   eutraCell.plmnId[0].mcc[2] = lwrEutranCell->plmnId[0].mcc[2];
   eutraCell.plmnId[0].mnc[0] = lwrEutranCell->plmnId[0].mnc[0];
   eutraCell.plmnId[0].mnc[1] = lwrEutranCell->plmnId[0].mnc[1];
   eutraCell.plmnId[0].mnc[2] = lwrEutranCell->plmnId[0].mnc[2];
   eutraCell.pci      = lwrEutranCell->pci;
   eutraCell.earfcnDl = lwrEutranCell->earfcnDl;
   eutraCell.earfcnUl = lwrEutranCell->earfcnUl;
   eutraCell.cio      = lwrEutranCell->cio;
   eutraCell.qoffset  = lwrEutranCell->qoffset;
   /* 
      eutraCell.RSTxPower   =;
      eutraCell.BlackListed =;
      */
   SmUpdateMibParms(MSM_LTE_EUTRAN_NEIGHBORLIST_IN_USE_PARAMETERS, MSM_ACT_ADD, &eutraCell);
   RETVALUE(ROK);
}/*end of SmHdlNghEurtaCellUpDateMib*/


/*
*
*       Fun:   wrSmmSetBwFrmBwIdx
*
*       Desc:  Function which changes the bandwidth prameter depending on the 
*              bandwidth Idx Configured configured. This Itself calls the wrSmmSetBandwidth.
*              This function used in case of OAM.
*
*       Ret:   Void
*
*       Notes: None
*
*       File:  wr_smm_enbapp_cmnplatutils.c
*
*/

PRIVATE Void wrSmmSetBwFrmBwIdx
(
U32    bwIdx
)
{
   switch ( bwIdx )
   {
      case MSM_LWR_BANDWIDTH_25 :
      {
         wrSmmSetBandwidth(WR_FREQ_BANDWIDTH_5MHZ);
      }
      break;

      case MSM_LWR_BANDWIDTH_50 :
      {
         wrSmmSetBandwidth(WR_FREQ_BANDWIDTH_10MHZ);
      }
      break;
 
      case MSM_LWR_BANDWIDTH_75 :
      {
         wrSmmSetBandwidth(WR_FREQ_BANDWIDTH_15MHZ);
      }
      break;

      case MSM_LWR_BANDWIDTH_100:
      {
         wrSmmSetBandwidth(WR_FREQ_BANDWIDTH_20MHZ);
      }
      break;
      
      default: /* By default 20 MHZ is configured */
      {
         WR_DBGP(WR_ERROR,(WR_PRNT_BUF,
          "Configured Bandwidth value [%ld] is not supported. Only Bandwidth 5, 10, 15 & 20Mhz are supported.Setting to 20MHz by default\n",
                          bwIdx));
         wrSmmSetBandwidth(WR_FREQ_BANDWIDTH_20MHZ);
      }
      break;
   }
   RETVOID;
}

/********************************************************************30**

           End of file:    $SID$

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
$SID$        ---      Sriky         1. initial release TotaleNodeB 1.1
*********************************************************************91*/
