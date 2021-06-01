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

     Sid:      fc_umm_ecsfb.c@@/main/TeNB_Main_BR/tenb_brcm_fdd_3.0.1_to_tdd_GA_4.0_merge/1 - Fri Sep 12 10:54:46 2014

     Prg:      Sriky 

*********************************************************************21*/

/* header include files (.h) */
static const char* RLOG_MODULE_NAME="UMM";
static int RLOG_MODULE_ID=32;
static int RLOG_FILE_ID=119;

#include "wr.h"
#include "wr_utils.h"
#include "wr_umm.h"
#include "wr_emm_ecsfb_utils.h"
#include "wr_umm_rrc_msg.h"
#include "wr_umm_trans.h"
#include "wr_ifm_rrc.h"
#include "nhu_asn.x"
#include "wr_lwr.h"
#include "wr_umm_meas_fun.h"
#include "wr_umm_ecsfb.h"
#include "wr_umm_ecsfb_utils.h"
#include "wr_kpi.h"
#include "wr_ifm_dam.h"/*ccpu00138576*/

PRIVATE S16 wrUmmFillAndSndHoFrmEutraPrepReq(WrUmmTransCb  *transCb, Bool sndDualRxTxRedirectInd);
PUBLIC S16 wrUmmBldAndSndMobFrmEutraCmd
(
 WrUmmTransCb                 *transCb,
 TknStrOSXL                   *cdmaS1Pdu,
 U32            ratType
);

PRIVATE S16 wrUmmSndHoFrmEutraPrepReq
(
 WrUeCb       *ueCb, 
 WrUmmTransCb *transCb,
 Bool         sndDualRxTxRedirectInd
);
/*RRC Rel10 Upgrade - start*/
PRIVATE S16 wrUmmEcsfbSndDlNas
(
 WrUmmTransCb        *transCb,
 TknStrOSXL          *cdmaS1Pdu,
 U32                 cdmaType
);
/*RRC Rel10 Upgrade - end*/

/** @brief This function will handle ECSFB transaction release.
 *
 * @details
 *
 *     Function: wrUmmBldAndSndMobFrmEutraCmd
 *
 *         Processing steps:
 *         - Build and send MobilityFromEUTRACommand
 *
 * @param[in]  transCb  : transaction inforamation
 * @param[in]  cdmaS1Pdu: 
 * @return S16
 *    -#Success : ROK
 */
PUBLIC S16 wrUmmBldAndSndMobFrmEutraCmd
(
 WrUmmTransCb            *transCb,
 TknStrOSXL              *cdmaS1Pdu,
 U32                     ratType
 )
{
   NhuDatReqSdus             *mobFrmEutraCmd = NULLP;

   /* Build RRC message and send it out to the UE                         */
   WR_ALLOCEVNT(&mobFrmEutraCmd, sizeof(NhuDatReqSdus));
   if(NULLP == mobFrmEutraCmd)
   {
      RLOG0(L_FATAL, "Memory allocation failed.");
      RETVALUE(RFAILED);
   }
   mobFrmEutraCmd->hdr.cellId         = transCb->ueCb->cellId;
   mobFrmEutraCmd->hdr.ueId           = transCb->ueCb->crnti;
   mobFrmEutraCmd->hdr.transId        =
             transCb->transId | WR_UMM_ECSFB_MOB_EUTRA_CMD_TRANSID;

   /* We will add hrpd Pdu also in function argument list and 
   * Hostatus for HRPD will be also added
   */
   if(ROK == wrUmmRrcFillMobFrmEutraCmd(mobFrmEutraCmd, cdmaS1Pdu, ratType))
   {
      RLOG_ARG0(L_EVENT, DBG_CRNTI, transCb->ueCb->crnti,"[OTA] Sending "
             "MobilityFromEUTRACommand for E_CSFB ");
      /* Call the function to dispatch the message over to RRC               */
      if (wrIfmRrcSndReqToUe(mobFrmEutraCmd) != ROK)
      {
         WR_FREE_EVNT(mobFrmEutraCmd);
         RETVALUE(RFAILED);
      }
      if(ratType == SztCdma2000RATTyphRPDEnum)
      {
         transCb->ueCb->csfbInfo.ratType = CDMA2000_HRPD;
      }
      else
      {
         transCb->ueCb->csfbInfo.ratType = CDMA2000_1XRTT;
      }
      transCb->ueCb->csfbInfo.redirType = CDMA2000_CSFB_WITHOUT_PS_HO;

      /* Started the new timer after sending the MobilityFromEUTRACommand 
       * message towards UE */
      wrStartTransTmr(transCb, WR_TMR_ECSFB_HO_EXEC_TMR,
                              WR_TMR_ECSFB_HO_EXEC_TMR_VAL);

      RLOG_ARG0(L_DEBUG, DBG_CRNTI,transCb->ueCb->crnti,
            "ECSFB:: RRC message MobilityFromEUTRACommand sent to UE\n");
   }
   else
   {
      /* free the mem */
      WR_FREE_EVNT(mobFrmEutraCmd);
      RETVALUE(RFAILED);
   }
   RETVALUE(ROK);
}
/** @brief This function will handle ECSFB transaction release.
 *
 * @details
 *
 *     Function: wrUmmPrcDatCfmTmrExp
 *
 *         Processing steps:
 *         - Stop the DatCfm timer
 *         - Check the datCfm state and transCb state and free memory if any
 *
 * @param[in]  transCb : transaction inforamation
			   sndDualRxTxRedirectInd:DualRxTxCfg support
 * @return S16
 *    -#Success : ROK
 */
PRIVATE S16 wrUmmPrcDatCfmTmrExp
(
   WrUmmTransCb           *transCb
)
{
   WrUmmEcsfbTransCb  *ecsfbTransCb = &transCb->u.ecsfbTransCb;
	  
    switch(ecsfbTransCb->ecsfbState)
	{ 
      case WR_UMM_DAT_CFM_WAIT_DL_CDMA_INFO:
         {
            /*Check if number of re-transmission reached WR_UMM_ECSFB_MAX_NUM_RETX
             * if reached send failure else build and send HoFromEutraPrepReq to UE
             */ 
            if(WR_UMM_ECSFB_MAX_NUM_RETX <= ecsfbTransCb->numOfDlTxCnt)
            {
               RETVALUE(RFAILED);
            }
            if(RFAILED != wrUmmBuildAndSndCdmaDlInfTfr(transCb, 
                     ecsfbTransCb->cdmaS1Pdu, ecsfbTransCb->cdmaType))
            {
               ecsfbTransCb->ecsfbState = WR_UMM_DAT_CFM_WAIT_DL_CDMA_INFO;
               ecsfbTransCb->numOfDlTxCnt++;
               RETVALUE(ROK);
            }
            else
            {
               RETVALUE(RFAILED);
            }
         }
         break;
      default:
         RLOG_ARG0(L_ERROR, DBG_CRNTI,transCb->ueCb->crnti,
               "ECSFB:: wrUmmPrcDatCfmTmrExp: Error Case \n");
         break;
   }
   RETVALUE(RFAILED);
}

/** @brief This function will handle ECSFB timer expiry events.
 *
 * @details
 *
 *     Function: wrUmmPrcEcsfbTmrExp
 *
 *         Processing steps:
 *         - Stop the DatCfm timer
 *         - Check the datCfm state and transCb state and free memory if any
 *
 * @param[in]  transCb : transaction inforamation
 * @return S16
 *    -#Success : ROK
 */
