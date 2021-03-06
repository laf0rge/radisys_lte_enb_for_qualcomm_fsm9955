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
/**********************************************************************

     Name:     S1AP Layer

     Type:     C source file

     Desc:     C source code for DB

     File:     szt_3gasn.x

     Sid:      
 
     Prg:      akaranth 

**********************************************************************/

#ifndef __SZT3GASNX__
#define __SZT3GASNX__ 

/* szt_3gasn_x_001.main_1 : Support for Source RNC to Target RNC TC Enc/Dec. */

/* Element ----- NumberOfIuInstances ------- */
typedef TknU32 SztNumberOfIuInstances;

/* Element ----- RelocationType ------- */
typedef enum enumSztRelocationTyp
{
   SztRelocationTypue_not_involvedEnum,
   SztRelocationTypue_involvedEnum
} EnumSztRelocationTyp;
typedef TknU32 SztRelocationTyp;

/* Element ---------- IntegrityProtectionKey ----------- */
typedef TknStrBSXL SztIntegrityProtectionKey;

typedef TknU32 SztCN_DomainIndicator;

/* Element ----- EncryptionAlgorithm ------- */
typedef TknU32 SztEncryptionAlgorithm;

/* Element ------------- ChosenEncryptionAlgorithm ------------ 
typedef SztEncryptionAlgorithm  SztChosenEncryptionAlgorithm; */

/* Element ------------- ChosenEncryptionAlgorithm ------------ */
typedef TknU8 SztChosenEncryptionAlgorithm;

/* Element ------------- ChosenIntegrityProtectionAlgorithm ------------ */
typedef TknU8 SztChosenIntegrityProtectionAlgorithm;

/* Integrity Protection algo : Inetger 0..15*/
typedef TknU8     sztIntegrityProtectionAlgo;

/* Chosen Protection algorithm */
typedef sztIntegrityProtectionAlgo  sztIntegrityProtectionAlgorithm;

/* Encryption algo : Integer 0..15 */
typedef TknU8    sztEncryptionAlgo;

/* Chosen Encryption Algorithm */
typedef sztEncryptionAlgo  sztEncryptionAlgorithm;

/* Element ---------- EncryptionKey ----------- */
typedef TknStrBSXL SztEncryptionKey;

/* Element ----- D-RNTI ------- */
typedef TknU32 SztD_RNTI;

/* Element ----- TargetCellId ------- */
typedef TknU32 SztTgetCellId;

typedef struct s1ap_ExtnSrcUTRANCellID_ExtIEsCls
{
   union 
   {
      TknU8 unused;
   } u;
} Szt_ExtnSrcUTRANCellID_ExtIEsCls;
typedef struct s1apProtExtnField_SrcUTRANCellID_ExtIEs
{
   TknPres pres;
   SztProtExtnID id;
   SztCriticality criticality;
   Szt_ExtnSrcUTRANCellID_ExtIEsCls extensionValue;
} SztProtExtnField_SrcUTRANCellID_ExtIEs;
typedef struct s1apProtExtnCont_SrcUTRANCellID_ExtIEs
{
   TknU16 noComp;
   SztProtExtnField_SrcUTRANCellID_ExtIEs *member;
}SztProtExtnCont_SrcUTRANCellID_ExtIEs;

typedef struct s1apSrcUTRANCellID
{
   TknPres pres;
   SztPLMNidentity pLMNidentity;
   SztTgetCellId uTRANcellID;
   SztProtExtnCont_SrcUTRANCellID_ExtIEs iE_Extns;
} SztSrcUTRANCellID;

/* Element ---------- RAC ----------- */
typedef TknStr4 Szt3gRAC;

typedef struct s1ap3g_ExtnCGI_ExtIEsCls{
   union {
      Szt3gRAC sztRAC;
   } u;
} Szt3g_ExtnCGI_ExtIEsCls;

typedef struct s1ap3gProtExtnField_CGI_ExtIEs
{
   TknPres pres;
   SztProtExtnID id;
   SztCriticality criticality;
   Szt3g_ExtnCGI_ExtIEsCls extensionValue;
} Szt3gProtExtnField_CGI_ExtIEs;

