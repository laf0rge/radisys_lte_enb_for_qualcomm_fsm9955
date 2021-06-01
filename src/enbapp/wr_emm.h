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
               

     File:     wr_emm.h

     Sid:      $SID$ 

     Prg:      Sriky 

*********************************************************************21*/



#ifndef __WR_EMM_H__
#define __WR_EMM_H__

#include "wr_cmn.h"
#include "wr_emm_pws.h"
#include "wr_emm_anr.h"
/* LTE_ADV starts */
#include "wr_emm_lteadv.h"
/* LTE_ADV ends */
#include "wr_emm_rim_app.h"

#ifdef __cplusplus
extern "C" {
#endif

#define WR_GET_SIB_PERIOD(_cellCb, _sibId, _sibPeriod) { \
   U8         _sibIdx;                                   \
   for(_sibIdx = 0; _sibIdx < WR_MAX_SIBS; _sibIdx++)    \
   {                                                     \
      if (_sibId == _cellCb->sibsCfg[_sibIdx].sibId)     \
      {                                                  \
         _sibPeriod = _cellCb->sibsCfg[_sibIdx].period;  \
      }                                                  \
   }                                                     \
}                                                        \

#define WR_GET_PAGING_CYCLE(enmValue, T) {               \
   if (enmValue == 0) T = 32;                            \
   else if (enmValue == 1) T = 64;                       \
   else if (enmValue == 2) T = 128;                      \
   else T = 256;                                         \
}

#define WR_GET_MODIFI_COEFF(enmValue, value) {           \
   if (enmValue == 0) value = 2;                         \
   else if (enmValue == 1) value = 4;                    \
   else if (enmValue == 2) value = 8;                    \
   else value = 16;                                      \
}

#define WR_GET_NB_VALUE(enmValue, T, value) {            \
   if (enmValue == 0) value = T * 4;                     \
   else if (enmValue == 1) value = T * 2;                \
   else if (enmValue == 2) value = T * 1;                \
   else if (enmValue == 3) value = (T / 2);              \
   else if (enmValue == 4) value = (T / 4);              \
   else if (enmValue == 5) value = (T / 8);              \
   else if (enmValue == 6) value = (T / 16);             \
   else if (enmValue == 7) value = (T / 32);             \
}

#define WR_EMM_EUTRAN_HASH_NO_BINS 10
#define WR_EMM_EUTRAN_HASH_CDMA_1XRTT_NO_BINS 10
#define WR_EMM_EUTRAN_HASH_CDMA_HRPD_NO_BINS 10
#define WR_EMM_NEIGHBOUR_ENODEB_NO_BINS 10

#define WR_CFI_POWOS                        6000
#define WR_SIG_POWOS                        6000
#define WR_DCI_POWOS                        6000
#define WR_EXT_WIN_MARGIN                   5
#define WR_PUCCH_NOISE_GAMA                 0x2000
#define WR_PRACH_RATIO_4                    0x24eb
#define WR_PRACH_RATIO_0                    0x38cc
#define WR_SRS_DOP_EST_FACTOR               0xffff
#define WR_PUSCH_DTX_ACK                    22
#define WR_PUCCH_DTX_ACK                    25
#define WR_TX_ANTENA_PORTS                  2
#define WR_RX_ANTENA_PORTS                  2
#define WR_PHY_SYNC_MODE                    0x02
#define WR_DATA_REP_MODE                    0x00
#define WR_RACH_SR_REP_MODE                 0




#define WR_SIB_2             30
#define WR_SIB_3             0
#define WR_SIB_4             1
#define WR_SIB_5             2
#define WR_SIB_6             3
#define WR_SIB_7             4
#define WR_SIB_8             5
#define WR_SIB_9             6 
#define WR_SIB_10            7 
#define WR_SIB_11            8 
#define WR_SIB_12            9

#define WR_RECFG 1
#define WR_CELL_SCHED_RECFG_PEND   1
#define WR_CELL_PHY_RECFG_PEND     2
#define WR_CELL_RRC_RECFG_PEND     4

#define WR_PERIOD_8RF        NhuSchedulingInfosi_Periodicityrf8Enum
#define WR_PERIOD_16F        NhuSchedulingInfosi_Periodicityrf16Enum
#define WR_PERIOD_32RF       NhuSchedulingInfosi_Periodicityrf32Enum
#define WR_PERIOD_64RF       NhuSchedulingInfosi_Periodicityrf64Enum
#define WR_PERIOD_128RF      NhuSchedulingInfosi_Periodicityrf128Enum
#define WR_PERIOD_256RF      NhuSchedulingInfosi_Periodicityrf256Enum
#define WR_PERIOD_512RF      NhuSchedulingInfosi_Periodicityrf512Enum

/* antz - Changed names to reflect use, and using for both RLC and MAC */
#define WR_BCCH_BCH_ID                 1
#define WR_BCCH_SIB1_ID                2
#define WR_BCCH_SIB_OTHERS_ID          3
#define WR_CCCH_DL_ID                  4
#define WR_CCCH_UL_ID                  5
#define WR_PCCH_ID                     6

/* Broadcast related parameters that should ultimately be done away with */
#define WR_SFN_INCREMENT               4
#define WR_MIB_PRESENT                 0x1
#define WR_MIB_PERIOD                  4
#define WR_MIB_SUBFRAME                0
#define WR_SIB1_PERIOD                 2
#define WR_SIB1_SUBFRAME               5
#define WR_SIB1_PRESENT                0x2
#define WR_SIB2_PERIOD                 NhuSchedulingInfosi_Periodicityrf16Enum
#define WR_SIB2_SUBFRAME               0
#define WR_SIB2_PRESENT                0x4
#define WR_SIB3_PERIOD                 NhuSchedulingInfosi_Periodicityrf32Enum
#define WR_SIB4_PERIOD                 NhuSchedulingInfosi_Periodicityrf64Enum
#define WR_SIB5_PERIOD                 NhuSchedulingInfosi_Periodicityrf128Enum
#define WR_SIB6_PERIOD                 NhuSchedulingInfosi_Periodicityrf512Enum
#define WR_SIB7_PERIOD                 NhuSchedulingInfosi_Periodicityrf512Enum
#define WR_SIB8_PERIOD                 NhuSchedulingInfosi_Periodicityrf256Enum
#define WR_SIB9_PERIOD                 NhuSchedulingInfosi_Periodicityrf512Enum
#define WR_SIB10_PERIOD                 NhuSchedulingInfosi_Periodicityrf128Enum
#define WR_SIB11_PERIOD                 NhuSchedulingInfosi_Periodicityrf128Enum
#define WR_SIB12_PERIOD                 NhuSchedulingInfosi_Periodicityrf512Enum
#ifdef ENB_OVRLOAD
/*Macros for Access Class Barring Time --*/
#define WR_SIB2_ACBRTIME4             NhuAC_BarringConfigac_BarringTimes4Enum
#define WR_SIB2_ACBRTIME8             NhuAC_BarringConfigac_BarringTimes8Enum
#define WR_SIB2_ACBRTIME16            NhuAC_BarringConfigac_BarringTimes16Enum
#define WR_SIB2_ACBRTIME32            NhuAC_BarringConfigac_BarringTimes32Enum
#define WR_SIB2_ACBRTIME64            NhuAC_BarringConfigac_BarringTimes64Enum
#define WR_SIB2_ACBRTIME128           NhuAC_BarringConfigac_BarringTimes128Enum
#define WR_SIB2_ACBRTIME256           NhuAC_BarringConfigac_BarringTimes256Enum
#define WR_SIB2_ACBRTIME512           NhuAC_BarringConfigac_BarringTimes512Enum

/*Macros for Access Class Barring Factor --*/
#define WR_SIB2_ACBRFTR0              NhuAC_BarringConfigac_BarringFactorp00Enum
#define WR_SIB2_ACBRFTR5              NhuAC_BarringConfigac_BarringFactorp05Enum
#define WR_SIB2_ACBRFTR10             NhuAC_BarringConfigac_BarringFactorp10Enum
#define WR_SIB2_ACBRFTR15             NhuAC_BarringConfigac_BarringFactorp15Enum
#define WR_SIB2_ACBRFTR20             NhuAC_BarringConfigac_BarringFactorp20Enum
#define WR_SIB2_ACBRFTR25             NhuAC_BarringConfigac_BarringFactorp25Enum
#define WR_SIB2_ACBRFTR30             NhuAC_BarringConfigac_BarringFactorp30Enum
#define WR_SIB2_ACBRFTR35             NhuAC_BarringConfigac_BarringFactorp35Enum
#define WR_SIB2_ACBRFTR40             NhuAC_BarringConfigac_BarringFactorp40Enum
#define WR_SIB2_ACBRFTR45             NhuAC_BarringConfigac_BarringFactorp45Enum
#define WR_SIB2_ACBRFTR50             NhuAC_BarringConfigac_BarringFactorp50Enum
#define WR_SIB2_ACBRFTR55             NhuAC_BarringConfigac_BarringFactorp55Enum
#define WR_SIB2_ACBRFTR60             NhuAC_BarringConfigac_BarringFactorp60Enum
#define WR_SIB2_ACBRFTR65             NhuAC_BarringConfigac_BarringFactorp65Enum
#define WR_SIB2_ACBRFTR70             NhuAC_BarringConfigac_BarringFactorp70Enum
#define WR_SIB2_ACBRFTR75             NhuAC_BarringConfigac_BarringFactorp75Enum
#define WR_SIB2_ACBRFTR80             NhuAC_BarringConfigac_BarringFactorp80Enum
#define WR_SIB2_ACBRFTR85             NhuAC_BarringConfigac_BarringFactorp85Enum
#define WR_SIB2_ACBRFTR90             NhuAC_BarringConfigac_BarringFactorp90Enum
#define WR_SIB2_ACBRFTR95             NhuAC_BarringConfigac_BarringFactorp95Enum
#endif

#define WR_EMM_UTRA_TDD_BAND_A 1
#define WR_EMM_UTRA_TDD_BAND_B 2
#define WR_EMM_UTRA_TDD_BAND_C 3
#define WR_EMM_UTRA_TDD_BAND_D 4
#define WR_EMM_UTRA_TDD_BAND_E 5
#define WR_EMM_UTRA_TDD_BAND_F 6

#define WR_EMM_DEFAULT_OFFSET 15

#define WR_NRT_CELL_SRC_OTHER    0  /* Default value */
#define WR_NRT_CELL_SRC_UE_MEAS  1  /* Cell added in NRT due to UE meas */


/** @brief This structure contains the following 
   U8                        period;
   U8                        sibId;
 */
typedef struct wrSibCfg
{
   U8                        period;
   U8                        sibId;
} WrSibCfg;

typedef struct wrGuGrpIdLst
{
#ifndef MME_LDBAL
   Bool                      isValid;
   U8                        refCnt;
#endif
   U16                       mmeGrpId;
#if 1
   WrPlmnId                  plmnId;
#endif
} WrGuGrpIdLst;

typedef enum wrPhichDuration {
   WrPhichNormal,
   WrPhichExtended
} WrPhichDuration;

typedef enum wrPhichGrps
{
   WrPhichOneSixth,
   WrPhichHalf,
   WrPhichOne,
   WrPhichTwice
} WrPhichGrps;

/**   @brief These are the enumerations for the different EUTRAN bandwidths
 */
typedef enum wrEutranBw
{
   WR_EUTRAN_BW_6 = 0,
   WR_EUTRAN_BW_15,
   WR_EUTRAN_BW_25,
   WR_EUTRAN_BW_50,
   WR_EUTRAN_BW_75,
   WR_EUTRAN_BW_100
}WrEutranBw;

/**   @brief These are the enumerations for the HO counters
 */
typedef enum wrCdmaType{
 
   WR_1XRTT,
   WR_HRPD
}WrCdmaType;

/**   @brief These are the enumerations for the Qoffset values
 */
typedef enum WrQoffRange
{
   WR_OFFSET_DB_MINUS_24 = 0,
   WR_OFFSET_DB_MINUS_22,
   WR_OFFSET_DB_MINUS_20,
   WR_OFFSET_DB_MINUS_18,
   WR_OFFSET_DB_MINUS_16,
   WR_OFFSET_DB_MINUS_14,
   WR_OFFSET_DB_MINUS_12,
   WR_OFFSET_DB_MINUS_10,
   WR_OFFSET_DB_MINUS_8,
   WR_OFFSET_DB_MINUS_6,
   WR_OFFSET_DB_MINUS_4,
   WR_OFFSET_DB_MINUS_2,
   WR_OFFSET_DB_ZERO_0,
   WR_OFFSET_DB_PLUS_1,
   WR_OFFSET_DB_PLUS_2,
   WR_OFFSET_DB_PLUS_3,
   WR_OFFSET_DB_PLUS_4,
   WR_OFFSET_DB_PLUS_5,
   WR_OFFSET_DB_PLUS_6,
   WR_OFFSET_DB_PLUS_8,
   WR_OFFSET_DB_PLUS_10,
   WR_OFFSET_DB_PLUS_12,
   WR_OFFSET_DB_PLUS_14,
   WR_OFFSET_DB_PLUS_16,
   WR_OFFSET_DB_PLUS_18,
   WR_OFFSET_DB_PLUS_20,
   WR_OFFSET_DB_PLUS_21,
   WR_OFFSET_DB_PLUS_24
}WrQoffRange;

/* PH04_CMAS */
/**     @brief These are the enumerations for transaction ID type.
*/
/* This enumeration is used to identify the SIB configuration confirm type. */
typedef enum wrTransType{
   WR_MIB_TRANS_ID = 10,
   WR_SIB1_TRANS_ID,
   WR_SIB1_PRI_ETWS_TRANS_ID,
   WR_SIB1_SEC_ETWS_TRANS_ID,
   WR_SIB1_BOTH_ETWS_TRANS_ID, 
   WR_SIB1_CMAS_TRANS_ID,
   WR_SIB1_KILL_TRANS_ID,
   WR_SIB8_TRANS_ID
}WrTransType;

/**     @brief These are the enumerations for transaction ID type.
*/
typedef enum wrSiIndex{
   WR_PBM_SIB10_SI_INDEX = 10, /* SIB10 SI index in fixed as 10 */
   WR_PBM_SIB11_SI_INDEX,
   WR_PBM_SIB12_SI_INDEX
}WrSiIndex;

#define WR_MAX_SI_MSG        32
#define WR_MAX_SIB_PER_SI    16
#define WR_PRES              1
#define WR_NOTPRSNT          0

#define WR_HENB_NAME_MAX_SIZE_SIB9  48
#define WR_EMM_CB_ENB_NAME          151

/* PH04_CMAS */
#define WR_PBM_MAX_NUM_SIS   13

#define WR_MIB_DLBW6          NhuMasterInfoBlockdl_Bandwidthn6Enum
#define WR_MIB_DLBW15         NhuMasterInfoBlockdl_Bandwidthn15Enum
#define WR_MIB_DLBW25         NhuMasterInfoBlockdl_Bandwidthn25Enum
#define WR_MIB_DLBW50         NhuMasterInfoBlockdl_Bandwidthn50Enum
#define WR_MIB_DLBW75         NhuMasterInfoBlockdl_Bandwidthn75Enum
#define WR_MIB_DLBW100        NhuMasterInfoBlockdl_Bandwidthn100Enum

/* LTE_ADV_DSFR starts */
#define WR_LOAD_INF_TRANS_ID     12    /* SKS :: TODO */
/* LTE_ADV_DSFR ends */

/* PH04_CMAS:  Macro to use if warning area type is not matching. */
#define WR_PWS_NO_MATCH_FOUND 3


typedef struct wrSISchdInfo
{
   U16                       period;
   U8                        numSibs;
   U8                        sibs[WR_MAX_SIB_PER_SI];
} WrSISchdInfo;

typedef struct wrMibParams
{
   WrCellBw                  dlBw;
   WrPhichDuration           phichDur;
   WrPhichGrps               phichGrps;
} WrMibParams;

/* The definitions below are meant for SIB 1                              */
#define WR_MAX_PLMN_IDS      6

/*Macros for SIB1 params Cell Bared */
#define WR_SIB1_CELL_BARRED_ENUM      NhuSysInfoBlockTyp1cellAccessRelatedInfocellBarredbarredEnum
#define WR_SIB1_CELL_NOT_BARRED_ENUM  NhuSysInfoBlockTyp1cellAccessRelatedInfocellBarrednotBarredEnum

/*Macros for  SIB1 Intra Frequency selection */
#define WR_SIB1_INTRAFREQSELALWD  NhuSysInfoBlockTyp1cellAccessRelatedInfointraFreqReselectionallowedEnum 
#define WR_SIB1_INTRAFRQSELNTALWD NhuSysInfoBlockTyp1cellAccessRelatedInfointraFreqReselectionnotAllowedEnum


/*Macros for SIB1 SI WINDOW LENGTH*/
#define WR_SIB1_SIWNDWLNGTH1      NhuSysInfoBlockTyp1si_WindowLengthms1Enum
#define WR_SIB1_SIWNDWLNGTH2      NhuSysInfoBlockTyp1si_WindowLengthms2Enum
#define WR_SIB1_SIWNDWLNGTH5      NhuSysInfoBlockTyp1si_WindowLengthms5Enum
#define WR_SIB1_SIWNDWLNGTH10     NhuSysInfoBlockTyp1si_WindowLengthms10Enum
#define WR_SIB1_SIWNDWLNGTH15     NhuSysInfoBlockTyp1si_WindowLengthms15Enum
#define WR_SIB1_SIWNDWLNGTH20     NhuSysInfoBlockTyp1si_WindowLengthms20Enum
#define WR_SIB1_SIWNDWLNGTH40     NhuSysInfoBlockTyp1si_WindowLengthms40Enum

/*Macros for SIB1 SI Periodicity */
#define WR_SIB1_SIPERIODRF8       NhuSchedulingInfosi_Periodicityrf8Enum
#define WR_SIB1_SIPERIODRF16      NhuSchedulingInfosi_Periodicityrf16Enum
#define WR_SIB1_SIPERIODRF32      NhuSchedulingInfosi_Periodicityrf32Enum
#define WR_SIB1_SIPERIODRF64      NhuSchedulingInfosi_Periodicityrf64Enum
#define WR_SIB1_SIPERIODRF128     NhuSchedulingInfosi_Periodicityrf128Enum
#define WR_SIB1_SIPERIODRF256     NhuSchedulingInfosi_Periodicityrf256Enum
#define WR_SIB1_SIPERIODRF512     NhuSchedulingInfosi_Periodicityrf512Enum

/* csgId  - has valid value only if csgInd is set to true                 */
/* qRxLevMinOffset - takes values 1 to 8. Use 0 as unavailable            */
/* The first SI with number of SIBs zero indicates the end of SI list     */
typedef struct wrPlmnInfo
{
   WrPlmnId                  plmnId;
   U8                        resvForOperUse;
   WrRatPriorityList         plmnRatPriority;
   U8                        isRedirSupport;   /* redirection support */
} WrPlmnInfo;

#define WR_PLMN_ACTIVATE     TRUE
#define WR_PLMN_DEACTIVATE   FALSE
typedef struct wrSib1Plmn
{
   WrPlmnId                  plmnId;
   Bool                      resvForOperUse;
   Bool                      activated;
} WrSib1Plmn;
/** @brief This structure defines the TDD configuration.
 *    - Subframe configuration {0, 1, 2, 3, 4, 5, 6}
 *    - Special subframe configuration {0 , 1, 2, 3, 4, 5, 6, 7, 8}
 */
typedef struct wrSib1TddCfg
{
   U8                        subframeCfg;          /*!< Subframe configuration {0, 1, ..} */
   U8                        specialSubframeCfg;   /*!< Special subframe configuration */
}WrSib1TddCfg;

/** @brief This structure contains the following members
 - U8           modified         Implies change has occured and 
                                 must be re encoded
 - U8           numPlmnIds       Number of broadcase PLMNs
 - WrSib1Plmn   plmns[WR_MAX_PLMN_IDS] Array of PLMNs
 - U16          tac              Tracking area
 - U32          cellIdentity     First 28 bits are valid 
 - U8           cellBarred       TRUE if cell is barred 
 - U8           intraFreqReselection Used to control Intra freq reselection
 - U8           csgInd           CSG Indicator
 - U32          csgId            CSG Identifier First 27 bits are valid 
 - S8           qRxLevMin        Cell selection parameter
 - U8           qRxLevMinOffsetPres If TRUE qRxLevMinOffset is present
 - S8           qRxLevMinOffset  Cell selection parameter
 - S8           pMax             Maximum TX power to be applied by the UEs 
 - U8           freqBandIndicator Frequency Band
 - U8           numSi            Number of SI(s) being broadcast
 - WrSISchdInfo siSchdInfo[WR_MAX_SIS] SI scheduling information
 - U8           siWindowLen      SI window length
 - U8           valueTag         value tag, incremented for every SI change
 */
typedef struct wrSib1Params
{
   U8                        modified;
#if 1
   U8                        numPlmnIds;
   WrSib1Plmn                plmns[WR_MAX_PLMN_IDS];
#endif
   U16                       tac;
   U32                       cellIdentity; /* First 28 bits are valid */
   U8                        cellBarred;
   U8                        intraFreqReselection;
   Bool                      csgInd;
   TknU32                    csgId; /* First 27 bits are valid */
   S8                        qRxLevMin;
   U8                        qRxLevMinOffsetPres;
   S8                        qRxLevMinOffset;
   S8                        pMax;
   U8                        freqBandIndicator;
   U8                        numSis;
   WrSISchdInfo              siSchdInfo[WR_PBM_MAX_NUM_SIS];
   U8                        siWindowLen;
   U8                        valueTag;
   WrSib1TddCfg              tddCfg; 
} WrSib1Params;

 
/*Macros for Num. of Rach Preambles */
#define  WR_SIB2_RA_PRMB4         NhuRACH_ConfigCmnpreambleInfonumberOfRA_Preamblesn4Enum
#define  WR_SIB2_RA_PRMB8         NhuRACH_ConfigCmnpreambleInfonumberOfRA_Preamblesn8Enum 
#define  WR_SIB2_RA_PRMB12        NhuRACH_ConfigCmnpreambleInfonumberOfRA_Preamblesn12Enum
#define  WR_SIB2_RA_PRMB16        NhuRACH_ConfigCmnpreambleInfonumberOfRA_Preamblesn16Enum
#define  WR_SIB2_RA_PRMB20        NhuRACH_ConfigCmnpreambleInfonumberOfRA_Preamblesn20Enum
#define  WR_SIB2_RA_PRMB24        NhuRACH_ConfigCmnpreambleInfonumberOfRA_Preamblesn24Enum
#define  WR_SIB2_RA_PRMB28        NhuRACH_ConfigCmnpreambleInfonumberOfRA_Preamblesn28Enum
#define  WR_SIB2_RA_PRMB36        NhuRACH_ConfigCmnpreambleInfonumberOfRA_Preamblesn36Enum
#define  WR_SIB2_RA_PRMB40        NhuRACH_ConfigCmnpreambleInfonumberOfRA_Preamblesn40Enum
#define  WR_SIB2_RA_PRMB44        NhuRACH_ConfigCmnpreambleInfonumberOfRA_Preamblesn44Enum
#define  WR_SIB2_RA_PRMB48        NhuRACH_ConfigCmnpreambleInfonumberOfRA_Preamblesn48Enum
#define  WR_SIB2_RA_PRMB52        NhuRACH_ConfigCmnpreambleInfonumberOfRA_Preamblesn52Enum
#define  WR_SIB2_RA_PRMB56        NhuRACH_ConfigCmnpreambleInfonumberOfRA_Preamblesn56Enum
#define  WR_SIB2_RA_PRMB60        NhuRACH_ConfigCmnpreambleInfonumberOfRA_Preamblesn60Enum
#define  WR_SIB2_RA_PRMB64        NhuRACH_ConfigCmnpreambleInfonumberOfRA_Preamblesn64Enum

