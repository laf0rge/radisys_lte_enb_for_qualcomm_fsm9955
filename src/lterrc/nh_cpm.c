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
  
     Desc:     Code for RRC common procedure handling.

     File:     nh_cpm.c

     Sid:      ni_cpm.c@@/main/3 - Fri Jul  1 01:13:14 2011

     Prg:      rer 

**********************************************************************/
/** @defgroup cpm_group Common procedures module
 *  @{
 */
/* header include files (.h) */
static const char* RLOG_MODULE_NAME="RRC";
static int RLOG_MODULE_ID=512;
static int RLOG_FILE_ID=85;
#include "envopt.h"        /* Environment options */
#include "envdep.h"        /* Environment dependent */
#include "envind.h"        /* Environment independent */
#include "gen.h"           /* General layer */
#include "ssi.h"           /* System services */
#include "cm_hash.h"       /* Common hashlist */
#include "cm_tkns.h"       /* Common tokens */
#include "cm_llist.h"      /* Ccommon Link list */
#include "cm_lte.h"        /* common LTE structure */ 
#include "nhu.h"           /* RRC Upper interface */
#include "lnh.h"           /* LNH interface defines */
#include "nh.h"

/* external headers */

/* header/extern include files (.x) */
#include "gen.x"           /* General layer */
#include "ssi.x"           /* System services */
#include "cm5.x"           /* Timer services */
#include "cm_lib.x"        /* Common library */
#include "cm_hash.x"       /* Common hashlist */
#include "cm_tkns.x"       /* Common tokens */
#include "cm_llist.x"      /* Common Link list */
#include "cm_lte.x"        /* Common LTE structure */ 
#include "nhu.x"           /* RRC Upper interface */
#include "lnh.x"           /* LNH interface includes */
#include "nh.x"


EXTERN U32 NhBcchPcchTbSzTbl[NH_BCCH_PCCH_TBSZTBL_SIZE];


