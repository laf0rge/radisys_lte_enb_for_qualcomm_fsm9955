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
               

     File:     wr_umm_s1srcho.c

     Sid:      $SID$ 

     Prg:      Sriky 

*********************************************************************21*/
/* header include files (.h) */
static const char* RLOG_MODULE_NAME="UMM";
static int RLOG_FILE_ID=71;


static int RLOG_MODULE_ID=32;

#include "wr.h"
#include "wr_umm.h"
#include "wr_umm_trans.h"
#include "wr_utils.h"
#include "wr_umm_l1_msg.h"
#include "wr_umm_sch_msg.h"
#include "wr_ifm_dam.h"
#include "wr_ifm_s1ap.h"
#include "wr_umm_s1srcho.h"
#include "wr_ifm_rrc.h"
#include "wr_umm_ho.h"
#include  "wr_emm.h"
#include  "wr_emm_mme.h"
#include "wr_ifm_dam.h"
#include "wr_dam.h"
#include "wr_kpi.h"
#include "wr_umm_rrc_msg.h"
#include "wr_ifm_schd.h"
#include "wr_umm_drx.h"
#include "wr_umm_s1ap_msg.h"



EXTERN S16 wrS1apGetTnlAddr(CmTptAddr  *addr,SztTportLyrAddr  *ie);
EXTERN S16 wrEmmMmeSztFillTAC(U16 tac,S1apPdu  *pdu,SztTAC  *tAC);
EXTERN S16 wrUmmS1SrcSndHovrCancel(WrUmmTransCb    *transCb);
/*UE history info - Start*/
EXTERN CmPAsnElmDef *szMsgIEDb[][4];
EXTERN S16 wrEmmGetCellSizeType
(
WrCellId                     cellId,
U8                           *cellSizeType
);
PRIVATE S16 wrUmmS1SrcHoPrcIRatHoTgtToSrcDecCfm 
(
 WrUmmTransCb                *transCb
);
/*UE history info - End*/

/** @brief This function is called to store the UTRA UE capability
 *             info into the transactio control block.
 *
 * @details
 *
 *     Function: wrUmmS1HoUeCapStoreUtraCapInfo
 *
 *         Processing steps:
 *           - allocate and store the UTRA capability in transCb 
 *           - Return failure if UTRA capability is missing
 *
 * @param[in, out] transCb   : transCb information 
 * @param[in]     ueCapInfo : UE Capability Information
 * @return S16
 *    -#Success : ROK
 */
PRIVATE S16 wrUmmS1HoUeCapStoreUtraCapInfo 
(
WrUmmTransCb                 *transCb,
NhuUECapInfo                 *ueCapInfo
)
{
   NhuUE_CapRAT_ContLst     *ueContLst;
   WrUmmIRatUtraInfo        *iRatUtraHoTransInfo =
                                    &(transCb->u.s1SrcHoTrans.t.interRatInfo);

   ueContLst = &ueCapInfo->criticalExtns.val.c1.val.ueCapInfo_r8.ue_CapRAT_ContLst;
   if ( ( ueContLst->noComp.val == 1 )  &&
        ( ueContLst->member[0].rat_Typ.val == NhuRAT_TyputraEnum ))
   {
      WR_ALLOC(&(iRatUtraHoTransInfo->utraUeCapContainer.val), 
            ueContLst->member[0].ueCapRAT_Cont.len);
      if ( NULLP == iRatUtraHoTransInfo->utraUeCapContainer.val )
      {
         RLOG0(L_FATAL,"Memory allocation failed.");
         RETVALUE(RFAILED);
      }
      cmMemcpy(iRatUtraHoTransInfo->utraUeCapContainer.val,
            ueContLst->member[0].ueCapRAT_Cont.val,
            ueContLst->member[0].ueCapRAT_Cont.len);
      iRatUtraHoTransInfo->utraUeCapContainer.pres = TRUE;
      iRatUtraHoTransInfo->utraUeCapContainer.len =
         ueContLst->member[0].ueCapRAT_Cont.len;
   }
   else
   {
     RLOG0(L_ERROR,"UeCapInfo doesnt contain UTRA cap" 
                   "Or contains more than requested RAT");
     RETVALUE(RFAILED);
   }
   RETVALUE(ROK);
}
PUBLIC S16 wrUmmS1SrcHoFillPLMNId
(
S1apPdu                      *pdu,
WrPlmnId                     *plmnId,
SztPLMNidentity              *plmnIe
)
{
   U8      pLMNId[3];

   WR_GET_MEM(pdu, WR_PLMNID_IE_LEN, &(plmnIe->val));
   plmnIe->pres = PRSNT_NODEF;
   plmnIe->len  = WR_PLMNID_IE_LEN;

   pLMNId[0] =(((plmnId->mcc[1])<<4) | (plmnId->mcc[0]));
   if (plmnId->numMncDigits == 2)
   {
      pLMNId[1] =((0xf0) | (plmnId->mcc[2]));
      pLMNId[2] =(((plmnId->mnc[1])<<4) | (plmnId->mnc[0]));
   }
   else
   {
      pLMNId[1] =(((plmnId->mnc[0])<<4) | (plmnId->mcc[2]));
      pLMNId[2] =(((plmnId->mnc[2])<<4) | (plmnId->mnc[1]));
   }
   wrFillTknStrOSXL(plmnIe, WR_PLMNID_IE_LEN, pLMNId, &pdu->memCp);
   RETVALUE(ROK);
}

PRIVATE S16 wrUmmS1SrcHoFillMsClassMark2 
(
WrUmmTransCb                 *transCb,
SztMSClassmark2              *sztMSClassmark2,
S1apPdu                      *hovrReqdPdu
)
{
   U8 idx;
   if(transCb->ueCb->ueCap->ueGeranCsOctStr.val == NULLP)
      RETVALUE(ROK);
   /* First 5 bytes in the ueGeranCsOctStr container is Class Mark 2 
    * which is of 5 bytes */
   sztMSClassmark2->pres = PRSNT_NODEF;

   sztMSClassmark2->len  = 5;

   WR_GET_MEM((&hovrReqdPdu->memCp), sztMSClassmark2->len, 
               &(sztMSClassmark2->val));
   for(idx = 0; idx < sztMSClassmark2->len; idx++)
   {
      sztMSClassmark2->val[idx] = 
                transCb->ueCb->ueCap->ueGeranCsOctStr.val[idx];
   }

   RETVALUE(ROK);
}/* End of wrUmmS1SrcHoFillMsClassMark2 */

PRIVATE S16 wrUmmS1SrcHoFillMsClassMark3
(
WrUmmTransCb                 *transCb,
SztMSClassmark3              *sztMSClassmark3,
S1apPdu                      *hovrReqdPdu
)
{
   U8 idx;
   sztMSClassmark3->pres = PRSNT_NODEF;

   /* First 5 bytes in ueGeranCsOctStr contains ClassMark2 IE info  */
   sztMSClassmark3->len  = transCb->ueCb->ueCap->ueGeranCsOctStr.len - 5;

   /* Remaining is the value of ClassMark3 */
   WR_GET_MEM((&hovrReqdPdu->memCp), sztMSClassmark3->len, 
               &(sztMSClassmark3->val));
   for(idx = 0; idx < sztMSClassmark3->len; idx++)
   {
      sztMSClassmark3->val[idx] = 
                transCb->ueCb->ueCap->ueGeranCsOctStr.val[idx + 5];
   }
   RETVALUE(ROK);
}/* End of wrUmmS1SrcHoFillMsClassMark3 */

PRIVATE S16 wrUmmS1SrcHoFillTgetId
(
WrUmmTransCb                 *transCb,
SztTgetID                    *sztTgetID,
S1apPdu                      *hovrReqdPdu
)
{
   SztTgeteNB_ID             *ie1 = NULLP;
   SztENB_ID                 *eNB_ID = NULLP;
   U8                        enbIdLen = 0;   /* KW fix for LTE_ADV */
   U32                       enbId;
   U32                       enbType;
   WrPlmnId                  *plmnId;
   U16                       tac;
   WrUmmS1SrcHoTrans         *s1SrcHoTrans = &transCb->u.s1SrcHoTrans;
   WrUmmIRatUtraInfo         *iRatUtraHoTransInfo =
                                        &(s1SrcHoTrans->t.interRatInfo);
   WrUmmIRatGeranInfo        *iRatGeranTransInfo =
                                        &(s1SrcHoTrans->t.interRatGeranInfo);
   SztTgetRNC_ID             *targetRncId;
   SztCGI                    *cGI = NULLP;

   plmnId = &s1SrcHoTrans->plmnId;
   enbId = s1SrcHoTrans->t.intraLteInfo.ecgi.eutranCellId;
   tac = s1SrcHoTrans->t.intraLteInfo.tac;
   enbType = s1SrcHoTrans->t.intraLteInfo.eNBType;

   /* TODO Assume we are getting handover type from Transaction controlblock.*/
   switch(s1SrcHoTrans->sztHovrTyp->val)
   {
      case SztHovrTypintralteEnum:
      {
         wrFillTknU8(&(sztTgetID->choice), TGETID_TARGETENB_ID);
         ie1 = &sztTgetID->val.targeteNB_ID;
         wrFillTknU8(&(ie1->pres), PRSNT_NODEF);
         wrFillTknU8(&(ie1->global_ENB_ID.pres), PRSNT_NODEF);

            wrUmmS1SrcHoFillPLMNId(hovrReqdPdu,plmnId, &ie1->global_ENB_ID.pLMNidentity); 
         /* Get the eNB type from transaction Control block.*/
         wrFillTknU8(&(ie1->global_ENB_ID.eNB_ID.choice), enbType);
         switch(ie1->global_ENB_ID.eNB_ID.choice.val)
         {
            case ENB_ID_MACROENB_ID:
            {
               enbIdLen = 20;
               eNB_ID = &ie1->global_ENB_ID.eNB_ID;
               enbId = enbId >> 8;
               //enbId = enbId & 0x000fffff;
               wrFillTknBStr32(&(eNB_ID->val.macroENB_ID), enbIdLen, enbId);
               break;
            }
            case ENB_ID_HOMEENB_ID:
            {
               enbIdLen = 28;
               eNB_ID = &ie1->global_ENB_ID.eNB_ID;
               enbId = enbId & 0x0fffffff;
               wrFillTknBStr32(&(eNB_ID->val.homeENB_ID), enbIdLen, enbId);
               break;
            }
            default:
            {
               RLOG0(L_ERROR, "Invalid eNB Type");
               RETVALUE(RFAILED);              
            }            
         }
         ie1->global_ENB_ID.iE_Extns.noComp.pres = NOTPRSNT;
         /* Fill the Tracking Area Identity */
         wrFillTknU8(&(ie1->selected_TAI.pres), PRSNT_NODEF);
         wrUmmS1SrcHoFillPLMNId(hovrReqdPdu,plmnId, &ie1->selected_TAI.pLMNidentity);
         wrEmmMmeSztFillTAC(tac, NULLP, &(ie1->selected_TAI.tAC));
         ie1->selected_TAI.iE_Extns.noComp.pres = NOTPRSNT;
         break;
      }
      case SztHovrTypltetoutranEnum:
         {         
            wrFillTknU8(&(sztTgetID->choice), TGETID_TARGETRNC_ID);

            targetRncId = &(sztTgetID->val.targetRNC_ID);

            wrFillTknU8(&(targetRncId->pres), PRSNT_NODEF);
            /* fill LAI - plmnId and LAC  */
            wrFillTknU8(&(targetRncId->lAI.pres), PRSNT_NODEF);
            wrUmmS1SrcHoFillPLMNId(hovrReqdPdu, plmnId, &(targetRncId->lAI.pLMNidentity)); 
            wrEmmMmeSztFillLAC(iRatUtraHoTransInfo->lac, &(targetRncId->lAI.lAC));

            /* Fill RAC */
            targetRncId->rAC.pres   = PRSNT_NODEF;
            targetRncId->rAC.len    = WR_RAC_IE_LEN;
            targetRncId->rAC.val[0] = iRatUtraHoTransInfo->rac;

            /* Fill RNC ID : As per 36.413 - If the Extended RNC-ID IE is 
               included in the Target ID IE, then RNC-ID IE shall be ignored */
            wrFillTknU32(&(targetRncId->rNC_ID), iRatUtraHoTransInfo->rncId);
            /* As per spec 36.413 - The Extended RNC-ID IE shall be used if the 
               RNC identity has a value larger than 4095  */
            if ( iRatUtraHoTransInfo->rncId >= WR_EXT_RNC_START )
            {
               wrFillTknU32(&(targetRncId->extendedRNC_ID), iRatUtraHoTransInfo->rncId);
            }
            break;
         }
      case SztHovrTypltetogeranEnum:
         {
            wrFillTknU8(&(sztTgetID->choice), TGETID_CGI);

            cGI = &(sztTgetID->val.cGI);

            wrFillTknU8(&(cGI->pres), PRSNT_NODEF);
            
            /* fill pLMNidentity */
            wrUmmS1SrcHoFillPLMNId(hovrReqdPdu, plmnId, &(cGI->pLMNidentity)); 
            
            /* fill LAC  */
            wrEmmMmeSztFillLAC(iRatGeranTransInfo->lac, &(cGI->lAC));
            
            /* Fill the GERAN Cell identity */
            wrEmmMmeSztFillCI(iRatGeranTransInfo->cId, &(cGI->cI));

            /* Fill RAC */
            cGI->rAC.pres   = PRSNT_NODEF;
            cGI->rAC.len    = WR_RAC_IE_LEN;
            cGI->rAC.val[0] = iRatGeranTransInfo->rac;
            break;
         }
      case SztHovrTyputrantolteEnum:
         {
            break;
         }
      case SztHovrTypgerantolteEnum:
         {
            break;
         }
      default:
         {
            RLOG0(L_ERROR, "Invalid HandOver Type");
            RETVALUE(RFAILED);       
         }
   }/* End of Switch(ie->value.u.sztHovrTyp)*/
   RETVALUE(ROK);
}/* End of wrUmmS1SrcHoFillTgetId*/

PUBLIC S16 wrUmmS1HoFillCause 
(
SztCause                     *transCause,
SztCause                     *cause
)
{
   wrFillTknU8(&(transCause->choice), cause->choice.val);
   switch(transCause->choice.val)
   {
      case WR_CAUSE_RADIONW:
         {
            wrFillTknU32(&(transCause->val.radioNw), cause->val.radioNw.val);
            break;
         }
      case WR_CAUSE_TRANSPORT:
         {
            wrFillTknU32(&(transCause->val.transport),cause->val.transport.val);
            break;
         }
      case WR_CAUSE_NAS:
         {
            wrFillTknU32(&(transCause->val.nas),cause->val.nas.val);
            break;
         }
      case WR_CAUSE_PROTOCOL:
         {
            wrFillTknU32(&(transCause->val.protocol),cause->val.protocol.val);
            break;
         }
      case WR_CAUSE_MISC: 
         {
            wrFillTknU32(&(transCause->val.misc),cause->val.misc.val);
            break;
         }
      default:
         {
            wrFillTknU32(&(transCause->val.radioNw),cause->val.radioNw.val);
            break;  
         }
   }/*End of switch (s1Con->cause.choice)*/
   RETVALUE(ROK);
}/* End of wrUmmS1HoFillCause*/

/* This function is used to find optional parameters to be included in       */
/* Handover Required Message based on their availability.                    */
PRIVATE S16 wrUmmS1SrcHoFndOptionalParam
(
WrUmmTransCb                 *transCb,
U16                          *numComp
)
{
   /* Direct Forwarding Path Availability indicates whether direct forwarding*/
   /* is available from the source eNB to target eNB. The eNB shall include  */
   /* Direct Forwarding Path Avilability IE based on presence of X2-U        */
   if (TRUE == transCb->u.s1SrcHoTrans.t.intraLteInfo.isDirectPathAvl )
   {
      (*numComp)++;
   }
   /* The Target cell may be either CSG Cell or Hybrid Cell.If the cell is   */
   /* CSG,the eNB shall include CSG ID of the cell.                          */
   if (WR_ENB_CLOSED_ACCESS == transCb->u.s1SrcHoTrans.csgCellAccessMode)
   {
      (*numComp)++;
   }
   /* If the target cell is Hybrid Cell, the source eNB shall include Cell   */
   /* Access Mode IE is TRUE.                                                */
   else if (WR_ENB_HYBRID_ACCESS == transCb->u.s1SrcHoTrans.csgCellAccessMode)
   {
      (*numComp)+=2;
   }

   RETVALUE(ROK);
}/* End of wrUmmS1SrcHoFndOptParam*/

PRIVATE S16 wrUmmS1SrcHoFillHovrCancel
(
WrUmmTransCb                 *transCb,
S1apPdu                      **pdu
)
{
   S1apPdu                        *hovrCancelPdu = NULLP;
   SztS1AP_PDU                    *s1apPdu = NULLP;
   SztInitiatingMsg               *initiatingMsg = NULLP;
   SztHovrCancel                  *sztHovrCancel = NULLP;
   SztProtIE_Field_HovrCancelIEs  *ie = NULLP;
   U16                            numComp;
   U16                            compCnt = 0;
   U32                            mme_Ue_S1ap_Id;
   U32                            enb_Ue_S1ap_Id;
   WrUeCb                         *ueCb = transCb->ueCb;

   /* Memory Allocation for S1ap PDU.*/
   if (ROK != cmAllocEvnt(sizeof(S1apPdu), WR_SZ_MEM_SDU_SIZE,  
            &wrCb.mem, (Ptr *)&hovrCancelPdu))
   {
      RLOG0(L_FATAL, "Memory allocation failed.");
      RETVALUE(RFAILED);
   }/* End of if condition.*/

   s1apPdu = &hovrCancelPdu->pdu;
   wrFillTknU8(&(s1apPdu->choice),S1AP_PDU_INITIATINGMSG);
   initiatingMsg = &s1apPdu->val.initiatingMsg; 
   wrFillTknU8(&(initiatingMsg->pres), PRSNT_NODEF);
   wrFillTknU32(&(initiatingMsg->procedureCode), Sztid_HovrCancel);
   wrFillTknU32(&(initiatingMsg->criticality), SztCriticalityrejectEnum);
   /* Indicates No of Mandatory IEs.*/
   numComp = 3;
   
   sztHovrCancel = &initiatingMsg->value.u.sztHovrCancel;
   /* Fill the Protocol IEs of Handover Required Message. */
   wrFillTknU8(&(sztHovrCancel->pres),PRSNT_NODEF);
   wrFillTknU16(&(sztHovrCancel->protocolIEs.noComp), numComp);
   /* Allocate the memory for Protocol IEs.*/
   if ((cmGetMem(&hovrCancelPdu->memCp, 
               (numComp * sizeof(SztProtIE_Field_HovrCancelIEs)),
               ((Ptr *)&sztHovrCancel->protocolIEs.member))) !=ROK)
   {
      RLOG0(L_FATAL, "Memory allocation failed.");
      RETVALUE(RFAILED);
   }
   
   ie = &sztHovrCancel->protocolIEs.member[compCnt++];
   wrFillTknU8(&(ie->pres), PRSNT_NODEF);
   wrFillTknU32(&(ie->id), Sztid_MME_UE_S1AP_ID);
   wrFillTknU32(&(ie->criticality), SztCriticalityrejectEnum);
   mme_Ue_S1ap_Id = ueCb->s1ConCb->mme_ue_s1ap_id;
   wrFillTknU32(&(ie->value.u.sztMME_UE_S1AP_ID), mme_Ue_S1ap_Id);
   
   ie = &sztHovrCancel->protocolIEs.member[compCnt++];
   wrFillTknU8(&(ie->pres), PRSNT_NODEF);
   wrFillTknU32(&(ie->id), Sztid_eNB_UE_S1AP_ID);
   wrFillTknU32(&(ie->criticality), SztCriticalityrejectEnum);
   enb_Ue_S1ap_Id = ueCb->s1ConCb->enb_ue_s1ap_id;
   wrFillTknU32(&(ie->value.u.sztENB_UE_S1AP_ID), enb_Ue_S1ap_Id);

   ie = &sztHovrCancel->protocolIEs.member[compCnt++];
   wrFillTknU8(&(ie->pres), PRSNT_NODEF);
   wrFillTknU32(&(ie->id), Sztid_Cause);
   wrFillTknU32(&(ie->criticality), SztCriticalityignoreEnum);
   if (ROK != wrUmmS1HoFillCause(&ie->value.u.sztCause, 
            transCb->u.s1SrcHoTrans.sztCause))
   {
      RLOG0(L_ERROR," Failed To Fill Cause Value.");
      RETVALUE(RFAILED);
   }
   /* ccpu00130451 */
   WR_INC_KPI_FUNC3(wrIncKpiFrS1apFailCause, WR_CAUSE_RADIONW, 
         SztCauseRadioNwhandover_cancelledEnum, INC_KPI_VALUE_BY_ONE);     
   *pdu = hovrCancelPdu;

   RETVALUE(ROK);
}

PRIVATE S16 wrUmmS1SrcHoFillHovrReqrdMsg
(
WrUmmTransCb                 *transCb,
S1apPdu                      **pdu
)
{
   S1apPdu                        *hovrReqrdPdu = NULLP;
   SztS1AP_PDU                    *s1apPdu = NULLP;
   SztInitiatingMsg               *initiatingMsg = NULLP;
   SztHovrReqd                    *sztHovrReqd = NULLP; 
   SztProtIE_Field_HovrReqdIEs    *ie = NULLP;
   U32                            mme_Ue_S1ap_Id;
   U32                            enb_Ue_S1ap_Id;
   U16                            numComp;
   U16                            compCnt = 0;
   MsgLen                         contLen;
   SztEncDecEvnt                  *sztEncCfm = transCb->msg->u.sztEncCfm;
   U16                            len;  
 WrUmmS1SrcHoTrans              *s1SrcHoTrans = &transCb->u.s1SrcHoTrans;
#ifdef WR_RSYS_KPI
   WrUeCb                         *ueCb = transCb->ueCb;
#endif
   /* Any cell is representative of eNB for some of its parameters        */
   /* Memory Allocation for S1ap PDU.*/
   if (ROK != cmAllocEvnt(sizeof(S1apPdu), WR_SZ_MEM_SDU_SIZE,  
            &wrCb.mem, (Ptr *)&hovrReqrdPdu))
   {
      RLOG0(L_FATAL, "Memory allocation failed.");
      RETVALUE(RFAILED);
   }/* End of if condition.*/

   s1apPdu = &hovrReqrdPdu->pdu;
   wrFillTknU8(&(s1apPdu->choice),S1AP_PDU_INITIATINGMSG);
   initiatingMsg = &s1apPdu->val.initiatingMsg; 
   wrFillTknU8(&(initiatingMsg->pres), PRSNT_NODEF);
   wrFillTknU32(&(initiatingMsg->procedureCode), Sztid_HovrPrep);
   wrFillTknU32(&(initiatingMsg->criticality), SztCriticalityrejectEnum);
   /* Indicates No of Mandatory IEs.*/
   numComp = 6;
   /* To find no of IEs will be needed. */
   if (ROK != wrUmmS1SrcHoFndOptionalParam(transCb, &numComp))
   {
      RLOG0(L_ERROR, "Failed to find optional parameters");
      RETVALUE(RFAILED);
   }
   sztHovrReqd = &initiatingMsg->value.u.sztHovrReqd;
   /* Fill the Protocol IEs of Handover Required Message. */
   wrFillTknU8(&(sztHovrReqd->pres),PRSNT_NODEF);
   wrFillTknU16(&(sztHovrReqd->protocolIEs.noComp), numComp);
   /* Allocate the memory for Protocol IEs.*/
   if ((cmGetMem(&hovrReqrdPdu->memCp, 
               (numComp * sizeof(SztProtIE_Field_HovrReqdIEs)),
               ((Ptr *)&sztHovrReqd->protocolIEs.member))) !=ROK)
   {
      RLOG0(L_FATAL, "Memory allocation failed.");
      RETVALUE(RFAILED);
   }
   /* 
    * Fill MME UE S1AP ID IE which is generated by MME.
    * MME UE S1AP Id uniquly identify the UE over S1 interface within MME.
    */
   ie = &sztHovrReqd->protocolIEs.member[compCnt++];
   wrFillTknU8(&(ie->pres), PRSNT_NODEF);
   wrFillTknU32(&(ie->id), Sztid_MME_UE_S1AP_ID);
   wrFillTknU32(&(ie->criticality), SztCriticalityrejectEnum);
   mme_Ue_S1ap_Id = transCb->ueCb->s1ConCb->mme_ue_s1ap_id;
   wrFillTknU32(&(ie->value.u.sztMME_UE_S1AP_ID), mme_Ue_S1ap_Id);
   /*
    * Fill eNB UE S1AP ID IE which is generated by eNB.
    * eNB UE S1AP ID uniquly identify the UE over S1 interface within eNB.
    */
   ie = &sztHovrReqd->protocolIEs.member[compCnt++];
   wrFillTknU8(&(ie->pres), PRSNT_NODEF);
   wrFillTknU32(&(ie->id), Sztid_eNB_UE_S1AP_ID);
   wrFillTknU32(&(ie->criticality), SztCriticalityrejectEnum);
   enb_Ue_S1ap_Id = transCb->ueCb->s1ConCb->enb_ue_s1ap_id;
   wrFillTknU32(&(ie->value.u.sztENB_UE_S1AP_ID), enb_Ue_S1ap_Id);
   /* 
    * Fill Handover type IE. 
    */
   ie = &sztHovrReqd->protocolIEs.member[compCnt++];
   wrFillTknU8(&(ie->pres), PRSNT_NODEF);
   wrFillTknU32(&(ie->id), Sztid_HovrTyp);
   wrFillTknU32(&(ie->criticality), SztCriticalityrejectEnum);
   /* Type of Handover.*/
   wrFillTknU32(&(ie->value.u.sztHovrTyp), 
         s1SrcHoTrans->sztHovrTyp->val);
   /*
    * Fill Cause IE which indicates the reason why the UE Handover from source
    * eNB to Target eNB.
    */
   ie = &sztHovrReqd->protocolIEs.member[compCnt++];
   wrFillTknU8(&(ie->pres), PRSNT_NODEF);
   wrFillTknU32(&(ie->id), Sztid_Cause);
   wrFillTknU32(&(ie->criticality), SztCriticalityignoreEnum);
        
   if (ROK != wrUmmS1HoFillCause(&ie->value.u.sztCause, 
            s1SrcHoTrans->sztCause))
   {
      RLOG0(L_ERROR, "Failed To Fill Cause Value");
      RETVALUE(RFAILED);
   }
        /*Added for KPI*/
#ifdef WR_RSYS_KPI 
        wrUmmS1HoFillCause(&(ueCb->hovSztCause),s1SrcHoTrans->sztCause);
#endif/*WR_RSYS_KPI*/
        /*ccpu00131043*/
        wrEmmUpdateKpiCounter(s1SrcHoTrans->sztCause);
   /* 
    * Fill Target EnbId IE. It is combination of Global eNB Id and Selected 
    * TAI.
    */
   ie = &sztHovrReqd->protocolIEs.member[compCnt++];
   wrFillTknU8(&(ie->pres), PRSNT_NODEF);
   wrFillTknU32(&(ie->id), Sztid_TgetID);
   wrFillTknU32(&(ie->criticality), SztCriticalityrejectEnum);
   /* Based on HANDOVER TYPE, Target-Id will change.*/
   if(ROK != wrUmmS1SrcHoFillTgetId(transCb, &ie->value.u.sztTgetID,
            hovrReqrdPdu))
   {
      RLOG0(L_ERROR, "Failed To Fill Cause Value");
      RETVALUE(RFAILED);
   }
   /*
    * To Fill whether Direct Forwarding Path is available b/n Src and Tgt
    */
   if(s1SrcHoTrans->t.intraLteInfo.isDirectPathAvl == TRUE)
   {
      /* Direct Path exists b/n Src and Tgt */
      ie = &sztHovrReqd->protocolIEs.member[compCnt++];
      wrFillTknU8(&(ie->pres), PRSNT_NODEF);
      wrFillTknU32(&(ie->id), Sztid_Direct_Fwding_Path_Avlblty);
      wrFillTknU32(&(ie->criticality), SztCriticalityignoreEnum);

      wrFillTknU32(&(ie->value.u.sztDirect_Fwding_Path_Avlblty), SztDirect_Fwding_Path_AvlbltydirectPathAvailableEnum);
   }


   /* Fill Src To Target Trans Cont For Testing Purpose TODO*/
   ie = &sztHovrReqd->protocolIEs.member[compCnt++];
   wrFillTknU8(&(ie->pres), PRSNT_NODEF);
   wrFillTknU32(&(ie->id), Sztid_Src_ToTget_TprntCont);
   wrFillTknU32(&(ie->criticality), SztCriticalityrejectEnum);

   ie->value.u.sztid_Src_ToTget_TprntCont.pres = PRSNT_NODEF;
   SFndLenMsg(sztEncCfm->u.mBuf,(S16*) &len);
   ie->value.u.sztid_Src_ToTget_TprntCont.len  = len;
   WR_GET_MEM((&hovrReqrdPdu->memCp), len, 
         &(ie->value.u.sztid_Src_ToTget_TprntCont.val));
   SCpyMsgFix(sztEncCfm->u.mBuf, 0, len,
         (Data *)(ie->value.u.sztid_Src_ToTget_TprntCont.val), &contLen);
   /* CSG_DEV Start */
   if ((WR_ENB_HYBRID_ACCESS == s1SrcHoTrans->csgCellAccessMode) || 
         (WR_ENB_CLOSED_ACCESS == s1SrcHoTrans->csgCellAccessMode))
   {
       ie = &sztHovrReqd->protocolIEs.member[compCnt++];

       wrFillTknU8(&(ie->pres), PRSNT_NODEF);
       wrFillTknU32(&(ie->id), Sztid_CSG_Id);
       wrFillTknU32(&(ie->criticality), SztCriticalityrejectEnum);
       wrFillTknBStr32(&(ie->value.u.sztCSG_Id), 27, s1SrcHoTrans->csgId);
   }
   if (WR_ENB_HYBRID_ACCESS == s1SrcHoTrans->csgCellAccessMode)
   {
       ie = &sztHovrReqd->protocolIEs.member[compCnt++];

       wrFillTknU8(&(ie->pres), PRSNT_NODEF);
       wrFillTknU32(&(ie->id), Sztid_CellAccessMode);
       wrFillTknU32(&(ie->criticality), SztCriticalityrejectEnum);
       wrFillTknU32(&(ie->value.u.sztCellAccessMode),
             SztCellAccessModehybridEnum);
   }
   /* CSG_DEV Ends */

   *pdu = hovrReqrdPdu;

   RETVALUE(ROK);


}/* End of wrUmmS1SrcHoFillHovrReqrdMsg*/