/*Macros for sizeof RA Preambeles GroupA*/
#define  WR_SIB2_PRMB_GRPA_SZ4    NhuRACH_ConfigCmnpreambleInfopreamblesGroupAConfigsizeOfRA_PreamblesGroupAn4Enum 
#define  WR_SIB2_PRMB_GRPA_SZ8    NhuRACH_ConfigCmnpreambleInfopreamblesGroupAConfigsizeOfRA_PreamblesGroupAn8Enum 
#define  WR_SIB2_PRMB_GRPA_SZ12   NhuRACH_ConfigCmnpreambleInfopreamblesGroupAConfigsizeOfRA_PreamblesGroupAn12Enum 
#define  WR_SIB2_PRMB_GRPA_SZ16   NhuRACH_ConfigCmnpreambleInfopreamblesGroupAConfigsizeOfRA_PreamblesGroupAn16Enum 
#define  WR_SIB2_PRMB_GRPA_SZ20   NhuRACH_ConfigCmnpreambleInfopreamblesGroupAConfigsizeOfRA_PreamblesGroupAn20Enum 
#define  WR_SIB2_PRMB_GRPA_SZ24   NhuRACH_ConfigCmnpreambleInfopreamblesGroupAConfigsizeOfRA_PreamblesGroupAn24Enum 
#define  WR_SIB2_PRMB_GRPA_SZ28   NhuRACH_ConfigCmnpreambleInfopreamblesGroupAConfigsizeOfRA_PreamblesGroupAn28Enum 
#define  WR_SIB2_PRMB_GRPA_SZ32   NhuRACH_ConfigCmnpreambleInfopreamblesGroupAConfigsizeOfRA_PreamblesGroupAn32Enum 
#define  WR_SIB2_PRMB_GRPA_SZ36   NhuRACH_ConfigCmnpreambleInfopreamblesGroupAConfigsizeOfRA_PreamblesGroupAn36Enum 
#define  WR_SIB2_PRMB_GRPA_SZ40   NhuRACH_ConfigCmnpreambleInfopreamblesGroupAConfigsizeOfRA_PreamblesGroupAn40Enum 
#define  WR_SIB2_PRMB_GRPA_SZ44   NhuRACH_ConfigCmnpreambleInfopreamblesGroupAConfigsizeOfRA_PreamblesGroupAn44Enum 
#define  WR_SIB2_PRMB_GRPA_SZ48   NhuRACH_ConfigCmnpreambleInfopreamblesGroupAConfigsizeOfRA_PreamblesGroupAn48Enum 
#define  WR_SIB2_PRMB_GRPA_SZ52   NhuRACH_ConfigCmnpreambleInfopreamblesGroupAConfigsizeOfRA_PreamblesGroupAn52Enum 
#define  WR_SIB2_PRMB_GRPA_SZ56   NhuRACH_ConfigCmnpreambleInfopreamblesGroupAConfigsizeOfRA_PreamblesGroupAn56Enum 
#define  WR_SIB2_PRMB_GRPA_SZ60   NhuRACH_ConfigCmnpreambleInfopreamblesGroupAConfigsizeOfRA_PreamblesGroupAn60Enum 

/*Macros for Group Size A*/
#define WR_SIB2_GRPA_MSG_SZ56     NhuRACH_ConfigCmnpreambleInfopreamblesGroupAConfigmessageSizeGroupAb56Enum 
#define WR_SIB2_GRPA_MSG_SZ144    NhuRACH_ConfigCmnpreambleInfopreamblesGroupAConfigmessageSizeGroupAb144Enum 
#define WR_SIB2_GRPA_MSG_SZ208    NhuRACH_ConfigCmnpreambleInfopreamblesGroupAConfigmessageSizeGroupAb208Enum 
#define WR_SIB2_GRPA_MSG_SZ256    NhuRACH_ConfigCmnpreambleInfopreamblesGroupAConfigmessageSizeGroupAb256Enum 


/*Macros for Power offset Group B*/
#define WR_SIB2_GRPB_OFFSTINF     NhuRACH_ConfigCmnpreambleInfopreamblesGroupAConfigmessagePowerOffsetGroupBminusinfinityEnum
#define WR_SIB2_GRPB_OFFST0DB     NhuRACH_ConfigCmnpreambleInfopreamblesGroupAConfigmessagePowerOffsetGroupBdB0Enum 
#define WR_SIB2_GRPB_OFFST5DB     NhuRACH_ConfigCmnpreambleInfopreamblesGroupAConfigmessagePowerOffsetGroupBdB5Enum 
#define WR_SIB2_GRPB_OFFST8DB     NhuRACH_ConfigCmnpreambleInfopreamblesGroupAConfigmessagePowerOffsetGroupBdB8Enum 
#define WR_SIB2_GRPB_OFFST10DB    NhuRACH_ConfigCmnpreambleInfopreamblesGroupAConfigmessagePowerOffsetGroupBdB10Enum 
#define WR_SIB2_GRPB_OFFST12DB    NhuRACH_ConfigCmnpreambleInfopreamblesGroupAConfigmessagePowerOffsetGroupBdB12Enum 
#define WR_SIB2_GRPB_OFFST16DB    NhuRACH_ConfigCmnpreambleInfopreamblesGroupAConfigmessagePowerOffsetGroupBdB16Enum 
#define WR_SIB2_GRPB_OFFST18DB    NhuRACH_ConfigCmnpreambleInfopreamblesGroupAConfigmessagePowerOffsetGroupBdB18Enum 

/*Macros for Ue Timers and constants  t300*/

#define WR_SIB2_T300VAL100MS      NhuUE_TmrsAndConstst300ms100Enum
#define WR_SIB2_T300VAL200MS      NhuUE_TmrsAndConstst300ms200Enum
#define WR_SIB2_T300VAL300MS      NhuUE_TmrsAndConstst300ms300Enum
#define WR_SIB2_T300VAL400MS      NhuUE_TmrsAndConstst300ms400Enum
#define WR_SIB2_T300VAL600MS      NhuUE_TmrsAndConstst300ms600Enum
#define WR_SIB2_T300VAL1000MS     NhuUE_TmrsAndConstst300ms1000Enum
#define WR_SIB2_T300VAL1500MS     NhuUE_TmrsAndConstst300ms1500Enum
#define WR_SIB2_T300VAL2000MS     NhuUE_TmrsAndConstst300ms2000Enum


/*Macros for Ue Timers and constants T301 */
#define WR_SIB2_T301VAL100MS      NhuUE_TmrsAndConstst301ms100Enum 
#define WR_SIB2_T301VAL200MS      NhuUE_TmrsAndConstst301ms200Enum 
#define WR_SIB2_T301VAL300MS      NhuUE_TmrsAndConstst301ms300Enum 
#define WR_SIB2_T301VAL400MS      NhuUE_TmrsAndConstst301ms400Enum 
#define WR_SIB2_T301VAL600MS      NhuUE_TmrsAndConstst301ms600Enum 
#define WR_SIB2_T301VAL1000MS     NhuUE_TmrsAndConstst301ms1000Enum 
#define WR_SIB2_T301VAL1500MS     NhuUE_TmrsAndConstst301ms1500Enum 
#define WR_SIB2_T301VAL2000MS     NhuUE_TmrsAndConstst301ms2000Enum 

/*Macros for Ue Timers and constants T310 */
#define WR_SIB2_T310VAL0MS        NhuUE_TmrsAndConstst310ms0Enum
#define WR_SIB2_T310VAL50MS       NhuUE_TmrsAndConstst310ms50Enum
#define WR_SIB2_T310VAL100MS      NhuUE_TmrsAndConstst310ms100Enum
#define WR_SIB2_T310VAL200MS      NhuUE_TmrsAndConstst310ms200Enum
#define WR_SIB2_T310VAL500MS      NhuUE_TmrsAndConstst310ms500Enum
#define WR_SIB2_T310VAL1000MS     NhuUE_TmrsAndConstst310ms1000Enum
#define WR_SIB2_T310VAL2000MS     NhuUE_TmrsAndConstst310ms2000Enum


/*Macros for Ue Timers and constants N310  */

#define WR_SIB2_N310VALN1         NhuUE_TmrsAndConstsn310n1Enum 
#define WR_SIB2_N310VALN2         NhuUE_TmrsAndConstsn310n2Enum 
#define WR_SIB2_N310VALN3         NhuUE_TmrsAndConstsn310n3Enum 
#define WR_SIB2_N310VALN4         NhuUE_TmrsAndConstsn310n4Enum 
#define WR_SIB2_N310VALN6         NhuUE_TmrsAndConstsn310n6Enum 
#define WR_SIB2_N310VALN8         NhuUE_TmrsAndConstsn310n8Enum 
#define WR_SIB2_N310VALN10        NhuUE_TmrsAndConstsn310n10Enum 
#define WR_SIB2_N310VALN20        NhuUE_TmrsAndConstsn310n20Enum 

/*Macros for Ue Timers and constants T311 */
#define WR_SIB2_T311VAL1000       NhuUE_TmrsAndConstst311ms1000Enum 
#define WR_SIB2_T311VAL3000       NhuUE_TmrsAndConstst311ms3000Enum 
#define WR_SIB2_T311VAL5000       NhuUE_TmrsAndConstst311ms5000Enum 
#define WR_SIB2_T311VAL10000      NhuUE_TmrsAndConstst311ms10000Enum 
#define WR_SIB2_T311VAL15000      NhuUE_TmrsAndConstst311ms15000Enum 
#define WR_SIB2_T311VAL20000      NhuUE_TmrsAndConstst311ms20000Enum 
#define WR_SIB2_T311VAL30000      NhuUE_TmrsAndConstst311ms30000Enum 

/*Macros for Ue Timers and constants N311  */
#define WR_SIB2_N311VALN1         NhuUE_TmrsAndConstsn311n1Enum 
#define WR_SIB2_N311VALN2         NhuUE_TmrsAndConstsn311n2Enum 
#define WR_SIB2_N311VALN3         NhuUE_TmrsAndConstsn311n3Enum 
#define WR_SIB2_N311VALN4         NhuUE_TmrsAndConstsn311n4Enum 
#define WR_SIB2_N311VALN5         NhuUE_TmrsAndConstsn311n5Enum 
#define WR_SIB2_N311VALN6         NhuUE_TmrsAndConstsn311n6Enum 
#define WR_SIB2_N311VALN8         NhuUE_TmrsAndConstsn311n8Enum 
#define WR_SIB2_N311VALN10        NhuUE_TmrsAndConstsn311n10Enum 

/* Macros for BCCH eNUM*/
#define WR_SIB2_PERIODCOEFF2      NhuBCCH_ConfigmodificationPeriodCoeffn2Enum 
#define WR_SIB2_PERIODCOEFF4      NhuBCCH_ConfigmodificationPeriodCoeffn4Enum 
#define WR_SIB2_PERIODCOEFF8      NhuBCCH_ConfigmodificationPeriodCoeffn8Enum 
#define WR_SIB2_PERIODCOEFF16     NhuBCCH_ConfigmodificationPeriodCoeffn16Enum 

/*Macros for Paging cycle*/
#define WR_SIB2_PAGCYCLERF32      NhuPCCH_ConfigdefaultPagCyclerf32Enum
#define WR_SIB2_PAGCYCLERF64      NhuPCCH_ConfigdefaultPagCyclerf32Enum
#define WR_SIB2_PAGCYCLERF128     NhuPCCH_ConfigdefaultPagCyclerf32Enum
#define WR_SIB2_PAGCYCLERF256     NhuPCCH_ConfigdefaultPagCyclerf32Enum

/*Macros for NB*/
#define WR_SIB2_PAGNBFOURT        NhuPCCH_ConfignBfourTEnum
#define WR_SIB2_PAGNBTWOT         NhuPCCH_ConfignBtwoTEnum 
#define WR_SIB2_PAGNBHALFT        NhuPCCH_ConfignBhalfTEnum 
#define WR_SIB2_PAGNBONET         NhuPCCH_ConfignBoneTEnum
#define WR_SIB2_PAGNBQUARTERT     NhuPCCH_ConfignBquarterTEnum
#define WR_SIB2_PAGNBONEEIGHTT    NhuPCCH_ConfignBoneEighthTEnum
#define WR_SIB2_PAGNBONESIXTEENHT NhuPCCH_ConfignBoneSixteenthTEnum
#define WR_SIB2_PAGNBTHIRTYSECT   NhuPCCH_ConfignBoneThirtySecondTEnum

/*Macros for Chopping mode*/
#define WR_SIB2_CHPNGMODEINTERSUB NhuPUSCH_ConfigCmnpusch_ConfigBasichoppingModeinterSubFrameEnum
#define WR_SIB2_CHPNGMODEINTRASUB NhuPUSCH_ConfigCmnpusch_ConfigBasichoppingModeintraAndInterSubFrameEnum


/*Macros for Bandwidth configuration*/
#define WR_SIB2_BWCFG0            NhuSoundingRS_UL_ConfigCmnsetupsrs_BandwidthConfigbw0Enum
#define WR_SIB2_BWCFG1            NhuSoundingRS_UL_ConfigCmnsetupsrs_BandwidthConfigbw1Enum
#define WR_SIB2_BWCFG2            NhuSoundingRS_UL_ConfigCmnsetupsrs_BandwidthConfigbw2Enum
#define WR_SIB2_BWCFG3            NhuSoundingRS_UL_ConfigCmnsetupsrs_BandwidthConfigbw3Enum
#define WR_SIB2_BWCFG4            NhuSoundingRS_UL_ConfigCmnsetupsrs_BandwidthConfigbw4Enum
#define WR_SIB2_BWCFG5            NhuSoundingRS_UL_ConfigCmnsetupsrs_BandwidthConfigbw5Enum
#define WR_SIB2_BWCFG6            NhuSoundingRS_UL_ConfigCmnsetupsrs_BandwidthConfigbw6Enum
#define WR_SIB2_BWCFG7            NhuSoundingRS_UL_ConfigCmnsetupsrs_BandwidthConfigbw7Enum

/*Macros for SF Config*/
#define WR_SIB2_SFCFG0            NhuSoundingRS_UL_ConfigCmnsetupsrs_SubfrmConfigsc0Enum 
#define WR_SIB2_SFCFG1            NhuSoundingRS_UL_ConfigCmnsetupsrs_SubfrmConfigsc1Enum 
#define WR_SIB2_SFCFG2            NhuSoundingRS_UL_ConfigCmnsetupsrs_SubfrmConfigsc2Enum 
#define WR_SIB2_SFCFG3            NhuSoundingRS_UL_ConfigCmnsetupsrs_SubfrmConfigsc3Enum 
#define WR_SIB2_SFCFG4            NhuSoundingRS_UL_ConfigCmnsetupsrs_SubfrmConfigsc4Enum 
#define WR_SIB2_SFCFG5            NhuSoundingRS_UL_ConfigCmnsetupsrs_SubfrmConfigsc5Enum 
#define WR_SIB2_SFCFG6            NhuSoundingRS_UL_ConfigCmnsetupsrs_SubfrmConfigsc6Enum 
#define WR_SIB2_SFCFG7            NhuSoundingRS_UL_ConfigCmnsetupsrs_SubfrmConfigsc7Enum 
#define WR_SIB2_SFCFG8            NhuSoundingRS_UL_ConfigCmnsetupsrs_SubfrmConfigsc8Enum 
#define WR_SIB2_SFCFG9            NhuSoundingRS_UL_ConfigCmnsetupsrs_SubfrmConfigsc9Enum 
#define WR_SIB2_SFCFG10           NhuSoundingRS_UL_ConfigCmnsetupsrs_SubfrmConfigsc10Enum 
#define WR_SIB2_SFCFG11           NhuSoundingRS_UL_ConfigCmnsetupsrs_SubfrmConfigsc11Enum 
#define WR_SIB2_SFCFG12           NhuSoundingRS_UL_ConfigCmnsetupsrs_SubfrmConfigsc12Enum 
#define WR_SIB2_SFCFG13           NhuSoundingRS_UL_ConfigCmnsetupsrs_SubfrmConfigsc13Enum 
#define WR_SIB2_SFCFG14           NhuSoundingRS_UL_ConfigCmnsetupsrs_SubfrmConfigsc14Enum 
#define WR_SIB2_SFCFG15           NhuSoundingRS_UL_ConfigCmnsetupsrs_SubfrmConfigsc15Enum 

/*Macros for ALPHA */
#define WR_SIB2_ALPHAL0           NhuUlnkPowerCntrlCmnalphaal0Enum 
#define WR_SIB2_ALPHAL04          NhuUlnkPowerCntrlCmnalphaal04Enum 
#define WR_SIB2_ALPHAL05          NhuUlnkPowerCntrlCmnalphaal05Enum
#define WR_SIB2_ALPHAL06          NhuUlnkPowerCntrlCmnalphaal06Enum
#define WR_SIB2_ALPHAL07          NhuUlnkPowerCntrlCmnalphaal07Enum
#define WR_SIB2_ALPHAL08          NhuUlnkPowerCntrlCmnalphaal08Enum
#define WR_SIB2_ALPHAL09          NhuUlnkPowerCntrlCmnalphaal09Enum
#define WR_SIB2_ALPHAL11          NhuUlnkPowerCntrlCmnalphaal1Enum 


/*Macros for Format 1 */
#define WR_SIB2_FRMT1DF0          NhuDeltaFLst_PUCCHdeltaF_PUCCH_Format1deltaF_2Enum
#define WR_SIB2_FRMT1DF2          NhuDeltaFLst_PUCCHdeltaF_PUCCH_Format1deltaF0Enum 
#define WR_SIB2_FRMT1DF02         NhuDeltaFLst_PUCCHdeltaF_PUCCH_Format1deltaF2Enum

/*Macros for format 1b*/
#define WR_SIB2_FRMT1BDF1         NhuDeltaFLst_PUCCHdeltaF_PUCCH_Format1bdeltaF1Enum 
#define WR_SIB2_FRMT1BDF3         NhuDeltaFLst_PUCCHdeltaF_PUCCH_Format1bdeltaF3Enum
#define WR_SIB2_FRMT1BDF5         NhuDeltaFLst_PUCCHdeltaF_PUCCH_Format1bdeltaF5Enum

/*Macros for format 2*/
#define WR_SIB2_FRMT2DF2          NhuDeltaFLst_PUCCHdeltaF_PUCCH_Format2deltaF_2Enum 
#define WR_SIB2_FRMT2DF0          NhuDeltaFLst_PUCCHdeltaF_PUCCH_Format2deltaF0Enum 
#define WR_SIB2_FRMT2DF1          NhuDeltaFLst_PUCCHdeltaF_PUCCH_Format2deltaF1Enum
#define WR_SIB2_FRMT2DF02         NhuDeltaFLst_PUCCHdeltaF_PUCCH_Format2deltaF2Enum

/*Macros for format 2a*/
#define WR_SIB2_FRMT2ADF2         NhuDeltaFLst_PUCCHdeltaF_PUCCH_Format2adeltaF_2Enum 
#define WR_SIB2_FRMT2ADF0         NhuDeltaFLst_PUCCHdeltaF_PUCCH_Format2adeltaF0Enum
#define WR_SIB2_FRMT2ADF02        NhuDeltaFLst_PUCCHdeltaF_PUCCH_Format2adeltaF2Enum


/*Macros for format 2b*/

#define WR_SIB2_FRMT2BDF2         NhuDeltaFLst_PUCCHdeltaF_PUCCH_Format2bdeltaF_2Enum 
#define WR_SIB2_FRMT2BDF0         NhuDeltaFLst_PUCCHdeltaF_PUCCH_Format2bdeltaF0Enum 
#define WR_SIB2_FRMT2BDF02        NhuDeltaFLst_PUCCHdeltaF_PUCCH_Format2bdeltaF2Enum

/*Macros for UL Cyclic Prefix */
#define WR_SIB2_ULCYLCPRFX1       NhuUL_CyclicPrefixLengthlen1Enum
#define WR_SIB2_ULCYLCPRFX2       NhuUL_CyclicPrefixLengthlen2Enum
typedef U8                   WrCyclicPrefix;

/*Macros for Uplink Bandwidth */
#define WR_SIB2_ULBW6             NhuSysInfoBlockTyp2freqInfoul_Bandwidthn6Enum
#define WR_SIB2_ULBW15            NhuSysInfoBlockTyp2freqInfoul_Bandwidthn15Enum 
#define WR_SIB2_ULBW25            NhuSysInfoBlockTyp2freqInfoul_Bandwidthn25Enum
#define WR_SIB2_ULBW50            NhuSysInfoBlockTyp2freqInfoul_Bandwidthn50Enum
#define WR_SIB2_ULBW75            NhuSysInfoBlockTyp2freqInfoul_Bandwidthn75Enum
#define WR_SIB2_ULBW100           NhuSysInfoBlockTyp2freqInfoul_Bandwidthn100Enum

/*Macros for TA Timer*/
#define WR_SIB2_TATMRCMNSF500     NhuTimeAlignmentTimersf500Enum
#define WR_SIB2_TATMRCMNSF750     NhuTimeAlignmentTimersf750Enum
#define WR_SIB2_TATMRCMNSF1280    NhuTimeAlignmentTimersf1280Enum
#define WR_SIB2_TATMRCMNSF1920    NhuTimeAlignmentTimersf1920Enum
#define WR_SIB2_TATMRCMNSF2560    NhuTimeAlignmentTimersf2560Enum
#define WR_SIB2_TATMRCMNSF5120    NhuTimeAlignmentTimersf5120Enum
#define WR_SIB2_TATMRCMNSF1024    NhuTimeAlignmentTimersf10240Enum
#define WR_SIB2_TATMRCMNSFINF     NhuTimeAlignmentTimerinfinityEnum 

typedef struct wrSib2AcBarringCfg
{
   U8                        pres;
   U8                        acBarringFactor;
   U8                        acBarringTime;
   U8                        acBarringForSpecialAC;
   U8                        lwrCfgAcbFactor;
   U8                        lwrCfgAcbTime;
   Bool                      isRrmCfgPres;
} WrSib2AcBarringCfg;

/** @brief This structure contains SIB 2 information and has
 * the following members
 - U8  noRaPreambles       Number of Random Access preambles
 - U8  noGrpAPreambles     Number of Preambles in Group A
 - U8  grpAMsgSize         Size of message for which UE chooses Group A
 - U8  grpBPwrOffset       Group B power offset
 - U8  pwrRampStep         Power Ramping Factor
 - U8  prmbInitRcvdTrgtPwr Initial target power for preambles
 - U8  maxPrmbTrans        Maximum number of Preamble transmissions
 - U8  raWindowSize        RA window size during which RAR can be expected
 - U8  contResTimer        Contention resolution timer
 - U8  maxMsg3HarqTrans    Maximum Msg3 transmissions
 */
typedef struct wrSib2RachCfg
{
   U8                        noRaPreambles;
   U8                        noGrpAPreambles;
   U8                        grpAMsgSize;
   U8                        grpBPwrOffset;
   U8                        pwrRampStep;
   U8                        prmbInitRcvdTrgtPwr;
   U8                        maxPrmbTrans;
   U8                        raWindowSize;
   U8                        contResTimer;
   U8                        maxMsg3HarqTrans;
} WrSib2RachCfg;

