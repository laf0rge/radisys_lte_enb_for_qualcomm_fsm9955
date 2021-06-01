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
               

     File:     wr_smm_smallcell.c

     Sid:      fc_smm_smallcell_brcm.c@@/main/TeNB_Main_BR/5 - Mon Aug 11 16:52:32 2014

     Prg:      Sriky 

*********************************************************************21*/

#include "wr.h"
#include "wr_cmn.h"
#include "wr_smm_smallcell.h"
#include "wr_lwr.h" 
#include "nhu.x" 
#ifdef LTE_QCOM
#include "rf/rfBandwidthMhz.h"
#include "rf/rfTechnology.h"
#include "rf/rfPathDirection.h"
#include "hex0/logLevel.h"
#endif
#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

   U8 wrSmDfltRlLogLvl     = 4; /* Log Level Default set to L_EVENT */
   U8 wrSmDfltNumNrFreq    = 3; /**/
   U32 wrSmDfltCellId      = 1;
   U32 wrSmDfltPhysCellId  = 1;
   U16 wrSmDfltNoOfGrp     = 4;
   /* Removing hardcoding of SecEnable to FALE */
#ifndef TENB_AS_SECURITY
   Bool wrSmDfltSecEnable  = FALSE;
#else
   Bool wrSmDfltSecEnable  = TRUE;
#endif
   U32 wrSmDfltOpMode      = 4;
   U32 wrSmDfltNumNrEnb    = 2;/**/
   U32 wrSmDfltNumCells    = 1;/**/
   /*MIMO :2 SISO:0 MIMO  :5*/
   U32 wrSmDfltTagPeriod   = 2;
 /* These are being configured via wr_cfg.txt file */
   U8 wrSmTddSubframeConfig         = 0;
   U8 wrSmTddSpecialSubframeConfig     = 0;

/* ACC-TDD */
#ifdef LTE_TDD
   /*Duplex Mode*/
   U8 wrSmDfltDuplexMode                = NHU_MODE_TDD;
#else
   U8 wrSmDfltDuplexMode                = NHU_MODE_FDD;
#endif
   
   /* DL EARFCN 3100, 2850 for Band 7; 
    *                 5230 for Band 13 */
#if 0 /* BAND 7, BAND 13 DL Freq config */
   U16 wrSmDfltServingCellDLCarrierFreq = 3100; 
#else
   U16 wrSmDfltServingCellDLCarrierFreq = 5230; 
#endif
   /* UL EARFCN 21100, 20850 for Band 7; 
                23230 for Band 13 */
#if 0 /* BAND 7, BAND 13 UL Freq config */
   U16 wrSmDfltUlEarFcn                 = 21100; 
#else
   U16 wrSmDfltUlEarFcn                 = 23230; 
#endif

   U16 wrSmDfltUlCyclicPrefixLength     = 0x00;

   CtfBandwidth wrSmDfltCellDlBW        = CTF_BW_RB_50;                  /* 50 */ 
   CtfBandwidth wrSmDfltCellUlBW        = CTF_BW_RB_50;                  /* 50 */ 

   /* 
    * Master Info Block 
    */
   CtfPhichDuration wrSmDfltPhichDur = CTF_PHICH_DUR_NORMAL;
   // ACC-TDD mod - use MSPD value
/* ACC-TDD */
#ifdef LTE_TDD
   CtfPhichResource wrSmDfltPhichRes = CTF_PHICH_RSRC_HALF;
#else
   CtfPhichResource wrSmDfltPhichRes = CTF_PHICH_RSRC_ONESIXTH;

#endif
   U8    wrSmDfltNumPlmn      = 1;
   U8    wrSmDfltNumMncDigits = 2; /* */
   U8    wrSmDfltPlmn1Mcc0    = 3;  /* */
   U8    wrSmDfltPlmn1Mcc1    = 1;  /**/
   U8    wrSmDfltPlmn1Mcc2    = 1; /**/
   U8    wrSmDfltPlmn1Mnc0    = 4; /**/
   U8    wrSmDfltPlmn1Mnc1    = 8; /**/
   U8    wrSmDfltPlmn1Mnc2    = -1; /**/
   U8   wrSmDfltNumOfMncDigits[6];
   U8   wrSmDfltPlmnMcc[6][3];
   U8   wrSmDfltPlmnMnc[6][3];

WreNodBPriIntAlgo wreNodBPriIntgAlgo[]={{TRUE,WR_UMM_ZUC_ALGO}, {TRUE,WR_UMM_AES_ALGO}, {TRUE, WR_UMM_SNOW3G_ALGO},{TRUE, WR_UMM_NULL_ALGO}};
WreNodBPriCiphAlgo wreNodBPriCiphAlgo[]={{TRUE,WR_UMM_ZUC_ALGO}, {TRUE,WR_UMM_AES_ALGO}, {TRUE, WR_UMM_SNOW3G_ALGO},{TRUE, WR_UMM_NULL_ALGO}};

   U8    wrSmDfltPlmn2Mcc0;
   U8    wrSmDfltPlmn2Mcc1;
   U8    wrSmDfltPlmn2Mcc2;
   U8    wrSmDfltPlmn2Mnc0;
   U8    wrSmDfltPlmn2Mnc1;
   U8    wrSmDfltPlmn2Mnc2;
   U16   wrSmDfltTac          = 1;
   U32   wrSmDfltCsgIdVal     = 0;
   /* CSG_DEV start */
   U8    wrSmDfltAccessMode          = WR_ENB_OPEN_ACCESS;
   S8    wrSmDfltHenbName[48]        = "\0";
   U8    wrSmDfltPreferredHoTarget   = 0;
   U8    wrSmDfltIsSmlCellPciCfgd    = FALSE;
   U16   wrSmDfltSmallCellStartPci   = 1;
   U16   wrSmDfltSmallCellPciRange   = 32;
   S16   wrSmDfltMaxCsgMembers       = -1;
   Bool  wrSmDfltIsCsgMbrshpDetermndLcly = FALSE;
   U8    wrSmDfltNonCsgUeAccDcsn     = WR_LWR_NON_CSG_UE_ACC_DECSN_BY_CORE;
/* SIB4 defaults */
   U8    wrSmDfltIsCsgPciCfgd        = FALSE;
   U32   wrSmDfltCSGStartPci         = 1;
   U32   wrSmDfltCSGPciRange         = 5;
   U16   wrSmDfltMaxCsgUEs           = 32;
   U16   wrSmDfltMaxNonCsgUEs        = 32;
   U8    wrSmDfltMinDlResNonCsg  = 10;/* Minimum number of PDSCH resources for Non CSG users in case
                                            of hybrid access mode, unit is in percentage */
   U8    wrSmDfltMinUlResNonCsg  = 10; /* Minimum number of PUSCH resources for Non CSG users in case
                                             of hybrid access mode, unit is in percentage */
   /* CSG_DEV end */
   U8    wrSmDfltFreqBandInd  = 7;
   U8    wrSmDfltnumSi;
   /* get the cellId from wr_cfg.txt */
/*   U32   wrSmDfltCellIdentity = 1; */

   U16 wrSmDfltCellRsrvdForOperUse = 1;  /* Not reserwrd */
   U16 wrSmDfltCellBarred          = 1;            
   U16 wrSmDfltIntraFreqResel      = 1;
   U16 wrSmDfltCSFInd              = 1;

   S16 wrSmDlftqRxLevMin       = -50;
   U16 wrSmDfltqRxLevMinOffset = 1; 

   // ACC-TDD mod - use MSPD value for TDD
#ifdef LTE_TDD
   U8 wrSmDfltSiWinSize          = WR_SI_WINDOWLEN_20;
#else
   /* PH04_CMAS */
   U8 wrSmDfltSiWinSize          = WR_SI_WINDOWLEN_20;
#endif
   U16 wrSmDfltSib10RptPrd       = 3600;
   U16 wrSmDfltSib10NumBroadCast = 24;
   U8 wrSmDfltWarnSegSize        = 217;

   U8 wrSmDfltACBarrInfoPres   = 0;
   U8 wrSmDfltACBarrEmergency  = 1;
   U8 wrSmDfltACBarrNormal     = 0;
   U8 wrSmDfltMOACBarrInfoPres = 0;
   U8 wrSmDfltDOACBarrInfoPres = 0;
   U8 wrSmDfltVDACBarrInfoPres = 0;
   U8 wrSmDfltVOACBarrInfoPres = 0;
   U16 wrSmDfltAddSpectEmn     = 0x01; 

   Bool      wrSmDfltMoSigAcBarringEnable   = FALSE;   /*!< 1-Barring Enabled for Mo-signalling else 0 */
   Bool      wrSmDfltMoDataAcBarringEnable   = FALSE;   /*!< 1-Barring Enabled for Mo-Data else 0 */
   U8        wrSmDfltacBarringFactor = 0; /*!< Barring factor {0, 5, 10, 15, ... 95} */
   U16       wrSmDfltacBarringTime   = 5;   /*!< Barring Time in seconds {4, 8, ... 512} */
   U8        wrSmDfltacBarringAC11   = 0;   /*!< Ac Barring for Access class 11 */
   U8        wrSmDfltacBarringAC12   = 0;   /*!< Ac Barring for Access class 12 */
   U8        wrSmDfltacBarringAC13   = 0;   /*!< Ac Barring for Access class 13 */
   U8        wrSmDfltacBarringAC14   = 0;   /*!< Ac Barring for Access class 14 */
   U8        wrSmDfltacBarringAC15   = 0; 

   /* BCCH Cfg */
   U16 wrSmDfltModPeriodCoeff = 0x00;            /* n2  */

   /* PCCH Cfg */
   U16 wrSmDfltDefaultPagCycle = 0x00;           /* 00:v32, 01:v64, 02:v128 03:v256 */
   U16 wrSmDfltPCCHCfgNB       = 0x07;               /* oneT */

   /* PDSCH Cfg */
#ifdef LTE_QCOM
   /* Tx power is increased from 0 to 10 in OAM, 
      hence it is modified from -27 to -17 */
   /* Tx power 0 -> RefSigPower -27
      Tx power 10 -> RefSigPower -17
   */
   S8 wrSmDfltRefSignalPower = -17;
#else
   S8 wrSmDfltRefSignalPower = 0;  
#endif
   U16 wrSmDfltPDSCHCfgPB    = 0x01;    

/* Starts - Fix for CR ccpu00123185 */
   S16 wrSmDfltMspdDbUnit     = 256;
   /* Right now PHY INIT is not accepting any reference signal power
    * we can use these parameters once PHY INIT is accepting 
    * initial referene signal power
    */
#ifdef LTE_QCOM
   S8  wrSmDfltRefSigPwr      = -17; /* unsed */
#else
   S8  wrSmDfltRefSigPwr      = 0; /* unsed */
#endif
   S16 wrSmDfltPilotSigPwr    = 0;
   S16 wrSmDfltPriSigPwr      = 0;
   S16 wrSmDfltSecSigPwr      = 0;

   S8  wrSmDfltLowRefSigPwr   = -100; /* unsed */
   S16 wrSmDfltLowPilotSigPwr = -100;
   S16 wrSmDfltLowPriSigPwr   = -100;
   S16 wrSmDfltLowSecSigPwr   = -100;
/* Ends - Fix for CR ccpu00123185 */
   /* PRACH Config */
#ifdef LTE_QCOM
   U16 wrSmDfltRootSequenceIdx   = 823;           /* 0 */
   U16 wrSmDfltPrachCfgIdx       = 3;                /* Changed from 12 to 8 for PHY 3.8.2*/
   U16 wrSmDfltHighSpeedFlag     = FALSE;            /* FALSE */
   U16 wrSmDfltZeroCorrelZoneCfg = 0x01;         /* 0x05 */
   U16 wrSmDfltPrachFreqOffset   = 1;          /* (assuming 10MHz Channel bandwidth) changed from 4 in mspd  */
   U16 wrSmDfltPrachMaskIdx      = 0;
#else
   U16 wrSmDfltRootSequenceIdx   = 823;           /* 0 */
   U16 wrSmDfltPrachCfgIdx       = 3;                /* 3*/
   U16 wrSmDfltHighSpeedFlag     = FALSE;            /* FALSE */
   U16 wrSmDfltZeroCorrelZoneCfg = 0x0A;         /* 0x05 */
   U16 wrSmDfltPrachFreqOffset   = 1; /*0x2b*/   /* (assuming 10MHz Channel bandwidth) changed from 4 in mspd  */
   U16 wrSmDfltPrachMaskIdx      = 0;
#endif

   /* PUCCH Cfg */
   CtfPucchDeltaShift wrSmDfltPUCCHDeltaShift = 0x01;       /* DS1 */
   U16 wrSmDfltPUCCHnRBCQI = 0x01;          /* Should have been 1 as per doc */
   U16 wrSmDfltPUCCHnCSAN  = 0x00;           /* zero */
   U16 wrSmDfltNlPUCCHAn   = 0x4 /*0x01*/;             /* Should have been 1 as per doc*/
   U8  wrSmDfltMaxPucchRbs = 8;      /* Zero indicates don't care field.
                                        For BCM 61750 chipset the max PUCCH RB 
                                        supported is 4 */
 
   /* PUSCH cfg */
   U16 wrSmDfltNSB = 0x1;                    /* 2 */
   CtfPuschHoppingMode wrSmDfltHoppingMode = CTF_PUSCH_HOPMODE_INTER_SUBFRM; /* interSubFrame */
   U16 wrSmDfltPuschHoppingOff     = 0x08;       /* Assuming 10Mhz channel bandwidth*/
   U16 wrSmDfltEnable64QAM         = FALSE;           /* false */
   U16 wrSmDfltGroupHoppingEnabled = FALSE;   /* false */
   U16 wrSmDfltSeqHoppingEnabled   = FALSE;     /* since group hopping disabled */
   U16 wrSmDfltGroupAssignPUSCH    = 0x0;       /* since group hopping disabled */
   U16 wrSmDfltCyclicShift         = 0x00;            /* since group hopping disabled */



   /* RACH Common Cfg */
   U16 wrSmDfltNumberOfRaPreambles = NhuRACH_ConfigCmnpreambleInfopreamblesGroupAConfigsizeOfRA_PreamblesGroupAn8Enum;      /* n8 */
   U16 wrSmDflSizeOfRaPreamblesGroupA = NhuRACH_ConfigCmnpreambleInfopreamblesGroupAConfigsizeOfRA_PreamblesGroupAn8Enum; /* must be less than wrSmDfltNumberOfRaPreambles. 
                                                                                                                             currently same as wrSmDfltNumberOfRaPreambles */
   U16 wrSmDfltMessageSizeGroupA = NhuRACH_ConfigCmnpreambleInfopreamblesGroupAConfigmessageSizeGroupAb144Enum; /* assigning > 7 (which is minimum) */
   U16 wrSmDfltMessagePowerOffsetGroupB = 0x01;    
   U16 wrSmDfltPowerRampingStep = 0x01;          
