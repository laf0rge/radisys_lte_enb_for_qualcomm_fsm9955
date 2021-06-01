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
               

     File:     wr_umm_csfb.c

     Sid:      $SID$ 

     Prg:      Sriky 

*********************************************************************21*/

/* header include files (.h) */
static const char* RLOG_MODULE_NAME="UMM";
static int RLOG_FILE_ID=49;


static int RLOG_MODULE_ID=32;

#include "wr.h"
#include "wr_utils.h"
#include "wr_umm.h"
#include "wr_umm_rrc_msg.h"
#include "wr_umm_trans.h"
#include "wr_ifm_rrc.h"
#include "nhu_asn.x"
#include "wr_lwr.h"
#include "wr_umm_meas_fun.h"
#include "wr_umm_csfbupdtcell.h"
#include "wr_umm_ecsfb_utils.h"
#include "wr_kpi.h"
#include "wr_kpiIds.h"

/** @brief This function will create RRC Connection Release message for CSFB.    
 *        and should be called after cf fallback timer expires or all expected 
 *        Measurement reports received.  
 * @details
 *
 *     Function: wrUmmCsfbSndRrcConRelMsg
 *
 *         Processing steps:
 *         - Read PSC from UeCb's PSC list.
 *         - If WCDMA PSC exists in UECb, Check if UE is release-8 or release-9
 *         supported by reading UE's capability information(Stratum Release 
 *         Indicator).
 *         - If UE is release-8 supported, fill the RRC Connection Release 
 *         Message with arfcn read in first step and send it to UE.  
 *         - Else if UE is release-9 supported,check if redirection to UTRA
 *         with SI is supported by reading  e-RedirectionUTRA-r9(UE capability),
 *         if yes fill RRC Connection Release Message with SI of PSC read in 
 *         first step else send without SI.
 *
 * @param [in] transCb : transation Control block
 * @return S16
    -# Success : ROK
    -# Failure : RFAILED 
 */
PRIVATE S16 wrUmmCsfbSndRrcConRelMsg
(
 WrUmmTransCb                *transCb
)
{
   NhuDatReqSdus             *sdu;
   U8                        accessStratumRls;
   WrUmmCsfbTransCb          *csfbTransCb = &transCb->u.csfbTrans;
   WrUeCapInfo               *ueCap = transCb->ueCb->ueCap;
   U32                        t320Val;
   
   WR_ALLOCEVNT(&sdu, sizeof(*sdu));
   if (sdu == NULLP)
   {
      RLOG0(L_FATAL, "Memory allocation faiiled.");
      RETVALUE(RFAILED);
   }
   accessStratumRls = ueCap->eUTRA_Cap.accessStratumRls.val;
   if(csfbTransCb->redirectInfo != NULLP)
   {
      csfbTransCb->redirectInfo->eRedirUtraR9Sup = FALSE;
      if(( ueCap->eUTRA_Cap.nonCriticalExtn.pres.pres == PRSNT_NODEF) &&
            ( ueCap->eUTRA_Cap.nonCriticalExtn.interRAT_ParamsUTRA_v920.pres.pres == PRSNT_NODEF))
      {
         if( ueCap->eUTRA_Cap.nonCriticalExtn.interRAT_ParamsUTRA_v920.e_RedirectionUTRA_r9.val
               == NhuIRAT_ParamsUTRA_v920e_RedirectionUTRA_r9supportedEnum)
         {
            csfbTransCb->redirectInfo->eRedirUtraR9Sup = TRUE; 
         }
      }
      /*Used to differentiate CSFB Redirection from Redirection procedure*/
      csfbTransCb->redirectInfo->redirType = WR_UMM_CSFB;
   }
	else
	{
		if ((csfbTransCb->ratPriority == UTRA_FDD) || (csfbTransCb->ratPriority == UTRA_TDD))
		{
			/*Increment UTRA KPI_ID_LTE_CSFB_UTRA_WITHOUT_REDIRECTION */
		   transCb->ueCb->csfbInfo.redirType = UTRA_CSFB_WITHOUT_REDIRECTION;
		}
   }
   wrUmmFillRrcEvntHdr(&sdu->hdr, transCb->ueCb->cellId, transCb->ueCb->crnti,
         transCb->transId);
   /* 
    *Setting transaction ID for above allocation transaction Id
    * */
   sdu->hdr.transId = transCb->transId | WR_UMM_CSFB_RRC_CON_REL_TRANSID;
   /* T320 Timer value has to be taken from HeMS */
   t320Val = 0;
   if (wrUmmRrcBldConRls(transCb->ueCb, accessStratumRls, csfbTransCb->redirectInfo, sdu, NhuRlsCauseotherEnum, t320Val) != ROK)
   {
      RETVALUE(RFAILED);
   }
   sdu->sdu.isUeCfgPres = FALSE;
/*
   RLOG_ARG2(L_DEBUG, DBG_CRNTI,transCb->ueCb->crnti,"Redirecting UE to "
      "UARFCN[%d] with PSC due to CSFB, S-TMSI[%d]",
      csfbTransCb->redirectInfo.val.utraInfo.arfcn, transCb->ueCb->sTMSI.mTMSI);
*/
   RLOG_ARG0(L_INFO,DBG_CRNTI,transCb->ueCb->crnti,"wrUmmCsfbSndRrcConRelMsg: Sending RRC Connection Release");
   wrIfmRrcSndReqToUe(sdu);
   /*Start RRC Connection Release Timer*/
   RETVALUE(ROK);
}


/** @brief This function will be called when any message receives from UE 
 *     to enodeb which is belongs to CSFB.   
 * @details
 *
 *     Function: wrUmmCsfbMeasPrcRrcMsg
 *
 *         Processing steps:
 *         - If measurement report receives give to CSFB for CS Fallback is 
 *         required, call the measurement API for measurement reports.
 *         - If above mentioned parameter is not set,call 
 *         wrUmmCsfbSndRrcConRelMsg() to send RRC Connection Release message
 *         to UE.In this case UE Cb will have PSC's for fallback based on 
 *         REM/periodic measurement for ANR. 
 *
 * @param [in] ueCb
 * @param [in] cellId 
 * @return S16
    -# Success : ROK
    -# Failure : RFAILED 
 */ 