/* This function fills GERAN info to oldBssToNewBss 
 * structure according to TLV format */
PRIVATE S16 wrUmmEncOldBssToNewBss
(
WrUmmIRatGeranInfo *iRatGeranHoTransInfo,
TknStrOSXL         *oldBssToNewBss,
CmMemListCp        *memCp
)
{
   U8 oldBssBewBssLen   = 0;
   U8 msgIdx = 0;
   /* calclulate total length */
   /* As of now including only Target cell radio information */
   oldBssBewBssLen = WR_UMM_LEN_IRAT_HO_TGT_RAD_INFO;
   /* Add length of IEI and len Indicatori = 2 */
   oldBssBewBssLen += 2;

   /* Allocate memory for the IEi, total memory required = oldBssBewBssLen + 2 */
   oldBssToNewBss->pres = PRSNT_NODEF;
   oldBssToNewBss->len = oldBssBewBssLen + 2;
   WR_GET_MEM(memCp, oldBssBewBssLen + 2, &(oldBssToNewBss->val));

   WR_UMM_TLV_PK_TAGIE(WR_UMM_OLD_BSS_TO_NEW_BSS_IE, (oldBssToNewBss->val[msgIdx++]) );
   WR_UMM_TLV_PK_LI(oldBssBewBssLen, (oldBssToNewBss->val[msgIdx++]));
   /* Fill Target cell radio information */
   WR_UMM_TLV_PK_TAGIE(WR_UMM_IRAT_HO_TGT_RAD_INFO, (oldBssToNewBss->val[msgIdx++]) );
   WR_UMM_TLV_PK_LI(WR_UMM_LEN_IRAT_HO_TGT_RAD_INFO,( oldBssToNewBss->val[msgIdx++]));
   WR_UMM_TLV_PK_RXLEV(iRatGeranHoTransInfo->rxLev, (oldBssToNewBss->val[msgIdx++]));
   RETVALUE(ROK);
}/* End of function wrUmmEncOldBssToNewBss */

/** @brief This function is responsible filling HO required 
 *  message in case of IRAT HO to UTRAN
 *
 * @details
 *
 *     Function: wrUmmS1SrcHoToUtraFillHovrReqrdMsg 
 *
 *
 * @param [in] WrUmmTransCb* transCb 
 * @param [in] S1apPdu**     pdu
 * @return S16
 *      -# Success  : ROK
 *      -# Failure  : RFAILED
 */
PRIVATE S16 wrUmmS1SrcHoToIRatFillHovrReqrdMsg
(
WrUmmTransCb                 *transCb,
S1apPdu                      **pdu
)
{
   S1apPdu                        *hovrReqrdPdu = NULLP;
   SztS1AP_PDU                    *s1apPdu = NULLP;
   SztInitiatingMsg               *initiatingMsg = NULLP;
   SztHovrReqd                    *sztHovrReqd = NULLP; 
   SztProtIE_Field_HovrReqdIEs    *ie = NULLP;
   U32                            mme_Ue_S1ap_Id;
   U32                            enb_Ue_S1ap_Id;
   U16                            numComp;
   U16                            compCnt = 0;
   MsgLen                            contLen;
   SztEncDecEvnt                  *sztEncCfm = transCb->msg->u.sztEncCfm;
   U16                            len;  
   WrUmmS1SrcHoTrans              *s1SrcHoTrans = &transCb->u.s1SrcHoTrans;
   TknU32                         *srvccHoInd = &(s1SrcHoTrans->srvccHoInd);
   WrUmmIRatGeranInfo             *iRatGeranHoTransInfo =
                                   &(transCb->u.s1SrcHoTrans.t.interRatGeranInfo);
   TknStrOSXL                     *oldBssToNewBss;

   /* Memory Allocation for S1ap PDU.*/
   if (ROK != cmAllocEvnt(sizeof(S1apPdu), WR_SZ_MEM_SDU_SIZE,  
            &wrCb.mem, (Ptr *)&hovrReqrdPdu))
   {
      RLOG0(L_FATAL,"Memory allocation failed.");
      RETVALUE(RFAILED);
   }/* End of if condition.*/

   s1apPdu = &hovrReqrdPdu->pdu;
   wrFillTknU8(&(s1apPdu->choice),S1AP_PDU_INITIATINGMSG);
   initiatingMsg = &s1apPdu->val.initiatingMsg; 
   wrFillTknU8(&(initiatingMsg->pres), PRSNT_NODEF);
   wrFillTknU32(&(initiatingMsg->procedureCode), Sztid_HovrPrep);
   wrFillTknU32(&(initiatingMsg->criticality), SztCriticalityrejectEnum);
   /* Indicates No of Mandatory IEs.*/
   numComp = 6;
   /* Include SRVCC Indication IE */
   if ( srvccHoInd->pres == PRSNT_NODEF ) 
   {
      numComp++;
      if(SztHovrTypltetogeranEnum == s1SrcHoTrans->sztHovrTyp->val)
      {
         /* To Fill Classmark 2 and Classmark 3*/
         numComp = numComp + 2;
      }
   }
   /* This is for IE-PS Service Not available */
   if((SztHovrTypltetogeranEnum == s1SrcHoTrans->sztHovrTyp->val) && 
      (s1SrcHoTrans->t.interRatGeranInfo.psSrvcNotAvlbl == TRUE))
   {
     numComp++;
   }

   /* TODO : Need to find the number of optional IE to be filled. Presently
             none of the optional IE filled in case of IRAT HO */
   sztHovrReqd = &initiatingMsg->value.u.sztHovrReqd;
   /* Fill the Protocol IEs of Handover Required Message. */
   wrFillTknU8(&(sztHovrReqd->pres),PRSNT_NODEF);
   wrFillTknU16(&(sztHovrReqd->protocolIEs.noComp), numComp);
   /* Allocate the memory for Protocol IEs.*/
   if ((cmGetMem(&hovrReqrdPdu->memCp, 
               (numComp * sizeof(SztProtIE_Field_HovrReqdIEs)),
               ((Ptr *)&sztHovrReqd->protocolIEs.member))) !=ROK)
   {
      RLOG0(L_FATAL, "Memory allocation failed.");
      RETVALUE(RFAILED);
   }
   /* 
    * Fill MME UE S1AP ID IE which is generated by MME.
    * MME UE S1AP Id uniquly identify the UE over S1 interface within MME.
    * End of Comment.
    */
   ie = &sztHovrReqd->protocolIEs.member[compCnt++];
   wrFillTknU8(&(ie->pres), PRSNT_NODEF);
   wrFillTknU32(&(ie->id), Sztid_MME_UE_S1AP_ID);
   wrFillTknU32(&(ie->criticality), SztCriticalityrejectEnum);
   mme_Ue_S1ap_Id = transCb->ueCb->s1ConCb->mme_ue_s1ap_id;
   wrFillTknU32(&(ie->value.u.sztMME_UE_S1AP_ID), mme_Ue_S1ap_Id);
   /*
    * Fill eNB UE S1AP ID IE which is generated by eNB.
    * eNB UE S1AP ID uniquly identify the UE over S1 interface within eNB.
    */
   ie = &sztHovrReqd->protocolIEs.member[compCnt++];
   wrFillTknU8(&(ie->pres), PRSNT_NODEF);
   wrFillTknU32(&(ie->id), Sztid_eNB_UE_S1AP_ID);
   wrFillTknU32(&(ie->criticality), SztCriticalityrejectEnum);
   enb_Ue_S1ap_Id = transCb->ueCb->s1ConCb->enb_ue_s1ap_id;
   wrFillTknU32(&(ie->value.u.sztENB_UE_S1AP_ID), enb_Ue_S1ap_Id);
   /* 
    * Fill Handover type IE. In Current version,we supports only Intra LTE 
    * handover type.So, Value is hard coded here.
    */
   ie = &sztHovrReqd->protocolIEs.member[compCnt++];
   wrFillTknU8(&(ie->pres), PRSNT_NODEF);
   wrFillTknU32(&(ie->id), Sztid_HovrTyp);
   wrFillTknU32(&(ie->criticality), SztCriticalityrejectEnum);
   /* Type of Handover.*/
   wrFillTknU32(&(ie->value.u.sztHovrTyp), 
         s1SrcHoTrans->sztHovrTyp->val);
   /*
    * Fill Cause IE which indicates the reason why the UE Handover from source
    * eNB to Target eNB.
    */
   ie = &sztHovrReqd->protocolIEs.member[compCnt++];
   wrFillTknU8(&(ie->pres), PRSNT_NODEF);
   wrFillTknU32(&(ie->id), Sztid_Cause);
   wrFillTknU32(&(ie->criticality), SztCriticalityignoreEnum);

   if (ROK != wrUmmS1HoFillCause(&ie->value.u.sztCause, 
            s1SrcHoTrans->sztCause))
   {
      RLOG0(L_ERROR,"Failed To Fill Cause Value.");
      RETVALUE(RFAILED);
   }
   /* 
    * Fill Target RNC ID Is is combination of LAC, RAC and RNC-ID 
    */
   ie = &sztHovrReqd->protocolIEs.member[compCnt++];
   wrFillTknU8(&(ie->pres), PRSNT_NODEF);
   wrFillTknU32(&(ie->id), Sztid_TgetID);
   wrFillTknU32(&(ie->criticality), SztCriticalityrejectEnum);

   if(ROK != wrUmmS1SrcHoFillTgetId(transCb, &ie->value.u.sztTgetID,
            hovrReqrdPdu))
   {
      RLOG0(L_ERROR,"Failed To Fill Target Id");
      RETVALUE(RFAILED);
   }
   /* Fill SRVCC Indication IE */
   if ( srvccHoInd->pres == PRSNT_NODEF ) 
   {
     ie = &sztHovrReqd->protocolIEs.member[compCnt++];
     wrFillTknU8(&(ie->pres), PRSNT_NODEF);
     wrFillTknU32(&(ie->id), Sztid_SRVCCHOInd);
     wrFillTknU32(&(ie->criticality), SztCriticalityrejectEnum);
     wrFillTknU32(&(ie->value.u.sztSRVCCHOInd), srvccHoInd->val);
     RLOG0(L_INFO, "Filling SRVCC HO Indication in HO required message \n");
     
     /* Fill MS Classmark 2 and MS Classmark 3 if SRVCC to GERAN is present */
     if(SztHovrTypltetogeranEnum == transCb->u.s1SrcHoTrans.sztHovrTyp->val)
     {
        /* Fill Source to Target Transparent container IE */
        ie = &sztHovrReqd->protocolIEs.member[compCnt++];
        wrFillTknU8(&(ie->pres), PRSNT_NODEF);
        wrFillTknU32(&(ie->id), Sztid_Src_ToTget_TprntCont);
        wrFillTknU32(&(ie->criticality), SztCriticalityrejectEnum);
        /*TODO filling part is pending now */
        /*Currently only filling Target cell radio information IEI in Old BSS to New BSS*/
        oldBssToNewBss = &(ie->value.u.sztid_Src_ToTget_TprntCont);
        wrUmmEncOldBssToNewBss(iRatGeranHoTransInfo, oldBssToNewBss, &(hovrReqrdPdu->memCp)); 

        ie = &sztHovrReqd->protocolIEs.member[compCnt++];
        wrFillTknU8(&(ie->pres), PRSNT_NODEF);
        wrFillTknU32(&(ie->id), Sztid_MSClassmark2);
        wrFillTknU32(&(ie->criticality), SztCriticalityrejectEnum);
        if(ROK != wrUmmS1SrcHoFillMsClassMark2(transCb, &ie->value.u.sztMSClassmark2,
                 hovrReqrdPdu))
        {
           RLOG0(L_ERROR,"Failed To Fill ClassMark2 IE");
           RETVALUE(RFAILED);
        }
         RLOG0(L_INFO, "Filling Classmark2 IE in handover required message \n");
        
        ie = &sztHovrReqd->protocolIEs.member[compCnt++];
        wrFillTknU8(&(ie->pres), PRSNT_NODEF);
        wrFillTknU32(&(ie->id), Sztid_MSClassmark3);
        wrFillTknU32(&(ie->criticality), SztCriticalityignoreEnum);
        if(ROK != wrUmmS1SrcHoFillMsClassMark3(transCb, &ie->value.u.sztMSClassmark3,
                 hovrReqrdPdu))
        {
           RLOG0(L_ERROR,"Failed To Fill Classmark3 IE");
           RETVALUE(RFAILED);
        }
         RLOG0(L_INFO, "Filling Classmark3 IE in handover required message \n");
        /* Fill PS Service not available if it is set to TRUE */
        if(s1SrcHoTrans->t.interRatGeranInfo.psSrvcNotAvlbl == TRUE)
        {
           ie = &sztHovrReqd->protocolIEs.member[compCnt];
           wrFillTknU8(&(ie->pres), PRSNT_NODEF);
           wrFillTknU32(&(ie->id), Sztid_PS_ServiceNotAvailable);
           wrFillTknU32(&(ie->criticality), SztCriticalityignoreEnum);
           wrFillTknU32(&(ie->value.u.sztPS_ServiceNotAvailable), 
                SztPS_ServiceNotAvailableps_service_not_availableEnum);
           RLOG0(L_INFO, "Filling PS serving not available IE in handover required message \n");
        }
        *pdu = hovrReqrdPdu;
        RETVALUE(ROK);
     }
   }
   /* Fill Src To Target Trans Cont */ 
   ie = &sztHovrReqd->protocolIEs.member[compCnt];
   wrFillTknU8(&(ie->pres), PRSNT_NODEF);
   wrFillTknU32(&(ie->id), Sztid_Src_ToTget_TprntCont);
   wrFillTknU32(&(ie->criticality), SztCriticalityrejectEnum);

   ie->value.u.sztid_Src_ToTget_TprntCont.pres = PRSNT_NODEF;
   SFndLenMsg(sztEncCfm->u.mBuf,(S16*) &len);
   ie->value.u.sztid_Src_ToTget_TprntCont.len  = len;
   WR_GET_MEM((&hovrReqrdPdu->memCp), len, 
         &(ie->value.u.sztid_Src_ToTget_TprntCont.val));
   SCpyMsgFix(sztEncCfm->u.mBuf, 0, len,
         (Data *)(ie->value.u.sztid_Src_ToTget_TprntCont.val), &contLen);

   *pdu = hovrReqrdPdu;

   RETVALUE(ROK);
}/* End of  wrUmmS1SrcHoToUtraFillHovrReqrdMsg */



PUBLIC S16 wrUmmS1SrcSndHovrCancel
(
 WrUmmTransCb                    *transCb
)
{
   /* Relocation Timer has expired So Src eNB shall prepare Handover Cancel  */
   /* and send to Tgt eNB.                                                   */
   SztDatEvntReq              datEvtReq;
   S1apPdu                   *hovrCancelPdu = NULLP;
   U32                       s1CancelTmrVal = 0;

   if(transCb->ueCb->s1ConCb == NULLP)
   {
      RLOG0(L_ERROR,"Failed to Send HO cancel message(MME is down)");
      RETVALUE(ROK);
   }
   
   if (ROK != wrUmmS1SrcHoFillHovrCancel(transCb, &hovrCancelPdu))
   {
      RLOG0(L_FATAL, "Memory allocation failed.");
      RETVALUE(RFAILED);
   }
   RLOG_ARG2(L_EVENT, DBG_CRNTI,transCb->ueCb->crnti,"[S1AP]:Sending HANDOVER CANCEL"
             "[MME-UE-S1AP-ID:%lu] [eNB-UE-S1AP-ID:%lu]",
             transCb->ueCb->s1ConCb->mme_ue_s1ap_id, transCb->ueCb->s1ConCb->enb_ue_s1ap_id);
   wrPrintS1apCause(transCb->u.s1SrcHoTrans.sztCause);
   /*Reseting HO params*/
#if 0 /* ccpu00147128 */
   transCb->ueCb->hoTransId = 0;
   transCb->ueCb->hoType = WR_HO_TYPE_NONE;
   transCb->ueCb->mobCtrlState = WR_HO_NORMAL;
#endif

   /* Send Handover Required Message to the MME.*/
   datEvtReq.spConnId = transCb->ueCb->s1ConCb->spConnId;
   datEvtReq.pdu    =  hovrCancelPdu;
   /* Trigger SZT Dat Req */
   RLOG1(L_INFO, "Sending Handover Cancel message to UE crnti %d \n", transCb->ueCb->crnti);
   WrIfmS1apDatReq(&datEvtReq);
   
   cmInitTimers(&transCb->u.s1SrcHoTrans.s1CancelTmr.tmr,1);
   wrEmmGetTmrVal( transCb->ueCb->cellId, 
            WR_TMR_SRC_HO_CANCEL ,&s1CancelTmrVal); 
   wrStartTransTmr(transCb, WR_TMR_SRC_HO_CANCEL, s1CancelTmrVal);

   RETVALUE(ROK);
}/* End of wrUmmS1SrcSndHovrCancel */

PRIVATE S16 wrUmmS1SrcHoPrepHandoverReqrd
(
WrUmmTransCb                 *transCb
)
{
   SztDatEvntReq             datEvtReq;
   S1apPdu                   *hovrReqrdPdu = NULLP;
   S16                       ret;
   U32                       s1PrepTmrVal = 0;
   WrUeCb                    *ueCb = transCb->ueCb;

   if((transCb->u.s1SrcHoTrans.sztHovrTyp->val != SztHovrTypltetogeranEnum) && 
      (transCb->msg->u.sztEncCfm->u.mBuf == NULLP))
   {
      RLOG0(L_ERROR, "encoding of Source to Target Container Failed");
      RETVALUE(RFAILED);
   }

   RLOG_ARG3(L_EVENT, DBG_CRNTI,ueCb->crnti,"[S1AP]:Sending HANDOVER REQUIRED "
             "[MME-UE-S1AP-ID:%lu] [eNB-UE-S1AP-ID:%lu] [HOType:%d]",
             ueCb->s1ConCb->mme_ue_s1ap_id, ueCb->s1ConCb->enb_ue_s1ap_id,
             transCb->u.s1SrcHoTrans.sztHovrTyp->val);
   /* Filling of Handover Required Message.*/
    
   if ( SztHovrTypintralteEnum == transCb->u.s1SrcHoTrans.sztHovrTyp->val)
   {   
      ret =  wrUmmS1SrcHoFillHovrReqrdMsg(transCb, &hovrReqrdPdu);
   }
   else if ((SztHovrTypltetoutranEnum == transCb->u.s1SrcHoTrans.sztHovrTyp->val)
         || (SztHovrTypltetogeranEnum == transCb->u.s1SrcHoTrans.sztHovrTyp->val))
   { 
      ret = wrUmmS1SrcHoToIRatFillHovrReqrdMsg(transCb, &hovrReqrdPdu);
   }
   else
   {
      RLOG1(L_ERROR,"Unhandled HO Type[%d]", 
           transCb->u.s1SrcHoTrans.sztHovrTyp->val);
      RETVALUE(RFAILED);
   }
   if (ROK != ret ) 
   {
    
      RLOG0(L_FATAL, "Memory allocation failed.");
      RETVALUE(RFAILED);
   }
   /* Send Handover Required Message to the MME.*/
   datEvtReq.spConnId = transCb->ueCb->s1ConCb->spConnId;
   datEvtReq.pdu    =  hovrReqrdPdu;


   RLOG_ARG3(L_DEBUG, DBG_CRNTI, transCb->ueCb->crnti,"Sending S1 HANDOVER "
      "REQUIRED, MME-UE-S1AP-ID[%lu] Handover Type[%d] Cause[%d]",
      transCb->ueCb->s1ConCb->mme_ue_s1ap_id,
      transCb->u.s1SrcHoTrans.sztHovrTyp->val,
      (U32)transCb->u.s1SrcHoTrans.sztCause->val.radioNw.val);
   cmInitTimers(&transCb->u.s1SrcHoTrans.s1RelocTmr.tmr,1);
   /* Start the relocation timer with respective values
      and increment the KPI counter for intra LTE HO */
   if ( SztHovrTypintralteEnum == transCb->u.s1SrcHoTrans.sztHovrTyp->val)
   {
      /* Increment KPI for Attempted outgoing inter-eNB handover preparations */
  WR_INC_KPI_FUNC(wrIncKpiFrOutIntrEnbHoPrpAtmpt);    
      wrEmmGetTmrVal( transCb->ueCb->cellId, 
            WR_UMM_INT_LTE_S1_PREP_TMR,&s1PrepTmrVal); 

   }
   else if ( ( SztHovrTypltetoutranEnum ==
                      transCb->u.s1SrcHoTrans.sztHovrTyp->val) ||
             ( SztHovrTypltetogeranEnum == 
                        transCb->u.s1SrcHoTrans.sztHovrTyp->val ))
   {
      wrEmmGetTmrVal( transCb->ueCb->cellId, 
            WR_UMM_IRAT_S1_PREP_TMR,&s1PrepTmrVal);

   }
   wrStartTransTmr(transCb, WR_TMR_S1_RELOC_TMR,s1PrepTmrVal); 
   /* Trigger SZT Dat Req */
   WrIfmS1apDatReq(&datEvtReq);
  RETVALUE(ROK);
}


