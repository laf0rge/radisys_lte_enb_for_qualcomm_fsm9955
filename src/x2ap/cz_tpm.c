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

     Name:     LTE-X2AP Layer 
  
     Type:     C Source file
  
     Desc:     This file communicates with the EDM module, handles the lower
               interface procedures.

     File:     cz_tpm.c

     Sid:      tr_tpm.c@@/main/2 - Tue Aug 30 18:36:15 2011

     Prg:       

**********************************************************************/
/* Header include files (.h) */
#include "envopt.h"        /* Environment options */
#include "envdep.h"        /* Environment dependent */
#include "envind.h"        /* Environment independent */

#include "gen.h"           /* General */
#include "ssi.h"           /* System services */
#include "cm5.h"           /* Common timer library            */
#include "cm_llist.h"      /* Common linked list library      */
#include "cm_hash.h"       /* Common hash library             */
#include "cm_tpt.h"        /* Common transport library        */
#include "cm_tkns.h"       /* Common tokens                   */
#include "cm_pasn.h"       /* common per asn.1                */
#include "cm_mblk.h"       /* Common memory allocation        */
#include "cm_inet.h"       /* Common socket library           */
#include "sct.h"           /* SCT interface defines           */
#include "czt.h"           /* CZT interface defines           */
#include "lcz.h"           /* X2AP Layer management           */
#include "cz.h"            /* X2AP layer defines              */
#include "cz_err.h"        /* X2AP - error */

/* header/extern include files (.x) */
#include "gen.x"           /* General layer                   */
#include "ssi.x"           /* System services interface       */
#include "cm5.x"           /* Common timer module             */
#include "cm_lib.x"        /* Common library function         */
#include "cm_llist.x"      /* Common link list library        */
#include "cm_hash.x"       /* Common hash list library        */
#include "cm_tkns.x"       /* Common tokens                   */
#include "cm_tpt.x"        /* Common transport library        */
#include "cm_mblk.x"       /* Common memory allocation        */
#include "cm_inet.x"       /* Common socket library           */
#include "cm_pasn.x"       /* common per asn.1                */
#include "sct.x"           /* SCT interface defines           */
#include "czt.x"           /* CZT interface structures        */
#include "lcz.x"           /* X2AP Layer management           */
#include "cz.x"            /* X2AP layer structures           */
#ifdef SS_DIAG
/* cz001.301:Inclusion for DIAGNOSTIC macro*/
#include "ss_diag.h"        /* Common log file */
#endif
/** @defgroup tpmmodule X2AP Transport Module
 *  @{
 */
/* local defines */

/* local typedefs */
 
/* local externs */
 
/* forward references */


/**
  @brief This function receives the X2AP message from SCTP.
 
  @details This function is used to process the PDU recieved from the peer
   through lower interface i.e SCTP. 
  
  @param[in] mBuf     Buffer to be decoded.
  @param[in] peerCb   Peer from which message is received.
                             
 
  @return
       - ROK           - If the Procedure was successful.
       - RFAILED       - If the procedure was unsuccessful due to any failure.
*/
#ifdef ANSI
PUBLIC S16 czTpmRcvFromLower 
(
Buffer    *mBuf,      
CzPeerCb  *peerCb     
)
#else
PUBLIC S16 czTpmRcvFromLower(mBuf,peerCb)
Buffer    *mBuf;        
CzPeerCb  *peerCb;      
#endif
{
   S16           ret = ROK;
   S16           retVal = ROK;
   CztX2AP_PDU   *pdu = NULLP;
   CztEvnt       *uiEvnt = NULLP;
   CztEvnt       *rspEvnt = NULLP;
   Mem           sMem;
   /* cz001.301:kworks fix - Initialise msgType */
   U8            msgType = CZT_MSG_UNKNOWN;
   S16            fsmType;
   CmPAsnErr     pasnErr;
   Bool          isResponse;
   CzSctSapCb  *sctSap   = NULLP;

   CZ_TRC2(czTpmRcvFromLower);
   CZ_PRNT_PEERID;

   CZ_SET_ZERO(&pasnErr, sizeof(CmPAsnErr));

   sctSap = peerCb->sctSapCb;
   /* If trace flag is enabled send the trace indication */
   /* Give a trace indication if either of flags in sctSap and peerCb
    * are enabled. */
   if (sctSap->trcLen != 0 || peerCb->peerCfg.trcLen != 0)
   {
      CzTrc         trcParam;
      CZ_SET_ZERO(&trcParam,sizeof(CzTrc));

      /* Populate the trace params */
      trcParam.event = SCT_EVTDATIND;

      CZ_MEM_COPY(&trcParam.srcAddr.ipAddr, 
            &peerCb->sctSapCb->endpCb.srcAddrLst, 
            sizeof(SctNetAddrLst));

      trcParam.srcAddr.sctPort = peerCb->sctSapCb->endpCb.port; 

      CZ_MEM_COPY(&trcParam.dstAddr.ipAddr, 
            &peerCb->assocCb->assocCfg->dstAddrLst, 
            sizeof(SctNetAddrLst));

      trcParam.sapId = peerCb->sctSapCb->spId;
      trcParam.peerId = peerCb->peerId;

      CZ_DBG_INFO((CZ_PRNT_BUF,
               "Message Received sending Trace to user\n"));
      if (sctSap->trcLen != 0 )
      {
         czLmmSendLmTrc(sctSap->trcLen, &trcParam, mBuf);
      }
      else
      {
         czLmmSendLmTrc(peerCb->peerCfg.trcLen, &trcParam, mBuf);
      }
   }
   sMem.region = peerCb->sctSapCb->pst.region;
   sMem.pool   = peerCb->sctSapCb->pst.pool;
      
   CZ_DBG_INFO((CZ_PRNT_BUF,
            "Decoding the incoming message...\n"));
#ifdef SS_DIAG
   /* cz001.301: Change for Diagnostics */
   CZ_DIAG_MSG(mBuf, "Recieved X2AP Message :");
#endif
   /* Decode the message */
   if ((retVal = czEdmDecode(&uiEvnt, &sMem, mBuf, &pasnErr)) != ROK)
   {
      if(uiEvnt == NULLP)
      {
         CZ_DBG_ERROR((CZ_PRNT_BUF, "UI event is NULL\n"));
         CZ_RETVALUE(RFAILED);
      }
      CZ_DBG_FATAL((CZ_PRNT_BUF, "Decoding Failed\n"));
      /* Update the decode error sts */
      czCb.genSts.nmbDecErr++;
      pdu = &(uiEvnt->pdu);
      rspEvnt = czUtlSndRspMsg(pdu, &pasnErr);
      if(rspEvnt != NULLP)
      {
         CZ_DBG_ERROR((CZ_PRNT_BUF, "Sending a Response message for message \
                  that failed decode.\n"));
         czTpmSndToLower(peerCb, &(rspEvnt->pdu), peerCb->peerCfg.globalStreamId);
         CZ_FREEEVNT(rspEvnt);
      }
      if(pasnErr.errCode == CM_PASN_MAND_MIS)
      {
         CZ_DBG_ERROR((CZ_PRNT_BUF, "Mandatory Param missing in the incoming \
                  message.\n"));
         CZ_SEND_LCL_GEN_ERR(peerCb, CZT_ERRTYPE_DEC_FAILED,
               CZT_ERRCAUSE_MAND_PARAM_MISSING, msgType );
      }
      else
      {
         CZ_DBG_ERROR((CZ_PRNT_BUF, "Syntax Error in the incoming \
                  message.\n"));
         CZ_SEND_LCL_GEN_ERR(peerCb, CZT_ERRTYPE_DEC_FAILED,
               CZT_ERRCAUSE_SYNTAX_ERR, msgType );
      }
      /* cz001.301: Change for Diagnostics */
#ifdef SS_DIAG
      CZ_DIAG_LVL0(0x04100011, CZ_DIAG_PEERID, peerCb->peerId,
            "ASN Decoding failed, AsnErrorCode = %d, msgType = %d",
            pasnErr.errCode, msgType, 0, 0);
#endif
      CZ_FREEEVNT(uiEvnt);
      CZ_RETVALUE(RFAILED);
   } /* decoding of msg fails */
   uiEvnt->peerId = peerCb->peerId;   
   
   /* Retrieve the X2AP Message type */
   msgType = czUtlGetMsgType(&uiEvnt->pdu, &isResponse, &fsmType);
   /* cz001.301:Change for Diagnostics*/
#ifdef SS_DIAG
   CZ_DIAG_LVL0(0x04100001, CZ_DIAG_PEERID, peerCb->peerId,
         "Received DatInd from SCTP, event  = %d",
         msgType, 0, 0, 0)
#endif
   if (msgType == CZT_MSG_UNKNOWN)
   {
      CZ_DBG_ERROR((CZ_PRNT_BUF,
               "Unknown Message Received\n"));
      /* TODO: Update unknown message statistics */
      CZ_FREEEVNT(uiEvnt);
      CZ_RETVALUE(RFAILED);
   }
   if(msgType == CZT_MSG_ERR_IND)
   {
      CZ_DBG_INFO((CZ_PRNT_BUF,
               "X2-Error Indication Received\n"));
      ret = czNmmHdlErrInd(peerCb, uiEvnt, CZ_DIR_IN);
      CZ_RETVALUE(ret);
   }
   /* Validate whether this is GPM Message or BMP Message */
   if(fsmType == CZ_GP)
   {
      CZ_DBG_INFO((CZ_PRNT_BUF,
               "X2-Global Procedure message Received\n"));
      ret = czGpmFsmMsgHandler(peerCb, uiEvnt, msgType, CZ_DIR_IN);  
   }
   else if(fsmType == CZ_BMP)
   {
      CZ_DBG_INFO((CZ_PRNT_BUF,
               "X2-Basic Mobility Procedure message Received\n"));
      ret = czBmpHdlHoEvnt(peerCb, uiEvnt, msgType, CZ_DIR_IN);
   }
   else
   {
      CZ_DBG_ERROR((CZ_PRNT_BUF, "Invalid message type received\n"));
      CZ_FREEEVNT(uiEvnt);
      CZ_RETVALUE(RFAILED);
   }  
      
   CZ_RETVALUE(ret);

} /* end of czTpmRcvFromLower */