PRIVATE S16 wrUmmPrcEcsfbTmrExp
 (
 WrUmmTransCb           *transCb
 )
{
   WrUmmCellCb               *cellCb;
   WrUeCb                    *ueCb;
   S16                       ret = RFAILED;
   Bool                      sndDualRxTxRedirectInd = FALSE;/*RRC Rel10 Upgrade*/

   ueCb = transCb->ueCb;
   WR_UMM_GET_CELLCB(cellCb, ueCb->cellId);
   if (NULLP == cellCb)
   {
      RLOG0(L_ERROR, "wrUmmPrcEcsfbTmrExp: Failed to get cellCB\n");
      RETVALUE(RFAILED);
   }
   /*RRC Rel10 Upgrade - start*/ 
   if(TRUE == (wrUmmUtilGetUeEcsfbAndDualRxTxSuppCfgs(ueCb)) && \
		   (TRUE == (wrEmmGetEcsfbAndDualRxTxSuppCfgs(ueCb->cellId))))
   {
	   RLOG0(L_INFO, "wrUmmPrcEcsfbTmrExp:DualRxTxCfg supported\n");
	   sndDualRxTxRedirectInd = TRUE;
   }
   /*RRC Rel10 Upgrade - end*/ 

   switch(transCb->msg->u.tmrExpiry.timerEvnt)
   {
      case WR_TMR_RRC_DAT_CFM:
         {
			RLOG0(L_WARNING,"DatCfmTmr expired\n");
            if(ROK != wrUmmPrcDatCfmTmrExp(transCb))
            {
               RLOG0(L_ERROR, " sending RRC connection release message Failed ");
               RETVALUE(RFAILED);
            }
            ret = ROK;
         }
         break;
      case WR_TMR_DL_CDMA_RECV_HO_WAIT_TMR:
		 /*RRC Rel10 Upgrade - start*/ 
		 {
			 if(TRUE == sndDualRxTxRedirectInd)
			 {
			     RLOG0(L_WARNING, "wrUmmEcsfbPrcRrcMsg: S1 DL CDMA2K Tunnelling" 
								" message not received hence sending "
                                "HOFrmEutraPrepReq again with dualRxTxRedirectIndicator\n");

				 if(RFAILED != wrUmmSndHoFrmEutraPrepReq(ueCb, transCb, TRUE))
				 {
					 RLOG0(L_INFO, "wrUmmEcsfbPrcRrcMsg: HoFrmEutraPrepReq sent\n");
	   			     wrIncKpiCsfbSucc(CDMA2000_1XRTT,CDMA2000_CSFB_DUALRXTX_REDIR);
            		 ret = ROK;
		         }	
				 wrUmmTransComplete(transCb);
				 break;
			     /*IMPORTANT NOTE: if dualRxTx config is NOT supported then this is a
				   					      fall through case, hence "break" is inside*/
			 }
		 }
		 /*RRC Rel10 Upgrade - end*/ 
	  case WR_TMR_UL_HO_PREP_WAIT_TMR:
         {
            /* update the ongoing eCSFB transaction to TRANS_DONE and 
             * call the function to create internal msg to send RRC 
             * Connection release towards UE based on UE capablity 
             * as part of cfsb transaction */
            wrIncKpiCsfbFail(CDMA2000_CSFB_WO_PSHO_FAIL_REDIR_SUCC);

            if((ret =  wrUmmCreateIntCsfb(cellCb, ueCb, WR_UMM_INTMSG_CSFB_ECSFB_TRANS)) != ROK)
            {
               RLOG0(L_ERROR, "wrUmmPrcEcsfbTmrExp: Failed to create internal"
                     "CSFB transaction \n");
            }
            wrUmmTransComplete(transCb);
         }
         break;
      case WR_TMR_ECSFB_HO_EXEC_TMR:
         {
            /* On the expiry of tmr only eNB will
             * send the UE Ctxt release Request to MME with 
             * SztCauseRadioNwcs_fallback_triggeredEnum as cause
             */
            transCb->ueCb->csfbInfo.redirType = CDMA2000_CSFB_WITHOUT_PS_HO;
            transCb->ueCb->mobCtrlState = WR_HO_UE_RELEASE;
            transCb->errorCause = WR_UMM_CTXT_CSFB_REL;
            transCb->state = WR_UMM_TRANS_RELEASE_CALL; 
            RLOG_ARG0(L_WARNING, DBG_CRNTI,transCb->ueCb->crnti,
                  "ECSFB:: WR_TMR_ECSFB_HO_EXEC_TMR timer expired for UE,"
                  "sending UE context Release Request\n");
            ret = ROK;
         }
         break;
      case WR_TMR_ECSFB_MEAS_RPT:
         {
            RLOG_ARG0(L_INFO, DBG_CRNTI, ueCb->crnti,
                  "ECSFB:: WR_TMR_ECSFB_MEAS_RPT timer expired for UE\n");

            /* In case measurement Timer expired and no report receivd  then
             * release the UE with default re-direction. And Close the
             * ongoing eCSFB transaction
             */
            if(0 == transCb->u.ecsfbTransCb.numOfPilotLst)
            {
				/*RRC Rel10 Upgrade - start*/
			    if(TRUE == sndDualRxTxRedirectInd)
				{
					if(RFAILED != wrUmmSndHoFrmEutraPrepReq(ueCb, transCb, TRUE))
					{
	   					wrIncKpiCsfbSucc(CDMA2000_1XRTT,CDMA2000_CSFB_DUALRXTX_REDIR);
						RLOG0(L_INFO, "wrUmmEcsfbPrcRrcMsg: HoFrmEutraPrepReq sent\n");
            			ret = ROK;
					}
				}
				/*RRC Rel10 Upgrade - end*/
				else
				{
					wrIncKpiCsfbFail(CDMA2000_CSFB_WO_PSHO_FAIL_REDIR_SUCC);
					if((ret =  wrUmmCreateIntCsfb(cellCb, ueCb, WR_UMM_INTMSG_CSFB_ECSFB_TRANS)) != ROK)
					{
						RLOG0(L_ERROR, "wrUmmPrcEcsfbTmrExp: Failed to create"
								"internal CSFB transaction\n");
					}
				}
               wrUmmTransComplete(transCb);
            }
            else
            {
               /* Trigger eCSFB procedure, since all required CDMA measurement
                * reports are received */
               ret = wrUmmSndHoFrmEutraPrepReq(ueCb, transCb, FALSE);
            }
         }
         break;
   }
   RETVALUE(ret);
} /* wrUmmPrcEcsfbTmrExp */
/*Function will build and send MobilityFromEUTRACommand */
/** @brief This function will handle ECSFB transaction release.
 *
 * @details
 *
 *     Function: wrUmmEcsfbPrcHoSuccess
 *
 *         Processing steps:
 *         - Stop the DatCfm timer
 *         - Check the datCfm state and transCb state and free memory if any
 *
 * @param[in]  transCb : transaction inforamation
 * @return S16
 *    -#Success : ROK
 */
PRIVATE S16 wrUmmEcsfbPrcHoSuccess
(
 WrUmmTransCb        *transCb,
 TknStrOSXL          *cdmaS1Pdu,
 U32                 cdmaType
)
{
    if( ROK != wrUmmBldAndSndMobFrmEutraCmd(transCb, cdmaS1Pdu,  
                                            cdmaType))
    {
       RETVALUE(RFAILED);
    }

    RETVALUE(ROK);
} /* wrUmmEcsfbPrcHoSuccess */

/*Function will fill DLInfoTransfer message for CDMA and send to UE */
/** @brief This function will handle S1 DL tunneling message when HO status
 *  is Failure. It will forward the CDMA Pdu to UE through DLInfoTransfer
 *  message
 *
 * @details
 *
 *     Function: wrUmmEcsfbPrcHoFail
 *
 *         Processing steps:
 *         - It forwards the CDMA Pdu to UE
 *         - Set the state to WR_UMM_DAT_CFM_WAIT_DL_CDMA_INFO
 *
 * @param[in]  transCb   : transaction inforamation
 * @param[in]  cdmaS1Pdu : cdma Pdu recevied from MME
 * @param[in]  cdmType   : tyep of CDMA 
 * @return S16
 *    -#Success : ROK
 */
PRIVATE S16 wrUmmEcsfbPrcHoFail
(
 WrUmmTransCb        *transCb,
 TknStrOSXL          *cdmaS1Pdu,
 U32                 cdmaType
)
{
   WrUmmEcsfbTransCb    *ecsfbTransCb = &transCb->u.ecsfbTransCb;
   /*This function will Build and send the Dl CDMA Info message to UE
    * it will start DAT CFM timer also */
   if( ROK != wrUmmBuildAndSndCdmaDlInfTfr(transCb, cdmaS1Pdu, 
                                              cdmaType))
   {
      RETVALUE(RFAILED);
   }
   ecsfbTransCb->ecsfbState = WR_UMM_DAT_CFM_WAIT_DL_CDMA_INFO;
   ecsfbTransCb->numOfDlTxCnt++;
   /* Received DL S1 CDMA2000 Msg with HOStatus = Fail */
   wrIncKpiCsfbFail(CDMA2000_CSFB_WITHOUT_PS_HO);
   RETVALUE(ROK);
}

/*Function will check the HO status in S1 DL CDMA tunneling message
   * and call the corresponding handler
   */
/** @brief This function will handle message from S1AP interface used in ECSFB
 *
 * @details
 *
 *     Function: wrUmmS1EcsfbHdlS1apPdu
 *
 *         Processing steps:
 *         - Check the 
 *
 * @param[in]  transCb : transaction inforamation
 * @return S16
 *    -#Success : ROK
 */