/**
 * @brief Handler for Downlink Information Transfer.
 * 
 * @details 
 * 
 *
 * This function encodes and sends the Downlink NAS message to PDCP
 * for transmission to the UE.
 *
 * @param[in] datReqRspSdu    This is a pointer data SDU
 * @param[in] cellAndUeCb     This contains both UE/Cell control block pointers
 * @param[in] msgTransId      This is a RRC Transaction ID To Identify a transaction
 * @param[out] errInfo        This is a used to fill any error info, if present
 *
 * @return S16
 *  -# ROK
 *  -# RFAILED
*/
#ifdef ANSI
PUBLIC S16 nhCPMHndlDlDirTx
(
NhDatReqRspSdu   *datReqRspSdu,
NhCellAndUeCb    *cellAndUeCb,
U8               msgTransId,
NhuErrorInfo     *errInfo
) 
#else /* ANSI */
PUBLIC S16 nhCPMHndlDlDirTx(datReqRspSdu, cellAndUeCb, msgTransId, errInfo)
NhDatReqRspSdu   *datReqRspSdu;
NhCellAndUeCb    *cellAndUeCb;
U8               msgTransId;
NhuErrorInfo     *errInfo;
#endif /* ANSI */
{
   NhDatReqParam  datReqParam;  
   S16            ret;          
   Buffer         *mBuf;        
   U32            length;       
   MuiInfo        *muiCb = NULLP;
   NhuDL_DCCH_Msg *dlDcchPdu = NULLP;
   NhuDatReqSdus  *nhDatReqSdu;
   U8             msgCategory;
   
   TRC2(nhCPMHndlDlDirTx);

   UNUSED(msgTransId);

   RLOG0(L_DEBUG, "In Downlink Info transfer Handler");
   
   nhDatReqSdu = datReqRspSdu->sdu.datReqSdu;

   NH_SET_ZERO(&(datReqParam), sizeof(NhDatReqParam));
   NH_FORM_NHID_FRM_DATREQ(datReqParam.nhId, nhDatReqSdu);

   /* Encode DL info transfer message and send it to UE */
   dlDcchPdu = &(nhDatReqSdu->sdu.ueMsg.msg.dedMsg.dlDcchMsg.m.dcchMsg);
   msgCategory = NHU_MSG_DLDCCH;

   ret = nhEDMEncReq((Void *)dlDcchPdu, msgCategory, &mBuf, &length);
   if (ROK != ret)
   {
     /* Send Err Ind to NHU */
      NH_FILL_NHU_ERROR(*errInfo, NHU_ERRTYPE_ENC_FAILED, \
                        NHU_ERRCAUSE_NONE);
      RETVALUE(RFAILED);
   }
    
   /* Add MUI info block to ueCb */
   if (ROK != nhDBMAddMuiCb(cellAndUeCb->ueCb, &muiCb, 
                            nhDatReqSdu->hdr.transId))
   {
      RLOG0(L_ERROR, "nhDBMAddMuiCb Failed");
      NH_FREEMBUF(mBuf);
      NH_FILL_NHU_ERROR(*errInfo, NHU_ERRTYPE_MSG_SEND_FAILED, \
                        NHU_ERRCAUSE_NONE);
      RETVALUE(RFAILED);
   }

   /* Fill DatReq parameters */
   datReqParam.isPdcpDatReq = TRUE;
   datReqParam.u.pdcpParam.mui = muiCb->mui;

   ret = nhPIMSendUeMsg(cellAndUeCb, &datReqParam, mBuf);
   if ( ROK != ret )
   {
      RLOG0(L_ERROR, "nhPIMSendUeMsg Failed");
      nhDBMDelMuiCb(cellAndUeCb->ueCb, &muiCb);
      NH_FILL_NHU_ERROR(*errInfo, NHU_ERRTYPE_MSG_SEND_FAILED, \
                        NHU_ERRCAUSE_NONE);
      RETVALUE(RFAILED);
   }
   
#ifndef NH_NO_STS   
   /* Update the statistics */
   nhUTLUpdPduSts(cellAndUeCb->ueCb, NH_MSGTYPE_DLTFR, NH_STS_CAUSE_SUCCESS);
#endif /* NH_NO_STS */

   RETVALUE(ROK);

}/* end of nhCPMHndlDlDirTx */