PUBLIC S16 wrUmmCsfbRelUe
(
 WrUmmTransCb                *transCb
)
{
   TRC2(wrUmmCsfbRelUe)
   RLOG0(L_DEBUG, "wrUmmCsfbRelUe Called");
      /*send RRC connection release to UE*/
   if(RFAILED == wrUmmCsfbSndRrcConRelMsg(transCb))
   {
      RLOG0(L_ERROR, "Sending RRC Connection Release Failed");
      RETVALUE(RFAILED);
   }
   transCb->ueCb->rrcConnState = WR_UMM_RRC_IDLE;
   transCb->u.csfbTrans.csfbState = WR_UMM_CSFB_TRANS_STATE_RRC_CONREL;
   wrStartTransTmr(transCb,WR_TMR_RRC_CONREL, 
                     wrEmmGetRrcConRelTmrVal (transCb->ueCb->cellId));
   RETVALUE(ROK);
}
/** @brief This function is called to Fill default candidate Uarfcn into
 * candidate selection list and send RRC Coneection release to UE
 *
 * @details
 *
 *     Function: wrUmmCsfbFillDfltArfcnSndRrcRelToUe
 *
 *        Description:
 *       Choose Default Arfcn supported  by UE 
 *       Send RRC Connection release to UE
 *
 * @param[in]  transCb    : pointer to transaction control block
 * @return S16
 *    -#Success : ROK
 *    -#Failure : RFAILED
 */
PUBLIC S16 wrUmmCsfbFillDfltArfcnSndRrcRelToUe
(
 WrUmmTransCb *transCb
)
{
   WrUeCb                    *ueCb      = NULLP;
   WrUmmRedirectInfo         *redirInfo = NULLP;
   U32                       ratPriority;
   WrUmmCellCb               *cellCb;
   U8                        numOfCdmaCells = 0;
	Bool 								laiCellsTobeSent = FALSE;

   TRC2(wrUmmCsfbFillDfltArfcnSndRrcRelToUe)
   RLOG0(L_DEBUG, "wrUmmCsfbFillDfltArfcnSndRrcRelToUe Called");   
   
   ueCb = transCb->ueCb;
   WR_UMM_GET_CELLCB(cellCb, ueCb->cellId)
   if(NULLP == cellCb)
   {
      RLOG0(L_ERROR, "Failed to get cellCB ");
      RETVALUE(NULLP);
   }
   transCb->u.csfbTrans.redirectInfo = NULLP;
   /*Check here the PLMN RAT priority if RAT priority is UTRA/WCDMA then
    * call the handler to fill UTRA re-direction Info
    * If RAT priority is CDMA then call the handler which will either fill
    * CDMA information in re-direction IE or will not fill re-direction IE(when
    * dualRx is TRUE or NumCells is Zero or Band list is not supporte)
    */
	ratPriority = transCb->u.csfbTrans.ratPriority;
   ueCb->csfbInfo.ratType = ratPriority;
	
   if(CDMA2000_1XRTT == ratPriority)
   {
      numOfCdmaCells = cellCb->measCellCb->cdma1xMeasObjLst.count;

      if(0 == numOfCdmaCells)
      {
         redirInfo = NULLP;
      }
      else
      {
         /*if Release 8 then fill re-direction Info if supported band configured
          * if num of CDMA cell is 0, No re-direction will be filled
          */
         if(NhuAccessStratumRlsrel8Enum == ueCb->ueCap->
               eUTRA_Cap.accessStratumRls.val)
         {
            RLOG0(L_DEBUG, "Rel 8 UE :: wrUmmFillCdmaRedirectInfo");
            redirInfo = wrUmmFillCdmaRedirectInfo(ueCb, ratPriority);
            transCb->u.csfbTrans.redirectInfo = redirInfo;
         }
         else
         {
            /* Release 9 and above UE */
            /*RRC Rel10 Upgrade - start*/
            if(((TRUE == wrUmmUtilGetUeDualRx(ueCb, ratPriority)) &&
                     (TRUE == wrEmmIsDualRxSupported(ueCb->cellId))) ||
                  ((TRUE == wrUmmUtilGetUeDualRxTx(ueCb,ratPriority)) &&
                   (TRUE == wrEmmIsDualRxTxSupported(ueCb->cellId))))
               /*RRC Rel10 Upgrade - end*/
            {
               redirInfo = NULLP;
            }
            else
            {
               redirInfo = wrUmmFillCdmaRedirectInfo(ueCb, ratPriority);
               transCb->u.csfbTrans.redirectInfo = redirInfo;
            }
         }
      }
   }
   else if(UTRA_FDD ==  ratPriority)
   {
	   if (ueCb->regLai.isLaiPres == TRUE)
	   {
	      laiCellsTobeSent = TRUE;
	   }
      redirInfo = wrUmmFillUtraFddRedirectInfo(ueCb, laiCellsTobeSent);
   }
   else if(UTRA_TDD ==  ratPriority)
   {
	   if (ueCb->regLai.isLaiPres == TRUE)
	   {
	      laiCellsTobeSent = TRUE;
	   }
      redirInfo = (WrUmmRedirectInfo *) wrUmmFillDfltUtraTddRedirectInfo(ueCb, laiCellsTobeSent);
   }

   if(redirInfo != NULLP)
   {
      transCb->u.csfbTrans.redirectInfo = redirInfo;
      /* UE is Rls 8 & Re-Direction Info is sent 
       * So Increment the REDIRECTION Counter */
      if(ratPriority == CDMA2000_1XRTT)
      {
      ueCb->csfbInfo.redirType = CDMA2000_CSFB_WITH_REDIRECTION;
      }
      /*
         cmMemcpy((U8*)&transCb->u.csfbTrans.redirectInfo, (U8*)redirInfo,
         sizeof(WrUmmRedirectInfo)); */
   }
   else
   {
      /* To fill the re-direction type and rat type for counters */
      /* UE has dual Rx support & Re-Direction Info is not sent 
       * So Increment the DUALRX_WO_REDIRECTION Counter */
      if(ratPriority == CDMA2000_1XRTT)
      {
      ueCb->csfbInfo.redirType = CDMA2000_CSFB_WITHOUT_REDIRECTION;
      }
   }
   wrUmmCsfbRelUe(transCb);
   RETVALUE(ROK);
}
/** @brief This function will be called when CS Fallback Indicator
 *          is received either in context modication or intial context 
 *          setup Request.   
 * @details
 *
 *     Function: wrUmmCsfbPrcIntMsg
 *
 *         Processing steps:
 *         - Read the Configuration Parameter "if measurement required".
 *         - If measurement for CS Fallback is required, call the measurement 
 *           API for measurement reports.
 *         - If above mentioned parameter is not set,call 
 *             wrUmmCsfbSndRrcConRelMsg() to send RRC Connection Release message
 *             to UE.In this case UE Cb will have PSC's for fallback based on 
 *             REM/periodic measurement for ANR. 
 *           
 * @param [in] transCb
 * @return S16
    -# Success : ROK
    -# Failure : RFAILED 
 */ 