PUBLIC S16 wrS1apGetTnlAddr
(
CmTptAddr                    *addr,
SztTportLyrAddr              *ie
)
{
   U8                        shiftBits = 0;
   U16                       indx = 0;
   U32                       addrMask = 0;
   switch(ie->len)
   {
      case WR_UMM_IPV4_LEN:
      {
         addr->type = CM_TPTADDR_IPV4;
         addr->u.ipv4TptAddr.port = WR_DFLT_EGTP_PORT; 

         /* copy 4bytes into the U32 */
         shiftBits = ie->len / 8;
         addrMask = 0xFF000000;
         for(indx = 0; indx < 4; indx++)
         {
            shiftBits--;
            addr->u.ipv4TptAddr.address |=
               ((U32)(ie->val[indx] << (8 * shiftBits)) & addrMask);
            addrMask = addrMask >> 8;
         }
         break;
      }
      case WR_UMM_IPV6_LEN:
      {
         addr->type = CM_TPTADDR_IPV6;
         addr->u.ipv6TptAddr.port = WR_DFLT_EGTP_PORT;
         for (indx = 0; indx < 16; indx++)
         {
            addr->u.ipv6TptAddr.ipv6NetAddr[indx] = ie->val[indx];
         }
         break;
      }
   }
   RETVALUE(ROK);
}

PRIVATE S16 wrS1apGetTnlId
(
U32                       *tnlId,
SztGTP_TEID               *remTeIdStr
)
{
   U8                        shiftBits = 0;
   U16                       indx = 0;
   U32                       addrMask = 0;

   shiftBits = (remTeIdStr->len);
   addrMask = 0xFF000000;
   for(indx = 0;indx < remTeIdStr->len;indx++)
   {
      shiftBits--;
      (*tnlId) |= ((U32)(remTeIdStr->val[indx] << (8*shiftBits)) & addrMask);
      addrMask = addrMask>>8;
   }
   RETVALUE(RFAILED);
}

#if 1
PRIVATE S16 wrUmmS1SrcHoFillErabFrmS1HoAck
(
WrUmmTransCb                 *transCb,
SztE_RABSubjtoDataFwdingLst  *e_RABFwdLst
)
{
   U8                           rbIdx;
   SztE_RABDataFwdingItem       *eRABItem = NULLP;
   WrUmmS1SrcHoRabInfo          *fwdRbInfo;
   SztGTP_TEID                  *tnlInfo = NULLP;
   WrUmmS1SrcHoTrans            *s1SrcHoTrans = NULLP;
   WrUeCb                       *ueCb = transCb->ueCb;
   WrUeRAB                      *ueRbInfo = NULLP;

   s1SrcHoTrans = &transCb->u.s1SrcHoTrans;
   s1SrcHoTrans->numOfRbInfo = e_RABFwdLst->noComp.val;
   WR_ALLOC(&s1SrcHoTrans->rbInfo,s1SrcHoTrans->numOfRbInfo * sizeof(WrUmmS1SrcHoRabInfo));
   if(s1SrcHoTrans->rbInfo == NULLP)
   {
      RLOG0(L_ERROR, "rbInfo is NULLP in s1SrcHoTrans");
      RETVALUE(RFAILED);
   }
   /* fill the FWD tunnel information from the ERAB list */
   for(rbIdx = 0; rbIdx < e_RABFwdLst->noComp.val; rbIdx++)
   {
      eRABItem = &e_RABFwdLst->member[rbIdx].value.u.sztE_RABDataFwdingItem;
      ueRbInfo = &ueCb->rbInfo.rab[eRABItem->e_RAB_ID.val-WR_UMM_ERAB_OFFSET];
      if(ueRbInfo->eRABId == eRABItem->e_RAB_ID.val)
      {
      }
      else
      {
         WR_FREE(s1SrcHoTrans->rbInfo,
               s1SrcHoTrans->numOfRbInfo * sizeof(WrUmmS1SrcHoRabInfo));
         s1SrcHoTrans->numOfRbInfo = 0;
         RETVALUE(RFAILED);
      }
      fwdRbInfo = &(s1SrcHoTrans->rbInfo[rbIdx]);
      /* Initialize the fwd information */
      fwdRbInfo->erabId    = ueRbInfo->eRABId;
      fwdRbInfo->rbId      = ueRbInfo->rbId;
      fwdRbInfo->rbDir     = ueRbInfo->rbDir;
      fwdRbInfo->qci       = ueRbInfo->qci;
      fwdRbInfo->locTunId  = ueRbInfo->locEgtpTunId;

      /* Copy DL Tunnel info */
      tnlInfo = &(eRABItem->dL_gTP_TEID);
      /* Check if the Transport address is present and if it is present, then 
       * only extract it
       */
      if((PRSNT_NODEF == tnlInfo->pres) && 
         (PRSNT_NODEF == eRABItem->dL_transportLyrAddr.pres))
      {
         fwdRbInfo->dlFwding  = TRUE;
         /* store the dl fwd tunnel id */
         wrS1apGetTnlId(&fwdRbInfo->datFwdDlRemTunId, tnlInfo);
         /* store the Peer IP Address */
         wrS1apGetTnlAddr(&fwdRbInfo->peerAddr, &eRABItem->dL_transportLyrAddr);
      }

      /* Copy UL Tunnel info */
      tnlInfo = &(eRABItem->uL_GTP_TEID);

      /* Moved the Transport layer address present check here */
      if ((PRSNT_NODEF == tnlInfo->pres) &&
          (PRSNT_NODEF == eRABItem->uL_TportLyrAddr.pres))

      {
         /* Ignore the UL GTP-TEID , if received in case of HO to UTRAN 
          * Or HO to Geran */
         if (( SztHovrTypltetoutranEnum == s1SrcHoTrans->sztHovrTyp->val ) ||
             ( SztHovrTypltetogeranEnum == s1SrcHoTrans->sztHovrTyp->val))
         {
            RLOG0(L_WARNING, "Ignoring UL GTP-TEID received,"
                          "as hoType is SztHovrTypltetoutran/GernaEnum");
            fwdRbInfo->isULDatFwdReq = FALSE;
            /* Removed the continue here as we have to handle the logical 
             * error below.
             */
         }
         else
         {
         /* store the ul fwd tunnel id */
         wrS1apGetTnlId(&fwdRbInfo->datFwdUlRemTunId, tnlInfo);

            /* Get the Transport address for UL data forwarding */
         wrS1apGetTnlAddr(&fwdRbInfo->peerAddr, &eRABItem->uL_TportLyrAddr);
         fwdRbInfo->isULDatFwdReq = TRUE;
         }
      }
      /* Check if we have any invalid tunnel information. A Tunnel is valid if
       * eNodeB receives both GTP-TEID and TRANSPORT ADDRESS valid for at least
       * one direction, i.e. either for UL or DL. If both tunnel information
       * are invalid, it will be treated as invalid and transaction is closed.
       * If both UL and DL forwarding information is FALSE, then the tunnel
       * information is not complete, hence close the transaction
       */
      if ((FALSE == fwdRbInfo->isULDatFwdReq) && 
          (FALSE == fwdRbInfo->dlFwding))
      {
         RLOG1(L_ERROR, "Invalid Forwading Tunnel infomation,"
               "Sending handover Cancel: transId %x\n", transCb->transId);
         /* To cleanup the target side, we need to send the HO Cancel
          * and the handover transaction will be closed after the 
          * HO Cancel ACK comes from MME or if HO Cancel TMR expires
          */
         transCb->u.s1SrcHoTrans.sztCause->choice.pres = TRUE;
         transCb->u.s1SrcHoTrans.sztCause->choice.val = WR_CAUSE_PROTOCOL;
         transCb->u.s1SrcHoTrans.sztCause->val.protocol.pres = TRUE;
         transCb->u.s1SrcHoTrans.sztCause->val.protocol.val = 
            SztCauseProtsemantic_errorEnum;

         wrUmmS1SrcSndHovrCancel(transCb);

         RETVALUE(RFAILED);
      }
   }/* End of for() */

   RETVALUE(ROK);
}
#endif
PRIVATE S16 wrUmmS1SrcHoFillE_RABSubjtoDataFwdingLst
(
WrUmmTransCb                 *transCb,
SztE_RABSubjtoDataFwdingLst  *sztE_RABSubjtoDataFwdingLst
)
{
#if 1
   if (ROK != wrUmmS1SrcHoFillErabFrmS1HoAck(transCb,   sztE_RABSubjtoDataFwdingLst))
   {
      RLOG0(L_ERROR, "Failed To Fill ERAB Subject To Data forwarding List");
      RETVALUE(RFAILED);
   }
#endif
   RETVALUE(ROK);
}/*End of wrUmmS1SrcHoFillE_RABSubjtoDataFwdingLst*/
/* Fill E-RAB Release List.*/
PRIVATE S16 wrUmmS1SrcHoFillE_RABToRlsLst
(
WrUmmTransCb                 *transCb,
SztE_RABLst                  *sztE_RABLst
)
{

   U8                                 noComp = 0;
   U8                                 idx;
   SztProtIE_SingleCont_E_RABItemIEs  *ie = NULLP;
   SztProtIE_SingleCont_E_RABItemIEs  *transERABItem = NULLP;
   SztCause                           *cause = NULLP;

   noComp = sztE_RABLst->noComp.val;

   transCb->u.s1SrcHoTrans.sztE_RABLst.noComp.val = noComp;
   WR_ALLOC(&transCb->u.s1SrcHoTrans.sztE_RABLst.member,
         noComp * sizeof(SztProtIE_SingleCont_E_RABItemIEs));
   if (NULLP == transCb->u.s1SrcHoTrans.sztE_RABLst.member)
   {
      RLOG0(L_FATAL, "Memory allocation failed.");
      RETVALUE(RFAILED);
   }
   for (idx = 0; idx < noComp; idx++)
   {
      ie = &sztE_RABLst->member[idx];
      transERABItem = &transCb->u.s1SrcHoTrans.sztE_RABLst.member[idx];
      if (NULLP == ie)
      {
         RLOG0(L_ERROR, "here is no member");
         RETVALUE(RFAILED);
      }
      transERABItem->value.u.sztE_RABItem.e_RAB_ID.pres = 
         ie->value.u.sztE_RABItem.e_RAB_ID.pres;
      transERABItem->value.u.sztE_RABItem.e_RAB_ID.val = 
         ie->value.u.sztE_RABItem.e_RAB_ID.val;
      /* Fill Cause values. */
      cause = &transERABItem->value.u.sztE_RABItem.cause;      
      if (ROK != wrUmmS1HoFillCause(&ie->value.u.sztE_RABItem.cause, cause))
      {
         RLOG0(L_ERROR, "Failed To Fill Cause");
         RETVALUE(RFAILED);
      }      
   }
   RETVALUE(ROK);
}

/**
    * To Fill Transaction Control Block.
    */

PRIVATE S16 wrUmmS1SrcHoParseHovrCmd
(
WrUmmTransCb                 *transCb
)
{
   WrUmmIncMsg                      *msg = transCb->msg;
   SztS1AP_PDU                      *s1apPdu = NULLP;
   Szt_SuccessfulOutcomeCls         *successfulOutCome = NULLP;
   SztProtIE_Field_HovrCmmdIEs      *ie = NULLP;
   U8                               noComp;
   U8                               hoCmdIeIdx = 0;
   TknStrOSXL                       *nasSecFrmEutran;

   RLOG_ARG3(L_EVENT, DBG_CRNTI, transCb->ueCb->crnti,"[S1AP]:Received HANDOVER COMMAND"
             "[MME-UE-S1AP-ID:%lu] [eNB-UE-S1AP-ID:%lu] [HOType:%d]",
             transCb->ueCb->s1ConCb->mme_ue_s1ap_id,transCb->ueCb->s1ConCb->enb_ue_s1ap_id, 
             transCb->u.s1SrcHoTrans.sztHovrTyp->val);
   s1apPdu = &msg->u.s1Pdu->pdu->pdu;
   successfulOutCome = &s1apPdu->val.successfulOutcome.value;
   noComp = successfulOutCome->u.sztHovrCmmd.protocolIEs.noComp.val;
   for (hoCmdIeIdx = 0; hoCmdIeIdx < noComp; hoCmdIeIdx++)
   {
      ie = &successfulOutCome->u.sztHovrCmmd.protocolIEs.member[hoCmdIeIdx];
      if (NULLP == ie)
      {
         RLOG0(L_ERROR, "Handover Command IE is NULL");
         RETVALUE(RFAILED);
      }
      switch (ie->id.val)
      {
         case Sztid_MME_UE_S1AP_ID:
         {
		  	   /* Fill MME UE S1AP ID.*/
			   if (transCb->ueCb->s1ConCb->mme_ue_s1ap_id != 
                                              ie->value.u.sztMME_UE_S1AP_ID.val)
            {
               RLOG0(L_ERROR, "MME UE S1AP ID is wrong.");
               RETVALUE(RFAILED);
            }
            break;
        }
        case Sztid_eNB_UE_S1AP_ID:
        {
             /* Fill ENB UE_S1AP ID.*/
             if (transCb->ueCb->s1ConCb->enb_ue_s1ap_id != 
                                              ie->value.u.sztENB_UE_S1AP_ID.val)
             {
              RLOG0(L_ERROR, "ENB UE S1AP ID is wrong");
              RETVALUE(RFAILED);
             }
             break;
        }
        case Sztid_HovrTyp:
        {
             /* Fill type of Handover.*/
               /* HO type should be validated with the Received HO type */
               if ( ie->value.u.sztHovrTyp.val != transCb->u.s1SrcHoTrans.sztHovrTyp->val )
               {
                  RLOG1(L_ERROR,"HoType[%d] is wrong",
                        transCb->u.s1SrcHoTrans.sztHovrTyp->val);
                  /* send HO cancel */
                  transCb->u.s1SrcHoTrans.sztCause->choice.pres = TRUE;
                  transCb->u.s1SrcHoTrans.sztCause->choice.val = WR_CAUSE_PROTOCOL;
                  transCb->u.s1SrcHoTrans.sztCause->val.radioNw.pres = TRUE;
                  transCb->u.s1SrcHoTrans.sztCause->val.radioNw.val = 
                     SztCauseProtmessage_not_compatible_with_receiver_stateEnum;

                  wrUmmS1SrcSndHovrCancel(transCb);

                  RETVALUE(RFAILED);
               }
             break;
        }
        case Sztid_NASSecurParamsfromE_UTRAN:
        {
               /* Handover From E-UTRAN to UTRAN / GERAN : Store the
                  nasSec From UTRAN */
               nasSecFrmEutran = &(transCb->u.s1SrcHoTrans.nasSecParamFrmEutran);
               nasSecFrmEutran->pres = 
                  ie->value.u.sztNASSecurParamsfromE_UTRAN.pres;
               nasSecFrmEutran->len = 
                      ie->value.u.sztNASSecurParamsfromE_UTRAN.len;

               WR_ALLOC(&(nasSecFrmEutran->val), nasSecFrmEutran->len);
               if ( NULLP == nasSecFrmEutran->val ) 
               {
                  RLOG0(L_FATAL,"Memory allocation failed.");
                  RETVALUE(RFAILED);
               }
               cmMemcpy( nasSecFrmEutran->val,
                         ie->value.u.sztNASSecurParamsfromE_UTRAN.val,
                         nasSecFrmEutran->len ); 
             break;
        }
        case Sztid_E_RABSubjtoDataFwdingLst:
        {
             if (ROK != wrUmmS1SrcHoFillE_RABSubjtoDataFwdingLst(transCb,&ie->value.u.sztE_RABSubjtoDataFwdingLst))
             {
              RLOG0(L_ERROR, "Failed to fill E_RABSubjtoDataFwdingLst");
              RETVALUE(RFAILED);
             }
             break;
        }
        case Sztid_E_RABtoRlsLstHOCmd:
        {
             if (ROK != wrUmmS1SrcHoFillE_RABToRlsLst(transCb,
                                                      &ie->value.u.sztE_RABLst))
             {
              RLOG0(L_ERROR, "Failed to fill E_RAB Release List");
              RETVALUE(RFAILED);
             }
           break;
        }
        case Sztid_Tget_ToSrc_TprntCont:
        {
		  	  /* Fill Target To Src Transparent container.*/
           transCb->u.s1SrcHoTrans.tgtToSrcCont = &ie->value.u.sztid_Tget_ToSrc_TprntCont;
		  	  break;
        }
        case Sztid_Src_ToTget_TprntCont_Secondary:
           /* Optional TODO */
            break;
        default:
           break;
      }/* End of Switch()*/
   }
   
   RETVALUE(ROK);
}

PRIVATE S16 wrUmmS1SrcHoDecTgtToSrcCont
(
 WrUmmTransCb                *transCb
)
{
   Buffer                    *trnspContMBuf = NULLP;
   SztEncDecEvnt             sztEncDecEvnt;

   if ( NULLP == transCb->u.s1SrcHoTrans.tgtToSrcCont )
   {
      RLOG0(L_ERROR,"TgtToSrcCont is NULL");
      RETVALUE(RFAILED);
   }
   else
      {
         sztEncDecEvnt.transId = transCb->transId;
         sztEncDecEvnt.type.pres = PRSNT_NODEF;
         sztEncDecEvnt.type.val = SZT_REQ_TYPE_DECODE_IE;
         sztEncDecEvnt.ieType.pres = PRSNT_NODEF;
      if ( SztHovrTypintralteEnum == transCb->u.s1SrcHoTrans.sztHovrTyp->val ) 
      {
         sztEncDecEvnt.ieType.val = SZT_REQ_TYPE_ENCDEC_IE_TGETSRC;
      }
      else if ( SztHovrTypltetoutranEnum == 
                  transCb->u.s1SrcHoTrans.sztHovrTyp->val )
      {
        sztEncDecEvnt.ieType.val = SZT_REQ_TYPE_ENCDEC_IE_TGETRNCSRCRNC; 
      }
      else
      {
          /* TODO: Need to add the types based on support */
      }
         /* Alloc memory for mBuf */
         SGetMsg(wrCb.mem.region, wrCb.mem.pool, &trnspContMBuf);
         if(trnspContMBuf == NULLP)
         {
            RLOG0(L_FATAL, "Memory allocation failed.");
            RETVALUE(RFAILED);
         }
      /* convert the OSXL to Buffer */
      SAddPstMsgMult(transCb->u.s1SrcHoTrans.tgtToSrcCont->val,
                     transCb->u.s1SrcHoTrans.tgtToSrcCont->len, 
                     trnspContMBuf);
         sztEncDecEvnt.u.mBuf = trnspContMBuf;
         WrIfmS1apEncDecReq(&sztEncDecEvnt);
      }
   RETVALUE(ROK);
}
/**
   *  This message is received from MME to inform the source eNB that resources
   *  for the handover have been prepared at the target side.
   *
   *
   */

PRIVATE S16 wrUmmS1SrcHoPrcHovrCmd
(
WrUmmTransCb                 *transCb
)
{
   WrUmmS1SrcHoTrans             *s1SrcHoTrans = &transCb->u.s1SrcHoTrans;


   /* Received the HO Command From MME. So stop the S1 Relocation Timer */
   wrStopTransTmr(transCb,WR_TMR_S1_RELOC_TMR);
   s1SrcHoTrans->state = WR_HANDOVER_PREPARATION_ACK_RCVD;
   /* Parse the Handover Command Message received from MME. */
   if (ROK != wrUmmS1SrcHoParseHovrCmd(transCb))
   {
      RLOG0(L_ERROR, "Failed To parse Handover Command Message");
      RETVALUE(RFAILED);
   }
   /* Decode Target To Source Transparent Container.*/
   /* In case of GERAN, no need to decode the container,
    * The received Handover Command ( L3 Information in case
    * of SRVCC to GERAN ) can be sent directly to UE, hence invoke the
    * wrUmmS1SrcHoPrcIRatHoTgtToSrcDecCfm directly */
   
   if ( transCb->u.s1SrcHoTrans.sztHovrTyp->val == SztHovrTypltetogeranEnum ) 
   {
       RLOG0(L_INFO, "Sending Mobility from EUTRA command message to UE with \
                                                      GERAN container \n");
       if( ROK != wrUmmS1SrcHoPrcIRatHoTgtToSrcDecCfm(transCb))
       {
          RLOG0(L_ERROR, "wrUmmS1SrcHoPrcIRatHoTgtToSrcDecCfm failed");
          RETVALUE(RFAILED);
       }
   }
   else if (ROK != wrUmmS1SrcHoDecTgtToSrcCont(transCb))
   {
      RLOG0(L_ERROR, "Failed To Decode Tgt To Src Cont");
      RETVALUE(RFAILED);
   }

   /* ccpu00131919 */
   /* Src shall not send LocRpt since its a Femto */

   RETVALUE(ROK);
}

PRIVATE S16 wrUmmS1SrcHoFillNhuRrcConReCfgnWithMbuf
(
WrUmmTransCb                 *transCb,
NhuDatReqSdus                *nhuDatReqSdu,
Buffer                       *mBuf
)
{
   /* Fill HDR Info */
   nhuDatReqSdu->hdr.cellId  = transCb->ueCb->cellId; 
   nhuDatReqSdu->hdr.ueId = transCb->ueCb->crnti;
   nhuDatReqSdu->hdr.transId = transCb->transId | C1_RRCCONRECFGN;
   /* Populating NHU Dat Req */
   nhuDatReqSdu->sdu.ueMsg.isMsgDed = TRUE;
   nhuDatReqSdu->sdu.ueMsg.msg.dedMsg.dlDcchMsg.pdcpId = WR_PDCP_ID_SRB1;
   nhuDatReqSdu->sdu.ueMsg.msg.dedMsg.dlDcchMsg.isMsgEncoded = TRUE;
   nhuDatReqSdu->sdu.ueMsg.msg.dedMsg.dlDcchMsg.m.mBuf = mBuf;
   /* Fill Config values */
   nhuDatReqSdu->sdu.isUeCfgPres = FALSE;
   
   RETVALUE(ROK);
}

/* forward declaration */
PRIVATE S16 wrUmmS1SrcHoGetPdcpSduSta(WrUmmTransCb  *transCb);
PRIVATE S16 wrUmmS1SrcHoSendStartPdcpDataFwdReq(WrUmmTransCb  *transCb);
/*Overall timer Expiry*/
PUBLIC S16 wrUmmS1SrcHoOvrallTmrExp
(
 WrUeCb                      *ueCb
)
{
   ueCb->mobCtrlState = WR_HO_UE_RELEASE;
   if (ROK != wrUmmSndCtxtRelReq(ueCb->crnti, 
            ueCb->cellId, WR_ENB_INIT_UE_REL))
   {
      RLOG0(L_ERROR, "FAiled To Send Ue Context Release Message");
      RETVALUE(RFAILED);
   }
   RETVALUE(ROK);
}

PRIVATE S16 wrUmmS1SrcHoHandleNhuDecCfm
(
WrUmmTransCb                 *transCb
)
{
   /* Handle the Decoded Confirm Message from lower layer.*/
   NhuHovrCmd                      *hovrCmd = NULLP;
   NhuHovrCmd_r8_IEshandoverCmdMsg *hoCmdMsg = NULLP;  /* It is TknStrOSXL */
   Buffer                          *mBuf = NULLP;
   NhuDatReqSdus                   *nhuDatReqSdu = NULLP;
   U32                             s1OvrlTmrVal = 0;
   
   if((transCb->msg->u.decCfm->status != NHU_DECODE_SUCCESS))
   {
      RLOG0(L_ERROR, " Decode Failed ");
      RETVALUE(RFAILED);
   }
   hovrCmd = &transCb->msg->u.decCfm->ie.hovrCmd;
   if(hovrCmd->pres.pres)
   {
      hoCmdMsg = 
               &hovrCmd->criticalExtns.val.c1.val.handoverCmd_r8.handoverCmdMsg;

      if((hovrCmd->criticalExtns.val.c1.choice.val != C1_HANDOVERCMD_R8) || (hoCmdMsg->val == NULLP))
      {
         RLOG0(L_ERROR, "Invalid Tgt_to_Src_Container");
         RETVALUE(RFAILED);
      }
      /* convert the OSXL to Buffer */
      WR_ALLOCMBUF((*(wrCb.nhuSap))->pst.region, wrCb.mem.pool,&mBuf );
      if (NULLP == mBuf)
      {
         RLOG0(L_FATAL,"Memory allocation failed.");
         RETVALUE(RFAILED);
      }
      SAddPstMsgMult((Data *)hoCmdMsg->val,
         (MsgLen)hoCmdMsg->len, (Buffer *)mBuf);
         /* Send the RRC Connection Reconfiguration Msg
          * which is already in the encoded format to RRC so that it 
          * sends out to the UE.
          */
         WR_ALLOCEVNT(&nhuDatReqSdu, sizeof(NhuDatReqSdus));
         if (NULLP == nhuDatReqSdu)
         {
            RLOG0(L_FATAL,"Memory allocation failed.");
            WR_FREEMBUF(mBuf);
            RETVALUE(RFAILED);
         }
         if (ROK != wrUmmS1SrcHoFillNhuRrcConReCfgnWithMbuf(transCb, 
                  nhuDatReqSdu, mBuf))
         {
            RLOG0(L_ERROR, "Failed To Fill NhuRrcConReCfgnWithMbuf");
            WR_FREEMBUF(mBuf);
            WR_FREE_EVNT(nhuDatReqSdu);
            RETVALUE(RFAILED);
         }
         wrEmmGetTmrVal( transCb->ueCb->cellId, 
                         WR_UMM_INT_LTE_S1_OVRL_TMR,&s1OvrlTmrVal);

         wrStartUeTmr(transCb->ueCb, WR_TMR_S1_OVRL_TMR,s1OvrlTmrVal);
         if(ROK != wrIfmRrcSndReqToUe(nhuDatReqSdu))
         {
            RLOG0(L_ERROR, "VeLiNhuDatReq failed");
            WR_FREEMBUF(mBuf);
            WR_FREE_EVNT(nhuDatReqSdu);
            RETVALUE(RFAILED);
         }

         /*Increment KPI for outgoing Hov attmpd*/
         transCb->ueCb->hovDir = OUT_HOV;
#ifdef WR_RSYS_KPI 
         transCb->ueCb->hov = transCb->u.s1SrcHoTrans.sztHovrTyp->val; 
#endif/*WR_RSYS_KPI*/
         WR_INC_KPI_FUNC5(wrIncKpiFrHovrAttmptd, WR_HO_TYPE_INTRA_LTE_S1,
               transCb->u.s1SrcHoTrans.sztHovrTyp->val, OUT_HOV,
               (transCb->u.s1SrcHoTrans.sztCause),
               &(transCb->u.s1SrcHoTrans.t.intraLteInfo.ecgi));

         /*Increment KPI for Handover related measurements for DRX/non-DRX Attempted*/
         WR_INC_KPI_FUNC1(wrIncKpiFrHONoDrxOutAttmptd,transCb->ueCb->drxCfg.isDrxEnabled);

         /*Increment KPI for Handover attempted related to meas gaps used or not*/
         WR_INC_KPI_FUNC2(wrIncKpiFrOutHoAtmptBsdOnFrqTyp,transCb->ueCb->freqTyp,transCb->ueCb->measGapUsd);

         transCb->ueCb->mobCtrlState = WR_HO_IN_PROGRESS;

         RLOG_ARG1(L_INFO, DBG_CRNTI, transCb->ueCb->crnti,"S1 HANDOVER state"
            "= IN PROGRESS[%d] Cause = Decoded Confirm Message from RRC",
            transCb->ueCb->mobCtrlState);
          /* Get PDCP SN Status */
         RLOG0(L_DEBUG, "Sending PDCP SN Status Req");
         wrUmmS1SrcHoGetPdcpSduSta(transCb);

   } /* end of if statement */
   else
   {
      RLOG0(L_ERROR, "NhuDecodeSduCfm:  HoCmd not available");
      RETVALUE(RFAILED);
   }
    RETVALUE(ROK);
}