#ifdef LTE_TDD
   U16 wrSmDfltPreambleInitRecvdTgtPwr = 0x0f;
#else        
   U16 wrSmDfltPreambleInitRecvdTgtPwr = 0x00; /*0x0b;*/
#endif
  
   U16 wrSmDfltPreambleTransMax = 0x07;
   U16 wrSmDfltRaRspWindowSize = 0x07;          /* sf8 */
   U16 wrSmDfltMacContentResTmr = NhuRACH_ConfigCmnra_SupervisionInfomac_ContentionResolutionTimersf48Enum;         /* sf40 */
   U16 wrSmDfltMaxHARQMsg3Tx = 0x04; 


   /* SoundingsRS UL Cfg */
   U16 wrSmDfltSrsBandwidthCfg   = 0x02;    
   U16 wrSmDfltSrsSubfrmCfg      = 0x00;        
   U16 wrSmDfltAckNackSrsSimTrns = FALSE;
   U16 wrSmDfltSrsMaxUpPts       = 0x00;      

   U16 wrSmDfltSrsCfgPrd           = 0x00;             /*  SRS period configuration */
   /* UL SRS Config */
   /* UL Sounding Reference Signal Periodicity in milli second */
   Bool wrSmEnableSrs                = FALSE;
   U32  wrSmUlSrsPeriod              = 20;
   U16 wrSmDfltSrsUeBandwidthCfg     = 0x00;
   U16 wrSmDfltSrsUEHoppingBandwidth = 0x03;
   /* Uplink Power Control Config */
   S16 wrSmDfltP0NominalPUCCH      = -105; /* -105; */ /*-96; */
   S16 wrSmDfltP0NominalPUSCH      = -80;

   U16 wrSmDfltULPwrCtrlAlpha      = 0x07;   
   /* Uplink Power Control Config */
   U16 wrSmDfltDeltaFPUCCHFormat1  = 0x02; /* 0x01 vrr */
   U16 wrSmDfltDeltaFPUCCHFormat1b = 0x02;
   U16 wrSmDfltDeltaFPUCCHFormat2  = 0x03;
   U16 wrSmDfltDeltaFPUCCHFormat2a = 0x02;
   U16 wrSmDfltDeltaFPUCCHFormat2b = 0x02;
   U32 wrSmDfltS3pMax = 0; 
   /* PDSCH Cfg */
//   S8 wrSmDfltRefSignalPower = 0;  
//   S8    wrSmDfltPMax       = 10; /* SK. Changed from 0 to 10 for TDD-SA2 Low 
  //                                           UL throughput change.*/
   S16 wrSmDfltUlDeltaPreambleMsg3 = 0x04;
   /*!< Number of PRACH resources*/
   U8 wrSmDfltPRACHNumRes          = 0;
   /*!< Number of common logical channels*/
   U8 wrSmDfltNumCmnLcs            = 6;



   /* SIB3 defaults */
   U32 wrSmDfltS3nCellChangeMedium = 1;
   U32 wrSmDfltS3nCellChangeHigh = 1;
   U32 wrSmDfltS3sNonIntraSearch = 0;
   U32 wrSmDfltS3ThreshServingLow = 0;
   U32 wrSmDfltS3CellReselectionPriority = 4;
   U32 wrSmDfltS3qRxLevMin = -70;
   U32 wrSmDfltS3sIntraSearch = 1;
   U32 wrSmDfltS3AllowedMeasBandwidth = 0;
   U32 wrSmDfltS3PresenceAntennaPort1 = FALSE;
   U8  wrSmDfltS3NeighCellCfg = 1 << 6;               
   U32 wrSmDfltS3tReselectionEUTRA = 0;
   U8  wrSmDfltqHyst = 1;
   U8  wrSmDfltSfMed = 1;
   U8  wrSmDfltSfHigh = 1;
   U8  wrSmDfltTEval;
   U8  wrSmDfltTHyst;

   /* SIB4 Configuration Parameters */
   U16 wrSmStartPci = 400;
   U8  wrSmPciRange = 5;
   U8  wrSmDfltPciTyp = 1;  
    

   /*Neighbour Configuration values*/

   U32 wrSmDfltNeighEnbID[2]       = {0x4567,0x6745}; /**/
   U8  wrSmDfltNeighEnbIndex[2]    = {0,1}; /**/
   U8  wrSmDfltNeighEnbType        = WR_ENODEB_TYPE_MACRO; /**/

   U8 wrSmDfltNeEnbNumMncDigits = 3;
   U8 wrSmDfltNeEnbPlmn1Mcc0[32] = {0,1,2}; /**/
   U8 wrSmDfltNeEnbPlmn1Mcc1[32] = {0,2,3};/**/
   U8 wrSmDfltNeEnbPlmn1Mcc2[32] = {0,3,4};/**/
   U8 wrSmDfltNeEnbPlmn1Mnc0[32] = {0,1,2};/**/
   U8 wrSmDfltNeEnbPlmn1Mnc1[32] = {0,2,3};/**/
   U8 wrSmDfltNeEnbPlmn1Mnc2[32] = {0,3,4};/**/

   U8 wrSmDfltNeEnbIPType = CM_IPV4ADDR_TYPE;

   S8 *wrSmDfltNeEnbIP[2] ={"172.27.2.64","172.27.2.65"}; /**/

   U8 wrSmDfltNeEnbState  = WR_NEIGH_ENB_STATE_DOWN;
   LwrX2ConFlg wrSmDfltX2ConFlg    = WR_X2_CON_DIS_ALL;

   U8 wrSmDfltNeEnbNoGrpID    = 1;
   U32 wrSmDfltNeEnbGrpIDPLmn = 0x001123;
   U16 wrSmDfltNeEnbMMEGrpID  = 6;


   /*MME Confiuration values*/

   U8 wrSmDfltMmeIndex =0;
   U8 wrSmDfltMmeIPType = CM_IPV4ADDR_TYPE;
   U16 wrSmDfltMmePort =347;
   S8 *wrSmDfltMmeIP ="172.24.2.75";

   U8 wrSmDftMmeNoPlmn  = 2;

   U8 wrSmDftMmeNoGID   = 1;
   U16 wrSmDfltMmeGID   = 7;

   U8 wrSmDfltMmeNoCode = 1;
   U16 wrSmDfltMmeCode  = 1;

   U8 wrSmDfltMmeState      = WR_MME_STATE_ACTIVE;
   U8 wrSmDfltMmeAdminState = WR_MME_UNLOCKED;


   /*eNodeB Configuration*/

   U8 wrSmDfltMaxNoUE  = TENB_MAX_UE_SUPPORTED;
   U8 wrSmDfltEnbNoGrp = 1;

/* U32 wrSmDfltEnbID = 0x00; in home enodb case enbId is nothing  but cellId */
   U8  wrSmDfltEnbType;
   CmTptAddr  wrSmDfltEnbIpAddr[WR_MAX_IP_TYPES]; 
   U16  wrSmDfltEnbPort     = 36412;
   U16  wrSmSctpPort       = 36412;

   U32 wrSmDfltEnbCfgTransTmrVal = 8000;
   U8 wrSmDfltEnbNoPlmn    = 1;
   U8 wrSmDfltNoMME        = 1;


   /*Neighbour Cell Configuration*/
   U8    wrSmDfltNeigCellNum[3]     = {2,3,4}; /**/
   U8    wrSmDfltNriIndex[3]        = {1,2,3}; /**/
   U32   wrSmDfltCid                = 0x4567;
   U16   wrSmDfltNeighCellID        = 0x45;
   U8    wrSmDfltCIO                = LWR_OFFSET_DB_PLUS_3; /*LWR_OFFSET_DB_PLUS_3*/
   U8    wrSmDfltNeighCellNoPlmn    = 1;
   U8    wrSmDfltNeighNoOfAnt       = 1;
   U16   wrSmDfltNeighCellPci[3]    = {2,3,4}; /**/
   U16   wrSmDfltNeighCellTAC[3]    = {0x58,0x00,0x00}; /**/
   U8    wrSmDfltQoff               = LWR_OFFSET_DB_MINUS_10;
   U8    wrSmDfltNeighBlockedList   = FALSE;
   U16   wrSmDfltEartcnNrDLEarfcn[3]    = {3100, 2850, 5230};/**/
   Bool  wrSmDfltEutranNrIsRemAllowed   = TRUE;
   Bool  wrSmDfltEutranNrIsHOAllowed    = TRUE;
   U8    wrSmDfltEutranNrCurRank        = 24;
   U8    wrSmDfltEutranNrPrevRank       = 34;

#ifdef OAM_CSFB
   U8     wrSmDfltVoipInCapable = 5;
