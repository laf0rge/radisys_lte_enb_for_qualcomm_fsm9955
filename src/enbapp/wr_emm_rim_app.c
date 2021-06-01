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
               

     File:     fc_emm_rim_app.c

     Sid:      fc_emm_rim_app.c@@/main/Br_Tenb_Rim_Csfb_Intg/8 - Sun Nov  2 23:17:14 2014

     Prg:      jkaur 

*********************************************************************21*/

static const char* RLOG_MODULE_NAME="EMM";
static int RLOG_MODULE_ID=1;
static int RLOG_FILE_ID=125;

#include "wr.h"
#include "wr_emm.h"
#include "wr_utils.h"
#include "wr_emm_rim.h"
#include "wr_emm_rim_app.h"
#include "wr_emm_mme.h"
#include "wr_ifm_s1ap.h"

/** @brief This function handles the RIM configuration request received 
 * from RIM User 
 *
 * @details
 *
 *     Function: wrEmmRimCfgReq
 *
 *     Processing steps:
 *     - Update the function pointer for start and stop the timer
 *     - Call the RIM user API to configure the RIM parameters
 *
 * @param[in]  WrRimCfg :
 * @param[out] 
 * @return S16
 */
PUBLIC S16 wrEmmRimCfgReq
(
 WrRimCfg                    *rimCfg
 )
{
   WrCellCb                  *cellCb = wrEmmCb.cellCb[0]; /* EMM cell cb */


   rimCfg->rimStartTmr = wrStartTmr;
   rimCfg->rimStopTmr  = wrStopTmr;

   /* Fill the Self address */
   rimCfg->selfAddr.ratChoice               = WR_RIM_EUTRA_RAT;
   rimCfg->selfAddr.t.eutraIdentity.enbId   = wrEmmCb.enbId;
   rimCfg->selfAddr.t.eutraIdentity.enbType = wrEmmCb.enbType;
   rimCfg->selfAddr.t.eutraIdentity.tac     = cellCb->sib1.tac;
   wrUtlCopyPlmnId(&(cellCb->sib1.plmns[0].plmnId),
         &(rimCfg->selfAddr.t.eutraIdentity.plmnId));

   if(ROK != wrRimCfgReq(rimCfg))
   {
      RLOG0(L_ERROR, " wrEmmRimCfgReq: RimCfgReq is Failed ");
      RETVALUE(RFAILED);
   }

   RETVALUE(ROK);
} /* end of wrEmmRimCfgReq */ 

/** @brief This function gets the MMECb based on the plmnId
 *
 * @details
 *
 *     Function: wrEmmMmeGetValidMmeByPlmn
 *
 *     Processing steps:
 *     - Traverse the list of connected MMEs by EnodeB with Primary PLMN,
 *     - Check the state of selected MME, if not up then again traverse
 *       the list of connected MMEs by EnodeB
 *     - return the MME CB
 *     - else
 *     - return NULLP
 *
 * @param[in] plmn : Primary PLMN for which MME CB needs to be fetched
 * @return  S16
 *        -# mmeCb : MME Control Block
 *        -# NULLP : Failure
 */
PRIVATE WrMmeCb *wrEmmMmeGetValidMmeByPlmn
(
 WrPlmnId                    *plmnId
 )
{
   WrMmeCb                   *mmeCb;
   WrPlmnCbMmeList           mmeList;
   U32                       mmeIdx;

   if ( ROK != wrEmmPlmnGetMmeIdList(&mmeList, *plmnId))
   {
      RLOG0(L_ERROR, "wrEmmMmeGetValidMmeByPlmn: \
                      wrEmmPlmnGetMmeIdList() failed");
      RETVALUE(NULLP);
   }

   for(mmeIdx = 0; mmeIdx < mmeList.numOfMme; mmeIdx++)
   {
      /* Get the MME control block */
      mmeCb = WR_EMM_MME_GET_MME(mmeList.mmeIdList[mmeIdx]);
      if ( WR_MME_UP == mmeCb->state)
      {
         RETVALUE(mmeCb);
      }
   }                  
   RETVALUE(NULLP);
} /* end of wrEmmMmeGetValidMmeByPlmn */


/** @brief This function used to build the ENB DIRECT INFORMATION 
 * TRANSFER message using the rimPDU and destination address. 
 *
 * @details
 *
 *     Function: wrEmmBldEnbDirectInfoTransfer
 *
 *     Processing steps:
 *     - Allocate the memory of s1ap pdu
 *     - Fill all the below IEs required for ENB Direct Transfer message 
 *     - Inter-system Information Transfer Type 
 *     - RIM Transfer  
 *     - RIM Information 
 *     - RIM Routing Address
 *     - after filling all the IEs pass the constructed PDU back to caller
 *
 * @param[in]   *rimPdu:
 * @param[in]   *dstAddr:
 * @param[out]  **pdu :
 * @return S16
 */