#define WR_GET_LOC_TUNNEL_ID(_idx)\
do{\
   _idx = (++(_idx)) % WR_MAX_TUNNEL_ID; \
   if ((_idx) == 0) ++(_idx); \
}while(0);

PRIVATE S16 wrUmmS1SrcHoFillEnbSnStatusTfr
(
WrUmmTransCb                 *transCb,
S1apPdu                      **eNBStatusTfr
)
{
   S1apPdu                   *eNBStatusTfrPdu = NULLP;
   S16                       numComp = 0;
   S16                       compCnt = 0;
   S16                       ret = 0;
   S16                       idx = 0;
   U32                       pdcpHfn = 0;
   U16                       pdcpSn = 0;          
   U8                        rbCnt = 0;
   U8                        idx1 = 0;
   U16                       noOfBearer = 0;
   WrUeRAB                   *rbInfo = NULLP;
   SztBrs_SubjToStatusTfr_Item      *item = NULLP;
   NhuPdcpSduStaCfm                 *sduStaCfm = NULLP;

   SztProtIE_Field_ENBStatusTfrIEs *ie = NULLP;
   SztProtIE_SingleCont_Brs_SubjToStatusTfr_ItemIEs *ie1 = NULLP;

   sduStaCfm = transCb->msg->u.pdcpSduCfm;
   if(sduStaCfm->status != 0)
   {
      RLOG0(L_ERROR,"PDCP has sent negative Status");
      RETVALUE(RFAILED);
   }
   /* Initialize memory control point */
   ret = cmAllocEvnt(sizeof(S1apPdu), WR_SZ_MEM_SDU_SIZE, &wrCb.mem, 
         (Ptr *)&eNBStatusTfrPdu);
   if(ret != ROK)
   {
      RLOG0(L_FATAL, "Memory allocation failed.");
      RETVALUE(RFAILED);
   }
   cmMemset((U8 *)&(eNBStatusTfrPdu->pdu), 0, sizeof(SztS1AP_PDU));
   /* Fill the Interface Elementory Procedure for eNB Status Transfer Message.*/
   wrFillTknU8(&(eNBStatusTfrPdu->pdu.choice), S1AP_PDU_INITIATINGMSG);
   wrFillTknU8(&(eNBStatusTfrPdu->pdu.val.initiatingMsg.pres), PRSNT_NODEF);
   wrFillTknU32(&(eNBStatusTfrPdu->pdu.val.initiatingMsg.procedureCode), 
         Sztid_eNBStatusTfr);
   wrFillTknU32(&(eNBStatusTfrPdu->pdu.val.initiatingMsg.criticality), 
         SztCriticalityignoreEnum);

   numComp = 3;/* Indicates No of Mandatory Fields.*/
   wrFillTknU8(&(eNBStatusTfrPdu->pdu.val.initiatingMsg.value.u.
            sztENBStatusTfr.pres), PRSNT_NODEF);
   wrFillTknU16(&(eNBStatusTfrPdu->pdu.val.initiatingMsg.value.u.
            sztENBStatusTfr.protocolIEs.noComp), numComp);
   if ((cmGetMem(&eNBStatusTfrPdu->memCp, 
               (numComp * sizeof(SztProtIE_Field_ENBStatusTfrIEs)), 
               (Ptr *)&eNBStatusTfrPdu->pdu.val.initiatingMsg.value.u.
               sztENBStatusTfr.protocolIEs.member)) !=ROK)
   {
      RLOG0(L_FATAL, "Memory allocation failed.");
      WR_FREE_EVNT(eNBStatusTfrPdu);
      RETVALUE(RFAILED);
   }
   /* 
    * Fill the MME_UE_S1AP_ID which is allocated by MME to uniquely 
    * identify the UE within the MME.
    */
   ie = &eNBStatusTfrPdu->pdu.val.initiatingMsg.value.u.sztENBStatusTfr.
                                                  protocolIEs.member[compCnt++];
   wrFillTknU8(&(ie->pres), PRSNT_NODEF);
   wrFillTknU32(&(ie->id), Sztid_MME_UE_S1AP_ID);
   wrFillTknU32(&(ie->criticality), SztCriticalityrejectEnum);
   wrFillTknU32(&(ie->value.u.sztMME_UE_S1AP_ID), 
                                      transCb->ueCb->s1ConCb->mme_ue_s1ap_id);
   /* 
    * Fill ENB_UE_S1AP_ID which is allocated by Source eNB to uniquely 
    * identify the UE within the eNB.
    */
   ie = &eNBStatusTfrPdu->pdu.val.initiatingMsg.value.u.sztENBStatusTfr.
                                                  protocolIEs.member[compCnt++];
   wrFillTknU8(&(ie->pres), PRSNT_NODEF);
   wrFillTknU32(&(ie->id), Sztid_eNB_UE_S1AP_ID);
   wrFillTknU32(&(ie->criticality), SztCriticalityrejectEnum);
   wrFillTknU32(&(ie->value.u.sztENB_UE_S1AP_ID), 
                                        transCb->ueCb->s1ConCb->enb_ue_s1ap_id);
   /* 
    * Fill eNB Status Transfer Transparent Container which contains Bearer ID,
    * UL Count and DL Count value.
    */
   ie = &eNBStatusTfrPdu->pdu.val.initiatingMsg.value.u.sztENBStatusTfr.
                                                    protocolIEs.member[compCnt];
   wrFillTknU8(&(ie->pres), PRSNT_NODEF);
   wrFillTknU32(&(ie->id), Sztid_eNB_StatusTfr_TprntCont);
   wrFillTknU32(&(ie->criticality), SztCriticalityrejectEnum);
   wrFillTknU8(&(ie->value.u.sztENB_StatusTfr_TprntCont.pres), PRSNT_NODEF);
   noOfBearer = transCb->msg->u.pdcpSduCfm->numRbId;
   wrFillTknU16(&(ie->value.u.sztENB_StatusTfr_TprntCont.
                                bearers_SubjToStatusTfrLst.noComp),noOfBearer);
   /* fix for ccpu00133777 : changed size from SztBrs_SubjToStatusTfrLst
    * to SztProtIE_SingleCont_Brs_SubjToStatusTfr_ItemIEs*/
   if ((cmGetMem(&eNBStatusTfrPdu->memCp, 
               (noOfBearer * sizeof(SztProtIE_SingleCont_Brs_SubjToStatusTfr_ItemIEs)),
               (Ptr *)&ie->value.u.sztENB_StatusTfr_TprntCont.
               bearers_SubjToStatusTfrLst.member)) !=ROK)
   {
      RLOG0(L_FATAL, "Memory allocation failed.");
      WR_FREE_EVNT(eNBStatusTfrPdu);
      RETVALUE(RFAILED);
   }
   rbCnt = transCb->ueCb->rbInfo.numOfRbCfgd;
   rbInfo = transCb->ueCb->rbInfo.rab;
   for(idx = 0; idx < noOfBearer; idx++)
   {
      ie1= &ie->value.u.sztENB_StatusTfr_TprntCont.bearers_SubjToStatusTfrLst.
         member[idx];
      wrFillTknU8(&(ie1->pres),PRSNT_NODEF);
      wrFillTknU32(&(ie1->id), Sztid_Brs_SubjToStatusTfr_Item);
      wrFillTknU32(&(ie1->criticality),SztCriticalityignoreEnum);
      wrFillTknU8(&(ie1->value.u.sztBrs_SubjToStatusTfr_Item.pres),
            PRSNT_NODEF);
      /* Fill E-RAB ID which is allocated by CN to identify the bearer.*/
        /* HO_FIX: Correct the loop check as array is of size WR_MAX_DRBS */
      for(idx1 = 0; idx1 < WR_MAX_DRBS; idx1++)
      {
         if(transCb->msg->u.pdcpSduCfm->pdcpSduStaInfo[idx].drbId == 
                                                              rbInfo[idx1].rbId)
         {
            wrFillTknU32(&(ie1->value.u.sztBrs_SubjToStatusTfr_Item.e_RAB_ID),
                                                           rbInfo[idx1].eRABId);
            break;
         }
         }
      if(ie1->value.u.sztBrs_SubjToStatusTfr_Item.e_RAB_ID.pres == NOTPRSNT)
      {
         RLOG0(L_ERROR, "Received SN info for an UNKNOWN RB ID");
         WR_FREE_EVNT(eNBStatusTfrPdu);
         RETVALUE(RFAILED);
      }
      item = &ie1->value.u.sztBrs_SubjToStatusTfr_Item;
      /* Check the SN bit */
      if(sduStaCfm->pdcpSduStaInfo[idx].snLen == NHU_PDCP_SN_SIZE_12BITS)
      {
         /* Fill UL_COUNT_VALUE */
         wrFillTknU8(&(item->uL_COUNTvalue.pres),PRSNT_NODEF);
         pdcpSn = ((sduStaCfm->pdcpSduStaInfo[idx].ulCountVal) & (0x0fff));
         wrFillTknU32(&(item->uL_COUNTvalue.pDCP_SN), pdcpSn);
         /* Fill HFN */
         pdcpHfn = ((sduStaCfm->pdcpSduStaInfo[idx].ulCountVal >> 12) & (0xfffff));
         wrFillTknU32(&(item->uL_COUNTvalue.hFN), pdcpHfn);

         RLOG3(L_DEBUG, "UL pdcp sn[%d] pdcp hfn[%ld] count[%ld]",pdcpSn,
               pdcpHfn, sduStaCfm->pdcpSduStaInfo[idx].ulCountVal);

         wrFillTknU8(&(item->dL_COUNTvalue.pres),PRSNT_NODEF);
         /* Fill DL_COUNT_VALUE */
         pdcpSn = ((sduStaCfm->pdcpSduStaInfo[idx].dlCountVal) & (0x0fff));
         wrFillTknU32(&(item->dL_COUNTvalue.pDCP_SN), pdcpSn);
         pdcpHfn = ((sduStaCfm->pdcpSduStaInfo[idx].dlCountVal >> 12) & (0xfffff));
         wrFillTknU32(&(item->dL_COUNTvalue.hFN), pdcpHfn);

         RLOG3(L_DEBUG, "DL pdcp sn[%d] pdcp hfn[%ld] count[%ld]",pdcpSn,
               pdcpHfn, sduStaCfm->pdcpSduStaInfo[idx].dlCountVal);

         /* Fill receive status of UL SDU's */
         if((sduStaCfm->pdcpSduStaInfo[idx].rcvStaOfUlPdcpSdus.numBits != 0) &&
               (transCb->u.s1SrcHoTrans.rbInfo[idx].isULDatFwdReq == TRUE))
         {
            /*ccpu00133566*/
            /* PDCP will send numBits as multiple of 8 by padding UL BitMap 
             * with 0s to make it Byte Aligned.*/
            wrFillTknStr4096BSXL(&(item->receiveStatusofULPDCPSDUs),
                  sduStaCfm->pdcpSduStaInfo[idx].rcvStaOfUlPdcpSdus.numBits,
                  sduStaCfm->pdcpSduStaInfo[idx].rcvStaOfUlPdcpSdus.ulBitMap,
                  &eNBStatusTfrPdu->memCp);
         }
         item->iE_Extns.noComp.pres = NOTPRSNT;
      }
      else if(sduStaCfm->pdcpSduStaInfo[idx].snLen == NHU_PDCP_SN_SIZE_15BITS)
      {
         SztProtExtnCont_Brs_SubjToStatusTfr_ItemExtIEs *extIEs;
         SztProtExtnField_Brs_SubjToStatusTfr_ItemExtIEs *newIE;

         wrFillTknU8(&(item->uL_COUNTvalue.pres),PRSNT_NODEF);
         wrFillTknU32(&(item->uL_COUNTvalue.pDCP_SN), 0);
         wrFillTknU32(&(item->uL_COUNTvalue.hFN), 0);

         wrFillTknU8(&(item->dL_COUNTvalue.pres),PRSNT_NODEF);
         wrFillTknU32(&(item->dL_COUNTvalue.pDCP_SN), 0);
         wrFillTknU32(&(item->dL_COUNTvalue.hFN), 0);

         /* Filling Extension values */
         extIEs = &item->iE_Extns;
         extIEs->noComp.pres = PRSNT_NODEF;
         extIEs->noComp.val = 3;

         if ((cmGetMem(&eNBStatusTfrPdu->memCp, 
                     (extIEs->noComp.val * sizeof(SztProtExtnField_Brs_SubjToStatusTfr_ItemExtIEs)), 
                     (Ptr *)&extIEs->member)) !=ROK)
         {
            RLOG0(L_FATAL, "Failed to allocate memory.cmGetMem Failed");
            WR_FREE_EVNT(eNBStatusTfrPdu);
            RETVALUE(RFAILED);
         }
         /* Filling UL Count Value extended */
         newIE = &extIEs->member[0];
         wrFillTknU8(&(newIE->pres),PRSNT_NODEF);
         wrFillTknU32(&(newIE->id), Sztid_ULCOUNTValueExtended);
         wrFillTknU32(&(newIE->criticality),SztCriticalityignoreEnum);
         wrFillTknU8(&(newIE->extensionValue.u.sztid_ULCOUNTValueExtended.pres),
               PRSNT_NODEF);
         pdcpSn = ((sduStaCfm->pdcpSduStaInfo[idx].ulCountVal) & (0x0fff));
         pdcpHfn = ((sduStaCfm->pdcpSduStaInfo[idx].ulCountVal >> 15) & (0xfffff));
         wrFillTknU32(&(newIE->extensionValue.u.sztid_ULCOUNTValueExtended.pDCP_SNExtended), 
               pdcpSn);
         wrFillTknU32(&(newIE->extensionValue.u.sztid_ULCOUNTValueExtended.hFNMdfd), 
               pdcpHfn);
         RLOG3(L_DEBUG, "Extended:UL pdcp sn[%d] pdcp hfn[%ld] count[%ld]",pdcpSn,
               pdcpHfn, sduStaCfm->pdcpSduStaInfo[idx].ulCountVal);

         /* Filling DL Count Value extended */
         newIE = &extIEs->member[1];
         wrFillTknU8(&(newIE->pres),PRSNT_NODEF);
         wrFillTknU32(&(newIE->id), Sztid_DLCOUNTValueExtended);
         wrFillTknU32(&(newIE->criticality),SztCriticalityignoreEnum);
         wrFillTknU8(&(newIE->extensionValue.u.sztid_DLCOUNTValueExtended.pres),
               PRSNT_NODEF);
         pdcpSn = ((sduStaCfm->pdcpSduStaInfo[idx].dlCountVal) & (0x0fff));
         pdcpHfn = ((sduStaCfm->pdcpSduStaInfo[idx].dlCountVal >> 15) & (0xfffff));
         wrFillTknU32(&(newIE->extensionValue.u.sztid_DLCOUNTValueExtended.pDCP_SNExtended), 
               pdcpSn);
         wrFillTknU32(&(newIE->extensionValue.u.sztid_DLCOUNTValueExtended.hFNMdfd), 
               pdcpHfn);
         RLOG3(L_DEBUG, "Extended:DL pdcp sn[%d] pdcp hfn[%ld] count[%ld]",pdcpSn,
               pdcpHfn, sduStaCfm->pdcpSduStaInfo[idx].dlCountVal);

         /* Filling receive status of UL SDU's extended */
         newIE = &extIEs->member[2];
         if((sduStaCfm->pdcpSduStaInfo[idx].rcvStaOfUlPdcpSdus.numBits != 0) &&
               (transCb->u.s1SrcHoTrans.rbInfo[idx].isULDatFwdReq == TRUE))
         {
            wrFillTknU8(&(newIE->pres),PRSNT_NODEF);
            wrFillTknU32(&(newIE->id), Sztid_ReceiveStatusOfULPDCPSDUsExtended);
            wrFillTknU32(&(newIE->criticality),SztCriticalityignoreEnum);
            /* PDCP will send numBits as multiple of 8 by padding UL BitMap 
             * with 0s to make it Byte Aligned. */
            wrFillTknStr16384BSXL(&(newIE->extensionValue.u.sztReceiveStatusOfULPDCPSDUsExtended),
                  sduStaCfm->pdcpSduStaInfo[idx].rcvStaOfUlPdcpSdus.numBits,
                  sduStaCfm->pdcpSduStaInfo[idx].rcvStaOfUlPdcpSdus.ulBitMap,
                  &eNBStatusTfrPdu->memCp);
         }
         else
         {
            wrFillTknU8(&(newIE->pres), NOTPRSNT);
         }
      }
   }/* End of for(idx = 0; idx < sztBrs_SubjToStatusTfrLst->noComp; idx++)*/
   ie->value.u.sztENB_StatusTfr_TprntCont.iE_Extns.noComp.pres = NOTPRSNT;

   *eNBStatusTfr = eNBStatusTfrPdu;

   RETVALUE(ROK);
}


PRIVATE S16 wrUmmS1SrcHoSndEnbSnStaTfr
(
WrUmmTransCb                 *transCb
)
{
   SztDatEvntReq             datEvntReq;
   S1apPdu                   *eNBStatusTfr = NULLP;

   /*  HOFIX */
   RLOG_ARG0(L_DEBUG, DBG_CRNTI, transCb->ueCb->crnti,"Received PDCP SN Status "
      "information");
   /* Fix for ccpu00146961 */
    if (( SztHovrTypintralteEnum == transCb->u.s1SrcHoTrans.sztHovrTyp->val) && 
        (transCb->u.s1SrcHoTrans.numOfRbInfo > 0))
    {
       if (ROK != wrUmmS1SrcHoFillEnbSnStatusTfr(transCb,&eNBStatusTfr))
       {
          RLOG0(L_ERROR, "Failed To Fill Enb Status Transfer Message");
          RETVALUE(RFAILED);
       }
       RLOG_ARG2(L_EVENT, DBG_CRNTI, transCb->ueCb->crnti,"Sending S1 eNB STATUS "
             "TRANSFER, with MME-UE-S1AP-ID[%lu] eNB-UE-S1AP-ID[%lu]",
             transCb->ueCb->s1ConCb->mme_ue_s1ap_id,
             transCb->ueCb->s1ConCb->enb_ue_s1ap_id);
       datEvntReq.spConnId = transCb->ueCb->s1ConCb->spConnId;
       datEvntReq.pdu = eNBStatusTfr;
       WrIfmS1apDatReq(&datEvntReq);

       /*ccpu00134977 : Tunnel state should be changed to 
        * WR_DAM_TNL_DL_FWDSTATE_NEW only after forwarding 
        * the buffered PDCP packtes*/

    }  
    /* Send Data Forwarding Requets to RRC */
    RLOG0(L_DEBUG, "Sending PDCP Data Forward Req");
    wrUmmS1SrcHoSendStartPdcpDataFwdReq(transCb);
    RETVALUE(ROK);
}


PRIVATE S16 wrUmmS1SrcHoPrcInitiatingMsgs
(
WrUmmTransCb                 *transCb
)
{
   U32                       msgVal;
   WrUmmIncMsg               *msg = transCb->msg;
   SztInitiatingMsg          *initiatingMsg = NULLP;
   /* ccpu00131919 */
   S1apPdu        *locRptFail;

   initiatingMsg = &msg->u.s1Pdu->pdu->pdu.val.initiatingMsg;
   msgVal = initiatingMsg->procedureCode.val;
   switch (msgVal)
   {
         /* ccpu00131919 */
      case Sztid_LocRprtngCntrl:
         {
            /* When Ho is in progress and if we receive LocRptCtrl Msg, we should send LocRptFail */
            RLOG_ARG2(L_EVENT, DBG_CRNTI,transCb->ueCb->crnti,"[S1AP]:Sending LOCATION REPORTING"
                      "FAILURE INDICATION [MME-UE-S1AP-ID:%lu] [eNB-UE-S1AP-ID:%lu] ",
                      transCb->ueCb->s1ConCb->mme_ue_s1ap_id, transCb->ueCb->s1ConCb->enb_ue_s1ap_id);
            if (ROK != wrUmmS1apBldLocReportFail(transCb->ueCb, &locRptFail))
            {
               RLOG0(L_ERROR, "Failed to Process Location Report Message");
               RETVALUE(RFAILED);
            }
            wrIfmS1apSndUeMsg(transCb->ueCb->s1ConCb->spConnId, locRptFail);
            break;
         }
      default :
         {
            RLOG1(L_ERROR, "Not handling the Initiating Message[%ld]",msgVal);
            RETVALUE(RFAILED);
         }
   }
   RETVALUE(ROK);
}

PRIVATE S16 wrUmmS1SrcHoPrcSuccessfulOutcomeMsgs
(
WrUmmTransCb                 *transCb
)
{
   U32                       msgVal;
   WrUmmIncMsg               *msg = transCb->msg;
   SztSuccessfulOutcome      *successfulOutcome = NULLP;
   Bool                      isTmrRunning = FALSE;
   successfulOutcome = &msg->u.s1Pdu->pdu->pdu.val.successfulOutcome;
   msgVal = successfulOutcome->procedureCode.val;

   switch (msgVal)
   {
      case Sztid_HovrPrep:
         {
            isTmrRunning = wrIsTmrRunning(&transCb->u.s1SrcHoTrans.s1RelocTmr.tmr, 
                  WR_TMR_S1_RELOC_TMR);
            if(TRUE == isTmrRunning)
            {
               if (ROK != wrUmmS1SrcHoPrcHovrCmd(transCb))
               {
                  RLOG0(L_ERROR, "Failed to Process SuccessfuloutCome Message");
                  RETVALUE(RFAILED);
               }
            }
            else
            {
               RLOG1(L_ERROR, "Discarding HO Command Msg, since CANCEL is triggered [%ld]",msgVal);
            }
            break;
         }
      case Sztid_HovrCancel:
         {
            wrStopTransTmr(transCb,WR_TMR_SRC_HO_CANCEL);
            WR_UMM_S1_SRC_HANDLE_HO_FAIL(transCb);
            wrUmmTransComplete(transCb);
            break;
         }
      default :
         RLOG1(L_ERROR, "Not handling the Successfuloutcome Msg[%ld]",msgVal);
         RETVALUE(RFAILED);
   }/* End of switch */
   RETVALUE(ROK);
}

PRIVATE S16 wrUmmS1SrcHoPrcHoPrepFailure
(
WrUmmTransCb                 *transCb
)
{
   SztS1AP_PDU                     *s1apPdu = NULLP;
   SztHovrPrepFail                 *sztHovrPrepFail = NULLP;
   SztProtIE_Field_HovrPrepFailIEs *hoPrepFailIe = NULLP;
   U16                             hoPrepFailNoComp = 0;
   U16                             hoPrepFailIdx = 0;
   U32                             ieId;
   U32                             mme_Ue_S1ap_Id;
   U32                             enb_Ue_S1ap_Id;
   
   /* if Timer is running, Need to stop.*/
   wrStopTransTmr(transCb, WR_TMR_S1_RELOC_TMR);   
   /* Upon reception of Handover Preparation Failure Message,eNB shall release */
   /* corresponding transaction and inform to Ho decision moule about handover */
   /* failure.                                                                 */ 
   mme_Ue_S1ap_Id = transCb->ueCb->s1ConCb->mme_ue_s1ap_id;
   enb_Ue_S1ap_Id = transCb->ueCb->s1ConCb->enb_ue_s1ap_id;

   s1apPdu = &transCb->msg->u.s1Pdu->pdu->pdu;
   sztHovrPrepFail = &s1apPdu->val.unsuccessfulOutcome.value.u.sztHovrPrepFail;

   hoPrepFailNoComp = sztHovrPrepFail->protocolIEs.noComp.val;
   for (hoPrepFailIdx = 0; hoPrepFailIdx < hoPrepFailNoComp; hoPrepFailIdx++)
   {
      hoPrepFailIe = &sztHovrPrepFail->protocolIEs.member[hoPrepFailIdx];
      ieId = hoPrepFailIe->id.val;
      RLOG_ARG2(L_DEBUG, DBG_CRNTI, transCb->ueCb->crnti,"Received S1 HANDOVER "
         "PREPARATION FAILURE, MME-UE-S1AP-ID[%lu] eNB-UE-S1AP-ID[%lu]",
         mme_Ue_S1ap_Id, enb_Ue_S1ap_Id);
      switch (ieId)
      {
         case Sztid_MME_UE_S1AP_ID:
         {
            if (hoPrepFailIe->value.u.sztMME_UE_S1AP_ID.val != mme_Ue_S1ap_Id)
            {
               RLOG0(L_ERROR, "MME UE S1AP ID Is Invalid");
               RETVALUE(RFAILED);
            }
            break;
         }
         case Sztid_eNB_UE_S1AP_ID:
         {
            if (hoPrepFailIe->value.u.sztENB_UE_S1AP_ID.val != enb_Ue_S1ap_Id)
            {
               RLOG0(L_ERROR, "ENB UE S1AP ID Is Invalid");
               RETVALUE(RFAILED);
            }
            break;
         }
         case Sztid_Cause:
         {
            wrEmmUpdateKpiCounter(&hoPrepFailIe->value.u.sztCause);
            /* CSG_DEV start */
            if(WR_CAUSE_RADIONW == hoPrepFailIe->value.u.sztCause.choice.val &&
                  (SztCauseRadioNwinvalid_CSG_IdEnum == 
                   hoPrepFailIe->value.u.sztCause.val.radioNw.val))
            {
               wrUmmCsgAddCsgIdIntoBlckLst(transCb->ueCb,
                     transCb->u.s1SrcHoTrans.csgId,
                     transCb->u.s1SrcHoTrans.tgtPci);
            }
            /* CSG_DEV end */
            break;
         }
         default:
            RETVALUE(RFAILED);
      }/* End of switch*/
   }/* End of for loop*/

   RLOG1(L_INFO, "Received Handover Preparation message for UE crnti %d \n", transCb->ueCb->crnti);
   WR_UMM_S1_SRC_HANDLE_HO_FAIL(transCb);   
   wrUmmTransComplete(transCb);

   RETVALUE(ROK);
}