#endif /* OAM_CSFB */

   /* measurement configuration */
   U32   wrSmDfltCsfbTmrVal   = 30000;
   U8    wrSmDfltReason       = 0;
   U32   wrSmDfltThresRSRP1   = 35;
   U32   wrSmDfltThresRSRQ1   = 32;
   U32   wrSmDfltThresRSRP2   = 35;
   U32   wrSmDfltThresRSRQ2   = 32;
   U32   wrSmDfltA1ThresRSRP  = 90;
   U32   wrSmDfltA2ThresRSRP  = 65;
   U32   wrSmDfltA5ThresRSRP1 = 70;
   U32   wrSmDfltA5ThresRSRP2 = 65;
   U32   wrSmDfltIntraA5ThresRSRP1 = 70;
   U32   wrSmDfltIntraA5ThresRSRP2 = 65;
   U32   wrSmDfltInterA5ThresRSRP1 = 70;
   U32   wrSmDfltInterA5ThresRSRP2 = 65;
   U32   wrSmDfltUtraB2ThresRSRP   = 70;
   U8    wrSmDfltGeranB2REDThres1RSRP = 65;
   U8    wrSmDfltGeranB2Thres1RSRQ = 30;
   S8    wrSmDfltGeranB2REDThres2     = 30;
   S16   wrSmDfltUtraB2ThresRSCP   = 65;
   S16   wrSmDfltUtraTddB2ThresRSCP   = 65;
   U16   wrSmDfltUtraB2ThresEcNo   = 30;
   U32   wrSmDfltUtraB2ThresRSRQ1  = 32;
   U8    wrSmDfltGeranB2HOThres1RSRP = 65;
   S8    wrSmDfltGeranB2HOThres2     = 30;
   U32   wrSmDfltHysteresis   = 0;
   U32   wrSmDfltTimeToTrig   = 9;
   U8    wrSmDfltTrigQty      = 0;
   U8    wrSmDfltRptQty       = 1;
   U8    wrSmDfltMaxRptCells  = 8;
   U32   wrSmDfltRptInterval  = 1;
   U32   wrSmDfltRptAmount    = 3;
   S32   wrSmDfltOffset       = 6;
   U8    wrSmDfltRptOnLeave   = 1;
   U8    wrSmDfltPurpose      = 1;
   U8    wrSmDfltCellType     = 1;
   U32   wrSmDfltCdmaThres1   = 20;
   S8    wrSmDfltIntraA3HoOffset = 10;
   U8    wrSmDfltTricePrdIntv = 10;
   U32   wrSmDfltEpocTmrVal = 1000;
   S8    wrSmDfltA3AnrOffset = 5;
   U32   wrSmDfltInterAnrA5ThresVal1 = 75;
   U32   wrSmDfltInterAnrA5ThresVal2 = 50;
   U8    wrSmDfltAnrCelValidAge = 10;
   U8    wrSmDfltHoRptCfgVal = 1; 
   U8    wrSmDfltAnrRptCfgVal = 1; 
   U8    wrSmDfltArpForEmerServ = 1;

   /*rsrq_ho*/
   U8    wrSmDfltThresRSRQ = 20;

   U32   wrSmDfltCdmaHysteresis  = 0;
   U32   wrSmDfltCdmaTimeToTrig  = 3;
   U8    wrSmDfltCdmaMaxRptCells = 1;
   U32   wrSmDfltCdmaRptInterval = 1;
   U32   wrSmDfltCdmaRptAmount   = 1;
   U8    wrSmDflddtPurpose       = 1;
   U32   wrSmDfltCdmaThresRSRP1  = 70;
   U32   wrSmDfltCdmaThresRSRQ1  = 10;
   U8    wrSmDfltSMeasure        = 0;
   U8    wrSmDfltTimeToTrigSfMedium = 3;
   U8    wrSmDfltTimeToTrigSfHigh   = 3;
   U16   wrSmDfltTEvalulation       = 0;
   U16   wrSmDfltTHystNormal        = 0;
   U16   wrSmDfltNCellChangeMedium  = 5;
   U16   wrSmDfltNCellChangeHigh    = 10;
   U16   wrSmDfltx2HandoverPrepTmr = 10000;
   U8    wrSmDfltRsrpFltrCoeff     = 4;
   U8    wrSmDfltRsrqFltrCoeff     = 4;
   U8    wrSmDfltMeasQtyCDMA2k     = 1;
   U32   wrSmDfltGapPatForInterFA  = 1;
   U32   wrSmDfltGapPatForInterRAT = 1;
   U32   wrSmDfltGapPatForANR      = 1;
   U8    wrSmDfltPreRegAllowed     = 0;
   U8    wrSmDfltPreRegZoneId      = 0;
   U8    wrSmDfltNumOf2ndPreRegZoneIdList = 1;
   U8    wrSmDfltSecondaryPreRegZoneId1   = 1;
   U8    wrSmDfltSecondaryPreRegZoneId2   = 0;

   U32   wrSmDfltReCfgType    = 0x00FFFFFF;
   U8    wrSmDfltStatus       = 1;
   U16   wrSmDfltDlEarFcn[3]    = {3100, 2850,5230}; /**/
   U8    wrSmDfltPriority    = 7;
   S8    wrSmDfltQRxLevMin    = -64;
   Bool  wrSmDfltPMaxUsage    = 1;
   S8    wrSmDfltPMax       = 0;
   U8    wrSmDfltTReselection    = 1;
   Bool  wrSmDfltSfUsage    = 0;
   U8    wrSmDfltTReselectionSFMed  = 3;
   U8    wrSmDfltTReselectionSFHigh = 3;
   U8    wrSmDfltSIntraSearch       = 31;
   U8    wrSmDfltSNonIntraSearch    = 31;
   U8    wrSmDfltServThresholdLow   = 31;
   U8    wrSmDfltThreshXHigh        = 31;
   U8    wrSmDfltThreshXLow         = 31;
   U8    wrSmDfltMeasBandwidth      = 3;
   Bool  wrSmDfltPresAntennaPort1   = 0;
   U8    wrSmDfltNeighCellCfg       = 1 << 6;
   U8    wrSmDfltsSearchPRel9       = 31;
   U8    wrSmDfltsSearchQRel9       = 31;
   S8    wrSmDfltDualMinRel9        = -30;
   U8    wrSmDfltReselecThreshQRel9 = 31;
   U8    wrSmDfltThreshXHighQR9     = 31;
   U8    wrSmDfltThreshXLowQR9      = 31;

   U8    wrSmDfltBandClass          = 0;
   U16   wrSmDfltArfcn              = 15;
   U8    wrSmDfltSearchRate         = 1;
   S8    wrSmDfltOffsetFreq         = 1;

   /*CDMA 1xRTT Band Class Configuration params*/
   U8 wrSmDfltHrpda              = LWR_HRPD;
   U8 wrSmDflt1XRTT               = LWR_1XRTT;
   U8 wrSmDfltBandClsReCfgType    = 0;
   U8 wrSmDfltBandClsStatus       = TRUE;
   U8 wrSmDfltBandClsCellReSelPri = 4;
   U8 wrSmDfltBandClassThresHigh  = 10;
   U8 wrSmDfltBandClassThresLow   = 5;
   U8 wrSmDfltCdmaSib8AdvTime     = 6;

   /* CDMA 1xRTT Neigh Freq configuration parameters */
   U8 wrSmDfltCdma1xNeighFreqReCfgType = 0;
   U8 wrSmDfltCdma1xSrchWindSize = 7;
   S8 wrSmDfltCdma1xOffsetFreq = 0;

   /* CDMA 1xRTT Mobility parameters */
   /* CNT = 0 */
   Bool wrSmDfltCdma1xSidIncl = 1; 
   U16  wrSmDfltCdma1xSid = 999;              

   /* CNT = 1 */
   Bool wrSmDfltCdma1xNidIncl = 1; 
   U16  wrSmDfltCdma1xNid = 5000;              

   /* CNT = 2 */
   Bool wrSmDfltCdma1xMultSidsIncl = 1;  
   Bool wrSmDfltCdma1xMultSids = 1;         

   /* CNT = 3 */
   Bool wrSmDfltCdma1xMultNidsIncl = 1;  
   Bool wrSmDfltCdma1xMultNids = 1;         

   /* CNT = 4 */
   Bool wrSmDfltCdma1xRegZoneIncl = 1;   
   U16  wrSmDfltCdma1xRegZone = 2;           

   /* CNT = 5 */
   Bool wrSmDfltCdma1xTotalZonesIncl = 1;
   U8   wrSmDfltCdma1xTotalZones = 7;       

   /* CNT = 6 */
   Bool wrSmDfltCdma1xZoneTimerIncl = 1;  
   U8   wrSmDfltCdma1xZoneTimer = 0;        

   /* CNT = 7 */
   Bool wrSmDfltCdma1xPktZoneIdIncl = 0;  
   U8   wrSmDfltCdma1xPktZoneId = 0;        

   /* CNT = 8 */
   Bool wrSmDfltCdma1xPzIdHysParamIncl = 0;
   Bool wrSmDfltCdma1xPzHystEnabled = 0;    
   Bool wrSmDfltCdma1xPzHystInfoIncl = 0;   
   Bool wrSmDfltCdma1xPzHystListLen = 0;    
   U8   wrSmDfltCdma1xPzHystActTimer = 0;   
   U8   wrSmDfltCdma1xPzHystTimerMul = 0;   
   U8   wrSmDfltCdma1xPzHystTimerExp = 0;   

   /* CNT = 9 */
   Bool wrSmDfltCdma1xPRevIncl = 1;    
   U8   wrSmDfltCdma1xPRev = 6;             

   /* CNT = 10 */
   Bool wrSmDfltCdma1xMinPRevIncl = 1;
   U8   wrSmDfltCdma1xMinPRev = 6;          

   /* CNT = 0 */
   Bool wrSmDfltCdma1xNegSlotCycIndxSupIncl = 0; 
   Bool wrSmDfltCdma1xNegSlotCycIndxSup = 0; 
   Bool wrSmDfltCdma1xEncModeIncl = 0;
   U8   wrSmDfltCdma1xEncMode = 0;          
   Bool wrSmDfltCdma1xEncSupIncl = 0;
   Bool wrSmDfltCdma1xEncSup = 0;           
   Bool wrSmDfltCdma1xSigEncSupIncl = 0;
   U8   wrSmDfltCdma1xSigEncSup = 0;        
   Bool wrSmDfltCdma1xMsgIntSupIncl = 0;
   Bool wrSmDfltCdma1xMsgIntSup = 0;        
   Bool wrSmDfltCdma1xSigIntSupINCLIncl = 0; 
   Bool wrSmDfltCdma1xSigIntSupINCL = 0;    
   Bool wrSmDfltCdma1xSigIntSupIncl = 0;
   U8   wrSmDfltCdma1xSigIntSup = 0;        

   Bool wrSmDfltCdma1xAuthIncl = 1;
   U8   wrSmDfltCdma1xAuth = 1;             

   Bool wrSmDfltCdma1xMaxNumAltoSoIncl = 1;
   U8   wrSmDfltCdma1xMaxNumAltoSo = 1;     

   Bool wrSmDfltCdma1xUseSyncIdIncl = 0;
   Bool wrSmDfltCdma1xUseSyncId = 0;        
   Bool wrSmDfltCdma1xMsInitPosLocSupIndIncl = 0;
   Bool wrSmDfltCdma1xMsInitPosLocSupInd = 0; 
   Bool wrSmDfltCdma1xMobQosIncl = 0;
   Bool wrSmDfltCdma1xMobQos = 0;           
   Bool wrSmDfltCdma1xBandClsInfoReqIncl = 0;
   Bool wrSmDfltCdma1xBandClsInfoReq = 0;   

   Bool wrSmDfltCdma1xBandClsIncl = 1;
   U8   wrSmDfltCdma1xBandCls = 3;          

   Bool wrSmDfltCdma1xBypassRegIndIncl = 0;
   U8   wrSmDfltCdma1xBypassRegInd = 0;     

   Bool wrSmDfltCdma1xAltBandClsIncl = 0;
   U8   wrSmDfltCdma1xAltBandCls = 0;       

   Bool wrSmDfltCdma1xMaxAddServInstIncl = 0; 
   U8   wrSmDfltCdma1xMaxAddServInst = 0;   

   Bool wrSmDfltCdma1xHomeRegIncl = 1;
   Bool wrSmDfltCdma1xHomeReg = 1;          

   Bool wrSmDfltCdma1xForSidRegIncl = 1;
   Bool wrSmDfltCdma1xForSidReg = 1;        

   Bool wrSmDfltCdma1xForNidRegIncl = 1;
   Bool wrSmDfltCdma1xForNidReg = 1;        

   Bool wrSmDfltCdma1xPowerUpRegIncl = 1;
   Bool wrSmDfltCdma1xPowerUpReg = 1;       

   Bool wrSmDfltCdma1xPowerDownRegIncl = 1;
   Bool wrSmDfltCdma1xPowerDownReg = 1;     

   Bool wrSmDfltCdma1xParmRegIncl = 1;
   Bool wrSmDfltCdma1xParmReg = 1;          

   Bool wrSmDfltCdma1xRegPrdIncl = 1;
   U8   wrSmDfltCdma1xRegPrd = 58;           

   Bool wrSmDfltCdma1xRegDistIncl = 0;
   U16  wrSmDfltCdma1xRegDist = 0;          

   Bool wrSmDfltCdma1xPrefMsIdTypeIncl = 1;
   U8   wrSmDfltCdma1xPrefMsIdType = 0;     

   Bool wrSmDfltCdma1xExtPrefMsIdTypeIncl = 0;
   U8   wrSmDfltCdma1xExtPrefMsIdType = 0;  

   Bool wrSmDfltCdma1xMeIdReqdIncl = 0;
   Bool wrSmDfltCdma1xMeIdReqd = 0;         

   Bool wrSmDfltCdma1xMccIncl = 1;
   U16  wrSmDfltCdma1xMcc = 1023;              

   Bool wrSmDfltCdma1xImsi1112Incl = 1;
   U8   wrSmDfltCdma1xImsi1112 = 127;         

   Bool wrSmDfltCdma1xImsiTSupIncl = 1;
   Bool wrSmDfltCdma1xImsiTSup = 0;         
   Bool wrSmDfltCdma1xRecntMsgIndIncl = 0;
   Bool wrSmDfltCdma1xRecntMsgInd = 0;      
   Bool wrSmDfltCdma1xRerModeSupIncl = 0; 
   Bool wrSmDfltCdma1xRerModeSup = 0;       
   Bool wrSmDfltCdma1xTzkModeSupIncl = 0;
   Bool wrSmDfltCdma1xTzkModeSup = 0;       
   Bool wrSmDfltCdma1xTzkIdIncl = 0;
   Bool wrSmDfltCdma1xTzkId = 0;
   Bool wrSmDfltCdma1xPilRepIncl = 0;
   Bool wrSmDfltCdma1xPilRep = 0;           
   Bool wrSmDfltCdma1xSdbSupIncl = 0;
   Bool wrSmDfltCdma1xSdbSup = 0;           
   Bool wrSmDfltCdma1xAutoFcsoAllIncl = 0;
   Bool wrSmDfltCdma1xAutoFcsoAll = 0;      
   Bool wrSmDfltCdma1xSdbInRcnmIncl = 0;
   Bool wrSmDfltCdma1xSdbInRcnm = 0;        
   Bool wrSmDfltCdma1xFpcFchIncl = 1;
   U8   wrSmDfltCdma1xFpcFchInitSetptRC3 = 7;
   U8   wrSmDfltCdma1xFpcFchInitSetptRC4 = 11;
   U8   wrSmDfltCdma1xFpcFchInitSetptRC5 = 7;
   U8   wrSmDfltCdma1xFpcFchInitSetptRC11 = 8;
   U8   wrSmDfltCdma1xFpcFchInitSetptRC12 = 0;
   Bool wrSmDfltCdma1xTAddIncl = 0;
   U8   wrSmDfltCdma1xTAdd = 0;             
   Bool wrSmDfltCdma1xPilotInclIncl = 1;
   U8   wrSmDfltCdma1xPilotInc = 2;

   Bool wrSmDfltCdma1xRandIncl = 0;
   U32  wrSmDfltCdma1xRand = 0;             

   Bool wrSmDfltCdma1xLpSecIncl = 1;
   U8   wrSmDfltCdma1xLpSec = 16;
   Bool wrSmDfltCdma1xLtmOffIncl = 1;
   U8   wrSmDfltCdma1xLtmOff = 54;           
   Bool wrSmDfltCdma1xDayltIncl = 1;
   Bool wrSmDfltCdma1xDaylt = 1;            
   Bool wrSmDfltCdma1xGcsnaL2ActTimerIncl = 1;
   U8   wrSmDfltCdma1xGcsnaL2ActTimer = 13;
   Bool wrSmDfltCdma1xGcsnaSeqCtxtTimerIncl = 1;
   U8   wrSmDfltCdma1xGcsnaSeqCtxtTimer = 22;
   U8   wrSmDfltCdma1xReserved = 0;

   /* CDMA 1xRTT SIB8 parameters */
   Bool  wrSmDfltSib8CdmaEutraTimeSync = TRUE;
   U8    wrSmDfltSib8SfMedium = 0;
   U8    wrSmDfltSib8SfHigh = 0;
   U8    wrSmDfltSib8TReselection = 0;

   /* CDMA 1xRTT Default Registration parameters */
   U16  wrSmDfltCdma1xRegParamsSid = 0;
   U16  wrSmDfltCdma1xRegParamsNid = 0;
   Bool wrSmDfltCdma1xRegParamsMulSID = 0;
   Bool wrSmDfltCdma1xRegParamsMulNID = 0;
   Bool wrSmDfltCdma1xRegParamsHomeReg = 0;
   Bool wrSmDfltCdma1xRegParamsForeignSIDReg = 0;
   Bool wrSmDfltCdma1xRegParamsForeignNIDReg = 0;
   Bool wrSmDfltCdma1xRegParamsParamReg = 0;
   Bool wrSmDfltCdma1xRegParamsPowerUpReg = 0;
   /* Start Fix for SIB8 */
   U16  wrSmDfltCdma1xRegParamsRegPeriod = 0;
   /* end of SIB8 fix */
   U16  wrSmDfltCdma1xRegParamsRegZone = 0;
   U8   wrSmDfltCdma1xRegParamsTotalZone = 0;
   U8   wrSmDfltCdma1xRegParamsZoneTimer = 0;
   Bool wrSmDfltCdma1xRegParamsPowerDownReg = 0;
 
   /* CDMA 1xRTT Default access class barring parameters */
   U8 wrSmDfltCdma1xAcBarring0to9 = 0;
   U8 wrSmDfltCdma1xAcBarring10 = 0;
   U8 wrSmDfltCdma1xAcBarring11 = 0;
   U8 wrSmDfltCdma1xAcBarring12 = 0;
   U8 wrSmDfltCdma1xAcBarring13 = 0;
   U8 wrSmDfltCdma1xAcBarring14 = 0;
   U8 wrSmDfltCdma1xAcBarring15 = 0;
   U8 wrSmDfltCdma1xAcBarringMsg = 0;
   U8 wrSmDfltCdma1xAcBarringReg = 0;
   U8 wrSmDfltCdma1xAcBarringEmg = 0;

   /* CDMA 1xRTT CSFB parameters */
   LwrRatPriorityList  wrSmDfltPlmnRatPriorList = {5, {LWR_CSFB_RAT_PRIOR_UTRA_TDD, 
         LWR_CSFB_RAT_PRIOR_UTRA_FDD, LWR_CSFB_RAT_PRIOR_GERAN,
         LWR_CSFB_RAT_PRIOR_CDMA_CDMA2000_HRPD, LWR_CSFB_RAT_PRIOR_CDMA_CDMA2000_1XRTT}}; 
   /* Redirection or Ho choince per PLMN Default value: Redirection (1) */
   U8          wrSmDfltPlmnRedirSuppLst[6]    = {1, 1, 1, 1, 1, 1}; 
   U8          wrSmDfltCsfbToUtraMeasSup    = 0; /* LWR_CSFB_WITHOUT_MEAS - 0
                                                    LWR_CSFB_WITH_MEAS    - 1
                                                    */
   U8          wrSmDfltCsfbToCdmaMeasSup    = 0; /* LWR_CSFB_WITHOUT_MEAS - 0
                                                    LWR_CSFB_WITH_MEAS    - 1
                                                    */
   U8          wrSmDfltEcsfbMeasSup         = 0; /* LWR_ECSFB_NOT_SUPPORTED - 0
                                                    LWR_ECSFB_WITHOUT_MEAS  - 1
                                                    LWR_ECSFB_WITH_MEAS     - 2
                                                    */
   Bool        wrSmDfltDualRxSup = TRUE;
   Bool        wrSmDfltCdma1xrttRegSup = TRUE;
   Bool        wrSmDfltDualRxTxSup = FALSE; /*RRC Rel 10 Upgrade*/
   U16         wrSmDfltCdmaMktId = 0;
   U8          wrSmDfltCdmaSwitchNum = 0;
   U16         wrSmDfltCdmaCellId = 0;
   U8          wrSmDfltCdmaSectorNum = 0;
   U64         wrSmDfltSecId = 0x22212;
   Bool        wrSmDfltSecIdIsOctStrgPres   = TRUE;
   U64         wrSmDfltSecIdPerNgh = 0x111;
   U8          wrSmDfltCdmaSystemType = 0;
   U16          wrSmDfltCdmaChannelNum = 0;
   U8          wrSmDfltCdmaPilotOneWay = 0;
   U16         wrSmDfltpilotPnPhase         = 0;
   U8          wrSmDfltpilotStrength        = 0; 

   /* ANR Ctrl Info */
   /* ANR ALGO INFO */
   U32         wrSmDfltAnrAlgoInfoReCfgMask       = 0;
   U8          wrSmDfltAnrAlgoInfoMaxNclSize      = 56;
   U8          wrSmDfltAnrAlgoInfoMaxNrtSize      = 64;
   U8          wrSmDfltAnrAlgoInfoMax1xRttNclSize = 52;
   U8          wrSmDfltAnrAlgoInfoMax1xRttNrtSize = 56;
   U8          wrSmDfltAnrAlgoInfoMaxHrpdNclSize  = 52;
   U8          wrSmDfltAnrAlgoInfoMaxHrpdNrtSize  = 56;

