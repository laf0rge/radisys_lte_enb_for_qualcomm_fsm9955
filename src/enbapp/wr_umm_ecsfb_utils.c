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
               

     File:     wr_umm.c

     Sid:      fc_umm_ecsfb_utils.c@@/main/TeNB_Main_BR/tenb_brcm_fdd_3.0.1_to_tdd_GA_4.0_merge/1 - Fri Sep 12 10:54:46 2014

     Prg:      Sriky 

*********************************************************************21*/

/* header include files (.h) */
static const char* RLOG_MODULE_NAME="UMM";
static int RLOG_MODULE_ID=32;
static int RLOG_FILE_ID=120;

#include "wr.h"            /* Common headers and Data Structures */
#include "wr_umm.h"        /* UMM Specific files */
#include "wr_umm_ecsfb_utils.h"        /* UMM Specific files */
#include "wr_umm_trans.h"
#include "wr_ifm_rrm.h"
#include "wr_umm_ho.h"
#include "wr_smm_smallcell.h"
#include "wr_umm_msg.h"
#include "wr_emm.h"
#include "wr_emm_ecsfb_utils.h"
#include "wr_kpi.h"
#include "wr_umm_meas_fun.h"
#include "wr_ifm_rrc.h"
#include "wr_umm_rrc_msg.h"
#include <stdlib.h>
/** @brief This function is responsible for finding out 
 *  if both ENB and UE supports e-CSFB-1XRTT.
 *
 * @details
 *
 *     Function: wrUmmUtilsIsEcsfbSupported 
 *
 * @return BOOL 
 *       -# TRUE
 *       -# FALSE
 *       */
PUBLIC Bool wrUmmUtilsIsEcsfbSupported
(
 WrUeCb                     *ueCb
)
{
   NhuUE_EUTRA_Cap_v920_IEs  *nonCriticalExtn = NULLP;
   Bool                      result = FALSE;
   U8                        eCsfbFlag;
   nonCriticalExtn = &ueCb->ueCap->eUTRA_Cap.nonCriticalExtn;


   if(ROK !=
         wrEmmIsCsfbOrEcsfbMeasSup(ueCb->cellId, TRUE, &eCsfbFlag))
   {
      RLOG0(L_WARNING," ECSFB NOT SUPPORTED \n");
      RETVALUE(RFAILED);
   }
   if(LWR_ECSFB_NOT_SUPPORTED != eCsfbFlag)
   {
      /* ENB supports eCSFB, now check whether UE supports eCSFB or not */
      if((nonCriticalExtn->pres.pres == PRSNT_NODEF) &&
            (nonCriticalExtn->interRAT_ParamsCDMA2000_v920.pres.pres == PRSNT_NODEF))
      {
         if(nonCriticalExtn->interRAT_ParamsCDMA2000_v920.e_CSFB_1XRTT_r9.val
               == NhuIRAT_ParamsCDMA2000_1XRTT_v920e_CSFB_1XRTT_r9supportedEnum)
         {
            result = TRUE;
         }
         else
         {
            result = FALSE;
         }
      }
   }
   else
   {
      RLOG0(L_WARNING," ECSFB NOT SUPPORTED \n");
      result = FALSE;
   }
   if((NULLP != ueCb->hoRstrctInfo) && 
         (PRSNT_NODEF == ueCb->hoRstrctInfo->frbdnInterRATs.pres))
   {
      if((SztForbiddenInterRATsallEnum == ueCb->hoRstrctInfo->frbdnInterRATs.val) || 
            (SztForbiddenInterRATscdma2000Enum == ueCb->hoRstrctInfo->frbdnInterRATs.val))
      {
         result = FALSE;
      }
   }
   RETVALUE(result);
}

/** @brief This function is responsible for finding out 
 *  if UE supports Dual rx-Config or not
 *
 * @details
 *
 *     Function: wrUmmUtilGetUeDualRx
 *
 * @return BOOL 
 *       -# TRUE
 *       -# FALSE
 *       */
PUBLIC Bool wrUmmUtilGetUeDualRx
(
 WrUeCb                     *ueCb,
 U32                        ratType
)
{
   NhuUE_EUTRA_CapinterRAT_Params  *interRatPar = NULLP;
   Bool                            result = FALSE;

   /* Check if interRAT_Params IE is present or not
    * if not present return FALSE */
   if((NULLP != ueCb->ueCap) && 
         (PRSNT_NODEF == ueCb->ueCap->eUTRA_Cap.interRAT_Params.pres.pres))
   {
      interRatPar = &ueCb->ueCap->eUTRA_Cap.interRAT_Params;
   }
   else
   {
      RETVALUE(result);
   }
   switch(ratType)
   {
      case CDMA2000_1XRTT:
         if(interRatPar->cdma2000_1xRTT.pres.pres != NOTPRSNT)
         {
            if(interRatPar->cdma2000_1xRTT.rx_Config1XRTT.val ==
                  NhuIRAT_ParamsCDMA2000_1XRTTrx_Config1XRTTdualEnum)
            {
               result = TRUE;
            }
            else
            {
               result = FALSE;
            }
         }
         break;
      case CDMA2000_HRPD:
         if (interRatPar->cdma2000_HRPD.pres.pres != NOTPRSNT)
         {
            if(interRatPar->cdma2000_HRPD.rx_ConfigHRPD.val ==
                  NhuIRAT_ParamsCDMA2000_HRPDrx_ConfigHRPDdualEnum)
            {
               result = TRUE;
            }
            else
            {
               result = FALSE;
            }
         }
         break;
   }
   RETVALUE(result);
}

/**
 *  @brief This function fills Downlink Information Transfer
 *          for S1 CDMA2000 tunneling message.
 *
 *      Function: wrUmmRrcFillCdmaDlInfTfr
 *
 *          Processing steps:
 *          - obtain the DL DCCH message pointer 
 *          - set the presence of the message
 *          - fill the Downlink Information Transfer
 *
 *  @param [out] dlInfoTfrPdu : pointer to DL info transfer PDU 
 *  @param [in]  cdmaPdu      : pointer to CDMA pdu
 *  @param [in]  pdcpId       : bearer PDCP ID 
 *  @return S16
    -# Success : ROK
    -# Failure : RFAILED
 */

