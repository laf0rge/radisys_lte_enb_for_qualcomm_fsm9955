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
               

     File:     wr_umm_rrc_hdl.c

     Sid:      $SID$ 

     Prg:      Sriky 

*********************************************************************21*/
/* header include files (.h) */
static const char* RLOG_MODULE_NAME="UMM";
static int RLOG_FILE_ID=65;


static int RLOG_MODULE_ID=32;

#include "wr.h"        /* Common Includes */
#include "wr_emm.h"
#include "wr_umm.h"
#include "wr_umm_trans.h"
#include "wr_ifm_rrc.h"
#include "wr_umm_meas_fun.h"

/**
 *  @brief This function determines the tansaction Id for a message 
 *
 *  @details Transaction ID determination from the message is needed only
 *           for successful response or a failure. If it is an initiating 
 *           PDU, a new transaction should be allocated for such procedures.
 *           This function returns ROK to suggest successful processing. It
 *           returns FAILED if the message ID ins invalid to indicate failure.
 *
 *      Function: wrUmmUuGetTransIdFromMsg
 *
 *          Processing steps:
 *          - switch to the message type
 *             - get teh transaction ID or  assign new message ID for an initiating
 *               PDU
 *             - if failed receive the transID 
 *                - return failure
 *             - set the transaction ID using the CRNTI, transID and cellId
 *             - return Success
 *
 *  @param [in]  ueCb    : pointer to UE control block
 *  @param [in]  msg     : pointer to the incoming message
 *  @param [in]  transId : pointer to return the transId
 *  @return S16
    -# Success : ROK
    -# Failure : RFAILED
 */

