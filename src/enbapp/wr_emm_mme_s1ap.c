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
               

     File:     wr_emm_mme_s1ap.c

     Sid:      $SID$ 

     Prg:      Sriky 

*********************************************************************21*/
static const char* RLOG_MODULE_NAME="EMM";
static int RLOG_MODULE_ID=1;
static int RLOG_FILE_ID=2;

#include "wr.h"
#include "wr_utils.h"
#include "wr_emm_mme.h"
#include "wr_kpi.h"

#define WR_CGI_IE_LEN        28         /* Bits   */
#define WR_SER_NUM_LEN       16         /* Bits   */
#define WR_S1AP_CSG_ID_LEN   27         /* Bits */

EXTERN S16 wrEmmMmeSztFillTAC(U16 tac,S1apPdu  *pdu,SztTAC  *tAC);
EXTERN S16 wrS1apFillCause(SztCause  *ie,WrUmmMsgCause *cause); 
EXTERN S16 wrEmmMmeSztFillPLMNId(S1apPdu  *pdu,WrPlmnId  *plmnId,SztPLMNidentity  *plmnIe);
EXTERN S16  wrDiagPrntPlmnId(WrPlmnId *plmnId, U8 diagLvl);


/** @brief This function fills the tracking area code. 
 *
 * @details This function fills the S1AP: Tracking Area Code IE.
 *
 *     Function: wrEmmMmeSztFillTAC 
 *
 * @param[in]  tac: Tracing Area Code value.
 * @param[in]  pdu: S1AP PDU.
 * @param[out] tAC: S1AP Tracking Area Code IE
 * @return 
 */
PUBLIC S16 wrEmmMmeSztFillTAC
(
U16                          tac,
S1apPdu                      *pdu,
SztTAC                       *tAC
)
{
   tAC->pres   = PRSNT_NODEF;
   tAC->len    = WR_LAC_IE_LEN;
   tAC->val[1] = (U8)(tac & 0xff);
   tAC->val[0] = (U8)((tac & 0xff00)>>8);
   RETVALUE(ROK);
}

/** @brief This function fills the LAC 
 *
 * @details This function fills the S1AP: Location Area Code IE.
 *
 *     Function: wrEmmMmeSztFillLAC 
 *
 * @param[in]  inLac: Location Area Code value.
 * @param[out] *outLac: Location Area Code IE
 * @return 
 */
PUBLIC S16 wrEmmMmeSztFillLAC
(
U16                          inLac,
SztLAC                       *outLac
)
{
   outLac->pres   = PRSNT_NODEF;
   outLac->len    = WR_LAC_IE_LEN;
   outLac->val[1] = (U8)(inLac & 0xff);
   outLac->val[0] = (U8)((inLac & 0xff00)>>8);
   RETVALUE(ROK);
}

/** @brief This function fills the Cell Identity  
 *
 * @details 
 *
 *     Function: wrEmmMmeSztFillCI
 *
 * @param[in]  inCi: Cell Identity value.
 * @param[out] *outCi: Cell Identity IE
 * @return 
 */
PUBLIC S16 wrEmmMmeSztFillCI
(
U16                          inCi,
SztCI                       *outCi
)
{
   outCi->pres   = PRSNT_NODEF;
   outCi->len    = WR_CI_IE_LEN;
   outCi->val[1] = (U8)(inCi & 0xff);
   outCi->val[0] = (U8)((inCi & 0xff00)>>8);
   RETVALUE(ROK);
}

/** @brief This function fills the S1-AP Cause IE. 
 *
 * @details This function fills the S1AP: Cause IE with the
 *          cause type and cause value.
 *
 *     Function: wrS1apFillCause
 *
 * @param[out]            ie: S1AP Cause IE.
 * @param[in]          cause: cause type and cause value. 
 * @return  S16
 *        -# Success : ROK
 */
PUBLIC S16 wrS1apFillCause
(
SztCause                     *ie,
WrUmmMsgCause                  *cause
)
{
   ie->choice.pres    = PRSNT_NODEF;
   ie->choice.val     = cause->causeTyp;
   switch(cause->causeTyp)
   {
      case WR_CAUSE_RADIONW:
         wrFillTknU32(&(ie->val.radioNw), cause->causeVal);
         break;
      case WR_CAUSE_TRANSPORT:
         wrFillTknU32(&(ie->val.transport), cause->causeVal);
         break;
      case WR_CAUSE_NAS:
         wrFillTknU32(&(ie->val.nas), cause->causeVal);
         break;
      case WR_CAUSE_PROTOCOL:
         wrFillTknU32(&(ie->val.protocol), cause->causeVal);
         break;
      case WR_CAUSE_MISC:
         wrFillTknU32(&(ie->val.misc), cause->causeVal);
         break;
   }
   RETVALUE(ROK);
}

/** @brief This function fills the S1AP Time to Wait IE.
 *
 * @details This function fills the S1AP: Time to Wait IE based on the given
 *          argument "timeToWaitSecs".
 *
 *     Function: wrS1apFillTimeToWait
 *
 * @param[out]            ie: S1AP Time to wait IE.
 * @param[in] timeToWaitSecs: time to wait in secs value. 
 * @return  S16
 *        -# Success : ROK
 */
PRIVATE S16 wrS1apFillTimeToWait
(
SztTimeToWait                 *ie,
U32                           timeToWaitSecs
)
{
   if (timeToWaitSecs <= 1)
   {
      wrFillTknU32(ie, SztTimeToWaitv1sEnum);
   }
   else if (timeToWaitSecs <= 2)
   {
      wrFillTknU32(ie, SztTimeToWaitv2sEnum);
   }
   else if (timeToWaitSecs <= 5)
   {
      wrFillTknU32(ie, SztTimeToWaitv5sEnum);
   }
   else if (timeToWaitSecs <= 10)
   {
      wrFillTknU32(ie, SztTimeToWaitv10sEnum);
   }
   else if (timeToWaitSecs <= 20)
   {
      wrFillTknU32(ie, SztTimeToWaitv20sEnum);
   }
   else
   {
      wrFillTknU32(ie, SztTimeToWaitv60sEnum);
   }
   RETVALUE(ROK);
} /* end of wrS1apFillTimeToWait */

/** @brief This function fills the S1AP PLMN ID IE.
 *
 * @details This function fills the S1AP: PLMN ID IE.
 *
 *     Function: wrEmmMmeSztFillPLMNId
 *
 * @param[in]     pdu: S1AP PDU.
 * @param[in]  plmnId: PLMN ID.
 * @param[out] plmnIe: S1AP PLMN ID IE. 
 * @return  S16
 *        -# Success : ROK
 */
PUBLIC S16 wrEmmMmeSztFillPLMNId
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
   wrFillTknStrOSXL(plmnIe, WR_PLMNID_IE_LEN, pLMNId, &(pdu->memCp));
   RETVALUE(ROK);
} /* end of wrEmmMmeSztFillPLMNId */

/** @brief This function fills the S1AP CSG ID IE. 
 *
 * @details This function fills the S1AP:CSG ID IE.
 *
 *     Function:wrEmmMmeSztFillCsgId 
 *
 * @param[in]  csgId: CSG ID.
 * @param[in]    pdu: S1AP PDU.  
 * @param[out] csgIe: S1AP CSG ID IE
 * @return  S16
 *        -# Success : ROK
 */
PRIVATE S16 wrEmmMmeSztFillCsgId
(
U32                          csgId,
S1apPdu                      *pdu,
SztCSG_IdLst_Item            *csgIe
)
{
   wrFillTknBStr32(&csgIe->cSG_Id, WR_S1AP_CSG_ID_LEN, csgId);
   csgIe->iE_Extns.noComp.pres = NOTPRSNT;
   RETVALUE(ROK);
} /* end of wrEmmMmeSztFillCsgId */

/** @brief This function fills the S1AP EnodeB Global ID IE.
 *
 * @details This function fills the S1AP:EnodeB Global ID IE.
 *
 *     Function: wrEmmMmeSztFillS1eNBId 
 *
 * @param[in]     pdu: S1AP PDU.
 * @param[in] enbType: EnodeB type.
 * @param[in]     cgi: CGI  
 * @param[in]  plmnId: PLMN ID IE.
 * @param[out]  enbIe: S1AP global eNodeB ID IE 
 * @return  S16
 *        -# Success : ROK
 */
PRIVATE S16 wrEmmMmeSztFillS1eNBId
(
S1apPdu                      *pdu,
U32                          enbType,
U32                          cgi,
WrPlmnId                     *plmnId,
SztGlobal_ENB_ID             *enbIe
)
{
   /*Do Not do right shift for macro as it is already done in SM*/
   U32                       enbId = (cgi & 0x0fffffff);
   /*Do Not do right shift for macro as it is already done in SM*/

   wrFillTknU8(&(enbIe->pres), PRSNT_NODEF);
   wrEmmMmeSztFillPLMNId(pdu, plmnId, &enbIe->pLMNidentity);
   wrFillTknU8(&(enbIe->eNB_ID.choice), enbType);
   switch(enbType)
   {
      case ENB_ID_HOMEENB_ID:
        {
         enbIe->eNB_ID.val.homeENB_ID.pres = PRSNT_NODEF;
         enbIe->eNB_ID.val.homeENB_ID.len = 28;
         wrFillTknBStr32(&(enbIe->eNB_ID.val.homeENB_ID), 28, enbId);
        }
         break;
      case ENB_ID_MACROENB_ID:
       {
         enbIe->eNB_ID.val.macroENB_ID.pres = PRSNT_NODEF;
         enbIe->eNB_ID.val.macroENB_ID.len = 20;
         wrFillTknBStr32(&(enbIe->eNB_ID.val.macroENB_ID), 20, enbId);
       }
         break;
      default:
       {
         RLOG0(L_DEBUG,"Invalid Enodeb type");
        }
         break;
   }
   RLOG2(L_DEBUG,"wrEmmMmeSztFillS1eNBId:enbType = %lu, enbId = %lu",enbType,enbId);
/*ccpu00138869 end*/
   enbIe->iE_Extns.noComp.pres = NOTPRSNT;
   RETVALUE(ROK);
} /* end of wrEmmMmeSztFillS1eNBId */

/** @brief This function fills the S1AP:Tracking Area Identity IE. 
 *
 * @details This function fills the S1AP:Tracking Area Identity IE.
 *
 *     Function: wrEmmMmeSztFillTAI
 *
 * @param[in]    pdu: S1AP PDU.
 * @param[in]    tac: Tracking Area Code
 * @param[in] plmnId: PLMN ID
 * @param[out]   tai: S1AP Tracking Area Identity IE. 
 * @return  S16
 *        -# Success : ROK
 *        -# Failure : RFAILED
 */
