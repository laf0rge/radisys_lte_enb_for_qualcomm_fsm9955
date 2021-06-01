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
               

     File:     wr_smm_enbapp_utils.c

     Sid:      $SID$ 

     Prg:      Sriky 

*********************************************************************21*/
/* header include files (.h) */
static const char* RLOG_MODULE_NAME="SMM";

static int RLOG_FILE_ID=54;

static int RLOG_MODULE_ID=2;

#include "wr.h"
#include "lrm.h"
#include "wr_emm.h"
#include "wr_lwr.h"
#include "wr_cmn.h"
#include "wr_smm_smallcell.h"
#include "wr_smm_init.h"
#include "wr_utils.h" 
#include "wr_umm.h" 
#include "lrm.x"
#include "wr_umm.h"
#include <math.h>

PRIVATE S16 smUtlGetNumInterFreqNghCellCfg (Void);
EXTERN Void smBldMeasCellCfg ( LwrMngmt   *lwrMng,  U8         cfgGrpIndexa);
PUBLIC S16 smBuildWrNr1xCdmaFreqCfg
(
 LwrMngmt *lwrMng
);
PUBLIC S16 smBuildWrNr1xCdmaCellCfg
(
 LwrMngmt *lwrMng
);
EXTERN S16 wrFillSectorId
(
TknStrOSXL                  *ptr,
U8                           len,
Data                         *val
);
EXTERN S16 smValidateIpAddr(Void);

/**
 *
 *       Fun:  smFillRmuAcbInfo 
 *
 *       Desc:  This function is used for filling default ACB info to RRM
 *
 *       Ret:   ROK
 *
 *       Notes: None
 *              
 */
#ifdef ANSI 
PRIVATE void smFillRmuAcbInfo(LrmCellConfiguration *cellCfg)
#else
PRIVATE void smFillRmuAcbInfo(cellCfg)
LrmCellConfiguration *cellCfg;
#endif
{
   U8 acbMask = 0x00;

   /*Filling ACB params*/
   cellCfg->stCellCfgInfo.stAcbInfo.bIsacBarringMoSig = 
                wrSmDfltMoSigAcBarringEnable;
   cellCfg->stCellCfgInfo.stAcbInfo.bIsacBarringMoData = 
                wrSmDfltMoDataAcBarringEnable;

   cellCfg->stCellCfgInfo.stAcbInfo.stAcBarringMoSig.enAcbTime = 
               wrSmDfltacBarringTime;

   cellCfg->stCellCfgInfo.stAcbInfo.stAcBarringMoData.enAcbTime = 
               wrSmDfltacBarringTime;

   if(wrSmDfltacBarringAC11)
   {
      acbMask |= LRM_ACB_SPECIAL_AC_11; 
   }
   if(wrSmDfltacBarringAC12)
   {
      acbMask |= LRM_ACB_SPECIAL_AC_12;
   }
   if(wrSmDfltacBarringAC13)
   {
      acbMask |= LRM_ACB_SPECIAL_AC_13;
   }
   if(wrSmDfltacBarringAC14)
   {
      acbMask |= LRM_ACB_SPECIAL_AC_14;
   }
   if(wrSmDfltacBarringAC15)
   {
      acbMask |= LRM_ACB_SPECIAL_AC_15;
   }

   cellCfg->stCellCfgInfo.stAcbInfo.stAcBarringMoData.bAcbForSpac = acbMask; 
   cellCfg->stCellCfgInfo.stAcbInfo.stAcBarringMoSig.bAcbForSpac = acbMask; 

   if(acbMask && (wrSmDfltacBarringFactor != 0))
   {
      cellCfg->stCellCfgInfo.stAcbInfo.stAcBarringMoData.enAcbFactor = 
         (CmRrmAcBarringFactor) RM_ACB_FACTOR_P00; 
      cellCfg->stCellCfgInfo.stAcbInfo.stAcBarringMoSig.enAcbFactor = 
         (CmRrmAcBarringFactor) RM_ACB_FACTOR_P00; 
   }
   else
   {
      cellCfg->stCellCfgInfo.stAcbInfo.stAcBarringMoData.enAcbFactor = 
         (CmRrmAcBarringFactor)wrSmDfltacBarringFactor; 
      cellCfg->stCellCfgInfo.stAcbInfo.stAcBarringMoSig.enAcbFactor = 
         (CmRrmAcBarringFactor)wrSmDfltacBarringFactor; 
   }
   cellCfg->stCellCfgInfo.stAcbInfo.bAcBarringEmergency = wrSmDfltACBarrNormal;

   RETVOID;
}

#ifdef RM_INTF
PUBLIC S16 smBuildRmuProtMmeCfg(U16 mmeId, LrmEnbConfiguration *enbCfg, U32 action);
EXTERN S16 smBuildRmuProtCellCfg(LrmCellConfiguration *cellCfg);
EXTERN S16 smBuildRmuProtEnbCfg(LrmEnbConfiguration *enbCfg);

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

   TRC2(smBuildRmuProtCellCfg);

    cellCfg->enCellAddModDel = LRM_PROTOCFG_ADD_CELL;
    cellCfg->stCellCfgInfo.bCellId         = 1;
    cellCfg->stCellCfgInfo.enCellDlBandWidth    = wrSmDfltCellDlBW;
    
    /*Filling ACB params*/
    smFillRmuAcbInfo(cellCfg);
    
    cellCfg->stCellCfgInfo.usMaxUlBroadbandBw   = 10; /*TODO*/
    cellCfg->stCellCfgInfo.usMaxDlBroadbandBw   = 10; /*TODO*/
    cellCfg->stCellCfgInfo.bNRbCqi              = wrSmDfltPUCCHnRBCQI; /* N2Rb */
    cellCfg->stCellCfgInfo.bNCsAn               = wrSmDfltPUCCHnCSAN; /*N1Cs */
    cellCfg->stCellCfgInfo.bMaxActiveUesServed  = smCfgCb.maxUeSupp;
    cellCfg->stCellCfgInfo.bMaxSigUesServed     = smCfgCb.maxUeSupp + WR_MAX_SR_CQI_NOT_CONFIGURED_UES;  /*TODO*/
    /* RRM_SP1_START */
    cellCfg->stCellCfgInfo.bMaxAvgGbrPrbUsage = smCfgCb.maxAvgGbrPrbUsage;
    cellCfg->stCellCfgInfo.bRbcQci1TmngReq = wrSmDfltQci1TmngReq;
    cellCfg->stCellCfgInfo.bRbcQci2TmngReq = wrSmDfltQci2TmngReq;
    cellCfg->stCellCfgInfo.bRbcQci3TmngReq = wrSmDfltQci3TmngReq;
    cellCfg->stCellCfgInfo.bRbcQci4TmngReq = wrSmDfltQci4TmngReq;
    cellCfg->stCellCfgInfo.bRbcQci4TmngReqEnable = wrSmDfltQci4TmngReqEnable;
    cellCfg->stCellCfgInfo.bDuplexMode = smCfgCb.duplexMode;
    cellCfg->stCellCfgInfo.bNumDlNewUesPerTti = wrSmDfltMaxUePerDlSf;
    cellCfg->stCellCfgInfo.bNumUlNewUesPerTti = wrSmDfltMaxUePerUlSf;
     /* reserving default number of DL re-Tx UEs per TTI to 1 */
    cellCfg->stCellCfgInfo.bNumDlReTxUesPerTti = wrSmDfltMinDlReTxUesPerTti;
    cellCfg->stCellCfgInfo.bNumUlReTxUesPerTti = wrSmDfltMinUlReTxUesPerTti;
    cellCfg->stCellCfgInfo.bArpForEmergencyBearer = wrSmDfltArpForEmerServ;
    /* RRM_SP1_END */ 
    cellCfg->stCellCfgInfo.flagDlfsScheduling   = wrSmDfltDLFSSchdTyp;
    cellCfg->stCellCfgInfo.bWaitTime            = 3;  /*TODO: Pick from OAM*/
    cellCfg->stCellCfgInfo.flagTransMode        = wrSmDfltTransModeUse;
    cellCfg->stCellCfgInfo.bRcmAlgoType         = 1;  /* TODO */
    cellCfg->stCellCfgInfo.bNumTxAnt            = wrSmDfltNumOfTxAnt; 
    cellCfg->stCellCfgInfo.enSrPrdcty           = wrSmDfltSrPrdcty; /* TODO 2*/
	 cellCfg->stCellCfgInfo.enCqiPrdcty          = wrSmDfltCqiPrdcty; /* TODO 5*/
	 cellCfg->stCellCfgInfo.usNumSrResPerTti     = wrSmDfltNumSrResPerTti; /* TODO 4*/
	 cellCfg->stCellCfgInfo.usNumCqiResPerTti    = wrSmDfltNumCqiResPerTti; /* TODO 4*/
	 cellCfg->stCellCfgInfo.enUlDlConfig         = wrSmTddSubframeConfig; /* TODO 1*/
	 cellCfg->stCellCfgInfo.enSplSfCfg           = wrSmTddSpecialSubframeConfig; /* TODO 7*/
	 cellCfg->stCellCfgInfo.usN1Pucch            = wrSmDfltN1PucchAn; /* TODO 7*/
	 cellCfg->stCellCfgInfo.enDsrTransMax        = wrSmDfltDsrTransMax; /* TODO 7*/
    /*RBC_SP1_X*/
	 cellCfg->stCellCfgInfo.utNumOfPrbRpts         = wrSmDfltNoOfPrbRpts; 
	 cellCfg->stCellCfgInfo.utPrbRptInterval       = wrSmDfltPrbRptInterval; 
    /*RBC_SP1_Y*/
    /*DRX QCI*/
    cellCfg->stCellCfgInfo.stRmCellDrxCfg.bIsGbrDrxDis = wrSmDfltisDrxQCIEnabled; 
    /*Removed DRX Flag */
    /*Cases WITHOUT RSYS_OAM*/
    cellCfg->stCellCfgInfo.enSrPrdcty           = wrSmDfltSrPrdcty; /* TODO 2*/
    cellCfg->stCellCfgInfo.enCqiPrdcty          = wrSmDfltCqiPrdcty; /* TODO 5*/
    cellCfg->stCellCfgInfo.usNumSrResPerTti     = wrSmDfltNumSrResPerTti; /* TODO 4*/
    cellCfg->stCellCfgInfo.usNumCqiResPerTti    = wrSmDfltNumCqiResPerTti; /* TODO 4*/
    cellCfg->stCellCfgInfo.bSimulCqiAckNack     = wrSmDfltSimultAckNackAndCQI;
#ifdef LTE_TDD
    cellCfg->stCellCfgInfo.enUlDlConfig         = wrSmTddSubframeConfig; /* TODO 1*/
    cellCfg->stCellCfgInfo.enSplSfCfg           = wrSmTddSpecialSubframeConfig; /* TODO 7*/
