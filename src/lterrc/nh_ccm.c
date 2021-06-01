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
  
     Type:     C include file
  
     Desc:     RRC Connection Control Module.

     File:     nh_ccm.c

     Sid:      ni_ccm.c@@/main/3 - Fri Jul  1 01:13:13 2011

     Prg:      gm

**********************************************************************/

/** @defgroup ccm_group Connection Control Module
 *  @{
 */



/* header include files (.h) */
static const char* RLOG_MODULE_NAME="RRC";
static int RLOG_MODULE_ID=512;
static int RLOG_FILE_ID=83;
#include "envopt.h"        /* Environment options */
#include "envdep.h"        /* Environment dependent */
#include "envind.h"        /* Environment independent */

#include "gen.h"           /* General */
#include "ssi.h"           /* System services */

#include "cm_tkns.h"       /* Common Token defines */
#include "cm_llist.h"      /* Common Link List defines */
#include "cm_hash.h"       /* Common Hashlist defines */
#include "cm_mblk.h"       /* common memory allocation library defines */
#include "cm_pasn.h"       /* Common Pasn defines */

#include "cm_lte.h"        /* Common LTE defines */
#include "kwu.h"           /* KWU Interface defines */
#include "ckw.h"           /* CKW Interface defines */
#include "crg.h"           /* CRG Interface defines */
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
#include "cm_llist.x"      /* Common Link list definitions */
#include "cm_hash.x"       /* Common Hashlist definitions */
#include "cm_lib.x"        /* Common Library definitions */
#include "cm_mblk.x"       /* common memory allocation */
#include "cm_pasn.x"       /* Common Hashlist definitions */

#include "cm_lte.x"        /* Common LTE includes */
#include "kwu.x"           /* KWU Interface includes */
#include "ckw.x"           /* CKW Interface includes */
#include "crg.x"           /* CRG Interface includes */
#include "pju.x"           /* DMU Interface includes */

#include "nhu.x"           /* NHU Interface includes */
#include "nhu_asn.x"       /* NHU ASN includes */
#include "lnh.x"           /* LNH Interface includes */
#include "nh.x"            /* NH defines */

/* local defines */

/** @def NH_CCM_DEL_UECB_TRANSCB
  *  This macro deletes the transaction context and the UE context based 
  *  on the flag.
  */
#define NH_CCM_DEL_UECB_TRANSCB(_isDelTransCb, _transCb, \
                              _isDeleteUe, _cellCb, _ueCb) \
{ \
   if(TRUE == _isDelTransCb) \
   { \
      nhDBMDelTransCb(*_ueCb, _transCb); \
   } \
   if(TRUE == _isDeleteUe) \
   { \
      nhDBMDeleteUeCb(_cellCb, _ueCb); \
   } \
}

/* local typedefs */

/* local externs */

/* forward references */





/**
 * @brief Handler for RRC Connection Setup message.
 * 
 * @details 
 * 
 *
 * This function creates the UE context and configures the lower layers and also 
 * stores the encoded message, if it receives configuration information.
 * If configuration is not received, it sends RRC Connection Setup 
 * message to RLC and updates the message statistics.
 * 
 * 
 * @param[in] datReqRspSdu   RRC Downlink messages
 * @param[in] cellAndUeCb    Cell and UE context
 * @param[in] msgTransId     Transaction identifier 
 * @param[out] errInfo       Error information, in case of error 
 * @return S16
 *  -# ROK
 *  -# RFAILED
*/
#ifdef ANSI
PUBLIC S16 nhCCMConSetupHdlr 
(
NhDatReqRspSdu    *datReqRspSdu, 
NhCellAndUeCb     *cellAndUeCb,
U8                msgTransId,
NhuErrorInfo      *errInfo
)
#else /* ANSI */
PUBLIC S16 nhCCMConSetupHdlr(datReqRspSdu, cellAndUeCb, msgTransId, errInfo)
NhDatReqRspSdu    *datReqRspSdu;
NhCellAndUeCb     *cellAndUeCb;
U8                msgTransId;
NhuErrorInfo      *errInfo;
#endif /* ANSI */
{
   S16               retVal;
   TransCb           *transCb;
   U32               msgTransKey;
   MsgType           msgType;
   NhDatReqParam     datReqParam;
   NhMsgPduAndType   msgPduAndType;
   NhuDatRspSdus     *datRspSdu;
   NhActvUeCb        *ueCb = NULLP;
   NhCellCb          *cellCb;
   Bool              isUeCbCreated = FALSE;
   Bool              isTransCbCreated = FALSE;

   TRC2(nhCCMConSetupHdlr)

   RLOG0(L_DEBUG, "nhCCMConSetupHdlr called");

   msgType = NH_MSGTYPE_CONNSETUP;
   NH_SET_ZERO(&(datReqParam), sizeof(NhDatReqParam));

   datRspSdu = datReqRspSdu->sdu.datRspSdu;
   cellCb = cellAndUeCb->cellCb;

   /* Get the message transaction key */
   nhUTLGetRrcTransId((Void *) & (datRspSdu->sdu.m.dlCcchMsg), C1_RRCCONSETUP, 
                    NH_SDU_DL_CCCH, &msgTransId);
   NH_GET_TRANSCB_KEY(msgTransKey, msgType, msgTransId);

   NH_FILL_MSG_PDU_AND_TYPE(msgPduAndType, &datRspSdu->sdu.m.dlCcchMsg.ccchMsg, 
         NHU_MSG_DLCCCH);

   /* Fill RLC parameters required by PIM */
   NH_FORM_NH_ID_FRM_DATRSP(datReqParam.nhId, datRspSdu);
   datReqParam.isPdcpDatReq = FALSE;
   datReqParam.u.rlcParam.lcType = CM_LTE_LCH_CCCH;
   datReqParam.u.rlcParam.u.ccchInfo.rnti = 
                  datRspSdu->sdu.m.dlCcchMsg.tmpCrnti;

   /* If both configuration and message is present, send configuration
    * first. Store the message in the buffer for transmision after 
    * successful configuration */
   if(TRUE == datRspSdu->sdu.isUeCfgPres)
   {
      /* Fetch UE context if it exists */
      if(datRspSdu->hdr.ueId > 0)
      {
         ueCb = cellAndUeCb->ueCb;
      }
      else
      {
         /* Create UE context */
         retVal = nhDBMCreateUeCb(cellCb, datRspSdu->sdu.m.dlCcchMsg.tmpCrnti, 
                                    &ueCb); 
         if (ROK != retVal)
         {
            RLOG0(L_ERROR, "Cannot create UE control block");
            NH_FILL_NHU_ERROR(*errInfo, NHU_ERRTYPE_UE_CFG, 
                              NHU_ERRCAUSE_UE_CB_CREATE_FAILED);
            RETVALUE(retVal);
         }
         cellAndUeCb->ueCb = ueCb;
         /* This is used for deleting UE context. */
         isUeCbCreated = TRUE;
      }

      /* Create transaction block for the configuration */
      retVal = nhDBMAddTransCb(ueCb, &transCb, msgTransKey);

      if (ROK != retVal)
      {
         RLOG0(L_ERROR, "Unable to create transaction control block");
         NH_CCM_DEL_UECB_TRANSCB(isTransCbCreated, &transCb, isUeCbCreated,
               cellCb, &ueCb);
         RETVALUE(retVal);
      }

      transCb->cause = msgType;
      transCb->nxTransId = datRspSdu->hdr.transId;

      /* This is used for deleting transaction context */
      isTransCbCreated = TRUE;

      /* For CGM, UE ID change logic this is done */
      if(datRspSdu->hdr.ueId > 0)
      {
         if(datRspSdu->hdr.ueId != datRspSdu->sdu.m.dlCcchMsg.tmpCrnti)
         {
            /* UE ID modification required at lower layers */
            transCb->ordrdCfg.actionMask |= NH_ORD_CFG_UEID;
            transCb->ordrdCfg.ueInfo.crntiCfgType = NH_UE_INFO_MOD_CRNTI;
            transCb->ordrdCfg.ueInfo.u.modCrnti =
               datRspSdu->sdu.m.dlCcchMsg.tmpCrnti;
            /* Update General Statistics because the RRC Conection Setup message
             * sent with old CRNTI has not received any response from UE */
            nhUTLUpdGenSts(NH_STS_CAUSE_CON_FAIL);
         }
      }
      else
      {
         /* UE ID configuration required at lower layers */
         transCb->ordrdCfg.actionMask |= NH_ORD_CFG_UEID;
         transCb->ordrdCfg.ueInfo.crntiCfgType = NH_UE_INFO_NEW_CRNTI;
         transCb->ordrdCfg.ueInfo.u.newCrnti =
            datRspSdu->sdu.m.dlCcchMsg.tmpCrnti;
      }

      /* Encodes the message and stores it in transCb */
      retVal = nhUTLEncAndStoreMsg(&msgPduAndType, &datReqParam, 
            transCb, errInfo);
      if (ROK != retVal)
      {
         RLOG0(L_ERROR, "nhUTLEncAndStoreMsg failed");
         NH_CCM_DEL_UECB_TRANSCB(isTransCbCreated, &transCb, isUeCbCreated,
               cellCb, &ueCb);
         RETVALUE(retVal);
      }

      /* Use CGM to configure lower layers */
      retVal = nhCGMLowerLyrCfgReq(cellAndUeCb, &datRspSdu->sdu.ueCfg, 
            transCb, TRUE);
      if (ROK != retVal)
      {
         RLOG0(L_ERROR, "Cannot perform lower layer config");
         /* Delete UE and Trans CB only if Lower Layer Config is not sent */
#ifndef RRC_PHY_CFG_SUPP
         if((!transCb->numOfMacCfms) && (!transCb->numOfRlcCfms) &&
               (!transCb->numOfPdcpCfms))
#else /* RRC_PHY_CFG_SUPP */
         if((!transCb->numOfPhyCfms) && (!transCb->numOfMacCfms) && 
            (!transCb->numOfRlcCfms) && (!transCb->numOfPdcpCfms))
#endif /* RRC_PHY_CFG_SUPP */
         {
            NH_CCM_DEL_UECB_TRANSCB(isTransCbCreated, &transCb, isUeCbCreated,
                                    cellCb, &ueCb);
         }

         NH_FILL_NHU_ERROR(*errInfo, NHU_ERRTYPE_UE_CFG, 
                              NHU_ERRCAUSE_LOW_LYR_CFG_FAIL);

         RETVALUE(retVal);
      }
   } /* end if(datRspSdu->sdu.isUeCfgPres) */ 
   else 
   {
         /* Encodes and sends message to UE */
         retVal = nhUTLEncAndSnd(cellAndUeCb, 0, &msgPduAndType, 
               &datReqParam, errInfo);
         if(ROK != retVal)
         {
            RLOG0(L_ERROR, "nhUTLEncAndSnd Failed.");
            NH_CCM_DEL_UECB_TRANSCB(isTransCbCreated, &transCb, isUeCbCreated,
                  cellCb, &ueCb);
            RETVALUE(retVal);
         }

#ifndef NH_NO_STS
         /* Update the Statistics */
         nhUTLUpdCellPduSts(cellAndUeCb->cellCb, msgType,
                   NH_STS_CAUSE_SUCCESS);
#endif /* NH_NO_STS */
   } /* end else if(datRspSdu->sdu.datRspBitMsk & NHU_DAT_UE_MSG) */

   RETVALUE(ROK);
} /* nhCCMConSetupHdlr */