PUBLIC S16 wrEmmBldEnbDirectInfoTransfer
( 
 S1apPdu                     **pdu,
 Buffer                      *rimPdu,
 WrRimRoutingInfo            *dstAddr
)
{
   U16                                    ieIdx = 0;
   U8                                     numComp;
   MsgLen                                 contLen;
   MsgLen                                 msgLen;
   S1apPdu                                *enbDrctPdu = NULLP;
   SztInitiatingMsg                       *initMsg = NULLP;
   SztENBDirectInformTfr                  *eNBDirectInfo = NULLP;
   SztProtIE_Field_ENBDirectInformTfrIEs  *ie = NULLP;
   SztRIMRoutingAddr                      *rimRoutAddr = NULLP;

   TRC2(wrEmmBldEnbDirectInfoTransfer);
   
   /* Initialize memory control point */
   if (cmAllocEvnt(sizeof(S1apPdu), SZ_MEM_SDU_SIZE, &wrCb.mem,
            (Ptr *)&enbDrctPdu) != ROK)
   {
      RLOG0(L_FATAL, "Memory allocation failed.");
      RETVALUE(RFAILED);
   }

   wrFillTknU8(&(enbDrctPdu->pdu.choice), S1AP_PDU_INITIATINGMSG);
   initMsg = &(enbDrctPdu->pdu.val.initiatingMsg);
   wrFillTknU8(&(initMsg->pres), PRSNT_NODEF);
   wrFillTknU32(&(initMsg->procedureCode), Sztid_eNBDirectInformTfr);
   wrFillTknU32(&(initMsg->criticality), SztCriticalityignoreEnum);

   eNBDirectInfo = &(initMsg->value.u.sztENBDirectInformTfr);
   wrFillTknU8(&(eNBDirectInfo->pres), PRSNT_NODEF);

   numComp = 1;
   /* Allocate memory for IE */
   if ((cmGetMem(enbDrctPdu, numComp * 
               sizeof(SztProtIE_Field_ENBDirectInformTfrIEs),
               (Ptr*)&eNBDirectInfo->protocolIEs.member)) !=ROK)
   {
      /* TODO - In failure case, we need to deallocate the allocated memory */
      RLOG0(L_FATAL, "Memory allocation failed.");
      WR_FREEEVNT(enbDrctPdu);
      RETVALUE(RFAILED);
   }

   ie = &eNBDirectInfo->protocolIEs.member[ieIdx];
   wrFillTknU8(&(ie->pres), PRSNT_NODEF);
   wrFillTknU32(&(ie->id), Sztid_Inter_SystemInformTfrTypEDT);
   wrFillTknU32(&(ie->criticality), SztCriticalityrejectEnum);
   wrFillTknU8(&(ie->value.u.sztInter_SystemInformTfrTyp.choice),
         INTER_SYSTEMINFORMTFRTYP_RIMTFR);
   wrFillTknU8(&(ie->value.u.sztInter_SystemInformTfrTyp.val.rIMTfr.
            pres), PRSNT_NODEF);

   /* Now copy the following IEs - 
    * SztRIMInform rIMInform
    * SztRIMRoutingAddr rIMRoutingAddr;
    * SztProtExtnCont_RIMTfr_ExtIEs iE_Extns;
    */
   
   SFndLenMsg(rimPdu, &msgLen);

   ie->value.u.sztInter_SystemInformTfrTyp.val.rIMTfr.rIMInform.pres = PRSNT_NODEF;
   ie->value.u.sztInter_SystemInformTfrTyp.val.rIMTfr.rIMInform.len  = msgLen;

   WR_GET_MEM(enbDrctPdu,((Size) msgLen), 
         &(ie->value.u.sztInter_SystemInformTfrTyp.val.rIMTfr.rIMInform.val));

   SCpyMsgFix(rimPdu, 0, msgLen,
         (Data *)(ie->value.u.sztInter_SystemInformTfrTyp.val.rIMTfr.rIMInform.val), &contLen);

   /* Based on the RAT Choice fill the RIM Routing address */

   rimRoutAddr = &(ie->value.u.sztInter_SystemInformTfrTyp.val.rIMTfr.rIMRoutingAddr);

   switch(dstAddr->ratChoice)
   {
      case WR_RIM_UTRA_RAT:
         {
            wrFillTknU8((&rimRoutAddr->choice), RIMROUTINGADDR_TARGETRNC_ID);
            wrFillTknU8(&(rimRoutAddr->val.targetRNC_ID.pres), PRSNT_NODEF);

            /* fill the RNC (UTRA Cell) LAI, which consist of PLMNID,LAC and iE_Extns */
            wrFillTknU8(&(rimRoutAddr->val.targetRNC_ID.lAI.pres), PRSNT_NODEF);
            wrEmmMmeSztFillPLMNId(enbDrctPdu, &dstAddr->t.rncIdentity.plmnId,
                  &rimRoutAddr->val.targetRNC_ID.lAI.pLMNidentity);
            wrEmmMmeSztFillLAC(dstAddr->t.rncIdentity.lac, &(rimRoutAddr->val.
                     targetRNC_ID.lAI.lAC));
            rimRoutAddr->val.targetRNC_ID.lAI.iE_Extns.noComp.pres = NOTPRSNT;

            /* Fill the UTRA cell RAC */
            rimRoutAddr->val.targetRNC_ID.rAC.pres  = PRSNT_NODEF;
            rimRoutAddr->val.targetRNC_ID.rAC.len   = WR_RAC_IE_LEN;
            rimRoutAddr->val.targetRNC_ID.rAC.val[0] = dstAddr->t.rncIdentity.rac;

            /* Fill the extended RNC ID */
            if ( dstAddr->t.rncIdentity.rncId >= WR_EXT_RNC_START )
            {
               /* Fill the UTRA Cell - RNC ID */
               /* Fill the RNC id to maximum if extended RNC id is present, anyway this will be
                * ignored */
               wrFillTknU32(&(rimRoutAddr->val.targetRNC_ID.rNC_ID), (WR_EXT_RNC_START-1));
               wrFillTknU32(&(rimRoutAddr->val.targetRNC_ID.extendedRNC_ID), dstAddr->t.rncIdentity.rncId);
            }
            else
            {
               /* Fill the UTRA Cell - RNC ID */
               wrFillTknU32(&(rimRoutAddr->val.targetRNC_ID.rNC_ID), dstAddr->t.rncIdentity.rncId);
            }
            /* Fill the UTRA Cell - iE_Extns */
            rimRoutAddr->val.targetRNC_ID.iE_Extns.noComp.pres = NOTPRSNT;
         }
         break;
      case WR_RIM_GERAN_RAT:
         {
            wrFillTknU8((&rimRoutAddr->choice), RIMROUTINGADDR_GERAN_CELL_ID);
            wrFillTknU8(&(rimRoutAddr->val.gERAN_Cell_ID.pres), PRSNT_NODEF);

            /* fill the GERAN Cell LAI - which consist of PLMNID,LAC and iE_Extns */

            wrFillTknU8(&(rimRoutAddr->val.gERAN_Cell_ID.lAI.pres), PRSNT_NODEF);
            wrEmmMmeSztFillPLMNId(enbDrctPdu, &dstAddr->t.geranCellId.plmnId,
                  &rimRoutAddr->val.gERAN_Cell_ID.lAI.pLMNidentity);
            wrEmmMmeSztFillLAC(dstAddr->t.geranCellId.lac, &(rimRoutAddr->val.
                     gERAN_Cell_ID.lAI.lAC));
            rimRoutAddr->val.gERAN_Cell_ID.lAI.iE_Extns.noComp.pres = NOTPRSNT;

            /* Fill the GERAN cell RAC */
            rimRoutAddr->val.gERAN_Cell_ID.rAC.pres  = PRSNT_NODEF;
            rimRoutAddr->val.gERAN_Cell_ID.rAC.len   = WR_RAC_IE_LEN;
            rimRoutAddr->val.gERAN_Cell_ID.rAC.val[0] = dstAddr->t.geranCellId.rac;

            /* Fill the GERAN Cell identity */
            wrEmmMmeSztFillCI(dstAddr->t.geranCellId.ci, &(rimRoutAddr->val.
                     gERAN_Cell_ID.cI));

            /* Fill the GERAN Cell - iE_Extns */
            rimRoutAddr->val.gERAN_Cell_ID.iE_Extns.noComp.pres = NOTPRSNT;
         }
         break;
      default:
         {
            RLOG0(L_ERROR, " wrEmmBldEnbDirectInfoTransfer: Invalid RAT choice \
                  is received from RIM protocol");
            WR_FREEEVNT(enbDrctPdu);
            RETVALUE(RFAILED);
         }
   }
   ieIdx++;
   ie->value.u.sztInter_SystemInformTfrTyp.val.rIMTfr.iE_Extns.noComp.pres = NOTPRSNT;

   /* Fill in the number of IEs included in the message */
   wrFillTknU16(&(eNBDirectInfo->protocolIEs.noComp), ieIdx);

   /* Pass the constructed PDU back to the caller */
   *pdu = enbDrctPdu;

   RETVALUE(ROK);
} /* end of wrEmmBldEnbDirectInfoTransfer */