#ifdef MME_LDBAL
   U8         wrSmDfltNumOfPriorityGrps = 4;
   U16        wrSmDfltMmeGrpID1         = 0x1111;
   U16        wrSmDfltMmeGrpID2         = 0x2222;
   U16        wrSmDfltMmeGrpID3         = 0x3333;
   U16        wrSmDfltMmeGrpID4         = 0x4444;
   U16        wrSmDfltMmeGrpIDPriority1 =  1; 
   U16        wrSmDfltMmeGrpIDPriority2 =  2; 
   U16        wrSmDfltMmeGrpIDPriority3 =  3; 
   U16        wrSmDfltMmeGrpIDPriority4 =  4; 
#endif


   /* Black List Cell Info */

   /* Action Tyupe Add */
   U8       wrSmDfltBlCellInfoReCfgType[LWR_MAX_BL_CELL_INFO]    = {0,0};    
   U8       wrSmDfltBlCellInfoBlackListIdx[LWR_MAX_BL_CELL_INFO] = {1,2};
   Bool     wrSmDfltBlCellInfoStatus[LWR_MAX_BL_CELL_INFO]       = {TRUE, TRUE};
   Bool     wrSmDfltBlCellInfoPciType[LWR_MAX_BL_CELL_INFO]      = {FALSE, FALSE};
   U16      wrSmDfltBlCellInfoStartPci[LWR_MAX_BL_CELL_INFO]     = {277, 278};
   U8       wrSmDfltBlCellInfoPciRange[LWR_MAX_BL_CELL_INFO]     = {0, 0};
   U16      wrSmDfltBlCellInfoEarfcnUl[LWR_MAX_BL_CELL_INFO]     = {21100, 23230};
   U16      wrSmDfltBlCellInfoEarfcnDl[LWR_MAX_BL_CELL_INFO]     = {3100, 5230};


   /* Fill the below variable with value "number of BL cell Info" + 1*/
   U8 wrSmDfltBlCellInfoIndex = 2;

   /* Black List EUTRAN list */

   /* Action Tyupe Add */
   LwrPlmnId plmnId_1 = { 2, {0,1,1}, {1,1}};

   U8          wrSmDfltEutranBlNrReCfgType[LWR_MAX_EUTRAN_BL_NR]     = {0};
   U8          wrSmDfltEutranBlNrBlackListIdx[LWR_MAX_EUTRAN_BL_NR]  = {1};
   Bool        wrSmDfltEutranBlNrStatus[LWR_MAX_EUTRAN_BL_NR]        = {TRUE};
   U32         wrSmDfltEutranBlNrEnbId[LWR_MAX_EUTRAN_BL_NR]         = {123};
   U8          wrSmDfltEutranBlNrTargetCellNum[LWR_MAX_EUTRAN_BL_NR] = {0};
   LwrEnbType  wrSmDfltEutranBlNrEnbType[LWR_MAX_EUTRAN_BL_NR]       = {WR_ENODEB_TYPE_MACRO};
   U16         wrSmDfltEutranBlNrTac[LWR_MAX_EUTRAN_BL_NR]           = {503};
   LwrPlmnId   wrSmDfltEutranBlNrEnbPlmn[LWR_MAX_EUTRAN_BL_NR]       = {
                            {2, {0,1,1}, {1,1}},
                                                                       };
   U16         wrSmDfltEutranBlNrPhyCellId[LWR_MAX_EUTRAN_BL_NR]     = {297};

   /* Fill the below variable with value "number of BL eutran Nr" + 1*/
   U8  wrSmDfltEutranBlNrIndex = 1;

   /* Black List CDMA 1xRTT List */

   /* Action Tyupe Add */
   U16      wrSmDfltCdmaBlNrReCfgType[LWR_MAX_CDMA_1XRTT_BL_NR]    = {0};
   U8       wrSmDfltCdmaBlNrBlackListIdx[LWR_MAX_CDMA_1XRTT_BL_NR] = {1};
   Bool     wrSmDfltCdmaBlNrStatus[LWR_MAX_CDMA_1XRTT_BL_NR]       = {TRUE};
   U16      wrSmDfltCdmaBlNrSId[LWR_MAX_CDMA_1XRTT_BL_NR]       = {29971};
   U16      wrSmDfltCdmaBlNrNId[LWR_MAX_CDMA_1XRTT_BL_NR]          = {54877};
   U16      wrSmDfltCdmaBlNrBaseId[LWR_MAX_CDMA_1XRTT_BL_NR]       = {27777};
   U16      wrSmDfltCdmaBlNrPnOffset[LWR_MAX_CDMA_1XRTT_BL_NR]     = {307};
   U16      wrSmDfltCdmaBlNrMarketId[LWR_MAX_CDMA_1XRTT_BL_NR]     = {0x123F};
   U16      wrSmDfltCdmaBlNrCellId[LWR_MAX_CDMA_1XRTT_BL_NR]       = {0x123};
   U8       wrSmDfltCdmaBlNrSectorNum[LWR_MAX_CDMA_1XRTT_BL_NR]    = {3};
   U32      wrSmDfltCdmaBlNrBandClass[LWR_MAX_CDMA_1XRTT_BL_NR]    = {0};
   U32      wrSmDfltCdmaBlNrArfcn[LWR_MAX_CDMA_1XRTT_BL_NR]        = {15};

   /* Fill the below variable with value "number of BL CDMA 1xRTT Nr" + 1*/
   U8 wrSmDfltCdma1xBlNrIndex = 1;

   /* Black List CDMA HRPD List */

   /* Action Tyupe Add */
   U16      wrSmDfltCdmaHrpdBlNrReCfgType[LWR_MAX_CDMA_HRPD_BL_NR]    = {0};
   U8       wrSmDfltCdmaHrpdBlNrBlackListIdx[LWR_MAX_CDMA_HRPD_BL_NR] = {1};
   Bool     wrSmDfltCdmaHrpdBlNrStatus[LWR_MAX_CDMA_HRPD_BL_NR]       = {TRUE};
   U16      wrSmDfltCdmaHrpdBlNrColorCode[LWR_MAX_CDMA_HRPD_BL_NR]    = {79};
   U16      wrSmDfltCdmaHrpdBlNrBsmId[LWR_MAX_CDMA_HRPD_BL_NR]        = {57};
   U16      wrSmDfltCdmaHrpdBlNrBscId[LWR_MAX_CDMA_HRPD_BL_NR]        = {0};
   U16      wrSmDfltCdmaHrpdBlNrDpssId[LWR_MAX_CDMA_HRPD_BL_NR]       = {5};
   U16      wrSmDfltCdmaHrpdBlNrBtsId[LWR_MAX_CDMA_HRPD_BL_NR]        = {351};
   U16      wrSmDfltCdmaHrpdBlNrSectorId[LWR_MAX_CDMA_HRPD_BL_NR]     = {3};
   U32      wrSmDfltCdmaHrpdBlNrBandClass[LWR_MAX_CDMA_HRPD_BL_NR]    = {0};
   U32      wrSmDfltCdmaHrpdBlNrArfcn[LWR_MAX_CDMA_HRPD_BL_NR]        = {15};
   U16      wrSmDfltCdmaHrpdBlNrPnOffset[LWR_MAX_CDMA_HRPD_BL_NR]     = {347};


   /* Fill the below variable with value "number of BL CDMA HRPD Nr" + 1*/
   U8 wrSmDfltCdmaHrpdBlNrIndex    = 1;
   U16 wrSmDfltS1MmeCfgTrfrTmr     = 10000;
   U16 wrSmDfltX2SnSTatTrfrTmr     = 10000;
   U16 wrSmDfltX2RstRtryCntTmr     = 0xFF;
   U16 wrSmDfltX2SetupRtryCntTmr   = 0xFF;
   U16 wrSmDfltIntSigInactivtTmr   = 10;
   U16 wrSmDfltHRPDCpltTmr         = 10000;
   U16 wrSmDfltIntraEnbHoCpltTmr   = 10000;
   U16 wrSmDfltMeasureRptToWaitTmr = 10000;
   U16 wrSmDfltS1MmeStatTrfrTmr    = 10000;
   U16 wrSmDfltS1RstRtryCntTmr     = 4;

   U16 wrSmDfltS1SetupTmr         = 30000;
   U16 wrSmDfltUpdateTmr          = 60000;
   U16 wrSmDfltS1TimeToWait       = 5;
   U16 wrSmDfltS1RtryCntTmr       = 4; 
   U16 wrSmDfltS1ResetRetryTmr    = 60000;
   U16 wrSmDfltX2SetupTmr         = 10000;
   U16 wrSmDfltX2SetTimeToWait    = 2;
   U16 wrSmDfltX2UpdateTmr      = 10000;
   U16 wrSmDfltX2UpdateTimeToWait = 2;
   U16 wrSmDfltX2UpdateRetryCount = 3;
   U16 wrSmDfltIntNoWaitRetryTmr  = 60000;
   U16 wrSmDfltX2ResetRetryTmr    = 60000;

   
   U32 wrSmDfltRrcConSetupTmr      = WR_TMR_RRCCON_VAL;
   U32 wrSmDfltRrcConReCfgTmr      = WR_TMR_RRC_RECONFIG_VAL;
   U32 wrSmDfltRrcConReEstTmr      = WR_TMR_REESTABLISH_VAL;
   U32 wrSmDfltRrcSecModCmdTmr     = WR_TMR_SECUCMD_VAL;
   U32 wrSmDfltRrcConRelTmr        = WR_TMR_CONREL_VAL;
   U32 wrSmDfltRrcUeCapEnqTmr      = WR_TMR_CAPENQ_VAL;
   U32 wrSmDfltRrcHandoverPrepTmr  = 1000;
   U32 wrSmDfltRrcUeInfReqTmr      = 1000;

   U16 wrSmDflts1InitialContextSetupTmr         = 65535;
   U16 wrSmDflts1ErabReleaseIndicationTmr       = 65535;
   U16 wrSmDflts1UeContextReleaseTmr            = 65535;
   U16 wrSmDflts1PathSwitchTmr                  = 65535;
   U16 wrSmDflts1HandoverPreperationTmr         = 65535;
   U16 wrSmDflts1RelocOveallTmr                 = 65535;
   U16 wrSmDflts1HandoverCancelTmr              = 400;
   U16 wrSmDfltx2HandoverPreperationTmr         = 10000;
   U16 wrSmDfltx2RelocOverallTmr                = 10000;
   U16 wrSmDfltx2StoreUeContextTmr              = 10000;
   U16 wrSmDfltx2SnStatusTransferTmr            = 10000; 
   U16 wrSmDfltinternalSubCellChangeTmr         = 10;
   U16 wrSmDfltinternalSubCellPdcpControlTmr    = 10;
   U16 wrSmDfltehrpdOptHoCompleteTmr            = 10000; 
   U16 wrSmDfltintraEnbHoCompleteTmr            = 10000;
   U16 wrSmDfltmeasurementReportToWaitTmr       = 10000;
   U16 wrSmDfltinternalResourceSetupTmr         = 10;
   U16 wrSmDfltinternalResourceReleaseTmr       = 10; 
   U16 wrSmDfltinternalSecurityControlTmr       = 10; 
   U16 wrSmDfltinternalRrcDataTransferTmr       = 10;
   U16 wrSmDfltinternalForwardingPathSetupTmr   = 10; 
   U16 wrSmDfltinternalReestablishControlTmr    = 10;
   U16 wrSmDfltinternalBufferFlushTmr           = 10;
   U16 wrSmDfltinternalDataTransferStartTmr     = 10; 
   U16 wrSmDfltinternalDataForwardEndIndTmr     = 10;
   U16 wrSmDfltinternalReestablishTimeToWaitTmr = 10;
   U16 wrSmDfltinternalUserInactivityTmr        = 10;
   U16 wrSmDfltinternalSignalingInactivityTmr   = 10;
   U16 wrSmDfltinternalUnassignedTmr            = 10;
   U16 wrSmDfltinternalNoWaitRetryTmr           = 60000;
   U16 wrSmDfltinternalRrcResetTmr              = 10; 
   U16 wrSmDfltinternalEnbResetTmr              = 10;
   U16 wrSmDfltinternalSolicitMeasurementTmr    = 10;
   U16 wrSmDfltinternalRtdInfoReqTmr            = 10;


   Bool wrSmDfltDirectFwd       = 0;
   Bool wrSmDfltPlmnEnbCacUsage = 0;
   Bool wrSmDfltStatRptReqd     = 1;


   U16 wrSmDfltCellRsrvExtn    = 1;        
   S16 wrSmDfltqRxlevmin       = -50;        
   U16 wrSmDfltqRxlevminoffset = 0x01;

   U16 wrSmDfltSubframeAssignment    = 0x01;        /* 5ms switch frame   */
   U16 wrSmDfltSpecialSubfrmPatterns = 0x07;     /* based on latest system reqs.  */

   U16 wrSmDfltSiPeriodicity[] = {0x01, 0x02, 0x03, 0x03}; 
   /* 16, 32, 64, 64 radioframes for SIB2, 3, 4, 5 respectively */
   U16 wrSmDfltSiMapInfoCnt[] = {0, 0x01, 0x01, 0x01}; 
   /* Si0 has SIB2 by default, Si1, 2, 3 has SIB3,4,5 respectively */
   U16 wrSmDfltSiMapInfoVal[] = {0, 0, 0x01, 0x02}; 
   /* SIB2, 3, 4, 5 respectively */
   U8 wrSmDfltNumSi = 4;

   U16 wrSmDfltSiWindowLength     = 0x05;           /* 20 ms */
   U16 wrSmDfltSystemInfoValueTag = 0x00;        /* Zero */

   U8 wrSmDfltPreambleFormat        = 0x01;          
   U16 wrSmDfltRaOccasionSize       = 0x01;        
   U16 wrSmDfltRaOccasionSfnEnum    = 0x01;    
   U16 wrSmDfltRaOccasionSubFrmNum0 = 0x01;
   U16 wrSmDfltRACHCfgMsgSizeGrpA   = 11; 

   U16 wrSmDfltPrachResource        = 0x01;     

   U16 wrSmDfltPagDrx          = 0;

   CtfSCSpacing wrSmDfltScSpacing   = CTF_SC_SPACING_15KHZ; 
   CtfCPCfg wrSmDfltCycPfx          = CTF_CP_NORMAL;              

   U8 wrSmDfltPUCCHFixBw     = 0x03;         
   U8 wrSmDfltPUCCHCyclShift = 0x01;    