PRIVATE S16 wrUmmS1SrcHoPrcUnSuccessfulOutcomeMsgs
(
WrUmmTransCb                 *transCb
)
{
   U32                  msgVal;
   WrUmmIncMsg            *msg = transCb->msg;
   SztUnsuccessfulOutcome *unSuccMsg = NULLP;
   Bool                      isTmrRunning = FALSE;

   unSuccMsg = &msg->u.s1Pdu->pdu->pdu.val.unsuccessfulOutcome;
   msgVal = unSuccMsg->procedureCode.val;
   switch(msgVal)
   {
      case Sztid_HovrPrep:
         {
            isTmrRunning = wrIsTmrRunning(&transCb->u.s1SrcHoTrans.s1RelocTmr.tmr, 
                  WR_TMR_S1_RELOC_TMR);
            if( TRUE == isTmrRunning)
            {
               if (ROK != wrUmmS1SrcHoPrcHoPrepFailure(transCb))
               {
                  RLOG0(L_ERROR, "Failed to Process Ho Prep Failure");
                  RETVALUE(RFAILED);
               }
            }
            else
            {
               RLOG1(L_ERROR, "Discarding HO Failure Msg, since CANCEL is triggered [%ld]",msgVal);
            }
            break;
         }
      default:
      {
         RLOG1(L_ERROR, "Not handling Unsuccessfuloutcome Message [%ld]",msgVal);
         RETVALUE(RFAILED);
      }
   }/* End of switch msgVal*/
   
   RETVALUE(ROK);
}

PRIVATE S16 wrUmmS1SrcHoHandleUlDcchMsgs(WrUmmTransCb *transCb)
{    
   NhuUL_DCCH_MsgTyp                 *msgTyp = NULLP;
   NhuUlDcchMsg                          *ulDcch = &transCb->msg->u.uuIndPdu->sdu.m.ulDcchMsg;
   NhuUL_DCCH_MsgTypc1                   *rrcMsg;
   NhuUECapInfo                          *ueCapInfo;
   NhuEncReqSdus                         *hoPrepEncMsg = NULLP;
   WrUeCb                                 *ueCb = transCb->ueCb;
   U32                                    rrcTransId = 0;

   msgTyp = &transCb->msg->u.uuIndPdu->sdu.m.ulDcchMsg.dcchMsg.message;
   rrcMsg = &ulDcch->dcchMsg.message.val.c1;
   switch (msgTyp->choice.val)
   {
      case UL_DCCH_MSGTYP_C1:
         {
            switch (msgTyp->val.c1.choice.val)
            {
#if 0 /* Unused code removing it */
               case C1_MEASUREMENTREPORT:
                  {
                     RLOG0(L_ERROR, "Got a measurment Report during HO and "
                        "dropping it");
                     break;
                  }
               case C1_RRCCONRECFGNCOMPL:
                  {
#if 0 /*130053*/                  
              rrcTransId =  c1->val.rrcConRecfgnCompl.rrc_TrnsxnId.val;
              wrUmmRelRrcTransId(transCb->ueCb, C1_RRCCONRECFGN, rrcTransId);
                  wrUmmDrxRelSchdUeRecfgReq(transCb);
#endif /* 0 */                  
                     break;
                  }
#endif
               case C1_UECAPINFO: 
                  {
                     if( ueCb->ueCap != NULL )
                     {   
                        RLOG_ARG2(L_EVENT, DBG_CRNTI, transCb->ueCb->crnti,"[OTA] Received UECapabilityInformation"
                              " ue-Category [%d] accessStratumRelease [%u]", ueCb->ueCat, ueCb->ueCap->eUTRA_Cap.accessStratumRls.val);
                     }
                     else
                     {
                        RLOG_ARG1(L_EVENT, DBG_CRNTI, transCb->ueCb->crnti,"[OTA] Received UECapabilityInformation"
                              " ue-Category [%d] ", ueCb->ueCat);
                     }
                     ueCapInfo = &(rrcMsg->val.ueCapInfo);
                     rrcTransId = ueCapInfo->rrc_TrnsxnId.val;
                     if (wrUmmRelRrcTransId(ueCb, C1_UECAPENQUIRY, 
                              rrcTransId) != ROK)
                     {
                        /*ccpu00127802*/
                        RLOG0(L_WARNING,"Release of transaction ID failed");
                     }
                     /*Stop Radio Capability Enquiry  Timer*/
                     wrStopTransTmr(transCb,WR_TMR_RRC_CAPENQ);
                     if ( ROK != wrUmmS1HoUeCapStoreUtraCapInfo(transCb, ueCapInfo))
                     {
                        RLOG0(L_ERROR,"wrUmmS1HoUeCapStoreUtraCapInfo failed");
                        transCb->state = WR_UMM_TRANS_DONE;
                        RETVALUE(RFAILED);
                     } 
                     /* Preprare UTRA RRC Container */ 
                     WR_ALLOCEVNT(&hoPrepEncMsg, sizeof(NhuEncReqSdus));
                     if(NULLP == hoPrepEncMsg)
                     {
                        RLOG0(L_FATAL,"Memory allocation failed.");
                        RETVALUE(RFAILED);
                     }
                     /* Fill RRC Encode Request Header */
                     hoPrepEncMsg->hdr.cellId = ueCb->cellId;
                     hoPrepEncMsg->hdr.ueId = ueCb->crnti;
                     hoPrepEncMsg->hdr.transId = transCb->transId | WR_UMM_SRC_HO_UTRA_CONT_ENC;
                     hoPrepEncMsg->sdu.msgCategory = NHU_ENC_SDUTYPE_UTRA_RRC_CONT;

                     /* fill transparent container */
                     wrUmmUtraRrcFillSrcToTgetIe(ueCb, hoPrepEncMsg, 
                                    &(transCb->u.s1SrcHoTrans.t.interRatInfo.utraUeCapContainer));
                     if(ROK != wrIfmRrcSndEncodeReq(hoPrepEncMsg))
                     {
                        RLOG0(L_ERROR,"Failed to send Encode Req to RRC");
                        RETVALUE(RFAILED);
                     }
                  }
                  break;
               default :
                  RLOG1(L_ERROR, "Invalid Msg Type[%d] Received.",
                     msgTyp->val.c1.choice.val);
                  RETVALUE(RFAILED);
            }/* End of switch.*/
            break;
         }
      case UL_DCCH_MSGTYP_MESSAGECLASSEXTN:
         break;
      default :
         RLOG0(L_ERROR, "Invalid Message Choice.");
         RETVALUE(RFAILED);
   }/* End of switch*/

   RETVALUE(ROK);
}


PRIVATE S16 wrUmmS1SrcHoHandleRRCMsgs
(
WrUmmTransCb                 *transCb
)
{
   NhuDatIndSdus            *nhuDatIndSdu = NULLP;

   nhuDatIndSdu = transCb->msg->u.uuIndPdu;

   switch (nhuDatIndSdu->sdu.msgCategory)
   {
      case NHU_MSG_ULDCCH:
         /* Handling of UL DCCH Messages*/
         if (ROK != wrUmmS1SrcHoHandleUlDcchMsgs(transCb))
         {
            RLOG0(L_ERROR, "Failed To Handle UL DCCH Msg.");
            RETVALUE(RFAILED);
         }/* wrUmmHoHandleUlDcchMsgs*/
         break;
      default:
         RLOG0(L_ERROR, "Invalid Message Category.");
         RETVALUE(RFAILED);
   }/* End of Switch*/
   RETVALUE(ROK);
}

/*
  * This function is used to handle s1ap PDU.
  */
PRIVATE S16 wrUmmS1SrcHoHandleS1apPdu(WrUmmTransCb *transCb)
{
   WrUmmIncMsg               *msg = transCb->msg;
   U32                       msgVal;

   msgVal = msg->u.s1Pdu->pdu->pdu.choice.val;
   switch(msgVal)
   {
      case INITIATING_MSG:
      {
         if (ROK != wrUmmS1SrcHoPrcInitiatingMsgs(transCb))
         {
            RLOG0(L_ERROR, "Failed To handle initiating message");
            RETVALUE(RFAILED);
         }
         break;
      }
      case SUCCESSFUL_OUTCOME:
      {
         if (ROK != wrUmmS1SrcHoPrcSuccessfulOutcomeMsgs(transCb))
         {
            RLOG0(L_ERROR, "Failed To handle successful outcome.");
            RETVALUE(RFAILED);
         }
         break;
      }
      case UN_SUCCESSFUL_OUTCOME:
      {
         if (ROK != wrUmmS1SrcHoPrcUnSuccessfulOutcomeMsgs(transCb))
         {
            RLOG0(L_ERROR, "Failed To handle unsuccessful outcome");
            RETVALUE(RFAILED);
         }
         break;
      }    
      default :
         RLOG0(L_ERROR, "Invalid Message");
         RETVALUE(RFAILED);
    }/*End of switch.*/
    RETVALUE(ROK);
}/* End of wrUmmS1SrcHoHandleS1apPdu*/


PRIVATE S16 wrUmmS1SrcHoInitTrans
(
WrUmmTransCb                 *transCb,
WrUmmIntCrHoTran             *internalMsg
)
{
   WrUmmS1SrcHoTrans         *hoTrans = &transCb->u.s1SrcHoTrans;
   WrUmmIntCrIntraLteHoTranInfo    *intraLteHoInfo = 
                                    &(internalMsg->t.intraLte);
   WrUmmIntCrIRatUtraHoTranInfo    *iRatUtraHoInfo = 
                                    &(internalMsg->t.interRatUtra);
   WrUmmIntCrIRatGeranHoTranInfo    *iRatGeranHoInfo = 
                                    &(internalMsg->t.interRatGeran);
   WrUmmIntraLteHoInfo             *intraLteHoTransInfo = 
                                    &(hoTrans->t.intraLteInfo); 
   WrUmmIRatUtraInfo               *iRatUtraHoTransInfo = 
                                    &(hoTrans->t.interRatInfo);
   WrUmmIRatGeranInfo               *iRatGeranHoTransInfo = 
                                    &(hoTrans->t.interRatGeranInfo);

   /*ccpu133818 : mobCtrlState is changed here  
    * inorder to prevent multiple HO requests
    * in case if both inter as well as intra 
    * meas report is received at the same time*/
   transCb->ueCb->mobCtrlState = WR_HO_INITIATED;
   transCb->ueCb->hoTransId    = transCb->transId;

 
   hoTrans->crnti            = internalMsg->crnti;
   hoTrans->cellId           = internalMsg->cellId;
   /* Fill Handover Cause  */
   WR_ALLOC(&hoTrans->sztCause,sizeof(SztCause));
   if (NULLP == hoTrans->sztCause)
   {
      RLOG0(L_FATAL, "Memory allocation failed.");
       RETVALUE(RFAILED);
   }
   hoTrans->sztCause->choice.pres       = TRUE;

   hoTrans->sztCause->choice.val        = internalMsg->hoCause.causeTyp;
   hoTrans->sztCause->val.radioNw.val   = internalMsg->hoCause.causeVal;

   /* Allocate memory for HO Type*/
   WR_ALLOC(&hoTrans->sztHovrTyp, sizeof(SztHovrTyp));
   if (NULLP == hoTrans->sztHovrTyp)
   {
      RLOG0(L_FATAL, "Memory allocation failed.");
       WR_FREE(hoTrans->sztCause, sizeof(SztCause));
       RETVALUE(RFAILED);
   }

   /* Fill Intra LTE and IRAT HO Specific parameters */ 
   switch ( internalMsg->hoType )
   {
      case WR_HO_TYPE_INTRA_LTE_S1:
         {

            transCb->ueCb->hoType       = WR_HO_TYPE_INTRA_LTE_S1;

            hoTrans->sztHovrTyp->val = SztHovrTypintralteEnum;
            /* CSG_DEV start */
            /* update CSG info */
            hoTrans->csgId = internalMsg->csgId;
            hoTrans->csgCellAccessMode = internalMsg->csgCellAccessMode;
            hoTrans->tgtPci  = internalMsg->t.intraLte.pci;
            /* CSG_DEV end */
            intraLteHoTransInfo->tac = intraLteHoInfo->tac;
            intraLteHoTransInfo->isDirectPathAvl = 
               intraLteHoInfo->isX2InterFace;
            intraLteHoTransInfo->eNBType = intraLteHoInfo->enbType;
            /* Copy PLMNID from intraLteHoInfo to HO transCb */ 
            wrUtlCopyPlmnId( &(intraLteHoInfo->ecgi.plmnId), &(hoTrans->plmnId));

            /* FIll ECGI */
            intraLteHoTransInfo->ecgi.eutranCellId = 
               intraLteHoInfo->ecgi.eutranCellId;
            wrUtlCopyPlmnId(&hoTrans->plmnId, &intraLteHoTransInfo->ecgi.plmnId);
            break;
         }
      case WR_HO_TYPE_IRAT_UTRA:
      {

         transCb->ueCb->hoType       = WR_HO_TYPE_IRAT_UTRA;
         hoTrans->sztHovrTyp->val   = SztHovrTypltetoutranEnum;
         iRatUtraHoTransInfo->psc   = iRatUtraHoInfo->psc;
         iRatUtraHoTransInfo->rncId = iRatUtraHoInfo->rncId;
         iRatUtraHoTransInfo->cId   = iRatUtraHoInfo->cId;
         iRatUtraHoTransInfo->lac   = iRatUtraHoInfo->lac;
         iRatUtraHoTransInfo->rac   = iRatUtraHoInfo->rac;
         /* Copy PLMNID from iRatUtraHoTransInfo to HO transCb */ 
         wrUtlCopyPlmnId( &(iRatUtraHoInfo->plmnId), &(hoTrans->plmnId));
         /* Copy SRVCC Indication IE */
         hoTrans->srvccHoInd = iRatUtraHoInfo->srvccHoInd;
         break;
      }
      case WR_HO_TYPE_IRAT_GERAN:
      {
         transCb->ueCb->hoType       = WR_HO_TYPE_IRAT_GERAN;
         hoTrans->sztHovrTyp->val    = SztHovrTypltetogeranEnum;
         iRatGeranHoTransInfo->bsic   = iRatGeranHoInfo->bsic;
         iRatGeranHoTransInfo->cId   = iRatGeranHoInfo->cId;
         iRatGeranHoTransInfo->lac   = iRatGeranHoInfo->lac;
         iRatGeranHoTransInfo->rac   = iRatGeranHoInfo->rac;
         iRatGeranHoTransInfo->psSrvcNotAvlbl = iRatGeranHoInfo->psSrvcNotAvlbl;
         iRatGeranHoTransInfo->bandIndicator = iRatGeranHoInfo->bandIndicator;

         /* Copy PLMNID from iRatGeranHoTransInfo to HO transCb */ 
         wrUtlCopyPlmnId( &(iRatGeranHoInfo->plmnId), &(hoTrans->plmnId));
         /* Copy SRVCC Indication IE */
         hoTrans->srvccHoInd = iRatGeranHoInfo->srvccHoInd;
         iRatGeranHoTransInfo->rxLev = iRatGeranHoInfo->rssi;
         break;
      }
      default:
      {
         RLOG0(L_ERROR,"Invalid/un-supported HO type");
         RETVALUE(RFAILED);
      }
   }

   RETVALUE(ROK);
}

/*UE History info - start*/
/**
 * @brief 
 *    This function is used to Fill Extended Last visited E-UTRAN Cell Info
 *
 * @details
 *    Function: wrUmmFillLastVisitedEUTRANCellInform_ExtIEs
 *
 *    Processing steps:
 *       -# Fill Time UE Stayed in Cell Enhanced Granularity
 *              
 * @param[in]
 *    iePdu        : Event structure pointer
 * @param[out]
 *    eUTRANCell   : Structure Last Visited EUTRAN Cell Information
 * @param[in]
 *    ueTime         : Time UE Stayed in Cell Enhanced Granularity
 * @return S16
 *    -#Success : ROK
 *    -#Failure : RFAILED
 */
PRIVATE S16 wrUmmFillLastVisitedEUTRANCellInform_ExtIEs
(
SzIePdu                         *iePdu,
SztLastVisitedEUTRANCellInform  *eUTRANCell,
U32                             ueTime
)
{
   U32   noExtnMembers;

   /* Filling only Time UE stayed in Cell Enhanced Granularity member */
   noExtnMembers = 1;
   /* Allocate the memory for Extension IEs.*/
   if ((cmGetMem(&iePdu->memCp, 
			(noExtnMembers * sizeof(SztProtExtnField_LastVisitedEUTRANCellInform_ExtIEs)),
			((Ptr *)&eUTRANCell->iE_Extns.member))) !=ROK)
   {
      RLOG0(L_FATAL,"cmGetMem failed in Filling Time UE stayed in Cell Enhanced Granularity\n");
      RETVALUE(RFAILED);
   }
   wrFillTknU16(&(eUTRANCell->iE_Extns.noComp), noExtnMembers);

   /* Filling Time UE stayed in Cell Enhanced Granularity. */
   wrFillTknU8(&(eUTRANCell->iE_Extns.member->pres), PRSNT_NODEF);
   wrFillTknU32(&(eUTRANCell->iE_Extns.member->id), Sztid_Time_UE_StayedInCell_EnhancedGranularity);
   wrFillTknU32(&(eUTRANCell->iE_Extns.member->criticality), SztCriticalityignoreEnum);
   wrFillTknU32(&(eUTRANCell->iE_Extns.member->extensionValue.u.sztTime_UE_StayedInCell_EnhancedGranularity), ueTime);
   
   RETVALUE(ROK);
}

/**
 * @brief 
 *    This function is used to Fill UE History Information
 *
 * @details
 *    Function: wrUmmS1SrcHoFillUeHistoryInform
 *
 *    Processing steps:
 *       -# Fill UE History Information
 *              
 * @param[in]
 *    iePdu            : Event structure pointer
 * @param[in]
 *    transCb          : transaction control information
 * @return S16
 *    -#Success : ROK
 *    -#Failure : RFAILED
 */
PRIVATE S16 wrUmmS1SrcHoFillUeHistoryInform
(
SzIePdu                         *iePdu,
WrUmmTransCb                    *transCb
)
{
   SztUE_HistoryInform             *ueHistoryInform = NULLP ; 
   SztLastVisitedCell_Item         *lastVstCelInfo = NULLP;
   SztLastVisitedEUTRANCellInform  *eUTRANCell = NULLP;
   WrUeCb                          *ueCb = transCb->ueCb;
   WrUeHstryEutranInfo             *eutraCellInfo;
   EpcTime                         currentTime;
   U32                             cellInfoIdx = 0;
   TknStrOSXL                      *utraCellInfo;
   SztLastVisitedUTRANCellInform   *uTRANCell = NULLP;
   U32                             ueTime = 0;
   U32                             ueHistryCompSize;
   U8                              ueHistryIdx;
   U8                              cellSizeType;
   WrCgi                           cgi;

   ueHistoryInform = &iePdu->u.srcToTget.uE_HistoryInform; 

   ueHistryCompSize = ueCb->ueHstryInfo.numOfCells + 1;
   if(ueHistryCompSize > WR_UMM_UE_HISTORY_MAX_CELLS)
   {
     ueHistryCompSize = WR_UMM_UE_HISTORY_MAX_CELLS;   
   }
   wrFillTknU16(&(ueHistoryInform->noComp),ueHistryCompSize);
   /* Allocate the memory for transparent containier IEs.*/
   if ((cmGetMem(&iePdu->memCp, 
			(ueHistryCompSize * sizeof(SztLastVisitedCell_Item)),
			((Ptr *)&ueHistoryInform->member))) !=ROK)
   {
      RLOG0(L_FATAL, "Memory allocation failed.");
      RETVALUE(RFAILED);
   }

   /* Update the current cell details in the history */

   /* Source Info */
   lastVstCelInfo = &(ueHistoryInform->member[0]);
   wrFillTknU8(&(lastVstCelInfo->choice),LASTVISITEDCELL_ITEM_E_UTRAN_CELL);

   eUTRANCell = &lastVstCelInfo->val.e_UTRAN_Cell;
   wrFillTknU8(&(eUTRANCell->pres), PRSNT_NODEF);

   /* Fill the global cell id */
   wrEmmGetCgi(ueCb->cellId,&cgi);
   wrFillTknU8(&(eUTRANCell->global_Cell_ID.pres),PRSNT_NODEF);
   wrUmmS1SrcHoFillPLMNId((S1apPdu *)iePdu, &cgi.plmnId, 
                         &(eUTRANCell->global_Cell_ID.pLMNidentity));
   wrFillTknBStr32(&(eUTRANCell->global_Cell_ID.cell_ID), 28, cgi.cellId);
   eUTRANCell->global_Cell_ID.iE_Extns.noComp.pres = NOTPRSNT;

   /* Fill cell type */ 
   wrEmmGetCellSizeType(ueCb->cellId, &cellSizeType);
   wrFillTknU8(&(eUTRANCell->cellTyp.pres), PRSNT_NODEF);
   wrFillTknU32(&(eUTRANCell->cellTyp.cell_Size), cellSizeType);
   eUTRANCell->cellTyp.iE_Extns.noComp.pres = NOTPRSNT;

   /* fill time UE stayed in cell */
   SGetEpcTime(&currentTime); //currenttime in milliseconds
   eUTRANCell->time_UE_StayedInCell.val = 
              (currentTime - ueCb->ueAttachTime) / 1000;

   RLOG3(L_INFO,"cTime = %lu, ueCb->ueAttachTime = %lu, ueStayedInCellT = %u",\
         currentTime, ueCb->ueAttachTime, eUTRANCell->time_UE_StayedInCell.val);

   if(eUTRANCell->time_UE_StayedInCell.val > WR_UMM_UE_MAX_STAY_TIME)
   {
      ueTime = WR_UMM_UE_MAX_STAY_TIME;
   }
   else
   {
     ueTime = (U32)(eUTRANCell->time_UE_StayedInCell.val);
   }
   wrFillTknU32(&(eUTRANCell->time_UE_StayedInCell), ueTime);

   /* Fill Time UE Stayed in Cell Enhanced Granularity */
   ueTime = (currentTime - ueCb->ueAttachTime) / 100;
   if(ueTime > WR_UMM_UE_MAX_STAY_TIME_ENH_GRAN)
   {
      ueTime = WR_UMM_UE_MAX_STAY_TIME_ENH_GRAN;
   }
   wrUmmFillLastVisitedEUTRANCellInform_ExtIEs(iePdu, eUTRANCell, 
         ueTime);

   /* fill UE HISTORY information */

   for(ueHistryIdx=1;ueHistryIdx < ueHistryCompSize;ueHistryIdx++,cellInfoIdx++)
   {
      lastVstCelInfo = &(ueHistoryInform->member[ueHistryIdx]);
     wrFillTknU8(&(lastVstCelInfo->choice), 
            ueCb->ueHstryInfo.cellInfo[cellInfoIdx].ratType);
      switch(lastVstCelInfo->choice.val)
      {
         case LASTVISITEDCELL_ITEM_E_UTRAN_CELL:
            {
            eutraCellInfo = &(ueCb->ueHstryInfo.cellInfo[cellInfoIdx].
                            ratInfo.eutranInfo);
            eUTRANCell = &lastVstCelInfo->val.e_UTRAN_Cell;
            wrFillTknU8(&(eUTRANCell->pres), PRSNT_NODEF);
            wrFillTknU8(&(eUTRANCell->global_Cell_ID.pres),PRSNT_NODEF);
               /* Fill the PLMN ID of global cell id */
            wrUmmS1SrcHoFillPLMNId((S1apPdu *)iePdu,
                             &eutraCellInfo->ecgi.plmnId,
                             &(eUTRANCell->global_Cell_ID.pLMNidentity));
               /* Fill the E-UTRAN Cell Identifier of global cell id */
            wrFillTknBStr32(&(eUTRANCell->global_Cell_ID.cell_ID), 28, 
                            eutraCellInfo->ecgi.eutranCellId);
            eUTRANCell->global_Cell_ID.iE_Extns.noComp.pres = NOTPRSNT;

            /* Fill cell type */ 
            wrFillTknU8(&(eUTRANCell->cellTyp.pres), PRSNT_NODEF);
            wrFillTknU32(&(eUTRANCell->cellTyp.cell_Size),
                         eutraCellInfo->cellSize);
            eUTRANCell->cellTyp.iE_Extns.noComp.pres = NOTPRSNT;
            /* fill time UE stayed in cell */
            wrFillTknU32(&(eUTRANCell->time_UE_StayedInCell), 
                         eutraCellInfo->timeInCell);
            /* Fill Time UE Stayed in Cell Enhanced Granularity */
            wrUmmFillLastVisitedEUTRANCellInform_ExtIEs(iePdu, 
                  eUTRANCell, eutraCellInfo->timeInCellEnhGranularity);
         }
         break;
         case LASTVISITEDCELL_ITEM_UTRAN_CELL:
         {
            utraCellInfo = &(ueCb->ueHstryInfo.cellInfo[cellInfoIdx].
                           ratInfo.utranInfo);
            uTRANCell = &lastVstCelInfo->val.uTRAN_Cell;
            wrFillTknStrOSXL(uTRANCell,utraCellInfo->len,
                            utraCellInfo->val,&iePdu->memCp);
         }
         break;
         case LASTVISITEDCELL_ITEM_GERAN_CELL:
         {
           /* Information for GERAN is currently undefined as per 3GPP *
             * Need to update GERAN as necessary later */
         }
         break;
      }
   }/* end of for loop */
   RETVALUE(ROK);
}
/**
 * @brief
 *    This function is used to encode UE History Information
 *
 * @details
 *    Function: wrEncUeHistoryInfo
 *
 *    Processing steps:
 *       -# Fill encode UE History Information
 *
 * @param[in]
 *    sztEncDecEvnt    : Event structure pointer
 * @param[in]
 *    transCb          : transaction control information
 * @return S16
 *    -#Success : ROK
 *    -#Failure : RFAILED
 */
