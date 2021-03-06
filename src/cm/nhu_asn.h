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
               

     File:     wr_smm_init.c

     Sid:      $SID$ 

     Prg:      Sriky 

*********************************************************************21*/
#ifndef __NHUASNH__
#define __NHUASNH__ 

#define NHUCHKUNPKPTR(func, val, ptr, mBuf) {\
   S16 ret;\
   if ((ret = func(val, mBuf, ptr)) != ROK)\
      RETVALUE(ret);\
}
#define Nhuid_Invalid 0xffffffff

#define SOUNDINGRS_UL_CONFIGCMN_RELEASE 0
#define SOUNDINGRS_UL_CONFIGCMN_SETUP 1

#define SUBFRAMEALLOCN_ONEFRAME 0
#define SUBFRAMEALLOCN_FOURFRAMES 1

#define FOLLOWINGARFCNS_EXPLICITLSTOFARFCNS 0
#define FOLLOWINGARFCNS_EQUALLYSPACEDARFCNS 1
#define FOLLOWINGARFCNS_VARIABLEBITMAPOFARFCNS 2

#define CDMA_SYSTIME_SYNCHRONOUSSYSTIME 0
#define CDMA_SYSTIME_ASYNCHRONOUSSYSTIME 1

#define SYSTEMTIMEINFO_R11_EXPLICITVALUE 0
#define SYSTEMTIMEINFO_R11_DEFAULTVALUE 1

#define PARAMETERSCDMA2000_R11_EXPLICITVALUE 0
#define PARAMETERSCDMA2000_R11_DEFAULTVALUE 1

#define EAB_PARAM_R11_EAB_CMN_R11 0
#define EAB_PARAM_R11_EAB_PERPLMN_LST_R11 1

#define MEMBER_SIB2 0
#define MEMBER_SIB3 1
#define MEMBER_SIB4 2
#define MEMBER_SIB5 3
#define MEMBER_SIB6 4
#define MEMBER_SIB7 5
#define MEMBER_SIB8 6
#define MEMBER_SIB9 7
#define MEMBER_SIB10 8
#define MEMBER_SIB11 9
#define MEMBER_SIB12_V920 10
#define MEMBER_SIB13_V920 11
#define MEMBER_SIB14_V1130 12
#define MEMBER_SIB15_V1130 13
#define MEMBER_SIB16_V1130 14

#define CRITICALEXTNS_SYSTEMINFO_R8 0
#define CRITICALEXTNS_CRITICALEXTNSFUTURE 1

#define C1_SYSTEMINFO 0
#define C1_SYSTEMINFOBLOCKTYP1 1
#define BCCH_DL_SCH_MSGTYP_C1 0
#define BCCH_DL_SCH_MSGTYP_MESSAGECLASSEXTN 1

#define PLMN_ID_R9_PLMN_IDX_R9 0
#define PLMN_ID_R9_EXPLICITVALUE_R9 1

#define C1_MBSFNAREACFG_R9 0
#define C2_MBMSCOUNTINGRQST_R10 0
#define LATER_C2 0
#define LATER_MESSAGECLASSEXTN 1
#define MCCH_MSGTYP_C1 0
#define MCCH_MSGTYP_LATER 1

#define PAGUE_IDENTITY_S_TMSI 0
#define PAGUE_IDENTITY_IMSI 1

#define C1_PAGING 0
#define PCCH_MSGTYP_C1 0
#define PCCH_MSGTYP_MESSAGECLASSEXTN 1

#define RLC_CONFIG_AM 0
#define RLC_CONFIG_UM_BI_DIRECTIONAL 1
#define RLC_CONFIG_UM_UNI_DIRECTIONAL_UL 2
#define RLC_CONFIG_UM_UNI_DIRECTIONAL_DL 3

#define RLC_CONFIG_EXPLICITVALUE 0
#define RLC_CONFIG_DEFAULTVALUE 1
#define LOGICALCHANNELCONFIG_EXPLICITVALUE 0
#define LOGICALCHANNELCONFIG_DEFAULTVALUE 1

#define HEADERCOMPRESSION_NOTUSED 0
#define HEADERCOMPRESSION_ROHC 1

#define LONGDRX_CYCLESTARTOFFSET_SF10 0
#define LONGDRX_CYCLESTARTOFFSET_SF20 1
#define LONGDRX_CYCLESTARTOFFSET_SF32 2
#define LONGDRX_CYCLESTARTOFFSET_SF40 3
#define LONGDRX_CYCLESTARTOFFSET_SF64 4
#define LONGDRX_CYCLESTARTOFFSET_SF80 5
#define LONGDRX_CYCLESTARTOFFSET_SF128 6
#define LONGDRX_CYCLESTARTOFFSET_SF160 7
#define LONGDRX_CYCLESTARTOFFSET_SF256 8
#define LONGDRX_CYCLESTARTOFFSET_SF320 9
#define LONGDRX_CYCLESTARTOFFSET_SF512 10
#define LONGDRX_CYCLESTARTOFFSET_SF640 11
#define LONGDRX_CYCLESTARTOFFSET_SF1024 12
#define LONGDRX_CYCLESTARTOFFSET_SF1280 13
#define LONGDRX_CYCLESTARTOFFSET_SF2048 14
#define LONGDRX_CYCLESTARTOFFSET_SF2560 15
#define DRX_CONFIG_RELEASE 0
#define DRX_CONFIG_SETUP 1