/* UE Timers and   s */
   U16 wrSmDfltTmrsAndConstsT300 = 0x04;       
   U16 wrSmDfltTmrsAndConstsT301 = 0x04;      
   /* Fix for ccpu00137900 - Updating the t304 timer value */
   U16 wrSmDfltTmrsAndConstsT304 = 0x04;      
   U16 wrSmDfltTmrsAndConstsT310 = 0x06;
   U16 wrSmDfltTmrsAndConstsN310 = 0x07;
   U16 wrSmDfltTmrsAndConstsT311 = 0x01;
   U16 wrSmDfltTmrsAndConstsN311 = 0x00;  

   U16 wrSmDfltUlCarrierFreq = 0x00;     
   U16 wrSmDfltUlBandwidth   = 0x03;                  /* 50 RB */
   U16 wrSmDfltDlBandwidth   = 0x03;                  /* 50 RB */

   U16 wrSmDfltTimeAlignTmrCmn = 0x07;               /* infinity */

/* Security Cfg */
   U16 wrSmDfltCiphAlgo     = 0;           
   U16 wrSmDfltIntgProtAlgo = 0;      

/* Priority table for LCG, LCH Priority, PBR and Bucket Size.
 * The values are as per the requirement of SPS and UL GBR bearers. 
 * These feature will be affected if values are changed. */
PUBLIC WrPrioInfo wrSmDfltPrioInfoTbl = 
{
   { /* SRBs */
      {0, 1, NhuLogChannelConfigul_SpecificParamsprioritisedBitRateinfinityEnum, \
         NhuLogChannelConfigul_SpecificParamsbucketSizeDurationms50Enum},           /* SRB 1 */
      {0, 1, NhuLogChannelConfigul_SpecificParamsprioritisedBitRateinfinityEnum, \
         NhuLogChannelConfigul_SpecificParamsbucketSizeDurationms50Enum}            /* SRB 2 */
   },
   { /* DRBs */
      {3, 4, 0, 0}, /* Unused (To increase readability, directly mapped to QCI) */
      {2, 2, NhuLogChannelConfigul_SpecificParamsprioritisedBitRatekBps64Enum, \
         NhuLogChannelConfigul_SpecificParamsbucketSizeDurationms50Enum},              /* GBR bearer QCI 1 with SPS belongs LCG1.BSD set to 50ms as service type is Conversation Voice and the data pattern may not be bursty in nature*/
      {2, 4, NhuLogChannelConfigul_SpecificParamsprioritisedBitRatekBps256Enum, \
         NhuLogChannelConfigul_SpecificParamsbucketSizeDurationms100Enum},              /* GBR bearer QCI 2. BSD is set to 100ms as service type is Conversation Video and the data pattern may not be bursty in nature*/
      {2, 6, NhuLogChannelConfigul_SpecificParamsprioritisedBitRatekBps256Enum, \
         NhuLogChannelConfigul_SpecificParamsbucketSizeDurationms1000Enum},              /* GBR bearer QCI 3.BSD is set to 1000ms as service type is Real Time Gaming and the data pattern may be bursty in nature*/
      {2, 8, NhuLogChannelConfigul_SpecificParamsprioritisedBitRatekBps256Enum, \
         NhuLogChannelConfigul_SpecificParamsbucketSizeDurationms1000Enum},              /* GBR bearer QCI 4.BSD is set to 1000ms as service type is Non-Conversational Video and the data pattern may be bursty in nature */
      {0, 1, NhuLogChannelConfigul_SpecificParamsprioritisedBitRateinfinityEnum, \
         NhuLogChannelConfigul_SpecificParamsbucketSizeDurationms50Enum},                /* IMS Signalling QCI 5 */
      {3, 10, NhuLogChannelConfigul_SpecificParamsprioritisedBitRatekBps8Enum, \
         NhuLogChannelConfigul_SpecificParamsbucketSizeDurationms50Enum},                /* Non-GBR bearers 6 */
      {3, 12, NhuLogChannelConfigul_SpecificParamsprioritisedBitRatekBps8Enum, \
         NhuLogChannelConfigul_SpecificParamsbucketSizeDurationms50Enum},                /* Non-GBR bearers 7 */
      {3, 14, NhuLogChannelConfigul_SpecificParamsprioritisedBitRatekBps8Enum, \
         NhuLogChannelConfigul_SpecificParamsbucketSizeDurationms50Enum},                /* Non-GBR bearers 8 */
      {3, 16, NhuLogChannelConfigul_SpecificParamsprioritisedBitRatekBps8Enum, \
         NhuLogChannelConfigul_SpecificParamsbucketSizeDurationms50Enum}                 /* Non-GBR bearers 9 */
   }
};

/* SRB */
   U16 wrSmDfltSrbPollRetrans   = 8;   
   U16 wrSmDfltSrbPollPdu       = 7;      
   U16 wrSmDfltSrbPollByte      = 14;   
   U16 wrSmDfltSrbMaxReTxThresh = 3;  
//   U16 wrSmDfltSrbTreordrng     = 2;     
   U16 wrSmDfltSrbTStatProhibit = 2;  

   U16 wrSmDfltMacCfgPeriodicBSRTimer = NhuMAC_MainConfigul_SCH_ConfigperiodicBSR_Timersf10Enum;
   U16 wrSmDfltMacCfgRetxBSRTimer     = NhuMAC_MainConfigul_SCH_ConfigretxBSR_Timersf320Enum;     /* sf320 */
   U8 wrSmDfltMacCfgTtiBundling       = FALSE;              
   U16 wrSmDfltNumOfTxAnt = 2;
   /*DRX QCI*/
   Bool wrSmDfltisDrxQCIEnabled    = FALSE;
/* Removed DRX flag*/
   Bool wrSmDfltIsDrxEnabled       = FALSE;
   Bool  wrSmDfltIsCnmEnabled     = FALSE;
   U8  wrSmDfltTddBoardType       = 1;/* 2- DM, 1 - AVNET */
   U16 wrSmDfltDrxInactvTmr        = 0;
   U16 wrSmDfltDrxReTxTmr          = 0;
   U16 wrSmDfltDrxLongCycleGbr     = 0;
   U16 wrSmDfltDrxLongCycleNonGbr  = 0;
   U16 wrSmDfltAnrDrxLongCycle     = 11;
   U16 wrSmDfltAnrDrxOnDurationTmr = 7;
   /* Fix for ccpu00132334 - To read DRX Inactivity timer for ANR*/
   U16 wrSmDfltAnrDrxInActivTmr = 8; 

   U16 wrSmDfltMacCfgPeriodicPHR_Timer   = 3;     
   U16 wrSmDfltMacCfgProhibitPHRTimer    = 0;     
   U16 wrSmDfltMacCfgDlPathlossChange    = 1;    
   U16 wrSmDfltDedPhyCfgPa 		 = 2;               /* dB0 */
   S16 wrSmDfltP0UEPUSCH                 = 0;               /* 0 */
   U16 wrSmDfltDeltaMCSEnabled           = 0;               /* en0 */
   U8 wrSmDfltAccumulationEnabled        = TRUE;               /* en0 */
   S16 wrSmDfltP0UEPUCCH                 = 0;               /* 0 */

   U16 wrSmDfltpSRSOffset                = 7;               /* 3(-6dB) */
   U16 wrSmDfltFilterCoef        = 4;               /* fc4 */
#ifdef LTE_TDD   
   U16 wrSmDfltBetaOffACKIdx             = 5; /*work around to avoid L1 issue */
#else
   U16 wrSmDfltBetaOffACKIdx             = 5;
#endif 
#ifdef LTE_QCOM
   /* NOTE: Refer to FAPI setting in CL, before changing these */
   U16 wrSmDfltBetaOffRIIdx              = 6;
   U16 wrSmDfltBetaOffCQIIdx             = 6;
#else  
   U16 wrSmDfltBetaOffRIIdx              = 5; /*ccpu00143246: Changed from 
                                                12 to 5 to avoid FLE crash*/
   U16 wrSmDfltBetaOffCQIIdx             = 5;
