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
               

     File:     wr_umm_s1ap_hdl.c

     Sid:      $SID$ 

     Prg:      Sriky 

*********************************************************************21*/
/* header include files (.h) */
static const char* RLOG_MODULE_NAME="UMM";
static int RLOG_FILE_ID=69;


static int RLOG_MODULE_ID=32;

#include "wr.h"        /* Common Includes */
#include "wr_emm.h"
#include "wr_umm.h"
#include "wr_ifm_s1ap.h"
#include "wr_umm_trans.h"


PRIVATE Bool wrUmmCheckIsHoStsIEPresDlCdma
(
S1apPdu              *pdu
)
{
   Bool     result    = FALSE;
   U8       idx = 0;

   SztDlnkS1cdma2000tunneling  *dlCdma;
   SztProtIE_Field_DlnkS1cdma2000tunnelingIEs *ie = NULLP;

   if(pdu->pdu.val.initiatingMsg.procedureCode.val == Sztid_DlnkS1cdma2000tunneling)
   {
      dlCdma = &pdu->pdu.val.initiatingMsg.value.u.sztDlnkS1cdma2000tunneling;
      /* now fill the protocol field member */
      /* if in dlCDMA tunneling msg contains HOStaus ID 
       * is present then return TRUE else FALSE */

      for(idx = 0; idx < dlCdma->protocolIEs.noComp.val; idx++)
      {
         ie = &dlCdma->protocolIEs.member[idx];

         if(ie->id.val == Sztid_cdma2000HOStatus)
         {
            result = TRUE;
            break;
         }
      }
   }
   RETVALUE(result);
}


/**
 *  @brief This function determines the tansaction Id for a message 
 *
 *  @details Transaction ID determination from the message is needed only
 *           for successful response or a failure. If it is an initiating 
 *           PDU, a new transaction should be allocated for such procedures.
 *           This function returns ROK to suggest successful processing. It
 *           returns FAILED if the message ID is invalid to indicate failure.
 *           
 *
 *      Function: wrUmmS1GetTransIdFromMsg
 *
 *          Processing steps:
 *          - switch to the message type
 *          - fetch the message ID or assign new message ID for an initiating
 *            PDU
 *
 *  @param[in]  ueCb    : pointer to UE control block
 *  @param[in]  msg     : pointer to the incoming message
 *  @param[out] transId : pointer to return the transId
 *  @return S16
    -# Success : ROK
    -# Failure : RFAILED
 */


/* ********************************************************************** */
/*                                                                        */
/* Transaction ID determination from the message is needed only for       */
/* successful response or a failure. If it is an initiating PDU, a new    */
/* transaction should be allocated for such procedures                    */
/*                                                                        */
/* This function returns ROK to suggest successful processing. It returns */
/* RFAILED to indicate failure.                                           */
/*                                                                        */
/* transId is populated with transaction identifier if a match is found.  */
/* If the PDU includes an initiating procedure, it returns 0 to suggest   */
/* that a new transaction needs to be allocated                           */
/*                                                                        */
/* ********************************************************************** */
PUBLIC S16 wrUmmS1GetTransIdFromMsg
(
WrUeCb                       *ueCb,
WrUmmIncMsg                  *msg,
U32                          *transId
)
{
   S1apPdu                   *pdu = msg->u.s1Pdu->pdu;
   SztInitiatingMsg          *initiatingMsg = &pdu->pdu.val.initiatingMsg;

   switch(pdu->pdu.choice.val)
   {
      case S1AP_PDU_INITIATINGMSG:
      {
            /* Check whether HOStatus is present in DL CDMA
             * tunnelling message or not. If return TRUE,
             * then find the existing eCSFB trnsaction Id
             * else create a new transaction id for DL CDMA msg */
            if(TRUE == wrUmmCheckIsHoStsIEPresDlCdma(pdu))
            {
               /* Get the existing transId of eCSFB transaction */
               if(wrUmmGetExistingTransId(WR_UMM_ECSFB_TRANS, &ueCb->transLst, transId) != ROK)
               {
                  RETVALUE(RFAILED);
               }
            }
            else if(ueCb->hoTransId != 0)
            {
               /*Check for the Message Type Received and proceed weather to create
                * new Transaction of use exitsting transaction */
               switch(initiatingMsg->procedureCode.val)
               {
                  case Sztid_UECntxtModification:
                  case Sztid_E_RABSetup:
                  case Sztid_E_RABMdfy:
                  case Sztid_E_RABRls:
                  case Sztid_downlinkNASTport:
                     {
                        *transId = WR_CREATE_NEW_TRANS;
                     }
                     break;
                  default:
                     {
                        *transId = ueCb->hoTransId;
                     }
                     break;

                     RETVALUE(ROK);
               }
            }
         else
         {
            *transId = WR_CREATE_NEW_TRANS;
         }
         RETVALUE(ROK);
      }
      case S1AP_PDU_SUCCESSFULOUTCOME:
      {
         if(ueCb->hoTransId != 0)
         {
            *transId = ueCb->hoTransId;
         }
         else
         {
            *transId = WR_CREATE_NEW_TRANS;
         }
         RETVALUE(ROK);
      }
      case S1AP_PDU_UNSUCCESSFULOUTCOME:
      {
         if(ueCb->hoTransId != 0)
         {
            *transId = ueCb->hoTransId;
         }
         else
         {
            *transId = WR_CREATE_NEW_TRANS;
         }
         RETVALUE(ROK);
      }
   }
   RETVALUE(RFAILED);
} /* end of wrUmmS1GetTransIdFromMsg */ 