/**
 * @brief Handler for Paging Message.
 * 
 * @details 
 * 
 *
 * This function encodes and sends the Paging message to RLC for transmission
 * to UE.
 *
 * @param[in] datReqRspSdu    Data request/response sdu 
 * @param[in] cellAndUeCb     Container of Cell and UE control block
 * @param[in] msgTransId      RRC Message Transaction Identifier (not used here) 
 * @param[in] errInfo         Error information used for sending error indication
 *
 * @return S16
 *  -# ROK
 *  -# RFAILED
*/
#ifdef ANSI 
PUBLIC S16 nhCPMHndlPagingMsg
( 
NhDatReqRspSdu   *datReqRspSdu,
NhCellAndUeCb    *cellAndUeCb,
U8               msgTransId,
NhuErrorInfo     *errInfo
) 
#else /* ANSI */
PUBLIC S16 nhCPMHndlPagingMsg(datReqRspSdu, cellAndUeCb, msgTransId, errInfo)
NhDatReqRspSdu   *datReqRspSdu;
NhCellAndUeCb    *cellAndUeCb;
U8               msgTransId;
NhuErrorInfo     *errInfo;
#endif /* ANSI */
{
   NhDatReqParam  datReqParam;
   Void           *pdu;
   NhuCmnMsg      *cmnMsg;
   Buffer         *encBuf;
   U32            length;
   U32            msgOctets;
   S32            padOctets;
   S16            ret;
   S32            idx;
   U32            tbSize;

   TRC2(nhCPMHndlPagingMsg);

   UNUSED(msgTransId);

   ret = ROK;
   encBuf = NULLP;
   length = 0;
   NH_SET_ZERO(&(datReqParam), sizeof(NhDatReqParam));

   cmnMsg = &(datReqRspSdu->sdu.datReqSdu->sdu.ueMsg.msg.cmnMsg);

   /* Encode the paging message */
   pdu = (Void *) &(cmnMsg->cmn.pcchMsg);

   ret = nhEDMEncReq(pdu, cmnMsg->cmnMsgCategory, &encBuf, &length);
   if ((ROK != ret) || (NULLP == encBuf))
   {
      /* Handle encode error here */
      NH_FILL_NHU_ERROR(*errInfo, NHU_ERRTYPE_ENC_FAILED, NHU_ERRCAUSE_NONE);
      RETVALUE(ret);
   }

   /* Address padding requirement */
   msgOctets = (length/8) + ((length%8)==0? 0:1);
   /* nh001.301 : Correction made in the padding bytes 
    *             calculation for the TB size of PCCH SDU.
    */
   if (length > NhBcchPcchTbSzTbl[NH_BCCH_PCCH_TBSZTBL_SIZE - 1])
   {
      RLOG0(L_ERROR, "encoded length is more than max TB size");

      NH_FILL_NHU_ERROR(*errInfo, NHU_ERRTYPE_MSG_SEND_FAILED, 
                        NHU_ERRCAUSE_INVALID_TBS_SIZE);
      NH_FREEMBUF(encBuf);
      RETVALUE(RFAILED);
   }

   NH_GET_BCCH_PCCH_TBSZ((msgOctets * 8), tbSize);
   
   padOctets = (tbSize / 8) - msgOctets;

   for (idx=0; idx < padOctets; idx++)
   {
      SAddPstMsg(0, encBuf);
   }

   /* Fill data request parameters */
   NH_FORM_NHID_FRM_DATREQ(datReqParam.nhId, datReqRspSdu->sdu.datReqSdu);
   datReqParam.isPdcpDatReq = FALSE;
   datReqParam.u.rlcParam.lcType = CM_LTE_LCH_PCCH;
   NH_COPY_TMG_INFO(datReqParam.u.rlcParam.u.timingInfo,
                   cmnMsg->cmnMsgInfo.timingInfo);

   /* Send the Sys info for transmission through KWU (RLC) interface */
   ret = nhPIMSendUeMsg(cellAndUeCb, &datReqParam, encBuf);
   if (ROK != ret)
   {
      /* handle the send error */
      RLOG1(L_ERROR, "Send to lower layer failed ret[%d]", ret);
      NH_FILL_NHU_ERROR(*errInfo, NHU_ERRTYPE_MSG_SEND_FAILED, \
                        NHU_ERRCAUSE_FAILED_SENDTORLC);
      RETVALUE(ret);
   }

#ifndef NH_NO_STS   
   /* Update the Statistics */
   nhUTLUpdCellPduSts(cellAndUeCb->cellCb, NH_MSGTYPE_PAG, 
                        NH_STS_CAUSE_SUCCESS);
#endif /* NH_NO_STS */

   RETVALUE(ROK);
} /* nhCPMHndlPagingMsg */