/** @brief This structure contains the Sib2 parameter for BCCH configuration 
 -  U8         modifPeriodCoeff     Modification period coefficient 
               Modification period = Modification Coeff * Default paging DRX 
 */
typedef struct wrSib2BcchCfg
{
   U8                        modifPeriodCoeff;
} WrSib2BcchCfg;

/** @brief This structure contains the PCCH related parameters sent in SIB2
 - U8   defaultPagingCycle    Default paging cycle i.e. "T" as per 36.304
 - U8   nB                    nB used to derive PF/PO as per 36.304 
 */
typedef struct wrSib2PcchCfg
{
   U8                        defaultPagingCycle;
   U8                        nB;
} WrSib2PcchCfg;

/** @brief This structure contains the PRACH related configuration sent in SIB2
 - U16  rootSeqIdx      Root sequence index used to derive Preambles by UEs
 - U8   prachCfgIdx     PRACH configuration index used to determine occassions
 - U8   highSpeedFlag   HighSpeedFlag for Restricted or unRestricted set 36.211
 - U8   zeroCorrZone    Zero correlation flag used as per 36.211
 - U8   prachFreqOffset PRACH frequency offset
 */
typedef struct wrSib2PrachCfg
{
   U16                       rootSeqIdx;
   U8                        prachCfgIdx;
   U8                        highSpeedFlag;
   U8                        zeroCorrZone;
   U8                        prachFreqOffset;
} WrSib2PrachCfg;

/** @brief This structure contains the PDSCH configuration sent in SIB2
 - S8  refSigPower   Reference signal TX power
 - U8  pB            Cell specific ratio of rohB/rohA
 */
typedef struct wrSib2PdschCfg
{
   S8                        refSigPower;
   U8                        pB;
} WrSib2PdschCfg;

/** @brief This structure contains the PUSCH configuration sent in SIB2
 - U8 nSB                Number of Subbands
 - U8 hoppingMode        Hopping Mode 
 - U8 hoppingOffset      Hopping offset
 - U8 qam64Enabled       TRUE if 64 QAM is enabled in the cell
 - U8 grpHoppingEnabled  TRUE if Group hopping is enabled
 - U8 grpAssignPusch     PUSCH group assignment
 - U8 seqHoppingEnabled  Hopping Sequence enabled
 - U8 cyclicShift        Cyclic shift for PUSCH
 */
typedef struct wrSib2PuschCfg
{
   U8                        nSB;
   U8                        hoppingMode;
   U8                        hoppingOffset;
   U8                        qam64Enabled;
   U8                        grpHoppingEnabled;
   U8                        grpAssignPusch;
   U8                        seqHoppingEnabled;
   U8                        cyclicShift;
   U8                        puschSubBndStrt;
   U8                        puschNoSubbnds;
   U8                        puschSubBandSz;
   U8                        puschSubBandDmrs;
} WrSib2PuschCfg;

/** @brief This structure contains the PUCCH configuration sent in SIB2
 - U8  deltaPucchShift Delta PUCCH shift
 - U8  nRBCqi          Number of RBs used for PUCCH format 2/2a/2b
 - U8  nCSAN           Number of Cyclic shifts in the mixed RB
 - U16 n1PucchAn       N(1) PUCCH 
 */
typedef struct wrSib2PucchCfg
{
   U8                        deltaPucchShift;
   U8                        nRBCqi;
   U8                        nCSAN;
   U16                       n1PucchAn;
} WrSib2PucchCfg;

/** @brief This structure contains the SRS configuration information sent in
 * SIB2
 - U8 release           TRUE if SRS configuration is being Released
 - U8 srsBandwidthCfg   Cell SRS bandwidth
 - U8 srsSubfrmCfg      SRS subframe configuration
 - U8 ackNackSrsSimulTx TRUE if ACK/NACK can be multiplexed with SRS
 */
typedef struct wrSib2SrsCfg
{
   U8                        release;
   U8                        srsBandwidthCfg;
   U8                        srsSubfrmCfg;
   U8                        ackNackSrsSimulTx;
} WrSib2SrsCfg;

/** @brief This structure contains the UL power control configuration sent in
 * SIB2 
 - S8 p0NominalPusch       Nominal for PUSCH as per 36.213
 - U8 alpha                Alpha as per 36.213
 - S8 p0NominlPucch        Nominal for PUCCH as per 36.213
 - U8 deltaFPucchFormat1   Delta PUCCH for format 1
 - U8 deltaFPucchFormat1b  Delta PUCCH for format 1b
 - U8 deltaFPucchFormat2   Delta PUCCH for format 2
 - U8 deltaFPucchFormat2a  Delta PUCCH for format 2a
 - U8 deltaFPucchFormat2b  Delta PUCCH for format 2b
 - S8 deltaPeambleMsg3     Delta preable for Msg3
 */
typedef struct wrSib2UlPwrCtrlCfg
{
   S8                        p0NominalPusch;
   U8                        alpha;
   S8                        p0NominlPucch;
   U8                        deltaFPucchFormat1;
   U8                        deltaFPucchFormat1b;
   U8                        deltaFPucchFormat2;
   U8                        deltaFPucchFormat2a;
   U8                        deltaFPucchFormat2b;
   S8                        deltaPeambleMsg3;
} WrSib2UlPwrCtrlCfg;

/** @brief This structure contains SIB2 parameters
 - U8                    emergency        TRUE if Access barred for Emergency
 - U8                    signalling       TRUE if Access barred for Signalling
 - U8                    dataCall         TRUE if Access barred for Data
 - WrSib2RachCfg         rachCfg          RACH configuration
 - WrSib2BcchCfg         bcchCfg          BCCH configuration
 - WrSib2PcchCfg         pcchCfg          PCCH configuration
 - WrSib2PrachCfg        prachCfg         PRACH configuration
 - WrSib2PdschCfg        pdschCfg         PDSCH configuration
 - WrSib2PuschCfg        puschCfg         PUSCH configuration
 - WrSib2PucchCfg        pucchCfg         PUCCH configuration
 - WrSib2SrsCfg          srsCfg           SRS configuration
 - WrSib2UlPwrCtrlCfg    ulPwrCtrlCfg     UL Power control configuration
 - WrCyclicPrefix        ulCyclicPrefix   UL cyclic prefix 
 - U8                    t300             Timer t300
 - U8                    t301             Timer t301
 - U8                    t310             Timer t310
 - U8                    n310             n310
 - U8                    t311             Timer 311
 - U8                    n311             n311
 - U16                   ulCarrierFreq    UL carrier frequency
 - U8                    ulBw             UL Bandwidth
 - U8                    addlSpectrEmission  Additional spectrum emission
 - U8                    timingAlignTimer    UL time alignment timer
 */
typedef struct wrSib2Params
{
   U8                        siIndex;
   U8                        barringInfoPres;
   struct {
      U8                     emergency;
      WrSib2AcBarringCfg     signalling;
      WrSib2AcBarringCfg     dataCall;
   } barringInfo;
   WrSib2RachCfg             rachCfg;
   WrSib2BcchCfg             bcchCfg;
   WrSib2PcchCfg             pcchCfg;
   WrSib2PrachCfg            prachCfg;
   WrSib2PdschCfg            pdschCfg;
   WrSib2PuschCfg            puschCfg;
   WrSib2PucchCfg            pucchCfg;
   WrSib2SrsCfg              srsCfg;
   WrSib2UlPwrCtrlCfg        ulPwrCtrlCfg;
   WrCyclicPrefix            ulCyclicPrefix;
   U8                        t300;
   U8                        t301;
   U8                        t310;
   U8                        n310;
   U8                        t311;
   U8                        n311;
   U16                       ulCarrierFreq;
   U8                        ulBw;
   U8                        addlSpectrEmission;
   U8                        timingAlignTimer;
   WrSib2AcBarringCfg        voiceBarringInfo;
   WrSib2AcBarringCfg        videoBarringInfo;
} WrSib2Params;

/** @brief This structure contains Serving frequency information broadcast in
 * SIB3
- U8 sNonIntraSearchPres      Reselection threshold presense
- U8 sNonIntraSearch          Reselection threshold
- U8 threshServingLow         Reselection threshold
- U8 cellReselPrio            Cell reselection priority
-*/
typedef struct wrSib3ServFreqInfo
{
  U8                         sNonIntraSearchPres;
  U8                         sNonIntraSearch;
  U8                         threshServingLow;
  U8                         cellReselPrio;
} WrSib3ServFreqInfo;

/** @brief This structure contains Intra frequency cell reselection information
 * broadcast in SIB3
 - S8 qRxLevMin               Q RX Level Minium
 - U8 pMaxPres                P-MAX presence
 - S8 pMax                    P-MAX
 - U8 sIntraSearchPres        Intra frequency search presence
 - U8 SIntraSearch            Intra frequency search Reselection threshold
 - U8 allwdMeasBandwidthPres  If TRUE use allwdMeasBandwidthPres else use DL BW
 - U8 allwdMeasBandwidth      Bandwidth to be used for measurement
 - U8 antennaPort1Pres        TRUE if antenna Port 1 is present
 - U8 neighCellCfg            For MBSFN and TDD UL/DL configuration
 - U8 tReselEutra             Reselection for EUTRA
 - U8 tResetEutraSFMedium     Speed dependent scaling factor for EUTRA
 - U8 tResetEutraSFHigh       Speed dependent scaling factor for EUTRA
 */
typedef struct wrSib3IntraFreqCellResel
{
   S8                      qRxLevMin;
   U8                      pMaxPres;
   S8                      pMax;
   U8                      sIntraSearchPres;
   U8                      SIntraSearch;
   U8                      allwdMeasBandwidthPres;
   U8                      allwdMeasBandwidth;
   U8                      antennaPort1Pres;
   U8                      neighCellCfg;
   U8                      tReselEutra;
   U8                      tResetEutraSFMedium;
   U8                      tResetEutraSFHigh;
} WrSib3IntraFreqCellResel;

/** @brief This structure contains the Mobility state information that is
 * broadcast in SIB3
 - U8 tEvaluation       t Evaluation
 - U8 tHyst             Hystrysis
 - U8 nCellChangeMed    Cell change Medium
 - U8 nCellChangeHigh   Cell change High
 */
typedef struct wrSib3MobiStateParams
{
   U8                        tEvaluation;
   U8                        tHyst;
   U8                        nCellChangeMed;
   U8                        nCellChangeHigh;
} WrSib3MobiStateParams;
typedef struct wrSib3R9Params
{
  U8                         sIntraSearchRel9Pres;
  U8                         sIntraSearchP;
  U8                         sIntraSearchQ;
  U8                         sNonIntraSearchRel9Pres;
  U8                         sNonIntraSearchP;
  U8                         sNonIntraSearchQ;
  U8                         qQualMinRel9Pres;
  S8                         qQualMin;
  U8                         threshServLowQRel9Pres; 
  U8                         threshServLowQ; 
} WrSib3R9Params;
/* * speedStReselPreset is 0 implies that sfMedium and sfHigh are not to  */
/*   be included in the broadcast. These define hysteris parameters for   */
/*   UEs with medium and high mobility                                    */
/** @brief This structure contains the mobililty related parameters broadcast in
 * SIB3
 - U8                        qHyst                 Hysteris
 - U8                        speedStateReselPres   Speed state 
                                                   reselection params presence
 - WrSib3MobiStateParams     mobStateParams        Speed state params
 - U8                        sfMedium              Medium speed 
 - U8                        sfHigh                High speed
 - WrSib3ServFreqInfo        servFreqInfo          Serving frequency information
 - WrSib3IntraFreqCellResel  intraFreqReSel        Intra frequency reselection 
                                                   parameters
 */
typedef struct wrSib3Params
{
   U8                        siIndex;
   U8                        qHyst;
   U8                        speedStateReselPres;
   WrSib3MobiStateParams     mobStateParams;
   U8                        sfMedium;
   U8                        sfHigh;
   WrSib3ServFreqInfo        servFreqInfo;
   WrSib3IntraFreqCellResel  intraFreqReSel;
   WrSib3R9Params            r9Params;
} WrSib3Params;


#define MAX_INTRA_FREQ_NEIGH_CELLS  16
/** @brief This structure contains the serving cell CSG ID information neeed to
 * broadcast in SIB4.This structure contains the folowing Parameters.
 -  U8                    siIndex           Index of SI msg which includes SIB4 
 -  Bool                  csgCellInfoPres   CSG supported cell.
 -  WrPhysCellIdRange     csgPhyCellIdRange CSG cell ID information.
 */
typedef struct wrSib4Params
{
   U8                    siIndex;   
   Bool                  csgCellInfoPres;   
   WrPhysCellIdRange     csgPhyCellIdRange;
} WrSib4Params;

/** @brief This structure contains the parameters broadcast in
 * SIB6
 */
typedef struct wrSib6Params
{
   U8         siIndex;
   Bool           sfUsagePres;
   U16          tReselection;
   U8          tReselectionSfMedium;
   U8          tReselectionSfHigh;
}WrSib6Params;

/** @brief This structure contains the parameters broadcast in
 * SIB9
 */
typedef struct wrSib9Params
{
   U8                        siIndex;
   U8                        henbName[WR_HENB_NAME_MAX_SIZE_SIB9];
}WrSib9Params;

typedef struct wrCellUlSchdCfgGrp
{
   U8      maxUePerUlSf;       
   U8      maxUlUeNewTxPerTti; 
   U32     ulSchedulingAlg;
}WrCellUlSchdCfgGrp;

typedef struct wrCellDlSchdCfgGrp
{
   U8      maxUePerDlSf; 
   U8      maxDlUeNewTxPerTti; 
   U32     dlSchedulingAlg;
}WrCellDlSchdCfgGrp;

#if 0
/** @brief This structure contains inter frequency information which
 * will be ibroadcasted in SIB5.
   - U32       dlCarrierFreq           Down Link Carriear Frequency.
   - S8        qRxLevMin               Q RX Level Minium
   - U8        pMaxPres                P MAX Presence
   - S8        pMax                    P Max Value
   - U8        tReselEutra             T-Reselection of EUTRAN 
   - U8        tReselSpeedSFPres       Speed Scale Factors presence for 
                                       T-Relection.
   - U8        tReselEutraSFMedium     SpeedStateScaleFactors Medium Optional
   - U8        tReselEutraSFHigh       SpeedStateScaleFactors High Optional
   - U8        threshHigh              ReselectionThreshold High
   - U8        threshLow               ReselectionThreshold Low 
   - U8        allwdMeasBandwidth      Allowed Measurement Bandwidth 
   - U8        antennaPort1Pres        Antenna Port Presence.
   - U8        cellReslectionPrioPres  Cell reselection priority present or not  
   - U8        cellReslectionPrio      Cell reselection priority 0-7 
   - U8        neighCellCfg            Neighbour Cell Config length: 2 bits 
   - U8        qOffsetRange            Q-Offset Range 
   - U8        qQualMinR9Pres          qQualMinR9 is present or not 
   - S8        qQualMinR9              minimum received RSRQ level in the(E-UTRA) cell 
   - U8        reSelcThreshHighR9      Reselection threshold High
   - U8        reSelcThreshLowR9       Teselcation threshold Low.
 */
typedef struct wrInterFreqInfo
{ 
   U32               dlCarrierFreq;  
   S8                qRxLevMin;
   U8                pMaxPres;
   S8                pMax;
   U8                tReselEutra;      
   U8                tReselSpeedSFPres;
   U8                tReselEutraSFMedium; 
   U8                tReselEutraSFHigh; 
   U8                threshHigh;  
   U8                threshLow;   
   U8                allwdMeasBandwidth;  
   U8                antennaPort1Pres;
   U8                cellReslectionPrioPres; 
   U8                cellReslectionPrio;  
   U8                neighCellCfg;       
   U8                qOffsetRange;      
   U8                qQualMinR9Pres;   
   S8                qQualMinR9;          
   U8                reSelcThreshHighR9;
   U8                reSelcThreshLowR9;
} WrInterFreqInfo;

#define MAX_EUTRAN_INTER_FREQ 8    /* Maximum number of EUTRAN Carrier frquencies */
#endif
/** @brief This structure contains the inter frequecy related information which
 *         is broadcast in SIB5.
   - U8              siIndex             Index of SI msg which includes SIB4
#if 0
   - U8              numInterFreq        Number of inter frequency informations
   - WrInterFreqInfo interFreq[MAX_EUTRAN_INTER_FREQ] Inter frequecy information.
#endif
 */
typedef struct wrSib5Params
{
   U8              siIndex;
#if 0
   U8              numInterFreq;
   WrInterFreqInfo interFreq[MAX_EUTRAN_INTER_FREQ]; 
#endif
} WrSib5Params;

/*

U8 syncChoice; choice parameter for sync system time
     union  tells the absolute time  in current cell.
     {  
        U8 synchSysTime[5]; * 39 Bits *   lower accuracy level time.
        U8 asyncSysTIme[7]; * 49 bits *Higher accuracy level tim
      } u;
*/

typedef struct wrSysTimeInfoCDMA2000
{
   Bool cdmaEutraTimeSync;   
   U8 syncChoice;
    union 
       {
         U8 synchSysTime[5]; /* 39 Bits */
         U8 asyncSysTime[7]; /* 49 bits */
        } u;
} WrSysTimeInfoCDMA2000;

/*
 * U8 preRegAllow Flag it tells if pre-Registration is allowed or not.
 * U8 preHrpdZoneId  used to control when the UE should re-register.
 * U8 secPreegZoneIds   used to control when the UE should re-register.
 * */
typedef struct wrHrpdPreRegInfo
{
     Bool                       preRegAllow;
     TknU8                      preHrpdZoneId;
     TknU8                      secPreRegZoneIds[2];
} WrHrpdPreRegInfo;
/**@brief 
 *   Bool     acBarringFor1XRTTPres; * Access-Class Barring info is Optional *
 *   U8       acBarr0to9;   access barring type 0 to9.
 *   U8       acBarr10;   access barring type 10. 
 *   U8       acBarr11; access barring type 11
 *   U8       acBarr12;  access barring type 12
 *   U8       acBarr13;   access barring type 13
 *   U8       acBarr14; access barring type 14
 *   U8       acBarr15;  access barring type 15
 *   U8       acBarrMsg;  access barring Message
 *   U8       acBarrReg;   access barring Registered.
 *   U8       acBarrEmg; access barring Emergency
 *
 **/

 typedef struct wrAcBarrCDMA2K
  {
     Bool     acBarringFor1XRTTPres; /* Access-Class Barring info is Optional */
     U8       acBarr0to9;
     U8       acBarr10;
     U8       acBarr11;
     U8       acBarr12;
     U8       acBarr13;
     U8       acBarr14;
     U8       acBarr15;
     U8       acBarrMsg;
     U8       acBarrReg;
     U8       acBarrEmg;
    } WrAcBarrCDMA2K;

/**@brief
 *  U8           bandClass; List of CDMA2K Bands.  
 *  U8           cellReselPriPres;flag to indicate Cell Reselection Priority.
 *  U8           cellReselPri; [* OPTIONAL] *Cell Reselection Priority
 *  U8           threshHigh;specifies High Threshold used in Reselection in
 *  CDMA2K Band class.
 *  U8           threshLow; specifies Low Threshold used in Reselection in
 *  CDMA2K Band Class.
 *      */
typedef struct wrCDMA2000BandClasInfo
  {
     U8           bandClass;
     TknU32           cellReselPri; /* OPTIONAL */
     U8           threshHigh;
     U8           threshLow;
   } WrCDMA2000BandClasInfo;


/**@brief
 * U16   arfcn Indicates the CDMA2K carrier freq withn CDMA@K Band.
 * U8 noOfPhyCellIds No of Phycell ids configured
 * U16   phyCellIdLst List of Phycell ids.
 * */

typedef struct wrCDMA2000BandInfo
  {   
     U16            arfcn;
     U8           noOfPhyCellIds;
     U16         phyCellIdLst[16];
  } WrCDMA2000BandInfo;

 
/**@brief
 * U8              bandClass; The bandclass for the neighbouring cell.
 * U8             noOfFreq;   The num of frequencies for the neighbouring cell.  
 * WrCDMA2000BandInfo neighFreqInfo[16];
 *
 * */
typedef struct wrCDMA2000NeighCellInfo
  {   
     U8              bandClass;
     U8             noOfFreq;
     WrCDMA2000BandInfo neighFreqInfo[16];
   } WrCDMA2000NeighCellInfo;

/**@brief
 *  U8                          numBandClasses;Num of Band class defined for
 * Reselection.
 *WrCDMA2000BandClasInfo      bandClassLst[WR_MAX_CDMA_BAND_CLASS];list of
 *Band class
 * U8                          numNeighCellLst; num of NeighCelllst for Cell
 *Reselection.
 * WrCDMA2000NeighCellInfo     neighCellLst[WR_MAX_NUM_CDMA_NEIGH_CELLS];
 * List of neighbour cell list
 * U8                          tReselCDMA2000;      * T-Reselection of
 * CDMA2000 *
 *  U8                          tReselSpeedSFPres;flag for speed related
 * scaling factor.
 * U8                          tReselCdma2000SFMedium; * Optional
 * SpeedStateScaleFactors Medium  *
 * U8                          tReselCdma2000SFHigh;   * Optional
 * SpeedStateScaleFactors High *
 *
 **/
typedef struct wrCDMA2000CellReselParam
  {
   U8                          numBandClasses;
    WrCDMA2000BandClasInfo      bandClassLst[WR_MAX_CDMA_BAND_CLASS];
    U8                          numNeighCellLst;
    WrCDMA2000NeighCellInfo     neighCellLst[WR_MAX_NUM_CDMA_NEIGH_CELLS];
    U8                          tReselCDMA2000;      /* T-Reselection of CDMA2000 */
    U8                          tReselSpeedSFPres;
    U8                          tReselCdma2000SFMedium; /* Optional SpeedStateScaleFactors Medium  */
    U8                          tReselCdma2000SFHigh;   /* Optional SpeedStateScaleFactors High */
  } WrCDMA2000CellReselParam;

/**@brief 
 *  U16       sid; system id , used to control when UE should Re-register
 *  U16       nid;  network id , used to control when UE should Re-register
 *  Bool      mulSID; Multiple SID Indicator
 *  Bool      mulNID;   Multiple NID Indicator
 *  Bool      homeReg; Home Registration Indicator.
 *  Bool      foreginSIDReg; Roamer SID Registration Indicator.
 *  Bool      foreginNIDReg; Roamer NID Registartion Indicator.
 *  Bool      paramReg;    Paramaeter change Registration Indicator.   
 *  Bool      powerUpReg;    Power UP Registration Indicator.
 *  U8        regPeriod;    Registration Indicator.
 *  U16       regZone;  Registration Zone.
 *  U8        totalZone;     Number of Zones to be retained.
 *
 **/
typedef struct wr1xRttCsfbRegInfo
{
  U16       sid;           /* System identifier */
  U16       nid;           /* Network identifier */
  Bool      mulSID;        /* Multiple SID storage indicator */
  Bool      mulNID;        /* Multiple NID storage indicator */
  Bool      homeReg;       /* Home reg indicator */
  Bool      foreignSIDReg; /* SID roamer registration indicator */
  Bool      foreignNIDReg; /* NID roamer registration indicator */
  Bool      paramReg;      /* Parameter change reg indicator */
  Bool      powerUpReg;    /* Power up registration indicator */
  U8        regPeriod;     /* Registration period */
  U16       regZone;       /* Registration zone number */
  U8        totalZone;     /* Total number of registration zone */
  U8        zoneTimer;     /* Zone timer */
  U8        powerDownRegPres; /* Power down registration indicator */
}Wr1xRttCsfbRegInfo;

