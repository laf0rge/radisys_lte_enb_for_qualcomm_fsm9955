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


     File:     wr_emm_neighenb.c

     Sid:      $SID$ 

     Prg:      Sriky 

*********************************************************************21*/
static const char* RLOG_MODULE_NAME="EMM";
static int RLOG_MODULE_ID=1;
static int RLOG_FILE_ID=14;

#include "wr.h"
#include "wr_cmn.h"
#include "wr_emm.h"
#include "wr_utils.h"
#include "czt.h"
#include "czt.x"
#include "wr_ifm_x2ap.h"
#include "wr_emm_neighenb.h"
#include "wr_emm_mme.h"

#include "wr_emm_s1_flex.h"
#ifdef WR_TEST_CODE
EXTERN U8 wrTgrX2SetupFail;
EXTERN U8 wrDntRspX2Setup;
EXTERN U8 wrTgrX2CfgUpdFail;
EXTERN U8 wrDntRspX2CfgUpd;
EXTERN U8 wrDntRspX2Reset;
EXTERN U8 wrTrgX2ErrInd;
EXTERN U8 wrTgX2SetupFailWoTW;
EXTERN U8 wrTgrX2CfgUpdFailWoTW;
#endif

EXTERN S16 wrX2AlarmHandle (WrNeighEnbCb *nbrEnbCb, U16 alarmId);

PUBLIC Bool wrEmmCmpEnodebId (U32 enbId1, U32 enbId2);
PUBLIC S16 wrEmmValidateEnbId (WrNeighEnbCb *enbnghCb, U32 enbId, U8 *causeTyp, U8 *causeVal);
PUBLIC S16 wrEmmCztFillPLMNId
(
 CztEvnt                      *pdu,
 WrPlmnId                     *plmnId,
 CztPLMN_Identity             *plmnIe
 );

EXTERN S16 wrX2apFillCause
(
 CztCause                     *ie,
 WrUmmMsgCause                *cause
 );
/** @brief This function parses the PLMN IE from
 *         the X2AP message and updates the local
 *         PLMNId data structure.
 *
 * @details
 *
 *     Function: wrEmmNghEnbParsePlmnIe
 *
 *         Processing steps:
 *         - Its an utility to parse PLMN Ie from
 *           the X2AP message and updates the same
 *           to the WrPlmnId structure.
 *
 *
 * @param [in]    plmnIe : Plmn ID IE 
 * @param [out]  plmnId : Plmn
 * @return S16
 *    -# Success : ROK
 */

PUBLIC S16 wrEmmNghEnbParsePlmnIe
(
 CztPLMN_Identity             *plmnIe,
 WrPlmnId                     *plmnId
 )
{
   plmnId->mcc[0]          = plmnIe->val[0] & 0x0f;
   plmnId->mcc[1]          = (plmnIe->val[0] & 0xf0) >> 4;
   plmnId->mcc[2]          = plmnIe->val[1] & 0x0f;
   if ((plmnIe->val[1] & 0xf0) == 0xf0)
   {
      plmnId->numMncDigits = 2;
      plmnId->mnc[1]       = (plmnIe->val[2] & 0xf0) >> 4;
      plmnId->mnc[0]       = plmnIe->val[2] & 0x0f;
   }
   else
   {
      plmnId->numMncDigits = 3;
      plmnId->mnc[0]       = (plmnIe->val[1] & 0xf0) >> 4;
      plmnId->mnc[1]       = plmnIe->val[2] & 0x0f;
      plmnId->mnc[2]       = (plmnIe->val[2] & 0xf0) >> 4;
   }
   RETVALUE(ROK);
}

/** @brief This function parses TKNSTR4 IE from
 *         the message and updates the U32 bit
 *         variable.
 *
 * @details
 *
 *     Function: wrEmmNghEnbParseTknStr4Ie
 *
 *         Processing steps:
 *         - Its an utility function to parse
 *           TknStr4 variable and convert it into
 *           32 bit value.
 *
 *
 * @param[out]   ie        : TknStr4
 * @param[in]     lvalue  : Value to be parsed
 * @return S16
 *     -# Success : ROK
 */
PUBLIC S16 wrEmmNghEnbParseTknStr4Ie
(
 TknStr4                     *ie,
 U32                         *lvalue
 )
{
   /* ccpu00122813 */
   /* START*/
   (*lvalue) = ie->val[0];
   (*lvalue) <<= 8;
   (*lvalue) |= ie->val[1];
   (*lvalue) <<= 8;
   (*lvalue) |= ie->val[2];
   (*lvalue) <<= 8;
   (*lvalue) |= ie->val[3];
   (*lvalue) >>= (32 - ie->len);
   /* END*/
   RETVALUE(ROK);
}

/** @brief This function is an utility to build
 *         Tracking area code into the S1AP message.
 *
 *
 * @details
 *
 *     Function: wrEmmCztFillTAC
 *
 *         Processing steps:
 *         - This utility constructs X2AP TAC IE
 *           from the TAC value as input.
 *
 *
 * @param[in]   tac  : Tracking Area code
 * @param[in]   pdu : Czt Event Structure
 * @param[out] tAC : Tracking Area Code IE
 * @return S16 
 *    -# Success : ROK
 */
PRIVATE S16 wrEmmCztFillTAC
(
 U16                          tac,
 CztEvnt                      *pdu,
 CztTAC                       *tAC
 )
{
   tAC->pres   = PRSNT_NODEF;
   tAC->len    = WR_LAC_IE_LEN;
   tAC->val[1] = (U8)(tac & 0xff);
   tAC->val[0] = (U8)((tac & 0xff00)>>8);
   RETVALUE(ROK);
}

/** @brief This function is an utility to construct
 *         CZT PLMNID IE.
 *
 * @details
 *
 *     Function: wrEmmCztFillPLMNId
 *
 *         Processing steps:
 *         - Its an utility to construct PLMNID IE into
 *           X2AP message.
 *
 *
 * @param[out]   pdu      : Czt Event Structure 
 * @param[in]     plmnId : Plmn ID
 * @param[out]   plmnIe : Plmn ID
 * @return S16 
 *     -# Success : ROK
 */
PUBLIC S16 wrEmmCztFillPLMNId
(
 CztEvnt                      *pdu,
 WrPlmnId                     *plmnId,
 CztPLMN_Identity             *plmnIe
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
   wrFillTknStrOSXL(plmnIe,WR_PLMNID_IE_LEN,pLMNId,&pdu->memCp);
   RETVALUE(ROK);
}
/** @brief This function is an utility to build
 *         CZT Global eNodeB IE for X2AP messages.
 *
 * @details
 *
 *     Function: wrEmmCztFillGlbeNBId
 *
 *         Processing steps:
 *         - Its an utility function to construct
 *           CZT Global eNBId IE by taking necessary
 *           parameters as inputs.
 *
 *
 * @param[in]   pdu : Czt Event Structure
 * @param[in]   enbType : eNodeB Type
 * @param[in]   cgi         : CGI
 * @param[in]   plmnId   : Plmn ID
 * @param[out]   enbIe   : Plmn ID IE
 * @return S16 
 *      -# Success : ROK
 */
PRIVATE S16 wrEmmCztFillGlbeNBId
(
 CztEvnt                      *pdu,
 U32                          enbType,
 U32                          cgi,
 WrPlmnId                     *plmnId,
 CztGlobalENB_ID              *enbIe
 )
{
   U32                       enbId  = cgi;

   wrFillTknU8(&(enbIe->pres), PRSNT_NODEF);
   wrEmmCztFillPLMNId(pdu, plmnId, &enbIe->pLMN_Identity);
   wrFillTknU8(&(enbIe->eNB_ID.choice), enbType);
   RLOG_ARG0(L_INFO,DBG_ENBID,enbId, 
         "Sending ENB ID from ACTUAL ");
   switch(enbType)
   {
      case ENB_ID_HOMEENB_ID:
         enbIe->eNB_ID.val.home_eNB_ID.pres = PRSNT_NODEF;
         enbIe->eNB_ID.val.home_eNB_ID.len = 28;
         enbId = enbId & 0x0fffffff;
         wrFillTknBStr32(&(enbIe->eNB_ID.val.home_eNB_ID), 28, enbId);
         break;
      default:
         enbIe->eNB_ID.val.macro_eNB_ID.pres = PRSNT_NODEF;
         enbIe->eNB_ID.val.macro_eNB_ID.len = 20;
		 /*DO NOT right shift here as its done in SMM already*/
         enbId = enbId & 0x000fffff;
         wrFillTknBStr32(&(enbIe->eNB_ID.val.macro_eNB_ID), 20, enbId);
         break;
   }
   RLOG1(L_INFO,"Sending ENB ID [%ld]",enbId);
   enbIe->iE_Extns.noComp.pres = NOTPRSNT;
   RETVALUE(ROK);
}
/** @brief This function builds the X2 Setup Fail
 *         CZT message.
 *
 * @details
 *
 *     Function: wrEmmNghEnbBldX2setupFail
 *
 *         Processing steps:
 *         - Its a build function for CZT X2 Setup
 *           failure.
 *
 * @param[in]  incTimeTowait : Include Time to wait IE
 * @param[out]  pdu          : Czt Event Structure
 * @return S16 
 *     -# Success : ROK 
 *     -# Failure   : RFAILED
 */
PRIVATE S16 wrEmmNghEnbBldX2setupFail
(
 CztEvnt                   **pdu,
 Bool                      incTimeTowait,
 U8                        causeTyp,
 U32                       causeVal
 )
{
   CztEvnt                      *x2SetupFailPdu;
   CztX2SetupFail               *x2SetupFail;
   CztProtIE_Field_X2SetupFail_IEs *ie;
   U8                           ieIdx = 0;
   U8                           numComp;
   S16                          ret = ROK;

   TRC2(wrEmmNghEnbBldX2setupFail);

   RLOG0(L_DEBUG,"Building of the X2 Setup Fail ");

   /* Allocate and Initialize memory control point */
   ret = cmAllocEvnt(sizeof(CztEvnt), WR_CZ_MEM_SDU_SIZE,
         &wrCb.mem, (Ptr *)&x2SetupFailPdu);
   if(ret != ROK)
   {
      RLOG0(L_FATAL,"Memory allocation failed.");
      RETVALUE(RFAILED);
   }
   /* Initialize the PDU */
   cmMemset((U8 *)&(x2SetupFailPdu->pdu), 0, sizeof(CztX2AP_PDU));

   /* Fill the contents of Unsuccessful Event*/
   wrFillTknU8(&(x2SetupFailPdu->pdu.choice),X2AP_PDU_UNSUCCESSFULOUTCOME);
   wrFillTknU8(&(x2SetupFailPdu->pdu.val.unsuccessfulOutcome.pres),PRSNT_NODEF);
   wrFillTknU32(&(x2SetupFailPdu->pdu.val.unsuccessfulOutcome.procedureCode),
         Cztid_x2Setup);
   wrFillTknU32(&(x2SetupFailPdu->pdu.val.unsuccessfulOutcome.criticality),
         CztCriticalityrejectEnum);
   wrFillTknU8(&(x2SetupFailPdu->pdu.val.unsuccessfulOutcome.value.u.
            cztX2SetupFail.pres), PRSNT_NODEF);

   x2SetupFail = &x2SetupFailPdu->pdu.val.unsuccessfulOutcome.
      value.u.cztX2SetupFail;

   /* Filling 2 mandatory IEs  into the pdu */
   if ( incTimeTowait == TRUE )
   {
      numComp = 2;
   }
   else
   {
      numComp = 1;
   }
   wrFillTknU16(&(x2SetupFail->protocolIEs.noComp), numComp);

   /* Get memory for numComp */
   if ((cmGetMem(x2SetupFailPdu, (numComp *
                  sizeof(CztProtIE_Field_X2SetupFail_IEs)),
               (Ptr*)&x2SetupFail->protocolIEs.member)) != ROK)
   {
      RLOG0(L_FATAL, "Memory allocation failed.");
      /* Free the memory of Event Structure */
      WR_FREE_EVNT(x2SetupFailPdu);
      RETVALUE(RFAILED);
   } /* end of if statement */

   /* IE1 - Filling  Cause IE*/
   ie = &x2SetupFail->protocolIEs.member[ieIdx];
   wrFillTknU8(&(ie->pres), PRSNT_NODEF);
   wrFillTknU32(&(ie->id), Cztid_Cause);
   wrFillTknU32(&(ie->criticality), CztCriticalityignoreEnum);
   wrFillTknU8(&(ie->value.u.cztCause.choice),causeTyp);

   switch(causeTyp)
   {
      case WR_CZ_CAUSE_TRANSPORT:
         wrFillTknU32(&(ie->value.u.cztCause.val.transport),
               causeVal);
         break;
      case WR_CZ_CAUSE_RADIONW:
         wrFillTknU32(&(ie->value.u.cztCause.val.radioNw),
               causeVal);
         break;

   }
   ieIdx++;

   /* IE1 - Filling  Time to Wait IE*/
   if ( incTimeTowait == TRUE )
   {
      ie = &x2SetupFail->protocolIEs.member[ieIdx];
      wrFillTknU8(&(ie->pres), PRSNT_NODEF);
      wrFillTknU32(&(ie->id), Cztid_TimeToWait);
      wrFillTknU32(&(ie->criticality), CztCriticalityignoreEnum);
      wrFillTknU32(&(ie->value.u.cztTimeToWait), 
            wrEmmCb.tmrCfg.x2SetTimeToWait);
   }

   *pdu = x2SetupFailPdu;

   RETVALUE(ROK);
}
/** @brief This function is an wrapper to build and
 *         deliver CZT X2 setup failure message to
 *         the peer.
 *
 * @details
 *
 *     Function: wrEmmNghEnbSndX2setupFail
 *
 *         Processing steps:
 *         - Its an wrapper funtion which builds
 *           X2 Setup Failure message.
 *         - It delivers the constructed PDU
 *           to the X2AP layer through IFM module.
 *
 *
 * @param[out]   peerId         : Peer ID
 * @param[out]   incTimeTowait  : Include Time to wait IE 
 * @return S16 
 *      -# Success : ROK 
 *      -# Failure   : RFAILED
 */
PRIVATE S16 wrEmmNghEnbSndX2setupFail
(
 U32                     peerId,
 Bool                    incTimeTowait,
 U8                      causeTyp,
 U32                     causeVal
 )
{

   CztEvnt                      *x2SetupFail = NULLP;

   if(ROK != wrEmmNghEnbBldX2setupFail(&x2SetupFail, incTimeTowait, causeTyp, causeVal))
   {
      RLOG0(L_ERROR," Failure in building X2 Setup Fail ");
      RETVALUE(RFAILED);
   }
   if(NULLP == x2SetupFail)
   {
      RLOG0(L_ERROR," Failure in building X2 Setup Fail ");
      RETVALUE(RFAILED);
   }
   x2SetupFail->peerId = peerId;

   RLOG_ARG1(L_DEBUG,DBG_PEERID, peerId,
         "Sending X2 SETUP FAILURE, Time To wait = [%d]",
         wrEmmCb.tmrCfg.x2SetTimeToWait);
   /* Trigger corresponding primitive */
   if (ROK != WrIfmX2apGpRsp(x2SetupFail))
   {
      RLOG0(L_ERROR,"Failed to Send X2 Setup Fail ");
      RETVALUE(RFAILED);
   }
   else
   {
      RLOG3(L_EVENT,"[X2AP]:Sending X2 SETUP FAILURE [Src(%ld)-->Peer(%ld)] [CAUSE:%ld]",
            wrEmmCb.enbId, peerId,causeVal);
   }

   RETVALUE(ROK);
}

/** @brief This function Add Neighbour cell information into X2 setup request
  message
 *
 * @details
 *
 *     Function: wrEmmAddNghCellIntoX2SetupMsg
 *
 *         Processing steps:
 *         - Add Neighbour cell information into X2 setup request message.
 *
 * @param[in]   x2SetupRqstPdu  : pointer to x2SetupRqstPdu
 * @param[in]   earfcn : Arfcn
 * @param[in]   nghCellLst : pointer to neighcell List
 * @param[in]   idx : pointer to index
 * @param[out]   nghInfo : pointer to CztNeighbour_Inform structure
 * @return S16 
 *      -# Success  : ROK
 *      -# Failure     : RFAILED
 */
PRIVATE S16 wrEmmAddNghCellIntoX2SetupMsg
(
 CztEvnt                     *x2SetupPdu,
 U32                         earfcn,
 CmLListCp                   *nghCellLst,
 CztNeighbour_Inform         *nghInfo,
 U16                         *numComp 
 )
{
   U16                                       freqCnt = 0;
   CmLList                                   *lnk;
   WrEutranNeighCellCb                       *nghCellCb;
   U16                                       count=0;
   U8                                        numNeighExtns;
   CztProtExtnField_Neighbour_Inform_ExtIEs  *extIe;

   CM_LLIST_FIRST_NODE(nghCellLst, lnk);

   /* copy the number of neighbour cells available into 
    * count variable*/
   count = nghCellLst->count;
   /* assign the next index to freqCnt in which neighbour
    * info is going to fill */
   freqCnt =*numComp;
   while (lnk != NULLP)
   {
      nghCellCb = (WrEutranNeighCellCb*)lnk->node;
      RLOG0(L_INFO,"Adding to neighbor list");
      RLOG2(L_INFO,"IE pci [%d] eutranCellId [%d]",nghCellCb->pci,
            nghCellCb->eCgi.eutranCellId);
      RLOG2(L_INFO," for enbId [%d] enbType [%d]",nghCellCb->enbId,
            nghCellCb->enbType);
      RLOG1(L_INFO,"earfcn [%d]",earfcn); 

      wrFillTknU8(&nghInfo->member[freqCnt].pres,PRSNT_NODEF);
      wrFillTknU32(&nghInfo->member[freqCnt].eARFCN, earfcn);
      wrFillTknU32(&nghInfo->member[freqCnt].pCI,nghCellCb->pci);
      wrFillTknU8(&nghInfo->member[freqCnt].eCGI.pres,PRSNT_NODEF);
      wrFillTknBStr32(&(nghInfo->member[freqCnt].eCGI.eUTRANcellIdentifier),
            28, nghCellCb->eCgi.eutranCellId);
      wrEmmCztFillPLMNId(x2SetupPdu, &nghCellCb->eCgi.plmnId, 
            &(nghInfo->member[freqCnt].eCGI.pLMN_Identity));

      /*Since we filling only TAC in extension WR_EMM_X2_NEIGH_EXTEN_IE_COUNT =1*/
      numNeighExtns = WR_EMM_X2_NEIGH_EXTEN_IE_COUNT;
      wrFillTknU16(&(nghInfo->member[freqCnt].iE_Extns.noComp), numNeighExtns);
      /*Allocate memory*/
      WR_GET_MEM(x2SetupPdu, numNeighExtns * sizeof(CztProtExtnField_Neighbour_Inform_ExtIEs), 
                 &(nghInfo->member[freqCnt].iE_Extns.member));
      {/*Fill TAC IE*/
         extIe = &(nghInfo->member[freqCnt].iE_Extns.member[0]);
         wrFillTknU8(&(extIe->pres), PRSNT_NODEF);
         wrFillTknU32(&(extIe->id), Cztid_NeighbourTAC);
         wrFillTknU32(&(extIe->criticality),CztCriticalityignoreEnum);
         wrEmmCztFillTAC(nghCellCb->tac, x2SetupPdu,
         &(extIe->extensionValue.u.cztTAC));
      }

      CM_LLIST_NEXT_NODE(nghCellLst, lnk);
      freqCnt++;
   }
   /* after processing the particular freq neighbour list, copy
    * the index info in *numComp.So that for next freq neighbour 
    * list, stored index will be used*/
   *numComp+=count;
   RETVALUE(ROK);
}

/*Fix for ccpu00126848:to count the number of neighbour cells */
/** @brief This function return the total number of 
 *   Intra frequency and Inter frequency neighbour cells
 *
 * @details
 *
 *     Function: wrEmmFillNghCellCount
 *
 *         Processing steps:
 *       - count WL and BL of Intra-frequency neighbour cells
 *       - plus WL and BL count of Inter frequency neighbour cells
 *
 * @return S16 
 */

PRIVATE S16 wrEmmFillNghCellInfo
(
 WrNrEutranCb                  *eutranCb,
 U16                           *count
 )
{
   CmLList                       *lnk = NULLP;
   WrNrEutranFreqCb              *lnkFreqNode = NULLP;
   WrNrEutranFreqCb            *freqNode;

   TRC2(wrEmmFillNghCellInfo);

   /* Get Intra Freq and add the count of WL and BL neighbor cells */
   freqNode = &eutranCb->intraFreqCb;
   *count = freqNode->nrWlLst.count + freqNode->nrBlLst.count;

   lnk = eutranCb->interFreqList.first;

   while(lnk != NULLP)
   {
      lnkFreqNode = (WrNrEutranFreqCb*)lnk->node;
      /* Get Inter Freq and add the count of WL and BL neighbor cells into cnt variable*/
      *count = lnkFreqNode->nrWlLst.count + lnkFreqNode->nrBlLst.count + *count;
      lnk = lnk->next;
   }

   RETVALUE(ROK);
}

/** @brief This function Fills Neighbour cell information for X2 setup request
  message
 *
 * @details
 *
 *     Function: wrEmmFillNghCellInfoforX2Msg
 *
 *         Processing steps:
 *         - Fills Neighbour cell information fro X2 setup request message.
 *
 * @param[in]   cellCb  : pointer to cell control block
 * @param[out]  nghInfo : Czt neighbour cell information
 * @return S16 
 *      -# Success  : ROK
 *      -# Failure     : RFAILED
 */
PRIVATE S16 wrEmmFillNghCellInfoforX2Msg
(
 CztEvnt                     *pdu,
 WrCellCb                    *cellCb,
 CztNeighbour_Inform         *nghInfo
 )
{
   U32                         earfcn = 0;
   WrNrEutranCb                *eutranCb;
   WrNrEutranFreqCb            *freqNode;
   U16                         numComp = 0;
   U16                         count = 0;

   CmLList                       *lnk = NULLP;
   WrNrEutranFreqCb              *lnkFreqNode = NULLP;

   TRC2(wrEmmFillNghCellInfoforX2Msg);

   eutranCb = &cellCb->nrCb->eutranCb;

   /*Fix for ccpu00126848:To copy the number of neighbour cells */
   wrEmmFillNghCellInfo(eutranCb,&count);

   if (count !=0)
   {
      wrFillTknU16(&(nghInfo->noComp), count);

      if ((cmGetMem(pdu, count * sizeof(CztNeighbour_InformMember),
                  (Ptr*)&nghInfo->member)) !=ROK)
      {
         RLOG0(L_FATAL, "Memory allocation failed.");
         RETVALUE(RFAILED);
      }

      /* Get Intra Freq and add the intra freq neighbor cell */
      earfcn = eutranCb->intraFreqCb.earfcn;

      freqNode = wrEmmAnrGetEutranFreqNode(earfcn, eutranCb);

      if(freqNode != NULLP)
      {
         if(freqNode->nrWlLst.count !=0)
         {
            /* Get Intra Freq whitelist neighbor cell*/
            wrEmmAddNghCellIntoX2SetupMsg(pdu, earfcn,
                  &freqNode->nrWlLst, nghInfo,&numComp);
         }
         if(freqNode->nrBlLst.count !=0)
         {
            /* Get Intra Freq blacklist neighbor cell*/
            wrEmmAddNghCellIntoX2SetupMsg(pdu, earfcn,
                  &freqNode->nrBlLst, nghInfo,&numComp);
         }
      }
      else
      {
         RLOG0(L_ERROR,"wrEmmAnrModNr CDMA NR Entry Not Exist.");
      }

      /* Get Inter freq and add Inter Freq Neighbors */
      lnk = eutranCb->interFreqList.first;
      while(lnk != NULLP)
      {
         lnkFreqNode = (WrNrEutranFreqCb*)lnk->node;
         earfcn = lnkFreqNode->earfcn;

         freqNode = wrEmmAnrGetEutranFreqNode(earfcn, eutranCb);

         if(freqNode != NULLP)
         {
            if(freqNode->nrWlLst.count !=0)
            {
               /* Get Inter Freq whitelist neighbor cell*/
               wrEmmAddNghCellIntoX2SetupMsg(pdu, earfcn,
                     &freqNode->nrWlLst, nghInfo,&numComp);
            }
            if(freqNode->nrBlLst.count !=0)
            {
               /* Get Inter Freq blacklist neighbor cell*/
               wrEmmAddNghCellIntoX2SetupMsg(pdu, earfcn,
                     &freqNode->nrBlLst, nghInfo,&numComp);
            }
         }
         lnk = lnk->next;
      }
   }
   else
   {
      nghInfo->noComp.pres = NOTPRSNT;
   }
   RETVALUE(ROK);
}

/** @brief This function builds X2 Setup Response
 *         CZT message.
 *
 * @details
 *
 *     Function: wrEmmNghEnbBldX2SetupRsp
 *
 *         Processing steps:
 *         - Its constructs CZT X2 Setup Response
 *           message.
 *
 *
 * @param[out]    pdu : Czt Event Structure
 * @return S16 
 *     -# Success : ROK
 *     -# Failure   : RFAILED
 */
PRIVATE S16  wrEmmNghEnbBldX2SetupRsp
(
 CztEvnt      **pdu
 )
{
   CztEvnt                   *x2SetupRspPdu;
   U16                       numComp = 0;
   S16                       ret = ROK;
   U8                        plmnIdx = 0;
   U8                        cellIdx = 0;
   U8                        guIdx = 0;
   U8                        len = 0;   /* KW fix for LTE_ADV */ 
   U8                        ieIdx = 0;
   CztProtIE_Field_X2SetupResp_IEs *ie;
   CztX2SetupResp            *x2SetupRsp;
   CztGU_Group_ID            *guIdIe;
   CztSrvdCellsMember        *protoie;
   CztProtExtnField_SrvdCell_Inform_ExtIEs *extIe;
#ifdef LTE_TDD
   CztTDD_Info               *tddInfo;
   CztSpecialSubframe_Info   *specialSfInfo;
#endif
   U8                        bw;
   CztNeighbour_Inform       *nghInfo;
   /*ccpu128503*/
   U16                       mmeGrpId = 0;
   WrGuGrupCbList            *guGrupCbList = NULLP;
   PTR                       prevEnt = NULLP;
   /* CSG_DEV start */
   Bool                     isCsgCfgd;
   U32                      csgId;
   /* CSG_DEV end */

   TRC2(wrEmmNghEnbBldX2SetupRsp);

   RLOG0(L_DEBUG,"Building of the X2 Setup Response");

   /* Allocate and Initialize memory control point */
   ret = cmAllocEvnt(sizeof(CztEvnt), WR_CZ_MEM_SDU_SIZE,
         &wrCb.mem, (Ptr *)&x2SetupRspPdu);
   if(ret != ROK)
   {
      RLOG0(L_FATAL,"Memory allocation failed.");
      RETVALUE(RFAILED);
   }
   /* Initialize the PDU */
   cmMemset((U8 *)&(x2SetupRspPdu->pdu), 0, sizeof(CztX2AP_PDU));

   /* Fill the contents of initiatingMsg */
   wrFillTknU8(&(x2SetupRspPdu->pdu.choice), X2AP_PDU_SUCCESSFULOUTCOME);
   wrFillTknU8(&(x2SetupRspPdu->pdu.val.successfulOutcome.pres),PRSNT_NODEF);
   wrFillTknU32(&(x2SetupRspPdu->pdu.val.successfulOutcome.procedureCode),
         Cztid_x2Setup);
   wrFillTknU32(&(x2SetupRspPdu->pdu.val.successfulOutcome.criticality),
         CztCriticalityrejectEnum);
   wrFillTknU8(&(x2SetupRspPdu->pdu.val.successfulOutcome.value.u.
            cztX2SetupResp.pres), PRSNT_NODEF);


   x2SetupRsp = &x2SetupRspPdu->pdu.val.successfulOutcome.
      value.u.cztX2SetupResp;
   /* Filling 3 mandatory IEs  into the pdu */
   numComp = 3;
   wrFillTknU16(&(x2SetupRsp->protocolIEs.noComp), numComp);

   /* Get memory for numComp */
   if ((cmGetMem(x2SetupRspPdu, (numComp *
                  sizeof(CztProtIE_Field_X2SetupResp_IEs)),
               (Ptr*)&x2SetupRsp->protocolIEs.member)) != ROK)
   {
      RLOG0(L_FATAL, "Memory allocation failed.");
      /* Free the memory of Event Structure */
      WR_FREE_EVNT(x2SetupRspPdu);
      RETVALUE(RFAILED);
   } /* end of if statement */

   /* IE1 - Filling ENB Global Id  in member[0]*/
   ie = &x2SetupRsp->protocolIEs.member[ieIdx];
   wrFillTknU8(&(ie->pres), PRSNT_NODEF);
   wrFillTknU32(&(ie->id), Cztid_GlobalENB_ID);
   wrFillTknU32(&(ie->criticality), CztCriticalityrejectEnum);
   wrFillTknU8(&(ie->value.u.cztGlobalENB_ID.pres), PRSNT_NODEF);
   /* Fill the PLMN ID of source Cell Id */
   /*Filling of Global ENB ID */
   wrEmmCztFillGlbeNBId(x2SetupRspPdu, wrEmmCb.enbType,
         wrEmmCb.enbId,
         &wrEmmCb.cellCb[0]->sib1.plmns[0].plmnId,
         &(ie->value.u.cztGlobalENB_ID));
   ieIdx++;

   /* IE2 - Filling Served Cells */
   ie = &x2SetupRsp->protocolIEs.member[ieIdx];

   wrFillTknU8(&(ie->pres), PRSNT_NODEF);
   wrFillTknU32(&(ie->id), Cztid_SrvdCells);
   wrFillTknU32(&(ie->criticality),CztCriticalityrejectEnum);
   numComp = wrEmmCb.numCells;

   wrFillTknU16(&(ie->value.u.cztSrvdCells.noComp), numComp);

   if ((cmGetMem(x2SetupRspPdu,(numComp * sizeof(CztSrvdCellsMember)),
               (Ptr *)&ie->value.u.cztSrvdCells.member)) !=ROK)
   {
      RLOG0(L_FATAL, "Memory allocation failed.");
      WR_FREE_EVNT(x2SetupRspPdu);
      RETVALUE(RFAILED);
   }

   for (cellIdx = 0; cellIdx < wrEmmCb.numCells;cellIdx++)
   {
      protoie = &ie->value.u.cztSrvdCells.member[cellIdx];

      wrFillTknU8(&(protoie->pres), PRSNT_NODEF);
      wrFillTknU8(&(protoie->servedCellInfo.pres), PRSNT_NODEF);
      wrFillTknU32(&(protoie->servedCellInfo.pCI),
            wrEmmCb.cellCb[cellIdx]->physCellId);
      wrFillTknU8(&(protoie->servedCellInfo.cellId.pres), PRSNT_NODEF);

      /* Using the first configured cell and the first PLMN ID in the list */
      wrEmmCztFillPLMNId(x2SetupRspPdu,
            &(wrEmmCb.cellCb[cellIdx]->sib1.plmns[0].plmnId),
            &(protoie->servedCellInfo.cellId.pLMN_Identity));

      len = 28;
      wrFillTknBStr32(&(protoie->servedCellInfo.cellId.eUTRANcellIdentifier),
                        len,wrEmmCb.cellCb[cellIdx]->sib1.cellIdentity);

      wrEmmCztFillTAC(wrEmmCb.cellCb[cellIdx]->sib1.tac,x2SetupRspPdu,
            &protoie->servedCellInfo.tAC);

      /*  Filling broadcastPLMNs */
      numComp = wrEmmCb.cellCb[cellIdx]->sib1.numPlmnIds;

      /* Allocate memory for broadcast PLMNs */
      if ((cmGetMem(x2SetupRspPdu, numComp * sizeof(CztPLMN_Identity),
                  (Ptr*)&protoie->servedCellInfo.broadcastPLMNs.member)) !=ROK)
      {
         RLOG0(L_FATAL,"Memory allocation failed.");
         WR_FREE_EVNT(x2SetupRspPdu);
         RETVALUE(RFAILED);
      }
      wrFillTknU16(&(protoie->servedCellInfo.broadcastPLMNs.noComp),numComp);

      for(plmnIdx = 0; plmnIdx < numComp; plmnIdx++)
      {
         /* Fill the PLMN ID of source Cell Id */
         wrEmmCztFillPLMNId(x2SetupRspPdu,
               &(wrEmmCb.cellCb[cellIdx]->sib1.plmns[plmnIdx].plmnId),
               &(protoie->servedCellInfo.broadcastPLMNs.member[plmnIdx]));
      }
#ifndef LTE_TDD
      /*Filling of FDD Info*/
      wrFillTknU8(&(protoie->servedCellInfo.eUTRA_Mode_Info.choice),
            EUTRA_MODE_INFO_FDD);
      wrFillTknU8(&(protoie->servedCellInfo.eUTRA_Mode_Info.val.fDD.pres),
            PRSNT_NODEF);
      wrFillTknU32(&(protoie->servedCellInfo.eUTRA_Mode_Info.val.
               fDD.uL_EARFCN),
            wrEmmCb.cellCb[cellIdx]->sib2.ulCarrierFreq);
      wrFillTknU32(&(protoie->servedCellInfo.eUTRA_Mode_Info.val.
               fDD.dL_EARFCN),
            wrEmmCb.cellCb[cellIdx]->dlCarrierFreq);
      /* Convert bandwidth from ASN to numbers */
      bw = wrEmmCb.cellCb[cellIdx]->sib2.ulBw;
      wrFillTknU32(&(protoie->servedCellInfo.eUTRA_Mode_Info.val.
               fDD.uL_Transmission_Bandwidth),
            bw);
      bw = wrEmmCb.cellCb[cellIdx]->mib.dlBw;
      wrFillTknU32(&(protoie->servedCellInfo.eUTRA_Mode_Info.val.
               fDD.dL_Transmission_Bandwidth),
            bw);
#else
      /*ccpu00134580-ADD-To support TDD in x2 setup*/
      /*Filling of TDD Info*/
      tddInfo = &protoie->servedCellInfo.eUTRA_Mode_Info.val.tDD;
      specialSfInfo = &tddInfo->specialSubframe_Info;
      wrFillTknU8(&(protoie->servedCellInfo.eUTRA_Mode_Info.choice),
            EUTRA_MODE_INFO_TDD);
      wrFillTknU8(&(tddInfo->pres),PRSNT_NODEF);
      /*For TDD both ARFCN and Bandwidth is same in DL & UL */
      wrFillTknU32(&(tddInfo->eARFCN),
            wrEmmCb.cellCb[cellIdx]->sib2.ulCarrierFreq);
      bw = wrEmmCb.cellCb[cellIdx]->sib2.ulBw;
      wrFillTknU32(&(tddInfo->transmission_Bandwidth),bw);
      wrFillTknU32(&(tddInfo->subframeAssignment),wrEmmCb.cellCb[cellIdx]->sib1.
            tddCfg.subframeCfg);
      wrFillTknU8(&(specialSfInfo->pres), PRSNT_NODEF);
      wrFillTknU32(&(specialSfInfo->specialSubframePatterns),  
            wrEmmCb.cellCb[cellIdx]->sib1.
            tddCfg.specialSubframeCfg);
      wrFillTknU32(&(specialSfInfo->cyclicPrefixDL), 
            wrEmmCb.cellCb[cellIdx]->dlCyclicprefix);
      wrFillTknU32(&(specialSfInfo->cyclicPrefixUL),
            wrEmmCb.cellCb[cellIdx]->sib2.ulCyclicPrefix);
#endif
      /*  Filling PRACH Information and Antenna Ports and CSG ID */
      /* CSG_DEV start */
      isCsgCfgd = wrEmmIsCsgCell(wrEmmCb.cellCb[cellIdx]->cellId);
      if(TRUE == isCsgCfgd)
      {
         numComp = WR_EMM_X2_CONFIG_EXTEN_IE_COUNT + 1;
      }
      else
      {
         numComp = WR_EMM_X2_CONFIG_EXTEN_IE_COUNT;
      }
      /* CSG_DEV end */
      /* Allocate memory for  PRACH Information and Antenna Ports*/
      if ((cmGetMem(x2SetupRspPdu, numComp *
                  sizeof(CztProtExtnField_SrvdCell_Inform_ExtIEs),
                  (Ptr*)&protoie->servedCellInfo.iE_Extns.member)) !=ROK)
      {
         RLOG0(L_FATAL,"Memory allocation failed.");
         WR_FREE_EVNT(x2SetupRspPdu);
         RETVALUE(RFAILED);
      }
      wrFillTknU16(&(protoie->servedCellInfo.iE_Extns.noComp),numComp);

      extIe = &protoie->servedCellInfo.iE_Extns.member[0];
      wrFillTknU8(&(extIe->pres), PRSNT_NODEF);
      wrFillTknU32(&(extIe->id), Cztid_Number_of_Antennaports);
      /* ccpu00134596: Correct the criticality filled */
      wrFillTknU32(&(extIe->criticality),CztCriticalityignoreEnum);
      wrFillTknU32(&(extIe->extensionValue.u.cztNumber_of_Antennaports),
            wrEmmCb.cellCb[cellIdx]->numAntPorts);

      extIe = &protoie->servedCellInfo.iE_Extns.member[1];
      wrFillTknU8(&(extIe->pres), PRSNT_NODEF);
      wrFillTknU32(&(extIe->id), Cztid_PRACH_Config);
      /* ccpu00134596: Correct the criticality filled */
      wrFillTknU32(&(extIe->criticality),CztCriticalityignoreEnum);

      wrFillTknU8(&(extIe->extensionValue.u.cztPRACH_Config.pres), PRSNT_NODEF);
      wrFillTknU32(&(extIe->extensionValue.u.cztPRACH_Config.rootSequenceIdx),
            wrEmmCb.cellCb[cellIdx]->sib2.prachCfg.rootSeqIdx);
      wrFillTknU32(&(extIe->extensionValue.u.cztPRACH_Config.zeroCorrelationIdx),
            wrEmmCb.cellCb[cellIdx]->sib2.prachCfg.zeroCorrZone);
      wrFillTknU8(&(extIe->extensionValue.u.cztPRACH_Config.highSpeedFlag),
            wrEmmCb.cellCb[cellIdx]->sib2.prachCfg.highSpeedFlag);
      wrFillTknU32(&(extIe->extensionValue.u.cztPRACH_Config.prach_FreqOffset),
            wrEmmCb.cellCb[cellIdx]->sib2.prachCfg.prachFreqOffset);
      wrFillTknU32(&(extIe->extensionValue.u.cztPRACH_Config.prach_ConfigIdx),
            wrEmmCb.cellCb[cellIdx]->sib2.prachCfg.prachCfgIdx);

      extIe->extensionValue.u.cztPRACH_Config.iE_Extns.noComp.pres =  NOTPRSNT;

      /* CSG_DEV start */
      if(TRUE == isCsgCfgd)
      {
         extIe = &protoie->servedCellInfo.iE_Extns.member[2];
         wrFillTknU8(&(extIe->pres), PRSNT_NODEF);
         wrFillTknU32(&(extIe->id), Cztid_CSG_Id);
         wrFillTknU32(&(extIe->criticality), CztCriticalityignoreEnum);
         csgId = wrEmmGetCsgIdFrmCellId(wrEmmCb.cellCb[cellIdx]->cellId);
         wrFillTknBStr32(&extIe->extensionValue.u.cztCSG_Id, WR_S1AP_CSG_ID_LEN,
               csgId);
      }
      /* CSG_DEV end */

      numComp = 0;
      nghInfo = &protoie->neighbour_Info;
      if(RFAILED == wrEmmFillNghCellInfoforX2Msg(x2SetupRspPdu,
               wrEmmCb.cellCb[cellIdx], nghInfo))
      {
         WR_FREE_EVNT(x2SetupRspPdu);
         RETVALUE(RFAILED);
      }
   }/*End of Num Cells Loop*/
   ieIdx++;

   /* Neighbouring info and GU group info are optional */
   /* IE3 - Filling GU GROUPID List */
   ie = &x2SetupRsp->protocolIEs.member[ieIdx];
   wrFillTknU8(&(ie->pres), PRSNT_NODEF);
   wrFillTknU32(&(ie->id), Cztid_GUGroupIDLst);
   wrFillTknU32(&(ie->criticality),CztCriticalityrejectEnum);
   numComp = wrEmmCb.guGrupIdLst.nmbEnt;

   wrFillTknU16(&(ie->value.u.cztGUGroupIDLst.noComp),numComp);


   if ((cmGetMem(x2SetupRspPdu,(numComp * sizeof(CztGU_Group_ID)),
               (Ptr *)&ie->value.u.cztGUGroupIDLst.member)) !=ROK)
   {
      RLOG0(L_FATAL," Memory allocation failed.");
      WR_FREE_EVNT(x2SetupRspPdu);
      RETVALUE(RFAILED);
   }

   /*MME_LDBAL: go through all the gu group Id in the hash list. insert in cztGUGroupIDLst IE.*/ 
   wrFillTknU16(&(ie->value.u.cztGUGroupIDLst.noComp),numComp);
   while((ret = cmHashListGetNext(&(wrEmmCb.guGrupIdLst), prevEnt, (PTR *) &guGrupCbList)) == ROK)
   {
      guIdIe = &ie->value.u.cztGUGroupIDLst.member[guIdx];
      wrFillTknU8(&(guIdIe->pres), PRSNT_NODEF);
      wrEmmCztFillPLMNId(x2SetupRspPdu,
            &(guGrupCbList->grupId.plmnId),
            &(guIdIe->pLMN_Identity));

      /*ccpu00128503*/
      mmeGrpId = CM_INET_HTON_U16(guGrupCbList->grupId.mmeGrpId);
      wrFill2TknStr4(&(guIdIe->mME_Group_ID),2,
            (U8 *)&mmeGrpId);
      /*ccpu00128503*/
      guIdIe->iE_Extns.noComp.pres = NOTPRSNT;
      guIdIe->iE_Extns.noComp.val = WR_VAL_ZERO;
      prevEnt = (PTR)guGrupCbList;
      guIdx++;
   }
   ieIdx++;

   *pdu = x2SetupRspPdu;

   RETVALUE(ROK);
}/*wrEmmBldX2SetupRsp*/