PUBLIC S16 wrUmmRrcFillCdmaDlInfTfr
(
NhuDatReqSdus                *dlInfoTfrPdu,
TknStrOSXL                   *cdmaPdu,
U32                           ratTyp,
U8                            pdcpId
)
{
   NhuDL_DCCH_Msg            *dlMsg = NULLP;
   NhuDLInfoTfr              *infoTfrMsg = NULLP;
   NhuDLInfoTfr_r8_IEs       *msgIe = NULLP;
   U16                       idx = 0;

   RLOG0(L_DEBUG, " Building CDMA2000 DL Info Transfer ");

   dlInfoTfrPdu->sdu.isUeCfgPres    = FALSE;
   dlInfoTfrPdu->sdu.ueMsg.isMsgDed = TRUE;
   dlInfoTfrPdu->sdu.ueMsg.msg.dedMsg.dlDcchMsg.pdcpId = pdcpId;
   dlMsg = &(dlInfoTfrPdu->sdu.ueMsg.msg.dedMsg.dlDcchMsg.m.dcchMsg);

   wrFillTknU8(&dlMsg->pres, PRSNT_NODEF);
   wrFillTknU8(&dlMsg->message.choice, DL_DCCH_MSGTYP_C1);
   wrFillTknU8(&dlMsg->message.val.c1.choice, C1_DLINFOTFR);

   /* DLInformationTransfer */
   infoTfrMsg = &(dlMsg->message.val.c1.val.dlInfoTfr);
   wrFillTknU8(&infoTfrMsg->pres, PRSNT_NODEF);
   /* TODO: Determine how to set the transaction id appropriately         */
   wrFillTknU32(&infoTfrMsg->rrc_TrnsxnId, 0);
   wrFillTknU8(&infoTfrMsg->criticalExtns.choice, CRITICALEXTNS_C1);
   wrFillTknU8(&infoTfrMsg->criticalExtns.val.c1.choice, C1_DLINFOTFR_R8);

   /* DLInformationTransfer-r8-IEs */ 
   msgIe = &(infoTfrMsg->criticalExtns.val.c1.val.dlInfoTfr_r8);
   wrFillTknU8(&msgIe->pres, PRSNT_NODEF);
   if (ratTyp == SztCdma2000RATTyponexRTTEnum)
   {
      wrFillTknU8(&msgIe->dedicatedInfoTyp.choice,
            DEDICATEDINFOTYP_DEDICATEDINFOCDMA2000_1XRTT);

      /* CDMA PDU */
      WR_GET_MEM(dlInfoTfrPdu, cdmaPdu->len, 
            &(msgIe->dedicatedInfoTyp.val.dedicatedInfoCDMA2000_1XRTT.val));
      msgIe->dedicatedInfoTyp.val.dedicatedInfoCDMA2000_1XRTT.pres = PRSNT_NODEF;
      msgIe->dedicatedInfoTyp.val.dedicatedInfoCDMA2000_1XRTT.len = cdmaPdu->len;
      /* copy each byte */
      for(idx = 0; idx < cdmaPdu->len; idx++)
      {
         msgIe->dedicatedInfoTyp.val.dedicatedInfoCDMA2000_1XRTT.val[idx] = cdmaPdu->val[idx];
      }
   }
   else
   {
      wrFillTknU8(&msgIe->dedicatedInfoTyp.choice,
            DEDICATEDINFOTYP_DEDICATEDINFOCDMA2000_HRPD);

      /* CDMA PDU */
      WR_GET_MEM(dlInfoTfrPdu, cdmaPdu->len,
            &(msgIe->dedicatedInfoTyp.val.dedicatedInfoCDMA2000_HRPD.val));
      msgIe->dedicatedInfoTyp.val.dedicatedInfoCDMA2000_HRPD.pres = PRSNT_NODEF;
      msgIe->dedicatedInfoTyp.val.dedicatedInfoCDMA2000_HRPD.len = cdmaPdu->len;
      /* copy each byte */
      for(idx = 0; idx < cdmaPdu->len; idx++)
      {
         msgIe->dedicatedInfoTyp.val.dedicatedInfoCDMA2000_HRPD.val[idx] = cdmaPdu->val[idx];
      }
   }
   /* criticalExtensionsFuture */
   wrFillTknU8(&msgIe->nonCriticalExtn.pres, NOTPRSNT);

   RETVALUE(ROK);
} /* End of wrUmmRrcFillCdmaDlInfTr */

/**
 *  @brief This function builds CDMA Downlink Information Transfer
 *
 *      Function:wrUmmBuildAndSndCdmaDlInfTfr 
 *
 *
 *  @param [in]  cdmaPdu      : pointer to CDMA pdu
 *  @param _in]  ratTyp       : Rat Type 
 *  @return S16
    -# Success : ROK
    -# Failure : RFAILED
 */

PUBLIC S16 wrUmmBuildAndSndCdmaDlInfTfr
(
 WrUmmTransCb                 *transCb,
 TknStrOSXL                   *cdmaPdu,
 U32                           ratTyp
 )
{
   WrUmmDlCdmaTransCb        *dlTrans = &(transCb->u.dlCdmaTrans);
   NhuDatReqSdus             *dlInfTfr = NULLP;

   /* Build RRC message and send it out to the UE                         */
   WR_ALLOCEVNT(&dlInfTfr, sizeof(NhuDatReqSdus));
   /* fix for ccpu00134184 */
   if(NULLP == dlInfTfr)
   {
      RLOG0(L_FATAL, "Memory allocation failed.");
      RETVALUE(RFAILED);
   }
   dlInfTfr->hdr.cellId         = transCb->ueCb->cellId;
   dlInfTfr->hdr.ueId           = transCb->ueCb->crnti;
   dlInfTfr->hdr.transId        = 
                         (transCb->transId | WR_UMM_ECSFB_DL_CDMA_INFO_TRANSID);

   if(ROK == 
         wrUmmRrcFillCdmaDlInfTfr(dlInfTfr, cdmaPdu, ratTyp,  WR_PDCP_ID_SRB2))
   {
      RLOG_ARG0(L_EVENT, DBG_CRNTI, transCb->ueCb->crnti, " [OTA] Sending "
            "DLInformationTransfer for CDMA");
      /* Call the function to dispatch the message over to RRC               */
      if (wrIfmRrcSndReqToUe(dlInfTfr) != ROK)
      {
         /* free the mem */
         WR_FREE_EVNT(dlInfTfr);
         dlTrans->state = WR_UMM_DL_CDMA_ERROR;
         RETVALUE(RFAILED);
      }
      /* Start DAT CFM timer */
      wrStartTransTmr(transCb, WR_TMR_RRC_DAT_CFM, WR_TMR_RRC_DAT_CFM_VAL);
      RLOG_ARG0(L_INFO, DBG_CRNTI,transCb->ueCb->crnti,
            "ECSFB:: RRC DLInfoTransfer message sent to UE\n");
   }
   else
   {
      /* free the mem */
      WR_FREE_EVNT(dlInfTfr);
      dlTrans->state = WR_UMM_DL_CDMA_ERROR;
   }
   RETVALUE(ROK);
}

/** @brief This function is called to Fill re-direction information
 *  of CDMA eithe 1xRTT or HRPD in RRC Coneection release message
 *
 * @details
 *
 *     Function: wrUmmFillCdmaRedirectionInfo
 *
 *        Description:
 *        Fill CDMA re-direction information in RRC Connection release message
 *
 * @param[in]  redirInfoIe:      RRC re-direction IE to be filled with re-direction info
 * @param[in]  cdmaRedirectInfo: CDMA(1xRTT or HRPD) re-direction info IE
 * @return S16
 *    -#Success : ROK
 *    -#Failure : RFAILED
 */
PUBLIC S16 wrUmmFillCdmaRedirectionInfo
(
 NhuRedirectedCarrierInfo *redirInfoIe,  
 WrUmmRedirectInfo        *cdmaRedirectInfo
)
{
   if(WR_UMM_MEAS_1XCDMA == cdmaRedirectInfo->ratTyp)
   {
      wrFillTknU8(&redirInfoIe->choice, 
                         REDIRECTEDCARRIERINFO_CDMA2000_1XRTT);
      wrFillTknU8(&redirInfoIe->val.cdma2000_1xRTT.pres, TRUE);
      wrFillTknU32(&redirInfoIe->val.cdma2000_1xRTT.bandClass, 
                         cdmaRedirectInfo->val.cdmaInfo.bandClass);
      wrFillTknU32(&redirInfoIe->val.cdma2000_1xRTT.arfcn, 
                             cdmaRedirectInfo->val.cdmaInfo.arfcn);
   } 
   else if(WR_UMM_MEAS_CDMA_HRPD == cdmaRedirectInfo->ratTyp)
   {
      wrFillTknU8(&redirInfoIe->choice, 
                         REDIRECTEDCARRIERINFO_CDMA2000_HRPD);
      wrFillTknU8(&redirInfoIe->val.cdma2000_HRPD.pres, TRUE);
      wrFillTknU32(&redirInfoIe->val.cdma2000_HRPD.bandClass, 
                           cdmaRedirectInfo->val.cdmaInfo.bandClass);
      wrFillTknU32(&redirInfoIe->val.cdma2000_HRPD.arfcn, 
                               cdmaRedirectInfo->val.cdmaInfo.arfcn);
   } 
   else
   {
      RLOG0(L_ERROR, "wrUmmFillCdmaRedirectionInfo::"
                                   "INVALID RAT TYPE Selected\n");
   }
   RETVALUE(ROK);
}
/*RRC Rel10 Upgrade - start*/
/**
*  @brief This function finds the highest priority frequency from
*         cellCb->cdmaBandLst list and the corresponding arfcn
*
*      Function: wrUmmGetBandAndArfcnCfg
*
*
*  @param [out]  bandClass : bandclass value
*  @param [out] arfcn      : arfcn of highest priority
*  @param [in]  ueCb       : ue control block 
*  @return U32
   -# Success: ROK
   -# Failure: RFAILED
*/
PRIVATE S16 wrUmmGetBandAndArfcnCfg
(
 WrUeCb               *ueCb,
 U32                  *bandClass,
 U32                  *arfcn
 )
{
	U32 band;
    U32 arfcnValue;

	if(ueCb->ueCap == NULLP)
	{
		RETVALUE(NULLP);
	}
	if(ROK != wrEmmGetHighestPrioBand(ueCb, &band))
	{
		RLOG0(L_ERROR, "wrEmmGetHighestPrioBand:Failed to get highest priority band");
		RETVALUE(RFAILED);
	}
	*bandClass = band;
	if(ROK != wrEmmGetHighestPrioFreq(band, &arfcnValue, ueCb->cellId))
	{
		RLOG0(L_ERROR, "wrEmmGetHighestPrioFreq:Failed to get highest priority freq");
		RETVALUE(RFAILED);
	}
	*arfcn = arfcnValue;

	RETVALUE(ROK);
}
/*RRC Rel10 Upgrade - end*/
/**
 *  @brief This function fills the Ecsb handover Info into the NHU structures
 *         NhuHovrFromEUTRAPrepRqst
 *  @details This function will encode into NHU NhuHovrFromEUTRAPrepRqst
 *           structure
 *
 *      Function: wrUmmRrcFillHoFrmEutraPrepReqInfo
 *
 *          Processing steps:
 *          - if flag phyCfgDedResBitMap for CQI update is set
 *             - update the ueReCfg with CQI info
 *
 *
 *  @param [in] rrcCfgDed      : pointer to PHY Config deidicated info of the RRC
 *                               reconfiguration info
 *  @param [in] rrmUeRecfgResp : pointer to Ue Control block of the UE for which UE
 *                               Recfg need to be done
 *  @return 
 */