typedef struct wr1xRttACBarringCfg
{
  U8 acBarring0to9_r9;
  U8 acBarring10_r9;
  U8 acBarring11_r9;
  U8 acBarring12_r9;
  U8 acBarring13_r9;
  U8 acBarring14_r9;
  U8 acBarring15_r9;
  U8 acBarringMsg_r9;
  U8 acBarringReg_r9;
  U8 acBarringEmg_r9;
}Wr1xRttACBarringCfg;

/**@brief
 *
 *WrHrpdPreRegInfo           preRegParam; HRPD pre registration parameter.
 * U8                         hrpdCellReselPres; Flag indicator for Cell
 *reselection parameter.
 * WrCDMA2000CellReselParam   cellResel; [O]
 * 
 **/

typedef struct wrHRPDParams
{
      WrHrpdPreRegInfo           preRegParam;
         U8                         hrpdCellReselPres;
            WrCDMA2000CellReselParam   cellResel;  /* Optional Param */
} WrHRPDParams;

/**@brief
 *
 * Bool                       csfbRegParamPres; 
 * Wr1xRttCsfbRegInfo         csfbRegParam;  [O] 
 * U8                         longCodeState[6]; [O] 42 bits,
 * ptional Param 
 * U8                         xRttCellReselPres;
 * WrCDMA2000CellReselParam   cellResel;  //CDMA2K
 * Reselection parameters. 
 *
 **/
typedef struct wrxRTTParams
{
  Bool                       csfbRegParamPres; /* presence of csfbReg */
  Wr1xRttCsfbRegInfo         csfbRegParam;     /* csfb registration params */
  U64                        longCodeState; /* 42 bits, Optional Param */ 
  Bool                       acBarringPres;     /* presence of ac-barring */
  Wr1xRttACBarringCfg        acBarring;         /* ac-Barring information */
} Wr1xRTTParams;


/**@brief
 * WrSysTimeInfoCDMA2000  sysTime;  Information on CDMA System time 
 *  Bool               searchWindowPres;   search window presence.
 *  U8                 searchWindSize;  search window for neighbouring
 *  pilot.
 *  Bool               hrpdParamPres;   hrpd param presence ind.   
 *  WrHRPDParams       hrpdParam; param applicable for  interworking
 *  with CDMA2k HRPD sys 
 *  Bool               XRTTParamPres;   1XRTT param presence ind.
 *  Wr1xRTTParams      XRTTparam; params applicable for
 * interworking with CDMA2K HRPD sys
 *WrSib8NonCriExt    nonCritiExt;  non critical params.
 **/
typedef struct wrSib8Params
{
  U8         siIndex;
  WrSysTimeInfoCDMA2000     sysTime;
  TknU8                    searchWindSize;
  Bool                      hrpdParamPres;
  WrHRPDParams              hrpdParam; 
  Bool                      XRTTParamPres;
  Wr1xRTTParams             XRTTparam;
  U8                         tReselection;
  Bool                       speedStatePres;
  U8                         sfMedium;
  U8                         sfHigh;
} WrSib8Params;

 /* This structure holds SIB7 parameter for GERAN */
typedef struct wrSib7Params
{
  U8			            siIndex;
  U8                    tReselectionGERAN;
  Bool                  tReselSpeedSFPres;
  LwrSpdStSclFactor     tReselGeranSFMedium;
  LwrSpdStSclFactor     tReselGeranSFHigh;
} WrSib7Params;


#define MAX_INTER_FREQ_NEIGH_CELLS  16
#define MAX_CELL_BLACK  16

/** @brief This enum describes the type of neighbour cell
 - WrIntraFreq  Intra frequency neighbour
 - WrInterFreq  Inter frequency neighbour
 - WrCDMA2kRAT  CDMA 2k neigbour
 */
/** @brief This enum describes the type of neighbour cell
 - WrIntraFreq  Intra frequency neighbour
 - WrInterFreq  Inter frequency neighbour
 - WrCDMA2kRAT  CDMA 2k neigbour
 - WrGeranFreq  GERAN Neighbor Frequency
 */
typedef enum
{
   WrIntraFreq = 0,
   WrInterFreq,
   WrCDMA2kRAT,
   WrUtraFreq,
   WrGeranFreq 
} WrNeighType;

#if 0
/* The following two macros are defined for data type WrEnbType which     */
/* classifies local eNB and the neighbour eNBs as MACRO or HOME eNBs      */
#define  WR_MACRO_ENB        ENB_ID_MACROENB_ID
#define  WR_HOME_ENB         ENB_ID_HOMEENB_ID
typedef  U8                  WrEnbType;
#endif
#define WR_MAX_GU_GRP_IDS      16
#define WR_MAX_NEIGH_ENBS      16
#define WR_X2AP_MAX_PEERS      32

/** @brief This enum defines the EnodeB duplex mode
 - VE_MODE_TDD    TDD cell
 - VE_MODE_FDD    FDD cell
 */
typedef enum
{
   VE_MODE_TDD = 0,
   VE_MODE_FDD
} WrDuplexMode;

/* pci        - this field stores physical cell id of the neighbor        */
/* cgi        - this field contains cell global identifier                */
/* enbType    - this field marks the neighbour as macro or home enb       */
/** @brief This structure contains the Intra frequency neigbour information 
 - U16          pci                          Physical cell ID
 - U32          cgi                          Cell global identifier
 - U16          tac                          Tracking area code
 - WrPlmnId     plmnIds[WR_MAX_PLMN_IDS]     Broadcast PLMNs
 - WrEnbType    enbType                      Macro/Home
 - U16          ulFreq                       UL Frequency
 - U16          dlFreq                       DL Frequency
 - U8           ulBw                         UL Bandwidth
 - U8           dlBw                         DL Bandwidth
 - CmTptAddr    addr                         IP address
 - U32          eGTPTunCntr                  GTP Tunnel counter
 - U8           x2State
 - WrDuplexMode duplexMode
 - U32          peerId
 - U8           trgrX2
 - UConnId      spConnId
 - UConnId      suConnId
 */
typedef struct wrIntraFreqNeighCell
{
   U16                       pci;
   U32                       cgi;
   U16                       tac;
   WrPlmnId                  plmnIds[WR_MAX_PLMN_IDS];
   WrEnbType                 enbType;
   U16                       ulFreq;
   U16                       dlFreq;
   U8                        ulBw;
   U8                        dlBw;
   CmTptAddr                 addr;
   U32                       eGTPTunCntr;
   U8                        x2State;
   WrDuplexMode              duplexMode;
   U32                       peerId;
   U8                        trgrX2;
   UConnId                   spConnId;
   UConnId                   suConnId;
} WrIntraFreqNeighCell;

/* This is yet to be completed                                            */
/** This structure contains Inter freq Neighbour cell's PCI
 - U16                       pci Physical cell ID
 */
typedef struct wrInterFreqNeighCell
{
   U16                       pci;
} WrInterFreqNeighCell;

/* This is yet to be completed                                            */
/** This structure contains CDMA 2K neigbour cell's PCI
 - U16                       pci Physical cell ID
 */
typedef struct wrCdma2000NeighCell
{
   U16            pci;
} WrCdma2000NeighCell;

typedef struct wrUtraFreqNeighCell
{
   U16        psc;
   U16         ulFreq;
   U16        dlFreq;
}WrUtraFreqNeighCell;

/** This structure contains Neigbour cell CB
 - CmLList                lnk          Link to Cmllist
 - WrNeighType            neighType    Neighbour type
 - WrIntraFreqNeighCell   intra        Intra Frequency neighbour
 - WrInterFreqNeighCell   inter        Inter Frequency neighbour
 - WrCdma2000NeighCell    cdma         CDMA 2k Neighbour
 */
typedef struct wrNeighCellCb
{
   CmLList                   lnk;
   WrNeighType               neighType;
   union
   {
      WrIntraFreqNeighCell   intra;
      WrInterFreqNeighCell   inter;
      WrCdma2000NeighCell    cdma;
      WrUtraFreqNeighCell    utra;
   } u;
} WrNeighCellCb;

/** This structure contains the list of Neighbours
 - CmLListCp     intra      List of Intra frequency neighbours
 - CmLListCp     inter      List of Inter Frequency neighbours
 - CmLListCp     cdma       List of CDMA neighbours
 - CmHashListCp  eutran     Hash List of Intra frequency neighbours
 - CmHashListCp  cdma1xRTT  Hash List of Inter Frequency neighbours
 - CmHashListCp  cdmaHRPD   Hash List of CDMA neighbours
 - CmHashListCp  utran      Hash List of UTRA  frequency neighbours
 */
typedef struct wrNeighCells
{
   CmLListCp                 intra;
   CmLListCp                 inter;
   CmLListCp                 cdma;

   CmHashListCp              eutran;
   CmHashListCp              cdma1xRTT;
   CmHashListCp              cdmaHRPD;
   CmHashListCp               utran;
} WrNeighCells;

/* TODO - These need to be set to the values as per specification         */
#define WR_MAX_PLMNS_PER_MME 32
#define WR_MAX_GRPS_PER_MME  256 /* According to spec it is 65535*/ 
#define WR_MAX_CODES_PER_MME 256 /* According to spec it is 256*/ 

#define WR_MAX_GUMMEI_PER_MME 256

#define WR_MME_INITED        0
#define WR_MME_IN_PROG       1
#define WR_MME_UP            2

/**
 * @brief This structure contains Inputs to Build Enb Reset Request Message 
 * @details These are the structure members
 *
 *  U32             causeType 
 *  U16             causeVal 
 *  U8              type                  Type of reset, partial or complete 
 *  U16             nmbUes                Number of UEs in case of partial reset
 *  WrEmmS1ConId    conIds[WR_MAX_MMES]   Ue ids incase of partial reset 
 *                                         
 */

typedef struct WrEmmS1Reset
{
   U16             causeType;  
   U16             causeVal;  
   U8              rstType;  
   U16             nmbUes;
   WrEmmS1ConId    conIds[WR_MAX_MMES];
}WrEmmS1Reset;

typedef struct wrAlarmVal
{
   U8      alarmCnt;
   Ticks   alarmTime; 
   U8      maxErrIndCnt;
   U8      maxResetCnt;
   U64     minErrIndTime;
   U64     minResetTime;
}WrAlarmVal;

typedef struct wrOvrldGummeiLst
{
   WrPlmnId         plmnId;
   U8               mmeCode;
   U32              mmeGroupId;
}WrOvrldGummeiLst;


/** This structure is the MME control block. 
 - WrMmeId   mmeId                     MME Index
 - U32       peerId                    Peer ID
 - U32       suConnId                  SU connId
 - U32       spConnId                  SP connId
 - CmTimer   timer                     Timer 
 - WrLiSapCb *s1apSap                  S1AP SAP
 - U8        state                     State of the MME
 - CmTptAddr mmeAddr                   IP address of the MME
 - U32       locEgtpTunCntr            Local EGTP Tunnel counter
 - U8        numPlmnIds                Number of PLMNs 
 - WrPlmnId  plmnIds[WR_MAX_PLMNS_PER_MME]  PLMNs
 - U16       numGrpIds                 Number of Group IDs
 - U16       groupIds[WR_MAX_GRPS_PER_MME] Group IDs
 - U16       numCodes                  Number MME Codes
 - U8        codes[WR_MAX_CODES_PER_MME]  MME Codes
 - U8        relCapacity               Relative capacity
 */
typedef struct wrMmeCb
{
   WrMmeId                   mmeId;
   U32                       peerId;
   U32                       suConnId;
   U32                       spConnId;
   CmTimer                   timer;
   WrLiSapCb                 *s1apSap;
   U8                        state;
   U8                        adminState;
   CmTptAddr                 mmeAddr[LWR_MAX_IP_PER_MME];
   U32                       locEgtpTunCntr;
   U8                        numPlmnIds;
   WrPlmnId                  plmnIds[WR_MAX_PLMNS_PER_MME];
   U16                       numGrpIds;
   U16                       groupIds[WR_MAX_GRPS_PER_MME];
   U16                       numCodes;
   U8                        codes[WR_MAX_CODES_PER_MME];
   U8                        relCapacity;
   Bool                      mmeOvldFlag;             /*!< MME Overload Flag */
   U32                       overloadAction;         /*!< MME Overload Action */
#ifdef MME_LDBAL
   U8                        dynCapacity; /*!<MME dynamic Capacity */
   F32                       selProb;     /*!<MME's Selection Probability --*/
#endif
   WrEmmS1Reset              resetRtxInputs; /*!< Inputs to retransmit S1 Reset */
   U16                       cfgUpdRtxCnt; /*!< CfgUpd Retransmission count */
   U16                       resetRtxCnt; /*!< Reset Retransmission count */
   WrAlarmVal                alarmVal;
   U32                       numOvrldGummei;
   WrOvrldGummeiLst          ovrldGummeiLst[WR_MAX_GUMMEI_PER_MME];
} WrMmeCb;

/* suConnId is same the index for MME. This allows efficient search.      */
/** This structure contains the list of MME configured for this ENodeB
 - U8       numMmes              Number of MMEs
 - WrMmeCb  *mmes[WR_MAX_MMES]   List of MMEs
 */
typedef struct wrMmeCont
{
   U8                       numMmes;
   WrMmeCb                  *mmes[WR_MAX_MMES];
} WrMmeCont;

#if 0
/** This structure contains UE's paging information 
 - CmLList       lnk       Link into the cellCb's list
 - WrSTmsi       sTMSI     S-TMSI of the UE
 - U16           crnti     CRNTI of the UE
 - U16           T         Value of T
 - U8            N         Value of N
 - U8            Ns        Vakue of Ns
 - U32           ue_Id     UE-ID
 - U16           pf        Paging frame  
 - U8            po        Paging Occassion
 - NhuDatReqSdus *pagMsg   Paging message
 - U8            domIndType Domain Type CS/PS
 */
typedef struct wrUePagingInfo
{
   CmLList       lnk;
   WrSTmsi       sTMSI;
   U16           crnti;
   U16           T;
   U8            N;
   U8            Ns;
   U32           ue_Id;
   U16           pf;
   U8            po;
   NhuDatReqSdus *pagMsg;
   U8            domIndType;
} WrUePagingInfo;
#endif

#define WR_PBM_MAX_SCHED_ENTRYS    1024 

/** @brief This structure is used to store the Paging and Broadcasting module
 * Inforamtion in each Cell
 */
typedef struct wrPbmCb
{
   Bool            siModPres; /* System Information chage is occured. 
                                 Need to send paging msg with SI change   */
   Bool            etwsIndPres; /* Warning message is received.
                                    Need to send Paging msg with ETWS-ind */
   Bool            cmasIndPres; /* PH04_CMAS : Warning message is received.
                                   Need to send Paging msg with CMAS-ind */
   U16             curSfn;
   U16             repeatNumPagCycles;   /*!< OAM should cfg this Value */
   CmLListCp       *pbmSchedInfo[WR_PBM_MAX_SCHED_ENTRYS]; 
   U8              **allPfsAndPos;
   U8              numSIs;   /* num of pending SI etries                  */
   WrSISchdInfo    pendSiInfo[WR_PBM_MAX_NUM_SIS]; /* Pending SI info             */
   Bool            pendReCalcAllPfsAndPos;
   U8              pendDfltPagingCycle;
   U8              pendnB;
   U8              pendModifPeriodCoeff;
} WrPbmCb;

typedef struct wrUeTimers
{
   U16                       t302;  
   U16                       t304;
   U16                       ccoT304;
   U16                       t320;
} WrUeTimers;

typedef struct wrEmmCellTmrCfg
{
   U32                       rrcConnectionSetupTmr;
   U32                       rrcConnectionReconfigTmr;
   U32                       rrcConnectionReEstablishTmr;
   U32                       rrcSecurityModeCommandTmr;
   U32                       rrcUeCapabilityEnquiryTmr;
   U32                       rrcConnectionReleaseTmr;
   U32                       rrcHandoverPrepTmr;
   U32                       rrcUeInformationRequestTmr;
   U16                       s1InitialContextSetupTmr;
   U16                       s1ErabReleaseIndicationTmr;
   U16                       s1UeContextReleaseTmr;
   U16                       s1PathSwitchTmr;
   U16                       s1HandoverPrepTmr;
   U16                       s1RelocOverallTmr;
/* S1 Handover Preparation timer for IRAT handover to UTRA */  
   U16                       s1IRatHandoverPrepTmr;  
   /* S1 Handover overall timer for IRAT handover to UTRA */ 
   U16                       s1IRatRelocOverallTmr;          
   U16                       s1HandoverCancelTmr;
   U16                       x2HandoverPrepTmr;
   U16                       x2RelocOverallTmr;
   U16                       x2StoreUeContextTmr;
   U16                       x2SnStatusTransferTmr;
   U16                       internalSubCellChangeTmr;
   U16                       internalSubCellPdcpControlTmr;
   U16                       ehrpdOptHoCompleteTmr;
   U16                       intraEnbHoCompleteTmr;
   U16                       measurementReportToWaitTmr;
   U16                       internalResourceSetupTmr;
   U16                       internalResourceReleaseTmr;
   U16                       internalSecurityControlTmr;
   U16                       internalRrcDataTransferTmr;
   U16                       internalForwardingPathSetupTmr;
   U16                       internalReestablishControlTmr;
   U16                       internalBufferFlushTmr;
   U16                       internalDataTransferStartTmr;
   U16                       internalDataForwardEndIndTmr;
   U16                       internalReestablishTimeToWaitTmr;
   U16                       internalUserInactivityTmr;
   U16                       internalSignalingInactivityTmr;
   U16                       internalUnassignedTmr;
   U16                       internalNoWaitRetryTmr;
   U16                       internalRrcResetTmr;
   U16                       internalEnbResetTmr;
   U16                       internalSolicitMeasurementTmr;
   U16                       internalRtdInfoReqTmr;
} WrEmmCellTmrCfg;

typedef struct wrEmmcellReCfgInfo
{
  U8                         cellReCfgBitMsk;
  RgrCellRecfg               schReCfg;
  CtfCellRecfgInfo           phyReCfg;
  NhuCellReCfgInfo           rrcReCfg;
} WrEmmcellReCfgInfo;

#define WR_MAX_SIBS          12 
/* numPlmnIds - This is the number of PLMN IDs broadcast                  */
/* plmnIds    - Array of PLMN IDs broadcast. First is primary             */
/* cellId     - This field carries cell Id used within the stack          */
/** This structure is the cell control block
   U16               cellId                   Cell Id
   U16               dlCarrierFreq            DL EARFCN
   U16               physCellId               Physical cell ID
   U16               dlCyclicprefix           DL cyclic prefix
   U8                numAntPorts              Number of Antenna ports
   WrMibParams       mib                      Master information block
   WrSib1Params      sib1                     System information block type 1
   WrSib2Params      sib2                     System information block type 2
   WrSib3Params      sib3                     System information block type 3
   WrSib4Params      sib4                     System information block type 4
   WrSib6Params      sib6                    System information block type 6
   WrSib9Params      sib9                    System information block type 9
   WrSib5Params      sib5                     System information block type 5
   U8                state                    State
   WrSibCfg          sibsCfg[WR_MAX_SIBS]     SIBs Configured by OAM
   U8                numSibsCfg               Num of OAM Configured SIBs 
   WrPbmCb           pbmCb
   U8                numOfCfgActiveEaids      Number of Active Emergency Area IDs
                                              configured by OAM
   WrActCfgEaid      eaidLst[WR_PWS_MAX_NUM_EAIDS] 
                                             OAM configured active 
                                             Emergency Area IDs
   WrCfgSIB10Info    sib10Info               Handling of SIB10 info given by OAM
   WrPwsWarningInfo  *warningInfo            WarningInfo contains both primary
                                             and secondary information
   U16               warMsgSegmentSize       Waring message size is configured by OAM for PWS
   U16               sfn                     System frame number
   U8                modType           
   U16               duplexMode              TDD/FDD
   U8                maxUeSupp               Maximum number of UEs
   U8                numUeCfg                Current number of configured UEs
   WrNeighCells      neighCells              List of Neighbour cells
   U16               wrDfltMacRntisize       Max RNTIs managed by MAC 
   U16               wrDfltStartRnti         Start RNTI value for MAC
   U8                wrDfltNumRaPreamble     Number of RA Preambles 
                                             supported at MAC 
   U32               rsrpTrshldRange         Serving Cell threshold RSRP value             
   CmLListCp         pendPagUesLst           Pending Paging UEs WrUePagingInfo 
   CmLListCp         **ueCbLst               List of UE CBs
   WrCellNrCb        *nrCb;                  pointer to Neighbor Relation 
                                             control Block   
 */

typedef struct wrCellStat
{
   WrRrcSts  wrRrcStat;
   WrRrcReEstabSts wrRrcReEstabSts;
   WrErabSts wrErabStat;
   WrCallSts wrCallStat;
   U32       wrPagStat;
   WrHoSts   wrHoStat;
   WrSigSts  wrSigSts;
   WrUeCtxtRelSts wrUeCtxtRelSts;
}WrCellStat;

typedef struct wrSchedulerCfg
{
   U8    ulSchType;
   U8    dlSchType;
   U8    dlfsSchdType;
   U32   pfsDlTptCoefficient;    /*!< Downlink Throughput co-effiecient*/
   U32   pfsDlFairCoefficient;   /*!< Downlink Fairness co-effiecient*/
   U32   pfsUlTptCoefficient;    /*!< Uplink Throughput co-effiecient*/
   U32   pfsUlFairCoefficient;   /*!< Uplink Fairness co-effiecient*/
   U32   pfsDlQciWgt[LWR_MAX_NUM_QCI];  /*!< Downlink Qci Weights*/
   U32   pfsUlQciWgt[LWR_MAX_NUM_QCI];  /*!< Uplink Qci Weights*/
}WrSchedulerCfg;

typedef struct wrAddPwrCntrlParams
{
   Bool deltaMCSenabled;
   S32  p0UePUSCH;
   S32  p0UePUCCH;
   Bool accumulationEnabled;
   U32  pSRSoffset;
   U32  filterCoefficient;
}WrAddPwrCntrlParams;

typedef enum wrEmmModuleStat   /* !< This enum houses various statistics classes.*/
{
   WR_EMM_RRC_STATS = 1,      /* !< RRC Related Statistics*/
   WR_EMM_RRC_REEST_STATS,    /* !< RRC RE-Establishment Statistics*/
   WR_EMM_ERAB_STATS,         /* !< ERAB Statistics*/
   WR_EMM_PAG_STATS,          /* !< Paging Statistics*/
   WR_EMM_CALL_STATS,         /* !< Call Statistics*/
   WR_EMM_SIG_STATS,          /* !< Signalling Statistics*/
   WR_EMM_UE_CTXT_REL_STAT,   /* !< UE Context Statistics*/
   WR_EMM_UE_PRB_QCI,         /* !< PRB Statistics per QCI*/
   WR_EMM_UE_PRB_AVG,         /* !< PRB Statistics per QCI*/
   WR_EMM_ACTV_UE_QCI,        /* !< Active UE Statistics per QCI*/
   WR_EMM_PREAMBLE,           /* !< Preamble */
   WR_EMM_ACTUE_CNT,          /* !< Active UE Count */
   WR_EMM_MAX                 /* !< Max Enum */
}WrEmmModuleStat;


typedef enum wrEmmPrbStat
{
   WR_EMM_PRB_UL,
   WR_EMM_PRB_DL,
   WR_EMM_PRB_MAX   
}WrEmmPrbStat;

