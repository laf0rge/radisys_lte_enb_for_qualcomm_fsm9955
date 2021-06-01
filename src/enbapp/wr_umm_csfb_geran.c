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

     Desc:     This file contains CSFB to GERAN  
               

     File:     wr_umm_csfb_geran.c

     Sid:      /main/

     Prg:      Sriky

*********************************************************************21*/

/* header include files (.h) */
static const char* RLOG_MODULE_NAME="UMM";
static int RLOG_FILE_ID=99;
static int RLOG_MODULE_ID=32;

#include "wr.h"
#include "wr_cmn.h"
#include "wr_utils.h"
#include "wr_umm.h"
#include "wr_umm_rrc_msg.h"
#include "wr_umm_trans.h"
#include "wr_ifm_rrc.h"
#include "nhu_asn.x"
#include "wr_umm_meas_fun.h"
#include "wr_umm_meas.h"
#include "wr_umm_csfbupdtcell.h"
#include "wr_kpi.h"
#include "wr_kpiIds.h" 
#include "wr_umm_csfb.h"
#include "wr_umm_csfb_geran.h"
/** @brief This function is used to build an internal message
 *         to trigger CSFB release with redirection transaction 
 * 
 *  @details
 *
 *     Function: wrUmmCsfbGeranBuildSndRedir
 *
 *         Processing steps:
 *         - Create CSFB_REDIR transaction internal message 
 *         - Update LAI into message
 *         - Process the message
 *
 * @param [in] transCb : transation Control block
 * @return S16
 -# Success : ROK
 -# Failure : RFAILED 
 */
PUBLIC S16 wrUmmCsfbGeranBuildSndRedir
(
 WrUmmTransCb     *transCb
 )
{
   Void                      *transMsg = NULLP;
   WrUmmIntCsfbTransMsg      *csfbtransMsg = NULLP;
   WrUmmCsfbCcoTransCb        *csfbGeranTrans = &transCb->u.csfbGeranTrans;
   WrUmmInternalMsg          *internalMsg = NULLP;
   WrUmmIncMsg               *msg = NULLP;
   WrUeCb                    *ueCb = NULLP;
   WrUmmCellCb               *cellCb = NULLP;

   ueCb = transCb->ueCb;

   WR_UMM_GET_CELLCB(cellCb,ueCb->cellId);
   if(NULLP == cellCb)
   {
      RLOG0(L_ERROR,"cellCb not found");
      RETVALUE(RFAILED);
   }

   /*This memory will be freed in wrUmmRlsIncMsg in case of 
    * positive scenarios*/ 
   WR_ALLOC(&csfbtransMsg, sizeof(WrUmmIntCsfbTransMsg));

   if (NULLP == csfbtransMsg)
   {
      RLOG0(L_FATAL, "Memory allocation failed.");
      RETVALUE(RFAILED);
   }

   /*For KPIs*/ 
   csfbtransMsg->ratPriority = GERAN;

   /*Copy Registered LAI*/
   cmMemcpy((U8*)&csfbtransMsg->regLai.plmnId,
         (U8*)&csfbGeranTrans->regLai.plmnId,sizeof(WrPlmnId));
   csfbtransMsg->regLai.lac = csfbGeranTrans->regLai.lac;

   /*Copy cs-Fallback Indicator */
   wrFillTknU32(&(csfbtransMsg->csFallBackInd),
         csfbGeranTrans->csFallBackInd.val); 

   transMsg = (Void *)csfbtransMsg;
   if (ROK != wrUmmBldInternalMsg(&internalMsg, WR_UMM_INTMSG_CSFB_REDIR_TRANS,
            transMsg))
   {
      RLOG0(L_ERROR,"creation of internal message Failed ");
      if(NULLP != csfbtransMsg)
      {
         WR_FREE(csfbtransMsg, sizeof(WrUmmIntCsfbTransMsg));
      }
      RETVALUE(RFAILED);
   }

   WR_ALLOC(&msg,sizeof(WrUmmIncMsg));
   if (NULLP == msg)
   {
      RLOG0(L_FATAL, "Memory allocation failed.");
      if(NULLP != csfbtransMsg)
      {
         WR_FREE(csfbtransMsg, sizeof(WrUmmIntCsfbTransMsg));
      }
      RETVALUE(RFAILED);
   }
   msg->msgTyp = WR_UMM_INTERNAL_MSG;
   msg->u.internalMsg = internalMsg;
   msg->refCnt = 1;

   wrUmmPrcIncMsg(cellCb, ueCb, msg);
   wrUmmRlsIncMsg(&msg);
   RETVALUE(ROK);
}

/** @brief This function is responsible for filling cell change order IE in 
 *         mobility from EUTRA Command message 
 * 
 *  @details
 *
 *     Function: wrUmmCsfbGeranFillCco 
 *
 *         Processing steps:
 *         - Fill NHU cell change order IE
 *
 * @param [in] cellChangeOrder      : NHU cell change order IE 
 * @param [in] geranCell            : Geran Cell Cb
 * @param [in] memCp                : Memory control pointer to allocate memory
 * @param [in] T304expWaitTmrVal    : T304 timer value
 * @return S16
 -# Success : ROK
 -# Failure : RFAILED
 */
PRIVATE S16 wrUmmCsfbGeranFillCco
(
 NhuCellChangeOrder                      *cellChangeOrder,
 WrUmmGeranCellCb                        *geranCell,
 CmMemListCp                             *memCp,
 U32                                     t304IRAT
)
{
   U8                                      bsic = 0x00; /*to store NCC or BCC*/
   U8                                      nco;
   /*U8                                      T304enum;*/
   U32                                     idx;

   wrFillTknU8(&(cellChangeOrder->pres),PRSNT_NODEF);

   /*Below IEs are filled in CCO
    * 1. t304
    * 2. targetRAT-Type*/

   /* Convert T304 IRAT to enum*/
   /*WR_CSFB_GERAN_CONV_T304VAL_TO_ENUM(t304IRAT, T304enum);*/

   /*t304: RSYS OAM is providing enum value for T304IRAT timer value
    * Note: expecting same with the other mode of configurations*/
   wrFillTknU32(&(cellChangeOrder->t304), t304IRAT);

   /*GERAN is the only choice as of now*/
   wrFillTknU8(&(cellChangeOrder->targetRAT_Typ.choice), TARGETRAT_TYP_GERAN);

   /*targetRAT-Type*/
   wrFillTknU8(&(cellChangeOrder->targetRAT_Typ.val.geran.pres),PRSNT_NODEF);

   /*physicalCellIdGERAN*/
   wrFillTknU8(&(cellChangeOrder->targetRAT_Typ.val.geran.physCellId.pres),
         PRSNT_NODEF);

   /*physicalCellIdGERAN - NCC*/
   bsic = WR_GET_NCC_FRM_BSIC(geranCell->bsic);
   /* Shifting 5 bits left as RRC encoder for BSXL string it treats as Big-Endian */ 
   bsic = bsic << 5;
   wrFillTknStrBSXL(&(cellChangeOrder->targetRAT_Typ.val.geran.physCellId.networkColourCode),
         3, &bsic, memCp);

   /*physicalCellIdGERAN - BCC*/
   bsic = WR_GET_BCC_FRM_BSIC(geranCell->bsic);
   /* Shifting 5 bits left as RRC encoder for BSXL string it treats as Big-Endian */ 
   bsic = bsic << 5;
   wrFillTknStrBSXL(&(cellChangeOrder->targetRAT_Typ.val.geran.physCellId.baseStationColourCode),
         3, &bsic, memCp);

   /*Carrier Frequencies GERAN*/
   wrFillTknU8(&(cellChangeOrder->targetRAT_Typ.val.geran.carrierFreq.pres),PRSNT_NODEF);

   /*Carrier Frequencies GERAN - ARFCN*/
   wrFillTknU32(&(cellChangeOrder->targetRAT_Typ.val.geran.carrierFreq.arfcn),
         geranCell->freqCb->arfcn);

   /*Carrier Frequencies GERAN - Band Indicator*/
   if(WR_GERAN_PCS1900 == geranCell->freqCb->bandInd)
   {
      wrFillTknU32(&(cellChangeOrder->targetRAT_Typ.val.geran.carrierFreq.bandIndicator),
            NhuBandIndicatorGERANpcs1900Enum);
   }
   else
   {
      wrFillTknU32(&(cellChangeOrder->targetRAT_Typ.val.geran.carrierFreq.bandIndicator),
            NhuBandIndicatorGERANdcs1800Enum);
   }

   /*Network Control Order*/
   if(PRSNT_NODEF == geranCell->nco.pres)
   {
      nco = (geranCell->nco.val[0]) << 6;
      wrFillTknStrBSXL(&(cellChangeOrder->targetRAT_Typ.val.geran.networkCntrlOrder),
            2, &nco, memCp);
   }

   /*SI or PSI*/
   if(geranCell->noOfsiPsi)
   {
      if(!geranCell->siPsiType) /*0 for SI*/
      {
         wrFillTknU8(&(cellChangeOrder->targetRAT_Typ.val.geran.systemInfo.choice),
               SI_ORPSI_GERAN_SI);
         wrFillTknU16(&(cellChangeOrder->targetRAT_Typ.val.geran.systemInfo.val.si.noComp),
               geranCell->noOfsiPsi);

         WR_GET_MEM(memCp,geranCell->noOfsiPsi*sizeof(NhuSysInfoLstGERANMember),
               (Ptr*)&(cellChangeOrder->targetRAT_Typ.val.geran.systemInfo.val.si.member));
         for(idx = 0; idx < geranCell->noOfsiPsi; idx++)
         {
            /*Length is 21 for SI*/
            wrFillTknStrOSXL(&(cellChangeOrder->targetRAT_Typ.val.geran.systemInfo.val.si.member[idx]),
                  21,geranCell->siOrPsi[idx]->val,memCp);
         }
      }
      else /*1 for PSI*/
      {
         wrFillTknU8(&(cellChangeOrder->targetRAT_Typ.val.geran.systemInfo.choice),
               SI_ORPSI_GERAN_PSI);
         wrFillTknU16(&(cellChangeOrder->targetRAT_Typ.val.geran.systemInfo.val.psi.noComp),
               geranCell->noOfsiPsi);
         WR_GET_MEM(memCp,
               geranCell->noOfsiPsi * sizeof(NhuSysInfoLstGERANMember),
               (Ptr*)&(cellChangeOrder->targetRAT_Typ.val.geran.systemInfo.val.psi.member));
         for(idx = 0; idx < geranCell->noOfsiPsi; idx++)
         {
            /*Length is 22 for PSI*/
            wrFillTknStrOSXL(&(cellChangeOrder->targetRAT_Typ.val.geran.systemInfo.val.psi.member[idx]),
                  22,geranCell->siOrPsi[idx]->val,memCp);
         }
      }
      /*Increment CCO with NACC KPI*/
      wrIncKpiCsfbWithSi(GERAN_CSFB_CCO_WITH_NACC);
   }
   RETVALUE(ROK); 
}
/** @brief This function is responsible for filling mobility from EUTRA
 *         Command message and sending to RRC
 * 
 *  @details
 *
 *     Function: wrUmmCsfbGeranBldSendCco 
 *
 *         Processing steps:
 *         - Fill NHU mobility from EUTRA Command IE
 *         - call NHU IFM wrapper function
 *
 * @param [in] geranCell : Geran Cell Cb
 * @param [in] transCb   : transaction control block
 * @return S16
 -# Success : ROK
 -# Failure : RFAILED 
 */