PRIVATE S16 wrEmmMmeSztFillTAI
(
S1apPdu                      *pdu,
U16                          tac,
WrPlmnId                     *plmnId,
SztTAI                       *tai
)
{
   RLOG0(L_DEBUG,"Filling S1AP TAI IE");
   wrFillTknU8(&(tai->pres),PRSNT_NODEF);     
   if (wrEmmMmeSztFillPLMNId(pdu, plmnId, &(tai->pLMNidentity)) != ROK)
   {
      RLOG0(L_ERROR,"Failed to fill PLMN ID");
      RETVALUE(RFAILED);
   }
   wrEmmMmeSztFillTAC(tac, pdu, &(tai->tAC));
   RETVALUE(ROK);
} /* end of wrEmmMmeSztFillTAI */

/** @brief This function fills the S1AP:Target EnodeB Inforamtion.
 *
 * @details This function fills the S1AP:Target EnodeB ID IE.
 *
 *     Function: wrEmmMmeS1apBldTrgtEnbInfo 
 *
 * @param[in, out]  pdu: S1AP PDU
 * @param[in]   enbInfo: eNodeB Info.
 * @param[out]    enbIe: S1AP:target eNodeB inforamtion 
 * @return  S16
 *        -# Success : ROK
 */
PRIVATE S16 wrEmmMmeS1apBldTrgtEnbInfo
(
S1apPdu                      *pdu,
WrEmmMmeEnbInfo              *enbInfo,
SztTgeteNB_ID                *enbIe
)
{
   RLOG0(L_DEBUG,"Filling S1AP target ENB information");
   enbIe->pres.pres = PRSNT_NODEF;
   wrEmmMmeSztFillS1eNBId(pdu, enbInfo->enbType, enbInfo->cellId,
                          &enbInfo->plmnId, &enbIe->global_ENB_ID);
   wrEmmMmeSztFillTAI(pdu, enbInfo->tac, &enbInfo->plmnId, 
                                             &enbIe->selected_TAI);
   enbIe->iE_Extns.noComp.pres = NOTPRSNT;
   RETVALUE(ROK);
} /* end of wrEmmMmeS1apBldTrgtEnbInfo */

/** @brief This function fills the S1AP:Source EnodeB Inforamtion.
 *
 * @details This function fills the S1AP:Source EnodeB ID IE.
 *
 *     Function: wrEmmMmeS1apBldSrceEnbInfo
 *
 * @param[in, out]  pdu: S1AP PDU
 * @param[in]   enbInfo: eNodeB Info.
 * @param[out]    enbIe: S1AP:Source eNodeB inforamtion 
 * @return  S16
 *        -# Success : ROK
 */
PRIVATE S16 wrEmmMmeS1apBldSrceEnbInfo
(
S1apPdu                      *pdu,
WrEmmMmeEnbInfo              *enbInfo,
SztSrceNB_ID                 *enbIe
)
{
   RLOG0(L_DEBUG,"Filling S1AP source ENB information");
   enbIe->pres.pres = PRSNT_NODEF;
   wrEmmMmeSztFillS1eNBId(pdu, enbInfo->enbType, enbInfo->cellId,
                          &enbInfo->plmnId, &enbIe->global_ENB_ID);
   wrEmmMmeSztFillTAI(pdu, enbInfo->tac, &enbInfo->plmnId, 
                                             &enbIe->selected_TAI);
   enbIe->iE_Extns.noComp.pres = NOTPRSNT;
   RETVALUE(ROK);
} /* end of wrEmmMmeS1apBldSrceEnbInfo */

/** @brief This function fill the given ipv4 address in 
 *         S1AP transport layer address IE.
 *
 * @details This function fills the S1AP: Transport Layer Address IE.
 *
 *     Function: wrEmmMmeS1apBldIpv4Addr
 *
 * @param[in, out]  pdu: S1AP PDU   
 * @param[in]  ipv4Addr: IPv4 adress 
 * @param[out]   addrIe: S1AP transport layer address IE.
 * @return  S16
 *        -# Success : ROK
 *        -# Failure : RFAILED
 */
PRIVATE S16  wrEmmMmeS1apBldIpv4Addr
(
S1apPdu                      *pdu,
U32                          ipv4Addr,
SztTportLyrAddr              *addrIe 
)
{
   U8                        addrOctets[4];

   addrOctets[0] = (ipv4Addr & 0xff000000) >> 24;
   addrOctets[1] = (ipv4Addr & 0x00ff0000) >> 16;
   addrOctets[2] = (ipv4Addr & 0x0000ff00) >> 8;
   addrOctets[3] = (ipv4Addr & 0x000000ff);

   if (wrFillTknStrBSXL(addrIe, 32, addrOctets, (CmMemListCp*)pdu) != ROK)
   {
      RLOG0(L_ERROR, "Failed to fill IPV4 adress");
      RETVALUE(RFAILED);
   }
   RETVALUE(ROK);
} /* end of wrEmmMmeS1apBldIpv4Addr */

/** @brief This function fill the given ipv6 address in 
 *         S1AP transport layer address IE.
 *
 * @details This function fills the S1AP: Transport Layer Address IE.
 *
 *     Function: wrEmmMmeS1apBldIpv6Addr
 *
 * @param[in, out]  pdu: S1AP PDU   
 * @param[in]  ipv6Addr: IPv6 adress in binary notation 
 * @param[out]   addrIe: S1AP transport layer address IE.
 * @return  S16
 *        -# Success : ROK
 *        -# Failure : RFAILED
 */
PRIVATE S16  wrEmmMmeS1apBldIpv6Addr
(
S1apPdu                      *pdu,
U8                           *ipv6Addr,
SztTportLyrAddr              *addrIe 
)
{
   /* IPv6 Address size is 16 * 8 = 128 bits long */
   if (wrFillTknStrBSXL(addrIe, 128, ipv6Addr, (CmMemListCp*)pdu) != ROK)
   {
      RLOG0(L_ERROR, "Failed to fill IPV6 adress");
      RETVALUE(RFAILED);
   }
   RETVALUE(ROK);
} /* end of wrEmmMmeS1apBldIpv6Addr */


/* All initiating messages follow                                         */
/* ======================================= */
/* S1AP S1 SETUP REQUEST MESSAGE BUILDING  */
/* ======================================= */
/** @brief This function will allocate memory and build the 
 *         S1AP:S1 Setup Request Message.
 *
 * @details This function allocates the memory for S1AP: S1 Setup Request
 *          Message PDU and fills the PDU with proper values.
 *
 *     Function: wrEmmMmeBldSetupReq 
 *
 *         Processing steps:
 *         - Allocate Memory for S1-AP: EnodeB S1 Setup Request Message PDU.
 *         - Fill the PDU with proper values.
 *
 * @param[out] pdu: S1-AP: S1 Setup Request PDU. 
 * @return  S16
 *        -# Success : ROK
 *        -# Failure : RFAILED
 */