/** @brief This function is to send the EnodeB Direct Transfer message to
 *         the MME. 
 *
 *     Function: wrEmmSendEnbDirectTrfr
 *
 *         Processing steps:
 *         -  Build config update PDU and dispatch the message to S1AP
 *
 * @param[in] mmeId: MME ID.
 * @return  S16
 *          -# Success : ROK
 *          -# Failure : RFAILED
 */
PRIVATE S16 wrEmmSendEnbDirectTrfr
(
Buffer                       *rimPdu,
WrRimRoutingInfo             *dstAddr
)
{
   SztUDatEvnt               uDatEvnt;
   WrMmeCb                   *mmeCb = NULLP; 


   mmeCb = wrEmmMmeGetValidMmeByPlmn(
         &(wrEmmCb.cellCb[0]->sib1.plmns[0].plmnId));
   if( NULLP == mmeCb)
   {
      RLOG0(L_ERROR, "There is no MME to send the ENB Direct Message Transfer"
            "message ");
      RETVALUE(RFAILED);
   }
   if(ROK != wrEmmBldEnbDirectInfoTransfer(&(uDatEvnt.pdu), rimPdu, dstAddr))
   {
      RLOG0(L_ERROR, "wrEmmSendEnbDirectTrfr:Building of ENB DIRECT TRANSFER"
            "message is Failed ");
      RETVALUE(RFAILED);
   }
   uDatEvnt.transId.pres = PRSNT_NODEF;
   uDatEvnt.transId.val = 1;
   uDatEvnt.peerId.pres = PRSNT_NODEF;
   uDatEvnt.peerId.val = mmeCb->peerId;

   /* Send the pdu to the MME */
   if((WrIfmS1apSndMgmtMsg(&uDatEvnt)) != ROK)
   {
      RLOG0(L_ERROR, "Sending ENB DIRECT INFORMATION TRANSFER Failed");
      RETVALUE(RFAILED);
   }
   else
   {
      RLOG_ARG0(L_EVENT, DBG_MMEID, mmeCb->mmeId,"[S1AP]:Sending ENB DIRECT INFORMATION TRANSFER");
   }
  
   RETVALUE(ROK);
} /* end of wrEmmSendEnbDirectTrfr */