/**
 *  @brief This function is responsible for identifying transaction type 
 *         based on the received message. All transactions start with 
 *         initiating messages
 *
 *      Function: wrUmmGetS1TransType
 *
 *          Processing steps:
 *          - switch to the message type
 *             - assign the transaction type to the pointer provided
 *             - return ROK
 *          - return RFAILED if invalid message type is received
 *
 *  @param[in]  msg      : pointer to the incomimng geessa
 *  @param[out] transTyp : pointer to return the transaction type
 *  @return S16
    -# Success : ROK
    -# Failure : RFAILED
 */


/* ********************************************************************** */
/*                                                                        */
/* This function is responsible for identifying transaction type based    */
/* on the received message. All transactions start with initiating        */
/* messages.                                                              */
/*                                                                        */
/* ********************************************************************** */
PUBLIC S16 wrUmmGetS1TransType
(
S1apPdu                      *pdu,
U32                          *transTyp
)
{
   U8                        msgType;

   wrSztUtilsGetMsgIdx(&msgType, pdu);
   switch(msgType)
   {
      case SZT_MSG_ID_HO_REQ:
      {
         *transTyp = WR_UMM_S1_TGT_HO_TRANS;
         RETVALUE(ROK);
      }
      case SZT_MSG_ID_HO_CANCEL_ACK:
      {
         RLOG0(L_ERROR, "HO_CANCEL_ACK received."
                        "No New Transaction will be created"); 
         RETVALUE(RFAILED);
      }
      case SZT_MSG_ID_HO_PREP_FAIL:
      case SZT_MSG_ID_HO_CMD:
      {
          ALARM("%s: Not handling S1AP Msg Type:%d, since HO Trans does not exist",__func__,msgType); 
          *transTyp = WR_UMM_TRANS_UKNW;
         RETVALUE(RFAILED);
      }
      case SZT_MSG_ID_DL_NAS_TPT:
      {
         /* Need to implement for initial context setup message here */
         *transTyp = WR_UMM_DL_NAS_TRANS;
         RETVALUE(ROK);
      }
      case SZT_MSG_ID_E_RAB_BRR_SETUP_REQ:
      {
         /* Need to implement for initial context setup message here */
         *transTyp = WR_UMM_ERAB_SETUP_TRANS;
         RETVALUE(ROK);
      }
      case SZT_MSG_ID_E_RAB_BRR_MOD_REQ:
      {
         /* Need to implement for ERAB modify setup message here */
         *transTyp = WR_UMM_ERAB_MOD_TRANS;
         RETVALUE(ROK);
      }
      /*In case of error indication release UE */
      case SZT_MSG_ID_UE_CXT_REL_CMD:
      case SZT_MSG_ID_ERR_IND:
      {
         *transTyp = WR_UMM_CTXT_REL_TRANS;
         RETVALUE(ROK);
      }
      case SZT_MSG_ID_E_RAB_BRR_REL_CMD:
      {
         *transTyp = WR_UMM_RAB_REL_TRANS;
         RETVALUE(ROK);
      }
      case SZT_MSG_ID_UE_CXT_MOD_REQ:
      {
         *transTyp = WR_UMM_UE_CTXT_MOD_TRANS;
         RETVALUE(ROK);
      }
      case SZT_MSG_ID_CXT_SETUP_REQ:
      {
         *transTyp = WR_UMM_INIT_CTXT_TRANS;
         RETVALUE(ROK);
      }
      case SZT_MSG_ID_LOC_REPORT_CNTRL:
      {
         *transTyp = WR_UMM_LOCATION_REPORT_TRANS;
         RETVALUE(ROK);
      }
      case SZT_MSG_ID_DNLNK_CDMA_TUNNL:
      {
         *transTyp = WR_UMM_DL_CDMA_TRANS; 
         RETVALUE(ROK);
      }
      default:
      {
          RLOG1(L_ERROR, "Not handling S1AP Msg Type[%d]",msgType); 
          *transTyp = WR_UMM_TRANS_UKNW;
          RETVALUE(ROK);
      }
   }
   RETVALUE(RFAILED);
} /* end of wrUmmGetS1TransType */ 



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