PRIVATE S16 wrUmmCsfbPrcIntMsg
(
 WrUmmTransCb                *transCb
)
{
   WrUmmCellCb                  *cellCb;
   WrUeCb                       *ueCb;
   WrUmmIncMsg                  *msg = transCb->msg;
   U16                          cellId;
   WrUmmCsfbTransCb             *csfbTransCb = &transCb->u.csfbTrans;
   WrUmmIntCsfbTransMsg         *csfbIntMsg;
   U8                           csfbUtraFlag = LWR_CSFB_WITHOUT_MEAS;
   U8                           csfbCdmaFlag = LWR_CSFB_WITHOUT_MEAS;

   TRC2(wrUmmCsfbPrcIntMsg)

   ueCb   = transCb->ueCb;
   cellId = ueCb->cellId;
   csfbIntMsg = msg->u.internalMsg->u.csfbIndMsg;
   /* Used during re-establishment to avoid creating a new transaction*/
   ueCb->csfbRedirTransId = transCb->transId;
   csfbTransCb->ratPriority   = csfbIntMsg->ratPriority;

   WR_UMM_GET_CELLCB(cellCb, cellId)
   if(NULLP == cellCb) 
   {
      RLOG_ARG0(L_ERROR, DBG_CELLID,(U32)ueCb->cellId,
            "Error in getting CELL CB");
      RETVALUE(RFAILED);
   }

   /*Store the Fall-back RAT Priority in csfbTransCb , this RatPriority
    * Information will be used later while filling the redirection information
    * in RRC Connection release message*/
   
   if((msg->u.internalMsg->intMsgType == WR_UMM_INTMSG_CSFB_REDIR_TRANS)
         && (GERAN != csfbIntMsg->ratPriority))
   {
      if(UTRA_FDD == csfbTransCb->ratPriority)
      {
         RLOG0(L_DEBUG, "Processing UTRA_FDD CSFB RAT");
         /*In case of RE-establishment, this part of code will be hit*/
         if (csfbTransCb->csfbState == WR_UMM_CSFB_TRANS_STATE_MEAS_CFGD)
         {
				if (csfbTransCb->redirectInfo == NULLP)
            {
               csfbTransCb->redirectInfo = 
                  (WrUmmRedirectInfo *) wrUmmFillUtraFddRedirectInfo(ueCb, TRUE);
            }
            wrUmmCsfbRelUe(transCb);
            RETVALUE(ROK);
            
         }
         if (csfbTransCb->csfbState == WR_UMM_CSFB_TRANS_STATE_RRC_CONREL)
         {
            csfbTransCb->redirectInfo = 
                  (WrUmmRedirectInfo *) wrUmmFillUtraFddRedirectInfo(ueCb, TRUE);
            wrUmmCsfbRelUe(transCb);
            RETVALUE(ROK);
         }
            
         
         if(ROK != wrEmmChkCsfbToUtraMeasSup(ueCb->cellId, &csfbUtraFlag))
         {
            RETVALUE(RFAILED);
         }
         if(WR_UMM_CSFB_WITH_MEAS == csfbUtraFlag)
         {
            RLOG0(L_DEBUG, "CSFB to UTRA FDD is Configured with Meas");
            /* check if only one UTRA Neighor is configured
             * then release the UE with default configured redirInfo*/

            if(RFAILED == wrUmmChkOneUtraNeigh(ueCb, csfbTransCb->ratPriority))
            {
               RLOG0(L_DEBUG, "UTRA FDD Configred with Only one NGH"
                                     "So Sending Defalut Redirection");
               wrUmmCsfbFillDfltArfcnSndRrcRelToUe(transCb);
               RETVALUE(ROK);
            }
            RLOG0(L_DEBUG, "CSFB with MEAS selected::CSFBPriority is UTRA");
            /*check for FGI and supported band list for UTRA and if supported
             * then call measurement Gap module to get the gap pattern */
            if((ROK != wrUmmMeasCsfbUtraFddCfg(ueCb, transCb)))
            {
               RLOG0(L_DEBUG, "wrUmmMeasCsfbUtraFddCfg failed, sending Default Redirection");
               wrUmmCsfbFillDfltArfcnSndRrcRelToUe(transCb);
               RETVALUE(ROK);
            }
            csfbTransCb->csfbState = WR_UMM_CSFB_TRANS_STATE_MEAS_CFGD;
         }
         else
         {
            RLOG0(L_ERROR, "CSFB to UTRA FDD is Configured without Meas"
                            "So Sending UE Release with Default Redirection");
            /* Without Measurement else Case */
            wrUmmCsfbFillDfltArfcnSndRrcRelToUe(transCb);
            RETVALUE(ROK);
         }
      }
		else if(UTRA_TDD == csfbTransCb->ratPriority)
      {
         RLOG0(L_DEBUG, "Processing UTRA_TDD CSFB RAT");
         /*In case of RE-establishment, this part of code will be hit*/
         if (csfbTransCb->csfbState == WR_UMM_CSFB_TRANS_STATE_MEAS_CFGD)
         {
            RLOG0(L_DEBUG, "In WR_UMM_CSFB_TRANS_STATE_MEAS_CFGD State ");
				if (csfbTransCb->redirectInfo == NULLP)
            {
               csfbTransCb->redirectInfo = 
                  (WrUmmRedirectInfo *) wrUmmFillDfltUtraTddRedirectInfo(ueCb, TRUE);
            }
            wrUmmCsfbRelUe(transCb);
            RETVALUE(ROK);
            
         }
         
         /*In case of RE-establishment, this part of code will be hit*/
         if (csfbTransCb->csfbState == WR_UMM_CSFB_TRANS_STATE_RRC_CONREL)
         {
            csfbTransCb->redirectInfo = 
               (WrUmmRedirectInfo *) wrUmmFillDfltUtraTddRedirectInfo(ueCb, TRUE);
            wrUmmCsfbRelUe(transCb);
            RETVALUE(ROK);
         }
         
         if(ROK != wrEmmChkCsfbToUtraMeasSup(ueCb->cellId, &csfbUtraFlag))
         {
            RETVALUE(RFAILED);
         }
         if(WR_UMM_CSFB_WITH_MEAS == csfbUtraFlag)
         {
            RLOG0(L_DEBUG, "CSFB to UTRA TDD is Configured with Meas");
            /* check if only one UTRA Neighor is configured
             * then release the UE with default configured redirInfo*/

            if(RFAILED == wrUmmChkOneUtraNeigh(ueCb, csfbTransCb->ratPriority))
            {
               RLOG0(L_DEBUG, "UTRA TDD Configred with Only one FREQ"
                                     "So Sending Defalut Redirection");
               wrUmmCsfbFillDfltArfcnSndRrcRelToUe(transCb);
               RETVALUE(ROK);
            }

            RLOG0(L_DEBUG, "CSFB with MEAS selected::CSFBPriority is UTRA");
            /*check for FGI and supported band list for UTRA and if supported then call measurement Gap module to get the gap pattern */
            if((ROK != wrUmmMeasCsfbUtraTddCfg(ueCb, transCb)))
            {
               RLOG0(L_ERROR, "wrUmmMeasCsfbUtraTddCfg failed, sending Default Redirection");
               wrUmmCsfbFillDfltArfcnSndRrcRelToUe(transCb);
               RETVALUE(ROK);
            }
            csfbTransCb->csfbState = WR_UMM_CSFB_TRANS_STATE_MEAS_CFGD;
         }
         else
         {
            RLOG0(L_ERROR, "CSFB to UTRA TDD is Configured without Meas"
                            "So Sending UE Release with Default Redirection");
            /* Without Measurement else Case */
            wrUmmCsfbFillDfltArfcnSndRrcRelToUe(transCb);
            RETVALUE(ROK);
         }
      }
      else if(CDMA2000_1XRTT == csfbTransCb->ratPriority)
      {
         if(ROK != wrEmmIsCsfbOrEcsfbMeasSup(ueCb->cellId, FALSE, &csfbCdmaFlag))
         {
            RETVALUE(RFAILED);
         }
         if(WR_UMM_CSFB_WITH_MEAS == csfbCdmaFlag)
         {
            RLOG0(L_DEBUG, "CSFB with MEAS selected::CSFBPriority is CDMA");
            /* check if only one BAND and FREQ is configured
             * then release the UE with default configured redirInfo*/

            /* Fix for ccpu00137043 */
            if(RFAILED == wrUmmChkOneBandAndFreq(ueCb))
            {
               RLOG0(L_DEBUG, "1xRTT Configred with Only one NGH"
                     "So Sending Defalut Redirection");
               wrUmmCsfbFillDfltArfcnSndRrcRelToUe(transCb);
               RETVALUE(ROK);
            }
            /*check for FGI and supported band list for UTRA and if supported
             * then call measurement Gap module to get the gap pattern */
            if((ROK != wrUmmMeasCsfbCdmaCfg(ueCb, transCb)))
            {
               wrUmmCsfbFillDfltArfcnSndRrcRelToUe(transCb);
               RETVALUE(ROK);
            }
            csfbTransCb->csfbState = WR_UMM_CSFB_TRANS_STATE_MEAS_CFGD;
         }
         else
         {
            RLOG0(L_ERROR, "wrUmmMeasCsfbUtraFddCfg failed, sending Default Redirection");
            /* Without Measurement else Case */
            wrUmmCsfbFillDfltArfcnSndRrcRelToUe(transCb);
            RETVALUE(ROK);
         }
      }
      /* other RAT types not supported */
      else
      {
         RLOG0(L_ERROR, "RAT Type not Supported To Perform CSFB\n");
      }
   }
   else if(msg->u.internalMsg->intMsgType == WR_UMM_INTMSG_CSFB_ECSFB_TRANS)
   {
      RLOG0(L_ERROR, "WR_UMM_INTMSG_CSFB_ECSFB_TRANS, sending Defalt Redirection");
      wrUmmCsfbFillDfltArfcnSndRrcRelToUe(transCb);
   }
   else if((msg->u.internalMsg->intMsgType == WR_UMM_INTMSG_CSFB_REDIR_TRANS)
         && (GERAN == csfbIntMsg->ratPriority)) 
   {
      /*Set CSFB GERAN UE state to RRC_REDIR*/
      ueCb->csfbInfo.csfbState = WR_CSFB_GSM_UE_STATE_RRC_REDIR;
      ueCb->csfbInfo.nwDTMCap = TRUE; /* setting this to TRUE for deciding rls cause as per SRS */

      wrUmmCsfbGeranFillArfcnSndRrcRelToUe(transCb);
   }
   RETVALUE(ROK);
}