PRIVATE S16 wrUmmS1EcsfbHdlS1apPdu
(
   WrUmmTransCb            *transCb
)
{
   WrUmmEcsfbTransCb          *ecsfbTransCb = &transCb->u.ecsfbTransCb;
   S1apPdu                    *pdu;
   SztInitiatingMsg           *initMsg;
   SztDlnkS1cdma2000tunneling *dlCdma;
   SztCdma2000PDU             *cdmaPdu = NULLP;
   U32                          ratTyp = 0xff;
   SztProtIE_Field_DlnkS1cdma2000tunnelingIEs *ie;
   U32                        idx;
   S16                        ret = RFAILED;
   U32                        eCSFBHoState = SztCdma2000HOStatushOFailEnum;

   /* Init the DL CDMA transaction */
   ecsfbTransCb->s1DlCdmaTunnMsg        = NULLP;
   ecsfbTransCb->cdmaS1Pdu              = NULLP;

   pdu     = transCb->msg->u.s1Pdu->pdu;
   initMsg = &pdu->pdu.val.initiatingMsg;
   dlCdma  = &initMsg->value.u.sztDlnkS1cdma2000tunneling;

   ie = dlCdma->protocolIEs.member;
   for(idx = 0; idx < dlCdma->protocolIEs.noComp.val; idx++)
   {
      switch(ie->id.val)
      {
         case Sztid_cdma2000PDU:
         {
            cdmaPdu = &ie->value.u.sztCdma2000PDU;
         }
         break;
         case Sztid_cdma2000RATTyp:
         {
            ratTyp = ie->value.u.sztCdma2000RATTyp.val;
         }
         break;
         case Sztid_cdma2000HOStatus:
         {
            eCSFBHoState =  ie->value.u.sztCdma2000HOStatus.val;
         }
         break;
      }
      ie = ie +1;
   }
   
   if (cdmaPdu == NULLP)
   {
      RLOG0(L_ERROR, " No CDMA PDU received ");
      RETVALUE(ROK);
   }

   wrStopTransTmr(transCb, WR_TMR_DL_CDMA_RECV_HO_WAIT_TMR);

   /*Check for the sztCdma2000HOStatus
   * if sztCdma2000HOStatus is FAILURE then call the handler to send DL
   *    InfoTransfer message to UE with CDMA pdu received in S1 DL CDMA 
   *    tunneling Msg
   * else (on Success)
   *    Call handler to send MobilityFromEUTRACommand with CDMA pdu received 
   *    in S1 DL CDMA tunneling Msg
   */
   /*RRC Rel10 Upgrade - start*/
   if((TRUE == wrEmmGetEcsfbAndDualRxTxSuppCfgs(transCb->ueCb->cellId))&& \
		   (TRUE == wrUmmUtilGetUeEcsfbAndDualRxTxSuppCfgs(transCb->ueCb)))
   {
	   RLOG0(L_INFO, "wrUmmS1EcsfbHdlS1apPdu:DualRxTxCfg supported\n");
	   ret = wrUmmEcsfbSndDlNas(transCb, cdmaPdu, ratTyp);
   }
   /*RRC Rel10 Upgrade - end*/
   else
   {
	   if(SztCdma2000HOStatushOFailEnum == eCSFBHoState)
	   {
		   /* In case of con-current HRPD, we have to wait for one more S1 DL
			* CDMA tunneling message and take a combined decision to send 
			* MobilityFromEutraCommand. This can be done by using bitmask. 
			* Bitmask will be set on the reception of ULHoPrepTransfer for 1xRTT
			* and HRPD and same will ne unset one by one on the reception of S1 DL
			* CDMA tunneling message (HO Command) for 1xRTT and HRPD
			*/

		   RLOG_ARG0(L_INFO, DBG_CRNTI,transCb->ueCb->crnti,"ECSFB:: S1AP DL CDMA"
				   "tunnelling message received with HO Status IE as failure for UE");
		   ret = wrUmmEcsfbPrcHoFail(transCb, cdmaPdu, ratTyp);
	   }
	   else
	   {
		   RLOG_ARG0(L_INFO, DBG_CRNTI,transCb->ueCb->crnti,"ECSFB:: S1AP DL CDMA"
				   "tunnelling message received with HO Status IE as success for UE");
		   ret = wrUmmEcsfbPrcHoSuccess(transCb, cdmaPdu, ratTyp);
		   ecsfbTransCb->ecsfbState = WR_UMM_ECSFB_HO_EXEC;
	   }
   }
   if(RFAILED != ret)
   {
      ecsfbTransCb->cdmaS1Pdu          = cdmaPdu;
      ecsfbTransCb->cdmaType           = ratTyp;
      ecsfbTransCb->s1DlCdmaTunnMsg    = transCb->msg;
      ecsfbTransCb->numOfDlTxCnt++;
      transCb->msg->refCnt++;
      RETVALUE(ROK);
   }
   else
   {
      RETVALUE(RFAILED);
   }
} /* wrUmmS1EcsfbHdlS1apPdu */


/* Function will do following
 * 1. get MSCID, Cell, Sector if cdma type is 1xRTT
 * 2. get Sector ID if cdma type is HRPD
 * 3. Encode either Sector Id(for HRPD) or Reference CellId(for 1xRTT)
 * 4. get the rand Value from UeCb
 * 5. Copy the CDMA PDU received in ULHoPrepTransfer to sztCdma2000PDU
 */

/** @brief This function will handle ECSFB transaction release.
 *
 * @details
 *
 *     Function: wrUmmFillAndSndS1UlCdmaTunnlMsg
 *
 *         Processing steps:
 *         - Stop the DatCfm timer
 *         - Check the datCfm state and transCb state and free memory if any
 *
 * @param[in]  transCb : transaction inforamation
 * @return S16
 *    -#Success : ROK
 */
PRIVATE S16 wrUmmFillAndSndS1UlCdmaTunnlMsg
 (
  WrUmmTransCb  *transCb,
  U32           cdmaType,
  Bool          hoReqdInd,
  TknStrOSXL    *dedicatedInfo
 )
{
   WrUeCb                 *ueCb = transCb->ueCb;
   WrUmmEcsfbTransCb      *ecsfbTransCb = &transCb->u.ecsfbTransCb;
   SztDatEvntReq           s1UlCdmaMsg;
   WrS1ConCb               *s1apConCb;
   TknStrOSXL             *encSztSectorIdInfo;
   WrUmmS1UlTnlMsgParams  s1UlTnlMsgParams = {0};
   U8                     idx;
   
    /* Get the S1 Con CB */
   s1apConCb = ueCb->s1ConCb;
   /* ccpu00127612 - temporary fix */
   /* Drop the UL NAS packets when UE-Associated S1 connection is not UP */
   if(s1apConCb == NULLP || ((s1apConCb != NULLP) && 
            (s1apConCb->s1apConnState != WR_S1AP_CONNECTED)))
   {
      ecsfbTransCb->ecsfbState = WR_UMM_ECSFB_TRANS_ERROR; 
      RETVALUE(RFAILED);
   }

   WR_SET_ZERO(&s1UlCdmaMsg, sizeof(SztDatEvntReq)); 
   /* Fill WrUmmS1UlTnlMsgParams structure which is passed. This structure 
    * contains all the parameter ulTnlMsgParamsrequired to build S1 UL tunneling message
    * for eCSFB procedure i.e with SRVCC IE and HoIndication IE */

      wrEmmGetCdma1xRttTgtCell(&encSztSectorIdInfo, transCb->ueCb->cellId);
   s1UlTnlMsgParams.cdmaType          = cdmaType;
   s1UlTnlMsgParams.mainSecId         = encSztSectorIdInfo;
   s1UlTnlMsgParams.hoReqdInd         = hoReqdInd;
   s1UlTnlMsgParams.numOfPilotLst     = ecsfbTransCb->numOfPilotLst;
   s1UlTnlMsgParams.PilotList         = ecsfbTransCb->cdmaRttPilotLst;
   s1UlTnlMsgParams.ulCdmaInfo        = dedicatedInfo;
   s1UlTnlMsgParams.meId              = ecsfbTransCb->meId;

   if (wrUmmS1apBldUlCdmaPdu(ueCb, &s1UlTnlMsgParams, &s1UlCdmaMsg.pdu) != ROK) 
   {
      /*Free encSztSectorIdInfo->val memory, which got allocated in SectorId 
       * Encoder function
       */
      ecsfbTransCb->ecsfbState = WR_UMM_ECSFB_TRANS_ERROR; 
      for(idx = 0; idx < ecsfbTransCb->numOfPilotLst; idx++)
      {
         WR_FREE(ecsfbTransCb->cdmaRttPilotLst[idx], sizeof(WrCdmaRttPilotLst)); 
      }
      ecsfbTransCb->ecsfbState = WR_UMM_ECSFB_TRANS_ERROR; 
      RETVALUE(RFAILED);
   }
   /*Free encSztSectorIdInfo->val memory, which got allocated in SectorId 
    * Encoder function
    */
   s1UlCdmaMsg.spConnId = s1apConCb->spConnId;

   /*Reset INACTIVITY TIMER by setting dataRcvd flag to TRUE*/
   wrIfmDamSetDataRcvdFlag(ueCb->cellId, ueCb->crnti);

  /* Dispatch the message using S1 interface to the selected MME         */
   if (ROK != WrIfmS1apDatReq(&s1UlCdmaMsg))
   {
      RLOG0(L_ERROR, " WrIfmS1apDatReq failed ");
      /*Free encSztSectorIdInfo->val memory, which got allocated in SectorId 
       * Encoder function
       */
      for(idx = 0; idx < ecsfbTransCb->numOfPilotLst; idx++)
      {
         WR_FREE(ecsfbTransCb->cdmaRttPilotLst[idx], sizeof(WrCdmaRttPilotLst)); 
      }
      ecsfbTransCb->ecsfbState = WR_UMM_ECSFB_TRANS_ERROR; 
      RETVALUE(RFAILED);

   }
       /* Started the new timer after sending the UL S1 CDMA 2000 tunneling 
        * message towards MME to wait for DL S1 CDMA2000 tunneling message*/

   ecsfbTransCb->ecsfbState = WR_UMM_DL_CDMA_RECV_HO_WAIT;

   wrStartTransTmr(transCb, WR_TMR_DL_CDMA_RECV_HO_WAIT_TMR, WR_TMR_DL_CDMA_RECV_HO_WAIT_TMR_VAL);

   RETVALUE(ROK);
} /* wrUmmFillAndSndS1UlCdmaTunnlMsg  */

/** @brief This function will be called when any message receives from UE 
 *     to enodeb which is belongs to ECSFB.   
 * @details
 *
 *     Function: wwrUmmEcsfbPrcRrcMsg
 *
 *         Processing steps:
 *         - When ULHandOverPrepTransfer is received from UE, do following
 *         1.Check if CDMA type 
 *         2.Trigger handler to fill and send S1 UL CDMA tunneling message to MME
 *           in which CDMA pdu of RRC message shall be copied in S1 UL tunneling
 *           message.
 *
 * @param [in] ueCb
 * @param [in] cellId 
 * @return S16
    -# Success : ROK
    -# Failure : RFAILED 
 */ 