/** @brief This function builds X2 Setup Request
 *         CZT message.
 *
 * @details
 *
 *     Function: wrEmmNghEnbBldX2SetupReq
 *
 *         Processing steps:
 *         - Its constructs CZT X2 Setup Request
 *           message.
 *
 *
 * @param[out]   pdu : Czt Event Structure
 * @return S16 
 *      -# Success  : ROK
 *      -# Failure     : RFAILED
 */
PUBLIC S16 wrEmmNghEnbBldX2SetupReq
(
 CztEvnt                 **pdu
 )
{
   CztEvnt                   *x2SetupRqstPdu;
   U16                       numComp = 0;
   S16                       ret = 0;
   U8                        cellIdx = 0;
   U8                        plmnIdx = 0;
   U8                        guIdx = 0;
   CztSrvdCellsMember        *protoie;
#ifdef LTE_TDD
   CztTDD_Info               *tddInfo;
   CztSpecialSubframe_Info   *specialSfInfo;
#endif
   CztX2SetupRqst            *x2Setup;
   U8                        ieIdx = 0;
   CztProtIE_Field_X2SetupRqst_IEs *ie;
   CztGU_Group_ID            *guIdIe;
   CztProtExtnField_SrvdCell_Inform_ExtIEs *extIe;
   U8                        bw;
   CztNeighbour_Inform       *nghInfo;
   /*ccpu00128503*/
   U16                       mmeGrpId = 0;
   WrGuGrupCbList            *guGrupCbList = NULLP;
   PTR                       prevEnt = NULLP;
   /* CSG_DEV start */
   Bool                     isCsgCfgd;
   U32                      csgId;
   /* CSG_DEV end */
   /* fix for ccpu00142530 */
   U32                     eci_len = 0;


   TRC2(wrEmmNghEnbBldX2SetupReq);

   RLOG0(L_DEBUG,"Building of the X2 Setup Request ");

   /* Allocate and Initialize memory control point */
   ret = cmAllocEvnt(sizeof(CztEvnt), WR_CZ_MEM_SDU_SIZE,
         &wrCb.mem, (Ptr *)&x2SetupRqstPdu);
   if(ret != ROK)
   {
      RLOG0(L_FATAL,"Memory allocation failed.");
      RETVALUE(RFAILED);
   }
   /* Initialize the PDU */
   cmMemset((U8 *)&(x2SetupRqstPdu->pdu), 0, sizeof(CztX2AP_PDU));

   /* Fill the contents of initiatingMsg */
   wrFillTknU8(&(x2SetupRqstPdu->pdu.choice), X2AP_PDU_INITIATINGMSG);
   wrFillTknU8(&(x2SetupRqstPdu->pdu.val.initiatingMsg.pres),PRSNT_NODEF);
   wrFillTknU32(&(x2SetupRqstPdu->pdu.val.initiatingMsg.procedureCode),
         Cztid_x2Setup);
   wrFillTknU32(&(x2SetupRqstPdu->pdu.val.initiatingMsg.criticality),
         CztCriticalityrejectEnum);
   wrFillTknU8(&(x2SetupRqstPdu->pdu.val.initiatingMsg.value.u.
            cztX2SetupRqst.pres), PRSNT_NODEF);

   x2Setup = &x2SetupRqstPdu->pdu.val.initiatingMsg.value.u.cztX2SetupRqst;

   /* Filling 3 mandatory IEs  into the pdu */
   numComp = 3;
   wrFillTknU16(&(x2Setup->protocolIEs.noComp), numComp);

   /* Get memory for numComp */
   if ((cmGetMem(x2SetupRqstPdu, (numComp *
                  sizeof(CztProtIE_Field_X2SetupRqst_IEs)),
               (Ptr*)&x2Setup->protocolIEs.member)) != ROK)
   {
      RLOG0(L_FATAL, "Memory allocation failed.");
      /* Free the memory of Event Structure */
      WR_FREE_EVNT(x2SetupRqstPdu);
      RETVALUE(RFAILED);
   } /* end of if statement */

   /* IE1 - Filling ENB Global Id  in member[0]*/
   ie = &x2Setup->protocolIEs.member[ieIdx];
   wrFillTknU8(&(ie->pres), PRSNT_NODEF);
   wrFillTknU32(&(ie->id), Cztid_GlobalENB_ID);
   wrFillTknU32(&(ie->criticality), CztCriticalityrejectEnum);
   wrFillTknU8(&(ie->value.u.cztGlobalENB_ID.pres), PRSNT_NODEF);
   /* Fill the PLMN ID of source Cell Id */
   /*Filling of Global ENB ID */
   wrEmmCztFillGlbeNBId(x2SetupRqstPdu, wrEmmCb.enbType,
         wrEmmCb.enbId,
         &wrEmmCb.cellCb[0]->sib1.plmns[0].plmnId,
         &(ie->value.u.cztGlobalENB_ID));
   ieIdx++;

   /* IE2 - Filling Served Cells */
   ie = &x2Setup->protocolIEs.member[ieIdx];

   wrFillTknU8(&(ie->pres), PRSNT_NODEF);
   wrFillTknU32(&(ie->id), Cztid_SrvdCells);
   wrFillTknU32(&(ie->criticality),CztCriticalityrejectEnum);
   numComp = wrEmmCb.numCells;
   wrFillTknU16(&(ie->value.u.cztSrvdCells.noComp), numComp);

   if ((cmGetMem(x2SetupRqstPdu,(numComp * sizeof(CztSrvdCellsMember)),
               (Ptr *)&ie->value.u.cztSrvdCells.member)) !=ROK)
   {
      RLOG0(L_FATAL," Memory allocation failed.");
      WR_FREE_EVNT(x2SetupRqstPdu);
      RETVALUE(RFAILED);
   }
   for (cellIdx = 0; cellIdx < wrEmmCb.numCells;cellIdx++)
   {
      protoie = &ie->value.u.cztSrvdCells.member[cellIdx];

      wrFillTknU8(&(protoie->pres), PRSNT_NODEF);
      wrFillTknU8(&(protoie->servedCellInfo.pres), PRSNT_NODEF);
      wrFillTknU32(&(protoie->servedCellInfo.pCI),
            wrEmmCb.cellCb[cellIdx]->physCellId);
      wrFillTknU8(&(protoie->servedCellInfo.cellId.pres), PRSNT_NODEF);
      /* Using the first configured cell and the first PLMN ID in the list */
      wrEmmCztFillPLMNId(x2SetupRqstPdu,
            &(wrEmmCb.cellCb[cellIdx]->sib1.plmns[0].plmnId),
            &(protoie->servedCellInfo.cellId.pLMN_Identity));
      /* Fix for ccpu00142530 */
      eci_len = 28;
      if ( WR_ENODEB_TYPE_MACRO == wrEmmCb.enbType )
      {
         wrFillTknBStr32(&(protoie->servedCellInfo.cellId.eUTRANcellIdentifier),
               eci_len,wrEmmCb.cellCb[cellIdx]->sib1.cellIdentity);

      }
      else
      {
         wrFillTknBStr32(&(protoie->servedCellInfo.cellId.eUTRANcellIdentifier),
               eci_len,wrEmmCb.enbId);
      }

      wrEmmCztFillTAC(wrEmmCb.cellCb[cellIdx]->sib1.tac,x2SetupRqstPdu,
            &protoie->servedCellInfo.tAC);

      /*  Filling broadcastPLMNs */
      numComp = wrEmmCb.cellCb[cellIdx]->sib1.numPlmnIds;

      /* Allocate memory for broadcast PLMNs */
      if ((cmGetMem(x2SetupRqstPdu, numComp * sizeof(CztPLMN_Identity),
                  (Ptr*)&protoie->servedCellInfo.broadcastPLMNs.member)) !=ROK)
      {
         RLOG0(L_FATAL,"Memory allocation failed.");
         WR_FREE_EVNT(x2SetupRqstPdu);
         RETVALUE(RFAILED);
      }
      wrFillTknU16(&(protoie->servedCellInfo.broadcastPLMNs.noComp),numComp);

      for(plmnIdx = 0; plmnIdx < numComp; plmnIdx++)
      {
         /* Fill the PLMN ID of source Cell Id */
         wrEmmCztFillPLMNId(x2SetupRqstPdu,
               &(wrEmmCb.cellCb[cellIdx]->sib1.plmns[plmnIdx].plmnId),
               &(protoie->servedCellInfo.broadcastPLMNs.member[plmnIdx]));
      }

#ifndef LTE_TDD
      /*Filling of FDD Info*/
      wrFillTknU8(&(protoie->servedCellInfo.eUTRA_Mode_Info.choice),
            EUTRA_MODE_INFO_FDD);
      wrFillTknU8(&(protoie->servedCellInfo.eUTRA_Mode_Info.val.fDD.pres),
            PRSNT_NODEF);
      wrFillTknU32(&(protoie->servedCellInfo.eUTRA_Mode_Info.val.
               fDD.uL_EARFCN),
            wrEmmCb.cellCb[cellIdx]->sib2.ulCarrierFreq);
      wrFillTknU32(&(protoie->servedCellInfo.eUTRA_Mode_Info.val.
               fDD.dL_EARFCN),
            wrEmmCb.cellCb[cellIdx]->dlCarrierFreq);
      bw = wrEmmCb.cellCb[cellIdx]->sib2.ulBw;
      wrFillTknU32(&(protoie->servedCellInfo.eUTRA_Mode_Info.val.
               fDD.uL_Transmission_Bandwidth),
            bw);
      bw = wrEmmCb.cellCb[cellIdx]->mib.dlBw;
      wrFillTknU32(&(protoie->servedCellInfo.eUTRA_Mode_Info.val.
               fDD.dL_Transmission_Bandwidth),
            bw);
#else
      /*ccpu00134580-ADD-To support TDD in x2 setup*/
      /*Filling of TDD Info*/
      tddInfo = &protoie->servedCellInfo.eUTRA_Mode_Info.val.tDD;
      specialSfInfo = &tddInfo->specialSubframe_Info;
      wrFillTknU8(&(protoie->servedCellInfo.eUTRA_Mode_Info.choice),
            EUTRA_MODE_INFO_TDD);
      wrFillTknU8(&(tddInfo->pres),PRSNT_NODEF);
      /*For TDD both ARFCN and Bandwidth is same in DL & UL */
      wrFillTknU32(&(tddInfo->eARFCN),
            wrEmmCb.cellCb[cellIdx]->sib2.ulCarrierFreq);
      bw = wrEmmCb.cellCb[cellIdx]->sib2.ulBw;
      wrFillTknU32(&(tddInfo->transmission_Bandwidth),bw);
      wrFillTknU32(&(tddInfo->subframeAssignment),  wrEmmCb.cellCb[cellIdx]->sib1.tddCfg.subframeCfg);
      wrFillTknU8(&(specialSfInfo->pres), PRSNT_NODEF);
      wrFillTknU32(&(specialSfInfo->specialSubframePatterns), 
            wrEmmCb.cellCb[cellIdx]->sib1.
            tddCfg.specialSubframeCfg);
      wrFillTknU32( &(specialSfInfo->cyclicPrefixDL), 
            wrEmmCb.cellCb[cellIdx]->dlCyclicprefix );
      wrFillTknU32( &(specialSfInfo->cyclicPrefixUL), 
            wrEmmCb.cellCb[cellIdx]->sib2.ulCyclicPrefix);
#endif
      /*  Filling PRACH Information and Antenna Ports and CSG ID */
      /* CSG_DEV start */
      isCsgCfgd = wrEmmIsCsgCell(wrEmmCb.cellCb[cellIdx]->cellId);
      if(TRUE == isCsgCfgd)
      {
         numComp = WR_EMM_X2_CONFIG_EXTEN_IE_COUNT + 1;
      }
      else
      {
         numComp = WR_EMM_X2_CONFIG_EXTEN_IE_COUNT;
      }
      /* CSG_DEV end */
      /* Allocate memory for  PRACH Information and Antenna Ports*/
      if ((cmGetMem(x2SetupRqstPdu, numComp *
                  sizeof(CztProtExtnField_SrvdCell_Inform_ExtIEs),
                  (Ptr*)&protoie->servedCellInfo.iE_Extns.member)) !=ROK)
      {
         RLOG0(L_FATAL,"Memory allocation failed.");
         WR_FREE_EVNT(x2SetupRqstPdu);
         RETVALUE(RFAILED);
      }
      wrFillTknU16(&(protoie->servedCellInfo.iE_Extns.noComp),numComp);

      extIe = &protoie->servedCellInfo.iE_Extns.member[0];
      wrFillTknU8(&(extIe->pres), PRSNT_NODEF);
      wrFillTknU32(&(extIe->id), Cztid_Number_of_Antennaports);
      /* ccpu00134389: Corrected the criticality value filling as per spec
       * changed from Reject --> Ignore 
       */
      wrFillTknU32(&(extIe->criticality),CztCriticalityignoreEnum);
      wrFillTknU32(&(extIe->extensionValue.u.cztNumber_of_Antennaports),
            wrEmmCb.cellCb[cellIdx]->numAntPorts);

      extIe = &protoie->servedCellInfo.iE_Extns.member[1];
      wrFillTknU8(&(extIe->pres), PRSNT_NODEF);
      wrFillTknU32(&(extIe->id), Cztid_PRACH_Config);
      /* ccpu00134389: Corrected the criticality value filling as per spec
       * changed from Reject --> Ignore 
       */
      wrFillTknU32(&(extIe->criticality),CztCriticalityignoreEnum);


      wrFillTknU8(&(extIe->extensionValue.u.cztPRACH_Config.pres), PRSNT_NODEF);
      wrFillTknU32(&(extIe->extensionValue.u.cztPRACH_Config.rootSequenceIdx),
            wrEmmCb.cellCb[cellIdx]->sib2.prachCfg.rootSeqIdx);
      wrFillTknU32(&(extIe->extensionValue.u.cztPRACH_Config.zeroCorrelationIdx),
            wrEmmCb.cellCb[cellIdx]->sib2.prachCfg.zeroCorrZone);
      wrFillTknU8(&(extIe->extensionValue.u.cztPRACH_Config.highSpeedFlag),
            wrEmmCb.cellCb[cellIdx]->sib2.prachCfg.highSpeedFlag);
      wrFillTknU32(&(extIe->extensionValue.u.cztPRACH_Config.prach_FreqOffset),
            wrEmmCb.cellCb[cellIdx]->sib2.prachCfg.prachFreqOffset);
      wrFillTknU32(&(extIe->extensionValue.u.cztPRACH_Config.prach_ConfigIdx),
            wrEmmCb.cellCb[cellIdx]->sib2.prachCfg.prachCfgIdx);

      extIe->extensionValue.u.cztPRACH_Config.iE_Extns.noComp.pres =  NOTPRSNT;

      /* CSG_DEV start */
      if(TRUE == isCsgCfgd)
      {
         extIe = &protoie->servedCellInfo.iE_Extns.member[2];
         wrFillTknU8(&(extIe->pres), PRSNT_NODEF);
         wrFillTknU32(&(extIe->id), Cztid_CSG_Id);
         wrFillTknU32(&(extIe->criticality), CztCriticalityignoreEnum);
         csgId = wrEmmGetCsgIdFrmCellId(wrEmmCb.cellCb[cellIdx]->cellId);
         wrFillTknBStr32(&extIe->extensionValue.u.cztCSG_Id, WR_S1AP_CSG_ID_LEN,
               csgId);
      }
      /* CSG_DEV end */

      nghInfo = &protoie->neighbour_Info;
      if(RFAILED == wrEmmFillNghCellInfoforX2Msg(x2SetupRqstPdu,
               wrEmmCb.cellCb[cellIdx], nghInfo))
      {
         WR_FREE_EVNT(x2SetupRqstPdu);
         RETVALUE(RFAILED);
      }
   }/*End of Num Cells Loop*/
   ieIdx++;
   /* Neighbouring info and GU group info are optional */
   /* IE3 - Filling GU GROUPID List */
   ie = &x2Setup->protocolIEs.member[ieIdx];
   wrFillTknU8(&(ie->pres), PRSNT_NODEF);
   wrFillTknU32(&(ie->id), Cztid_GUGroupIDLst);
   wrFillTknU32(&(ie->criticality),CztCriticalityrejectEnum);
   numComp = wrEmmCb.guGrupIdLst.nmbEnt;

   wrFillTknU16(&(ie->value.u.cztGUGroupIDLst.noComp),
         numComp);


   if ((cmGetMem(x2SetupRqstPdu,(numComp * sizeof(CztGU_Group_ID)),
               (Ptr *)&ie->value.u.cztGUGroupIDLst.member)) !=ROK)
   {
      RLOG0(L_FATAL," Memory allocation failed.");
      WR_FREE_EVNT(x2SetupRqstPdu);
      RETVALUE(RFAILED);
   }        

   /*MME_LDBAL*/ 
   while((ret = cmHashListGetNext(&(wrEmmCb.guGrupIdLst), prevEnt, (PTR *) &guGrupCbList)) == ROK)
   {
      guIdIe = &ie->value.u.cztGUGroupIDLst.member[guIdx];
      wrFillTknU8(&(guIdIe->pres), PRSNT_NODEF);
      wrEmmCztFillPLMNId(x2SetupRqstPdu,
            &(guGrupCbList->grupId.plmnId),
            &(guIdIe->pLMN_Identity));

      /*ccpu00128503*/
      mmeGrpId = CM_INET_HTON_U16(guGrupCbList->grupId.mmeGrpId);
      wrFill2TknStr4(&(guIdIe->mME_Group_ID),2,
            (U8 *)&mmeGrpId);
      /*ccpu00128503*/
      guIdIe->iE_Extns.noComp.pres = NOTPRSNT;
      guIdIe->iE_Extns.noComp.val = 0;
      prevEnt = (PTR)guGrupCbList;
      guIdx++;
   }
   ieIdx++;

   *pdu = x2SetupRqstPdu;

   RETVALUE(ROK);
}/*wrEmmNghEnbBldX2SetupReq*/

/** @brief This function builds and delivers the PDU
 *         to the X2AP layer through IFM module.
 *
 * @details
 *
 *     Function: wrEmmNghEnbSndX2SetupReq
 *
 *         Processing steps:
 *         - Its an wrapper function to construct and
 *           deliver the X2 Setup Request message to the
 *           peer eNodeB.
 *
 *
 * @param[in]   peerId : Peer ID
 * @return S16 
 *     -# Success  : ROK
 */
PUBLIC S16 wrEmmNghEnbSndX2SetupReq
(
 WrNeighEnbCb                 *nghEnbCb
 )
{
   CztEvnt                   *x2SetupReq;
   U16                       retVal = ROK;

   TRC2(wrEmmNghEnbSndX2SetupReq);

   RLOG2(L_INFO,"Triggering X2 Setup Request for peerId [%ld] enbId [%ld]",
         nghEnbCb->peerId, nghEnbCb->enbId);

   /* Build X2 Setup message */
   retVal = wrEmmNghEnbBldX2SetupReq(&x2SetupReq);
   if(ROK != retVal)
   {
      RLOG0(L_ERROR,"Failed to build X2 Setup Req ");
      RETVALUE(retVal);
   }
   x2SetupReq->peerId = nghEnbCb->peerId;

   RLOG_ARG0(L_INFO,DBG_PEERID, nghEnbCb->peerId,
         "Sending X2 SETUP REQUEST");
   /*Start  the Timer*/
#if 0
#else
   wrStartTmr((PTR)nghEnbCb, WR_TMR_ENB_SETUP_RSP, wrEmmCb.tmrCfg.x2SetupTmr);
#endif
   /* Trigger corresponding primitive */
   if (ROK != WrIfmX2apGpReq(x2SetupReq))
   {
      RLOG0(L_ERROR,"Failed to Send X2 Setup Req ");
      wrStopTmr((PTR)nghEnbCb, WR_TMR_ENB_SETUP_RSP);
      retVal = RFAILED;
   }
   else
   {
      RLOG2(L_EVENT,"[X2AP]:Sending X2 SETUP REQUEST [Src(%ld)-->Peer(%ld)]",
            wrEmmCb.enbId, nghEnbCb->enbId);
   }
   RETVALUE(retVal);
} /* end of wrEmmNghEnbSndX2SetupReq*/
/* ccpu00122813 */
/* START */
/**
 *    @brief This function validate the two enodeB ID are equal or not
 *   
 *          Function: wrEmmCmpEnodebId
 *     
 *                Processing steps:
 *                 -  validate enodeB ID 
 *                  -  if matches 
 *                      Trigger x2Setup Failure to nbrEnb  and generate an
 *                      ALARM towards SM with proper cause as LCM_CAUSE_DUP_ENB_ID.
 @param[in]     :enbnghCb

 * @return S16
 *       -# Success : ROK
 *        -# Failure   : RFAILED
 *    
 */

PUBLIC Bool wrEmmCmpEnodebId
(
 U32            enbId1,
 U32            enbId2
 )
{
   if(enbId1 == enbId2)
   {
      RETVALUE(TRUE);
   } 
   else
   {
      RETVALUE(FALSE);
   }
}
/* END*/

/**
 *    @brief This function validate the two enodeB ID are equal or not
 *   
 *          Function: wrEmmValidateEnbId
 *     
 *                Processing steps:
 *                 -  validate enodeB ID 
 *                  -  if matches 
 *                      Trigger x2Setup Failure to nbrEnb  and generate an
 *                      ALARM towards SM with proper cause as LCM_CAUSE_DUP_ENB_ID.
 @param[in]     :enbnghCb

 * @return S16
 *       -# Success : ROK
 *        -# Failure   : RFAILED
 *    
 */
PUBLIC S16 wrEmmValidateEnbId
(
 WrNeighEnbCb   *enbnghCb,
 U32            enbId,
 U8             *causeTyp,
 U8             *causeVal
 )
{
   Bool  retVal = FALSE;
   /* If enodeb Id received in X2-Setup matches with target enodeb Id, send
    * Failure*/
   retVal = wrEmmCmpEnodebId(enbId,wrEmmCb.enbId);
   if(TRUE == retVal)
   {
      *causeTyp = WR_CZ_CAUSE_RADIONW;
      *causeVal = CztCauseRadioNwcell_not_availableEnum;
      RLOG2(L_ERROR," EnodebId[%lu] received in X2-Setup"
            "matches with Own EnodebId[%lu]",enbId, wrEmmCb.enbId);
      RLOG0(L_ERROR,"[ALARM] Sending ERRIND_FROM_PEER Alarm Cause DUP_ENB_ID to OAM" );
      wrSendLmAlarm(LCM_CATEGORY_PROTOCOL, LWR_EVENT_ERRIND_FROM_PEER,
            LCM_CAUSE_DUP_ENB_ID);
      RETVALUE(RFAILED);
   }
   retVal = wrEmmCmpEnodebId(enbId,enbnghCb->enbId);
   if(FALSE == retVal)
   {
      *causeTyp = WR_CZ_CAUSE_RADIONW;
      *causeVal = CztCauseRadioNwcell_not_availableEnum;
      RLOG2(L_ERROR,"EnodebId[%lu] received in X2-Setup "
            "Configured EnodebId[%lu]",enbId, enbnghCb->enbId);
      /* Generate Alarm Towards LM */
      RLOG0(L_ERROR,"[ALARM] Sending ERRIND_FROM_PEER Alarm Cause CELL_UNAVAILABLE to OAM" );
      wrSendLmAlarm(LCM_CATEGORY_PROTOCOL, LWR_EVENT_ERRIND_FROM_PEER,
            LWR_EVENT_CELL_UNAVAILABLE);
      RETVALUE(RFAILED);
   }
   RETVALUE(ROK);
}

/** @brief This function updates Global eNBId IE information
 *         into the neighbouring ENB CB.
 *
 * @details
 *
 *     Function: wrEmmNghEnbUpdEnbGlbId
 *
 *         Processing steps:
 *         - It parses Global eNB ID Ie from the message then
 *           updates the same to neighbouring eNB CB.
 *
 *
 * @param[out]  enbnghCb  : Neighbour eNodeB Control Block 
 * @param[in]    eNbIdIe     : eNodeB ID
 * @return S16 
 *    -# Success  : ROK
 */
PRIVATE S16 wrEmmNghEnbUpdEnbGlbId
(
 WrNeighEnbCb                 *enbnghCb,
 CztGlobalENB_ID              *eNbIdIe,
 U8                           *causeTyp,
 U8                           *causeVal
 )
{
   /*Fix for ccpu00126143 */
   U32              enbId = 0;
   U8               ret = RFAILED;        
   /*ccpu0012281*/
   enbnghCb->enbType = eNbIdIe->eNB_ID.choice.val;
   switch(enbnghCb->enbType)
   {
      case ENB_ID_MACRO_ENB_ID:
         wrEmmNghEnbParseTknStr4Ie(&eNbIdIe->eNB_ID.val.macro_eNB_ID,
               &enbId);
         break;
      case ENB_ID_HOME_ENB_ID:
         wrEmmNghEnbParseTknStr4Ie(&eNbIdIe->eNB_ID.val.home_eNB_ID,
               &enbId);
         break;
   }
   if(enbnghCb->isAnrDetected == TRUE)
   {
      enbnghCb->enbId = enbId;
      enbnghCb->isAnrDetected = FALSE;
   }
   ret = wrEmmValidateEnbId(enbnghCb, enbId, causeTyp, causeVal);
   if (ROK != ret)
   {
      RETVALUE(RFAILED);
   }

   RLOG1(L_INFO, "Updated ENB ID [%ld]",enbnghCb->enbId);
   wrEmmNghEnbParsePlmnIe(&eNbIdIe->pLMN_Identity,&enbnghCb->plmnId);
   RETVALUE(ROK);
   /* ccpu00122813*/
   /* START*/

   /*END*/
}
/* ccpu00122813 */
/* START */
/**
 *    @brief This function validate the two Ecgi are equal or not
 *   
 *          Function: wrEmmNghEcgiCmp
 *     
 *                Processing steps:
 *                 -  validate ecgi 
 *                  -  if matches 
 *                      Trigger x2Setup Failure to nbrEnb  and generate an
 *                      ALARM towards SM with proper cause as LCM_CAUSE_DUP_ECGI.
 @param[in]     :nghSrvdCellCb
 * @param[in]     :enbnghCb

 * @return S16
 *       -# Success : ROK
 *        -# Failure   : RFAILED
 *    
 */

PUBLIC S16 wrEmmNghEcgiCmp
(
 WrEutranNeighCellCb    *nghSrvdCellCb,
 WrNeighEnbCb                 *enbnghCb
 )
{

   WrCgi                   cgi;
   wrEmmGetCgi(wrEmmCb.cellCb[0]->cellId,&cgi);
   if((cgi.cellId == nghSrvdCellCb->eCgi.eutranCellId) && (wrUtlPlmnsEqual(&cgi.plmnId, &nghSrvdCellCb->eCgi.plmnId) == TRUE))
   {
      RETVALUE(RFAILED);
   }
   RETVALUE(ROK);
}

/* END*/


/** @brief This function parses the Served Cell infromation
 *         IE from the message and updates the neighbouring
 *         eNodeB CB.
 *
 * @details
 *
 *     Function: wrEmmNghEnbUpdSrvdCellInfo
 *
 *         Processing steps:
 *         - It processes the Served Cell Info IE from the
 *           message.
 *         - All the information from the message are updated
 *           into Neighbouring eNodeB CB.
 *
 *
 * @param[out]      enbnghCb  : Neighbour eNodeB Control Block
 * @param[in]        srvdCellIe  : Serverd Cell Info IE
 * @return S16 
 *     -# Success : ROK
 *     -# Failure   : RFAILED
 */
