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
               

     File:     wr_umm_s1ap_msg.c

     Sid:      $SID$ 

     Prg:      Sriky 

*********************************************************************21*/
/* header include files (.h) */
static const char* RLOG_MODULE_NAME="UMM";
static int RLOG_FILE_ID=70;


static int RLOG_MODULE_ID=32;

#include "wr.h"        /* Common Includes */
#include "wr_emm.h"
#include "wr_umm.h"
#include "wr_ifm_s1ap.h"
#include "wr_utils.h"
#include "szt_asn.x"
#include "wr_umm_trans.h"
#include "wr_umm_s1ap_msg.h"
#include "wr_kpi.h"

/* Move this utility to wr_utils.h to share it between EMM and UMM */
#define WR_LAC_IE_LEN        2          /* Octets */
#define WR_CGI_IE_LEN        28         /* Bits   */
#define WR_PLMNID_IE_LEN     3          /* Octets */
#define WR_SER_NUM_LEN       16         /* Bits   */
#define WR_S1AP_CSG_ID_LEN   27         /* Bits */

EXTERN S16 wrEmmGetTai
(
WrUeCb *ueCb,
WrTai  *tai
);

EXTERN S16 wrEmmGetCgiFromUeCb
(
 WrUeCb                     *ueCb,
 WrCgi                        *cgi
);
/**
 *  @brief This function fills the TAC
 *
 *      Function: wrUmmSztFillTAC
 *
 *          Processing steps:
 *          - encode the TAC value
 *
 *
 *  @param [in]  tac : TAC value to be encoded
 *  @param [out] pdu : pointer to S1AP Pdu 
 *  @param [in]  tAC : pointer for encoding the timer value
 *  @return S16
    -# Success : ROK
    -# Failure : RFAILED
 */

PRIVATE S16 wrUmmSztFillTAC
(
S1apPdu                      *pdu,
U16                          tac,
SztTAC                       *tAC
)
{
   tAC->pres   = PRSNT_NODEF;
   tAC->len    = WR_LAC_IE_LEN;
   tAC->val[1] = (U8)(tac & 0xff);
   tAC->val[0] = (U8)((tac & 0xff00)>>8);
   RETVALUE(ROK);
} /* end of wrUmmSztFillTAC */ 


/**
 *  @brief This function fills the PLMN Id
 *
 *      Function: wrUmmSztFillPLMNId
 *
 *          Processing steps:
 *          - encodes the PLMN ID
 *
 *  @param [out] pdu    : pointer to memCb
 *  @param [in]  plmnId : PLMN ID to be encoded
 *  @param [in] plmnIe : pointer to IE inwhich PLMN is encoded
 *  @return S16
    -# Success : ROK
    -# Failure : RFAILED
 */

PRIVATE S16 wrUmmSztFillPLMNId
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
} /* end of wrUmmSztFillPLMNId */ 


/**
 *  @brief This function fills EUTRAN CGI
 *
 *      Function: wrUmmS1apFillEutranCgi
 *
 *          Processing steps:
 *          - encodes the EUTRAN CGI
 *
 *  @param [in]  ueCb : ueCb of the UE for which E-CGI will be filled
 *  @param [out] pdu    : S1Ap memCP pointer
 *  @param [in]  cgiIe  :  pointer to IE to update the ECGI
 *  @return 
 */
/* FIX for ccpu00135294 */
PRIVATE S16 wrUmmS1apFillEutranCgi
(
WrUeCb                     *ueCb,
S1apPdu                      *pdu,
SztEUTRAN_CGI                *cgiIe
)
{
   WrCgi                     cgi;

   if (wrEmmGetCgiFromUeCb(ueCb, &cgi) != ROK)
   {
      RETVALUE(RFAILED);
   }

   wrFillTknU8(&(cgiIe->pres), PRSNT_NODEF);
   wrUmmSztFillPLMNId(pdu, &cgi.plmnId, &cgiIe->pLMNidentity);
   wrFillTknBStr32(&(cgiIe->cell_ID), 28, cgi.cellId);
   cgiIe->iE_Extns.noComp.pres = NOTPRSNT;

   RETVALUE(ROK);
}

/**
 *  @brief This function encodes the TAI IE
 *
 *      Function: wrUmmS1apFillTAI
 *
 *          Processing steps:
 *          - fill the PLMN Id
 *          - fill the TAC
 *
 *
 *  @param [in]  ueCb : ueCb of the UE for which TAI will be filled
 *  @param [out] pdu    : S1Ap memCp pointer
 *  @param [in]  taiIe  : pointer to TAI IE
 *  @return 
 */

PRIVATE S16 wrUmmS1apFillTAI
(
WrUeCb                     *ueCb,
S1apPdu                      *pdu,
SztTAI                       *taiIe
)
{
   WrTai                     tai;

   if (wrEmmGetTai(ueCb, &tai) != ROK)
   {
      RETVALUE(RFAILED);
   }
   wrFillTknU8(&(taiIe->pres),PRSNT_NODEF);     
   if (wrUmmSztFillPLMNId(pdu, &tai.plmnId, &(taiIe->pLMNidentity)) != ROK)
   {
      RETVALUE(RFAILED);
   }
   wrUmmSztFillTAC(pdu, tai.tac, &(taiIe->tAC));
   RETVALUE(ROK);
} /* end of wrUmmS1apFillTAI */ 

/**
 *  @brief This function fill the cause
 *
 *      Function: wrS1apFillCause
 *
 *          Processing steps:
 *          - switch to the cause Type
 *          - encode the cause value
 *
 *
 *  @param [in] ie    : pointer to cause IE
 *  @param [in] cause : pointer to s1ap cause info
 *  @return S16
    -# Success : ROK
    -# Failure : RFAILED 
 */

PRIVATE S16 wrS1apFillCause
(
SztCause                     *ie,
WrUmmMsgCause                *cause
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
} /* end of wrS1apFillCause */ 

/* ======================================= */
/* S1AP S1 INITIAL UE  MESSAGE BUILDING  */
/* ======================================= */

/**
 *  @brief This function builds the S1AP initial UE message
 *
 *      Function: wrUmmS1apBldInitUePdu
 *
 *          Processing steps:
 *          - Alloc memory to initial message pdu
 *          - fill the mandatory IE's
 *          - update the pointer to initial ue message pdu into out pointer
 *
 *
 *  @param [in] ueCb   : pointer to UE control block
 *  @param [in] nasPdu : pointer to encoded NAS PDU 
 *  @param [in] plmnId : pointer to PLMN ID list
 *  @param [in] tac    : TAC to be sent to UE
 *  @param [in] gummei : globally unique MME Identifier to be sent to UE
 *  @param [in] gummeiType : GUMMEI type
 *  @param [in] pdu    : double pointer to S1AP PDU 
 *  @return S16
    -# Success : ROK
    -# Failure : RFAILED
 */