/**
 * @brief Handler for RRC Re-establishment Reject message.
 * 
 * @details 
 * 
 *
 * This function encodes the RRC Re-establishment Reject message and indicates 
 * MAC to release the new allocated temporary CRNTI. The message is sent 
 * to RLC. It also releases the lower layer configuration associated with
 * old CRNTI, if present. It updates the message statistics.
 * 
 * 
 * @param[in] datReqRspSdu   RRC Downlink messages
 * @param[in] cellAndUeCb    Cell and UE context
 * @param[in] msgTransId     Transaction identifier 
 * @param[out] errInfo       Error information, in case of error
 * @return S16
 *  -# ROK
 *  -# RFAILED
*/
#ifdef ANSI
PUBLIC S16 nhCCMConRstbRejHdlr 
(
NhDatReqRspSdu    *datReqRspSdu, 
NhCellAndUeCb     *cellAndUeCb,
U8                msgTransId,
NhuErrorInfo      *errInfo
)
#else /* ANSI */
PUBLIC S16 nhCCMConRstbRejHdlr(datReqRspSdu, cellAndUeCb, msgTransId, errInfo)
NhDatReqRspSdu    *datReqRspSdu;
NhCellAndUeCb     *cellAndUeCb;
U8                msgTransId;
NhuErrorInfo      *errInfo;
#endif /* ANSI */
{
   S16                retVal;
   NhDatReqParam      datReqParam;
   NhMsgPduAndType    msgPduAndType;
   NhuDatRspSdus      *datRspSdu;
   NhActvUeCb         *ueCb;

   TRC2(nhCCMConRstbRejHdlr)

   UNUSED(msgTransId);

   RLOG0(L_DEBUG, "nhCCMConRstbRejHdlr called.");

   NH_SET_ZERO(&(datReqParam), sizeof(NhDatReqParam));
   datRspSdu = datReqRspSdu->sdu.datRspSdu;

   /* Fill RLC parameters required by PIM */
   NH_FORM_NH_ID_FRM_DATRSP(datReqParam.nhId, datRspSdu);

   datReqParam.isPdcpDatReq = FALSE;
   datReqParam.u.rlcParam.lcType = CM_LTE_LCH_CCCH;
   datReqParam.u.rlcParam.u.ccchInfo.rnti = 
                        datRspSdu->sdu.m.dlCcchMsg.tmpCrnti;

   NH_FILL_MSG_PDU_AND_TYPE(msgPduAndType, &datRspSdu->sdu.m.dlCcchMsg.ccchMsg,
                            NHU_MSG_DLCCCH);

   /* Encodes and sends message to UE */
   retVal = nhUTLEncAndSnd(cellAndUeCb, 0, &msgPduAndType, 
                           &datReqParam, errInfo);
   if(ROK != retVal)
   {
      RLOG0(L_ERROR, "nhUTLEncAndSnd Failed.");
      RETVALUE(retVal);
   }

#ifndef NH_NO_STS
   /* Update statistics for messages */
   nhUTLUpdCellPduSts(cellAndUeCb->cellCb, NH_MSGTYPE_CONNREESTABREJ,
                   NH_STS_CAUSE_SUCCESS);
#endif /* NH_NO_STS */

    /* The configuration associated with old CRNTI is released, 
     * if CRNTI is present in the header */
   if(datRspSdu->hdr.ueId > 0)
   {
      /* Set the old CRNTI from header */
      datReqParam.nhId.ueId = datRspSdu->hdr.ueId;

      ueCb = nhDBMGetUeCb(cellAndUeCb->cellCb, datReqParam.nhId.ueId);

      if(NULLP == ueCb)
      {
         RLOG0(L_ERROR, "UE Control block does not exist.   "
             "Cannot release the configuration.");
         NH_FILL_NHU_ERROR(*errInfo, NHU_ERRTYPE_INV_NHU_HDR, 
                           NHU_ERRCAUSE_UE_NOT_CONFIGURED);
         RETVALUE(RFAILED);
      }

      retVal = nhCGMCnclUe(cellAndUeCb->cellCb, ueCb);
      if(ROK != retVal)
      {
         RLOG_ARG1(L_ERROR, DBG_CRNTI, datReqParam.nhId.ueId,"nhCGMCnclUe "
            "failed CellId[%d]", datReqParam.nhId.cellId);
         NH_FILL_NHU_ERROR(*errInfo, NHU_ERRTYPE_INV_CFG,
                           NHU_ERRCAUSE_LOW_LYR_CFG_FAIL);
         RETVALUE(retVal);
      }
   } /* end if(datRspSdu->hdr.ueId > 0) */

   RETVALUE(ROK);
} /* nhCCMConRstbRejHdlr */





