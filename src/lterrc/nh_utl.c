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



/**********************************************************************

     Name:     LTE-RRC Layer 
  
     Type:     C Source file
  
     Desc:     LTE RRC Utility Functions.

     File:     nh_utl.c

     Sid:      ni_utl.c@@/main/3 - Fri Jul  1 01:13:23 2011

     Prg:      gm

**********************************************************************/

/** @defgroup utl_group Utility Module
  * @{
  */



/* header include files (.h) */
static const char* RLOG_MODULE_NAME="RRC";
static int RLOG_MODULE_ID=512;
static int RLOG_FILE_ID=98;
#include "envopt.h"        /* Environment options */
#include "envdep.h"        /* Environment dependent */
#include "envind.h"        /* Environment independent */

#include "gen.h"           /* General */
#include "ssi.h"           /* System services */

#include "cm_tkns.h"       /* Common Token defines */
#include "cm_llist.h"      /* Common Link List defines */
#include "cm_hash.h"       /* Common Hashlist defines */
#include "cm_mblk.h"       /* Common memory allocation library defines */
#include "cm_pasn.h"       /* Common Pasn defines */

#include "cm_lte.h"        /* common LTE defines */
#include "kwu.h"           /* KWU Interface defines */
#include "ckw.h"           /* CKW Interface defines */
#include "crg.h"           /* CRG Interface defines */
#include "cpj.h"           /* CPJ Interface defines */
#include "pju.h"           /* PJU Interface defines */

#include "nhu.h"           /* NHU Interface defines */
#include "nhu_asn.h"       /* NHU ASN defines */
#include "lnh.h"           /* LNH Interface defines */
#include "nh.h"            /* NH defines */

/* header/extern include files (.x) */
#include "gen.x"           /* General */
#include "ssi.x"           /* System services */
#include "cm5.x"           /* Timer services */

#include "cm_tkns.x"       /* Common Token definitions */
#include "cm_llist.x"      /* Common Link List definitions */
#include "cm_hash.x"       /* Common Hashlist definitions */
#include "cm_lib.x"        /* Common Library definitions */
#include "cm_mblk.x"       /* Common Memory allocation */
#include "cm_pasn.x"       /* Common Hashlist definitions */


#include "cm_lte.x"        /* Common LTE includes */
#include "kwu.x"           /* KWU Interface includes */
#include "ckw.x"           /* CKW Interface includes */
#include "crg.x"           /* CRG Interface includes */
#include "cpj.x"           /* CDM Interface includes */
#include "pju.x"           /* DMU Interface includes */

#include "nhu.x"           /* NHU Interface includes */
#include "nhu_asn.x"       /* NHU ASN includes */
#include "lnh.x"           /* LNH Interface includes */
#include "nh.x"            /* NH defines */


/* 
 * Transport Block size table for BCCH and PCCH messages as per spec 36.213.
 * For BCCH and PCCH Nprb = 2 or 3, and  Itbs = 0-26. 
 * This table is in sorted order and duplicate entries has been removed.
 */
PUBLIC U32 NhBcchPcchTbSzTbl[NH_BCCH_PCCH_TBSZTBL_SIZE] = 
{
   32, 56, 72, 88, 104, 120, 144, 176, 208, 224, 256, 296, 328, 376, 392, 440, 
   456, 488, 504, 552, 584, 600, 632, 680, 696, 744, 776, 840, 904, 968, 1000, 
   1064, 1128, 1160, 1192, 1256, 1288, 1384, 1480, 1608, 1736, 1800, 1864, 2216
};


/* local defines */

/* local typedefs */

/* local externs */


PRIVATE S16 nhUTLUpdNhCntr ARGS((U32 *nhCntr, Cause evntCause));

PRIVATE S16 nhUTLValidateTransId ARGS((NhuDatReqSdus  *datReqSdus, NhActvUeCb  
              *ueCb, U8 *msgTransId, NhuErrorInfo *errInfo, U8 *event, 
              U8 *mesgType));
PRIVATE S16 nhUTLValidateRbOrPdcpId ARGS((NhId  nhIdentity, 
              NhCellAndUeCb *cellAndUeCb, NhuErrorInfo *errInfo));
PRIVATE S16 nhUTLValidateLogChTrchMap ARGS((NhuCellCfgInfo *cellCfgInfo));

PRIVATE S16 nhUTLValidateCfgTypeSetup ARGS((NhuCellCfgReqSdus *cellCfgReqSdus,
              NhCellCb *cellCb, NhuErrorInfo   *errInfo));

#ifdef NHU_MULT_SAP_SUPPRT
PRIVATE S16 nhUTLValidateMsapInfo ARGS((NhuCellCfgInfo *cellCfgInfo));
#endif /* NHU_MULT_SAP_SUPPRT */

#ifdef NH_LOG_TO_FILE
PRIVATE S16 nhStrcpy ARGS((S8 *tgt, CONSTANT S8 *src));
PRIVATE S16 nhStrcat ARGS((S8 *tgt, S8 *src));
#endif /* NH_LOG_TO_FILE */



/**
 * @brief Encodes and sends the message to the lower layer.
 * 
 * @details 
 * 
 *
 * This function encodes the message and sends the message to
 * PDCP or RLC layer. 
 * 
 * 
 * 
 * @param[in] cellAndUeCb      Cell and UE context
 * @param[in] nxTransId        RRC User Transaction ID
 * @param[in] msgPduAndType    Message Contents and Message Type
 * @param[in] datReqParam      PIM parameters for lower layer configuration
 * @param[out] errInfo         Pointer to the error information to 
 *                             be filled (for any error)
 *
 * @return S16
 *  -# ROK
 *  -# RFAILED
*/
#ifdef ANSI
PUBLIC S16 nhUTLEncAndSnd
(
NhCellAndUeCb       *cellAndUeCb,
U32                 nxTransId,
NhMsgPduAndType     *msgPduAndType,
NhDatReqParam       *datReqParam,
NhuErrorInfo        *errInfo
)
#else /* ANSI */
PUBLIC S16 nhUTLEncAndSnd(cellAndUeCb, nxTransId, msgPduAndType, datReqParam, 
                          errInfo)
NhCellAndUeCb      *cellAndUeCb;
U32                 nxTransId;
NhMsgPduAndType     *msgPduAndType;
NhDatReqParam       *datReqParam;
NhuErrorInfo        *errInfo;
#endif /* ANSI */
{
   Buffer      *nhEncBuf;        /* Buffer to store encoded buffer */
   U32         length;
   S16         retVal;
   MuiInfo     *muiCb = NULLP;

   TRC2(nhUTLEncAndSnd);

   RLOG0(L_DEBUG,"nhUTLEncAndSnd called.");

   /* If isMsgEncoded is set to TRUE, skip encode. */
   if(datReqParam->isMsgEncoded == TRUE)
   {
      nhEncBuf = (Buffer *)msgPduAndType->encBuf;
   }
   else
   {
      /* Call EDM module function to Encode the message 
       * nhEncBuf is allocated internally by nhEDMEncReq */
      retVal = nhEDMEncReq(msgPduAndType->pdu, msgPduAndType->type, 
                          &nhEncBuf, &length);
      if (ROK != retVal)
      {
         NH_FILL_NHU_ERROR(*errInfo, NHU_ERRTYPE_ENC_FAILED, NHU_ERRCAUSE_NONE);
         RETVALUE(retVal);
      }
   }

   /* PIM information is filled here */
   if(TRUE == datReqParam->isPdcpDatReq)
   {
      retVal = nhDBMAddMuiCb(cellAndUeCb->ueCb, &muiCb, nxTransId);
      if (ROK != retVal)
      {
         RLOG0(L_ERROR,"nhDBMAddMuiCb Failed.");
         /* free the encoded buffer */
         NH_FREEMBUF(nhEncBuf);
         RETVALUE(retVal);
      }

      datReqParam->u.pdcpParam.mui = muiCb->mui;
   }

   /* Send Encoded message to RLC or PDCP */
   retVal = nhPIMSendUeMsg(cellAndUeCb, datReqParam, nhEncBuf);
   if (ROK != retVal) 
   {
      RLOG0(L_ERROR,"nhPIMSendUeMsg Failed.");
      if(TRUE == datReqParam->isPdcpDatReq)
      {
         nhDBMDelMuiCb(cellAndUeCb->ueCb, &muiCb);
      }
      /* Free the encoded buffer */
      RETVALUE(retVal);
   }

   RETVALUE(ROK);
} /* nhUTLEncAndSnd */



/**
 * @brief Encodes and saves message in the UE context.
 * 
 * @details 
 * 
 *
 *
 * This function encodes the message and stores the message in
 * the UE transaction control block.  It also stores lower layer
 * information which must send the messsage. 
 *
 * 
 *
 * @param[in] msgPduAndType    Message Contents and Message Type
 * @param[in] datReqParam      PIM parameters for lower layer configuration
 * @param[in] transCtrlBlk          UE Transaction Context
 * @param[out] errInfo         Pointer to the error information to 
 *                             be filled (for any error)
 *
 * @return S16
 *  -# ROK
 *  -# RFAILED
*/
#ifdef ANSI
PUBLIC S16 nhUTLEncAndStoreMsg
(
NhMsgPduAndType     *msgPduAndType,
NhDatReqParam       *datReqParam,
TransCb             *transCtrlBlk,
NhuErrorInfo        *errInfo
)
#else /* ANSI */
PUBLIC S16 nhUTLEncAndStoreMsg(msgPduAndType, datReqParam,  
                               transCtrlBlk, errInfo)
NhMsgPduAndType     *msgPduAndType;
NhDatReqParam       *datReqParam;
TransCb             *transCtrlBlk;
NhuErrorInfo        *errInfo;
#endif /* ANSI */
{
   Buffer    *nhEncBuf; /* Buffer to store encoded buffer */
   U32       length;
   S16       retVal;

   TRC2(nhUTLEncAndStoreMsg);
   
   RLOG0(L_DEBUG,"nhUTLEncAndStoreMsg called.");

   /* Call EDM module function to Encode the message 
   * nhEncBuf is allocated internally by nhEDMEncReq */
   retVal = nhEDMEncReq(msgPduAndType->pdu, msgPduAndType->type, 
                        &nhEncBuf, &length);
   if (ROK != retVal)
   {
         NH_FILL_NHU_ERROR(*errInfo, NHU_ERRTYPE_ENC_FAILED, NHU_ERRCAUSE_NONE);
         RETVALUE(retVal);
   }

   /* Store the encoded message and lower layer parameters in transCtrlBlk */
   transCtrlBlk->storedMsg = nhEncBuf;   

   /* Allocate memory and store PIM information */
   NH_ALLOC(NH_REG, NH_POOL, &transCtrlBlk->lowerLyrParam, 
                    sizeof(NhDatReqParam));

   if(NULLP == transCtrlBlk->lowerLyrParam)
   {
      RLOG0(L_FATAL,"Memory Allocation failed for storing PIM information.");

      transCtrlBlk->storedMsg = NULLP;
      /* free the encoded buffer */
      NH_FREEMBUF(nhEncBuf);
      RETVALUE(ROUTRES);
   }

   NH_MEM_COPY((U8 *)transCtrlBlk->lowerLyrParam, (U8 *) datReqParam, 
                                        sizeof(NhDatReqParam));

   RETVALUE(ROK);
} /* nhUTLEncAndStoreMsg */




/**
 * @brief Sends stored CCCH/DCCH message to the lower layers.
 * 
 * @details 
 * 
 *
 *
 * This function sends the stored message to the lower layers using
 * stored lower layer information. The stored information is 
 * present in the transaction control block. It sends only CCCH and DCCH
 * messages. It updates the message statistics. 
 * 
 * 
 * 
 * 
 * @param[in] cellAndUeCb  Cell and UE Context
 * @param[in] transCtrlBlk      UE Transaction Context
 *
 * @return S16
 *  -# ROK
 *  -# RFAILED
*/
#ifdef ANSI
PUBLIC S16 nhUTLSendStoredMsg
(
NhCellAndUeCb *cellAndUeCb,
TransCb       *transCtrlBlk
)
#else /* ANSI */
PUBLIC S16 nhUTLSendStoredMsg(cellAndUeCb, transCtrlBlk)
NhCellAndUeCb *cellAndUeCb;
TransCb       *transCtrlBlk;
#endif /* ANSI */
{
   MuiInfo     *muiCb = NULLP;
   S16         retVal;
   Buffer      *encBuf;

   TRC2(nhUTLSendStoredMsg);

   RLOG0(L_DEBUG,"nhUTLSendStoredMsg called.");

   /* PIM information is filled here */
   if(TRUE == transCtrlBlk->lowerLyrParam->isPdcpDatReq)
   {
      retVal = nhDBMAddMuiCb(cellAndUeCb->ueCb, &muiCb, 
                             transCtrlBlk->nxTransId);
      if (ROK != retVal)
      {
         RLOG0(L_ERROR,"nhDBMAddMuiCb Failed.");
         /* free the encoded buffer and PIM information */
         NH_FREEMBUF(transCtrlBlk->storedMsg);
         NH_FREE(NH_REG, NH_POOL, transCtrlBlk->lowerLyrParam, 
                  sizeof(NhDatReqParam));
         RETVALUE(retVal);
      }

      transCtrlBlk->lowerLyrParam->u.pdcpParam.mui = muiCb->mui;
   }

   /* Copy encoded buffer from TransCb, so TransCb can be deleted. 
    * Encoded buffer is deleted by the corresponding lower layer.
    * LowerLayerParam is used only in PIM. */
   encBuf = transCtrlBlk->storedMsg;
   transCtrlBlk->storedMsg = NULLP;

   /* Send encoded message to RLC or PDCP */
   retVal = nhPIMSendUeMsg(cellAndUeCb, transCtrlBlk->lowerLyrParam, encBuf);
   if (ROK != retVal) 
   {
      RLOG0(L_ERROR,"nhPIMSendUeMsg Failed.");

      if(TRUE == transCtrlBlk->lowerLyrParam->isPdcpDatReq)
      {
         nhDBMDelMuiCb(cellAndUeCb->ueCb, &muiCb);
      }

      /* Free the encoded buffer and PIM information */
      NH_FREE(NH_REG, NH_POOL, transCtrlBlk->lowerLyrParam, 
                  sizeof(NhDatReqParam));
      RETVALUE(retVal);
   }

   /* Mark that Initial Security Activation Procedure
      Has been activated if this message was Security Mode */
   if (transCtrlBlk->cause == NH_MSGTYPE_SECMODCMD)
   {
      cellAndUeCb->ueCb->rbSecCfgBitMask |= NH_SEC_INIT;
   }

#ifndef NH_NO_STS
   /* Update the statistics */
   if(NH_MSGTYPE_CONNSETUP == transCtrlBlk->cause ||
      NH_MSGTYPE_CONNREESTAB == transCtrlBlk->cause)
   {
      nhUTLUpdCellPduSts(cellAndUeCb->cellCb, (MsgType) transCtrlBlk->cause, 
                         NH_STS_CAUSE_SUCCESS);
   }
   else
   {
      nhUTLUpdPduSts(cellAndUeCb->ueCb, (MsgType) transCtrlBlk->cause, 
                     NH_STS_CAUSE_SUCCESS);
   }
#endif /* NH_NO_STS */

   /* free the PIM information */
   NH_FREE(NH_REG, NH_POOL, transCtrlBlk->lowerLyrParam, 
                  sizeof(NhDatReqParam));

   RETVALUE(ROK);
} /* nhUTLSendStoredMsg */


/**
 * @brief Returns the UE transaction control block. 
 *
 * @details
 * This function returns UE transaction control block based on message type 
 * and RRC transaction identifier.
 * 
 * 
 * 
 * @param[in] ueCb          UE Context
 * @param[in] msgTransKey    Message Transaction Key
 *
 * @return TransCb*
 * -# Pointer to the Transaction Context upon success
 * -# NULLP upon failure
*/
#ifdef ANSI
PUBLIC TransCb *nhUTLGetTransCbFrmMsgTransId
(
NhActvUeCb   *ueCb,
U32         msgTransKey
)
#else /* ANSI */
PUBLIC TransCb *nhUTLGetTransCbFrmMsgTransId(ueCb, msgTransKey)
NhActvUeCb   *ueCb;
U32         msgTransKey;
#endif /* ANSI */
{
   TransCb   *transCb;
   U8         *key;
   S16      ret;

   TRC2(nhUTLGetTransCbFrmMsgTransId)

   transCb = NULLP;
   key = (U8 *)&msgTransKey;
   /* Find in the hash list */
   ret = nhHashListFind(&ueCb->msgTransHlCp, key, sizeof(U32), (PTR *)&transCb);

   if (ROK != ret)
      RETVALUE(NULLP);
   else
      RETVALUE(transCb);

}/* nhUTLGetTransCbFrmMsgTransId */


/**
 * @brief Returns the UE transaction control block. 
 *
 * @details
 * This function returns the UE transaction control block based on 
 * the RRC transaction identifier.
 * 
 * 
 * 
 * @param[in] ueCb       UE Context
 * @param[in] transId    Transaction Identifier 
 *
 * @return TransCb*
 * -# Pointer to the Transaction Context upon Success
 * -# NULLP upon Failure
*/

#ifdef ANSI
PUBLIC TransCb *nhUTLGetTransCbFrmNxTransId
(
NhActvUeCb   *ueCb,
NhTransId    transId
)
#else /* ANSI */
PUBLIC TransCb *nhUTLGetTransCbFrmNxTransId(ueCb, transId)
NhActvUeCb   *ueCb;
NhTransId    transId;
#endif /* ANSI */
{
   TransCb   *transCb=NULLP;
   CmLList   *tmp;
   S16       ret;

   TRC2(nhUTLGetTransCbFrmNxTransId)

   ret = ROK;

   /* Traverse the default list */
   CM_LLIST_FIRST_NODE(&(ueCb->transCbLst), tmp);

   while(NULLP != tmp)
   {
      transCb = (TransCb *)(tmp->node);
      if (transCb->nxTransId == transId)
      {
         RETVALUE(transCb);
      }
      /* Find out the next node */
      CM_LLIST_NEXT_NODE(&(ueCb->transCbLst), tmp);
   }

   RETVALUE(NULLP);

}/* nhUTLGetTransCbFrmNxTransId */


/**
 * @brief Returns the RRC transacion identifier.
 *
 * @details
 * This function gets the RRC transaction identifier by seeking into 
 * DL-CCCH, UL-CCCH, DL-DCCH, UL-DCCH messages as received on UIM/LIM.
 * 
 * 
 * 
 * @param[in] pdu         Message Contents
 * @param[in] msgChoice   RRC Message Type used in Pasn module
 * @param[in] sduType     RRC Message Logical Channel information 
 * @param[in] rrcTransId  Transaction identifier
 *
 * @return S16
 * -# ROK
 * -# RFAILED
*/
#ifdef ANSI
PUBLIC S16 nhUTLGetRrcTransId
(
Void       *pdu,
U8         msgChoice,
NhSduType  sduType,
U8         *rrcTransId
)
#else /* ANSI */
PUBLIC S16 nhUTLGetRrcTransId(pdu, msgChoice, sduType, rrcTransId)
Void       *pdu;
U8         msgChoice;
NhSduType  sduType;
U8         *rrcTransId;
#endif /* ANSI */
{
   NhuDL_CCCH_Msg *dlCcchMsg;
   NhuDL_DCCH_Msg *dlDcchMsg;
   NhuUL_DCCH_Msg *ulDcchMsg;

   TRC2(nhUTLGetRrcTransId)

   if(sduType == NH_SDU_DL_CCCH)
   {
      dlCcchMsg = (NhuDL_CCCH_Msg *) pdu;
      *rrcTransId = NH_INVALID_RRC_TRANS_ID;
      switch(msgChoice)
      {
         case C1_RRCCONSETUP:
               *rrcTransId = (U8) 
                    dlCcchMsg->message.val.c1.val.rrcConSetup.rrc_TrnsxnId.val;
               break;
         default:
               break;
      }
   } 
   else if(sduType == NH_SDU_DL_DCCH)
   {
      dlDcchMsg = (NhuDL_DCCH_Msg *) pdu;
      *rrcTransId = NH_INVALID_RRC_TRANS_ID;
      switch(msgChoice)
      {
         case C1_RRCCONRECFGN:
               *rrcTransId = (U8) 
                   dlDcchMsg->message.val.c1.val.rrcConRecfgn.rrc_TrnsxnId.val;
               break;
         case C1_RRCCONRLS:
               *rrcTransId = (U8) 
                   dlDcchMsg->message.val.c1.val.rrcConRls.rrc_TrnsxnId.val; 
               break;
         case C1_SECURITYMODECMD:
               *rrcTransId = (U8) 
                 dlDcchMsg->message.val.c1.val.securityModeCmd.rrc_TrnsxnId.val;
               break;
         default:
               break;
      }
   } 
   else if(sduType == NH_SDU_UL_DCCH)
   {
      ulDcchMsg = (NhuUL_DCCH_Msg *) pdu;
      *rrcTransId = NH_INVALID_RRC_TRANS_ID;
      switch(msgChoice)
      {
         case C1_RRCCONRECFGNCOMPL:
               *rrcTransId = (U8) 
               ulDcchMsg->message.val.c1.val.rrcConRecfgnCompl.rrc_TrnsxnId.val;
               break;
         case C1_RRCCONSETUPCOMPL:
               *rrcTransId = (U8) 
                ulDcchMsg->message.val.c1.val.rrcConSetupCompl.rrc_TrnsxnId.val;
               break;
         case C1_SECURITYMODECOMPL:
               *rrcTransId = (U8) 
               ulDcchMsg->message.val.c1.val.securityModeCompl.rrc_TrnsxnId.val;
               break;
         case C1_SECURITYMODEFAIL:
               *rrcTransId = (U8) 
               ulDcchMsg->message.val.c1.val.securityModeFail.rrc_TrnsxnId.val; 
               break;
         default:
               break;
      }
   } 
   else
   {
      RLOG0(L_ERROR, "Incorrect SDU type received");
      RETVALUE(RFAILED);
   }
   RETVALUE(ROK);
} /* nhUTLGetRrcTransId */



/**
 * @brief Returns the RRC message and event type.
 *
 * @details
 * This function returns the RRC message and event type based on the message 
 * choice and the SDU type.
 * 
 * 
 * 
 * @param[in] msgChoice  RRC Message Type used in Pasn module
 * @param[in] sduType    RRC Message Logical Channel information
 * @param[in] nhMsgType  RRC Message Type used in RRC modules
 * @param[in] event      RRC Event Type used in RRC modules
 *
 * @return S16
 * -# ROK
 * -# RFAILED
*/
#ifdef ANSI
PUBLIC S16 nhUTLGetMsgTypeAndEvntFrmSdu
(
U8          msgChoice,
NhSduType   sduType,
U8          *nhMsgType,
U8          *event
)
#else /* ANSI */
PUBLIC S16 nhUTLGetMsgTypeAndEvntFrmSdu(msgChoice, sduType, nhMsgType, event)
U8          msgChoice;
NhSduType   sduType;
U8          *nhMsgType;
U8          *event;
#endif /* ANSI */
{

   TRC2(nhUTLGetMsgTypeAndEvntFrmSdu)

      *nhMsgType = NH_IGNORE;
      switch(sduType)
      {
         case NH_SDU_DL_CCCH:
            {
               switch(msgChoice)
               {
                  case C1_RRCCONREESTBREJ:
                     *nhMsgType = NH_MSGTYPE_CONNREESTABREJ;
                     *event = NH_EVNT_UI_CONN_REESTB_REJ - NH_EVNT_LI_MAX_EVNTS;
                     break;
                  case C1_RRCCONREJ:
                     *nhMsgType = NH_MSGTYPE_CONNREJ;
                     *event = NH_EVNT_UI_CONN_REJ - NH_EVNT_LI_MAX_EVNTS;
                     break;
                  case C1_RRCCONSETUP:
                     *nhMsgType = NH_MSGTYPE_CONNSETUP;
                     *event = NH_EVNT_UI_CONN_SETUP - NH_EVNT_LI_MAX_EVNTS;
                     break;
                  case C1_RRCCONREESTB:
                     *nhMsgType = NH_MSGTYPE_CONNREESTAB;
                     *event = NH_EVNT_UI_CONN_REESTB - NH_EVNT_LI_MAX_EVNTS;
                     break;
                  default:
                     break;
               }
               break;
            }
         case NH_SDU_UL_CCCH:
            {
               switch(msgChoice)
               {
                  case C1_RRCCONREESTBRQST:
                     *nhMsgType = NH_MSGTYPE_CONNREESTABREQ;
                     *event = NH_EVNT_LI_RRC_REESTAB_REQ;
                     break;
                  case C1_RRCCONRQST:
                     *nhMsgType = NH_MSGTYPE_CONNREQ;
                     *event = NH_EVNT_LI_RRC_CON_REQ;
                     break;
                  default:
                     break;
               }
               break;
            }
         case NH_SDU_DL_DCCH:
            {
               switch(msgChoice)
               {
                  case C1_DLINFOTFR:
                     *nhMsgType = NH_MSGTYPE_DLTFR;
                     *event = NH_EVNT_UI_DL_DT - NH_EVNT_LI_MAX_EVNTS;
                     break;
                  case C1_RRCCONRECFGN:
                     *nhMsgType = NH_MSGTYPE_CONNRECFG;
                     *event = NH_EVNT_UI_RRC_CON_RECFG - NH_EVNT_LI_MAX_EVNTS;
                     break;
                  case C1_RRCCONRLS:
                     *nhMsgType = NH_MSGTYPE_CONNRLS;
                     *event = NH_EVNT_UI_RRC_CON_RLS - NH_EVNT_LI_MAX_EVNTS;
                     break;
                  case C1_SECURITYMODECMD:
                     *nhMsgType = NH_MSGTYPE_SECMODCMD;
                     *event = NH_EVNT_UI_SEC_MOD_CMD - NH_EVNT_LI_MAX_EVNTS;
                     break;
                  case C1_MOBILITYFROMEUTRACMD:
                     *nhMsgType = NH_MSGTYPE_MOBFRMEUTRAN;
                     *event = NH_EVNT_UI_MOB_FRM_EUTRA_CMD - 
                              NH_EVNT_LI_MAX_EVNTS;
                     break;
                  case C1_HANDOVERFROMEUTRAPREPRQST:
                     *nhMsgType = NH_MSGTYPE_HOFRMEUTRAPREPREQ;
                     *event = NH_EVNT_UI_HO_FRM_EUTRA_PRP_TFR - 
                              NH_EVNT_LI_MAX_EVNTS;
                     break;
                  case C1_CSFBPARAMSRSPCDMA2000:
                     *nhMsgType = NH_MSGTYPE_CSFBPARAMRSPCDMA;
                     *event = NH_EVNT_UI_CSFB_PARAM_RSP - NH_EVNT_LI_MAX_EVNTS;
                     break;
                  case C1_UECAPENQUIRY:
                     *nhMsgType = NH_MSGTYPE_UECAPENQ;
                     *event = NH_EVNT_UI_UE_CAP_ENQ - NH_EVNT_LI_MAX_EVNTS;
                     break;
                  case C1_COUNTERCHECK:
                     *nhMsgType = NH_MSGTYPE_COUNTCHK;
                     *event = NH_EVNT_UI_COUNT_CHK - NH_EVNT_LI_MAX_EVNTS;
                     break;
#ifdef LTERRC_REL9
/* Case added for UE information request r9*/
                  case C1_UEINFORQST_R9__1:
                     *nhMsgType = NH_MSGTYPE_UEINFOREQ;
                     *event = NH_EVNT_UI_UE_INFO_REQ - NH_EVNT_LI_MAX_EVNTS;
                     break;
#endif /* LTERRC_REL9 */
                  default:
                     break;
               }
               break;
            }
         case NH_SDU_UL_DCCH:
            {
               switch(msgChoice)
               {
                  case C1_RRCCONSETUPCOMPL:
                     *nhMsgType = NH_MSGTYPE_CONNSETUPCOMP;
                     *event = NH_EVNT_LI_RRC_CON_SETUP_CMP;
                     break;
                  case C1_RRCCONRECFGNCOMPL:
                     *nhMsgType = NH_MSGTYPE_CONNRECFGCOMP;
                     *event = NH_EVNT_LI_RRC_CON_RECFG_CMP;
                     break;
                  case C1_SECURITYMODECOMPL:
                     *nhMsgType = NH_MSGTYPE_SECMODCOMP;
                     *event = NH_EVNT_LI_SEC_MOD_CMP;
                     break;
                  case C1_SECURITYMODEFAIL:
                     *nhMsgType = NH_MSGTYPE_SECMODFAIL;
                     *event = NH_EVNT_LI_SEC_MOD_FAIL;
                     break;
                  case C1_ULINFOTFR:
                     *nhMsgType = NH_MSGTYPE_ULTFR;
                     *event = NH_EVNT_LI_UL_DT;
                     break;
                  case C1_MEASUREMENTREPORT:
                     *nhMsgType = NH_MSGTYPE_MEASUREMENTRPT;
                     *event = NH_EVNT_LI_RRC_MES_RPT;
                     break;
                  case C1_RRCCONREESTBCOMPL:
                     *nhMsgType = NH_MSGTYPE_CONNREESTABCOMP;
                     *event = NH_EVNT_LI_RRC_CON_RESTB_CMP;
                     break;
                  case C1_ULHOVRPREPTFR:
                     *nhMsgType = NH_MSGTYPE_ULHOPREPTFR;
                     *event = NH_EVNT_LI_UL_HO_PREP_TFR;
                     break;
                  case C1_CSFBPARAMSRQSTCDMA2000:
                     *nhMsgType = NH_MSGTYPE_CSFBPARAMREQCDMA;
                     *event = NH_EVNT_LI_CSFB_PARAM_REQ;
                     break;
                  case C1_UECAPINFO:
                     *nhMsgType = NH_MSGTYPE_UECAPINFO;
                     *event = NH_EVNT_LI_UE_CAP_INFO;
                     break;
                  case C1_COUNTERCHECKRSP:
                     *nhMsgType = NH_MSGTYPE_COUNTCHKRSP;
                     *event = NH_EVNT_LI_COUNT_CHK_RSP;
                     break;
#ifdef LTERRC_REL9
                     /* UE Information Response */
                  case C1_UEINFORSP_R9__2:
                     *nhMsgType = NH_MSGTYPE_UEINFORSP;
                     *event = NH_EVNT_LI_UE_INFO_RSP;
                     break;

                     /* Proximity Indication */
                  case C1_PROXIMITYIND_R9__2:
                     *nhMsgType = NH_MSGTYPE_PRXMTYIND;
                     *event = NH_EVNT_LI_PRXMTY_IND;
                     break;
#endif /* LTERRC_REL9 */
                  default:
                     break;
               }
               break;
            }
         default:
            RLOG0(L_ERROR, "Incorrect sdu type received");
            RETVALUE(RFAILED);
      }

   RETVALUE(ROK);
} /* nhUTLGetMsgTypeAndEvntFrmSdu */