#define LONGDRX_CYCLESTARTOFFSET_V1130_SF60_V1130 0
#define LONGDRX_CYCLESTARTOFFSET_V1130_SF70_V1130 1

#define PHR_CONFIG_RELEASE 0
#define PHR_CONFIG_SETUP 1

#define TWOANTENNAPORTACTIVATED_R10_RELEASE 0
#define TWOANTENNAPORTACTIVATED_R10_SETUP 1
#define SPS_CONFIGDL_RELEASE 0
#define SPS_CONFIGDL_SETUP 1

#define SPS_CONFIGUL_RELEASE 0
#define SPS_CONFIGUL_SETUP 1

#define ACKNACKREPETITION_RELEASE 0
#define ACKNACKREPETITION_SETUP 1

#define TPC_IDX_INDEXOFFORMAT3 0
#define TPC_IDX_INDEXOFFORMAT3A 1

#define TPC_PDCCH_CONFIG_RELEASE 0
#define TPC_PDCCH_CONFIG_SETUP 1

#define CQI_FORMATINDICATORPERIODIC_WIDEBANDCQI 0
#define CQI_FORMATINDICATORPERIODIC_SUBBANDCQI 1
#define CQI_REPORTPERIODIC_RELEASE 0
#define CQI_REPORTPERIODIC_SETUP 1

#define SOUNDINGRS_UL_CONFIGDEDICATED_RELEASE 0
#define SOUNDINGRS_UL_CONFIGDEDICATED_SETUP 1

#define CODEBOOKSUBSETRESTRN_N2TXANTENNA_TM3 0
#define CODEBOOKSUBSETRESTRN_N4TXANTENNA_TM3 1
#define CODEBOOKSUBSETRESTRN_N2TXANTENNA_TM4 2
#define CODEBOOKSUBSETRESTRN_N4TXANTENNA_TM4 3
#define CODEBOOKSUBSETRESTRN_N2TXANTENNA_TM5 4
#define CODEBOOKSUBSETRESTRN_N4TXANTENNA_TM5 5
#define CODEBOOKSUBSETRESTRN_N2TXANTENNA_TM6 6
#define CODEBOOKSUBSETRESTRN_N4TXANTENNA_TM6 7
#define UE_TRANSMITANTENNASELECTION_RELEASE 0
#define UE_TRANSMITANTENNASELECTION_SETUP 1

#define SCHEDULINGRQSTCONFIG_RELEASE 0
#define SCHEDULINGRQSTCONFIG_SETUP 1

#define CODEBOOKSUBSETRESTRN_V920_N2TXANTENNA_TM8_R9 0
#define CODEBOOKSUBSETRESTRN_V920_N4TXANTENNA_TM8_R9 1

#define UE_TRANSMITANTENNASELECTION_RELEASE 0
#define UE_TRANSMITANTENNASELECTION_SETUP 1

#define CQI_REPORTAPERIODIC_R10_RELEASE 0
#define CQI_REPORTAPERIODIC_R10_SETUP 1

#define CQI_FORMATINDICATORPERIODIC_R10_WIDEBANDCQI_R10 0
#define CQI_FORMATINDICATORPERIODIC_R10_SUBBANDCQI_R10 1
#define CSI_CONFIGIDX_R10_RELEASE 0
#define CSI_CONFIGIDX_R10_SETUP 1
#define CQI_REPORTPERIODIC_R10_RELEASE 0
#define CQI_REPORTPERIODIC_R10_SETUP 1

#define SUBFRAMEPATTERNTDD_R10_SUBFRAMECONFIG1_5_R10 0
#define SUBFRAMEPATTERNTDD_R10_SUBFRAMECONFIG0_R10 1
#define SUBFRAMEPATTERNTDD_R10_SUBFRAMECONFIG6_R10 2
#define MEASSUBFRMPATTERN_R10_SUBFRAMEPATTERNFDD_R10 0
#define MEASSUBFRMPATTERN_R10_SUBFRAMEPATTERNTDD_R10 1

#define CSI_SUBFRMPATTERNCONFIG_R10_RELEASE 0
#define CSI_SUBFRMPATTERNCONFIG_R10_SETUP 1

#define CSI_RS_R10_RELEASE 0
#define CSI_RS_R10_SETUP 1
#define ZEROTXPOWERCSI_RS_R10_RELEASE 0
#define ZEROTXPOWERCSI_RS_R10_SETUP 1

#define TWOANTENNAPORTACTIVATEDPUCCH_FORMAT3_R10_RELEASE 0
#define TWOANTENNAPORTACTIVATEDPUCCH_FORMAT3_R10_SETUP 1
#define N1PUCCH_AN_CS_R10_RELEASE 0
#define N1PUCCH_AN_CS_R10_SETUP 1
#define PUCCH_FORMAT_R10_FORMAT3_R10 0
#define PUCCH_FORMAT_R10_CHANNELSELECTION_R10 1