PRIVATE S16 wrEmmNghEnbUpdSrvdCellInfo
(
 WrNeighEnbCb                 *enbnghCb,
 CztSrvdCells                 *srvdCellIe,
 U8                           *causeTyp,
 U8                           *causeVal
 )
{
   WrEutranNeighCellCb    *nghSrvdCellCb;
   U8                     numSrvdCells;
   U8                     idx;
   U8                     plmnidx;
   CztSrvdCellsMember     *ie;
#ifdef LTE_TDD
   CztTDD_Info            *tddInfo;
   CztSpecialSubframe_Info   *specialSfInfo;
#endif
   WrNeighNeighCb         *nghNeighCb = NULLP;
   CztNeighbour_InformMember  *nghIe;
   U8                     ngIdx;
   S16                    ret = ROK;
   CmLListCp              newServCellLst;
   CmLList                *nrLnk = NULLP;
   Bool                   isNewCell = FALSE;
   U8                     cellIdx = 0; /* First cell */
   /* CSG_DEV */
   U8                     extIeCnt = 0;
   Bool                   isCsgIdSentInMsg = FALSE;

   /* Call Anr to initialize the ANR-MEAS interface data structure 
    * to send the NR update info to Measurement 
    */

   for ( cellIdx = 0; cellIdx < wrEmmCb.numCells; cellIdx++ )
   {
      wrEmmAnrInitMeasNrUpdtInfo(wrEmmCb.cellCb[cellIdx]);
   }

   /* Delete the remaining cell, which are gone down at neighbor */
   /*ccpu00149007 : When the x2 request/response comes from the neighbour, 
     we are deleting all the existing neighbour in the in use list first and 
     adding the new entries of the neighbours reported in X2 setup request/response later
     This will take care of maintaing/updating proper values at OAM,
     if there are any inconsistancy in configured and reported value.*/
#if 0
   if(enbnghCb->srvdCellLst.count)
   {
      CM_LLIST_FIRST_NODE((&(enbnghCb->srvdCellLst)), nrLnk);

      while(NULLP != nrLnk)
      {
         nghSrvdCellCb = (WrEutranNeighCellCb *)nrLnk->node;


         /* delete the cell from the serving cell list */
         for ( cellIdx = 0; cellIdx < wrEmmCb.numCells; cellIdx++ )
         {
            wrEmmAnrDelEutranNr(wrEmmCb.cellCb[cellIdx], nghSrvdCellCb);
         } 

         /* delete the neighbor cell node from the serving cell list of neighbor
          * eNB
          */
         cmLListDelFrm(&(enbnghCb->srvdCellLst), nrLnk);

         /* Free the memory of the neighbor cell node */
         WR_FREE(nghSrvdCellCb,sizeof(WrEutranNeighCellCb));

         CM_LLIST_FIRST_NODE((&(enbnghCb->srvdCellLst)), nrLnk);
      }
   }
#endif

   numSrvdCells = srvdCellIe->noComp.val;
   /* Initialize the temporary New erving cell list */ 
   cmLListInit(&newServCellLst);   

   for(idx = 0;idx < numSrvdCells;idx++)
   {
      ie =&srvdCellIe->member[idx];

      nghSrvdCellCb = wrEmmGetNeighEnbSrvdCell(enbnghCb, 
            ie->servedCellInfo.pCI.val);

      if(NULLP == nghSrvdCellCb)
      {
         WR_ALLOC(&nghSrvdCellCb, sizeof(WrEutranNeighCellCb));
         if (NULLP == nghSrvdCellCb)
         {
            RLOG0(L_FATAL,"Memory allocation failed.");
            RETVALUE(RFAILED);
         }

         isNewCell = TRUE;

         nghSrvdCellCb->srvdCellLstEnt.node = (PTR)nghSrvdCellCb;
         nghSrvdCellCb->isHeMsCell = FALSE;
      }
      else
      {
         cmLListDelFrm(&(enbnghCb->srvdCellLst), 
               &nghSrvdCellCb->srvdCellLstEnt);
      }

      /* sps - ho change */
      nghSrvdCellCb->enbId = enbnghCb->enbId;
      nghSrvdCellCb->enbType = enbnghCb->enbType;
      cmMemcpy((U8 *)&nghSrvdCellCb->enbPlmn,
            (U8 *)&enbnghCb->plmnId, sizeof(enbnghCb->plmnId));

      if(wrEmmNghEcgiCmp(nghSrvdCellCb,enbnghCb) != ROK)
      {
         *causeTyp = WR_CZ_CAUSE_RADIONW;
         *causeVal = CztCauseRadioNwcell_not_availableEnum;
         /* Generate Alarm Towards LM */
         RLOG0(L_ERROR,"[ALARM] Sending ERRIND_FROM_PEER Alarm Cause DUP_ECGI to OAM" );
         wrSendLmAlarm(LCM_CATEGORY_PROTOCOL, LWR_EVENT_ERRIND_FROM_PEER,
               LCM_CAUSE_DUP_ECGI);
         RETVALUE(RFAILED);
      }
      nghSrvdCellCb->pci = ie->servedCellInfo.pCI.val;
      nghSrvdCellCb->cio = WR_EMM_DEFAULT_OFFSET;
      nghSrvdCellCb->qoffset = WR_EMM_DEFAULT_OFFSET;
      wrEmmNghEnbParsePlmnIe(&ie->servedCellInfo.cellId.pLMN_Identity,
            &nghSrvdCellCb->eCgi.plmnId);

      nghSrvdCellCb->tac = ie->servedCellInfo.tAC.val[0];
      nghSrvdCellCb->tac <<= 8;
      nghSrvdCellCb->tac |= ie->servedCellInfo.tAC.val[1];

      wrEmmNghEnbParseTknStr4Ie(&ie->servedCellInfo.cellId.eUTRANcellIdentifier,
            &nghSrvdCellCb->eCgi.eutranCellId);

      RLOG3(L_INFO,"Adding cell pci [%d] eutranCellId [%ld] for enbId [%ld]"
            ,nghSrvdCellCb->pci,nghSrvdCellCb->eCgi.eutranCellId,
            nghSrvdCellCb->enbId);
      RLOG2(L_INFO,"enbType [%d] peerId [%ld]", nghSrvdCellCb->enbType,
            enbnghCb->peerId);

#ifndef LTE_TDD
      if((nghSrvdCellCb->earfcnDl != 
               ie->servedCellInfo.eUTRA_Mode_Info.val.fDD.dL_EARFCN.val) &&
            isNewCell != TRUE)
      {
         /* delete the cell front eh serving cell list */
         for ( cellIdx = 0; cellIdx < wrEmmCb.numCells; cellIdx++ )
         {
            wrEmmAnrDelEutranNr(wrEmmCb.cellCb[cellIdx], nghSrvdCellCb);
         }        

         isNewCell = TRUE;
      }

      nghSrvdCellCb->earfcnDl   = ie->servedCellInfo.
         eUTRA_Mode_Info.val.fDD.dL_EARFCN.val;

      nghSrvdCellCb->earfcnUl = 
         ie->servedCellInfo.eUTRA_Mode_Info.val.fDD.uL_EARFCN.val;
      nghSrvdCellCb->dlBw       = ie->servedCellInfo.
         eUTRA_Mode_Info.val.fDD.
         dL_Transmission_Bandwidth.val;
      nghSrvdCellCb->ulBw       = ie->servedCellInfo.
         eUTRA_Mode_Info.val.fDD.
         uL_Transmission_Bandwidth.val;
#else
      /*ccpu00134580-ADD-To support TDD in x2 setup*/
      if((nghSrvdCellCb->earfcnDl != 
               ie->servedCellInfo.eUTRA_Mode_Info.val.tDD.eARFCN.val) &&
            isNewCell != TRUE)
      {
         /* delete the cell front eh serving cell list */
         for ( cellIdx = 0; cellIdx < wrEmmCb.numCells; cellIdx++ )
         {
            wrEmmAnrDelEutranNr(wrEmmCb.cellCb[cellIdx], nghSrvdCellCb);
         }        

         isNewCell = TRUE;
      }

      tddInfo       = &ie->servedCellInfo.eUTRA_Mode_Info.val.tDD;
      specialSfInfo = &tddInfo->specialSubframe_Info;

      nghSrvdCellCb->earfcnDl        = tddInfo->eARFCN.val;
      nghSrvdCellCb->earfcnUl        = nghSrvdCellCb->earfcnDl;
      nghSrvdCellCb->dlBw            = tddInfo->transmission_Bandwidth.val;
      nghSrvdCellCb->ulBw            = nghSrvdCellCb->dlBw;
      nghSrvdCellCb->ulDlSubframeCfg = tddInfo->subframeAssignment.val;
      nghSrvdCellCb->splSfCfg        = specialSfInfo->specialSubframePatterns.val;
      nghSrvdCellCb->cpDl            = specialSfInfo->cyclicPrefixDL.val;
      nghSrvdCellCb->cpUl            = specialSfInfo->cyclicPrefixUL.val;

#endif
      nghSrvdCellCb->numPlmn    = ie->servedCellInfo.
         broadcastPLMNs.noComp.val;

      for (plmnidx = 0; plmnidx < nghSrvdCellCb->numPlmn;plmnidx++)
      {
         wrEmmNghEnbParsePlmnIe(&ie->servedCellInfo.broadcastPLMNs.
               member[plmnidx],
               &nghSrvdCellCb->plmn[plmnidx]);
      }

      /* PRACH and Antenna Info*/
      if(ie->servedCellInfo.iE_Extns.noComp.pres)
      {
         /* CSG_DEV start */
         for(extIeCnt = 0; extIeCnt < ie->servedCellInfo.iE_Extns.noComp.val;
               extIeCnt++)
         {
            switch(ie->servedCellInfo.iE_Extns.member[extIeCnt].id.val)
            {
               case Cztid_Number_of_Antennaports:
                  {
                     nghSrvdCellCb->numAntenna = ie->servedCellInfo.iE_Extns.
                        member[extIeCnt].extensionValue.u.
                        cztNumber_of_Antennaports.val;
                  }
                  break;
               case Cztid_PRACH_Config:
                  {
                     nghSrvdCellCb->prachCfg.rootSeqIdx =
                        ie->servedCellInfo.iE_Extns.
                        member[extIeCnt].extensionValue.u.cztPRACH_Config.
                        rootSequenceIdx.val;
                     nghSrvdCellCb->prachCfg.prachCfgIdx
                        = ie->servedCellInfo.iE_Extns.
                        member[extIeCnt].extensionValue.u.cztPRACH_Config.
                        prach_ConfigIdx.val;
                     nghSrvdCellCb->prachCfg.highSpeedFlag
                        = ie->servedCellInfo.iE_Extns.
                        member[extIeCnt].extensionValue.u.cztPRACH_Config.
                        highSpeedFlag.val;
                     nghSrvdCellCb->prachCfg.zeroCorrZone
                        = ie->servedCellInfo.iE_Extns.
                        member[extIeCnt].extensionValue.u.cztPRACH_Config.
                        zeroCorrelationIdx.val;
                     nghSrvdCellCb->prachCfg.prachFreqOffset
                        = ie->servedCellInfo.iE_Extns.
                        member[extIeCnt].extensionValue.u.cztPRACH_Config.
                        prach_FreqOffset.val;
                  }
                  break;
               case Cztid_CSG_Id:
                  {
                     isCsgIdSentInMsg = TRUE;
                     wrEmmCsgNbrUpdCsgInfoFrmExtnIe(
                           &(ie->servedCellInfo.iE_Extns.member[extIeCnt].
                              extensionValue.u.cztCSG_Id), nghSrvdCellCb);
                     nghSrvdCellCb->isCsgAccessModeResolved =
                        WR_EMM_CELL_ACCESS_MODE_NOT_DETERMINED;
                  }
                  break;
               default:
                  break;
            }
         }
         /* CSG_DEV end */
      }

      /* Memory leak fix ccpu00145573 */
      /*Clear neighour of Old neighour cells if present */
      if(nghSrvdCellCb->neighNeighLstCp.count != 0)
      {
         CM_LLIST_FIRST_NODE((&nghSrvdCellCb->neighNeighLstCp), nrLnk);
         while(nrLnk != NULLP)
         {
            cmLListDelFrm((&nghSrvdCellCb->neighNeighLstCp), nrLnk);
            WR_FREE(nrLnk->node, sizeof(WrNeighNeighCb));
            CM_LLIST_FIRST_NODE((&nghSrvdCellCb->neighNeighLstCp), nrLnk);
         }
      }

      for(ngIdx = 0;ngIdx < ie->neighbour_Info.noComp.val;ngIdx++)
      {
         nghIe =&ie->neighbour_Info.member[ngIdx];

         WR_ALLOC(&nghNeighCb, sizeof(WrNeighNeighCb));
         if(NULLP == nghNeighCb)
         {
            RLOG0(L_FATAL,"Memory allocation failed.");
            RETVALUE(RFAILED);
         }

         nghNeighCb->dlEarfcn = nghIe->eARFCN.val;
         nghNeighCb->pci      = nghIe->pCI.val;

         wrEmmNghEnbParseTknStr4Ie(&nghIe->eCGI.eUTRANcellIdentifier,
               &nghNeighCb->ecgi.eutranCellId);
         wrEmmNghEnbParsePlmnIe(&nghIe->eCGI.pLMN_Identity,
               &nghNeighCb->ecgi.plmnId);
         /*Update TAC*/
         for(extIeCnt = 0; extIeCnt < nghIe->iE_Extns.noComp.val;
               extIeCnt++)
         {
            switch(nghIe->iE_Extns.member[extIeCnt].id.val)
            {
               case Cztid_NeighbourTAC: /*Update received TAC in nghNeighCb*/
                   wrEmmNghEnbParseTknStr4Ie(&(nghIe->iE_Extns.member[extIeCnt].
                               extensionValue.u.cztTAC), &(nghNeighCb->tac));
                   break;
            }
         }
         /*Adding the new neighbours neighbour control block*/
         /* sri - HOFIX */
         nghNeighCb->LstEnt.node = (PTR)nghNeighCb;
         cmLListAdd2Tail(&(nghSrvdCellCb->neighNeighLstCp),
               &(nghNeighCb->LstEnt));
      }/*End of Neighbours Neighbour Loop*/


      /* Inserting serv cell to new Served Cell list */
      cmLListAdd2Tail(&(newServCellLst), &nghSrvdCellCb->srvdCellLstEnt);

      for (cellIdx = 0; cellIdx < wrEmmCb.numCells; cellIdx++)
      {
         /* access mode is derived only for neighbors detected
          * via ANR/X2 Setup procedure */
         /* access mode of neighbors configured via HeMS remains
          * untouched */
         /* ccpu00148590 */
         if(FALSE == nghSrvdCellCb->isHeMsCell)
         {
            if(WR_EMM_CELL_ACCESS_MODE_NOT_DETERMINED == 
                  nghSrvdCellCb->isCsgAccessModeResolved)
            {
               wrEmmCsgSetNbrAccessMode(wrEmmCb.cellCb[cellIdx]->cellId,
                     nghSrvdCellCb->pci, isCsgIdSentInMsg,
                     &nghSrvdCellCb->csgAccessMode);
               nghSrvdCellCb->isCsgAccessModeResolved =
                  WR_EMM_CELL_ACCESS_MODE_DETERMINED;
               /* ccpu00148589 */
               if(WR_ENB_OPEN_ACCESS == nghSrvdCellCb->csgAccessMode)
               {
                  nghSrvdCellCb->isCsgIdResolved = WR_EMM_NBR_CSG_ID_DISCOVERED;
               }
            }
         }

         if(isNewCell == TRUE)
         {
            ret = wrEmmAnrAddEutranNr(wrEmmCb.cellCb[cellIdx], nghSrvdCellCb);

            if(RFAILED == ret)
            {
               RLOG1(L_ERROR,"Nr Cell Add In ANR failed"
                     "to cell [%d]",wrEmmCb.cellCb[cellIdx]->cellId);
            }
         }

         isNewCell = FALSE;
      }
   }
   /* Copy the new list into the Neighbor Enb Cb*/
   enbnghCb->srvdCellLst = newServCellLst;

   /* Process NR Update Info and send Trigger to Meas and PBM module */
   for ( cellIdx = 0; cellIdx < wrEmmCb.numCells; cellIdx++ )
   {
      ret = wrEmmAnrProcNrUpdtInfo(wrEmmCb.cellCb[cellIdx], FALSE);
   }

   RETVALUE(ret);
}

/** @brief This function parses GU GroupID List IE from the
 *         message and updates the Neighbouring eNB Cb.
 *
 * @details
 *
 *     Function: wrEmmNghEnbUpdGuGrpIdLst
 *
 *         Processing steps:
 *         - It processes all the information from GU GroupId List
 *           IE from the message and updates the same into the
 *           Neighbouring eNB CB.
 *
 *
 * @param[out] enbnghCb   : Neighbour eNodeB Control Block
 * @param [in]   guGrpIdIe  : GU Group ID IE
 * @return S16 
 *    -# Success : ROK
 */

PRIVATE S16 wrEmmNghEnbUpdGuGrpIdLst
(
 WrNeighEnbCb                 *enbnghCb,
 CztGUGroupIDLst              *guGrpIdIe,
 U8                           *causeTyp,
 U8                           *causeVal
 )
{
   U8                        idx;
   CztGU_Group_ID            *grpId;


   enbnghCb->numGuGrpIds = guGrpIdIe->noComp.val;
   for (idx = 0;idx < enbnghCb->numGuGrpIds;idx++)
   {
      grpId = &guGrpIdIe->member[idx];
      enbnghCb->guGrpIdLst[idx].mmeGrpId = grpId->mME_Group_ID.val[0];
      enbnghCb->guGrpIdLst[idx].mmeGrpId = (enbnghCb->guGrpIdLst[idx].mmeGrpId << 8) | grpId->mME_Group_ID.val[1];
      wrEmmNghEnbParsePlmnIe(&grpId->pLMN_Identity,
            &enbnghCb->guGrpIdLst[idx].plmnId);
      RLOG3(L_INFO,"Peer [%ld] enbId [%ld] MME Group Id [%d]",
            enbnghCb->peerId, enbnghCb->enbId,
            enbnghCb->guGrpIdLst[idx].mmeGrpId);
   }
   RETVALUE(ROK);
}
/** @brief This function updates the Neighbouring eNodeB CB
 *         with the information received from the X2AP message.
 *
 * @details
 *
 *     Function: wrEmmNghEnbUpdateNghEnbCb
 *
 *         Processing steps:
 *         - It pasres all the information elements from the
 *           message.
 *         - It updates the necessary information from the message
 *           into the Neighbouring eNodeB CB.
 *
 *
 * @param[out] enbnghCb  : Neighbour eNodeB Control Block
 * @param[in]   setupRsp   : X2 Setup Response Message
 * @return S16 
 *     -# Success : ROK
 */
PRIVATE S16 wrEmmNghEnbUpdateNghEnbCb
(
 WrNeighEnbCb                 *enbnghCb,
 CztX2SetupResp               *setupRsp
 )
{
   U8             idx;
   /*X2AP_FIX */
   S16            ret = ROK;
   U8             causeTyp;
   U8             causeVal;

   CztProtIE_Field_X2SetupResp_IEs *IEs = setupRsp->protocolIEs.member;

   for(idx = 0; ((idx < setupRsp->protocolIEs.noComp.val) && (ret == ROK)); idx++)
   {
      CztProtIE_Field_X2SetupResp_IEs *ie = IEs + idx;

      switch(ie->id.val)
      {
         case Cztid_GlobalENB_ID:
            ret = wrEmmNghEnbUpdEnbGlbId(enbnghCb, &ie->value.u.cztGlobalENB_ID, &causeTyp, &causeVal);
            break;
         case Cztid_SrvdCells:
            ret = wrEmmNghEnbUpdSrvdCellInfo(enbnghCb, &ie->value.u.cztSrvdCells, &causeTyp, &causeVal);
            break;
         case Cztid_GUGroupIDLst:
            ret = wrEmmNghEnbUpdGuGrpIdLst(enbnghCb, &ie->value.u.cztGUGroupIDLst, &causeTyp, &causeVal);
            break;
      }
   }
   if(ret !=ROK)
   {
      /* Processing of X2 setup Response failed, take proper action:[TODO] */
      RLOG0(L_ERROR,"Processing of X2 setup Response Failed ");

      RETVALUE(RFAILED);
   }
   RETVALUE(ret);
}/*wrEmmNghEnbUpdateNghEnbCb*/

/** @brief This function parses all the IEs from the
 *         X2 Setup request message.
 *
 * @details
 *
 *     Function: wrEmmNghEnbUpdNghCbFromReq
 *
 *         Processing steps:
 *         - It parses all the IEs from the X2 Setup request
 *           message and then call correponding handler to
 *           update the information based in IE Ids.
 *
 *
 * @param[out]   enbnghCb  : Neighbour eNodeB Control Block
 * @param[in]     setupReq   : X2 Setup Request Message
 * @return S16 
 *      -# Success : ROK
 */
PRIVATE S16 wrEmmNghEnbUpdNghCbFromReq
(
 WrNeighEnbCb                 *enbnghCb,
 CztX2SetupRqst               *setupReq
 )
{
   U8             idx;
   /*X2AP_FIX */
   S16            ret = ROK;
   U8             causeTyp;
   U8             causeVal;

   CztProtIE_Field_X2SetupRqst_IEs *IEs = setupReq->protocolIEs.member;

   for(idx = 0; ((idx < setupReq->protocolIEs.noComp.val) && (ret == ROK)); idx++)
   {
      CztProtIE_Field_X2SetupRqst_IEs  *ie = IEs + idx;

      switch(ie->id.val)
      {
         case Cztid_GlobalENB_ID:
            ret = wrEmmNghEnbUpdEnbGlbId(enbnghCb, &ie->value.u.cztGlobalENB_ID, &causeTyp, &causeVal);
            break;
         case Cztid_SrvdCells:
            ret = wrEmmNghEnbUpdSrvdCellInfo(enbnghCb,&ie->value.u.cztSrvdCells, &causeTyp, &causeVal);
            break;
         case Cztid_GUGroupIDLst:
            ret = wrEmmNghEnbUpdGuGrpIdLst(enbnghCb, &ie->value.u.cztGUGroupIDLst,&causeTyp, &causeVal);
            break;
      }
   }
   if(ret !=ROK)
   {
      /* Send X2-Setup Failure Message */
      wrEmmNghEnbSndX2setupFail(enbnghCb->peerId, FALSE, causeTyp, causeVal);
   }
   RETVALUE(ret);
}/*wrEmmNghEnbUpdateNghEnbCb*/

/** @brief This function updates Global eNBId IE information
 *         into the neighbouring ENB CB.
 *
 * @details
 *
 *     Function: wrEmmNghGetEnbIdFrmSetupReq
 *
 *         Processing steps:
 *         - get  Global eNB ID Ie from the message.
 *
 *
 * @param[out]  enbId  : eNodeB ID
 * @param[in]   setupReq : X2Setup Request 
 * @return S16 
 *    -# Success  : ROK
 */
PRIVATE S16 wrEmmNghGetEnbIdFrmSetupReq
(
 U32                          *enbId,
 CztX2SetupRqst               *setupReq
 )
{
   U32              enbType = 0;
   CztProtIE_Field_X2SetupRqst_IEs *IEs = setupReq->protocolIEs.member;
   U32              idx = 0;

   for(idx = 0; idx < setupReq->protocolIEs.noComp.val; idx++)
   {
      CztProtIE_Field_X2SetupRqst_IEs  *ie = IEs + idx;
      if(ie->id.val == Cztid_GlobalENB_ID)
      {
         CztGlobalENB_ID  *eNbIdIe = &ie->value.u.cztGlobalENB_ID;
         enbType = eNbIdIe->eNB_ID.choice.val;
         if(enbType == ENB_ID_MACRO_ENB_ID)
         {
            wrEmmNghEnbParseTknStr4Ie(&eNbIdIe->eNB_ID.val.macro_eNB_ID, enbId);
         }
         else
         {
            wrEmmNghEnbParseTknStr4Ie(&eNbIdIe->eNB_ID.val.home_eNB_ID, enbId);
         }
         break;
      }
   }
   RETVALUE(ROK);
} 
#ifdef WR_RSYS_OAM
/** @brief This function updates Global eNBId IE information
 *         into the neighbouring ENB CB.
 *
 * @details
 *
 *     Function: wrEmmNghGetEnbIdFrmSetupRsp
 *
 *         Processing steps:
 *         - get  Global eNB ID Ie from the message.
 *
 *
 * @param[out]  enbId  : enbId
 * @param[in]   setupRsp : X2Setup respose pdu 
 * @return S16 
 *    -# Success  : ROK
 */
PRIVATE S16 wrEmmNghGetEnbIdFrmSetupRsp
(
 U32                          *enbId,
 CztX2SetupResp               *setupRsp
 )
{
   U32              enbType = 0;
   CztProtIE_Field_X2SetupResp_IEs *IEs = setupRsp->protocolIEs.member;
   U32              idx = 0;

   for(idx = 0; idx < setupRsp->protocolIEs.noComp.val; idx++)
   {
      CztProtIE_Field_X2SetupResp_IEs  *ie = IEs + idx;
      if(ie->id.val == Cztid_GlobalENB_ID)
      {
         CztGlobalENB_ID  *eNbIdIe = &ie->value.u.cztGlobalENB_ID;
         enbType = eNbIdIe->eNB_ID.choice.val;
         if(enbType == ENB_ID_MACRO_ENB_ID)
         {
            wrEmmNghEnbParseTknStr4Ie(&eNbIdIe->eNB_ID.val.macro_eNB_ID, enbId);
         }
         else
         {
            wrEmmNghEnbParseTknStr4Ie(&eNbIdIe->eNB_ID.val.home_eNB_ID, enbId);
         }
         break;
      }
   }
   RETVALUE(ROK);
} 
#
#endif

/** @brief This function handles the X2AP Setup request from
 *         the peer eNodeB.
 *
 * @details
 *
 *     Function: wrEmmNghEnbPrcSetupReq
 *
 *         Processing steps:
 *         - It processes the X2 Setup request message.
 *         - It stores all the information about the peer
 *           into the Neighbouring eNodeB CB.
 *         - It builds X2 Setup response message.
 *         - It delivers the constructed PDU in reponse to
 *           X2 Setup request message from the peer through
 *           IFM module.
 *
 *
 * @param[in]    peerId : Peer ID
 * @param[in]    pdu     : Czt Event Structure
 * @return S16 
 *    -# Success : ROK
 *    -# Failure   : RFAILED
 */

PRIVATE S16 wrEmmNghEnbPrcSetupReq
(
 U32                          peerId,
 CztEvnt                      *pdu
 )
{
   WrNeighEnbCb              *enbNghCb = NULLP;
   CztInitiatingMsg          *intMsg = &pdu->pdu.val.initiatingMsg;
   CztX2SetupRqst            *setupReq = &intMsg->value.u.cztX2SetupRqst;
   S16                       ret = ROK;
   CztEvnt                   *x2SetupRspPdu = NULLP;
   Bool                       enbCbPres = TRUE;
   U32                        enbId = 2555;
   U8                         cellIdx = 0; /* First cell */
   WrCellCb                   *cellCb = NULLP;

   wrEmmNghGetEnbIdFrmSetupReq(&enbId, setupReq);
   WR_FREE_DUPICATE_PEERID_FROM_ENBID(enbId, peerId);
   RLOG2(L_INFO, 
         "Received X2 Setup Request from PeerId [%ld] enbId [%ld]",
         peerId, enbId);
   RLOG2(L_EVENT,"[X2AP]:Received X2 SETUP REQUEST [Peer(%ld)<--Src(%ld)]",
         wrEmmCb.enbId,enbId);

   /*Finding the neighbour eNodeB control block*/
   ret = cmHashListFind(&(wrEmmCb.neighEnbLstCp),(U8 *)&enbId,sizeof(enbId),0,
         (PTR *)&enbNghCb);
   if (ret != ROK)
   {
      /*Allocate the Neighbouring CB for this peer*/
      WR_ALLOC(&enbNghCb, sizeof(WrNeighEnbCb));
      if(NULLP == enbNghCb)
      {
         RLOG0(L_FATAL,"Memory allocation failed.");
         RETVALUE(RFAILED);
      }
      enbCbPres = FALSE;
      /* LTE_ADV starts */
      enbNghCb->enbId = enbId;
      /* LTE_ADV ends */
      enbNghCb->isAnrDetected = TRUE;
      cmLListInit(&(enbNghCb->srvdCellLst));
      cmInitTimers(&(enbNghCb->timer), 1);
      cmInitTimers(&(enbNghCb->enbCfgUpTm), 1);
      cmInitTimers(&(enbNghCb->enbcfgTransTmr), 1);

   }
   /* sri - temoporary change - need to change after fixing simultanuous x2 setup issue */
#ifdef WR_TEST_CODE
   else if ( wrTgrX2SetupFail == TRUE )
   {
      RLOG1(L_DEBUG,"Incoming X2 setup not supported. "
            "Sending X2 Setup Failure to PeerId [%ld]", peerId);
      wrEmmNghEnbSndX2setupFail(peerId, TRUE, WR_CZ_CAUSE_TRANSPORT,CztCauseTporttransport_resource_unavailableEnum);
      RETVALUE(ROK);
   }

   if ( wrDntRspX2Setup == TRUE )
   {
      RETVALUE(ROK);
   }
   if ( wrTgrX2SetupFail == TRUE )
   {
      wrEmmNghEnbSndX2setupFail(peerId, TRUE, WR_CZ_CAUSE_TRANSPORT,CztCauseTporttransport_resource_unavailableEnum);
      RETVALUE(ROK);
   }
   if ( wrTgX2SetupFailWoTW == TRUE )
   {
      wrEmmNghEnbSndX2setupFail(peerId, FALSE, WR_CZ_CAUSE_TRANSPORT,CztCauseTporttransport_resource_unavailableEnum);
      RETVALUE(ROK);
   }

#endif /* WR_TEST_CODE */
   /* Send X2 Setup failure with timeToWait if CELL State not UP */
   /* Assumptios: Only one cell */
   cellCb = wrEmmCb.cellCb[cellIdx];
   if ( wrEmmCb.cellCb[cellIdx]->cellState != WR_CELL_STATE_UP )
   {
      RLOG0(L_DEBUG,"*** Sending X2Setup Fail as cell state is down ****");
      wrEmmNghEnbSndX2setupFail(peerId, TRUE, WR_CZ_CAUSE_TRANSPORT,CztCauseTporttransport_resource_unavailableEnum);
      RETVALUE(RFAILED);
   } 
   /*Update the Neighbouring ENB CB*/
   if(ROK != wrEmmNghEnbUpdNghCbFromReq(enbNghCb,setupReq))
   {
      RLOG0(L_ERROR,"Failure in updating  Neighbouring eNB CB ");
      RETVALUE(RFAILED);
   }

   enbNghCb->peerId        = peerId;
   enbNghCb->x2Connection = WR_NEIGH_ENB_STATE_UP;
   RLOG_ARG0(L_DEBUG, DBG_PEERID, peerId,"Received X2 SETUP REQUEST");

   /* if a X2 Setup Request is received from a unknown peerId */
   /* Update the enbId list */
   WR_UPDATE_X2AP_ENB_ID(peerId, enbNghCb->enbId);

   /*Hash List Insertion of Neighbouring ENB CB*/
   if ( enbCbPres == FALSE )
   { 

      RLOG2(L_INFO,"Adding to neighbor list PeerId [%ld] enbId [%ld]",
            enbNghCb->peerId, enbNghCb->enbId);

      ret = cmHashListInsert(&(wrEmmCb.neighEnbLstCp), (PTR)enbNghCb,
            (U8 *)&enbNghCb->enbId,
            (U16)sizeof(enbNghCb->enbId));
      if (ROK != ret)
      {
         RLOG0(L_ERROR," Failure in the Linked List Insertion ");
         /*send the X2 Setup Failure as resource is not available*/
         wrEmmNghEnbSndX2setupFail(peerId, FALSE, WR_CZ_CAUSE_TRANSPORT,CztCauseTporttransport_resource_unavailableEnum);
         RETVALUE(RFAILED);
      }
   }

   /* Build the X2 Setup Response Message*/
   if(ROK != wrEmmNghEnbBldX2SetupRsp(&x2SetupRspPdu))
   {
      RLOG0(L_ERROR,"Failure in building the X2Setup Response");
      RETVALUE(RFAILED);
   }
   x2SetupRspPdu->peerId = peerId;

   RLOG_ARG0(L_DEBUG,DBG_PEERID, peerId,"Sending X2 SETUP RESPONSE");
   /* Trigger corresponding primitive */
   if (ROK != WrIfmX2apGpRsp(x2SetupRspPdu))
   {
      RLOG0(L_ERROR,"Failed to Send X2 Setup Rsp ");
      RETVALUE(RFAILED);
   }
   else
   {
      RLOG2(L_EVENT,"[X2AP]:Sending X2 SETUP RESPONSE [Src(%ld)-->Peer(%ld)]",
            wrEmmCb.enbId, enbNghCb->enbId);
   }
   /* LTE_ADV_ABS starts */
   WrEmmAbsEvent event;
   event.eventId = WR_EMM_ABS_X2AP_UP_IND_EVENT;
   event.u.x2apEvent.peerId = peerId;
   wrEmmAbsProcessEvent(&event);
   /* LTE_ADV_ABS ends */

   /*Update the neighbor eNodeB serving as well as neighbor cell 
     information to SON for PCI confusion and collision detection*/
   wrEmmSendSetupReqNghEnbInfoToSon(cellCb, enbNghCb, setupReq);
   RETVALUE(ROK);
}

/* LTE_ADV starts */
/** @brief This function handles the Load Inform msg from
 *         the peer eNodeB.
 *
 * @details
 *
 *     Function: wrEmmNghEnbPrcLoadInfo
 *
 *         Processing steps:
 *         - It processes the Load Inform message.
 *         - It stores all the information about the peer
 *           into the Neighbouring eNodeB CB.*
 *
 * @param[in]    peerId : Peer ID
 * @param[in]    pdu     : Czt Event Structure
 * @return S16 
 *    -# Success : ROK
 *    -# Failure   : RFAILED
 */

PRIVATE S16 wrEmmNghEnbPrcLoadInfo
(
 U32                          peerId,
 CztEvnt                      *pdu
 )
{
   TRC2(wrEmmNghEnbPrcLoadInfo)
      wrEmmLteAdvPrcLoadInfo(peerId, pdu); 
   RETVALUE(ROK);
}
/* LTE_ADV ends */

/* LTE_ADV_ABS starts */

/** @brief This function handles the Resource Status Reporting msg from
 *         the peer eNodeB.
 *
 * @details
 *
 *     Function: wrEmmNghEnbPrcRsrcStsRptng
 *
 *         Processing steps:
 - Calls lte-adv module
 *
 * @param[in]    peerId : Peer ID
 * @param[in]    pdu     : Czt Event Structure
 * @return S16 
 *    -# Success : ROK
 *    -# Failure   : RFAILED
 */

PRIVATE S16 wrEmmNghEnbPrcRsrcStsRptng
(
 U32                          peerId,
 CztEvnt                      *pdu
 )
{
   WrEmmAbsEvent             event;
   TRC2(wrEmmNghEnbPrcRsrcStsRptng)

      event.eventId           = WR_EMM_ABS_RSRC_STS_UPD_RCVD_EVENT;
   event.u.x2apEvent.peerId  = peerId;
   event.u.x2apEvent.pdu     = pdu;
   wrEmmAbsProcessEvent(&event);

   RETVALUE(ROK);
}

/** @brief This function handles the Resource Status Reporting Initiation msg from
 *         the peer eNodeB.
 *
 * @details
 *
 *     Function: wrEmmNghEnbPrcRsrcStsRptngInit
 *
 *         Processing steps:
 - Calls lte-adv module
 *
 * @param[in]    peerId : Peer ID
 * @param[in]    pdu     : Czt Event Structure
 * @return S16 
 *    -# Success : ROK
 *    -# Failure   : RFAILED
 */