/** @brief This function handles the request from the RIM User 
 * to initiate RAN-INFORMATION-REQUEST procedure
 *
 * @details
 *
 *     Function: wrEmmRimInfoReq
 *
 *     Processing steps:
 *         - Call RIM Protocol API to build the RIR
 *         - RIM Protocol returns the RIM PDU
 *         - On the reception of valid RIM PDU, call API to build ENB direct
 *           information transfer and send towards MME
 *
 * @param[in]  WrRimReqInfo :
 * @param[out] 
 * @return S16
 */
PUBLIC S16 wrEmmRimInfoReq
(
 WrRimReqInfo                *rimReqInfo
)
{

   Buffer                    *rimPdu = NULLP;
   S16                       ret = ROK;

   /* Call Rim Protocol API to build the RIR procedure. RIM Protocol will 
    * allocate the memory for the rimPdu and then reply with RIR PDU*/

   if(ROK != wrRimRirProcReq(rimReqInfo, &rimPdu))
   {
      RLOG0(L_ERROR, " wrEmmRimInfoReq: Processing of RIR request is Failed ");
      RETVALUE(RFAILED);
   }

   /* Call API to build ENB direct transfer message and send towards MME */

   ret = wrEmmSendEnbDirectTrfr(rimPdu, &(rimReqInfo->dstAddr));

   if(ROK != ret)
   {
      RLOG0(L_ERROR, " wrEmmRimInfoReq:Sending of RIR reuqest towards"
                     "MME is Failed ");
   }
   /* Deallocate the memory allocated for rimPdu by RIM protocol */
   WR_FREEMBUF(rimPdu);

   RETVALUE(ret);

} /* end of wrEmmRimInfoReq */ 

/** @brief This function initiates the RIM association abort procedure when
 * request is received from RIM user. Applicable when association with all of
 * the MMEs goes down
 *
 * @details
 *
 *     Function: wrEmmRimMmeDownHdlr
 *
 *     Processing steps:
 *     - Mark the abrtAll flag as TRUE and set rimReqInfo as NULLP
 *     - Call RIM Protocol API to abort all the RIM associations
 *
 * @param[in]  Void :
 * @param[out] 
 * @return S16
 */
PUBLIC S16 wrEmmRimMmeDownHdlr
(
Void
)
{
   WrRimReqInfo              *rimReqInfo = NULLP;
   Bool                      abrtAll = TRUE;

   /* Here, Call RIM Protocol API with abrtAll flag as TRUE and rimReqInfo
    * as NULL,so that RIM protocol will abort all the associations instead
    * of particular association - As all of the MMEs goes down*/
   
   if(ROK != wrRimAbortRirProc(abrtAll, rimReqInfo))
   {
      RLOG0(L_ERROR, " wrEmmRimMmeDownHdlr:RIR Abort is Failed ");
      RETVALUE(RFAILED);
   }

   RETVALUE(ROK);
} /* end of wrEmmRimMmeDownHdlr */ 

/** @brief This function initiates RIM Protocol shut down when request
 * received from the RIM user. Applicable when dynamic IP got changed.
 *
 *
 * @details
 *
 *     Function: wrEmmRimShutdown
 *
 *     Processing steps:
 *     - Call RIM Protocol API to stop all the timers and clear all the 
 *       RIM associations
 *
 *
 * @param[in]  Void :
 * @param[out] 
 * @return S16
 */
PUBLIC S16 wrEmmRimShutdown
(
Void
)
{
   /* Here, Call RIM Protocol API, so that RIM Protocol shall stop all the
    * running RIM timers and deallocate the memory for all the RIM related 
    * parameters and clear all the RIM associations */

   wrRimShutdown();

   RETVALUE(ROK);
} /* end of wrRimShutdown */ 