typedef enum wrEmmActUeCnt
{
   WR_EMM_TOT_ACTV_UE
}WrEmmActUeCnt;


typedef enum wrEmmSubActUeCnt
{
   WR_EMM_TOT_ACTV_UE_MAX

}WrEmmSubActUeCnt;


typedef enum wrEmmPrbAvg
{
   WR_EMM_UE_PRB_AVG_MAX

}WrEmmPrbAvg;

typedef enum wrEmmPreambleSubTypeStat
{
   WR_EMM_PREAMBLE_MAX

}WrEmmPreambleSubTypeStat;



typedef enum wrEmmPreambleStat
{
   WR_EMM_Preamble_DEDICATED = 1,
   WR_EMM_Preamble_GRPA,
   WR_EMM_Preamble_GRPB
}WrEmmPreambleStat;


typedef enum wrEmmPrbSubStat
{
   WR_EMM_PRB_QCI1 = 1,
   WR_EMM_PRB_QCI2,
   WR_EMM_PRB_QCI3,
   WR_EMM_PRB_QCI4,
   WR_EMM_PRB_QCI5,
   WR_EMM_PRB_QCI6,
   WR_EMM_PRB_QCI7,
   WR_EMM_PRB_QCI8,
   WR_EMM_PRB_QCI9,
   WR_EMM_PRB_QCI_SUM
}WrEmmPrbSubStat;

typedef enum wrEmmSigStat
{
   WR_EMM_SIG_ATTEMPT =1,     /* !< Total Signalling Attempts : Mapping S1SIG.ConnEstabAtt*/
   WR_EMM_SIG_SUCCESS,        /* !< Total Sucessfull Attempts : Mapping S1SIG.ConnEstabSucc*/
   WR_EMM_SIG_MAX             /* !< Max Enum */
}WrEmmSigStat;


typedef enum wrEmmUeCtxtRelStat
{
   WR_EMM_UE_CTXT_REL_REQ =1,   /* !< Total UE Context Release Attempt : Mapping UECNTX.RelReq.Sum*/
   WR_EMM_UE_CTXT_REL_SUCCESS,  /* !< Total UE Context Release Sucessfull : Mapping UEContext.RelSuccNbr*/
   WR_EMM_UE_CTXT_REL_MAX       /* !< Max Enum */
}wrEmmUeCtxtRelStat;


typedef enum wrEmmRrcStat
{
   WR_EMM_RRC_CON_ATTEMPT = 1,  /* !< Total RRC Connection Attempted by UE : Mapping RRC.AttConnEstab.Sum*/
   WR_EMM_RRC_CON_SUCCESS,      /* !< Total RRC Connection Request Resulted in Success : Mapping RRC.SuccConnEstab.Sum*/
   WR_EMM_RRC_CON_FAIL,         /* !< Total RRC Connection Request Resulted in failure : Mapping RRC.FailConnEstab.Sum*/
   WR_EMM_RRC_CON_FAIL_DEGRAD,  /* !< Total RRC Connection Request Resulted in failure due to degraded Radio Conditions :
                                  Mapping RRC.FailConnEstab.DegradedRadio.Sum */
   WR_EMM_RRC_CON_MAX           /* !< Max Enum*/
}WrEmmRrcStat;


typedef enum wrEmmRrcReEstabStat
{
   WR_EMM_RRC_REESTAB_CON_ATTEMPT = 1, /*!< Total RRC Re-Establishment Attempted by UE : Mapping RRC.ConnReEstabAtt.Sum*/
   WR_EMM_RRC_REESTAB_CON_SUCCESS,     /*!< Total RRC Re-Establishment Request Resulted in Success : Mapping RRC.ConnReEstabSucc.Sum*/
   WR_EMM_RRC_REESTAB_CON_FAIL,        /*!< Total RRC Re-Establishment Request Resulted in Failure : Mapping RRC.ConnReEstabFail.Sum*/
   WR_EMM_RRC_REESTAB_CON_FAIL_DEGRAD, /*!< Total RRC Re-Establishment Request Resulted in failure due to Degraded radio Conditions
                                         : Mapping No Mapping*/
   WR_EMM_RRC_REESTAB_CON_MAX          /*!< Max Enum*/
}WrEmmRrcReEstabStat;


typedef enum wrEmmErabStat
{
   WR_EMM_ERAB_DEFAULT = 1,           /*!< Statistics related to Default ERAB*/
   WR_EMM_ERAB_DEDICATED,             /*!< Statistics related to Default ERAB*/
   WR_EMM_ERAB_MODIFY,                /*!< Statistics related to ERAB Modification */
   WR_EMM_ERAB_RELEASE_ENB,           /*!< Statistics related to ERAB Release ENB Initiated */
   WR_EMM_ERAB_RELEASE_MME,           /*!< Statistics related to ERAB Release MME Initiated */
   WR_EMM_ERAB_MAX                    /*!< Max Enum */
}WrEmmErabStat;

typedef enum wrEmmPagStat
{
   WR_EMM_PAG_TOT = 1,                /*!< Total Paging Request Received at ENB */
   WR_EMM_PAG_MAX                     /*!< Max Enum */
}WrEmmPagStat;


typedef enum wrEmmcallStat
{
   WR_EMM_CALL_TOT = 1,              /*!< Total Call Attempted at EnodeB */
   WR_EMM_CALL_ACT,                  /*!< Total Active Call  */
   WR_EMM_CALL_MAX                   /*!< Max Enum */
}WrEmmcallStat;


typedef enum wrEmmRrcFailDegRad     /*!< This enum houses all possible Mobile Originated RRC Connection Req */
{
   WR_EMM_RRC_EMERGENCY = 0,        /*!< RRC Connection Request with Emergency Reason : No Mapping */
   WR_EMM_RRC_HIGH_PRIORITY_ACCESS, /*!< RRC Connection Request with High Priority Access : No Mapping */
   WR_EMM_RRC_MT_ACCESS,            /*!< RRC Connection Request with Mobile Terminated Access : No Mapping */
   WR_EMM_RRC_MO_SIGNALLING,        /*!< RRC Connection Request with Mobile Originated Signalling : No Mapping */
   WR_EMM_RRC_MO_DATA,              /*!< RRC Connection Request with Mobile Originated Data : No Mapping */
   WR_EMM_RRC_DELAY_TOLERANT_ACCESS_V1020, /*!< RRC Connection Request  : No Mapping */
   WR_EMM_RRC_MAX                   /*!< Max Enum */
}WrEmmRrcFailDegRad;


typedef enum wrEmmRrcReEstabFail    /*!< This enum houses all possible reason for RRC-Re Establishment */
{
   WR_EMM_RRC_REESTAB_RECFG =0,     /*!< RRC-Re Establishment Request due to Re-Cfg Failure : No Mapping */
   WR_EMM_RRC_REESTAB_HO,           /*!< RRC-Re Establishment Request due to HO Failure : No Mapping */
   WR_EMM_RRC_REESTAB_OTHR,         /*!< RRC-Re Establishment Request due to Other Failure : No Mapping */
   WR_EMM_RRC_REESTAB_MAX           /*!< Max Enum */
}WrEmmRrcReEstabFail;


typedef enum wrEmmErabSubType       /*!< This enum houses all possible Sub Statistics for ERAB */
{
   WR_EMM_ERAB_ATTEMPT = 1,         /*!< Attempt : Maps to ERAB.EstabInitAttNbr.QCI.Sum, ERAB.EstabAttNbr.QCI.Sum
                                     ERAB.RelEnbNbr.QCI.Sum, ERAB.RelAttNbr.QCI.Sum, ERAB.ModQoSAttNbr.QCI.Sum*/

   WR_EMM_ERAB_SUCCESS,             /*!< Success : Maps to ERAB.EstabInitSuccNbr.QCI.Sum, ERAB.EstabSuccNbr.QCI.Sum,
                                     ERAB.RelSuccNbr.QCI.Sum, ERAB.ModQoSSuccNbr.QCI.Sum*/

   WR_EMM_ERAB_QCI_1,               /*!< No of Procedure initiated for ERAB with QCI=1. : Mapping All ERAB Procedure
                                     Creation: Default, Dedicated, Modification, Release: Enb Initiated, MME Initiated*/

   WR_EMM_ERAB_QCI_2,               /*!< No of Procedure initiated for ERAB with QCI=2. : Mapping All ERAB Procedure
                                     Creation: Default, Dedicated, Modification, Release: Enb Initiated, MME Initiated*/

   WR_EMM_ERAB_QCI_3,               /*!< No of Procedure initiated for ERAB with QCI=3. : Mapping All ERAB Procedure
                                     Creation: Default, Dedicated, Modification, Release: Enb Initiated, MME Initiated*/

   WR_EMM_ERAB_QCI_4,               /*!< No of Procedure initiated for ERAB with QCI=4. : Mapping All ERAB Procedure
                                     Creation: Default, Dedicated, Modification, Release: Enb Initiated, MME Initiated*/

   WR_EMM_ERAB_QCI_5,               /*!< No of Procedure initiated for ERAB with QCI=5. : Mapping All ERAB Procedure
                                     Creation: Default, Dedicated, Modification, Release: Enb Initiated, MME Initiated*/

   WR_EMM_ERAB_QCI_6,               /*!< No of Procedure initiated for ERAB with QCI=6. : Mapping All ERAB Procedure
                                     Creation: Default, Dedicated, Modification, Release: Enb Initiated, MME Initiated*/

   WR_EMM_ERAB_QCI_7,               /*!< No of Procedure initiated for ERAB with QCI=7. : Mapping All ERAB Procedure
                                     Creation: Default, Dedicated, Modification, Release: Enb Initiated, MME Initiated*/

   WR_EMM_ERAB_QCI_8,               /*!< No of Procedure initiated for ERAB with QCI=8. : Mapping All ERAB Procedure
                                     Creation: Default, Dedicated, Modification, Release: Enb Initiated, MME Initiated*/

   WR_EMM_ERAB_QCI_9,               /*!< No of Procedure initiated for ERAB with QCI=9. : Mapping All ERAB Procedure
                                     Creation: Default, Dedicated, Modification, Release: Enb Initiated, MME Initiated*/

   WR_EMM_ERAB_FAIL_RADIORESOURCESNOTAVAILABLE,      /*!< No of ERAB Procedure failed Radio resource unavaliablility
                                                     Mapping All ERAB Procedure Creation: Default, Dedicated, Modification,
                                                     Release: Enb Initiated, MME Initiated*/

   WR_EMM_ERAB_FAIL_REDUCELOADINSERVINGCELL,         /*!< No of ERAB Release Procedure due to load reduction of Serving Cell :
                                                     Mapping ERAB.RelEnbNbr.ReduceLoadInServingCell*/

   WR_EMM_ERAB_FAIL_FAILUREINTHERADIOINTERFACEPROCEDURE,/*!< No of ERAB Release Procedure due to Radio Interface Procedure failure :
                                                     Mapping ERAB.RelEnbNbr.FailureInTheRadioInterfaceProcedure*/

   WR_EMM_ERAB_FAIL_RELEASEDUETOEUTRANGENERATEDREASONS,/*!< No of ERAB Release Procedure due to EUTRANGENERATEDREASONS :
                                                     Mapping ReleaseDueToEUTRANGeneratedReasons*/

   WR_EMM_ERAB_FAIL_USERINACTIVITY,                  /*!< No of ERAB Release Procedure due to User Inactivity :
                                                     Mapping ERAB.RelEnbNbr.UserInactivity*/

   WR_EMM_ERAB_FAIL_RADIOCONNECTIONWITHUELOST,       /*!< No of ERAB Release Procedure due to RLF :
                                                     Mapping ERAB.RelEnbNbr.RadioConnectionWithUELost*/

   WR_EMM_ERAB_FAIL_OAMINTERVENTION,                 /*!< No of ERAB Release Procedure due to OAM Intervention :
                                                     Mapping ERAB.RelEnbNbr.OAMIntervention*/
   WR_EMM_ERAB_SUBTYP_MAX
}WrEmmErabSubType;

typedef enum wrEmmPagSubType
{
   WR_EMM_PAG_SUBTYP_MAX = 1
}WrEmmPagSubType;


typedef enum wrEmmSigSubType
{
   WR_EMM_SIG_SUBTYP_MAX = 1
}WrEmmSigSubType;

typedef enum wrEmmUeCtxtRelSubType
{
   WR_EMM_UE_CTXT_REL_SUBTYP_MAX = 1
}WrEmmUeCtxtRelSubType;


#define WR_EMM_DFLT_POS_STEP 1
#define WR_EMM_DFLT_NEG_STEP 1
#define QCI_OFFSET 2

/*CTF power cfg*/
typedef struct wrLowPwrCfg
{
   S8    lowRefSigPwr;
   S16   lowPilotSigPwr;
   S16   lowPriSigPwr;
   S16   lowSecSigPwr;
}WrLowPwrCfg;

typedef struct wrHighPwrCfg
{
   S8    highRefSigPwr;
   S16   highPilotSigPwr;
   S16   highPriSigPwr;
   S16   highSecSigPwr;
}WrHighPwrCfg;
typedef struct wrShutdownPwrCfg
{
   WrLowPwrCfg   lwPwrCfg;
   WrHighPwrCfg  hiPwrCfg;
}WrShutdownPwrCfg;

typedef struct wrDlRlcModeInfo
{
   U8  amRlcSrbmaxRetxThresh;
   U16 amRlcSrbmaxpollByte;
   U16 amRlcSrbpollPdu;
   U16 amRlcSrbtPollRetx;
   U8  amRlcDrbmaxRetxThresh;
   U16 amRlcDrbmaxpollByte;
   U16 amRlcDrbpollPdu;
   U16 amRlcDrbtPollRetx;
   U8  umRlcsnFieldLen;
   U8  srbtReordring;
   U8  drbtReordring;
}WrDlRlcModeInfo;

typedef struct wrUlRlcModeInfo
{
   U8  amRlcSrbtStatusProh;
   U8  amRlcSrbtReordring;
   U8  amRlcDrbtStatusProh;
   U8  amRlcDrbtReordring;
   U8  umRlcsnFieldLen;
   U8  srbtReordring;
   U8  drbtReordring;
}WrUlRlcModeInfo;

typedef struct wrRlcCfgGrp
{
   WrDlRlcModeInfo dlModeInfo;
   WrUlRlcModeInfo ulModeInfo;
}WrRlcCfgGrp;

typedef struct wrRgrCfgGrp
{
   U16 dfltStartRnti;
   U16 dfltRntiSize;
   U8  dfltCfiCfg;
   U8  dfltTrgUlCqi;
   U16 dfltBcchPchRaCodeRate;
   U16 dfltPdcchCodeRate;
   U8  dfltULCmnCodRateCcchCqi; 
   U8  dfltDLCmnCodRateCcchCqi; 
   U8  dfltIsDlFreqSel;
   U8  dfltThresholdCqi;
   U8  dfltMaxDlHqTx;
   U8  dfltMaxMsg4HqTx;
   U8  dfltMaxPucchRbs;  
}WrRgrCfgGrp;

typedef enum {
   WR_CTF_SC_SPACING_15KHZ,
   WR_CTF_SC_SPACING_7DOT5KHZ
} WrCtfSCSpacing;

typedef struct wrCtfCfgGrp
{
   WrCtfSCSpacing    dfltScSpacing;
   U32             dfltOpMode;
   U32             dfltTagPeriod;
}WrCtfCfgGrp;

typedef struct wrCellSchdPwrCfg
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
}WrCellSchdPwrCfg;

typedef struct wrCellSchdRachCfg
{
   U8 preambleFormat;
   U8 raOccSize;
   U8 raOccSfnEnum; 
   U16 prachResource;
}WrCellSchdRachCfg;

typedef struct wrCellSchdGenCfg
{
   U8                  macInst;
   U8                  maxUePerUlSf;   /*!< Maximum UEs that may be scheduled
                                            per uplink subframe. Currently this is
                                           unused parameter */
   U8                  maxUePerDlSf;   /*!< Maximum UE to be considered for DL 
                                            scheduling in a TTI.Currently this is
                                            unused parameter */
   U8                  maxUlBwPerUe;   /*!< Maximum number of RBs that can be  
                                           allocated to an UE in an UL subframe
                                           Maximum value is defined by 
                                           RG_SCH_CMN_MAX_UL_BW_PER_UE in   
                                           rg_env.h. This can be modified as
                                           per need basis */
   U8                  maxDlBwPerUe;   /*!< Maximum number of RBs that can be    
                                        allocated to an UE in an DL subframe  
                                        Maximum value is defined by     
                                        RG_SCH_CMN_MAX_DL_BW_PER_UE in
                                             rg_env.h. This can be modified as 
                                         per need basis */
   U8                  maxDlRetxBw;    /*!< Maximum number of RBs that can be     
                                        allocated for retransmission in DL
                                        Maximum value is defined by 
                                        RG_SCH_CMN_MAX_DL_RETX_BW in   
                                        rg_env.h. This can be modified as   
                                        per need basis */
   U8                  maxDlUeNewTxPerTti; /*!< Maximum number of UEs that can  
                                            be scheduled for a new DL    
                                            transmission in a TTI. Value should
                                            be configured by the user as per  
                                            specific needs */ 
  U8                  maxUlUeNewTxPerTti;  /*!< Maximum number of UEs that can */
  Bool                cellModSchm;
            
}WrCellSchdGenCfg;

/* SPS changes starts */
typedef struct wrEmmSpsCfgGrp
{
   Bool bIsSpsEnabled;
   U16  maxDlSpsUePerTti;
   U16  maxUlSpsUePerTti;
   U8   maxDlBwInRbs;
}WrEmmSpsCfgGrp;
/* SPS changes ends */

typedef struct wrCellSchdCfg
{
   U8 srsBwEnum;
   U8  prdEnum;
   U8 modPrd;
   U8  siWinSize;  /*!< SI window size */
   U8  retxCnt;    /*!< Retransmission count */
   WrCellSchdPwrCfg pwrCfg;
   Bool    preamPres;      /*!< Indicates if other configuration fields are valid */
   U8      preamStart;     /*!< Start Preamble ID for the range managed by MAC */
   U8      preamSize;      /*!< Indicates contiguous range of premables managae*/
   WrCellSchdRachCfg rachCfg;
   WrCellSchdGenCfg  genCfg;
   /* SPS changes starts */
   WrEmmSpsCfgGrp    spsCfg;
   /* SPS changes ends */
   Bool    isDynCfiEnb;
}WrCellSchdCfg;

typedef struct wrCdma2k1xMobParams
{
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
   Bool mobQos;            /* QoS record */
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
   Bool tzkModeSup;       /* Tracking zone mode indicator */
   Bool tzkIdIncl;
   Bool tzkId;            /* Tracking zone identifier */
   Bool pilRepIncl;
   Bool pilRep;           /* Pilot reporting indicator */
   Bool sdbSupIncl;
   Bool sdbSup;           /* Short data burst supported indicator */
   Bool autoFcsoAllIncl;
   Bool autoFcsoAll;      /* Autonomous fast call setup order allowed ind */
   Bool sdbInRcnmIncl;
   Bool sdbInRcnm;        /* Short data burst allowed in recon msg ind */
   Bool fpcFchIncl;       /* Fundamental Channel included indicator */ 
   U8   fpcFchInitSetptRC3; /* Initial Fundamental Channel outer loop
                               Eb/Nt setpoint with RC3 */
   U8   fpcFchInitSetptRC4; /* Initial Fundamental Channel outer loop
                               Eb/Nt setpoint with RC4 */
   U8   fpcFchInitSetptRC5; /* Initial Fundamental Channel outer loop
                               Eb/Nt setpoint with RC5 */
   U8   fpcFchInitSetptRC11; /* Initial Fundamental Channel outer loop
                                Eb/Nt setpoint with RC11 */
   U8   fpcFchInitSetptRC12; /* Initial Fundamental Channel outer loop
                                Eb/Nt setpoint with RC12 */
   Bool tAddIncl;
   U8   tAdd;                /* Pilot detection threshold */          
   Bool pilotIncIncl;
   U8   PilotInc;           /* Pilot PN sequence offset index increment */
   Bool randIncl;
   U32  rand;             /* Random challenge */
   Bool lpSecIncl;
   U8   lpSec;             /* Number  of  leap  seconds */
   Bool ltmOffIncl;
   U8   ltmOff;           /* Offset of local time from system time */
   Bool dayltIncl;
   Bool daylt;            /* Daylight savings time indicator */
   Bool gcsnaL2ActTimerIncl;
   U8   gcsnaL2ActTimer;  /* GCSNAL2AckTimer */
   Bool gcsnaSeqCtxtTimerIncl;
   U8   gcsnaSeqCtxtTimer; /* GCSNASequenceContextTimer */
   U8   reserved;
}WrCdma2k1xMobParams;

/* Added structure to store CSFB configuration specific to 1xRtt */
typedef struct wrCsfbCdma1xRttCfgParams
{
   TknStrOSXL          sectorIdPtr; /* Referenced Cell Id/Sector id */
   WrCdma2k1xMobParams mobParams;   /* Mobility parameters for Cdma2000 */
}WrCsfbCdma1xRttCfgParams;

/* Added structure to store CSFB related configuration information*/
typedef struct wrCsfbCfgParams
{
   U8                  csfbToUtraMeasSup; /* Indicates 0 - withoutMeas, 1 withMeas */
   U8                  csfbToCdmaMeasSup; /* Indicates 0 - withoutMeas, 1 withMeas */
   U8                  eCsfbMeasSup; /* Indicates 0 - notSuporrted, 1 withoutMeas, 2 withMeas */
   Bool                dualRxSup;    /* Indicates whether Dual-Rx is supported */
   Bool                dualRxTxSup;    /* Indicates whether Dual-Rx-Tx is supported */
   Bool                cdma1xRttSup; /*indicates whether 1xRTT is supported or not */
   WrCsfbCdma1xRttCfgParams cdma1xrttCsfbCfg; /* CSFB configuration specific
                                                 to CDMA 1xRtt */
}WrCsfbCfgGrp;

/* CSG_DEV start */
typedef enum
{
   WR_EMM_NBR_CSG_ID_NOT_DISCOVERED,
   WR_EMM_NBR_CSG_ID_DISCOVERERY_ON_GOING,
   WR_EMM_NBR_CSG_ID_DISCOVERED
} WrNbrCsgIdSts;

typedef enum
{
   WR_UMM_PREF_HO_TGT_NO_PREFERENCE,
   WR_UMM_PREF_HO_TGT_OPEN,
   WR_UMM_PREF_HO_TGT_CLOSED,
   WR_UMM_PREF_HO_TGT_HYBRID
} WrHoTgtPref;

typedef enum
{
   WR_EMM_CELL_ACCESS_MODE_NOT_DETERMINED,
   WR_EMM_CELL_ACCESS_MODE_DETERMINED
} WrNbrAccessModeSts;

typedef enum
{
   WR_EMM_ANR_REPORT_CGI,
   WR_EMM_ANR_REPORT_CSG
} WrEmmReportType;
/* CSG_DEV end */

/*UE history info - start*/
typedef enum wrEmmCellType
{
 	WR_EMM_CELL_TYPE_VERY_SMALL,
 	WR_EMM_CELL_TYPE_SMALL,
 	WR_EMM_CELL_TYPE_MEDIUM,
 	WR_EMM_CELL_TYPE_LARGE
}WrEmmCellType;
/*UE history info - end*/

/**
 * @brief
 * This structure store the number of RRC reconfiguration request sent
 * for different UEs whose PA got modified by SON
 * */

typedef struct wrSonPaReCfg
{
   U32   NumPaReCfg;

}WrSonPaReCfg;