/**
 * @brief Handler for RRC UE Capability Enquiry message.
 * 
 * @details 
 * 
 * This function receives encodes UE Capability message and stores it. It 
 * configures the lower layers if it receives configuration information. Then 
 * it checks whether UE is in RRC_CONNECTED state apart from validating other 
 * information provided to it. If not, sends error indication to RRC user and 
 * updates statistics. If in RRC_CONNECTED state, this function sends stored 
 * UE Capability Enquiry Message to PDCP using LIM and it updates the
 * message statistics.
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
PUBLIC S16 nhCPMUeCapEnqHndlr
(
NhDatReqRspSdu *datReqRspSdu,
NhCellAndUeCb  *cellAndUeCb,
U8             msgTransId,
NhuErrorInfo   *errInfo
)
#else /* ANSI */
PUBLIC S16 nhCPMUeCapEnqHndlr(datReqRspSdu, cellAndUeCb, msgTransId, errInfo)
NhDatReqRspSdu *datReqRspSdu;
NhCellAndUeCb  *cellAndUeCb;
U8             msgTransId;
NhuErrorInfo   *errInfo;
#endif /* ANSI */
{
   S16               retVal;
   MsgType           msgType;
   NhuDatReqSdus     *datReqSdu;
   NhMsgPduAndType   msgPduAndType;
   NhDatReqParam     datReqParam;

   TRC2(nhCPMUeCapEnqHndlr);

   RLOG0(L_DEBUG, "nhCPMUeCapEnqHndlr called.");

   NH_SET_ZERO(&datReqParam, sizeof(NhDatReqParam));
   datReqSdu = datReqRspSdu->sdu.datReqSdu;

   /* SRB1 must be configured before sending this message */
   if(!(cellAndUeCb->ueCb->rbSecCfgBitMask & NH_SRB1_CFGD))
   {
      RLOG0(L_ERROR, "SRB1 is not configured.");
      NH_FILL_NHU_ERROR(*errInfo, NHU_ERRTYPE_UE_CFG, 
                                  NHU_ERRCAUSE_SRB1_NOT_CONFIGURED);
      RETVALUE(RFAILED);
   }

   msgType = NH_MSGTYPE_UECAPENQ;

   NH_FILL_MSG_PDU_AND_TYPE(msgPduAndType,
         &datReqSdu->sdu.ueMsg.msg.dedMsg.dlDcchMsg.m.dcchMsg, NHU_MSG_DLDCCH);

   /* Fill PDCP parameters required by PIM */
   NH_FORM_NHID_FRM_DATREQ(datReqParam.nhId, datReqSdu);
   datReqParam.isPdcpDatReq = TRUE;

   /* Encodes and sends message to UE */
   retVal = nhUTLEncAndSnd(cellAndUeCb, datReqSdu->hdr.transId,
         &msgPduAndType, &datReqParam, errInfo);
   if(ROK != retVal)
   {
      RLOG0(L_ERROR, "nhUTLEncAndSnd Failed.");
      RETVALUE(retVal);
   }


#ifndef NH_NO_STS
   /* Update the Statistics */
   nhUTLUpdPduSts(cellAndUeCb->ueCb, msgType, NH_STS_CAUSE_SUCCESS);
#endif /* NH_NO_STS */

   RETVALUE(ROK);
} /* nhCPMUeCapEnqHndlr */