PUBLIC S16 wrUmmRrcFillHoFrmEutraPrepReqInfo
(
   NhuDatReqSdus        *nhuDatReqSdu,
   WrUeCb               *ueCb,
   U8                   cdmaType,
   Bool                 isConCurrHoHrpd,
   TknStrOSXL           *mobilityParams,
   Bool                 sndDualRxTxRedirectInd
)
{   
   NhuDL_DCCH_Msg                       *dlMsg = NULLP;;
   NhuHovrFromEUTRAPrepRqst             *hoFrmEutraPrepRqst = NULLP;
   NhuHovrFromEUTRAPrepRqst_r8_IEs      *handoverFromEUTRAPrepRqst_r8 = NULLP;
   NhuHovrFromEUTRAPrepRqst_v890_IEs    *nonCriticalExtn_v890 = NULLP;
   /*RRC Rel10 Upgrade - start*/
   U32                       			arfcn = 0;
   U32                       			bandClass = 0;
   /*RRC Rel10 Upgrade - end*/
	/* 
	 *Setting transaction ID for above allocation transaction Id
	 * */
	dlMsg = &(nhuDatReqSdu->sdu.ueMsg.msg.dedMsg.dlDcchMsg.m.dcchMsg);
	wrFillTknU8(&dlMsg->pres, PRSNT_NODEF);
	wrFillTknU8(&dlMsg->message.choice, DL_DCCH_MSGTYP_C1);
	wrFillTknU8(&dlMsg->message.val.c1.choice, C1_HANDOVERFROMEUTRAPREPRQST);

	/* HandoverFromEUTRAPrepRqst */
	hoFrmEutraPrepRqst = &(dlMsg->message.val.c1.val.handoverFromEUTRAPrepRqst);
	wrFillTknU8(&hoFrmEutraPrepRqst->pres, PRSNT_NODEF);
	/* TODO: Determine how to set the transaction id appropriately         */
	wrFillTknU32(&hoFrmEutraPrepRqst->rrc_TrnsxnId, 0);
	wrFillTknU8(&hoFrmEutraPrepRqst->criticalExtns.choice, CRITICALEXTNS_C1);
	wrFillTknU8(&hoFrmEutraPrepRqst->criticalExtns.val.c1.choice, C1_HANDOVERFROMEUTRAPREPRQST_R8);

	handoverFromEUTRAPrepRqst_r8 = &hoFrmEutraPrepRqst->criticalExtns.val.c1.val.handoverFromEUTRAPrepRqst_r8;

	wrFillTknU8(&handoverFromEUTRAPrepRqst_r8->pres, PRSNT_NODEF);
	/* CDMA type will be received from the caller 
	 * either NhuCDMA2000_Typtype1XRTTEnum or NhuCDMA2000_TyptypeHRPDEnum*/
	wrFillTknU32(&handoverFromEUTRAPrepRqst_r8->cdma2000_Typ, cdmaType);
/*	wrFillTknStrBSXL(&(handoverFromEUTRAPrepRqst_r8->rand),
			32, (U8 *)&ueCb->eCsfbRandNum.val, &(nhuDatReqSdu->memCp));*/

	WR_FILL_BIT_STR_VAL(handoverFromEUTRAPrepRqst_r8->rand,
			32, ueCb->eCsfbRandNum.val, nhuDatReqSdu);

	/*Get the Mobility Params for a particular Cell. Fill SID, NID in MP
	 * structure and pass to encoded to get the encoded MP as StrOSXL
	 */
	/*Filling MobilityParamsCDMA2K in TknStrOSXL,pass lenght in no of bytes 
	 *In Utility function pass the encoded MP as StrOSXL(len, val)only*/
	wrFillTknStrOSXL(&(handoverFromEUTRAPrepRqst_r8->mobilityParams), 
			mobilityParams->len,mobilityParams->val, &(nhuDatReqSdu->memCp));
	nonCriticalExtn_v890 = &(handoverFromEUTRAPrepRqst_r8->nonCriticalExtn);

	/*Filling Release 9 Non-Critical Extension, which contains
	 *concurrPrepCDMA2000-HRPD-r9*/
	wrFillTknU8(&nonCriticalExtn_v890->pres, PRSNT_NODEF);
	wrFillTknU8(&nonCriticalExtn_v890->nonCriticalExtn.pres, PRSNT_NODEF);
	/*Filling concurrPrepCDMA2000_HRPD_r9 as FALSE as of now */
	wrFillTknU8(&nonCriticalExtn_v890->nonCriticalExtn.concurrPrepCDMA2000_HRPD_r9, isConCurrHoHrpd);
   /*RRC Rel10 Upgrade - start*/
	if(TRUE == sndDualRxTxRedirectInd)
	{
		if(ROK != wrUmmGetBandAndArfcnCfg(ueCb, &bandClass, &arfcn))
		{
			RLOG0(L_ERROR,"wrUmmRrcFillHoFrmEutraPrepReqInfo:Not able to send dualRxTxindicator Cfg");
			nonCriticalExtn_v890->nonCriticalExtn.nonCriticalExtn.pres.pres = NOTPRSNT;
			RETVALUE(RFAILED);
		}
		if ((NhuCDMA2000_Typtype1XRTTEnum == cdmaType)||
				(SztCdma2000RATTyponexRTTEnum == cdmaType))
		{
			wrFillTknU8(&nonCriticalExtn_v890->nonCriticalExtn.nonCriticalExtn.pres,PRSNT_NODEF);
			wrFillTknU32(&nonCriticalExtn_v890->nonCriticalExtn.nonCriticalExtn.dualRxTxRedirectIndicator_r10,\
					NhuHovrFromEUTRAPrepRqst_v1020_IEsdualRxTxRedirectIndicator_r10trueEnum);
			wrFillTknU8(&nonCriticalExtn_v890->nonCriticalExtn.nonCriticalExtn.redirectCarrierCDMA2000_1XRTT_r10.pres,\
					PRSNT_NODEF);
			wrFillTknU32(&nonCriticalExtn_v890->nonCriticalExtn.nonCriticalExtn.redirectCarrierCDMA2000_1XRTT_r10.bandClass,\
					bandClass);
			wrFillTknU32(&nonCriticalExtn_v890->nonCriticalExtn.nonCriticalExtn.redirectCarrierCDMA2000_1XRTT_r10.arfcn,\
					arfcn);
			RLOG0(L_DEBUG,"wrUmmRrcFillHoFrmEutraPrepReqInfo:dualRxTxRedirectIndicator_r10 is filled");
		}
		else
		{
			RLOG0(L_ERROR,"wrUmmRrcFillHoFrmEutraPrepReqInfo:dualRxTxRedirectIndicator_r10 is not filled");
			nonCriticalExtn_v890->nonCriticalExtn.nonCriticalExtn.pres.pres = NOTPRSNT;
			nonCriticalExtn_v890->nonCriticalExtn.nonCriticalExtn.dualRxTxRedirectIndicator_r10.pres = NOTPRSNT;
			nonCriticalExtn_v890->nonCriticalExtn.nonCriticalExtn.redirectCarrierCDMA2000_1XRTT_r10.pres.pres = NOTPRSNT;
		}
		nonCriticalExtn_v890->nonCriticalExtn.nonCriticalExtn.nonCriticalExtn.pres.pres = NOTPRSNT;
	}
	/*RRC Rel10 Upgrade - end*/
    RETVALUE(ROK);
} /* wrUmmRrcFillHoFrmEutraPrepReqInfo */