PUBLIC S16 wrUmmCsfbGeranBldSendCco
(
 WrUmmGeranCellCb        *geranCell,
 WrUmmTransCb             *transCb
 )
{
   NhuDatReqSdus                           *mobiltyToEutraCmdMsg;
   WrUeCb                                  *ueCb = transCb->ueCb;
   WrUmmCellCb                             *cellCb = NULLP;
   NhuDL_DCCH_Msg                          *dlMsg = NULLP;
   NhuMobilityFromEUTRACmd                 *mobFrmEutraCmd = NULLP;
   NhuMobilityFromEUTRACmd_r8_IEs          *mobEutraCmdRel8Ie = NULLP;
   NhuMobilityFromEUTRACmd_r9_IEs          *mobEutraCmdRel9Ie = NULLP;
   NhuMobilityFromEUTRACmdcriticalExtnsc1  *critExtnC1 = NULLP;
   NhuCellChangeOrder                      *cellChangeOrder = NULLP;
   U8                                      transId;
   U32                                     T304IratTmrVal = ZERO;
   U32                                     T301TmrVal = ZERO;

   RLOG_ARG0(L_DEBUG, DBG_CRNTI,ueCb->crnti,"Sending Cell Change Order to UE");

   /*For KPI*/
   ueCb->csfbInfo.redirType = GERAN_CSFB_CCO;
   ueCb->csfbInfo.ratType = GERAN;

   WR_UMM_GET_CELLCB(cellCb,ueCb->cellId);
   if(NULLP == cellCb)
   {
      RLOG0(L_ERROR,"cellCb not found");
      RETVALUE(RFAILED);
   }

   /*check for accessStratumRls and fill accordingly*/
   WR_ALLOCEVNT(&mobiltyToEutraCmdMsg, sizeof(NhuDatReqSdus));
   if ( mobiltyToEutraCmdMsg == NULLP )
   {
      RLOG0(L_FATAL,"Memory allocation failed.");
      RETVALUE(RFAILED);
   }

   mobiltyToEutraCmdMsg->hdr.cellId  = ueCb->cellId;
   mobiltyToEutraCmdMsg->hdr.ueId    = ueCb->crnti;
   mobiltyToEutraCmdMsg->hdr.transId = transCb->transId | WR_UMM_CSFB_GERAN_CCO_TRANSID;

   mobiltyToEutraCmdMsg->sdu.isUeCfgPres = FALSE;
   mobiltyToEutraCmdMsg->sdu.ueMsg.isMsgDed = TRUE;
   mobiltyToEutraCmdMsg->sdu.ueMsg.msg.dedMsg.dlDcchMsg.pdcpId = WR_PDCP_ID_SRB1;

   dlMsg = &(mobiltyToEutraCmdMsg->sdu.ueMsg.msg.dedMsg.dlDcchMsg.m.dcchMsg);

   wrFillTknU8(&dlMsg->pres, PRSNT_NODEF);
   wrFillTknU8(&dlMsg->message.choice, DL_DCCH_MSGTYP_C1);
   wrFillTknU8(&dlMsg->message.val.c1.choice, C1_MOBILITYFROMEUTRACMD);

   mobFrmEutraCmd = &(dlMsg->message.val.c1.val.mobilityFromEUTRACmd);

   wrFillTknU8(&mobFrmEutraCmd->pres, PRSNT_NODEF);

   if(wrUmmGetRrcTransId(ueCb,C1_MOBILITYFROMEUTRACMD,
            WR_GET_TRANS_FRM_TRANSID(transCb->transId),&transId) != ROK)
   {
      RLOG_ARG0(L_INFO, DBG_CRNTI,ueCb->crnti,"MAX RRC Transaction already "
            "Reached");
      WR_FREEEVNT(mobiltyToEutraCmdMsg);
      RETVALUE(RFAILED);
   }
   wrFillTknU32(&mobFrmEutraCmd->rrc_TrnsxnId, transId);  
   wrFillTknU8(&mobFrmEutraCmd->criticalExtns.choice, CRITICALEXTNS_C1);

   critExtnC1 = &(mobFrmEutraCmd->criticalExtns.val.c1);
   /*For release 8*/
   if(NhuAccessStratumRlsrel8Enum == ueCb->ueCap->eUTRA_Cap.accessStratumRls.val)
   {
      wrFillTknU8(&critExtnC1->choice, C1_MOBILITYFROMEUTRACMD_R8);
      mobEutraCmdRel8Ie = &(mobFrmEutraCmd->criticalExtns.val.c1.val.
            mobilityFromEUTRACmd_r8);

      wrFillTknU8(&(mobEutraCmdRel8Ie->pres), PRSNT_NODEF);

      /* Fill cs fallback Indicator presence */
      wrFillTknU8(&(mobEutraCmdRel8Ie->cs_FallbackIndicator), TRUE);

      /* Fill the purpose as Cell Change Order */
      wrFillTknU8(&(mobEutraCmdRel8Ie->purpose.choice), PURPOSE_CELLCHANGEORDER);
      cellChangeOrder = &(mobEutraCmdRel8Ie->purpose.val.cellChangeOrder);

   }
   else /*For release 9 and higher*/
   {
      wrFillTknU8(&critExtnC1->choice, C1_MOBILITYFROMEUTRACMD_R9);
      mobEutraCmdRel9Ie = &(mobFrmEutraCmd->criticalExtns.val.c1.val.
            mobilityFromEUTRACmd_r9);

      wrFillTknU8(&(mobEutraCmdRel9Ie->pres), PRSNT_NODEF);

      /* Fill cs fallback Indicator presence */
      wrFillTknU8(&(mobEutraCmdRel9Ie->cs_FallbackIndicator), TRUE);

      /* Fill the purpose as Cell Change Order */
      wrFillTknU8(&(mobEutraCmdRel9Ie->purpose.choice), PURPOSE_CELLCHANGEORDER);

      cellChangeOrder = &(mobEutraCmdRel9Ie->purpose.val.cellChangeOrder);
   }

   /*Fill cell change order IE*/
   if((ROK != wrEmmGetT304IRATTmrVal(ueCb->cellId, &T304IratTmrVal)) ||
         (ROK !=  wrEmmGetT301TmrVal(transCb->ueCb->cellId, &T301TmrVal)))
    {
      WR_FREEEVNT(mobiltyToEutraCmdMsg);
      /* Set transction state to WR_UMM_TRANS_RELEASE_CALL so that this
       * transaction doesn't lay stale.*/
      transCb->state = WR_UMM_TRANS_RELEASE_CALL;
      transCb->errorCause = WR_UMM_CTXT_REL_SW_ERROR;
      RLOG0(L_ERROR,"wrEmmGetT304IRATTmrVal has failed");
      RETVALUE(RFAILED);
   }

   wrUmmCsfbGeranFillCco(cellChangeOrder,geranCell,&mobiltyToEutraCmdMsg->memCp,
        T304IratTmrVal);

   RLOG_ARG0(L_EVENT, DBG_CRNTI,ueCb->crnti,"[OTA] Sending MobilityFromEUTRACommand"
        " for Cell Change Order ");
   if(ROK != wrIfmRrcSndReqToUe(mobiltyToEutraCmdMsg))
   {
      WR_FREEEVNT(mobiltyToEutraCmdMsg);
      /* Set transction state to WR_UMM_TRANS_RELEASE_CALL so that this
       * transaction doesn't lay stale.*/
      transCb->state = WR_UMM_TRANS_RELEASE_CALL;
      transCb->errorCause = WR_UMM_CTXT_REL_SW_ERROR;
      RLOG0(L_ERROR,"wrIfmRrcSndReqToUe failed");
      RETVALUE(RFAILED);
   }

   /* Set CSFB GERAN UE state to CCO_IN_PROGRESS*/
   ueCb->csfbInfo.csfbState = WR_CSFB_GSM_UE_STATE_CCO_IN_PROGRESS;

   /*If target cell is capable of DTM then set
    * ueCb->csfbInfo.NwDTMCap to TRUE else FALSE.
    * ueCb->csfbInfo.ueDTMCap will decide the cause in
    * UE CONTEXT RELEASE REQUEST*/
   if(TRUE == geranCell->isDtmCapable)
   {
      /*cause: CSFB Triggered*/
      ueCb->csfbInfo.nwDTMCap = TRUE;
   }
   else
   {
      /*cause: UE Not Available for PS Services*/
      ueCb->csfbInfo.nwDTMCap = FALSE;
   }

   /*Start T304expWaitTmr*/
   RLOG_ARG2(L_INFO,DBG_CRNTI,transCb->ueCb->crnti,
               "Starting T304expWaitTmr timer(%d), T301 timer(%d)", 
               cellCb->csfbToGeranCfg.T304expWaitTmrVal, T301TmrVal);   
   wrStartTransTmr(transCb, WR_TMR_CSFB_GSM_T304_EXP_WAIT,
            (cellCb->csfbToGeranCfg.T304expWaitTmrVal + T301TmrVal));

   RETVALUE(ROK);
}
/** @brief This function is responsible for checking if 
 *         both the configured bands are equal.
 * 
 *  @details
 *
 *     Function: wrUmmChkGeranBandindEql
 *
 *         Processing steps:
 *         - Check if both the configured band and 
 *           RRC band are same. 
 *
 * @param [in] cfgdBand : App configured bandInd enum
 * @param [in] nhuBand  : RRC enum
 * @return Bool
 -# If equal     : TRUE
 -# If NOT equal : FALSE 
 */
