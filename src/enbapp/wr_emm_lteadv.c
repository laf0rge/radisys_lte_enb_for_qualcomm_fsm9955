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

     Name:     EnodeB LTE-Advanced Application

     Type:     C source file

     Desc:     This file contains 
               

     File:     wr_emm_lteadv.c

     Sid:      fc_emm_lteadv.c@@/main/TeNB_Main_BR/5 - Mon Aug 11 16:51:41 2014

     Prg:      skulal

*********************************************************************21*/

/* LTE_ADV starts */
static const char* RLOG_MODULE_NAME="EMM";
static int RLOG_MODULE_ID=1;
static int RLOG_FILE_ID=11;

/* header include files (.h) */
#include "wr_emm.h"
#include "wr_cmn.h"
#include "wr_emm_lteadv.h"
#include "wr_umm_lteadv.h"
#include "czt_asn.h"
#include "czt_asn.x"
#include "wr_umm_msg.h"
#include "wr_ifm_x2ap.h"
#include "wr_ifm_schd.h"

PRIVATE S16 wrEmmDsfrPrcLoadInfo(U32 peerId, CztEvnt *pdu);
PRIVATE S16 wrEmmNghEnbSndLoadInfo(WrNeighEnbCb *nghEnbCb, RgrLoadInfIndInfo *loadInfInd);

/** @brief This function fill plmn id into X2AP message IE
 *
 * @details 
 *
 *     Function: wrEmmLteAdvCztFillPLMNId
 *
 *         Processing steps:
 *
 *
 * @param[in]   pdu    : Pointer to X2AP message
 * @param[in]   plmnId : Pointer to WR plmn id 
 * @param[out]  plmnIe : Pointer to X2AP IE to be filled
 * @return S16
 *     -# Success : ROK
 *     -# Failure : RFAILED
 */
PRIVATE S16 wrEmmLteAdvCztFillPLMNId
(                     
CztEvnt                      *pdu,
WrPlmnId                     *plmnId,
CztPLMN_Identity             *plmnIe
)
{
   U8      pLMNId[3];

   TRC3(wrEmmLteAdvCztFillPLMNId)
   
   WR_GET_MEM(pdu, WR_PLMNID_IE_LEN, &(plmnIe->val));
   plmnIe->pres = PRSNT_NODEF;
   plmnIe->len  = WR_PLMNID_IE_LEN;

   pLMNId[0] =(U8)(((plmnId->mcc[1])<<4) | (plmnId->mcc[0])); /* KW fix for LTE_ADV */
   if (plmnId->numMncDigits == 2)
   {
      pLMNId[1] =(U8)(((0xf0) | (plmnId->mcc[2])));    /* KW fix for LTE_ADV */  
      pLMNId[2] =(U8)(((plmnId->mnc[1])<<4) | (plmnId->mnc[0])); /* KW fix for LTE_ADV */  
   }
   else
   {
      pLMNId[1] =(U8)((((plmnId->mnc[0])<<4) | (plmnId->mcc[2])));     /* KW fix for LTE_ADV */     
      pLMNId[2] =(U8)((((plmnId->mnc[2])<<4) | (plmnId->mnc[1])));      /* KW fix for LTE_ADV */     
   }
   wrFillTknStrOSXL(plmnIe,WR_PLMNID_IE_LEN,pLMNId,&pdu->memCp);
   RETVALUE(ROK);
}

/** @brief This function convert X2AP plmn id to WR plmn id
 *
 * @details 
 *
 *     Function: 
 *
 *         Processing steps:
 *
 *
 * @param[in]  plmnIe : Pointer to X2AP IE
 * @param[out] plmnId : Pointer to WR plmn id to be filled
 * @return S16
 *     -# Success : ROK
 *     -# Failure : RFAILED
 */
PRIVATE S16 wrEmmLteAdvNghEnbParsePlmnIe
(
CztPLMN_Identity             *plmnIe,
WrPlmnId                     *plmnId
)
{
   TRC3(wrEmmLteAdvNghEnbParsePlmnIe)
   plmnId->mcc[0]          = (U8)(plmnIe->val[0] & 0x0f); /* KW fix for LTE_Adv */ 
   plmnId->mcc[1]          = (U8)((plmnIe->val[0] & 0xf0) >> 4);      /* KW fix for LTE_Adv */    
   plmnId->mcc[2]          = (U8)(plmnIe->val[1] & 0x0f);   /* KW fix for LTE_Adv */ 
   if ((plmnIe->val[1] & 0xf0) == 0xf0)
   {
      plmnId->numMncDigits = 2;
      plmnId->mnc[1]       = (U8)((plmnIe->val[2] & 0xf0) >> 4);    /* KW fix for LTE_Adv */  
      plmnId->mnc[0]       = (U8)(plmnIe->val[2] & 0x0f);    /* KW fix for LTE_Adv */  
      plmnId->mnc[2]       = 0;    /* gcc fix for LTE_Adv */  
   }
   else
   {
      plmnId->numMncDigits = 3;
      plmnId->mnc[0]       = (U8)((plmnIe->val[1] & 0xf0) >> 4);     /* KW fix for LTE_Adv */  
      plmnId->mnc[1]       = (U8)(plmnIe->val[2] & 0x0f);             /* KW fix for LTE_Adv */  
      plmnId->mnc[2]       = (U8)((plmnIe->val[2] & 0xf0) >> 4);       /* KW fix for LTE_Adv */  
   }
   RETVALUE(ROK);
}

/* LTE_ADV_ABS starts */
/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/
/*%                        EMM ABS MODULE - START                    %*/
/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/

/* Syntax of the ABS event handlers */
typedef S16 (*ABSEvntHndlr) (WrEmmAbsEvent *evnt);

/*********************************************************************/
/*          LTE-ADVANCED MODULE FUNCTION DECLARATION                 */
/*********************************************************************/

PRIVATE S16 wrEmmAbsStaticProcessEvent (WrEmmAbsEvent *evnt);
PRIVATE S16 wrEmmAbsDynamicProcessEvent (WrEmmAbsEvent *evnt);

PRIVATE S16 wrEmmAbsHndlAggrCellInit (WrEmmAbsEvent *evnt);
PRIVATE S16 wrEmmAbsHndlAggrLoadInfo (WrEmmAbsEvent *evnt);
PRIVATE S16 wrEmmAbsHndlAggrRsrcStsRsp (WrEmmAbsEvent *evnt);
PRIVATE S16 wrEmmAbsHndlAggrRsrcStsFlr (WrEmmAbsEvent *evnt);
PRIVATE S16 wrEmmAbsHndlAggrRsrcStsUpd (WrEmmAbsEvent *evnt);
PRIVATE S16 wrEmmAbsHndlAggrX2DownInd (WrEmmAbsEvent *evnt);
PRIVATE S16 wrEmmAbsHndlAggrCellUpInd(WrEmmAbsEvent *evnt);
PRIVATE S16 wrEmmAbsHndlAggrCellDownInd (WrEmmAbsEvent *evnt);
PRIVATE S16 wrEmmAbsHndlAggrSchLoadInd (WrEmmAbsEvent *evnt);
PRIVATE S16 wrEmmAbsHndlAggrAbsDisableInd (WrEmmAbsEvent *evnt);
PRIVATE S16 wrEmmAbsHndlAggrAbsEnableInd (WrEmmAbsEvent *evnt);
PRIVATE S16 wrEmmAbsHndlAggrLoadInfoInAbsDisable (WrEmmAbsEvent *evnt);
PRIVATE S16 wrEmmAbsHndlAggrErrorInd (WrEmmAbsEvent *evnt);

PRIVATE S16 wrEmmAbsHndlVctmCellInit (WrEmmAbsEvent *evnt);
PRIVATE S16 wrEmmAbsHndlVctmAbsUeInd (WrEmmAbsEvent *evnt);
PRIVATE S16 wrEmmAbsHndlVctmNonAbsUeInd (WrEmmAbsEvent *evnt);
PRIVATE S16 wrEmmAbsHndlVctmLoadInfo (WrEmmAbsEvent *evnt);
PRIVATE S16 wrEmmAbsHndlVctmRsrcStsReq (WrEmmAbsEvent *evnt);
PRIVATE S16 wrEmmAbsHndlVctmX2UpInd (WrEmmAbsEvent *evnt);
PRIVATE S16 wrEmmAbsHndlVctmX2DownInd (WrEmmAbsEvent *evnt);
PRIVATE S16 wrEmmAbsHndlVctmCellUpInd (WrEmmAbsEvent *evnt);
PRIVATE S16 wrEmmAbsHndlVctmCellDownInd (WrEmmAbsEvent *evnt);
PRIVATE S16 wrEmmAbsHndlVctmSchLoadInd (WrEmmAbsEvent *evnt);
PRIVATE S16 wrEmmAbsHndlVctmAbsDisableInd (WrEmmAbsEvent *evnt);
PRIVATE S16 wrEmmAbsHndlVctmAbsEnableInd(WrEmmAbsEvent *evnt);
PRIVATE S16 wrEmmAbsHndlVctmUeRelInd (WrEmmAbsEvent *evnt);
PRIVATE S16 wrEmmAbsHndlVctmErrorInd (WrEmmAbsEvent *evnt);
PRIVATE S16 wrEmmAbsHndlVctmTimerExpiryInd (WrEmmAbsEvent *evnt);
PRIVATE S16 wrEmmAbsHndlVctmRsrcStsReqInAbsDisable (WrEmmAbsEvent *evnt);

PRIVATE S16 wrEmmAbsHndlInvEvent (WrEmmAbsEvent *evnt);
PRIVATE Void wrEmmAbsReleaseMem (WrEmmAbsEvent *evnt);

PRIVATE S16 wrEmmAbsAggrRemoveNDeallocPeerCellCb (WrEmmAbsAggrPeerEnbCb *peerEnbCb,
                                                  WrEmmAbsAggrPeerCellCb *peerCellCb);

PRIVATE S16 wrEmmAbsVctmGetPeerEnbCb (WrEmmAbsEnbId *enbId, WrEmmAbsVctmPeerEnbCb **peerEnbCb);
PRIVATE S16 wrEmmAbsVctmGetPeerCellCb (WrEmmAbsVctmPeerEnbCb *enbCb, WrEmmAbsCellId *cellId, 
                                       WrEmmAbsVctmPeerCellCb **peerCellCb);
PRIVATE S16 wrEmmAbsVctmAddCellWeightage (WrEmmAbsVctmPeerCellCb *peerCellCb, U32 count); /*KW fix for LTE_ADV */
PRIVATE S16 wrEmmAbsVctmRemoveCellWeightage (WrEmmAbsVctmPeerCellCb *peerCellCb, U32 count);      /*KW fix for LTE_ADV */ 
PRIVATE S16 wrEmmAbsVctmAddAbsWeightage (WrEmmAbsVctmPatternCb *patternCb, U32 count);     /*KW fix for LTE_ADV */    
PRIVATE S16 wrEmmAbsVctmRemoveAbsWeightage (WrEmmAbsVctmPatternCb *patternCb, U32 count);  /*KW fix for LTE_ADV */     


/*********************************************************************/
/*           ABS MODULE FSM FUNCTION REGISTRATION                    */
/*********************************************************************/

PRIVATE ABSEvntHndlr absAggrEvntHndlr[WR_EMM_ABS_MAX_STATE][WR_EMM_ABS_MAX_EVENT] =
{
   /* WR_EMM_ABS_DISABLED_STATE */
   {
      wrEmmAbsHndlAggrCellInit,      /* WR_EMM_ABS_CELL_INIT_EVENT */         
      wrEmmAbsHndlInvEvent,          /* WR_EMM_ABS_ABS_UE_EVENT */            
      wrEmmAbsHndlInvEvent,          /* WR_EMM_ABS_NONABS_UE_EVENT */         
      wrEmmAbsHndlAggrLoadInfoInAbsDisable,  /* WR_EMM_ABS_LOAD_INFO_RCVD_EVENT */    
      wrEmmAbsHndlInvEvent,          /* WR_EMM_ABS_RSRC_STS_REQ_RCVD_EVENT */ 
      wrEmmAbsHndlInvEvent,          /* WR_EMM_ABS_RSRC_STS_RSP_RCVD_EVENT */ 
      wrEmmAbsHndlInvEvent,          /* WR_EMM_ABS_RSRC_STS_FLR_RCVD_EVENT */ 
      wrEmmAbsHndlInvEvent,          /* WR_EMM_ABS_RSRC_STS_UPD_RCVD_EVENT */ 
      wrEmmAbsHndlInvEvent,          /* WR_EMM_ABS_X2AP_UP_IND_EVENT */       
      wrEmmAbsHndlAggrX2DownInd,     /* WR_EMM_ABS_X2AP_DOWN_IND_EVENT */     
      wrEmmAbsHndlInvEvent,          /* WR_EMM_ABS_CELL_UP_IND_EVENT */       
      wrEmmAbsHndlInvEvent,          /* WR_EMM_ABS_CELL_DOWN_IND_EVENT */     
      wrEmmAbsHndlInvEvent,          /* WR_EMM_ABS_SCH_LOAD_IND_EVENT */      
      wrEmmAbsHndlAggrAbsEnableInd,  /* WR_EMM_ABS_ENABLED_EVENT */           
      wrEmmAbsHndlInvEvent,          /* WR_EMM_ABS_DISABLED_EVENT */          
      wrEmmAbsHndlInvEvent,          /* WR_EMM_ABS_UE_REL_IND_EVENT */        
      wrEmmAbsHndlInvEvent,          /* WR_EMM_ABS_TIMER_EVENT */
      wrEmmAbsHndlInvEvent           /* WR_EMM_ABS_ERROR_IND_EVENT */
   },
   /* WR_EMM_ABS_ENABLED_STATE */     
   {
      wrEmmAbsHndlAggrCellInit,      /* WR_EMM_ABS_CELL_INIT_EVENT */         
      wrEmmAbsHndlInvEvent,          /* WR_EMM_ABS_ABS_UE_EVENT */            
      wrEmmAbsHndlInvEvent,          /* WR_EMM_ABS_NONABS_UE_EVENT */         
      wrEmmAbsHndlAggrLoadInfo,      /* WR_EMM_ABS_LOAD_INFO_RCVD_EVENT */    
      wrEmmAbsHndlInvEvent,          /* WR_EMM_ABS_RSRC_STS_REQ_RCVD_EVENT */ 
      wrEmmAbsHndlAggrRsrcStsRsp,    /* WR_EMM_ABS_RSRC_STS_RSP_RCVD_EVENT */ 
      wrEmmAbsHndlAggrRsrcStsFlr,    /* WR_EMM_ABS_RSRC_STS_FLR_RCVD_EVENT */ 
      wrEmmAbsHndlAggrRsrcStsUpd,    /* WR_EMM_ABS_RSRC_STS_UPD_RCVD_EVENT */ 
      wrEmmAbsHndlInvEvent,          /* WR_EMM_ABS_X2AP_UP_IND_EVENT */       
      wrEmmAbsHndlAggrX2DownInd,     /* WR_EMM_ABS_X2AP_DOWN_IND_EVENT */     
      wrEmmAbsHndlAggrCellUpInd,     /* WR_EMM_ABS_CELL_UP_IND_EVENT */       
      wrEmmAbsHndlAggrCellDownInd,   /* WR_EMM_ABS_CELL_DOWN_IND_EVENT */     
      wrEmmAbsHndlAggrSchLoadInd,    /* WR_EMM_ABS_SCH_LOAD_IND_EVENT */      
      wrEmmAbsHndlInvEvent,          /* WR_EMM_ABS_ENABLED_EVENT */           
      wrEmmAbsHndlAggrAbsDisableInd, /* WR_EMM_ABS_DISABLED_EVENT */          
      wrEmmAbsHndlInvEvent,          /* WR_EMM_ABS_UE_REL_IND_EVENT */        
      wrEmmAbsHndlInvEvent,          /* WR_EMM_ABS_TIMER_EVENT */
      wrEmmAbsHndlAggrErrorInd       /* WR_EMM_ABS_ERROR_IND_EVENT */
   }
};

/* State Machine for Pico eNodeB */
PRIVATE ABSEvntHndlr absVctmEvntHndlr[WR_EMM_ABS_MAX_STATE][WR_EMM_ABS_MAX_EVENT] =
{
   /* WR_EMM_ABS_DISABLED_STATE */
   {
      wrEmmAbsHndlVctmCellInit,          /* WR_EMM_ABS_CELL_INIT_EVENT */         
      wrEmmAbsHndlInvEvent,          /* WR_EMM_ABS_ABS_UE_EVENT */            
      wrEmmAbsHndlInvEvent,          /* WR_EMM_ABS_NONABS_UE_EVENT */         
      wrEmmAbsHndlInvEvent,          /* WR_EMM_ABS_LOAD_INFO_RCVD_EVENT */    
      wrEmmAbsHndlVctmRsrcStsReqInAbsDisable,  /* WR_EMM_ABS_RSRC_STS_REQ_RCVD_EVENT */ 
      wrEmmAbsHndlInvEvent,          /* WR_EMM_ABS_RSRC_STS_RSP_RCVD_EVENT */ 
      wrEmmAbsHndlInvEvent,          /* WR_EMM_ABS_RSRC_STS_FLR_RCVD_EVENT */ 
      wrEmmAbsHndlInvEvent,          /* WR_EMM_ABS_RSRC_STS_UPD_RCVD_EVENT */ 
      wrEmmAbsHndlInvEvent,          /* WR_EMM_ABS_X2AP_UP_IND_EVENT */       
      wrEmmAbsHndlInvEvent,          /* WR_EMM_ABS_X2AP_DOWN_IND_EVENT */     
      wrEmmAbsHndlInvEvent,          /* WR_EMM_ABS_CELL_UP_IND_EVENT */       
      wrEmmAbsHndlInvEvent,          /* WR_EMM_ABS_CELL_DOWN_IND_EVENT */     
      wrEmmAbsHndlInvEvent,          /* WR_EMM_ABS_SCH_LOAD_IND_EVENT */      
      wrEmmAbsHndlVctmAbsEnableInd,  /* WR_EMM_ABS_ENABLED_EVENT */           
      wrEmmAbsHndlInvEvent,          /* WR_EMM_ABS_DISABLED_EVENT */          
      wrEmmAbsHndlInvEvent,          /* WR_EMM_ABS_UE_REL_IND_EVENT */        
      wrEmmAbsHndlInvEvent,          /* WR_EMM_ABS_TIMER_EVENT */
      wrEmmAbsHndlInvEvent           /* WR_EMM_ABS_ERROR_IND_EVENT */
   },
   /* WR_EMM_ABS_ENABLED_STATE */
   {
      wrEmmAbsHndlVctmCellInit,      /* WR_EMM_ABS_CELL_INIT_EVENT */
      wrEmmAbsHndlVctmAbsUeInd,      /* WR_EMM_ABS_ABS_UE_EVENT */
      wrEmmAbsHndlVctmNonAbsUeInd,   /* WR_EMM_ABS_NONABS_UE_EVENT */
      wrEmmAbsHndlVctmLoadInfo,      /* WR_EMM_ABS_LOAD_INFO_RCVD_EVENT */
      wrEmmAbsHndlVctmRsrcStsReq,    /* WR_EMM_ABS_RSRC_STS_REQ_RCVD_EVENT */
      wrEmmAbsHndlInvEvent,          /* WR_EMM_ABS_RSRC_STS_RSP_RCVD_EVENT */
      wrEmmAbsHndlInvEvent,          /* WR_EMM_ABS_RSRC_STS_FLR_RCVD_EVENT */
      wrEmmAbsHndlInvEvent,          /* WR_EMM_ABS_RSRC_STS_UPD_RCVD_EVENT */
      wrEmmAbsHndlVctmX2UpInd,       /* WR_EMM_ABS_X2AP_UP_IND_EVENT */
      wrEmmAbsHndlVctmX2DownInd,     /* WR_EMM_ABS_X2AP_DOWN_IND_EVENT */
      wrEmmAbsHndlVctmCellUpInd,     /* WR_EMM_ABS_CELL_UP_IND_EVENT */
      wrEmmAbsHndlVctmCellDownInd,   /* WR_EMM_ABS_CELL_DOWN_IND_EVENT */
      wrEmmAbsHndlVctmSchLoadInd,    /* WR_EMM_ABS_SCH_LOAD_IND_EVENT */
      wrEmmAbsHndlInvEvent,          /* WR_EMM_ABS_ENABLED_EVENT */
      wrEmmAbsHndlVctmAbsDisableInd, /* WR_EMM_ABS_DISABLED_EVENT */
      wrEmmAbsHndlVctmUeRelInd,       /* WR_EMM_ABS_UE_REL_IND_EVENT */
      wrEmmAbsHndlVctmTimerExpiryInd,/* WR_EMM_ABS_TIMER_EVENT */
      wrEmmAbsHndlVctmErrorInd       /* WR_EMM_ABS_ERROR_IND_EVENT */
   }
};

PRIVATE ABSEvntHndlr absEvntHndlr[WR_EMM_ABS_MAX_NEG_TYPE] = 
{
   wrEmmAbsStaticProcessEvent,       /* WR_EMM_ABS_STATIC_NEG_TYPE */
   wrEmmAbsDynamicProcessEvent       /* WR_EMM_ABS_DYNAMIC_NEG_TYPE */
};

/*********************************************************************/
/*           ABS MODULE FSM FUNCTION DEFINATION                      */
/*********************************************************************/

/** @brief This function receives event for ABS module
 *
 * @details 
 *
 *     Function: wrEmmAbsProcessEvent
 *
 *         Processing steps:
 *             1. Validate the event
 *             2. Calls the FSM of dynamic/static ABS pattern negotiation
 *
 *
 * @param[in]   evnt    : Pointer to WrEmmAbsEvent 
 * @return S16
 *     -# Success : ROK
 *     -# Failure : RFAILED
 */
PUBLIC S16 wrEmmAbsProcessEvent
(
WrEmmAbsEvent           *evnt
)
{
   TRC2(wrEmmAbsProcessEvent)

   if((evnt->eventId < 0) || (evnt->eventId >= WR_EMM_ABS_MAX_EVENT))
   {
      RLOG1(L_ERROR, " Invalid event(%d) received for ABS", evnt->eventId);
      RETVALUE(RFAILED);
   }

   RLOG3(L_INFO, " Event(%d) in State(%d) for Type(%d) received for ABS", 
                evnt->eventId, wrEmmCb.lteAdvCb.absInfo.state, wrEmmCb.lteAdvCb.absInfo.negType);

   absEvntHndlr[wrEmmCb.lteAdvCb.absInfo.negType](evnt);

   wrEmmAbsReleaseMem(evnt);

   RETVALUE(ROK);
} 
/*--------------------------------------------------------------------*/
/*            IMPLEMENTATION OF ABS MODULE UTILITIES                  */
/*--------------------------------------------------------------------*/

/** @brief This function releases memory allocated for WrEmmAbsEvent
 *
 * @details 
 *
 *     Function: wrEmmAbsReleaseMem
 *
 *         Processing steps:
 *
 *
 * @param[in]   evnt    : Pointer to WrEmmAbsEvent 
 * @return Void
 *
 */
PRIVATE Void wrEmmAbsReleaseMem
(
WrEmmAbsEvent                 *evnt
)
{
   TRC3(wrEmmAbsReleaseMem)

   switch(evnt->eventId)
   {
      case WR_EMM_ABS_CELL_INIT_EVENT:
      case WR_EMM_ABS_ABS_UE_EVENT:
      case WR_EMM_ABS_NONABS_UE_EVENT:
      case WR_EMM_ABS_LOAD_INFO_RCVD_EVENT:
      case WR_EMM_ABS_RSRC_STS_REQ_RCVD_EVENT:
      case WR_EMM_ABS_RSRC_STS_RSP_RCVD_EVENT:
      case WR_EMM_ABS_RSRC_STS_FLR_RCVD_EVENT:
      case WR_EMM_ABS_RSRC_STS_UPD_RCVD_EVENT:
      case WR_EMM_ABS_X2AP_UP_IND_EVENT:
      case WR_EMM_ABS_X2AP_DOWN_IND_EVENT:
      case WR_EMM_ABS_CELL_UP_IND_EVENT:
      case WR_EMM_ABS_CELL_DOWN_IND_EVENT:
      case WR_EMM_ABS_SCH_LOAD_IND_EVENT:
      case WR_EMM_ABS_ENABLED_EVENT:
      case WR_EMM_ABS_DISABLED_EVENT:
      case WR_EMM_ABS_UE_REL_IND_EVENT:
      {
         break;
      }
      default:
      {
         break;
      }
   }
   /* Currently dyanamic memory not allocate to event */
   /* WR_FREE(evnt, sizeof(WrEmmAbsEvent));*/
   RETVOID;
}

/** @brief This function converts X2AP cell id to WR cell id
 *
 * @details 
 *
 *     Function: wrEmmAbsGetCellId
 *
 *         Processing steps:
 *
 *
 * @param[in]   eUTRANcellIdentifier   : Pointer to X2AP cell id
 * @param[out]   cellId   : Pointer to WR cell id
 * @return S16
 *     -# Success : ROK
 *     -# Failure : RFAILED
 */
PRIVATE S16 wrEmmAbsGetCellId
(
U32                             *cellId,
CztEUTRANCellIdentifier         *eUTRANcellIdentifier
)
{
   U32                          eEUTRAN_CellId = 0;

   TRC3(wrEmmAbsGetCellId)

   eEUTRAN_CellId |= eUTRANcellIdentifier->val[0];
   eEUTRAN_CellId <<= 8;
   eEUTRAN_CellId |= eUTRANcellIdentifier->val[1];
   eEUTRAN_CellId <<= 8;
   eEUTRAN_CellId |= eUTRANcellIdentifier->val[2];
   eEUTRAN_CellId <<= 8;
   eEUTRAN_CellId |= eUTRANcellIdentifier->val[3];
   eEUTRAN_CellId >>= (32 - eUTRANcellIdentifier->len);
   *cellId = eEUTRAN_CellId;

   RETVALUE(ROK);
}

/*--------------------------------------------------------------------*/
/*        IMPLEMENTATION OF ABS MODULE UTILITIES(MACRO)               */
/*--------------------------------------------------------------------*/

/** @brief This function updates weightage of ABS pattern used by peer cell
 *
 * @details 
 *
 *     Function:wrEmmAbsAggrAddWeightage 
 *
 *         Processing steps:
 *              1. Add the weightage to ABS pattern based on the 
 *                 usable ABS pattern received from peer
 *
 *
 * @param[in]   peerCellCb   : Pointer to peer cell cb
 * @return S16
 *     -# Success : ROK
 *     -# Failure : RFAILED
 */
PRIVATE S16 wrEmmAbsAggrAddWeightage
(
WrEmmAbsAggrPeerCellCb      *peerCellCb
)
{
   WrEmmAbsAggrPatternCfg   *absPatternCfg = NULLP;
   WrCellCb                 *cellCb;

   TRC3(wrEmmAbsAggrAddWeightage)

   WR_GET_ABS_CELLCB(cellCb);
   if(cellCb != NULLP)
   {    
       absPatternCfg = &cellCb->lteAdvCb.absInfo.u.aggrInfo.absPatternCfgTable.
           absPatternCfg[peerCellCb->usablePatternId];
       if((absPatternCfg != NULLP) && (0 != absPatternCfg->absPercentage))  
       {

           absPatternCfg = &cellCb->lteAdvCb.absInfo.u.aggrInfo.absPatternCfgTable.
               absPatternCfg[peerCellCb->usablePatternId];
           if(0 != absPatternCfg->absPercentage)
           {
              absPatternCfg->absWeightage +=
                  peerCellCb->absPercentageRsrcUsed * absPatternCfg->absPercentage;
           }
       }
   }   
   RETVALUE(ROK);
}

/** @brief This function updates weightage of ABS pattern used by peer cell
 *
 * @details 
 *
 *     Function:wrEmmAbsAggrRemoveWeightage 
 *
 *         Processing steps:
 *              1. Remove the weightage to ABS pattern based on the 
 *                 usable ABS pattern received from peer
 *
 *
 * @param[in]   peerCellCb   : Pointer to peer cell cb
 * @return S16
 *     -# Success : ROK
 *     -# Failure : RFAILED
 */
PRIVATE S16 wrEmmAbsAggrRemoveWeightage
(
WrEmmAbsAggrPeerCellCb       *peerCellCb
)
{
   WrCellCb                  *cellCb;
   WrEmmAbsAggrPatternCfg    *absPatternCfg;

   TRC3(wrEmmAbsAggrRemoveWeightage)

   WR_GET_ABS_CELLCB(cellCb);

   if(cellCb != NULLP)
   {
       absPatternCfg = &cellCb->lteAdvCb.absInfo.u.aggrInfo.absPatternCfgTable.
           absPatternCfg[peerCellCb->usablePatternId];
       if((absPatternCfg != NULLP ) && (0 != absPatternCfg->absPercentage))
       {

           absPatternCfg->absWeightage -=
               peerCellCb->absPercentageRsrcUsed * absPatternCfg->absPercentage;
           peerCellCb->absPercentageRsrcUsed = 0;
       }
       RETVALUE(ROK);      
   }    /* KW fix for LTE_ADV */
   else
   {
       RETVALUE(RFAILED);
   }
}

/** @brief This function selects ABS pattern based on its weightage
 *
 * @details 
 *
 *     Function: wrEmmAbsAggrSelectAbsPattern
 *
 *         Processing steps:
 *              1. Selects an ABS pattern with height weightage 
 *                 within the available free resource. 
 *              2. If new ABS pattern is selected then data base is updated
 *
 * @param[in]   Void
 * @return S16
 *     -# Success : ROK
 *     -# Failure : RFAILED
 */
PRIVATE S16 wrEmmAbsAggrSelectAbsPattern
(
WrEnumAbsPatternSelectFlrCause  *cause
)
{
   U8                            idx;
   U8                            absPatternId = 0;
   U32                           maxAbsPatternWeightage  = 0;
   WrCellCb                      *cellCb;
   WrEmmAbsAggrPatternCfgTable   *absPatternCfgTable;

   TRC3(wrEmmAbsAggrSelectAbsPattern)

   WR_GET_ABS_CELLCB(cellCb);
   if((cellCb == NULLP) || (0 == wrEmmCb.lteAdvCb.absInfo.peerInfo.u.aggrInfo.absPeerEnbLstCp.count))
   {
       RLOG0(L_ERROR, " CELLCB not found ");
       *cause = WR_EMM_ABS_NO_PATTERN; 
       RETVALUE(RFAILED);   
   }  /*KW fix for LTE_ADV */  

   absPatternCfgTable = &cellCb->lteAdvCb.absInfo.u.aggrInfo.absPatternCfgTable;

   /* Initialize to currently used abs pattern */
   absPatternId = cellCb->lteAdvCb.absInfo.u.aggrInfo.absPatternId;

   /* If current id is invalid go for first valid ABS Pattern */
   if(absPatternId == 0)
   {
      absPatternId = 1;
   }

   maxAbsPatternWeightage = absPatternCfgTable->absPatternCfg[absPatternId].absWeightage;

   for(idx = 0; idx < absPatternCfgTable->absPatternCount; idx++)
   {
      if(absPatternCfgTable->absPatternCfg[idx].absPercentage <=
                             cellCb->lteAdvCb.absInfo.u.aggrInfo.absTotalRsrcAvail)
      {
#ifdef AIRSPAN_LTE_ADV_ABS_MULTIPLE_MACRO_1_PICO         
         if(absPatternCfgTable->absPatternCfg[idx].absWeightage > maxAbsPatternWeightage)
         {
            maxAbsPatternWeightage = absPatternCfgTable->absPatternCfg[idx].absWeightage;
            absPatternId           = idx;
         }
#else
         absPatternId           = idx;
#endif /* AIRSPAN_LTE_ADV_ABS_MULTIPLE_MACRO_1_PICO */
      }
      else
      {
         break;
      }
   }

   /* If ABS Percentage is zero then it is considered as failure */
   if(0 == absPatternCfgTable->absPatternCfg[absPatternId].absPercentage)
   {
      *cause = WR_EMM_ABS_NULL_PATTERN;
      RETVALUE(RFAILED);
   }

   if(absPatternId != cellCb->lteAdvCb.absInfo.u.aggrInfo.absPatternId)
   {
      cmMemcpy(cellCb->lteAdvCb.absInfo.absPattern,
           absPatternCfgTable->absPatternCfg[absPatternId].absPattern, WR_EMM_ABS_PATTERN_SIZE);
      cellCb->lteAdvCb.absInfo.u.aggrInfo.absPatternId = absPatternId;

      RLOG3(L_DEBUG, " Selected ABS Pattern:[0x%x, 0x%x, 0x%x,] ",
                       cellCb->lteAdvCb.absInfo.absPattern[0],
                       cellCb->lteAdvCb.absInfo.absPattern[1],                                                  
                       cellCb->lteAdvCb.absInfo.absPattern[2]);                                                  
      RLOG3(L_DEBUG, " Selected ABS Pattern:[0x%x, 0x%x] Weightage:[0x%lx] ", 
                       cellCb->lteAdvCb.absInfo.absPattern[3],                                                  
                       cellCb->lteAdvCb.absInfo.absPattern[4],
                       absPatternCfgTable->absPatternCfg[absPatternId].absWeightage);                        
      RETVALUE(ROK);
   }
   else
   {
      *cause = WR_EMM_ABS_DUP_PATTERN;
      RETVALUE(RFAILED);
   }
}

/** @brief This function finds ABS pattern id for given ABS pattern
 *
 * @details 
 *
 *     Function: wrEmmAbsAggrFndPatternId
 *
 *         Processing steps:
 *
 *
 * @param[in]   absPattern     : Pointer to ABS Pattern
 * @param[out]  absPatternId   : Id of ABS Pattern
 * @return S16
 *     -# Success : ROK
 *     -# Failure : RFAILED
 */