#define SRS_ACTIVATEAP_R10_RELEASE 0
#define SRS_ACTIVATEAP_R10_SETUP 1
#define SOUNDINGRS_UL_CONFIGDEDICATEDAPERIODIC_R10_RELEASE 0
#define SOUNDINGRS_UL_CONFIGDEDICATEDAPERIODIC_R10_SETUP 1

#define MBSFN_SUBFRMCONFIGLST_R11_RELEASE 0
#define MBSFN_SUBFRMCONFIGLST_R11_SETUP 1

#define SUBFRAMEPATTERNCONFIG_R11_RELEASE 0
#define SUBFRAMEPATTERNCONFIG_R11_SETUP 1
#define CONFIG_R11_RELEASE 0
#define CONFIG_R11_SETUP 1

#define DMRS_CONFIG_R11_RELEASE 0
#define DMRS_CONFIG_R11_SETUP 1

#define MBSFN_SUBFRMCONFIGLST_R11_RELEASE 0
#define MBSFN_SUBFRMCONFIGLST_R11_SETUP 1

#define CQI_FORMATINDICATORPERIODIC_R11_WIDEBANDCQI_R11 0
#define CQI_FORMATINDICATORPERIODIC_R11_SUBBANDCQI_R11 1
#define CSI_CONFIGIDX_R11_RELEASE 0
#define CSI_CONFIGIDX_R11_SETUP 1

#define N1PUCCH_AN_CS_V1130_RELEASE 0
#define N1PUCCH_AN_CS_V1130_SETUP 1
#define NPUCCH_PARAM_R11_RELEASE 0
#define NPUCCH_PARAM_R11_SETUP 1

#define PUSCH_DMRS_R11_RELEASE 0
#define PUSCH_DMRS_R11_SETUP 1

#define ANTENNAINFO_EXPLICITVALUE 0
#define ANTENNAINFO_DEFAULTVALUE 1
#define ANTENNAINFO_R10_EXPLICITVALUE_R10 0
#define ANTENNAINFO_R10_DEFAULTVALUE 1
#define ADDITIONALSPECTRUMEMISSIONCA_R10_RELEASE 0
#define ADDITIONALSPECTRUMEMISSIONCA_R10_SETUP 1

#define RLF_TMRSANDCONSTS_R9_RELEASE 0
#define RLF_TMRSANDCONSTS_R9_SETUP 1

#define MEASSUBFRMPATTERNPCELL_R10_RELEASE 0
#define MEASSUBFRMPATTERNPCELL_R10_SETUP 1

#define NEIGHCELLSCRS_INFO_R11_RELEASE 0
#define NEIGHCELLSCRS_INFO_R11_SETUP 1

#define MAC_MAINCONFIG_EXPLICITVALUE 0
#define MAC_MAINCONFIG_DEFAULTVALUE 1

#define C1_RRCCONREESTB_R8 0
#define C1_SPARE7__18 1
#define C1_SPARE6__18 2
#define C1_SPARE5__18 3
#define C1_SPARE4__18 4
#define C1_SPARE3__18 5
#define C1_SPARE2__18 6
#define C1_SPARE1__18 7
#define CRITICALEXTNS_C1 0
#define CRITICALEXTNS_CRITICALEXTNSFUTURE 1

#define CRITICALEXTNS_RRCCONREESTBREJ_R8 0
#define CRITICALEXTNS_CRITICALEXTNSFUTURE 1

#define C1_RRCCONREJ_R8 0
#define C1_SPARE3__19 1
#define C1_SPARE2__19 2
#define C1_SPARE1__19 3
#define CRITICALEXTNS_C1 0
#define CRITICALEXTNS_CRITICALEXTNSFUTURE 1

#define C1_RRCCONSETUP_R8 0
#define C1_SPARE7__21 1
#define C1_SPARE6__21 2
#define C1_SPARE5__21 3
#define C1_SPARE4__21 4
#define C1_SPARE3__21 5
#define C1_SPARE2__21 6
#define C1_SPARE1__21 7
#define CRITICALEXTNS_C1 0
#define CRITICALEXTNS_CRITICALEXTNSFUTURE 1

#define C1_RRCCONREESTB 0
#define C1_RRCCONREESTBREJ 1
#define C1_RRCCONREJ 2
#define C1_RRCCONSETUP 3
#define DL_CCCH_MSGTYP_C1 0
#define DL_CCCH_MSGTYP_MESSAGECLASSEXTN 1

#define CRITICALEXTNS_CSFBPARAMSRSPCDMA2000_R8 0
#define CRITICALEXTNS_CRITICALEXTNSFUTURE 1

#define DEDICATEDINFOTYP_DEDICATEDINFONAS 0
#define DEDICATEDINFOTYP_DEDICATEDINFOCDMA2000_1XRTT 1
#define DEDICATEDINFOTYP_DEDICATEDINFOCDMA2000_HRPD 2