PUBLIC Bool wrUmmChkGeranBandindEql
(
 U8   cfgdBand,
 U32  nhuBand
 )
{
   if((((WR_GERAN_GSM850 == cfgdBand) ||
               (WR_GERAN_GSM900 == cfgdBand) ||
               (WR_GERAN_DCS1800 == cfgdBand)) && 
            ((NhuBandIndicatorGERANdcs1800Enum == nhuBand))))
   {
      RETVALUE(TRUE);
   }
   else if((WR_GERAN_PCS1900 == cfgdBand) && 
         (NhuBandIndicatorGERANpcs1900Enum == nhuBand))
   {
      RETVALUE(TRUE);
   }
   else
   {
      RETVALUE(FALSE);
   }
}
/** @brief This function is reponsible for freeing the 
 *        conifugred measurement object for GERAN
 * 
 *  @details
 *
 *     Function: wrUmmCsfbGeranFreeMeasObj
 *
 *         Processing steps:
 *         - FREE the memory allocated for measObj 
 *
 * @param [in] transCb : transation Control block
 * @return Void
 */
PRIVATE Void wrUmmCsfbGeranFreeMeasObj
(
 WrUeCb         *ueCb,
 U8              measObjId
 )
{
   CmLListCp                   *cp = NULLP;
   CmLList                     *node = NULLP;
   WrMeasUeCbGeranMeasObjNode  *geranObj = NULLP;

   cp = &ueCb->measCfg->geranMeasObjLst;
   node = cp->first;
   while(NULLP != node)
   {
      /* Note: this API can be used to free any GERAN Measurement Object
       */
      geranObj = (WrMeasUeCbGeranMeasObjNode *)(node->node);
      if(geranObj->measObjId == measObjId)
      {
         cmLListDelFrm(cp, node);
         WR_FREE(node->node, sizeof(WrMeasUeCbGeranMeasObjNode));
         /* Geran has only one object as of now.
          * When ever requested measobject is deleted, loop should be exit */
         break;
      }
      node = node->next;
   }
   RETVOID;
}
/** @brief This function will be invoked for CCO with/without
 *         measurements.
 * 
 *  @details
 *
 *     Function: wrUmmCsfbGeranSendCco
 *
 *         Processing steps:
 *         - In case of CCO without measurements, pick the
 *           neighbour matching registered LAI else pick the 
 *           first neighbour 
 *         - call wrUmmCsfbGeranBldSendCco
 *
 * @param [in] transCb : transation Control block
 * @return S16
 -# Success : ROK
 -# Failure : RFAILED 
 */
PUBLIC S16 wrUmmCsfbGeranSendCco
(
 WrUmmTransCb *transCb
 )
{
   WrUmmCsfbCcoTransCb       *csfbTransCb = &transCb->u.csfbGeranTrans;
   WrUmmGeranCellCb          *cellNode = NULLP;
   WrUeCb                    *ueCb = transCb->ueCb;
   U32                       idx;
   /* preferrabily same as registered LAI else select the first node  */
   for(idx = 0; idx < csfbTransCb->cfgdNeighCnt; idx++)
   {
      cellNode = csfbTransCb->measObjCfgdNeig[idx];
      if((TRUE == wrEmmPlmnPlmnsEqual(&cellNode->plmnId,
                  &ueCb->regLai.plmnId)) && 
            (cellNode->lac == ueCb->regLai.lac))
      {
         break;
      }
   }
   if(idx >= csfbTransCb->cfgdNeighCnt)
   {
      idx = 0;
      cellNode = csfbTransCb->measObjCfgdNeig[idx];
   }

   wrUmmCsfbGeranBldSendCco(cellNode,transCb);
   RETVALUE(ROK);
}
/** @brief This function is responsible for handling the GERAN
 *         measurement sent by UE before CCO 
 * 
 *  @details
 *
 *     Function: wrUmmCsfbGeranHdlMeasRpt
 *
 *         Processing steps:
 *         - Return ROK for empty measurement report 
 *         - For non-empty report 
 *              - Stop the WR_TMR_CSFB_GSM_MEAS_WAIT
 *                timer.
 *              - Set TRANS and UE states to MEAS_RPT_RCVD
 *              - Check for the neighbour index in measObjCfgdNeig by checking
 *              i.  BCCH ARFCN
 *              ii. BandIndicator
 *              iii.NCC -
 *                       |- PhysicalCellIdGERAN
 *              iv. BCC -
 *              - Call wrUmmCsfbGeranBldSendCco with the cell matching the index
 *                found in the previous step
 *
 * @param [in] transCb : transation Control block
 * @param [in] measRpt : GERAN mesurement report
 * @return S16
 -# Success : ROK
 -# Failure : RFAILED 
 */
PUBLIC S16 wrUmmCsfbGeranHdlMeasRpt
(
 WrUmmTransCb                *transCb,
 NhuMeasResults              *measRpt
 )
{
   NhuMeasResultLstGERAN    *geranMeasRes = NULLP;
   NhuMeasResultGERAN       *member = NULLP;
   WrUmmCsfbCcoTransCb      *geranTrans = NULLP;
   WrUmmGeranCellCb         *cellNode = NULLP;
   U8                       idx;
   U32                      idx1;
   S8                       regLaiIdx = -1;
   S8                       csDmnLaiIdx = -1;
   U8                       bsic = 0x00;
   U8                       ncc = 0x00;
   U8                       bcc = 0x00;

   geranMeasRes = &measRpt->measResultNeighCells.val.measResultLstGERAN;
   member = geranMeasRes->member;
   geranTrans = &transCb->u.csfbGeranTrans;

   RLOG_ARG0(L_DEBUG, DBG_CRNTI, transCb->ueCb->crnti,
         "GERAN measurement report received");

   if(measRpt->measResultNeighCells.choice.val != MEASRESULTNEIGHCELLS_MEASRESULTLSTGERAN)
   {
      RLOG0(L_ERROR, "invalid measurement report received");
      RETVALUE(RFAILED);
   }

   /*stop GERANMeasWaitTmr timer on receving non-empty measurement report*/
   RLOG_ARG0(L_DEBUG,DBG_CRNTI,transCb->ueCb->crnti,
         "Stopping GERANMeasWaitTmr timer");
   wrStopTransTmr(transCb, WR_TMR_CSFB_GSM_MEAS_WAIT);

   for(idx = 0; ((regLaiIdx == -1) && (idx < geranMeasRes->noComp.val) && (idx < LWR_MAX_NUM_NEIGH_GERAN_CELLS)); idx++)
   {
      ncc = *member[idx].physCellId.networkColourCode.val;
      bcc = *member[idx].physCellId.baseStationColourCode.val;
      /* ASN Decoder gives the bit strings in big-endian, make them little
       * endian. ncc and bcc are three bit length */
      ncc >>= 5;
      bcc >>= 5;

      for(idx1 = 0; idx1 < geranTrans->cfgdNeighCnt; idx1++) 
      {
         bsic = geranTrans->measObjCfgdNeig[idx1]->bsic;
         if((geranTrans->measObjCfgdNeig[idx1]->freqCb->arfcn == 
                  member[idx].carrierFreq.arfcn.val) && 
               (TRUE == wrUmmChkGeranBandindEql(geranTrans->measObjCfgdNeig[idx1]->freqCb->bandInd,
                                                member[idx].carrierFreq.bandIndicator.val)) && 
               (WR_GET_NCC_FRM_BSIC(bsic) == ncc) && (WR_GET_BCC_FRM_BSIC(bsic) == bcc))
         {
            if(csDmnLaiIdx == -1)
            {
               csDmnLaiIdx = idx1;
            }

            if((TRUE == 
                     wrEmmPlmnPlmnsEqual(&geranTrans->measObjCfgdNeig[idx1]->plmnId,
                        &transCb->ueCb->regLai.plmnId)) && 
                  (geranTrans->measObjCfgdNeig[idx1]->lac == transCb->ueCb->regLai.lac))
            {
               regLaiIdx = idx1;
               break;
            }
         }
      }
   }/* for */

   if(regLaiIdx != -1)
   {
      idx = regLaiIdx;
   }
   else if(csDmnLaiIdx != -1)
   {
      idx = csDmnLaiIdx;
   }
   else
   {
      idx = 0xFF;
   }
   if((idx != 0xFF) && (idx < LWR_MAX_NUM_NEIGH_GERAN_CELLS))
   {
      RLOG0(L_DEBUG,"GERAN neighbour from MeasRpt found");
      cellNode = geranTrans->measObjCfgdNeig[idx];
      if(RFAILED == wrUmmCsfbGeranBldSendCco(cellNode, transCb))
      {
         RLOG0(L_ERROR,"Sending CCO failed");
         RETVALUE(RFAILED);
      }
      else
      {
         RLOG0(L_INFO,"CCO sent to UE");
         RETVALUE(ROK);
      }
   }

   RETVALUE(RFAILED);
}
/** @brief This function is used to fill cellInfoLstGERAN in
 *         RRC Redirection information
 * 
 *  @details
 *
 *     Function: wrFillCsfbGeranCellInfoLst
 *
 *         Processing steps:
 *         - Fill NHU structure from WR structure
 *         - Allocate memory for members
 *
 * @param [in] nhuDatReqSdu : NHU Datreq SDU 
 * @param [in] cellInfo     : cellinformation to be filled
 * @param [in] bandInd      : band indicator
 * @param [out] member       : NhuCellInfoGERAN_r9 member
 * @return S16
 -# Success : ROK
 -# Failure : RFAILED 
 */