PUBLIC S16 wrUmmUuGetTransIdFromMsg
(
WrUeCb                       *ueCb,
WrUmmIncMsg                  *msg,
U32                          *transId
)
{
   U8                        rrcTransId;
   U8                        smallTrans;

   switch(msg->msgTyp)
   {
      case WR_UMM_UU_IND_PDU:
      {
         switch (msg->u.uuIndPdu->sdu.msgCategory)
         {
            case NHU_MSG_ULCCCH:
            {
               *transId = WR_CREATE_NEW_TRANS;
            }
            break;
            case NHU_MSG_ULDCCH:
            {
               NhuUlDcchMsg *ulDcch = &msg->u.uuIndPdu->sdu.m.ulDcchMsg;
               switch (ulDcch->dcchMsg.message.val.c1.choice.val)
               {
                  case C1_RRCCONRECFGNCOMPL:
                  {
                        NhuUL_DCCH_MsgTypc1 *c1 = &ulDcch->dcchMsg.message.val.c1;
                        rrcTransId = c1->val.rrcConRecfgnCompl.rrc_TrnsxnId.val;
                        if(wrUmmGetTransId(ueCb, C1_RRCCONRECFGN, 
                                 rrcTransId, &smallTrans) == ROK)
                        {
                           WR_SET_TRANS_ID(*transId, ueCb->cellId, ueCb->ueIdx, 
                                 smallTrans);
                        }
                        else if(ueCb->hoTransId)
                        {
                          *transId = ueCb->hoTransId;
                        }
                        else
                        {
            *transId = 0;
                           RETVALUE(RFAILED);
                        }
                  }
                  break;
                  case C1_RRCCONSETUPCOMPL:
                  {
                     /* RRC Connecion procedure is always the first and   */
                     /* uses small transaction value of 0                 */
                     WR_SET_TRANS_ID(*transId, ueCb->cellId, ueCb->ueIdx, 0);
                  }
                  break;
                  case C1_RRCCONREESTBCOMPL:
                  {
                  
                     NhuUL_DCCH_MsgTypc1 *c1 = &ulDcch->dcchMsg.message.val.c1;
                     rrcTransId = c1->val.rrcConReestbCompl.rrc_TrnsxnId.val;
                     /*-Small transaction is derived directly from ueCb as there can be only 1
                       Downlink CCCH transaction. For connection Setup Request default "0" is
                       used and for all others we store the transcation Id in dlcccTransId variable-*/
                     WR_SET_TRANS_ID(*transId, ueCb->cellId, ueCb->ueIdx, ueCb->dlcchTransId);
                  }
                  break;
                  case C1_ULINFOTFR:
                  {
                        *transId = WR_CREATE_NEW_TRANS;
                     }
                  break;
                  case C1_UECAPINFO:
                  {
                     NhuUL_DCCH_MsgTypc1 *c1 = &ulDcch->dcchMsg.message.val.c1;
                     rrcTransId = c1->val.ueCapInfo.rrc_TrnsxnId.val;
                     if(wrUmmGetTransId(ueCb, C1_UECAPENQUIRY, 
                                        rrcTransId, &smallTrans) != ROK)
                     {
                        *transId = 0;
                        RETVALUE(RFAILED);
                     }
                     WR_SET_TRANS_ID(*transId, ueCb->cellId, ueCb->ueIdx, 
                                                               smallTrans);
                  }
                  break;
                  case C1_SECURITYMODECOMPL:
                  {
                     NhuUL_DCCH_MsgTypc1 *c1 = &ulDcch->dcchMsg.message.val.c1;
                     rrcTransId = c1->val.securityModeCompl.rrc_TrnsxnId.val;
                     if(wrUmmGetTransId(ueCb, C1_SECURITYMODECMD, 
                              rrcTransId, &smallTrans) != ROK)
                     {
                        *transId = 0;
                        RETVALUE(RFAILED);
                     }
                     WR_SET_TRANS_ID(*transId, ueCb->cellId, ueCb->ueIdx, 
                           smallTrans);
                  }
                  break;
                  case C1_SECURITYMODEFAIL:
                  {
                     NhuUL_DCCH_MsgTypc1 *c1 = &ulDcch->dcchMsg.message.val.c1;
                     rrcTransId = c1->val.securityModeFail.rrc_TrnsxnId.val;
                     if(wrUmmGetTransId(ueCb, C1_SECURITYMODECMD, 
                              rrcTransId, &smallTrans) != ROK)
                     {
                        *transId = 0;
                        RETVALUE(RFAILED);
                     }
                     WR_SET_TRANS_ID(*transId, ueCb->cellId, ueCb->ueIdx, 
                           smallTrans);
                  }
                  break;
                  case C1_MEASUREMENTREPORT:
                  {
                     U8                  measId;
                     WrMeasIdNode        *measIdObj = NULLP;
                     NhuUL_DCCH_MsgTypc1 *c1 = &ulDcch->dcchMsg.message.val.c1;
                     NhuMeasurementReportcriticalExtns *criExtn = &c1->
                        val.measurementReport.criticalExtns;
                     measId = criExtn->val.c1.val.measurementReport_r8.measResults.measId.val;
                     if(RFAILED == wrUmmMeasGetMeasIdObj(measId, &measIdObj,
                              &ueCb->measCfg->measIdLst))
                     {
                        RLOG_ARG1(L_ERROR, DBG_CRNTI, ueCb->crnti, "MeasId "
                           "Object not found from UE Cb for measID[%d]", measId);
                        RETVALUE(RFAILED);
                     }
                     /* ccpu00133079: Multiple A2 handling */
                     if((measIdObj->measIdTyp == WR_UMM_MEAS_TYPE_HO) ||
                                  (measIdObj->measIdTyp == WR_UMM_MEAS_TYPE_REDIR))
                     {
                        if(wrUmmGetExistingTransId(WR_UMM_MEAS_TRANS, 
                                          &ueCb->transLst, transId) != ROK)
                        {
                           *transId = WR_CREATE_NEW_TRANS;
                        }
                     }
                     else if(measIdObj->measIdTyp == WR_UMM_MEAS_TYPE_PERD_ANR)
                     {
                        /*DRX QCI*/
                        if(ueCb->isGBRBearerExists == NOT_IN_USE)
                        {
                           if(wrUmmGetExistingTransId(WR_UMM_ANR_MEAS_TRANS, 
                                               &ueCb->transLst, transId) != ROK)
                           {
                              *transId = 0;
                              RETVALUE(RFAILED);
                           }
                        }               
                        else
                        {
                           RLOG0(L_ERROR,"Failed to create Event ANR as GBR bearer exist\n");
                           *transId = 0;
                           RETVALUE(RFAILED);
                        }
                        
                     }
                     /* Measurement report received for A3 Event ANR */ 
                     else if(measIdObj->measIdTyp == WR_UMM_MEAS_TYPE_EVENT_ANR)
                     {
                        if(ueCb->eventAnrTransId)
                        {
                           /* UE has already sent measurement report
                            * for A3,so transaction will be present*/
                           *transId = ueCb->eventAnrTransId;
                           RETVALUE(ROK);
                        }
                        /*DRX QCI*/
                        if(ueCb->isGBRBearerExists == NOT_IN_USE)
                        {
                           RLOG0(L_WARNING,"Message received for creating Event ANR\n");
                           *transId = WR_CREATE_NEW_TRANS;
                        }
                        else
                        {
                           RLOG0(L_ERROR,"Failed to create Event ANR as GBR bearer exist\n");
                           *transId = 0;
                           RETVALUE(RFAILED);
                        }
                     }
                     else if(measIdObj->measIdTyp == WR_UMM_MEAS_TYPE_CSFB)
                     {
                        if(wrUmmGetExistingTransId(WR_UMM_CSFB_REDIR_TRANS, &ueCb->transLst, transId) != ROK)
                        {
                           *transId = 0;
                           RETVALUE(RFAILED);
                        }
                     }
                     else if(measIdObj->measIdTyp == WR_UMM_MEAS_TYPE_CSFB_CCO)
                     {
                        if(wrUmmGetExistingTransId(WR_UMM_CSFB_CCO_TRANS, &ueCb->transLst, transId) != ROK)
                        {
                           *transId = 0;
                           RETVALUE(RFAILED);
                        }
                     }

                     else if(measIdObj->measIdTyp == WR_UMM_MEAS_TYPE_ECSFB)
                     {
                        if(wrUmmGetExistingTransId(WR_UMM_ECSFB_TRANS, &ueCb->transLst, transId) != ROK)
                        {
                           *transId = 0;
                           RETVALUE(RFAILED);
                        }
                     }
                     /* LTE_ADV_ABS starts */
                     else if (measIdObj->measIdTyp == WR_UMM_MEAS_TYPE_ABS)
                     {
                        *transId = WR_CREATE_NEW_TRANS;
                     }
                     /* LTE_ADV_ABS ends */
                     else if(measIdObj->measIdTyp == WR_UMM_MEAS_TYPE_PERD_STRONG_CELL)
                     {
                        if(wrUmmGetExistingTransId(WR_UMM_PERD_STRONG_CELL_MEAS_TRANS, 
                                          &ueCb->transLst, transId) != ROK)
                        {
                           *transId = WR_CREATE_NEW_TRANS;
                        }
                     }
                     else
                     {
                        *transId = 0;
                        RETVALUE(RFAILED);
                     }
                  }
                  break;
                  case C1_ULHOVRPREPTFR:
                  {
                     if(wrUmmGetExistingTransId(WR_UMM_ECSFB_TRANS, &ueCb->transLst, transId) != ROK)
                     {
                        RLOG1(L_ERROR, " ECSFB transaction not available for crnti = %d ", ueCb->crnti); 
                        *transId = 0;
                        RETVALUE(RFAILED);
                     }

                  }
                  break;
                  case C1_CSFBPARAMSRQSTCDMA2000:
                  {
                     if(ueCb->rrcCsfbTransId)
                     {
                        *transId = ueCb->rrcCsfbTransId;
                     }
                     else
                     {
                        *transId = WR_CREATE_NEW_TRANS;
                     }
                  }
                  break;
                  default :
                  {
                    *transId = 0;
                    RETVALUE(RFAILED);
                  }
               }
            }
         }
         break;
      }
      case WR_UMM_UU_CFM_PDU:
      {
         if(ueCb->hoTransId)
         {
            *transId = ueCb->hoTransId;
         }
         break;
      }
   }

   RETVALUE(ROK);
} /* end of wrUmmUuGetTransIdFromMsg */ 