/**
 * @brief Handler for Mobility from E-UTRA Command.
 * 
 * @details 
 * 
 *
 * This function encodes and sends Mobility from E-UTRA Command message to 
 * PDCP for transmission to UE. Also updates pdu statistics.
 *
 * @param[in] datReqRspSdu    Data request SDU 
 * @param[in] cellAndUeCb     Cell and UE control block
 * @param[in] msgTransId      RRC Message Transaction Identifier (not used here) 
 * @param[in] errInfo         Error information used for sending error indication
 *
 * @return S16
 *  -# ROK
 *  -# RFAILED
*/
#ifdef ANSI
PUBLIC S16 nhCPMMobFrmEutraCmd
(
NhDatReqRspSdu   *datReqRspSdu,
NhCellAndUeCb    *cellAndUeCb,
U8               msgTransId,
NhuErrorInfo     *errInfo
)
#else /* ANSI */
PUBLIC S16 nhCPMMobFrmEutraCmd(datReqRspSdu, cellAndUeCb, msgTransId, errInfo)
NhDatReqRspSdu   *datReqRspSdu;
NhCellAndUeCb    *cellAndUeCb;
U8               msgTransId;
NhuErrorInfo     *errInfo;
#endif /* ANSI */
{
   S16                retVal;
   MsgType            msgType;
   NhDatReqParam      datReqParam;
   NhMsgPduAndType    msgPduAndType;
   NhuDatReqSdus      *datReqSdu;

   TRC2(nhCPMMobFrmEutraCmd)

   RLOG0(L_DEBUG, "nhCPMMobFrmEutraCmd called.");

   NH_SET_ZERO(&(datReqParam), sizeof(NhDatReqParam));
   datReqSdu = datReqRspSdu->sdu.datReqSdu;

   /* SRB1 must be configured before sending this message */
   if(!(cellAndUeCb->ueCb->rbSecCfgBitMask & NH_SRB1_CFGD))
   {
      RLOG0(L_ERROR, "SRB1 is not configured.");
      NH_FILL_NHU_ERROR(*errInfo, NHU_ERRTYPE_UE_CFG, 
                                  NHU_ERRCAUSE_SRB1_NOT_CONFIGURED);
      RETVALUE(RFAILED);
   }

   /* Security procedure must be completed before sending 
    * this message */
   if(!(cellAndUeCb->ueCb->rbSecCfgBitMask & NH_SEC_CMPLTD))
   {
      RLOG0(L_ERROR, "Security procedure is not initiated.");
      NH_FILL_NHU_ERROR(*errInfo, NHU_ERRTYPE_UE_CFG, 
                                  NHU_ERRCAUSE_SEC_NOT_CONFIGURED);
      RETVALUE(RFAILED);
   }

#ifdef LTERRC_REL9
   /* R9 Upgrade:NH_NW_90: MobilityFromEutra procedure is initiated only when 
    * AS-security has been activated, and SRB2 with at least one DRB are setup
    * and not suspended.
    * SRB2 must be established before sending this message */
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
#endif /*LTERRC_REL9*/

   msgType = NH_MSGTYPE_MOBFRMEUTRAN;

   /* Fill PDCP Parameters required by PIM */
   NH_FORM_NHID_FRM_DATREQ(datReqParam.nhId, datReqSdu);
   datReqParam.isPdcpDatReq = TRUE;

   NH_FILL_MSG_PDU_AND_TYPE(msgPduAndType, 
         &datReqSdu->sdu.ueMsg.msg.dedMsg.dlDcchMsg.m.dcchMsg, NHU_MSG_DLDCCH);

   /* Encodes and sends message to UE */
   retVal = nhUTLEncAndSnd(cellAndUeCb, datReqSdu->hdr.transId, 
                           &msgPduAndType, &datReqParam, errInfo);
   if(ROK != retVal)
   {
      RLOG0(L_ERROR, "nhUTLEncAndSnd Failed.");
      RETVALUE(retVal);
   }

   cellAndUeCb->ueCb->hoToEutraOrMobFrmEutra = NH_MSGTYPE_MOBFRMEUTRAN;

#ifndef NH_NO_STS
   /* Update the Statistics */
   nhUTLUpdPduSts(cellAndUeCb->ueCb, msgType, NH_STS_CAUSE_SUCCESS);
#endif /* NH_NO_STS */

   RETVALUE(ROK);
} /* nhCPMMobFrmEutraCmd */

/**
 * @brief Handler for Handover from E-UTRA Preparation Transfer.
 * 
 * @details 
 * 
 *
 * This function encodes and sends Handover from E-UTRA Preparation Transfer 
 * message to PDCP for transmission to UE.Also updates pdu statistics.
 
 * @param[in] datReqRspSdu    Data request SDU 
 * @param[in] cellAndUeCb     Cell and UE control block
 * @param[in] msgTransId      RRC Message Transaction Identifier (not used here) 
 * @param[in] errInfo         Error information used for sending error indication
 *
 * @return S16
 *  -# ROK
 *  -# RFAILED
*/
#ifdef ANSI
PUBLIC S16 nhCPMHoFrmEutraPrepTfr
(
NhDatReqRspSdu   *datReqRspSdu,
NhCellAndUeCb    *cellAndUeCb,
U8               msgTransId,
NhuErrorInfo     *errInfo
)
#else /* ANSI */
PUBLIC S16 nhCPMHoFrmEutraPrepTfr(datReqRspSdu, cellAndUeCb, msgTransId, 
                                 errInfo)