#ifndef NH_NO_STS

/**
 * @brief  Updates the UE PDU statistics.
 * 
 * @details 
 * 
 * This function increments the statistics counters depending on 
 * message type and cause for the UE.
 *
 * @param[in] ueCb      UE Context
 * @param[in] mesgType   Message Type used in RRC modules
 * @param[in] evntCause     Statistics Update Cause: ASN.1 Violation or Success
 *
 * @return S16
 *  -# ROK
 *  -# RFAILED
*/
#ifdef ANSI
PUBLIC S16 nhUTLUpdPduSts
(
NhActvUeCb     *ueCb,   
MsgType        mesgType,
Cause          evntCause  
)
#else /* ANSI */
PUBLIC S16 nhUTLUpdPduSts(ueCb, mesgType, evntCause)
NhActvUeCb    *ueCb;   
MsgType       mesgType;    
Cause         evntCause;       
#endif /* ANSI */
{

   NhPduSts *nhPduSts;

   TRC2(nhUTLUpdPduSts);

   RLOG0(L_DEBUG,"Updating the PDU Statistics");
    
   nhPduSts = &(ueCb->ueSts.cfm.pdu);
   /*  switch on mesgType */ 
   switch(mesgType)
   {

      case NH_MSGTYPE_CONNSETUPCOMP: /* Conn. Setup complete */
         nhUTLUpdNhCntr(&nhPduSts->connSetupComplete, evntCause);
         break;      

      case NH_MSGTYPE_CONNRLS:       /* Connection Release */
         nhUTLUpdNhCntr(&nhPduSts->connRel, evntCause);
         break;
         
      case NH_MSGTYPE_CONNRECFG:     /* Connecton Re-config message */ 
         nhUTLUpdNhCntr(&nhPduSts->connReconfig, evntCause);
         break;  
         
      case NH_MSGTYPE_CONNRECFGCOMP: /* Connecton Re-config cmplete message */ 
         nhUTLUpdNhCntr(&nhPduSts->connReconfigCom, evntCause);
         break;  

      case NH_MSGTYPE_DLTFR:         /* DL direct transfer */
         nhUTLUpdNhCntr(&nhPduSts->dlInfoTransfer, evntCause);
         break;       

      case NH_MSGTYPE_ULTFR:         /* UL direct transfer */
         nhUTLUpdNhCntr(&nhPduSts->ulInfoTransfer, evntCause);
         break;                          

      case NH_MSGTYPE_SECMODCMD:      /* Security mode command */
         nhUTLUpdNhCntr(&nhPduSts->secModCmd, evntCause);
         break;

      case NH_MSGTYPE_SECMODCOMP:      /* Security Mode Complete */
         nhUTLUpdNhCntr(&nhPduSts->secModCmpl, evntCause);
         break;

      case NH_MSGTYPE_SECMODFAIL:      /* Security mode failure */
         nhUTLUpdNhCntr(&nhPduSts->secModFail, evntCause);
         break;

      case NH_MSGTYPE_COUNTCHK:      /* Counter Check */
         nhUTLUpdNhCntr(&nhPduSts->cntChk, evntCause);
         break;

      case NH_MSGTYPE_COUNTCHKRSP:    /* Counter Check Response */
         nhUTLUpdNhCntr(&nhPduSts->cntChkRes, evntCause);
         break;

      case NH_MSGTYPE_UECAPENQ:      /* UE Capability Enquiry */
         nhUTLUpdNhCntr(&nhPduSts->ueCapEnq, evntCause);
         break;

      case NH_MSGTYPE_UECAPINFO:    /* UE Capability Information */
         nhUTLUpdNhCntr(&nhPduSts->ueCapInfo, evntCause);
         break;

      case NH_MSGTYPE_MOBFRMEUTRAN:    /* Mobility from E-UTRA */
         nhUTLUpdNhCntr(&nhPduSts->mobFrmEUTRACmd, evntCause);
         break;

      case NH_MSGTYPE_HOFRMEUTRAPREPREQ:/* HO from E-UTRA Preparation Request
                                           - CDMA2000 */
         nhUTLUpdNhCntr(&nhPduSts->hoFrmEUTRAPrepReq, evntCause);
         break;

      case NH_MSGTYPE_ULHOPREPTFR:    /* UL HO preparation transfer - CDMA2000 */
         nhUTLUpdNhCntr(&nhPduSts->ulHOPrepTfr, evntCause);
         break;

      case NH_MSGTYPE_CSFBPARAMREQCDMA:  /* CSFB Parameters Request CDMA2000 */
         nhUTLUpdNhCntr(&nhPduSts->csfbParamReqCDMA, evntCause);
         break;

      case NH_MSGTYPE_CSFBPARAMRSPCDMA:  /* CSFB Parameters Response CDMA2000 */
         nhUTLUpdNhCntr(&nhPduSts->csfbParamResCDMA, evntCause);
         break;

      case NH_MSGTYPE_CONNREESTABCOMP:
        nhUTLUpdNhCntr(&nhPduSts->connReestabCom, evntCause);
        break;

#ifdef LTERRC_REL9
      case NH_MSGTYPE_UEINFOREQ: /*UE Information request Message*/
        nhUTLUpdNhCntr(&nhPduSts->ueInfoReq, evntCause);
        break;
        
      case NH_MSGTYPE_UEINFORSP: /*UE Information response Message*/
        nhUTLUpdNhCntr(&nhPduSts->ueInfoRsp, evntCause);
        break;
        
      case NH_MSGTYPE_PRXMTYIND: /*Proximity Indication Message*/
        nhUTLUpdNhCntr(&nhPduSts->prxmtyInd, evntCause);
        break;
#endif /* LTERRC_REL9 */

      default:
         RLOG4(L_DEBUG,"Invalid/Unhandled message type [%d] evnt[%d] UE CRNTI[%d] restState[%d] ", \
               mesgType, evntCause,ueCb->ueCbKey.crnti, ueCb->restbState);
         break;
   
   } /* end of switch(mesgType) */         
   
   RETVALUE(ROK);

} /* nhUTLUpdPduSts */ 

/**
 * @brief  Updates the statistics counters.
 * 
 * @details 
 * 
 * This function updates the counters for ASN.1 violation errors or success.
 * 
 *
 * @param[in] nhCntr    Statistics counter
 * @param[in] evntCause     Counter Update Cause: ASN.1 Violation or Success 
 *
 *
 * @return S16
 *  -# ROK
 *  -# RFAILED
*/
#ifdef ANSI
PRIVATE S16 nhUTLUpdNhCntr
(
U32         *nhCntr,
Cause       evntCause  
)
#else /* ANSI */
PRIVATE S16 nhUTLUpdNhCntr(nhCntr, evntCause)
U32         *nhCntr; 
Cause       evntCause;  
#endif /* ANSI */
{
   TRC2(nhUTLUpdNhCntr);

   RLOG0(L_DEBUG,"nhUTLUpdNhCntr called.");

   /*  switch on evntCause */ 
   switch(evntCause)
   {
      case NH_STS_CAUSE_SUCCESS:            /* Message successfully received */
      case NH_STS_CAUSE_MOB_FRM_EUTRA_FAIL:    /* Update Mobility failure counter */
      case NH_STS_CAUSE_HO_TO_EUTRA_SUCC:      /* Update HO to E-UTRA success counter */
          NH_VALIDATE_CNTR(nhCntr);
          (*nhCntr)++; 
          break;

      default:
         RLOG0(L_ERROR,"Invalid Cause.");
         break;      

   } /* end of switch(evntCause) */

   RETVALUE(ROK);

} /* nhUTLUpdNhCntr */

/**
 * @brief  Updates the cell PDU statistics.
 * 
 * @details 
 * 
 * This function increments the statistics counters depending on 
 * the message type and the cause for the cell.
 *
 * @param[in] cellCb   Cell context
 * @param[in] mesgType  Message Type used in RRC modules
 * @param[in] evntCause    Statistics Update Cause: ASN.1 Violation or Success
 *
 * @return S16
 *  -# ROK
 *  -# RFAILED
*/
#ifdef ANSI
PUBLIC S16 nhUTLUpdCellPduSts
(
NhCellCb   *cellCb,  
MsgType    mesgType,  
Cause      evntCause      
)
#else /* ANSI */
PUBLIC S16 nhUTLUpdCellPduSts(cellCb, mesgType, evntCause)
NhCellCb   *cellCb;
MsgType    mesgType;
Cause      evntCause;   
#endif /* ANSI */
{

   NhCellSts *cellSts;

   TRC2(nhUTLUpdCellPduSts);

   RLOG0(L_DEBUG,"Updating the Cell PDU Statistics.");
    
   cellSts = &(cellCb->cellSts);
   /*  switch on mesgType */ 
   switch(mesgType)
   {
      case NH_MSGTYPE_PAG:     /* Paging */
         nhUTLUpdNhCntr(&cellSts->pag, evntCause);
         break;      
      case NH_MSGTYPE_CONNREQ:     /* RRC Connection Request */
         nhUTLUpdNhCntr(&cellSts->connReq, evntCause);
         break;      
      case NH_MSGTYPE_CONNSETUP:     /* RRC Connection Setup */
         nhUTLUpdNhCntr(&cellSts->connSetup, evntCause);
         break;      
      case NH_MSGTYPE_CONNREESTABREQ:     /* RRC Reestablishment Request */
         nhUTLUpdNhCntr(&cellSts->connReestabReq, evntCause);
         break;      
      case NH_MSGTYPE_CONNREJ:     /* RRC Connection Reject */
         nhUTLUpdNhCntr(&cellSts->connReject, evntCause);
         break;      
      case NH_MSGTYPE_CONNREESTABREJ:     /* RRC Re-establishment Reject */
         nhUTLUpdNhCntr(&cellSts->connReestabRej, evntCause);
         break;     
      case NH_MSGTYPE_CONNREESTAB:
         nhUTLUpdNhCntr(&cellSts->connReestab,evntCause);
         break;
      case NH_MSGTYPE_CONNRECFGCOMP_HO: /* RRC Reconfiguration Complete during HO */
         nhUTLUpdNhCntr(&cellSts->hoToEUTRASucc, evntCause);
         break;      
      case NH_MSGTYPE_MOBFRMEUTRAN:     /* Re-establisment for mobility */
         nhUTLUpdNhCntr(&cellSts->hoFromEUTRAFail, evntCause);
         break;      
      default:
         RLOG0(L_ERROR,"Invalid Message Type.");
         break;      

   } /* end of switch(cause) */

   RETVALUE(ROK);

} /* nhUTLUpdCellPduSts */ 
#endif /* NH_NO_STS */


/**
 * @brief  Updates the general statistics.
 * 
 * @details 
 * 
 * This function increments the statistics counters depending on 
 * cause for the statistics.
 *
 * @param[in] stsType   Statistics Type
 *
 * @return S16
 *  -# ROK
 *  -# RFAILED
*/
#ifdef ANSI
PUBLIC S16 nhUTLUpdGenSts
(
CntrType    stsType
)
#else /* ANSI */
PUBLIC S16 nhUTLUpdGenSts(stsType)
CntrType    stsType;
#endif /* ANSI */
{

   TRC2(nhUTLUpdGenSts);

   RLOG0(L_DEBUG,"Updating the General Statistics.");
    
   /*  switch on stsType */ 
   switch(stsType)
   {
      case NH_STS_CAUSE_CON_SUCC:
         NH_VALIDATE_CNTR(&nhCb.genSts.numEstbCon);
         nhCb.genSts.numEstbCon++;
         break;
      case NH_STS_CAUSE_CON_FAIL:
         NH_VALIDATE_CNTR(&nhCb.genSts.numFailCon);
         nhCb.genSts.numFailCon++;
         break;
      case NH_STS_CAUSE_RECFG_SUCC:
         NH_VALIDATE_CNTR(&nhCb.genSts.numReConfigSucc);
         nhCb.genSts.numReConfigSucc++;
         break;
      case NH_STS_CAUSE_RECFG_FAIL:
         NH_VALIDATE_CNTR(&nhCb.genSts.numReConfigFail);
         nhCb.genSts.numReConfigFail++;
         break;
      case NH_STS_CAUSE_LOWER_LYR_FAIL:
         NH_VALIDATE_CNTR(&nhCb.genSts.numLwLyFail);
         nhCb.genSts.numLwLyFail++;
         break;
      case NH_STS_CAUSE_DEC_ERR:
         NH_VALIDATE_CNTR(&nhCb.genSts.numDecErr);
         nhCb.genSts.numDecErr++;
         break;
      case NH_STS_CAUSE_ENC_ERR:
         NH_VALIDATE_CNTR(&nhCb.genSts.numEncErr);
         nhCb.genSts.numEncErr++;
         break;
      case NH_STS_CAUSE_PDCP_ERR:
         NH_VALIDATE_CNTR(&nhCb.genSts.numPdcpErr);
         nhCb.genSts.numPdcpErr++;
         break;
      case NH_STS_CAUSE_RLC_ERR:
         NH_VALIDATE_CNTR(&nhCb.genSts.numRlcErr);
         nhCb.genSts.numRlcErr++;
         break;
      default:
         RLOG0(L_ERROR,"Invalid Statistics Type.");
         break;      

   } /* end of switch(cause) */

   RETVALUE(ROK);

} /* nhUTLUpdGenSts */ 



/** 
 * @brief Validates the transaction identifier in Data Request.
 * 
 * @details 
 * This function validates the following in a Data Request:
 * -# If there is a duplicate transaction already pending 
 *    in RRC based on the transaction identifier in the NHU
 *    header.
 * -# If there is a duplicate transaction pending in RRC 
 *    that matches the RRC transaction identifier in the
 *    UE message.
 *  
 *  
 *  
 * @param[in] datReqSdus   Data Request SDUs
 * @param[in] ueCb         UE Context
 * @param[in] msgTransId   Message Transaction identifier
 * @param[out] errInfo     Pointer to the error information to 
 *                         be filled (for any error)
 * @param[in] event        Message Event Type used in RRC modules
 * @param[in] messageType  Message Type used in RRC modules
 * 
 * @return S16 
 * -# ROK     Validation Success
 * -# RFAILED Validation Failure       
*/
#ifdef ANSI
PRIVATE S16 nhUTLValidateTransId
(
NhuDatReqSdus  *datReqSdus,
NhActvUeCb  *ueCb,
U8          *msgTransId,
NhuErrorInfo   *errInfo,
U8             *event,
U8             *messageType
)
#else /* ANSI */
PRIVATE S16 nhUTLValidateTransId(datReqSdus, ueCb, msgTransId, errInfo, 
                                 event, messageType)
NhuDatReqSdus  *datReqSdus;
NhActvUeCb  *ueCb;
U8          *msgTransId;
NhuErrorInfo   *errInfo;
U8             *event;
U8             *messageType;
#endif /* ANSI */
{
   S16            ret;
   TransCb        *transCb;
   NhuDL_DCCH_Msg *dlDcchMsg;
   U32            msgTransKey;
   U8             msgType;

   TRC2(nhUTLValidateTransId);

   transCb = nhUTLGetTransCbFrmNxTransId(ueCb, datReqSdus->hdr.transId);
   if ((NULLP != transCb) && (transCb->status == NH_STATUS_USED))
   {
      RLOG_ARG0(L_ERROR, DBG_TRNSID,datReqSdus->hdr.transId,"Duplicate NX "
         "triggered from RRC user");
      /* fill the error info and return */
      NH_FILL_NHU_ERROR(*errInfo, NHU_ERRTYPE_INV_NHU_HDR,
                                  NHU_ERRCAUSE_DUPLICATE_RXTRANSID);
      RETVALUE(RFAILED);      
   }
   
   if(TRUE == datReqSdus->sdu.ueMsg.msg.dedMsg.dlDcchMsg.isMsgEncoded)
   {
      RLOG0(L_DEBUG,"Msg is already encoded by RRC user.");
      if(TRUE == datReqSdus->sdu.isUeCfgPres)
      {
         RLOG0(L_ERROR,"UE configuration info present.");
         /* fill the error info and return */
         NH_FILL_NHU_ERROR(*errInfo, NHU_ERRTYPE_UE_CFG,
                                     NHU_ERRCAUSE_NONE);
         RETVALUE(RFAILED);
      }
      RETVALUE(ROK);
   }

   /* look for transCb with (MsgType + RRC TransId) */
   /* fetch the message type */
   dlDcchMsg = &(datReqSdus->sdu.ueMsg.msg.dedMsg.dlDcchMsg.m.dcchMsg);
   ret = nhUTLGetMsgTypeAndEvntFrmSdu(dlDcchMsg->message.val.c1.choice.val, 
                                    NH_SDU_DL_DCCH, 
                                    messageType, event);
   msgType = *messageType;
   if (ROK != ret)
   {
      /* Unknown Message Type error */
      RLOG0(L_ERROR, "Invalid message type in the UE message");
      NH_FILL_NHU_ERROR(*errInfo, NHU_ERRTYPE_INV_UE_MSG_PARAM,
                                  NHU_ERRCAUSE_INV_MSG_CATEGORY);
      RETVALUE(RFAILED);      
   }

   /* Only conn. recfg and sec mode cmd are expected in DL-DCCH message
      which need duplicate transaction check */
   /* ICNC: Add any new messages on DL-DCCH here */
   if ((msgType == NH_MSGTYPE_CONNRECFG) ||
       (msgType == NH_MSGTYPE_SECMODCMD) ||
       (msgType == NH_MSGTYPE_CONNRLS))
   {
      ret = nhUTLGetRrcTransId((Void *)dlDcchMsg, 
                      dlDcchMsg->message.val.c1.choice.val, 
                      NH_SDU_DL_DCCH, msgTransId);
      if (ROK != ret)
      {
         /* Unknown Message Type, error */
         RLOG0(L_ERROR, "Invalid Message type in the UE message ");
         NH_FILL_NHU_ERROR(*errInfo, NHU_ERRTYPE_INV_UE_MSG_PARAM,
                                  NHU_ERRCAUSE_INV_MSG_CATEGORY);
         RETVALUE(RFAILED);      
      }

      NH_GET_TRANSCB_KEY(msgTransKey, msgType, *msgTransId);

      transCb = nhUTLGetTransCbFrmMsgTransId(ueCb, msgTransKey);
      if (NULLP != transCb)
      {
         /* Unknown Message Type, error */
          RLOG_ARG0(L_ERROR, DBG_TRNSID, msgTransKey, "Duplicate message "
             "triggered from RRC user");
         NH_FILL_NHU_ERROR(*errInfo, NHU_ERRTYPE_INV_NHU_HDR,
                                  NHU_ERRCAUSE_DUPLICATE_RRCTRANSID);
         RETVALUE(RFAILED);      
      }
   }

   RETVALUE(ROK);
} /* end of nhUTLValidateTransId */


/** 
 * @brief Validates the PDCP ID or RB ID. 
 * 
 * @details 
 * This function validates whether PDCP ID or RB ID
 * sent in the NHU SDU is configured or not. 
 *  
 *  
 *  
 * @param[in] nhIdentity         RRC ID containing PDCP or RB ID
 * @param[in] cellAndUeCb  Cell context and UE context
 * @param[out] errInfo     Pointer to the error information to 
 *                         be filled (for any error)
 * 
 * @return S16 
 * -# ROK     Validation Success
 * -# RFAILED Validation Failure       
*/
#ifdef ANSI
PRIVATE S16 nhUTLValidateRbOrPdcpId
(
NhId  nhIdentity,
NhCellAndUeCb  *cellAndUeCb,
NhuErrorInfo   *errInfo
)
#else /* ANSI */
PRIVATE S16 nhUTLValidateRbOrPdcpId(nhIdentity, cellAndUeCb, errInfo)
NhId  nhIdentity;
NhCellAndUeCb  *cellAndUeCb;
NhuErrorInfo   *errInfo;
#endif /* ANSI */
{
   CmLListCp      *rbCfgInfoList;
   CmLList        *tmp;
   NhuRbCfgInfo   *rbCfgInfo;

   TRC2(nhUTLValidateRbOrPdcpId);

   if(nhIdentity.isPdcpId)
   {
      /* Validate that the PDCP entity with this ID is configured */
      rbCfgInfoList = &(cellAndUeCb->ueCb->crntCfg.rbCfgList.rbCfgInfoList);
      
      CM_LLIST_FIRST_NODE(rbCfgInfoList, tmp);
      while(NULLP != tmp)
      {
         rbCfgInfo = (NhuRbCfgInfo *)(tmp->node);
         if ((rbCfgInfo->rbCfgBitMsk & NHU_RB_CFG_PDCP_INFO) &&
             (rbCfgInfo->rbId.rbId == nhIdentity.rbId))
         {
            break;
         }
         CM_LLIST_NEXT_NODE(rbCfgInfoList, tmp);
      } /* while () */

      /* tmp is NULLP, if the PDCP entity is not found */
      if (tmp == NULLP)
      {
         RLOG1(L_ERROR,"PDCP radio bearer with ID[%d] cannot be found",
            nhIdentity.rbId);
         /* fill the error info and return */
         NH_FILL_NHU_ERROR(*errInfo, NHU_ERRTYPE_INV_SDU_PARAM, 
                                     NHU_ERRCAUSE_WRONG_PDCP_ID);
         RETVALUE(RFAILED);
      }
   }
   else
   {
      /* Validate that the RB with this ID is configured */
      rbCfgInfoList =
         &(cellAndUeCb->cellCb->crntCellCfgInfo.rbCfgList.rbCfgInfoList);

      CM_LLIST_FIRST_NODE(rbCfgInfoList, tmp);
      while(NULLP != tmp)
      {
         rbCfgInfo = (NhuRbCfgInfo *)(tmp->node);
         if ((rbCfgInfo->rbId.rbId == nhIdentity.rbId) &&
             (rbCfgInfo->rbCfgBitMsk & NHU_RB_CFG_DL))
         {
            break;
         }
         CM_LLIST_NEXT_NODE(rbCfgInfoList, tmp);
      } /* while () */

      /* tmp is NULLP, if the RLC entity is not found */
      if (tmp == NULLP)
      {
         RLOG1(L_ERROR, "RLC entity with ID[%d] cannot be found",
            nhIdentity.rbId);
         /* Fill the error info and return */
         NH_FILL_NHU_ERROR(*errInfo, NHU_ERRTYPE_INV_SDU_PARAM, 
                                     NHU_ERRCAUSE_WRONG_RB_ID);
         RETVALUE(RFAILED);
      }
   }

   RETVALUE(ROK);
} /* nhUTLValidateRbOrPdcpId */


/**
 * @brief Validates data response from the RRC user. 
 *
 * @details
 * This function validates data response for cell and
 * message presence. It also validates the UE ID related configuration
 * and the RB configuration.
 * 
 * 
 * @param[in] datRspSdus      Data Response SDUs
 * @param[in] cellAndUeCb     Cell context and UE context
 * @param[in] msgTransId      Message Transaction identifier 
 * @param[in] event           Message Event Type used in RRC modules
 * @param[out] errInfo        Pointer to the error information to 
 *                            be filled (for any error)
 *
 * @return S16 
 * -# ROK     Validation Success
 * -# RFAILED Validation Failure       
*/
#ifdef ANSI
PUBLIC S16 nhUTLValidateDatRsp
(
NhuDatRspSdus  *datRspSdus,
NhCellAndUeCb  *cellAndUeCb,
U8             *msgTransId,
U8             *event,
NhuErrorInfo   *errInfo
)
#else /* ANSI */
PUBLIC S16 nhUTLValidateDatRsp(datRspSdus, cellAndUeCb, msgTransId , 
                               event, errInfo)
NhuDatRspSdus  *datRspSdus;
NhCellAndUeCb  *cellAndUeCb;
U8             *msgTransId;
U8             *event;   
NhuErrorInfo   *errInfo;
#endif /* ANSI */
{
   NhActvUeCb  *ueCb;
   NhCellCb    *cellCb;
   NhId        nhIdentity;
   NhuDL_DCCH_MsgTyp *dlDcchMsg = NULLP;
   MsgType     msgType = NH_IGNORE;
   U16         crnti = 0;
   S16         ret;

   TRC2(nhUTLValidateDatRsp);

   ueCb = NULLP;
   cellCb = NULLP;
   
   NH_SET_ZERO(&nhIdentity, sizeof(NhId));

   /* First check the maximum number of UE connections reached or not */
   if (nhCb.eNodeBCb.numActUEs >= nhCb.eNodeBCb.protCfg.maxConn)
   {
      RLOG0(L_ERROR, "Maximum Limit of UE Connections Reached.");

      /* fill the error info and return */
      NH_FILL_NHU_ERROR(*errInfo, NHU_ERRTYPE_MAX_CONN,
                                  NHU_ERRCAUSE_NONE);
      RETVALUE(RFAILED);
   }

   /* Validate Header; look for cellCb and ueCb as applicable */
   if(!datRspSdus->hdr.cellId)
   {
      RLOG0(L_ERROR, "CellId is Zero.");
      /* Fill the error info and return */
      NH_FILL_NHU_ERROR(*errInfo, NHU_ERRTYPE_INV_NHU_HDR,
                                  NHU_ERRCAUSE_INVALID_CELLID);
      RETVALUE(RFAILED);
   }
   
   cellCb = nhDBMGetCellCb(datRspSdus->hdr.cellId);
   if (NULLP == cellCb)
   {
      RLOG_ARG0(L_ERROR, DBG_CELLID,datRspSdus->hdr.cellId,"CellCb doesn't Exist");
      /* Fill the error info and return */
      NH_FILL_NHU_ERROR(*errInfo, NHU_ERRTYPE_INV_NHU_HDR,
                                  NHU_ERRCAUSE_CELL_NOT_CONFIGURED);
      RETVALUE(RFAILED);
   }

   cellAndUeCb->cellCb = cellCb;

   ret = nhUTLValidateDatRspDlCChMsg(datRspSdus, cellAndUeCb, errInfo,
                     &crnti, msgTransId, event, &msgType);
   if (ret != ROK)
   {
      RLOG0(L_ERROR,"Validation for DLCCH Msg failed.");
      RETVALUE(RFAILED);
   }

   else if(datRspSdus->sdu.msgCategory == NHU_MSG_DLDCCH)
   {
      dlDcchMsg = &(datRspSdus->sdu.m.dlDcchMsg.m.dcchMsg.message);
      if(dlDcchMsg && dlDcchMsg->choice.val == DL_DCCH_MSGTYP_C1)
      {
         nhUTLGetMsgTypeAndEvntFrmSdu(dlDcchMsg->val.c1.choice.val, 
                                         NH_SDU_DL_DCCH, &msgType, event);
      
         switch(dlDcchMsg->val.c1.choice.val)
         {
            case C1_CSFBPARAMSRSPCDMA2000:
            {
               ueCb = nhDBMGetUeCb(cellCb, datRspSdus->hdr.ueId);
               if(NULLP == ueCb)
               {
                      /* Unknown Message Type, error */
                  RLOG_ARG0(L_ERROR, DBG_UEID,datRspSdus->hdr.ueId,
                     "Invalid UeId in Header");
                  NH_FILL_NHU_ERROR(*errInfo, NHU_ERRTYPE_INV_NHU_HDR,
                                              NHU_ERRCAUSE_INV_HDR_UEID);
                  RETVALUE(RFAILED);
               }
               else
               {
                  cellAndUeCb->ueCb = ueCb;
               }
               break;
            }
            default:
            {
               /* It must not come here. Return error */
               NH_FILL_NHU_ERROR(*errInfo, NHU_ERRTYPE_INV_UE_MSG_PARAM,
                                           NHU_ERRCAUSE_INV_MSG_CATEGORY);
               RETVALUE(RFAILED);
             }
         }
      }

   }/*datRspSdus->sdu.msgCategory == NHU_MSG_DLDCCH*/
   
   /* form the nhIdentity for use further */
   NH_FORM_NH_ID(nhIdentity, datRspSdus, crnti);
/* Conditional IE validation */
#ifdef NH_ASN_COND_VALIDATE
   if(ROK != nhUTLValidateAsnCondIes((Void *)datRspSdus,
                         msgType, errInfo, *event))
   {
      RLOG0(L_ERROR,"conditional IE validation failed");
      RETVALUE(RFAILED);
   }
#endif /* NH_ASN_COND_VALIDATE */

   /* Validate if the message is UE message and supposed to go on 
    * the RbId/PdcpId requested
    */
   if (ROK != nhUTLValidateRbOrPdcpId(nhIdentity, cellAndUeCb, errInfo))
   {
      /* return on error */
      RLOG0(L_ERROR, "RbId is not Configured ");
      RETVALUE(RFAILED);
   }

   RETVALUE(ROK);
} /* nhUTLValidateDatRsp */