/**
 * @brief Handler for RRC Connection Reject message.
 * 
 * @details 
 * 
 *
 * This function encodes RRC Connection Reject message and indicates 
 * MAC to release the allocated temporary CRNTI. The message is sent
 * to RLC. It updates the message statistics.
 * 
 * @param[in] datReqRspSdu   RRC Downlink messages
 * @param[in] cellAndUeCb    Cell and UE context
 * @param[in] msgTransId     Transaction identifier 
 * @param[out] errInfo       Error information, in case of error
 * @return S16
 *  -# ROK
 *  -# RFAILED
*/
#ifdef ANSI
PUBLIC S16 nhCCMConRejHdlr 
(
NhDatReqRspSdu    *datReqRspSdu,
NhCellAndUeCb     *cellAndUeCb,
U8                msgTransId,
NhuErrorInfo      *errInfo
)
#else /* ANSI */
PUBLIC S16 nhCCMConRejHdlr(datReqRspSdu, cellAndUeCb, msgTransId, errInfo)
NhDatReqRspSdu    *datReqRspSdu;
NhCellAndUeCb     *cellAndUeCb;
U8                msgTransId;
NhuErrorInfo      *errInfo;
#endif /* ANSI */
{
   S16                retVal;
   NhDatReqParam      datReqParam;
   NhMsgPduAndType    msgPduAndType;
   NhuDatRspSdus      *datRspSdu;

   TRC2(nhCCMConRejHdlr)

   UNUSED(msgTransId);

   RLOG0(L_DEBUG, "nhCCMConRejHdlr called.");

   NH_SET_ZERO(&(datReqParam), sizeof(NhDatReqParam));
   datRspSdu = datReqRspSdu->sdu.datRspSdu;

   NH_FILL_MSG_PDU_AND_TYPE(msgPduAndType, &datRspSdu->sdu.m.dlCcchMsg.ccchMsg, 
                            NHU_MSG_DLCCCH);

   /* Fill RLC parameters required by PIM */
   NH_FORM_NH_ID_FRM_DATRSP(datReqParam.nhId, datRspSdu);

   datReqParam.isPdcpDatReq = FALSE;
   datReqParam.u.rlcParam.lcType = CM_LTE_LCH_CCCH;
   datReqParam.u.rlcParam.u.ccchInfo.rnti = 
               datRspSdu->sdu.m.dlCcchMsg.tmpCrnti;

   /* Encodes and sends message to UE */
   retVal = nhUTLEncAndSnd(cellAndUeCb, 0, &msgPduAndType, &datReqParam, 
                           errInfo);
   if(ROK != retVal)
   {
      RLOG0(L_ERROR, "nhUTLEncAndSnd Failed.");
      RETVALUE(retVal);
   }

#ifndef NH_NO_STS
   /* update stats for messages */
   nhUTLUpdCellPduSts(cellAndUeCb->cellCb, NH_MSGTYPE_CONNREJ, 
                   NH_STS_CAUSE_SUCCESS);
#endif /* NH_NO_STS */

   RETVALUE(ROK);
} /* nhCCMConRejHdlr */




/**
 * 
 * @brief Handler for RRC Connection Setup Complete Message.
 * 
 * @details 
 * 
 *
 * This function updates the message statistics and SRB1 configured 
 * flag and passes the message to the RRC user.
 * 
 * 
 * @param[in] nhuDatIndSdu    RRC Uplink messages
 * @param[in] cellAndUeCb     Cell and UE context
 * @param[in] msgTransId      Transaction identifier 
 * @param[in] msgType         Message type 
 * @return S16
 *  -# ROK
 *  -# RFAILED
*/
#ifdef ANSI
PUBLIC S16 nhCCMConSetupCmpHndlr 
(
NhuDatIndSdus       *nhuDatIndSdu,
NhCellAndUeCb       *cellAndUeCb,
U8                  msgTransId,
U8                  msgType
)
#else /* ANSI */
PUBLIC S16 nhCCMConSetupCmpHndlr(nhuDatIndSdu, cellAndUeCb, msgTransId, msgType)
NhuDatIndSdus       *nhuDatIndSdu;
NhCellAndUeCb       *cellAndUeCb;
U8                  msgTransId;
U8                  msgType;
#endif /* ANSI */
{
   S16            retVal;

   TRC2(nhCCMConSetupCmpHndlr)

   UNUSED(msgTransId);

   RLOG0(L_DEBUG, "nhCCMConSetupCmpHndlr called.");

#ifndef NH_NO_STS
   /* Update the Statistics */
   nhUTLUpdPduSts(cellAndUeCb->ueCb, NH_MSGTYPE_CONNSETUPCOMP, 
               NH_STS_CAUSE_SUCCESS);
#endif /* NH_NO_STS */
   /* Update the General Statistics */
   nhUTLUpdGenSts(NH_STS_CAUSE_CON_SUCC);

   /*Set SRB1 is configured */
   cellAndUeCb->ueCb->rbSecCfgBitMask |= NH_SRB1_CFGD;

   /* Pass message to RRC user */
   retVal = nhUIMDatInd(nhuDatIndSdu);
   if (ROK != retVal)
   {
      RLOG0(L_ERROR, "nhUIMDatInd failed.");
      RETVALUE(retVal);
   }

   RETVALUE(ROK);
} /* nhCCMConSetupCmpHndlr */




/**
 * @brief Handler for RRC Connection Release Message.
 * 
 * @details 
 * 
 *
 * This function encodes and sends the RRC Connection Release Message 
 * sent in DL. It creates the transaction context for LIM to call CCM handler when
 * data confirm is received for this message. It updates the message statistics.
 * 
 * 
 * @param[in] datReqRspSdu   RRC Downlink messages
 * @param[in] cellAndUeCb    Cell and UE context
 * @param[in] msgTransId     Transaction identifier 
 * @param[out] errInfo       Error information, in case of error
 * @return S16
 *  -# ROK
 *  -# RFAILED
*/
#ifdef ANSI
PUBLIC S16 nhCCMConRlsHndlr 
(
NhDatReqRspSdu    *datReqRspSdu, 
NhCellAndUeCb     *cellAndUeCb, 
U8                msgTransId, 
NhuErrorInfo      *errInfo
)
#else /* ANSI */
PUBLIC S16 nhCCMConRlsHndlr(datReqRspSdu, cellAndUeCb, msgTransId, errInfo)
NhDatReqRspSdu    *datReqRspSdu;
NhCellAndUeCb     *cellAndUeCb;
U8                msgTransId;
NhuErrorInfo      *errInfo;
#endif/* ANSI */
{
   S16                retVal;
   NhDatReqParam       datReqParam;
   NhMsgPduAndType    msgPduAndType;
   MsgType            msgType;
   U32                msgTransKey;
   TransCb             *transCb;
   NhuDatReqSdus      *datReqSdu;

   TRC2(nhCCMConRlsHndlr)

   RLOG0(L_DEBUG, "nhCCMConRlsHndlr called.");

   NH_SET_ZERO(&(datReqParam), sizeof(NhDatReqParam));
   datReqSdu = datReqRspSdu->sdu.datReqSdu;

   msgType = NH_MSGTYPE_CONNRLS;

   NH_GET_TRANSCB_KEY(msgTransKey, msgType, msgTransId);

   /* Create transaction block for the configuration */
   retVal = nhDBMAddTransCb(cellAndUeCb->ueCb, &transCb, msgTransKey);

   if (ROK != retVal)
   {
      RLOG0(L_ERROR, "Unable to create transaction control block.");
      RETVALUE(retVal);
   }

   transCb->cause = msgType;
   transCb->nxTransId = datReqSdu->hdr.transId;

   NH_FILL_MSG_PDU_AND_TYPE(msgPduAndType,
                            &datReqSdu->sdu.ueMsg.msg.dedMsg.dlDcchMsg.m.dcchMsg,
                            NHU_MSG_DLDCCH);

   /* Fill RLC parameters required by PIM */
   NH_FORM_NHID_FRM_DATREQ(datReqParam.nhId, datReqSdu);
   datReqParam.isPdcpDatReq = TRUE;
   datReqParam.isMsgEncoded = FALSE;

   /* Encodes and sends message to UE */
   retVal = nhUTLEncAndSnd(cellAndUeCb, datReqSdu->hdr.transId, 
                           &msgPduAndType, &datReqParam, errInfo);
   if(ROK != retVal)
   {
      RLOG0(L_ERROR, "nhUTLEncAndSnd Failed.");
      nhDBMDelTransCb(cellAndUeCb->ueCb, &transCb);
      RETVALUE(retVal);
   }

#ifndef NH_NO_STS
   /* Update the Statistics */
   nhUTLUpdPduSts(cellAndUeCb->ueCb, msgType, NH_STS_CAUSE_SUCCESS);
#endif /* NH_NO_STS */

   RETVALUE(ROK);
} /* nhCCMConRlsHndlr */