NhDatReqRspSdu   *datReqRspSdu;
NhCellAndUeCb    *cellAndUeCb;
U8               msgTransId;
NhuErrorInfo     *errInfo;
#endif /* ANSI */
{
   S16                retVal;
   MsgType            msgType;
   NhDatReqParam      datReqParam;
   NhMsgPduAndType    msgPduAndType;
   NhuDatReqSdus      *datReqSdu;

   TRC2(nhCPMHoFrmEutraPrepTfr)

   RLOG0(L_DEBUG, "nhCPMHoFrmEutraPrepTfr called.");

   NH_SET_ZERO(&(datReqParam), sizeof(NhDatReqParam));
   datReqSdu = datReqRspSdu->sdu.datReqSdu;

   /* SRB1 must be configured before sending this message */
   if(!(cellAndUeCb->ueCb->rbSecCfgBitMask & NH_SRB1_CFGD))
   {
      RLOG0(L_ERROR, "SRB1 is not configured");
      NH_FILL_NHU_ERROR(*errInfo, NHU_ERRTYPE_UE_CFG, 
                                  NHU_ERRCAUSE_SRB1_NOT_CONFIGURED);
      RETVALUE(RFAILED);
   }

   /* Security procedure must be completed before sending 
    * this message */
   if(!(cellAndUeCb->ueCb->rbSecCfgBitMask & NH_SEC_CMPLTD))
   {
      RLOG0(L_ERROR, "Security procedure is not initiated.");
      NH_FILL_NHU_ERROR(*errInfo, NHU_ERRTYPE_UE_CFG, 
                                  NHU_ERRCAUSE_SEC_NOT_CONFIGURED);
      RETVALUE(RFAILED);
   }

   msgType = NH_MSGTYPE_HOFRMEUTRAPREPREQ;

   /* Fill PDCP Parameters required by PIM */
   NH_FORM_NHID_FRM_DATREQ(datReqParam.nhId, datReqSdu);
   datReqParam.isPdcpDatReq = TRUE;

   NH_FILL_MSG_PDU_AND_TYPE(msgPduAndType, 
         &datReqSdu->sdu.ueMsg.msg.dedMsg.dlDcchMsg.m.dcchMsg, NHU_MSG_DLDCCH);

   /* Encodes and sends message to UE */
   retVal = nhUTLEncAndSnd(cellAndUeCb, datReqSdu->hdr.transId, 
                           &msgPduAndType, &datReqParam, errInfo);
   if(ROK != retVal)
   {
      RLOG0(L_ERROR, "nhUTLEncAndSnd Failed.");
      RETVALUE(retVal);
   }

#ifndef NH_NO_STS
   /* Update the Statistics */
   nhUTLUpdPduSts(cellAndUeCb->ueCb, msgType, NH_STS_CAUSE_SUCCESS);
#endif /* NH_NO_STS */

   RETVALUE(ROK);
} /* nhCPMHoFrmEutraPrepTfr */