/**
 * @brief Validates configuration request from the RRC user. 
 *
 * @details
 * This function validates configuration request for UE ID
 * existence and radio bearer configuration validity.
 * 
 * 
 * @param[in] cfgReqSdus      Configuration Request SDUs
 * @param[in] ueCb            UE context
 * @param[in] cellCb          Cell context
 * @param[out] errInfo        Pointer to the error information to 
 *                            be filled (for any error)
 *
 * @return S16 
 * -# ROK     Validation Success
 * -# RFAILED Validation Failure       
*/
#ifdef ANSI
PUBLIC S16 nhUTLValidateCfgReq
(
NhuCfgReqSdus *cfgReqSdus,
NhActvUeCb    **ueCb,
NhCellCb **cellCb,
NhuErrorInfo  *errInfo
)
#else /* ANSI */
PUBLIC S16 nhUTLValidateCfgReq(cfgReqSdus, ueCb, cellCb, errInfo)
NhuCfgReqSdus *cfgReqSdus;
NhActvUeCb    **ueCb;
NhCellCb      **cellCb;
NhuErrorInfo  *errInfo;
#endif /* ANSI */
{
   Bool bError;
   NhCellCb   *tmpCellCb = NULLP;
   NhActvUeCb *tmpUeCb = NULLP;
   
   TRC2(nhUTLValidateCfgReq);

   bError = FALSE;

   
   tmpCellCb = nhDBMGetCellCb(cfgReqSdus->hdr.cellId);
   if (NULLP == tmpCellCb)
   {
      /* Fill the error info and return */
      NH_FILL_NHU_ERROR(*errInfo, NHU_ERRTYPE_INV_NHU_HDR,
                                  NHU_ERRCAUSE_CELL_NOT_CONFIGURED);
      RETVALUE(RFAILED);
   }

   *cellCb = tmpCellCb;

   if(cfgReqSdus->cfgInfo.ueCfgBitMsk & NHU_EXP_CFG_UE_CFG)
   {
      tmpUeCb = nhDBMGetUeCb(tmpCellCb, 
                          cfgReqSdus->cfgInfo.ueInfo.newCrnti);
      if(NULLP != tmpUeCb)
      {
         RLOG0(L_ERROR,
                 "UE ID already configured, but sent in UE ID cfg");
         NH_FILL_NHU_ERROR(*errInfo, NHU_ERRTYPE_INV_CFG,
                                     NHU_ERRCAUSE_INVALID_CFG_INFO);
         RETVALUE(RFAILED);  
      }      
   }

   
   if(cfgReqSdus->hdr.ueId == 0) 
   {
      if(!(cfgReqSdus->cfgInfo.ueCfgBitMsk & NHU_EXP_CFG_UE_CFG))
      {
         RLOG0(L_ERROR,"UE ID config not present in UE Cfg ");
         NH_FILL_NHU_ERROR(*errInfo, NHU_ERRTYPE_INV_CFG,
                                     NHU_ERRCAUSE_CRNTI_NOT_CFG);
         RETVALUE(RFAILED); 
      }
   }
   else
   {
      tmpUeCb = nhDBMGetUeCb(tmpCellCb, cfgReqSdus->hdr.ueId);
      if(NULLP == tmpUeCb)
      {
         RLOG0(L_ERROR, "Invalid UE ID in the header ");
         NH_FILL_NHU_ERROR(*errInfo, NHU_ERRTYPE_INV_NHU_HDR,
                                     NHU_ERRCAUSE_INV_HDR_UEID);
         RETVALUE(RFAILED);  
      }      
   }

   if(NULLP != tmpUeCb)
   {
      /* This means ueCb exists. This is a reconfiguration of the existing UE or RB */
      /* Validate whether the particular parameter is allowed to reconfigure */

      if (ROK != nhUTLValidateUeCfg(&(cfgReqSdus->cfgInfo), tmpUeCb, errInfo))
      {
         NH_FILL_NHU_ERROR(*errInfo, NHU_ERRTYPE_UE_CFG,
                                     NHU_ERRCAUSE_INV_RB_CFG_INFO);
         RETVALUE(RFAILED);         
      }
   } /* if (NULLP != tmpUeCb) */
   
   *ueCb = tmpUeCb;

   RETVALUE(ROK);

} /* nhUTLValidateCfgReq */

/**
 * @brief Validates data request from the RRC user. 
 *
 * @details
 * This function validates UE and Cell context existence and
 * radio bearer configuration validity.
 * 
 * 
 * @param[in] datReqSdus   Data Request SDUs
 * @param[in] cellAndUeCb  Cell context and UE context
 * @param[in] msgTransId   Message Transaction identifier
 * @param[out] errInfo     Pointer to the error information to 
 *                         be filled (for any error)
 * @param[in] event        Message Event Type used in RRC modules
 *
 * @return S16 
 * -# ROK     Validation Success
 * -# RFAILED Validation Failure       
*/
#ifdef ANSI
PUBLIC S16 nhUTLValidateDatReq
(
NhuDatReqSdus  *datReqSdus,
NhCellAndUeCb  *cellAndUeCb,
U8             *msgTransId,
NhuErrorInfo   *errInfo,
U8             *event
)
#else /* ANSI */
PUBLIC S16 nhUTLValidateDatReq(datReqSdus, cellAndUeCb, msgTransId, 
                               errInfo, event)
NhuDatReqSdus  *datReqSdus;
NhCellAndUeCb  *cellAndUeCb;
U8             *msgTransId;
NhuErrorInfo   *errInfo;
U8             *event;
#endif /* ANSI */
{
   NhActvUeCb  *ueCb;
   NhCellCb    *cellCb;
   NhId        nhIdentity;
   U8          messageType = NH_IGNORE;

   TRC2(nhUTLValidateDatReq);

   ueCb = NULLP;
   cellCb = NULLP;
   NH_SET_ZERO(&nhIdentity, sizeof(NhId));

   /* Validate Header; look for cellCb and ueCb as applicable */
   cellCb = nhDBMGetCellCb(datReqSdus->hdr.cellId);
   if (NULLP == cellCb)
   {
      /* Fill the error info and return */
      NH_FILL_NHU_ERROR(*errInfo, NHU_ERRTYPE_INV_NHU_HDR,
                                  NHU_ERRCAUSE_CELL_NOT_CONFIGURED);
      RETVALUE(RFAILED);
   }

   cellAndUeCb->cellCb = cellCb;

   /* Look for ueCb, if it is an UE specific message */
   if (TRUE == datReqSdus->sdu.ueMsg.isMsgDed)
   {
      ueCb = nhDBMGetUeCb(cellCb, datReqSdus->hdr.ueId);
      if (NULLP == ueCb)
      {
         /* Fill the error info and return */
         NH_FILL_NHU_ERROR(*errInfo, NHU_ERRTYPE_INV_NHU_HDR,
                                     NHU_ERRCAUSE_UE_NOT_CONFIGURED);
         RETVALUE(RFAILED);
      }
      cellAndUeCb->ueCb = ueCb;

      /* Validate the transaction */
      if (ROK != nhUTLValidateTransId(datReqSdus, ueCb, msgTransId,
                                 errInfo, event, &messageType))
      {
         /* return on error */
         RETVALUE(RFAILED);
      }
   }

   if(messageType == NH_MSGTYPE_CONNRECFG)
   {
      /* SRB1 must be configured before sending this message */
      if(!(cellAndUeCb->ueCb->rbSecCfgBitMask & NH_SRB1_CFGD))
      {
         RLOG0(L_ERROR, "SRB1 is not configured.");
         NH_FILL_NHU_ERROR(*errInfo, NHU_ERRTYPE_UE_CFG, 
                                     NHU_ERRCAUSE_SRB1_NOT_CONFIGURED);
         RETVALUE(RFAILED);
      }
   }

   /*if message type is Security mode command and no Security configuration 
     is provided by RRC user and lower security configuration is not done
     then throw an error*/
   if(((messageType == NH_MSGTYPE_SECMODCMD) && 
      (datReqSdus->sdu.isUeCfgPres == FALSE)) &&
            !(ueCb->rbSecCfgBitMask & NH_SEC_CFG_DONE))
   {
      RLOG0(L_ERROR, "Security not yet configured, Can't send Sec Mode Command"
          "Message");
      NH_FILL_NHU_ERROR(*errInfo, NHU_ERRTYPE_MSG_SEND_FAILED,
                           NHU_ERRCAUSE_SEC_NOT_CONFIGURED);
      RETVALUE(RFAILED);
   }

/* Conditional IE validation */
#ifdef NH_ASN_COND_VALIDATE
   if(ROK != nhUTLValidateAsnCondIes((Void *)datReqSdus,
                         messageType, errInfo, *event))
   {
      RLOG0(L_ERROR,"conditional IE validation failed");
      RETVALUE(RFAILED);
   }
#endif /* NH_ASN_COND_VALIDATE */

   /* Form the nhIdentity for use further */
   NH_FORM_NHID_FRM_DATREQ(nhIdentity, datReqSdus);

   /* Validate if the message is an UE message and supposed to go on 
    * the RbId/PdcpId requested
    */
   /* nh001.301:  Return on Error */
   if (ROK != nhUTLValidateRbOrPdcpId(nhIdentity, cellAndUeCb, errInfo))
   {
      RLOG0(L_ERROR,"conditional IE validation failed");
      RETVALUE(RFAILED);
   }

   RETVALUE(ROK);
} /* nhUTLValidateDatReq */

/**
 * @brief Validates the UE configuration from the RRC user. 
 *
 * @details
 * This function validates the UE configuration as received
 * from the RRC user. It verifies if the RBs requested for configuration
 * are correct.
 * 
 * 
 * @param[in] ueCfg        UE Configuration from RRC user
 * @param[in] ueCb         UE context
 * @param[in] errInfo      Pointer to the error information to 
 *                         be filled (for any error)
 *
 * @return S16 
 * -# ROK     Validation Success
 * -# RFAILED Validation Failure       
*/
#ifdef ANSI
PUBLIC S16 nhUTLValidateUeCfg
(
NhuUeCfgInfo   *ueCfg,
NhActvUeCb     *ueCb,
NhuErrorInfo   *errInfo
)
#else /* ANSI */
PUBLIC S16 nhUTLValidateUeCfg(ueCfg, ueCb, errInfo)
NhuUeCfgInfo   *ueCfg;
NhActvUeCb     *ueCb;
NhuErrorInfo   *errInfo;
#endif /* ANSI */
{
   NhuRbCfgList    *rbToModList;
   NhuRbRlsCfgList *rbToRlsList;
   CmLList         *tmpNode;
   S16              i;
   U32              dirMsk[2];
   S16              ret = ROK;

   TRC2(nhUTLValidateUeCfg);

   if(NULLP == ueCb)
   {
      /* nh004.201 - changed debug print to INFO type.  */
      RLOG0(L_DEBUG,"ueCb does not exist.");
      RETVALUE(ROK);
   }

   NH_SET_ZERO(dirMsk, 2*sizeof(U32));

   /* validate security configuration */
   if(ueCfg->ueCfgBitMsk & NHU_EXP_CFG_SEC_CFG)
   {
      if (!(ueCb->rbSecCfgBitMask & NH_SRB1_CFGD))
      {
         RLOG0(L_ERROR,"SRB1 Not yet configured, Can't initiate Security "
             "Activation");
         /* Fill the error info and return */
         NH_FILL_NHU_ERROR(*errInfo, NHU_ERRTYPE_UE_CFG,
                              NHU_ERRCAUSE_SRB1_NOT_CONFIGURED);
         RETVALUE(RFAILED);
      }
      if(!(ueCb->rbSecCfgBitMask & NH_SEC_CFG_DONE))
      {
         if (!(ueCfg->secCfg.secCfgBitMsk & NHU_SEC_CIPHER_ALGO) ||
             !(ueCfg->secCfg.secCfgBitMsk & NHU_SEC_INTG_ALGO))
         {
            RLOG0(L_ERROR, "Both intg and Cipher cfg must be provided for "
               "first sec cfg");
            NH_FILL_NHU_ERROR(*errInfo, NHU_ERRTYPE_UE_CFG,
                                 NHU_ERRCAUSE_INV_SEC_CFG);
            RETVALUE(RFAILED);
         }
      }
   }

   /* validate the RB config list */
   if (ueCfg->ueCfgBitMsk & NHU_EXP_CFG_RB_CFG)
   {
      /* Add Case */
      ret = nhUTLValidateUeCfgRbAdd(ueCfg, ueCb, errInfo);
      if (ret != ROK)
      {
         RLOG0(L_ERROR, "Validation for RB Addition failed");
         RETVALUE(RFAILED);
      }
      
      /* Modify */
      if (ueCfg->rbCfgAllList.rbCfgTypBitMsk & NHU_CFG_RB_MODIFY)
      {
         if (!(ueCb->crntCfg.bitMskPres & NH_CRNT_CFG_RB))
         {
            /* Nothing exists in crnt config to validate */
            RETVALUE(RFAILED);
         }

         /* Validate that the RBs to be modified already exist in crnt cfg */
         rbToModList = &(ueCfg->rbCfgAllList.rbToModCfgList);

         for(i=0; i<(rbToModList->numOfRbCfgInfo); i++)
         {
            CM_LLIST_FIRST_NODE(&(ueCb->crntCfg.rbCfgList.rbCfgInfoList), 
                                       tmpNode);
            while (NULLP != tmpNode)
            {
               if ((NULLP != tmpNode->node) &&
                     (((NhuRbCfgInfo*)(tmpNode->node))->rbId.rbId 
                       == rbToModList->rbCfgInfoList[i].rbId.rbId) &&
                     (((NhuRbCfgInfo*)(tmpNode->node))->rbId.rbTyp
                       == rbToModList->rbCfgInfoList[i].rbId.rbTyp))
               {
                  /* RB already in crnt config */
                  break;
               }
               CM_LLIST_NEXT_NODE(&ueCb->crntCfg.rbCfgList.rbCfgInfoList,
                                     tmpNode);
            } /* while */

            if (NULLP == tmpNode)
            {
               /* This means the RB cannot be located */
               /* Fill the error info and return */
               RETVALUE(RFAILED);
            }
         } /* for (..) */
      } /* if (NHU_CFG_RB_MODIFY) */

      /* Release */
      if (ueCfg->rbCfgAllList.rbCfgTypBitMsk & NHU_CFG_RB_RLS)
      {
         /* validate that the RBs to be modified alreayd exist in crnt cfg */
         rbToRlsList = &(ueCfg->rbCfgAllList.rbToRlsCfgList);

         for (i=0; i<(rbToRlsList->numOfRbCfgInfo); i++)
         {
            CM_LLIST_FIRST_NODE(&(ueCb->crntCfg.rbCfgList.rbCfgInfoList), 
                                       tmpNode);
            while (NULLP != tmpNode)
            {
               if ((NULLP != tmpNode->node) &&
                     (((NhuRbCfgInfo*)(tmpNode->node))->rbId.rbId 
                       == rbToRlsList->rbsToRel[i].rbId))
               {
                  /* RB already in crnt config */
                  break;
               }
               CM_LLIST_NEXT_NODE(&ueCb->crntCfg.rbCfgList.rbCfgInfoList,
                                     tmpNode);
            } /* while */

            if (NULLP == tmpNode)
            {
               /* This means the RB cannot be located */
               /* Fill the error info and return */
               RETVALUE(RFAILED);
            }
         } /* for(..) */
      } /* if (NHU_CFG_RB_RLS) */
   } /* if (ueCfg->ueCfgBitMsk & NHU_EXP_CFG_RB_CFG) */

/*nh001.201 validation not required*/
   RETVALUE(ROK);
} /* nhUTLValidateUeCfg */

/**
 * @brief Validates Count request from the RRC user. 
 *
 * @details
 * This function validates UE and Cell context existence for
 * which RB count values have been requested.
 * 
 * 
 * @param[in] countHdr     Header containing cell, UE and transaction IDs
 * @param[out] errInfo     Pointer to the error information to 
 *                         be filled (for any error)
 * @param[in] event        Message Event Type used in RRC modules
 *
 * @return S16 
 * -# ROK     Validation Success
 * -# RFAILED Validation Failure       
*/
#ifdef ANSI
PUBLIC S16 nhUTLValidateCountReq
(
NhuHdr          *countHdr,
NhuErrorInfo   *errInfo,
U8             *event
)
#else /* ANSI */
PUBLIC S16 nhUTLValidateCountReq(countHdr, errInfo, event)
NhuHdr          *countHdr;
NhuErrorInfo   *errInfo;
U8             *event;
#endif /* ANSI */
{
   NhActvUeCb     *ueCb = NULLP;
   NhCellAndUeCb  cellAndUeCb;
   NhCellCb       *cellCb = NULLP;
   NhId           nhIdentity;

   TRC2(nhUTLValidateCountReq);

   ueCb = NULLP;
   cellCb = NULLP;
   NH_SET_ZERO(&nhIdentity, sizeof(NhId));

   /* Validate Header; look for cellCb and ueCb as applicable */
   cellCb = nhDBMGetCellCb(countHdr->cellId);
   if (NULLP == cellCb)
   {
      /* Fill the error info and return */
      NH_FILL_NHU_ERROR(*errInfo, NHU_ERRTYPE_INV_NHU_HDR,
                                  NHU_ERRCAUSE_CELL_NOT_CONFIGURED);
      RETVALUE(RFAILED);
   }

   cellAndUeCb.cellCb = cellCb;

   /* Look for ueCb, if it is an UE specific message */
   ueCb = nhDBMGetUeCb(cellCb, countHdr->ueId);
   if (NULLP == ueCb)
   {
      /* Fill the error info and return */
      NH_FILL_NHU_ERROR(*errInfo, NHU_ERRTYPE_INV_NHU_HDR,
                                  NHU_ERRCAUSE_UE_NOT_CONFIGURED);
      RETVALUE(RFAILED);
   }

   RETVALUE(ROK);
} /* nhUTLValidateCountReq */

#ifdef NHU_MULT_SAP_SUPPRT
/**
 * @brief Validates the consistency of sap information provided in 
 * NhuLLyrSapIdInfo from RRC user.
 * 
 * @details
 * This function validates that sap IDs of lower saps provided by the user
 * are configured in RRC layer.
 * 
 * @param[in] cellCfgInfo  Cell Configuration Info
 *
 * @return S16 
 * -# ROK     Validation Success
 * -# RFAILED Validation Failure       
*/
#ifdef ANSI
PRIVATE S16 nhUTLValidateMsapInfo
(
NhuCellCfgInfo *cellCfgInfo
)
#else /* ANSI */
PRIVATE S16 nhUTLValidateMsapInfo (cellCfgInfo)
NhuCellCfgInfo *cellCfgInfo;
#endif /* ANSI */
{
   TRC2(nhUTLValidateMsapInfo)

   if ((LCM_REASON_NOT_APPL != 
        nhUTLValidateLSapSuId(STNHKWUSAP, cellCfgInfo->llyrSapIdInfo.kwuSapId)) 
      || (NULLP == 
         nhUTLFindLlyrSapFrmSuId(STNHKWUSAP, 
                 cellCfgInfo->llyrSapIdInfo.kwuSapId)))
   {
      RLOG1(L_ERROR,"KWU sapID [%d] not configured",
         cellCfgInfo->llyrSapIdInfo.kwuSapId);
      RETVALUE(RFAILED);
   }

   if ((LCM_REASON_NOT_APPL != 
        nhUTLValidateLSapSuId(STNHCKWSAP, cellCfgInfo->llyrSapIdInfo.ckwSapId)) 
      || (NULLP == nhUTLFindLlyrSapFrmSuId(STNHCKWSAP, 
                      cellCfgInfo->llyrSapIdInfo.ckwSapId)))
   {
      RLOG1(L_ERROR,"CKW sapID [%d] not configured",
            cellCfgInfo->llyrSapIdInfo.ckwSapId);
      RETVALUE(RFAILED);
   }

   if ((LCM_REASON_NOT_APPL !=
        nhUTLValidateLSapSuId(STNHCRGSAP, cellCfgInfo->llyrSapIdInfo.crgSapId)) 
      || (NULLP == nhUTLFindLlyrSapFrmSuId(STNHCRGSAP, 
                      cellCfgInfo->llyrSapIdInfo.crgSapId)))
   {
      RLOG1(L_ERROR,"CRG sapID [%d] not configured",
         cellCfgInfo->llyrSapIdInfo.crgSapId);
      RETVALUE(RFAILED);
   }

   if ((LCM_REASON_NOT_APPL !=
        nhUTLValidateLSapSuId(STNHCPJSAP, cellCfgInfo->llyrSapIdInfo.cpjSapId)) 
      || (NULLP == nhUTLFindLlyrSapFrmSuId(STNHCPJSAP, 
                      cellCfgInfo->llyrSapIdInfo.cpjSapId)))
   {
      RLOG1(L_ERROR,"CPJ sapID [%d] not configured",
         cellCfgInfo->llyrSapIdInfo.cpjSapId);
      RETVALUE(RFAILED);
   }

   if ((LCM_REASON_NOT_APPL !=
        nhUTLValidateLSapSuId(STNHPJUSAP, cellCfgInfo->llyrSapIdInfo.pjuSapId)) 
      || (NULLP == nhUTLFindLlyrSapFrmSuId(STNHPJUSAP, 
                    cellCfgInfo->llyrSapIdInfo.pjuSapId)))
   {
      RLOG1(L_ERROR,"PJU sapID [%d] not configured",
         cellCfgInfo->llyrSapIdInfo.pjuSapId);
      RETVALUE(RFAILED);
   }

#ifdef RRC_PHY_CFG_SUPP
   if ((cellCfgInfo->cellCfgBitMsk & NHU_CELL_PHY_CFG) &&
       ((LCM_REASON_NOT_APPL !=
                    nhUTLValidateLSapSuId(STNHCTFSAP, 
                    cellCfgInfo->llyrSapIdInfo.ctfSapId)) 
         || (NULLP == nhUTLFindLlyrSapFrmSuId(STNHCTFSAP, 
                    cellCfgInfo->llyrSapIdInfo.ctfSapId))))
   {
      RLOG1(L_ERROR,"CTF sapID [%d] not configured",
         cellCfgInfo->llyrSapIdInfo.ctfSapId);
      RETVALUE(RFAILED);
   }
#endif /* RRC_PHY_CFG_SUPP */

   RETVALUE(ROK);
} /* end of nhUTLValidateMsapInfo */
#endif /* NHU_MULT_SAP_SUPPRT */