/* Function to fill MobilityFromEUTRACommand message */
PUBLIC S16 wrUmmRrcFillMobFrmEutraCmd
(
   NhuDatReqSdus            *nhuDatReqSdu,
   TknStrOSXL               *cdmaS1Pdu,
   U32                      ratTyp
)
{
   NhuDL_DCCH_Msg                         *dlMsg = NULLP;
   NhuMobilityFromEUTRACmd                *mobFrmEutraCmd = NULLP;  
   NhuMobilityFromEUTRACmd_r9_IEs         *mobEutraCmdRel9Ie = NULLP;
   NhuMobilityFromEUTRACmdcriticalExtnsc1 *critExtnC1 = NULLP;
   NhuE_CSFB_r9                           *e_CSFB_r9 = NULLP;


   nhuDatReqSdu->sdu.ueMsg.msg.dedMsg.dlDcchMsg.pdcpId = WR_PDCP_ID_SRB1;
   nhuDatReqSdu->sdu.ueMsg.isMsgDed = TRUE;
   nhuDatReqSdu->sdu.isUeCfgPres = FALSE;

   dlMsg = &(nhuDatReqSdu->sdu.ueMsg.msg.dedMsg.dlDcchMsg.m.dcchMsg);

   wrFillTknU8(&dlMsg->pres, PRSNT_NODEF);
   wrFillTknU8(&dlMsg->message.choice, DL_DCCH_MSGTYP_C1);
   wrFillTknU8(&dlMsg->message.val.c1.choice, C1_MOBILITYFROMEUTRACMD);
   
   mobFrmEutraCmd = &(dlMsg->message.val.c1.val.mobilityFromEUTRACmd);

   wrFillTknU8(&mobFrmEutraCmd->pres, PRSNT_NODEF);
   wrFillTknU32(&mobFrmEutraCmd->rrc_TrnsxnId, 0);
   wrFillTknU8(&mobFrmEutraCmd->criticalExtns.choice, CRITICALEXTNS_C1);
   
   critExtnC1 = &(mobFrmEutraCmd->criticalExtns.val.c1);
   wrFillTknU8(&critExtnC1->choice, C1_MOBILITYFROMEUTRACMD_R9);
   mobEutraCmdRel9Ie = &(mobFrmEutraCmd->criticalExtns.val.c1.val.
                         mobilityFromEUTRACmd_r9);

   wrFillTknU8(&mobEutraCmdRel9Ie->pres, PRSNT_NODEF);
   /* this can be changed for other Rat */
   wrFillTknU8(&mobEutraCmdRel9Ie->cs_FallbackIndicator, FALSE);
   wrFillTknU8(&mobEutraCmdRel9Ie->purpose.choice, PURPOSE_E_CSFB_R9);
   e_CSFB_r9 = &(mobEutraCmdRel9Ie->purpose.val.e_CSFB_r9);
   wrFillTknU8(&e_CSFB_r9->pres, PRSNT_NODEF);
   /*When HRPD is supported, below 3 IEs will be filled considering 
    * success and failure scenarios
         * 1. mobilityCDMA2000_HRPD_r9, 
    * 2. messageContCDMA2000_HRPD_r9  
    * 3. redirectCarrierCDMA2000_HRPD_r9
    */
    if(SztCdma2000RATTyponexRTTEnum == ratTyp)
    {
      
       wrFillTknStrOSXL(&(e_CSFB_r9->messageContCDMA2000_1XRTT_r9), 
      cdmaS1Pdu->len, cdmaS1Pdu->val, &(nhuDatReqSdu->memCp));
    }
   RETVALUE(ROK);
} /* wrUmmRrcFillMobFrmEutraCmd */


/**
 *  @brief This function finds whether any of the band supported by the Ue
 *  is configured at EMM or not. It calls wrEmmIs1xcCdmaBandCfg which checks
 *  if the bandclass is configured in cellCb->cdmaBandLst list 
 *
 *      Function: wrUmmIs1xCdmaBandSupported 
 *
 *
 *  @param [in]  pointer to UeCb 
 *  @return S16
    -# Success: TRUE
    -# Failure: FALSE
 */
/** @brief This function is used to Fill UTRA redirection information for RRC
 * connection release
 *
 * @details
 *
 *     Function: wrUmmFillCdmaRedirectInfo
 *
 *         Processing steps:
 *     1) check the Ue capabilities
 *     3) if Ue cap is present in ue cb select WCDMA frequency based on Ue
 *     capabilities
 *
 * @param[in]  ueCb: Ue control block
 * @return
 *    -#Success : WrUmmRedirectInfo*
 *    -#Failure : RFAILED
 */
PUBLIC  WrUmmRedirectInfo* wrUmmFillCdmaRedirectInfo
(
   WrUeCb                    *ueCb,
   wrCsfbRatType             ratPriority
)
{
   WrUmmCellCb               *cellCb;
   WrUmmRedirectInfo         *redirInfo = NULLP;
   U32                       arfcn;
   U32                       bandClass = 0;

   TRC2(wrUmmFillCdmaRedirectInfo)

      WR_UMM_GET_CELLCB(cellCb, ueCb->cellId)
      if(NULLP == cellCb)
      {
         RLOG0(L_ERROR, " Failed to get cellCB ");
         RETVALUE(NULLP);     
      }

   if(ueCb->ueCap == NULLP)
   {
      RETVALUE(NULLP);
   }

   /*Do following to fill the CDMA re-direction Information
    * 1. Get the list of supported band with highest priority from UE capability
    *     - For each suppported band check if it is configured or not 
    *     - Fetch the highest priority band from the supported bands
    * 2. If band is supported at eNodeb then 
    *     - Find the list of frequency from ANR table for the particular 
    *       supported band
    *     - Fetch the highest priority frequency form the frequency list
    * 3. Fill this frequency and corresponding supported band in redirection Info
    */
   /* TODO - Need to check whether bandClass need to check here */
   if(ROK != wrEmmGetHighestPrioBand(ueCb, &bandClass))
   {
      RLOG0(L_DEBUG, " wrEmmGetHighestPrioBand :: RETUNR NULLP");
      RETVALUE(NULLP);
   }
   /* For HRPD it is required
      else if((CDMA2000_HRPD == ratPriority) && (NhuCDMA2000_TyptypeHRPDEnum == 
      cdmaMeasObj->cdmaType))
      {
      isUeSupported = wrUmmChkCdmaHrpdBandSupported(ueCb,
      cdmaMeasObj->bandClass);
      } */

   /*Find the highest priority frequency for this band 
    * from ANR table*/
   if(ROK != wrEmmGetHighestPrioFreq(bandClass, &arfcn, ueCb->cellId))
   {
      RLOG0(L_DEBUG, "wrEmmGetHighestPrioFreq :: RETUNR NULLP");
      RETVALUE(NULLP);
   }
   WR_ALLOC(&redirInfo, sizeof(WrUmmRedirectInfo));
   if (NULLP == redirInfo)
   {
      RLOG0(L_FATAL, "Memory allocation failed. ");
      RETVALUE(NULLP);
   }
   if(CDMA2000_1XRTT == ratPriority)
   {
      redirInfo->ratTyp = WR_UMM_MEAS_1XCDMA;
   }
   else
   {
      redirInfo->ratTyp = WR_UMM_MEAS_CDMA_HRPD;
   }
   redirInfo->cellId = ueCb->cellId;
   redirInfo->val.cdmaInfo.bandClass = bandClass;
   redirInfo->val.cdmaInfo.arfcn = arfcn;
   RETVALUE(redirInfo);
}/* End of wrUmmFillCdmaRedirectInfo */