#endif 
	 cellCfg->stCellCfgInfo.usN1Pucch            = wrSmDfltN1PucchAn; /* TODO 7*/
	 cellCfg->stCellCfgInfo.enDsrTransMax        = wrSmDfltDsrTransMax; /* TODO 7*/


    cellCfg->stCellCfgInfo.stRmCellDrxCfg.bIsDrxEnabled        = wrSmDfltIsDrxEnabled; 
    cellCfg->stCellCfgInfo.stRmCellDrxCfg.usDrxInactvTmr       = wrSmDfltDrxInactvTmr; 
    cellCfg->stCellCfgInfo.stRmCellDrxCfg.bDrxRetxTmr          = wrSmDfltDrxReTxTmr; 
    cellCfg->stCellCfgInfo.stRmCellDrxCfg.usLongDrxCycleGbr    = wrSmDfltDrxLongCycleGbr; 
    cellCfg->stCellCfgInfo.stRmCellDrxCfg.usLongDrxCycleNonGbr = wrSmDfltDrxLongCycleNonGbr; 
    /*Cases WITHOUT RSYS_OAM*/

   /* SPS changes starts */
   /* By default the SPS will be disabled in the wrSmDfltIsSpsEnabled */
   cellCfg->stCellCfgInfo.stSpsCellCfg.bIsSpsEnabled = wrSmDfltIsSpsEnabled;
   if (cellCfg->stCellCfgInfo.stSpsCellCfg.bIsSpsEnabled == TRUE)
   {
      cellCfg->stCellCfgInfo.stSpsCellCfg.usMaxDlSpsUePerTti = wrSmDfltMaxDlSpsUePerTti;
      cellCfg->stCellCfgInfo.stSpsCellCfg.usMaxUlSpsUePerTti = wrSmDfltMaxUlSpsUePerTti;
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

   if( wrSmDfltNlPUCCHAn < 
         (cellCfg->stCellCfgInfo.usNumSrResPerTti + usTotalSpsN1PucchRes))
   {
      RLOG3(L_ERROR,"SM : Not Enough N1 PUCCH resources are available,"
            " so Changing it"
            "usN1Pucch(%d), bNumSrPerTti(%d), usTotalSpsN1PucchRes(%d)\n",
            wrSmDfltNlPUCCHAn,
            cellCfg->stCellCfgInfo.usNumSrResPerTti, usTotalSpsN1PucchRes);
      wrSmDfltNlPUCCHAn = 
         cellCfg->stCellCfgInfo.usNumSrResPerTti + usTotalSpsN1PucchRes;
   }
	cellCfg->stCellCfgInfo.usN1Pucch  = wrSmDfltNlPUCCHAn;

   /* HCSG_DEV Start */
   if(wrSmDfltAccessMode == WR_ENB_HYBRID_ACCESS)
   {
      cellCfg->stCellCfgInfo.usMaxNonCsgUes   = wrSmDfltMaxNonCsgUEs;
      cellCfg->stCellCfgInfo.usMaxCsgUes      = wrSmDfltMaxCsgUEs;
      cellCfg->stCellCfgInfo.usMaxExtraUes    = 
         ceil(cellCfg->stCellCfgInfo.bMaxActiveUesServed * 0.25);
      cellCfg->stCellCfgInfo.usAccessMode     = wrSmDfltAccessMode;
   }
   else
   {
      cellCfg->stCellCfgInfo.usMaxNonCsgUes = 0;
      cellCfg->stCellCfgInfo.usMaxCsgUes    = wrSmDfltMaxCsgUEs;
      cellCfg->stCellCfgInfo.usMaxExtraUes  =  0;
   }
    /* HCSG_DEV End */

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
    cellCfg->stCellCfgInfo.bCellId         = 1;
#if 1
    cellCfg->stCellCfgInfo.enCellDlBandWidth    = wrSmDfltCellDlBW;

    /*Filling ACB params*/
    smFillRmuAcbInfo(cellCfg);

    cellCfg->stCellCfgInfo.usMaxUlBroadbandBw   = 10; /*TODO*/
    cellCfg->stCellCfgInfo.usMaxDlBroadbandBw   = 10; /*TODO*/
    cellCfg->stCellCfgInfo.bNRbCqi              = wrSmDfltPUCCHnRBCQI; /* N2Rb */
    cellCfg->stCellCfgInfo.bNCsAn               = wrSmDfltPUCCHnCSAN; /*N1Cs */
    cellCfg->stCellCfgInfo.bMaxActiveUesServed  = smCfgCb.maxUeSupp;
    cellCfg->stCellCfgInfo.bMaxSigUesServed     = smCfgCb.maxUeSupp + WR_MAX_SR_CQI_NOT_CONFIGURED_UES;  /*TODO*/
    /* RRM_SP1_START */
    cellCfg->stCellCfgInfo.bMaxAvgGbrPrbUsage = smCfgCb.maxAvgGbrPrbUsage;
    cellCfg->stCellCfgInfo.bRbcQci1TmngReq = wrSmDfltQci1TmngReq;
    cellCfg->stCellCfgInfo.bRbcQci2TmngReq = wrSmDfltQci2TmngReq;
    cellCfg->stCellCfgInfo.bRbcQci3TmngReq = wrSmDfltQci3TmngReq;
    cellCfg->stCellCfgInfo.bRbcQci4TmngReq = wrSmDfltQci4TmngReq;
    cellCfg->stCellCfgInfo.bRbcQci4TmngReqEnable = wrSmDfltQci4TmngReqEnable;
    cellCfg->stCellCfgInfo.bDuplexMode = smCfgCb.duplexMode;
    cellCfg->stCellCfgInfo.bNumDlNewUesPerTti = wrSmDfltMaxUePerDlSf;
    cellCfg->stCellCfgInfo.bNumUlNewUesPerTti = wrSmDfltMaxUePerUlSf;
     /* reserving default number of DL re-Tx UEs per TTI to 1 */
    cellCfg->stCellCfgInfo.bNumDlReTxUesPerTti = wrSmDfltMinDlReTxUesPerTti;
    cellCfg->stCellCfgInfo.bNumUlReTxUesPerTti = wrSmDfltMinUlReTxUesPerTti;
    cellCfg->stCellCfgInfo.bArpForEmergencyBearer = wrSmDfltArpForEmerServ;

    /* RRM_SP1_END */ 
    cellCfg->stCellCfgInfo.flagDlfsScheduling   = wrSmDfltDLFSSchdTyp;
    cellCfg->stCellCfgInfo.bWaitTime            = 3;  /*TODO: Pick from OAM*/
    cellCfg->stCellCfgInfo.flagTransMode        = wrSmDfltTransModeUse;
    cellCfg->stCellCfgInfo.bRcmAlgoType = 1;  /* TODO */
    /* Fix for CR defeat ccpu00136285 */
    /* Align the Num of antenna enum to RRM cell configuration
     */
    cellCfg->stCellCfgInfo.bNumTxAnt = wrSmDfltNumOfTxAnt;
    /*Cases WITHOUT RSYS_OAM*/
    cellCfg->stCellCfgInfo.stRmCellDrxCfg.bIsDrxEnabled        = wrSmDfltIsDrxEnabled; 
    cellCfg->stCellCfgInfo.stRmCellDrxCfg.usDrxInactvTmr       = wrSmDfltDrxInactvTmr; 
    cellCfg->stCellCfgInfo.stRmCellDrxCfg.bDrxRetxTmr          = wrSmDfltDrxReTxTmr; 
    cellCfg->stCellCfgInfo.stRmCellDrxCfg.usLongDrxCycleGbr    = wrSmDfltDrxLongCycleGbr; 
    cellCfg->stCellCfgInfo.stRmCellDrxCfg.usLongDrxCycleNonGbr = wrSmDfltDrxLongCycleNonGbr; 
    /*Cases WITHOUT RSYS_OAM*/
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
            RLOG1(L_ERROR, "PCI Range value not found for PCI entry[%d]",
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
      RLOG2(L_ERROR, "For startPhyCellId [%d] and PciRange [%d] the PCI is "
         "exceeding 504 So Setting pciRange to default values ",
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
U32                   action;
#endif
{
   TRC2(smBuildRmuProtMmeCfg);

   enbCfg->bCfgType = RRM_ENB_CFG_MME_CFG;
   enbCfg->u.stMmeCfg.enMmeAddModOrDel = action;
   enbCfg->u.stMmeCfg.usMmeId        = mmeId;

   RETVALUE(ROK);
}

#endif /*RM_INTF*/
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
*       File:  wr_smm_enbapp_utils.c  
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
   LwrGenCfg    *cfg = NULLP;
   U8           loopCnt = 0;
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
   cfg->cellCfg.rrmRntiStart = smCfgCb.rrmRntiStart;
   cfg->cellCfg.maxRrmRntis = smCfgCb.maxRrmRntis;
   cfg->cellCfg.rrmPrmblStart = smCfgCb.rrmDedPrmStart;
   cfg->cellCfg.maxRrmPrmbls = smCfgCb.rrmNumDedPrm;
   cfg->cellCfg.macRntiStart = smCfgCb.macRntiStart;
   cfg->cellCfg.maxMacRntis = smCfgCb.maxMacRntis;
   cfg->cellCfg.numOfMacPrm = smCfgCb.numOfMacPrm;
   cfg->cellCfg.rsrpTrshldRange = smCfgCb.rsrpTrshldRange;
#endif
   /* Filling PLMN configuration */
   cfg->cellCfg.noOfBrdCstPlmn = smCfgCb.noOfBrdCstPlmn;
   for (loopCnt = 0; loopCnt < smCfgCb.noOfBrdCstPlmn; loopCnt++)
      {
         strcpy((S8 *)cfg->cellCfg.plmnId[loopCnt], (S8 *)smCfgCb.plmnId[loopCnt]);
      }
   /* Filling the ENB Type */
   cfg->eNBType = smGetEnbType(smCfgCb.pci);
   printf("\n smBuildWrGenCfg: cfg->enblSIAndPagngLog %d  smCb.rrcLogEnable %d\n", cfg->enblSIAndPagngLog, smCb.rrcLogEnable);
   cfg->enblSIAndPagngLog = smCb.rrcLogEnable;
   cfg->noOfNghInfo = smCfgCb.noOfNghInfo;
   /*cfg->nghInfo = smCfgCb.nghInfo;*/
   cfg->noOfguGrp = smCfgCb.noOfguGrp;
   /*cfg->guGrpId = smCfgCb.guGrpId;*/
   
   /* Filling UE HO Attach Timer */
   cfg->ueHoAttachTimerVal = smCfgCb.ueHoAttachTimerVal;

   /* Filling UE Inactivity Timer */
   cfg->inactvTmrVal = smCfgCb.inactvTmrVal;
   cfg->expiryCnt = smCfgCb.maxExpires;
   cfg->endMrkTmrVal = smCfgCb.endMrkTmrVal;
   /* Fill UL SRS Periodicity */ 
   cfg->ulSrsPeriod  = wrSmUlSrsPeriod;

   /* Filling Physical Cell Id */
   cfg->cellCfg.physCellId = smCfgCb.pci;

#if 0 /* enb ip address is configured already in protoCfg of WR(enbapp). Hence removing it */
   wrCpyCmTptAddr(&cfg->dataAppAddr, &smCfgCb.enbIpAddr);                
   if(cfg->dataAppAddr.type == CM_TPTADDR_IPV4)
   {
      cfg->dataAppAddr.u.ipv4TptAddr.port = WR_DFLT_EGTP_PORT;
   }
   else if(cfg->dataAppAddr.type == CM_TPTADDR_IPV6)
   {
      cfg->dataAppAddr.u.ipv6TptAddr.port = WR_DFLT_EGTP_PORT;
   }
#endif
#endif /* end of WR_RELAY */
   
   /* Filling UE Inactivity Timer */
   cfg->inactvTmrVal = smCfgCb.inactvTmrVal;
   cfg->endMrkTmrVal = smCfgCb.endMrkTmrVal;

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
   U8                loopCnt, ratIdx =0; 
   TRC2(smBuildWrEnbCfg);

   lwrMng->t.protoCfg.type = WR_CFG_TYPE_ENB;
   lwrMng->t.protoCfg.t.eNbCfg.action = WR_ACT_ADD;
   
   enbCfg =&(lwrMng->t.protoCfg.t.eNbCfg.t.eNodeBCfg);

   /* add enb name */
   enbName = &(enbCfg->protoCfg.enbName);
   enbName->len = (U16)(strlen((S8 *)wrSmDfltHenbName));
   WR_ALLOC(&(enbName->val), enbName->len);
   if(enbName->val == NULLP)
   {
      RETVALUE(RFAILED);
   }
   cmMemcpy(enbName->val, (U8*)wrSmDfltHenbName, enbName->len);

   enbCfg->maxNumOfUe = wrSmDfltMaxNoUE;
   enbCfg->numTxAntenna = wrSmDfltNumOfTxAnt; 
   enbCfg->enbCfgTransTmrVal = wrSmDfltEnbCfgTransTmrVal;
   enbCfg->pwsEtwsCmas = wrSmPwsEtwsCmas; /* Enable ETWS or CMAS or disable both*/
   /*Fix for ccpu00144101 */
   enbCfg->protoCfg.enbType = smGetEnbType(smCfgCb.pci);/*wrSmDfltEnbType*/;
   /* ccpu00138340 start */
   if(WR_ENODEB_TYPE_HOME == enbCfg->protoCfg.enbType) 
   {
       enbCfg->protoCfg.enbId = wrSmDfltCellId; 
   /* in home enb case eNodeB Id and cellId are same wrSmDfltEnbID; */
   }
   else
   {
         enbCfg->protoCfg.enbId = (wrSmDfltCellId>>8); 
   }/* in macro case eNodeB Id and cellId are same wrSmDfltEnbID; */
   /* ccpu00138340 end */
   wrCpyCmTptAddr(&enbCfg->protoCfg.enbIpAddr[WR_IPV4_IDX], &smCfgCb.enbIpAddr[WR_IPV4_IDX]);
   wrCpyCmTptAddr(&enbCfg->protoCfg.enbIpAddr[WR_IPV6_IDX], &smCfgCb.enbIpAddr[WR_IPV6_IDX]);
   if(enbCfg->protoCfg.enbIpAddr[WR_IPV4_IDX].type == CM_TPTADDR_IPV4)
   {
      enbCfg->protoCfg.enbIpAddr[WR_IPV4_IDX].u.ipv4TptAddr.port =  wrSmDfltEnbPort;
   }
   else if(enbCfg->protoCfg.enbIpAddr[WR_IPV6_IDX].type == CM_TPTADDR_IPV6)
   {       
      enbCfg->protoCfg.enbIpAddr[WR_IPV6_IDX].u.ipv6TptAddr.port =  wrSmDfltEnbPort;
   }

   enbCfg->protoCfg.numPlmns                 = wrSmDfltNumPlmn;
   for(loopCnt=0; loopCnt<enbCfg->protoCfg.numPlmns; loopCnt++)
   {
      enbCfg->protoCfg.plmns[loopCnt].plmnId.numMncDigits = wrSmDfltNumOfMncDigits[loopCnt];

      enbCfg->protoCfg.plmns[loopCnt].plmnId.mcc[0] = wrSmDfltPlmnMcc[loopCnt][0];
      enbCfg->protoCfg.plmns[loopCnt].plmnId.mcc[1] = wrSmDfltPlmnMcc[loopCnt][1];
      enbCfg->protoCfg.plmns[loopCnt].plmnId.mcc[2] = wrSmDfltPlmnMcc[loopCnt][2];

      enbCfg->protoCfg.plmns[loopCnt].plmnId.mnc[0] = wrSmDfltPlmnMnc[loopCnt][0];
      enbCfg->protoCfg.plmns[loopCnt].plmnId.mnc[1] = wrSmDfltPlmnMnc[loopCnt][1];
      enbCfg->protoCfg.plmns[loopCnt].plmnId.mnc[2] = wrSmDfltPlmnMnc[loopCnt][2];

		enbCfg->protoCfg.plmns[loopCnt].plmnRatPriority.numberOfRats = wrSmDfltPlmnRatPriorList.numberOfRats;
   	for(ratIdx = 0; ratIdx < enbCfg->protoCfg.plmns[loopCnt].plmnRatPriority.numberOfRats; ratIdx++)
      {
         enbCfg->protoCfg.plmns[loopCnt].plmnRatPriority.rat[ratIdx] = wrSmDfltPlmnRatPriorList.rat[ratIdx];
      }
      enbCfg->protoCfg.plmns[loopCnt].isRedirSupport = wrSmDfltPlmnRedirSuppLst[loopCnt];
   }

#ifdef MME_LDBAL
   enbCfg->numOfMmePriGrps                 = wrSmDfltNumOfPriorityGrps;
#endif /* end of MME_LDBAL */
    
    enbCfg->macMainCfg.timeAlignmentTimerDedicated = wrSmDfltTaTmrDed; 
    enbCfg->macMainCfg.ulSchCfg.maxHARQ_Tx = wrSmDfltMaxUlHqTx; 
    enbCfg->macMainCfg.ulSchCfg.periodicBSR_Timer =
       wrSmDfltMacCfgPeriodicBSRTimer;
    enbCfg->macMainCfg.ulSchCfg.retxBSR_Timer = wrSmDfltMacCfgRetxBSRTimer;
    enbCfg->macMainCfg.ulSchCfg.ttiBundling = wrSmDfltMacCfgTtiBundling;
    enbCfg->macMainCfg.ulSchCfg.deltaHqOffset = wrSmDfltDeltaHqOffst;
    enbCfg->macMainCfg.phr_Config.periodicPHR_Timer = wrSmDfltMacCfgPeriodicPHR_Timer;
    enbCfg->macMainCfg.phr_Config.prohibitPHR_Timer = wrSmDfltMacCfgProhibitPHRTimer;
    enbCfg->macMainCfg.phr_Config.dl_PathlossChange = wrSmDfltMacCfgDlPathlossChange; 
   
    enbCfg->pdcpInfo.umSnSize = NHU_PDCP_SN_SIZE_7BITS; 
    enbCfg->pdcpInfo.statRprtReqrd = wrSmDfltStatusReportReqd;
    enbCfg->pdcpInfo.discardTimer = wrSmDfltDiscardTimer;

    enbCfg->schReqCfg.srPucchRsrcIdx = wrSmDfltSrPUCCHResIdx; 
    enbCfg->schReqCfg.dsrTransMax = wrSmDfltDsrTransMax;

    enbCfg->puschDedicated.ackIdx = wrSmDfltBetaOffACKIdx;
    enbCfg->puschDedicated.riIdx = wrSmDfltBetaOffRIIdx;
    enbCfg->puschDedicated.cqiIdx = wrSmDfltBetaOffCQIIdx;
    enbCfg->puschDedicated.puschPwrCfg.pres = wrSmDfltisUlGrpPwrCntrlPuschEnable;
    enbCfg->puschDedicated.puschPwrCfg.isFmt3a = wrSmDfltPuschPwrFmt3AEnable;

    enbCfg->pucchPwrCfg.pres = wrSmDfltisUlGrpPwrCntrlPucchEnable;
    enbCfg->pucchPwrCfg.isFmt3a = wrSmDfltPucchPwrFmt3AEnable; 

    enbCfg->pdschCfgDedicated.pA = wrSmDfltPdschPA;

    enbCfg->qosCfg.ueBr = wrSmDfltUeBr;

    enbCfg->cqiRptCfg.aperCqiEnbld = FALSE;
    enbCfg->cqiRptCfg.cqiAperiodicMode = wrSmDfltCqiRprtModeAperiodic;
    enbCfg->cqiRptCfg.nomPdschRsEpreOff = (U8)wrSmDfltCqiNomPDSCHRSEPREoff;
    enbCfg->cqiRptCfg.periodRptSetupPres = FALSE;
    enbCfg->cqiRptCfg.cqiPucchRsrcIdx = wrSmDfltCqiPUCCHResIdx;
    enbCfg->cqiRptCfg.cqipmiCfgIdx = WR_UMM_CQI_PMI_CONFIG_IDX;
    enbCfg->cqiRptCfg.subbndCqik = wrSmDfltUePrdDlCqiK;
    enbCfg->cqiRptCfg.fmtInd = WR_UE_CQI_FMT_WB;/*129311*/
    enbCfg->cqiRptCfg.riCfgIdx = 0;
    enbCfg->cqiRptCfg.simulAckNackAndCQI = wrSmDfltSimultAckNackAndCQI;
    enbCfg->cqiRptCfg.periodicmode = wrSmDfltPrdModeEnum;
    /*ccpu00129641*/
    enbCfg->cqiRptCfg.riPres = FALSE;
#ifdef ENABLE_CNM
    enbCfg->isCnmEnabled = wrSmDfltIsCnmEnabled;
#endif
    enbCfg->x2ConFlg =  wrSmDfltX2ConFlg;

    /* CNM_DEV */

   /* CSFB GERAN */
   enbCfg->csfbToGeranCfg.isGERANMeasAllowedForCCO = 
      wrSmDfltisGERANMeasAllowedForCCO;
   enbCfg->csfbToGeranCfg.isCCOAllowed = wrSmDfltGeranisCCOAllowed;
   enbCfg->csfbToGeranCfg.rrcDatCfmWaitTmr = wrSmDfltGeranrrcDatCfmWaitTmr;
   enbCfg->csfbToGeranCfg.GERANMeasWaitTmr = wrSmDfltGERANMeasWaitTmr;
   enbCfg->csfbToGeranCfg.T304expWaitTmrVal = wrSmDfltGeranT304expWaitTmrVal;
   /* CSFB GERAN */

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
#else
PUBLIC S16 smBuildCdma1xRTTBandClass(*lwrMng)
LwrMngmt *lwrMng
#endif
{
   LwrNeighBandClassCfg *bandClassCfg =NULLP;
   U8                    idx = 0;
   U8                    numOfBandCls = 0;

   lwrMng->t.protoCfg.type = WR_CFG_TYPE_BAND_CLASS;
   lwrMng->t.protoCfg.t.bandClassCfg.action = WR_ACT_ADD;
   lwrMng->t.protoCfg.t.bandClassCfg.cellId = 1;

   bandClassCfg =lwrMng->t.protoCfg.t.bandClassCfg.t.bcAddCfg;

   for(idx=0; idx<smCfgCb.noOfCdma1xBndCls; idx++)
   {
      if((WR_CDMA_2K_BAND_CLASS_0 <= smCfgCb.cdma1xBndClsCfg[idx]->bandClass) &&
      (WR_CDMA_2K_BAND_CLASS_21 >= smCfgCb.cdma1xBndClsCfg[idx]->bandClass))
      {
      bandClassCfg->bandType = LWR_NEIGH_BAND_TYPE_CDMA_1XRTT;
      bandClassCfg->t.bandClass1xRtt.reCfgType = wrSmDfltBandClsReCfgType;
      bandClassCfg->t.bandClass1xRtt.bandClassIndex = idx;

      bandClassCfg->t.bandClass1xRtt.bandClass = 
         smCfgCb.cdma1xBndClsCfg[idx]->bandClass;

      bandClassCfg->t.bandClass1xRtt.cellReselectPrior =
         smCfgCb.cdma1xBndClsCfg[idx]->cellReselectPrior;

      bandClassCfg->t.bandClass1xRtt.threshXHigh = 
         smCfgCb.cdma1xBndClsCfg[idx]->threshXHigh;

      bandClassCfg->t.bandClass1xRtt.threshXLow = 
         smCfgCb.cdma1xBndClsCfg[idx]->threshXLow;

      bandClassCfg++;
      numOfBandCls++;
   } 
   } 
   lwrMng->t.protoCfg.t.bandClassCfg.numBandClass = numOfBandCls;

   RETVALUE(ROK);
}
 
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
 *      File:    wr_smm_enbapp.c
 *
 */

PUBLIC S16 smBuildWrNr1xCdmaFreqCfg
(
 LwrMngmt *lwrMng
)
{
   LwrNeighFreqAddModCfg *nrFreqCfg =NULLP;
   U32 idx = 0;
 
   lwrMng->t.protoCfg.type = WR_CFG_TYPE_NEIGH_FREQ;
   lwrMng->t.protoCfg.t.neighFreqCfg.action = WR_ACT_ADD;
   lwrMng->t.protoCfg.t.neighFreqCfg.cellId = 1;

   lwrMng->t.protoCfg.t.neighFreqCfg.numFreq = smCfgCb.noOfNeighCdma1xFreq;
   nrFreqCfg = lwrMng->t.protoCfg.t.neighFreqCfg.t.addNeighFreq;

   for(idx=0; idx<smCfgCb.noOfNeighCdma1xFreq; idx++)
   {
     nrFreqCfg->type = WR_NEIGH_CELL_CDMA2K_1XRTT;
  
     nrFreqCfg->t.cdma1xFreq.reCfgType = wrSmDfltCdma1xNeighFreqReCfgType;
     nrFreqCfg->t.cdma1xFreq.freqIndex = idx;
     nrFreqCfg->t.cdma1xFreq.status = TRUE;

     nrFreqCfg->t.cdma1xFreq.bandClass = 
        smCfgCb.neighCdma1xFreq[idx]->bandClass;

     nrFreqCfg->t.cdma1xFreq.arfcn = 
        smCfgCb.neighCdma1xFreq[idx]->arfcn;

     nrFreqCfg->t.cdma1xFreq.searchRate = wrSmDfltCdma1xSrchWindSize;     
     nrFreqCfg->t.cdma1xFreq.offsetFreq = wrSmDfltCdma1xOffsetFreq;

     nrFreqCfg->t.cdma1xFreq.freqPriority = 
        smCfgCb.neighCdma1xFreq[idx]->freqPriority;

     nrFreqCfg++;
  }
   RETVALUE(ROK);
}

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
 *      File:    wr_smm_enbapp_rsys.c
 *
 */

PRIVATE S16 smBuildMobParams
(
  LwrCdma2k1xMobParams *mobParams
)
{
   mobParams->sidIncl = wrSmDfltCdma1xSidIncl; 
   mobParams->sid = wrSmDfltCdma1xSid;              
   mobParams->nidIncl = wrSmDfltCdma1xNidIncl;
   mobParams->nid = wrSmDfltCdma1xNid;
   mobParams->multSidsIncl = wrSmDfltCdma1xMultSidsIncl;
   mobParams->multSids = wrSmDfltCdma1xMultSids;         
   mobParams->multNidsIncl = wrSmDfltCdma1xMultNidsIncl;
   mobParams->multNids = wrSmDfltCdma1xMultNids;
   mobParams->regZoneIncl = wrSmDfltCdma1xRegZoneIncl;
   mobParams->regZone = wrSmDfltCdma1xRegZone;
   mobParams->totalZonesIncl = wrSmDfltCdma1xTotalZonesIncl;
   mobParams->totalZones = wrSmDfltCdma1xTotalZones;
   mobParams->zoneTimerIncl = wrSmDfltCdma1xZoneTimerIncl;
   mobParams->zoneTimer = wrSmDfltCdma1xZoneTimer;
   mobParams->pktZoneIdIncl = wrSmDfltCdma1xPktZoneIdIncl;
   mobParams->pktZoneId = wrSmDfltCdma1xPktZoneId;
   mobParams->pzIdHysParamIncl = wrSmDfltCdma1xPzIdHysParamIncl;
   mobParams->pzHystEnabled = wrSmDfltCdma1xPzHystEnabled;
   mobParams->pzHystInfoIncl = wrSmDfltCdma1xPzHystInfoIncl;
   mobParams->pzHystListLen = wrSmDfltCdma1xPzHystListLen;
   mobParams->pzHystActTimer = wrSmDfltCdma1xPzHystActTimer;
   mobParams->pzHystTimerMul = wrSmDfltCdma1xPzHystTimerMul;
   mobParams->pzHystTimerExp = wrSmDfltCdma1xPzHystTimerExp; 
   mobParams->pRevIncl = wrSmDfltCdma1xPRevIncl;
   mobParams->pRev = wrSmDfltCdma1xPRev;
   mobParams->minPRevIncl = wrSmDfltCdma1xMinPRevIncl;
   mobParams->minPRev = wrSmDfltCdma1xMinPRev;
   mobParams->negSlotCycIndxSupIncl = wrSmDfltCdma1xNegSlotCycIndxSupIncl;
   mobParams->negSlotCycIndxSup = wrSmDfltCdma1xNegSlotCycIndxSup;
   mobParams->encModeIncl = wrSmDfltCdma1xEncModeIncl;
   mobParams->encMode = wrSmDfltCdma1xEncMode;
   mobParams->encSupIncl = wrSmDfltCdma1xEncSupIncl;
   mobParams->encSup = wrSmDfltCdma1xEncSup;
   mobParams->sigEncSupIncl = wrSmDfltCdma1xSigEncSupIncl;
   mobParams->sigEncSup = wrSmDfltCdma1xSigEncSup;
   mobParams->msgIntSupIncl = wrSmDfltCdma1xMsgIntSupIncl;
   mobParams->msgIntSup = wrSmDfltCdma1xMsgIntSup;
   mobParams->sigIntSupINCLIncl = wrSmDfltCdma1xSigIntSupINCLIncl;
   mobParams->sigIntSupINCL = wrSmDfltCdma1xSigIntSupINCL;
   mobParams->sigIntSupIncl = wrSmDfltCdma1xSigIntSupIncl;
   mobParams->sigIntSup = wrSmDfltCdma1xSigIntSup;
   mobParams->authIncl = wrSmDfltCdma1xAuthIncl;
   mobParams->auth = wrSmDfltCdma1xAuth;
   mobParams->maxNumAltoSoIncl = wrSmDfltCdma1xMaxNumAltoSoIncl;
   mobParams->maxNumAltoSo = wrSmDfltCdma1xMaxNumAltoSo;
   mobParams->useSyncIdIncl = wrSmDfltCdma1xUseSyncIdIncl;
   mobParams->useSyncId = wrSmDfltCdma1xUseSyncId;
   mobParams->msInitPosLocSupIndIncl = wrSmDfltCdma1xMsInitPosLocSupIndIncl; 
   mobParams->msInitPosLocSupInd = wrSmDfltCdma1xMsInitPosLocSupInd;
   mobParams->mobQosIncl = wrSmDfltCdma1xMobQosIncl;
   mobParams->mobQos = wrSmDfltCdma1xMobQos;
   mobParams->bandClsInfoReqIncl = wrSmDfltCdma1xBandClsInfoReqIncl;
   mobParams->bandClsInfoReq = wrSmDfltCdma1xBandClsInfoReq;
   mobParams->bandClsIncl = wrSmDfltCdma1xBandClsIncl;
   mobParams->bandCls = wrSmDfltCdma1xBandCls;
   mobParams->bypassRegIndIncl = wrSmDfltCdma1xBypassRegIndIncl;
   mobParams->bypassRegInd = wrSmDfltCdma1xBypassRegInd;
   mobParams->altBandClsIncl = wrSmDfltCdma1xAltBandClsIncl;
   mobParams->altBandCls = wrSmDfltCdma1xAltBandCls;
   mobParams->maxAddServInstIncl = wrSmDfltCdma1xMaxAddServInstIncl;
   mobParams->maxAddServInst = wrSmDfltCdma1xMaxAddServInst;
   mobParams->homeRegIncl = wrSmDfltCdma1xHomeRegIncl;
   mobParams->homeReg = wrSmDfltCdma1xHomeReg;
   mobParams->forSidRegIncl = wrSmDfltCdma1xForSidRegIncl;
   mobParams->forSidReg = wrSmDfltCdma1xForSidReg;
   mobParams->forNidRegIncl = wrSmDfltCdma1xForNidRegIncl;
   mobParams->forNidReg = wrSmDfltCdma1xForNidReg;
   mobParams->powerUpRegIncl = wrSmDfltCdma1xPowerUpRegIncl;
   mobParams->powerUpReg = wrSmDfltCdma1xPowerUpReg;
   mobParams->powerDownRegIncl = wrSmDfltCdma1xPowerDownRegIncl;
   mobParams->powerDownReg = wrSmDfltCdma1xPowerDownReg;
   mobParams->parmRegIncl = wrSmDfltCdma1xParmRegIncl;
   mobParams->parmReg = wrSmDfltCdma1xParmReg;
   mobParams->regPrdIncl = wrSmDfltCdma1xRegPrdIncl;
   mobParams->regPrd = wrSmDfltCdma1xRegPrd;
   mobParams->regDistIncl = wrSmDfltCdma1xRegDistIncl;
   mobParams->regDist = wrSmDfltCdma1xRegDist;
   mobParams->prefMsIdTypeIncl = wrSmDfltCdma1xPrefMsIdTypeIncl;
   mobParams->prefMsIdType = wrSmDfltCdma1xPrefMsIdType;
   mobParams->extPrefMsIdTypeIncl = wrSmDfltCdma1xExtPrefMsIdTypeIncl;
   mobParams->extPrefMsIdType = wrSmDfltCdma1xExtPrefMsIdType;
   mobParams->meIdReqdIncl = wrSmDfltCdma1xMeIdReqdIncl;
   mobParams->meIdReqd = wrSmDfltCdma1xMeIdReqd;
   mobParams->mccIncl = wrSmDfltCdma1xMccIncl;
   mobParams->mcc = wrSmDfltCdma1xMcc;
   mobParams->imsi1112Incl = wrSmDfltCdma1xImsi1112Incl;
   mobParams->imsi1112 = wrSmDfltCdma1xImsi1112;
   mobParams->imsiTSupIncl = wrSmDfltCdma1xImsiTSupIncl;
   mobParams->imsiTSup = wrSmDfltCdma1xImsiTSup;
   mobParams->recntMsgIndIncl = wrSmDfltCdma1xRecntMsgIndIncl;
   mobParams->recntMsgInd = wrSmDfltCdma1xRecntMsgInd;
   mobParams->rerModeSupIncl = wrSmDfltCdma1xRerModeSupIncl;
   mobParams->rerModeSup = wrSmDfltCdma1xRerModeSup;
   mobParams->tzkModeSupIncl = wrSmDfltCdma1xTzkModeSupIncl;
   mobParams->tzkModeSup = wrSmDfltCdma1xTzkModeSup;
   mobParams->tzkIdIncl = wrSmDfltCdma1xTzkIdIncl;
   mobParams->tzkId = wrSmDfltCdma1xTzkId;
   mobParams->pilRepIncl = wrSmDfltCdma1xPilRepIncl;
   mobParams->pilRep = wrSmDfltCdma1xPilRep;
   mobParams->sdbSupIncl = wrSmDfltCdma1xSdbSupIncl;
   mobParams->sdbSup = wrSmDfltCdma1xSdbSup;
   mobParams->autoFcsoAllIncl = wrSmDfltCdma1xAutoFcsoAllIncl;
   mobParams->autoFcsoAll = wrSmDfltCdma1xAutoFcsoAll;
   mobParams->sdbInRcnmIncl = wrSmDfltCdma1xSdbInRcnmIncl;
   mobParams->sdbInRcnm = wrSmDfltCdma1xSdbInRcnm;
   mobParams->fpcFchIncl = wrSmDfltCdma1xFpcFchIncl;
   mobParams->fpcFchInitSetptRC3 = wrSmDfltCdma1xFpcFchInitSetptRC3;
   mobParams->fpcFchInitSetptRC4 = wrSmDfltCdma1xFpcFchInitSetptRC4;
   mobParams->fpcFchInitSetptRC5 = wrSmDfltCdma1xFpcFchInitSetptRC5;
   mobParams->fpcFchInitSetptRC11 = wrSmDfltCdma1xFpcFchInitSetptRC11;
   mobParams->fpcFchInitSetptRC12 = wrSmDfltCdma1xFpcFchInitSetptRC12;
   mobParams->tAddIncl = wrSmDfltCdma1xTAddIncl;
   mobParams->tAdd = wrSmDfltCdma1xTAdd;
   mobParams->pilotIncIncl = wrSmDfltCdma1xPilotInclIncl;
   mobParams->PilotInc = wrSmDfltCdma1xPilotInc;
   mobParams->randIncl = wrSmDfltCdma1xRandIncl;
   mobParams->rand = wrSmDfltCdma1xRand;
   mobParams->lpSecIncl = wrSmDfltCdma1xLpSecIncl;
   mobParams->lpSec = wrSmDfltCdma1xLpSec;
   mobParams->ltmOffIncl = wrSmDfltCdma1xLtmOffIncl;
   mobParams->ltmOff = wrSmDfltCdma1xLtmOff;
   mobParams->dayltIncl = wrSmDfltCdma1xDayltIncl;
   mobParams->daylt = wrSmDfltCdma1xDaylt;
   mobParams->gcsnaL2ActTimerIncl = wrSmDfltCdma1xGcsnaL2ActTimerIncl;
   mobParams->gcsnaL2ActTimer = wrSmDfltCdma1xGcsnaL2ActTimer;
   mobParams->gcsnaSeqCtxtTimerIncl = wrSmDfltCdma1xGcsnaSeqCtxtTimerIncl;
   mobParams->gcsnaSeqCtxtTimer = wrSmDfltCdma1xGcsnaSeqCtxtTimer;
   mobParams->reserved = wrSmDfltCdma1xReserved;

   RETVALUE(ROK);
}

/*
 *
 *       Fun: smBuildWrNr1xCdmaCellCfg- Build Neighbour cell
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
PUBLIC S16 smBuildWrNr1xCdmaCellCfg
(
 LwrMngmt *lwrMng
)
{
   LwrNeighCellAddCfg *nrCellCfg =NULLP;
   U32 idx = 0;
   
   lwrMng->t.protoCfg.type = WR_CFG_TYPE_NEIGH_CELL;
   lwrMng->t.protoCfg.t.neighCellCfg.action = WR_ACT_ADD;
   lwrMng->t.protoCfg.t.neighCellCfg.cellId = 1;

   lwrMng->t.protoCfg.t.neighCellCfg.numCells = smCfgCb.noOfNeighCdma1xCells;
   nrCellCfg =lwrMng->t.protoCfg.t.neighCellCfg.t.neighAddCfg;

   for(idx=0; idx<smCfgCb.noOfNeighCdma1xCells; idx++)
   {
     nrCellCfg->type = WR_NEIGH_CELL_CDMA2K_1XRTT;

     nrCellCfg->t.cdma1xCell.cellNum = 1;
     nrCellCfg->t.cdma1xCell.nrIndex = idx;
     nrCellCfg->t.cdma1xCell.status = TRUE;

     nrCellCfg->t.cdma1xCell.bandClass = 
        smCfgCb.neighCdma1xCell[idx]->bandClass;

     nrCellCfg->t.cdma1xCell.arfcn = 
        smCfgCb.neighCdma1xCell[idx]->arfcn;

     nrCellCfg->t.cdma1xCell.pnOffset = 
        smCfgCb.neighCdma1xCell[idx]->pnOffset;

     nrCellCfg->t.cdma1xCell.curRank = 
        smCfgCb.neighCdma1xCell[idx]->curRank;

     nrCellCfg->t.cdma1xCell.neighCellFlag = 
        smCfgCb.neighCdma1xCell[idx]->neighCellFlag;

      nrCellCfg->t.cdma1xCell.cellId = 
         smCfgCb.neighCdma1xCell[idx]->cid;

      /* As of now we are not taking from config file,
       * so we are copying the default values */

      nrCellCfg->t.cdma1xCell.systemType = wrSmDfltCdmaSystemType;

      nrCellCfg->t.cdma1xCell.channelNum = wrSmDfltCdmaChannelNum;

      nrCellCfg->t.cdma1xCell.pilotOneWay = wrSmDfltCdmaPilotOneWay;

      nrCellCfg->t.cdma1xCell.secId.cdmaType = LWR_1XRTT;
      nrCellCfg->t.cdma1xCell.secId.isOctStrPres = TRUE;

      if(TRUE == nrCellCfg->t.cdma1xCell.secId.isOctStrPres)
      {
         wrFillSectorId(&nrCellCfg->t.cdma1xCell.secId.s.sectorIdPtr, 5,
               (Data *)&wrSmDfltSecIdPerNgh);
      }
      else
      {
         nrCellCfg->t.cdma1xCell.secId.s.cdmaSectorId.marketId = 
            wrSmDfltCdmaMktId;
         nrCellCfg->t.cdma1xCell.secId.s.cdmaSectorId.switchNum = 
            wrSmDfltCdmaSwitchNum;
         nrCellCfg->t.cdma1xCell.secId.s.cdmaSectorId.cellId = 
            wrSmDfltCdmaCellId;
         nrCellCfg->t.cdma1xCell.secId.s.cdmaSectorId.sectorNum = 
            wrSmDfltCdmaSectorNum;
      }
     smBuildMobParams(&nrCellCfg->t.cdma1xCell.mobParams);
     nrCellCfg++;
  }
   RETVALUE(ROK);
} 

 
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

   LwrCellMibCfgGrp  *lwrMib = NULLP;
   LwrCellSib1CfgGrp *sib1 =NULLP;
   U8                cfgGrpIndex = 0;

   TRC2(smBuildModCellCfg);

   lwrMng->t.protoCfg.type = WR_CFG_TYPE_CELL;
   lwrMng->t.protoCfg.t.cellCfg.action = WR_ACT_MOD;

   lwrMng->t.protoCfg.t.cellCfg.t.modCell.cellId = 1; /* this ownCellID in case of PLD based, jst logic cell */
   lwrMng->t.protoCfg.t.cellCfg.t.modCell.cellIndex = WR_DFLT_CELL_IDX;

   /* mem_issue_fix: Allocating memory for all the cell group though
    * all the groups are not being used as of now. This helps to avoid 
    * multiple allocation and can have a common logic to free the memory */
   WR_ALLOC(&(lwrMng->t.protoCfg.t.cellCfg.t.modCell.cellCfgGrp[cfgGrpIndex]), \
                       (sizeof(LwrCellCfgGrp)));
   if(NULLP == lwrMng->t.protoCfg.t.cellCfg.t.modCell.cellCfgGrp[cfgGrpIndex])
   {
      RLOG0(L_FATAL, "Memory allocation failed for cell modification");
      RETVALUE(RFAILED);
   }
   /*Filling MIB params*/
   lwrMng->t.protoCfg.t.cellCfg.t.modCell.cellCfgGrp[cfgGrpIndex]->grpType 
      = WR_CELL_CFG_GRP_MIB;
   lwrMib = &(lwrMng->t.protoCfg.t.cellCfg.t.modCell.cellCfgGrp[cfgGrpIndex]->
         t.mibCfgGrp);
   lwrMng->t.protoCfg.t.cellCfg.t.modCell.reCfgType = LWR_CELL_CFG_GRP;  
   lwrMib->dlBw = wrSmDfltCellDlBW;
   lwrMib->phichCfg.phichDur = wrSmDfltPhichDur;
   lwrMib->phichCfg.phichRes = wrSmDfltPhichRes;
   cfgGrpIndex++;

   /*Filling sib1 params*/
   lwrMng->t.protoCfg.t.cellCfg.t.modCell.cellCfgGrp[cfgGrpIndex]->grpType 
                                                        = WR_CELL_CFG_GRP_SIB1;
                                                          
   sib1 =&(lwrMng->t.protoCfg.t.cellCfg.t.modCell.cellCfgGrp[cfgGrpIndex]->
         t.sib1CfgGrp);

   sib1->reCfgType = LWR_CELL_SIB1_CFG_CELL_ACCESS_INFO;

   sib1->cellAccessInfo.plmnInfo[0].rsrvdFrOperUse =
      wrSmDfltCellRsrvdForOperUse;

   /* Modify tac */
   sib1->cellAccessInfo.tac = (U16)(wrSmDfltTac + 1);

   sib1->cellAccessInfo.cellId = wrSmDfltCellId; /* wrSmDfltCellIdentity */
   /* CSG_DEV start */
   if (WR_ENB_OPEN_ACCESS == wrSmDfltAccessMode)
   {
      sib1->cellAccessInfo.csgIndication = FALSE;
      sib1->cellAccessInfo.csgIdentity.pres = NOTPRSNT;
   }
   else if(WR_ENB_CLOSED_ACCESS == wrSmDfltAccessMode)
   {
      sib1->cellAccessInfo.csgIndication = TRUE;
      wrFillTknU32(&sib1->cellAccessInfo.csgIdentity, wrSmDfltCsgIdVal);
   }
   else /* hybrid access */
   {
      sib1->cellAccessInfo.csgIndication = FALSE;
      wrFillTknU32(&sib1->cellAccessInfo.csgIdentity, wrSmDfltCsgIdVal);
   }
   /* CSG_DEV end */
   sib1->cellAccessInfo.cellBarred = (Bool)wrSmDfltCellBarred;
   sib1->cellAccessInfo.intraFreqReSelection = (Bool)wrSmDfltIntraFreqResel;

   sib1->cellSelectInfo.qRxLevMin = (S8)wrSmDlftqRxLevMin;
   /* Start - Fix for ccpu00123651 */
   wrFillTknU8(&(sib1->cellSelectInfo.qRxLevMinOffset),
         (U8)wrSmDfltqRxlevminoffset);
   /* End - Fix for ccpu00123651 */

   sib1->pMax = (S8)wrSmDfltPMax;
   
   sib1->siWindowSize = wrSmDfltSiWinSize;
/* ACC-TDD */   
#ifdef LTE_TDD
   sib1->tddCfg.specialSubframeCfg = wrSmTddSpecialSubframeConfig;
   sib1->tddCfg.subframeCfg = wrSmTddSubframeConfig;
#else
   // Not used for FDD - zero the configuration just to be safe
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
*       File:  wr_sm_wr_cntrl.c 
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
   U8                        idx = 0;
   U8                        plmnIdx = 0;
   LwrUtraNeighCell          *utraNeghCell = NULLP;

   TRC2(smBuildWrNehUtraFddCellCfg);

   for (idx = 0; idx < wrSmDfltNumofUtraFddNrCell; idx++) 
   {
      lwrMng->t.protoCfg.t.neighCellCfg.t.neighAddCfg[idx].type = 
         WR_NEIGH_CELL_UTRA_FDD;

      utraNeghCell =&(lwrMng->t.protoCfg.t.neighCellCfg.t.neighAddCfg[idx].
            t.utranCell);

      utraNeghCell->nrIndex = wrSmDfltExtUtraCell[idx].relationIdx;
      utraNeghCell->rncId= smCfgCb.nghUtraFddInfo[idx]->rncId;

      utraNeghCell->cId = smCfgCb.nghUtraFddInfo[idx]->cId;

      utraNeghCell->numPlmn = wrSmDfltExtUtraCell[idx].numPlmns;
      for(plmnIdx = 0; plmnIdx < utraNeghCell->numPlmn; plmnIdx++)
      {
         /*Fix for ccpu00124056 */
         utraNeghCell->plmnId[plmnIdx].mcc[0] = wrSmDfltExtUtraCell[idx].
            plmnIds[plmnIdx].mcc[0];
         utraNeghCell->plmnId[plmnIdx].mcc[1] = wrSmDfltExtUtraCell[idx].
            plmnIds[plmnIdx].mcc[1];
         utraNeghCell->plmnId[plmnIdx].mcc[2] = wrSmDfltExtUtraCell[idx].
            plmnIds[plmnIdx].mcc[2];

         utraNeghCell->plmnId[plmnIdx].numMncDigits = wrSmDfltExtUtraCell[idx].
            plmnIds[plmnIdx].numMncDigits;

         utraNeghCell->plmnId[plmnIdx].mnc[0] = wrSmDfltExtUtraCell[idx].
            plmnIds[plmnIdx].mnc[0];
         utraNeghCell->plmnId[plmnIdx].mnc[1] = wrSmDfltExtUtraCell[idx].
            plmnIds[plmnIdx].mnc[1];

         if(3 == utraNeghCell->plmnId[plmnIdx].numMncDigits)
            utraNeghCell->plmnId[plmnIdx].mnc[2] = wrSmDfltExtUtraCell[idx].
               plmnIds[plmnIdx].mnc[2];
         /*End Fix for ccpu00124056 */
      }

      utraNeghCell->duplexMode = (U8)wrSmDfltExtUtraCell[idx].duplexType;
      utraNeghCell->t.utraFdd.psc = smCfgCb.nghUtraFddInfo[idx]->psc;
      utraNeghCell->t.utraFdd.arfcnDl = smCfgCb.nghUtraFddInfo[idx]->arfcnDl;
      utraNeghCell->t.utraFdd.arfcnUl = smCfgCb.nghUtraFddInfo[idx]->arfcnUl;
      utraNeghCell->t.utraFdd.isHOAllowed = TRUE;
      utraNeghCell->t.utraFdd.pcpich = 70;
      utraNeghCell->lac = smCfgCb.nghUtraFddInfo[idx]->lac;
      utraNeghCell->rac = smCfgCb.nghUtraFddInfo[idx]->rac;
      utraNeghCell->isRimSupported = smCfgCb.nghUtraFddInfo[idx]->isRimSupported;
      utraNeghCell->isVoipCapable = smCfgCb.nghUtraFddInfo[idx]->voipCapable;
      utraNeghCell->isPsHOCapable = smCfgCb.nghUtraFddInfo[idx]->psHOCapable;

      /*
         utraNeghCell->curRank      = wrSmDfltUtranNrCurRank;
         utraNeghCell->prevRank     = wrSmDfltUtranNrPrevRank;
         */
   }
   RETVALUE(ROK);
} /* end of smBuildWrNehUtraFddCellCfg */