typedef struct s1ap3gProtExtnCont_CGI_ExtIEs
{
   TknU16 noComp;
   Szt3gProtExtnField_CGI_ExtIEs *member;
}Szt3gProtExtnCont_CGI_ExtIEs;

typedef struct s1ap3gCGI
{
   TknPres pres;
   SztPLMNidentity pLMNidentity;
   SztLAC lAC;
   SztCI cI;
   Szt3gProtExtnCont_CGI_ExtIEs iE_Extns;
} Szt3gCGI;

typedef struct s1apSrcCellID
{
   TknU8 choice;
   union {
      SztSrcUTRANCellID sourceUTRANCellID;
      Szt3gCGI sourceGERANCellID; 
   } val;
} SztSrcCellID;
/* Element ---------- RAB-ID ----------- */
typedef TknBStr32 SztRAB_ID;

/* Element ----- DCH-ID ------- */
typedef TknU32 SztDCH_ID;

/* Element ----- DSCH-ID ------- */
typedef TknU32 SztDSCH_ID;

/* Element ----- HS-DSCH-MAC-d-Flow-ID ------- */
typedef TknU32 SztHS_DSCH_MAC_d_Flow_ID;

/* Element ----- E-DCH-MAC-d-Flow-ID ------- */
typedef TknU32 SztE_DCH_MAC_d_Flow_ID;

/* Element ----- USCH-ID ------- */
typedef TknU32 SztUSCH_ID;

/* Element ----- CN-DomainIndicator ------- */
typedef enum enumSztCN_DomainIndicator
{
   SztCN_DomainIndicatorcs_domainEnum,
   SztCN_DomainIndicatorps_domainEnum
} EnumSztCN_DomainIndicator;

typedef struct s1ap_ExtnTrCH_ID_ExtIEsCls{
   union {
      SztHS_DSCH_MAC_d_Flow_ID sztHS_DSCH_MAC_d_Flow_ID;
      SztE_DCH_MAC_d_Flow_ID sztE_DCH_MAC_d_Flow_ID;
   } u;
} Szt_ExtnTrCH_ID_ExtIEsCls;

typedef struct s1apProtExtnField_TrCH_ID_ExtIEs
{
   TknPres pres;
   SztProtExtnID id;
   SztCriticality criticality;
   Szt_ExtnTrCH_ID_ExtIEsCls extensionValue;
} SztProtExtnField_TrCH_ID_ExtIEs;
typedef struct s1apProtExtnCont_TrCH_ID_ExtIEs
{
   TknU16 noComp;
   SztProtExtnField_TrCH_ID_ExtIEs *member;
}SztProtExtnCont_TrCH_ID_ExtIEs;

typedef struct s1apTrCH_ID
{
   TknPres pres;
   SztDCH_ID dCH_ID;
   SztDSCH_ID dSCH_ID;
   SztUSCH_ID uSCH_ID;
   SztProtExtnCont_TrCH_ID_ExtIEs iE_Extns;
} SztTrCH_ID;

typedef struct s1apTrCH_ID_Lst
{
   TknU16 noComp;
   SztTrCH_ID *member;
}SztTrCH_ID_Lst;

typedef struct s1ap_ExtnRAB_TrCH_MappingItem_ExtIEsCls
{
   union 
   {
      SztCN_DomainIndicator sztCN_DomainIndicator;
   } u;
} Szt_ExtnRAB_TrCH_MappingItem_ExtIEsCls;

typedef struct s1apProtExtnField_RAB_TrCH_MappingItem_ExtIEs
{
   TknPres pres;
   SztProtExtnID id;
   SztCriticality criticality;
   Szt_ExtnRAB_TrCH_MappingItem_ExtIEsCls extensionValue;
} SztProtExtnField_RAB_TrCH_MappingItem_ExtIEs;

typedef struct s1apProtExtnCont_RAB_TrCH_MappingItem_ExtIEs
{
   TknU16 noComp;
   SztProtExtnField_RAB_TrCH_MappingItem_ExtIEs *member;
}SztProtExtnCont_RAB_TrCH_MappingItem_ExtIEs;