/** @brief This function will be called when any message receives from UE 
 *     to enodeb which is belongs to CSFB.   
 * @details
 *
 *     Function: wrUmmCsfbMeasPrcRrcMsg
 *
 *         Processing steps:
 *         1. if no of Pilots is ZERO then return RFAILED
 *         2. Find the best Pilot in the PilotList table and store the
 *         corresponding bandclass and frequency in output parameters
 *
 * @param [in]  csfbTransCb
 * @param [out] bandClass 
 * @param [out] arfcn 
 * @return S16
    -# Success : ROK
    -# Failure : RFAILED 
 */ 
PRIVATE S16 wrUmmCdmaGetHighestBandFreq
(
 WrUmmCsfbTransCb *csfbTransCb, 
 U8               *bandClass,
 U32              *arfcn
)
{
   /*Go through list of all pilots, belonging to all measuremnt object is stored
    * in single table */
   if(csfbTransCb->cdmaRttPilotLst == NULLP)
   {
      RETVALUE(RFAILED);
   }
   *bandClass       = csfbTransCb->cdmaRttPilotLst->bandClass;
   *arfcn           = csfbTransCb->cdmaRttPilotLst->arfcn;
   RETVALUE(ROK);
} /* End of wrUmmCdmaGetHighestBandFreq */

/** @brief This function will be called when any message receives from UE 
 *     to enodeb which is belongs to CSFB.   
 * @details
 *
 *     Function: wwrUmmCsfbMeasPrcRrcMsg
 *
 *         Processing steps:
 *         - If measurement report receives give to CSFB for CS Fallback is 
 *         required, call the measurement API for measurement reports.
 *         - If above mentioned parameter is not set,call 
 *         wrUmmCsfbSndRrcConRelMsg() to send RRC Connection Release message
 *         to UE.In this case UE Cb will have PSC's for fallback based on 
 *         REM/periodic measurement for ANR. 
 *
 * @param [in] ueCb
 * @param [in] cellId 
 * @return S16
    -# Success : ROK
    -# Failure : RFAILED 
 */ 