PRIVATE S16 wrEmmAbsAggrFndPatternId
(
U8                               *absPattern,
U8                               *absPatternId
)
{
   U8                            idx;
   WrCellCb                      *cellCb;
   WrEmmAbsAggrPatternCfgTable   *absPatternCfgTable;

   TRC3(wrEmmAbsFndPatternId)

   WR_GET_ABS_CELLCB(cellCb);
   /* KWork fix */
   if(cellCb == NULLP)
   {
      RLOG0(L_ERROR, " CellCb not found ");
      RETVALUE(RFAILED);
   }
   absPatternCfgTable = &cellCb->lteAdvCb.absInfo.u.aggrInfo.absPatternCfgTable;
   /* KWork fix */
   if(absPatternCfgTable == NULLP)
   {
      RLOG0(L_ERROR, " ABS pattern cfg table not found ");
      RETVALUE(RFAILED);
   }
   *absPatternId      = 0;

   for(idx = 0; idx < absPatternCfgTable->absPatternCount; idx++)
   {
      if(ROK == cmMemcmp(absPatternCfgTable->absPatternCfg[idx].absPattern,
                                             absPattern, WR_EMM_ABS_PATTERN_SIZE))
      {
         *absPatternId = idx;
         RETVALUE(ROK);
      }
   }

   RETVALUE(RFAILED);
}

/** @brief This function apply ABS Pattern offset on ABS Pattern
 *
 * @details 
 *
 *     Function: wrEmmAbsApplyAbsPatternOffset
 *
 *         Processing steps:
 *
 *
 * @param[in]   srcAbsPattern   :   ABS Pattern before offset applied
 * @param[in]   offset          :   ABS offset to be applied
 * @param[out]  dstAbsPattern   :   ABS Pattern after offset applied
 * @return S16
 *     -# Success : ROK
 *     -# Failure : RFAILED
 */
PRIVATE S16 wrEmmAbsApplyAbsPatternOffset
(
U8                           *srcAbsPattern,
S8                           offset,
U8                           *dstAbsPattern
)
{
   U8                        idx;
   U8                        cnt = 0;

   TRC3(wrEmmAbsApplyAbsPatternOffset)

   /* +ve is advanced */
   if(offset > 0)
   {
      for(idx = RGR_ABS_PATTERN_LEN-1; idx> (RGR_ABS_PATTERN_LEN-1+offset); idx--)
      {
         dstAbsPattern[(U8)offset-cnt] = srcAbsPattern[idx];
         cnt++;
      }

      for(idx = 0; idx<=(RGR_ABS_PATTERN_LEN-1+offset); idx++)
      {
         dstAbsPattern[cnt+idx] = srcAbsPattern[idx];
      }
   }
   /* -ve is delayed */
   else if(offset < 0)
   {
      for(idx = offset; idx < RGR_ABS_PATTERN_LEN; idx++)
      {
         dstAbsPattern[cnt++] = srcAbsPattern[idx];
      }

      for(idx = 0; idx < offset; idx++)
      {
         dstAbsPattern[cnt++] = srcAbsPattern[idx];
      }
   }
   else
   {
      cmMemcpy(dstAbsPattern, srcAbsPattern, RGR_ABS_PATTERN_LEN);
   }

   RETVALUE(ROK);
}

/** @brief This function configures MAC scheduler with ABS parameters 
 *
 * @details 
 *
 *     Function: wrEmmAbsCfgScheduler 
 *
 *         Processing steps:
 *
 *
 * @param[in]  status : Indicates whether ABS feature is enabled or disabled  
 * @return S16
 *     -# Success : ROK
 *     -# Failure : RFAILED
 */
PRIVATE S16 wrEmmAbsCfgScheduler
(
RgrFeatureStatus                  status
)
{
   U8                             idx;
   WrCellCb                       *cellCb;
   LwrLteAdvancedConfigTrans      absSchCfg;
   U8                             absPattern[RGR_ABS_PATTERN_LEN];

   TRC3(wrEmmAbsCfgScheduler)

   WR_GET_ABS_CELLCB(cellCb);
   if((cellCb == NULLP))
   {
       RLOG0(L_ERROR, " CELLCB not found ");
       RETVALUE(RFAILED);   
   }  /*KW fix for LTE_ADV */    
   cmMemset((U8*)&absSchCfg, 0, sizeof(LwrLteAdvancedConfigTrans));

   absSchCfg.pres = RGR_ABS;
   absSchCfg.lteAdvancedConfig.absCfg.status = status;
   if(RGR_ENABLE == status)
   {
      absSchCfg.lteAdvancedConfig.absCfg.absPatternType =
                cellCb->lteAdvCb.lteAdvCfg.absCfg.absPatternType;
      /* If dynamic abs negotiation is enabled then 
                  configure load periodicity else set it to 0 */
      if(cellCb->lteAdvCb.lteAdvCfg.absCfg.absLoadPeriodicity)
      {
         absSchCfg.lteAdvancedConfig.absCfg.absLoadPeriodicity = 
                                      WR_EMM_ABS_LOAD_PERIODICITY;
      }
      else
      {
         absSchCfg.lteAdvancedConfig.absCfg.absLoadPeriodicity = 0;
      }

      /* Convert abs pattern to 5 bytes to 40 bytes format */
      for(idx = 0; idx< WR_EMM_ABS_PATTERN_SIZE; idx++)
      {
         absPattern[idx*8+ 0] = (U8)((cellCb->lteAdvCb.absInfo.absPattern[idx] >> 7) & 1); /* KW fix for LTE_Adv */    
         absPattern[idx*8+ 1] = (U8)((cellCb->lteAdvCb.absInfo.absPattern[idx] >> 6) & 1); /* KW fix for LTE_Adv */ 
         absPattern[idx*8+ 2] = (U8)((cellCb->lteAdvCb.absInfo.absPattern[idx] >> 5) & 1); /* KW fix for LTE_Adv */ 
         absPattern[idx*8+ 3] = (U8)((cellCb->lteAdvCb.absInfo.absPattern[idx] >> 4) & 1); /* KW fix for LTE_Adv */ 
         absPattern[idx*8+ 4] = (U8)((cellCb->lteAdvCb.absInfo.absPattern[idx] >> 3) & 1); /* KW fix for LTE_Adv */ 
         absPattern[idx*8+ 5] = (U8)((cellCb->lteAdvCb.absInfo.absPattern[idx] >> 2) & 1); /* KW fix for LTE_Adv */ 
         absPattern[idx*8+ 6] = (U8)((cellCb->lteAdvCb.absInfo.absPattern[idx] >> 1) & 1); /* KW fix for LTE_Adv */ 
         absPattern[idx*8+ 7] = (U8)((cellCb->lteAdvCb.absInfo.absPattern[idx] >> 0) & 1); /* KW fix for LTE_Adv */ 
      }

      /* Incase of victim eNodeB apply ABS Pattern offset */
      if(WR_LTE_ADV_PICO_ENB == cellCb->lteAdvCb.lteAdvCfg.enbType) 
      {
         wrEmmAbsApplyAbsPatternOffset(absPattern, LTE_ADV_ABS_OFFSET, 
                         absSchCfg.lteAdvancedConfig.absCfg.absPattern);
      }
      else
      {
         cmMemcpy(absSchCfg.lteAdvancedConfig.absCfg.absPattern, absPattern, 
                                                        RGR_ABS_PATTERN_LEN);
      }
      cellCb->lteAdvCb.absInfo.isAbsEnabled = TRUE;
   }
   else
   {
      cellCb->lteAdvCb.absInfo.isAbsEnabled = FALSE;
      /* Reset pattern old pattern info */
      cmMemset(cellCb->lteAdvCb.absInfo.absPattern, 0 , WR_EMM_ABS_PATTERN_SIZE);
      cellCb->lteAdvCb.absInfo.u.aggrInfo.absPatternId = 0;
   }

   RETVALUE(wrEmmCellSchdReCfgLteAdvFeatures(cellCb->cellId, &absSchCfg));
}

/** @brief This function determines ABS Pattern selection is required or not
 *
 * @details 
 *
 *     Function: wrEmmAbsAggrIsPatternSlctnRqrd
 *
 *         Processing steps:
 *
 *
 * @param[in] Void  
 * @return S16
 *     -# Success : ROK
 *     -# Failure : RFAILED
 */
PRIVATE S16 wrEmmAbsAggrIsPatternSlctnRqrd
(
Void
)
{
   WrCellCb                           *cellCb;
   WrEmmAbsAggrPatternCfg             *absPatternCfg;

   TRC3(wrEmmAbsAggrIsPatternSlctnRqrd)

   WR_GET_ABS_CELLCB(cellCb);
   if((cellCb == NULLP))
   {
       RLOG0(L_ERROR, "wrEmmAbsAggrIsPatternSlctnRqrd: CELLCB not found ");
       RETVALUE(RFAILED);   
   }  /*KW fix for LTE_ADV */    
   absPatternCfg = &cellCb->lteAdvCb.absInfo.u.aggrInfo.absPatternCfgTable.
                    absPatternCfg[cellCb->lteAdvCb.absInfo.u.aggrInfo.absPatternId];

   if((0 == absPatternCfg->absPercentage) && 
      (0 != wrEmmCb.lteAdvCb.absInfo.peerInfo.u.aggrInfo.absPeerEnbLstCp.count))
   {
      RETVALUE(TRUE);
   }

   RETVALUE(FALSE);
}

/*--------------------------------------------------------------------*/
/*        IMPLEMENTATION OF ABS MODULE UTILITIES(PICO)                */
/*--------------------------------------------------------------------*/


/** @brief This function finds whether any UE facing interferece from 
 *         given eNodeB
 *
 * @details 
 *
 *     Function:wrEmmAbsVctmFndAnyUeFacingInerference 
 *
 *         Processing steps:
 *
 *
 * @param[in]   enbId : To find eNodeB from which UE is facing interference
 * @return S16
 *     -# Success : ROK
 *     -# Failure : RFAILED
 */
PRIVATE S16 wrEmmAbsVctmFndAnyUeFacingInerference
(
WrEmmAbsEnbId                             *enbId
)
{
   U16                                    ueCnt = 0;
   WrUeCb                                 *ueCb = NULLP;    /*KW fix for LTE_ADV */    
   WrUmmCellCb                            *cellCb = NULLP;  /*KW fix for LTE_ADV */    
   WrEmmAbsCellInfoLstItem                *cellInfo;
   CmLList                                *node;

   TRC3(wrEmmAbsVctmFndAnyUeFacingInerference)

   WR_UMM_GET_ABS_CELLCB(cellCb);
   if((cellCb == NULLP))
   {
       RLOG0(L_ERROR, " CELLCB not found ");
       RETVALUE(RFAILED);   
   }  /*KW fix for LTE_ADV */       
   while(ueCnt < cellCb->maxUesPerCell)
   {
      ueCb = cellCb->ueCbs[ueCnt++];
      if(ueCb == NULLP)
      {
         continue;
      }

      node = ueCb->lteAdvCb.absInfo.cellListCp.first;

      while(NULLP != node)
      {
         cellInfo = (WrEmmAbsCellInfoLstItem*)node->node;
         node     = node->next;
         if(cellInfo->cellInfo.enbId.val == enbId->val)
         {
            RETVALUE(TRUE);
         }
      }
   }

   RETVALUE(FALSE);
}

/** @brief This function finds number of UEs facing interferece 
 *         from given cell
 *
 * @details 
 *
 *     Function:wrEmmAbsVctmFndNumUeFacingInerference
 *
 *         Processing steps:
 *
 *
 * @param[in]   cellId : To find cell from which UE is facing interference
 * @param[out]  numUes : Number of UEs facing interference from given cell
 * @return S16                             
 *     -# Success : ROK
 *     -# Failure : RFAILED
 */
PRIVATE S16 wrEmmAbsVctmFndNumUeFacingInerference
(
WrEmmAbsCellId                             *cellId,
U32                                        *numUes
)
{
   U16                                    ueCnt = 0;
   WrUeCb                                 *ueCb = NULLP;  /*KW fix for LTE_ADV */    
   WrUmmCellCb                            *cellCb = NULLP;    /*KW fix for LTE_ADV */    
   WrEmmAbsCellInfoLstItem                *cellInfo;
   CmLList                                *node;

   TRC3(wrEmmAbsVctmFndNumUeFacingInerference)

   WR_UMM_GET_ABS_CELLCB(cellCb);
   if((cellCb == NULLP))
   {
       RLOG0(L_ERROR, " CELLCB not found ");
       RETVALUE(RFAILED);   
   }  /*KW fix for LTE_ADV */    
   *numUes = 0;

   while(ueCnt < cellCb->maxUesPerCell)
   {
      ueCb = cellCb->ueCbs[ueCnt++];
      if(ueCb == NULLP)
      {
         continue;
      }

      node = ueCb->lteAdvCb.absInfo.cellListCp.first;

      while(NULLP != node)
      {
         cellInfo = (WrEmmAbsCellInfoLstItem*)node->node;
         node     = node->next;
         if(cellInfo->cellInfo.cellId.cId == cellId->cId)
         {
            (*numUes)++;
         }
      }
   }

   RETVALUE(ROK);
}

/** @brief This function adds cells to UE Cb
 *
 * @details 
 *
 *     Function: wrEmmAbsVctmAddCellToUeCb 
 *
 *         Processing steps:
 *
 *
 * @param[in]   ueId    : Id of UE  
 * @param[in]  cellInfo : Information of cell 
 * @return S16
 *     -# Success : ROK
 *     -# Failure : RFAILED
 */
PRIVATE S16 wrEmmAbsVctmAddCellToUeCb
(
U32                              ueId,
WrEmmAbsCellInfo                 *cellInfo
)
{
   WrUeCb                        *ueCb = NULLP; /*KW fix for LTE_ADV */    
   WrEmmAbsCellInfoLstItem       *cellInfoItem;
   WrEmmAbsVctmPeerEnbCb         *peerEnbCb;
   WrEmmAbsVctmPeerCellCb        *peerCellCb;

   TRC3(wrEmmAbsVctmAddCellToUeCb)

   WR_EMM_ABS_GET_UECB(ueId, ueCb);
   if(NULLP == ueCb)
   {
       RLOG1(L_ERROR, " UECB not found for given ueId(%d)", (U16)ueId);
       RETVALUE(RFAILED);    /* KW fix for LTE_ADV */
   }                                                      
   if(NULLP == ueCb)
   {
      RETVALUE(RFAILED);
   }

   WR_ALLOC(&cellInfoItem, sizeof(WrEmmAbsCellInfoLstItem));
   if(NULLP == cellInfoItem)
   {
       RLOG0(L_FATAL, "Memory allocation failed.");
       RETVALUE(RFAILED);
   }          /*KW fix for LTE_ADV */               
   cellInfoItem->cellInfo.cellId = cellInfo->cellId;
   cellInfoItem->cellInfo.enbId  = cellInfo->enbId;
   cellInfoItem->lnk.node = (PTR)cellInfoItem;

   cmLListAdd2Tail(&ueCb->lteAdvCb.absInfo.cellListCp, &cellInfoItem->lnk);

   if(ROK == wrEmmAbsVctmGetPeerEnbCb(&cellInfoItem->cellInfo.enbId, &peerEnbCb))
   {
      if(ROK == wrEmmAbsVctmGetPeerCellCb(peerEnbCb, &cellInfoItem->cellInfo.cellId, &peerCellCb))
      {
         wrEmmAbsVctmAddCellWeightage(peerCellCb, 1);
      }
   }

   RETVALUE(ROK);
}

/** @brief This function removes all cells from UE Cb
 *
 * @details 
 *
 *     Function: wrEmmAbsVctmRemoveAllCellFrmUeCb 
 *
 *         Processing steps:
 *
 *
 * @param[in] ueId  : Id of UE  
 * @return S16
 *     -# Success : ROK
 *     -# Failure : RFAILED
 */
PRIVATE S16 wrEmmAbsVctmRemoveAllCellFrmUeCb
(
U32                                      ueId
)
{
   WrUeCb                                *ueCb = NULLP; /*KW fix for LTE_ADV */    
   CmLList                               *node;
   WrEmmAbsVctmPeerEnbCb                 *peerEnbCb;
   WrEmmAbsVctmPeerCellCb                *peerCellCb;
   WrEmmAbsCellInfoLstItem               *cellInfo;

   TRC3(wrEmmAbsVctmRemoveAllCellFrmUeCb)

   WR_EMM_ABS_GET_UECB(ueId, ueCb);

   if(NULLP == ueCb)
   {
       RLOG1(L_ERROR, " UECB not found for given ueId(%d)", (U16)ueId);
       RETVALUE(RFAILED);    /* KW fix for LTE_ADV */
   }                                                      
   node = ueCb->lteAdvCb.absInfo.cellListCp.first;

   while(NULLP != node)
   {
      cellInfo = (WrEmmAbsCellInfoLstItem*)node->node;
      if(ROK == wrEmmAbsVctmGetPeerEnbCb(&cellInfo->cellInfo.enbId, &peerEnbCb))
      {
         if(ROK == wrEmmAbsVctmGetPeerCellCb(peerEnbCb, &cellInfo->cellInfo.cellId, &peerCellCb))
         {
            wrEmmAbsVctmRemoveCellWeightage(peerCellCb, 1);
         }
      }
      node = node->next;
      cmLListDelFrm(&ueCb->lteAdvCb.absInfo.cellListCp, &cellInfo->lnk);
      WR_FREE(cellInfo, sizeof(WrEmmAbsCellInfoLstItem));
   }
   RETVALUE(ROK);
}

/** @brief This function adds weightage to peer cell
 *
 * @details 
 *
 *     Function: wrEmmAbsVctmAddCellWeightage
 *
 *         Processing steps:
 *             1. Adds weightage to cell
 *             2. Adds weightage to abs pattern
 *
 *
 * @param[in]  peerCellCb : Peer cell control information
 * @param[in]  count      : Weightage factor(Num UEs)
 * @return S16
 *     -# Success : ROK
 *     -# Failure : RFAILED
 */
PRIVATE S16 wrEmmAbsVctmAddCellWeightage
(
WrEmmAbsVctmPeerCellCb            *peerCellCb,
U32                               count
)                                 /*KW fix for LTE_ADV */
{
   TRC3(wrEmmAbsVctmAddCellWeightage)

   peerCellCb->cellWeightage+= count;

   wrEmmAbsVctmAddAbsWeightage(peerCellCb->absPatternCb, count);

   RETVALUE(ROK);
}

/** @brief This function removes weightage from peer cell
 *
 * @details 
 *
 *     Function: wrEmmAbsVctmRemoveCellWeightage
 *
 *         Processing steps:
 *             1. Removes weightage from cell
 *             2. Removes weightage from abs pattern
 *
 *
 * @param[in]  peerCellCb : Peer cell control information
 * @param[in]  count      : Weightage factor(Num UEs)
 * @return S16
 *     -# Success : ROK
 *     -# Failure : RFAILED
 */
PRIVATE S16 wrEmmAbsVctmRemoveCellWeightage
(
WrEmmAbsVctmPeerCellCb            *peerCellCb,
U32                               count
)                                  /*KW fix for LTE_ADV */ 
{
   TRC3(wrEmmAbsVctmRemoveCellWeightage)

   if(peerCellCb->cellWeightage < count)
   {
      count = peerCellCb->cellWeightage;
   }

   peerCellCb->cellWeightage-= count;

   wrEmmAbsVctmRemoveAbsWeightage(peerCellCb->absPatternCb, count);

   RETVALUE(ROK);
}

/** @brief This function adds weightage to abs pattern
 *
 * @details 
 *
 *     Function: wrEmmAbsVctmAddAbsWeightage
 *
 *         Processing steps:
 *             1. Adds weightage to abs pattern
 *
 *
 * @param[in]  patternCb  : ABS Pattern control information
 * @param[in]  count      : Weightage factor(Num UEs)
 * @return S16
 *     -# Success : ROK
 *     -# Failure : RFAILED
 */
PRIVATE S16 wrEmmAbsVctmAddAbsWeightage
(
WrEmmAbsVctmPatternCb            *patternCb,
U32                               count    
)       /* KW fix for LTE_ADV */
{
   TRC3(wrEmmAbsVctmAddAbsWeightage)
   if(NULLP != patternCb)
   {
      patternCb->absWeightage += count;
   }
   RETVALUE(ROK);
}

/** @brief This function removes weightage to abs pattern
 *
 * @details 
 *
 *     Function: wrEmmAbsVctmRemoveAbsWeightage
 *
 *         Processing steps:
 *             1. Remove weightage from abs pattern
 *
 *
 * @param[in]  patternCb  : ABS Pattern control information
 * @param[in]  count      : Weightage factor(Num UEs)
 * @return S16                    
 *     -# Success : ROK
 *     -# Failure : RFAILED
 */
PRIVATE S16 wrEmmAbsVctmRemoveAbsWeightage
(
WrEmmAbsVctmPatternCb            *patternCb,
U32                               count
)       /*KW fix for LTE_ADV */     
{
   TRC3(wrEmmAbsVctmRemoveAbsWeightage)
   if(NULLP != patternCb)
   {
      patternCb->absWeightage -= count;
   }
   RETVALUE(ROK);
}

/** @brief This function selects ABS Pattern for Pico eNodeBs
 *
 * @details 
 *
 *     Function: wrEmmAbsVctmSelectAbsPattern
 *
 *         Processing steps:
 *              1. Selects ABS Pattern with hightest weigtage 
 *
 * @param[in]    Void 
 * @return S16
 *     -# Success : ROK
 *     -# Failure : RFAILED
 */
PRIVATE S16 wrEmmAbsVctmSelectAbsPattern
(
WrEnumAbsPatternSelectFlrCause *cause    
)
{
   U32                          absWeightage = 0;
   Bool                         flag = FALSE;
   CmLList                      *node;
   WrEmmAbsVctmPatternCb        *patternCb = NULLP;
   WrCellCb                     *cellCb;

   TRC3(wrEmmAbsVctmSelectAbsPattern)

   WR_GET_ABS_CELLCB(cellCb);
   if((cellCb == NULLP))
   {
       RLOG0(L_ERROR, " CELLCB not found ");
       RETVALUE(RFAILED);   
   }  /*KW fix for LTE_ADV */         
   node = wrEmmCb.lteAdvCb.absInfo.peerInfo.u.vctmInfo.absPatternLstCp.first;

   while(NULLP != node)
   {
      if((flag == FALSE) || (((WrEmmAbsVctmPatternCb*)node->node)->absWeightage > absWeightage))
      {
         patternCb = (WrEmmAbsVctmPatternCb*)node->node;
         if(patternCb->absPercentage)
         {
            flag = TRUE;
            absWeightage = patternCb->absWeightage;
         }
      }
      node = node->next;
   }

   if(NULLP == patternCb)
   {
      *cause =  WR_EMM_ABS_NO_PATTERN;
      RETVALUE(RFAILED);
   }

   /* If old pattern and new are same, then it is considered as failure */
   if(ROK == cmMemcmp(cellCb->lteAdvCb.absInfo.absPattern, 
                            patternCb->absPattern, WR_EMM_ABS_PATTERN_SIZE))
   {
      *cause = WR_EMM_ABS_DUP_PATTERN;
      RETVALUE(RFAILED);
   }

   if(0 == patternCb->absPercentage)
   {
      *cause = WR_EMM_ABS_NULL_PATTERN;
      RETVALUE(RFAILED);
   }

   cmMemcpy(cellCb->lteAdvCb.absInfo.absPattern, patternCb->absPattern,
                                                    WR_EMM_ABS_PATTERN_SIZE);
   cellCb->lteAdvCb.absInfo.u.vctmInfo.absPercentage = patternCb->absPercentage;

   RLOG3(L_DEBUG, " Selected ABS Pattern:[0x%x, 0x%x, 0x%x] ",
                    patternCb->absPattern[0], patternCb->absPattern[1],
                    patternCb->absPattern[2]); 
   RLOG3(L_DEBUG, " Selected ABS Pattern:[0x%x, 0x%x] Weightage:[0x%lx] ",  
                    patternCb->absPattern[3], patternCb->absPattern[4],
                    patternCb->absWeightage);
   RETVALUE(ROK);
}

/** @brief This function invoke UMM Lte-Advanced module 
 *
 * @details 
 *
 *     Function: wrEmmAbsConfigUmmUe
 *
 *         Processing steps:
 *
 *
 * @param[in]   ueId     : Identifier of UE
 * @param[in]   status   : To indicate ABS feature is enabled or disabled
 * @param[in]   peerType : To indicate local or remote UE configuration
 * @return S16
 *     -# Success : ROK
 *     -# Failure : RFAILED
 */
PRIVATE S16 wrEmmAbsConfigUmmUe
(
U32                         ueId,
RgrFeatureStatus            status,
WrUmmLteAdvPeerType         peerType
)
{
    WrUeCb                   *ueCb = NULLP;   /*KW fix for LTE_ADV */    
    WrUmmCellCb              *cellCb = NULLP; /*KW fix for LTE_ADV */    

    TRC3(wrEmmAbsConfigUmmUe)
    WR_EMM_ABS_GET_UECB(ueId, ueCb);
    if(NULLP == ueCb)
    {
        RLOG1(L_ERROR, " UECB not found for given ueId(%d)", (U16)ueId);
        RETVALUE(RFAILED);    /* KW fix for LTE_ADV */
    }
    WR_UMM_GET_CELLCB(cellCb, ueCb->cellId);
    if(cellCb == NULLP)            /* KW fix for LTE_ADV */
    {    
        RLOG1(L_ERROR, " CELLCB not found for given cellId(%d)", (U16)ueCb->cellId);
        RETVALUE(RFAILED);        /* KW fix for LTE_ADV */   
    }
    wrUmmHdlLteAdvRecfgForUe(cellCb, ueCb, status, WR_UMM_LTE_ADV_ABS_PROC, peerType);
    RETVALUE(ROK);
}

/** @brief This function invoke UMM Lte-Advanced module 
 *
 * @details 
 *
 *     Function: wrEmmAbsConfigAllUmmUe
 *
 *         Processing steps:
 *
 * @return S16
 *     -# Success : ROK
 *     -# Failure : RFAILED
 */
PRIVATE S16 wrEmmAbsConfigAllUmmUe
(
Void
)
{
   WrUeCb                   *ueCb;
   WrCellCb                 *emmCellCb = NULLP; /*KW fix for LTE_ADV */    
   WrUmmCellCb              *cellCb = NULLP;  /*KW fix for LTE_ADV */    
   U16                      ueCnt = 0;

   TRC3(wrEmmAbsConfigAllUmmUe)

   WR_UMM_GET_ABS_CELLCB(cellCb);
   if(cellCb == NULLP)
   {
       RLOG0(L_ERROR, " CELLCB not found ");
       RETVALUE(RFAILED);     
   }        /*KW fix for LTE_ADV */   
   WR_GET_CELLCB(emmCellCb, cellCb->cellId);
   if((emmCellCb == NULLP))
   {
       RLOG0(L_ERROR, " CELLCB not found ");
       RETVALUE(RFAILED);   
   }  /*KW fix for LTE_ADV */         
   while(ueCnt < cellCb->maxUesPerCell)
   {
      ueCb = cellCb->ueCbs[ueCnt++];
      if((ueCb == NULLP) || (ueCb->lteAdvCb.absInfo.isAbsUe == FALSE))
      {
         continue;
      }
      if(TRUE == emmCellCb->lteAdvCb.absInfo.isAbsEnabled)
      {
         wrUmmHdlLteAdvRecfgForUe(cellCb, ueCb, RGR_ENABLE, WR_UMM_LTE_ADV_ABS_PROC, WR_UMM_LTE_ADV_LCL_UE);         
      }
      else
      {
         wrUmmHdlLteAdvRecfgForUe(cellCb, ueCb, RGR_DISABLE, WR_UMM_LTE_ADV_ABS_PROC, WR_UMM_LTE_ADV_LCL_UE);         
      }
   }

   RETVALUE(ROK);
}


/**********************************************************************/
/*             IMPLEMENTATION OF DATA STRUCTURE ACCESS                */
/**********************************************************************/

/*--------------------------------------------------------------------*/
/*                            MACRO                                   */
/*--------------------------------------------------------------------*/


/** @brief This function allocates and adds peer eNodeB Cb to WrEmmCb 
 *
 * @details 
 *
 *     Function: wrEmmAbsAggrAllocNAddPeerEnbCb
 *
 *         Processing steps:
 *
 *
 * @param[in]   enbId  : Identifier of peer eNodeB
 * @param[out]  peerEnbCb : Pointer to peer eNodeB Cb 
 * @return S16
 *     -# Success : ROK
 *     -# Failure : RFAILED
 */
PRIVATE S16 wrEmmAbsAggrAllocNAddPeerEnbCb
(
WrEmmAbsEnbId                      *enbId,
WrEmmAbsAggrPeerEnbCb              **peerEnbCb
)
{
   TRC3(wrEmmAbsAggrAllocNAddPeerEnbCb)

   WR_ALLOC(peerEnbCb, sizeof(WrEmmAbsAggrPeerEnbCb));
   if(NULLP == *peerEnbCb)
   {
      RLOG0(L_FATAL, "Memory allocation failed.");
      RETVALUE(RFAILED);
   }

   (*peerEnbCb)->enbId        = *enbId;
   (*peerEnbCb)->enb1MeasId   = enbId->val; 

   (*peerEnbCb)->lnk.node = (PTR)(*peerEnbCb);
   cmLListAdd2Tail(&wrEmmCb.lteAdvCb.absInfo.peerInfo.u.aggrInfo.absPeerEnbLstCp,
                                                 &(*peerEnbCb)->lnk);

   RETVALUE(ROK);
}

/** @brief This function finds peer eNodeB Cb using eNodeB id
 *
 * @details 
 *
 *     Function: wrEmmAbsAggrGetPeerEnbCb
 *
 *         Processing steps:
 *
 *
 * @param[in]   enbId  : Identifier of peer eNodeB
 * @param[out]  peerEnbCb : Pointer to peer eNodeB Cb 
 * @return S16
 *     -# Success : ROK
 *     -# Failure : RFAILED
 */
PRIVATE S16 wrEmmAbsAggrGetPeerEnbCb
(
WrEmmAbsEnbId                  *enbId,
WrEmmAbsAggrPeerEnbCb          **peerEnbCb
)
{
   CmLList                     *node = NULLP;

   TRC3(wrEmmAbsAggrGetPeerEnbCb)

   node = wrEmmCb.lteAdvCb.absInfo.peerInfo.u.aggrInfo.absPeerEnbLstCp.first;
   while(NULLP != node)
   {
      *peerEnbCb = (WrEmmAbsAggrPeerEnbCb*)node->node;
      if((*peerEnbCb)->enbId.val == enbId->val)
      {
         RETVALUE(ROK);
      }
      node = node->next;
   }

   *peerEnbCb = NULLP;
   RETVALUE(RFAILED);
}

/** @brief This function removes eNodeB Cb from list and 
 *         deallocates the memory 
 *
 * @details 
 *
 *     Function: wrEmmAbsAggrRemoveNDeallocPeerEnbCb
 *
 *         Processing steps:
 *             1. Deallocates all cells linked to this eNodeB
 *             2. Removes eNodeB Cb from list and deallocates
 *
 * @param[in]  peerEnbCb : Pointer to peer eNodeB Cb 
 * @return S16
 *     -# Success : ROK
 *     -# Failure : RFAILED
 */
PRIVATE S16 wrEmmAbsAggrRemoveNDeallocPeerEnbCb
(
WrEmmAbsAggrPeerEnbCb                  *peerEnbCb
)
{
   CmLList                             *node = NULLP;
   WrEmmAbsAggrPeerCellCb              *peerCellCb;

   TRC3(wrEmmAbsAggrRemoveNDeallocPeerEnbCb)

   if(NULLP == peerEnbCb)
   {
      RLOG0(L_ERROR, " Null pointer passed");
      RETVALUE(RFAILED);
   }

   node = peerEnbCb->absPeerCellLstCp.first;
   while(NULLP != node)
   {
      peerCellCb = (WrEmmAbsAggrPeerCellCb*)node->node;
      node = node->next;
      wrEmmAbsAggrRemoveNDeallocPeerCellCb(peerEnbCb, peerCellCb);
   }
   cmLListDelFrm(&wrEmmCb.lteAdvCb.absInfo.peerInfo.u.aggrInfo.absPeerEnbLstCp, &peerEnbCb->lnk);
   WR_FREE(peerEnbCb, sizeof(WrEmmAbsAggrPeerEnbCb));

   RETVALUE(ROK);
}

/** @brief This function allocate and add peer cell Cb to eNodeB Cb 
 *
 * @details 
 *
 *     Function: wrEmmAbsAggrAllocNAddPeerCellCb
 *
 *         Processing steps:
 *            1. Allocate memory and initializes cell cb
 *            2. Sets default usable pattern id and adds weigthage
 *            3. Add into list of cell of peer eNodeB Cb 
 *
 * @param[in]   enbCb   : Peer eNodeB Control Block
 * @param[in]   cellId  : Identifier of cell
 * @param[out]  peerCellCb : Pointer to peer cell Cb 
 * @return S16
 *     -# Success : ROK
 *     -# Failure : RFAILED
 */