/**
 * @brief
 * This structure contains the SON configured scheduler
 * related configuration. SON configures PRB configuration and
 * cell edge and non cell edge UE PA values. This structure also
 * store the number of RRC reconfiguration request sent for different
 * UEs whose PA got modified by SON
 * */

typedef struct wrSonCfg
{
   LwrSonSchdCfg                schdCfg;
   WrSonPaReCfg                 paReCfg;
}WrSonCfg;


typedef struct WrCellCb
{
   U16                       cellId;
   U16                       dlCarrierFreq;
   U16                       physCellId;
   U16                       dlCyclicprefix;
   U8                        numAntPorts;
   WrMibParams               mib;
   WrSib1Params              sib1;
   WrSib2Params              sib2;
   WrSib3Params              sib3;
   WrSib4Params              sib4;
   WrSib5Params              sib5;
   WrSib6Params 	           sib6;
   WrSib7Params              sib7;/*GERAN*/ 
   WrSib8Params              sib8; 
   WrSib9Params              sib9;
   WrCellUlSchdCfgGrp        ulSchdCfgGrp;
   WrCellDlSchdCfgGrp        dlSchdCfgGrp;
   WrCellState               cellState;
   WrSibCfg                  sibsCfg[WR_MAX_SIBS];
   U8                        numSibsCfg;    
   WrPbmCb                   pbmCb; 
   U8                        numOfCfgActiveEaids;
   WrActCfgEaid              eaidLst[WR_PWS_MAX_NUM_EAIDS];
   WrCfgSIB10Info            sib10Info;      
   WrPwsWarningInfo          *warningInfo;  
   /*!< Specifies the warning message segment length range is 0 to 217. */
   U16                       warMsgSegmentSize; 
   U16                       sfn;
   U8                        modType;
   U16                       duplexMode;
   U8                        maxUeSupp;
   U8                        numUeCfg;
   WrNeighCells              neighCells;
   U8                        numGuGrpIds;
   WrGuGrpIdLst              guGrpIdLst[WR_MAX_GU_GRP_IDS];
#if 0
   U16                       wrDfltMacRntisize; /* Max RNTIs managed by MAC */
   U16                       wrDfltStartRnti; /* Start RNTI value for MAC */
#endif /* these 2 variables are moved under rgrCfg */
   U8                        wrDfltNumRaPreamble; /* Number of RA Preambles 
                                                     supported at MAC */
   U16                       wrDfltRrmRntisize;
   U16                       wrDfltRrmStartPreamble;
   U16                       wrDfltRrmNumPreamble;
   U32                       rsrpTrshldRange; /* Serving Cell threshold 
                                                   RSRP value */            
   CmLListCp                 pendPagUesLst; /* Pending Paging UEs 
                                                  WrUePagingInfo */
   CmLListCp                 **ueCbLst; /*UeCb List*/

   CmLListCp                 eutranNbrFreqLst; /* WrEutranFreqCb List */
   CmLListCp                 cdma1xFreqLst; /* WrCdma1xFreqCb List */
   CmLListCp                 cdmaHrpdFreqLst; /* WrCdmaHRPDFreqCb List */
   CmLListCp                 cdmaBandLst; /* WrCdmaBandClassCb List */
   CmLListCp	              utraFddNbrFreqLst; /* WrUtraFreqCb List for UTRA FDD neighbor frequencies */
   CmLListCp	              utraTddNbrFreqLst; /* WrUtraFreqCb List for UTRA TDD neighbor frequencies */
   CmLListCp                 geranNbrFreqLst; /*WrGeranFreqCb List*/
   WrCellNrCb                *nrCb;               
   WrUeTimers                ueTmrConstants;
   WrEmmCellTmrCfg           tmrCfg;
   Bool                      pendReCfgPres;
   WrEmmcellReCfgInfo        pendRecfg;
   U16                       noMmeToReset;
   U16                       noNghEnbToReset;
   WrCellStat                wrCellStats; /* !< This structure holds Cell level statistics */
   /* Timers for RRM are maintained and manged by FSM.
    This is because RRM is not an TAPA Entity. Simllar
    entry can be found  at EmmCb as well*/
  CmTimer                    rrmRrcConHystTmr;
  CmTimer                    pciModTmr;
  LwrAnrDrxCfg               ueAnrDrxCfg;
  WrSchedulerCfg             schCfg;
  WrAddPwrCntrlParams        ulPwrCtrl;
  WrShutdownPwrCfg           shutdwnPwrCfg;
  WrCtfCfgGrp                ctfCfg;
  WrRgrCfgGrp                rgrCfg;
  WrRlcCfgGrp                rlcCfg;
  WrCellSchdCfg              schdCfg;
/* LTE_ADV starts */
   WrEmmLteAdvCellCb         lteAdvCb;
/* LTE_ADV ends */
   LwrSrbPolicy   srbPolicy[LWR_MAX_SRBS];
   LwrDrbPolicy   drbPolicy[LWR_MAX_QCI];
   Bool                     sib1Modified;
   WrCsfbCfgGrp              csfbCfgGrp;
   /* CSG_DEV start */
   U8                        csgAccessMode;
   LwrCsgPciRangeInfo        smallCellCsgPciRange;
   /* CSG_DEV end */
   /* HCSG_DEV Start */
   U8                        minDlResNonCsg;
   U8                        minUlResNonCsg;
   U16                       maxCsgUeSup;
   U16                       maxNonCsgUeSup;
   /* HCSG_DEV end */
   /* DRX CQI */
   Bool isDRXQCIEnabled;
/* CNM_DEV */
   U32                       cnmLstSyncEarfcn;
   U16                       cnmLstSyncPci;
   U16                       cnmState;
   WrEmmCellType             cellSizeType;/*UE History info*/
   U8                        txPrefMode; /* Indicates the Preferred Transmission Mode */ 
   Bool                      isAutoCfgModeEnb;/* Indicates the Auto TM mode enabled/disabled */
   WrSonCfg                  sonCfg;  /*!< SON configuration related to PA for Scheduler*/
} WrCellCb;

/** This structure is the PLMN CB
 - WrPlmnInfo   plmnInfo            PLMN Information
 - U32          numMmes             Number of MMEs
 - U8           crntMmeIdx          Current MME index 
 - U8           mmes[WR_MAX_MMES]   List of MME indices
 */
typedef struct wrPlmnCb
{
   WrPlmnInfo                plmnInfo;
   U32                       numMmes;
   U8                        crntMmeIdx;
   U8                        mmes[WR_MAX_MMES];
} WrPlmnCb;

/** This structure Is the PLMN information container
 - U32      numPlmns                Number of PLMNs configured
 - WrPlmnCb plmns[WR_MAX_PLMN_IDS]  List of PLMNs
 */
typedef struct wrPlmnCont
{
   U32                       numPlmns;
   WrPlmnCb                  plmns[WR_MAX_PLMN_IDS];
} WrPlmnCont;

#ifdef ENB_OVRLOAD
/** @brief This structure 
 *  */
typedef struct wrEnbOvldParams
{
   U8   critThrldVal;            /*----!< EnodeB Overload Critical Threshold value --*/
   U8   majThrldVal;             /*----!< EnodeB Overload Major threshold value --*/
   U8   minThrldVal;             /*----!< EnodeB Overload minor threshold value --*/
   U8   critRcvryVal;            /*----!< EnodeB Overload recovery Critical threshold  value --*/
   U8   majRcvryVal;             /*----!< EnodeB Overload recovery Major threshold  value --*/
   U8   minRcvryVal;             /*----!< EnodeB Overload recovery Minor threshold  value --*/
   U8   eNBMemCritThresVal;      /*----!< EnodeB Overload Critical threshold  value --*/
   U8   eNBMemMajThresVal;       /*----!< EnodeB Overload Major threshold  value --*/
   U8   eNBMemMinThresVal;       /*----!< EnodeB Overload Minor threshold  value --*/
   U8   maxCntval;               /*----!< EnodeB Overload Cnt value before alarms is raised --*/
   U32  eNBOvldMonTimerVal;      /*----!< EnodeB Overload monitor timer value --*/
   U32  eNBOvldMonfreq;          /*----!< EnodeB Overload monitor Frequency --*/
   Bool eNbOvrLdFlag;            /*----!< EnodeB overload Feature Flag --*/
   U8   ovldCntVal;              /*----!< EnodeB Overload counter value --*/
   U8   prevOvldFlag;            /*----!< EnodeB Overload previous status Indication --*/
} WrEnbOvldParams;

/** @brief This structure 
 *  */
typedef struct _wrCpuParam
{
   unsigned long idle_time;
   unsigned int sys_time;
   unsigned int nice_time;
   unsigned int usr_time;
   unsigned long uptime;
}WrCpuParam;

#endif 


#ifdef MME_LDBAL
/** @brief This structure 
 *  */
typedef struct _wrPrefGrpId
{
   U8 numGrpPref;
   U16 grpId[MAX_GP_PREF];
}WrPrefGrpId;
#endif

/** @brief This structure 
 *  */
typedef struct wrMmeLoaddBalCfg
{
   U16 mmeGroupId;
   U16 mmeGrpPrior;
} WrMmeLoaddBalCfg;

/** @brief This structure 
 *  */
typedef struct wrSSearchR9
{
   Bool        pres;
   U8          sSearchPRel9;
   U8          sSearchQRel9;
} WrSSearchR9;


/** @brief This structure 
 *  */
typedef struct wrThreshXQR9
{
   U8       threshXHighQR9;
   U8       threshXLowQR9;
} WrThreshXQR9;

/** @brief This structure contains information of an inter frequency configured
 *         for th cell
 *  */
typedef struct wrEutranFreqCb
{
   U32                 reCfgType;
   U8                  freqIndex; /**< Index to the frequency Info */
   Bool                status;
   U16                 dlEarfcn; /**< DL Frequecy channel Number */
   U16                 ulEarfcn; /**< UL requecy Channel number for the
                                  *   frequency Index */
   U8                  cellReSelPri; /**< Cell Reselection Priority */
   S8                  qRxLevMin;   /**< minimum Rx Level required for
                                     *   selecting the cell */
   Bool                pMaxPres;  /**< whether to use pMax Value */    
   S8                  pMax;    /**< maximum Allowed TX power on the UL
                                 *   carrier frequency */
   U8                  tReselcxn; /**< Reselection time value */
   Bool                sfPres; /**< whether to use speed state scale 
                                 *   factors */
   WrSpdStSclFctr      sfMedium; /**< medium, reselection Scaling factor */
   WrSpdStSclFctr      sfHigh; /**< High, reselection Scaling factor */

   TknU8               sIntraSrch;  /**< Threshold value for Intra freq 
                                     *   measurenment */
   TknU8               sNonIntraSrch;  /**< Threshold value for Inter freq 
                                        *   measurenment */
   U8                  thSrvngLow; /**< low threshold value for serving freq,
                                    *   when reselection evaluation */
   U8                  threshXHigh; /**< */
   U8                  threshXLow; /**< */
   Bool                measBWusage; /**< Whether to use measurementBandwidth */
   TknU8               allowedMeasBW; /**< This is maximum measurement bandwidth 
                                       *   to permit for Carrier frequency */
   Bool                presenceAntPort1; /* Whether of Antenna Port1 exist */
   
   U8                  neighCellConfig; /**< Provides informatOAion related to 
                                         *   MBSFN and TDD UL/DL configuration 
                                         *   of neighbour cells of this 
                                         *   frequency */
   
   WrQoffRange         qOffsetFreq; /**< qOffset broadcasted in SIB5 */
   WrQoffRange         offsetFreq; /**< Frequency offset for MeasEUTRA */
   WrSSearchR9         sIntraSrchR9; /**< Rel-9 Intra search Threshold */
   WrSSearchR9         sNonIntraSrchR9; /**< Rel-9 Inter search Threshold */
   TknS8               qQualMinR9; /**< qQualMin value for Rel-9 */
   TknU8               thSrvngLowQR9; /**< low threshold value for serving freq,
                                         *   when reselection evaluation */
   WrThreshXQR9        threshXQR9; /**< This is threshold used when 
                                       *   reselecting high priority frequency 
                                       *   than current camped frequency in 
                                       *   Rel-9  */

   CmLList             freqLstEnt;
} WrEutranFreqCb;

/* CSFB - Start */

/** @brief This structure holds the neighbour frequency information specific for UTRAN FDD
   *  - U8                   pQualMin;                 The minimum quality level required in UTRA FDD cells
   *  - U16                  arfcnUl;                  The uplink absolute radio frequency channel
   *                                                          (ARFCN) of UTRAN neighboring cell to the eNB
   *  - U16                  arfcnDl;                  The downlink absolute radio frequency channel
   *                                                          number (ARFCN) of UTRAN neighboring cell to the eNB
 */
typedef struct wrUtranFddNeighFreq{
   U8          thresQR9Pres;        /** This param used as presence flag for threshXhighQR9 and threshXlowQR9 */
   U8			   threshXhighQR9; 		/**<The threshold  Rel 9 used in the UE to reselect a frequency
                                   whose priority is higher than the current camped frequency*/
   U8			   threshXlowQR9;  		/**<The threshold Rel 9 used in the UE to reselect a low-priority
                                   frequency from high-priority frequency*/
   S8			   pQualMin;  		
   U16 			arfcnUl;    		
   U16			arfcnDl;    		
}WrUtranFddNeighFreq;

/** @brief This structure holds the neighbour frequency information specific for UTRAN TDD
 * - LwrUtraTddMode    tddMode; ENUM to identify the UTRA TDD modes.
 * - U16    arfcn; The absolute radio frequency channel of the UTRA TDD neighbor cell.
 */
typedef struct wrUtranTddNeighFreq{
   U8                bandIndicator;
   U16               arfcn;
   LwrUtraTddMode    tddMode;
}WrUtranTddNeighFreq;

/** @brief This structure holds the neighbour frequency information for UTRAN
  - U32                  reCfgType;
  - U32                  freqIndex;                Database index of UTRAN neighboring cell
  - U8                   priority;                 Priority information on the UTRA FA
  - U8                   threshXhigh;              The threshold used in the UE to reselect a frequency
                                                         whose priority is higher than the current camped frequency
  - U8                   threshXlow;               The threshold used in the UE to reselct a low-priority
                                                         frequency from high-priority frequency
  - U8                   qRxLevMin;                The minimum RX level required in a cell in dbM units
  - U8                   pMaxUtra;                 The maximum RF output power in the UE
  - U8                   offsetFreq;               The offset applied to UTRA carrier frequency
*/
typedef struct wrUtraFreqCb
{
   U32			reCfgType; 
   U8			freqIndex;    		/**< UTRAN frequency index.Upto 6 FAs
                                         		can be assigned per cell */
   TknU8		priority;   		/**< Priority information on the UTRA FA*/
   U8			threshXhigh; 		/**<The threshold used in the UE to reselect a frequency
                                         		whose priority is higher than the current camped frequency*/
   U8			threshXlow;  		/**<The threshold used in the UE to reselect a low-priority
                                         		frequency from high-priority frequency*/
   S8			qRxLevMin;   		/**<The minimum RX level required in a cell in dBm units */
   S8			pMaxUtra;    		/**<The maximum RF output power in the UE*/
   U8 			offsetFreq;  		/**<The offset applied to UTRA carrier frequency */
   WrDuplexMode	duplexMode;  /*!<The duplex type of UTRAN(TDD/FDD) neighboring cell to the eNB */
   CmLList                    freqLstEnt;
   union{
      WrUtranFddNeighFreq     utraFddFreq;
      WrUtranTddNeighFreq     utraTddFreq;
   }t;   	
}WrUtraFreqCb;

/** @brief This structure holds the neighbour cell information specific for UTRAN FDD
 * - U16                  psc;       The primary scramble code of UTRAN neighboring cell
 * - U16                  arfcnUl;   The uplink absolute radio frequency channel
 *                                   (ARFCN) of UTRAN neighboring cell to the eNB
 * - U16                  arfcnDl;   The downlink absolute radio frequency channel
 *                                   (ARFCN) of UTRAN neighboring cell to the eNB
 **/
typedef struct wrUtranFddNeighCell{
  U16                         psc;       	
  U16	                        arfcnUl;	
  U16	                        arfcnDl;		
} WrUtranFddNeighCell;

/** @brief This structure holds the neighbour cell information specific for UTRAN TDD
 *
 * - U16                  psc;      The Cell Parameter Id of the UTRA TDD Cell.
 * - U16                  arfcnUl;  The uplink absolute radio frequency channel
 *                                  (ARFCN) of UTRAN TDD neighboring cell to the eNB
 * - S16                  pccpch;   PCCPCH Tx power of the UTRA TDD Cell.                                                                    
 * */
typedef struct wrUtranTddNeighCell{
  U16                         psc;       	
  U16	                        arfcn;	
  S16	                        pccpch;		
} WrUtranTddNeighCell;

/** @brief This structure holds the neighbour cell information for UTRAN
  - U32                  reCfgType;
  - U32                  nrIndex;                  Database index of UTRAN neighboring cell
  - U16                  rncId;                    RNC ID of UTRAN neighboring cell to the eNB
  - U16                  cId;                      CID of UTRAN neighboring cell to the eNB
  - U16                  lac;                      Location Area Code of UTRAN neighboring cell to the eNB
  - U8                   rac;                      Routing area code of UTRAN neighboring cell to the eNB
  - LwrPlmnId            plmnId[LWR_MAX_PLMN_IDS]; This is the PLMN List information(MNC) of ENB
                                                         in the neighbor cell belongs
  - WrDuplexMode         duplexMode;               The duplex type of UTRAN neighboring cell to the eNB
  - Bool                 isRemAllowed;             Whether to delete a certain neighboring cell or not
  - Bool                 isHOAllowed;              Whether to perform handover to UTRAN neighboring cell or not
  - Bool                 isVoipCapable;            Whether to support VoIp of UTRAN neighboring cell
  - U16                  curRank;                  To be multiplied with with a multiply factor 1000 to the
                                                          PLD value
  - U16                  prevRank;                 To be multiplied with with a multiply factor 1000 to the
                                                          PLD value
*/
typedef struct wrUtraNeighCellCb
{
  U32	                        reCfgType;
  U32	                        nrIndex;		
  U16	                        rncId;			
  U16   	                     cId;			
  U16	                        lac;			
  U8 	                        rac;			
  U8	                        numPlmn;
  WrPlmnId                    plmnId[LWR_MAX_PLMN_IDS]; 
   U8       	  	            duplexMode;	
   union{
      WrUtranFddNeighCell      utraFdd;
      WrUtranTddNeighCell      utraTdd;
   }t;
  Bool                       isVoipCapable;
  Bool                       isPsHOCapable;
  Bool                       isRimSupported; /* This variable is 
                                                 used to know whether neighbour
                                                 UTRA cell is RIM supported 
                                                 or not */                           
  U8	                        qoffset;
  U8                          curRank; /**< previous priority of the UTRAN Nbr Cell */
  Bool                        blackListed; /**< The Nbr is HO Blacklisted by the Algo */
  WrNrUtranFreqCb             *freqNode; 		
  CmLList                     nrWlBlLnk;
  TknStrOSXL                 utraSI;
  Bool                       rimState;
}WrUtraNeighCellCb;

/** @brief This structure holds the freq Info of cdma1xRTT carrier
 *  */
typedef struct wrCdma1xFreqCb
{
   U8                         reCfgType;
   U8                         freqIndex;
   Bool                       status;
   U8                         bandClass;
   U16                        arfcn;
   U8                         searchRate;
   S8                         offsetFreq;
   U8                         freqPriority;
   CmLList                    freqLstEnt;
} WrCdma1xFreqCb;

/** @brief This structure holds the freq Info of CDMA HRPD carrier
 *  */
typedef struct wrCdmaHRPDFreqCb
{
   U8                         reCfgType;
   U8                         freqIndex;
   Bool                       status;
   U8                         bandClass;
   U16                        arfcn;
   U8                         searchRate;
   S8                         offsetFreq;
#ifdef DG
   U16                        anrUeSearchRateHRPD;
#endif
   CmLList                    freqLstEnt;
} WrCdmaHRPDFreqCb;


/** @brief This structure holds the BandClass Info of cdma1xRTT band
 *  */
typedef struct wrCdmaBandClassCb
{
   U8                         reCfgType;
   U8                         bandIndex;
   Bool                       status;
   U8                         bandClass;
   U8                         cellReselPri;
   U8                         threshxHigh;
   S8                         threshxLow;
   CmLList                    bandLstEnt;
} WrCdmaBandClassCb;
 /* This structure holds Frequency parameter for GERAN */
typedef struct wrGeranFreqCb
{
   CmLList        freqLstEnt;  
   U16            arfcn;              /*BCCH ARFCN*/
   TknU8          pMaxGeran;          /*Max allowed Tx power on a UL carrier freq*/
   U8             bandInd;            /*Band Indicator: GSM 850, GSM 900, 
                                        DCS 1800, PCS 1900*/
   U8             qRxLevMin;          /*Min required Rx level of GERAN 
                                        cell*/
   U8             cellReselPriority;  /*Absolute priority of the concerned
                                        carrier frequency*/
   U8             threshXLo;          /*Threshold used when re-selecting towards
                                        a lower priority GERAN frequency group 
                                        than current serving E-UTRA frequency*/
   U8             threshXHi;          /*Threshold used when re-selecting towards
                                        a higher priority GERAN frequency group 
                                        than current serving E-UTRA frequency*/
   U8             nccPermitted;       /*Broadcasted in SIB 7*/
}WrGeranFreqCb;

 /* This structure holds Cell parameter for GERAN */
typedef struct wrGeranCellCb
{
   CmLList         cellLstEnt;
   WrGeranFreqCb   *freqCb;
   WrPlmnId        plmnId;             /*PLMN Id*/
   U32	          nrIndex;	         /*NRT index*/	
   U16             lac;                /*Location Area Code*/
   U8              rac;                /*Routing Area Code*/
   U16             cId;                /*Cell Id*/
   U8              bsic;               /*Base Station Identifier Code: 
                                         bits 7 & 6 = 0, bits 5 4 3 = NCC, 
                                         bits 2 1 0 = BCC*/
   U8              nccPermittedMeas;   /*NCC permitted per Cell used for 
                                         measurement configuration*/
   Bool            isDtmCapable;       /*DTM capability of this cell*/
   U8              siPsiType;/*to indicate SI or PSI*/
   U8              noOfsiPsi;/*no. of SIs or PSIs*/
   WrSiInfoGeran   siOrPsi;/*SI or PSI*/
   TknStrBSXL      nco; /*network control order*/
   S8              offsetFreq;/*Geran Qoffset*/ 
   Bool            isRimSupported; /* This variable is 
                                      used to know whether neighbour
                                       GERAN cell is RIM supported 
                                       or not */                           
  Bool             rimState;
}WrGeranCellCb;

typedef struct wrNeighNeighCb
{
   WrEcgi                    ecgi;
   U16                       pci;
   U16                       dlEarfcn;
   U32                       tac; /**< Tracking Area Code of E-UTRAN Neighbor Cell*/
   CmLList                   LstEnt;
} WrNeighNeighCb;

typedef struct wrTimeStamp
{
   U32               timeSeconds; /*time in seconds*/
   U32               timeMs;       /*time in micro seconds*/ 
}WrTimeStamp;