PRIVATE S16 wrUmmCsfbMeasPrcRrcMsg
(
 WrUmmTransCb                *transCb
)
{
   NhuUlDcchMsg                          *ulDcch;
   NhuUL_DCCH_MsgTypc1                   *c1;
   NhuMeasurementReport                  *measRpt;
   NhuMeasurementReportcriticalExtnsc1   *measRptC1;
   NhuMeasResults                        *measRes;
   WrUmmCsfbTransCb                      *csfbTransCb = &transCb->u.csfbTrans;
   U8                                    bandClass = 0;
   S16                                   ret = ROK;
   U32                                   arfcn = 0;
  
   TRC2(wrUmmCsfbMeasPrcRrcMsg)

   ulDcch = &transCb->msg->u.uuIndPdu->sdu.m.ulDcchMsg;
   c1 = &ulDcch->dcchMsg.message.val.c1;
   switch(c1->choice.val)
   {
      case C1_MEASUREMENTREPORT:
         {
            RLOG0(L_DEBUG, "wrUmmCsfbMeasPrcRrcMsg:C6_MEASUREMENTREPORT Received");
            measRpt = &c1->val.measurementReport;
            measRptC1 = &measRpt->criticalExtns.val.c1;
            measRes = &measRptC1->val.measurementReport_r8.measResults;
            csfbTransCb->recvMeasRptCnt++; 
            if(RFAILED == wrUmmCsfbHdlMeasRpt(transCb, measRes))
            {
               RLOG0(L_ERROR, "Updation of candidate selection arfcn and "
                     "cell Failed");
            }
            if(csfbTransCb->recvMeasRptCnt == csfbTransCb->measTransCb.
                  addMeasInfo.measIdCnt)
            {
               /*All required measurement reports received from UE, so stop the
                * timer and release the UE with valid re-direction Information if
                * at least ONE MR is valid */

               wrStopTransTmr(transCb, WR_TMR_CSFB_CFG);
               if(CDMA2000_1XRTT == csfbTransCb->ratPriority)
               {
                  /* In case of CSFB to CDMA1xRTT we will process the
                   * cdmaRttPilotList table first to find the best band,arfcn to
                   * be filled in re-direction information on the basis of received
                   * PilotStrength */
                  ret = wrUmmCdmaGetHighestBandFreq(csfbTransCb, &bandClass, &arfcn);
                  if(ROK == ret)
                  {
                     WrUmmRedirectInfo   *redirInfo = NULLP;
                     WR_ALLOC(&redirInfo, sizeof(WrUmmRedirectInfo));
                     if (NULLP == redirInfo)
                     {
                        RLOG0(L_FATAL, "Memory allocation failed. ");
                        RETVALUE(NULLP);
                     }
                     
                        redirInfo->ratTyp = WR_UMM_MEAS_1XCDMA;
                        transCb->ueCb->csfbInfo.ratType = CDMA2000_1XRTT;
                     redirInfo->val.cdmaInfo.bandClass = bandClass;
                     redirInfo->val.cdmaInfo.arfcn = arfcn;
                     csfbTransCb->redirectInfo = redirInfo;
                     transCb->ueCb->csfbInfo.redirType = 
                                           CDMA2000_CSFB_WITH_REDIRECTION;
                     /*Free the memory which was allocated to store the CDMA
                      * measurement report in csfbTransCb
                      */
                     WR_FREE(csfbTransCb->cdmaRttPilotLst, sizeof(WrCdmaRttPilotLst));
                  }
                  else
                  {
                     /* No Valid Pilot Received from CDMA measurement report, so
                      * release the UE with default re-direction information */
                     wrUmmCsfbFillDfltArfcnSndRrcRelToUe(transCb);
                     RETVALUE(ROK);
                  }
               }
                 
					if (UTRA_TDD == csfbTransCb->ratPriority)
					{
                  RLOG0(L_DEBUG, "MR Received for UTRA TDD");
					  /*In case no valid LAI cells are found after all the reports as *
                  * all reports are false reports, we need to send all the frequencies
                  * and cell list based on HRL(if present). In case HRL is also not
                  * present,we will send all the cell information.*/
 					   if (transCb->u.csfbTrans.redirectInfo == NULLP)
                  {
                     transCb->u.csfbTrans.redirectInfo = 
                        (WrUmmRedirectInfo *) wrUmmFillDfltUtraTddRedirectInfo(transCb->ueCb, FALSE);
                  }
					}
               
               if(RFAILED == wrUmmCsfbRelUe(transCb))
               {
                  RLOG0(L_ERROR, "sending RRC connection release message Failed");
                  RETVALUE(RFAILED);
               }
            }
         }
         break;
      case C1_RRCCONRECFGNCOMPL:
      {
         /*Fix for ccpu00127431*/
         /* Stop the RRC Reconfiguration Timer */
         /* wrStopTransTmr(transCb,WR_TMR_RECONFIGURE); */
         /* Remove any measObj/measIds/rptCfg from measUeCb, if it was sent to UE */
         wrUmmMeasCfgReCfgReqCompHand(WR_UMM_MEAS_TYPE_CSFB, transCb);
         /*Start the Timer*/
         cmInitTimers(&(csfbTransCb->csfbCfgTmr.tmr), 1);
         /* Start timer to wait for measurment reports from UE */
         wrStartTransTmr(transCb, WR_TMR_CSFB_CFG, WR_TMR_CSFB_CFG_VAL);
         break;
      }
      default:
      {
         RLOG0(L_WARNING, "Currently there is no handling for dcchMsg message received as part of a UU_IND_PDU");
      }
   }
   RETVALUE(ROK);
}

/** @brief This function is responsible for handling CSFB related messages.
 *
 * @details
 *
 * Function:wrUmmCsfbProcessor
 *
 *   Processing steps:
 *    - Check the message type
 *    - If  message type is measuremnet report call the measurement handler API
 *    - If  message type is reconfiguration complete start the CSFB measuremnt 
 *       Timer
 *    - If  timer expires or expected number of measurement reports are
 *    received, copy Redirected information to UE CB and trigger RRC release
 *    transaction and S1AP UE context release transaction.
 *
 * @param[in]  transCb: pointer to transaction control block
 * @return S16
 *  -#Success : ROK
 *  -#Failure : RFAILED
 */