/** @brief This function initiates the RIM Protocol about expiry of 
 * RIM timers 
 *
 * @details
 *
 *     Function: wrEmmRimPrcTmrExp
 *
 *     Processing steps:
 *     - Copy the control block and event into rimTO then
 *     - Call RIM protocol API to handle the RIM timer expiry
 *     - If isMaxRtxReached is FALSE, then call API to resend the ENB
 *       direct transfer message
 *     - Else call RIM user API for the failure case
 *
 *
 * @param[in]   cb :
 * @param[in]   event :
 * @param[out] 
 * @return S16
 */
PUBLIC S16 wrEmmRimPrcTmrExp
( 
 PTR                         cb, 
 S16                         event
)
{
   WrRimTimeOut              rimTO ; 
   WrRimToAction             timeOutAction ;
   WrRimErrInd               rimErrInd;
   S16                       ret = ROK;

   /* Copy the control block and event into rimTO, then call RIM Protocol API to
    * handle the RIM timer expiry */
   
   rimTO.event = event;
   rimTO.cb    = cb;

   if(ROK != wrRimTimoutHdl(&rimTO, &timeOutAction))
   {
      RLOG0(L_ERROR, " wrEmmRimPrcTmrExp: Handling of timeOut is Failed ");
      RETVALUE(RFAILED);
   }

   /* If isMaxRtxReached is false then call the API to resend the ENB Direct 
    * transfr msg else initimate to RIM user about the failure case */

   if (timeOutAction.isMaxRtxReached != TRUE)
   {
      ret = wrEmmSendEnbDirectTrfr(timeOutAction.rtxPdu, 
            &(timeOutAction.dstAddr));
      if(ROK != ret)
      {
         RLOG0(L_ERROR, "wrEmmRimPrcTmrExp: Building of ENB DIRECT TRANSFER \
               message is Failed ");
      }
      WR_FREEMBUF(timeOutAction.rtxPdu);
   }
   else
   {
      /* Retransmission count reached maximum, so we need to initimate the RIM
       * user about the Tmr Expiry case so that RIM user update the NRT state
       * appropirately */

      rimErrInd.appId = timeOutAction.appId;
      rimErrInd.rptCellId = timeOutAction.rptCellId;
      rimErrInd.errType = WR_EMM_RIM_ERR_TMR_EXP;
      wrEmmRimErrInd(&rimErrInd);
   }
   
   RETVALUE(ret);
} /* end of wrEmmRimPrcTmrExp */

/** @brief This function used to process the RIR PDU received from
 * RIM protocol after decoding 
 *
 * @details
 *
 *     Function: wrEmmPrcRir 
 *
 *     Processing steps:
 *     - Check the RIM application identity
 *     - If RIM application Id is supported at eNodeB, then call RIM 
 *       protocol API to build the RAN-INFORMATION with Application 
 *       error container.
 *     - On the reception of RI PDU from RIM protocol,call the API to 
 *       send the ENB Direct Information Transfer message towards MME
 *     - else call the RIM protocol API to abort the association
 *
 *
 * @param[in]   *rimInfo :
 * @param[out] 
 * @return S16
 */
PRIVATE S16 wrEmmPrcRir
(
 WrRimDecRspInfo             *rimInfo 
)
{

   WrRimAppErr               appErr;
   Buffer                    *riPdu = NULLP;
   WrRimReqInfo              rimReq;
   S16                       ret = ROK;

    /* If RIM appId is NACC/SI3/UTRA SI then call RIM protocol to build the
     * RI with Application error container. Else
     * Discard the received RIR and send request to RIM protocol to abort 
     * the association */

   switch(rimInfo->appId)
   {
      case WR_RIM_APP_UTRA_SI:
      case WR_RIM_APP_NACC:
      case WR_RIM_APP_SI3:
         {
            appErr.appId   = rimInfo->appId;
            appErr.appCont = rimInfo->pdu.rirDecInfo.appCont;
            appErr.dstAddr = rimInfo->dstAddr;
            appErr.srcAddr = rimInfo->srcAddr;
            appErr.rptCellId = rimInfo->rptCellId;

            /* As cause value for NACC/SI3/UTRA SI is same.So we are using the
             * one enum value here */
            appErr.cause = WR_RIM_NACC_OTHRUNSPEC;
            /* Call API to trigger RI with App error container */
            if( ROK != wrRimRiErrReq(&appErr, &riPdu) )
            {
               RLOG0(L_ERROR, "wrEmmPrcRir:Building of RiErrReq failed in \
                     RIM Protocol");
               RETVALUE(RFAILED);
            }
            /* Call API to build eNB direct transfer msg and send towards MME */
            ret = wrEmmSendEnbDirectTrfr(riPdu, &(rimInfo->dstAddr));
            if(ROK != ret)
            {
               RLOG0(L_ERROR, "wrEmmPrcRir: Building of ENB DIRECT \
                             TRANSFER message with ACK PDU is Failed ");
            }
            /* Deallocate the memory allocated for riPdu by RIM protocol */
            WR_FREEMBUF(riPdu);
         }
         break;
      case WR_RIM_APP_MBMS:
         /* Free the application container IE */
         if ( NULLP != rimInfo->pdu.rirDecInfo.appCont.val )
         {
            WR_FREE(rimInfo->pdu.rirDecInfo.appCont.val,
                  rimInfo->pdu.rirDecInfo.appCont.len)
         }
         /* Fall Through */

      case WR_RIM_APP_SON_TRFR:
         {
            RLOG0(L_INFO, "RIM Application Id is not supported at eNB");

            rimReq.appId     = rimInfo->appId;
            rimReq.srcAddr   = rimInfo->srcAddr; 
            rimReq.dstAddr   = rimInfo->dstAddr;
            rimReq.rptCellId = rimInfo->rptCellId;
            /* Now call API to delete the RIM association */
            if(ROK != wrRimAbortRirProc(FALSE, &rimReq))
            {
               RLOG0(L_ERROR, " wrEmmPrcRir: RIR Abort is Failed ");
               RETVALUE(RFAILED);
            }
         }
         break;
      default:
         {
            /* discard the received App ID */
            RLOG0(L_INFO, "wrEmmPrcRir:Invalid RIM Application Id is received");
            ret = RFAILED;
         }
   }
   RETVALUE(ret);
} /* end of wrEmmPrcRir */

