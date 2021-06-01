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
               

     File:     wr_kpi.h

     Sid:      $SID$ 

     Prg:      Sriky 

*********************************************************************21*/
#ifndef __WR_KPI_H__
#define __WR_KPI_H__

#ifdef __cplusplus
extern "C" {
#endif /* end of __cplusplus */

#ifdef WR_RSYS_KPI 
/*Prototypes*/

/*RRC Counters */
/*Attempted RRC Connection Establishments*/
/*KPI_ID_LTE_RRC_ATTCONESTAB_SUM*/
EXTERN S16 wrIncKpiFrRrcConAttmptd(U32 estabCause);

/*Successful RRC Connection Establishments*/    
/*KPI_ID_LTE_RRC_SUCCCONNESTAB_SUM*/
EXTERN S16 wrIncKpiFrRrcConSuccd(U32 estabCause);

/* Failed RRC Connection Establishments Due to Non Availability of the Uu
 * Resource*/
/*KPI_ID_LTE_RRC_FAILCONNESTAB_SUM*/
EXTERN S16 wrIncKpiFrRrcCnFailDuToNoUuRsrc(U32 estabCause);
/*KPI_ID_LTE_RRC_FAILCONNESTAB_SUM*/
EXTERN S16 wrIncKpiFrRrcConFaildEstabSum(Void);

/*Failed RRC Connection Establishments Due to degraded Radio conditions*/
/*KPI_ID_LTE_RRC_FAILCONNESTAB_DEGRADEDRADIO_SUM*/ 
EXTERN S16 wrIncKpiFrRrcCnFailDuToDgrdRdio(U32 estabCause);

/*Attempted RRC connection re-establishments*/
/*KPI_ID_LTE_RRC_CONNREESTABATT_SUM*/
EXTERN S16 wrIncKpiFrReestbAtmptdPrEtbCaus(U32 estabCause);

/*Successful RRC connection re-establishments*/
/*KPI_ID_LTE_RRC_CONNREESTABSUCC_SUM*/  
EXTERN S16 wrIncKpiFrReestbSuccdPrEtabCaus(U32 estabCause);

/*Failed RRC connection re-establishments*/
/*KPI_ID_LTE_RRC_CONNREESTABFAIL_SUM*/
EXTERN S16 wrIncKpiFrReestbFaildPrEtabCaus(U32 estabCause);

/*E-RAB Counters*/
/*Number of Initial E-RABs attempted to Setup*/
/*KPI_ID_LTE_ERAB_ESTABINITATTNBR_QCI_SUM*/
EXTERN S16 wrIncKpiFrERABEstbInitAttPrQci(U8 qciVal);

/*Number of Initial E-RABs successfully established*/
/*KPI_ID_LTE_ERAB_ESTABINITSUCCNBR_QCI_SUM*/
EXTERN S16 wrIncKpiFrERABEstbIntSuccPrQci(U8 qciVal);
 
/*Number of Initial E-RABs failed to setup*/
/*KPI_ID_LTE_ERAB_ESTABINITFAILNBR_SUM*/
EXTERN S16 wrIncKpiFrERABEstbInitFailSum(U16 numFailedErabs);

/*Number of E-RAB Attempted to setup*/
/*KPI_ID_LTE_ERAB_ESTABATTNBR_QCI_SUM*/
EXTERN S16 wrIncKpiFrERABEstabAttPrQci(U8 qciVal);

/*Number of E-RABs successfully established*/
/*KPI_ID_LTE_ERAB_ESTABSUCCNBR_QCI_SUM*/
EXTERN S16 wrIncKpiFrERABEstabSuccPrQci(U8 qciVal); 

/*Number of E-RABs failed to setup*/
/*KPI_ID_LTE_ERAB_ESTABFAILNBR_SUM*/
EXTERN S16 wrIncKpiFrERABFailPrCause(WrUmmMsgCause *cause);

/*Number of E-RABs requested to release initiated by HeNB per QCI*/
/*KPI_ID_LTE_ERAB_RELENBNBR_QCI_SUM*/
EXTERN S16 wrIncKpiFrERABReqToRelPrCause(WrUmmMsgCause *cause,U32 numRabs);

/*Number of E-RABS requested to released initieated by HeNB per QCI*/
EXTERN  S16 wrIncKpiFrERABReqToRelPrQci(U8 qciVal);

/*Number of E-RABs attempted to release*/
/*KPI_ID_LTE_ERAB_RELATTNBR_QCI_SUM*/
EXTERN S16 wrIncKpiFrERABRelAttPrQci(U8 qciVal);

/*Number of E-RABs successfully released*/
/*KPI_ID_LTE_ERAB_RELSUCCNBR_QCI_SUM*/
EXTERN S16 wrIncKpiFrERABRelSuccPrQci(U8 qciVal);

/*Number of E-RABs failed to release*/
/*KPI_ID_LTE_ERAB_RELFAILNBR_SUM*/
EXTERN S16 wrIncKpiFrERABRelCmdPrCause(WrUmmMsgCause *cause);

/*E-RAB modification Number of E-RABs attempted to modify the QoS parameter*/
/*KPI_ID_LTE_ERAB_MODQOSATTNBR_QCI_SUM*/
EXTERN S16 wrIncKpiFrERABModQoSAttPrQci(U8 qciVal);

/*E-RAB modification:Number of E-RABs successfully modified the QoS parameter*/
/*KPI_ID_LTE_ERAB_MODQOSSUCCNBR_QCI_SUM*/
EXTERN S16 wrIncKpiFrERABMdQoSSuccNbrPrQci(U8 qciVal);

/*E-RAB modification Number of E-RABs failed to modify the QoS parameter*/
/*KPI_ID_LTE_ERAB_MODQOSFAILNBR_SUM*/
EXTERN S16 wrIncKpiFrERABModFailPrCause(WrUmmMsgCause *cause);

/*UE Counters*/
/*UE CONTEXT Release: Number of UE CONTEXT Release Request initiated by eNodeB*/
/*KPI_ID_LTE_UECNTX_RELREQ_SUM*/
EXTERN S16 wrIncKpiUeCtxRelReqAttmptd(Void);

/*UE CONTEXT Release: Successful UE CONTEXT Release*/
/*KPI_ID_LTE_UECONTEXT_RELSUCCNBR*/
EXTERN S16 wrIncKpiUeCtxRelReqSuccd(Void);

/*HO Counters*/
/*Common HandOver methods*/
EXTERN S16 wrIncKpiFrHovrAttmptd(U8 hovInf,U32 hovType,U8 hoDir,Void *hovCause,Void *kpiInfo);
EXTERN S16 wrIncKpiFrHovrSuccd(U8 hovInf,U32 hovType,U8 hoDir,Void *hovCause,Void  *kpiInfo);
EXTERN S16 wrIncKpiFrHovrFaild(U8 hovType,U32 hov,U8 dir,Void *hovCause);

/* SRVCC counter */
EXTERN S16 wrIncKpiFrSrvccAttmptd(U32 hovType,U8 hoDir,Void *hovCause);
EXTERN S16 wrIncKpiFrSrvccSucc(U32 hovType,U8 hoDir,Void *hovCause);
EXTERN S16 wrIncKpiFrSrvccFail(U32 hovType,U8 hoDir,Void *hovCause);

/*Intra-LTE Handovers: Attempted outgoing Handover to eNodeB per Handover cause*/
/*KPI_ID_LTE_HO_TOENBATT_SUM*/
EXTERN S16 wrIncKpiFrOutX2BsdHovrAttmptd(WrUmmMsgCause *cztCause, WrEcgi *info);
/*KPI_ID_LTE_HO_TOENBATT_SUM */
EXTERN S16 wrIncKpiFrOutS1BsdLTEHovAttmptd(SztCause *sztCause, WrEcgi *info);

/*Intra-LTE Handovers: Successful outgoing Handover to eNodeB per Handover cause*/
/*KPI_ID_LTE_HO_TOENBSUCC_SUM*/
EXTERN S16 wrIncKpiFrOutX2BsdHovrSuccd(WrUmmMsgCause *cztCause, WrEcgi *info);
/*KPI_ID_LTE_HO_TOENBSUCC_SUM*/ 
EXTERN S16 wrIncKpiFrOutS1BsdLTEHovSuccd(SztCause *sztCause, WrEcgi *info);

/*Intra-LTE Handovers: Failed outgoing Handover to eNodeB per Handover cause*/
/*KPI_ID_LTE_HO_TOENBFAIL_SUM*/   /*NOT Implemented*/
EXTERN S16 wrIncKpiFrOutX2BsdLTEHovFaild(CztCause *cztCause);
/*KPI_ID_LTE_HO_TOENBFAIL_SUM*/   /*NOT Implemented*/
EXTERN S16 wrIncKpiFrOutS1BsdLTEHovFaild(SztCause *sztCause);

/*Intra-LTE Handovers: Attempted incomming Handover from eNodeB per Handover cause*/
/*KPI_ID_LTE_HO_FROMENBATT_SUM*/
EXTERN S16 wrIncKpiFrIncX2BsdLTEHovAttmptd(WrUmmMsgCause *cztCause);
/*KPI_ID_LTE_HO_FROMENBATT_SUM*/ 
EXTERN S16 wrIncKpiFrIncS1BsdLTEHovAttmptd(SztCause *sztCause);

/*Intra-LTE Handovers: Successful incoming Handover from eNodeB per Handover cause*/
/*KPI_ID_LTE_HO_FROMENBSUCC_SUM*/
EXTERN S16 wrIncKpiFrIncX2BsdLTEHovSuccd(CztCause *cztCause);
/*KPI_ID_LTE_HO_FROMENBSUCC_SUM*/ 
EXTERN S16 wrIncKpiFrIncS1BsdLTEHovSuccd(SztCause *sztCause);

/*Intra-LTE Handovers: Failed incoming Handover from eNodeB per Handover cause*/
/*KPI_ID_LTE_HO_FROMENBFAIL_SUM*/  /*NOT Implemented*/
EXTERN S16 wrIncKpiFrIncX2BsdLTEHovFaild(CztCause *cztCause);
/*KPI_ID_LTE_HO_FROMENBFAIL_SUM*/  /*NOT Implemented*/
EXTERN S16 wrIncKpiFrIncS1BsdLTEHovFaild(SztCause *sztCause);

/*Inter-RAT Handovers: Attempted outgoing Handovers to UTRAN per Handover cause*/
/*KPI_ID_LTE_HO_TOUTRANATT_TOUTRANCELL_SUM*/
EXTERN S16 wrIncKpiFrS1BsdLTEtoUtrnHoAtmpt(SztCause *sztCause);

/*Inter-RAT Handovers: Successful outgoing Handovers to UTRAN per Handover cause*/
/*KPI_ID_LTE_HO_TOUTRANSUCC_TOUTRANCELL_SUM*/
EXTERN S16 wrIncKpiFrS1BsdLTEtoUtrnHoSuccd(SztCause *sztCause);

/*Inter-RAT Handovers: Failed outgoing Handovers to UTRAN per Handover cause*/
/*KPI_ID_LTE_HO_TOUTRANFAIL_TOUTRANCELL_SUM*/
EXTERN S16 wrIncKpiFrS1BsdLTEtoUtrnHoFaild(SztCause *sztCause);

/*Inter-RAT Handovers: Attempted outgoing Handovers to GERAN per Handover cause*/
/*KPI_ID_LTE_HO_TOGERANATT_TOGERANCELL_SUM*/
EXTERN S16 wrIncKpiFrS1BsdLTEtoGernHoAtmpt(SztCause *sztCause);

/*Inter-RAT Handovers: Successful outgoing Handovers to GERAN per Handover cause*/
/*KPI_ID_LTE_HO_TOGERANSUCC_TOGERANCELL_SUM*/
EXTERN S16 wrIncKpiFrS1BsdLTEtoGernHoSuccd(SztCause *sztCause);

/*Inter-RAT Handovers: Failed outgoing Handovers to GERAN per Handover cause*/
/*KPI_ID_LTE_HO_TOGERANFAIL_TOGERANCELL_SUM*/
EXTERN S16 wrIncKpiFrS1BsdLTEtoGernHoFaild(SztCause *sztCause);

/*Attempted outgoing inter-eNB handover preparations*/
/*KPI_ID_LTE_HO_INTERENBOUTPREPATT*/
EXTERN S16 wrIncKpiFrOutIntrEnbHoPrpAtmpt(Void);

EXTERN S16 wrIncKpiFrOutHoAtmptBsdOnFrqTyp(U8 freqTyp,Bool measGapUsd);  
EXTERN S16 wrIncKpiFrOutHoSuccdBsdOnFrqTyp(U8 freqTyp,Bool measGapUsd);
/*Attempted outgoing intra-frequency handovers*/
/*KPI_ID_LTE_HO_INTRAFREQOUTATT*/    /*NOT Implemented*/
EXTERN S16 wrIncKpiFrOutIntraFrqHoAttmptd(Void);

/*Successful outgoing intra-frequency handovers*/
/*KPI_ID_LTE_HO_INTRAFREQOUTSUCC*/  /*NOT Implemented*/
EXTERN S16 wrIncKpiFrOutIntraFrqHoSuccd(Void);

/*Handover related measurements for DRX/non-DRX Attempted*/
/*KPI_ID_LTE_HO_DRXOUTATT/KPI_ID_LTE_HO_NODRXOUTATT*/
EXTERN S16 wrIncKpiFrHONoDrxOutAttmptd(Bool drxStatus);

/*Handover related measurements for DRX/non-DRX Successful*/
/*KPI_ID_LTE_HO_DRXOUTSUCC/KPI_ID_LTE_HO_NOPDRXOUTSUCC*/
EXTERN S16 wrIncKpiFrHONoDrxOutSuccd(Bool drxStatus);

/*Attempted outgoing inter-frequency handovers gap-assisted measurement*/
/*KPI_ID_LTE_HO_INTERFREQMEASGAPOUTATT/KPI_ID_LTE_HO_INTERFREQNOMEASGAPOUTATT*/
EXTERN S16 wrIncKpiFrInterFrqHOGpAsstAtmpt(Bool measGapSetUp);

/*Successful outgoing inter-frequency handovers gap-assisted measurement*/
/*KPI_ID_LTE_HO_INTERFREQMEASGAPOUTSUCC/KPI_ID_LTE_HO_INTERFREQNOMEASGAPOUTSUCC*/
EXTERN S16 wrIncKpiFrInterFrqHOGpAsstSuccd(Bool measGapSetUp);


EXTERN S16 wrIncKpiCsfbAtt(U8 redirTyp);
EXTERN S16 wrIncKpiCsfbSucc(U32 ratType, U8 redirType);
EXTERN S16 wrIncKpiCsfbFail(U8 redirTyp);
EXTERN S16 wrIncKpiCsfbWithSi(U8 redirTyp);
EXTERN S16 wrIncKpiFrRimAttmptd(Void);
EXTERN S16 wrIncKpiFrRimSuccd(Void);
EXTERN S16 wrIncKpiFrRimFaild(Void);
EXTERN S16 wrIncKpiFullRecfgHo(Void);
EXTERN S16 wrIncKpiFullReCfgReestb(Void);

/* S1 Signalling Counters */
/* UE-associated logical S1-connection establishment:Succesful UE-associated
 * logical S1-connection establishment from eNB to MME*/
/*KPI_ID_LTE_S1SIG_CONNESTABSUCC*/
EXTERN S16 wrIncKpiFrS1SIGConnEstabSucc(Void);

/* UE-associated logical S1-connection establishmentSuccesful UE-associated
 * logical S1-connection establishment from eNB to MME*/
/*KPI_ID_LTE_S1SIG_CONNESTABATT*/
EXTERN S16 wrIncKpiFrS1SIGConnEstabAttmptd(Void);

/* CSG_DEV Starts*/
/*CSG Counters*/

/* Inbound CSG Mobility Measurements Attempted inbound mobility for UEs to CSG
 * cells or  Hybrid cells in RRC_CONNECTED mode*/
EXTERN S16 wrIncKpiFrInboundCsgMobilityAttmptd(Void);  /*KPI_ID_LTE_CSG_ATTINBOUNDMOBILITY*/

/*Inbound CSG Mobility Measurements Successful inbound mobility for UEs to CSG
  cells or Hybrid cells in RRC_CONNECTED mode */
EXTERN S16 wrIncKpiFrInboundCsgMobilitySuccd(Void);    /*KPI_ID_LTE_CSG_SUCCINBOUNDMOBILITY*/

/*Inbound CSG Mobility Measurements Failed inbound mobility for UEs to CSG cells
  or Hybrid cells in RRC_CONNECTED mode */
EXTERN S16 wrIncKpiFrInboundCsgMobilityFaild(Void);    /*KPI_ID_LTE_CSG_FAILEDINBOUNDMOBILITY_SUM*/

/*Number of CSG  UEs to attaching to CSG Cells or Hybrid cells in RRC_CONNECTED mode */

EXTERN S16 wrIncKpiFrCsgUeAttachSuccd (Void);          /* KPI_ID_LTE_CSG_UEATTACHED_MEAN */ 

/*Failure Cause Statistics */
/*S1AP Failure Cause Counters */
EXTERN S16 wrIncKpiFrS1apFailCause(U32 causeType, U32 causeVal, U32 val);
/*S1AP Failure Cause Counters w.r.t RadioNw*/
EXTERN S16 wrIncKpiFrS1apFailCauseForRadioNW(U32 causeVal, U32 val);
/*S1AP Failure Cause Counters w.r.t Transport*/
EXTERN S16 wrIncKpiFrS1apFailCauseForTransport(U32 causeVal, U32 val);
/*S1AP Failure Cause Counters w.r.t NAS*/
EXTERN S16 wrIncKpiFrS1apFailCauseForNas(U32 causeVal, U32 val);
/*S1AP Failure Cause Counters w.r.t Protocol*/
EXTERN S16 wrIncKpiFrS1apFailCauseForProtocol(U32 causeVal, U32 val);
/*S1AP Failure Cause Counters w.r.t Misc*/
EXTERN S16 wrIncKpiFrS1apFailCauseForMisc(U32 causeVal, U32 val);
#if 0
/*RRC Re-Establish Failure Cause Counters*/
EXTERN S16 wrIncKpiFrRRCReEstabFailure(U32 causeVal);
#endif
/*EGTP Failure Cause Counters*/
EXTERN S16 wrIncKpiFrEgtpFailure(U32 causeVal);
/*RLC Failure Cause Counters*/
EXTERN S16 wrIncKpiFrRlcFailure(Void);
#if 0
/*RRC Failure Cause Counters*/
EXTERN S16 wrIncKpiFrRrcFailCause(Void);
#endif
/*RRC Security Failure Cause Counters*/
EXTERN S16 wrIncKpiFrRrcSecFailCause(Void);
/* RRC con estab fail due to S1AP failure */
EXTERN S16 wrIncKpiFrRrcS1apFailCause(Void);
/* RRC con estab fail due to PDCP failure */
EXTERN S16 wrIncKpiFrRrcPdcpFailCause(Void);
/* RRC con estab fail due to RLC failure */
EXTERN S16 wrIncKpiFrRrcRlcFailCause(Void);
/* RRC con estab fail due to MAC failure */
EXTERN S16 wrIncKpiFrRrcMacFailCause(Void);
/* RRC con estab fail due to RRC failure */
EXTERN S16 wrIncKpiFrRrcRrcFailCause(Void);

EXTERN S16 wrIncKpiFrRrcConReject(U8 cause);

EXTERN S16 wrUpdateRrcConnEstabTimeMeanMax(U32 estabCause,EpcTime rrcConReqRcvdTm,EpcTime rrcConCmplRcvdTm);
EXTERN  S16 wrUpdateErabEstabTimeMeanMax(U32 qciType,EpcTime ErabReqRcvdTm,EpcTime ErabEstabTm); 
EXTERN S16 wrIncKpiPredfndIntrvlTmrExpiry(Void);
EXTERN  S16 wrKpiResetKpiInfo(S16 evnt);
EXTERN Void IncFapKpiByIntVal(U16 Id, U32 incVal);
EXTERN Void IncFapKpiByRealVal(U16 Id,F32 incVal);
EXTERN Void KpisInfoSentNotification(Void);
EXTERN S16 wrSendKpisInfo(Void);

EXTERN S16 wrKpiStopKpiCollecPrcCfm(Void);
EXTERN S16 wrKpiStopKpiCollecPrc(Void);
EXTERN S16 wrKpiInitKpiInfo(S16 evnt, Bool l2CollecEna);
EXTERN S16 wrKpiStartKpiCollecPrc(Bool l2CollecEna);
EXTERN S16 wrKpiDInitKpiInfo(Void);
EXTERN S16 wrIncKpiIncHoSuccAftrReEst(Void);
EXTERN Void wrIncIratMobLai
(
 WrCellId  cellId,
 WrPlmnId  plmnId,
 U16       lac
);

#endif/*WR_RSYS_KPI*/

/*KPI FUNCTION to increment KPI*/
#ifdef WR_RSYS_KPI
#define WR_INC_KPI_FUNC(_FUNC_NAME) _FUNC_NAME()
#define WR_INC_KPI_FUNC1(_FUNC_NAME,_arg1) _FUNC_NAME(_arg1)
#define WR_INC_KPI_FUNC2(_FUNC_NAME,_arg1,_arg2) _FUNC_NAME(_arg1,_arg2)
#define WR_INC_KPI_FUNC3(_FUNC_NAME,_arg1,_arg2,_arg3) _FUNC_NAME(_arg1,_arg2,_arg3)
#define WR_INC_KPI_FUNC4(_FUNC_NAME,_arg1,_arg2,_arg3,_arg4) _FUNC_NAME(_arg1,_arg2,_arg3,_arg4)
#define WR_INC_KPI_FUNC5(_FUNC_NAME,_arg1,_arg2,_arg3,_arg4,_arg5) _FUNC_NAME(_arg1,_arg2,_arg3,_arg4,_arg5)
#else /* if not defined WR_RSYS_KPI */
#define WR_INC_KPI_FUNC(_FUNC_NAME)
#define WR_INC_KPI_FUNC1(_FUNC_NAME,_arg1) 
#define WR_INC_KPI_FUNC2(_FUNC_NAME,_arg1,_arg2) 
#define WR_INC_KPI_FUNC3(_FUNC_NAME,_arg1,_arg2,_arg3)
#define WR_INC_KPI_FUNC4(_FUNC_NAME,_arg1,_arg2,_arg3,_arg4) 
#define WR_INC_KPI_FUNC5(_FUNC_NAME,_arg1,_arg2,_arg3,_arg4,_arg5) 
#endif/* WR_RSYS_KPI */

#ifdef WR_RSYS_KPI
#define INC_KPI_VALUE_BY_ONE      1  /*Increment KPI by one*/ 
#define NUM_RRC_CONN_ESTAB_CAUSE  6  /*Number of RRC CONN Estab Causes*/
#define WR_KPI_MAX_QCI            10 /*Maximum Value of QCI*/
#define MAX_QCI_VALUE             9  /*Maximum Value of QCI*/
#define WR_KPI_MAX_UES_PER_CELL   TENB_MAX_UE_SUPPORTED 
#define MAX_NUM_COLLECTION_PERIOD 4  /*maximum number of collection periods*/
#define MAX_NUM_CHAR              255/*Maximum length*/
#define TM_MULTIPLY              (60*1000) /*Adjusting timer val*/
#define WR_KPI_SET_TO_ZERO        0  
#define WR_KPI_SET_TO_ONE         1 
#define WR_KPI_INIT_KPI_INFO      0
#define WR_KPI_RESET_KPI_INFO     1
#define WR_MEAS_PDCP_CFG          0x1
#define WR_MEAS_RLC_UULOSS_CFG    0x2
#define WR_MEAS_RLC_DLULIP_CFG    0x4
#define WR_MEAS_MAC_CFG           0x8
#define WR_MEAS_CFG_DONE         (WR_MEAS_RLC_UULOSS_CFG | WR_MEAS_RLC_DLULIP_CFG | WR_MEAS_PDCP_CFG | WR_MEAS_MAC_CFG)
#define WR_KPI_MAX_NUM_MBLTY_LAI         32 /* Max Number of Mobility LAI */
#endif /*WR_RSYS_KPI*/
    
#ifdef WR_RSYS_KPI

/*Kpi value types*/
typedef enum  __kpiValueType
{  
   KPI_VALUE_TYPE_INTEGER,  
   KPI_VALUE_TYPE_REAL,
   KPI_VALUE_TYPE_INVALID   
}KpiValueType;
#endif /*WR_RSYS_KPI*/

/*This macro calls oam api to increment KPI*/
#ifdef WR_RSYS_KPI 
#ifdef WR_RSYS_OAM
#define WR_INC_KPI(__KPI_ID,__KPI_VAL_TYPE, __VAL) {\
   if (__KPI_VAL_TYPE == KPI_VALUE_TYPE_INTEGER) \
   {\
      IncFapKpiByIntVal(__KPI_ID, __VAL);\
   }\
   else \
   {  \
      IncFapKpiByRealVal(__KPI_ID, __VAL);\
     /*Add function here for handling Real Type kpi*/ \
   }\
}