PRIVATE S16 wrEmmNghEnbPrcRsrcStsRptngInit
(
 U32                          peerId,
 CztEvnt                      *pdu,
 U32                          eventId
 )
{
   WrEmmAbsEvent             event;
   TRC2(wrEmmNghEnbPrcRsrcStsRptngInit)

      event.eventId            = eventId;
   event.u.x2apEvent.peerId = peerId;
   event.u.x2apEvent.pdu    = pdu;
   wrEmmAbsProcessEvent(&event);

   RETVALUE(ROK);
}
/* LTE_ADV_ABS ends */

/** @brief This function handles the X2 Setup Response
 *         message from the peer eNodeB.
 *
 * @details
 *
 *     Function: wrEmmNghEnbPrcSetupRsp
 *
 *         Processing steps:
 *         - It processes X2AP Setup Response message
 *           and updates all the information into
 *           the Neighbouring eNode CB.
 *
 *
 * @param[in]     peerId  : Peer ID
 * @param[in]     pdu      : Czt Event Structure
 * @return S16 
 *    -# Success : ROK
 *    -# Failure   : RFAILED
 */

PRIVATE S16 wrEmmNghEnbPrcSetupRsp
(
 U32                          peerId,
 CztEvnt                      *pdu
 )
{
   CztSuccessfulOutcome      *succMsg = &pdu->pdu.val.successfulOutcome;
   CztX2SetupResp            *setupRsp = &succMsg->value.u.cztX2SetupResp;
   WrNeighEnbCb              *enbNghCb;
   U32                       eNbId=0;
   S16                       ret = ROK;
#ifdef WR_RSYS_OAM
   wrEmmNghGetEnbIdFrmSetupRsp(&eNbId, setupRsp);
#else
   WR_GET_ENBID_FROM_PEERID(eNbId,peerId);
#endif

   RLOG2(L_INFO, 
         "Received X2 Setup Response from PeerId [%ld] eNbId [%ld]",
         peerId, eNbId);

   RLOG2(L_EVENT,"[X2AP]:Received X2 SETUP RESPONSE [Peer(%ld)<--src(%ld)]",
         wrEmmCb.enbId,eNbId);
   /*Get the Neighbouring ENB Cb*/
   ret = cmHashListFind(&(wrEmmCb.neighEnbLstCp), (U8 *)&(eNbId),
         (U16)sizeof(eNbId), 0,(PTR *)&enbNghCb);
   if (ROK != ret)
   {
      RLOG0(L_ERROR," Failure in the Hash List Find ");
      RETVALUE(RFAILED);
   }
   /* Stop the X2-Setup Timer started for this peer */
   wrStopTmr((PTR)enbNghCb, WR_TMR_ENB_SETUP_RSP);
   enbNghCb->x2SetupRtxCnt = 0;

   enbNghCb->peerId        = peerId;
   enbNghCb->x2Connection = WR_NEIGH_ENB_STATE_UP;

   RLOG0(L_INFO,"[ALARM] Sending X2AP_PATH_REACHABLE Alarm to OAM" );
   wrSendLmAlarm(LCM_CATEGORY_PROTOCOL, LWR_EVENT_X2AP_PATH_REACHABLE, LCM_CAUSE_UNKNOWN);

   RLOG_ARG0(L_DEBUG, DBG_PEERID, peerId,
         "Received X2 SETUP RESPONSE");

   wrAlarmEvt(WR_ALARM_ID_X2_SETUP_FAIL, WR_ALARM_SEVERITY_CLEARED, NULLP, NULLP);

   /*Update the Neighbouring ENB CB*/
   if(ROK != wrEmmNghEnbUpdateNghEnbCb(enbNghCb,setupRsp))
   {
      RLOG0(L_ERROR,"Failure in finding Neighbouring eNB CB ");
      RETVALUE(RFAILED);
   }
   /* Assumptios: Only one cell */
   WrCellCb    *cellCb = NULLP;
   cellCb = wrEmmCb.cellCb[0];
   wrEmmSendSetupRspNghEnbInfoToSon(cellCb, enbNghCb, setupRsp);
   RETVALUE(ROK);
}
/** @brief This function handles the Status Indication
 *         API from the lower layer i.e X2AP.
 *
 * @details
 *
 *     Function: wrEmmNghEnbPrcStaInd
 *
 *         Processing steps:
 *         - If the status indication is due to SCTP ASSOC
 *           down case then start Connection timer.
 *         - If the status indication is due to WAIT timer
 *           expiry then retrasmitt the same PDU to the
 *           peer eNodeB.
 *
 *
 * @param[in]   cztSta  : Czt Status Indication
 * @return S16
 *     -# Success : ROK
 *     -# Failure   : RFAILED
 */

PUBLIC  S16 wrEmmNghEnbPrcStaInd
(
 CztStaInd          *cztSta
 )
{
   U32                       peerId;
   WrNeighEnbCb              *nghEnbCb;
   U32                       eNbId=0;
   S16                       ret = ROK;
   WrCellCb                  *cellCb = NULLP;

   /* Verify if Assoc is down;
    * */
   if(cztSta->type == CZT_STA_TYPE_ASSOC)
   {
      if(cztSta->status == CZT_STATUS_ASSOC_DOWN)
      {

         peerId = cztSta->peerId;
         WR_GET_ENBID_FROM_PEERID(eNbId,peerId);
         RLOG3(L_ERROR," Assoc. Down Reason (CZT) "
               "reason [%x] enbId [%ld] peerId [%ld]",
               cztSta->reason, eNbId, peerId);
         /*Get the Neighbouring ENB Cb*/
         ret = cmHashListFind(&(wrEmmCb.neighEnbLstCp), (U8 *)&(eNbId),
               sizeof(eNbId), 0,(PTR *)&nghEnbCb);
         if (ROK != ret)
         {
            RLOG0(L_ERROR,"Failure in the Hash List Find ");
            RETVALUE(RFAILED);
         }

         if (nghEnbCb == NULLP)
         {
            RLOG0(L_INFO,"Invalid ENB ID in Sta Ind");
            RETVALUE(ROK);
         }
         /* Stop the WR_TMR_ENB_SETUP_RSP tmr , before starting SETUP_WAIT 
            timer */
         wrStopTmr((PTR)nghEnbCb, WR_TMR_ENB_SETUP_RSP);
         nghEnbCb->x2Connection = WR_NEIGH_ENB_STATE_DOWN;
         /*Admin state is down no need to connect to ngh*/
         /* Fix for ccpu00123732 */
         if( (wrEmmCb.adminState == TRUE) && (nghEnbCb->isX2Reqd == TRUE))
         {
            /* ccpu00130201: After receiving termInd fron SCTP,
               marking X2 status as Down */
            nghEnbCb->isX2Connected = WR_NEIGH_ENB_STATE_DOWN;
            /* Start a timer for attempting to connect again */
            if (wrStartTmr((PTR)nghEnbCb, WR_TMR_ENB_SETUP_WAIT,
                     WR_ENB_CON_WAIT_TIMER) != ROK)
            {
               RLOG0(L_INFO,"Unable to start timer for  ENB ID");
            }
         }
         RLOG0(L_INFO,"[ALARM] Sending X2AP_PATH_UNREACHABLE Alarm to OAM" );
         wrSendLmAlarm(LCM_CATEGORY_PROTOCOL, LWR_EVENT_X2AP_PATH_UNREACHABLE, LCM_CAUSE_UNKNOWN);
         RLOG_ARG2(L_INFO,DBG_ENBID, nghEnbCb->enbId,
               "X2AP Association Down, IP Type = [%d], Event = [%d]",
               nghEnbCb->neighEnbAddr.type,cztSta->reason);
/* LTE_ADV_ABS starts */
           WrEmmAbsEvent event;
           event.eventId = WR_EMM_ABS_X2AP_DOWN_IND_EVENT;
           event.u.x2apEvent.peerId = peerId;
           wrEmmAbsProcessEvent(&event);
/* LTE_ADV_ABS ends */
         /* Assumptios: Only one cell */
         cellCb = wrEmmCb.cellCb[0];
         wrEmmSendNghEnbDelToSon(cellCb, nghEnbCb);
      }
   }
   else if (cztSta->status == CZT_STATUS_SETUPREQ_TW_EXPIRY)
   {
      RLOG1(L_ERROR,"SETUP TIMER EXPIRED (CZT) [%x]",cztSta->reason);
      peerId = cztSta->peerId;

      WR_GET_ENBID_FROM_PEERID(eNbId,peerId);
      /*Get the Neighbouring ENB Cb*/
      ret = cmHashListFind(&(wrEmmCb.neighEnbLstCp), (U8 *)&(eNbId),
            sizeof(eNbId), 0,(PTR *)&nghEnbCb);
      if (ROK != ret)
      {
         RLOG0(L_ERROR," Failure in the Hash List Find ");
         RETVALUE(RFAILED);
      }

      if (nghEnbCb == NULLP)
      {
         RLOG0(L_INFO,"Invalid ENB ID in Sta Ind");
         RETVALUE(ROK);
      }

      ret = wrEmmNghEnbSndX2SetupReq(nghEnbCb);
      if ( ROK != ret )
      {
         RLOG0(L_ERROR,"wrEmmNghEnbSndX2SetupReq Failed!!");
         RETVALUE(RFAILED);
      }
   }
   else if (cztSta->status == CZT_STATUS_CFGUPD_TW_EXPIRY)
   {
      RLOG1(L_ERROR,"X2-CGF-UPD TIMER EXPIRED (CZT) %x",
            cztSta->reason);
      peerId = cztSta->peerId;
      WR_GET_ENBID_FROM_PEERID(eNbId,peerId);
      /*Get the Neighbouring ENB Cb*/
      ret = cmHashListFind(&(wrEmmCb.neighEnbLstCp), (U8 *)&(eNbId),
            sizeof(eNbId), 0,(PTR *)&nghEnbCb);
      if (ROK != ret)
      {
         RLOG0(L_ERROR,"Failure in the Hash List Find ");
         RETVALUE(RFAILED);
      }
      if (nghEnbCb == NULLP)
      {
         RLOG0(L_INFO,"Invalid ENB ID in Sta Ind");
         RETVALUE(ROK);
      }
      ret = wrEmmSndEnCfgUpReq(nghEnbCb, &(nghEnbCb->cfgUpdRtxInfo));
      if (ROK != ret)
      {

         RLOG0(L_ERROR,"wrEmmSndEnCfgUpReq Failed!!!");
         RETVALUE(RFAILED);
      }
   }
   RETVALUE(ROK);
}
/** @brief This function is a wrapper to build and send X2Setup
 *         request message.
 *
 * @details
 *
 *     Function: wrEmmNghEnbSndX2Setup
 *
 *         Processing steps:
 *         - It sends the X2 Setup message if it is not connected
 *           witht the peer eNodeB.
 *         - It retrasmitts the same PDU if it is connected with the
 *           peer eNodeB.
 *
 *
 * @param[in]   nghEnbCb : Neighbour eNodeB control Block
 * @return S16 
 *     -# Success  : ROK
 *     -# Failure    : RFAILED
 */
PUBLIC S16 wrEmmNghEnbSndX2Setup
(
 WrNeighEnbCb            *nghEnbCb
 )
{
   CztEvnt                   *x2SetupReq;
   U16                       retVal = 0;

   TRC2(wrEmmNghEnbSndX2Setup);

   RLOG2(L_INFO,
         "Triggering X2 Setup Request for peerId [%ld] enbId [%ld]",
         nghEnbCb->peerId, nghEnbCb->enbId);

   /* ccpu00138578: if MME is down, X2 setup is not sent */
   if ( wrEmmCb.cellCb[0]->cellState != WR_CELL_STATE_UP ||
        wrEmmCb.numGuGrpIds <= 0)
   {
      RLOG0(L_ERROR," Cell is down, X2 Setup req is not sent ");
      if( (wrEmmCb.adminState == TRUE) && (nghEnbCb->isX2Reqd == TRUE))
      {
         /* Start a timer for attempting to connect again */
         if (wrStartTmr((PTR)nghEnbCb, WR_TMR_ENB_SETUP_WAIT,
                                       WR_ENB_CON_WAIT_TIMER) != ROK)
         {
            ALARM("Unable to start timer for  ENB ID\n");
         }
      }
      RETVALUE(ROK);
   }

   /* Build X2 Setup message */
   retVal = wrEmmNghEnbBldX2SetupReq(&x2SetupReq);
   if(ROK != retVal)
   {
      RLOG0(L_ERROR,"Failed to build X2 Setup Req ");
      RETVALUE(retVal);
   }
   x2SetupReq->peerId = nghEnbCb->peerId;

   /*Stop the  wait Timer*/
   wrStopTmr((PTR)nghEnbCb, WR_TMR_ENB_SETUP_WAIT);

   /*Start  the Timer*/
#if 0
#else
   wrStartTmr((PTR)nghEnbCb, WR_TMR_ENB_SETUP_RSP,
         wrEmmCb.tmrCfg.x2SetupTmr);
#endif

   /* Trigger corresponding primitive */
   if (ROK != WrIfmX2apGpReq(x2SetupReq))
   {
      RLOG0(L_ERROR," Failed to Send X2 Setup Req ");
      wrStopTmr((PTR)nghEnbCb, WR_TMR_ENB_SETUP_RSP);
      retVal = RFAILED;
   }
   else
   {
      RLOG2(L_EVENT,"[X2AP]:Sending X2 SETUP REQUEST [Src(%ld)-->Peer(%ld)]",
            wrEmmCb.enbId, nghEnbCb->enbId);
   }
   RETVALUE(retVal);
} /* end of wrEmmNghEnbSndX2Setup*/
/** @brief This function handles the X2 Setup Timer expiry.
 *
 * @details
 *
 *     Function: wrEmmNghEnbSetupTmrExpiry
 *
 *         Processing steps:
 *         - It retrasmitts the X2 Setup message on the
 *           expiry of the timer.
 *
 *
 * @param [in] nghEnbCb : Neighbour eNodeB Control Block
 * @return S16
 *     -# Success  : ROK
 */
PUBLIC S16 wrEmmNghEnbSetupTmrExpiry
(
 PTR                          cb
 )
{
   CztReTxInfo *cztReTxInfo;
   WrNeighEnbCb *nghEnbCb;
   S32    retVal;

   nghEnbCb = (WrNeighEnbCb *)cb;

   if ( nghEnbCb->x2SetupRtxCnt == 0xFFFF )
   {
      retVal = wrEmmNghEnbSndX2SetupReq(nghEnbCb);
      if ( retVal != ROK )
      {
         RLOG0(L_ERROR,"wrEmmNghEnbSndX2SetupReq Failed");
         RETVALUE(RFAILED);
      }
      /* Reset x2SetupRtxCnt */
      nghEnbCb->x2SetupRtxCnt = 0;
      RETVALUE(ROK);
   }

   if ( ( nghEnbCb->x2SetupRtxCnt < wrEmmCb.tmrCfg.X2SetupRetryCount )  || 
         wrEmmCb.tmrCfg.X2SetupRetryCount == 0xFF)
   {
      WR_ALLOC(&cztReTxInfo, sizeof(CztReTxInfo));
      if(NULLP == cztReTxInfo)
      {
         RLOG0(L_FATAL,"Memory allocation failed.");
         RETVALUE(RFAILED);
      }

      cztReTxInfo->peerId = ((WrNeighEnbCb *)nghEnbCb)->peerId;
      cztReTxInfo->msgType = CZT_MSG_X2_SETUP;
#if 0
#else
      wrStartTmr((PTR)nghEnbCb, WR_TMR_ENB_SETUP_RSP, wrEmmCb.tmrCfg.x2SetupTmr);
#endif

      if (WrIfmX2apGpRetxReq(cztReTxInfo) != ROK)
      {
         wrStopTmr((PTR)nghEnbCb, WR_TMR_ENB_SETUP_RSP);
         RLOG0(L_ERROR,"Unable to RETX X2 Setup Req to peer");
         RETVALUE(RFAILED);
      }
      nghEnbCb->x2SetupRtxCnt++;
   }
   else
   {
      RLOG2(L_INFO, 
            "Reached maximum retries. Stopped sending X2 Setup to "
            "PeerId [%ld] enbId [%ld]", nghEnbCb->peerId,
            nghEnbCb->enbId);
      nghEnbCb->x2SetupRtxCnt = 0;
   }
   RETVALUE(ROK);
}

/** @brief This function handles the X2 setup timer
 *         expiry.
 *
 * @details
 *
 *     Function: wrEmmNghEnbSetupWaitTmrExpiry
 *
 *         Processing steps:
 *         - It handles the X2 Setup timer expiry.
 *         - It retrasmitts the same X2 Setup message.
 *
 *
 * @param[in]  nghEnbCb  : Neighbour eNodeB Control Block
 * @return S16 
 *    -# Success : ROK
 */
PUBLIC S16 wrEmmNghEnbSetupWaitTmrExpiry
(
 PTR                          nghEnbCb
 )
{
   /* ccpu00127157 */
   WrNeighEnbCb *enbNghCb;

   enbNghCb = (WrNeighEnbCb *)nghEnbCb;

   /* If the X2 interface for this neigbhor eNB is already connected and up, 
    * then do not send another X2 Setup Request for this neighbor eNB. 
    */
   if(WR_NEIGH_ENB_STATE_UP != enbNghCb->x2Connection)
   {
      if (wrEmmNghEnbSndX2Setup(((WrNeighEnbCb *)nghEnbCb)) != ROK)
      {
         RLOG0(L_ERROR,"Unable to initiate connection to ENB");
      }
   }

   RETVALUE(ROK);
}
/** @brief This function handle X2 Setup Failure message
 *         from  the peer in response to X2 Setup request
 *         message.
 *
 * @details
 *
 *     Function: wrEmmNghEnbPrcSetupFail
 *
 *         Processing steps:
 *         - It handles the unsuccessful outcome of
 *           X2 Setup request as Failure.
 *
 *
 * @param[in]   peerId  : Peer ID
 * @param[in]   pdu      : Czt Event Structure
 * @return S16
 *    -# Success : ROK
 */

PRIVATE S16 wrEmmNghEnbPrcSetupFail
(
 U32                          peerId,
 CztEvnt                      *pdu
 )
{
   U32             eNbId=0;
   WrNeighEnbCb   *nghEnbCb;
   S16             ret = 0;
   CztTimeToWait *timeToWait = NULLP;
   TknU32       *ie = NULLP;
   CztCause     *cause = NULLP;
   U8            causeType;
   U8            causeVal = 0;

   RLOG1(L_DEBUG,"X2 Setup Fail has been received for PEERId [%lu]",peerId);
   /* Stop the X2-Setup Timer started for this peer */
   WR_GET_ENBID_FROM_PEERID(eNbId,peerId);

   RLOG2(L_EVENT,"[X2AP]:Received X2 SETUP FAILURE [Peer(%ld)<--Src(%ld)]",
         wrEmmCb.enbId,eNbId);

   RLOG2(L_INFO,"Received X2 Setup Failure from PeerId [%ld] enbId [%ld]"
         ,peerId, eNbId);

   wrUtlX2GetCztIE(&(pdu->pdu), Cztid_Cause,(void **)&ie);

   if(ie == NULLP)
   {
      RLOG0(L_ERROR,"Failed to retrive cause ");
      RETVALUE(RFAILED);
   }

   cause = (CztCause *)ie;
   causeType = cause->choice.val;
   switch(cause->choice.val)
   {
      case CAUSE_RADIONW:
         causeVal = cause->val.radioNw.val;
         break;
      case CAUSE_TRANSPORT:
         causeVal = cause->val.transport.val;
         break;
      case CAUSE_PROTOCOL:
         causeVal = cause->val.protocol.val;
         break;
      case CAUSE_MISC:
         causeVal = cause->val.misc.val;
         break;
      default:
         RLOG0(L_ERROR," Invalid case");
         break;
   }
   wrAlarmEvt(WR_ALARM_ID_X2_SETUP_FAIL, WR_ALARM_SEVERITY_MAJOR, causeType, causeVal);

   /*Get the Neighbouring ENB Cb*/
   ret = cmHashListFind(&(wrEmmCb.neighEnbLstCp), (U8 *)&(eNbId),
         sizeof(eNbId), 0,(PTR *)&nghEnbCb);
   if (ROK != ret)
   {
      RLOG0(L_ERROR, " Failure in the Hash List Find ");
      RETVALUE(RFAILED);
   }

   if (nghEnbCb == NULLP)
   {
      RLOG0(L_ERROR,"Invalid ENB ID in Sta Ind");
      RETVALUE(ROK);
   }
   wrStopTmr((PTR)nghEnbCb, WR_TMR_ENB_SETUP_RSP);
   nghEnbCb->x2SetupRtxCnt = 0;

   wrUtlX2GetCztIE(&pdu->pdu, Cztid_TimeToWait, (void **)&timeToWait);
   if ( timeToWait != NULL )
   {
      /* X2AP Layer will start timer , hence no need to start here */
      RLOG0(L_DEBUG,"Received X2-Setup Failure with TimeToWait IE");
   }
   else
   {
      RLOG0(L_DEBUG,"Received X2-Setup Failure with out TimeToWait IE");
      wrStartTmr((PTR)nghEnbCb, WR_TMR_ENB_SETUP_RSP, 
            wrEmmCb.tmrCfg.internalNoWaitRetryTmr);
      /* sett x2SetupRtxCnt = 0XFFFF to identify the Failure */    
      nghEnbCb->x2SetupRtxCnt = 0xFFFF;
   }

   RLOG_ARG0(L_DEBUG, DBG_PEERID, peerId,"Received X2 SETUP FAILURE");
   RETVALUE(ROK);
}

/** @brief This function constructs the X2AP Error
 *         Indication message.
 *
 * @details
 *
 *     Function: wrEmmNghEnbBldErrorInd
 *
 *         Processing steps:
 *         - It builds X2AP Error Indication message
 *           with all the necessary information in the
 *           message.
 *
 *
 * @param[out]   pdu  :Czt Event Structure
 * @return S16 
 *     -# Success : ROK
 *     -# Failure   : RFAILED
 */
PRIVATE S16 wrEmmNghEnbBldErrorInd
(
 CztEvnt                      **pdu,
 Bool                           causePres,
 WrUmmMsgCause                 *cause,
 UeX2apId                      *ueX2apId
 )
{
   CztEvnt                   *x2ErrorIndPdu;
   CztErrInd                 *x2ErrorInd;
   CztProtIE_Field_ErrInd_IEs *ie;
   U8                        ieIdx = 0;
   S16                       ret = ROK;
   U8                        numComp = 0;

   TRC2(wrEmmNghEnbBldErrorInd);

   RLOG0(L_DEBUG,"Building of the X2 Error Ind");

   /* Allocate and Initialize memory control point */
   ret = cmAllocEvnt(sizeof(CztEvnt), WR_CZ_MEM_SDU_SIZE,
         &wrCb.mem, (Ptr *)&x2ErrorIndPdu);
   if(ret != ROK)
   {
      RLOG0(L_FATAL,"Memory allocation failed.");
      RETVALUE(RFAILED);
   }

   /* Initialize the PDU */
   cmMemset((U8 *)&(x2ErrorIndPdu->pdu), 0, sizeof(CztX2AP_PDU));

   /* Fill the contents of Unsuccessful Event*/
   wrFillTknU8(&(x2ErrorIndPdu->pdu.choice),X2AP_PDU_INITIATINGMSG);
   wrFillTknU8(&(x2ErrorIndPdu->pdu.val.initiatingMsg.pres),PRSNT_NODEF);
   wrFillTknU32(&(x2ErrorIndPdu->pdu.val.initiatingMsg.procedureCode),
         Cztid_errorInd);
   wrFillTknU32(&(x2ErrorIndPdu->pdu.val.initiatingMsg.criticality),
         CztCriticalityignoreEnum);
   wrFillTknU8(&(x2ErrorIndPdu->pdu.val.initiatingMsg.value.u.
            cztErrInd.pres), PRSNT_NODEF);

   x2ErrorInd = &x2ErrorIndPdu->pdu.val.initiatingMsg.
      value.u.cztErrInd;

   /* Filling 1 mandatory IEs  into the pdu */
   if ( ueX2apId->oldeNBUeX2apIdPres == TRUE )
   {
      numComp++;
   }
   if ( ueX2apId->neweNBUeX2apIdPres == TRUE )
   {
      numComp++;
   }
   if ( causePres == TRUE )
   {
      numComp++;
   }
   wrFillTknU16(&(x2ErrorInd->protocolIEs.noComp), numComp);

   /* Get memory for numComp */
   if ((cmGetMem(x2ErrorIndPdu, (numComp *
                  sizeof(CztProtIE_Field_ErrInd_IEs)),
               (Ptr*)&x2ErrorInd->protocolIEs.member)) != ROK)
   {
      RLOG0(L_FATAL, "Memory allocation failed.");
      /* Free the memory of Event Structure */
      WR_FREE_EVNT(x2ErrorIndPdu);
      RETVALUE(RFAILED);
   } /* end of if statement */

   /* IE1 - Filling  Cause IE*/

   if(ueX2apId->oldeNBUeX2apIdPres == TRUE)
   {
      ie = &x2ErrorInd->protocolIEs.member[ieIdx];

      wrFillTknU8(&(ie->pres), PRSNT_NODEF);
      wrFillTknU32(&(ie->id), Cztid_Old_eNB_UE_X2AP_ID);
      wrFillTknU32(&(ie->criticality),CztCriticalityignoreEnum);
      wrFillTknU32(&(ie->value.u.cztid_Old_eNB_UE_X2AP_ID),
            ueX2apId->oldeNBUeX2apId);
      ieIdx++;
   }
   /* Fill the Old EnB UE X2AP Id */
   if(ueX2apId->neweNBUeX2apIdPres == TRUE)
   {
      ie = &x2ErrorInd->protocolIEs.member[ieIdx];

      wrFillTknU8(&(ie->pres), PRSNT_NODEF);
      wrFillTknU32(&(ie->id), Cztid_New_eNB_UE_X2AP_ID);
      wrFillTknU32(&(ie->criticality),CztCriticalityignoreEnum);
      wrFillTknU32(&(ie->value.u.cztid_New_eNB_UE_X2AP_ID),
            ueX2apId->neweNBUeX2apId);
      ieIdx++;
   }

   if ( causePres == TRUE )
   {
      ie = &x2ErrorInd->protocolIEs.member[ieIdx];
      wrFillTknU8(&(ie->pres), PRSNT_NODEF);
      wrFillTknU32(&(ie->id), Cztid_Cause);
      wrFillTknU32(&(ie->criticality), CztCriticalityignoreEnum);
      wrFillTknU8(&(ie->value.u.cztCause.choice),cause->causeTyp);

      switch(cause->causeTyp)
      {
         case CAUSE_RADIONW:
            wrFillTknU32(&(ie->value.u.cztCause.val.radioNw),
                  cause->causeVal);
            break;
         case CAUSE_TRANSPORT:
            wrFillTknU32(&(ie->value.u.cztCause.val.transport),
                  cause->causeVal);
            break;
         case CZT_CAUSE_PROTOCOL:
            wrFillTknU32(&(ie->value.u.cztCause.val.protocol),
                  cause->causeVal);
            break;
         case CZT_CAUSE_MISC:
            wrFillTknU32(&(ie->value.u.cztCause.val.misc),
                  cause->causeVal);
            break;
      }
      ieIdx++;
   }

   *pdu = x2ErrorIndPdu;
   RETVALUE(ROK);
}
/** @brief This function is a wrapper to build and
 *         deliver X2AP Error Indication message.
 *
 * @details
 *
 *     Function: wrEmmNghEnbSndErrorInd
 *
 *         Processing steps:
 *         - It builds the CZT Error Indication message.
 *         - It delivers the constructed PDU to the peer
 *           through IFM module.
 *
 *
 * @param[in]   peerId : Peer ID
 * @return S16 
 *     -# Success : ROK
 *     -# Failure   : RAFAILED
 */

PRIVATE S16 wrEmmNghEnbSndErrorInd
(
 U32                          peerId,
 Bool                         causePres,
 WrUmmMsgCause               *cause,
 UeX2apId                    *uex2apId
 )
{

   CztEvnt                   *x2ErrorIndPdu = NULLP;

   TRC2(wrEmmNghEnbSndErrorInd);

   RLOG0(L_DEBUG,"Message processing ");
   if(ROK != wrEmmNghEnbBldErrorInd(&x2ErrorIndPdu, causePres, cause, uex2apId))
   {
      RLOG0(L_ERROR,"Failure in building Error Ind");
      RETVALUE(RFAILED);
   }
   x2ErrorIndPdu->peerId = peerId;
   RLOG_ARG0(L_DEBUG, DBG_PEERID, peerId,"Sending X2 ERROR INDICATION");
   /* Trigger corresponding primitive */
   if (ROK != WrIfmX2apGpErrReq(x2ErrorIndPdu))
   {
      RLOG0(L_ERROR,"Failed to Send X2 Error Ind ");
      RETVALUE(RFAILED);
   }
   else
   {
      RLOG2(L_EVENT,"[X2AP]:Sending ERROR INDICATION [Old-eNB-UE-X2AP-ID:%d][New-eNB-UE-X2AP-ID:%d]",
           uex2apId->oldeNBUeX2apId,uex2apId->neweNBUeX2apId);
   }

   RETVALUE(ROK);
}

/** @brief This function constructs the X2AP Reset
 *         Request message.
 *
 *
 * @details
 *
 *     Function: wrEmmNghEnbBldResetReq
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
PRIVATE S16 wrEmmNghEnbBldResetReq
(
 CztEvnt                      **pdu,
 WrUmmMsgCause                  *cause
 )
{
   CztEvnt                   *x2ResetReqPdu;
   CztResetRqst              *x2ResetReq;
   CztProtIE_Field_ResetRqst_IEs *ie;
   U8                        ieIdx = 0;
   S16                       ret = ROK;
   U8                        numComp;

   TRC2(wrEmmNghEnbBldResetReq);

   RLOG0(L_INFO,"Building of  the X2 Reset Req");

   /* Allocate and Initialize memory control point */
   ret = cmAllocEvnt(sizeof(CztEvnt), WR_CZ_MEM_SDU_SIZE,
         &wrCb.mem, (Ptr *)&x2ResetReqPdu);
   if(ret != ROK)
   {
      RLOG0(L_FATAL,"Memory allocation failed.");
      RETVALUE(RFAILED);
   }
   /* Initialize the PDU */
   cmMemset((U8 *)&(x2ResetReqPdu->pdu), 0, sizeof(CztX2AP_PDU));

   /* Fill the contents of Unsuccessful Event*/
   wrFillTknU8(&(x2ResetReqPdu->pdu.choice),X2AP_PDU_INITIATINGMSG);
   wrFillTknU8(&(x2ResetReqPdu->pdu.val.initiatingMsg.pres),PRSNT_NODEF);
   wrFillTknU32(&(x2ResetReqPdu->pdu.val.initiatingMsg.procedureCode),
         Cztid_reset);
   wrFillTknU32(&(x2ResetReqPdu->pdu.val.initiatingMsg.criticality),
         CztCriticalityrejectEnum);
   wrFillTknU8(&(x2ResetReqPdu->pdu.val.initiatingMsg.value.u.
            cztResetRqst.pres), PRSNT_NODEF);

   x2ResetReq = &x2ResetReqPdu->pdu.val.initiatingMsg.
      value.u.cztResetRqst;

   /* Filling 1 mandatory IEs  into the pdu */
   numComp = 1;
   wrFillTknU16(&(x2ResetReq->protocolIEs.noComp), numComp);

   /* Get memory for numComp */
   if ((cmGetMem(x2ResetReqPdu, (numComp *
                  sizeof(CztProtIE_Field_ResetRqst_IEs)),
               (Ptr*)&x2ResetReq->protocolIEs.member)) != ROK)
   {
      RLOG0(L_FATAL, "Memory allocation failed.");
      /* Free the memory of Event Structure */
      WR_FREE_EVNT(x2ResetReqPdu);
      RETVALUE(RFAILED);
   } /* end of if statement */

   /* IE1 - Filling  Cause IE*/
   ie = &x2ResetReq->protocolIEs.member[ieIdx];
   wrFillTknU8(&(ie->pres), PRSNT_NODEF);
   wrFillTknU32(&(ie->id), Cztid_Cause);
   wrFillTknU32(&(ie->criticality), CztCriticalityignoreEnum);
   wrX2apFillCause(&ie->value.u.cztCause, cause);


   ieIdx++;

   *pdu = x2ResetReqPdu;

   RETVALUE(ROK);
}
/** @brief This function handles the expiry of
 *         reset request retrasmission timer.
 *
 * @details
 *
 *     Function: wrEmmNghEnbMsgTmrExpiry
 *
 *         Processing steps:
 *         - It builds retrasmitt API with peerId
 *           and message type.
 *         - It  call retrasmitt PDU API of X2AP
 *           layer.
 *
 * @param[in]   cb   : eNodeB Control Block
 * @return S16 
 *    -# Success : ROK
 *    -# Failure   : RFAILED
 */

PUBLIC S16 wrEmmNghEnbMsgTmrExpiry
(
 PTR                          cb
 )
{
   CztReTxInfo *cztReTxInfo;
   WrNeighEnbCb *nghEnbCb;

   nghEnbCb = (WrNeighEnbCb *)cb;

   RLOG0(L_DEBUG,"X2-Reset TimeOut");

   if ( ( nghEnbCb->rstRtxCnt < wrEmmCb.tmrCfg.X2ResetRetryCount)  || 
         wrEmmCb.tmrCfg.X2ResetRetryCount == 0xFF)
   {
      WR_ALLOC(&cztReTxInfo, sizeof(CztReTxInfo));
      if(NULLP == cztReTxInfo)
      {
         RLOG0(L_FATAL,"Memory allocation failed.");
         RETVALUE(RFAILED);
      }
      cztReTxInfo->peerId = nghEnbCb->peerId;
      cztReTxInfo->msgType = CZT_MSG_RESET;

      if (WrIfmX2apGpRetxReq(cztReTxInfo) != ROK)
      {
         RLOG0(L_ERROR,"Unable to RETX X2 Reset Req to peer");
      }
#if 0
#else
      wrStartTmr((PTR)nghEnbCb,WR_TMR_X2_RST_RSP, wrEmmCb.tmrCfg.x2ResetTmr);
#endif
      nghEnbCb->rstRtxCnt++;
   }
   else
   {
      RLOG0(L_ERROR,"Unable to RETX X2 Reset Req to peer");
      nghEnbCb->rstRtxCnt = 0;
   }

   RETVALUE(ROK);
}
/** @brief This function is wrapper to build and
 *         deliver the X2AP Reset Request PDU
 *         to the peer eNB.
 *
 * @details
 *
 *     Function: wrEmmNghEnbSndResetReq
 *
 *         Processing steps:
 *         - It constructs the X2AP Reset Request
 *           message.
 *         - It delivers the constructed PDU to the
 *           peer enodeB.
 *         - It starts the retrasmission timer.
 *
 *
 * @param[in]   peerId  : Peer ID
 * @param[in]   cause   : Cause value
 * @return S16 
 *     -# Success : ROK
 *     -# Failure  : RFAILED
 */