/** @brief This function used to process the RI PDU received from
 * RIM protocol after decoding 
 *
 * @details
 *
 *     Function: wrEmmPrcRi
 *
 *     Processing steps:
 *     - Check the RIM application identity
 *     - If RIM App- NACC then validate the length of received SI/PSI
 *     - If it is invalid then call RIM protocol API to trigger RIAE procedure
 *     - On the reception of RIAE PDU from RIM protocol, call API to send the
 *       ENB Direct Information transfer message towards MME
 *     - else pass the recaived SI/PSI towards RIM user to update NRT.
 *     - If RIM App - UTRA SI, without any validation forward towards RIM user
 *
 *
 * @param[in]   *rimInfo:
 * @param[out] 
 * @return S16
 */
PRIVATE S16 wrEmmPrcRi
(
 WrRimDecRspInfo             *rimInfo 
)
{
   U32                       len = 0;
   S16                       ret = ROK;
   Buffer                    *riaePdu = NULLP;
   WrEmmRimRspInfo           riInfo;
   WrRimAppErr               appErr;

   WR_SET_ZERO((U8*)&riInfo, sizeof(WrEmmRimRspInfo));
   /* If riDecInfo contains the appErrCause value then UTRA SI or GERAN SI/PSI
    * will not be present. So check the presence of appErrCause */

   if(PRSNT_NODEF != rimInfo->pdu.riDecInfo.appErrCause.pres)
   {
      switch(rimInfo->appId)
      {
         case WR_RIM_APP_NACC:
            {
               appErr.cause = WR_RIM_NACC_RESVD_CAUSE;
               if (WR_RIM_SI == rimInfo->pdu.riDecInfo.app.
                     nacc.siType)
               {
                  /* Get the exepcted length of received SI by multiplying the
                   * noOfSi with actual length of SI. Then compare with the
                   * received length of SI. If there is mismatch then trigger 
                   * the RIAE procedure else update the NRT */
                  len = rimInfo->pdu.riDecInfo.app.nacc.noOfSi * 
                     WR_EMM_RIM_GERAN_SI_LEN;
               }
               else
               {
                  /* Get the exepcted length of received PSI by multiplying the
                   * noOfSi with actual length of PSI. Then compare with the
                   * received length of PSI. If there is mismatch then trigger
                   * the RIAE procedure else update the NRT */
                  len = rimInfo->pdu.riDecInfo.app.nacc.noOfSi * 
                     WR_EMM_RIM_GERAN_PSI_LEN;
               }

               if (len != rimInfo->pdu.riDecInfo.app.nacc.si.len)
               {
                  appErr.cause   = WR_RIM_NACC_INCONSISTENT_SI_PSI_LEN;
               }
               else if (rimInfo->pdu.riDecInfo.app.nacc.noOfSi > 
                     WR_EMM_RIM_NACC_MAX_SI )
               {
                  appErr.cause   = WR_RIM_NACC_OTHRUNSPEC;
               }

               if ( appErr.cause != WR_RIM_NACC_RESVD_CAUSE )
               {
                  appErr.appId   = rimInfo->appId;
                  appErr.appCont = rimInfo->pdu.riDecInfo.appCont;
                  appErr.dstAddr = rimInfo->dstAddr;
                  appErr.srcAddr = rimInfo->srcAddr;
                  appErr.rptCellId = rimInfo->rptCellId;
                  /* Call API to trigger RIAE procedure */
                  if(ROK != wrRimRiaeProcReq(&appErr, &riaePdu))
                  {
                     RLOG0(L_ERROR, "wrEmmPrcRi: Building of RiErrReq failed \
                           in RIM Protocol");
                     RETVALUE(RFAILED);
                  }
                  /* Call API to build eNB direct transfer message and send 
                   * towards MME */
                  ret = wrEmmSendEnbDirectTrfr(riaePdu, &(rimInfo->dstAddr));
                  if(ROK != ret)
                  {
                     RLOG0(L_ERROR, "wrEmmPrcRi: sending of ENB DIRECT \
                           TRANSFER message with ACK PDU is Failed");
                  }
                  /* Deallocate the memory allocated for riaePdu by RIM protocol */
                  WR_FREEMBUF(riaePdu);
                  /* De-allocate the memory allocated for SI info, as 
                   * it is not being used */
                  if ( NULLP != rimInfo->pdu.riDecInfo.app.nacc.si.val )
                  {
                     WR_FREE(rimInfo->pdu.riDecInfo.app.nacc.si.val, \
                           rimInfo->pdu.riDecInfo.app.nacc.si.len);
                  }
                  RETVALUE(ROK);
               }
            }
            break;
         case WR_RIM_APP_UTRA_SI:
            {
               /* No validation is required for UTRA SI length */
               RLOG0(L_INFO, "wrEmmPrcRi: Validation is not required for \
                     received UTRA SI");
            }
            break;
         default:
            {
               RLOG0(L_ERROR, "wrEmmPrcRi: Invalid RIM Application is received");
               ret = RFAILED;
            }
      }
   }

   /* Allocate the memory for riInfo and then fill the required information
    * and call RIM user API to update the NRT in following cases -
    * - END is received from peer
    * - Validation passed for the NACC application Identity
    * - RIM Application Identity is UTRA SI
    */

   if(ROK == ret)
   {
      riInfo.rptType   = rimInfo->pdu.riDecInfo.rptType;
      riInfo.appId     = rimInfo->appId; 
      riInfo.rptCellId = rimInfo->rptCellId; 

      if (PRSNT_NODEF != rimInfo->pdu.riDecInfo.appErrCause.pres)
      {
         if ( WR_RIM_APP_NACC == riInfo.appId )
         {
            riInfo.app.nacc = rimInfo->pdu.riDecInfo.app.nacc;
         }
         else
         {
            riInfo.app.utraSI = rimInfo->pdu.riDecInfo.app.utraSI;
         }
      }
      else
      {
         riInfo.appErrCause = rimInfo->pdu.riDecInfo.appErrCause;
      }
      /* Call RIM User API to update the NRT with received information */
      wrEmmRimInfoRsp(&riInfo);
   }
   /* Cntrol reached till here, only if Validation of RI App container is succesful
    * hence appContIE can be freed( which is used to send App Error) */
   if ( NULLP != rimInfo->pdu.riDecInfo.appCont.val)
   {
      WR_FREE(rimInfo->pdu.riDecInfo.appCont.val,
            rimInfo->pdu.riDecInfo.appCont.len);
   }
   RETVALUE(ret);
} /* end of wrEmmPrcRi */