PRIVATE S16 wrEncUeHistoryInfo
(
 SztEncDecEvnt                   *sztEncDecEvnt,
 WrUmmTransCb                    *transCb
 )
{
	Buffer          *mBuf;
	U8              dbgFlag = TRUE;
    SzIePdu         *ueHistIePdu = NULLP;
    CmPAsnErr       asnErr;
    MsgLen          contLen;
    MsgLen          msgLen;
    U32             mBufLen;
    U16             noComp;
    SztSrcRNC_ToTgetRNC_TprntCont_3gasn *srcRnctoTgtRncCnt = NULLP;
    srcRnctoTgtRncCnt = &(sztEncDecEvnt->u.iePdu->u.srcRncToTgetRnc);
    WR_ALLOCEVNT(&ueHistIePdu, sizeof(SzIePdu));
    if(NULLP == ueHistIePdu)
    {
       RLOG0(L_FATAL,"Memory allocation failed.");
       RETVALUE(RFAILED);
    }
    if (ROK != wrUmmS1SrcHoFillUeHistoryInform(ueHistIePdu, transCb))
    {
       RLOG0(L_ERROR,"wrEncUeHistoryInfo:Failed To Fill UE History Inform\n");
       WR_FREEEVNT(ueHistIePdu);
       RETVALUE(RFAILED);
    }
    SGetMsg(WR_MEM_REGION, WR_MEM_POOL, &mBuf);
    if(mBuf == NULLP)
    {
       RLOG0(L_FATAL, "Memory allocation failed.");
       WR_FREEEVNT(ueHistIePdu);
       RETVALUE(RFAILED);
    }
    if(ROK != cmPAsnEncMsg((Void *)(&ueHistIePdu->u.srcToTget.uE_HistoryInform), CM_PASN_USE_MBUF,
             (Void *)(mBuf), 0 /*SZ_PASN_VER_ZERO*/,
             (CmPAsnElmDef **) /* tmpszMsgIEDb[0]*/  szMsgIEDb[WR_UMM_S1AP_ENC_UE_HISTORY], &asnErr,
			&mBufLen, dbgFlag, FALSE ))
	{
		RLOG0(L_ERROR,"wrEncUeHistoryInfo:Failed To Encode UE History IE");
        WR_FREEEVNT(ueHistIePdu);
        SPutMsg(mBuf);
		RETVALUE(RFAILED);
	}
	else
	{
    	WR_FREEEVNT(ueHistIePdu);
    }

    noComp = 1;
    if (ROK != (cmGetMem(&sztEncDecEvnt->u.iePdu->memCp,
					(noComp * sizeof(SztProtExtnField_SrcRNC_ToTgetRNC_TprntCont_ExtIEs)),
            			((Ptr *)&srcRnctoTgtRncCnt->iE_Extns.member))))
    {
       RLOG0(L_FATAL, "Memory allocation failed.");
       SPutMsg(mBuf);
       RETVALUE(RFAILED);
    }
    SFndLenMsg(mBuf, &msgLen);
    wrFillTknU16(&(srcRnctoTgtRncCnt->iE_Extns.noComp),noComp);
	wrFillTknU8(&(srcRnctoTgtRncCnt->iE_Extns.member[0].pres), PRSNT_NODEF);
    wrFillTknU32(&(srcRnctoTgtRncCnt->iE_Extns.member[0].id), Sztid_UE_History_Inform);
    wrFillTknU32(&(srcRnctoTgtRncCnt->iE_Extns.member[0].criticality), SztCriticalityrejectEnum);
    srcRnctoTgtRncCnt->iE_Extns.member[0].extensionValue.u.sztUE_History_Inform.pres = PRSNT_NODEF;
    srcRnctoTgtRncCnt->iE_Extns.member[0].extensionValue.u.sztUE_History_Inform.len = msgLen;
    WR_GET_MEM((&sztEncDecEvnt->u.iePdu->memCp), msgLen, 
                        &(srcRnctoTgtRncCnt->iE_Extns.member[0].extensionValue.u.sztUE_History_Inform.val));
    SCpyMsgFix(mBuf, 0, msgLen,
                        (Data *)(srcRnctoTgtRncCnt->iE_Extns.member[0].extensionValue.u.sztUE_History_Inform.val), &contLen);
   
    SPutMsg(mBuf);
    RETVALUE(ROK);
}
/* UE History info - end */

/* Preparation of Source To target Transparent Container.*/

PRIVATE S16 wrUmmS1SrcHoPrepSrcToTgtTransCont
(
WrUmmTransCb                 *transCb
)
{
   SztEncDecEvnt                   sztEncDecEvnt;
   SztE_RABInformLst               *e_RABInformLst = NULLP;
   SztE_RABInformLstItem           *rbItem = NULLP;
   WrUeRAB                         *rbInfo = NULLP;
   SzIePdu                         *iePdu = NULLP;
   U8                              rbCnt;
   U8                              rbIdx = 0;
   MsgLen                          contLen;
   MsgLen                          msgLen;
   NhuEncCfmSdus                   *srcToTgtCntr = transCb->msg->u.encCfm;
   U8                              idx = 0;

   /* Allocate memory for event structure */
   /*ccpu00129931*/
   sztEncDecEvnt.transId = transCb->transId;
   sztEncDecEvnt.type.pres = PRSNT_NODEF;
   /* Set Type of Encode.*/
   sztEncDecEvnt.type.val = SZT_REQ_TYPE_ENCODE_IE;
   sztEncDecEvnt.ieType.pres = PRSNT_NODEF;
   /* Set Type of IE Encode.*/
   sztEncDecEvnt.ieType.val = SZT_REQ_TYPE_ENCDEC_IE_SRCTGET;
   /* Allocate memory for SzIePdu */
   WR_ALLOCEVNT(&sztEncDecEvnt.u.iePdu, sizeof(SzIePdu));
   if(NULLP == (sztEncDecEvnt.u.iePdu))
   {
        RLOG0(L_FATAL,"Memory allocation failed.");
        RETVALUE(RFAILED);
   }

   iePdu = sztEncDecEvnt.u.iePdu;
   wrFillTknU8(&iePdu->u.srcToTget.pres, PRSNT_NODEF);
   /* Fill RRC Container */
   SFndLenMsg(srcToTgtCntr->sdu.mBuf, &msgLen);
   iePdu->u.srcToTget.rRC_Cont.pres = PRSNT_NODEF;
   iePdu->u.srcToTget.rRC_Cont.len  = msgLen;
   WR_GET_MEM((&iePdu->memCp), msgLen, 
                        &(iePdu->u.srcToTget.rRC_Cont.val));
   SCpyMsgFix(srcToTgtCntr->sdu.mBuf, 0, msgLen,
                        (Data *)(iePdu->u.srcToTget.rRC_Cont.val), &contLen);

   /* fill ERAB information */
   e_RABInformLst = &iePdu->u.srcToTget.e_RABInformLst;
   rbCnt = transCb->ueCb->rbInfo.numOfRbCfgd;
   rbInfo = transCb->ueCb->rbInfo.rab;
   wrFillTknU16(&(e_RABInformLst->noComp), rbCnt);
   /* Allocate the memory for transparent containier IEs.*/
   if ((cmGetMem(&iePdu->memCp,
       (rbCnt * sizeof(SztProtIE_SingleCont_E_RABInformLstIEs)),
			((Ptr *)&e_RABInformLst->member))) !=ROK)
   {
      RLOG0(L_FATAL, "Memory allocation failed.");
      RETVALUE(RFAILED);
   }
   for(idx = 0; (idx < WR_UMM_MAX_DRBS) && (rbIdx < rbCnt); idx++)
   {
      if(rbInfo[idx].inUse != TRUE)
      {
         continue;
      }
      wrFillTknU8(&(e_RABInformLst->member[rbIdx].pres),PRSNT_NODEF);
      wrFillTknU32(&(e_RABInformLst->member[rbIdx].id),
				Sztid_E_RABInformLstItem);
      wrFillTknU32(&(e_RABInformLst->member[rbIdx].criticality),
				SztCriticalityignoreEnum);
      rbItem = &(e_RABInformLst->member[rbIdx].value.u.sztE_RABInformLstItem);
      wrFillTknU8(&(rbItem->pres), PRSNT_NODEF);
      wrFillTknU32(&(rbItem->e_RAB_ID), rbInfo[idx].eRABId);
      if(rbInfo[idx].rbDir == WR_DIR_DL || rbInfo[idx].rbDir == WR_DIR_DL_UL)
      {
          wrFillTknU32(&(rbItem->dL_Fwding), SztDL_FwdingdL_Fwding_proposedEnum);
      }
      else
      {
         rbItem->dL_Fwding.pres = NOTPRSNT;
      }
         rbItem->iE_Extns.noComp.pres = NOTPRSNT;
      rbIdx++;
   }

   /* fill targetCell_ID */
   wrFillTknU8(&iePdu->u.srcToTget.targetCell_ID.pres, PRSNT_NODEF);

   wrUmmS1SrcHoFillPLMNId((S1apPdu *)iePdu, &transCb->u.s1SrcHoTrans.plmnId,
         &iePdu->u.srcToTget.targetCell_ID.pLMNidentity);
   wrFillTknBStr32(&(iePdu->u.srcToTget.targetCell_ID.cell_ID), 28, 
		transCb->u.s1SrcHoTrans.t.intraLteInfo.ecgi.eutranCellId);
   iePdu->u.srcToTget.targetCell_ID.iE_Extns.noComp.pres = NOTPRSNT;
   /* fill subscriberProfileIDforRFP */
   /* If we have the Subscriber Profile ID, then fill the same in handover
    * required message and send it to Target eNB
    */
   if(TRUE == transCb->ueCb->subscProfileId.pres)
   {
      wrFillTknU32(&(iePdu->u.srcToTget.subscriberProfileIDforRFP),
                    transCb->ueCb->subscProfileId.val);
   }

/* UE History info - start */
/* Filling of UE History Information */
   /**
    * The eNB collects and stores the UE History Information for as long as
    * the UE stays in one of its cells.
    */
   if (ROK != wrUmmS1SrcHoFillUeHistoryInform(iePdu, transCb))
   {
      RLOG0(L_ERROR, "wrUmmS1SrcHoPrepSrcToTgtTransCont:Failed To Fill UE History Inform");
      WR_FREEEVNT(iePdu);
      RETVALUE(RFAILED);
   }
/* UE History info - end */

   iePdu->u.srcToTget.iE_Extns.noComp.pres = NOTPRSNT;
   WrIfmS1apEncDecReq(&sztEncDecEvnt);

   /*ccpu00129829*/
   //WR_FREE(sztEncDecEvnt, sizeof(SztEncDecEvnt));

   RETVALUE(ROK);
}/* End of wrUmmS1SrcHoPrepSrcToTgtTransCont*/

/* Preparation of Source To target Transparent Container.*/

PRIVATE S16 wrUmmS1SrcHoPrepSrcRncToTgtRncTransCont
(
WrUmmTransCb                 *transCb
)
{
   SztEncDecEvnt                        sztEncDecEvnt;
   SztSrcRNC_ToTgetRNC_TprntCont_3gasn *srcRnctoTgtRncCnt = NULLP; 
   NhuEncCfmSdus                       *rrcCntr = transCb->msg->u.encCfm;
   MsgLen                               contLen;
   MsgLen                               msgLen;
   U32                                  targetUCellId = 0;
   WrUmmIRatUtraInfo                   *iRatUtraHoTransInfo =
                                       &(transCb->u.s1SrcHoTrans.t.interRatInfo);
   TknU32                              *srvccHoInd = &(transCb->u.s1SrcHoTrans.srvccHoInd);

   /* Allocate memory for event structure */
   /*ccpu00129931*/
   sztEncDecEvnt.transId = transCb->transId;
   sztEncDecEvnt.type.pres = PRSNT_NODEF;
   /* Set Type of Encode.*/
   sztEncDecEvnt.type.val = SZT_REQ_TYPE_ENCODE_IE;
   sztEncDecEvnt.ieType.pres = PRSNT_NODEF;
   /* Set Type of IE Encode.*/
   sztEncDecEvnt.ieType.val = SZT_REQ_TYPE_ENCDEC_IE_SRCRNCTGETRNC;
   /* Allocate memory for SzIePdu */
   WR_ALLOCEVNT(&sztEncDecEvnt.u.iePdu, sizeof(SzIePdu));
   srcRnctoTgtRncCnt = &(sztEncDecEvnt.u.iePdu->u.srcRncToTgetRnc);
   wrFillTknU8(&(srcRnctoTgtRncCnt->pres), PRSNT_NODEF);
   /* Fill RRC Container */
   SFndLenMsg(rrcCntr->sdu.mBuf, &msgLen);
   srcRnctoTgtRncCnt->rRC_Cont.pres = PRSNT_NODEF;
   srcRnctoTgtRncCnt->rRC_Cont.len  = msgLen;
   WR_GET_MEM(&(sztEncDecEvnt.u.iePdu->memCp), msgLen, 
                        &(srcRnctoTgtRncCnt->rRC_Cont.val));
   SCpyMsgFix(rrcCntr->sdu.mBuf, 0, msgLen,
                        (Data *)(srcRnctoTgtRncCnt->rRC_Cont.val), &contLen);

   
   /* Fill IuInstance as 2 if both CS and PS enabled */
   if ( (srvccHoInd->pres == PRSNT_NODEF )  &&
        (srvccHoInd->val == SztSRVCCHOIndpSandCSEnum )) 
   {
      wrFillTknU32(&(srcRnctoTgtRncCnt->numberOfIuInstances), 2);
   }
   else
   {
      wrFillTknU32(&(srcRnctoTgtRncCnt->numberOfIuInstances), 1);
   }
   /* Fill IE - Relocation Type  -  in case of IRAT HO from EUTRA  It is set 
      to "UE involved in relocation of SRNS" */
   wrFillTknU32(&(srcRnctoTgtRncCnt->relocationTyp), 
                              SztRelocationTypue_involvedEnum);

   /* Fill targetCell_ID:Target Cell Id = U-Cid = RNC ID(12 bits)+CID(16 bits) 
      if the RNC id is extended, then it would be RNC ID(16 bits)+CID(12 bits)*/
   if ( iRatUtraHoTransInfo->rncId < WR_EXT_RNC_START )
   {
      targetUCellId =  (( 0xFFFF & iRatUtraHoTransInfo->cId ) | 
            (iRatUtraHoTransInfo->rncId << 16));
   }
   else
   {
      targetUCellId =  (( 0xFFF & iRatUtraHoTransInfo->cId ) | 
            (iRatUtraHoTransInfo->rncId << 12));
   }
   
   wrFillTknU32(&(srcRnctoTgtRncCnt->targetCellId), targetUCellId);
   if (ROK != wrEncUeHistoryInfo(&sztEncDecEvnt, transCb))
   {
      RLOG0(L_ERROR,"wrUmmS1SrcHoPrepSrcRncToTgtRncTransCont:Failed To Fill UE History Inform");
      WR_FREEEVNT(sztEncDecEvnt.u.iePdu);
      RETVALUE(RFAILED);
   }
   WrIfmS1apEncDecReq(&sztEncDecEvnt);

   RETVALUE(ROK);
}/* End of wrUmmS1SrcHoPrepSrcRncToTgtRncTransCont*/


PRIVATE S16 wrUmmS1SrcHoSndTnlCreateReq
(
 U32                          transId,
 WrUeCb                       *ueCb,
 WrUmmS1SrcHoRabInfo          *rabInfo,
 U32                          peerId,
 WrDamTnlType                 tnlType,
 WrHoType                     hoType
 )
{
   WrDamTnlInfo              *tnlInfo;
   CmTptAddr                 *lclAddr;
   /*CmTptAddr                 *peerAddr; */

   WR_ALLOC_SHAREBLE_BUF(&tnlInfo, sizeof(WrDamTnlInfo));
   if (NULLP == tnlInfo)
   {
      RLOG0(L_FATAL, "Memory allocation failed.");
       RETVALUE(RFAILED);
   }
   tnlInfo->hoSrcEnodeb = TRUE;
   tnlInfo->transId = transId;
   tnlInfo->tnlId.cellId = ueCb->cellId;
   tnlInfo->tnlId.crnti = ueCb->crnti;
   tnlInfo->tnlId.drbId = rabInfo->rbId;
   tnlInfo->tnlType = tnlType;
   tnlInfo->hoType = hoType;
   wrEmmGetDatAppAddr(&lclAddr, rabInfo->peerAddr.type);
   wrCpyCmTptAddr(&tnlInfo->srcAddr, lclAddr);
   wrCpyCmTptAddr(&tnlInfo->dstAddr, &rabInfo->peerAddr);
   if(tnlType == WR_TNL_DL_DATFWD)
   {
      tnlInfo->remTeid = rabInfo->datFwdDlRemTunId;
   }
   else
   {
      tnlInfo->remTeid = rabInfo->datFwdUlRemTunId;
   }
   wrUtlGetDscp(ueCb->cellId, rabInfo->qci, &(tnlInfo->dscp));

   rabInfo->tnlCfgState = WR_UMM_SRC_HO_TNL_CFG_SENT;
   wrIfmDamTnlCreatReq(transId, tnlInfo);

   RETVALUE(ROK);
}

PRIVATE S16 wrUmmS1SrcHoCfgTnls
(
 WrUmmTransCb                 *transCb
)
{
   WrUmmS1SrcHoTrans         *s1SrcHoTrans = &transCb->u.s1SrcHoTrans;
   WrUeCb                    *ueCb = transCb->ueCb;
   WrUmmS1SrcHoRabInfo       *rabInfo = NULLP;
   U32                       transId = transCb->transId;
   U16                       idx; 
   U16                       noOfRbs;
   U32                       peerId = 0;
   U8                        rbMode;
   WrDamTnlStInfo            *tnlInfo;
   U8                        isDlOrUlTnlCfgd = FALSE;
   WrHoType                  hoType = WR_HO_TYPE_NONE;

   noOfRbs = s1SrcHoTrans->numOfRbInfo;
   /* return ROK with state WR_UMM_SRC_HO_TNL_CFG_DONE if
    * number of RBs are zero */
   if ( !noOfRbs )
   {
      s1SrcHoTrans->state = WR_UMM_SRC_HO_TNL_CFG_DONE;
      RLOG0(L_DEBUG, "noOfRbs are zero");
      RETVALUE(ROK);
   }
   /* Last 4 bits of transId would be set to HO type.  The transId will be
      added in tnlCb, hence this could be used in DAM module, during data 
      forwarding to identify HO type. In case of HO to UTRAN  PDCP sn will
      not be included in eGTP extention */ 
   if ( SztHovrTypintralteEnum == s1SrcHoTrans->sztHovrTyp->val )
   {
      hoType = WR_HO_TYPE_INTRA_LTE_S1;
   }
   else if ( SztHovrTypltetoutranEnum == s1SrcHoTrans->sztHovrTyp->val )
   {
      hoType = WR_HO_TYPE_IRAT_UTRA;
   }
   else if ( SztHovrTypltetogeranEnum == s1SrcHoTrans->sztHovrTyp->val )
   {
      hoType = WR_HO_TYPE_IRAT_GERAN;
   }
   else
   {
      /* TODO: add other ho type based on support */
      /* As of now returning RFAILED with debug prints */
      RLOG1(L_ERROR,"Invalid HO Type[%d]",
            s1SrcHoTrans->sztHovrTyp->val);
      RETVALUE(RFAILED);
   }

   for (idx = 0; idx < noOfRbs; idx++)
   {
      rabInfo = &s1SrcHoTrans->rbInfo[idx];
      /* TODO : temporarily added. need to remove when DAM becomes loosely coupled */
      WR_UMM_SRC_HO_CALC_NUM_TNL_CFG(rabInfo);

      if(rabInfo->rbDir == WR_DIR_UL || 
            rabInfo->rbDir == WR_DIR_DL_UL)
      {
         /* Create UL Forwarding Tnl only if Target requested for UL Data Forwarding */
         if(rabInfo->isULDatFwdReq == TRUE)
         {
            RLOG_ARG1(L_DEBUG, DBG_CRNTI, transCb->ueCb->crnti,"UL Forwarding "
                  "Tunnel create request for DRB[%d]", rabInfo->rbId);
            wrUmmS1SrcHoSndTnlCreateReq(transId, ueCb, rabInfo, peerId,
                  WR_TNL_UL_DATFWD, hoType);
            isDlOrUlTnlCfgd = TRUE;
         }    
      }    
      if(rabInfo->rbDir == WR_DIR_DL || 
            rabInfo->rbDir == WR_DIR_DL_UL)
      {
         if ( rabInfo->dlFwding == TRUE )
         {
            RLOG_ARG1(L_DEBUG, DBG_CRNTI, transCb->ueCb->crnti,"DL Forwarding "
                  "Tunnel create request for DRB[%d]", rabInfo->rbId);
            wrUmmS1SrcHoSndTnlCreateReq(transId, ueCb, rabInfo, peerId,
                  WR_TNL_DL_DATFWD, hoType);
            isDlOrUlTnlCfgd = TRUE;
         }
      }    

      wrUtlGetRbMode(ueCb->cellId, rabInfo->qci, &rbMode);
      if(rbMode == WR_RLC_UM_MODE)
      {
         WR_ALLOC_SHAREBLE_BUF(&tnlInfo, sizeof(WrDamTnlStInfo));
         if(tnlInfo == NULLP)
         {
            RLOG0(L_FATAL, "Memory allocation failed.");
            RETVALUE(RFAILED);
         }
         RLOG_ARG1(L_INFO, DBG_CRNTI, transCb->ueCb->crnti,"Changing tunnel "
            "state for UM bearer [%d]", rabInfo->rbId);
         tnlInfo->transId = 0;
         tnlInfo->state = WR_DAM_DRB_STATE_BUFFERING;
         tnlInfo->cause = WR_DAM_HO_UE_UM_DAT_FWD;
         tnlInfo->lclTeid = rabInfo->locTunId;
         wrIfmDamChngTnlState(tnlInfo);
      }
   }
   if ( isDlOrUlTnlCfgd != TRUE )
   {
      s1SrcHoTrans->state = WR_UMM_SRC_HO_TNL_CFG_DONE;
   }
   RETVALUE(ROK);
}


PRIVATE S16 wrUmmS1SrcHoPrcTgtToSrcDecCfm
(
 WrUmmTransCb                *transCb
)
{
   SztEncDecEvnt                 *decCfm = transCb->msg->u.sztDecCfm;
   SztTgeteNB_ToSrceNB_TprntCont *tgtEnbToSrcEnb = NULLP;
   NhuDecodeSduReq               *nhuDecodeSduReq = NULLP;
   Buffer                        *mBuf = NULLP;

   /*Decode Target eNB To Src eNB Transparent Container.*/
   tgtEnbToSrcEnb = &decCfm->u.iePdu->u.tgetToSrc;

   /* Request RRC to decode the TargeteNB_To_SrceNB Trasparent Container*/
   WR_ALLOC(&nhuDecodeSduReq, sizeof(NhuDecodeSduReq));
   if(NULLP == nhuDecodeSduReq)
   {
      RLOG0(L_FATAL, "Memory allocation failed.");
      RETVALUE(RFAILED);
   }/* End of if condition.*/
   nhuDecodeSduReq->hdr.cellId  =   transCb->ueCb->cellId;
   nhuDecodeSduReq->hdr.ueId    =   transCb->ueCb->crnti;
   nhuDecodeSduReq->hdr.transId =   transCb->transId | WR_UMM_SRC_HO_HOCMD_DEC;
   nhuDecodeSduReq->sduType     =   NHU_MSG_HOCMD;
   WR_ALLOCMBUF(wrCb.mem.region, wrCb.mem.pool, &mBuf);
   if (NULLP == mBuf)
   {
      RLOG0(L_FATAL, "Memory allocation failed.");
      WR_FREE(nhuDecodeSduReq, sizeof(NhuDecodeSduReq));
      RETVALUE(RFAILED);
   }
   /* convert the OSXL to Buffer */
   SAddPstMsgMult((Data *)tgtEnbToSrcEnb->rRC_Cont.val,
         (MsgLen)tgtEnbToSrcEnb->rRC_Cont.len,
         (Buffer *)mBuf);
   nhuDecodeSduReq->mbuf = mBuf;
   /* Decode the HO Command.This HO Command is used to send the RRC
    * Re-configuration. Further processing shall be done at the receipt
    * of NhuDecodeCfm.
    */
   wrIfmRrcSndDecodeReq(nhuDecodeSduReq);
   /* configure fwd tunnels */
   wrUmmS1SrcHoCfgTnls(transCb);
   RETVALUE(ROK);
}
/** @brief This function is responsible handling TgtToSrcDecFm
 * and send MobilityFromUtra Command to UE. 
 *
 * @details
 *
 *     Function: wrUmmS1SrcHoPrcIRatHoTgtToSrcDecCfm 
 *
 *
 * @param [in] WrUmmTransCb* transCb 
 * @return S16
 *      -# Success  : ROK
 *      -# Failure  : RFAILED
 */