PUBLIC S16 wrEmmMmeBldSetupReq
(
S1apPdu                           **pdu
)
{
   S1apPdu                        *setupReqPdu = NULLP;
   U16                            numComp;
   S16                            ret;
   WrCellCb                       *cellCb = NULLP;
   SztProtIE_Field_S1SetupRqstIEs *ie;
   SztSuppTAs_Item                *tACItem;
   SztInitiatingMsg               *initMsg;
   U32                            numTAs;
   U32                            taIdx;
   U32                            idx;
   SztCSG_IdLst_Item              *csgItem = NULLP;
   U32                            ieIdx = 0;

   TRC2(wrEmmMmeSztBldSetupReq);

   /* Any cell is representative of eNB for some of its parameters        */
   cellCb = wrEmmCb.cellCb[0];
   RLOG0(L_DEBUG,"Constructing S1 setup message");

   RLOG_ARG1(L_DEBUG, DBG_CELLID,cellCb->cellId,"eNodeB ID: [%d]",
             wrEmmCb.enbId);
   RLOG0(L_DEBUG,"PLMN ID: ");
   RLOG3(L_DEBUG,"MCC [%d] [%d] [%d]",cellCb->sib1.plmns[0].plmnId.mcc[0],\
   cellCb->sib1.plmns[0].plmnId.mcc[1],cellCb->sib1.plmns[0].plmnId.mcc[2]);
   RLOG3(L_DEBUG,"MNC [%d] [%d] [%d]",cellCb->sib1.plmns[0].plmnId.mnc[0],\
   cellCb->sib1.plmns[0].plmnId.mnc[1],cellCb->sib1.plmns[0].plmnId.mnc[2]);
         
   RLOG1(L_DEBUG,"TAC : [%d]",cellCb->sib1.tac);
#ifdef SS_DIAG
   wrDiagPrntPlmnId(&(cellCb->sib1.plmns[0].plmnId),3);
#endif
   RLOG_ARG3(L_DEBUG, DBG_CELLID,cellCb->cellId,"SIB1 configured Q-RxLevMin[%d]"
      " Q-RxLevMin offset[%d] cellBarred[%d]", cellCb->sib1.qRxLevMin,
      cellCb->sib1.qRxLevMinOffset, cellCb->sib1.cellBarred);
   RLOG_ARG4(L_DEBUG, DBG_CELLID,cellCb->cellId,"SIB2 configured "
      "defaultPagingCycle[%d] nB[%d] rootSequenceIndx[%d] prach-ConfigIndex[%d]",
      cellCb->sib2.pcchCfg.defaultPagingCycle, cellCb->sib2.pcchCfg.nB,
      cellCb->sib2.prachCfg.rootSeqIdx, cellCb->sib2.prachCfg.prachCfgIdx);

   /* Initialize memory control point */
   ret = cmAllocEvnt(sizeof(S1apPdu), WR_SZ_MEM_SDU_SIZE, &wrCb.mem, 
                     (Ptr *)&setupReqPdu);
   if(ret != ROK)
   {
      RLOG0(L_FATAL,"Memory allocation failed "
      "while constructing S1 setup message");
      RETVALUE(RFAILED);
   }

   /*wrFillTkn functions will be referred from sz_utl.c file*/
   wrFillTknU8(&(setupReqPdu->pdu.choice), SZT_TRGR_INTI_MSG);
   initMsg = &(setupReqPdu->pdu.val.initiatingMsg);
   wrFillTknU8(&(initMsg->pres),PRSNT_NODEF);
   wrFillTknU32(&(initMsg->procedureCode), Sztid_S1Setup);
   wrFillTknU32(&(initMsg->criticality), SztCriticalityrejectEnum);
   wrFillTknU8(&(initMsg->value.u.sztS1SetupRqst.pres),PRSNT_NODEF);

   /* Filling mandatory IEs */
   if(cellCb->csgAccessMode == WR_ENB_CLOSED_ACCESS ||
      cellCb->csgAccessMode == WR_ENB_HYBRID_ACCESS)
   {
      numComp = 5;
   }
   else
   { 
      numComp = 4;
   }
   if ((cmGetMem(setupReqPdu, 
              numComp * sizeof(SztProtIE_Field_S1SetupRqstIEs),
             (Ptr*)&initMsg->value.u.sztS1SetupRqst.protocolIEs.member)) !=ROK)
   { 
      RLOG0(L_FATAL,"Memory allocation failed for S1 setup req IE's");
      WR_FREE_EVNT(setupReqPdu);
      RETVALUE(RFAILED);
   }

   /* IE1 - Filling ENB Global Id */
   ie = &initMsg->value.u.sztS1SetupRqst.protocolIEs.member[ieIdx];
   wrFillTknU8(&(ie->pres),PRSNT_NODEF);
   wrFillTknU32(&(ie->id),Sztid_Global_ENB_ID);
   wrFillTknU32(&(ie->criticality),SztCriticalityrejectEnum);
   wrFillTknU8(&(ie->value.u.sztGlobal_ENB_ID.pres), PRSNT_NODEF);
   wrEmmMmeSztFillS1eNBId(setupReqPdu, wrEmmCb.enbType, 
                          wrEmmCb.enbId,
                          &cellCb->sib1.plmns[0].plmnId,
                          &(ie->value.u.sztGlobal_ENB_ID));
   ieIdx++;

   /* IE2 - Filling ENB Name */
   ie = &initMsg->value.u.sztS1SetupRqst.protocolIEs.member[ieIdx];
   wrFillTknU8(&(ie->pres),PRSNT_NODEF);
   wrFillTknU32(&(ie->id),Sztid_eNBname);
   wrFillTknU32(&(ie->criticality),SztCriticalityignoreEnum);
   WR_GET_MEM(setupReqPdu, wrEmmCb.enbNameLen, &(ie->value.u.sztENBname.val));
   ie->value.u.sztENBname.len  = wrEmmCb.enbNameLen;
   if(wrEmmCb.enbNameLen <= WR_EMM_CB_ENB_NAME) /*KW Fix */
   {
      wrFillTknStrOSXL(&(ie->value.u.sztENBname), wrEmmCb.enbNameLen, 
            wrEmmCb.enbName, &(setupReqPdu->memCp));
   }
   else
   {
      wrFillTknStrOSXL(&(ie->value.u.sztENBname), WR_EMM_CB_ENB_NAME, 
            wrEmmCb.enbName, &(setupReqPdu->memCp));
   }
   ieIdx++;

   /* IE3 - Filling Supported TAs                                         */
   ie = &initMsg->value.u.sztS1SetupRqst.protocolIEs.member[ieIdx];
   wrFillTknU8(&(ie->pres),PRSNT_NODEF);
   wrFillTknU32(&(ie->id),Sztid_SuppTAs);
   wrFillTknU32(&(ie->criticality),SztCriticalityrejectEnum);
   /* TODO : Handle the case of multiple TAs/cells                        */
   numTAs = 1;
   wrFillTknU16(&(ie->value.u.sztSuppTAs.noComp), numTAs);
   if ((cmGetMem(setupReqPdu, numTAs * sizeof(SztSuppTAs_Item), 
                 (Ptr*)&(ie->value.u.sztSuppTAs.member))) !=ROK)
   {
      RLOG0(L_FATAL,"Memory allocation failed for support TAS");
      WR_FREE_EVNT(setupReqPdu);
      RETVALUE(RFAILED);
   }
   for( taIdx = 0; taIdx < numTAs; taIdx++)
   {
      tACItem = &ie->value.u.sztSuppTAs.member[taIdx];
      wrFillTknU8(&(tACItem->pres), PRSNT_NODEF);
      wrEmmMmeSztFillTAC(cellCb->sib1.tac, setupReqPdu, &tACItem->tAC);
      numComp = cellCb->sib1.numPlmnIds;
      /* Allocate memory for broadcast PLMNs */
      if((cmGetMem(setupReqPdu, numComp * sizeof(SztPLMNidentity),
                  (Ptr*)&(tACItem->broadcastPLMNs.member))) !=ROK)
      {
         RLOG0(L_FATAL, "Memory allocation failed for PLMN identity");
         WR_FREE_EVNT(setupReqPdu);
         RETVALUE(RFAILED);
      }
      wrFillTknU16(&(tACItem->broadcastPLMNs.noComp),numComp);
      for(idx = 0; idx < numComp; idx++)
      {
         wrEmmMmeSztFillPLMNId(setupReqPdu, &(cellCb->sib1.plmns[idx].plmnId),
                               &(tACItem->broadcastPLMNs.member[idx]));
         RLOG2(L_EVENT, "[S1AP]:Sending S1 SETUP REQUEST [CGI:%d] [Supported TAI:%d]",
               wrEmmCb.enbId, cellCb->sib1.tac);
      }
   }
   ieIdx++;

   ie = &initMsg->value.u.sztS1SetupRqst.protocolIEs.member[ieIdx];
   wrFillTknU8(&(ie->pres), PRSNT_NODEF);
   wrFillTknU32(&(ie->id), Sztid_DefaultPagDRX);
   wrFillTknU32(&(ie->value.u.sztPagDRX), cellCb->sib2.pcchCfg.defaultPagingCycle);
   wrFillTknU32(&(ie->criticality),SztCriticalityignoreEnum);
   ieIdx++;

 

   /* IE4 - Filling the CSGLst */
   if(cellCb->csgAccessMode == WR_ENB_CLOSED_ACCESS ||
      cellCb->csgAccessMode == WR_ENB_HYBRID_ACCESS)
   {
      ie = &initMsg->value.u.sztS1SetupRqst.protocolIEs.member[ieIdx];
      wrFillTknU8(&(ie->pres), PRSNT_NODEF);
      wrFillTknU32(&(ie->id), Sztid_CSG_IdLst);
      wrFillTknU32(&(ie->criticality),SztCriticalityrejectEnum);

      /* TODO : Handle the case of multiple CSG IDs                        */
      wrFillTknU16(&(ie->value.u.sztCSG_IdLst.noComp), 1);
      if((cmGetMem(setupReqPdu, sizeof(SztCSG_IdLst_Item),
                  (Ptr*)&(ie->value.u.sztCSG_IdLst.member))) !=ROK)
      {
         RLOG0(L_FATAL,"Memory allocation failed  for IDlst item");
         WR_FREE_EVNT(setupReqPdu);
         RETVALUE(RFAILED);
      }
      csgItem = &(ie->value.u.sztCSG_IdLst.member[0]);
      wrFillTknU8(&(csgItem->pres), cellCb->sib1.csgId.pres);
      wrFillTknBStr32(&(csgItem->cSG_Id), WR_S1AP_CSG_ID_LEN, cellCb->sib1.csgId.val);
      csgItem->iE_Extns.noComp.pres = NOTPRSNT;
      ieIdx++;
   }

   /* IE5 - Filling the PagDRX */
  /* Fill in the number of IEs included in the message */
   wrFillTknU16(&(initMsg->value.u.sztS1SetupRqst.protocolIEs.noComp), ieIdx);

   /* Pass the constructed PDU back to the caller */
   *pdu = setupReqPdu;

   RETVALUE(ROK);

} /* wrEmmMmeSztBldSetupReq */

/** @brief This function will allocate memory and build the
 *         S1AP: Enodeb Configuration Update message.
 *
 * @details This function allocates memory for S1AP:EnodeB Configuration Update
 *          Message PDU and fills the PDU with proper values.
 *
 *     Function: wrEmmMmeBldEnbCfgUpd
 *
 *         Processing steps:
 *         - Allocate Memory for S1-AP: EnodeB Configuration Update message PDU.
 *         - Fill the PDU with proper values.
 *
 * @param[out] pdu: S1-AP: ENB Confguration Update PDU. 
 * @return  S16
 *        -# Success : ROK
 *        -# Failure : RFAILED
 */