/** @brief This function used to process the RI ACK PDU received from
 * RIM protocol after decoding 
 *
 * @details
 *
 *     Function: wrEmmPrcRiAck
 *
 *     Processing steps:
 *     - Check the PDU type for which ACK is received
 *     - If it is for RIAE, fill all the required information and update 
 *       the RIM User 
 *
 * @param[in]   *rimInfo:
 * @param[out] 
 * @return S16
 */
PRIVATE S16 wrEmmPrcRiAck
(
 WrRimDecRspInfo             *rimInfo 
)
{
   WrRimErrInd               rimErrInd;

   switch(rimInfo->pdu.riaDecInfo.pduType)
   {
      case WR_RIM_RIAE_PDU:
         {
            rimErrInd.appId     = rimInfo->appId;
            rimErrInd.errType   = WR_EMM_RIM_ERR_RI_VAL_FAILED;
            rimErrInd.rptCellId = rimInfo->rptCellId;
            wrEmmRimErrInd(&rimErrInd);
         }
         break;
      default:
         {
            /* As per present implementation only RIAE Pdu can receive ACK, as 
             * eNB will behave only as controlling BSS */
            RLOG0(L_INFO, "wrEmmPrcRiAck: ACK is received for invalid RIM PDU");
            RETVALUE(RFAILED);
         }
   }
   RETVALUE(ROK);
} /* end of wrEmmPrcRiAck */

/** @brief This function used to process the RI Error PDU received 
 * from RIM protocol after decoding 
 *
 * @details
 *
 *     Function: wrEmmPrcRiErr
 *
 *     Processing steps:
 *     - Fill the RIM Error cause value along with all the required information
 *     - And update the RIM user 
 *
 * @param[in]   *rimInfo:
 * @param[out] 
 * @return S16
 */
PRIVATE S16 wrEmmPrcRiErr
(
 WrRimDecRspInfo             *rimInfo 
 )
{
   WrRimErrInd               rimErrInd;

   rimErrInd.appId     = rimInfo->appId;
   rimErrInd.errType   = WR_EMM_RIM_ERR_PROT;
   rimErrInd.errCause  = rimInfo->pdu.riErrDecInfo.rimErrCause; 
   rimErrInd.rptCellId = rimInfo->rptCellId;
   /* Call RIM User API to update the NRT, as protocol level error is identified
    * in received information */
   wrEmmRimErrInd(&rimErrInd);

   RLOG0(L_INFO, "wrEmmPrcRiErr: Update the NRT for received \
         RIM error ");
   RETVALUE(ROK);
} /* end of wrEmmPrcRiErr */