PRIVATE S16 wrEmmAbsAggrAllocNAddPeerCellCb
(
WrEmmAbsAggrPeerEnbCb                *enbCb,
WrEmmAbsCellId                       *cellId,
WrEmmAbsAggrPeerCellCb               **peerCellCb
)
{
   WrCellCb                          *cellCb;
   TRC3(wrEmmAbsAggrAllocNAddPeerCellCb)

   WR_ALLOC(peerCellCb, sizeof(WrEmmAbsAggrPeerCellCb));
   if(NULLP == *peerCellCb)
   {
      RLOG0(L_FATAL, "Memory allocation failed.");
      RETVALUE(RFAILED);
   }

   (*peerCellCb)->cellId = *cellId;
   (*peerCellCb)->absPercentageRsrcUsed = 100;

   WR_GET_ABS_CELLCB(cellCb);
   if(cellCb == NULLP)
   {
       RLOG0(L_ERROR, " CELLCB not found ");
       RETVALUE(RFAILED);     
   }        /*KW fix for LTE_ADV */     
   if(0 == cellCb->lteAdvCb.absInfo.u.aggrInfo.absPatternCfgTable.
                  absPatternCfg[cellCb->lteAdvCb.absInfo.u.aggrInfo.absPatternId].absPercentage)
   {
      /* To add weightage to first valid ABS pattern */
      (*peerCellCb)->usablePatternId = (U8)(cellCb->lteAdvCb.absInfo.u.aggrInfo.absPatternId+1); /* KW fix for LTE_ADV */
   }
   else
   {
      /* To add weightage to currently used valid ABS pattern */
      (*peerCellCb)->usablePatternId = cellCb->lteAdvCb.absInfo.u.aggrInfo.absPatternId;
   }

   /* Update weightage in cellcb */
   wrEmmAbsAggrAddWeightage(*peerCellCb);

   (*peerCellCb)->lnk.node = (PTR)(*peerCellCb);
   cmLListAdd2Tail(&enbCb->absPeerCellLstCp, &(*peerCellCb)->lnk);

   RETVALUE(ROK);
}

/** @brief This function finds the peer cell from peer eNodeB Cb 
 *
 * @details 
 *
 *     Function: wrEmmAbsAggrGetPeerCellCb
 *
 *         Processing steps:
 *
 * @param[in]   enbCb   : Peer eNodeB Control Block
 * @param[in]   cellId  : Identifier of cell
 * @param[out]  peerCellCb : Pointer to peer cell Cb 
 * @return S16
 *     -# Success : ROK
 *     -# Failure : RFAILED
 */
PRIVATE S16 wrEmmAbsAggrGetPeerCellCb
(
WrEmmAbsAggrPeerEnbCb            *enbCb,
WrEmmAbsCellId                   *cellId,
WrEmmAbsAggrPeerCellCb           **peerCellCb
)
{
   CmLList                       *node = NULLP;
   TRC3(wrEmmAbsAggrGetPeerCellCb)

   node = enbCb->absPeerCellLstCp.first;
   while(NULLP != node)
   {
      *peerCellCb = (WrEmmAbsAggrPeerCellCb*)node->node;
      if((*peerCellCb)->cellId.cId == cellId->cId)
      {
         RETVALUE(ROK);
      }
      node =node->next;
   }

   *peerCellCb = NULLP;

   RETVALUE(RFAILED);
}

/** @brief This function remove peer cell cb from peer eNodeB Cb
 *
 * @details 
 *
 *     Function: wrEmmAbsAggrRemoveNDeallocPeerCellCb
 *
 *         Processing steps:
 *
 *
 * @param[in]   peerEnbCb : Pointer to peer eNodeB Cb
 * @param[in]   peerCellCb: Pointer to peer cell Cb   
 * @return S16
 *     -# Success : ROK
 *     -# Failure : RFAILED
 */
PRIVATE S16 wrEmmAbsAggrRemoveNDeallocPeerCellCb
(
WrEmmAbsAggrPeerEnbCb                  *peerEnbCb,
WrEmmAbsAggrPeerCellCb                 *peerCellCb
)
{
   TRC3(wrEmmAbsAggrRemoveNDeallocPeerCellCb)

   if(NULLP == peerCellCb || NULLP == peerEnbCb)
   {
      RLOG0(L_ERROR, " Null pointer passed");
      RETVALUE(RFAILED);
   }

   wrEmmAbsAggrRemoveWeightage(peerCellCb);

   cmLListDelFrm(&peerEnbCb->absPeerCellLstCp, &peerCellCb->lnk);
   WR_FREE(peerCellCb, sizeof(WrEmmAbsAggrPeerCellCb));

   RETVALUE(ROK);
}


/*--------------------------------------------------------------------*/
/*                            PICO                                    */
/*--------------------------------------------------------------------*/

/** @brief This function allocates and add ABS Pattern Cb to WrEmmCb
 *
 * @details 
 *
 *     Function: wrEmmAbsVctmAllocNAddAbsPatternCb
 *
 *         Processing steps:
 *             1. Allocates memory and initializes variables
 *             2. Adds into ABS Pattern list of WrEmmCb
 *
 * @param[in]  absPattern : Pointer to ABS Pattern 
 * @param[out] absPatternCb: Pointer to ABS PatternCb  
 * @return S16
 *     -# Success : ROK
 *     -# Failure : RFAILED
 */
PRIVATE S16 wrEmmAbsVctmAllocNAddAbsPatternCb
(
U8                                    *absPattern,
WrEmmAbsVctmPatternCb                 **absPatternCb
)
{
   U8                                 absSubFrames = 0;
   U8                                 idx1;
   U8                                 idx2;
   TRC3(wrEmmAbsVctmAllocNAddAbsPatternCb)

   WR_ALLOC(absPatternCb, sizeof(WrEmmAbsVctmPatternCb));
   if( NULLP == *absPatternCb)
   {
      RLOG0(L_FATAL, "Memory allocation failed.");
      RETVALUE(RFAILED);
   }

   cmMemcpy((*absPatternCb)->absPattern, absPattern, WR_EMM_ABS_PATTERN_SIZE);

   for(idx1 = 0; idx1< WR_EMM_ABS_PATTERN_SIZE; idx1++)
   {
      for(idx2 = 0; idx2< 8; idx2++)
      {
         if(absPattern[idx1]>>idx2 & 0x01)
            absSubFrames++;
      }
   }
   (*absPatternCb)->absPercentage = (U8)((absSubFrames*100)/(WR_EMM_ABS_PATTERN_SIZE*8)); /* KW fix for LTE_ADV */

   (*absPatternCb)->lnk.node = (PTR)(*absPatternCb);
   cmLListAdd2Tail(&wrEmmCb.lteAdvCb.absInfo.peerInfo.u.vctmInfo.absPatternLstCp, &(*absPatternCb)->lnk);

   RETVALUE(ROK);
}

/** @brief This function finds Pattern Cb from WrEmmCb
 *
 * @details 
 *
 *     Function: wrEmmAbsVctmGetAbsPatternCb
 *
 *         Processing steps:
 *
 * @param[in]  absPattern : Pointer to ABS Pattern 
 * @param[out] absPatternCb: Pointer to ABS PatternCb  
 * @return S16
 *     -# Success : ROK
 *     -# Failure : RFAILED
 */
PRIVATE S16 wrEmmAbsVctmGetAbsPatternCb
(
U8                               *absPattern,
WrEmmAbsVctmPatternCb            **absPatternCb
)
{
   CmLList                       *node = NULLP;

   TRC3(wrEmmAbsVctmGetAbsPatternCb)

   node = wrEmmCb.lteAdvCb.absInfo.peerInfo.u.vctmInfo.absPatternLstCp.first;
   while(NULLP != node)
   {
      *absPatternCb = (WrEmmAbsVctmPatternCb*)node->node;
      if(ROK == cmMemcmp((*absPatternCb)->absPattern, absPattern, WR_EMM_ABS_PATTERN_SIZE))
      {
         RETVALUE(ROK);
      }
      node =node->next;
   }

   *absPatternCb = NULLP;

   RETVALUE(RFAILED);
}

/** @brief This function removes ABS Pattern Cb from WrEmmCb 
 *         and deallocates the same
 *
 * @details 
 *
 *     Function: wrEmmAbsVctmRemoveNDeallocAbsPatternCb 
 *
 *         Processing steps:
 *             1. Unlinks Abs PatterCb from peer cell Cb
 *             2. Removes ABS Pattern Cb from list and deallocate it
 *
 * @param[in] absPatternCb: Pointer to ABS PatternCb  
 * @return S16
 *     -# Success : ROK
 *     -# Failure : RFAILED
 */
PRIVATE S16 wrEmmAbsVctmRemoveNDeallocAbsPatternCb
(
WrEmmAbsVctmPatternCb                  *absPatternCb
)
{
   CmLList                             *node;
   WrEmmAbsVctmPeerCellCb              *peerCellCb;

   TRC3(wrEmmAbsVctmRemoveNDeallocAbsPatternCb)

   if(NULLP == absPatternCb)
   {
      RLOG0(L_ERROR, " Invalid pointer");
      RETVALUE(RFAILED);
   }

   node = absPatternCb->absCellLstCp.first;
   while(node)
   {
      peerCellCb = (WrEmmAbsVctmPeerCellCb*)node->node;
      peerCellCb->absPatternCb = NULLP;
      node = node->next;
   }

   cmLListDelFrm(&wrEmmCb.lteAdvCb.absInfo.peerInfo.u.vctmInfo.absPatternLstCp, &absPatternCb->lnk);
   WR_FREE(absPatternCb, sizeof(WrEmmAbsVctmPatternCb));

   RETVALUE(ROK);
}

/** @brief This function adds peer cell to Abs Pattern's cell list
 *
 * @details 
 *
 *     Function: wrEmmAbsVctmAddCellCbToAbsPatternCb 
 *
 *         Processing steps:
 *
 *
 * @param[in]   absPatternCb : Pointer to ABS Pattern Cb
 * @param[out]  peerCellCb   : Pointer to peer cell Cb 
 * @return S16
 *     -# Success : ROK
 *     -# Failure : RFAILED
 */
PRIVATE S16 wrEmmAbsVctmAddCellCbToAbsPatternCb
(
WrEmmAbsVctmPatternCb                *absPatternCb,
WrEmmAbsVctmPeerCellCb               *peerCellCb
)
{
   TRC3(wrEmmAbsVctmAddCellCbToAbsPatternCb)

   if(NULLP == absPatternCb || NULLP == peerCellCb)
   {
      RLOG0(L_ERROR, " Invalid pointer");
      RETVALUE(RFAILED);
   }

   peerCellCb->absPatternLnk.node = (PTR)peerCellCb;
   cmLListAdd2Tail(&absPatternCb->absCellLstCp, &peerCellCb->absPatternLnk);

   peerCellCb->absPatternCb = absPatternCb;

   RETVALUE(ROK);
}

/** @brief This function removes peer cell from Abs Pattern's cell list
 *
 * @details 
 *
 *     Function: wrEmmAbsVctmRemoveCellCbToAbsPatternCb 
 *
 *         Processing steps:
 *            1. Removes cell from Abs Pattern's cell list
 *            2. If no cell linked to Abs Pattern, then removes ABS PatternCb
 *
 * @param[in]   absPatternCb : Pointer to ABS Pattern Cb
 * @param[out]  peerCellCb   : Pointer to peer cell Cb 
 * @return S16
 *     -# Success : ROK 
 *     -# Failure : RFAILED
 */
PRIVATE S16 wrEmmAbsVctmRemoveCellCbFrmAbsPatternCb
(
WrEmmAbsVctmPatternCb                   *absPatternCb,
WrEmmAbsVctmPeerCellCb                  *peerCellCb
)
{
   TRC3(wrEmmAbsVctmRemoveCellCbToAbsPatternCb)

   if(NULLP == absPatternCb || NULLP == peerCellCb)
   {
      RLOG0(L_ERROR, " Invalid pointer");
      RETVALUE(RFAILED);
   }

   cmLListDelFrm(&absPatternCb->absCellLstCp, &peerCellCb->absPatternLnk);

   peerCellCb->absPatternCb = NULLP;
   if(0 == absPatternCb->absCellLstCp.count)
   {
      wrEmmAbsVctmRemoveNDeallocAbsPatternCb(absPatternCb);
   }

   RETVALUE(ROK);
}

/** @brief This function allocates and add peer cell cb to peer eNodeB Cb
 *
 * @details 
 *
 *     Function: wrEmmAbsVctmAllocNAddPeerCellCb
 *
 *         Processing steps:
 *             1. Allocate moemory and inittialize 
 *             2. Add to eNodeB's cell list
 *             3. Find number of UE facing interference 
 *                and add weightage to cell
 *
 *
 * @param[in]   enbCb   : Pointer to peer eNodeB Cb
 * @param[in]   cellId  : Identifier of cell
 * @param[out]  peerCellCb : Pointer to peer cell Cb 
 * @return S16
 *     -# Success : ROK
 *     -# Failure : RFAILED
 */
PRIVATE S16 wrEmmAbsVctmAllocNAddPeerCellCb
(
WrEmmAbsVctmPeerEnbCb                *enbCb,
WrEmmAbsCellId                       *cellId,
WrEmmAbsVctmPeerCellCb               **peerCellCb
)
{
   U32                               numUes=0;
   TRC3(wrEmmAbsVctmAllocNAddPeerCellCb)

   WR_ALLOC(peerCellCb, sizeof(WrEmmAbsVctmPeerCellCb));
   if( NULLP == *peerCellCb)
   {
      RLOG0(L_FATAL, "Memory allocation failed.");
      RETVALUE(RFAILED);
   }

   (*peerCellCb)->cellId = *cellId;

   (*peerCellCb)->enbLnk.node = (PTR)(*peerCellCb);
   cmLListAdd2Tail(&enbCb->absPeerCellLstCp, &(*peerCellCb)->enbLnk);

   /* Find out number of UEs facing interference and add weightage */
   wrEmmAbsVctmFndNumUeFacingInerference(cellId, &numUes);
   wrEmmAbsVctmAddCellWeightage((*peerCellCb), numUes);

   RETVALUE(ROK);
}

/** @brief This function finds peer cell cb from peer eNodeB Cb
 *
 * @details 
 *
 *     Function: wrEmmAbsVctmGetPeerCellCb
 *
 *         Processing steps:
 *
 *
 * @param[in]   enbCb   : Pointer to peer eNodeB Cb
 * @param[in]   cellId  : Identifier of cell
 * @param[out]  peerCellCb : Pointer to peer cell Cb 
 * @return S16
 *     -# Success : ROK
 *     -# Failure : RFAILED
 */
PRIVATE S16 wrEmmAbsVctmGetPeerCellCb
(
WrEmmAbsVctmPeerEnbCb             *enbCb,
WrEmmAbsCellId                    *cellId,
WrEmmAbsVctmPeerCellCb            **peerCellCb
)
{
   CmLList                        *node;

   TRC3(wrEmmAbsVctmGetPeerCellCb)

   node = enbCb->absPeerCellLstCp.first;
   while(NULLP != node)
   {
      *peerCellCb = (WrEmmAbsVctmPeerCellCb*)node->node;
      if(cellId->cId == (*peerCellCb)->cellId.cId)
      {
         RETVALUE(ROK);
      }
      node =node->next;
   }

   *peerCellCb = NULLP;
   RETVALUE(RFAILED);
}

/** @brief This function removes peer cell from eNodeB's list and deallocate it 
 *
 * @details 
 *
 *     Function: wrEmmAbsVctmRemoveNDeallocPeerCellCb
 *
 *         Processing steps:
 *
 * @param[in]   peerEnbCb   : Pointer to peer eNodeB Cb
 * @param[out]  peerCellCb  : Pointer to peer cell Cb 
 * @return S16
 *     -# Success : ROK
 *     -# Failure : RFAILED
 */
PRIVATE S16 wrEmmAbsVctmRemoveNDeallocPeerCellCb
(
WrEmmAbsVctmPeerEnbCb                    *peerEnbCb,
WrEmmAbsVctmPeerCellCb                   *peerCellCb
)
{
   TRC3(wrEmmAbsVctmRemoveNDeallocPeerCellCb)

   if(NULLP == peerCellCb || NULLP == peerEnbCb)
   {
      RLOG0(L_ERROR, " Null pointer passed");
      RETVALUE(RFAILED);
   }

   /* Remove weightage and cell from ABS Pattern */
   if(NULLP != peerCellCb->absPatternCb)
   {
      wrEmmAbsVctmRemoveCellWeightage(peerCellCb, peerCellCb->cellWeightage);
      wrEmmAbsVctmRemoveCellCbFrmAbsPatternCb(peerCellCb->absPatternCb, peerCellCb);
   }

   cmLListDelFrm(&peerEnbCb->absPeerCellLstCp, &peerCellCb->enbLnk);
   WR_FREE(peerCellCb, sizeof(WrEmmAbsVctmPeerCellCb));

   RETVALUE(ROK);
}

/** @brief This function allocates and add peer eNodeB Cb to WrEmmCb 
 *
 * @details 
 *
 *     Function: wrEmmAbsVctmAllocNAddPeerEnbCb
 *
 *         Processing steps:
 *
 *
 * @param[in]   enbId   : Identifier of eNodeB id
 * @param[out]  peerEnbCb : Pointer to peer eNodeB Cb 
 * @return S16
 *     -# Success : ROK
 *     -# Failure : RFAILED
 */
PRIVATE S16 wrEmmAbsVctmAllocNAddPeerEnbCb
(
WrEmmAbsEnbId                       *enbId,
WrEmmAbsVctmPeerEnbCb               **peerEnbCb
)
{
   TRC3(wrEmmAbsVctmAllocNAddPeerEnbCb)

   WR_ALLOC(peerEnbCb, sizeof(WrEmmAbsVctmPeerEnbCb));
   if(NULLP == *peerEnbCb)
   {
      RLOG0(L_FATAL, "Memory allocation failed.");
      RETVALUE(RFAILED);
   }

   (*peerEnbCb)->enbId         = *enbId;
   (*peerEnbCb)->enb2MeasId    = enbId->val; 
   (*peerEnbCb)->absRprtStatus = CztRegistration_RqststopEnum;
   (*peerEnbCb)->absRprtPeriodCount  = 1;
   cmInitTimers(&(*peerEnbCb)->tmr, 1);

   (*peerEnbCb)->lnk.node = (PTR)(*peerEnbCb);
   cmLListAdd2Tail(&wrEmmCb.lteAdvCb.absInfo.peerInfo.u.vctmInfo.absPeerEnbLstCp,
                                                &(*peerEnbCb)->lnk);

   RETVALUE(ROK);
}

/** @brief This function finds peer eNodeB Cb from WrEmmCb 
 *
 * @details 
 *
 *     Function: wrEmmAbsVctmGetPeerEnbCb
 *
 *         Processing steps:
 *
 *
 * @param[in]   enbId   : Identifier of eNodeB id
 * @param[out]  peerEnbCb : Pointer to peer eNodeB Cb 
 * @return S16
 *     -# Success : ROK
 *     -# Failure : RFAILED
 */
PRIVATE S16 wrEmmAbsVctmGetPeerEnbCb
(
WrEmmAbsEnbId                 *enbId,
WrEmmAbsVctmPeerEnbCb         **peerEnbCb
)
{
   CmLList                    *node = NULLP;

   TRC3(wrEmmAbsVctmGetPeerEnbCb)

   node = wrEmmCb.lteAdvCb.absInfo.peerInfo.u.vctmInfo.absPeerEnbLstCp.first;
   while(NULLP != node)
   {
      *peerEnbCb = (WrEmmAbsVctmPeerEnbCb*)node->node;
      if((*peerEnbCb)->enbId.val == enbId->val)
      {
         RETVALUE(ROK);
      }
      node =node->next;
   }

   *peerEnbCb = NULLP;
   RETVALUE(RFAILED);
}

/** @brief This function removes peer eNodeB Cb from WrEmmCb and deallocate it. 
 *
 * @details 
 *
 *     Function: wrEmmAbsVctmRemoveNDeallocPeerEnbCb
 *
 *         Processing steps:
 *             1. Remove all cells linked to eNodeB Cb
 *             2. Remove from list and deallocate it
 *
 * @param[in]  peerEnbCb : Pointer to peer eNodeB Cb 
 * @return S16
 *     -# Success : ROK
 *     -# Failure : RFAILED
 */
PRIVATE S16 wrEmmAbsVctmRemoveNDeallocPeerEnbCb
(
WrEmmAbsVctmPeerEnbCb                *peerEnbCb
)
{
   CmLList                           *node = NULLP;
   WrEmmAbsVctmPeerCellCb            *peerCellCb;

   TRC3(wrEmmAbsVctmRemoveNDeallocPeerEnbCb)

   if(NULLP == peerEnbCb)
   {
      RLOG0(L_ERROR, " Null pointer passed");
      RETVALUE(RFAILED);
   }

   node = peerEnbCb->absPeerCellLstCp.first;
   while(NULLP != node)
   {
      peerCellCb = (WrEmmAbsVctmPeerCellCb*)node->node;
      node       = node->next;

      wrEmmAbsVctmRemoveNDeallocPeerCellCb(peerEnbCb, peerCellCb);
   }
   wrStopTmr((PTR)peerEnbCb, WR_TMR_ABS_LOAD_INFO_TMR);
   cmLListDelFrm(&wrEmmCb.lteAdvCb.absInfo.peerInfo.u.vctmInfo.absPeerEnbLstCp, &peerEnbCb->lnk);

   WR_FREE(peerEnbCb, sizeof(WrEmmAbsVctmPeerEnbCb));
   RETVALUE(ROK);
}

/**********************************************************************/
/* IMPLEMENTATION OF MESSAGE BUILD AND SEND                           */
/**********************************************************************/

/** @brief This function builds load information message
 *
 * @details 
 *
 *     Function: wrEmmAbsBuildLoadInfo
 *
 *         Processing steps:
 *
 *
 * @param[in]   ieId    : Id of IE
 * @param[in]   choice  : Choice within IE
 * @param[out]  pdu     : Pointer to X2AP event 
 * @return S16
 *     -# Success : ROK
 *     -# Failure : RFAILED
 */
PRIVATE S16 wrEmmAbsBuildLoadInfo
(
U8                                          ieId, 
U8                                          choice,
CztEvnt                                     **pdu
)
{
   S16                                      ret;
   CztEvnt                                  *x2LoadInfoPdu;
   CztLoadInform                            *x2LoadInfo;
   U8                                       ieIdx = 0;
   U8                                       numComp;
   U8                                       len;   /* KW fix for LTE_ADV */ 
   WrCellCb                                 *cellCb;
   CztProtIE_Field_CellInform_ItemIEs       *protoie;
   CztProtIE_Field_LoadInform_IEs           *ie;
   CztProtExtnField_CellInform_Item_ExtIEs  *extie;

   TRC2(wrEmmAbsBuildLoadInfo)

   WR_GET_ABS_CELLCB(cellCb);

   /* Allocate and Initialize memory control point */
   ret = cmAllocEvnt(sizeof(CztEvnt), WR_CZ_MEM_SDU_SIZE,
                            &wrCb.mem, (Ptr *)&x2LoadInfoPdu);
   if(ret != ROK)
   {
      RLOG0(L_FATAL, "Memory allocation failed.");
      RETVALUE(RFAILED);
   }

   /* Initialize the PDU */
   cmMemset((U8 *)&(x2LoadInfoPdu->pdu), 0, sizeof(CztX2AP_PDU));

   /* Load Information message header info */
   wrFillTknU8(&(x2LoadInfoPdu->pdu.choice),X2AP_PDU_INITIATINGMSG);
   wrFillTknU8(&(x2LoadInfoPdu->pdu.val.initiatingMsg.pres),PRSNT_NODEF);
   wrFillTknU32(&(x2LoadInfoPdu->pdu.val.initiatingMsg.procedureCode),
                                                           Cztid_loadInd);
   wrFillTknU32(&(x2LoadInfoPdu->pdu.val.initiatingMsg.criticality),
                                                CztCriticalityignoreEnum);
   wrFillTknU8(&(x2LoadInfoPdu->pdu.val.initiatingMsg.value.u.
                                       cztLoadInform.pres), PRSNT_NODEF);

   x2LoadInfo = &x2LoadInfoPdu->pdu.val.initiatingMsg.
                                    value.u.cztLoadInform;

   /* Allocate and fill Cell Information */
   numComp = 1;
   wrFillTknU16(&(x2LoadInfo->protocolIEs.noComp), numComp);
   if ((cmGetMem(x2LoadInfoPdu, (numComp *
                              sizeof(CztProtIE_Field_LoadInform_IEs)),
                              (Ptr*)&x2LoadInfo->protocolIEs.member)) != ROK)
   {
      RLOG0(L_FATAL, "Memory allocation failed.");
      WR_FREE_EVNT(x2LoadInfoPdu);
      RETVALUE(RFAILED);
   } 
   ie = &x2LoadInfo->protocolIEs.member[ieIdx];
   wrFillTknU8(&(ie->pres), PRSNT_NODEF);
   wrFillTknU32(&(ie->id), Cztid_CellInform);
   wrFillTknU32(&(ie->criticality), CztCriticalityignoreEnum);

   numComp = 1; 
   if(!numComp)
   {
      RLOG0(L_ERROR, " Number of cells are zero");
      WR_FREE_EVNT(x2LoadInfoPdu);
      RETVALUE(RFAILED);
   }

   wrFillTknU16(&(ie->value.u.cztCellInform_Lst.noComp), numComp);
   if ((cmGetMem(x2LoadInfoPdu,(numComp * sizeof(CztProtIE_Field_CellInform_ItemIEs)),
                                  (Ptr *)&ie->value.u.cztCellInform_Lst.member)) !=ROK)
   {
      RLOG0(L_FATAL, "Memory allocation failed.");
      WR_FREE_EVNT(x2LoadInfoPdu);
      RETVALUE(RFAILED);
   }

   for(ieIdx = 0; ieIdx < numComp; ieIdx++)
   {
      protoie = &ie->value.u.cztCellInform_Lst.member[ieIdx];
      wrFillTknU8(&(protoie->pres), PRSNT_NODEF);
      wrFillTknU32(&(protoie->id), Cztid_CellInform_Item);
      wrFillTknU32(&(protoie->criticality), CztCriticalityignoreEnum);
      wrFillTknU8(&(protoie->value.u.cztCellInform_Item.pres), PRSNT_NODEF);
      wrFillTknU8(&(protoie->value.u.cztCellInform_Item.cell_ID.pres), PRSNT_NODEF);
      /* Using the first configured cell and the first PLMN ID in the list */
      wrEmmLteAdvCztFillPLMNId(x2LoadInfoPdu,
                         &(cellCb->sib1.plmns[0].plmnId),
                         &(protoie->value.u.cztCellInform_Item.cell_ID.pLMN_Identity));
      len = 28;
      wrFillTknBStr32(&(protoie->value.u.cztCellInform_Item.cell_ID.eUTRANcellIdentifier),
                              len, wrEmmCb.cellCb[0]->sib1.cellIdentity);


      /* Allocate and fill ABS information */
      numComp = 1;
      wrFillTknU16(&(protoie->value.u.cztCellInform_Item.iE_Extns.noComp), numComp);

      if ((cmGetMem(x2LoadInfoPdu,(numComp * sizeof(CztProtExtnField_CellInform_Item_ExtIEs)),
                          (Ptr *)&protoie->value.u.cztCellInform_Item.iE_Extns.member)) !=ROK)
      {
         RLOG0(L_FATAL, "Memory allocation failed.");
         WR_FREE_EVNT(x2LoadInfoPdu);
         RETVALUE(RFAILED);
      }
  
      extie = &protoie->value.u.cztCellInform_Item.iE_Extns.member[0];
      wrFillTknU8(&(extie->pres), PRSNT_NODEF);
      wrFillTknU32(&(extie->id), ieId);
      wrFillTknU32(&(extie->criticality), CztCriticalityignoreEnum);
      switch(ieId)
      {
         case Cztid_ABSInform:
         {
            wrFillTknU8(&(extie->extensionValue.u.cztABSInform.choice), choice); 
            if(ABSINFORM_FDD == choice)
            {
               wrFillTknU8(&(extie->extensionValue.u.cztABSInform.val.fdd.pres), PRSNT_NODEF);
               WR_EMM_ABS_FILL_STRBSXL(x2LoadInfoPdu, (extie->extensionValue.u.cztABSInform.val.
                     fdd.abs_pattern_info), cellCb->lteAdvCb.absInfo.absPattern, WR_EMM_ABS_PATTERN_SIZE);
 
               wrFillTknU32(&(extie->extensionValue.u.cztABSInform.val.fdd.numberOfCellSpecificAntennaPorts),
                                                                                   wrEmmCb.numTxAntenna);

               WR_EMM_ABS_FILL_STRBSXL(x2LoadInfoPdu, (extie->extensionValue.u.cztABSInform.val.
                   fdd.measurement_subset), cellCb->lteAdvCb.absInfo.absPattern, WR_EMM_ABS_PATTERN_SIZE);
            } 
            else if(ABSINFORM_ABS_INACTIVE == choice)
            {
               /* Do nothing */
            }
            else
            {
               /* TDD choice is not supported */
               RLOG1(L_WARNING, " Invalid choice(%d)", choice);
               WR_FREE_EVNT(x2LoadInfoPdu);
               RETVALUE(RFAILED);
            }
            break;
         }
         case Cztid_InvokeInd:
         {
            wrFillTknU32(&(extie->extensionValue.u.cztInvokeInd), CztInvokeIndabs_informationEnum);
            break;
         }
         default:
         {
            RLOG1(L_WARNING, " Invalid IE(%d)", ieId);
            WR_FREE_EVNT(x2LoadInfoPdu);
            RETVALUE(RFAILED);
         }
      }
   }
 
   *pdu = x2LoadInfoPdu;

   RETVALUE(ROK);   
}


/** @brief This function builds the Resource Status Request Message
 *
 * @details 
 *
 *     Function: wrEmmAbsBuildRsrcStsReq
 *
 *         Processing steps:
 *
 *
 * @param[in]  peerEnbCb  : Pointer to peer eNodeB Cb
 * @param[in]  regReq     : Registrtion request type
 * @param[out] pdu        : Pointer to X2AP pdu  
 * @return S16
 *     -# Success : ROK
 *     -# Failure : RFAILED
 */
PRIVATE S16 wrEmmAbsBuildRsrcStsReq
(
WrEmmAbsAggrPeerEnbCb                           *peerEnbCb,
U8                                              regReq,
CztEvnt                                         **pdu
)
{
   S16                                          ret;
   U8                                           idx;
   CztEvnt                                      *x2RsrcStsReqPdu;
   CztResStatusRqst                             *x2RsrcStsReq;
   U8                                           ieIdx = 0;
   U16                                          numComp;   /* KW fix for LTE_ADV */   
   U8                                           len;     /* KW fix for LTE_ADV */ 
   CztProtIE_Field_ResStatusRqst_IEs            *ie;
   CztProtIE_Single_Cont_CellToReport_ItemIEs   *cellToRprtIe;
   CmLList                                      *node;
   WrEmmAbsAggrPeerCellCb                       *peerCellCb;
   WrCellCb                                     *cellCb;
   U32                                          rptngPeriod;

   TRC2(wrEmmAbsBuildRsrcStsReq)

   /* Allocate and Initialize memory control point */
   ret = cmAllocEvnt(sizeof(CztEvnt), WR_CZ_MEM_SDU_SIZE,
                            &wrCb.mem, (Ptr *)&x2RsrcStsReqPdu);
   if(ret != ROK)
   {
      RLOG0(L_FATAL, "Memory allocation failed.");
      RETVALUE(RFAILED);
   }
  
   /* Initialize the PDU */
   cmMemset((U8 *)&(x2RsrcStsReqPdu->pdu), 0, sizeof(CztX2AP_PDU));

   /* Resource Status Request message header info */
   wrFillTknU8(&(x2RsrcStsReqPdu->pdu.choice),X2AP_PDU_INITIATINGMSG);
   wrFillTknU8(&(x2RsrcStsReqPdu->pdu.val.initiatingMsg.pres),PRSNT_NODEF);
   wrFillTknU32(&(x2RsrcStsReqPdu->pdu.val.initiatingMsg.procedureCode),
                                    Cztid_resourceStatusRprtngInitiation);
   wrFillTknU32(&(x2RsrcStsReqPdu->pdu.val.initiatingMsg.criticality),
                                                CztCriticalityrejectEnum);
   wrFillTknU8(&(x2RsrcStsReqPdu->pdu.val.initiatingMsg.value.u.
                                       cztResStatusRqst.pres), PRSNT_NODEF);

   x2RsrcStsReq = &x2RsrcStsReqPdu->pdu.val.initiatingMsg.
                                    value.u.cztResStatusRqst;

   /* Allocate and fill IEs of Resource Status Request */
   numComp = 5;

   wrFillTknU16(&(x2RsrcStsReq->protocolIEs.noComp), numComp);
   if ((cmGetMem(x2RsrcStsReqPdu, (numComp *
                              sizeof(CztProtIE_Field_ResStatusRqst_IEs)),
                              (Ptr*)&x2RsrcStsReq->protocolIEs.member)) != ROK)
   {
      RLOG0(L_FATAL, "Memory allocation failed.");
      WR_FREE_EVNT(x2RsrcStsReqPdu);
      RETVALUE(RFAILED);
   } 

   /* Filling eNB1 Measurement ID */
   ie = &x2RsrcStsReq->protocolIEs.member[ieIdx];
   wrFillTknU8(&(ie->pres), PRSNT_NODEF);
   wrFillTknU32(&(ie->id), Cztid_ENB1_Measurement_ID);
   wrFillTknU32(&(ie->criticality), CztCriticalityrejectEnum);
   wrFillTknU32(&(ie->value.u.cztid_ENB1_Measurement_ID), peerEnbCb->enb1MeasId);
   ieIdx++;

   /* Filling eNB2 Measurement ID */
   if(CztRegistration_RqststopEnum == regReq)
   {
      ie = &x2RsrcStsReq->protocolIEs.member[ieIdx];
      wrFillTknU8(&(ie->pres), PRSNT_NODEF);
      wrFillTknU32(&(ie->id), Cztid_ENB2_Measurement_ID);
      wrFillTknU32(&(ie->criticality), CztCriticalityignoreEnum);
      wrFillTknU32(&(ie->value.u.cztid_ENB2_Measurement_ID), peerEnbCb->enb2MeasId);   
      ieIdx++;
   }

   ie = &x2RsrcStsReq->protocolIEs.member[ieIdx];
   wrFillTknU8(&(ie->pres), PRSNT_NODEF);
   wrFillTknU32(&(ie->id), Cztid_Registration_Rqst);
   wrFillTknU32(&(ie->criticality), CztCriticalityrejectEnum);
   wrFillTknU32(&(ie->value.u.cztRegistration_Rqst), regReq);  
   ieIdx++;

   ie = &x2RsrcStsReq->protocolIEs.member[ieIdx];
   wrFillTknU8(&(ie->pres), PRSNT_NODEF);
   wrFillTknU32(&(ie->id), Cztid_ReportCharacteristics);
   wrFillTknU32(&(ie->criticality), CztCriticalityrejectEnum);
   wrFillTknBStr32(&(ie->value.u.cztReportCharacteristics), 32, 
                                   WR_EMM_ABS_RPT_CHARACTERISTICS);  
   ieIdx++;

   ie = &x2RsrcStsReq->protocolIEs.member[ieIdx];
   wrFillTknU8(&(ie->pres), PRSNT_NODEF);
   wrFillTknU32(&(ie->id), Cztid_CellToReport);
   wrFillTknU32(&(ie->criticality), CztCriticalityignoreEnum);

   numComp = (U16)peerEnbCb->absPeerCellLstCp.count;  /* KW fix for LTE_ADV */   
   if(!numComp)
   {
      RLOG0(L_ERROR, " Number of cells are zero");
      WR_FREE_EVNT(x2RsrcStsReqPdu);
      RETVALUE(RFAILED);
   }

   wrFillTknU16(&(ie->value.u.cztCellToReport_Lst.noComp), numComp);
   if ((cmGetMem(x2RsrcStsReqPdu, (numComp *
                              sizeof(CztProtIE_Single_Cont_CellToReport_ItemIEs)),
                              (Ptr*)&ie->value.u.cztCellToReport_Lst.member)) != ROK)
   {
      RLOG0(L_FATAL, "Memory allocation failed.");
      WR_FREE_EVNT(x2RsrcStsReqPdu);
      RETVALUE(RFAILED);
   } 

   node = peerEnbCb->absPeerCellLstCp.first;
   for(idx = 0; idx < numComp; idx++)
   {
      peerCellCb = (WrEmmAbsAggrPeerCellCb*) node->node;
      node = node->next;

      cellToRprtIe = &ie->value.u.cztCellToReport_Lst.member[idx]; 
      wrFillTknU8(&(cellToRprtIe->pres), PRSNT_NODEF);
      wrFillTknU32(&(cellToRprtIe->id), Cztid_CellToReport_Item);
      wrFillTknU32(&(cellToRprtIe->criticality), CztCriticalityignoreEnum);
      wrFillTknU8(&(cellToRprtIe->value.u.cztCellToReport_Item.pres), PRSNT_NODEF);
      wrFillTknU8(&(cellToRprtIe->value.u.cztCellToReport_Item.cell_ID.pres), PRSNT_NODEF);
      wrEmmLteAdvCztFillPLMNId(x2RsrcStsReqPdu,
                       &(peerCellCb->cellId.plmnId),
                       &(cellToRprtIe->value.u.cztCellToReport_Item.cell_ID.pLMN_Identity));
      len = 28;
      wrFillTknBStr32(&(cellToRprtIe->value.u.cztCellToReport_Item.cell_ID.eUTRANcellIdentifier),
                           len, peerCellCb->cellId.cId);
   }
   ieIdx++;

   if(CztRegistration_RqststartEnum == regReq)
   {
      WR_GET_ABS_CELLCB(cellCb);
      if((cellCb == NULLP))
      {
          RLOG0(L_ERROR, " CELLCB not found ");
          RETVALUE(RFAILED);   
      }  /*KW fix for LTE_ADV */    
      ie = &x2RsrcStsReq->protocolIEs.member[ieIdx];
      wrFillTknU8(&(ie->pres), PRSNT_NODEF);
      wrFillTknU32(&(ie->id), Cztid_RprtngPeriodicity);
      wrFillTknU32(&(ie->criticality), CztCriticalityignoreEnum);
      WR_EMM_ABS_CONVERT_NEGSEC_ENUM((cellCb->lteAdvCb.lteAdvCfg.absCfg.absLoadPeriodicity), (rptngPeriod));
      wrFillTknU32(&(ie->value.u.cztRprtngPeriodicity), rptngPeriod);
      ieIdx++;
   }

   *pdu = x2RsrcStsReqPdu;

   RETVALUE(ROK);   
}