#define C1_DLINFOTFR_R8 0
#define C1_SPARE3__5 1
#define C1_SPARE2__5 2
#define C1_SPARE1__5 3
#define CRITICALEXTNS_C1 0
#define CRITICALEXTNS_CRITICALEXTNSFUTURE 1

#define C1_HANDOVERFROMEUTRAPREPRQST_R8 0
#define C1_SPARE3__6 1
#define C1_SPARE2__6 2
#define C1_SPARE1__6 3
#define CRITICALEXTNS_C1 0
#define CRITICALEXTNS_CRITICALEXTNSFUTURE 1

#define SI_ORPSI_GERAN_SI 0
#define SI_ORPSI_GERAN_PSI 1

#define TARGETRAT_TYP_GERAN 0

#define PURPOSE_HANDOVER 0
#define PURPOSE_CELLCHANGEORDER 1

#define PURPOSE_HANDOVER 0
#define PURPOSE_CELLCHANGEORDER 1
#define PURPOSE_E_CSFB_R9 2

#define C1_MOBILITYFROMEUTRACMD_R8 0
#define C1_MOBILITYFROMEUTRACMD_R9 1
#define C1_SPARE2__13 2
#define C1_SPARE1__13 3
#define CRITICALEXTNS_C1 0
#define CRITICALEXTNS_CRITICALEXTNSFUTURE 1

#define MEASSUBFRMPATTERNCONFIGNEIGH_R10_RELEASE 0
#define MEASSUBFRMPATTERNCONFIGNEIGH_R10_SETUP 1

#define CELLSTOADDMODLST_CELLSTOADDMODLSTUTRA_FDD 0
#define CELLSTOADDMODLST_CELLSTOADDMODLSTUTRA_TDD 1
#define CELLFORWHICHTOREPORTCGI_UTRA_FDD 0
#define CELLFORWHICHTOREPORTCGI_UTRA_TDD 1

#define MEASOBJECT_MEASOBJECTEUTRA 0
#define MEASOBJECT_MEASOBJECTUTRA 1
#define MEASOBJECT_MEASOBJECTGERAN 2
#define MEASOBJECT_MEASOBJECTCDMA2000 3

#define THRESHOLDEUTRA_THRESHOLD_RSRP 0
#define THRESHOLDEUTRA_THRESHOLD_RSRQ 1

#define EVENTID_EVENTA1 0
#define EVENTID_EVENTA2 1
#define EVENTID_EVENTA3 2
#define EVENTID_EVENTA4 3
#define EVENTID_EVENTA5 4
#define EVENTID_EVENTA6_R10 5
#define TRIGGERTYP_EVENT 0
#define TRIGGERTYP_PERIODICAL 1

#define THRESHOLDUTRA_UTRA_RSCP 0
#define THRESHOLDUTRA_UTRA_ECN0 1

#define B1_THRESHOLD_B1_THRESHOLDUTRA 0
#define B1_THRESHOLD_B1_THRESHOLDGERAN 1
#define B1_THRESHOLD_B1_THRESHOLDCDMA2000 2
#define B2_THRESHOLD2_B2_THRESHOLD2UTRA 0
#define B2_THRESHOLD2_B2_THRESHOLD2GERAN 1
#define B2_THRESHOLD2_B2_THRESHOLD2CDMA2000 2
#define EVENTID_EVENTB1 0
#define EVENTID_EVENTB2 1
#define TRIGGERTYP_EVENT 0
#define TRIGGERTYP_PERIODICAL 1

#define REPORTCONFIG_REPORTCONFIGEUTRA 0
#define REPORTCONFIG_REPORTCONFIGINTERRAT 1

#define GAPOFFSET_GP0 0
#define GAPOFFSET_GP1 1
#define MEASGAPCONFIG_RELEASE 0
#define MEASGAPCONFIG_SETUP 1

#define SPEEDSTATEPARS_RELEASE 0
#define SPEEDSTATEPARS_SETUP 1

#define HANDOVERTYP_INTRALTE 0
#define HANDOVERTYP_INTERRAT 1

#define POWERPREFINDCONFIG_R11_RELEASE 0
#define POWERPREFINDCONFIG_R11_SETUP 1

#define SCHEDULINGCELLINFO_R10_OWN_R10 0
#define SCHEDULINGCELLINFO_R10_OTHER_R10 1

#define C1_RRCCONRECFGN_R8 0
#define C1_SPARE7__17 1
#define C1_SPARE6__17 2
#define C1_SPARE5__17 3
#define C1_SPARE4__17 4
#define C1_SPARE3__17 5
#define C1_SPARE2__17 6
#define C1_SPARE1__17 7
#define CRITICALEXTNS_C1 0
#define CRITICALEXTNS_CRITICALEXTNSFUTURE 1

#define REDIRECTEDCARRIERINFO_EUTRA 0
#define REDIRECTEDCARRIERINFO_GERAN 1
#define REDIRECTEDCARRIERINFO_UTRA_FDD 2
#define REDIRECTEDCARRIERINFO_UTRA_TDD 3
#define REDIRECTEDCARRIERINFO_CDMA2000_HRPD 4
#define REDIRECTEDCARRIERINFO_CDMA2000_1XRTT 5
#define REDIRECTEDCARRIERINFO_UTRA_TDD_R10 6