#endif
   U16 wrSmDfltCqiRprtModeAperiodic      = 3;               /* rm20 */

   U16 wrSmDfltCqiNomPDSCHRSEPREoff      = 0;               /* 0 */
   U16 wrSmDfltCqiPUCCHResIdx            = 0;               /* 0 */
   U8 wrSmDfltSimultAckNackAndCQI        = TRUE;               /* FALSE */
   U16 wrSmDfltRiConfigIdx               = 181;               /* FDD - 483 / TDD - 484 */
   U8  wrSmDfltSrPrdcty                  = 2; 
   U8  wrSmDfltCqiPrdcty                 = 5;
   U8  wrSmDfltNumSrResPerTti            = 2; 
   U8  wrSmDfltNumCqiResPerTti           = 2;

   U16 wrSmDfltSrsHoppingBw        = 0;               /* hbw0 */

   U16 wrSmDfltFreqDomainPos             = 0;               /* 0 */
   U8 wrSmDfltDuration                   = TRUE;               /* TRUE */
   U16 wrSmDfltCqiPmiConfigIdx           = 37;               /* FDD 25 TDD 24 */
   U16 wrSmDfltSrsConfigIdx              = 20;               /* FDD 20 / TDD 31 */
   U16 wrSmDfltTransmissionComb          = 0;               /* 0 */
   U16 wrSmDfltDedCyclicShift            = 0;               /* cs0 */

/* Scheduling Request Configuration */
   U16 wrSmDfltSrPUCCHResIdx             = 0;             
   U16 wrSmDfltDsrTransMax               = 3; /*n16*/  /*4=n64 */
/* Antenna Configuration */
   U16 wrSmDfltTransMode                 = NhuAntennaInfoDedicatedtransmissionModetm1Enum;               /* tm1 */
/* DRB PDCP Config  */
   U16 wrSmDfltDiscardTimer              = 7;               /* infinity */
   U8 wrSmDfltStatusReportReqd           = TRUE;               /* TRUE */

   U16 wrSmDfltDrbTPollRetx              = 15;               /* ms80 */
   U16 wrSmDfltDrbTPollPDU               = 2;               /* Enum{4,8,16,32,64,128,256,Inf}*/
   U16 wrSmDfltDrbTPollByte              = 13;               /*NhuPollBytekB3000Enum kb125 */
   U16 wrSmDfltDrbTMaxRetxThres          = 5;               /* t4 */
//   U16 wrSmDfltDrbTReordrng              = 2;               /* ms80 */
   U16 wrSmDfltDrbTStatProh              = 2;               /* ms60 */

   CmLteUeCategory wrSmDfltUECategory    = CM_LTE_UE_CAT_4;  
   CtfPdschPaParam wrSmDfltPdschPA       = 2;           /* same as wrSmDfltDedPhyCfgPa */
   CtfPucchRepFactor wrSmDfltRepFact     = CTF_PUCCH_REP_FACTOR_N2;   
   U16 wrSmDfltN1PUCCHRep                = 0;              
   U16 wrSmDfltFormatInd                 = 1;               /* 1 */
   U16 wrSmDfltSubBandK                  = 0;               

/* Ctf Cfg Info */
   CtfUeTxAntMode wrSmDfltAntInfoTxMode    = CTF_UE_TX_ANT_MODE_TM3;
   CtfUeTxAntSelectType wrSmDfltAntennaSel = 0;               /*  Closed 0 | Open 1 */

   U16 wrSmDfltTxModeEnum     = 1;               /* TM1 */
   U16 wrSmDfltAPrdModeEnum   = 3;      /* MOD30 */
/* Periodic CQI reporting mode */
   U16 wrSmDfltPrdModeEnum    =  0;      

   U16 wrSmDfltPrdicityEnum   = 2;      
   S8 wrSmDfltCqiOffst        = 0;      
   S8 wrSmDfltCqiCfgk         = 2;      
   U16 wrSmDfltCqiPmiCfgIdx   = 39;      
   U8 wrSmDfltSubframeOffst   = 0;      
/* TM4 changes */
   Bool wrSmDfltTmAutoConfig  = TRUE;
   U8 wrSmDfltPreferredTm     = 4;
   U8 wrSmDfltTransModeUse    = 0;
   Bool wrCodeBookRstnPres    = FALSE;
   U32 wrPmiBitMap0          = 0xc0000000; /* corresponds to n2TxAntenna-tm3 */
   U32 wrPmiBitMap1           = 0;
   U8 wrSmDfltMaxUlHqTx       = 4;  /* Maximum number of transmissions in UL */
       
/*!< Delta HARQ offset */
   U8 wrSmDfltDeltaHqOffst    = 0;    
/* Timer configuration */
   Bool wrSmDfltTaTmrPres     = FALSE;       /* Infinity */
   U8 wrSmDfltTaTmrCmn          = WR_TIME_ALIGNMENT_TIMER_INFINITE;      
   U8 wrSmDfltTaTmrDed          = WR_TIME_ALIGNMENT_TIMER_INFINITE;      

/* k value: range [1-4] */
   U8 wrSmDfltUePrdDlCqiK      = 1;
/* CQI to aim for during PUSCH power control */
   U8 wrSmDfltUeUlPwrTrgCqi    = 10;
   U8 wrSmDfltAckNackRepFactor = 4;

   U8 wrSmDfltMacInst            = 0;
   U8 wrSmDfltMaxUePerUlSf       = 1; /* 1 to 4 */
   U8 wrSmDfltMaxUePerDlSf       = 1; /* 1 to 4 */
   U8 wrSmDfltMaxUlBwPerUe       = 50;
   U16 wrSmDfltMaxDlBwPerUe      = 50;
   U8 wrSmDfltMaxDlRetxBw        = 50;
   U8 wrSmDfltMaxDlUeNwTxPerTti  = 1;
   U8 wrSmDfltMaxUlUeNwTxPerTti  = 1;

   U16 wrSmDfltIsCycPrefULExt    = FALSE; /* TRUE-extended */
   U16 wrSmDfltIsCycPrefDLExt    = FALSE; /* TRUE-extended */
   U16 wrSmDfltCellModSchm       = FALSE;    /* TRUE indicates 64QAM */

/* Changing from 0 to 2 for RR */
   /* Scheduler types: RR -> 2, PFS->1 */
   U8 wrSmDfltDlSchdType         = 2;  /*  0..(number of schedulers - 1) */ 
   U8 wrUlDlCfgIdx               = 2;
   U8 wrSpclSfCfgIdx             = 8;     
   /* Scheduler types: RR -> 2, PFS->1 */
   U8 wrSmDfltUlSchdType         = 2;  /*  0..(number of schedulers - 1) */
   U8 wrSmDfltDLFSSchdTyp        = 0;  /* DLFS scheduler; 0..(no. of schedulers - 1) */
   U8 wrSmDfltMaxDlHqTx          = 4;   /* Maximum number of DL HARQ Transmissions */
   U8 wrSmDfltMaxMsg4HqTx        = 4; /* Maximum msg4 HARQ Transmissions */
   
   U8 wrSmDfltPfsDlTptCoefficient  = 10;  /*  takes values from 0 to 10 Default is 10 */ 
   U8 wrSmDfltPfsDlFairCoefficient = 10;  /*  takes values from 0 to 10 Default is 10 */ 
   U8 wrSmDfltPfsUlTptCoefficient  = 10;  /*  takes values from 0 to 10 Default is 10 */ 
   U8 wrSmDfltPfsUlFairCoefficient = 10;  /*  takes values from 0 to 10 Default is 10 */ 
   U32 wrSmDfltDlQciWgt[9] = {8,6,7,5,9,4,3,2,1};
   U32 wrSmDfltUlQciWgt[9] = {8,6,7,5,9,4,3,2,1};
   
   U16 wrSmDfltStartRnti        = 61; /* Start RNTI for the range managed by MAC */
   U16 wrSmDfltMacRntisize      = 20;   /* Indicates contiguous range of RNTI *//* CFI for PDCCH: a value in set {1,2,3} */
   U16 wrSmDfltRrmRntisize      = 20;   /* Indicates maximum rnti's that can be used by RRM */
   U16 wrSmDfltRrmStartPreamble = 41;   /* starting value of the dedicated preambles used by RRM */
   U16 wrSmDfltRrmNumPreambles  = 10;   /* maximum number of dedicated preambles that can be used by RRM */
   U8 wrSmDfltCfiCfg            = 1;

   U8 wrSmDfltTrgUlCqi            = 10;
   U16 wrSmDfltBcchPchRaCodeRate  = 0;
   U16 wrSmDfltPdcchCodeRate      = 0;
   U8 wrSmDfltDlCmnCodRateCcchCqi = 6;
/* Sub-band start */
   U8 wrSmDfltPuschSubBndStrt     = 1;
   U8 wrSmDfltPuschNoSubbnds      = 8;
/* Size of a sub-band */
   U8 wrSmDfltPuschSubBandSz      = 6;

   U8 wrSmDfltPuschSubBandDmrs    = 1;
#ifdef TFU_UPGRADE
   U8 wrSmDfltULCmnCodRateCcchCqi = 6;
#else
   U8 wrSmDfltULCmnCodRateCcchCqi = 11;
#endif
   U8 wrSmDfltThresholdCqi        = TRUE;
   U8 wrSmDfltDLTotalBw           = 50;
/*!< Total Uplink Bandwidth  */
   U8 wrSmDfltULTotalBw           = 50;

   U16 wrSmDfltNgEnum             = RGR_NG_ONESIXTH;
/*!< PHICH Duration: TRUE-extended/FALSE-normal */
   U8 wrSmDfltIsDurExt            = FALSE;
/*!< PUCCH resource-size or N^(2)_RB (in RBs) */
   U8 wrSmDfltPUCCHResSz          = 1;

   U16 wrSmDfltN1PucchAn           = 1;
/*!< Delta Shift for PUCCH: a value in set {1,2,3} */
   U8 wrSmDfltDeltaShift          = 1;
/*!< Cyclic Shift for PUCCH (N^(1)_CS): a value in range [0-7] */
   U8 wrSmDfltPUCCHCycShift       = 0;

   U16 wrSmDfltSrsCfgPrdEnum      = 0;
/*!< SRS Bandwidth configuration per cell Range - [0-7] */
   U16 wrSmDfltSrsBwEnum          = 0;
/*!< SRS subframe configuration index per cell Range - [0-15] */
   U8 wrSmDfltSrsSubFrameCfg      = 1;
   U8 wrSmDfltPreambleFrmt        = 0;
 /*!< RA Window size */
   U8 wrSmDfltRaWinSz             = 10;
   U8 wrSmDfltRaOccSz             = 1;
   U16 wrSmDfltRaOccSfnEnum       = RGR_SFN_ANY;

   U8 wrSmDfltMaxMsg3Tx           = 3;
   U8 wrSmDfltNumRaPreamble       = 45;
/*!< Size of RA Preamble in Group A */
   U8 wrSmDfltSzRaPreambleGrpA    = 5;
 /*!< MESSAGE_SIZE_GROUP_A */
   U16 wrSmDfltMsgSzGrpA          = 56;
   U8 wrSmDfltRetxCnt             = 1;
   U16 wrSmDfltUlPwrCfgAlpha      = RGR_PWR_ALPHA0;

   /* GRP_PWR: TODO: Following MACROS can be removed after a logic for determining 
      varying group power configuration across UEs is written */
   U8 wrSmDfltGrpPwrTpcIdx    = 1;   
   /*ccpu00139148 -- DCI Format 3/3A changes start*/
   U32 wrSmNumUesGrpPwrCtrlFmt3  = 14;
   U32 wrSmNumUesGrpPwrCtrlFmt3A = 28;

   U16  wrSmDfltPucchPwrFmt3RntiSt          = 158;
   U16  wrSmDfltPucchPwrFmt3RntiSz          = 32;
   U16  wrSmDfltPucchPwrFmt3aRntiSt         = 158;
   U16  wrSmDfltPucchPwrFmt3aRntiSz         = 32;
   U16  wrSmDfltPuschPwrFmt3RntiSt          = 223;
   U16  wrSmDfltPuschPwrFmt3RntiSz          = 32;
   U16  wrSmDfltPuschPwrFmt3aRntiSt         = 223;
   U16  wrSmDfltPuschPwrFmt3aRntiSz         = 32;
   U8   wrSmDfltPucchPwrFmt3AEnable         = FALSE;
   U8   wrSmDfltPuschPwrFmt3AEnable         = FALSE;
   U8   wrSmDfltisUlGrpPwrCntrlPucchEnable  = FALSE;
   U8   wrSmDfltisUlGrpPwrCntrlPuschEnable  = FALSE;
   /*ccpu00139148 -- DCI Format 3/3A changes end*/
    
   U32 wrSmDfltUeBr           = 80000000; /* 2048000; */
   U32 wrSmDfltQci            = 0; 
   U32 wrSmDfltDlQosGbr       = 80000000;/* 2048000; */
   U32 wrSmDfltDlQosMbr       = 80000000;/* 2048000; */
/*!< Number of sub-bands */
   U8 wrSmDfltPuschCfgNoSubBands  = 1;
/*!< Hopping mode inter/intra subframe */
   U8 wrSmDfltPuschCfgIsIntraHop  = FALSE;
/*!< Hopping offset {0 ... 98} */
   U8 wrSmDfltPuschCfgHopOffst    = 0;
/*!< Indicates if other configuration fields are valid */
   U8 wrSmDfltPreambleSetCfgPres  = FALSE;
/*!< Start Preamble ID for the range managed by MAC */
   U8 wrSmDfltPreambleSetCfgStart = 0;
/*!< Indicates contiguous range of premables managaed by MAC */
   U8 wrSmDfltPreambleSetCfgSz    = 0;

/* SIB5 defaults */
   U32 wrSmDfltS5DlCarrierFreq           = 15;
   S32 wrSmDfltS5QRxLevMin               = -65;
   U32 wrSmDfltS5InterFreqtReselection   = 0;
   U32 wrSmDfltS5ThreshXHigh             = 4;
   U32 wrSmDfltS5ThreshXLow              = 2;
   U32 wrSmDfltS5AllowedMeasBandwidth    = NhuAllowedMeasBandwidthmbw25Enum;
   U32 wrSmDfltS5PresenceAntennaPort1    = FALSE;
   U32 wrSmDfltS5CellReselectionPriority = 4;
   U8 wrSmDfltS5NeighCellCfg             = 1 << 6;
   U32 wrSmDfltS5QOffsetFreq             = 0;