/** @brief This function builds Resources Status Update message
 *
 * @details 
 *
 *     Function: wrEmmAbsBuildRsrcStsUpd
 *
 *         Processing steps:
 *
 *
 * @param[in]   peerEnbCb : Pointer to peer eNodeB Cb
 * @param[out]  pdu       : Pointer to X2AP pdu
 * @return S16
 *     -# Success : ROK
 *     -# Failure : RFAILED
 */
PRIVATE S16 wrEmmAbsBuildRsrcStsUpd 
(
WrEmmAbsVctmPeerEnbCb                 *peerEnbCb,
CztEvnt                               **pdu
)
{
   S16                                ret;
   U8                                 len;   /* KW fix for LTE_ADV */ 
   U8                                 numComp;
   U8                                 ieIdx = 0;
   WrCellCb                           *cellCb;
   CztEvnt                            *rsrcStsUpdPdu;
   CztResStatusUpd                    *rsrcStsUpd;
   CztProtIE_Field_ResStatusUpd_IEs   *ie;
   CztProtIE_Single_Cont_CellMeasurementResult_ItemIEs *measRslt;
   CztProtExtnField_CellMeasurementResult_Item_ExtIEs *extIes;

   TRC2(wrEmmAbsBuildRsrcStsUpd)

   /* Allocate and Initialize memory control point */
   ret = cmAllocEvnt(sizeof(CztEvnt), WR_CZ_MEM_SDU_SIZE,
                            &wrCb.mem, (Ptr *)&rsrcStsUpdPdu);
   if(ret != ROK)
   {
      RLOG0(L_FATAL, "Memory allocation failed.");
      RETVALUE(RFAILED);
   }

   /* Initialize the PDU */
   cmMemset((U8 *)&(rsrcStsUpdPdu->pdu), 0, sizeof(CztX2AP_PDU));

   /* Load Information message header info */
   wrFillTknU8(&(rsrcStsUpdPdu->pdu.choice),X2AP_PDU_INITIATINGMSG);
   wrFillTknU8(&(rsrcStsUpdPdu->pdu.val.initiatingMsg.pres),PRSNT_NODEF);
   wrFillTknU32(&(rsrcStsUpdPdu->pdu.val.initiatingMsg.procedureCode),
                                                     Cztid_resourceStatusRprtng);
   wrFillTknU32(&(rsrcStsUpdPdu->pdu.val.initiatingMsg.criticality),
                                                CztCriticalityignoreEnum);
   wrFillTknU8(&(rsrcStsUpdPdu->pdu.val.initiatingMsg.value.u.
                                       cztResStatusUpd.pres), PRSNT_NODEF);
   
   
   rsrcStsUpd = &rsrcStsUpdPdu->pdu.val.initiatingMsg.value.u.cztResStatusUpd;

   numComp = 3;
   wrFillTknU16(&(rsrcStsUpd->protocolIEs.noComp), numComp);
   if ((cmGetMem(rsrcStsUpdPdu, (numComp *
                              sizeof(CztProtIE_Field_ResStatusUpd_IEs)),
                              (Ptr*)&rsrcStsUpd->protocolIEs.member)) != ROK)
   {
      RLOG0(L_FATAL, "Memory allocation failed.");
      WR_FREE_EVNT(rsrcStsUpd);
      RETVALUE(RFAILED);
   }

   ie = &rsrcStsUpd->protocolIEs.member[ieIdx];
   wrFillTknU8(&(ie->pres), PRSNT_NODEF);
   wrFillTknU32(&(ie->id), Cztid_ENB1_Measurement_ID);
   wrFillTknU32(&(ie->criticality), CztCriticalityrejectEnum);
   wrFillTknU32(&(ie->value.u.cztid_ENB1_Measurement_ID), peerEnbCb->enb1MeasId);
   ieIdx++;

   ie = &rsrcStsUpd->protocolIEs.member[ieIdx];
   wrFillTknU8(&(ie->pres), PRSNT_NODEF);
   wrFillTknU32(&(ie->id), Cztid_ENB2_Measurement_ID);
   wrFillTknU32(&(ie->criticality), CztCriticalityrejectEnum);
   wrFillTknU32(&(ie->value.u.cztid_ENB2_Measurement_ID), peerEnbCb->enb2MeasId);
   ieIdx++;

   ie = &rsrcStsUpd->protocolIEs.member[ieIdx];
   wrFillTknU8(&(ie->pres), PRSNT_NODEF);
   wrFillTknU32(&(ie->id), Cztid_CellMeasurementResult);
   wrFillTknU32(&(ie->criticality), CztCriticalityignoreEnum);


   /* Fill srs cell id and ABS status */
   numComp = 1;
   wrFillTknU16(&(ie->value.u.cztCellMeasurementResult_Lst.noComp), numComp);
 
   if ((cmGetMem(rsrcStsUpdPdu,(numComp * sizeof(CztProtIE_Single_Cont_CellMeasurementResult_ItemIEs)),
                       (Ptr *)&ie->value.u.cztCellMeasurementResult_Lst.member)) !=ROK)
   {
      RLOG0(L_FATAL, "Memory allocation failed.");
      WR_FREE_EVNT(rsrcStsUpdPdu);
      RETVALUE(RFAILED);
   }

   /* Fill cell id */
   measRslt = &ie->value.u.cztCellMeasurementResult_Lst.member[0];

   WR_GET_ABS_CELLCB(cellCb);
   if((cellCb == NULLP))
   {
       RLOG0(L_ERROR, " CELLCB not found ");
       RETVALUE(RFAILED);   
   }  /*KW fix for LTE_ADV */    
   wrFillTknU8(&(measRslt->pres), PRSNT_NODEF);
   wrFillTknU32(&(measRslt->id), Cztid_CellMeasurementResult_Item);
   wrFillTknU32(&(measRslt->criticality), CztCriticalityignoreEnum);
   wrFillTknU8(&(measRslt->value.u.cztCellMeasurementResult_Item.pres), PRSNT_NODEF);
   wrFillTknU8(&(measRslt->value.u.cztCellMeasurementResult_Item.cell_ID.pres), PRSNT_NODEF);
   /* Using the first configured cell and the first PLMN ID in the list */
   wrEmmLteAdvCztFillPLMNId(rsrcStsUpdPdu,
                      &(cellCb->sib1.plmns[0].plmnId),
                      &(measRslt->value.u.cztCellMeasurementResult_Item.cell_ID.pLMN_Identity));
   len = 28;
   wrFillTknBStr32(&(measRslt->value.u.cztCellMeasurementResult_Item.cell_ID.eUTRANcellIdentifier),
                     len, wrEmmCb.cellCb[0]->sib1.cellIdentity);


   numComp = 1;
   wrFillTknU16(&(measRslt->value.u.cztCellMeasurementResult_Item.
                                          iE_Extns.noComp), numComp);
   
   if ((cmGetMem(rsrcStsUpdPdu, (numComp * sizeof(CztProtExtnField_CellMeasurementResult_Item_ExtIEs)),
                   (Ptr *)&measRslt->value.u.cztCellMeasurementResult_Item.iE_Extns.member)) !=ROK)
   {
      RLOG0(L_FATAL, "Memory allocation failed.");
      WR_FREE_EVNT(rsrcStsUpdPdu);
      RETVALUE(RFAILED);
   }

   extIes = &measRslt->value.u.cztCellMeasurementResult_Item.iE_Extns.member[0];   

   wrFillTknU8(&(extIes->pres), PRSNT_NODEF);
   wrFillTknU32(&(extIes->id), Cztid_ABS_Status);
   wrFillTknU32(&(extIes->criticality), CztCriticalityignoreEnum);

   wrFillTknU8(&(extIes->extensionValue.u.cztABS_Status.pres), PRSNT_NODEF);   

   wrFillTknU32(&(extIes->extensionValue.u.cztABS_Status.dL_ABS_status), 
                              cellCb->lteAdvCb.absInfo.u.vctmInfo.absPercentageRsrcUsed);

   wrFillTknU8(&(extIes->extensionValue.u.cztABS_Status.usableABSInform.choice),
                                                               USABLEABSINFORM_FDD);
   wrFillTknU8(&(extIes->extensionValue.u.cztABS_Status.usableABSInform.val.fdd.pres), 
                                                                PRSNT_NODEF);
 
   WR_EMM_ABS_FILL_STRBSXL(rsrcStsUpdPdu, (extIes->extensionValue.u.cztABS_Status.usableABSInform.val.fdd.\
            usable_abs_pattern_info), cellCb->lteAdvCb.absInfo.absPattern, WR_EMM_ABS_PATTERN_SIZE);
   
   *pdu = rsrcStsUpdPdu;

   RETVALUE(ROK);
}

/** @brief This function builds Resource Status Failure message

 *
 * @details 
 *
 *     Function: wrEmmAbsBuildRsrcStsFlr
 *
 *         Processing steps:
 *
 *
 * @param[in]   peerEnbCb : Pointer to peer eNodeB Cb
 * @param[in]   choice    : Choice of cause 
 * @param[in]   cause     : Value of cause
 * @param[out]  pdu       : Pointer to X2AP pdu
 * @return S16
 *     -# Success : ROK
 *     -# Failure : RFAILED
 */
PRIVATE S16 wrEmmAbsBuildRsrcStsFlr 
(
WrEmmAbsVctmPeerEnbCb                 *peerEnbCb, 
U8                                    choice, 
U32                                   cause,
CztEvnt                               **pdu
)
{
   S16                                ret;
   U8                                 ieIdx = 0;
   U8                                 numComp;
   CztEvnt                            *rsrcStsFlrPdu;
   CztResStatusFail                   *rsrcStsFlr;
   CztProtIE_Field_ResStatusFail_IEs  *ie;

   TRC2(wrEmmAbsBuildRsrcStsFlr)

   /* Allocate and Initialize memory control point */
   ret = cmAllocEvnt(sizeof(CztEvnt), WR_CZ_MEM_SDU_SIZE,
                            &wrCb.mem, (Ptr *)&rsrcStsFlrPdu);
   if(ret != ROK)
   {
      RLOG0(L_FATAL, "Memory allocation failed.");
      RETVALUE(RFAILED);
   }

   /* Initialize the PDU */
   cmMemset((U8 *)&(rsrcStsFlrPdu->pdu), 0, sizeof(CztX2AP_PDU));

   /* Load Information message header info */
   wrFillTknU8(&(rsrcStsFlrPdu->pdu.choice),X2AP_PDU_UNSUCCESSFULOUTCOME);
   wrFillTknU8(&(rsrcStsFlrPdu->pdu.val.unsuccessfulOutcome.pres),PRSNT_NODEF);
   wrFillTknU32(&(rsrcStsFlrPdu->pdu.val.unsuccessfulOutcome.procedureCode),
                                                           Cztid_resourceStatusRprtngInitiation);
   wrFillTknU32(&(rsrcStsFlrPdu->pdu.val.unsuccessfulOutcome.criticality),
                                                CztCriticalityignoreEnum);
   wrFillTknU8(&(rsrcStsFlrPdu->pdu.val.unsuccessfulOutcome.value.u.
                                       cztResStatusFail.pres), PRSNT_NODEF);

   rsrcStsFlr = &rsrcStsFlrPdu->pdu.val.unsuccessfulOutcome.
                                    value.u.cztResStatusFail;

   numComp = 3;
   wrFillTknU16(&(rsrcStsFlr->protocolIEs.noComp), numComp);
   if ((cmGetMem(rsrcStsFlrPdu, (numComp *
                              sizeof(CztProtIE_Field_ResStatusFail_IEs)),
                              (Ptr*)&rsrcStsFlr->protocolIEs.member)) != ROK)
   {
      RLOG0(L_FATAL, "Memory allocation failed.");
      WR_FREE_EVNT(rsrcStsFlrPdu);
      RETVALUE(RFAILED);
   }

   ie = &rsrcStsFlr->protocolIEs.member[ieIdx];
   wrFillTknU8(&(ie->pres), PRSNT_NODEF);
   wrFillTknU32(&(ie->id), Cztid_ENB1_Measurement_ID);
   wrFillTknU32(&(ie->criticality), CztCriticalityrejectEnum);
   wrFillTknU32(&(ie->value.u.cztid_ENB1_Measurement_ID), peerEnbCb->enb1MeasId);
   ieIdx++;

   ie = &rsrcStsFlr->protocolIEs.member[ieIdx];
   wrFillTknU8(&(ie->pres), PRSNT_NODEF);
   wrFillTknU32(&(ie->id), Cztid_ENB2_Measurement_ID);
   wrFillTknU32(&(ie->criticality), CztCriticalityrejectEnum);
   wrFillTknU32(&(ie->value.u.cztid_ENB2_Measurement_ID), peerEnbCb->enb2MeasId);
   ieIdx++;

   ie = &rsrcStsFlr->protocolIEs.member[ieIdx];
   wrFillTknU8(&(ie->pres), PRSNT_NODEF);
   wrFillTknU32(&(ie->id), Cztid_Cause);
   wrFillTknU32(&(ie->criticality), CztCriticalityignoreEnum);
   wrFillTknU8(&(ie->value.u.cztCause.choice), choice);
   wrFillTknU32(&(ie->value.u.cztCause.val.misc), cause);

   *pdu = rsrcStsFlrPdu;
   
   RETVALUE(ROK);
}

/** @brief This function builds Resource Status Response message
 *
 * @details 
 *
 *     Function: wrEmmAbsBuildRsrcStsRsp
 *
 *         Processing steps:
 *
 *
 * @param[in]   peerEnbCb : Pointer to peer eNodeB Cb
 * @param[out]  pdu       : Pointer to X2AP pdu
 * @return S16
 *     -# Success : ROK
 *     -# Failure : RFAILED
 */
PRIVATE S16 wrEmmAbsBuildRsrcStsRsp 
(
WrEmmAbsVctmPeerEnbCb                 *peerEnbCb, 
CztEvnt                               **pdu
)
{
   U8                                 ieIdx = 0;
   S16                                ret;
   U8                                 numComp;
   CztEvnt                            *rsrcStsRspPdu;
   CztResStatusResp                   *rsrcStsRsp;
   CztProtIE_Field_ResStatusResp_IEs  *ie;

   TRC2(wrEmmAbsBuildRsrcStsRsp)

   /* Allocate and Initialize memory control point */
   ret = cmAllocEvnt(sizeof(CztEvnt), WR_CZ_MEM_SDU_SIZE,
                            &wrCb.mem, (Ptr *)&rsrcStsRspPdu);
   if(ret != ROK)
   {
      RLOG0(L_FATAL, "Memory allocation failed.");
      RETVALUE(RFAILED);
   }

   /* Initialize the PDU */
   cmMemset((U8 *)&(rsrcStsRspPdu->pdu), 0, sizeof(CztX2AP_PDU));

   /* Load Information message header info */
   wrFillTknU8(&(rsrcStsRspPdu->pdu.choice),X2AP_PDU_SUCCESSFULOUTCOME);
   wrFillTknU8(&(rsrcStsRspPdu->pdu.val.successfulOutcome.pres),PRSNT_NODEF);
   wrFillTknU32(&(rsrcStsRspPdu->pdu.val.successfulOutcome.procedureCode),
                                                           Cztid_resourceStatusRprtngInitiation);
   wrFillTknU32(&(rsrcStsRspPdu->pdu.val.successfulOutcome.criticality),
                                                CztCriticalityignoreEnum);
   wrFillTknU8(&(rsrcStsRspPdu->pdu.val.successfulOutcome.value.u.
                                       cztResStatusResp.pres), PRSNT_NODEF);

   rsrcStsRsp = &rsrcStsRspPdu->pdu.val.successfulOutcome.
                                    value.u.cztResStatusResp;

   numComp = 2;
   wrFillTknU16(&(rsrcStsRsp->protocolIEs.noComp), numComp);
   if ((cmGetMem(rsrcStsRspPdu, (numComp *
                              sizeof(CztProtIE_Cont_ResStatusResp_IEs)),
                              (Ptr*)&rsrcStsRsp->protocolIEs.member)) != ROK)
   {
      RLOG0(L_FATAL, "Memory allocation failed.");
      WR_FREE_EVNT(rsrcStsRspPdu);
      RETVALUE(RFAILED);
   }
   ie = &rsrcStsRsp->protocolIEs.member[ieIdx];
   wrFillTknU8(&(ie->pres), PRSNT_NODEF);
   wrFillTknU32(&(ie->id), Cztid_ENB1_Measurement_ID);
   wrFillTknU32(&(ie->criticality), CztCriticalityrejectEnum);
   wrFillTknU32(&(ie->value.u.cztid_ENB1_Measurement_ID), peerEnbCb->enb1MeasId);
   ieIdx++;

   ie = &rsrcStsRsp->protocolIEs.member[ieIdx];
   wrFillTknU8(&(ie->pres), PRSNT_NODEF);
   wrFillTknU32(&(ie->id), Cztid_ENB2_Measurement_ID);
   wrFillTknU32(&(ie->criticality), CztCriticalityrejectEnum);
   wrFillTknU32(&(ie->value.u.cztid_ENB2_Measurement_ID), peerEnbCb->enb2MeasId);

   *pdu = rsrcStsRspPdu;
   
   RETVALUE(ROK);
}

/** @brief This function builds and sends Load Information message to X2AP
 *
 * @details 
 *
 *     Function: wrEmmAbsBuildAndSendLoadInfo
 *
 *         Processing steps:
 *
 * @param[in]   enbId   : Identifieres of eNodeB 
 * @param[in]   ieId    : Id of IE
 * @param[in]   choice  : Choice within IE
 * @return S16
 *     -# Success : ROK
 *     -# Failure : RFAILED
 */
PRIVATE S16 wrEmmAbsBuildAndSendLoadInfo 
(
WrEmmAbsEnbId                       *enbId,
U8                                  ieId, 
U8                                  choice
)
{
   S16                              ret;
   CztEvnt                          *x2LoadInfo;
   WrNeighEnbCb                     *nghEnbCb;
   U32                              enbID = 0;

   TRC2(wrEmmAbsBuildAndSendLoadInfo)

   ret = cmHashListFind(&(wrEmmCb.neighEnbLstCp), (U8 *)&(enbId->val),
                      (U16)sizeof(enbId->val), 0,(PTR *)&nghEnbCb);
   if (ROK != ret)
   {
      RLOG0(L_ERROR, " Failure in the Hash List find");
      RETVALUE(RFAILED);
   }
   WR_GET_ENBID_FROM_PEERID(enbID,nghEnbCb->peerId);
   
   if(ROK != wrEmmAbsBuildLoadInfo(ieId, choice, &x2LoadInfo))
   {
      RLOG0(L_ERROR, " Build Load Information message failed ");
      RETVALUE(RFAILED);
   }

   x2LoadInfo->peerId = nghEnbCb->peerId;
   if (ROK != WrIfmX2apGpReq(x2LoadInfo))
   {
      RLOG0(L_ERROR, " Failed to Send Load Information message ");
   }
   else
   {
      RLOG2(L_EVENT,"[X2AP]:Sending LOAD INFORMATION [Src(%ld)-->Peer(%ld)]",
            wrEmmCb.enbId,enbID);
   }

   RETVALUE(ROK);
}


/** @brief This function build and sends Resource Status Request to X2AP
 *
 * @details 
 *
 *     Function: wrEmmAbsBuildAndSendRsrcStsReq
 *
 *         Processing steps:
 *
 *
 * @param[in]   peerEnbCb : Pointer peer eNodeB Cb
 * @param[in]   regReq    : Registration Request type   
 * @return S16
 *     -# Success : ROK
 *     -# Failure : RFAILED
 */
PRIVATE S16 wrEmmAbsBuildAndSendRsrcStsReq 
(
WrEmmAbsAggrPeerEnbCb              *peerEnbCb,
U8                                 regReq
)
{
   S16                             ret;
   WrNeighEnbCb                    *nghEnbCb;
   CztEvnt                         *x2RsrcStsReq;
   U32                       enbId = 0;

   TRC2(wrEmmAbsBuildAndSendRsrcStsReq)

   ret = cmHashListFind(&(wrEmmCb.neighEnbLstCp), (U8 *)&(peerEnbCb->enbId.val),
                      (U16)sizeof(peerEnbCb->enbId.val), 0,(PTR *)&nghEnbCb);
   if (ROK != ret)
   {
      RLOG0(L_ERROR, " Failure in the Hash List find");
      RETVALUE(RFAILED);
   }

   WR_GET_ENBID_FROM_PEERID(enbId,nghEnbCb->peerId);
   if(ROK != wrEmmAbsBuildRsrcStsReq(peerEnbCb, regReq, &x2RsrcStsReq))
   {
      RLOG0(L_ERROR, " Failed to build Resource Status Request message ");
      RETVALUE(RFAILED); 
   }

   x2RsrcStsReq->peerId = nghEnbCb->peerId;
   if (ROK != WrIfmX2apGpReq(x2RsrcStsReq))
   {
      RLOG0(L_ERROR, " Failed to send Resource Status Request message ");
      RETVALUE(RFAILED); 
   }
   else
   {
      RLOG2(L_EVENT,"[X2AP]:Sending RESOURCE STATUS REQUEST [Src(%ld)-->Peer(%ld)]",
            wrEmmCb.enbId,enbId);
   }

   RETVALUE(ROK);
}


/** @brief This function build and sends Resource Status Response 
 *
 * @details 
 *
 *     Function: wrEmmAbsBuildAndSendRsrcStsRsp
 *
 *         Processing steps:
 *
 *
 * @param[in]   peerEnbCb : Pointer peer eNodeB Cb
 * @return S16
 *     -# Success : ROK
 *     -# Failure : RFAILED
 */
PRIVATE S16 wrEmmAbsBuildAndSendRsrcStsRsp
(
WrEmmAbsVctmPeerEnbCb           *peerEnbCb
)
{
   S16                          ret;
   WrNeighEnbCb                 *nghEnbCb;
   CztEvnt                      *x2RsrcStsRsp;
   U32                       enbId = 0;

   TRC2(wrEmmAbsBuildAndSendRsrcStsRsp)

   ret = cmHashListFind(&(wrEmmCb.neighEnbLstCp), (U8 *)&(peerEnbCb->enbId.val),
                      (U16)sizeof(peerEnbCb->enbId.val), 0,(PTR *)&nghEnbCb);
   if (ROK != ret)
   {
      RLOG0(L_ERROR, " Failure in the Hash List find ");
      RETVALUE(RFAILED);
   }

   WR_GET_ENBID_FROM_PEERID(enbId,nghEnbCb->peerId);
   if(ROK != wrEmmAbsBuildRsrcStsRsp(peerEnbCb, &x2RsrcStsRsp))
   {
      RLOG0(L_ERROR, "Resource Status Response building failed ");
      RETVALUE(RFAILED);
   }

   x2RsrcStsRsp->peerId = nghEnbCb->peerId;
   if (ROK != WrIfmX2apGpRsp(x2RsrcStsRsp))
   {
      RLOG0(L_ERROR, " Failed to send Resource Status Response message ");
      RETVALUE(RFAILED);
   }
   else
   {
      RLOG2(L_EVENT,"[X2AP]:Sending RESOURCE STATUS RESPONSE [Src(%ld)-->Peer(%ld)]",
            wrEmmCb.enbId,enbId);
   }

   RETVALUE(ROK);
}

/** @brief This function build and sends Resource Status Failure to X2AP
 *
 * @details 
 *
 *     Function: wrEmmAbsBuildAndSendRsrcStsFlr
 *
 *         Processing steps:
 *
 * @param[in]   peerEnbCb : Pointer to peer eNodeB Cb
 * @param[in]   choice    : Choice of cause 
 * @param[in]   cause     : Value of cause
 * @return S16
 *     -# Success : ROK
 *     -# Failure : RFAILED
 */
PRIVATE S16 wrEmmAbsBuildAndSendRsrcStsFlr
(
WrEmmAbsVctmPeerEnbCb            *peerEnbCb,
U8                               choice,
U32                              cause
)
{
   S16                           ret;
   WrNeighEnbCb                  *nghEnbCb;
   CztEvnt                       *x2RsrcStsFlr;
   U32                       enbId = 0;

   TRC2(wrEmmAbsBuildAndSendRsrcStsFlr)

   ret = cmHashListFind(&(wrEmmCb.neighEnbLstCp), (U8 *)&(peerEnbCb->enbId.val),
                      (U16)sizeof(peerEnbCb->enbId.val), 0,(PTR *)&nghEnbCb);
   if (ROK != ret)
   {
      RLOG0(L_ERROR, " Failure in the Hash List find ");
      RETVALUE(RFAILED);
   }

   WR_GET_ENBID_FROM_PEERID(enbId,nghEnbCb->peerId);
   if(ROK != wrEmmAbsBuildRsrcStsFlr(peerEnbCb, choice, cause, &x2RsrcStsFlr))
   {
      RLOG0(L_ERROR, " Resource Status Failure building failed ");
      RETVALUE(RFAILED);
   }

   x2RsrcStsFlr->peerId = nghEnbCb->peerId;
   if (ROK != WrIfmX2apGpRsp(x2RsrcStsFlr))
   {
      RLOG0(L_ERROR, " Failed to send Resource Status Failure message ");
      RETVALUE(RFAILED);
   }
   else
   {
      RLOG2(L_EVENT,"[X2AP]:Sending RESOURCE STATUS FAILURE [Src(%ld)-->Peer(%ld)]",
            wrEmmCb.enbId,enbId);
   }

   RETVALUE(ROK);
}

/** @brief This function build and sends Resource Status Update to X2AP
 *
 * @details 
 *
 *     Function: wrEmmAbsBuildAndSendRsrcStsUpd
 *
 *         Processing steps:
 *
 *
 * @param[in]   peerEnbCb  : Pointer to peer eNodeB Cb
 * @return S16
 *     -# Success : ROK
 *     -# Failure : RFAILED
 */
PRIVATE S16 wrEmmAbsBuildAndSendRsrcStsUpd
(
WrEmmAbsVctmPeerEnbCb              *peerEnbCb
)
{
   S16                             ret;
   WrNeighEnbCb                    *nghEnbCb;
   CztEvnt                         *x2RsrcStsUpd;
   U32                       enbId = 0;

   TRC2(wrEmmAbsBuildAndSendRsrcStsUpd)

   ret = cmHashListFind(&(wrEmmCb.neighEnbLstCp), (U8 *)&(peerEnbCb->enbId.val),
                      (U16)sizeof(peerEnbCb->enbId.val), 0,(PTR *)&nghEnbCb);
   if (ROK != ret)
   {
      RLOG0(L_ERROR, " Failure in the Hash List find ");
      RETVALUE(RFAILED);
   }
   WR_GET_ENBID_FROM_PEERID(enbId,nghEnbCb->peerId);

   if(ROK != wrEmmAbsBuildRsrcStsUpd(peerEnbCb, &x2RsrcStsUpd))
   {
      RLOG0(L_ERROR, " Resource Status Update building failed ");
      RETVALUE(RFAILED);
   }

   x2RsrcStsUpd->peerId = nghEnbCb->peerId;
   if (ROK != WrIfmX2apGpReq(x2RsrcStsUpd))
   {
      RLOG0(L_ERROR, " Failed to send Resource Status Update message ");
      RETVALUE(RFAILED);
   }
   else
   {
      RLOG2(L_EVENT,"[X2AP]:Sending RESOURCE STATUS UPDATE [Src(%ld)-->Peer(%ld)]",
            wrEmmCb.enbId,enbId);
   }

   RETVALUE(ROK);
}


/**********************************************************************/
/* IMPLEMENTATION OF MESSAGE IE HANDLERS                              */
/**********************************************************************/


/** @brief This function handles Load Information message with 
 *         invoke indication at macro eNodeB
 *
 * @details 
 *
 *     Function: wrEmmAbsAggrHndlAbsInvokeInd
 *
 *         Processing steps:
 *              1. Validate received message
 *              2. Allocate peer eNodeB Cb and Cell Cb if already not allocated
 *
 * @param[in]  enbId         : Id of peer eNodeB 
 * @param[in]  cellId        : Id of peer cell
 * @param[in]  extie         : X2AP IE carrying cztInvokeInd
 * @param[out] isAbsInfoRqrd : Flag to indicate ABS information 
 *                             required to send it to peer 
 * @return S16
 *     -# Success : ROK
 *     -# Failure : RFAILED
 */
PRIVATE S16 wrEmmAbsAggrHndlAbsInvokeInd 
(
WrEmmAbsEnbId                           *enbId, 
WrEmmAbsCellId                          *cellId,
CztProtExtnField_CellInform_Item_ExtIEs *extie, 
Bool                                    *isAbsInfoRqrd
)
{
   WrEmmAbsAggrPeerEnbCb                *peerEnbCb;
   WrEmmAbsAggrPeerCellCb               *peerCellCb;

   TRC2(wrEmmAbsAggrHndlAbsInvokeInd)

   if(extie->extensionValue.u.cztInvokeInd.val != CztInvokeIndabs_informationEnum)
   {
      RLOG0(L_ERROR, " Invalid invoke indication "); 
      RETVALUE(RFAILED);
   }

   if(ROK != wrEmmAbsAggrGetPeerEnbCb (enbId, &peerEnbCb))
   {
      if(ROK != wrEmmAbsAggrAllocNAddPeerEnbCb(enbId, &peerEnbCb))
      {
         RLOG0(L_ERROR, " Creation of new peer eNodeB cb failed ");
         RETVALUE(RFAILED);
      }
   }
 
   if(ROK == wrEmmAbsAggrGetPeerCellCb(peerEnbCb, cellId, &peerCellCb))
   {
      wrEmmAbsAggrRemoveWeightage(peerCellCb);
   }
   else
   {
      if(ROK != wrEmmAbsAggrAllocNAddPeerCellCb(peerEnbCb, cellId, &peerCellCb))
      {
         RLOG0(L_ERROR, " Creation of new peer cell cb failed ");

         /* If cell list is empty then release enb cb*/
         if(peerEnbCb->absPeerCellLstCp.count)
         {
            wrEmmAbsAggrRemoveNDeallocPeerEnbCb(peerEnbCb);
         }
         RETVALUE(RFAILED);
      }
   }

   *isAbsInfoRqrd = TRUE;

   RETVALUE(ROK);
}