PUBLIC S16 wrUmmS1apBldInitUePdu
(
WrUeCb                       *ueCb,
TknStrOSXL                   *nasPdu,
WrPlmnId                     *plmnId,
U16                          tac,
WrGummei                     *gummei,
TknU32                       *gummeiType,
S1apPdu                      **pdu
)
{ 
   S1apPdu                   *initUePdu = NULLP;
   SztInitiatingMsg          *initMsg;
   SztInitUEMsg              *initUeMsg;
   U16                       ieIdx = 0;
   SztProtIE_Field_InitUEMsg_IEs *ie;
   WrUmmCellCb               *cellCb = NULLP;
   U16                        numComp = 0;

   RLOG0(L_DEBUG, "Building Initial UE Message");

   if (cmAllocEvnt(sizeof(S1apPdu), SZ_MEM_SDU_SIZE, &wrCb.mem, 
                                           (Ptr *)&initUePdu) != ROK)
   {
      RLOG0(L_FATAL, "Memory allocation failed.");
      RETVALUE(RFAILED);
   }

   /* Setup the message as an initiating message and the procedure as */
   /* INITIAL UE MESSAGE                                              */
   initMsg = &(initUePdu->pdu.val.initiatingMsg);
   wrFillTknU8(&(initUePdu->pdu.choice), SZT_TRGR_INTI_MSG);
   wrFillTknU8(&(initMsg->pres), PRSNT_NODEF);
   wrFillTknU32(&(initMsg->procedureCode), Sztid_initialUEMsg);
   wrFillTknU32(&(initMsg->criticality), SztCriticalityignoreEnum);

   /* List of IEs 
    * 1. UE ENB S1AP ID
    * 2. NAS PDU  
    * 3. TAI 
    * 4. EUTRAN CGI
    * 5. Establishment Cause 
    * 6. S-TMSI
    * 7  Csg Id
    * 8. Registered MME
    * 9. Cell Access Mode
    *10. GUMMEI Type
    * */
   /* Allocate memory for  IEs */
   initUeMsg = &(initMsg->value.u.sztInitUEMsg);
   wrFillTknU8(&(initUeMsg->pres), PRSNT_NODEF);

   WR_UMM_GET_CELLCB(cellCb, ueCb->cellId);
   if (cellCb == NULLP)
   {
      /* Log the error through an error debug */
      RLOG_ARG0(L_ERROR, DBG_CELLID,(U32)ueCb->cellId,
            "Error in getting CELL CB");
      WR_FREE_EVNT(initUePdu);
      RETVALUE(RFAILED);
   }

   /* CSG_DEV start */
   if (cellCb->csgAccessMode == WR_ENB_CLOSED_ACCESS)
   {
      numComp = 8;
   }/* Closed Mode Case */
   else if (cellCb->csgAccessMode == WR_ENB_HYBRID_ACCESS)
   {
      numComp = 9;
   }/* Hybrid Mode Case */
   else
   {
      numComp = 7;
   }/* Open Mode Case */
   /* CSG_DEV end */
   /* GUMMEI Type */
   if(gummeiType->pres != NOTPRSNT)
   {
      numComp ++;
   }

   if((cmGetMem(initUePdu, numComp * sizeof(SztProtIE_Field_InitUEMsg_IEs),
               (Ptr*)&initUeMsg->protocolIEs.member)) !=ROK)
   {
      RLOG0(L_FATAL, "Memory allocation failed.");
      RETVALUE(RFAILED);
   }

   /* IE1 - Filling enb s1ap id */
   ie = &initUeMsg->protocolIEs.member[ieIdx++];
   wrFillTknU8(&(ie->pres), PRSNT_NODEF);     
   wrFillTknU32(&(ie->id), Sztid_eNB_UE_S1AP_ID);
   wrFillTknU32(&(ie->criticality), SztCriticalityrejectEnum);
   wrFillTknU32(&(ie->value.u.sztENB_UE_S1AP_ID), 
                                ueCb->s1ConCb->enb_ue_s1ap_id);

   /* IE2 - Filling nas pdu */
   ie = &initUeMsg->protocolIEs.member[ieIdx++];
   wrFillTknU8(&(ie->pres), PRSNT_NODEF);     
   wrFillTknU32(&(ie->id), Sztid_NAS_PDU);
   wrFillTknU32(&(ie->criticality), SztCriticalityrejectEnum);
   wrFillTknStrOSXL(&(ie->value.u.sztNAS_PDU), nasPdu->len, nasPdu->val,
                                                         &initUePdu->memCp);
   /* IE3 - Filling TAI */
   ie = &initUeMsg->protocolIEs.member[ieIdx++];
   wrFillTknU8(&(ie->pres), PRSNT_NODEF);     
   wrFillTknU32(&(ie->id), Sztid_TAI);
   wrFillTknU32(&(ie->criticality), SztCriticalityrejectEnum);
   wrFillTknU8(&(ie->value.u.sztTAI.pres), PRSNT_NODEF);     
   wrUmmSztFillPLMNId(initUePdu, plmnId, &(ie->value.u.sztTAI.pLMNidentity));
   wrUmmSztFillTAC(initUePdu, tac, &(ie->value.u.sztTAI.tAC));
   
   /*IE4 - Filling CGI */
   ie = &initUeMsg->protocolIEs.member[ieIdx++];
   wrFillTknU8(&(ie->pres), PRSNT_NODEF);
   wrFillTknU32(&(ie->id), Sztid_EUTRAN_CGI);
   wrFillTknU32(&(ie->criticality), SztCriticalityignoreEnum);
   wrFillTknU8(&(ie->value.u.sztEUTRAN_CGI.pres), PRSNT_NODEF);
   wrUmmS1apFillEutranCgi(ueCb, initUePdu,
                                            &(ie->value.u.sztEUTRAN_CGI));

   /* IE5 - Filling RRC Establishment Cause */
   ie = &initUeMsg->protocolIEs.member[ieIdx++];
   wrFillTknU8(&(ie->pres), PRSNT_NODEF);
   wrFillTknU32(&(ie->id), Sztid_RRC_Establishment_Cause);
   wrFillTknU32(&(ie->criticality), SztCriticalityignoreEnum );
   wrFillTknU32(&(ie->value.u.sztRRC_Establishment_Cause), 
                                                    ueCb->establishCause);
   /*IE6 - Filling STMSI */
   if (ueCb->sTMSI.pres == TRUE)
   {
      ie = &initUeMsg->protocolIEs.member[ieIdx++];
      wrFillTknU8(&(ie->pres), PRSNT_NODEF);
      wrFillTknU32(&(ie->id), Sztid_S_TMSI);
      wrFillTknU32(&(ie->criticality), SztCriticalityrejectEnum);

      wrFillTknU8(&(ie->value.u.sztS_TMSI.pres), PRSNT_NODEF);
      wrFill2TknStr4(&(ie->value.u.sztS_TMSI.mMEC), 1, (U8 *)&ueCb->sTMSI.mmec);
      wrFillTknStrOSXL1(&(ie->value.u.sztS_TMSI.m_TMSI),4, ueCb->sTMSI.mTMSI, 
                                                             &initUePdu->memCp);
      ie->value.u.sztS_TMSI.iE_Extns.noComp.pres = NOTPRSNT;
   }

   /* CSG_DEV: For Closed Mode of operation the CSGID has to be included */
   if (cellCb->csgAccessMode != WR_ENB_OPEN_ACCESS)  
   {
       ie = &initUeMsg->protocolIEs.member[ieIdx++];

       wrFillTknU8(&(ie->pres),PRSNT_NODEF);
       wrFillTknU32(&(ie->id),Sztid_CSG_Id);
       wrFillTknU32(&(ie->criticality),SztCriticalityrejectEnum);
       wrFillTknBStr32(&(ie->value.u.sztCSG_Id), WR_S1AP_CSG_ID_LEN,
             cellCb->csgId);
    }/* CSG_DEV End*/
   
   /*IE7 - Filling GUMMEI */
   if(gummei->pres == TRUE)
   {
      SztGUMMEI              *gummeiIe;

      ie = &initUeMsg->protocolIEs.member[ieIdx++];
      gummeiIe = &(ie->value.u.sztGUMMEI);
      wrFillTknU8(&(ie->pres), PRSNT_NODEF);
      wrFillTknU32(&(ie->id), Sztid_GUMMEI_ID);
      wrFillTknU32(&(ie->criticality), SztCriticalityrejectEnum);
      wrFillTknU8(&(gummeiIe->pres), PRSNT_NODEF);
      wrFill2TknStr4(&(gummeiIe->mME_Group_ID), 2, (U8*)&gummei->mmeGrpId);
      wrFill2TknStr4(&(gummeiIe->mME_Code), 1, (U8*)&gummei->mmec);
      wrUmmSztFillPLMNId(initUePdu, &(gummei->plmnId), 
                                               &(gummeiIe->pLMN_Identity));
      ie->value.u.sztGUMMEI.iE_Extns.noComp.pres = NOTPRSNT;
   }
  
   /* CSG_DEV Start:Added the Cell Access Mode IE. 
    */
   if ((cellCb->csgAccessMode == WR_ENB_HYBRID_ACCESS))  
   {
       ie = &initUeMsg->protocolIEs.member[ieIdx++];

       wrFillTknU8(&(ie->pres), PRSNT_NODEF);
       wrFillTknU32(&(ie->id), Sztid_CellAccessMode);
       wrFillTknU32(&(ie->criticality), SztCriticalityrejectEnum);
       wrFillTknU32(&(ie->value.u.sztCellAccessMode), 
             SztCellAccessModehybridEnum);
   }/*CSG_DEV End*/

   /* Fill GUMMEI Type */
   if(gummeiType->pres != NOTPRSNT)
   {
      ie = &initUeMsg->protocolIEs.member[ieIdx++];

      wrFillTknU8(&(ie->pres), PRSNT_NODEF);
      wrFillTknU32(&(ie->id), Sztid_GUMMEITyp);
      wrFillTknU32(&(ie->criticality), SztCriticalityignoreEnum);
      wrFillTknU32(&(ie->value.u.sztGUMMEITyp), 
            gummeiType->val);
   }

   /* Set the number of IEs filled into the INIT UE message */
   wrFillTknU16(&(initUeMsg->protocolIEs.noComp), ieIdx);

   *pdu = initUePdu;

   RETVALUE(ROK);
}

/* ================================================ */
/* S1AP S1 INIT CNTXT SETUP RSP MESSAGE BUILDING  */
/* ================================================ */

/* CODE PRESENT IN init context .c */

/* ==================================================== */
/* S1AP UE CAPABILITY INFO INDICATION MESSAGE BUILDING  */
/* ==================================================== */


/**
 *  @brief This function fills the UE capability info 
 *
 *      Function: wrUmmS1apFillUeCapInfo
 *
 *          Processing steps:
 *          - Alloc memory to UE Capability Info Indication pdu
 *          - fill the mandatory IE's
 *          - update the pointer to capability info pdu into out pointer
 *
 *
 *  @param [in]  s1apCon : pointer  to S1AP connection CB 
 *  @param [in]  octnStr : pointer to encode data
 *  @param [out] pdu     : double pointer to S1AP Pdu
 *  @return S16
    -# Success : ROK
    -# Failure : RFAILED
 */

PUBLIC S16 wrUmmS1apFillUeCapInfo
(
WrS1ConCb                    *s1apCon,
TknStrOSXL                   *octnStr,
S1apPdu                      **pdu
)
{
   S1apPdu                   *szUeCapInfoIndPdu = NULLP;
   U16                       numComp;
   SztProtIE_Field_UECapbltyInfoIndIEs  *ie = NULLP;
   U16                       crntIe = 0;

   RLOG0(L_DEBUG, "Constructing UE Capability Info Message");

   if (cmAllocEvnt(sizeof(S1apPdu), SZ_MEM_SDU_SIZE, &wrCb.mem,
                                     (Ptr *)&szUeCapInfoIndPdu) != ROK)
   {
      RLOG0(L_FATAL, "Memory allocation failed.");
      RETVALUE(RFAILED);
   }

   wrFillTknU8(&(szUeCapInfoIndPdu->pdu.choice), SZT_TRGR_INTI_MSG);
   wrFillTknU8(&(szUeCapInfoIndPdu->pdu.val.initiatingMsg.pres), PRSNT_NODEF);
   wrFillTknU32(&(szUeCapInfoIndPdu->pdu.val.initiatingMsg.procedureCode),
              Sztid_UECapbltyInfoInd);
   wrFillTknU32(&(szUeCapInfoIndPdu->pdu.val.initiatingMsg.criticality),
              SztCriticalityignoreEnum);

   /* Filling 3 mandatory headers */
   wrFillTknU8(&(szUeCapInfoIndPdu->pdu.val.initiatingMsg.value.u.\
              sztUECapbltyInfoInd.pres),PRSNT_NODEF);
   numComp = 3;
   wrFillTknU16(&(szUeCapInfoIndPdu->pdu.val.initiatingMsg.value.u.\
              sztUECapbltyInfoInd.protocolIEs.noComp), numComp);
   /*List of IEs Filled
    * 1.UE MME S1AP ID
    * 2.UE ENB S1AP ID
    * 3.UE Radio Capability*/
   if ((cmGetMem(szUeCapInfoIndPdu,\
              (numComp * sizeof(SztProtIE_Field_UECapbltyInfoIndIEs)),\
              (Ptr *)&szUeCapInfoIndPdu->pdu.val.initiatingMsg.value.u.\
              sztUECapbltyInfoInd.protocolIEs.member)) !=ROK)
   {
      RLOG0(L_FATAL, "Memory allocation failed.");
      RETVALUE(RFAILED);
   }

   /* IE1 - Filling MME ID */
   ie = &szUeCapInfoIndPdu->pdu.val.initiatingMsg.value.u.\
        sztUECapbltyInfoInd.protocolIEs.member[crntIe++];

   wrFillTknU8(&(ie->pres), PRSNT_NODEF);   
   wrFillTknU32(&(ie->id), Sztid_MME_UE_S1AP_ID);   
   wrFillTknU32(&(ie->criticality), SztCriticalityrejectEnum);   
   wrFillTknU32(&(ie->value.u.sztMME_UE_S1AP_ID), s1apCon->mme_ue_s1ap_id);  

   /* IE2 - Filling ENB ID */
   ie = &szUeCapInfoIndPdu->pdu.val.initiatingMsg.value.u.\
        sztUECapbltyInfoInd.protocolIEs.member[crntIe++];    

   wrFillTknU8(&(ie->pres), PRSNT_NODEF);  
   wrFillTknU32(&(ie->id), Sztid_eNB_UE_S1AP_ID);   
   wrFillTknU32(&(ie->criticality), SztCriticalityrejectEnum); 
   wrFillTknU32(&(ie->value.u.sztENB_UE_S1AP_ID), s1apCon->enb_ue_s1ap_id);

   /* IE3 - Filling Radio cap */
   ie = &szUeCapInfoIndPdu->pdu.val.initiatingMsg.value.u.\
        sztUECapbltyInfoInd.protocolIEs.member[crntIe++];    

   wrFillTknU8(&(ie->pres), PRSNT_NODEF);  
   wrFillTknU32(&(ie->id), Sztid_UERadioCapblty);   
   wrFillTknU32(&(ie->criticality), SztCriticalityignoreEnum);

   wrFillTknStrOSXL(&(ie->value.u.sztUERadioCapblty), octnStr->len,
                                      octnStr->val, &szUeCapInfoIndPdu->memCp);

   *pdu = szUeCapInfoIndPdu; 

   RETVALUE(ROK);
} /* wrUmmS1apFillUeCapInfo */


/* ================================================ */
/* S1AP ERAB RELEASE  RESPONSE MESSAGE BUILDING  */
/* ================================================ */

/**
 *  @brief This function  fill E-RAB release response
 *
 *      Function: wrUmmS1apBldERABRelRsp
 *
 *          Processing steps:
 *          - Alloc memory to E-RAB release response message pdu
 *          - fill the mandatory IE's
 *          - update the pointer to release responce pdu into out pointer
 *
 *
 *  @param [in]  s1apCon : pointer to S1AP connection CB  
 *  @param [out] pdu     : double pointer to S1Ap Pdu
 *  @return S16
    -# Success : ROK
    -# Failure : RFAILED
 */