PUBLIC S16 wrEmmNghEnbSndResetReq
(
 U32                          peerId,
 WrUmmMsgCause                  *cause
 )
{

   CztEvnt                   *x2ResetReq = NULLP;
   U32                       eNbId;
   WrNeighEnbCb              *nghEnbCb = NULLP;
   S16                       ret = ROK;


   TRC2(wrEmmNghEnbSndResetReq);

   RLOG0(L_DEBUG,"Message processing ");


   /*Find the NeighbourCb */
   WR_GET_ENBID_FROM_PEERID(eNbId,peerId);
   ret = cmHashListFind(&(wrEmmCb.neighEnbLstCp), (U8 *)&(eNbId),
         sizeof(eNbId), 0,(PTR *)&nghEnbCb);
   if (ROK != ret)
   {
      RLOG0(L_ERROR," Failure in the Hash List Find ");
      RETVALUE(RFAILED);
   }


   if(ROK != wrEmmNghEnbBldResetReq(&x2ResetReq,cause))
   {
      RLOG0(L_ERROR,"Failure in building Reset Req");
      RETVALUE(RFAILED);
   }
   x2ResetReq->peerId = peerId;

   RLOG_ARG0(L_DEBUG, DBG_PEERID, peerId,"Sending X2AP RESET REQUEST");
   /* Trigger corresponding primitive */
   if (ROK != WrIfmX2apGpReq(x2ResetReq))
   {
      RLOG0(L_ERROR,"Failed to Send X2 Reset Req ");
      RETVALUE(RFAILED);
   }
   else
   {
      RLOG3(L_EVENT,"[X2AP]:Sending RESET REQUEST [Src(%ld)-->Peer(%ld)][CAUSE:%ld]",
            wrEmmCb.enbId, eNbId,cause->causeVal);
   }
   /*Start Timer for Retx of ResetReq Msg*/
#if 0
#else
   wrStartTmr((PTR)nghEnbCb,WR_TMR_X2_RST_RSP, wrEmmCb.tmrCfg.x2ResetTmr);
#endif

   RETVALUE(ROK);
}
/** @brief This function processes the Reset Response
 *         message received from the peer enodeB.
 *
 * @details
 *
 *     Function: wrEmmNghEnbPrcResetRsp
 *
 *         Processing steps:
 *         - It processes the Reset response message
 *           from the peer enodeB.
 *         - It stops all the ongoing HO processes
 *           with that peer.
 *         - It removes all the information about the
 *           peer from the Neighboring eNB list.
 *
 *
 * @param[in]  peerId  : Peer ID
 * @param[in]  pdu      : Czt Event Structure
 * @return S16 
 *     -# Success : ROK
 *     -# Failure   : RFAILED
 */

PRIVATE S16 wrEmmNghEnbPrcResetRsp
(
 U32                          peerId,
 CztEvnt                      *pdu
 )
{
   U32                       eNbId;
   WrNeighEnbCb              *nghEnbCb;
   S16                       ret = ROK;
   WrCellCb                  *cellCb = wrEmmCb.cellCb[0];



   WR_GET_ENBID_FROM_PEERID(eNbId,peerId);
   ret = cmHashListFind(&(wrEmmCb.neighEnbLstCp), (U8 *)&(eNbId),
         sizeof(eNbId), 0,(PTR *)&nghEnbCb);
   if (ROK != ret)
   {
      RLOG0(L_ERROR," Failure in the Hash List Find ");
      RETVALUE(RFAILED);
   }

   RLOG_ARG0(L_DEBUG,DBG_PEERID, peerId,"Received X2 RESET RESPONSE");
   RLOG2(L_EVENT,"[X2AP]:Received RESET RESPONSE [Peer(%ld)<--Src(%ld)]",
            wrEmmCb.enbId, eNbId);
   /*Stop the Reset Timer*/
   wrStopTmr((PTR)nghEnbCb,WR_TMR_X2_RST_RSP);
   nghEnbCb->rstRtxCnt = 0;

   if ((WR_CELL_STATE_RESET == cellCb->cellState) && (--cellCb->noNghEnbToReset == 0))
   {
      wrCb.resetCmfState |= WR_X2_RESET_DONE; 
   }
   if(WR_ALL_RESET_DONE == wrCb.resetCmfState)
   {
      wrCb.resetCmfState = WR_INIT_STATE_NONE;
      if(ROK != wrFillCtfCellDelCfgReq(cellCb->cellId))
      {
         RLOG_ARG0(L_ERROR,DBG_CELLID,cellCb->cellId,
               "wrFillCtfCellDelCfgReq failed. ");     
         RETVALUE(RFAILED); 
      }
   } 

   /*TODO Stopping of Ongoing HO procedures */


   RETVALUE(ROK);
}
/** @brief This function constructs the X2AP Reset response
 *         message.
 *
 * @details
 *
 *     Function: wrEmmNghEnbBldResetRsp
 *
 *         Processing steps:
 *         - It builds the CZT Reset Response message
 *           with all the information.
 *
 *
 * @param[out]  pdu : Czt Event Structure
 * @return S16 
 *     -# Success : ROK
 *     -# Faillure  : RFAILED
 */
PRIVATE S16 wrEmmNghEnbBldResetRsp
(
 CztEvnt                      **pdu
 )
{
   CztEvnt                   *x2ResetRspPdu;
   U8                        numComp = 0;
   S16                       ret = ROK;
   CztResetResp              *x2ResetRsp;

   TRC2(wrEmmNghEnbBldResetRsp);

   RLOG0(L_DEBUG,"Building of the X2 Reset Response");

   /* Allocate and Initialize memory control point */
   ret = cmAllocEvnt(sizeof(CztEvnt), WR_CZ_MEM_SDU_SIZE,
         &wrCb.mem, (Ptr *)&x2ResetRspPdu);
   if(ret != ROK)
   {
      RLOG0(L_FATAL,"Memory allocation failed.");
      RETVALUE(RFAILED);
   }
   /* Initialize the PDU */
   cmMemset((U8 *)&(x2ResetRspPdu->pdu), 0, sizeof(CztX2AP_PDU));

   /* Fill the contents of successful Event*/
   wrFillTknU8(&(x2ResetRspPdu->pdu.choice),X2AP_PDU_SUCCESSFULOUTCOME);
   wrFillTknU8(&(x2ResetRspPdu->pdu.val.successfulOutcome.pres),PRSNT_NODEF);
   wrFillTknU32(&(x2ResetRspPdu->pdu.val.successfulOutcome.procedureCode),
         Cztid_reset);
   wrFillTknU32(&(x2ResetRspPdu->pdu.val.successfulOutcome.criticality),
         CztCriticalityrejectEnum);
   wrFillTknU8(&(x2ResetRspPdu->pdu.val.successfulOutcome.value.u.
            cztResetResp.pres), PRSNT_NODEF);

   x2ResetRsp = &x2ResetRspPdu->pdu.val.successfulOutcome.
      value.u.cztResetResp;

   /* Filling 1 mandatory IEs  into the pdu */
   numComp = 0;
   wrFillTknU16(&(x2ResetRsp->protocolIEs.noComp), numComp);

   /*TODO Build Criticality Diagnostics*/

   *pdu = x2ResetRspPdu;

   RETVALUE(ROK);
}
/** @brief This function handles Reset Request message
 *         received from the peer enodeB.
 * @details
 *
 *     Function: wrEmmNghEnbPrcResetReq
 *
 *         Processing steps:
 *         - It processes the X2AP Reset request message
 *           received from the peer enodeB.
 *
 *
 * @param[in] peerId    : Peer ID
 * @param[in] pdu        : Czt Event Structure
 * @return S16
 *     -# Success : ROK
 *     -# Failure   : RFAILED
 */

PRIVATE S16 wrEmmNghEnbPrcResetReq
(
 U32                          peerId,
 CztEvnt                      *pdu
 )
{
   WrNeighEnbCb              *nghEnbCb = NULLP;
   CztEvnt                   *x2ResetRsp = NULLP;
   S16                       ret = ROK;
   U32                       eNbId;
   WrUmmMsgCause             cause;
   UeX2apId                  uex2apId;
   TknU32                    *ie = NULLP;
   CztCause                  *x2ResetCause = NULLP;
   U8                        causeType;
   U8                        causeVal = 0;

   WR_GET_ENBID_FROM_PEERID(eNbId,peerId);

   RLOG_ARG0(L_DEBUG, DBG_PEERID, peerId,"Received X2 RESET REQUEST");
   RLOG2(L_EVENT,"[X2AP]:Received RESET REQUEST [Peer(%ld)<--Src(%ld)]",
         wrEmmCb.enbId, eNbId);
   /*Get the Neighbouring ENB Cb*/
#ifdef WR_TEST_CODE
   if ( wrDntRspX2Reset == TRUE )
   {
      RETVALUE(ROK);
   } 
   if ( wrTrgX2ErrInd == TRUE )
   {
      cause.causeTyp = 1;
      cause.causeVal = 0;
      uex2apId.oldeNBUeX2apIdPres = FALSE;
      uex2apId.neweNBUeX2apIdPres = FALSE;
      uex2apId.oldeNBUeX2apId = 5;
      uex2apId.neweNBUeX2apId = 6;

      wrEmmNghEnbSndErrorInd(peerId, TRUE, &cause, &uex2apId);
      RETVALUE(ROK);
   }
#endif /* WR_TEST_CODE */
   ret = cmHashListFind(&(wrEmmCb.neighEnbLstCp), (U8 *)&(eNbId),
         sizeof(eNbId), 0,(PTR *)&nghEnbCb);
   if (ROK != ret)
   {
      RLOG0(L_ERROR,"Failure in the Hash List Find ");
      /* TODO: send appropriate cause value */
      cause.causeTyp = 1;
      cause.causeVal = 0;
      uex2apId.oldeNBUeX2apIdPres = FALSE;
      uex2apId.neweNBUeX2apIdPres = FALSE;
      wrEmmNghEnbSndErrorInd(peerId, TRUE, &cause, &uex2apId);
      RETVALUE(RFAILED);
   }
   if (nghEnbCb == NULLP)
   {
      /* TODO: send appropriate cause value */
      cause.causeTyp = 1;
      cause.causeVal = 0;
      uex2apId.oldeNBUeX2apIdPres = FALSE;
      uex2apId.neweNBUeX2apIdPres = FALSE;
      RLOG0(L_ERROR,"Neighbouring ENB is NULL ");
      wrEmmNghEnbSndErrorInd(peerId, TRUE, &cause, &uex2apId);
      RETVALUE(ROK);
   }

   wrUtlX2GetCztIE(&(pdu->pdu), Cztid_Cause, (void **)&ie);

   if(ie == NULLP)
   { 
      RLOG0(L_ERROR," Failed to retrieve cause ");
      RETVALUE(RFAILED);
   }

   if(TRUE == wrX2AlarmHandle(nghEnbCb, WR_ALARM_ID_X2_RESET))
   {
      x2ResetCause = (CztCause *)ie;
      causeType = x2ResetCause->choice.val;
      switch(x2ResetCause->choice.val)
      {
         case CAUSE_RADIONW:
            causeVal = x2ResetCause->val.radioNw.val;
            break;
         case CAUSE_TRANSPORT:
            causeVal = x2ResetCause->val.transport.val;
            break;
         case CAUSE_PROTOCOL:
            causeVal = x2ResetCause->val.protocol.val;
            break;
         case CAUSE_MISC:
            causeVal = x2ResetCause->val.misc.val;
            break;
         default:
            RLOG0(L_ERROR," Invalid case");
            break;
      }
      wrAlarmEvt(WR_ALARM_ID_X2_RESET, WR_ALARM_SEVERITY_MINOR, causeType, causeVal);
   }

   /* Handle the reset depending on the reset type */

   /*TODO cleanup of HO resources*/

   if(ROK != wrEmmNghEnbBldResetRsp(&x2ResetRsp))
   {
      RLOG0(L_ERROR,"Failure in building Reset Response");
      RETVALUE(RFAILED);
   }
   x2ResetRsp->peerId = peerId;

   RLOG_ARG0(L_DEBUG,DBG_PEERID, peerId,"Sending X2 RESET RESPONSE");
   /* Trigger corresponding primitive */
   if (ROK != WrIfmX2apGpRsp(x2ResetRsp))
   {
      RLOG0(L_ERROR,"Failed to Send X2 Reset Response ");
      RETVALUE(RFAILED);
   }
   else
   {
      RLOG2(L_EVENT,"[X2AP]:Sending RESET RESPONSE [Src(%ld)-->Peer(%ld)]",
            wrEmmCb.enbId, eNbId);
   }

   RETVALUE(ROK);
}

/** @brief This function handles Error Indication message 
 *         received from the peer enodeB.
 * @details
 *
 *     Function: wrEmmNghEnbPrcErrInd
 *
 *         Processing steps:
 *         - It processes the X2AP error Indication message
 *           received from the peer enodeB.
 *
 *
 * @param[in] peerId    : Peer ID
 * @param[in] pdu       : Czt Event Structure
 * @return S16
 *     -# Success : ROK
 *     -# Failure   : RFAILED
 */

PRIVATE S16 wrEmmNghEnbPrcErrInd
(
 U32                          peerId,
 CztEvnt                      *pdu
 )
{

   WrNeighEnbCb              *nghEnbCb;
   S16                       ret = ROK;
   U32                       eNbId;
   WrUmmMsgCause             cause;
   UeX2apId                  ueX2apId;
   TknU32                    *oldUeX2apId = NULLP;
   TknU32                    *newUeX2apId = NULLP;
   //CztCriticalityDiag        *criticalityDiag = NULLP; 
   TknU32                    *ie = NULLP;
   CztCause                  *x2ErrIndCause = NULLP;
   U8                        causeType;
   U8                        causeVal = 0;

   WR_GET_ENBID_FROM_PEERID(eNbId,peerId);
   /*Get the Neighbouring ENB Cb*/
   ret = cmHashListFind(&(wrEmmCb.neighEnbLstCp), (U8 *)&(eNbId),
         sizeof(eNbId), 0,(PTR *)&nghEnbCb);
   if (ROK != ret)
   {
      RLOG0(L_ERROR,"Failure in the Hash List Find ");
      RETVALUE(RFAILED);
   }
   if (nghEnbCb == NULLP)
   {
      RLOG0(L_ERROR,"Neighbouring ENB is NULL ");
      /* TODO: Send appropriate cause value */
      cause.causeTyp = 1;
      cause.causeVal = 0;
      ueX2apId.oldeNBUeX2apIdPres = FALSE;
      ueX2apId.neweNBUeX2apIdPres = FALSE;
      wrEmmNghEnbSndErrorInd(peerId, TRUE, &cause, &ueX2apId);
      RETVALUE(ROK);
   }

   /* Get the criticality diagnostic IE if present */
   /* ccpu00127472 start */

   if (nghEnbCb->enbCfgUpTm.tmrEvnt == WR_TMR_ENB_CONFIG_UPDATE_WAIT)
   {
      wrStopTmr((PTR)nghEnbCb, WR_TMR_ENB_CONFIG_UPDATE_WAIT); 
   }
   if (nghEnbCb->enbCfgUpTm.tmrEvnt == WR_TMR_X2_RST_RSP)
   {
      wrStopTmr((PTR)nghEnbCb,WR_TMR_X2_RST_RSP); 
   }

   /* end */  
   /* Retrive the Old and New x2ap Id'd if present */
   wrUtlX2GetCztIE(&(pdu->pdu), Cztid_Old_eNB_UE_X2AP_ID, (void **) &oldUeX2apId);
   wrUtlX2GetCztIE(&(pdu->pdu), Cztid_New_eNB_UE_X2AP_ID, (void **) &newUeX2apId);

   if ( oldUeX2apId != NULLP )
   {
      RLOG1(L_INFO,"oldUeX2apId.val  = [%ld]", oldUeX2apId->val);
      /* TODO: Take necessary action */
   }
   if ( newUeX2apId != NULLP )
   {
      RLOG1(L_INFO,"newUeX2apId.val  = [%ld]", newUeX2apId->val);
      /* TODO: Take necessary action */
   }

   RLOG0(L_DEBUG,"Received Error Indication from the peer ");

   RLOG_ARG0(L_DEBUG,DBG_PEERID, peerId,
         "Received X2 ERROR INDICATION");

   wrUtlX2GetCztIE(&(pdu->pdu), Cztid_Cause,(void **)&ie);

   if(ie == NULLP)
   { 
      RLOG0(L_ERROR," Failed to retrieve cause ");
      RETVALUE(RFAILED);
   }

   if(TRUE == wrX2AlarmHandle(nghEnbCb, WR_ALARM_ID_X2_ERROR_IND))
   {
      x2ErrIndCause = (CztCause *)ie;
      causeType = x2ErrIndCause->choice.val;
      switch(x2ErrIndCause->choice.val)
      {
         case CAUSE_RADIONW:
            causeVal = x2ErrIndCause->val.radioNw.val;
            break;
         case CAUSE_TRANSPORT:
            causeVal = x2ErrIndCause->val.transport.val;
            break;
         case CAUSE_PROTOCOL:
            causeVal = x2ErrIndCause->val.protocol.val;
            break;
         case CAUSE_MISC:
            causeVal = x2ErrIndCause->val.misc.val;
            break;
         default:
            RLOG0(L_ERROR," Invalid case");
            break;
      }
      wrAlarmEvt(WR_ALARM_ID_X2_ERROR_IND, WR_ALARM_SEVERITY_WARNING, causeType, causeVal);
   }


   /* Generate Alarm Towards LM */
   RLOG0(L_ERROR,"[ALARM] Sending ERRIND_FROM_PEER Alarm Cause UNKNOWN to OAM" );
   wrSendLmAlarm(LCM_CATEGORY_PROTOCOL, LWR_EVENT_ERRIND_FROM_PEER,
         LCM_CAUSE_UNKNOWN);
   RETVALUE(ROK);
}
/** @brief This function is for fill the modified Served Cell IE
 *
 * @details
 *
 *     Function: wrEmmCfgUpdModSrvdCell
 *
 *         Processing steps:
 *         -Finding the cell CB using the modified cell ID
 *          and fill the modified served cell IE
 *
 * @param[out] x2ConfigUpdPdu : Czt Event Structure
 * @param[in]  cellInfo             : Cell Info
 * @param[in] ieIndex               : IE Index
 * @return S16
 *      -# Success : ROK
 *      -# Failure   : RFAILED
 */

PRIVATE S16 wrEmmCfgUpdModSrvdCell
(
 CztEvnt                      *x2ConfigUpdPdu,
 WrEmmEnbCfgUpd               *cellInfo,
 U16                          ieIndex
 )
{
   U8                                        idx = 0;
   U8                                        idx1 = 0;
   U8                                        noComp = 0;
   U8                                        numSrvdCellModified = 0;
   U32                                       crntCellId = 0;
   WrCellCb                                  *crntCellCb = NULLP;
   CztProtIE_Field_ENBConfigUpd_IEs          *ie  = NULLP;
   CztSrvdCellsToMdfy_Item                   *ie1 = NULLP;
#ifdef LTE_TDD
   CztTDD_Info               *tddInfo;
   CztSpecialSubframe_Info   *specialSfInfo;
#endif
   CztProtExtnField_SrvdCell_Inform_ExtIEs   *extIe;
   U8                                        bw;
   U8                                        len = 0;  /* KW fix for LTE_ADV */ 
   /* CSG_DEV start */
   Bool                                      isCsgCfgd;
   U32                                       csgId;
   /* CSG_DEV end */

   TRC2(wrEmmCfgUpdModSrvdCell);

   RLOG0(L_DEBUG,"Building Modify Cell IE ");

   numSrvdCellModified = cellInfo->numSrvdCellModified;

   ie = &(x2ConfigUpdPdu->pdu.val.initiatingMsg.value.u.cztENBConfigUpd.
         protocolIEs.member[ieIndex]);

   wrFillTknU8(&(ie->pres), PRSNT_NODEF);
   wrFillTknU32(&(ie->id), Cztid_SrvdCellsToMdfy);
   wrFillTknU32(&(ie->criticality),CztCriticalityrejectEnum);

   wrFillTknU16(&(ie->value.u.cztSrvdCellsToMdfy.noComp),numSrvdCellModified);

   /* Allocate memory for components. */
   if ((cmGetMem(x2ConfigUpdPdu,(numSrvdCellModified * sizeof(
                     CztSrvdCellsToMdfy_Item)),
               (Ptr *)&ie->value.u.cztSrvdCellsToMdfy.member)) != ROK)
   {
      RLOG0(L_FATAL, "Memory allocation failed.");
      RETVALUE(RFAILED);
   }
   for (idx=0; idx < numSrvdCellModified; idx++)
   {
      crntCellId = cellInfo->srvdCellMod[idx].newCellId;

      WR_GET_CELLCB(crntCellCb,crntCellId);
      if (crntCellCb == NULLP )
      {
         RLOG0(L_ERROR," WR_GET_CELLCB failed");
         RETVALUE(RFAILED);
      }
      ie1 = &(ie->value.u.cztSrvdCellsToMdfy.member[idx]);

      wrFillTknU8(&(ie1->pres), PRSNT_NODEF);
      wrFillTknU8(&(ie1->servedCellInfo.pres), PRSNT_NODEF);
      /* Filling Old ECGI */
      /* Filling cell id */
      wrFillTknU8(&(ie1->old_ecgi.pres), PRSNT_NODEF);

      /* Filling the PLMN ID within cell id. */
      wrEmmCztFillPLMNId(x2ConfigUpdPdu,
            &(crntCellCb->sib1.plmns[0].plmnId),
            &(ie1->old_ecgi.pLMN_Identity));
      len = 28;
      wrFillTknBStr32(&(ie1->old_ecgi.eUTRANcellIdentifier), len,
            wrEmmCb.enbId); 

      /* Filling Served Cell Info. */
      /* 1. Filling PCI */
      wrFillTknU32(&(ie1->servedCellInfo.pCI), crntCellCb->physCellId);

      /* 2. Filling cell id */
      wrFillTknU8(&(ie1->servedCellInfo.cellId.pres), PRSNT_NODEF);

      /* 2.1 Filling the PLMN ID within cell id. */
      wrEmmCztFillPLMNId(x2ConfigUpdPdu,
            &(crntCellCb->sib1.plmns[0].plmnId),
            &(ie1->servedCellInfo.cellId.pLMN_Identity));

      /* 2.2 Filling eUTRAN cell identifier with cell id */

      len = 28;
      wrFillTknBStr32(&(ie1->servedCellInfo.cellId.eUTRANcellIdentifier),
            len,wrEmmCb.enbId);

      /* 3 : Filling TAC */
      wrEmmCztFillTAC(crntCellCb->sib1.tac,x2ConfigUpdPdu,
            &(ie1->servedCellInfo.tAC));

      /* 4 Filling broadcast PLMNS. */
      noComp = crntCellCb->sib1.numPlmnIds;

      /* Allocate memory for broadcast PLMNs */
      if ((cmGetMem(x2ConfigUpdPdu, noComp*sizeof(CztPLMN_Identity),
                  (Ptr*)&ie1->servedCellInfo.broadcastPLMNs.member)) != ROK)
      {
         RLOG0(L_FATAL,"Memory allocation failed.");
         RETVALUE(RFAILED);
      }
      wrFillTknU16(&(ie1->servedCellInfo.broadcastPLMNs.noComp),noComp);

      for(idx1 = 0; idx1 < noComp; idx1++)
      {
         /* Filling the PLMN ID of source Cell Id */
         wrEmmCztFillPLMNId(x2ConfigUpdPdu,
               &(crntCellCb->sib1.plmns[idx1].plmnId),
               &(ie1->servedCellInfo.broadcastPLMNs.member[idx1]));
      }
#ifndef LTE_TDD
      /*Filling of FDD Info*/
      wrFillTknU8(&(ie1->servedCellInfo.eUTRA_Mode_Info.choice),
            EUTRA_MODE_INFO_FDD);
      wrFillTknU8(&(ie1->servedCellInfo.eUTRA_Mode_Info.val.fDD.pres),
            PRSNT_NODEF);
      wrFillTknU32(&(ie1->servedCellInfo.eUTRA_Mode_Info.val.
               fDD.uL_EARFCN),
            crntCellCb->sib2.ulCarrierFreq);
      wrFillTknU32(&(ie1->servedCellInfo.eUTRA_Mode_Info.val.
               fDD.dL_EARFCN),
            crntCellCb->dlCarrierFreq);
      bw = crntCellCb->sib2.ulBw;
      wrFillTknU32(&(ie1->servedCellInfo.eUTRA_Mode_Info.val.
               fDD.uL_Transmission_Bandwidth),
            bw);
      bw = crntCellCb->mib.dlBw;
      wrFillTknU32(&(ie1->servedCellInfo.eUTRA_Mode_Info.val.
               fDD.dL_Transmission_Bandwidth),
            bw);
#else
      tddInfo       = &ie1->servedCellInfo.eUTRA_Mode_Info.val.tDD;
      specialSfInfo = &tddInfo->specialSubframe_Info;
      /*Filling of TDD Info*/
      wrFillTknU8(&(ie1->servedCellInfo.eUTRA_Mode_Info.choice),
            EUTRA_MODE_INFO_TDD);
      wrFillTknU8(&(tddInfo->pres),PRSNT_NODEF);
      /*For TDD both ARFCN and Bandwidth is same in DL & UL */
      wrFillTknU32(&(tddInfo->eARFCN),crntCellCb->sib2.ulCarrierFreq);
      bw = crntCellCb->sib2.ulBw;
      wrFillTknU32(&(tddInfo->transmission_Bandwidth),
            bw);
      wrFillTknU32( &(tddInfo->subframeAssignment),  crntCellCb->sib1.tddCfg.subframeCfg);
      wrFillTknU8( &(specialSfInfo->pres), PRSNT_NODEF);
      wrFillTknU32( &(specialSfInfo->specialSubframePatterns), crntCellCb->sib1.tddCfg.specialSubframeCfg);
      wrFillTknU32( &(specialSfInfo->cyclicPrefixDL), crntCellCb->dlCyclicprefix);
      wrFillTknU32( &(specialSfInfo->cyclicPrefixUL), crntCellCb->sib2.ulCyclicPrefix);
#endif

      /*  Filling PRACH Information and Antenna Ports and CSG ID */
      /* CSG_DEV start */
      isCsgCfgd = wrEmmIsCsgCell(crntCellCb->cellId);
      if(TRUE == isCsgCfgd)
      {
         noComp = WR_EMM_X2_CONFIG_EXTEN_IE_COUNT + 1;
      }
      else
      {
         noComp = WR_EMM_X2_CONFIG_EXTEN_IE_COUNT;
      }
      /* CSG_DEV end */
      /* Allocate memory for  PRACH Information and Antenna Ports*/
      if ((cmGetMem(x2ConfigUpdPdu, noComp *
                  sizeof(CztProtExtnField_SrvdCell_Inform_ExtIEs),
                  (Ptr*)&ie1->servedCellInfo.iE_Extns.member)) !=ROK)
      {
         RLOG0(L_FATAL,"Memory allocation failed.");
         WR_FREE_EVNT(x2ConfigUpdPdu);
         RETVALUE(RFAILED);
      }
      wrFillTknU16(&(ie1->servedCellInfo.iE_Extns.noComp),noComp);

      extIe = &ie1->servedCellInfo.iE_Extns.member[0];
      wrFillTknU8(&(extIe->pres), PRSNT_NODEF);
      wrFillTknU32(&(extIe->id), Cztid_Number_of_Antennaports);
      /* ccpu00134596: Correct the criticality filled */
      wrFillTknU32(&(extIe->criticality),CztCriticalityignoreEnum);
      wrFillTknU32(&(extIe->extensionValue.u.cztNumber_of_Antennaports),
            crntCellCb->numAntPorts);

      extIe = &ie1->servedCellInfo.iE_Extns.member[1];
      wrFillTknU8(&(extIe->pres), PRSNT_NODEF);
      wrFillTknU32(&(extIe->id), Cztid_PRACH_Config);
      /* ccpu00134596: Correct the criticality filled */
      wrFillTknU32(&(extIe->criticality),CztCriticalityignoreEnum);

      wrFillTknU8(&(extIe->extensionValue.u.cztPRACH_Config.pres), PRSNT_NODEF);
      wrFillTknU32(&(extIe->extensionValue.u.cztPRACH_Config.rootSequenceIdx),
            crntCellCb->sib2.prachCfg.rootSeqIdx);
      wrFillTknU32(&(extIe->extensionValue.u.cztPRACH_Config.zeroCorrelationIdx),
            crntCellCb->sib2.prachCfg.zeroCorrZone);
      wrFillTknU8(&(extIe->extensionValue.u.cztPRACH_Config.highSpeedFlag),
            crntCellCb->sib2.prachCfg.highSpeedFlag);
      wrFillTknU32(&(extIe->extensionValue.u.cztPRACH_Config.prach_FreqOffset),
            crntCellCb->sib2.prachCfg.prachFreqOffset);
      wrFillTknU32(&(extIe->extensionValue.u.cztPRACH_Config.prach_ConfigIdx),
            crntCellCb->sib2.prachCfg.prachCfgIdx);

      extIe->extensionValue.u.cztPRACH_Config.iE_Extns.noComp.pres =  NOTPRSNT;

      /* CSG_DEV start */
      if(TRUE == isCsgCfgd)
      {
         extIe = &ie1->servedCellInfo.iE_Extns.member[2];
         wrFillTknU8(&(extIe->pres), PRSNT_NODEF);
         wrFillTknU32(&(extIe->id), Cztid_CSG_Id);
         wrFillTknU32(&(extIe->criticality), CztCriticalityignoreEnum);
         csgId = wrEmmGetCsgIdFrmCellId(crntCellCb->cellId);
         wrFillTknBStr32(&extIe->extensionValue.u.cztCSG_Id, WR_S1AP_CSG_ID_LEN,
               csgId);
      }
      /* CSG_DEV end */

      /* 6: Filling Neighbour info. */
      if(RFAILED == wrEmmFillNghCellInfoforX2Msg(x2ConfigUpdPdu,
               crntCellCb, 
               &ie1->neighbour_Info))
      {
         WR_FREE_EVNT(x2ConfigUpdPdu);
         RETVALUE(RFAILED);
      }
   }/* end of for loop */
   RETVALUE(ROK);
} /* end of wrEmmCfgUpdModSrvdCell */
/** @brief This function is for filling the Added served cell IE
 *
 * @details
 *
 *     Function: wrEmmCfgUpdAddSrvdCell
 *
 *         Processing steps:
 *         -Finding the cell CB using the added cell
 *          and filling the added served cell IE info
 *
 * @param[out] x2ConfigUpdPdu : Czt Event Structure
 * @param[in] cellInfo                : Cell Info
 * @param [in] ieIndex               : IE index
 * @return S16
 *     -# Success : ROK
 *     -# Failure   : RFAILED
 */