/**
  @brief This function sends the encoded X2AP setup request buffer.
 
  @details This function is used to send the X2AP setup request message to
   peer after a successful association. The encoded buffer is sent out.
  
  @param[in] peerCb    Peer to which to send the message.
  @param[in] mBuf      X2AP setup Request encoded buffer.
                             
 
  @return
       - ROK           - If the Procedure was successful.
       - RFAILED       - If the procedure was unsuccessful due to any failure.
*/
#ifdef ANSI
PUBLIC S16 czTpmSndStoredBuf
(
CzPeerCb       *peerCb,  
Buffer         *mBuf
)
#else
PUBLIC S16 czTpmSndStoredBuf (peerCb, mBuf) 
CzPeerCb       *peerCb;     
Buffer         *mBuf;
#endif
{
   CzSctSapCb  *sctSap   = NULLP;
   CztLclErrInd errInd;
   Buffer       *tmpBuf = NULLP;
   Mem          mem;

   CZ_TRC2(czTpmSndStoredBuf);
   CZ_PRNT_PEERID;

   cmMemset((U8*)&errInd, 0, sizeof(CztLclErrInd));
   CZ_SET_ZERO(&mem, sizeof(Mem));
    
   /* Retrieve the sctSap control block and check for NULLP */
   sctSap = peerCb->sctSapCb;

   if (sctSap == (CzSctSapCb *)NULLP)
   {
      CZ_DBG_ERROR((CZ_PRNT_BUF, "SAP is NULL\n"));
      CZ_RETVALUE(RFAILED);
   } /* end of if sctCb is NULL */


   /* If endpoint is not in open state  or assoc state isnot connected
    * free the pdu and return error 
    */
   if ((sctSap->state != LCZ_SAP_ISBOUND) || 
       (sctSap->endpCb.state != CZ_ENDP_OPEN) || 
       (peerCb->assocCb->assocState != CZ_ASSOC_UP))
   {
      CZ_DBG_ERROR((CZ_PRNT_BUF, "SAP is not bound. OR Association is \
               not connected.\n"));
      CZ_RETVALUE(RFAILED);
   } 

   /* If trace flag is enabled send the trace indication */
   /* If trace flag is enabled send the trace indication */
   /* Give a trace indication if either of flags in sctSap and peerCb
    * are enabled. */
   if (sctSap->trcLen != 0 || peerCb->peerCfg.trcLen != 0)
   {
      CzTrc        trcParam;
      cmMemset((U8*)&trcParam, 0, sizeof(CzTrc));
      /* Populate the trace params */
      trcParam.event = SCT_EVTDATREQ;
      CZ_MEM_COPY((&trcParam.srcAddr.ipAddr), (&sctSap->endpCb.srcAddrLst),
                   sizeof(SctNetAddrLst));
  
      trcParam.srcAddr.sctPort = sctSap->endpCb.port; 
      CZ_MEM_COPY((&trcParam.dstAddr.ipAddr), 
                  (&peerCb->assocCb->assocCfg->dstAddrLst),
                  sizeof(SctNetAddrLst));

      trcParam.sapId = sctSap->spId;
      trcParam.peerId = peerCb->peerId;

      CZ_DBG_INFO((CZ_PRNT_BUF,
               "Message being sent, sending Trace to user\n"));
      if (sctSap->trcLen != 0 )
      {
         czLmmSendLmTrc(sctSap->trcLen, &trcParam, mBuf);
      }
      else
      {
         czLmmSendLmTrc(peerCb->peerCfg.trcLen, &trcParam, mBuf);
      }
   }
   mem.region = sctSap->pst.region;
   mem.pool   = sctSap->pst.pool;

   /* Copy the actual buffer into tempBuff and send it to peer */
   SCpyMsgMsg(mBuf, mem.region, mem.pool, &tmpBuf);
   
   CZ_DBG_INFO((CZ_PRNT_BUF,
            "Stored buffer sent to SCTP to be transmitted.\n"));
   CzLiSctDatReq(&sctSap->pst, sctSap->spId,
                 peerCb->assocCb->spAssocId,
                 &peerCb->assocCb->assocCfg->dstAddrLst.nAddr[0],
                  peerCb->peerCfg.globalStreamId,
                  FALSE, FALSE, 0, CZ_SCT_PROTID_X2AP, tmpBuf);

   CZ_RETVALUE(ROK);

} /* end of czTpmSndStoredBuf */