#define CELLINFOLST_R9_GERAN_R9 0
#define CELLINFOLST_R9_UTRA_FDD_R9 1
#define CELLINFOLST_R9_UTRA_TDD_R9 2
#define CELLINFOLST_R9_UTRA_TDD_R10 3

#define C1_RRCCONRLS_R8 0
#define C1_SPARE3__20 1
#define C1_SPARE2__20 2
#define C1_SPARE1__20 3
#define CRITICALEXTNS_C1 0
#define CRITICALEXTNS_CRITICALEXTNSFUTURE 1

#define C1_SECURITYMODECMD_R8 0
#define C1_SPARE3__23 1
#define C1_SPARE2__23 2
#define C1_SPARE1__23 3
#define CRITICALEXTNS_C1 0
#define CRITICALEXTNS_CRITICALEXTNSFUTURE 1

#define C1_UECAPENQUIRY_R8 0
#define C1_SPARE3__25 1
#define C1_SPARE2__25 2
#define C1_SPARE1__25 3
#define CRITICALEXTNS_C1 0
#define CRITICALEXTNS_CRITICALEXTNSFUTURE 1

#define C1_COUNTERCHECK_R8 0
#define C1_SPARE3__4 1
#define C1_SPARE2__4 2
#define C1_SPARE1__4 3
#define CRITICALEXTNS_C1 0
#define CRITICALEXTNS_CRITICALEXTNSFUTURE 1

#define C1_UEINFORQST_R9__27 0
#define C1_SPARE3__27 1
#define C1_SPARE2__27 2
#define C1_SPARE1__27 3
#define CRITICALEXTNS_C1 0
#define CRITICALEXTNS_CRITICALEXTNSFUTURE 1

#define AREACFG_R10_CELLGLOBALIDLST_R10 0
#define AREACFG_R10_TRACKINGAREACODELST_R10 1

#define C1_LOGGEDMEASUREMENTCFG_R10__9 0
#define C1_SPARE3__9 1
#define C1_SPARE2__9 2
#define C1_SPARE1__9 3
#define CRITICALEXTNS_C1 0
#define CRITICALEXTNS_CRITICALEXTNSFUTURE 1

#define SUBFRAMECONFIGPATTERN_R10_SUBFRAMECONFIGPATTERNFDD_R10 0
#define SUBFRAMECONFIGPATTERN_R10_SUBFRAMECONFIGPATTERNTDD_R10 1
#define TYPE01_R10_NRB6_R10 0
#define TYPE01_R10_NRB15_R10 1
#define TYPE01_R10_NRB25_R10 2
#define TYPE01_R10_NRB50_R10 3
#define TYPE01_R10_NRB75_R10 4
#define TYPE01_R10_NRB100_R10 5
#define TYPE2_R10_NRB6_R10 0
#define TYPE2_R10_NRB15_R10 1
#define TYPE2_R10_NRB25_R10 2
#define TYPE2_R10_NRB50_R10 3
#define TYPE2_R10_NRB75_R10 4
#define TYPE2_R10_NRB100_R10 5
#define RESOURCEBLOCKASSIGNMENT_R10_TYPE01_R10 0
#define RESOURCEBLOCKASSIGNMENT_R10_TYPE2_R10 1
#define DEMODULATIONRS_R10_INTERLEAVING_R10 0
#define DEMODULATIONRS_R10_NOINTERLEAVING_R10 1
#define TDD_CHANNELSELECTIONMULTIPLEXINGBUNDLING 0
#define TDD_FALLBACKFORFORMAT3 1
#define PUCCH_CONFIG_R10_TDD 0
#define PUCCH_CONFIG_R10_FDD 1

#define C1_RNRECFGN_R10__15 0
#define C1_SPARE3__15 1
#define C1_SPARE2__15 2
#define C1_SPARE1__15 3
#define CRITICALEXTNS_C1 0
#define CRITICALEXTNS_CRITICALEXTNSFUTURE 1

#define C1_CSFBPARAMSRSPCDMA2000 0
#define C1_DLINFOTFR 1
#define C1_HANDOVERFROMEUTRAPREPRQST 2
#define C1_MOBILITYFROMEUTRACMD 3
#define C1_RRCCONRECFGN 4
#define C1_RRCCONRLS 5
#define C1_SECURITYMODECMD 6
#define C1_UECAPENQUIRY 7
#define C1_COUNTERCHECK 8
#define C1_UEINFORQST_R9__1 9
#define C1_LOGGEDMEASUREMENTCFG_R10__1 10
#define C1_RNRECFGN_R10__1 11
#define C1_SPARE4__1 12
#define C1_SPARE3__1 13
#define C1_SPARE2__1 14
#define C1_SPARE1__1 15
#define DL_DCCH_MSGTYP_C1 0
#define DL_DCCH_MSGTYP_MESSAGECLASSEXTN 1

#define CRITICALEXTNS_RRCCONREESTBRQST_R8 0
#define CRITICALEXTNS_CRITICALEXTNSFUTURE 1

