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
               

     File:     wr_msm_common.h

     Sid:      $SID$ 

     Prg:      Sriky 

*********************************************************************21*/
#ifndef __WR_MSM_COMMON_H__
#define __WR_MSM_COMMON_H__

#ifdef __cplusplus
extern "C" {
#endif /* end of __cplusplus */

#define MSM_TRUE 1
#define MSM_FALSE 0

#define MSM_INIT_0 0
#define MSM_INIT_1 1

#define MAX_ALLOWED_EARFCNDL              64
#define MAX_NUM_OF_IP                     16
#define MSM_MEAS_MAX_EVENT_A1_CFG         4
#define MSM_MEAS_MAX_EVENT_A2_CFG         10
#define MSM_MEAS_MAX_EVENT_A3_CFG         5
#define MSM_MEAS_MAX_EUTRA_PERD_CFG       2
#define MSM_MEAS_MAX_UTRA_PERD_CFG        4
#define MSM_MEAS_MAX_GERAN_EVNT_B2_CFG    2
#define MSM_CSG_OPEN_ACCESS               0x01
#define MSM_CSG_CLOSED_ACCESS             0x02
#define MSM_CSG_HYBRID_ACCESS             0x03
#define MSM_MAX_RATS                      5

#define MAX_NUM_ROOTSEQ_IDX               838
#define MAX_NUM_PRACH_CFG_IDX             256
#define MAX_NUM_ZERO_CORR_CFGZONE         64
#define MAX_NUM_PRACH_FREQ_OFFSET         256
#define MAX_NUM_PCI                       512
#define MAX_NUM_EARFCN                    64
#define MSM_UTRA_DUPLEX_MODE_TDD          1
#define MSM_UTRA_DUPLEX_MODE_FDD          0
#define MAX_NUM_QCI                       9
#if LTE_QCOM
#define MAX_BANDS_SUPPORTED               16
#define MAX_SUPPORTED_SYSTEM              4
#endif

/* SON: Tx Power Chnages */
#define MAX_RS_TXPWR_CNT             128
#define MAX_POWER_OFFSET_CNT         512
#define MAX_PA_CNT                   4
#define MAX_PB_CNT                   8
#define MAX_EARFCN_CNT               128
#define MAX_PCI_CNT                  504
#define MAX_BW_CNT                   6
#define MIN_NCELL_RSCP_THRESHOLD_CNT 3
#define MAX_NCELL_RSCP_THRESHOLD_CNT 8
/* End of SON Changes */

/** @brief Enum defines alarm severity **/
typedef enum
{
   SM_ALARM_SEVERITY_CLEARED = 0,
   SM_ALARM_SEVERITY_WARNING,
   SM_ALARM_SEVERITY_MINOR,
   SM_ALARM_SEVERITY_MAJOR,
   SM_ALARM_SEVERITY_CRITICAL
} SmAlarmSeverity;

/** @brief Enum defines alarm Cause Type **/
typedef enum
{
   SM_ALARM_CAUSE_TYPE_ERR_IND,
   SM_ALARM_CAUSE_TYPE_TMR_EXP
} SmAlarmCause;

/** @brief Enum defines alarm Cause Value **/
typedef enum
{
   SM_ALARM_CAUSE_VAL_INIT_FAIL,
   SM_ALARM_CAUSE_VAL_ABNORMAL
} SmAlarmCauseVal;

/** @brief This enum defines the purpose to use Event A3
 * the following have been defined
 * -# Intra Frequency Handover for Event A3
 * -# Inter Frequency RSRP Handover for Event A3
 * -# Inter Frequency RSRQ Handover for Event A3
 * -# Intra Frequency ANR for Event A3
 * -# Inter Frequency ANR for Event A3
 * -# Reserved1
 * -# Reserved2
 */
typedef enum msmMeasEvntA3Type
{
   MSM_MEAS_EVENTA3_INTRA_FREQ_HO,
   MSM_MEAS_EVENTA3_INTER_FREQ_HO_RSRP,
   MSM_MEAS_EVENTA3_INTER_FREQ_HO_RSRQ,
   MSM_MEAS_EVENTA3_INTRA_FREQ_ANR,
   MSM_MEAS_EVENTA3_INTER_FREQ_ANR,
   MSM_MEAS_EVENTA3_RESV1,
   MSM_MEAS_EVENTA3_RESV2
} MsmMeasEvntA3Type;
#define MSM_MEAS_MAX_EVENT_A5_CFG        5 
/** @brief This enum defines the purpose to use Event A5
 * the following have been defined
 * -# Intra Frequency Handover for Event A5
 * -# Inter Frequency RSRP Handover for Event A5
 * -# Inter Frequency RSRQ Handover for Event A5
 * -# Intra Frequency ANR for Event A5
 * -# Inter Frequency ANR for Event A5
 * -# Reserved1
 * -# Reserved2
 */
typedef enum MsmMeasEvntA5Type
{
   MSM_MEAS_EVENTA5_INTRA_FREQ_HO,
   MSM_MEAS_EVENTA5_INTER_FREQ_HO_RSRP,
   MSM_MEAS_EVENTA5_INTER_FREQ_HO_RSRQ,
   MSM_MEAS_EVENTA5_INTRA_FREQ_ANR,
   MSM_MEAS_EVENTA5_INTER_FREQ_ANR,
   MSM_MEAS_EVENTA5_RESV1,
   MSM_MEAS_EVENTA5_RESV2
} MsmMeasEvntA5Type;

/** @brief This enum defines the purpose to use EUTRA Periodic
 * the following have been defined
 * -# Eutra periodic report strongest cells
 * -# Eutra periodic report CGI
 * -# Reserved2
 */
typedef enum msmMeasEutraPeriodic
{
   MSM_MEAS_EUTRA_PERIODIC_REPORT_STRNG_CELLS,
   MSM_MEAS_EUTRA_PERIODIC_REPORT_CGI,
   MSM_MEAS_EUTRA_PERIODIC_RESV1
}MsmMeasEutraPeriodic;

/** @brief This enum defines the purpose to use UTRA Periodic
 * the following have been defined
 * -# UTRA periodic report strongest cells
 * -# UTRA periodic report ANR
 * -# UTRA periodic report CSFB
 * -# UTRA periodic report CGI
 */
typedef enum msmMeasUtraPeriodic
{
   MSM_MEAS_UTRA_PRDRPT_STRNGCELLS,
   MSM_MEAS_UTRA_PRDRPT_STRGCELLS_ANR,
   MSM_MEAS_UTRA_PRDRPT_STRGCELLS_CSFB,
   MSM_MEAS_UTRA_PERIODIC_REPORT_CGI
}MsmMeasUtraPeriodic;

/* These are CSFB GERAN related macros */
#define   MSM_MAX_NUM_NEIGH_GERAN_CELLS            16 

#define   MSM_NBR_GERAN_FREQ_FREQIDX              1
#define   MSM_NBR_GERAN_FREQ_BANDINDICATOR        2
#define   MSM_NBR_GERAN_FREQ_ARFCN                4
#define   MSM_NBR_GERAN_FREQ_QRXLEVMIN            8
#define   MSM_NBR_GERAN_FREQ_CELLRESEL_PRIORITY   16
#define   MSM_NBR_GERAN_FREQ_THRESHXHIGH          32
#define   MSM_NBR_GERAN_FREQ_THRESHXLOW           64
#define   MSM_NBR_GERAN_FREQ_NCC_PERMITTED        128

#define  MSM_NBR_GERAN_CELL_NRINDEX               1
#define  MSM_NBR_GERAN_CELL_PLMNID                2
#define  MSM_NBR_GERAN_CELL_LAC                   4
#define  MSM_NBR_GERAN_CELL_RAC                   8
#define  MSM_NBR_GERAN_CELL_BSIC                  16
#define  MSM_NBR_GERAN_CELL_CI                    32
#define  MSM_NBR_GERAN_CELL_BANDINDCATOR          64
#define  MSM_NBR_GERAN_CELL_ARFCN                 128
#define  MSM_NBR_GERAN_CELL_NCC_PERMITTED         256
#define  MSM_NBR_GERAN_CELL_DTM_CAPABLE           512

#define MSM_SIB7_TRESELCTIONGERAN                 1
#define MSM_SIB7_PRES_SPEEDSTATEPARAM             2
#define MSM_SIB7_TRESELECT_SF_MEDIUM              4
#define MSM_SIB7_TRESELECT_SF_HIGH                8
#define MSM_SIB7_PMAXGERAN                        16

/** This enum defines the purpose to use GERAN Periodic
 * the following have been defined
 * -# Geran periodic report strongest cells
 * -# Geran periodic report CGI
 */
typedef enum msmLteMeasGERANPeriodic
{
    MSM_MEAS_GERAN_REPORT_STRNG_CELLS,
    MSM_MEAS_GERAN_REPORT_STRNG_CELLS_ANR,
    MSM_MEAS_GERAN_CGI
}MsmLteMeasGERANPeriodic;

#define MSM_MEAS_MAX_UTRA_PERD_CFG        4
#define MSM_MAX_PLMN_IDS                  6
#define MSM_MAX_MCC_DIGITS                3
#define MSM_MAX_NUM_NEIGH_CELLS           40
#define MSM_MAX_NUM_EUTRA_NEIGH_CELLS     32
#define MSM_MAX_SRBS                      3
#define MSM_SRB_ID_1                      1
#define MSM_SRB_ID_2                      2
#define MSM_SRB1                          1
#define MSM_SRB2                          2
#define MSM_SM_MAX_EAIDS                  15
#define MAX_NUM_FREQ_CFG                  16
#define MAX_NUM_CHARS                     255
#define MAX_LTE_BANDS                     16
#define MAX_UMTS_BANDS                    16
#define MSM_MAX_PRB                       6
#define MSM_MAX_PLMN                      6
#define MSM_MAX_NUM_BAND_CFG              32
#define MSM_MAX_QCI                       9
#define MSM_MAX_AS_SEC_ALGOS              4
#define MSM_MEAS_MAX_CDMA_PERD_CFG        3
#define MSM_MEAS_MAX_GERAN_PERD_CFG       3

#ifdef SMM_DATATYPE
typedef u8 U8;
typedef s8 S8;
typedef u16 U16;
typedef s16 S16;
typedef u32 U32;
typedef s32 S32;
typedef u8 Bool;
#endif /* end of SMM_DATATYPE */

typedef enum msmQOffsetRange
{
   MSM_Q_OFFSET_RANGE_MINUS_24,
   MSM_Q_OFFSET_RANGE_MINUS_22,
   MSM_Q_OFFSET_RANGE_MINUS_20,
   MSM_Q_OFFSET_RANGE_MINUS_18,
   MSM_Q_OFFSET_RANGE_MINUS_16,
   MSM_Q_OFFSET_RANGE_MINUS_14,
   MSM_Q_OFFSET_RANGE_MINUS_12,
   MSM_Q_OFFSET_RANGE_MINUS_10,
   MSM_Q_OFFSET_RANGE_MINUS_8,
   MSM_Q_OFFSET_RANGE_MINUS_6,
   MSM_Q_OFFSET_RANGE_MINUS_5,
   MSM_Q_OFFSET_RANGE_MINUS_4,
   MSM_Q_OFFSET_RANGE_MINUS_3,
   MSM_Q_OFFSET_RANGE_MINUS_2,
   MSM_Q_OFFSET_RANGE_MINUS_1,
   MSM_Q_OFFSET_RANGE_0,
   MSM_Q_OFFSET_RANGE_1,
   MSM_Q_OFFSET_RANGE_2,
   MSM_Q_OFFSET_RANGE_3,
   MSM_Q_OFFSET_RANGE_4,
   MSM_Q_OFFSET_RANGE_5,
   MSM_Q_OFFSET_RANGE_6,
   MSM_Q_OFFSET_RANGE_8,
   MSM_Q_OFFSET_RANGE_10,
   MSM_Q_OFFSET_RANGE_12,
   MSM_Q_OFFSET_RANGE_14,
   MSM_Q_OFFSET_RANGE_16,
   MSM_Q_OFFSET_RANGE_18,
   MSM_Q_OFFSET_RANGE_20,
   MSM_Q_OFFSET_RANGE_22,
   MSM_Q_OFFSET_RANGE_24
} MsmLteOffsetRange;

/** @brief Enum defines the msm subscriptions **/
typedef enum msmSubscriptionGroup
{
   MSM_LTE_ADMIN_STATE_CHANGE,
   MSM_LTE_PRACH_CONFIG_COMMON_PARAMETERS,
   MSM_LTE_ENODEB_IP_PARAMETERS,
   MSM_LTE_RACH_CONFIG_COMMON_PARAMETERS,
   MSM_LTE_PDSCH_CONFIG_COMMON_PARAMETERS,
   MSM_LTE_UL_POWER_CONTROL_COMMON_PARAMETERS,
   MSM_LTE_UL_FREQUENCY_INFO_PARAMETERS,
   MSM_LTE_CELL_SIB3_CONFIG_GROUP_PARAMETERS,
   MSM_LTE_CELL_SIB1_CONFIG_GROUP_PARAMETERS,
   MSM_LTE_DRX_CONFIG_PARAMETERS,
   MSM_LTE_CELL_MEAS_CONFIG_PARAMETERS,
   MSM_LTE_ADD_CELL_CONFIG_PARAMETERS,
   MSM_LTE_SM_CELL_CONFIG_PARAMETERS,
   MSM_LTE_CELL_MIB_CONFIG_PARAMETERS,
   MSM_LTE_MAC_SCH_CONFIG_PARAMETERS,
   MSM_LTE_ANTENNA_COMMON_CONFIG_PARAMETERS,
   MSM_LTE_PDSCH_CONFIG_DEDICATED_PARAMETERS,
   MSM_LTE_PUSCH_BASIC_CFG_COMMON_PARAMETERS,
   MSM_LTE_PUSCH_REF_SIGNAL_CFG_PARAMETERS,
   MSM_LTE_PUCCH_CFG_COMMON_PARAMETERS,
   MSM_LTE_SRS_CFG_COMMON_PARAMETERS,
   MSM_LTE_RAB_SRB1_POLICY_CFG_GRP_PARAMETERS,
   MSM_LTE_RAB_SRB2_POLICY_CFG_GRP_PARAMETERS,
   MSM_LTE_UE_TIMER_CONSTANTS_PARAMETERS,
   MSM_LTE_CELL_EAID_CFG_GRP_PARAMETERS,  
   MSM_LTE_CELL_SIB9_CFG_GRP_PARAMETERS,
   MSM_LTE_CQI_PERIODIC_REPORT_CFG_PARAMETERS,
   MSM_LTE_UTRA_NEIGH_FREQ_CFG_PARAMETERS,
   MSM_LTE_EUTRAN_NEIGH_FREQ_CFG_PARAMETERS,
   MSM_LTE_EUTRAN_NEIGHBORLIST_IN_USE_PARAMETERS,
   MSM_LTE_UTRA_NEIGHBORLIST_IN_USE_PARAMETERS,
   MSM_LTE_SIB5_INTER_FREQ_CARRIER_INFO_PARAMETERS,
   MSM_LTE_CELL_SIB6_CONFIG_GROUP_PARAMETERS,
   MSM_LTE_DYNAMIC_CFI_PARAMETERS, 
   MSM_LTE_ENB_PROTO_CFG_PARAMETERS,
   MSM_LTE_UPDATE_SON_SELECTED_PCI,
   MSM_LTE_UPDATE_PCI_LIST,
   MSM_LTE_UPDATE_SON_SELECTED_EARFCNUL,
   MSM_LTE_UPDATE_SON_SELECTED_EARFCNDL,
   MSM_LTE_UPDATE_EARFCN_LIST,
   MSM_LTE_UPDATE_PRACH_CONFIG,
   MSM_LTE_FACTORY_PARAMETERS,
   MSM_LTE_AC_BARRING_INFO_PARAMETERS,
   MSM_LTE_SELF_CONFIG_PARAMETERS,
   MSM_ENB_IP_ADDRESS,
   MSM_LTE_TUNNEL_INFO_PARAMETERS,
   MSM_LTE_POWER_OFFSET_PARAMETERS,
   MSM_LTE_FAP_CONTROL_PARAMETERS,
   MSM_LTE_PRB_ALLOCATION_PARAMETERS, 
   MSM_LTE_SIB4_CSG_INFO_PARAMETERS,
   MSM_LTE_RRM_TDD_PARAMETERS,
   MSM_LTE_TDD_PARAMETERS,
   MSM_LTE_FAP_ACCESS_PARAMETERS,
   MSM_LTE_X2_CONFIG_PARAMETERS,
   MSM_LTE_SCTP_CONFIG_PARAMETERS,
   MSM_LTE_PCCH_CONFIG_COMMON_PARAMETERS,
   MSM_LTE_CELL_UL_SCHD_CONFIG_PARAMETERS,
   MSM_LTE_CELL_DL_SCHD_CONFIG_PARAMETERS,
   MSM_LTE_MS_CELL_CFG_REQ_PARAMETERS,
   MSM_LTE_DBG_PARAMETERS,
   MSM_LTE_UL_ALLOC_INFO_CB_PARAMETERS,
   MSM_LTE_PROC_TIMER_CFG_PARAMETERS,
   MSM_PM_PARAMETERS,
   MSM_FREQUENT_FAULT_PARAMETERS,
   MSM_CDMA_BAND_CLASS_PARAMETERS,
   MSM_CDMA_MOBILITY_PARAMETERS,  
   MSM_BARRING_CFG_1XRTT,         
   MSM_CDMA_1XRTT_CELL,           
   MSM_CDMA_1XRTT_CSFB_REG_INFO,  
   MSM_CDMA_1XRTT_CSFB_CFG_INFO,
   MSM_LTE_SPS_PARAMETERS,
   MSM_LTE_TM_PARAMETERS,
   MSM_CDMA_1XRTT_DSCP_CFG_INFO,
   MSM_TENB_STATS_CFG,
   MSM_LTE_LOG_LVL_PARAMETERS,
   MSM_GERAN_NEIGHBOR_CELL_IN_USE_PARAMETERS,
/* *** Add any new enumaration before this Line *** */
   MSM_LTE_CONFIGURATION_COMPLETE_END
} MsmSubscriptionGroup;

/** @brief the following enum defines the possible actions on the configuration
 * entities like Cell, MME, Neighbour etc.
 */
typedef enum msmActType
{
   MSM_ACT_ADD = 1,   /*!< ACTION = ADD */
   MSM_ACT_MOD,       /*!< Modify */
   MSM_ACT_DEL,       /*!< Delete */
   MSM_DYN_ADD
} MsmActType;

/** @brief Enum defines the Type of neighbour i.e. EUTRA/CDMA2K **/
typedef enum msmLteNeighType
{
   MSM_NEIGH_CELL_EUTRA,
   MSM_NEIGH_CELL_CDMA2K_1XRTT,
   MSM_NEIGH_CELL_CDMA2K_HRPD,
   MSM_NEIGH_CELL_UTRA,
   MSM_NEIGH_CELL_GERAN
}MsmLteNeighType;

/** @brief This structure defines the Confirmation message for dynamic configuration request **/
typedef struct msmDynCfmInfo
{
   U16 cfgType; /*configuration Type (neighbour Cell,neigbour Frequency)*/
   U8 action; /*Configuration action (addition or modification or deletion)*/
   union key /*configuration Index for multi instance configuration (neighbour Cell,neighbour frequency)*/
   {
      U8 neighFreqIndx;
      U8 neighCellIndex;
   } u;
}MsmDynCfmInfo;

/** @brief the following enum defines the possible outcomes of configuration request
 * like SUCCESS, FAILURE etc.
 */
typedef enum msmCfmType
{
   MSM_CFM_SUCCESS = 0,   /*!< CFM = SUCCESS */
   MSM_CFM_FAILURE,       /*!< CFM = FAILURE */
} MsmCfmType;

/** @brief the following enum defines the possible reasons for configuration request failure
 * like config failure, timer expiry etc.
 */
typedef enum msmCauseType
{
   MSM_CAUSE_CFG_FAIL = 0,   /*!< Cause = config fail */
   MSM_CAUSE_TIMER_EXPIRY,       /*!< Cause = timer expiry */
   MSM_CAUSE_NA,       /*!< Cause = not applicable */
} MsmCauseType;

/** @brief This structure defines the ADMIN State params **/
typedef struct msmLteAdminStateParams
{
   U32              adminState;   /*!< Administrative state of the LTE FAP */
   U32              rfTx_OpState; /*!< RfTx state and Op state have to be either 
                     True or False at the same time, hence have been clubbed*/
}MsmLteAdminStateParams;

/************************************************************************************************************
@brief This structure used to store RIM configuration parameters
**************************************************************************************************************/
typedef struct msmLteRimParams
{
   U32 rimOvrlTmrVal; /*This is the overall timer value to initiate RIM procedure for any GERAN/UTRAN cells
                        for which previous RIM procedure has been failed.*/
   U32 rimRirTmr;     /* This timer value is used for retransmission of RAN-INFORMATION-REQUEST PDU */
   U32 rimRiTmr;      /* This timer value is used for retransmission of RAN-INFORMATION PDU */
   U32 rimRiaeTmr;    /* This timer value is used for retransmission of RAN-APPLICATION-ERROR PDU */
   U32 rimMaxRetryCnt; /* Max retransmission attempts for the RIM PDUs RAN-INFORMATION-REQUET and RAN-APPLICATION-ERROR */
}MsmLteRimParams;

typedef enum msmLtePciSelType
{
   MSM_PCI_SEL_DISABLE,
   MSM_PCI_SEL_DSON,
   MSM_PCI_SEL_CSON
}MsmLtePciSelType;

typedef enum msmLtePrachSelType
{
   MSM_PRACH_SEL_DISABLE,
   MSM_PRACH_SEL_DSON,
   MSM_PRACH_SEL_CSON
}MsmLtePrachSelType;

typedef enum msmLteEarfcnSelType
{
   MSM_EARFCN_SEL_DISABLE,
   MSM_EARFCN_SEL_DSON,
   MSM_EARFCN_SEL_CSON
}MsmLteEarfcnSelType;

/* OAM-REQ-1: Generic configuration for enabling SON Features during initial boot-up configuration */
/* Direction: OAM to SM */
typedef struct msmLteSonGenericParams
{
   MsmActType       actType;
   Bool         isPciSelEnabled;
   Bool         isPrachSelEnabled;
   Bool         isEarfcnSelEnabled;
   Bool		isTxPowerSelEnabled; /* SON: Tx Power */
   U8           sonSrvrVendor;
   /*ALL selection type should support CSON also TODO*/
}MsmLteSonGenericParams;

/* This structure is used for dynamic configuration of PCIList from OAM to SM. This is updated to OAM by HeMS */
/* OAM-REQ-5: OAM shall support dynamic configuration of PCI List (triggered from HeMS) for the serving cell.  */
typedef struct msmLteSrvdCellPciListCfg
{
   U32                    numPci;   /*!< Number of PCI's that are part of PCILst */
   U16                    pciLst[MAX_NUM_PCI];  /*!< List of Physical Cell id */
}MsmLteSrvdCellPciListCfg;

typedef struct msmRssiEarfcn
{
   U16   earfcnDl;
   S32   rssi;
}MsmRssiEarfcn;

typedef struct msmLteEarfcnRssiCfg
{
   U8              numEarfcn;
   MsmRssiEarfcn   remScanEarfcn[MSM_MAX_NUM_NEIGH_CELLS];
}MsmLteEarfcnRssiCfg;


typedef struct msmEarfcnListCfg
{
   U8                     numEarfcnDl;
   U8                     numEarfcnUl;
   U32                    earfcnDlLst[MAX_NUM_EARFCN];
   U32                    earfcnUlLst[MAX_NUM_EARFCN];
}MsmEarfcnListCfg;


 /*this structure is separated as it is a per PRACH data, is it so ?
*/
/** @brief This structure defines the PRACH configuration info parameters **/
typedef struct msmPrachCfgInfo
{
   U8       prachCfgIdx;            /*!< PRACH config index determines format and RA
                                    occassions */
   Bool     highSpeedFlag;          /*!< TRUE if high speed is enabled */
   U8       zeroCorrelationZoneCfg; /*!< Zero Correlation zone */
   U8       prachFreqOffset;        /*!< PRACH frequency offset */
} MsmPrachCfgInfo; 

typedef struct msmPrachParamLst
{
   U8    numRootSeqIndex;
   U8    numPrachCfgIdx;
   U8    numZeroCorrZoneCfg;
   U8    numPrachFreqOffset;
   U16   rootSequenceIndxLst[MAX_NUM_ROOTSEQ_IDX];/*!< List of RootSeqIndex. 
                                If SON Prach Selection is not enabled, then the RootSeqIndex to be 
                                configured should be set as the only element in the list */
   U8    prachCfgIdxLst[MAX_NUM_PRACH_CFG_IDX];   /*!< List of PrachCfgIdx. 
                                If SON Prach Selection is not enabled, then the PrachCfgIdx to be configured 
                                should be set as the only element in the list */
   U8    zeroCorrelationZoneCfgLst[MAX_NUM_ZERO_CORR_CFGZONE]; /*!< List of zeroCorrelationZoneCfg. 
                                If SON Prach Selection is not enabled, then the zeroCorrelationZoneCfg to be 
                                configured should be set as the only element in the list */
   U8   prachFreqOffsetLst[MAX_NUM_PRACH_FREQ_OFFSET]; /*!< List of PrachFreqOffset. 
                                If SON Prach Selection is not enabled, then the PrachFreqOffset to be configured 
                                should be set as the only element in the list */
}MsmPrachParamLst;

/** @brief This structure defines the PRACH common configuration parameters **/
typedef struct msmLtePrachCfgCommon
{
   MsmActType       actType;
   U16              rootSequenceIndx;   /*!< ROOT sequence for preambles */
   MsmPrachCfgInfo  prachCfgInfo;           /*!< PRACH configuration */
   U8               prachPosition;      /*!< PRACH Position */
   MsmPrachParamLst prachParamLst;
} MsmLtePrachCfgCommon;

/** @brief This structure defines the Cell Carrier params **/
typedef struct msmLteCellCarrierParams
{
   MsmActType       actType;
   U32          earfdnDl;/* !< Downlink EARFCN*/
   /*U32           allowedEarfcndl[MAX_ALLOWED_EARFCNDL];*/
}MsmLteCellCarrierParams;
typedef struct mmeiptype
{
   U32 mmeipv4;
   U8 mmeipv6[16];
   Bool ipv4pres;
   Bool ipv6pres;
}Mmeiptype;
/** @brief This structure defines the eNodeB IP related params **/
typedef struct msmLteeNodeBIPparams
{
   MsmActType      actType;
   U32             lteS1connmode;                      /*!< No of MMME Ips send in the list*/
   Mmeiptype       lteSiglinkserverlist[MAX_NUM_OF_IP];/*!< MME IP list*/
   U16             sctpPortNumS1;                      /*!< S1AP port number same for all mme*/
   Mmeiptype       lteSecIp1[MAX_NUM_OF_IP];           /*<! Secondary IPs */
   Mmeiptype       lteSecIp2[MAX_NUM_OF_IP]; /*sctpmh*/
   Mmeiptype       lteSecIp3[MAX_NUM_OF_IP];
   Mmeiptype       lteSecIp4[MAX_NUM_OF_IP];
}MsmLteeNodeBIPparams;

/** @brief This structure defines Preamble Configuration params **/
typedef struct msmLtePreambleCfgParams
{
   MsmActType       actType;
   U8              numOfRAPreambles;   /*!< Number of RA preambles BCAST */
   U8              groupASize;         /*!< Group A size */
   U16             msgSizeGroupA;      /*!< Message size group A */
   U8              pwrOffstGrpB;       /*!< Group B message power offset */
}MsmLtePreambleCfgParams;

/** @brief This structure defines Power ramping params **/
typedef struct msmLtePowerRampingParams
{
   MsmActType     actType;
   U8             powerRampingstep;    /*!< Power Ramping step in db */
   U16            initRcvdTrgtPwr;     /*!< Initial preamble power */
}MsmLtePowerRampingParams;

/** @brief This structure defines RA Suppervision info params **/
typedef struct msmLteRaSupervisionCfgParams
{
   MsmActType     actType;
   U8             preambleTransMax;    /*!< Maximum preamble transmisssions */
   U8             rarWindowSize;       /*!< RA response window size */
   U8             macContResTimer;     /*!< MAC contention resolution timer */
}MsmLteRaSupervisionCfgParams;

/** @brief This structure defines RACH common configuration params **/
typedef struct msmLteRachCfgCommonParams
{
   MsmLtePreambleCfgParams      ltePreambleCfg;      /*!< Preamble Configuration */
   MsmLtePowerRampingParams     ltePowerRamping;     /*!< Power ramping config */
   MsmLteRaSupervisionCfgParams lteRaSupervisionCfg; /*!< RA Suppervision info */
   U8                           maxHARQMsg3;      /*!< Msg3 HARQ retransmissions */
} MsmLteRachCfgCommonParams;

/** @brief This structure defines the PDSCH configuration that is broadcast as
 * part of SIB 2.
 * @todo this values are to be determined by SON/Application and is not expected
 * to be populated over the OAM interface. 
 */
typedef struct msmLtePdschCfgCommon
{
   MsmActType     actType;
   U8	       refSignalPwrCnt;                       /*!< Number Parameters to be configured for Reference signal power */
   S8          referenceSignalPwr[MAX_RS_TXPWR_CNT];  /*!< Array of Reference signal power {-60..50} */
   U8	       pBCnt;			              /*!< Num Parameters to be configured for pB */
   U8          pB[MAX_PB_CNT];                        /*!< Array of p-b values */
} MsmLtePdschCfgCommon;

/** @brief This structure defines the UE specific PDSCH configuration **/
typedef struct msmLtePdschCfgDedicated
{
   MsmActType   actType;
   U8	        pACnt;		           	/*!< Num Parameters to be configured for pA */
   U8           pA[MAX_PA_CNT];                 /*!< Array of p-a values */
} MsmLtePdschCfgDedicated;

/** @brief This structure defines the Delta PUCCH formats **/
typedef struct msmLteDeltaPUCCHLst
{
   U8  deltaPucchFormat1;      /*!< Format 1 */
   U8  deltaPucchFormat1b;     /*!< Format 1b */
   U8  deltaPucchFormat2;      /*!< Format 2 */
   U8  deltaPucchFormat2a;     /*!< Format 2a */
   U8  deltaPucchFormat2b;     /*!< Format 2b */
} MsmLteDeltaPUCCHLst;

/** @brief This structure defines the UL power control common parameters that
 * are broadcast */
typedef struct msmLteULPwrCtrlCommon
{
   MsmActType           actType;
   S8                   pNomialPusch;   /*!< P0_Nominal_PUSCH */
   U8                   alpha;          /*!< Alpha 36.213 */
   S8                   pNomialPucch;   /*!< P0_Nominal_PUCCH */
   MsmLteDeltaPUCCHLst  deltaFLst;      /*!< Delta PUCCH for formats */
   S8                   deltaMsg3;      /*!< Delta Preamble Msg3 */
} MsmLteULPwrCtrlCommon;

/** @brief Enum defines the uplink and downlink Bandwidth **/
typedef enum msmLteBandwidth
{
   MSM_LWR_BANDWIDTH_6,
   MSM_LWR_BANDWIDTH_15,
   MSM_LWR_BANDWIDTH_25,
   MSM_LWR_BANDWIDTH_50,
   MSM_LWR_BANDWIDTH_75,
   MSM_LWR_BANDWIDTH_100
} MsmLteBandwidth;

/** @brief Enum defines number of antenna ports configured **/
typedef enum msmLteAntennaPortsCount
{
   MSM_LWR_AP_CNT_1,
   MSM_LWR_AP_CNT_2,
   MSM_LWR_AP_CNT_4
}MsmLteAntennaPortsCount;
 
/** @brief This structure defines the UL Frequency info params **/
typedef struct msmLteULFrequencyInfo
{
   MsmActType        actType;
   U16               eARFCN;             /*!< ARFCN for the cell */
   MsmLteBandwidth   ulBandwidth;        /*!< UL bandwidth */
/* U8                addSpectEmission; */ /*!< Additional Spectrum emission */
} MsmLteULFrequencyInfo;


typedef struct msmLteDynCfiGrp
{
   Bool isDynCfiEnb;   /*!< Indicate whether Dynamic CFI feature is enabled or not */
}MsmLteDynCfiGrp;
/** @brief This structure used to get Measurement Mobilty State Params from EMM. **/
typedef struct msmLteMeasMobilityStatParams
{
   MsmActType               actType;
   U16                      tEvalulation;     /*!< Duration for evaluating allowed amount of cell re-selection
                                                   required to enter mobility states(seconds)*/
   U16                      tHystNormal;      /*!< Duration for evaluating criteria to enter normal mobility state*/
   U16                      nCellChangeMedium;/*!< Defines the number of cell re-selections within TEvaluation
                                                   to enter Medium-mobility state*/
   U16                      nCellChangeHigh;  /*!< Defines the number of cell re-selections within TEvaluation 
                                                   to enter High-mobility state*/
} MsmLteMeasMobilityStatParams;

/** @brief This structure defines the Intra Freq re-selection params for Sib3 **/
typedef struct msmLteSib3IntraFreqCellResel
{
   MsmActType           actType;
   S8                   qRxLevMin;         /*!< REQUIRED minimum received RSRP level for intra-frequency E-UTRA cell re-selection*/
   S8                   pMax;              /*!< Used to limit the allowed UE uplink transmission power on the serving frequency*/
   U8                   SIntraSearch;      /*!< Threshold for intra-frequency measurements*/
/* U8                      allwdMeasBandwidth;*/
/* U8                      neighCellCfg;      */
   U8                   tReselEutra;       /*!< Cell re-selection timer for intra-frequency E-UTRA cell re-selection*/
   U8                   tReselectionSfMedium;/*!< Scaling factor for intra-frequency TreselectionEUTRA in Medium-mobility state*/
   U8                   tReselectionSfHigh;  /*!< Scaling factor for intra-frequency TreselectionEUTRA in High-mobility state*/
} MsmLteSib3IntraFreqCellResel;

/** @brief This structure defines the Intra serving Freq info for Sib3 **/
typedef struct msmLteSib3ServFreqInfo
{
   MsmActType                actType;
   U8                        sNonIntraSearch; /*!< Threshold for inter-frequency and inter-RAT*/
   U8                        threshServingLow;/*!< ThreshServingLow*/
   U8                        cellReselPrio;   /*!< Absolute priority of the E-UTRAN serving carrier frequency*/
} MsmLteSib3ServFreqInfo;

/** @brief This structure contains the information for the SIB3 group of
 * information.
**/
typedef struct msmLteCellSib3CfgGrp
{
   MsmActType                    actType;
   U8                            qHyst;          /*!< Hysteresis value applied to serving cell for evaluating cell ranking criteria*/
   U8                            sfMedium;       /*!< Speed-dependent scaling factor for Qhyst in Medium-mobility state(dB) */
   U8                            sfHigh;         /*!< Speed-dependent scaling factor for Qhyst in High-mobility state(dB)*/
   MsmLteMeasMobilityStatParams  mobStateParams; /*!< Mobility state Param */
   MsmLteSib3ServFreqInfo        servFreqInfo;   /*!< Sib3 Intra serving Freq Info */
   MsmLteSib3IntraFreqCellResel  intraFreqReSel; /*!< Sib3 Intra Freq Cell re-selection Param */

} MsmLteCellSib3CfgGrp;

/** @brief This structure defines PLMN Id **/
typedef struct msmLtePlmnId
{
   U8             numMncDigits;   /*!< Number of digits in the MNC */
   U8             mcc[3];         /*!< Mobile country code */
   U8             mnc[3];         /*!< Mobile country code */
} MsmLtePlmnId;

/** @brief This structure contains PLMN Info **/
typedef struct msmLtePlmnInfo
{
   MsmLtePlmnId             plmnId;           /*!< PLMN ID */
   U8                       rsrvdFrOperUse;   /*!< Reserved for Operator */
   U8                       isPrimary;     /*!< Indicates whether primary PLMN or not*/
} MsmLtePlmnInfo;

/** @brief This structure contains the cell access parameters **/
typedef struct msmLteCellAccessInfo
{
   MsmActType         actType;
   U8                 maxNumOfPlmns;          /*!< Max Number of PLMNs */
   U8                 numOfPlmns;             /*!< Number of PLMNs */
   MsmLtePlmnInfo     plmnInfo[MSM_MAX_PLMN]; /*!< List of PLMNs to be
                                                     broadcast */
   U16                tac;                    /*!< Tracking area code */

   U32                cellId;                 /*!< Cell Identity 28bits LSB */
   Bool               cellBarred;             /*!< Cell is barred or not-barred */
/* Bool               intraFreqReSelection;*/ /*!< Intra frequency reselection
                                                   is allowed or not allowed.*/
   U32                csgIdentity;            /*!< CSG Identity 27 bits */
} MsmLteCellAccessInfo;

/** @brief This structure defines the parameters used for cell reselection by
 * the UE, and broadcast in SIB1
 */
typedef struct msmLteCellSelectInfo
{
   MsmActType         actType;
   S8                 qRxLevMin;        /*!< Minimum RSRP level */
   U8                 qRxLevMinOffset;  /*!< Minimum RSRP level offset */
/*   S8                qQualMin;    */    /*!< Minimum qQual */
/*   TknU8             qQualMinOffset;*/  /*!< qQual offset */
} MsmLteCellSelectInfo;

/** @brief This structure contains the information for the SIB1 group of
 * information.
**/
typedef struct msmLteCellSib1CfgGrp
{
   MsmActType              actType;
   MsmLteCellAccessInfo    cellAccessInfo;   /*!< All information related to cell
                                                access. */
   MsmLteCellSelectInfo    cellSelectInfo;   /*!< Cell Selection related
                                                information */
   S8                      pMax;             /*!< P-MAX Maximum Transmit power */
   U8                      freqBandInd;      /*!< Frequency band indicatory */
} MsmLteCellSib1CfgGrp;

/** @brief This structure used to get Event A1, A2, and A4 Measurement releated
 *     OAM parameters from EMM.
**/
typedef struct msmLteMeasEvntRptCfg
{
   MsmActType               actType;
   U32                      threshold_RSRP; /*!< threshold of RSRP used in EUTRA
                                              measurement report */
   U32                      threshold_RSRQ; /*!<threshold of RSRQ used
                                              in EUTRA measurement report */
   U32                      hysteresis;     /*!< hysteresis value used in entry and
                                              leave condition */
   U32                      timeToTrigger;  /*!< time to trigger value */
   U8                       triggerQty;     /*!< quantity used to calculate
                                              triggering  condition */
   U8                       reportQty;      /*!< Report Quantity */
   U8                       maxRptCells;    /*!< Number of maximum Cell including in
                                              measurement report */
   U32                      reportInterval; /*!< interval of measurement report */
   U32                      reportAmount;   /*!< number of measurement report */
} MsmLteMeasEvntRptCfg;

/** @brief This structure used to get Event A5 Measurement releated OAM
 * parameters from EMM. */
typedef struct msmLteMeasEvntA5RptCfg
{
   MsmActType                actType;
/* U8                        reason;*/      /*!< reason to configure */  
   U32                       threshold1_RSRP; /*!< threshold1 of RSRP used in EUTRA
                                               measurement report */
   U32                       threshold1_RSRQ; /*!< threshold1 of RSRQ used
                                               in EUTRA measurement report */
   U32                       threshold2_RSRP; /*!< threshold2 of RSRP used in EUTRA
                                               measurement report */
   U32                       threshold2_RSRQ; /*!< threshold2 of RSRQ used in EUTRA
                                               measurement report */
   U32                       hysteresis;      /*!< hysteresis value used in entry and
                                               leave condition */
   U32                       timeToTrigger;   /*!< time to trigger value */
   U8                        triggerQty;      /*!< quantity used to calculate
                                               triggering  condition */
   U8                        reportQty;       /*!< Report Quantity */
   U8                        maxRptCells;     /*!< Maximum number of cells that can be included in a measurement report*/
   U32                       reportInterval;  /*!< Interval between successive IRAT measurement reports*/
   U32                       reportAmount;    /*!< Number of times an IRAT measurement report is sent*/
} MsmLteMeasEvntA5RptCfg;

typedef struct msmLteMeasEvntA3RptCfg
{
//   TknPres                   isPres;
   U8                        reason;
   S32                       offset;
   U8                        reportOnLeave;
   U32                       hysteresis;
   U32                       timeToTrigger;
   U8                        triggerQty;
   U8                        reportQty;
   U8                        maxRptCells;
   U32                       reportInterval;
   U32                       reportAmount;
} MsmLteMeasEvntA3RptCfg;

/** @brief This structure holds event-B2 information for UTRAN **/
typedef struct msmLteMeasUtraEvntB2RptCfg
{
   MsmActType                actType;
//   MsmMeasUtraB2Type          reason;              /*!< Reason for using UTRA event B2 */
   U8                         b2Threshold1RSRP;    /*!< UTRA RSRP threshold used in the
//                                                   Inter-RAT measurement report
//                                                   triggering condition for the UTRA 
//                                                   event B2 */ 
   U8                         b2Threshold1RSRQ;    /*!< UTRA RSRQ threshold used in the 
//                                                   Inter-RAT measurement report
//                                                   triggering condition for the 
//                                                   UTRA event B2 */ 
   S8                         b2Threshold2UtraRSCP;  /*!< UTRA RSCP threshold used in the 
                                                     Inter-RAT measurement report
                                                     triggering condition for the UTRA
                                                     event B2 */
   U8                         b2Threshold2UtraEcNo;  /*!< UTRA ECNO threshold used in the 
                                                     Inter-RAT measurement report
                                                     triggering condition for the UTRA
                                                     event B2 */
   U32                        hysteresis;          /*!<Hysteresis applied to entry and
                                                     leaving condition */ 
   U32                        timeToTrigger;       /*!< timeToTrigegr value for UTRA Event
                                                     B2.The timeToTrigger value is
                                                     the period of time that must be
                                                     met for the UE to
                                                     trigger a measurement report */
   U8                         maxReportCell;       /*!< Maximum number of cells that
						     can be included in a measurement report*/
   U32                        reportInterval;      /*!< Interval between successive 
						     IRAT measurement reports*/
   U32                        reportAmount;        /*!< Number of times an IRAT 
						     measurement report is sent*/
   S32                        b2Threshold2UtraTddRSRP;
}MsmLteMeasUtraEvntB2RptCfg;

/** @brief This structure holds event-B2 information for GERAN **/
typedef struct msmLteMeasGeranEvntB2RptCfg 
{
   MsmActType                 actType;
   U32                        b2Threshold1RSRP; /*!< EUTRA Threshold B2 RSRP */
   U32                        b2Threshold1RSRQ; /*!< EUTRA Threshold B2 RSCP */     
   U32                        b2Threshold2Geran; /*!< Geran threshold used
                                                    in the B2 */
   U8                         hysteresis;/*!< Hysteresis applied to entry and
                                                        leaving condition */         
   U16                        timeToTrigger; /*!< timeToTrigegr value for 
                                                GERAN Event B2 */
   U8                         maxReportCell;/*!< The maximum number of cells
                                              included in a measurement report
                                              for GERAN Event B2 */       
   U8                         reportInterval; /*! The reporting interval of a
                                                measurement report for GERAN 
                                                Event B2 */        
   U8                         reportAmount;  /* The number of measurement 
                                                reports  for GERAN Event B2 */  
}MsmLteMeasGeranEvntB2RptCfg;

/** @brief This structure holds CDMA Period Report Configuration information **/
typedef struct msmMeasCDMAPeridRptCfg
{
	U8						   maxReportCell;		/*!< Maximum number of cells that
								 can be included in a measurement report*/
    U32						  reportInterval;	   /*!< Interval between successive 
								  measurement reports*/
    U32						  reportAmount; 	   /*!< Number of times measurement report is sent*/
}MsmMeasCDMAPeridRptCfg;

/** @brief This structure used to get EUTRAN Periodic Measurement releated OAM
 * parameters from EMM.
 * @details These are the structure members
  - Bool        isTobeUsed        TRUE: configure, FALSE: Not configaure
  - U8          reason            reason to configure
  - U8          triggerQty        quantity used to calculate triggering
                                  condition
  - U8          reportQty         Report Quantity
  - U8          maxRptCells       Number of maximum Cell including in
                                  measurement report
  - U32         reportInterval    interval of measurement report
  - U32         reportAmount      number of measurement report 
 *
 * */
typedef struct msmMeasEutraPeridRptCfg
{
   //TknPres                   isPres;
   //U8                        reason;
   U8                        triggerQty;
   U8                        reportQty;
   U8                        maxRptCells;
   U32                       reportInterval;
   U32                       reportAmount;
   Bool                      isPerdStrongCellCfgRptEnable;
   U32                       sonCfgRptInterval;
}MsmMeasEutraPeridRptCfg;

/** @brief This structure holds periodic report configuration information for UTRAN
  - U8    reason;              Reason for using UTRA periodic report configuration
  - Bool  activeState;         Indicates whether periodic report is used or not
  - U8    maxReportCell;       The maximum number of cells included in a measurement
                                     report for the UTRA periodic report
  - U8    reportInterval;      The reporting interval of a measurement report
                                     for the UTRA periodic report
  - U8    reportAmount;        The number of measurement reports for the
			    	     UTRA periodic report
*/
typedef struct msmMeasUtraPeriodRptCfg
{
   //U8                         reason;
   //Bool                       activeState;
   U8                         maxReportCell;
   U8                         reportInterval;
   U8                         reportAmount;
}MsmMeasUtraPeriodRptCfg;

/** @brief This structure used to get Measurement Info parameters from EMM.
 * @details These are the structure members
   - U8        sMeasure                S-Measure
   - U8        sfUsage                 Whether to use Scaling factor
   - U8        timeToTriggerSfMedium   Medium Speed State Scale Factors
   - U8        timeToTriggerSfHigh     High Speed State Scale Factors
 *
 *  */
typedef struct msmMeasSpdStateScaleFact
{
   U8                       sMeasure;
   //TknPres                  sfUsagePres;
   U8                       timeToTriggerSfMedium;
   U8                       timeToTriggerSfHigh;
} MsmMeasSpdStateScaleFact;

/** @brief This structure used to get Quality Config Meas OAM Params. **/
typedef struct msmLteMeasQtyCfgParams 
{  
   MsmActType                actType;
   U8                        rsrpFltrCoeff;  /*!< Filtering coefficient used for RSRP measurements*/
   U8                        rsrqFltrCoeff;  /*!< Filtering coefficient used for RSRQ measurements*/
   U8                        measQtyCDMA2k;  /*!< CDMA2k Meas Quantity */
   U8                        ecnoFltrCoeff;  /*!< Filtering coefficient used for UTRA measurements*/
   U8                        measQtyUtraFdd; /*!< Measurement quantity used for UTRA measurements*/
   U8                        fltrCoeffGERAN;/* Filtering coefficient used for GERAN measurement */
   U8                        filterCoefficient2FDDr10;/*RRC Rel10 Upgrade*/
   U8                        measQtyUtraTdd;
} MsmLteMeasQtyCfgParams;

/* This structure holds the Meas Gap Config information for GERAN */
typedef struct msmLteMeasGapConfig
{
   U32                      gapPatternForInterFA;
   U32                      gapPatternForInterRAT;
   //TknPres                  gapForANRPres;
   U32                      gapPatternForANR;
} MsmLteMeasGapConfig;

/* This structure holds periodic report configuration information for GERAN */
typedef struct msmLteMeasGERANPeridRptCfg
{
   U8          maxRptCells;
   U32         reportInterval;
   U32         reportAmount;
} MsmLteMeasGERANPeridRptCfg;

/** @brief This structure used to store measurement configuration Params in cell
 *     Control block. 
 **/
typedef struct msmLteCellMeasCfgGrp
{
   MsmActType                    actType;
   MsmLteMeasEvntRptCfg          measA1EvtRptCfg[MSM_MEAS_MAX_EVENT_A1_CFG]; /*!< Event A1 configuration */
   MsmLteMeasEvntRptCfg          measA2EvtRptCfg[MSM_MEAS_MAX_EVENT_A2_CFG]; /*!< Event A2 configuration */
//   MsmLteMeasEvntRptCfg          measA4EvtRptCfg[LWR_MEAS_MAX_EVENT_A4_CFG]; /*!< Event A4 configuration */
   MsmLteMeasEvntA3RptCfg        measEvtA3Cfg[MSM_MEAS_MAX_EVENT_A3_CFG];    /*!< Event A3 configuration */
   MsmLteMeasEvntA5RptCfg        measEvtA5Cfg[MSM_MEAS_MAX_EVENT_A5_CFG];      /*!< Event A5 configuration */
/* MsmLteMeasUtraEvntB1RptCfg    measUtraEventB1RptCfg[LWR_MEAS_MAX_UTRA_PERD_CFG];*/
   MsmLteMeasUtraEvntB2RptCfg    measUtraEventB2RptCfg[MSM_MEAS_MAX_UTRA_PERD_CFG]; /*!< UTRA Event B2 configuration */
   MsmLteMeasGeranEvntB2RptCfg   measGeranEventB2RptCfg[MSM_MEAS_MAX_GERAN_EVNT_B2_CFG]; /*!< Geran Event B2 configuration */
   MsmMeasCDMAPeridRptCfg        measCDMA1xPeridRptCfg[MSM_MEAS_MAX_CDMA_PERD_CFG]; /*!< CDMA Period Report Configuration  */
   MsmMeasEutraPeridRptCfg       measEutraPeridRptCfg[MSM_MEAS_MAX_EUTRA_PERD_CFG]; /*!< EUTRA Period Report Configuration  */
   MsmMeasUtraPeriodRptCfg       measUtraPeriodRptCfg[MSM_MEAS_MAX_UTRA_PERD_CFG];  /*!< UTRA Period Report Configuration  */
   MsmMeasSpdStateScaleFact      measSpeedCfg;
   MsmLteMeasQtyCfgParams        qtyCfg;                                       /*!< Quantity configuration */
   MsmLteMeasGapConfig           measGap;
   U8 	            	         isA3SupportReq;                               /*!< TRUE:1, FALSE:0 */
   U8                            anrRptCfg;
   U8                            hoRptCfg;
   MsmLteMeasGERANPeridRptCfg    measGERANPeridRptCfg[MSM_MEAS_MAX_GERAN_PERD_CFG];
}MsmLteCellMeasCfgGrp;

/** @brief This structure 
 - U16  longDrxCycle   This parameter specifies the Long DRX cycle
 - U16  drxStartOffst  This parameter specifies the DRX Start offset
 */
typedef struct msmDrxLongCycleOffst
{
   /* U16      longDrxCycle; */
   U16      longDrxCycleGbr;
   U16      longDrxCycleNonGbr;
   /* U16      drxStartOffst; */
} MsmDrxLongCycleOffst;

/** @brief This structure 
 - Bool    pres        This paramter indicates the presence of short
                       DRX
 - U16     shortDrxCycle This parameter specifies the short DRX cycle
 - U8      drxShortCycleTmr This parameter specifies the short DRX
                            timer value
 */
typedef struct msmDrxShortDrx
{
   U16     shortDrxCycle;    /*!< DRX Short Cycle value in sub-frames*/
   U8      drxShortCycleTmr; /*!< Value in multiples of Short DRX Cycles*/
} MsmDrxShortDrx;
/** @brief This structure holds the Drx Configuration
 - Bool                   isDrxEnabled       Specifies whethere DRX is enabled or not
 - TknS32                 cqiMask            Specifies the CQI mask
 - U8                     drxOnDurTmr        Specifies the Duration of Drx On timer
 - U16                    drxInactvTmr       Specifies the Drx Inactivity timer
 - U8                     drxRetxTmr         Specifies the DRX Retransmission timer
 - WrDrxLongCycleOffst    drxLongCycleOffst  Specifies the long cycle Offset
 - WrDrxShortDrx          drxShortDrx        Specifies the short cycle information
 */
typedef struct msmLteDrxCfgParams
{
   MsmActType              actType;
   Bool                  isDrxEnabled;
/* TknS32                cqiMask;*/
/* U8                      drxOnDurTmr; */
   U16                     drxInactvTmr;
   U8                      drxRetxTmr;
   MsmDrxLongCycleOffst    drxLongCycleOffst;
   /*DRX QCI*/
   Bool                    isDRXQCIEnabled;
/* MsmDrxShortDrx          drxShortDrx; */
}MsmLteDrxCfgParams;

/* SPS changes starts */
typedef struct msmSpsCellConfigData
{
   MsmActType                 actType;
   Bool                       bIsSpsEnabled;     /*!< TRUE = SPS is Enabled and FALSE = SPS is diabled */
   U16                        maxDlSpsUePerTti;  /*!< Number of DL SPS User per TTI for new transmission */
   U16                        maxUlSpsUePerTti;  /*!< Number of UL SPS User per TTI for new transmission */
   U8                         maxDlBwInRbs;        /*!< Number of DL SPS Bandwidth */
} MsmSpsCellConfigData;  
/* SPS changes ends */
 
/* TM4 changes starts */
typedef struct msmTMCellConfig
{
   MsmActType                 actType;
   Bool                       bIsAutoConfigEnabled;     /*!< TRUE = AUTO Config is Enabled and FALSE = Auto Config is disbled */
   U8                         preferredTransMode;        /*!< Preferred Transmission Mode to be use */
   U8                         transModeUse;        /*!< Preferred Transmission Mode to be use */
} MsmTMCellConfig;  
/* TM4 changes ends */


typedef struct msmEnbLogLvlData
{
   U8                         enbL3LogLvl;        /*!< L3 Log Level */
   U8                         enbL2LogLvl;        /*!< L2 Log Level */
   U8                         remoteLogin;       /* remote loging */
   U8                         enbLog_mask;       /*log  mask */
} MsmEnbLogLvlData;  

/** @brief This structure is to add a new configuration. It shall contain all
 * possible configuration coming from the OAM to the Node.
*/
typedef struct msmLteAddCellCfg
{
   MsmActType     actType;
   U32            cellId;      /*!< Cell ID */
   U16            physCellId;  /*!< Physical cell id */
   U8             duplexMode;
   U32            dlCarrierFreq; /*dl earfcn*/
   U32            earfdnDl;/* !< Downlink EARFCN*/
   U32            earfdnUl;/* !< Uplink EARFCN*/
   /* CSG_DEV */
   U8             isSmallCellPciCfgd;
   U16            startPci;
   U16            pciRange;
   Bool           isCcoEnabled; /* Flag to enable/disable CCO */
   Bool           isGERANMeasAllowedForCCO; /* Flag enable/disable Geran Meas */
   U16            cellSizeType;
   U16            emergencyServicePriorityLevel;
   MsmLteSrvdCellPciListCfg   pciListCfg;
   MsmEarfcnListCfg           earfcnListCfg;
   U8          minDlResNonCsg;
   U8          minUlResNonCsg;
} MsmLteAddCellCfg;

/** @brief This structure holds the Security related info **/
typedef struct msmLteGenUeCfg
{
/*Security related info */
  MsmActType   actType;
  S32	       intgAlgo[MSM_MAX_AS_SEC_ALGOS];
  S32          ciphAlgo[MSM_MAX_AS_SEC_ALGOS];
} MsmLteGenUeCfg;

/** @brief This structure holds the Cell cfg related info **/
typedef struct msmLteSmCellCfg
{
   MsmActType     actType;
/* U16          cellId;     */   /* Cell cfg related info */
/* U8           modType;    */   /* Modulation Type */
   U16            duplexMode;    /* duplexing mode : TDD/FDD/HD_FDD */
   S8           maxUeSupp;     /* maximum number of UEs supported by each eNodeB */
   /* RRM_SP1_START */
   U8           maxAvgGbrPrbUsage;   /* maximum Average GBR PRB Usage in % */ 
   U8           rrmQci1TmngReq;      /* QCI-1 Timing Requirements in ms */ 
   U8           rrmQci2TmngReq;      /* QCI-2 Timing Requirements in ms */ 
   U8           rrmQci3TmngReq;      /* QCI-3 Timing Requirements in ms */ 
   U8           rrmQci4TmngReq;      /* QCI-4 Timing Requirements in ms */ 
   U8           rrmQci4TmngReqEnable;      /* QCI-4 Timing Requirements to enable or not */ 

/* TknU8        mcc[3];
   TknU8        mnc[3];    */    /* PLMN ID -Token since size could be of 2 to 3 */
   U16            trackAreaCode; /* Tracking Area Code */
   U8             freqBandInd;   /* Frequency Band Indicator */
   MsmLteGenUeCfg genUeCfg;

//   U16 physCellId;   /* Physical Cell ID */
   U16 rrmRntiStart;
   U16 maxRrmRntis;
   U16 rrmPrmblStart;
   U16 maxRrmPrmbls;
   U16 macRntiStart;
   U16 maxMacRntis;
   U8  numOfMacPrm;
   /*RBC_SP1_X*/
   U32 numOfPrbRpts; /*!< window size - number of PRB reports considered for 
                          taking RB admission control decision */
   U32 prbRptIntvl;  /*!< Interval in which Scheduler reports PRB usage to RRM.
                          Interval is configured in millisecons. Min - 5ms,
                          Max - 650ms */
   /*RBC_SP1_Y*/
//   U32 rsrpTrshldRange;            /* Serving Cell threshold 
//                                                   RSRP value */
//   LwrCellFddCfg    fddCfg;                     /* FDD config INFO */
//   LwrCellTddCfg    tddCfg;                     /* TDD config info */
//   U8              noOfBrdCstPlmn;             /* numbre of broadcasr plmns */
//   U8              plmnId[LWR_MAX_PLMN_IDS][3];
   U8 dfltCfiCfg;
} MsmLteSmCellCfg;

typedef struct msmLteFreqSyncParams
{
   U32          freqSyncPci;
   U32          freqSyncEarfcn;
}MsmLteFreqSyncParams;


/** @brief This structure contains the information for the MIB group of
 * information. */
typedef struct msmLteCellMibCfgGrp
{
   MsmActType            actType;
   U8			 dlBwCnt;                /*!< Downlink Bandwith Count */
   MsmLteBandwidth       dlBw[MAX_BW_CNT];       /*!< Downlink Bandwidth values */
/* MsmPhichCfg           phichCfg;*/   /*!< PHICH Configuration */
} MsmLteCellMibCfgGrp;

 /** @brief This structure holds the MacMainSch Configuration
 - U32 maxHARQ_Tx        Specifies Maximum number of transmissions for UL HARQ
 - U32 periodicBSR_Timer Specifies Timer for BSR reporting
 - U32 retxBSR_Timer     Specifies Timer for BSR reporting
 - U32 ttiBundling       Specifies indicates that TTI bundling enable or disable
 */
typedef struct msmLteMAC_SCH_Config
{
   MsmActType     actType;
   U32            maxHARQ_Tx;
   U32            periodicBSR_Timer;
   U32            retxBSR_Timer;
   U8             ttiBundling;
} MsmLteMAC_SCH_Config;

 /** @brief This structure holds the Antenna Common Configuration **/
typedef struct msmLteAntenna_Common_Config
{
   MsmActType     actType;
   U8             numAntPorts;   /* Number of Antenna ports */
}MsmLteAntenna_Common_Config;

/** @brief This structure holds the neighbour Prach Info as detected by REM/SON  **/
typedef struct msmSonPrachInfo
{
   U16   rootSeqIndex;
   U8    zeroCorrelationZoneCfg;
   U8    highSpeedFlag;
   U8    prachCfgIdx;
   U8    prachFreqOffset;
}MsmSonPrachInfo;

/** @brief This structure holds the neighbour cell information for EUTRA **/
typedef struct msmEUTRANeighCell
{
   Bool         enable;
   U8           nrIndex; /**< Index to the Neighbor Cell Info */
   U32          cid;      /*!< Cell Identity*/
   U16          pci; 
   U16          tac;
               /**< This is Physical Cell ID of E-UTRAN Neighbor Cell
                * on around basestation */
   U8           numPlmn; /**< Number of PLMN ID present for the cell */
   MsmLtePlmnId plmnId[MSM_MAX_PLMN_IDS]; 
               /**< This is Broadcast PLMN List information(MNC) of
                                        *   E-UTRAN Neighbor Cell on around basestation.
                                        */
   U16                        earfcnDl; /**< This is E-UTRAN Neighbor Cell.
                                         *   Downlink EARFCN(E-UTRAN Absolute Radio Frequency
                                         *   Channel Number) on  around basestation
                                         */
   U16                        earfcnUl; /**< This is E-UTRAN Neighbor Cell.
                                         *   Uplink EARFCN on around basestation 
                                         */
   U8                 cio;  /**<  indOffset : This is  Cell Individual Offset applyed to
                                       *   E-UTRAN Neighbor Cell on around basestation.
                                       *   that use when terminal is measuring on RRC
                                       *   Connected mode */
   U8                 qoffset; /**<  qOffsetCell : This is Cell Quality Offset value
                                       *    applied to E-UTRAN Neighbor Cell on around base
                                       *    station. That use Whenreselecting Cell of
                          *    terminal on RRC Idle mode */
   S8                            RSTxPower;/*The downlink reference-signal transmit power*/
   Bool         BlackListed;/*Indicates whether this neighbor cell is allowed 
                              for UEs as handover target or not*/
  U8            csgAccessMode; /* Indicates the type of access mode the FAP operates in */
  U32           csgId;         /* Identity of the Closed Subscriber Group the
                                  cell belongs to. The field is present in a CSG cell */
  Bool  	mustInclude; /*Indicates whether this instance of the neighbor shall be 
				included or excluded in the FAP's NL configuration */
   U8           numAntenna; /* Antenna count of the neihbour */
   U8           ulDlSubFrameCfg; /* Subfame config of the neibhour */
   U8           splSfCfg; /* Special subframe config of the neighbour */
   U8           cpDl; /* DL cyclic prefix */
   U8           cpUl; /* UL cyclic prefix */
   S16          rsrp; /* RSRP in the range of -128 to 0 */
   S16          rssi; /* RSSI in the range of 0 to 256 */
   U8           ulBw; /* UL Bandwidth of the neighbor 0=6,1=15,2=25,3=50,4=75,5=100 */
   U8           dlBw; /* DL Bandwidth of the neighbor 0=6,1=15,2=25,3=50,4=75,5=100 */
   U16          lteBand; /* LTE Band of the cell */
   Bool         isCellBarred; /* Indicates if cell is barred or not */
   Bool             isPrachConfig; /* Informs if PRACH Configuration of neighbor cell is present. */
   MsmSonPrachInfo  prachCfgInfo;
   U8               numIntraFreqNeighbors;
   U16              intraFreqNghPci[MSM_MAX_NUM_NEIGH_CELLS]; /* Physical cell Id of Neighbor cell */
   U8               isHeMsCell; 
}MsmEUTRANeighCell;

/** @brief This structure holds the neighbour cell information for UTRAN **/
typedef struct msmUtraTddNeighCell
{
   U32                          nrIndex;    /*!< Database index of UTRAN neighboring cell */ 
   U16                          rncId;      /*!< RNC ID of UTRAN neighboring cell to the eNB */
   U16                          cId;        /*!< Cell Identity*/ 
   U16                          lac;        /*!< Location Area Code (LAC)*/                                     
   U8                           rac;        /*!< Routing Area Code (RAC)*/                                    
   U8                           ura;        /*!< Routing Area Code (RAC)*/ 
   U8                           numPlmn;                                 
   MsmLtePlmnId                 plmnId[MSM_MAX_PLMN_IDS];
   S8                           pccpch;
   U16                          arfcn;
   U8                           duplexMode;/*to indicate TDD or FDD*/
   U8                           cellParamId;
   Bool                         isRimSupported; /*Indicates whether neighbor cell supports RIM*/
} MsmUtraTddNeighCell; 

/** @brief This structure holds the neighbour cell information for UTRAN **/
typedef struct msmUtraNeighCell
{
   U32                          nrIndex;    /*!< Database index of UTRAN neighboring cell */ 
   U16                          rncId;      /*!< RNC ID of UTRAN neighboring cell to the eNB */
   U16                          cId;        /*!< Cell Identity*/ 
   U16                          lac;        /*!< Location Area Code (LAC)*/                                     
   U8                           rac;        /*!< Routing Area Code (RAC)*/                                    
   U8                           numPlmn;                                 
   MsmLtePlmnId                 plmnId[MSM_MAX_PLMN_IDS];
   U16                          psc;          /*!< Primary CPICH scrambling code*/                                   
   S8                           pcpichTxPower;/*!< Primary CPICH Tx power (dBm)*/
   U16                          arfcnUl;       /*!< UL UTRA Absolute Radio Frequency Channel Number 
                                                              (UARFCN) in an FDD mode cell*/
   U16                          arfcnDl;       /*!< DL UTRA Absolute Radio Frequency Channel Number 
                                                               (UARFCN) in an FDD mode cell*/      
   U8                           duplexMode;/*to indicate TTD of FDD*/
   Bool                         isRimSupported; /*Indicates whether neighbor cell supports RIM*/
   Bool                         isVoipCapable; /*Indicates whether neighbor cell supports Voip*/
   Bool                         isPsHOCapable; /*Indicates whether neighbor cell supports PS HO*/
} MsmUtraNeighCell;

/* Structure for CDMA target cell information ,
 * which consist of - marketId, switchNum, cellId & sectorNum 
 */
typedef struct msmCdmaTgtcell
{
   U16          marketId; 
   U16          switchNum;
   U16          cellId;
   U16          sectorNum;
}MsmCdmaTgtCell;

typedef struct msmCDMA1xNeighCell
{
   U8             cellType;
   S8             *CID;
   U8             bandClass; 
   U32            arfcn;
   U16            pnOffset; 
   U16            curRank;
   MsmCdmaTgtCell tgtCell;		  /* sector id per neighbour */ 
   U8	            systemType;	  /* system Type per pilot */
   U16		      channelNum;	/* channel num per pilot */
} MsmCDMA1xNeighCell;

/* This structure defines the GERAN neighbor cell params */
typedef struct msmLteGERANNeighCell
{
   MsmLtePlmnId plmnId;
   U16          lac;
   U16          rac;
   U8           bsic;
   U16          ci;
   U8           bandIndicator;
   U16          bcchArfcn;
   U8           ncc_PermittedMeas;
   Bool         isDTMCapable; 
   Bool         rimSupportForGeranNgh;
   U8           networkControlOrder;
   Bool         networkControlOrderPres;
   S8           qoffsetGERAN;
}MsmLteGERANNeighCell;

/** @brief This structure defines the addition of cell **/
typedef struct msmNeighCellAddCfg
{
   MsmEUTRANeighCell           eutraCell;
   MsmCDMA1xNeighCell          cdma1xCell;
/*   MsmCDMAHrpdNeighCell      cdmaHrpdCell;*/
   MsmUtraNeighCell            utranCell;
   MsmUtraTddNeighCell         utranTddCell;
   MsmLteGERANNeighCell        geranCell;
} MsmNeighCellAddCfg;

/** @brief This structure defines the deletion of Eutran cell **/
typedef struct msmEutranCellDelCfg
{
   U16            nrIndex; /*!< Index of the neighbor cell */
   U16            earfcn; /*!< EARFCN of the cell */
   U16            phyCellId; /*!< PCI of the cell */
}MsmEutranCellDelCfg;

/** @brief This structure defines the deletion of cell **/
typedef struct msmDynNeighCellDelCfg
{
   union
   {
      MsmEutranCellDelCfg  eutranCell;
   }t;
}MsmDynNeighCellDelCfg;

/** @brief This structure holds the Neighbour cell configuration **/
typedef struct msmNeighCellCfg
{
/* U8                      cellId; */  /*!< Cell Index */
   MsmActType              actType;   /*!< Configure/ReConfigure/Reset */
   U16                     maxCellEntries;/*Max number of entries*/
   U16                     maxTddCellEntries;/*Max number of entries*/
   U16                     numEutraCells; /*!< Number of Cells to be configured */
   U16                     numUtraCells; /*!< Number of Cells to be configured */
   U16                     numUtraTddCells; /*!< Number of Cells to be configured */
   U16                     numGeranCells; /*!< Number of Cells to be configured */
   U16                     numCdma1xCells; /*!< Number of Cells to be configured */
   U16                     isX2Enabled;
   union
   {
      MsmNeighCellAddCfg      neighAddCfg[MSM_MAX_NUM_NEIGH_CELLS];   /*!< Static config */
   }t;
} MsmNeighCellCfg;

typedef struct msmDynNeighCellAddModCfg
{
   union
   {
      MsmEUTRANeighCell           eutraCell;
      MsmCDMA1xNeighCell          cdma1xCell;
      MsmUtraNeighCell            utranCell;
      MsmUtraTddNeighCell         utranTddCell;
      MsmLteGERANNeighCell        geranCell;
   }t;
}MsmDynNeighCellAddModCfg;


/* Dynamic config - Only one cell can be added/modified/deleted */
typedef struct msmDynNeighCellCfg
{
   MsmActType         actType;
   U8                 cellType;
   union
   {
      MsmDynNeighCellAddModCfg    addModCfg;
      MsmDynNeighCellDelCfg       delCfg;
   }t;
}MsmDynNeighCellCfg;


/** @brief This structure defines the Basic PUSCH configuration **/
typedef struct msmLtePuschBasicCfgCommon
{
   MsmActType              actType;
   U8                      nSb;           /*!< n-SB i.e. number of Subbands 36.211[5.3.4] */
   U8                      hoppingMode;   /*!< PUSCH hopping mode */
   U8                      hoppingOffset; /*!< PUSCH hopping offset */
   Bool                    enable64QAM;   /*!< Enable 64 QAM or not */
} MsmLtePuschBasicCfgCommon;
 
/** @brief This structure defines the UL Reference signal configuration **/
typedef struct msmLtePuschRefSignalCfg
{
   MsmActType  actType;
   Bool        grpHopEnabled; /*!< Boolean if group hopping is enabled or not*/
   U8          grpAsgnPusch;  /*!< Group assignment PUSCH i.e. f(ss) in 36.211*/
   Bool        seqHopEnabled; /*!< Sequence hopping enabled or not */
   U8          cyclicShift;   /*!< Value of N1(DMRS) 36.211 */
} MsmLtePuschRefSignalCfg;

/** @brief Enum defines the PUCCH Delta shift **/
typedef enum msmLteDeltaPucchShift
{
   MSM_DELTA_PUCCH_SHIFT_1,
   MSM_DELTA_PUCCH_SHIFT_2,
   MSM_DELTA_PUCCH_SHIFT_3
} MsmLteDeltaPucchShift;

/** @brief Structure defines the pucch configuration that is common to all
 * UEs and broadcast in the SIB
 */
typedef struct msmLtePucchCfgCommon
{
   MsmActType            actType;
   MsmLteDeltaPucchShift deltaShift;   /*!< @todo self PUCCH Delta shift */
   U8                    nRBCqi;       /*!< number of RBs to be used for PUCCH 2/2a..
                                       */
   U8                    nCSAN;        /*!< Cyclic shift determines the number of
                                           mixed RBs */
   U16                   n1PucchAn;    /*!< N1(PUCCH) */
} MsmLtePucchCfgCommon;

/** @brief Structure defines the SRS common configuration */
typedef struct msmLteSRSCfgCommon
{
   MsmActType    actType;
   Bool          enable;           /*!< SRS Enable flag TRUE or FALSE */
   U8            cSrs;             /*!< SRS bandwidth = cSrs */
/* U8          subframeCfg;   */   /*!< SRS subframe configuration */
   Bool          simulAckNackSrs;  /*!< Simultaneous ACK/NACK SRS */
   Bool          srsMaxUpts;       /*!< TDD specific */
} MsmLteSRSCfgCommon;

/** @brief Structure defines the SRB RLC Info*/
typedef struct msmLteSrbRlcInfo
{
   MsmActType  actType;
   U8                        srbId;
   Bool                      srbDefaultCfg;   /*!< Indicates the whether default RLC configuration for SRB is used or not*/
   U8                        tpRetransmit;    /*!< Timer between successive poll transmissions*/
   U8                        pollPdu;         /*!< Counter of PDUs between successive poll transmissions*/
   U8                        pollByte;        /*!< Counter of PDU bytes transmitted between successive poll transmissions*/
   U8                        maxRetransThresh;/*!< Maximum number of transmission retry used by the RLC entity*/
   U8               timerReordering;          /*!< TReordering*/
   U8            timerStatusProhibit;         /*!< TStatusProhibit*/
}MsmLteSrbRlcInfo;

/** @brief Structure defines the SRB Policy */
typedef struct msmLteSrbPolicy
{
   MsmActType             actType;
   U8                        srbId;      /*!< SRB ID */
   MsmLteSrbRlcInfo          rlcInfo;
/* MsmLteSrbLchInfo          lchInfo;
   MsmLteSrbQciInfo          qciInfo;*/
}MsmLteSrbPolicy;

typedef struct msmDrbDscpInfo
{
   S16                     qci;
   U16                     dscp;
   U16                     port;   /*!< Need for Backhual feature */
}MsmDrbDscpInfo;

typedef struct msmLteDrbPolicy
{
   U32                       reCfgType;
   U8                        qci;
   MsmActType                action;   /*!< Config, Modify or Delete */
/*   MsmDrbQciInfo             qciInfo;*/
 /*  MsmDrbLchInfo             lchInfo;
   MsmDrbRlcInfo             rlcInfo;
   MsmDrbPdcpInfo            pdcpInfo;*/
   MsmDrbDscpInfo            dscpInfo;
} MsmLteDrbPolicy;


/** @brief This structure contains the information for the Rab Policy group */
typedef struct msmLteRabPolicyCfgGrp
{
   MsmActType          actType;
   Bool                isDscpEnable;
/* U8                numSrbPolicies;*/
   U8                numDrbPolicies;
   MsmLteSrbPolicy   srbPolicy[MSM_MAX_SRBS]; /*!< SRB Policy */
   MsmLteDrbPolicy   drbPolicy[MSM_MAX_QCI];
} MsmLteRabPolicyCfgGrp;

/** @brief This structure contains the Timer and Constants */
typedef struct msmLteUeTimerConstants
{  
   MsmActType  actType;
   U16         t300;   /*!< RRC Connection Request timer */
   U16         t301;   /*!< RRC Connection reestablishnment timer */
   U16         t302;   /*!< RRC timer */
   U16         t304;   /*!< RRC timer */
   U16         ccoT304;   /*!< RRC timer */
   U16         t310;   /*!< RRC timer */
   U16         n310;   /*!< RRC timer */
   U16         t311;   /*!< RRC timer */
   U16         n311;   /*!< RRC timer */
   U16         t320;   /*!< RRC timer */
} MsmLteUeTimerConstants;

/** @brief This structure contains the Emergency Area ID Configuration */
typedef struct msmLteCellEAIDCfgGrp
{     
   MsmActType  actType;
   U32         eaid[MSM_SM_MAX_EAIDS];/*!< Emergency Area ID*/
} MsmLteCellEAIDCfgGrp; 

/** @brief This structure holds SIB9 Config params **/
typedef struct msmLteCellSib9CfgGrp
{     
   MsmActType   actType;
   U8           henbName[48];/*!< Home NodeB name*/
}MsmLteCellSib9CfgGrp;

/** @brief This structure holds the SubBand CQI **/
typedef struct msmLteSubBandCQI
{
   MsmActType   actType;
   U8           k;      /*!< Sub-band size for sub-band reporting mode*/
}MsmLteSubBandCQI;

/** @brief This structure holds the CQI Report configuration **/
typedef struct msmLteCQIPeriodicReportCfg 
{
   MsmActType        actType;
   U8                cqi_PUCCH_ResourceIdx;/*!< Position of PUCCH*/
   MsmLteSubBandCQI  subBandCQI;           /*!< SubBand CQI */
} MsmLteCQIPeriodicReportCfg;

/** @brief This structure holds the neighbour frequency information for EUTRA **/
typedef struct msmLteNeighEutraFreq
{
/* U32                  reCfgType;*/
   U8                   freqIndex;        /*!< Enables or disables IRAT*/
/* U8                   status;*/
   U16                  earfcnUl;/*manjappa*/
   U16                  earfcnDl;         /*!< ARFCN of the frequency carrier*/
   U8                   priority;         /*!< Absolute priority of the UTRA FDD serving carrier frequency*/
   S8                   qRxLevMin;        /*!< REQUIRED minimum received RSCP level on this UTRA frequency carrier*/
/* Bool                 pMaxUsage;*/
   S8                   pMax;             /*!< Used to limit the allowed UE uplink transmission power on the serving frequency*/
   U8                   tReselection;     /*!< Cell re-selection timer for intra-frequency E-UTRA cell re-selection*/
/* Bool                 sfUsage;*/
   U8                   tReselectionSFMed;/*!< Scaling factor for intra-frequency TreselectionEUTRA in Medium-mobility state*/
   U8                   tReselectionSFHigh;/*!< Scaling factor for intra-frequency TreselectionEUTRA in High-mobility state*/
   U8                   sIntraSearch;      /*!< Threshold for intra-frequency measurements*/ 
   U8                   sNonIntraSearch;   /*!< Threshold for inter-frequency and inter-RAT*/ 
   U8                   servThresholdLow;  /*!< ThreshServingLow*/
   U8                   threshXHigh;       /*!< Threshold used when re-selecting from a lower priority E-UTRAN frequency*/
   U8                   threshXLow;        /*!< Threshold used when re-selecting from a higher priority E-UTRAN frequency*/
   U8                measBandwidth;          
   Bool                 presAntennaPort1;
   U8                   neighCellCfg;  /*!< Bit string (2) */
/* MsmLteOffsetRange      qOffsetFreq;*/   /*!< For SIB 5 */
   U8                    qOffsetFreq;      /*cell-specific offset*/ 
   U8                    offsetFreq;    /*!<  FOR MeasEUTRA */
/* MsmLteSSearchRel9       sIntraSearchRel9;
   MsmLteSSearchRel9       sInterSearchRel9; */
   S8                   qQualMinRel9;    /*!< {-34,..., -3} */
   U8                   threshXHighQR9;
   U8                   threshXLowQR9;
} MsmLteNeighEutraFreq;

/** @brief This structure holds the neighbour frequency information for UTRAN **/
typedef struct msmLteNeighUtraFreq
{
   Bool                   enable;
/* U32                   reCfgType;*/
   U8                    freqIndex;   /*!< Database index of UTRAN neighboring cell */ 
/* U8                    status;*/
   U16                   arfcnUl;     /*manjappa*/
   U16                   arfcnDl;     /*!< ARFCN of the frequency carrier*/ 
   U8                    priority;    /*!< Absolute priority of the UTRA FDD serving carrier frequency*/ 
   U8                    threshXhigh; /*!< Threshold used when re-selecting from a lower priority UTRA frequency*/ 
   U8                    threshXlow;  /*!< Threshold used when re-selecting from a higher priority UTRA frequency*/ 
   S8                    qRxLevMin;   /*!< REQUIRED minimum received RSCP level on this UTRA frequency carrier*/ 
   S8                    pMaxUtra;    /*!< limit the allowed UE uplink transmission power on this UTRA FDD carrier frequency*/ 
   S8                    pQualMin;    /*!< REQUIRED minimum received Ec/Io level on this UTRA FDD carrier*/ 
   S8                    offsetFreq;  /*!< Indicates UTRA-specific offset to be applied when evaluating triggering
                                       conditions for measurement reporting in connected mode*/ 
   U8                    duplexModeUtra;               
   U8                    threshXHighQR9;
   U8                    threshXLowQR9;
} MsmLteNeighUtraFreq;

/** @brief This structure holds the neighbour frequency information for UTRAN **/
typedef struct msmLteNeighUtraTddFreq
{
   U8                    freqIndex;   /*!< Database index of UTRAN neighboring cell */ 
   U16                   arfcn;
   U8                    priority;    /*!< Absolute priority of the UTRA FDD serving carrier frequency*/ 
   U8                    threshXhigh; /*!< Threshold used when re-selecting from a lower priority UTRA frequency*/ 
   U8                    threshXlow;  /*!< Threshold used when re-selecting from a higher priority UTRA frequency*/ 
   S8                    qRxLevMin;   /*!< REQUIRED minimum received RSCP level on this UTRA frequency carrier*/ 
   S8                    pMaxUtra;    /*!< limit the allowed UE uplink transmission power on this UTRA FDD carrier frequency*/ 
   S8                    offsetFreq;  /*!< Indicates UTRA-specific offset to be applied when evaluating triggering
                                       conditions for measurement reporting in connected mode*/ 
   U8                    duplexModeUtra;               
   U8                    tddMode;
   U8                    bandIndicator;
} MsmLteNeighUtraTddFreq;

/* This structure holds the neighbour frequency information for GERAN */
typedef struct msmLteNeighGERANFreq
{
   U8                   bandIndicator;
   U16                  bcchArfcn;
   U8                   qRxLevMin;
   U8                   cellReselectionPriority;
   U8                   threshXHigh;
   U8                   threshXLow;
   U8                   ncc_Permitted;
   U8                   pMaxPres;
   U8                   pMax;
} MsmLteNeighGERANFreq;

/** @brief This structure defines the addition of neighbour Freq **/
typedef struct msmLteNeighCdma1xFreq
{
	S8       offsetFreq;	/*!< QoffsetCDMA2000 for Measurement*/
} MsmLteNeighCdma1xFreq;

typedef struct msmLteNeighEutraDelFreq
{
   U8  freqIndex;
}MsmLteNeighEutraDelFreq;

typedef struct msmLteNeighFreqDelCfg
{
  MsmLteNeighEutraDelFreq    eutranFreq;
/*
  MsmLteNeighCdma1xDelFreq   cdma1xFreq; 
  MsmLteNeighUtraDelFreq     utranFreq;
*/
}MsmLteNeighFreqDelCfg;

/** @brief This structure defines the addition of neighbour Freq **/
typedef struct msmLteNeighFreqAddCfg
{
      MsmLteNeighEutraFreq          eutranFreq;
      MsmLteNeighCdma1xFreq         cdma1xFreq; 
/*      MsmLteNeighCdmaHRPDFreq     cdmaHrpdFreq;*/
      MsmLteNeighUtraFreq           utranFreq;
      MsmLteNeighUtraTddFreq        utranTddFreq;
      MsmLteNeighGERANFreq          geranFreq;
} MsmLteNeighFreqAddModCfg;

/** @brief This structure defines the neighbour Freq configuration **/
typedef struct msmLteNeighFreqCfg
{
   MsmActType  actType;       /*!< ADD/MOD/DEL */
/*  U8              cellId; */  /*!< Cell index */
    U8              maxUtraTddFreqEntries; /*!< Max number of Freq entries */
    U8              numUtraTddFreq;        /*!< number of UTRA freq entries*/
    U8              maxUtraFreqEntries; /*!< Max number of Freq entries */
    U8              numUtraFreq;        /*!< number of UTRA freq entries*/
    U8              numEutraFreq;       /*!< number of EUTRA freq entries*/
    U8              numCdma1xFreq;      /*!< number of CDMA1x freq entries*/
    U8              numGeranFreq;      /*!< number of Geran freq entries*/
   union
   {
      MsmLteNeighFreqAddModCfg      addNeighFreq[MAX_NUM_FREQ_CFG];
      MsmLteNeighFreqAddModCfg      dynNeighFreq;
      MsmLteNeighFreqDelCfg         delNeighFreq;
   }t;
} MsmLteNeighFreqCfg;

/** @brief This structure defines the Inter Freq Carrier Info for Sib5 **/
typedef struct msmLteSib5InterFreqCarrierInfo
{
   MsmActType       actType;        /*!< ADD/MOD/DEL */
   U16              maxCellEntries;     /*!< Max number of entries*/
   U16              numCells;       /*!< Number of Cells to be configured */
   Bool             enable;             /*!< Enables or disables InterFreq.Carrier*/
   U16              arfcnDl;            /*!< Indicates the ARFCN of this frequency carrier*/ 
   S8               qRxLevMin;          /*!< REQUIRED minimum received RSRP level on this E-UTRA frequency carrier*/
   U8               qOffsetFreq;        /*!< cell-specific offset */
   U8               cellReselPrio;      /*!< Absolute priority of the E-UTRAN serving carrier frequency*/
   S8               pMax;               /*!< Used to limit the allowed UE uplink transmission power on this carrier frequency*/
   U8               tReselEutra;        /*!< Cell re-selection timer for inter-frequency E-UTRA cell re-selection*/
   U8               tResetEutraSFMedium;/*!< Scaling factor for inter-frequency TreselectionEUTRA in Medium-mobility state*/
   U8               tResetEutraSFHigh;  /*!< Scaling factor for inter-frequency TreselectionEUTRA in High-mobility state*/
   U8               threshXHigh;        /*!< Scaling factor for inter-frequency TreselectionEUTRA in High-mobility state*/
   U8               threshXLow;
/* U8               allwdMeasBandwidth;
   U8               antennaPort1Pres;*/
} MsmLteSib5InterFreqCarrierInfo;

/** @brief This structure holds SIB6 Config group params **/
typedef struct msmLteCellSib6CfgGrp
{
   MsmActType           actType;
   U16                  tReselection;         /*!< Cell re-selection */
   U8                   tReselectionSfMedium; /*!< Medium Speed State Scale Factors */
   U8                   tReselectionSfHigh;   /*!< High Speed State Scale Factors */
}MsmLteCellSib6CfgGrp;

typedef struct msmRatPriorityList
{
   U8                numberOfRats;        /*!< The number of RATs provided in the 
                                            RAT priority list. */
   U8                rat[MSM_MAX_RATS];   /*!< Store the RATs in the order of 
                                                     highest priority. */
}MsmRatPriorityList;

/** @brief This structure protocol specific options from the OAM **/
typedef struct msmLteEnbProtoCfg
{
   MsmActType        actType;
 //  CmTptAddr         enbIpAddr;      /*!< eNB IP Address */
   U32               enbIpAddr;      /*!< eNB IP Address */
   U16               sctpPortNumS1;  /*!< The port number to be used in SCTP 
                                          association configuration for S1AP 
                                          with the MME*/
   U16               sctpPortNumX2;  /*!< The port number to be used in SCTP 
                                          association configuration for X2AP 
                                          with the neighbor eNB */
 
/* U32               bhLinkCapacity; */ /*!< Total backhaul link of Mhe eNB */
   U8                henbName[48];   /* EnodeB name is same as homeEnodeB name */

   U32               enbType;        /*!< EnodeB type */
   U32               enbId;          /*!< EnodeB Id 20/28 bits starting from LSB */
   U32               numPlmns;
   MsmRatPriorityList    plmnRatPriority[MSM_MAX_PLMN_IDS];  /* RAT Priority list for CSFB per PLMN */
   U32               inactvTmrVal;     /* UE Inactivity timer value */
   U32               endMrkTmrVal;     /* End Marker timer value */
   /* This is moved to ProcTmrCfg */
   U32               maxExpires;
   U8                pwsEtwsCmas;/* To enable ETWS or CMAS or disable both*/
   U8                avgFreeCpuRqd;/*AvgFree cpu required*/
   U8                isRedirectionSupport[MSM_MAX_PLMN_IDS];
} MsmLteEnbProtoCfg;

/** @brief This structure holds the Access barring configuration **/
typedef struct msmLteAcBarringCfg
{
//   TknPres     pres;            /*!< Optional hence presence needed */
   U8          acBarringFactor; /*!< Barring factor {0, 5, 10, 15, ... 95} */
   U16         acBarringTime;   /*!< Barring Time in seconds {4, 8, ... 512} */
   Bool        acBarringAC11;   /*!< Ac Barring for Access class 11 */
   Bool        acBarringAC12;   /*!< Ac Barring for Access class 12 */
   Bool        acBarringAC13;   /*!< Ac Barring for Access class 13 */
   Bool        acBarringAC14;   /*!< Ac Barring for Access class 14 */
   Bool        acBarringAC15;   /*!< Ac Barring for Access class 15 */
} MsmLteAcBarringCfg;

/** @brief This structure holds the Access barring information **/
typedef struct msmLteAcBarringInfo
{
   MsmActType   actType;
   Bool   acBarringForEmergency;/*!< TRUE or false */
   MsmLteAcBarringCfg   acBarringForMOSignal; /*!< Ac Barring for MO Signalling */
   MsmLteAcBarringCfg   acBarringForMOData;  /*!< AC Barring for MO Data */
   Bool   barringDataUsage;   /*!< TRUE then we shall enable barring
                                           for DATA */
   Bool   barringSignalUsage; /*!< TRUE then we shall enable
                                            barring for Signalling */
} MsmLteAcBarringInfo;

/** @brief This structure holds the Tpc Releted  configurable params **/
typedef struct msmCellSchdPwrCfg
{
   U16  pucchPwrFmt3Rnti;
   U16  pucchPwrFmt3Siz;
   U16  pucchPwrFmt3aRnti;
   U16  pucchPwrFmt3aSiz;
   U16  puschPwrFmt3Rnti;
   U16  puschPwrFmt3Siz;
   U16  puschPwrFmt3aRnti;
   U16  puschPwrFmt3aSiz;
   U8   pucchPwrFmt3aEnable;
   U8   puschPwrFmt3aEnable;
   U8   isUlGrpPwrCntrlPucchEnable;
   U8   isUlGrpPwrCntrlPuschEnable;
}MsmCellSchdPwrCfg;

/** @brief This structure holds the Rsys TPM configuration **/
typedef struct msmRsysTpmCfg
{
   S8  tgtRsrp;
   U8  attnConstIdx;
   S8  minRsrp;
   S8  maxRsrp;
}MsmRsysTpmCfg;


#ifdef LTE_QCOM
/** @brief This structure holds the list of LTE Bands supported **/
typedef struct msmLteBandsSupported
{
    U8                    numOfbands;   /*!< Number of bands that are part of BandsSupported list  */
    U16                   bandLst[MAX_BANDS_SUPPORTED]; 
}MsmLteBandsSupported;

/** @brief This structure holds the list of supported systems **/
typedef struct msmSupportedSystem
{
   U8                     numOfSupportedSys;
   U16                    supportedSystemList[MAX_SUPPORTED_SYSTEM]; 
}MsmSupportedSystem;

/** @brief This structure holds Qualcomm configuration parameters pushed 
  * by Radisys OAM **/
typedef struct msmRsysQcomCfgParam
{
   Bool adminState;
   MsmLteBandsSupported   bandSupportedList;
   MsmSupportedSystem   supportedSysList;
}MsmRsysQcomCfgParam;
#endif

/** @brief This structure holds the power offset params **/
typedef struct msmLtePowerOffsetParams
{
   MsmActType        actType;
   U8		     pschPowerOffCnt;                       /*!< Number of PschPowerOffset count */
   U8		     sschPowerOffCnt;                       /*!< Number of sschPowerOffset count */
   U8		     pbchPowerOffCnt;                       /*!< Number of PbchPowerOffset count */
   S8                pschPowerOffset[MAX_POWER_OFFSET_CNT]; /*!< power offset of PSCH w.r.t ReferenceSignalPower */ 
   S8                sschPowerOffset[MAX_POWER_OFFSET_CNT]; /*!< power offset of SSCH w.r.t ReferenceSignalPower */
   S8                pbchPowerOffset[MAX_POWER_OFFSET_CNT]; /*!< power offset of PBCH w.r.t ReferenceSignalPower */
} MsmLtePowerOffsetParams;

/** @brief This structure holds the FAP Control params **/
typedef struct msmLteFapControlParams
{
   MsmActType        actType;
   U8                connHandling;             /*!< FAP REM behavior */
   U8                maxUserCount;             /*!< Maximum Users */
   U8                maxCapacityRate;          /*!< Maximum FAP capacity Rate */
   Bool              handOverEmergencyAllowed; /*!< enable emergency handover or not */
   S8                fapCoverageTarget;        /*!< target value measured for range of the FAP’s DL coverage */
   U16               inServiceDeferredTimer;   /*!< timer value */
   U16               minRBAllocationPerUser;   /*!< minimum RB allocation per User */
   Bool              enableX2;                 /*!< enable X2 or not */ 
   U32               anrEpochPeriod;           /*!< ANR Epoch Period */
   U8                anrTricePeriod;           /*!< ANR Trice Period */
   U16               anrDrxInactvTmr;          /*!< ANR Drx Inactivity Timer*/
   U8                anrDrxOnDurTmr;            /*!< ANR Drx On duration Timer */
   U16               anrDrxLongCycle;          /*!< ANR Drx Long  cycle */
   U8                specialAccessClass;       /*!< access class */  
   /* Max age for the cell to be present in NRT table, in minutes */
   U8                anrCellValidAge;          
} MsmLteFapControlParams;

/** @brief This structure holds the PRB allocations **/
typedef struct msmLtePRBallocation
{
   MsmActType        actType;
   MsmLtePlmnId         plmns[MSM_MAX_PRB];
} MsmLtePRBallocation;

/** @brief This structure holds the FAP Access params **/
typedef struct msmLteFapAccessParams
{
   MsmActType        actType;
   U8                accessMode;               /*!< access mode of the FAP */
   U8                maxCSGMembers;            /*!< Maximum number of concurerent CSG */
   U8                maxNonCSGMembers;         /*!< Maximum number of concurrent non-CSG */
   U8                maxResourceNonCSGMembers; /*!< Maximum percentage of physical 
						resource that can be assigned to non-CSG members */
} MsmLteFapAccessParams;

/** @brief This structure holds the CSG Info for Sib4 **/
typedef struct msmLteSib4CsgInfo
{
   MsmActType        actType;
/*U8                 csgPciType;*/
  U16                csgStartPhyCellId; /*!< StartPCI */ 
  /* CSG_DEV */
  U8                 isCsgPciCfgd;
  U8                 csgPciRange;       /*!< Num of PCI */
  U8                 csgRange2Offset;   /*!< PCI Range2 Offset */
} MsmLteSib4CsgInfo;
 
typedef struct msmLteRrmTddParam
{
   MsmActType        actType;
   U8                srPrdcty;
   U8                cqiPrdcty;
   U8                numSrPerTti;
   U8                numCqiPerTti;
   U8                simulAckNackEnb;
} MsmLteRrmTddParam;
 
typedef struct msmLteTddParam
{
   MsmActType        actType;
   U8                sfconfigMaxEntries;
   U8                sfconfigNumEntries;
   U8                syncStratumId;
   U8                subframeAssignment;
   U8                specialSubFramePattern;
} MsmLteTddParam;

typedef struct msmEnodebIpAddr
{
  /**< OAM shall provide IP address in string notation */
  U32 enbIpAddr; 
/*  U8 enbIpAddr[MAX_IPADDR_LEN]; */
  U8 enbIpv6Addr[16];
  Bool ipv4pres;
  Bool ipv6pres;
}MsmEnodebIpAddr;

/** @brief This structure holds the X2AP layer related configuration **/
typedef struct msmLteX2ConfigParams
{
   MsmActType        actType;
   //   U16               tRelocationprep;      /**< Specifies the maximum time
   /* This is moved to ProcTmrCfg */
   //   U16               tX2RelocationOverall; /**< Specifies the maximum time
   /* This is moved to ProcTmrCfg */
   //   U16               timeToWait;           /**< Value for the time to wait IE.
   U32               x2TimeToWaitTimerVal;   /**< Value for the time to wait IE.
                                               The timeToWait is an IE which
                                               is filled in the ConfigUpdate
                                               Failure and Setup Failure 
                                               message,incase X2AP layer 
                                               generates these two.*/
} MsmLteX2ConfigParams;

/** @brief This structure defines the Sctp parameter configuration **/
typedef struct  msmLteSctpCfgParams
{
   MsmActType        actType;
   //  U32 		     mmeFailoverTimer;
   U32 		         heartBeatInterval;
   //  U32 		     numPathReTx;
   U32 		         rtoInitial;
   U32 		         rtoMin;
   U32 		         rtoMax;
   //  U32 		     connectInterval;
   U32               maxInitRetx;
   U32               maxPathRetx;
   U32               maxAssocRetx;
   U32               cookieLife;
   U32               localPort;
}MsmLteSctpCfgParams;

/** @briefThis structure defines the PCCH common configuration **/
typedef struct msmLtePCCHCfgCommon
{
   MsmActType        actType;
   U8 		         defaultPagingCycle;   /*!< Default paging cycle */
   U8                nB;                   /*!< nB as per spec 36.304 */
} MsmLtePCCHCfgCommon;

typedef struct msmLteCellUlSchdCfgGrp
{
   MsmActType        actType;
   U8                maxUePerUlSf;  /*!< Max Allocations in a given SF */
   /* this can be mapped to maxAllocPerUlSf */
   U8                maxUlUeNewTxPerTti; /*!< Max UEs to be considered for New Tx Alloc in UL */
   U32               pfsUlTptCoefficient; /*!< Uplink Throughput co-efficieint*/
   U32               pfsUlFairCoefficient;/*!< Uplink Fairness co-efficient */
   U32		         ulSchedulingAlg;
   U32               ulQciWgtList[MAX_NUM_QCI]; /*!< UL QCI Weights */
}MsmLteCellUlSchdCfgGrp;

typedef struct msmLteCellDlSchdCfgGrp
{
   MsmActType        actType;
   U8                maxUePerDlSf;     /*!< Max UE to be considered for DL
                                         scheduling in a TTI */
   U8                maxDlUeNewTxPerTti; /*!< Max UEs to be considered for New Tx Alloc in DL */
   U32               pfsDlTptCoefficient; /*!< Downlink Throughput co-efficieint*/
   U32               pfsDlFairCoefficient;/*!< Downlink Fairness co-efficient */
   U32		         dlSchedulingAlg;
   Bool              dlfsSchd; /*!< TRUE if DL allocation is Freq selective */
   U32               dlQciWgtList[MAX_NUM_QCI]; /*!< DL QCI Weights */
}MsmLteCellDlSchdCfgGrp;

/* This structure is used for storing the MSPD Platform specific parameters */
typedef struct msmLteMsCellCfgReq
{
   MsmActType        actType;
   U16               opMode;
   U32               period;
}MsmLteMsCellCfgReq;

/* This structure is used to enable debug */
typedef struct msmLteDbgParams
{
   MsmActType        actType;
   U8                hiDbg;    /* TUCL Debug  */
   U8                sbDbg;    /* SCTP Debug  */
   U8                szDbg;    /* S1AP Debug  */
   U8                egDbg;    /* EGTP Debug  */
   U8                wrDbg;    /* RRC APP Debug  */
   U8                nhDbg;    /* RRC Debug  */
   U8   	            kwDbg;    /* RLC/PDCP Debug  */
   U8                rgDbg;    /* MAC Debug  */
   U8                ysDbg;    /* CL Debug  */
   U8   	            smDbg;    /* SM Debug  */
   U8	               czDbg;    /* X2AP Debug  */
}MsmLteDbgParams;

/* typedef struct _smUlAllocInfoCb SmUlAllocInfoCb; */
typedef struct msmLteUlAllocInfoCbParams
{
   MsmActType        actType;
   U8  		         mcs;
   U16 		         numOfRb;
   U16 		         rbStart;
   //   Bool 	     enaLog;
   U16  	            logTime;
}MsmLteUlAllocInfoCbParams;

typedef struct msmGostLog
{
   U8 ipAddress[16];
   U32 portNumber;
   U8 rrcLogEnable;
}MsmGostLog;

typedef struct msmLteProcTimerCfg
{
   MsmActType        actType;
   U16               s1ResetTmr;
   //   U16               S1ResetRetryCount; 
   U16               x2ResetTmr;
   //   U16               X2ResetRetryCount;
/*  ccpu00135220 */
   U32               s1PrepTimerVal;
   U32               s1OvrAllTimerVal;
   U32				   s1RATPrepTimerVal;
   U32				   s1RATOvrAllTimerVal;
   U32               x2PrepTimerVal;
   U32               x2OvrAllTimerVal;
   U32		     s1HandoverCancelTmr;
/*  ccpu00135220 */
}MsmLteProcTimerCfg;

 /** @brief This structure holds CDMA band class structures **/
typedef struct msmCdmaBandClassCfg
{
   U8          bandClass;
   U8          cellReselectPrior;
   U8          threshXHigh;
   U8          threshXLow;
}MsmCdmaBandClassCfg;
 
typedef struct msmNeighBandClassCfg
{
   U8 bandType;
   union
   {
      MsmCdmaBandClassCfg bandClass1XRtt;
      MsmCdmaBandClassCfg bandClassHRPD;
   }t;
}MsmNeighBandClassCfg;
 
 /** @brief This structure holds CDMA structures **/
typedef struct msmCdmaBandClass
{
   MsmActType  actType;
   U8          maxBandClass;
   U8          numBandClass;
   union
   {
      MsmNeighBandClassCfg      bandClassAddCfg[MSM_MAX_NUM_BAND_CFG];   /*!< Addition */
      MsmNeighBandClassCfg      bandClassModCfg[MSM_MAX_NUM_BAND_CFG];   /*!< Modification */
      //MsmNeighBandClassCfgDel   bandClassDelCfg[MSM_MAX_NUM_BAND_CFG];   /*!< Deletion */
   }t;
}MsmCdmaBandClass;

typedef struct msmCdmaMobilityParam
{
   MsmActType   actType;
   Bool sidIncl;
   U16  sid;              /* System identifier */
   Bool nidIncl;
   U16  nid;              /* Network identifier */
   Bool multSidsIncl;
   Bool multSids;         /* Multiple SID storage indicator */
   Bool multNidsIncl;
   Bool multNids;         /* Multiple NID storage indicator */
   Bool regZoneIncl;
   U16  regZone;          /* Registration zone number */
   Bool totalZonesIncl;
   U8   totalZones;       /* Total number of registration zones */
   Bool zoneTimerIncl;
   U8   zoneTimer;        /* Zone timer */
   Bool pktZoneIdIncl;
   U8   pktZoneId;        /* Pakcet data services zone identifier */
   Bool pzIdHysParamIncl;
   Bool pzHystEnabled;    /* Packet zone hysteris enabled */
   Bool pzHystInfoIncl;   /* Pkt zone Hys Info Included indicator */
   Bool pzHystListLen;    /* Pkt zone Hys list length */
   U8   pzHystActTimer;   /* Pkt zone Hys Activation timer */
   U8   pzHystTimerMul;   /* Pkt zone Hys timer multiplier */
   U8   pzHystTimerExp;   /* Pkt zone hys timer exponent */
   Bool pRevIncl;
   U8   pRev;             /* Protocol revision level */
   Bool minPRevIncl;
   U8   minPRev;          /* Min protocol revision level */
   Bool negSlotCycIndxSupIncl;
   Bool negSlotCycIndxSup; /* Negative slot cycle index supported indicator */
   Bool encModeIncl;
   U8   encMode;          /* Message encryption mode */
   Bool encSupIncl;
   Bool encSup;           /* Encryption fields included */
   Bool sigEncSupIncl;
   U8   sigEncSup;        /* Signalling encryption supported indicator */
   Bool msgIntSupIncl;
   Bool msgIntSup;        /* Message integrity supported indicator */
   Bool sigIntSupINCLIncl;
   Bool sigIntSupINCL;    /* Signalling message integrity info included ind */
   Bool sigIntSupIncl;
   U8   sigIntSup;        /* Signalling integrity algo supported */
   Bool authIncl;
   U8   auth;             /* Authentication mode */
   Bool maxNumAltoSoIncl;
   U8   maxNumAltoSo;     /* Max numb of alternative service options */
   Bool useSyncIdIncl;
   Bool useSyncId;        /* Sync ID supported indicator */
   Bool msInitPosLocSupIndIncl;
   /* MS initiated position location determination supported ind */
   Bool msInitPosLocSupInd;
   Bool mobQosIncl;
   Bool mobQos;
   Bool bandClsInfoReqIncl;
   Bool bandClsInfoReq;   /* Band class info request indicator */
   Bool bandClsIncl;
   U8   bandCls;          /* Band class */
   Bool bypassRegIndIncl;
   U8   bypassRegInd;     /* Bypass powerup reg Ind */
   Bool altBandClsIncl;
   U8   altBandCls;       /* Alternate band class */
   Bool maxAddServInstIncl;
   U8   maxAddServInst;   /* Max num of additional serv ref ids allowed */
   Bool homeRegIncl;
   Bool homeReg;          /* Home reg indicator */
   Bool forSidRegIncl;
   Bool forSidReg;        /* SID roamer registration indicator */
   Bool forNidRegIncl;
   Bool forNidReg;        /* NID roamer registration indicator */
   Bool powerUpRegIncl;
   Bool powerUpReg;       /* Power up registration indicator */
   Bool powerDownRegIncl;
   Bool powerDownReg;     /* Power down registration indicator */
   Bool parmRegIncl;
   Bool parmReg;          /* Parameter change reg indicator */
   Bool regPrdIncl;
   U8   regPrd;           /* Registration period */
   Bool regDistIncl;
   U16  regDist;          /* Registration distance */
   Bool prefMsIdTypeIncl;
   U8   prefMsIdType;     /* Pref enhanced access channel MS ID Type */
   Bool extPrefMsIdTypeIncl;
   U8   extPrefMsIdType;  /* Ext Pref enhanced access channel MS ID Type */
   Bool meIdReqdIncl;
   Bool meIdReqd;         /* MEID Required indicator */
   Bool mccIncl;
   U16  mcc;              /* MCC */
   Bool imsi1112Incl;
   U8   imsi1112;         /* 11t and 12th digits of IMSI */
   Bool imsiTSupIncl;
   Bool imsiTSup;         /* IMSI_T support indicator */
   Bool recntMsgIndIncl;
   Bool recntMsgInd;      /* Reconnect message support indicator */
   Bool rerModeSupIncl;
   Bool rerModeSup;       /* Radio env reporting mode support indicator */
   Bool tzkModeSupIncl;
   Bool tzkModeSup;
   Bool tzkIdIncl;
   Bool tzkId;
   Bool pilRepIncl;
   Bool pilRep;           /* Pilot reporting indicator */
   Bool sdbSupIncl;
   Bool sdbSup;           /* Short data burst supported indicator */
   Bool autoFcsoAllIncl;
   Bool autoFcsoAll;      /* Autonomous fast call setup order allowed ind */
   Bool sdbInRcnmIncl;
   Bool sdbInRcnm;        /* Short data burst allowed in recon msg ind */
   Bool fpcFchIncl;
   U8   fpcFchInitSetptRC3;
   U8   fpcFchInitSetptRC4;
   U8   fpcFchInitSetptRC5;
   U8   fpcFchInitSetptRC11;
   U8   fpcFchInitSetptRC12;
   Bool tAddIncl;
   U8   tAdd;
   Bool pilotIncIncl;
   U8   pilotInc;
   Bool randIncl;
   U32  rand;             /* Random challenge */
   Bool lpSecIncl;
   U8   lpSec;
   Bool ltmOffIncl;
   U8   ltmOff;           /* Offset of local time from system time */
   Bool dayltIncl;
   Bool daylt;            /* Daylight savings time indicator */
   Bool gcsnaL2ActTimerIncl;
   U8   gcsnaL2ActTimer;
   Bool gcsnaSeqCtxtTimerIncl;
   U8   gcsnaSeqCtxtTimer;
   U8   reserved;
} MsmCdmaMobilityParam;

typedef struct msmBarringConfig1xRTTCfg
{
   MsmActType   actType;
   U8 ac_Barring0to9_r9;
   U8 ac_Barring10_r9;
   U8 ac_Barring11_r9;     
   U8 ac_Barring12_r9;        
   U8 ac_Barring13_r9;     
   U8 ac_Barring14_r9;        
   U8 ac_Barring15_r9;              
   U8 ac_BarringMsg_r9;             
   U8 ac_BarringReg_r9;             
   U8 ac_BarringEmg_r9;
} MsmBarringConfig1xRTTCfg;
 
typedef struct msm1xRttCsfbRegInfo
{
   U16  sid;
   U16  nid;
   Bool mulSID;
   Bool mulNID;
   Bool homeReg;
   Bool foreignSIDReg;
   Bool foreignNIDReg;
   Bool paramReg;
   Bool powerUpReg;
   U8   regPeriod;
   U16  regZone;
   U8   totalZone;
   U8   zoneTimer;
   Bool powerDownReg;
}Msm1xRttCsfbRegInfo;

typedef struct msmCellSib8CfgGrp
{
   MsmActType               actType;
   Bool                     cdmaEutraTimeSync;
   U8                       tReselection;
   Bool                     speedStatePres;
   U8                       sfMedium;
   U8                       sfHigh;
   Msm1xRttCsfbRegInfo      cdma1xrttRegParams;
   MsmBarringConfig1xRTTCfg cdma1xrttACBarringCfg;
}MsmCellSib8CfgGrp; 

typedef struct msmCsfbCfgParams
{
   /* RAT Priority for CSFB for the PLMN */
   MsmActType          actType;
   U8                  ecsfbSupport; /* Indicates whether E-CSFB is supported */
   U8                  csfbSupport; /* Indicates whether CSFB is supported */
   Bool                dualRxSup;
   U16                 marketId;
   U8                  switchNum;
   U8                  sectorNum;
   U8                  searchRate;
   Bool                csfbToUtranWithMeasEnabled; /* Indicates whether CSFB to UTRA supported with measurement */
   Bool                dualRxTxSup; /*Indicates DualRxTx support*/
}MsmCsfbCfgGrp;

typedef struct msmSib2Cfg
{
   MsmActType actType;
   U8         timeAlignmentCommon;
}MsmSib2Cfg;

typedef struct msmMacMainConfig
{
   U8         timeAlignmentTimerDedicated;
}MsmMacMainConifg;

typedef struct msmCnmCfg
{
   MsmActType actType;
   U8         cnmEnable;
}MsmCnmCfg;

typedef struct msmTenbStatsCfg
{
   U8 tenbSocketStats;
   U8 tenbFileStats;
   U8 tenbConsoleStats;
   U32 tenbStatsPeriodicity;
   U32 tendStatsSocketPort;
}MsmTenbStatsCfg;
/* This structure holds the SIB7 group information for GERAN */
typedef struct msmLteCellSib7CfgGrp
{
   U8                   tReselectionGERAN;           /*!<  cell reselection timer */
   Bool                 pres_SpeedStateScaleFactors;
   U8                   tReselectionGERANSFMedium;
   U8                   tReselectionGERANSFHigh;
}MsmLteCellSib7CfgGrp;

/* This structure holds the CSFB related info for GERAN */
typedef struct msmLteCsfbGeranCfg
{
   Bool     isGERANMeasAllowedForCCO;
   Bool     isCCOAllowed;
   U32      rrcDatCfmWaitTmr;
   U32      GERANMeasWaitTmr;
   U32      T304expWaitTmrVal;
} MsmLteCsfbGeranCfg;

 /* Main Structure */
/** @brief This structure holds all the Lte msm structs **/
typedef struct msmLteeNodeBparams
{
   MsmLteRimParams lteRimParams;
   /*SON Start*/
   MsmLteSonGenericParams      lteSonGenericParams;
   MsmLteEarfcnRssiCfg         lteEarfcnRssiCfg;
   /*SON end*/ 
   MsmLtePrachCfgCommon ltePrachCfgCommon;
   MsmLteeNodeBIPparams lteeNodeBIPparams;
   MsmLteRachCfgCommonParams lteRachCfgCommonParams;
   MsmLtePdschCfgCommon ltePdschCfgCommon;
   MsmLteULPwrCtrlCommon lteULPwrCtrlCommon;
   MsmLteDynCfiGrp       lteDynCfiParams;
   MsmLteULFrequencyInfo lteULFrequencyInfo;
   MsmLteCellSib3CfgGrp lteCellSib3CfgGrp;
   MsmLteCellSib1CfgGrp lteCellSib1CfgGrp; 
   MsmLteDrxCfgParams lteDrxCfgParams;
   MsmLteCellMeasCfgGrp lteCellMeasCfgGrp;
   MsmLteAddCellCfg lteAddCellCfg;
   MsmLteSmCellCfg lteSmCellCfg;
   MsmLteFreqSyncParams lteFreqSyncParams;
   MsmLteCellMibCfgGrp lteCellMibCfgGrp;
   MsmLteMAC_SCH_Config lteMAC_SCH_Config;
   MsmLteAntenna_Common_Config lteAntenna_Common_Config;
   MsmLtePdschCfgDedicated ltePdschCfgDedicated;
   MsmLtePuschBasicCfgCommon ltePuschBasicCfgCommon;
   MsmLtePuschRefSignalCfg ltePuschRefSignalCfg;
   MsmLtePucchCfgCommon ltePucchCfgCommon;
   MsmLteSRSCfgCommon lteSRSCfgCommon;
   MsmLteRabPolicyCfgGrp lteRabPolicyCfgGrp;
   MsmLteUeTimerConstants lteUeTimerConstants;
   MsmLteCellEAIDCfgGrp lteCellEAIDCfgGrp;
   MsmLteCellSib9CfgGrp lteCellSib9CfgGrp;
   MsmLteCQIPeriodicReportCfg lteCQIPeriodicReportCfg;
   MsmLteNeighFreqCfg lteNeighFreqCfg;
   MsmNeighCellCfg lteNeighCellCfg;
   MsmCnmCfg lteCnmCfg;
   MsmTenbStatsCfg tenbStatsCfg;
   MsmLteSib5InterFreqCarrierInfo lteSib5InterFreqCarrierInfo;
   MsmLteCellSib6CfgGrp lteCellSib6CfgGrp;
   MsmLteCellSib7CfgGrp     sib7CfgGrp;
   MsmLteCsfbGeranCfg       csfbToGeranCfg;
   MsmLteEnbProtoCfg lteEnbProtoCfg;
   MsmLteAcBarringInfo lteAcBarringInfo;
   MsmEnodebIpAddr        lteEnodebIpAddr;
   MsmLtePowerOffsetParams ltePowerOffsetParams;
   MsmLteFapControlParams lteFapControlParams;
   MsmLtePRBallocation ltePRBallocation;
   MsmLteFapAccessParams lteFapAccessParams;
   MsmLteSib4CsgInfo lteSib4CsgInfo;
   MsmLteX2ConfigParams lteX2ConfigParams;
   MsmLteSctpCfgParams lteSctpCfgParams;
   MsmLtePCCHCfgCommon ltePCCHCfgCommon;
   MsmLteCellUlSchdCfgGrp lteCellUlSchdCfgGrp;
   MsmLteCellDlSchdCfgGrp lteCellDlSchdCfgGrp;
   MsmLteMsCellCfgReq lteMsCellCfgReq; 
   MsmLteDbgParams lteDbgParams;
   MsmGostLog lteGostLog;
   MsmLteUlAllocInfoCbParams lteUlAllocInfoCbParams;
   MsmLteProcTimerCfg lteProcTimerCfg;
   MsmCdmaBandClass   cdmaBandClassParams;
   MsmCdmaMobilityParam cdmaMobilityParam;
   MsmBarringConfig1xRTTCfg barringConfig1xRTTCfg;
   MsmCellSib8CfgGrp        cellSib8CfgGrp;
   MsmCsfbCfgGrp            csfbCfgGrp;
   MsmSib2Cfg               sib2Cfg;
   MsmMacMainConifg         macMainConfig;
   MsmCellSchdPwrCfg        ltecellSchdPwrCfg;
   MsmLteRrmTddParam lteRrmTddParam;
   MsmLteTddParam lteTddParam;
   /* SPS changes starts */
   MsmSpsCellConfigData     stSpsCellCfg;
   /* SPS changes ends */
   /* TM4 chanages */
   MsmTMCellConfig  lteTransModeCellCfg;
   MsmEnbLogLvlData lteLogLvlCfg;
   MsmRsysTpmCfg    tpmCfg;
#ifdef LTE_QCOM
   MsmRsysQcomCfgParam  lteRsysQcomCfgParam;
#endif
}MsmLteeNodeBparams; 


/*Add WCDMA related parameters after this*/
/** @brief This structure holds the performance mgmt params **/
typedef struct msmPerfMgmtParameters
{
   MsmActType        actType;
   U8   collectionMethodForPM[MAX_NUM_CHARS];/*!< collection method for PM All or L3 only */
   Bool l2PMCollectionEnable;               /*!< enable l2PMCollection or not */
}MsmPerfMgmtParameters;

/** @brief This structure holds WCDMA structures **/
typedef struct msmWcdmaNodeBparams
{
   MsmPerfMgmtParameters perfMgmtParameters;
}MsmWcdmaNodeBparams;

typedef struct msmMmeDynUpdate
{
   U8                 mmeNewIpPresent;
   U32                mmeNewIp;
}MsmMmeDynUpdate;

typedef struct msmMmeDynUpdateParams
{
   U8                 mmeIPpresent;
   U8                 newIPlist;
   U8                 numDelMmeIds;
   U8                 deleteMme;
   U8                 newMmeId;
}MsmMmeDynUpdateParams;

/** @brief This structure contains the parameters related to frequent alarms **/
typedef struct msmFrequentFaultMgmtParameters
{
   MsmActType      actType;
   U8              maxS1ErrIndCnt;      /*!< Maximum count for S1 Error Indication */
   U8              maxS1ResetCnt;       /*!< Maximum count for S1 Reset */
   U8              maxX2ErrIndCnt;      /*!< Maximum count for X2 Error Indication */
   U8              maxX2ResetCnt;       /*!< Maximum count for X2 Reset */
   U32             minS1ErrIndTime;     /*!< Minimum Time Interval for S1 Error Indication */
   U32             minS1ResetTime;      /*!< Minimum Time interval for S1 Reset */
   U32             minX2ErrIndTime;     /*!< Minimum Time Interval for X2 Error Indication */
   U32             minX2ResetTime;      /*!< Minimum Time interval for X2 Reset */
}MsmFrequentFaultMgmtParameters;

typedef struct msmFaultMgmtParams
{
   MsmFrequentFaultMgmtParameters frequentFaultMgmtParameters;
}MsmFaultMgmtParams;

/** @brief This structure contains the parameters containing additional information for alarms from SON**/
typedef struct pCIConflictCollision
{
	U16 pci;				/*!< Physical Cell ID */
	U8  actionType;			/*!< Alarm recovery recommendation - Manual or Automatic */
	U32 cellId;				/*!< Cell Identifier */
}PCIConflictCollision;

/** @brief This structure contains the parameters containing dynamic additional information for alarms**/
typedef struct msmUpdateAlarms
{
   U32 alarmId;						/*!< Alarm Identifier */
   SmAlarmSeverity severity;		/*!< Alarm Severity */
   U8 causeType;					/*!< Probable Cause  */
   U8 causeValue;					/*!< Enum value to identify specific problem */
   U8 isAdditionalInfo;				/*!< Flag that indicates presence of Additional Information */
   union
   	{
   	  PCIConflictCollision info;	/*!< Alarm Details due to PCI collision or confusion from SON */
   	}t;
      
} MsmUpdateAlarms;


/** @brief Enum defines the msm subscriptions **/
typedef enum msmAlarmId
{
   LTE_CONFIGURATION_PLAN_ERROR = 11100,
   LTE_CONFIGURATION_PLAN_INCONSISTENCY,
   LTE_FREQUENCY_SYNCHRONISATION_FAILURE,
   LTE_NTP_SERVER_NOT_RESPONDING,
   LTE_OSCILLATOR_STABILITY_PROBLEM,
   LTE_FAP_TEMPERATURE_OVER_THRESHOLD,
   PCI_CONFLICT,
   LTE_LOW_QUALITY_CHANNEL,
   LTE_NETWORK_JITTER,
   S1_SETUP_FAIL,
   X2_SETUP_FAIL,
   LTE_MEMORY_ALLOCATION_FAIL,
   SCTP_LINK_FAIL,
   TNL_SETUP_FAIL,
   S1_ERROR_IND,
   S1_RESET,
   X2_ERROR_IND,
   X2_RESET,
   /* Static Configuration Alarm */
   SOCKET_CFG_FAIL,
   SCTP_CFG_FAIL,
   S1AP_CFG_FAIL,
   EGTP_CFG_FAIL,
   X2AP_CFG_FAIL,
   RRM_CFG_FAIL,
   APP_CFG_FAIL,
   RRC_CFG_FAIL,
   PDCPUL_CFG_FAIL,
   PDCPDL_CFG_FAIL,
   RLCUL_CFG_FAIL,
   RLCDL_CFG_FAIL,
   MAC_CFG_FAIL,
   CL_CFG_FAIL,
   CELLUP_CFG_FAIL,

   /* Dynamic Configuration Alarm */
   ADMIN_STATE_CHANGE_FAIL,
   GENERIC_PRAMS_CFG_FAIL,
   MME_IP_PRAMS_CFG_FAIL,
   CELL_CONFIG_PRAMS_FAIL,
   ENBIP_CFG_FAIL,
   MIB_CFG_FAIL,
   PRACH_CFG_FAIL,
   RACH_CFG_FAIL,
   PDSCH_CFG_FAIL,
   SRS_CFG_FAIL,
   PUSCH_CFG_FAIL,
   ULPOWER_CFG_FAIL,
   ULFREQ_CFG_FAIL,
   PUCCH_CFG_FAIL,
   UETMRCONS_CFG_FAIL,
   SIB1_CFG_FAIL,
   SIB2_CFG_FAIL,
   SIB3_CFG_FAIL,
   SIB4_CFG_FAIL,
   SIB6_CFG_FAIL,
   SIB9_CFG_FAIL,
   ANR_CFG_FAIL,
   MEAS_CFG_FAIL,
   EAID_CFG_FAIL,
   SIBSCHED_CFG_FAIL,
   RABPOLICY_CFG_FAIL,
   TIMER_CFG_FAIL,
   RRM_CELL_CFG_FAIL,
   ULSCHD_CFG_FAIL,
   DLSCHD_CFG_FAIL,
   NEIGH_CFG_FAIL,
   NEIGH_CELL_CFG_FAIL,
   NEIGH_FREQ_CFG_FAIL,
   NEIGH_BAND_CLASS_CFG_FAIL,
   ENB_PROTO_CFG_FAIL,
   UTRANEIFREQ_CFG_FAIL,
   EUTRANEIFREQ_CFG_FAIL,
   UTRANEICELL_CFG_FAIL,
   EUTRANEICELL_CFG_FAIL,
   UTRANEILIST_CFG_FAIL,
   EUTRANEILIST_CFG_FAIL,
   RABSRB1_CFG_FAIL,
   RABSRB2_CFG_FAIL,
   EVNT_NEIGH_ENB_CFG_FAIL,
   SMCELL_CFG_FAIL,
   PROCTMR_CFG_FAIL,
   CFG_FAIL_FROM_SON,
   PCI_COLLISION,
   PCI_CONFUSION
} MsmAlarmId;


/** SM Static configuration Timer **/
#define SM_TMR_STATIC_CFG_TMR          1
#define SM_TMR_STATIC_CFG_TMR_VAL      5000

/** SM Dynamic configuration Timer **/
#define SM_TMR_DYN_CFG_TMR          2
#define SM_TMR_DYN_CFG_TMR_VAL      5000


  S16 msmSetPrachConfigCommonParameters(const MsmLtePrachCfgCommon *ltePrachCfgCommon);
  S16 msmSetRimParameters(const MsmLteRimParams *lteRimParams);
  S16 msmSetEnodebIpParameters(const MsmLteeNodeBIPparams *lteEnodebIpAddr);
  S16 msmSetRachConfigCommonParameters(const MsmLteRachCfgCommonParams *lteRachCfgCommonParams);
  S16 msmSetPdschConfigCommonParameters(const MsmLtePdschCfgCommon *ltePdschCfgCommon);
  S16 msmSetUlPowerControlCommonParameters(const MsmLteULPwrCtrlCommon *lteULPwrCtrlCommon);
  S16 msmSetUlFrequencyInfoParameters(const MsmLteULFrequencyInfo *lteULFrequencyInfo);
  S16 msmSetCell_Sib3ConfigGroupParameters(const MsmLteCellSib3CfgGrp *lteCellSib3CfgGrp);
  S16 msmSetCellSib1ConfigGroupParameters(const MsmLteCellSib1CfgGrp *lteCellSib1CfgGrp, U32 isPlmnCfgd);
  S16 msmSetDrxConfigParameters(const MsmLteDrxCfgParams *lteDrxCfgParams);
  S16 msmSetCellMeasConfigParameters(const MsmLteCellMeasCfgGrp *lteCellMeasCfgGrp);
  S16 msmSetAddCellConfigParameters(const MsmLteAddCellCfg *lteAddCellCfg);
  S16 msmSetFreqSyncParameters(const MsmLteFreqSyncParams *lteFreqSyncParams);
  S16 msmSetSmCellConfigParameters(const MsmLteSmCellCfg *lteSmCellCfg);
  S16 msmSetCellMibConfigParameters(const MsmLteCellMibCfgGrp *lteCellMibCfgGrp);
  S16 msmSetMacSchConfigParameters(const MsmLteMAC_SCH_Config *lteMAC_SCH_Config);
  S16 msmSetAntennaCommonConfigParameters(const MsmLteAntenna_Common_Config *lteAntenna_Common_Config);
  S16 msmSetPdschConfigDedicatedParameters(const MsmLtePdschCfgDedicated *ltePdschCfgDedicated);
  S16 msmSetPuschBasicCfgCommonParameters(const MsmLtePuschBasicCfgCommon *ltePuschBasicCfgCommon);
  S16 msmSetPuschRefSignalCfgParameters(const MsmLtePuschRefSignalCfg *ltePuschRefSignalCfg);
  S16 msmSetPucchCfgCommonParameters(const MsmLtePucchCfgCommon *ltePucchCfgCommon);
  S16 msmSetSrsCfgCommonParameters(const MsmLteSRSCfgCommon *lteSRSCfgCommon);
  S16 msmSetRabPolicyCfgGrpParameters(const MsmLteRabPolicyCfgGrp *lteRabPolicyCfgGrp);
  S16 msmSetUeTimerConstantsParameters(const MsmLteUeTimerConstants *lteUeTimerConstants);
  S16 msmSetCellEaidCfgGrpParameters(const MsmLteCellEAIDCfgGrp *lteCellEAIDCfgGrp);
  S16 msmSetCellSib9CfgGrpParameters(const MsmLteCellSib9CfgGrp *lteCellSib9CfgGrp);
  S16 msmSetCqiPeriodicReportCfgParameters(const MsmLteCQIPeriodicReportCfg *lteCQIPeriodicReportCfg);

  S16 msmGetNeighFreqEutraCfgParameters(MsmLteNeighFreqCfg *lteNeighFreqCfg);
  S16 msmGetNeighFreqUtraCfgParameters(MsmLteNeighFreqCfg *lteNeighFreqCfg);
  S16 msmSetNeighFreqEutraCfgParameters(const MsmLteNeighFreqCfg *lteNeighFreqCfg);
  S16 msmSetNeighFreqUtraCfgParameters(const MsmLteNeighFreqCfg *lteNeighFreqCfg);
  S16 msmSetSib5InterFreqCarrierInfoParameters(const MsmLteSib5InterFreqCarrierInfo *lteSib5InterFreqCarrierInfo);
  S16 msmSetCellSib6ConfigGroupParameters(const MsmLteCellSib6CfgGrp *lteCellSib6CfgGrp);
  S16 msmSetEnbProtoCfgParameters(const MsmLteEnbProtoCfg *lteEnbProtoCfg);
  S16 msmSetAcBarringInfoParameters(const MsmLteAcBarringInfo *lteAcBarringInfo);
  S16 msmSetSelfConfigParameters(const MsmLteSonGenericParams *lteSelfConfigParams);
  S16 msmSetPowerOffsetParameters(const MsmLtePowerOffsetParams *ltePowerOffsetParams);
  S16 msmSetDynCfiParameters(const MsmLteDynCfiGrp *lteDynCfiParams);
  S16 msmSetFapControlParameters(const MsmLteFapControlParams *lteFapControlParams);
  S16 msmSetPrbAllocationParameters(const MsmLtePRBallocation *ltePRBallocation);
  S16 msmSetSib4CsgInfoParameters(const MsmLteSib4CsgInfo *lteSib4CsgInfo);
  S16 msmSetRrmTddParameters(const MsmLteRrmTddParam *lteRrmTddParam);
  S16 msmSetX2ConfigParameters(const MsmLteX2ConfigParams *lteX2ConfigParams);
  S16 msmSetSctpConfigParameters(const MsmLteSctpCfgParams *lteSctpCfgParams);
  S16 msmSetPcchConfigCommonParameters(const MsmLtePCCHCfgCommon *ltePCCHCfgCommon);
  S16 msmSetCellUlSchdConfigParameters(const MsmLteCellUlSchdCfgGrp *lteCellUlSchdCfgGrp);
  S16 msmSetCellDlSchdConfigParameters(const MsmLteCellDlSchdCfgGrp *lteCellDlSchdCfgGrp);
  S16 msmSetMsCellCfgReqParameters(const MsmLteMsCellCfgReq *lteMsCellCfgReq);
  S16 msmSetDbgParameters(const MsmLteDbgParams *lteDbgParams);
  S16 msmSetGostLog(MsmGostLog *lteGostLog);
  S16 msmSetUlAllocInfoCbParameters(const MsmLteUlAllocInfoCbParams *lteUlAllocInfoCbParams);
  S16 msmSetProcTimerCfgParameters(const MsmLteProcTimerCfg *lteProcTimerCfg);
  S16 msmSetFapAccessParameters(const MsmLteFapAccessParams *lteFapAccessParams);
  S16  msmGetFapAccessParameters( MsmLteFapAccessParams *lteFapAccessParams);
  S16 msmSetFactorySmCellParameters(const MsmLteSmCellCfg *lteSmCellCfg);
  S16 msmGetFactoryFapAceessParameters(MsmLteFapAccessParams *lteFapAccessParams);
  S16 msmGetFactorySmCellParameters(MsmLteSmCellCfg *lteSmCellCfg);
  S16 msmSetEnbIpAddress(const MsmEnodebIpAddr *lteEnbIpAddr);
  S16 msmSetPmParameters(const MsmPerfMgmtParameters *perfMgmtParameters);
  S16 msmSetFrequentFaultParameters(const MsmFrequentFaultMgmtParameters *frequentFaultMgmtParameters);
  S16 msmSetCdmaBandClassParameters(const MsmCdmaBandClass *cdmaBandClass);
  S16 msmSetCdmaMobilityParameters(const MsmCdmaMobilityParam *cdmaMobilityParam);
  S16 msmSetBarringCfg1xRtt(const MsmBarringConfig1xRTTCfg *barringConfig1xRTTCfg);
  S16 msmSetCdma1XRttNghCell(const MsmNeighCellCfg *lteNeighCellCfg);
  S16 msmSetSib8Parameters(const MsmCellSib8CfgGrp *cellSib8CfgGrp);
  S16 msmSetCsfbCfgParameters(const MsmCsfbCfgGrp *csfbCfgGrp);
  S16 msmSetSpsParameters(const MsmSpsCellConfigData *stSpsCellCfg);
  /* TM4 changes*/
  S16 msmSetTMParameters(const MsmTMCellConfig *lteTransModeCellCfg);
  S16 msmSetSib2Cfg(MsmSib2Cfg *sib2Cfg);
  S16 msmSetMacMainConfig(MsmMacMainConifg *macMainConfig);
 S16 msmSetSchdPwrCfgParameters(MsmCellSchdPwrCfg *ltecellSchdPwrCfg);
  S16 msmSetLogLvlParameter(const MsmEnbLogLvlData *lteLogLvlCfg);
  S16 msmSetRsysTpmCfgParameters(MsmRsysTpmCfg *tpmCfg);
  
  S16 msmGetPrachConfigCommonParameters(MsmLtePrachCfgCommon *ltePrachCfgCommon);
  S16 msmGetRimParameters(MsmLteRimParams* lteRimParams);
   S16 msmSetEarfcnRssiParameters(const MsmLteEarfcnRssiCfg *lteEarfcnRssiCfg);
  S16 msmGetEarfcnRssiParameters(MsmLteEarfcnRssiCfg *lteEarfcnRssiCfg);
  S16 msmGetEnodebIpParameters(MsmLteeNodeBIPparams *lteeNodeBIPparams);
  S16 msmGetRachConfigCommonParameters(MsmLteRachCfgCommonParams *lteRachCfgCommonParams);
  S16 msmGetPdschConfigCommonParameters(MsmLtePdschCfgCommon *ltePdschCfgCommon);
  S16 msmGetUlPowerControlCommonParameters(MsmLteULPwrCtrlCommon *lteULPwrCtrlCommon);
  S16 msmGetUlFrequencyInfoParameters(MsmLteULFrequencyInfo *lteULFrequencyInfo);
  S16 msmGetCell_Sib3ConfigGroupParameters(MsmLteCellSib3CfgGrp *lteCellSib3CfgGrp);
  S16 msmGetCellSib1ConfigGroupParameters(MsmLteCellSib1CfgGrp *lteCellSib1CfgGrp);
  S16 msmGetDrxConfigParameters(MsmLteDrxCfgParams *lteDrxCfgParams);
  S16 msmGetCellMeasConfigParameters(MsmLteCellMeasCfgGrp *lteCellMeasCfgGrp);
  S16 msmGetAddCellConfigParameters(MsmLteAddCellCfg *lteAddCellCfg);
  S16 msmGetFreqSyncParameters(const MsmLteFreqSyncParams *lteFreqSyncParams);
  S16 msmGetSmCellConfigParameters(MsmLteSmCellCfg *lteSmCellCfg);
  S16 msmGetCellMibConfigParameters(MsmLteCellMibCfgGrp *lteCellMibCfgGrp);
  S16 msmGetMacSchConfigParameters(MsmLteMAC_SCH_Config *lteMAC_SCH_Config);
  S16 msmGetAntennaCommonConfigParameters(MsmLteAntenna_Common_Config *lteAntenna_Common_Config);
  S16 msmGetPdschConfigDedicatedParameters(MsmLtePdschCfgDedicated *ltePdschCfgDedicated);
  S16 msmGetPuschBasicCfgCommonParameters(MsmLtePuschBasicCfgCommon *ltePuschBasicCfgCommon);
  S16 msmGetPuschRefSignalCfgParameters(MsmLtePuschRefSignalCfg *ltePuschRefSignalCfg);
  S16 msmGetPucchCfgCommonParameters(MsmLtePucchCfgCommon *ltePucchCfgCommon);
  S16 msmGetSrsCfgCommonParameters(MsmLteSRSCfgCommon *lteSRSCfgCommon);
  S16 msmGetRabPolicyCfgGrpParameters(MsmLteRabPolicyCfgGrp *lteRabPolicyCfgGrp);
  S16 msmGetUeTimerConstantsParameters(MsmLteUeTimerConstants *lteUeTimerConstants);
  S16 msmGetCellEaidCfgGrpParameters(MsmLteCellEAIDCfgGrp *lteCellEAIDCfgGrp);
  S16 msmGetCellSib9CfgGrpParameters(MsmLteCellSib9CfgGrp *lteCellSib9CfgGrp);
  S16 msmGetCqiPeriodicReportCfgParameters(MsmLteCQIPeriodicReportCfg *lteCQIPeriodicReportCfg);
  S16 msmGetSib5InterFreqCarrierInfoParameters(MsmLteSib5InterFreqCarrierInfo *lteSib5InterFreqCarrierInfo);
  S16 msmGetCellSib6ConfigGroupParameters(MsmLteCellSib6CfgGrp *lteCellSib6CfgGrp);
  S16 msmGetEnbProtoCfgParameters(MsmLteEnbProtoCfg *lteEnbProtoCfg);
  S16 msmGetAcBarringInfoParameters(MsmLteAcBarringInfo *lteAcBarringInfo);
  S16 msmGetSelfConfigParameters(MsmLteSonGenericParams *lteSelfConfigParams);
  S16 msmGetPowerOffsetParameters(MsmLtePowerOffsetParams *ltePowerOffsetParams);
  S16 msmGetFapControlParameters(MsmLteFapControlParams *lteFapControlParams);
  S16 msmGetPrbAllocationParameters(MsmLtePRBallocation *ltePRBallocation);
  S16 msmGetSib4CsgInfoParameters(MsmLteSib4CsgInfo *lteSib4CsgInfo);
  S16 msmGetRrmTddParameters(MsmLteRrmTddParam *lteRrmTddParam);
  S16 msmGetX2ConfigParameters(MsmLteX2ConfigParams *lteX2ConfigParams);
  S16 msmGetSctpConfigParameters(MsmLteSctpCfgParams *lteSctpCfgParams);
  S16 msmGetPcchConfigCommonParameters(MsmLtePCCHCfgCommon *ltePCCHCfgCommon);
  S16 msmGetCellUlSchdConfigParameters(MsmLteCellUlSchdCfgGrp *lteCellUlSchdCfgGrp);
  S16 msmGetCellDlSchdConfigParameters(MsmLteCellDlSchdCfgGrp *lteCellDlSchdCfgGrp);
  S16 msmGetMsCellCfgReqParameters(MsmLteMsCellCfgReq *lteMsCellCfgReq);
  S16 msmGetDbgParameters(MsmLteDbgParams *lteDbgParams);
  S16 msmGetUlAllocInfoCbParameters(MsmLteUlAllocInfoCbParams *lteUlAllocInfoCbParams);
  S16 msmGetProcTimerCfgParameters(MsmLteProcTimerCfg *lteProcTimerCfg);
  S16 msmGetEnbIpAddress(MsmEnodebIpAddr *lteEnbIpAddr);
  S16 msmGetPmParameters(MsmPerfMgmtParameters *perfMgmtParameters);
  S16 msmGetFrequentFaultParameters(MsmFrequentFaultMgmtParameters *frequentFaultMgmtParameters);
  S16 msmGetCdmaBandClassParameters(MsmCdmaBandClass *cdmaBandClass);
  S16 msmGetCdmaMobilityParameters(MsmCdmaMobilityParam *cdmaMobilityParam);
  S16 msmGetBarringCfg1xRtt(MsmBarringConfig1xRTTCfg *barringConfig1xRTTCfg);
  S16 msmGetCdma1XRttNghCell(MsmNeighCellCfg *lteNeighCellCfg);
  S16 msmGetSib8Parameters(MsmCellSib8CfgGrp *cellSib8CfgGrp);
  S16 msmGetCsfbCfgParameters(MsmCsfbCfgGrp *csfbCfgGrp);
  S16 msmGetSpsParameters(MsmSpsCellConfigData *stSpsCellCfg);
  /* TM4 changes*/
  S16 msmGetTMParameters(MsmTMCellConfig *lteTransModeCellCfg);
  S16 msmGetTddParameters(MsmLteTddParam *lteTddParam);
  S16 msmGetLogLvlParameter(MsmEnbLogLvlData *lteLogLvlCfg);
  S16 msmGetsib7CfgGrpParameters(MsmLteCellSib7CfgGrp *sib7CfgGrp);
  S16 msmGetcsfbGeranCfgParameters(MsmLteCsfbGeranCfg *csfbToGeranCfg);
  S16 msmGetNeighFreqCfgGeranParameters(MsmLteNeighFreqCfg *lteNeighFreqCfg);
  S16 msmGetNeighCellCfgGeranParameters(MsmNeighCellCfg *lteNeighCellCfg);
  S16 msmSetsib7CfgGrpParameters(MsmLteCellSib7CfgGrp *sib7CfgGrp);
  S16 msmSetcsfbGeranCfgParameters(MsmLteCsfbGeranCfg *csfbToGeranCfg);
  S16 msmSetNeighFreqCfgGeranParameters(const MsmLteNeighFreqCfg *lteNeighFreqCfg);
  S16 msmSetNeighCellCfgGeranParameters(const MsmNeighCellCfg *lteNeighCellCfg);
  S16 msmSetTddParameters(MsmLteTddParam *lteTddParam);
  S16 msmSetNeighborListInUseEutraParameters(const MsmNeighCellCfg *lteNeighCellCfg);
  S16 msmSetNeighborListInUseUtraParameters(const MsmNeighCellCfg *lteNeighCellCfg);
  S16 msmSetEnodebIpAddr(const MsmEnodebIpAddr *lteEnodebIpAddr);
  S16 msmSetCellCnmCfgGrpParameters(MsmCnmCfg *lteCnmCfg);
  S16 msmGetNeighborListInUseUtraParameters(MsmNeighCellCfg *lteNeighCellCfg);
  S16 msmGetNeighborListInUseEutraParameters(MsmNeighCellCfg *lteNeighCellCfg);
  S16 msmSetTdsCdmaUtranTddFreqParameters(const MsmLteNeighFreqCfg *lteNeighFreqCfg);
  S16 msmSetTdsCdmaUtranTddCellParameters(const MsmNeighCellCfg *lteNeighCellCfg);
  S16 msmGetTdsCdmaUtranTddFreqParameters(MsmLteNeighFreqCfg *lteNeighFreqCfg);
  S16 msmGetTdsCdmaUtranTddCellParameters(MsmNeighCellCfg *lteNeighCellCfg);
 S16 msmSetRabPolicyCfgGrpSrb1Parameters(const MsmLteRabPolicyCfgGrp *lteRabPolicyCfgGrp);
 S16 msmSetRabPolicyCfgGrpSrb2Parameters(const MsmLteRabPolicyCfgGrp *lteRabPolicyCfgGrp);
 S16 msmSetRabPolicyCfgGrpDrbParameters(const MsmLteRabPolicyCfgGrp *lteRabPolicyCfgGrp);
 S16 msmGetRabPolicyCfgGrpSrb1Parameters(MsmLteRabPolicyCfgGrp *lteRabPolicyCfgGrp);
 S16 msmGetRabPolicyCfgGrpSrb2Parameters(MsmLteRabPolicyCfgGrp *lteRabPolicyCfgGrp);
 S16 msmGetRabPolicyCfgGrpDrbParameters(MsmLteRabPolicyCfgGrp *lteRabPolicyCfgGrp);
 S16 msmGetFactoryFapAccessParameters(MsmLteFapAccessParams *lteFapAccessParams);
 S16 msmGetEnodebIpAddr(MsmEnodebIpAddr *lteEnodebIpAddr);
 S16 msmGetSchdPwrCfgParameters(MsmCellSchdPwrCfg *ltecellSchdPwrCfg);
 S16 msmSetRimParameters(const MsmLteRimParams* lteRimParams);
 S16 msmGetRimParameters(MsmLteRimParams* lteRimParams);
 S16 msmSetTenbStatsCfgGrpParameters(MsmTenbStatsCfg *tenbStatsCfg);
 S16 msmSetFapAcceessParameters(const MsmLteFapAccessParams *lteFapAccessParams);
#ifdef LTE_QCOM
 S16 msmSetRsysQcomCfgParam(const MsmRsysQcomCfgParam  *lteRsysQcomCfgParam);
#endif

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /*End of __WR_MSM_COMMON_H__*/

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