PUBLIC S16 wrEmmMmeBldEnbCfgUpd
(
S1apPdu                           **pdu
)
{
   S1apPdu                             *enbCfgUpdPdu = NULLP;
   U16                                 numComp;
   S16                                 ret;
   WrCellCb                            *cellCb = NULLP;
   SztProtIE_Field_ENBConfigUpdIEs     *ie;
   SztSuppTAs_Item                     *tACItem;
   SztInitiatingMsg                    *initMsg;
   U32                                 numTAs;
   U32                                 taIdx;
   U32                                 idx;
   SztCSG_IdLst_Item                   *csgItem = NULLP;
   U32                                 memIdx = 0;
   SztENBConfigUpd                     *cfgUpdIes;

   TRC2(wrEmmMmeBldEnbCfgUpd);

   *pdu   = NULLP;
   cellCb = wrEmmCb.cellCb[0];
   RLOG0(L_DEBUG,"Constructing eNodeB Cfg update message");

   /* Initialize memory control point */
   ret = cmAllocEvnt(sizeof(S1apPdu), WR_SZ_MEM_SDU_SIZE, &wrCb.mem, 
                     (Ptr *)&enbCfgUpdPdu);
   if(ret != ROK)
   {
      RLOG0(L_FATAL,"Memory allocation failed "
      "while constructing eNodeB Cfg update message");
      RETVALUE(RFAILED);
   }

   /*wrFillTkn functions will be referred from sz_utl.c file*/
   wrFillTknU8(&(enbCfgUpdPdu->pdu.choice), S1AP_PDU_INITIATINGMSG);
   initMsg = &(enbCfgUpdPdu->pdu.val.initiatingMsg);
   wrFillTknU8(&(initMsg->pres),PRSNT_NODEF);
   wrFillTknU32(&(initMsg->procedureCode), Sztid_ENBConfigUpd);
   wrFillTknU32(&(initMsg->criticality), SztCriticalityrejectEnum);
   cfgUpdIes = &initMsg->value.u.sztENBConfigUpd;
   wrFillTknU8(&(cfgUpdIes->pres), PRSNT_NODEF);

   /* Filling mandatory IEs */
   if(cellCb->sib1.csgInd)
   {
      numComp = 4;
   }
   else
   {
     numComp = 3;
   }
   wrFillTknU16(&(cfgUpdIes->protocolIEs.noComp), numComp);
   if ((cmGetMem(enbCfgUpdPdu, 
                 numComp * sizeof(SztProtIE_Field_S1SetupRqstIEs),
                 (Ptr*)&cfgUpdIes->protocolIEs.member)) !=ROK)
   { 
      RLOG0(L_FATAL,"Memory allocation failed for S1 setup request IE's");
      WR_FREE_EVNT(enbCfgUpdPdu);
      RETVALUE(RFAILED);
   }

   /* IE1 - Filling ENB Name */
   ie = &cfgUpdIes->protocolIEs.member[memIdx];
   wrFillTknU8(&(ie->pres),PRSNT_NODEF);
   wrFillTknU32(&(ie->id),Sztid_eNBname);
   wrFillTknU32(&(ie->criticality),SztCriticalityignoreEnum);
   WR_GET_MEM(enbCfgUpdPdu, wrEmmCb.enbNameLen, &(ie->value.u.sztENBname.val));
   ie->value.u.sztENBname.len  = wrEmmCb.enbNameLen;
   if(wrEmmCb.enbNameLen <= WR_EMM_CB_ENB_NAME) /*KW Fix */
   {
      wrFillTknStrOSXL(&(ie->value.u.sztENBname), wrEmmCb.enbNameLen, 
            wrEmmCb.enbName, &(enbCfgUpdPdu->memCp));
   }
   else
   {
      wrFillTknStrOSXL(&(ie->value.u.sztENBname), WR_EMM_CB_ENB_NAME, 
            wrEmmCb.enbName, &(enbCfgUpdPdu->memCp));
   }

   memIdx++;

   /* IE2 - Filling Supported TAs                                         */
   ie = &cfgUpdIes->protocolIEs.member[memIdx];
   wrFillTknU8(&(ie->pres),PRSNT_NODEF);
   wrFillTknU32(&(ie->id),Sztid_SuppTAs);
   wrFillTknU32(&(ie->criticality),SztCriticalityrejectEnum);
   /* TODO : Handle the case of multiple TAs/cells                        */
   numTAs = 1;
   wrFillTknU16(&(ie->value.u.sztSuppTAs.noComp),numTAs);
   if ((cmGetMem(enbCfgUpdPdu, numTAs * sizeof(SztSuppTAs_Item), 
                 (Ptr*)&(ie->value.u.sztSuppTAs.member))) !=ROK)
   {
      RLOG0(L_FATAL,"Memory allocation failed for TAS");
      WR_FREE_EVNT(enbCfgUpdPdu);
      RETVALUE(RFAILED);
   }
   for( taIdx = 0; taIdx < numTAs; taIdx++)
   {
      tACItem = &ie->value.u.sztSuppTAs.member[taIdx];
      wrFillTknU8(&(tACItem->pres),PRSNT_NODEF);
      wrEmmMmeSztFillTAC(cellCb->sib1.tac, enbCfgUpdPdu, &tACItem->tAC);
      numComp = cellCb->sib1.numPlmnIds;
      /* Allocate memory for broadcast PLMNs */

      if((cmGetMem(enbCfgUpdPdu, numComp * sizeof(SztPLMNidentity),
                  (Ptr*)&(tACItem->broadcastPLMNs.member))) !=ROK)
      {
         RLOG0(L_FATAL,"Memory allocation failed for PLMN Identity");
         WR_FREE_EVNT(enbCfgUpdPdu);
         RETVALUE(RFAILED);
      }
      wrFillTknU16(&(tACItem->broadcastPLMNs.noComp),numComp);
      for(idx = 0; idx < numComp; idx++)
      {
         wrEmmMmeSztFillPLMNId(enbCfgUpdPdu, &(cellCb->sib1.plmns[idx].plmnId),
                               &(tACItem->broadcastPLMNs.member[idx]));
      }
   }
   memIdx++;

   /* IE3 - Filling the CSGLst */
   if(cellCb->sib1.csgInd)
   {
      ie = &cfgUpdIes->protocolIEs.member[memIdx];
      wrFillTknU8(&(ie->pres), PRSNT_NODEF);
      wrFillTknU32(&(ie->id), Sztid_CSG_IdLst);
      wrFillTknU32(&(ie->criticality),SztCriticalityrejectEnum);
      numComp = 1;
      wrFillTknU16(&(ie->value.u.sztCSG_IdLst.noComp), numComp);
      if((cmGetMem(enbCfgUpdPdu, numComp * sizeof(SztCSG_IdLst_Item),
                  (Ptr*)&(ie->value.u.sztCSG_IdLst.member))) !=ROK)
      {
         RLOG0(L_FATAL,"Memory allocation failed for IDLst");
         WR_FREE_EVNT(enbCfgUpdPdu);
         RETVALUE(RFAILED);
      }
      csgItem = &(ie->value.u.sztCSG_IdLst.member[0]);
      wrFillTknU8(&(csgItem->pres), cellCb->sib1.csgId.pres);
      wrEmmMmeSztFillCsgId(cellCb->sib1.csgId.val, enbCfgUpdPdu, csgItem);
      csgItem->iE_Extns.noComp.pres = NOTPRSNT;
      memIdx++;
   }

   /* IE4 - Filling the PagDRX */
   ie = &cfgUpdIes->protocolIEs.member[memIdx];
   wrFillTknU8(&(ie->pres), PRSNT_NODEF);
   wrFillTknU32(&(ie->id), Sztid_DefaultPagDRX);
   if (cellCb->pbmCb.pendReCalcAllPfsAndPos)
   {
      wrFillTknU32(&(ie->value.u.sztPagDRX), cellCb->pbmCb.pendDfltPagingCycle);
   } else {
      wrFillTknU32(&(ie->value.u.sztPagDRX), cellCb->sib2.pcchCfg.defaultPagingCycle);
   }
   wrFillTknU32(&(ie->criticality),SztCriticalityignoreEnum);
   memIdx++;

   /* Pass the constructed PDU back to the caller */
   *pdu = enbCfgUpdPdu;

   RETVALUE(ROK);

} /* wrEmmMmeBldEnbCfgUpd */

/** @brief This function will allocate memory and build the
 *         S1AP: Enodeb Configuration Update Fail message.
 *
 * @details This function allocates memory for S1AP:EnodeB Configuration Update
 *          Fail Message PDU and fills the PDU with proper values.
 *
 *     Function: wrEmmMmeBldEnbCfgUpdFail
 *
 *         Processing steps:
 *         - Allocate Memory for S1-AP: EnodeB Configuration Update Fail message PDU.
 *         - Fill the PDU with proper values.
 *
 * @param[out] pdu: S1-AP: ENB Confguration Update Fail PDU. 
 * @return  S16
 *        -# Success : ROK
 *        -# Failure : RFAILED
 */
PUBLIC S16 wrEmmMmeBldEnbCfgUpdFail
(
S1apPdu                           **pdu,
WrUmmMsgCause                     *cause,
U8                                timeToWaitSecs
)
{
   S16                                  ret;
   S1apPdu                              *mmeCfgUpdFail;
   U16                                  numComp;
   SztProtIE_Field_MMEConfigUpdFailIEs  *ie = NULLP;

   /* Initialize memory control point */

   RLOG0(L_DEBUG,"Sending eNodeB configuration update fail");
   RLOG1(L_DEBUG,"Time to wait in Sec [%d]",timeToWaitSecs);
   RLOG2(L_DEBUG,"Cause Type [%d], Cause Value [%d] ",cause->causeTyp,cause->causeVal);
   ret = cmAllocEvnt(sizeof(S1apPdu), WR_SZ_MEM_SDU_SIZE, &wrCb.mem,
         (Ptr *)&mmeCfgUpdFail);

   if(ret != ROK)
   {
      RLOG0(L_FATAL, "Memory allocation failed "
      "while sending eNodeB configuration update fail");
      RETVALUE(RFAILED);
   }
   wrFillTknU8(&(mmeCfgUpdFail->pdu.choice), SZT_TRGR_UNSUCCESS_OUTCOME);
   wrFillTknU8(&(mmeCfgUpdFail->pdu.val.unsuccessfulOutcome.pres), PRSNT_NODEF);
   wrFillTknU32(&(mmeCfgUpdFail->pdu.val.unsuccessfulOutcome.procedureCode),
                  Sztid_MMEConfigUpd);
   wrFillTknU32(&(mmeCfgUpdFail->pdu.val.unsuccessfulOutcome.criticality),
                  SztCriticalityrejectEnum);
   wrFillTknU8(&(mmeCfgUpdFail->pdu.val.unsuccessfulOutcome.value.u.\
                sztMMEConfigUpdFail.pres), PRSNT_NODEF);

   /* Fill the two parameters */
   numComp = 2;
   wrFillTknU16(&(mmeCfgUpdFail->pdu.val.unsuccessfulOutcome.value.u.\
                 sztMMEConfigUpdFail.protocolIEs.noComp), numComp);

   if ((cmGetMem(mmeCfgUpdFail, numComp * sizeof(SztProtIE_Field_MMEConfigUpdFailIEs),\
               (Ptr*)&mmeCfgUpdFail->pdu.val.unsuccessfulOutcome.value.u.sztMMEConfigUpdFail.\
               protocolIEs.member)) !=ROK)
   {
      RLOG0(L_FATAL, "Memory allocation failed for MME cfg update fail IE's");
      WR_FREE_EVNT(mmeCfgUpdFail);
      RETVALUE(RFAILED);
   }
   ie = &(mmeCfgUpdFail->pdu.val.unsuccessfulOutcome.value.u.sztMMEConfigUpdFail.\
          protocolIEs.member[0]);

   wrFillTknU8(&(ie->pres), PRSNT_NODEF);
   wrFillTknU32(&(ie->id), Sztid_Cause);
   wrFillTknU32(&(ie->criticality), SztCriticalityignoreEnum);
   wrS1apFillCause(&ie->value.u.sztCause, cause);

   if (timeToWaitSecs != 0)
   {
     ie = &(mmeCfgUpdFail->pdu.val.unsuccessfulOutcome.value.u.sztMMEConfigUpdFail.\
          protocolIEs.member[1]);
      wrFillTknU8(&(ie->pres),PRSNT_NODEF);
      wrFillTknU32(&(ie->id),Sztid_TimeToWait);
      wrFillTknU32(&(ie->criticality),SztCriticalityignoreEnum);
      wrS1apFillTimeToWait(&ie->value.u.sztTimeToWait, 
                                timeToWaitSecs);
   }

   *pdu = mmeCfgUpdFail;

   RETVALUE(ROK);
}
 
