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

     Type:     C include file

     Desc:     This file contains 
               

     File:     wr_smm_smallcell.h

     Sid:      $SID$ 

     Prg:      Sriky 

*********************************************************************21*/
    
#ifndef WR_SMM_SMALLCELL_H
#define WR_SMM_SMALLCELL_H

#ifdef __cplusplus
extern "C" {
#endif
#define WR_DFLT_CELL_IDX 0
#define WR_DFLT_NUM_CMN_LCS 6


EXTERN U8 wrSmDfltRlLogLvl; /* Log Level Default set to L_EVENT */

EXTERN U8 wrSmDfltDuplexMode;
EXTERN U16 wrSmDfltServingCellDLCarrierFreq;


EXTERN CtfBandwidth wrSmDfltCellDlBW;                   
EXTERN CtfBandwidth wrSmDfltCellUlBW; 
/* ACC-TDD */
EXTERN U8 wrSmTddSubframeConfig;
EXTERN U8 wrSmTddSpecialSubframeConfig;
/* 
 * Master Info Block 
 */
EXTERN CtfPhichDuration wrSmDfltPhichDur;
EXTERN CtfPhichResource wrSmDfltPhichRes;

/*
 * SystemInfo Blocks
 */
EXTERN U16   wrSmDfltPagDrx; /* moved frm enb_params */
EXTERN U16   wrSmDfltCellRsvdForOperuser; /* moved frm enb_params */
EXTERN U16   wrSmDfltCellBarred;
EXTERN U16   wrSmDfltIntraFreqRese;
EXTERN U16   wrSmDfltCellRsrvExtn;
EXTERN S16   wrSmDfltqRxlevmin;
EXTERN U16   wrSmDfltqRxlevminoffset;
EXTERN U16   wrSmDfltSubframeAssignment;
EXTERN U16   wrSmDfltSpecialSubfrmPatterns;
EXTERN    U16 wrSmDfltSiPeriodicity[];
EXTERN    U16 wrSmDfltSiMapInfoCnt[];

EXTERN    U16 wrSmDfltSiMapInfoVal[];
EXTERN    U8 wrSmDfltNumSi;
EXTERN    U8 wrSmDfltModPrd;
EXTERN    U16 wrSmDfltSiWindowLength;
EXTERN    U16 wrSmDfltSystemInfoValueTag;

EXTERN U8    wrSmDfltNumPlmn;
EXTERN U8    wrSmDfltNumMncDigits;
EXTERN U8    wrSmDfltPlmn1Mcc0;
EXTERN U8    wrSmDfltPlmn1Mcc1;
EXTERN U8    wrSmDfltPlmn1Mcc2;
EXTERN U8    wrSmDfltPlmn1Mnc0;
EXTERN U8    wrSmDfltPlmn1Mnc1;
EXTERN U8    wrSmDfltPlmn1Mnc2;


EXTERN U32   wrSmDfltCsgIdVal;
/* CSG_DEV start */
EXTERN U8    wrSmDfltAccessMode;
EXTERN S8    wrSmDfltHenbName[48];
EXTERN U8    wrSmDfltPreferredHoTarget;
EXTERN U8    wrSmDfltIsSmlCellPciCfgd;
EXTERN U16   wrSmDfltSmallCellStartPci;
EXTERN U16   wrSmDfltSmallCellPciRange;
EXTERN S16   wrSmDfltMaxCsgMembers;
EXTERN Bool  wrSmDfltIsCsgMbrshpDetermndLcly;
EXTERN U8    wrSmDfltNonCsgUeAccDcsn;
EXTERN U8    wrSmDfltIsCsgPciCfgd;
/* SIB4 defaults */
EXTERN    U32 wrSmDfltCSGStartPci;
EXTERN    U32 wrSmDfltCSGPciRange;
EXTERN    U16 wrSmDfltMaxCsgUEs;
EXTERN    U16 wrSmDfltMaxNonCsgUEs;
EXTERN    U8 wrSmDfltMinDlResNonCsg;
EXTERN    U8 wrSmDfltMinUlResNonCsg;
/* CSG_DEV end */

EXTERN U8    wrSmDfltPlmn2Mcc0;
EXTERN U8    wrSmDfltPlmn2Mcc1;
EXTERN U8    wrSmDfltPlmn2Mcc2;
EXTERN U8    wrSmDfltPlmn2Mnc0;
EXTERN U8    wrSmDfltPlmn2Mnc1;
EXTERN U8    wrSmDfltPlmn2Mnc2;
EXTERN U16   wrSmDfltTac;
/* CSG_DEV - removing unused variable - remove when checking-in */
/*EXTERN U32   wrSmDfltCsgId; */
EXTERN U8    wrSmDfltnumSi;
/* EXTERN U32   wrSmDfltCellIdentity; */

EXTERN U16 wrSmDfltCellRsrvdForOperUse;  /* Not reserwrd */
EXTERN U16 wrSmDfltCellBarred;            
EXTERN U16 wrSmDfltIntraFreqResel;
EXTERN U16 wrSmDfltCSFInd;

EXTERN S16 wrSmDlftqRxLevMin;
EXTERN U16 wrSmDfltqRxLevMinOffset; 

/*rsrq_ho*/
EXTERN U8 wrSmDfltThresRSRQ; 
EXTERN U8 wrSmDfltSiWinSize;
EXTERN U16 wrSmDfltAddSpectEmn; 
EXTERN U16 wrSmDfltSib10RptPrd;
EXTERN U16 wrSmDfltSib10NumBroadCast;
EXTERN U8 wrSmDfltWarnSegSize;

EXTERN U8 wrSmDfltACBarrEmergency;
EXTERN U8 wrSmDfltACBarrNormal;
EXTERN U8 wrSmDfltMOACBarrInfoPres;
EXTERN U8 wrSmDfltDOACBarrInfoPres;
EXTERN U8 wrSmDfltACBarrInfoPres;
EXTERN U8 wrSmDfltVDACBarrInfoPres;
EXTERN U8 wrSmDfltVOACBarrInfoPres;

EXTERN  Bool  wrSmDfltMoSigAcBarringEnable;
EXTERN  Bool  wrSmDfltMoDataAcBarringEnable;
EXTERN U8  wrSmDfltacBarringFactor; /*!< Barring factor {0, 5, 10, 15, ... 95} */
EXTERN U16 wrSmDfltacBarringTime;   /*!< Barring Time in seconds {4, 8, ... 512} */
EXTERN U8  wrSmDfltacBarringAC11;   /*!< Ac Barring for Access class 11 */
EXTERN U8  wrSmDfltacBarringAC12;   /*!< Ac Barring for Access class 12 */
EXTERN U8  wrSmDfltacBarringAC13;   /*!< Ac Barring for Access class 13 */
EXTERN U8  wrSmDfltacBarringAC14;   /*!< Ac Barring for Access class 14 */
EXTERN U8  wrSmDfltacBarringAC15; 

/* BCCH Cfg */
EXTERN U16 wrSmDfltModPeriodCoeff;            /* n2  */

/* PCCH Cfg */
EXTERN U16 wrSmDfltDefaultPagCycle;           /* rf128 */
EXTERN U16 wrSmDfltPCCHCfgNB;               /* oneT */
EXTERN    CtfSCSpacing wrSmDfltScSpacing;                    
EXTERN    CtfCPCfg wrSmDfltCycPfx;                    

/* PDSCH Cfg */
EXTERN S8 wrSmDfltRefSignalPower;  
EXTERN U16 wrSmDfltPDSCHCfgPB;    

/* Starts - Fix for CR ccpu00123185 */

EXTERN S16 wrSmDfltMspdDbUnit;
   /* Right now PHY INIT is not accepting any reference signal power
    * we can use these parameters once PHY INIT is accepting 
    * initial referene signal power
    */
EXTERN S8  wrSmDfltRefSigPwr;  
EXTERN S16 wrSmDfltPilotSigPwr;
EXTERN S16 wrSmDfltPriSigPwr;
EXTERN S16 wrSmDfltSecSigPwr;

EXTERN S8 wrSmDfltLowRefSigPwr;  
EXTERN S16 wrSmDfltLowPilotSigPwr;
EXTERN S16 wrSmDfltLowPriSigPwr;
EXTERN S16 wrSmDfltLowSecSigPwr;

/* Ends - Fix for CR ccpu00123185 */

/* PRACH Config */
EXTERN U16 wrSmDfltRootSequenceIdx;           /* 0 */
EXTERN U16 wrSmDfltPrachCfgIdx;                /* 3*/
EXTERN U16 wrSmDfltHighSpeedFlag;            /* FALSE */
EXTERN U16 wrSmDfltZeroCorrelZoneCfg;         /* 0x05 */
EXTERN U16 wrSmDfltPrachFreqOffset;          /* (assuming 10MHz Channel bandwidth) changed from 4 in mspd  */

EXTERN    U16 wrSmDfltPrachMaskIdx;


/* PUCCH Cfg */
EXTERN CtfPucchDeltaShift wrSmDfltPUCCHDeltaShift;       /* DS1 */
EXTERN U16 wrSmDfltPUCCHnRBCQI;          /* Should have been 1 as per doc */
EXTERN U16 wrSmDfltPUCCHnCSAN;           /* zero */
EXTERN U16 wrSmDfltNlPUCCHAn;             /* Should have been 1 as per doc*/
EXTERN    U8 wrSmDfltPUCCHFixBw;             
EXTERN    U8 wrSmDfltPUCCHCyclShift;        
EXTERN    U8 wrSmDfltMaxPucchRbs;
/* PUSCH cfg */
EXTERN U16 wrSmDfltNSB;                    /* 2 */
EXTERN CtfPuschHoppingMode wrSmDfltHoppingMode; /* interSubFrame */
EXTERN U16 wrSmDfltPuschHoppingOff;       /* Assuming 10Mhz channel bandwidth*/
EXTERN U16 wrSmDfltEnable64QAM;           /* false */
EXTERN U16 wrSmDfltGroupHoppingEnabled;   /* false */
EXTERN U16 wrSmDfltSeqHoppingEnabled;     /* since group hopping disabled */
EXTERN U16 wrSmDfltGroupAssignPUSCH;       /* since group hopping disabled */
EXTERN U16 wrSmDfltCyclicShift;            /* since group hopping disabled */



/* RACH Common Cfg */
EXTERN U16 wrSmDfltNumberOfRaPreambles;      /* n8 */
EXTERN U16 wrSmDflSizeOfRaPreamblesGroupA;
EXTERN U16 wrSmDfltMessageSizeGroupA;
EXTERN U16 wrSmDfltMessagePowerOffsetGroupB;    
EXTERN U16 wrSmDfltPowerRampingStep;          
EXTERN U16 wrSmDfltPreambleInitRecvdTgtPwr;
EXTERN U16 wrSmDfltPreambleTransMax;
EXTERN U16 wrSmDfltRaRspWindowSize;          /* sf8 */
EXTERN U16 wrSmDfltMacContentResTmr;         /* sf40 */
EXTERN U16 wrSmDfltMaxHARQMsg3Tx; 
EXTERN    U8  wrSmDfltPreambleFormat;
EXTERN    U16 wrSmDfltRaOccasionSize;
EXTERN    U16 wrSmDfltRaOccasionSfnEnum;
EXTERN    U16 wrSmDfltRaOccasionSubFrmNum0;
EXTERN    U16 wrSmDfltRACHCfgMsgSizeGrpA;
EXTERN    U16 wrSmDfltPrachResource;



/* SoundingsRS UL Cfg */
EXTERN U16 wrSmDfltSrsBandwidthCfg;    
EXTERN U16 wrSmDfltSrsSubfrmCfg;        
EXTERN U16 wrSmDfltAckNackSrsSimTrns;
EXTERN U16 wrSmDfltSrsMaxUpPts;      
EXTERN Bool wrSmEnableSrs;
EXTERN U16 wrSmDfltSrsUeBandwidthCfg;
EXTERN U16 wrSmDfltSrsUEHoppingBandwidth;

EXTERN U16 wrSmDfltSrsCfgPrd;             /*  SRS period configuration */
/* Uplink Power Control Config */
EXTERN U16 wrSmDfltULPwrCtrlAlpha;
EXTERN S16 wrSmDfltP0NominalPUCCH;
EXTERN S16 wrSmDfltP0NominalPUSCH;
EXTERN U16 wrSmDfltDeltaFPUCCHFormat1;
EXTERN U16 wrSmDfltDeltaFPUCCHFormat1b;
EXTERN U16 wrSmDfltDeltaFPUCCHFormat2;
EXTERN U16 wrSmDfltDeltaFPUCCHFormat2a;
EXTERN U16 wrSmDfltDeltaFPUCCHFormat2b;
EXTERN S16 wrSmDfltUlDeltaPreambleMsg3;
EXTERN U16 wrSmDfltUlCyclicPrefixLength;

/* UE Timers and Constants */
EXTERN    U16 wrSmDfltTmrsAndConstsT300;
EXTERN    U16 wrSmDfltTmrsAndConstsT301;
EXTERN    U16 wrSmDfltTmrsAndConstsT304;
EXTERN    U16 wrSmDfltTmrsAndConstsT310;
EXTERN    U16 wrSmDfltTmrsAndConstsN310;
EXTERN    U16 wrSmDfltTmrsAndConstsT311;
EXTERN    U16 wrSmDfltTmrsAndConstsN311;

/* frequencyInformation */
EXTERN    U16 wrSmDfltUlCarrierFreq;                
EXTERN    U16 wrSmDfltUlBandwidth;
EXTERN    U16 wrSmDfltDlBandwidth;
EXTERN    U16 wrSmDfltAddSpectEmn;                   
EXTERN    U16 wrSmDfltTimeAlignTmrCmn;

/* RGR Cell Cfg */
EXTERN    U8 wrSmDfltMacInst;
EXTERN    U8 wrSmDfltMaxUePerUlSf;
EXTERN    U8 wrSmDfltMaxUePerDlSf;
EXTERN    U8 wrSmDfltMaxUlBwPerUe;
EXTERN    U16 wrSmDfltMaxDlBwPerUe;
EXTERN    U8 wrSmDfltMaxDlRetxBw;
EXTERN    U8 wrSmDfltMaxDlUeNwTxPerTti;
EXTERN    U8 wrSmDfltMaxUlUeNwTxPerTti;
EXTERN    U16 wrSmDfltNumOfTxAnt;
EXTERN    U16 wrSmDfltIsCycPrefULExt;
EXTERN    U16 wrSmDfltIsCycPrefDLExt;
EXTERN    U16 wrSmDfltCellModSchm;

EXTERN    U8 wrSmDfltDlSchdType;
EXTERN    U8 veUlDlCfgIdx;
EXTERN    U8 veSpclSfCfgIdx;
EXTERN    U8 wrSmDfltUlSchdType;
EXTERN    U8 wrSmDfltDLFSSchdTyp;
EXTERN    Bool wrSmDfltTmAutoConfig;
EXTERN    U8 wrSmDfltPreferredTm;
EXTERN    U8 wrSmDfltPfsDlTptCoefficient;
EXTERN    U8 wrSmDfltPfsDlFairCoefficient;
EXTERN    U8 wrSmDfltPfsUlTptCoefficient;
EXTERN    U8 wrSmDfltPfsUlFairCoefficient;
EXTERN    U32 wrSmDfltDlQciWgt[9];
EXTERN    U32 wrSmDfltUlQciWgt[9];
EXTERN    U8 wrSmDfltTransModeUse;
EXTERN    U8 wrSmDfltMaxDlHqTx;
EXTERN    U8 wrSmDfltMaxMsg4HqTx;
EXTERN    U16 wrSmDfltStartRnti;
EXTERN    U16 wrSmDfltMacRntisize;
EXTERN    U16 wrSmDfltRrmRntisize;
EXTERN    U16 wrSmDfltRrmStartPreamble;
EXTERN    U16 wrSmDfltRrmNumPreambles;
EXTERN    U8 wrSmDfltCfiCfg;
EXTERN    Bool wrSmDfltIsDcfiEnabled;

EXTERN    U8 wrSmDfltTrgUlCqi;
EXTERN    U16 wrSmDfltBcchPchRaCodeRate;
EXTERN    U16 wrSmDfltPdcchCodeRate;
EXTERN    U8 wrSmDfltDlCmnCodRateCcchCqi;
EXTERN    U8 wrSmDfltPuschSubBndStrt;
EXTERN    U8 wrSmDfltPuschNoSubbnds;
EXTERN    U8 wrSmDfltPuschSubBandSz;
EXTERN    U8 wrSmDfltPuschSubBandDmrs;
EXTERN    U8 wrSmDfltULCmnCodRateCcchCqi;
EXTERN    U8 wrSmDfltThresholdCqi;
EXTERN    U8 wrSmDfltDLTotalBw;
EXTERN    U8 wrSmDfltULTotalBw;
EXTERN    U16 wrSmDfltNgEnum;
EXTERN    U8 wrSmDfltIsDurExt;
EXTERN    U8 wrSmDfltPUCCHResSz;
EXTERN    U16 wrSmDfltN1PucchAn;
EXTERN    U8 wrSmDfltDeltaShift;
EXTERN    U8 wrSmDfltPUCCHCycShift;
EXTERN    U16 wrSmDfltSrsCfgPrdEnum;
EXTERN    U16 wrSmDfltSrsBwEnum;
EXTERN    U8 wrSmDfltSrsSubFrameCfg;
EXTERN    U8 wrSmDfltPreambleFrmt;
EXTERN    U8 wrSmDfltRaWinSz;
EXTERN    U8 wrSmDfltRaOccSz;
EXTERN    U16 wrSmDfltRaOccSfnEnum;
EXTERN    U8 wrSmDfltMaxMsg3Tx;
EXTERN    U8 wrSmDfltNumRaPreamble;
EXTERN    U8 wrSmDfltSzRaPreambleGrpA;
EXTERN    U16 wrSmDfltMsgSzGrpA;
/*EXTERN    U8 wrSmDfltPrachResource; */
EXTERN    U8 wrSmDfltSiWinSize;
EXTERN    U8 wrSmDfltRetxCnt;
EXTERN    S8 wrSmDfltP0NominalPusch;
EXTERN    U16 wrSmDfltUlPwrCfgAlpha;
EXTERN    S8 wrSmDfltP0NominalPucch; 
EXTERN    S8 wrSmDfltDeltaPreambleMsg3;
EXTERN    U16 wrSmDfltPucchPwrFmt3RntiSt;
EXTERN    U16 wrSmDfltPucchPwrFmt3RntiSz;
EXTERN    U16 wrSmDfltPucchPwrFmt3aRntiSt;
EXTERN    U16 wrSmDfltPucchPwrFmt3aRntiSz;
EXTERN    U16 wrSmDfltPuschPwrFmt3RntiSt;
EXTERN    U16 wrSmDfltPuschPwrFmt3RntiSz;
EXTERN    U16 wrSmDfltPuschPwrFmt3aRntiSt;
EXTERN    U16 wrSmDfltPuschPwrFmt3aRntiSz;
EXTERN    U8 wrSmDfltPucchPwrFmt3AEnable;
EXTERN    U8 wrSmDfltPuschPwrFmt3AEnable;
EXTERN    U8 wrSmDfltisUlGrpPwrCntrlPucchEnable;
EXTERN    U8 wrSmDfltisUlGrpPwrCntrlPuschEnable;
EXTERN    U8 wrSmDfltPuschCfgNoSubBands;
EXTERN    U8 wrSmDfltPuschCfgIsIntraHop;
EXTERN    U8 wrSmDfltPuschCfgHopOffst;
EXTERN    U8 wrSmDfltPreambleSetCfgPres;
EXTERN    U8 wrSmDfltPreambleSetCfgStart;
EXTERN    U8 wrSmDfltPreambleSetCfgSz;
EXTERN    U8 wrSmDfltPRACHNumRes;
EXTERN    U8 wrSmDfltNumCmnLcs;
EXTERN    U8 wrSmDfltGrpPwrTpcIdx;   
    

/* Security Cfg */
EXTERN    U16 wrSmDfltCiphAlgo;                   
EXTERN    U16 wrSmDfltIntgProtAlgo;               


/* RRC Dedicated Config */
/* SRB */
EXTERN    U16 wrSmDfltSrbPollRetrans;
EXTERN    U16 wrSmDfltSrbPollPdu;
EXTERN    U16 wrSmDfltSrbPollByte;
EXTERN    U16 wrSmDfltSrbMaxReTxThresh;
EXTERN    U16 wrSmDfltSrbTStatProhibit;

EXTERN    U16 wrSmDfltMacCfgMaxHARQTx;
EXTERN    U16 wrSmDfltMacCfgPeriodicBSRTimer;
EXTERN    U16 wrSmDfltMacCfgRetxBSRTimer;
EXTERN    U8 wrSmDfltMacCfgTtiBundling;               
/*DRX QCI*/
EXTERN    Bool wrSmDfltisDrxQCIEnabled;
EXTERN    Bool wrSmDfltIsDrxEnabled;         
EXTERN    Bool wrSmDfltIsCnmEnabled;
EXTERN    U8   wrSmDfltTddBoardType;
EXTERN    U16 wrSmDfltDrxInactvTmr;               
EXTERN    U16 wrSmDfltDrxReTxTmr;               
EXTERN    U16 wrSmDfltDrxLongCycleGbr;
EXTERN    U16 wrSmDfltDrxLongCycleNonGbr;
EXTERN    U16 wrSmDfltAnrDrxLongCycle;
EXTERN    U16 wrSmDfltAnrDrxOnDurationTmr;
/* Fix for ccpu00132334 - To read DRX Inactivity timer for ANR*/
EXTERN    U16 wrSmDfltAnrDrxInActivTmr; 

EXTERN    U16 wrSmDfltMacCfgTimeAligTmrDed;

EXTERN    U16 wrSmDfltMacCfgPeriodicPHR_Timer;               
EXTERN    U16 wrSmDfltMacCfgProhibitPHRTimer;
EXTERN    U16 wrSmDfltMacCfgDlPathlossChange;

EXTERN    U16 wrSmDfltDedPhyCfgPa;
/* SPS changes starts */
EXTERN    Bool wrSmDfltIsSpsEnabled;
EXTERN    U16  wrSmDfltMaxDlSpsUePerTti;
EXTERN    U16  wrSmDfltMaxUlSpsUePerTti;
EXTERN    U8   wrSmDfltSpsDlBandwidth;
/* SPS changes ends */
/* PUCCH-ConfigDedicated  */
/* Uplink Power Config dedicated */
EXTERN    S16 wrSmDfltP0UEPUSCH;
EXTERN    U16 wrSmDfltDeltaMCSEnabled;
EXTERN    U8 wrSmDfltAccumulationEnabled;
EXTERN    S16 wrSmDfltP0UEPUCCH;
EXTERN    U16 wrSmDfltpSRSOffset;
EXTERN    U16 wrSmDfltCqiPmiConfigIdx;
EXTERN    U16 wrSmDfltFilterCoef;
/* PUSCH ConfigDedicated */
EXTERN    U16 wrSmDfltBetaOffACKIdx;
EXTERN    U16 wrSmDfltBetaOffRIIdx;
EXTERN    U16 wrSmDfltBetaOffCQIIdx;
/* CQI Reporting Related Information */
EXTERN    U16 wrSmDfltCqiRprtModeAperiodic;
EXTERN    U16 wrSmDfltCqiNomPDSCHRSEPREoff;
EXTERN    U16 wrSmDfltCqiPUCCHResIdx;
EXTERN    U16 wrSmDfltRiConfigIdx;
EXTERN    U8 wrSmDfltSimultAckNackAndCQI;

EXTERN    U8  wrSmDfltSrPrdcty; 
EXTERN    U8  wrSmDfltCqiPrdcty;
EXTERN    U8  wrSmDfltNumSrResPerTti; 
EXTERN    U8  wrSmDfltNumCqiResPerTti;
EXTERN    U16  wrSmDfltDsrTransMax; 

/* Sounding Rs Uplink Configuration */
EXTERN    U16 wrSmDfltSrsBandwidth;
EXTERN    U16 wrSmDfltSrsHoppingBw;
EXTERN    U16 wrSmDfltFreqDomainPos;
EXTERN    U8 wrSmDfltDuration;
EXTERN    U16 wrSmDfltSrsConfigIdx;
EXTERN    U16 wrSmDfltTransmissionComb;
EXTERN    U16 wrSmDfltDedCyclicShift;

/* Scheduling Request Configuration */
EXTERN    U16 wrSmDfltSrPUCCHResIdx;
EXTERN    U16 wrSmDfltDsrTransMax;

/* Antenna Configuration */
EXTERN    U16 wrSmDfltTransMode;

/* DRB PDCP Config  */
EXTERN    U16 wrSmDfltDiscardTimer;
EXTERN    U8 wrSmDfltStatusReportReqd;

/* DRB RLC-Config */
EXTERN    U16 wrSmDfltDrbTPollRetx;
EXTERN    U16 wrSmDfltDrbTPollPDU;
EXTERN    U16 wrSmDfltDrbTPollByte;
EXTERN    U16 wrSmDfltDrbTMaxRetxThres;
EXTERN    U16 wrSmDfltDrbTStatProh;

/* Ctf Cfg Info */
EXTERN    CmLteUeCategory wrSmDfltUECategory;               
EXTERN    CtfPdschPaParam wrSmDfltPdschPA;               
EXTERN    CtfPucchRepFactor wrSmDfltRepFact;               
EXTERN    U16 wrSmDfltN1PUCCHRep;               
EXTERN    U16 wrSmDfltFormatInd;               
EXTERN    U16 wrSmDfltSubBandK;  
EXTERN    CtfUeTxAntMode wrSmDfltAntInfoTxMode;

EXTERN    CtfUeTxAntSelectType wrSmDfltAntennaSel;

/* Rgr cfg Info  UE Cfg */
/* Rgr cfg Info  UE Cfg */
EXTERN    U16 wrSmDfltTxModeEnum;
EXTERN    U16 wrSmDfltAPrdModeEnum;
EXTERN    U16 wrSmDfltPrdModeEnum;
EXTERN    U16 wrSmDfltPrdicityEnum;
EXTERN    U8 wrSmDfltSubframeOffst;
EXTERN    S8 wrSmDfltCqiOffst;
EXTERN    S8 wrSmDfltCqiCfgk;
EXTERN    U16 wrSmDfltCqiPmiCfgIdx;
EXTERN    U8 wrSmDfltMaxUlHqTx;
EXTERN    U8 wrSmDfltDeltaHqOffst;
EXTERN    U8 wrSmDfltTaTmrCmn;
EXTERN    U8 wrSmDfltTaTmrDed;
EXTERN    Bool wrSmDfltTaTmrPres;
EXTERN    U8 wrSmDfltUePrdDlCqiK;
EXTERN    U16 wrSmDfltCqiPmiCfgIdx;
EXTERN    U8 wrSmDfltUeUlPwrTrgCqi;
EXTERN    U8 wrSmDfltAckNackRepFactor;

EXTERN    U32 wrSmDfltUeBr;
EXTERN    U32 wrSmDfltQci;
EXTERN    U32 wrSmDfltDlQosGbr;
EXTERN    U32 wrSmDfltDlQosMbr;
EXTERN    Bool wrSmDfltRiCfgPres;
EXTERN    U32 wrSmDfltRiCfgIndx;
EXTERN    U32 wrSmDfltAntCmnInfoPorts;
EXTERN    Bool veCodeBookRstnPres;
EXTERN    U32 wrPmiBitMap0;
EXTERN    U32 wrPmiBitMap1;

/* Cell Selection and Reselection Related Parameters */
/* SIB3 defaults */
EXTERN    U32 wrSmDfltS3nCellChangeMedium;
EXTERN    U32 wrSmDfltS3nCellChangeHigh;
EXTERN    U32 wrSmDfltS3sNonIntraSearch;
EXTERN    U32 wrSmDfltS3ThreshServingLow;
EXTERN    U32 wrSmDfltS3CellReselectionPriority;
EXTERN    U32 wrSmDfltS3qRxLevMin;
EXTERN    U32 wrSmDfltS3pMax;
EXTERN    U32 wrSmDfltS3sIntraSearch;
EXTERN    U32 wrSmDfltS3AllowedMeasBandwidth;
EXTERN    U32 wrSmDfltS3PresenceAntennaPort1;
EXTERN    U8 wrSmDfltS3NeighCellCfg;
EXTERN    U32 wrSmDfltS3tReselectionEUTRA;
/* SIB5 defaults */
EXTERN    U32 wrSmDfltS5DlCarrierFreq;
EXTERN    S32 wrSmDfltS5QRxLevMin;
EXTERN    U32 wrSmDfltS5InterFreqtReselection;
EXTERN    U32 wrSmDfltS5ThreshXHigh;
EXTERN    U32 wrSmDfltS5ThreshXLow;
EXTERN    U32 wrSmDfltS5AllowedMeasBandwidth;
EXTERN    U32 wrSmDfltS5PresenceAntennaPort1;
EXTERN    U32 wrSmDfltS5CellReselectionPriority;
EXTERN    U8 wrSmDfltS5NeighCellCfg;
EXTERN    U32 wrSmDfltS5QOffsetFreq;
EXTERN U32   wrSmDfltCellIdentity;
EXTERN U8    wrSmDfltNumMncDigits;
EXTERN U8    wrSmDfltNumPlmn;
EXTERN U8    wrSmDfltPlmn1Mcc0;
EXTERN U8    wrSmDfltPlmn1Mcc1;
EXTERN U8    wrSmDfltPlmn1Mcc2;
EXTERN U8    wrSmDfltPlmn1Mnc0;
EXTERN U8    wrSmDfltPlmn1Mnc1;
EXTERN U8    wrSmDfltPlmn1Mnc2;

EXTERN U8    wrSmDfltPlmn2Mcc0;
EXTERN U8    wrSmDfltPlmn2Mcc1;
EXTERN U8    wrSmDfltPlmn2Mcc2;
EXTERN U8    wrSmDfltPlmn2Mnc0;
EXTERN U8    wrSmDfltPlmn2Mnc1;
EXTERN U8    wrSmDfltPlmn2Mnc2;
EXTERN U16   wrSmDfltTac;
EXTERN U32   wrSmDfltPhysCellId;
/* CSG_DEV - removing unused variable - remove when checking-in */
/*EXTERN U32   wrSmDfltCsgId; */
EXTERN U8    wrSmDfltFreqBandInd;
EXTERN U8    wrSmDfltnumSi;
EXTERN U8    wrSmDfltNumNrFreq; /**/
EXTERN U32   wrSmDfltCellId;
EXTERN U16   wrSmDfltNoOfGrp;
EXTERN U32   wrSmDfltOpMode;
EXTERN Bool  wrSmDfltSecEnable;
EXTERN U8    wrSmDfltFreqBandInd;
EXTERN U32   wrSmDfltNumCells; /**/
EXTERN U32   wrSmDfltNumNrEnb; /**/
EXTERN U32 wrSmDfltTagPeriod;
EXTERN U8    wrSmDfltNumOfMncDigits[6];
EXTERN U8    wrSmDfltPlmnMcc[6][3];
EXTERN U8    wrSmDfltPlmnMnc[6][3];

/*SIB3 Defaults*/
EXTERN U8    wrSmDfltqHyst;
EXTERN U8    wrSmDfltSfMed;
EXTERN U8    wrSmDfltSfHigh;
EXTERN U8    wrSmDfltTEval;
EXTERN U8    wrSmDfltTHyst;
EXTERN U16   wrSmDfltUlEarFcn;

/*RB Cfg Related Parameters*/
EXTERN    U8    wrSmDfltAmRlcSrbmaxRetxThresh;
EXTERN    U8    wrSmDfltAmRlcDrbmaxRetxThresh;
EXTERN    U16    wrSmDfltAmRlcSrbmaxpollByte;
EXTERN    U16    wrSmDfltAmRlcDrbmaxpollByte;
EXTERN    U16    wrSmDfltAmRlcSrbpollPdu;
EXTERN    U16    wrSmDfltAmRlcDrbpollPdu;
EXTERN    U16    wrSmDfltAmRlcSrbtPollRetx;
EXTERN    U16    wrSmDfltAmRlcDrbtPollRetx;
EXTERN    U8    wrSmDfltUmRlcsnFieldLen;

/* ANR Related Parameters */
EXTERN  U32  wrSmDfltAnrEnbCfgTmrVal;

/*UM RLC Parameters*/
EXTERN    U8    wrSmDfltAmRlcSrbtStatusProh;
EXTERN    U8    wrSmDfltAmRlcDrbtStatusProh;

EXTERN    U8    wrSmDfltDlSrbtReordring;
EXTERN    U8    wrSmDfltDlDrbtReordring;
EXTERN    U8    wrSmDfltUlSrbtReordring;
EXTERN    U8    wrSmDfltUlDrbtReordring;
EXTERN    U8    wrSmDfltAntPortCnt;

#ifdef E_TM
EXTERN   U8 wrSmDfltBootMode;
EXTERN   U32 wrSmDfltEtmId;
#endif

EXTERN   U32  wrSmNumofEaid;
EXTERN   U32  wrSmEaidList[ ];
EXTERN   U8   wrSmPwsEtwsCmas;
EXTERN   U32  wrSmUlSrsPeriod; 
/* ccpu00128279 : Primary ETWS default parameters. */
EXTERN  U32 wrSmDfltPrimEtwsBrdcst;
EXTERN  U16 wrSmDfltPrimEtwsRepetition;

typedef struct wrRlcUmModeUl
{
   U8                       snFldLen;
   U32                      tReordering;
} WrRlcUmModeUl;
typedef struct wrRlcUmModeDl
{
   U8                       snFldLen;
   U32                      tReordering;
} WrRlcUmModeDl;

typedef struct wrRlcUmMode
{
   WrRlcUmModeUl             ul;
   WrRlcUmModeDl             dl;
} WrRlcUmMode;

typedef struct wrRlcAmModeUl
{
   U32                       statusProhTmr;
   U32                       reorderingTmr;
   U32                       pollByte;
   U32                       pollPdu;
   U32                       pollRetxTmr;
   U32                       maxRetxThrsh;
   U32                       pbr;
   U32                       priority;
   U32                       bucketSizeDur;
   U32                       lcg;
} WrRlcAmModeUl;

typedef struct wrRlcAmModeDl
{
   U32                       statusProhTmr;
   U32                       reorderingTmr;
   U32                       pollByte;
   U32                       pollPdu;
   U32                       pollRetxTmr;
   U32                       maxRetxThrsh;
} WrRlcAmModeDl;

typedef struct wrRlcAmMode
{
   WrRlcAmModeUl             ul;
   WrRlcAmModeDl             dl;
} WrRlcAmMode;

/* Currently, there is no support for header compression. The value of    */
/* hdrCompress should always be set to 0.                                 */
typedef struct wrPdcpCfgParams
{
   U32                       discardTimer;
   U32                       fwdEndTimer;
   U32                       statusRprtReqd;
   U32                       hdrCompress;
   U32                       cfmReqrd;
   U32                       pdcpSnSize;
} WrPdcpCfgParams;

typedef struct wrRlcCfgParams
{
   union
   {
      WrRlcUmMode            um;
      WrRlcAmMode            am;
   } u;
} WrRlcCfgParams;

typedef struct wrLchCfgParams
{
   U8                        priority;
   U8                        bitRate;
   U8                        bucketSizeDur;
   U8                        lchGrp;
} WrLchCfgParams;

typedef struct wrRabCfgParams
{
   U8                        qciSupported;
   U8                        rlcMode;
   U8                        rlcDir;
   U16                       delayBudget;
   WrRlcCfgParams            rlcParams;
   WrPdcpCfgParams           pdcpParams;
   WrLchCfgParams            lchParams;
   U16                       dscp;
   U16                       port;
} WrRabCfgParams;

/* We support QCIs 1 to 9 as mentioned in specification xx.yyy          */
/* The QCI value received is used to index into the array to access the */
/* QCI specific RLC, PDCP parameter                                     */
/* QCI values WR_MAX_QCI + 1 and WR_MAX_QCI + 2 are used for SRB1 and SRB2  */
/* MAX  QCI Number from  */

/* MAX SRB  */
#define WR_MAX_SRBS          2

#define WR_MAX_QCI           9
#define WR_SRB1_QCI          WR_MAX_QCI + 1
#define WR_SRB2_QCI          WR_MAX_QCI + 2
/* This  macro is used as an index for SRB1 and SRB2 in wrRabCfgParams defined
 * in wr_umm.h file */
#define WR_QCI_IDX           3

/* Priority table for SRB/DRB */
typedef struct wrPrioInfoPerRb {
   U8    lchGrp;
   U8    lchPriority;
   U32   pbr;
   U32   bucketSizeDur;
} WrPrioInfoPerRb;

typedef struct wrPrioInfo {
   WrPrioInfoPerRb   srb[WR_MAX_SRBS];
   WrPrioInfoPerRb   drb[WR_MAX_QCI+1];
} WrPrioInfo;

EXTERN    WrPrioInfo wrSmDfltPrioInfoTbl;


/*Neighbour Configuration values*/

EXTERN U32 wrSmDfltNeighEnbID[2];  
EXTERN U8  wrSmDfltNeighEnbIndex[2]; 
EXTERN U8  wrSmDfltNeighEnbType;  


EXTERN U8 wrSmDfltNeEnbNumMncDigits;
EXTERN U8 wrSmDfltNeEnbPlmn1Mcc0[32]; 
EXTERN U8 wrSmDfltNeEnbPlmn1Mcc1[32]; 
EXTERN U8 wrSmDfltNeEnbPlmn1Mcc2[32]; 
EXTERN U8 wrSmDfltNeEnbPlmn1Mnc0[32]; 
EXTERN U8 wrSmDfltNeEnbPlmn1Mnc1[32]; 
EXTERN U8 wrSmDfltNeEnbPlmn1Mnc2[32]; 

EXTERN U8 wrSmDfltNeEnbIPType;

EXTERN S8 *wrSmDfltNeEnbIP[2]; 

EXTERN U8 wrSmDfltNeEnbState;

EXTERN LwrX2ConFlg wrSmDfltX2ConFlg;

EXTERN U8 wrSmDfltNeEnbNoGrpID;
EXTERN U32 wrSmDfltNeEnbGrpIDPLmn;
EXTERN U16 wrSmDfltNeEnbMMEGrpID;


/*MME Confiuration values*/

EXTERN U8 wrSmDfltMmeIndex;
EXTERN U8 wrSmDfltMmeIPType;
EXTERN U16 wrSmDfltMmePort;
EXTERN S8 *wrSmDfltMmeIP;

EXTERN U8 wrSmDftMmeNoPlmn;

EXTERN U8 wrSmDftMmeNoGID;
EXTERN U16 wrSmDfltMmeGID;

EXTERN U8 wrSmDfltMmeNoCode;
EXTERN U16 wrSmDfltMmeCode;

EXTERN U8 wrSmDfltMmeState;
EXTERN U8 wrSmDfltMmeAdminState;


/*eNodeB Configuration*/

EXTERN U8 wrSmDfltMaxNoUE;
EXTERN U8 wrSmDfltEnbNoGrp;

/* EXTERN U32 wrSmDfltEnbID;  */
EXTERN U8  wrSmDfltEnbType;
EXTERN CmTptAddr wrSmDfltEnbIpAddr[WR_MAX_IP_TYPES];  
EXTERN U16 wrSmDfltEnbPort;        
EXTERN S8  wrSmDfltCellSizeType;/*Ue History info*/

EXTERN  U32 wrSmDfltEnbCfgTransTmrVal;
EXTERN U8 wrSmDfltEnbNoPlmn;
EXTERN U8 wrSmDfltNoMME;


/*Neighbour Cell Configuration*/
EXTERN U16 wrSmDfltEartcnNrDLEarfcn[3]; 
EXTERN U8 wrSmDfltNeigCellNum[3]; 
EXTERN U8 wrSmDfltNriIndex[3];
EXTERN U32 wrSmDfltCid;
EXTERN U16 wrSmDfltNeighCellID;
EXTERN U8 wrSmDfltCIO;
EXTERN U8 wrSmDfltNeighCellNoPlmn;
EXTERN U8 wrSmDfltNeighNoOfAnt;
EXTERN U16 wrSmDfltNeighCellPci[3]; 
EXTERN U16 wrSmDfltNeighCellTAC[3]; 
EXTERN U8 wrSmDfltQoff;
EXTERN U8 wrSmDfltNeighBlockedList;
EXTERN U16 wrSmDfltUlEarFcn;
EXTERN U16 wrSmDfltDlEarFcn[3];
EXTERN Bool  wrSmDfltEutranNrIsRemAllowed;
EXTERN      Bool  wrSmDfltEutranNrIsHOAllowed;
EXTERN      U8      wrSmDfltEutranNrCurRank;
EXTERN      U8      wrSmDfltEutranNrPrevRank;

#ifdef OAM_CSFB
EXTERN U8 wrSmDfltVoipInCapable;
#endif /* OAM_CSFB */

/* SIB3 defaults */
EXTERN U32 wrSmDfltS3nCellChangeMedium;
EXTERN U32 wrSmDfltS3nCellChangeHigh;
EXTERN U32 wrSmDfltS3sNonIntraSearch;
EXTERN U32 wrSmDfltS3ThreshServingLow;
EXTERN U32 wrSmDfltS3CellReselectionPriority;
EXTERN U32 wrSmDfltS3qRxLevMin;
EXTERN U32 wrSmDfltS3pMax; 
EXTERN U32 wrSmDfltS3sIntraSearch;
EXTERN U32 wrSmDfltS3AllowedMeasBandwidth;
EXTERN U32 wrSmDfltS3PresenceAntennaPort1;
EXTERN U8  wrSmDfltS3NeighCellCfg;               
EXTERN U32 wrSmDfltS3tReselectionEUTRA;
EXTERN U8  wrSmDfltqHyst;
EXTERN U8  wrSmDfltSfMed;
EXTERN U8  wrSmDfltSfHigh;
EXTERN U8  wrSmDfltTEval;
EXTERN U8  wrSmDfltTHyst;

/* measurement configuration */
EXTERN  U32   wrSmDfltCsfbTmrVal;
EXTERN  U8    wrSmDfltReason;
EXTERN  U32   wrSmDfltThresRSRP1;
EXTERN  U32   wrSmDfltThresRSRQ1;
EXTERN  U32   wrSmDfltThresRSRP2;
EXTERN  U32   wrSmDfltThresRSRQ2;
EXTERN  U32   wrSmDfltA1ThresRSRP;
EXTERN  U32   wrSmDfltA2ThresRSRP;
EXTERN  U32   wrSmDfltA5ThresRSRP1;
EXTERN  U32   wrSmDfltA5ThresRSRP2;
EXTERN  U8    wrSmDfltHoRptCfgVal;
EXTERN  U8    wrSmDfltAnrRptCfgVal;
EXTERN  U8    wrSmDfltArpForEmerServ;
EXTERN  S8    wrSmDfltIntraA3HoOffset;
EXTERN  U8    wrSmDfltTricePrdIntv;
EXTERN  U32   wrSmDfltEpocTmrVal;
EXTERN  S8    wrSmDfltA3AnrOffset;
EXTERN  U8    wrSmDfltAnrCelValidAge;
EXTERN  U32   wrSmDfltInterAnrA5ThresVal1;
EXTERN  U32   wrSmDfltInterAnrA5ThresVal2;
EXTERN  U32   wrSmDfltIntraA5ThresRSRP1;
EXTERN  U32   wrSmDfltIntraA5ThresRSRP2;
EXTERN  U32   wrSmDfltInterA5ThresRSRP1;
EXTERN  U32   wrSmDfltInterA5ThresRSRP2;
EXTERN  U32   wrSmDfltUtraB2ThresRSRP;
EXTERN  S16   wrSmDfltUtraB2ThresRSCP;
EXTERN  S16   wrSmDfltUtraTddB2ThresRSCP;
EXTERN  U16   wrSmDfltUtraB2ThresEcNo;
EXTERN  U32   wrSmDfltUtraB2ThresRSRQ1;
EXTERN  U32   wrSmDfltHysteresis;
EXTERN  U32   wrSmDfltTimeToTrig;
EXTERN  U8    wrSmDfltTrigQty;
EXTERN  U8    wrSmDfltRptQty;
EXTERN  U8    wrSmDfltMaxRptCells;
EXTERN  U32   wrSmDfltRptInterval;
EXTERN  U32   wrSmDfltRptAmount;
EXTERN  S32   wrSmDfltOffset;
EXTERN  U8    wrSmDfltRptOnLeave;
EXTERN  U8    wrSmDfltPurpose;
EXTERN  U8    wrSmDfltCellType;
EXTERN  U32   wrSmDfltCdmaThres1;
EXTERN  U32   wrSmDfltCdmaHysteresis;
EXTERN  U32   wrSmDfltCdmaTimeToTrig;
EXTERN  U8    wrSmDfltCdmaMaxRptCells;
EXTERN  U32   wrSmDfltCdmaRptInterval;
EXTERN  U32   wrSmDfltCdmaRptAmount;
EXTERN  U8    wrSmDflddtPurpose;
EXTERN  U32   wrSmDfltCdmaThresRSRP1;
EXTERN  U32   wrSmDfltCdmaThresRSRQ1;
EXTERN  U8    wrSmDfltSMeasure;
EXTERN  U8    wrSmDfltTimeToTrigSfMedium;
EXTERN  U8    wrSmDfltTimeToTrigSfHigh;
EXTERN  U16   wrSmDfltTEvalulation;
EXTERN  U16   wrSmDfltTHystNormal;
EXTERN  U16   wrSmDfltNCellChangeMedium;
EXTERN  U16   wrSmDfltNCellChangeHigh;
  
EXTERN  U8    wrSmDfltRsrpFltrCoeff;
EXTERN  U8    wrSmDfltRsrqFltrCoeff;
EXTERN  U8    wrSmDfltMeasQtyCDMA2k;
EXTERN  U32   wrSmDfltGapPatForInterFA;
EXTERN  U32   wrSmDfltGapPatForInterRAT;
EXTERN  U32   wrSmDfltGapPatForANR;
EXTERN  U8    wrSmDfltPreRegAllowed;
EXTERN  U8    wrSmDfltPreRegZoneId;
EXTERN  U8    wrSmDfltNumOf2ndPreRegZoneIdList;
EXTERN  U8    wrSmDfltSecondaryPreRegZoneId1;
EXTERN  U8    wrSmDfltSecondaryPreRegZoneId2;

EXTERN  U8    wrSmDfltNumofNrFreq;

EXTERN  U32                  wrSmDfltReCfgType;
EXTERN  U8                   wrSmDfltStatus;
EXTERN  U8                   wrSmDfltPriority;
EXTERN  S8                   wrSmDfltQRxLevMin;
EXTERN  Bool                 wrSmDfltPMaxUsage;
EXTERN  S8                   wrSmDfltPMax;
EXTERN  U8                   wrSmDfltTReselection;
EXTERN  Bool                 wrSmDfltSfUsage;
EXTERN  U8                   wrSmDfltTReselectionSFMed;
EXTERN  U8                   wrSmDfltTReselectionSFHigh;
EXTERN  U8                   wrSmDfltSIntraSearch;
EXTERN  U8                   wrSmDfltSNonIntraSearch;
EXTERN  U8                   wrSmDfltServThresholdLow;
EXTERN  U8                   wrSmDfltThreshXHigh;
EXTERN  U8                   wrSmDfltThreshXLow;
EXTERN  U8                   wrSmDfltMeasBandwidth;
EXTERN  Bool                 wrSmDfltPresAntennaPort1;
EXTERN  U8                   wrSmDfltNeighCellCfg;
EXTERN  U8                   wrSmDfltsSearchPRel9;
EXTERN  U8                   wrSmDfltsSearchQRel9;
EXTERN  S8                   wrSmDfltDualMinRel9;
EXTERN  U8                   wrSmDfltReselecThreshQRel9;
EXTERN  U8                   wrSmDfltThreshXHighQR9;
EXTERN  U8                   wrSmDfltThreshXLowQR9;

EXTERN  U8       wrSmDfltBandClass;
EXTERN  U16      wrSmDfltArfcn;
EXTERN  U8       wrSmDfltSearchRate;
EXTERN  S8       wrSmDfltOffsetFreq;
EXTERN  U16      wrSmDfltAnrUeSearchRate1xRTT;

/*CDMA 1XRTT Band Class Configuration*/
EXTERN U8 wrSmDflt1XRTT;
EXTERN U8 wrSmDfltNumBandClass;
EXTERN U8 wrSmDfltBandClsReCfgType;
EXTERN U8 wrSmDfltBandClsStatus;
EXTERN U8 wrSmDfltBandClass;
EXTERN U8 wrSmDfltBandClsCellReSelPri;
EXTERN U8 wrSmDfltBandClassThresHigh;
EXTERN U8 wrSmDfltBandClassThresLow;
EXTERN U8 wrSmDfltHrpda;

/* CDMA 1xRTT Neigh Freq configuration parameters */
EXTERN U8 wrSmDfltCdma1xNeighFreqReCfgType;
EXTERN U8 wrSmDfltCdma1xSrchWindSize;
EXTERN S8 wrSmDfltCdma1xOffsetFreq;

/* CDMA 1xRTT Mobility parameters */
EXTERN Bool wrSmDfltCdma1xSidIncl; 
EXTERN U16  wrSmDfltCdma1xSid;              
EXTERN Bool wrSmDfltCdma1xNidIncl; 
EXTERN U16  wrSmDfltCdma1xNid;              
EXTERN Bool wrSmDfltCdma1xMultSidsIncl;  
EXTERN Bool wrSmDfltCdma1xMultSids;         
EXTERN Bool wrSmDfltCdma1xMultNidsIncl;  
EXTERN Bool wrSmDfltCdma1xMultNids;         
EXTERN Bool wrSmDfltCdma1xRegZoneIncl;   
EXTERN U16  wrSmDfltCdma1xRegZone;           
EXTERN Bool wrSmDfltCdma1xTotalZonesIncl;
EXTERN U8   wrSmDfltCdma1xTotalZones;       
EXTERN Bool wrSmDfltCdma1xZoneTimerIncl;  
EXTERN U8   wrSmDfltCdma1xZoneTimer;        
EXTERN Bool wrSmDfltCdma1xPktZoneIdIncl;  
EXTERN U8   wrSmDfltCdma1xPktZoneId;        
EXTERN Bool wrSmDfltCdma1xPzIdHysParamIncl;
EXTERN Bool wrSmDfltCdma1xPzHystEnabled;    
EXTERN Bool wrSmDfltCdma1xPzHystInfoIncl;   
EXTERN Bool wrSmDfltCdma1xPzHystListLen;    
EXTERN U8   wrSmDfltCdma1xPzHystActTimer;   
EXTERN U8   wrSmDfltCdma1xPzHystTimerMul;   
EXTERN U8   wrSmDfltCdma1xPzHystTimerExp;   
EXTERN Bool wrSmDfltCdma1xPRevIncl;    
EXTERN U8   wrSmDfltCdma1xPRev;             
EXTERN Bool wrSmDfltCdma1xMinPRevIncl;
EXTERN U8   wrSmDfltCdma1xMinPRev;          
EXTERN Bool wrSmDfltCdma1xNegSlotCycIndxSupIncl; 
EXTERN Bool wrSmDfltCdma1xNegSlotCycIndxSup; 
EXTERN Bool wrSmDfltCdma1xEncModeIncl;
EXTERN U8   wrSmDfltCdma1xEncMode;          
EXTERN Bool wrSmDfltCdma1xEncSupIncl;
EXTERN Bool wrSmDfltCdma1xEncSup;           
EXTERN Bool wrSmDfltCdma1xSigEncSupIncl;
EXTERN U8   wrSmDfltCdma1xSigEncSup;        
EXTERN Bool wrSmDfltCdma1xMsgIntSupIncl;
EXTERN Bool wrSmDfltCdma1xMsgIntSup;        
EXTERN Bool wrSmDfltCdma1xSigIntSupINCLIncl; 
EXTERN Bool wrSmDfltCdma1xSigIntSupINCL;    
EXTERN Bool wrSmDfltCdma1xSigIntSupIncl;
EXTERN U8   wrSmDfltCdma1xSigIntSup;        
EXTERN Bool wrSmDfltCdma1xAuthIncl;
EXTERN U8   wrSmDfltCdma1xAuth;             
EXTERN Bool wrSmDfltCdma1xMaxNumAltoSoIncl;
EXTERN U8   wrSmDfltCdma1xMaxNumAltoSo;     
EXTERN Bool wrSmDfltCdma1xUseSyncIdIncl;
EXTERN Bool wrSmDfltCdma1xUseSyncId;        
EXTERN Bool wrSmDfltCdma1xMsInitPosLocSupIndIncl;
EXTERN Bool wrSmDfltCdma1xMsInitPosLocSupInd; 
EXTERN Bool wrSmDfltCdma1xMobQosIncl;
EXTERN Bool wrSmDfltCdma1xMobQos;           
EXTERN Bool wrSmDfltCdma1xBandClsInfoReqIncl;
EXTERN Bool wrSmDfltCdma1xBandClsInfoReq;   
EXTERN Bool wrSmDfltCdma1xBandClsIncl;
EXTERN U8   wrSmDfltCdma1xBandCls;          
EXTERN Bool wrSmDfltCdma1xBypassRegIndIncl;
EXTERN U8   wrSmDfltCdma1xBypassRegInd;     
EXTERN Bool wrSmDfltCdma1xAltBandClsIncl;
EXTERN U8   wrSmDfltCdma1xAltBandCls;       
EXTERN Bool wrSmDfltCdma1xMaxAddServInstIncl; 
EXTERN U8   wrSmDfltCdma1xMaxAddServInst;   
EXTERN Bool wrSmDfltCdma1xHomeRegIncl;
EXTERN Bool wrSmDfltCdma1xHomeReg;          
EXTERN Bool wrSmDfltCdma1xForSidRegIncl;
EXTERN Bool wrSmDfltCdma1xForSidReg;        
EXTERN Bool wrSmDfltCdma1xForNidRegIncl;
EXTERN Bool wrSmDfltCdma1xForNidReg;        
EXTERN Bool wrSmDfltCdma1xPowerUpRegIncl;
EXTERN Bool wrSmDfltCdma1xPowerUpReg;       
EXTERN Bool wrSmDfltCdma1xPowerDownRegIncl;
EXTERN Bool wrSmDfltCdma1xPowerDownReg;     
EXTERN Bool wrSmDfltCdma1xParmRegIncl;
EXTERN Bool wrSmDfltCdma1xParmReg;          
EXTERN Bool wrSmDfltCdma1xRegPrdIncl;
EXTERN U8   wrSmDfltCdma1xRegPrd;           
EXTERN Bool wrSmDfltCdma1xRegDistIncl;
EXTERN U16  wrSmDfltCdma1xRegDist;          
EXTERN Bool wrSmDfltCdma1xPrefMsIdTypeIncl;
EXTERN U8   wrSmDfltCdma1xPrefMsIdType;     
EXTERN Bool wrSmDfltCdma1xExtPrefMsIdTypeIncl;
EXTERN U8   wrSmDfltCdma1xExtPrefMsIdType;  
EXTERN Bool wrSmDfltCdma1xMeIdReqdIncl;
EXTERN Bool wrSmDfltCdma1xMeIdReqd;         
EXTERN Bool wrSmDfltCdma1xMccIncl;
EXTERN U16  wrSmDfltCdma1xMcc;              
EXTERN Bool wrSmDfltCdma1xImsi1112Incl;
EXTERN U8   wrSmDfltCdma1xImsi1112;         
EXTERN Bool wrSmDfltCdma1xImsiTSupIncl;
EXTERN Bool wrSmDfltCdma1xImsiTSup;         
EXTERN Bool wrSmDfltCdma1xRecntMsgIndIncl;
EXTERN Bool wrSmDfltCdma1xRecntMsgInd;      
EXTERN Bool wrSmDfltCdma1xRerModeSupIncl; 
EXTERN Bool wrSmDfltCdma1xRerModeSup;       
EXTERN Bool wrSmDfltCdma1xTzkModeSupIncl;
EXTERN Bool wrSmDfltCdma1xTzkModeSup;       
EXTERN Bool wrSmDfltCdma1xTzkIdIncl;
EXTERN Bool wrSmDfltCdma1xTzkId;
EXTERN Bool wrSmDfltCdma1xPilRepIncl;
EXTERN Bool wrSmDfltCdma1xPilRep;           
EXTERN Bool wrSmDfltCdma1xSdbSupIncl;
EXTERN Bool wrSmDfltCdma1xSdbSup;           
EXTERN Bool wrSmDfltCdma1xAutoFcsoAllIncl;
EXTERN Bool wrSmDfltCdma1xAutoFcsoAll;      
EXTERN Bool wrSmDfltCdma1xSdbInRcnmIncl;
EXTERN Bool wrSmDfltCdma1xSdbInRcnm;        
EXTERN Bool wrSmDfltCdma1xFpcFchIncl;
EXTERN U8   wrSmDfltCdma1xFpcFchInitSetptRC3;
EXTERN U8   wrSmDfltCdma1xFpcFchInitSetptRC4;
EXTERN U8   wrSmDfltCdma1xFpcFchInitSetptRC5;
EXTERN U8   wrSmDfltCdma1xFpcFchInitSetptRC11;
EXTERN U8   wrSmDfltCdma1xFpcFchInitSetptRC12;
EXTERN Bool wrSmDfltCdma1xTAddIncl;
EXTERN U8   wrSmDfltCdma1xTAdd;             
EXTERN Bool wrSmDfltCdma1xPilotInclIncl;
EXTERN U8   wrSmDfltCdma1xPilotInc;
EXTERN Bool wrSmDfltCdma1xRandIncl;
EXTERN U32  wrSmDfltCdma1xRand;             
EXTERN Bool wrSmDfltCdma1xLpSecIncl;
EXTERN U8   wrSmDfltCdma1xLpSec;
EXTERN Bool wrSmDfltCdma1xLtmOffIncl;
EXTERN U8   wrSmDfltCdma1xLtmOff;           
EXTERN Bool wrSmDfltCdma1xDayltIncl;
EXTERN Bool wrSmDfltCdma1xDaylt;            
EXTERN Bool wrSmDfltCdma1xGcsnaL2ActTimerIncl;
EXTERN U8   wrSmDfltCdma1xGcsnaL2ActTimer;
EXTERN Bool wrSmDfltCdma1xGcsnaSeqCtxtTimerIncl;
EXTERN U8   wrSmDfltCdma1xGcsnaSeqCtxtTimer;
EXTERN U8   wrSmDfltCdma1xReserved;

/* CDMA 1xRTT SIB8 parameters */
EXTERN Bool  wrSmDfltSib8CdmaEutraTimeSync;
EXTERN U8    wrSmDfltSib8SfMedium;
EXTERN U8    wrSmDfltSib8SfHigh;
EXTERN U8    wrSmDfltSib8TReselection;

/* CDMA 1xRTT Default Registration parameters */
EXTERN U16  wrSmDfltCdma1xRegParamsSid;
EXTERN U16  wrSmDfltCdma1xRegParamsNid;
EXTERN Bool wrSmDfltCdma1xRegParamsMulSID;
EXTERN Bool wrSmDfltCdma1xRegParamsMulNID;
EXTERN Bool wrSmDfltCdma1xRegParamsHomeReg;
EXTERN Bool wrSmDfltCdma1xRegParamsForeignSIDReg;
EXTERN Bool wrSmDfltCdma1xRegParamsForeignNIDReg;
EXTERN Bool wrSmDfltCdma1xRegParamsParamReg;
EXTERN Bool wrSmDfltCdma1xRegParamsPowerUpReg;
/* Start Fix for SIB8 */
EXTERN U16  wrSmDfltCdma1xRegParamsRegPeriod;
/* end of SIB8 fix */
EXTERN U16  wrSmDfltCdma1xRegParamsRegZone;
EXTERN U8   wrSmDfltCdma1xRegParamsTotalZone;
EXTERN U8   wrSmDfltCdma1xRegParamsZoneTimer;
EXTERN Bool wrSmDfltCdma1xRegParamsPowerDownReg;
 
/* CDMA 1xRTT Default access class barring parameters */
EXTERN U8 wrSmDfltCdma1xAcBarring0to9;
EXTERN U8 wrSmDfltCdma1xAcBarring10;
EXTERN U8 wrSmDfltCdma1xAcBarring11;
EXTERN U8 wrSmDfltCdma1xAcBarring12;
EXTERN U8 wrSmDfltCdma1xAcBarring13;
EXTERN U8 wrSmDfltCdma1xAcBarring14;
EXTERN U8 wrSmDfltCdma1xAcBarring15;
EXTERN U8 wrSmDfltCdma1xAcBarringMsg;
EXTERN U8 wrSmDfltCdma1xAcBarringReg;
EXTERN U8 wrSmDfltCdma1xAcBarringEmg;

/* CDMA 1xRTT CSFB parameters */
EXTERN LwrRatPriorityList wrSmDfltPlmnRatPriorList;
EXTERN U8          wrSmDfltPlmnRedirSuppLst[6];
EXTERN U8          wrSmDfltCsfbToUtraMeasSup;
EXTERN U8          wrSmDfltCsfbToCdmaMeasSup;
EXTERN U8          wrSmDfltEcsfbMeasSup;
EXTERN U8          wrSmDfltDualRxSup;
EXTERN U8          wrSmDfltCdma1xrttRegSup;
EXTERN U8          wrSmDfltDualRxTxSup;/*RRC Rel 10 Upgrade*/
EXTERN U16         wrSmDfltCdmaMktId;
EXTERN U8          wrSmDfltCdmaSwitchNum;
EXTERN U16         wrSmDfltCdmaCellId;
EXTERN U8          wrSmDfltCdmaSectorNum;
EXTERN U8          wrSmDfltCdmaSib8AdvTime;

EXTERN U64         wrSmDfltSecId;
EXTERN U64         wrSmDfltSecIdPerNgh;
EXTERN U8          wrSmDfltCdmaSystemType;
EXTERN U16         wrSmDfltCdmaChannelNum;
EXTERN U8          wrSmDfltCdmaPilotOneWay;
#ifdef MME_LDBAL
EXTERN  U8         wrSmDfltNumOfPriorityGrps;
#endif

/* Default GERAN parameters */
EXTERN   U32 wrSmDfltGeranReCfgType;
EXTERN   U32  wrSmDfltGeranplmnID;
EXTERN   U16  wrSmDfltGeranLAC;
EXTERN   U16  wrSmDfltGeranRAC;
EXTERN   U8   wrSmDfltGeranBSIC;
EXTERN   U16  wrSmDfltGeranCI;
EXTERN   U8   wrSmDfltGerantReselection;
EXTERN   U8   wrSmDfltGeranpMaxGeran;
EXTERN   U8   wrSmDfltGeranBandInd;
EXTERN   U16  wrSmDfltGeranBcchArfcn;
EXTERN   U8   wrSmDfltGeranNccPermittedMeas;
EXTERN   Bool  wrSmDfltGeranisDTMCapability;
EXTERN   U8   wrSmDfltGeranCellRelectionPriority;
EXTERN   U8   wrSmDfltGeranNccPermitted;
EXTERN   U8   wrSmDfltGeranqRxLevMin;
EXTERN   U32  wrSmDfltGeranQoffset;
EXTERN   U32  wrSmDfltGeranFltrCoeff;
EXTERN   U32  wrSmDfltGeranMaxRptCells;
EXTERN   U32  wrSmDfltGeranRptAmt;
EXTERN   U32  wrSmDfltGeranRptInterval;
EXTERN   Bool wrSmDfltisGERANMeasAllowedForCCO;
EXTERN   Bool wrSmDfltGeranisCCOAllowed;
EXTERN   U32  wrSmDfltGERANMeasWaitTmr;
EXTERN   U32  wrSmDfltGeranT304expWaitTmrVal;
EXTERN   U32  wrSmDfltGeranrrcDatCfmWaitTmr;
EXTERN   U8   wrSmDfltGeranthreshXHigh;
EXTERN   U8   wrSmDfltGeranthreshXLow;
EXTERN   U32  wrSmDfltGerantReselSFMedium;
EXTERN   U32  wrSmDfltGerantReselSFHigh;
EXTERN   Bool wrSmDfltGeranpres_SpeedStateScaleFactors;
EXTERN   Bool wrSmDfltGeranpMaxGeranPress;

/* Action Tyupe Add */
EXTERN  U8       wrSmDfltBlCellInfoReCfgType[];
EXTERN  U8       wrSmDfltBlCellInfoBlackListIdx[];
EXTERN  Bool     wrSmDfltBlCellInfoStatus[];
EXTERN  Bool     wrSmDfltBlCellInfoPciType[];
EXTERN  U16      wrSmDfltBlCellInfoStartPci[];
EXTERN  U8       wrSmDfltBlCellInfoPciRange[];
EXTERN  U16      wrSmDfltBlCellInfoEarfcnUl[];
EXTERN  U16      wrSmDfltBlCellInfoEarfcnDl[];


/* Fill the below variable with value "number of BL cell Info" + 1*/
EXTERN  U8 wrSmDfltBlCellInfoIndex;

/* Black List EUTRAN list */


/* Fill the below variable with value "number of BL eutran Nr" + 1*/
EXTERN  U8  wrSmDfltEutranBlNrIndex;

/* Black List CDMA 1xRTT List */

/* Action Tyupe Add */
EXTERN  U16      wrSmDfltCdmaBlNrReCfgType[];
EXTERN  U8       wrSmDfltCdmaBlNrBlackListIdx[];
EXTERN  Bool     wrSmDfltCdmaBlNrStatus[];
EXTERN  U16      wrSmDfltCdmaBlNrSId[];
EXTERN  U16      wrSmDfltCdmaBlNrNId[];
EXTERN  U16      wrSmDfltCdmaBlNrBaseId[];
EXTERN  U16      wrSmDfltCdmaBlNrPnOffset[];
EXTERN  U16      wrSmDfltCdmaBlNrMarketId[];
EXTERN  U16      wrSmDfltCdmaBlNrCellId[];
EXTERN  U8       wrSmDfltCdmaBlNrSectorNum[];
EXTERN  U32      wrSmDfltCdmaBlNrBandClass[];
EXTERN  U32      wrSmDfltCdmaBlNrArfcn[];

/* Fill the below variable with value "number of BL CDMA 1xRTT Nr" + 1*/
EXTERN  U8 wrSmDfltCdma1xBlNrIndex;

/* Black List CDMA HRPD List */

/* Action Tyupe Add */
EXTERN  U16      wrSmDfltCdmaHrpdBlNrReCfgType[];
EXTERN  U8       wrSmDfltCdmaHrpdBlNrBlackListIdx[];
EXTERN  Bool     wrSmDfltCdmaHrpdBlNrStatus[];
EXTERN  U16      wrSmDfltCdmaHrpdBlNrColorCode[];
EXTERN  U16      wrSmDfltCdmaHrpdBlNrBsmId[];
EXTERN  U16      wrSmDfltCdmaHrpdBlNrBscId[];
EXTERN  U16      wrSmDfltCdmaHrpdBlNrDpssId[];
EXTERN  U16      wrSmDfltCdmaHrpdBlNrBtsId[];
EXTERN  U16      wrSmDfltCdmaHrpdBlNrSectorId[];
EXTERN  U32      wrSmDfltCdmaHrpdBlNrBandClass[];
EXTERN  U32      wrSmDfltCdmaHrpdBlNrArfcn[];
EXTERN  U16      wrSmDfltCdmaHrpdBlNrPnOffset[];


/* Fill the below variable with value "number of BL CDMA HRPD Nr" + 1*/
EXTERN  U8 wrSmDfltCdmaHrpdBlNrIndex;

EXTERN U16 wrSmDfltS1MmeCfgTrfrTmr;
EXTERN U16 wrSmDfltX2SnSTatTrfrTmr;
EXTERN U16 wrSmDfltX2RstRtryCntTmr;
EXTERN U16 wrSmDfltX2SetupRtryCntTmr;
EXTERN U16 wrSmDfltIntSigInactivtTmr;
EXTERN U16 wrSmDfltHRPDCpltTmr;
EXTERN U16 wrSmDfltIntraEnbHoCpltTmr;
EXTERN U16 wrSmDfltMeasureRptToWaitTmr;
EXTERN U16 wrSmDfltS1MmeStatTrfrTmr;
EXTERN U16 wrSmDfltS1RstRtryCntTmr;
EXTERN Bool wrSmDfltDirectFwd;
EXTERN Bool wrSmDfltPlmnEnbCacUsage;
EXTERN Bool wrSmDfltStatRptReqd;

EXTERN U16 wrSmDfltS1SetupTmr;
EXTERN U16 wrSmDfltUpdateTmr;
EXTERN U16 wrSmDfltS1TimeToWait;
EXTERN U16 wrSmDfltS1RtryCntTmr;
EXTERN U16 wrSmDfltS1ResetRetryTmr;
EXTERN U16 wrSmDfltX2SetupTmr;
EXTERN U16 wrSmDfltX2SetTimeToWait;
EXTERN U16 wrSmDfltX2UpdateTmr;
EXTERN U16 wrSmDfltX2UpdateTimeToWait;
EXTERN U16 wrSmDfltX2UpdateRetryCount;
EXTERN U16 wrSmDfltX2ResetRetryTmr;
EXTERN U16 wrSmDfltIntNoWaitRetryTmr;


EXTERN U32 wrSmDfltRrcConSetupTmr;
EXTERN U32 wrSmDfltRrcConReCfgTmr;
EXTERN U32 wrSmDfltRrcConReEstTmr;
EXTERN U32 wrSmDfltRrcSecModCmdTmr;
EXTERN U32 wrSmDfltRrcConRelTmr;
EXTERN U32 wrSmDfltRrcUeCapEnqTmr;
EXTERN U32 wrSmDfltRrcHandoverPrepTmr;
EXTERN U32 wrSmDfltRrcUeInfReqTmr;

EXTERN U16 wrSmDflts1InitialContextSetupTmr;
EXTERN U16 wrSmDflts1ErabReleaseIndicationTmr;
EXTERN U16 wrSmDflts1UeContextReleaseTmr;
EXTERN U16 wrSmDflts1PathSwitchTmr;
EXTERN U16 wrSmDflts1HandoverCancelTmr;
EXTERN U16 wrSmDfltx2HandoverPrepTmr;
EXTERN U16 wrSmDfltx2RelocOverallTmr;
EXTERN U16 wrSmDfltx2StoreUeContextTmr;
EXTERN U16 wrSmDfltx2SnStatusTransferTmr;
EXTERN U16 wrSmDfltinternalSubCellChangeTmr;
EXTERN U16 wrSmDfltinternalSubCellPdcpControlTmr;
EXTERN U16 wrSmDfltehrpdOptHoCompleteTmr;
EXTERN U16 wrSmDfltintraEnbHoCompleteTmr;
EXTERN U16 wrSmDfltmeasurementReportToWaitTmr;
EXTERN U16 wrSmDfltinternalResourceSetupTmr;
EXTERN U16 wrSmDfltinternalResourceReleaseTmr;
EXTERN U16 wrSmDfltinternalSecurityControlTmr;
EXTERN U16 wrSmDfltinternalRrcDataTransferTmr;
EXTERN U16 wrSmDfltinternalForwardingPathSetupTmr;
EXTERN U16 wrSmDfltinternalReestablishControlTmr;
EXTERN U16 wrSmDfltinternalBufferFlushTmr;
EXTERN U16 wrSmDfltinternalDataTransferStartTmr;
EXTERN U16 wrSmDfltinternalDataForwardEndIndTmr;
EXTERN U16 wrSmDfltinternalReestablishTimeToWaitTmr;
EXTERN U16 wrSmDfltinternalUserInactivityTmr;
EXTERN U16 wrSmDfltinternalSignalingInactivityTmr;
EXTERN U16 wrSmDfltinternalUnassignedTmr;
EXTERN U16 wrSmDfltinternalNoWaitRetryTmr;
EXTERN U16 wrSmDfltinternalRrcResetTmr;
EXTERN U16 wrSmDfltinternalEnbResetTmr;
EXTERN U16 wrSmDfltinternalSolicitMeasurementTmr;
EXTERN U16 wrSmDfltinternalRtdInfoReqTmr;



/*fill for UTRA neighbour freq */
EXTERN U32 wrSmDfltUtraReCfgType;
EXTERN U16  wrSmDfltUtraUlArFcn;
EXTERN U16 wrSmDfltUtraDlArFcn;
EXTERN U8 wrSmDfltUtraPriority;
EXTERN U8 wrSmDfltUtraThreshXHigh;
EXTERN U8 wrSmDfltUtraThreshXLow;
EXTERN S8 wrSmDfltUtraQRxLevMin;
EXTERN S8 wrSmDfltUtraDualMin;
EXTERN U8 wrSmDfltUtraDuplexMode;
EXTERN S8 wrSmDfltUtraPMax;
EXTERN U8 wrSmDfltUtraFreqIdx;

/*fill UTRA CELL info*/
EXTERN U8 wrSmDfltUtraNriIndex;
EXTERN U32 wrSmDfltRncID;
EXTERN U16 wrSmDfltUtraCid;
EXTERN U8 wrSmDfltNeRncNumMncDigits;
EXTERN U8 wrSmDfltNeRncPlmn1Mcc0;
EXTERN U8 wrSmDfltNeRncPlmn1Mcc1;
EXTERN U8 wrSmDfltNeRncPlmn1Mcc2;
EXTERN U8 wrSmDfltNeRncPlmn1Mnc0;
EXTERN U8 wrSmDfltNeRncPlmn1Mnc1;
EXTERN U8 wrSmDfltNeRncPlmn1Mnc2;
EXTERN U16 wrSmDfltNeighCellPsc;
EXTERN U16 wrSmDfltNeighCellLac;
EXTERN U8 wrSmDfltNeighCellRac;
EXTERN Bool  wrSmDfltUtranNrIsRemAllowed;
EXTERN Bool  wrSmDfltUtranNrIsHOAllowed;
EXTERN U8      wrSmDfltUtranNrCurRank;
EXTERN U8      wrSmDfltUtranNrPrevRank;

EXTERN U8    wrSmDfltUtraReason;
EXTERN U32   wrSmDfltUtraRptAmount;
EXTERN U32   wrSmDfltUtraRptInterval;
EXTERN U8    wrSmDfltUtraMaxRptCells;
EXTERN U8 wrSmDfltEcnoFltrCoeff;
EXTERN U16 wrSmDfltMeasQtyUtraFdd;
EXTERN U16 wrSmDfltMeasQtyUtraTdd;
EXTERN U8 wrSmDfltReslctSfMed;
EXTERN U8 wrSmDfltReslctSfHigh;
EXTERN U16 wrSmDfltReslct;
EXTERN U8    wrSmDfltUtraNumNrFreq;
EXTERN U8    wrSmDfltUtraNumNrCell;
EXTERN Bool  wrSmDfltMeasRptReqFlg;

#ifdef RSYS_WIRESHARK
EXTERN U32  wrSmDfltWiresharkDstIpAddr;
EXTERN U16  wrSmDfltWiresharkPort;
#endif

/*RBC_SP1_X*/
EXTERN U32 wrSmDfltNoOfPrbRpts;  /*For RRM onlly. To calculate sliding window size*/
EXTERN U32 wrSmDfltPrbRptInterval; /*In milliseconds. For MAC/SCH to report PRBs*/
EXTERN U8 wrSmDfltQci1TmngReq ; /* QCI-1 timing requiremnt */
EXTERN U8 wrSmDfltQci2TmngReq ; /* QCI-2 timing requiremnt */
EXTERN U8 wrSmDfltQci3TmngReq ; /* QCI-3 iming requiremnt */
EXTERN U8 wrSmDfltQci4TmngReq ; /* QCI-4 timing requiremnt */
EXTERN U8 wrSmDfltQci4TmngReqEnable ; /* To enable or disable the QCI-4 timing requiremnt */
EXTERN U8 wrSmDfltMinDlReTxUesPerTti; /*!< Minumum number of DL re-transmission UEs per TTI */
EXTERN U8 wrSmDfltMinUlReTxUesPerTti; /*!< Minumum number of UL re-transmission UEs per TTI */
/*RBC_SP1_Y*/

typedef struct {
   U32   cellNum;
   U32   faIndex;
   S16   status;
   U16   earfcnUl;
   U16   earfcnDl;
   S16   priority;
   S16   qRxLevMin;
   S16   pMaxUsage;
   S16   pMax;
   S16   tReselection;
   S16   sfUsage;
   S16   tReselectionSfMedium;
   S16   tReselectionSfHigh;
   S16   sIntraSearchUsage;
   S16   sIntraSearch;
   S16   sNonIntraSearchUsage;
   S16   sNonIntraSearch;
   S16   threshServingLow;
   S16   threshXhigh;
   S16   threshXLow;
   S16   mesaBandwidthUsage;
   S16   measurementBandwidth;
   S16   presenceAntPort1;
   S16   neighCellConfig;
   S16   qOffsetFreq;
   S16   offsetFreq;
   U16   sIntraSearchRel9Usage;
   U16   sIntraSearchP;
   U16   sIntraSearchQ;
   U16   sNonIntraSearchRel9Usage;
   U16   sNonIntraSearchP;
   U16   sNonIntraSearchQ;
   U32   qQualMinRel9Usage;
   S16   qQualMinRel9;
   U16   threshServingLowQRel9Usage;
   U16   threshServingLowQRel9;
   U16   threshXhighQRel9;
   U16   threshXLowQRel9;
} WrSmDfltEutraFaPriorInfoFunc;

typedef struct {
   U32   cellNum;
   U32   faIndex;
   S16   status;
   S16   duplexType;
   short uarfcnUl;
   short uarfcnDl;
   S16   priority;
   S16   threshXhigh;
   S16   threshXLow;
   S16   qRxLevMin;
   S16   pMaxUtra;
   S16   qQualMin;
   S16   offsetFreq;
   U16   threshXhighQRel9;
   U16   threshXlowQRel9;
} WrSmDfltUtraFaPriorInfoFunc;


typedef struct {
   U32   cellNum;
   U32   faIndex;
   S16   status;
   S16   duplexType;
   short uarfcn;
   S16   priority;
   S16   threshXhigh;
   S16   threshXLow;
   S16   qRxLevMin;
   S16   pMaxUtra;
   S16   offsetFreq;
   U16   threshXhighQRel9;
   U16   threshXlowQRel9;
} WrSmDfltUtraTddFaPriorInfoFunc;

typedef struct {
   U32   cellNum;
   U32   purpose;
   U16   activeState;
   U16   maxReportCell;
   U16   reportInterval;
   U16   reportAmount;
} WrSmDfltUtraPeriodicInfo;

typedef struct {
   U32   cellNum;
   U32   relationIdx;
   U32   status;
   U32   enbId;
   U32   targetCellNum;
   U32   enbType;
   U8    mncSize;
   U8    enbMcc[4];
   U8    enbMnc[4];
   U16   phyCellId;
   U16   tac;
   U8    numPlmns;
   U8    mcc0[4];
   U8    mnc0[4];
   U8    mcc1[4];
   U8    mnc1[4];
   U8    mcc2[4];
   U8    mnc2[4];
   U8    mcc3[4];
   U8    mnc3[4];
   U8    mcc4[4];
   U8    mnc4[4];
   U8    mcc5[4];
   U8    mnc5[4];
   U16   earfcnUl;
   U16   earfcnDl;
   U16   bandwidthUl;
   U16   bandwidthDl;
   U16   indOffset;
   U16   qoffsetCell;
   U16   isRemoveAllowed;
   U16   isHOAllowed;
}WrSmDfltExternalEUtranCellFDDLogic;

typedef struct {
   U32   cellNum;
   U32   relationIdx;
   U32   status;
   U16   rncId;
   U16   cId;
   U16   lac;
   U8    rac;
   U8    reserved2;
   U8    numPlmns;
   LwrPlmnId  plmnIds[6];
   U32   duplexType;
   U16   pScmCode;
   U16   cellParaId;
   U16   arfcnUl;
   U16   arfcnDl;
   U16   isRemoveAllowed;
   U16   isHOAllowed;
   U32   rimStatus;
} WrSmDfltExternalUtranCellLogic;

/*#ifdef TENB_AS_SECURITY*/
typedef struct {
  Bool pres;
  U8   val;
}WreNodBPriIntAlgo; 

typedef struct {
  Bool pres;
  U8   val;
}WreNodBPriCiphAlgo; 

EXTERN WreNodBPriIntAlgo wreNodBPriIntgAlgo[];
EXTERN WreNodBPriCiphAlgo wreNodBPriCiphAlgo[];
/*#endif*/
EXTERN U8    wrSmDfltNumofUtraNrFreq;
EXTERN WrSmDfltUtraFaPriorInfoFunc    wrSmDfltUtraFaPriorInfo[];
EXTERN WrSmDfltUtraTddFaPriorInfoFunc    wrSmDfltUtraTddFaPriorInfo[];
EXTERN U8    wrSmDfltNumofUtraFddNrCell;
EXTERN U8    wrSmDfltNumofUtraTddNrCell;
EXTERN WrSmDfltExternalUtranCellLogic    wrSmDfltExtUtraCell[MAX_NBR_UTRA_CELL];
EXTERN WrSmDfltExternalUtranCellLogic    wrSmDfltExtUtraTddCell[MAX_NBR_UTRA_CELL];

EXTERN U8 wrSmDfltNumofEutraNrFreq;
EXTERN WrSmDfltEutraFaPriorInfoFunc    wrSmDfltEutraFaPriorInfo[];

EXTERN U32 wrSmNumUesGrpPwrCtrlFmt3 ; 
EXTERN U32 wrSmNumUesGrpPwrCtrlFmt3A; 
EXTERN U32 wrSmPciSelType;
EXTERN U32 wrSmEarfcnSelType;
EXTERN U32 wrSmPrachSelType;
EXTERN U8  wrSmDfltGeranB2REDThres1RSRP;
EXTERN U8  wrSmDfltGeranB2HOThres1RSRP;
EXTERN U8  wrSmDfltGeranB2Thres1RSRQ;
EXTERN S8  wrSmDfltGeranB2REDThres2;
EXTERN S8  wrSmDfltGeranB2HOThres2;

#ifdef LTE_QCOM
/* Default RF Configuration which are unsupported from OAM */
/* Record - rfCarrierCommon */
EXTERN U16   rfCarrierCommonCarrierId;       /* 1 to 8 */
EXTERN Bool  rfCarrierCommonAdminState;      /* TRUE/FALSE */
EXTERN Bool  rfCarrierCommonStandalone;      /* TRUE/FALSE */
EXTERN U16   rfCarrierCommonRxPathIdLst_0;
EXTERN U16   rfCarrierCommonRxPathIdLst_1;
EXTERN U16   rfCarrierCommonRxPathIdLst_2;
EXTERN U16   rfCarrierCommonRxPathIdLst_3;
EXTERN U16   rfCarrierCommonTxPathIdLst_0;
EXTERN U16   rfCarrierCommonTxPathIdLst_1;
EXTERN U16   rfCarrierCommonTxPathIdLst_2;
EXTERN U16   rfCarrierCommonTxPathIdLst_3;

   /* Record - rfPathProv */
EXTERN U16   rfPathProvPathId1;             /* 1 to 6 */
EXTERN U16   rfPathProvPath1TxDirection;    /* Rx or Tx */
EXTERN U16   rfPathProvPath1RxDirection;    /* Rx or Tx */

EXTERN U16   rfPathProvPathId2;            /* 1 to 6 */
EXTERN U16   rfPathProvPath2TxDirection;   /*  RF_PATH_DIRECTION_NONE = 0, RF_PATH_DIRECTION_TX = 1, RF_PATH_DIRECTION_RX = 2,  RF_PATH_DIRECTION_LAST =3 */
EXTERN U16   rfPathProvPath2RxDirection;   /*  RF_PATH_DIRECTION_NONE = 0, RF_PATH_DIRECTION_TX = 1, RF_PATH_DIRECTION_RX = 2,  RF_PATH_DIRECTION_LAST =3 */

   /* Record - ffTxCarrierProv */
EXTERN U16   rfTxCarrierProvCarrierId;     /* 1 to 8 */

   /* Record - rfRxCarrierProv */
EXTERN U16   rfRxCarrierProvCarrierId;     /* 1 to 8 */

   /* Record - rfManagerConfig */
EXTERN U32   rfManagerConfigLogLevel;      /* The RF Manager Log Level */
EXTERN U64   rfManagerConfigLogArea;       /* The RF Manager Log area to control rf logging */
EXTERN Bool  rfManagerConfigRequireCal;    /* TRUE/FALSE */
#endif   

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* WR_SMM_SMALLCELL_H */



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