#define INITUE_IDENTITY_S_TMSI 0
#define INITUE_IDENTITY_RANDOMVALUE 1

#define CRITICALEXTNS_RRCCONRQST_R8 0
#define CRITICALEXTNS_CRITICALEXTNSFUTURE 1

#define C1_RRCCONREESTBRQST 0
#define C1_RRCCONRQST 1
#define UL_CCCH_MSGTYP_C1 0
#define UL_CCCH_MSGTYP_MESSAGECLASSEXTN 1

#define CRITICALEXTNS_CSFBPARAMSRQSTCDMA2000_R8 0
#define CRITICALEXTNS_CRITICALEXTNSFUTURE 1

#define PHYSCELLID_FDD 0
#define PHYSCELLID_TDD 1

#define CELLGLOBALIDCDMA2000_CELLGLOBALID1XRTT 0
#define CELLGLOBALIDCDMA2000_CELLGLOBALIDHRPD 1

#define LOCATIONCOORDINATES_R10_ELLIPSOID_POINT_R10 0
#define LOCATIONCOORDINATES_R10_ELLIPSOIDPOINTWITHALTITUDE_R10 1
#define LOCATIONCOORDINATES_R10_ELLIPSOIDPOINTWITHUNCERTAINTYCIRCLE_R11 2
#define LOCATIONCOORDINATES_R10_ELLIPSOIDPOINTWITHUNCERTAINTYELLIPSE_R11 3
#define LOCATIONCOORDINATES_R10_ELLIPSOIDPOINTWITHALTITUDEANDUNCERTAINTYELLIPSOID_R11 4
#define LOCATIONCOORDINATES_R10_ELLIPSOIDARC_R11 5
#define LOCATIONCOORDINATES_R10_POLYGON_R11 6

#define MEASRESULTNEIGHCELLS_MEASRESULTLSTEUTRA 0
#define MEASRESULTNEIGHCELLS_MEASRESULTLSTUTRA 1
#define MEASRESULTNEIGHCELLS_MEASRESULTLSTGERAN 2
#define MEASRESULTNEIGHCELLS_MEASRESULTSCDMA2000 3

#define C1_MEASUREMENTREPORT_R8 0
#define C1_SPARE7__12 1
#define C1_SPARE6__12 2
#define C1_SPARE5__12 3
#define C1_SPARE4__12 4
#define C1_SPARE3__12 5
#define C1_SPARE2__12 6
#define C1_SPARE1__12 7
#define CRITICALEXTNS_C1 0
#define CRITICALEXTNS_CRITICALEXTNSFUTURE 1

#define CRITICALEXTNS_RRCCONRECFGNCOMPL_R8 0
#define CRITICALEXTNS_CRITICALEXTNSFUTURE 1

#define CRITICALEXTNS_RRCCONREESTBCOMPL_R8 0
#define CRITICALEXTNS_CRITICALEXTNSFUTURE 1

#define C1_RRCCONSETUPCOMPL_R8 0
#define C1_SPARE3__22 1
#define C1_SPARE2__22 2
#define C1_SPARE1__22 3
#define CRITICALEXTNS_C1 0
#define CRITICALEXTNS_CRITICALEXTNSFUTURE 1

#define CRITICALEXTNS_SECURITYMODECOMPL_R8 0
#define CRITICALEXTNS_CRITICALEXTNSFUTURE 1

#define CRITICALEXTNS_SECURITYMODEFAIL_R8 0
#define CRITICALEXTNS_CRITICALEXTNSFUTURE 1

#define C1_UECAPINFO_R8 0
#define C1_SPARE7__26 1
#define C1_SPARE6__26 2
#define C1_SPARE5__26 3
#define C1_SPARE4__26 4
#define C1_SPARE3__26 5
#define C1_SPARE2__26 6
#define C1_SPARE1__26 7
#define CRITICALEXTNS_C1 0
#define CRITICALEXTNS_CRITICALEXTNSFUTURE 1

#define C1_ULHOVRPREPTFR_R8 0
#define C1_SPARE3__29 1
#define C1_SPARE2__29 2
#define C1_SPARE1__29 3
#define CRITICALEXTNS_C1 0
#define CRITICALEXTNS_CRITICALEXTNSFUTURE 1

#define DEDICATEDINFOTYP_DEDICATEDINFONAS 0
#define DEDICATEDINFOTYP_DEDICATEDINFOCDMA2000_1XRTT 1
#define DEDICATEDINFOTYP_DEDICATEDINFOCDMA2000_HRPD 2

#define C1_ULINFOTFR_R8 0
#define C1_SPARE3__30 1
#define C1_SPARE2__30 2
#define C1_SPARE1__30 3
#define CRITICALEXTNS_C1 0
#define CRITICALEXTNS_CRITICALEXTNSFUTURE 1

#define CRITICALEXTNS_COUNTERCHECKRSP_R8 0
#define CRITICALEXTNS_CRITICALEXTNSFUTURE 1

#define FAILEDPCELLID_R10_CELLGLOBALID_R10 0
#define FAILEDPCELLID_R10_PCI_ARFCN_R10 1
#define PHYSCELLID_R11_FDD_R11 0
#define PHYSCELLID_R11_TDD_R11 1