PUBLIC S16 wrUmmCsfbProcessor
(
 WrUmmTransCb                *transCb
)
{
   WrUmmIncMsg               *msg       = transCb->msg;
   S16                        ret = ROK;
   U32                        arfcn = 0;

   switch(msg->msgTyp)
   {
      case WR_UMM_UU_IND_PDU:
         {
            if(wrUmmCsfbMeasPrcRrcMsg(transCb))
            {
               RLOG0(L_ERROR, "Processing UU Indication message Failed");
               RETVALUE(RFAILED);
            }
            break;
         }
      case WR_UMM_INTERNAL_MSG:
         {
            if(RFAILED == wrUmmCsfbPrcIntMsg(transCb))
            {
               RLOG0(L_ERROR, "Processing Internal message Failed");
               RETVALUE(RFAILED);
            }
            break;
         }
      case WR_UMM_TMR_EXPIRY:
         {
            U8    bandClass = 0;
            switch (transCb->msg->u.tmrExpiry.timerEvnt )
            {
               case WR_TMR_CSFB_CFG:
                  {
                     /* Measurment report wait timer expired. By this time
                      * whatever reports are available will be processed to fill
                      * the re-direction information */
                     RLOG0(L_INFO, "WR_UMM_TMR_EXPIRY:: Measurement Timer Expired");
                     WrUmmCsfbTransCb          *csfbTransCb = &transCb->u.csfbTrans;
                     if(CDMA2000_1XRTT == csfbTransCb->ratPriority)
                     {
                        /* In case of CSFB to CDMA1xRTT we will process the
                         * cdmaRttPilotList table first to find the best band,arfcn to
                         * be filled in re-direction information on the basis of received
                         * PilotStrength */
                        ret = wrUmmCdmaGetHighestBandFreq(&transCb->u.csfbTrans, &bandClass, &arfcn);
                        if(ROK == ret)
                        {
                           WrUmmRedirectInfo   *redirInfo = NULLP;
                           WR_ALLOC(&redirInfo, sizeof(WrUmmRedirectInfo));
                           if (NULLP == redirInfo)
                           {
                              RLOG0(L_FATAL, "Memory allocation failed. ");
                              RETVALUE(NULLP);
                           }
                           redirInfo->ratTyp = WR_UMM_MEAS_1XCDMA;
                           redirInfo->val.cdmaInfo.bandClass = bandClass;
                           redirInfo->val.cdmaInfo.arfcn = arfcn;
                           csfbTransCb->redirectInfo = redirInfo;
                           /*Free the memory which was allocated to store the CDMA
                            * measurement report in csfbTransCb
                            */
                           WR_FREE(csfbTransCb->cdmaRttPilotLst, sizeof(WrCdmaRttPilotLst));
                        }
                        else
                        {
                           /* No Valid Pilot Received from CDMA measurement report, so
                            * release the UE with default re-direction information */
                           wrUmmCsfbFillDfltArfcnSndRrcRelToUe(transCb);
                           RETVALUE(ROK);
                        }
                     }
                     else if (UTRA_TDD == csfbTransCb->ratPriority)
							{
								/*In case no LAI cells are found even after timer expires,
                         * we need to send all the frequencies and cell list 
                         * based on HRL(if present).  In case HRL is also not
                         * present,we will send all the cell information.
                         * */
								if (transCb->u.csfbTrans.redirectInfo == NULLP)
                        {
                           transCb->u.csfbTrans.redirectInfo = 
                              (WrUmmRedirectInfo *) wrUmmFillDfltUtraTddRedirectInfo(transCb->ueCb, FALSE);
                        }
							}
                     else if(UTRA_FDD == csfbTransCb->ratPriority)
                     {
								/*In case no LAI cells are found even after timer expires,
                         * we need to send all the frequencies and cell list 
                         * based on HRL(if present).  In case HRL is also not
                         * present,we will send all the cell information.
                         * */
								if (transCb->u.csfbTrans.redirectInfo == NULLP)
                        {
                           transCb->u.csfbTrans.redirectInfo = 
                              (WrUmmRedirectInfo *) wrUmmFillUtraFddRedirectInfo(transCb->ueCb, FALSE);
                        }
                     }
                     
                     if(RFAILED == wrUmmCsfbRelUe(transCb))
                     {
                        RLOG0(L_ERROR, "sending RRC connection release message"
                              " Failed");
                        RETVALUE(RFAILED);
                     }
                  }
                  break;

               case WR_TMR_RECONFIGURE:
                  {
                     RLOG_ARG0(L_INFO, DBG_CRNTI,transCb->ueCb->crnti,
                           "RRC Connection Reconfiguration timer expired");
		     /* Reconfiguration of measurment timer expiry will release the UE.
		      * So send cancel UE request to RRC for releasing the UE from RRC,
		      * PDCP,RLC and MAC*/
		     if (wrIfmRrcRelUe(transCb->ueCb->cellId, transCb->ueCb->crnti) != ROK)
                     {
                        RLOG0(L_ERROR, "Failure in release of UE at RRC");
                     }  
                     transCb->errorCause = WR_UMM_CTXT_REL_SW_ERROR;
                     transCb->state = WR_UMM_TRANS_RELEASE_CALL; 
                  }
                  break;
               case WR_TMR_RRC_CONREL:
                  {
                     /* In case of Release Timer Expiry, send Cancel UE request
                      * to RRC to clear the UE context from RRC,RLC,MAC and PDCP as
                      * in Errorneous case RRC doesn't free UE context from these
                      * layers */
                     RLOG_ARG0(L_INFO, DBG_CRNTI,transCb->ueCb->crnti,
                           "RRC Connection Release timer expired initiating local ctxt rls");
                     if (wrIfmRrcRelUe(transCb->ueCb->cellId, transCb->ueCb->crnti) != ROK)
                     {
                        RLOG0(L_ERROR, "Failure in release of UE at RRC");
                     }  
                     transCb->errorCause = WR_UMM_UE_LOST;
                     transCb->state = WR_UMM_TRANS_RELEASE_CALL; 
                  }
                  break;
                   
            }
            break;
         }
      case WR_UMM_SCHD_CFG:
      {
         if (transCb->u.csfbTrans.measTransCb.measGapStatus ==
                                             WR_UMM_MEAS_GAP_INIT_SCH_CONFIG)
         {
            RLOG0(L_DEBUG, " Received Schd Gap Setup Cfm ");
#ifdef WR_RSYS_KPI 
               /* Schd Cfm for MeasGap is received */
            transCb->ueCb->measGapUsd = TRUE;
#endif /*WR_RSYS_KPI*/

            /* Change the measGapStatus to UE Config */
            transCb->u.csfbTrans.measTransCb.measGapStatus =
                                               WR_UMM_MEAS_GAP_INIT_UE_CONFIG;

               /*Prepare and send the RRC Reconfiguration message */
               if(ROK != wrUmmMeasPrepareSendRrcRecfg(transCb->ueCb->cellId, 
                        transCb->ueCb, 
                        &transCb->u.csfbTrans.measTransCb, 
                        transCb->transId))
               {
                  /* Failed to send the message to the UE. 
                     So close the transaction and return failure */
                  wrUmmCsfbFillDfltArfcnSndRrcRelToUe(transCb);
                  RETVALUE(ROK);
               }
         }
         else if (transCb->u.csfbTrans.measTransCb.measGapStatus ==
                                                   WR_UMM_MEAS_GAP_REL)
         {
            /*Measurment Gap release is not triggered from CSFB transaction */
            RLOG0(L_DEBUG, "Received Scd Cfm for MeasGap rel");
                  
         }
         break;
      }
      case WR_UMM_RRC_DAT_CFM:
         {
            wrStopTransTmr(transCb,WR_TMR_RRC_CONREL);
            if(msg->u.uuCfmPdu->hdr.transId & WR_UMM_CSFB_RRC_CON_REL_TRANSID)
            {
               if (transCb->msg->u.rrcDatCfm != NULLP)
               {
                  /* In case of Errorneous RRC_DAT_CFM, send Cancel UE request
                   * to RRC to clear the UE context from RRC,RLC,MAC and PDCP as
                   * in Errorneous case RRC doesn't free UE context from these
                   * layers */
                  if (transCb->msg->u.rrcDatCfm->datCfmStatus != NHU_ERRCAUSE_NONE)
                  {
                     RLOG0(L_ERROR,"ACK not Receievd for RRC Connection Release and Relsing UE at RRC");
                     if(wrIfmRrcRelUe(transCb->ueCb->cellId,transCb->ueCb->crnti) != ROK)
                     {
                        RLOG0(L_ERROR, "Failure in release of UE at RRC");
                     }
                  }
                  else
                  {
                      RLOG1(L_DEBUG,"[RRC] CSFB DAT CFM  :%d",transCb->ueCb->crnti);
                  }
               }
               /*send UE CONTEXT RELEASE REQUEST to MME*/
               transCb->errorCause = WR_UMM_CTXT_CSFB_REL;
               transCb->state = WR_UMM_TRANS_RELEASE_CALL; 
            }
            break;
         }
   }

   RETVALUE(ROK);
}