/**
 * 
 * @brief Handler for RRC Connection Release Message Confirm.
 * 
 * @details 
 * 
 *
 * This function initiates the release of lower layer configuration and deletes
 * the transaction context created for LIM while sending the RRC Connection 
 * Release message. 
 * 
 * 
 * @param[in] nhuDatIndSdu    RRC Uplink messages
 * @param[in] cellAndUeCb     Cell and UE context
 * @param[in] msgTransId      Transaction identifier 
 * @param[in] msgType         Message type 
 * @return S16
 *  -# ROK
 *  -# RFAILED
*/
#ifdef ANSI
PUBLIC S16 nhCCMConRlsCmpHndlr 
(
NhuDatIndSdus       *nhuDatIndSdu,
NhCellAndUeCb       *cellAndUeCb,
U8                  msgTransId,
U8                  msgType
)
#else /* ANSI */
PUBLIC S16 nhCCMConRlsCmpHndlr(nhuDatIndSdu, cellAndUeCb, msgTransId, msgType)
NhuDatIndSdus       *nhuDatIndSdu;
NhCellAndUeCb       *cellAndUeCb;
U8                  msgTransId;
U8                  msgType;
#endif /* ANSI */
{
   S16      retVal;
   TransCb  *transCb = NULLP;
   U32      msgTransKey;

   TRC2(nhCCMConRlsCmpHndlr)

   UNUSED(nhuDatIndSdu);

   RLOG0(L_DEBUG, "nhCCMConRlsCmpHndlr called.");

   NH_GET_TRANSCB_KEY(msgTransKey, NH_MSGTYPE_CONNRLS, msgTransId);
   transCb = nhUTLGetTransCbFrmMsgTransId(cellAndUeCb->ueCb, msgTransKey);

   /* Transcb must be present, because it helps to call this 
    * function from LIM, based on the cause in the transCb. */
   if(NULLP == transCb) 
   {
      RLOG_ARG1(L_DEBUG, DBG_CRNTI, cellAndUeCb->ueCb->ueCbKey.crnti,
         "Unable to retrieve transCb with transId[%d].", msgTransId);
      RETVALUE(RFAILED);
   }
   
   /* Delete the transaction control block */
   nhDBMDelTransCb(cellAndUeCb->ueCb, &transCb);

   retVal = nhCGMCnclUe(cellAndUeCb->cellCb, cellAndUeCb->ueCb);
   if(ROK != retVal)
   {
      RLOG0(L_ERROR, "nhCGMCnclUe failed.");
      RETVALUE(retVal);
   }

   RETVALUE(ROK);
} /* nhCCMConRlsCmpHndlr */


/**
 * @brief Handler for RRC Connection Re-configuration Message.
 * 
 * @details 
 * 
 *
 * This function encodes and sends the RRC Connection Re-configuration Message 
 * sent in DL. It stores the configuration information, if received with 
 * the message. It updates the message statistics.
 * 
 * 
 * @param[in] datReqRspSdu   RRC Downlink messages
 * @param[in] cellAndUeCb    Cell and UE context
 * @param[in] msgTransId     Transaction identifier 
 * @param[out] errInfo       Error information, in case of error
 * @return S16
 *  -# ROK
 *  -# RFAILED
*/
#ifdef ANSI
PUBLIC S16 nhCCMReCfgReqHndlr 
(
NhDatReqRspSdu     *datReqRspSdu, 
NhCellAndUeCb      *cellAndUeCb, 
U8                 msgTransId,
NhuErrorInfo       *errInfo
)
#else /* ANSI */
PUBLIC S16 nhCCMReCfgReqHndlr(datReqRspSdu, cellAndUeCb, msgTransId, errInfo)
NhDatReqRspSdu    *datReqRspSdu;
NhCellAndUeCb     *cellAndUeCb;
U8                msgTransId;
NhuErrorInfo      *errInfo;
#endif /* ANSI */
{
   S16                retVal;
   MsgType            msgType;
   U32                msgTransKey;
   TransCb            *transCb = NULLP;
   NhDatReqParam      datReqParam;
   NhMsgPduAndType    msgPduAndType;
   NhuDatReqSdus      *datReqSdu;
   NhuRRCConRecfgn_r8_IEs  *ies;
   Bool               isTransCbCreated = FALSE;

   TRC2(nhCCMReCfgReqHndlr)

   RLOG0(L_DEBUG, "nhCCMReCfgReqHndlr called.");

   NH_SET_ZERO(&(datReqParam), sizeof(NhDatReqParam));
   datReqSdu = datReqRspSdu->sdu.datReqSdu;

   /* Security procedure must be completed before sending 
    * this message */

   NH_CONDIES_GET_SDU_TYPE(datReqSdu, EVTNHUCFGREQ, ies);
   if (  !(NH_IS_IE_PRSNT(ies->securityConfigHO) ||
         datReqSdu->sdu.ueCfg.conReCfgCause & NHU_CONN_RECFG_FOR_MESR_CFG))
   {
#ifdef DISABLE_RRCSEC
            cellAndUeCb->ueCb->rbSecCfgBitMask |= NH_SEC_CMPLTD | NH_SEC_INIT;
#endif
      if(!(cellAndUeCb->ueCb->rbSecCfgBitMask & (NH_SEC_CMPLTD | NH_SEC_INIT)))
      {
         RLOG0(L_ERROR, "Message Cannot be sent unprotected.");
         NH_FILL_NHU_ERROR(*errInfo, NHU_ERRTYPE_UE_CFG, 
                                     NHU_ERRCAUSE_SEC_NOT_CONFIGURED);
         RETVALUE(RFAILED);
      }
   }

#ifdef LTERRC_REL9
   /* R9 Upgrade:NH_NW_90: MobilityFromEutra procedure is initiated only when 
    * AS-security has been activated, and SRB2 with at least one DRB are setup
    * and not suspended.
    */
   if (NH_IS_IE_PRSNT(ies->mobilityCntrlInfo))
   { 
      /* SRB2 must be established before sending this message */
      if(!(cellAndUeCb->ueCb->crntCfg.bitMskPres & NH_CRNT_CFG_SRB2))
      {
         RLOG0(L_ERROR, "SRB2 is not establisged.");
         NH_FILL_NHU_ERROR(*errInfo, NHU_ERRTYPE_UE_CFG, 
               NHU_ERRCAUSE_SRB2_NOT_ESTABLISHED);
         RETVALUE(RFAILED);
      }

      /* Atleast One DRB must be established before sending this message */
      if(!(cellAndUeCb->ueCb->crntCfg.bitMskPres & NH_CRNT_CFG_DRBS))
      {
         RLOG0(L_ERROR, "Atleast one DRB is not established.");
         NH_FILL_NHU_ERROR(*errInfo, NHU_ERRTYPE_UE_CFG, 
               NHU_ERRCAUSE_DRBS_NOT_ESTABLISHED);
         RETVALUE(RFAILED);
      }
   }
#endif /*LTERRC_REL9*/

   msgType = NH_MSGTYPE_CONNRECFG;

   /* nh003.201 - Order of config during RRC conn reconfig is changed. */
   if (TRUE == datReqSdu->sdu.isUeCfgPres)
    {
      NH_GET_TRANSCB_KEY(msgTransKey, msgType, msgTransId);

      /* Create transaction block for the configuration */
      retVal = nhDBMAddTransCb(cellAndUeCb->ueCb, &transCb, msgTransKey);

      if (ROK != retVal)
      {
         RLOG0(L_ERROR, "Unable to create transaction control block.");
         RETVALUE(retVal);
      }
   
      transCb->cause = msgType;
      transCb->nxTransId = datReqSdu->hdr.transId;
      isTransCbCreated = TRUE;

      /* Fill PDCP Parameters required by PIM */
      NH_FORM_NHID_FRM_DATREQ(datReqParam.nhId, datReqSdu);
      datReqParam.isPdcpDatReq = TRUE;

      NH_FILL_MSG_PDU_AND_TYPE(msgPduAndType, 
                  &datReqSdu->sdu.ueMsg.msg.dedMsg.dlDcchMsg.m.dcchMsg, 
                  NHU_MSG_DLDCCH);

      /* nh003.201 - Order of config during RRC conn reconfig is changed. */
      /* Encodes the message and stores it in transCb */
      retVal = nhUTLEncAndStoreMsg(&msgPduAndType, &datReqParam,
            transCb, errInfo);
      if (ROK != retVal)
      {
         RLOG0(L_ERROR, "nhUTLEncAndStoreMsg failed.");
         /* nh004.201 - In case of error delete the transCb created. */
         NH_CCM_DEL_UECB_TRANSCB(isTransCbCreated, &transCb, FALSE,
                                 NULLP, &(cellAndUeCb->ueCb));
         RETVALUE(retVal);
      }

      /* Send the configuration stored in transCb */
      retVal = nhCGMLowerLyrCfgReq(cellAndUeCb, &datReqSdu->sdu.ueCfg, 
                                   transCb, TRUE);
      if (ROK != retVal)
      {
         RLOG0(L_ERROR, "Cannot send lower layer configuration.");
         NH_CCM_DEL_UECB_TRANSCB(isTransCbCreated, &transCb, FALSE,
                                 NULLP, &(cellAndUeCb->ueCb));
         /* nh004.201 - In case of error fill the error type and cause. */
         NH_FILL_NHU_ERROR(*errInfo, NHU_ERRTYPE_UE_CFG,
                           NHU_ERRCAUSE_LLYR_SAP_NOT_EXIST);
         RETVALUE(retVal);
      }
   }
   else
   {
      /* Fill PDCP Parameters required by PIM */
      NH_FORM_NHID_FRM_DATREQ(datReqParam.nhId, datReqSdu);
      datReqParam.isPdcpDatReq = TRUE;
   
      NH_FILL_MSG_PDU_AND_TYPE(msgPduAndType,
               &datReqSdu->sdu.ueMsg.msg.dedMsg.dlDcchMsg.m.dcchMsg, 
               NHU_MSG_DLDCCH);

      /* Encodes and sends message to UE */
      retVal = nhUTLEncAndSnd(cellAndUeCb, datReqSdu->hdr.transId,
                        &msgPduAndType, &datReqParam, errInfo);
      if(ROK != retVal)
      {
         RLOG0(L_ERROR, "nhUTLEncAndSnd Failed.");
         NH_CCM_DEL_UECB_TRANSCB(isTransCbCreated, &transCb, FALSE,
               NULLP, &(cellAndUeCb->ueCb));
         RETVALUE(retVal);
      }
   }

#ifndef NH_NO_STS
   /* Update the Statistics */
   nhUTLUpdPduSts(cellAndUeCb->ueCb, msgType, NH_STS_CAUSE_SUCCESS);
#endif /* NH_NO_STS */

   RETVALUE(ROK);
} /* nhCCMReCfgReqHndlr */