/**
  @brief This function sends the stored X2AP messages to peer through SCTP.
 
  @details This function is used to send X2AP messages to SCTP and stores the
  encoded buffer in the respective CB. This is required for retransmission of:
  - X2 Setup
  - eNodeB Configuration Update Request
  - Resource Status Request
 if the peer has not responded yet for previous respective requests yet.
  
  @param[in]   peerCb        Peer to which to send the message.
  @param[in]   pdu           The pointer to X2AP ASN PDU structure to send to
                             peer.
  @param[in]   streamId      SCTP outbound stream Id to send this message
                             to peer.
  @param[out]  copyBuf       Buffer to copy the message before sending it to 
                             trnsport.
                           
 
  @return
       - ROK           - If the Procedure was successful.
       - RFAILED       - If the procedure was unsuccessful due to any failure.
*/
#ifdef ANSI
PUBLIC S16 czTpmStoreAndSendMsg
(
CzPeerCb       *peerCb,  
CztX2AP_PDU    *pdu,
SctStrmId       streamId,
Buffer         **copyBuf
)
#else
PUBLIC S16 czTpmStoreAndSendMsg(peerCb, pdu, streamId, copyBuf) 
CzPeerCb       *peerCb;     
CztX2AP_PDU    *pdu;        
SctStrmId       streamId;
Buffer         **copyBuf;
#endif
{
   Buffer *mBuf = NULLP;    /* Encoded Buffer */
   Mem     mem;
   S16     retVal = ROK;
   CzSctSapCb  *sctSap   = NULLP;
   
   CztLclErrInd errInd;

   CZ_TRC2(czTpmStoreAndSendMsg);
   CZ_PRNT_PEERID;

   cmMemset((U8*)&errInd, 0, sizeof(CztLclErrInd));

   /* Retrieve the sctSap control block and check for NULLP */
   sctSap = peerCb->sctSapCb;

   if (sctSap == (CzSctSapCb *)NULLP)
   {
      CZ_DBG_ERROR((CZ_PRNT_BUF, "SAP is NULL\n"));
      CZ_RETVALUE(CZT_ERRCAUSE_SAP_NOTPRST);
   } /* end of if sctCb is NULL */

   
   /* If endpoint is not in open state  or assoc state isnot connected
    * free the pdu and return error 
    */
   if ((sctSap->state != LCZ_SAP_ISBOUND) || 
       (sctSap->endpCb.state != CZ_ENDP_OPEN) || 
       (peerCb->assocCb->assocState != CZ_ASSOC_UP))
   {
      CZ_DBG_ERROR((CZ_PRNT_BUF, "SAP is not bound. OR Association is \
               not connected.\n"));
      CZ_RETVALUE(CZT_ERRCAUSE_ASSOC_NOTPRST);
   } 
   mem.region = peerCb->sctSapCb->pst.region;
   mem.pool   = peerCb->sctSapCb->pst.pool;

   CZ_DBG_INFO((CZ_PRNT_BUF,
            "Encoding the outgoing message...\n"));
   /* Encode the pdu */
   if ((retVal = czEdmEncode(&mem, pdu, &mBuf)) != ROK)
   {
      CZ_DBG_ERROR((CZ_PRNT_BUF, "Encoding failed\n"));
      /* Update the global statistics */
      czCb.genSts.nmbEncErr++;
      *copyBuf = NULLP;
      CZ_RETVALUE(CZT_ERRCAUSE_ENC_FAILED);
   }
#ifdef SS_DIAG
   /* cz001.301:Change for Diagnostics*/
   CZ_DIAG_MSG(mBuf, "Send X2Ap Message : ");
#endif
   /* If trace flag is enabled send the trace indication */
   /* If trace flag is enabled send the trace indication */
   /* Give a trace indication if either of flags in sctSap and peerCb
    * are enabled. */
   if (sctSap->trcLen != 0 || peerCb->peerCfg.trcLen != 0)
   {
      CzTrc        trcParam;
      cmMemset((U8*)&trcParam, 0, sizeof(CzTrc));
      /* Populate the trace params */
      trcParam.event = SCT_EVTDATREQ;

      CZ_MEM_COPY(&trcParam.srcAddr.ipAddr, 
                  &sctSap->endpCb.srcAddrLst, 
                  sizeof(SctNetAddrLst));

      trcParam.srcAddr.sctPort = sctSap->endpCb.port; 

      CZ_MEM_COPY((&trcParam.dstAddr.ipAddr), 
                  (&peerCb->assocCb->assocCfg->dstAddrLst), 
                  sizeof(SctNetAddrLst));

      trcParam.sapId = sctSap->spId;
      trcParam.peerId = peerCb->peerId;

      CZ_DBG_INFO((CZ_PRNT_BUF,
               "Message being sent, sending Trace to user\n"));
      if (sctSap->trcLen != 0 )
      {
         czLmmSendLmTrc(sctSap->trcLen, &trcParam, mBuf);
      }
      else
      {
         czLmmSendLmTrc(peerCb->peerCfg.trcLen, &trcParam, mBuf);
      }
   }
   if (NULLP != mBuf)
   {   
   SCpyMsgMsg(mBuf, mem.region, mem.pool, copyBuf);
   
   CZ_DBG_INFO((CZ_PRNT_BUF,
            "Sending message to SCTP to be transmitted.\n"));
   /* Call SctDatReq primitive to send the pdu to 
    * lower layer.
    */
   CzLiSctDatReq(&sctSap->pst, sctSap->spId,
                   peerCb->assocCb->spAssocId,
                   &peerCb->assocCb->assocCfg->dstAddrLst.nAddr[0],
                   streamId,
                   FALSE, FALSE, 0, CZ_SCT_PROTID_X2AP, mBuf);
   }

   CZ_RETVALUE(ROK);

} /* end of czTpmStoreAndSendMsg */