PRIVATE S16 wrEmmCfgUpdAddSrvdCell
(
 CztEvnt                     *x2ConfigUpdPdu,
 WrEmmEnbCfgUpd              *cellInfo,
 U16                         ieIndex
 )
{
   U8                                  idx = 0;
   U8                                  idx1 = 0;
   U8                                  choice = 0;
   U8                                  noComp = 0;
   U8                                  numSrvdCellAdded = 0;
   U32                                 crntCellId = 0;
   WrCellCb                            *crntCellCb = NULLP;
   CztProtIE_Field_ENBConfigUpd_IEs    *ie  = NULLP;
   CztSrvdCellsMember                  *ie1 = NULLP;
   Czt_ExtnSrvdCell_Inform_ExtIEsCls   *extIE = NULLP;
   U8                                  bw = 0; /*KW fix for LTE_ADV */
   /* CSG_DEV start */
   Bool                                isCsgCfgd;
   U32                                 csgId;
   /* CSG_DEV end */

   TRC2(wrEmmCfgUpdAddSrvdCell);

   RLOG0(L_DEBUG,"Building Serverd Cell IE ");

   numSrvdCellAdded = cellInfo->numSrvdCellAdded;

   ie = &(x2ConfigUpdPdu->pdu.val.initiatingMsg.value.u.cztENBConfigUpd.
         protocolIEs.member[ieIndex]);
   wrFillTknU8(&(ie->pres), PRSNT_NODEF);
   wrFillTknU32(&(ie->id), Cztid_SrvdCellsToAdd);

   wrFillTknU16(&(ie->value.u.cztSrvdCells.noComp), numSrvdCellAdded);

   /* Allocate memory for components. */
   if ((cmGetMem(x2ConfigUpdPdu,(numSrvdCellAdded * sizeof(CztSrvdCellsMember)),
               (Ptr *)&ie->value.u.cztSrvdCells.member)) != ROK)
   {
      RLOG0(L_FATAL,"Memory allocation failed.");
      RETVALUE(RFAILED);
   }

   for (idx = 0; idx < numSrvdCellAdded; idx++)
   {
      crntCellId = cellInfo->srvdCellAdd[idx];
      WR_GET_CELLCB(crntCellCb,crntCellId);
      if (crntCellCb == NULLP)
      {
         RLOG0(L_ERROR," WR_GET_CELLCB failed");
         RETVALUE(RFAILED);
      }
      ie1 = &(ie->value.u.cztSrvdCells.member[idx]);

      wrFillTknU8(&(ie1->pres), PRSNT_NODEF);
      wrFillTknU8(&(ie1->servedCellInfo.pres), PRSNT_NODEF);

      /* 1. Filling PCI */
      wrFillTknU32(&(ie1->servedCellInfo.pCI), crntCellCb->physCellId);

      /* 2. Filling cell id */
      wrFillTknU8(&(ie1->servedCellInfo.cellId.pres), PRSNT_NODEF);

      /* 2.1 Filling the PLMN ID within cell id. */
      wrEmmCztFillPLMNId(x2ConfigUpdPdu, &(crntCellCb->sib1.plmns[0].plmnId),
            &(ie1->servedCellInfo.cellId.pLMN_Identity));

      /* 2.2 Filling eUTRAN cell identifier with cell id */
      if(ENB_ID_MACRO_ENB_ID == wrEmmCb.enbType)
      {
         wrFillTknBStr32(&(ie1->servedCellInfo.cellId.eUTRANcellIdentifier),
               WR_EMM_X2_MACRO_ENBID_LEN,crntCellId);
      }
      else
      {
         wrFillTknBStr32(&(ie1->servedCellInfo.cellId.eUTRANcellIdentifier),
               WR_EMM_X2_HOME_ENBID_LEN,crntCellId);
      }

      /* 3 :Filling TAC */
      wrEmmCztFillTAC(crntCellCb->sib1.tac,x2ConfigUpdPdu,
            &(ie1->servedCellInfo.tAC));

      /* 4 Filling broadcast PLMNS. */
      noComp = crntCellCb->sib1.numPlmnIds;

      /* Allocate memory for broadcast PLMNs */
      if ((cmGetMem(x2ConfigUpdPdu, noComp*sizeof(CztPLMN_Identity),
                  (Ptr*)&ie1->servedCellInfo.broadcastPLMNs.member)) != ROK)
      {
         RLOG0(L_FATAL,"Memory allocation failed.");
         RETVALUE(RFAILED);
      }
      wrFillTknU16(&(ie1->servedCellInfo.broadcastPLMNs.noComp),noComp);

      for(idx1 = 0; idx1 < noComp; idx1++)
      {
         /* Filling the PLMN ID of source Cell Id */
         wrEmmCztFillPLMNId(x2ConfigUpdPdu,
               &(crntCellCb->sib1.plmns[idx1].plmnId),
               &(ie1->servedCellInfo.broadcastPLMNs.member[idx1]));
      }

      /* 5 Filling the eUTRA mode info. */
      choice = crntCellCb->duplexMode;

      switch (choice)
      {
         case EUTRA_MODE_INFO_FDD:
            {
               wrFillTknU8(&(ie1->servedCellInfo.eUTRA_Mode_Info.choice),choice);
               wrFillTknU8(&(ie1->servedCellInfo.eUTRA_Mode_Info.val.fDD.pres),
                     PRSNT_NODEF);
               wrFillTknU32(&(ie1->servedCellInfo.eUTRA_Mode_Info.val.fDD.uL_EARFCN),
                     crntCellCb->sib2.ulCarrierFreq);
               wrFillTknU32(&(ie1->servedCellInfo.eUTRA_Mode_Info.val.fDD.dL_EARFCN),
                     crntCellCb->dlCarrierFreq);
               bw = wrFillBwCfg(crntCellCb->sib2.ulBw);
               wrFillTknU32(
                     &(ie1->servedCellInfo.eUTRA_Mode_Info.val.fDD.uL_Transmission_Bandwidth),
                     bw);
               bw = wrFillBwCfg(crntCellCb->mib.dlBw);
               wrFillTknU32(&(ie1->servedCellInfo.eUTRA_Mode_Info.val.fDD.
                        dL_Transmission_Bandwidth),
                     bw);
               break;
            }
         default:
            {
               RLOG0(L_ERROR," Invalid Mode.");
            }
      } /* end of switch statement */

      /* CSG_DEV start */
      isCsgCfgd = wrEmmIsCsgCell(crntCellCb->cellId);
      if(TRUE == isCsgCfgd)
      {
         noComp = WR_EMM_X2_CONFIG_EXTEN_IE_COUNT + 1;
      }
      else
      {
         noComp = WR_EMM_X2_CONFIG_EXTEN_IE_COUNT;
      }
      /* CSG_DEV end */

      /*Filling Number of Antenna Ports and  PRACH Configuration IE*/
      wrFillTknU16(&(ie1->servedCellInfo.iE_Extns.noComp),noComp);

      /*Allocating memory*/
      if ((cmGetMem(x2ConfigUpdPdu, noComp*
                  sizeof(CztProtExtnField_SrvdCell_Inform_ExtIEs),
                  (Ptr*)&ie1->servedCellInfo.iE_Extns.member)) != ROK)
      {
         RLOG0(L_FATAL,"Memory allocation failed.");
         RETVALUE(RFAILED);
      }
      /*Filling no of Antenna Ports IE*/
      wrFillTknU8(&(ie1->servedCellInfo.iE_Extns.member[0].pres),PRSNT_NODEF);
      wrFillTknU32(&(ie1->servedCellInfo.iE_Extns.member[0].id),
            Cztid_Number_of_Antennaports);
      wrFillTknU32(&(ie1->servedCellInfo.iE_Extns.member[0].criticality),
            CztCriticalityignoreEnum);
      extIE =&(ie1->servedCellInfo.iE_Extns.member[0].extensionValue);

      wrFillTknU32(&(extIE->u.cztNumber_of_Antennaports),
            crntCellCb->numAntPorts);

      /*Filling PRACH Configuration IE*/

      wrFillTknU8(&(ie1->servedCellInfo.iE_Extns.member[1].pres),PRSNT_NODEF);
      wrFillTknU32(&(ie1->servedCellInfo.iE_Extns.member[1].id),
            Cztid_PRACH_Config);
      wrFillTknU32(&(ie1->servedCellInfo.iE_Extns.member[1].criticality),
            CztCriticalityignoreEnum);
      extIE =&(ie1->servedCellInfo.iE_Extns.member[1].extensionValue);

      wrFillTknU32(&(extIE->u.cztPRACH_Config.rootSequenceIdx),
            crntCellCb->sib2.prachCfg.rootSeqIdx);
      wrFillTknU8(&(extIE->u.cztPRACH_Config.highSpeedFlag),
            crntCellCb->sib2.prachCfg.highSpeedFlag);
      wrFillTknU32(&(extIE->u.cztPRACH_Config.prach_FreqOffset),
            crntCellCb->sib2.prachCfg.prachFreqOffset);
      wrFillTknU32(&(extIE->u.cztPRACH_Config.zeroCorrelationIdx),
            crntCellCb->sib2.prachCfg.zeroCorrZone);
      wrFillTknU32(&(extIE->u.cztPRACH_Config.prach_ConfigIdx),
            crntCellCb->sib2.prachCfg.prachCfgIdx);

      /* CSG_DEV start */
      extIE->u.cztPRACH_Config.iE_Extns.noComp.pres =  NOTPRSNT;

      if(TRUE == isCsgCfgd)
      {
         wrFillTknU8(&(ie1->servedCellInfo.iE_Extns.member[1].pres),
               PRSNT_NODEF);
         wrFillTknU32(&(ie1->servedCellInfo.iE_Extns.member[1].id),
               Cztid_CSG_Id);
         wrFillTknU32(&(ie1->servedCellInfo.iE_Extns.member[1].criticality),
               CztCriticalityignoreEnum);

         extIE = &(ie1->servedCellInfo.iE_Extns.member[2].extensionValue);
         csgId = wrEmmGetCsgIdFrmCellId(crntCellCb->cellId);
         wrFillTknBStr32(&extIE->u.cztCSG_Id, WR_S1AP_CSG_ID_LEN, csgId);
      }
      /* CSG_DEV end */

      /* 6: Filling Neighbour info. */
      if(RFAILED == wrEmmFillNghCellInfoforX2Msg(x2ConfigUpdPdu,
               crntCellCb, 
               &ie1->neighbour_Info))
      {
         WR_FREE_EVNT(x2ConfigUpdPdu);
         RETVALUE(RFAILED);
         RLOG0(L_FATAL, "Memory allocation failed.");
      }
   }/* end of for loop */
   RETVALUE(ROK);
} /* end of wrEmmCfgUpdAddSrvdCell */
/** @brief This function is for filling the deleted served cell IE
 *
 * @details
 *
 *     Function: wrEmmCfgUpdDelSrvdCell
 *
 *         Processing steps:
 *         -Finding the Cell CB using Deleted cell ID
 *          and filling the deleted served cell IE info
 *
 * @param[in] cellInfo                : Cell info
 * @param[out] x2ConfigUpdPdu : Czt Event Structure
 * @param[in] ieIndex                : IE Index
 * @return S16 
 *    -# Success : ROK
 *    -# Failure   : RFAILED
 */
PRIVATE S16 wrEmmCfgUpdDelSrvdCell
(
 WrEmmEnbCfgUpd               *cellInfo,
 CztEvnt                      *x2ConfigUpdPdu,
 U16                          ieIndex
 )
{
   U8                        numSrvdCellDeleted = 0;
   U16                       idx = 0;
   U32                       crntCellId = 0;
   WrCellCb                  *crntCellCb = NULLP;
   CztProtIE_Field_ENBConfigUpd_IEs *ie  = NULLP;
   CztECGI                   *ie1 = NULLP;

   TRC2(wrEmmCfgUpdDelSrvdCell);

   RLOG0(L_DEBUG,"Building Deleted Cell IE ");

   numSrvdCellDeleted = cellInfo->numSrvdCellDeleted;

   ie = &x2ConfigUpdPdu->pdu.val.initiatingMsg.value.u.cztENBConfigUpd.
      protocolIEs.member[ieIndex];

   wrFillTknU8(&(ie->pres), PRSNT_NODEF);
   wrFillTknU32(&(ie->id), Cztid_SrvdCellsToDelete);
   wrFillTknU32(&(ie->criticality),CztCriticalityrejectEnum);

   wrFillTknU16(&(ie->value.u.cztOld_ECGIs.noComp), numSrvdCellDeleted);

   /* Allocate memory for components. */
   if ((cmGetMem(x2ConfigUpdPdu,(numSrvdCellDeleted * sizeof(CztECGI)),
               (Ptr *)&ie->value.u.cztOld_ECGIs.member)) !=ROK)

   {
      RLOG0(L_FATAL, "Memory allocation failed.");
      RETVALUE(RFAILED);
   }

   for (idx = 0; idx < numSrvdCellDeleted; idx++)
   {
      crntCellId = cellInfo->oldEcgiToDel[idx];
      WR_GET_CELLCB(crntCellCb,crntCellId);
      if( crntCellCb == NULLP)
      {
         RLOG0(L_ERROR," WR_GET_CELLCB failed");
         RETVALUE(RFAILED);
      }
      ie1 = &ie->value.u.cztOld_ECGIs.member[idx];

      /* Filling Old ECGI */
      /* Filling cell id */
      wrFillTknU8(&(ie1->pres), PRSNT_NODEF);

      /* Fill the PLMN ID within cell id. */
      wrEmmCztFillPLMNId(x2ConfigUpdPdu,
            &(crntCellCb->sib1.plmns[0].plmnId),&(ie1->pLMN_Identity));

      /* Filling eUTRAN cell identifier with cell id */
      if(ENB_ID_MACRO_ENB_ID == wrEmmCb.enbType)
      {
         wrFillTknBStr32(&(ie1->eUTRANcellIdentifier), 20,crntCellId );
      }
      else
      {
         wrFillTknBStr32(&(ie1->eUTRANcellIdentifier), 28,crntCellId );
      }

   }/* end of for loop */

   RETVALUE(ROK);
} /* end of wrEmmCfgUpdDelSrvdCell */
/** @brief This function fill for filling Added and deleted GU GroupID IE
 *
 * @details
 *
 *     Function: wrEmmCfgUpdAddDelGUGrpIDLst
 *
 *         Processing steps:
 *         -Finding the Cell CB and filling the Added and Deleted
 *          GU Group ID IE
 *
 * @param[out] x2ConfigUpdPdu : Czt event strcuture
 * @param[in] cellInfo : Cell Info
 * @param[in] ieIndex : IE Index
 * @param[in] opFlag   : Operation Flag
 * @return S16
 *      -# Success : ROK
 *      -# Failure   : RFAILED
 */

PRIVATE S16 wrEmmCfgUpdAddDelGUGrpIDLst
(
 CztEvnt                      *x2ConfigUpdPdu,
 WrEmmEnbCfgUpd               *cellInfo,
 U8                           ieIndex,
 Bool                         opFlag
 )
{
   U8                        idx = 0;
   U8                        numGrpIDAdded = 0;
   CztProtIE_Field_ENBConfigUpd_IEs *ie  = NULLP;
   CztGU_Group_ID            *ie1 = NULLP;
   WrGuGrpIdLst*             guGrupId;
   U16                       mmeGrpId;

   TRC2(wrEmmCfgUpdAddDelGUGrpIDLst);

   RLOG0(L_DEBUG,"Building Add and Del GU ID IE ");


   ie = &(x2ConfigUpdPdu->pdu.val.initiatingMsg.value.u.cztENBConfigUpd.
         protocolIEs.member[ieIndex]);
   wrFillTknU8(&(ie->pres), PRSNT_NODEF);

   if(opFlag == FALSE)
   {
      numGrpIDAdded = cellInfo->numGrpIdAdded;
      wrFillTknU32(&(ie->id), Cztid_GUGroupIDToAddLst);
   }
   else
   {
      numGrpIDAdded = cellInfo->numGrpIdDeleted;
      wrFillTknU32(&(ie->id), Cztid_GUGroupIDToDeleteLst);
   }

   wrFillTknU32(&(ie->criticality),CztCriticalityrejectEnum);

   wrFillTknU16(&(ie->value.u.cztid_GUGroupIDToAddLst.noComp), numGrpIDAdded);

   /* Allocate memory for components. */
   if ((cmGetMem(x2ConfigUpdPdu,(numGrpIDAdded * sizeof(CztGU_Group_ID)),

               (Ptr *)&ie->value.u.cztid_GUGroupIDToAddLst.member)) != ROK)
   {
      RLOG0(L_FATAL," Memory allocation failed.");
      RETVALUE(RFAILED);
   }

   for (idx =0; idx < numGrpIDAdded; idx++)
   {
      if( opFlag == FALSE)
      {
         guGrupId =  &cellInfo->guIdAddLst[idx];
      }
      else
      {
         guGrupId =  &cellInfo->guIdDelLst[idx];
      }
      ie1 = &(ie->value.u.cztid_GUGroupIDToAddLst.member[idx]);

      wrFillTknU8(&(ie1->pres), PRSNT_NODEF);
      /* Filling the PLMN ID within GU Group Id  */
      /* TODO Shashank later */

      wrEmmCztFillPLMNId(x2ConfigUpdPdu,
            &(guGrupId->plmnId),&(ie1->pLMN_Identity));

      mmeGrpId = CM_INET_HTON_U16(guGrupId->mmeGrpId);
   
      /* Filling the MME id */
      wrFill2TknStr4(&(ie1->mME_Group_ID),2,(U8 *)&mmeGrpId);
   }
   /*MME_LDBAL*/
   RETVALUE(ROK);
}
/** @brief This function is for building the eNodeB Configuration
 *         Request Message
 *
 * @details
 *
 *     Function: wrEmmBldEBCfgUptReq
 *
 *         Processing steps:
 *         -Couting the number IE is to be filled
 *         -if added served cell ID is present then
 *          call the function to fill the added served cell IE
 *         -if modified served cell ID is present then
 *           call the function to fill the modified served cell IE
 *         -if deleted served cell id present then
 *           call the function to file the deleted served cell IE
 *         -if added or deleted GU group id persent then
 *           call the function to fill the added and delete GU group
 *           ID IE
 *
 * @param[out] pdu    : Czt Event Strcuture
 * @param[in] cellInfo : Cell Info
 * @return S16
 *      -# Success  : ROK
 *      -# Failure    : RFAILED
 */
PUBLIC S16 wrEmmBldEBCfgUptReq
(
 CztEvnt                      **pdu,
 WrEmmEnbCfgUpd               *cellInfo
 )
{
   CztEvnt                   *x2ConfigUpdPdu = NULLP;
   U8                        ieIndex =0;
   S16                       ret = ROK;
   CztENBConfigUpd           *enCfgUpReq=NULLP;
   U8                        numComp = 0;

   TRC2(wrEmmBldEBCfgUptReq);

   RLOG0(L_DEBUG,
         "Building of  the eNodeB config update Request ");

   /* Allocate and Initialize memory control point */
   ret = cmAllocEvnt(sizeof(CztEvnt), WR_CZ_MEM_SDU_SIZE,
         &wrCb.mem, (Ptr *)&x2ConfigUpdPdu);
   if(ret != ROK)
   {
      RLOG0(L_FATAL,"Memory allocation failed.");
      RETVALUE(RFAILED);
   }

   /* Initialize the PDU */
   cmMemset((U8 *)&(x2ConfigUpdPdu->pdu), 0, sizeof(CztX2AP_PDU));

   /* Filling the contents of initiatingMsg */
   wrFillTknU8(&(x2ConfigUpdPdu->pdu.choice), X2AP_PDU_INITIATINGMSG);
   wrFillTknU8(&(x2ConfigUpdPdu->pdu.val.initiatingMsg.pres),PRSNT_NODEF);
   wrFillTknU32(&(x2ConfigUpdPdu->pdu.val.initiatingMsg.procedureCode),
         Cztid_eNBConfigUpd);
   wrFillTknU32(&(x2ConfigUpdPdu->pdu.val.initiatingMsg.criticality),
         CztCriticalityrejectEnum);
   wrFillTknU8(
         &(x2ConfigUpdPdu->pdu.val.initiatingMsg.value.u.cztENBConfigUpd.pres),
         PRSNT_NODEF);

   enCfgUpReq =
      &(x2ConfigUpdPdu->pdu.val.initiatingMsg.value.u.cztENBConfigUpd);

   /*Counting the number IE's to be encoded in eNode Confi update message*/
   if (cellInfo->numSrvdCellAdded != 0 )
   {
      numComp++;
   }

   if (cellInfo->numSrvdCellModified != 0)
   {
      numComp++;
   }

   if (cellInfo->numSrvdCellDeleted != 0 )
   {
      numComp++;
   }

   if (cellInfo->numGrpIdAdded != 0)
   {
      numComp++;
   }

   if (cellInfo->numGrpIdDeleted != 0)
   {
      numComp++;
   }

   /*Setting the number IE count */
   wrFillTknU16(&(enCfgUpReq->protocolIEs.noComp), numComp);

   /* Get memory for numComp */
   if ((cmGetMem(x2ConfigUpdPdu,
               (numComp * sizeof(CztProtIE_Field_ENBConfigUpd_IEs)),
               (Ptr*)&enCfgUpReq->protocolIEs.member)) !=ROK)
   {
      RLOG0(L_FATAL,"Memory allocation failed.");
      /* Free the memory of Event Structure */
      WR_FREE_EVNT(x2ConfigUpdPdu);
      RETVALUE(RFAILED);
   } /* end of if statement */

   /*If any new cells added ,fill the  servedCellAdded IE*/
   if (cellInfo->numSrvdCellAdded != 0 )
   {
      ret = wrEmmCfgUpdAddSrvdCell(x2ConfigUpdPdu,cellInfo,ieIndex);
      if (ret != ROK)
      {
         RLOG0(L_ERROR,"wrEmmCfgUpdAddSrvdCell failed ");
         WR_FREE_EVNT(x2ConfigUpdPdu);
         RETVALUE(ret);
      }
      ieIndex++;
   }
   /*If any existing cells to be modified ,fill the srvdCellMod IE*/
   if (cellInfo->numSrvdCellModified != 0)
   {

      /* Fill the IE2 : Served Cells to Modify */
      /* IE2 consists of old ecgi, Served Cell Info and Neighbour Info. */
      ret = wrEmmCfgUpdModSrvdCell(x2ConfigUpdPdu,cellInfo,ieIndex);
      if (ret != ROK)
      {
         RLOG0(L_ERROR,"wrEmmCfgUpdModSrvdCell failed ");
         WR_FREE_EVNT(x2ConfigUpdPdu);
         RETVALUE(ret);
      }
      ieIndex++;
   }
   /*If any existing cell to be deleted , filling the  srvdCellDel IE*/
   if (cellInfo->numSrvdCellDeleted != 0 )
   {

      /* Fill the IE3 : Served Cells to Delete */
      /* IE3 consists of old ecgi */
      ret = wrEmmCfgUpdDelSrvdCell(cellInfo,x2ConfigUpdPdu,ieIndex);
      {
         RLOG0(L_ERROR,"wrEmmCfgUpdDelSrvdCell failed ");
         WR_FREE_EVNT(x2ConfigUpdPdu);
         RETVALUE(ret);
      }
      ieIndex++;
   }
   if (cellInfo->numGrpIdAdded != 0)
   {
      /* Fill teh IE4 : GU Group Id To Add List */
      /* IE4 consists of GU Group ID IE. which in turns consists of:
         a. PLMN Id
         b. MME group id */
      ret = wrEmmCfgUpdAddDelGUGrpIDLst(x2ConfigUpdPdu,cellInfo,ieIndex,FALSE);
      if( ret != ROK)
      {
         RLOG0(L_ERROR,"wrEmmCfgUpdAddGUGrpIDLst failed ");
         WR_FREE_EVNT(x2ConfigUpdPdu);
         RETVALUE(ret);
      }
      ieIndex++;
   }

   if (cellInfo->numGrpIdDeleted != 0)
   {
      /* Fill teh IE5 : GU Group Id To Add List */
      /* IE4 consists of GU Group ID IE. which in turns consists of:
         a. PLMN Id
         b. MME group id */
      ret = wrEmmCfgUpdAddDelGUGrpIDLst(x2ConfigUpdPdu,cellInfo,ieIndex,TRUE);
      if( ret != ROK)
      {
         RLOG0(L_ERROR,"wrEmmCfgUpdAddGUGrpIDLst failed ");
         WR_FREE_EVNT(x2ConfigUpdPdu);
         RETVALUE(ret);
      }
   }
   *pdu = x2ConfigUpdPdu;
   RETVALUE(ROK);
}
/** @brief This function is for sending the eNodeB Configuration
 *         Update Request message to X2AP layer
 * @details
 *
 *     Function: wrEmmSndEnCfgUpReq
 *
 *         Processing steps:
 *         -Bulding the enNodeConfiguration update message
 *         -Sending the message to x2AP layer
 *         -Starting the re-transmission timer
 *
 * @param [in] nghEnbCb : Neighbour eNodeB Control Block
 * @param [in] cellInfo     : Cell Info
 * @return S16 
 *    -# Success : ROK
 *    -# Failure   : RFAILED
 */
PUBLIC S16 wrEmmSndEnCfgUpReq
(
 WrNeighEnbCb                 *nghEnbCb,
 WrEmmEnbCfgUpd               *cellInfo
 )
{
   CztEvnt                   *cfgUpReq=NULLP;
   S16                       retVal = 0;
   U32                       enbId = 0;

   TRC2(wrEmmSndEnCfgUpReq);

   WR_GET_ENBID_FROM_PEERID(enbId,nghEnbCb->peerId);
   RLOG1(L_DEBUG,"Triggering eNode Config Update Request "
         "to peerId [%ld]", nghEnbCb->peerId);

   /* Storing the WrEmmEnbCfgUpd, to re-transmit the Cfg-Upd in case of Time to Wait */
   cmMemcpy((U8 *)&(nghEnbCb->cfgUpdRtxInfo),
         (U8 *)(cellInfo), sizeof(WrEmmEnbCfgUpd));
   /* Build  eNode Config update Request message */
   retVal = wrEmmBldEBCfgUptReq(&cfgUpReq,cellInfo);
   if (ROK != retVal)
   {
      RLOG0(L_ERROR,"eNode Config Update Request ");
      RETVALUE(RFAILED);
   }

   cfgUpReq->peerId = nghEnbCb->peerId;
   /*before sending eNodeB config update request check whether any enodeB config update
     already sent to MME and waiting for response*/
   if(TRUE == wrIsTmrRunning(&(nghEnbCb->enbCfgUpTm),
                   WR_TMR_ENB_CONFIG_UPDATE_WAIT))
   {
      nghEnbCb->isParallelCfgUpdate = TRUE;
      WR_FREE_EVNT(cfgUpReq);
      RETVALUE(ROK);
   }
   RLOG_ARG0(L_DEBUG,DBG_PEERID, nghEnbCb->peerId,
         "Sending X2 CONFIGURATION UPDATE REQUEST");

   /* Trigger corresponding primitive */
   if (ROK != WrIfmX2apGpReq(cfgUpReq))
   {
      RLOG0(L_ERROR,
            " Failed to Send eNode Config Update Request ");
      WR_FREE_EVNT(cfgUpReq);
      RETVALUE(RFAILED);
   }
   else
   {
      RLOG2(L_EVENT,"[X2AP]:Sending ENB CONFIGURATION UPDATE [Src(%ld)-->Peer(%ld)]",
            wrEmmCb.enbId,enbId);
   }
   /*Start  the Timer*/
#if 0
#else
   wrStartTmr((PTR)nghEnbCb, WR_TMR_ENB_CONFIG_UPDATE_WAIT,
         wrEmmCb.tmrCfg.x2UpdateTmr);
#endif

   RETVALUE(ROK);
} /* wrEmmSndEnCfgUpReq*/

/** @brief This function is for filling the eNodeB Config Update
 *         Request failure message
 * @details
 *
 *     Function: wrEmmBldEBCfgUptReqFail
 *
 *         Processing steps:
 *         -Filling the cause value and time to wait IE
 *
 * @param [in]  incTimeToWait : Include Time To Wait IE 
 * @param [out] pdu           : Czt Event Structure
 * @return S16
 *   -# Success : ROK
 *   -# Failure   : RFAILED
 */

PUBLIC S16 wrEmmBldEBCfgUptReqFail
(
 CztEvnt                      **pdu,
 Bool                         incTimeToWait
 )
{

   CztEvnt                   *x2ConfigUpdPdu = NULLP;
   S16                       ret = ROK;
   CztENBConfigUpdFail       *enCfgUpReq = NULLP;
   U8                        numComp = 0;

   TRC2(wrEmmBldEBCfgUptReqFail);

   RLOG0(L_DEBUG,"Building eNode Config Update Request Failure ");

   /* Allocate and Initialize memory control point */
   ret = cmAllocEvnt(sizeof(CztEvnt), WR_CZ_MEM_SDU_SIZE,
         &wrCb.mem, (Ptr *)&x2ConfigUpdPdu);
   if (ret != ROK)
   {
      RLOG0(L_FATAL,"Memory allocation failed.");
      RETVALUE(RFAILED);
   }
   /* Initialize the PDU */
   cmMemset((U8 *)&(x2ConfigUpdPdu->pdu), 0, sizeof(CztX2AP_PDU));

   /* Fill the contents of initiatingMsg */
   wrFillTknU8(&(x2ConfigUpdPdu->pdu.choice), X2AP_PDU_UNSUCCESSFULOUTCOME);
   wrFillTknU8(&(x2ConfigUpdPdu->pdu.val.unsuccessfulOutcome.pres),PRSNT_NODEF);
   wrFillTknU32(&(x2ConfigUpdPdu->pdu.val.unsuccessfulOutcome.procedureCode),
         Cztid_eNBConfigUpd);
   wrFillTknU32(&(x2ConfigUpdPdu->pdu.val.unsuccessfulOutcome.criticality),
         CztCriticalityrejectEnum);
   enCfgUpReq =
      &(x2ConfigUpdPdu->pdu.val.unsuccessfulOutcome.value.u.cztENBConfigUpdFail);

   wrFillTknU8(&(enCfgUpReq->pres),PRSNT_NODEF);

   if ( incTimeToWait == TRUE )
   {
      numComp = WR_EMM_X2_CONFIG_FAIL_IE_COUNT;
   }
   else
   {
      numComp = WR_EMM_X2_CONFIG_FAIL_IE_COUNT - 1;
   }


   wrFillTknU16(&(enCfgUpReq->protocolIEs.noComp) ,numComp);

   /* Get memory for numComp */
   if ((cmGetMem(x2ConfigUpdPdu,
               (numComp * sizeof(CztProtIE_Field_ENBConfigUpdFail_IEs)),
               (Ptr*)&enCfgUpReq->protocolIEs.member)) != ROK)
   {
      RLOG0(L_FATAL,"Memory allocation failed. ");
      /* Free the memory of Event Structure */
      WR_FREE_EVNT(x2ConfigUpdPdu);
      RETVALUE(RFAILED);
   } /* end of if statement */

   /*Filling Cause value*/
   wrFillTknU32(&(enCfgUpReq->protocolIEs.member[0].id), Cztid_Cause);
   wrFillTknU8(&(enCfgUpReq->protocolIEs.member[0].pres), PRSNT_NODEF);
   wrFillTknU32(&(enCfgUpReq->protocolIEs.member[0].criticality),
         CztCriticalityignoreEnum);

   wrFillTknU8(&(enCfgUpReq->protocolIEs.member[0].value.u.cztCause.choice),
         WR_CZ_CAUSE_TRANSPORT);
   wrFillTknU32(
         &(enCfgUpReq->protocolIEs.member[0].value.u.cztCause.val.protocol),
         CztCauseTporttransport_resource_unavailableEnum);

   /*Filling time to wait IE*/
   if ( incTimeToWait == TRUE )
   {
      wrFillTknU32(&(enCfgUpReq->protocolIEs.member[1].id), Cztid_TimeToWait);
      wrFillTknU8(&(enCfgUpReq->protocolIEs.member[1].pres), PRSNT_NODEF);
      wrFillTknU32(&(enCfgUpReq->protocolIEs.member[1].criticality),
            CztCriticalityignoreEnum);

      wrFillTknU32(&(enCfgUpReq->protocolIEs.member[1].value.u.cztTimeToWait),
            wrEmmCb.tmrCfg.x2UpdTimeToWait);
   }
   *pdu = x2ConfigUpdPdu;
   RETVALUE(ROK);
}

/** @brief This function is for sending the eNodeB Config update Request
 *         Failure message to X2AP layer
 * @details
 *
 *     Function: wrEmmSndEnCfgUpReqFail
 *
 *         Processing steps:
 *         -Building the eNodeB Config update Request failure message
 *         -Sending to X2AP layer
 *
 *
 * @param [in] peerId        : peer ID
 * @param [in] incTimeToWait : Include Time to wait
 * @return S16 
 *    -# Success : ROK
 *    -# Failure  : RFAILED
 */
PUBLIC S16 wrEmmSndEnCfgUpReqFail
(
 U32                          peerId,
 Bool                         incTimeToWait
 )
{
   CztEvnt                   *cfgUpReq = NULLP;
   U16                       retVal = 0;
   U32                       enbId = 0;

   TRC2(wrEmmSndEnCfgUpReq);

   WR_GET_ENBID_FROM_PEERID(enbId,peerId);
   RLOG0(L_DEBUG,"Triggering eNode Config Update Request Failure ");

   /* Build  eNode Config update Request message */
   retVal = wrEmmBldEBCfgUptReqFail(&cfgUpReq, incTimeToWait);
   if (ROK != retVal)
   {
      RLOG0(L_ERROR,"Failed to sent eNode Config Update Request ");
      RETVALUE(RFAILED);
   }

   cfgUpReq->peerId = peerId;

   RLOG_ARG0(L_DEBUG,DBG_PEERID, peerId,
         "Sending X2 CONFIGURATION UPDATE REQUEST FAILURE");
   /* Trigger corresponding primitive */
   if (ROK != WrIfmX2apGpRsp(cfgUpReq))
   {
      RLOG0(L_ERROR," Failed to Send eNode Config Update Request ");
      WR_FREE_EVNT(cfgUpReq);
      RETVALUE(RFAILED);
   }
   else
   {
      RLOG2(L_EVENT,"[X2AP]:Sending ENB CONFIGURATION UPDATE FAILURE [Src(%ld)-->Peer(%ld)]",
            wrEmmCb.enbId,enbId);
   }

   RETVALUE(ROK);
} /* wrEmmSndEnCfgUpReqFail*/

/** @brief This function is for filling the eNodeB config update
 *          Ack Message
 * @details
 *
 *     Function: wrEmmBldEBCfgUptReqAck
 *
 *         Processing steps:
 *         -Filling the message type
 *
 *
 * @param[out] pdu : Czt Event Structure
 * @return S16 
 *     -# Success : ROK
 *     -# Failure  : RFAILED
 */

PUBLIC S16 wrEmmBldEBCfgUptReqAck
(
 CztEvnt                      **pdu
 )
{
   CztEvnt                   *x2ConfigUpdPdu = NULLP;
   S16                       ret = ROK;
   U16                       noComp = 0;

   TRC2(wrEmmBldEBCfgUptReqAck);

   RLOG0(L_DEBUG,"Building eNode Config Update Request Ack ");

   /* Allocate and Initialize memory control point */
   ret = cmAllocEvnt(sizeof(CztEvnt), WR_CZ_MEM_SDU_SIZE,
         &wrCb.mem, (Ptr *)&x2ConfigUpdPdu);
   if (ret != ROK)
   {
      RLOG0(L_FATAL,"Memory allocation failed.");
      RETVALUE(RFAILED);
   }
   /* Initialize the PDU */
   cmMemset((U8 *)&(x2ConfigUpdPdu->pdu), 0, sizeof(CztX2AP_PDU));

   /* Fill the contents of initiatingMsg */
   wrFillTknU8(&(x2ConfigUpdPdu->pdu.choice), X2AP_PDU_SUCCESSFULOUTCOME);
   wrFillTknU8(&(x2ConfigUpdPdu->pdu.val.successfulOutcome.pres),PRSNT_NODEF);
   wrFillTknU32(&(x2ConfigUpdPdu->pdu.val.successfulOutcome.procedureCode),
         Cztid_eNBConfigUpd);
   wrFillTknU32(&(x2ConfigUpdPdu->pdu.val.successfulOutcome.criticality),
         CztCriticalityrejectEnum);
   wrFillTknU8(&(x2ConfigUpdPdu->pdu.val.successfulOutcome.value.u.cztENBConfigUpdAckg.pres),
         PRSNT_NODEF);
   wrFillTknU16(&(x2ConfigUpdPdu->pdu.val.successfulOutcome.value.u.cztENBConfigUpdAckg.protocolIEs.noComp),  noComp);
   *pdu = x2ConfigUpdPdu;
   RETVALUE(ROK);
}


/** @brief This function id for sending eNodeB config update Request Ack
 *          Message to X2AP layer
 * @details
 *
 *     Function: wrEmmSndEnCfgUpReqAck
 *
 *         Processing steps:
 *         -Bulding the eNodeB config update Request Ack Message
 *         -Sending to X2AP layer
 *
 * @param[in] peerId : peer ID
 * @return S16 
 *   -# Success : ROK
 *   -# Failure   : RFAILED
 */

PUBLIC S16 wrEmmSndEnCfgUpReqAck
(
 U32                          peerId
 )
{
   CztEvnt                   *cfgUpReq = NULLP;
   U16                       retVal = 0;
   U32                       enbId = 0;

   TRC2(wrEmmSndEnCfgUpReq);

   WR_GET_ENBID_FROM_PEERID(enbId,peerId);

   RLOG0(L_DEBUG," Triggering eNode Config Update Request Ack ");

   /* Build  eNode Config update Request message */
   retVal = wrEmmBldEBCfgUptReqAck(&cfgUpReq);
   if (ROK != retVal)
   {
      RLOG0(L_ERROR,"Failed to sent eNode Config Update Request Ack");
      RETVALUE(RFAILED);
   }

   cfgUpReq->peerId = peerId;

   RLOG_ARG0(L_DEBUG,DBG_PEERID, peerId,
         "Sending X2 CONFIGURATION UPDATE REQUEST ACKNOWLEDGMENT");
   /* Trigger corresponding primitive */
   if (ROK != WrIfmX2apGpRsp(cfgUpReq))
   {
      RLOG0(L_ERROR," Failed to Send eNode Config Update Request ");
      WR_FREE_EVNT(cfgUpReq);
      RETVALUE(RFAILED);
   }
   else
   {
      RLOG2(L_EVENT,"[X2AP]:Sending ENB CONFIGURATION UPDATE ACKNOWLEDGE [Src(%ld)-->Peer(%ld)]",
            wrEmmCb.enbId,enbId);
   }

   RETVALUE(ROK);
} /* wrEmmSndEnCfgUpReqAck*/

/** @brief This function is for finding the Neighbour Cell CB
 *
 * @details
 *
 *     Function: wrEmmFindNeigCellCb
 *
 *         Processing steps:
 *         -iterate Neigbbour cell CB list
 *         -if ecgi matches return the neighbour cell CB
 *
 * @param[in] neghEnList : Neighbour eNodeB List Control
 * @param[in] ecgi          : ECGI
 * @param[out] node       : Neighbour Served Cell Control Block
 * @return S16
 *      -# Success : ROK
 *      -# Failure : RFAILED
 */

PRIVATE S16 wrEmmFindNeigCellCb
(
 CmLListCp                    *neghEnList,
 WrEcgi                       *ecgi,
 WrEutranNeighCellCb          **node
 )
{
   CmLList                  *list = NULLP;
   U8                       count = 0;
   WrEutranNeighCellCb      *neServCellCb = NULLP;

   CM_LLIST_FIRST_NODE(neghEnList, list);

   for (count =0; count < neghEnList->count; count++)
   {
      neServCellCb =(WrEutranNeighCellCb *)list->node;
      if (cmMemcmp((U8 *)&neServCellCb->eCgi, (U8 *)ecgi, sizeof(WrEcgi)) ==
            FALSE)
      {
         *node =  neServCellCb;
         RETVALUE(ROK);
      }
      CM_LLIST_NEXT_NODE(neghEnList,list);
   }
   RETVALUE(RFAILED);
}