#define C1_UEINFORSP_R9__28 0
#define C1_SPARE3__28 1
#define C1_SPARE2__28 2
#define C1_SPARE1__28 3
#define CRITICALEXTNS_C1 0
#define CRITICALEXTNS_CRITICALEXTNSFUTURE 1

#define CARRIERFREQ_R9_EUTRA_R9 0
#define CARRIERFREQ_R9_UTRA_R9 1
#define CARRIERFREQ_R9_EUTRA2_V9E0 2

#define C1_PROXIMITYIND_R9__14 0
#define C1_SPARE3__14 1
#define C1_SPARE2__14 2
#define C1_SPARE1__14 3
#define CRITICALEXTNS_C1 0
#define CRITICALEXTNS_CRITICALEXTNSFUTURE 1

#define C1_RNRECFGNCOMPL_R10__16 0
#define C1_SPARE3__16 1
#define C1_SPARE2__16 2
#define C1_SPARE1__16 3
#define CRITICALEXTNS_C1 0
#define CRITICALEXTNS_CRITICALEXTNSFUTURE 1

#define C1_COUNTINGRSP_R10 0
#define C1_SPARE3__10 1
#define C1_SPARE2__10 2
#define C1_SPARE1__10 3
#define CRITICALEXTNS_C1 0
#define CRITICALEXTNS_CRITICALEXTNSFUTURE 1

#define RSTD_INTERFREQIND_R10_START 0
#define RSTD_INTERFREQIND_R10_STOP 1

#define C1_INTERFREQRSTDMEASUREMENTIND_R10__8 0
#define C1_SPARE3__8 1
#define C1_SPARE2__8 2
#define C1_SPARE1__8 3
#define CRITICALEXTNS_C1 0
#define CRITICALEXTNS_CRITICALEXTNSFUTURE 1

#define C1_UEASSISTANCEINFO_R11 0
#define C1_SPARE3__24 1
#define C1_SPARE2__24 2
#define C1_SPARE1__24 3
#define CRITICALEXTNS_C1 0
#define CRITICALEXTNS_CRITICALEXTNSFUTURE 1

#define SUBFRAMEPATTERNTDD_R11_SUBFRAMECONFIG0_R11 0
#define SUBFRAMEPATTERNTDD_R11_SUBFRAMECONFIG1_5_R11 1
#define SUBFRAMEPATTERNTDD_R11_SUBFRAMECONFIG6_R11 2
#define IDC_SUBFRMPATTERN_R11_SUBFRAMEPATTERNFDD_R11 0
#define IDC_SUBFRMPATTERN_R11_SUBFRAMEPATTERNTDD_R11 1

#define TDM_ASSISTANCEINFO_R11_DRX_ASSISTANCEINFO_R11 0
#define TDM_ASSISTANCEINFO_R11_IDC_SUBFRMPATTERNLST_R11 1

#define C1_INDEVICECOEXIND_R11 0
#define C1_SPARE3__7 1
#define C1_SPARE2__7 2
#define C1_SPARE1__7 3
#define CRITICALEXTNS_C1 0
#define CRITICALEXTNS_CRITICALEXTNSFUTURE 1

#define C1_INTERESTIND_R11 0
#define C1_SPARE3__11 1
#define C1_SPARE2__11 2
#define C1_SPARE1__11 3
#define CRITICALEXTNS_C1 0
#define CRITICALEXTNS_CRITICALEXTNSFUTURE 1