PUBLIC S16 wrUmmS1apBldERABRelRsp
(
WrS1ConCb                    *s1apCon, 
S1apPdu                      **pdu,
U8                           numrab,
WrUmmRabRlsLst               *rabLst 
)
{

   S1apPdu                   *szERABRspPdu = NULLP;
   U16                       numComp;
   SztProtIE_Field_E_RABRlsRespIEs *ie = NULLP;
   SztProtIE_Field_E_RABRlsRespIEs *succIe = NULLP;
   SztProtIE_Field_E_RABRlsRespIEs *failIe = NULLP;
   SztSuccessfulOutcome      *succMsg;
   U16                       ieIdx = 0;
   SztE_RABRlsResp           *erabRls;
   SztProtIE_SingleCont_E_RABRlsItemBrRelCompIEs *protie;
   SztProtIE_SingleCont_E_RABItemIEs             *proties;
   U8                        rabIdx  = 0;
   U8                        rabFailed = 0;
   U8                        rabSucc = 0;
   U8                        failIdx = 0;
   U8                        succIdx = 0;

   RLOG0(L_DEBUG, "Building E-RAB Release Response Message");

   if (cmAllocEvnt(sizeof(S1apPdu), SZ_MEM_SDU_SIZE, &wrCb.mem,
         (Ptr *)&szERABRspPdu))
   {
      RLOG0(L_FATAL, "Memory allocation failed.");
      RETVALUE(RFAILED);
   }

   wrFillTknU8(&(szERABRspPdu->pdu.choice), SZT_TRGR_SUCCESS_OUTCOME);
   succMsg = &(szERABRspPdu->pdu.val.successfulOutcome);
   wrFillTknU8(&(succMsg->pres), PRSNT_NODEF);
   wrFillTknU32(&(succMsg->procedureCode), Sztid_E_RABRls);
   wrFillTknU32(&(succMsg->criticality), SztCriticalityrejectEnum);

   /* Filling 2 mandatory headers */
   /*List of IEs Filled
    * 1.UE ENB S1AP ID
    * 2.UE MME S1AP ID
    * 3.ERAB Release Lst
    */

   erabRls = &(succMsg->value.u.sztE_RABRlsResp);
   /* TODO Allocate the Proper Memory for the IEs present Only */
     for (rabIdx = 0;rabIdx < numrab;rabIdx++)
     {
        if((rabLst[rabIdx]).rabFailed)
        {
           rabFailed++;
        }
        else
        {
           /*Increment KPI for Number of E-RABs successfully released*/
           WR_INC_KPI_FUNC1(wrIncKpiFrERABRelSuccPrQci,rabLst[rabIdx].qci);
           rabSucc++;
        }
     }   
     if(rabFailed)
        numComp = 4;
     else
        numComp = 3;
   wrFillTknU8(&(erabRls->pres), PRSNT_NODEF);
   if ((cmGetMem(szERABRspPdu,
                 (numComp * sizeof(SztProtIE_Field_E_RABRlsRespIEs)),
                 (Ptr *)&erabRls->protocolIEs.member)) !=ROK)
   {
      RLOG0(L_FATAL, "Memory allocation failed.");
      RETVALUE(RFAILED);
   }

   /* Filling MME ID */
   ie = &erabRls->protocolIEs.member[ieIdx];
   wrFillTknU8(&(ie->pres), PRSNT_NODEF);   
   wrFillTknU32(&(ie->id), Sztid_MME_UE_S1AP_ID);   
   wrFillTknU32(&(ie->criticality), SztCriticalityignoreEnum);   
   wrFillTknU32(&(ie->value.u.sztMME_UE_S1AP_ID), s1apCon->mme_ue_s1ap_id);  
   ieIdx++;

   /* Filling ENB ID */
   ie = &erabRls->protocolIEs.member[ieIdx];
   wrFillTknU8(&(ie->pres), PRSNT_NODEF);  
   wrFillTknU32(&(ie->id), Sztid_eNB_UE_S1AP_ID);   
   wrFillTknU32(&(ie->criticality), SztCriticalityignoreEnum); 
   wrFillTknU32(&(ie->value.u.sztENB_UE_S1AP_ID), 
                              s1apCon->enb_ue_s1ap_id);                        
   ieIdx++;

     /* Filling ERAB ID List */
     if(rabSucc > 0)
     {
        succIe = &erabRls->protocolIEs.member[ieIdx];
        wrFillTknU8(&(succIe->pres), PRSNT_NODEF);
        wrFillTknU32(&(succIe->id), Sztid_E_RABRlsLstBrRelComp);
        wrFillTknU32(&(succIe->criticality), SztCriticalityignoreEnum);
        wrFillTknU16(&(succIe->value.u.sztE_RABRlsLstBrRelComp.noComp), rabSucc);
        if (cmGetMem(szERABRspPdu,
                 (rabSucc * sizeof(SztProtIE_SingleCont_E_RABRlsItemBrRelCompIEs)),
                 (Ptr *)&(succIe->value.u.sztE_RABRlsLstBrRelComp.member)) != ROK)
        {
           RLOG0(L_FATAL, "Memory allocation failed.");
           RETVALUE(RFAILED);
        }
       ieIdx++;
     }
     if(rabFailed > 0)
     {
       /* Filling ERAB ID List */
       failIe = &erabRls->protocolIEs.member[ieIdx];
       wrFillTknU8(&(failIe->pres), PRSNT_NODEF);
       wrFillTknU32(&(failIe->id), Sztid_E_RABFailedToRlsLst);
       wrFillTknU32(&(failIe->criticality), SztCriticalityignoreEnum);
       wrFillTknU16(&(failIe->value.u.sztE_RABLst.noComp), rabFailed);

       if (cmGetMem(szERABRspPdu,
              (rabFailed * sizeof(SztProtIE_SingleCont_E_RABItemIEs)),
              (Ptr *)&(failIe->value.u.sztE_RABLst.member)) != ROK)
       {
          RLOG0(L_FATAL, "Memory allocation failed.");
          RETVALUE(RFAILED);
       }
       ieIdx++;
     }
     for (rabIdx = 0;rabIdx < numrab;rabIdx++)
     {
        if(!(rabLst[rabIdx]).rabFailed)
        {
            
           if( NULL == succIe) 
           {
               RLOG0(L_ERROR, "Unable to get succIe");
               RETVALUE(RFAILED);
           }
           protie = &succIe->value.u.sztE_RABRlsLstBrRelComp.member[succIdx++];
           wrFillTknU8(&(protie->pres), PRSNT_NODEF);
           wrFillTknU32(&(protie->id), Sztid_E_RABRlsItemBrRelComp);
           wrFillTknU32(&(protie->criticality), SztCriticalityignoreEnum);
           protie->value.u.sztE_RABRlsItemBrRelComp.pres.pres = PRSNT_NODEF;
           wrFillTknU32(&(protie->value.u.sztE_RABRlsItemBrRelComp.e_RAB_ID),
                 (rabLst[rabIdx]).erabId);
           /*Filling of IE Extensions*/
           protie->value.u.sztE_RABRlsItemBrRelComp.iE_Extns.noComp.pres = NOTPRSNT;
        }
        else
        {
            
            if( NULL == failIe ) 
            {
                RLOG0(L_ERROR, "Unable to get failIe");
                RETVALUE(RFAILED);
            }
           proties = &failIe->value.u.sztE_RABLst.member[failIdx++];
           wrFillTknU8(&(proties->pres), PRSNT_NODEF);
           wrFillTknU32(&(proties->id), Sztid_E_RABItem);
           wrFillTknU32(&(proties->criticality), SztCriticalityignoreEnum);
           proties->value.u.sztE_RABItem.pres.pres = PRSNT_NODEF;
           wrFillTknU32(&(proties->value.u.sztE_RABItem.e_RAB_ID),
                 (rabLst[rabIdx]).erabId);
            /*Error_Cause: Fill the proper cause value here*/
           RLOG3(L_DEBUG, "E-Rab ID[%d]\t Cause Type[%ld]\t Cause Value[%ld]",
              (rabLst[rabIdx]).erabId, (rabLst[rabIdx]).cause.causeTyp,
              (rabLst[rabIdx]).cause.causeVal);
           wrS1apFillCause(&(proties->value.u.sztE_RABItem.cause),
                 &((rabLst[rabIdx]).cause));
           /*Filling of IE Extensions*/
           proties->value.u.sztE_RABItem.iE_Extns.noComp.pres = NOTPRSNT;
      
           /*Increment KPI for Number of E-RABs failed to release*/
           WR_INC_KPI_FUNC1(wrIncKpiFrERABRelCmdPrCause,
                             &(rabLst[rabIdx].cause));
           WR_INC_KPI_FUNC3(wrIncKpiFrS1apFailCause, rabLst[rabIdx].cause.causeTyp, 
                       rabLst[rabIdx].cause.causeVal, INC_KPI_VALUE_BY_ONE);
        }
     }

   /* Set the number of protocol IEs here                                 */
   wrFillTknU16(&(erabRls->protocolIEs.noComp), ieIdx);

   /*TODO Failed list*/
   *pdu = szERABRspPdu; 

   RETVALUE(ROK);
}


/* ================================================ */
/* S1AP ERAB MODIFY RESPONSE MESSAGE BUILDING       */
/* ================================================ */

/**
 *  @brief This function fills the E-RAB Modify Response
 *
 *      Function: wrUmmS1apBldERABModRsp
 *
 *          Processing steps:
 *          - Alloc memory to E-RAB Modify response message pdu
 *          - fill the mandatory IE's
 *          - update the pointer to modify responce pdu into out pointer
 *                
 *                
 *  @param [in]   s1apCon  : pointer to S1AP connection CB
 *  @param [out]  pdu      : double pointer to return the message
 *  @param [in]   numRabs  : number of radio bearer in modify response message
 *  @param [in]   rabLst   : pointer to bearer list
 *  @return       
 */               