/** @brief This function will allocate memory and build the
 *         S1AP: Enodeb Configuration Update Fail message.
 *
 * @details This function allocates memory for S1AP:EnodeB Configuration Update
 *          Fail Message PDU and fills the PDU with proper values.
 *
 *     Function: wrEmmMmeBldEnbErrInd
 *
 *         Processing steps:
 *         - Allocate Memory for S1-AP: EnodeB Configuration Update Fail message PDU.
 *         - Fill the PDU with proper values.
 *
 * @param[out] pdu: S1-AP: ENB Confguration Update Fail PDU. 
 * @return  S16
 *        -# Success : ROK
 *        -# Failure : RFAILED
 */
PUBLIC S16 wrEmmMmeBldEnbErrInd
(
S1apPdu                           **pdu,
WrUmmMsgCause                     *cause,
U32                               *eNbUeId,
U32                               *mmeUeId
)
{
   U16                         noComp = 1;
   S1apPdu                    *errPdu;
   SztProtIE_Field_ErrIndIEs  *ie;
   U16                         idx = 0;
   S16                         ret;

   RLOG0(L_DEBUG,"Constructing eNodeB error ind");
   if(eNbUeId != NULLP)
   {
      noComp++;
   }
   if(mmeUeId != NULLP)
   {
      noComp++;
   }

    ret = cmAllocEvnt(sizeof(S1apPdu), SZ_MEM_SDU_SIZE, &wrCb.mem,
                     (Ptr *)&errPdu);

   if(ret != ROK)
   {
      RLOG0(L_FATAL, "Memory allocation failed "
      "while constructing eNodeB error ind");
      RETVALUE(RFAILED);
   }

   /* Fill the PDU with the procedure informarion */
   wrFillTknU8(&(errPdu->pdu.choice), SZT_TRGR_INTI_MSG);
   wrFillTknU8(&(errPdu->pdu.val.initiatingMsg.pres), PRSNT_NODEF);
   wrFillTknU32(&(errPdu->pdu.val.initiatingMsg.procedureCode), Sztid_ErrInd);
   wrFillTknU32(&(errPdu->pdu.val.initiatingMsg.criticality),
                 SztCriticalityignoreEnum);

   wrFillTknU8(&(errPdu->pdu.val.initiatingMsg.value.u.sztErrInd.pres),
                 PRSNT_NODEF);

   wrFillTknU16(&(errPdu->pdu.val.initiatingMsg.value.u.sztErrInd.protocolIEs.\
                  noComp), noComp);

   if ((cmGetMem(errPdu, (noComp * sizeof(SztProtIE_Field_ErrIndIEs)),
                 (Ptr*)&(errPdu->pdu.val.initiatingMsg.value.u.sztErrInd.\
                 protocolIEs.member))) != ROK)
   {
      RLOG0(L_FATAL,": Memory allocation failed for err ind IE's");
      RETVALUE(RFAILED);
   }

   ie = &(errPdu->pdu.val.initiatingMsg.value.u.sztErrInd.protocolIEs.\
          member[idx]);

   if(mmeUeId != NULLP)
   {
      RLOG1(L_DEBUG,"MME UE ID Is: [%d]",*mmeUeId);
      wrFillTknU8(&(ie->pres), PRSNT_NODEF);
      wrFillTknU32(&(ie->id), Sztid_MME_UE_S1AP_ID);
      wrFillTknU32(&(ie->criticality), SztCriticalityignoreEnum);
      wrFillTknU32(&(ie->value.u.sztMME_UE_S1AP_ID), *mmeUeId);
      idx++;
   }

   ie = &(errPdu->pdu.val.initiatingMsg.value.u.sztErrInd.protocolIEs.\
          member[idx]);

   if(eNbUeId != NULLP)
   {
      RLOG1(L_DEBUG,"ENODEB UE ID Is: [%d]",*eNbUeId);
      wrFillTknU8(&(ie->pres), PRSNT_NODEF);
      wrFillTknU32(&(ie->id), Sztid_eNB_UE_S1AP_ID);
      wrFillTknU32(&(ie->criticality), SztCriticalityignoreEnum);
      wrFillTknU32(&(ie->value.u.sztENB_UE_S1AP_ID), *eNbUeId);
      idx++;
   }

   ie = &(errPdu->pdu.val.initiatingMsg.value.u.sztErrInd.protocolIEs.\
          member[idx]);

   wrFillTknU8(&(ie->pres), PRSNT_NODEF);
   wrFillTknU32(&(ie->id), Sztid_Cause);
   wrFillTknU32(&(ie->criticality), SztCriticalityignoreEnum);
   wrFillTknU8(&(ie->value.u.sztCause.choice), cause->causeTyp);
   WR_INC_KPI_FUNC3(wrIncKpiFrS1apFailCause, cause->causeTyp, cause->causeVal, INC_KPI_VALUE_BY_ONE);
   switch (cause->causeTyp)
   {
      case CAUSE_RADIONW:
         wrFillTknU32(&(ie->value.u.sztCause.val.radioNw), cause->causeVal);
         break;
      case CAUSE_TRANSPORT:
         wrFillTknU32(&(ie->value.u.sztCause.val.transport), cause->causeVal);
         break;
      case CAUSE_NAS:
         wrFillTknU32(&(ie->value.u.sztCause.val.nas), cause->causeVal);
         break;
      case CAUSE_MISC:
         wrFillTknU32(&(ie->value.u.sztCause.val.misc), cause->causeVal);
         break;
  }
  *pdu = errPdu;

   RETVALUE(ROK);
}

/** @brief This function allocates the memory for EnodeB Configuration Transfer
 *         Message PDU and fills the PDU with proper values.
 *
 * @details This function allocates the memory for EnodeB Configuration Transfer
 *         Message PDU and fills the PDU with proper values.
 *
 *     Function: wrEmmMmeBldEnbCfgTrnsfr 
 *
 *         Processing steps:
 *         - Allocate Memory for S1-AP: EnodeB Configuration Transfer message PDU.
 *         - Fill the PDU with proper values.
 *
 * @param[out]       pdu: S1AP:ENB configuration transfer message PDU.
 * @param[in] trnsfrInfo: ENodeB configuration transfer info. 
 * @return  S16
 *        -# Success : ROK
 *        -# Failure : RFAILED
 */