/*
*
*       Fun:   smBuildWrNehUtraTddCellCfg
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
PUBLIC S16 smBuildWrNehUtraTddCellCfg
(
LwrMngmt                     *lwrMng
)
#else /* if ANSI not defined*/
PUBLIC S16 smBuildWrNehUtraTddCellCfg(*lwrMng)
LwrMngmt                     *lwrMng;
#endif /* end of ANSI*/
{
   U8                        idx = 0;
   U8                        plmnIdx = 0;
   LwrUtraNeighCell          *utraNeghCell = NULLP;

   TRC2(smBuildWrNehUtraTddCellCfg);

   for (idx = 0; idx < wrSmDfltNumofUtraTddNrCell; idx++) 
   {
      lwrMng->t.protoCfg.t.neighCellCfg.t.neighAddCfg[idx].type = 
         WR_NEIGH_CELL_UTRA_TDD;

      utraNeghCell =&(lwrMng->t.protoCfg.t.neighCellCfg.t.neighAddCfg[idx].
            t.utranCell);

      utraNeghCell->nrIndex = wrSmDfltExtUtraTddCell[idx].relationIdx;
      utraNeghCell->rncId= smCfgCb.nghUtraTddInfo[idx]->rncId;

      utraNeghCell->cId = smCfgCb.nghUtraTddInfo[idx]->cId;

      utraNeghCell->numPlmn = wrSmDfltExtUtraTddCell[idx].numPlmns;
      for(plmnIdx = 0; plmnIdx < utraNeghCell->numPlmn; plmnIdx++)
      {
         /*Fix for ccpu00124056 */
         utraNeghCell->plmnId[plmnIdx].mcc[0] = wrSmDfltExtUtraTddCell[idx].
            plmnIds[plmnIdx].mcc[0];
         utraNeghCell->plmnId[plmnIdx].mcc[1] = wrSmDfltExtUtraTddCell[idx].
            plmnIds[plmnIdx].mcc[1];
         utraNeghCell->plmnId[plmnIdx].mcc[2] = wrSmDfltExtUtraTddCell[idx].
            plmnIds[plmnIdx].mcc[2];

         utraNeghCell->plmnId[plmnIdx].numMncDigits = wrSmDfltExtUtraTddCell[idx].
            plmnIds[plmnIdx].numMncDigits;

         utraNeghCell->plmnId[plmnIdx].mnc[0] = wrSmDfltExtUtraTddCell[idx].
            plmnIds[plmnIdx].mnc[0];
         utraNeghCell->plmnId[plmnIdx].mnc[1] = wrSmDfltExtUtraTddCell[idx].
            plmnIds[plmnIdx].mnc[1];

         if(3 == utraNeghCell->plmnId[plmnIdx].numMncDigits)
            utraNeghCell->plmnId[plmnIdx].mnc[2] = wrSmDfltExtUtraTddCell[idx].
               plmnIds[plmnIdx].mnc[2];
         /*End Fix for ccpu00124056 */
      }

      utraNeghCell->duplexMode = (U8)wrSmDfltExtUtraTddCell[idx].duplexType;
      utraNeghCell->t.utraTdd.cellParamId = smCfgCb.nghUtraTddInfo[idx]->cellParamId;
      utraNeghCell->t.utraTdd.arfcn = smCfgCb.nghUtraTddInfo[idx]->arfcn;
      utraNeghCell->t.utraTdd.pccpch = smCfgCb.nghUtraTddInfo[idx]->ccpchTPwr;
      utraNeghCell->lac = smCfgCb.nghUtraTddInfo[idx]->lac;
      utraNeghCell->rac = smCfgCb.nghUtraTddInfo[idx]->rac;
      utraNeghCell->isRimSupported = smCfgCb.nghUtraTddInfo[idx]->isRimSupported;

      /*
         utraNeghCell->curRank      = wrSmDfltUtranNrCurRank;
         utraNeghCell->prevRank     = wrSmDfltUtranNrPrevRank;
         */
   }
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
 *       Fun:    smBuildWrNrUtraFddFreqCfg - Build Neighbour frequency
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
PUBLIC S16 smBuildWrNrUtraFddFreqCfg
(
 LwrMngmt   *lwrMng
)
{
   U8  idx = 0;
   LwrNeighFreqAddModCfg *nrFreqCfg =NULLP;

   TRC2(smBuildWrNrUtraFddFreqCfg);

   for(idx = 0; idx < smCfgCb.noOfUtraFddFreq; idx++)
   {
      nrFreqCfg = &lwrMng->t.protoCfg.t.neighFreqCfg.t.addNeighFreq[idx];
      nrFreqCfg->type = WR_NEIGH_CELL_UTRA_FDD;
      nrFreqCfg->t.utranFreq.reCfgType = 0x000fff;
      nrFreqCfg->t.utranFreq.freqIndex = (U8)(wrSmDfltUtraFaPriorInfo[idx].
            faIndex);
      nrFreqCfg->t.utranFreq.t.utraFdd.arfcnUl = smCfgCb.nghUtraFddFreq[idx]->t.utraFdd.arfcnUl;
      nrFreqCfg->t.utranFreq.t.utraFdd.arfcnDl =  smCfgCb.nghUtraFddFreq[idx]->t.utraFdd.arfcnDl;
      
      nrFreqCfg->t.utranFreq.priority.pres = PRSNT_NODEF;
      nrFreqCfg->t.utranFreq.priority.val = (U8)wrSmDfltUtraFaPriorInfo[idx].
         priority;
      nrFreqCfg->t.utranFreq.threshXhigh = (U8)wrSmDfltUtraFaPriorInfo[idx].
         threshXhigh;
      nrFreqCfg->t.utranFreq.threshXlow = (U8)wrSmDfltUtraFaPriorInfo[idx].
         threshXLow;
      nrFreqCfg->t.utranFreq.qRxLevMin = (U8)wrSmDfltUtraFaPriorInfo[idx].
         qRxLevMin;
      nrFreqCfg->t.utranFreq.pMaxUtra = (U8)wrSmDfltUtraFaPriorInfo[idx].
         pMaxUtra;
      nrFreqCfg->t.utranFreq.t.utraFdd.pQualMin = (U8)wrSmDfltUtraFaPriorInfo[idx].
         qQualMin;
      nrFreqCfg->t.utranFreq.offsetFreq = (U8)wrSmDfltUtraFaPriorInfo[idx].
         offsetFreq;
      nrFreqCfg->t.utranFreq.duplexMode = (U8)wrSmDfltUtraFaPriorInfo[idx].
         duplexType;
      nrFreqCfg->t.utranFreq.t.utraFdd.thresR9.pres = PRSNT_NODEF;
      nrFreqCfg->t.utranFreq.t.utraFdd.thresR9.thresXHighQR9 =
         (U8)wrSmDfltUtraFaPriorInfo[idx].threshXhighQRel9;
      nrFreqCfg->t.utranFreq.t.utraFdd.thresR9.thresXLowQR9 =
         (U8)wrSmDfltUtraFaPriorInfo[idx].threshXlowQRel9;
   }
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
 *      File:    wr_smm_enbapp.c
 *
 */
PUBLIC S16 smBuildWrNrUtraTddFreqCfg
(
 LwrMngmt   *lwrMng
 )
{
   U8  idx = 0;
   LwrNeighFreqAddModCfg *nrFreqCfg =NULLP;

   TRC2(smBuildWrNrUtraTddFreqCfg);

   for(idx = 0; idx < smCfgCb.noOfUtraTddFreq; idx++)
   {
      nrFreqCfg = &lwrMng->t.protoCfg.t.neighFreqCfg.t.addNeighFreq[idx];
      nrFreqCfg->type = WR_NEIGH_CELL_UTRA_TDD;
      nrFreqCfg->t.utranFreq.reCfgType = 0x000fff;
      nrFreqCfg->t.utranFreq.freqIndex = (U8)(wrSmDfltUtraTddFaPriorInfo[idx].
            faIndex);
      nrFreqCfg->t.utranFreq.t.utraTdd.arfcn = smCfgCb.nghUtraTddFreq[idx]->t.utraTdd.arfcn;
      nrFreqCfg->t.utranFreq.t.utraTdd.tddMode = smCfgCb.nghUtraTddFreq[idx]->t.utraTdd.tddMode;
      nrFreqCfg->t.utranFreq.t.utraTdd.bandIndicator = smCfgCb.nghUtraTddFreq[idx]->t.utraTdd.bandIndicator;
      
      nrFreqCfg->t.utranFreq.priority.pres = PRSNT_NODEF;
      nrFreqCfg->t.utranFreq.priority.val = (U8)wrSmDfltUtraTddFaPriorInfo[idx].
         priority;
      nrFreqCfg->t.utranFreq.threshXhigh = (U8)wrSmDfltUtraTddFaPriorInfo[idx].
         threshXhigh;
      nrFreqCfg->t.utranFreq.threshXlow = (U8)wrSmDfltUtraTddFaPriorInfo[idx].
         threshXLow;
      nrFreqCfg->t.utranFreq.qRxLevMin = (U8)wrSmDfltUtraTddFaPriorInfo[idx].
         qRxLevMin;
      nrFreqCfg->t.utranFreq.pMaxUtra = (U8)wrSmDfltUtraTddFaPriorInfo[idx].
         pMaxUtra;
      nrFreqCfg->t.utranFreq.offsetFreq = (U8)wrSmDfltUtraTddFaPriorInfo[idx].
         offsetFreq;
      nrFreqCfg->t.utranFreq.duplexMode = (U8)wrSmDfltUtraTddFaPriorInfo[idx].
         duplexType;
   }
   RETVALUE(ROK);
} /* end of smBuildWrNrUtraTddFreqCfg */


/*
 *
 *       Fun:    smBuildWrNrGeranFreqCfg - Build Neighbour frequency
 *
 *       Desc:  LwrMngmt filled for GERAN freq configuration 
 *
 *       Ret:    Void
 *
 *       Notes:  None
 *
 *      File:    wr_smm_enbapp_utils.c
 *
 */
PUBLIC S16 smBuildWrNrGeranFreqCfg
(
 LwrMngmt *lwrMng
)
{
   U8  idx = 0;
   LwrNeighFreqAddModCfg *nrFreqCfg = NULLP;
   LwrNeighGERANFreq  *geranFreqCfg = NULLP;

   TRC2(smBuildWrNrGeranFreqCfg);

   lwrMng->t.protoCfg.type = WR_CFG_TYPE_NEIGH_FREQ;
   lwrMng->t.protoCfg.t.neighFreqCfg.action = WR_ACT_ADD;
   lwrMng->t.protoCfg.t.neighFreqCfg.cellId = 1; /* this is ownCellID in case of
                                           PLD based conf, jst a logic entity */
   lwrMng->t.protoCfg.t.neighFreqCfg.numFreq = (U8)smCfgCb.noOfGeranFreq;
   nrFreqCfg = lwrMng->t.protoCfg.t.neighFreqCfg.t.addNeighFreq;

   for(idx = 0; idx < smCfgCb.noOfGeranFreq; idx++)
   {
      geranFreqCfg = smCfgCb.geranFreq[idx];
      nrFreqCfg->type = WR_NEIGH_CELL_GERAN;
      nrFreqCfg->t.geranFreq.reCfgType     = 0x000fff;     
      nrFreqCfg->t.geranFreq.freqIndex     = idx+1;     
      nrFreqCfg->t.geranFreq.status        = wrSmDfltStatus;
      nrFreqCfg->t.geranFreq.bandIndicator = geranFreqCfg->bandIndicator;
      nrFreqCfg->t.geranFreq.bcchArfcn     = geranFreqCfg->bcchArfcn;
      nrFreqCfg->t.geranFreq.qRxLevMin     = wrSmDfltGeranqRxLevMin;
      nrFreqCfg->t.geranFreq.cellReselePri = geranFreqCfg->cellReselePri;
      nrFreqCfg->t.geranFreq.threshXHigh   = wrSmDfltGeranthreshXHigh;
      nrFreqCfg->t.geranFreq.threshXLow    = wrSmDfltGeranthreshXLow;
      nrFreqCfg->t.geranFreq.ncc_Permitted = geranFreqCfg->ncc_Permitted;
      nrFreqCfg->t.geranFreq.pMaxGeran.pres= PRSNT_NODEF;
      nrFreqCfg->t.geranFreq.pMaxGeran.val = wrSmDfltGeranpMaxGeran;
      nrFreqCfg++;
   }
   RETVALUE(ROK);
} /* end of smBuildWrNrGeranFreqCfg*/


/*
 *
 *       Fun:    smBuildWrNrGeranCellCfg - Build Neighbour cell cfg
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
PUBLIC S16 smBuildWrNrGeranCellCfg
(
 LwrMngmt *lwrMng
)
{
   U8  idx = 0;
   LwrNeighCellAddCfg *nrCellCfg =NULLP;
   LwrGERANNeighCell   *geranCellCfg = NULLP;

   TRC2(smBuildWrNrGeranCellCfg);

   lwrMng->t.protoCfg.type = WR_CFG_TYPE_NEIGH_CELL;
   lwrMng->t.protoCfg.t.neighCellCfg.action = WR_ACT_ADD;
   lwrMng->t.protoCfg.t.neighCellCfg.cellId = 1; /* this is ownCellID in case of
                                           PLD based conf, jst a logic entity */
   lwrMng->t.protoCfg.t.neighCellCfg.numCells = (U8)smCfgCb.noOfGeranCell;
   nrCellCfg =lwrMng->t.protoCfg.t.neighCellCfg.t.neighAddCfg; 
   for(idx = 0; idx < smCfgCb.noOfGeranCell; idx++) /*TODO:check for >/= smCfgCb.noOfGeranCell */
   {
      geranCellCfg = smCfgCb.geranCell[idx];
      nrCellCfg->type                      = WR_NEIGH_CELL_GERAN;
      nrCellCfg->t.geranCell.reCfgType     = wrSmDfltGeranReCfgType;
      nrCellCfg->t.geranCell.status        = wrSmDfltStatus;
      nrCellCfg->t.geranCell.nrIndex       = idx+1;
      nrCellCfg->t.geranCell.plmnId.numMncDigits = geranCellCfg->plmnId.numMncDigits;
      nrCellCfg->t.geranCell.plmnId.mcc[0] = geranCellCfg->plmnId.mcc[0];
      nrCellCfg->t.geranCell.plmnId.mcc[1] = geranCellCfg->plmnId.mcc[1];
      nrCellCfg->t.geranCell.plmnId.mcc[2] = geranCellCfg->plmnId.mcc[2];
      nrCellCfg->t.geranCell.plmnId.mnc[0] = geranCellCfg->plmnId.mnc[0];
      nrCellCfg->t.geranCell.plmnId.mnc[1] = geranCellCfg->plmnId.mnc[1];
      if(geranCellCfg->plmnId.numMncDigits == 3)
      {
         nrCellCfg->t.geranCell.plmnId.mnc[2] = geranCellCfg->plmnId.mnc[2];
      }
      nrCellCfg->t.geranCell.lac           = geranCellCfg->lac;
      nrCellCfg->t.geranCell.rac           = geranCellCfg->rac;
      nrCellCfg->t.geranCell.bsic          = geranCellCfg->bsic;
      nrCellCfg->t.geranCell.ci            = geranCellCfg->ci;
      nrCellCfg->t.geranCell.bandIndicator = geranCellCfg->bandIndicator;
      nrCellCfg->t.geranCell.bcchArfcn     = geranCellCfg->bcchArfcn;
      nrCellCfg->t.geranCell.nccPermittedMeas = geranCellCfg->nccPermittedMeas;
      nrCellCfg->t.geranCell.isDTMCapable  = geranCellCfg->isDTMCapable;
      nrCellCfg->t.geranCell.rimSupportForGeranNgh  = 
                                           geranCellCfg->rimSupportForGeranNgh;

      nrCellCfg->t.geranCell.ncoVal        = geranCellCfg->ncoVal;
      nrCellCfg->t.geranCell.ncoPres       = geranCellCfg->ncoPres;
      nrCellCfg->t.geranCell.offsetFreq = geranCellCfg->offsetFreq ; 
      nrCellCfg++;
   }
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
   U32 idx = 0;
   LwrNeighFreqAddModCfg *nrFreqCfg =NULLP;

   TRC2(smBuildWrNrEutraFreqCfg);

   lwrMng->t.protoCfg.type = WR_CFG_TYPE_NEIGH_FREQ;
   lwrMng->t.protoCfg.t.neighFreqCfg.action = WR_ACT_ADD;
   lwrMng->t.protoCfg.t.neighFreqCfg.cellId = 1; /* this is ownCellID in case of PLD based, jst a logic entity */
   lwrMng->t.protoCfg.t.neighFreqCfg.numFreq = wrSmDfltNumNrFreq;  

   nrFreqCfg = (lwrMng->t.protoCfg.t.neighFreqCfg.t.addNeighFreq);
   nrFreqCfg->type = WR_NEIGH_CELL_EUTRA;
   for(idx=0; idx < wrSmDfltNumNrFreq; idx++)
   {
     nrFreqCfg->t.eutranFreq.reCfgType = wrSmDfltReCfgType;
     nrFreqCfg->t.eutranFreq.status = wrSmDfltStatus;
     nrFreqCfg->t.eutranFreq.earfcnUl = smCfgCb.nghEutraFreq[idx]->arfcnUl;
     nrFreqCfg->t.eutranFreq.earfcnDl = smCfgCb.nghEutraFreq[idx]->arfcnDl;

     nrFreqCfg->t.eutranFreq.priority = wrSmDfltPriority;
     nrFreqCfg->t.eutranFreq.qRxLevMin = wrSmDfltQRxLevMin;
     nrFreqCfg->t.eutranFreq.pMaxUsage = wrSmDfltPMaxUsage;
     nrFreqCfg->t.eutranFreq.pMax = wrSmDfltS3pMax;
     nrFreqCfg->t.eutranFreq.tReselection = wrSmDfltTReselection;
     nrFreqCfg->t.eutranFreq.sfUsage = wrSmDfltSfUsage;
     nrFreqCfg->t.eutranFreq.tReselectionSFMed = wrSmDfltTReselectionSFMed;
     nrFreqCfg->t.eutranFreq.tReselectionSFHigh = wrSmDfltTReselectionSFHigh;
     nrFreqCfg->t.eutranFreq.sIntraSearch.pres = PRSNT_NODEF;
     nrFreqCfg->t.eutranFreq.sIntraSearch.val = wrSmDfltSIntraSearch;
     nrFreqCfg->t.eutranFreq.sNonIntraSearch.pres = PRSNT_NODEF;
     nrFreqCfg->t.eutranFreq.sNonIntraSearch.val = wrSmDfltSNonIntraSearch;
     nrFreqCfg->t.eutranFreq.servThresholdLow = wrSmDfltServThresholdLow;
     nrFreqCfg->t.eutranFreq.threshXHigh = wrSmDfltThreshXHigh;
     nrFreqCfg->t.eutranFreq.threshXLow =  wrSmDfltThreshXLow;
     nrFreqCfg->t.eutranFreq.measBandwidth.pres =  PRSNT_NODEF;
     nrFreqCfg->t.eutranFreq.measBandwidth.val = wrSmDfltCellDlBW;
     nrFreqCfg->t.eutranFreq.presAntennaPort1 =  wrSmDfltPresAntennaPort1;
     nrFreqCfg->t.eutranFreq.neighCellCfg =  wrSmDfltNeighCellCfg;
     nrFreqCfg->t.eutranFreq.qOffsetFreq = LWR_Q_OFFSET_RANGE_0;
     nrFreqCfg->t.eutranFreq.offsetFreq =  LWR_Q_OFFSET_RANGE_0;
     nrFreqCfg->t.eutranFreq.sIntraSearchRel9.pres =  PRSNT_NODEF;
     nrFreqCfg->t.eutranFreq.sIntraSearchRel9.sSearchPRel9 =
        wrSmDfltsSearchPRel9;
     nrFreqCfg->t.eutranFreq.sIntraSearchRel9.sSearchQRel9 =
        wrSmDfltsSearchQRel9;
     nrFreqCfg->t.eutranFreq.sInterSearchRel9.pres = PRSNT_NODEF;
     nrFreqCfg->t.eutranFreq.sInterSearchRel9.sSearchPRel9 =
        wrSmDfltsSearchPRel9;
     nrFreqCfg->t.eutranFreq.sInterSearchRel9.sSearchQRel9 =
        wrSmDfltsSearchQRel9;
     nrFreqCfg->t.eutranFreq.qQualMinRel9.pres = PRSNT_NODEF;
     nrFreqCfg->t.eutranFreq.qQualMinRel9.val = wrSmDfltDualMinRel9;
     nrFreqCfg->t.eutranFreq.reselecThreshQRel9.pres = PRSNT_NODEF;
     nrFreqCfg->t.eutranFreq.reselecThreshQRel9.val =
        wrSmDfltReselecThreshQRel9;
     nrFreqCfg->t.eutranFreq.threshXQRel9.threshXHighQR9 =
        wrSmDfltThreshXHighQR9;
     nrFreqCfg->t.eutranFreq.threshXQRel9.threshXLowQR9 =
        wrSmDfltThreshXLowQR9;

     nrFreqCfg++;
   }
   RETVALUE(ROK);
} /* end of smBuildWrNrEutraFreqCfg */

PRIVATE S16 smUtlGetNumInterFreqNghCellCfg
(
)
{
   S32   lpCnt;
   S16   numCells = 0;

   TRC2(smUtlGetNumInterFreqNghCellCfg);

   for (lpCnt = 0; lpCnt < smCfgCb.noOfNghCfg; lpCnt++)
   {
      /* not including intra - frequency neighbors */
      if(smCfgCb.wrNghCellCfg[lpCnt]->dlEarfcn !=
            wrSmDfltServingCellDLCarrierFreq)
      {
         numCells++;
      }
   }
   RETVALUE(numCells);
} /* end of smUtlGetNumInterFreqNghCellCfg */


/*
*
*       Fun:   smBuildWrInterFreqNehCellCfg
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
PUBLIC S16 smBuildWrInterFreqNehCellCfg
(
LwrMngmt *lwrMng
)
#else /* if ANSI not defined*/
PUBLIC S16 smBuildWrInterFreqNehCellCfg(*lwrMng)
LwrMngmt *lwrMng;
#endif /* end of ANSI*/
{
   LwrEUTRANeighCell *eutraNeghCell = NULLP;
   S32   lpCnt = 0;
   S32   cfgIndx = 0;
   S32   numCfg = 0;
   
   TRC2(smBuildWrInterFreqNehCellCfg);

   lwrMng->t.protoCfg.type = WR_CFG_TYPE_NEIGH_CELL;
   lwrMng->t.protoCfg.t.neighCellCfg.action = WR_ACT_ADD;
   lwrMng->t.protoCfg.t.neighCellCfg.cellId = 1; /* this is ownCellID in case of PLD based, jst a logic entity */
   numCfg = smUtlGetNumInterFreqNghCellCfg();
   lwrMng->t.protoCfg.t.neighCellCfg.numCells = (U16)numCfg;

   for (lpCnt = 0; lpCnt < smCfgCb.noOfNghCfg; lpCnt++)
   {
      /* not including intra - frequency neighbors */
      if(smCfgCb.wrNghCellCfg[lpCnt]->dlEarfcn ==
            wrSmDfltServingCellDLCarrierFreq)
      {
         continue;
      }
      lwrMng->t.protoCfg.t.neighCellCfg.t.neighAddCfg[cfgIndx].type =
         WR_NEIGH_CELL_EUTRA;
      eutraNeghCell =&(lwrMng->t.protoCfg.t.neighCellCfg.t.
            neighAddCfg[cfgIndx].t.eutraCell);

      eutraNeghCell->cellNum = wrSmDfltNeigCellNum[0];
      eutraNeghCell->nrIndex = 4;
      eutraNeghCell->enbId = smCfgCb.wrNghCellCfg[lpCnt]->enbId;
   
      eutraNeghCell->cio =wrSmDfltCIO;
       eutraNeghCell->enbPlmn.numMncDigits = wrSmDfltNeEnbNumMncDigits;
      eutraNeghCell->enbPlmn.mcc[0] =wrSmDfltNeEnbPlmn1Mcc0[1];
      eutraNeghCell->enbPlmn.mcc[1] =wrSmDfltNeEnbPlmn1Mcc1[1];
      eutraNeghCell->enbPlmn.mcc[2] =wrSmDfltNeEnbPlmn1Mcc2[1];

      eutraNeghCell->enbPlmn.mnc[0] =wrSmDfltNeEnbPlmn1Mnc0[1];
      eutraNeghCell->enbPlmn.mnc[1] =wrSmDfltNeEnbPlmn1Mnc1[1];
      eutraNeghCell->enbPlmn.mnc[2] =wrSmDfltNeEnbPlmn1Mnc2[1];

      eutraNeghCell->enbType = smCfgCb.wrNghCellCfg[lpCnt]->enbType;
      eutraNeghCell->numPlmn = wrSmDfltNeighCellNoPlmn;

      eutraNeghCell->pci = smCfgCb.wrNghCellCfg[lpCnt]->physCellId;
      eutraNeghCell->tac = smCfgCb.wrNghCellCfg[lpCnt]->tac;

      eutraNeghCell->qoffset = wrSmDfltQoff;

   eutraNeghCell->dlBw = wrSmDfltCellDlBW;
   eutraNeghCell->earfcnDl = (U16)(smCfgCb.wrNghCellCfg[lpCnt]->dlEarfcn);
   eutraNeghCell->earfcnUl= (U16)(smCfgCb.wrNghCellCfg[lpCnt]->ulEarfcn);
   cfgIndx++;
   }
  
   RETVALUE(ROK);
} /* end of smBuildWrInterFreqNehCellCfg */


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
   LwrEUTRANeighCell *eutraNeghCell = NULLP;
   U32 idx; 
   U32 index1 = 0;
   //U8 nghPlmn[6],noMnc;
   /* S16 numCells = 0;*/

   TRC2(smBuildWrNehCellCfg);

   lwrMng->t.protoCfg.type = WR_CFG_TYPE_NEIGH_CELL;
   lwrMng->t.protoCfg.t.neighCellCfg.action = WR_ACT_ADD;
   lwrMng->t.protoCfg.t.neighCellCfg.cellId = 1; /* this is ownCellID in case of PLD, jst a logical entity */
   /*   numCells = smUtlGetNumIntraFreqNghCellCfg();*/
   lwrMng->t.protoCfg.t.neighCellCfg.numCells = smCfgCb.noOfNghCfg;
   /*
      lwrMng->t.protoCfg.t.neighCellCfg.numCells = numCells;
      */

   for(idx = 0; idx < smCfgCb.noOfNghCfg; idx++)
   {
      /* not including inter - frequency neighbors */
      /*
         if(smCfgCb.wrNghCellCfg[idx]->dlEarfcn != wrSmDfltServingCellDLCarrierFreq)
         {
         continue;
         }
         */
      lwrMng->t.protoCfg.t.neighCellCfg.t.neighAddCfg[idx].type =
         WR_NEIGH_CELL_EUTRA;
      eutraNeghCell =&(lwrMng->t.protoCfg.t.neighCellCfg.t.neighAddCfg[idx].
            t.eutraCell);
      eutraNeghCell->cellNum = wrSmDfltNeigCellNum[idx];
      eutraNeghCell->nrIndex = wrSmDfltNriIndex[idx];
      eutraNeghCell->enbId = smCfgCb.wrNghCellCfg[idx]->enbId;

      eutraNeghCell->cio =wrSmDfltCIO;
      eutraNeghCell->enbPlmn.numMncDigits = smCfgCb.wrNghCellCfg[idx]->noMnc;
      eutraNeghCell->enbPlmn.mcc[0] = smCfgCb.wrNghCellCfg[idx]->nghPlmn[0];
      eutraNeghCell->enbPlmn.mcc[1] = smCfgCb.wrNghCellCfg[idx]->nghPlmn[1];
      eutraNeghCell->enbPlmn.mcc[2] = smCfgCb.wrNghCellCfg[idx]->nghPlmn[2];
      eutraNeghCell->enbPlmn.mnc[0] = smCfgCb.wrNghCellCfg[idx]->nghPlmn[3];
      eutraNeghCell->enbPlmn.mnc[1] = smCfgCb.wrNghCellCfg[idx]->nghPlmn[4];
      eutraNeghCell->enbPlmn.mnc[2] = smCfgCb.wrNghCellCfg[idx]->nghPlmn[5];

      eutraNeghCell->enbType = smCfgCb.wrNghCellCfg[idx]->enbType;
      eutraNeghCell->numPlmn = wrSmDfltNeighCellNoPlmn;

      eutraNeghCell->pci = smCfgCb.wrNghCellCfg[idx]->physCellId;
      eutraNeghCell->tac = smCfgCb.wrNghCellCfg[idx]->tac;

      eutraNeghCell->qoffset = wrSmDfltQoff;

      eutraNeghCell->dlBw = wrSmDfltCellDlBW;
      eutraNeghCell->earfcnDl = (U16)(smCfgCb.wrNghCellCfg[idx]->dlEarfcn);
      eutraNeghCell->earfcnUl= (U16)(smCfgCb.wrNghCellCfg[idx]->ulEarfcn);

      /* CSG_DEV start */
      eutraNeghCell->isCsgIdResolved = WR_EMM_NBR_CSG_ID_DISCOVERED;
      eutraNeghCell->isCsgAccessModeResolved = WR_EMM_CELL_ACCESS_MODE_DETERMINED;
      eutraNeghCell->csgAccessMode = smCfgCb.wrNghCellCfg[idx]->csgAccessMode;
      eutraNeghCell->csgId = smCfgCb.wrNghCellCfg[idx]->csgId;
      /* CSG_DEV end */

      /*  eutraNeghCell++;*/
      /* CNM_DEV start */
      eutraNeghCell->numAntenna = (U16)(smCfgCb.wrNghCellCfg[idx]->numAntenna);
      eutraNeghCell->ulDlSubFrameCfg = (U16)(smCfgCb.wrNghCellCfg[idx]->ulDlSubFrameCfg);
      eutraNeghCell->splSfCfg = (U16)(smCfgCb.wrNghCellCfg[idx]->splSfCfg);
      eutraNeghCell->cpDl = (U16)(smCfgCb.wrNghCellCfg[idx]->cpDl);
      eutraNeghCell->cpUl = (U16)(smCfgCb.wrNghCellCfg[idx]->cpUl);
      eutraNeghCell->rsrp = (U16)(smCfgCb.wrNghCellCfg[idx]->rsrp);
      eutraNeghCell->rssi = (U16)(smCfgCb.wrNghCellCfg[idx]->rssi);
      eutraNeghCell->isHeMsCell = TRUE;
      /* CNM_DEV end */
      index1++;
   }   

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
   U8 idx; 
   TRC2(smBuildWrMmeCfg);

   lwrMng->t.protoCfg.type = WR_CFG_TYPE_MME;
   lwrMng->t.protoCfg.t.mmeCfg.action = WR_ACT_ADD;

   mmeCfg =&(lwrMng->t.protoCfg.t.mmeCfg.t.addMme);

   mmeCfg->mmeIndex = (U8)(mmeSmCfg->mmeId);

   /* Filling IP Addresses */
   mmeCfg->noOfIps = mmeSmCfg->noOfIps;
   for(idx = 0; idx < mmeSmCfg->noOfIps; idx++)
   {
      wrCpyCmTptAddr(&mmeCfg->ipAddr[idx], &mmeSmCfg->mmeAddr[idx]);
      if(CM_TPTADDR_IPV4 ==  mmeCfg->ipAddr[idx].type)
      {
         mmeCfg->ipAddr[idx].u.ipv4TptAddr.port = wrSmDfltMmePort;
      }
      else if(CM_TPTADDR_IPV6 ==  mmeCfg->ipAddr[idx].type)
      {
         mmeCfg->ipAddr[idx].u.ipv6TptAddr.port = wrSmDfltMmePort;
      }
   }
   /*Filling MME state*/
   mmeCfg->mmeState =wrSmDfltMmeState;
   mmeCfg->adminState =wrSmDfltMmeAdminState;
   
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
   LwrNeighAddModCfg *neighCfg = NULLP;
   U32 idx = 0;
   U32 enbId = 0;
   U32 indx2 = 0;
   
   TRC2(smBuildWrNghCfg);

   lwrMng->t.protoCfg.type = WR_CFG_TYPE_NEIGH;
   lwrMng->t.protoCfg.t.neighCfg.action = WR_ACT_ADD;
   lwrMng->t.protoCfg.t.neighCfg.cellId = 1; /* this ownCellID in case of PLD based, jst a logic entity */
   lwrMng->t.protoCfg.t.neighCfg.numEnb = smCfgCb.noOfNghCfg;
   
   neighCfg = (lwrMng->t.protoCfg.t.neighCfg.t.neighAdd);
   for(idx = 0; idx < smCfgCb.noOfNghCfg; idx++)
   {
      enbId = smCfgCb.wrNghCellCfg[idx]->enbId;
      for(indx2 = 0; indx2 < idx; indx2++)
      {
         /* checking if the eNB is already add to configuration */
         /* if yes, ignoring the dulicate configuration and proceeding */
         /* this may occur when multiple cells or same eNB is configured */
         if(enbId == smCfgCb.wrNghCellCfg[indx2]->enbId)
         {
            continue;
         }
      }
      neighCfg->enbType = smCfgCb.wrNghCellCfg[idx]->enbType;
      if(neighCfg->enbType == 0)
      {
         neighCfg->enbId = (enbId >> 8);
      }
      else
      {
      neighCfg->enbId = enbId;
      }
      
      neighCfg->enbIndx = wrSmDfltNeighEnbIndex[idx];
      neighCfg->peerId = smCfgCb.wrNghCellCfg[idx]->peerId;
      
      neighCfg->plmn.numMncDigits=wrSmDfltNeEnbNumMncDigits;
      neighCfg->plmn.mcc[0]=wrSmDfltNeEnbPlmn1Mcc0[idx];
      neighCfg->plmn.mcc[1]=wrSmDfltNeEnbPlmn1Mcc1[idx];
      neighCfg->plmn.mcc[2]=wrSmDfltNeEnbPlmn1Mcc2[idx];

      neighCfg->plmn.mnc[0] =wrSmDfltNeEnbPlmn1Mnc0[idx];
      neighCfg->plmn.mnc[1] =wrSmDfltNeEnbPlmn1Mnc1[idx];
      neighCfg->plmn.mnc[2] =wrSmDfltNeEnbPlmn1Mnc2[idx];

      wrCpyCmTptAddr(&neighCfg->ipAddr, &smCfgCb.wrNghCellCfg[idx]->enbAddr);
      if(CM_TPTADDR_IPV4 == neighCfg->ipAddr.type)
      {
         neighCfg->ipAddr.u.ipv4TptAddr.port = WRSM_LCZ_ENB_SCTPORT;
      }
      else  if(CM_TPTADDR_IPV6 == neighCfg->ipAddr.type)
      {
         neighCfg->ipAddr.u.ipv6TptAddr.port = WRSM_LCZ_ENB_SCTPORT;
      }

      
/* LTE_ADV_DSFR starts */
      neighCfg->dsfrCEFreqRange = smCfgCb.wrNghCellCfg[0]->dsfrCEFreqRange;
/* LTE_ADV_DSFR ends */
      neighCfg->state = wrSmDfltNeEnbState;
      if ( ( neighCfg->enbType == WR_ENODEB_TYPE_MACRO ) &&   
            ((smCfgCb.wrNghCellCfg[idx]->trgrX2 == WR_X2_CON_ENBL_ALL ) ||
             (smCfgCb.wrNghCellCfg[idx]->trgrX2 == WR_X2_CON_ENBL_MACRO )))
      {
         neighCfg->isX2Reqd = TRUE;
      }
      else if ( ( neighCfg->enbType == WR_ENODEB_TYPE_HOME) &&   
            ((smCfgCb.wrNghCellCfg[idx]->trgrX2 == WR_X2_CON_ENBL_ALL ) ||
             (smCfgCb.wrNghCellCfg[idx]->trgrX2 == WR_X2_CON_ENBL_FEMTO )))
      {
         neighCfg->isX2Reqd = TRUE;
      }
      else
      {
         neighCfg->isX2Reqd = FALSE;
      }
      neighCfg++;
   }   
   RETVALUE(ROK);
} /* end of smBuildWrNghCfg */ 

 
PUBLIC void smBldCellAnrCfg
(
 LwrMngmt   *lwrMng,
 U8         cfgGrpIndex
)
{
   LwrCellAnrCfgGrp          *anrCfg;
   TRC2(smBldCellAnrCfg);

   anrCfg = &lwrMng->t.protoCfg.t.cellCfg.t.addCell.cellCfgGrp[cfgGrpIndex]->t.anrCfgGrp;

   lwrMng->t.protoCfg.t.cellCfg.t.addCell.cellCfgGrp[cfgGrpIndex]->grpType =
                                                     WR_CELL_CFG_GRP_ANR;
   anrCfg->csfbTransTmrVal = wrSmDfltCsfbTmrVal;

   wrFillTknU32(&(anrCfg->epocTimerVal), wrSmDfltEpocTmrVal);

   anrCfg->tricePeriodIntv    = wrSmDfltTricePrdIntv;
   anrCfg->anrCellValidAge    = wrSmDfltAnrCelValidAge;

   /*Removed DRX Flag */
   anrCfg->ueAnrDrxCfg.longDrxCycle = wrSmDfltAnrDrxLongCycle;  
   anrCfg->ueAnrDrxCfg.drxOnDurTmr  = wrSmDfltAnrDrxOnDurationTmr;
   /* Fix for ccpu00132334 - To read DRX Inactivity timer for ANR*/
   anrCfg->ueAnrDrxCfg.inActivTmr  = wrSmDfltAnrDrxInActivTmr; 
   RETVOID;

} /* end of smBldCellAnrCfg */