PRIVATE S16 wrUmmEcsfbPrcRrcMsg
(
 WrUmmTransCb                *transCb
)
{  
   WrUeCb                                *ueCb;
   NhuUlDcchMsg                          *ulDcch;
   NhuUL_DCCH_MsgTypc1                   *c1;
   NhuULHovrPrepTfr                      *ulHovrPrepTfr;
   NhuULHovrPrepTfr_r8_IEs               *ulHovrPrepTfr_r8 = NULLP;
   NhuMeasurementReport                  *measRpt;
   NhuMeasurementReportcriticalExtnsc1   *measRptC1;
   NhuMeasResults                        *measRes;
   WrUmmMeasRptInfo                      measRptInfo;
   WrUmmEcsfbTransCb                     *ecsfbTransCb = &transCb->u.ecsfbTransCb;
   S16                                   ret = RFAILED;
   /* GA3.0 */
   WrCdmaRttPilotLst                     *cdmaRttPilotLst;
   WrUmmCellCb                           *cellCb = NULLP;
   U8                                    idx;
  
   TRC2(wrUmmEcsfbPrcRrcMsg)

   ueCb   = transCb->ueCb;
   WR_UMM_GET_CELLCB(cellCb, ueCb->cellId)
   if (NULLP == cellCb)
   {
      RLOG0(L_ERROR, "wrUmmEcsfbPrcRrcMsg Getting CELL CB Failed \n");
      RETVALUE(RFAILED);
   }
   
   ulDcch = &transCb->msg->u.uuIndPdu->sdu.m.ulDcchMsg;
   c1     = &ulDcch->dcchMsg.message.val.c1;
   switch(c1->choice.val)
   {
      case C1_MEASUREMENTREPORT:
         {
            RLOG0(L_INFO, "Measurement Report Received for 1xRTT Cell");
            measRpt = &c1->val.measurementReport;
            measRptC1 = &measRpt->criticalExtns.val.c1;
            measRes = &measRptC1->val.measurementReport_r8.measResults;
            ecsfbTransCb->recvMeasRptCnt++; 
            if(wrUmmMeasGetArfcnAndRptType(ueCb->ueIdx, ueCb->cellId,
                     measRes,&measRptInfo) != ROK)
            {
               RETVALUE(RFAILED);
            }
            if((ret = wrUmmEcsfbCdmaHdlMeasRpt(transCb, measRes, &measRptInfo)) != ROK)
            {
               RLOG0(L_ERROR, "Measurement report handling failed");
               /*Only this instnce of measurement report failed, so ignore this report
                * and wait for next one */
            }
            if(ecsfbTransCb->recvMeasRptCnt == ecsfbTransCb->measTransCb.
                  addMeasInfo.measIdCnt)
            {
               /*All required measurement reports received from UE, so stop the
                * timer and release the UE with valid re-direction Information if
                * at least ONE MR is valid */

               wrStopTransTmr(transCb, WR_TMR_ECSFB_MEAS_RPT);

               /* In case all received measurement reports are Empty then
                * release the UE with default re-direction. And Close the
                * ongoing eCSFB transaction
                */
			   if(0 == ecsfbTransCb->numOfPilotLst)
			   {
				   /*RRC Rel10 Upgrade - start*/
				   if(TRUE == (wrUmmUtilGetUeEcsfbAndDualRxTxSuppCfgs(ueCb)) && \
						   (TRUE == (wrEmmGetEcsfbAndDualRxTxSuppCfgs(ueCb->cellId))))
				   {
	   				   RLOG0(L_INFO, "wrUmmEcsfbPrcRrcMsg:DualRxTxCfg supported\n");
					   if(RFAILED != wrUmmSndHoFrmEutraPrepReq(ueCb, transCb, TRUE))
					   {
						   RLOG0(L_INFO, "wrUmmEcsfbPrcRrcMsg:HoFrmEutraPrepReq sent\n");
	   					   wrIncKpiCsfbSucc(CDMA2000_1XRTT,CDMA2000_CSFB_DUALRXTX_REDIR);
         				   ret = ROK;
					   }
				   }
				   /*RRC Rel10 Upgrade - end*/
				   else
				   {
					   if((ret =  wrUmmCreateIntCsfb(cellCb, ueCb, WR_UMM_INTMSG_CSFB_ECSFB_TRANS)) != ROK)
					   {
						   RLOG0(L_ERROR, "wrUmmEcsfbPrcRrcMsg: Failed to create"
								   "internal CSFB transaction\n");
					   }
				   }
				   wrUmmTransComplete(transCb);
			   }
			   else
               {
                  /* Trigger eCSFB procedure, since all required CDMA measurement
                   * reports are received */
                  if(ROK != wrUmmSndHoFrmEutraPrepReq(ueCb, transCb, FALSE))
                  {
                     RLOG0(L_ERROR, "wrUmmEcsfbPrcRrcMsg: Failed to send"
                           "HoFrmEutraPrepReq\n");
                     ret = RFAILED;
                  }
               }
            }
         }
         break;
      case C1_ULHOVRPREPTFR:
		 {
			 if((ecsfbTransCb->ecsfbState == WR_UMM_WAIT_AFTER_HOPREP_REQ)|| \
			   (ecsfbTransCb->ecsfbState == WR_UMM_ECSFB_RRC_DAT_CFM_RECVD_FOR_HOPREP_REQ))
			 {
				 ulHovrPrepTfr     = &c1->val.ulHovrPrepTfr;
				 ulHovrPrepTfr_r8  = 
					 &ulHovrPrepTfr->criticalExtns.val.c1.val.ulHovrPrepTfr_r8;

				 /* Added this Check Because RRC and S1AP enums are not same from
				  * the Spec - which are revese for 1XRTT and CDMS */
				 if(NhuCDMA2000_Typtype1XRTTEnum == 
						 ulHovrPrepTfr_r8->cdma2000_Typ.val)
				 {
					 ecsfbTransCb->cdmaType = SztCdma2000RATTyponexRTTEnum;
				 }
				 else /*NhuCDMA2000_TyptypeHRPDEnum*/
				 {
					 ecsfbTransCb->cdmaType = SztCdma2000RATTyphRPDEnum;
				 } 

				 /* stop the WR_TMR_UL_HO_PREP_WAIT_TMR timer */
				 wrStopTransTmr(transCb, WR_TMR_UL_HO_PREP_WAIT_TMR);

				 /* Copy MEID from the UlHandoverPrepTransfer message */
				 if(SztCdma2000RATTyponexRTTEnum == ecsfbTransCb->cdmaType)
				 {
					 if(PRSNT_NODEF ==  ulHovrPrepTfr_r8->meid.pres)
					 {
						 ecsfbTransCb->meId = &ulHovrPrepTfr_r8->meid;
					 }
				 }
				 /* GA3.0 */
				 /*In case of 
				  *    -->eCSFB WITHOUT measurement case get the default cell
				  *       from NRT and fill in SRVCC
				  *    -->eCSFB WITH measurement case get all the pilot from
				  *    ecsfbtransCb->cdmaRttPilotLst table and fill each in sRVCC
				  *    PilotList */
				 if((LWR_ECSFB_WITHOUT_MEAS ==  ecsfbTransCb->eCsfbMeasEnbFlag) &&
						 (SztCdma2000RATTyponexRTTEnum == ecsfbTransCb->cdmaType))
				 {
					 /* Since measurement is disabled for eCSFB, so get only one cell
					  * from NRT by calling ANR function. ANR will have the logic
					  * to find the first cell(pnOffset) in first supported band,
					  * frequency */
					 cdmaRttPilotLst = NULLP;
					 ecsfbTransCb->numOfPilotLst = 0;
					 if(ROK != wrUmmGet1xRttNeighCell(ueCb, &cdmaRttPilotLst, 
								 CDMA2000_1XRTT))
					 {
						 RETVALUE(RFAILED);
					 }
					 else
					 {
						 /* One CDMA 1xRTT cell is fetched from Measurement and 
						  * ANR NRT this cell will be stored in 
						  * ecsfbTransCb->cdmaRttPilotLst[]
						  */
						 ecsfbTransCb->cdmaRttPilotLst[ecsfbTransCb->numOfPilotLst++]
							 = cdmaRttPilotLst;
					 }
				 }
				 else
				 {
					 RLOG0(L_DEBUG," Received ULHOPREPTx for eCSFB with MEASUREMENTREPORT");
					 /*for HRPD we need to call different ANR function to fetch 
					  * one or mutiple HRPD target cells to be filled in PilotList IE
					  * of SRVCC IE*/ 
				 } 
				 if(SztCdma2000RATTyponexRTTEnum == ecsfbTransCb->cdmaType)
				 {
					 if((ret = wrUmmFillAndSndS1UlCdmaTunnlMsg(transCb, 
									 ecsfbTransCb->cdmaType, TRUE, 
									 &ulHovrPrepTfr_r8->dedicatedInfo)) == ROK)
					 {
                                            RLOG_ARG2(L_EVENT, DBG_CRNTI, transCb->ueCb->crnti,"[S1AP]:Sending UPLINK "
                                                     "S1 CDMA2000 TUNNELING [MME-UE-S1AP-ID:%lu] [eNB-UE-S1AP-ID:%lu]",
                                                     transCb->ueCb->s1ConCb->mme_ue_s1ap_id,
                                                     transCb->ueCb->s1ConCb->enb_ue_s1ap_id);
						 /*Free the complete cdmaPilotList table, since S1 UL tunneling
						  * message is sent*/
						 for(idx = 0; idx < ecsfbTransCb->numOfPilotLst; idx++)
						 {
							 WR_FREE(ecsfbTransCb->cdmaRttPilotLst[idx],
									 sizeof(WrCdmaRttPilotLst)); 
						 }

					 }
					 RETVALUE(ret);
				 }
				 else /* HRPD not supported, so returning failure */
				 {
					 RETVALUE(RFAILED);
				 }
			 }
			 else
			 {
				 RLOG0(L_DEBUG," Received ULHOPREPTRANSFER before sending"
						 "HOFromEUTRAPreparationRequest");
				 RETVALUE(RFAILED);
			 }
          RLOG_ARG0(L_EVENT, DBG_CRNTI,transCb->ueCb->crnti,"[OTA] Received ECSFB::"
                "ULHOVRPREPTFR  message received for UE");
		 }
      break;
      case C1_RRCCONRECFGNCOMPL:
      {
         /* Stop the RRC Reconfiguration Timer */
         /*wrStopTransTmr(transCb,WR_TMR_RECONFIGURE);*/
         /*Start the Timer*/
         cmInitTimers(&(ecsfbTransCb->measRptTmr.tmr), 1);
         /* Start timer to wait for measurment reports from UE */
         wrStartTransTmr(transCb, WR_TMR_ECSFB_MEAS_RPT, WR_TMR_ECSFB_CFG_VAL);
         ret = ROK;
         break;
      }

      default:
      {
         RLOG0(L_WARNING, "Currently there is no handling for dcchMsg message received as part of a UU_IND_PDU");
         RETVALUE(RFAILED);
      }
   }
    RETVALUE(ret);
}
/*Function fills the HoFromEutraPrepReq and send it to RRC */
/** @brief This function will handle ECSFB transaction release.
 *
 * @details
 *
 *     Function: wrUmmFillAndSndHoFrmEutraPrepReq
 *
 *         Processing steps:
 *         - Stop the DatCfm timer
 *         - Check the datCfm state and transCb state and free memory if any
 *
 * @param[in]  transCb : transaction inforamation
			   sndDualRxTxRedirectInd : dualRxTxCfg support
 * @return S16
 *    -#Success : ROK
 */