/**
 * @brief Handler for RRC Connection Re-configuration Complete Message.
 * 
 * @details 
 * 
 *
 * This function performs the lower layer configuration, if it has stored 
 * the configuration information. It updates the message statistics and passes
 * the message to the RRC user.
 * 
 * 
 * @param[in] nhuDatIndSdu    RRC Uplink messages
 * @param[in] cellAndUeCb     Cell and UE context
 * @param[in] msgTransId      Transaction identifier 
 * @param[in] msgType         Message type 
 *
 * @return S16
 *  -# ROK
 *  -# RFAILED
*/
#ifdef ANSI
PUBLIC S16 nhCCMReCfgCmpHndlr
(
NhuDatIndSdus       *nhuDatIndSdu,
NhCellAndUeCb       *cellAndUeCb,
U8                  msgTransId,
U8                  msgType
)
#else /* ANSI */
PUBLIC S16 nhCCMReCfgCmpHndlr(nhuDatIndSdu, cellAndUeCb, msgTransId, msgType)
NhuDatIndSdus       *nhuDatIndSdu;
NhCellAndUeCb       *cellAndUeCb;
U8                  msgTransId;
U8                  msgType;
#endif /* ANSI */
{
   S16               retVal;
   TransCb           *transCb = NULLP;
   U32               msgTransKey;
  
   
   /* nh003.201 - removing un-used variable */

   TRC2(nhCCMReCfgCmpHndlr)

   RLOG0(L_DEBUG, "nhCCMReCfgCmpHndlr called.");

  

#ifndef NH_NO_STS
   /* Update the Statistics */
   nhUTLUpdPduSts(cellAndUeCb->ueCb, msgType, NH_STS_CAUSE_SUCCESS);
#endif /* NH_NO_STS */

   if(cellAndUeCb->ueCb->hoToEutraOrMobFrmEutra == NH_MSGTYPE_CONNRECFGCOMP_HO)
   {
#ifndef NH_NO_STS
      /* Update the Cell Statistics */
      nhUTLUpdCellPduSts(cellAndUeCb->cellCb, NH_MSGTYPE_CONNRECFGCOMP_HO, 
                         NH_STS_CAUSE_HO_TO_EUTRA_SUCC);
#endif /* NH_NO_STS */
      /* Pass message to the RRC user */
      retVal = nhUIMDatInd(nhuDatIndSdu);
      if (ROK != retVal)
      {
         RLOG0(L_ERROR, "nhUIMDatInd failed.");
      }
      cellAndUeCb->ueCb->hoToEutraOrMobFrmEutra = FALSE;
      RETVALUE(retVal);
   }

   /* Update the General Statistics */
   nhUTLUpdGenSts(NH_STS_CAUSE_RECFG_SUCC);

   NH_GET_TRANSCB_KEY(msgTransKey, NH_MSGTYPE_CONNRECFG, msgTransId);
   transCb = nhUTLGetTransCbFrmMsgTransId(cellAndUeCb->ueCb, msgTransKey);

   /* nh003.201 - Order of config during RRC conn reconfig is changed. */
   if(NULLP != transCb) 
   {
      /* Delete the transaction control block */
      nhDBMDelTransCb(cellAndUeCb->ueCb, &transCb);
   }

   /* Pass message to the RRC user */
   retVal = nhUIMDatInd(nhuDatIndSdu);
   if (ROK != retVal)
   {
      RLOG0(L_ERROR, "nhUIMDatInd failed.");
      RETVALUE(retVal);
   }

   RETVALUE(ROK);
} /* nhCCMReCfgCmpHndlr */

/**
 * @brief Handler for Security Mode Command Message.
 * 
 * @details 
 * This function configures the integrity protection and ciphering. It 
 * stores the encoded Security Mode Command Message sent in DL,if it receives 
 * the security configuration. If security configuration is not present, 
 * it encodes and sends Security Mode Command to PDCP and updates the message
 * statistics. It configures Selective Security Mode as applicable.
 * 
 * @param[in] datReqRspSdu   RRC Downlink messages
 * @param[in] cellAndUeCb    Cell and UE context
 * @param[in] msgTransId     Transaction identifier 
 * @param[out] errInfo       Error information, in case of error
 * 
 * @return S16
 *  -# ROK
 *  -# RFAILED
*/
#ifdef ANSI
PUBLIC S16 nhCCMSecModCmdHndlr
(
NhDatReqRspSdu    *datReqRspSdu,
NhCellAndUeCb       *cellAndUeCb,
U8                msgTransId,
NhuErrorInfo      *errInfo
)
#else /* ANSI */
PUBLIC S16 nhCCMSecModCmdHndlr(datReqRspSdu, cellAndUeCb, msgTransId, errInfo)
NhDatReqRspSdu    *datReqRspSdu;
NhCellAndUeCb       *cellAndUeCb;
U8                msgTransId;
NhuErrorInfo      *errInfo;
#endif /* ANSI */
{
   S16               retVal;
   MsgType           msgType;
   U32               msgTransKey;
   TransCb           *transCb;
   NhuDatReqSdus     *datReqSdu;
   NhMsgPduAndType   msgPduAndType;
   NhDatReqParam     datReqParam;
   Bool              isTransCbCreated = FALSE;

   TRC2(nhCCMSecModCmdHndlr)

   RLOG0(L_DEBUG, "nhCCMSecModCmdHndlr called.");

   NH_SET_ZERO(&(datReqParam), sizeof(NhDatReqParam));
   datReqSdu = datReqRspSdu->sdu.datReqSdu;

   msgType = NH_MSGTYPE_SECMODCMD;


   NH_FILL_MSG_PDU_AND_TYPE(msgPduAndType,
         &datReqSdu->sdu.ueMsg.msg.dedMsg.dlDcchMsg.m.dcchMsg, NHU_MSG_DLDCCH);

   /* Fill PDCP parameters required by PIM */
   NH_FORM_NHID_FRM_DATREQ(datReqParam.nhId, datReqSdu);
   datReqParam.isPdcpDatReq = TRUE;

   if (TRUE == datReqSdu->sdu.isUeCfgPres)
   {
      NH_GET_TRANSCB_KEY(msgTransKey, msgType, msgTransId);

      /* create transaction block for the configuration */
      retVal = nhDBMAddTransCb(cellAndUeCb->ueCb, &transCb, msgTransKey);

      if (ROK != retVal)
      {
         RLOG0(L_ERROR, "Unable to create transaction control block.");
         RETVALUE(retVal);
      }

      transCb->cause = msgType;
      transCb->nxTransId = datReqSdu->hdr.transId;
      isTransCbCreated = TRUE;

      /* Stores the security configuation */
      retVal = nhCGMLowerLyrCfgReq(cellAndUeCb, &datReqSdu->sdu.ueCfg,
                                   transCb, FALSE);
      if (ROK != retVal)
      {
         RLOG0(L_ERROR, "Could not store lower layer config.");
         RETVALUE(retVal);
      }

      /* If security configuration is present */
      if(transCb->ordrdCfg.actionMask & NH_ORD_CFG_SEC)
      {
         /* Encodes the message and stores it in transCb */
         retVal = nhUTLEncAndStoreMsg(&msgPduAndType, &datReqParam,
                                      transCb, errInfo);
         if (ROK != retVal)
         {
            RLOG0(L_ERROR, "nhUTLEncAndStoreMsg failed.");
            NH_CCM_DEL_UECB_TRANSCB(isTransCbCreated, &transCb, FALSE,
                                    NULLP, &(cellAndUeCb->ueCb));
            RETVALUE(retVal);
         }

         if(datReqSdu->sdu.ueCfg.conReCfgCause & NHU_CONN_RECFG_AT_HNDOVR ||
               cellAndUeCb->ueCb->rbSecCfgBitMask & NH_SEC_CFG_DONE)
         {
            /* Selective Security Mode Config is disbale in this case */
            transCb->enblSelSecModeCfg = NH_SEL_SEC_MOD_CFG_DISABLE;
         }
         else
         {
            /* Enable Selective Security Mode Config is disbale if it is
               not the in this case */
            transCb->enblSelSecModeCfg = NH_SEL_SEC_MOD_CFG_ENABLE;
         }

         /* Use CGM to configure security information at PDCP */
         retVal = nhCGMSecCfgReq(cellAndUeCb, transCb);
         if (ROK != retVal)
         {
            RLOG0(L_ERROR, "Cannot perform lower layer config.");
            /* Delete Trans CB only if Lower Layer Config is not sent */
            if(!transCb->numOfPdcpCfms)
            {
               NH_CCM_DEL_UECB_TRANSCB(isTransCbCreated, &transCb, FALSE,
                     NULLP, &(cellAndUeCb->ueCb));
            }

            NH_FILL_NHU_ERROR(*errInfo, NHU_ERRTYPE_UE_CFG,
                  NHU_ERRCAUSE_LOW_LYR_CFG_FAIL);

            RETVALUE(retVal);
         }

         RETVALUE(ROK);
      } /* end if(transCb->ordrdCfg.expCfgBitMsk & NH_ORD_CFG_SEC_CFG) */
      else
      {
         /* If Security is not yet configured, Sec Mod Cmd cannot be sent */
         if (!(cellAndUeCb->ueCb->rbSecCfgBitMask & NH_SEC_CFG_DONE))
         {
            RLOG0(L_ERROR,"Security not configured, Can't send Sec Mode Command");
            NH_CCM_DEL_UECB_TRANSCB(isTransCbCreated, &transCb, FALSE, NULLP,
                                                   &(cellAndUeCb->ueCb));
           
           

            NH_FILL_NHU_ERROR(*errInfo, NHU_ERRTYPE_UE_CFG,
                  NHU_ERRCAUSE_SEC_NOT_CONFIGURED);

            RETVALUE(RFAILED);
         }

      } /* else of if(transCb->ordrdCfg.expCfgBitMsk & NH_ORD_CFG_SEC_CFG) */

   } /* end if (TRUE == datReqSdu->sdu.isUeCfgPres) */

   /* Encodes and sends message to UE */
   retVal = nhUTLEncAndSnd(cellAndUeCb, datReqSdu->hdr.transId,
         &msgPduAndType, &datReqParam, errInfo);
   if(ROK != retVal)
   {
      RLOG0(L_ERROR, "nhUTLEncAndSnd Failed.");
      NH_CCM_DEL_UECB_TRANSCB(isTransCbCreated, &transCb, FALSE,
            NULLP, &(cellAndUeCb->ueCb));
      NH_FILL_NHU_ERROR(*errInfo, NHU_ERRTYPE_UE_CFG,
            NHU_ERRCAUSE_SEC_NOT_CONFIGURED);

      RETVALUE(retVal);
   }

      /*Set Security Mode Command is initiated */
   cellAndUeCb->ueCb->rbSecCfgBitMask |= NH_SEC_INIT;

#ifndef NH_NO_STS
   /* Update the Statistics */
   nhUTLUpdPduSts(cellAndUeCb->ueCb, msgType, NH_STS_CAUSE_SUCCESS);
#endif /* NH_NO_STS */

   RETVALUE(ROK);
} /* nhCCMSecModCmdHndlr */