/*RB Cfg Related Parameters*/
/*Changing maxRlcDlRetx for SRB and DRB value from 4 to 6 */   
   U8    wrSmDfltAmRlcSrbmaxRetxThresh  = NHU_MAX_RETX_THRES_T6;
   U8    wrSmDfltAmRlcDrbmaxRetxThresh  = NHU_MAX_RETX_THRES_T6;
   U16   wrSmDfltAmRlcSrbmaxpollByte    = NhuPollBytekBinfinityEnum;
#ifdef LTE_TDD
	 U16   wrSmDfltAmRlcDrbmaxpollByte    = NHU_POLL_BYTE_KB50;
#else
   U16   wrSmDfltAmRlcDrbmaxpollByte    = NHU_POLL_BYTE_KB125;
#endif
   U16   wrSmDfltAmRlcSrbpollPdu        = NHU_POLL_PDU_INF;
   U16   wrSmDfltAmRlcDrbpollPdu        = NHU_POLL_PDU_P4;
   U16   wrSmDfltAmRlcSrbtPollRetx      = NHU_TPOLL_RETX_MS400;
   U16   wrSmDfltAmRlcDrbtPollRetx      = NHU_TPOLL_RETX_MS80;
   U8    wrSmDfltUmRlcsnFieldLen        = 10;

   U8    wrSmDfltAmRlcSrbtStatusProh    = 2;
   U8    wrSmDfltAmRlcDrbtStatusProh    = 2;

   U8    wrSmDfltAmRlcSrbtReordring     = 2;
   U8    wrSmDfltAmRlcDrbtReordring     = 2;
   U8    wrSmDfltDlSrbtReordring     = NhuT_Reorderingms40Enum;
   U8    wrSmDfltDlDrbtReordring     = NhuT_Reorderingms40Enum;
   U8    wrSmDfltUlSrbtReordring     = NhuT_Reorderingms40Enum;
   U8    wrSmDfltUlDrbtReordring     = NhuT_Reorderingms40Enum;

   U8    wrSmDfltModPrd = 128;
   /* CSG_DEV - removing unused variable - remove when checking-in */
   //U32   wrSmDfltCsgId  = 0;

   /*fill for UTRA neighbour freq */
   U32 wrSmDfltUtraReCfgType    = 0x0000ffff;

   /*fill UTRA CELL info*/
   U8 wrSmDfltUtraNriIndex      = 1;
   U32 wrSmDfltRncID            = 0x4567;
   U16 wrSmDfltUtraCid          = 0x4567;
   U8 wrSmDfltNeRncNumMncDigits = 3;

   U8 wrSmDfltNeRncPlmn1Mcc0 = 0;
   U8 wrSmDfltNeRncPlmn1Mcc1 = 0;
   U8 wrSmDfltNeRncPlmn1Mcc2 = 1;
   U8 wrSmDfltNeRncPlmn1Mnc0 = 0;
   U8 wrSmDfltNeRncPlmn1Mnc1 = 2;
   U8 wrSmDfltNeRncPlmn1Mnc2 = 0;

   U16 wrSmDfltReslct                = 0;
   U16 wrSmDfltNeighCellPsc          = 0x56;
   U16 wrSmDfltNeighCellLac          = 0x32;
   U8 wrSmDfltNeighCellRac           = 0xff;
   Bool  wrSmDfltUtranNrIsRemAllowed = TRUE;
   Bool  wrSmDfltUtranNrIsHOAllowed  = TRUE;
   U8      wrSmDfltUtranNrCurRank    = 24;
   U8      wrSmDfltUtranNrPrevRank   = 34;
   U8    wrSmDfltUtraReason          = 0;
   U32   wrSmDfltUtraRptAmount       = 3;
   U32   wrSmDfltUtraRptInterval     = 1;
   U8    wrSmDfltUtraMaxRptCells     = 8;
   U8 wrSmDfltEcnoFltrCoeff          = 4;
   U16 wrSmDfltMeasQtyUtraFdd        = 0;
   U16 wrSmDfltMeasQtyUtraTdd        = 0;
   U8 wrSmDfltReslctSfMed            = 0;
   U8 wrSmDfltReslctSfHigh           = 0;
   U8    wrSmDfltUtraNumNrFreq       = 1;
   U8    wrSmDfltUtraNumNrCell       = 1;
   U8    wrSmDfltNumofEutraNrFreq    = 3;
   U32   wrSmPciSelType                  = FALSE;
   U32   wrSmPrachSelType                = FALSE;
   U32   wrSmEarfcnSelType               = FALSE;

   /*fill for GERAN neighbour freq */
   U32 wrSmDfltGeranReCfgType = 0x3f;
/* Con Mode */
   U32  wrSmDfltGeranplmnID              = 0;
   U16  wrSmDfltGeranLAC                 = 0;
   U16  wrSmDfltGeranRAC                 = 0;
   U8   wrSmDfltGeranBSIC                = 0;
   U16  wrSmDfltGeranCI                  = 0;
   /*U8   wrSmDfltGeranBandInd             = WR_GERAN_GSM850;*/
   /*U16  wrSmDfltGeranBcchArfcn           = 0;*/
   U8   wrSmDfltGeranNccPermittedMeas    = 0xFF;
   Bool  wrSmDfltGeranisDTMCapability    = FALSE;
/* Idle Mode */
   U8   wrSmDfltGerantReselection        = 0;
   U8   wrSmDfltGeranpMaxGeran           = 0;
   U8   wrSmDfltGeranBandInd             = WR_GERAN_GSM850;
   U16  wrSmDfltGeranBcchArfcn           = 0;
   U8   wrSmDfltGeranCellRelectionPriority = 2;
   U8   wrSmDfltGeranNccPermitted        = 0xFF;
   U8   wrSmDfltGeranqRxLevMin           = 0;
   U8   wrSmDfltGeranthreshXHigh         = 2;
   U8   wrSmDfltGeranthreshXLow          = 2;
   U32  wrSmDfltGerantReselSFMedium      = LWR_O_DOT_5;
   U32  wrSmDfltGerantReselSFHigh        = LWR_O_DOT_5;
   Bool wrSmDfltGeranpres_SpeedStateScaleFactors = PRSNT_NODEF;
   Bool wrSmDfltGeranpMaxGeranPress      = PRSNT_NODEF;

/* GERAN meas */
   U32  wrSmDfltGeranQoffset             = 0;
   U32  wrSmDfltGeranFltrCoeff           = 2;
   U32  wrSmDfltGeranMaxRptCells         = 8;
   U32  wrSmDfltGeranRptInterval         = 1;
   U32  wrSmDfltGeranRptAmt              = 0;
   Bool wrSmDfltisGERANMeasAllowedForCCO = FALSE;
   Bool wrSmDfltGeranisCCOAllowed        = FALSE;
   U32  wrSmDfltGERANMeasWaitTmr         = 300;
   U32  wrSmDfltGeranT304expWaitTmrVal   = 1100;
   U32  wrSmDfltGeranrrcDatCfmWaitTmr    = 300;
#ifdef E_TM
   U8 wrSmDfltBootMode = 0;
   U32 wrSmDfltEtmId   = 1;
#endif
   U32  wrSmNumofEaid                =  0;
   U32  wrSmEaidList[LWR_SM_MAX_EAIDS];

   /* ccpu00128279 : The Primary ETWS by default will be broadcasted every hour 
    * for 24 hours.*/
   U32 wrSmDfltPrimEtwsBrdcst        = 24;
   U16 wrSmDfltPrimEtwsRepetition    = 3600;
   U8   wrSmPwsEtwsCmas              =  0;
   /* SPS changes starts */
   Bool wrSmDfltIsSpsEnabled = FALSE;
   /* If wrSmDfltIsSpsEnabled==TRUE then by default the value is taken as 1 
    * for wrSmDfltMaxDlSpsUePerTti or wrSmDfltMaxUlSpsUePerTti to cover the case
    * when the parameter is not present or invalid value is present */
   U16 wrSmDfltMaxDlSpsUePerTti = 1;
   U16 wrSmDfltMaxUlSpsUePerTti = 1;
   U8  wrSmDfltSpsDlBandwidth = 25;
   /* SPS changes ends */
   S8 wrSmDfltCellSizeType = WR_ENB_CELL_SIZE_TYPE_VERY_SMALL;/*UE History info*/

   Bool wrSmDfltIsDcfiEnabled        = FALSE;
   
   /*RBC_SP1_X*/
   U32 wrSmDfltNoOfPrbRpts    = 5;  /*For RRM onlly. To calculate sliding window size*/
   U32 wrSmDfltPrbRptInterval = 20; /*In milliseconds. For MAC/SCH to report PRBs*/
   U8 wrSmDfltQci1TmngReq = 20; /* QCI-1 timing requirements in ms ; 
                                   default value is considered based on RTP */
   U8 wrSmDfltQci2TmngReq = 20; /* QCI-2 timing requirements in ms; i
                                   both voice and video are co-exited most of the times 
                                   hence the default value is same QCI-1 timing requirement */
   U8 wrSmDfltQci3TmngReq = 10; /* QCI-3 timing requirements in ms ;
                                   default value is set as half of the QCI-1.
                                   since the QCI-2 packet delay budget is half of the QCI-1*/
   U8 wrSmDfltQci4TmngReq = 32; /* QCI-4 timing requirements in ms */
   U8 wrSmDfltQci4TmngReqEnable = 0; /* To enable or disable the QCI-4 timing requirement */
   U8 wrSmDfltMinDlReTxUesPerTti = 1; /*!< Minumum number of DL re-transmission UEs per TTI */
   U8 wrSmDfltMinUlReTxUesPerTti = 1; /*!< Minumum number of UL re-transmission UEs per TTI */
   /*RBC_SP1_Y*/

   WrSmDfltEutraFaPriorInfoFunc    wrSmDfltEutraFaPriorInfo[] =
   {
      {
         /*cellNum*/0,
         /*faIndex*/0,
         /*status*/1,
         /*earfcnUl*/21100,
         /*earfcnDl*/3100,
         /*priority*/7,
         /*qRxLevMin*/-64,
         /*pMaxUsage*/1,
         /*pMax*/1,
         /*tReselection*/1,
         /*sfUsage*/0,
         /*tReselectionSfMedium*/3,
         /*tReselectionSfHigh*/3,
         /*sIntraSearchUsage*/1,
         /*sIntraSearch*/31,
         /*sNonIntraSearchUsage*/1,
         /*sNonIntraSearch*/31,
         /*threshServingLow*/31,
         /*threshXhigh*/31,
         /*threshXLow*/31,
         /*mesaBandwidthUsage*/1,
         /*measurementBandwidth*/3,
         /*presenceAntPort1*/0,
         /*neighCellConfig*/1,
         /*qOffsetFreq*/15,
         /*offsetFreq*/15,
         /*sIntraSearchRel9Usage*/1,
         /*sIntraSearchP*/31,
         /*sIntraSearchQ*/31,
         /*sNonIntraSearchRel9Usage*/1,
         /*sNonIntraSearchP*/31,
         /*sNonIntraSearchQ*/31,
         /*qQualMinRel9Usage*/1,
         /*qQualMinRel9*/-30,
         /*threshServingLowQRel9Usage*/1,
         /*threshServingLowQRel9*/31,
         /*threshXhighQRel9*/31,
         /*threshXLowQRel9*/31
      },
      {
         /*cellNum*/0,
         /*faIndex*/1,
         /*status*/1,
         /*earfcnUl*/23230,
         /*earfcnDl*/5230,
         /*priority*/7,
         /*qRxLevMin*/-64,
         /*pMaxUsage*/1,
         /*pMax*/1,
         /*tReselection*/1,
         /*sfUsage*/0,
         /*tReselectionSfMedium*/3,
         /*tReselectionSfHigh*/3,
         /*sIntraSearchUsage*/1,
         /*sIntraSearch*/31,
         /*sNonIntraSearchUsage*/1,
         /*sNonIntraSearch*/31,
         /*threshServingLow*/31,
         /*threshXhigh*/31,
         /*threshXLow*/31,
         /*mesaBandwidthUsage*/1,
         /*measurementBandwidth*/3,
         /*presenceAntPort1*/0,
         /*neighCellConfig*/1,
         /*qOffsetFreq*/15,
         /*offsetFreq*/15,
         /*sIntraSearchRel9Usage*/1,
         /*sIntraSearchP*/31,
         /*sIntraSearchQ*/31,
         /*sNonIntraSearchRel9Usage*/1,
         /*sNonIntraSearchP*/31,
         /*sNonIntraSearchQ*/31,
         /*qQualMinRel9Usage*/1,
         /*qQualMinRel9*/-30,
         /*threshServingLowQRel9Usage*/1,
         /*threshServingLowQRel9*/31,
         /*threshXhighQRel9*/31,
         /*threshXLowQRel9*/31
      },
      {
         /*cellNum*/0,
         /*faIndex*/2,
         /*status*/1,
         /*earfcnUl*/20850,
         /*earfcnDl*/2850,
         /*priority*/7,
         /*qRxLevMin*/-50,
         /*pMaxUsage*/1,
         /*pMax*/1,
         /*tReselection*/1,
         /*sfUsage*/0,
         /*tReselectionSfMedium*/3,
         /*tReselectionSfHigh*/3,
         /*sIntraSearchUsage*/1,
         /*sIntraSearch*/31,
         /*sNonIntraSearchUsage*/1,
         /*sNonIntraSearch*/31,
         /*threshServingLow*/31,
         /*threshXhigh*/31,
         /*threshXLow*/31,
         /*mesaBandwidthUsage*/1,
         /*measurementBandwidth*/3,
         /*presenceAntPort1*/0,
         /*neighCellConfig*/1,
         /*qOffsetFreq*/15,
         /*offsetFreq*/15,
         /*sIntraSearchRel9Usage*/1,
         /*sIntraSearchP*/31,
         /*sIntraSearchQ*/31,
         /*sNonIntraSearchRel9Usage*/1,
         /*sNonIntraSearchP*/31,
         /*sNonIntraSearchQ*/31,
         /*qQualMinRel9Usage*/1,
         /*qQualMinRel9*/-30,
         /*threshServingLowQRel9Usage*/1,
         /*threshServingLowQRel9*/31,
         /*threshXhighQRel9*/31,
         /*threshXLowQRel9*/31
      }
   };
   U8    wrSmDfltNumofUtraNrFreq = 1;
   WrSmDfltUtraFaPriorInfoFunc    wrSmDfltUtraFaPriorInfo[] =
   {
      {
         /*cellNum*/1,
         /*faIndex*/1,
         /*status*/1,
         /*duplexType*/0,/*FDD*/
         /*uarfcnUl*/9763,
         /*uarfcnDl*/10713,
         /*priority*/1,
         /*threshXhigh*/31,
         /*threshXLow*/31,
         /*qRxLevMin*/-50,
         /*pMaxUtra*/1,
         /*qQualMin*/-16,
         /*offsetFreq*/15,
         /*threshXhighQRel9*/31,
         /*threshXlowQRel9*/31
      },
      {
         /*cellNum*/1,
         /*faIndex*/2,
         /*status*/0,
         /*duplexType*/0,/*FDD*/
         /*uarfcnUl*/9940,
         /*uarfcnDl*/9668,
         /*priority*/1,
         /*threshXhigh*/31,
         /*threshXLow*/31,
         /*qRxLevMin*/-50,
         /*pMaxUtra*/1,
         /*qQualMin*/-16,
         /*offsetFreq*/15,
         /*threshXhighQRel9*/31,
         /*threshXlowQRel9*/31
      },
      {
         /*cellNum*/1,
         /*faIndex*/3,
         /*status*/0,
         /*duplexType*/0,/*FDD*/
         /*uarfcnUl*/9942,
         /*uarfcnDl*/9670,
         /*priority*/1,
         /*threshXhigh*/31,
         /*threshXLow*/31,
         /*qRxLevMin*/-50,
         /*pMaxUtra*/1,
         /*qQualMin*/-16,
         /*offsetFreq*/15,
         /*threshXhighQRel9*/31,
         /*threshXlowQRel9*/31
      },
      {
         /*cellNum*/1,
         /*faIndex*/4,
         /*status*/0,
         /*duplexType*/0,/*FDD*/
         /*uarfcnUl*/9500,
         /*uarfcnDl*/9200,
         /*priority*/1,
         /*threshXhigh*/31,
         /*threshXLow*/31,
         /*qRxLevMin*/-50,
         /*pMaxUtra*/1,
         /*qQualMin*/-16,
         /*offsetFreq*/15,
         /*threshXhighQRel9*/31,
         /*threshXlowQRel9*/31
      }
   };
  
   WrSmDfltUtraTddFaPriorInfoFunc    wrSmDfltUtraTddFaPriorInfo[] =
   {
      {
         /*cellNum*/1,
         /*faIndex*/1,
         /*status*/1,
         /*duplexType*/1,/*TDD*/
         /*uarfcn*/10713,
         /*priority*/1,
         /*threshXhigh*/.1,
         /*threshXLow*/31,
         /*qRxLevMin*/-50,
         /*pMaxUtra*/1,
         /*offsetFreq*/15,
         /*threshXhighQRel9*/31,
         /*threshXlowQRel9*/31
      },
      {
         /*cellNum*/1,
         /*faIndex*/2,
         /*status*/0,
         /*duplexType*/1,/*TDD*/
         /*uarfcn*/9668,
         /*priority*/1,
         /*threshXhigh*/31,
         /*threshXLow*/31,
         /*qRxLevMin*/-50,
         /*pMaxUtra*/1,
         /*offsetFreq*/15,
         /*threshXhighQRel9*/31,
         /*threshXlowQRel9*/31
      },
      {
         /*cellNum*/1,
         /*faIndex*/3,
         /*status*/0,
         /*duplexType*/1,/*TDD*/
         /*uarfcn*/9670,
         /*priority*/1,
         /*threshXhigh*/31,
         /*threshXLow*/31,

         /*qRxLevMin*/-50,
         /*pMaxUtra*/1,
         /*offsetFreq*/15,
         /*threshXhighQRel9*/31,
         /*threshXlowQRel9*/31
      },
      {
         /*cellNum*/1,
         /*faIndex*/4,
         /*status*/0,
         /*duplexType*/1,/*TDD*/
         /*uarfcn*/9200,
         /*priority*/1,
         /*threshXhigh*/31,
         /*threshXLow*/31,
         /*qRxLevMin*/-50,
         /*pMaxUtra*/1,
         /*offsetFreq*/15,
         /*threshXhighQRel9*/31,
         /*threshXlowQRel9*/31
      }
   };

   U8    wrSmDfltNumofUtraFddNrCell = 1;
   U8    wrSmDfltNumofUtraTddNrCell = 1;