PRIVATE void smBldCellRabConfigParam
(
 LwrMngmt   *lwrMng,
 U8         cfgGrpIndex
)
{
   U16                idx;
   LwrRabPolicyCfgGrp *rabPolicyCfg = NULLP;

   lwrMng->t.protoCfg.t.cellCfg.t.addCell.cellCfgGrp[cfgGrpIndex]->grpType =
                                                     WR_CELL_CFG_GRP_RAB_PARAMS;
   
   rabPolicyCfg= &(lwrMng->t.protoCfg.t.cellCfg.t.addCell.cellCfgGrp[cfgGrpIndex]->t.rabPolicyCfgGrp);
   rabPolicyCfg->numDrbPolicies=WR_MAX_QCI;
   rabPolicyCfg->arpForEmergencyServ = wrSmDfltArpForEmerServ;
   for(idx=0;idx<rabPolicyCfg->numDrbPolicies;idx++)
   {
       if(smCfgCb.isDscpEnable==TRUE)
       {
          rabPolicyCfg->drbPolicy[idx].qciInfo.qciSupported=TRUE;
          rabPolicyCfg->drbPolicy[idx].qci=(idx+1);
          rabPolicyCfg->drbPolicy[idx].dscpInfo.qci=(idx+1);
          rabPolicyCfg->drbPolicy[idx].dscpInfo.dscp=smCfgCb.qciDscpMap[idx];
       }
       else
       {
          rabPolicyCfg->drbPolicy[idx].qciInfo.qciSupported=TRUE;
          rabPolicyCfg->drbPolicy[idx].qci=(idx+1);
          rabPolicyCfg->drbPolicy[idx].dscpInfo.qci=(idx+1);
          rabPolicyCfg->drbPolicy[idx].dscpInfo.dscp=0;
       } 
   }
   rabPolicyCfg->drbPolicy[0].rlcInfo.rlcMode = WR_RLC_UM_MODE;
   rabPolicyCfg->drbPolicy[0].rlcInfo.ulSnFieldLength = NhuSN_FieldLengthsize5Enum;
   rabPolicyCfg->drbPolicy[0].rlcInfo.dlSnFieldLength = NhuSN_FieldLengthsize10Enum;
   rabPolicyCfg->drbPolicy[0].rlcInfo.ulTimerReordering = wrSmDfltUlDrbtReordring;
   rabPolicyCfg->drbPolicy[0].rlcInfo.dlTimerReordering = wrSmDfltDlDrbtReordring;
   rabPolicyCfg->drbPolicy[0].pdcpInfo.umSnSize = NhuPDCP_Configrlc_UMpdcp_SN_Sizelen7bitsEnum;
   rabPolicyCfg->drbPolicy[0].pdcpInfo.discardTimer = NhuPDCP_ConfigdiscardTimerinfinityEnum;
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
   rabPolicyCfg->drbPolicy[1].pdcpInfo.discardTimer = NhuPDCP_ConfigdiscardTimerinfinityEnum;
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
   rabPolicyCfg->drbPolicy[2].pdcpInfo.discardTimer = NhuPDCP_ConfigdiscardTimerinfinityEnum;
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
   rabPolicyCfg->drbPolicy[3].pdcpInfo.discardTimer = NhuPDCP_ConfigdiscardTimerinfinityEnum;
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

} /* end of smBldCellRabConfigParam */
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

   sibSchCfg = &(lwrMng->t.protoCfg.t.cellCfg.t.addCell.
         cellCfgGrp[cfgGrpIndex]->t.sibSchedCfgGrp);
   /* Populate the SIB periodicites */
   sibSchCfg->sib2Period = WR_SIB2_PERIOD; 
   sibSchCfg->sib3Period = WR_SIB3_PERIOD;
   sibSchCfg->sib4Period = WR_SIB4_PERIOD; 
   /* If the neighbour Frequency is only one, it can be either intra or inter frequncy Hence
    * need to validae the neighbour frequency is matched with serving cell
    * frequency or not 
    */
   sibSchCfg->sib5Period = WR_SIB5_PERIOD; 

   if ((smCfgCb.noOfUtraFddFreq > 0) || (smCfgCb.noOfUtraTddFreq > 0))
   {
      sibSchCfg->sib6PeriodValid = TRUE;   
      sibSchCfg->sib6Period = WR_SIB6_PERIOD; 
   }
   else
   {
      sibSchCfg->sib6PeriodValid = FALSE;
   }
   if (smCfgCb.noOfGeranFreq > 0)
   {
      sibSchCfg->sib7PeriodValid = TRUE;   
      sibSchCfg->sib7Period = WR_SIB7_PERIOD; 
   }
   else
   {
      sibSchCfg->sib7PeriodValid = FALSE;
   }
   sibSchCfg->sib9Period = WR_SIB9_PERIOD; 
   if (smCfgCb.noOfNeighCdma1xCells > 0)
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

   sib8Cfg->cdmaEutraTimeSync = wrSmDfltSib8CdmaEutraTimeSync;
   sib8Cfg->sfMedium = wrSmDfltSib8SfMedium;
   sib8Cfg->sfHigh = wrSmDfltSib8SfHigh;
   sib8Cfg->tReselection = wrSmDfltSib8TReselection;

   sib8Cfg->cdma1xrttParams.cdma1xrttRegParams.pres = wrSmDfltCdma1xrttRegSup;/*RRC Rel 10 Upgrade*/
   /*wrSmDfltCdma1xrttRegSup is set to TRUE by default. configurability to be supported
	 in the future*/
   sib8Cfg->cdma1xrttParams.cdma1xrttRegParams.sid = wrSmDfltCdma1xRegParamsSid;
   sib8Cfg->cdma1xrttParams.cdma1xrttRegParams.nid = wrSmDfltCdma1xRegParamsNid;
   sib8Cfg->cdma1xrttParams.cdma1xrttRegParams.mulSID = wrSmDfltCdma1xRegParamsMulSID;
   sib8Cfg->cdma1xrttParams.cdma1xrttRegParams.mulNID = wrSmDfltCdma1xRegParamsMulNID;
   sib8Cfg->cdma1xrttParams.cdma1xrttRegParams.homeReg = wrSmDfltCdma1xRegParamsHomeReg;
   sib8Cfg->cdma1xrttParams.cdma1xrttRegParams.foreignSIDReg = 
      wrSmDfltCdma1xRegParamsForeignSIDReg;
   sib8Cfg->cdma1xrttParams.cdma1xrttRegParams.foreignNIDReg = 
      wrSmDfltCdma1xRegParamsForeignNIDReg;
   sib8Cfg->cdma1xrttParams.cdma1xrttRegParams.paramReg = wrSmDfltCdma1xRegParamsParamReg;
   sib8Cfg->cdma1xrttParams.cdma1xrttRegParams.powerUpReg = wrSmDfltCdma1xRegParamsPowerUpReg;
   sib8Cfg->cdma1xrttParams.cdma1xrttRegParams.regPeriod = wrSmDfltCdma1xRegParamsRegPeriod;
   sib8Cfg->cdma1xrttParams.cdma1xrttRegParams.regZone = wrSmDfltCdma1xRegParamsRegZone;
   sib8Cfg->cdma1xrttParams.cdma1xrttRegParams.totalZone = wrSmDfltCdma1xRegParamsTotalZone;
   sib8Cfg->cdma1xrttParams.cdma1xrttRegParams.zoneTimer = wrSmDfltCdma1xRegParamsZoneTimer;
   sib8Cfg->cdma1xrttParams.cdma1xrttRegParams.powerDownRegPres = 
      wrSmDfltCdma1xRegParamsPowerDownReg;

   sib8Cfg->cdma1xrttParams.cdma1xrttACBarringCfg.pres = PRSNT_NODEF; 
   sib8Cfg->cdma1xrttParams.cdma1xrttACBarringCfg.acBarring0to9_r9 = 
      wrSmDfltCdma1xAcBarring0to9;
   sib8Cfg->cdma1xrttParams.cdma1xrttACBarringCfg.acBarring10_r9 = wrSmDfltCdma1xAcBarring10;
   sib8Cfg->cdma1xrttParams.cdma1xrttACBarringCfg.acBarring11_r9 = wrSmDfltCdma1xAcBarring11;
   sib8Cfg->cdma1xrttParams.cdma1xrttACBarringCfg.acBarring12_r9 = wrSmDfltCdma1xAcBarring12;
   sib8Cfg->cdma1xrttParams.cdma1xrttACBarringCfg.acBarring13_r9 = wrSmDfltCdma1xAcBarring13;
   sib8Cfg->cdma1xrttParams.cdma1xrttACBarringCfg.acBarring14_r9 = wrSmDfltCdma1xAcBarring14;
   sib8Cfg->cdma1xrttParams.cdma1xrttACBarringCfg.acBarring15_r9 = wrSmDfltCdma1xAcBarring15;
   sib8Cfg->cdma1xrttParams.cdma1xrttACBarringCfg.acBarringMsg_r9 = wrSmDfltCdma1xAcBarringMsg;
   sib8Cfg->cdma1xrttParams.cdma1xrttACBarringCfg.acBarringReg_r9 = wrSmDfltCdma1xAcBarringReg;
   sib8Cfg->cdma1xrttParams.cdma1xrttACBarringCfg.acBarringEmg_r9 = wrSmDfltCdma1xAcBarringEmg;

} /* end of smBldSib8Cfg */

PRIVATE S16 smBldCsfbCfg
(
 LwrMngmt   *lwrMng,
 U8         cfgGrpIndex
)
{
   LwrCsfbCfgGrp *csfbCfg = NULLP;

   lwrMng->t.protoCfg.t.cellCfg.t.addCell.cellCfgGrp[cfgGrpIndex]->grpType =
                                                     WR_CELL_CFG_GRP_CSFB;
   
   csfbCfg = &(lwrMng->t.protoCfg.t.cellCfg.t.addCell.cellCfgGrp[cfgGrpIndex]->
                  t.csfbCfgGrp);

   csfbCfg->csfbToUtraMeasSup = wrSmDfltCsfbToUtraMeasSup;
   csfbCfg->csfbToCdmaMeasSup = wrSmDfltCsfbToCdmaMeasSup;
   csfbCfg->eCsfbMeasSup = wrSmDfltEcsfbMeasSup;
   csfbCfg->dualRxSup = wrSmDfltDualRxSup;
/*RRC Rel 10 Upgrade - start*/
   if (TRUE == wrSmDfltDualRxTxSup)
   {
   		if(LWR_ECSFB_NOT_SUPPORTED == csfbCfg->eCsfbMeasSup) 
   		{
            RLOG2(L_ERROR, "Invalid Cfg for paramter DUAL_RX_TX_SUPPORTED:\
            eCSFB should be supported(currently=%d) inorder to support\
			DualRxTx Cfg(%d)",wrSmDfltEcsfbMeasSup,wrSmDfltDualRxTxSup);
			
			RETVALUE(RFAILED);
		}
   }
   csfbCfg->dualRxTxSup = wrSmDfltDualRxTxSup;
   csfbCfg->cdma1xRttSup = wrSmDfltCdma1xrttRegSup;
/*RRC Rel 10 Upgrade - end*/
   csfbCfg->cdma1xrttCsfbCfg.secId.cdmaType = LWR_1XRTT;

   csfbCfg->cdma1xrttCsfbCfg.secId.isOctStrPres = FALSE;

   if(FALSE == csfbCfg->cdma1xrttCsfbCfg.secId.isOctStrPres)
   {
      csfbCfg->cdma1xrttCsfbCfg.secId.s.cdmaSectorId.marketId = 
         wrSmDfltCdmaMktId;
      csfbCfg->cdma1xrttCsfbCfg.secId.s.cdmaSectorId.switchNum = 
         wrSmDfltCdmaSwitchNum;
      csfbCfg->cdma1xrttCsfbCfg.secId.s.cdmaSectorId.cellId = 
         wrSmDfltCdmaCellId;
      csfbCfg->cdma1xrttCsfbCfg.secId.s.cdmaSectorId.sectorNum = 
         wrSmDfltCdmaSectorNum;
   }
   else
   {
      wrFillSectorId(&csfbCfg->cdma1xrttCsfbCfg.secId.s.sectorIdPtr, 7,
            (Data *)&wrSmDfltSecId);
   }
   smBuildMobParams(&csfbCfg->cdma1xrttCsfbCfg.mobParams);

   RETVALUE(ROK);
} /* end of smBldCsfbCfg */


PUBLIC void smBldCellEaidCfg
(
 LwrMngmt   *lwrMng,
 U8         cfgGrpIndex
)
{
   LwrCellEAIDCfgGrp         *eaidCfgGrp = NULLP;
   U8                        idx = 0; 
   U32                       tempEaid = 0x112233;

   TRC2(smBldCellEaidCfg);

   lwrMng->t.protoCfg.t.cellCfg.t.addCell.cellCfgGrp[cfgGrpIndex]->grpType =
      WR_CELL_CFG_GRP_EAID;

   eaidCfgGrp = &(lwrMng->t.protoCfg.t.cellCfg.t.addCell.
         cellCfgGrp[cfgGrpIndex]->t.eaidCfgGrp);

   for (idx = 0; idx < LWR_SM_MAX_EAIDS; idx++)
   {
      eaidCfgGrp->eaid[idx].pres = TRUE;
      eaidCfgGrp->eaid[idx].val = tempEaid++;
   }
   RETVOID;
} /* end of smBldCellEaidCfG */

PUBLIC Void smBldMeasCellCfg
(
 LwrMngmt   *lwrMng,
 U8         cfgGrpIndex
)
{
   LwrCellMeasCfgGrp         *measCfg = NULLP;
   LwrMeasEvntA5RptCfg       *a5Cfg;
   LwrMeasEvntA3RptCfg       *a3Cfg;
   LwrMeasEvntRptCfg         *eventCfg;
   LwrMeasEutraPeridRptCfg   *eutraPerd;
   LwrMeasUtraPeriodRptCfg   *utraPerd;
   LwrMeasUtraEvntB2RptCfg   *utraB2Cfg;
   LwrMeasGERANPeridRptCfg   *geranPrdCfg;
   LwrMeasGeranEvntB2RptCfg   *geranEvtB2Rpt;

   TRC2(smBldMeasCellCfg);

   lwrMng->t.protoCfg.t.cellCfg.t.addCell.cellCfgGrp[cfgGrpIndex]->grpType =
      WR_CELL_CFG_GRP_MEAS;

   measCfg = &(lwrMng->t.protoCfg.t.cellCfg.t.addCell.
         cellCfgGrp[cfgGrpIndex]->t.measCfgGrp);
   eventCfg = &measCfg->measA1EvtRptCfg[WR_MEAS_EVENTA1_HO_RSRP];

   /* configure Event A1 */
  eventCfg->isPres.pres = PRSNT_NODEF;
  eventCfg->reason = wrSmDfltReason;
  eventCfg->threshold_RSRP = wrSmDfltA1ThresRSRP;
  eventCfg->threshold_RSRQ = wrSmDfltThresRSRQ1;
  eventCfg->hysteresis = wrSmDfltHysteresis;
  eventCfg->timeToTrigger = wrSmDfltTimeToTrig;
  eventCfg->triggerQty = wrSmDfltTrigQty;
  eventCfg->reportQty = wrSmDfltRptQty;
  eventCfg->maxRptCells = wrSmDfltMaxRptCells;
  eventCfg->reportInterval = wrSmDfltRptInterval;
  eventCfg->reportAmount = wrSmDfltRptAmount;

   /* configure Event A2 */
  eventCfg = &measCfg->measA2EvtRptCfg[WR_MEAS_EVENTA2_HO_RSRP];

  eventCfg->isPres.pres = PRSNT_NODEF;
  eventCfg->reason = wrSmDfltReason;
  eventCfg->threshold_RSRP = wrSmDfltA2ThresRSRP;
  eventCfg->threshold_RSRQ = wrSmDfltThresRSRQ1;
  eventCfg->hysteresis = wrSmDfltHysteresis;
  eventCfg->timeToTrigger = wrSmDfltTimeToTrig;
  eventCfg->triggerQty = wrSmDfltTrigQty;
  eventCfg->reportQty = wrSmDfltRptQty;
  eventCfg->maxRptCells = wrSmDfltMaxRptCells;
  eventCfg->reportInterval = wrSmDfltRptInterval;
  eventCfg->reportAmount = wrSmDfltRptAmount;

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

   /* configure Event A3 */
   a3Cfg = &measCfg->measEvtA3Cfg[WR_MEAS_EVENTA3_INTRA_FREQ_HO];
   a3Cfg->isPres.pres = PRSNT_NODEF;
   a3Cfg->reason = wrSmDfltReason;

   /* Offset Range is between -30 to 30*/
   if(wrSmDfltIntraA3HoOffset > 31)
   {
      wrSmDfltIntraA3HoOffset = 30;
   }
   else if (wrSmDfltIntraA3HoOffset < -31)
   {
      wrSmDfltIntraA3HoOffset = -30;
   }

   a3Cfg->offset = wrSmDfltIntraA3HoOffset;
   a3Cfg->reportOnLeave = wrSmDfltRptOnLeave;
   a3Cfg->hysteresis = wrSmDfltHysteresis;
   a3Cfg->timeToTrigger = wrSmDfltTimeToTrig;
   a3Cfg->triggerQty = wrSmDfltTrigQty;
   a3Cfg->reportQty = wrSmDfltRptQty;
   a3Cfg->maxRptCells = wrSmDfltMaxRptCells;
   a3Cfg->reportInterval = wrSmDfltRptInterval;
   a3Cfg->reportAmount = wrSmDfltRptAmount;

/* LTE_ADV_ABS starts */
   measCfg->measEvtA3Cfg[WR_MEAS_EVENTA3_INTRA_FREQ_ABS].isPres.pres = PRSNT_NODEF;
   measCfg->measEvtA3Cfg[WR_MEAS_EVENTA3_INTRA_FREQ_ABS].reason = wrSmDfltReason;
   measCfg->measEvtA3Cfg[WR_MEAS_EVENTA3_INTRA_FREQ_ABS].offset = 0; /* Report all neighbor stronger than serving */
   measCfg->measEvtA3Cfg[WR_MEAS_EVENTA3_INTRA_FREQ_ABS].reportOnLeave = FALSE;
   measCfg->measEvtA3Cfg[WR_MEAS_EVENTA3_INTRA_FREQ_ABS].hysteresis = wrSmDfltHysteresis;
   measCfg->measEvtA3Cfg[WR_MEAS_EVENTA3_INTRA_FREQ_ABS].timeToTrigger = wrSmDfltTimeToTrig;
   measCfg->measEvtA3Cfg[WR_MEAS_EVENTA3_INTRA_FREQ_ABS].triggerQty = wrSmDfltTrigQty;
   measCfg->measEvtA3Cfg[WR_MEAS_EVENTA3_INTRA_FREQ_ABS].reportQty = wrSmDfltRptQty;
   measCfg->measEvtA3Cfg[WR_MEAS_EVENTA3_INTRA_FREQ_ABS].maxRptCells = wrSmDfltMaxRptCells;
   measCfg->measEvtA3Cfg[WR_MEAS_EVENTA3_INTRA_FREQ_ABS].reportInterval = wrSmDfltRptInterval;
   measCfg->measEvtA3Cfg[WR_MEAS_EVENTA3_INTRA_FREQ_ABS].reportAmount = 0; /* Report only once */
   measCfg->measEvtA3Cfg[WR_MEAS_EVENTA3_INTRA_FREQ_ABS].offset = smCfgCb.lteAdvancedConfig.a3Offset; /* ABS Offset */
/* LTE_ADV_ABS ends */

   /*Event A3 for ANR */
   a3Cfg = &measCfg->measEvtA3Cfg[WR_MEAS_EVENTA3_INTRA_FREQ_ANR];
   a3Cfg->isPres.pres = PRSNT_NODEF;
   a3Cfg->reason = wrSmDfltReason;

   /* Offset Range is between
    * -30 to 30*/
   if(wrSmDfltA3AnrOffset > wrSmDfltIntraA3HoOffset)
   {
      wrSmDfltA3AnrOffset = wrSmDfltIntraA3HoOffset - 5;
   }
   else if (wrSmDfltA3AnrOffset < -31)
   {
      wrSmDfltA3AnrOffset = -30;
   }
   a3Cfg->offset = wrSmDfltA3AnrOffset;

   a3Cfg->reportOnLeave = wrSmDfltRptOnLeave;
   a3Cfg->hysteresis = wrSmDfltHysteresis;
   a3Cfg->timeToTrigger = wrSmDfltTimeToTrig;
   a3Cfg->triggerQty = wrSmDfltTrigQty;
   a3Cfg->reportQty = wrSmDfltRptQty;
   a3Cfg->maxRptCells = wrSmDfltMaxRptCells;
   a3Cfg->reportInterval = wrSmDfltRptInterval;
   a3Cfg->reportAmount = wrSmDfltRptAmount;

   /* configure Event A5 */
   a5Cfg = &measCfg->measEvtA5Cfg[WR_MEAS_EVENTA5_INTRA_FREQ_HO];
   /* configure Intra Event A5 */
   a5Cfg->isPres.pres = PRSNT_NODEF;
   a5Cfg->reason = wrSmDfltReason;
   a5Cfg->threshold1_RSRP = wrSmDfltIntraA5ThresRSRP1;
   a5Cfg->threshold1_RSRQ = wrSmDfltThresRSRQ1;
   a5Cfg->threshold2_RSRP = wrSmDfltIntraA5ThresRSRP2;
   a5Cfg->threshold2_RSRQ = wrSmDfltThresRSRQ2;
   a5Cfg->hysteresis = wrSmDfltHysteresis;
   a5Cfg->timeToTrigger = wrSmDfltTimeToTrig;
   a5Cfg->triggerQty = wrSmDfltTrigQty;
   a5Cfg->reportQty = wrSmDfltRptQty;
   a5Cfg->maxRptCells = wrSmDfltMaxRptCells;
   a5Cfg->reportInterval = wrSmDfltRptInterval;
   a5Cfg->reportAmount = wrSmDfltRptAmount;

   a5Cfg = &measCfg->measEvtA5Cfg[WR_MEAS_EVENTA5_INTER_FREQ_HO_RSRP];
   /* configure Inter Event A5 */
   a5Cfg->isPres.pres = PRSNT_NODEF;
   a5Cfg->reason = wrSmDfltReason;
   /* if RSRP configured is out range (which is defined in RRC)
    * will be defaulted nearest value with in the range 
    * Here check spec defined range only */
   if (wrSmDfltInterA5ThresRSRP1 > 97) /* RSRP Max range check */
   {
      wrSmDfltInterA5ThresRSRP1 = 97;
   }
   else if (wrSmDfltInterA5ThresRSRP1 < 0)/* RSRP Min range check */
   {
      wrSmDfltInterA5ThresRSRP1 = 0;
   }
   a5Cfg->threshold1_RSRP = wrSmDfltInterA5ThresRSRP1;
   a5Cfg->threshold1_RSRQ = wrSmDfltThresRSRQ1;
   /* if RSRP configured is out range (which is defined in RRC)
    * will be defaulted nearest value with in the range 
    * Here check spec defined range only */
   if (wrSmDfltInterA5ThresRSRP2 > 97) /* RSRP Max range check */
   {
      wrSmDfltInterA5ThresRSRP2 = 97;
   }
   else if (wrSmDfltInterA5ThresRSRP2 < 0)/* RSRP Min range check */
   {
      wrSmDfltInterA5ThresRSRP2 = 0;
   } 
   a5Cfg->threshold2_RSRP = wrSmDfltInterA5ThresRSRP2;
   a5Cfg->threshold2_RSRQ = wrSmDfltThresRSRQ2;
   a5Cfg->hysteresis = wrSmDfltHysteresis;
   a5Cfg->timeToTrigger = wrSmDfltTimeToTrig;
   a5Cfg->triggerQty = wrSmDfltTrigQty;
   a5Cfg->reportQty = wrSmDfltRptQty;
   a5Cfg->maxRptCells = wrSmDfltMaxRptCells;
   a5Cfg->reportInterval = wrSmDfltRptInterval;
   a5Cfg->reportAmount = wrSmDfltRptAmount;

   /* Configure A5 based Inter frequency ANR meas cfg */
   a5Cfg = &measCfg->measEvtA5Cfg[WR_MEAS_EVENTA5_INTER_FREQ_ANR];
   a5Cfg->isPres.pres = PRSNT_NODEF;
   a5Cfg->reason = wrSmDfltReason;
   /* if RSRP configured is out range (which is defined in RRC)
    * will be defaulted nearest value with in the range
    * Here check spec defined range only */
   if (wrSmDfltInterAnrA5ThresVal1 > 97) /* RSRP Max range check */
   {
      wrSmDfltInterAnrA5ThresVal1 = 97;
   }
   else if (wrSmDfltInterAnrA5ThresVal1 < 0)/* RSRP Min range check */
   {
      wrSmDfltInterAnrA5ThresVal1 = 0;
   }
   a5Cfg->threshold1_RSRP = wrSmDfltInterAnrA5ThresVal1;
   a5Cfg->threshold1_RSRQ = wrSmDfltThresRSRQ1;
   /* if RSRP configured is out range (which is defined in RRC)
    * will be defaulted nearest value with in the range 
    * Here check spec defined range only */
   if (wrSmDfltInterAnrA5ThresVal2 > 97) /* RSRP Max range check */
   {
      wrSmDfltInterAnrA5ThresVal2 = 97;
   }
   else if (wrSmDfltInterAnrA5ThresVal2 < 0)/* RSRP Min range check */
   {
      wrSmDfltInterAnrA5ThresVal2 = 0;
   } 
   a5Cfg->threshold2_RSRP = wrSmDfltInterAnrA5ThresVal2;
   a5Cfg->threshold2_RSRQ = wrSmDfltThresRSRQ2;
   a5Cfg->hysteresis = wrSmDfltHysteresis;
   a5Cfg->timeToTrigger = wrSmDfltTimeToTrig;
   a5Cfg->triggerQty = wrSmDfltTrigQty;
   a5Cfg->reportQty = wrSmDfltRptQty;
   a5Cfg->maxRptCells = wrSmDfltMaxRptCells;
   a5Cfg->reportInterval = wrSmDfltRptInterval;
   a5Cfg->reportAmount = wrSmDfltRptAmount; 

   /* configure Event Periodic */
   eutraPerd = &measCfg->measEutraPerdRptCfg[WR_MEAS_EUTRA_PERIODIC_REPORT_STRNG_CELLS];

   eutraPerd->isPres.pres = PRSNT_NODEF;
   eutraPerd->reason = wrSmDfltReason;
   eutraPerd->triggerQty = wrSmDfltTrigQty;
   eutraPerd->reportQty = wrSmDfltRptQty;
   eutraPerd->maxRptCells = wrSmDfltMaxRptCells;
   eutraPerd->reportInterval = wrSmDfltRptInterval;
   eutraPerd->reportAmount = wrSmDfltRptAmount;

   /*config for UTRA*/
   /* Below Report Amount parametrs hardcoded because current CSFB design 
    * support for only one measurement report per frequency. 
    * This will be resolved soon*/
   utraPerd = &measCfg->measUtraPerdRptCfg[WR_MEAS_UTRA_PRDRPT_STRGCELLS_CSFB];

   utraPerd->reason = wrSmDfltUtraReason;
   utraPerd->maxReportCell= wrSmDfltUtraMaxRptCells;
   utraPerd->reportInterval= (U8)wrSmDfltUtraRptInterval;
   utraPerd->reportAmount= 0;
   
   utraPerd = &measCfg->measUtraPerdRptCfg[WR_MEAS_UTRA_PRDRPT_STRGCELLS_ANR];

   utraPerd->reason = wrSmDfltUtraReason;
   utraPerd->maxReportCell= wrSmDfltUtraMaxRptCells;
   utraPerd->reportInterval= (U8)wrSmDfltUtraRptInterval;
   utraPerd->reportAmount= 0;

    /* Configure GERAN B2 Report confguration parameters for redirection */
   geranEvtB2Rpt = &measCfg->measGeranEventB2RptCfg[WR_MEAS_GERAN_EVENTB2_INTER_RAT_REDIR];
   geranEvtB2Rpt->reason            = WR_MEAS_GERAN_EVENTB2_INTER_RAT_REDIR; 
   geranEvtB2Rpt->activeState       = TRUE;
   geranEvtB2Rpt->b2Threshold1RSRP  = wrSmDfltGeranB2REDThres1RSRP;
   geranEvtB2Rpt->b2Threshold1RSRQ  = wrSmDfltGeranB2Thres1RSRQ;
   geranEvtB2Rpt->b2Threshold2Geran = wrSmDfltGeranB2REDThres2;
   geranEvtB2Rpt->hysteresis        = wrSmDfltHysteresis;
   geranEvtB2Rpt->timeToTrigger     = wrSmDfltTimeToTrig;
   geranEvtB2Rpt->maxReportCell     = wrSmDfltMaxRptCells;
   geranEvtB2Rpt->reportInterval    = wrSmDfltRptInterval;
   geranEvtB2Rpt->reportAmount      = wrSmDfltRptAmount;

    /* Configure GERAN B2 Report confguration parameters for  HO */
   geranEvtB2Rpt = &measCfg->measGeranEventB2RptCfg[WR_MEAS_GERAN_EVENTB2_INTER_RAT_HO];
   geranEvtB2Rpt->reason            = WR_MEAS_GERAN_EVENTB2_INTER_RAT_HO; 
   geranEvtB2Rpt->activeState       = TRUE;
   geranEvtB2Rpt->b2Threshold1RSRP  = wrSmDfltGeranB2HOThres1RSRP;
   geranEvtB2Rpt->b2Threshold1RSRQ  = wrSmDfltGeranB2Thres1RSRQ;
   geranEvtB2Rpt->b2Threshold2Geran = wrSmDfltGeranB2HOThres2;
   geranEvtB2Rpt->hysteresis        = wrSmDfltHysteresis;
   geranEvtB2Rpt->timeToTrigger     = wrSmDfltTimeToTrig;
   geranEvtB2Rpt->maxReportCell     = wrSmDfltMaxRptCells;
   geranEvtB2Rpt->reportInterval    = wrSmDfltRptInterval;
   geranEvtB2Rpt->reportAmount      = wrSmDfltRptAmount;


   /* Configure UTRA Event B2 for IRAT HO to WCDMA */
   utraB2Cfg = &measCfg->measUtraEventB2RptCfg[WR_MEAS_UTRA_EVENTB2_HO];

   utraB2Cfg->reason = WR_MEAS_UTRA_EVENTB2_HO; 
   utraB2Cfg->activeState = TRUE;
   
   /* If RSRP configured is exceeding Spec defined Val
    * then set it to default value */
   if( wrSmDfltUtraB2ThresRSRP > WR_SM_MAX_B2_THRESH1_RSRP)
   {
      wrSmDfltUtraB2ThresRSRP = 70;
   }
   utraB2Cfg->b2Threshold1RSRP = wrSmDfltUtraB2ThresRSRP;
   
   if( wrSmDfltUtraB2ThresRSRQ1 > WR_SM_MAX_B2_THRESH1_RSRQ)
   {
      wrSmDfltUtraB2ThresRSRQ1 = 32;
   }
   utraB2Cfg->b2Threshold1RSRQ = wrSmDfltUtraB2ThresRSRQ1;
   
   if(( wrSmDfltUtraB2ThresRSCP > WR_SM_MAX_B2_THRESH1_RSCP) || 
      ( wrSmDfltUtraB2ThresRSCP < WR_SM_MIN_B2_THRESH1_RSCP))
   {
      wrSmDfltUtraB2ThresRSCP = 65;
   }
   utraB2Cfg->b2Threshold2UtraRSCP = wrSmDfltUtraB2ThresRSCP;
   
   if( wrSmDfltUtraB2ThresEcNo > WR_SM_MAX_B2_THRESH1_ECNO)
   {
      wrSmDfltUtraB2ThresEcNo = 30;
   }
   utraB2Cfg->b2Threshold2UtraEcNo = wrSmDfltUtraB2ThresEcNo;
   
   utraB2Cfg->hysteresis = wrSmDfltHysteresis;
   utraB2Cfg->timeToTrigger = wrSmDfltTimeToTrig;
   utraB2Cfg->maxReportCell = wrSmDfltMaxRptCells;
   utraB2Cfg->reportInterval = wrSmDfltRptInterval;
   utraB2Cfg->reportAmount = wrSmDfltRptAmount;

   /* Configure UTRA Event B2 for Redirection to TD-SCDMA */
   utraB2Cfg = &measCfg->measUtraEventB2RptCfg[WR_MEAS_UTRA_EVENTB2_REDIR];

   utraB2Cfg->reason = WR_MEAS_UTRA_EVENTB2_REDIR; 
   utraB2Cfg->activeState = TRUE;
   
   /* If RSRP configured is exceeding Spec defined Val
    * then set it to default value */
   if( wrSmDfltUtraB2ThresRSRP > WR_SM_MAX_B2_THRESH1_RSRP)
   {
      wrSmDfltUtraB2ThresRSRP = 70;
   }
   utraB2Cfg->b2Threshold1RSRP = wrSmDfltUtraB2ThresRSRP;
   
   if( wrSmDfltUtraB2ThresRSRQ1 > WR_SM_MAX_B2_THRESH1_RSRQ)
   {
      wrSmDfltUtraB2ThresRSRQ1 = 32;
   }
   utraB2Cfg->b2Threshold1RSRQ = wrSmDfltUtraB2ThresRSRQ1;
   
   if(( wrSmDfltUtraTddB2ThresRSCP > WR_SM_MAX_B2_THRESH1_RSCP) || 
      ( wrSmDfltUtraTddB2ThresRSCP < WR_SM_MIN_B2_THRESH1_RSCP))
   {
      wrSmDfltUtraTddB2ThresRSCP = 65;
   }
   utraB2Cfg->b2Threshold2UtraTddRSCP = wrSmDfltUtraTddB2ThresRSCP;
   
   utraB2Cfg->hysteresis = wrSmDfltHysteresis;
   utraB2Cfg->timeToTrigger = wrSmDfltTimeToTrig;
   utraB2Cfg->maxReportCell = wrSmDfltMaxRptCells;
   utraB2Cfg->reportInterval = wrSmDfltRptInterval;
   utraB2Cfg->reportAmount = wrSmDfltRptAmount;

   geranPrdCfg = &measCfg->measGERANPeridRptCfg[LWR_MEAS_GERAN_REPORT_STRNG_CELLS];
   geranPrdCfg->isPres.pres    = PRSNT_NODEF;
   geranPrdCfg->reason         = wrSmDfltReason;
   geranPrdCfg->maxRptCells    = wrSmDfltGeranMaxRptCells;
   geranPrdCfg->reportInterval = wrSmDfltGeranRptInterval;
   geranPrdCfg->reportAmount   = wrSmDfltGeranRptAmt;

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
   measCfg->meas1xCDMAPeridRptCfg[0].maxRptCells = wrSmDfltCdmaMaxRptCells;
   measCfg->meas1xCDMAPeridRptCfg[0].reportInterval = wrSmDfltCdmaRptInterval;
   measCfg->meas1xCDMAPeridRptCfg[0].reportAmount = wrSmDfltCdmaRptAmount;

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
   measCfg->measSpdSclFact.sMeasure = wrSmDfltSMeasure;
   measCfg->measSpdSclFact.timeToTriggerSfMedium = wrSmDfltTimeToTrigSfMedium;
   measCfg->measSpdSclFact.timeToTriggerSfHigh = wrSmDfltTimeToTrigSfHigh;

   /* configure Mobility state Params */
   measCfg->mobStatParams.speedStatPres.pres =  PRSNT_NODEF;
   measCfg->mobStatParams.tEvalulation = wrSmDfltTEvalulation;
   measCfg->mobStatParams.tHystNormal = wrSmDfltTHystNormal;
   measCfg->mobStatParams.nCellChangeMedium = wrSmDfltNCellChangeMedium;
   measCfg->mobStatParams.nCellChangeHigh = wrSmDfltNCellChangeHigh;

   /* configure Quantity params */
   measCfg->qtyCfg.rsrpFltrCoeff = wrSmDfltRsrpFltrCoeff;
   measCfg->qtyCfg.rsrqFltrCoeff = wrSmDfltRsrqFltrCoeff;
   measCfg->qtyCfg.measQtyCDMA2k = wrSmDfltMeasQtyCDMA2k;
   measCfg->qtyCfg.ecnoFltrCoeff = wrSmDfltEcnoFltrCoeff;
   measCfg->qtyCfg.measQtyUtraFdd = (U8)wrSmDfltMeasQtyUtraFdd;
   measCfg->qtyCfg.measQtyUtraTdd = (U8)wrSmDfltMeasQtyUtraTdd;
   measCfg->qtyCfg.fltrCoeffGERAN = (U8)wrSmDfltGeranFltrCoeff;
   measCfg->qtyCfg.filterCoefficient2FDDr10 = wrSmDfltEcnoFltrCoeff;/*RRC Rel10 Upgrade*/

   /* configure Measurement Gap params */
   measCfg->measGap.gapForANRPres.pres =  PRSNT_NODEF;
   measCfg->measGap.gapPatternForInterFA = wrSmDfltGapPatForInterFA;
   measCfg->measGap.gapPatternForInterRAT = wrSmDfltGapPatForInterRAT;
   measCfg->measGap.gapPatternForANR = wrSmDfltGapPatForANR;

   /* configure PreRegistration HRPD */
   measCfg->preRegHrpd.preRegAllowed = wrSmDfltPreRegAllowed;
   measCfg->preRegHrpd.preRegZoneId = wrSmDfltPreRegZoneId;
   measCfg->preRegHrpd.NumOf2ndPreRegZoneIdList =
      wrSmDfltNumOf2ndPreRegZoneIdList;
   measCfg->preRegHrpd.secondaryPreRegZoneId1 = wrSmDfltSecondaryPreRegZoneId1;
   measCfg->preRegHrpd.secondaryPreRegZoneId2 = wrSmDfltSecondaryPreRegZoneId2;

   measCfg->anrRptCfg = wrSmDfltAnrRptCfgVal;
   measCfg->hoRptCfg = wrSmDfltHoRptCfgVal;

   /*rsrq_ho*/
   measCfg->rsrqHoThrshld = wrSmDfltThresRSRQ;

   RETVOID;
} /* end of smBldMeasCellCfg */