PUBLIC S16 wrEmmMmeBldEnbCfgTrnsfr
(
S1apPdu                      **pdu,
WrEmmMmeEnbCfgTrnsfr         *trnsfrInfo
)
{
   S1apPdu                                 *cfgTrnsfrPdu = NULLP;
   SztInitiatingMsg                        *initMsg;
   U32                                     memIdx = 0;
   SztENBConfigTfr                         *cfgTrnsfrIes;
   SztProtIE_Field_ENBConfigTfrIEs         *ie;
   SztSONConfigTfr                         *sonCont;
   SztProtExtnField_SONConfigTfr_ExtIEs    *sonExtIE;
   SztX2TNLConfigInfo                      *x2TnlCfgInfo;
   SztProtExtnField_X2TNLConfigInfo_ExtIEs *x2TnlExtIE;
   SztENBX2ExtTLA                          *x2ExtTLA;
   WrMmeCb                                 *mmeCb;
   U8                                      mmeIpType;

   TRC2(wrEmmMmeBldEnbCfgTrnsfr);

   RLOG0(L_DEBUG,"Constructing eNodeB config transfer message");

   RLOG_ARG0(L_EVENT, DBG_ENBID, wrEmmCb.enbId,"[S1AP]:Sending ENB CONFIGURATION TRANSFER");
   /* Initialize memory control point */
   if (cmAllocEvnt(sizeof(S1apPdu), WR_SZ_MEM_SDU_SIZE, &wrCb.mem, 
                                             (Ptr *)&cfgTrnsfrPdu) != ROK)
   {
      RLOG0(L_FATAL, "Memory allocation failed "
      "while constructing eNodeB config transfer message");
      RETVALUE(RFAILED);
   }
   /* Fetch MME CB */
   mmeCb = wrEmmMmeGetMmeByPlmn(&trnsfrInfo->targetInfo.plmnId);
   if(mmeCb == NULLP)
   {
      RLOG0(L_FATAL,"wrEmmMmeBldEnbCfgTrnsfr: MME CB fetch failed");
      RETVALUE(RFAILED);
   }
   mmeIpType = mmeCb->mmeAddr[0].type;
   *pdu = cfgTrnsfrPdu;

   /*wrFillTkn functions will be referred from sz_utl.c file*/
   wrFillTknU8(&(cfgTrnsfrPdu->pdu.choice), S1AP_PDU_INITIATINGMSG);
   initMsg = &(cfgTrnsfrPdu->pdu.val.initiatingMsg);
   wrFillTknU8(&(initMsg->pres),PRSNT_NODEF);
   wrFillTknU32(&(initMsg->procedureCode), Sztid_eNBConfigTfr);
   wrFillTknU32(&(initMsg->criticality), SztCriticalityignoreEnum);
   cfgTrnsfrIes = &initMsg->value.u.sztENBConfigTfr;
   wrFillTknU8(&(cfgTrnsfrIes->pres), PRSNT_NODEF);

   wrFillTknU16(&(cfgTrnsfrIes->protocolIEs.noComp), 1);
   if ((cmGetMem(cfgTrnsfrPdu, 
                 sizeof(SztProtIE_Field_ENBConfigTfrIEs), 
                 (Ptr*)&cfgTrnsfrIes->protocolIEs.member)) != ROK)
   { 
      cmFreeMem(cfgTrnsfrPdu);
      *pdu = NULLP;
      RLOG0(L_FATAL,"Memory allocation failed for eNodeB Cfg Transfer IE's");
      RETVALUE(RFAILED);
   }

   ie = &cfgTrnsfrIes->protocolIEs.member[memIdx];
   wrFillTknU8(&(ie->pres), PRSNT_NODEF);
   wrFillTknU32(&(ie->id), Sztid_SONConfigTfrECT);
   wrFillTknU32(&(ie->criticality), SztCriticalityignoreEnum);
   sonCont = &ie->value.u.sztSONConfigTfr;
   sonCont->pres.pres = PRSNT_NODEF;
   wrEmmMmeS1apBldTrgtEnbInfo(*pdu, &trnsfrInfo->targetInfo, 
                                                     &sonCont->targeteNB_ID);
   wrEmmMmeS1apBldSrceEnbInfo(*pdu, &trnsfrInfo->sourceInfo,
                                                     &sonCont->sourceeNB_ID);
   if (trnsfrInfo->isReply == TRUE)
   {
      SztSONInformReply      *reply = &sonCont->sONInform.val.sONInformReply;
      SztENBX2TLAs           *TLAs;
      wrFillTknU8(&sonCont->sONInform.choice, SONINFORM_SONINFORMREPLY);
      reply->pres.pres = PRSNT_NODEF;
      reply->x2TNLConfigInfo.pres.pres = PRSNT_NODEF;
      TLAs = & reply->x2TNLConfigInfo.eNBX2TportLyrAddres;
      wrFillTknU16(&TLAs->noComp, 1);
      if ((cmGetMem(cfgTrnsfrPdu, sizeof(SztTportLyrAddr), 
                                              (Ptr*)&TLAs->member)) != ROK)
      {
         cmFreeMem(cfgTrnsfrPdu);
         *pdu = NULLP;
         RLOG0(L_FATAL, "Memory allocation failed for Transport Lyr Addr");
         RETVALUE(RFAILED);
      }

      if((CM_TPTADDR_IPV4 == wrEmmCb.datAppAddr[WR_IPV4_IDX].type) &&
         (mmeIpType == wrEmmCb.datAppAddr[WR_IPV4_IDX].type))
      {
         wrEmmMmeS1apBldIpv4Addr(*pdu, 
           wrEmmCb.datAppAddr[WR_IPV4_IDX].u.ipv4TptAddr.address,
           &TLAs->member[0]);
      }
      else if((CM_TPTADDR_IPV6 == wrEmmCb.datAppAddr[WR_IPV6_IDX].type) &&
         (mmeIpType == wrEmmCb.datAppAddr[WR_IPV6_IDX].type))
      {
         wrEmmMmeS1apBldIpv6Addr(*pdu,
           (U8 *) &wrEmmCb.datAppAddr[WR_IPV6_IDX].u.ipv6TptAddr.ipv6NetAddr, 
           &TLAs->member[0]);
      }
      reply->x2TNLConfigInfo.iE_Extns.noComp.pres = NOTPRSNT;
      reply->iE_Extns.noComp.pres = NOTPRSNT;
   }
   else
   {
      SztSONInformRqst       *rqst = &sonCont->sONInform.val.sONInformRqst;
      wrFillTknU8(&sonCont->sONInform.choice, SONINFORM_SONINFORMRQST);
      wrFillTknU32(rqst, SztSONInformRqstx2TNL_Config_InfoEnum);
   }

   /* Fill the Address information for access control and pass it to neighbor
    * so that access control can be applied by neighbor
   sonCont->iE_Extns.noComp.pres = NOTPRSNT;
    */
   wrFillTknU16(&(sonCont->iE_Extns.noComp), WR_VAL_ONE);

   if ((cmGetMem(cfgTrnsfrPdu, (sonCont->iE_Extns.noComp.val * 
                 sizeof(SztProtExtnField_SONConfigTfr_ExtIEs)), 
                 (Ptr*)&sonCont->iE_Extns.member)) != ROK)
   { 
      cmFreeMem(cfgTrnsfrPdu);
      *pdu = NULLP;
      RLOG0(L_FATAL,"Memory allocation failed for SON cfg transfer");
      RETVALUE(RFAILED);
   }

   sonExtIE = &(sonCont->iE_Extns.member[0]);
   wrFillTknU8(&(sonExtIE->pres), PRSNT_NODEF);
   wrFillTknU32(&(sonExtIE->id), Sztid_x2TNLConfigInfo);
   wrFillTknU32(&(sonExtIE->criticality), SztCriticalityignoreEnum);

   x2TnlCfgInfo = &(sonExtIE->extensionValue.u.sztX2TNLConfigInfo);
   wrFillTknU8(&(x2TnlCfgInfo->pres), PRSNT_NODEF);

   wrFillTknU16(&(x2TnlCfgInfo->eNBX2TportLyrAddres.noComp), WR_VAL_ONE);

   if ((cmGetMem(cfgTrnsfrPdu, 
                 (sizeof(SztTportLyrAddr) * 
                  x2TnlCfgInfo->eNBX2TportLyrAddres.noComp.val), 
                 (Ptr*)&(x2TnlCfgInfo->eNBX2TportLyrAddres.member))) != ROK)
   { 
      cmFreeMem(cfgTrnsfrPdu);
      *pdu = NULLP;
      RLOG0(L_FATAL,"Memory allocation failed for transport lyr addr");
      RETVALUE(RFAILED);
   }

   if((CM_TPTADDR_IPV4 == wrEmmCb.datAppAddr[WR_IPV4_IDX].type) && 
      (mmeIpType == wrEmmCb.datAppAddr[WR_IPV4_IDX].type))
   {
      wrEmmMmeS1apBldIpv4Addr(cfgTrnsfrPdu, 
        wrEmmCb.datAppAddr[WR_IPV4_IDX].u.ipv4TptAddr.address,
        (&x2TnlCfgInfo->eNBX2TportLyrAddres.member[0]));
   }
   else if((CM_TPTADDR_IPV6 == wrEmmCb.datAppAddr[WR_IPV6_IDX].type) &&
      (mmeIpType == wrEmmCb.datAppAddr[WR_IPV6_IDX].type))
   {
      wrEmmMmeS1apBldIpv6Addr(cfgTrnsfrPdu, 
        (U8 *) &wrEmmCb.datAppAddr[WR_IPV6_IDX].u.ipv6TptAddr.ipv6NetAddr,
        (&x2TnlCfgInfo->eNBX2TportLyrAddres.member[0]));
   }

   wrFillTknU16(&(x2TnlCfgInfo->iE_Extns.noComp), WR_VAL_ONE);

   if ((cmGetMem(cfgTrnsfrPdu, 
                 (sizeof(SztProtExtnField_X2TNLConfigInfo_ExtIEs) * 
                  x2TnlCfgInfo->iE_Extns.noComp.val),
                 (Ptr*)&(x2TnlCfgInfo->iE_Extns.member))) != ROK)
   { 
      cmFreeMem(cfgTrnsfrPdu);
      *pdu = NULLP;
      RLOG0(L_FATAL,"Memory allocation failed for config ext IE's");
      RETVALUE(RFAILED);
   }

   x2TnlExtIE = &(x2TnlCfgInfo->iE_Extns.member[0]);

   wrFillTknU8(&(x2TnlExtIE->pres), PRSNT_NODEF);
   wrFillTknU32(&(x2TnlExtIE->id), Sztid_eNBX2ExtendedTportLyrAddres);
   wrFillTknU32(&(x2TnlExtIE->criticality), SztCriticalityignoreEnum);

   wrFillTknU16(&(x2TnlExtIE->extensionValue.u.sztENBX2ExtTLAs.noComp), WR_VAL_ONE);

   if ((cmGetMem(cfgTrnsfrPdu, 
                 (sizeof(SztENBX2ExtTLA) *
                  x2TnlExtIE->extensionValue.u.sztENBX2ExtTLAs.noComp.val), 
       (Ptr*)&(x2TnlExtIE->extensionValue.u.sztENBX2ExtTLAs.member))) != ROK)
   { 
      cmFreeMem(cfgTrnsfrPdu);
      *pdu = NULLP;
      RLOG0(L_FATAL,"Memory allocation failed for transport lyr addr");
      RETVALUE(RFAILED);
   }

   x2ExtTLA = &(x2TnlExtIE->extensionValue.u.sztENBX2ExtTLAs.member[0]);
   wrFillTknU8(&(x2ExtTLA->pres), PRSNT_NODEF);
   x2ExtTLA->iPsecTLA.pres = NOTPRSNT;

   wrFillTknU16(&(x2ExtTLA->gTPTLAa.noComp), WR_VAL_ONE);

   if ((cmGetMem(cfgTrnsfrPdu, 
                 (sizeof(SztTportLyrAddr) *
                  x2ExtTLA->gTPTLAa.noComp.val), 
                 (Ptr*)&(x2ExtTLA->gTPTLAa.member))) != ROK)
   { 
      cmFreeMem(cfgTrnsfrPdu);
      *pdu = NULLP;
      RLOG0(L_FATAL,"Memory allocation failed for transport lyr addr");
      RETVALUE(RFAILED);
   }

   /* As of now the TeNB femto supports only one address for the X2 Signalling
    * and GTPU tunnel. Hence filling the same address for both X2 signalling
    * address and GTP tunnel address
    */
   if((CM_TPTADDR_IPV4 ==  wrEmmCb.datAppAddr[WR_IPV4_IDX].type) &&
      (mmeIpType == wrEmmCb.datAppAddr[WR_IPV4_IDX].type))
   {
      wrEmmMmeS1apBldIpv4Addr(cfgTrnsfrPdu, 
        wrEmmCb.datAppAddr[WR_IPV4_IDX].u.ipv4TptAddr.address,
        (&x2ExtTLA->gTPTLAa.member[0]));
   }
   else if((CM_TPTADDR_IPV6 ==  wrEmmCb.datAppAddr[WR_IPV6_IDX].type) &&
           (mmeIpType == wrEmmCb.datAppAddr[WR_IPV6_IDX].type))
   {
      wrEmmMmeS1apBldIpv6Addr(cfgTrnsfrPdu, 
        (U8 *)&wrEmmCb.datAppAddr[WR_IPV6_IDX].u.ipv6TptAddr.ipv6NetAddr,
        (&x2ExtTLA->gTPTLAa.member[0]));
   }
   
   x2ExtTLA->iE_Extns.noComp.pres = NOTPRSNT;

   RETVALUE(ROK);
} /* end of wrEmmMmeBldEnbCfgTrnsfr */

/* All successful outcome messages frome here                             */
/** @brief This function allocates memory and build the
 *         S1AP:MME configure update Acknowledgement message.
 *
 * @details This function allocates memory for S1AP: MME Configuration Update
 *          Acknowledgement message and fills the proper values.
 *
 *     Function: wrEmmMmeBldMmeCfgUpdAck
 *
 *         Processing steps:
 *         - Allocate Memory for S1-AP: Write Replacement Warning Response PDU.
 *         - Fill the PDU with proper values.
 *
 * @param[out] pdu: MME configure Update Ack PDU 
 * @return  S16
 *        -# Success : ROK
 *        -# Failure : RFAILED
 */