/**
 * @brief Validates the LogCh to Trch Mapping in cell configration request
 * from RRC user.
 * @details
 * This function validates the Logical channel to Trch channel mapping in 
 * cell configuration. There should be only one RB for each of the following
 * mapping:
 *  CCCH->DL_CCCH, CCCH->UL_SCH, BCCH->BCH, BCCH->DL_SCH, PCCH->PCH.
 * 
 * @param[in] cellCfgInfo  Cell Configuration Info
 *
 * @return S16 
 * -# ROK     Validation Success
 * -# RFAILED Validation Failure       
*/
#ifdef ANSI
PRIVATE S16 nhUTLValidateLogChTrchMap
(
NhuCellCfgInfo *cellCfgInfo
)
#else /* ANSI */
PRIVATE S16 nhUTLValidateLogChTrchMap(cellCfgInfo)
NhuCellCfgInfo *cellCfgInfo;
#endif /* ANSI */
{
   NhuRbCfgInfo *rbCfgInfo = NULLP;
   U8            rbId      = 0;
   U8            lcType    = 0;
   U8            trChType  = 0;
   Bool          chkForRb[NH_MAX_CFGD_RB_CELL];
   U8            bcchLcId=0xFF; /* Temp placeholder for first bcch->DlSch. */
   S16           ret = ROK;

   TRC2(nhUTLValidateLogChTrchMap)

   NH_SET_ZERO(chkForRb, NH_MAX_CFGD_RB_CELL*sizeof(Bool));
   
   for (rbId = 0; rbId < cellCfgInfo->rbToAddCfgList.numOfRbCfgInfo; rbId++)
   {
      rbCfgInfo = &(cellCfgInfo->rbToAddCfgList.rbCfgInfoList[rbId]);
      if((rbCfgInfo->rbCfgBitMsk & NHU_RB_CFG_DL) && 
         (rbCfgInfo->dlRbCfg.dlRbBitMsk & NHU_RB_CFG_RLC_MODE_DL) &&
         (rbCfgInfo->dlRbCfg.dlRlcMode.mode != NHU_RLC_MODE_TM))
      {
         RLOG0(L_ERROR,"Invalid RLC Mode");
         RETVALUE(RFAILED);
      }

      if((rbCfgInfo->rbCfgBitMsk & NHU_RB_CFG_UL) && 
         (rbCfgInfo->ulRbCfg.ulRbBitMsk & NHU_RB_CFG_RLC_MODE_UL) &&
         (rbCfgInfo->ulRbCfg.ulRlcMode.mode != NHU_RLC_MODE_TM))
      {
         RLOG0(L_ERROR,"Invalid RLC Mode");
         RETVALUE(RFAILED);
      }

      if(rbCfgInfo->rbCfgBitMsk & NHU_RB_CFG_DL &&
         rbCfgInfo->dlRbCfg.dlRbBitMsk & NHU_RB_CFG_LOGCH_MAP_DL)
      {
         lcType = rbCfgInfo->lcId.logChType;
         trChType =  rbCfgInfo->dlRbCfg.dlLogChMap.trChType;
         if (lcType == CM_LTE_LCH_CCCH &&
             trChType == CM_LTE_TRCH_DL_SCH)
         {
            NH_SET_CELLCFG_RBMAP_ELMNT(chkForRb, 0, ret)

            if (ret != ROK)
            {
               RLOG0(L_ERROR, "Duplicate CCCH->DL_SCH mapping in cell cfg ");
               RETVALUE(RFAILED);
            }
         }
         else if(lcType == CM_LTE_LCH_BCCH &&
                 trChType == CM_LTE_TRCH_BCH)
         {
            NH_SET_CELLCFG_RBMAP_ELMNT(chkForRb, 2, ret)

            if (ret != ROK)
            {
               RLOG0(L_ERROR, "Duplicate BCCH->BCH mapping in cell cfg ");
               RETVALUE(RFAILED);
            }
         }
         else if (lcType == CM_LTE_LCH_PCCH &&
                  trChType == CM_LTE_TRCH_PCH)
         {
            NH_SET_CELLCFG_RBMAP_ELMNT(chkForRb, 3, ret)
            if (ret != ROK)
            {
               RLOG0(L_ERROR, "Duplicate PCCH->PCH mapping in cell cfg ");
               RETVALUE(RFAILED);
            }
         }
         else if (lcType == CM_LTE_LCH_BCCH &&
                  trChType == CM_LTE_TRCH_DL_SCH)
         {
            /* Positions 4 & 5 reserved for BCCH-->DLSCH mapping.
             * One for carrying SIB1 and the other for SIs.
             */

            NH_SET_CELLCFG_BCCH_DLSCH_RBMAP(chkForRb, 4, bcchLcId, \
                           rbCfgInfo->lcId.lcId, ret);

            if (ROK != ret)
            {
               RLOG0(L_ERROR, "Duplicate BCCH->DL_SCH mapping in cell cfg ");
               RETVALUE(RFAILED);
            }
         }
         else
         {
             RLOG0(L_ERROR, "Invalid LogCh to Trch mapping in cell cfg ");
             RETVALUE(RFAILED);
         }
      } /* rbCfgInfo->rbCfgBitMsk & NHU_RB_CFG_DL */

      if(rbCfgInfo->rbCfgBitMsk & NHU_RB_CFG_UL && 
         rbCfgInfo->ulRbCfg.ulRbBitMsk & NHU_RB_CFG_LOGCH_MAP_UL)
      {
         lcType = rbCfgInfo->lcId.logChType;
         trChType =  rbCfgInfo->ulRbCfg.ulLogChMap.trChType; 
         if(lcType == CM_LTE_LCH_CCCH &&
            trChType == CM_LTE_TRCH_UL_SCH)
         {
            NH_SET_CELLCFG_RBMAP_ELMNT(chkForRb, 1, ret)
            if (ret != ROK)
            {
               RLOG0(L_ERROR, "Duplicate CCCH->UL_SCH mapping in cell cfg ");
               RETVALUE(RFAILED);
            }
         }
         else
         {
             RLOG0(L_ERROR, "Invalid LogCh to Trch mapping in cell cfg ");
             RETVALUE(RFAILED);
         }
      }/* if RbCfgBitMsk & NHU_RB_CFG_UL */
   }/* end of for loop */

   RETVALUE(ROK);
} /* end of nhUTLValidateLogChTrchMap */
/**
 * @brief Validates cell configration request from the RRC user. 
 *
 * @details
 * This function validates cell configuration bitmask and 
 * radio bearer information for Configuration Type Setup.
 * 
 * 
 * 
 * @param[in] cellCfgReqSdus  Configuration Request SDUs
 * @param[in] cellCb          Cell context
 * @param[out] errInfo        Pointer to the error information to 
 *                            be filled (for any error)
 *
 * @return S16 
 * -# ROK     Validation Success
 * -# RFAILED Validation Failure       
*/
#ifdef ANSI
PRIVATE S16 nhUTLValidateCfgTypeSetup
(
NhuCellCfgReqSdus *cellCfgReqSdus,
NhCellCb          *cellCb,
NhuErrorInfo   *errInfo
)
#else /* ANSI */
PRIVATE S16 nhUTLValidateCfgTypeSetup(cellCfgReqSdus, cellCb, errInfo)
NhuCellCfgReqSdus *cellCfgReqSdus;
NhCellCb          *cellCb;
NhuErrorInfo   *errInfo;
#endif /* ANSI */
{
   NhuCellCfgInfo *cellCfgInfo;

   TRC2(nhUTLValidateCfgTypeSetup);

   cellCfgInfo = &(cellCfgReqSdus->sdu.u.cfgInfo);                              
   if (NULLP != cellCb)                                                         
   {                                                                            
      RLOG_ARG0(L_ERROR, DBG_CELLID,cellCfgReqSdus->hdr.cellId,
         "Cell Setup: duplicate cell found");
      NH_FILL_NHU_ERROR(*errInfo, NHU_ERRTYPE_INV_NHU_HDR,                      
                                  NHU_ERRCAUSE_DUPLICATE_CELL);                 
      RETVALUE(RFAILED);                                                        
   }                                                                            
                                                                                
   if (!((cellCfgInfo->cellCfgBitMsk & NHU_CELL_CFG_RB) &&                      
        (cellCfgInfo->cellCfgBitMsk & NHU_CELL_CFG_MAC_CMN)))                   
   {                                                                            
      RLOG0(L_ERROR,"Invalid cell cfg ");                                               
      NH_FILL_NHU_ERROR(*errInfo, NHU_ERRTYPE_INV_CFG,                          
                                  NHU_ERRCAUSE_INVALID_CFG_INFO);               
      RETVALUE(RFAILED);                                                        
   }                                                                            
                                                                                
   if ((cellCfgInfo->cellCfgBitMsk & NHU_CELL_CFG_RB) &&                        
       (!(cellCfgInfo->rbToAddCfgList.numOfRbCfgInfo) ||                        
    (cellCfgInfo->rbToAddCfgList.numOfRbCfgInfo > NH_MAX_CFGD_RB_CELL)))        
   {                                                                            
      RLOG0(L_ERROR, "Invalid Add RB config in cell cfg ");
      NH_FILL_NHU_ERROR(*errInfo, NHU_ERRTYPE_INV_CFG,
                                  NHU_ERRCAUSE_INV_RB_CFG_INFO);
      RETVALUE(RFAILED);
   }
   /* Validation for the no. of Rbs that can be configured for each 
      Logical channel. One Rb for each of the folloing LogCh->Trch 
      should be configured.
      CCCH->DL_SCH, CCCH->UL_SCH, BCCH->BCH, BCCH->DL_SCH, PCCH->PCH */
   if (ROK != (nhUTLValidateLogChTrchMap(cellCfgInfo)))
   {
      NH_FILL_NHU_ERROR(*errInfo, NHU_ERRTYPE_INV_CFG,
                                  NHU_ERRCAUSE_INV_RB_CFG_INFO);
      RETVALUE(RFAILED);
   }
 
   /* Added Validation for System Bandwidth */
   if(((cellCfgInfo->sysBwCfg.dlSysBw != NHU_SYS_BW_N6) &&
       (cellCfgInfo->sysBwCfg.dlSysBw != NHU_SYS_BW_N15) &&
       (cellCfgInfo->sysBwCfg.dlSysBw != NHU_SYS_BW_N25) &&
       (cellCfgInfo->sysBwCfg.dlSysBw != NHU_SYS_BW_N50) &&
       (cellCfgInfo->sysBwCfg.dlSysBw != NHU_SYS_BW_N75) &&
       (cellCfgInfo->sysBwCfg.dlSysBw != NHU_SYS_BW_N100))||
      ((cellCfgInfo->sysBwCfg.ulSysBw != NHU_SYS_BW_N6) &&
       (cellCfgInfo->sysBwCfg.ulSysBw != NHU_SYS_BW_N15) &&
       (cellCfgInfo->sysBwCfg.ulSysBw != NHU_SYS_BW_N25) &&
       (cellCfgInfo->sysBwCfg.ulSysBw != NHU_SYS_BW_N50) &&
       (cellCfgInfo->sysBwCfg.ulSysBw != NHU_SYS_BW_N75) &&
       (cellCfgInfo->sysBwCfg.ulSysBw != NHU_SYS_BW_N100)))
             
         {
            NH_FILL_NHU_ERROR(*errInfo, NHU_ERRTYPE_INV_CFG,
                                     NHU_ERRCAUSE_INVALID_CFG_INFO);
            RETVALUE(RFAILED);
         }

#ifdef RRC_PHY_CFG_SUPP
   /* validate if cell id, antenna cfg, tx scheme is correct and none 
      of the mandatory config elements are absent(since it is SETUP) */
   if ((cellCfgInfo->cellCfgBitMsk & NHU_CELL_PHY_CFG) &&
       (((cellCfgInfo->phyCellCfg.pcellCfg.cellIdGrpId > 167) ||
        (cellCfgInfo->phyCellCfg.pcellCfg.physCellId > 2)) ||
       ((cellCfgInfo->phyCellCfg.numOfTxAnt != 1) &&
        (cellCfgInfo->phyCellCfg.numOfTxAnt != 2) &&
        (cellCfgInfo->phyCellCfg.numOfTxAnt != 4)) ||
       ((cellCfgInfo->phyCellCfg.schmCfg.scSpacing != 
            NHU_TX_SCHM_SC_SPACING_15KHZ) &&
       (cellCfgInfo->phyCellCfg.schmCfg.scSpacing !=
            NHU_TX_SCHM_SC_SPACING_7DOT5KHZ)) ||
       !((cellCfgInfo->phyCellCfg.bitmask & NHU_CMN_PHY_CFG_PRACH) &&
         (cellCfgInfo->phyCellCfg.bitmask & NHU_CMN_PHY_CFG_PDSCH) &&
         (cellCfgInfo->phyCellCfg.bitmask & NHU_CMN_PHY_CFG_PUSCH) &&
         (cellCfgInfo->phyCellCfg.bitmask & NHU_CMN_PHY_CFG_PHICH) &&
         (cellCfgInfo->phyCellCfg.bitmask & NHU_CMN_PHY_CFG_PUCCH) &&
         (cellCfgInfo->phyCellCfg.bitmask & NHU_CMN_PHY_CFG_UL_SRS)) ||
         ((cellCfgInfo->duplexMode == NHU_MODE_TDD) && 
          (cellCfgInfo->phyCellCfg.bitmask & NHU_CMN_PHY_CFG_TDD_SF))))
   {
      RLOG0(L_ERROR, "Invalid PHY config for cell ");
      NH_FILL_NHU_ERROR(*errInfo, NHU_ERRTYPE_INV_CFG,
                                  NHU_ERRCAUSE_INVALID_PHYCFG_INFO);
      RETVALUE(RFAILED);
   }
#endif /* RRC_PHY_CFG_SUPP */
 
   /* validate consistency of MSAP info in the config req. */
#ifdef NHU_MULT_SAP_SUPPRT
   if (ROK != (nhUTLValidateMsapInfo(cellCfgInfo)))
   {
      NH_FILL_NHU_ERROR(*errInfo, NHU_ERRTYPE_INV_CFG,
                                  NHU_ERRCAUSE_INVALID_CFG_INFO);
      RETVALUE(RFAILED);
   }
#endif /* NHU_MULT_SAP_SUPPRT */
   RETVALUE(ROK);

}/* End of nhUTLValidateCfgTypeSetup */

/**
 * @brief Validates cell configration request from the RRC user. 
 *
 * @details
 * This function validates cell configuration bitmask and 
 * radio bearer information.
 * 
 * 
 * 
 * @param[in] cellCfgReqSdus  Configuration Request SDUs
 * @param[in] cellCb          Cell context
 * @param[out] errInfo        Pointer to the error information to 
 *                            be filled (for any error)
 *
 * @return S16 
 * -# ROK     Validation Success
 * -# RFAILED Validation Failure       
*/
#ifdef ANSI
PUBLIC S16 nhUTLValidateCellCfgReq
(
NhuCellCfgReqSdus *cellCfgReqSdus,
NhCellCb          *cellCb,
NhuErrorInfo   *errInfo
)
#else /* ANSI */
PUBLIC S16 nhUTLValidateCellCfgReq(cellCfgReqSdus, cellCb, errInfo)
NhuCellCfgReqSdus *cellCfgReqSdus;
NhCellCb          *cellCb;
NhuErrorInfo   *errInfo;
#endif /* ANSI */
{
   NhuCellReCfgInfo *cellReCfgInfo;
   S16            ret;

   TRC2(nhUTLValidateCellCfgReq);

   ret = ROK;
   switch(cellCfgReqSdus->sdu.cfgType)
   {
      case NHU_CFGTYPE_SETUP:
      {
         ret = nhUTLValidateCfgTypeSetup(cellCfgReqSdus, cellCb, errInfo);
         if(ROK != ret)
         {
             RETVALUE(RFAILED);
         }
         else
         {
             RETVALUE(ROK);
         }
      }
      break;
      
      case NHU_CFGTYPE_MODIFY:
      {
         cellReCfgInfo = &(cellCfgReqSdus->sdu.u.reCfgInfo);
         if (NULLP == cellCb)
         {
            RLOG_ARG0(L_ERROR, DBG_CELLID,cellCfgReqSdus->hdr.cellId,
               "Cell Modify/Release: cell not found");
            NH_FILL_NHU_ERROR(*errInfo, NHU_ERRTYPE_INV_NHU_HDR,
                                        NHU_ERRCAUSE_CELL_NOT_CONFIGURED);
            RETVALUE(RFAILED);
         }

         /* check if there is an ongoing cell related transaction active already.
          * RRC does not support simultaneous cell configurations on the same cell
          */
         if (NH_USED == cellCb->cellTransCb.status)
         {
            RLOG_ARG0(L_ERROR, DBG_CELLID,cellCfgReqSdus->hdr.cellId,
               "Cell Modify: cellId has ongoing transaction");
            NH_FILL_NHU_ERROR(*errInfo, NHU_ERRTYPE_INV_CFG,
                                        NHU_ERRCAUSE_CFG_IN_PROGRESS);
            RETVALUE(RFAILED);
         }

         if (!((cellReCfgInfo->cellCfgBitMsk & NHU_CELL_CFG_MAC_CMN)
#ifdef RRC_PHY_CFG_SUPP
            || (cellReCfgInfo->cellCfgBitMsk & NHU_CELL_PHY_CFG)))
#else /* RRC_PHY_CFG_SUPP */
            ))
#endif /* RRC_PHY_CFG_SUPP */
         {
            RLOG1(L_ERROR,"Invalid cell config bitmask[%d] in cell cfg",
               cellReCfgInfo->cellCfgBitMsk);
            NH_FILL_NHU_ERROR(*errInfo, NHU_ERRTYPE_INV_CFG,
                                        NHU_ERRCAUSE_INV_BITMASK);
            RETVALUE(RFAILED);
         }

#ifdef RRC_PHY_CFG_SUPP
         /* check if atleast one element is present for reconfig */
         if ((cellReCfgInfo->cellCfgBitMsk & NHU_CELL_PHY_CFG) &&
             !((cellReCfgInfo->phyCellCfg.bitmask & NHU_CMN_PHY_CFG_PRACH) &&
             (cellReCfgInfo->phyCellCfg.bitmask & NHU_CMN_PHY_CFG_PDSCH) &&
             (cellReCfgInfo->phyCellCfg.bitmask & NHU_CMN_PHY_CFG_PUSCH) &&
             (cellReCfgInfo->phyCellCfg.bitmask & NHU_CMN_PHY_CFG_PHICH) &&
             (cellReCfgInfo->phyCellCfg.bitmask & NHU_CMN_PHY_CFG_PUCCH) &&
             (cellReCfgInfo->phyCellCfg.bitmask & NHU_CMN_PHY_CFG_UL_SRS) &&
             (cellReCfgInfo->phyCellCfg.bitmask & NHU_CMN_PHY_CFG_TDD_SF)))
         {
            RLOG0(L_ERROR, "Invalid PHY config for cell ");
            NH_FILL_NHU_ERROR(*errInfo, NHU_ERRTYPE_INV_CFG,
                                        NHU_ERRCAUSE_INV_PHYCFG_BITMASK);
            RETVALUE(RFAILED);
         }
#endif /* RRC_PHY_CFG_SUPP */
      }
      break;
      case NHU_CFGTYPE_RELEASE:
      {
         if (NULLP == cellCb)
         {
            RLOG_ARG0(L_ERROR, DBG_CELLID,cellCfgReqSdus->hdr.cellId,
               "Cell Release: cell not found");
            NH_FILL_NHU_ERROR(*errInfo, NHU_ERRTYPE_INV_NHU_HDR,
                                        NHU_ERRCAUSE_CELL_NOT_CONFIGURED);
            RETVALUE(RFAILED);
         }

         /* Check if there is an ongoing cell related transaction active already.
          * RRC does not support simultaneous cell configurations on the same 
            cell */
         if (NH_USED == cellCb->cellTransCb.status)
         {
            RLOG_ARG0(L_ERROR, DBG_CELLID,cellCfgReqSdus->hdr.cellId,
               "Cell Modify/Release: cellId has ongoing transaction");
            NH_FILL_NHU_ERROR(*errInfo, NHU_ERRTYPE_INV_CFG,
                                        NHU_ERRCAUSE_CFG_IN_PROGRESS);
            RETVALUE(RFAILED);
         }
      }
      break;
      default:
      {
         RLOG1(L_ERROR, "Invalid config type(cfgType: [%d]) in cell cfg",
            cellCfgReqSdus->sdu.cfgType);
         NH_FILL_NHU_ERROR(*errInfo, NHU_ERRTYPE_INV_CFG, 
                                     NHU_ERRCAUSE_INV_CFG_TYPE);
         RETVALUE(RFAILED);
      }
   }

   RETVALUE(ROK);
} /* nhUTLValidateCellCfgReq */


/**
 * @brief Retrieves the lower SAP control block.
 *
 * @details  
 * This function retrieves the lower SAP control block based SAP type
 * on SAP type and service user ID.  
 *
 * @param[in] sapType   SAP Type: CKW, CRG, CPJ, KWU, PJU
 * @param[in] lSapId    SAP Identifier
 *
 * @return S16 
 * -# ROK     Validation Success
 * -# RFAILED Validation Failure       
*/
#ifdef ANSI
PUBLIC NhLSapCb* nhUTLFindLlyrSapFrmSuId
(
U8                 sapType,           /* Type of SAP */
SuId               lSapId             /* Lower SAP ID */
)
#else /* ANSI */
PUBLIC NhLSapCb* nhUTLFindLlyrSapFrmSuId(sapType, lSapId)
U8                 sapType;           /* Type of SAP */
SuId               lSapId;            /* Lower SAP ID */
#endif /* ANSI */
{ 
   NhLSapCb   *lSapCb = NULLP; 
   
   /* nh004.201 - Correction to TRC functionality. */
   TRC2(nhUTLFindLlyrSapFrmSuId)

   switch(sapType)
   {
      case STNHKWUSAP:
#if (ERRCLASS & ERRCLS_DEBUG)
         if ((lSapId >= (SuId)nhCb.genCfg.nmbKwuSaps) || (lSapId < 0))
         {
            /* Invalid SAP ID. This must never happen.
             * Drop message, return failure.
             */
            RLOG0(L_ERROR,"Invalid KWU spId");
            RETVALUE(NULLP);
         }
#endif /* ERRCLASS */
           
         lSapCb = *(nhCb.kwuSapCbLst + lSapId);  
         break;

      case STNHCKWSAP:
#if (ERRCLASS & ERRCLS_DEBUG)
         if ((lSapId >= (SuId)nhCb.genCfg.nmbCkwSaps) || (lSapId < 0))
         {
            /* Invalid SAP ID. This must never happen.
             * Drop message, return failure.
             */
            RLOG0(L_ERROR,"Invalid CKW spId");
            RETVALUE(NULLP);
         }
#endif /* ERRCLASS */
           
         lSapCb = *(nhCb.ckwSapCbLst + lSapId);  
         break;

      case STNHCRGSAP:
#if (ERRCLASS & ERRCLS_DEBUG)
         if ((lSapId >= (SuId)nhCb.genCfg.nmbCrgSaps) || (lSapId < 0))
         {
            /* Invalid SAP ID. This must never happen.
             * Drop message, return failure.
             */
            RLOG0(L_ERROR,"Invalid CRG spId");
            RETVALUE(NULLP);
         }
#endif /* ERRCLASS */
           
         lSapCb = *(nhCb.crgSapCbLst + lSapId);  
         break;

      case STNHCPJSAP:
#if (ERRCLASS & ERRCLS_DEBUG)
         if ((lSapId >= (SuId)nhCb.genCfg.nmbCpjSaps) || (lSapId < 0))
         {
            /* Invalid SAP ID. This must never happen.
             * Drop message, return failure.
             */
            RLOG0(L_ERROR,"Invalid CPJ spId");
            RETVALUE(NULLP);
         }
#endif /* ERRCLASS */
           
         lSapCb = *(nhCb.cpjSapCbLst + lSapId);  
         break;

      case STNHPJUSAP:
#if (ERRCLASS & ERRCLS_DEBUG)
         if ((lSapId >= (SuId)nhCb.genCfg.nmbPjuSaps) || (lSapId < 0))
         {
            /* Invalid SAP ID. This must never happen.
             * Drop message, return failure.
             */
            RLOG0(L_ERROR,"Invalid PJU spId");
            RETVALUE(NULLP);
         }
#endif /* ERRCLASS */
           
         lSapCb = *(nhCb.pjuSapCbLst + lSapId);  
         break;

#ifdef RRC_PHY_CFG_SUPP
      case STNHCTFSAP:
#if (ERRCLASS & ERRCLS_DEBUG)
         if ((lSapId >= (SuId)nhCb.genCfg.nmbCtfSaps) || (lSapId < 0))
         {
            /* Invalid SAP ID. This must never happen.
             * Drop message, return failure.
             */
            RLOG0(L_ERROR,"Invalid CTF spId");
            RETVALUE(NULLP);
         }
#endif /* ERRCLASS */
           
         lSapCb = *(nhCb.ctfSapCbLst + lSapId);  
         break;
#endif /* RRC_PHY_CFG_SUPP */

      default:
         RLOG0(L_ERROR,"Invalid sapType ");
         RETVALUE(NULLP);
   }
  

   RETVALUE(lSapCb);
     
} /* nhUTLFindLlyrSapFrmSuId */




/**
 * @brief Validates messages against the allowed SRB IDs.
 *
 * @details
 * This function validates whether the message is
 * allowed to be received from the indicated SRB.
 *
 *
 * @param[in] ueCb      UE context
 * @param[in] rbId    PDCP ID
 * @param[in] msgType   Message Type as per ASN
 *
 * @return S16 
 * -# ROK     Validation Success
 * -# RFAILED Validation Failure
 */
#ifdef ANSI
PUBLIC S16 nhUTLValidateSRBforMsgRx
(
NhActvUeCb     *ueCb,
U8             rbId,
U8             msgType
)
#else /* ANSI */
PUBLIC S16 nhUTLValidateSRBforMsgRx(ueCb, rbId, msgType)
NhActvUeCb     *ueCb;
U8             rbId;
U8             msgType;
#endif /* ANSI */
{
   TRC2(nhUTLValidateSRBforMsgRx)

   switch(msgType)
   {
      case C1_ULINFOTFR:
      case C1_MEASUREMENTREPORT:
         if((NH_RB_ID_SRB1 != rbId) && (NH_RB_ID_SRB2 != rbId))
         {
            RLOG2(L_ERROR, "SRB validation failed for message[%d ] received in "
               "RB ID: [%d]", msgType, rbId);
            RETVALUE(RFAILED);
         }
         break;
      case C1_RRCCONRECFGNCOMPL:
      case C1_RRCCONSETUPCOMPL:
      case C1_SECURITYMODECOMPL:
      case C1_SECURITYMODEFAIL:
      case C1_RRCCONREESTBCOMPL:
      case C1_UECAPINFO:
      case C1_COUNTERCHECKRSP:
      case C1_CSFBPARAMSRQSTCDMA2000:
      case C1_ULHOVRPREPTFR:
#ifdef LTERRC_REL9
         /* R9 Upgrade: SRB1 must be configured before receiving UE Info RSP
          * and Proximity indication from UE*/
      case C1_UEINFORSP_R9__2:
      case C1_PROXIMITYIND_R9__2:
#endif /* LTERRC_REL9*/
         if(NH_RB_ID_SRB1 != rbId)
         {
            RLOG2(L_ERROR, "SRB validation failed for message[%d ] received in "
               "RB ID: [%d]", msgType, rbId);
            RETVALUE(RFAILED);
         }
         break;
      default:
         RLOG0(L_ERROR,"Unknown message received.");
         RETVALUE(RFAILED);
   } /* end of switch */

   RETVALUE(ROK);
} /* nhUTLValidateSRBforMsgRx */


/**
 * @brief Fills the default configuration.
 *     
 * @details 
 * This function fills the default configuration for RLC, 
 * Transport Channel, and Logical Channel.
 *            
 * @param[in] cellAndUeCb  Cell & UE control block pointer
 * @param[in] ueCfg        UE configuration
 *
 * @return S16 
 * -# ROK     
 * -# RFAILED
 */