PRIVATE S16 wrUmmS1SrcHoPrcIRatHoTgtToSrcDecCfm 
(
 WrUmmTransCb                *transCb
)
{
   SztEncDecEvnt                           *decCfm = transCb->msg->u.sztDecCfm;
   SztRRC_Cont                             *utraRrcCont = NULLP;
   SztRRC_Cont                             *geranRrcCont = NULLP;
   NhuDatReqSdus                           *MobiltyToEutraCmdMsg;
   WrUeCb                                  *ueCb;
   NhuDL_DCCH_Msg                          *dlMsg = NULLP;
   NhuMobilityFromEUTRACmd                 *mobFrmEutraCmd = NULLP;
   NhuMobilityFromEUTRACmd_r8_IEs          *mobEutraCmdRel8Ie = NULLP;
   NhuMobilityFromEUTRACmd_v8a0_IEs        *mobEutraCmdRel8a0Ie = NULLP;
   NhuHovr                                 *handoverIe;
   NhuMobilityFromEUTRACmdcriticalExtnsc1  *critExtnC1 = NULLP;
   TknStrOSXL                              *nasSecFrmEutran;
   U32                                     s1OvrlTmrVal = 0;


   WR_ALLOCEVNT(&MobiltyToEutraCmdMsg, sizeof(NhuDatReqSdus));
   if ( MobiltyToEutraCmdMsg == NULLP )
   {
      RLOG0(L_FATAL,"Memory allocation failed.");
      RETVALUE(RFAILED);
   }

   ueCb = transCb->ueCb;
   MobiltyToEutraCmdMsg->hdr.cellId  = ueCb->cellId;
   MobiltyToEutraCmdMsg->hdr.ueId    = ueCb->crnti;
   MobiltyToEutraCmdMsg->hdr.transId = transCb->transId; 

   MobiltyToEutraCmdMsg->sdu.isUeCfgPres = FALSE;
   MobiltyToEutraCmdMsg->sdu.ueMsg.isMsgDed = TRUE;
   MobiltyToEutraCmdMsg->sdu.ueMsg.msg.dedMsg.dlDcchMsg.pdcpId = WR_PDCP_ID_SRB1;

   dlMsg = &(MobiltyToEutraCmdMsg->sdu.ueMsg.msg.dedMsg.dlDcchMsg.m.dcchMsg);

   wrFillTknU8(&dlMsg->pres, PRSNT_NODEF);
   wrFillTknU8(&dlMsg->message.choice, DL_DCCH_MSGTYP_C1);
   wrFillTknU8(&dlMsg->message.val.c1.choice, C1_MOBILITYFROMEUTRACMD);

   mobFrmEutraCmd = &(dlMsg->message.val.c1.val.mobilityFromEUTRACmd);

   wrFillTknU8(&mobFrmEutraCmd->pres, PRSNT_NODEF);
   wrFillTknU32(&mobFrmEutraCmd->rrc_TrnsxnId, 0);  /* TODO: verify - TransxId  0 */
   wrFillTknU8(&mobFrmEutraCmd->criticalExtns.choice, CRITICALEXTNS_C1);

   critExtnC1 = &(mobFrmEutraCmd->criticalExtns.val.c1);
   /* Fill R8 IEs, which would work for R9 as well */
   wrFillTknU8(&critExtnC1->choice, C1_MOBILITYFROMEUTRACMD_R8);
   mobEutraCmdRel8Ie = &(mobFrmEutraCmd->criticalExtns.val.c1.val.
         mobilityFromEUTRACmd_r8);
   mobEutraCmdRel8a0Ie = &(mobEutraCmdRel8Ie->nonCriticalExtn);
   wrFillTknU8(&(mobEutraCmdRel8Ie->pres), PRSNT_NODEF);
   /* Fill cs fallback Indicator not present */
   wrFillTknU8(&(mobEutraCmdRel8Ie->cs_FallbackIndicator), FALSE);
   /* Fill the purpose as Haodover */
   wrFillTknU8(&(mobEutraCmdRel8Ie->purpose.choice), PURPOSE_HANDOVER);
   handoverIe = &(mobEutraCmdRel8Ie->purpose.val.handover);
   wrFillTknU8(&handoverIe->pres, PRSNT_NODEF);
   /* Below listed IEs would be filled in case of
      HO to UTRAN  
    * 1. targetRAT_Typ 
    * 2. targetRAT_MsgCont 
    * 3. nas_SecParamFromEUTRA 
    */

   /* Fill Target RAT type */
   if ( transCb->u.s1SrcHoTrans.sztHovrTyp->val == SztHovrTypltetogeranEnum )
   {
      wrFillTknU32(&(handoverIe->targetRAT_Typ), NhuHovrtargetRAT_TypgeranEnum);
      geranRrcCont = transCb->u.s1SrcHoTrans.tgtToSrcCont;
      U8      bandInc = transCb->u.s1SrcHoTrans.t.interRatGeranInfo.bandIndicator;

      /* Fill target Msg container */
      wrFillTknStrOSXL(&(handoverIe->targetRAT_MsgCont),
            geranRrcCont->len, geranRrcCont->val, 
            &(MobiltyToEutraCmdMsg->memCp));
      /*filling band Indicator ie*/
      wrFillTknU8(&(mobEutraCmdRel8a0Ie->pres), PRSNT_NODEF);
      wrFillTknU8(&(mobEutraCmdRel8a0Ie->nonCriticalExtn.pres), PRSNT_NODEF);
      if(bandInc == WR_GERAN_PCS1900)
      {
         wrFillTknU32(&(mobEutraCmdRel8a0Ie->nonCriticalExtn.bandIndicator),NhuBandIndicatorGERANpcs1900Enum);
      }
      else
      {
         wrFillTknU32(&(mobEutraCmdRel8a0Ie->nonCriticalExtn.bandIndicator),NhuBandIndicatorGERANdcs1800Enum);
      }
      wrFillTknU8(&(mobEutraCmdRel8a0Ie->nonCriticalExtn.nonCriticalExtn.pres), NOTPRSNT);
      mobEutraCmdRel8a0Ie->lateNonCriticalExtn.pres = NOTPRSNT;
   }
   else /* SztHovrTypltetoutranEnum */
   {
      wrFillTknU32(&(handoverIe->targetRAT_Typ), NhuHovrtargetRAT_TyputraEnum);
      utraRrcCont = &(decCfm->u.iePdu->u.TgetRncToSrcRnc.rRC_Cont);

      /* Fill target Msg container */
      wrFillTknStrOSXL(&(handoverIe->targetRAT_MsgCont),
            utraRrcCont->len, utraRrcCont->val, 
            &(MobiltyToEutraCmdMsg->memCp));
   }
   /* Fill Nas Security parameter from EUTRAN  */
   nasSecFrmEutran =  &(transCb->u.s1SrcHoTrans.nasSecParamFrmEutran);
   wrFillTknStrOSXL(&(handoverIe->nas_SecParamFromEUTRA),
                      nasSecFrmEutran->len, nasSecFrmEutran->val, 
                     &(MobiltyToEutraCmdMsg->memCp));

   RLOG_ARG0(L_EVENT, DBG_CRNTI,ueCb->crnti,"[OTA] Sending" 
        " MobilityFromEUTRACommand for HandOver ");
   if(ROK != wrIfmRrcSndReqToUe(MobiltyToEutraCmdMsg))
   {
      RLOG0(L_ERROR,"wrIfmRrcSndReqToUe failed");
      RETVALUE(RFAILED);
   }

   ueCb->mobCtrlState = WR_HO_IN_PROGRESS;

   /* Increment the IRAT Attempted Counter after sending Mobility from EUTRA 
    * Command to UE */
   WR_INC_KPI_FUNC5(wrIncKpiFrHovrAttmptd, WR_HO_TYPE_IRAT_UTRA,
         transCb->u.s1SrcHoTrans.sztHovrTyp->val, OUT_HOV,
         (transCb->u.s1SrcHoTrans.sztCause), NULLP);
   if ( transCb->u.s1SrcHoTrans.srvccHoInd.pres == PRSNT_NODEF )
   {
      WR_INC_KPI_FUNC3(wrIncKpiFrSrvccAttmptd, transCb->u.s1SrcHoTrans.sztHovrTyp->val,
            OUT_HOV, (transCb->u.s1SrcHoTrans.sztCause));
   }
   /* Start Overall timer */
   wrEmmGetTmrVal( transCb->ueCb->cellId, 
                   WR_UMM_IRAT_S1_OVRL_TMR,&s1OvrlTmrVal);
   wrStartUeTmr(transCb->ueCb, WR_TMR_S1_OVRL_TMR,s1OvrlTmrVal); 

  /* Send Fwd Tunnel configuration request */ 
   wrUmmS1SrcHoCfgTnls(transCb);
   /* If No DL or UL Forwarding tunnel is created then 
      no need to wait for DAM_CFM, send GetPdcpSta immediately */
   if ( transCb->u.s1SrcHoTrans.state == WR_UMM_SRC_HO_TNL_CFG_DONE )
   {
      wrUmmS1SrcHoGetPdcpSduSta(transCb);
   }
   RETVALUE(ROK);
}
PRIVATE S16 wrUmmS1SrcHoPrcDamCfm
(
 WrUmmTransCb                  *transCb
)
{
   WrUmmS1SrcHoTrans         *s1SrcHoTrans = &transCb->u.s1SrcHoTrans;
   WrUmmS1SrcHoRabInfo       *rabInfo = NULLP;
   U32                       drbId;
   U8                        status;
   U8                        idx;
   U16                       noOfRbs;
   WrUmmDamCfgCfm            *damCfm;
   WrDamTnlType              tnlType;

   damCfm = &transCb->msg->u.damCfm;
   drbId   = (damCfm->lclTeId & 0x000000ff) >> 3;
   tnlType = (damCfm->lclTeId & 0x00000007);
   status  = damCfm->status;

   noOfRbs = s1SrcHoTrans->numOfRbInfo;
   for (idx = 0; idx < noOfRbs; idx++)
   {
      rabInfo = &s1SrcHoTrans->rbInfo[idx];
      if(rabInfo->rbId == drbId)
      {
         if(status == WR_DAM_CFG_OK)
         {
            switch(tnlType)
            {
               case WR_TNL_DL_DATFWD:
               {
                  RLOG_ARG1(L_INFO, DBG_CRNTI, transCb->ueCb->crnti,
                     "DL Forwarding Tunnel created DRB[%ld]", drbId);
                  rabInfo->tnlCfgState = WR_UMM_SRC_HO_TNL_CFG_SUCC;
                  rabInfo->datFwdDlLclTunId  = damCfm->lclTeId;
                  break;
               }
               case WR_TNL_UL_DATFWD:
               {
                  RLOG_ARG1(L_INFO, DBG_CRNTI, transCb->ueCb->crnti,
                     "UL Forwarding Tunnel created DRB[%ld]", drbId);
                  rabInfo->tnlCfgState = WR_UMM_SRC_HO_TNL_CFG_SUCC;
                  rabInfo->datFwdUlLclTunId  = damCfm->lclTeId;
                  break;
               }
               default:
               {
                  RLOG0(L_ERROR, "Invalid case");
                  break;
               }
            }
         }
         else
         {
            rabInfo->tnlCfgState = WR_UMM_SRC_HO_TNL_CFG_FAIL;
         }
         rabInfo->expectedTnlRsp--;
      }
   }
   for(idx = 0; idx < noOfRbs; idx++)
   {
      rabInfo = &s1SrcHoTrans->rbInfo[idx];
      if((rabInfo->tnlCfgState == WR_UMM_SRC_HO_TNL_CFG_SENT) ||
            (rabInfo->expectedTnlRsp != 0))
      {
         RETVALUE(ROK);
      }
   }
   s1SrcHoTrans->state = WR_UMM_SRC_HO_TNL_CFG_DONE;
   RETVALUE(ROK);
}

PRIVATE S16 wrUmmS1SrcHoGetPdcpSduSta
(
 WrUmmTransCb                 *transCb
)
{
   WrUeCb                    *ueCb = transCb->ueCb;
   NhuHdr                    nhuHdr;

   nhuHdr.cellId = ueCb->cellId;
   nhuHdr.ueId = ueCb->crnti;
   nhuHdr.transId = transCb->transId;
   nhuHdr.isUlDatFwdReq = FALSE;

   RLOG_ARG0(L_DEBUG, DBG_CRNTI, transCb->ueCb->crnti,"Sending PDCP SN status"
      "request");

   WrIfmRrcGetPdcpSduSta(nhuHdr);
   RETVALUE(ROK);
}

PRIVATE S16 wrUmmS1SrcHoSendStartPdcpDataFwdReq
(
 WrUmmTransCb                 *transCb
)
{
   WrUeCb                    *ueCb = transCb->ueCb;
   NhuHdr                    nhuHdr;
   WrUmmS1SrcHoTrans         *s1SrcHoTrans = NULLP;
   Bool                      ulDatFwdReq = FALSE;
   U8                        rbIdx = 0;

   nhuHdr.cellId = ueCb->cellId;
   nhuHdr.ueId = ueCb->crnti;
   nhuHdr.transId = transCb->transId | 0x2;

   s1SrcHoTrans = &transCb->u.s1SrcHoTrans;
   for(rbIdx = 0; rbIdx < s1SrcHoTrans->numOfRbInfo; rbIdx++)
   {
      if(s1SrcHoTrans->rbInfo[rbIdx].isULDatFwdReq == TRUE)
      {
         /* Even if for 1 Rab UL Forwarding is proposed set the flag to TRUE */
         ulDatFwdReq = TRUE; 
         break;
      }
   }
   nhuHdr.isUlDatFwdReq = ulDatFwdReq;
   RLOG_ARG0(L_DEBUG,DBG_CRNTI,transCb->ueCb->crnti,"Sending PDCP Data "
      "Forward request");

   WrIfmRrcSendStartPdcpDataFwdReq(nhuHdr);
   RETVALUE(ROK);
}

PRIVATE S16 wrUmmS1SrcHoHdlIntMsg
(
WrUmmTransCb                 *transCb
)
{
   WrUeCb                    *ueCb = transCb->ueCb;
   WrUmmInternalMsg          *internalMsg = transCb->msg->u.internalMsg;
   NhuEncReqSdus             *hoPrepEncMsg = NULLP;
   WrUmmRatLst                ratLst;

   if (internalMsg == NULLP)
   {
      RLOG0(L_ERROR, "Invalid Inputs (msg is NULL)");
      RETVALUE(RFAILED);
   }
   switch(internalMsg->intMsgType)
   {
      /* Create S1 Handover Transaction.*/
      case WR_UMM_INTMSG_CR_HO_TRANS:
      {
         if ( ROK != wrUmmS1SrcHoInitTrans(transCb, internalMsg->u.hoTrans))
         {
            wrUmmTransComplete(transCb);
            RETVALUE(RFAILED);
         }

         if ( SztHovrTypintralteEnum == 
                   transCb->u.s1SrcHoTrans.sztHovrTyp->val ) 
         {
            /* prepare the handover preparation information ie             */
            WR_ALLOCEVNT(&hoPrepEncMsg, sizeof(NhuEncReqSdus));
            if(NULLP == hoPrepEncMsg)
            {
               RLOG0(L_FATAL, "Memory allocation failed.");
               RETVALUE(RFAILED);
            }
            /* Fill RRC Encode Request Header */
            hoPrepEncMsg->hdr.cellId = ueCb->cellId;
            hoPrepEncMsg->hdr.ueId = ueCb->crnti;
            hoPrepEncMsg->hdr.transId = transCb->transId | WR_UMM_SRC_HO_HOPREP_ENC;
            hoPrepEncMsg->sdu.msgCategory = NHU_MSG_HOPREPINFO;

            /* fill transparent container */
            wrUmmRrcFillSrcToTgetIe(ueCb, hoPrepEncMsg);
            if(ROK != wrIfmRrcSndEncodeReq(hoPrepEncMsg))
            {
               RLOG0(L_ERROR, "Failed to send Encode Req to RRC");
               RETVALUE(RFAILED);
            }
         }
         else if ( SztHovrTypltetoutranEnum == 
                     transCb->u.s1SrcHoTrans.sztHovrTyp->val) 
         {
           /* Send UTRAN capapability request to UE,
              Which is required to include in RRC container of 
              HO Preparation message  */
            ratLst.noComp =  1; /* Request for UTRA capability */
            ratLst.ratType[0] = NhuRAT_TyputraEnum;

            if(ROK != wrUmmUeCapSndUeCapEnq(transCb, &ratLst))
            {
               RLOG0(L_ERROR,"Failed to Send UECAP Enq");
               RETVALUE(RFAILED);
            }
         }
         /* In case of HO to GERAN, RRC and S1AP encoding is not required, because
          * Target GERAN IEs are in TLV format */
         else if(SztHovrTypltetogeranEnum == transCb->u.s1SrcHoTrans.sztHovrTyp->val)
         {
            /* Snd Handover Required Message to the MME.*/
            if (ROK != wrUmmS1SrcHoPrepHandoverReqrd(transCb))
            {
               RLOG0(L_ERROR, "Failed To Prepare Handover Request Message");
               WR_UMM_S1_SRC_HANDLE_HO_FAIL(transCb);   
               wrUmmTransComplete(transCb);
               RETVALUE(RFAILED);
            }/* End of wrUmmS1SrcHoSndHandoverReq */
         }
         break;
      }
      default:
      {
         RLOG0(L_ERROR, "Invalid internal msgType");
         RETVALUE(RFAILED);
      }
   }
   RETVALUE(ROK);
}/* End of wrUmmS1SrcHoHdlIntMsg */

PRIVATE S16 wrUmmS1SrcHoHdlRrcEncCfm
(
WrUmmTransCb                 *transCb
)
{
   NhuEncCfmSdus             *encMsg;
   U16                       msgType;

   encMsg = transCb->msg->u.encCfm;
   msgType = encMsg->hdr.transId & 0x0f;
   if(encMsg->sdu.status)
   {
      RLOG1(L_ERROR, "Encoding failed for [%d]", msgType);
      RETVALUE(RFAILED);
   }
   switch(msgType)
   {
      case WR_UMM_SRC_HO_HOPREP_ENC:
      {
         if (ROK != wrUmmS1SrcHoPrepSrcToTgtTransCont(transCb))
         {
            RLOG0(L_ERROR, "Invalid Inputs (msg is NULL).");
            RETVALUE(RFAILED);
         }
         break;
      }
      case WR_UMM_SRC_HO_UTRA_CONT_ENC :/* IRAT HoToUTRA */
      {
         /* Prepare Source RNC to Target RNC Transparent container */
         if (ROK != wrUmmS1SrcHoPrepSrcRncToTgtRncTransCont(transCb))
         {
            RLOG0(L_ERROR,"Invalid Inputs (msg is NULL)");
            RETVALUE(RFAILED);
         }
      }
      break;
      default:
      {
        RLOG1(L_ERROR,"Invalid Msg type[%d]",
              msgType);
        RETVALUE(RFAILED);
      }
   }
   RLOG_ARG0(L_INFO, DBG_CRNTI, transCb->ueCb->crnti,"S1 HANDOVER state = "
      "INITIATED , Cause = Encoding Confirm Message from RR");
   RETVALUE(ROK);
}


PRIVATE S16 wrUmmS1SrcHoPrcTmrExpiry
(
WrUmmTransCb                 *transCb
)
{
   RLOG0(L_ERROR, "Timer WR_UMM_TMR_EXPIRY expired");
   switch(transCb->msg->u.tmrExpiry.timerEvnt)
   {
       case WR_TMR_RRC_CAPENQ:
       {
           RLOG0(L_ERROR,"Timer WR_TMR_RRC_CAPENQ expired");
           /* Release the UE with cause "Radio Connection With UE Lost"  */
           transCb->errorCause = WR_UMM_UE_LOST;
           transCb->state = WR_UMM_TRANS_RELEASE_CALL;
       }
       break;
       case WR_TMR_S1_RELOC_TMR:
       {
            transCb->u.s1SrcHoTrans.sztCause->choice.pres = TRUE;
            transCb->u.s1SrcHoTrans.sztCause->choice.val = WR_CAUSE_RADIONW;
            transCb->u.s1SrcHoTrans.sztCause->val.radioNw.pres = TRUE;
            transCb->u.s1SrcHoTrans.sztCause->val.radioNw.val = 
                               SztCauseRadioNwtS1relocprep_expiryEnum;
             /* ccpu00130451 */
             wrUmmS1SrcSndHovrCancel(transCb);
             /* Increment the counter for Reloc Timer Expiry */
             WR_INC_KPI_FUNC3(wrIncKpiFrS1apFailCause, WR_CAUSE_RADIONW, 
                SztCauseRadioNwtS1relocprep_expiryEnum, INC_KPI_VALUE_BY_ONE);    
       }
       break;
       case WR_TMR_SRC_HO_CANCEL:
       {
          WR_UMM_S1_SRC_HANDLE_HO_FAIL(transCb);
          wrUmmTransComplete(transCb); 
       }
       break;
       default:
       {
          RLOG1(L_ERROR, "Unknown Tmr Event[%d]",
             transCb->msg->u.tmrExpiry.timerEvnt);
       }
   }
   RETVALUE(ROK);
}

/* ********************************************************************** */
/*                                                                        */
/* Name: wrUmmS1SrcHoProcessor                                            */
/*                                                                        */
/* This function is called when any message related to a particular       */
/*    S1 HO is received in the source enodeb. This is the main function   */
/*   which determines the next course of action based on the message rcvd.*/
/*                                                                        */
/* After the receipt of the measurement report if the HO decision has been*/
/*   taken to perform S1 HO, this transaction shall be created.           */
/*                                                                        */
/* Steps involved in S1 HO Source side:                                   */
/*    1. Fill the source to Target Transparent container and send to RRC
         for encoding.
      2. The received RRCEncCfm message shall be having the same transId.
         After the receipt of the Encode Confirm we shall build HO Required
         Message.*/
/*    1. Build the S1 HO Request, start the S1HoRelocPrepTmr              */ 
/*       and send to target enodeb. Change the state within the transCb   */
/*       to HOREQ_SENT state.                                             */
/*    2. When the S1 HO Ack message is received, process the message and  */
/*       stop the S1HoReqAckTmr timer.                                    */
/*    3. Upon receiving, scheduler confirmation, RRC is configured to     */
/*       send out RRC Reconfiguration and the lower layer configuration.  */
/*    4. When RRC Reconfiguration complete is received, an ack is sent    */
/*       to MME.                                                          */
/*                                                                        */
/* Failure:                                                               */
/*    Any faiulre causes RAB setup failure and corrective configuration   */
/*    is performed to undo any changes to other layers.                   */
/*                                                                        */
/* ********************************************************************** */