PRIVATE S16 wrUmmFillAndSndHoFrmEutraPrepReq
 (
 WrUmmTransCb  *transCb,
 Bool          sndDualRxTxRedirectInd
 )
{
	NhuDatReqSdus       *nhuDatReqSdu = NULLP;
	WrUeCb              *ueCb         = NULLP;
	WrUmmEcsfbTransCb   *ecsfbTransCb  = NULLP;
	U32                 transId;
	U16                 cellId;
	WrUmmCellCb         *cellCb;
	Bool                isDualRxTxCfgSup = FALSE;/*RRC Rel10 Upgrade*/

	ueCb   = transCb->ueCb;
	ecsfbTransCb = &transCb->u.ecsfbTransCb;
	cellId = ueCb->cellId;

	WR_UMM_GET_CELLCB(cellCb, cellId)
		if (NULLP == cellCb)
		{
			RLOG0(L_ERROR, "wrUmmFillAndSndHoFrmEutraPrepReq"
					"Getting CELL CB Failed \n");
			RETVALUE(RFAILED);
		}
   /*RRC Rel10 Upgrade - start*/
   if(TRUE == (wrUmmUtilGetUeEcsfbAndDualRxTxSuppCfgs(ueCb)) && \
		   (TRUE == (wrEmmGetEcsfbAndDualRxTxSuppCfgs(ueCb->cellId))))
   {
	   RLOG0(L_INFO, "wrUmmFillAndSndHoFrmEutraPrepReq:DualRxTxCfg supported\n");
	   isDualRxTxCfgSup = TRUE;
   }
   /*RRC Rel10 Upgrade - end*/
	
	WR_ALLOCEVNT(&nhuDatReqSdu, sizeof(*nhuDatReqSdu));
	if (nhuDatReqSdu == NULLP)
	{
		RLOG0(L_FATAL, " Allocation of RRC Connection release message  Failed ");
		RETVALUE(RFAILED);
	}

	transId = (transCb->transId | WR_UMM_ECSFB_HO_FROM_EUTRA_TRANSID);
	wrUmmFillRrcEvntHdr(&nhuDatReqSdu->hdr, transCb->ueCb->cellId, transCb->ueCb->crnti, transId);
	nhuDatReqSdu->sdu.isUeCfgPres    = FALSE;
	nhuDatReqSdu->sdu.ueMsg.isMsgDed = TRUE;
	nhuDatReqSdu->sdu.ueMsg.msg.dedMsg.dlDcchMsg.pdcpId = WR_PDCP_ID_SRB1;


	wrUmmRrcFillHoFrmEutraPrepReqInfo(nhuDatReqSdu, ueCb, ecsfbTransCb->cdmaType, 
			ecsfbTransCb->isConCurrHoHrpd, 
			ecsfbTransCb->encMobilityParamsInfo,sndDualRxTxRedirectInd);

   RLOG_ARG0(L_EVENT, DBG_CRNTI, ueCb->crnti,"[OTA] Sending " 
         "HandoverFromEUTRAPreparationRequest ");
	if(wrIfmRrcSndReqToUe(nhuDatReqSdu) != ROK)
	{
		RLOG0(L_ERROR, " wrIfmRrcSndReqToUe failed ");
		WR_FREE_EVNT(nhuDatReqSdu); 
		RETVALUE(RFAILED);
	}

	/*RRC Rel10 Upgrade - start*/
	if(FALSE == sndDualRxTxRedirectInd)
	{
	    /* Starting UlHoPrepWaitTmr after sending the HandoverFromEUTRAPreparationRequest
	 	* message towards UE only if dualRxTxRedirectIndicator-r10 is not included*/
		wrStartTransTmr(transCb, WR_TMR_UL_HO_PREP_WAIT_TMR, WR_TMR_UL_HO_PREP_WAIT_TMR_VAL);
		RLOG_ARG0(L_INFO, DBG_CRNTI,transCb->ueCb->crnti," ECSFB::"
			"HandoverFromEUTRAPreparationRequest message sent to UE:UlHoPrepWaitTmr started");
	}
	if(TRUE == isDualRxTxCfgSup)
	{
		/*isDualRxTxCfgSup check can be removed once ccpu00141982 is implemented*/
		/* Start DAT CFM Timer for this DL message */
	    wrStartTransTmr(transCb,WR_TMR_RRC_DAT_CFM, WR_TMR_RRC_DAT_CFM_VAL);
		RLOG_ARG0(L_INFO, DBG_CRNTI,transCb->ueCb->crnti," ECSFB::"
			"HandoverFromEUTRAPreparationRequest message sent to UE:DatCfmTmr started");
	}
	/*RRC Rel10 Upgrade - end*/
	RETVALUE(ROK);
} /* wrUmmFillAndSndHoFrmEutraPrepReq */


/** @brief This function will build the HoFromEutraPrepReq and send it towards
 * UE. This is one single point function to trigger eCSFB procedure without
 * measurment case
 *    @param[in]  ueCb    : ueCb Information
				  sndDualRxTxRedirectInd: dualRxTxCfg support
 *    @param[in]  transCb : transaction inforamation
 */
 PRIVATE S16 wrUmmSndHoFrmEutraPrepReq
 (
  WrUeCb       *ueCb, 
  WrUmmTransCb *transCb,
  Bool         sndDualRxTxRedirectInd
 )
{
   WrUmmCellCb             *cellCb;
   U16                     cellId;
   WrUmmEcsfbTransCb       *ecsfbTransCb = &transCb->u.ecsfbTransCb;
   S16                     ret = RFAILED;              

   TRC2(wrUmmSndHoFrmEutraPrepReq)

   cellId         = ueCb->cellId;

   WR_UMM_GET_CELLCB(cellCb, cellId)
   if(NULLP == cellCb) 
   {
      RLOG0(L_ERROR, " Getting cell CB Failed ");
      RETVALUE(RFAILED);
   }
   /*Below API will Fill the HOFromEutraPrepReq and send it to RRC.
   */
   /*If Random number is already present in ueCb as StrBSXL then 
    * use the same else generate a new random number */
   /*Get the selected neighbor cell and fill the folloing information 
    * about neighbor cell in transCb and ueCb
    * 1. generate rand number if it does not exist in ueCb
    * 2. store MscId, cell and sector in transCb  
    * 3. Store the encoded MobilityParams in transCb
    */
   wrUmmGetRand32(ueCb);

   /* This function should enocde the MP , allocate the buffer and store 
    * the address of encoded pdu in encMobilityParamsInfo whose address 
    * is passed as argument */
   ecsfbTransCb->encMobilityParamsInfo  = &cellCb->mobParam1xOct;
   /* Rat type will be 1xRTT for HoFromEutraPrepReq */
   ecsfbTransCb->cdmaType = NhuCDMA2000_Typtype1XRTTEnum; 
   ecsfbTransCb->isConCurrHoHrpd = FALSE;
   /*RRC Rel10 Upgrade - start*/
   if(ROK == wrUmmFillAndSndHoFrmEutraPrepReq(transCb,sndDualRxTxRedirectInd))
   {
	   ecsfbTransCb->ecsfbState = WR_UMM_WAIT_AFTER_HOPREP_REQ;
	   ret = ROK;
   }
   /*RRC Rel10 Upgrade - end*/
   else
   {
      RLOG0(L_ERROR, "Failed to send wrUmmFillAndSndHoFrmEutraPrepReq\n");
      ret = RFAILED;
   }
   RETVALUE(ret);
}