#ifdef ANSI
PUBLIC S16 nhUTLFillDfltConfig
(
NhCellAndUeCb   *cellAndUeCb,
NhuUeCfgInfo    *ueCfg
)
#else /* ANSI */
PUBLIC S16 nhUTLFillDfltConfig(cellAndUeCb, ueCfg)
NhCellAndUeCb   *cellAndUeCb;
NhuUeCfgInfo    *ueCfg;
#endif /* ANSI */
{
   U8 cnt;
   NhuRbCfgInfo *rbCfgInfo;
#ifdef RRC_PHY_CFG_SUPP
   NhuPhyCfg    *phyCfg; 
#endif /* RRC_PHY_CFG_SUPP */

   TRC2(nhUTLFillDfltConfig)

#ifndef RRC_PHY_CFG_SUPP
   UNUSED(cellAndUeCb);
#endif /* RRC_PHY_CFG_SUPP */

   if (ueCfg->ueCfgBitMsk & NHU_EXP_CFG_RB_CFG)
   {
      if (ueCfg->rbCfgAllList.rbCfgTypBitMsk & NHU_CFG_RB_ADD)
      {
         for(cnt = 0; cnt < ueCfg->rbCfgAllList.rbToAddCfgList.numOfRbCfgInfo;
               cnt++)
         {
            rbCfgInfo = 
                    &(ueCfg->rbCfgAllList.rbToAddCfgList.rbCfgInfoList[cnt]);

            if((rbCfgInfo->rbId.rbId == NH_RB_ID_SRB1) ||
                  (rbCfgInfo->rbId.rbId == NH_RB_ID_SRB2))
            {
               /* If RLC & RB CFG is Uplink and the cfg type is Default,
                * then perform default configuration */
               if((rbCfgInfo->rbCfgBitMsk & NHU_RB_CFG_UL) &&
                     (rbCfgInfo->ulRbCfg.ulRbBitMsk & NHU_RB_CFG_RLC_MODE_UL) &&
                     (rbCfgInfo->ulRbCfg.ulRlcMode.rlcCfgType == 
                                          NHU_CFG_TYPE_DFLT))
               {
                  rbCfgInfo->ulRbCfg.ulRlcMode.mode = NHU_RLC_MODE_AM;
                  rbCfgInfo->ulRbCfg.ulRlcMode.m.amRlc.tStatusProh = 
                                          NHU_STATUS_PROH_MS0;  
                  rbCfgInfo->ulRbCfg.ulRlcMode.m.amRlc.tReordring = 
                                          NHU_TREORD_MS35; 
               }/* if(rbCfgInfo->rbCfgBitMsk & NHU_RB_CFG_UL &&) */


               /* If RLC & RB CFG is Uplink and the cfg type is Default,
                * then perform default configuration */
               if((rbCfgInfo->rbCfgBitMsk & NHU_RB_CFG_UL) &&
                    (rbCfgInfo->ulRbCfg.ulRbBitMsk & NHU_RB_CFG_LOGCH_MAP_UL) &&
                     (rbCfgInfo->ulRbCfg.ulLogChMap.lcCfgType == 
                      NHU_CFG_TYPE_DFLT))
               {
                  rbCfgInfo->ulRbCfg.ulLogChMap.lcCfg.logChCfgpres =
                     NHU_UL_LOGCH_MAP_GRP; 
                  rbCfgInfo->ulRbCfg.ulLogChMap.lcCfg.logChGrp = 0;
               }


               /* If RLC & RB CFG is Downlink and the cfg type is Default,
                * then perform default configuration */
               if((rbCfgInfo->rbCfgBitMsk & NHU_RB_CFG_DL) &&
                     (rbCfgInfo->dlRbCfg.dlRbBitMsk & NHU_RB_CFG_RLC_MODE_DL) &&
                     (rbCfgInfo->dlRbCfg.dlRlcMode.rlcCfgType == 
                      NHU_CFG_TYPE_DFLT))
               {
                  rbCfgInfo->dlRbCfg.dlRlcMode.mode = NHU_RLC_MODE_AM;
                  rbCfgInfo->dlRbCfg.dlRlcMode.m.amRlc.maxRetxThresh =
                     NHU_MAX_RETX_THRES_T4;  
                  rbCfgInfo->dlRbCfg.dlRlcMode.m.amRlc.pollByte = 
                          NHU_POLL_BYTE_INF;  
                  rbCfgInfo->dlRbCfg.dlRlcMode.m.amRlc.pollPdu = 
                          NHU_POLL_PDU_INF;  
                  rbCfgInfo->dlRbCfg.dlRlcMode.m.amRlc.tPollRetx = 
                          NHU_TPOLL_RETX_MS45;  
               }/* if(rbCfgInfo->rbCfgBitMsk & NHU_RB_CFG_DL &&) */
            } /* if((rbCfgInfo->rbId == NH_RB_ID_SRB1) */
         } /* End of For loop */
      } /*  if (ueCfg->rbCfgAllList.rbCfgTypBitMsk & NHU_CFG_RB_ADD) */

      if (ueCfg->rbCfgAllList.rbCfgTypBitMsk & NHU_CFG_RB_MODIFY)
      {
         for(cnt = 0; cnt < ueCfg->rbCfgAllList.rbToModCfgList.numOfRbCfgInfo;
               cnt++)
         {
            rbCfgInfo = 
                    &(ueCfg->rbCfgAllList.rbToModCfgList.rbCfgInfoList[cnt]);

            if((rbCfgInfo->rbId.rbId == NH_RB_ID_SRB1) ||
                  (rbCfgInfo->rbId.rbId == NH_RB_ID_SRB2))
            {
               /* If RLC & RB CFG is Uplink and the cfg type is Default, 
                * then perform default configuration */
               if((rbCfgInfo->rbCfgBitMsk & NHU_RB_CFG_UL) &&
                     (rbCfgInfo->ulRbCfg.ulRbBitMsk & NHU_RB_CFG_RLC_MODE_UL) &&
                     (rbCfgInfo->ulRbCfg.ulRlcMode.rlcCfgType == 
                                NHU_CFG_TYPE_DFLT))
               {
                  rbCfgInfo->ulRbCfg.ulRlcMode.mode = NHU_RLC_MODE_AM;
                  rbCfgInfo->ulRbCfg.ulRlcMode.m.amRlc.tStatusProh = 
                                NHU_STATUS_PROH_MS0;  
                  rbCfgInfo->ulRbCfg.ulRlcMode.m.amRlc.tReordring = 
                                NHU_TREORD_MS35; 
               }/* if(rbCfgInfo->rbCfgBitMsk & NHU_RB_CFG_UL &&) */

               /* If RLC & RB CFG is Uplink and the cfg type is Default,
                * then perform default configuration */
               if((rbCfgInfo->rbCfgBitMsk & NHU_RB_CFG_UL) &&
                    (rbCfgInfo->ulRbCfg.ulRbBitMsk & NHU_RB_CFG_LOGCH_MAP_UL) &&
                     (rbCfgInfo->ulRbCfg.ulLogChMap.lcCfgType == 
                                NHU_CFG_TYPE_DFLT))
               {
                  rbCfgInfo->ulRbCfg.ulLogChMap.lcCfg.logChCfgpres =
                     NHU_UL_LOGCH_MAP_GRP; 
                  rbCfgInfo->ulRbCfg.ulLogChMap.lcCfg.logChGrp = 0;
               }

               /* If RLC & RB CFG is Downlink and the cfg type is Default,
                * then perform default configuration */
               if((rbCfgInfo->rbCfgBitMsk & NHU_RB_CFG_DL) &&
                     (rbCfgInfo->dlRbCfg.dlRbBitMsk & NHU_RB_CFG_RLC_MODE_DL) &&
                     (rbCfgInfo->dlRbCfg.dlRlcMode.rlcCfgType == 
                                NHU_CFG_TYPE_DFLT))
               {
                  rbCfgInfo->dlRbCfg.dlRlcMode.mode = NHU_RLC_MODE_AM;
                  rbCfgInfo->dlRbCfg.dlRlcMode.m.amRlc.maxRetxThresh =
                     NHU_MAX_RETX_THRES_T4;  
                  rbCfgInfo->dlRbCfg.dlRlcMode.m.amRlc.pollByte = 
                                NHU_POLL_BYTE_INF;  
                  rbCfgInfo->dlRbCfg.dlRlcMode.m.amRlc.pollPdu = 
                                NHU_POLL_PDU_INF;  
                  rbCfgInfo->dlRbCfg.dlRlcMode.m.amRlc.tPollRetx = 
                                NHU_TPOLL_RETX_MS45;  
               }/* if(rbCfgInfo->rbCfgBitMsk & NHU_RB_CFG_DL &&) */
            } /* if((rbCfgInfo->rbId == NH_RB_ID_SRB1) */
         } /* End of For loop */
      }/*  if (ueCfg->rbCfgAllList.rbCfgTypBitMsk & NHU_CFG_RB_MOD) */
   } /* if (ueCfg->ueCfgBitMsk & NHU_EXP_CFG_RB_CFG) */
   
   if (ueCfg->ueCfgBitMsk & NHU_EXP_CFG_TRCH_CFG && 
       ueCfg->trChCfg.trChCfgType == NHU_CFG_TYPE_DFLT)
   {
      ueCfg->trChCfg.trChBitMsk |= NHU_TRCH_CFG_UL_HARQ; 
      ueCfg->trChCfg.ulHarqCfg.ulMaxHarqTx = NHU_MAX_HARQ_TX_N5;
   }

#ifdef RRC_PHY_CFG_SUPP
   /* Fill the default PHY config params [Ref: 36.331, 9.2.4] */
   if (ueCfg->ueCfgBitMsk & NHU_EXP_CFG_PHY_CFG) 
   {
      phyCfg = &(ueCfg->phyCfg);
      
      /* Default PDSCH config */
      if ((phyCfg->bitMask & NHU_DED_PHY_CFG_PDSCH) &&
          (phyCfg->pdschCfg.cfgTyp == NHU_CFG_TYPE_DFLT))
      {
         phyCfg->pdschCfg.pa = NHU_PDSCH_PA_ZERO;
      }
      /* Default PUCCH config */
      if ((phyCfg->bitMask & NHU_DED_PHY_CFG_PUCCH) &&
          (phyCfg->pucchCfg.cfgTyp == NHU_CFG_TYPE_DFLT))
      {
         phyCfg->pucchCfg.choice = NHU_CFGTYPE_RELEASE;
      }
      /* Default PUSCH config */
      if ((phyCfg->bitMask & NHU_DED_PHY_CFG_PUSCH) &&
          (phyCfg->puschCfg.cfgTyp == NHU_CFG_TYPE_DFLT))
      {
         phyCfg->puschCfg.betaOffsetAckIdx = 10;
         phyCfg->puschCfg.betaOffsetRiIdx = 12;
         phyCfg->puschCfg.betaOffsetCqiIdx = 15;
      }
      /* Default CQI config */
      if ((phyCfg->bitMask & NHU_DED_PHY_CFG_CQI) &&
          (phyCfg->cqiCfg.cfgType == NHU_CFG_TYPE_DFLT))
      {
#ifdef NHU_VER2
	      /* Added simultaneous config of periodic and aperiodic cqi */
	      phyCfg->cqiCfg.reportMode.aPeriodicRpt.pres = FALSE;
	      phyCfg->cqiCfg.reportMode.periodicRpt.pres  = TRUE;
#else /* NHU_VER2 */
         phyCfg->cqiCfg.reportingMode = NHU_CQI_RPTMODE_PERIODIC;
#endif /* NHU_VER2 */
         phyCfg->cqiCfg.reportMode.periodicRpt.cqiPeriodicCfgType
                      = NHU_CFGTYPE_RELEASE;
      }
      /* Default UL SRS config */
      if ((phyCfg->bitMask & NHU_DED_PHY_CFG_UL_SRS) &&
          (phyCfg->srsUlCfg.cfgTyp == NHU_CFG_TYPE_DFLT))
      {
         phyCfg->srsUlCfg.choice = NHU_CFGTYPE_RELEASE;
      }
      /* Default UL SR config */
      if ((phyCfg->bitMask & NHU_DED_PHY_CFG_SR) &&
          (phyCfg->srCfg.cfgTyp == NHU_CFG_TYPE_DFLT))
      {
         phyCfg->srCfg.dedSRCfgType = NHU_CFGTYPE_RELEASE;
      }
      /* Default Antenna config */
      if ((phyCfg->bitMask & NHU_DED_PHY_CFG_ANT) &&
          (phyCfg->antCfg.cfgTyp == NHU_CFG_TYPE_DFLT))
      {
         /* 36.331, 9.2.4 : If the number of PBCH antenna ports is one, 
            tm1 is used as default; otherwise tm2 is used as default */
         if(cellAndUeCb->cellCb->crntCellCfgInfo.phyCfg.numOfTxAnt == 1)
            phyCfg->antCfg.txMode = NHU_DED_ANT_CFG_TX_MODE_TM1;
         else
            phyCfg->antCfg.txMode = NHU_DED_ANT_CFG_TX_MODE_TM2;

         phyCfg->antCfg.txAntSelectchoice = NHU_CFGTYPE_RELEASE;
      }
   }
#endif /* RRC_PHY_CFG_SUPP */

   RETVALUE(ROK);
} /* nhUTLFillDfltConfig */

#ifdef NH_LOG_TO_FILE
/** 
 *
 * @brief
 * 
 * @details
 * 
 * This function copies a string from source to destination.
 * 
 * @param[in] tgt     Destination String Pointer
 * @param[in] src     Source String Pointer    
 * 
 * @return Void
 *  -# ROK
 **/
#ifdef ANSI
PRIVATE S16  nhStrcpy
(
S8             *tgt,          /* dest string*/
CONSTANT S8    *src           /* src string */
)
#else /* ANSI */
PRIVATE S16 nhStrcpy(tgt, src)
S8                *tgt;         /* dest string*/
CONSTANT S8       *src;         /* src string */
#endif /* ANSI */
{
   PTR len;

   /* nh004.201 - Correction to TRC functionality */
   TRC2(nhStrcpy)

   len = cmStrlen( (CONSTANT U8* )src) + 1;

   cmMemcpy( (U8* )tgt, (CONSTANT U8* )src, len);

   RETVALUE(ROK);
} /* nhStrcpy */


/** 
 *
 * @brief
 * 
 * @details
 * 
 * This function concatenates two strings.
 * 
 * @param[in] tgt     Destination String Pointer
 * @param[in] src     Source String Pointer    
 * 
 * @return Void
 *  -# ROK
 **/
#ifdef ANSI
PRIVATE S16  nhStrcat
(
S8    *tgt,          /* dest string*/
S8    *src           /* src string */
)
#else /* ANSI */
PRIVATE S16 nhStrcat(tgt, src)
S8    *tgt;         /* dest string*/
S8    *src;         /* src string */
#endif /* ANSI */
{
   U16 srcIdx = 0;
   U16 destIdx = 0;

   /* nh004.201 - Correction to TRC functionality */
   TRC2(nhStrcat)

   while (tgt[destIdx++]);
   for (destIdx--; src[srcIdx]; (tgt[destIdx++] = src[srcIdx++]));

   RETVALUE(ROK);
} /* end of nhStrcat */

/***********************************************************
 *
 *     Func : nhUTLCreateDbgFile
 *
 *     Desc : Creates the file for logging RRC debug logs
 *            in the path given via layer manager.
 *
 *     Ret  : ROK - success
 *            RFAILED - failure
 *
 *     Notes: None
 *
 *     File : rr_utl.c
 *
 **********************************************************/
#ifdef ANSI
PUBLIC Void nhUTLCreateDbgFile
(
Void
)
#else /* ANSI */
PUBLIC Void nhUTLCreateDbgFile(Void)
#endif /* ANSI */
{
   Txt   fName[LNH_MAX_FILE_PATH]; /* to hold the Date & Time in File name */
   Txt   outfName[LNH_MAX_FILE_PATH * 2]; /* file name with prefix, Date & Time and Suffix */
   Txt   *prefName = "nh_"; /* file prefix */
   Txt   *sufFname = ".log"; /* file suffix */
   S16   strLen;
   S16   cnt;
   SystemId sysId;
   S16   retVal;
   Txt   sysIdInfo[LNH_MAX_FILE_PATH];
   time_t timeVal;
   struct tm tmStmp;

   /* nh004.201 - Correction to TRC functionality */
   TRC2(nhUTLCreateDbgFile)

   strLen = 0; 
   cnt = 0;
   retVal = ROK;

   cmMemset((U8*)fName, 0, LNH_MAX_FILE_PATH);
   cmMemset((U8*)outfName, 0, (LNH_MAX_FILE_PATH * 2));
   cmMemset((U8*)&sysId, 0, sizeof(SystemId));
   cmMemset((U8*)sysIdInfo, 0, LNH_MAX_FILE_PATH);

   time(&timeVal);
   localtime_r(&timeVal, &tmStmp);

#ifdef SS_LINUX
   asctime_r(&tmStmp, (S8*)fName);
#endif /* SS_LINUX */

   strLen = cmStrlen((U8*)fName);

   for(cnt = 0; cnt < strLen; cnt++)
   {
      if((fName[cnt] == ' ') || (fName[cnt] == ':'))
      {
         fName[cnt] = '-';
      }
   }

   /* copy the file path given by LM into the file name */
   nhStrcpy(outfName, nhCb.filePath);

   /* copy the file prefix, "rr_" into the file name */
   nhStrcat(outfName, prefName);

   /* copy the file name string made out of Date & Time */
   nhStrcat(outfName, fName);

   /* remove the extra char at the end */
   strLen = cmStrlen((U8*)outfName);
   outfName[strLen-1] = '\0';

   /* copy the suffix, ".log"  into the file name */
   nhStrcat(outfName, sufFname);

   /* check whether file we have the valid file handle or not */
   if(nhCb.dbgFp)
      fclose(nhCb.dbgFp);

   /* open the file in append mode */
   nhCb.dbgFp = fopen(outfName, "a");

   /* add the RRC Protocol Information in the begining of the file */
   retVal = nhGetSId(&sysId);

   if((retVal == ROK) && (nhCb.dbgFp))
   {
      sprintf(sysIdInfo, "--------- RRC Protocol Main Version    : \
                         %d ---------\n", sysId.mVer);
      fwrite(sysIdInfo, cmStrlen((U8*)sysIdInfo), 1, nhCb.dbgFp);
      sprintf(sysIdInfo, "--------- RRC Protocol Main Revision   : \
                         %d ---------\n", sysId.mRev);
      fwrite(sysIdInfo, cmStrlen((U8*)sysIdInfo), 1, nhCb.dbgFp);
      sprintf(sysIdInfo, "--------- RRC Protocol Branch Version  : \
                         %d ---------\n", sysId.bVer);
      fwrite(sysIdInfo, cmStrlen((U8*)sysIdInfo), 1, nhCb.dbgFp);
      sprintf(sysIdInfo, "--------- RRC Protocol Branch Revision : \
                         %d ---------\n", sysId.bRev);
      fwrite(sysIdInfo, cmStrlen((U8*)sysIdInfo), 1, nhCb.dbgFp);
      if(sysId.ptNmb)
      {
         sprintf(sysIdInfo, "--------- RRC Protocol Part Number     : \
                            %s ---------\n\n", sysId.ptNmb);
         fwrite(sysIdInfo, cmStrlen((U8*)sysIdInfo), 1, nhCb.dbgFp);
      }
   }

   RETVOID;
} /* nhUTLCreateDbgFile */

#endif /* NH_LOG_TO_FILE */

#ifdef NH_TRACE_ENABLED
/**
 * @brief Send a trace indication to the layer manager
 *     
 * @details 
 * This function sends the message trace dump to the layer manager
 * received/transmitted at the lower interface.
 *            
 *
 * @param[in] suId     lower sap Id
 * @param[in] evnt     
 * @param[in] mBuf     message buffer received or transmitted at the lower
 *                     interface.
 *
 * @return S16 
 * -# ROK     
 * -# RFAILED
 */
#ifdef ANSI
PUBLIC Void nhUTLGenTrc
(
SuId suId,   
U8 sapType, 
U16 evnt,   
Buffer *mBuf 
)
#else /* ANSI */
PUBLIC Void nhUTLGenTrc(suId, sapType, evnt, mBuf)
SuId suId;  
U8  sapType; 
U16  evnt; 
Buffer *mBuf; 
#endif /* ANSI */
{
   NhMngmt    trc;                      
   MsgLen     msgLen;
  
   TRC2(nhUTLGenTrc);

   NH_SET_ZERO((U8*)&trc, sizeof(NhMngmt));

   trc.hdr.msgLen = 0;                   /* optional - message length */
   trc.hdr.version = 0;                  /* optional - version */
   trc.hdr.seqNmb = 0;                   /* optional - sequence number */
   trc.hdr.msgType = TTRC;               /* message type */
   trc.hdr.entId.ent = nhCb.init.ent;  /* entity id */
   trc.hdr.entId.inst = nhCb.init.inst;/* instance id */
 
   /* SAP where event occured */
   trc.hdr.elmId.elmnt = sapType;        /* lower SAP */
   trc.hdr.elmId.elmntInst1 = suId;      /* SAP number */
   trc.hdr.elmId.elmntInst2 = 0;         /* optional - element instance */
   trc.hdr.elmId.elmntInst3 = 0;         /* optional - element instance */
 
   /* event */
   trc.u.trc.evnt = evnt;                /* event to be traced */

   /* find length of message buffer */
   (Void)SFndLenMsg(mBuf, &msgLen);
 
   /* trace upto LNH_MAX_TRACE_LEN bytes of message buffer */
   if(msgLen > LNH_MAX_TRACE_LEN)
      msgLen = LNH_MAX_TRACE_LEN;

   {
      MsgLen tmp;
      (void)SCpyMsgFix (mBuf, 0, msgLen, &trc.u.trc.evntParm[0], &tmp);
   }

   /* store length of trace */
   trc.u.trc.len = msgLen;

   /* initialize date and time in trace structure */
   SGetDateTime(&trc.u.trc.dt);
 
   /* call management trace indication primitive */
   NhMiLnhTrcInd(&nhCb.init.lmPst, &trc);
 
   RETVOID;
} /* nhGenTrc */

#endif /* NH_TRACE_ENABLED */

/**
 * @brief Returns RB's to re-establish list.
 * 
 * @details 
 * 
 *
 *
 * This function populates a list of RB's to re-establish 
 * by calculating a delta between RB's present in current
 * Rb configuration of UE CB and RB's to be released and modified
 * present in UE configuration information.
 * 
 *
 * @param[in] transCtrlBlk          UE Transaction Context
 * @param[in] crntRbCfgList    Current RB configuration list of a UE.
 *
 * @return S16
 *  -# ROK
 *  -# RFAILED
*/
#ifdef ANSI
PUBLIC S16 nhUTLGetRbToRestb
(
TransCb         *transCtrlBlk,
NhRbCfgList     crntRbCfgList
)
#else /* ANSI */
PUBLIC S16 nhUTLGetRbToRestb(transCtrlBlk, crntRbCfgList )
TransCb         *transCtrlBlk;
NhRbCfgList     crntRbCfgList;
#endif /* ANSI */
{
   NhRbCfgList  *rbCfgList = NULLP;
   NhuRbCfgInfo *rbInfo = NULLP;
   CmLList      *tmpNode = NULLP;
   CmLList      *crntRbInf = NULLP;
   Bool         found;
   U8           i;

   TRC2(nhUTLGetRbToRestb);
   
   RLOG0(L_DEBUG, "nhUTLGetRbToRestb called.");

   rbCfgList = &transCtrlBlk->ordrdCfg.rbToRestbCfgList;

   CM_LLIST_FIRST_NODE(&crntRbCfgList.rbCfgInfoList, crntRbInf);
   while(NULLP != crntRbInf)
   {
       /*if RB is is SRB1 skip it as its already been re-established*/
       if(NH_RB_ID_SRB1 == ((NhuRbCfgInfo *) crntRbInf->node)->rbId.rbId &&
          CM_LTE_SRB == ((NhuRbCfgInfo *) crntRbInf->node)->rbId.rbTyp)
       {
          CM_LLIST_NEXT_NODE(&crntRbCfgList.rbCfgInfoList, crntRbInf);
          continue;
       }
       found = FALSE;
       /* Check if there are RB's provided for release or modify.
          These RB's should not be included in re-establish list.
          If there are no RB's then the all RB's in current config
          list get included
       */
       for(i=0; i < (transCtrlBlk->ordrdCfg.rbToModCfgList.numOfRbCfgInfo) || 
                i< (transCtrlBlk->ordrdCfg.rbToRlsCfgList.numOfRbCfgInfo); i++)
       {
          if(i<transCtrlBlk->ordrdCfg.rbToModCfgList.numOfRbCfgInfo)
          {
             if(((NhuRbCfgInfo *) crntRbInf->node)->rbId.rbId == 
                transCtrlBlk->ordrdCfg.rbToModCfgList.rbCfgInfoList[i].rbId.rbId
              && ((NhuRbCfgInfo *) crntRbInf->node)->rbId.rbTyp == 
              transCtrlBlk->ordrdCfg.rbToModCfgList.rbCfgInfoList[i].rbId.rbTyp)
             {
                found = TRUE;
                break;
             }
          }
          if(i<transCtrlBlk->ordrdCfg.rbToRlsCfgList.numOfRbCfgInfo)
          {
            if(((NhuRbCfgInfo *) crntRbInf->node)->rbId.rbId
                == transCtrlBlk->ordrdCfg.rbToRlsCfgList.rbsToRel[i].rbId 
             && 
                ((NhuRbCfgInfo *) crntRbInf->node)->rbId.rbTyp
                == transCtrlBlk->ordrdCfg.rbToRlsCfgList.rbsToRel[i].rbTyp
             )
             {
                found = TRUE;
                break;
             }
          }
       }
       if(found == FALSE)
       {
          /*Update */
          NH_ALLOC(NH_REG, NH_POOL, &tmpNode, sizeof(CmLList));
          if(NULLP == tmpNode)
          {
             RLOG0(L_FATAL,"Memory Allocation failed for adding radio bearer.");
             RETVALUE(ROUTRES);
          }
          NH_ALLOC(NH_REG, NH_POOL, &rbInfo, sizeof(NhuRbCfgInfo)); 
          if(NULLP == rbInfo) 
          {
             RLOG0(L_FATAL,"Memory Allocation failed for adding radio bearer.");
             NH_FREE(NH_REG, NH_POOL, tmpNode, sizeof(CmLList));
             RETVALUE(ROUTRES);
          }
         if(CM_LTE_SRB == ((NhuRbCfgInfo *) crntRbInf->node)->rbId.rbTyp)
         {
            transCtrlBlk->rlcMapTbl.srbToCfg[NH_RB_TO_REST] |= 
                    (1 << (((NhuRbCfgInfo *) crntRbInf->node)->rbId.rbId-1));
         }
         else
         {
            transCtrlBlk->rlcMapTbl.drbToCfg[NH_RB_TO_REST] |= 
                    (1 << (((NhuRbCfgInfo *) crntRbInf->node)->rbId.rbId-1));
         }

         if(CM_LTE_SRB == ((NhuRbCfgInfo *) crntRbInf->node)->rbId.rbTyp)
         {
            transCtrlBlk->pdcpMapTbl.srbToCfg[NH_RB_TO_REST] |= 
                    (1 << (((NhuRbCfgInfo *) crntRbInf->node)->rbId.rbId-1));
         }
         else
         {
            transCtrlBlk->pdcpMapTbl.drbToCfg[NH_RB_TO_REST] |= 
                    (1 << (((NhuRbCfgInfo *) crntRbInf->node)->rbId.rbId-1));
         }

          NH_MEM_COPY((U8*)rbInfo, (U8*)crntRbInf->node, sizeof(NhuRbCfgInfo));
          tmpNode->node = (PTR)rbInfo; 
          cmLListAdd2Tail(&rbCfgList->rbCfgInfoList, tmpNode);
          rbCfgList->numOfRbCfgInfo++;
       }
       CM_LLIST_NEXT_NODE(&crntRbCfgList.rbCfgInfoList, crntRbInf); 
    }

   RETVALUE(ROK);
} /* nhUTLGetRbToRestb */

/**
 * @brief This function frees ulBitMap allocated memory from crntCfg in case
 *        of failure.
 *
 * @details
 *
 * This function frees memory of ulBitMap  from crntCfg in case of failure.
 * ulBitMap structure in pdcpSduStaInfo for each RB.
 *
 * @param[in] crntCfgInfo   current configuration from which ulBitMap memory
 *                          needs to be deleted.
 *
 * @return S16
 *  -# ROK
*/
#ifdef ANSI
PUBLIC S16 nhUTLFreeUlBitMapFrmCrntRbCfg
(
NhCrntCfgInfo *crntCfgInfo
)
#else /* ANSI */
PUBLIC S16 nhUTLFreeUlBitMapFrmCrntRbCfg(crntCfgInfo)
NhCrntCfgInfo *crntCfgInfo;
#endif /* ANSI */
{
   NhuHoPdcpSduInfo *hoPdcpSduStaInfo;
   CmLList          *tmpNode =  NULLP;
   Size             ulBitMapSize;

   TRC2(nhUTLFreeUlBitMapFrmCrntRbCfg)

   RLOG0(L_DEBUG, "nhUTLFreeUlBitMapFrmCrntRbCfg called  ");
 
   CM_LLIST_FIRST_NODE(&crntCfgInfo->rbCfgList.rbCfgInfoList, tmpNode); 

   while(NULLP != tmpNode)
   {
      hoPdcpSduStaInfo = &(((NhuRbCfgInfo *) tmpNode->node)->pdcpCfg.\
                         hoPdcpSduStaInfo);

      /* Get number of octets to free from numBits */
      NH_GET_UL_BITMAP_SIZE(hoPdcpSduStaInfo->numBits, ulBitMapSize);

      if((NULLP != hoPdcpSduStaInfo->ulBitMap) && (ulBitMapSize > 0))
      {
         NH_FREE(NH_REG, NH_POOL, hoPdcpSduStaInfo->ulBitMap, ulBitMapSize);
      }
      CM_LLIST_NEXT_NODE(&crntCfgInfo->rbCfgList.rbCfgInfoList, tmpNode);
   }
   RETVALUE(ROK);
} /* end of nhUTLFreeUlBitMapFrmCrntRbCfg */

/**
 * @brief This function frees ulBitMap allocated memory from ordered config 
 *        in case of failure or while deleting ueCb.
 *
 * @details
 *
 * This function frees memory of ulBitMap  from orderedCfg in case of failure
 *          while deleting ueCb.
 *
 * @param[in] transCtrlBlk   transaction control bock from which ulBitMap memory
 *                          needs to be deleted.
 * @param[in] numRbs    Number of RBs
 *
 * @return S16
 *  -# ROK
*/
#ifdef ANSI
PUBLIC S16 nhUTLFreeUlBitMapFrmOrdCfg
(
TransCb *transCtrlBlk,
U8      numRbs
)
#else /* ANSI */
PUBLIC S16 nhUTLFreeUlBitMapFrmOrdCfg(transCtrlBlk, numRbs)
TransCb *transCtrlBlk;
U8      numRbs;
#endif /* ANSI */
{
   S16        rbCnt;
   Size       ulBitMapSize;
   NhuHoPdcpSduInfo *hoPdcpSduStaInfo;
   U8         pdcpCfgBitMsk;

   TRC2(nhUTLFreeUlBitMapFrmOrdCfg)

   RLOG0(L_DEBUG, "nhUTLFreeUlBitMapFrmOrdCfg called  ");
 
   for(rbCnt = 0; rbCnt < numRbs; rbCnt++)
   {
      pdcpCfgBitMsk = transCtrlBlk->ordrdCfg.rbToModCfgList.\
             rbCfgInfoList[rbCnt].pdcpCfg.pdcpCfgBitMsk;
      if (pdcpCfgBitMsk & NHU_PDCP_SDU_STA_INFO)
      {
         hoPdcpSduStaInfo = &transCtrlBlk->ordrdCfg.rbToModCfgList.\
                   rbCfgInfoList[rbCnt].pdcpCfg.hoPdcpSduStaInfo;
         if( (NHU_HO_UL & hoPdcpSduStaInfo->isHoPres) &&
             (hoPdcpSduStaInfo->numBits > 0) )
         {
            /* Get number of octets to free from numBits */
            NH_GET_UL_BITMAP_SIZE(hoPdcpSduStaInfo->numBits,
                                  ulBitMapSize);

            NH_FREE(NH_REG, NH_POOL, hoPdcpSduStaInfo->ulBitMap,
                                ulBitMapSize);
         }
      }
   }
   RETVALUE(ROK);
} /* end of nhUTLFreeUlBitMapFrmOrdCfg */

