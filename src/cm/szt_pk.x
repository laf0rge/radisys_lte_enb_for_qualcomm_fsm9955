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

#ifndef __SZTPKX__
#define __SZTPKX__ 


EXTERN S16 cmPkSztPrivIE_ID ARGS ((SztPrivIE_ID *param, Buffer *mBuf));
EXTERN S16 cmPkSzt_ExtnEUTRAN_CGI_ExtIEsCls ARGS ((Szt_ExtnEUTRAN_CGI_ExtIEsCls *param, U32 val, Buffer *mBuf));
EXTERN S16 cmPkSztProtExtnField_EUTRAN_CGI_ExtIEs ARGS ((SztProtExtnField_EUTRAN_CGI_ExtIEs *param, Buffer *mBuf));
EXTERN S16 cmPkSztProtExtnCont_EUTRAN_CGI_ExtIEs ARGS ((SztProtExtnCont_EUTRAN_CGI_ExtIEs *param, Buffer *mBuf));
EXTERN S16 cmPkSztEUTRAN_CGI ARGS ((SztEUTRAN_CGI *param, Buffer *mBuf));
EXTERN S16 cmPkSztCellIdLstforMDT ARGS ((SztCellIdLstforMDT *param, Buffer *mBuf));
EXTERN S16 cmPkSzt_ExtnCellBasedMDT_ExtIEsCls ARGS ((Szt_ExtnCellBasedMDT_ExtIEsCls *param, U32 val, Buffer *mBuf));
EXTERN S16 cmPkSztProtExtnField_CellBasedMDT_ExtIEs ARGS ((SztProtExtnField_CellBasedMDT_ExtIEs *param, Buffer *mBuf));
EXTERN S16 cmPkSztProtExtnCont_CellBasedMDT_ExtIEs ARGS ((SztProtExtnCont_CellBasedMDT_ExtIEs *param, Buffer *mBuf));
EXTERN S16 cmPkSztCellBasedMDT ARGS ((SztCellBasedMDT *param, Buffer *mBuf));
EXTERN S16 cmPkSztTALstforMDT ARGS ((SztTALstforMDT *param, Buffer *mBuf));
EXTERN S16 cmPkSzt_ExtnTABasedMDT_ExtIEsCls ARGS ((Szt_ExtnTABasedMDT_ExtIEsCls *param, U32 val, Buffer *mBuf));
EXTERN S16 cmPkSztProtExtnField_TABasedMDT_ExtIEs ARGS ((SztProtExtnField_TABasedMDT_ExtIEs *param, Buffer *mBuf));
EXTERN S16 cmPkSztProtExtnCont_TABasedMDT_ExtIEs ARGS ((SztProtExtnCont_TABasedMDT_ExtIEs *param, Buffer *mBuf));
EXTERN S16 cmPkSztTABasedMDT ARGS ((SztTABasedMDT *param, Buffer *mBuf));
EXTERN S16 cmPkSzt_ExtnTAI_ExtIEsCls ARGS ((Szt_ExtnTAI_ExtIEsCls *param, U32 val, Buffer *mBuf));
EXTERN S16 cmPkSztProtExtnField_TAI_ExtIEs ARGS ((SztProtExtnField_TAI_ExtIEs *param, Buffer *mBuf));
EXTERN S16 cmPkSztProtExtnCont_TAI_ExtIEs ARGS ((SztProtExtnCont_TAI_ExtIEs *param, Buffer *mBuf));
EXTERN S16 cmPkSztTAI ARGS ((SztTAI *param, Buffer *mBuf));
EXTERN S16 cmPkSztTAILstforMDT ARGS ((SztTAILstforMDT *param, Buffer *mBuf));
EXTERN S16 cmPkSzt_ExtnTAIBasedMDT_ExtIEsCls ARGS ((Szt_ExtnTAIBasedMDT_ExtIEsCls *param, U32 val, Buffer *mBuf));
EXTERN S16 cmPkSztProtExtnField_TAIBasedMDT_ExtIEs ARGS ((SztProtExtnField_TAIBasedMDT_ExtIEs *param, Buffer *mBuf));
EXTERN S16 cmPkSztProtExtnCont_TAIBasedMDT_ExtIEs ARGS ((SztProtExtnCont_TAIBasedMDT_ExtIEs *param, Buffer *mBuf));
EXTERN S16 cmPkSztTAIBasedMDT ARGS ((SztTAIBasedMDT *param, Buffer *mBuf));
EXTERN S16 cmPkSztAreaScopeOfMDT ARGS ((SztAreaScopeOfMDT *param, Buffer *mBuf));
EXTERN S16 cmPkSzt_ExtnAllocnAndRetentionPriority_ExtIEsCls ARGS ((Szt_ExtnAllocnAndRetentionPriority_ExtIEsCls *param, U32 val, Buffer *mBuf));
EXTERN S16 cmPkSztProtExtnField_AllocnAndRetentionPriority_ExtIEs ARGS ((SztProtExtnField_AllocnAndRetentionPriority_ExtIEs *param, Buffer *mBuf));
EXTERN S16 cmPkSztProtExtnCont_AllocnAndRetentionPriority_ExtIEs ARGS ((SztProtExtnCont_AllocnAndRetentionPriority_ExtIEs *param, Buffer *mBuf));
EXTERN S16 cmPkSztAllocnAndRetentionPriority ARGS ((SztAllocnAndRetentionPriority *param, Buffer *mBuf));
EXTERN S16 cmPkSzt_ExtnCOUNTvalue_ExtIEsCls ARGS ((Szt_ExtnCOUNTvalue_ExtIEsCls *param, U32 val, Buffer *mBuf));
EXTERN S16 cmPkSztProtExtnField_COUNTvalue_ExtIEs ARGS ((SztProtExtnField_COUNTvalue_ExtIEs *param, Buffer *mBuf));
EXTERN S16 cmPkSztProtExtnCont_COUNTvalue_ExtIEs ARGS ((SztProtExtnCont_COUNTvalue_ExtIEs *param, Buffer *mBuf));
EXTERN S16 cmPkSztCOUNTvalue ARGS ((SztCOUNTvalue *param, Buffer *mBuf));
EXTERN S16 cmPkSzt_ExtnCOUNTValueExtended_ExtIEsCls ARGS ((Szt_ExtnCOUNTValueExtended_ExtIEsCls *param, U32 val, Buffer *mBuf));
EXTERN S16 cmPkSztProtExtnField_COUNTValueExtended_ExtIEs ARGS ((SztProtExtnField_COUNTValueExtended_ExtIEs *param, Buffer *mBuf));
EXTERN S16 cmPkSztProtExtnCont_COUNTValueExtended_ExtIEs ARGS ((SztProtExtnCont_COUNTValueExtended_ExtIEs *param, Buffer *mBuf));
EXTERN S16 cmPkSztCOUNTValueExtended ARGS ((SztCOUNTValueExtended *param, Buffer *mBuf));
EXTERN S16 cmPkSzt_ExtnBrs_SubjToStatusTfr_ItemExtIEsCls ARGS ((Szt_ExtnBrs_SubjToStatusTfr_ItemExtIEsCls *param, U32 val, Buffer *mBuf));
EXTERN S16 cmPkSztProtExtnField_Brs_SubjToStatusTfr_ItemExtIEs ARGS ((SztProtExtnField_Brs_SubjToStatusTfr_ItemExtIEs *param, Buffer *mBuf));
EXTERN S16 cmPkSztProtExtnCont_Brs_SubjToStatusTfr_ItemExtIEs ARGS ((SztProtExtnCont_Brs_SubjToStatusTfr_ItemExtIEs *param, Buffer *mBuf));
EXTERN S16 cmPkSztBrs_SubjToStatusTfr_Item ARGS ((SztBrs_SubjToStatusTfr_Item *param, Buffer *mBuf));
EXTERN S16 cmPkSzt_ValueBrs_SubjToStatusTfr_ItemIEsCls ARGS ((Szt_ValueBrs_SubjToStatusTfr_ItemIEsCls *param, U32 val, Buffer *mBuf));
EXTERN S16 cmPkSztProtIE_Field_Brs_SubjToStatusTfr_ItemIEs ARGS ((SztProtIE_Field_Brs_SubjToStatusTfr_ItemIEs *param, Buffer *mBuf));
EXTERN S16 cmPkSztBrs_SubjToStatusTfrLst ARGS ((SztBrs_SubjToStatusTfrLst *param, Buffer *mBuf));
EXTERN S16 cmPkSztBPLMNs ARGS ((SztBPLMNs *param, Buffer *mBuf));
EXTERN S16 cmPkSzt_ExtnCellID_Cancelled_Item_ExtIEsCls ARGS ((Szt_ExtnCellID_Cancelled_Item_ExtIEsCls *param, U32 val, Buffer *mBuf));
EXTERN S16 cmPkSztProtExtnField_CellID_Cancelled_Item_ExtIEs ARGS ((SztProtExtnField_CellID_Cancelled_Item_ExtIEs *param, Buffer *mBuf));
EXTERN S16 cmPkSztProtExtnCont_CellID_Cancelled_Item_ExtIEs ARGS ((SztProtExtnCont_CellID_Cancelled_Item_ExtIEs *param, Buffer *mBuf));
EXTERN S16 cmPkSztCellID_Cancelled_Item ARGS ((SztCellID_Cancelled_Item *param, Buffer *mBuf));
EXTERN S16 cmPkSztCellID_Cancelled ARGS ((SztCellID_Cancelled *param, Buffer *mBuf));
EXTERN S16 cmPkSzt_ExtnCancelledCellinTAI_Item_ExtIEsCls ARGS ((Szt_ExtnCancelledCellinTAI_Item_ExtIEsCls *param, U32 val, Buffer *mBuf));
EXTERN S16 cmPkSztProtExtnField_CancelledCellinTAI_Item_ExtIEs ARGS ((SztProtExtnField_CancelledCellinTAI_Item_ExtIEs *param, Buffer *mBuf));
EXTERN S16 cmPkSztProtExtnCont_CancelledCellinTAI_Item_ExtIEs ARGS ((SztProtExtnCont_CancelledCellinTAI_Item_ExtIEs *param, Buffer *mBuf));
EXTERN S16 cmPkSztCancelledCellinTAI_Item ARGS ((SztCancelledCellinTAI_Item *param, Buffer *mBuf));
EXTERN S16 cmPkSztCancelledCellinTAI ARGS ((SztCancelledCellinTAI *param, Buffer *mBuf));
EXTERN S16 cmPkSzt_ExtnTAI_Cancelled_Item_ExtIEsCls ARGS ((Szt_ExtnTAI_Cancelled_Item_ExtIEsCls *param, U32 val, Buffer *mBuf));
EXTERN S16 cmPkSztProtExtnField_TAI_Cancelled_Item_ExtIEs ARGS ((SztProtExtnField_TAI_Cancelled_Item_ExtIEs *param, Buffer *mBuf));
EXTERN S16 cmPkSztProtExtnCont_TAI_Cancelled_Item_ExtIEs ARGS ((SztProtExtnCont_TAI_Cancelled_Item_ExtIEs *param, Buffer *mBuf));
EXTERN S16 cmPkSztTAI_Cancelled_Item ARGS ((SztTAI_Cancelled_Item *param, Buffer *mBuf));
EXTERN S16 cmPkSztTAI_Cancelled ARGS ((SztTAI_Cancelled *param, Buffer *mBuf));
EXTERN S16 cmPkSzt_ExtnCancelledCellinEAI_Item_ExtIEsCls ARGS ((Szt_ExtnCancelledCellinEAI_Item_ExtIEsCls *param, U32 val, Buffer *mBuf));
EXTERN S16 cmPkSztProtExtnField_CancelledCellinEAI_Item_ExtIEs ARGS ((SztProtExtnField_CancelledCellinEAI_Item_ExtIEs *param, Buffer *mBuf));
EXTERN S16 cmPkSztProtExtnCont_CancelledCellinEAI_Item_ExtIEs ARGS ((SztProtExtnCont_CancelledCellinEAI_Item_ExtIEs *param, Buffer *mBuf));
EXTERN S16 cmPkSztCancelledCellinEAI_Item ARGS ((SztCancelledCellinEAI_Item *param, Buffer *mBuf));
EXTERN S16 cmPkSztCancelledCellinEAI ARGS ((SztCancelledCellinEAI *param, Buffer *mBuf));
EXTERN S16 cmPkSzt_ExtnEmergencyAreaID_Cancelled_Item_ExtIEsCls ARGS ((Szt_ExtnEmergencyAreaID_Cancelled_Item_ExtIEsCls *param, U32 val, Buffer *mBuf));
EXTERN S16 cmPkSztProtExtnField_EmergencyAreaID_Cancelled_Item_ExtIEs ARGS ((SztProtExtnField_EmergencyAreaID_Cancelled_Item_ExtIEs *param, Buffer *mBuf));
EXTERN S16 cmPkSztProtExtnCont_EmergencyAreaID_Cancelled_Item_ExtIEs ARGS ((SztProtExtnCont_EmergencyAreaID_Cancelled_Item_ExtIEs *param, Buffer *mBuf));
EXTERN S16 cmPkSztEmergencyAreaID_Cancelled_Item ARGS ((SztEmergencyAreaID_Cancelled_Item *param, Buffer *mBuf));
EXTERN S16 cmPkSztEmergencyAreaID_Cancelled ARGS ((SztEmergencyAreaID_Cancelled *param, Buffer *mBuf));
EXTERN S16 cmPkSztBroadcastCancelledAreaLst ARGS ((SztBroadcastCancelledAreaLst *param, Buffer *mBuf));
EXTERN S16 cmPkSzt_ExtnCellID_Broadcast_Item_ExtIEsCls ARGS ((Szt_ExtnCellID_Broadcast_Item_ExtIEsCls *param, U32 val, Buffer *mBuf));
EXTERN S16 cmPkSztProtExtnField_CellID_Broadcast_Item_ExtIEs ARGS ((SztProtExtnField_CellID_Broadcast_Item_ExtIEs *param, Buffer *mBuf));
EXTERN S16 cmPkSztProtExtnCont_CellID_Broadcast_Item_ExtIEs ARGS ((SztProtExtnCont_CellID_Broadcast_Item_ExtIEs *param, Buffer *mBuf));
EXTERN S16 cmPkSztCellID_Broadcast_Item ARGS ((SztCellID_Broadcast_Item *param, Buffer *mBuf));
EXTERN S16 cmPkSztCellID_Broadcast ARGS ((SztCellID_Broadcast *param, Buffer *mBuf));
EXTERN S16 cmPkSzt_ExtnCompletedCellinTAI_Item_ExtIEsCls ARGS ((Szt_ExtnCompletedCellinTAI_Item_ExtIEsCls *param, U32 val, Buffer *mBuf));
EXTERN S16 cmPkSztProtExtnField_CompletedCellinTAI_Item_ExtIEs ARGS ((SztProtExtnField_CompletedCellinTAI_Item_ExtIEs *param, Buffer *mBuf));
EXTERN S16 cmPkSztProtExtnCont_CompletedCellinTAI_Item_ExtIEs ARGS ((SztProtExtnCont_CompletedCellinTAI_Item_ExtIEs *param, Buffer *mBuf));
EXTERN S16 cmPkSztCompletedCellinTAI_Item ARGS ((SztCompletedCellinTAI_Item *param, Buffer *mBuf));
EXTERN S16 cmPkSztCompletedCellinTAI ARGS ((SztCompletedCellinTAI *param, Buffer *mBuf));
EXTERN S16 cmPkSzt_ExtnTAI_Broadcast_Item_ExtIEsCls ARGS ((Szt_ExtnTAI_Broadcast_Item_ExtIEsCls *param, U32 val, Buffer *mBuf));
EXTERN S16 cmPkSztProtExtnField_TAI_Broadcast_Item_ExtIEs ARGS ((SztProtExtnField_TAI_Broadcast_Item_ExtIEs *param, Buffer *mBuf));
EXTERN S16 cmPkSztProtExtnCont_TAI_Broadcast_Item_ExtIEs ARGS ((SztProtExtnCont_TAI_Broadcast_Item_ExtIEs *param, Buffer *mBuf));
EXTERN S16 cmPkSztTAI_Broadcast_Item ARGS ((SztTAI_Broadcast_Item *param, Buffer *mBuf));
EXTERN S16 cmPkSztTAI_Broadcast ARGS ((SztTAI_Broadcast *param, Buffer *mBuf));
EXTERN S16 cmPkSzt_ExtnCompletedCellinEAI_Item_ExtIEsCls ARGS ((Szt_ExtnCompletedCellinEAI_Item_ExtIEsCls *param, U32 val, Buffer *mBuf));
EXTERN S16 cmPkSztProtExtnField_CompletedCellinEAI_Item_ExtIEs ARGS ((SztProtExtnField_CompletedCellinEAI_Item_ExtIEs *param, Buffer *mBuf));
EXTERN S16 cmPkSztProtExtnCont_CompletedCellinEAI_Item_ExtIEs ARGS ((SztProtExtnCont_CompletedCellinEAI_Item_ExtIEs *param, Buffer *mBuf));
EXTERN S16 cmPkSztCompletedCellinEAI_Item ARGS ((SztCompletedCellinEAI_Item *param, Buffer *mBuf));
EXTERN S16 cmPkSztCompletedCellinEAI ARGS ((SztCompletedCellinEAI *param, Buffer *mBuf));
EXTERN S16 cmPkSzt_ExtnEmergencyAreaID_Broadcast_Item_ExtIEsCls ARGS ((Szt_ExtnEmergencyAreaID_Broadcast_Item_ExtIEsCls *param, U32 val, Buffer *mBuf));
EXTERN S16 cmPkSztProtExtnField_EmergencyAreaID_Broadcast_Item_ExtIEs ARGS ((SztProtExtnField_EmergencyAreaID_Broadcast_Item_ExtIEs *param, Buffer *mBuf));
EXTERN S16 cmPkSztProtExtnCont_EmergencyAreaID_Broadcast_Item_ExtIEs ARGS ((SztProtExtnCont_EmergencyAreaID_Broadcast_Item_ExtIEs *param, Buffer *mBuf));
EXTERN S16 cmPkSztEmergencyAreaID_Broadcast_Item ARGS ((SztEmergencyAreaID_Broadcast_Item *param, Buffer *mBuf));
EXTERN S16 cmPkSztEmergencyAreaID_Broadcast ARGS ((SztEmergencyAreaID_Broadcast *param, Buffer *mBuf));
EXTERN S16 cmPkSztBroadcastCompletedAreaLst ARGS ((SztBroadcastCompletedAreaLst *param, Buffer *mBuf));
EXTERN S16 cmPkSztCause ARGS ((SztCause *param, Buffer *mBuf));
EXTERN S16 cmPkSzt_ExtnCdma2000OneXSRVCCInfo_ExtIEsCls ARGS ((Szt_ExtnCdma2000OneXSRVCCInfo_ExtIEsCls *param, U32 val, Buffer *mBuf));
EXTERN S16 cmPkSztProtExtnField_Cdma2000OneXSRVCCInfo_ExtIEs ARGS ((SztProtExtnField_Cdma2000OneXSRVCCInfo_ExtIEs *param, Buffer *mBuf));
EXTERN S16 cmPkSztProtExtnCont_Cdma2000OneXSRVCCInfo_ExtIEs ARGS ((SztProtExtnCont_Cdma2000OneXSRVCCInfo_ExtIEs *param, Buffer *mBuf));
EXTERN S16 cmPkSztCdma2000OneXSRVCCInfo ARGS ((SztCdma2000OneXSRVCCInfo *param, Buffer *mBuf));
EXTERN S16 cmPkSzt_ExtnCellTyp_ExtIEsCls ARGS ((Szt_ExtnCellTyp_ExtIEsCls *param, U32 val, Buffer *mBuf));
EXTERN S16 cmPkSztProtExtnField_CellTyp_ExtIEs ARGS ((SztProtExtnField_CellTyp_ExtIEs *param, Buffer *mBuf));
EXTERN S16 cmPkSztProtExtnCont_CellTyp_ExtIEs ARGS ((SztProtExtnCont_CellTyp_ExtIEs *param, Buffer *mBuf));
EXTERN S16 cmPkSztCellTyp ARGS ((SztCellTyp *param, Buffer *mBuf));
EXTERN S16 cmPkSzt_ExtnCGI_ExtIEsCls ARGS ((Szt_ExtnCGI_ExtIEsCls *param, U32 val, Buffer *mBuf));
EXTERN S16 cmPkSztProtExtnField_CGI_ExtIEs ARGS ((SztProtExtnField_CGI_ExtIEs *param, Buffer *mBuf));
EXTERN S16 cmPkSztProtExtnCont_CGI_ExtIEs ARGS ((SztProtExtnCont_CGI_ExtIEs *param, Buffer *mBuf));
EXTERN S16 cmPkSztCGI ARGS ((SztCGI *param, Buffer *mBuf));
EXTERN S16 cmPkSzt_ExtnCSG_IdLst_Item_ExtIEsCls ARGS ((Szt_ExtnCSG_IdLst_Item_ExtIEsCls *param, U32 val, Buffer *mBuf));
EXTERN S16 cmPkSztProtExtnField_CSG_IdLst_Item_ExtIEs ARGS ((SztProtExtnField_CSG_IdLst_Item_ExtIEs *param, Buffer *mBuf));
EXTERN S16 cmPkSztProtExtnCont_CSG_IdLst_Item_ExtIEs ARGS ((SztProtExtnCont_CSG_IdLst_Item_ExtIEs *param, Buffer *mBuf));
EXTERN S16 cmPkSztCSG_IdLst_Item ARGS ((SztCSG_IdLst_Item *param, Buffer *mBuf));
EXTERN S16 cmPkSztCSG_IdLst ARGS ((SztCSG_IdLst *param, Buffer *mBuf));
EXTERN S16 cmPkSzt_ExtnCriticalityDiag_IE_Item_ExtIEsCls ARGS ((Szt_ExtnCriticalityDiag_IE_Item_ExtIEsCls *param, U32 val, Buffer *mBuf));
EXTERN S16 cmPkSztProtExtnField_CriticalityDiag_IE_Item_ExtIEs ARGS ((SztProtExtnField_CriticalityDiag_IE_Item_ExtIEs *param, Buffer *mBuf));
EXTERN S16 cmPkSztProtExtnCont_CriticalityDiag_IE_Item_ExtIEs ARGS ((SztProtExtnCont_CriticalityDiag_IE_Item_ExtIEs *param, Buffer *mBuf));
EXTERN S16 cmPkSztCriticalityDiag_IE_Item ARGS ((SztCriticalityDiag_IE_Item *param, Buffer *mBuf));
EXTERN S16 cmPkSztCriticalityDiag_IE_Lst ARGS ((SztCriticalityDiag_IE_Lst *param, Buffer *mBuf));
EXTERN S16 cmPkSzt_ExtnCriticalityDiag_ExtIEsCls ARGS ((Szt_ExtnCriticalityDiag_ExtIEsCls *param, U32 val, Buffer *mBuf));
EXTERN S16 cmPkSztProtExtnField_CriticalityDiag_ExtIEs ARGS ((SztProtExtnField_CriticalityDiag_ExtIEs *param, Buffer *mBuf));
EXTERN S16 cmPkSztProtExtnCont_CriticalityDiag_ExtIEs ARGS ((SztProtExtnCont_CriticalityDiag_ExtIEs *param, Buffer *mBuf));
EXTERN S16 cmPkSztCriticalityDiag ARGS ((SztCriticalityDiag *param, Buffer *mBuf));
EXTERN S16 cmPkSztECGILst ARGS ((SztECGILst *param, Buffer *mBuf));
EXTERN S16 cmPkSztEmergencyAreaIDLst ARGS ((SztEmergencyAreaIDLst *param, Buffer *mBuf));
EXTERN S16 cmPkSztENB_ID ARGS ((SztENB_ID *param, Buffer *mBuf));
EXTERN S16 cmPkSzt_ExtnLAI_ExtIEsCls ARGS ((Szt_ExtnLAI_ExtIEsCls *param, U32 val, Buffer *mBuf));
EXTERN S16 cmPkSztProtExtnField_LAI_ExtIEs ARGS ((SztProtExtnField_LAI_ExtIEs *param, Buffer *mBuf));
EXTERN S16 cmPkSztProtExtnCont_LAI_ExtIEs ARGS ((SztProtExtnCont_LAI_ExtIEs *param, Buffer *mBuf));
EXTERN S16 cmPkSztLAI ARGS ((SztLAI *param, Buffer *mBuf));
EXTERN S16 cmPkSzt_ExtnGERAN_Cell_ID_ExtIEsCls ARGS ((Szt_ExtnGERAN_Cell_ID_ExtIEsCls *param, U32 val, Buffer *mBuf));
EXTERN S16 cmPkSztProtExtnField_GERAN_Cell_ID_ExtIEs ARGS ((SztProtExtnField_GERAN_Cell_ID_ExtIEs *param, Buffer *mBuf));
EXTERN S16 cmPkSztProtExtnCont_GERAN_Cell_ID_ExtIEs ARGS ((SztProtExtnCont_GERAN_Cell_ID_ExtIEs *param, Buffer *mBuf));
EXTERN S16 cmPkSztGERAN_Cell_ID ARGS ((SztGERAN_Cell_ID *param, Buffer *mBuf));
EXTERN S16 cmPkSzt_ExtnGlobalENB_ID_ExtIEsCls ARGS ((Szt_ExtnGlobalENB_ID_ExtIEsCls *param, U32 val, Buffer *mBuf));
EXTERN S16 cmPkSztProtExtnField_GlobalENB_ID_ExtIEs ARGS ((SztProtExtnField_GlobalENB_ID_ExtIEs *param, Buffer *mBuf));
EXTERN S16 cmPkSztProtExtnCont_GlobalENB_ID_ExtIEs ARGS ((SztProtExtnCont_GlobalENB_ID_ExtIEs *param, Buffer *mBuf));
EXTERN S16 cmPkSztGlobal_ENB_ID ARGS ((SztGlobal_ENB_ID *param, Buffer *mBuf));
EXTERN S16 cmPkSzt_ExtnGUMMEI_ExtIEsCls ARGS ((Szt_ExtnGUMMEI_ExtIEsCls *param, U32 val, Buffer *mBuf));
EXTERN S16 cmPkSztProtExtnField_GUMMEI_ExtIEs ARGS ((SztProtExtnField_GUMMEI_ExtIEs *param, Buffer *mBuf));
EXTERN S16 cmPkSztProtExtnCont_GUMMEI_ExtIEs ARGS ((SztProtExtnCont_GUMMEI_ExtIEs *param, Buffer *mBuf));
EXTERN S16 cmPkSztGUMMEI ARGS ((SztGUMMEI *param, Buffer *mBuf));
EXTERN S16 cmPkSztGUMMEILst ARGS ((SztGUMMEILst *param, Buffer *mBuf));
EXTERN S16 cmPkSzt_ExtnENB_StatusTfr_TprntCont_ExtIEsCls ARGS ((Szt_ExtnENB_StatusTfr_TprntCont_ExtIEsCls *param, U32 val, Buffer *mBuf));
EXTERN S16 cmPkSztProtExtnField_ENB_StatusTfr_TprntCont_ExtIEs ARGS ((SztProtExtnField_ENB_StatusTfr_TprntCont_ExtIEs *param, Buffer *mBuf));
EXTERN S16 cmPkSztProtExtnCont_ENB_StatusTfr_TprntCont_ExtIEs ARGS ((SztProtExtnCont_ENB_StatusTfr_TprntCont_ExtIEs *param, Buffer *mBuf));
EXTERN S16 cmPkSztENB_StatusTfr_TprntCont ARGS ((SztENB_StatusTfr_TprntCont *param, Buffer *mBuf));
EXTERN S16 cmPkSztENBX2TLAs ARGS ((SztENBX2TLAs *param, Buffer *mBuf));
EXTERN S16 cmPkSztEPLMNs ARGS ((SztEPLMNs *param, Buffer *mBuf));
EXTERN S16 cmPkSzt_ExtnE_RABInformLstItem_ExtIEsCls ARGS ((Szt_ExtnE_RABInformLstItem_ExtIEsCls *param, U32 val, Buffer *mBuf));
EXTERN S16 cmPkSztProtExtnField_E_RABInformLstItem_ExtIEs ARGS ((SztProtExtnField_E_RABInformLstItem_ExtIEs *param, Buffer *mBuf));
EXTERN S16 cmPkSztProtExtnCont_E_RABInformLstItem_ExtIEs ARGS ((SztProtExtnCont_E_RABInformLstItem_ExtIEs *param, Buffer *mBuf));
EXTERN S16 cmPkSztE_RABInformLstItem ARGS ((SztE_RABInformLstItem *param, Buffer *mBuf));
EXTERN S16 cmPkSzt_ValueE_RABInformLstIEsCls ARGS ((Szt_ValueE_RABInformLstIEsCls *param, U32 val, Buffer *mBuf));
EXTERN S16 cmPkSztProtIE_Field_E_RABInformLstIEs ARGS ((SztProtIE_Field_E_RABInformLstIEs *param, Buffer *mBuf));
EXTERN S16 cmPkSztE_RABInformLst ARGS ((SztE_RABInformLst *param, Buffer *mBuf));
EXTERN S16 cmPkSzt_ExtnE_RABItem_ExtIEsCls ARGS ((Szt_ExtnE_RABItem_ExtIEsCls *param, U32 val, Buffer *mBuf));
EXTERN S16 cmPkSztProtExtnField_E_RABItem_ExtIEs ARGS ((SztProtExtnField_E_RABItem_ExtIEs *param, Buffer *mBuf));
EXTERN S16 cmPkSztProtExtnCont_E_RABItem_ExtIEs ARGS ((SztProtExtnCont_E_RABItem_ExtIEs *param, Buffer *mBuf));
EXTERN S16 cmPkSztE_RABItem ARGS ((SztE_RABItem *param, Buffer *mBuf));
EXTERN S16 cmPkSzt_ValueE_RABItemIEsCls ARGS ((Szt_ValueE_RABItemIEsCls *param, U32 val, Buffer *mBuf));
EXTERN S16 cmPkSztProtIE_Field_E_RABItemIEs ARGS ((SztProtIE_Field_E_RABItemIEs *param, Buffer *mBuf));
EXTERN S16 cmPkSztE_RABLst ARGS ((SztE_RABLst *param, Buffer *mBuf));
EXTERN S16 cmPkSzt_ExtnGBR_QosInform_ExtIEsCls ARGS ((Szt_ExtnGBR_QosInform_ExtIEsCls *param, U32 val, Buffer *mBuf));
EXTERN S16 cmPkSztProtExtnField_GBR_QosInform_ExtIEs ARGS ((SztProtExtnField_GBR_QosInform_ExtIEs *param, Buffer *mBuf));
EXTERN S16 cmPkSztProtExtnCont_GBR_QosInform_ExtIEs ARGS ((SztProtExtnCont_GBR_QosInform_ExtIEs *param, Buffer *mBuf));
EXTERN S16 cmPkSztGBR_QosInform ARGS ((SztGBR_QosInform *param, Buffer *mBuf));
EXTERN S16 cmPkSzt_ExtnE_RABQoSParams_ExtIEsCls ARGS ((Szt_ExtnE_RABQoSParams_ExtIEsCls *param, U32 val, Buffer *mBuf));
EXTERN S16 cmPkSztProtExtnField_E_RABQoSParams_ExtIEs ARGS ((SztProtExtnField_E_RABQoSParams_ExtIEs *param, Buffer *mBuf));
EXTERN S16 cmPkSztProtExtnCont_E_RABQoSParams_ExtIEs ARGS ((SztProtExtnCont_E_RABQoSParams_ExtIEs *param, Buffer *mBuf));
EXTERN S16 cmPkSztE_RABLvlQoSParams ARGS ((SztE_RABLvlQoSParams *param, Buffer *mBuf));
EXTERN S16 cmPkSztForbiddenTACs ARGS ((SztForbiddenTACs *param, Buffer *mBuf));
EXTERN S16 cmPkSzt_ExtnForbiddenTAs_Item_ExtIEsCls ARGS ((Szt_ExtnForbiddenTAs_Item_ExtIEsCls *param, U32 val, Buffer *mBuf));
EXTERN S16 cmPkSztProtExtnField_ForbiddenTAs_Item_ExtIEs ARGS ((SztProtExtnField_ForbiddenTAs_Item_ExtIEs *param, Buffer *mBuf));
EXTERN S16 cmPkSztProtExtnCont_ForbiddenTAs_Item_ExtIEs ARGS ((SztProtExtnCont_ForbiddenTAs_Item_ExtIEs *param, Buffer *mBuf));
EXTERN S16 cmPkSztForbiddenTAs_Item ARGS ((SztForbiddenTAs_Item *param, Buffer *mBuf));
EXTERN S16 cmPkSztForbiddenTAs ARGS ((SztForbiddenTAs *param, Buffer *mBuf));
EXTERN S16 cmPkSztForbiddenLACs ARGS ((SztForbiddenLACs *param, Buffer *mBuf));
EXTERN S16 cmPkSzt_ExtnForbiddenLAs_Item_ExtIEsCls ARGS ((Szt_ExtnForbiddenLAs_Item_ExtIEsCls *param, U32 val, Buffer *mBuf));
EXTERN S16 cmPkSztProtExtnField_ForbiddenLAs_Item_ExtIEs ARGS ((SztProtExtnField_ForbiddenLAs_Item_ExtIEs *param, Buffer *mBuf));
EXTERN S16 cmPkSztProtExtnCont_ForbiddenLAs_Item_ExtIEs ARGS ((SztProtExtnCont_ForbiddenLAs_Item_ExtIEs *param, Buffer *mBuf));
EXTERN S16 cmPkSztForbiddenLAs_Item ARGS ((SztForbiddenLAs_Item *param, Buffer *mBuf));
EXTERN S16 cmPkSztForbiddenLAs ARGS ((SztForbiddenLAs *param, Buffer *mBuf));
EXTERN S16 cmPkSzt_ExtnHovrRestrnLst_ExtIEsCls ARGS ((Szt_ExtnHovrRestrnLst_ExtIEsCls *param, U32 val, Buffer *mBuf));
EXTERN S16 cmPkSztProtExtnField_HovrRestrnLst_ExtIEs ARGS ((SztProtExtnField_HovrRestrnLst_ExtIEs *param, Buffer *mBuf));
EXTERN S16 cmPkSztProtExtnCont_HovrRestrnLst_ExtIEs ARGS ((SztProtExtnCont_HovrRestrnLst_ExtIEs *param, Buffer *mBuf));
EXTERN S16 cmPkSztHovrRestrnLst ARGS ((SztHovrRestrnLst *param, Buffer *mBuf));
EXTERN S16 cmPkSztMeasurementThresholdA2 ARGS ((SztMeasurementThresholdA2 *param, Buffer *mBuf));
EXTERN S16 cmPkSzt_ExtnM1ThresholdEventA2_ExtIEsCls ARGS ((Szt_ExtnM1ThresholdEventA2_ExtIEsCls *param, U32 val, Buffer *mBuf));
EXTERN S16 cmPkSztProtExtnField_M1ThresholdEventA2_ExtIEs ARGS ((SztProtExtnField_M1ThresholdEventA2_ExtIEs *param, Buffer *mBuf));
EXTERN S16 cmPkSztProtExtnCont_M1ThresholdEventA2_ExtIEs ARGS ((SztProtExtnCont_M1ThresholdEventA2_ExtIEs *param, Buffer *mBuf));
EXTERN S16 cmPkSztM1ThresholdEventA2 ARGS ((SztM1ThresholdEventA2 *param, Buffer *mBuf));
EXTERN S16 cmPkSzt_ExtnM1PeriodicRprtng_ExtIEsCls ARGS ((Szt_ExtnM1PeriodicRprtng_ExtIEsCls *param, U32 val, Buffer *mBuf));
EXTERN S16 cmPkSztProtExtnField_M1PeriodicRprtng_ExtIEs ARGS ((SztProtExtnField_M1PeriodicRprtng_ExtIEs *param, Buffer *mBuf));
EXTERN S16 cmPkSztProtExtnCont_M1PeriodicRprtng_ExtIEs ARGS ((SztProtExtnCont_M1PeriodicRprtng_ExtIEs *param, Buffer *mBuf));
EXTERN S16 cmPkSztM1PeriodicRprtng ARGS ((SztM1PeriodicRprtng *param, Buffer *mBuf));
EXTERN S16 cmPkSzt_ExtnM3Config_ExtIEsCls ARGS ((Szt_ExtnM3Config_ExtIEsCls *param, U32 val, Buffer *mBuf));
EXTERN S16 cmPkSztProtExtnField_M3Config_ExtIEs ARGS ((SztProtExtnField_M3Config_ExtIEs *param, Buffer *mBuf));
EXTERN S16 cmPkSztProtExtnCont_M3Config_ExtIEs ARGS ((SztProtExtnCont_M3Config_ExtIEs *param, Buffer *mBuf));
EXTERN S16 cmPkSztM3Config ARGS ((SztM3Config *param, Buffer *mBuf));
EXTERN S16 cmPkSzt_ExtnM4Config_ExtIEsCls ARGS ((Szt_ExtnM4Config_ExtIEsCls *param, U32 val, Buffer *mBuf));
EXTERN S16 cmPkSztProtExtnField_M4Config_ExtIEs ARGS ((SztProtExtnField_M4Config_ExtIEs *param, Buffer *mBuf));
EXTERN S16 cmPkSztProtExtnCont_M4Config_ExtIEs ARGS ((SztProtExtnCont_M4Config_ExtIEs *param, Buffer *mBuf));
EXTERN S16 cmPkSztM4Config ARGS ((SztM4Config *param, Buffer *mBuf));
EXTERN S16 cmPkSzt_ExtnM5Config_ExtIEsCls ARGS ((Szt_ExtnM5Config_ExtIEsCls *param, U32 val, Buffer *mBuf));
EXTERN S16 cmPkSztProtExtnField_M5Config_ExtIEs ARGS ((SztProtExtnField_M5Config_ExtIEs *param, Buffer *mBuf));
EXTERN S16 cmPkSztProtExtnCont_M5Config_ExtIEs ARGS ((SztProtExtnCont_M5Config_ExtIEs *param, Buffer *mBuf));
EXTERN S16 cmPkSztM5Config ARGS ((SztM5Config *param, Buffer *mBuf));
EXTERN S16 cmPkSzt_ExtnImmediateMDT_ExtIEsCls ARGS ((Szt_ExtnImmediateMDT_ExtIEsCls *param, U32 val, Buffer *mBuf));
EXTERN S16 cmPkSztProtExtnField_ImmediateMDT_ExtIEs ARGS ((SztProtExtnField_ImmediateMDT_ExtIEs *param, Buffer *mBuf));
EXTERN S16 cmPkSztProtExtnCont_ImmediateMDT_ExtIEs ARGS ((SztProtExtnCont_ImmediateMDT_ExtIEs *param, Buffer *mBuf));
EXTERN S16 cmPkSztImmediateMDT ARGS ((SztImmediateMDT *param, Buffer *mBuf));
EXTERN S16 cmPkSzt_ExtnLastVisitedEUTRANCellInform_ExtIEsCls ARGS ((Szt_ExtnLastVisitedEUTRANCellInform_ExtIEsCls *param, U32 val, Buffer *mBuf));
EXTERN S16 cmPkSztProtExtnField_LastVisitedEUTRANCellInform_ExtIEs ARGS ((SztProtExtnField_LastVisitedEUTRANCellInform_ExtIEs *param, Buffer *mBuf));
EXTERN S16 cmPkSztProtExtnCont_LastVisitedEUTRANCellInform_ExtIEs ARGS ((SztProtExtnCont_LastVisitedEUTRANCellInform_ExtIEs *param, Buffer *mBuf));
EXTERN S16 cmPkSztLastVisitedEUTRANCellInform ARGS ((SztLastVisitedEUTRANCellInform *param, Buffer *mBuf));
EXTERN S16 cmPkSztLastVisitedGERANCellInform ARGS ((SztLastVisitedGERANCellInform *param, Buffer *mBuf));
EXTERN S16 cmPkSztLastVisitedCell_Item ARGS ((SztLastVisitedCell_Item *param, Buffer *mBuf));
EXTERN S16 cmPkSzt_ExtnLoggedMDT_ExtIEsCls ARGS ((Szt_ExtnLoggedMDT_ExtIEsCls *param, U32 val, Buffer *mBuf));
EXTERN S16 cmPkSztProtExtnField_LoggedMDT_ExtIEs ARGS ((SztProtExtnField_LoggedMDT_ExtIEs *param, Buffer *mBuf));
EXTERN S16 cmPkSztProtExtnCont_LoggedMDT_ExtIEs ARGS ((SztProtExtnCont_LoggedMDT_ExtIEs *param, Buffer *mBuf));
EXTERN S16 cmPkSztLoggedMDT ARGS ((SztLoggedMDT *param, Buffer *mBuf));
EXTERN S16 cmPkSztMDTMode ARGS ((SztMDTMode *param, Buffer *mBuf));
EXTERN S16 cmPkSztMDTPLMNLst ARGS ((SztMDTPLMNLst *param, Buffer *mBuf));
EXTERN S16 cmPkSzt_ExtnMDT_Config_ExtIEsCls ARGS ((Szt_ExtnMDT_Config_ExtIEsCls *param, U32 val, Buffer *mBuf));
EXTERN S16 cmPkSztProtExtnField_MDT_Config_ExtIEs ARGS ((SztProtExtnField_MDT_Config_ExtIEs *param, Buffer *mBuf));
EXTERN S16 cmPkSztProtExtnCont_MDT_Config_ExtIEs ARGS ((SztProtExtnCont_MDT_Config_ExtIEs *param, Buffer *mBuf));
EXTERN S16 cmPkSztMDT_Config ARGS ((SztMDT_Config *param, Buffer *mBuf));
EXTERN S16 cmPkSztOverloadResp ARGS ((SztOverloadResp *param, Buffer *mBuf));
EXTERN S16 cmPkSzt_ExtnRqstTyp_ExtIEsCls ARGS ((Szt_ExtnRqstTyp_ExtIEsCls *param, U32 val, Buffer *mBuf));
EXTERN S16 cmPkSztProtExtnField_RqstTyp_ExtIEs ARGS ((SztProtExtnField_RqstTyp_ExtIEs *param, Buffer *mBuf));
EXTERN S16 cmPkSztProtExtnCont_RqstTyp_ExtIEs ARGS ((SztProtExtnCont_RqstTyp_ExtIEs *param, Buffer *mBuf));
EXTERN S16 cmPkSztRqstTyp ARGS ((SztRqstTyp *param, Buffer *mBuf));
EXTERN S16 cmPkSzt_ExtnTgetRNC_ID_ExtIEsCls ARGS ((Szt_ExtnTgetRNC_ID_ExtIEsCls *param, U32 val, Buffer *mBuf));
EXTERN S16 cmPkSztProtExtnField_TgetRNC_ID_ExtIEs ARGS ((SztProtExtnField_TgetRNC_ID_ExtIEs *param, Buffer *mBuf));
EXTERN S16 cmPkSztProtExtnCont_TgetRNC_ID_ExtIEs ARGS ((SztProtExtnCont_TgetRNC_ID_ExtIEs *param, Buffer *mBuf));
EXTERN S16 cmPkSztTgetRNC_ID ARGS ((SztTgetRNC_ID *param, Buffer *mBuf));
EXTERN S16 cmPkSztRIMRoutingAddr ARGS ((SztRIMRoutingAddr *param, Buffer *mBuf));
EXTERN S16 cmPkSzt_ExtnRIMTfr_ExtIEsCls ARGS ((Szt_ExtnRIMTfr_ExtIEsCls *param, U32 val, Buffer *mBuf));
EXTERN S16 cmPkSztProtExtnField_RIMTfr_ExtIEs ARGS ((SztProtExtnField_RIMTfr_ExtIEs *param, Buffer *mBuf));
EXTERN S16 cmPkSztProtExtnCont_RIMTfr_ExtIEs ARGS ((SztProtExtnCont_RIMTfr_ExtIEs *param, Buffer *mBuf));
EXTERN S16 cmPkSztRIMTfr ARGS ((SztRIMTfr *param, Buffer *mBuf));
EXTERN S16 cmPkSzt_ExtnSecurCntxt_ExtIEsCls ARGS ((Szt_ExtnSecurCntxt_ExtIEsCls *param, U32 val, Buffer *mBuf));
EXTERN S16 cmPkSztProtExtnField_SecurCntxt_ExtIEs ARGS ((SztProtExtnField_SecurCntxt_ExtIEs *param, Buffer *mBuf));
EXTERN S16 cmPkSztProtExtnCont_SecurCntxt_ExtIEs ARGS ((SztProtExtnCont_SecurCntxt_ExtIEs *param, Buffer *mBuf));
EXTERN S16 cmPkSztSecurCntxt ARGS ((SztSecurCntxt *param, Buffer *mBuf));
EXTERN S16 cmPkSztENBX2GTPTLAs ARGS ((SztENBX2GTPTLAs *param, Buffer *mBuf));
EXTERN S16 cmPkSzt_ExtnENBX2ExtTLA_ExtIEsCls ARGS ((Szt_ExtnENBX2ExtTLA_ExtIEsCls *param, U32 val, Buffer *mBuf));
EXTERN S16 cmPkSztProtExtnField_ENBX2ExtTLA_ExtIEs ARGS ((SztProtExtnField_ENBX2ExtTLA_ExtIEs *param, Buffer *mBuf));
EXTERN S16 cmPkSztProtExtnCont_ENBX2ExtTLA_ExtIEs ARGS ((SztProtExtnCont_ENBX2ExtTLA_ExtIEs *param, Buffer *mBuf));
EXTERN S16 cmPkSztENBX2ExtTLA ARGS ((SztENBX2ExtTLA *param, Buffer *mBuf));
EXTERN S16 cmPkSztENBX2ExtTLAs ARGS ((SztENBX2ExtTLAs *param, Buffer *mBuf));
EXTERN S16 cmPkSzt_ExtnX2TNLConfigInfo_ExtIEsCls ARGS ((Szt_ExtnX2TNLConfigInfo_ExtIEsCls *param, U32 val, Buffer *mBuf));
EXTERN S16 cmPkSztProtExtnField_X2TNLConfigInfo_ExtIEs ARGS ((SztProtExtnField_X2TNLConfigInfo_ExtIEs *param, Buffer *mBuf));
EXTERN S16 cmPkSztProtExtnCont_X2TNLConfigInfo_ExtIEs ARGS ((SztProtExtnCont_X2TNLConfigInfo_ExtIEs *param, Buffer *mBuf));
EXTERN S16 cmPkSztX2TNLConfigInfo ARGS ((SztX2TNLConfigInfo *param, Buffer *mBuf));
EXTERN S16 cmPkSzt_ExtnTimeSynchronizationInfo_ExtIEsCls ARGS ((Szt_ExtnTimeSynchronizationInfo_ExtIEsCls *param, U32 val, Buffer *mBuf));
EXTERN S16 cmPkSztProtExtnField_TimeSynchronizationInfo_ExtIEs ARGS ((SztProtExtnField_TimeSynchronizationInfo_ExtIEs *param, Buffer *mBuf));
EXTERN S16 cmPkSztProtExtnCont_TimeSynchronizationInfo_ExtIEs ARGS ((SztProtExtnCont_TimeSynchronizationInfo_ExtIEs *param, Buffer *mBuf));
EXTERN S16 cmPkSztTimeSynchronizationInfo ARGS ((SztTimeSynchronizationInfo *param, Buffer *mBuf));
EXTERN S16 cmPkSzt_ExtnSONInformReply_ExtIEsCls ARGS ((Szt_ExtnSONInformReply_ExtIEsCls *param, U32 val, Buffer *mBuf));
EXTERN S16 cmPkSztProtExtnField_SONInformReply_ExtIEs ARGS ((SztProtExtnField_SONInformReply_ExtIEs *param, Buffer *mBuf));
EXTERN S16 cmPkSztProtExtnCont_SONInformReply_ExtIEs ARGS ((SztProtExtnCont_SONInformReply_ExtIEs *param, Buffer *mBuf));
EXTERN S16 cmPkSztSONInformReply ARGS ((SztSONInformReply *param, Buffer *mBuf));
EXTERN S16 cmPkSztSONInform ARGS ((SztSONInform *param, Buffer *mBuf));
EXTERN S16 cmPkSzt_ExtnTgeteNB_ID_ExtIEsCls ARGS ((Szt_ExtnTgeteNB_ID_ExtIEsCls *param, U32 val, Buffer *mBuf));
EXTERN S16 cmPkSztProtExtnField_TgeteNB_ID_ExtIEs ARGS ((SztProtExtnField_TgeteNB_ID_ExtIEs *param, Buffer *mBuf));
EXTERN S16 cmPkSztProtExtnCont_TgeteNB_ID_ExtIEs ARGS ((SztProtExtnCont_TgeteNB_ID_ExtIEs *param, Buffer *mBuf));
EXTERN S16 cmPkSztTgeteNB_ID ARGS ((SztTgeteNB_ID *param, Buffer *mBuf));
EXTERN S16 cmPkSzt_ExtnSrceNB_ID_ExtIEsCls ARGS ((Szt_ExtnSrceNB_ID_ExtIEsCls *param, U32 val, Buffer *mBuf));
EXTERN S16 cmPkSztProtExtnField_SrceNB_ID_ExtIEs ARGS ((SztProtExtnField_SrceNB_ID_ExtIEs *param, Buffer *mBuf));
EXTERN S16 cmPkSztProtExtnCont_SrceNB_ID_ExtIEs ARGS ((SztProtExtnCont_SrceNB_ID_ExtIEs *param, Buffer *mBuf));
EXTERN S16 cmPkSztSrceNB_ID ARGS ((SztSrceNB_ID *param, Buffer *mBuf));
EXTERN S16 cmPkSzt_ExtnSONConfigTfr_ExtIEsCls ARGS ((Szt_ExtnSONConfigTfr_ExtIEsCls *param, U32 val, Buffer *mBuf));
EXTERN S16 cmPkSztProtExtnField_SONConfigTfr_ExtIEs ARGS ((SztProtExtnField_SONConfigTfr_ExtIEs *param, Buffer *mBuf));
EXTERN S16 cmPkSztProtExtnCont_SONConfigTfr_ExtIEs ARGS ((SztProtExtnCont_SONConfigTfr_ExtIEs *param, Buffer *mBuf));
EXTERN S16 cmPkSztSONConfigTfr ARGS ((SztSONConfigTfr *param, Buffer *mBuf));
EXTERN S16 cmPkSztUE_HistoryInform ARGS ((SztUE_HistoryInform *param, Buffer *mBuf));
EXTERN S16 cmPkSzt_ExtnSrceNB_ToTgeteNB_TprntCont_ExtIEsCls ARGS ((Szt_ExtnSrceNB_ToTgeteNB_TprntCont_ExtIEsCls *param, U32 val, Buffer *mBuf));
EXTERN S16 cmPkSztProtExtnField_SrceNB_ToTgeteNB_TprntCont_ExtIEs ARGS ((SztProtExtnField_SrceNB_ToTgeteNB_TprntCont_ExtIEs *param, Buffer *mBuf));
EXTERN S16 cmPkSztProtExtnCont_SrceNB_ToTgeteNB_TprntCont_ExtIEs ARGS ((SztProtExtnCont_SrceNB_ToTgeteNB_TprntCont_ExtIEs *param, Buffer *mBuf));
EXTERN S16 cmPkSztSrceNB_ToTgeteNB_TprntCont ARGS ((SztSrceNB_ToTgeteNB_TprntCont *param, Buffer *mBuf));
EXTERN S16 cmPkSztSrvdPLMNs ARGS ((SztSrvdPLMNs *param, Buffer *mBuf));
EXTERN S16 cmPkSztSrvdGroupIDs ARGS ((SztSrvdGroupIDs *param, Buffer *mBuf));
EXTERN S16 cmPkSztSrvdMMECs ARGS ((SztSrvdMMECs *param, Buffer *mBuf));
EXTERN S16 cmPkSzt_ExtnSrvdGUMMEIsItem_ExtIEsCls ARGS ((Szt_ExtnSrvdGUMMEIsItem_ExtIEsCls *param, U32 val, Buffer *mBuf));
EXTERN S16 cmPkSztProtExtnField_SrvdGUMMEIsItem_ExtIEs ARGS ((SztProtExtnField_SrvdGUMMEIsItem_ExtIEs *param, Buffer *mBuf));
EXTERN S16 cmPkSztProtExtnCont_SrvdGUMMEIsItem_ExtIEs ARGS ((SztProtExtnCont_SrvdGUMMEIsItem_ExtIEs *param, Buffer *mBuf));
EXTERN S16 cmPkSztSrvdGUMMEIsItem ARGS ((SztSrvdGUMMEIsItem *param, Buffer *mBuf));
EXTERN S16 cmPkSztSrvdGUMMEIs ARGS ((SztSrvdGUMMEIs *param, Buffer *mBuf));
EXTERN S16 cmPkSzt_ExtnSuppTAs_Item_ExtIEsCls ARGS ((Szt_ExtnSuppTAs_Item_ExtIEsCls *param, U32 val, Buffer *mBuf));
EXTERN S16 cmPkSztProtExtnField_SuppTAs_Item_ExtIEs ARGS ((SztProtExtnField_SuppTAs_Item_ExtIEs *param, Buffer *mBuf));
EXTERN S16 cmPkSztProtExtnCont_SuppTAs_Item_ExtIEs ARGS ((SztProtExtnCont_SuppTAs_Item_ExtIEs *param, Buffer *mBuf));
EXTERN S16 cmPkSztSuppTAs_Item ARGS ((SztSuppTAs_Item *param, Buffer *mBuf));
EXTERN S16 cmPkSztSuppTAs ARGS ((SztSuppTAs *param, Buffer *mBuf));
EXTERN S16 cmPkSzt_ExtnS_TMSI_ExtIEsCls ARGS ((Szt_ExtnS_TMSI_ExtIEsCls *param, U32 val, Buffer *mBuf));
EXTERN S16 cmPkSztProtExtnField_S_TMSI_ExtIEs ARGS ((SztProtExtnField_S_TMSI_ExtIEs *param, Buffer *mBuf));
EXTERN S16 cmPkSztProtExtnCont_S_TMSI_ExtIEs ARGS ((SztProtExtnCont_S_TMSI_ExtIEs *param, Buffer *mBuf));
EXTERN S16 cmPkSztS_TMSI ARGS ((SztS_TMSI *param, Buffer *mBuf));
EXTERN S16 cmPkSztTAILstforWarning ARGS ((SztTAILstforWarning *param, Buffer *mBuf));
EXTERN S16 cmPkSztTgetID ARGS ((SztTgetID *param, Buffer *mBuf));
EXTERN S16 cmPkSzt_ExtnTgeteNB_ToSrceNB_TprntCont_ExtIEsCls ARGS ((Szt_ExtnTgeteNB_ToSrceNB_TprntCont_ExtIEsCls *param, U32 val, Buffer *mBuf));
EXTERN S16 cmPkSztProtExtnField_TgeteNB_ToSrceNB_TprntCont_ExtIEs ARGS ((SztProtExtnField_TgeteNB_ToSrceNB_TprntCont_ExtIEs *param, Buffer *mBuf));
EXTERN S16 cmPkSztProtExtnCont_TgeteNB_ToSrceNB_TprntCont_ExtIEs ARGS ((SztProtExtnCont_TgeteNB_ToSrceNB_TprntCont_ExtIEs *param, Buffer *mBuf));
EXTERN S16 cmPkSztTgeteNB_ToSrceNB_TprntCont ARGS ((SztTgeteNB_ToSrceNB_TprntCont *param, Buffer *mBuf));
EXTERN S16 cmPkSzt_ExtnTraceActvn_ExtIEsCls ARGS ((Szt_ExtnTraceActvn_ExtIEsCls *param, U32 val, Buffer *mBuf));
EXTERN S16 cmPkSztProtExtnField_TraceActvn_ExtIEs ARGS ((SztProtExtnField_TraceActvn_ExtIEs *param, Buffer *mBuf));
EXTERN S16 cmPkSztProtExtnCont_TraceActvn_ExtIEs ARGS ((SztProtExtnCont_TraceActvn_ExtIEs *param, Buffer *mBuf));
EXTERN S16 cmPkSztTraceActvn ARGS ((SztTraceActvn *param, Buffer *mBuf));
EXTERN S16 cmPkSzt_ExtnTunnel_Inform_ExtIEsCls ARGS ((Szt_ExtnTunnel_Inform_ExtIEsCls *param, U32 val, Buffer *mBuf));
EXTERN S16 cmPkSztProtExtnField_Tunnel_Inform_ExtIEs ARGS ((SztProtExtnField_Tunnel_Inform_ExtIEs *param, Buffer *mBuf));
EXTERN S16 cmPkSztProtExtnCont_Tunnel_Inform_ExtIEs ARGS ((SztProtExtnCont_Tunnel_Inform_ExtIEs *param, Buffer *mBuf));
EXTERN S16 cmPkSztTunnelInform ARGS ((SztTunnelInform *param, Buffer *mBuf));
EXTERN S16 cmPkSzt_ExtnUEAgg_MaxBitrates_ExtIEsCls ARGS ((Szt_ExtnUEAgg_MaxBitrates_ExtIEsCls *param, U32 val, Buffer *mBuf));
EXTERN S16 cmPkSztProtExtnField_UEAgg_MaxBitrates_ExtIEs ARGS ((SztProtExtnField_UEAgg_MaxBitrates_ExtIEs *param, Buffer *mBuf));
EXTERN S16 cmPkSztProtExtnCont_UEAgg_MaxBitrates_ExtIEs ARGS ((SztProtExtnCont_UEAgg_MaxBitrates_ExtIEs *param, Buffer *mBuf));
EXTERN S16 cmPkSztUEAggMaxBitrate ARGS ((SztUEAggMaxBitrate *param, Buffer *mBuf));
EXTERN S16 cmPkSzt_ExtnUE_S1AP_ID_pair_ExtIEsCls ARGS ((Szt_ExtnUE_S1AP_ID_pair_ExtIEsCls *param, U32 val, Buffer *mBuf));
EXTERN S16 cmPkSztProtExtnField_UE_S1AP_ID_pair_ExtIEs ARGS ((SztProtExtnField_UE_S1AP_ID_pair_ExtIEs *param, Buffer *mBuf));
EXTERN S16 cmPkSztProtExtnCont_UE_S1AP_ID_pair_ExtIEs ARGS ((SztProtExtnCont_UE_S1AP_ID_pair_ExtIEs *param, Buffer *mBuf));
EXTERN S16 cmPkSztUE_S1AP_ID_pair ARGS ((SztUE_S1AP_ID_pair *param, Buffer *mBuf));
EXTERN S16 cmPkSztUE_S1AP_IDs ARGS ((SztUE_S1AP_IDs *param, Buffer *mBuf));
EXTERN S16 cmPkSzt_ExtnUE_assocLogS1_ConItemExtIEsCls ARGS ((Szt_ExtnUE_assocLogS1_ConItemExtIEsCls *param, U32 val, Buffer *mBuf));
EXTERN S16 cmPkSztProtExtnField_UE_assocLogS1_ConItemExtIEs ARGS ((SztProtExtnField_UE_assocLogS1_ConItemExtIEs *param, Buffer *mBuf));
EXTERN S16 cmPkSztProtExtnCont_UE_assocLogS1_ConItemExtIEs ARGS ((SztProtExtnCont_UE_assocLogS1_ConItemExtIEs *param, Buffer *mBuf));
EXTERN S16 cmPkSztUE_assocLogS1_ConItem ARGS ((SztUE_assocLogS1_ConItem *param, Buffer *mBuf));
EXTERN S16 cmPkSztUEPagID ARGS ((SztUEPagID *param, Buffer *mBuf));
EXTERN S16 cmPkSzt_ExtnUESecurCapabilities_ExtIEsCls ARGS ((Szt_ExtnUESecurCapabilities_ExtIEsCls *param, U32 val, Buffer *mBuf));
EXTERN S16 cmPkSztProtExtnField_UESecurCapabilities_ExtIEs ARGS ((SztProtExtnField_UESecurCapabilities_ExtIEs *param, Buffer *mBuf));
EXTERN S16 cmPkSztProtExtnCont_UESecurCapabilities_ExtIEs ARGS ((SztProtExtnCont_UESecurCapabilities_ExtIEs *param, Buffer *mBuf));
EXTERN S16 cmPkSztUESecurCapabilities ARGS ((SztUESecurCapabilities *param, Buffer *mBuf));
EXTERN S16 cmPkSztWarningAreaLst ARGS ((SztWarningAreaLst *param, Buffer *mBuf));
EXTERN S16 cmPkSzt_ValueHovrReqdIEsCls ARGS ((Szt_ValueHovrReqdIEsCls *param, U32 val, Buffer *mBuf));
EXTERN S16 cmPkSztProtIE_Field_HovrReqdIEs ARGS ((SztProtIE_Field_HovrReqdIEs *param, Buffer *mBuf));
EXTERN S16 cmPkSztProtIE_Cont_HovrReqdIEs ARGS ((SztProtIE_Cont_HovrReqdIEs *param, Buffer *mBuf));
EXTERN S16 cmPkSztHovrReqd ARGS ((SztHovrReqd *param, Buffer *mBuf));
EXTERN S16 cmPkSzt_ExtnE_RABDataFwdingItem_ExtIEsCls ARGS ((Szt_ExtnE_RABDataFwdingItem_ExtIEsCls *param, U32 val, Buffer *mBuf));
EXTERN S16 cmPkSztProtExtnField_E_RABDataFwdingItem_ExtIEs ARGS ((SztProtExtnField_E_RABDataFwdingItem_ExtIEs *param, Buffer *mBuf));
EXTERN S16 cmPkSztProtExtnCont_E_RABDataFwdingItem_ExtIEs ARGS ((SztProtExtnCont_E_RABDataFwdingItem_ExtIEs *param, Buffer *mBuf));
EXTERN S16 cmPkSztE_RABDataFwdingItem ARGS ((SztE_RABDataFwdingItem *param, Buffer *mBuf));
EXTERN S16 cmPkSzt_ValueE_RABDataFwdingItemIEsCls ARGS ((Szt_ValueE_RABDataFwdingItemIEsCls *param, U32 val, Buffer *mBuf));
EXTERN S16 cmPkSztProtIE_Field_E_RABDataFwdingItemIEs ARGS ((SztProtIE_Field_E_RABDataFwdingItemIEs *param, Buffer *mBuf));
EXTERN S16 cmPkSztProtIE_ContLst_E_RABDataFwdingItemIEs ARGS ((SztProtIE_ContLst_E_RABDataFwdingItemIEs *param, Buffer *mBuf));
EXTERN S16 cmPkSzt_ValueHovrCmmdIEsCls ARGS ((Szt_ValueHovrCmmdIEsCls *param, U32 val, Buffer *mBuf));
EXTERN S16 cmPkSztProtIE_Field_HovrCmmdIEs ARGS ((SztProtIE_Field_HovrCmmdIEs *param, Buffer *mBuf));
EXTERN S16 cmPkSztProtIE_Cont_HovrCmmdIEs ARGS ((SztProtIE_Cont_HovrCmmdIEs *param, Buffer *mBuf));
EXTERN S16 cmPkSztHovrCmmd ARGS ((SztHovrCmmd *param, Buffer *mBuf));
EXTERN S16 cmPkSzt_ValueHovrPrepFailIEsCls ARGS ((Szt_ValueHovrPrepFailIEsCls *param, U32 val, Buffer *mBuf));
EXTERN S16 cmPkSztProtIE_Field_HovrPrepFailIEs ARGS ((SztProtIE_Field_HovrPrepFailIEs *param, Buffer *mBuf));
EXTERN S16 cmPkSztProtIE_Cont_HovrPrepFailIEs ARGS ((SztProtIE_Cont_HovrPrepFailIEs *param, Buffer *mBuf));
EXTERN S16 cmPkSztHovrPrepFail ARGS ((SztHovrPrepFail *param, Buffer *mBuf));
EXTERN S16 cmPkSzt_ExtnE_RABToBeSetupItemHOReq_ExtIEsCls ARGS ((Szt_ExtnE_RABToBeSetupItemHOReq_ExtIEsCls *param, U32 val, Buffer *mBuf));
EXTERN S16 cmPkSztProtExtnField_E_RABToBeSetupItemHOReq_ExtIEs ARGS ((SztProtExtnField_E_RABToBeSetupItemHOReq_ExtIEs *param, Buffer *mBuf));
EXTERN S16 cmPkSztProtExtnCont_E_RABToBeSetupItemHOReq_ExtIEs ARGS ((SztProtExtnCont_E_RABToBeSetupItemHOReq_ExtIEs *param, Buffer *mBuf));
EXTERN S16 cmPkSztE_RABToBeSetupItemHOReq ARGS ((SztE_RABToBeSetupItemHOReq *param, Buffer *mBuf));
EXTERN S16 cmPkSzt_ValueE_RABToBeSetupItemHOReqIEsCls ARGS ((Szt_ValueE_RABToBeSetupItemHOReqIEsCls *param, U32 val, Buffer *mBuf));
EXTERN S16 cmPkSztProtIE_Field_E_RABToBeSetupItemHOReqIEs ARGS ((SztProtIE_Field_E_RABToBeSetupItemHOReqIEs *param, Buffer *mBuf));
EXTERN S16 cmPkSztProtIE_ContLst_E_RABToBeSetupItemHOReqIEs ARGS ((SztProtIE_ContLst_E_RABToBeSetupItemHOReqIEs *param, Buffer *mBuf));
EXTERN S16 cmPkSzt_ValueHovrRqstIEsCls ARGS ((Szt_ValueHovrRqstIEsCls *param, U32 val, Buffer *mBuf));
EXTERN S16 cmPkSztProtIE_Field_HovrRqstIEs ARGS ((SztProtIE_Field_HovrRqstIEs *param, Buffer *mBuf));
EXTERN S16 cmPkSztProtIE_Cont_HovrRqstIEs ARGS ((SztProtIE_Cont_HovrRqstIEs *param, Buffer *mBuf));
EXTERN S16 cmPkSztHovrRqst ARGS ((SztHovrRqst *param, Buffer *mBuf));
EXTERN S16 cmPkSzt_ExtnE_RABAdmtdItem_ExtIEsCls ARGS ((Szt_ExtnE_RABAdmtdItem_ExtIEsCls *param, U32 val, Buffer *mBuf));
EXTERN S16 cmPkSztProtExtnField_E_RABAdmtdItem_ExtIEs ARGS ((SztProtExtnField_E_RABAdmtdItem_ExtIEs *param, Buffer *mBuf));
EXTERN S16 cmPkSztProtExtnCont_E_RABAdmtdItem_ExtIEs ARGS ((SztProtExtnCont_E_RABAdmtdItem_ExtIEs *param, Buffer *mBuf));
EXTERN S16 cmPkSztE_RABAdmtdItem ARGS ((SztE_RABAdmtdItem *param, Buffer *mBuf));
EXTERN S16 cmPkSzt_ValueE_RABAdmtdItemIEsCls ARGS ((Szt_ValueE_RABAdmtdItemIEsCls *param, U32 val, Buffer *mBuf));
EXTERN S16 cmPkSztProtIE_Field_E_RABAdmtdItemIEs ARGS ((SztProtIE_Field_E_RABAdmtdItemIEs *param, Buffer *mBuf));
EXTERN S16 cmPkSztProtIE_ContLst_E_RABAdmtdItemIEs ARGS ((SztProtIE_ContLst_E_RABAdmtdItemIEs *param, Buffer *mBuf));
EXTERN S16 cmPkSzt_ExtnE_RABFailedToSetupItemHOReqAckExtIEsCls ARGS ((Szt_ExtnE_RABFailedToSetupItemHOReqAckExtIEsCls *param, U32 val, Buffer *mBuf));
EXTERN S16 cmPkSztProtExtnField_E_RABFailedToSetupItemHOReqAckExtIEs ARGS ((SztProtExtnField_E_RABFailedToSetupItemHOReqAckExtIEs *param, Buffer *mBuf));
EXTERN S16 cmPkSztProtExtnCont_E_RABFailedToSetupItemHOReqAckExtIEs ARGS ((SztProtExtnCont_E_RABFailedToSetupItemHOReqAckExtIEs *param, Buffer *mBuf));
EXTERN S16 cmPkSztE_RABFailedToSetupItemHOReqAck ARGS ((SztE_RABFailedToSetupItemHOReqAck *param, Buffer *mBuf));
EXTERN S16 cmPkSzt_ValueE_RABFailedtoSetupItemHOReqAckIEsCls ARGS ((Szt_ValueE_RABFailedtoSetupItemHOReqAckIEsCls *param, U32 val, Buffer *mBuf));
EXTERN S16 cmPkSztProtIE_Field_E_RABFailedtoSetupItemHOReqAckIEs ARGS ((SztProtIE_Field_E_RABFailedtoSetupItemHOReqAckIEs *param, Buffer *mBuf));
EXTERN S16 cmPkSztProtIE_ContLst_E_RABFailedtoSetupItemHOReqAckIEs ARGS ((SztProtIE_ContLst_E_RABFailedtoSetupItemHOReqAckIEs *param, Buffer *mBuf));
EXTERN S16 cmPkSzt_ValueHovrRqstAckgIEsCls ARGS ((Szt_ValueHovrRqstAckgIEsCls *param, U32 val, Buffer *mBuf));
EXTERN S16 cmPkSztProtIE_Field_HovrRqstAckgIEs ARGS ((SztProtIE_Field_HovrRqstAckgIEs *param, Buffer *mBuf));
EXTERN S16 cmPkSztProtIE_Cont_HovrRqstAckgIEs ARGS ((SztProtIE_Cont_HovrRqstAckgIEs *param, Buffer *mBuf));
EXTERN S16 cmPkSztHovrRqstAckg ARGS ((SztHovrRqstAckg *param, Buffer *mBuf));
EXTERN S16 cmPkSzt_ValueHovrFailIEsCls ARGS ((Szt_ValueHovrFailIEsCls *param, U32 val, Buffer *mBuf));
EXTERN S16 cmPkSztProtIE_Field_HovrFailIEs ARGS ((SztProtIE_Field_HovrFailIEs *param, Buffer *mBuf));
EXTERN S16 cmPkSztProtIE_Cont_HovrFailIEs ARGS ((SztProtIE_Cont_HovrFailIEs *param, Buffer *mBuf));
EXTERN S16 cmPkSztHovrFail ARGS ((SztHovrFail *param, Buffer *mBuf));
EXTERN S16 cmPkSzt_ValueHovrNtfyIEsCls ARGS ((Szt_ValueHovrNtfyIEsCls *param, U32 val, Buffer *mBuf));
EXTERN S16 cmPkSztProtIE_Field_HovrNtfyIEs ARGS ((SztProtIE_Field_HovrNtfyIEs *param, Buffer *mBuf));
EXTERN S16 cmPkSztProtIE_Cont_HovrNtfyIEs ARGS ((SztProtIE_Cont_HovrNtfyIEs *param, Buffer *mBuf));
EXTERN S16 cmPkSztHovrNtfy ARGS ((SztHovrNtfy *param, Buffer *mBuf));
EXTERN S16 cmPkSzt_ExtnE_RABToBeSwedDLItem_ExtIEsCls ARGS ((Szt_ExtnE_RABToBeSwedDLItem_ExtIEsCls *param, U32 val, Buffer *mBuf));
EXTERN S16 cmPkSztProtExtnField_E_RABToBeSwedDLItem_ExtIEs ARGS ((SztProtExtnField_E_RABToBeSwedDLItem_ExtIEs *param, Buffer *mBuf));
EXTERN S16 cmPkSztProtExtnCont_E_RABToBeSwedDLItem_ExtIEs ARGS ((SztProtExtnCont_E_RABToBeSwedDLItem_ExtIEs *param, Buffer *mBuf));
EXTERN S16 cmPkSztE_RABToBeSwedDLItem ARGS ((SztE_RABToBeSwedDLItem *param, Buffer *mBuf));
EXTERN S16 cmPkSzt_ValueE_RABToBeSwedDLItemIEsCls ARGS ((Szt_ValueE_RABToBeSwedDLItemIEsCls *param, U32 val, Buffer *mBuf));
EXTERN S16 cmPkSztProtIE_Field_E_RABToBeSwedDLItemIEs ARGS ((SztProtIE_Field_E_RABToBeSwedDLItemIEs *param, Buffer *mBuf));
EXTERN S16 cmPkSztProtIE_ContLst_E_RABToBeSwedDLItemIEs ARGS ((SztProtIE_ContLst_E_RABToBeSwedDLItemIEs *param, Buffer *mBuf));
EXTERN S16 cmPkSzt_ValuePathSwRqstIEsCls ARGS ((Szt_ValuePathSwRqstIEsCls *param, U32 val, Buffer *mBuf));
EXTERN S16 cmPkSztProtIE_Field_PathSwRqstIEs ARGS ((SztProtIE_Field_PathSwRqstIEs *param, Buffer *mBuf));
EXTERN S16 cmPkSztProtIE_Cont_PathSwRqstIEs ARGS ((SztProtIE_Cont_PathSwRqstIEs *param, Buffer *mBuf));
EXTERN S16 cmPkSztPathSwRqst ARGS ((SztPathSwRqst *param, Buffer *mBuf));
EXTERN S16 cmPkSzt_ExtnE_RABToBeSwedULItem_ExtIEsCls ARGS ((Szt_ExtnE_RABToBeSwedULItem_ExtIEsCls *param, U32 val, Buffer *mBuf));
EXTERN S16 cmPkSztProtExtnField_E_RABToBeSwedULItem_ExtIEs ARGS ((SztProtExtnField_E_RABToBeSwedULItem_ExtIEs *param, Buffer *mBuf));
EXTERN S16 cmPkSztProtExtnCont_E_RABToBeSwedULItem_ExtIEs ARGS ((SztProtExtnCont_E_RABToBeSwedULItem_ExtIEs *param, Buffer *mBuf));
EXTERN S16 cmPkSztE_RABToBeSwedULItem ARGS ((SztE_RABToBeSwedULItem *param, Buffer *mBuf));
EXTERN S16 cmPkSzt_ValueE_RABToBeSwedULItemIEsCls ARGS ((Szt_ValueE_RABToBeSwedULItemIEsCls *param, U32 val, Buffer *mBuf));
EXTERN S16 cmPkSztProtIE_Field_E_RABToBeSwedULItemIEs ARGS ((SztProtIE_Field_E_RABToBeSwedULItemIEs *param, Buffer *mBuf));
EXTERN S16 cmPkSztProtIE_ContLst_E_RABToBeSwedULItemIEs ARGS ((SztProtIE_ContLst_E_RABToBeSwedULItemIEs *param, Buffer *mBuf));
EXTERN S16 cmPkSzt_ValuePathSwRqstAckgIEsCls ARGS ((Szt_ValuePathSwRqstAckgIEsCls *param, U32 val, Buffer *mBuf));
EXTERN S16 cmPkSztProtIE_Field_PathSwRqstAckgIEs ARGS ((SztProtIE_Field_PathSwRqstAckgIEs *param, Buffer *mBuf));
EXTERN S16 cmPkSztProtIE_Cont_PathSwRqstAckgIEs ARGS ((SztProtIE_Cont_PathSwRqstAckgIEs *param, Buffer *mBuf));
EXTERN S16 cmPkSztPathSwRqstAckg ARGS ((SztPathSwRqstAckg *param, Buffer *mBuf));
EXTERN S16 cmPkSzt_ValuePathSwRqstFailIEsCls ARGS ((Szt_ValuePathSwRqstFailIEsCls *param, U32 val, Buffer *mBuf));
EXTERN S16 cmPkSztProtIE_Field_PathSwRqstFailIEs ARGS ((SztProtIE_Field_PathSwRqstFailIEs *param, Buffer *mBuf));
EXTERN S16 cmPkSztProtIE_Cont_PathSwRqstFailIEs ARGS ((SztProtIE_Cont_PathSwRqstFailIEs *param, Buffer *mBuf));
EXTERN S16 cmPkSztPathSwRqstFail ARGS ((SztPathSwRqstFail *param, Buffer *mBuf));
EXTERN S16 cmPkSzt_ValueHovrCancelIEsCls ARGS ((Szt_ValueHovrCancelIEsCls *param, U32 val, Buffer *mBuf));
EXTERN S16 cmPkSztProtIE_Field_HovrCancelIEs ARGS ((SztProtIE_Field_HovrCancelIEs *param, Buffer *mBuf));
EXTERN S16 cmPkSztProtIE_Cont_HovrCancelIEs ARGS ((SztProtIE_Cont_HovrCancelIEs *param, Buffer *mBuf));
EXTERN S16 cmPkSztHovrCancel ARGS ((SztHovrCancel *param, Buffer *mBuf));
EXTERN S16 cmPkSzt_ValueHovrCancelAckgIEsCls ARGS ((Szt_ValueHovrCancelAckgIEsCls *param, U32 val, Buffer *mBuf));
EXTERN S16 cmPkSztProtIE_Field_HovrCancelAckgIEs ARGS ((SztProtIE_Field_HovrCancelAckgIEs *param, Buffer *mBuf));
EXTERN S16 cmPkSztProtIE_Cont_HovrCancelAckgIEs ARGS ((SztProtIE_Cont_HovrCancelAckgIEs *param, Buffer *mBuf));
EXTERN S16 cmPkSztHovrCancelAckg ARGS ((SztHovrCancelAckg *param, Buffer *mBuf));
EXTERN S16 cmPkSzt_ExtnE_RABToBeSetupItemBrSUReqExtIEsCls ARGS ((Szt_ExtnE_RABToBeSetupItemBrSUReqExtIEsCls *param, U32 val, Buffer *mBuf));
EXTERN S16 cmPkSztProtExtnField_E_RABToBeSetupItemBrSUReqExtIEs ARGS ((SztProtExtnField_E_RABToBeSetupItemBrSUReqExtIEs *param, Buffer *mBuf));
EXTERN S16 cmPkSztProtExtnCont_E_RABToBeSetupItemBrSUReqExtIEs ARGS ((SztProtExtnCont_E_RABToBeSetupItemBrSUReqExtIEs *param, Buffer *mBuf));
EXTERN S16 cmPkSztE_RABToBeSetupItemBrSUReq ARGS ((SztE_RABToBeSetupItemBrSUReq *param, Buffer *mBuf));
EXTERN S16 cmPkSzt_ValueE_RABToBeSetupItemBrSUReqIEsCls ARGS ((Szt_ValueE_RABToBeSetupItemBrSUReqIEsCls *param, U32 val, Buffer *mBuf));
EXTERN S16 cmPkSztProtIE_Field_E_RABToBeSetupItemBrSUReqIEs ARGS ((SztProtIE_Field_E_RABToBeSetupItemBrSUReqIEs *param, Buffer *mBuf));
EXTERN S16 cmPkSztE_RABToBeSetupLstBrSUReq ARGS ((SztE_RABToBeSetupLstBrSUReq *param, Buffer *mBuf));
EXTERN S16 cmPkSzt_ValueE_RABSetupRqstIEsCls ARGS ((Szt_ValueE_RABSetupRqstIEsCls *param, U32 val, Buffer *mBuf));
EXTERN S16 cmPkSztProtIE_Field_E_RABSetupRqstIEs ARGS ((SztProtIE_Field_E_RABSetupRqstIEs *param, Buffer *mBuf));
EXTERN S16 cmPkSztProtIE_Cont_E_RABSetupRqstIEs ARGS ((SztProtIE_Cont_E_RABSetupRqstIEs *param, Buffer *mBuf));
EXTERN S16 cmPkSztE_RABSetupRqst ARGS ((SztE_RABSetupRqst *param, Buffer *mBuf));
EXTERN S16 cmPkSzt_ExtnE_RABSetupItemBrSUResExtIEsCls ARGS ((Szt_ExtnE_RABSetupItemBrSUResExtIEsCls *param, U32 val, Buffer *mBuf));
EXTERN S16 cmPkSztProtExtnField_E_RABSetupItemBrSUResExtIEs ARGS ((SztProtExtnField_E_RABSetupItemBrSUResExtIEs *param, Buffer *mBuf));
EXTERN S16 cmPkSztProtExtnCont_E_RABSetupItemBrSUResExtIEs ARGS ((SztProtExtnCont_E_RABSetupItemBrSUResExtIEs *param, Buffer *mBuf));
EXTERN S16 cmPkSztE_RABSetupItemBrSURes ARGS ((SztE_RABSetupItemBrSURes *param, Buffer *mBuf));
EXTERN S16 cmPkSzt_ValueE_RABSetupItemBrSUResIEsCls ARGS ((Szt_ValueE_RABSetupItemBrSUResIEsCls *param, U32 val, Buffer *mBuf));
EXTERN S16 cmPkSztProtIE_Field_E_RABSetupItemBrSUResIEs ARGS ((SztProtIE_Field_E_RABSetupItemBrSUResIEs *param, Buffer *mBuf));
EXTERN S16 cmPkSztE_RABSetupLstBrSURes ARGS ((SztE_RABSetupLstBrSURes *param, Buffer *mBuf));
EXTERN S16 cmPkSzt_ValueE_RABSetupRespIEsCls ARGS ((Szt_ValueE_RABSetupRespIEsCls *param, U32 val, Buffer *mBuf));
EXTERN S16 cmPkSztProtIE_Field_E_RABSetupRespIEs ARGS ((SztProtIE_Field_E_RABSetupRespIEs *param, Buffer *mBuf));
EXTERN S16 cmPkSztProtIE_Cont_E_RABSetupRespIEs ARGS ((SztProtIE_Cont_E_RABSetupRespIEs *param, Buffer *mBuf));
EXTERN S16 cmPkSztE_RABSetupResp ARGS ((SztE_RABSetupResp *param, Buffer *mBuf));
EXTERN S16 cmPkSzt_ExtnE_RABToBeMdfyItemBrModReqExtIEsCls ARGS ((Szt_ExtnE_RABToBeMdfyItemBrModReqExtIEsCls *param, U32 val, Buffer *mBuf));
EXTERN S16 cmPkSztProtExtnField_E_RABToBeMdfyItemBrModReqExtIEs ARGS ((SztProtExtnField_E_RABToBeMdfyItemBrModReqExtIEs *param, Buffer *mBuf));
EXTERN S16 cmPkSztProtExtnCont_E_RABToBeMdfyItemBrModReqExtIEs ARGS ((SztProtExtnCont_E_RABToBeMdfyItemBrModReqExtIEs *param, Buffer *mBuf));
EXTERN S16 cmPkSztE_RABToBeMdfdItemBrModReq ARGS ((SztE_RABToBeMdfdItemBrModReq *param, Buffer *mBuf));
EXTERN S16 cmPkSzt_ValueE_RABToBeMdfdItemBrModReqIEsCls ARGS ((Szt_ValueE_RABToBeMdfdItemBrModReqIEsCls *param, U32 val, Buffer *mBuf));
EXTERN S16 cmPkSztProtIE_Field_E_RABToBeMdfdItemBrModReqIEs ARGS ((SztProtIE_Field_E_RABToBeMdfdItemBrModReqIEs *param, Buffer *mBuf));
EXTERN S16 cmPkSztE_RABToBeMdfdLstBrModReq ARGS ((SztE_RABToBeMdfdLstBrModReq *param, Buffer *mBuf));
EXTERN S16 cmPkSzt_ValueE_RABMdfyRqstIEsCls ARGS ((Szt_ValueE_RABMdfyRqstIEsCls *param, U32 val, Buffer *mBuf));
EXTERN S16 cmPkSztProtIE_Field_E_RABMdfyRqstIEs ARGS ((SztProtIE_Field_E_RABMdfyRqstIEs *param, Buffer *mBuf));
EXTERN S16 cmPkSztProtIE_Cont_E_RABMdfyRqstIEs ARGS ((SztProtIE_Cont_E_RABMdfyRqstIEs *param, Buffer *mBuf));
EXTERN S16 cmPkSztE_RABMdfyRqst ARGS ((SztE_RABMdfyRqst *param, Buffer *mBuf));
EXTERN S16 cmPkSzt_ExtnE_RABMdfyItemBrModResExtIEsCls ARGS ((Szt_ExtnE_RABMdfyItemBrModResExtIEsCls *param, U32 val, Buffer *mBuf));
EXTERN S16 cmPkSztProtExtnField_E_RABMdfyItemBrModResExtIEs ARGS ((SztProtExtnField_E_RABMdfyItemBrModResExtIEs *param, Buffer *mBuf));
EXTERN S16 cmPkSztProtExtnCont_E_RABMdfyItemBrModResExtIEs ARGS ((SztProtExtnCont_E_RABMdfyItemBrModResExtIEs *param, Buffer *mBuf));
EXTERN S16 cmPkSztE_RABMdfyItemBrModRes ARGS ((SztE_RABMdfyItemBrModRes *param, Buffer *mBuf));
EXTERN S16 cmPkSzt_ValueE_RABMdfyItemBrModResIEsCls ARGS ((Szt_ValueE_RABMdfyItemBrModResIEsCls *param, U32 val, Buffer *mBuf));
EXTERN S16 cmPkSztProtIE_Field_E_RABMdfyItemBrModResIEs ARGS ((SztProtIE_Field_E_RABMdfyItemBrModResIEs *param, Buffer *mBuf));
EXTERN S16 cmPkSztE_RABMdfyLstBrModRes ARGS ((SztE_RABMdfyLstBrModRes *param, Buffer *mBuf));
EXTERN S16 cmPkSzt_ValueE_RABMdfyRespIEsCls ARGS ((Szt_ValueE_RABMdfyRespIEsCls *param, U32 val, Buffer *mBuf));
EXTERN S16 cmPkSztProtIE_Field_E_RABMdfyRespIEs ARGS ((SztProtIE_Field_E_RABMdfyRespIEs *param, Buffer *mBuf));
EXTERN S16 cmPkSztProtIE_Cont_E_RABMdfyRespIEs ARGS ((SztProtIE_Cont_E_RABMdfyRespIEs *param, Buffer *mBuf));
EXTERN S16 cmPkSztE_RABMdfyResp ARGS ((SztE_RABMdfyResp *param, Buffer *mBuf));
EXTERN S16 cmPkSzt_ValueE_RABRlsCmmdIEsCls ARGS ((Szt_ValueE_RABRlsCmmdIEsCls *param, U32 val, Buffer *mBuf));
EXTERN S16 cmPkSztProtIE_Field_E_RABRlsCmmdIEs ARGS ((SztProtIE_Field_E_RABRlsCmmdIEs *param, Buffer *mBuf));
EXTERN S16 cmPkSztProtIE_Cont_E_RABRlsCmmdIEs ARGS ((SztProtIE_Cont_E_RABRlsCmmdIEs *param, Buffer *mBuf));
EXTERN S16 cmPkSztE_RABRlsCmmd ARGS ((SztE_RABRlsCmmd *param, Buffer *mBuf));
EXTERN S16 cmPkSzt_ExtnE_RABRlsItemBrRelCompExtIEsCls ARGS ((Szt_ExtnE_RABRlsItemBrRelCompExtIEsCls *param, U32 val, Buffer *mBuf));
EXTERN S16 cmPkSztProtExtnField_E_RABRlsItemBrRelCompExtIEs ARGS ((SztProtExtnField_E_RABRlsItemBrRelCompExtIEs *param, Buffer *mBuf));
EXTERN S16 cmPkSztProtExtnCont_E_RABRlsItemBrRelCompExtIEs ARGS ((SztProtExtnCont_E_RABRlsItemBrRelCompExtIEs *param, Buffer *mBuf));
EXTERN S16 cmPkSztE_RABRlsItemBrRelComp ARGS ((SztE_RABRlsItemBrRelComp *param, Buffer *mBuf));
EXTERN S16 cmPkSzt_ValueE_RABRlsItemBrRelCompIEsCls ARGS ((Szt_ValueE_RABRlsItemBrRelCompIEsCls *param, U32 val, Buffer *mBuf));
EXTERN S16 cmPkSztProtIE_Field_E_RABRlsItemBrRelCompIEs ARGS ((SztProtIE_Field_E_RABRlsItemBrRelCompIEs *param, Buffer *mBuf));
EXTERN S16 cmPkSztE_RABRlsLstBrRelComp ARGS ((SztE_RABRlsLstBrRelComp *param, Buffer *mBuf));
EXTERN S16 cmPkSzt_ValueE_RABRlsRespIEsCls ARGS ((Szt_ValueE_RABRlsRespIEsCls *param, U32 val, Buffer *mBuf));
EXTERN S16 cmPkSztProtIE_Field_E_RABRlsRespIEs ARGS ((SztProtIE_Field_E_RABRlsRespIEs *param, Buffer *mBuf));
EXTERN S16 cmPkSztProtIE_Cont_E_RABRlsRespIEs ARGS ((SztProtIE_Cont_E_RABRlsRespIEs *param, Buffer *mBuf));
EXTERN S16 cmPkSztE_RABRlsResp ARGS ((SztE_RABRlsResp *param, Buffer *mBuf));
EXTERN S16 cmPkSzt_ValueE_RABRlsIndIEsCls ARGS ((Szt_ValueE_RABRlsIndIEsCls *param, U32 val, Buffer *mBuf));
EXTERN S16 cmPkSztProtIE_Field_E_RABRlsIndIEs ARGS ((SztProtIE_Field_E_RABRlsIndIEs *param, Buffer *mBuf));
EXTERN S16 cmPkSztProtIE_Cont_E_RABRlsIndIEs ARGS ((SztProtIE_Cont_E_RABRlsIndIEs *param, Buffer *mBuf));
EXTERN S16 cmPkSztE_RABRlsInd ARGS ((SztE_RABRlsInd *param, Buffer *mBuf));
EXTERN S16 cmPkSzt_ExtnE_RABToBeSetupItemCtxtSUReqExtIEsCls ARGS ((Szt_ExtnE_RABToBeSetupItemCtxtSUReqExtIEsCls *param, U32 val, Buffer *mBuf));
EXTERN S16 cmPkSztProtExtnField_E_RABToBeSetupItemCtxtSUReqExtIEs ARGS ((SztProtExtnField_E_RABToBeSetupItemCtxtSUReqExtIEs *param, Buffer *mBuf));
EXTERN S16 cmPkSztProtExtnCont_E_RABToBeSetupItemCtxtSUReqExtIEs ARGS ((SztProtExtnCont_E_RABToBeSetupItemCtxtSUReqExtIEs *param, Buffer *mBuf));
EXTERN S16 cmPkSztE_RABToBeSetupItemCtxtSUReq ARGS ((SztE_RABToBeSetupItemCtxtSUReq *param, Buffer *mBuf));
EXTERN S16 cmPkSzt_ValueE_RABToBeSetupItemCtxtSUReqIEsCls ARGS ((Szt_ValueE_RABToBeSetupItemCtxtSUReqIEsCls *param, U32 val, Buffer *mBuf));
EXTERN S16 cmPkSztProtIE_Field_E_RABToBeSetupItemCtxtSUReqIEs ARGS ((SztProtIE_Field_E_RABToBeSetupItemCtxtSUReqIEs *param, Buffer *mBuf));
EXTERN S16 cmPkSztE_RABToBeSetupLstCtxtSUReq ARGS ((SztE_RABToBeSetupLstCtxtSUReq *param, Buffer *mBuf));
EXTERN S16 cmPkSzt_ValueInitCntxtSetupRqstIEsCls ARGS ((Szt_ValueInitCntxtSetupRqstIEsCls *param, U32 val, Buffer *mBuf));
EXTERN S16 cmPkSztProtIE_Field_InitCntxtSetupRqstIEs ARGS ((SztProtIE_Field_InitCntxtSetupRqstIEs *param, Buffer *mBuf));
EXTERN S16 cmPkSztProtIE_Cont_InitCntxtSetupRqstIEs ARGS ((SztProtIE_Cont_InitCntxtSetupRqstIEs *param, Buffer *mBuf));
EXTERN S16 cmPkSztInitCntxtSetupRqst ARGS ((SztInitCntxtSetupRqst *param, Buffer *mBuf));
EXTERN S16 cmPkSzt_ExtnE_RABSetupItemCtxtSUResExtIEsCls ARGS ((Szt_ExtnE_RABSetupItemCtxtSUResExtIEsCls *param, U32 val, Buffer *mBuf));
EXTERN S16 cmPkSztProtExtnField_E_RABSetupItemCtxtSUResExtIEs ARGS ((SztProtExtnField_E_RABSetupItemCtxtSUResExtIEs *param, Buffer *mBuf));
EXTERN S16 cmPkSztProtExtnCont_E_RABSetupItemCtxtSUResExtIEs ARGS ((SztProtExtnCont_E_RABSetupItemCtxtSUResExtIEs *param, Buffer *mBuf));
EXTERN S16 cmPkSztE_RABSetupItemCtxtSURes ARGS ((SztE_RABSetupItemCtxtSURes *param, Buffer *mBuf));
EXTERN S16 cmPkSzt_ValueE_RABSetupItemCtxtSUResIEsCls ARGS ((Szt_ValueE_RABSetupItemCtxtSUResIEsCls *param, U32 val, Buffer *mBuf));
EXTERN S16 cmPkSztProtIE_Field_E_RABSetupItemCtxtSUResIEs ARGS ((SztProtIE_Field_E_RABSetupItemCtxtSUResIEs *param, Buffer *mBuf));
EXTERN S16 cmPkSztE_RABSetupLstCtxtSURes ARGS ((SztE_RABSetupLstCtxtSURes *param, Buffer *mBuf));
EXTERN S16 cmPkSzt_ValueInitCntxtSetupRespIEsCls ARGS ((Szt_ValueInitCntxtSetupRespIEsCls *param, U32 val, Buffer *mBuf));
EXTERN S16 cmPkSztProtIE_Field_InitCntxtSetupRespIEs ARGS ((SztProtIE_Field_InitCntxtSetupRespIEs *param, Buffer *mBuf));
EXTERN S16 cmPkSztProtIE_Cont_InitCntxtSetupRespIEs ARGS ((SztProtIE_Cont_InitCntxtSetupRespIEs *param, Buffer *mBuf));
EXTERN S16 cmPkSztInitCntxtSetupResp ARGS ((SztInitCntxtSetupResp *param, Buffer *mBuf));
EXTERN S16 cmPkSzt_ValueInitCntxtSetupFailIEsCls ARGS ((Szt_ValueInitCntxtSetupFailIEsCls *param, U32 val, Buffer *mBuf));
EXTERN S16 cmPkSztProtIE_Field_InitCntxtSetupFailIEs ARGS ((SztProtIE_Field_InitCntxtSetupFailIEs *param, Buffer *mBuf));
EXTERN S16 cmPkSztProtIE_Cont_InitCntxtSetupFailIEs ARGS ((SztProtIE_Cont_InitCntxtSetupFailIEs *param, Buffer *mBuf));
EXTERN S16 cmPkSztInitCntxtSetupFail ARGS ((SztInitCntxtSetupFail *param, Buffer *mBuf));
EXTERN S16 cmPkSzt_ExtnTAIItemExtIEsCls ARGS ((Szt_ExtnTAIItemExtIEsCls *param, U32 val, Buffer *mBuf));
EXTERN S16 cmPkSztProtExtnField_TAIItemExtIEs ARGS ((SztProtExtnField_TAIItemExtIEs *param, Buffer *mBuf));
EXTERN S16 cmPkSztProtExtnCont_TAIItemExtIEs ARGS ((SztProtExtnCont_TAIItemExtIEs *param, Buffer *mBuf));
EXTERN S16 cmPkSztTAIItem ARGS ((SztTAIItem *param, Buffer *mBuf));
EXTERN S16 cmPkSzt_ValueTAIItemIEsCls ARGS ((Szt_ValueTAIItemIEsCls *param, U32 val, Buffer *mBuf));
EXTERN S16 cmPkSztProtIE_Field_TAIItemIEs ARGS ((SztProtIE_Field_TAIItemIEs *param, Buffer *mBuf));
EXTERN S16 cmPkSztTAILst ARGS ((SztTAILst *param, Buffer *mBuf));
EXTERN S16 cmPkSzt_ValuePagIEsCls ARGS ((Szt_ValuePagIEsCls *param, U32 val, Buffer *mBuf));
EXTERN S16 cmPkSztProtIE_Field_PagIEs ARGS ((SztProtIE_Field_PagIEs *param, Buffer *mBuf));
EXTERN S16 cmPkSztProtIE_Cont_PagIEs ARGS ((SztProtIE_Cont_PagIEs *param, Buffer *mBuf));
EXTERN S16 cmPkSztPag ARGS ((SztPag *param, Buffer *mBuf));
EXTERN S16 cmPkSzt_ValueUECntxtRlsRqst_IEsCls ARGS ((Szt_ValueUECntxtRlsRqst_IEsCls *param, U32 val, Buffer *mBuf));
EXTERN S16 cmPkSztProtIE_Field_UECntxtRlsRqst_IEs ARGS ((SztProtIE_Field_UECntxtRlsRqst_IEs *param, Buffer *mBuf));
EXTERN S16 cmPkSztProtIE_Cont_UECntxtRlsRqst_IEs ARGS ((SztProtIE_Cont_UECntxtRlsRqst_IEs *param, Buffer *mBuf));
EXTERN S16 cmPkSztUECntxtRlsRqst ARGS ((SztUECntxtRlsRqst *param, Buffer *mBuf));
EXTERN S16 cmPkSzt_ValueUECntxtRlsCmmd_IEsCls ARGS ((Szt_ValueUECntxtRlsCmmd_IEsCls *param, U32 val, Buffer *mBuf));
EXTERN S16 cmPkSztProtIE_Field_UECntxtRlsCmmd_IEs ARGS ((SztProtIE_Field_UECntxtRlsCmmd_IEs *param, Buffer *mBuf));
EXTERN S16 cmPkSztProtIE_Cont_UECntxtRlsCmmd_IEs ARGS ((SztProtIE_Cont_UECntxtRlsCmmd_IEs *param, Buffer *mBuf));
EXTERN S16 cmPkSztUECntxtRlsCmmd ARGS ((SztUECntxtRlsCmmd *param, Buffer *mBuf));
EXTERN S16 cmPkSzt_ValueUECntxtRlsComplete_IEsCls ARGS ((Szt_ValueUECntxtRlsComplete_IEsCls *param, U32 val, Buffer *mBuf));
EXTERN S16 cmPkSztProtIE_Field_UECntxtRlsComplete_IEs ARGS ((SztProtIE_Field_UECntxtRlsComplete_IEs *param, Buffer *mBuf));
EXTERN S16 cmPkSztProtIE_Cont_UECntxtRlsComplete_IEs ARGS ((SztProtIE_Cont_UECntxtRlsComplete_IEs *param, Buffer *mBuf));
EXTERN S16 cmPkSztUECntxtRlsComplete ARGS ((SztUECntxtRlsComplete *param, Buffer *mBuf));
EXTERN S16 cmPkSzt_ValueUECntxtModificationRqstIEsCls ARGS ((Szt_ValueUECntxtModificationRqstIEsCls *param, U32 val, Buffer *mBuf));
EXTERN S16 cmPkSztProtIE_Field_UECntxtModificationRqstIEs ARGS ((SztProtIE_Field_UECntxtModificationRqstIEs *param, Buffer *mBuf));
EXTERN S16 cmPkSztProtIE_Cont_UECntxtModificationRqstIEs ARGS ((SztProtIE_Cont_UECntxtModificationRqstIEs *param, Buffer *mBuf));
EXTERN S16 cmPkSztUECntxtModificationRqst ARGS ((SztUECntxtModificationRqst *param, Buffer *mBuf));
EXTERN S16 cmPkSzt_ValueUECntxtModificationRespIEsCls ARGS ((Szt_ValueUECntxtModificationRespIEsCls *param, U32 val, Buffer *mBuf));
EXTERN S16 cmPkSztProtIE_Field_UECntxtModificationRespIEs ARGS ((SztProtIE_Field_UECntxtModificationRespIEs *param, Buffer *mBuf));
EXTERN S16 cmPkSztProtIE_Cont_UECntxtModificationRespIEs ARGS ((SztProtIE_Cont_UECntxtModificationRespIEs *param, Buffer *mBuf));
EXTERN S16 cmPkSztUECntxtModificationResp ARGS ((SztUECntxtModificationResp *param, Buffer *mBuf));
EXTERN S16 cmPkSzt_ValueUECntxtModificationFailIEsCls ARGS ((Szt_ValueUECntxtModificationFailIEsCls *param, U32 val, Buffer *mBuf));
EXTERN S16 cmPkSztProtIE_Field_UECntxtModificationFailIEs ARGS ((SztProtIE_Field_UECntxtModificationFailIEs *param, Buffer *mBuf));
EXTERN S16 cmPkSztProtIE_Cont_UECntxtModificationFailIEs ARGS ((SztProtIE_Cont_UECntxtModificationFailIEs *param, Buffer *mBuf));
EXTERN S16 cmPkSztUECntxtModificationFail ARGS ((SztUECntxtModificationFail *param, Buffer *mBuf));
EXTERN S16 cmPkSzt_ValueUERadioCapbltyMatchRqstIEsCls ARGS ((Szt_ValueUERadioCapbltyMatchRqstIEsCls *param, U32 val, Buffer *mBuf));
EXTERN S16 cmPkSztProtIE_Field_UERadioCapbltyMatchRqstIEs ARGS ((SztProtIE_Field_UERadioCapbltyMatchRqstIEs *param, Buffer *mBuf));
EXTERN S16 cmPkSztProtIE_Cont_UERadioCapbltyMatchRqstIEs ARGS ((SztProtIE_Cont_UERadioCapbltyMatchRqstIEs *param, Buffer *mBuf));
EXTERN S16 cmPkSztUERadioCapbltyMatchRqst ARGS ((SztUERadioCapbltyMatchRqst *param, Buffer *mBuf));
EXTERN S16 cmPkSzt_ValueUERadioCapbltyMatchRespIEsCls ARGS ((Szt_ValueUERadioCapbltyMatchRespIEsCls *param, U32 val, Buffer *mBuf));
EXTERN S16 cmPkSztProtIE_Field_UERadioCapbltyMatchRespIEs ARGS ((SztProtIE_Field_UERadioCapbltyMatchRespIEs *param, Buffer *mBuf));
EXTERN S16 cmPkSztProtIE_Cont_UERadioCapbltyMatchRespIEs ARGS ((SztProtIE_Cont_UERadioCapbltyMatchRespIEs *param, Buffer *mBuf));
EXTERN S16 cmPkSztUERadioCapbltyMatchResp ARGS ((SztUERadioCapbltyMatchResp *param, Buffer *mBuf));
EXTERN S16 cmPkSzt_ValueDlnkNASTport_IEsCls ARGS ((Szt_ValueDlnkNASTport_IEsCls *param, U32 val, Buffer *mBuf));
EXTERN S16 cmPkSztProtIE_Field_DlnkNASTport_IEs ARGS ((SztProtIE_Field_DlnkNASTport_IEs *param, Buffer *mBuf));
EXTERN S16 cmPkSztProtIE_Cont_DlnkNASTport_IEs ARGS ((SztProtIE_Cont_DlnkNASTport_IEs *param, Buffer *mBuf));
EXTERN S16 cmPkSztDlnkNASTport ARGS ((SztDlnkNASTport *param, Buffer *mBuf));
EXTERN S16 cmPkSzt_ValueInitUEMsg_IEsCls ARGS ((Szt_ValueInitUEMsg_IEsCls *param, U32 val, Buffer *mBuf));
EXTERN S16 cmPkSztProtIE_Field_InitUEMsg_IEs ARGS ((SztProtIE_Field_InitUEMsg_IEs *param, Buffer *mBuf));
EXTERN S16 cmPkSztProtIE_Cont_InitUEMsg_IEs ARGS ((SztProtIE_Cont_InitUEMsg_IEs *param, Buffer *mBuf));
EXTERN S16 cmPkSztInitUEMsg ARGS ((SztInitUEMsg *param, Buffer *mBuf));
EXTERN S16 cmPkSzt_ValueUlnkNASTport_IEsCls ARGS ((Szt_ValueUlnkNASTport_IEsCls *param, U32 val, Buffer *mBuf));
EXTERN S16 cmPkSztProtIE_Field_UlnkNASTport_IEs ARGS ((SztProtIE_Field_UlnkNASTport_IEs *param, Buffer *mBuf));
EXTERN S16 cmPkSztProtIE_Cont_UlnkNASTport_IEs ARGS ((SztProtIE_Cont_UlnkNASTport_IEs *param, Buffer *mBuf));
EXTERN S16 cmPkSztUlnkNASTport ARGS ((SztUlnkNASTport *param, Buffer *mBuf));
EXTERN S16 cmPkSzt_ValueNASNonDlvryInd_IEsCls ARGS ((Szt_ValueNASNonDlvryInd_IEsCls *param, U32 val, Buffer *mBuf));
EXTERN S16 cmPkSztProtIE_Field_NASNonDlvryInd_IEs ARGS ((SztProtIE_Field_NASNonDlvryInd_IEs *param, Buffer *mBuf));
EXTERN S16 cmPkSztProtIE_Cont_NASNonDlvryInd_IEs ARGS ((SztProtIE_Cont_NASNonDlvryInd_IEs *param, Buffer *mBuf));
EXTERN S16 cmPkSztNASNonDlvryInd ARGS ((SztNASNonDlvryInd *param, Buffer *mBuf));
EXTERN S16 cmPkSzt_ValueUE_assocLogS1_ConItemResCls ARGS ((Szt_ValueUE_assocLogS1_ConItemResCls *param, U32 val, Buffer *mBuf));
EXTERN S16 cmPkSztProtIE_Field_UE_assocLogS1_ConItemRes ARGS ((SztProtIE_Field_UE_assocLogS1_ConItemRes *param, Buffer *mBuf));
EXTERN S16 cmPkSztUE_assocLogS1_ConLstRes ARGS ((SztUE_assocLogS1_ConLstRes *param, Buffer *mBuf));
EXTERN S16 cmPkSztResetTyp ARGS ((SztResetTyp *param, Buffer *mBuf));
EXTERN S16 cmPkSzt_ValueResetIEsCls ARGS ((Szt_ValueResetIEsCls *param, U32 val, Buffer *mBuf));
EXTERN S16 cmPkSztProtIE_Field_ResetIEs ARGS ((SztProtIE_Field_ResetIEs *param, Buffer *mBuf));
EXTERN S16 cmPkSztProtIE_Cont_ResetIEs ARGS ((SztProtIE_Cont_ResetIEs *param, Buffer *mBuf));
EXTERN S16 cmPkSztReset ARGS ((SztReset *param, Buffer *mBuf));
EXTERN S16 cmPkSzt_ValueUE_assocLogS1_ConItemResAckCls ARGS ((Szt_ValueUE_assocLogS1_ConItemResAckCls *param, U32 val, Buffer *mBuf));
EXTERN S16 cmPkSztProtIE_Field_UE_assocLogS1_ConItemResAck ARGS ((SztProtIE_Field_UE_assocLogS1_ConItemResAck *param, Buffer *mBuf));
EXTERN S16 cmPkSztUE_assocLogS1_ConLstResAck ARGS ((SztUE_assocLogS1_ConLstResAck *param, Buffer *mBuf));
EXTERN S16 cmPkSzt_ValueResetAckgIEsCls ARGS ((Szt_ValueResetAckgIEsCls *param, U32 val, Buffer *mBuf));
EXTERN S16 cmPkSztProtIE_Field_ResetAckgIEs ARGS ((SztProtIE_Field_ResetAckgIEs *param, Buffer *mBuf));
EXTERN S16 cmPkSztProtIE_Cont_ResetAckgIEs ARGS ((SztProtIE_Cont_ResetAckgIEs *param, Buffer *mBuf));
EXTERN S16 cmPkSztResetAckg ARGS ((SztResetAckg *param, Buffer *mBuf));
EXTERN S16 cmPkSzt_ValueErrIndIEsCls ARGS ((Szt_ValueErrIndIEsCls *param, U32 val, Buffer *mBuf));
EXTERN S16 cmPkSztProtIE_Field_ErrIndIEs ARGS ((SztProtIE_Field_ErrIndIEs *param, Buffer *mBuf));
EXTERN S16 cmPkSztProtIE_Cont_ErrIndIEs ARGS ((SztProtIE_Cont_ErrIndIEs *param, Buffer *mBuf));
EXTERN S16 cmPkSztErrInd ARGS ((SztErrInd *param, Buffer *mBuf));
EXTERN S16 cmPkSzt_ValueS1SetupRqstIEsCls ARGS ((Szt_ValueS1SetupRqstIEsCls *param, U32 val, Buffer *mBuf));
EXTERN S16 cmPkSztProtIE_Field_S1SetupRqstIEs ARGS ((SztProtIE_Field_S1SetupRqstIEs *param, Buffer *mBuf));
EXTERN S16 cmPkSztProtIE_Cont_S1SetupRqstIEs ARGS ((SztProtIE_Cont_S1SetupRqstIEs *param, Buffer *mBuf));
EXTERN S16 cmPkSztS1SetupRqst ARGS ((SztS1SetupRqst *param, Buffer *mBuf));
EXTERN S16 cmPkSzt_ValueS1SetupRespIEsCls ARGS ((Szt_ValueS1SetupRespIEsCls *param, U32 val, Buffer *mBuf));
EXTERN S16 cmPkSztProtIE_Field_S1SetupRespIEs ARGS ((SztProtIE_Field_S1SetupRespIEs *param, Buffer *mBuf));
EXTERN S16 cmPkSztProtIE_Cont_S1SetupRespIEs ARGS ((SztProtIE_Cont_S1SetupRespIEs *param, Buffer *mBuf));
EXTERN S16 cmPkSztS1SetupResp ARGS ((SztS1SetupResp *param, Buffer *mBuf));
EXTERN S16 cmPkSzt_ValueS1SetupFailIEsCls ARGS ((Szt_ValueS1SetupFailIEsCls *param, U32 val, Buffer *mBuf));
EXTERN S16 cmPkSztProtIE_Field_S1SetupFailIEs ARGS ((SztProtIE_Field_S1SetupFailIEs *param, Buffer *mBuf));
EXTERN S16 cmPkSztProtIE_Cont_S1SetupFailIEs ARGS ((SztProtIE_Cont_S1SetupFailIEs *param, Buffer *mBuf));
EXTERN S16 cmPkSztS1SetupFail ARGS ((SztS1SetupFail *param, Buffer *mBuf));
EXTERN S16 cmPkSzt_ValueENBConfigUpdIEsCls ARGS ((Szt_ValueENBConfigUpdIEsCls *param, U32 val, Buffer *mBuf));
EXTERN S16 cmPkSztProtIE_Field_ENBConfigUpdIEs ARGS ((SztProtIE_Field_ENBConfigUpdIEs *param, Buffer *mBuf));
EXTERN S16 cmPkSztProtIE_Cont_ENBConfigUpdIEs ARGS ((SztProtIE_Cont_ENBConfigUpdIEs *param, Buffer *mBuf));
EXTERN S16 cmPkSztENBConfigUpd ARGS ((SztENBConfigUpd *param, Buffer *mBuf));
EXTERN S16 cmPkSzt_ValueENBConfigUpdAckgIEsCls ARGS ((Szt_ValueENBConfigUpdAckgIEsCls *param, U32 val, Buffer *mBuf));
EXTERN S16 cmPkSztProtIE_Field_ENBConfigUpdAckgIEs ARGS ((SztProtIE_Field_ENBConfigUpdAckgIEs *param, Buffer *mBuf));
EXTERN S16 cmPkSztProtIE_Cont_ENBConfigUpdAckgIEs ARGS ((SztProtIE_Cont_ENBConfigUpdAckgIEs *param, Buffer *mBuf));
EXTERN S16 cmPkSztENBConfigUpdAckg ARGS ((SztENBConfigUpdAckg *param, Buffer *mBuf));
EXTERN S16 cmPkSzt_ValueENBConfigUpdFailIEsCls ARGS ((Szt_ValueENBConfigUpdFailIEsCls *param, U32 val, Buffer *mBuf));
EXTERN S16 cmPkSztProtIE_Field_ENBConfigUpdFailIEs ARGS ((SztProtIE_Field_ENBConfigUpdFailIEs *param, Buffer *mBuf));
EXTERN S16 cmPkSztProtIE_Cont_ENBConfigUpdFailIEs ARGS ((SztProtIE_Cont_ENBConfigUpdFailIEs *param, Buffer *mBuf));
EXTERN S16 cmPkSztENBConfigUpdFail ARGS ((SztENBConfigUpdFail *param, Buffer *mBuf));
EXTERN S16 cmPkSzt_ValueMMEConfigUpdIEsCls ARGS ((Szt_ValueMMEConfigUpdIEsCls *param, U32 val, Buffer *mBuf));
EXTERN S16 cmPkSztProtIE_Field_MMEConfigUpdIEs ARGS ((SztProtIE_Field_MMEConfigUpdIEs *param, Buffer *mBuf));
EXTERN S16 cmPkSztProtIE_Cont_MMEConfigUpdIEs ARGS ((SztProtIE_Cont_MMEConfigUpdIEs *param, Buffer *mBuf));
EXTERN S16 cmPkSztMMEConfigUpd ARGS ((SztMMEConfigUpd *param, Buffer *mBuf));
EXTERN S16 cmPkSzt_ValueMMEConfigUpdAckgIEsCls ARGS ((Szt_ValueMMEConfigUpdAckgIEsCls *param, U32 val, Buffer *mBuf));
EXTERN S16 cmPkSztProtIE_Field_MMEConfigUpdAckgIEs ARGS ((SztProtIE_Field_MMEConfigUpdAckgIEs *param, Buffer *mBuf));
EXTERN S16 cmPkSztProtIE_Cont_MMEConfigUpdAckgIEs ARGS ((SztProtIE_Cont_MMEConfigUpdAckgIEs *param, Buffer *mBuf));
EXTERN S16 cmPkSztMMEConfigUpdAckg ARGS ((SztMMEConfigUpdAckg *param, Buffer *mBuf));
EXTERN S16 cmPkSzt_ValueMMEConfigUpdFailIEsCls ARGS ((Szt_ValueMMEConfigUpdFailIEsCls *param, U32 val, Buffer *mBuf));
EXTERN S16 cmPkSztProtIE_Field_MMEConfigUpdFailIEs ARGS ((SztProtIE_Field_MMEConfigUpdFailIEs *param, Buffer *mBuf));
EXTERN S16 cmPkSztProtIE_Cont_MMEConfigUpdFailIEs ARGS ((SztProtIE_Cont_MMEConfigUpdFailIEs *param, Buffer *mBuf));
EXTERN S16 cmPkSztMMEConfigUpdFail ARGS ((SztMMEConfigUpdFail *param, Buffer *mBuf));
EXTERN S16 cmPkSzt_ValueDlnkS1cdma2000tunnelingIEsCls ARGS ((Szt_ValueDlnkS1cdma2000tunnelingIEsCls *param, U32 val, Buffer *mBuf));
EXTERN S16 cmPkSztProtIE_Field_DlnkS1cdma2000tunnelingIEs ARGS ((SztProtIE_Field_DlnkS1cdma2000tunnelingIEs *param, Buffer *mBuf));
EXTERN S16 cmPkSztProtIE_Cont_DlnkS1cdma2000tunnelingIEs ARGS ((SztProtIE_Cont_DlnkS1cdma2000tunnelingIEs *param, Buffer *mBuf));
EXTERN S16 cmPkSztDlnkS1cdma2000tunneling ARGS ((SztDlnkS1cdma2000tunneling *param, Buffer *mBuf));
EXTERN S16 cmPkSzt_ValueUlnkS1cdma2000tunnelingIEsCls ARGS ((Szt_ValueUlnkS1cdma2000tunnelingIEsCls *param, U32 val, Buffer *mBuf));
EXTERN S16 cmPkSztProtIE_Field_UlnkS1cdma2000tunnelingIEs ARGS ((SztProtIE_Field_UlnkS1cdma2000tunnelingIEs *param, Buffer *mBuf));
EXTERN S16 cmPkSztProtIE_Cont_UlnkS1cdma2000tunnelingIEs ARGS ((SztProtIE_Cont_UlnkS1cdma2000tunnelingIEs *param, Buffer *mBuf));
EXTERN S16 cmPkSztUlnkS1cdma2000tunneling ARGS ((SztUlnkS1cdma2000tunneling *param, Buffer *mBuf));
EXTERN S16 cmPkSzt_ValueUECapbltyInfoIndIEsCls ARGS ((Szt_ValueUECapbltyInfoIndIEsCls *param, U32 val, Buffer *mBuf));
EXTERN S16 cmPkSztProtIE_Field_UECapbltyInfoIndIEs ARGS ((SztProtIE_Field_UECapbltyInfoIndIEs *param, Buffer *mBuf));
EXTERN S16 cmPkSztProtIE_Cont_UECapbltyInfoIndIEs ARGS ((SztProtIE_Cont_UECapbltyInfoIndIEs *param, Buffer *mBuf));
EXTERN S16 cmPkSztUECapbltyInfoInd ARGS ((SztUECapbltyInfoInd *param, Buffer *mBuf));
EXTERN S16 cmPkSzt_ValueENBStatusTfrIEsCls ARGS ((Szt_ValueENBStatusTfrIEsCls *param, U32 val, Buffer *mBuf));
EXTERN S16 cmPkSztProtIE_Field_ENBStatusTfrIEs ARGS ((SztProtIE_Field_ENBStatusTfrIEs *param, Buffer *mBuf));
EXTERN S16 cmPkSztProtIE_Cont_ENBStatusTfrIEs ARGS ((SztProtIE_Cont_ENBStatusTfrIEs *param, Buffer *mBuf));
EXTERN S16 cmPkSztENBStatusTfr ARGS ((SztENBStatusTfr *param, Buffer *mBuf));
EXTERN S16 cmPkSzt_ValueMMEStatusTfrIEsCls ARGS ((Szt_ValueMMEStatusTfrIEsCls *param, U32 val, Buffer *mBuf));
EXTERN S16 cmPkSztProtIE_Field_MMEStatusTfrIEs ARGS ((SztProtIE_Field_MMEStatusTfrIEs *param, Buffer *mBuf));
EXTERN S16 cmPkSztProtIE_Cont_MMEStatusTfrIEs ARGS ((SztProtIE_Cont_MMEStatusTfrIEs *param, Buffer *mBuf));
EXTERN S16 cmPkSztMMEStatusTfr ARGS ((SztMMEStatusTfr *param, Buffer *mBuf));
EXTERN S16 cmPkSzt_ValueTraceStartIEsCls ARGS ((Szt_ValueTraceStartIEsCls *param, U32 val, Buffer *mBuf));
EXTERN S16 cmPkSztProtIE_Field_TraceStartIEs ARGS ((SztProtIE_Field_TraceStartIEs *param, Buffer *mBuf));
EXTERN S16 cmPkSztProtIE_Cont_TraceStartIEs ARGS ((SztProtIE_Cont_TraceStartIEs *param, Buffer *mBuf));
EXTERN S16 cmPkSztTraceStart ARGS ((SztTraceStart *param, Buffer *mBuf));
EXTERN S16 cmPkSzt_ValueTraceFailIndIEsCls ARGS ((Szt_ValueTraceFailIndIEsCls *param, U32 val, Buffer *mBuf));
EXTERN S16 cmPkSztProtIE_Field_TraceFailIndIEs ARGS ((SztProtIE_Field_TraceFailIndIEs *param, Buffer *mBuf));
EXTERN S16 cmPkSztProtIE_Cont_TraceFailIndIEs ARGS ((SztProtIE_Cont_TraceFailIndIEs *param, Buffer *mBuf));
EXTERN S16 cmPkSztTraceFailInd ARGS ((SztTraceFailInd *param, Buffer *mBuf));
EXTERN S16 cmPkSzt_ValueDeactvTraceIEsCls ARGS ((Szt_ValueDeactvTraceIEsCls *param, U32 val, Buffer *mBuf));
EXTERN S16 cmPkSztProtIE_Field_DeactvTraceIEs ARGS ((SztProtIE_Field_DeactvTraceIEs *param, Buffer *mBuf));
EXTERN S16 cmPkSztProtIE_Cont_DeactvTraceIEs ARGS ((SztProtIE_Cont_DeactvTraceIEs *param, Buffer *mBuf));
EXTERN S16 cmPkSztDeactvTrace ARGS ((SztDeactvTrace *param, Buffer *mBuf));
EXTERN S16 cmPkSzt_ValueCellTrafficTraceIEsCls ARGS ((Szt_ValueCellTrafficTraceIEsCls *param, U32 val, Buffer *mBuf));
EXTERN S16 cmPkSztProtIE_Field_CellTrafficTraceIEs ARGS ((SztProtIE_Field_CellTrafficTraceIEs *param, Buffer *mBuf));
EXTERN S16 cmPkSztProtIE_Cont_CellTrafficTraceIEs ARGS ((SztProtIE_Cont_CellTrafficTraceIEs *param, Buffer *mBuf));
EXTERN S16 cmPkSztCellTrafficTrace ARGS ((SztCellTrafficTrace *param, Buffer *mBuf));
EXTERN S16 cmPkSzt_ValueLocRprtngCntrlIEsCls ARGS ((Szt_ValueLocRprtngCntrlIEsCls *param, U32 val, Buffer *mBuf));
EXTERN S16 cmPkSztProtIE_Field_LocRprtngCntrlIEs ARGS ((SztProtIE_Field_LocRprtngCntrlIEs *param, Buffer *mBuf));
EXTERN S16 cmPkSztProtIE_Cont_LocRprtngCntrlIEs ARGS ((SztProtIE_Cont_LocRprtngCntrlIEs *param, Buffer *mBuf));
EXTERN S16 cmPkSztLocRprtngCntrl ARGS ((SztLocRprtngCntrl *param, Buffer *mBuf));
EXTERN S16 cmPkSzt_ValueLocRprtngFailIndIEsCls ARGS ((Szt_ValueLocRprtngFailIndIEsCls *param, U32 val, Buffer *mBuf));
EXTERN S16 cmPkSztProtIE_Field_LocRprtngFailIndIEs ARGS ((SztProtIE_Field_LocRprtngFailIndIEs *param, Buffer *mBuf));
EXTERN S16 cmPkSztProtIE_Cont_LocRprtngFailIndIEs ARGS ((SztProtIE_Cont_LocRprtngFailIndIEs *param, Buffer *mBuf));
EXTERN S16 cmPkSztLocRprtngFailInd ARGS ((SztLocRprtngFailInd *param, Buffer *mBuf));
EXTERN S16 cmPkSzt_ValueLocReportIEsCls ARGS ((Szt_ValueLocReportIEsCls *param, U32 val, Buffer *mBuf));
EXTERN S16 cmPkSztProtIE_Field_LocReportIEs ARGS ((SztProtIE_Field_LocReportIEs *param, Buffer *mBuf));
EXTERN S16 cmPkSztProtIE_Cont_LocReportIEs ARGS ((SztProtIE_Cont_LocReportIEs *param, Buffer *mBuf));
EXTERN S16 cmPkSztLocReport ARGS ((SztLocReport *param, Buffer *mBuf));
EXTERN S16 cmPkSzt_ValueOverloadStartIEsCls ARGS ((Szt_ValueOverloadStartIEsCls *param, U32 val, Buffer *mBuf));
EXTERN S16 cmPkSztProtIE_Field_OverloadStartIEs ARGS ((SztProtIE_Field_OverloadStartIEs *param, Buffer *mBuf));
EXTERN S16 cmPkSztProtIE_Cont_OverloadStartIEs ARGS ((SztProtIE_Cont_OverloadStartIEs *param, Buffer *mBuf));
EXTERN S16 cmPkSztOverloadStart ARGS ((SztOverloadStart *param, Buffer *mBuf));
EXTERN S16 cmPkSzt_ValueOverloadStopIEsCls ARGS ((Szt_ValueOverloadStopIEsCls *param, U32 val, Buffer *mBuf));
EXTERN S16 cmPkSztProtIE_Field_OverloadStopIEs ARGS ((SztProtIE_Field_OverloadStopIEs *param, Buffer *mBuf));
EXTERN S16 cmPkSztProtIE_Cont_OverloadStopIEs ARGS ((SztProtIE_Cont_OverloadStopIEs *param, Buffer *mBuf));
EXTERN S16 cmPkSztOverloadStop ARGS ((SztOverloadStop *param, Buffer *mBuf));
EXTERN S16 cmPkSzt_ValueWriteReplaceWarningRqstIEsCls ARGS ((Szt_ValueWriteReplaceWarningRqstIEsCls *param, U32 val, Buffer *mBuf));
EXTERN S16 cmPkSztProtIE_Field_WriteReplaceWarningRqstIEs ARGS ((SztProtIE_Field_WriteReplaceWarningRqstIEs *param, Buffer *mBuf));
EXTERN S16 cmPkSztProtIE_Cont_WriteReplaceWarningRqstIEs ARGS ((SztProtIE_Cont_WriteReplaceWarningRqstIEs *param, Buffer *mBuf));
EXTERN S16 cmPkSztWriteReplaceWarningRqst ARGS ((SztWriteReplaceWarningRqst *param, Buffer *mBuf));
EXTERN S16 cmPkSzt_ValueWriteReplaceWarningRespIEsCls ARGS ((Szt_ValueWriteReplaceWarningRespIEsCls *param, U32 val, Buffer *mBuf));
EXTERN S16 cmPkSztProtIE_Field_WriteReplaceWarningRespIEs ARGS ((SztProtIE_Field_WriteReplaceWarningRespIEs *param, Buffer *mBuf));
EXTERN S16 cmPkSztProtIE_Cont_WriteReplaceWarningRespIEs ARGS ((SztProtIE_Cont_WriteReplaceWarningRespIEs *param, Buffer *mBuf));
EXTERN S16 cmPkSztWriteReplaceWarningResp ARGS ((SztWriteReplaceWarningResp *param, Buffer *mBuf));
EXTERN S16 cmPkSztInter_SystemInformTfrTyp ARGS ((SztInter_SystemInformTfrTyp *param, Buffer *mBuf));
EXTERN S16 cmPkSzt_ValueENBDirectInformTfrIEsCls ARGS ((Szt_ValueENBDirectInformTfrIEsCls *param, U32 val, Buffer *mBuf));
EXTERN S16 cmPkSztProtIE_Field_ENBDirectInformTfrIEs ARGS ((SztProtIE_Field_ENBDirectInformTfrIEs *param, Buffer *mBuf));
EXTERN S16 cmPkSztProtIE_Cont_ENBDirectInformTfrIEs ARGS ((SztProtIE_Cont_ENBDirectInformTfrIEs *param, Buffer *mBuf));
EXTERN S16 cmPkSztENBDirectInformTfr ARGS ((SztENBDirectInformTfr *param, Buffer *mBuf));
EXTERN S16 cmPkSzt_ValueMMEDirectInformTfrIEsCls ARGS ((Szt_ValueMMEDirectInformTfrIEsCls *param, U32 val, Buffer *mBuf));
EXTERN S16 cmPkSztProtIE_Field_MMEDirectInformTfrIEs ARGS ((SztProtIE_Field_MMEDirectInformTfrIEs *param, Buffer *mBuf));
EXTERN S16 cmPkSztProtIE_Cont_MMEDirectInformTfrIEs ARGS ((SztProtIE_Cont_MMEDirectInformTfrIEs *param, Buffer *mBuf));
EXTERN S16 cmPkSztMMEDirectInformTfr ARGS ((SztMMEDirectInformTfr *param, Buffer *mBuf));
EXTERN S16 cmPkSzt_ValueENBConfigTfrIEsCls ARGS ((Szt_ValueENBConfigTfrIEsCls *param, U32 val, Buffer *mBuf));
EXTERN S16 cmPkSztProtIE_Field_ENBConfigTfrIEs ARGS ((SztProtIE_Field_ENBConfigTfrIEs *param, Buffer *mBuf));
EXTERN S16 cmPkSztProtIE_Cont_ENBConfigTfrIEs ARGS ((SztProtIE_Cont_ENBConfigTfrIEs *param, Buffer *mBuf));
EXTERN S16 cmPkSztENBConfigTfr ARGS ((SztENBConfigTfr *param, Buffer *mBuf));
EXTERN S16 cmPkSzt_ValueMMEConfigTfrIEsCls ARGS ((Szt_ValueMMEConfigTfrIEsCls *param, U32 val, Buffer *mBuf));
EXTERN S16 cmPkSztProtIE_Field_MMEConfigTfrIEs ARGS ((SztProtIE_Field_MMEConfigTfrIEs *param, Buffer *mBuf));
EXTERN S16 cmPkSztProtIE_Cont_MMEConfigTfrIEs ARGS ((SztProtIE_Cont_MMEConfigTfrIEs *param, Buffer *mBuf));
EXTERN S16 cmPkSztMMEConfigTfr ARGS ((SztMMEConfigTfr *param, Buffer *mBuf));
EXTERN S16 cmPkSzt_ValuePrivMsgIEsCls ARGS ((Szt_ValuePrivMsgIEsCls *param, U32 val, Buffer *mBuf));
EXTERN S16 cmPkSztPrivIE_Field_PrivMsgIEs ARGS ((SztPrivIE_Field_PrivMsgIEs *param, Buffer *mBuf));
EXTERN S16 cmPkSztPrivIE_Cont_PrivMsgIEs ARGS ((SztPrivIE_Cont_PrivMsgIEs *param, Buffer *mBuf));
EXTERN S16 cmPkSztPrivMsg ARGS ((SztPrivMsg *param, Buffer *mBuf));
EXTERN S16 cmPkSzt_ValueKillRqstIEsCls ARGS ((Szt_ValueKillRqstIEsCls *param, U32 val, Buffer *mBuf));
EXTERN S16 cmPkSztProtIE_Field_KillRqstIEs ARGS ((SztProtIE_Field_KillRqstIEs *param, Buffer *mBuf));
EXTERN S16 cmPkSztProtIE_Cont_KillRqstIEs ARGS ((SztProtIE_Cont_KillRqstIEs *param, Buffer *mBuf));
EXTERN S16 cmPkSztKillRqst ARGS ((SztKillRqst *param, Buffer *mBuf));
EXTERN S16 cmPkSzt_ValueKillRespIEsCls ARGS ((Szt_ValueKillRespIEsCls *param, U32 val, Buffer *mBuf));
EXTERN S16 cmPkSztProtIE_Field_KillRespIEs ARGS ((SztProtIE_Field_KillRespIEs *param, Buffer *mBuf));
EXTERN S16 cmPkSztProtIE_Cont_KillRespIEs ARGS ((SztProtIE_Cont_KillRespIEs *param, Buffer *mBuf));
EXTERN S16 cmPkSztKillResp ARGS ((SztKillResp *param, Buffer *mBuf));
EXTERN S16 cmPkSzt_ValueDlnkUEAssociatedLPPaTport_IEsCls ARGS ((Szt_ValueDlnkUEAssociatedLPPaTport_IEsCls *param, U32 val, Buffer *mBuf));
EXTERN S16 cmPkSztProtIE_Field_DlnkUEAssociatedLPPaTport_IEs ARGS ((SztProtIE_Field_DlnkUEAssociatedLPPaTport_IEs *param, Buffer *mBuf));
EXTERN S16 cmPkSztProtIE_Cont_DlnkUEAssociatedLPPaTport_IEs ARGS ((SztProtIE_Cont_DlnkUEAssociatedLPPaTport_IEs *param, Buffer *mBuf));
EXTERN S16 cmPkSztDlnkUEAssociatedLPPaTport ARGS ((SztDlnkUEAssociatedLPPaTport *param, Buffer *mBuf));
EXTERN S16 cmPkSzt_ValueUlnkUEAssociatedLPPaTport_IEsCls ARGS ((Szt_ValueUlnkUEAssociatedLPPaTport_IEsCls *param, U32 val, Buffer *mBuf));
EXTERN S16 cmPkSztProtIE_Field_UlnkUEAssociatedLPPaTport_IEs ARGS ((SztProtIE_Field_UlnkUEAssociatedLPPaTport_IEs *param, Buffer *mBuf));
EXTERN S16 cmPkSztProtIE_Cont_UlnkUEAssociatedLPPaTport_IEs ARGS ((SztProtIE_Cont_UlnkUEAssociatedLPPaTport_IEs *param, Buffer *mBuf));
EXTERN S16 cmPkSztUlnkUEAssociatedLPPaTport ARGS ((SztUlnkUEAssociatedLPPaTport *param, Buffer *mBuf));
EXTERN S16 cmPkSzt_ValueDlnkNonUEAssociatedLPPaTport_IEsCls ARGS ((Szt_ValueDlnkNonUEAssociatedLPPaTport_IEsCls *param, U32 val, Buffer *mBuf));
EXTERN S16 cmPkSztProtIE_Field_DlnkNonUEAssociatedLPPaTport_IEs ARGS ((SztProtIE_Field_DlnkNonUEAssociatedLPPaTport_IEs *param, Buffer *mBuf));
EXTERN S16 cmPkSztProtIE_Cont_DlnkNonUEAssociatedLPPaTport_IEs ARGS ((SztProtIE_Cont_DlnkNonUEAssociatedLPPaTport_IEs *param, Buffer *mBuf));
EXTERN S16 cmPkSztDlnkNonUEAssociatedLPPaTport ARGS ((SztDlnkNonUEAssociatedLPPaTport *param, Buffer *mBuf));
EXTERN S16 cmPkSzt_ValueUlnkNonUEAssociatedLPPaTport_IEsCls ARGS ((Szt_ValueUlnkNonUEAssociatedLPPaTport_IEsCls *param, U32 val, Buffer *mBuf));
EXTERN S16 cmPkSztProtIE_Field_UlnkNonUEAssociatedLPPaTport_IEs ARGS ((SztProtIE_Field_UlnkNonUEAssociatedLPPaTport_IEs *param, Buffer *mBuf));
EXTERN S16 cmPkSztProtIE_Cont_UlnkNonUEAssociatedLPPaTport_IEs ARGS ((SztProtIE_Cont_UlnkNonUEAssociatedLPPaTport_IEs *param, Buffer *mBuf));
EXTERN S16 cmPkSztUlnkNonUEAssociatedLPPaTport ARGS ((SztUlnkNonUEAssociatedLPPaTport *param, Buffer *mBuf));
EXTERN S16 cmPkSzt_InitiatingMsgCls ARGS ((Szt_InitiatingMsgCls *param, U32 val, Buffer *mBuf));
EXTERN S16 cmPkSztInitiatingMsg ARGS ((SztInitiatingMsg *param, Buffer *mBuf));
EXTERN S16 cmPkSzt_SuccessfulOutcomeCls ARGS ((Szt_SuccessfulOutcomeCls *param, U32 val, Buffer *mBuf));
EXTERN S16 cmPkSztSuccessfulOutcome ARGS ((SztSuccessfulOutcome *param, Buffer *mBuf));
EXTERN S16 cmPkSzt_UnsuccessfulOutcomeCls ARGS ((Szt_UnsuccessfulOutcomeCls *param, U32 val, Buffer *mBuf));
EXTERN S16 cmPkSztUnsuccessfulOutcome ARGS ((SztUnsuccessfulOutcome *param, Buffer *mBuf));
EXTERN S16 cmPkSztS1AP_PDU ARGS ((SztS1AP_PDU *param, Buffer *mBuf));
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
$SID$        ---      chebli         1. Updated DB for S1AP R11 for TotaleNodeB GA3.0
*********************************************************************91*/