#define WR_INC_KPI_INST(__KPI_ID,__KPI_VAL_TYPE, __VAL, __KPI_INST_INFO) {\
   if (__KPI_VAL_TYPE == KPI_VALUE_TYPE_INTEGER) \
   {\
      IncFapKpiByIntValMInst(__KPI_ID, __VAL,__KPI_INST_INFO);\
   }\
   else \
   {  \
      IncFapKpiByRealValMInst(__KPI_ID, __VAL,__KPI_INST_INFO);\
   }\
}

#else /* WR_RSYS_OAM not defined */
/* place holder for non RSYS OAM KPI counter collecter */
#define WR_INC_KPI(__KPI_ID,__KPI_VAL_TYPE, __VAL) {\
}
#define WR_INC_KPI_INST(__KPI_ID,__KPI_VAL_TYPE, __VAL, __KPI_INST_INFO) {\
}
#endif /*WR_RSYS_OAM*/
#endif /*WR_RSYS_KPI*/

/*This macro calculates MAX*/
#ifdef WR_RSYS_KPI
#define WR_CAL_MAX(_PREV_VAL,_CURR_VAL)\
{\
if((*_PREV_VAL) < _CURR_VAL)\
{\
  (*_PREV_VAL) = _CURR_VAL;\
}\
}