PRIVATE S16 wrFillCsfbGeranCellInfoLst
(
 NhuDatReqSdus            *nhuDatReqSdu,
 NhuCellInfoGERAN_r9      *member,
 WrCellInfoListGeran      cellInfo,
 U8                       bandInd
 )
{
   U8       bsic = 0x00;/*To store NCC or BCC*/
   U32      idx;
   wrFillTknU8(&member->pres,PRSNT_NODEF);

   /*Fill Physical Cell Id GERAN*/
   wrFillTknU8(&member->physCellId_r9.pres,PRSNT_NODEF);

   /*physicalCellIdGERAN - NCC*/
   bsic = WR_GET_NCC_FRM_BSIC(cellInfo.bsic);
   /* Shifting 5 bits left as RRC encoder for BSXL string it treats as Big-Endian */ 
   bsic = bsic << 5;
   wrFillTknStrBSXL(&(member->physCellId_r9.networkColourCode),
         3, &bsic, &nhuDatReqSdu->memCp);

   /*physicalCellIdGERAN - BCC*/
   bsic = WR_GET_BCC_FRM_BSIC(cellInfo.bsic);
   /* Shifting 5 bits left as RRC encoder for BSXL string it treats as Big-Endian */ 
   bsic = bsic << 5;
   wrFillTknStrBSXL(&(member->physCellId_r9.baseStationColourCode),
         3, &bsic, &nhuDatReqSdu->memCp);

   /*Fill Carrier Freq GERAN*/
   wrFillTknU8(&(member->carrierFreq_r9.pres),PRSNT_NODEF);

   /*Carrier Frequencies GERAN - ARFCN*/
   wrFillTknU32(&(member->carrierFreq_r9.arfcn),
         cellInfo.arfcn);

   /*Carrier Frequencies GERAN - Band Indicator*/
   if(WR_GERAN_PCS1900 == bandInd)
   {
      wrFillTknU32(&(member->carrierFreq_r9.bandIndicator),
            NhuBandIndicatorGERANpcs1900Enum);
   }
   else
   {
      wrFillTknU32(&(member->carrierFreq_r9.bandIndicator),
            NhuBandIndicatorGERANdcs1800Enum);
   }

   wrFillTknU16(&(member->systemInfo_r9.noComp),cellInfo.noOfsiPsi);

   WR_GET_MEM(&nhuDatReqSdu->memCp,
         cellInfo.noOfsiPsi*sizeof(NhuSysInfoLstGERANMember),
         (Ptr*)&(member->systemInfo_r9.member));

   for(idx = 0; idx < cellInfo.noOfsiPsi; idx++)
   {
      /*Length is 21 for SI*/
      wrFillTknStrOSXL(&(member->systemInfo_r9.member[idx]),
            21,cellInfo.siListGeran[idx]->val,&nhuDatReqSdu->memCp);
   }

   RETVALUE(ROK);
}


/**
 *  @brief This function fills the redirection information of GERAN neighbours
 *        from UMM structure to NHU structure
 *
 *      Function: wrUmmFillNhuGeranRedirectInfo
 *
 *          Processing steps:
 *          - Copy Redirection information from UMM to NHU 
 *
 *  @param[in]  redirectInfo   : Redirection data to be filled to NHU
 *  @param[out] redirInfo      : NHU Redirection data to be sent to UE
 *  @return ROK
 */
PUBLIC S16 wrUmmFillNhuGeranRedirectInfo
(
 WrUmmRedirectInfo          *redirectInfo,
 NhuRRCConRls_r8_IEs         *conRlsR8,
 NhuDatReqSdus                *nhuDatReqSdu,
 U8                           accessStratumRls
 )
{
   U32                         arfcnCnt = 0;
   U32                         cellInfoCnt = redirectInfo->val.geranInfo.cellInfoCnt;
   U32                         celInfoSiCnt = 0;
   U32                         idx = 0;
   U32                         idx1 = 0;
   NhuRedirectedCarrierInfo    *redirInfo = NULLP;
   NhuRRCConRls_v890_IEs       *nonCritExtn_v890 = NULLP;
   NhuRRCConRls_v920_IEs       *nonCritExtn_v920 = NULLP;
   NhuRRCConRls_v920_IEscellInfoLst_r9 *cellInfoLst_r9 = NULLP;
   NhuCellInfoLstGERAN_r9              *geran_r9 = NULLP;
   redirInfo = &conRlsR8->redirectedCarrierInfo;

   wrFillTknU8(&redirInfo->val.geran.pres,PRSNT_NODEF);
   /* Fill starting ARFCN*/
   if(redirectInfo->val.geranInfo.cellInfoList[arfcnCnt].noOfsiPsi)
   {
      celInfoSiCnt++;
   }
   wrFillTknU32(&redirInfo->val.geran.startingARFCN,
         redirectInfo->val.geranInfo.cellInfoList[arfcnCnt++].arfcn);

   /*Fill band indicator*/
   if(WR_GERAN_PCS1900 == redirectInfo->val.geranInfo.bandInd)
   {
      wrFillTknU32(&redirInfo->val.geran.bandIndicator,NhuBandIndicatorGERANpcs1900Enum);
   }
   else
   {
      wrFillTknU32(&redirInfo->val.geran.bandIndicator,NhuBandIndicatorGERANdcs1800Enum);
   }

   /*Fill Explicit list of ARFCNs*/
   wrFillTknU8(&redirInfo->val.geran.followingARFCNs.choice,
         FOLLOWINGARFCNS_EXPLICITLSTOFARFCNS);
   /*Number of components can be zero. */
   wrFillTknU16(&redirInfo->val.geran.followingARFCNs.val.explicitLstOfARFCNs.noComp,
         --cellInfoCnt);

   if(cellInfoCnt)
   {
      WR_GET_MEM(nhuDatReqSdu,cellInfoCnt * sizeof(NhuARFCN_ValueGERAN),
            &redirInfo->val.geran.followingARFCNs.val.explicitLstOfARFCNs.member);

      while(((WR_UMM_MAX_GERAN_ARFCN-1) > (arfcnCnt-1)) && cellInfoCnt--)
      {
         wrFillTknU32(&redirInfo->val.geran.followingARFCNs.val.explicitLstOfARFCNs.member[arfcnCnt-1]
               ,redirectInfo->val.geranInfo.cellInfoList[arfcnCnt].arfcn);
         if(redirectInfo->val.geranInfo.cellInfoList[arfcnCnt].noOfsiPsi)
         {
            celInfoSiCnt++;
         }
         arfcnCnt++;
      }
   }

   /*If UE is rel 9 or higher and if ONLY SIs are present, fill CellInfoListGERAN-r9*/
   if(celInfoSiCnt)
   {
      /*Re-initializing count to previously initialzed count because it is used
       * as a decrementing counter while filling explicitLstOfARFCNs and will be
       * set to 0*/
      cellInfoCnt = redirectInfo->val.geranInfo.cellInfoCnt;

      nonCritExtn_v890 = &conRlsR8->nonCriticalExtn;
      wrFillTknU8(&nonCritExtn_v890->pres,PRSNT_NODEF);

      nonCritExtn_v920 = &nonCritExtn_v890->nonCriticalExtn;
      wrFillTknU8(&nonCritExtn_v920->pres,PRSNT_NODEF);

      cellInfoLst_r9 = &nonCritExtn_v920->cellInfoLst_r9;
      wrFillTknU8(&cellInfoLst_r9->choice,CELLINFOLST_R9_GERAN_R9);

      geran_r9 = &cellInfoLst_r9->val.geran_r9;

      /*Increment for Redirection with SI*/
      wrIncKpiCsfbWithSi(GERAN_CSFB_WITH_REDIRECTION_WITH_SI);

      wrFillTknU16(&geran_r9->noComp,celInfoSiCnt);

      WR_GET_MEM(nhuDatReqSdu,celInfoSiCnt * sizeof(NhuCellInfoGERAN_r9)
            ,&geran_r9->member);

      for(idx = 0; idx < cellInfoCnt; idx++)
      {
         /*If no of SIs(NOT PSIs) is more than 0 */
         if(redirectInfo->val.geranInfo.cellInfoList[idx].noOfsiPsi)
         {
            wrFillCsfbGeranCellInfoLst(nhuDatReqSdu,&geran_r9->member[idx1]
                  ,redirectInfo->val.geranInfo.cellInfoList[idx],
                  redirectInfo->val.geranInfo.bandInd);
            idx1++;
         }
      }

   }
   RETVALUE(ROK);
}
/** @brief This function is used to prepare list of measurement IDs to be removed
 * 
 *  @details
 *
 *     Function: wrUmmFillGeranMeasIdsRmvLst 
 *
 *         Processing steps:
 *         - Fill measurement IDs to remove list
 *         - Fill measurement object IDs to remove list
 *         - Fill report config IDs to remove list
 *
 * @param [in] transCb : transation Control block
 * @return Void
 */
PRIVATE Void wrUmmFillGeranMeasIdsRmvLst
(
 WrUmmTransCb    *transCb
 )
{
   U8      measObjId;
   U32     idx;
   U32     measIdCnt;
   U32     measObjIdCnt;
   U32     rptCfgIdCnt;

   measIdCnt = transCb->u.csfbGeranTrans.measCfgInfo.addMeasInfo.measIdCnt;
   measObjIdCnt = transCb->u.csfbGeranTrans.measCfgInfo.addMeasInfo.measObjIdCnt;
   rptCfgIdCnt = transCb->u.csfbGeranTrans.measCfgInfo.addMeasInfo.rptCfgIdCnt;

   /*Fill meas ID list*/
   transCb->u.csfbGeranTrans.measCfgInfo.rmvMeasInfo.measIdRmvLstCnt = 
      measIdCnt;
   for(idx = 0;idx < measIdCnt;idx++)
   {
      transCb->u.csfbGeranTrans.measCfgInfo.rmvMeasInfo.measIdRmvLst[idx] =
         transCb->u.csfbGeranTrans.measCfgInfo.addMeasInfo.measIdLst[idx];
   }

   /*Fill meas Object ID list*/
   transCb->u.csfbGeranTrans.measCfgInfo.rmvMeasInfo.measObjIdRmvLstCnt = 
      measObjIdCnt;
   for(idx = 0;idx < measObjIdCnt;idx++)
   {
      transCb->u.csfbGeranTrans.measCfgInfo.rmvMeasInfo.measObjIdRmvLst[idx] =
         transCb->u.csfbGeranTrans.measCfgInfo.addMeasInfo.measObjLst[idx];
      measObjId = transCb->u.csfbGeranTrans.measCfgInfo.rmvMeasInfo.measObjIdRmvLst[idx];
      /* free the memory allocated for GERAN Measurement Object */
      wrUmmCsfbGeranFreeMeasObj(transCb->ueCb,measObjId);
   }

   /*Fill report cfg ID list*/
   transCb->u.csfbGeranTrans.measCfgInfo.rmvMeasInfo.rptCfgRmvLstCnt = 
      rptCfgIdCnt;
   for(idx = 0;idx < rptCfgIdCnt;idx++)
   {
      transCb->u.csfbGeranTrans.measCfgInfo.rmvMeasInfo.rptCfgRmvLst[idx] = 
         transCb->u.csfbGeranTrans.measCfgInfo.addMeasInfo.rptCfgLst[idx];
   }

   RETVOID;
}
/** @brief This function is used to process the message
 *         sent from UE i.e. measurement report or RRC 
 *         Reconfig complete.
 * 
 *  @details
 *
 *     Function: wrUmmCsfbGeranPrcRrcMsg
 *
 *         Processing steps:
 *         - In case of C1_MEASUREMENTREPORT
 *             - Call wrUmmCsfbHdlMeasRpt
 *               If this fails, call wrUmmCsfbGeranBuildSndRedir
 *               and compete the trans
 *         - In case of C1_RRCCONRECFGNCOMPL
 *              - call wrUmmFillGeranMeasIdsRmvLst
 *              - call wrUmmMeasCfgReCfgReqCompHand
 *              - call wrUmmCsfbGeranFreeMeasObj
 *
 * @param [in] transCb : transation Control block
 * @return S16
 -# Success : ROK
 -# Failure : RFAILED 
 */