typedef struct s1apRAB_TrCH_MappingItem
{
   TknPres pres;
   SztRAB_ID rAB_ID;
   SztTrCH_ID_Lst trCH_ID_Lst;
   SztProtExtnCont_RAB_TrCH_MappingItem_ExtIEs iE_Extns;
} SztRAB_TrCH_MappingItem;

typedef struct s1apRAB_TrCH_Mapping
{
   TknU16 noComp;
   SztRAB_TrCH_MappingItem *member;
}SztRAB_TrCH_Mapping;


/* Element ----- SRB-ID ------- */
typedef TknU32 SztSRB_ID;

typedef struct s1ap_ExtnSRB_TrCH_MappingItem_ExtIEsCls
{
   union 
   {
      TknU8 unused;
   } u;
} Szt_ExtnSRB_TrCH_MappingItem_ExtIEsCls;

typedef struct s1apProtExtnField_SRB_TrCH_MappingItem_ExtIEs
{
   TknPres pres;
   SztProtExtnID id;
   SztCriticality criticality;
   Szt_ExtnSRB_TrCH_MappingItem_ExtIEsCls extensionValue;
} SztProtExtnField_SRB_TrCH_MappingItem_ExtIEs;
typedef struct s1apProtExtnCont_SRB_TrCH_MappingItem_ExtIEs
{
   TknU16 noComp;
   SztProtExtnField_SRB_TrCH_MappingItem_ExtIEs *member;
}SztProtExtnCont_SRB_TrCH_MappingItem_ExtIEs;

typedef struct s1apSRB_TrCH_MappingItem
{
   TknPres pres;
   SztSRB_ID sRB_ID;
   SztTrCH_ID trCH_ID;
   SztProtExtnCont_SRB_TrCH_MappingItem_ExtIEs iE_Extns;
} SztSRB_TrCH_MappingItem;

typedef struct s1apSRB_TrCH_Mapping
{
   TknU16 noComp;
   SztSRB_TrCH_MappingItem *member;
}SztSRB_TrCH_Mapping;

/* Element ----- Cell-Capacity-Class-Value ------- */
typedef TknU32 SztCell_Capacity_Class_Value;

/* Element ----- LoadValue ------- */
typedef TknU32 SztLoadValue;

/* Element ----- RTLoadValue ------- */
typedef TknU32 SztRTLoadValue;

/* Element ----- NRTLoadInformationValue ------- */
typedef TknU32 SztNRTLoadInformValue;
typedef struct s1ap_ExtnCellLoadInform_ExtIEsCls
{
   union 
   {
      TknU8 unused;
   } u;
} Szt_ExtnCellLoadInform_ExtIEsCls;
typedef struct s1apProtExtnField_CellLoadInform_ExtIEs
{
   TknPres pres;
   SztProtExtnID id;
   SztCriticality criticality;
   Szt_ExtnCellLoadInform_ExtIEsCls extensionValue;
} SztProtExtnField_CellLoadInform_ExtIEs;

typedef struct s1apProtExtnCont_CellLoadInform_ExtIEs
{
   TknU16 noComp;
   SztProtExtnField_CellLoadInform_ExtIEs *member;
}SztProtExtnCont_CellLoadInform_ExtIEs;

typedef struct s1apCellLoadInform
{
   TknPres pres;
   SztCell_Capacity_Class_Value cell_Capacity_Class_Value;
   SztLoadValue loadValue;
   SztRTLoadValue rTLoadValue;
   SztNRTLoadInformValue nRTLoadInformValue;
   SztProtExtnCont_CellLoadInform_ExtIEs iE_Extns;
} SztCellLoadInform;

typedef struct s1ap_ExtnCellLoadInformGroup_ExtIEsCls
{
   union 
   {
      TknU8 unused;
   } u;
} Szt_ExtnCellLoadInformGroup_ExtIEsCls;

typedef struct s1apProtExtnField_CellLoadInformGroup_ExtIEs
{
   TknPres pres;
   SztProtExtnID id;
   SztCriticality criticality;
   Szt_ExtnCellLoadInformGroup_ExtIEsCls extensionValue;
} SztProtExtnField_CellLoadInformGroup_ExtIEs;
typedef struct s1apProtExtnCont_CellLoadInformGroup_ExtIEs
{
   TknU16 noComp;
   SztProtExtnField_CellLoadInformGroup_ExtIEs *member;
}SztProtExtnCont_CellLoadInformGroup_ExtIEs;

