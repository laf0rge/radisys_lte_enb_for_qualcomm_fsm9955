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
               

     File:     wr_umm_anrmeas.c

     Sid:      fc_umm_rrccsfb.c@@/main/TeNB_Main_BR/4 - Mon Aug 11 16:52:56 2014

     Prg:      Sriky 

*********************************************************************21*/

/* header include files (.h) */
static const char* RLOG_MODULE_NAME="UMM";
static int RLOG_MODULE_ID=32;
static int RLOG_FILE_ID=123;

#include "wr.h"
#include "wr_utils.h"
#include "wr_umm.h"
#include "wr_umm_rrc_msg.h"
#include "wr_umm_trans.h"
#include "wr_ifm_schd.h"
#include "wr_ifm_rrc.h"
#include "nhu_asn.x"
#include "wr_lwr.h"
#include "wr_umm_mobparms.h"
#include "wr_ifm_dam.h"/*ccpu00138576*/

#ifdef __cplusplus
extern "C" {
#endif


#ifdef __cplusplus
}
#endif /* __cplusplus */

/** @brief This function is responsible for building RRC CSFB response message
 *  to UE.
 * @details
 *
 *  Function:wrUmmRrcFillCsfbRsp   
 *
 *  Processing steps:
 *    - Build RRC CSFB response message.
 *
 *  @param[in]  ueCb: Pointer to Ue Cb.
 *  @param[in]  memCp: pointer to the Memory control pointer.
 *  @param[out] rrcCsfbRsp: pointer to RRC CSFB response message.
 *  @return S16
 *       SUCCESS : ROK
 *       FAILED  : RFAILED
 */
PRIVATE S16 wrUmmRrcFillCsfbRsp
(
 WrUeCb                      *ueCb,
 CmMemListCp                 *memCp,
 NhuDatReqSdus               *rrcCsfbRsp
)
{
   WrUmmCellCb                     *cellCb;
   NhuDL_DCCH_Msg                  *dlMsg;
   NhuCSFBParamsRspCDMA2000        *csfbParamsRsp;
   NhuCSFBParamsRspCDMA2000_r8_IEs  *csfbParamsRsp_r8;

   
   WR_UMM_GET_CELLCB(cellCb, ueCb->cellId)
   if (cellCb == NULLP)
    {
       /* Log the error through an error debug */
       RLOG1(L_ERROR, " Error in getting CELL CB for cellId = %lu ", (U32)ueCb->cellId);
       RETVALUE(RFAILED);
    }
   rrcCsfbRsp->sdu.isUeCfgPres    = FALSE;
   rrcCsfbRsp->sdu.ueMsg.isMsgDed = TRUE;
   rrcCsfbRsp->sdu.ueMsg.msg.dedMsg.dlDcchMsg.pdcpId = WR_PDCP_ID_SRB1;
   dlMsg = &(rrcCsfbRsp->sdu.ueMsg.msg.dedMsg.dlDcchMsg.m.dcchMsg);

   wrFillTknU8(&dlMsg->pres, PRSNT_NODEF);
   wrFillTknU8(&dlMsg->message.choice, DL_DCCH_MSGTYP_C1);
   wrFillTknU8(&dlMsg->message.val.c1.choice, C1_CSFBPARAMSRSPCDMA2000);

   /* RRC CSFB parameter response */
   csfbParamsRsp = &(dlMsg->message.val.c1.val.csfbParamsRspCDMA2000);
   wrFillTknU8(&csfbParamsRsp->pres, PRSNT_NODEF);
   
   wrFillTknU32(&csfbParamsRsp->rrc_TrnsxnId, 0);
   wrFillTknU8(&csfbParamsRsp->criticalExtns.choice, 
                          CRITICALEXTNS_CSFBPARAMSRSPCDMA2000_R8);

   csfbParamsRsp_r8 = &(csfbParamsRsp->criticalExtns.val.csfbParamsRspCDMA2000_r8);
   wrFillTknU8(&csfbParamsRsp_r8->pres, PRSNT_NODEF);

    if(ueCb->eCsfbRandNum.pres == TRUE)
    {
       ueCb->eCsfbRandNum.pres = FALSE;
    }
   wrUmmGetRand32(ueCb);

   WR_FILL_BIT_STR_VAL(csfbParamsRsp_r8->rand, 32,
         ueCb->eCsfbRandNum.val, rrcCsfbRsp);
/*   wrFillTknStrBSXL(&csfbParamsRsp_r8->rand, 32,
         (U8 *)&ueCb->eCsfbRandNum.val, memCp);
*/
   wrFillTknStrOSXL(&csfbParamsRsp_r8->mobilityParams,cellCb->mobParam1xOct.len,
                        (U8 *)cellCb->mobParam1xOct.val, memCp);
   csfbParamsRsp_r8->nonCriticalExtn.pres.pres = NOTPRSNT;

   RETVALUE(ROK);
}
/** @brief This function is responsible for sending  RRC CSFB response message
 *  to UE.
 * @details
 *
 *  Function:wrUmmRrcCsfbSndCsfbRsp
 *
 *  Processing steps:
 *    - send RRC CSFB response message to UE.
 *
 *  @param[in]  transCb: pointer to the Transaction control block.
 *  @return S16
 *       SUCCESS : ROK
 *       FAILED  : RFAILED
 */
 