PUBLIC S16 wrUmmCsfbGeranPrcRrcMsg
(
 WrUmmTransCb                *transCb
 )
{
   NhuUlDcchMsg                          *ulDcch;
   NhuUL_DCCH_MsgTypc1                   *c1Msg;
   NhuMeasurementReport                  *measRpt;
   NhuMeasurementReportcriticalExtnsc1   *measRptC1;
   NhuMeasResults                        *measRes;
   S16                                   ret;

   ulDcch = &transCb->msg->u.uuIndPdu->sdu.m.ulDcchMsg;
   c1Msg = &ulDcch->dcchMsg.message.val.c1;

   switch(c1Msg->choice.val)
   {
      case C1_MEASUREMENTREPORT:
         {
            RLOG0(L_INFO, " GERAN MEASUREMENTREPORT Received");
            measRpt = &c1Msg->val.measurementReport;
            measRptC1 = &measRpt->criticalExtns.val.c1;
            measRes = &measRptC1->val.measurementReport_r8.measResults;
            if(WR_CSFB_GSM_UE_STATE_CCO_IN_PROGRESS != 
                  transCb->ueCb->csfbInfo.csfbState)
            {
               ret = wrUmmCsfbHdlMeasRpt(transCb, measRes);
               if(RFAILED == ret)
               {
                  /*Release with re-direction info*/
                  RLOG0(L_ERROR, "GERAN Measurement report handling failed");
                  wrUmmCsfbGeranBuildSndRedir(transCb);
                  wrUmmTransComplete(transCb);
               }
            }
         }
         break;
      case C1_RRCCONRECFGNCOMPL:
         {
            U8   rrcTransId = c1Msg->val.rrcConRecfgnCompl.rrc_TrnsxnId.val;

            /* Free RRC transaction ID*/
            ret = wrUmmRelRrcTransId(transCb->ueCb, C1_RRCCONRECFGN, rrcTransId);
            if (ret != ROK)
            {
               RLOG0(L_ERROR, "Release of transaction ID failed");
            }
          
             break;
         }
      default:
         {
            RLOG0(L_WARNING, "Currently there is no handling for dcchMsg message received as part of a UU_IND_PDU");
            RETVALUE(RFAILED);
         }
   }
   RETVALUE(ROK);
}


/** @brief This function is responsible for building and sending the 
 *         RRC Connection Release message to UE .
 * 
 *  @details
 *
 *     Function: wrUmmCsfbGeranSndRrcConRelMsg
 *
 *         Processing steps:
 *         - Allocate EVENT SDU
 *         - Fill RRC EVENT header
 *         - Call wrUmmRrcBldConRls
 *            - If ROK then call IFM RRC
 *            - else return FAIL
 *
 * @param [in] transCb : transation Control block
 * @return S16
 -# Success : ROK
 -# Failure : RFAILED 
 */
PUBLIC S16 wrUmmCsfbGeranSndRrcConRelMsg
(
 WrUmmTransCb                *transCb
 )
{
   NhuDatReqSdus             *sdu;
   U32                        accessStratumRls;
   WrUmmCsfbTransCb          *csfbTransCb = &transCb->u.csfbTrans;
   NhuRRCConRls              *rrcConRls = NULLP;
   NhuCarrierFreqsGERAN      *geran = NULLP;
   U32                       arfcnCnt;

   WR_ALLOCEVNT(&sdu, sizeof(*sdu));
   if (NULLP == sdu)
   {
      RLOG0(L_FATAL, "Memory allocation failed for RRC Connection release message.");
      RETVALUE(RFAILED);
   }

   arfcnCnt = csfbTransCb->redirectInfo->val.geranInfo.cellInfoCnt;
   rrcConRls = &sdu->sdu.ueMsg.msg.dedMsg.dlDcchMsg.m.dcchMsg.message.val.c1.val.rrcConRls;
   geran = &rrcConRls->criticalExtns.val.c1.val.rrcConRls_r8.redirectedCarrierInfo.val.geran;   

   WR_GET_MEM(&sdu->memCp,arfcnCnt * sizeof(U32),(PTR*)
         &geran->followingARFCNs.val.explicitLstOfARFCNs.member);
   accessStratumRls = transCb->ueCb->ueCap->eUTRA_Cap.accessStratumRls.val;
   wrUmmFillRrcEvntHdr(&sdu->hdr, transCb->ueCb->cellId, transCb->ueCb->crnti,
         transCb->transId);
   /* 
    *Setting transaction ID for above allocation transaction Id
    * */
   sdu->hdr.transId = transCb->transId | WR_UMM_CSFB_RRC_CON_REL_TRANSID;
   if (wrUmmRrcBldConRls(transCb->ueCb, accessStratumRls, csfbTransCb->redirectInfo, sdu, 
            NhuRlsCauseotherEnum, 0) != ROK)
   {
      RETVALUE(RFAILED);
   }
   sdu->sdu.isUeCfgPres = FALSE;
   RLOG0(L_INFO,"wrUmmCsfbSndRrcConRelMsg: Sending RRC Connection Release");
   wrIfmRrcSndReqToUe(sdu);
   RETVALUE(ROK);
}

/** @brief This function is used to Fill GERAN redirection information for RRC
 * connection release
 *
 * @details
 *
 *     Function: wrUmmFillGeranRedirectInfo
 *
 *         Processing steps:
 *            - Fill redirection information based on CS domain PLMNs by calling
 *            funtion wrGetCsdomainPLMNNeighFreqs
 *
 *     capabilities
 *
 * @param[in]  transCb    : transaction control block
 * @param[out]  redirInfo : redirection information
 * @return
 *    -#Success : WrUmmRedirectInfo*
 *    -#Failure : NULLP
 */
PUBLIC  WrUmmRedirectInfo* wrUmmFillGeranRedirectInfo
(
 WrUmmTransCb             *transCb
 )
{
   WrUmmRedirectInfo         *redirInfo = NULLP;

   TRC2(wrUmmFillGeranRedirectInfo)

      /*This is a wrapper function to access EMM cellCb*/
      wrGetCsdomainPLMNNeighFreqs(transCb->ueCb,
            transCb->ueCb->regLai.plmnId,&redirInfo);

   RETVALUE(redirInfo);
}

/** @brief This function is responsible for invoking functions that build and
 *         send RRC Redirection information to UE. 
 * 
 *  @details
 *
 *     Function: wrUmmCsfbGeranFillArfcnSndRrcRelToUe
 *
 *         Processing steps:
 *         - call wrUmmFillGeranRedirectInfo and fill redirInfo
 *         - call wrUmmCsfbGeranSndRrcConRelMsg
 *         - Start WR_TMR_RRC_CONREL and WR_TMR_RRC_DAT_CFM
 *
 * @param [in] transCb : transation Control block
 * @return S16
 -# Success : ROK
 -# Failure : RFAILED 
 */
PUBLIC S16 wrUmmCsfbGeranFillArfcnSndRrcRelToUe
(
 WrUmmTransCb *transCb
 )
{
   WrUeCb                    *ueCb      = transCb->ueCb;
   WrUmmRedirectInfo         *redirInfo = NULLP;
   WrUmmCellCb               *cellCb = NULLP;

   if (transCb->u.csfbTrans.csfbState != WR_UMM_CSFB_TRANS_STATE_REDIR_SENT)
   {
      /* 1. This csFallbackInd will be used for filling RRC Connection Release cause
       * 2. with v1020 cause "NhuRlsCausecs_FallbackHighPriority_v1020Enum" as
       * defined for CSFB high priority.
       * 3. csfbTransCb has to be initialised in the beginning of handling the
       * internal message, but that is not done.
       * 4. this transaction can be deferred and resumed by Re-establishment transaction
       * 5. upon resuming this transaction csfbTransCb should not be
       * re-initialized
       * 6. So, as a work around initialising it here because 
       *  transaction state will be WR_UMM_CSFB_TRANS_STATE_REDIR_SENT when it
       *  gets resumed 
       *  7. similarly for other RATs this has to be taken care for Highpriority
       *  cs calls*/
      wrFillTknU32(&transCb->u.csfbTrans.csFallBackInd,
            transCb->msg->u.internalMsg->u.csfbIndMsg->csFallBackInd.val);

      WR_UMM_GET_CELLCB(cellCb, ueCb->cellId)
         if(NULLP == cellCb)
         {
            RLOG0(L_ERROR, " Failed to get cellCB ");
            RETVALUE(RFAILED);
         }

      redirInfo = wrUmmFillGeranRedirectInfo(transCb);
      if(NULLP == redirInfo)
      {
         RLOG0(L_ERROR,"No Neighbor Frequencies to redirect");
         transCb->state = WR_UMM_TRANS_RELEASE_CALL;
         transCb->errorCause = WR_UMM_CTXT_CSFB_REL;
         /*For KPI*/
         ueCb->csfbInfo.redirType = GERAN_CSFB_WITHOUT_REDIRECTION;
         ueCb->csfbInfo.ratType = GERAN;
         /*CSFB failure KPI*/
         wrIncKpiCsfbFail(GERAN_CSFB);
         RETVALUE(RFAILED);
      }
      /*For KPI*/
      ueCb->csfbInfo.redirType = GERAN_CSFB_WITH_REDIRECTION;
      ueCb->csfbInfo.ratType = GERAN;

      transCb->u.csfbTrans.redirectInfo = redirInfo;
   }
   if(RFAILED == wrUmmCsfbGeranSndRrcConRelMsg(transCb))
   {
      RLOG0(L_ERROR, "Sending RRC Connection Release Failed");
      RETVALUE(RFAILED);
   }

   ueCb->rrcConnState = WR_UMM_RRC_IDLE;
   transCb->u.csfbTrans.csfbState  = WR_UMM_CSFB_TRANS_STATE_REDIR_SENT;
   /* Starting rrcDatCfmWaitTmr timer*/
   RLOG_ARG0(L_INFO,DBG_CRNTI,transCb->ueCb->crnti,
         "Starting rrc connection release datCfm Wait timer");
   wrStartTransTmr(transCb,WR_TMR_RRC_CONREL,
         wrEmmGetRrcConRelTmrVal (transCb->ueCb->cellId));

   RETVALUE(ROK);
}