/** @brief This function is for search the Neighbour Cell Info
 *
 * @details
 *
 *     Function: wrEmmCmpNeigCellInfo
 *
 *         Processing steps:
 *         -Check  all Served cell info (CELLID, PCI, PLMN etc) 
 *         -If not matches return RFAILED
 *         -else return ROK
 *
 * @param[in] ServCellCb        : New cellCb
 * @param[in] neServCellCb      : Found Same ecgi Cell Cb in Lst 
 * @return S16
 *      -# Success : ROK
 *      -# Failure : RFAILED
 */

PRIVATE S16 wrEmmCmpNeigCellInfo
(
 WrEutranNeighCellCb          *ServCellCb, 
 WrEutranNeighCellCb          *neServCellCb
 )
{
   U8     i = 0;

   if(neServCellCb->enbId != ServCellCb->enbId)
   {
      RLOG2(L_ERROR, "Cell Enodeb Id is not matching, " 
            "new enbId (%d) old enbId (%d)", neServCellCb->enbId,ServCellCb->enbId);
      RETVALUE(RFAILED);
   }
   if(neServCellCb->pci != ServCellCb->pci)
   {
      RLOG2(L_ERROR, "Cell PCI is not matching, "
            "new PCI (%d) old PCI (%d)", neServCellCb->pci,ServCellCb->pci);
      RETVALUE(RFAILED);
   }
   if(neServCellCb->tac != ServCellCb->tac)
   {
      RLOG2(L_ERROR, "Cell TAC is not matching, "
            "new TAC (%d) old TAC (%d)", neServCellCb->tac,ServCellCb->tac);
      RETVALUE(RFAILED);
   }
   for(i=0; i < neServCellCb->numPlmn; i++)
   {
      if(neServCellCb->plmn[i].mcc[0] != ServCellCb->plmn[i].mcc[0])
      {
         RLOG2(L_ERROR, "Cell PLMN is not matching, new plmn mcc[0] (%d) old plmn mcc[0] (%d)",
               neServCellCb->plmn[i].mcc[0],ServCellCb->plmn[i].mcc[0]);
         RETVALUE(RFAILED);
      }
      if(neServCellCb->plmn[i].mcc[1] != ServCellCb->plmn[i].mcc[1])
      {
         RLOG2(L_ERROR, "Cell PLMN is not matching, new plmn mcc[1] (%d) old plmn mcc[1] (%d)",
               neServCellCb->plmn[i].mcc[1],ServCellCb->plmn[i].mcc[1]);
         RETVALUE(RFAILED);
      }
      if(neServCellCb->plmn[i].mcc[2] != ServCellCb->plmn[i].mcc[2])
      {
         RLOG2(L_ERROR, "Cell PLMN is not matching, new plmn mcc[2] (%d) old plmn mcc[2] (%d)",
               neServCellCb->plmn[i].mcc[2],ServCellCb->plmn[i].mcc[2]);
         RETVALUE(RFAILED);
      }
      if(neServCellCb->plmn[i].mnc[0] != ServCellCb->plmn[i].mnc[0])
      {
         RLOG2(L_ERROR, "Cell PLMN is not matching, new plmn mnc[0] (%d) old plmn mnc[0] (%d)",
               neServCellCb->plmn[i].mnc[0],ServCellCb->plmn[i].mnc[0]);
         RETVALUE(RFAILED);
      }
      if(neServCellCb->plmn[i].mnc[1] != ServCellCb->plmn[i].mnc[1])
      {
         RLOG2(L_ERROR, "Cell PLMN is not matching, new plmn mnc[1] (%d) old plmn mnc[1] (%d)",
               neServCellCb->plmn[i].mnc[1],ServCellCb->plmn[i].mnc[1]);
         RETVALUE(RFAILED);
      }
      if(neServCellCb->plmn[i].mnc[2] != ServCellCb->plmn[i].mnc[2])
      {
         RLOG2(L_ERROR, "Cell PLMN is not matching, new plmn mnc[2] (%d) old plmn mnc[2] (%d)",
               neServCellCb->plmn[i].mnc[2],ServCellCb->plmn[i].mnc[2]);
         RETVALUE(RFAILED);
      }
   }
   if(neServCellCb->earfcnDl != ServCellCb->earfcnDl)
   {
      RLOG2(L_ERROR, "Cell earfcnDl is not matching, new earfcnDl (%d) old earfcnDl (%d)",
            neServCellCb->earfcnDl,ServCellCb->earfcnDl);
      RETVALUE(RFAILED);
   }
   if(neServCellCb->earfcnUl != ServCellCb->earfcnUl)
   {
      RLOG2(L_ERROR, "Cell earfcnUl is not matching, new earfcnUl (%d) old earfcnUl (%d)",
            neServCellCb->earfcnUl,ServCellCb->earfcnUl);
      RETVALUE(RFAILED);
   }
#ifdef LTE_TDD
   if(neServCellCb->ulDlSubframeCfg != ServCellCb->ulDlSubframeCfg)
   {
      RLOG2(L_ERROR, "Cell ulDlSubframeCfg is not matching, new ulDlSubframeCfg (%d) old ulDlSubframeCfg (%d)",
            neServCellCb->ulDlSubframeCfg, ServCellCb->ulDlSubframeCfg);
      RETVALUE(RFAILED);
   }
#endif

   RETVALUE(ROK);
}
/* End of wrEmmCmpNeigCellInfo */


/** @brief This function is for finding the Neighbours negighbour
 *          Cell CB
 * @details
 *
 *     Function: wrEmmFindNeigNeigCellCb
 *
 *         Processing steps:
 *         -iterate the neighbours neighbours list
 *         -if ecgi matches return the Cell CB
 *
 * @param[in] neghEnList : Neighbours Neighbour List Control
 * @param[in] ecgi          : ECGI
 * @param[out] node       : Neighbours Neighbour Control Block
 * @return S16
 *     -# Success : ROK
 *     -# Failure   : RFAILED
 */
/** @brief This function is for handling the Served Cell to Add IE
 *
 * @details
 *
 *     Function: wrEmmEnCfgUpPrcSrvCellToAdd
 *
 *         Processing steps:
 *         -Parse the served cell IE
 *         -Add new served cell CB in the served cell list
 *
 *
 * @param[out] neighEnCb : Neighbour eNodeB Control Block
 * @param[in] srvCell        : Served Cell to Add IE
 * @return S16 
 *    -# Success  : ROK
 *    -#          : ROKDUP
 *    -# Failure  : RFAILED
 */
PRIVATE S16 wrEmmEnCfgUpPrcSrvCellToAdd
(
 WrNeighEnbCb                 *neighEnCb,
 CztSrvdCells                 *srvCell
 )
{
   U8                        noComp = 0;
   U8                        idx1 = 0;
   U8                        index1 = 0;
   U8                        plmnIdx = 0;
   U8                        extIeCount = 0;
   WrEutranNeighCellCb       *neServCellCb = NULLP;
   WrEutranNeighCellCb       *temp = NULLP;
   CztSrvdCell_Inform        *srvdCellInfo = NULLP;
   CztNeighbour_InformMember *srvNeigInfo = NULLP;
#ifdef LTE_TDD
   CztTDD_Info               *tddInfo;
   CztSpecialSubframe_Info   *specialSfInfo;
#endif
   WrNeighNeighCb            *neighNeighCb = NULLP;
   CztProtExtnField_SrvdCell_Inform_ExtIEs *extIE =NULLP;
   U32                       cellIdx = 0;
   S16                       ret = ROK;
   /* CSG_DEV */
   Bool                      isCsgIdSentInMsg = FALSE;
   U8                        extIeCnt;

   TRC2(wrEmmEnCfgUpPrcSrvCellToAdd);

   RLOG0(L_DEBUG," Handling served cell to Add IE");

   noComp = srvCell->noComp.val;

   for (idx1 = 0; idx1 < noComp; idx1++)
   {
      /*Handling served CellInfo IE*/
      if (srvCell->member[idx1].servedCellInfo.pres.pres == TRUE)
      {
         srvdCellInfo =&(srvCell->member[idx1].servedCellInfo);
         WR_ALLOC(&neServCellCb, sizeof(WrEutranNeighCellCb));
         if (neServCellCb == NULLP)
         {
            RLOG0(L_FATAL,"Memory allocation failed.");
            RETVALUE(RFAILED);
         }
#ifndef LTE_TDD
         /*Filling the values*/
         neServCellCb->dlBw =
            srvdCellInfo->eUTRA_Mode_Info.val.fDD.dL_Transmission_Bandwidth.val;
         neServCellCb->ulBw=
            srvdCellInfo->eUTRA_Mode_Info.val.fDD.uL_Transmission_Bandwidth.val;
         neServCellCb->earfcnUl=
            srvdCellInfo->eUTRA_Mode_Info.val.fDD.uL_EARFCN.val;
         neServCellCb->earfcnDl=
            srvdCellInfo->eUTRA_Mode_Info.val.fDD.dL_EARFCN.val;
#else
         tddInfo = &srvdCellInfo->eUTRA_Mode_Info.val.tDD;
         specialSfInfo = &tddInfo->specialSubframe_Info;
         /*Filling the values*/
         neServCellCb->dlBw = tddInfo->transmission_Bandwidth.val;
         neServCellCb->ulBw = neServCellCb->dlBw;
         neServCellCb->earfcnUl = tddInfo->eARFCN.val;
         neServCellCb->earfcnDl = neServCellCb->earfcnDl;
         neServCellCb->ulDlSubframeCfg =tddInfo->subframeAssignment.val;
         neServCellCb->splSfCfg = specialSfInfo->specialSubframePatterns.val;
         neServCellCb->cpDl =  specialSfInfo->cyclicPrefixDL.val;
         neServCellCb->cpUl = specialSfInfo->cyclicPrefixUL.val;


#endif      
         neServCellCb->numPlmn = srvdCellInfo->broadcastPLMNs.noComp.val;
         for (plmnIdx = 0; plmnIdx < neServCellCb->numPlmn; plmnIdx++)
         {
            wrEmmNghEnbParsePlmnIe(&srvdCellInfo->broadcastPLMNs.member[plmnIdx],
                  &neServCellCb->plmn[plmnIdx]);
         }

         neServCellCb->pci = srvdCellInfo->pCI.val;
         wrEmmNghEnbParseTknStr4Ie(&(srvdCellInfo->cellId.eUTRANcellIdentifier),
               &neServCellCb->eCgi.eutranCellId);
         wrEmmNghEnbParsePlmnIe(&(srvdCellInfo->cellId.pLMN_Identity),
               &(neServCellCb->eCgi.plmnId));
         neServCellCb->cio = WR_EMM_DEFAULT_OFFSET;
         neServCellCb->qoffset = WR_EMM_DEFAULT_OFFSET;
         neServCellCb->enbId = neighEnCb->enbId;
         neServCellCb->enbType = neighEnCb->enbType;
         cmMemcpy((U8 *)&neServCellCb->enbPlmn,
               (U8 *)&neighEnCb->plmnId, sizeof(neighEnCb->plmnId));
         neServCellCb->tac = srvdCellInfo->tAC.val[0];
         neServCellCb->tac <<= 8;
         neServCellCb->tac |= srvdCellInfo->tAC.val[1];
         /*Finding the neighbour served cell Control Block*/
         if (wrEmmFindNeigCellCb(&(neighEnCb->srvdCellLst),
                  &(neServCellCb->eCgi),&temp) == ROK)
         {
            if (wrEmmCmpNeigCellInfo(neServCellCb, temp) == ROK)
            {
               RLOG0(L_INFO, "neighbour cell exist with same parameters");
               WR_FREE(neServCellCb, sizeof(WrEutranNeighCellCb));
               RETVALUE(ROKDUP);
            }
            else
            {
               RLOG0(L_ERROR, "neighbour cell exist with different parameters");
               WR_FREE(neServCellCb, sizeof(WrEutranNeighCellCb));
               RETVALUE(RFAILED);
            }
         }

         /*Adding new neighbour served control block */
         cmLListInit(&(neServCellCb->neighNeighLstCp));
         neServCellCb->srvdCellLstEnt.node =(PTR)neServCellCb;
         cmLListAdd2Tail(&(neighEnCb->srvdCellLst),
               &(neServCellCb->srvdCellLstEnt));

         /*Handling Number of Antenna Ports and PRACH Configuration IE*/
         /*Getting number ie present in the  iE_Extns*/
         extIeCount =srvdCellInfo->iE_Extns.noComp.val;
         /*If any ie present getting those values*/
         if (extIeCount != 0)
         {
            for (index1 = 0; index1 < extIeCount; index1++)
            {
               extIE =&(srvdCellInfo->iE_Extns.member[index1]);
               switch (extIE->id.val)
               {
                  /*Handling Number of Antenna Ports IE*/
                  case Cztid_Number_of_Antennaports:
                     {
                        neServCellCb->numAntenna =
                           (U8)extIE->extensionValue.u.cztNumber_of_Antennaports.val;
                     }
                     break;
                     /*Handling PRACH Configuration IE*/
                  case Cztid_PRACH_Config:
                     {
                        neServCellCb->prachCfg.highSpeedFlag =
                           (U8)extIE->extensionValue.u.cztPRACH_Config.
                           highSpeedFlag.val;
                        neServCellCb->prachCfg.prachCfgIdx=
                           (U8)extIE->extensionValue.u.cztPRACH_Config.
                           prach_ConfigIdx.val;
                        neServCellCb->prachCfg.rootSeqIdx=
                           (U16)extIE->extensionValue.u.cztPRACH_Config.
                           rootSequenceIdx.val;
                        neServCellCb->prachCfg.prachFreqOffset=
                           (U8)extIE->extensionValue.u.cztPRACH_Config.
                           prach_FreqOffset.val;
                        neServCellCb->prachCfg.zeroCorrZone=
                           (U8)extIE->extensionValue.u.cztPRACH_Config.
                           zeroCorrelationIdx.val;
                     }
                     break;
                     /* CSG_DEV start */
                     /*Handling CSG ID IE*/
                  case Cztid_CSG_Id:
                     {
                        isCsgIdSentInMsg = TRUE;
                        wrEmmCsgNbrUpdCsgInfoFrmExtnIe(
                              &extIE->extensionValue.u.cztCSG_Id,
                              neServCellCb);
                        neServCellCb->isCsgAccessModeResolved =
                           WR_EMM_CELL_ACCESS_MODE_NOT_DETERMINED;
                     }
                     break;
                     /* CSG_DEV end */
               }
            }
         }
         /*Handling neighbour info IE*/
         if (srvCell->member[idx1].neighbour_Info.noComp.val != 0)
         {
            for (index1 = 0; index1 <
                  srvCell->member[idx1].neighbour_Info.noComp.val; index1++)
            {
               WR_ALLOC(&neighNeighCb, sizeof(WrNeighNeighCb));
               if(NULLP == neighNeighCb)
               {
                  RLOG0(L_FATAL, "Memory allocation failed.");
                  RETVALUE(RFAILED);
               }
               srvNeigInfo
                  = &(srvCell->member[idx1].neighbour_Info.member[index1]);

               /*Filling the value*/
               neighNeighCb->dlEarfcn = srvNeigInfo->eARFCN.val;
               neighNeighCb->pci = srvNeigInfo->pCI.val;

               wrEmmNghEnbParseTknStr4Ie(&(srvNeigInfo->eCGI.eUTRANcellIdentifier),
                     &neighNeighCb->ecgi.eutranCellId);
               wrEmmNghEnbParsePlmnIe(&(srvNeigInfo->eCGI.pLMN_Identity),
                     &(neighNeighCb->ecgi.plmnId));

               /*Update TAC*/
               for(extIeCnt = 0; extIeCnt < srvNeigInfo->iE_Extns.noComp.val;
                     extIeCnt++)
               {
                  switch(srvNeigInfo->iE_Extns.member[extIeCnt].id.val)
                  {
                     case Cztid_NeighbourTAC: /*Update received TAC in nghNeighCb*/
                            wrEmmNghEnbParseTknStr4Ie(&(srvNeigInfo->iE_Extns.member[extIeCnt].
                               extensionValue.u.cztTAC), &(neighNeighCb->tac));
                         break;
                  }
               }
               /*Adding the new neighbours neighbour control block*/
               neighNeighCb->LstEnt.node = (PTR)neighNeighCb;
               cmLListAdd2Tail(&(neServCellCb->neighNeighLstCp),
                     &(neighNeighCb->LstEnt));
            }
         }

         for ( cellIdx = 0; cellIdx < wrEmmCb.numCells; cellIdx++ )
         {
            /* CSG_DEV start */
            if(WR_EMM_CELL_ACCESS_MODE_NOT_DETERMINED == 
                  neServCellCb->isCsgAccessModeResolved)
            {
               wrEmmCsgSetNbrAccessMode(wrEmmCb.cellCb[cellIdx]->cellId,
                     neServCellCb->pci, isCsgIdSentInMsg,
                     &neServCellCb->csgAccessMode);
               neServCellCb->isCsgAccessModeResolved = WR_EMM_CELL_ACCESS_MODE_DETERMINED;
            }
            /* CSG_DEV end */

            ret = wrEmmAnrAddEutranNr(wrEmmCb.cellCb[cellIdx], neServCellCb);

            if(RFAILED == ret)
            {

               RLOG0(L_ERROR," Nr Cell Add In ANR failed ");
            }

         }
      }
   }

   RETVALUE(ROK);
}
/** @brief This function is for handling the modify served cell IE
 *
 * @details
 *
 *     Function: wrEmmEnCfgUpPrcSrvCellToMdfy
 *
 *         Processing steps:
 *         -Parse the modify served cell IE
 *         -Find the cell cb
 *         - Update the cell CB
 *
 * @param [out] neighEnCb : Neighbour eNodeB Control Block
 * @param [in] mdfCell       : Server Cell to Modify IE
 * @return S16
 *   -# Success : ROK
 *   -# Failure   : RFAILED
 */

PRIVATE S16 wrEmmEnCfgUpPrcSrvCellToMdfy
(
 WrNeighEnbCb                 *neighEnCb,
 CztSrvdCellsToMdfy           *mdfCell
 )
{
   U8                        noComp = 0;
   U8                        idx1 = 0;
   U8                        index1 = 0;
   U8                        extIeCount = 0;
   S16                       retVal = ROK;
   U32                       cellIdx;
   WrEutranNeighCellCb       *neServCellCb =NULLP;
   CztSrvdCell_Inform        *srvdCellInfo =NULLP;
#ifdef LTE_TDD
   CztTDD_Info               *tddInfo;
   CztSpecialSubframe_Info   *specialSfInfo;
#endif
   CztNeighbour_InformMember *srvNeigInfo = NULLP;
   WrEcgi                    oldEcgi;
   WrNeighNeighCb            *neighNeighCb = NULLP;
   CztProtExtnField_SrvdCell_Inform_ExtIEs *extIE =NULLP;
   /* CSG_DEV */
   Bool                      isCsgIdSentInMsg = FALSE;
   U8                        plmnIdx = 0;
   CmLList                  *list = NULLP;
   U8                        extIeCnt;

   TRC2(wrEmmEnCfgUpPrcSrvCellToMdfy);

   RLOG0(L_DEBUG," Handling Modify served cell IE");

   cmMemset((U8*)&oldEcgi, 0, sizeof(WrEcgi));

   noComp = mdfCell->noComp.val;
   for (idx1 = 0; idx1 < noComp; idx1++)
   {
      if (mdfCell->member[idx1].old_ecgi.pres.pres == FALSE)
      {
         RLOG0(L_ERROR,"old ecgi IE missing ");
         RETVALUE(RFAILED);
      }
      /*Getting old ecgi value from old ECGI IE*/
      wrEmmNghEnbParseTknStr4Ie(&(mdfCell->member[idx1].old_ecgi.
               eUTRANcellIdentifier), &oldEcgi.eutranCellId);
      wrEmmNghEnbParsePlmnIe(&(mdfCell->member[idx1].old_ecgi.pLMN_Identity),
            &(oldEcgi.plmnId));
      /*Handling served CellInfo IE*/
      if (mdfCell->member[idx1].servedCellInfo.pres.pres == TRUE)
      {
         srvdCellInfo =&(mdfCell->member[idx1].servedCellInfo);
         /*Finding the neighbour served cell Control Block*/
         if (wrEmmFindNeigCellCb(&(neighEnCb->srvdCellLst),
                  &oldEcgi,&neServCellCb) != ROK)
         {
            RLOG0(L_ERROR,"neighbour served cell Control Block not found ");
            RETVALUE(RFAILED);
         }
         wrEmmAnrDelEutranNr( wrEmmCb.cellCb[0], neServCellCb);
         wrEmmAnrProcNrUpdtInfo(wrEmmCb.cellCb[0], FALSE);
#ifndef LTE_TDD
         /*Filling the values*/
         neServCellCb->dlBw =
            srvdCellInfo->eUTRA_Mode_Info.val.fDD.dL_Transmission_Bandwidth.val;
         neServCellCb->ulBw=
            srvdCellInfo->eUTRA_Mode_Info.val.fDD.uL_Transmission_Bandwidth.val;
         neServCellCb->earfcnUl=
            srvdCellInfo->eUTRA_Mode_Info.val.fDD.uL_EARFCN.val;
         neServCellCb->earfcnDl=
            srvdCellInfo->eUTRA_Mode_Info.val.fDD.dL_EARFCN.val;
#else
         tddInfo = &srvdCellInfo->eUTRA_Mode_Info.val.tDD;
         specialSfInfo = &tddInfo->specialSubframe_Info;
         /*Filling the values*/
         neServCellCb->dlBw = tddInfo->transmission_Bandwidth.val;
         neServCellCb->ulBw = neServCellCb->dlBw;
         neServCellCb->earfcnUl = tddInfo->eARFCN.val;
         neServCellCb->earfcnDl = neServCellCb->earfcnDl;
         neServCellCb->ulDlSubframeCfg = tddInfo->subframeAssignment.val;
         neServCellCb->splSfCfg = specialSfInfo->specialSubframePatterns.val;
         neServCellCb->cpDl = specialSfInfo->cyclicPrefixDL.val;
         neServCellCb->cpUl = specialSfInfo->cyclicPrefixUL.val;
#endif
         neServCellCb->numPlmn = srvdCellInfo->broadcastPLMNs.noComp.val;
         for (plmnIdx = 0; plmnIdx < neServCellCb->numPlmn; plmnIdx++)
         {
            wrEmmNghEnbParsePlmnIe(&srvdCellInfo->broadcastPLMNs.member[plmnIdx],
                  &neServCellCb->plmn[plmnIdx]);
         }

         neServCellCb->pci = srvdCellInfo->pCI.val;
         /* Reset the value to zero, before updating it */
         neServCellCb->eCgi.eutranCellId = 0;
         wrEmmNghEnbParseTknStr4Ie(&(srvdCellInfo->cellId.eUTRANcellIdentifier),
               &neServCellCb->eCgi.eutranCellId);
         wrEmmNghEnbParsePlmnIe(&(srvdCellInfo->cellId.pLMN_Identity),
               &(neServCellCb->eCgi.plmnId));


         /*Handling Number of Antenna Ports and PRACH Configuration IE*/

         /*Getting number ie present in the  iE_Extns*/
         extIeCount =srvdCellInfo->iE_Extns.noComp.val;
         /*if Optional IE's present */
         if (extIeCount != 0)
         {
            for (index1 = 0; index1 < extIeCount; index1++)
            {
               extIE = &(srvdCellInfo->iE_Extns.member[index1]);
               switch (extIE->id.val)
               {
                  /*Handling Number of Antenna Ports IE*/
                  case Cztid_Number_of_Antennaports:
                     {
                        neServCellCb->numAntenna =
                           (U8)extIE->extensionValue.u.cztNumber_of_Antennaports.val;
                     }
                     break;
                     /*Handling PRACH Configuration IE*/
                  case Cztid_PRACH_Config:
                     {
                        neServCellCb->prachCfg.highSpeedFlag =
                           (U8)extIE->extensionValue.u.cztPRACH_Config.
                           highSpeedFlag.val;
                        neServCellCb->prachCfg.prachCfgIdx=
                           (U8)extIE->extensionValue.u.cztPRACH_Config.
                           prach_ConfigIdx.val;
                        neServCellCb->prachCfg.rootSeqIdx=
                           (U16)extIE->extensionValue.u.cztPRACH_Config.
                           rootSequenceIdx.val;
                        neServCellCb->prachCfg.prachFreqOffset=
                           (U8)extIE->extensionValue.u.cztPRACH_Config.
                           prach_FreqOffset.val;
                        neServCellCb->prachCfg.zeroCorrZone=
                           (U8)extIE->extensionValue.u.cztPRACH_Config.
                           zeroCorrelationIdx.val;
                     }
                     break;
                     /* CSG_DEV start */
                     /*Handling CSG ID IE*/
                  case Cztid_CSG_Id:
                     {
                        isCsgIdSentInMsg = TRUE;
                        wrEmmCsgNbrUpdCsgInfoFrmExtnIe(
                              &extIE->extensionValue.u.cztCSG_Id,
                              neServCellCb);
                        neServCellCb->isCsgAccessModeResolved =
                           WR_EMM_CELL_ACCESS_MODE_NOT_DETERMINED;
                     }
                     break;
                     /* CSG_DEV end */
               }
            }
         }

         /*Clear neighour of neighour cells */
         if(neServCellCb->neighNeighLstCp.count != 0)
         {
            CM_LLIST_FIRST_NODE((&neServCellCb->neighNeighLstCp), list);
            while(list != NULLP)
            {
                cmLListDelFrm((&neServCellCb->neighNeighLstCp), list);
                WR_FREE(list->node, sizeof(WrNeighNeighCb));
                CM_LLIST_FIRST_NODE((&neServCellCb->neighNeighLstCp), list);
            }
         }
         
         /*Handling neighbour info IE*/
         if (mdfCell->member[idx1].neighbour_Info.noComp.val != 0)
         {
            for (index1 = 0; index1 <
                  mdfCell->member[idx1].neighbour_Info.noComp.val; index1++)
            {
              WR_ALLOC(&neighNeighCb, sizeof(WrNeighNeighCb));
               if(NULLP == neighNeighCb)
               {
                  RLOG0(L_FATAL, "Memory allocation failed.");
                  RETVALUE(RFAILED);
               }
               srvNeigInfo
                  = &(mdfCell->member[idx1].neighbour_Info.member[index1]);

               /*Filling the value*/
               neighNeighCb->dlEarfcn = srvNeigInfo->eARFCN.val;
               neighNeighCb->pci = srvNeigInfo->pCI.val;

               wrEmmNghEnbParseTknStr4Ie(&(srvNeigInfo->eCGI.eUTRANcellIdentifier),
                     &neighNeighCb->ecgi.eutranCellId);
               wrEmmNghEnbParsePlmnIe(&(srvNeigInfo->eCGI.pLMN_Identity),
                     &(neighNeighCb->ecgi.plmnId));
               /*Update TAC*/
               for(extIeCnt = 0; extIeCnt < srvNeigInfo->iE_Extns.noComp.val;
                     extIeCnt++)
               {
                  switch(srvNeigInfo->iE_Extns.member[extIeCnt].id.val)
                  {
                     case Cztid_NeighbourTAC: /*Update received TAC in nghNeighCb*/
                            wrEmmNghEnbParseTknStr4Ie(&(srvNeigInfo->iE_Extns.member[extIeCnt].
                               extensionValue.u.cztTAC), &(neighNeighCb->tac));
                         break;
                  }
               }

               /*Adding the new neighbours neighbour control block*/
               neighNeighCb->LstEnt.node = (PTR)neighNeighCb;
               cmLListAdd2Tail(&(neServCellCb->neighNeighLstCp),
                     &(neighNeighCb->LstEnt));
            }
         }

         for ( cellIdx = 0; cellIdx < wrEmmCb.numCells; cellIdx++ )
         {
            /* CSG_DEV start */
            if(WR_EMM_CELL_ACCESS_MODE_NOT_DETERMINED == 
                  neServCellCb->isCsgAccessModeResolved)
            {
               wrEmmCsgSetNbrAccessMode(wrEmmCb.cellCb[cellIdx]->cellId,
                     neServCellCb->pci, isCsgIdSentInMsg,
                     &neServCellCb->csgAccessMode);
               neServCellCb->isCsgAccessModeResolved = WR_EMM_CELL_ACCESS_MODE_DETERMINED;
            }
            /* CSG_DEV end */
            wrEmmAnrAddEutranNr(wrEmmCb.cellCb[cellIdx],neServCellCb);
            wrEmmAnrProcNrUpdtInfo(wrEmmCb.cellCb[cellIdx],FALSE);
         }
      }
   }

   RETVALUE(retVal);
}
/** @brief This function is for handling the served cell to delete IE
 *
 * @details
 *
 *     Function: wrEmmEnCfgUpPrcSrvCellToDel
 *
 *         Processing steps:
 *         -Parse the served cell IE to Delete
 *         -Find the Cell CB
 *         -Delete the Cell CB from the list
 *
 * @param[out] neighEnCb : Neighbour eNodeB Control Block
 * @param[in]  delCell : Served Cell to Delete IE
 * @return S16
 *    -# Success : ROK
 *    -# Failure   : RFAILED
 */

PRIVATE S16 wrEmmEnCfgUpPrcSrvCellToDel
(
 WrNeighEnbCb                 *neighEnCb,
 CztOld_ECGIs                 *delCell
 )
{
   U8                        noComp = 0;
   U8                        idx1 = 0;
   U8                        index1 = 0;
   S16                       retVal = ROK;
   U32                       cellIdx;
   WrEcgi                    oldEcgi;
   CztECGI                   *ecgi = NULLP;
   CmLList                   *list = NULLP;
   WrEutranNeighCellCb       *neServCellCb = NULLP;
   WrNeighNeighCb            *neighNeighCb = NULLP;

   TRC2(wrEmmEnCfgUpPrcSrvCellToDel);

   RLOG0(L_DEBUG," Handling Delete cell IE");

   noComp = delCell->noComp.val;

   for (idx1 = 0; idx1 < noComp; idx1++)
   {
      ecgi = &(delCell->member[idx1]);

      /*Getting the ecgi from the ecgi IE*/
      wrEmmNghEnbParseTknStr4Ie(&(ecgi->eUTRANcellIdentifier),
            &oldEcgi.eutranCellId);
      wrEmmNghEnbParsePlmnIe(&(ecgi->pLMN_Identity),&(oldEcgi.plmnId));

      /*Finding the neighbour served cell Control Block*/
      if (wrEmmFindNeigCellCb(&(neighEnCb->srvdCellLst),
               &oldEcgi,&neServCellCb) != ROK)
      {
         RLOG0(L_ERROR,"neighbour served cell Control Block not found ");
         RETVALUE(ROK); 
      }
      /*Deleting all the neighbours neighbour node and list*/
      CM_LLIST_FIRST_NODE(&neServCellCb->neighNeighLstCp, list);

      for (index1=0; index1 < neServCellCb->neighNeighLstCp.count; index1++)
      {
         neighNeighCb =(WrNeighNeighCb *)list->node;
         if (cmLListDelFrm(&(neServCellCb->neighNeighLstCp),
                  (CmLList *)neighNeighCb) != (CmLList  *)neighNeighCb)
         {
            RLOG0(L_ERROR,"neighbour control block deletion failed");
            RETVALUE(RFAILED);
         }
         WR_FREE(neighNeighCb,sizeof(WrNeighNeighCb));
         /*Moving next node to delete*/

         CM_LLIST_NEXT_NODE(&neServCellCb->neighNeighLstCp,list);
      }
      /*Deleting the neighbour cell control block from the list*/
      if (cmLListDelFrm(&(neighEnCb->srvdCellLst),(CmLList *)neServCellCb) !=
            (CmLList  *)neServCellCb)
      {
         RLOG0(L_ERROR,"neighbour served cell Control Block deletion failed ");
         RETVALUE(RFAILED);
      }
      /*Freeing the neighbour cell control block memory*/

      /* Neighbor cell Node will be freed in ANR */
      for ( cellIdx = 0; cellIdx < wrEmmCb.numCells; cellIdx++ )
      {
         wrEmmAnrDelEutranNr(wrEmmCb.cellCb[cellIdx], neServCellCb);
      }

      WR_FREE(neServCellCb,sizeof(WrEutranNeighCellCb));

   }

   RETVALUE(retVal);
}
/** @brief This function id for handling the GU Group ID IE
 *
 * @details
 *
 *     Function: wrEmmEnCfgUpPrcGUListToAddDelete
 *
 *         Processing steps:
 *         -Find the GU Group ID
 *         -if opFlag is delete then delete the ID
 *         -if opFlag is add then add the ID
 *
 * @param [out] neighEnCb  : Neighbour eNodeB Control Block
 * @param [in] guListAddDel : Gu Group Id IE
 * @param [in] opFlag          : Operation flag
 *
 * @return S16 
 *   -# Success : ROK
 *   -# Failure   : RFAILED
 */

PRIVATE S16 wrEmmEnCfgUpPrcGUListToAddDelete
(
 WrNeighEnbCb                 *neighEnCb,
 CztGUGroupIDLst              *guListAddDel,
 U8                           opFlag
 )
{
   U8                        noComp = 0;
   U8                        idx1 = 0;
   U8                        index1 = 0;
   U8                        index3 =0;
   U8                        numGuGrpIds=0;
   U8                        findFlag=0;
   U32                       val = 0;
   WrGuGrpIdLst              guGrp;

   TRC2(wrEmmEnCfgUpPrcGUListToAddDelete);

   RLOG0(L_DEBUG,"Handling Add and Delete GU List cell IE");

   numGuGrpIds = neighEnCb->numGuGrpIds;
   noComp = guListAddDel->noComp.val;

   /*Adding GU list*/
   if (opFlag == FALSE)
   {
      if ((noComp + numGuGrpIds) > WR_MAX_GU_GRP_IDS)
      {
         RLOG0(L_ERROR, "GU ID list size exceeds");
         RETVALUE(RFAILED);
      }
      for (idx1 =0; idx1 < noComp; idx1++ )
      {

         val  = guListAddDel->member[idx1].mME_Group_ID.val[0];
         val  = (val << 8) | guListAddDel->member[idx1].mME_Group_ID.val[1];
         neighEnCb->guGrpIdLst[numGuGrpIds+idx1].mmeGrpId = (U16)val;
         wrEmmNghEnbParsePlmnIe(&(guListAddDel->member[idx1].pLMN_Identity),
               &(neighEnCb->guGrpIdLst[numGuGrpIds+idx1].plmnId));
      }
      neighEnCb->numGuGrpIds +=  noComp;
   }
   /*Deleting GU list*/
   else
   {
      for (idx1 =0; idx1 < noComp; idx1++ )
      {
         cmMemset((U8 *)&guGrp, 0, sizeof(WrGuGrpIdLst));

         val  = guListAddDel->member[idx1].mME_Group_ID.val[0];
         val  = (val << 8) | guListAddDel->member[idx1].mME_Group_ID.val[1];

         guGrp.mmeGrpId = (U16)val;
         wrEmmNghEnbParsePlmnIe(&(guListAddDel->member[idx1].pLMN_Identity),
               &(guGrp.plmnId));
         numGuGrpIds = neighEnCb->numGuGrpIds;
         findFlag = FALSE;
         /*MME_LDBAL*/
         for (index1 =0; index1 < numGuGrpIds; index1++)
         {
            /*Comparing the GU Group ID*/
            if (cmMemcmp((U8 *)&neighEnCb->guGrpIdLst[index1],(U8 *)&guGrp,
                     sizeof(WrGuGrpIdLst)) == FALSE)
            {
               findFlag = TRUE;
               break;
            }
         }
         if (findFlag == FALSE)
         {
            /*If peer sends GU Group ID to delete list with GU Group ID
             * not present in our list for that peer, return by sending ROK
             * so that an ACK will be sent to peer. An ACK is being sent to
             * avoid resending of the same ENB_CFG_UPD by peer to us. From
             * peer's perspective, GU Group ID shall be deleted in either case
             * successfully by us.*/

            RLOG0(L_ERROR, "GU Group ID not found ");
            RETVALUE(ROK);
         }
         for (index3 =index1; index3<(numGuGrpIds-1); index3++)
         {
            neighEnCb->guGrpIdLst[index3]=  neighEnCb->guGrpIdLst[index3+1];
         }
         /*Decrementing the GU Group ID count*/
         neighEnCb->numGuGrpIds -= 1;
      }

   }
   RETVALUE(ROK);
}
/** @brief This function is for handling the return failure
 *         from ENB CFG UPDATE
 * @details
 *
 *     Function: wrEmmEnbCfgUpdHandleRetFailure
 *
 *         Processing steps:
 *         - Based on IE type, handling the RFAILED scenario. 
 *
 * @param [in] type : IE type 
 * @param [in] ret : return value of the correspinding IEs process function 
 *
 * @return S16 
 *   -# Success : ROK/ROKDUP
 *   -# Failure   : RFAILED
 */