PUBLIC S16 wrEmmMmeBldMmeCfgUpdAck
(
S1apPdu                           **pdu
)
{
   S1apPdu                        *mmeCfgUpdAck;
   SztSuccessfulOutcome           *succMsg;
   SztMMEConfigUpdAckg            *updAckIes;

   RLOG0(L_DEBUG,"Constructing MME configuration update ACK");
   /* Initialize memory control point */
   if(cmAllocEvnt(sizeof(S1apPdu), WR_SZ_MEM_SDU_SIZE, &wrCb.mem, 
                                           (Ptr *)&mmeCfgUpdAck) != ROK)
   {
      RLOG0(L_FATAL,"Memory allocation failed "
      "while Constructing MME configuration update ACK");
      RETVALUE(RFAILED);
   }
   *pdu = mmeCfgUpdAck;

   /*wrFillTkn functions will be referred from sz_utl.c file*/
   wrFillTknU8(&(mmeCfgUpdAck->pdu.choice), S1AP_PDU_SUCCESSFULOUTCOME);
   succMsg = &(mmeCfgUpdAck->pdu.val.successfulOutcome);
   wrFillTknU8(&(succMsg->pres),PRSNT_NODEF);
   wrFillTknU32(&(succMsg->procedureCode), Sztid_MMEConfigUpd);
   wrFillTknU32(&(succMsg->criticality), SztCriticalityrejectEnum);
   updAckIes = &succMsg->value.u.sztMMEConfigUpdAckg;
   wrFillTknU8(&(updAckIes->pres), PRSNT_NODEF);
   wrFillTknU16(&(updAckIes->protocolIEs.noComp), 0);

   RETVALUE(ROK);
} /* end of wrEmmMmeBldMmeCfgUpdAck */

/* TODO: Additionally, we need to support broadcast area list             */
/** @brief This function is used to allocate memory and build the 
 *         S1AP:Write replaceemnnt warnig response message.
 *
 * @details This function allocates the memory for S1AP:Write-Replacement
 *          Warning Response PDU and fills the PDU with Porper values.
 *
 *     Function: wrEmmMmeBldWriteRepResp
 *
 *         Processing steps:
 *         - Allocate Memory for S1-AP: Write Replacement Warning Response PDU.
 *         - Fill the PDU with proper values.
 *
 * @param[out]      pdu: Write Replacement Warning Response PDU.
 * @param[in] serialNum: serail number same as received in write replacement
 *                       warning request. 
 * @return  S16
 *        -# Success : ROK
 *        -# Failure : RFAILED
 */
PUBLIC S16 wrEmmMmeBldWriteRepResp
(
S1apPdu                           **pdu,
U16                               serialNum
)
{
   S1apPdu                                    *writeRepResp;
   SztSuccessfulOutcome                       *succMsg;
   SztWriteReplaceWarningResp                 *writeRepRspIes;
   SztProtIE_Field_WriteReplaceWarningRespIEs *ie;
   
   RLOG0(L_DEBUG,"Constructing S1AP write replacement response");
   /* Initialize memory control point */
   if(cmAllocEvnt(sizeof(S1apPdu), WR_SZ_MEM_SDU_SIZE, &wrCb.mem, 
                                                (Ptr *)&writeRepResp) != ROK)
   {
      RLOG0(L_FATAL,"Memory allocation failed while"
      "constructing S1AP write replacement response");
      RETVALUE(RFAILED);
   }
   *pdu = writeRepResp;

   /*wrFillTkn functions will be referred from sz_utl.c file*/
   wrFillTknU8(&(writeRepResp->pdu.choice), S1AP_PDU_SUCCESSFULOUTCOME);
   succMsg = &(writeRepResp->pdu.val.successfulOutcome);
   wrFillTknU8(&(succMsg->pres),PRSNT_NODEF);
   wrFillTknU32(&(succMsg->procedureCode), Sztid_MMEConfigUpd);
   wrFillTknU32(&(succMsg->criticality), SztCriticalityrejectEnum);
   writeRepRspIes = &succMsg->value.u.sztWriteReplaceWarningResp;
   wrFillTknU8(&(writeRepRspIes->pres), PRSNT_NODEF);

   /* Add protocol IEs for this event structure                           */
   wrFillTknU16(&(writeRepRspIes->protocolIEs.noComp), 1);
   if((cmGetMem(writeRepResp, 
                sizeof(SztProtIE_Field_WriteReplaceWarningRespIEs),
                (Ptr*)&(writeRepRspIes->protocolIEs.member))) !=ROK)
   {
      RLOG0(L_FATAL,"Memory allocation failed for "
      "write replace warning rsp IE's");
      RETVALUE(RFAILED);
   }

   ie = &writeRepRspIes->protocolIEs.member[0];
   wrFillTknU8(&(ie->pres), PRSNT_NODEF);
   wrFillTknU32(&(ie->criticality),SztCriticalityignoreEnum);
   wrFillTknU32(&(ie->id), Sztid_SerialNumber);
   wrFillTknBStr32(&(ie->value.u.sztSerialNumber), WR_SER_NUM_LEN, serialNum);

   RETVALUE(ROK);
} /* end of wrEmmMmeBldWriteRepResp */

/* TODO: Additionally, we need to support broadcast area list             */
/** @brief This function is used to allocate memory and build the 
 *         S1AP:KILL response message.
 *
 * @details This function is used to allocate memory for S1AP:KILL Response 
 *          PDU and fills the PDU with proper values.
 *
 *     Function:wrEmmMmeBldKillRsp 
 *
 *         Processing steps:
 *         - Allocate Memory for S1-AP: KILL Response PDU.
 *         - Fill the PDU with proper values.
 *
 * @param[out]      pdu: KILL response PDU.
 * @param[in] serialNum: serail number same as received in Kill request. 
 * @return  S16
 *        -# Success : ROK
 *        -# Failure : RFAILED
 */
PUBLIC S16 wrEmmMmeBldKillRsp
(
S1apPdu                         **pdu,
U16                             serialNum
)
{
   S1apPdu                      *killRsp;
   SztSuccessfulOutcome         *succMsg;
   SztKillResp                  *killRspIes;
   SztProtIE_Field_KillRespIEs  *ie;

   RLOG0(L_DEBUG,"Constructing S1AP kill response");
   /* Initialize memory control point */
   if (cmAllocEvnt(sizeof(S1apPdu), WR_SZ_MEM_SDU_SIZE, &wrCb.mem, 
                                                     (Ptr *)&killRsp) != ROK)
   {
      RLOG0(L_FATAL,"Memory allocation failed while "
      "constructing S1AP kill response");
      RETVALUE(RFAILED);
   }
   *pdu = killRsp;

   /*wrFillTkn functions will be referred from sz_utl.c file*/
   wrFillTknU8(&(killRsp->pdu.choice), S1AP_PDU_SUCCESSFULOUTCOME);
   succMsg = &(killRsp->pdu.val.successfulOutcome);
   wrFillTknU8(&(succMsg->pres),PRSNT_NODEF);
   wrFillTknU32(&(succMsg->procedureCode), Sztid_MMEConfigUpd);
   wrFillTknU32(&(succMsg->criticality), SztCriticalityrejectEnum);
   killRspIes = &succMsg->value.u.sztKillResp;
   wrFillTknU8(&(killRspIes->pres), PRSNT_NODEF);

   /* Add protocol IEs for this event structure                           */
   wrFillTknU16(&(killRspIes->protocolIEs.noComp), 1);
   if((cmGetMem(killRspIes, sizeof(SztProtIE_Field_WriteReplaceWarningRespIEs),
                (Ptr*)&(killRspIes->protocolIEs.member))) !=ROK)
   {
      RLOG0(L_FATAL,"Memory allocation failed "
      "for write replace warning rsp IE's");
      RETVALUE(RFAILED);
   }

   ie = &killRspIes->protocolIEs.member[0];
   wrFillTknU8(&(ie->pres), PRSNT_NODEF);
   wrFillTknU32(&(ie->criticality),SztCriticalityignoreEnum);
   wrFillTknU32(&(ie->id), Sztid_SerialNumber);
   wrFillTknBStr32(&(ie->value.u.sztSerialNumber), WR_SER_NUM_LEN, serialNum);

   RETVALUE(ROK);
} /* end of wrEmmMmeBldKillRsp */

/** @brief This function Fill the Reset Message
 *
 *
 * @details This function allocates the memory for Reset message and fills
 *          the proper values bases on the Reset Type.
 *
 *     Function: wrEmmMmeBldResetReq 
 *
 *         Processing steps:
 *         - Allocate Memory for S1-AP: RESET Req PDU.
 *         - Fills the PDU based on RstType.
 *
 * @param[in] pdu        : Reset Type and cause.
 * @param[out] rstAckPdu : Pointer to reset Req pdu
 * @return  S16
 *        -# Success : ROK
 *        -# Failure : RFAILED
 */