/**
  @brief This function sends the X2AP messages to peer through SCTP.

  @details
   This function is used to send all the x2ap pdu's to the lower layer.
   First it checks for the proper end point and the association.If endpoint is not in open state  
   or the ssociation state is not connected, free the pdu and return error to the caller.
   else it encodes the pdu and send the encoded pdu to lower layer.
   
    
  @param[in] peerCb        Peer to which to send the message.
  @param[in] pdu           The pointer to X2AP ASN PDU structure to send to
                           peer.
  @param[in] streamId      SCTP outbound stream Id to send this message
                           to peer.
 
  @return
       - ROK           - If the Procedure was successful.
       - RFAILED       - If the procedure was unsuccessful due to any failure.
*/
#ifdef ANSI
PUBLIC S16 czTpmSndToLower 
(
CzPeerCb       *peerCb,  
CztX2AP_PDU    *pdu,
SctStrmId       streamId
)
#else
PUBLIC S16 czTpmSndToLower (peerCb, pdu, streamId) 
CzPeerCb       *peerCb;     
CztX2AP_PDU    *pdu;        
SctStrmId       streamId;
#endif
{
   S16    retVal = ROK;
   Mem    mem;              /* Memory block */
   Buffer *mBuf = NULLP;    /* Encoded Buffer */

   CzSctSapCb  *sctSap   = NULLP;
   
   CztLclErrInd errInd;

   CZ_TRC2(czTpmSndToLower);
   CZ_PRNT_PEERID;

   cmMemset((U8*)&errInd, 0, sizeof(CztLclErrInd));

   /* Retrieve the sctSap control block and check for NULLP */
   sctSap = peerCb->sctSapCb;

   if (sctSap == (CzSctSapCb *)NULLP)
   {
      CZ_DBG_ERROR((CZ_PRNT_BUF, "SAP is NULL\n"));
      CZ_RETVALUE(CZT_ERRCAUSE_SAP_NOTPRST);
   } /* end of if sctCb is NULL */

   
   /* If endpoint is not in open state  or assoc state isnot connected
    * free the pdu and return error 
    */
   if ((sctSap->state != LCZ_SAP_ISBOUND) || 
       (sctSap->endpCb.state != CZ_ENDP_OPEN) || 
       (peerCb->assocCb->assocState != CZ_ASSOC_UP))
   {

      CZ_DBG_ERROR((CZ_PRNT_BUF, "SAP is not bound. OR Association is \
               not connected.\n"));
      CZ_RETVALUE(CZT_ERRCAUSE_ASSOC_NOTPRST);
   } 
   mem.region = peerCb->sctSapCb->pst.region;
   mem.pool   = peerCb->sctSapCb->pst.pool;

   CZ_DBG_INFO((CZ_PRNT_BUF,
            "Encoding the outgoing message...\n"));
   /* Encode the pdu */
   if ((retVal = czEdmEncode(&mem, pdu, &mBuf)) != ROK)
   {
      /* Update the global statistics */
      czCb.genSts.nmbEncErr++;

      CZ_RETVALUE(CZT_ERRCAUSE_ENC_FAILED);
   }

   /* If trace flag is enabled send the trace indication */
   /* If trace flag is enabled send the trace indication */
   /* Give a trace indication if either of flags in sctSap and peerCb
    * are enabled. */
   if (sctSap->trcLen != 0 || peerCb->peerCfg.trcLen != 0)
   {
      CzTrc        trcParam;
      cmMemset((U8*)&trcParam, 0, sizeof(CzTrc));
      /* Populate the trace params */
      trcParam.event = SCT_EVTDATREQ;

      CZ_MEM_COPY((&trcParam.srcAddr.ipAddr), (&sctSap->endpCb.srcAddrLst),
                   sizeof(SctNetAddrLst));

      trcParam.srcAddr.sctPort = sctSap->endpCb.port; 
      CZ_MEM_COPY((&trcParam.dstAddr.ipAddr), 
                  (&peerCb->assocCb->assocCfg->dstAddrLst),
                  sizeof(SctNetAddrLst));

      trcParam.sapId = sctSap->spId;
      trcParam.peerId = peerCb->peerId;

      CZ_DBG_INFO((CZ_PRNT_BUF,
               "Message being sent, sending Trace to user\n"));
      if (sctSap->trcLen != 0 )
      {
         czLmmSendLmTrc(sctSap->trcLen, &trcParam, mBuf);
      }
      else
      {
         czLmmSendLmTrc(peerCb->peerCfg.trcLen, &trcParam, mBuf);
      }
   }
   
   CZ_DBG_INFO((CZ_PRNT_BUF,
            "Sending message to SCTP to be transmitted.\n"));
   /* Call SctDatReq primitive to send the pdu to 
    * lower layer.
    */
    CzLiSctDatReq(&sctSap->pst, sctSap->spId,
                   peerCb->assocCb->spAssocId,
                   &peerCb->assocCb->assocCfg->dstAddrLst.nAddr[0],
                   streamId,
                   FALSE, FALSE, 0, CZ_SCT_PROTID_X2AP, mBuf);
   CZ_RETVALUE(ROK);

} /* end of czTpmSndToLower */