/* Element ---------- TraceReference ----------- */
typedef TknStrOSXL SztTraceReference;

/* Element ----- TraceRecordingSessionReference ------- */
typedef TknU32 SztTraceRecordingSessionReference;

typedef struct s1ap_ExtnTraceRecordingSessionInform_ExtIEsCls{
   union {
      TknU8 unused;
   } u;
} Szt_ExtnTraceRecordingSessionInform_ExtIEsCls;

typedef struct s1apProtExtnField_TraceRecordingSessionInform_ExtIEs
{
   TknPres pres;
   SztProtExtnID id;
   SztCriticality criticality;
   Szt_ExtnTraceRecordingSessionInform_ExtIEsCls extensionValue;
} SztProtExtnField_TraceRecordingSessionInform_ExtIEs;

typedef struct s1apProtExtnCont_TraceRecordingSessionInform_ExtIEs
{
   TknU16 noComp;
   SztProtExtnField_TraceRecordingSessionInform_ExtIEs *member;
}SztProtExtnCont_TraceRecordingSessionInform_ExtIEs;

typedef struct s1apTraceRecordingSessionInform
{
   TknPres pres;
   SztTraceReference traceReference;
   SztTraceRecordingSessionReference traceRecordingSessionReference;
   SztProtExtnCont_TraceRecordingSessionInform_ExtIEs iE_Extns;
} SztTraceRecordingSessionInform;

/* Element ----- MBMSLinkingInformation ------- */
typedef enum enumSztMBMSLinkingInform
{
   SztMBMSLinkingInformuE_has_joined_multicast_servicesEnum
} EnumSztMBMSLinkingInform;
typedef TknU32 SztMBMSLinkingInform;

/* Element ---------- UE-History-Information ----------- */
typedef TknStrOSXL SztUE_History_Inform;

/* Element ----- CSFB-Information ------- */
typedef enum enumSztCSFB_Inform
{
   SztCSFB_InformcsfbEnum,
   SztCSFB_Informcsfb_high_priorityEnum
} EnumSztCSFB_Inform;
typedef TknU32 SztCSFB_Inform;

/* Element ---------- nonce ----------- */
typedef TknStrBSXL SztSRVCC_Informnonce;
typedef struct s1ap_ExtnSRVCC_Inform_ExtIEsCls
{
   union 
   {
      TknU8 unused;
   } u;
} Szt_ExtnSRVCC_Inform_ExtIEsCls;

typedef struct s1apProtExtnField_SRVCC_Inform_ExtIEs
{
   TknPres pres;
   SztProtExtnID id;
   SztCriticality criticality;
   Szt_ExtnSRVCC_Inform_ExtIEsCls extensionValue;
} SztProtExtnField_SRVCC_Inform_ExtIEs;

typedef struct s1apProtExtnCont_SRVCC_Inform_ExtIEs
{
   TknU16 noComp;
   SztProtExtnField_SRVCC_Inform_ExtIEs *member;
}SztProtExtnCont_SRVCC_Inform_ExtIEs;

typedef struct s1apSRVCC_Inform
{
   TknPres pres;
   SztSRVCC_Informnonce nonce;
   SztProtExtnCont_SRVCC_Inform_ExtIEs iE_Extns;
} SztSRVCC_Inform;


typedef struct s1apCellLoadInformGroup
{
   TknPres pres;
   SztSrcCellID sourceCellID;
   SztCellLoadInform uplinkCellLoadInform;
   SztCellLoadInform downlinkCellLoadInform;
   SztProtExtnCont_CellLoadInformGroup_ExtIEs iE_Extns;
} SztCellLoadInformGroup;