/**
 * @brief Handler for Security Mode Complete Message
 * 
 * @details 
 * 
 *
 * This function sends the Security Mode Complete message to the RRC user. 
 * It configures ciphering information, if the configuration information
 * is stored in the transaction control block. If configuration is not
 * present, it deletes the transaction control block. It updates the
 * message statistics.
 * 
 * 
 * @param[in] nhuDatIndSdu    RRC Uplink messages
 * @param[in] cellAndUeCb     Cell and UE context
 * @param[in] msgTransId      Transaction identifier 
 * @param[in] msgType         Message type 
 * @return S16
 *  -# ROK
 *  -# RFAILED
*/
#ifdef ANSI
PUBLIC S16 nhCCMSecModCmpHndlr
(
NhuDatIndSdus       *nhuDatIndSdu,
NhCellAndUeCb       *cellAndUeCb,
U8                  msgTransId,
U8                  msgType
)
#else /* ANSI */
PUBLIC S16 nhCCMSecModCmpHndlr(nhuDatIndSdu, cellAndUeCb, msgTransId, msgType)
NhuDatIndSdus       *nhuDatIndSdu;
NhCellAndUeCb       *cellAndUeCb;
U8                  msgTransId;
U8                  msgType;
#endif/* ANSI */
{
   S16       retVal;
   TransCb   *transCb = NULLP;
   U32       transCbKey;
   U32       msgTransKey;
  
   

   TRC2(nhCCMSecModCmpHndlr)

   RLOG0(L_DEBUG, "nhCCMSecModCmpHndlr called.");

   

#ifndef NH_NO_STS
   /* Update the Statistics */
   nhUTLUpdPduSts(cellAndUeCb->ueCb, msgType, NH_STS_CAUSE_SUCCESS);
#endif /* NH_NO_STS */

   /* Set Security Mode Command is initiated */
   cellAndUeCb->ueCb->rbSecCfgBitMask |= NH_SEC_CMPLTD;
   cellAndUeCb->ueCb->rbSecCfgBitMask &= ~NH_SEC_INIT;

   NH_GET_TRANSCB_KEY(msgTransKey, NH_MSGTYPE_SECMODCMD, msgTransId);
   transCb = nhUTLGetTransCbFrmMsgTransId(cellAndUeCb->ueCb, msgTransKey);

   if(NULLP == transCb)
   {
      /* This may be separate Dat request from user,
         in this case no TransCb exists, So create it */

      /* user RX transId as key */
      transCbKey = nhuDatIndSdu->hdr.transId;
      /* create transaction block for the configuration */
      retVal = nhDBMAddTransCb(cellAndUeCb->ueCb, &transCb, transCbKey);
      if (ROK != retVal)
      {
         RLOG0(L_ERROR, "NULL transCb found.");
         RETVALUE(RFAILED);
      }

      /* Set cause to Sec Mod Cmp as this is done after 
         initial Sec Activation procedure */

      transCb->cause = NH_MSGTYPE_SECMODCMD;
   }

   retVal = nhCGMPdcpDiscReqdCfgReq(transCb, cellAndUeCb);
   if(ROK != retVal)
   {
      RLOG0(L_ERROR, "DiscReqd cfg request failed .");
   }

   /* Pass message to the RRC user */
   retVal = nhUIMDatInd(nhuDatIndSdu);
   if (ROK != retVal)
   {
      RLOG0(L_ERROR, "nhUIMDatInd failed.");
      RETVALUE(retVal);
   }

   RETVALUE(ROK);
} /* nhCCMSecModCmpHndlr */


/**
 * @brief Handler for Security Mode Failure Message.
 * 
 * @details 
 * 
 *
 * This function sends the Security Mode Failure message to the RRC user
 * and updates the message statistics. It deletes the transaction
 * control block, if present. 
 * 
 * 
 * @param[in] nhuDatIndSdu    RRC Uplink messages
 * @param[in] cellAndUeCb     Cell and UE context
 * @param[in] msgTransId      Transaction identifier 
 * @param[in] msgType         Message type 
 * @return S16
 *  -# ROK
 *  -# RFAILED
*/
#ifdef ANSI
PUBLIC S16 nhCCMSecModFailHndlr 
(
NhuDatIndSdus       *nhuDatIndSdu,
NhCellAndUeCb       *cellAndUeCb,
U8                  msgTransId,
U8                  msgType
)
#else /* ANSI */
PUBLIC S16 nhCCMSecModFailHndlr(nhuDatIndSdu, cellAndUeCb, msgTransId, msgType)
NhuDatIndSdus       *nhuDatIndSdu;
NhCellAndUeCb       *cellAndUeCb;
U8                  msgTransId;
U8                  msgType;
#endif /* ANSI */
{
   S16       retVal;
   TransCb   *transCb = NULLP;
   U32       msgTransKey;

   TRC2(nhCCMSecModFailHndlr)

   RLOG0(L_DEBUG, "nhCCMSecModFailHndlr called.");

   NH_GET_TRANSCB_KEY(msgTransKey, NH_MSGTYPE_SECMODCMD, msgTransId);
   transCb = nhUTLGetTransCbFrmMsgTransId(cellAndUeCb->ueCb, msgTransKey);

   if(NULLP != transCb) 
   {
      /* Delete the transaction control block */
      nhDBMDelTransCb(cellAndUeCb->ueCb, &transCb);
   }

#ifndef NH_NO_STS
   /* Update the Statistics */
   nhUTLUpdPduSts(cellAndUeCb->ueCb, msgType, NH_STS_CAUSE_SUCCESS);
#endif /* NH_NO_STS */

   /* Pass message to the RRC user */
   retVal = nhUIMDatInd(nhuDatIndSdu);
   if (ROK != retVal)
   {
      RLOG0(L_ERROR, "nhIMDatInd failed.");
      RETVALUE(retVal);
   }

   RETVALUE(ROK);
} /* nhCCMSecModFailHndlr */