/**
  @brief This function performs the dynamic peer configuration of a new
  peer which is not previously configured by layer manager. 
 
  @details
   This function will be called when we receive assosiation from unknown 
   peer which was not previously configured by layer manager.
   This function allocate new peer control block and assoc control block if 
   dynamic peer configration is enbaled.It will insert the created assoc control block
   into dynamic assosiation list and intializes all the peer assoc indication parameters(CztPeerAssocInd)
   and sends the assosiation indication to the x2ap user.
   
   
  
  @param[in] suId         Service user Id.
  @param[in] assocParams  Association parameters.
                             
 
  @return
       - ROK           - If the Procedure was successful.
       - RFAILED       - If the procedure was unsuccessful due to any failure.
*/
#ifdef ANSI
PUBLIC S16 czTpmDynamPeerConfig
(
SuId              suId,             
SctAssocIndParams *assocParams       
)
#else
PUBLIC S16 czTpmDynamPeerConfig (suId, assocParams)
SuId              suId;              
SctAssocIndParams *assocParams;      
#endif
{
   CzDynamPeerAssocCb  *peerAssocCb = NULLP;
   CzDynamPeerAssocCb  *pPeerAssocCb = NULLP;
   CztNetAddrLst       *dstAddrLst;
   SctPort              dstPort;
   CzPeerCb            *peerCb = NULLP;
   CztPeerAssocInd     *peerAssocInd;
   S16                  ret;
   U16                  idx;

   CZ_TRC2(czTpmDynamPeerConfig);

   if (assocParams->type == SCT_ASSOC_IND_INIT)
   {
      dstAddrLst = (CztNetAddrLst *)&assocParams->t.initParams.peerAddrLst;
      dstPort    = assocParams->t.initParams.peerPort;
   }
   else
   {
      dstAddrLst = (CztNetAddrLst *)&assocParams->t.cookieParams.peerAddrLst;
      dstPort    = assocParams->t.cookieParams.peerPort;
   }

   /* Search for any existing transient peerAssoc control block */
   ret = czNmmFindDynamPeerAssocCb(dstAddrLst, dstPort, &pPeerAssocCb);
   if (pPeerAssocCb != NULLP)
   {
      CZ_DBG_ERROR((CZ_PRNT_BUF, "Dynamic Peer Association Cb not found\n"));
      CZ_RETVALUE(ROK);
   }

   /* Allocate peer control block and assoc control block */
   if ((ret = czNmmAllocDynamPeerCb(suId, &peerCb)) != ROK)
   {
      CZ_DBG_FATAL((CZ_PRNT_BUF, "Dynamic Peer Config failed. \
               Dynamic Peer Cb could not be created.\n"));
      CZ_RETVALUE(RFAILED);
   }

   /* Fill peer and association control blocks */
   if (assocParams->type == SCT_ASSOC_IND_INIT)
   {
      CZ_MEM_COPY((&peerCb->assocCb->assocCfg->dstAddrLst), 
                  (&assocParams->t.initParams.peerAddrLst),
                   sizeof(SctNetAddrLst));

      peerCb->assocCb->assocCfg->dstPort = assocParams->t.initParams.peerPort;

      peerCb->assocCb->assocCfg->locOutStrms = 
      (assocParams->t.initParams.inStrms < assocParams->t.initParams.outStrms)?
       assocParams->t.initParams.inStrms : assocParams->t.initParams.outStrms;
   }
   else
   {
      CZ_MEM_COPY((&peerCb->assocCb->assocCfg->dstAddrLst), 
                  (&assocParams->t.cookieParams.peerAddrLst),
                   sizeof(SctNetAddrLst));

      peerCb->assocCb->assocCfg->dstPort = assocParams->t.cookieParams.peerPort;

      peerCb->assocCb->spAssocId  = assocParams->t.cookieParams.spAssocId;
      peerCb->assocCb->assocState = CZ_ASSOC_IN_PRG;
   }

   /* Insert the assoc control block in transient assocCb hash list */
   for (idx = 0; idx < dstAddrLst->nmb; idx++)
   {
      CZ_ALLOC(peerAssocCb, sizeof(CzDynamPeerAssocCb));
      if (NULLP == peerAssocCb)
      {
         SLogError(peerCb->cztSapCb->pst.srcEnt,
               peerCb->cztSapCb->pst.srcInst, peerCb->cztSapCb->pst.srcProcId,
         __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
         (ErrVal)ECZT015, (ErrVal)0, "Memory allocaiton failed");

         CZ_DBG_FATAL((CZ_PRNT_BUF, "peer Assoc Cb allocation failed.\n"));
         czNmmRemDynamPeerAssoc(peerCb);
         czNmmDeallocDynamPeerCb(peerCb);

         CZ_RETVALUE(RFAILED);
      }

      peerAssocCb->peerCbPtr   = peerCb;
      peerAssocCb->assocParams = *assocParams;

      peerAssocCb->peerAssocEntry.peerAddr.type = dstAddrLst->nAddr[idx].type;
      if (peerAssocCb->peerAssocEntry.peerAddr.type == CM_NETADDR_IPV4)
      {
         peerAssocCb->peerAssocEntry.peerAddr.u.ipv4NetAddr =
                                 dstAddrLst->nAddr[idx].u.ipv4NetAddr;
      }
      else
      {
         CZ_MEM_COPY(&peerAssocCb->peerAssocEntry.peerAddr, 
                     &dstAddrLst->nAddr[idx], 
                     sizeof(CmNetAddr));
      }
               
      peerAssocCb->peerAssocEntry.port = peerCb->assocCb->assocCfg->dstPort;

      ret = cmHashListInsert(&(czCb.nodeCb.dynamPeerAssocLst), (PTR)(peerAssocCb),
                              (U8 *)&(peerAssocCb->peerAssocEntry), 
                              sizeof(CzDynamPeerAssocEntry));

      if (ret != ROK)
      {
         czNmmRemDynamPeerAssoc(peerCb);
         czNmmDeallocDynamPeerCb(peerCb);
         /* cz001.301 : purify Fix: Freeing the peerAssocCb in Error Scenario  */
         CZ_FREE(peerAssocCb, sizeof(CzDynamPeerAssocCb));
         CZ_RETVALUE(RFAILED);
      }
   }

   /* fill peerAssocInd structure */
   CZ_ALLOC(peerAssocInd, sizeof(CztPeerAssocInd));
   if(peerAssocInd == NULLP)
   {
         SLogError(peerCb->cztSapCb->pst.srcEnt,
               peerCb->cztSapCb->pst.srcInst, peerCb->cztSapCb->pst.srcProcId,
         __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
         (ErrVal)ECZT015, (ErrVal)0, "Memory allocaiton failed");

         CZ_DBG_FATAL((CZ_PRNT_BUF, "peerAssocInd allocation failed.\n"));
         czNmmRemDynamPeerAssoc(peerCb);
         czNmmDeallocDynamPeerCb(peerCb);
         CZ_RETVALUE(RFAILED);
   }

   peerAssocInd->dstPort = peerCb->assocCb->assocCfg->dstPort;

   CZ_MEM_COPY((&peerAssocInd->dstAddrLst), dstAddrLst,
                sizeof(SctNetAddrLst));

      CZ_DBG_INFO((CZ_PRNT_BUF,
               "Sending Assoc Indication to the user.\n"));
   /* Send dynamic peer indication to the user */
   /* cz001.301 : purify fix: freeing peerAssocInd */
   if(czUimSndPeerAssocInd(peerCb, peerAssocInd) != ROK)
   {
      CZ_FREE(peerAssocInd, sizeof(CztPeerAssocInd));
   }

   CZ_RETVALUE(ROK);
}