/* Amit_Changes Start*/
   WrSmDfltExternalUtranCellLogic    wrSmDfltExtUtraCell[MAX_NBR_UTRA_CELL] =
/* Amit_Changes End*/
   {
      {
         /*cellNum*/1,
         /*relationIdx*/1,
         /*status*/1,
         /*rncId*/12,
         /*cId*/5,
         /*lac*/50,
         /*rac*/50,
         /*reserved2*/0,
         /*numPlmns*/2,
         /*plmnIds[6] */{{3,{0,0,1}, {0,2,0}},{2,{3,1,1},{4,8,0}},},
         /*duplexType*/0,/*0:FDD, 1:TDD*/
         /*pScmCode*/86,
         /*cellParaId*/0,
         /*uarfcnUl*/9763,
         /*uarfcnDl*/10713,
         /*isRemoveAllowed*/1,
         /*isHOAllowed*/1,
         /*rimStatus*/0
      },
      {
         /*cellNum*/1,
         /*relationIdx*/2,
         /*status*/0,
         /*rncId*/12,
         /*cId*/5,
         /*lac*/52,
         /*rac*/50,
         /*reserved2*/0,
         /*numPlmns*/2,
         /*plmnIds[6] */{{3,{0,0,1}, {0,2,0}},{2,{3,1,1},{4,8,0}},},
         /*duplexType*/0,/*0:FDD, 1:TDD*/
         /*pScmCode*/82,
         /*cellParaId*/0,
         /*uarfcnUl*/9938,
         /*uarfcnDl*/9666,
         /*isRemoveAllowed*/1,
         /*isHOAllowed*/1,
         /*rimStatus*/0
      },
      {
         /*cellNum*/1,
         /*relationIdx*/3,
         /*status*/0,
         /*rncId*/12,
         /*cId*/5,
         /*lac*/50,
         /*rac*/50,
         /*reserved2*/0,
         /*numPlmns*/2,
         /*plmnIds[6] */{{3,{0,0,1}, {0,2,0}},{2,{3,1,1},{4,8,0}},},
         /*duplexType*/0,/*0:FDD, 1:TDD*/
         /*pScmCode*/80,
         /*cellParaId*/0,
         /*uarfcnUl*/9942,
         /*uarfcnDl*/9670,
         /*isRemoveAllowed*/1,
         /*isHOAllowed*/1,
         /*rimStatus*/0
      }
   };

   WrSmDfltExternalUtranCellLogic    wrSmDfltExtUtraTddCell[MAX_NBR_UTRA_CELL] =
   {
      {
         /*cellNum*/1,
         /*relationIdx*/1,
         /*status*/1,
         /*rncId*/12,
         /*cId*/5,
         /*lac*/50,
         /*rac*/50,
         /*reserved2*/0,
         /*numPlmns*/2,
         /*plmnIds[6] */{{3,{0,0,1}, {0,2,0}},{2,{3,1,1},{4,8,0}},},
         /*duplexType*/0,/*0:FDD, 1:TDD*/
         /*pScmCode*/86,
         /*cellParaId*/0,
         /*uarfcnUl*/9763,
         /*uarfcnDl*/10713,
         /*isRemoveAllowed*/1,
         /*isHOAllowed*/1,
         /*rimStatus*/0
      },
      {
         /*cellNum*/1,
         /*relationIdx*/2,
         /*status*/0,
         /*rncId*/12,
         /*cId*/5,
         /*lac*/52,
         /*rac*/50,
         /*reserved2*/0,
         /*numPlmns*/2,
         /*plmnIds[6] */{{3,{0,0,1}, {0,2,0}},{2,{3,1,1},{4,8,0}},},
         /*duplexType*/0,/*0:FDD, 1:TDD*/
         /*pScmCode*/82,
         /*cellParaId*/0,
         /*uarfcnUl*/9938,
         /*uarfcnDl*/9666,
         /*isRemoveAllowed*/1,
         /*isHOAllowed*/1,
         /*rimStatus*/0
      },
      {
         /*cellNum*/1,
         /*relationIdx*/3,
         /*status*/0,
         /*rncId*/12,
         /*cId*/5,
         /*lac*/50,
         /*rac*/50,
         /*reserved2*/0,
         /*numPlmns*/2,
         /*plmnIds[6] */{{3,{0,0,1}, {0,2,0}},{2,{3,1,1},{4,8,0}},},
         /*duplexType*/0,/*0:FDD, 1:TDD*/
         /*pScmCode*/80,
         /*cellParaId*/0,
         /*uarfcnUl*/9942,
         /*uarfcnDl*/9670,
         /*isRemoveAllowed*/1,
         /*isHOAllowed*/1,
         /*rimStatus*/0
      }
   };
#ifdef RSYS_WIRESHARK
   U16  wrSmDfltWiresharkPort; /*to send logs to wireshark */
   U32  wrSmDfltWiresharkDstIpAddr;  /*to send logs to wireshark */
#endif
#ifdef LTE_QCOM
   /* Default RF Configuration which are unsupported from Radisys OAM */
   /* Record - rfCarrierCommon */
   U16   rfCarrierCommonCarrierId       = 1;                       /* 1 to 8 */
   Bool  rfCarrierCommonAdminState      = FALSE; 
   Bool  rfCarrierCommonStandalone      = TRUE;                    /* true or false */
#ifdef LTE_TDD
   U16   rfCarrierCommonRxPathIdLst_0   = 3;
   U16   rfCarrierCommonRxPathIdLst_1   = 4;
#else
   U16   rfCarrierCommonRxPathIdLst_0   = 1;
   U16   rfCarrierCommonRxPathIdLst_1   = 2;
#endif
   U16   rfCarrierCommonRxPathIdLst_2   = 0;
   U16   rfCarrierCommonRxPathIdLst_3   = 0;
#ifdef LTE_TDD
   U16   rfCarrierCommonTxPathIdLst_0   = 3;
   U16   rfCarrierCommonTxPathIdLst_1   = 4;
#else
   U16   rfCarrierCommonTxPathIdLst_0   = 1;
   U16   rfCarrierCommonTxPathIdLst_1   = 2;
#endif 
   U16   rfCarrierCommonTxPathIdLst_2   = 0;
   U16   rfCarrierCommonTxPathIdLst_3   = 0;

   /* Record - rfPathProv */
#ifdef LTE_TDD
   U16   rfPathProvPathId1              = 3;                      /* 1 to 6 */
#else
   U16   rfPathProvPathId1              = 1;                      /* 1 to 6 */
#endif
   U16   rfPathProvPath1TxDirection     = RF_PATH_DIRECTION_TX;   /*  RF_PATH_DIRECTION_NONE = 0, RF_PATH_DIRECTION_TX = 1, RF_PATH_DIRECTION_RX = 2,  RF_PATH_DIRECTION_LAST =3 */
   U16   rfPathProvPath1RxDirection     = RF_PATH_DIRECTION_RX;   /*  RF_PATH_DIRECTION_NONE = 0, RF_PATH_DIRECTION_TX = 1, RF_PATH_DIRECTION_RX = 2,  RF_PATH_DIRECTION_LAST =3 */

#ifdef LTE_TDD
   U16   rfPathProvPathId2              = 4;                      /* 1 to 6 */
#else
   U16   rfPathProvPathId2              = 2;                      /* 1 to 6 */
#endif
   U16   rfPathProvPath2TxDirection     = RF_PATH_DIRECTION_TX;   /*  RF_PATH_DIRECTION_NONE = 0, RF_PATH_DIRECTION_TX = 1, RF_PATH_DIRECTION_RX = 2,  RF_PATH_DIRECTION_LAST =3 */
   U16   rfPathProvPath2RxDirection     = RF_PATH_DIRECTION_RX;   /*  RF_PATH_DIRECTION_NONE = 0, RF_PATH_DIRECTION_TX = 1, RF_PATH_DIRECTION_RX = 2,  RF_PATH_DIRECTION_LAST =3 */

   /* Record - rfTxCarrierProv */
   U16   rfTxCarrierProvCarrierId       = 1;                       /* 1 to 8 */

   /* Record - rfRxCarrierProv */
   U16   rfRxCarrierProvCarrierId       = 1;                       /* 1 to 8 */

   /* Record - rfManagerConfig */
   U32   rfManagerConfigLogLevel        = LOG_LEVEL_INFO;          /* The RF Manager Log Level */
   U64   rfManagerConfigLogArea         = 1;                       /* The RF Manager Log area to control rf logging */
   Bool  rfManagerConfigRequireCal      = FALSE;                   /* TRUE/FALSE */
#endif   

#ifdef __cplusplus
}
#endif /* __cplusplus */

/********************************************************************30**

           End of file:     fc_smm_smallcell_brcm.c@@/main/TeNB_Main_BR/5 - Mon Aug 11 16:52:32 2014

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
SID$        ---      Sriky         1. initial release TotaleNodeB 1.1
*********************************************************************91*/
