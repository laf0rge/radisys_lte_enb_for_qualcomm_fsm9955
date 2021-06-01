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
#ifndef __CZTASNH__
#define __CZTASNH__ 

#define CZUCHKUNPKPTR(func, val, ptr, mBuf) {\
   S16 ret;\
   if ((ret = func(val, mBuf, ptr)) != ROK)\
      RETVALUE(ret);\
}
#define Cztid_Invalid 0xffffffff
#define CztmaxPrivIEs 65535
#define CztmaxProtExtns 65535
#define CztmaxProtIEs 65535

#define PRIVIE_ID_LOCAL 0
#define PRIVIE_ID_GLOBAL 1
#define Cztid_handoverPrep 0
#define Cztid_handoverCancel 1
#define Cztid_loadInd 2
#define Cztid_errorInd 3
#define Cztid_snStatusTfr 4
#define Cztid_uECntxtRls 5
#define Cztid_x2Setup 6
#define Cztid_reset 7
#define Cztid_eNBConfigUpd 8
#define Cztid_resourceStatusRprtngInitiation 9
#define Cztid_resourceStatusRprtng 10
#define Cztid_privateMsg 11
#define Cztid_mobilitySettingsChange 12
#define Cztid_rLFInd 13
#define Cztid_handoverReport 14
#define Cztid_cellActvn 15
#define CztmaxEARFCN 65535
#define CztmaxEARFCNPlusOne 65536
#define CztnewmaxEARFCN 262143
#define CztmaxIntfs 16
#define CztmaxCellineNB 256
#define CztmaxnoofBands 16
#define CztmaxnoofBrs 256
#define CztmaxNrOfErrs 256
#define CztmaxnoofPDCP_SN 16
#define CztmaxnoofEPLMNs 15
#define CztmaxnoofEPLMNsPlusOne 16
#define CztmaxnoofForbLACs 4096
#define CztmaxnoofForbTACs 4096
#define CztmaxnoofBPLMNs 6
#define CztmaxnoofNeighbours 512
#define CztmaxnoofPRBs 110
#define CztmaxPools 16
#define CztmaxnoofCells 16
#define CztmaxnoofMBSFN 8
#define CztmaxFailedMeasObjects 32
#define CztmaxnoofCellIDforMDT 32
#define CztmaxnoofTAforMDT 8
#define CztmaxnoofMBMSServiceAreaIdentities 256
#define CztmaxnoofMDTPLMNs 16
#define Cztid_E_RABs_Admtd_Item 0
#define Cztid_E_RABs_Admtd_Lst 1
#define Cztid_E_RAB_Item 2
#define Cztid_E_RABs_NotAdmtd_Lst 3
#define Cztid_E_RABs_ToBeSetup_Item 4
#define Cztid_Cause 5
#define Cztid_CellInform 6
#define Cztid_CellInform_Item 7
#define Cztid_New_eNB_UE_X2AP_ID 9
#define Cztid_Old_eNB_UE_X2AP_ID 10
#define Cztid_TgetCell_ID 11
#define Cztid_TgeteNBtoSrc_eNBTprntCont 12
#define Cztid_TraceActvn 13
#define Cztid_UE_CntxtInform 14
#define Cztid_UE_HistoryInform 15
#define Cztid_UE_X2AP_ID 16
#define Cztid_CriticalityDiag 17
#define Cztid_E_RABs_SubjToStatusTfr_Lst 18
#define Cztid_E_RABs_SubjToStatusTfr_Item 19
#define Cztid_SrvdCells 20
#define Cztid_GlobalENB_ID 21
#define Cztid_TimeToWait 22
#define Cztid_GUMMEI_ID 23
#define Cztid_GUGroupIDLst 24
#define Cztid_SrvdCellsToAdd 25
#define Cztid_SrvdCellsToMdfy 26
#define Cztid_SrvdCellsToDelete 27
#define Cztid_Registration_Rqst 28
#define Cztid_CellToReport 29
#define Cztid_RprtngPeriodicity 30
#define Cztid_CellToReport_Item 31
#define Cztid_CellMeasurementResult 32
#define Cztid_CellMeasurementResult_Item 33
#define Cztid_GUGroupIDToAddLst 34
#define Cztid_GUGroupIDToDeleteLst 35
#define Cztid_SRVCCOperationPossible 36
#define Cztid_Measurement_ID 37
#define Cztid_ReportCharacteristics 38
#define Cztid_ENB1_Measurement_ID 39
#define Cztid_ENB2_Measurement_ID 40
#define Cztid_Number_of_Antennaports 41
#define Cztid_CompositeAvailableCapacityGroup 42
#define Cztid_ENB1_Cell_ID 43
#define Cztid_ENB2_Cell_ID 44
#define Cztid_ENB2_Proposed_Mobility_Params 45
#define Cztid_ENB1_Mobility_Params 46
#define Cztid_ENB2_Mobility_Params_Modification_Range 47
#define Cztid_FailCellPCI 48
#define Cztid_Re_establishmentCellECGI 49
#define Cztid_FailCellCRNTI 50
#define Cztid_ShortMAC_I 51
#define Cztid_SrcCellECGI 52
#define Cztid_FailCellECGI 53
#define Cztid_HovrReportTyp 54
#define Cztid_PRACH_Config 55
#define Cztid_MBSFN_Subframe_Info 56
#define Cztid_SrvdCellsToActivate 57
#define Cztid_ActivatedCellLst 58
#define Cztid_DeactivationInd 59
#define Cztid_UE_RLF_Report_Cont 60
#define Cztid_ABSInform 61
#define Cztid_InvokeInd 62
#define Cztid_ABS_Status 63
#define Cztid_PartialSuccessIndicator 64
#define Cztid_MeasurementInitiationResult_Lst 65
#define Cztid_MeasurementInitiationResult_Item 66
#define Cztid_MeasurementFailCause_Item 67
#define Cztid_CompleteFailCauseInform_Lst 68
#define Cztid_CompleteFailCauseInform_Item 69
#define Cztid_CSG_Id 70
#define Cztid_CSGMembershipStatus 71
#define Cztid_MDTConfig 72
#define Cztid_ManagementBasedMDTallowed 74
#define Cztid_RRCConnSetupIndicator 75
#define Cztid_NeighbourTAC 76
#define Cztid_Time_UE_StayedInCell_EnhancedGranularity 77
#define Cztid_RRCConnReestabIndicator 78
#define Cztid_MBMS_Service_Area_Lst 79
#define Cztid_HO_cause 80
#define Cztid_TgetCellInUTRAN 81
#define Cztid_MobilityInform 82
#define Cztid_SrcCellCRNTI 83
#define Cztid_MultibandInfoLst 84
#define Cztid_M3Config 85
#define Cztid_M4Config 86
#define Cztid_M5Config 87
#define Cztid_MDT_Loc_Info 88
#define Cztid_ManagementBasedMDTPLMNLst 89
#define Cztid_SignallingBasedMDTPLMNLst 90
#define Cztid_ReceiveStatusOfULPDCPSDUsExtended 91
#define Cztid_ULCOUNTValueExtended 92
#define Cztid_DLCOUNTValueExtended 93
#define Cztid_eARFCNExtn 94
#define Cztid_UL_EARFCNExtn 95
#define Cztid_DL_EARFCNExtn 96
#define Cztid_AdditionalSpecialSubframe_Info 97