/** @brief This function handle ABS Status Received 
 *         from peer in Macro eNodeB
 *
 * @details 
 *
 *     Function: wrEmmAbsAggrHndlAbsStatus 
 *
 *         Processing steps:
 *             1. Update percentage resource used, and usabel abs patter to cell cb
 *             2. Evaluates ABS Pattern weightage
 *
 *
 * @param[in]   peerEnbCb   : Pointer to peer eNodeB control block  
 * @param[in]   peerCellCb  : Pointer to peer cell control block 
 * @param[in]   absStatus   : Pointer to peer abs status X2AP IE 
 * @return S16
 *     -# Success : ROK
 *     -# Failure : RFAILED
 */
PRIVATE S16 wrEmmAbsAggrHndlAbsStatus 
(
WrEmmAbsAggrPeerEnbCb                 *peerEnbCb,
WrEmmAbsAggrPeerCellCb                *peerCellCb, 
CztABS_Status                         *absStatus
)
{
   static U32 disableAbsAfterNCount = 0;

   TRC3(wrEmmAbsAggrHndlAbsStatus)
   
   if(USABLEABSINFORM_FDD == absStatus->usableABSInform.choice.val)
   {
      /* Remove old usable pattern's weightage */
      wrEmmAbsAggrRemoveWeightage(peerCellCb);

      /* Update new usable pattern and weightage */
      peerCellCb->absPercentageRsrcUsed = (U8)absStatus->dL_ABS_status.val; /* KW fix for LTE_ADV */   
      if (peerCellCb->absPercentageRsrcUsed == 0)
      {
         disableAbsAfterNCount ++;
      }
      else
      {
         disableAbsAfterNCount = 0;
      }
      wrEmmAbsAggrFndPatternId(absStatus->usableABSInform.val.fdd.
              usable_abs_pattern_info.val, &peerCellCb->usablePatternId);

      wrEmmAbsAggrAddWeightage(peerCellCb); 
      if (disableAbsAfterNCount == LTE_ADV_MAX_NUM_NIL_RSRC_UPD)
      {
         disableAbsAfterNCount = 0;
         /* 1. Disable ABS feature of MAC Scheduler */
         wrEmmAbsCfgScheduler(RGR_DISABLE);

         /* 2. Send Load Information with ABS disabled to all peer eNodeB */
         if(ROK != wrEmmAbsBuildAndSendRsrcStsReq(peerEnbCb,
                  CztRegistration_RqststopEnum))

         {
            RLOG0(L_ERROR, " Sending Resource Status Request failed ");
            RETVALUE(RFAILED);
         }

         if(ROK != wrEmmAbsBuildAndSendLoadInfo(&peerEnbCb->enbId, 
                  Cztid_ABSInform, ABSINFORM_ABS_INACTIVE))
         {
            RLOG0(L_ERROR, " Sending ABS Load Information failed ");
            RETVALUE(RFAILED);
         }
      }
   }

   RETVALUE(ROK);
}

/** @brief This function handles the ABS Measurement Result Received 
 *         from peer at Macro eNodeB
 *
 * @details 
 *
 *     Function: wrEmmAbsAggrHndlMeasResultExtIe
 *
 *         Processing steps:
 *             1. For each cell ABS measurement result it call wrEmmAbsAggrHndlAbsStatus
 *
 * @param[in]   peerEnbCb :  Pointer to peer eNodeB Cb
 * @param[in]   peerCellCb : Pointer to peer cell Cb
 * @param[in]   extIes  : Pointer to X2AP measurement result IE
 * @return S16
 *     -# Success : ROK
 *     -# Failure : RFAILED
 */
PRIVATE S16 wrEmmAbsAggrHndlMeasResultExtIe 
(
WrEmmAbsAggrPeerEnbCb                                 *peerEnbCb,
WrEmmAbsAggrPeerCellCb                                *peerCellCb,
CztProtExtnCont_CellMeasurementResult_Item_ExtIEs     *extIes
)
{
   U8                                                 idx;
   CztProtExtnField_CellMeasurementResult_Item_ExtIEs *extIeItem;

   TRC3(wrEmmAbsAggrHndlMeasResultExtIe)

   for(idx = 0; idx < extIes->noComp.val; idx++)
   {
      extIeItem = &extIes->member[idx];
      if(extIeItem->id.val == Cztid_ABS_Status)
      {
         wrEmmAbsAggrHndlAbsStatus(peerEnbCb, peerCellCb, 
                                &extIeItem->extensionValue.u.cztABS_Status);
      }
   }

   RETVALUE(ROK); 
}

/** @brief This function handles Resource Status Update IE IE 
 *         from peer at Macro eNodeB 
 *
 * @details 
 *
 *     Function: wrEmmAbsAggrHndlMeasResult
 *
 *         Processing steps:
 *            1. For each cell resource update it call wrEmmAbsAggrHndlMeasResultExtIe
 *
 * @param[in] peerEnbCb : Pointer peer eNodeB Cb  
 * @paraE[in] rsrcStsUpdIe : Pointer to X2AP Resource Status Update IE  
 * @return S16
 *     -# Success : ROK
 *     -# Failure : RFAILED
 */
PRIVATE S16 wrEmmAbsAggrHndlMeasResult 
(
WrEmmAbsAggrPeerEnbCb                                  *peerEnbCb, 
CztProtIE_Field_ResStatusUpd_IEs                       *rsrcStsUpdIe
)
{
   U8                                                  idx;
   WrEmmAbsCellId                                      cellId;
   CztProtIE_Single_Cont_CellMeasurementResult_ItemIEs *rsrcStsUpdItemIe;
   CztProtExtnCont_CellMeasurementResult_Item_ExtIEs   *extIes;
   WrEmmAbsAggrPeerCellCb                              *peerCellCb;

   TRC3(wrEmmAbsAggrHndlMeasResult)

   for(idx = 0; idx < rsrcStsUpdIe->value.u.cztCellMeasurementResult_Lst.noComp.val; idx++)
   {
      rsrcStsUpdItemIe = &rsrcStsUpdIe->value.u.cztCellMeasurementResult_Lst.member[idx];
      if(rsrcStsUpdItemIe->value.u.cztCellMeasurementResult_Item.iE_Extns.noComp.val)
      {
         /* Extract cell id and process extention ie */
         WR_EMM_ABS_GET_CELLID((&cellId), (&rsrcStsUpdItemIe->value.u.cztCellMeasurementResult_Item.cell_ID));
         if(ROK == wrEmmAbsAggrGetPeerCellCb(peerEnbCb, &cellId, &peerCellCb))
         {
            extIes = &rsrcStsUpdItemIe->value.u.cztCellMeasurementResult_Item.iE_Extns; 
            wrEmmAbsAggrHndlMeasResultExtIe(peerEnbCb, peerCellCb, extIes);
         }
      } 
   }

   RETVALUE(ROK);
}

/** @brief This function handles ABS information with ABS info at Macro eNodeB
 *
 * @details 
 *
 *     Function: wrEmmAbsAggrHndlAbsInfo
 *
 *         Processing steps:
 *             1. If this message is received for inactivation of ABS feature, 
 *                then removes peer control info
 *
 * @param[in]   enbId : Id of peer eNodeB
 * @param[in]  cellId : id of peer cell
 * @param[in]   extie : ABS information X2AP IE
 * @return S16
 *     -# Success : ROK
 *     -# Failure : RFAILED
 */
PRIVATE S16 wrEmmAbsAggrHndlAbsInfo 
(
WrEmmAbsEnbId                           *enbId,
WrEmmAbsCellId                          *cellId,
CztProtExtnField_CellInform_Item_ExtIEs *extie
)
{
   WrEmmAbsAggrPeerEnbCb                *peerEnbCb;  
   TRC3(wrEmmAbsAggrHndlAbsInfo)

   /* If request is for ABS Inactive, then release enb cb */
   if (ABSINFORM_ABS_INACTIVE == extie->extensionValue.u.cztABSInform.choice.val)
   {
      if(ROK != wrEmmAbsAggrGetPeerEnbCb (enbId, &peerEnbCb))
      {
         RLOG0(L_ERROR, " Failure in getting peer enodeb cb ");
         RETVALUE(RFAILED);
      }
      /* Stop Resource Status Update from peer */
      if(ROK != wrEmmAbsBuildAndSendRsrcStsReq(peerEnbCb,
                                  CztRegistration_RqststopEnum))

      {
         RLOG0(L_ERROR, " Sending Resource Status Request failed ");
         RETVALUE(RFAILED);
      }
      /* TODO: Waiting for response */
      wrEmmAbsAggrRemoveNDeallocPeerEnbCb(peerEnbCb);
      /* Stop ABS at MAC if there is no more Vctm */
      if (wrEmmCb.lteAdvCb.absInfo.peerInfo.u.aggrInfo.absPeerEnbLstCp.count == 0)
      {
         wrEmmAbsCfgScheduler(RGR_DISABLE);
      }
   } 
   else 
   {
      RLOG1(L_DEBUG, " Unhandled ABS Pattern Info(%d) received from peer",
            extie->extensionValue.u.cztABSInform.choice.val);
   }

   RETVALUE(ROK);
}

/** @brief This function handles ABS IE of Load Information message  
 *
 * @details 
 *
 *     Function: wrEmmAbsAggrHndlAbsLoadInfo
 *
 *         Processing steps:
 *             1. For each received IE it call appropriate handler
 *
 * @param[in] enbId          : Id of peer eNodeB  
 * @param[in] cellInfoIe     : Cell Information IE of X2AP  
 * @param[out] isAbsInfoRqrd : To indicate ABS information 
 *                             need to send into peer  
 * @return S16
 *     -# Success : ROK
 *     -# Failure : RFAILED
 */
PRIVATE S16 wrEmmAbsAggrHndlAbsLoadInfo 
( 
WrEmmAbsEnbId                               *enbId,
CztProtIE_Field_CellInform_ItemIEs          *cellInfoIe, 
Bool                                        *isAbsInfoRqrd
)
{
   U8                                       idx;
   CztProtExtnField_CellInform_Item_ExtIEs  *extie;
   WrEmmAbsCellId                           cellId;

   TRC2(wrEmmAbsAggrHndlAbsLoadInfo)

   /* Extract cell id into WrEmmAbsCellId */
   WR_EMM_ABS_GET_CELLID((&cellId), (&cellInfoIe->value.u.cztCellInform_Item.cell_ID));


   for(idx = 0; idx < cellInfoIe->value.u.cztCellInform_Item.iE_Extns.noComp.val; idx++)
   {
      extie = &cellInfoIe->value.u.cztCellInform_Item.iE_Extns.member[0];
      if(extie->id.val == Cztid_InvokeInd)
      {
         wrEmmAbsAggrHndlAbsInvokeInd(enbId, &cellId, extie, isAbsInfoRqrd);
      }
      else if(extie->id.val == Cztid_ABSInform)
      {
         wrEmmAbsAggrHndlAbsInfo(enbId, &cellId, extie);
      }
   }  
   
   RETVALUE(ROK); 
}



/** @brief This function handles cell to report IE of Resource Status Request at Pico eNodeB
 *
 * @details 
 *
 *     Function: wrEmmAbsHndlVctmRsrcStsReqCellToRprt 
 *
 *         Processing steps:
 *           1. Validate the IE
 *
 *
 * @param[in]  peerEnbCb : Pointer to peer eNodeB Cb
 * @param[out] rsrcStsRqstIe : Pointer to X2AP IE 
 * @return S16
 *     -# Success : ROK
 *     -# Failure : RFAILED
 */
PRIVATE S16 wrEmmAbsHndlVctmRsrcStsReqCellToRprt
(
WrEmmAbsVctmPeerEnbCb                           *peerEnbCb,
CztProtIE_Field_ResStatusRqst_IEs               *rsrcStsRqstIe
)
{
   U8                                           idx;
   WrEmmAbsCellId                               cellId;
   WrCellCb                                     *cellCb;
   CztProtIE_Single_Cont_CellToReport_ItemIEs   *cellToRprtIe;

   TRC3(wrEmmAbsHndlVctmRsrcStsReqCellToRprt)

   /* TODO: Currently only one cell is supported */
   if(1 != rsrcStsRqstIe->value.u.cztCellToReport_Lst.noComp.val)
   {
      RLOG1(L_ERROR, " One cell(%d) supported per eNodeB ",
           (U16)rsrcStsRqstIe->value.u.cztCellToReport_Lst.noComp.val);
      RETVALUE(RFAILED);
   }
   
   for(idx = 0; idx < rsrcStsRqstIe->value.u.cztCellToReport_Lst.noComp.val; idx++)
   {
      cellToRprtIe = &rsrcStsRqstIe->value.u.cztCellToReport_Lst.member[idx];
      WR_EMM_ABS_GET_CELLID((&cellId), (&cellToRprtIe->value.u.cztCellToReport_Item.cell_ID));
      WR_GET_ABS_CELLCB(cellCb);
      if((cellCb == NULLP))
      {
          RLOG0(L_ERROR, " CELLCB not found ");
          RETVALUE(RFAILED);   
      }  /*KW fix for LTE_ADV */    
      /*ABS_TESTING*/      
      RLOG3(L_DEBUG, " cellCb->physCellId(%d ) wrEmmCb.enbId(%lu) Cell(0x%x) not supported ",
            cellCb->physCellId, wrEmmCb.enbId, (U16)cellId.cId);
      if(wrEmmCb.enbId != cellId.cId)
      /*ABS_TESTING*/         
      {
         RLOG2(L_ERROR, " cellCb->enbId(%lu) Cell(0x%x) not supported ",
               wrEmmCb.enbId, (U16)cellId.cId);
         RETVALUE(RFAILED);
      }
   }
      
   RETVALUE(ROK);
}

/** @brief This function handles Fdd ABS Info recieved at Pico eNodeB
 *
 * @details 
 *
 *     Function: wrEmmAbsVctmHndlFddAbsInfo
 *
 *         Processing steps:
 *             1. Creates peer cell Cb
 *             2. Creates ABS Pattern Cb if already not present and updates ABS info
 *             3. Add Weightage to abs pattern
 *
 *
 * @param[in] peerEnbCb : Pointer to peer eNodeB Cb 
 * @param[in] cellId    : Peer cell id 
 * @param[in] extie     : X2AP IE of Abs Info
 * @return S16
 *     -# Success : ROK
 *     -# Failure : RFAILED
 */
PRIVATE S16 wrEmmAbsVctmHndlFddAbsInfo 
(
WrEmmAbsVctmPeerEnbCb                       *peerEnbCb,
WrEmmAbsCellId                              *cellId,
CztProtExtnField_CellInform_Item_ExtIEs     *extie
)
{
   U8                                       absPattern[WR_EMM_ABS_PATTERN_SIZE];
   WrEmmAbsVctmPatternCb                    *absPatternCb;
   WrEmmAbsVctmPeerCellCb                   *peerCellCb;

   TRC3(wrEmmAbsVctmHndlFddAbsInfo)

   if(ROK == wrEmmAbsVctmGetPeerCellCb (peerEnbCb, cellId, &peerCellCb))
   {
      if(NULLP != peerCellCb->absPatternCb)
      {
         wrEmmAbsVctmRemoveAbsWeightage(peerCellCb->absPatternCb, peerCellCb->cellWeightage);
         wrEmmAbsVctmRemoveCellCbFrmAbsPatternCb(peerCellCb->absPatternCb, peerCellCb);
      }
   }
   else
   {
      if(ROK != wrEmmAbsVctmAllocNAddPeerCellCb(peerEnbCb, cellId, &peerCellCb))
      {
         RLOG0(L_ERROR, " Peer Cell Cb not found ");
         RETVALUE(RFAILED);
      }
   }

   cmMemcpy(absPattern, extie->extensionValue.u.cztABSInform.val.fdd.abs_pattern_info.val, 
                                                                  WR_EMM_ABS_PATTERN_SIZE);

   if(ROK != wrEmmAbsVctmGetAbsPatternCb(absPattern, &absPatternCb)) 
   {
      if(ROK != wrEmmAbsVctmAllocNAddAbsPatternCb(absPattern, &absPatternCb))
      {
         RLOG0(L_ERROR, " ABS Pattern Cb alloc failed ");
         RETVALUE(RFAILED);
      }
   }

   /* Add weightage to ABS pattern */
   wrEmmAbsVctmAddAbsWeightage(absPatternCb, peerCellCb->cellWeightage);

   if(0 == extie->extensionValue.u.cztABSInform.val.fdd.
                                 numberOfCellSpecificAntennaPorts.val)
   {
      peerCellCb->noCSAntPorts = 1;
   }
   else if(1 == extie->extensionValue.u.cztABSInform.val.fdd.
                                 numberOfCellSpecificAntennaPorts.val)
   {
      peerCellCb->noCSAntPorts = 2;
   }
   else
   {
      peerCellCb->noCSAntPorts = 4;
   }
   
   cmMemcpy(peerCellCb->measSubset, extie->extensionValue.u.cztABSInform.val.fdd.
                       measurement_subset.val, WR_EMM_ABS_PATTERN_SIZE);
   
   wrEmmAbsVctmAddCellCbToAbsPatternCb(absPatternCb, peerCellCb);
   wrStopTmr((PTR)peerEnbCb, WR_TMR_ABS_LOAD_INFO_TMR);

   RETVALUE(ROK);
}

/** @brief This function handle ABS info IE received at Pico eNodeB
 *
 * @details 
 *
 *     Function: wrEmmAbsVctmHndlAbsInfo
 *           1. If ABS Information received for fdd then call wrEmmAbsVctmHndlFddAbsInfo
 *           2. If ABS Inactive indication is received from peer then remove eNodeB Cb 
 *
 *         Processing steps:
 *
 * @param[in] peerEnbCb : Pointer to peer eNodeB Cb  
 * @param[in] cellId    : Id of peer cell  
 * @param[in] extie     : Pointer to X2AP IE   
 * @return S16
 *     -# Success : ROK
 *     -# Failure : RFAILED
 */
PRIVATE S16 wrEmmAbsVctmHndlAbsInfo 
(
WrEmmAbsVctmPeerEnbCb                       *peerEnbCb,
WrEmmAbsCellId                              *cellId,
CztProtExtnField_CellInform_Item_ExtIEs     *extie
)
{
   TRC3(wrEmmAbsVctmHndlAbsInfo)

   if(ABSINFORM_FDD == extie->extensionValue.u.cztABSInform.choice.val)
   {
      wrEmmAbsVctmHndlFddAbsInfo(peerEnbCb, cellId, extie); 
   }
   else if(ABSINFORM_ABS_INACTIVE == extie->extensionValue.u.cztABSInform.choice.val)
   {
      /* Send ABS disable to Scheduler */
      wrEmmAbsCfgScheduler(RGR_DISABLE);

      /* Configure scheduler with UE info and UE with ABS info */
      wrEmmAbsConfigAllUmmUe();
      
      wrEmmAbsVctmRemoveNDeallocPeerEnbCb(peerEnbCb);
   }
   else
   {
      RLOG1(L_DEBUG, " Unhandled ABS Pattern Info(%d) Received from peer ",
            extie->extensionValue.u.cztABSInform.choice.val);
   }

   RETVALUE(ROK);
}

/** @brief This function handles cell information with ABS IEs at Pico eNodeB
 *
 * @details 
 *
 *     Function: wrEmmAbsVctmHndlAbsLoadInfo 
 *
 *         Processing steps:
 *             1. For each of abs information IE it calls wrEmmAbsVctmHndlAbsInfo 
 *
 *
 * @param[in]  peerEnbCb : Pointer to peer eNodeB Cb 
 * @param[in]  cellInfoIe: X2AP cell information IE  
 * @return S16
 *     -# Success : ROK
 *     -# Failure : RFAILED
 */
PRIVATE S16 wrEmmAbsVctmHndlAbsLoadInfo 
(
WrEmmAbsVctmPeerEnbCb                       *peerEnbCb,
CztProtIE_Field_CellInform_ItemIEs          *cellInfoIe
)
{
   U8                                       idx;
   WrEmmAbsCellId                           cellId;
   CztProtExtnField_CellInform_Item_ExtIEs  *extie;

   TRC3(wrEmmAbsVctmHndlAbsLoadInfo)

   WR_EMM_ABS_GET_CELLID((&cellId), (&cellInfoIe->value.u.cztCellInform_Item.cell_ID));
   
   for(idx = 0; idx < cellInfoIe->value.u.cztCellInform_Item.iE_Extns.noComp.val; idx++)
   {
      extie = &cellInfoIe->value.u.cztCellInform_Item.iE_Extns.member[idx];
      if(extie->id.val == Cztid_ABSInform)
      {
         wrEmmAbsVctmHndlAbsInfo(peerEnbCb, &cellId, extie);
      }
   }

   RETVALUE(ROK);
}


/**********************************************************************/
/*                 IMPLEMENTATION OF COMMON EVENT HANDLERS            */
/**********************************************************************/

/** @brief This function handles any invalid event recieved from other module
 *
 * @details
 *
 *     Function: wrEmmAbsHndlInvEvent
 *
 *         Processing steps:
 *
 *
 * @param[in]   evnt    : Pointer to WrEmmAbsEvent 
 * @return S16
 *     -# Success : ROK
 *     -# Failure : RFAILED
 */
PRIVATE S16 wrEmmAbsHndlInvEvent 
(
WrEmmAbsEvent                *evnt
)
{
   TRC2(wrEmmAbsHndlInvEvent)
   RLOG2(L_DEBUG, " Invalid Event(0x%x) In State(0x%x) ",
        (U16)evnt->eventId, (U16)wrEmmCb.lteAdvCb.absInfo.state);
   RETVALUE(ROK);
}


/**********************************************************************/
/*                 IMPLEMENTATION OF MACRO EVENT HANDLERS             */
/**********************************************************************/

/** @brief This function handles the Cell Initialization Indication 
 *         for aggressor eNodeB(Macro) from eNodeB application
 *
 * @details
 *
 *     Function: wrEmmAbsHndlAggrCellInit
 *
 *         Processing steps:
 *
 * @param[in]   evnt    : Pointer to WrEmmAbsEvent 
 * @return S16
 *     -# Success : ROK
 *     -# Failure : RFAILED
 */
PRIVATE S16 wrEmmAbsHndlAggrCellInit
(
WrEmmAbsEvent                    *evnt
)
{
   WrCellCb                      *cellCb;
   WrEmmLteAdvAbsAggrCellInfo    *aggrInfo;
   WrEmmAbsAggrPatternCfgTable   *absPatternCfgTable;

   TRC2(wrEmmAbsHndlAggrCellInit)

   WR_GET_CELLCB(cellCb, evnt->u.cellEvent.cellId);
   if(cellCb == NULLP)            
   {    
       RLOG1(L_ERROR, "CELLCB not found for given cellId(%d) ", 
            (U16)evnt->u.cellEvent.cellId);
       RETVALUE(RFAILED);        /* KW fix for LTE_ADV */   
   }                            
   /* Currently one cell per eNodeB supported */
   wrEmmCb.lteAdvCb.absInfo.state  = (cellCb->lteAdvCb.lteAdvCfg.absCfg.status == RGR_ENABLE)?
                         WR_EMM_ABS_ENABLED_STATE : WR_EMM_ABS_DISABLED_STATE;

   cmMemset((U8*)&cellCb->lteAdvCb.absInfo, 0, sizeof(WrEmmLteAdvAbsAggrCellInfo));
   aggrInfo           = &cellCb->lteAdvCb.absInfo.u.aggrInfo;
   absPatternCfgTable = &aggrInfo->absPatternCfgTable;
    
   /* ABS Patterns are added in ascending order of absPercentage */
   absPatternCfgTable->absPatternCfg[absPatternCfgTable->absPatternCount].absPattern[0] = 0x0;
   absPatternCfgTable->absPatternCfg[absPatternCfgTable->absPatternCount].absPattern[1] = 0x0;
   absPatternCfgTable->absPatternCfg[absPatternCfgTable->absPatternCount].absPattern[2] = 0x0;
   absPatternCfgTable->absPatternCfg[absPatternCfgTable->absPatternCount].absPattern[3] = 0x0;
   absPatternCfgTable->absPatternCfg[absPatternCfgTable->absPatternCount].absPattern[4] = 0x0;
   absPatternCfgTable->absPatternCfg[absPatternCfgTable->absPatternCount].absPercentage = 0x0;
   absPatternCfgTable->absPatternCfg[absPatternCfgTable->absPatternCount].absWeightage  = 0x0;
   absPatternCfgTable->absPatternCount++;


   absPatternCfgTable->absPatternCfg[absPatternCfgTable->absPatternCount].absPattern[0] = 0x11;
   absPatternCfgTable->absPatternCfg[absPatternCfgTable->absPatternCount].absPattern[1] = 0x11;
   absPatternCfgTable->absPatternCfg[absPatternCfgTable->absPatternCount].absPattern[2] = 0x11;
   absPatternCfgTable->absPatternCfg[absPatternCfgTable->absPatternCount].absPattern[3] = 0x11;
   absPatternCfgTable->absPatternCfg[absPatternCfgTable->absPatternCount].absPattern[4] = 0x11;
   absPatternCfgTable->absPatternCfg[absPatternCfgTable->absPatternCount].absPercentage = 25;
   absPatternCfgTable->absPatternCfg[absPatternCfgTable->absPatternCount].absWeightage  = 0;
   absPatternCfgTable->absPatternCount++;

   absPatternCfgTable->absPatternCfg[absPatternCfgTable->absPatternCount].absPattern[0] = 0x22;
   absPatternCfgTable->absPatternCfg[absPatternCfgTable->absPatternCount].absPattern[1] = 0x22;
   absPatternCfgTable->absPatternCfg[absPatternCfgTable->absPatternCount].absPattern[2] = 0x22;
   absPatternCfgTable->absPatternCfg[absPatternCfgTable->absPatternCount].absPattern[3] = 0x22;
   absPatternCfgTable->absPatternCfg[absPatternCfgTable->absPatternCount].absPattern[4] = 0x22;
   absPatternCfgTable->absPatternCfg[absPatternCfgTable->absPatternCount].absPercentage = 25;
   absPatternCfgTable->absPatternCfg[absPatternCfgTable->absPatternCount].absWeightage  = 0;
   absPatternCfgTable->absPatternCount++;

   absPatternCfgTable->absPatternCfg[absPatternCfgTable->absPatternCount].absPattern[0] = 0x44;
   absPatternCfgTable->absPatternCfg[absPatternCfgTable->absPatternCount].absPattern[1] = 0x44;
   absPatternCfgTable->absPatternCfg[absPatternCfgTable->absPatternCount].absPattern[2] = 0x44;
   absPatternCfgTable->absPatternCfg[absPatternCfgTable->absPatternCount].absPattern[3] = 0x44;
   absPatternCfgTable->absPatternCfg[absPatternCfgTable->absPatternCount].absPattern[4] = 0x44;
   absPatternCfgTable->absPatternCfg[absPatternCfgTable->absPatternCount].absPercentage = 25;
   absPatternCfgTable->absPatternCfg[absPatternCfgTable->absPatternCount].absWeightage  = 0;
   absPatternCfgTable->absPatternCount++;

   absPatternCfgTable->absPatternCfg[absPatternCfgTable->absPatternCount].absPattern[0] = 0x88;
   absPatternCfgTable->absPatternCfg[absPatternCfgTable->absPatternCount].absPattern[1] = 0x88;
   absPatternCfgTable->absPatternCfg[absPatternCfgTable->absPatternCount].absPattern[2] = 0x88;
   absPatternCfgTable->absPatternCfg[absPatternCfgTable->absPatternCount].absPattern[3] = 0x88;
   absPatternCfgTable->absPatternCfg[absPatternCfgTable->absPatternCount].absPattern[4] = 0x88;
   absPatternCfgTable->absPatternCfg[absPatternCfgTable->absPatternCount].absPercentage = 25;
   absPatternCfgTable->absPatternCfg[absPatternCfgTable->absPatternCount].absWeightage  = 0;
   absPatternCfgTable->absPatternCount++;

   absPatternCfgTable->absPatternCfg[absPatternCfgTable->absPatternCount].absPattern[0] = 0x55;
   absPatternCfgTable->absPatternCfg[absPatternCfgTable->absPatternCount].absPattern[1] = 0x55;
   absPatternCfgTable->absPatternCfg[absPatternCfgTable->absPatternCount].absPattern[2] = 0x55;
   absPatternCfgTable->absPatternCfg[absPatternCfgTable->absPatternCount].absPattern[3] = 0x55;
   absPatternCfgTable->absPatternCfg[absPatternCfgTable->absPatternCount].absPattern[4] = 0x55;
   absPatternCfgTable->absPatternCfg[absPatternCfgTable->absPatternCount].absPercentage = 50;
   absPatternCfgTable->absPatternCfg[absPatternCfgTable->absPatternCount].absWeightage  = 0;
   absPatternCfgTable->absPatternCount++;

   absPatternCfgTable->absPatternCfg[absPatternCfgTable->absPatternCount].absPattern[0] = 0xAA;
   absPatternCfgTable->absPatternCfg[absPatternCfgTable->absPatternCount].absPattern[1] = 0xAA;
   absPatternCfgTable->absPatternCfg[absPatternCfgTable->absPatternCount].absPattern[2] = 0xAA;
   absPatternCfgTable->absPatternCfg[absPatternCfgTable->absPatternCount].absPattern[3] = 0xAA;
   absPatternCfgTable->absPatternCfg[absPatternCfgTable->absPatternCount].absPattern[4] = 0xAA;
   absPatternCfgTable->absPatternCfg[absPatternCfgTable->absPatternCount].absPercentage = 50;
   absPatternCfgTable->absPatternCfg[absPatternCfgTable->absPatternCount].absWeightage  = 0;
   absPatternCfgTable->absPatternCount++;

   absPatternCfgTable->absPatternCfg[absPatternCfgTable->absPatternCount].absPattern[0] = 0xEE;
   absPatternCfgTable->absPatternCfg[absPatternCfgTable->absPatternCount].absPattern[1] = 0xEE;
   absPatternCfgTable->absPatternCfg[absPatternCfgTable->absPatternCount].absPattern[2] = 0xEE;
   absPatternCfgTable->absPatternCfg[absPatternCfgTable->absPatternCount].absPattern[3] = 0xEE;
   absPatternCfgTable->absPatternCfg[absPatternCfgTable->absPatternCount].absPattern[4] = 0xEE;
   absPatternCfgTable->absPatternCfg[absPatternCfgTable->absPatternCount].absPercentage = 75;
   absPatternCfgTable->absPatternCfg[absPatternCfgTable->absPatternCount].absWeightage  = 0;
   absPatternCfgTable->absPatternCount++;

   absPatternCfgTable->absPatternCfg[absPatternCfgTable->absPatternCount].absPattern[0] = 0xDD;
   absPatternCfgTable->absPatternCfg[absPatternCfgTable->absPatternCount].absPattern[1] = 0xDD;
   absPatternCfgTable->absPatternCfg[absPatternCfgTable->absPatternCount].absPattern[2] = 0xDD;
   absPatternCfgTable->absPatternCfg[absPatternCfgTable->absPatternCount].absPattern[3] = 0xDD;
   absPatternCfgTable->absPatternCfg[absPatternCfgTable->absPatternCount].absPattern[4] = 0xDD;
   absPatternCfgTable->absPatternCfg[absPatternCfgTable->absPatternCount].absPercentage = 75;
   absPatternCfgTable->absPatternCfg[absPatternCfgTable->absPatternCount].absWeightage  = 0;
   absPatternCfgTable->absPatternCount++;

   absPatternCfgTable->absPatternCfg[absPatternCfgTable->absPatternCount].absPattern[0] = 0xBB;
   absPatternCfgTable->absPatternCfg[absPatternCfgTable->absPatternCount].absPattern[1] = 0xBB;
   absPatternCfgTable->absPatternCfg[absPatternCfgTable->absPatternCount].absPattern[2] = 0xBB;
   absPatternCfgTable->absPatternCfg[absPatternCfgTable->absPatternCount].absPattern[3] = 0xBB;
   absPatternCfgTable->absPatternCfg[absPatternCfgTable->absPatternCount].absPattern[4] = 0xBB;
   absPatternCfgTable->absPatternCfg[absPatternCfgTable->absPatternCount].absPercentage = 75;
   absPatternCfgTable->absPatternCfg[absPatternCfgTable->absPatternCount].absWeightage  = 0;
   absPatternCfgTable->absPatternCount++;

   absPatternCfgTable->absPatternCfg[absPatternCfgTable->absPatternCount].absPattern[0] = 0x77;
   absPatternCfgTable->absPatternCfg[absPatternCfgTable->absPatternCount].absPattern[1] = 0x77;
   absPatternCfgTable->absPatternCfg[absPatternCfgTable->absPatternCount].absPattern[2] = 0x77;
   absPatternCfgTable->absPatternCfg[absPatternCfgTable->absPatternCount].absPattern[3] = 0x77;
   absPatternCfgTable->absPatternCfg[absPatternCfgTable->absPatternCount].absPattern[4] = 0x77;
   absPatternCfgTable->absPatternCfg[absPatternCfgTable->absPatternCount].absPercentage = 75;
   absPatternCfgTable->absPatternCfg[absPatternCfgTable->absPatternCount].absWeightage  = 0;
   absPatternCfgTable->absPatternCount++;

   if(0 == absPatternCfgTable->absPatternCfg[0].absPercentage)
   {
      aggrInfo->absMinRsrc = absPatternCfgTable->absPatternCfg[1].absPercentage;
   }
   else
   {
      aggrInfo->absMinRsrc = absPatternCfgTable->absPatternCfg[0].absPercentage;
   }
   aggrInfo->absMaxRsrc = absPatternCfgTable->absPatternCfg[
                          absPatternCfgTable->absPatternCount-1].absPercentage;

   aggrInfo->absTotalRsrcAvail = aggrInfo->absMinRsrc;
  
   WR_EMM_ABS_CONVERT_NEGSEC_COUNT((cellCb->lteAdvCb.lteAdvCfg.absCfg.absLoadPeriodicity),
                                                      (aggrInfo->absRprtPeriodCount));

   RETVALUE(ROK);
}

