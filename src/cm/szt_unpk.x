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

#ifndef __SZTUNPKX__
#define __SZTUNPKX__ 


EXTERN S16 cmUnpkSztPrivIE_ID ARGS ((SztPrivIE_ID *param, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSzt_ExtnEUTRAN_CGI_ExtIEsCls ARGS ((Szt_ExtnEUTRAN_CGI_ExtIEsCls *param, U32 val, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSztProtExtnField_EUTRAN_CGI_ExtIEs ARGS ((SztProtExtnField_EUTRAN_CGI_ExtIEs *param, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSztProtExtnCont_EUTRAN_CGI_ExtIEs ARGS ((SztProtExtnCont_EUTRAN_CGI_ExtIEs *param, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSztEUTRAN_CGI ARGS ((SztEUTRAN_CGI *param, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSztCellIdLstforMDT ARGS ((SztCellIdLstforMDT *param, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSzt_ExtnCellBasedMDT_ExtIEsCls ARGS ((Szt_ExtnCellBasedMDT_ExtIEsCls *param, U32 val, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSztProtExtnField_CellBasedMDT_ExtIEs ARGS ((SztProtExtnField_CellBasedMDT_ExtIEs *param, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSztProtExtnCont_CellBasedMDT_ExtIEs ARGS ((SztProtExtnCont_CellBasedMDT_ExtIEs *param, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSztCellBasedMDT ARGS ((SztCellBasedMDT *param, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSztTALstforMDT ARGS ((SztTALstforMDT *param, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSzt_ExtnTABasedMDT_ExtIEsCls ARGS ((Szt_ExtnTABasedMDT_ExtIEsCls *param, U32 val, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSztProtExtnField_TABasedMDT_ExtIEs ARGS ((SztProtExtnField_TABasedMDT_ExtIEs *param, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSztProtExtnCont_TABasedMDT_ExtIEs ARGS ((SztProtExtnCont_TABasedMDT_ExtIEs *param, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSztTABasedMDT ARGS ((SztTABasedMDT *param, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSzt_ExtnTAI_ExtIEsCls ARGS ((Szt_ExtnTAI_ExtIEsCls *param, U32 val, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSztProtExtnField_TAI_ExtIEs ARGS ((SztProtExtnField_TAI_ExtIEs *param, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSztProtExtnCont_TAI_ExtIEs ARGS ((SztProtExtnCont_TAI_ExtIEs *param, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSztTAI ARGS ((SztTAI *param, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSztTAILstforMDT ARGS ((SztTAILstforMDT *param, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSzt_ExtnTAIBasedMDT_ExtIEsCls ARGS ((Szt_ExtnTAIBasedMDT_ExtIEsCls *param, U32 val, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSztProtExtnField_TAIBasedMDT_ExtIEs ARGS ((SztProtExtnField_TAIBasedMDT_ExtIEs *param, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSztProtExtnCont_TAIBasedMDT_ExtIEs ARGS ((SztProtExtnCont_TAIBasedMDT_ExtIEs *param, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSztTAIBasedMDT ARGS ((SztTAIBasedMDT *param, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSztAreaScopeOfMDT ARGS ((SztAreaScopeOfMDT *param, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSzt_ExtnAllocnAndRetentionPriority_ExtIEsCls ARGS ((Szt_ExtnAllocnAndRetentionPriority_ExtIEsCls *param, U32 val, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSztProtExtnField_AllocnAndRetentionPriority_ExtIEs ARGS ((SztProtExtnField_AllocnAndRetentionPriority_ExtIEs *param, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSztProtExtnCont_AllocnAndRetentionPriority_ExtIEs ARGS ((SztProtExtnCont_AllocnAndRetentionPriority_ExtIEs *param, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSztAllocnAndRetentionPriority ARGS ((SztAllocnAndRetentionPriority *param, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSzt_ExtnCOUNTvalue_ExtIEsCls ARGS ((Szt_ExtnCOUNTvalue_ExtIEsCls *param, U32 val, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSztProtExtnField_COUNTvalue_ExtIEs ARGS ((SztProtExtnField_COUNTvalue_ExtIEs *param, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSztProtExtnCont_COUNTvalue_ExtIEs ARGS ((SztProtExtnCont_COUNTvalue_ExtIEs *param, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSztCOUNTvalue ARGS ((SztCOUNTvalue *param, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSzt_ExtnCOUNTValueExtended_ExtIEsCls ARGS ((Szt_ExtnCOUNTValueExtended_ExtIEsCls *param, U32 val, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSztProtExtnField_COUNTValueExtended_ExtIEs ARGS ((SztProtExtnField_COUNTValueExtended_ExtIEs *param, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSztProtExtnCont_COUNTValueExtended_ExtIEs ARGS ((SztProtExtnCont_COUNTValueExtended_ExtIEs *param, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSztCOUNTValueExtended ARGS ((SztCOUNTValueExtended *param, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSzt_ExtnBrs_SubjToStatusTfr_ItemExtIEsCls ARGS ((Szt_ExtnBrs_SubjToStatusTfr_ItemExtIEsCls *param, U32 val, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSztProtExtnField_Brs_SubjToStatusTfr_ItemExtIEs ARGS ((SztProtExtnField_Brs_SubjToStatusTfr_ItemExtIEs *param, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSztProtExtnCont_Brs_SubjToStatusTfr_ItemExtIEs ARGS ((SztProtExtnCont_Brs_SubjToStatusTfr_ItemExtIEs *param, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSztBrs_SubjToStatusTfr_Item ARGS ((SztBrs_SubjToStatusTfr_Item *param, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSzt_ValueBrs_SubjToStatusTfr_ItemIEsCls ARGS ((Szt_ValueBrs_SubjToStatusTfr_ItemIEsCls *param, U32 val, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSztProtIE_Field_Brs_SubjToStatusTfr_ItemIEs ARGS ((SztProtIE_Field_Brs_SubjToStatusTfr_ItemIEs *param, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSztBrs_SubjToStatusTfrLst ARGS ((SztBrs_SubjToStatusTfrLst *param, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSztBPLMNs ARGS ((SztBPLMNs *param, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSzt_ExtnCellID_Cancelled_Item_ExtIEsCls ARGS ((Szt_ExtnCellID_Cancelled_Item_ExtIEsCls *param, U32 val, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSztProtExtnField_CellID_Cancelled_Item_ExtIEs ARGS ((SztProtExtnField_CellID_Cancelled_Item_ExtIEs *param, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSztProtExtnCont_CellID_Cancelled_Item_ExtIEs ARGS ((SztProtExtnCont_CellID_Cancelled_Item_ExtIEs *param, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSztCellID_Cancelled_Item ARGS ((SztCellID_Cancelled_Item *param, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSztCellID_Cancelled ARGS ((SztCellID_Cancelled *param, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSzt_ExtnCancelledCellinTAI_Item_ExtIEsCls ARGS ((Szt_ExtnCancelledCellinTAI_Item_ExtIEsCls *param, U32 val, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSztProtExtnField_CancelledCellinTAI_Item_ExtIEs ARGS ((SztProtExtnField_CancelledCellinTAI_Item_ExtIEs *param, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSztProtExtnCont_CancelledCellinTAI_Item_ExtIEs ARGS ((SztProtExtnCont_CancelledCellinTAI_Item_ExtIEs *param, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSztCancelledCellinTAI_Item ARGS ((SztCancelledCellinTAI_Item *param, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSztCancelledCellinTAI ARGS ((SztCancelledCellinTAI *param, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSzt_ExtnTAI_Cancelled_Item_ExtIEsCls ARGS ((Szt_ExtnTAI_Cancelled_Item_ExtIEsCls *param, U32 val, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSztProtExtnField_TAI_Cancelled_Item_ExtIEs ARGS ((SztProtExtnField_TAI_Cancelled_Item_ExtIEs *param, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSztProtExtnCont_TAI_Cancelled_Item_ExtIEs ARGS ((SztProtExtnCont_TAI_Cancelled_Item_ExtIEs *param, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSztTAI_Cancelled_Item ARGS ((SztTAI_Cancelled_Item *param, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSztTAI_Cancelled ARGS ((SztTAI_Cancelled *param, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSzt_ExtnCancelledCellinEAI_Item_ExtIEsCls ARGS ((Szt_ExtnCancelledCellinEAI_Item_ExtIEsCls *param, U32 val, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSztProtExtnField_CancelledCellinEAI_Item_ExtIEs ARGS ((SztProtExtnField_CancelledCellinEAI_Item_ExtIEs *param, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSztProtExtnCont_CancelledCellinEAI_Item_ExtIEs ARGS ((SztProtExtnCont_CancelledCellinEAI_Item_ExtIEs *param, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSztCancelledCellinEAI_Item ARGS ((SztCancelledCellinEAI_Item *param, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSztCancelledCellinEAI ARGS ((SztCancelledCellinEAI *param, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSzt_ExtnEmergencyAreaID_Cancelled_Item_ExtIEsCls ARGS ((Szt_ExtnEmergencyAreaID_Cancelled_Item_ExtIEsCls *param, U32 val, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSztProtExtnField_EmergencyAreaID_Cancelled_Item_ExtIEs ARGS ((SztProtExtnField_EmergencyAreaID_Cancelled_Item_ExtIEs *param, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSztProtExtnCont_EmergencyAreaID_Cancelled_Item_ExtIEs ARGS ((SztProtExtnCont_EmergencyAreaID_Cancelled_Item_ExtIEs *param, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSztEmergencyAreaID_Cancelled_Item ARGS ((SztEmergencyAreaID_Cancelled_Item *param, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSztEmergencyAreaID_Cancelled ARGS ((SztEmergencyAreaID_Cancelled *param, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSztBroadcastCancelledAreaLst ARGS ((SztBroadcastCancelledAreaLst *param, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSzt_ExtnCellID_Broadcast_Item_ExtIEsCls ARGS ((Szt_ExtnCellID_Broadcast_Item_ExtIEsCls *param, U32 val, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSztProtExtnField_CellID_Broadcast_Item_ExtIEs ARGS ((SztProtExtnField_CellID_Broadcast_Item_ExtIEs *param, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSztProtExtnCont_CellID_Broadcast_Item_ExtIEs ARGS ((SztProtExtnCont_CellID_Broadcast_Item_ExtIEs *param, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSztCellID_Broadcast_Item ARGS ((SztCellID_Broadcast_Item *param, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSztCellID_Broadcast ARGS ((SztCellID_Broadcast *param, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSzt_ExtnCompletedCellinTAI_Item_ExtIEsCls ARGS ((Szt_ExtnCompletedCellinTAI_Item_ExtIEsCls *param, U32 val, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSztProtExtnField_CompletedCellinTAI_Item_ExtIEs ARGS ((SztProtExtnField_CompletedCellinTAI_Item_ExtIEs *param, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSztProtExtnCont_CompletedCellinTAI_Item_ExtIEs ARGS ((SztProtExtnCont_CompletedCellinTAI_Item_ExtIEs *param, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSztCompletedCellinTAI_Item ARGS ((SztCompletedCellinTAI_Item *param, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSztCompletedCellinTAI ARGS ((SztCompletedCellinTAI *param, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSzt_ExtnTAI_Broadcast_Item_ExtIEsCls ARGS ((Szt_ExtnTAI_Broadcast_Item_ExtIEsCls *param, U32 val, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSztProtExtnField_TAI_Broadcast_Item_ExtIEs ARGS ((SztProtExtnField_TAI_Broadcast_Item_ExtIEs *param, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSztProtExtnCont_TAI_Broadcast_Item_ExtIEs ARGS ((SztProtExtnCont_TAI_Broadcast_Item_ExtIEs *param, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSztTAI_Broadcast_Item ARGS ((SztTAI_Broadcast_Item *param, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSztTAI_Broadcast ARGS ((SztTAI_Broadcast *param, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSzt_ExtnCompletedCellinEAI_Item_ExtIEsCls ARGS ((Szt_ExtnCompletedCellinEAI_Item_ExtIEsCls *param, U32 val, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSztProtExtnField_CompletedCellinEAI_Item_ExtIEs ARGS ((SztProtExtnField_CompletedCellinEAI_Item_ExtIEs *param, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSztProtExtnCont_CompletedCellinEAI_Item_ExtIEs ARGS ((SztProtExtnCont_CompletedCellinEAI_Item_ExtIEs *param, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSztCompletedCellinEAI_Item ARGS ((SztCompletedCellinEAI_Item *param, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSztCompletedCellinEAI ARGS ((SztCompletedCellinEAI *param, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSzt_ExtnEmergencyAreaID_Broadcast_Item_ExtIEsCls ARGS ((Szt_ExtnEmergencyAreaID_Broadcast_Item_ExtIEsCls *param, U32 val, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSztProtExtnField_EmergencyAreaID_Broadcast_Item_ExtIEs ARGS ((SztProtExtnField_EmergencyAreaID_Broadcast_Item_ExtIEs *param, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSztProtExtnCont_EmergencyAreaID_Broadcast_Item_ExtIEs ARGS ((SztProtExtnCont_EmergencyAreaID_Broadcast_Item_ExtIEs *param, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSztEmergencyAreaID_Broadcast_Item ARGS ((SztEmergencyAreaID_Broadcast_Item *param, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSztEmergencyAreaID_Broadcast ARGS ((SztEmergencyAreaID_Broadcast *param, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSztBroadcastCompletedAreaLst ARGS ((SztBroadcastCompletedAreaLst *param, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSztCause ARGS ((SztCause *param, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSzt_ExtnCdma2000OneXSRVCCInfo_ExtIEsCls ARGS ((Szt_ExtnCdma2000OneXSRVCCInfo_ExtIEsCls *param, U32 val, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSztProtExtnField_Cdma2000OneXSRVCCInfo_ExtIEs ARGS ((SztProtExtnField_Cdma2000OneXSRVCCInfo_ExtIEs *param, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSztProtExtnCont_Cdma2000OneXSRVCCInfo_ExtIEs ARGS ((SztProtExtnCont_Cdma2000OneXSRVCCInfo_ExtIEs *param, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSztCdma2000OneXSRVCCInfo ARGS ((SztCdma2000OneXSRVCCInfo *param, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSzt_ExtnCellTyp_ExtIEsCls ARGS ((Szt_ExtnCellTyp_ExtIEsCls *param, U32 val, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSztProtExtnField_CellTyp_ExtIEs ARGS ((SztProtExtnField_CellTyp_ExtIEs *param, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSztProtExtnCont_CellTyp_ExtIEs ARGS ((SztProtExtnCont_CellTyp_ExtIEs *param, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSztCellTyp ARGS ((SztCellTyp *param, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSzt_ExtnCGI_ExtIEsCls ARGS ((Szt_ExtnCGI_ExtIEsCls *param, U32 val, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSztProtExtnField_CGI_ExtIEs ARGS ((SztProtExtnField_CGI_ExtIEs *param, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSztProtExtnCont_CGI_ExtIEs ARGS ((SztProtExtnCont_CGI_ExtIEs *param, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSztCGI ARGS ((SztCGI *param, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSzt_ExtnCSG_IdLst_Item_ExtIEsCls ARGS ((Szt_ExtnCSG_IdLst_Item_ExtIEsCls *param, U32 val, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSztProtExtnField_CSG_IdLst_Item_ExtIEs ARGS ((SztProtExtnField_CSG_IdLst_Item_ExtIEs *param, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSztProtExtnCont_CSG_IdLst_Item_ExtIEs ARGS ((SztProtExtnCont_CSG_IdLst_Item_ExtIEs *param, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSztCSG_IdLst_Item ARGS ((SztCSG_IdLst_Item *param, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSztCSG_IdLst ARGS ((SztCSG_IdLst *param, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSzt_ExtnCriticalityDiag_IE_Item_ExtIEsCls ARGS ((Szt_ExtnCriticalityDiag_IE_Item_ExtIEsCls *param, U32 val, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSztProtExtnField_CriticalityDiag_IE_Item_ExtIEs ARGS ((SztProtExtnField_CriticalityDiag_IE_Item_ExtIEs *param, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSztProtExtnCont_CriticalityDiag_IE_Item_ExtIEs ARGS ((SztProtExtnCont_CriticalityDiag_IE_Item_ExtIEs *param, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSztCriticalityDiag_IE_Item ARGS ((SztCriticalityDiag_IE_Item *param, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSztCriticalityDiag_IE_Lst ARGS ((SztCriticalityDiag_IE_Lst *param, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSzt_ExtnCriticalityDiag_ExtIEsCls ARGS ((Szt_ExtnCriticalityDiag_ExtIEsCls *param, U32 val, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSztProtExtnField_CriticalityDiag_ExtIEs ARGS ((SztProtExtnField_CriticalityDiag_ExtIEs *param, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSztProtExtnCont_CriticalityDiag_ExtIEs ARGS ((SztProtExtnCont_CriticalityDiag_ExtIEs *param, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSztCriticalityDiag ARGS ((SztCriticalityDiag *param, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSztECGILst ARGS ((SztECGILst *param, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSztEmergencyAreaIDLst ARGS ((SztEmergencyAreaIDLst *param, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSztENB_ID ARGS ((SztENB_ID *param, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSzt_ExtnLAI_ExtIEsCls ARGS ((Szt_ExtnLAI_ExtIEsCls *param, U32 val, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSztProtExtnField_LAI_ExtIEs ARGS ((SztProtExtnField_LAI_ExtIEs *param, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSztProtExtnCont_LAI_ExtIEs ARGS ((SztProtExtnCont_LAI_ExtIEs *param, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSztLAI ARGS ((SztLAI *param, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSzt_ExtnGERAN_Cell_ID_ExtIEsCls ARGS ((Szt_ExtnGERAN_Cell_ID_ExtIEsCls *param, U32 val, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSztProtExtnField_GERAN_Cell_ID_ExtIEs ARGS ((SztProtExtnField_GERAN_Cell_ID_ExtIEs *param, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSztProtExtnCont_GERAN_Cell_ID_ExtIEs ARGS ((SztProtExtnCont_GERAN_Cell_ID_ExtIEs *param, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSztGERAN_Cell_ID ARGS ((SztGERAN_Cell_ID *param, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSzt_ExtnGlobalENB_ID_ExtIEsCls ARGS ((Szt_ExtnGlobalENB_ID_ExtIEsCls *param, U32 val, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSztProtExtnField_GlobalENB_ID_ExtIEs ARGS ((SztProtExtnField_GlobalENB_ID_ExtIEs *param, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSztProtExtnCont_GlobalENB_ID_ExtIEs ARGS ((SztProtExtnCont_GlobalENB_ID_ExtIEs *param, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSztGlobal_ENB_ID ARGS ((SztGlobal_ENB_ID *param, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSzt_ExtnGUMMEI_ExtIEsCls ARGS ((Szt_ExtnGUMMEI_ExtIEsCls *param, U32 val, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSztProtExtnField_GUMMEI_ExtIEs ARGS ((SztProtExtnField_GUMMEI_ExtIEs *param, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSztProtExtnCont_GUMMEI_ExtIEs ARGS ((SztProtExtnCont_GUMMEI_ExtIEs *param, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSztGUMMEI ARGS ((SztGUMMEI *param, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSztGUMMEILst ARGS ((SztGUMMEILst *param, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSzt_ExtnENB_StatusTfr_TprntCont_ExtIEsCls ARGS ((Szt_ExtnENB_StatusTfr_TprntCont_ExtIEsCls *param, U32 val, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSztProtExtnField_ENB_StatusTfr_TprntCont_ExtIEs ARGS ((SztProtExtnField_ENB_StatusTfr_TprntCont_ExtIEs *param, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSztProtExtnCont_ENB_StatusTfr_TprntCont_ExtIEs ARGS ((SztProtExtnCont_ENB_StatusTfr_TprntCont_ExtIEs *param, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSztENB_StatusTfr_TprntCont ARGS ((SztENB_StatusTfr_TprntCont *param, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSztENBX2TLAs ARGS ((SztENBX2TLAs *param, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSztEPLMNs ARGS ((SztEPLMNs *param, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSzt_ExtnE_RABInformLstItem_ExtIEsCls ARGS ((Szt_ExtnE_RABInformLstItem_ExtIEsCls *param, U32 val, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSztProtExtnField_E_RABInformLstItem_ExtIEs ARGS ((SztProtExtnField_E_RABInformLstItem_ExtIEs *param, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSztProtExtnCont_E_RABInformLstItem_ExtIEs ARGS ((SztProtExtnCont_E_RABInformLstItem_ExtIEs *param, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSztE_RABInformLstItem ARGS ((SztE_RABInformLstItem *param, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSzt_ValueE_RABInformLstIEsCls ARGS ((Szt_ValueE_RABInformLstIEsCls *param, U32 val, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSztProtIE_Field_E_RABInformLstIEs ARGS ((SztProtIE_Field_E_RABInformLstIEs *param, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSztE_RABInformLst ARGS ((SztE_RABInformLst *param, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSzt_ExtnE_RABItem_ExtIEsCls ARGS ((Szt_ExtnE_RABItem_ExtIEsCls *param, U32 val, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSztProtExtnField_E_RABItem_ExtIEs ARGS ((SztProtExtnField_E_RABItem_ExtIEs *param, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSztProtExtnCont_E_RABItem_ExtIEs ARGS ((SztProtExtnCont_E_RABItem_ExtIEs *param, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSztE_RABItem ARGS ((SztE_RABItem *param, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSzt_ValueE_RABItemIEsCls ARGS ((Szt_ValueE_RABItemIEsCls *param, U32 val, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSztProtIE_Field_E_RABItemIEs ARGS ((SztProtIE_Field_E_RABItemIEs *param, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSztE_RABLst ARGS ((SztE_RABLst *param, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSzt_ExtnGBR_QosInform_ExtIEsCls ARGS ((Szt_ExtnGBR_QosInform_ExtIEsCls *param, U32 val, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSztProtExtnField_GBR_QosInform_ExtIEs ARGS ((SztProtExtnField_GBR_QosInform_ExtIEs *param, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSztProtExtnCont_GBR_QosInform_ExtIEs ARGS ((SztProtExtnCont_GBR_QosInform_ExtIEs *param, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSztGBR_QosInform ARGS ((SztGBR_QosInform *param, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSzt_ExtnE_RABQoSParams_ExtIEsCls ARGS ((Szt_ExtnE_RABQoSParams_ExtIEsCls *param, U32 val, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSztProtExtnField_E_RABQoSParams_ExtIEs ARGS ((SztProtExtnField_E_RABQoSParams_ExtIEs *param, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSztProtExtnCont_E_RABQoSParams_ExtIEs ARGS ((SztProtExtnCont_E_RABQoSParams_ExtIEs *param, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSztE_RABLvlQoSParams ARGS ((SztE_RABLvlQoSParams *param, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSztForbiddenTACs ARGS ((SztForbiddenTACs *param, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSzt_ExtnForbiddenTAs_Item_ExtIEsCls ARGS ((Szt_ExtnForbiddenTAs_Item_ExtIEsCls *param, U32 val, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSztProtExtnField_ForbiddenTAs_Item_ExtIEs ARGS ((SztProtExtnField_ForbiddenTAs_Item_ExtIEs *param, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSztProtExtnCont_ForbiddenTAs_Item_ExtIEs ARGS ((SztProtExtnCont_ForbiddenTAs_Item_ExtIEs *param, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSztForbiddenTAs_Item ARGS ((SztForbiddenTAs_Item *param, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSztForbiddenTAs ARGS ((SztForbiddenTAs *param, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSztForbiddenLACs ARGS ((SztForbiddenLACs *param, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSzt_ExtnForbiddenLAs_Item_ExtIEsCls ARGS ((Szt_ExtnForbiddenLAs_Item_ExtIEsCls *param, U32 val, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSztProtExtnField_ForbiddenLAs_Item_ExtIEs ARGS ((SztProtExtnField_ForbiddenLAs_Item_ExtIEs *param, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSztProtExtnCont_ForbiddenLAs_Item_ExtIEs ARGS ((SztProtExtnCont_ForbiddenLAs_Item_ExtIEs *param, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSztForbiddenLAs_Item ARGS ((SztForbiddenLAs_Item *param, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSztForbiddenLAs ARGS ((SztForbiddenLAs *param, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSzt_ExtnHovrRestrnLst_ExtIEsCls ARGS ((Szt_ExtnHovrRestrnLst_ExtIEsCls *param, U32 val, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSztProtExtnField_HovrRestrnLst_ExtIEs ARGS ((SztProtExtnField_HovrRestrnLst_ExtIEs *param, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSztProtExtnCont_HovrRestrnLst_ExtIEs ARGS ((SztProtExtnCont_HovrRestrnLst_ExtIEs *param, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSztHovrRestrnLst ARGS ((SztHovrRestrnLst *param, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSztMeasurementThresholdA2 ARGS ((SztMeasurementThresholdA2 *param, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSzt_ExtnM1ThresholdEventA2_ExtIEsCls ARGS ((Szt_ExtnM1ThresholdEventA2_ExtIEsCls *param, U32 val, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSztProtExtnField_M1ThresholdEventA2_ExtIEs ARGS ((SztProtExtnField_M1ThresholdEventA2_ExtIEs *param, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSztProtExtnCont_M1ThresholdEventA2_ExtIEs ARGS ((SztProtExtnCont_M1ThresholdEventA2_ExtIEs *param, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSztM1ThresholdEventA2 ARGS ((SztM1ThresholdEventA2 *param, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSzt_ExtnM1PeriodicRprtng_ExtIEsCls ARGS ((Szt_ExtnM1PeriodicRprtng_ExtIEsCls *param, U32 val, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSztProtExtnField_M1PeriodicRprtng_ExtIEs ARGS ((SztProtExtnField_M1PeriodicRprtng_ExtIEs *param, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSztProtExtnCont_M1PeriodicRprtng_ExtIEs ARGS ((SztProtExtnCont_M1PeriodicRprtng_ExtIEs *param, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSztM1PeriodicRprtng ARGS ((SztM1PeriodicRprtng *param, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSzt_ExtnM3Config_ExtIEsCls ARGS ((Szt_ExtnM3Config_ExtIEsCls *param, U32 val, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSztProtExtnField_M3Config_ExtIEs ARGS ((SztProtExtnField_M3Config_ExtIEs *param, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSztProtExtnCont_M3Config_ExtIEs ARGS ((SztProtExtnCont_M3Config_ExtIEs *param, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSztM3Config ARGS ((SztM3Config *param, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSzt_ExtnM4Config_ExtIEsCls ARGS ((Szt_ExtnM4Config_ExtIEsCls *param, U32 val, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSztProtExtnField_M4Config_ExtIEs ARGS ((SztProtExtnField_M4Config_ExtIEs *param, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSztProtExtnCont_M4Config_ExtIEs ARGS ((SztProtExtnCont_M4Config_ExtIEs *param, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSztM4Config ARGS ((SztM4Config *param, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSzt_ExtnM5Config_ExtIEsCls ARGS ((Szt_ExtnM5Config_ExtIEsCls *param, U32 val, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSztProtExtnField_M5Config_ExtIEs ARGS ((SztProtExtnField_M5Config_ExtIEs *param, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSztProtExtnCont_M5Config_ExtIEs ARGS ((SztProtExtnCont_M5Config_ExtIEs *param, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSztM5Config ARGS ((SztM5Config *param, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSzt_ExtnImmediateMDT_ExtIEsCls ARGS ((Szt_ExtnImmediateMDT_ExtIEsCls *param, U32 val, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSztProtExtnField_ImmediateMDT_ExtIEs ARGS ((SztProtExtnField_ImmediateMDT_ExtIEs *param, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSztProtExtnCont_ImmediateMDT_ExtIEs ARGS ((SztProtExtnCont_ImmediateMDT_ExtIEs *param, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSztImmediateMDT ARGS ((SztImmediateMDT *param, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSzt_ExtnLastVisitedEUTRANCellInform_ExtIEsCls ARGS ((Szt_ExtnLastVisitedEUTRANCellInform_ExtIEsCls *param, U32 val, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSztProtExtnField_LastVisitedEUTRANCellInform_ExtIEs ARGS ((SztProtExtnField_LastVisitedEUTRANCellInform_ExtIEs *param, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSztProtExtnCont_LastVisitedEUTRANCellInform_ExtIEs ARGS ((SztProtExtnCont_LastVisitedEUTRANCellInform_ExtIEs *param, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSztLastVisitedEUTRANCellInform ARGS ((SztLastVisitedEUTRANCellInform *param, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSztLastVisitedGERANCellInform ARGS ((SztLastVisitedGERANCellInform *param, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSztLastVisitedCell_Item ARGS ((SztLastVisitedCell_Item *param, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSzt_ExtnLoggedMDT_ExtIEsCls ARGS ((Szt_ExtnLoggedMDT_ExtIEsCls *param, U32 val, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSztProtExtnField_LoggedMDT_ExtIEs ARGS ((SztProtExtnField_LoggedMDT_ExtIEs *param, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSztProtExtnCont_LoggedMDT_ExtIEs ARGS ((SztProtExtnCont_LoggedMDT_ExtIEs *param, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSztLoggedMDT ARGS ((SztLoggedMDT *param, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSztMDTMode ARGS ((SztMDTMode *param, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSztMDTPLMNLst ARGS ((SztMDTPLMNLst *param, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSzt_ExtnMDT_Config_ExtIEsCls ARGS ((Szt_ExtnMDT_Config_ExtIEsCls *param, U32 val, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSztProtExtnField_MDT_Config_ExtIEs ARGS ((SztProtExtnField_MDT_Config_ExtIEs *param, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSztProtExtnCont_MDT_Config_ExtIEs ARGS ((SztProtExtnCont_MDT_Config_ExtIEs *param, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSztMDT_Config ARGS ((SztMDT_Config *param, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSztOverloadResp ARGS ((SztOverloadResp *param, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSzt_ExtnRqstTyp_ExtIEsCls ARGS ((Szt_ExtnRqstTyp_ExtIEsCls *param, U32 val, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSztProtExtnField_RqstTyp_ExtIEs ARGS ((SztProtExtnField_RqstTyp_ExtIEs *param, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSztProtExtnCont_RqstTyp_ExtIEs ARGS ((SztProtExtnCont_RqstTyp_ExtIEs *param, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSztRqstTyp ARGS ((SztRqstTyp *param, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSzt_ExtnTgetRNC_ID_ExtIEsCls ARGS ((Szt_ExtnTgetRNC_ID_ExtIEsCls *param, U32 val, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSztProtExtnField_TgetRNC_ID_ExtIEs ARGS ((SztProtExtnField_TgetRNC_ID_ExtIEs *param, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSztProtExtnCont_TgetRNC_ID_ExtIEs ARGS ((SztProtExtnCont_TgetRNC_ID_ExtIEs *param, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSztTgetRNC_ID ARGS ((SztTgetRNC_ID *param, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSztRIMRoutingAddr ARGS ((SztRIMRoutingAddr *param, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSzt_ExtnRIMTfr_ExtIEsCls ARGS ((Szt_ExtnRIMTfr_ExtIEsCls *param, U32 val, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSztProtExtnField_RIMTfr_ExtIEs ARGS ((SztProtExtnField_RIMTfr_ExtIEs *param, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSztProtExtnCont_RIMTfr_ExtIEs ARGS ((SztProtExtnCont_RIMTfr_ExtIEs *param, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSztRIMTfr ARGS ((SztRIMTfr *param, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSzt_ExtnSecurCntxt_ExtIEsCls ARGS ((Szt_ExtnSecurCntxt_ExtIEsCls *param, U32 val, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSztProtExtnField_SecurCntxt_ExtIEs ARGS ((SztProtExtnField_SecurCntxt_ExtIEs *param, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSztProtExtnCont_SecurCntxt_ExtIEs ARGS ((SztProtExtnCont_SecurCntxt_ExtIEs *param, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSztSecurCntxt ARGS ((SztSecurCntxt *param, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSztENBX2GTPTLAs ARGS ((SztENBX2GTPTLAs *param, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSzt_ExtnENBX2ExtTLA_ExtIEsCls ARGS ((Szt_ExtnENBX2ExtTLA_ExtIEsCls *param, U32 val, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSztProtExtnField_ENBX2ExtTLA_ExtIEs ARGS ((SztProtExtnField_ENBX2ExtTLA_ExtIEs *param, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSztProtExtnCont_ENBX2ExtTLA_ExtIEs ARGS ((SztProtExtnCont_ENBX2ExtTLA_ExtIEs *param, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSztENBX2ExtTLA ARGS ((SztENBX2ExtTLA *param, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSztENBX2ExtTLAs ARGS ((SztENBX2ExtTLAs *param, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSzt_ExtnX2TNLConfigInfo_ExtIEsCls ARGS ((Szt_ExtnX2TNLConfigInfo_ExtIEsCls *param, U32 val, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSztProtExtnField_X2TNLConfigInfo_ExtIEs ARGS ((SztProtExtnField_X2TNLConfigInfo_ExtIEs *param, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSztProtExtnCont_X2TNLConfigInfo_ExtIEs ARGS ((SztProtExtnCont_X2TNLConfigInfo_ExtIEs *param, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSztX2TNLConfigInfo ARGS ((SztX2TNLConfigInfo *param, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSzt_ExtnTimeSynchronizationInfo_ExtIEsCls ARGS ((Szt_ExtnTimeSynchronizationInfo_ExtIEsCls *param, U32 val, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSztProtExtnField_TimeSynchronizationInfo_ExtIEs ARGS ((SztProtExtnField_TimeSynchronizationInfo_ExtIEs *param, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSztProtExtnCont_TimeSynchronizationInfo_ExtIEs ARGS ((SztProtExtnCont_TimeSynchronizationInfo_ExtIEs *param, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSztTimeSynchronizationInfo ARGS ((SztTimeSynchronizationInfo *param, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSzt_ExtnSONInformReply_ExtIEsCls ARGS ((Szt_ExtnSONInformReply_ExtIEsCls *param, U32 val, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSztProtExtnField_SONInformReply_ExtIEs ARGS ((SztProtExtnField_SONInformReply_ExtIEs *param, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSztProtExtnCont_SONInformReply_ExtIEs ARGS ((SztProtExtnCont_SONInformReply_ExtIEs *param, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSztSONInformReply ARGS ((SztSONInformReply *param, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSztSONInform ARGS ((SztSONInform *param, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSzt_ExtnTgeteNB_ID_ExtIEsCls ARGS ((Szt_ExtnTgeteNB_ID_ExtIEsCls *param, U32 val, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSztProtExtnField_TgeteNB_ID_ExtIEs ARGS ((SztProtExtnField_TgeteNB_ID_ExtIEs *param, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSztProtExtnCont_TgeteNB_ID_ExtIEs ARGS ((SztProtExtnCont_TgeteNB_ID_ExtIEs *param, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSztTgeteNB_ID ARGS ((SztTgeteNB_ID *param, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSzt_ExtnSrceNB_ID_ExtIEsCls ARGS ((Szt_ExtnSrceNB_ID_ExtIEsCls *param, U32 val, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSztProtExtnField_SrceNB_ID_ExtIEs ARGS ((SztProtExtnField_SrceNB_ID_ExtIEs *param, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSztProtExtnCont_SrceNB_ID_ExtIEs ARGS ((SztProtExtnCont_SrceNB_ID_ExtIEs *param, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSztSrceNB_ID ARGS ((SztSrceNB_ID *param, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSzt_ExtnSONConfigTfr_ExtIEsCls ARGS ((Szt_ExtnSONConfigTfr_ExtIEsCls *param, U32 val, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSztProtExtnField_SONConfigTfr_ExtIEs ARGS ((SztProtExtnField_SONConfigTfr_ExtIEs *param, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSztProtExtnCont_SONConfigTfr_ExtIEs ARGS ((SztProtExtnCont_SONConfigTfr_ExtIEs *param, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSztSONConfigTfr ARGS ((SztSONConfigTfr *param, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSztUE_HistoryInform ARGS ((SztUE_HistoryInform *param, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSzt_ExtnSrceNB_ToTgeteNB_TprntCont_ExtIEsCls ARGS ((Szt_ExtnSrceNB_ToTgeteNB_TprntCont_ExtIEsCls *param, U32 val, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSztProtExtnField_SrceNB_ToTgeteNB_TprntCont_ExtIEs ARGS ((SztProtExtnField_SrceNB_ToTgeteNB_TprntCont_ExtIEs *param, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSztProtExtnCont_SrceNB_ToTgeteNB_TprntCont_ExtIEs ARGS ((SztProtExtnCont_SrceNB_ToTgeteNB_TprntCont_ExtIEs *param, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSztSrceNB_ToTgeteNB_TprntCont ARGS ((SztSrceNB_ToTgeteNB_TprntCont *param, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSztSrvdPLMNs ARGS ((SztSrvdPLMNs *param, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSztSrvdGroupIDs ARGS ((SztSrvdGroupIDs *param, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSztSrvdMMECs ARGS ((SztSrvdMMECs *param, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSzt_ExtnSrvdGUMMEIsItem_ExtIEsCls ARGS ((Szt_ExtnSrvdGUMMEIsItem_ExtIEsCls *param, U32 val, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSztProtExtnField_SrvdGUMMEIsItem_ExtIEs ARGS ((SztProtExtnField_SrvdGUMMEIsItem_ExtIEs *param, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSztProtExtnCont_SrvdGUMMEIsItem_ExtIEs ARGS ((SztProtExtnCont_SrvdGUMMEIsItem_ExtIEs *param, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSztSrvdGUMMEIsItem ARGS ((SztSrvdGUMMEIsItem *param, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSztSrvdGUMMEIs ARGS ((SztSrvdGUMMEIs *param, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSzt_ExtnSuppTAs_Item_ExtIEsCls ARGS ((Szt_ExtnSuppTAs_Item_ExtIEsCls *param, U32 val, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSztProtExtnField_SuppTAs_Item_ExtIEs ARGS ((SztProtExtnField_SuppTAs_Item_ExtIEs *param, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSztProtExtnCont_SuppTAs_Item_ExtIEs ARGS ((SztProtExtnCont_SuppTAs_Item_ExtIEs *param, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSztSuppTAs_Item ARGS ((SztSuppTAs_Item *param, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSztSuppTAs ARGS ((SztSuppTAs *param, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSzt_ExtnS_TMSI_ExtIEsCls ARGS ((Szt_ExtnS_TMSI_ExtIEsCls *param, U32 val, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSztProtExtnField_S_TMSI_ExtIEs ARGS ((SztProtExtnField_S_TMSI_ExtIEs *param, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSztProtExtnCont_S_TMSI_ExtIEs ARGS ((SztProtExtnCont_S_TMSI_ExtIEs *param, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSztS_TMSI ARGS ((SztS_TMSI *param, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSztTAILstforWarning ARGS ((SztTAILstforWarning *param, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSztTgetID ARGS ((SztTgetID *param, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSzt_ExtnTgeteNB_ToSrceNB_TprntCont_ExtIEsCls ARGS ((Szt_ExtnTgeteNB_ToSrceNB_TprntCont_ExtIEsCls *param, U32 val, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSztProtExtnField_TgeteNB_ToSrceNB_TprntCont_ExtIEs ARGS ((SztProtExtnField_TgeteNB_ToSrceNB_TprntCont_ExtIEs *param, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSztProtExtnCont_TgeteNB_ToSrceNB_TprntCont_ExtIEs ARGS ((SztProtExtnCont_TgeteNB_ToSrceNB_TprntCont_ExtIEs *param, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSztTgeteNB_ToSrceNB_TprntCont ARGS ((SztTgeteNB_ToSrceNB_TprntCont *param, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSzt_ExtnTraceActvn_ExtIEsCls ARGS ((Szt_ExtnTraceActvn_ExtIEsCls *param, U32 val, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSztProtExtnField_TraceActvn_ExtIEs ARGS ((SztProtExtnField_TraceActvn_ExtIEs *param, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSztProtExtnCont_TraceActvn_ExtIEs ARGS ((SztProtExtnCont_TraceActvn_ExtIEs *param, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSztTraceActvn ARGS ((SztTraceActvn *param, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSzt_ExtnTunnel_Inform_ExtIEsCls ARGS ((Szt_ExtnTunnel_Inform_ExtIEsCls *param, U32 val, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSztProtExtnField_Tunnel_Inform_ExtIEs ARGS ((SztProtExtnField_Tunnel_Inform_ExtIEs *param, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSztProtExtnCont_Tunnel_Inform_ExtIEs ARGS ((SztProtExtnCont_Tunnel_Inform_ExtIEs *param, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSztTunnelInform ARGS ((SztTunnelInform *param, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSzt_ExtnUEAgg_MaxBitrates_ExtIEsCls ARGS ((Szt_ExtnUEAgg_MaxBitrates_ExtIEsCls *param, U32 val, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSztProtExtnField_UEAgg_MaxBitrates_ExtIEs ARGS ((SztProtExtnField_UEAgg_MaxBitrates_ExtIEs *param, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSztProtExtnCont_UEAgg_MaxBitrates_ExtIEs ARGS ((SztProtExtnCont_UEAgg_MaxBitrates_ExtIEs *param, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSztUEAggMaxBitrate ARGS ((SztUEAggMaxBitrate *param, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSzt_ExtnUE_S1AP_ID_pair_ExtIEsCls ARGS ((Szt_ExtnUE_S1AP_ID_pair_ExtIEsCls *param, U32 val, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSztProtExtnField_UE_S1AP_ID_pair_ExtIEs ARGS ((SztProtExtnField_UE_S1AP_ID_pair_ExtIEs *param, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSztProtExtnCont_UE_S1AP_ID_pair_ExtIEs ARGS ((SztProtExtnCont_UE_S1AP_ID_pair_ExtIEs *param, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSztUE_S1AP_ID_pair ARGS ((SztUE_S1AP_ID_pair *param, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSztUE_S1AP_IDs ARGS ((SztUE_S1AP_IDs *param, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSzt_ExtnUE_assocLogS1_ConItemExtIEsCls ARGS ((Szt_ExtnUE_assocLogS1_ConItemExtIEsCls *param, U32 val, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSztProtExtnField_UE_assocLogS1_ConItemExtIEs ARGS ((SztProtExtnField_UE_assocLogS1_ConItemExtIEs *param, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSztProtExtnCont_UE_assocLogS1_ConItemExtIEs ARGS ((SztProtExtnCont_UE_assocLogS1_ConItemExtIEs *param, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSztUE_assocLogS1_ConItem ARGS ((SztUE_assocLogS1_ConItem *param, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSztUEPagID ARGS ((SztUEPagID *param, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSzt_ExtnUESecurCapabilities_ExtIEsCls ARGS ((Szt_ExtnUESecurCapabilities_ExtIEsCls *param, U32 val, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSztProtExtnField_UESecurCapabilities_ExtIEs ARGS ((SztProtExtnField_UESecurCapabilities_ExtIEs *param, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSztProtExtnCont_UESecurCapabilities_ExtIEs ARGS ((SztProtExtnCont_UESecurCapabilities_ExtIEs *param, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSztUESecurCapabilities ARGS ((SztUESecurCapabilities *param, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSztWarningAreaLst ARGS ((SztWarningAreaLst *param, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSzt_ValueHovrReqdIEsCls ARGS ((Szt_ValueHovrReqdIEsCls *param, U32 val, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSztProtIE_Field_HovrReqdIEs ARGS ((SztProtIE_Field_HovrReqdIEs *param, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSztProtIE_Cont_HovrReqdIEs ARGS ((SztProtIE_Cont_HovrReqdIEs *param, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSztHovrReqd ARGS ((SztHovrReqd *param, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSzt_ExtnE_RABDataFwdingItem_ExtIEsCls ARGS ((Szt_ExtnE_RABDataFwdingItem_ExtIEsCls *param, U32 val, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSztProtExtnField_E_RABDataFwdingItem_ExtIEs ARGS ((SztProtExtnField_E_RABDataFwdingItem_ExtIEs *param, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSztProtExtnCont_E_RABDataFwdingItem_ExtIEs ARGS ((SztProtExtnCont_E_RABDataFwdingItem_ExtIEs *param, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSztE_RABDataFwdingItem ARGS ((SztE_RABDataFwdingItem *param, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSzt_ValueE_RABDataFwdingItemIEsCls ARGS ((Szt_ValueE_RABDataFwdingItemIEsCls *param, U32 val, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSztProtIE_Field_E_RABDataFwdingItemIEs ARGS ((SztProtIE_Field_E_RABDataFwdingItemIEs *param, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSztProtIE_ContLst_E_RABDataFwdingItemIEs ARGS ((SztProtIE_ContLst_E_RABDataFwdingItemIEs *param, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSzt_ValueHovrCmmdIEsCls ARGS ((Szt_ValueHovrCmmdIEsCls *param, U32 val, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSztProtIE_Field_HovrCmmdIEs ARGS ((SztProtIE_Field_HovrCmmdIEs *param, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSztProtIE_Cont_HovrCmmdIEs ARGS ((SztProtIE_Cont_HovrCmmdIEs *param, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSztHovrCmmd ARGS ((SztHovrCmmd *param, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSzt_ValueHovrPrepFailIEsCls ARGS ((Szt_ValueHovrPrepFailIEsCls *param, U32 val, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSztProtIE_Field_HovrPrepFailIEs ARGS ((SztProtIE_Field_HovrPrepFailIEs *param, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSztProtIE_Cont_HovrPrepFailIEs ARGS ((SztProtIE_Cont_HovrPrepFailIEs *param, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSztHovrPrepFail ARGS ((SztHovrPrepFail *param, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSzt_ExtnE_RABToBeSetupItemHOReq_ExtIEsCls ARGS ((Szt_ExtnE_RABToBeSetupItemHOReq_ExtIEsCls *param, U32 val, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSztProtExtnField_E_RABToBeSetupItemHOReq_ExtIEs ARGS ((SztProtExtnField_E_RABToBeSetupItemHOReq_ExtIEs *param, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSztProtExtnCont_E_RABToBeSetupItemHOReq_ExtIEs ARGS ((SztProtExtnCont_E_RABToBeSetupItemHOReq_ExtIEs *param, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSztE_RABToBeSetupItemHOReq ARGS ((SztE_RABToBeSetupItemHOReq *param, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSzt_ValueE_RABToBeSetupItemHOReqIEsCls ARGS ((Szt_ValueE_RABToBeSetupItemHOReqIEsCls *param, U32 val, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSztProtIE_Field_E_RABToBeSetupItemHOReqIEs ARGS ((SztProtIE_Field_E_RABToBeSetupItemHOReqIEs *param, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSztProtIE_ContLst_E_RABToBeSetupItemHOReqIEs ARGS ((SztProtIE_ContLst_E_RABToBeSetupItemHOReqIEs *param, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSzt_ValueHovrRqstIEsCls ARGS ((Szt_ValueHovrRqstIEsCls *param, U32 val, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSztProtIE_Field_HovrRqstIEs ARGS ((SztProtIE_Field_HovrRqstIEs *param, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSztProtIE_Cont_HovrRqstIEs ARGS ((SztProtIE_Cont_HovrRqstIEs *param, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSztHovrRqst ARGS ((SztHovrRqst *param, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSzt_ExtnE_RABAdmtdItem_ExtIEsCls ARGS ((Szt_ExtnE_RABAdmtdItem_ExtIEsCls *param, U32 val, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSztProtExtnField_E_RABAdmtdItem_ExtIEs ARGS ((SztProtExtnField_E_RABAdmtdItem_ExtIEs *param, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSztProtExtnCont_E_RABAdmtdItem_ExtIEs ARGS ((SztProtExtnCont_E_RABAdmtdItem_ExtIEs *param, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSztE_RABAdmtdItem ARGS ((SztE_RABAdmtdItem *param, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSzt_ValueE_RABAdmtdItemIEsCls ARGS ((Szt_ValueE_RABAdmtdItemIEsCls *param, U32 val, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSztProtIE_Field_E_RABAdmtdItemIEs ARGS ((SztProtIE_Field_E_RABAdmtdItemIEs *param, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSztProtIE_ContLst_E_RABAdmtdItemIEs ARGS ((SztProtIE_ContLst_E_RABAdmtdItemIEs *param, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSzt_ExtnE_RABFailedToSetupItemHOReqAckExtIEsCls ARGS ((Szt_ExtnE_RABFailedToSetupItemHOReqAckExtIEsCls *param, U32 val, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSztProtExtnField_E_RABFailedToSetupItemHOReqAckExtIEs ARGS ((SztProtExtnField_E_RABFailedToSetupItemHOReqAckExtIEs *param, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSztProtExtnCont_E_RABFailedToSetupItemHOReqAckExtIEs ARGS ((SztProtExtnCont_E_RABFailedToSetupItemHOReqAckExtIEs *param, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSztE_RABFailedToSetupItemHOReqAck ARGS ((SztE_RABFailedToSetupItemHOReqAck *param, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSzt_ValueE_RABFailedtoSetupItemHOReqAckIEsCls ARGS ((Szt_ValueE_RABFailedtoSetupItemHOReqAckIEsCls *param, U32 val, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSztProtIE_Field_E_RABFailedtoSetupItemHOReqAckIEs ARGS ((SztProtIE_Field_E_RABFailedtoSetupItemHOReqAckIEs *param, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSztProtIE_ContLst_E_RABFailedtoSetupItemHOReqAckIEs ARGS ((SztProtIE_ContLst_E_RABFailedtoSetupItemHOReqAckIEs *param, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSzt_ValueHovrRqstAckgIEsCls ARGS ((Szt_ValueHovrRqstAckgIEsCls *param, U32 val, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSztProtIE_Field_HovrRqstAckgIEs ARGS ((SztProtIE_Field_HovrRqstAckgIEs *param, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSztProtIE_Cont_HovrRqstAckgIEs ARGS ((SztProtIE_Cont_HovrRqstAckgIEs *param, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSztHovrRqstAckg ARGS ((SztHovrRqstAckg *param, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSzt_ValueHovrFailIEsCls ARGS ((Szt_ValueHovrFailIEsCls *param, U32 val, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSztProtIE_Field_HovrFailIEs ARGS ((SztProtIE_Field_HovrFailIEs *param, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSztProtIE_Cont_HovrFailIEs ARGS ((SztProtIE_Cont_HovrFailIEs *param, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSztHovrFail ARGS ((SztHovrFail *param, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSzt_ValueHovrNtfyIEsCls ARGS ((Szt_ValueHovrNtfyIEsCls *param, U32 val, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSztProtIE_Field_HovrNtfyIEs ARGS ((SztProtIE_Field_HovrNtfyIEs *param, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSztProtIE_Cont_HovrNtfyIEs ARGS ((SztProtIE_Cont_HovrNtfyIEs *param, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSztHovrNtfy ARGS ((SztHovrNtfy *param, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSzt_ExtnE_RABToBeSwedDLItem_ExtIEsCls ARGS ((Szt_ExtnE_RABToBeSwedDLItem_ExtIEsCls *param, U32 val, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSztProtExtnField_E_RABToBeSwedDLItem_ExtIEs ARGS ((SztProtExtnField_E_RABToBeSwedDLItem_ExtIEs *param, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSztProtExtnCont_E_RABToBeSwedDLItem_ExtIEs ARGS ((SztProtExtnCont_E_RABToBeSwedDLItem_ExtIEs *param, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSztE_RABToBeSwedDLItem ARGS ((SztE_RABToBeSwedDLItem *param, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSzt_ValueE_RABToBeSwedDLItemIEsCls ARGS ((Szt_ValueE_RABToBeSwedDLItemIEsCls *param, U32 val, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSztProtIE_Field_E_RABToBeSwedDLItemIEs ARGS ((SztProtIE_Field_E_RABToBeSwedDLItemIEs *param, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSztProtIE_ContLst_E_RABToBeSwedDLItemIEs ARGS ((SztProtIE_ContLst_E_RABToBeSwedDLItemIEs *param, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSzt_ValuePathSwRqstIEsCls ARGS ((Szt_ValuePathSwRqstIEsCls *param, U32 val, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSztProtIE_Field_PathSwRqstIEs ARGS ((SztProtIE_Field_PathSwRqstIEs *param, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSztProtIE_Cont_PathSwRqstIEs ARGS ((SztProtIE_Cont_PathSwRqstIEs *param, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSztPathSwRqst ARGS ((SztPathSwRqst *param, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSzt_ExtnE_RABToBeSwedULItem_ExtIEsCls ARGS ((Szt_ExtnE_RABToBeSwedULItem_ExtIEsCls *param, U32 val, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSztProtExtnField_E_RABToBeSwedULItem_ExtIEs ARGS ((SztProtExtnField_E_RABToBeSwedULItem_ExtIEs *param, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSztProtExtnCont_E_RABToBeSwedULItem_ExtIEs ARGS ((SztProtExtnCont_E_RABToBeSwedULItem_ExtIEs *param, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSztE_RABToBeSwedULItem ARGS ((SztE_RABToBeSwedULItem *param, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSzt_ValueE_RABToBeSwedULItemIEsCls ARGS ((Szt_ValueE_RABToBeSwedULItemIEsCls *param, U32 val, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSztProtIE_Field_E_RABToBeSwedULItemIEs ARGS ((SztProtIE_Field_E_RABToBeSwedULItemIEs *param, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSztProtIE_ContLst_E_RABToBeSwedULItemIEs ARGS ((SztProtIE_ContLst_E_RABToBeSwedULItemIEs *param, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSzt_ValuePathSwRqstAckgIEsCls ARGS ((Szt_ValuePathSwRqstAckgIEsCls *param, U32 val, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSztProtIE_Field_PathSwRqstAckgIEs ARGS ((SztProtIE_Field_PathSwRqstAckgIEs *param, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSztProtIE_Cont_PathSwRqstAckgIEs ARGS ((SztProtIE_Cont_PathSwRqstAckgIEs *param, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSztPathSwRqstAckg ARGS ((SztPathSwRqstAckg *param, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSzt_ValuePathSwRqstFailIEsCls ARGS ((Szt_ValuePathSwRqstFailIEsCls *param, U32 val, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSztProtIE_Field_PathSwRqstFailIEs ARGS ((SztProtIE_Field_PathSwRqstFailIEs *param, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSztProtIE_Cont_PathSwRqstFailIEs ARGS ((SztProtIE_Cont_PathSwRqstFailIEs *param, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSztPathSwRqstFail ARGS ((SztPathSwRqstFail *param, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSzt_ValueHovrCancelIEsCls ARGS ((Szt_ValueHovrCancelIEsCls *param, U32 val, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSztProtIE_Field_HovrCancelIEs ARGS ((SztProtIE_Field_HovrCancelIEs *param, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSztProtIE_Cont_HovrCancelIEs ARGS ((SztProtIE_Cont_HovrCancelIEs *param, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSztHovrCancel ARGS ((SztHovrCancel *param, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSzt_ValueHovrCancelAckgIEsCls ARGS ((Szt_ValueHovrCancelAckgIEsCls *param, U32 val, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSztProtIE_Field_HovrCancelAckgIEs ARGS ((SztProtIE_Field_HovrCancelAckgIEs *param, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSztProtIE_Cont_HovrCancelAckgIEs ARGS ((SztProtIE_Cont_HovrCancelAckgIEs *param, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSztHovrCancelAckg ARGS ((SztHovrCancelAckg *param, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSzt_ExtnE_RABToBeSetupItemBrSUReqExtIEsCls ARGS ((Szt_ExtnE_RABToBeSetupItemBrSUReqExtIEsCls *param, U32 val, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSztProtExtnField_E_RABToBeSetupItemBrSUReqExtIEs ARGS ((SztProtExtnField_E_RABToBeSetupItemBrSUReqExtIEs *param, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSztProtExtnCont_E_RABToBeSetupItemBrSUReqExtIEs ARGS ((SztProtExtnCont_E_RABToBeSetupItemBrSUReqExtIEs *param, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSztE_RABToBeSetupItemBrSUReq ARGS ((SztE_RABToBeSetupItemBrSUReq *param, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSzt_ValueE_RABToBeSetupItemBrSUReqIEsCls ARGS ((Szt_ValueE_RABToBeSetupItemBrSUReqIEsCls *param, U32 val, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSztProtIE_Field_E_RABToBeSetupItemBrSUReqIEs ARGS ((SztProtIE_Field_E_RABToBeSetupItemBrSUReqIEs *param, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSztE_RABToBeSetupLstBrSUReq ARGS ((SztE_RABToBeSetupLstBrSUReq *param, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSzt_ValueE_RABSetupRqstIEsCls ARGS ((Szt_ValueE_RABSetupRqstIEsCls *param, U32 val, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSztProtIE_Field_E_RABSetupRqstIEs ARGS ((SztProtIE_Field_E_RABSetupRqstIEs *param, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSztProtIE_Cont_E_RABSetupRqstIEs ARGS ((SztProtIE_Cont_E_RABSetupRqstIEs *param, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSztE_RABSetupRqst ARGS ((SztE_RABSetupRqst *param, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSzt_ExtnE_RABSetupItemBrSUResExtIEsCls ARGS ((Szt_ExtnE_RABSetupItemBrSUResExtIEsCls *param, U32 val, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSztProtExtnField_E_RABSetupItemBrSUResExtIEs ARGS ((SztProtExtnField_E_RABSetupItemBrSUResExtIEs *param, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSztProtExtnCont_E_RABSetupItemBrSUResExtIEs ARGS ((SztProtExtnCont_E_RABSetupItemBrSUResExtIEs *param, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSztE_RABSetupItemBrSURes ARGS ((SztE_RABSetupItemBrSURes *param, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSzt_ValueE_RABSetupItemBrSUResIEsCls ARGS ((Szt_ValueE_RABSetupItemBrSUResIEsCls *param, U32 val, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSztProtIE_Field_E_RABSetupItemBrSUResIEs ARGS ((SztProtIE_Field_E_RABSetupItemBrSUResIEs *param, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSztE_RABSetupLstBrSURes ARGS ((SztE_RABSetupLstBrSURes *param, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSzt_ValueE_RABSetupRespIEsCls ARGS ((Szt_ValueE_RABSetupRespIEsCls *param, U32 val, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSztProtIE_Field_E_RABSetupRespIEs ARGS ((SztProtIE_Field_E_RABSetupRespIEs *param, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSztProtIE_Cont_E_RABSetupRespIEs ARGS ((SztProtIE_Cont_E_RABSetupRespIEs *param, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSztE_RABSetupResp ARGS ((SztE_RABSetupResp *param, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSzt_ExtnE_RABToBeMdfyItemBrModReqExtIEsCls ARGS ((Szt_ExtnE_RABToBeMdfyItemBrModReqExtIEsCls *param, U32 val, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSztProtExtnField_E_RABToBeMdfyItemBrModReqExtIEs ARGS ((SztProtExtnField_E_RABToBeMdfyItemBrModReqExtIEs *param, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSztProtExtnCont_E_RABToBeMdfyItemBrModReqExtIEs ARGS ((SztProtExtnCont_E_RABToBeMdfyItemBrModReqExtIEs *param, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSztE_RABToBeMdfdItemBrModReq ARGS ((SztE_RABToBeMdfdItemBrModReq *param, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSzt_ValueE_RABToBeMdfdItemBrModReqIEsCls ARGS ((Szt_ValueE_RABToBeMdfdItemBrModReqIEsCls *param, U32 val, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSztProtIE_Field_E_RABToBeMdfdItemBrModReqIEs ARGS ((SztProtIE_Field_E_RABToBeMdfdItemBrModReqIEs *param, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSztE_RABToBeMdfdLstBrModReq ARGS ((SztE_RABToBeMdfdLstBrModReq *param, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSzt_ValueE_RABMdfyRqstIEsCls ARGS ((Szt_ValueE_RABMdfyRqstIEsCls *param, U32 val, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSztProtIE_Field_E_RABMdfyRqstIEs ARGS ((SztProtIE_Field_E_RABMdfyRqstIEs *param, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSztProtIE_Cont_E_RABMdfyRqstIEs ARGS ((SztProtIE_Cont_E_RABMdfyRqstIEs *param, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSztE_RABMdfyRqst ARGS ((SztE_RABMdfyRqst *param, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSzt_ExtnE_RABMdfyItemBrModResExtIEsCls ARGS ((Szt_ExtnE_RABMdfyItemBrModResExtIEsCls *param, U32 val, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSztProtExtnField_E_RABMdfyItemBrModResExtIEs ARGS ((SztProtExtnField_E_RABMdfyItemBrModResExtIEs *param, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSztProtExtnCont_E_RABMdfyItemBrModResExtIEs ARGS ((SztProtExtnCont_E_RABMdfyItemBrModResExtIEs *param, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSztE_RABMdfyItemBrModRes ARGS ((SztE_RABMdfyItemBrModRes *param, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSzt_ValueE_RABMdfyItemBrModResIEsCls ARGS ((Szt_ValueE_RABMdfyItemBrModResIEsCls *param, U32 val, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSztProtIE_Field_E_RABMdfyItemBrModResIEs ARGS ((SztProtIE_Field_E_RABMdfyItemBrModResIEs *param, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSztE_RABMdfyLstBrModRes ARGS ((SztE_RABMdfyLstBrModRes *param, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSzt_ValueE_RABMdfyRespIEsCls ARGS ((Szt_ValueE_RABMdfyRespIEsCls *param, U32 val, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSztProtIE_Field_E_RABMdfyRespIEs ARGS ((SztProtIE_Field_E_RABMdfyRespIEs *param, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSztProtIE_Cont_E_RABMdfyRespIEs ARGS ((SztProtIE_Cont_E_RABMdfyRespIEs *param, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSztE_RABMdfyResp ARGS ((SztE_RABMdfyResp *param, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSzt_ValueE_RABRlsCmmdIEsCls ARGS ((Szt_ValueE_RABRlsCmmdIEsCls *param, U32 val, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSztProtIE_Field_E_RABRlsCmmdIEs ARGS ((SztProtIE_Field_E_RABRlsCmmdIEs *param, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSztProtIE_Cont_E_RABRlsCmmdIEs ARGS ((SztProtIE_Cont_E_RABRlsCmmdIEs *param, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSztE_RABRlsCmmd ARGS ((SztE_RABRlsCmmd *param, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSzt_ExtnE_RABRlsItemBrRelCompExtIEsCls ARGS ((Szt_ExtnE_RABRlsItemBrRelCompExtIEsCls *param, U32 val, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSztProtExtnField_E_RABRlsItemBrRelCompExtIEs ARGS ((SztProtExtnField_E_RABRlsItemBrRelCompExtIEs *param, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSztProtExtnCont_E_RABRlsItemBrRelCompExtIEs ARGS ((SztProtExtnCont_E_RABRlsItemBrRelCompExtIEs *param, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSztE_RABRlsItemBrRelComp ARGS ((SztE_RABRlsItemBrRelComp *param, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSzt_ValueE_RABRlsItemBrRelCompIEsCls ARGS ((Szt_ValueE_RABRlsItemBrRelCompIEsCls *param, U32 val, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSztProtIE_Field_E_RABRlsItemBrRelCompIEs ARGS ((SztProtIE_Field_E_RABRlsItemBrRelCompIEs *param, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSztE_RABRlsLstBrRelComp ARGS ((SztE_RABRlsLstBrRelComp *param, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSzt_ValueE_RABRlsRespIEsCls ARGS ((Szt_ValueE_RABRlsRespIEsCls *param, U32 val, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSztProtIE_Field_E_RABRlsRespIEs ARGS ((SztProtIE_Field_E_RABRlsRespIEs *param, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSztProtIE_Cont_E_RABRlsRespIEs ARGS ((SztProtIE_Cont_E_RABRlsRespIEs *param, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSztE_RABRlsResp ARGS ((SztE_RABRlsResp *param, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSzt_ValueE_RABRlsIndIEsCls ARGS ((Szt_ValueE_RABRlsIndIEsCls *param, U32 val, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSztProtIE_Field_E_RABRlsIndIEs ARGS ((SztProtIE_Field_E_RABRlsIndIEs *param, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSztProtIE_Cont_E_RABRlsIndIEs ARGS ((SztProtIE_Cont_E_RABRlsIndIEs *param, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSztE_RABRlsInd ARGS ((SztE_RABRlsInd *param, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSzt_ExtnE_RABToBeSetupItemCtxtSUReqExtIEsCls ARGS ((Szt_ExtnE_RABToBeSetupItemCtxtSUReqExtIEsCls *param, U32 val, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSztProtExtnField_E_RABToBeSetupItemCtxtSUReqExtIEs ARGS ((SztProtExtnField_E_RABToBeSetupItemCtxtSUReqExtIEs *param, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSztProtExtnCont_E_RABToBeSetupItemCtxtSUReqExtIEs ARGS ((SztProtExtnCont_E_RABToBeSetupItemCtxtSUReqExtIEs *param, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSztE_RABToBeSetupItemCtxtSUReq ARGS ((SztE_RABToBeSetupItemCtxtSUReq *param, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSzt_ValueE_RABToBeSetupItemCtxtSUReqIEsCls ARGS ((Szt_ValueE_RABToBeSetupItemCtxtSUReqIEsCls *param, U32 val, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSztProtIE_Field_E_RABToBeSetupItemCtxtSUReqIEs ARGS ((SztProtIE_Field_E_RABToBeSetupItemCtxtSUReqIEs *param, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSztE_RABToBeSetupLstCtxtSUReq ARGS ((SztE_RABToBeSetupLstCtxtSUReq *param, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSzt_ValueInitCntxtSetupRqstIEsCls ARGS ((Szt_ValueInitCntxtSetupRqstIEsCls *param, U32 val, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSztProtIE_Field_InitCntxtSetupRqstIEs ARGS ((SztProtIE_Field_InitCntxtSetupRqstIEs *param, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSztProtIE_Cont_InitCntxtSetupRqstIEs ARGS ((SztProtIE_Cont_InitCntxtSetupRqstIEs *param, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSztInitCntxtSetupRqst ARGS ((SztInitCntxtSetupRqst *param, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSzt_ExtnE_RABSetupItemCtxtSUResExtIEsCls ARGS ((Szt_ExtnE_RABSetupItemCtxtSUResExtIEsCls *param, U32 val, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSztProtExtnField_E_RABSetupItemCtxtSUResExtIEs ARGS ((SztProtExtnField_E_RABSetupItemCtxtSUResExtIEs *param, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSztProtExtnCont_E_RABSetupItemCtxtSUResExtIEs ARGS ((SztProtExtnCont_E_RABSetupItemCtxtSUResExtIEs *param, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSztE_RABSetupItemCtxtSURes ARGS ((SztE_RABSetupItemCtxtSURes *param, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSzt_ValueE_RABSetupItemCtxtSUResIEsCls ARGS ((Szt_ValueE_RABSetupItemCtxtSUResIEsCls *param, U32 val, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSztProtIE_Field_E_RABSetupItemCtxtSUResIEs ARGS ((SztProtIE_Field_E_RABSetupItemCtxtSUResIEs *param, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSztE_RABSetupLstCtxtSURes ARGS ((SztE_RABSetupLstCtxtSURes *param, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSzt_ValueInitCntxtSetupRespIEsCls ARGS ((Szt_ValueInitCntxtSetupRespIEsCls *param, U32 val, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSztProtIE_Field_InitCntxtSetupRespIEs ARGS ((SztProtIE_Field_InitCntxtSetupRespIEs *param, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSztProtIE_Cont_InitCntxtSetupRespIEs ARGS ((SztProtIE_Cont_InitCntxtSetupRespIEs *param, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSztInitCntxtSetupResp ARGS ((SztInitCntxtSetupResp *param, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSzt_ValueInitCntxtSetupFailIEsCls ARGS ((Szt_ValueInitCntxtSetupFailIEsCls *param, U32 val, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSztProtIE_Field_InitCntxtSetupFailIEs ARGS ((SztProtIE_Field_InitCntxtSetupFailIEs *param, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSztProtIE_Cont_InitCntxtSetupFailIEs ARGS ((SztProtIE_Cont_InitCntxtSetupFailIEs *param, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSztInitCntxtSetupFail ARGS ((SztInitCntxtSetupFail *param, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSzt_ExtnTAIItemExtIEsCls ARGS ((Szt_ExtnTAIItemExtIEsCls *param, U32 val, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSztProtExtnField_TAIItemExtIEs ARGS ((SztProtExtnField_TAIItemExtIEs *param, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSztProtExtnCont_TAIItemExtIEs ARGS ((SztProtExtnCont_TAIItemExtIEs *param, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSztTAIItem ARGS ((SztTAIItem *param, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSzt_ValueTAIItemIEsCls ARGS ((Szt_ValueTAIItemIEsCls *param, U32 val, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSztProtIE_Field_TAIItemIEs ARGS ((SztProtIE_Field_TAIItemIEs *param, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSztTAILst ARGS ((SztTAILst *param, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSzt_ValuePagIEsCls ARGS ((Szt_ValuePagIEsCls *param, U32 val, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSztProtIE_Field_PagIEs ARGS ((SztProtIE_Field_PagIEs *param, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSztProtIE_Cont_PagIEs ARGS ((SztProtIE_Cont_PagIEs *param, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSztPag ARGS ((SztPag *param, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSzt_ValueUECntxtRlsRqst_IEsCls ARGS ((Szt_ValueUECntxtRlsRqst_IEsCls *param, U32 val, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSztProtIE_Field_UECntxtRlsRqst_IEs ARGS ((SztProtIE_Field_UECntxtRlsRqst_IEs *param, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSztProtIE_Cont_UECntxtRlsRqst_IEs ARGS ((SztProtIE_Cont_UECntxtRlsRqst_IEs *param, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSztUECntxtRlsRqst ARGS ((SztUECntxtRlsRqst *param, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSzt_ValueUECntxtRlsCmmd_IEsCls ARGS ((Szt_ValueUECntxtRlsCmmd_IEsCls *param, U32 val, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSztProtIE_Field_UECntxtRlsCmmd_IEs ARGS ((SztProtIE_Field_UECntxtRlsCmmd_IEs *param, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSztProtIE_Cont_UECntxtRlsCmmd_IEs ARGS ((SztProtIE_Cont_UECntxtRlsCmmd_IEs *param, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSztUECntxtRlsCmmd ARGS ((SztUECntxtRlsCmmd *param, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSzt_ValueUECntxtRlsComplete_IEsCls ARGS ((Szt_ValueUECntxtRlsComplete_IEsCls *param, U32 val, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSztProtIE_Field_UECntxtRlsComplete_IEs ARGS ((SztProtIE_Field_UECntxtRlsComplete_IEs *param, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSztProtIE_Cont_UECntxtRlsComplete_IEs ARGS ((SztProtIE_Cont_UECntxtRlsComplete_IEs *param, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSztUECntxtRlsComplete ARGS ((SztUECntxtRlsComplete *param, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSzt_ValueUECntxtModificationRqstIEsCls ARGS ((Szt_ValueUECntxtModificationRqstIEsCls *param, U32 val, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSztProtIE_Field_UECntxtModificationRqstIEs ARGS ((SztProtIE_Field_UECntxtModificationRqstIEs *param, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSztProtIE_Cont_UECntxtModificationRqstIEs ARGS ((SztProtIE_Cont_UECntxtModificationRqstIEs *param, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSztUECntxtModificationRqst ARGS ((SztUECntxtModificationRqst *param, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSzt_ValueUECntxtModificationRespIEsCls ARGS ((Szt_ValueUECntxtModificationRespIEsCls *param, U32 val, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSztProtIE_Field_UECntxtModificationRespIEs ARGS ((SztProtIE_Field_UECntxtModificationRespIEs *param, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSztProtIE_Cont_UECntxtModificationRespIEs ARGS ((SztProtIE_Cont_UECntxtModificationRespIEs *param, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSztUECntxtModificationResp ARGS ((SztUECntxtModificationResp *param, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSzt_ValueUECntxtModificationFailIEsCls ARGS ((Szt_ValueUECntxtModificationFailIEsCls *param, U32 val, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSztProtIE_Field_UECntxtModificationFailIEs ARGS ((SztProtIE_Field_UECntxtModificationFailIEs *param, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSztProtIE_Cont_UECntxtModificationFailIEs ARGS ((SztProtIE_Cont_UECntxtModificationFailIEs *param, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSztUECntxtModificationFail ARGS ((SztUECntxtModificationFail *param, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSzt_ValueUERadioCapbltyMatchRqstIEsCls ARGS ((Szt_ValueUERadioCapbltyMatchRqstIEsCls *param, U32 val, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSztProtIE_Field_UERadioCapbltyMatchRqstIEs ARGS ((SztProtIE_Field_UERadioCapbltyMatchRqstIEs *param, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSztProtIE_Cont_UERadioCapbltyMatchRqstIEs ARGS ((SztProtIE_Cont_UERadioCapbltyMatchRqstIEs *param, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSztUERadioCapbltyMatchRqst ARGS ((SztUERadioCapbltyMatchRqst *param, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSzt_ValueUERadioCapbltyMatchRespIEsCls ARGS ((Szt_ValueUERadioCapbltyMatchRespIEsCls *param, U32 val, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSztProtIE_Field_UERadioCapbltyMatchRespIEs ARGS ((SztProtIE_Field_UERadioCapbltyMatchRespIEs *param, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSztProtIE_Cont_UERadioCapbltyMatchRespIEs ARGS ((SztProtIE_Cont_UERadioCapbltyMatchRespIEs *param, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSztUERadioCapbltyMatchResp ARGS ((SztUERadioCapbltyMatchResp *param, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSzt_ValueDlnkNASTport_IEsCls ARGS ((Szt_ValueDlnkNASTport_IEsCls *param, U32 val, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSztProtIE_Field_DlnkNASTport_IEs ARGS ((SztProtIE_Field_DlnkNASTport_IEs *param, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSztProtIE_Cont_DlnkNASTport_IEs ARGS ((SztProtIE_Cont_DlnkNASTport_IEs *param, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSztDlnkNASTport ARGS ((SztDlnkNASTport *param, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSzt_ValueInitUEMsg_IEsCls ARGS ((Szt_ValueInitUEMsg_IEsCls *param, U32 val, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSztProtIE_Field_InitUEMsg_IEs ARGS ((SztProtIE_Field_InitUEMsg_IEs *param, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSztProtIE_Cont_InitUEMsg_IEs ARGS ((SztProtIE_Cont_InitUEMsg_IEs *param, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSztInitUEMsg ARGS ((SztInitUEMsg *param, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSzt_ValueUlnkNASTport_IEsCls ARGS ((Szt_ValueUlnkNASTport_IEsCls *param, U32 val, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSztProtIE_Field_UlnkNASTport_IEs ARGS ((SztProtIE_Field_UlnkNASTport_IEs *param, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSztProtIE_Cont_UlnkNASTport_IEs ARGS ((SztProtIE_Cont_UlnkNASTport_IEs *param, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSztUlnkNASTport ARGS ((SztUlnkNASTport *param, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSzt_ValueNASNonDlvryInd_IEsCls ARGS ((Szt_ValueNASNonDlvryInd_IEsCls *param, U32 val, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSztProtIE_Field_NASNonDlvryInd_IEs ARGS ((SztProtIE_Field_NASNonDlvryInd_IEs *param, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSztProtIE_Cont_NASNonDlvryInd_IEs ARGS ((SztProtIE_Cont_NASNonDlvryInd_IEs *param, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSztNASNonDlvryInd ARGS ((SztNASNonDlvryInd *param, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSzt_ValueUE_assocLogS1_ConItemResCls ARGS ((Szt_ValueUE_assocLogS1_ConItemResCls *param, U32 val, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSztProtIE_Field_UE_assocLogS1_ConItemRes ARGS ((SztProtIE_Field_UE_assocLogS1_ConItemRes *param, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSztUE_assocLogS1_ConLstRes ARGS ((SztUE_assocLogS1_ConLstRes *param, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSztResetTyp ARGS ((SztResetTyp *param, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSzt_ValueResetIEsCls ARGS ((Szt_ValueResetIEsCls *param, U32 val, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSztProtIE_Field_ResetIEs ARGS ((SztProtIE_Field_ResetIEs *param, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSztProtIE_Cont_ResetIEs ARGS ((SztProtIE_Cont_ResetIEs *param, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSztReset ARGS ((SztReset *param, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSzt_ValueUE_assocLogS1_ConItemResAckCls ARGS ((Szt_ValueUE_assocLogS1_ConItemResAckCls *param, U32 val, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSztProtIE_Field_UE_assocLogS1_ConItemResAck ARGS ((SztProtIE_Field_UE_assocLogS1_ConItemResAck *param, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSztUE_assocLogS1_ConLstResAck ARGS ((SztUE_assocLogS1_ConLstResAck *param, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSzt_ValueResetAckgIEsCls ARGS ((Szt_ValueResetAckgIEsCls *param, U32 val, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSztProtIE_Field_ResetAckgIEs ARGS ((SztProtIE_Field_ResetAckgIEs *param, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSztProtIE_Cont_ResetAckgIEs ARGS ((SztProtIE_Cont_ResetAckgIEs *param, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSztResetAckg ARGS ((SztResetAckg *param, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSzt_ValueErrIndIEsCls ARGS ((Szt_ValueErrIndIEsCls *param, U32 val, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSztProtIE_Field_ErrIndIEs ARGS ((SztProtIE_Field_ErrIndIEs *param, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSztProtIE_Cont_ErrIndIEs ARGS ((SztProtIE_Cont_ErrIndIEs *param, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSztErrInd ARGS ((SztErrInd *param, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSzt_ValueS1SetupRqstIEsCls ARGS ((Szt_ValueS1SetupRqstIEsCls *param, U32 val, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSztProtIE_Field_S1SetupRqstIEs ARGS ((SztProtIE_Field_S1SetupRqstIEs *param, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSztProtIE_Cont_S1SetupRqstIEs ARGS ((SztProtIE_Cont_S1SetupRqstIEs *param, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSztS1SetupRqst ARGS ((SztS1SetupRqst *param, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSzt_ValueS1SetupRespIEsCls ARGS ((Szt_ValueS1SetupRespIEsCls *param, U32 val, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSztProtIE_Field_S1SetupRespIEs ARGS ((SztProtIE_Field_S1SetupRespIEs *param, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSztProtIE_Cont_S1SetupRespIEs ARGS ((SztProtIE_Cont_S1SetupRespIEs *param, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSztS1SetupResp ARGS ((SztS1SetupResp *param, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSzt_ValueS1SetupFailIEsCls ARGS ((Szt_ValueS1SetupFailIEsCls *param, U32 val, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSztProtIE_Field_S1SetupFailIEs ARGS ((SztProtIE_Field_S1SetupFailIEs *param, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSztProtIE_Cont_S1SetupFailIEs ARGS ((SztProtIE_Cont_S1SetupFailIEs *param, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSztS1SetupFail ARGS ((SztS1SetupFail *param, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSzt_ValueENBConfigUpdIEsCls ARGS ((Szt_ValueENBConfigUpdIEsCls *param, U32 val, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSztProtIE_Field_ENBConfigUpdIEs ARGS ((SztProtIE_Field_ENBConfigUpdIEs *param, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSztProtIE_Cont_ENBConfigUpdIEs ARGS ((SztProtIE_Cont_ENBConfigUpdIEs *param, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSztENBConfigUpd ARGS ((SztENBConfigUpd *param, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSzt_ValueENBConfigUpdAckgIEsCls ARGS ((Szt_ValueENBConfigUpdAckgIEsCls *param, U32 val, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSztProtIE_Field_ENBConfigUpdAckgIEs ARGS ((SztProtIE_Field_ENBConfigUpdAckgIEs *param, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSztProtIE_Cont_ENBConfigUpdAckgIEs ARGS ((SztProtIE_Cont_ENBConfigUpdAckgIEs *param, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSztENBConfigUpdAckg ARGS ((SztENBConfigUpdAckg *param, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSzt_ValueENBConfigUpdFailIEsCls ARGS ((Szt_ValueENBConfigUpdFailIEsCls *param, U32 val, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSztProtIE_Field_ENBConfigUpdFailIEs ARGS ((SztProtIE_Field_ENBConfigUpdFailIEs *param, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSztProtIE_Cont_ENBConfigUpdFailIEs ARGS ((SztProtIE_Cont_ENBConfigUpdFailIEs *param, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSztENBConfigUpdFail ARGS ((SztENBConfigUpdFail *param, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSzt_ValueMMEConfigUpdIEsCls ARGS ((Szt_ValueMMEConfigUpdIEsCls *param, U32 val, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSztProtIE_Field_MMEConfigUpdIEs ARGS ((SztProtIE_Field_MMEConfigUpdIEs *param, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSztProtIE_Cont_MMEConfigUpdIEs ARGS ((SztProtIE_Cont_MMEConfigUpdIEs *param, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSztMMEConfigUpd ARGS ((SztMMEConfigUpd *param, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSzt_ValueMMEConfigUpdAckgIEsCls ARGS ((Szt_ValueMMEConfigUpdAckgIEsCls *param, U32 val, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSztProtIE_Field_MMEConfigUpdAckgIEs ARGS ((SztProtIE_Field_MMEConfigUpdAckgIEs *param, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSztProtIE_Cont_MMEConfigUpdAckgIEs ARGS ((SztProtIE_Cont_MMEConfigUpdAckgIEs *param, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSztMMEConfigUpdAckg ARGS ((SztMMEConfigUpdAckg *param, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSzt_ValueMMEConfigUpdFailIEsCls ARGS ((Szt_ValueMMEConfigUpdFailIEsCls *param, U32 val, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSztProtIE_Field_MMEConfigUpdFailIEs ARGS ((SztProtIE_Field_MMEConfigUpdFailIEs *param, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSztProtIE_Cont_MMEConfigUpdFailIEs ARGS ((SztProtIE_Cont_MMEConfigUpdFailIEs *param, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSztMMEConfigUpdFail ARGS ((SztMMEConfigUpdFail *param, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSzt_ValueDlnkS1cdma2000tunnelingIEsCls ARGS ((Szt_ValueDlnkS1cdma2000tunnelingIEsCls *param, U32 val, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSztProtIE_Field_DlnkS1cdma2000tunnelingIEs ARGS ((SztProtIE_Field_DlnkS1cdma2000tunnelingIEs *param, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSztProtIE_Cont_DlnkS1cdma2000tunnelingIEs ARGS ((SztProtIE_Cont_DlnkS1cdma2000tunnelingIEs *param, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSztDlnkS1cdma2000tunneling ARGS ((SztDlnkS1cdma2000tunneling *param, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSzt_ValueUlnkS1cdma2000tunnelingIEsCls ARGS ((Szt_ValueUlnkS1cdma2000tunnelingIEsCls *param, U32 val, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSztProtIE_Field_UlnkS1cdma2000tunnelingIEs ARGS ((SztProtIE_Field_UlnkS1cdma2000tunnelingIEs *param, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSztProtIE_Cont_UlnkS1cdma2000tunnelingIEs ARGS ((SztProtIE_Cont_UlnkS1cdma2000tunnelingIEs *param, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSztUlnkS1cdma2000tunneling ARGS ((SztUlnkS1cdma2000tunneling *param, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSzt_ValueUECapbltyInfoIndIEsCls ARGS ((Szt_ValueUECapbltyInfoIndIEsCls *param, U32 val, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSztProtIE_Field_UECapbltyInfoIndIEs ARGS ((SztProtIE_Field_UECapbltyInfoIndIEs *param, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSztProtIE_Cont_UECapbltyInfoIndIEs ARGS ((SztProtIE_Cont_UECapbltyInfoIndIEs *param, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSztUECapbltyInfoInd ARGS ((SztUECapbltyInfoInd *param, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSzt_ValueENBStatusTfrIEsCls ARGS ((Szt_ValueENBStatusTfrIEsCls *param, U32 val, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSztProtIE_Field_ENBStatusTfrIEs ARGS ((SztProtIE_Field_ENBStatusTfrIEs *param, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSztProtIE_Cont_ENBStatusTfrIEs ARGS ((SztProtIE_Cont_ENBStatusTfrIEs *param, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSztENBStatusTfr ARGS ((SztENBStatusTfr *param, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSzt_ValueMMEStatusTfrIEsCls ARGS ((Szt_ValueMMEStatusTfrIEsCls *param, U32 val, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSztProtIE_Field_MMEStatusTfrIEs ARGS ((SztProtIE_Field_MMEStatusTfrIEs *param, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSztProtIE_Cont_MMEStatusTfrIEs ARGS ((SztProtIE_Cont_MMEStatusTfrIEs *param, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSztMMEStatusTfr ARGS ((SztMMEStatusTfr *param, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSzt_ValueTraceStartIEsCls ARGS ((Szt_ValueTraceStartIEsCls *param, U32 val, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSztProtIE_Field_TraceStartIEs ARGS ((SztProtIE_Field_TraceStartIEs *param, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSztProtIE_Cont_TraceStartIEs ARGS ((SztProtIE_Cont_TraceStartIEs *param, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSztTraceStart ARGS ((SztTraceStart *param, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSzt_ValueTraceFailIndIEsCls ARGS ((Szt_ValueTraceFailIndIEsCls *param, U32 val, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSztProtIE_Field_TraceFailIndIEs ARGS ((SztProtIE_Field_TraceFailIndIEs *param, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSztProtIE_Cont_TraceFailIndIEs ARGS ((SztProtIE_Cont_TraceFailIndIEs *param, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSztTraceFailInd ARGS ((SztTraceFailInd *param, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSzt_ValueDeactvTraceIEsCls ARGS ((Szt_ValueDeactvTraceIEsCls *param, U32 val, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSztProtIE_Field_DeactvTraceIEs ARGS ((SztProtIE_Field_DeactvTraceIEs *param, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSztProtIE_Cont_DeactvTraceIEs ARGS ((SztProtIE_Cont_DeactvTraceIEs *param, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSztDeactvTrace ARGS ((SztDeactvTrace *param, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSzt_ValueCellTrafficTraceIEsCls ARGS ((Szt_ValueCellTrafficTraceIEsCls *param, U32 val, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSztProtIE_Field_CellTrafficTraceIEs ARGS ((SztProtIE_Field_CellTrafficTraceIEs *param, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSztProtIE_Cont_CellTrafficTraceIEs ARGS ((SztProtIE_Cont_CellTrafficTraceIEs *param, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSztCellTrafficTrace ARGS ((SztCellTrafficTrace *param, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSzt_ValueLocRprtngCntrlIEsCls ARGS ((Szt_ValueLocRprtngCntrlIEsCls *param, U32 val, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSztProtIE_Field_LocRprtngCntrlIEs ARGS ((SztProtIE_Field_LocRprtngCntrlIEs *param, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSztProtIE_Cont_LocRprtngCntrlIEs ARGS ((SztProtIE_Cont_LocRprtngCntrlIEs *param, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSztLocRprtngCntrl ARGS ((SztLocRprtngCntrl *param, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSzt_ValueLocRprtngFailIndIEsCls ARGS ((Szt_ValueLocRprtngFailIndIEsCls *param, U32 val, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSztProtIE_Field_LocRprtngFailIndIEs ARGS ((SztProtIE_Field_LocRprtngFailIndIEs *param, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSztProtIE_Cont_LocRprtngFailIndIEs ARGS ((SztProtIE_Cont_LocRprtngFailIndIEs *param, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSztLocRprtngFailInd ARGS ((SztLocRprtngFailInd *param, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSzt_ValueLocReportIEsCls ARGS ((Szt_ValueLocReportIEsCls *param, U32 val, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSztProtIE_Field_LocReportIEs ARGS ((SztProtIE_Field_LocReportIEs *param, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSztProtIE_Cont_LocReportIEs ARGS ((SztProtIE_Cont_LocReportIEs *param, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSztLocReport ARGS ((SztLocReport *param, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSzt_ValueOverloadStartIEsCls ARGS ((Szt_ValueOverloadStartIEsCls *param, U32 val, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSztProtIE_Field_OverloadStartIEs ARGS ((SztProtIE_Field_OverloadStartIEs *param, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSztProtIE_Cont_OverloadStartIEs ARGS ((SztProtIE_Cont_OverloadStartIEs *param, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSztOverloadStart ARGS ((SztOverloadStart *param, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSzt_ValueOverloadStopIEsCls ARGS ((Szt_ValueOverloadStopIEsCls *param, U32 val, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSztProtIE_Field_OverloadStopIEs ARGS ((SztProtIE_Field_OverloadStopIEs *param, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSztProtIE_Cont_OverloadStopIEs ARGS ((SztProtIE_Cont_OverloadStopIEs *param, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSztOverloadStop ARGS ((SztOverloadStop *param, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSzt_ValueWriteReplaceWarningRqstIEsCls ARGS ((Szt_ValueWriteReplaceWarningRqstIEsCls *param, U32 val, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSztProtIE_Field_WriteReplaceWarningRqstIEs ARGS ((SztProtIE_Field_WriteReplaceWarningRqstIEs *param, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSztProtIE_Cont_WriteReplaceWarningRqstIEs ARGS ((SztProtIE_Cont_WriteReplaceWarningRqstIEs *param, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSztWriteReplaceWarningRqst ARGS ((SztWriteReplaceWarningRqst *param, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSzt_ValueWriteReplaceWarningRespIEsCls ARGS ((Szt_ValueWriteReplaceWarningRespIEsCls *param, U32 val, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSztProtIE_Field_WriteReplaceWarningRespIEs ARGS ((SztProtIE_Field_WriteReplaceWarningRespIEs *param, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSztProtIE_Cont_WriteReplaceWarningRespIEs ARGS ((SztProtIE_Cont_WriteReplaceWarningRespIEs *param, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSztWriteReplaceWarningResp ARGS ((SztWriteReplaceWarningResp *param, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSztInter_SystemInformTfrTyp ARGS ((SztInter_SystemInformTfrTyp *param, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSzt_ValueENBDirectInformTfrIEsCls ARGS ((Szt_ValueENBDirectInformTfrIEsCls *param, U32 val, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSztProtIE_Field_ENBDirectInformTfrIEs ARGS ((SztProtIE_Field_ENBDirectInformTfrIEs *param, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSztProtIE_Cont_ENBDirectInformTfrIEs ARGS ((SztProtIE_Cont_ENBDirectInformTfrIEs *param, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSztENBDirectInformTfr ARGS ((SztENBDirectInformTfr *param, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSzt_ValueMMEDirectInformTfrIEsCls ARGS ((Szt_ValueMMEDirectInformTfrIEsCls *param, U32 val, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSztProtIE_Field_MMEDirectInformTfrIEs ARGS ((SztProtIE_Field_MMEDirectInformTfrIEs *param, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSztProtIE_Cont_MMEDirectInformTfrIEs ARGS ((SztProtIE_Cont_MMEDirectInformTfrIEs *param, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSztMMEDirectInformTfr ARGS ((SztMMEDirectInformTfr *param, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSzt_ValueENBConfigTfrIEsCls ARGS ((Szt_ValueENBConfigTfrIEsCls *param, U32 val, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSztProtIE_Field_ENBConfigTfrIEs ARGS ((SztProtIE_Field_ENBConfigTfrIEs *param, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSztProtIE_Cont_ENBConfigTfrIEs ARGS ((SztProtIE_Cont_ENBConfigTfrIEs *param, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSztENBConfigTfr ARGS ((SztENBConfigTfr *param, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSzt_ValueMMEConfigTfrIEsCls ARGS ((Szt_ValueMMEConfigTfrIEsCls *param, U32 val, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSztProtIE_Field_MMEConfigTfrIEs ARGS ((SztProtIE_Field_MMEConfigTfrIEs *param, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSztProtIE_Cont_MMEConfigTfrIEs ARGS ((SztProtIE_Cont_MMEConfigTfrIEs *param, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSztMMEConfigTfr ARGS ((SztMMEConfigTfr *param, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSzt_ValuePrivMsgIEsCls ARGS ((Szt_ValuePrivMsgIEsCls *param, U32 val, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSztPrivIE_Field_PrivMsgIEs ARGS ((SztPrivIE_Field_PrivMsgIEs *param, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSztPrivIE_Cont_PrivMsgIEs ARGS ((SztPrivIE_Cont_PrivMsgIEs *param, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSztPrivMsg ARGS ((SztPrivMsg *param, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSzt_ValueKillRqstIEsCls ARGS ((Szt_ValueKillRqstIEsCls *param, U32 val, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSztProtIE_Field_KillRqstIEs ARGS ((SztProtIE_Field_KillRqstIEs *param, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSztProtIE_Cont_KillRqstIEs ARGS ((SztProtIE_Cont_KillRqstIEs *param, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSztKillRqst ARGS ((SztKillRqst *param, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSzt_ValueKillRespIEsCls ARGS ((Szt_ValueKillRespIEsCls *param, U32 val, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSztProtIE_Field_KillRespIEs ARGS ((SztProtIE_Field_KillRespIEs *param, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSztProtIE_Cont_KillRespIEs ARGS ((SztProtIE_Cont_KillRespIEs *param, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSztKillResp ARGS ((SztKillResp *param, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSzt_ValueDlnkUEAssociatedLPPaTport_IEsCls ARGS ((Szt_ValueDlnkUEAssociatedLPPaTport_IEsCls *param, U32 val, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSztProtIE_Field_DlnkUEAssociatedLPPaTport_IEs ARGS ((SztProtIE_Field_DlnkUEAssociatedLPPaTport_IEs *param, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSztProtIE_Cont_DlnkUEAssociatedLPPaTport_IEs ARGS ((SztProtIE_Cont_DlnkUEAssociatedLPPaTport_IEs *param, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSztDlnkUEAssociatedLPPaTport ARGS ((SztDlnkUEAssociatedLPPaTport *param, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSzt_ValueUlnkUEAssociatedLPPaTport_IEsCls ARGS ((Szt_ValueUlnkUEAssociatedLPPaTport_IEsCls *param, U32 val, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSztProtIE_Field_UlnkUEAssociatedLPPaTport_IEs ARGS ((SztProtIE_Field_UlnkUEAssociatedLPPaTport_IEs *param, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSztProtIE_Cont_UlnkUEAssociatedLPPaTport_IEs ARGS ((SztProtIE_Cont_UlnkUEAssociatedLPPaTport_IEs *param, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSztUlnkUEAssociatedLPPaTport ARGS ((SztUlnkUEAssociatedLPPaTport *param, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSzt_ValueDlnkNonUEAssociatedLPPaTport_IEsCls ARGS ((Szt_ValueDlnkNonUEAssociatedLPPaTport_IEsCls *param, U32 val, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSztProtIE_Field_DlnkNonUEAssociatedLPPaTport_IEs ARGS ((SztProtIE_Field_DlnkNonUEAssociatedLPPaTport_IEs *param, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSztProtIE_Cont_DlnkNonUEAssociatedLPPaTport_IEs ARGS ((SztProtIE_Cont_DlnkNonUEAssociatedLPPaTport_IEs *param, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSztDlnkNonUEAssociatedLPPaTport ARGS ((SztDlnkNonUEAssociatedLPPaTport *param, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSzt_ValueUlnkNonUEAssociatedLPPaTport_IEsCls ARGS ((Szt_ValueUlnkNonUEAssociatedLPPaTport_IEsCls *param, U32 val, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSztProtIE_Field_UlnkNonUEAssociatedLPPaTport_IEs ARGS ((SztProtIE_Field_UlnkNonUEAssociatedLPPaTport_IEs *param, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSztProtIE_Cont_UlnkNonUEAssociatedLPPaTport_IEs ARGS ((SztProtIE_Cont_UlnkNonUEAssociatedLPPaTport_IEs *param, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSztUlnkNonUEAssociatedLPPaTport ARGS ((SztUlnkNonUEAssociatedLPPaTport *param, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSzt_InitiatingMsgCls ARGS ((Szt_InitiatingMsgCls *param, U32 val, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSztInitiatingMsg ARGS ((SztInitiatingMsg *param, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSzt_SuccessfulOutcomeCls ARGS ((Szt_SuccessfulOutcomeCls *param, U32 val, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSztSuccessfulOutcome ARGS ((SztSuccessfulOutcome *param, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSzt_UnsuccessfulOutcomeCls ARGS ((Szt_UnsuccessfulOutcomeCls *param, U32 val, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSztUnsuccessfulOutcome ARGS ((SztUnsuccessfulOutcome *param, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSztS1AP_PDU ARGS ((SztS1AP_PDU *param, Ptr ptr, Buffer *mBuf));
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