/**
 * @brief Handler for CSFB Parameters Response CDMA2000 message.
 * 
 * @details 
 * 
 *
 * This function encodes and sends CSFB Parameters Response CDMA2000 message 
 * to PDCP for transmission to UE.Also updates pdu statistics.
 
 * @param[in] datReqRspSdu    Data response SDU 
 * @param[in] cellAndUeCb     Cell and UE control block
 * @param[in] msgTransId      RRC Message Transaction Identifier (not used here) 
 * @param[in] errInfo         Error information used for sending error indication
 *
 * @return S16
 *  -# ROK
 *  -# RFAILED
*/
#ifdef ANSI
PUBLIC S16 nhCPMCDMACsfbParamRsp
(
NhDatReqRspSdu   *datReqRspSdu,
NhCellAndUeCb    *cellAndUeCb,
U8               msgTransId,
NhuErrorInfo     *errInfo
)
#else /* ANSI */
PUBLIC S16 nhCPMCDMACsfbParamRsp(datReqRspSdu, cellAndUeCb, msgTransId, errInfo)
NhDatReqRspSdu   *datReqRspSdu;
NhCellAndUeCb    *cellAndUeCb;
U8               msgTransId;
NhuErrorInfo     *errInfo;
#endif /* ANSI */
{
   S16                retVal;
   MsgType            msgType;
   NhDatReqParam      datReqParam;
   NhMsgPduAndType    msgPduAndType;
   NhuDatRspSdus      *datRspSdu;

   TRC2(nhCPMCDMACsfbParamRsp)

   RLOG0(L_DEBUG, "nhCPMCDMACsfbParamRsp called.");

   NH_SET_ZERO(&(datReqParam), sizeof(NhDatReqParam));
   datRspSdu = datReqRspSdu->sdu.datRspSdu;

   /* SRB1 must be configured before sending this message */
   if(!(cellAndUeCb->ueCb->rbSecCfgBitMask & NH_SRB1_CFGD))
   {
      RLOG0(L_ERROR, "SRB1 is not configured.");
      NH_FILL_NHU_ERROR(*errInfo, NHU_ERRTYPE_UE_CFG, 
                                  NHU_ERRCAUSE_SRB1_NOT_CONFIGURED);
      RETVALUE(RFAILED);
   }
   
   msgType = NH_MSGTYPE_CSFBPARAMRSPCDMA;

   /* Fill PDCP Parameters required by PIM */
   NH_FORM_NH_ID_FRM_DCCH_DATRSP(datReqParam.nhId, datRspSdu);
   datReqParam.isPdcpDatReq = TRUE;

   NH_FILL_MSG_PDU_AND_TYPE(msgPduAndType, 
         &datRspSdu->sdu.m.dlDcchMsg.m.dcchMsg, NHU_MSG_DLDCCH);

   /* Encodes and sends message to UE */
   retVal = nhUTLEncAndSnd(cellAndUeCb, datRspSdu->hdr.transId, 
                           &msgPduAndType, &datReqParam, errInfo);
   if(ROK != retVal)
   {
      RLOG0(L_ERROR, "nhUTLEncAndSnd Failed.");
      RETVALUE(retVal);
   }

#ifndef NH_NO_STS
   /* Update the Statistics */
   nhUTLUpdPduSts(cellAndUeCb->ueCb, msgType, NH_STS_CAUSE_SUCCESS);
#endif /* NH_NO_STS */

   RETVALUE(ROK);
} /* nhCPMCDMACsfbParamRsp */