/* Populate the SIB periodicites */

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
*       File:  wr_sm_wr_cntrl.c 
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
   
   LwrCellMibCfgGrp *lwrMib = NULLP;
   LwrCellSib1CfgGrp *sib1 =NULLP;
   LwrCellSib2CfgGrp *sib2 =NULLP;
   LwrCellSib3CfgGrp *sib3 =NULLP;
   LwrCellSib4CfgGrp *sib4 =NULLP;
   LwrProcTimerCfgGrp *tmrCfg = NULLP;
   U8                cfgGrpIndex = 0;
   U8                idx = 0;
   LwrCellSib6CfgGrp *sib6 = NULLP;
   LwrCellSib7CfgGrp *sib7 = NULLP;
   LwrCellSib9CfgGrp *sib9 = NULLP;      
   LwrCellUlSchdCfgGrp   *ulSchdCfgGrp = NULLP;
   LwrCellDlSchdCfgGrp   *dlSchdCfgGrp = NULLP;
   LwrAddCellCfg     *enbCfg = NULLP;
   LwrRgrCfgGrp      *rgrCfg = NULLP;
   LwrCtfCfgGrp      *ctfCfg = NULLP;
   LwrRlcCfgGrp      *rlcCfg = NULLP;
   LwrCellSchdCfg    *schdCfg = NULLP;
   LwrSrbPolicy     *srbInfo = NULLP;
   U16               dfltDrxCycle;
   U8                dfltModPrdCoeff;
   U8                loopCnt;
   U8 wrHoDedPreamCount = 0;
   U32 grpIdx,freingIdx;
   TRC2(smBuildCellCfg);

   enbCfg = &(lwrMng->t.protoCfg.t.cellCfg.t.addCell);
   lwrMng->t.protoCfg.type = WR_CFG_TYPE_CELL;
   lwrMng->t.protoCfg.t.cellCfg.action = WR_ACT_ADD;
   
   lwrMng->t.protoCfg.t.cellCfg.t.addCell.dlCarrierFreq =
      wrSmDfltServingCellDLCarrierFreq;
   lwrMng->t.protoCfg.t.cellCfg.t.addCell.duplexMode = wrSmDfltDuplexMode;

    /*DRX QCI*/

   lwrMng->t.protoCfg.t.cellCfg.t.addCell.isDRXQCIEnabled = wrSmDfltisDrxQCIEnabled;

   lwrMng->t.protoCfg.t.cellCfg.t.addCell.cellId = 1; /* this is ownCellID in case of PLD based, this is jst a logical value */
   lwrMng->t.protoCfg.t.cellCfg.t.addCell.cellIndex = WR_DFLT_CELL_IDX;
   lwrMng->t.protoCfg.t.cellCfg.t.addCell.physCellId = (U16)wrSmDfltPhysCellId;
   lwrMng->t.protoCfg.t.cellCfg.t.addCell.measFlagForCsfbUtra = wrSmDfltCsfbToUtraMeasSup;
   lwrMng->t.protoCfg.t.cellCfg.t.addCell.measFlagForCsfbCdma = wrSmDfltCsfbToCdmaMeasSup;
   lwrMng->t.protoCfg.t.cellCfg.t.addCell.measFlagForEcsfb = wrSmDfltEcsfbMeasSup;

   /* CSG_DEV start */
   lwrMng->t.protoCfg.t.cellCfg.t.addCell.csgAccessMode = wrSmDfltAccessMode;
   if(wrSmDfltIsSmlCellPciCfgd == TRUE)
   {
      lwrMng->t.protoCfg.t.cellCfg.t.addCell.smallCellCsgPciRange.pciRangeInfoPres = PRSNT_NODEF;
      lwrMng->t.protoCfg.t.cellCfg.t.addCell.smallCellCsgPciRange.startPci = wrSmDfltSmallCellStartPci;
      lwrMng->t.protoCfg.t.cellCfg.t.addCell.smallCellCsgPciRange.pciRange = wrSmDfltSmallCellPciRange;
   }
   if(wrSmDfltAccessMode == WR_ENB_HYBRID_ACCESS)
   {
      lwrMng->t.protoCfg.t.cellCfg.t.addCell.hcsgParam.minDlResNonCsg = 
                                 wrSmDfltMinDlResNonCsg;
      lwrMng->t.protoCfg.t.cellCfg.t.addCell.hcsgParam.minUlResNonCsg = 
                                 wrSmDfltMinUlResNonCsg;
      lwrMng->t.protoCfg.t.cellCfg.t.addCell.hcsgParam.maxCsgUeSup    = 
                                 wrSmDfltMaxCsgUEs;
      lwrMng->t.protoCfg.t.cellCfg.t.addCell.hcsgParam.maxNonCsgUeSup = 
                                 wrSmDfltMaxNonCsgUEs;
   }
   /* CSG_DEV end */

   lwrMng->t.protoCfg.t.cellCfg.t.addCell.cellSizeType = wrSmDfltCellSizeType;/*UE History info*/
   /*Filling MIB params*/
   /* mem_issue_fix: Allocating memory for all the cell group */
   for(grpIdx = 0;grpIdx<WR_CELL_CFG_MAX_GRPS ;grpIdx++)
   {
      WR_ALLOC(&(lwrMng->t.protoCfg.t.cellCfg.t.addCell.cellCfgGrp[grpIdx]), \
             sizeof(LwrCellCfgGrp));
      if(NULLP == lwrMng->t.protoCfg.t.cellCfg.t.addCell.cellCfgGrp[grpIdx])
      {
         for(freingIdx= 0 ; freingIdx<grpIdx; freingIdx++)
         {
            WR_FREE(lwrMng->t.protoCfg.t.cellCfg.t.addCell.cellCfgGrp[freingIdx],\
                  sizeof(LwrCellCfgGrp));
         }
         RLOG0(L_FATAL, "Memory allocation failed for cell configuration");
         RETVALUE(RFAILED);
      }
   }
   lwrMng->t.protoCfg.t.cellCfg.t.addCell.cellCfgGrp[cfgGrpIndex]->grpType 
                                                         = WR_CELL_CFG_GRP_MIB;
   lwrMib = &(lwrMng->t.protoCfg.t.cellCfg.t.addCell.cellCfgGrp[cfgGrpIndex]->
         t.mibCfgGrp);
   
   lwrMib->dlBw = wrSmDfltCellDlBW;
   lwrMib->phichCfg.phichDur = wrSmDfltPhichDur;
   lwrMib->phichCfg.phichRes = wrSmDfltPhichRes;
   cfgGrpIndex++;

   /*Filling sib1 params*/
   lwrMng->t.protoCfg.t.cellCfg.t.addCell.cellCfgGrp[cfgGrpIndex]->grpType 
                                                        = WR_CELL_CFG_GRP_SIB1;
                                                          
   sib1 =&(lwrMng->t.protoCfg.t.cellCfg.t.addCell.cellCfgGrp[cfgGrpIndex]->
         t.sib1CfgGrp);

   sib1->freqBandInd = wrSmDfltFreqBandInd;
   sib1->cellAccessInfo.numOfPlmns = wrSmDfltNumPlmn;

   for(loopCnt=0; loopCnt<sib1->cellAccessInfo.numOfPlmns; loopCnt++)
   {
      sib1->cellAccessInfo.plmnInfo[loopCnt].plmnId.numMncDigits = wrSmDfltNumOfMncDigits[loopCnt];

      sib1->cellAccessInfo.plmnInfo[loopCnt].plmnId.mcc[0] = wrSmDfltPlmnMcc[loopCnt][0];
      sib1->cellAccessInfo.plmnInfo[loopCnt].plmnId.mcc[1] = wrSmDfltPlmnMcc[loopCnt][1];
      sib1->cellAccessInfo.plmnInfo[loopCnt].plmnId.mcc[2] = wrSmDfltPlmnMcc[loopCnt][2];

      sib1->cellAccessInfo.plmnInfo[loopCnt].plmnId.mnc[0] = wrSmDfltPlmnMnc[loopCnt][0];
      sib1->cellAccessInfo.plmnInfo[loopCnt].plmnId.mnc[1] = wrSmDfltPlmnMnc[loopCnt][1];
      sib1->cellAccessInfo.plmnInfo[loopCnt].plmnId.mnc[2] = wrSmDfltPlmnMnc[loopCnt][2];

      sib1->cellAccessInfo.plmnInfo[loopCnt].rsrvdFrOperUse = wrSmDfltCellRsrvdForOperUse;
   }

   sib1->cellAccessInfo.tac = wrSmDfltTac;

   sib1->cellAccessInfo.cellId = wrSmDfltCellId; /* read cellId from wr_cfg.txt wrSmDfltCellIdentity */
   /* CSG_DEV start */
   if (WR_ENB_OPEN_ACCESS == wrSmDfltAccessMode)
   {
      sib1->cellAccessInfo.csgIndication = FALSE;
      sib1->cellAccessInfo.csgIdentity.pres = NOTPRSNT;
   }
   else if(WR_ENB_CLOSED_ACCESS == wrSmDfltAccessMode)
   {
      sib1->cellAccessInfo.csgIndication = TRUE;
      wrFillTknU32(&sib1->cellAccessInfo.csgIdentity, wrSmDfltCsgIdVal);
   }
   else /* hybrid access */
   {
      sib1->cellAccessInfo.csgIndication = FALSE;
      wrFillTknU32(&sib1->cellAccessInfo.csgIdentity, wrSmDfltCsgIdVal);
   }
   /* CSG_DEV end */
   sib1->cellAccessInfo.cellBarred = (Bool)wrSmDfltCellBarred;
   sib1->cellAccessInfo.intraFreqReSelection = (Bool)wrSmDfltIntraFreqResel;

   sib1->cellSelectInfo.qRxLevMin = (S8)wrSmDlftqRxLevMin;
   /* Start - Fix for ccpu00123651 */
   wrFillTknU8(&(sib1->cellSelectInfo.qRxLevMinOffset),
         (U8)wrSmDfltqRxlevminoffset);
   /* End - Fix for ccpu00123651 */

   sib1->pMax = (S8)wrSmDfltPMax;
   
   sib1->siWindowSize = wrSmDfltSiWinSize;
/* ACC-TDD */   
#ifdef LTE_TDD
   sib1->tddCfg.specialSubframeCfg = wrSmTddSpecialSubframeConfig;
   sib1->tddCfg.subframeCfg = wrSmTddSubframeConfig;
#else
   // Not used for FDD - zero the configuration just to be safe
   sib1->tddCfg.specialSubframeCfg = 0;
   sib1->tddCfg.subframeCfg = 0;