/** @brief This function is to perform Cell Change Order or redirection 
 *     wrUmmCsfbGeranDecision 
 *
 *  @details
 *
 *     Function: wrUmmCsfbGeranDecision
 *
 *         Processing steps:
 *         - Set the choice based on eNB capabilities and UE capabilities 
 *
 * @param [in] transCb : transation Control block
 * @return Void
 */
PRIVATE Void wrUmmCsfbGeranDecision 
(
 WrUmmTransCb         *transCb,
 WrUmmCellCb          *cellCb
 )
{   
   WrUmmCsfbCcoTransCb          *ccoTrans = &transCb->u.csfbGeranTrans;
   WrUeCb                       *ueCb = transCb->ueCb;
   Bool                         cndNghMeas = FALSE;

   /*Set CSFB GERAN UE state to CCO Initialized */
   ueCb->csfbInfo.csfbState = WR_CSFB_GSM_UE_STATE_CCO_INIT;
   /* Set CSFB GERAN Transaction state to CSFB_INIT*/
   ccoTrans->csfbState = WR_UMM_CSFB_TRANS_STATE_CSFB_INIT;

   /* Initialise the choice to CCO without measurements*/
   ccoTrans->choice = WR_CSFB_GERAN_CCO;

   /* Find candidate neighbour cells for measurement. cndNghMeas will be set to
    * TRUE only if more than one nieghbour is found*/
   cndNghMeas = wrFndCandidateNeighs(transCb);

   /* Measurements will be configured to UE only if isGERANMeasAllowedForCCO 
    * is TRUE and FGI bit 16 and 23 is set and more than one neighbours selected for
    * measurements*/
   if((TRUE == cellCb->csfbToGeranCfg.isGERANMeasAllowedForCCO) && 
         (TRUE == cndNghMeas))
   {
      if((TRUE == wrUmmUtilFeatureGrpInd(ueCb->cellId,ueCb->crnti,
                  WR_UMM_GERAN_MEAS_SUPPORTED)) && 
            (TRUE == wrUmmUtilFeatureGrpInd(ueCb->cellId, ueCb->crnti,
                                            WR_UMM_NON_ANR_PERD_MEAS_SUPPORTED)))/* RRC-REL10-Upgrade */ 
      {
         ccoTrans->choice |= WR_CSFB_GERAN_MEAS;
      }
   }

   if(0 == transCb->u.csfbGeranTrans.cfgdNeighCnt)
   {
      ccoTrans->choice = WR_CSFB_GERAN_REDIR; 
   }
   RETVOID; 
}
/** @brief This function is responsible for checking if measurement gap is
 *         required for the UE for IRAT
 * 
 *  @details
 *
 *     Function: wrUmmChkIRATBndLstNeedFrGaps 
 *
 *         Processing steps:
 *         - Fill bandArr if respective enum band indicator
 *           is supported with TRUE.
 *         - Call wrUmmMeasGapReqdInterRat for every band indicator 
 *           array element if set to TRUE.
 *         - Return isGapReqd 
 *
 * @param [in] ueCb         : UE Control block
 * @param [in] tgtBnd       : target GERAN neighbour band array
 * @param [in] nghCnt       : neighbour count 
 * @param [out] isGapReqd   : bool flag 
 * @return Bool
 -# Success : TRUE
 -# Failure : FALSE 
 */
PUBLIC Bool wrUmmChkIRATBndLstNeedFrGaps
(
 WrUeCb   *ueCb,
 U8       *tgtBnd,
 U8       nghCnt
 )
{
   Bool      bandArr[WR_GERAN_MAX_BANDS] = {0};
   Bool      isGapReqd = FALSE;
   U32       idx;
   U16       servFreq;
   U8        servingBand;

   servFreq = wrEmmGetServCarrierFreq(ueCb->cellId);
   wrUmmFndEUTRABand(servFreq,&servingBand);

   for(idx = 0;idx < nghCnt && idx < WR_UMM_MEAS_MAX_GERAN_ARFCN;idx++)
   {
      if((FALSE == bandArr[WR_GERAN_GSM850]) && (WR_GERAN_GSM850 == tgtBnd[idx]))
      {
         bandArr[WR_GERAN_GSM850] = TRUE;
      }
      else if((FALSE == bandArr[WR_GERAN_GSM900]) && 
            (WR_GERAN_GSM900 == tgtBnd[idx]))
      {
         bandArr[WR_GERAN_GSM900] = TRUE;
      }
      else if((FALSE == bandArr[WR_GERAN_DCS1800]) && 
            (WR_GERAN_DCS1800 == tgtBnd[idx]))
      {
         bandArr[WR_GERAN_DCS1800] = TRUE;
      }
      else if((FALSE == bandArr[WR_GERAN_PCS1900]) &&
            (WR_GERAN_PCS1900 == tgtBnd[idx]))/* For WR_GERAN_PCS1900 */
      {
         bandArr[WR_GERAN_PCS1900] = TRUE;
      }
   }

   for(idx = 0;idx < WR_GERAN_MAX_BANDS; idx++)
   {
      if(TRUE == bandArr[WR_GERAN_GSM850])
      {
         wrUmmMeasGapReqdInterRat(ueCb,servingBand,NhuSuppBandGERANgsm850Enum,
               GERAN,NULLP,&isGapReqd);
      }
      else if(TRUE == bandArr[WR_GERAN_GSM900])
      {
         wrUmmMeasGapReqdInterRat(ueCb,servingBand,NhuSuppBandGERANgsm900EEnum,
               GERAN,NULLP,&isGapReqd);
      }
      else if(TRUE == bandArr[WR_GERAN_DCS1800])
      {
         wrUmmMeasGapReqdInterRat(ueCb,servingBand,NhuSuppBandGERANgsm1800Enum,
               GERAN,NULLP,&isGapReqd);
      }
      else /*For WR_GERAN_PCS1900*/ 
      {
         wrUmmMeasGapReqdInterRat(ueCb,servingBand,NhuSuppBandGERANgsm1900Enum,
               GERAN,NULLP,&isGapReqd);
      }

      if(TRUE == isGapReqd)
      {
         /*Returns TRUE*/
         RETVALUE(isGapReqd);
      }
   }
   /*Returns FALSE*/
   RETVALUE(isGapReqd);
}
/** @brief This function is responsible for building and sending GERAN
 *         measurement object to UE.
 * 
 *  @details
 *
 *     Function: wrUmmCsfbGeranSendMeasCfg 
 *
 *         Processing steps:
 *         - Fill measurement object 
 *         - Check if measurement gap is required and set accordingly
 *         - 
 *
 * @param [in] transCb : transation Control block
 * @return S16
 -# Success : ROK
 -# Failure : RFAILED 
 */
PRIVATE S16 wrUmmCsfbGeranSendMeasCfg
(
 WrUmmTransCb     *transCb
 )
{
   U8                          measObjId;
   /*This flag is set to TRUE only if PCS1900 bandindicator is filled into measObj*/
   Bool                        flag = FALSE;
   WrMeasUeCb                  *measUeCb = NULLP;
   WrUmmMeasTransInfo          *measCfgInfo = NULLP;
   WrUmmCellCb                 *cellCb = NULLP;
   WrMeasUeCbGeranMeasObjNode  *geranObj = NULLP;
   WrUmmMeasGeranMeasObjNode   *geranMeasObj = NULLP;
   WrUmmMeasAddInfo            *addMeasInfo = NULLP;
   WrUeCb                      *ueCb = NULLP;
   WrUmmMeasRptCfgInfo         rptCfgInfo; 
   U32                         idx;
   U8                          nghCnt = transCb->u.csfbGeranTrans.cfgdNeighCnt;
   U8                          targetBand[nghCnt];

   ueCb = transCb->ueCb;

   WR_UMM_GET_CELLCB(cellCb, ueCb->cellId);
   if(NULLP == cellCb)
   {
      RLOG0(L_ERROR, "cellCb is NULL");
      RETVALUE(RFAILED);
   }
   measCfgInfo = &transCb->u.csfbGeranTrans.measCfgInfo;
   measUeCb = transCb->ueCb->measCfg;
   addMeasInfo = &measCfgInfo->addMeasInfo;

   cmMemset((U8*)&rptCfgInfo,0,sizeof(WrUmmMeasRptCfgInfo));
   if(ROK != wrUmmMeasUpdtGeranMeasObjAndIdFromArfcn(measUeCb, &measObjId,
            &geranObj))
   {
      if(WR_UMM_MEAS_INVALID_MEAS_ID == wrUmmMeasGetFreeMeasObjId(measUeCb, &measObjId))
      {
         RLOG0(L_ERROR, "Meas Object Id not available");
         RETVALUE(RFAILED);
      }

      /*To be freed after receiving RRC Reconfiguration Complete 
       * case C1_RRCCONRECFGNCOMPL or on expiry of WR_TMR_CSFB_GSM_MEAS_WAIT */
      WR_ALLOC(&geranObj,sizeof(WrMeasUeCbGeranMeasObjNode));

      if(NULLP == geranObj)
      {
         RLOG0(L_FATAL,"Memory allocation failed.");
         RETVALUE(RFAILED);
      }

      WR_ALLOC(&geranMeasObj, sizeof(WrUmmMeasGeranMeasObjNode));
      if(geranMeasObj == NULLP)
      {
         RLOG0(L_FATAL,"Memory allocation failed.");
         WR_FREE(geranObj,sizeof(WrMeasUeCbGeranMeasObjNode)); 
         RETVALUE(RFAILED);
      }
      geranObj->measObjId = measObjId;

      for(idx = 0;idx < nghCnt; idx++)
      { 
         geranMeasObj->arfcn[idx] = 
            transCb->u.csfbGeranTrans.measObjCfgdNeig[idx]->freqCb->arfcn; 
         geranMeasObj->nccPermittedMeas |=  
            transCb->u.csfbGeranTrans.measObjCfgdNeig[idx]->nccPermittedMeas;
         geranMeasObj->bandIndicator[idx] =  
            transCb->u.csfbGeranTrans.measObjCfgdNeig[idx]->freqCb->bandInd;
         /*Index is 0 for Strong cell*/
         geranMeasObj->qOffset = 
            transCb->u.csfbGeranTrans.measObjCfgdNeig[idx]->offsetFreq;

         if((FALSE == flag) && 
               (WR_GERAN_PCS1900 == transCb->u.csfbGeranTrans.measObjCfgdNeig[idx]->freqCb->bandInd))
         {
            geranMeasObj->isBandInd1900Pres = TRUE;
            flag = TRUE ;
         }
      }
      geranMeasObj->arfcnCnt = nghCnt;
      geranMeasObj->lnk.node = (PTR) geranMeasObj;
      cmLListAdd2Tail(&ueCb->hrlMeasCfg.geranMeasObjLst, &geranMeasObj->lnk);
      /* as of now there will be only one GERAN Measurement Object */
      geranObj->lnk.node = (PTR)geranObj;
      cmLListAdd2Tail(&measUeCb->geranMeasObjLst, &geranObj->lnk);

      addMeasInfo->measObjLst[addMeasInfo->measObjIdCnt] = measObjId;
      addMeasInfo->measObjIdCnt++;
   }
    /* case C1_RRCCONRECFGNCOMPL or on expiry of WR_TMR_CSFB_GSM_MEAS_WAIT */

   rptCfgInfo.rptCfgTyp = WR_UMM_MEAS_GERAN_STRONG_CELL_CSFB;
   rptCfgInfo.thrshldTyp = WR_UMM_MEAS_THRES_TYPE_NONE;
   rptCfgInfo.cdma2kCellType = WR_UMM_MEAS_CELL_TYPE_NONE;
   rptCfgInfo.intRptCfgType = WR_UMM_MEAS_RPTCFG_NONE;

   if(ROK != wrUmmMeasRptAndMeasIdCfg(WR_UMM_MEAS_TYPE_CSFB_CCO, measObjId,
            &rptCfgInfo, ueCb, cellCb->measCellCfg, addMeasInfo))
   {
      /*Release the allocated memory for measurement object
       * in wrUmmCsfbGeranSendMeasCfg*/
      wrUmmCsfbGeranFreeMeasObj(transCb->ueCb,measObjId);

      RLOG0(L_ERROR, "Invalid report and Meas config Id");
      RETVALUE(RFAILED);
   }

   /*Check for InterRAT-BandList before configuring gap*/
   if(TRUE == wrUmmChkIRATBndLstNeedFrGaps(ueCb,targetBand,nghCnt))
   {
      wrUmmMeasAddMeasGap(WR_UMM_MEAS_GAP_CSFB,
            cellCb, ueCb, &measCfgInfo->measGapStatus,
            transCb->transId);
   }
   if((measUeCb->gapMeasInfo.latestTxId == transCb->transId) &&
         ((WR_UMM_MEAS_GAP_INIT_SCH_CONFIG == measCfgInfo->measGapStatus)))
   {
      wrUmmMeasGapSchdUeRecfgReq(transCb);
   }
   else
   {
      /* Prepare and send the message to the RRC layer */
      if(ROK != wrUmmMeasPrepareSendRrcRecfg(ueCb->cellId, ueCb,
               measCfgInfo,transCb->transId))
      {
         RLOG0(L_ERROR,"RRC ReCfg Req for measCFG send failed");
         RETVALUE(RFAILED);
      }

      /*start GERANMeasWaitTmr after sending RRC Reconfig to UE*/
      RLOG_ARG0(L_INFO,DBG_CRNTI,transCb->ueCb->crnti,
            "Starting GERANMeasWaitTmr timer");
      wrStartTransTmr(transCb,WR_TMR_CSFB_GSM_MEAS_WAIT,
            (cellCb->csfbToGeranCfg.GERANMeasWaitTmr + 
             WR_TMR_GERAN_MEAS_RPT_WAIT_DELAY));

   }
   RETVALUE(ROK); 
}