PUBLIC S16 wrUmmS1apBldERABModRsp
(
WrS1ConCb                    *s1apCon,
S1apPdu                      **pdu,
U8                           numRabs,
WrUmmRabRlsLst               *rabLst 
)
{
   S1apPdu                   *szERABModRspPdu = NULLP;
   U16                       numComp;
   SztProtIE_Field_E_RABMdfyRespIEs *ie = NULLP;
   SztProtIE_Field_E_RABMdfyRespIEs *succIe = NULLP;
   SztProtIE_Field_E_RABMdfyRespIEs *failIe = NULLP;
   SztSuccessfulOutcome      *succMsg;
   U32                       ieIdx = 0;
   SztE_RABMdfyResp          *erabMod;
   U8                        rabIdx;
   U8                        rabFailed = 0; 
   U8                        rabSucc = 0;
   U8                        failIdx = 0;
   U8                        succIdx = 0;
   SztProtIE_SingleCont_E_RABItemIEs             *proties;
  SztProtIE_SingleCont_E_RABMdfyItemBrModResIEs *protIe;

   RLOG0(L_DEBUG, "Building E-RAB Modify Response Message");

   if (cmAllocEvnt(sizeof(S1apPdu), SZ_MEM_SDU_SIZE, &wrCb.mem,
                                       (Ptr *)&szERABModRspPdu) != ROK)
   {
      RLOG0(L_FATAL, "Memory allocation failed.");
      RETVALUE(RFAILED);
   }

   wrFillTknU8(&(szERABModRspPdu->pdu.choice), SZT_TRGR_SUCCESS_OUTCOME);
   succMsg = &szERABModRspPdu->pdu.val.successfulOutcome;
   wrFillTknU8(&(succMsg->pres), PRSNT_NODEF);
   wrFillTknU32(&(succMsg->procedureCode), Sztid_E_RABMdfy);
   wrFillTknU32(&(succMsg->criticality), SztCriticalityrejectEnum);

   /* Filling 2 mandatory headers */
   /*List of IEs Filled
    * 1.UE ENB S1AP ID
    * 2.UE MME S1AP ID
    * 3.ERAB Modify List
    * */
   erabMod = &succMsg->value.u.sztE_RABMdfyResp;
   /*TODO Check the Modify Status for Number of COMPONENT */
   for (rabIdx = 0;rabIdx < numRabs;rabIdx++)
   {
      if((rabLst[rabIdx]).rabFailed)
      {
         rabFailed++;
      }
      else
      {
        /*Increment KPI for E-RAB modification:Number of E-RABs successfully modified the QoS parameter*/
         WR_INC_KPI_FUNC1(wrIncKpiFrERABMdQoSSuccNbrPrQci,rabLst[rabIdx].qci);
         rabSucc++;
      }
   } 

   /* For mandatory IEs*/
   numComp = 2; 

   if(rabFailed)
   {
      numComp++;
   }

   if(rabSucc)
   {
      numComp++;
   }

   wrFillTknU8(&(erabMod->pres), PRSNT_NODEF);
   if ((cmGetMem(szERABModRspPdu,
                 (numComp * sizeof(SztProtIE_Field_E_RABMdfyRespIEs)),
                 (Ptr *)&erabMod->protocolIEs.member)) != ROK)
   {
      RLOG0(L_FATAL, "Memory allocation failed.");
      RETVALUE(RFAILED);
   }

   /* Filling MME ID */
   ie = &erabMod->protocolIEs.member[ieIdx];
   wrFillTknU8(&(ie->pres), PRSNT_NODEF);   
   wrFillTknU32(&(ie->id), Sztid_MME_UE_S1AP_ID);   
   wrFillTknU32(&(ie->criticality), SztCriticalityignoreEnum);   
   wrFillTknU32(&(ie->value.u.sztMME_UE_S1AP_ID), s1apCon->mme_ue_s1ap_id);  
   ieIdx++;

   /* Filling ENB ID */
   ie = &erabMod->protocolIEs.member[ieIdx];
   wrFillTknU8(&(ie->pres), PRSNT_NODEF);  
   wrFillTknU32(&(ie->id), Sztid_eNB_UE_S1AP_ID);   
   wrFillTknU32(&(ie->criticality), SztCriticalityignoreEnum); 
   wrFillTknU32(&(ie->value.u.sztENB_UE_S1AP_ID), s1apCon->enb_ue_s1ap_id);                        
   ieIdx++;
   if(rabSucc > 0)
   {
      /* ERAB Modify List*/
      succIe = &erabMod->protocolIEs.member[ieIdx];
      wrFillTknU8(&(succIe->pres), PRSNT_NODEF);
      wrFillTknU32(&(succIe->id), Sztid_E_RABMdfyLstBrModRes);
      wrFillTknU32(&(succIe->criticality), SztCriticalityignoreEnum);

      wrFillTknU16(&(succIe->value.u.sztE_RABMdfyLstBrModRes.noComp),rabSucc); 

      if (cmGetMem(szERABModRspPdu,
               (rabSucc * sizeof(SztProtIE_SingleCont_E_RABMdfyItemBrModResIEs)),
               (Ptr *)&(succIe->value.u.sztE_RABMdfyLstBrModRes.member)) !=ROK)
      {
         RLOG0(L_FATAL, "Memory allocation failed.");
         RETVALUE(RFAILED);
      } 
      ieIdx++;
   }
   if(rabFailed > 0)
   {
      /* ERAB Modify Falied List*/
      failIe = &erabMod->protocolIEs.member[ieIdx];
      wrFillTknU8(&(failIe->pres), PRSNT_NODEF);
      wrFillTknU32(&(failIe->id), Sztid_E_RABFailedToMdfyLst);
      wrFillTknU32(&(failIe->criticality), SztCriticalityignoreEnum);
      wrFillTknU16(&(failIe->value.u.sztE_RABLst.noComp),rabFailed); 

      if (cmGetMem(szERABModRspPdu,
               (rabFailed * sizeof(SztProtIE_SingleCont_E_RABItemIEs)),
               (Ptr *)&(failIe->value.u.sztE_RABLst.member)) !=ROK)
      {
         RLOG0(L_FATAL, "Memory allocation failed.");
         RETVALUE(RFAILED);
      } 
      ieIdx++;
   }


   for (rabIdx = 0;rabIdx < numRabs;rabIdx++)
   {
      if(!(rabLst[rabIdx]).rabFailed)
      {
          
         if( NULL == succIe) 
         {
            RLOG0(L_ERROR, "Unable to get succIe");
            RETVALUE(RFAILED);
         }
         protIe = &succIe->value.u.sztE_RABMdfyLstBrModRes.member[succIdx++];
         wrFillTknU8(&(protIe->pres), PRSNT_NODEF);  
         wrFillTknU32(&(protIe->id), Sztid_E_RABMdfyItemBrModRes);   
         wrFillTknU32(&(protIe->criticality), SztCriticalityrejectEnum); 
         wrFillTknU8(&(protIe->value.u.sztE_RABMdfyItemBrModRes.pres),
               PRSNT_NODEF);   
         wrFillTknU32(&(protIe->value.u.sztE_RABMdfyItemBrModRes.e_RAB_ID),
               (rabLst[rabIdx]).erabId 
              );   
         /* IE Extensions has been disabled*/
         protIe->value.u.sztE_RABMdfyItemBrModRes.iE_Extns.noComp.pres = NOTPRSNT;
      }
      else
      {

         if( NULL == failIe ) 
         {
            RLOG0(L_ERROR, "Unable to get failIe");
            RETVALUE(RFAILED);
         }
         proties = &failIe->value.u.sztE_RABLst.member[failIdx++];
         wrFillTknU8(&(proties->pres), PRSNT_NODEF);
         wrFillTknU32(&(proties->id), Sztid_E_RABItem);
         wrFillTknU32(&(proties->criticality), SztCriticalityignoreEnum);
         proties->value.u.sztE_RABItem.pres.pres = PRSNT_NODEF;
         wrFillTknU32(&(proties->value.u.sztE_RABItem.e_RAB_ID),
               (rabLst[rabIdx]).erabId);
         /*Error_Cause: Fill the proper cause value here*/
         RLOG3(L_DEBUG, "E-Rab ID[%d]\t Cause Type[%ld]\t Cause Value[%ld]",
            rabLst[rabIdx].erabId, rabLst[rabIdx].cause.causeTyp,
            rabLst[rabIdx].cause.causeVal);
         wrS1apFillCause(&(proties->value.u.sztE_RABItem.cause),
               &((rabLst[rabIdx]).cause));
         /*Filling of IE Extensions*/
         proties->value.u.sztE_RABItem.iE_Extns.noComp.pres = NOTPRSNT;
      
         /*Increment KPI for E-RAB modification Number of E-RABs failed to modify the QoS parameter*/
         WR_INC_KPI_FUNC1(wrIncKpiFrERABModFailPrCause,&(rabLst[rabIdx].cause));
        WR_INC_KPI_FUNC3(wrIncKpiFrS1apFailCause, rabLst[rabIdx].cause.causeTyp, 
                       rabLst[rabIdx].cause.causeVal, INC_KPI_VALUE_BY_ONE);
      }
   } 

   /* Set the number of protocol IEs here                                 */
   wrFillTknU16(&(erabMod->protocolIEs.noComp), ieIdx);
   *pdu = szERABModRspPdu; 

   RETVALUE(ROK);
}

/* ================================================ */
/* S1AP ERAB RELEASE  INDICATON MESSAGE BUILDING  */
/* ================================================ */

/**
 *  @brief This function fill E-RAB release Indication 
 *
 *      Function: wrUmmS1apBldERABRelInd
 *
 *          Processing steps:
 *          - Alloc memory to E-RAB release indication message
 *          - fill the mandatory IE's
 *          - update the pointer to release indication pdu into out pointer
 *
 *
 *  @param [in]  s1apCon : pointer to connection CB
 *  @param [out] pdu     : double pointer to reutrn the message
 *  @param [in]  numRabs : number of bearer in the message
 *  @param [in]  rabLst  : pointer to bearer list
 *  @return S16
    -# Success : ROK
    -# Failure : RFAILED 
 */
PUBLIC S16 wrUmmS1apBldERABRelInd
(
WrS1ConCb                    *s1apCon, 
S1apPdu                      **pdu,
U8                           numRabs,
WrUmmRabRlsLst               *rabLst 
)
{
   S1apPdu                   *szERABRelIndPdu = NULLP;
   U16                       numComp;
   SztInitiatingMsg          *initMsg;
   U32                       ieIdx = 0;
   SztE_RABRlsInd            *erabRlsInd; 
   SztProtIE_SingleCont_E_RABItemIEs   *protie;
   SztProtIE_Field_E_RABRlsIndIEs      *ie;
   U8                        rabIdx = 0;

   RLOG0(L_DEBUG, "Building E-RAB Release Indication Message");

   if (cmAllocEvnt(sizeof(S1apPdu), SZ_MEM_SDU_SIZE, &wrCb.mem,
                                       (Ptr *)&szERABRelIndPdu) != ROK)
   {
      RLOG0(L_FATAL, "Memory allocation failed.");
      RETVALUE(RFAILED);
   }

   wrFillTknU8(&(szERABRelIndPdu->pdu.choice), SZT_TRGR_INTI_MSG);
   initMsg = &(szERABRelIndPdu->pdu.val.initiatingMsg);
   wrFillTknU8(&(initMsg->pres), PRSNT_NODEF);
   wrFillTknU32(&(initMsg->procedureCode), Sztid_E_RABRlsInd);
   wrFillTknU32(&(initMsg->criticality), SztCriticalityignoreEnum);

   /* Filling 2 mandatory headers */
   /*List of IEs Filled
    * 1.UE ENB S1AP ID
    * 2.UE MME S1AP ID
    * 3.ERAB Lst to be released
    * */
   erabRlsInd = &(initMsg->value.u.sztE_RABRlsInd); 
   numComp = 3;
   wrFillTknU8(&(erabRlsInd->pres), PRSNT_NODEF);
   if ((cmGetMem(szERABRelIndPdu,
                 (numComp * sizeof(SztProtIE_Field_E_RABRlsIndIEs)),
                 (Ptr *)&erabRlsInd->protocolIEs.member)) !=ROK)
   {
      RLOG0(L_FATAL, "Memory allocation failed.");
      RETVALUE(RFAILED);
   }

   /* Filling MME ID */
   ie = &erabRlsInd->protocolIEs.member[ieIdx];
   wrFillTknU8(&(ie->pres), PRSNT_NODEF);   
   wrFillTknU32(&(ie->id), Sztid_MME_UE_S1AP_ID);   
   wrFillTknU32(&(ie->criticality), SztCriticalityrejectEnum);   
   wrFillTknU32(&(ie->value.u.sztMME_UE_S1AP_ID), s1apCon->mme_ue_s1ap_id);  
   ieIdx++;

   /* Filling ENB ID */
   ie = &erabRlsInd->protocolIEs.member[ieIdx];
   wrFillTknU8(&(ie->pres), PRSNT_NODEF);  
   wrFillTknU32(&(ie->id), Sztid_eNB_UE_S1AP_ID);   
   wrFillTknU32(&(ie->criticality), SztCriticalityrejectEnum); 
   wrFillTknU32(&(ie->value.u.sztENB_UE_S1AP_ID), s1apCon->enb_ue_s1ap_id);                        
   ieIdx++;

   /* Filling ERAB ID List */
   ie = &erabRlsInd->protocolIEs.member[ieIdx];
   wrFillTknU8(&(ie->pres), PRSNT_NODEF);
   wrFillTknU32(&(ie->id), Sztid_E_RABRlsdLst);
   wrFillTknU32(&(ie->criticality), SztCriticalityignoreEnum);
   wrFillTknU16(&(ie->value.u.sztE_RABLst.noComp), numRabs); 

   if (cmGetMem(szERABRelIndPdu,
                (numRabs * sizeof(SztProtIE_SingleCont_E_RABItemIEs)),
                (Ptr *)&(ie->value.u.sztE_RABLst.member)) != ROK)
   {
      RLOG0(L_FATAL, "Memory allocation failed.");
      RETVALUE(RFAILED);
   } 

   for (rabIdx=0;rabIdx < numRabs;rabIdx++)
   {   
      protie = &ie->value.u.sztE_RABLst.member[rabIdx];
      protie->pres.pres = PRSNT_NODEF;  
      wrFillTknU32(&(protie->id), Sztid_E_RABItem);   
      wrFillTknU32(&(protie->criticality), SztCriticalityrejectEnum); 
      protie->value.u.sztE_RABItem.pres.pres = PRSNT_NODEF;
      wrFillTknU32(&(protie->value.u.sztE_RABItem.e_RAB_ID), 
                                              (rabLst[rabIdx]).erabId);   
      wrS1apFillCause(&(protie->value.u.sztE_RABItem.cause),
                                              &((rabLst[rabIdx]).cause));
      /*Filling of IE Extensions*/
      protie->value.u.sztE_RABItem.iE_Extns.noComp.pres = NOTPRSNT;
      
        /*Increment Erab req to release pr Qci*/
      WR_INC_KPI_FUNC1(wrIncKpiFrERABReqToRelPrQci,rabLst[rabIdx].qci);
      /*Increment KPI for Number of E-RABs requested to release initiated by HeNB per cause*/
      WR_INC_KPI_FUNC2(wrIncKpiFrERABReqToRelPrCause,&((rabLst[rabIdx]).cause),1);
   }
   ieIdx++;

   /* Set the number of protocol IEs here                                 */
   wrFillTknU16(&(erabRlsInd->protocolIEs.noComp), ieIdx);
   
   *pdu = szERABRelIndPdu; 
   RETVALUE(ROK);
}

