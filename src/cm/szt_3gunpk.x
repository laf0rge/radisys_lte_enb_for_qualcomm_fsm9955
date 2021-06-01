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

     Name:     S1AP - SZT interface

     Type:     C source file

     Desc:     C source code for szt interface

     File:     szt_3gunpk.x

     Sid:     

     Prg:     akaranth 

*********************************************************************21*/

/* szt_3gunpk_x_001.main_1: Support for Source RNC to Target RNC TC Enc/Dec. */

EXTERN S16 cmUnpkSzt_ExtnTrCH_ID_ExtIEsCls ARGS ((Szt_ExtnTrCH_ID_ExtIEsCls *param, U32 val, Ptr ptr, Buffer *mBuf ));
EXTERN S16 cmUnpkSztProtExtnField_TrCH_ID_ExtIEs ARGS ((SztProtExtnField_TrCH_ID_ExtIEs *param, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSztProtExtnCont_TrCH_ID_ExtIEs ARGS ((SztProtExtnCont_TrCH_ID_ExtIEs *param, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSzt_ExtnRAB_TrCH_MappingItem_ExtIEsCls ARGS ((Szt_ExtnRAB_TrCH_MappingItem_ExtIEsCls *param, U32 val, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSztProtExtnField_RAB_TrCH_MappingItem_ExtIEs ARGS ((SztProtExtnField_RAB_TrCH_MappingItem_ExtIEs *param, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSztProtExtnCont_RAB_TrCH_MappingItem_ExtIEs ARGS ((SztProtExtnCont_RAB_TrCH_MappingItem_ExtIEs *param, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSztTrCH_ID ARGS ((SztTrCH_ID *param, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSztTrCH_ID_Lst ARGS ((SztTrCH_ID_Lst *param, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSztRAB_TrCH_MappingItem ARGS ((SztRAB_TrCH_MappingItem *param, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSztRAB_TrCH_Mapping ARGS ((SztRAB_TrCH_Mapping *param, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSzt_ExtnSRB_TrCH_MappingItem_ExtIEsCls ARGS ((Szt_ExtnSRB_TrCH_MappingItem_ExtIEsCls *param, U32 val, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSztProtExtnField_SRB_TrCH_MappingItem_ExtIEs ARGS ((SztProtExtnField_SRB_TrCH_MappingItem_ExtIEs *param, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSztProtExtnCont_SRB_TrCH_MappingItem_ExtIEs ARGS ((SztProtExtnCont_SRB_TrCH_MappingItem_ExtIEs *param, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSztSRB_TrCH_MappingItem ARGS ((SztSRB_TrCH_MappingItem *param, Ptr ptr, Buffer *mBuf ));
EXTERN S16 cmUnpkSztSRB_TrCH_Mapping ARGS ((SztSRB_TrCH_Mapping *param, Ptr ptr, Buffer *mBuf ));
EXTERN S16 cmUnpkSzt_ExtnSrcUTRANCellID_ExtIEsCls ARGS ((Szt_ExtnSrcUTRANCellID_ExtIEsCls *param, U32 val, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSztProtExtnField_SrcUTRANCellID_ExtIEs ARGS ((SztProtExtnField_SrcUTRANCellID_ExtIEs *param, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSztProtExtnCont_SrcUTRANCellID_ExtIEs ARGS ((SztProtExtnCont_SrcUTRANCellID_ExtIEs *param, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSztSrcUTRANCellID ARGS ((SztSrcUTRANCellID *param, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSzt_ExtnCGI_ExtIEsCls ARGS ((Szt_ExtnCGI_ExtIEsCls *param, U32 val, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSztProtExtnField_CGI_ExtIEs ARGS ((SztProtExtnField_CGI_ExtIEs *param, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSztProtExtnCont_CGI_ExtIEs ARGS ((SztProtExtnCont_CGI_ExtIEs *param, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSztCGI ARGS ((SztCGI *param, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSztSrcCellID ARGS ((SztSrcCellID *param, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSzt_ExtnCellLoadInform_ExtIEsCls ARGS ((Szt_ExtnCellLoadInform_ExtIEsCls *param, U32 val, Ptr ptr, Buffer *mBuf ));
EXTERN S16 cmUnpkSztProtExtnField_CellLoadInform_ExtIEs ARGS ((SztProtExtnField_CellLoadInform_ExtIEs *param, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSztProtExtnCont_CellLoadInform_ExtIEs ARGS ((SztProtExtnCont_CellLoadInform_ExtIEs *param, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSztCellLoadInform ARGS ((SztCellLoadInform *param, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSzt_ExtnCellLoadInformGroup_ExtIEsCls ARGS ((Szt_ExtnCellLoadInformGroup_ExtIEsCls *param, U32 val, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSztProtExtnField_CellLoadInformGroup_ExtIEs ARGS((SztProtExtnField_CellLoadInformGroup_ExtIEs *param, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSztProtExtnCont_CellLoadInformGroup_ExtIEs ARGS ((SztProtExtnCont_CellLoadInformGroup_ExtIEs *param, Ptr ptr, Buffer *mBuf ));
EXTERN S16 cmUnpkSztCellLoadInformGroup ARGS((SztCellLoadInformGroup *param, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSzt_ExtnTraceRecordingSessionInform_ExtIEsCls ARGS ((Szt_ExtnTraceRecordingSessionInform_ExtIEsCls *param, U32 val, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSztProtExtnField_TraceRecordingSessionInform_ExtIEs ARGS ((SztProtExtnField_TraceRecordingSessionInform_ExtIEs *param, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSztProtExtnCont_TraceRecordingSessionInform_ExtIEs ARGS ((SztProtExtnCont_TraceRecordingSessionInform_ExtIEs *param, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSztTraceRecordingSessionInform ARGS ((SztTraceRecordingSessionInform *param, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSzt_ExtnSRVCC_Inform_ExtIEsCls ARGS ((Szt_ExtnSRVCC_Inform_ExtIEsCls *param, U32 val, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSztProtExtnField_SRVCC_Inform_ExtIEs ARGS ((SztProtExtnField_SRVCC_Inform_ExtIEs *param, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSztProtExtnCont_SRVCC_Inform_ExtIEs ARGS ((SztProtExtnCont_SRVCC_Inform_ExtIEs *param, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSztSRVCC_Inform ARGS ((SztSRVCC_Inform *param, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSzt_ExtnSrcRNC_ToTgetRNC_TprntCont_ExtIEsCls ARGS ((Szt_ExtnSrcRNC_ToTgetRNC_TprntCont_ExtIEsCls *param, U32 val, Ptr ptr, Buffer *mBuf ));
EXTERN S16 cmUnpkSztProtExtnField_SrcRNC_ToTgetRNC_TprntCont_ExtIEs ARGS ((SztProtExtnField_SrcRNC_ToTgetRNC_TprntCont_ExtIEs *param, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSztProtExtnCont_SrcRNC_ToTgetRNC_TprntCont_ExtIEs ARGS ((SztProtExtnCont_SrcRNC_ToTgetRNC_TprntCont_ExtIEs *param, Ptr ptr, Buffer *mBuf ));
EXTERN S16 cmUnpkSztSrcRNC_ToTgetRNC_TprntCont ARGS ((SztSrcRNC_ToTgetRNC_TprntCont_3gasn *param, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSzt_ExtnTgetRNC_ToSrcRNC_TprntCont_ExtIEsCls ARGS ((Szt_ExtnTgetRNC_ToSrcRNC_TprntCont_ExtIEsCls *param, U32 val, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSztProtExtnField_TgetRNC_ToSrcRNC_TprntCont_ExtIEs ARGS ((SztProtExtnField_TgetRNC_ToSrcRNC_TprntCont_ExtIEs *param, Ptr ptr, Buffer *mBuf ));
EXTERN S16 cmUnpkSztProtExtnCont_TgetRNC_ToSrcRNC_TprntCont_ExtIEs ARGS ((SztProtExtnCont_TgetRNC_ToSrcRNC_TprntCont_ExtIEs *param, Ptr ptr, Buffer *mBuf ));
EXTERN S16 cmUnpkSztTgetRNC_ToSrcRNC_TprntCont ARGS ((SztTgetRNC_ToSrcRNC_TprntCont_3gasn *param, Ptr ptr, Buffer *mBuf ));
EXTERN S16 cmUnpkSzt3g_ExtnCGI_ExtIEsCls ARGS ((Szt3g_ExtnCGI_ExtIEsCls *param, U32 val, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSzt3gProtExtnField_CGI_ExtIEs ARGS ((Szt3gProtExtnField_CGI_ExtIEs *param, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSzt3gProtExtnCont_CGI_ExtIEs ARGS ((Szt3gProtExtnCont_CGI_ExtIEs *param, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkSzt3gCGI ARGS ((Szt3gCGI *param, Ptr ptr, Buffer *mBuf));


/**********************************************************************
 
         End of file:    szt_3gunpk.x 
 
**********************************************************************/

/********************************************************************40**
 
        Notes:
 
*********************************************************************41*/

/********************************************************************50**
 
*********************************************************************51*/

/********************************************************************60**
 
        Revision history:
 
*********************************************************************61*/

/********************************************************************80**
 
*********************************************************************81*/
/********************************************************************90**
 
     ver       pat    init                  description
------------ -------- ---- ----------------------------------------------
$SID$    szt_3gunpk_x_001.main_1  akaranth 1. Support for Source RNC to Target RNC TC Enc/Dec.

********************************************************************91*/