/** @brief This function is responsible for sending an interanal message to
 *         CSFB_REDIR transaction if re-establishment is received during CCO 
 * 
 *  @details
 *
 *     Function: wrUmmCsfbSndRedirAftReest 
 *
 *         Processing steps:
 *         - Create and process INTERNAL MSG to CSFB_REDIR transaction 
 *
 * @param [in] ueCb : UE Control block
 * @return S16
 -# Success : ROK
 -# Failure : RFAILED 
 */
PUBLIC S16 wrUmmCsfbSndRedirAftReest
(
 WrUeCb   *ueCb
 )
{
   Void                      *transMsg = NULLP;
   WrUmmIntCsfbTransMsg      *csfbtransMsg = NULLP;
   WrUmmInternalMsg          *internalMsg = NULLP;
   WrUmmIncMsg               *msg = NULLP;
   WrUmmCellCb               *cellCb = NULLP;

   WR_UMM_GET_CELLCB(cellCb,ueCb->cellId);
   if(NULLP == cellCb)
   {
      RLOG0(L_ERROR,"cellCb not found");
      RETVALUE(RFAILED);
   }

   /*This memory will be freed in wrUmmRlsIncMsg in case of 
    * positive scenarios*/ 
   WR_ALLOC(&csfbtransMsg, sizeof(WrUmmIntCsfbTransMsg));

   if (NULLP == csfbtransMsg)
   {
      RLOG0(L_FATAL, "Memory allocation failed.");
      RETVALUE(RFAILED);
   }

   /* for other RATs cSFB Redirection transaction resumption should happen with
    * this internal message rather than from the frame work sending the
    * RRCReestablishmentComplete message to the deferred transaction
    * if that gets implemented ratPriority should not be sent in this internal
    * message because this information should already exist in
    * csfb_redir_transCb */
   csfbtransMsg->ratPriority = ueCb->csfbInfo.ratType;

   transMsg = (Void *)csfbtransMsg;
   if (ROK != wrUmmBldInternalMsg(&internalMsg, WR_UMM_INTMSG_CSFB_REDIR_TRANS,
            transMsg))
   {
      RLOG0(L_ERROR,"creation of internal message Failed ");
      if(NULLP != csfbtransMsg)
      {
         WR_FREE(csfbtransMsg, sizeof(WrUmmIntCsfbTransMsg));
      }
      RETVALUE(RFAILED);
   }

   WR_ALLOC(&msg,sizeof(WrUmmIncMsg));
   if (NULLP == msg)
   {
      RLOG0(L_FATAL, "Memory allocation failed.");
      if(NULLP != csfbtransMsg)
      {
         WR_FREE(csfbtransMsg, sizeof(WrUmmIntCsfbTransMsg));
      }
      RETVALUE(RFAILED);
   }
   msg->msgTyp = WR_UMM_INTERNAL_MSG;
   msg->u.internalMsg = internalMsg;
   msg->refCnt = 1;

   wrUmmPrcIncMsg(cellCb, ueCb, msg);
   wrUmmRlsIncMsg(&msg);
   RETVALUE(ROK);
}
/** @brief This function is repsonsible for invoking respcetive functions based
 *         on the choice.
 * 
 *  @details
 *
 *     Function: wrUmmCsfbGeranDecisionExec 
 *
 *         Processing steps:
 *         - Initialze timers WR_TMR_CSFB_GSM_T304_EXP_WAIT and
 *           WR_TMR_CSFB_GSM_MEAS_WAIT
 *         - Based on choice invoke CCO or Redirection or measurements
 *
 * @param [in] transCb : transation Control block
 * @return Void 
 */

PRIVATE Void wrUmmCsfbGeranDecisionExec
(
 WrUmmTransCb     *transCb
 )
{
   /*Initialize WR_TMR_CSFB_GSM_T304_EXP_WAIT timer*/
   cmInitTimers(&(transCb->u.csfbGeranTrans.t304WaitTmr.tmr), WR_MAX_TMR_PER_UE);

   /*Initialize WR_TMR_CSFB_GSM_MEAS_WAIT timer*/
   cmInitTimers(&(transCb->u.csfbGeranTrans.measWaitTmr.tmr), WR_MAX_TMR_PER_UE);

   switch(transCb->u.csfbGeranTrans.choice)
   {
      case WR_CSFB_GERAN_CCO:
         {
            wrUmmCsfbGeranSendCco(transCb);
            break;
         }
      case WR_CSFB_GERAN_CCO_MEAS:
         {
            if(RFAILED == wrUmmCsfbGeranSendMeasCfg(transCb))
            {
               /* Go for Redirection by posting INTERNAL message to
                * WR_UMM_INTMSG_CSFB_REDIR_TRANS*/
               RLOG0(L_WARNING,"Measurements for CCO procedure failed! "
                     "Triggering Redirection to GERAN!");
               wrUmmCsfbGeranBuildSndRedir(transCb);
               wrUmmTransComplete(transCb);
            }
            break;
         }
      case WR_CSFB_GERAN_REDIR:
         {
            /* Go for Redirection by posting INTERNAL message to 
             * WR_UMM_INTMSG_CSFB_REDIR_TRANS*/
            RLOG0(L_INFO,"Triggering Redirection to GERAN!");
            wrUmmCsfbGeranBuildSndRedir(transCb);  
            wrUmmTransComplete(transCb); 
            break;
         }
      default:
         RLOG0(L_ERROR,"Invalid Choice");
         break;
   }
   RETVOID;
}
/** @brief This function is responsible for handling GERAN CSFB CCO procedure 
 *         related messages 
 * 
 *  @details
 *
 *     Function: wrUmmCsfbGeranCcoProcessor 
 *
 *         Processing steps:
 *         - Check the message type
 *         - Handle Internal message and take decision
 *         - Handle timer expiry events
 *         - Handle measurement reports and Reconfig complete
 *         - Handle RRC DAT CFM and SCHD CFG CFM for meas gap
 *
 * @param [in] transCb : transation Control block
 * @return S16
 -# Success : ROK
 -# Failure : RFAILED 
 */