/**
 @brief This function handles association termination indication from SCTP.

 @details
  This function informs to x2ap iuser that the assosiation is terminated for the 
  for the association Id received and also inform the user that assosiation is terminated
  through status indication primitive and  send alarm to layer manager.

 @param[in] suId          Service user Id.
 @param[in] assocId       Association Id.
 @param[in] assocIdType   Association Id type.
                            

 @return
       - ROK           - If the Procedure was successful.
       - RFAILED       - If the procedure was unsuccessful due to any failure.
*/
#ifdef ANSI
PUBLIC S16 czTpmHndlTermInd
(
SuId              suId,
UConnId           assocId,
U8                assocIdType
)
#else
PUBLIC S16 czTpmHndlTermInd (suId, assocId, assocIdType)
SuId              suId;
UConnId           assocId;
U8                assocIdType;
#endif
{
   S16         ret = RFAILED;
   CzPeerCb   *peerCb = NULLP;
   CztStaInd   sta;
   CzCztSapCb *cztSapCb = NULLP;
   U32         peerId;
   
   CZ_TRC2(czTpmHndlTermInd);

   peerId = assocId;
   ret = czNmmFndPeer(peerId, &peerCb); 

#if (ERRCLASS & ERRCLS_DEBUG)
   if(peerCb == NULLP)
   {
      CZ_DBG_INFO((CZ_PRNT_BUF,
               "PeerCb not found. Has to be a Dynamically configured Peer.\n"));
      if ((czCb.nodeCb.protCfg.dynamPeerFlag == TRUE) &&
          (assocIdType ==  SCT_ASSOCID_SP))
      { 
         CzDynamPeerAssocCb *crnt = NULLP;
         CzDynamPeerAssocCb *next = NULLP; 
         CzPeerCb           *peer = NULLP;

         while (cmHashListGetNext(&czCb.nodeCb.dynamPeerAssocLst,
                  (PTR)crnt,
                  (PTR *)&next) == ROK)
         {
            if ((next->assocParams.type == SCT_ASSOC_IND_COOKIE) &&
                (next->peerCbPtr->sctSapCb->suId == suId) &&
                (next->assocParams.t.cookieParams.spAssocId == assocId))
            {
               peer = next->peerCbPtr;
               break;
            }
            crnt = next;
            next = NULLP;
         }
         if (peer != NULLP)
         {
            czNmmRemDynamPeerAssoc(peer);
            czNmmDeallocDynamPeerCb(peer);
         }
         CZ_RETVALUE(ROK);
      }
      CZ_RETVALUE(RFAILED);

   } /* end of if peerCb == NULLP */
#endif /* (ERRCLASS & ERRCLS_DEBUG) */

   CZ_PRNT_PEERID;
   czNmmResetPeer(peerCb);
   cztSapCb = peerCb->cztSapCb;

   /* Set the assoc state to DOWN */
   peerCb->assocCb->assocState = CZ_ASSOC_DOWN;

   CZ_SET_ZERO(&sta, sizeof(CztStaInd));

   /* Populate the sta indication structure */
   sta.peerId = peerCb->peerId;
   sta.type   = CZT_STA_TYPE_ASSOC;
   sta.status = CZT_STATUS_ASSOC_DOWN;
   sta.reason = CZT_REASON_RCVD_TERMIND;

   CZ_DBG_INFO((CZ_PRNT_BUF,
            "Sending Status Indication to the user about the term indication\
            received from the SCTP.\n"));

   /* cz001.101 : Fix to give all the indications (Flow Ind, Term Ind..) 
    * only to the concerned CZT SAP and not to all the SAPs. */
   ret = czUimSndStaInd (cztSapCb, &sta);
   /* Send status indication to user */
   if(ret != ROK)
   {
      /* Sending status indication failed */
      CZ_DBG_ERROR((CZ_PRNT_BUF, "Sending StaInd to user failed \n"));
      CZ_RETVALUE(ret);
   }
   /* Send an alarm indicating association is UP */
   CZ_SEND_PEER_ALARM(peerCb->peerId, NULLP, LCZ_EVENT_LI_TERM_IND, 
         LCZ_CAUSE_ASSOC_DOWN);

   CZ_RETVALUE(ROK);
}


/**
 @brief This function handles association status indication from SCTP.

 @details It is called from LIM. If status indicates that association is up
 then it sends out the stored X2 Setup buffer to peer. This is done only on
 source eNodeB, if user had initiated X2 setup previously.
 
 @param[in] peerId     Peer ID on which association status is received. 
 @param[in] status     Status of association.
 @param[in] cause      Cause of Status Indication.
 @param[in] dstNAddr   Destination Address. 
 @param[in] mBuf       Message Buffer.
                            

 @return
       - ROK           - If the Procedure was successful.
       - RFAILED       - If the procedure was unsuccessful due to any failure.
*/
#ifdef ANSI
PUBLIC S16 czTpmHndlStaInd
(
U32               peerId,             
SctStatus         status,  /* window's warning fix changed data type to SctStatus frm u8 */
SctCause          cause,    /* window's warning fix changed data type to SctCause frm u8 */       
CmNetAddr        *dstNAddr,        
Buffer           *mBuf           
)
#else
PUBLIC S16 czTpmHndlStaInd (peerId, status, cause, dstNAddr, mBuf)
U32               peerId;             
SctStatus         status;  /* window's warning fix changed data type to SctStatus frm u8 */
SctCause          cause;    /* window's warning fix changed data type to SctCause frm u8 */       
CmNetAddr        *dstNAddr;        
Buffer           *mBuf;           
#endif
{
   S16         ret = RFAILED;
   CzPeerCb   *peerCb = NULLP;
   /*cz001.301:x2ap build warning fix:Variable "sta" is never used*/
   CzCztSapCb *cztSapCb = NULLP;
   Mem         mem;
   U16          czCause = 0;
   U16          event = 0;

   CZ_TRC2(czTpmHndlStaInd)

    ret = czNmmFndPeer(peerId, &peerCb);
    if(peerCb == NULLP)
    {
       CZ_DBG_ERROR((CZ_PRNT_BUF, "PeerCb not found.\n"));
       CZ_RETVALUE(RFAILED);
    }
    CZ_PRNT_PEERID;
    cztSapCb = peerCb->cztSapCb;


    CZ_SET_ZERO(&mem, sizeof(Mem));

    mem.region = czCb.init.region;
    mem.pool   = czCb.init.pool;

    switch(status)
    {
        case SCT_STATUS_COMM_UP:
           {
              /* Retrieve the peer control block.Since the peerId is
               * same as suAssocId, peer control block will have
               * the same index as suAssocId 
               */

              CZ_DBG_INFO((CZ_PRNT_BUF,
                       "Association is up.\n"));
              /* Update the assocState in assocCb */
              peerCb->assocCb->assocState = CZ_ASSOC_UP;

              /* Retrieve the X2Setup Request from peerCb if any.
               * and send it to the peer.
               */
              if(peerCb->gpmCb.retxBuf != NULLP) 
              {
                 ret = czTpmSndStoredBuf (peerCb, peerCb->gpmCb.retxBuf);   
              }
              break;
           } /* end of SCT_STATUS_COMM_UP */
        default:
           {
              break;
           }
    } /* end of switch */

    CZ_DBG_INFO((CZ_PRNT_BUF,
             "The Status Indication received from SCTP is sent to the layer \
             manager. Status is %d and Cause is %d.\n",status,cause));
    /* cz001.101: LCZ_EVENT_LI_START and LCZ_CAUSE_SCT_START are reference points
     * in LCZ define from where the SCT status and cause values are 
     * mapped. Each status/cause value received from SCTP is mapped
     * to a macro in LCZ in the "same order". Hence we aviod a switch here.*/

    event = LCZ_EVENT_LI_START + status;
    czCause = LCZ_CAUSE_SCT_START + cause;
    
    /* cz001.101 : Give only alarm about the event. CZT Indication is 
     * removed. */
    /* Send status indication to user */
    CZ_SEND_PEER_ALARM(peerCb->peerId, dstNAddr, event, czCause);

    CZ_RETVALUE(ROK);
}