#endif

   cfgGrpIndex++; 

   /*Filling Sib2 params*/
   lwrMng->t.protoCfg.t.cellCfg.t.addCell.cellCfgGrp[cfgGrpIndex]->grpType 
                                                        = WR_CELL_CFG_GRP_SIB2;
                                                          
   sib2 =&(lwrMng->t.protoCfg.t.cellCfg.t.addCell.cellCfgGrp[cfgGrpIndex]->
         t.sib2CfgGrp);

   sib2->ueTimerConstants.t302 = 9;

   sib2->ueTimerConstants.t300 = wrSmDfltTmrsAndConstsT300;
   sib2->ueTimerConstants.t301 = wrSmDfltTmrsAndConstsT301;
   /* Fix for ccpu00137900 - Updating the t304 timer value */
   sib2->ueTimerConstants.t304 = wrSmDfltTmrsAndConstsT304;
   sib2->ueTimerConstants.t310 = wrSmDfltTmrsAndConstsT310;
   sib2->ueTimerConstants.n310 = wrSmDfltTmrsAndConstsN310;
   sib2->ueTimerConstants.t311 = wrSmDfltTmrsAndConstsT311;
   sib2->ueTimerConstants.n311 = wrSmDfltTmrsAndConstsN311;
   sib2->timeAlignmentCommon = wrSmDfltTimeAlignTmrCmn;

   wrFillTknU8(&(sib2->acBarringInfo.pres), wrSmDfltACBarrInfoPres);
   sib2->acBarringInfo.acBarringForEmergency =  wrSmDfltACBarrNormal;

   wrFillTknU8(&(sib2->acBarringInfo.acBarringForMOData.pres), 
                                                     wrSmDfltMOACBarrInfoPres);

   sib2->ulFreqInfo.ulBandwidth = wrSmDfltCellUlBW;
   sib2->ulFreqInfo.eARFCN = wrSmDfltUlEarFcn;
   sib2->ulFreqInfo.addSpectEmission =(Bool)wrSmDfltAddSpectEmn;
   
   sib2->acBarringInfo.acBarringForMOData.acBarringAC11 = FALSE;
   sib2->acBarringInfo.acBarringForMOData.acBarringAC12 = FALSE;
   sib2->acBarringInfo.acBarringForMOData.acBarringAC13 = FALSE;
   sib2->acBarringInfo.acBarringForMOData.acBarringAC14 = FALSE;
   sib2->acBarringInfo.acBarringForMOData.acBarringAC15 = FALSE;

   sib2->acBarringInfo.acBarringForMOData.acBarringFactor 
                                                      =wrSmDfltacBarringFactor;
                                                      
   sib2->acBarringInfo.acBarringForMOData.acBarringTime = 
                                                        wrSmDfltacBarringTime;
                                                        
   wrFillTknU8(&(sib2->acBarringInfo.acBarringForMOSignal.pres), 
                                                     wrSmDfltDOACBarrInfoPres);
                                                       
   sib2->acBarringInfo.acBarringForMOSignal.acBarringAC11 = FALSE;
   sib2->acBarringInfo.acBarringForMOSignal.acBarringAC12 = FALSE;
   sib2->acBarringInfo.acBarringForMOSignal.acBarringAC13 = FALSE;
   sib2->acBarringInfo.acBarringForMOSignal.acBarringAC14 = FALSE;
   sib2->acBarringInfo.acBarringForMOSignal.acBarringAC15 = FALSE;

   sib2->acBarringInfo.acBarringForMOSignal.acBarringFactor = 
                                                       wrSmDfltacBarringFactor;

   sib2->acBarringInfo.acBarringForMOSignal.acBarringTime = 
                                                         wrSmDfltacBarringTime;
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
                                                 =(Bool)wrSmDfltModPeriodCoeff;
   /*Filling PCCH*/
   sib2->radioResourceCommon.pcchCfgCommon.defaultPagingCycle 
                                                      =wrSmDfltDefaultPagCycle;
   sib2->radioResourceCommon.pcchCfgCommon.nB =wrSmDfltPCCHCfgNB;
   
   /*Filling PDSCH*/

   sib2->radioResourceCommon.pdschCfgCommon.pB =(Bool)wrSmDfltPDSCHCfgPB;
   sib2->radioResourceCommon.pdschCfgCommon.referenceSignalPwr = 
                                                        wrSmDfltRefSignalPower;

   /*Filling PRACH*/

   sib2->radioResourceCommon.prachCfgCommon.rootSequenceIndx = 
                                                      wrSmDfltRootSequenceIdx;

   sib2->radioResourceCommon.prachCfgCommon.prachCfgInfo.highSpeedFlag = 
                                                   (Bool)wrSmDfltHighSpeedFlag;

   sib2->radioResourceCommon.prachCfgCommon.prachCfgInfo.prachCfgIdx = 
                                                     (Bool)wrSmDfltPrachCfgIdx;
   sib2->radioResourceCommon.prachCfgCommon.prachCfgInfo.prachFreqOffset = 
                                                 (Bool)wrSmDfltPrachFreqOffset;
   sib2->radioResourceCommon.prachCfgCommon.prachCfgInfo.zeroCorrelationZoneCfg
                                             = (Bool)wrSmDfltZeroCorrelZoneCfg;

   /*Filling PUCCH*/

   sib2->radioResourceCommon.pucchCfgCommon.deltaShift 
                                                      =wrSmDfltPUCCHDeltaShift;
   sib2->radioResourceCommon.pucchCfgCommon.n1PucchAn =wrSmDfltNlPUCCHAn;
   sib2->radioResourceCommon.pucchCfgCommon.nCSAN = (Bool)wrSmDfltPUCCHnCSAN;
   sib2->radioResourceCommon.pucchCfgCommon.nRBCqi =(Bool)wrSmDfltPUCCHnRBCQI;

   /*Filling PUSCH*/

   sib2->radioResourceCommon.puschCfgCommon.puschBasicCfg.enable64QAM 
                                                  =(Bool)wrSmDfltEnable64QAM;
   sib2->radioResourceCommon.puschCfgCommon.puschBasicCfg.hoppingMode =
                                                           wrSmDfltHoppingMode;
   sib2->radioResourceCommon.puschCfgCommon.puschBasicCfg.hoppingOffset= 
                                                   (U8)wrSmDfltPuschHoppingOff;
                                                      
   sib2->radioResourceCommon.puschCfgCommon.puschBasicCfg.nSb =(U8)wrSmDfltNSB;

   /*Filling RACH*/
   
   sib2->radioResourceCommon.rachCfgCommon.maxHARQMsg3 =
      (U8)wrSmDfltMaxHARQMsg3Tx;
   
   sib2->radioResourceCommon.rachCfgCommon.powerRamping.initRcvdTrgtPwr = 
                                               wrSmDfltPreambleInitRecvdTgtPwr;
                                                  
   sib2->radioResourceCommon.rachCfgCommon.powerRamping.powerRampingstep=
                                                  (U8)wrSmDfltPowerRampingStep;
   
   sib2->radioResourceCommon.rachCfgCommon.preambleCfg.groupASize =
                                            (U8)wrSmDflSizeOfRaPreamblesGroupA;
   sib2->radioResourceCommon.rachCfgCommon.preambleCfg.numOfRAPreambles=
                                               (U8)wrSmDfltNumberOfRaPreambles;
   sib2->radioResourceCommon.rachCfgCommon.preambleCfg.msgSizeGroupA=
                                                   wrSmDfltMessageSizeGroupA;
                                                      
   sib2->radioResourceCommon.rachCfgCommon.preambleCfg.pwrOffstGrpB=
                                              wrSmDfltMessagePowerOffsetGroupB;
    
   sib2->radioResourceCommon.rachCfgCommon.raSupervisionCfg.macContResTimer=
                                                  (U8)wrSmDfltMacContentResTmr;
                                                      
   sib2->radioResourceCommon.rachCfgCommon.raSupervisionCfg.preambleTransMax=
                                                 (U8)wrSmDfltPreambleTransMax;
   sib2->radioResourceCommon.rachCfgCommon.raSupervisionCfg.rarWindowSize=
                                                   (U8)wrSmDfltRaRspWindowSize;

   /*Filling SRC */

   sib2->radioResourceCommon.srsCfgCommon.cSrs =(U8)wrSmDfltSrsBandwidthCfg;
   sib2->radioResourceCommon.srsCfgCommon.simulAckNackSrs=
                                                 (U8)wrSmDfltAckNackSrsSimTrns;
   sib2->radioResourceCommon.srsCfgCommon.srsMaxUpts =(U8)wrSmDfltSrsMaxUpPts;
   sib2->radioResourceCommon.srsCfgCommon.subframeCfg =
      (U8)wrSmDfltSrsSubfrmCfg;

   /*Filling UL Power Control*/
   sib2->radioResourceCommon.ulCyclicPrefix =wrSmDfltUlCyclicPrefixLength;
   sib2->radioResourceCommon.ulPwrCtrlCommon.alpha = wrSmDfltULPwrCtrlAlpha;
   
   sib2->radioResourceCommon.ulPwrCtrlCommon.deltaFLst.deltaPucchFormat1=
                                                   wrSmDfltDeltaFPUCCHFormat1;
                                                   
   sib2->radioResourceCommon.ulPwrCtrlCommon.deltaFLst.deltaPucchFormat1b=
                                                   wrSmDfltDeltaFPUCCHFormat1b;
                                                   
   sib2->radioResourceCommon.ulPwrCtrlCommon.deltaFLst.deltaPucchFormat2=
                                                    wrSmDfltDeltaFPUCCHFormat2;
   sib2->radioResourceCommon.ulPwrCtrlCommon.deltaFLst.deltaPucchFormat2a=
                                                   wrSmDfltDeltaFPUCCHFormat2a;
   sib2->radioResourceCommon.ulPwrCtrlCommon.deltaFLst.deltaPucchFormat2b=
                                                   wrSmDfltDeltaFPUCCHFormat2b;
   sib2->radioResourceCommon.ulPwrCtrlCommon.deltaMsg3 =
                                               (S8)wrSmDfltUlDeltaPreambleMsg3;
   
   sib2->radioResourceCommon.ulPwrCtrlCommon.pNomialPucch=
                                                    (S8)wrSmDfltP0NominalPUCCH;
   sib2->radioResourceCommon.ulPwrCtrlCommon.pNomialPusch=
                                                    (S8)wrSmDfltP0NominalPUSCH;
   sib2->timeAlignmentCommon = wrSmDfltTaTmrCmn;
   cfgGrpIndex++; 

    /* Configure the SIB3 parameters*/
   lwrMng->t.protoCfg.t.cellCfg.t.addCell.cellCfgGrp[cfgGrpIndex]->grpType =
                                                          WR_CELL_CFG_GRP_SIB3;
   
   sib3 = &(lwrMng->t.protoCfg.t.cellCfg.t.addCell.cellCfgGrp[cfgGrpIndex]->
         t.sib3CfgGrp);

    /*Updating the SIB3 parameters*/
   sib3->qHyst                               = wrSmDfltqHyst;
   sib3->speedStateReselPres                 = WR_PRSNT;
   sib3->sfMedium                            = wrSmDfltSfMed;
   sib3->sfHigh                              = wrSmDfltSfHigh;
   /*Updating the mobility state parameters*/
   sib3->mobStateParams.tEvalulation         = wrSmDfltTEval;
   sib3->mobStateParams.tHystNormal          = wrSmDfltTHyst;
   sib3->mobStateParams.nCellChangeMedium    =(U16)wrSmDfltS3nCellChangeMedium;
   sib3->mobStateParams.nCellChangeHigh      = (U16)wrSmDfltS3nCellChangeHigh;

   cfgGrpIndex++; 

   /* Configure the SIB4 parameters*/
   lwrMng->t.protoCfg.t.cellCfg.t.addCell.cellCfgGrp[cfgGrpIndex]->grpType =
                                                          WR_CELL_CFG_GRP_SIB4;
   sib4 = &(lwrMng->t.protoCfg.t.cellCfg.t.addCell.cellCfgGrp[cfgGrpIndex]->
         t.sib4CfgGrp);

   /*Updating the CSG Split Info parameters */
   /* CSG_DEV start */
   if(wrSmDfltIsCsgPciCfgd == TRUE)
   {
      sib4->csgInfo.csgStartPhyCellId           = (U16)wrSmDfltCSGStartPci; 
      sib4->csgInfo.isCsgPciCfgd            = WR_PRSNT;
      sib4->csgInfo.csgPciRange                 = (U8)wrSmDfltCSGPciRange;
   }
   else
   {
      sib4->csgInfo.isCsgPciCfgd            = NOTPRSNT;
   }
   /* CSG_DEV end */

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
   tmrCfg->s1ErabReleaseIndicationTmr      = wrSmDflts1ErabReleaseIndicationTmr;
   tmrCfg->s1UeContextReleaseTmr           = wrSmDflts1UeContextReleaseTmr;
   tmrCfg->s1PathSwitchTmr                 = wrSmDflts1PathSwitchTmr;
   tmrCfg->s1HandoverPrepTmr               = smCfgCb.s1PrepTimerVal;
   tmrCfg->s1RelocOverallTmr               = smCfgCb.s1OvrAllTimerVal;
   /* For wr_cfg.txt the IRAT HO Timers uses same value 
    * as cfgd for Intra LTE HO */
   tmrCfg->s1IRatHandoverPrepTmr           = smCfgCb.s1PrepTimerVal;
   tmrCfg->s1IRatRelocOverallTmr           = smCfgCb.s1OvrAllTimerVal;
   tmrCfg->s1HandoverCancelTmr             = smCfgCb.s1CancelTimerVal; 
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
   tmrCfg->rimOvrlTmr                      = smCfgCb.rimOvrlTmr;
   tmrCfg->rimRirTmr                       = smCfgCb.rimRirTmr;
   tmrCfg->rimRiTmr                        = smCfgCb.rimRiTmr;
   tmrCfg->rimRiaeTmr                      = smCfgCb.rimRiaeTmr;
   tmrCfg->rimMaxRetryCnt                  = smCfgCb.rimMaxRetryCnt;

   /*Config SIB6 Params*/
   cfgGrpIndex++; 

   lwrMng->t.protoCfg.t.cellCfg.t.addCell.cellCfgGrp[cfgGrpIndex]->grpType =
                                                          WR_CELL_CFG_GRP_SIB6;
   
   sib6 = &(lwrMng->t.protoCfg.t.cellCfg.t.addCell.cellCfgGrp[cfgGrpIndex]->
         t.sib6CfgGrp);

    /*Updating the SIB3 parameters*/
   sib6->reCfgType                           = wrSmDfltUtraReCfgType;
   sib6->sfUsagePres.pres                    = PRSNT_NODEF;
   sib6->tReselection                        = wrSmDfltReslct;
   sib6->tReselectionSfMedium                = wrSmDfltReslctSfMed;
   sib6->tReselectionSfHigh                  = wrSmDfltReslctSfHigh;
   /*Config SIB7 Params*/
   cfgGrpIndex++; 

   lwrMng->t.protoCfg.t.cellCfg.t.addCell.cellCfgGrp[cfgGrpIndex]->grpType =
                                                          WR_CELL_CFG_GRP_SIB7;
   
   sib7 = &(lwrMng->t.protoCfg.t.cellCfg.t.addCell.cellCfgGrp[cfgGrpIndex]->t.sib7CfgGrp);

   sib7->reCfgType                           = wrSmDfltGeranReCfgType;
   sib7->tReselectionGERAN                   = wrSmDfltGerantReselection;
   sib7->pres_SpeedStateScaleFactors.pres    = PRSNT_NODEF;
   sib7->pres_SpeedStateScaleFactors.val     = PRSNT_NODEF;
   sib7->tReselectionGERANSFMedium           = LWR_O_DOT_5;
   sib7->tReselectionGERANSFHigh             = LWR_O_DOT_5;
   /*Config SIB7 Params*/ 

   cfgGrpIndex++; 
    /*Config SIB9 Params*/
   lwrMng->t.protoCfg.t.cellCfg.t.addCell.cellCfgGrp[cfgGrpIndex]->grpType =
                                                          WR_CELL_CFG_GRP_SIB9;
   sib9 =
      &(lwrMng->t.protoCfg.t.cellCfg.t.addCell.cellCfgGrp[cfgGrpIndex]->
            t.sib9CfgGrp);
   /* CSG_DEV */
   cmMemcpy(sib9->henbName, (U8 *)wrSmDfltHenbName, strlen((S8 *)wrSmDfltHenbName));
   sib9->henbName[strlen((S8 *)wrSmDfltHenbName)] = '\0';
  
   cfgGrpIndex++;

   /* Build the SIB8 configuration parameters */
   if (smCfgCb.noOfNeighCdma1xCells > 0)
   {
      smBldSib8Cfg(lwrMng,cfgGrpIndex);
      cfgGrpIndex++; 
   }

   /* Build the CSFB configuration parameters */
   if(RFAILED == smBldCsfbCfg(lwrMng,cfgGrpIndex))
   {
        RETVALUE(RFAILED);
   }
   cfgGrpIndex++; 

   /* Build the SIB Scheduling confuration parameters */
   smBldCellSibSchCfg(lwrMng, cfgGrpIndex);
   cfgGrpIndex++; 
  
   /* Build the RAB config params*/
   smBldCellRabConfigParam(lwrMng,cfgGrpIndex);
   cfgGrpIndex++;

   smBldCellEaidCfg(lwrMng, cfgGrpIndex);
   cfgGrpIndex++; 

   /* Build the Measurement  configuration parameters */
   smBldMeasCellCfg(lwrMng, cfgGrpIndex);
   cfgGrpIndex++; 

   /* Build the ANR confuration parameters */
   smBldCellAnrCfg(lwrMng, cfgGrpIndex); 
   cfgGrpIndex++; 

   /* Fill the RRM config */
   lwrMng->t.protoCfg.t.cellCfg.t.addCell.cellCfgGrp[cfgGrpIndex]->grpType = 
                                                           WR_CELL_CFG_GRP_RRM;
   lwrMng->t.protoCfg.t.cellCfg.t.addCell.cellCfgGrp[cfgGrpIndex]->
      t.rrmCfgGrp.enbCacInfo.maxEnbCallCount = smCfgCb.maxUeSupp;
   
   lwrMng->t.protoCfg.t.cellCfg.t.addCell.cellCfgGrp[cfgGrpIndex]->
       t.rrmCfgGrp.enbCacInfo.dfltRrmRntiSize = wrSmDfltRrmRntisize;

   lwrMng->t.protoCfg.t.cellCfg.t.addCell.cellCfgGrp[cfgGrpIndex]->
      t.rrmCfgGrp.enbCacInfo.dfltRrmStartPreamble = wrSmDfltRrmStartPreamble;
   
   /* Using 2/3 of the dedicated preambles for HO and remaining 
    * for PDCCH order */
   wrHoDedPreamCount = (wrSmDfltRrmNumPreambles * 2)/3;
   if(wrHoDedPreamCount)
   {
      lwrMng->t.protoCfg.t.cellCfg.t.addCell.cellCfgGrp[cfgGrpIndex]->
         t.rrmCfgGrp.enbCacInfo.dfltRrmNumPreamble = wrHoDedPreamCount;
   }
   else
   {/* Very less preambles are configured. Assigning complete 
       * for HO and none for pdcch order */
      lwrMng->t.protoCfg.t.cellCfg.t.addCell.cellCfgGrp[cfgGrpIndex]->
         t.rrmCfgGrp.enbCacInfo.dfltRrmNumPreamble = wrSmDfltRrmNumPreambles;
   }
   cfgGrpIndex++; 



/* LTE_ADV starts */
   /*Fill the LTE Advanced Config*/
   lwrMng->t.protoCfg.t.cellCfg.t.addCell.cellCfgGrp[cfgGrpIndex]->grpType = WR_CELL_CFG_GRP_LTEA;
   cmMemcpy((U8 *)&lwrMng->t.protoCfg.t.cellCfg.t.addCell.cellCfgGrp[cfgGrpIndex]->t.lteAdvCfgGrp.lteAdvancedConfig, 
            (U8 *)&smCfgCb.lteAdvancedConfig, sizeof(LwrLteAdvancedConfig));
   lwrMng->t.protoCfg.t.cellCfg.t.addCell.cellCfgGrp[cfgGrpIndex]->t.lteAdvCfgGrp.pres = TRUE;
   cfgGrpIndex++; 
/* LTE_ADV ends */

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
   
   rgrCfg->dfltStartRnti = wrSmDfltStartRnti;
   rgrCfg->dfltRntiSize = wrSmDfltMacRntisize;
   rgrCfg->dfltCfiCfg = wrSmDfltCfiCfg;
   rgrCfg->dfltTrgUlCqi = wrSmDfltTrgUlCqi;
   rgrCfg->dfltBcchPchRaCodeRate = wrSmDfltBcchPchRaCodeRate;
   rgrCfg->dfltPdcchCodeRate = wrSmDfltPdcchCodeRate;
   rgrCfg->dfltPuschSubBndStrt = wrSmDfltPuschSubBndStrt;
   rgrCfg->dfltPuschNoSubbnds = wrSmDfltPuschNoSubbnds;
   rgrCfg->dfltULCmnCodRateCcchCqi = wrSmDfltULCmnCodRateCcchCqi;
   rgrCfg->dfltDLCmnCodRateCcchCqi = wrSmDfltDlCmnCodRateCcchCqi;
   rgrCfg->dfltIsDlFreqSel = wrSmDfltDLFSSchdTyp;
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
   rlcCfg->ulModeInfo.srbtReordring = wrSmDfltUlSrbtReordring;
   rlcCfg->ulModeInfo.dfltAmRlcDrbtStatusProh = wrSmDfltAmRlcDrbtStatusProh;
   rlcCfg->ulModeInfo.drbtReordring = wrSmDfltUlDrbtReordring;
   rlcCfg->ulModeInfo.dfltUmRlcsnFieldLen = wrSmDfltUmRlcsnFieldLen;

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
   lwrMng->t.protoCfg.t.cellCfg.t.addCell.cellCfgGrp[cfgGrpIndex]->grpType = 
      WR_CELL_CFG_GRP_RAB_POLICY;

   for(idx = 0; idx < (LWR_MAX_SRBS-1); idx++)
   {   
      srbInfo = &(lwrMng->t.protoCfg.t.cellCfg.t.addCell.cellCfgGrp[cfgGrpIndex]->t.rabPolicyCfgGrp.srbPolicy[idx]);
      srbInfo->rlcInfo.tpRetransmit = wrSmDfltSrbPollRetrans;
      srbInfo->rlcInfo.pollPdu = wrSmDfltSrbPollPdu;
      srbInfo->rlcInfo.pollByte = wrSmDfltSrbPollByte;
      srbInfo->rlcInfo.maxRetransThresh = wrSmDfltSrbMaxReTxThresh;
      srbInfo->rlcInfo.timerReorderingDl = wrSmDfltDlSrbtReordring;
      srbInfo->rlcInfo.timerReorderingUl = wrSmDfltUlSrbtReordring;
      srbInfo->rlcInfo.timerStatusProhibit = wrSmDfltSrbTStatProhibit;
      srbInfo->lchInfo.lchGrpId           = wrSmDfltPrioInfoTbl.srb[idx].lchGrp;
      srbInfo->lchInfo.bucketSizeDuration = wrSmDfltPrioInfoTbl.srb[idx].bucketSizeDur;
      srbInfo->lchInfo.pbr                = wrSmDfltPrioInfoTbl.srb[idx].pbr;
      srbInfo->lchInfo.priority           = wrSmDfltPrioInfoTbl.srb[idx].lchPriority;
   }
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
   schdCfg->pwrCfg.pucchPwrFmt3aEnable = wrSmDfltPucchPwrFmt3AEnable;
   schdCfg->pwrCfg.puschPwrFmt3aEnable = wrSmDfltPuschPwrFmt3AEnable;
   schdCfg->pwrCfg.isUlGrpPwrCntrlPucchEnable = wrSmDfltisUlGrpPwrCntrlPucchEnable;
   schdCfg->pwrCfg.isUlGrpPwrCntrlPuschEnable = wrSmDfltisUlGrpPwrCntrlPuschEnable;
   if((wrHoDedPreamCount) && 
      (wrSmDfltRrmNumPreambles - wrHoDedPreamCount))   
   {/* Assigning the remaining ded preambles to MAC for
       * PDCCH order RACH */
      schdCfg->preamPres  = TRUE;
      schdCfg->preamStart = wrSmDfltRrmStartPreamble + wrHoDedPreamCount;
      schdCfg->preamSize  = wrSmDfltRrmNumPreambles - wrHoDedPreamCount;
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
   schdCfg->genCfg.maxUePerUlSf = wrSmDfltMaxUePerUlSf;
   schdCfg->genCfg.maxUePerDlSf = wrSmDfltMaxUePerDlSf;
   schdCfg->genCfg.maxUlBwPerUe = wrSmDfltMaxUlBwPerUe;
   schdCfg->genCfg.maxDlBwPerUe = wrSmDfltMaxDlBwPerUe;
   schdCfg->genCfg.maxDlRetxBw = wrSmDfltMaxDlRetxBw;
   schdCfg->genCfg.maxDlUeNewTxPerTti = wrSmDfltMaxDlUeNwTxPerTti;
   schdCfg->genCfg.maxUlUeNewTxPerTti = wrSmDfltMaxUlUeNwTxPerTti;
   schdCfg->genCfg.cellModSchm = wrSmDfltCellModSchm;
   /* SPS changes starts */
   /* By default the SPS will be disabled in the wrSmDfltIsSpsEnabled */
   schdCfg->spsCfg.bIsSpsEnabled = wrSmDfltIsSpsEnabled;
   if (schdCfg->spsCfg.bIsSpsEnabled == TRUE)
   {
      schdCfg->spsCfg.maxDlSpsUePerTti = wrSmDfltMaxDlSpsUePerTti;
      schdCfg->spsCfg.maxUlSpsUePerTti = wrSmDfltMaxUlSpsUePerTti;
      schdCfg->spsCfg.maxDlBwInRbs = wrSmDfltSpsDlBandwidth;
   }
   /* SPS changes ends */
   schdCfg->isDynCfiEnb = wrSmDfltIsDcfiEnabled;
   cfgGrpIndex++;


   lwrMng->t.protoCfg.t.cellCfg.t.addCell.cellCfgGrp[cfgGrpIndex]->grpType = WR_CELL_CFG_GRP_ULSCHD;
   ulSchdCfgGrp = &lwrMng->t.protoCfg.t.cellCfg.t.addCell.cellCfgGrp[cfgGrpIndex]->t.ulSchdCfgGrp;
   ulSchdCfgGrp->ulSchedulingAlg = wrSmDfltUlSchdType;
   ulSchdCfgGrp->pfsUlTptCoefficient = wrSmDfltPfsUlTptCoefficient;
   ulSchdCfgGrp->pfsUlFairCoefficient = wrSmDfltPfsUlFairCoefficient;
   for(idx = 0; idx < WR_MAX_QCI; idx++)
   {
      ulSchdCfgGrp->ulQciWgtList[idx] = wrSmDfltUlQciWgt[idx];
   }
   ulSchdCfgGrp->maxUePerUlSf = wrSmDfltMaxUePerUlSf;
   ulSchdCfgGrp->maxUlUeNewTxPerTti = wrSmDfltMaxUlUeNwTxPerTti;
   cfgGrpIndex++; 

   lwrMng->t.protoCfg.t.cellCfg.t.addCell.cellCfgGrp[cfgGrpIndex]->grpType = WR_CELL_CFG_GRP_DLSCHD;
   dlSchdCfgGrp = &lwrMng->t.protoCfg.t.cellCfg.t.addCell.cellCfgGrp[cfgGrpIndex]->t.dlSchdCfgGrp;
   dlSchdCfgGrp->dlSchedulingAlg = wrSmDfltDlSchdType;
   dlSchdCfgGrp->pfsDlTptCoefficient = wrSmDfltPfsDlTptCoefficient;
   dlSchdCfgGrp->pfsDlFairCoefficient = wrSmDfltPfsDlFairCoefficient;
   for(idx = 0; idx < WR_MAX_QCI; idx++)
   {
      dlSchdCfgGrp->dlQciWgtList[idx] = wrSmDfltDlQciWgt[idx];
   }
   dlSchdCfgGrp->maxUePerDlSf = wrSmDfltMaxUePerDlSf;
   dlSchdCfgGrp->maxDlUeNewTxPerTti = wrSmDfltMaxDlUeNwTxPerTti;
   dlSchdCfgGrp->dlfsSchdType = wrSmDfltDLFSSchdTyp;
   cfgGrpIndex++; 

   lwrMng->t.protoCfg.t.cellCfg.t.addCell.numOfGrps = cfgGrpIndex;
   enbCfg->txPrefMode = wrSmDfltTransModeUse;
   enbCfg->isAutoCfgModeEnb = wrSmDfltTmAutoConfig;

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
 *        Fun:   smBuildCellCfg
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

   TRC2(smFillX2InitCntrl);

   for ( noOfCfg=0; noOfCfg<smCfgCb.noOfNghCfg; noOfCfg++)
   {
      cntrl->t.cntrl.idLst[noOfCfg] = smCfgCb.wrNghCellCfg[noOfCfg]->peerId;
   }
   cntrl->t.cntrl.numConn = noOfCfg;

   RETVALUE(ROK);
} /* end of smFillX2InitCntrl */    

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
U32    idx,
LwrMngmt *lwrMng
)
#else /* if ANSI not defined*/
PUBLIC S16 smBuildDynWrNbrEnbCfg(idx,lwrMng)
U32    idx;
LwrMngmt *lwrMng;
#endif /* end of ANSI*/
{
   TRC2(smBuildDynWrNbrEnbCfg);

   lwrMng->t.nbrAddCfm.enbId = smCfgCb.wrNghCellCfg[idx]->enbId;
   lwrMng->t.nbrAddCfm.peerId =  smCfgCb.wrNghCellCfg[idx]->peerId -1;
   wrCpyCmTptAddr(&lwrMng->t.nbrAddCfm.ipAddr,
      &smCfgCb.wrNghCellCfg[idx]->ipAddr);
   RETVALUE(ROK);
}/*smBuildDynWrNbrEnbCfg*/

/*************************************** SIB Modification changes ********************/
#ifdef ANSI
PUBLIC S16 smBuildDynWrNehCellCfg
(
LwrMngmt *lwrMng
)
#else /* ANSI not defined */
PUBLIC S16 smBuildDynWrNehCellCfg(*lwrMng)
LwrMngmt *lwrMng;
#endif /* end of ANSI */
{
   LwrEUTRANeighCell *eutraNeghCell = NULLP;
   U32 idx; 

   TRC2(smBuildDynWrNehCellCfg);

   lwrMng->t.protoCfg.type = WR_CFG_TYPE_NEIGH_CELL;
   lwrMng->t.protoCfg.t.neighCellCfg.action = WR_ACT_ADD;
   lwrMng->t.protoCfg.t.neighCellCfg.cellId = 1;
   lwrMng->t.protoCfg.t.neighCellCfg.numCells = 2;

   for(idx = 0; idx < lwrMng->t.protoCfg.t.neighCellCfg.numCells; idx++)
   {
      lwrMng->t.protoCfg.t.neighCellCfg.t.neighAddCfg[idx].type = 
         WR_NEIGH_CELL_EUTRA;
      eutraNeghCell = &(lwrMng->t.protoCfg.t.neighCellCfg.t.neighAddCfg[idx].
            t.eutraCell);
      eutraNeghCell->cellNum = 3;
      eutraNeghCell->nrIndex = 2;
      eutraNeghCell->enbId = 36103 + idx;

      eutraNeghCell->cio = wrSmDfltCIO;
      eutraNeghCell->enbPlmn.numMncDigits = 2;
      eutraNeghCell->enbPlmn.mcc[0] = 4;
      eutraNeghCell->enbPlmn.mcc[1] = 5;
      eutraNeghCell->enbPlmn.mcc[2] = 0;

      eutraNeghCell->enbPlmn.mnc[0] = 0;
      eutraNeghCell->enbPlmn.mnc[1] = 5;
      eutraNeghCell->enbPlmn.mnc[2] = -1;

      eutraNeghCell->enbType = wrSmDfltNeighEnbType;
      eutraNeghCell->numPlmn = wrSmDfltNeighCellNoPlmn;

      eutraNeghCell->pci = (U16)(142 + idx);
      eutraNeghCell->tac = 1;

      eutraNeghCell->qoffset = wrSmDfltQoff;

      eutraNeghCell->dlBw = wrSmDfltCellDlBW;
      eutraNeghCell->earfcnDl = 2500;
      eutraNeghCell->earfcnUl= 20500;

      eutraNeghCell++;
   }   
   RETVALUE(ROK);
} /* end of smBuildDynWrNehCellCfg */

#ifdef ANSI
PUBLIC S16 smSndDynWrNhCellCfg
(
void
)
#else /* ANSI not defined */
PUBLIC S16 smSndDynWrNhCellCfg()
#endif /* end of ANSI */
{
   LwrMngmt *lwrMng = NULLP;
   Pst pst;
   
   TRC2(smSndDynWrNhCellCfg);
   
   WR_ALLOC(&lwrMng, sizeof(LwrMngmt));
   SM_SET_ZERO(&pst, sizeof(Pst));
   
   if(lwrMng == NULLP)
   {
      RETVALUE(RFAILED);
   }
   smBuildDynWrNehCellCfg(lwrMng);
   
   lwrMng->hdr.msgType             = TCFG;
   lwrMng->hdr.msgLen              = 0;
   lwrMng->hdr.entId.ent           = ENTWR;
   lwrMng->hdr.entId.inst          = SM_INST_ZERO;
   lwrMng->hdr.elmId.elmnt         = STWRPROTOCFG;
   lwrMng->hdr.response.selector   = SM_SELECTOR_LWLC;

    /* Fill Pst */
   pst.selector  = SM_SELECTOR_LWLC;
   pst.srcEnt    = ENTSM;
   pst.dstEnt    = ENTWR;
   pst.dstProcId = SM_WR_PROC;
   pst.srcProcId = SM_SM_PROC;

   (Void) SmMiLwrCfgReq(&pst, lwrMng);
   
   RETVALUE(ROK);
} /* end of smSndDynWrNhCellCfg */

#ifdef ANSI
PUBLIC S16 smBuildDynWrNehCellDel
(
LwrMngmt *lwrMng
)
#else /* ANSI not defined */
PUBLIC S16 smBuildDynWrNehCellCfg(*lwrMng)
LwrMngmt *lwrMng;
#endif /* end of ANSI */
{
   LwrEutranCellDelCfg *eutraNeghCell = NULLP;
   U32 idx = 0;

   TRC2(smBuildDynWrNehCellDel);

   lwrMng->t.protoCfg.type = WR_CFG_TYPE_NEIGH_CELL;
   lwrMng->t.protoCfg.t.neighCellCfg.action = WR_ACT_DEL;
   lwrMng->t.protoCfg.t.neighCellCfg.cellId = 1;
   lwrMng->t.protoCfg.t.neighCellCfg.numCells = 2;

   for(idx = 0; idx < lwrMng->t.protoCfg.t.neighCellCfg.numCells; idx++)
   {
      lwrMng->t.protoCfg.t.neighCellCfg.t.neighDelCfg[idx].type = 
         WR_NEIGH_CELL_EUTRA;
      eutraNeghCell = &(lwrMng->t.protoCfg.t.neighCellCfg.t.neighDelCfg[idx].
            t.eutranCell);
      eutraNeghCell->nrIndex = 2;
      eutraNeghCell->phyCellId = (U16)(142 + idx);
      eutraNeghCell->earfcn = 2500;
   }

   RETVALUE(ROK);
} /* end of smBuildDynWrNehCellDel */

#ifdef ANSI
PUBLIC S16 smSndDynWrNhCellDel
(
void
)
#else /* ANSI not defined */
PUBLIC S16 smSndDynWrNhCellCfg()
#endif /* end of ANSI */
{
   LwrMngmt *lwrMng;
   Pst pst;
   
   TRC2(smSndDynWrNhCellDel);
   
   SM_SET_ZERO(&pst, sizeof(Pst));
   WR_ALLOC(&lwrMng, sizeof(LwrMngmt));
   
   if(lwrMng == NULLP)
   {
      RETVALUE(RFAILED);
   }
   smBuildDynWrNehCellDel(lwrMng);
   
   lwrMng->hdr.msgType             = TCFG;
   lwrMng->hdr.msgLen              = 0;
   lwrMng->hdr.entId.ent           = ENTWR;
   lwrMng->hdr.entId.inst          = SM_INST_ZERO;
   lwrMng->hdr.elmId.elmnt         = STWRPROTOCFG;
   lwrMng->hdr.response.selector   = SM_SELECTOR_LWLC;

    /* Fill Pst */
   pst.selector  = SM_SELECTOR_LWLC;
   pst.srcEnt    = ENTSM;
   pst.dstEnt    = ENTWR;
   pst.dstProcId = SM_WR_PROC;
   pst.srcProcId = SM_SM_PROC;

   (Void) SmMiLwrCfgReq(&pst, lwrMng);
   
   RETVALUE(ROK);
} /* end of smSndDynWrNhCellDel */

PRIVATE S16 smDynUtraFreqMod
(
LwrMngmt                     *lwrmng
)
{
   LwrNeighFreqAddModCfg *nrfreqcfg =NULLP;

   TRC2(smDynUtraFreqMod);

   lwrmng->t.protoCfg.type = WR_CFG_TYPE_NEIGH_FREQ;
   lwrmng->t.protoCfg.t.neighFreqCfg.action = WR_ACT_MOD;
   lwrmng->t.protoCfg.t.neighFreqCfg.cellId = 1; /* this is owncellid in case of pld based conf, jst a logic entity */

   lwrmng->t.protoCfg.t.neighFreqCfg.numFreq = 1;
   nrfreqcfg = lwrmng->t.protoCfg.t.neighFreqCfg.t.modNeighFreq;
   /* TODO: Currently evrythning is being hardcoded, so have to check whether Dynamic Mod. feature changes this part */
   nrfreqcfg->type = WR_NEIGH_CELL_UTRA_FDD;
   nrfreqcfg->t.utranFreq.reCfgType = 0x000fff;
   nrfreqcfg->t.utranFreq.freqIndex = 1;
   nrfreqcfg->t.utranFreq.t.utraFdd.arfcnUl = 9764;
   nrfreqcfg->t.utranFreq.t.utraFdd.arfcnDl = 10726;
   nrfreqcfg->t.utranFreq.priority.pres = PRSNT_NODEF;
   nrfreqcfg->t.utranFreq.priority.val = 2;
   nrfreqcfg->t.utranFreq.threshXhigh = 31;
   nrfreqcfg->t.utranFreq.threshXlow = 31;
   nrfreqcfg->t.utranFreq.qRxLevMin = -50;
   nrfreqcfg->t.utranFreq.pMaxUtra = 1;
   nrfreqcfg->t.utranFreq.t.utraFdd.pQualMin = -16;
   nrfreqcfg->t.utranFreq.offsetFreq = 30; /* changed */
   nrfreqcfg->t.utranFreq.duplexMode = 0;
   nrfreqcfg->t.utranFreq.t.utraFdd.thresR9.pres = PRSNT_NODEF;
   nrfreqcfg->t.utranFreq.t.utraFdd.thresR9.thresXHighQR9 = 31;
   nrfreqcfg->t.utranFreq.t.utraFdd.thresR9.thresXLowQR9 = 31;

   RETVALUE(ROK);
} /* end of smDynUtraFreqMod */