/** @brief This function is used to get the first UE supported band from measurment
 *  bandLst and the corresponding Arfcn of that measurement object.
 *
 * @details
 *
 *     Function: wrUmmGetBandArfcnFrmMeas
 *
 *         Processing steps:
 *     1) check the Ue capabilities
 *     3) if Ue cap is present in ue cb select WCDMA frequency based on Ue
 *     capabilities
 *
 * @param[in]  ueCb: Ue control block
 * @return
 *    -#Success : WrUmmRedirectInfo*
 *    -#Failure : RFAILED
 */
PUBLIC  S16 wrUmmGet1xRttNeighCell
(
   WrUeCb              *ueCb,
   WrCdmaRttPilotLst   **cdma1xRttPilotLst,
   U8                  ratPriority
)
{
   WrUmmCellCb               *cellCb;
   Bool                      isUeSupported = FALSE;
   WrUmmMeasCdmaMeasObjNode  *cdmaMeasObj = NULLP;
   CmLList                   *lnk;
   WrCdmaRttPilotLst         *cdmaPilotLst;

   TRC2(wrUmmGet1xRttNeighCell)

   WR_UMM_GET_CELLCB(cellCb, ueCb->cellId)
   if(NULLP == cellCb)
   {
      RLOG0(L_ERROR, " wrUmmGet1xRttNeighCell: Failed to get cellCB\n ");
      RETVALUE(NULLP);     
   }

   if(ueCb->ueCap == NULLP)
   {
      RETVALUE(NULLP);
   }
   if(CDMA2000_1XRTT == ratPriority)
   {
      lnk = cellCb->measCellCb->cdma1xMeasObjLst.first;
   }
   else
   {
      lnk = cellCb->measCellCb->cdmaHrpdMeasObjLst.first;
   }
   /*Do following to fill the CDMA re-direction Information
    * 1. Get the list of supported bands from UE capability
    * 2. For each suppported band check if it is configured in UMM measurment
    *  CB or not
    * 3. If band is supported at eNodeb then find the list of 
    *    frequency from ANR table for the particular supported band
    * 4. Find the first Frequency from the list
    * 5. Call NRT function with band and frequency as argument 
    *    to get the folloiwng details per Cell
    *     -- Band
    *     -- Frequency
    *     -- PnOffset
    *     -- PilotPnPhase
    *     -- PilotStrengt
    */
   while (NULLP != lnk)
   {
      cdmaMeasObj = (WrUmmMeasCdmaMeasObjNode*)lnk->node;
      if(NULLP != cdmaMeasObj)
      {
         /*This function will return TRUE if 
          * ratPriority matches with the measurment object cdmaType
          * and also the band class is present in measurement Cb
          * on the mismatch of ratPriority and cdmaType it will return
          * FALSE
          */
         if((CDMA2000_1XRTT == ratPriority) && 
               (NhuCDMA2000_Typtype1XRTTEnum == cdmaMeasObj->cdmaType))
         {
            isUeSupported = wrUmmChk1xRttCdmaBandSupported(ueCb,
                  cdmaMeasObj->bandClass);
         }
         /* For HRPD it is required */
         else if((CDMA2000_HRPD == ratPriority) && 
               (NhuCDMA2000_TyptypeHRPDEnum == cdmaMeasObj->cdmaType))
         {
            isUeSupported = wrUmmChkCdmaHrpdBandSupported(ueCb,
                                cdmaMeasObj->bandClass);
         }

         if(TRUE == isUeSupported)
         {
            /*Find other fields of CDMA 1xRTT cell from ANR NRT table
             * pass bandClass and Arfcn as input parameter to fetch 
             * neighbor cell
             */
            cdmaPilotLst = NULLP;
            WR_ALLOC(&cdmaPilotLst, sizeof(WrCdmaRttPilotLst));
            if (NULLP == cdmaPilotLst)
            {
               RLOG0(L_FATAL, "Memory allocation failed.");
               RETVALUE(RFAILED);
            }

            cdmaPilotLst->bandClass = cdmaMeasObj->bandClass;
            cdmaPilotLst->arfcn     = cdmaMeasObj->arfcn;
            /* For HRPD call other function */
            if (ROK != wrEmmAnrGetCdma1xRttCell(ueCb->cellId, cdmaPilotLst))
            {
               WR_FREE(cdmaPilotLst, sizeof(WrCdmaRttPilotLst));
               RETVALUE(RFAILED);
            }

            *cdma1xRttPilotLst = cdmaPilotLst;
            RETVALUE(ROK);
         }
      }
      lnk = lnk->next;
   }
   RETVALUE(RFAILED);
}/* End of wrUmmGet1xRttNeighCell */

/** @brief This function is used to Fill Pilot List UE from NRT table   
* and also do CDMA encoding on the pilot message
* @details
*
*     Function: wrFillPilotLstIe 
*
*         Processing steps:
*     1) Access NRT Table
*     2) Fill PilotList as per CDMA A.S0008-C v4.0 Spec
*     3) Fill PN Phase and pilot Strength received from measurmntes
*     if No measurments then fill default values
*
* @param[in]  ulTnlMsgParams
* @return
*    -#Success :cdma2000OneXPilot
*    -#Failure : RFAILED
*/

PUBLIC S16 wrFillPilotLstIe
(
   U8                            cellId,
   SztCdma2000OneXSRVCCInfo      *ie,
   WrUmmS1UlTnlMsgParams         *ulTnlMsgParams,
   S1apPdu                       *ulCdmaPdu
)
{
   U8                         *pltLstEncBuf = NULLP;
   U16                        pltLstLen = 0;
   U8                         idx, lenCnt = 0;
   WrCdma1xRttPnOffset        cdmaPnOffset;
   WrCdma1xRttPilotInfo       cdmaPltLstInfo;

   SztCdma2000OneXPilot       *cdma2000OneXPilot = &ie->cdma2000OneXPilot;
   /* Allocate pltLstEncBuf of Max length for CDMA PilotList */
   cmGetMem(ulCdmaPdu, (ulTnlMsgParams->numOfPilotLst * 
            MAX_CDMA2OOO_PILOT_LIST_MAX_LEN + 3), (Ptr*)&(pltLstEncBuf));
   if (NULLP == pltLstEncBuf)
   {
      RLOG0(L_FATAL, "Memory allocation failed.");
      RETVALUE(RFAILED);
   }

   /* NOTE: Considering bit positions from 1 to 8. 1-LSB, 8-MSB */
   /* Fill the Pilot List: A21 Element Identifier=[03H] Length = 8 bits */
   pltLstEncBuf[pltLstLen++] = 0x03;

   lenCnt = pltLstLen++; /* incremented to store pltLstLen@END */
   /*pltLstEncBuf[pltLstLen++] = &pltLstLen; */


   /* Fill Number of Pilots: This field indicates the number of pilots
    * represented by this IE including  the  reference  pilot.  The  Channel
    * Record,  Cell Identifier,  Pilot  PN  Phase  and  Pilot  Strength  are
    * indicated  for each pilot. */
   pltLstEncBuf[pltLstLen++] = ulTnlMsgParams->numOfPilotLst; /*NRT */

   for(idx=0; ((idx < ulTnlMsgParams->numOfPilotLst) &&
              (idx < WR_UMM_MAX_PILOT_LIST)); idx++)
   {
      /* Call NRT table to get following details for each PnOffset
       * sectorID
       * systemType
       * pilotOneWay
       * channelNum
       */
      cdmaPnOffset.bandClass = ulTnlMsgParams->PilotList[idx]->bandClass;
      cdmaPnOffset.arfcn     = ulTnlMsgParams->PilotList[idx]->arfcn;
      cdmaPnOffset.pnOffset  = ulTnlMsgParams->PilotList[idx]->pnOffset;
      wrEmmAnrGetCdma1xRttCellByPnOffset(cellId, &cdmaPnOffset, 
            &cdmaPltLstInfo);
      /* Filling of Channel Record Length:
       * This field contains the numbers of octets in the Channel Record 
         field as a binary number. */
      pltLstEncBuf[pltLstLen++] = WR_UMM_CHNL_RCD_LEN; /*NRT */
      /* Filling System Type, 8 bits */
      pltLstEncBuf[pltLstLen++] = (U8)(cdmaPltLstInfo.systemType & 0x000000ff); 
      /* Filling Band Class , 5 bits*/
      pltLstEncBuf[pltLstLen++] = (U8)(((cdmaPltLstInfo.bandClass & 0x1f) << 3) | 
            ((cdmaPltLstInfo.channelNum & 0x0700) >> 8));
      /* Filling Channel Number, 11 bits*/
      pltLstEncBuf[pltLstLen++] = (U8)(cdmaPltLstInfo.channelNum & 0x000000ff);

      /*Fill 1xCell Identifier IF (Cell ID Info = 001, 010, 011, 111) */
      pltLstEncBuf[pltLstLen++] = WR_UMM_CELL_ID_INFO & 0x07;
      if ( (pltLstEncBuf[pltLstLen - 1] ==  0x01) || 
            (pltLstEncBuf[pltLstLen - 1] == 0x02) ||
            (pltLstEncBuf[pltLstLen - 1] == 0x03) ||
            (pltLstEncBuf[pltLstLen - 1] == 0x07) )
      {
         cmMemcpy((U8 *)&pltLstEncBuf[pltLstLen],
                      (U8 *)(cdmaPltLstInfo.sectorId->val),
                  cdmaPltLstInfo.sectorId->len);
            pltLstLen += cdmaPltLstInfo.sectorId->len;
      }
      else
      {
         /*IF (Cell ID Info = 100, 101, 110), HRPD Sector Identifier */
      }
      /* Reference Pilot 1 bit(i.e 16th bit) Not Considered as its for HRPD*/ 
      /* Fill 15 bits for PilotPnPhase: */
      pltLstEncBuf[pltLstLen++] = 
         (U8)(ulTnlMsgParams->PilotList[idx]->pilotPnPhase & 0x7F00) >> 8;
      pltLstEncBuf[pltLstLen++] = 
         (U8)(ulTnlMsgParams->PilotList[idx]->pilotPnPhase & 0x00FF);

      pltLstEncBuf[pltLstLen++] = (U8)((0x40) | 
            (ulTnlMsgParams->PilotList[idx]->pilotStrength & 0x3F));
      pltLstEncBuf[pltLstLen++] = (U8)((cdmaPltLstInfo.pilotOneWay
               & 0xFF00) >> 8);
      pltLstEncBuf[pltLstLen++] = (U8)(cdmaPltLstInfo.pilotOneWay
               & 0x00FF);
   }/* End of PilotList Entry */
   /* Fill length field: This field contains the number of octets in this IE
    * following this field as a binary number */
   pltLstEncBuf[lenCnt] = pltLstLen - 2;
   
   cdma2000OneXPilot->pres = TRUE; 
   cdma2000OneXPilot->len = pltLstLen; 
   cdma2000OneXPilot->val = pltLstEncBuf;

   RETVALUE(ROK);
}/*end of wrFillPilotLstIe */