typedef struct s1ap_ExtnSrcRNC_ToTgetRNC_TprntCont_ExtIEsCls
{
   union 
   {
      SztSRB_TrCH_Mapping sztSRB_TrCH_Mapping;
      SztCellLoadInformGroup sztCellLoadInformGroup;
      SztTraceRecordingSessionInform sztTraceRecordingSessionInform;
      SztMBMSLinkingInform sztMBMSLinkingInform;
      SztD_RNTI sztD_RNTI;
      SztUE_History_Inform sztUE_History_Inform;
      SztSubscriberProfileIDforRFP sztSubscriberProfileIDforRFP;
      SztSRVCC_Inform sztSRVCC_Inform;
      SztRAB_ID sztRAB_ID;
      SztCSFB_Inform sztCSFB_Inform;
   } u;
} Szt_ExtnSrcRNC_ToTgetRNC_TprntCont_ExtIEsCls;

typedef struct s1apProtExtnField_SrcRNC_ToTgetRNC_TprntCont_ExtIEs
{
   TknPres pres;
   SztProtExtnID id;
   SztCriticality criticality;
   Szt_ExtnSrcRNC_ToTgetRNC_TprntCont_ExtIEsCls extensionValue;
} SztProtExtnField_SrcRNC_ToTgetRNC_TprntCont_ExtIEs;

typedef struct s1apProtExtnCont_SrcRNC_ToTgetRNC_TprntCont_ExtIEs
{
   TknU16 noComp;
   SztProtExtnField_SrcRNC_ToTgetRNC_TprntCont_ExtIEs *member;
}SztProtExtnCont_SrcRNC_ToTgetRNC_TprntCont_ExtIEs;

typedef struct s1apSrcRNC_ToTgetRNC_TprntCont
{
   TknPres pres;
   SztRRC_Cont rRC_Cont;
   SztNumberOfIuInstances numberOfIuInstances;
   SztRelocationTyp relocationTyp;
   SztChosenIntegrityProtectionAlgorithm chosenIntegrityProtectionAlgorithm;
   SztIntegrityProtectionKey integrityProtectionKey;
   SztChosenEncryptionAlgorithm chosenEncryptionAlgorithForSignalling;
   SztEncryptionKey cipheringKey;
   SztChosenEncryptionAlgorithm chosenEncryptionAlgorithForCS;
   SztChosenEncryptionAlgorithm chosenEncryptionAlgorithForPS;
   SztD_RNTI d_RNTI;
   SztTgetCellId targetCellId;
   SztRAB_TrCH_Mapping rAB_TrCH_Mapping;
   SztProtExtnCont_SrcRNC_ToTgetRNC_TprntCont_ExtIEs iE_Extns;
} SztSrcRNC_ToTgetRNC_TprntCont_3gasn;



typedef struct s1ap_ExtnTgetRNC_ToSrcRNC_TprntCont_ExtIEsCls{
   union {
      TknU8 unused;
   } u;
} Szt_ExtnTgetRNC_ToSrcRNC_TprntCont_ExtIEsCls;

typedef struct s1apProtExtnField_TgetRNC_ToSrcRNC_TprntCont_ExtIEs
{
   TknPres pres;
   SztProtExtnID id;
   SztCriticality criticality;
   Szt_ExtnTgetRNC_ToSrcRNC_TprntCont_ExtIEsCls extensionValue;
} SztProtExtnField_TgetRNC_ToSrcRNC_TprntCont_ExtIEs;

typedef struct s1apProtExtnCont_TgetRNC_ToSrcRNC_TprntCont_ExtIEs
{
   TknU16 noComp;
   SztProtExtnField_TgetRNC_ToSrcRNC_TprntCont_ExtIEs *member;
}SztProtExtnCont_TgetRNC_ToSrcRNC_TprntCont_ExtIEs;
typedef struct s1apTgetRNC_ToSrcRNC_TprntCont
{
   TknPres pres;
   SztRRC_Cont rRC_Cont;
   SztD_RNTI d_RNTI;
   SztProtExtnCont_TgetRNC_ToSrcRNC_TprntCont_ExtIEs iE_Extns;
} SztTgetRNC_ToSrcRNC_TprntCont_3gasn;

#endif

/**********************************************************************
 
         End of file:    szt_3gasn.x 
 
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
$SID$    szt_3gasn_x_001.main_1   akaranth 1. Support for Source RNC to Target RNC TC Enc/Dec.
********************************************************************91*/