/*This macro calcualtes Mean*/
#define WR_CAL_MEAN(_PREV_VAL,_CURR_VAL)\
{\
if((*_PREV_VAL) == 0)\
{\
  (*_PREV_VAL) = _CURR_VAL;\
}\
else\
{\
  (*_PREV_VAL) = ((*_PREV_VAL) + _CURR_VAL)>>1;\
}\
}

/*This macro calcualtes SUM of all the RRC Connection setup time */
#define WR_CAL_SUM(_PREV_VAL,_CURR_VAL)\
{\
  (*_PREV_VAL) = ((*_PREV_VAL) + _CURR_VAL);\
}

#define WR_PRMBL_DIV_ROUND(_PRMBL, _TIME_PRD)\
((_PRMBL) + (_TIME_PRD)/2) / (_TIME_PRD)

#endif/*WR_RSYS_KPI*/

/*This strucutres requires to udpate Max/Mean KPI*/
#ifdef WR_RSYS_KPI
#ifdef LTE_L2_MEAS
/*This structure contains Ue info required for L2 MEAS*/
typedef struct wrKpiUeInfo
{
   U8 cellId;
   U8 ueId;
   U8 qci[WR_KPI_MAX_QCI];
   U8 numQcis;
}WrKpiUeInfo;
#endif/*LTE_L2_MEAS*/
/*This strucutres requires to udpate Max/Mean KPI*/
typedef struct rrcConMeanInfo
{
   U32 numOfRrcCon;       /*with in timer predefined interval*/
   U32 numOfRrcConEmergency;
   U32 numOfRrcConHighPriorAcc;
   U32 numOfRrcConMtAcc;
   U32 numOfRrcConMoSig;
   U32 numOfRrcConMoData;
   U32 numOfRrcConDelTolAcc;
   U32 meanOfNumOfRrcCon; /*Mean of number of rrc conn*/ 
}RrcConMeanInfo;