/**
 @brief This function sends association termination request to SCTP.
 
 @details
   This function is used to terminate an association request with a peer. The
   Depending on the assocAbort flag , this function takes down the connection either
   gracefully, or ungracefully. For an ungraceful take down, the function
   sbAcAbortAssoc is called, after which the association is removed, after the
   freeze timer has elapsed, and memory has been cleaned. For a graceful
   shutdown, the sbAcShutdownAssoc is called. Once completed, it sends the SbUiSctTermCfm. 
   The assocIdType is used if an spAssocId has not yet been received from the SCTP layer and 
   the suAssocId must be used. This can be the case when the service user requests an AssocReq, 
   then sends an TermReq before receiving an AssocCfm primitive.

 
 @param[in] assocCb      Association control block.
 @param[in] assocAbort   Bool to represent whether to do graceful
                         termination.

 @return
       - ROK           - If the Procedure was successful.
       - RFAILED       - If the procedure was unsuccessful due to any failure.
*/
#ifdef ANSI
PUBLIC S16 czTpmTermAssoc 
(
CzAssocCb  *assocCb,
Bool        assocAbort
)
#else
PUBLIC S16 czTpmTermAssoc(assocCb, assocAbort)
CzAssocCb  *assocCb;    
Bool        assocAbort;
#endif
{
   U8       assocType;
   UConnId  assocId;
   CzPeerCb   *peerCb;

   CZ_TRC2(CzTpmTermAssoc)
   peerCb=assocCb->peer;
   CZ_PRNT_PEERID;

   if (assocCb->assocState == CZ_ASSOC_DOWN)
   {
      CZ_DBG_INFO((CZ_PRNT_BUF,
             "CzTpmTermAssoc: Association is already set to down \n"));
      CZ_RETVALUE(RFAILED);
   }

   /* Update the Assoc state to DOWN */
   if(assocCb->assocState == CZ_ASSOC_UP)
   {
      assocId   = assocCb->spAssocId;
      assocType = SCT_ASSOCID_SP;
   }
   else
   {
      assocId   = assocCb->suAssocId;
      assocType = SCT_ASSOCID_SU;
   }      
   assocCb->assocState  = CZ_ASSOC_DOWN;

   CZ_DBG_INFO((CZ_PRNT_BUF,
            "Sending the Term Request to the SCTP\n"));
   /* Send termination request */
   CzLiSctTermReq(&(assocCb->peer->sctSapCb->pst), 
                  assocCb->peer->sctSapCb->spId,  
                  assocId, assocType, assocAbort);

   CZ_RETVALUE(ROK);

} /* czTpmTermAssoc*/


/**
  @brief  This function handles association indication from SCTP
 
  @details It sends out the association response successful or failure back to
  SCTP if peer is configured by layer manager.
  If no such peer is present, based on if layer manager allows the dynamic
  peer it performs the dynamic peer configuration and informs the user
  about this new peer.
  
  @param[in] suId          Service user Id
  @param[in] sctSapCb      SCTP SAP reference control block.
  @param[in] assocParams   Association parameters
 
  @return
       - ROK           - If the Procedure was successful.
       - RFAILED       - If the procedure was unsuccessful due to any failure.
*/
#ifdef ANSI
PUBLIC S16 czTpmHndlAssocInd
(
SuId                suId, 
CzSctSapCb          *sctSapCb,
SctAssocIndParams   *assocParams
)
#else
PUBLIC S16 czTpmHndlAssocInd(suId, sctSapCb, assocParams)
SuId                suId;   
CzSctSapCb          *sctSapCb;    
SctAssocIndParams   *assocParams;
#endif
{
   CzPeerCb   *peerCb = NULLP;
   S16         ret    = ROK;
   CztStaInd   sta;
   U8          spIdCnt = 0;
   CzCztSapCb *cztSapCb = NULLP;

   CZ_TRC2(czTpmHndlAssocInd);

   CZ_SET_ZERO(&sta, sizeof(CztStaInd));

   /* Retrieve the peer control block based on the peer control address */
   if(assocParams->type == SCT_ASSOC_IND_INIT)
   {
      peerCb  = 
      czNmmMatchPeer((CztNetAddrLst *)&assocParams->t.initParams.peerAddrLst,assocParams->t.initParams.peerPort ); 
   }
   else
   {
      peerCb = 
      czNmmMatchPeer((CztNetAddrLst *)&assocParams->t.cookieParams.peerAddrLst, assocParams->t.cookieParams.peerPort); 
   }
   if(NULLP == peerCb)
   {
      CZ_DBG_INFO((CZ_PRNT_BUF,
               "PeerCb NULL. Has to be dynamically configured peer.\n"));
      if (czCb.nodeCb.protCfg.dynamPeerFlag == TRUE)
      {
         /* do dynamic peer configuration */
         ret = czTpmDynamPeerConfig(suId, assocParams);
      }
      else
      {
         /* Send StaInd to User Indicating that Dynamic peeer is not configured
          * */
            /* Populate the sta indication structure */
            sta.type   = CZT_STA_TYPE_ASSOC;
            sta.status = CZT_STATUS_DYNAM_PEERCFG_NOK;
            sta.reason = CZT_REASON_DYNAM_PEER_NOT_CFG;

          /* Retrieve the upper sap ids from CztSapIdLst and 
            * send the status indication to all the users
            * that association is up
            */
          for (spIdCnt = 0; spIdCnt < czCb.genCfg.nmbCztSaps; spIdCnt++)
          {
             if (((cztSapCb = czCb.cztSapLst[spIdCnt]) != NULLP) && 
                 ((czCb.cztSapLst[spIdCnt]->state == LCZ_SAP_ISBOUND)))
             {
                /* Inform the user indicating association is UP */
                czUimSndStaInd (cztSapCb, &sta);
             } /* cztSap is not NULL */      

          } /* end of for loop which iterates through all the upperSaps */
          ret = RFAILED;
      }
      
      if(ret != ROK)
      {
#ifdef SCT3
         CzLiSctAssocRsp(&sctSapCb->pst, sctSapCb->spId, 
                         sctSapCb->endpCb.locSpEndpId,
                         assocParams, 0, SCT_NOK, NULLP);
#else
         CzLiSctAssocRsp(&sctSapCb->pst, sctSapCb->spId, 
                         sctSapCb->endpCb.locSpEndpId,
                         assocParams, SCT_NOK, NULLP);
#endif /* end of SCT3 */
      /* Debug Error */
      }
      CZ_RETVALUE(ret);
   }
    
   /* suAssocId is same as peerId, so that it would be easy
    * to retrieve the peer control block later. Update suAssocId
    * in assocCb.PeerId is the index in the peerLst (which is an 
    * array of pointers to peerCb 
    */
   peerCb->assocCb->suAssocId = peerCb->peerId;
   /* cz001.301: Change for Diagnostics*/
#ifdef SS_DIAG
   CZ_DIAG_LVL0(0x04100002, CZ_DIAG_PEERID, peerCb->peerId,
      "Received AssocInd from SCTP, ind type  = %d, spAssocId  = %d",
                assocParams->type, assocParams->t.cookieParams.spAssocId, 0, 0)
#endif
   /* Update the peerCb with assoc info */
   if (assocParams->type == SCT_ASSOC_IND_INIT)
   {
      /* Update the assocCb */
      peerCb->assocCb->assocCfg->dstPort = assocParams->t.initParams.peerPort;
      CZ_MEM_COPY(&peerCb->assocCb->assocCfg->dstAddrLst, 
                  &assocParams->t.initParams.peerAddrLst,
                  sizeof(SctNetAddrLst));
      peerCb->assocCb->assocCfg->locOutStrms = 
       ((assocParams->t.initParams.inStrms) < 
       (peerCb->assocCb->assocCfg->locOutStrms))?
       (assocParams->t.initParams.inStrms):(peerCb->assocCb->assocCfg->locOutStrms);
   } /* end of chunk type is INIT */
   else
   {
      /* Update the assocCb */
      CZ_MEM_COPY(&peerCb->assocCb->assocCfg->dstAddrLst, 
                  &assocParams->t.cookieParams.peerAddrLst,
                  sizeof(SctNetAddrLst));
      peerCb->assocCb->assocCfg->dstPort= assocParams->t.cookieParams.peerPort;
      peerCb->assocCb->spAssocId = assocParams->t.cookieParams.spAssocId;
      assocParams->t.cookieParams.suAssocId = peerCb->assocCb->suAssocId;
      peerCb->assocCb->peer = peerCb;
      peerCb->assocCb->assocState = CZ_ASSOC_IN_PRG;

   } /* end of if chunk type is COOKIE_CHUNK */
   
   CZ_DBG_INFO((CZ_PRNT_BUF,
            "Sending Association Response to the SCTP to be transmitted.\n"));
   /* Send the association response */
#ifdef SCT3
   CzLiSctAssocRsp(&sctSapCb->pst, sctSapCb->spId, sctSapCb->endpCb.locSpEndpId,
                   assocParams, 0, SCT_OK, NULLP);
#else
   CzLiSctAssocRsp(&sctSapCb->pst, sctSapCb->spId, sctSapCb->endpCb.locSpEndpId,
                   assocParams, SCT_OK, NULLP);
#endif /* end of SCT3 */

   CZ_RETVALUE(ROK);

}