/**
 * @brief Handler for UL CCCH messages
 * 
 * @details 
 * 
 * This function updates the statistics for the following messages:
 * - RRC Connection Request
 * - RRC Re-establishment Request
 * It passes the message to the RRC user.
 * 
 * @param[in] nhuDatIndSdu    RRC Uplink messages
 * @param[in] cellAndUeCb     Cell and UE context
 * @param[in] msgTransId      Transaction identifier 
 * @param[in] msgType         Message type 
 * @return S16
 *  -# ROK
 *  -# RFAILED
*/
#ifdef ANSI
PUBLIC S16 nhCCMUlCmnHndlr 
(
NhuDatIndSdus       *nhuDatIndSdu,
NhCellAndUeCb       *cellAndUeCb,
U8                  msgTransId,
U8                  msgType
)
#else /* ANSI */
PUBLIC S16 nhCCMUlCmnHndlr(nhuDatIndSdu, cellAndUeCb, msgTransId, msgType)
NhuDatIndSdus       *nhuDatIndSdu;
NhCellAndUeCb       *cellAndUeCb;
U8                  msgTransId;
U8                  msgType;
#endif /* ANSI */
{
   S16                                 retVal;
   NhuRRCConReestbRqstcriticalExtns    criticalExtns;
   NhuUL_CCCH_MsgTypc1                 c1TypeMsg;
   U8                                  ueId;
   NhActvUeCb                          *ueCb;

   TRC2(nhCCMUlCmnHndlr)

   UNUSED(msgTransId);

   RLOG0(L_DEBUG, "nhCCMUlCmnHndlr called.");

#ifndef NH_NO_STS
   /* Update the Statistics */
   nhUTLUpdCellPduSts(cellAndUeCb->cellCb, msgType, NH_STS_CAUSE_SUCCESS);
#endif /* NH_NO_STS */

   /* Update General Statistics in case of reconfiguration failure */
   if(NH_MSGTYPE_CONNREESTABREQ == msgType)
   {
      c1TypeMsg = nhuDatIndSdu->sdu.m.ulCcchMsg.ccchMsg.message.val.c1;
      criticalExtns = c1TypeMsg.val.rrcConReestbRqst.criticalExtns;
      if(NhuReestbCausereconfigurationFailEnum == 
            criticalExtns.val.rrcConReestbRqst_r8.reestablishmentCause.val)
      {
         nhUTLUpdGenSts(NH_STS_CAUSE_RECFG_FAIL);
      }
#ifndef NH_NO_STS
      if(NhuReestbCausehandoverFailEnum == 
           criticalExtns.val.rrcConReestbRqst_r8.reestablishmentCause.val) 
      {
         ueId = *(criticalExtns.val.rrcConReestbRqst_r8.ue_Identity.c_RNTI.val);
         ueCb = nhDBMGetUeCb(cellAndUeCb->cellCb, ueId);

         if( (NULLP != ueCb) && 
             (NH_MSGTYPE_MOBFRMEUTRAN == ueCb->hoToEutraOrMobFrmEutra) )
         {
            /* Update the Statistics */
            nhUTLUpdCellPduSts(cellAndUeCb->cellCb, NH_MSGTYPE_MOBFRMEUTRAN, 
                               NH_STS_CAUSE_MOB_FRM_EUTRA_FAIL);
            ueCb->hoToEutraOrMobFrmEutra = FALSE;
         }
      }
#endif /* NH_NO_STS */
   }

   /* Pass message to the RRC user */
   retVal = nhUIMDatInd(nhuDatIndSdu);
   if (ROK != retVal)
   {
      RLOG0(L_ERROR, "nhUIMDatInd failed.");
      RETVALUE(retVal);
   }

   RETVALUE(ROK);
} /* nhCCMUlCmnHndlr */

/**
 * @brief Handler for RRC Counter Check Message.
 * 
 * @details 
 * This function receives, encodes Counter Check message and stores it. 
 * It validates parameters provided to it and sends stored Counter Check 
 * Message to PDCP using LIM and updates the message statistics.
 * 
 * @param[in] datReqRspSdu   RRC Downlink messages
 * @param[in] cellAndUeCb    Cell and UE context
 * @param[in] msgTransId     Transaction identifier 
 * @param[out] errInfo       Error information, in case of error 
 * 
 * @return S16
 *  -# ROK
 *  -# RFAILED
*/
#ifdef ANSI
PUBLIC S16 nhCCMCountChkHndlr
(
NhDatReqRspSdu *datReqRspSdu,
NhCellAndUeCb  *cellAndUeCb,
U8             msgTransId,
NhuErrorInfo   *errInfo
)
#else /* ANSI */
PUBLIC S16 nhCCMCountChkHndlr  (datReqRspSdu, cellAndUeCb, msgTransId,  errInfo)
NhDatReqRspSdu *datReqRspSdu;
NhCellAndUeCb  *cellAndUeCb;
U8             msgTransId;
NhuErrorInfo   *errInfo;
#endif /* ANSI */
{
   S16               retVal;
   MsgType           msgType;
   TransCb           *transCb;
   NhuDatReqSdus     *datReqSdu;
   NhMsgPduAndType   msgPduAndType;
   NhDatReqParam     datReqParam;
   Bool              isTransCbCreated = FALSE;

   TRC2(nhCCMCountChkHndlr)

   RLOG0(L_DEBUG, "nhCCMCountChkHndlr called.");

   NH_SET_ZERO(&(datReqParam), sizeof(NhDatReqParam));
   datReqSdu = datReqRspSdu->sdu.datReqSdu;

   msgType = NH_MSGTYPE_COUNTCHK;


   NH_FILL_MSG_PDU_AND_TYPE(msgPduAndType,
         &datReqSdu->sdu.ueMsg.msg.dedMsg.dlDcchMsg.m.dcchMsg, NHU_MSG_DLDCCH);

   NH_FORM_NHID_FRM_DATREQ(datReqParam.nhId, datReqSdu);
   datReqParam.isPdcpDatReq = TRUE;

   datReqParam.isMsgEncoded = FALSE;

   /* Encodes and sends message to UE */
   retVal = nhUTLEncAndSnd(cellAndUeCb, datReqSdu->hdr.transId,
         &msgPduAndType, &datReqParam, errInfo);
   if(ROK != retVal)
   {
      RLOG0(L_ERROR, "nhUTLEncAndSnd Failed.");
      NH_CCM_DEL_UECB_TRANSCB(isTransCbCreated, &transCb, FALSE,
            NULLP, &(cellAndUeCb->ueCb));

      RETVALUE(retVal);
   }

#ifndef NH_NO_STS
   /* Update the Statistics */
   nhUTLUpdPduSts(cellAndUeCb->ueCb, msgType, NH_STS_CAUSE_SUCCESS);
#endif /* NH_NO_STS */

   RETVALUE(ROK);
} /*nhCCMCountChkHndlr */

/**
 * @brief Handler for UL DCCH messages
 * 
 * @details 
 * 
 * This function updates the statistics for the following messages:
 * - RRC Connection Re-establishment complete message
 * It passes the message to the RRC user.
 * 
 * @param[in] nhuDatIndSdu    RRC Uplink messages
 * @param[in] cellAndUeCb     Cell and UE context
 * @param[in] msgTransId      Transaction identifier 
 * @param[in] msgType         Message type 
 * @return S16
 *  -# ROK
 *  -# RFAILED
*/
#ifdef ANSI
PUBLIC S16 nhCCMUlDedMsgHndlr 
(
NhuDatIndSdus       *nhuDatIndSdu,
NhCellAndUeCb       *cellAndUeCb,
U8                  msgTransId,
U8                  msgType
)
#else /* ANSI */
PUBLIC S16 nhCCMUlDedMsgHndlr(nhuDatIndSdu, cellAndUeCb, msgTransId, msgType)
NhuDatIndSdus       *nhuDatIndSdu;
NhCellAndUeCb       *cellAndUeCb;
U8                  msgTransId;
U8                  msgType;
#endif /* ANSI */
{
   S16                                 retVal;

   TRC2(nhCCMUlDedMsgHndlr)

   UNUSED(msgTransId);

   RLOG0(L_DEBUG, "nhCCMUlDedMsgHndlr called.");

#ifndef NH_NO_STS
   /* Update the Statistics */
   nhUTLUpdPduSts(cellAndUeCb->ueCb, msgType, NH_STS_CAUSE_SUCCESS);
#endif /* NH_NO_STS */

   /* Pass message to the RRC user */
   retVal = nhUIMDatInd(nhuDatIndSdu);
   if (ROK != retVal)
   {
      RLOG0(L_ERROR, "nhUIMDatInd failed.");
      RETVALUE(retVal);
   }

   RETVALUE(ROK);
} /* nhCCMUlDedMsgHndlr */