typedef struct rrcConnEstabTimeMeanInfo
{
   U32 emergency;
   U32 highPriorityAccess;
   U32 mtAccess;
   U32 moSignalling;
   U32 moData;
   U32 delayTolAccess;
}RrcConnEstabTimeMeanInfo;

/*This strucutre contains Mean of number of Existing Erab */
typedef struct erabMeanInfo{
   U32 numOfEarb;       /*Number of Erab in a predefined interval */
   U32 meanOfNumOfErab; /*Mean of number Earbs*/
}ErabMeanInfo;

/* CSG_DEV Start*/

/*This strucutre contains Mean of number of Existing Erab */
typedef struct csgUeMeanInfo{
   U32 numOfCsgUe;       /*Number of Attached UE in a predefined interval */
   U32 meanOfNumOfCsgUe; /*Mean of number UE*/
}CsgUeMeanInfo;

/* CSG_DEV End */
/*This strucuter contains the Mean info related to procedures*/
typedef struct wrKpiMeanInfo
{
   RrcConMeanInfo           rrcConnMean;         /*Mean of number of rrc connection*/
   RrcConnEstabTimeMeanInfo rrcConnEstabTimeSum;/*Mean of time taken in rrc conn estab*/
   U32    erabEstabTimeMeanPrQci[MAX_QCI_VALUE]; /*Mean of time taken in earb estab*/
   /* ccpu00149628 */
   U32                      noOfSuccEstERabs[MAX_QCI_VALUE]; 
   /* ccpu00149628 */
   ErabMeanInfo             erabMean;            /*Mean of number of erabs*/
   CsgUeMeanInfo            csgUeMean;           /* CSG_DEV:Mean of CSG Attached Ue */ 
}WrKpiMeanInfo;