/* ================================================ */
/* S1AP UE CONTEXT MODIFICATION MESSAGE BUILDING    */
/* ================================================ */
/*
*       Fun:   wrUmmSztUeCntxtModRsp
*
*       Desc:  S1AP Ue Context Modification Response
*
*       Ret:   ROK
*
*       Notes: None
*/
PUBLIC S16 wrUmmS1apUeCntxtModRsp
(
WrS1ConCb                    *s1apCon,
S1apPdu                      **ctxtModPdu
)
{
   S1apPdu                                     *pdu;
   SztSuccessfulOutcome                        *succMsg;
   SztUECntxtModificationResp                  *ctxtModRsp;
   SztProtIE_Field_UECntxtModificationRespIEs  *ie;
   U16                                         numComp;

   RLOG0(L_DEBUG, "Building Context Modification Response");

   if (cmAllocEvnt(sizeof(S1apPdu), SZ_MEM_SDU_SIZE, &wrCb.mem, 
                                                       (Ptr *)&pdu) != ROK)
   {
      RLOG0(L_FATAL, "Memory allocation failed.");
      RETVALUE(RFAILED);
   }

   wrFillTknU8(&(pdu->pdu.choice), SZT_TRGR_SUCCESS_OUTCOME);
   succMsg = &(pdu->pdu.val.successfulOutcome);
   wrFillTknU8(&(succMsg->pres), PRSNT_NODEF);
   wrFillTknU32(&(succMsg->procedureCode), Sztid_UECntxtModification);
   wrFillTknU32(&(succMsg->criticality), SztCriticalityrejectEnum);

   /*List of IEs Filled
    * 1.UE ENB S1AP ID
    * 2.UE MME S1AP ID
    * */
   ctxtModRsp = &(succMsg->value.u.sztUECntxtModificationResp); 
   numComp = 2;
   wrFillTknU8(&(ctxtModRsp->pres), PRSNT_NODEF);
   wrFillTknU16(&(ctxtModRsp->protocolIEs.noComp), numComp);
   if ((cmGetMem(pdu,
                 (numComp * sizeof(SztProtIE_Field_UECntxtModificationRespIEs)),
                 (Ptr *)&ctxtModRsp->protocolIEs.member)) != ROK)
   {
      RLOG0(L_FATAL, "Memory allocation failed.");
      cmFreeMem(pdu);
      RETVALUE(RFAILED);
   }

   /* Filling MME ID */
   ie = &ctxtModRsp->protocolIEs.member[0];
   wrFillTknU8(&(ie->pres), PRSNT_NODEF);   
   wrFillTknU32(&(ie->id), Sztid_MME_UE_S1AP_ID);   
   wrFillTknU32(&(ie->criticality), SztCriticalityignoreEnum);   
   wrFillTknU32(&(ie->value.u.sztMME_UE_S1AP_ID), s1apCon->mme_ue_s1ap_id);  

   /* Filling ENB ID */
   ie = &ctxtModRsp->protocolIEs.member[1];
   wrFillTknU8(&(ie->pres), PRSNT_NODEF);  
   wrFillTknU32(&(ie->id), Sztid_eNB_UE_S1AP_ID);   
   wrFillTknU32(&(ie->criticality), SztCriticalityignoreEnum); 
   wrFillTknU32(&(ie->value.u.sztENB_UE_S1AP_ID), s1apCon->enb_ue_s1ap_id);

   *ctxtModPdu = pdu;
   RETVALUE(ROK);
}

/*
*       Fun:   wrUmmSztUeCntxtModFailure
*
*       Desc:  S1AP Ue Context Modification failure
*
*       Ret:   ROK
*
*       Notes: None
*/
PUBLIC S16 wrUmmS1apUeCntxtModFailure
(
WrS1ConCb                    *s1apCon,
S1apPdu                      **ctxtModPdu,
WrUmmMsgCause                *cause
)
{
   S1apPdu                                       *pdu;
   SztUnsuccessfulOutcome                        *failMsg;
   SztUECntxtModificationFail                    *ctxtModRsp;
   SztProtIE_Field_UECntxtModificationFailIEs    *ie;
   U16                                           numComp;

   RLOG0(L_DEBUG, "Building Context Mod Failure Resp");

   if (cmAllocEvnt(sizeof(S1apPdu), SZ_MEM_SDU_SIZE, &wrCb.mem,
                                                    (Ptr *)&pdu) != ROK)
   {
      RLOG0(L_FATAL, "Memory allocation failed.");
      RETVALUE(RFAILED);
   }

   wrFillTknU8(&(pdu->pdu.choice), SZT_TRGR_UNSUCCESS_OUTCOME);
   failMsg = &(pdu->pdu.val.unsuccessfulOutcome);
   wrFillTknU8(&(failMsg->pres), PRSNT_NODEF);
   wrFillTknU32(&(failMsg->procedureCode), Sztid_UECntxtModification);
   wrFillTknU32(&(failMsg->criticality), SztCriticalityrejectEnum);

   /*List of IEs Filled
    * 1.UE ENB S1AP ID
    * 2.UE MME S1AP ID
    * 3.Cause
    * */
   ctxtModRsp = &(failMsg->value.u.sztUECntxtModificationFail); 
   numComp = 3;
   wrFillTknU8(&(ctxtModRsp->pres), PRSNT_NODEF);
   wrFillTknU16(&(ctxtModRsp->protocolIEs.noComp), numComp);
   if ((cmGetMem(pdu,
                 (numComp * sizeof(SztProtIE_Field_UECntxtModificationFailIEs)),
                 (Ptr *)&ctxtModRsp->protocolIEs.member)) !=ROK)
   {
      RLOG0(L_FATAL, "Memory allocation failed.");
      cmFreeMem(pdu);
      RETVALUE(RFAILED);
   }

   /* Filling MME ID */
   ie = &ctxtModRsp->protocolIEs.member[0];
   wrFillTknU8(&(ie->pres), PRSNT_NODEF);   
   wrFillTknU32(&(ie->id), Sztid_MME_UE_S1AP_ID);   
   wrFillTknU32(&(ie->criticality), SztCriticalityignoreEnum);   
   wrFillTknU32(&(ie->value.u.sztMME_UE_S1AP_ID), s1apCon->mme_ue_s1ap_id);  

   /* Filling ENB ID */
   ie = &ctxtModRsp->protocolIEs.member[1];
   wrFillTknU8(&(ie->pres), PRSNT_NODEF);  
   wrFillTknU32(&(ie->id), Sztid_eNB_UE_S1AP_ID);   
   wrFillTknU32(&(ie->criticality), SztCriticalityignoreEnum); 
   wrFillTknU32(&(ie->value.u.sztENB_UE_S1AP_ID), s1apCon->enb_ue_s1ap_id);

   /* Filling Cause */
   ie = &ctxtModRsp->protocolIEs.member[2];
   wrFillTknU8(&(ie->pres), PRSNT_NODEF);
   wrFillTknU32(&(ie->id), Sztid_Cause);
   wrFillTknU32(&(ie->criticality), SztCriticalityignoreEnum);
   wrS1apFillCause(&(ie->value.u.sztCause), cause);
   WR_INC_KPI_FUNC3(wrIncKpiFrS1apFailCause, cause->causeTyp, cause->causeVal, INC_KPI_VALUE_BY_ONE);
   *ctxtModPdu = pdu;
   RETVALUE(ROK);
}

/* ================================================ */
/* S1AP UE CONTEXT RELEASE MESSAGE BUILDING         */
/* ================================================ */


/**
 *  @brief This function builds the S1AP Ue Context Release Request
 *
 *      Function: wrUmmS1apFillCtxtRelReq
 *
 *          Processing steps:
 *          - Alloc memory to S1AP Ue Context Release Request message
 *          - fill the mandatory IE's
 *          - update the pointer to Release Request message into out pointer
 *
 *  @param [in]  s1Con : pointer to S1Ap connection CB
 *  @param [out] pdu   : double pointer to reurn the release request message
 *  @param [in]  cause : cause for the release
 *  @return S16
    -# Success : ROK
    -# Failure : RFAILED
 */