#define C1_CSFBPARAMSRQSTCDMA2000 0
#define C1_MEASUREMENTREPORT 1
#define C1_RRCCONRECFGNCOMPL 2
#define C1_RRCCONREESTBCOMPL 3
#define C1_RRCCONSETUPCOMPL 4
#define C1_SECURITYMODECOMPL 5
#define C1_SECURITYMODEFAIL 6
#define C1_UECAPINFO 7
#define C1_ULHOVRPREPTFR 8
#define C1_ULINFOTFR 9
#define C1_COUNTERCHECKRSP 10
#define C1_UEINFORSP_R9__2 11
#define C1_PROXIMITYIND_R9__2 12
#define C1_RNRECFGNCOMPL_R10__2 13
#define C1_MBMSCOUNTINGRSP_R10 14
#define C1_INTERFREQRSTDMEASUREMENTIND_R10__2 15
#define C2_UEASSISTANCEINFO_R11 0
#define C2_INDEVICECOEXIND_R11 1
#define C2_MBMSINTERESTIND_R11 2
#define C2_SPARE13__3 3
#define C2_SPARE12__3 4
#define C2_SPARE11__3 5
#define C2_SPARE10__3 6
#define C2_SPARE9__3 7
#define C2_SPARE8__3 8
#define C2_SPARE7__3 9
#define C2_SPARE6__3 10
#define C2_SPARE5__3 11
#define C2_SPARE4__3 12
#define C2_SPARE3__3 13
#define C2_SPARE2__3 14
#define C2_SPARE1__3 15
#define MESSAGECLASSEXTN_C2 0
#define MESSAGECLASSEXTN_MESSAGECLASSEXTNFUTURE_R11 1
#define UL_DCCH_MSGTYP_C1 0
#define UL_DCCH_MSGTYP_MESSAGECLASSEXTN 1
#define NhumaxBandComb_r10 128
#define NhumaxBandComb_r11 256
#define NhumaxBands 64
#define NhumaxBandwidthClass_r10 16
#define NhumaxBandwidthCombSet_r10 32
#define NhumaxCDMA_BandClass 32
#define NhumaxCellBlack 16
#define NhumaxCellInfoGERAN_r9 32
#define NhumaxCellInfoUTRA_r9 16
#define NhumaxCSI_IM_r11 3
#define NhumaxCSI_Proc_r11 4
#define NhumaxCSI_RS_NZP_r11 3
#define NhumaxCSI_RS_ZP_r11 4
#define NhumaxCQI_ProcExt_r11 3
#define NhumaxFreqUTRA_TDD_r10 6
#define NhumaxCellInter 16
#define NhumaxCellIntra 16
#define NhumaxCellLstGERAN 3
#define NhumaxCellMeas 32
#define NhumaxCellReport 8
#define NhumaxDRB 11
#define NhumaxEARFCN 65535
#define NhumaxEARFCN_Plus1 65536
#define NhumaxEARFCN2 262143
#define NhumaxEPDCCH_Set_r11 2
#define NhumaxFBI 64
#define NhumaxFBI_Plus1 65
#define NhumaxFBI2 256
#define NhumaxFreq 8
#define NhumaxFreqIDC_r11 32
#define NhumaxFreqMBMS_r11 5
#define NhumaxGERAN_SI 10
#define NhumaxGNFG 16
#define NhumaxLogMeasReport_r10 520
#define NhumaxMBSFN_Allocns 8
#define NhumaxMBSFN_Area 8
#define NhumaxMBSFN_Area_1 7
#define NhumaxMeasId 32
#define NhumaxMultiBands 8
#define NhumaxObjectId 32
#define NhumaxPageRec 16
#define NhumaxPhysCellIdRange_r9 4
#define NhumaxPLMN_r11 6
#define NhumaxPNOffset 511
#define NhumaxPMCH_PerMBSFN 15
#define NhumaxRAT_Capabilities 8
#define NhumaxRE_MapQCL_r11 4
#define NhumaxReportConfigId 32
#define NhumaxRSTD_Freq_r10 3
#define NhumaxSAI_MBMS_r11 64
#define NhumaxSCell_r10 4
#define NhumaxSTAG_r11 3
#define NhumaxServCell_r10 5
#define NhumaxServiceCount 16
#define NhumaxServiceCount_1 15
#define NhumaxSessionPerPMCH 29
#define NhumaxSessionPerPMCH_1 28
#define NhumaxSIB 32
#define NhumaxSIB_1 31
#define NhumaxSI_Msg 32
#define NhumaxSimultaneousBands_r10 64
#define NhumaxSubfrmPatternIDC_r11 8
#define NhumaxUTRA_FDD_Carrier 16
#define NhumaxUTRA_TDD_Carrier 16

#define SPEEDSTATEPARS_RELEASE 0
#define SPEEDSTATEPARS_SETUP 1

#define PHYSCELLIDUTRA_FDD 0
#define PHYSCELLIDUTRA_TDD 1
#define MEMBER_PHYSCELLIDEUTRA 0
#define MEMBER_PHYSCELLIDUTRA 1
#define MEMBER_PHYSCELLIDGERAN 2
#define MEMBER_PHYSCELLIDCDMA2000 3
#define NhumaxLogMeas_r10 4060

#define C1_HANDOVERCMD_R8 0
#define C1_SPARE7__31 1
#define C1_SPARE6__31 2
#define C1_SPARE5__31 3
#define C1_SPARE4__31 4
#define C1_SPARE3__31 5
#define C1_SPARE2__31 6
#define C1_SPARE1__31 7
#define CRITICALEXTNS_C1 0
#define CRITICALEXTNS_CRITICALEXTNSFUTURE 1

#define C1_HANDOVERPREPINFO_R8 0
#define C1_SPARE7__32 1
#define C1_SPARE6__32 2
#define C1_SPARE5__32 3
#define C1_SPARE4__32 4
#define C1_SPARE3__32 5
#define C1_SPARE2__32 6
#define C1_SPARE1__32 7
#define CRITICALEXTNS_C1 0
#define CRITICALEXTNS_CRITICALEXTNSFUTURE 1

#define C1_UERADIOACCESSCAPINFO_R8 0
#define C1_SPARE7__33 1
#define C1_SPARE6__33 2
#define C1_SPARE5__33 3
#define C1_SPARE4__33 4
#define C1_SPARE3__33 5
#define C1_SPARE2__33 6
#define C1_SPARE1__33 7
#define CRITICALEXTNS_C1 0
#define CRITICALEXTNS_CRITICALEXTNSFUTURE 1
#define NhumaxReestabInfo 32

#endif
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
$SID$        ---      chebli         1. Updated DB for RRC R11 for TotaleNodeB GA3.0
*********************************************************************91*/