/** @brief This function handles the Load Information from X2AP 
 *         during ABS feature enabled in aggressor eNodeB
 *
 * @details
 *
 *     Function: wrEmmAbsHndlAggrLoadInfo
 *
 *         Processing steps:
 *
 *
 * @param[in]   evnt    : Pointer to WrEmmAbsEvent 
 * @return S16
 *     -# Success : ROK
 *     -# Failure : RFAILED
 */
PRIVATE S16 wrEmmAbsHndlAggrLoadInfo 
(
WrEmmAbsEvent                              *evnt
)
{
   U8                                       idx;
   WrEmmAbsEnbId                            enbId;
   Bool                                     isAbsInfoRqrd = FALSE;
   CztLoadInform                            *loadInfo;
   CztProtIE_Field_LoadInform_IEs           *loadInfoIe;
   CztProtIE_Field_CellInform_ItemIEs       *cellInfoIe;
   WrEmmAbsAggrPeerEnbCb                    *peerEnbCb;
   CmLList                                  *node;
   WrEnumAbsPatternSelectFlrCause           cause;
   
   TRC2(wrEmmAbsHndlAggrLoadInfo)

   loadInfo   = &evnt->u.x2apEvent.pdu->pdu.val.initiatingMsg.
                                           value.u.cztLoadInform;

   for(idx = 0; idx < loadInfo->protocolIEs.noComp.val; idx++)
   {
      loadInfoIe = &loadInfo->protocolIEs.member[idx];
      if(Cztid_CellInform == loadInfoIe->id.val)
      {
         break;
      }
   }

   if(idx == loadInfo->protocolIEs.noComp.val)
   {
      RLOG0(L_ERROR, " Cell Information not received ");
      RETVALUE(RFAILED); 
   }

   /* Construct eNodeB id */
   WR_GET_ENBID_FROM_PEERID(enbId.val, evnt->u.x2apEvent.peerId);
   RLOG2(L_EVENT,"[X2AP]:Received LOAD INFORMATION [Peer(%ld)<--Src(%ld)]",
         wrEmmCb.enbId,enbId.val);
   
   /* Search for ABS information within cellinfo  and call corresponding handler */
   for(idx = 0; idx < loadInfoIe->value.u.cztCellInform_Lst.noComp.val; idx++)
   {
      cellInfoIe = &loadInfoIe->value.u.cztCellInform_Lst.member[idx];
      if(cellInfoIe->value.u.cztCellInform_Item.iE_Extns.noComp.val != 0)
      {
         wrEmmAbsAggrHndlAbsLoadInfo(&enbId, cellInfoIe, &isAbsInfoRqrd);
      }
   }

   if(FALSE == isAbsInfoRqrd)
   {
      RETVALUE(ROK);
   }

   /* Current ABS pattern is not valid(0% ABS) then, 
            1. Select new ABS pattern
            2. Configure scheduler
            3. Send ABS Pattern to all peer requested for ABS pattern
            4. Send Resource Status Request to current peer eNodeB 
      Else
            1. Send ABS Pattern to all peer requested for ABS pattern
            2. Send Resource Status Request to current peer eNodeB 
       
    */

    if(TRUE == wrEmmAbsAggrIsPatternSlctnRqrd())
    {
       /* 1. Select new ABS pattern(Expecting no failure) */
       wrEmmAbsAggrSelectAbsPattern(&cause);
       
       /* 2. Configure scheduler */
       wrEmmAbsCfgScheduler(RGR_ENABLE);

       /* 3. Send ABS Pattern to all peer requested for ABS pattern */
       node = wrEmmCb.lteAdvCb.absInfo.peerInfo.u.aggrInfo.absPeerEnbLstCp.first;
       while(NULLP != node)
       {
          peerEnbCb = (WrEmmAbsAggrPeerEnbCb*)node->node;

          if(ROK != wrEmmAbsBuildAndSendLoadInfo(&peerEnbCb->enbId, 
                                           Cztid_ABSInform, ABSINFORM_FDD))
          {
             RLOG0(L_ERROR, " Sending ABS Load Information failed ");
             RETVALUE(RFAILED);
          }

          /* 4. Send Resource Status Request to current peer eNodeB */
          if(peerEnbCb->enbId.val == enbId.val)
          {
             if(ROK != wrEmmAbsBuildAndSendRsrcStsReq(peerEnbCb,
                                              CztRegistration_RqststartEnum))
             {
                RLOG0(L_ERROR, "wrEmmAbsHndlAggrLoadInfo: Sending Resource Status Request failed ");
                RETVALUE(RFAILED);
             }
          }

          /* Go to next node */
          node = node->next;
       }
    }
    else
    {
       if(ROK != wrEmmAbsAggrGetPeerEnbCb(&enbId, &peerEnbCb))
       {
          RLOG0(L_ERROR, " Failure in getting peer enodeb cb ");
          RETVALUE(RFAILED);
       }

       if(ROK != wrEmmAbsBuildAndSendLoadInfo(&enbId, Cztid_ABSInform, ABSINFORM_FDD))
       {
          RLOG0(L_ERROR, " Sending ABS Load Information failed ");
          RETVALUE(RFAILED);
       }
       
       /* 2. Send Resource Status Request to current peer eNodeB */
       if(ROK != wrEmmAbsBuildAndSendRsrcStsReq(peerEnbCb, 
                               CztRegistration_RqststartEnum))
       {
          RLOG0(L_ERROR, " Sending Resource Status Request failed ");
          RETVALUE(RFAILED);
       }
    }

    RETVALUE(ROK);
}

/** @brief This function handles the Resource Status Response from X2AP 
 *
 * @details
 *
 *     Function: wrEmmAbsHndlAggrRsrcStsRsp
 *
 *         Processing steps:
 *
 *
 * @param[in]   evnt    : Pointer to WrEmmAbsEvent 
 * @return S16
 *     -# Success : ROK
 *     -# Failure : RFAILED
 */
PRIVATE S16 wrEmmAbsHndlAggrRsrcStsRsp 
(
WrEmmAbsEvent                         *evnt
)
{
   U8                                 idx           = 0;
   WrEmmAbsEnbId                      enbId;
   WrEmmAbsAggrPeerEnbCb              *peerEnbCb;
   CztResStatusResp                   *rsrcStsRsp;
   CztProtIE_Field_ResStatusResp_IEs  *rsrcStsRspIe;

   TRC2(wrEmmAbsHndlAggrRsrcStsRsp)

   /* Construct eNodeB id */
   WR_GET_ENBID_FROM_PEERID(enbId.val, evnt->u.x2apEvent.peerId);
   RLOG2(L_EVENT,"[X2AP]:Received RESOURCE STATUS RESPONSE [Peer(%ld)<--Src(%ld)]",
         wrEmmCb.enbId,enbId.val);

   if(ROK != wrEmmAbsAggrGetPeerEnbCb(&enbId, &peerEnbCb))
   {
      RLOG0(L_ERROR, " Failure in getting peer enodeb cb ");
      RETVALUE(RFAILED);
   }

   rsrcStsRsp   = &evnt->u.x2apEvent.pdu->pdu.val.successfulOutcome.
                                    value.u.cztResStatusResp;

   for(idx = 0; idx< rsrcStsRsp->protocolIEs.noComp.val; idx++)
   {
      rsrcStsRspIe = &rsrcStsRsp->protocolIEs.member[idx];
      if(Cztid_ENB2_Measurement_ID == rsrcStsRspIe->id.val)
      {
         peerEnbCb->enb2MeasId = rsrcStsRspIe->value.u.cztid_ENB2_Measurement_ID.val;
         RETVALUE(ROK);
      }
   }

   RETVALUE(RFAILED);
}

/** @brief This function handles the Resource Status Failure from X2AP 
 *
 * @details
 *
 *     Function: wrEmmAbsHndlAggrRsrcStsFlr
 *
 *         Processing steps:
 *
 *
 * @param[in]   evnt    : Pointer to WrEmmAbsEvent 
 * @return S16
 *     -# Success : ROK
 *     -# Failure : RFAILED
 */
PRIVATE S16 wrEmmAbsHndlAggrRsrcStsFlr 
(
WrEmmAbsEvent                 *evnt
)
{
   WrEmmAbsEnbId              enbId;
   WrEmmAbsAggrPeerEnbCb      *peerEnbCb;

   TRC2(wrEmmAbsHndlAggrRsrcStsFlr)

   /* Construct eNodeB id */
   WR_GET_ENBID_FROM_PEERID(enbId.val, evnt->u.x2apEvent.peerId);
   RLOG2(L_EVENT,"[X2AP]:Received RESOURCE STATUS FAILURE [Peer(%ld)<--Src(%ld)]",
         wrEmmCb.enbId,enbId.val);

   if(ROK != wrEmmAbsAggrGetPeerEnbCb(&enbId, &peerEnbCb))
   {
      RLOG0(L_ERROR, " Failure in getting peer enodeb cb ");
      RETVALUE(RFAILED);
   }

   wrEmmAbsAggrRemoveNDeallocPeerEnbCb(peerEnbCb);

   RETVALUE(ROK);
}

/** @brief This function handles the Resource Status Update from X2AP 
 *
 * @details
 *
 *     Function: wrEmmAbsHndlAggrRsrcStsUpd
 *
 *         Processing steps:
 *
 *
 * @param[in]   evnt    : Pointer to WrEmmAbsEvent 
 * @return S16
 *     -# Success : ROK
 *     -# Failure : RFAILED
 */
PRIVATE S16 wrEmmAbsHndlAggrRsrcStsUpd 
(
WrEmmAbsEvent                       *evnt
)
{
   U8                               idx;
   WrEmmAbsEnbId                    enbId;
   WrEmmAbsAggrPeerEnbCb            *peerEnbCb;
   CztResStatusUpd                  *rsrcStsUpd;
   CztProtIE_Field_ResStatusUpd_IEs *rsrcStsUpdIe;

   TRC2(wrEmmAbsHndlAggrRsrcStsUpd)

   /* Construct eNodeB id */
   WR_GET_ENBID_FROM_PEERID(enbId.val, evnt->u.x2apEvent.peerId);
   RLOG2(L_EVENT,"[X2AP]:Received RESOURCE STATUS UPDATE [Peer(%ld)<--Src(%ld)]",
         wrEmmCb.enbId,enbId.val);

   if(ROK != wrEmmAbsAggrGetPeerEnbCb(&enbId, &peerEnbCb))
   {
      RLOG0(L_ERROR, " Failure in getting peer enodeb cb ");
      RETVALUE(RFAILED);
   }

   rsrcStsUpd   = &evnt->u.x2apEvent.pdu->pdu.val.initiatingMsg.
                     value.u.cztResStatusUpd;
   
   for(idx = 0; idx < rsrcStsUpd->protocolIEs.noComp.val; idx++)
   { 
      rsrcStsUpdIe = &rsrcStsUpd->protocolIEs.member[idx];
      if(Cztid_CellMeasurementResult == rsrcStsUpdIe->id.val)
      {
         wrEmmAbsAggrHndlMeasResult(peerEnbCb, rsrcStsUpdIe);
      }
   }

   RETVALUE(ROK);
}

/** @brief This function handles the peer down indication from X2AP 
 *
 * @details
 * 
 *     Function: wrEmmAbsHndlAggrX2DownInd
 *
 *         Processing steps:
 *
 *
 * @param[in]   evnt    : Pointer to WrEmmAbsEvent 
 * @return S16
 *     -# Success : ROK
 *     -# Failure : RFAILED
 */
PRIVATE S16 wrEmmAbsHndlAggrX2DownInd 
(
WrEmmAbsEvent                    *evnt
)
{
   WrEmmAbsEnbId                 enbId;
   WrEmmAbsAggrPeerEnbCb         *peerEnbCb;

   TRC2(wrEmmAbsHndlAggrX2DownInd)

   /* Construct eNodeB id */
   WR_GET_ENBID_FROM_PEERID(enbId.val, evnt->u.x2apEvent.peerId);

   if(ROK != wrEmmAbsAggrGetPeerEnbCb(&enbId, &peerEnbCb))
   {
      RLOG0(L_ERROR, " Failure in getting peer enodeb cb ");
      RETVALUE(RFAILED);
   }
   
   wrEmmAbsAggrRemoveNDeallocPeerEnbCb(peerEnbCb);

   /* Stop ABS at MAC if there is no more Vctm */
   if (wrEmmCb.lteAdvCb.absInfo.peerInfo.u.aggrInfo.absPeerEnbLstCp.count == 0)
   {
      wrEmmAbsCfgScheduler(RGR_DISABLE);
   }

   RETVALUE(ROK);
}

/** @brief This function handles the cell up indication from eNodeB app 
 *
 * @details
 * 
 *     Function: wrEmmAbsHndlAggrCellUpInd
 *
 *         Processing steps:
 *
 *
 * @param[in]   evnt    : Pointer to WrEmmAbsEvent 
 * @return S16
 *     -# Success : ROK
 *     -# Failure : RFAILED
 */
PRIVATE S16 wrEmmAbsHndlAggrCellUpInd
(
WrEmmAbsEvent                   *evnt
)
{
   TRC2(wrEmmAbsHndlAggrCellUpInd)
   RETVALUE(ROK);
}

/** @brief This function handles the cell down indication from eNodeB app 
 *
 * @details
 * 
 *     Function: wrEmmAbsHndlAggrCellDownInd
 *
 *         Processing steps:
 *
 *
 * @param[in]   evnt    : Pointer to WrEmmAbsEvent 
 * @return S16
 *     -# Success : ROK
 *     -# Failure : RFAILED
 */
PRIVATE S16 wrEmmAbsHndlAggrCellDownInd 
(
WrEmmAbsEvent                       *evnt
)
{
   TRC2(wrEmmAbsHndlAggrCellDownInd)
   RETVALUE(ROK);
}

/** @brief This function handles the load indication indication from scheduler
 *
 * @details
 * 
 *     Function: wrEmmAbsHndlAggrSchLoadInd
 *
 *         Processing steps:
 *
 *
 * @param[in]   evnt    : Pointer to WrEmmAbsEvent 
 * @return S16
 *     -# Success : ROK
 *     -# Failure : RFAILED
 */
PRIVATE S16 wrEmmAbsHndlAggrSchLoadInd 
(
WrEmmAbsEvent                    *evnt
)
{
   U8                              idx;
   WrCellCb                        *cellCb;
   U32                             nonAbsRsrcUsed = 0;
   U8                              percentageRsrcUsed = 0;
   WrEmmAbsAggrPatternCfgTable     *absPatternCfgTable;
   RgrLoadInfIndInfo               *schLoadInfo;
   WrEmmAbsAggrPeerEnbCb           *peerEnbCb;
   CmLList                         *node;
   WrEnumAbsPatternSelectFlrCause  cause; 
   /*ABS_TESTING start*/
   static U32                      cumulativePercentageRsrcUsed = 0;
   U32                             averagePercentageRsrcUsed = 0;
   /*ABS_TESTING end*/

   TRC2(wrEmmAbsHndlAggrSchLoadInd)

   schLoadInfo = evnt->u.schLoadInfoEvent.schLoadInfo;
   if(RGR_ABS != schLoadInfo->type)
   {
      RETVALUE(RFAILED);
   }

   WR_GET_CELLCB(cellCb, schLoadInfo->cellId);
   if(cellCb == NULLP)           
   {    
       RLOG1(L_ERROR, " CELLCB not found for given cellId(%d) ", (U16)schLoadInfo->cellId);
       RETVALUE(RFAILED);        /* KW fix for LTE_ADV */   
   }                            
   absPatternCfgTable = &cellCb->lteAdvCb.absInfo.u.aggrInfo.absPatternCfgTable;

   for(idx = 0; idx < RGR_ABS_PATTERN_LEN; idx++)
   {
      /* Scheduler sends resourcre used in Non-ABS subframes */
      nonAbsRsrcUsed += schLoadInfo->u.absLoadInfo[idx];
   }
   /* Find out how much percentage of resource being available in Macro eNodeB */
   if(100 != absPatternCfgTable->absPatternCfg[
        cellCb->lteAdvCb.absInfo.u.aggrInfo.absPatternId].absPercentage)
   {
      percentageRsrcUsed = (U8)((nonAbsRsrcUsed*100*100)/(WR_EMM_ABS_LOAD_PERIODICITY*schLoadInfo->bw* 
                           (100 - absPatternCfgTable->absPatternCfg
                          [cellCb->lteAdvCb.absInfo.u.aggrInfo.absPatternId].absPercentage))); /* KW fix for LTE_ADV */
   }

   if(percentageRsrcUsed > 100)
   {
      RLOG1(L_DEBUG, " Non Abs percentage resource used(%d) exceeds 100 Percent ", 
            percentageRsrcUsed);
      percentageRsrcUsed = 100;
   }
    /*ABS_TESTING start */
    cumulativePercentageRsrcUsed+= percentageRsrcUsed;

    /*ABS_TESTING end*/

   cellCb->lteAdvCb.absInfo.u.aggrInfo.absRprtPeriodCount--;

   if(0 != cellCb->lteAdvCb.absInfo.u.aggrInfo.absRprtPeriodCount)
   {
      RETVALUE(ROK);
   }

   /* Reinitialize the absRprtPeriodCount */
   WR_EMM_ABS_CONVERT_NEGSEC_COUNT((cellCb->lteAdvCb.lteAdvCfg.absCfg.absLoadPeriodicity), 
                               (cellCb->lteAdvCb.absInfo.u.aggrInfo.absRprtPeriodCount));
 
    /*ABS_TESTING start*/
    averagePercentageRsrcUsed = cumulativePercentageRsrcUsed/cellCb->lteAdvCb.absInfo.u.aggrInfo.absRprtPeriodCount;
    /*Resetting to initial value*/
    cumulativePercentageRsrcUsed = 0;
    switch (absPatternCfgTable->absPatternCfg[cellCb->lteAdvCb.absInfo.u.aggrInfo.absPatternId].absPercentage)
    {
      case WR_EMM_ABS_PERCENTAGE_25:
      {
         if (averagePercentageRsrcUsed <= 25)
         {
            cellCb->lteAdvCb.absInfo.u.aggrInfo.absTotalRsrcAvail = WR_EMM_ABS_PERCENTAGE_75;
         }
         else if (averagePercentageRsrcUsed > 25 && averagePercentageRsrcUsed < 60)
         {
            /*change to WR_EMM_ABS_PERCENTAGE_50*/
            cellCb->lteAdvCb.absInfo.u.aggrInfo.absTotalRsrcAvail = WR_EMM_ABS_PERCENTAGE_50;
         }            
         break;
      }
      case WR_EMM_ABS_PERCENTAGE_50:
      {
         if (averagePercentageRsrcUsed > 90)
         {
            /*change to WR_EMM_ABS_PERCENTAGE_25*/
            cellCb->lteAdvCb.absInfo.u.aggrInfo.absTotalRsrcAvail = WR_EMM_ABS_PERCENTAGE_25;
         }
         else if (averagePercentageRsrcUsed <= 40)
         {
            /*change to WR_EMM_ABS_PERCENTAGE_75*/
            cellCb->lteAdvCb.absInfo.u.aggrInfo.absTotalRsrcAvail = WR_EMM_ABS_PERCENTAGE_75;
         }
         break;
                                          }
         case WR_EMM_ABS_PERCENTAGE_75:
         {   
            if (averagePercentageRsrcUsed > 90)
            {
                /*change to WR_EMM_ABS_PERCENTAGE_50*/
                cellCb->lteAdvCb.absInfo.u.aggrInfo.absTotalRsrcAvail = WR_EMM_ABS_PERCENTAGE_50;
            }
            break;
         }
         
         default: 
         {
            RLOG1(L_DEBUG, " AbsPercentage(%d) ",
               absPatternCfgTable->absPatternCfg[cellCb->lteAdvCb.absInfo.u.aggrInfo.absPatternId].absPercentage);
            break;
         }
     }

     RLOG1(L_DEBUG, " AbsTotalRsrcAvail(%d) ",cellCb->lteAdvCb.absInfo.u.aggrInfo.absTotalRsrcAvail);
    /*ABS_TESTING end*/  

   /* 2. ABS pattern is selected using ABS Pattern Selection Criteria */
   if(ROK != wrEmmAbsAggrSelectAbsPattern(&cause))
   {
      RETVALUE(ROK);
   }
   
   /* 4. Configure MAC Scheduler with new ABS Pattern */
   wrEmmAbsCfgScheduler(RGR_ENABLE);

   /* 5. Send peer eNodeBs with Load Indication with ABS Info */
   node = wrEmmCb.lteAdvCb.absInfo.peerInfo.u.aggrInfo.absPeerEnbLstCp.first;

   while(NULLP != node)
   {
      peerEnbCb = (WrEmmAbsAggrPeerEnbCb*)node->node;
      if(ROK != wrEmmAbsBuildAndSendLoadInfo(&peerEnbCb->enbId, 
                                     Cztid_ABSInform, ABSINFORM_FDD))
      {
         RLOG0(L_ERROR, " Sending ABS Load Information failed ");
         RETVALUE(RFAILED);
      }
      node = node->next;
   } 

   RETVALUE(ROK);
}

/** @brief This function handles the disabling of ABS feature during runtime of eNodeB
 *
 * @details
 * 
 *     Function: wrEmmAbsHndlAggrAbsDisableInd
 *
 *         Processing steps:
 *
 *
 * @param[in]   evnt    : Pointer to WrEmmAbsEvent 
 * @return S16
 *     -# Success : ROK
 *     -# Failure : RFAILED
 */
PRIVATE S16 wrEmmAbsHndlAggrAbsDisableInd 
(
WrEmmAbsEvent                    *evnt
)
{
   WrCellCb                      *cellCb;
   WrEmmAbsAggrPeerEnbCb         *peerEnbCb;
   CmLList                       *node;

   TRC2(wrEmmAbsHndlAggrAbsDisableInd)

   wrEmmCb.lteAdvCb.absInfo.state = WR_EMM_ABS_DISABLED_STATE;
   /*
   Steps:
   1. Initialize currentABSPatternId of CellCb to 0.
   2. Disable ABS feature of MAC Scheduler
   3. Send Load Information with ABS disabled to all peer eNodeB
   */

   WR_GET_ABS_CELLCB(cellCb);
   if((cellCb == NULLP))
   {
       RLOG0(L_ERROR, " CELLCB not found ");
       RETVALUE(RFAILED);   
   }  /*KW fix for LTE_ADV */       
   /* 1. Initialize agggressor info */
   cmMemset(cellCb->lteAdvCb.absInfo.absPattern, 0, WR_EMM_ABS_PATTERN_SIZE);
   cellCb->lteAdvCb.absInfo.u.aggrInfo.absPatternId = 0;
   cellCb->lteAdvCb.absInfo.u.aggrInfo.absTotalRsrcAvail = cellCb->lteAdvCb.absInfo.u.aggrInfo.absMinRsrc;

   WR_EMM_ABS_CONVERT_NEGSEC_COUNT((cellCb->lteAdvCb.lteAdvCfg.absCfg.absLoadPeriodicity), 
                               (cellCb->lteAdvCb.absInfo.u.aggrInfo.absRprtPeriodCount));

   /* 2. Disable ABS feature of MAC Scheduler */
   wrEmmAbsCfgScheduler(RGR_DISABLE);

   /* 3. Send Load Information with ABS disabled to all peer eNodeB */
   node = wrEmmCb.lteAdvCb.absInfo.peerInfo.u.aggrInfo.absPeerEnbLstCp.first;
   while(NULLP != node)
   {
      peerEnbCb = (WrEmmAbsAggrPeerEnbCb*)node->node;
      if(ROK != wrEmmAbsBuildAndSendRsrcStsReq(peerEnbCb,
                                  CztRegistration_RqststopEnum))

      {
         RLOG0(L_ERROR, " Sending Resource Status Request failed ");
         RETVALUE(RFAILED);
      }

      if(ROK != wrEmmAbsBuildAndSendLoadInfo(&peerEnbCb->enbId, 
                                Cztid_ABSInform, ABSINFORM_ABS_INACTIVE))
      {
         RLOG0(L_ERROR, " Sending ABS Load Information failed ");
         RETVALUE(RFAILED);
      }
      node = node->next;
   }
   
   RETVALUE(ROK);
}

/** @brief This function handles the enabling of ABS feature during runtime of eNodeB
 *
 * @details
 * 
 *     Function: wrEmmAbsHndlAggrAbsEnableInd
 *
 *         Processing steps:
 *
 *
 * @param[in]   evnt    : Pointer to WrEmmAbsEvent 
 * @return S16
 *     -# Success : ROK
 *     -# Failure : RFAILED
 */
PRIVATE S16 wrEmmAbsHndlAggrAbsEnableInd 
(
WrEmmAbsEvent                     *evnt
)
{
   WrEmmAbsAggrPeerEnbCb          *peerEnbCb;
   CmLList                        *node;
   WrEnumAbsPatternSelectFlrCause cause;

   TRC2(wrEmmAbsHndlAggrAbsEnableInd)
   wrEmmCb.lteAdvCb.absInfo.state = WR_EMM_ABS_ENABLED_STATE;

   /* 1. ABS pattern is selected using ABS Pattern Selection Criteria */
   /* 2. Store index of ABS pattern in currentABSPatternId of CellCb */
   if(ROK != wrEmmAbsAggrSelectAbsPattern(&cause))
   {
      /* This case will be hit when none of the peer are using ABS pattern */
      RETVALUE(ROK);
   }

   /* 3. Configure MAC Scheduler with new ABS Pattern */
   wrEmmAbsCfgScheduler(RGR_ENABLE);

   /* 4. Respond to eNodeBs in Table 8 with Load Indication with ABS Info */
   /* 5. Request eNodeB in Table 8 to start report resource periodically */
   node = wrEmmCb.lteAdvCb.absInfo.peerInfo.u.aggrInfo.absPeerEnbLstCp.first;
   while(NULLP != node)
   {
      peerEnbCb = (WrEmmAbsAggrPeerEnbCb*)node->node;

      if(ROK != wrEmmAbsBuildAndSendLoadInfo(&peerEnbCb->enbId, 
                                      Cztid_ABSInform, ABSINFORM_FDD))
      {
         RLOG0(L_ERROR, " Sending ABS Load Information failed ");
         RETVALUE(RFAILED);
      }

      if(ROK != wrEmmAbsBuildAndSendRsrcStsReq(peerEnbCb,
                                  CztRegistration_RqststartEnum))
      {
         RLOG0(L_ERROR, " Sending Resource Status Request failed ");
         RETVALUE(RFAILED);
      }
      node = node->next;
   }

   RETVALUE(ROK);
}

/** @brief This function handles the Load Information from X2AP 
 *         during ABS feature disabled in aggressor eNodeB
 *
 * @details
 *
 *     Function: wrEmmAbsHndlAggrLoadInfoInAbsDisable
 *
 *         Processing steps:
 *
 *
 * @param[in]   evnt    : Pointer to WrEmmAbsEvent 
 * @return S16
 *     -# Success : ROK
 *     -# Failure : RFAILED
 */
PRIVATE S16 wrEmmAbsHndlAggrLoadInfoInAbsDisable 
(
WrEmmAbsEvent                              *evnt
)
{
   U8                                       idx;
   WrEmmAbsEnbId                            enbId;
   Bool                                     isAbsInfoRqrd = FALSE;
   CztLoadInform                            *loadInfo;
   CztProtIE_Field_LoadInform_IEs           *loadInfoIe;
   CztProtIE_Field_CellInform_ItemIEs       *cellInfoIe;
   
   TRC2(wrEmmAbsHndlAggrLoadInfoInAbsDisable)

   loadInfo   = &evnt->u.x2apEvent.pdu->pdu.val.initiatingMsg.
                                           value.u.cztLoadInform;

   for(idx = 0; idx < loadInfo->protocolIEs.noComp.val; idx++)
   {
      loadInfoIe = &loadInfo->protocolIEs.member[idx];
      if(Cztid_CellInform == loadInfoIe->id.val)
      {
         break;
      }
   }

   if(idx == loadInfo->protocolIEs.noComp.val)
   {
      RLOG0(L_ERROR, " Cell Information not received ");
      RETVALUE(RFAILED); 
   }

   /* Construct eNodeB id */
   WR_GET_ENBID_FROM_PEERID(enbId.val, evnt->u.x2apEvent.peerId);
   RLOG2(L_EVENT,"[X2AP]:Received LOAD INFORMATION [Peer(%ld)<--Src(%ld)]",
         wrEmmCb.enbId,enbId.val);
   
   /* Search for ABS information within cellinfo  and call corresponding handler */
   for(idx = 0; idx < loadInfoIe->value.u.cztCellInform_Lst.noComp.val; idx++)
   {
      cellInfoIe = &loadInfoIe->value.u.cztCellInform_Lst.member[idx];
      if(cellInfoIe->value.u.cztCellInform_Item.iE_Extns.noComp.val != 0)
      {
         wrEmmAbsAggrHndlAbsLoadInfo(&enbId, cellInfoIe, &isAbsInfoRqrd);
      }
   }

   RETVALUE(ROK);
}

/** @brief This function handles error indication from lower layer
 *
 * @details
 *
 *     Function: wrEmmAbsHndlAggrErrorInd
 *
 *         Processing steps:
 *
 *
 * @param[in]   evnt    : Pointer to WrEmmAbsEvent 
 * @return S16
 *     -# Success : ROK
 *     -# Failure : RFAILED
 */

PRIVATE S16 wrEmmAbsHndlAggrErrorInd
(
WrEmmAbsEvent                *evnt
)
{
   CztLclErrInd              *errInd;
   WrEmmAbsEnbId             enbId;
   WrEmmAbsAggrPeerEnbCb     *peerEnbCb;

   TRC2(wrEmmAbsHndlAggrErrorInd)

   errInd = (CztLclErrInd*)evnt->u.errIndEvent.cb;
   if(NULLP == errInd)
   {
      RETVALUE(RFAILED);
   }
   
   RLOG2(L_DEBUG, " Error Ind for peer(0x%lx) for msg(0x%lx) ", errInd->peerId, evnt->u.errIndEvent.event); 
   switch(evnt->u.errIndEvent.event)
   {
      case CZT_MSG_LOAD_IND:
      case CZT_MSG_RSRC_ST_REQ:
      {
         WR_GET_ENBID_FROM_PEERID(enbId.val, errInd->peerId);

         if(ROK != wrEmmAbsAggrGetPeerEnbCb(&enbId, &peerEnbCb))
         {
            RLOG0(L_ERROR, " Failure in getting peer enodeb cb ");
            RETVALUE(RFAILED);
         }
         wrEmmAbsAggrRemoveNDeallocPeerEnbCb(peerEnbCb);
         break;
      }
      default:
      {
         break;
      }
   }
   RETVALUE(ROK);
}



/**********************************************************************/
/* IMPLEMENTATION OF PICO EVENT HANDLERS                              */
/**********************************************************************/

/** @brief This function handles the Cell Initialization Indication 
 *         for victim eNodeB(Pico) from eNodeB application
 *
 * @details
 *
 *     Function: wrEmmAbsHndlVctmCellInit
 *
 *         Processing steps:
 *
 * @param[in]   evnt    : Pointer to WrEmmAbsEvent 
 * @return S16
 *     -# Success : ROK
 *     -# Failure : RFAILED
 */
PRIVATE S16 wrEmmAbsHndlVctmCellInit 
(
WrEmmAbsEvent                  *evnt
)
{
   WrCellCb                    *cellCb;

   TRC2(wrEmmAbsHndlVctmCellInit)

   WR_GET_CELLCB(cellCb, evnt->u.cellEvent.cellId);
   if(cellCb == NULLP)            /* KW fix for LTE_ADV */
   {    
       RLOG1(L_ERROR, " CELLCB not found for given cellId(%d)", 
            (U16)evnt->u.cellEvent.cellId);
       RETVALUE(RFAILED);        /* KW fix for LTE_ADV */   
   }                         
   /* Currently one cell per eNodeB supported */
   wrEmmCb.lteAdvCb.absInfo.state  = (cellCb->lteAdvCb.lteAdvCfg.absCfg.status == RGR_ENABLE)?
                         WR_EMM_ABS_ENABLED_STATE : WR_EMM_ABS_DISABLED_STATE;

   /* Initialize ABS cell information to 0 */
   cmMemset((U8*)&cellCb->lteAdvCb.absInfo, 0, sizeof(WrEmmLteAdvAbsCellInfo));

   RETVALUE(ROK);
}

/** @brief This function handles the ABS UE Indication 
 *         for victim eNodeB(Pico) from eNodeB application
 *
 * @details 
 *
 *     Function: wrEmmAbsHndlVctmAbsUeInd
 *
 *         Processing steps:
 *
 * @param[in]   evnt    : Pointer to WrEmmAbsEvent 
 * @return S16
 *     -# Success : ROK
 *     -# Failure : RFAILED
 */