/** @brief This function will be called when CS Fallback Indicator
 *          is received either in context modication or intial context 
 *          setup Request and eCSFB procedure is triggered.   
 * @details
 *
 *     Function: wrUmmEcsfbPrcIntMsg
 *
 *         Processing steps:
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
PRIVATE S16 wrUmmEcsfbPrcIntMsg
(
 WrUmmTransCb                *transCb
)
 {
   WrUmmCellCb             *cellCb;
   WrUeCb                  *ueCb;
   WrUmmIncMsg             *msg = transCb->msg;
   U16                     cellId;
   WrUmmEcsfbTransCb       *ecsfbTransCb = &transCb->u.ecsfbTransCb;
   S16                     ret = RFAILED;              
   WrUmmIntCsfbTransMsg    *ecsfbIntMsg;
   Bool                    sndDualRxTxRedirectInd = FALSE;/*RRC Rel10 Upgrade*/

   TRC2(wrUmmEcsfbPrcIntMsg)

   ueCb           = transCb->ueCb;
   cellId         = ueCb->cellId;

   WR_UMM_GET_CELLCB(cellCb, cellId)
   if(NULLP == cellCb) 
   {
      RLOG0(L_ERROR, " Getting cell CB Failed ");
      RETVALUE(RFAILED);
   }

   if(WR_UMM_INTMSG_ECSFB_TRANS == msg->u.internalMsg->intMsgType)
   { 
       ecsfbIntMsg = msg->u.internalMsg->u.csfbIndMsg;
       ecsfbTransCb->ratPriority   = ecsfbIntMsg->ratPriority;
      /*if Measurement option is supported then trigger measurement procedure 
       *for CDMA/HRPD i.e I-RAT frequency and skip sending HoFromEutraPrepReq
       *at this point of time */
      if (ROK != wrEmmIsCsfbOrEcsfbMeasSup(ueCb->cellId, 
                               TRUE, &ecsfbTransCb->eCsfbMeasEnbFlag))
      {
         RETVALUE(RFAILED);
      }
      /*RRC Rel10 Upgrade - start*/ 
	  if(TRUE == (wrUmmUtilGetUeEcsfbAndDualRxTxSuppCfgs(ueCb)) && \
				(TRUE == (wrEmmGetEcsfbAndDualRxTxSuppCfgs(ueCb->cellId))))
	  {
	        RLOG0(L_INFO, "wrUmmEcsfbPrcIntMsg:DualRxTxCfg supported\n");
      		sndDualRxTxRedirectInd = TRUE;
      }
      /*RRC Rel10 Upgrade - end*/ 
 
      if(WR_UMM_ECSFB_WITH_MEAS == ecsfbTransCb->eCsfbMeasEnbFlag)
      {
         /*Measurement is enabled at Cell lavel for eCSFB procedure, now
          * check whether UE supports the IRAT i.e 1xRTT or HRDP
          * event/periodic measurment or not */
         if((ret = wrUmmMeasCsfbCdmaCfg(ueCb, transCb)) == ROK)
         {
            /* measurement send towards UE, henc st th eCSFBTrans here */
            ecsfbTransCb->ecsfbState = WR_UMM_MEAS_CFGD;
         }
         else
         {
            /*At Cell level Measurment is enabled, but measurment configuration
             * failed, because UE doesn't support measType then select eCSFB
             * without measurement */
            ecsfbTransCb->eCsfbMeasEnbFlag = WR_UMM_ECSFB_WITHOUT_MEAS;
			/*RRC Rel10 Upgrade - start*/
			if(TRUE == sndDualRxTxRedirectInd)
			{
				if(RFAILED != wrUmmSndHoFrmEutraPrepReq(ueCb, transCb, TRUE))
				{
					wrIncKpiCsfbSucc(CDMA2000_1XRTT,CDMA2000_CSFB_DUALRXTX_REDIR);
					RLOG0(L_INFO,"wrUmmEcsfbPrcIntMsg: eNodeB supports measurements "
					"but UE doesn't hence sending HoFromEutraPrepReq "
					"with dualRxTxRedirectInd-r10\n");
				}
				wrUmmTransComplete(transCb);
				ret=ROK;
			}
			else
			{
				ret = wrUmmSndHoFrmEutraPrepReq(ueCb, transCb, FALSE);
			}
			/*RRC Rel10 Upgrade - end*/
		 }
	  }/*With measurement case ends */
	  else
	  {
		  /* Without Measurement Case, trigger HoFromEutraPrepReq 
		   */
		  /*RRC Rel10 Upgrade - start*/
		  if(TRUE == sndDualRxTxRedirectInd)
		  {
			  if(RFAILED != wrUmmSndHoFrmEutraPrepReq(ueCb, transCb, TRUE))
			  {
				  wrIncKpiCsfbSucc(CDMA2000_1XRTT,CDMA2000_CSFB_DUALRXTX_REDIR);
				  RLOG0(L_INFO,"wrUmmEcsfbPrcIntMsg without meas: HoFromEutraPrepReq sent\n");
			  }
			  wrUmmTransComplete(transCb);
			  ret=ROK;
		  }
		  else
		  {
			  ret = wrUmmSndHoFrmEutraPrepReq(ueCb, transCb, FALSE);
		  }
		  /*RRC Rel10 Upgrade - end*/
	  }/*without measurement case ends */
   }
   RETVALUE(ret);
 } /* wrUmmEcsfbPrcIntMsg */ 

/** @brief This function will handle ECSFB transaction release.
 *
 * @details
 *
 *     Function: wrUmmEcsfbTransRel
 *
 *         Processing steps:
 *         - Stop the DatCfm timer
 *         - Check the datCfm state and transCb state and free memory if any
 *
 * @param[in]  transCb : transaction inforamation
 * @return S16
 *    -#Success : ROK
 */
PUBLIC S16 wrUmmEcsfbTransRel
(
WrUmmTransCb                 *transCb
)
{
   WrUmmEcsfbTransCb         *ecsfbTransCb = &transCb->u.ecsfbTransCb;

   /* can be based on state */
   wrStopTransTmr(transCb, WR_TMR_RRC_DAT_CFM);

   /* Failure in transaction, free any memory if its not freed
    * Complete the transaction */
   if(WR_UMM_DAT_CFM_WAIT_DL_CDMA_INFO == ecsfbTransCb->ecsfbState)
   {
      /*Free the info which was stored in ecsfbTransCb to build the 
       * message: Need to check the free function 
       * WR_FREE_EVNT(ecsfbTransCb->cdmaS1Pdu);
       */
      wrUmmRlsIncMsg(&ecsfbTransCb->s1DlCdmaTunnMsg);
   }
   RETVALUE(ROK);
} /* wrUmmEcsfbTransRel */
/** @brief This function(transaction) handles all ECSFB related messages.
 *        
 * @details
 *
 * Function:wrUmmEcsfbProcessor
 *
 *   Processing steps:
 *    List of Messages handled in this transaction
 *         1. WR_UMM_INTERNAL_MSG
 *         2. WR_UMM_UU_IND_PDU
 *         3. WR_UMM_S1_DAT_IND
 *         4. WR_UMM_TMR_EXPIRY
 *    -Check the message type
 *    
 * @param[in]  transCb: pointer to transaction control block
 * @return S16
 *  -#Success : ROK
 *  -#Failure : RFAILED
 */