#ifdef NH_ASN_COND_VALIDATE
/**
 * @brief Handler for validating conditional IEs.
 *
 * @details This function validates conditional IEs for RRC messages.
 *
 * @param[in] msgSdu    Event structure containing RRC message to be validated.
 * @param[in] msgType   RRC message type.
 * @param[in] errInfo   Error Info.
 *
 * @return S16
 *  -# ROK
 *  -# RFAILED
*/
#ifdef ANSI
PUBLIC S16 nhUTLValidateAsnCondIes
(
Void             *msgSdu,
U8               msgType,
NhuErrorInfo     *errInfo,
U8               evtType
)
#else /* ANSI */
PUBLIC S16 nhUTLValidateAsnCondIes(msgSdu, msgType, errInfo, evtType)
Void             *msgSdu;
U8               msgType;
NhuErrorInfo     *errInfo;
U8               evtType;
#endif /* ANSI */
{
   NhuRRCConRecfgn_r8_IEs  *reCfgIEs;
   NhCellCb                *cellCb;
   NhActvUeCb              *ueCb;
   S16                     retVal;
   NhCellAndUeCb           cellandUeCb;

   TRC2(nhUTLValidateAsnCondIes)

   RLOG0(L_DEBUG,"nhUTLValidateAsnCondIes called.");

   retVal = ROK;


   /* Extract cellCb using msgSdu->hdr.cellId. Typecasted DatReq, since all *
    * the event structures (EncReq,DatReq and DatRsp) have NhuHdr as first  *
    * field. cellCb is used to obtain ueCb. cellCb will be used to get      *
    * duplex mode(FDD or TDD)                                               */
   if (NULLP ==(cellCb = nhDBMGetCellCb(((NhuDatReqSdus *)msgSdu)->hdr.cellId)))
   {
      RLOG0(L_ERROR,"NULLP Cell Cb");
      NH_FILL_NHU_ERROR(*errInfo, NHU_ERRTYPE_INV_SDU_PARAM,
                        NHU_ERRCAUSE_COND_IE_FAIL);
      RETVALUE(RFAILED);
   }

   /* Extract ueCb using msgSdu->hdr.ueId */
   ueCb = nhDBMGetUeCb(cellCb, ((NhuDatReqSdus *)msgSdu)->hdr.ueId);

   cellandUeCb.cellCb = cellCb;
   cellandUeCb.ueCb = ueCb;
   errInfo->errCause = NH_ZERO_VAL;

   switch(msgType)
   {
      case NH_MSGTYPE_CONNSETUP:
            retVal = nhUTLValidateCondIesRRCConnSetup((NhuDatRspSdus *)msgSdu,
                                                &cellandUeCb);
            if (retVal != ROK)
            {
               RLOG0(L_ERROR, "Valdiation for RRCConnSetup failed.");
            }
            break;

      case NH_MSGTYPE_MOBFRMEUTRAN:
            retVal = nhUTLValidateCondIesMobFrmEutra((NhuDatReqSdus *)msgSdu,
                                                ueCb);
            if (retVal != ROK)
            {
               RLOG0(L_ERROR, "Valdiation for MOB-FRM-EUTRAN failed.");
            }
            break;

      case NH_MSGTYPE_HOFRMEUTRAPREPREQ:
            retVal = nhUTLValidateCondIesHoFrmEutraPrepTfr(
                                               (NhuDatReqSdus *)msgSdu, ueCb);
            if (retVal != ROK)
            {
               RLOG0(L_ERROR, "Valdiation for HO-FRM-EUTRA-PREP-REQ failed.");
            }
            break;

      case NH_MSGTYPE_SIB1:
      case NH_MSGTYPE_SI:
            retVal = nhUTLValidateCondIesSibs((NhuDatReqSdus *)msgSdu,
                                                  &cellandUeCb);
            if (retVal != ROK)
            {
               RLOG0(L_ERROR, "Valdiation for SIBs failed.");
            }
            break;

      case NH_MSGTYPE_CONNRECFG:
            NH_CONDIES_GET_SDU_TYPE(msgSdu, evtType, reCfgIEs);

            if(NH_IS_IE_PRSNT(reCfgIEs->mobilityCntrlInfo))
            {
               retVal = nhUTLValidateCondIesReCfgHo(reCfgIEs, &cellandUeCb);
            }
            else
            {
               retVal = nhUTLValidateCondIesRRCConnReCfg(reCfgIEs, 
                               &cellandUeCb);
            }

            if(ROK != retVal)
            {
               RLOG0(L_ERROR, "Valdiation for ConnReCfg failed.");
            }
            break;

      case NH_MSGTYPE_HOPREPINFO:
            retVal = nhUTLValidateCondIesHoPrepInfo((NhuEncReqSdus *)msgSdu,
                                                &cellandUeCb);
            if (retVal != ROK)
            {
               RLOG0(L_ERROR, "Valdiation for HO-PREP-INFO failed.");
            }
            break;

      default:
         /* Other messages do not have any conditional values to be validated */
         RETVALUE(ROK);
   }  /* end of switch */

   if(retVal != ROK)
   {
      RLOG0(L_ERROR,"conditional IE validation failed");
      NH_FILL_NHU_ERROR(*errInfo, NHU_ERRTYPE_INV_SDU_PARAM,
                        NHU_ERRCAUSE_COND_IE_FAIL);
      RETVALUE(RFAILED);
   }

   RETVALUE(ROK);
} /* end of nhUTLValidateAsnCondIes */

/**
 * @brief Handler for validating conditional IEs in HO Recfg message.
 *
 * @details This function validates conditional IEs for RRC Reconfiguration 
 *          message in case of HO.
 *
 * @param[in] reCfgIE   RRC Reconfiguration IE.
 * @param[in] ueCb      UE control block.
 *
 * @return S16
 *  -# ROK
 *  -# RFAILED
*/
#ifdef ANSI
PUBLIC S16 nhUTLValidateCondIesReCfgHo
(
NhuRRCConRecfgn_r8_IEs   *reCfgIEs,
NhCellAndUeCb            *cellandUeCb
)
#else /* ANSI */
PUBLIC S16 nhUTLValidateCondIesReCfgHo(reCfgIEs, cellandUeCb)
NhuRRCConRecfgn_r8_IEs   *reCfgIEs;
NhCellAndUeCb            *cellandUeCb;
#endif /* ANSI */
{
   NhuDuplexMode     duplexMode; /* FDD or TDD */ 
   S16               retVal;

   TRC2(nhUTLValidateCondIesReCfgHo)

   RLOG0(L_DEBUG,"nhUTLValidateCondIesReCfgHo called.");

   retVal = ROK;

   /* Get duplex mode from ueCb->cellCb.duplexMode indicates whether FDD *
    * or TDD                                                             */
   duplexMode = cellandUeCb->cellCb->crntCellCfgInfo.duplexMode;

   /* Validation of mobility control Info conditional carrier freq IE.     *  
    * If FDD, the ul_CarrierFreq is mandatory. Otherwise it is not present */
   if( ((NHU_MODE_FDD == duplexMode) &&
        (NH_IS_TKN_NOTPRSNT( 
            reCfgIEs->mobilityCntrlInfo.carrierFreq.ul_CarrierFreq))) ||
       ((NHU_MODE_TDD == duplexMode) && 
        (NH_IS_TKN_PRSNT(
            reCfgIEs->mobilityCntrlInfo.carrierFreq.ul_CarrierFreq))) )
   {
      RLOG0(L_ERROR, "Failed NHU_ERRCAUSE_UL_ARFCN_VALUEUTRA");
      RETVALUE(RFAILED);
   }

   /* Dedicated Info NAS List should not be present in case of mobility. *
    * If present, issue cond IE failure to user.                         */
   if(NH_IS_TKN_PRSNT(reCfgIEs->dedicatedInfoNASLst.noComp))
   {
      RLOG0(L_ERROR, "Failed NHU_ERRCAUSE_DEDTED_INF_NAS");
      RETVALUE(RFAILED);
   }

   /* Validate conditional IEs within Radio Resource Dedicated IE */
   retVal = nhUTLValidateCondIesRadResDed(
                                &reCfgIEs->radioResourceConfigDedicated, 
                                cellandUeCb, TRUE); 

   if(ROK != retVal)
   {
      RLOG0(L_ERROR,"conditional IE validation for radio resource dedicated"
         " failed");
      RETVALUE(RFAILED);
   }

   /* Security HO IE is mandatory in case of mobility. *
    * If not present, issue cond IE failure to user.   */
   if(NH_IS_IE_NOTPRSNT(reCfgIEs->securityConfigHO))
   {
      RLOG0(L_ERROR, "Failed NHU_ERRCAUSE_SCRTY_CFG_HO");
      RETVALUE(RFAILED);
   }

   /* Measurement config conditional IE validation */
   retVal = nhUTLValidateCondIesMeasCfg(reCfgIEs->measConfig);
   if(ROK != retVal)
   {
      RLOG0(L_ERROR,"conditional IE validation for measurement config failed");
      RETVALUE(RFAILED);
   }

   RETVALUE(ROK);
} /* end of nhUTLValidateCondIesReCfgHo */

/**
 * @brief Handler for validating conditional IEs in Radio Resource Dedicated IE.
 *
 * @details This function validates conditional IEs for Radio Resource 
 *          Dedicated IE in RRC Reconfiguration message.
 *
 * @param[in] radioResDed   Radio Resource Dedicated IE.
 * @param[in] ueCb          UE control block required to validate RBs.
 * @param[in] isHo          To indicate whether Radio Resource Dedicated IE is
 *                          for HO or not.
 *
 * @return S16
 *  -# ROK
 *  -# RFAILED
*/
#ifdef ANSI
PUBLIC S16 nhUTLValidateCondIesRadResDed
(
NhuRadioResourceConfigDedicated *radioResDed,
NhCellAndUeCb                   *cellandUeCb,
Bool                            isHo
)
#else /* ANSI */
PUBLIC S16 nhUTLValidateCondIesRadResDed(radioResDed, cellandUeCb, isHo)
NhuRadioResourceConfigDedicated *radioResDed;
NhCellAndUeCb                   *cellandUeCb;
Bool                            isHo;
#endif /* ANSI */
{
   S16                retVal;

   TRC2(nhUTLValidateCondIesRadResDed)

   RLOG0(L_DEBUG,"nhUTLValidateCondIesRadResDed called.");

   retVal = ROK;

   /* Validate for radio resource dedicated config in case of HO */
   if(TRUE == isHo)
   {
      /* SRB modify list is mandatory for HO */
      if(NH_IS_TKN_NOTPRSNT(radioResDed->srb_ToAddModLst.noComp))
      {
         RLOG0(L_ERROR,"Failed NHU_ERRCAUSE_SRB_TOADDMDLST");
         RETVALUE(RFAILED);
      }

      /* DRB modify list is mandatory for HO */
      if(NH_IS_TKN_NOTPRSNT(radioResDed->drb_ToAddModLst.noComp))
      {
         RLOG0(L_ERROR,"Failed NHU_ERRCAUSE_DRB_TOADDMDLST");
         RETVALUE(RFAILED);
      }
   }
   else
   {
      /* Validate SRBs and DRBs in radio resource dedicated in case of *
       * connection setup or non HO reconfiguration                   */
      retVal = nhUTLValidateCondIesRbs(radioResDed, cellandUeCb->ueCb);

      if(ROK != retVal)
      {
         RLOG0(L_ERROR,"conditional IE validation for RBs failed");
         RETVALUE(RFAILED);
      }
   }

   /* Validation of Radio Resource Dedicated for Physical Config Dedicated */
   retVal = nhUTLValidateCondIesRadResPhyCfgDed(radioResDed, cellandUeCb);
   if(ROK != retVal)
   {
      RLOG0(L_ERROR,"Validation for Physical config dedicated failed ");
      RETVALUE(RFAILED);
   }

   RETVALUE(ROK);
} /* End of nhUTLValidateCondIesRadResDed */

/**
 * @brief Handler for validation of common conditions of Radio Resource
 *        Dedicated IE.
 *
 * @details This function validation of common conditions of Radio Resource
 *          Dedicated IE. 
 *
 * @param[in] radioResDed   Radio Resource Dedicated IE.
 * @param[in] ueCb          UE control block required to validate RBs.
 *
 * @return S16
 *  -# ROK
 *  -# RFAILED
*/
#ifdef ANSI
PUBLIC S16 nhUTLValidateCondIesRadResPhyCfgDed
(
NhuRadioResourceConfigDedicated *radioResDed,
NhCellAndUeCb                   *cellandUeCb
)
#else /* ANSI */
PUBLIC S16 nhUTLValidateCondIesRadResPhyCfgDed(radioResDed, cellandUeCb)
NhuRadioResourceConfigDedicated *radioResDed;
NhCellAndUeCb                   *cellandUeCb;
#endif /* ANSI */
{
   U32                txMode;             /* Transmission mode */
   NhuDuplexMode      duplexMode;     /* FDD or TDD */
#ifdef LTERRC_REL9
   NhuCQI_ReportConfig_v920 cqi_ReportConfig_v920;
#endif /* LTERRC_REL9 */

   TRC2(nhUTLValidateCondIesRadResPhyCfgDed)

   /* Validation of RadioResourceConfigDedicated.AntennaInfoDedicated.  *
    * AntennaInfoDedicated.codebookSubsetRestriction                    */

   if (NH_IS_TKN_PRSNT(radioResDed->physicalConfigDedicated.antennaInfo.choice))
   {
      txMode = radioResDed->physicalConfigDedicated.\
           antennaInfo.val.explicitValue.transmissionMode.val;

      /* If transmission mode is tm3/tm4/tm5/tm6, codebook subset restriction *
       * IE is mandatory.                                                     */
      if( (NhuAntennaInfoDedicatedtransmissionModetm3Enum == txMode) ||
          (NhuAntennaInfoDedicatedtransmissionModetm4Enum == txMode) ||
          (NhuAntennaInfoDedicatedtransmissionModetm5Enum == txMode) ||
          (NhuAntennaInfoDedicatedtransmissionModetm6Enum == txMode) )
      {
         if(NH_IS_TKN_NOTPRSNT(
            radioResDed->physicalConfigDedicated.antennaInfo.val.explicitValue.\
            codebookSubsetRestrn.choice))
         {
            RLOG0(L_ERROR, "Failed NHU_ERRCAUSE_CODEBK_SUBSET_RSTRCN");
            RETVALUE(RFAILED);
         }
      }

#ifdef LTERRC_REL9
      /* R9 upgrade:NH_NW_122:If transmission mode is not tm8 then the IE
       * codebook subset restriction R9 should not be present
       */
      if(NhuAntennaInfoDedicatedtransmissionModetm8_v920Enum != txMode)
      {
         if(NH_IS_TKN_PRSNT(radioResDed->physicalConfigDedicated.extaddgrp_r9.\
                  antennaInfo_v920.codebookSubsetRestrn_v920.choice))
         {
            RLOG0(L_ERROR, "Failed NHU_ERRCAUSE_CODEBK_SUBSET_RSTRCN_R9");
            RETVALUE(RFAILED);
         }
      }
#endif /* LTERRC_REL9 */
   }

   /* Validation of RadioResourceConfigDedicated.PhysicalConfigDedicated  *
    * .PUCCH-ConfigDedicated.tdd-AckNackFeedbackMode.                     */

   /* Get duplex mode from ueCb->cellCb.duplexMode indicates whether FDD *
    * or TDD                                                             */
   duplexMode = cellandUeCb->cellCb->crntCellCfgInfo.duplexMode;

   /* If TDD, ACK/NACK feedback mode IE is mandatory */
   if( (NHU_MODE_TDD == duplexMode) &&
       (NH_IS_TKN_NOTPRSNT(radioResDed->physicalConfigDedicated.\
                           pucch_ConfigDedicated.tdd_AckNackFeedbackMode)) )
   {
      RLOG0(L_ERROR, "Failed NHU_ERRCAUSE_TDD_ACKNACKFB_MOD");
      RETVALUE(RFAILED);
   }

#ifdef LTERRC_REL9
   /* R9 upgrade:NH_NW_85,NH_NW_122: Validating the CQI-MASK and PMI RI RPT
    *The CQI-MASK is optional present, need OR, if the cqi-ReportPeriodic
    *in the cqi-ReportConfig is set to setup. If the field cqi-ReportPeriodic
    *is present and set to release, the field is not present and the UE shall
    *delete any existing value for this field. Otherwise the field is not 
    *present and the UE takes no action i.e. continues to use the existing
    *value, if previously configured*/
   if (NH_IS_TKN_PRSNT(radioResDed->physicalConfigDedicated.extaddgrp_r9.pres))
   {
      cqi_ReportConfig_v920 = radioResDed->physicalConfigDedicated.extaddgrp_r9.\
                              cqi_ReportConfig_v920;
      if (!(NH_IS_TKN_PRSNT(radioResDed->physicalConfigDedicated.cqi_ReportConfig\
                  .pres) && (radioResDed->physicalConfigDedicated.cqi_ReportConfig\
                     .cqi_ReportPeriodic.choice.val == NHU_CFGTYPE_SETUP)))
      {
         if (NH_IS_TKN_PRSNT(radioResDed->physicalConfigDedicated.\
                  extaddgrp_r9.cqi_ReportConfig_v920.cqi_Mask_r9))
         {
            RLOG0(L_ERROR, "Failed NHU_ERRCAUSE_CQI_MASK");
            RETVALUE(RFAILED);
         }
      }
      /*NH_NW_122:The field pmi_RI_Report_r9 is optional present, need OR, if
       *cqi-ReportPeriodic is included and set to setup,or 
       *cqi-ReportModeAperiodic is included. If the field cqi-ReportPeriodic
       *is present and set to release and cqi-ReportModeAperiodic is absent,
       *the field is not present and the UE*/
      if (!((NH_IS_TKN_PRSNT(radioResDed->physicalConfigDedicated.cqi_ReportConfig\
                     .pres) && (radioResDed->physicalConfigDedicated.cqi_ReportConfig\
                        .cqi_ReportPeriodic.choice.val == NHU_CFGTYPE_SETUP)) || 
               NH_IS_TKN_PRSNT(radioResDed->physicalConfigDedicated.\
                  cqi_ReportConfig.cqi_ReportModeAperiodic)))
      {
         if (NH_IS_TKN_PRSNT(radioResDed->physicalConfigDedicated.\
                  extaddgrp_r9.cqi_ReportConfig_v920.pmi_RI_Report_r9))
         {
            RLOG0(L_ERROR, "Failed NHU_ERRCAUSE_PMI_RI_REPORT");
            RETVALUE(RFAILED);
         }
      }
   }
#endif /* LTERRC_REL9 */

   RETVALUE(ROK);
} /* end of nhUTLValidateCondIesRadResPhyCfgDed */

/**
 * @brief Handler for validating conditional IEs in Measurement config IE.
 *
 * @details This function validates conditional IEs for Measurement
 *          configuration IE in RRC Reconfiguration message.
 *
 * @param[in] measCfg       Measurement configuration IE.
 *
 * @return S16
 *  -# ROK
 *  -# RFAILED
*/
#ifdef ANSI
PUBLIC S16 nhUTLValidateCondIesMeasCfg
(
NhuMeasConfig         measCfg
)
#else /* ANSI */
PUBLIC S16 nhUTLValidateCondIesMeasCfg(measCfg)
NhuMeasConfig         measCfg;
#endif /* ANSI */
{
#ifdef LTERRC_REL9
   NhuReportConfigToAddMod rptCfgToAddMod;
   S16 retVal;
   U8                      idx;
   U16                     count;
#endif /*LTERRC_REL9 */

   TRC2(nhUTLValidateCondIesMeasCfg)

   RLOG0(L_DEBUG,"nhUTLValidateCondIesMeasCfg called.");

   /* If measurement config IE present, validate for conditional IE within */
   if(NH_IS_IE_PRSNT(measCfg))
   {
      if(NH_IS_IE_PRSNT(measCfg.preRegistrationInfoHRPD))
      {
         /* If pre-registration allowed flag is present and set to TRUE, *
          * pre-registration zoneId is mandatory.                        */
         if( (NH_IS_TKN_PRSNT(measCfg.preRegistrationInfoHRPD.\
                              preRegistrationAllowed)) &&
             (TRUE == measCfg.preRegistrationInfoHRPD.preRegistrationAllowed.\
              val))
         {
            if(NH_IS_TKN_NOTPRSNT(measCfg.preRegistrationInfoHRPD.\
                           preRegistrationZoneId))
            {
               RLOG0(L_ERROR, "Failed NHU_ERRCAUSE_PREREG_ZONE_ID");
               RETVALUE(RFAILED);
            }
         } /* end of pre-registration allowed flag pres and val check */
      } /* end of pre-registration Info pres check */
#ifdef LTERRC_REL9
      /*Valdiation of Report Config, si_RqstForHO_r9  */ 
      if (NH_IS_TKN_NOTPRSNT(measCfg.reportConfigToAddModLst.noComp))
      {
         RLOG0(L_ERROR, "Failed NHU_ERRCAUSE_RPRT_CFG_TOADDMODLST");
         RETVALUE(RFAILED);
      }
      count = measCfg.reportConfigToAddModLst.noComp.val;
      for (idx = 0; idx < count; idx ++)
      {
         rptCfgToAddMod = measCfg.reportConfigToAddModLst.member[idx];

         retVal = nhUTLValidateCondIesReportCfg(rptCfgToAddMod.\
               reportConfig);
         if (retVal != ROK)
         {
            RLOG0(L_ERROR, "Failed NHU_ERRCAUSE_RPRT_CFG");
            RETVALUE(RFAILED);
         }
      }
#endif /* LTERRC_REL9 */
   } /* end of measurement config pres check */

   RETVALUE(ROK);
} /* end of nhUTLValidateCondIesMeasCfg */

#ifdef LTERRC_REL9
/**
 * @brief Handler for validating conditional IEs in Report config IE.
 *
 * @details This function validates conditional IEs for Report Config in
 *  Measurement configuration IE in RRC Reconfiguration message.
 *
 * @param[in] reportConfig       Report Configuration IE.
 *
 * @return S16
 *  -# ROK
 *  -# RFAILED
*/
#ifdef ANSI
PUBLIC S16 nhUTLValidateCondIesReportCfg
(
NhuReportConfigToAddModreportConfig  reportCfg
)
#else /* ANSI */
PUBLIC S16 nhUTLValidateCondIesReportCfg(reportCfg)
NhuReportConfigToAddModreportConfig  reportCfg;
#endif /* ANSI */
{
   TRC2(nhUTLValidateCondIesReportCfg)

   RLOG0(L_DEBUG,"nhUTLValidateCondIesMeasCfg called.");

   /* If measurement config IE present, validate for conditional IE within */
   if(NH_IS_TKN_PRSNT(reportCfg.choice))
   {
     /* si_RqstForHO_r9 is optional, need OR, in case 'purpose' is included and
      * set to'reportCGI'otherwise this field is present.
      */
     if(!(NH_IS_TKN_PRSNT(reportCfg.val.reportConfigEUTRA.triggerTyp.val.\
        periodical.purpose) && (reportCfg.val.reportConfigEUTRA.triggerTyp.val.\
        periodical.purpose.val) == 
             NhuReportConfigInterRATtriggerTypperiodicalpurposereportCGIEnum))
        {
           if(NH_IS_TKN_PRSNT(reportCfg.val.reportConfigEUTRA.extaddgrp_r9.\
           si_RqstForHO_r9))
           {
              RLOG0(L_ERROR, "Failed NHU_ERRCAUSE_SI_REQSTFORHO_R9");
              RETVALUE(RFAILED);
           }
        }
   } /* end of measurement config pres check */

   RETVALUE(ROK);
} /* end of nhUTLValidateCondIesReportCfg */
#endif /*LTERRC_REL9*/


/**
 *
 * @brief  Validate Conditional IEs for message RRC Connection Setup:
 * @details
 * This function validates Conditional IEs for message RRC Connection Setup:
 *
 * @param[in] nhuDataRspSdus  Message sdu
 * @param[in] ueCb            UE control block
 *
 * @return S16
 * -# ROK
 * -# RFAILED 
*/
#ifdef ANSI
PUBLIC S16 nhUTLValidateCondIesRRCConnSetup
(
NhuDatRspSdus  *nhuDataRspSdus,
NhCellAndUeCb  *cellAndUeCb
)
#else /* ANSI */
PUBLIC S16 nhUTLValidateCondIesRRCConnSetup(nhuDataRspSdus, cellAndUeCb)
NhuDatRspSdus  *nhuDataRspSdus;
NhCellAndUeCb  *cellAndUeCb;
#endif /* ANSI */
{
   NhuRRCConSetup_r8_IEs   *ies = NULLP;
   NhuSRB_ToAddMod         srb_toAddMod;
   U8                      idx;
   U16                      count;
   S16                     retVal;

   TRC2(nhUTLValidateCondIesRRCConnSetup)

   ies = &(nhuDataRspSdus->sdu.m.dlCcchMsg.ccchMsg.message.val.c1.val.\
         rrcConSetup.criticalExtns.val.c1.val.rrcConSetup_r8);

   if (NH_IS_IE_PRSNT(ies->radioResourceConfigDedicated))
   {
      /*Valdiation of RadioResourceConfigDedicated.SRB-ToAddModList 
       
      TC 14.1 */
      if (NH_IS_TKN_NOTPRSNT(ies->radioResourceConfigDedicated.srb_ToAddModLst.\
                                    noComp)
       )
      {
         RLOG0(L_ERROR, "Failed NHU_ERRCAUSE_SRB_TOADDMDLST");
         RETVALUE(RFAILED);
      }

      /* Validation of RadioResourceConfigDedicated.DRB-ToAddModList
       
      TC 14.2 */
      if (NH_IS_TKN_PRSNT(ies->radioResourceConfigDedicated.drb_ToAddModLst.
                        noComp))
      {
         RLOG0(L_ERROR, "Failed NHU_ERRCAUSE_DRB_TOADDMDLST");
         RETVALUE(RFAILED);
      }

      /* Validation of RadioResourceConfigDedicated.SRB-ToAddModList.RLC-Config
         
      TC 14.3 */
      count = ies->radioResourceConfigDedicated.srb_ToAddModLst.noComp.val;

      for (idx = 0; idx < count; idx ++)
      {
         srb_toAddMod =
            ies->radioResourceConfigDedicated.srb_ToAddModLst.member[idx];
         if (NH_IS_TKN_NOTPRSNT(srb_toAddMod.rlc_Config.choice))
         {
            RLOG0(L_ERROR, "Failed NHU_ERRCAUSE_RLC_CFG");
            RETVALUE(RFAILED);
         }
      }

      /* Validation of RadioResourceConfigDedicated.SRB-ToAddModList.
         logicalChannelConfig 
       
      TC 14.5 */
      for (idx = 0; idx < count; idx ++)
      {
         srb_toAddMod =
            ies->radioResourceConfigDedicated.srb_ToAddModLst.member[idx];
         if (NH_IS_TKN_NOTPRSNT(srb_toAddMod.logicalChannelConfig.choice))
         {
            RLOG0(L_ERROR, "Failed NHU_ERRCAUSE_LGCL_CHANNEL_CFG");
            RETVALUE(RFAILED);
         }
      }

      /* Validate for  common conditions of RedResDed IEs */
      retVal = nhUTLValidateCondIesRadResPhyCfgDed(&(ies->\
                        radioResourceConfigDedicated), cellAndUeCb);
      if(ROK != retVal)
      {
         RLOG0(L_ERROR, "Validation for RadResDedCom faield ");
         RETVALUE(RFAILED);
      }
   }/* if (NH_IS_IE_PRSNT(ies->radioResourceConfigDedicated)) */

   RETVALUE(ROK);
} /* end of nhUTLValidateCondIesRRCConnSetup */


/**
 *
 * @brief  Validate Conditional IEs for message RRC Connection Reconfiguration:
 * @details
 * This function validates Conditional IEs for message RRC Connection 
 * Reconfiguration:
 *
 * @param[in] ies     Recfg IE.
 * @param[in] ueCb    UE control block.
 *
 * @return S16
 * -# ROK
 * -# RFAILED *
*/
#ifdef ANSI
PUBLIC S16 nhUTLValidateCondIesRRCConnReCfg
(
NhuRRCConRecfgn_r8_IEs  *ies,
NhCellAndUeCb           *cellandUeCb
)
#else /* ANSI */
PUBLIC S16 nhUTLValidateCondIesRRCConnReCfg(ies, cellandUeCb)
NhuRRCConRecfgn_r8_IEs  *ies;
NhCellAndUeCb           *cellandUeCb;
#endif /* ANSI */
{
   S16 retVal;

   TRC2(nhUTLValidateCondIesRRCConnReCfg)

   if (NH_IS_IE_PRSNT(ies->radioResourceConfigDedicated))
   {

      retVal = nhUTLValidateCondIesRadResDed(&(ies->\
                     radioResourceConfigDedicated), cellandUeCb, FALSE);
      if (retVal != ROK)
      {
         RETVALUE(RFAILED);
      }
      /*
      Validation of Condition IE
      MeasConfig.PreRegistrationInfoHRPD
      .PreRegistrationZoneIdHRPD during RRC Connection Reconfiguration
      procedure.
      */
      retVal = nhUTLValidateCondIesMeasCfg(ies->measConfig);
      if (retVal != ROK)
      {
         RETVALUE(RFAILED);
      }

   }

   RETVALUE(ROK);
} /* end of nhUTLValidateCondIesRRCConnReCfg */