PRIVATE S16 smDynUtraFreqAdd
(
LwrMngmt                     *lwrmng
)
{
   LwrNeighFreqAddModCfg *nrfreqcfg =NULLP;

   TRC2(smDynUtraFreqAdd);

   lwrmng->t.protoCfg.type = WR_CFG_TYPE_NEIGH_FREQ;
   lwrmng->t.protoCfg.t.neighFreqCfg.action = WR_ACT_ADD;
   lwrmng->t.protoCfg.t.neighFreqCfg.cellId = 1; /* this is owncellid in case of pld based conf, jst a logic entity */

   lwrmng->t.protoCfg.t.neighFreqCfg.numFreq = 1;
   nrfreqcfg =lwrmng->t.protoCfg.t.neighFreqCfg.t.addNeighFreq;
   /* TODO: Currently evrythning is being hardcoded, so have to check whether Dynamic Mod. feature changes this part */
   nrfreqcfg->type = WR_NEIGH_CELL_UTRA_FDD;

   nrfreqcfg->t.utranFreq.reCfgType = 0x000fff;
   nrfreqcfg->t.utranFreq.freqIndex = 1;
   nrfreqcfg->t.utranFreq.t.utraFdd.arfcnUl = 9764;
   nrfreqcfg->t.utranFreq.t.utraFdd.arfcnDl = 10726;
   nrfreqcfg->t.utranFreq.priority.pres = PRSNT_NODEF;
   nrfreqcfg->t.utranFreq.priority.val = 2;
   nrfreqcfg->t.utranFreq.threshXhigh = 31;
   nrfreqcfg->t.utranFreq.threshXlow = 31;
   nrfreqcfg->t.utranFreq.qRxLevMin = -50;
   nrfreqcfg->t.utranFreq.pMaxUtra = 1;
   nrfreqcfg->t.utranFreq.t.utraFdd.pQualMin = -16;
   nrfreqcfg->t.utranFreq.offsetFreq = 15;
   nrfreqcfg->t.utranFreq.duplexMode = 0;
   nrfreqcfg->t.utranFreq.t.utraFdd.thresR9.pres = PRSNT_NODEF;
   nrfreqcfg->t.utranFreq.t.utraFdd.thresR9.thresXHighQR9 = 31;
   nrfreqcfg->t.utranFreq.t.utraFdd.thresR9.thresXLowQR9 = 31;

   RETVALUE(ROK);
} /* end of smDynUtraFreqAdd */

PRIVATE S16 smDynUtraFreqDel
(
LwrMngmt                     *lwrmng
)
{
   LwrNeighFreqDelCfg *nrfreqcfg =NULLP;

   TRC2(smDynUtraFreqDel);

   lwrmng->t.protoCfg.type = WR_CFG_TYPE_NEIGH_FREQ;
   lwrmng->t.protoCfg.t.neighFreqCfg.action = WR_ACT_DEL;
   lwrmng->t.protoCfg.t.neighFreqCfg.cellId = 1; /* this is owncellid in case of pld based conf, jst a logic entity */

   lwrmng->t.protoCfg.t.neighFreqCfg.numFreq = 1;
   nrfreqcfg =lwrmng->t.protoCfg.t.neighFreqCfg.t.delNeighFreq;
   nrfreqcfg->type = WR_NEIGH_CELL_UTRA_FDD;
   nrfreqcfg->t.utranFreq.freqIndex = 5;
   nrfreqcfg->t.utranFreq.arfcn = 10726;

   RETVALUE(ROK);
} /* end of smDynUtraFreqDel */

PUBLIC S16 smDynWrNrUtraFreqCfg
(
U8                           type
)
{
   LwrMngmt *lwrMng;
   Pst pst;

   TRC2(smDynWrNrUtraFreqCfg);

   SM_SET_ZERO(&pst, sizeof(Pst));
   WR_ALLOC(&lwrMng, sizeof(LwrMngmt));

   if(lwrMng == NULLP)
   {
      RETVALUE(RFAILED);
   }
   if(type == 1)
   {
      /* add new neighbor */
      smDynUtraFreqAdd(lwrMng);
   }
   else if (type == 2)
   {
      /* modify existing neighbor */
      smDynUtraFreqMod(lwrMng);
   }
   else
   {
      /* remove existing neighbor */
      smDynUtraFreqDel(lwrMng);
   }
   
   lwrMng->hdr.msgType             = TCFG;
   lwrMng->hdr.msgLen              = 0;
   lwrMng->hdr.entId.ent           = ENTWR;
   lwrMng->hdr.entId.inst          = SM_INST_ZERO;
   lwrMng->hdr.elmId.elmnt         = STWRPROTOCFG;
   lwrMng->hdr.response.selector   = SM_SELECTOR_LWLC;

   /* Fill Pst */
   pst.selector  = SM_SELECTOR_LWLC;
   pst.srcEnt    = ENTSM;
   pst.dstEnt    = ENTWR;
   pst.dstProcId = SM_WR_PROC;
   pst.srcProcId = SM_SM_PROC;

   (Void) SmMiLwrCfgReq(&pst, lwrMng);

   RETVALUE(ROK);
} /* end of smDynWrNrUtraFreqCfg */

/*
*
*       Fun:   smSndWrCellModSib6
*
*       Desc:  This function for sending eNodeB SIB6 Modification to LWR
*
*       Ret:   ROK
*
*       Notes: None
*              
*       File:  wr_sm_wr_cntrl.c 
*
*/
#ifdef ANSI
PUBLIC S16 smSndWrCellModSib6
(
)
#else /* ANSI not defined */
PUBLIC S16 smSndWrCellModSib6()
#endif /* end of ANSI */
{
   LwrMngmt          *lwrMng = NULLP;
   Pst               pst;
   LwrCellSib6CfgGrp *sib6 =NULLP;
   U8                cfgGrpIndex  = 0;

   TRC2(smSndWrCellModSib6);

   WR_ALLOC(&lwrMng, sizeof(LwrMngmt));
   SM_SET_ZERO(&pst, sizeof(Pst));

   if(lwrMng == NULLP)
   {
      RETVALUE(RFAILED);
   }
   lwrMng->t.protoCfg.type = WR_CFG_TYPE_CELL;
   lwrMng->t.protoCfg.t.cellCfg.action = WR_ACT_MOD;

   lwrMng->t.protoCfg.t.cellCfg.t.modCell.cellId = wrSmDfltCellId;
   lwrMng->t.protoCfg.t.cellCfg.t.modCell.cellIndex = WR_DFLT_CELL_IDX;
   lwrMng->t.protoCfg.t.cellCfg.t.modCell.reCfgType = LWR_CELL_CFG_GRP;
   /* mem_issue_fix: Allocating memory for all the cell group though
    * all the groups are not being used as of now. This helps to avoid 
    * multiple allocation and can have a common logic to free the memory */
   WR_ALLOC(&(lwrMng->t.protoCfg.t.cellCfg.t.modCell.cellCfgGrp[cfgGrpIndex]), \
                       (sizeof(LwrCellCfgGrp)));
   if(NULLP == lwrMng->t.protoCfg.t.cellCfg.t.modCell.cellCfgGrp[cfgGrpIndex])
   {
      RLOG0(L_FATAL, "Memory allocation failed for modcellCfg");
      RETVALUE(RFAILED);
   }

   /*Filling Sib6 params*/
   lwrMng->t.protoCfg.t.cellCfg.t.modCell.cellCfgGrp[cfgGrpIndex]->grpType
                                                        = WR_CELL_CFG_GRP_SIB6;

   sib6 =&(lwrMng->t.protoCfg.t.cellCfg.t.modCell.cellCfgGrp[cfgGrpIndex]->t.
         sib6CfgGrp);
   sib6->reCfgType = LWR_UTRAN_SIB6_TRESELECT;
   sib6->tReselection= 2;

   cfgGrpIndex++; 
   lwrMng->t.protoCfg.t.cellCfg.t.modCell.numOfGrps = cfgGrpIndex;
   lwrMng->hdr.msgType             = TCFG;
   lwrMng->hdr.msgLen              = 0;
   lwrMng->hdr.entId.ent           = ENTWR;
   lwrMng->hdr.entId.inst          = SM_INST_ZERO;
   lwrMng->hdr.elmId.elmnt         = STWRPROTOCFG;

    /* Fill Pst */
   pst.selector  = SM_SELECTOR_LWLC;
   pst.srcEnt    = ENTSM;
   pst.dstEnt    = ENTWR;
   pst.dstProcId = SM_WR_PROC;
   pst.srcProcId = SM_SM_PROC;

   (Void) SmMiLwrCfgReq(&pst, lwrMng);
   
   RETVALUE(ROK);
} /* end of smSndWrCellModSib6 */                                            

/*
*
*       Fun:   smSndWrCellModSib9
*
*       Desc:  This function for sending eNodeB SIB9 Modification to LWR
*
*       Ret:   ROK
*
*       Notes: None
*              
*       File:  wr_sm_wr_cntrl.c 
*
*/
#ifdef ANSI
PUBLIC S16 smSndWrCellModSib9
(
)
#else /* ANSI not defined */
PUBLIC S16 smSndWrCellModSib9()
#endif /* end of ANSI */
{
   LwrMngmt          *lwrMng = NULLP;
   Pst               pst;
   LwrCellSib9CfgGrp *sib9 =NULLP;
   U8                cfgGrpIndex  = 0;
   
   TRC2(smSndWrCellModSib9);

   WR_ALLOC(&lwrMng, sizeof(LwrMngmt));
   SM_SET_ZERO(&pst, sizeof(Pst));

   if(lwrMng == NULLP)
   {
      RETVALUE(RFAILED);
   }
   lwrMng->t.protoCfg.type = WR_CFG_TYPE_CELL;
   lwrMng->t.protoCfg.t.cellCfg.action = WR_ACT_MOD;

   lwrMng->t.protoCfg.t.cellCfg.t.modCell.cellId = wrSmDfltCellId;
   lwrMng->t.protoCfg.t.cellCfg.t.modCell.cellIndex = WR_DFLT_CELL_IDX;
   lwrMng->t.protoCfg.t.cellCfg.t.modCell.reCfgType = LWR_CELL_CFG_GRP;

   WR_ALLOC(&(lwrMng->t.protoCfg.t.cellCfg.t.modCell.cellCfgGrp[cfgGrpIndex]), \
                       (sizeof(LwrCellCfgGrp)));
   if(NULLP == lwrMng->t.protoCfg.t.cellCfg.t.modCell.cellCfgGrp[cfgGrpIndex])
   {
      RLOG0(L_FATAL, "Memory allocation failed for modCellCfgGroup");
      RETVALUE(RFAILED);
   }

   /*Filling Sib9 params*/
   lwrMng->t.protoCfg.t.cellCfg.t.modCell.cellCfgGrp[cfgGrpIndex]->grpType
                                                        = WR_CELL_CFG_GRP_SIB9;

   sib9 =&(lwrMng->t.protoCfg.t.cellCfg.t.modCell.cellCfgGrp[cfgGrpIndex]->t.
         sib9CfgGrp);

   cmMemcpy(sib9->henbName, (U8 *)"SKT-Pangyo", cmStrlen((U8 *)"SKT-Pangyo"));

   cfgGrpIndex++; 
   lwrMng->t.protoCfg.t.cellCfg.t.modCell.numOfGrps = cfgGrpIndex;
   lwrMng->hdr.msgType             = TCFG;
   lwrMng->hdr.msgLen              = 0;
   lwrMng->hdr.entId.ent           = ENTWR;
   lwrMng->hdr.entId.inst          = SM_INST_ZERO;
   lwrMng->hdr.elmId.elmnt         = STWRPROTOCFG;

    /* Fill Pst */
   pst.selector  = SM_SELECTOR_LWLC;
   pst.srcEnt    = ENTSM;
   pst.dstEnt    = ENTWR;
   pst.dstProcId = SM_WR_PROC;
   pst.srcProcId = SM_SM_PROC;

   (Void) SmMiLwrCfgReq(&pst, lwrMng);
   
   RETVALUE(ROK);
} /* end of smSndWrCellModSib9 */                                       

/*
*
*       Fun:  smModBuildWrNehCellCfg 
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
PUBLIC S16 smModBuildWrNehCellCfg
(
LwrMngmt *lwrMng
)
#else /* ANSI not defined */
PUBLIC S16 smModBuildWrNehCellCfg(*lwrMng)
LwrMngmt *lwrMng;
#endif /* end of ANSI */
{
   LwrEUTRANeighCell *eutraNeghCell = NULLP;
   U32 idx; 

   TRC2(smModBuildWrNehCellCfg);

   lwrMng->t.protoCfg.type = WR_CFG_TYPE_NEIGH_CELL;
   lwrMng->t.protoCfg.t.neighCellCfg.action = WR_ACT_MOD;
   lwrMng->t.protoCfg.t.neighCellCfg.cellId = 1; /* this is ownCellID in case of PLD, jst a logical entity */
   lwrMng->t.protoCfg.t.neighCellCfg.numCells = (U16)wrSmDfltNumCells;

   for(idx=0;idx<wrSmDfltNumCells;idx++)
   {
      lwrMng->t.protoCfg.t.neighCellCfg.t.neighModCfg[idx].type = 
         WR_NEIGH_CELL_EUTRA;
      eutraNeghCell =&(lwrMng->t.protoCfg.t.neighCellCfg.t.neighModCfg[idx].
            t.eutraCell);
      eutraNeghCell->cellNum = wrSmDfltNeigCellNum[idx];
      eutraNeghCell->nrIndex = wrSmDfltNriIndex[idx];
      eutraNeghCell->enbId = wrSmDfltNeighEnbID[idx];

      eutraNeghCell->cio =wrSmDfltCIO;
      eutraNeghCell->enbPlmn.numMncDigits = wrSmDfltNeEnbNumMncDigits;
      eutraNeghCell->enbPlmn.mcc[0] =wrSmDfltNeEnbPlmn1Mcc0[idx];
      eutraNeghCell->enbPlmn.mcc[1] =wrSmDfltNeEnbPlmn1Mcc1[idx];
      eutraNeghCell->enbPlmn.mcc[2] =wrSmDfltNeEnbPlmn1Mcc2[idx];

      eutraNeghCell->enbPlmn.mnc[0] =wrSmDfltNeEnbPlmn1Mnc0[idx];
      eutraNeghCell->enbPlmn.mnc[1] =wrSmDfltNeEnbPlmn1Mnc1[idx];
      eutraNeghCell->enbPlmn.mnc[2] =wrSmDfltNeEnbPlmn1Mnc2[idx];

      eutraNeghCell->enbType = wrSmDfltNeighEnbType;
      eutraNeghCell->numPlmn = wrSmDfltNeighCellNoPlmn;

      eutraNeghCell->pci = wrSmDfltNeighCellPci[idx];
      eutraNeghCell->tac = wrSmDfltNeighCellTAC[idx];

      eutraNeghCell->qoffset = LWR_OFFSET_DB_MINUS_8;

      eutraNeghCell->dlBw = wrSmDfltCellDlBW;
      eutraNeghCell->earfcnDl = wrSmDfltEartcnNrDLEarfcn[idx];
      eutraNeghCell->earfcnUl= wrSmDfltUlEarFcn;

      eutraNeghCell++;
   }   
   RETVALUE(ROK);
} /*end of smModBuildWrNehCellCfg */

#ifdef ANSI
PUBLIC S16 smModWrNhCellCfg
(
void
)
#else /* ANSI not defined */
PRIVATE S16 smModWrNhCellCfg()
#endif /* end of ANSI */
{
   LwrMngmt *lwrMng = NULLP;
   Pst pst;
   
   TRC2(smModWrNhCellCfg);
   
   WR_ALLOC(&lwrMng, sizeof(LwrMngmt));
   SM_SET_ZERO(&pst, sizeof(Pst));
   
   if(lwrMng == NULLP)
   {
      RETVALUE(RFAILED);
   }
   smModBuildWrNehCellCfg(lwrMng);
   
   lwrMng->hdr.msgType             = TCFG;
   lwrMng->hdr.msgLen              = 0;
   lwrMng->hdr.entId.ent           = ENTWR;
   lwrMng->hdr.entId.inst          = SM_INST_ZERO;
   lwrMng->hdr.elmId.elmnt         = STWRPROTOCFG;
   lwrMng->hdr.response.selector   = SM_SELECTOR_LWLC;

    /* Fill Pst */
   pst.selector  = SM_SELECTOR_LWLC;
   pst.srcEnt    = ENTSM;
   pst.dstEnt    = ENTWR;
   pst.dstProcId = SM_WR_PROC;
   pst.srcProcId = SM_SM_PROC;

   (Void) SmMiLwrCfgReq(&pst, lwrMng);
   
   RETVALUE(ROK);
} /* end of smModWrNhCellCfg */

/*
*
*       Fun:   smSndWrCellModSib4
*
*       Desc:  This function for sending eNodeB SIB4 Modification to LWR
*
*       Ret:   ROK
*
*       Notes: None
*              
*       File:  wr_sm_wr_cntrl.c 
*
*/
#ifdef ANSI
PUBLIC S16 smSndWrCellModSib4
(
)
#else /* ANSI not defined */
PUBLIC S16 smSndWrCellModSib4()
#endif /* end of ANSI */
{
   LwrMngmt          *lwrMng= NULLP;
   Pst               pst;
   LwrCellSib4CfgGrp *sib4 =NULLP;
   U8                cfgGrpIndex  = 0;

   TRC2(smSndWrCellModSib4);

   WR_ALLOC(&lwrMng, sizeof(LwrMngmt));
   SM_SET_ZERO(&pst, sizeof(Pst));

   if(lwrMng == NULLP)
   {
      RETVALUE(RFAILED);
   }
   lwrMng->t.protoCfg.type = WR_CFG_TYPE_CELL;
   lwrMng->t.protoCfg.t.cellCfg.action = WR_ACT_MOD;

   lwrMng->t.protoCfg.t.cellCfg.t.modCell.cellId = wrSmDfltCellId;
   lwrMng->t.protoCfg.t.cellCfg.t.modCell.cellIndex = WR_DFLT_CELL_IDX;
   lwrMng->t.protoCfg.t.cellCfg.t.modCell.reCfgType = LWR_CELL_CFG_GRP;
   
   WR_ALLOC(&(lwrMng->t.protoCfg.t.cellCfg.t.modCell.cellCfgGrp[cfgGrpIndex]), \
                       (sizeof(LwrCellCfgGrp)));
   if(NULLP == lwrMng->t.protoCfg.t.cellCfg.t.modCell.cellCfgGrp[cfgGrpIndex])
   {
      RLOG0(L_FATAL, "Memory allocation failed for celll modification");
      RETVALUE(RFAILED);
   }
   /*Filling Sib4 params*/
   lwrMng->t.protoCfg.t.cellCfg.t.modCell.cellCfgGrp[cfgGrpIndex]->grpType
                                                        = WR_CELL_CFG_GRP_SIB4;

   sib4 =&(lwrMng->t.protoCfg.t.cellCfg.t.modCell.cellCfgGrp[cfgGrpIndex]->t.
         sib4CfgGrp);

   sib4->reCfgType = LWR_CELL_SIB3_CFG_CSG_INFO;

   sib4->csgInfo.csgPciRange = 0;
   sib4->csgInfo.isCsgPciCfgd =0;
   /*sib4->csgInfo.csgPciType = 0;*/
   sib4->csgInfo.csgStartPhyCellId = 0;

   cfgGrpIndex++; 
   lwrMng->t.protoCfg.t.cellCfg.t.modCell.numOfGrps = cfgGrpIndex;
   lwrMng->hdr.msgType             = TCFG;
   lwrMng->hdr.msgLen              = 0;
   lwrMng->hdr.entId.ent           = ENTWR;
   lwrMng->hdr.entId.inst          = SM_INST_ZERO;
   lwrMng->hdr.elmId.elmnt         = STWRPROTOCFG;

    /* Fill Pst */
   pst.selector  = SM_SELECTOR_LWLC;
   pst.srcEnt    = ENTSM;
   pst.dstEnt    = ENTWR;
   pst.dstProcId = SM_WR_PROC;
   pst.srcProcId = SM_SM_PROC;

   (Void) SmMiLwrCfgReq(&pst, lwrMng);
   
   RETVALUE(ROK);
} /* end of smSndWrCellModSib4 */  

PRIVATE S16 smDynEutraFreqMod
(
LwrMngmt                     *lwrMng
)
{
   LwrNeighFreqAddModCfg *nrFreqCfg =NULLP;

   TRC2(smDynEutraFreqMod);

   lwrMng->t.protoCfg.type = WR_CFG_TYPE_NEIGH_FREQ;
   lwrMng->t.protoCfg.t.neighFreqCfg.action = WR_ACT_MOD;
   lwrMng->t.protoCfg.t.neighFreqCfg.cellId = 1; /* this is ownCellID in case of PLD based, jst a logic entity */

   lwrMng->t.protoCfg.t.neighFreqCfg.numFreq = 1;
   nrFreqCfg = (lwrMng->t.protoCfg.t.neighFreqCfg.t.modNeighFreq);
   nrFreqCfg->type = WR_NEIGH_CELL_EUTRA;
   nrFreqCfg->t.eutranFreq.reCfgType = wrSmDfltReCfgType;
   nrFreqCfg->t.eutranFreq.status = wrSmDfltStatus;
   nrFreqCfg->t.eutranFreq.earfcnUl = wrSmDfltUlEarFcn;
   nrFreqCfg->t.eutranFreq.earfcnDl = 5250;
   nrFreqCfg->t.eutranFreq.priority = wrSmDfltPriority;
   nrFreqCfg->t.eutranFreq.qRxLevMin = -60; /* changed */
   nrFreqCfg->t.eutranFreq.pMaxUsage = wrSmDfltPMaxUsage;
   nrFreqCfg->t.eutranFreq.pMax = wrSmDfltS3pMax;
   nrFreqCfg->t.eutranFreq.tReselection = wrSmDfltTReselection;
   nrFreqCfg->t.eutranFreq.sfUsage = wrSmDfltSfUsage;
   nrFreqCfg->t.eutranFreq.tReselectionSFMed = wrSmDfltTReselectionSFMed;
   nrFreqCfg->t.eutranFreq.tReselectionSFHigh = wrSmDfltTReselectionSFHigh;
   nrFreqCfg->t.eutranFreq.sIntraSearch.pres = PRSNT_NODEF;
   nrFreqCfg->t.eutranFreq.sIntraSearch.val = wrSmDfltSIntraSearch;
   nrFreqCfg->t.eutranFreq.sNonIntraSearch.pres = PRSNT_NODEF;
   nrFreqCfg->t.eutranFreq.sNonIntraSearch.val = 30;
   nrFreqCfg->t.eutranFreq.servThresholdLow = 30;
   nrFreqCfg->t.eutranFreq.threshXHigh = 30;
   nrFreqCfg->t.eutranFreq.threshXLow =  30;
   nrFreqCfg->t.eutranFreq.measBandwidth.pres =  PRSNT_NODEF;
   nrFreqCfg->t.eutranFreq.measBandwidth.val = 4;
   nrFreqCfg->t.eutranFreq.presAntennaPort1 =  wrSmDfltPresAntennaPort1;
   nrFreqCfg->t.eutranFreq.neighCellCfg =  wrSmDfltNeighCellCfg;
   nrFreqCfg->t.eutranFreq.qOffsetFreq = LWR_Q_OFFSET_RANGE_0;
   nrFreqCfg->t.eutranFreq.offsetFreq =  LWR_Q_OFFSET_RANGE_0;
   nrFreqCfg->t.eutranFreq.sIntraSearchRel9.pres =  PRSNT_NODEF;
   nrFreqCfg->t.eutranFreq.sIntraSearchRel9.sSearchPRel9 =wrSmDfltsSearchPRel9;
   nrFreqCfg->t.eutranFreq.sIntraSearchRel9.sSearchQRel9 =wrSmDfltsSearchQRel9;
   nrFreqCfg->t.eutranFreq.sInterSearchRel9.pres = PRSNT_NODEF;
   nrFreqCfg->t.eutranFreq.sInterSearchRel9.sSearchPRel9 =wrSmDfltsSearchPRel9;
   nrFreqCfg->t.eutranFreq.sInterSearchRel9.sSearchQRel9 =wrSmDfltsSearchQRel9;
   nrFreqCfg->t.eutranFreq.qQualMinRel9.pres = PRSNT_NODEF;
   nrFreqCfg->t.eutranFreq.qQualMinRel9.val = wrSmDfltDualMinRel9;
   nrFreqCfg->t.eutranFreq.reselecThreshQRel9.pres = PRSNT_NODEF;
   nrFreqCfg->t.eutranFreq.reselecThreshQRel9.val = wrSmDfltReselecThreshQRel9;
   nrFreqCfg->t.eutranFreq.threshXQRel9.threshXHighQR9 =wrSmDfltThreshXHighQR9;
   nrFreqCfg->t.eutranFreq.threshXQRel9.threshXLowQR9 =  wrSmDfltThreshXLowQR9;

   RETVALUE(ROK);
} /* end of smDynEutraFreqMod */

PRIVATE S16 smDynEutraFreqAdd
(
LwrMngmt                     *lwrMng
)
{
   LwrNeighFreqAddModCfg *nrFreqCfg =NULLP;

   TRC2(smDynEutraFreqAdd);

   lwrMng->t.protoCfg.type = WR_CFG_TYPE_NEIGH_FREQ;
   lwrMng->t.protoCfg.t.neighFreqCfg.action = WR_ACT_ADD;
   lwrMng->t.protoCfg.t.neighFreqCfg.cellId = 1; /* this is ownCellID in case of PLD based, jst a logic entity */

   lwrMng->t.protoCfg.t.neighFreqCfg.numFreq = 1;
   nrFreqCfg = (lwrMng->t.protoCfg.t.neighFreqCfg.t.modNeighFreq);
   nrFreqCfg->type = WR_NEIGH_CELL_EUTRA;
   nrFreqCfg->t.eutranFreq.reCfgType = wrSmDfltReCfgType;
   nrFreqCfg->t.eutranFreq.status = wrSmDfltStatus;
   nrFreqCfg->t.eutranFreq.earfcnUl = wrSmDfltUlEarFcn;
   nrFreqCfg->t.eutranFreq.earfcnDl = 5250;
   nrFreqCfg->t.eutranFreq.priority = wrSmDfltPriority;
   nrFreqCfg->t.eutranFreq.qRxLevMin = wrSmDfltQRxLevMin;
   nrFreqCfg->t.eutranFreq.pMaxUsage = wrSmDfltPMaxUsage;
   nrFreqCfg->t.eutranFreq.pMax = wrSmDfltS3pMax;
   nrFreqCfg->t.eutranFreq.tReselection = wrSmDfltTReselection;
   nrFreqCfg->t.eutranFreq.sfUsage = wrSmDfltSfUsage;
   nrFreqCfg->t.eutranFreq.tReselectionSFMed = wrSmDfltTReselectionSFMed;
   nrFreqCfg->t.eutranFreq.tReselectionSFHigh = wrSmDfltTReselectionSFHigh;
   nrFreqCfg->t.eutranFreq.sIntraSearch.pres = PRSNT_NODEF;
   nrFreqCfg->t.eutranFreq.sIntraSearch.val = wrSmDfltSIntraSearch;
   nrFreqCfg->t.eutranFreq.sNonIntraSearch.pres = PRSNT_NODEF;
   nrFreqCfg->t.eutranFreq.sNonIntraSearch.val = 30;
   nrFreqCfg->t.eutranFreq.servThresholdLow = 30;
   nrFreqCfg->t.eutranFreq.threshXHigh = 30;
   nrFreqCfg->t.eutranFreq.threshXLow =  30;
   nrFreqCfg->t.eutranFreq.measBandwidth.pres =  PRSNT_NODEF;
   nrFreqCfg->t.eutranFreq.measBandwidth.val = 4;
   nrFreqCfg->t.eutranFreq.presAntennaPort1 =  wrSmDfltPresAntennaPort1;
   nrFreqCfg->t.eutranFreq.neighCellCfg =  wrSmDfltNeighCellCfg;
   nrFreqCfg->t.eutranFreq.qOffsetFreq = LWR_Q_OFFSET_RANGE_0;
   nrFreqCfg->t.eutranFreq.offsetFreq =  LWR_Q_OFFSET_RANGE_0;
   nrFreqCfg->t.eutranFreq.sIntraSearchRel9.pres =  PRSNT_NODEF;
   nrFreqCfg->t.eutranFreq.sIntraSearchRel9.sSearchPRel9 =wrSmDfltsSearchPRel9;
   nrFreqCfg->t.eutranFreq.sIntraSearchRel9.sSearchQRel9 =wrSmDfltsSearchQRel9;
   nrFreqCfg->t.eutranFreq.sInterSearchRel9.pres = PRSNT_NODEF;
   nrFreqCfg->t.eutranFreq.sInterSearchRel9.sSearchPRel9 =wrSmDfltsSearchPRel9;
   nrFreqCfg->t.eutranFreq.sInterSearchRel9.sSearchQRel9 =wrSmDfltsSearchQRel9;
   nrFreqCfg->t.eutranFreq.qQualMinRel9.pres = PRSNT_NODEF;
   nrFreqCfg->t.eutranFreq.qQualMinRel9.val = wrSmDfltDualMinRel9;
   nrFreqCfg->t.eutranFreq.reselecThreshQRel9.pres = PRSNT_NODEF;
   nrFreqCfg->t.eutranFreq.reselecThreshQRel9.val = wrSmDfltReselecThreshQRel9;
   nrFreqCfg->t.eutranFreq.threshXQRel9.threshXHighQR9 =wrSmDfltThreshXHighQR9;
   nrFreqCfg->t.eutranFreq.threshXQRel9.threshXLowQR9 =  wrSmDfltThreshXLowQR9;

   RETVALUE(ROK);
} /* end of smDynEutraFreqAdd */

PRIVATE S16 smDynEutraFreqDel
(
LwrMngmt                     *lwrMng
)
{
   LwrNeighFreqDelCfg *nrFreqCfg = NULLP;

   TRC2(smDynEutraFreqDel);

   lwrMng->t.protoCfg.type = WR_CFG_TYPE_NEIGH_FREQ;
   lwrMng->t.protoCfg.t.neighFreqCfg.action = WR_ACT_DEL;
   lwrMng->t.protoCfg.t.neighFreqCfg.cellId = 1; /* this is ownCellID in case of PLD based, jst a logic entity */

   lwrMng->t.protoCfg.t.neighFreqCfg.numFreq = 1;
   nrFreqCfg = (lwrMng->t.protoCfg.t.neighFreqCfg.t.delNeighFreq);
   nrFreqCfg->type = WR_NEIGH_CELL_EUTRA;
   nrFreqCfg->t.eutranFreq.earfcn = 5250;

   RETVALUE(ROK);
} /* end of smDynEutraFreqDel */
 