PRIVATE S16 wrUmmRrcCsfbSndCsfbRsp
(
  WrUmmTransCb                *transCb
)
{
   NhuDatReqSdus              *rrcCsfbRsp = NULLP;
   TRC2(wrUmmRrcCsfbSndCsfbRsp)

   WR_ALLOCEVNT(&rrcCsfbRsp, sizeof(NhuDatReqSdus));
   if(rrcCsfbRsp == NULLP)
   {
      RLOG0(L_FATAL, "Memory allocation failed. ");
      RETVALUE(RFAILED);
   }
   rrcCsfbRsp->hdr.cellId         = transCb->ueCb->cellId;
   rrcCsfbRsp->hdr.ueId           = transCb->ueCb->crnti;
   rrcCsfbRsp->hdr.transId        = transCb->transId;
   if(ROK != wrUmmRrcFillCsfbRsp(transCb->ueCb, &rrcCsfbRsp->memCp ,rrcCsfbRsp))
   {
       RLOG0(L_ERROR, " Filling of RRC CSFB RESPONSE message failed ");
       wrUmmTransComplete(transCb);
       RETVALUE(RFAILED);
   }
   RLOG_ARG0(L_EVENT, DBG_CRNTI, transCb->ueCb->crnti, "[OTA] Received"
         " CSFBParametersResponseCDMA2000");
   /* Call the function to dispatch the message over to RRC               */
   if(wrIfmRrcSndReqToUe(rrcCsfbRsp) != ROK)
   {
      RLOG0(L_ERROR, " wrIfmRrcSndReqToUe in RRC CSFB RESPONSE failed ");
      wrUmmTransComplete(transCb);
      RETVALUE(RFAILED);
   }

   RETVALUE(ROK);
}
/** @brief This function is responsible for Processing RRC message
 *
 * @details
 *
 *  Function:wrUmmRrcCsfbPrcRrcMsg
 *
 *  Processing steps:
 *    - Process RRC Message
 *
 *  @param[in]  transCb: pointer to the Transaction control block.
 *  @return S16
 *       SUCCESS : ROK
 *       FAILED  : RFAILED
 */
 
 PRIVATE S16 wrUmmRrcCsfbPrcRrcMsg
 (
   WrUmmTransCb                 *transCb
 )
 {
    NhuUlDcchMsg                        *ulDcch;
    NhuUL_DCCH_MsgTypc1                 *c1;

    ulDcch = &transCb->msg->u.uuIndPdu->sdu.m.ulDcchMsg;
    c1 = &ulDcch->dcchMsg.message.val.c1;
    switch(c1->choice.val)
    {
       case C1_CSFBPARAMSRQSTCDMA2000:
       {
          /*Reset INACTIVITY TIMER by setting dataRcvd flag to TRUE*/
          wrIfmDamSetDataRcvdFlag(transCb->ueCb->cellId, transCb->ueCb->crnti);

          wrUmmRrcCsfbSndCsfbRsp(transCb);
          break;
       }
       default:
       {
          RLOG0(L_ERROR, " Invalid Message received ");
          RETVALUE(RFAILED);
       }
    }
    RETVALUE(ROK);
 }