PRIVATE S16 wrEmmAbsHndlVctmAbsUeInd 
(
WrEmmAbsEvent                  *evnt
)
{
   U8                          idx;
   WrEmmAbsUeInfo              *ueInfo;
   WrEmmAbsVctmPeerEnbCb       *peerEnbCb;
   WrNeighEnbCb                *nghEnbCb;
   WrCellCb                    *cellCb = NULLP;

   TRC2(wrEmmAbsHndlVctmAbsUeInd)

   ueInfo = &evnt->u.absUeEvent;

   WR_GET_CELLCB(cellCb, evnt->u.absUeEvent.cellId);
   if(NULLP ==cellCb)
   {
      RETVALUE(ROK);
   }

   if(TRUE == cellCb->lteAdvCb.absInfo.isAbsEnabled)
   {
      /* Configure scheduler with ABS UE configuration */
      wrEmmAbsConfigUmmUe(evnt->u.absUeEvent.ueId, RGR_ENABLE,                         
                                     WR_UMM_LTE_ADV_LCL_UE);
   }

   /* Remove weightage and cell from UeCb */
   wrEmmAbsVctmRemoveAllCellFrmUeCb(ueInfo->ueId);

   for(idx = 0; idx < ueInfo->numOfCell; idx++)
   {
      /* Add new cell to ueCb */
      wrEmmAbsVctmAddCellToUeCb(ueInfo->ueId, &ueInfo->cellInfo[idx]);

      if(ROK == wrEmmAbsVctmGetPeerEnbCb(&ueInfo->cellInfo[idx].enbId, &peerEnbCb))
      {
         continue;
      }
      if(ROK != cmHashListFind(&(wrEmmCb.neighEnbLstCp),
                           (U8 *)&(ueInfo->cellInfo[idx].enbId.val),
                (U16)sizeof(ueInfo->cellInfo[idx].enbId.val), 0,(PTR *)&nghEnbCb))
      {
          RLOG1(L_DEBUG, " Failure in hash list find of enb(0x%x)",
               (U16)ueInfo->cellInfo[idx].enbId.val);
          continue;
      }

      if(WR_NEIGH_ENB_STATE_DOWN == nghEnbCb->x2Connection)
      {
         RLOG1(L_DEBUG, " X2 not connected to enb(0x%x)",
              (U16)ueInfo->cellInfo[idx].enbId.val);
         continue;
      }
      if(ROK != wrEmmAbsVctmAllocNAddPeerEnbCb(&ueInfo->cellInfo[idx].enbId, &peerEnbCb))
      {
         RLOG0(L_ERROR, "wrEmmAbsVctmAllocNAddPeerEnbCb failed.");
         RETVALUE(RFAILED);
      }

      if(ROK != wrEmmAbsBuildAndSendLoadInfo(&peerEnbCb->enbId, Cztid_InvokeInd, ABSINFORM_FDD))
      {
         RLOG0(L_ERROR, " Build and send load info failed ");
         RETVALUE(RFAILED);
      }
      wrStartTmr((PTR)peerEnbCb, WR_TMR_ABS_LOAD_INFO_TMR, WR_TMR_ABS_LOAD_INFO_TMR_VAL);
   }

   RETVALUE(ROK);
}

/** @brief This function handles the Non-ABS UE Indication 
 *         for victim eNodeB(Pico) from eNodeB application
 *
 * @details 
 *
 *     Function: wrEmmAbsHndlVctmNonAbsUeInd
 *
 *         Processing steps:
 *
 * @param[in]   evnt    : Pointer to WrEmmAbsEvent 
 * @return S16
 *     -# Success : ROK
 *     -# Failure : RFAILED
 */
PRIVATE S16 wrEmmAbsHndlVctmNonAbsUeInd 
(
WrEmmAbsEvent                     *evnt
)
{
   TRC2(wrEmmAbsHndlVctmNonAbsUeInd)

   /* Configure scheduler with ABS UE configuration */
   wrEmmAbsConfigUmmUe(evnt->u.absUeEvent.ueId, RGR_DISABLE,         
                                     WR_UMM_LTE_ADV_LCL_UE);

   /* Remove weightage and cell from UeCb */
   wrEmmAbsVctmRemoveAllCellFrmUeCb(evnt->u.absUeEvent.ueId);

   RETVALUE(ROK);
}

/** @brief This function handles the Load Information from X2AP 
 *         in victim eNodeB(Pico)
 *
 * @details
 *
 *     Function: wrEmmAbsHndlVctmLoadInfo
 *
 *         Processing steps:
 *
 *
 * @param[in]   evnt    : Pointer to WrEmmAbsEvent 
 * @return S16
 *     -# Success : ROK
 *     -# Failure : RFAILED
 */
PRIVATE S16 wrEmmAbsHndlVctmLoadInfo 
(
WrEmmAbsEvent                  *evnt
)
{
   U8                                       idx;
   WrEmmAbsEnbId                            enbId;
   CztLoadInform                            *loadInfo;
   CztProtIE_Field_LoadInform_IEs           *loadInfoIe;
   CztProtIE_Field_CellInform_ItemIEs       *cellInfoIe;
   WrEmmAbsVctmPeerEnbCb                    *peerEnbCb;
   WrEnumAbsPatternSelectFlrCause           cause;

   TRC2(wrEmmAbsHndlVctmLoadInfo)

   loadInfo   = &evnt->u.x2apEvent.pdu->pdu.val.initiatingMsg.
                                           value.u.cztLoadInform;
   static U32 count = 0;
   if (count++%10 == 0)
   {   
        count = 1;
        RLOG0(L_DEBUG, " Received Load Information at victim ");
   }
   for(idx = 0; idx < loadInfo->protocolIEs.noComp.val; idx++)
   {
      loadInfoIe = &loadInfo->protocolIEs.member[idx];
      if(Cztid_CellInform == loadInfoIe->id.val)
      {
         break;
      }
   }

   if(idx == loadInfo->protocolIEs.noComp.val)
   {
      RLOG0(L_ERROR, " Cell Information not received ");
      RETVALUE(RFAILED);
   }

   /* Construct eNodeB id */
   WR_GET_ENBID_FROM_PEERID(enbId.val, evnt->u.x2apEvent.peerId);

   if(ROK != wrEmmAbsVctmGetPeerEnbCb(&enbId, &peerEnbCb))
   {
      /* This is case will hit when ABS is disabled and then enabled in Macro eNodeB */
      RLOG1(L_DEBUG, " Load Info Recieved Voluntarily from peer enb(0x%x) ",
           (U16)enbId.val);
      if(ROK != wrEmmAbsVctmAllocNAddPeerEnbCb(&enbId, &peerEnbCb))
      {
         RLOG0(L_ERROR, "wrEmmAbsVctmAllocNAddPeerEnbCb Failed. ");
         RETVALUE(RFAILED);
      }
   }

   /* Search for ABS information within cellinfo  and call corresponding handler */
   for(idx = 0; idx < loadInfoIe->value.u.cztCellInform_Lst.noComp.val; idx++)
   {
      cellInfoIe = &loadInfoIe->value.u.cztCellInform_Lst.member[idx];
      if(cellInfoIe->value.u.cztCellInform_Item.iE_Extns.noComp.val != 0)
      {
         wrEmmAbsVctmHndlAbsLoadInfo(peerEnbCb, cellInfoIe);
      }
   }

   /* ABS pattern is selected using ABS Pattern Selection Criteria */
   if(ROK != wrEmmAbsVctmSelectAbsPattern(&cause))
   {
      if((cause == WR_EMM_ABS_NO_PATTERN) || (cause == WR_EMM_ABS_NULL_PATTERN))
      {
         /* Configure MAC Scheduler with new ABS Pattern */
         wrEmmAbsCfgScheduler(RGR_DISABLE);
         /* TODO: Disable UE ABS measurement report */
      }
      RETVALUE(ROK);
   }


   /* Configure MAC Scheduler with new ABS Pattern */
   wrEmmAbsCfgScheduler(RGR_ENABLE);

   /* Configure scheduler with UE info and UE with ABS info */
   wrEmmAbsConfigAllUmmUe();

   RETVALUE(ROK);
}

/** @brief This function handles the Resource Status Request from X2AP 
 *         in victim eNodeB(Pico)
 *
 * @details 
 *
 *     Function: wrEmmAbsHndlVctmRsrcStsReq
 *
 *         Processing steps:
 *
 *
 * @param[in]   evnt    : Pointer to WrEmmAbsEvent 
 * @return S16
 *     -# Success : ROK
 *     -# Failure : RFAILED
 */
PRIVATE S16 wrEmmAbsHndlVctmRsrcStsReq
(
WrEmmAbsEvent                    *evnt
)
{
   U8                                    idx = 0;
   S16                                   ret = ROK;
   WrEmmAbsEnbId                         enbId;
   CztResStatusRqst                      *x2RsrcStsReq;
   CztProtIE_Field_ResStatusRqst_IEs     *ie;
   WrEmmAbsVctmPeerEnbCb                 *peerEnbCb;

   TRC2(wrEmmAbsHndlVctmRsrcStsReq)

   /* Construct eNodeB id */
   WR_GET_ENBID_FROM_PEERID(enbId.val, evnt->u.x2apEvent.peerId);
   RLOG2(L_EVENT,"[X2AP]:Received RESOURCE STATUS REQUEST [Peer(%ld)<--Src(%ld)]",
         wrEmmCb.enbId,enbId.val);

   if(ROK != wrEmmAbsVctmGetPeerEnbCb(&enbId, &peerEnbCb))
   {
      RLOG1(L_ERROR, " PeerEnbCb(%d) not found ", (U16)enbId.val);
      RETVALUE(RFAILED);
   }

   x2RsrcStsReq = &evnt->u.x2apEvent.pdu->pdu.val.initiatingMsg.
                                    value.u.cztResStatusRqst;

   while(idx < x2RsrcStsReq->protocolIEs.noComp.val)
   {
      ie  = &x2RsrcStsReq->protocolIEs.member[idx++];
      switch(ie->id.val)
      {
         case Cztid_ENB1_Measurement_ID:
         {
            peerEnbCb->enb1MeasId = ie->value.u.cztid_ENB1_Measurement_ID.val;
            break;
         }
         case Cztid_Registration_Rqst:
         {
            peerEnbCb->absRprtStatus = (U8)(ie->value.u.cztRegistration_Rqst.val);  /* KW fix for LTE_ADV */

            peerEnbCb->absRprtPeriodCount  = 1;
            break;
         }
         case Cztid_ReportCharacteristics:
         {
            if(ie->value.u.cztReportCharacteristics.val[0] != 0x08) /* TODO *: */
            {
               ret = RFAILED;
            }
            break;
         }
         case Cztid_RprtngPeriodicity:
         {
            WR_EMM_ABS_CONVERT_ENUM_NEGSEC(ie->value.u.cztRprtngPeriodicity.val, peerEnbCb->rprtPeriodicity);
            WR_EMM_ABS_CONVERT_NEGSEC_COUNT(peerEnbCb->rprtPeriodicity, peerEnbCb->absRprtPeriodCount);
            break;
         }
         case Cztid_CellToReport:
         {
            if(CztRegistration_RqststartEnum == peerEnbCb->absRprtStatus)
            {
               ret = wrEmmAbsHndlVctmRsrcStsReqCellToRprt(peerEnbCb, ie);
            }
            break;
         }
      }

      if(ROK != ret)
      {
         /* TODO: Get proper cause  */
         wrEmmAbsBuildAndSendRsrcStsFlr(peerEnbCb, 3, (U32)CztCauseMiscunspecifiedEnum);
         RETVALUE(ret);
      }
   }

   /* Send Resource Status Success */
   wrEmmAbsBuildAndSendRsrcStsRsp(peerEnbCb);

   RETVALUE(ROK);
}

/** @brief This function handles the Resource Status Request from X2AP 
 *         in victim eNodeB(Pico) when ABS is disabled
 *
 * @details 
 *
 *     Function: wrEmmAbsHndlVctmRsrcStsReqInAbsDisable
 *
 *         Processing steps:
 *
 *
 * @param[in]   evnt    : Pointer to WrEmmAbsEvent 
 * @return S16
 *     -# Success : ROK
 *     -# Failure : RFAILED
 */
PRIVATE S16 wrEmmAbsHndlVctmRsrcStsReqInAbsDisable
(
WrEmmAbsEvent                    *evnt
)
{
   WrEmmAbsEnbId                          enbId;      
   WrEmmAbsVctmPeerEnbCb                 *peerEnbCb;

   TRC2(wrEmmAbsHndlVctmRsrcStsReqInAbsDisable)

   /* Construct eNodeB id */
   WR_GET_ENBID_FROM_PEERID(enbId.val, evnt->u.x2apEvent.peerId);

   if(ROK != wrEmmAbsVctmGetPeerEnbCb(&enbId, &peerEnbCb))
   {
      RLOG1(L_ERROR, " PeerEnbCb(%d) not found ", (U16)enbId.val);
      RETVALUE(RFAILED);
   }

   /* Send Resource Status Success */
   wrEmmAbsBuildAndSendRsrcStsRsp(peerEnbCb);

   /* Release peer eNodeB */
   wrEmmAbsVctmRemoveNDeallocPeerEnbCb(peerEnbCb);

   RETVALUE(ROK);
}

/** @brief This function handles the peer up indication from X2AP 
 *         in victim eNodeB(Pico)
 *
 * @details 
 *
 *     Function: wrEmmAbsHndlVctmX2UpInd
 *
 *         Processing steps:
 *
 *
 * @param[in]   evnt    : Pointer to WrEmmAbsEvent 
 * @return S16
 *     -# Success : ROK
 *     -# Failure : RFAILED
 */
PRIVATE S16 wrEmmAbsHndlVctmX2UpInd 
(
WrEmmAbsEvent                     *evnt
)
{
   WrEmmAbsEnbId                  enbId;     
   WrEmmAbsVctmPeerEnbCb          *peerEnbCb;

   TRC2(wrEmmAbsHndlVctmX2UpInd)

   /* Construct eNodeB id */
   WR_GET_ENBID_FROM_PEERID(enbId.val, evnt->u.x2apEvent.peerId);

   if(ROK == wrEmmAbsVctmGetPeerEnbCb(&enbId, &peerEnbCb))
   {
      RLOG1(L_ERROR, " eNodeB(%d) not found ", (U16)enbId.val);
      RETVALUE(RFAILED);
   }

   if(TRUE != wrEmmAbsVctmFndAnyUeFacingInerference(&enbId))
   {
      RETVALUE(ROK);
   }

   if(ROK != wrEmmAbsVctmAllocNAddPeerEnbCb (&enbId, &peerEnbCb))
   {
      RLOG0(L_ERROR, " eNodeB peer cb allocation failed ");
      RETVALUE(RFAILED);
   }

   if(ROK != wrEmmAbsBuildAndSendLoadInfo(&enbId, Cztid_InvokeInd, ABSINFORM_FDD))
   {
      RLOG1(L_ERROR, " Load Information sending failed for eNbodeB(%d) ", (U16)enbId.val);

      wrEmmAbsVctmRemoveNDeallocPeerEnbCb(peerEnbCb);
      RETVALUE(RFAILED);
   }
   wrStartTmr((PTR)peerEnbCb, WR_TMR_ABS_LOAD_INFO_TMR, WR_TMR_ABS_LOAD_INFO_TMR_VAL);

   RETVALUE(ROK);
}

/** @brief This function handles the peer down indication from X2AP 
 *         in victim eNodeB(Pico)
 *
 * @details 
 *
 *     Function: wrEmmAbsHndlVctmX2DownInd
 *
 *         Processing steps:
 *
 *
 * @param[in]   evnt    : Pointer to WrEmmAbsEvent 
 * @return S16
 *     -# Success : ROK
 *     -# Failure : RFAILED
 */
PRIVATE S16 wrEmmAbsHndlVctmX2DownInd 
(
WrEmmAbsEvent                   *evnt
)
{
   WrEmmAbsEnbId                enbId;
   WrEmmAbsVctmPeerEnbCb        *peerEnbCb;
   TRC2(wrEmmAbsHndlVctmX2DownInd)

   /* Construct eNodeB id */
   WR_GET_ENBID_FROM_PEERID(enbId.val, evnt->u.x2apEvent.peerId);

   if(ROK != wrEmmAbsVctmGetPeerEnbCb(&enbId, &peerEnbCb))
   {
      RLOG1(L_ERROR, "  eNodeB(%d) not found ", (U16)enbId.val);
      RETVALUE(RFAILED);
   }
   
   wrEmmAbsVctmRemoveNDeallocPeerEnbCb(peerEnbCb);

   /* Stop ABS at MAC if there is no more Vctm */
   if (wrEmmCb.lteAdvCb.absInfo.peerInfo.u.vctmInfo.absPeerEnbLstCp.count == 0)
   {
      /* Since currently we are supporting interference from only one macro so following function.
       * Else we would have to disable all the UEs related to following EnodeB and doesnot
       * have to call this function since it will disable the ABS for complete cell. */
      wrEmmAbsCfgScheduler(RGR_DISABLE);
      wrEmmAbsConfigAllUmmUe();
   }

   RETVALUE(ROK);
}

/** @brief This function handles the cell up indication from eNodeB app 
 *         in victim eNodeB(Pico)
 *
 * @details 
 *
 *     Function: wrEmmAbsHndlVctmCellUpInd
 *
 *         Processing steps:
 *
 *
 * @param[in]   evnt    : Pointer to WrEmmAbsEvent 
 * @return S16
 *     -# Success : ROK
 *     -# Failure : RFAILED
 */
PRIVATE S16 wrEmmAbsHndlVctmCellUpInd 
(
WrEmmAbsEvent                     *evnt
)
{
   TRC2(wrEmmAbsHndlVctmCellUpInd)
    
   RETVALUE(ROK);
}

/** @brief This function handles the cell down indication from eNodeB app 
 *         in victim eNodeB(Pico)
 *
 * @details 
 *
 *     Function: wrEmmAbsHndlVctmCellDownInd
 *
 *         Processing steps:
 *
 *
 * @param[in]   evnt    : Pointer to WrEmmAbsEvent 
 * @return S16
 *     -# Success : ROK
 *     -# Failure : RFAILED
 */
PRIVATE S16 wrEmmAbsHndlVctmCellDownInd 
(
WrEmmAbsEvent                     *evnt
)
{
   TRC2(wrEmmAbsHndlVctmCellDownInd)
   RETVALUE(ROK);
}

/** @brief This function handles the Load Indocation from scheduler
 *         in victim eNodeB(Pico)
 *
 * @details 
 *
 *     Function: wrEmmAbsHndlVctmSchLoadInd
 *
 *         Processing steps:
 *
 *
 * @param[in]   evnt    : Pointer to WrEmmAbsEvent 
 * @return S16
 *     -# Success : ROK
 *     -# Failure : RFAILED
 */
PRIVATE S16 wrEmmAbsHndlVctmSchLoadInd 
(
WrEmmAbsEvent                     *evnt
)
{
   U8                             idx;
   U32                            absRsrcUsed = 0;
   RgrLoadInfIndInfo              *schLoadInfo;
   WrCellCb                       *cellCb;
   CmLList                        *node;
   WrEmmAbsVctmPeerEnbCb          *peerEnbCb;
   WrEnumAbsPatternSelectFlrCause cause;

   TRC2(wrEmmAbsHndlVctmSchLoadInd)

   schLoadInfo = evnt->u.schLoadInfoEvent.schLoadInfo;
   if(RGR_ABS != schLoadInfo->type)
   {
      RETVALUE(RFAILED);
   }

   WR_GET_CELLCB(cellCb, schLoadInfo->cellId);
   if(cellCb == NULLP)           
   {    
       RLOG1(L_ERROR, " CELLCB not found for given cellId(%d) ", 
                   (U16)schLoadInfo->cellId);
       RETVALUE(RFAILED);        /* KW fix for LTE_ADV */   
   }                         
   for(idx = 0; idx < RGR_ABS_PATTERN_LEN; idx++)
   {
      /* Scheduler sends resource used in ABS subframes */
      absRsrcUsed += schLoadInfo->u.absLoadInfo[idx];
   }

   if(cellCb->lteAdvCb.absInfo.u.vctmInfo.absPercentage)
   {
      cellCb->lteAdvCb.absInfo.u.vctmInfo.absPercentageRsrcUsed = 
            (U8)((absRsrcUsed*100*100) / (WR_EMM_ABS_LOAD_PERIODICITY*
             schLoadInfo->bw*(cellCb->lteAdvCb.absInfo.u.vctmInfo.absPercentage))); /* KW fix for LTE_ADV */
   }
   else
   {
      cellCb->lteAdvCb.absInfo.u.vctmInfo.absPercentageRsrcUsed = 0;
   }

   if(cellCb->lteAdvCb.absInfo.u.vctmInfo.absPercentageRsrcUsed > 100)
   {
      RLOG1(L_DEBUG, " Abs percentage resource used(%d) exceeds 100 percent ", 
               cellCb->lteAdvCb.absInfo.u.vctmInfo.absPercentageRsrcUsed);
      cellCb->lteAdvCb.absInfo.u.vctmInfo.absPercentageRsrcUsed = 100;
   }


   node = wrEmmCb.lteAdvCb.absInfo.peerInfo.u.vctmInfo.absPeerEnbLstCp.first;
   while(NULLP != node)
   {
      peerEnbCb = (WrEmmAbsVctmPeerEnbCb*)node->node;
      node      = node->next;
     
      if(CztRegistration_RqststopEnum == peerEnbCb->absRprtStatus)
      {
         continue;
      }
      
      peerEnbCb->absRprtPeriodCount--;
      if(0 != peerEnbCb->absRprtPeriodCount)
      {
         continue;
      }

      WR_EMM_ABS_CONVERT_NEGSEC_COUNT(peerEnbCb->rprtPeriodicity, peerEnbCb->absRprtPeriodCount);

      if(ROK != wrEmmAbsBuildAndSendRsrcStsUpd(peerEnbCb))
      {
         RLOG1(L_ERROR, " Failed to send resource status update to enb(%d) ",
              (U16)peerEnbCb->enbId.val);
      }
   }

   /* 1. Reset ABS Resource Used */
   cellCb->lteAdvCb.absInfo.u.vctmInfo.absPercentageRsrcUsed = 0; 

   /* 2. ABS pattern is selected using ABS Pattern Selection Criteria */
   if(ROK != wrEmmAbsVctmSelectAbsPattern(&cause))
   {
      if((cause == WR_EMM_ABS_NO_PATTERN) || (cause == WR_EMM_ABS_NULL_PATTERN))
      {
         /* Configure MAC Scheduler with new ABS Pattern */
         wrEmmAbsCfgScheduler(RGR_DISABLE);
         /* TODO: Disable UE ABS measurement report */
      }

      RETVALUE(ROK);
   }

   /* 4. Configure MAC Scheduler with new ABS Pattern */
   wrEmmAbsCfgScheduler(RGR_ENABLE);

   /* Configure scheduler with UE info and UE with ABS info */
   wrEmmAbsConfigAllUmmUe();

   RETVALUE(ROK);
}

/** @brief This function handles the enabling of ABS feature during runtime of eNodeB
 *         in victim eNodeB(Pico)
 * @details
 * 
 *     Function: wrEmmAbsHndlVctmAbsEnableInd
 *
 *         Processing steps:
 *
 *
 * @param[in]   evnt    : Pointer to WrEmmAbsEvent 
 * @return S16
 *     -# Success : ROK
 *     -# Failure : RFAILED
 */
PRIVATE S16 wrEmmAbsHndlVctmAbsEnableInd 
( 
WrEmmAbsEvent                      *evnt
)
{
   TRC2(wrEmmAbsHndlVctmAbsEnableInd)
   wrEmmCb.lteAdvCb.absInfo.state = WR_EMM_ABS_ENABLED_STATE;
   /* UE configuration of ABS measurement report */
   wrUmmHdlLteAdvRecfg(evnt->u.absCfgEvent.cellId, RGR_ABS, RGR_ENABLE);
   RETVALUE(ROK);
}

/** @brief This function handles the disabling of ABS feature during runtime
 *         of victim eNodeB(Pico)
 * @details 
 * 
 *     Function: wrEmmAbsHndlVctmAbsDisableInd
 *
 *         Processing steps:
 *
 *
 * @param[in]   evnt    : Pointer to WrEmmAbsEvent 
 * @return S16
 *     -# Success : ROK
 *     -# Failure : RFAILED
 */
PRIVATE S16 wrEmmAbsHndlVctmAbsDisableInd
(
WrEmmAbsEvent                       *evnt
)
{
   WrEmmAbsVctmPeerEnbCb            *peerEnbCb;
   CmLList                          *node;

   TRC2(wrEmmAbsHndlVctmAbsDisableInd)

   wrEmmCb.lteAdvCb.absInfo.state = WR_EMM_ABS_DISABLED_STATE;
   
   /* Send ABS disable to Scheduler */
   wrEmmAbsCfgScheduler(RGR_DISABLE);

   /* Configure scheduler with UE info and UE with ABS info */
   wrEmmAbsConfigAllUmmUe();


   node = wrEmmCb.lteAdvCb.absInfo.peerInfo.u.vctmInfo.absPeerEnbLstCp.first;
   while(NULLP != node)
   {
      peerEnbCb = (WrEmmAbsVctmPeerEnbCb*)node->node;
      node = node->next;

      if(ROK != wrEmmAbsBuildAndSendLoadInfo(&peerEnbCb->enbId, 
                            Cztid_ABSInform, ABSINFORM_ABS_INACTIVE))
      {
         RLOG1(L_ERROR, " Load Information sending failed for eNbodeB(0x%x) ",
              (U16)peerEnbCb->enbId.val);
      }

      if(CztRegistration_RqststopEnum == peerEnbCb->absRprtStatus)
      {
         wrEmmAbsVctmRemoveNDeallocPeerEnbCb(peerEnbCb);
      }
      else
      {
         RLOG0(L_INFO, " Waiting for resource status update disable message ");
         /* TODO: Start timer */
      }
   }

   /* UE configuration of ABS measurement report */
   wrUmmHdlLteAdvRecfg(evnt->u.absCfgEvent.cellId, RGR_ABS, RGR_DISABLE);

   RETVALUE(ROK);
}

/** @brief This function handles the release of UE in victim eNodeB(Pico)
 * @details 
 * 
 *     Function: wrEmmAbsHndlVctmUeRelInd
 *
 *         Processing steps:
 *
 *
 * @param[in]   evnt    : Pointer to WrEmmAbsEvent 
 * @return S16
 *     -# Success : ROK
 *     -# Failure : RFAILED
 */
PRIVATE S16 wrEmmAbsHndlVctmUeRelInd 
(
WrEmmAbsEvent                  *evnt
)
{
   TRC2(wrEmmAbsHndlVctmUeRelInd)
  
   /* Remove weightage and cell from UeCb */
   wrEmmAbsVctmRemoveAllCellFrmUeCb( evnt->u.absUeEvent.ueId);

   RETVALUE(ROK);
}

/** @brief This function handles error indication from lower layer
 *
 * @details
 *
 *     Function: wrEmmAbsHndlVctmErrorInd
 *
 *         Processing steps:
 *
 *
 * @param[in]   evnt    : Pointer to WrEmmAbsEvent 
 * @return S16
 *     -# Success : ROK
 *     -# Failure : RFAILED
 */

PRIVATE S16 wrEmmAbsHndlVctmErrorInd
(
WrEmmAbsEvent                *evnt
)
{
   CztLclErrInd              *errInd;
   WrEmmAbsVctmPeerEnbCb     *peerEnbCb;
   WrEmmAbsEnbId             enbId;

   TRC2(wrEmmAbsHndlVctmErrorInd)

   errInd = (CztLclErrInd*)evnt->u.errIndEvent.cb;
   if(NULLP == errInd)
   {
      RLOG0(L_ERROR, "errInd is NULL");
      RETVALUE(RFAILED);
   }
   RLOG2(L_ERROR, " Error Ind for peer(0x%lx) for msg(0x%lx) ",errInd->peerId, evnt->u.errIndEvent.event);
   switch(evnt->u.errIndEvent.event)
   {
      case CZT_MSG_LOAD_IND:
      case CZT_MSG_RSRC_ST_UPD:
      case CZT_MSG_RSRC_ST_RSP:
      case CZT_MSG_RSRC_ST_FAIL:
      {
         WR_GET_ENBID_FROM_PEERID(enbId.val, errInd->peerId);

         if(ROK != wrEmmAbsVctmGetPeerEnbCb(&enbId, &peerEnbCb))
         {
            RETVALUE(RFAILED);
         }
         wrEmmAbsVctmRemoveNDeallocPeerEnbCb(peerEnbCb);
         break;
      }
      default:
      {
         break;
      }
   }

   RETVALUE(ROK);
}

/** @brief This function handles timer expiry event
 *
 * @details
 *
 *     Function: wrEmmAbsHndlVctmTimerExpiryInd
 *
 *         Processing steps:
 *
 *
 * @param[in]   evnt    : Pointer to WrEmmAbsEvent 
 * @return S16
 *     -# Success : ROK
 *     -# Failure : RFAILED
 */

PRIVATE S16 wrEmmAbsHndlVctmTimerExpiryInd
(
WrEmmAbsEvent                *evnt
)
{
   WrEmmAbsVctmPeerEnbCb     *peerEnbCb;
   TRC2(wrEmmAbsHndlVctmTimerExpiryInd)
   
   if(WR_TMR_ABS_LOAD_INFO_TMR == evnt->u.timerEvent.event)
   {
      peerEnbCb = (WrEmmAbsVctmPeerEnbCb*)evnt->u.timerEvent.cb;

      if(ROK != wrEmmAbsBuildAndSendLoadInfo(&peerEnbCb->enbId, Cztid_InvokeInd, ABSINFORM_FDD))
      {
         RLOG1(L_ERROR, " Load Information sending failed for eNbodeB(%d) ", (U16)peerEnbCb->enbId.val);

         wrEmmAbsVctmRemoveNDeallocPeerEnbCb(peerEnbCb);
         RETVALUE(RFAILED);
      }
      wrStartTmr((PTR)peerEnbCb, WR_TMR_ABS_LOAD_INFO_TMR, WR_TMR_ABS_LOAD_INFO_TMR_VAL);
   }
   else
   {
      RLOG1(L_ERROR, " Invalid Timer Event(0x%lx)",evnt->u.timerEvent.event); 
   }

   RETVALUE(ROK);
}




/** @brief This function handles abs event in case of 
 *         static abs pattern negotiation
 *
 * @details 
 *
 *     Function: wrEmmAbsStaticProcessEvent
 *
 *         Processing steps:
 *
 * @param[in]   evnt : Pointer to ABS event  
 * @return S16
 *     -# Success : ROK
 *     -# Failure : RFAILED
 */
PUBLIC S16 wrEmmAbsStaticProcessEvent
(
WrEmmAbsEvent                   *evnt
)
{
   WrCellCb                     *cellCb = NULLP;
   TRC2(wrEmmAbsStaticProcessEvent)

   switch(evnt->eventId)
   {
      case WR_EMM_ABS_DISABLED_EVENT:
      {
         wrEmmCb.lteAdvCb.absInfo.state = WR_EMM_ABS_DISABLED_STATE; 
         /*ABS_TESTING start */
         WR_GET_CELLCB(cellCb, evnt->u.absCfgEvent.cellId);
    
         if(cellCb == NULLP)
         {
            RLOG0(L_ERROR, "cellCb is NULL");
            RETVALUE(RFAILED);
         }
         cellCb->lteAdvCb.absInfo.isAbsEnabled = FALSE;
         /*ABS_TESTING end*/
         /* If any ABS Ue mark that as non ABS UE at MAC */
         if (WR_LTE_ADV_PICO_ENB == wrEmmCb.lteAdvCb.enbType)
         {
            wrEmmAbsConfigAllUmmUe();
         }

         /* Scheduler configuration of ABS feature */
         wrEmmCellSchdReCfgLteAdvFeatures(evnt->u.absCfgEvent.cellId, evnt->u.absCfgEvent.lteAdvTrans);
         /* UE configuration of ABS measurement */
         wrUmmHdlLteAdvRecfg(evnt->u.absCfgEvent.cellId, RGR_ABS, RGR_DISABLE);
         break;
      }
      case WR_EMM_ABS_ENABLED_EVENT:
      {
         wrEmmCb.lteAdvCb.absInfo.state = WR_EMM_ABS_ENABLED_STATE; 
         /*ABS_TESTING start */
         WR_GET_CELLCB(cellCb, evnt->u.absCfgEvent.cellId);
    
         if(cellCb == NULLP)
         {
            RLOG0(L_ERROR, "cellCb is NULL");
            RETVALUE(RFAILED);
         }
         cellCb->lteAdvCb.absInfo.isAbsEnabled = TRUE;
         /*ABS_TESTING end*/       
         
         wrEmmCellSchdReCfgLteAdvFeatures(evnt->u.absCfgEvent.cellId, evnt->u.absCfgEvent.lteAdvTrans);
         /* UE configuration of ABS measurement report */
         wrUmmHdlLteAdvRecfg(evnt->u.absCfgEvent.cellId, RGR_ABS, RGR_ENABLE);
         break;
      }
      case WR_EMM_ABS_ABS_UE_EVENT:
      {
         if((WR_EMM_ABS_ENABLED_STATE == wrEmmCb.lteAdvCb.absInfo.state) && 
                             (WR_LTE_ADV_PICO_ENB == wrEmmCb.lteAdvCb.enbType))
         {
            WR_GET_CELLCB(cellCb, evnt->u.absUeEvent.cellId);
            
         /*ABS_TESTING start */
            if(cellCb == NULLP)
         /*ABS_TESTING end */
            {
               RLOG0(L_ERROR, "cellCb is NULL");
               RETVALUE(RFAILED);
            }
            if(TRUE == cellCb->lteAdvCb.absInfo.isAbsEnabled)
            {
               /* Configure scheduler with ABS UE configuration */
               wrEmmAbsConfigUmmUe(evnt->u.absUeEvent.ueId, RGR_ENABLE, WR_UMM_LTE_ADV_LCL_UE);
            }
         }
         break;
      }
      case WR_EMM_ABS_NONABS_UE_EVENT:
      {
         if((WR_EMM_ABS_ENABLED_STATE == wrEmmCb.lteAdvCb.absInfo.state) && 
                             (WR_LTE_ADV_PICO_ENB == wrEmmCb.lteAdvCb.enbType))
         {
            /* Configure scheduler with ABS UE configuration */
            wrEmmAbsConfigUmmUe(evnt->u.absUeEvent.ueId, RGR_DISABLE, WR_UMM_LTE_ADV_LCL_UE);
         }
         break;
      }
      default:
      {
         break;
      }         
   }

   RETVALUE(ROK);
}

/** @brief This function abs event in case of 
 *         dynamic abs pattern negotiation
 *
 * @details 
 *
 *     Function: wrEmmAbsDynamicProcessEvent 
 *
 *         Processing steps:
 *            1. Call specific FSM handlers
 *
 * @param[in] evnt: Pointer to ABS event 
 * @return S16
 *     -# Success : ROK
 *     -# Failure : RFAILED
 */