#define ABSINFORM_FDD 0
#define ABSINFORM_TDD 1
#define ABSINFORM_ABS_INACTIVE 2

#define USABLEABSINFORM_FDD 0
#define USABLEABSINFORM_TDD 1

#define AREASCOPEOFMDT_CELLBASED 0
#define AREASCOPEOFMDT_TABASED 1
#define AREASCOPEOFMDT_PLMNWIDE 2
#define AREASCOPEOFMDT_TAIBASED 3

#define CAUSE_RADIONW 0
#define CAUSE_TRANSPORT 1
#define CZT_CAUSE_PROTOCOL 2
#define CZT_CAUSE_MISC 3

#define EUTRA_MODE_INFO_FDD 0
#define EUTRA_MODE_INFO_TDD 1

#define ENB_ID_MACRO_ENB_ID 0
#define ENB_ID_HOME_ENB_ID 1

#define LASTVISITEDGERANCELLINFORM_UNDEFINED 0

#define LASTVISITEDCELL_ITEM_E_UTRAN_CELL 0
#define LASTVISITEDCELL_ITEM_UTRAN_CELL 1
#define LASTVISITEDCELL_ITEM_GERAN_CELL 2

#define MEASUREMENTTHRESHOLDA2_THRESHOLD_RSRP 0
#define MEASUREMENTTHRESHOLDA2_THRESHOLD_RSRQ 1

#define SUBFRAMEALLOCN_ONEFRAME 0
#define SUBFRAMEALLOCN_FOURFRAMES 1

#define X2AP_PDU_INITIATINGMSG 0
#define X2AP_PDU_SUCCESSFULOUTCOME 1
#define X2AP_PDU_UNSUCCESSFULOUTCOME 2

#endif

/********************************************************************30**

           End of file:    $SID$

*********************************************************************31*/

/********************************************************************60**

        Revision history:

*********************************************************************61*/

/********************************************************************90**

     ver       pat    init                  description
------------ -------- ---- ----------------------------------------------
$SID$        ---      chebli         1. Updated DB for X2AP R11 for TotaleNodeB GA3.0
$SID$        ---      chebli         1. Updated DB for X2AP R11.8.0 for TotaleNodeB GA5.0
*********************************************************************91*/