typedef struct wrEutranNeighCellCb
{
   U8                nrIndex; /**< Index to the Neighbor Cell Info */
   U32               enbId; /**< This is eNB ID of basestation affiliated with
                             *   E-UTRAN Neighbor Cell on around basestation
                             *   if enbType value is Macro eNB, eNB ID is 20bit,
                             *   if enbType value is Home eNB, eNB ID is 28bit
                             *   Used when creating Cell Identifier
                             */
   WrEnbType         enbType; /**< This is basestation type information affiliated
                               *   with E-UTRAN Neighbor Cell on around basestation
                               *   - ci_Macro_eNB: Macro eNB
                               *   - ci_Home_eNB: Home eNB
                               */
   WrPlmnId          enbPlmn; /**< This is the PLMN List information(MNC) of
                               *   ENB in the neighbor cell belongs.
                               */
   U16               pci; /**< This is Physical Cell ID of E-UTRAN Neighbor Cell on
                           *   around basestation
                           */
   U16               tac; /**< This is Tracking Area Code of E-UTRAN Neighbor Cell
                           *   on around basestation
                           */
   U8                numPlmn; /**< Number of PLMN ID present for the cell */
   WrPlmnId          plmn[WR_MAX_PLMN_IDS]; /**< This is Broadcast PLMN List information(MNC) of
                               *   E-UTRAN Neighbor Cell on around basestation.
                               */
   U16               earfcnDl; /**< This is E-UTRAN Neighbor Cell.
                                *   Downlink EARFCN(E-UTRAN Absolute Radio Frequency
                                *   Channel Number) on  around basestation
                                */
   U16               earfcnUl; /**< This is E-UTRAN Neighbor Cell.
                                *   Uplink EARFCN on around basestation
                                */
   WrEutranBw        dlBw; /**< This is Downlink Bandwidth information of
                            *   E-UTRAN Neighbor Cell on around basestation.
                            */
   WrEutranBw        ulBw; /**< This is Uplink Bandwidth information of
                            *   E-UTRAN Neighbor Cell on around basestation.
                            */
   WrQoffRange       cio;  /**< CIO : This is  Cell Individual Offset applyed to
                               *   E-UTRAN Neighbor Cell on around basestation.
                               *   that use when terminal is measuring on RRC
                               *   Connected mode */
   WrQoffRange       qoffset; /**<  Q-Offset : This is Cell Quality Offset value
                               *    applied to E-UTRAN Neighbor Cell on around base
                               *    station. That use Whenreselecting Cell of
                               *    terminal on RRC Idle mode
                               */
   WrEcgi            eCgi; /**< This  specifies the Evolved Cell Global Identifier
                            *   (ECGI), the globally unique identity of a cell in
                            *   E-UTRA. */
   Bool              blackListed; /**< The Nbr is HO Blacklisted by the Algo */
 
   U8                numAntenna;  /*!< NUM OF ANT for the cell */
   WrSib2PrachCfg    prachCfg;    /*!< PRACH CONFIGURATION */
   
   CmLList           srvdCellLstEnt;
   CmLListCp         neighNeighLstCp;

   CmLList           nrWlBlLnk; /**< Linked List link for WL/BL */
                                                                              
   WrNrEutranFreqCb  *freqNode; /**< Pointer to the frequency Node in which
                                  * the cell is present */
   U8                curRank; /**< current priority of the E-UTRAN Nbr Cell */
   WrTimeStamp       timeStamp; 
   /* CSG_DEV start */
   U8                isCsgIdResolved;
   U32               csgId;
   U8                isCsgAccessModeResolved;
   U8                csgAccessMode;
   /* CSG_DEV end */
/*ccpu00134580-ADD-To support TDD in x2 setup*/
#ifdef LTE_TDD
   U8 ulDlSubframeCfg;
   U8 splSfCfg;
   U8 cpDl;
   U8 cpUl;
#endif
   /* CNM_DEV */
   Bool              isCnmPerformed; /**< indicates if CNM is performed with
                                       * this neighbour cell */
   Bool              isCsgCell;
   S16               detectedRsrp;
   CtfCnmMibInfo     cnmMibInfo;/* MIB information */
   U8     isHeMsCell;   /* Source from where the cell is added. */
   U32    numTtiTicks;
}WrEutranNeighCellCb;

/**   
 *   @brief This Structure contains CDMA2000 Nbr Info
 */
typedef struct wrCdma2kNeighCellCb
{
    CmHashListEnt     cdmaNrHashEnt; /**< Hash list link, for the list 
                                      *   maintained  in EMM */
                      
    U8                nrIndex; /**< Index information of NRT Black List Cell */
    U32               bandClass; /**< Defines the CDMA2000 band in which the 
                                  *   CDMA frequency carrier can be found, 
                                  *   specified in bandclass number.
                                  */
    U32               arfcn; /**< Defines the CDMA2000 frequency carrier within
                              *   a CDMA2000 band, as specified by
                              *   ARFCN-Value CDMA2000 in SIB8
                              */
    U16               pnOffset; /**< Defines the PN offset that represents the
                                 *   'Physical cell identity' in CDMA2000 
                                 *   system, as specified by PhysCellIdCDMA2000
                                 *   in SIB8 */
    WrCdmaType        cdmaType; /**< Indicates the type of the cell. This 
                                 *   parameter determines the length of the CID 
                                 *   parameter.
                                 *   Enumeration of:
                                 *    1xRTT
                                 *    HRPD
                                 */
    U16               cid; /**< Defines the global cell identity of the cell.
                            *   For a 1xRTT cell, the cell identity is a binary
                            *   string 47 bits long. For a HRPD cell, the cell
                            *   identity is a binary string 128 bits long
                            */
    Bool              blackListed; /**< Indicates whether the cell is 
                                    *   blacklisted */
    TknStrOSXL        sectorId;
                      
    CmLList           cdmaNrLLstEnt; /**< Linked List link for NCL/HOBL */

    WrNrCdmaFreqCb    *freqNode; /**< Pointer to the frequency Node in which 
                                  * the cell is present */
    U16               pilotPnPhase; /* default Pilot Phase, same is reported by
                   UE measurement */
    U8                pilotStrength; /* default Pilot Phase, same is reported by
                                         UE measurement */
    U8                systemType;
    U16               channelNum;
    U8                pilotOneWay;
} WrCdma2kNeighCellCb;

typedef struct wrEutranNrInfo
{
   U16            nrIndex; /*!< Index of the neighbor cell */
   U16            earfcn; /*!< EARFCN of the cell */
   U16            phyCellId; /*!< PCI of the cell */
   U16            cid; /*cell Id*/
   WrPlmnId       plmnId;/*plmn Id of the cell*/  
}WrEutranNbrCellInfo;

typedef struct wrUtranNrInfo
{
   U16           nrIndex;  /*!< Index of the neighbor cell */
   U16           arfcn;    /*!< ARFCN of the cell */
   U16           psc;     
   U16           cid; /*cell Identity of the cell*/
   WrPlmnId      plmnId;/*plmn Id of the cell*/  
}WrUtranNrInfo;
 /* This structure holds GERAN neighbor information */
typedef struct wrGeranNrInfo
{
   WrPlmnId        plmnId;  /*PLMN Id*/
   U32             nrIndex; /*Index of the neighbor cell*/
   U16             arfcn;   /*ARFCN of the cell*/
   U16             cId;     /*Cell Id*/
   U8              bsic;    /*Base Station Identifier Code: bits 7 & 6 = 0, 
                             *bits 5 4 3 = NCC, bits 2 1 0 = BCC*/
   U8              bandInd; /*Band Indicator: GSM 850, GSM 900, DCS 1800,
                             *PCS 1900*/
}WrGeranNrInfo;

/** @brief This structure contains the NR modification Info and the pointer to
 *         the neighbor Cell Cb
 *  */
typedef struct wrEutranCellModInfo
{
   WrEutranNbrCellInfo     nrInfo;
}WrEutranCellModInfo;

typedef struct wrUtranCellModInfo
{
   WrUtranNrInfo      nrInfo;     
}WrUtranCellModInfo;

#define WR_MAX_GROUP 16

#define WR_UTRA_MODE_FDD 0
#define WR_UTRA_MODE_TDD 1

/** 
  * @brief This is a structure for Served cell 
  * modifiation information. 
  * @details The structure members 
  * - U32             oldCellId Old CellId 
  * - U32             newCellId New CellId
  */

typedef struct wrEmmSrvdCellMod
{
   U32             oldCellId;
   U32             newCellId;
} WrEmmSrvdCellMod;

/** 
  * @brief This is a structure for ENB Configuration 
  * update  information. 
  * @details The structure members 
  * - U8           numSrvdCellAdded             Number of cells to be added
  * - U8           numSrvdCellDeleted           Number of cells to be deleted
  * - U8           numSrvdCellModified          Number of cells to be modified
  * - U8           numGrpIdAdded                Number of groupIds added
  * - U8           numGrpIdDeleted              Number of groupIds deleted
  * - U32          srvdCellAdd[WR_MAX_CELLS]     Served cellIds to be added
  * - WrEmmSrvdCellMod  srvdCellMod[WR_MAX_CELLS] Substructure for Cell Modify
  * - U32          oldEcgiToDel[WR_MAX_CELLS]    List of ECGI to be deleted
  * - U32          guIdAddLst[WR_MAX_GROUP]     List of GUID Addition 
  * - U32          guIdDelLst[WR_MAX_GROUP]     List of GUID Deleton 
  */
typedef struct wrEmmEnbCfgUpdInfo
{
   U8                        numSrvdCellAdded;
   U8                        numSrvdCellDeleted;
   U8                        numSrvdCellModified;
   U8                        numGrpIdAdded;
   U8                        numGrpIdDeleted;
   U32                       srvdCellAdd[WR_MAX_CELLS];
   WrEmmSrvdCellMod          srvdCellMod[WR_MAX_CELLS];
   U32                       oldEcgiToDel[WR_MAX_CELLS];
   WrGuGrpIdLst              guIdAddLst[WR_MAX_GU_GRP_IDS];
   WrGuGrpIdLst              guIdDelLst[WR_MAX_GU_GRP_IDS];
   Bool                      isCellModified;   
} WrEmmEnbCfgUpd;

typedef struct wrNeighEnbCb
{
   CmHashListEnt            neighEnbHashEnt;  

   U16                       cellId;
   U32                       peerId;
   U32                       enbId;
   U8                        x2Connection;
   Bool                      isX2Connected;
   Bool                      isX2Reqd;
   Bool                      isAnrDetected;
   U8                        enbType;
   U8                        numGuGrpIds;
   WrGuGrpIdLst              guGrpIdLst[WR_MAX_GU_GRP_IDS];

   CmTptAddr                 neighEnbAddr;
   CmTptAddr                 secIpAddr;
   U32                       egtpTnlCntr;
   CmTimer                   timer;
   CmTimer                   enbCfgUpTm;
   U32                       rstRtxCnt; 
   U32                       cfgUpdRtxCnt; 
   U32                       x2SetupRtxCnt; 
   WrPlmnId                  plmnId;
   CmLListCp                 srvdCellLst;
   WrEmmEnbCfgUpd            cfgUpdRtxInfo;
   U16                       enbcfgTransRtxCnt;/*Count for eNB CFG TRANS RETXION*/
   CmTimer                   enbcfgTransTmr;
   WrAlarmVal                alarmVal;
   U8                        enbCfgTransTriggerd;/*flag to indicate enbCfg 
                                                      transfer is triggered or not*/
/* LTE_ADV_DSFR starts */
   RgrSfrRbRange             dsfrCEFreqRange; /* Cell edge RB range */
/* LTE_ADV_DSFR ends */
   U8                        isParallelCfgUpdate; /* flag to indicate whether this 
                                is parallel eNB configuration update or not*/
} WrNeighEnbCb;


typedef struct wrEmmTmrCfg
{
   U16                       s1SetupTmr;
   U16                       s1UpdateTmr;
   LwrUpdateRetryCount       s1RetryCount;
   LwrUpdateTimeToWait       s1TimeToWait;
   U16                       s1MmeConfigurationTransferTmr;
   U16                       s1ResetTmr;
   U16                       S1ResetRetryCount; 
   U16                       x2SetupTmr;
   LwrUpdateTimeToWait       x2SetTimeToWait;
   U16                       x2UpdateTmr;
   LwrUpdateRetryCount       x2UpdRetryCount;
   LwrUpdateTimeToWait       x2UpdTimeToWait;
   U16                       x2ResetTmr;
   U16                       X2ResetRetryCount;
   U16                       X2SetupRetryCount;
   U16                       internalNoWaitRetryTmr;
   U32                       rimOvrlTmr;
} WrEmmTmrCfg;

/** This structure contains
 * - peerId                  Peer ID
 * - enbId                   eNodeB ID
 */
typedef struct _wrEmmEnbIdNode
{
   U32                       peerId;
   U32                       enbId;
} WrEmmEnbIdNode;

typedef struct wrUePerBsrTmr
{
   U16       prdBsrTmr;
   U16       retxBsrTmr;
} WrUePerBsrTmr;
                
typedef struct wrPdschDed
{
   U32                       p_a;
}WrPdschDed;

/* Fix for ccpu00142992:TTIs are not coming after s1-close triggered from
 * MME when UE is in attached state To avoid the race condition at application 
 * between receiving the ABORT cfm and UE local context release.
 *
 *   This structure contains
 *  -  mmeId         MME ID for which the Abort Confirm has clome.              
 *   - isAbortCfmRcv Boolean flag for abort confirm has been received or not.
 */
typedef struct _wrPeerAbrtCfm
{
   WrMmeId  mmeId;
   Bool  isAbortCfmRcv;
}wrPeerAbrtCfm;


/** This structure contains
 - Mem                       mem memory pool info 
 - TskInit                   init
 - Header                    lmHdr
 - UConnId                   s1apConId Last used connection id; counter 
 - U8                        numCells
 - WrCellCb                  **cellCb
 - WrMmeCont                 mmeCont
 - CmTptAddr                 datAppAddr
 - U32                       nhuTransId
 - WrEnbType                 enbType
 - U16                       enbNameLen
 - U8                        enbName[151]
 - U32                       x2apOldEnbUeIdCntr
 - WrPlmnCont                plmns
 - U32                       s1PrepTimerVal  Preparation Timer Value 
 - U32                       s1OvrAllTimerVal  Overall Timer Value 
 - U32                       ueHoAttachTimerVal  Time within which UE 
 -                                                must detach from source
 -                                                cell and attach to 
 -                                                target cell during 
 -                                                Handover
 - CmTimer                  enbCfgWaitTmr    Timer for X2 Enodeb 
                                             configuration update timer.
 - CmTimer                  enbCfgRspTmr     Timer for Response of X2 EnodeB 
                                             configuration update timer.
 */

typedef struct _wrEmmCb
{
   Mem                       mem; /* memory pool info */
   TskInit                   init;
   Header                    lmHdr;
   UConnId                   s1apConId; /* Last used connection id; counter */
   U8                        numCells;
   WrCellCb                  **cellCb;
   WrMmeCont                 mmeCont;
   CmHashListCp              neighEnbLstCp;
   CmTptAddr                 datAppAddr[WR_MAX_IP_TYPES];
   U8                        numTxAntenna;
   U32                       nhuTransId;
   U32                       enbId;
   WrEnbType                 enbType;
   WrPlmnCont                 plmns;
   U16                       enbNameLen;
   U8                        numGuGrpIds;
   WrGuGrpIdLst              guGrpIdLst[WR_MAX_GU_GRP_IDS];
   U8                        enbName[WR_EMM_CB_ENB_NAME];
   U32                       x2apOldEnbUeIdCntr;
   U32                       s1CancelTimerVal; /*HO Cancel Timer Value*/
   U32                       ueHoAttachTimerVal; /* Time within which UE 
                                                  must detach from source
                                                  cell and attach to 
                                                  target cell during 
                                                  Handover. */
   CmTimer                  enbCfgWaitTmr;
   CmTimer                  enbCfgRspTmr;
   CmTimer                  enbResetRspTmr;
   U8                       adminState;
   WrEmmEnbIdNode           x2ApEnbIdLst[WR_X2AP_MAX_PEERS];
#if (VE_SM_LOG_TO_FILE && DEBUGP)
   FILE*                    dbgFp;  /*!< Debug file pointer */
   Txt                      filePath[LVE_MAX_FILE_PATH]; /*!< store log files */
   U32                      nmbDbgLines; /*!< Number of lines per debug file */
   U32                      cfgDbgLines; /*!< Cfgd number of lines/Dbg file */
#endif
#ifdef ENB_OVRLOAD
   WrEnbOvldParams          eNBOvldParams;   /*!<Stores Configured parameters*/
   CmTimer                  smplTimer;       /*!<Timer for Sampling Memory 
                                                 and CPU usage*/
   CmTimer                  eNbOvrldStatUpd; /*!<Timer for Updating eNodeB 
                                                 overload status based upon 
                                                 Sampled values*/
   U32                      memSample;       /*!<Stores  memory samples taken 
                                                 over time*/
   U32                      cpuSample;       /*!<Stores  CPU samples taken 
                                                 over time*/
   WrCpuParam               cpuUsageparam;   /*!<This stores values of CPU usage 
                                              during last sampling interval.*/
#endif
#ifdef MME_LDBAL
   WrPrefGrpId grpIdPref;   /*!<This stores GrpId in decreasing order of preference.*/
#endif 
   U8                       maxNumOfUe;   /*!< Maximum number of UEs to be admitted */
   U16                      sctpPortNumS1;  /*!< The port number to be used in SCTP 
                                                 association configuration for S1AP 
                                                 with the MME*/
   U16                      sctpPortNumX2;  /*!< The port number to be used in SCTP 
                                                 association configuration for X2AP 
                                                 with the neighbor eNB */
         
   U32                      bhLinkCapacity; /*!< Total backhaul link capacity 
                                                 of the eNB */
           
   U8                       numOfMmePriGrps;
   WrMmeLoaddBalCfg         mmeLoadBalCfg[LWR_SM_MAX_MMES];
   WrEmmTmrCfg              tmrCfg;
   U32                     s1UStatus;
   /* Timers for RRM are maintained and manged by FSM.
    This is because RRM is not an TAPA Entity. Simllar
    entry can be found  at cellCb as well*/
   CmTimer                  rrmCpuLdHystTmr;
   U32                      enbCfgTransTmrVal;
   WrUePerBsrTmr          uePerBsrTmrCfg;
   WrPdschDed             pdschDedCfg;
   CmTimer                sib8Tmr;
/* LTE_ADV starts */
   WrEmmLteAdvCb            lteAdvCb;
/* LTE_ADV ends */
   /* This Data type contains gummei and MME Cbs list */
   CmHashListCp              GummeiMmeCbs;
   CmHashListCp              guGrupIdLst;
   WrEmmEnbCfgUpd            cellInfo;
   LwrX2ConFlg               x2ConFlg;
   U8                        pwsEtwsCmas;
   U8                        avgFreeCpuRqd; /* AvgFree cpu required*/
/* 0 to disable PWS, 1 to enable ETWS and 2 to enable CMAS */     
   /* CNM_DEV */
   Bool                      isCnmEnabled;
   CmTimer                   rimOvrlTmr;
   /* UL SRS Periodicity in milli seconds */
   U32                       ulSrsPeriod;
   wrPeerAbrtCfm             peerAbrtCfm; 
} WrEmmCb;

typedef enum _wrOamAccessBaringUpdType
{
   WR_OAM_CELL_BARING,
   WR_OAM_ENB_OVRLD_MAN,
   WR_OAM_1RTT_ACC_BARING
}WrOamAccessBaringUpdType;

typedef struct _wrOamAccessBarring
{
   U16 cellId;
   WrOamAccessBaringUpdType  updType;
   union
   {
      U8 cellBarred;
      struct
      {
         U8                     emergency;
         WrSib2AcBarringCfg     signalling;
         WrSib2AcBarringCfg     dataCall;
      } barringInfo;
      struct 
      {
         U8 acBarr0to9;
         U8 acBarr10;
         U8 acBarr11;
         U8 acBarr12;
         U8 acBarr13;
         U8 acBarr14;
         U8 acBarr15;
         U8 acBarrMsg;
         U8 acBarrReg;
         U8 acBarrEmg;
      }RTT;
   }u;
}WrOamAccessBarring;

/** This structure contains
 - U32 numOfMme  No of MME in a given PLMN CB 
 - U8 *mmeIdList List of MME ID which below of Given PLMN
 */
typedef struct _wrPlmnCbMmeList
{
   U32 *crntIdx;
   U32 numOfMme;
   U8 *mmeIdList;
}WrPlmnCbMmeList;

typedef struct _wrEutraNghCellModInfo
{
   U16   earfcnDl;
   U16   pci;
   U8    blackListed;
   U32   mask;
}WrEutraNghCellModInfo;



EXTERN  WrEmmCb   wrEmmCb;
#define WR_EMM_MAX_CELLS     1

#define WR_GET_CELLCB(_cellCb, _cellId){\
   U32 _idx;\
   _cellCb = NULLP;\
   for(_idx = 0; _idx < WR_EMM_MAX_CELLS; _idx++) {\
      if (wrEmmCb.cellCb[_idx] != NULLP) {\
         if (wrEmmCb.cellCb[_idx]->cellId == _cellId) {\
            _cellCb = wrEmmCb.cellCb[_idx];\
         }\
      }\
   }\
}

/* Size of TB size table of BCCH  */
#define WR_TBSZTBL_SIZE   44
/** @def WR_GET_TBSZ
 *  This macro finds the appropriate TB size for  BCCH and DLSCH messages.
 *  It applies binary seach on sorted TB size list to find the TB size.
 */
#define WR_GET_TBSZ(_msgOctets, _tbSz) \
do \
{ \
   S32 _minIdx, _midIdx, _maxIdx; \
   for (_minIdx = 0, _maxIdx = WR_TBSZTBL_SIZE - 1; _minIdx <= _maxIdx;) \
   { \
      _midIdx = ((_minIdx + _maxIdx) / 2); \
      if (WrTbSzTbl[_midIdx] < _msgOctets) \
      { \
         _minIdx = _midIdx + 1; \
      } \
      else if (WrTbSzTbl[_midIdx] > _msgOctets) \
      { \
         _maxIdx = _midIdx - 1; \
      } \
      else \
      { \
         _tbSz = WrTbSzTbl[_midIdx]; \
         break; \
      } \
   } \
   if (_minIdx > _maxIdx) \
      _tbSz = WrTbSzTbl[_minIdx]; \
} \
while(0)

#define FILL_UL_BW(_ulBwVal,_cfg) {\
switch(_ulBwVal)\
{                               \
   case WR_SIB2_ULBW6:          \
      _cfg.ulBw= 6;       \
      break;                    \
   case WR_SIB2_ULBW15:         \
      _cfg.ulBw = 15;      \
      break;                    \
   case WR_SIB2_ULBW25:         \
      _cfg.ulBw = 25;      \
      break;                    \
   case WR_SIB2_ULBW50:         \
      _cfg.ulBw = 50;      \
      break;                    \
   case WR_SIB2_ULBW75:         \
      _cfg.ulBw = 75;      \
      break;                    \
   case WR_SIB2_ULBW100:        \
                                _cfg.ulBw = 100;     \
   break;                    \
}\
}

#define FILL_DL_BW(_dlBwVal,_cfg) {\
   switch(_dlBwVal)\
   {\
      case WR_MIB_DLBW6:\
         _cfg.dlBw = 6;\
         break;\
      case WR_MIB_DLBW15:\
         _cfg.dlBw = 15;\
         break;\
      case WR_MIB_DLBW25:\
         _cfg.dlBw = 25;\
         break;\
      case WR_MIB_DLBW50:\
         _cfg.dlBw = 50;\
         break;\
      case WR_MIB_DLBW75:\
         _cfg.dlBw = 75;\
         break;\
      case WR_MIB_DLBW100:\
         _cfg.dlBw = 100;\
         break;\
   }\
}