PUBLIC S16 wrUmmCsfbGeranCcoProcessor
(
 WrUmmTransCb                *transCb
 )
{
   WrUmmIncMsg                *msg = transCb->msg;
   WrUmmCsfbCcoTransCb        *csfbGeranTrans = &transCb->u.csfbGeranTrans;
   WrUmmIntCsfbTransMsg       *csfbIntMsg = NULLP;
   WrUmmCellCb                *cellCb = NULLP;

   WR_UMM_GET_CELLCB(cellCb,transCb->ueCb->cellId);
   if(NULLP == cellCb)
   {
      RLOG0(L_ERROR,"cellCb not found");
      RETVALUE(RFAILED);
   }

   switch(msg->msgTyp)
   {
      case WR_UMM_INTERNAL_MSG:
         {
            csfbIntMsg = msg->u.internalMsg->u.csfbIndMsg;
            wrFillTknU32(&(csfbGeranTrans->csFallBackInd),
                  csfbIntMsg->csFallBackInd.val); 
            wrUmmCsfbGeranDecision(transCb,cellCb);
            wrUmmCsfbGeranDecisionExec(transCb);
         }
         break;
      case WR_UMM_TMR_EXPIRY:
         {
            switch (transCb->msg->u.tmrExpiry.timerEvnt )
            {
               case WR_TMR_CSFB_GSM_T304_EXP_WAIT:
                  {
                     /*send UE CONTEXT RELEASE REQUEST to MME*/
                     RLOG_ARG0(L_INFO, DBG_CRNTI,transCb->ueCb->crnti,
                           "IRAT T304 timer for CCO expired"
                           "CCO Transaction complete");
                     transCb->errorCause = WR_UMM_CTXT_CSFB_REL;
                     transCb->state = WR_UMM_TRANS_RELEASE_CALL;
                     transCb->ueCb->rrcConnState = WR_UMM_RRC_IDLE;
                     transCb->ueCb->mobCtrlState = WR_HO_UE_RELEASE;
                     break;
                  }

               case WR_TMR_CSFB_GSM_MEAS_WAIT:
                  {
                     /*Send RRC Connection Release with redirection*/
                     RLOG_ARG0(L_INFO, DBG_CRNTI,transCb->ueCb->crnti,
                           "Measurement report wait timer for CCO expired."
                           "Sending RRCCONNECTION RELEASE with redirection");
                     wrUmmCsfbGeranBuildSndRedir(transCb);
                     wrUmmTransComplete(transCb);
                     break;
                  }
            }
         }
         break;
      case WR_UMM_UU_IND_PDU:
         { 
            wrUmmCsfbGeranPrcRrcMsg(transCb);
         }
         break;

      case WR_UMM_RRC_DAT_CFM:
         {
            if(msg->u.uuCfmPdu->hdr.transId & WR_UMM_CSFB_GERAN_CCO_TRANSID)
            {
               RLOG0(L_INFO,"RRC DAT CFM received for Mobility from EUTRA Command");
            }
            break;
         }
      case WR_UMM_SCHD_CFG:
         {
            if (transCb->u.csfbGeranTrans.measCfgInfo.measGapStatus ==
                  WR_UMM_MEAS_GAP_INIT_SCH_CONFIG)
            {
               RLOG0(L_DEBUG, " Received Schd Gap Cfg Cfm ");
#ifdef WR_RSYS_KPI 
               /* Schd Cfm for MeasGap is received */
               transCb->ueCb->measGapUsd = TRUE;
#endif /*WR_RSYS_KPI*/

               /* Change the measGapStatus to UE Config */
               transCb->u.csfbGeranTrans.measCfgInfo.measGapStatus =
                  WR_UMM_MEAS_GAP_INIT_UE_CONFIG;

               /*Prepare and send the RRC Reconfiguration message */
               if(ROK != wrUmmMeasPrepareSendRrcRecfg(transCb->ueCb->cellId, 
                        transCb->ueCb, 
                        &transCb->u.csfbGeranTrans.measCfgInfo, 
                        transCb->transId))
               {
                  /* Failed to send the message to the UE. 
                   * So call Redirection trans and return failure */
                  wrUmmCsfbGeranBuildSndRedir(transCb);
                  wrUmmTransComplete(transCb);
                  RETVALUE(RFAILED);
               }

               /*start GERANMeasWaitTmr after sending RRC Reconfig to UE*/
               RLOG_ARG0(L_INFO,DBG_CRNTI,transCb->ueCb->crnti,
                     "Starting GERANMeasWaitTmr timer");
               wrStartTransTmr(transCb,WR_TMR_CSFB_GSM_MEAS_WAIT,
                     (cellCb->csfbToGeranCfg.GERANMeasWaitTmr + 
                      WR_TMR_GERAN_MEAS_RPT_WAIT_DELAY));
            }
            break;
         }
   }
   RETVALUE(ROK);
}
/** @brief This function is reponsible for taking the defined actions on
 *         releasing the CCO transaction.
 * 
 *  @details
 *
 *     Function: wrUmmCsfbGeranCcoTransRel 
 *
 *         Processing steps:
 *         - Stop all transaction timers
 *
 * @param [in] transCb : transation Control block
 * @return S16
 -# Success : ROK
 -# Failure : RFAILED 
 */
PUBLIC S16 wrUmmCsfbGeranCcoTransRel
(
 WrUmmTransCb *transCb
 )
{
   WrUmmCellCb               *cellCb = NULLP;
   WR_UMM_GET_CELLCB(cellCb,transCb->ueCb->cellId);
   if(NULLP == cellCb)
   {
      RLOG0(L_ERROR,"cellCb not found");
      RETVALUE(RFAILED);
   }
   wrUmmStopTransTimers(transCb);
   /*Fill remove measurement info*/
   wrUmmFillGeranMeasIdsRmvLst(transCb);

   /*Add the to-be-removed IDs to free list from used list*/
   wrUmmMeasCfgReCfgReqCompHand(WR_UMM_MEAS_TYPE_CSFB_CCO,transCb);
   /* If Meas gap is configured then inform to the Meas 
    *              transaction and close the transaction */
   wrUmmMeasRelMeasGap(WR_UMM_MEAS_GAP_CSFB, cellCb, transCb->ueCb,
         &transCb->u.csfbGeranTrans.measCfgInfo.measGapStatus,
         transCb->transId);


   RETVALUE(ROK);
}
/** @brief  This function is used to handle the incoming transactions when CCO
 *          is going on. The new transaction is passed to this function and this
 *          function determines the next course of action based on the new
 *          transaction.
 * 
 *  @details
 *
 *     Function: wrUmmCsfbGeranCcoNewTransProcessor 
 *
 *         Processing steps:
 *         - For re-establishment with Cause
 *             i.  Other Failure: stop WR_TMR_CSFB_GSM_T304_EXP_WAIT and trigger
 *                                redirection by allowing re-establishment trans
 *             ii. HO Failure: stop WR_TMR_CSFB_GSM_T304_EXP_WAIT and allow
 *                             re-establishment.
 *             iii.Reconfig Failure: stop WR_TMR_CSFB_GSM_T304_EXP_WAIT and send
 *                                   re-establishment reject and release the UE.
 *         - E-Rab Setup/Modify/Release - Call the wrUmmHdlAndPrcRabMsg API
 *              which will send the failure response towards S1AP.
 *              And update the status of incoming transaction.
 *         - Measurement - Fail this transaction
 *
 * @param [in] transCb : transation Control block
 * @param[in]  incTrans   : incoming transaction information
 * @return S16         : WR_UMM_TRANS_DEFER_ONGOING
 *                       WR_UMM_TRANS_FAIL
 *                       WR_UMM_TRANS_OK
 */
PUBLIC S16 wrUmmCsfbGeranCcoNewTransProcessor
(
 WrUmmTransCb                *transCb,
 WrUmmTransCb                *incTrans
 )
{
   WrUmmMsgCause              cause;
   S16                        ret = WR_UMM_TRANS_OK;

   switch(incTrans->transTyp)
   {
      case WR_UMM_RRC_REESTAB_TRANS:
         {
            U32                       reestabCause; 
            NhuUlCcchMsg              *ulCcchPdu = NULLP;

            ulCcchPdu =  &(incTrans->msg->u.uuIndPdu->sdu.m.ulCcchMsg);
            reestabCause = ulCcchPdu->ccchMsg.message.val.c1.val.rrcConReestbRqst.
               criticalExtns.val.rrcConReestbRqst_r8.reestablishmentCause.val;
            /*STOP T304expWaitTmr timer*/
            wrStopTransTmr(transCb,WR_TMR_CSFB_GSM_T304_EXP_WAIT);

            if(NhuReestbCauseotherFailEnum == reestabCause)
            {
               /*Handle the incoming Re-establishment transaction
                * and abort the CSFB transaction*/
               RLOG_ARG0(L_INFO, DBG_CRNTI, transCb->ueCb->crnti, 
                     "Re-Establishment has been received with cause OtherFailure"
                     "Admitting the UE and sending Redirection");

               /*Set CSFB GERAN UE state to RRC_REDIR*/
               transCb->ueCb->csfbInfo.csfbState = WR_CSFB_GSM_UE_STATE_RRC_REDIR;
            }
            else if(NhuReestbCausehandoverFailEnum == reestabCause)
            {
               /*Abort the CCO procedure and accept the re-establishment*/
               RLOG_ARG0(L_INFO, DBG_CRNTI, transCb->ueCb->crnti, 
                     "Re-Establishment has been received with cause HOFailure"
                     "Aborting CCO procedure and admitting the UE");

               /*CSFB failure KPI*/
               wrIncKpiCsfbFail(GERAN_CSFB);

               transCb->ueCb->csfbInfo.csfbState = WR_CSFB_GSM_UE_STATE_CSFB_READY;
            }
            else if(NhuReestbCausereconfigurationFailEnum == reestabCause)
            {
               RLOG_ARG0(L_INFO, DBG_CRNTI, transCb->ueCb->crnti, 
                     "Re-Establishment has been received with cause Reconfig"
                     "Failure. Aborting CCO procedure and Re-establishment"
                     "transaction rejects reestablishment");
            }
            ret = WR_UMM_TRANS_OK;
            wrUmmTransComplete(transCb);
         }
         break;

      case WR_UMM_ERAB_SETUP_TRANS:
      case WR_UMM_ERAB_MOD_TRANS:
      case WR_UMM_RAB_REL_TRANS:
         {
            /* may be this can be enhanced to set failure status of the erab 
             * transaction, allow the transaction, parse message and send
             * erab failure response */
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
            /* According to 36.331 Section 5.5.6.1 upon re-establishment accept
             * UE reset VarMeasReportList and sends any event type measurement reports
             * so, even though eNB drops event measurement reports post Re-establishment
             * eNB will get measurement report(s) if UE still meets the event entry criteria
             * with this approach, eNB is not acting on the old reports */
            if(incTrans->msg->msgTyp == WR_UMM_INTERNAL_MSG && transCb->numDefMsgCount < WR_UMM_DEF_LIST)
            {
               WR_UMM_CPY_DEFR_MSG_TO_TRANSCB(transCb, incTrans->msg);       
            } 
            else
            {
               RLOG0(L_WARNING,"Dropping the received measurement report \n");
            }
            ret = WR_UMM_TRANS_FAIL;
         }
         break;
      case WR_UMM_CSFB_REDIR_TRANS:
         {
            ret = WR_UMM_TRANS_OK;
         }
         break;
      case WR_UMM_ANR_MEAS_TRANS:
      case WR_UMM_EVENT_ANR_TRANS:
         {
            RLOG0(L_WARNING, "wrUmmCsfbNewTransProcessor not allowing Periodic ANR Transaction handling"
                  "as CSFB is ongoing\n");
            ret = WR_UMM_TRANS_FAIL;
         }
         break;
      default:
         break;

   }
   RETVALUE(ret);
}

/********************************************************************30**

           End of file:    /main/

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
/main/        ---      Sriky             GA4.0Gamma
*********************************************************************91*/