/** @brief This function is used to Fill SRVCC Info   
* and also fill PilotList with CDMA encoding on the 
* pilot message
* @details
*
*     Function: wrFillSrvccInfoIe 
*
*         Processing steps:
*     1) Fill the MEID 
*     2) Fill CDMA2000 1xRTT Mobile Subscription
*     3) Fill PilotList as per CDMA A.S0008-C v4.0 Spec
*
* @return
*    -#Success : ROK
*    -#Failure : RFAILED
*/

PUBLIC S16 wrFillSrvccInfoIe
(
WrUeCb                       *ueCb,
WrUmmS1UlTnlMsgParams        *ulTnlMsgParams,
SztCdma2000OneXSRVCCInfo     *ie,
S1apPdu                      *ulCdmaPdu
)
{
   WrUeCapInfo                *ueCap = ueCb->ueCap;
   

   wrFillTknU8(&(ie->pres),PRSNT_NODEF);
   /* Filling MEID in SRVCC Info */
  if(ulTnlMsgParams->meId != NULLP)
  {
   wrFillTknStrOSXL(&ie->cdma2000OneXMEID, ((ulTnlMsgParams->meId->len + 7)/8), 
                        ulTnlMsgParams->meId->val, &(ulCdmaPdu->memCp));
  }
   /*Filling CDMA2000 1xRTT Mobile Subscription Information which is 
    * stored in UeCap structure. Control should not reach here if UE doesn't
    * have CDMA octet string. Validation of CDMA octet string should be done
    * in the caller of this function 
    */
   wrFillTknStrOSXL(&ie->cdma2000OneXMSI, ueCap->ueCdmaOctStr.len, 
                        ueCap->ueCdmaOctStr.val, &(ulCdmaPdu->memCp));
   /* Filling PilotList IE. This function will pick all Pilots from 
    * transCb->cdmaRttPilotLst one by one and fill as octet string
    */
   wrFillPilotLstIe(ueCb->cellId, ie, ulTnlMsgParams, ulCdmaPdu);
   RETVALUE(ROK);
}/* End of wrFillSrvccInfoIe */
/**
 *  @brief This function builds the UL CDMA Tunneling pdu
 *
 *      Function: wrUmmS1apBldUlCdmaTunnlPdu
 *
 *          Processing steps:
 *          - Alloc memory to S1AP UL CDMA pdu message
 *          - fill the mandatory IE's
 *          - update the pointer to S1AP UL CDMA pdu message
 *            into out pointer
 *
 *  @param [in]  ueCb     : pointer to UE CB
 *  @param [in]  nasPdu   : encoded nas pdu
 *  @param [out] pdu      : double pointer to return the UL NAS PDU
 *  @return S16
    -# Success : ROK
    -# Failure : RFAILED 
 */