/**
 *  @brief This function is responsible for identifying transaction type 
 *         based on the received message. All transactions start with 
 *         initiating messages
 *
 *      Function: wrUmmGetUuTransType
 *
 *          Processing steps:
 *          - switch to the message type
 *             - assign the transaction type to the pointer provided
 *             - return ROK
 *          - return RFAILED if invalid message type is received
 *  @param [in]  ueCb     : pointer to the Ue control block
 *  @param [in]  msg      : pointer to the incomimng geessa
 *  @param [in]  transTyp : pointer to return the transaction type
 *  @return S16
    -# Success : ROK
    -# Failure : RFAILED
 */

/* ********************************************************************** */
/*                                                                        */
/* This function should be implemented if any RRC message can initiate    */
/*     a transaction                                                      */
/*                                                                        */
/* ********************************************************************** */
PUBLIC S16 wrUmmGetUuTransType
(
WrUeCb                       *ueCb,
WrUmmIncMsg                  *msg,
U32                          *transTyp
)
{
   U8                        messageType;

   if (msg->u.uuIndPdu->sdu.msgCategory == NHU_MSG_ULCCCH)
   {
      NhuUL_CCCH_Msg         *ulCcchPdu;

      ulCcchPdu   = &msg->u.uuIndPdu->sdu.m.ulCcchMsg.ccchMsg;
      messageType = ulCcchPdu->message.val.c1.choice.val;

      switch(messageType)
      {
         case C1_RRCCONRQST:
         {
            *transTyp = WR_UMM_RRC_SETUP_TRANS;
            break;
         }
         case C1_RRCCONREESTBRQST:
         {
            /*ccpu00127802*/
            RLOG0(L_DEBUG, "REESTABLISHMENT RECEIVED");
            *transTyp = WR_UMM_RRC_REESTAB_TRANS;
            break;
         }
         default:
         {
            *transTyp = WR_UMM_TRANS_UKNW;
            break;
         }
      }
   }
   else if (msg->u.uuIndPdu->sdu.msgCategory == NHU_MSG_ULDCCH)
   {
      NhuUL_DCCH_Msg         *ulDcchPdu;

      ulDcchPdu = &msg->u.uuIndPdu->sdu.m.ulDcchMsg.dcchMsg;
      messageType = ulDcchPdu->message.val.c1.choice.val;
      switch (messageType)
      {
         case C1_ULINFOTFR:
         {
            RLOG_ARG0(L_EVENT, DBG_CRNTI,ueCb->crnti,"[OTA] Received" 
                  " ULInformationTransfer");
            *transTyp = WR_UMM_UL_NAS_TRANS;
            break;
         }
         case C1_MEASUREMENTREPORT:
         {
            U8                  measId;
            WrMeasIdNode        *measIdObj = NULLP;
            NhuUL_DCCH_MsgTypc1 *c1 = &ulDcchPdu->message.val.c1;
            NhuMeasurementReportcriticalExtns *criExtn = &c1->
               val.measurementReport.criticalExtns;
            measId = criExtn->val.c1.val.measurementReport_r8.measResults.measId.val;

            if(RFAILED == wrUmmMeasGetMeasIdObj(measId, &measIdObj,
                     &ueCb->measCfg->measIdLst))
            {
               RLOG_ARG1(L_ERROR, DBG_CRNTI, ueCb->crnti,"MeasId Object not "
                  "found from UE Cb for measID[%d]", measId);
               RETVALUE(RFAILED);
            }
            if(measIdObj->measIdTyp == WR_UMM_MEAS_TYPE_EVENT_ANR)
            {
               *transTyp = WR_UMM_EVENT_ANR_TRANS;
            }
            else if((measIdObj->measIdTyp == WR_UMM_MEAS_TYPE_HO) || 
                    (measIdObj->measIdTyp == WR_UMM_MEAS_TYPE_ABS) ||
                    (measIdObj->measIdTyp == WR_UMM_MEAS_TYPE_REDIR))
            {
               *transTyp = WR_UMM_MEAS_TRANS;
            }
            else if(measIdObj->measIdTyp == WR_UMM_MEAS_TYPE_CSFB)
            {
               *transTyp = WR_UMM_CSFB_REDIR_TRANS;
            }
            else if(measIdObj->measIdTyp == WR_UMM_MEAS_TYPE_ECSFB)
            {
               *transTyp = WR_UMM_ECSFB_TRANS;
            }
            else if(measIdObj->measIdTyp == WR_UMM_MEAS_TYPE_PERD_STRONG_CELL)
            {
               *transTyp = WR_UMM_PERD_STRONG_CELL_MEAS_TRANS;
            }
            else
            {
               RLOG_ARG1(L_ERROR, DBG_CRNTI, ueCb->crnti,"MeasId Object not "
                  "found from UE Cb for measID[%d]", measId);
               RETVALUE(RFAILED);
            }
            break;
         }
         case C1_CSFBPARAMSRQSTCDMA2000:
         {
            *transTyp = WR_UMM_RRC_CSFB_TRANS;
            break;
         }
         default:
         {
             *transTyp = WR_UMM_TRANS_UKNW;
             break;
         }       
      }
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