/**
 *
 * @brief  Validate Conditional IEs for message System Information Block Type 1:
 * @details
 * This function validates Conditional IEs for message System Information Block 
 * Type 1:
 *
 * @param[in] nhuDataReqSdus Message sdu
 * @param[in] ueCb          UE control block.

 * @return S16
 * -# ROK
 * -# RFAILED *
*/
#ifdef ANSI
PUBLIC S16 nhUTLValidateCondIesSibs
(
NhuDatReqSdus *nhuDataReqSdus,
NhCellAndUeCb *cellandUeCb
)
#else /* ANSI */
PUBLIC S16 nhUTLValidateCondIesSibs(nhuDataReqSdus, cellandUeCb)
NhuDatReqSdus *nhuDataReqSdus;
NhCellAndUeCb *cellandUeCb;
#endif /* ANSI */
{
   NhuSysInfoBlockTyp1  *sib1 = NULLP;
   NhuSysInfo           *sysInfo = NULLP;
   NhuSysInfo_r8_IEssib_TypAndInfo  *sib_TypAndInfo = NULLP;
   NhuDuplexMode        duplexMode;
   Bool                 isCSGCell;
   U16                   count;
   U8                   idx;


   TRC2(nhUTLValidateCondIesSibs)

   duplexMode = cellandUeCb->cellCb->crntCellCfgInfo.duplexMode;
   isCSGCell = cellandUeCb->cellCb->crntCellCfgInfo.isCSGCell;

   switch(nhuDataReqSdus->sdu.ueMsg.msg.cmnMsg.cmn.bcchDlSchMsg.\
                                      message.val.c1.choice.val)
   {
      case C1_SYSTEMINFOBLOCKTYP1:
         sib1 = &(nhuDataReqSdus->sdu.ueMsg.msg.cmnMsg.
                    cmn.bcchDlSchMsg.message.val.c1.val.systemInfoBlockTyp1);

         /*
         Validation of Condition IE SystemInformationBlockType1.TDD-Config
         */
         if(duplexMode == NHU_MODE_TDD)
         {
            if(NH_IS_IE_NOTPRSNT(sib1->tdd_Config))
            {
               RLOG0(L_ERROR, "Failed NHU_ERRCAUSE_TDD_CFG");
               RETVALUE(RFAILED);
            }
         }
         break;

      case C1_SYSTEMINFO:
         sysInfo = &(nhuDataReqSdus->sdu.ueMsg.msg.cmnMsg.
                    cmn.bcchDlSchMsg.message.val.c1.val.systemInfo);

         sib_TypAndInfo = &(sysInfo->criticalExtns.val.systemInfo_r8.
                        sib_TypAndInfo);

         /* 
         Validation of Condition IE SystemInformationBlockType4
         .csg-PhysCellIdRange
         */
         count = sib_TypAndInfo->noComp.val;
         if (NH_IS_TKN_PRSNT(sib_TypAndInfo->noComp) && duplexMode == 
                         NHU_MODE_FDD)
         {
            for (idx = 0; idx < count; idx++)
            {
               if(NH_IS_TKN_PRSNT(sib_TypAndInfo->member[idx].choice) &&
                  sib_TypAndInfo->member[idx].choice.val == MEMBER_SIB4 )
               {
                  if (isCSGCell)
                  {
                     if(NH_IS_IE_NOTPRSNT(sib_TypAndInfo->member[idx].val.sib4.
                         csg_PhysCellIdRange))
                     {
                        RLOG0(L_ERROR, "Failed NHU_ERRCAUSE_CSG_PHYSCELLIDRNGE");
                        RETVALUE(RFAILED);
                     }
                  } /* if (isCSGCell)  */
               } /* if(NH_IS_TKN_PRSNT(sib_T  */
            } /* for (idx = 0; idx < cou  */
         } /* if (NH_IS_TKN_PRSNT(sib_TypAndInfo->noComp) && d */

         /* 
         Validation of Condition IE SystemInformationBlockType8
         preRegistrationZoneId */

         if (NH_IS_TKN_PRSNT(sib_TypAndInfo->noComp) && duplexMode == 
                                               NHU_MODE_FDD)
         {
            for (idx = 0; idx < count; idx++)
            {
               if(NH_IS_TKN_PRSNT(sib_TypAndInfo->member[idx].choice) &&
                  sib_TypAndInfo->member[idx].choice.val == MEMBER_SIB8 )
               {
                  if (sib_TypAndInfo->member[idx].val.sib8.parametersHRPD.
                      preRegistrationInfoHRPD.preRegistrationAllowed.val == 
                                              TRUE)
                  {

                     if(NH_IS_TKN_NOTPRSNT(sib_TypAndInfo->member[idx].val.sib8.
                         parametersHRPD.preRegistrationInfoHRPD.
                         preRegistrationZoneId))
                     {
                        RLOG0(L_ERROR, "Failed NHU_ERRCAUSE_PREREG_ZONE_ID");
                        RETVALUE(RFAILED);
                     } /* if(NH_IS_TKN_NOTPRSNT(sib_TypAndInfo.membe */
                  } /* if (sib_TypAndInfo.member[idx].val.sib8.p*/
               }/* if(NH_IS_TKN_PRSNT(sib_TypAnd  */
            } /*  for (idx = 0; inde  */
         } /* if (NH_IS_TKN_PRSNT(sib_TypAndInfo->noComp) &&  */

#ifdef LTERRC_REL9
         /* R9 upgrade:NH_NW_100:ac_BarringConfig1XRTT_r9 is optional present,
          * need OR,if csfb-RegistrationParam1XRTT is present otherwise it is
          * not present
          */
         if (NH_IS_TKN_PRSNT(sib_TypAndInfo->noComp))
         {
            for (idx = 0; idx < count; idx++)
            {
               if(NH_IS_TKN_PRSNT(sib_TypAndInfo->member[idx].choice) &&
                  sib_TypAndInfo->member[idx].choice.val == MEMBER_SIB8 )
               {
                  if (NH_IS_TKN_NOTPRSNT(sib_TypAndInfo->member[idx].val.sib8.\
                           parameters1XRTT.csfb_RegistrationParam1XRTT.pres))
                  {
                     if (NH_IS_TKN_PRSNT(sib_TypAndInfo->member[idx].val.sib8.\
                              extaddgrp_r9.ac_BarringConfig1XRTT_r9.pres))
                     {
                        RLOG0(L_ERROR, "Failed NHU_ERRCAUSE_AC_BARRINGCONFIG1XRTT_R9");
                        RETVALUE(RFAILED);
                            
                     }/* end of ac_BarringConfig1XRTT_r9.pres*/
                  } /*end of csfb_RegistrationParam1XRTT.pres*/
               }/* end of choice and member check*/
            } /* end of loop*/
         } /*end of noComp check*/
#endif /*LTERRC_REL9*/

         /*
         Validation of Condition IE SystemInformationBlockType11
         dataCodingScheme
         */
         if (NH_IS_TKN_PRSNT(sib_TypAndInfo->noComp) && duplexMode == 
                             NHU_MODE_FDD)
         {
            for (idx = 0; idx < count; idx++)
            {
               if(NH_IS_TKN_PRSNT(sib_TypAndInfo->member[idx].choice) &&
                  sib_TypAndInfo->member[idx].choice.val == MEMBER_SIB11 )
               {
                  if (sib_TypAndInfo->member[idx].val.sib11.
                        warningMsgSegmentNumber.val == 0)
                  {

                     if(NH_IS_TKN_NOTPRSNT(
                        sib_TypAndInfo->member[idx].val.sib11.dataCodingScheme))
                     {
                        RLOG0(L_ERROR, "Failed NHU_ERRCAUSE_DATACODINGSCHM");
                        RETVALUE(RFAILED);
                     } /* if(NH_IS_TKN_NOTPRSNT(sib_TypAndInfo.membe */
                  } /* if (sib_TypAndInfo.member[idx].val.sib8.p*/
               }/* if(NH_IS_TKN_PRSNT(sib_TypAnd  */
            } /*  for (idx = 0; inde  */
         } /* if (NH_IS_TKN_PRSNT(sib_TypAndInfo->noComp) &&  */
         break;
     default:
         RLOG0(L_ERROR, "Invalid Message Type");
   }/*end of Switch*/

   RETVALUE(ROK);
}/* end of nhUTLValidateCondIesSibs */

/**
 * @brief Handler for validation of SRB and DRBs IEs of Radio Resource
 *        Dedicated IE.
 *
 * @details This function validation of SRB and DRBs IEs of Radio Resource
 *          Dedicated IE. 
 *
 * @param[in] radioResDed   Radio Resource Dedicated IE.
 * @param[in] ueCb      UE control block required to validate RBs.
 *
 * @return S16
 *  -# ROK
 *  -# RFAILED
*/
#ifdef ANSI
PUBLIC S16 nhUTLValidateCondIesRbs
(
NhuRadioResourceConfigDedicated *radioResDed,
NhActvUeCb                      *ueCb
)
#else /* ANSI */
PUBLIC S16 nhUTLValidateCondIesRbs(radioResDed, ueCb)
NhuRadioResourceConfigDedicated *radioResDed;
NhActvUeCb                      *ueCb;
#endif /* ANSI */
{
   NhuSRB_ToAddMod         srb_toAddMod;
   CmLListCp               *rbCfgInfoList = NULLP;
   U8                      idx;
   U16                      count;

   TRC2(nhUTLValidateCondIesRbs)

   /*
   Validation of Condition IE
   RadioResourceConfigDedicated.SRB-ToAddModList.RLC-Config
   during RRC Connection Reconfiguration procedure
   */
   count = radioResDed->srb_ToAddModLst.noComp.val;

   for (idx = 0; idx < count; idx ++)
   {
      rbCfgInfoList = &(ueCb->crntCfg.rbCfgList.rbCfgInfoList);
      srb_toAddMod =
         radioResDed->srb_ToAddModLst.member[idx];

      if (nhUTLIsRbAvailInLst(rbCfgInfoList,srb_toAddMod.srb_Identity.val,
                              FALSE, ueCb) == FALSE)
      {
         if (NH_IS_TKN_NOTPRSNT(srb_toAddMod.rlc_Config.choice))
         {
            RLOG0(L_ERROR, "Failed NHU_ERRCAUSE_RLC_CFG");
            RETVALUE(RFAILED);
         }
      }
   }

   /*
   Validation of Condition IE
   RadioResourceConfigDedicated.SRB-ToAddModList
   .logicalChannelConfig during RRC Connection Reconfiguration
   procedure
   */

   for (idx = 0; idx < count; idx ++)
   {
      srb_toAddMod =
         radioResDed->srb_ToAddModLst.member[idx];
      if (NH_IS_TKN_NOTPRSNT(srb_toAddMod.logicalChannelConfig.choice))
      {
         RLOG0(L_ERROR, "Failed NHU_ERRCAUSE_LGCL_CHANNEL_CFG");
         RETVALUE(RFAILED);
      }
#ifdef LTERRC_REL9
      /* R9 upgrade:NH_NW_118:conditional IE logicalChannelSR-Mask-r9. This 
       * field is optionally present if ul-SpecificParameters is present,
       * need OR; otherwise it is not present.
       */
      if (NH_IS_TKN_NOTPRSNT(srb_toAddMod.logicalChannelConfig.val.\
               explicitValue.ul_SpecificParams.pres))
      {
         if (NH_IS_TKN_PRSNT(srb_toAddMod.logicalChannelConfig.val.\
                  explicitValue.extaddgrp_r9.logicalChannelSR_Mask_r9))
         {
            RLOG0(L_ERROR, "Failed NHU_ERRCAUSE_LGCL_CHANNELSR_MASK_R9");
            RETVALUE(RFAILED);
         }
      }
#endif /* LTERRC_REL9 */
   }

   /* Validate DRB Specific Params */
   /* Check Logical PDCP Cfg related params */
   if (ROK != nhUTLValidateCondIesRb_DRB_PdcpCfg(radioResDed, ueCb))
   {
      RLOG0(L_ERROR, "Failedd: nhUTLValidateCondIesRb_DRB_PdcpCfg");
      RETVALUE(RFAILED);
   }
   /* Check RLC Cofig related params */
   if (ROK != nhUTLValidateCondIesRb_DRB_RLCCfg(radioResDed, ueCb))
   {
      RLOG0(L_ERROR, "Failed: nhUTLValidateCondIesRb_DRB_RLCCfg");
      RETVALUE(RFAILED);
   }
   /* Check Logical Channel/Id related params */
   if (ROK != nhUTLValidateCondIesRb_DRB_LgclCfg(radioResDed, ueCb))
   {
      RLOG0(L_ERROR, "Failed: nhUTLValidateCondIesRb_DRB_LgclCfg");
      RETVALUE(RFAILED);
   }
   /* Check Logical UL Specific params */
   if (ROK != nhUTLValidateCondIesRb_DRB_Ul_SpParam(radioResDed, ueCb))
   {
      RLOG0(L_ERROR, "Failed: nhUTLValidateCondIesRb_DRB_Ul_SpParam");
      RETVALUE(RFAILED);
   }
   /* Check Logical Other (EPS Bearer Id and Discard timer) related params */
   if (ROK != nhUTLValidateCondIesRb_DRB_Othr(radioResDed, ueCb))
   {
      RLOG0(L_ERROR, "Failed: nhUTLValidateCondIesRb_DRB_Othr");
      RETVALUE(RFAILED);
   }

   RETVALUE(ROK);
}/* End of nhUTLValidateCondIesRbs */

/**
 *
 * @brief  Search item in a given list
 * @details
 * This function searches the item in the list provided
 *
 * @param[in] CmLListCp List
 * @param[in] rbId      rbId to search
 * @param[in] isDRB     is it DRB
 * @param[in] ueCb      UE control block

 * @return S16
 * -# TRUE  if found
 * -# FALSE if not found
*/
#ifdef ANSI
PUBLIC Bool nhUTLIsRbAvailInLst
(
CmLListCp   *rbCfgInfoList,
U32         rbId,
Bool       isDRB,
NhActvUeCb *ueCb
)
#else /* ANSI */
PUBLIC Bool nhUTLIsRbAvailInLst(rbCfgInfoList, rbId, isDRB, ueCb)
CmLListCp   *rbCfgInfoList;
U32         rbId;
Bool       isDRB;
NhActvUeCb *ueCb;
#endif /* ANSI */
{
   CmLList                 *tmpNode =  NULLP;
   Bool                    found = FALSE;

   TRC2(nhUTLIsRbAvailInLst)

   rbCfgInfoList = &(ueCb->crntCfg.rbCfgList.rbCfgInfoList);

   /* Check for the condition that this is RB setup case */
   CM_LLIST_FIRST_NODE(rbCfgInfoList, tmpNode);
   while(tmpNode)
   {
      if (  ((NhuRbCfgInfo *)tmpNode->node)->rbId.rbTyp == isDRB &&
            ((NhuRbCfgInfo *)tmpNode->node)->rbId.rbId == rbId)
      {
         found = TRUE;
         break;
      }
      CM_LLIST_NEXT_NODE(rbCfgInfoList, tmpNode);
   }
   RETVALUE(found);

} /* End of nhUTLIsRbAvailInLst */

/**
 *
 * @brief  Handler to validate conditional IEs of MOBILITY FRM EUTRA 
 * @details
 * This function is to validate conditional IEs of MOBILITY FRM EUTRA
 *
 * @param[in] CmLListCp List
 * @param[in] element elment to search
 *
 * @return S16
 *  -# ROK
 *  -# RFAILED
*/
#ifdef ANSI
PUBLIC S16 nhUTLValidateCondIesMobFrmEutra
(
NhuDatReqSdus   *nhuDataReqSdus,
NhActvUeCb      *ueCb
)
#else /* ANSI */
PUBLIC S16 nhUTLValidateCondIesMobFrmEutra(nhuDataReqSdus, ueCb)
NhuDatReqSdus   *nhuDataReqSdus;
NhActvUeCb      *ueCb;
#endif /* ANSI */
{
   NhuDlDcchMsg                    *dlDcchMsg = NULLP;
   NhuMobilityFromEUTRACmd_r8_IEs  *mobIEs = NULLP;

   TRC2(nhUTLValidateCondIesMobFrmEutra)

   dlDcchMsg = &(nhuDataReqSdus->sdu.ueMsg.msg.dedMsg.dlDcchMsg);
   mobIEs    = &(dlDcchMsg->m.dcchMsg.message.val.c1.val.\
                 mobilityFromEUTRACmd.criticalExtns.val.c1.val.\
                 mobilityFromEUTRACmd_r8);

   /* Validate if purpose is HO */
   if( (PURPOSE_HANDOVER == mobIEs->purpose.choice.val) &&
       (NH_IS_TKN_PRSNT(mobIEs->purpose.choice)) &&
       (NH_IS_IE_PRSNT(mobIEs->purpose.val.handover)) )
   {
      /* Validate if NAS security Param is present if target type is *
       * UTRA or GERAN                                               */
      if( (NhuHovrtargetRAT_TyputraEnum == 
                        mobIEs->purpose.val.handover.targetRAT_Typ.val) ||
          (NhuHovrtargetRAT_TypgeranEnum == 
                        mobIEs->purpose.val.handover.targetRAT_Typ.val) )
      {
         if(NH_IS_TKN_NOTPRSNT(
            mobIEs->purpose.val.handover.nas_SecParamFromEUTRA))
         {
            RLOG0(L_ERROR, "Failed NHU_ERRCAUSE_NAS_SCRTY_PARAM");
            RETVALUE(RFAILED);
         }
      }
      /* Validate if SI or PSI GERAN IE is present if target type is *
       * GERAN                                                       */
      if(NhuHovrtargetRAT_TypgeranEnum == 
                        mobIEs->purpose.val.handover.targetRAT_Typ.val)
      {
         if(NH_IS_TKN_NOTPRSNT(
             mobIEs->purpose.val.handover.systemInfo.val.si.noComp))
         {
            RLOG0(L_ERROR, "Failed NHU_ERRCAUSE_SI_OR_PSI_GERAN");
            RETVALUE(RFAILED);
         }
      }
   }
   RETVALUE(ROK);
} /* End of nhUTLValidateCondIesMobFrmEutra */

/**
 *
 * @brief  Handler to validate conditional IEs of HO Preparation Info Message
 * @details
 * This function is to validate conditional IEs of Inter-node message -
 *     HO Preparation Information.
 *
 * @param[in] nhuDataReqSdus  Data Request structure containing the HO 
 *                           preparation Info msg.
 * @param[in] ueCb           UE control block to obtain duplex mode.
 *
 * @return S16
 *  -# ROK
 *  -# RFAILED
*/
#ifdef ANSI
PUBLIC S16 nhUTLValidateCondIesHoFrmEutraPrepTfr
(
NhuDatReqSdus   *nhuDataReqSdus,
NhActvUeCb      *ueCb
)
#else /* ANSI */
PUBLIC S16 nhUTLValidateCondIesHoFrmEutraPrepTfr(nhuDataReqSdus, ueCb)
NhuDatReqSdus   *nhuDataReqSdus;
NhActvUeCb      *ueCb;
#endif /* ANSI */
{
   NhuDlDcchMsg                    *dlDcchMsg = NULLP;
   NhuHovrFromEUTRAPrepRqst_r8_IEs *hoPrepIEs = NULLP;

   TRC2(nhUTLValidateCondIesHoFrmEutraPrepTfr)

   dlDcchMsg = &(nhuDataReqSdus->sdu.ueMsg.msg.dedMsg.dlDcchMsg);
   hoPrepIEs = &(dlDcchMsg->m.dcchMsg.message.val.c1.val.\
         handoverFromEUTRAPrepRqst.criticalExtns.val.c1.val.\
         handoverFromEUTRAPrepRqst_r8);

   /* Validate if Rand CDMA2000 and Mobility param IEs are present *
    * if CDMA2000 type is 1xRTT                                    */
   if( (NH_IS_IE_PRSNT(*hoPrepIEs)) &&
       (NH_IS_TKN_PRSNT(hoPrepIEs->cdma2000_Typ)) &&
       (NhuCDMA2000_Typtype1XRTTEnum == hoPrepIEs->cdma2000_Typ.val) )
   {
      if(NH_IS_TKN_NOTPRSNT(hoPrepIEs->rand))
      {
            RLOG0(L_ERROR, "Failed NHU_ERRCAUSE_RAND_CDMA2000");
            RETVALUE(RFAILED);
      }
      else if(NH_IS_TKN_NOTPRSNT(hoPrepIEs->mobilityParams))
      {
            RLOG0(L_ERROR, "Failed NHU_ERRCAUSE_MBLTY_PARAM_CDMA2000");
            RETVALUE(RFAILED);
      }
   }
   RETVALUE(ROK);
} /* end of nhUTLValidateCondIesHoFrmEutraPrepTfr */

/**
 *
 * @brief  Handler to validate conditional IEs of HO Preparation Info Message
 * @details
 * This function is to validate conditional IEs of Inter-node message -
 *     HO Preparation Information.
 *
 * @param[in] nhuEncodeReqSdus  Encode Request structure containing the HO 
 *                           preparation Info msg.
 * @param[in] ueCb           UE control block to obtain duplex mode.
 *
 * @return S16
 *  -# ROK
 *  -# RFAILED
*/
#ifdef ANSI
PUBLIC S16 nhUTLValidateCondIesHoPrepInfo
(
NhuEncReqSdus   *nhuEncodeReqSdus,
NhCellAndUeCb   *cellandUeCb
)
#else /* ANSI */
PUBLIC S16 nhUTLValidateCondIesHoPrepInfo(nhuEncodeReqSdus, cellandUeCb)
NhuEncReqSdus   *nhuEncodeReqSdus;
NhCellAndUeCb   *cellandUeCb;
#endif /* ANSI */
{
   NhuHovrPrepInfo_r8_IEs  *hoPrepInfoIEs = NULLP;
   S16                     retVal;

   TRC2(nhUTLValidateCondIesHoPrepInfo)

   hoPrepInfoIEs =
     &(nhuEncodeReqSdus->sdu.u.hovrPrepInfo.criticalExtns.val.c1.val.\
            handoverPrepInfo_r8);

   /* Validate conditional IEs within Radio Resource Dedicated IE */
   retVal = nhUTLValidateCondIesRadResDed(
                   &hoPrepInfoIEs->as_Config.sourceRadioResourceConfig,
                   cellandUeCb, TRUE);
   if (retVal != ROK)
   {
      RLOG0(L_ERROR, "Failed: conditional IEs within Radio Resource Dedicated IE");
      RETVALUE(RFAILED);
   }

   /* Measurement config conditional IE validation */
   retVal = nhUTLValidateCondIesMeasCfg(hoPrepInfoIEs->\
                           as_Config.sourceMeasConfig);
   if (retVal != ROK)
   {
      RLOG0(L_ERROR, "Failed: conditional IEs within Measurement IE");
      RETVALUE(RFAILED);
   }

   RETVALUE(ROK);
} /* end of nhUTLValidateCondIesHoPrepInfo */

/**
 * @brief Handler for validation of DRBs IEs of Radio Resource
 *        Dedicated IE.
 *
 * @details This function validation of DRBs IEs of Radio Resource
 *          Dedicated IE. 
 *
 * @param[in] radioResDed   Radio Resource Dedicated IE.
 * @param[in] ueCb      UE control block required to validate RBs.
 *
 * @return S16
 *  -# ROK
 *  -# RFAILED
*/
#ifdef ANSI
PUBLIC S16 nhUTLValidateCondIesRb_DRB_RLCCfg
(
NhuRadioResourceConfigDedicated *radioResDed,
NhActvUeCb                      *ueCb
)
#else /* ANSI */
PUBLIC S16 nhUTLValidateCondIesRb_DRB_RLCCfg(radioResDed, ueCb)
NhuRadioResourceConfigDedicated *radioResDed;
NhActvUeCb                      *ueCb;
#endif /* ANSI */
{
   NhuDRB_ToAddMod         drb_toAddMod;
   CmLListCp               *rbCfgInfoList = NULLP;
   U8                      idx;
   U16                      count;

   TRC2(nhUTLValidateCondIesRb_DRB_RLCCfg)

   count = radioResDed->drb_ToAddModLst.noComp.val;

   /*
   Validation of Condition IE
   RadioResourceConfigDedicated.DRB-ToAddModList.RLC-Config
   during RRC Connection Reconfiguration procedure
   */
   /*
   Validation of Condition IE
   RadioResourceConfigDedicated.DRB-ToAddModList.PDCP-Config
   .rlc-AM during RRC Connection Reconfiguration procedure.
   */
   /*
   Validation of Condition IE
   RadioResourceConfigDedicated.DRB-ToAddModList.PDCP-Config
   .rlc-UM during RRC Connection Reconfiguration procedure.
   */
   for (idx = 0; idx < count; idx ++)
   {
      rbCfgInfoList = &(ueCb->crntCfg.rbCfgList.rbCfgInfoList);
      drb_toAddMod =
         radioResDed->drb_ToAddModLst.member[idx];

      /* Check for the condition that this is RB setup case and this is
        not first ReCfg after re-est or it is not during handover */
      if(nhUTLIsRbAvailInLst(rbCfgInfoList, drb_toAddMod.drb_Identity.val,
                             TRUE, ueCb) == FALSE)
      {
         if (NH_IS_TKN_NOTPRSNT(drb_toAddMod.rlc_Config.choice))
         {
            RLOG0(L_ERROR, "Failed NHU_ERRCAUSE_RLC_CFG");
            RETVALUE(RFAILED);
         }

         if (drb_toAddMod.rlc_Config.choice.val == RLC_CONFIG_AM)
         {
            if (NH_IS_IE_NOTPRSNT(drb_toAddMod.pdcp_Config.rlc_AM) ||
                NH_IS_IE_PRSNT(drb_toAddMod.pdcp_Config.rlc_UM) )
            {
               RLOG0(L_ERROR, "Failed NHU_ERRCAUSE_RLC_AM");
               RETVALUE(RFAILED);
            }
         }

         if (  NH_IS_TKN_PRSNT(drb_toAddMod.rlc_Config.choice) &&
               drb_toAddMod.rlc_Config.choice.val != RLC_CONFIG_AM)
         {
            if (NH_IS_IE_NOTPRSNT(drb_toAddMod.pdcp_Config.rlc_UM))
            {
               RLOG0(L_ERROR, "Failed NHU_ERRCAUSE_RLC_UM");
               RETVALUE(RFAILED);
            }
         }
      }
   }

   RETVALUE(ROK);
} /* End of nhUTLValidateCondIesRb_DRB_RLCCfg */


/**
 * @brief Handler for validation of DRBs IEs of Radio Resource
 *        Dedicated IE.
 *
 * @details This function validation of DRBs IEs of Radio Resource
 *          Dedicated IE. 
 *
 * @param[in] radioResDed   Radio Resource Dedicated IE.
 * @param[in] ueCb      UE control block required to validate RBs.
 *
 * @return S16
 *  -# ROK
 *  -# RFAILED
*/
#ifdef ANSI
PUBLIC S16 nhUTLValidateCondIesRb_DRB_LgclCfg
(
NhuRadioResourceConfigDedicated *radioResDed,
NhActvUeCb                      *ueCb
)
#else /* ANSI */
PUBLIC S16 nhUTLValidateCondIesRb_DRB_LgclCfg(radioResDed, ueCb)
NhuRadioResourceConfigDedicated *radioResDed;
NhActvUeCb                      *ueCb;
#endif /* ANSI */
{
   NhuDRB_ToAddMod         drb_toAddMod;
   CmLListCp               *rbCfgInfoList = NULLP;
   U8                      idx;
   U16                      count;

   TRC2(nhUTLValidateCondIesRb_DRB_LgclCfg)

   count = radioResDed->drb_ToAddModLst.noComp.val;

   /*
   Validation of Condition IE
   RadioResourceConfigDedicated.DRB-ToAddModList.logicalChannelIdentity   
   during RRC Connection Reconfiguration procedure.
   */

   for (idx = 0; idx < count; idx ++)
   {
      rbCfgInfoList = &(ueCb->crntCfg.rbCfgList.rbCfgInfoList);
      drb_toAddMod =
         radioResDed->drb_ToAddModLst.member[idx];

      /* Check for the condition that this is RB setup case */
      if(nhUTLIsRbAvailInLst(rbCfgInfoList, drb_toAddMod.drb_Identity.val,
                             TRUE, ueCb) == FALSE)
      {
         if (NH_IS_TKN_NOTPRSNT(drb_toAddMod.logicalChannelIdentity))
         {
            RLOG0(L_ERROR, "Failed NHU_ERRCAUSE_LGCL_CHANNEL_ID");
            RETVALUE(RFAILED);
         }
      }

   }

   /*
   Validation of Condition IE
   RadioResourceConfigDedicated.DRB-ToAddModList
   .logicalChannelIdentity during RRC Connection Reconfiguration
   procedure (Not first time)
   */

   for (idx = 0; idx < count; idx ++)
   {
      rbCfgInfoList = &(ueCb->crntCfg.rbCfgList.rbCfgInfoList);
      drb_toAddMod =
         radioResDed->drb_ToAddModLst.member[idx];

      /* Check for the condition that this is not RB setup case and this is
        not first ReCfg after re-est or it is not during handover */
      if(nhUTLIsRbAvailInLst(rbCfgInfoList, drb_toAddMod.drb_Identity.val,
                             TRUE, ueCb) == TRUE)
      {
         if (NH_IS_TKN_PRSNT(drb_toAddMod.logicalChannelIdentity))
         {
            RLOG0(L_ERROR, "Failed NHU_ERRCAUSE_LGCL_CHANNEL_ID");
            RETVALUE(RFAILED);
         }
      }
   }


   /*
   Validation of Condition IE
   RadioResourceConfigDedicated.DRB-ToAddModList.LogicalChannelConfig
   during RRC Connection Reconfiguration procedure
   */
   for (idx = 0; idx < count; idx ++)
   {
      rbCfgInfoList = &(ueCb->crntCfg.rbCfgList.rbCfgInfoList);
      drb_toAddMod =
         radioResDed->drb_ToAddModLst.member[idx];

      /* Check for the condition that this is RB setup case*/
      if(nhUTLIsRbAvailInLst(rbCfgInfoList, drb_toAddMod.drb_Identity.val,
                             TRUE, ueCb) == FALSE)
      {
         if (NH_IS_IE_NOTPRSNT(drb_toAddMod.logicalChannelConfig))
         {
            RLOG0(L_ERROR, "Failed NHU_ERRCAUSE_LGCL_CHANNEL_CFG");
            RETVALUE(RFAILED);
         }
      }
   }
   RETVALUE(ROK);
} /* End of nhUTLValidateCondIesRb_DRB_LgclCfg */