PUBLIC S16 wrUmmS1apBldUlCdmaPdu
(
WrUeCb                       *ueCb,
WrUmmS1UlTnlMsgParams        *ulTnlMsgParams,
S1apPdu                      **pdu
)
{
   WrS1ConCb                        *s1apCon = ueCb->s1ConCb;
   S1apPdu                          *ulCdmaPdu = NULLP;
   SztInitiatingMsg                 *initMsg;
   U32                              ieIdx = 0;
   SztUlnkS1cdma2000tunneling       *ulS1cdma2000tunn = NULLP;
   SztProtIE_Field_UlnkS1cdma2000tunnelingIEs *ie = NULLP;
   U8                               numComp = 0;

   /* Initialize memory control point */
   if (cmAllocEvnt(sizeof(S1apPdu), SZ_MEM_SDU_SIZE, &wrCb.mem,
                                             (Ptr *)&ulCdmaPdu) != ROK)
   {
      RLOG0(L_FATAL, "Memory allocation failed.");
      RETVALUE(RFAILED);
   }

   wrFillTknU8(&(ulCdmaPdu->pdu.choice), S1AP_PDU_INITIATINGMSG);
   initMsg = &ulCdmaPdu->pdu.val.initiatingMsg;
   wrFillTknU8(&(initMsg->pres), PRSNT_NODEF);
   wrFillTknU32(&(initMsg->criticality), SztCriticalityignoreEnum);
   wrFillTknU32(&(initMsg->procedureCode), Sztid_UlnkS1cdma2000tunneling);
   ulS1cdma2000tunn = &(initMsg->value.u.sztUlnkS1cdma2000tunneling);
   wrFillTknU8(&(ulS1cdma2000tunn->pres), PRSNT_NODEF);
   /*Memebers of UlCdma tunneling message 
            * SztMME_UE_S1AP_ID sztMME_UE_S1AP_ID;
            * SztENB_UE_S1AP_ID sztENB_UE_S1AP_ID;
            * SztCdma2000RATTyp sztCdma2000RATTyp;
            * SztCdma2000SectorID sztCdma2000SectorID;
            * SztCdma2000HOReqdInd sztCdma2000HOReqdInd;
            * SztCdma2000OneXSRVCCInfo sztCdma2000OneXSRVCCInfo;
            * SztCdma2000OneXRAND sztCdma2000OneXRAND;
            * SztCdma2000PDU sztCdma2000PDU;
            * SztEUTRANRoundTripDelayEstimationInfo 
            *       sztEUTRANRoundTripDelayEstimationInfo;
            */
   if(TRUE == ulTnlMsgParams->hoReqdInd)
   {
     /* For eCSFB procedure S1 UL tunneling message two more IEs
      * are encoded, 
      * 1. sztCdma2000HOReqdInd
      * 2. sztCdma2000OneXSRVCCInfo */
     numComp = 8;
   }
   else
   {
     numComp = 6;
   }
   /* Allocate memory for 5 IEs */
   if ((cmGetMem(ulCdmaPdu, 
                 numComp * sizeof(SztProtIE_Field_UlnkS1cdma2000tunnelingIEs),
                                   (Ptr*)&ulS1cdma2000tunn->protocolIEs.member)) !=ROK)
   {
      RLOG0(L_FATAL, "Memory allocation failed.");
      RETVALUE(RFAILED);
   }

   /* IE1 - Filling MME ID */
   ie = &ulS1cdma2000tunn->protocolIEs.member[ieIdx];
   wrFillTknU8(&(ie->pres), PRSNT_NODEF);
   wrFillTknU32(&(ie->id), Sztid_MME_UE_S1AP_ID);
   wrFillTknU32(&(ie->criticality), SztCriticalityrejectEnum);
   wrFillTknU32(&(ie->value.u.sztMME_UE_S1AP_ID), s1apCon->mme_ue_s1ap_id);
   ieIdx++;

   /* IE2 - Filling enb s1ap id */
   ie = &ulS1cdma2000tunn->protocolIEs.member[ieIdx];
   wrFillTknU8(&(ie->pres),PRSNT_NODEF);     
   wrFillTknU32(&(ie->id),Sztid_eNB_UE_S1AP_ID);
   wrFillTknU32(&(ie->criticality),SztCriticalityrejectEnum);
   wrFillTknU32(&(ie->value.u.sztENB_UE_S1AP_ID), s1apCon->enb_ue_s1ap_id);
   ieIdx++;

   /* IE3 - Filling RAT type */
   ie = &ulS1cdma2000tunn->protocolIEs.member[ieIdx];
   wrFillTknU8(&(ie->pres),PRSNT_NODEF);     
   wrFillTknU32(&(ie->id),Sztid_cdma2000RATTyp);
   wrFillTknU32(&(ie->criticality),SztCriticalityrejectEnum);
   wrFillTknU32(&(ie->value.u.sztCdma2000RATTyp), ulTnlMsgParams->cdmaType);
   ieIdx++;

   /* IE4 - Filling  Sztid_cdma2000SectorID*/
   ie = &ulS1cdma2000tunn->protocolIEs.member[ieIdx];
   wrFillTknU8(&(ie->pres),PRSNT_NODEF);
   wrFillTknU32(&(ie->id),Sztid_cdma2000SectorID);
   wrFillTknU32(&(ie->criticality),SztCriticalityrejectEnum);
   
   /* get eNodeb level sectorID/Ref Cell ID from CellCb->csfbParams 
    * For eCSFB S1 Ul tunneling message fill the eNodeb level RCID only
    */
   wrFillEcsfbMainSecId(&(ie->value.u.sztCdma2000SectorID), 
                 ulTnlMsgParams->mainSecId->len, 
                 ulTnlMsgParams->mainSecId->val,&(ulCdmaPdu->memCp)); 
   ieIdx++;
  
   /* IE5 - Filling  sztCdma2000HOReqdInd*/
   if(TRUE == ulTnlMsgParams->hoReqdInd)
   {
      /* Filling HoRequiredIndication */
   ie = &ulS1cdma2000tunn->protocolIEs.member[ieIdx];
      wrFillTknU8(&(ie->pres),PRSNT_NODEF);
      wrFillTknU32(&(ie->id),Sztid_cdma2000HOReqdInd);
      wrFillTknU32(&(ie->criticality),SztCriticalityignoreEnum);
      wrFillTknU32(&(ie->value.u.sztCdma2000HOReqdInd),
                                  SztCdma2000HOReqdIndtrueEnum);
   ieIdx++;
      
      /*Filling SRVCC Info IE */
      ie = &ulS1cdma2000tunn->protocolIEs.member[ieIdx];
      wrFillTknU8(&(ie->pres),PRSNT_NODEF);
      wrFillTknU32(&(ie->id), Sztid_cdma2000OneXSRVCCInfo);
      wrFillTknU32(&(ie->criticality),SztCriticalityrejectEnum);
      wrFillSrvccInfoIe(ueCb, ulTnlMsgParams, &ie->value.u.\
            sztCdma2000OneXSRVCCInfo, ulCdmaPdu);
      ieIdx++;
   }

   /* IE6 - Filling sztCdma2000OneXRAND */
   wrUmmGetRand32(ueCb);
   ie = &ulS1cdma2000tunn->protocolIEs.member[ieIdx];
   wrFillTknU8(&(ie->pres),PRSNT_NODEF);
   wrFillTknU32(&(ie->id),Sztid_cdma2000OneXRAND);
   wrFillTknU32(&(ie->criticality),SztCriticalityrejectEnum);
   U8 val[7] = {0x06,0x05,0x01,0x00,0x00,0x00,0x00};
   U8 idx;
   U8 idx1 = 3;
   U8 *tmp =  (U8 *)&ueCb->eCsfbRandNum.val;
   for (idx = 0;idx <= 3;idx++) 
   {
       val[idx1++] = tmp[3 -idx];
   }
   wrFillTknStrOSXL(&(ie->value.u.sztCdma2000OneXRAND), 7, 
                                  val, &(ulCdmaPdu->memCp));
   ieIdx++;

   /* IE7 - Filling CDMA Pdu received in ULInfoTransfer */
   ie = &ulS1cdma2000tunn->protocolIEs.member[ieIdx];
   wrFillTknU8(&(ie->pres),PRSNT_NODEF);
   wrFillTknU32(&(ie->id),Sztid_cdma2000PDU);
   wrFillTknU32(&(ie->criticality),SztCriticalityrejectEnum);
   wrFillTknStrOSXL(&(ie->value.u.sztCdma2000PDU), ulTnlMsgParams->ulCdmaInfo->len,
            ulTnlMsgParams->ulCdmaInfo->val, &(ulCdmaPdu->memCp));
   ieIdx++;
   /* TODO Fill EUTRAN Round Trip Delay Estimation Info  */

   /* Fill the number of IEs in the UL NAS message                        */
   wrFillTknU16(&(ulS1cdma2000tunn->protocolIEs.noComp), ieIdx);

   *pdu = ulCdmaPdu;

   RETVALUE(ROK);
} /* wrUmmS1apBldUlCdmaPdu */

/**
 *  @brief This function generate the random number and store in UeCb
 *
 *      Function: wrUmmGetRand32 
 *
 *
 *  @param [in]  UeCb         : pointer to UeCb
 *  @return S16
    -# Success : ROK
    -# Failure : RFAILED
 */
PUBLIC Void wrUmmGetRand32
(
   WrUeCb        *ueCb
)
{
   U32    randNum;

   if(ueCb->eCsfbRandNum.pres != TRUE)
   {
      randNum = random();
      ueCb->eCsfbRandNum.val = randNum;
      ueCb->eCsfbRandNum.pres = TRUE;
   }

}/* wrUmmGetRand32 */

/**
 *  @brief This function fills the structure TknStrOSXL
 *
 *      Function: wrFillEcsfbMainSecId
 *
 *          Processing steps:
 *          - allocate memory to val in TknStrOSXL
 *          - update token present field as TRUE
 *          - update the memory with the value  received
 *
 *
 *  @param[out] ptr  : TknStrOSXL stucture pointer to be updated
 *  @param[in]  len  : length for which memory to allocated
 *  @param[in]  val  : value getting updated
 *  @param[in]  mem  : pointer to memCp
 *  @return S16
    -# Success : ROK
    -# Failure : RFAILED
 *          RFAILED
 */
PUBLIC S16 wrFillEcsfbMainSecId
(
TknStrOSXL                   *ptr,
U16                          len,
Data                         *val,
CmMemListCp                  *mem
)
{
   U16                       idx;

   /* make the element as present */
   ptr->pres = TRUE;
      /* 2 bytes extra for message type and length field
       * In main sectorID 2 message type and length will be added
       * but in PilotList 2 bytes are not added
       */
   ptr->len =  len + 2;


   if (cmGetMem(mem, len * sizeof(U8), (Ptr *)&ptr->val) != ROK)
   {
      RLOG0(L_FATAL, "Memory allocation failed.");
      RETVALUE(RFAILED);
   }

   ptr->val[0] = 0x0d; /* Filling message type for Ref Cell Id */ 
   ptr->val[1] = 0x05; /* number of bytes/octet required */
   for(idx = 2; idx < ptr->len; idx++)
   {
      ptr->val[idx] = val[idx - 2];
   }

   RETVALUE(ROK);
} /* wrFillEcsfbMainSecId */