/*This structure contains the Maximum rrc connection*/
typedef struct rrcConMaxInfo
{
   U32 numOfRrcCon;     /*with in timer predefined interval*/
   U32 maxNmOfRrcCon;   /*Max number of rrc conn in a granularity period*/ 
}RrcConMaxInfo;

/*This strucutre contains  Mean of Rrc Connection Estab time per estab cause*/
typedef struct rrcConnEstabTimeMaxInfo
{
   U32 emergency;
   U32 highPriorityAccess;
   U32 mtAccess;
   U32 moSignalling;
   U32 moData;
   U32 delayTolAccess;
}RrcConnEstabTimeMaxInfo;

typedef struct erabMaxInfo
{
   U32 numOfErab;       /*number of erab within a specified interval*/
   U32 maxOfNumOfErab;  /*max of number of Erab*/
}ErabMaxInfo;

/*This strcutre contains Max info related to  procedures*/
typedef struct wrKpiMaxInfo
{
   RrcConMaxInfo           rrcConnMax;            /*Max number of Rrc Connection */
   RrcConnEstabTimeMaxInfo rrcConnEstabTimeMax;   /*Max time taken in rrc Connection*/
   U32       erabEstabTimeMaxPrQci[MAX_QCI_VALUE];/*Max time taken in earb estab*/
   ErabMaxInfo             erabMax;               /*Max number of erab*/
}WrKpiMaxInfo;
typedef struct wrKpiInfo
{
   Bool            startCollec;           /*Start Kpi Collection*/
   U32             kpiMeasPrd;            /*Kpi Collection Period*/
   EpcTime         startTime;             /*start time*/
   EpcTime         endTime;               /*end time*/ 
   WrKpiMeanInfo   kpiMeanInfo;           /*Kpi Info contains Means*/ 
   WrKpiMaxInfo    kpiMaxInfo;            /*Kpi Info contains Max*/
   CmTimer         kpiPredfndIntrvlTmr;   /*Predfined interval timer*/
   U32             kpiPredfndIntrvlTmrVal;/*Predefined interval timer val*/
   Bool            kpiCollectionTmrExpry; /*Collection Timer Expire?*/
   Bool            l2CollecEna;           /*Enabling/Disabling of L2 Counters*/
#ifdef LTE_L2_MEAS
   U8              numOfSmpls;            /*For Active Ue*/
   Bool            qciMatch[WR_KPI_MAX_QCI];/*QCi values configured*/
   WrKpiUeInfo     ueInfoLst[WR_KPI_MAX_UES_PER_CELL]; /*For Ul/Dl Ip thrpught*/
   U8              numUes;                /*For Ul/Dl Ip throughput*/
   U8              transId;               /*used to identify L2 meas req*/
   U8              numOfTrggrL2MeasReq;   /*Number of L2 Meas Reqeust sent*/
   U8              numOfRcvdL2MeasCfm;    /*Number of L2 Meas Cfm received */
   U8              numOfL2MeasStopReq;
   U8              numOfL2MeasStopCfm;
   U8              measConfigRecv;        /*Indicates the RLC/PDCP/MAC confirmations*/ 
#endif/* LTE_L2_MEAS */ 
}WrKpiInfo;

EXTERN WrKpiInfo kpiInfo;
#endif /*WR_RSYS_KPI*/ 

/*  Multi KPI types */
typedef enum
{
   KP_INST_TYPE_NONE =0,
   KP_INST_TYPE_ECGI,
   KP_INST_TYPE_NONE_INVALID
}KpiInstType;

/* Ecgi def for per tgt cell counters*/
typedef struct kpiInstTypeeCgi
{
   U8   numMncDigs;
   U8   mcc[3];
   U8   mnc[3];
   U32  cellId;
}KpiInstTypeeCgi;
/* Used for multi-instance KPI's  */
typedef struct kpiInstInfo
{
   KpiInstType kpiInstType;
   union 
   {
      KpiInstTypeeCgi  eCgi;             /* used for store ecgi in case of per cell counter */
   } u;

}KpiInstInfo; 

#ifdef WR_RSYS_KPI
EXTERN Void IncFapKpiByIntValMInst(U32 Id, U32  incVal, KpiInstInfo mInfoVal);
EXTERN Void IncFapKpiByRealValMInst(U32 Id, F32 incVal, KpiInstInfo mInfoVal);
#endif

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif/* __QY_KPI_H__*/

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