/** @brief This function is called when a new Transaction is created for any
 * other procedure. The new transaction is passed to this function and this
 * function determines the next course of action based on the new
 transaction
 *
 * @details
 *
 *     Function: wrUmmCsfbNewTransProcessor
 *
 *         Description:
 *         This function is called when a new Transaction is
 *         created for any other procedure. The new transaction is passed to
 *         this function and this function update the state of transaction 
 *         based on the new transaction. If incoming transaction is-
 *       a) re-establsihment - This will add the ongoing transaction
 *          into deferred list of re-establsihment and update the status.
 *       b) E-Rab Setup/Modify/Release - Call the wrUmmHdlAndPrcRabMsg API 
 *          which will send the failure response towards S1AP. 
 *          And update the status of incoming transaction.    
 *
 *
 * @param[in]  transCb    : transaction information
 * @param[in]  incTrans   : incoming transaction information
 * @return S16            : WR_UMM_TRANS_DEFER_ONGOING 
 *                          WR_UMM_TRANS_FAIL 
 *                          WR_UMM_TRANS_OK
 */

PUBLIC S16 wrUmmCsfbNewTransProcessor
(
 WrUmmTransCb                *transCb,
 WrUmmTransCb                *incTrans
)
{
   /* Fix for ccpu00124053 - Handling the E-Rab 
    * messages while CSFB is in progress */
   WrUmmMsgCause              cause;
   S16                        ret = WR_UMM_TRANS_FAIL;

   TRC2(wrUmmCsfbNewTransProcessor)
   switch(incTrans->transTyp)
   {
      case WR_UMM_RRC_REESTAB_TRANS:
         {
            U32                       reestabCause; 
            NhuUlCcchMsg              *ulCcchPdu = NULLP;

            ulCcchPdu =  &(incTrans->msg->u.uuIndPdu->sdu.m.ulCcchMsg);
            reestabCause = ulCcchPdu->ccchMsg.message.val.c1.val.rrcConReestbRqst.
               criticalExtns.val.rrcConReestbRqst_r8.reestablishmentCause.val;

            WrUmmCsfbTransCb  *csfbTransCb = &transCb->u.csfbTrans;
            
            if(WR_UMM_CSFB_TRANS_STATE_MEAS_CFGD == csfbTransCb->csfbState)
            {
               wrStopTransTmr(transCb, WR_TMR_CSFB_CFG);
            }
            else
            {
               wrStopTransTmr(transCb, WR_TMR_RRC_CONREL);
            }
            /* cause is RLF case, differ the CSFB transaction
             * post admiting release with re-direction information */
            if(NhuReestbCauseotherFailEnum == reestabCause)
            {
               ret = WR_UMM_TRANS_DEFER_ONGOING;
            }
            /* Reestablishment cause HO Failure occuring at this point is
             * an abnormal scenario. Even though its an abnormal scenario
             * RRC Re-establishment with hoFailure will admit UE and continue to
             * give LTE services is the design decision; close CSFB transaction */
            else if(NhuReestbCausehandoverFailEnum ==  reestabCause)
            {
               transCb->ueCb->csfbInfo.csfbState = WR_CSFB_GSM_UE_STATE_CSFB_READY;
               transCb->state = WR_UMM_TRANS_DONE;
               /*CSFB failure KPI*/
               if((UTRA_FDD == transCb->ueCb->csfbInfo.ratType)
                     || (UTRA_TDD == transCb->ueCb->csfbInfo.ratType))
               {
                  wrIncKpiCsfbFail(UTRA_CSFB);
               }
               else if(GERAN == transCb->ueCb->csfbInfo.ratType)
               {
                  wrIncKpiCsfbFail(GERAN_CSFB);
               }
               else if(CDMA2000_1XRTT == transCb->ueCb->csfbInfo.ratType)
               {
                  wrIncKpiCsfbFail(CDMA2000_CSFB);
               }

              ret = WR_UMM_TRANS_OK;
            }
            /* reconfiguration failure, re-establishment will be rejected
             * close csfb transaction */
            else 
            {
               transCb->state = WR_UMM_TRANS_DONE;
               /*CSFB failure KPI*/
               if((UTRA_FDD == transCb->ueCb->csfbInfo.ratType)
                     || (UTRA_TDD == transCb->ueCb->csfbInfo.ratType))
               {
                  wrIncKpiCsfbFail(UTRA_CSFB);
               }
               else if(GERAN == transCb->ueCb->csfbInfo.ratType)
               {
                  wrIncKpiCsfbFail(GERAN_CSFB);
               }
               else if(CDMA2000_1XRTT == transCb->ueCb->csfbInfo.ratType)
               {
                  wrIncKpiCsfbFail(CDMA2000_CSFB);
               }
               ret = WR_UMM_TRANS_OK;
            }
         }
         break;
         /* Fix for ccpu00124053 - Handling the E-Rab 
          * messages while CSFB is in progress */
      case WR_UMM_ERAB_SETUP_TRANS:
      case WR_UMM_ERAB_MOD_TRANS:
      case WR_UMM_RAB_REL_TRANS:
         {
            cause.causeTyp = CAUSE_RADIONW;
            cause.causeVal = SztCauseRadioNwcs_fallback_triggeredEnum;
            if(ROK == wrUmmHdlAndPrcRabMsg(incTrans,&cause))
            {
               ret = WR_UMM_TRANS_FAIL;
         }
   }
         break;
      case WR_UMM_MEAS_TRANS:
         {
            RLOG0(L_WARNING, "wrUmmCsfbNewTransProcessor Measurement report "
                  "is not handled, as CSFB is ongoing\n");
            ret = WR_UMM_TRANS_FAIL;
            break;
         }
      case WR_UMM_ANR_MEAS_TRANS:
      case WR_UMM_EVENT_ANR_TRANS:
         {
            RLOG0(L_WARNING, "wrUmmCsfbNewTransProcessor not allowing ANR Transaction handling"
                  "as CSFB is ongoing\n");
            ret = WR_UMM_TRANS_FAIL;
            break;
         }
      default:
         {
            ALARM("Transaction %d received at wrUmmCsfbNewTransProcessor\n",
                  incTrans->transTyp);
            ret = WR_UMM_TRANS_OK;
         }
         break;
   }
   RETVALUE(ret);
}