PUBLIC S16 wrEmmAbsDynamicProcessEvent
(
WrEmmAbsEvent           *evnt
)
{
   TRC2(wrEmmAbsDynamicProcessEvent)

   if(WR_LTE_ADV_PICO_ENB == wrEmmCb.lteAdvCb.enbType)
   {
      absVctmEvntHndlr[wrEmmCb.lteAdvCb.absInfo.state][evnt->eventId](evnt);
   }
   else
   {
      absAggrEvntHndlr[wrEmmCb.lteAdvCb.absInfo.state][evnt->eventId](evnt);
   }

   RETVALUE(ROK);
}
/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/
/*%                        EMM ABS MODULE - END                      %*/
/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/
/* LTE_ADV_ABS ends */

/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/
/*%                       EMM DSFR MODULE - END                      %*/
/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/
/* LTE_ADV_DSFR starts */

/** @brief This function parses the Cell Informl infromation
 *         IE from the message and updates the neighbouring
 *         eNodeB CB.
 *
 * @details
 *
 *     Function: wrEmmNghEnbUpdEnbCellInfo
 *
 *         Processing steps:
 *         - It processes the Cell Inform IE from the
 *           message.
 *       
 *
 * @param[out]      enbnghCb  : Neighbour eNodeB Control Block
 * @param[in]        srvdCellIe  : Serverd Cell Info IE
 * @return S16 
 *     -# Success : ROK
 *     -# Failure   : RFAILED
 */
PRIVATE S16 wrEmmNghEnbUpdEnbCellInfo
(
WrNeighEnbCb                 *enbnghCb,
CztCellInform_Lst            *cellInfoLst
)
{
   S16                    ret = RFAILED;
   CztProtIE_Field_CellInform_ItemIEs     *ie;
   CztCellInform_Item     *cellInfo;
   RgrLoadInfReqInfo      *loadInfReq = NULLP;       /* Information to be sent to SCH via SPstTsk   */
   U8                     tempBit;                   /* Temp variable to store particular bit value */
   U8                     enbnghCEStartRbTmp;        /* Temp variable to store ngh CE StartRb       */
   U8                     pHighStartLocByte;         /* Temp variable to store Byte number          */
   U8                     pHighStartLocBit;          /* Temp variable to store Bit Position         */
   U32                    transId;                   /* transaction id for MAC                      */
   /* dsfr_pal_fixes */
   U8                     cellIdx = 0;

   WR_ALLOC(&loadInfReq, sizeof(RgrLoadInfReqInfo));
   if(NULLP == loadInfReq)
   {
      RLOG0(L_FATAL, "Memory allocation failed.");
      RETVALUE(RFAILED);
   }

   ie = &cellInfoLst->member[0];

   /* obtain cellInfo block containing RNTP info sent from Peer eNodeB */
   if((ie->pres.pres == PRSNT_NODEF) && (ie->id.val == Cztid_CellInform_Item))
   {
      cellInfo = &ie->value.u.cztCellInform_Item;
   }
   else
   {
      RLOG0(L_ERROR, " Invalid CztProtIE_Field_CellInform_ItemIEs info received ");
      RETVALUE(RFAILED);
   }
   if((cellInfo->relativeNarrowbandTxPower.pres.pres == NOTPRSNT) || (cellInfo->relativeNarrowbandTxPower.rNTP_PerPRB.pres == NOTPRSNT))
   {
      RLOG2(L_ERROR, " RNTP Info is NOT Present in LOAD IND so returning with FAILURE,\
            RNTP status (%d), RNTP per PRB status (%d) ", cellInfo->relativeNarrowbandTxPower.pres.pres,
                                                  cellInfo->relativeNarrowbandTxPower.rNTP_PerPRB.pres); 
      RETVALUE(RFAILED);
   }
   /* ABS_TESTING :: removed the check of RNTP */
   /* Now we need to fill RgrLoadInfReqInfo and Send it to SCH as LOAD INF Msg */

   /* STEP 1 ::
    * loadInfReq->rgrCcPHighEndRb will be same as enbnghCb->dsfrCEFreqRange.endRb
    * endRb will not change as we use same sub-bands f1, f2 & f3 among neighbour eNB's */
   loadInfReq->rgrCcPHighEndRb = enbnghCb->dsfrCEFreqRange.endRb;

   /* STEP 2 ::
    * Find out loadInfReq->rgrCcPHighStartRb */
   tempBit                 = 1;
   enbnghCEStartRbTmp      = enbnghCb->dsfrCEFreqRange.startRb;

   /* RNTP is set as 1 in peer eNB for PRB's which are transmitted by UE on P-High.
    * We need to identify the first entry in RNTP as 0 in CE band of peer eNB which corresponds
    * to CC sub-band here */
   while (tempBit && (enbnghCEStartRbTmp < enbnghCb->dsfrCEFreqRange.endRb))
   {
      pHighStartLocByte = (U8)(enbnghCEStartRbTmp / 8); /*KW fix for LTE_ADV */
      pHighStartLocBit  = (U8)(enbnghCEStartRbTmp % 8); /*KW fix for LTE_ADV */  

      tempBit           = (U8)WR_EMM_CHECK_RNTP_BIT
                             (cellInfo->relativeNarrowbandTxPower.rNTP_PerPRB.val[pHighStartLocByte], pHighStartLocBit);
                           /*KW fix for LTE_ADV */ 
      enbnghCEStartRbTmp++;
   }

   loadInfReq->rgrCcPHighStartRb = (U8)(enbnghCEStartRbTmp - 1); /* KW fix for LTE_ADV */

   /* STEP 3 ::
    * Invoke WrIfmSchdLoadInfReq function (TODO SKS :: Check for transId) */
   /* dsfr_pal_fixes ** 22-March-2013 ** SKS */
   loadInfReq->cellId = wrEmmCb.cellCb[cellIdx]->cellId;
   transId = (loadInfReq->cellId << 16 | WR_LOAD_INF_TRANS_ID);
   RLOG3(L_DEBUG, " Enb Id: %lu : Cell Center PHigh Range: Start Rb [%u] End Rb [%u] ",
         enbnghCb->enbId,loadInfReq->rgrCcPHighStartRb,loadInfReq->rgrCcPHighEndRb);          
   
   if (WrIfmSchdLoadInfReq(transId, loadInfReq) != ROK)
   {
      RLOG0(L_ERROR, " WrIfmSchdLoadInfReq primitive failed. ");
      RETVALUE(RFAILED);
   }

   /*Finding the neighbour eNodeB control block*/

   RETVALUE(ret);
}

/** @brief This function parses all the IEs from the
 *         Load Inform message.
 *
 * @details
 *
 *     Function: wrEmmNghEnbUpdNghCbFromLoadInfo
 *
 *         Processing steps:
 *         - It parses all the IEs from the Load Inform
 *           message and then call correponding handler to
 *           update the information based in IE Ids.
 *
 *
 * @param[out]   enbnghCb  : Neighbour eNodeB Control Block
 * @param[in]     setupReq   : Load Inform mesaage
 * @return S16 
 *      -# Success : ROK
 */
PRIVATE S16 wrEmmNghEnbUpdNghCbFromLoadInfo
(
WrNeighEnbCb                 *enbnghCb,
CztLoadInform                *loadInfo
)
{
   U8                              idx;
   S16                             ret = RFAILED;
   CztProtIE_Field_LoadInform_IEs *IEs = loadInfo->protocolIEs.member;

   for(idx = 0; idx < loadInfo->protocolIEs.noComp.val; idx++)
   {
      CztProtIE_Field_LoadInform_IEs  *ie = IEs + idx;

      switch(ie->id.val)
      {
         case Cztid_CellInform:
            ret = wrEmmNghEnbUpdEnbCellInfo(enbnghCb, &ie->value.u.cztCellInform_Lst);
            RETVALUE(ROK);
      }
   }
 RETVALUE(ret);
}/*wrEmmNghEnbUpdateNghEnbCb*/


/** @brief This function constructs the X2AP Load Information message.
 *
 *
 * @details
 *
 *     Function: wrEmmNghEnbBldLoadInfo
 *
 *         Processing steps:
 *         - It builds the CZT X2AP Reset Request
 *           message with all the necessary information.
 *
 *
 * @param[out]  pdu    : Czt Event Structure
 * @param[in]    cause : Cause Value
 * @return S16 
 *   -# Success : ROK
 *   -# Failure   : RFAILED
 */
PRIVATE S16 wrEmmNghEnbBldLoadInfo
(
CztEvnt                      **pdu,
RgrLoadInfIndInfo             *loadInfInd

)
{
   CztEvnt                   *x2LoadInfoPdu;
   CztLoadInform             *x2LoadInfo;
   CztProtIE_Field_LoadInform_IEs *ie;
   U8                        ieIdx = 0;
   S16                       ret = ROK;
   U8                        numComp;
   U8                        cellIdx;
   U8                        len;    /* KW fix for LTE_ADV */ 
   CztProtIE_Field_CellInform_ItemIEs *protoie;

   TRC2(wrEmmNghEnbBldLoadInfo);

   for (cellIdx = 0; cellIdx < wrEmmCb.numCells;cellIdx++)
   {
      if(wrEmmCb.cellCb[cellIdx]->cellId == loadInfInd->cellId)
           break;
   }

   /* Allocate and Initialize memory control point */
   ret = cmAllocEvnt(sizeof(CztEvnt), WR_CZ_MEM_SDU_SIZE,
                            &wrCb.mem, (Ptr *)&x2LoadInfoPdu);
   if(ret != ROK)
   {
      RLOG0(L_FATAL, "Memory allocation failed.");
      RETVALUE(ROK);
   }
      /* Initialize the PDU */
   cmMemset((U8 *)&(x2LoadInfoPdu->pdu), 0, sizeof(CztX2AP_PDU));

   /* Fill the contents of Unsuccessful Event*/
   wrFillTknU8(&(x2LoadInfoPdu->pdu.choice),X2AP_PDU_INITIATINGMSG);
   wrFillTknU8(&(x2LoadInfoPdu->pdu.val.initiatingMsg.pres),PRSNT_NODEF);
   wrFillTknU32(&(x2LoadInfoPdu->pdu.val.initiatingMsg.procedureCode),
                                                           Cztid_loadInd);
   wrFillTknU32(&(x2LoadInfoPdu->pdu.val.initiatingMsg.criticality),
                                                CztCriticalityignoreEnum);
   wrFillTknU8(&(x2LoadInfoPdu->pdu.val.initiatingMsg.value.u.
                                       cztLoadInform.pres), PRSNT_NODEF);

   x2LoadInfo = &x2LoadInfoPdu->pdu.val.initiatingMsg.
                                    value.u.cztLoadInform;

   /* Filling 1 mandatory IEs  into the pdu */
   numComp = 1;
   wrFillTknU16(&(x2LoadInfo->protocolIEs.noComp), numComp);

   /* Get memory for numComp */
   if ((cmGetMem(x2LoadInfoPdu, (numComp *
                              sizeof(CztProtIE_Field_LoadInform_IEs)),
                              (Ptr*)&x2LoadInfo->protocolIEs.member)) != ROK)
   {
      RLOG0(L_FATAL, "Memory allocation failed.");
      /* Free the memory of Event Structure */
      WR_FREE_EVNT(x2LoadInfoPdu);
      RETVALUE(RFAILED);
   } /* end of if statement */

   /* IE1 - Filling  Cause IE*/
   ie = &x2LoadInfo->protocolIEs.member[ieIdx];
   wrFillTknU8(&(ie->pres), PRSNT_NODEF);
   wrFillTknU32(&(ie->id), Cztid_CellInform);
   wrFillTknU32(&(ie->criticality), CztCriticalityignoreEnum);
   
    
   numComp = 1;
   wrFillTknU16(&(ie->value.u.cztCellInform_Lst.noComp), numComp);
   
   if ((cmGetMem(x2LoadInfoPdu,(numComp * sizeof(CztProtIE_Field_CellInform_ItemIEs)),
              (Ptr *)&ie->value.u.cztCellInform_Lst.member)) !=ROK)
   {
      RLOG0(L_FATAL, "Memory allocation failed.");
      WR_FREE_EVNT(x2LoadInfoPdu);
      RETVALUE(RFAILED);
   }
  
   protoie = &ie->value.u.cztCellInform_Lst.member[ieIdx];
   
   wrFillTknU8(&(protoie->pres), PRSNT_NODEF);
   wrFillTknU32(&(protoie->id), Cztid_CellInform_Item);
   wrFillTknU32(&(protoie->criticality), CztCriticalityignoreEnum);
   wrFillTknU8(&(protoie->value.u.cztCellInform_Item.pres), PRSNT_NODEF);
   wrFillTknU8(&(protoie->value.u.cztCellInform_Item.cell_ID.pres), PRSNT_NODEF);
   /* Using the first configured cell and the first PLMN ID in the list */
   wrEmmLteAdvCztFillPLMNId(x2LoadInfoPdu,
           &(wrEmmCb.cellCb[cellIdx]->sib1.plmns[0].plmnId),
               &(protoie->value.u.cztCellInform_Item.cell_ID.pLMN_Identity));     
   len = 28;
   wrFillTknBStr32(&(protoie->value.u.cztCellInform_Item.cell_ID.eUTRANcellIdentifier),
	    		   len,wrEmmCb.cellCb[0]->sib1.cellIdentity);
   /*
   WR_GET_MEM(x2LoadInfoPdu, wrEmmCb.cellCb[cellIdx]->lteAdvancedConfig.rntpInfo.len, 
                 &(protoie->value.u.cztCellInform_Item.relativeNarrowbandTxPower.rNTP_PerPRB.val));           
   CZ_FILL_TKN_STR_OSXL(protoie->value.u.cztCellInform_Item.relativeNarrowbandTxPower.rNTP_PerPRB.val,
                        wrEmmCb.cellCb[cellIdx]->lteAdvancedConfig.rntpInfo.len,
                       wrEmmCb.cellCb[cellIdx]->lteAdvancedConfig.rntpInfo.val);
  */

  /* dsfr_pal_fixes ** 22-March-2013 ** SKS */
  protoie->value.u.cztCellInform_Item.relativeNarrowbandTxPower.pres.pres = PRSNT_NODEF;
  WR_GET_MEM(x2LoadInfoPdu, loadInfInd->u.rntpInfo.len, 
                 &(protoie->value.u.cztCellInform_Item.relativeNarrowbandTxPower.rNTP_PerPRB.val)); 
  protoie->value.u.cztCellInform_Item.relativeNarrowbandTxPower.rNTP_PerPRB.pres = PRSNT_NODEF;
  protoie->value.u.cztCellInform_Item.relativeNarrowbandTxPower.rNTP_PerPRB.len =
                                                                        loadInfInd->bw;
  cmMemcpy(protoie->value.u.cztCellInform_Item.relativeNarrowbandTxPower.rNTP_PerPRB.val,
           (CONSTANT U8*)loadInfInd->u.rntpInfo.val,
           (PTR)loadInfInd->u.rntpInfo.len);  
  wrFillTknU32(&(protoie->value.u.cztCellInform_Item.relativeNarrowbandTxPower.rNTP_Threshold), 
                  CztRNTP_ThresholdminusInfinityEnum );
  wrFillTknU32(&(protoie->value.u.cztCellInform_Item.relativeNarrowbandTxPower.numberOfCellSpecificAntennaPorts), 
                  CztRelativeNarrowbandTxPowernumberOfCellSpecificAntennaPortsoneEnum );
  wrFillTknU32(&(protoie->value.u.cztCellInform_Item.relativeNarrowbandTxPower.p_B), 0);
  wrFillTknU32(&(protoie->value.u.cztCellInform_Item.relativeNarrowbandTxPower.pDCCH_InterferenceImpact), 0);
  
  ieIdx++;

  *pdu = x2LoadInfoPdu;

  RETVALUE(ROK);
}

/** @brief This function builds and delivers the PDU
 *         to the X2AP layer through IFM module.
 *
 * @details
 *
 *     Function: wrEmmNghEnbSndLoadInfo
 *
 *         Processing steps:
 *         - Its an wrapper function to construct and
 *           deliver the Load Inform message to the
 *           peer eNodeB.
 *
 *
 * @param[in]   peerId : Peer ID
 * @return S16 
 *     -# Success  : ROK
 */
PRIVATE S16 wrEmmNghEnbSndLoadInfo 
(
WrNeighEnbCb                 *nghEnbCb,
RgrLoadInfIndInfo               *loadInfInd
)
{
   CztEvnt                   *x2LoadInfo = NULLP; 
   U16                       retVal = ROK;

   TRC2(wrEmmNghEnbSndLoadInfo);
   /*PRIVATE U32 tempCnt = 0;*/

   /* Build X2 Setup message */
   retVal = wrEmmNghEnbBldLoadInfo(&x2LoadInfo, loadInfInd);
   if(ROK != retVal)
   {
      RLOG0(L_ERROR, "Failed to build X2 Setup Req ");
      RETVALUE(retVal);
   }
   x2LoadInfo->peerId = nghEnbCb->peerId;
   /* Trigger corresponding primitive */
   if (ROK != WrIfmX2apGpReq(x2LoadInfo))
   {
     RLOG0(L_ERROR, " Failed to Send Load Inform ");     
     retVal = RFAILED;
   }
   RETVALUE(retVal);
} /* end of wrEmmNghEnbSndLoadInfo*/

/** @brief This function Triggers Load Info msg to its neighbors.
 *
 * @details
 *
 *     Function: wrEmmNghTrigLoadInfo
 *
 *         Processing steps:
 *         - Sends Load Info to each neighbor eNB   
 * 
 * @return S16 
 *     -# Success   : ROK
 *     -# Failure   : RFAILED
 */
PRIVATE S16 wrEmmNghTrigLoadInfo
(
RgrLoadInfIndInfo      *loadInfInd
)
{
   PTR                          prevEntry = NULLP;
   WrNeighEnbCb                 *nghEnbCb;
   S16                          retVal = ROK;

   /* Trigger Load Info to all neihgbor enodeb's */
   while ((retVal = cmHashListGetNext(&(wrEmmCb.neighEnbLstCp), prevEntry, (PTR *) &nghEnbCb)) == ROK)
   {
      if (nghEnbCb->x2Connection == WR_NEIGH_ENB_STATE_UP )
      {
         retVal = wrEmmNghEnbSndLoadInfo(nghEnbCb, loadInfInd);
         if ( retVal != ROK )
         {
            RLOG0(L_ERROR, " Sending Load Info failed ");
            RETVALUE(RFAILED);
         }
      }
      prevEntry = (PTR )nghEnbCb;
   }

   /* dsfr_pal_fixes ** 21-March-2013 ** SKS */
   WR_FREE(loadInfInd->u.rntpInfo.val,loadInfInd->u.rntpInfo.len);

   RETVALUE(ROK);
}

PRIVATE S16 wrEmmDsfrPrcLoadInfo 
(
U32                          peerId,
CztEvnt                      *pdu
)
{
   WrNeighEnbCb              *enbNghCb;
   CztInitiatingMsg          *intMsg = &pdu->pdu.val.initiatingMsg;
   CztLoadInform *loadInfo = &intMsg->value.u.cztLoadInform;
   S16                       ret = ROK;
   U32                       enbId = 0;
   U8                        cellIdx = 0; /* First cell */

   TRC2(wrEmmDsfrPrcLoadInfo)
   WR_GET_ENBID_FROM_PEERID(enbId,peerId);

   PRIVATE U32 tempCnt = 0;
   if(tempCnt++ == 10)
   {
      RLOG2(L_DEBUG, " Received Load Inform msg from PeerId [%lu] enbId [%lu] ",       
            peerId, enbId);
      tempCnt = 0;
   }
   /*Finding the neighbour eNodeB control block*/
   ret = cmHashListFind(&(wrEmmCb.neighEnbLstCp),(U8 *)&enbId,sizeof(enbId),0,
                   (PTR *)&enbNghCb);
   if (ret != ROK)
   {
      RLOG0(L_ERROR, " Hash List entry not found for enbNghCb ");
      RETVALUE(RFAILED);
   }

   /* Sound an alarm if CELL State not UP */
   /* Assumptios: Only one cell */
   if ( wrEmmCb.cellCb[cellIdx]->cellState != WR_CELL_STATE_UP )
   {
       RLOG0(L_INFO, "After receiving Load Inform msg, cell state is down");
       RETVALUE(RFAILED);
   }


   /*Update the Neighbouring ENB CB*/
   if(WR_NEIGH_ENB_STATE_UP == enbNghCb->x2Connection)
   {
      if(ROK != wrEmmNghEnbUpdNghCbFromLoadInfo(enbNghCb,loadInfo))
      {
         RLOG0(L_ERROR, " Failure in updating  Neighbouring eNB CB ");
         RETVALUE(RFAILED);
      }
   }
   else
   {
      RLOG0(L_ERROR, " X2 Connected state is NOT up ");
      RETVALUE(RFAILED);
   }

   RETVALUE(ROK);
}

/* LTE_ADV_DSFR ends */


/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/
/*%                       EMM DSFR MODULE - END                      %*/
/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/

/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/
/*%                   EMM LTE-ADAVANCED MODULE - START               %*/
/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/

/** @brief This function handles LTE-Advanced feature configuration 
 *         received from stack manager during initial startup
 *
 * @details 
 *
 *     Function: wrEmmHdlLteAdvCfg 
 *
 *         Processing steps:
 *
 * @param[in] cellId           : Id of cell   
 * @param[in] lteAdvancedTrans : Lte-Advanced configuration from stack maanager
 * @return S16
 *     -# Success : ROK
 *     -# Failure : RFAILED
 */
PUBLIC S16 wrEmmHdlLteAdvCfg
(
U32                        cellId,
LwrLteAdvancedConfigTrans  *lteAdvancedTrans
)
{
   S16                     ret = ROK;
   WrCellCb                *cellCb;
/* LTE_ADV_ABS starts */
   WrEmmAbsEvent           event;
/* LTE_ADV_ABS ends */

   TRC1(wrEmmHdlLteAdvCfg)

   WR_GET_CELLCB(cellCb, cellId);
   if(cellCb == NULLP)            /* KW fix for LTE_ADV */
   {    
       RLOG1(L_ERROR, " CELLCB not found for given cellId(%d) ", (U16)cellId);
       RETVALUE(RFAILED);        /* KW fix for LTE_ADV */   
   }                         
   RLOG0(L_INFO, " Request lte adv feature configuration rcvd ");
   cmMemcpy((U8 *) &cellCb->lteAdvCb.lteAdvCfg, (U8 *)&lteAdvancedTrans->lteAdvancedConfig,
                                                          sizeof(LwrLteAdvancedConfig));

/* LTE_ADV_ABS starts */
   wrEmmCb.lteAdvCb.enbType = cellCb->lteAdvCb.lteAdvCfg.enbType;
   wrEmmCb.lteAdvCb.absInfo.negType = 
            (lteAdvancedTrans->lteAdvancedConfig.absCfg.absLoadPeriodicity == 0)? 
                        WR_EMM_ABS_STATIC_NEG_TYPE: WR_EMM_ABS_DYNAMIC_NEG_TYPE;

   /* Send Cell configuration event to abs emm negotiation module */
   event.eventId = WR_EMM_ABS_CELL_INIT_EVENT;
   event.u.cellEvent.cellId = cellId;
   ret = wrEmmAbsProcessEvent(&event);
/* LTE_ADV_ABS ends */

   RETVALUE(ret);
}

/** @brief This function calls wrUmmHdlLteAdvReCfg function with feature and its status.
 *         and update the Lte Advanced configuration
 *
 * @details
 *
 *     Function: wrEmmHdlLteAdvReCfg
 *
 *         Processing steps:
 *         - 
 * @param [out] cellCb        : cell control block. 
 * @param [in]  lteAdvanced   : Lte Advanced configuration parameters.
 * @return S16
 *    -# Success : ROK
 *    -# Failure : RFAILED
 */
PUBLIC S16 wrEmmHdlLteAdvReCfg
(
CmLteCellId                cellId,
LwrLteAdvancedConfigTrans  *lteAdvancedTrans,
Bool *trgSib4   /*RE_CHANGES*/
)               /* KW fix for LTE_ADV */
{
   WrCellCb                *cellCb;
/* LTE_ADV_ABS starts */
   WrEmmAbsEvent           event;
/* LTE_ADV_ABS ends */

   TRC1(wrEmmHdlLteAdvReCfg)

   WR_GET_CELLCB(cellCb, cellId);
   if(cellCb == NULLP)            /* KW fix for LTE_ADV */
   {    
       RLOG1(L_ERROR, " CELLCB not found for given cellId(%d) ", (U16)cellId);
       RETVALUE(RFAILED);        /* KW fix for LTE_ADV */   
   }                         
   RLOG0(L_INFO, " Request lte adv feature modification rcvd  ");

/* LTE_ADV_ABS starts */
   /* Handle cell/enb specific configuration */
   if(lteAdvancedTrans->pres & RGR_ABS)
   { 
      event.eventId = (RGR_ENABLE == lteAdvancedTrans->lteAdvancedConfig.absCfg.status)? 
                       WR_EMM_ABS_ENABLED_EVENT: WR_EMM_ABS_DISABLED_EVENT;
      event.u.absCfgEvent.cellId = cellId;
      event.u.absCfgEvent.lteAdvTrans = lteAdvancedTrans;
      wrEmmAbsProcessEvent(&event);
      
      /* Store in lte adv configuration */
      cellCb->lteAdvCb.lteAdvCfg.absCfg.status = 
                               lteAdvancedTrans->lteAdvancedConfig.absCfg.status;
   }
/* LTE_ADV_ABS ends */

   if((lteAdvancedTrans->pres & RGR_SFR) || (lteAdvancedTrans->pres & RGR_DSFR))
   {
      wrEmmCellSchdReCfgLteAdvFeatures(cellId, lteAdvancedTrans);
   }

   /* Handle UE specific configuration */
   if(lteAdvancedTrans->pres & RGR_RE)
   {
      /*RE_CHANGES : modify SIB4*/
      *trgSib4 = TRUE;
      wrEmmLteAdvUpdCellSpfcOffset(cellId, RGR_RE, lteAdvancedTrans->lteAdvancedConfig.reCfg.status);
      wrUmmHdlLteAdvRecfg(cellId, RGR_RE, lteAdvancedTrans->lteAdvancedConfig.reCfg.status);
      cellCb->lteAdvCb.lteAdvCfg.reCfg.status = lteAdvancedTrans->lteAdvancedConfig.reCfg.status;
   }

   if(lteAdvancedTrans->pres & RGR_SFR)
   {
      wrUmmHdlLteAdvRecfg(cellId, RGR_SFR, lteAdvancedTrans->lteAdvancedConfig.sfrCfg.status);
      cellCb->lteAdvCb.lteAdvCfg.sfrCfg = lteAdvancedTrans->lteAdvancedConfig.sfrCfg; 
   }

   if(lteAdvancedTrans->pres & RGR_DSFR)
   {
      cellCb->lteAdvCb.lteAdvCfg.dsfrCfg.status = lteAdvancedTrans->lteAdvancedConfig.dsfrCfg.status;
   }

   RETVALUE(ROK);
} /* wrEmmHdlLteAdvReCfg */

/** @brief This function applies cell specific offset 
 *
 * @details 
 *
 *     Function: wrEmmLteAdvUpdCellSpfcOffset
 *
 *         Processing steps:
 *
 * @param[in] cellId: Id of cell 
 * @param[in] feature: RGR feature
 * @param[in] status: Status of feature
 * @return S16
 *     -# Success : ROK
 *     -# Failure : RFAILED
 */
PUBLIC S16 wrEmmLteAdvUpdCellSpfcOffset
(  
U32                          cellId,
RgrFeature                   feature,
RgrFeatureStatus             status
)     
{        
   WrCellCb                  *cellCb;
   WrUmmMeasCellCb           *measCellCb;
   CmLListCp                 *wlCellLst;
   CmLList                   *nrWCellLnk;
   WrMeasNrWCellNode         *nrWCellNode;
   WrUmmMeasEutraMeasObjNode *intraMeasObj;
   WrUmmCellCb               *ummCellCb;
   U16                       pci;
   U32                       cio;
   
   TRC2(wrEmmLteAdvUpdCellSpfcOffset)

   WR_GET_CELLCB(cellCb, cellId);
   if(cellCb == NULLP)            /* KW fix for LTE_ADV */
   {    
       RLOG1(L_ERROR, " CELLCB not found for given cellId(%d) ", (U16)cellId);
       RETVALUE(RFAILED);        /* KW fix for LTE_ADV */   
   }                         
   WR_UMM_GET_CELLCB(ummCellCb, cellId)
   if(ummCellCb == NULLP)            /* KW fix for LTE_ADV */
   {    
       RLOG1(L_ERROR, " CELLCB not found for given cellId(%d) ", (U16)cellId);
       RETVALUE(RFAILED);        /* KW fix for LTE_ADV */   
   }                         
   measCellCb   = ummCellCb->measCellCb;
   intraMeasObj = &measCellCb->intraMeasObj;
   wlCellLst    = &intraMeasObj->wlCellLst;
   pci          = wrEmmGetPhyCellId(cellCb->cellId);

   if(RGR_ENABLE == status)
   {
      cio = cellCb->lteAdvCb.lteAdvCfg.reCfg.neighCIO;
   }
   else
   {
      cio = 15; /* 0 db */
   }

   /* Change the offset of neighbour cell */
   nrWCellLnk   = wlCellLst->first;
   while (NULLP != nrWCellLnk)
   {
      nrWCellNode = (WrMeasNrWCellNode*)nrWCellLnk->node;
      if (pci != nrWCellNode->pci)
      {
         nrWCellNode->cio = cio;
      }
      nrWCellLnk = nrWCellLnk->next;
   }

   RETVALUE(ROK);
}

/** @brief This function is used to Process LOAD INF Indication received from MAC-SCH
 *
 * @details
 *
 *     Function: wrEmmSchdLoadInfInd
 *
 *     Processing steps:
 *     1)
 *
 * @param[in] loadInfInd : Pointer to RgrLoadInfIndInfo structure.
 * @return S16
 *    -#Success :
 *    -#Failure :
 */
PUBLIC S16 wrEmmSchdLoadInfInd
(
RgrLoadInfIndInfo                *loadInfInd
)
{
   S16                           ret = RFAILED;
/* LTE_ADV_ABS starts */
   WrEmmAbsEvent                 event;
/* LTE_ADV_ABS ends */

   TRC2(wrEmmSchdLoadInfInd)

/* LTE_ADV_ABS starts */
   if(RGR_ABS == loadInfInd->type)
   {
      event.eventId = WR_EMM_ABS_SCH_LOAD_IND_EVENT;
      event.u.schLoadInfoEvent.schLoadInfo = loadInfInd;
      wrEmmAbsProcessEvent(&event);
   }
/* LTE_ADV_ABS ends */
/* LTE_ADV_DSFR starts */
   if(RGR_SFR == loadInfInd->type)
   {
      ret = wrEmmNghTrigLoadInfo(loadInfInd);
      if(ret != ROK)
      {
         RLOG0(L_ERROR, " wrEmmNghTrigLoadInfo Failed");
         RETVALUE(RFAILED);
      }
   }
/* LTE_ADV_DSFR ends */
   WR_FREE(loadInfInd, sizeof(RgrLoadInfIndInfo));
   RETVALUE(ROK);
}

/** @brief This function is used to Process LOAD INF Indication received from X2AP
 *
 * @details
 *
 *     Function: wrEmmLteAdvPrcLoadInfo
 *
 *     Processing steps:
 *         1. Invoke ABS Module
 *         2. Invoke DSFR Module
 *
 * @param[in] peerId : Id of peer eNodeB
 * @param[in] pdu : Pointer to X2AP Pdu
 * @return S16
 *    -#Success :
 *    -#Failure :
 */
PUBLIC S16 wrEmmLteAdvPrcLoadInfo
(
U32                          peerId,
CztEvnt                      *pdu
)
{
/* LTE_ADV_ABS starts */
   WrEmmAbsEvent             event;
/* LTE_ADV_ABS ends */
   WrCellCb                  *cellCb;
   TRC2(wrEmmLteAdvPrcLoadInfo)

/* LTE_ADV_ABS starts */
   /* Pass Load Info to ABS Module */
   event.eventId             = WR_EMM_ABS_LOAD_INFO_RCVD_EVENT;
   event.u.x2apEvent.peerId  = peerId;
   event.u.x2apEvent.pdu     = pdu;
   wrEmmAbsProcessEvent(&event);
/* LTE_ADV_ABS ends */

   /* Pass Load Info to DSFR Module */
/* LTE_ADV_DSFR starts */
   WR_GET_ABS_CELLCB(cellCb);
   if((cellCb == NULLP))
   {
       RLOG0(L_ERROR, "  CELLCB not found ");
       RETVALUE(RFAILED);   
   }  /*KW fix for LTE_ADV */      
   if(RGR_ENABLE == cellCb->lteAdvCb.lteAdvCfg.dsfrCfg.status)
   {
      wrEmmDsfrPrcLoadInfo(peerId, pdu);
   }
/* LTE_ADV_DSFR ends */

   RETVALUE(ROK); 
}

PUBLIC S16 wrEmmLteAdvPrcTmrExpiry
(
PTR                       cb,
U32                       eventId
)
{
/* LTE_ADV_ABS starts */
   WrEmmAbsEvent             event;
/* LTE_ADV_ABS ends */

   TRC2(wrEmmLteAdvPrcTmrExpiry)
/* LTE_ADV_ABS starts */
   event.eventId             = WR_EMM_ABS_TIMER_EVENT;
   event.u.timerEvent.event  = eventId;
   event.u.timerEvent.cb     = cb;
   wrEmmAbsProcessEvent(&event);
/* LTE_ADV_ABS ends */
   RETVALUE(ROK);  
}

/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/
/*%                   EMM LTE-ADAVANCED MODULE - END                 %*/
/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/

/* LTE_ADV ends */