PUBLIC S16 wrUmmS1SrcHoProcessor
(
WrUmmTransCb                 *transCb
)
{
   WrUmmIncMsg               *msg = transCb->msg;
   S16                        ret;

   switch(msg->msgTyp)
   {
      case WR_UMM_INTERNAL_MSG:
         {
            wrUmmS1SrcHoHdlIntMsg(transCb);
            break;
         }
      case WR_UMM_S1AP_ENC_DEC_CFM:
         {
            /* Snd Handover Required Message to the MME.*/
            if (ROK != wrUmmS1SrcHoPrepHandoverReqrd(transCb))
            {
               RLOG0(L_ERROR, "Failed To Prepare Handover Request Message");
               WR_UMM_S1_SRC_HANDLE_HO_FAIL(transCb);   
               wrUmmTransComplete(transCb);
               RETVALUE(RFAILED);
            }/* End of wrUmmS1SrcHoSndHandoverReq */
            break;
         }
      case WR_UMM_S1AP_ENC_DEC_FAIL:
         {
            /* S1AP Encode/Decode Failed */
            if( SZT_REQ_TYPE_ENCODE_IE == msg->u.sztDecCfm->type.val)
            {
               /* Encoding has failed for SrcToTgt Trans Cont. 
                * So Ho Req is not yet sent. Close the HO Transaction */
               RLOG0(L_ERROR,"Encoding of Src To Tgt Trans Cont failed");
               /* store transId in ueCb */
               transCb->ueCb->hoTransId = 0;
               transCb->ueCb->hoType = WR_HO_TYPE_NONE;
               /* change state of the ue */
               transCb->ueCb->mobCtrlState = WR_HO_NORMAL;
               wrUmmTransComplete(transCb);
            }
            if( SZT_REQ_TYPE_DECODE_IE == msg->u.sztDecCfm->type.val)
            {
               /* Decoding has failed for SrcToTgt Trans Cont.
                * So Ho Cmd is received. Send Ho Cancel to Tgt */
               RLOG0(L_ERROR,"Decoding of Tgt To Src Trans Cont failed");
               transCb->u.s1SrcHoTrans.sztCause->choice.pres = TRUE;
               transCb->u.s1SrcHoTrans.sztCause->choice.val = WR_CAUSE_PROTOCOL;
               transCb->u.s1SrcHoTrans.sztCause->val.radioNw.pres = TRUE;
               transCb->u.s1SrcHoTrans.sztCause->val.radioNw.val = 
                  SztCauseProtmessage_not_compatible_with_receiver_stateEnum;

               wrUmmS1SrcSndHovrCancel(transCb);
            }
            break;
         }
      case WR_UMM_TGTTOSRC_CONT_DEC_CFM:
         {
            /* Need to send Decode Request to RRC to decode RRC container in case 
              of intra LTE HO */  
            if ( SztHovrTypintralteEnum == 
                  transCb->u.s1SrcHoTrans.sztHovrTyp->val)
            {
               ret =  wrUmmS1SrcHoPrcTgtToSrcDecCfm(transCb);
            }
            /* In case of HO To UTRAN  No need of Decoding RRC container,
               immediately after S1AP/RANAP decoding MobilityFromUtraCommand
               could be sent */
            else if ( SztHovrTypltetoutranEnum == 
                        transCb->u.s1SrcHoTrans.sztHovrTyp->val)
            {
               ret = wrUmmS1SrcHoPrcIRatHoTgtToSrcDecCfm(transCb);
            }
            else
            {
               /* TODO: Handling of other hoType */
               RLOG1(L_ERROR,"Invalid/unsupported HO Type[%d]",
                     transCb->u.s1SrcHoTrans.sztHovrTyp->val);
               wrUmmTransComplete(transCb);
               RETVALUE(RFAILED);
            }
            if ( ret  != ROK ) 
            {
               RLOG0(L_ERROR, "Failed To Handle Tgt To Src Cont DEC Cfm");
               
               transCb->u.s1SrcHoTrans.sztCause->choice.pres = TRUE;
               transCb->u.s1SrcHoTrans.sztCause->choice.val = WR_CAUSE_PROTOCOL;
               transCb->u.s1SrcHoTrans.sztCause->val.radioNw.pres = TRUE;
               transCb->u.s1SrcHoTrans.sztCause->val.radioNw.val = 
                                                SztCauseProttransfer_syntax_errorEnum;
               wrUmmS1SrcSndHovrCancel(transCb);

               RETVALUE(RFAILED);
            }
            break;
         }
      case WR_UMM_S1_DAT_IND:
         {
            /* Handle S1ap PDU*/
            if (ROK != wrUmmS1SrcHoHandleS1apPdu(transCb))
            {
               RLOG0(L_ERROR, "Failed to handle S1AP PDU");
               RETVALUE(RFAILED);
            }/* End of wrUmmX2SrcHoHandleX2apPdu*/
            break;
         }
      case WR_UMM_UU_IND_PDU:
         {
            /* Handle RRC Messages.*/
            if (ROK != wrUmmS1SrcHoHandleRRCMsgs(transCb))
            {
               RLOG0(L_ERROR, "Failed To Handle RRC Message");
               RETVALUE(RFAILED);
            }
            break;
         }
      case WR_UMM_RRC_DEC_CFM:
         {
            if (ROK != wrUmmS1SrcHoHandleNhuDecCfm(transCb))
            {
               RLOG0(L_ERROR, "Failed To Handle Nhu Decode confirm");
               transCb->u.s1SrcHoTrans.sztCause->choice.pres = TRUE;
               transCb->u.s1SrcHoTrans.sztCause->choice.val = WR_CAUSE_PROTOCOL;
               transCb->u.s1SrcHoTrans.sztCause->val.radioNw.pres = TRUE;
               transCb->u.s1SrcHoTrans.sztCause->val.radioNw.val = 
                                                SztCauseProttransfer_syntax_errorEnum;
               /* ccpu00130451 */
               wrUmmS1SrcSndHovrCancel(transCb);
               RETVALUE(RFAILED);
            }
            break;
         }
      case WR_UMM_DAM_CFM:
         {
            wrUmmS1SrcHoPrcDamCfm(transCb);
            if  ((WR_UMM_SRC_HO_TNL_CFG_DONE == transCb->u.s1SrcHoTrans.state)
                &&((SztHovrTypltetoutranEnum == 
                      transCb->u.s1SrcHoTrans.sztHovrTyp->val)
                || (SztHovrTypltetogeranEnum == 
                       transCb->u.s1SrcHoTrans.sztHovrTyp->val)))
            {
               wrUmmS1SrcHoGetPdcpSduSta(transCb);
            }
            break;
         }
      case WR_UMM_PDCP_SDU_STATUS_CFM:
         {
            if (ROK != wrUmmS1SrcHoSndEnbSnStaTfr(transCb))
            {
               RLOG0(L_ERROR, "Failed to send Sn Status Tfr Msg");
               RETVALUE(RFAILED);
            }
            break;
         }
      case WR_UMM_RRC_ENC_CFM:
      {
            /*ccpu00133818*/
            if(ROK != wrUmmS1SrcHoHdlRrcEncCfm(transCb))
            {
               WR_UMM_S1_SRC_HANDLE_HO_FAIL(transCb); 
               wrUmmTransComplete(transCb);
               RETVALUE(RFAILED);
            }
            break;
      }
      /*  HOFIX */
      case WR_UMM_RRC_DAT_CFM:
      case WR_UMM_RRC_CFG:
      {
         /* no action */
         break;
      }
      case WR_UMM_SCHD_CFG:
      {
#if 0 /*130053*/         
         if((transCb->msg->u.schdCfm.transId & 0x0f) ==
               WR_UMM_SRC_SCH_CFG_DRX_REL)
         {
            transCb->u.s1SrcHoTrans.state = WR_UMM_SRC_DRX_DISABLED;
            if(transCb->u.s1SrcHoTrans.hoCmd != NULLP)
            {
               NhuDatReqSdus                   *nhuDatReqSdu = NULLP;
               /* Send the RRC Connection Reconfiguration Msg
                * which is already in the encoded format to RRC so that it 
                * sends out to the UE.
                */
               WR_ALLOCEVNT(&nhuDatReqSdu, sizeof(NhuDatReqSdus));
               if (NULLP == nhuDatReqSdu)
               {
                  RLOG0(L_FATAL, "Memory allocation failed.");
                  RETVALUE(RFAILED);
               }
               if (ROK != wrUmmS1SrcHoFillNhuRrcConReCfgnWithMbuf(transCb, 
                        nhuDatReqSdu, transCb->u.s1SrcHoTrans.hoCmd))
               {
                  RLOG0(L_ERROR, "Failed To Fill NhuRrcConReCfgnWithMbuf");
                  RETVALUE(RFAILED);
               }
               /*Start timer RELOCOverall*/
               cmInitTimers(&transCb->ueCb->s1OvrallTmr.tmr,1);
               wrStartUeTmr(transCb->ueCb, WR_TMR_S1_OVRL_TMR, WR_TMR_S1_RELOC_VAL);
               if(ROK != wrIfmRrcSndReqToUe(nhuDatReqSdu))
               {
                  RLOG0(L_ERROR, "VeLiNhuDatReq failed");
                  RETVALUE(RFAILED);
               }

               /*Increment KPI for outgoing Hov attmpd*/
#ifdef WR_RSYS_KPI 
               transCb->ueCb->hovDir = OUT_HOV;
               transCb->ueCb->hov = transCb->u.s1SrcHoTrans.sztHovrTyp->val; 
#endif/*WR_RSYS_KPI*/
               WR_INC_KPI_FUNC4(wrIncKpiFrHovrAttmptd,WR_HO_TYPE_S1,transCb->u.s1SrcHoTrans.sztHovrTyp->val,
                     OUT_HOV,&(transCb->u.s1SrcHoTrans.sztCause));
               /*Increment KPI for Handover related measurements for DRX/non-DRX Attempted*/
               WR_INC_KPI_FUNC1(wrIncKpiFrHONoDrxOutAttmptd,transCb->ueCb->drxCfg.isDrxEnabled);
               /*Increment KPI for Handover attempted related to meas gaps used or not*/
               WR_INC_KPI_FUNC2(wrIncKpiFrOutHoAtmptBsdOnFrqTyp,transCb->ueCb->freqTyp,transCb->ueCb->measGapUsd);

               transCb->ueCb->mobCtrlState = WR_HO_IN_PROGRESS;

               RLOG_ARG1(L_DEBUG, DBG_CRNTI, transCb->ueCb->crnti,"S1 HANDOVER "
                  "state = IN PROGRESS[%d] Cause = Decoded Confirm Message from"
                  "RRC", transCb->ueCb->mobCtrlState);
            /* Get PDCP SN Status */
               RLOG0(L_DEBUG, "Sending PDCP SN Status Req");
            wrUmmS1SrcHoGetPdcpSduSta(transCb);
           }
         }
#endif /* 0 */         
         break;
      }
      case WR_UMM_TMR_EXPIRY:
      {
         wrUmmS1SrcHoPrcTmrExpiry(transCb);
      }
      break;
      case WR_UMM_SND_HANDOVER_CANCEL:
      {
         transCb->u.s1SrcHoTrans.sztCause->choice.pres = TRUE;
         transCb->u.s1SrcHoTrans.sztCause->choice.val = WR_CAUSE_TRANSPORT;
         transCb->u.s1SrcHoTrans.sztCause->val.transport.pres = TRUE;
         transCb->u.s1SrcHoTrans.sztCause->val.transport.val = 
            SztCauseTportunspecifiedEnum;
         /*Stop if any HO timers running */
         wrStopUeTmr(transCb->ueCb, WR_TMR_S1_OVRL_TMR);
         wrStopUeTmr(transCb->ueCb, WR_TMR_S1_RELOC_TMR);

         wrUmmS1SrcSndHovrCancel(transCb);
         break;
      }
      default :
      {
         RLOG1(L_ERROR, "Invalid msgType[%d]", msg->msgTyp);
         RETVALUE(RFAILED);
   }
   }
   RETVALUE(ROK);
}/* End of wrUmmS1SrcHoProcessor*/

/* HO_REEST */
/** @brief This function is called when a reestablishment message is received during
 *         S1 Source Handover transaction .
 *
 * @details
 *
 *     Function: wrUmmS1SrcHoNewTransProcessor
 *
 *         Processing steps:
 *
 * @param[in]  transCb    : transaction information
 * @param[in]  inctransCb : incoming transaction information
 * @return S16
 *    -#Success : ROK
 */
PUBLIC S16 wrUmmS1SrcHoNewTransProcessor
(
WrUmmTransCb                 *transCb,
WrUmmTransCb                 *incTrans
)
{
   WrUmmS1SrcHoTrans                     *s1SrcHoTrans;
   SztUECntxtRlsCmmd                     *ueCtxtRls = NULLP;
   SztProtIE_Field_UECntxtRlsCmmd_IEs    *ueCtxtRlsIes = NULLP;
   SztCause                              rlsCause;
   WrUmmMsgCause                         cause;
   U8                                    numComp = 0;
   U8                                    Idx = 0;
   WrUeCb                                *ueCb = transCb->ueCb;
   S1apPdu                               *s1Pdu;
   TknStrOSXL                            *nasPdu = NULLP;
   S1apPdu                               *pdu;
   SztInitiatingMsg                      *initMsg;
   SztDlnkNASTport                       *dlNas = NULLP;
   SztProtIE_Field_DlnkNASTport_IEs      *ie;

   switch(incTrans->transTyp)
   {
      case WR_UMM_CSFB_REDIR_TRANS:
         {
            transCb->u.s1SrcHoTrans.sztCause->choice.pres = TRUE;
            transCb->u.s1SrcHoTrans.sztCause->choice.val = WR_CAUSE_RADIONW;
            transCb->u.s1SrcHoTrans.sztCause->val.radioNw.pres = TRUE;
            transCb->u.s1SrcHoTrans.sztCause->val.radioNw.val = 
               SztCauseRadioNwcs_fallback_triggeredEnum;
            /*Stop if any HO timers running */
            wrStopUeTmr(transCb->ueCb, WR_TMR_S1_OVRL_TMR);
            wrStopUeTmr(transCb->ueCb, WR_TMR_S1_RELOC_TMR);
            /*Send HO Cancel while CSFB is triggered */
            wrUmmS1SrcSndHovrCancel(transCb);
         }
         break;
      case WR_UMM_RRC_REESTAB_TRANS:
         {
            if( WR_HO_INITIATED == transCb->ueCb->mobCtrlState)
            {
               /* HO preparation phase so defer the HO Transaction */
               RETVALUE(WR_UMM_TRANS_DEFER_ONGOING);
            }
            if( WR_HO_IN_PROGRESS == transCb->ueCb->mobCtrlState)
            { 
             /* ccpu00141244:Increment the counter before updating the cause */
#ifdef WR_RSYS_KPI
               if( SztHovrTypintralteEnum == 
                     transCb->u.s1SrcHoTrans.sztHovrTyp->val)
               {
                  /* Increment the INTRA LTE HO Fail Counter */
                  WR_INC_KPI_FUNC4(wrIncKpiFrHovrFaild, 
                        WR_HO_TYPE_INTRA_LTE_S1,
                        transCb->u.s1SrcHoTrans.sztHovrTyp->val, OUT_HOV,
                        transCb->u.s1SrcHoTrans.sztCause );
               }
               else if( SztHovrTypltetoutranEnum == 
                     transCb->u.s1SrcHoTrans.sztHovrTyp->val)
               {
                  /* Increment the IRAT UTRA HO Fail Counter */
                  WR_INC_KPI_FUNC4(wrIncKpiFrHovrFaild, WR_HO_TYPE_IRAT_UTRA,
                        transCb->u.s1SrcHoTrans.sztHovrTyp->val, OUT_HOV,
                        (transCb->u.s1SrcHoTrans.sztCause));
               }
               if ( transCb->u.s1SrcHoTrans.srvccHoInd.pres == PRSNT_NODEF )
               {
                  WR_INC_KPI_FUNC3(wrIncKpiFrSrvccFail,
                        transCb->u.s1SrcHoTrans.sztHovrTyp->val,
                        OUT_HOV, (transCb->u.s1SrcHoTrans.sztCause));
               }
#endif

               /*ccpu138253 start*/
               transCb->u.s1SrcHoTrans.sztCause->choice.val = WR_CAUSE_RADIONW;
               transCb->u.s1SrcHoTrans.sztCause->val.radioNw.val =
               SztCauseRadioNwfailure_in_radio_interface_procedureEnum;
               /*ccpu138253 end*/

               /* HO execution phase so send HO Cancel and close HO Transaction */
               wrUmmS1SrcSndHovrCancel(transCb);
               /* Calling Dam function to change the drb state to NORMAL and delete UL/DL forwarding
                * tunnels if any */
               wrIfmDamHdlHoFailure(ueCb->cellId, ueCb->crnti);
               wrUmmTransComplete(transCb);
               wrStopUeTmr(transCb->ueCb,WR_TMR_S1_OVRL_TMR);

            }
         }
         break;
      case WR_UMM_ERAB_SETUP_TRANS:
      case WR_UMM_ERAB_MOD_TRANS:
      case WR_UMM_RAB_REL_TRANS:
         {
            cause.causeTyp = CAUSE_RADIONW;
            /* Handover is ongoing, so sending ERAB set up failure with cause
               value as SztCauseRadioNws1_intra_system_handover_triggeredEnum 
               in case of intra LTE HO or with cause value as 
               SztCauseRadioNws1_inter_system_handover_triggeredEnum in case 
               of inter RAT HO */ 
            /* TODO: As of now  HO  is triggered due to "Time critical" 
               reason, need to take care  to take care the scenario handling
               when other causes are introduced */
            if ( SztHovrTypintralteEnum == 
                  transCb->u.s1SrcHoTrans.sztHovrTyp->val )  
            {
               cause.causeVal = 
                  SztCauseRadioNws1_intra_system_handover_triggeredEnum;
            }
            else
            {
               cause.causeVal = 
                  SztCauseRadioNws1_inter_system_handover_triggeredEnum;
            }
            /* Invoke wrUmmHdlAndPrcRabMsg(), to handle the rab setup/rel/mod 
               during Handover */
            wrUmmHdlAndPrcRabMsg(incTrans,&cause);
            /* Return WR_UMM_TRANS_FAIL, so that new transaction would
               be created for eRAB handling */ 
            RETVALUE(WR_UMM_TRANS_FAIL);
         }
         break;
      case  WR_UMM_CTXT_REL_TRANS:
         {
            if(incTrans->msg->msgTyp == WR_UMM_S1_REL_IND)
            {
               /* Increment Counter only if Ue Ctxt Rls Msg is received from MME */
               ueCtxtRls = &incTrans->msg->u.relInd->pdu->pdu.val.initiatingMsg.value.u.sztUECntxtRlsCmmd;

               numComp = ueCtxtRls->protocolIEs.noComp.val;
               for(Idx = 0; Idx < numComp; Idx++)
               {
                  ueCtxtRlsIes = &ueCtxtRls->protocolIEs.member[Idx];
                  if (NULLP == ueCtxtRlsIes)
                  {
                     RLOG0(L_ERROR,"There are No IEs present");
                     RETVALUE(RFAILED);
                  }/* End of if condition.*/

                  if( Sztid_Cause == ueCtxtRlsIes->id.val)
                  {
                     /* Retrieve the cause IE */ 
                     rlsCause = ueCtxtRlsIes->value.u.sztCause;

                     if(( WR_HO_IN_PROGRESS == transCb->ueCb->mobCtrlState) && 
                           ( WR_CAUSE_RADIONW == rlsCause.choice.val) &&
                           ( SztCauseRadioNwsuccessful_handoverEnum == 
                             rlsCause.val.radioNw.val))
                     {
                        /* Only if the release cause is successful Handover 
                         * increment the counter */
                        s1SrcHoTrans = &(transCb->u.s1SrcHoTrans);
                        if( SztHovrTypintralteEnum == 
                              s1SrcHoTrans->sztHovrTyp->val)
                        {
                           /* Successful HO Cntrs based on per cause */
                           WR_INC_KPI_FUNC5(wrIncKpiFrHovrSuccd, 
                                 WR_HO_TYPE_INTRA_LTE_S1,
                                 s1SrcHoTrans->sztHovrTyp->val, OUT_HOV,
                                 s1SrcHoTrans->sztCause,
                                 &(s1SrcHoTrans->t.intraLteInfo.ecgi));
                           /* Successful HO Cntrs based on Intra & Inter 
                            * Freq HO */
                           WR_INC_KPI_FUNC2(wrIncKpiFrOutHoSuccdBsdOnFrqTyp, 
                                 transCb->ueCb->freqTyp, 
                                 transCb->ueCb->measGapUsd); 
                           /*Increment KPI for handover based on Drx Cycle used or not*/
                           WR_INC_KPI_FUNC1(wrIncKpiFrHONoDrxOutSuccd,
                                 transCb->ueCb->drxCfg.isDrxEnabled);
                        }
                        else if( SztHovrTypltetoutranEnum == 
                              s1SrcHoTrans->sztHovrTyp->val)
                        {
                           /* Increment the IRAT Success HO Counter */
                           WR_INC_KPI_FUNC5(wrIncKpiFrHovrSuccd, 
                                 WR_HO_TYPE_IRAT_UTRA,
                                 s1SrcHoTrans->sztHovrTyp->val, OUT_HOV,
                                 s1SrcHoTrans->sztCause, NULLP);
                        }
                        if ( s1SrcHoTrans->srvccHoInd.pres == PRSNT_NODEF )
                        {

                           WR_INC_KPI_FUNC3(wrIncKpiFrSrvccSucc,
                                 transCb->u.s1SrcHoTrans.sztHovrTyp->val,
                                 OUT_HOV, (transCb->u.s1SrcHoTrans.sztCause));
                        }
                     }
                  }
               }
            }
         }
         break;
      case WR_UMM_ANR_MEAS_TRANS:
      case WR_UMM_EVENT_ANR_TRANS:
         {
            /* As the handover transaction is ongoing ANR
               transaction's shouldn't be started */
            RETVALUE(WR_UMM_TRANS_FAIL);
         }
         break;
      case WR_UMM_UL_NAS_TRANS:
         {
            RETVALUE(WR_UMM_TRANS_OK);
         }
         break;
      case WR_UMM_DL_NAS_TRANS:
         {
            pdu = incTrans->msg->u.s1Pdu->pdu;
            initMsg = &pdu->pdu.val.initiatingMsg;
            dlNas   = &initMsg->value.u.sztDlnkNASTport;
            switch(transCb->ueCb->mobCtrlState)
            {
               case WR_HO_NORMAL:
                  {
                     RETVALUE(WR_UMM_TRANS_OK);
                  }
                  break;
               case WR_HO_INITIATED:
                  {
                     for(Idx = 0; Idx < dlNas->protocolIEs.noComp.val; Idx++)
                     {
                        ie = &dlNas->protocolIEs.member[Idx];
                        if((ie->id.val == Sztid_HovrRestrnLst) && 
                              (ueCb->ueServTyp == WR_UMM_NORMAL_SERV))
                        {
                           /* Cancel the ongoing handover procedure 
                            * and proceed with DL NAS */
                           transCb->u.s1SrcHoTrans.sztCause->choice.val = WR_CAUSE_RADIONW;
                           transCb->u.s1SrcHoTrans.sztCause->val.radioNw.val =
                              SztCauseRadioNwhandover_cancelledEnum;

                           wrUmmS1SrcSndHovrCancel(transCb);
                           wrStopUeTmr(transCb->ueCb,WR_TMR_S1_RELOC_TMR);
                           RETVALUE(WR_UMM_TRANS_OK);
                        }
                     }
                  }
                  break;
               default:
                  break;
            }
            /* Send nas non delivery */
            cause.causeTyp = CAUSE_RADIONW;
            if ( SztHovrTypintralteEnum ==
                  transCb->u.s1SrcHoTrans.sztHovrTyp->val)
            {
               cause.causeVal = 
                  SztCauseRadioNws1_intra_system_handover_triggeredEnum;
            }
            else 
            {
               cause.causeVal = 
                  SztCauseRadioNws1_inter_system_handover_triggeredEnum;
            }

            ie = dlNas->protocolIEs.member;
            for(Idx = 0; Idx < dlNas->protocolIEs.noComp.val; Idx++)
            {
               if (ie->id.val == Sztid_NAS_PDU)
               {
                  nasPdu = &ie->value.u.sztNAS_PDU;
                  break;
               }
               ie = ie + 1;
            }
            WR_INC_KPI_FUNC3(wrIncKpiFrS1apFailCause, cause.causeTyp, 
                  cause.causeVal, INC_KPI_VALUE_BY_ONE);
            /* CCB_MERGE : KW Fix */
            if(nasPdu != NULLP)
            {
               RLOG_ARG4(L_INFO, DBG_CRNTI,ueCb->crnti,"Sending NAS NON"
                     "DELIVER INDICATION MME-UE-S1AP-ID[%lu] eNB-UE-S1AP-ID[%lu]"
                     "Cause Type[%d] Cause Value[%d]",
                     ueCb->s1ConCb->mme_ue_s1ap_id, 
                     ueCb->s1ConCb->enb_ue_s1ap_id,
                     cause.causeTyp, cause.causeVal);

               wrUmmS1apFillNasNonDlvInd(ueCb->s1ConCb, &s1Pdu, nasPdu, &cause);
               wrIfmS1apSndUeMsg(ueCb->s1ConCb->spConnId, s1Pdu);
            }
            else
            {
               RLOG_ARG4(L_INFO, DBG_CRNTI,ueCb->crnti,"Failed to send NAS NON"
                     "DELIVER INDICATION due to NULL nasPdu MME-UE-S1AP-ID[%lu] eNB-UE-S1AP-ID[%lu]"
                     "Cause Type[%d] Cause Value[%d]",
                     ueCb->s1ConCb->mme_ue_s1ap_id, 
                     ueCb->s1ConCb->enb_ue_s1ap_id,
                     cause.causeTyp, cause.causeVal);
            }
            RETVALUE(WR_UMM_TRANS_FAIL);
         }
         break;
      default:
         {
            RLOG1(L_ERROR,"Invalid transType [%d]",incTrans->transTyp);
         }
   }
   RETVALUE(WR_UMM_TRANS_OK);
}

/** @brief This function is to release the transaction 
 *
 * @details
 *
 *     Function: wrUmmS1SrcHoTransRel
 *
 *         Processing steps:
 *
 * @param[in]  transCb    : transaction information, release the memory
 * allocated for the transaction created
 * @return S16
 *    -#Success : ROK
 */
PUBLIC S16 wrUmmS1SrcHoTransRel
(
WrUmmTransCb                 *transCb
)
{
    /*ccpu00127635 - MEM_LEAK*/
    WrUmmS1SrcHoTrans         *s1SrcTrans = &transCb->u.s1SrcHoTrans;
   TknStrOSXL                 *utraCapCont;

   /* Deallocate any transaction specific information */
   if(s1SrcTrans->sztE_RABLst.member != NULLP)
   {
   WR_FREE(s1SrcTrans->sztE_RABLst.member, 
         s1SrcTrans->sztE_RABLst.noComp.val * sizeof(SztProtIE_SingleCont_E_RABItemIEs)) 
   }
   if(s1SrcTrans->sztCause != NULLP)
   {
   WR_FREE(s1SrcTrans->sztCause,sizeof(SztCause));
   }
   if ( s1SrcTrans->nasSecParamFrmEutran.val != NULLP )
   {

      WR_FREE(s1SrcTrans->nasSecParamFrmEutran.val, 
            s1SrcTrans->nasSecParamFrmEutran.len);
   }
   if(s1SrcTrans->sztHovrTyp != NULLP)
   {
      if ( SztHovrTypltetoutranEnum == s1SrcTrans->sztHovrTyp->val )
      {
         utraCapCont = &(s1SrcTrans->t.interRatInfo.utraUeCapContainer); 
         if(utraCapCont->val != NULLP)
         {
            WR_FREE(utraCapCont->val, utraCapCont->len);
         }
      }
      WR_FREE(s1SrcTrans->sztHovrTyp, sizeof(SztHovrTyp));
   }
   if(s1SrcTrans->Cause != NULLP)
   {
   WR_FREE(s1SrcTrans->Cause, sizeof(WrUmmMsgCause));
   }
   if(s1SrcTrans->rbInfo != NULLP)
   {
   WR_FREE(s1SrcTrans->rbInfo, 
         s1SrcTrans->numOfRbInfo * sizeof(WrUmmS1SrcHoRabInfo));
   }
   RETVALUE(ROK);
}


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