/** @brief This function is called by the reestabshment transaction to intimate
 *   CSFB transaction about reception of reestablishment complete message.
 *   CSFB transaction will take a decision to stop reestablishment transaction
 *   by sending "DEQUEUE_CSFB" .
 *
 * @details
 *
 *     Function: wrUmmCsfbPrcDeferTrans
 *
 *        Description:
 *         This function is called from the Transaction that has CSFB
 *        queued as a deferred transaction. This function is called to process
 *        all messages received by the original transaction, this provides a way
 *        for the CSFB transaction to take appropriate action.
 *        
 *     -#Example:The reestabshment transaction to intimate CSFB transaction 
 *     about reception of reestablishment complete message. CSFB transaction 
 *     will take a decision to stop reestablishment transaction.
 *
 * @param[in]  transCb    : pointer to transaction control block
 * @param[in]  defTransCb : pointer to Deferred transaction control block
 * @return S16
 *    -#Success : ROK
 *    -#Failure : RFAILED
 */
PUBLIC S16 wrUmmCsfbPrcDeferTrans
(
  WrUmmTransCb                *transCb,
  WrUmmTransCb                *defTransCb
)
{
   NhuUL_DCCH_Msg            *ulDcchPdu = NULLP;
   U8                        messageType = 0;
   WrUmmCsfbTransCb          *csfbTransCb = &defTransCb->u.csfbTrans;

   TRC2(wrUmmCsfbPrcDeferTrans)
   /* Currently Handling only UU Ind Messages. Later will handling 
    * remaining messages*/
   if(transCb->msg->msgTyp != WR_UMM_UU_IND_PDU)
   {
      RETVALUE(ROK);
   }
   ulDcchPdu = &(transCb->msg->u.uuIndPdu->sdu.m.ulDcchMsg.dcchMsg);
   messageType = ulDcchPdu->message.val.c1.choice.val;
   switch(messageType)
   {
      case C1_RRCCONREESTBCOMPL:
         {
            if((WR_UMM_CSFB_TRANS_STATE_REDIR_SENT == csfbTransCb->csfbState)
                  || (csfbTransCb->ratPriority == UTRA_FDD) 
                  || (csfbTransCb->ratPriority == UTRA_TDD))
            {
               /*For GERAN case, we are not processing here but in 
                * re-estblishment transaction.*/
               RETVALUE(WR_UMM_TRANS_OK);
            }
            if(WR_UMM_CSFB_TRANS_STATE_MEAS_CFGD == csfbTransCb->csfbState)
            {
               //wrUmmCsfbFillDfltArfcnSndRrcRelToUe(transCb);
               wrUmmCsfbFillDfltArfcnSndRrcRelToUe(defTransCb);
            }
            else
            {
               /*Fix for ccpu00123852 */
               if(RFAILED == wrUmmCsfbRelUe(defTransCb))
               {
                  RLOG0(L_ERROR, "Sending RRC connection release message Failed");
                  RETVALUE(RFAILED);
               }
            }
            /* Since, after re-establishment complete,we will release the UE */
            RETVALUE(WR_UMM_STOP_OLD_TRANSACTION);
         }
   }
   RETVALUE(WR_UMM_TRANS_OK);
}

/** @brief This function is called while releasing the CSFB transaction. 
 *
 * @details
 *
 *     Function: wrUmmCsfbPrcDeferTrans
 *
 *        Description:
 *       Release if any memory allocated for CSFB transaction 
 *
 * @param[in]  transCb    : pointer to transaction control block
 * @return S16
 *    -#Success : ROK
 *    -#Failure : RFAILED
 */
PUBLIC S16 wrUmmCsfbTransRel
(
 WrUmmTransCb *transCb
)
{
   TRC2(wrUmmCsfbTransRel)
      
   if(transCb->u.csfbTrans.redirectInfo != NULLP)
   {
      WR_FREE(transCb->u.csfbTrans.redirectInfo, sizeof(WrUmmRedirectInfo));
   }
   wrUmmStopTransTimers(transCb);
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