/**
 * @brief Handler for validation of DRBs IEs of Radio Resource
 *        Dedicated IE.
 *
 * @details This function validation of DRBs IEs of Radio Resource
 *          Dedicated IE. 
 *
 * @param[in] radioResDed   Radio Resource Dedicated IE.
 * @param[in] ueCb      UE control block required to validate RBs.
 *
 * @return S16
 *  -# ROK
 *  -# RFAILED
*/
#ifdef ANSI
PUBLIC S16 nhUTLValidateCondIesRb_DRB_PdcpCfg
(
NhuRadioResourceConfigDedicated *radioResDed,
NhActvUeCb                      *ueCb
)
#else /* ANSI */
PUBLIC S16 nhUTLValidateCondIesRb_DRB_PdcpCfg(radioResDed, ueCb)
NhuRadioResourceConfigDedicated *radioResDed;
NhActvUeCb                      *ueCb;
#endif /* ANSI */
{
   NhuDRB_ToAddMod        *drb_toAddMod;
   CmLListCp               *rbCfgInfoList = NULLP;
   U8                      idx;
   U16                      count;

   TRC2(nhUTLValidateCondIesRb_DRB_PdcpCfg)

   count = radioResDed->drb_ToAddModLst.noComp.val;

   if (count == 0)
   {
      /* No DRB present */
      RETVALUE(ROK);
   }

   /*
   Validation of Condition IE
   RadioResourceConfigDedicated.DRB-ToAddModList.PDCP-Config
   during RRC Connection Reconfiguration procedure - Type 1.
   */

   for (idx = 0; idx < count; idx ++)
   {
      rbCfgInfoList = &(ueCb->crntCfg.rbCfgList.rbCfgInfoList);
      drb_toAddMod =
         &(radioResDed->drb_ToAddModLst.member[idx]);

      /* Check for the condition that this is RB setup case and this is
        not first ReCfg after re-est or it is not during handover */
      if(nhUTLIsRbAvailInLst(rbCfgInfoList, drb_toAddMod->drb_Identity.val,
                             TRUE, ueCb) == FALSE)
      {
         if (NH_IS_IE_NOTPRSNT(drb_toAddMod->pdcp_Config))
         {
            RLOG0(L_ERROR, "Failed NHU_ERRCAUSE_PDCP_CFG");
            RETVALUE(RFAILED);
         }
      }
   }

   /*
   Validation of Condition IE
   RadioResourceConfigDedicated.DRB-ToAddModList.PDCP-Config
   during RRC Connection Reconfiguration procedure - Type 2
   */
   for (idx = 0; idx < count; idx ++)
   {
      rbCfgInfoList = &(ueCb->crntCfg.rbCfgList.rbCfgInfoList);
      drb_toAddMod =
         &(radioResDed->drb_ToAddModLst.member[idx]);

      /* Check for the condition that this is not RB setup case and this is
        not first ReCfg after re-est or it is not during handover */
      if(nhUTLIsRbAvailInLst(rbCfgInfoList, drb_toAddMod->drb_Identity.val,
                             TRUE, ueCb) == TRUE)
      {
         if (NH_IS_IE_PRSNT(drb_toAddMod->pdcp_Config))
         {
            RLOG0(L_ERROR, "Failed NHU_ERRCAUSE_PDCP_CFG");
            RETVALUE(RFAILED);
         }
      }
   }

   RETVALUE(ROK);
} /* End of nhUTLValidateCondIesRb_DRB_PdcpCfg */

/**
 * @brief Handler for validation of DRBs IEs of Radio Resource
 *        Dedicated IE.
 *
 * @details This function validation of DRBs IEs of Radio Resource
 *          Dedicated IE. 
 *
 * @param[in] radioResDed   Radio Resource Dedicated IE.
 * @param[in] ueCb      UE control block required to validate RBs.
 *
 * @return S16
 *  -# ROK
 *  -# RFAILED
*/
#ifdef ANSI
PUBLIC S16 nhUTLValidateCondIesRb_DRB_Ul_SpParam
(
NhuRadioResourceConfigDedicated *radioResDed,
NhActvUeCb                      *ueCb
)
#else /* ANSI */
PUBLIC S16 nhUTLValidateCondIesRb_DRB_Ul_SpParam(radioResDed, ueCb)
NhuRadioResourceConfigDedicated *radioResDed;
NhActvUeCb                      *ueCb;
#endif /* ANSI */
{
   NhuDRB_ToAddMod         drb_toAddMod;
   CmLListCp               *rbCfgInfoList = NULLP;
   U8                      idx;
   U16                      count;

   TRC2(nhUTLValidateCondIesRb_DRB_Ul_SpParam)

   count = radioResDed->drb_ToAddModLst.noComp.val;


   if (count == 0)
   {
      /* No DRB present */
      RETVALUE(ROK);
   }

   /*
   Validation of Condition IE
   RadioResourceConfigDedicated.DRB-ToAddModList
   .LogicalChannelConfig.ul-SpecificParameters during RRC Connection
   Reconfiguration procedure - Test Type 1.
   */
   for (idx = 0; idx < count; idx ++)
   {
      rbCfgInfoList = &(ueCb->crntCfg.rbCfgList.rbCfgInfoList);
      drb_toAddMod =
         radioResDed->drb_ToAddModLst.member[idx];

      if (NH_IS_IE_PRSNT(drb_toAddMod.rlc_Config.val.um_Uni_Directional_UL))
      {
         if (NH_IS_IE_NOTPRSNT(drb_toAddMod.logicalChannelConfig.
                     ul_SpecificParams))
         {
            RLOG0(L_ERROR, "Failed NHU_ERRCAUSE_UL_SPCFCPARAMS");
            RETVALUE(RFAILED);
         }
      }
   }

   /*
   Validation of Condition IE
   RadioResourceConfigDedicated.DRB-ToAddModList
   .LogicalChannelConfig.ul-SpecificParameters during RRC Connection
   Reconfiguration procedure - Test Type 2.
   */
   for (idx = 0; idx < count; idx ++)
   {
      rbCfgInfoList = &(ueCb->crntCfg.rbCfgList.rbCfgInfoList);
      drb_toAddMod =
         radioResDed->drb_ToAddModLst.member[idx];

      if (NH_IS_IE_NOTPRSNT(drb_toAddMod.rlc_Config.val.
                     um_Uni_Directional_UL))
      {
         if (NH_IS_IE_PRSNT(drb_toAddMod.logicalChannelConfig.
                     ul_SpecificParams))
         {
            RLOG0(L_ERROR, "Failed NHU_ERRCAUSE_UL_SPCFCPARAMS");
            RETVALUE(RFAILED);
         }
      }
   }
   RETVALUE(ROK);
} /* End of  nhUTLValidateCondIesRb_DRB_Ul_SpParam */

/**
 * @brief Handler for validation of DRBs IEs of Radio Resource
 *        Dedicated IE.
 *
 * @details This function validation of DRBs IEs of Radio Resource
 *          Dedicated IE. 
 *
 * @param[in] radioResDed   Radio Resource Dedicated IE.
 * @param[in] ueCb      UE control block required to validate RBs.
 *
 * @return S16
 *  -# ROK
 *  -# RFAILED
*/
#ifdef ANSI
PUBLIC S16 nhUTLValidateCondIesRb_DRB_Othr
(
NhuRadioResourceConfigDedicated *radioResDed,
NhActvUeCb                      *ueCb
)
#else /* ANSI */
PUBLIC S16 nhUTLValidateCondIesRb_DRB_Othr(radioResDed, ueCb)
NhuRadioResourceConfigDedicated *radioResDed;
NhActvUeCb                      *ueCb;
#endif /* ANSI */
{
   NhuDRB_ToAddMod         drb_toAddMod;
   CmLListCp               *rbCfgInfoList = NULLP;
   U8                      idx;
   U16                      count;

   TRC2(nhUTLValidateCondIesRb_DRB_Othr)

   count = radioResDed->drb_ToAddModLst.noComp.val;

   if (count == 0)
   {
      /* No DRB present */
      RETVALUE(ROK);
   }

   /*
   Validation of Condition IE
   RadioResourceConfigDedicated.DRB-ToAddModList.eps-BearerIdentity
   during RRC Connection Reconfiguration procedure.
   */
   for (idx = 0; idx < count; idx ++)
   {
      rbCfgInfoList = &(ueCb->crntCfg.rbCfgList.rbCfgInfoList);
      drb_toAddMod =
         radioResDed->drb_ToAddModLst.member[idx];

      /* Check for the condition that this is RB setup case */
      if(nhUTLIsRbAvailInLst(rbCfgInfoList, drb_toAddMod.drb_Identity.val,
                             TRUE, ueCb) == FALSE)
      {
         if (NH_IS_TKN_NOTPRSNT(drb_toAddMod.eps_BrIdentity))
         {
            RLOG0(L_ERROR, "Failed NHU_ERRCAUSE_EPS_BEARERIDENTITY");
            RETVALUE(RFAILED);
         }
      }
   }

   /*
   Validation of Condition IE
   RadioResourceConfigDedicated.DRB-ToAddModList.PDCP-Config
   .discardTimer during RRC Connection Reconfiguration procedure.
   */
   for (idx = 0; idx < count; idx ++)
   {
      rbCfgInfoList = &(ueCb->crntCfg.rbCfgList.rbCfgInfoList);
      drb_toAddMod =
         radioResDed->drb_ToAddModLst.member[idx];

      /* Check for the condition that this is RB setup case*/
      if(nhUTLIsRbAvailInLst(rbCfgInfoList, drb_toAddMod.drb_Identity.val,
                             TRUE, ueCb) == FALSE)
      {
         if (NH_IS_TKN_NOTPRSNT(drb_toAddMod.pdcp_Config.discardTimer))
         {
            RLOG0(L_ERROR, "Failed NHU_ERRCAUSE_DISCRD_TMR");
            RETVALUE(RFAILED);
         }
      }
   }
   RETVALUE(ROK);
} /* End of nhUTLValidateCondIesRb_DRB_Othr */

#endif /* #ifdef NH_ASN_COND_VALIDATE */

/**
 * @brief Validates the RRC Lower SAP Configuration parameters
 *
 * @details Validates the RRC Lower SAP Configuration parameters
 *
 * @param[in] elmntId   Element Id
 * @param[in] suId      UE Id
 *
 * @return S16
 *  -# LCM_REASON_NOT_APPL - Validation succeeded
 *  -# LCM_REASON_<REASON> - The reason for which validation failed
*/
#ifdef ANSI
PUBLIC S16 nhUTLValidateLSapSuId
(
U16       elmntId,
SuId      suId    /* Lower Sap SuId */
)
#else /* ANSI */
PUBLIC S16 nhUTLValidateLSapSuId(elmntId, suId)
U16       elmntId;
SuId      suId;   /* Lower Sap SuId */
#endif /* ANSI */
{
   TRC2(nhUTLValidateLSapSuId);

   /* Check if is within the max value */
   switch(elmntId)
   {
      case STNHKWUSAP:
         if ((suId >= (SuId)nhCb.genCfg.nmbKwuSaps) || (suId < 0))
         {
            RETVALUE(LCM_REASON_EXCEED_CONF_VAL);
         }
         break;

      case STNHCKWSAP:
         if ((suId >= (SuId)nhCb.genCfg.nmbCkwSaps) || (suId < 0))
         {
            RETVALUE(LCM_REASON_EXCEED_CONF_VAL);
         }
         break;

      case STNHCRGSAP:
         if ((suId >= (SuId)nhCb.genCfg.nmbCrgSaps) || (suId < 0))
         {
            RETVALUE(LCM_REASON_EXCEED_CONF_VAL);
         }
         break;

      case STNHCPJSAP:
         if ((suId >= (SuId)nhCb.genCfg.nmbCpjSaps) || (suId < 0))
         {
            RETVALUE(LCM_REASON_EXCEED_CONF_VAL);
         }
         break;

      case STNHPJUSAP:
         if ((suId >= (SuId)nhCb.genCfg.nmbPjuSaps) || (suId < 0))
         {
            RETVALUE(LCM_REASON_EXCEED_CONF_VAL);
         }
         break;

#ifdef RRC_PHY_CFG_SUPP
      case STNHCTFSAP:
         if ((suId >= (SuId)nhCb.genCfg.nmbCtfSaps) || (suId < 0))
         {
            RETVALUE(LCM_REASON_EXCEED_CONF_VAL);
         }
         break;
#endif /* RRC_PHY_CFG_SUPP */

      default:
         RLOG0(L_ERROR, "Invalid sapType ");
         RETVALUE(LCM_REASON_INVALID_PAR_VAL);
   }

   RETVALUE(LCM_REASON_NOT_APPL);
} /* nhUTLValidateLSapSuId */

/**
 * @brief Function for validation of DLCCCH Data Responce
 *
 * @details This function validates DLCCCH Data Responce
 *
 * @param[in] datRspSdus   Data response structure
 * @param[in] cellAndUeCb  Cell and UE Context
 * @param[out] errInfo  Structure to hold error information
 * @param[out] crnti  CRNTI
 * @param[in] msgTransId  Message transaction ID
 * @param[in] event  Event stucture
 * @param[out] mesgType  Message type
 *
 * @return S16
 *  -# ROK
 *  -# RFAILED
*/
#ifdef ANSI
PUBLIC S16 nhUTLValidateDatRspDlCChMsg
(
NhuDatRspSdus  *datRspSdus,
NhCellAndUeCb  *cellAndUeCb,
NhuErrorInfo   *errInfo,
U16            *crnti,
U8             *msgTransId,
U8             *event,
MsgType        *mesgType
)
#else /* ANSI */
PUBLIC S16 nhUTLValidateDatRspDlCChMsg(datRspSdus, cellAndUeCb, errInfo,
                  crnti, msgTransId, event, mesgType)
NhuDatRspSdus  *datRspSdus;
NhCellAndUeCb  *cellAndUeCb;
NhuErrorInfo   *errInfo;
U16            *crnti;
U8             *msgTransId;
U8             *event;
MsgType        *mesgType;
#endif /* ANSI */
{
   NhuDL_CCCH_MsgTyp *dlCcchMsg = NULLP;
   NhActvUeCb  *ueCb;
   NhCellCb    *cellCb = NULLP;

   TRC2(nhUTLValidateDatRspDlCChMsg);
   cellCb= cellAndUeCb->cellCb;
   *mesgType = NH_IGNORE;

   if(datRspSdus->sdu.msgCategory == NHU_MSG_DLCCCH)
   {
      dlCcchMsg = &(datRspSdus->sdu.m.dlCcchMsg.ccchMsg.message);
      if(dlCcchMsg && dlCcchMsg->choice.val == DL_CCCH_MSGTYP_C1)
      {
         nhUTLGetMsgTypeAndEvntFrmSdu(dlCcchMsg->val.c1.choice.val, 
                                      NH_SDU_DL_CCCH, mesgType, event);
         switch(dlCcchMsg->val.c1.choice.val)
         {
            case C1_RRCCONSETUP:
            {
               if(NH_ZERO_VAL == datRspSdus->sdu.m.dlCcchMsg.tmpCrnti)
               {
                  RLOG0(L_ERROR, "Temporary Crnti is not present.");
                  NH_FILL_NHU_ERROR(*errInfo, NHU_ERRTYPE_INV_CFG,
                                    NHU_ERRCAUSE_CRNTI_NOT_CFG);
                  RETVALUE(RFAILED);
               }
       
               if(datRspSdus->hdr.ueId == NH_ZERO_VAL) 
               {
                  if((TRUE == datRspSdus->sdu.isUeCfgPres) &&
                     !(datRspSdus->sdu.ueCfg.ueCfgBitMsk & 
                                                      NHU_EXP_CFG_UE_CFG))
                  {
                     RLOG0(L_ERROR, "UE ID config not present in the UE CFG.");
                     NH_FILL_NHU_ERROR(*errInfo, NHU_ERRTYPE_INV_CFG,
                                                 NHU_ERRCAUSE_CRNTI_NOT_CFG);
                     RETVALUE(RFAILED); 
                  }
                  else if((TRUE == datRspSdus->sdu.isUeCfgPres) &&
                        (datRspSdus->sdu.ueCfg.ueCfgBitMsk &
                                            NHU_EXP_CFG_UE_CFG) &&
                        (datRspSdus->sdu.ueCfg.ueInfo.newCrnti !=
                            datRspSdus->sdu.m.dlCcchMsg.tmpCrnti))
                  {
                     RLOG0(L_ERROR, "UE ID mismatch in config and message SDU.");
                     NH_FILL_NHU_ERROR(*errInfo, NHU_ERRTYPE_INV_CFG,
                           NHU_ERRCAUSE_CRNTI_MISMATCH);
                     RETVALUE(RFAILED);
                  }
                  *crnti = datRspSdus->sdu.m.dlCcchMsg.tmpCrnti;
               }
               else
               {
                  ueCb = nhDBMGetUeCb(cellCb, datRspSdus->hdr.ueId);
                  if(NULLP == ueCb)
                  {
                      /* Unknown Message Type, error */
                     RLOG_ARG0(L_ERROR, DBG_UEID,datRspSdus->hdr.ueId,
                        "Invalid UeId in Header");
                     NH_FILL_NHU_ERROR(*errInfo, NHU_ERRTYPE_INV_NHU_HDR,
                                                 NHU_ERRCAUSE_INV_HDR_UEID);
                     RETVALUE(RFAILED);  
                  }
                  else
                  {
                     cellAndUeCb->ueCb = ueCb;
                  }
       
       
                  if((TRUE == datRspSdus->sdu.isUeCfgPres) &&
                     (datRspSdus->sdu.ueCfg.ueCfgBitMsk &
                                                          NHU_EXP_CFG_UE_CFG) &&
                     (datRspSdus->sdu.ueCfg.ueInfo.newCrnti !=
                                         datRspSdus->sdu.m.dlCcchMsg.tmpCrnti))
                  {
                     RLOG_ARG0(L_DEBUG, DBG_CRNTI, datRspSdus->sdu.m.dlCcchMsg.tmpCrnti,
                        "UE ID mismatch in config and message SDU");
                     NH_FILL_NHU_ERROR(*errInfo, NHU_ERRTYPE_INV_CFG,
                           NHU_ERRCAUSE_CRNTI_MISMATCH);
                     RETVALUE(RFAILED);
                  }
       
                  *crnti = datRspSdus->hdr.ueId;
               }
       
               nhUTLGetRrcTransId((void *)&datRspSdus->sdu.m.dlCcchMsg.ccchMsg,
                                      *mesgType,NH_SDU_DL_CCCH, msgTransId);
               
               break;
            }
            case C1_RRCCONREESTBREJ:
            case C1_RRCCONREJ:
               break;
            case C1_RRCCONREESTB:
            {
               ueCb = nhDBMGetUeCb(cellCb, datRspSdus->hdr.ueId);
               if(NULLP == ueCb)
               {
                      /* Unknown Message Type, error */
                  RLOG_ARG0(L_ERROR, DBG_UEID,datRspSdus->hdr.ueId,
                     "Invalid UeId in Header");
                  NH_FILL_NHU_ERROR(*errInfo, NHU_ERRTYPE_INV_NHU_HDR,
                                              NHU_ERRCAUSE_INV_HDR_UEID);
                  RETVALUE(RFAILED);  
               }
               else
               {
                  cellAndUeCb->ueCb = ueCb;
               }
               /*If RRC user has not provided Ue config info or security 
                 config info throw an error.*/
               if((TRUE == datRspSdus->sdu.isUeCfgPres) && (
                 ((datRspSdus->sdu.ueCfg.ueCfgBitMsk & NHU_EXP_CFG_UE_CFG) 
                                                                     == 0)))
               {
                  RLOG_ARG0(L_ERROR, DBG_UEID,datRspSdus->hdr.ueId,
                     "Invalid Configuration Info");
                  NH_FILL_NHU_ERROR(*errInfo, NHU_ERRTYPE_INV_CFG,
                                              NHU_ERRCAUSE_INVALID_CFG_INFO);
                  RETVALUE(RFAILED);
       
               }
               else if((TRUE == datRspSdus->sdu.isUeCfgPres) &&
                     (datRspSdus->sdu.ueCfg.ueCfgBitMsk & NHU_EXP_CFG_UE_CFG) &&
                      (datRspSdus->sdu.ueCfg.ueInfo.newCrnti !=
                                          datRspSdus->sdu.m.dlCcchMsg.tmpCrnti))
               {
                  RLOG_ARG0(L_DEBUG, DBG_CRNTI, datRspSdus->sdu.m.dlCcchMsg.tmpCrnti,
                     "UE ID mismatch in config and message SDU");
                  NH_FILL_NHU_ERROR(*errInfo, NHU_ERRTYPE_INV_CFG,
                                    NHU_ERRCAUSE_CRNTI_MISMATCH);
                  RETVALUE(RFAILED);
               }
       
               *crnti = datRspSdus->hdr.ueId;
               break;
            }
            default:
            {
               /* It must not come here. Return error */
               NH_FILL_NHU_ERROR(*errInfo, NHU_ERRTYPE_INV_UE_MSG_PARAM,
                                           NHU_ERRCAUSE_INV_MSG_CATEGORY);
               RETVALUE(RFAILED);  
             }
         }
      }
   }/*datRspSdus->sdu.msgCategory == NHU_MSG_DLCCCH*/
   RETVALUE(ROK);
} /* End of nhUTLValidateDatRspDlCChMsg */

/**
 * @brief Function for validation of DLCCCH Data Responce
 *
 * @details This function validates DLCCCH Data Responce
 *
 * @param[in] datRspSdus   Data response structure
 * @param[in] cellAndUeCb  Cell and UE Context
 * @param[out] errInfo  Structure to hold error information
 * @param[out] crnti  CRNTI
 * @param[in] msgTransId  Message transaction ID
 * @param[in] event  Event stucture
 * @param[out] msgType  Message type
 *
 * @return S16
 *  -# ROK
 *  -# RFAILED
*/
#ifdef ANSI
PUBLIC S16 nhUTLValidateUeCfgRbAdd
(
NhuUeCfgInfo   *ueCfg,
NhActvUeCb     *ueCb,
NhuErrorInfo   *errInfo
)
#else /* ANSI */
PUBLIC S16 nhUTLValidateUeCfgRbAdd(ueCfg, ueCb, errInfo)
NhuUeCfgInfo   *ueCfg;
NhActvUeCb     *ueCb;
NhuErrorInfo   *errInfo;
#endif /* ANSI */
{
   NhuRbCfgList    *rbToAddList;
   NhuRbCfgInfo    *rbCfgInfo;
   S16              i;
   /* nh001.301:  Fix for 124582 */
   U32              drbMsk = 0, dlcMsk = 0, ddirMsk[2] = {0, 0};
   U32              srbMsk = 0, slcMsk = 0, sdirMsk[2] = {0, 0};
   S16              ret = ROK;
   CmLList         *tmpNode;

   /* nh004.201 - Correction to TRC functionality */
   TRC2(nhUTLValidateUeCfgRbAdd)

   /* nh001.301:  Fix for 124582 */
   NH_SET_ZERO(ddirMsk, 2 * sizeof(U32));
   NH_SET_ZERO(sdirMsk, 2 * sizeof(U32));

   if ((ueCfg->rbCfgAllList.rbCfgTypBitMsk & NHU_CFG_RB_ADD)
         && (ueCb->crntCfg.bitMskPres & NH_CRNT_CFG_RB))
   {
      /* Validate that the RBs requested to be added are 
       * not part of crnt config 
       */
      rbToAddList = &(ueCfg->rbCfgAllList.rbToAddCfgList);

      for(i=0; i<(rbToAddList->numOfRbCfgInfo); i++)
      {
         rbCfgInfo = &rbToAddList->rbCfgInfoList[i];
         /* Fix for ccpu00124582 */
         if(rbCfgInfo->rbId.rbTyp == CM_LTE_SRB)
         {
             NH_VALIDATE_SRBID_TO_LCID_MAP(rbCfgInfo, 
                                          srbMsk, slcMsk, sdirMsk, ret );
         }
         else
         {
             NH_VALIDATE_DRBID_TO_LCID_MAP(rbCfgInfo, 
                                          drbMsk, dlcMsk, ddirMsk, ret );
         }
         if(RFAILED == ret)
         {
            RLOG0(L_ERROR,"Logical channel Configuration with Invalid Ul/Dl "
               "BitMask ");
            /* Fill the error info and return */
            NH_FILL_NHU_ERROR(*errInfo, NHU_ERRTYPE_UE_CFG,
                               NHU_ERRCAUSE_INV_RB_CFG_INFO);
            RETVALUE(RFAILED);
         }
         /* Validate that if PDCP is to be configured then all RLC 
          * info also exists */
         if (rbCfgInfo->rbCfgBitMsk & NHU_RB_CFG_PDCP_INFO)
         {
            if (!((rbCfgInfo->rbCfgBitMsk & NHU_RB_CFG_UL) ||
                 (rbCfgInfo->rbCfgBitMsk & NHU_RB_CFG_DL)))
            {
               RLOG0(L_ERROR,"RB(PDCP) Configuration with Invalid Ul/Dl(RLC)"
                  "BitMask ");
               /* Fill the error info and return */
               NH_FILL_NHU_ERROR(*errInfo, NHU_ERRTYPE_UE_CFG,
                               NHU_ERRCAUSE_INV_RB_CFG_INFO);
               RETVALUE(RFAILED);
            }
         }

         /* Validate that if RB type is SRB then RB Id is 1 or 2 */
         NH_VALIDATE_RB_ID(rbCfgInfo->rbId.rbId, rbCfgInfo->rbId.rbTyp, ret);
         if(RFAILED == ret)
         {
            RLOG0(L_ERROR,"RB type and RB Id mismatch ");
            /* Fill the error info and return */
            NH_FILL_NHU_ERROR(*errInfo, NHU_ERRTYPE_UE_CFG,
                               NHU_ERRCAUSE_INV_RB_CFG_INFO);
            RETVALUE(RFAILED);
         }

         CM_LLIST_FIRST_NODE(&(ueCb->crntCfg.rbCfgList.rbCfgInfoList), 
                                    tmpNode);
         while (NULLP != tmpNode)
         {
            if ((NULLP != tmpNode->node) &&
                 (((NhuRbCfgInfo*)(tmpNode->node))->rbId.rbId 
                    == rbToAddList->rbCfgInfoList[i].rbId.rbId) &&
                 (((NhuRbCfgInfo*)(tmpNode->node))->rbId.rbTyp
                    == rbToAddList->rbCfgInfoList[i].rbId.rbTyp))
            {
               /* RB already in crnt config */
               break;
            }
            CM_LLIST_NEXT_NODE(&ueCb->crntCfg.rbCfgList.rbCfgInfoList,
                                  tmpNode);
         } /* while */

         if (NULLP != tmpNode)
         {
            /* Fill the error info and return */
            NH_FILL_NHU_ERROR(*errInfo, NHU_ERRTYPE_UE_CFG,
                               NHU_ERRCAUSE_INV_RB_CFG_INFO);
            RETVALUE(RFAILED);
         }
      } /* for (..) */
   } /* if (NHU_CFG_RB_ADD) */
   RETVALUE(ROK);
} /* End of nhUTLValidateUeCfgRbAdd */
/** @} */

/**********************************************************************
  
         End of file:     ni_utl.c@@/main/3 - Fri Jul  1 01:13:23 2011
   
**********************************************************************/   
 
/**********************************************************************
        Revision History:

**********************************************************************/
/********************************************************************90**
 
     ver       pat    init                  description
------------ -------- ---- ----------------------------------------------
/main/1      ---      rer              1. LTE-RRC Initial Release.
/main/2      ---      ch               1. RRC Release 2.1.
             ---      chebli           1. nh001.201 validation removed.
          nh004.201 vkulkarni   1. Correction to TRC functionality.
                                1. Changed debug print type to INFO. 
/main/3      ---      sbalakrishna     1. Updated for Release of 3.1.
/main/4   nh001.301   njha       1. Initialising the param.
*********************************************************************91*/