PUBLIC S16 wrUmmS1apFillCtxtRelReq
(
WrUeCb                       *ueCb,
S1apPdu                      **pdu,
WrUmmMsgCause                *cause
)
{
   S1apPdu                   *szUeCxtRlsPdu = NULLP;
   U16                       numComp;
   SztProtIE_Field_UECntxtRlsRqst_IEs *ie = NULLP;
   U32                       ieIdx = 0;
   SztInitiatingMsg          *initMsg;
   SztUECntxtRlsRqst         *ctxtRel;
   WrUmmTransCb              *hoTransCb = NULLP;
   WrS1ConCb                 *s1Con = ueCb->s1ConCb;

   RLOG0(L_DEBUG, "Constructing UE Context Release Request");

   if (cmAllocEvnt(sizeof(S1apPdu), SZ_MEM_SDU_SIZE, &wrCb.mem,
                                             (Ptr *)&szUeCxtRlsPdu) != ROK)
   {
      RLOG0(L_FATAL, "Memory allocation failed.");
      RETVALUE(RFAILED);
   }

   wrFillTknU8(&(szUeCxtRlsPdu->pdu.choice), SZT_TRGR_INTI_MSG);
   initMsg = &(szUeCxtRlsPdu->pdu.val.initiatingMsg);
   wrFillTknU8(&(initMsg->pres), PRSNT_NODEF);
   wrFillTknU32(&(initMsg->procedureCode), Sztid_UECntxtRlsRqst);
   wrFillTknU32(&(initMsg->criticality), SztCriticalityignoreEnum);

   /* Filling 3 mandatory headers */
   /*List of IEs Filled
    * 1.UE MME S1AP ID
    * 2.UE ENB S1AP ID
    * 3.CAUSE */
   ctxtRel = &initMsg->value.u.sztUECntxtRlsRqst;
   wrFillTknU8(&(ctxtRel->pres),PRSNT_NODEF);
   numComp = 3;

   /* Check if we are sending the UE Context Release request for successful X2
    * HO to HeNB-GW, if yes, increase the number of component as we need to 
    * fill the GW Context Release Indication IE too
    */
   hoTransCb = wrUmmGetHoTransCb(ueCb);
   if((NULLP != hoTransCb) && 
      (WR_UMM_X2_SRC_HO_TRANS == hoTransCb->transTyp) &&
      (TRUE == hoTransCb->u.x2SrcHoTrans.henbGwCtxtRelRequired))
   {
      numComp++;
   }

   if ((cmGetMem(szUeCxtRlsPdu, 
                 (numComp * sizeof(SztProtIE_Field_UECntxtRlsRqst_IEs)),
                 (Ptr *)&ctxtRel->protocolIEs.member)) != ROK)
   {
      RLOG0(L_FATAL, "Memory allocation failed.");
      RETVALUE(RFAILED);
   }

   /* IE1 - Filling MME ID */
   ie = &ctxtRel->protocolIEs.member[ieIdx];
   wrFillTknU8(&(ie->pres), PRSNT_NODEF);   
   wrFillTknU32(&(ie->id), Sztid_MME_UE_S1AP_ID);   
   wrFillTknU32(&(ie->criticality), SztCriticalityrejectEnum);   
   wrFillTknU32(&(ie->value.u.sztMME_UE_S1AP_ID), s1Con->mme_ue_s1ap_id);  
   ieIdx++;

   /* IE2 - Filling ENB ID */
   ie = &ctxtRel->protocolIEs.member[ieIdx];
   wrFillTknU8(&(ie->pres), PRSNT_NODEF);  
   wrFillTknU32(&(ie->id), Sztid_eNB_UE_S1AP_ID);   
   wrFillTknU32(&(ie->criticality), SztCriticalityrejectEnum); 
   wrFillTknU32(&(ie->value.u.sztENB_UE_S1AP_ID), s1Con->enb_ue_s1ap_id);
   ieIdx++;

   /* IE3 - Filling Cause */
   ie = &ctxtRel->protocolIEs.member[ieIdx];
   wrFillTknU8(&(ie->pres), PRSNT_NODEF);  
   wrFillTknU32(&(ie->id), Sztid_Cause);   
   wrFillTknU32(&(ie->criticality), SztCriticalityignoreEnum); 
   wrS1apFillCause(&ie->value.u.sztCause, cause);
   ieIdx++;

   /* Fill the GW Context Release Indication IE if the release is called
    * after the successful X2 HO and eNodeB is connected to HeNB-GW.
    * HeNB-GW check is added before calling UE context release 
    * request function from X2 UE Context Release processing function 
    */

   if((NULLP != hoTransCb) && 
      (WR_UMM_X2_SRC_HO_TRANS == hoTransCb->transTyp) &&
      (TRUE == hoTransCb->u.x2SrcHoTrans.henbGwCtxtRelRequired))
   {
      ie = &ctxtRel->protocolIEs.member[ieIdx];
      wrFillTknU8(&(ie->pres), PRSNT_NODEF);
      wrFillTknU32(&(ie->id), Sztid_GWCntxtRlsInd);
      wrFillTknU32(&(ie->criticality), SztCriticalityrejectEnum);
      wrFillTknU32(&(ie->value.u.sztGWCntxtRlsInd), SztGWCntxtRlsIndtrueEnum);
      ieIdx++;
   }

   wrFillTknU16(&(ctxtRel->protocolIEs.noComp), ieIdx);

   *pdu = szUeCxtRlsPdu; 
    WR_INC_KPI_FUNC3(wrIncKpiFrS1apFailCause, cause->causeTyp, cause->causeVal, INC_KPI_VALUE_BY_ONE);
   RETVALUE(ROK);
}

/* ================================================ */
/* S1AP NAS NON DELIVARY MESSAGE BUILDING           */
/* ================================================ */


/**
 *  @brief This function fill NAS non delivery Indication
 *
 *      Function: wrUmmSztFillNasNonDlvInd
 *
 *          Processing steps:
 *          - Alloc memory to NAS non delivery Indication message
 *          - fill the mandatory IE's
 *          - update the pointer to non delivery Indication message into out 
 *            pointer
 *
 *  @param [in]  s1apCon  : pointer to S1AP connection CB
 *  @param [out] s1ap_pdu : double pointer to to return the message
 *  @param [in]  nasPdu   : pointer to NAS PDU
 *  @param [in]  cause    : cause for the non-delivery
 *  @return S16
    -# Success : ROK
    -# Failure : RFAILED
 */
PUBLIC S16 wrUmmS1apFillNasNonDlvInd
(
WrS1ConCb                    *s1apCon,
S1apPdu                      **s1ap_pdu,
TknStrOSXL                   *nasPdu,
WrUmmMsgCause                *cause
)
{
   U16                                numComp = 0;
   SztProtIE_Field_NASNonDlvryInd_IEs *ie = NULLP;
   S1apPdu                            *szNasNonDlvInd;
   SztInitiatingMsg                   *initMsg;
   U32                                ieIdx = 0;
   SztNASNonDlvryInd                  *nasNonDel;

   /* Initialize memory control point */
   if (cmAllocEvnt(sizeof(S1apPdu), SZ_MEM_SDU_SIZE, &wrCb.mem,
                                        (Ptr *)&szNasNonDlvInd) != ROK)
   {
      RLOG0(L_FATAL, "Memory allocation failed.");
      RETVALUE(RFAILED);
   }

   wrFillTknU8(&(szNasNonDlvInd->pdu.choice), SZT_TRGR_INTI_MSG);
   initMsg = &(szNasNonDlvInd->pdu.val.initiatingMsg);
   wrFillTknU8(&(initMsg->pres), PRSNT_NODEF);
   wrFillTknU32(&(initMsg->procedureCode), Sztid_NASNonDlvryInd);
   wrFillTknU32(&(initMsg->criticality), SztCriticalityignoreEnum);

   /* Filling of 4 mandatory IEs */
   /*List of IEs Filled
    * 1. UE MME S1AP ID
    * 2. UE ENB S1AP ID
    * 3. NAS PDU
    * 4. CAUSE */
   numComp   = 4;
   nasNonDel = &initMsg->value.u.sztNASNonDlvryInd;
   wrFillTknU8(&(nasNonDel->pres), PRSNT_NODEF);
   if ((cmGetMem(szNasNonDlvInd, 
                 numComp * sizeof(SztProtIE_Field_NASNonDlvryInd_IEs),
                 (Ptr*)&nasNonDel->protocolIEs.member)) != ROK)
   {
      RLOG0(L_FATAL, "Memory allocation failed.");
      RETVALUE(RFAILED);
   }

   /* IE1 - Filling MME ID */
   ie = &nasNonDel->protocolIEs.member[ieIdx];
   wrFillTknU8(&(ie->pres), PRSNT_NODEF);
   wrFillTknU32(&(ie->id), Sztid_MME_UE_S1AP_ID);
   wrFillTknU32(&(ie->criticality), SztCriticalityrejectEnum);
   wrFillTknU32(&(ie->value.u.sztMME_UE_S1AP_ID), s1apCon->mme_ue_s1ap_id);
   ieIdx++;

   /* IE2 - Filling enb s1ap id */
   ie = &nasNonDel->protocolIEs.member[ieIdx];
   wrFillTknU8(&(ie->pres),PRSNT_NODEF);     
   wrFillTknU32(&(ie->id),Sztid_eNB_UE_S1AP_ID);
   wrFillTknU32(&(ie->criticality),SztCriticalityrejectEnum);
   wrFillTknU32(&(ie->value.u.sztENB_UE_S1AP_ID),s1apCon->enb_ue_s1ap_id);
   ieIdx++;

   /* IE3 - Filling NAS PDU */
   ie = &nasNonDel->protocolIEs.member[ieIdx];
   wrFillTknU8(&(ie->pres),PRSNT_NODEF);     
   wrFillTknU32(&(ie->id),Sztid_NAS_PDU);
   wrFillTknU32(&(ie->criticality),SztCriticalityignoreEnum);
   wrFillTknStrOSXL(&(ie->value.u.sztNAS_PDU), nasPdu->len,
                    nasPdu->val, &(szNasNonDlvInd->memCp));
   ieIdx++;

   /* IE4 - Filling Cause */
   ie = &nasNonDel->protocolIEs.member[ieIdx];
   wrFillTknU8(&(ie->pres), PRSNT_NODEF);
   wrFillTknU32(&(ie->id), Sztid_Cause);
   wrFillTknU32(&(ie->criticality), SztCriticalityignoreEnum);
   wrS1apFillCause(&ie->value.u.sztCause, cause); 
   ieIdx++;

   wrFillTknU16(&(nasNonDel->protocolIEs.noComp), ieIdx);
   *s1ap_pdu = szNasNonDlvInd;

   RETVALUE(ROK);
}

/* ================================================ */
/* S1AP INIT CONTXT SETUP FAIL MESSAGE BUILDING     */
/* ================================================ */

/**
 *  @brief This function fills the initial context setup fail message
 *
 *      Function: wrUmmSztFillInitCntxtSetFail
 *
 *          Processing steps:
 *          - Alloc memory to initial context setup fail message
 *          - fill the mandatory IE's
 *          - update the pointer to initial context setup fail message into out 
 *            pointer
 *
 *  @param [in]  s1apCon  : pointer to S1AP connection CB 
 *  @param [out] s1apPdu  : double pointer S1AP pdu to return the ICS fail message
 *  @param [in]  cause    : cause for the failure
 *  @return 
 */