/** @brief This function is responsible for receive and handling RRC CSFB 
 * parameter request  messages which is received from UE.
 *
 * @details
 *
 *  Function:wrUmmRrcCsfbProcessor
 *
 *  Processing steps:
 *    - Handle RRC CSFB parameter request  message and send RRC CSFB parameter
 *       response message with mobility params information.
 *
 *  @param[in]  transCb: pointer to the Transaction control block.
 *  @return S16
 *       SUCCESS : ROK
 *       FAILED  : RFAILED
 *
 */
 PUBLIC S16 wrUmmRrcCsfbProcessor
 (
 WrUmmTransCb                 *transCb
 )
{
   WrUmmIncMsg                  *msg       = transCb->msg;
   TRC2(wrUmmRrcCsfbProcessor)

      switch(msg->msgTyp)
      {
         case WR_UMM_UU_IND_PDU:
            {
               wrUmmRrcCsfbPrcRrcMsg(transCb);
               break;
            }
         case WR_UMM_RRC_DAT_CFM:
            {
               wrUmmTransComplete(transCb);
               break;
            }
         default:
            {
               RLOG1(L_ERROR, " SWERROR : Received invalid message = %d ",msg->msgTyp);
               RETVALUE(RFAILED);
            } 
      }

   RETVALUE(ROK);
}

 /** @brief This function is responsible for deallocate transaction related
 * information after transaction complete.
 *
 * @details
 *
 *  Function:wrUmmRrcCsfbTransRel
 *
 *  Processing steps:
 *    - Deallocates transaction information.
 *
 *  @param[in]  transCb: pointer to the Transaction control block.
 *  @return S16
 *       SUCCESS : ROK
 *       FAILED  : RFAILED
 */
PUBLIC S16 wrUmmRrcCsfbTransRel
(
 WrUmmTransCb                *transCb
)
{
   TRC2(wrUmmRrcCsfbTransRel)
   transCb->ueCb->rrcCsfbTransId = 0;
   RETVALUE(ROK);
}

/** @brief This function is called when a new Transaction is created for any
 * other procedure. The new transaction is passed to this function and this
 * function determines the next course of action based on the new transaction
 *
 * @details
 *
 *     Function: wrUmmRrcCsfbNewTransProcessor
 *
 *         Description:
 *         This function is called when a new Transaction is
 *         created for any other procedure. The new transaction is passed to
 *         this function and this function determines the next course of action
 *         based on the new transaction
 *
 * @param[in]  transCb    : transaction information
 * @param[in]  incTrans   : incoming transaction information
 * @return S16
 *  -#Success : ROK
 *  -#Failure : RFAILED
 */

PUBLIC S16 wrUmmRrcCsfbNewTransProcessor
(
 WrUmmTransCb                *transCb,
 WrUmmTransCb                *incTrans
)
{
   TRC2(wrUmmRrcCsfbNewTransProcessor)
      switch(incTrans->transTyp)
      {
         case WR_UMM_RRC_REESTAB_TRANS:
            {
               RLOG0(L_DEBUG, " Reestablishment received \
                   Moving RRC Csfb trans into Reestblishment deffered list ");
               RETVALUE(WR_UMM_TRANS_DEFER_ONGOING);
            }
      }

   RETVALUE(WR_UMM_TRANS_OK);
}

/** @brief This function is called by the reestabshment transaction to intimate
 *   RRC CSFB transaction about reception of reestablishment complete message.
 *   RRC CSFB transaction will take a decision to stop reestablishment transaction
 *   by sending "DEQUEUE_CSFB" .
 *
 * @details
 *
 *     Function: wrUmmRrcCsfbPrcDeferTrans
 *
 *        Description:
 *         This function is called from the Transaction that has Rrc CSFB
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
PUBLIC S16 wrUmmRrcCsfbPrcDeferTrans
(
 WrUmmTransCb                *transCb,
 WrUmmTransCb                *defTransCb
 )
{
   NhuUL_DCCH_Msg            *ulDcchPdu = NULLP;
   U8                        messageType = 0;

   TRC2(wrUmmCsfbPrcDeferTrans)
   if(transCb->msg->msgTyp != WR_UMM_UU_IND_PDU)
   {
      RETVALUE(WR_UMM_TRANS_OK);
   }
   ulDcchPdu = &(transCb->msg->u.uuIndPdu->sdu.m.ulDcchMsg.dcchMsg);
   messageType = ulDcchPdu->message.val.c1.choice.val;
   switch(messageType)
   {
      case C1_RRCCONREESTBCOMPL:
         {
            RETVALUE(WR_UMM_STOP_OLD_TRANSACTION);
         }
   }
   RETVALUE(WR_UMM_TRANS_OK);
}
/********************************************************************30**

           End of file:     fc_umm_rrccsfb.c@@/main/TeNB_Main_BR/4 - Mon Aug 11 16:52:56 2014

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