/**
  @brief This function sends a new SCTP association request. 

  @details
    This function sends the association request to peer.
    It checks for the association , if already up then return the failure to the caller
    else encodes the x2 setup envent and stores the encoded buffer into the peerCb and send the 
    association request to sctp and moves the association state to association in progress.
   
 
  @param[in] pdu         X2AP Setup Request pdu.
  @param[in] assocCb     Association control block.
 
  @return
       - ROK           - If the Procedure was successful.
       - RFAILED       - If the procedure was unsuccessful due to any failure.
*/
#ifdef ANSI
PUBLIC S16 czTpmSndAssocReq
(
CztX2AP_PDU  *pdu, 
CzAssocCb    *assocCb                
)
#else
PUBLIC S16 czTpmSndAssocReq(pdu, assocCb)
CztX2AP_PDU  *pdu;
CzAssocCb    *assocCb;               
#endif
{
   S16        ret = ROK;           /* return value */
   Mem        mem;
   CzPeerCb   *peerCb = NULLP;     /* Peer control block */
   CzSctSapCb *sctSapCb = NULLP;   /* SCT SAP control block */
#ifdef SCT3
   SctTos     tos = 0;             /* Type of service */
#endif /* SCT3 */


   CZ_TRC2(CzTpmSndAssocReq)

   /* Check the assoc state to see if its already up */
   if (assocCb->assocState == CZ_ASSOC_UP)
   {
      CZ_DBG_INFO((CZ_PRNT_BUF,
             "Association is already up and running \n"));
      CZ_RETVALUE(RFAILED);
   }

   /* Update the Assoc state */
   assocCb->assocState = CZ_ASSOC_IN_PRG;

   peerCb = assocCb->peer;
   sctSapCb = peerCb->sctSapCb;
   assocCb->suAssocId = peerCb->peerId;

   CZ_PRNT_PEERID;
  
   mem.region = peerCb->sctSapCb->pst.region;
   mem.pool   = peerCb->sctSapCb->pst.pool;

   CZ_DBG_INFO((CZ_PRNT_BUF,
            "Encoding the outgoing message...\n"));
   /* Encode the setupreq and store it in the peerCb */
   if ((ret = czEdmEncode(&mem, pdu, &peerCb->gpmCb.retxBuf)) != ROK)
   {
      CZ_DBG_ERROR((CZ_PRNT_BUF, "Encoding of setup Req Failed \n"));
      CZ_RETVALUE(CZT_ERRCAUSE_ENC_FAILED);
   }

      CZ_DBG_INFO((CZ_PRNT_BUF,
               "Sending Association Request to SCTP...\n"));
    /* Call SCT interface method to establish an association */ 
#ifdef SCT3
   CzLiSctAssocReq(&(sctSapCb->pst), sctSapCb->spId, 
                   sctSapCb->endpCb.locSpEndpId,
                   assocCb->suAssocId, &(assocCb->assocCfg->priDstAddr), 
                   assocCb->assocCfg->dstPort, assocCb->assocCfg->locOutStrms,
                   &(assocCb->assocCfg->dstAddrLst),
                   (SctNetAddrLst *)&(sctSapCb->endpCb.srcAddrLst), 
                   tos, (Buffer *)NULLP);
#else
   CzLiSctAssocReq(&sctSapCb->pst, sctSapCb->spId, sctSapCb->endpCb.locSpEndpId,
                   assocCb->suAssocId, &(assocCb->assocCfg->priDstAddr), 
                   assocCb->assocCfg->dstPort, assocCb->assocCfg->locOutStrms,
                   &(assocCb->assocCfg->dstAddrLst),
                   (SctNetAddrLst *)&(sctSapCb->endpCb.srcAddrLst), 
                   (Buffer *)NULLP);
#endif /* SCT3 */
   
   CZ_RETVALUE(ret);

} /* czTpmSndAssocReq */

/** @} */

/**********************************************************************
  
         End of file:     tr_tpm.c@@/main/2 - Tue Aug 30 18:36:15 2011
   
**********************************************************************/
 
/**********************************************************************
        Revision History:

**********************************************************************/
/********************************************************************90**
 
     ver       pat    init                  description
------------ -------- ---- ----------------------------------------------
/main/1      ---        ds              1. LTE-X2AP Initial Release.
            cz001.101   mm              1. CR Fixes, Memory Leak fixes.
                                        2. Fix to give all the indications
                                        (Flow Ind, Term Ind..) only to the 
                                        concerned CZT SAP and not to all
                                        the SAPs.
                                        3. Update to pass the destn address 
                                        to the user through alarm whenever a 
                                        NET_DOWN is received from the SCTP.
                                        4. The alarm types and causes in X2AP
                                        are mapped to the status types and 
                                        causes that are received from SCTP. 
                                        5. Set the primary address in the 
                                        AssocCfg structure of a peerCb 
                                        depending the result in SctSetPriCfm.
                                        6. Correction in usage of Trace Flag.
/main/2      ---         pt             1. LTE-X2AP 3.1 release.
/main/2      cz001.301   akaranth       1. Inclusion for Diagnostic MACRO
                                        2. Chage for Diagnostics
                                        3. x2ap build warning fix 
                                        4. Freeing the peerAssocCb in Error Scenario.
*********************************************************************91*/