PUBLIC S16 wrUmmS1apFillInitCntxtSetFail
(
WrS1ConCb                    *s1apCon,
S1apPdu                      **s1apPdu,
WrUmmMsgCause                *cause
)
{
   U16                                   numComp = 0;
   SztProtIE_Field_InitCntxtSetupFailIEs *ie = NULLP;
   S1apPdu                               *szInitCntxtSetFail = NULLP;
   U16                                   ieIdx = 0;
   SztUnsuccessfulOutcome                *unsuccMsg;
   SztInitCntxtSetupFail                 *ctxtFail;

   /* Initialize memory control point */
   if (cmAllocEvnt(sizeof(S1apPdu), SZ_MEM_SDU_SIZE, &wrCb.mem,
                                  (Ptr *)&szInitCntxtSetFail) != ROK)
   {
      RLOG0(L_FATAL, "Memory allocation failed.");
      RETVALUE(RFAILED);
   }

   wrFillTknU8(&(szInitCntxtSetFail->pdu.choice), SZT_TRGR_UNSUCCESS_OUTCOME);
   unsuccMsg = &szInitCntxtSetFail->pdu.val.unsuccessfulOutcome;
   wrFillTknU8(&(unsuccMsg->pres), PRSNT_NODEF);
   wrFillTknU32(&(unsuccMsg->procedureCode), Sztid_InitCntxtSetup);
   wrFillTknU32(&(unsuccMsg->criticality), SztCriticalityrejectEnum);

   /* Filling of 3 mandatory IEs */
   /*List of IEs Filled
    * 1.UE MME S1AP ID
    * 2.UE ENB S1AP ID
    * 3.CAUSE */
   numComp  = 3;
   ctxtFail = &unsuccMsg->value.u.sztInitCntxtSetupFail;
   wrFillTknU8(&(ctxtFail->pres), PRSNT_NODEF);

   /* Allocate memory for 3 IEs */
   if ((cmGetMem(szInitCntxtSetFail, 
                 numComp * sizeof(SztProtIE_Field_InitCntxtSetupFailIEs),
                 (Ptr*)&ctxtFail->protocolIEs.member)) != ROK)
   {
      RLOG0(L_FATAL, "Memory allocation failed.");
      RETVALUE(RFAILED);
   }

   /* IE1 - Filling MME ID */
   ie = &ctxtFail->protocolIEs.member[ieIdx];
   wrFillTknU8(&(ie->pres), PRSNT_NODEF);
   wrFillTknU32(&(ie->id), Sztid_MME_UE_S1AP_ID);
   wrFillTknU32(&(ie->criticality), SztCriticalityignoreEnum);
   wrFillTknU32(&(ie->value.u.sztMME_UE_S1AP_ID), s1apCon->mme_ue_s1ap_id);
   ieIdx++;

   /* IE2 - Filling enb s1ap id */
   ie = &ctxtFail->protocolIEs.member[ieIdx];
   wrFillTknU8(&(ie->pres), PRSNT_NODEF);     
   wrFillTknU32(&(ie->id), Sztid_eNB_UE_S1AP_ID);
   wrFillTknU32(&(ie->criticality),SztCriticalityignoreEnum);
   wrFillTknU32(&(ie->value.u.sztENB_UE_S1AP_ID),s1apCon->enb_ue_s1ap_id);
   ieIdx++;

   /* IE3 - Filling Cause */
   ie = &ctxtFail->protocolIEs.member[ieIdx];
   wrFillTknU8(&(ie->pres), PRSNT_NODEF);
   wrFillTknU32(&(ie->id), Sztid_Cause);
   wrFillTknU32(&(ie->criticality), SztCriticalityignoreEnum);
   wrS1apFillCause(&ie->value.u.sztCause, cause);
   ieIdx++;

   wrFillTknU16(&(ctxtFail->protocolIEs.noComp), ieIdx);

   *s1apPdu = szInitCntxtSetFail;

   RETVALUE(ROK);
}

/* ================================================ */
/* S1AP UE CNTXT RELEASE CMPLT MESSAGE BUILDING     */
/* ================================================ */


/**
 *  @brief This function builds the S1AP Ue context release complete message
 *
 *      Function: wrUmmSztUeCntxtRelComp 
 *
 *          Processing steps:
 *          - Alloc memory to S1AP Ue context release complete message
 *          - fill the mandatory IE's
 *          - update the pointer to S1AP Ue context release complete message
 *            into out pointer
 *
 *  @param [in]  s1Con : pointer to S1 Connection CB 
 *  @param [out] pdu   : double pointer to return the pointer to S1AP Ue context
 *                       release complete message
 *  @return S16
    -# Success : ROK
    -# Failure : RFAILED
 */

PUBLIC S16 wrUmmS1apFillCtxtRelCmpl
(
WrS1ConCb                    *s1Con,
S1apPdu                      **pdu
)
{
   S1apPdu                   *szUeCxtRlsCmplPdu = NULLP;
   U16                       numComp;
   SztProtIE_Field_UECntxtRlsComplete_IEs *ie = NULLP;
   U16                       ieIdx = 0;
   SztSuccessfulOutcome      *succMsg;
   SztUECntxtRlsComplete     *ctxtRelComp;

   RLOG0(L_DEBUG, "Constructing UE Context Release");

   if (cmAllocEvnt(sizeof(S1apPdu), SZ_MEM_SDU_SIZE, &wrCb.mem,
                                     (Ptr *)&szUeCxtRlsCmplPdu) != ROK)
   {
      RLOG0(L_FATAL, "Memory allocation failed.");
      RETVALUE(RFAILED);
   }

   wrFillTknU8(&(szUeCxtRlsCmplPdu->pdu.choice), SZT_TRGR_SUCCESS_OUTCOME);
   succMsg = &szUeCxtRlsCmplPdu->pdu.val.successfulOutcome;
   wrFillTknU8(&(succMsg->pres), PRSNT_NODEF);
   wrFillTknU32(&(succMsg->procedureCode), Sztid_UECntxtRls);
   wrFillTknU32(&(succMsg->criticality), SztCriticalityrejectEnum);

   /* Filling 2 mandatory headers */
   /*List of IEs Filled
    * 1. UE MME S1AP ID
    * 2. UE ENB S1AP ID*/
   numComp     = 2;
   ctxtRelComp = &succMsg->value.u.sztUECntxtRlsComplete;
   wrFillTknU8(&(ctxtRelComp->pres),PRSNT_NODEF);
   if ((cmGetMem(szUeCxtRlsCmplPdu, 
                 (numComp * sizeof(SztProtIE_Field_UECntxtRlsComplete_IEs)),
                 (Ptr *)&ctxtRelComp->protocolIEs.member)) != ROK)
   {
      RLOG0(L_FATAL, "Memory allocation failed.");
      RETVALUE(RFAILED);
   }

   /* IE1 - Filling MME ID */
   ie = &ctxtRelComp->protocolIEs.member[ieIdx];
   wrFillTknU8(&(ie->pres), PRSNT_NODEF);   
   wrFillTknU32(&(ie->id), Sztid_MME_UE_S1AP_ID);   
   wrFillTknU32(&(ie->criticality), SztCriticalityignoreEnum);   
   wrFillTknU32(&(ie->value.u.sztMME_UE_S1AP_ID), s1Con->mme_ue_s1ap_id);  
   ieIdx++;

   /* IE2 - Filling ENB ID */
   ie = &ctxtRelComp->protocolIEs.member[ieIdx];
   wrFillTknU8(&(ie->pres), PRSNT_NODEF);  
   wrFillTknU32(&(ie->id), Sztid_eNB_UE_S1AP_ID);   
   wrFillTknU32(&(ie->criticality), SztCriticalityignoreEnum); 
   wrFillTknU32(&(ie->value.u.sztENB_UE_S1AP_ID), s1Con->enb_ue_s1ap_id);
   ieIdx++;

   wrFillTknU16(&(ctxtRelComp->protocolIEs.noComp), numComp);

   *pdu = szUeCxtRlsCmplPdu; 

   RETVALUE(ROK);
} /* wrUmmS1apFillCtxtRelCmpl */

/* ================================================ */
/* S1AP ERAB SETUP RESPONSE MESSAGE BUILDING        */
/* ================================================ */
/* CODE PRESENT IN init context .c */


/**
 *  @brief This function builds the UL NAS pdu
 *
 *      Function: wrUmmS1apBldUlNasPdu
 *
 *          Processing steps:
 *          - Alloc memory to S1AP UL NAS pdu message
 *          - fill the mandatory IE's
 *          - update the pointer to S1AP UL NAS pdu message
 *            into out pointer
 *
 *  @param [in]  ueCb     : pointer to UE CB
 *  @param [in]  nasPdu   : encoded nas pdu
 *  @param [out] pdu      : double pointer to return the UL NAS PDU
 *  @return S16
    -# Success : ROK
    -# Failure : RFAILED 
 */
PUBLIC S16 wrUmmS1apBldUlNasPdu
(
WrUeCb                       *ueCb,
TknStrOSXL                   *nasPdu,
S1apPdu                      **pdu
)
{
   WrS1ConCb                 *s1apCon = ueCb->s1ConCb;
   S1apPdu                   *ulNasPdu = NULLP;
   SztInitiatingMsg          *initMsg;
   U32                       ieIdx = 0;
   SztUlnkNASTport           *ulNas;
   SztProtIE_Field_UlnkNASTport_IEs *ie = NULLP;

   /* Initialize memory control point */
   if (cmAllocEvnt(sizeof(S1apPdu), SZ_MEM_SDU_SIZE, &wrCb.mem,
                                             (Ptr *)&ulNasPdu) != ROK)
   {
      RLOG0(L_FATAL, "Memory allocation failed.");
      RETVALUE(RFAILED);
   }

   wrFillTknU8(&(ulNasPdu->pdu.choice), S1AP_PDU_INITIATINGMSG);
   initMsg = &ulNasPdu->pdu.val.initiatingMsg;
   wrFillTknU8(&(initMsg->pres), PRSNT_NODEF);
   wrFillTknU32(&(initMsg->criticality), SztCriticalityignoreEnum);
   wrFillTknU32(&(initMsg->procedureCode), Sztid_uplinkNASTport);
   ulNas = &(initMsg->value.u.sztUlnkNASTport);
   wrFillTknU8(&(ulNas->pres), PRSNT_NODEF);

   /* Allocate memory for 5 IEs */
   if ((cmGetMem(ulNasPdu, 5 * sizeof(SztProtIE_Field_UlnkNASTport_IEs),
                                   (Ptr*)&ulNas->protocolIEs.member)) !=ROK)
   {
      RLOG0(L_FATAL, "Memory allocation failed.");
      RETVALUE(RFAILED);
   }

   /* IE1 - Filling MME ID */
   ie = &ulNas->protocolIEs.member[ieIdx];
   wrFillTknU8(&(ie->pres), PRSNT_NODEF);
   wrFillTknU32(&(ie->id), Sztid_MME_UE_S1AP_ID);
   wrFillTknU32(&(ie->criticality), SztCriticalityrejectEnum);
   wrFillTknU32(&(ie->value.u.sztMME_UE_S1AP_ID), s1apCon->mme_ue_s1ap_id);
   ieIdx++;

   /* IE2 - Filling enb s1ap id */
   ie = &ulNas->protocolIEs.member[ieIdx];
   wrFillTknU8(&(ie->pres),PRSNT_NODEF);     
   wrFillTknU32(&(ie->id),Sztid_eNB_UE_S1AP_ID);
   wrFillTknU32(&(ie->criticality),SztCriticalityrejectEnum);
   wrFillTknU32(&(ie->value.u.sztENB_UE_S1AP_ID), s1apCon->enb_ue_s1ap_id);
   ieIdx++;

   /* IE3 - Filling nas pdu */
   ie = &ulNas->protocolIEs.member[ieIdx];
   wrFillTknU8(&(ie->pres),PRSNT_NODEF);     
   wrFillTknU32(&(ie->id),Sztid_NAS_PDU);
   wrFillTknU32(&(ie->criticality),SztCriticalityrejectEnum);
   wrFillTknStrOSXL(&(ie->value.u.sztNAS_PDU), nasPdu->len, nasPdu->val, 
                    &(ulNasPdu->memCp));
   ieIdx++;

   /* IE4 - Filling  EUTRAN CGI */
   ie = &ulNas->protocolIEs.member[ieIdx];
   wrFillTknU8(&(ie->pres),PRSNT_NODEF);
   wrFillTknU32(&(ie->id),Sztid_EUTRAN_CGI);
   wrFillTknU32(&(ie->criticality),SztCriticalityignoreEnum);
   wrUmmS1apFillEutranCgi(ueCb, ulNasPdu, &(ie->value.u.sztEUTRAN_CGI));
   ieIdx++;
  
   /* IE5 - Filling  TAI*/
   ie = &ulNas->protocolIEs.member[ieIdx];
   wrFillTknU8(&(ie->pres),PRSNT_NODEF);
   wrFillTknU32(&(ie->id),Sztid_TAI);
   wrFillTknU32(&(ie->criticality),SztCriticalityignoreEnum);
   wrFillTknU8(&(ie->value.u.sztTAI.pres),PRSNT_NODEF);
   wrUmmS1apFillTAI(ueCb, ulNasPdu, &(ie->value.u.sztTAI));
   ieIdx++;

   /* Fill the number of IEs in the UL NAS message                        */
   wrFillTknU16(&(ulNas->protocolIEs.noComp), ieIdx);

   *pdu = ulNasPdu;

   RETVALUE(ROK);
}