/* Fix for ccpu00137043 */

/** @brief   
This function will be called when any message receives from UE 
 *     to enodeb which is belongs to CSFB.   
 * @details
 *
 *     Function: wrUmmChkOneBandAndFreq
 *
 *         Processing steps:
 *            - Get count of meas objects for bands supported by UE 
 *                - If num of meas Obj configured for bands supported by 
 *                  UE >1, then eNB can choose UE for redirection with UE meas.
 *                - else eNB shall choose the UE for redirection without UE meas
 *           
 * @param [in] wrUeCb 
 * @return S16
    -# Success : ROK
    -# Failure : RFAILED 
 */ 
PUBLIC S16 wrUmmChkOneBandAndFreq
(
 WrUeCb           *ueCb
)
{
   WrUmmCellCb               *cellCb;
   CmLList                   *lnk;
   WrUmmMeasCdmaMeasObjNode  *cdmaMeasObj = NULLP;
   Bool                      isUeSupported = FALSE;
   U8                        cnt = 0;

   WR_UMM_GET_CELLCB(cellCb, ueCb->cellId)
      if(NULLP == cellCb) 
      {
         RLOG_ARG0(L_ERROR, DBG_CELLID,(U32)ueCb->cellId,
               "Error in getting CELL CB");
         RETVALUE(RFAILED);
      }

   /* Here, we need to check whether more than one freq is configured
    * for Band which is supported by UE or not
    * If num of meas objects configured for Bands supported by UE is greater
    * than 1,return ROK. So eNB can choose UE for redirection with UE meas.
    * Else return RFAILED.
    *
    */
   lnk = cellCb->measCellCb->cdma1xMeasObjLst.first;

   while (NULLP != lnk)
   {
      cdmaMeasObj = (WrUmmMeasCdmaMeasObjNode*)lnk->node;
      if(NULLP != cdmaMeasObj)
      {
         if(NhuCDMA2000_Typtype1XRTTEnum == cdmaMeasObj->cdmaType)
         {
            isUeSupported = wrUmmChk1xRttCdmaBandSupported(ueCb,
                  cdmaMeasObj->bandClass);
            if(isUeSupported == TRUE)
            {
               /* If band is supported by UE, increment the count here */
               cnt++;
               if(cnt > 1)
               {
                  /* cnt is greater than 1 only in two cases - 
                   * 1. Either more than 1 band supported by UE is configured.
                   * 2. Or more than 1 frequency is configured for band supported by UE
                   * Hence, eNb can choose this UE for redirection with meas */

                  RLOG0(L_DEBUG, " More than one frequency Or one band supported by UE\
                        is configured \n");
                  RETVALUE(ROK);
               }
            }
         }
      }
      lnk = lnk->next;
   }

   RLOG0(L_DEBUG, "Only one frequency is configured for band supported by UE");
   RETVALUE (RFAILED);
}
/** @brief This function is responsible for finding out 
 *  if UE supports Dual rx-tx Config or not
 *
 * @details
 *
 *     Function: wrUmmUtilGetUeDualRxTx
 *
 * @return BOOL 
 *       -# TRUE
 *       -# FALSE
 *       */
PUBLIC Bool wrUmmUtilGetUeDualRxTx
(
 WrUeCb                     *ueCb,
 U32                        ratPriority
)
{
	NhuIRAT_ParamsCDMA2000_1XRTT_v1020 *cdma2000_v1020 = NULLP;
	NhuIRAT_ParamsCDMA2000_1XRTT 	   *cdma2000_1xRTT = NULLP;
	Bool                                result = FALSE;

	/* Check if interRAT_Params IE is present or not
	 * if not present return FALSE */

	if(NULLP != ueCb->ueCap)
	{
		if((NOTPRSNT != ueCb->ueCap->eUTRA_Cap.pres.pres)&& \
				(CDMA2000_1XRTT == ratPriority))
		{
			if((NOTPRSNT != ueCb->ueCap->eUTRA_Cap.accessStratumRls.pres)&& \
					(NhuAccessStratumRlsrel9Enum < ueCb->ueCap->eUTRA_Cap.accessStratumRls.val))
			{
				if((NOTPRSNT != ueCb->ueCap->eUTRA_Cap.nonCriticalExtn.pres.pres)&& \
						(NOTPRSNT != ueCb->ueCap->eUTRA_Cap.nonCriticalExtn.\
						 nonCriticalExtn.pres.pres)&& \
						(NOTPRSNT != ueCb->ueCap->eUTRA_Cap.nonCriticalExtn.nonCriticalExtn.\
						 nonCriticalExtn.pres.pres)&& \
						(NOTPRSNT != ueCb->ueCap->eUTRA_Cap.nonCriticalExtn.nonCriticalExtn.nonCriticalExtn.\
						 interRAT_ParamsCDMA2000_v1020.pres.pres))
				{
					cdma2000_v1020 = &ueCb->ueCap->eUTRA_Cap.nonCriticalExtn.nonCriticalExtn.nonCriticalExtn.\
									 interRAT_ParamsCDMA2000_v1020;

					if(NOTPRSNT != cdma2000_v1020->e_CSFB_dual_1XRTT_r10.pres)
					{
						if((NOTPRSNT != ueCb->ueCap->eUTRA_Cap.interRAT_Params.pres.pres)&& \
								(NOTPRSNT != ueCb->ueCap->eUTRA_Cap.interRAT_Params.cdma2000_1xRTT.pres.pres))
						{
							cdma2000_1xRTT = &ueCb->ueCap->eUTRA_Cap.interRAT_Params.cdma2000_1xRTT;			

							if((cdma2000_1xRTT->tx_Config1XRTT.val == \
										NhuIRAT_ParamsCDMA2000_1XRTTtx_Config1XRTTdualEnum)&& \
									(cdma2000_1xRTT->rx_Config1XRTT.val == \
									 NhuIRAT_ParamsCDMA2000_1XRTTrx_Config1XRTTdualEnum)&& \
									(cdma2000_v1020->e_CSFB_dual_1XRTT_r10.val == \
									 NhuIRAT_ParamsCDMA2000_1XRTT_v1020e_CSFB_dual_1XRTT_r10supportedEnum))
							{
								RLOG0(L_INFO, "wrUmmUtilGetUeDualRxTx:DualRxTx config is enabled at UE");
								result = TRUE;
							}
						}
					}
				}
			}
		}
	}
	RETVALUE(result);
}

/** @brief This function is responsible for finding out 
 *  if UE supports Dual rx-tx Config and 1xrtt registration and dual Rx 
 *
 * @details
 *
 *     Function: wrUmmUtilGetUeEcsfbAndDualRxTxSuppCfgs
 *
 * @return BOOL 
 *       -# TRUE
 *       -# FALSE
 *       */
PUBLIC Bool wrUmmUtilGetUeEcsfbAndDualRxTxSuppCfgs
(
 WrUeCb                     *ueCb
 )
{
	WrRatPriorityList        ratPriority;
	U8                       cnt;

	wrEmmGetRatPriority(&ueCb->plmnId, &ratPriority, ueCb->cellId);

	for(cnt = 0; cnt < ratPriority.numberOfRats; cnt++)
	{
		if ((CDMA2000_1XRTT == ratPriority.rat[cnt]) && \
				(TRUE == wrUmmUtilGetUeDualRxTx(ueCb, ratPriority.rat[cnt]))&& \
				(NhuAccessStratumRlsrel9Enum < ueCb->ueCap->eUTRA_Cap.accessStratumRls.val)&& \
				(TRUE == wrUmmUtilGetUeDualRx(ueCb, ratPriority.rat[cnt]))&& \
				(TRUE == wrUmmUtilsIsEcsfbSupported(ueCb)))
		{
			RETVALUE(TRUE);
		}
	}
	RLOG0(L_INFO,"wrUmmUtilGetUeEcsfbAndDualRxTxSuppCfgs:Ue doesnt support dualRxTx"); 
	RETVALUE(FALSE);
}
/********************************************************************30**

           End of file:     fc_umm_ecsfb_utils.c@@/main/TeNB_Main_BR/tenb_brcm_fdd_3.0.1_to_tdd_GA_4.0_merge/1 - Fri Sep 12 10:54:46 2014

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
/main/1        ---      Sriky         1. initial release TotaleNodeB 1.1
*********************************************************************91*/