PUBLIC S16 wrUmmEcsfbProcessor
(
 WrUmmTransCb                *transCb
)
{
   WrUmmIncMsg               *msg       = transCb->msg;
   WrUmmEcsfbTransCb         *ecsfbTransCb = &transCb->u.ecsfbTransCb;

   switch(msg->msgTyp)
   {
      case WR_UMM_INTERNAL_MSG:
         {
            if(RFAILED == wrUmmEcsfbPrcIntMsg(transCb))
            {
               RLOG0(L_ERROR, " Processing Internal message Failed ");
               ecsfbTransCb->ecsfbState = WR_UMM_ECSFB_TRANS_ERROR;
            }
            break;
         }
      case WR_UMM_UU_IND_PDU:
         {
            if(ROK != wrUmmEcsfbPrcRrcMsg(transCb))
            {
               RLOG0(L_ERROR, " Processing UU Indication message Failed ");
               ecsfbTransCb->ecsfbState = WR_UMM_ECSFB_TRANS_ERROR;
            }
            break;
         }
      case WR_UMM_S1_DAT_IND:
         {
            /* Handle S1ap PDU*/
            if (ROK != wrUmmS1EcsfbHdlS1apPdu(transCb))
            {
               RLOG0(L_ERROR, " Failed to handle S1AP PDU. ");
               ecsfbTransCb->ecsfbState = WR_UMM_ECSFB_TRANS_ERROR;
            }/* End of wrUmmX2SrcHoHandleX2apPdu*/
         }
         break;
      case WR_UMM_TMR_EXPIRY:
         {
            if(ROK != wrUmmPrcEcsfbTmrExp(transCb))
            {
                ecsfbTransCb->ecsfbState = WR_UMM_ECSFB_TRANS_ERROR;
            }
         }
         break;
      case WR_UMM_SCHD_CFG:
      {
         if (transCb->u.ecsfbTransCb.measTransCb.measGapStatus ==
                                             WR_UMM_MEAS_GAP_INIT_SCH_CONFIG)
         {
            RLOG0(L_DEBUG, " Received Schd Gap Setup Cfm ");
#ifdef WR_RSYS_KPI 
               /* Schd Cfm for MeasGap is received */
            transCb->ueCb->measGapUsd = TRUE;
#endif /*WR_RSYS_KPI*/

            /* Change the measGapStatus to UE Config */
            transCb->u.ecsfbTransCb.measTransCb.measGapStatus =
                                               WR_UMM_MEAS_GAP_INIT_UE_CONFIG;

               /*Prepare and send the RRC Reconfiguration message */
               if(ROK != wrUmmMeasPrepareSendRrcRecfg(transCb->ueCb->cellId, 
                        transCb->ueCb, 
                        &transCb->u.ecsfbTransCb.measTransCb, 
                        transCb->transId))
               {
                  /* Failed to send the message to the UE. 
                   * Trigger normal CSFB i.e RRC release with re-direction and 
                   * close the transaction
                   */
                  WrUmmCellCb               *cellCb;

                  WR_UMM_GET_CELLCB(cellCb, transCb->ueCb->cellId);

                  if(NULLP == cellCb) 
                  {
                     RLOG0(L_ERROR, " Getting cell CB Failed ");
                     RETVALUE(RFAILED);
                  }

                  if(ROK != wrUmmCreateIntCsfb(cellCb, transCb->ueCb, WR_UMM_INTMSG_CSFB_ECSFB_TRANS))
                  {
                     RLOG0(L_ERROR, "wrUmmPrcEcsfbTmrExp: Failed to create "
                           "internal CSFB transaction \n");
                  }
                  wrUmmTransComplete(transCb);
               }
         }
         else if (transCb->u.ecsfbTransCb.measTransCb.measGapStatus ==
                                                   WR_UMM_MEAS_GAP_REL)
         {
            /*Measurment Gap release is not triggered from CSFB transaction */
            RLOG0(L_DEBUG, "Received Scd Cfm for MeasGap rel");
                  
         }
         break;
      }
      case WR_UMM_RRC_DAT_CFM:
         {
            /* Stop the Dat Cfm timer*/
            wrStopTransTmr(transCb, WR_TMR_RRC_DAT_CFM);
			/*RRC Rel10 Upgrade - start*/
	        if((msg->u.uuCfmPdu->hdr.transId & 0x0f) == WR_UMM_ECSFB_HO_FROM_EUTRA_TRANSID)
            {
				ecsfbTransCb->ecsfbState = WR_UMM_ECSFB_RRC_DAT_CFM_RECVD_FOR_HOPREP_REQ;
			}
            /*removed state WR_UMM_ECSFB_RRC_DAT_CFM_RECVD*/
			/*RRC Rel10 Upgrade - end*/
	        if((msg->u.uuCfmPdu->hdr.transId & 0x0f) == WR_UMM_ECSFB_DL_CDMA_INFO_TRANSID)
            {
               ecsfbTransCb->numOfDlTxCnt = 0;
               /*Free the info which was stored in ecsfbTransCb to build the 
               * message: Need to check the frre function 
          WR_FREE_EVNT(ecsfbTransCb->cdmaS1Pdu);
          */
               wrUmmRlsIncMsg(&ecsfbTransCb->s1DlCdmaTunnMsg);
               wrUmmTransComplete(transCb);
            }
            break;
         }
      default :
         RLOG1(L_ERROR, " Invalid msgType(%d) ", msg->msgTyp);
   }
   /* If error occurs in any message during this transation 
    * then free all memory if any and complete the transaction
    */
   if (WR_UMM_ECSFB_TRANS_ERROR == ecsfbTransCb->ecsfbState)
   {
      wrUmmTransComplete(transCb);
   }

   RETVALUE(ROK);
} /* wrUmmEcsfbProcessor */ 


/* @details
 *
 *Function: wrUmmEcsfbPrcDeferTrans
 *
 *Description:
 *   This function is called from the main transaction to invoke its 
 *   deferred trnsaction. eCSFB transaction will be in the deferred queue
 *   of re-establishment transaction. On the reception of RRC Re-Configurtion 
 *   complete message eCSFB transaction will in invoked.
 *
 * @param[in]  transCb    : pointer to transaction control block
 * @param[in]  defTransCb : pointer to Deferred transaction control block
 * @return S16
 *    -#Success : ROK
 *    -#Failure : RFAILED
 */
PUBLIC S16 wrUmmEcsfbPrcDeferTrans
(
  WrUmmTransCb                *transCb,
  WrUmmTransCb                *defTransCb
)
{
   NhuUL_DCCH_Msg            *ulDcchPdu = NULLP;
   U8                        messageType = 0;
   WrUmmEcsfbTransCb         *ecsfbTransCb = &defTransCb->u.ecsfbTransCb;
   WrUmmCellCb               *cellCb;
   WrUeCb                    *ueCb;
   Bool                      sndDualRxTxRedirectInd = FALSE;/*RRC Rel10 Upgrade*/

   TRC2(wrUmmEcsfbPrcDeferTrans)

      ueCb = transCb->ueCb;
   WR_UMM_GET_CELLCB(cellCb, ueCb->cellId);
   if (NULLP == cellCb)
   {
      RLOG0(L_ERROR, " wrUmmEcsfbPrcDeferTrans: Getting cell CB Failed ");
      RETVALUE(RFAILED);
   }

   if(transCb->msg->msgTyp != WR_UMM_UU_IND_PDU)
   {
      RETVALUE(ROK);
   }
   /*RRC Rel10 Upgrade - start*/
   if(TRUE == (wrUmmUtilGetUeEcsfbAndDualRxTxSuppCfgs(ueCb)) && \
		   (TRUE == (wrEmmGetEcsfbAndDualRxTxSuppCfgs(ueCb->cellId))))
   {
	   RLOG0(L_INFO, "wrUmmEcsfbPrcDeferTrans:DualRxTxCfg supported\n");
	   sndDualRxTxRedirectInd = TRUE;
   }
   /*RRC Rel10 Upgrade - end*/
   ulDcchPdu = &(transCb->msg->u.uuIndPdu->sdu.m.ulDcchMsg.dcchMsg);
   messageType = ulDcchPdu->message.val.c1.choice.val;
   switch(messageType)
   {
      case C1_RRCCONRECFGNCOMPL:
         {
            switch(ecsfbTransCb->ecsfbState)
            {
               case WR_UMM_DAT_CFM_WAIT_DL_CDMA_INFO:
                  {
                     /* RRC Re-establishment is successful and eNB is waiting for 
                      * WR_UMM_DAT_CFM_WAIT_DL_CDMA_INFO, then build the Dl Info Transfer
                      * and forward it again */
                     if(RFAILED != wrUmmBuildAndSndCdmaDlInfTfr(defTransCb,
                              ecsfbTransCb->cdmaS1Pdu, ecsfbTransCb->cdmaType))
                     {
                        ecsfbTransCb->ecsfbState = WR_UMM_DAT_CFM_WAIT_DL_CDMA_INFO;
                        ecsfbTransCb->numOfDlTxCnt++;
                        RLOG0(L_INFO, "wrUmmEcsfbPrcDeferTrans:Again sending"
                              " S1 DL CDMA2000 message \n");
                        RETVALUE(WR_UMM_RESUME_DEF_TRANSACTION);
                     }
                     else
                     {
                        RLOG0(L_ERROR, "wrUmmEcsfbPrcDeferTrans:Max re-transmission"
                              "happened for S1 DL CDMA2000 message,closing the"
                              "eCSFB transaction\n");
                        RETVALUE(WR_UMM_STOP_OLD_TRANSACTION);
                     }
                  }
                  break;
               case WR_UMM_WAIT_AFTER_HOPREP_REQ:
                  {
                     /* RRC Re-establishment is successful,hence send HandoverFromEUTRAPreparationRequest
                      * msg towards UE and start the UlHoPrepWaitTmr again */

                     /*This function will build the message and send it to RRC
                      * it will start the UlHoPrepWaitTmr also.
                      */
                     if(RFAILED != wrUmmSndHoFrmEutraPrepReq(ueCb, defTransCb, sndDualRxTxRedirectInd))
                     {
                        if(TRUE == sndDualRxTxRedirectInd)
                        {
                           RLOG0(L_ERROR, "wrUmmEcsfbPrcDeferTrans:R10:Again Sending"
                                 " HoFromEutraPrepReq towards UE.\n");
                           wrIncKpiCsfbSucc(CDMA2000_1XRTT,CDMA2000_CSFB_DUALRXTX_REDIR);
                           wrUmmTransComplete(transCb);
                           RETVALUE(WR_UMM_STOP_OLD_TRANSACTION);
                        }
                        else
                        {
                           RLOG0(L_ERROR, "wrUmmEcsfbPrcDeferTrans:R9:Sending"
                                 " HoFromEutraPrepReq towards UE\n");
                           RETVALUE(WR_UMM_RESUME_DEF_TRANSACTION);
                        }
                     }
                     else
                     {
                        RLOG0(L_ERROR, " Sending HoFromEutraPrepReq Failed, closing eCSFB transaction ");
                        RETVALUE(WR_UMM_STOP_OLD_TRANSACTION);
                     }
                  }
                  break;
               case WR_UMM_DL_CDMA_RECV_HO_WAIT:
                  {
                     /* After Re-establishment complete, Start the timer again
                      * and wait for DL S1 CDMA2000 tunelling message */

                     ecsfbTransCb->ecsfbState = WR_UMM_DL_CDMA_RECV_HO_WAIT;
                     wrStartTransTmr(transCb, WR_TMR_DL_CDMA_RECV_HO_WAIT_TMR,
                           WR_TMR_DL_CDMA_RECV_HO_WAIT_TMR_VAL);
                  }
                  break;
				/*RRC Rel10 Upgrade - start*/
               case WR_UMM_MEAS_CFGD:
				  {
					  if(TRUE == sndDualRxTxRedirectInd)
					  {
						  if(RFAILED != wrUmmSndHoFrmEutraPrepReq(ueCb, defTransCb, TRUE))
						  {
							  RLOG0(L_INFO, "wrUmmEcsfbPrcDeferTrans:HoFrmEutraPrepReq"
							  " sent with dualRxTxRedirectIndicator-r10\n");
	   						  wrIncKpiCsfbSucc(CDMA2000_1XRTT,CDMA2000_CSFB_DUALRXTX_REDIR);
							  wrUmmTransComplete(transCb);
							  RETVALUE(WR_UMM_STOP_OLD_TRANSACTION);
						  }
						  else
						  {
						      RLOG0(L_ERROR, "wrUmmEcsfbPrcDeferTrans:Sending HoFromEutraPrepReq Failed, "
										 "closing eCSFB transaction ");
							  RETVALUE(WR_UMM_STOP_OLD_TRANSACTION);
						  }
						  break;
						 /*IMPORTANT NOTE: if dualRxTx config is NOT supported then this is a
						   fall through case, hence "break" is inside*/
					  }
				  }
				  /*RRC Rel10 Upgrade - end*/
               case WR_UMM_ECSFB_HO_EXEC:
                  {
                     /* In case of ecsfbTransCb->ecsfbState equals to WR_UMM_MEAS_CFGD
                      * Or WR_UMM_ECSFB_HO_EXEC and eNB received RRC re-establishment
                      * complete from UE, here we will post th internal msg with msgType
                      * WR_UMM_INTMSG_CSFB_ECSFB_TRANS . And close the eCSFB transaction.
                      */
                     if(ROK != wrUmmCreateIntCsfb(cellCb, ueCb, WR_UMM_INTMSG_CSFB_ECSFB_TRANS))
                     {
                        RLOG0(L_ERROR, "wrUmmEcsfbPrcDeferTrans:Failed to create"
                              " internal CSFB transaction\n");
                        RETVALUE(RFAILED);
                     }
                     RETVALUE(WR_UMM_STOP_OLD_TRANSACTION);
                  }
                  break;
               default:
                  RLOG0(L_ERROR, "wrUmmEcsfbPrcDeferTrans: Reached default which error case\n");
                  RETVALUE(RFAILED);
                  break;

            }
            RETVALUE(WR_UMM_RESUME_DEF_TRANSACTION);
         }
   }
   RETVALUE(WR_UMM_TRANS_OK);
} /* wrUmmEcsfbPrcDeferTrans */