/**
 * @brief Handler for RRC Re-establishment  message.
 * 
 * @details 
 * 
 *
 * This function encodes and stores the RRC Re-establishment message. 
 * It calls funtion to update RLC and PDCP security configuration 
 * information received from user to be used to re-establish SRB1.
 * It calls funtion to update RLC and PDCP configuration to re-establish 
 * SRB1.
 * It updates the message statistics.
 * 
 * 
 * @param[in] datReqRspSdu   RRC Downlink messages
 * @param[in] cellAndUeCb    Cell and UE context
 * @param[in] msgTransId     Transaction identifier 
 * @param[out] errInfo       Error information, in case of error
 * @return S16
 *  -# ROK
 *  -# RFAILED
*/
#ifdef ANSI
PUBLIC S16 nhCCMConRestbHdlr 
(
NhDatReqRspSdu    *datReqRspSdu, 
NhCellAndUeCb     *cellAndUeCb,
U8                msgTransId,
NhuErrorInfo      *errInfo
)
#else /* ANSI */
PUBLIC S16 nhCCMConRestbHdlr(datReqRspSdu, cellAndUeCb, msgTransId, errInfo)
NhDatReqRspSdu    *datReqRspSdu;
NhCellAndUeCb     *cellAndUeCb;
U8                msgTransId;
NhuErrorInfo      *errInfo;
#endif /* ANSI */
{
   S16               retVal;
   TransCb           *transCb;
   U32               msgTransKey;
   MsgType           msgType;
   NhDatReqParam     datReqParam;
   NhMsgPduAndType   msgPduAndType;
   NhuDatRspSdus     *datRspSdu;
   NhActvUeCb        *ueCb = NULLP;
   NhCellCb          *cellCb;
   Bool              isUeCbCreated = FALSE;
   Bool              isTransCbCreated = FALSE;


   TRC2(nhCCMConRestbHdlr)

   RLOG0(L_DEBUG, "nhCCMConRestbHdlr called.");

   msgType = NH_MSGTYPE_CONNREESTAB;
   NH_SET_ZERO(&(datReqParam), sizeof(NhDatReqParam));

   datRspSdu = datReqRspSdu->sdu.datRspSdu;
   cellCb = cellAndUeCb->cellCb;

   /* Get the message transaction key */
   nhUTLGetRrcTransId((Void *) & (datRspSdu->sdu.m.dlCcchMsg), C1_RRCCONREESTB, 
                    NH_SDU_DL_CCCH, &msgTransId);
   NH_GET_TRANSCB_KEY(msgTransKey, msgType, msgTransId);

   NH_FILL_MSG_PDU_AND_TYPE(msgPduAndType, &datRspSdu->sdu.m.dlCcchMsg.ccchMsg, 
         NHU_MSG_DLCCCH);

   /* Fill RLC parameters required by PIM */
   NH_FORM_NH_ID_FRM_DATRSP(datReqParam.nhId, datRspSdu);
   datReqParam.isPdcpDatReq = FALSE;
   datReqParam.u.rlcParam.lcType = CM_LTE_LCH_CCCH;
   datReqParam.u.rlcParam.u.ccchInfo.rnti = 
                  datRspSdu->sdu.m.dlCcchMsg.tmpCrnti;
/* ccpu00142868 : eNB does not send RRC_CONN_RLS in case of RRC_REEST with cause other */
   /* Delete or clean all the existing transactions associated with this UE 
    * during re-establishment. 
    */
   nhDBMDelTransCbs(cellAndUeCb->ueCb);

   /* If both configuration and message is present, send configuration
    * first. Store the message in the buffer for transmision after 
    * successful configuration */
   if(TRUE == datRspSdu->sdu.isUeCfgPres)
   {
      /* Fetch UE context */
      ueCb = cellAndUeCb->ueCb;

      /* Create transaction block for the configuration */
      retVal = nhDBMAddTransCb(ueCb, &transCb, msgTransKey);

      if (ROK != retVal)
      {
         RLOG0(L_FATAL, "Unable to create transaction control block.");
         NH_CCM_DEL_UECB_TRANSCB(isTransCbCreated, &transCb, isUeCbCreated,
               cellCb, &ueCb);
         RETVALUE(retVal);
      }

      transCb->cause = msgType;
      transCb->nxTransId = datRspSdu->hdr.transId;

      /* This is used for deleting transaction context */
      isTransCbCreated = TRUE;

      /* Connection re-establishment request comes with 
       * a new tmp Crnti. Modify the old crnti with the new 
       * crnti*/
      /* UE ID modification required at lower layers */
      transCb->ordrdCfg.actionMask |= NH_ORD_CFG_UEID;
      transCb->ordrdCfg.ueInfo.crntiCfgType = NH_UE_INFO_MOD_CRNTI;
      transCb->ordrdCfg.ueInfo.u.modCrnti =
         datRspSdu->sdu.m.dlCcchMsg.tmpCrnti;

      /*Set ordered config action mask flags related to re-establishment 
        SRB1 and MAC reset.*/
      transCb->ordrdCfg.actionMask |= NH_ORD_CFG_RESTB_SRB1;
      transCb->ordrdCfg.actionMask |= NH_ORD_CFG_RESET_MAC;

      /* Encodes the message and stores it in transCb */
      retVal = nhUTLEncAndStoreMsg(&msgPduAndType, &datReqParam, 
                    transCb, errInfo);
      if (ROK != retVal)
      {
         RLOG0(L_ERROR, "nhUTLEncAndStoreMsg failed.");
         NH_CCM_DEL_UECB_TRANSCB(isTransCbCreated, &transCb, isUeCbCreated,
               cellCb, &ueCb);
         RETVALUE(retVal);
      }

      /* Stores the security configuation */
      retVal = nhCGMLowerLyrCfgReq(cellAndUeCb, &datRspSdu->sdu.ueCfg, 
            transCb, TRUE);
      if (ROK != retVal)
      {
         RLOG0(L_ERROR, "Cannot store lower layer config");
         /* Delete UE and Trans CB only if Lower Layer Config is not sent */
         if((!transCb->numOfMacCfms) && (!transCb->numOfRlcCfms) &&
               (!transCb->numOfPdcpCfms))
         {
            NH_CCM_DEL_UECB_TRANSCB(isTransCbCreated, &transCb, isUeCbCreated,
                                    cellCb, &ueCb);
         }

         NH_FILL_NHU_ERROR(*errInfo, NHU_ERRTYPE_UE_CFG, 
                              NHU_ERRCAUSE_TRANSCB_UPD_FAIL);

         RETVALUE(retVal);
      }
   } /* end if(datRspSdu->sdu.isUeCfgPres) */ 
   else
   {
      /*Check if RRC user has issued config requuest before sending the message*/
       retVal = nhUTLEncAndSnd(cellAndUeCb, 0,
                               &msgPduAndType, &datReqParam, errInfo);
       if(ROK != retVal)
       {
          RLOG0(L_ERROR, "nhCCMConRestbHdlr: nhUTLEncAndSnd Failed.");
          NH_CCM_DEL_UECB_TRANSCB(isTransCbCreated, &transCb, FALSE,
                NULLP, &(cellAndUeCb->ueCb));

          RETVALUE(retVal);
       }
   }

   RETVALUE(ROK);
} /* nhCCMConRestbHdlr */

/** @} */

/**********************************************************************
         End of file:     ni_ccm.c@@/main/3 - Fri Jul  1 01:13:13 2011

**********************************************************************/

/**********************************************************************
        Revision History:

**********************************************************************/
/********************************************************************90**

     ver       pat    init                  description
------------ -------- ---- ----------------------------------------------
/main/1      ---     rer              1. LTE-RRC Initial Release.
/main/2      ---     ch               1. RRC Release 2.1.
           nh003.201 vkulkarni        1. Order of config during RRC conn 
                                         reconfig is corrected.
/main/3      ---     sbalakrishna     1. Updated for Release of 3.1.
           nh004.201 vkulkarni 1. Corrected error handling for RRC 
                                  connection reconfiguration procedure.
*********************************************************************91*/