PUBLIC S16 wrEmmMmeBldResetReq
(
S1apPdu      **pdu,
U8           rstType,
U16          numCons,
U8           choice,
U32          cause,
WrEmmS1ConId   *conIds
)
{
   S1apPdu     *reset;
   S16         ret;
   U16         noComp;
   U16         idx;
   SztProtIE_Field_ResetIEs *ie;
   SztProtIE_SingleCont_UE_assocLogS1_ConItemRes *ie1;

   TRC2(wrEmmMmeBldResetReq);
   
   RLOG0(L_DEBUG,"Constructing S1AP reset request");

   ret = cmAllocEvnt(sizeof(S1apPdu), SZ_MEM_SDU_SIZE, &wrCb.mem,
                     (Ptr *)&reset);

   if(ret != ROK)
   {
      RLOG0(L_FATAL,"Memory allocation failed "
      "while constructing S1AP reset request");
      RETVALUE(RFAILED);
   }

   /* Fill the PDU with the procedure informarion */
   wrFillTknU8(&(reset->pdu.choice), SZT_TRGR_INTI_MSG);
   wrFillTknU8(&(reset->pdu.val.initiatingMsg.pres),PRSNT_NODEF);
   wrFillTknU32(&(reset->pdu.val.initiatingMsg.procedureCode), Sztid_Reset);
   wrFillTknU32(&(reset->pdu.val.initiatingMsg.criticality),
                 SztCriticalityrejectEnum);

   wrFillTknU8(&(reset->pdu.val.initiatingMsg.value.u.sztReset.pres),
                 PRSNT_NODEF);

   noComp = 2;

   wrFillTknU16(&(reset->pdu.val.initiatingMsg.value.u.sztReset.protocolIEs.\
                  noComp), noComp);

   if ((cmGetMem(reset, (noComp * sizeof(SztProtIE_Field_ResetIEs)),
                 (Ptr *)&reset->pdu.val.initiatingMsg.value.u.sztReset.\
                 protocolIEs.member)) != ROK)
   {
      RLOG0(L_FATAL,"Memory allocation failed for reset IE's");
      RETVALUE(RFAILED);
   }

   ie = &(reset->pdu.val.initiatingMsg.value.u.sztReset.protocolIEs.\
          member[0]);

   wrFillTknU8(&(ie->pres), PRSNT_NODEF);
   wrFillTknU32(&(ie->id), Sztid_Cause);
   wrFillTknU32(&(ie->criticality), SztCriticalityignoreEnum);
   WR_INC_KPI_FUNC3(wrIncKpiFrS1apFailCause, (U32)choice, cause, INC_KPI_VALUE_BY_ONE);
   switch (choice)
   {
      case CAUSE_RADIONW:
         wrFillTknU8(&(ie->value.u.sztCause.choice), choice);
         wrFillTknU32(&(ie->value.u.sztCause.val.radioNw), cause);
         break;
      case CAUSE_TRANSPORT:
         wrFillTknU8(&(ie->value.u.sztCause.choice), choice);
         wrFillTknU32(&(ie->value.u.sztCause.val.radioNw), cause);
         break;
      case CAUSE_NAS:
         wrFillTknU8(&(ie->value.u.sztCause.choice), choice);
         wrFillTknU32(&(ie->value.u.sztCause.val.radioNw), cause);
         break;
      case CAUSE_MISC:
         wrFillTknU8(&(ie->value.u.sztCause.choice), choice);
         wrFillTknU32(&(ie->value.u.sztCause.val.radioNw), cause);
         break;
   }

   ie = &(reset->pdu.val.initiatingMsg.value.u.sztReset.protocolIEs.\
         member[1]);
   wrFillTknU8(&(ie->pres), PRSNT_NODEF);
   wrFillTknU32(&(ie->id), Sztid_ResetTyp);
   wrFillTknU32(&(ie->criticality), SztCriticalityrejectEnum);

   if(rstType == RESETTYP_S1_INTF)
   {
      wrFillTknU8(&(ie->value.u.sztResetTyp.choice), RESETTYP_S1_INTF);
      wrFillTknU32(&(ie->value.u.sztResetTyp.val.s1_Intf),
                     SztResetAllreset_allEnum);
   }
   else
   {
      /* Partial reset, fill the S1AP eNB ids */
      noComp = numCons;
      wrFillTknU8(&(ie->value.u.sztResetTyp.choice), RESETTYP_PARTOFS1_INTF);
      wrFillTknU16(&(ie->value.u.sztResetTyp.val.partOfS1_Intf.noComp),
                    noComp);

      if (cmGetMem(reset,
             (noComp * sizeof(SztProtIE_SingleCont_UE_assocLogS1_ConItemRes)),
             (Ptr *)&(ie->value.u.sztResetTyp.val.partOfS1_Intf.member)) !=ROK)
      {
      RLOG0(L_FATAL,"Memory allocation failed for S1 connection items");
      RETVALUE(RFAILED);
      }

      for(idx = 0; (idx < noComp) && (idx <= WR_MAX_MMES); idx++) /* KW Fix */
      {
         ie1 = &(ie->value.u.sztResetTyp.val.partOfS1_Intf.member[idx]);

         wrFillTknU8(&(ie1->pres), PRSNT_NODEF);
         wrFillTknU32(&(ie1->id), SZT_ELM_ID_UE_ASSOC_LOG_S1_CON_ITM);
         wrFillTknU32(&(ie1->criticality), SztCriticalityignoreEnum);
         wrFillTknU8(&(ie1->value.u.sztUE_assocLogS1_ConItem.pres),
                     PRSNT_NODEF);
         if(conIds[idx].type == WR_S1AP_ENB_ID)
         {
            wrFillTknU32(&(ie1->value.u.sztUE_assocLogS1_ConItem.eNB_UE_S1AP_ID),
                  conIds[idx].conId);
            ie1->value.u.sztUE_assocLogS1_ConItem.mME_UE_S1AP_ID.pres = NOTPRSNT;
         }
         else if(conIds[idx].type == WR_S1AP_MME_ID)
         {
            wrFillTknU32(&(ie1->value.u.sztUE_assocLogS1_ConItem.mME_UE_S1AP_ID),
                  conIds[idx].conId);
            ie1->value.u.sztUE_assocLogS1_ConItem.mME_UE_S1AP_ID.pres = NOTPRSNT;
         }
         else
         {
            ie1->value.u.sztUE_assocLogS1_ConItem.mME_UE_S1AP_ID.pres = NOTPRSNT;
            ie1->value.u.sztUE_assocLogS1_ConItem.mME_UE_S1AP_ID.pres = NOTPRSNT;
         }
         ie1->value.u.sztUE_assocLogS1_ConItem.iE_Extns.noComp.pres = FALSE;
      }
   }

   *pdu = reset;

   RETVALUE(ROK);
}
/** @brief This function Fill the Reset ACK Message for the Reset Req received
 *
 *
 * @details This function allocates the memory for Reset Ack message and fills
 *          the proper values and respond back to caller function.
 *
 *     Function: wrEmmMmeBldResetAck 
 *
 *         Processing steps:
 *         - Allocate Memory for S1-AP: RESET Response PDU.
 *         - Fill the Response PDU properly.
 *
 * @param[in] pdu        : Reset PDU received
 * @param[out] rstAckPdu : Pointer to reset ack pdu
 * @return  S16
 *        -# Success : ROK
 *        -# Failure : RFAILED
 */
PUBLIC S16 wrEmmMmeBldResetAck
(
S1apPdu     *pdu,            /* S1AP Reset PDU */
S1apPdu     **rstAckPdu    /* S1AP Reset ACK PDU */
)
{
   S16                           ret = RFAILED;
   SztSuccessfulOutcome         *succMsg;
   TknU8                        *rstIe;
   S16                           numCon;
   S16                           idx;
   SztProtIE_Field_ResetAckgIEs *ie;
   SztProtIE_SingleCont_UE_assocLogS1_ConItemResAck *ie1;
    
   RLOG0(L_DEBUG,"Constructing S1AP reset ACK"); 
   /* Initialize memory control point */
   ret = cmAllocEvnt(sizeof(S1apPdu), SZ_MEM_SDU_SIZE, &(wrEmmCb.mem), (Ptr *)rstAckPdu);
   if (ret != ROK)
   {
      RLOG0(L_FATAL,"Memory allocation failed while constructing S1AP reset ACK");
      RETVALUE(RFAILED);
   }

   succMsg = &((*rstAckPdu)->pdu.val.successfulOutcome);

   wrFillTknU8(&((*rstAckPdu)->pdu.choice), SZT_TRGR_SUCCESS_OUTCOME);
   wrFillTknU8(&(succMsg->pres),PRSNT_NODEF);
   wrFillTknU32(&(succMsg->procedureCode), Sztid_Reset);
   wrFillTknU32(&(succMsg->criticality), SztCriticalityrejectEnum);
   wrFillTknU8(&(succMsg->value.u.sztResetAckg.pres),PRSNT_NODEF);

   wrSzGetIE(&(pdu->pdu),SZT_ELM_ID_RESET_TYPE, &rstIe);

   if(rstIe == NULLP)
   {
      RLOG0(L_ERROR, "wrEmmMmeBldResetAck: reset type IE is NULL.");
      RETVALUE(RFAILED);
   }

   /* Fill the conneciton information in case of the the partial reset */
   if(((SztResetTyp *)rstIe)->choice.val == SZT_PARTIAL_RESET)
   {
      wrFillTknU16(&(succMsg->value.u.sztResetAckg.protocolIEs.noComp), 1);

      if ((cmGetMem(&((*rstAckPdu)->memCp), sizeof(SztProtIE_Field_ResetAckgIEs),
          (Ptr *)&((*rstAckPdu)->pdu.val.successfulOutcome.value.\
                       u.sztResetAckg.protocolIEs.member))) !=ROK)
      {
         cmFreeMem(*rstAckPdu);
         RLOG0(L_FATAL,"Memory allocation failed for reset ackg IE's");
         RETVALUE(RFAILED);
      }

      ie = succMsg->value.u.sztResetAckg.protocolIEs.member;
   
      /* Fill IE container */
      wrFillTknU8(&(ie->pres), PRSNT_NODEF);
      wrFillTknU32(&(ie->id), SZT_ELM_ID_UE_ASSOC_LOG_S1_CON_LST_RES_ACK);
      wrFillTknU32(&(ie->criticality), SztCriticalityignoreEnum);
      wrFillTknU16(&(ie->value.u.sztUE_assocLogS1_ConLstResAck.noComp), 1);

      numCon = ((SztResetTyp *)rstIe)->val.partOfS1_Intf.noComp.val;

      if ((cmGetMem(&((*rstAckPdu)->memCp),
          (numCon * sizeof(SztProtIE_SingleCont_UE_assocLogS1_ConItemResAck)),
          (Ptr *)&ie->value.u.sztUE_assocLogS1_ConLstResAck.member)) !=ROK)
      {
         cmFreeMem(*rstAckPdu);
         RLOG0(L_FATAL,"Memory allocation failed for S1 concction item");
         RETVALUE(RFAILED);
      }

      for(idx = 0; idx < numCon; idx++)
      {
         ie1 = &ie->value.u.sztUE_assocLogS1_ConLstResAck.member[idx];

         wrFillTknU8(&(ie1->pres), PRSNT_NODEF);
         wrFillTknU32(&(ie1->id), SZT_ELM_ID_UE_ASSOC_LOG_S1_CON_ITM);
         wrFillTknU32(&(ie1->criticality), SztCriticalityignoreEnum);
         wrFillTknU8(&(ie1->value.u.sztUE_assocLogS1_ConItem.pres), PRSNT_NODEF);

         wrFillTknU32(&(ie1->value.u.sztUE_assocLogS1_ConItem.mME_UE_S1AP_ID),\
                       ((SztResetTyp *)rstIe)->val.partOfS1_Intf.member[idx].\
                       value.u.sztUE_assocLogS1_ConItem.mME_UE_S1AP_ID.val);

         wrFillTknU32(&(ie1->value.u.sztUE_assocLogS1_ConItem.eNB_UE_S1AP_ID),\
                       ((SztResetTyp *)rstIe)->val.partOfS1_Intf.member[idx].\
                       value.u.sztUE_assocLogS1_ConItem.eNB_UE_S1AP_ID.val);

         ie1->value.u.sztUE_assocLogS1_ConItem.iE_Extns.noComp.pres = NOTPRSNT;
      }
   }
   else 
   {
      wrFillTknU16(&(succMsg->value.u.sztResetAckg.protocolIEs.noComp), 0);
   }

   RETVALUE(ROK);
} /* end of wrEmmMmeBldResetAck */



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