#define WR_EMM_MME_GET_TIME_TO_WAIT_VAL(_rcvdVal, _tmrVal)     \
{                                                      \
   switch(_rcvdVal)                                    \
   {                                                   \
      /* 1Sec */                                       \
      case 0:                                          \
         _tmrVal = (1 * 1000);                 \
      break;                                           \
      /* 2Sec */                                       \
      case 1:                                          \
         _tmrVal = (2 * 1000);                 \
      break;                                           \
      /* 5Sec */                                       \
      case 2:                                          \
         _tmrVal = (5 * 1000);                 \
      break;                                           \
      /* 10Sec */                                      \
      case 3:                                          \
         _tmrVal = (10 * 1000);                \
      break;                                           \
      /* 20Sec */                                      \
      case 4:                                          \
         _tmrVal = (20 * 1000);                \
      break;                                           \
      /* 60Sec */                                      \
      case 5:                                          \
         _tmrVal = (60 * 1000);                \
      break;                                           \
      default:                                         \
         break;                                        \
   }                                                   \
}

/* get the eNBId for the peerId from the list */
/* KW fix for LTE_ADV */   
#define WR_GET_PEERID_FROM_ENBID(_eNbId, _peerId) {\
    U32 i;\
    _peerId = 0;\
    for(i = 0; i < WR_X2AP_MAX_PEERS; i++)\
    {\
        if(wrEmmCb.x2ApEnbIdLst[i].enbId == _eNbId)\
        {\
            _peerId = wrEmmCb.x2ApEnbIdLst[i].peerId;\
            break;\
        }\
    }\
}

/* get the eNBId for the peerId from the list */
/* KW fix for LTE_ADV */  
#define WR_GET_ENBID_FROM_PEERID(_eNbId, _peerId) {\
    U32 i;\
   _eNbId = 0;\
    for(i = 0; i < WR_X2AP_MAX_PEERS; i++)\
    {\
        if(wrEmmCb.x2ApEnbIdLst[i].peerId == _peerId)\
        {\
            _eNbId = wrEmmCb.x2ApEnbIdLst[i].enbId;\
            break;\
        }\
    }\
}

/* set the configured eNbId from SM to the list and get the peerId */
/* KW fix for LTE_ADV */
#define WR_GET_X2AP_PEER_ID(_peerId, eNBId) {\
   U32 i;\
   for(i = 0; i < WR_X2AP_MAX_PEERS; i++)\
   {\
      if(wrEmmCb.x2ApEnbIdLst[i].enbId  == eNBId)\
      {\
         _peerId=wrEmmCb.x2ApEnbIdLst[i].peerId;\
         break;\
      }\
      if(wrEmmCb.x2ApEnbIdLst[i].peerId == 0)\
      {\
         _peerId = i + 1; \
         wrEmmCb.x2ApEnbIdLst[i].peerId = _peerId;\
         wrEmmCb.x2ApEnbIdLst[i].enbId  = eNBId;\
         break;\
      }\
   }\
}

/* set the peerId from X2AP and eNBId to the list */
#define WR_SET_X2AP_PEER_ID(_peerId, _eNBId) {\
   U32 i;\
   for(i = 0; i < WR_X2AP_MAX_PEERS; i++)\
   {\
      if(wrEmmCb.x2ApEnbIdLst[i].peerId == 0)\
      {\
         wrEmmCb.x2ApEnbIdLst[i].enbId  = _eNBId;\
         wrEmmCb.x2ApEnbIdLst[i].peerId = _peerId;\
         break;\
      }\
   }\
}

/* set the peerId from X2AP and eNBId to the list */
#define WR_UPDATE_X2AP_ENB_ID(_peerId, _eNBId) {\
   U32 i;\
   for(i = 0; i < WR_X2AP_MAX_PEERS; i++)\
   {\
      if(wrEmmCb.x2ApEnbIdLst[i].peerId == _peerId)\
      {\
         wrEmmCb.x2ApEnbIdLst[i].enbId  = _eNBId;\
         break;\
      }\
   }\
}

/* clear peerId and eNBId from the list */
#define WR_CLEAR_X2AP_PEER_ID(_peerId) {\
   U32 i;\
   for(i = 0; i < WR_X2AP_MAX_PEERS; i++)\
   {\
      if(wrEmmCb.x2ApEnbIdLst[i].peerId == _peerId)\
      {\
         wrEmmCb.x2ApEnbIdLst[i].enbId  = 0;\
         wrEmmCb.x2ApEnbIdLst[i].peerId = 0;\
         break;\
      }\
   }\
}

/* clear the duplicate peerId and eNBId from the list for the received eNBId */
#define WR_FREE_DUPICATE_PEERID_FROM_ENBID(_eNbId, _peerId) {\
   U32 i;\
   for(i = 0; i < WR_X2AP_MAX_PEERS; i++)\
   {\
      if((wrEmmCb.x2ApEnbIdLst[i].enbId == _eNbId) &&\
         (_peerId != wrEmmCb.x2ApEnbIdLst[i].peerId))\
         {\
             wrEmmCb.x2ApEnbIdLst[i].enbId  = 0;\
             wrEmmCb.x2ApEnbIdLst[i].peerId = 0;\
             break;\
         }\
   }\
}
/* SAM_IFHC - end */

/* extern declarations for functions provided by MME submodule            */
EXTERN S16 wrEmmMmeBldResetAck (S1apPdu *pdu, S1apPdu **rstAckPdu);
EXTERN S16 wrEmmMmeAddMme(WrMmeId mmeId, U32 ipAddr, U16 port);
EXTERN S16 wrEmmMmeGetMmeByMmec(U8 mmeCode, WrMmeId *mmeId);
EXTERN S16 wrEmmMmeGetMme(WrPlmnId plmn, U16 grp, U8 mmec, WrMmeId *mmeId);
EXTERN S16 wrEmmMmeGrpidMmecEqual(U8 mmeId, U16 grpId, U8 mmeCode);
EXTERN S16 wrEmmMmeSetup(WrMmeId mmeId);
EXTERN S16 wrEmmMmeAddMme(WrMmeId mmeId, U32 ipAddr, U16 port);
EXTERN WrHrlDecisionRet wrChkForFrbRat(WrForbRatTyp forbRatTyp, WrHoRstrctInfo *hoRstrctInfo);
/* Fix for ccpu00125227 */ 
EXTERN S16 wrEmmMmeGetMmeByMmeGrpId(U32 estCause, U16 mmeGrpId, WrMmeId *mmeId);
EXTERN S16 wrEmmMmeGetMme(WrPlmnId plmn, U16 grp, U8 mmec, WrMmeId *mmeId);
/* sri - HOFIX */
EXTERN WrMmeCb *wrEmmMmeGetMmeByPeerId(U32 peerId);
EXTERN WrMmeCb *wrEmmMmeGetMmeByPlmn(WrPlmnId *plmnId);
EXTERN S16 wrEmmMmeGrpidMmecEqual(U8 mmeId, U16 grpId, U8 mmeCode);
EXTERN S16 wrEmmMmeSetup(WrMmeId mmeId);
EXTERN S16 wrEmmMmeSendCfgUpdate(WrMmeId mmeId);
EXTERN S16 wrEmmMmePrcRcvdPdu(U32 peerId, S1apPdu *pdu);
EXTERN S16 wrEmmMmePrcStaInd(SztStaInd *sztSta);

/* Extern declarations for functions provided by PLMN sub module          */
EXTERN S16 wrEmmPlmnAddMme(WrPlmnId *plmnId, WrMmeId mmeId);
EXTERN S16 wrEmmPlmnDelMme(WrPlmnId *plmnId, WrMmeId mmeId);
EXTERN S16 wrEmmPlmnAddPlmn(WrPlmnInfo *plmnInfo);
EXTERN S16 wrEmmPlmnGetMmeByPlmn(WrPlmnId *plmnId, WrMmeId *mmeId);
EXTERN S16 wrEmmPlmnGetMme(WrPlmnId *plmn, U16 grpId, U8 mmec, WrMmeId *mmeId);
EXTERN WrHrlDecisionRet wrMbltyRestrc
(
WrMobilityType    mbltyTyp,
PTR               *neighCellCb,
PTR               *neighEnbCb,
U8                *hoTyp,
WrPlmnId          *fnlzdPLMN,
WrHoRstrctInfo    *hoRstrctInfo
);

EXTERN Bool wrEmmChkServPlmnHoUe
(
WrCellId          cellId,
WrPlmnId          srvngPlmn
);

EXTERN U8 wrEmmGetNosOfBdcstPlmn(WrCellId  cellId);
EXTERN WrEutranNeighCellCb* wrEmmGetNeighEutraCell
(
WrCellId      cellId,
U32           carrFreq,
U16           pci
 );

EXTERN Bool wrEmmPlmnPlmnsEqual
(
WrPlmnId                     *plmnId1,
WrPlmnId                     *plmnId2
);

/* Extern declarations for functions provided by PBM submodule            */
EXTERN S16 wrEmmPbmCellInit(WrCellCb *);
EXTERN S16 wrEmmPbmUpdateSis(WrCellCb *, U32 , WrSibCfg *);
EXTERN S16 wrEmmPbmSibConfig(U16 cellId);
EXTERN S16 wrEmmPbmModifyPlmn( WrPlmnId *plmnId, Bool activate);
EXTERN S16 wrPagHdlTmrInd(RgrTtiIndInfo *);

/* Extern declarations for functions provided by CELL submodule           */
EXTERN S16 wrEmmCellAdd(WrCellId cellId);


EXTERN S16 wrEmmCellSchdCfg(WrCellId cellId);
EXTERN S16 wrEmmCellDelSchdCfg(WrCellId cellId);
EXTERN S16 wrEmmCellSchdReCfg(WrCellId cellId);
EXTERN S16 wrEmmCellSchdSiCfg (RgrSiCfg *cfg, WrCellCb *cellCb);
EXTERN S16 wrEmmCellSchdRachCfg ( RgrRachCfg *cfg, WrCellCb *cellCb);
EXTERN S16 wrFillNhuCellCfgReq (WrCellCb *cellCb);
EXTERN S16 wrFillNhuCellDelCfgReq (U16 cellId);
EXTERN S16 wrFillCtfCellCfgReq (U16  cellId);
EXTERN S16 wrFillCtfCellReCfgReq (U32  cellId);
EXTERN S16 wrFillCtfCellDelCfgReq (U16  cellId);
/*EXTERN S16 wrEmmGetMme (WrPlmnId  *plmnId); */
EXTERN S16 wrEmmCellCfgDam(U16 cellId);
EXTERN S16 wrEmmCellGetSelPlmn(WrCellId cellId, U8 selPlmnIdx, WrPlmnId **plmn);
EXTERN S16 wrEmmCellGetSelPlmn(WrCellId cellId, U8 selPlmnIdx, WrPlmnId **plmn);
EXTERN S16 wrEmmCellRrmCfg(WrCellCb *cellCb);
EXTERN S16 wrEmmCellDelRrmCfg(WrCellCb *cellCb);

/* Extern declarations for functions provided by Overload module       */
EXTERN S16 wrEmmSampleData(Void);
EXTERN S16 wrEmmUpdOvrLdStat(Void);
EXTERN S16 wrEmmOamCfg(WrOamAccessBarring  *oamParam);
EXTERN S16 wrEmmPlmnGetMmeIdList(WrPlmnCbMmeList *wrEmmPlmnMmeLst, WrPlmnId wrEmmPlmnId);
EXTERN S16 wrEmmSndEnCfgUpReq(WrNeighEnbCb *nghEnbCb, WrEmmEnbCfgUpd *cellInfo);
EXTERN S16 wrEmCellSchdSiPeriodMap( U16  period, RgrSiPeriodicity *siPeriodicity);

/*-Macro for Overload Functionality-*/
#define CRITICAL 0x08
#define MAJOR    0x04
#define MINOR    0x02
#define NORMAL   0x01

/* Starts - Fix for CR ccpu00123185 */
#define WR_TX_LOW 0  
#define WR_TX_NORMAL 1  
EXTERN S16 wrEmmCellTxPwrReCfg (U16 txType,U8 cellId, U32 transId);
/* Ends - Fix for CR ccpu00123185 */

EXTERN S16 wrEmmNghEnbCfgUptTmrExpiry(WrNeighEnbCb *cb);

/* ANR functions */

EXTERN Void wrEmmAnrInitMeasNrUpdtInfo
(
   WrCellCb                      *cellCb
);
EXTERN S16 wrEmmAnrProcNrUpdtInfo
(
   WrCellCb                   *cellCb, 
   Bool                       isNewFreq
);
EXTERN S16 wrEmmAnrAddEutranNr
(
   WrCellCb                      *cellCb, 
   WrEutranNeighCellCb           *newNr
);
EXTERN S16 wrEmmAnrAddUtraNr
(
   WrCellCb                      *cellCb,
   WrUtraNeighCellCb             *newNr
);
EXTERN S16 wrEmmAnrAddGeranNr
(
   WrCellCb                      *cellCb,
   WrGeranCellCb                 *newNr,
   U16                           bcchArfcn
);
EXTERN Void wrEmmAnrDelEutranNr
(
   WrCellCb                      *cellCb, 
   WrEutranNeighCellCb           *nrNode
);
EXTERN Void wrEmmAnrDelUtraNr
(
   WrCellCb                      *cellCb,
   WrUtraNeighCellCb             *nrNode
);
EXTERN Void wrEmmAnrUpdtEutranModInfo
(
   WrCellCb                      *cellCb,
   WrEutranNeighCellCb           *modNr
);
EXTERN Void wrEmmAnrUpdtUtranModInfo
(
   WrCellCb                      *cellCb,
   WrUtraNeighCellCb             *modNr
);

#if 0
EXTERN S16 wrEmmAnrAddNrFreq 
(
    U8                           cellId,
    LwrNeighFreqAddModCfg        *nbrFreqAdd
);
EXTERN S16 wrEmmAnrDelNrFreq 
(
    U8                           cellId,
    LwrNeighFreqDelCfg           *nbrFreqDel
);
#endif
EXTERN Void wrEmmAnrProcEutranDfltPrdTmrExp
(
   WrEutranNeighCellCb           *neighCellInfo
);

EXTERN S16 wrEmmDelNrFrmServCellLst
(
   WrEutranNeighCellCb *neighCellCb
);
EXTERN  WrEutranNeighCellCb* wrEmmAnrGetEutranNr
(
   WrNrEutranCb               *eutranCb, 
   WrEutranNbrCellInfo        *nrInfo
);
PUBLIC WrUtraNeighCellCb* wrEmmAnrGetUtranNr
(
   WrNrUtranCb                *utranCb,
   WrUtranNrInfo              *nrInfo
);

EXTERN  S16 wrEmmDelNbrEnbServedCellList
(
   WrNeighEnbCb                *neignEnb
);

EXTERN S16 wrEmmGetIntraLteHoNghInfo(U16 cellId, WrHoNghInfo *hoNghInfo);
EXTERN WrEutranNeighCellCb* wrEmmFindEutranNeighCell(U8 cellId, U16 earfcn, 
                   U16 pci);
EXTERN S16 wrEmmNghHdlLocalErrInd(CztLclErrInd *);
EXTERN S16 wrEmmcheckActiveMmme(Void);
EXTERN WrGeranCellCb* wrEmmGetNeighGeranCell
(
 U16                          cellId,
 U16                          arfcn,
 U16                          bsic 
);
EXTERN WrUtraNeighCellCb* wrEmmGetNeighUtraCell
(
 U16                       cellId,
 U16                          arfcn,
 U16                          psc,
 U8 								  duplexMode
);
EXTERN S16 wrEmmGetOffsetFreqFrmArfcn ( U16 cellId, U16 arfcn, U8  *offsetFreq);

EXTERN S16 wrEmmPrcEpochTimerExp
(
 Void                         *cb,
 U8                           wrUmmTmrExpEvnt
);
EXTERN S16 wrEmmPrcTriceTimerExp
(
 Void                         *cb,
 U8                           wrUmmTmrExpEvnt
);
EXTERN S16 wrEmmPrcCellMaxAgeTmrExp
(
Void                         *cb
);
EXTERN Bool wrEmmChkTriceTmr
(
  U16                         cellId
);
EXTERN S16 wrEmmPrcSib8SchdEvnt 
(
   WrCellCb                   *cellCb
);
PUBLIC S16 wrEmmCfgAnrMeasFreqInfo
(
   WrCellCb                   *cellCb
);
PUBLIC WrEutranNeighCellCb* wrEmmGetNeighEnbSrvdCell
(
   WrNeighEnbCb               *neignEnb,
   U16                        pci
);
PUBLIC S16 wrEmmResetCell
(
    WrCellId                    cellId
);
EXTERN S16 wrEmmRrcCellDelCfgCfm(U16  cellId);

PUBLIC S16 wrEmmSchdCellReCfgCfm
(
 U32                          transId,
 U8                           status
);
PUBLIC S16 wrEmmSchdCellDelCfgCfm
(
 U32                          transId,
 U8                           status
);

PUBLIC S16 wrEmmAnrUpdtEutranKeyModInfo
(
 WrCellCb                      *cellCb,
 WrEutraNghCellModInfo         *modCellInfo,
 WrEutranNeighCellCb           *modNr
);


EXTERN S16 wrGetMmeRejectStatus(U32 establishmentCause, U32 mmeId);
EXTERN S16 wrEmmMmeEnbCfgUpdWaitTmrExp(U32 mmeId);
EXTERN S16 wrEmmMmeEnbCfgRspTmrExp(U32 mmeId);
EXTERN S16 wrEmmMmeResetRspTmrExp(U32 mmeId);
EXTERN S16 wrEmmGetOvldStatus(U32 mmeId);
EXTERN Bool wrEmmGetRelCap(U32 mmeId);
EXTERN Bool wrEmmGetMmeStatus(U32 mmeId);
EXTERN S16 wrEmmMmePrcErrInd(U32 peerId,S1apPdu *pdu);
EXTERN S16 wrPwsStartPrimaryEtws ( WrCellCb *cellCb );
EXTERN S16 wrPwsStartSecondaryEtws( WrCellCb *cellCb);
EXTERN S16 wrPwsStartCmas( WrCellCb *cellCb);
EXTERN S16 wrPwsStartPrimaryEtws ( WrCellCb *cellCb );
EXTERN S16 wrPwsStartSecondaryEtws( WrCellCb *cellCb);
EXTERN S16 wrPwsStartCmas( WrCellCb *cellCb);
EXTERN Void wrEmmMmeBldEnbConfigTranForAllNegh ( WrCellCb   *cellCb);
EXTERN S16 wrEmmCellDedCrntiRange(U8 cellId,U16 *minVal,U16 *maxVal); /* THURSDAY: FIX */
EXTERN S16 wrEmmStartTriceTmr ( WrCellCb                 *cellCb);
EXTERN S16 wrEmmMmeFillPrimaryGummei(WrGummei *gummei,U16 mmeId);
EXTERN S16 wrEmmIsPrdAnrTmrsRunning(WrCellId cellId,Bool  *running);
EXTERN S16 wrGetMmeIdfromPlmnId(WrGummei  *gummei, WrMmeId  *mmeId, U32  establishCause);
/* Added for eCSFB */
EXTERN WrPlmnCb *wrEmmPlmnGetPlmnCb(WrPlmnId *plmnId);
#if 0
EXTERN S16 wrEmmMmePrcRcvdPdu
(
U32                          peerId,
S1apPdu                      *pdu
);
#endif
/* LTE_ADV starts */
PUBLIC S16 wrEmmCellSchdReCfgLteAdvFeatures
(
   CmLteCellId                cellId, 
   LwrLteAdvancedConfigTrans  *lteAdvancedTrans
);    /* KW fix for LTE_ADV */    
/* LTE_ADV ends */

/* CSG_DEV start */
EXTERN Bool wrEmmIsCsgCell (U16 cellId);
EXTERN U32 wrEmmGetCsgIdFrmCellId (U16 cellId);
EXTERN U8 wrEmmGetAccessModeFrmCellId (U16 cellId);
EXTERN S16 wrEmmCsgNbrUpdCsgInfoFrmExtnIe (TknBStr32 *csg_Id,
      WrEutranNeighCellCb *eutranNrCellCb);
EXTERN S16 wrUmmCsgPrntNbrCsgInfo(U16 cellId);
EXTERN S16 wrUmmCsgPrntCellInfo(U16 cellId);
EXTERN S16 wrEmmCsgSetNbrAccessMode (U16 cellId, U16 nbrPci,
      Bool isCsgIdPres, U8 *accessMode);
EXTERN Bool wrEmmCsgIsReportCgiNeeded (U8 ueIdx, U8 isEvntAnr,
      WrCellCb *cellCb, WrEutranNeighCellCb *eutNbrCellCb);

EXTERN S16 wrEmmCellSchdReCfgDynCfi(WrCellCb *cellCb); 

EXTERN S16 wrEmmSonSchdPrbCfg(WrCellCb   *cellCb);

/* CSG_DEV ends */
/* Ue history info - start */
PUBLIC S16 wrEmmGetCellSizeType
(
WrCellId                     cellId,
U8                           *cellSizeType
);
/* Ue history info - end */
PUBLIC S16 wrEmmGetPeerId
(
 U32 eNodeBId,
 U32  *peerId
 );

EXTERN S16 wrEmmTrigRirProc
(
 WrCellCb                    *cellCb,
 U8                          reptType
);
EXTERN S16 wrEmmRimErrInd
(
  WrRimErrInd  *rimErrInd
);

EXTERN S16 wrEmmRimInfoRsp
(
  WrEmmRimRspInfo *riInfo
);
EXTERN S16 wrEmmRimTrigRirStop
(
  WrCellCb     *cellCb
);

EXTERN Void wrEmmDelGeranSi
(
 WrGeranCellCb             *geranCellCb
);

PUBLIC S16 wrEmmRimNghStateChange
(
 WrCellCb                    *cellCb
);

EXTERN Bool wrFindRegLaiPlmnCell
(
WrRegLai				*regLai,
WrUtraNeighCellCb *neighCellCb,
WrHoRstrctInfo    *hoRstrctInfo
);

EXTERN CmLListCp* wrEmmGetUtraFddFreqLst
(
WrCellId                  cellId
);

EXTERN CmLListCp* wrEmmGetUtraTddFreqLst
(
WrCellId                  cellId
);

EXTERN  WrNrUtranFreqCb* wrEmmAnrGetFreqNode
(
U16                           arfcnDl,
WrNrUtranCb                   *utranCb
);

EXTERN  WrNrUtranCb* wrEmmGetUtranCb
(
WrCellId                     cellId,
U8                            duplexMode
);

EXTERN S16 wrEmmTrigUtraRirProc
(
 CmLListCp                   *lst,
 U8                          reptType,
 U8                          *status,
 WrRimReqInfo                *rimReqInfo
);
 
EXTERN Bool wrEmmGetNeighUtraTddSIInfo
(
 U16         psc,
 U16         arfcn,
 U16         cellId,
 TknStrOSXL  *utraSI
 );


EXTERN S16 wrEmmCellSchdReCfgTmMode
(
WrCellCb                   *cellCb
);
EXTERN Void wrEmmCheckAgeAndDeleteCell
(
WrCellCb                     *cellCb,
WrEutranNeighCellCb          *nbrCell
);

EXTERN S16 wrEmmStopNRTAgeTimer
(
Void
);
EXTERN S16 wrEmmCellSchdCmdCpuOvrldReCfg
(
U16                          cellId,
U8                       instruction
);
#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __WR_EMM_H__ */


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