#ifdef LTERRC_REL9 
/**
 * @brief Handler for Downlink UE information request.
 * 
 * @details 
 * 
 *
 * This function encodes and sends the Downlink UE info req message to PDCP
 * for transmission to the UE.
 *
 * @param[in] datReqRspSdu    This is a pointer data SDU
 * @param[in] cellAndUeCb     This contains both UE/Cell control block pointers
 * @param[in] msgTransId      This is a RRC Transaction ID To Identify a transaction
 * @param[out] errInfo        This is a used to fill any error info, if present
 *
 * @return S16
 *  -# ROK
 *  -# RFAILED
*/
#ifdef ANSI
PUBLIC S16 nhCPMUEInfoReqHndlr
(
NhDatReqRspSdu   *datReqRspSdu,
NhCellAndUeCb    *cellAndUeCb,
U8               msgTransId,
NhuErrorInfo     *errInfo
) 
#else /* ANSI */
PUBLIC S16 nhCPMUEInfoReqHndlr (datReqRspSdu, cellAndUeCb, msgTransId, errInfo)
NhDatReqRspSdu   *datReqRspSdu;
NhCellAndUeCb    *cellAndUeCb;
U8               msgTransId;
NhuErrorInfo     *errInfo;
#endif /* ANSI */
{
   NhDatReqParam  datReqParam;  
   S16            ret;          
   Buffer         *mBuf;        
   U32            length;       
   MuiInfo        *muiCb = NULLP;
   NhuDL_DCCH_Msg *dlDcchPdu = NULLP;
   NhuDatReqSdus  *nhDatReqSdu;
   U8             msgCategory;
   
   TRC2(nhCPMUEInfoReqHndlr);

   UNUSED(msgTransId);

   RLOG0(L_DEBUG, "UE Information Request Handler ");
   
   /* SRB1 must be configured before sending this message */
   if(!(cellAndUeCb->ueCb->rbSecCfgBitMask & NH_SRB1_CFGD))
   {
      RLOG0(L_ERROR, "SRB1 is not configured.");
      NH_FILL_NHU_ERROR(*errInfo, NHU_ERRTYPE_UE_CFG, 
                                  NHU_ERRCAUSE_SRB1_NOT_CONFIGURED);
      RETVALUE(RFAILED);
   }

   /* Security procedure must be completed before sending 
    * this message */
   if(!(cellAndUeCb->ueCb->rbSecCfgBitMask & NH_SEC_CMPLTD))
   {
      RLOG0(L_ERROR, "Security procedure is not initiated.");
      NH_FILL_NHU_ERROR(*errInfo, NHU_ERRTYPE_UE_CFG, 
                                  NHU_ERRCAUSE_SEC_NOT_CONFIGURED);
      RETVALUE(RFAILED);
   }

   nhDatReqSdu = datReqRspSdu->sdu.datReqSdu;

   NH_SET_ZERO(&(datReqParam), sizeof(NhDatReqParam));
   NH_FORM_NHID_FRM_DATREQ(datReqParam.nhId, nhDatReqSdu);

   /* Encode UE info req message and send it to UE */
   dlDcchPdu = &(nhDatReqSdu->sdu.ueMsg.msg.dedMsg.dlDcchMsg.m.dcchMsg);
   msgCategory = NHU_MSG_DLDCCH;

   ret = nhEDMEncReq((Void *)dlDcchPdu, msgCategory, &mBuf, &length);
   if (ROK != ret)
   {
     /* Send Err Ind to NHU */
      NH_FILL_NHU_ERROR(*errInfo, NHU_ERRTYPE_ENC_FAILED, \
                        NHU_ERRCAUSE_NONE);
      RETVALUE(RFAILED);
   }
    
   /* Add MUI info block to ueCb */
   if (ROK != nhDBMAddMuiCb(cellAndUeCb->ueCb, &muiCb, 
                            nhDatReqSdu->hdr.transId))
   {
      RLOG0(L_ERROR, "nhDBMAddMuiCb Failed ");
      NH_FREEMBUF(mBuf);
      NH_FILL_NHU_ERROR(*errInfo, NHU_ERRTYPE_MSG_SEND_FAILED, \
                        NHU_ERRCAUSE_NONE);
      RETVALUE(RFAILED);
   }

   /* Fill DatReq parameters */
   datReqParam.isPdcpDatReq = TRUE;
   datReqParam.u.pdcpParam.mui = muiCb->mui;

   ret = nhPIMSendUeMsg(cellAndUeCb, &datReqParam, mBuf);
   if ( ROK != ret )
   {
      RLOG0(L_ERROR, "nhPIMSendUeMsg Failed");
      nhDBMDelMuiCb(cellAndUeCb->ueCb, &muiCb);
      NH_FILL_NHU_ERROR(*errInfo, NHU_ERRTYPE_MSG_SEND_FAILED, \
                        NHU_ERRCAUSE_NONE);
      RETVALUE(RFAILED);
   }
   
#ifndef NH_NO_STS   
   /* Update the statistics */
   nhUTLUpdPduSts(cellAndUeCb->ueCb, NH_MSGTYPE_UEINFOREQ, 
                   NH_STS_CAUSE_SUCCESS);
#endif /* NH_NO_STS */

   RETVALUE(ROK);

}/* end of nhCPMUEInfoReqHndlr */
#endif /* LTERRC_REL9*/



/** @} */
/**********************************************************************
  
         End of file:     ni_cpm.c@@/main/3 - Fri Jul  1 01:13:14 2011
   
**********************************************************************/

/**********************************************************************
        Revision History:

**********************************************************************/
/********************************************************************90**
 
     ver       pat    init                  description
------------ -------- ---- ----------------------------------------------
/main/1      ---      rer              1. LTE-RRC Initial Release.
/main/2      ---      ch               1. RRC Release 2.1.
/main/3      ---      sbalakrishna     1. Updated for Release of 3.1.
           nh001.301  hsingh           1. Correction made in the padding 
                                          bytes calculation for the TB size 
                                          of PCCH SDU.
/main/4   nh002.301   njha	       1. added NHU_VER2
*********************************************************************91*/