PUBLIC S16 smDynWrNrEutraFreqCfg
(
U8                           type
)
{
   LwrMngmt *lwrMng = NULLP;
   Pst pst;
   
   TRC2(smDynWrNrEutraFreqCfg);
   
   WR_ALLOC(&lwrMng, sizeof(LwrMngmt));
   SM_SET_ZERO(&pst, sizeof(Pst));

   if(lwrMng == NULLP)
   {
      RETVALUE(RFAILED);
   }
   if(type == 1)
   {
      /* add new configuration */
      smDynEutraFreqAdd(lwrMng);
   }
   else if(type == 2)
   {
      /* modify existing configuration */
      smDynEutraFreqMod(lwrMng);
   }
   else
   {
      /* delete existing configuration */
      smDynEutraFreqDel(lwrMng);
   }
   
   lwrMng->hdr.msgType             = TCFG;
   lwrMng->hdr.msgLen              = 0;
   lwrMng->hdr.entId.ent           = ENTWR;
   lwrMng->hdr.entId.inst          = SM_INST_ZERO;
   lwrMng->hdr.elmId.elmnt         = STWRPROTOCFG;
   lwrMng->hdr.response.selector   = SM_SELECTOR_LWLC;

    /* Fill Pst */
   pst.selector  = SM_SELECTOR_LWLC;
   pst.srcEnt    = ENTSM;
   pst.dstEnt    = ENTWR;
   pst.dstProcId = SM_WR_PROC;
   pst.srcProcId = SM_SM_PROC;

   (Void) SmMiLwrCfgReq(&pst, lwrMng);

   RETVALUE(ROK);
} /* end of smDynWrNrEutraFreqCfg */

/*
*
*       Fun:   smSndWrCellModSib5
*
*       Desc:  This function for sending eNodeB SIB5 Modification to LWR
*
*       Ret:   ROK
*
*       Notes: None
*              
*       File:  wr_sm_wr_cntrl.c 
*
*/
#ifdef ANSI
PUBLIC S16 smSndWrCellModSib5
(
)
#else /* ANSI not defined */
PUBLIC S16 smSndWrCellModSib5()
#endif /* end of ANSI */
{
   LwrMngmt          *lwrMng = NULLP;
   Pst               pst;
   U8                cfgGrpIndex  = 0;

   TRC2(smSndWrCellModSib5);

   WR_ALLOC(&lwrMng, sizeof(LwrMngmt));
   SM_SET_ZERO(&pst, sizeof(Pst));

   if(lwrMng == NULLP)
   {
      RETVALUE(RFAILED);
   }
   lwrMng->t.protoCfg.type = WR_CFG_TYPE_CELL;
   lwrMng->t.protoCfg.t.cellCfg.action = WR_ACT_MOD;

   lwrMng->t.protoCfg.t.cellCfg.t.modCell.cellId = wrSmDfltCellId;
   lwrMng->t.protoCfg.t.cellCfg.t.modCell.cellIndex = WR_DFLT_CELL_IDX;
   lwrMng->t.protoCfg.t.cellCfg.t.modCell.reCfgType = LWR_CELL_CFG_GRP;


   cfgGrpIndex++; 
   lwrMng->t.protoCfg.t.cellCfg.t.modCell.numOfGrps = cfgGrpIndex;
   lwrMng->hdr.msgType             = TCFG;
   lwrMng->hdr.msgLen              = 0;
   lwrMng->hdr.entId.ent           = ENTWR;
   lwrMng->hdr.entId.inst          = SM_INST_ZERO;
   lwrMng->hdr.elmId.elmnt         = STWRPROTOCFG;

    /* Fill Pst */
   pst.selector  = SM_SELECTOR_LWLC;
   pst.srcEnt    = ENTSM;
   pst.dstEnt    = ENTWR;
   pst.dstProcId = SM_WR_PROC;
   pst.srcProcId = SM_SM_PROC;

   (Void) SmMiLwrCfgReq(&pst, lwrMng);
   
   RETVALUE(ROK);
} /* end of smSndWrCellModSib5 */  

/*
*
*       Fun:   smSndWrCellModSib3
*
*       Desc:  This function for sending eNodeB SIB3 Modification to LWR
*
*       Ret:   ROK
*
*       Notes: None
*              
*       File:  wr_sm_wr_cntrl.c 
*
*/
#ifdef ANSI
PUBLIC S16 smSndWrCellModSib3
(
)
#else /* ANSI not defined */
PUBLIC S16 smSndWrCellModSib3()
#endif /* end of ANSI */
{
   LwrMngmt          *lwrMng = NULLP;
   Pst               pst;
   LwrCellSib3CfgGrp *sib3 =NULLP;
   U8                cfgGrpIndex  = 0;

   TRC2(smSndWrCellModSib3);

   WR_ALLOC(&lwrMng, sizeof(LwrMngmt));
   SM_SET_ZERO(&pst, sizeof(Pst));

   if(lwrMng == NULLP)
   {
      RETVALUE(RFAILED);
   }
   lwrMng->t.protoCfg.type = WR_CFG_TYPE_CELL;
   lwrMng->t.protoCfg.t.cellCfg.action = WR_ACT_MOD;

   lwrMng->t.protoCfg.t.cellCfg.t.modCell.cellId = wrSmDfltCellId;
   lwrMng->t.protoCfg.t.cellCfg.t.modCell.cellIndex = WR_DFLT_CELL_IDX;
   lwrMng->t.protoCfg.t.cellCfg.t.modCell.reCfgType = LWR_CELL_CFG_GRP;

   /* mem_issue_fix: Allocating memory for all the cell group though
    *     * all the groups are not being used as of now. This helps to avoid
    *         * multiple allocation and can have a common logic to free the memory */
   WR_ALLOC(&(lwrMng->t.protoCfg.t.cellCfg.t.modCell.cellCfgGrp[cfgGrpIndex]), \
         (sizeof(LwrCellCfgGrp)));
   if(NULLP == lwrMng->t.protoCfg.t.cellCfg.t.modCell.cellCfgGrp[cfgGrpIndex])
   {
      RLOG0(L_FATAL, "Memory allocation failed.");
      RETVALUE(RFAILED);
   }

   /*Filling Sib3 params*/
   lwrMng->t.protoCfg.t.cellCfg.t.modCell.cellCfgGrp[cfgGrpIndex]->grpType
                                                        = WR_CELL_CFG_GRP_SIB3;

   sib3 =&(lwrMng->t.protoCfg.t.cellCfg.t.modCell.cellCfgGrp[cfgGrpIndex]->t.
         sib3CfgGrp);

   sib3->reCfgType = LWR_CELL_SIB3_CFG_QHYST | LWR_CELL_SIB3_CFG_SPD_ST_RESEL
      | LWR_CELL_SIB3_CFG_MOB_ST_PAR | LWR_CELL_SIB3_CFG_SF_MEDIUM
      | LWR_CELL_SIB3_CFG_SF_HIG | LWR_CELL_SIB3_CFG_SERV_FREQ_INFO
      | LWR_CELL_SIB3_CFG_INTRA_RREQ;
   sib3->qHyst = 2;
   sib3->speedStateReselPres = WR_PRSNT;
   sib3->mobStateParams.nCellChangeHigh = 2;
   sib3->mobStateParams.nCellChangeMedium =2;
   sib3->mobStateParams.speedStatPres.pres =WR_PRSNT;
   sib3->mobStateParams.tEvalulation = 1;
   sib3->mobStateParams.tHystNormal =1;
   sib3->sfMedium = 0;
   sib3->sfHigh =0;

   cfgGrpIndex++; 
   lwrMng->t.protoCfg.t.cellCfg.t.modCell.numOfGrps = cfgGrpIndex;
   lwrMng->hdr.msgType             = TCFG;
   lwrMng->hdr.msgLen              = 0;
   lwrMng->hdr.entId.ent           = ENTWR;
   lwrMng->hdr.entId.inst          = SM_INST_ZERO;
   lwrMng->hdr.elmId.elmnt         = STWRPROTOCFG;

    /* Fill Pst */
   pst.selector  = SM_SELECTOR_LWLC;
   pst.srcEnt    = ENTSM;
   pst.dstEnt    = ENTWR;
   pst.dstProcId = SM_WR_PROC;
   pst.srcProcId = SM_SM_PROC;

   (Void) SmMiLwrCfgReq(&pst, lwrMng);
   
   RETVALUE(ROK);
} /*end of smSndWrCellModSib3 */

/*
*
*       Fun:   smSndWrCellModSib2
*
*       Desc:  This function for sending eNodeB SIB2 Modification to LWR
*
*       Ret:   ROK
*
*       Notes: None
*              
*       File:  wr_sm_wr_cntrl.c 
*
*/
#ifdef ANSI
PUBLIC S16 smSndWrCellModSib2
(
)
#else /* ANSI not defined */
PUBLIC S16 smSndWrCellModSib2()
#endif /* end of ANSI */
{
   LwrMngmt          *lwrMng = NULLP;
   Pst               pst;
   LwrCellSib2CfgGrp *sib2 =NULLP;
   U8                cfgGrpIndex  = 0;
   
   TRC2(smSndWrCellModSib2);
   
   WR_ALLOC(&lwrMng, sizeof(LwrMngmt));
   SM_SET_ZERO(&pst, sizeof(Pst));

   if(lwrMng == NULLP)
   {
      RETVALUE(RFAILED);
   }
   lwrMng->t.protoCfg.type = WR_CFG_TYPE_CELL;
   lwrMng->t.protoCfg.t.cellCfg.action = WR_ACT_MOD;

   lwrMng->t.protoCfg.t.cellCfg.t.modCell.cellId = wrSmDfltCellId;
   lwrMng->t.protoCfg.t.cellCfg.t.modCell.cellIndex = WR_DFLT_CELL_IDX;
   lwrMng->t.protoCfg.t.cellCfg.t.modCell.reCfgType = LWR_CELL_CFG_GRP;
   /* mem_issue_fix: Allocating memory for all the cell group though
    * all the groups are not being used as of now. This helps to avoid
    * multiple allocation and can have a common logic to free the memory */
   WR_ALLOC(&(lwrMng->t.protoCfg.t.cellCfg.t.modCell.cellCfgGrp[cfgGrpIndex]), \
                       (sizeof(LwrCellCfgGrp)));
   if(NULLP == lwrMng->t.protoCfg.t.cellCfg.t.modCell.cellCfgGrp[cfgGrpIndex])
   {
      RLOG0(L_FATAL, "Memory allocation failed.");
      RETVALUE(RFAILED);
   }
   /*Filling Sib2 params*/
   lwrMng->t.protoCfg.t.cellCfg.t.modCell.cellCfgGrp[cfgGrpIndex]->grpType
                                                        = WR_CELL_CFG_GRP_SIB2;

   sib2 =&(lwrMng->t.protoCfg.t.cellCfg.t.modCell.cellCfgGrp[cfgGrpIndex]->t.
         sib2CfgGrp);


   sib2->reCfgType = LWR_CELL_SIB2_CFG_RADIO_RSRC_CMN
      | LWR_CELL_SIB2_CFG_UE_TMR_CONS | LWR_CELL_SIB2_CFG_TIME_ALIGN;
   
   sib2->radioResourceCommon.reCfgType = LWR_CELL_SIB2_RRSRCMN_RACH_CFG_CMN 
   | LWR_CELL_SIB2_RRSRCMN_PDSCH_CFG_CMN | LWR_CELL_SIB2_RRSRCMN_SRS_CFG_CMN;
    

   sib2->radioResourceCommon.rachCfgCommon.maxHARQMsg3 = 2;
   
   sib2->radioResourceCommon.rachCfgCommon.preambleCfg.numOfRAPreambles =
      NhuRACH_ConfigCmnpreambleInfopreamblesGroupAConfigsizeOfRA_PreamblesGroupAn4Enum;
   sib2->radioResourceCommon.rachCfgCommon.preambleCfg.groupASize =
      NhuRACH_ConfigCmnpreambleInfopreamblesGroupAConfigsizeOfRA_PreamblesGroupAn4Enum;
   sib2->radioResourceCommon.rachCfgCommon.preambleCfg.msgSizeGroupA =
    NhuRACH_ConfigCmnpreambleInfopreamblesGroupAConfigmessageSizeGroupAb56Enum;
   sib2->radioResourceCommon.rachCfgCommon.preambleCfg.pwrOffstGrpB  = 0x02;

   sib2->radioResourceCommon.rachCfgCommon.powerRamping.initRcvdTrgtPwr = 0x0a;
   sib2->radioResourceCommon.rachCfgCommon.powerRamping.powerRampingstep =0x02;

   sib2->radioResourceCommon.rachCfgCommon.raSupervisionCfg.macContResTimer =
      NhuRACH_ConfigCmnra_SupervisionInfomac_ContentionResolutionTimersf40Enum;
   sib2->radioResourceCommon.rachCfgCommon.raSupervisionCfg.rarWindowSize=0x06;
   sib2->radioResourceCommon.rachCfgCommon.raSupervisionCfg.preambleTransMax=
      0x02;


   sib2->radioResourceCommon.pdschCfgCommon.pB = 0x01;
   sib2->radioResourceCommon.pdschCfgCommon.referenceSignalPwr = 1;

   sib2->radioResourceCommon.srsCfgCommon.cSrs = 0x01;
   sib2->radioResourceCommon.srsCfgCommon.simulAckNackSrs = TRUE; 
   sib2->radioResourceCommon.srsCfgCommon.srsMaxUpts = 0x01;
   sib2->radioResourceCommon.srsCfgCommon.subframeCfg = 0x01;


   sib2->ueTimerConstants.t300 = 0x01;
   sib2->ueTimerConstants.t301 = 0x01;
   sib2->ueTimerConstants.t310 = 0x01;
   sib2->ueTimerConstants.n310 =0x01;
   sib2->ueTimerConstants.t311 =0x01;
   sib2->ueTimerConstants.n311 = 0x01;
   /* ccpu00125578 T304 value set to ms500 so that
    * RRC Connection Reestablishment will be delayed during HO */
   sib2->ueTimerConstants.ccoT304 = 0x04;
   sib2->ueTimerConstants.t304 = 0x04;

   sib2->timeAlignmentCommon = 0x06;



   cfgGrpIndex++; 
   lwrMng->t.protoCfg.t.cellCfg.t.modCell.numOfGrps = cfgGrpIndex;
   lwrMng->hdr.msgType             = TCFG;
   lwrMng->hdr.msgLen              = 0;
   lwrMng->hdr.entId.ent           = ENTWR;
   lwrMng->hdr.entId.inst          = SM_INST_ZERO;
   lwrMng->hdr.elmId.elmnt         = STWRPROTOCFG;

    /* Fill Pst */
   pst.selector  = SM_SELECTOR_LWLC;
   pst.srcEnt    = ENTSM;
   pst.dstEnt    = ENTWR;
   pst.dstProcId = SM_WR_PROC;
   pst.srcProcId = SM_SM_PROC;

   (Void) SmMiLwrCfgReq(&pst, lwrMng);
   
   RETVALUE(ROK);
} /* end of smSndWrCellModSib2 */

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
   U8                         idx = 0;
   U8                         dbIdx = 0;
   LwrNghCellCfg             *nghCellCfg;

   TRC2(SmMiLwrAddNeighCfgReq)

   WR_ALLOC(&nghCellCfg, sizeof(LwrNghCellCfg));
   if((nghCellCfg == NULLP) || (NULLP == cfgReq))
   {
      RLOG0(L_FATAL, "Memory allocation failed.");
      RETVALUE(RFAILED);
   }
   nghCellCfg->enbId = cfgReq->t.nbrAddReq.enbId; 
   wrCpyCmTptAddr(&nghCellCfg->enbAddr, &cfgReq->t.nbrAddReq.nbIpAddress);
   wrCpyCmTptAddr(&nghCellCfg->ipAddr, &cfgReq->t.nbrAddReq.nbIpAddress);
/*   nghCellCfg->peerId = wrSmmGetX2apFreePeerId();*/
   WR_GET_PEERID_FROM_ENBID(nghCellCfg->enbId, nghCellCfg->peerId);
   if(WR_SMM_INVALID_PEERID == nghCellCfg->peerId)
   {
      RLOG0(L_ERROR, "Failed to get PeerId from wrX2apPeerIdLst List");
      WR_FREE(nghCellCfg, sizeof(LwrNghCellCfg));
      RETVALUE(RFAILED);
   }
   /* check if this neighbor info is already present in smCfgCb list */
   for(idx=0; idx<WR_SMM_MAX_NBR_ENB; idx++)
   {
      if(NULLP != smCfgCb.wrNghCellCfg[idx])
      {
         if(nghCellCfg->enbId == smCfgCb.wrNghCellCfg[idx]->enbId)
         {
            dbIdx = idx;
            /*ccpu00127802*/
            RLOG0(L_WARNING, "Neighbor already present in neighbor list");
            WR_FREE(nghCellCfg, sizeof(LwrNghCellCfg));
            break;
         }
      }
      else if(NULLP == smCfgCb.wrNghCellCfg[idx])
      {
         
         dbIdx = idx;
         smCfgCb.wrNghCellCfg[idx] = nghCellCfg;
         smCfgCb.noOfNghCfg++;
         break;
      }
   }
   if(idx == WR_SMM_MAX_NBR_ENB)
   {
      WR_FREE(nghCellCfg, sizeof(LwrNghCellCfg));
      RETVALUE(RFAILED);
   }
   if(cfgReq != NULLP)
   {
      WR_FREE(cfgReq, sizeof(LwrMngmt));
   }
   RLOG1(L_INFO, "smCb.smState before adding dynamic Nbr Enodeb is =%d",
      smCb.smState);
/*      smCb.smState = WR_SM_STATE_DYN_X2AP_CFG_DONE;*/
      smBuildDynCzPeerCfg(idx +1,dbIdx);
   RETVALUE(ROK);
} /* end of SmMiLwrAddNeighCfgReq() */

/*
 *
 *       Fun:    SmMiLwrUpDateNhgCellCfgReq - Neighbor Config Request
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
PUBLIC S16 SmMiLwrUpDateNhgCellCfgReq 
(
Pst          *pst,
LwrMngmt     *cfgReq
)
#else
PUBLIC S16 SmMiLwrUpDateNhgCellCfgReq(pst, cfgReq)
Pst          *pst;
LwrMngmt     *cfgReq;
#endif /* ANSI */
{
   TRC2(SmMiLwrUpDateNhgCellCfgReq)

  /* Memory leak fix ccpu00145573 */
  WR_FREE(cfgReq, sizeof(LwrMngmt));
  RETVALUE(ROK);
}/*end of SmMiLwrUpDateNhgCellCfgReq*/

/**
 *  @brief This function validates the IpAddress read from wr_cfg.txt for MME 
 *  and  Neighbour that are stored in smCfgCb. 
 *
 *  @details  Function: smValidateIpAddr 
 *          Processing steps:
 *          - Return SUCESS If:
 *                The types of IP ADDRESS supported by ENodeB should be 
 *                matched with types of IP ADDRESS supported by list of 
 *                MMEs and Neighbours. 
 *            Else
 *                Return FAILURE
 *
 *  @param[in] Void Void
 *
 *  @return S32
 *     -# Success : ROK
 *     -# Failure : RFAILED
 *
 */
#ifdef ANSI 
PUBLIC S16 smValidateIpAddr
(
)
#else
PUBLIC S16 smValidateIpAddr(Void)
#endif
{
   Bool          ipv4Present = FALSE;
   Bool          ipv6Present = FALSE;
   U16           cnt;
   S16           retVal = ROK;
   Bool          eNBIpv4Present = FALSE;
   Bool          eNBIpv6Present = FALSE;
   TRC2(smValidateIpAddr);
   if(smCfgCb.enbIpAddr[WR_IPV4_IDX].type == CM_TPTADDR_IPV4)
   {
      eNBIpv4Present = TRUE; 
   }
   if(smCfgCb.enbIpAddr[WR_IPV6_IDX].type == CM_TPTADDR_IPV6)
   {
      eNBIpv6Present = TRUE; 
   }
   /* Validate MME IP Addresses */
   for(cnt = 0; cnt < smCfgCb.noOfCfg; cnt++)
   {
      if(smCfgCb.mmeCfg[cnt]->mmeAddr[0].type == CM_TPTADDR_IPV4)
      {
         ipv4Present = TRUE;
      }
      else if(smCfgCb.mmeCfg[cnt]->mmeAddr[0].type == CM_TPTADDR_IPV6)
      {
         ipv6Present = TRUE;
      }
   }
   /* eNB is having IPv4 and IPv6 and MME is either IPv4 or IPv6 then it is 
    * valid configuration.
    */
   if((eNBIpv4Present == eNBIpv6Present) && (TRUE == eNBIpv4Present)) 
   {
      if((ipv4Present != TRUE) && (ipv6Present != TRUE))
      {
         RLOG0(L_ERROR, "MME and EnbIp Address configuration not valid");
         retVal = RFAILED;
         RETVALUE(retVal);
      }
   }
   /* eNB is having IPv4 and MME is having IPv4 or eNB is having IPv6
    *  and MME is having IPv6 then it is valid configuration.
    */
   if((eNBIpv4Present != ipv4Present) && (eNBIpv6Present != ipv6Present))
   {
      RLOG0(L_ERROR, "MME and EnbIp Address configuration not valid");
      retVal = RFAILED;
      RETVALUE(retVal);
   }
   if((eNBIpv4Present == ipv4Present) && (eNBIpv6Present != ipv6Present))
   {
      RLOG0(L_ERROR, "MME and EnbIp Address configuration not valid");
      retVal = RFAILED;
      RETVALUE(retVal);
   }
   if((eNBIpv6Present == ipv6Present) && (eNBIpv4Present != ipv4Present))
   {
      RLOG0(L_ERROR, "MME and EnbIp Address configuration not valid");
      retVal = RFAILED;
      RETVALUE(retVal);
   }
   /* Validate Neighbour IP Addresses */
   ipv4Present = FALSE;
   ipv6Present = FALSE;
   for(cnt = 0; cnt < smCfgCb.noOfNghCfg; cnt++)
   {
      if(smCfgCb.wrNghCellCfg[cnt]->enbAddr.type == CM_TPTADDR_IPV4)
      {
         ipv4Present = TRUE;
      }
      if(smCfgCb.wrNghCellCfg[cnt]->enbAddr.type == CM_TPTADDR_IPV6)
      {
         ipv6Present = TRUE;
      }
   }
   /* eNB is having IPv4 and IPv6 and Neighbour is either IPv4 or IPv6 then it is 
    * valid configuration.
    */

   if((eNBIpv4Present == eNBIpv6Present) && (TRUE == eNBIpv4Present)) 
   {
      if((ipv4Present != TRUE) && (ipv6Present != TRUE))
      {
         RLOG0(L_ERROR, "Neighbour and EnbIp Address configuration not valid");
         retVal = RFAILED;
         RETVALUE(retVal);
      }
   }
   /* eNB is having IPv4 and MME is having IPv4 or eNB is having IPv6
    *  and Neighbour is having IPv6 then it is valid configuration.
    */

   if((eNBIpv4Present != ipv4Present) && (eNBIpv6Present != ipv6Present))
   {
      RLOG0(L_ERROR, "Neighbour and EnbIp Address configuration not valid");
      retVal = RFAILED;
      RETVALUE(retVal);
   }
   if((eNBIpv4Present == ipv4Present) && (eNBIpv6Present != ipv6Present))
   {
      RLOG0(L_ERROR, "Neighbour and EnbIp Address configuration not valid");
      retVal = RFAILED;
      RETVALUE(retVal);
   }
   if((eNBIpv6Present == ipv6Present) && (eNBIpv4Present != ipv4Present))
   {
      RLOG0(L_ERROR, "Neighbour and EnbIp Address configuration not valid");
      retVal = RFAILED;
      RETVALUE(retVal);
   }
   RETVALUE(retVal);
} /*end of  smValidateIpAddr*/



/* CSG_DEV start */
/*
 *
 *       Fun:   smWrValidateConfigParams
 *
 *       Desc:  Validates the values read from wr_cfg.txt that are stored
 *              in smCfgCb.
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
   U16                       maxCsgPci = 0;
   U16                       maxSmallCellPci;
   U16                       csgPciRange;
   S16                       retVal = ROK;

   /* HCSG_DEV */
   wrSmDfltEnbType = smGetEnbType(wrSmDfltPhysCellId);

   /*SON will do PCI selection hence commenting this validation*/
   if(TRUE == wrSmDfltIsSmlCellPciCfgd)
   {
      maxSmallCellPci = wrSmDfltSmallCellStartPci +
         wrSmDfltSmallCellPciRange;
      /* Removed one part of comparision to fix warning */   
      /*if(WR_PCI_MIN_VAL > wrSmDfltSmallCellStartPci || WR_PCI_MAX_VAL < maxSmallCellPci)*/
      if(WR_PCI_MAX_VAL < maxSmallCellPci)
      {
         RLOG2(L_ERROR, "Invalid SmallCell PCI "
               "configuration SmallCell Start PCI [%d] Max [%d]",
               wrSmDfltSmallCellStartPci, maxSmallCellPci);
         RETVALUE(RFAILED);
      }
   }
   else
   {
      RLOG1(L_ERROR, "Missing SmallCell PCI configuration AccesMode [%d]",
            wrSmDfltAccessMode);
      RETVALUE(RFAILED);
   }

   if(TRUE == wrSmDfltIsCsgPciCfgd)
   {
      WR_EMM_GET_PCI_RANGE(wrSmDfltCSGPciRange, csgPciRange);
      maxCsgPci = wrSmDfltCSGStartPci + csgPciRange;
      if(WR_PCI_MIN_VAL > wrSmDfltCSGStartPci ||
            WR_PCI_MAX_VAL < maxCsgPci)
      {
         wrSmDfltIsCsgPciCfgd = FALSE;
         RLOG2(L_ERROR, "Ingnoring invalid CSG PCI OPEN Access "
               "configuration CSG Start PCI [%d] Max [%d]",
               wrSmDfltCSGStartPci, maxCsgPci);
      }
   }
   if(smValidateIpAddr() != ROK)
   {
      RLOG0(L_ERROR, "Validation of Configuration Parameters Failed");
      RETVALUE(RFAILED);
   }

   /* CSG configuration parameter validation */
   switch(wrSmDfltAccessMode)
   {
      case WR_ENB_OPEN_ACCESS:
      {
/*eNBType can't be detected as PCI will be selected in SON, hence commenting this validation*/
      }
      break;
      case WR_ENB_CLOSED_ACCESS:
      {
         if(FALSE == wrSmDfltIsCsgPciCfgd)
         {
            RLOG0(L_ERROR, "Incomplete CLOSED Mode CSG configuration");
            RLOG3(L_ERROR, "CSG ID [%d] CSG PCI CFGD [%d] SmallCell PCI "
                  "CFGD [%d]", wrSmDfltCsgIdVal, wrSmDfltIsCsgPciCfgd,
                  wrSmDfltIsSmlCellPciCfgd);
            retVal = RFAILED;
         }
         else
         {
            if(0 > wrSmDfltCsgIdVal || WR_CSG_ID_MAX_VAL < wrSmDfltCsgIdVal)
            {
               RLOG1(L_ERROR, "CSG ID not in range CSG ID [%d]",
                     wrSmDfltCsgIdVal);
               retVal = RFAILED;
            }
         }
      }
      break;
      case WR_ENB_HYBRID_ACCESS:
      /* Validate Max number of CSG and Non CSG UEs configured */
      if((wrSmDfltMaxCsgUEs > smCfgCb.maxUeSupp) || 
         (wrSmDfltMaxNonCsgUEs > smCfgCb.maxUeSupp) ||
         ((wrSmDfltMaxCsgUEs == wrSmDfltMaxNonCsgUEs) && 
          (wrSmDfltMaxCsgUEs < smCfgCb.maxUeSupp) &&
          ((wrSmDfltMaxNonCsgUEs + wrSmDfltMaxCsgUEs) != smCfgCb.maxUeSupp)) ||
         ((wrSmDfltMaxCsgUEs != wrSmDfltMaxNonCsgUEs) &&
          ((wrSmDfltMaxNonCsgUEs + wrSmDfltMaxCsgUEs) != smCfgCb.maxUeSupp)))
      {
            RLOG0(L_ERROR, "Invalid number of CSG or Non CSG UEs configured");
            RLOG4(L_ERROR, "CSG ID [%d] CSG Non CSG UE CFGD [%d] CSG UE CFGD "
                  "[%d] Max UE Supported [%d]", wrSmDfltCsgIdVal, wrSmDfltMaxNonCsgUEs,
                  wrSmDfltMaxCsgUEs, smCfgCb.maxUeSupp);
            RETVALUE(RFAILED);
      }
      /* Validate minimum number of UL and DL resources configured */
      if(wrSmDfltMinDlResNonCsg > 100)
      {
         RLOG1(L_ERROR, "Invalid number of Minimum DL Resources [%d]",
                  wrSmDfltMinDlResNonCsg);
         RETVALUE(RFAILED);
      }
      if(wrSmDfltMinUlResNonCsg > 100)
      {
         RLOG1(L_ERROR, "Invalid number of Minimum UL Resources [%d]",
                  wrSmDfltMinUlResNonCsg);
         RETVALUE(RFAILED);
      }
      break;
      default:
      {
         RLOG1(L_ERROR, "Invalid Access Mode configuration [%d]",
               wrSmDfltAccessMode);
         retVal = RFAILED;
      }
      break;
   }

   if(TRUE == wrSmDfltTmAutoConfig)
   {
      wrSmDfltTransModeUse = 4;
   }
   else
   {
      wrSmDfltTransModeUse = wrSmDfltPreferredTm;
   }

   RETVALUE(retVal);
} /* end of smWrValidateConfigParams */
/* CSG_DEV end */

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
*       File:  fc_smm_enbapp_rsys.c 
*
*/
#ifdef ANSI 
PUBLIC U8 smGetEnbType(U16 phyCellId)
#else
PUBLIC U8 smGetEnbType(phyCellId)
U16 phyCellId;
#endif
{
   U16 smallCellStartPci = wrSmDfltSmallCellStartPci;
   U16 smallCellPciRange = wrSmDfltSmallCellPciRange;

   if ((phyCellId >= smallCellStartPci) && (phyCellId <= (smallCellStartPci + smallCellPciRange)))
   {
      RETVALUE(WR_ENODEB_TYPE_HOME);
   }
   else
   {
      RETVALUE(WR_ENODEB_TYPE_MACRO);
   }
}/*end of smGetEnbType*/

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