/** @brief This function handles MME DIRECT INFORMATION TRANSFER message 
 * received from S1AP.
 *
 * @details
 *
 *     Function: wrEmmRimPrcRimPdu
 *
 *     Processing steps:
 *     - Forward the received RIM PDU towards RIM protocol to decode
 *     - If decoding of RIM PDU is successful then-
 *       - check if ACK is requested. Call API to send ENB direct information
 *         transfer message towards MME.
 *       - If PDU is RAN-INFORMATION-REQUEST, call API to validate.
 *       - If PDU is RAN-INFORMATION, call API to validate the received
 *         information
 *     - Else decoding is failed, call API to send the ENB direct information
 *       transfer message with RAN-INFORMATION-ERROR PDU. And also update the 
 *       RIM user about failure case.
 *
 * @param[in]   peerId :
 * @param[in]   *pdu :
 * @param[out] 
 * @return S16
 */
PUBLIC S16 wrEmmRimPrcRimPdu
( 
 TknStrOSXL                  *rimPdu
)
{

   WrRimDecRspInfo           rimInfo;

   RLOG0(L_EVENT, "[S1AP]:Received MME DIRECT INFORMATION TRANSFER");
   /* Forward the rimPdu to RIM protocol to decode it */
   if(ROK != wrRimPrcRcvdPdu(rimPdu, &rimInfo))
   {
      RLOG0(L_ERROR, "wrEmmRimPrcRimPdu: wrRimPrcRcvdPdu Failed ");
      RETVALUE(RFAILED);
   }
   /* Check whether decoding of received RIM PDU is successful or not */
   if( TRUE == rimInfo.isDecSucc)
   {
      /* Check if Ack is requested or not , if yes then call API to build the 
       * ENB Direct Information Transfer message and send towards MME */
      if(TRUE == rimInfo.isAckRequested)
      {
         if(ROK != wrEmmSendEnbDirectTrfr(rimInfo.rimAckPdu, &(rimInfo.dstAddr)))
         {
            RLOG0(L_ERROR, "wrEmmRimPrcRimPdu: Building of ENB DIRECT TRANSFER \
                  message with ACK PDU is Failed ");
            RETVALUE(RFAILED);
         }
         /* Deallocate the memory allocated for rimAckPdu by RIM protocol */
         WR_FREEMBUF(rimInfo.rimAckPdu);
      }
      /* Check the pdu Type */
      switch(rimInfo.rimPduType)
      {
         case WR_RIM_RIR_PDU:
            {
               if( ROK != wrEmmPrcRir(&rimInfo))
               {
                  RLOG0(L_ERROR, "wrEmmRimPrcRimPdu: validation of RIR \
                        is Failed ");
                  RETVALUE(RFAILED);
               }
            }
            break;
         case WR_RIM_RI_PDU:
            {
               if(ROK != wrEmmPrcRi(&rimInfo))
               {
                  RLOG0(L_ERROR, "wrEmmRimPrcRimPdu: validation of RI \
                        is Failed ");
                  RETVALUE(RFAILED);
               }
            }
            break;
         case WR_RIM_RIA_PDU:
            {      
               if(ROK != wrEmmPrcRiAck(&rimInfo))
               {
                  RLOG0(L_ERROR, "wrEmmRimPrcRimPdu: validation of RI ACK \
                        is Failed ");
                  RETVALUE(RFAILED);
               }
            }
            break;
         case WR_RIM_RIE_PDU:
            {      
               if(ROK != wrEmmPrcRiErr(&rimInfo))
               {
                  RLOG0(L_ERROR, "wrEmmRimPrcRimPdu: validation of RI Error \
                        is Failed ");
                  RETVALUE(RFAILED);
               }
            }
            break;
         default:
            {
               RLOG0(L_ERROR, " wrEmmRimPrcRimPdu: Invalid RIM PDU \
                     is received from RIM protocol");
               RETVALUE(RFAILED);
            }
      }
   }
   else 
   {
      /* Call API to build ENB Direct Information Transfer message with 
       * RAN-INFORMATION ERROR PDU and send towards the MME */
      
      if(NULLP != rimInfo.rimErrPdu)
      {
         if(ROK != wrEmmSendEnbDirectTrfr(rimInfo.rimErrPdu,&(rimInfo.dstAddr)))
         {
            RLOG0(L_ERROR, "wrEmmRimPrcRimPdu: Building of ENB DIRECT\
                  TRANSFER message with RIM Error PDU is Failed ");
            RETVALUE(RFAILED);
         }
         /* Deallocate the memory allocated for rimErrPdu by RIM protocol */
         WR_FREEMBUF(rimInfo.rimErrPdu);
      }
   }
   RETVALUE(ROK);
} /* end of wrEmmRimPrcRimPdu */

/********************************************************************30**

           End of file:     fc_emm_rim_app.c@@/main/Br_Tenb_Rim_Csfb_Intg/8 - Sun Nov  2 23:17:14 2014

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
/main/1        ---    jkaur         1. initial release TotaleNodeB 1.1
*********************************************************************91*/