PRIVATE S16 wrEmmEnbCfgUpdHandleRetFailure
(
 U32 type,
 S16 ret 
 )
{
   switch(type)
   {
      case Cztid_SrvdCellsToAdd:
         {
            if (ret == RFAILED)
            {
               RLOG0(L_ERROR,"failed to handle"
                     "Cztid_SrvdCellsToAdd IE's ");
               RETVALUE(ret);
            }
         }
         break;

      case Cztid_SrvdCellsToMdfy:
         {
            if (ret != ROK)
            {
               RLOG0(L_ERROR,"failed to Cztid_SrvdCellsToMdfy IE's ");
               RETVALUE(ret);
            }
         }
         break;

      case Cztid_SrvdCellsToDelete:
         {
            if (ret != ROK)
            {
               RLOG0(L_ERROR,"failed to Cztid_SrvdCellsToDelete IE's ");
               RETVALUE(ret);
            }
         }
         break;

      case Cztid_GUGroupIDToAddLst:
         {
            if (ret != ROK)
            {
               RLOG0(L_ERROR,"failed to Cztid_GUGroupIDToAddLst IE's ");
               RETVALUE(ret);
            }
         }
         break;

      case Cztid_GUGroupIDToDeleteLst:
         {

            /*In case of Cztid_GUGroupIDToDeleteLst, we always send ROK*/
            RLOG0(L_ERROR,"failed to Cztid_GUGroupIDToDeleteLst IE's ");
            RETVALUE(ret);

         }
         break;
      default:
         {
            RLOG0(L_ERROR,"wrEmmEnbCfgUpdHandleRetFailure: \
                  Invalid IE type\n");
            RETVALUE(RFAILED);
         }
   }
   RETVALUE(ret);

}

/** @brief This function is for handling the eNode Confiuration Update
 *          Request Message
 * @details
 *
 *     Function: wrEmmPrcENodCfgUpd
 *
 *         Processing steps:
 *         -Parse the message and check the received IE ID
 *         -if served cell to add IE is present then
 *          call the function to parse that IE
 *         -if modify cell IE is present then
 *          call the function to parse that IE
 *         -if served cell to delete IE is present then
 *           call the function to parse that IE
 *
 * @param[in] peerId : Peer ID
 * @param[in] pdu     : Czt Event Structure
 * @return 
 *    -# Success : ROK
 *    -# Failure   : RFAILED
 */

PRIVATE S16 wrEmmPrcENodCfgUpd
(
 U32                          peerId,
 CztEvnt                      *pdu
 )
{
   CztENBConfigUpd           *enCfgUpReq = NULLP;
   U32                       enbId = 0;
   WrNeighEnbCb              *neighEnCb= NULLP;
   S16                       ret = ROK;
   U8                        noComp =0;
   U8                        idx1=0;
#ifdef WR_TEST_CODE
   WrUmmMsgCause             cause;
   UeX2apId                  uex2apId;
#endif
   U8                        cellIdx = 0; /* First cell */
   WrCellCb                  *cellCb = NULLP;
   TRC2(wrEmmPrcENodCfgUpd);

   /*Getting the eNode ID from peerId*/
   WR_GET_ENBID_FROM_PEERID(enbId,peerId);

   RLOG2(L_INFO," Handling eNode Config Update Request Message "
         "from peerId [%ld] enbId[%ld]", peerId, enbId);

   RLOG2(L_EVENT,"[X2AP]:Received ENB CONFIGURATION UPDATE [Peer(%ld)<--Src(%ld)]",
         wrEmmCb.enbId,enbId);

   RLOG_ARG0(L_DEBUG, DBG_PEERID, peerId,
         "Received X2 CONFIGURATION UPDATE REQUEST");

   enCfgUpReq = &(pdu->pdu.val.initiatingMsg.value.u.cztENBConfigUpd);
   if (enCfgUpReq == NULLP )
   {
      RLOG0(L_ERROR,"Empty eNodeB Config Update Request message ");
      RETVALUE(ROK);
   }
#ifdef WR_TEST_CODE
   if ( wrDntRspX2CfgUpd == TRUE )
   {
      RETVALUE(ROK);
   }
   if ( wrTgrX2CfgUpdFail == TRUE )
   {
      wrEmmSndEnCfgUpReqFail(peerId, TRUE);
      RETVALUE(ROK); 
   }
   if ( wrTgrX2CfgUpdFailWoTW == TRUE )
   {
      wrEmmSndEnCfgUpReqFail(peerId, FALSE);
      RETVALUE(ROK); 
   }
   if ( wrTrgX2ErrInd == TRUE )
   {
      cause.causeTyp = 1;
      cause.causeVal = 0;
      uex2apId.oldeNBUeX2apIdPres = FALSE;
      uex2apId.neweNBUeX2apIdPres = FALSE;
      uex2apId.oldeNBUeX2apId = 5;
      uex2apId.neweNBUeX2apId = 6;

      wrEmmNghEnbSndErrorInd(peerId, TRUE, &cause, &uex2apId);
      RETVALUE(ROK);
   }

#endif /* WR_TEST_CODE */
   /*Finding the neighbour eNodeB control block*/
   ret = cmHashListFind(&(wrEmmCb.neighEnbLstCp),(U8 *)&enbId, (U16)sizeof(enbId),0,
         (PTR *)&neighEnCb);
   if (ret != ROK)
   {
      RLOG0(L_ERROR,"cmHashListFind failed ");
      RETVALUE(RFAILED);
   }

   /* Call Anr to initialize the ANR-MEAS interface data structure 
    * to send the NR update info to Measurement 
    */
   for ( cellIdx = 0; cellIdx < wrEmmCb.numCells; cellIdx++ )
   {
      wrEmmAnrInitMeasNrUpdtInfo(wrEmmCb.cellCb[cellIdx]);
   }

   /*Getting number of IE count*/
   noComp = enCfgUpReq->protocolIEs.noComp.val;

   /*Processing the all the IE's*/
   for (idx1 = 0; idx1 < noComp; idx1++)
   {

      switch (enCfgUpReq->protocolIEs.member[idx1].id.val)
      {
         /*
          * Handling served cells to Add IE to add in the neighbour eNodeB cell Control
          * block
          */
         case Cztid_SrvdCellsToAdd:
            {
               ret = wrEmmEnCfgUpPrcSrvCellToAdd(neighEnCb,
                     &(enCfgUpReq->protocolIEs.member[idx1].value.u.cztSrvdCells));

               ret = wrEmmEnbCfgUpdHandleRetFailure( Cztid_SrvdCellsToAdd, ret);
            }
            break;

            /*
             * Handling served cells to Delete IE to delete in the neighbour eNodeB cell
             * Control block
             */
         case Cztid_SrvdCellsToMdfy:
            {
               ret = wrEmmEnCfgUpPrcSrvCellToMdfy(neighEnCb,
                     &(enCfgUpReq->protocolIEs.member[idx1].value.u.cztSrvdCellsToMdfy));

               ret = wrEmmEnbCfgUpdHandleRetFailure( Cztid_SrvdCellsToMdfy, ret);
            }
            break;

            /*
             * Handling GU group ID to Delete IE to Delete in the neighbour eNodeB cell
             * Control block
             */
         case Cztid_SrvdCellsToDelete:
            {
               ret = wrEmmEnCfgUpPrcSrvCellToDel(neighEnCb,
                     &(enCfgUpReq->protocolIEs.member[idx1].value.u.cztOld_ECGIs));

               ret = wrEmmEnbCfgUpdHandleRetFailure( Cztid_SrvdCellsToDelete, ret);
            }
            break;

            /*
             * Handling GU group ID to Add IE to add in the neighbour eNodeB cell
             * Control block
             */
         case Cztid_GUGroupIDToAddLst:
            {
               ret = wrEmmEnCfgUpPrcGUListToAddDelete(neighEnCb,
                     &(enCfgUpReq->protocolIEs.member[idx1].value.u.cztid_GUGroupIDToAddLst),
                     FALSE);

               ret = wrEmmEnbCfgUpdHandleRetFailure( Cztid_GUGroupIDToAddLst, ret);
            }
            break;

            /*
             *Handling GU group ID to Delete IE to Delete in the neighbour eNodeB cell
             *Control block
             */
         case Cztid_GUGroupIDToDeleteLst:
            {
               ret = wrEmmEnCfgUpPrcGUListToAddDelete(neighEnCb,
                     &(enCfgUpReq->protocolIEs.member[idx1].value.u.cztid_GUGroupIDToDeleteLst),
                     TRUE);

               ret = wrEmmEnbCfgUpdHandleRetFailure( Cztid_GUGroupIDToDeleteLst, ret);
            }
            break;

         default:
            {
               RLOG0(L_ERROR,"Invalid IE ID ");
               RETVALUE(RFAILED);
            }
      }

   }
   if(ROK == ret)
   {
      /* Process NR Update Info and send Trigger to Meas and PBM module */
      for ( cellIdx = 0; cellIdx < wrEmmCb.numCells; cellIdx++ )
      {
         ret = wrEmmAnrProcNrUpdtInfo(wrEmmCb.cellCb[cellIdx], FALSE);
      }
   }

   if ((ROKDUP == ret) || (ROK == ret))
   {
      wrEmmSndEnCfgUpReqAck(peerId);
   }
   if(RFAILED == ret)
   {
      wrEmmSndEnCfgUpReqFail(peerId, FALSE);
   }

   /*Send eNodeB Config Update message to SON module for PCI collision/confusion detection*/
   /* Assumptios: Only one cell */
   cellCb = wrEmmCb.cellCb[0];
   wrEmmSendCfgUpdateNghEnbInfoToSon(cellCb, neighEnCb, enCfgUpReq);

   RETVALUE(ROK);
}

/** @brief This function is for Handling the eNodeB Config
 *         Update failure message
 * @details
 *
 *     Function: wrEmmPrcENodCfgUpdFail
 *
 *         Processing steps:
 *         -
 *
 *
 * @param[in] peerId  : Peer ID
 * @param[in] pdu      : Czt Event Structure
 * @return S16 
 *     -# Success : ROK
 *     -# Failure   : RFAILED
 */

PRIVATE S16 wrEmmPrcENodCfgUpdFail
(
 U32                          peerId,
 CztEvnt                      *pdu
 )
{
   U32                       enbId;
   WrNeighEnbCb              *neighEnCb= NULLP;
   S16                       ret = ROK;
   CztTimeToWait *timeToWait = NULLP;

   TRC2(wrEmmPrcENodCfgUpdFail);

   RLOG1(L_DEBUG,"X2 CfgUpd Failure has been received for PEERId [%lu]",peerId);

   RLOG_ARG0(L_DEBUG,DBG_PEERID, peerId,
         "Received X2 CONFIGURATION UPDATE FAILURE");
   /*Getting the eNode ID from peerId*/
   WR_GET_ENBID_FROM_PEERID(enbId,peerId);
   RLOG2(L_EVENT,"[X2AP]:Received ENB CONFIGURATION UPDATE FAILURE [Peer(%ld)<--Src(%ld)]",
         wrEmmCb.enbId,enbId);

   ret = cmHashListFind(&(wrEmmCb.neighEnbLstCp),(U8 *)&enbId,sizeof(enbId),0,
         (PTR *)&neighEnCb);
   if (ret != ROK)
   {
      RLOG0(L_ERROR,"cmHashListFind failed ");
      RETVALUE(RFAILED);
   }

   RLOG0(L_INFO,"eNodeB Configuration Update Failure");

   wrStopTmr((PTR)neighEnCb, WR_TMR_ENB_CONFIG_UPDATE_WAIT); 
   neighEnCb->cfgUpdRtxCnt = 0;
   wrUtlX2GetCztIE(&pdu->pdu, Cztid_TimeToWait, (void **)&timeToWait);
   if ( timeToWait != NULL )
   {
      /* X2AP Layer will start timer , hence no need to start here */
      RLOG0(L_DEBUG,"Received X2-CfgUpd Failure with TimeToWait IE");
   }
   else
   {
      RLOG0(L_DEBUG,"Received X2-CfgFailure Failure with out TimeToWait IE");
      wrStartTmr((PTR)neighEnCb, WR_TMR_ENB_CONFIG_UPDATE_WAIT , 
            wrEmmCb.tmrCfg.internalNoWaitRetryTmr);

      /* sett rtxCnt = 0XFFFF to identify the Failure */    
      neighEnCb->cfgUpdRtxCnt = 0xFFFF;
   }

   RETVALUE(ROK);
}
/** @brief This function is for handling the eNodeB Confgirtion update Ack Message
 *
 * @details
 *
 *     Function: wrEmmPrcENodCfgUpdAck
 *
 *         Processing steps:
 *         -Finding the eNode control block using peer ID
 *         -Stoping the enNode Config update req re-transmission timer
 *
 * @param[in] peerId : Peer ID
 * @param[in] pdu     : Czt Event Structure
 * @return S16
 *     -# Success : ROK
 *     -# Failure   : RFAILED
 */

PRIVATE S16 wrEmmPrcENodCfgUpdAck
(
 U32                          peerId,
 CztEvnt                      *pdu
 )
{
   U32                       enbId;
   WrNeighEnbCb              *neighEnCb= NULLP;
   S16                       ret = ROK;

   TRC2(wrEmmPrcENodCfgUpdAck);

   RLOG0(L_DEBUG,"Processing eNode Config Update Request Ack ");

   RLOG_ARG0(L_DEBUG,DBG_PEERID, peerId,
         "Received X2 CONFIGURATION UPDATE ACKNOWLEDGEMENT");
   /*Getting the eNode ID from peerId*/
   WR_GET_ENBID_FROM_PEERID(enbId,peerId);
   RLOG2(L_EVENT,"[X2AP]:Received ENB CONFIGURATION UPDATE ACKNOWLEDGE [Peer(%ld)<--Src(%ld)]",
         wrEmmCb.enbId,enbId);

   /*Finding the neighbour eNodeB control block*/
   ret = cmHashListFind(&(wrEmmCb.neighEnbLstCp),(U8 *)&enbId,sizeof(enbId),0,
         (PTR *)&neighEnCb);
   if (ret != ROK)
   {
      RLOG0(L_ERROR,"cmHashListFind failed ");
      RETVALUE(RFAILED);
   }

   /* Stop the X2-Setup Timer started for this peer */
   wrStopTmr((PTR)neighEnCb, WR_TMR_ENB_CONFIG_UPDATE_WAIT); 
   neighEnCb->cfgUpdRtxCnt = 0;
   if(neighEnCb->isParallelCfgUpdate)
   {
      neighEnCb->isParallelCfgUpdate = FALSE;
      /*trigger enodeB config update again*/
      ret = wrEmmSndEnCfgUpReq(neighEnCb, &(neighEnCb->cfgUpdRtxInfo));
      if (ROK != ret)
      {
         RLOG0(L_ERROR,"wrEmmSndEnCfgUpReq processing Failed!!!");
         RETVALUE(RFAILED);
      }
   }
   RETVALUE(ROK);
}
/** @brief This function is for handling the eNodeB Config
 *          update request message re-tranmission timer expiry
 * @details
 *
 *     Function: wrEmmNghEnbCfgUptTmrExpiry
 *
 *         Processing steps:
 *         - Filling the peer id and message ID
 *         - Sending the message to X2AP layer to
 *           re-transmit the message
 *
 * @param [in] cb  : Neighbour eNodeB Control Block
 * @return S16 
 *      -# Success : ROK
 *      -# Failure   : RFAILED
 */

PUBLIC S16 wrEmmNghEnbCfgUptTmrExpiry
(
 WrNeighEnbCb                  *cb
 )
{
   CztReTxInfo              *retInfo;
   WrNeighEnbCb             *nghEnbCb;
   S32                       ret = ROK;

   TRC2(wrEmmNghEnbCfgUptTmrExpiry);
   nghEnbCb = (WrNeighEnbCb *)cb;

   RLOG0(L_DEBUG,"Re-transmitting eNode Config Update Request");

   if ( nghEnbCb->cfgUpdRtxCnt == 0xFFFF )
   {
      ret = wrEmmSndEnCfgUpReq(nghEnbCb, &(nghEnbCb->cfgUpdRtxInfo));
      if (ROK != ret)
      {
         RLOG0(L_ERROR,"wrEmmSndEnCfgUpReq Failed!!!");
         RETVALUE(RFAILED);
      }
      /* Reset the rtxCnt to 0 */
      nghEnbCb->cfgUpdRtxCnt = 0;
      RETVALUE(ROK);
   }

   if ( ( nghEnbCb->cfgUpdRtxCnt < wrEmmCb.tmrCfg.x2UpdRetryCount)  || 
         wrEmmCb.tmrCfg.x2UpdRetryCount == 0xFF)
   {
      WR_ALLOC(&retInfo, sizeof(CztReTxInfo));
      if(NULLP == retInfo)
      {
         RLOG0(L_FATAL, "Memory allocation failed.");
         RETVALUE(RFAILED);
      }
      /*Filling peer ID and Message type to re-transmit the
       *  eNodeB Config Update Request msg
       */
      retInfo->peerId  = nghEnbCb->peerId;
      retInfo->msgType = CZT_MSG_CFG_UPD_REQ;

      /* Trigger corresponding primitive */
      if (WrIfmX2apGpRetxReq(retInfo) != ROK)
      {
         RLOG0(L_ERROR,"Failed to Re-Send eNode Config Update Request ");
         RETVALUE(RFAILED);
      }
      nghEnbCb->cfgUpdRtxCnt++;
#if 0
#else
      wrStartTmr((PTR)nghEnbCb, WR_TMR_ENB_CONFIG_UPDATE_WAIT,
            wrEmmCb.tmrCfg.x2UpdateTmr);
#endif
   }
   else
   {
      nghEnbCb->cfgUpdRtxCnt = 0;
   }
   RETVALUE(ROK);
}

/** @brief This function processes all the incoming
 *         unsuccessful  X2AP PDUs.
 *
 * @details
 *
 *     Function: wrEmmNghEnbPrcUnsuccPdu
 *
 *         Processing steps:
 *         - It processes unsuccessful Pdus from peer
 *           eNodeBs and gives it for corresponding handlers
 *           based on message type.
 *
 *
 * @param[in] peerId    : Peer ID
 * @param[in] pdu        : Czt Event Structure
 * @return S16 
 *     -# Success : ROK
 *     -# Failure   : RFAILES
 */

PRIVATE S16 wrEmmNghEnbPrcUnsuccPdu
(
 U32                          peerId,
 CztEvnt                      *pdu
 )
{
   CztUnsuccessfulOutcome    *unsuccMsg = &pdu->pdu.val.unsuccessfulOutcome;
   S16                       ret = ROK;

   switch (unsuccMsg->procedureCode.val)
   {
      case Cztid_x2Setup:
         ret = wrEmmNghEnbPrcSetupFail(peerId, pdu);
         break;
      case Cztid_eNBConfigUpd:
         ret = wrEmmPrcENodCfgUpdFail(peerId, pdu);
         break;
         /* LTE_ADV_ABS starts */
      case Cztid_resourceStatusRprtngInitiation:
         ret = wrEmmNghEnbPrcRsrcStsRptngInit(peerId, pdu, 
               WR_EMM_ABS_RSRC_STS_FLR_RCVD_EVENT);
         break;
         /* LTE_ADV_ABS ends */

   }
   RETVALUE(ret);
}

/** @brief This function processes the successful
 *         X2AP PDUs.
 *
 * @details
 *
 *     Function: wrEmmNghEnbPrcSuccPdu
 *
 *         Processing steps:
 *         - It processes all the incoming successful
 *           X2AP PDUs from the peer eNodebs and gives
 *           it for corresponding handler based on message
 *           type.
 *
 *
 * @param[in]  peerId     : Peer ID
 * @param[in]  pdu         : Czt event strcuture
 * @return S16
 *     -# Success : ROK
 *     -# Failure   : RFAILED
 */

PRIVATE S16 wrEmmNghEnbPrcSuccPdu
(
 U32                          peerId,
 CztEvnt                      *pdu
 )
{
   CztSuccessfulOutcome      *succMsg = &pdu->pdu.val.successfulOutcome;
   S16                       ret = ROK;

   switch (succMsg->procedureCode.val)
   {
      case Cztid_x2Setup:
         ret = wrEmmNghEnbPrcSetupRsp(peerId, pdu);
         break;
      case Cztid_reset:
         ret = wrEmmNghEnbPrcResetRsp(peerId, pdu);
         break;
      case Cztid_eNBConfigUpd:
         ret = wrEmmPrcENodCfgUpdAck(peerId, pdu);
         /* LTE_ADV_ABS starts */
      case Cztid_resourceStatusRprtngInitiation:
         ret = wrEmmNghEnbPrcRsrcStsRptngInit(peerId, pdu, 
               WR_EMM_ABS_RSRC_STS_RSP_RCVD_EVENT);
         break;
         /* LTE_ADV_ABS ends */

         break;
   }

   RETVALUE(ret);
}

/** @brief This function processes the Initlizing
 *         X2AP PDU.
 *
 * @details
 *
 *     Function: wrEmmNghEnbPrcInitPdu
 *
 *         Processing steps:
 *         - It processes all the incoming Initlizing
 *           PDUs from peer EnodeBs.
 *
 *
 * @param[in] peerId  : Peer ID
 * @param[in] pdu      : Czt Event Structure
 * @return S16 
 *    -# Success : ROK
 *    -# Failure   : RFAILED
 */

PRIVATE S16 wrEmmNghEnbPrcInitPdu
(
 U32                          peerId,
 CztEvnt                      *pdu
 )
{
   CztInitiatingMsg          *initMsg = &pdu->pdu.val.initiatingMsg;
   S16 ret = ROK;

   switch (initMsg->procedureCode.val)
   {
      case Cztid_x2Setup:
         ret = wrEmmNghEnbPrcSetupReq(peerId, pdu);
         break;
      case Cztid_eNBConfigUpd:
         ret = wrEmmPrcENodCfgUpd(peerId, pdu);
         break;
      case Cztid_reset:
         ret = wrEmmNghEnbPrcResetReq(peerId, pdu);
         break;
      case Cztid_errorInd:
         ret = wrEmmNghEnbPrcErrInd(peerId, pdu);
         break;
         /* LTE_ADV_ABS starts */ /* || */ /* LTE_ADV_DSFR starts */
      case Cztid_loadInd:
         ret = wrEmmNghEnbPrcLoadInfo(peerId, pdu);
         break;
         /* LTE_ADV_ABS ends */ /* || */ /* LTE_ADV_DSFR ends */
         /* LTE_ADV_ABS starts */
      case Cztid_resourceStatusRprtng:
         ret = wrEmmNghEnbPrcRsrcStsRptng(peerId, pdu);
         break;
      case Cztid_resourceStatusRprtngInitiation:
         ret = wrEmmNghEnbPrcRsrcStsRptngInit(peerId, pdu, 
               WR_EMM_ABS_RSRC_STS_REQ_RCVD_EVENT);
         break;
         /* LTE_ADV_ABS ends */
      default:
         break;
   }

   RETVALUE(ret);
}
/** @brief This function processed the incoming messages
 *         from the peer ENBs.
 *
 * @details
 *
 *     Function: wrEmmNghEnbPrcRcvdPdu
 *
 *         Processing steps:
 *         - It processes all the incoming messages
 *           from peer eNodeBs.
 *
 *
 * @param[in]  peerId   : Peer ID
 * @param[in]  pdu       : Czt Event Structure
 * @return S16 
 *     -# Success : ROK
 *     -# Failure   : RFAILED
 */
PUBLIC S16 wrEmmNghEnbPrcRcvdPdu
(
 U32                          peerId,
 CztEvnt                      *pdu
 )
{
   S16                       ret = ROK;

   switch (pdu->pdu.choice.val)
   {
      case X2AP_PDU_INITIATINGMSG:
         ret = wrEmmNghEnbPrcInitPdu(peerId, pdu);
         break;
      case X2AP_PDU_SUCCESSFULOUTCOME:
         ret = wrEmmNghEnbPrcSuccPdu(peerId, pdu);
         break;
      case X2AP_PDU_UNSUCCESSFULOUTCOME:
         ret = wrEmmNghEnbPrcUnsuccPdu(peerId, pdu);
         break;
   }

   /* Memory not released for Gp procedure */
   /* LTE_ADV starts */
   cmFreeMem((Ptr *)pdu);
   /* LTE_ADV ends */

   RETVALUE(ret);
}

/** @brief This function Triggers eNB CfgUpd msg to its neighbors.
 *
 * @details
 *
 *     Function: wrEmmNghTrigCfgUpd
 *
 *         Processing steps:
 *         - Sends Cfg Upd to each neighbor eNB  
 *
 *
 * @param[in]  cellCb   : Cell Control block
 * @return S16 
 *     -# Success : ROK
 5B
 *     -# Failure   : RFAILED
 */
PUBLIC S16 wrEmmNghTrigCfgUpd
(
 WrCellCb  *cellCb
 )
{
   PTR                        prevEntry = NULLP;
   WrNeighEnbCb              *nghEnbCb;
   WrEmmEnbCfgUpd             cellInfo;
   S16                        retVal = ROK;

   cmMemset((U8 *) &cellInfo, (U8) 0, (S16) sizeof(WrEmmEnbCfgUpd));

   cellInfo.numSrvdCellModified = 1;
   cellInfo.srvdCellMod[0].oldCellId = cellCb->cellId;
   cellInfo.srvdCellMod[0].newCellId = cellCb->cellId;

   /* Trigger Cfg Upd to all neihgbor enodeb's */
   while ((retVal = cmHashListGetNext(&(wrEmmCb.neighEnbLstCp), prevEntry, (PTR *) &nghEnbCb)) == ROK)
   {
      if ( nghEnbCb->x2Connection == WR_NEIGH_ENB_STATE_UP )
      {
         retVal = wrEmmSndEnCfgUpReq(nghEnbCb, &cellInfo);
         if ( retVal != ROK )
         {
            RLOG0(L_ERROR,"wrEmmSndEnCfgUpReq failed");
            RETVALUE(RFAILED);
         }
      }
      prevEntry = (PTR )nghEnbCb;
   }

   RETVALUE(ROK);
}

/** @brief This function Handles X2AP Local err Indication.
 *
 * @details
 *
 *     Function: wrEmmNghHdlLocalErrInd 
 *
 *         Processing steps:
 *         - Stop the timer 
 *
 *
 * @param[in]  : cztLclErrInd 
 * @return S16 
 *     -# Success : ROK
 5B
 *     -# Failure   : RFAILED
 */
PUBLIC S16 wrEmmNghHdlLocalErrInd 
(
 CztLclErrInd   *cztLclErrInd
 )
{
   WrNeighEnbCb              *enbNghCb;
   U32                       eNbId;
   S16                       ret = ROK;

   WR_GET_ENBID_FROM_PEERID(eNbId,cztLclErrInd->peerId);
   /*Get the Neighbouring ENB Cb*/
   ret = cmHashListFind(&(wrEmmCb.neighEnbLstCp), (U8 *)&(eNbId),
         (U16)sizeof(eNbId), 0,(PTR *)&enbNghCb);
   if (ROK != ret)
   {
      RLOG0(L_ERROR,"Failure in the Hash List Find ");
      RETVALUE(RFAILED);
   }

   switch( cztLclErrInd->msgType )
   {
      case CZT_MSG_CFG_UPD_REQ:
         /* Stop the timer */
         RLOG0(L_DEBUG,"Received errInd, Stopping the CfgUpd Rtx timer ");
         wrStopTmr((PTR)enbNghCb, WR_TMR_ENB_CONFIG_UPDATE_WAIT);
         break;

      case CZT_MSG_X2_SETUP:
         RLOG0(L_DEBUG,"Received errInd, Stopping the Setup Rtx timer ");
         wrStopTmr((PTR)enbNghCb, WR_TMR_ENB_SETUP_RSP);
         break;

      case CZT_MSG_RESET: 
         RLOG0(L_DEBUG,"Received errInd, Stopping the Reset Rtx timer ");
         wrStopTmr((PTR)enbNghCb, WR_TMR_X2_RST_RSP);
         break;   
         /* LTE_ADV_ABS starts */
      case CZT_MSG_LOAD_IND:
      case CZT_MSG_RSRC_ST_UPD:
      case CZT_MSG_RSRC_ST_REQ:
      case CZT_MSG_RSRC_ST_RSP:
      case CZT_MSG_RSRC_ST_FAIL:
         {
            WrEmmAbsEvent event;

            event.eventId = WR_EMM_ABS_ERROR_IND_EVENT;
            event.u.errIndEvent.cb = (PTR)cztLclErrInd;
            event.u.errIndEvent.event = cztLclErrInd->msgType;
            wrEmmAbsProcessEvent(&event);
            break;
         }
         /* LTE_ADV_ABS ends */
      default:
         RLOG1(L_ERROR,"X2 Local err Ind not handled for [%d] msg", 
               cztLclErrInd->msgType);
         RETVALUE(RFAILED);
   }

   RETVALUE(ret);
}

#ifdef WR_TEST_CODE
/** @brief This function Triggers eNB Reset msg to its neighbor.
 *
 * @details
 *
 *     Function: wrEmmNghTrigReset
 *
 *         Processing steps:
 *         - Sends Reset Request to Neighbor eNB. 
 *
 *
 * @return S16 
 *     -# Success : ROK
 *     -# Failure   : RFAILED
 */
PUBLIC S16 wrEmmNghTrigReset
(
 Void
 )
{
   PTR                        prevEntry = NULLP;
   WrNeighEnbCb              *nghEnbCb;
   S16                        retVal = ROK;
   WrUmmMsgCause              cause;

   cause.causeTyp = 1;
   cause.causeVal = 0;

   while ((retVal = cmHashListGetNext(&(wrEmmCb.neighEnbLstCp), prevEntry, (PTR *) &nghEnbCb)) == ROK)
   {
      if ( nghEnbCb->x2Connection == WR_NEIGH_ENB_STATE_UP )
      {
         retVal = wrEmmNghEnbSndResetReq(nghEnbCb->peerId, &cause);
         if ( retVal != ROK )
         {
            RLOG0(L_ERROR,"wrEmmNghEnbSndResetReq failed");
            RETVALUE(RFAILED);
         }
      }
      prevEntry = (PTR )nghEnbCb;
   }
   RETVALUE(ROK);
}
#endif /* WR_TEST_CODE */

/** @brief This function make all neighbor eNB to Down.
 *
 * @details
 *
 *     Function: wrEmmNghTrigReset
 *
 *         Processing steps:
 *         - Go through the Neighbor eNB list. If Neighbor eNB is active 
 *           bring down the SCTP connection.
 *         - Sends Release Request to all Neighbor eNBs
 *
 *
 * @return S16 
 *     -# Success : ROK
 *     -# Failure   : RFAILED
 */
PUBLIC S16 wrEmmNghBrngAllX2Down
(
 Void
 )
{
   PTR                        prevEntry = NULLP;
   WrNeighEnbCb              *nghEnbCb;
   CztRlsReq                 *x2RelReq;


   while ((cmHashListGetNext(&(wrEmmCb.neighEnbLstCp), prevEntry, (PTR *) &nghEnbCb)) == ROK)
   {
      if (nghEnbCb->x2Connection == WR_NEIGH_ENB_STATE_UP)
      {
         WR_ALLOCEVNT(&x2RelReq, sizeof(CztRlsReq));
         if(x2RelReq == NULLP)
         {
            RLOG0(L_FATAL,
                  "Memory allocation for X2 Release Request FAILED!!!");
         }
         else
         {
            x2RelReq->peerId = nghEnbCb->peerId;
            x2RelReq->rlsType = CZT_RELTYPE_TERM;

            if ( ROK != WrIfmX2apRelReq (x2RelReq))
            {
               RLOG0(L_ERROR," WrIfmX2apRelReq failed");
               RETVALUE(RFAILED);
            }
         }
      }
      prevEntry = (PTR )nghEnbCb;
   }
   RETVALUE(ROK);
} /* end of wrEmmNghBrngAllX2Down */

/** @brief This function Triggers eNB CfgUpd msg to its neighbors .
 *
 * @details
 *
 *     Function: wrEmmNghTrigCfgUpdNghCellMod
 *
 *         Processing steps:
 *         - Sends Cfg Upd to each neighbor eNB  
 *
 *
 * @param[in]  cellCb   : Cell Control block
 * @param[in]  enbId   : Modified Cell eNodeB ID
 * @return S16 
 *
 *     -# Success : ROK
 *     -# Failure   : RFAILED
 */
PUBLIC S16 wrEmmNghTrigCfgUpdNghCellMod
(
 WrCellCb  *cellCb,
 U32       enbId
 )
{
   PTR                        prevEntry = NULLP;
   WrNeighEnbCb              *nghEnbCb;
   WrEmmEnbCfgUpd             cellInfo;
   S16                        retVal = ROK;

   cmMemset((U8 *) &cellInfo, (U8) 0, (S16) sizeof(WrEmmEnbCfgUpd));

   cellInfo.numSrvdCellModified = 1;
   cellInfo.srvdCellMod[0].oldCellId = cellCb->cellId;
   cellInfo.srvdCellMod[0].newCellId = cellCb->cellId;

   /* Trigger Cfg Upd to all neihgbor enodeb's */
   while ((retVal = cmHashListGetNext(&(wrEmmCb.neighEnbLstCp), prevEntry, (PTR *) &nghEnbCb)) == ROK)
   {
      if ( (nghEnbCb->x2Connection == WR_NEIGH_ENB_STATE_UP)&& (nghEnbCb->enbId != enbId) )
      {
         retVal = wrEmmSndEnCfgUpReq(nghEnbCb, &cellInfo);
         if ( retVal != ROK )
         {
            RLOG0(L_ERROR,"wrEmmSndEnCfgUpReq failed");
            RETVALUE(RFAILED);
         }
      }
      prevEntry = (PTR )nghEnbCb;
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