/** @brief This function is called when a new Transaction is created when eCSFB 
 * transaction is ongoing. The new transaction is passed to this function
 * It should take the appropriate action the basis of incoming transaction type
 * @details
 *
 *     Function: wrUmmEcsfbNewTransProcessor
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
 *
 */

PUBLIC S16 wrUmmEcsfbNewTransProcessor
(
 WrUmmTransCb                *transCb,
 WrUmmTransCb                *incTrans
 )
{
	WrUmmEcsfbTransCb         *ecsfbTransCb = &transCb->u.ecsfbTransCb;
	WrUmmMsgCause              cause;
	S16                        ret = WR_UMM_TRANS_FAIL;

	TRC2(wrUmmEcsfbNewTransProcessor)
		switch(incTrans->transTyp)
		{
			case WR_UMM_RRC_REESTAB_TRANS:
				{
					/* Stop the ongoing timers based on the State of ecsfb trans,
					 * and move the eCSFB into deferred trans list, to handle after
					 * successful re-establsihment */
					switch(ecsfbTransCb->ecsfbState)
					{
						case WR_UMM_WAIT_AFTER_HOPREP_REQ:
							{
								wrStopTransTmr(transCb, WR_TMR_UL_HO_PREP_WAIT_TMR);
								RLOG0(L_WARNING, "wrUmmEcsfbNewTransProcessor:Deferring "
										"the ongoing eCSFB Transaction in WR_UMM_WAIT_AFTER_HOPREP_REQ " 
										"state due to Re-establishment \n");
								ret = WR_UMM_TRANS_DEFER_ONGOING;
							}
							break;
						case WR_UMM_DL_CDMA_RECV_HO_WAIT:
							{
								wrStopTransTmr(transCb, WR_TMR_DL_CDMA_RECV_HO_WAIT_TMR);
								RLOG0(L_WARNING, "wrUmmEcsfbNewTransProcessor:Deferring "
										"the ongoing eCSFB Transaction in WR_UMM_DL_CDMA_RECV_HO_WAIT " 
										"state due to Re-establishment \n");
								ret = WR_UMM_TRANS_DEFER_ONGOING;
							}
							break;
						case WR_UMM_ECSFB_HO_EXEC:
							{
								/* Re-est with handoverFailure Inc the Fail Counter 
								 * check for Cause need to added                   */
								wrIncKpiCsfbFail(CDMA2000_CSFB_WO_PSHO_FAIL_REDIR_SUCC);
								/* In case of Re-establishment with as cause HOFailure or
								 * Other Failure, First stop the WR_TMR_ECSFB_HO_EXEC_TMR 
								 * and move the ongoing eCSFB transaction into deferred
								 * transaction list*/

								wrStopTransTmr(transCb, WR_TMR_ECSFB_HO_EXEC_TMR);
								RLOG0(L_WARNING, "wrUmmEcsfbNewTransProcessor:Deferring "
										"the ongoing eCSFB Transaction in WR_UMM_ECSFB_HO_EXEC " 
										"state due to Re-establishment \n");
								ret = WR_UMM_TRANS_DEFER_ONGOING;
							}
							break;
						case WR_UMM_DAT_CFM_WAIT_DL_CDMA_INFO:
							{
								wrStopTransTmr(transCb, WR_TMR_RRC_DAT_CFM);
								RLOG0(L_WARNING, "wrUmmEcsfbNewTransProcessor:Deferring "
										"the ongoing eCSFB Transaction in WR_UMM_DAT_CFM_WAIT_DL_CDMA_INFO " 
										"state due to Re-establishment \n");
								ret = WR_UMM_TRANS_DEFER_ONGOING;
							}
							break;
						case WR_UMM_MEAS_CFGD:
							{
								/* If Tmr is wait for meas tmr is running,then stop the timer
								 * and move the eCSFB transaction into deffred trans */
								wrStopTransTmr(transCb, WR_TMR_ECSFB_MEAS_RPT);
								wrIncKpiCsfbFail(CDMA2000_CSFB_WO_PSHO_FAIL_REDIR_SUCC);
								RLOG0(L_WARNING, "wrUmmEcsfbNewTransProcessor:Deferring "
										"the ongoing eCSFB Transaction in WR_UMM_MEAS_CFGD " 
										"state due to Re-establishment \n");
								ret = WR_UMM_TRANS_DEFER_ONGOING;
							}
							break;
						default:
							break;
					}
				}
				break;
			case WR_UMM_ERAB_SETUP_TRANS:
			case WR_UMM_ERAB_MOD_TRANS:
			case WR_UMM_RAB_REL_TRANS:
				{
					cause.causeTyp = CAUSE_RADIONW;
					cause.causeVal = SztCauseRadioNws1_inter_system_handover_triggeredEnum;
					if(ROK == wrUmmHdlAndPrcRabMsg(incTrans,&cause))
					{
						ret = WR_UMM_TRANS_FAIL;
					}
				}
				break;
			case WR_UMM_MEAS_TRANS:
				{
					RLOG0(L_WARNING, "wrUmmEcsfbNewTransProcessor Measurement report"
							"is not handled, as eCSFB is ongoing \n");
					ret = WR_UMM_TRANS_FAIL;
				}
				break;
			default:
				{
					ALARM("Transaction %d received at wrUmmEcsfbNewTransProcessor\n",
							incTrans->transTyp);
					ret = WR_UMM_TRANS_OK;
				}
				break;
		}
	RETVALUE(ret);
} /* wrUmmEcsfbNewTransProcessor */
/*RRC Rel10 Upgrade - start*/
/*Function will fill DLInfoTransfer message for CDMA and send to UE */
/** @brief This function will handle S1 DL tunneling message when HO status
 *  is Failure. It will forward the CDMA Pdu to UE through DLInfoTransfer
 *  message
 *
 * @details
 *
 *     Function: wrUmmEcsfbSndDlNas
 *
 *         Processing steps:
 *         - It forwards the CDMA Pdu to UE
 *         - Set the state to WR_UMM_DAT_CFM_WAIT_DL_CDMA_INFO
 *
 * @param[in]  transCb   : transaction inforamation
 * @param[in]  cdmaS1Pdu : cdma Pdu recevied from MME
 * @param[in]  cdmType   : tyep of CDMA 
 * @return S16
 *    -#Success : ROK
 */
PRIVATE S16 wrUmmEcsfbSndDlNas
(
 WrUmmTransCb        *transCb,
 TknStrOSXL          *cdmaS1Pdu,
 U32                 cdmaType
)
{
   WrUmmEcsfbTransCb    *ecsfbTransCb = &transCb->u.ecsfbTransCb;
   /*This function will Build and send the Dl CDMA Info message to UE
    * it will start DAT CFM timer also */
   if( ROK != wrUmmBuildAndSndCdmaDlInfTfr(transCb, cdmaS1Pdu, 
                                              cdmaType))
   {
      RETVALUE(RFAILED);
   }
   ecsfbTransCb->ecsfbState = WR_UMM_DAT_CFM_WAIT_DL_CDMA_INFO;
   ecsfbTransCb->numOfDlTxCnt++;
   wrIncKpiCsfbSucc(CDMA2000_1XRTT,CDMA2000_CSFB_DUALRXTX_DLNAS);
   RETVALUE(ROK);
}
/*RRC Rel10 Upgrade - end*/