/**
 *  @brief This function builds the Location Report PDU
 *
 *      Function: wrUmmS1apBldLocReport
 *
 *          Processing steps:
 *          - Alloc memory to S1AP Location Report pdu message
 *          - fill the mandatory IE's
 *          - update the pointer to S1AP Location Report pdu message
 *            into out pointer
 *
 *  @param [in]  ueCb     : pointer to UE CB
 *  @param [out] pdu      : double pointer to return the Location Report
 *  @return S16
    -# Success : ROK
    -# Failure : RFAILED 
 */
PUBLIC S16 wrUmmS1apBldLocReport
(
WrUeCb                       *ueCb,
S1apPdu                      **pdu
)
{
   WrS1ConCb                    *s1apCon = ueCb->s1ConCb;
   S1apPdu                      *locReptPdu;
   U32                           noComp;
   SztProtIE_Field_LocReportIEs *ie;
   SztLocReport                 *locRpt;
   WrTai                         tai;

   *pdu = NULLP;

   /* Initialize memory control point */
   if (cmAllocEvnt(sizeof(S1apPdu), SZ_MEM_SDU_SIZE, &wrCb.mem,
                                             (Ptr *)&locReptPdu) != ROK)
   {
      RLOG0(L_FATAL, "Memory allocation failed.");
      RETVALUE(RFAILED);
   }

   RLOG_ARG2(L_EVENT, DBG_CRNTI,ueCb->crnti,"[S1AP]:Sending LOCATION REPORT"
             "[MME-UE-S1AP-ID:%lu] [eNB-UE-S1AP-ID:%lu] ",
             s1apCon->mme_ue_s1ap_id, s1apCon->enb_ue_s1ap_id);

   wrFillTknU8(&(locReptPdu->pdu.choice), S1AP_PDU_INITIATINGMSG);
   wrFillTknU8(&(locReptPdu->pdu.val.initiatingMsg.pres), PRSNT_NODEF);
   wrFillTknU32(&(locReptPdu->pdu.val.initiatingMsg.criticality), 
                  SztCriticalityignoreEnum);
   wrFillTknU32(&(locReptPdu->pdu.val.initiatingMsg.procedureCode), Sztid_LocReport);

   noComp = WR_UMM_NO_COMP_LOCATION_REPORT;

   locRpt = &(locReptPdu->pdu.val.initiatingMsg.value.u.sztLocReport);
   wrFillTknU8(&(locRpt->pres), PRSNT_NODEF);
   wrFillTknU16(&(locRpt->protocolIEs.noComp), noComp);

   if ((cmGetMem(locReptPdu, noComp * sizeof(SztProtIE_Field_LocReportIEs),
                 (Ptr*)&locRpt->protocolIEs.member)) !=ROK)
   {
      RLOG0(L_FATAL, "Memory allocation failed.");
      cmFreeMem(locReptPdu);
      RETVALUE(RFAILED);
   }

   /* IE1 - Filling MME ID */
   ie = &locRpt->protocolIEs.member[0];
   wrFillTknU8(&(ie->pres), PRSNT_NODEF);
   wrFillTknU32(&(ie->id), Sztid_MME_UE_S1AP_ID);
   wrFillTknU32(&(ie->criticality), SztCriticalityrejectEnum);
   wrFillTknU32(&(ie->value.u.sztMME_UE_S1AP_ID), s1apCon->mme_ue_s1ap_id);

   /* IE2 - Filling enb s1ap id */
   ie = &locRpt->protocolIEs.member[1];
   wrFillTknU8(&(ie->pres), PRSNT_NODEF);     
   wrFillTknU32(&(ie->id), Sztid_eNB_UE_S1AP_ID);
   wrFillTknU32(&(ie->criticality), SztCriticalityrejectEnum);
   wrFillTknU32(&(ie->value.u.sztENB_UE_S1AP_ID), s1apCon->enb_ue_s1ap_id);

   /* IE 3 Fill E-UTRAN CGI Id which is primary PLMN id from the SIB */
   ie = &locRpt->protocolIEs.member[2];
   wrFillTknU8(&(ie->pres), PRSNT_NODEF);     
   wrFillTknU32(&(ie->id), Sztid_EUTRAN_CGI);
   wrFillTknU32(&(ie->criticality), SztCriticalityignoreEnum);
   wrUmmS1apFillEutranCgi(ueCb, locReptPdu, &(ie->value.u.sztEUTRAN_CGI));

   /* Get the TAI informatiom, only TAC is picked from this as we need to fill the 
    * PLMN from the UeCb which is sent by the UE during initial attach */
   wrEmmGetTai(ueCb, &tai);

   /* IE 4 Fill the TAI IE in the PDU */
   ie = &locRpt->protocolIEs.member[3];
   wrFillTknU8(&(ie->pres), PRSNT_NODEF);     
   wrFillTknU32(&(ie->id), Sztid_TAI);
   wrFillTknU32(&(ie->criticality), SztCriticalityignoreEnum);

   wrFillTknU8(&(ie->value.u.sztTAI.pres), PRSNT_NODEF);
   wrUmmSztFillPLMNId(locReptPdu, &(ueCb->plmnId), &(ie->value.u.sztTAI.pLMNidentity));
   wrUmmSztFillTAC(locReptPdu, tai.tac, &(ie->value.u.sztTAI.tAC));
   ie->value.u.sztTAI.iE_Extns.noComp.pres = NOTPRSNT;

   /* IE 5 Fill the request type. it will be either 'Direct' or 
    * 'Report whenever cell changes' */
   ie = &locRpt->protocolIEs.member[4];
   wrFillTknU8(&(ie->pres), PRSNT_NODEF);     
   wrFillTknU32(&(ie->id), Sztid_RqstTyp);
   wrFillTknU32(&(ie->criticality), SztCriticalityignoreEnum);

   wrFillTknU8(&(ie->value.u.sztRqstTyp.pres), PRSNT_NODEF);
   if(ueCb->locReportType)
   {
      wrFillTknU32(&(ie->value.u.sztRqstTyp.eventTyp), SztEventTypchange_of_serve_cellEnum);
   }
   else
   {
      wrFillTknU32(&(ie->value.u.sztRqstTyp.eventTyp), SztEventTypdirectEnum);
   }
   wrFillTknU32(&(ie->value.u.sztRqstTyp.reportArea), SztReportAreaecgiEnum);
   ie->value.u.sztRqstTyp.iE_Extns.noComp.pres = NOTPRSNT;

   *pdu = locReptPdu;

   RETVALUE(ROK);
}

/**
 *  @brief This function builds the Location Report Fail PDU
 *
 *      Function: wrUmmS1apBldLocReportFail
 *
 *          Processing steps:
 *          - Alloc memory to S1AP Location Report Fail pdu message
 *          - fill the mandatory IE's
 *          - update the pointer to S1AP Location Report Fail pdu message
 *            into out pointer
 *
 *  @param [in]  ueCb     : pointer to UE CB
 *  @param [out] pdu      : double pointer to return the Location Report Fail
 *  @return S16
    -# Success : ROK
    -# Failure : RFAILED 
 */
PUBLIC S16 wrUmmS1apBldLocReportFail
(
WrUeCb                       *ueCb,
S1apPdu                      **pdu
)
{
   WrS1ConCb                           *s1apCon = ueCb->s1ConCb;
   S1apPdu                             *locReptFailPdu;
   U32                                  noComp;
   SztProtIE_Field_LocRprtngFailIndIEs *ie;
   SztLocRprtngFailInd                 *locRptFail;
   WrUmmMsgCause                        cause;

   *pdu = NULLP;

   /* Initialize memory control point */
   if (cmAllocEvnt(sizeof(S1apPdu), SZ_MEM_SDU_SIZE, &wrCb.mem,
                                             (Ptr *)&locReptFailPdu) != ROK)
   {
      RLOG0(L_FATAL, "Memory allocation failed.");
      RETVALUE(RFAILED);
   }

   RLOG_ARG2(L_EVENT, DBG_CRNTI,ueCb->crnti,"[S1AP]:Sending LOCATION REPORT "
             "FAILURE INDICATION [MME-UE-S1AP-ID:%lu] [eNB-UE-S1AP-ID:%lu] ",
             s1apCon->mme_ue_s1ap_id, s1apCon->enb_ue_s1ap_id);

   wrFillTknU8(&(locReptFailPdu->pdu.choice), S1AP_PDU_INITIATINGMSG);
   wrFillTknU8(&(locReptFailPdu->pdu.val.initiatingMsg.pres), PRSNT_NODEF);
   wrFillTknU32(&(locReptFailPdu->pdu.val.initiatingMsg.criticality), 
                  SztCriticalityignoreEnum);
   wrFillTknU32(&(locReptFailPdu->pdu.val.initiatingMsg.procedureCode), 
                  Sztid_LocRprtngFailInd);

   noComp = WR_UMM_NO_COMP_LOCATION_REPORT_FAIL;

   locRptFail = &(locReptFailPdu->pdu.val.initiatingMsg.value.u.sztLocRprtngFailInd);
   wrFillTknU8(&(locRptFail->pres), PRSNT_NODEF);
   wrFillTknU16(&(locRptFail->protocolIEs.noComp), noComp);

   if ((cmGetMem(locReptFailPdu, noComp * sizeof(SztProtIE_Field_LocRprtngFailIndIEs),
                 (Ptr*)&locRptFail->protocolIEs.member)) !=ROK)
   {
      RLOG0(L_FATAL, "Memory allocation failed.");
      cmFreeMem(locReptFailPdu);
      RETVALUE(RFAILED);
   }

   /* IE1 - Filling MME ID */
   ie = &locRptFail->protocolIEs.member[0];
   wrFillTknU8(&(ie->pres), PRSNT_NODEF);
   wrFillTknU32(&(ie->id), Sztid_MME_UE_S1AP_ID);
   wrFillTknU32(&(ie->criticality), SztCriticalityrejectEnum);
   wrFillTknU32(&(ie->value.u.sztMME_UE_S1AP_ID), s1apCon->mme_ue_s1ap_id);

   /* IE2 - Filling enb s1ap id */
   ie = &locRptFail->protocolIEs.member[1];
   wrFillTknU8(&(ie->pres), PRSNT_NODEF);     
   wrFillTknU32(&(ie->id), Sztid_eNB_UE_S1AP_ID);
   wrFillTknU32(&(ie->criticality), SztCriticalityrejectEnum);
   wrFillTknU32(&(ie->value.u.sztENB_UE_S1AP_ID), s1apCon->enb_ue_s1ap_id);

   /* Fill the Cause for failure */
   ie = &locRptFail->protocolIEs.member[2];
   wrFillTknU8(&(ie->pres), PRSNT_NODEF);     
   wrFillTknU32(&(ie->id), Sztid_Cause);
   wrFillTknU32(&(ie->criticality), SztCriticalityignoreEnum);

   /* ccpu00131919 */
   cause.causeTyp = WR_CAUSE_RADIONW;
   if(ueCb->hoType == WR_HO_TYPE_X2)
   {
      cause.causeVal = SztCauseRadioNwx2_handover_triggeredEnum;   
   }
   else if( WR_HO_TYPE_INTRA_LTE_S1 == ueCb->hoType)
   {
      cause.causeVal = SztCauseRadioNws1_intra_system_handover_triggeredEnum;      
   }
   else
   {
      cause.causeVal = SztCauseRadioNws1_inter_system_handover_triggeredEnum;         
   }

   wrS1apFillCause(&(ie->value.u.sztCause), &cause);
   /* ccpu00131043 */
   WR_INC_KPI_FUNC3(wrIncKpiFrS1apFailCause, cause.causeTyp, cause.causeVal, INC_KPI_VALUE_BY_ONE);
   *pdu = locReptFailPdu;

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
