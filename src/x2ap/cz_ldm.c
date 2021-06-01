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

     Desc:     This file represents Trillium X2AP Load Management Procedure
               module. It contains handlers for the Resource Status
               Reporting procedures and Load Information procedure.

     File:     cz_ldm.c

     Sid:      tr_ldm.c@@/main/2 - Tue Aug 30 18:36:12 2011

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
#include "cm_lib.x"        /* Common linrary function         */
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


/** @defgroup ldmmodule X2AP Load Management Module
 *  @{
 */
/*
 * Local defines
 */
/** The maximum number of Resource Status related messages */
#define CZ_MAX_RSRC_STS_MSGS 4

/** The maximum number of directions in which
         messages arrive */
#define CZ_MAX_RSRC_STS_MSG_DIRS 2

/** @enum CzRsrcMsg The Resource Status Reporting related messages */
typedef enum
{
   CZ_RSRC_REQ = 0,
   CZ_RSRC_RSP,
   CZ_RSRC_FLR,
   CZ_RSRC_UPD
} CzRsrcMsg;

/**
  To build a key using direction, message type and eNB1 
        measurement ID, to retrieve the rsrcCb from peerCb
 */
#define CZ_MAKE_RSRC_KEY(_key, _eNB1MsrmntId, _msg, _dir) \
{ \
         _key=((U32)_eNB1MsrmntId | (U32)czProcDir[_msg][_dir]); \
}

/*
 * Local globals
 */
/**
  Matrix to give the eNB Type based on the Resource Status message
  and its direction.
 */
CzRsrcEnbType czProcDir[CZ_MAX_RSRC_STS_MSGS][CZ_MAX_RSRC_STS_MSG_DIRS] =
{
/* OUT */    /* IN */
{CZ_ENB1,      CZ_ENB2},        /* CZ_RSRC_REQ */
{CZ_ENB2,      CZ_ENB1},        /* CZ_RSRC_RSP */
{CZ_ENB2,      CZ_ENB1},        /* CZ_RSRC_FLR */
{CZ_ENB2,      CZ_ENB1}         /* CZ_RSRC_UPD */
};

PRIVATE Void czGpmSndRsrcErr ARGS((CzPeerCb* peerCb, CztEvnt* event, 
                                   CzRsrcInfo* rsrcInfo, U8 msgType, 
                                   U8 causeType, U8 causeValue));


PRIVATE S16 czGpmSndRsrcMsgToPeer ARGS((CzPeerCb* peerCb, U16 eNb1MeasId,
                                        U16 eNb2MeasId,   CztEvnt* event, 
                                        U8 msgType));

PRIVATE S16 czGpmRsrcStoreAndSendMsg ARGS((CzRsrcCb** rsrcCb, CztEvnt* event, 
                                           U8 msgType, Bool isNewCb));

PRIVATE S16 czGpmSndRsrcInd ARGS((CzPeerCb* peerCb, CzRsrcCb** rsrcCb, 
                                  CztEvnt* event, U8 msgType,
                                  Bool isNewCb, EnumCztRegistration_Rqst rqst));

PRIVATE S16 czGpmSndRsrcCfm ARGS((CzPeerCb* peerCb, CztEvnt* event, 
                                  U8 msgType));


PRIVATE S16 czGpmHdlOldRsrcCb ARGS((CzRsrcCb *rsrcCb, CztEvnt *event,
                                    CzRsrcInfo* rsrcInfo, 
                                    CztRegistration_Rqst *regnRqstIE,
                                    U8 dir));

PRIVATE S16 czGpmHdlNewRsrcCb ARGS((CzPeerCb *peerCb, CztEvnt *event,
                                    CztRegistration_Rqst *regnRqstIE,
                                    U16 eNb1MeasId, U8 dir));



/*
 *  GPM load management message handlers
 */

/**
   @brief The function  czGpmSndRsrcErr is used for sending error indication
          to the user.
   @details The function czGpmSndRsrcErr is used for sending error indication
          to the user, this function is called by various Resource Status
          message handlers when an error condition is met or when appropriate.
   @param[in] peerCb The control block related to the peer on whose
               X2-interface the message is exchanged.
   @param[in] event The pdu of the X2AP Message that is exchanged.
   @param[in] rsrcInfo The information related to the Resource Status Reporting.
   @param[in] msgType The X2AP Message type exchanged.
   @param[in] causeType The cause type related to the error.
   @param[in] causeValue The cause value related to the error.
   @return
        - ROK     - If the procedure was successful.
        - RFAILED - If the procedure was unsuccessful due to any failure.
*/
#ifdef ANSI
PRIVATE Void czGpmSndRsrcErr
(
CzPeerCb*   peerCb, 
CztEvnt*    event, 
CzRsrcInfo* rsrcInfo,
U8         msgType, 
U8         causeType,
U8         causeValue
) 
#else
PRIVATE Void czGpmSndRsrcErr(peerCb, event, rsrcInfo, 
                             msgType, causeType, causeValue)
CzPeerCb*   peerCb;
CztEvnt*    event; 
CzRsrcInfo* rsrcInfo;
U8         msgType; 
U8         causeType;
U8         causeValue; 
#endif
{
   CztEvnt* errEvnt = NULLP;

   CZ_TRC2(czGpmSndRsrcErr)
   CZ_PRNT_PEERID;

   if (CZT_MSG_RSRC_ST_REQ == msgType)
   {
      errEvnt = czUtlGenRsrcStsFail(NULLP,
                              rsrcInfo, causeType, causeValue);
   }
   else
   {
      errEvnt = czUtlGenPeerErrInd(&event->pdu, msgType,
                                 NULLP, causeType, causeValue);
   }
   if (errEvnt)
   {
      czTpmSndToLower(peerCb, &(errEvnt->pdu),
               peerCb->peerCfg.globalStreamId);
      CZ_FREEEVNT(errEvnt);
   }
   CZ_DBG_INFO((CZ_PRNT_BUF, "Error Indication Sent.\n" ));
   /* czUtlUpdInErrSts(peerCb, msgType); */
} /* czGpmSndRsrcErr */


/**
   @brief The function  czGpmSndRsrcMsgToPeer is called while sending a message the to peer.
   @details The function czGpmSndRsrcMsgToPeer is used for sending messages to peer.
            This function uses the TPM module to deliver the message to peer.
   @param[in] peerCb The control block related to the peer on whose
               X2-interface the message is exchanged.
   @param[in] eNb1MeasId The eNB1 Measurement ID referred to in the message.
   @param[in] eNb2MeasId The eNB2 Measurement ID referred to in the message.
   @param[in] event The pdu of the X2 Message that is exchanged.
   @param[in] msgType The X2 Message exchanged.
   @return
        - ROK     - If the procedure was successful.
        - RFAILED - If the procedure was unsuccessful due to any failure.
*/
#ifdef ANSI
PRIVATE S16 czGpmSndRsrcMsgToPeer
(
CzPeerCb* peerCb, 
U16       eNb1MeasId,
U16       eNb2MeasId,
CztEvnt*  event, 
U8       msgType
)
#else
PRIVATE S16 czGpmSndRsrcMsgToPeer(peerCb, eNb1MeasId, eNb2MeasId,
                                  event, msgType)
CzPeerCb* peerCb;
U16       eNb1MeasId;
U16       eNb2MeasId;
CztEvnt*  event; 
U8       msgType; 
#endif
{
   S16 ret = ROK;

   CZ_TRC2(czGpmSndRsrcMsgToPeer)
   CZ_PRNT_PEERID;

   ret = czTpmSndToLower(peerCb, &(event->pdu),
                         peerCb->peerCfg.globalStreamId);
   if(ret != ROK)
   {
      CZ_DBG_FATAL((CZ_PRNT_BUF, "Transmission of the Resource message failed .\n" ));
      CZ_SEND_LCL_RSRCPROC_ERR(peerCb, 
          CZT_ERRTYPE_MSG_SEND_FAILED, 
          ret, 
          msgType, eNb1MeasId, eNb2MeasId, 
          event, NULLP); 
      /* czUtlUpdOutErrSts(peerCb, msgType); */
   }
   else
   {
      CZ_DBG_INFO((CZ_PRNT_BUF, "Resource message Sent .\n" ));
      CZ_FREEEVNT(event);
      /* czUtlUpdOutSts(peerCb, msgType); */
   }
   
   CZ_RETVALUE(ret);

} /* czGpmSndRsrcMsgToPeer */

/**
   @brief The function czGpmRsrcStoreAndSendMsg is used for sending Resource 
          status request message to peer.
   @details The function czGpmRsrcStoreAndSendMsg is used for sending Resource
            status request message to the peer. It invokes the TPM function
            czTpmStoreAndSendMsg() to store the message buffer before sending 
            and sending it onto the network. The stored buffer can be later 
            used to retransmit the same message by the user.
   @param[in] rsrcCb The control block related to Resource Status Reporting 
              procedure in the context of the peer on whose X2-interface 
              the message is exchanged.
   @param[in] event The pdu of the X2 Message that is exchanged.
   @param[in] msgType The X2 Message exchanged.
   @param[in] isNewCb A boolean argument to indicate whether to delete the Cb.
   @return
        - ROK     - If the procedure was successful.
        - RFAILED - If the procedure was unsuccessful due to any failure.
*/
#ifdef ANSI
PRIVATE S16 czGpmRsrcStoreAndSendMsg
(
CzRsrcCb** rsrcCb, 
CztEvnt*  event, 
U8       msgType, 
Bool      isNewCb
)
#else
PRIVATE S16 czGpmRsrcStoreAndSendMsg(rsrcCb, event, msgType, isNewCb)
CzRsrcCb** rsrcCb;
CztEvnt*  event; 
U8       msgType; 
Bool      isNewCb;
#endif
{
   S16 ret = ROK;

   CZ_TRC2(czGpmRsrcStoreAndSendMsg)

   /* update outgoing message statistics */
   /* czUtlUpdOutSts(rsrcCb->peerCb, msgType); */

   /* Invoke TPM to send the message to peer */
   ret = czTpmStoreAndSendMsg((*rsrcCb)->peerCb, &(event->pdu), 
                              (*rsrcCb)->peerCb->peerCfg.globalStreamId,
                              &((*rsrcCb)->retxBuf));
   if(ret != ROK)
   {
      CZ_DBG_FATAL((CZ_PRNT_BUF, "Trasmission of the Resource message failed .\n" ));
      CZ_SEND_LCL_RSRCPROC_ERR((*rsrcCb)->peerCb, 
          CZT_ERRTYPE_MSG_SEND_FAILED, 
          ret, 
          msgType, (*rsrcCb)->eNb1MeasId, 
          (*rsrcCb)->eNb2MeasId, event, NULLP); 

      /* czUtlUpdOutErrSts(*rsrcCb->peerCb, msgType); */

      if (isNewCb) 
      { 
         CZ_DBG_INFO((CZ_PRNT_BUF, "Deleting Resource Cb .\n" ));
         czNmmDelRsrcCb(*rsrcCb); 
         *rsrcCb = NULLP;
      } 
   }
   else
   {
      CZ_DBG_INFO((CZ_PRNT_BUF, "Resource message Stored and Sent .\n" ));
      CZ_FREEEVNT(event);
   }

   CZ_RETVALUE(ret);

} /* czGpmRsrcStoreAndSendMsg */

/**
   @brief The function is used for sending Resource Status messages received 
          from the peer to the user.
   @details The function czGpmSndRsrcInd is used for sending any Initiating 
          Resource Status messages received from the peer to the user.
   @param[in] peerCb The peerCb is the control block to the peer on whose
               X2-interface the message is exchanged.
   @param[in] rsrcCb The control block related to Resource Status Reporting 
              procedure in the context of the peer on whose X2-interface 
              the message is exchanged.
   @param[in] event The pdu of the X2 Message that is exchanged.
   @param[in] msgType The X2 Message type exchanged.
   @param[in] isNewCb A boolean argument to indicate whether to delete the Cb.
   @param[in] rqst The registration request sent in the message.
   @return
        - ROK     - If the procedure was successful.
        - RFAILED - If the procedure was unsuccessful due to any failure.
*/
#ifdef ANSI
PRIVATE S16 czGpmSndRsrcInd
(
CzPeerCb* peerCb, 
CzRsrcCb** rsrcCb, 
CztEvnt*  event, 
U8       msgType, 
Bool      isNewCb, 
EnumCztRegistration_Rqst rqst
)
#else
PRIVATE S16 czGpmSndRsrcInd(peerCb, rsrcCb, event, msgType, isNewCb, rqst)
CzPeerCb* peerCb;
CzRsrcCb** rsrcCb;
CztEvnt*  event; 
U8       msgType; 
Bool      isNewCb; 
EnumCztRegistration_Rqst rqst;
#endif
{
   CztEvnt* errEvnt = NULLP; 
   CzRsrcInfo rsrcInfo;
   S16 ret = ROK;

   CZ_TRC2(czGpmSndRsrcInd)
   CZ_PRNT_PEERID;

   /* update incoming message statistics */
   /* czUtlUpdInSts(peerCb, msgType); */

   CZ_DBG_INFO((CZ_PRNT_BUF, "Sending Resource Message Indication to the user .\n" ));
   /* Invoke UIM to send the indication to user */
   ret = czUimSndGpmInd(peerCb, event);
   if(ret != ROK)
   {
      if (msgType == CZT_MSG_RSRC_ST_REQ)
      {
         if (rqst == CztRegistration_RqststopEnum)
         {
            /* Fill the rsrcInfo with the eNB measurement IDs */
            rsrcInfo.eNb1measId = (*rsrcCb)->eNb1MeasId;
            rsrcInfo.eNb2measId = (*rsrcCb)->eNb2MeasId;
            CZ_DBG_INFO((CZ_PRNT_BUF, "Resource Cb Info. eNB1MeasID = %d, \
                     eNB2MeasID = %d .\n", rsrcInfo.eNb1measId,
                     rsrcInfo.eNb2measId ));
            /* Fill the Resource Status Failure event with the information */
            CZ_DBG_FATAL((CZ_PRNT_BUF, "Indication of the incoming X2-Resource \
                     Status Request to Stop Registration failed. \
                     Sending X2-Resource Status Failure. \n" ));
            errEvnt = czUtlGenRsrcStsFail(NULLP, 
                       &rsrcInfo, 
         /* cz001.301: Fix for the build warnings */
#ifdef CZTV1
                       CZT_CAUSE_MISC, 
#else
                       CAUSE_MISC, 
#endif
                       CztCauseMiscom_interventionEnum); 
         }
         else
         {
            CZ_DBG_FATAL((CZ_PRNT_BUF, "Indication of the incoming X2-Resource \
                     Status Request failed. Sending X2-Error Indication.\n" ));
            errEvnt = czUtlGenPeerErrInd(&event->pdu, msgType, NULLP,
         /* cz001.301: Fix for the build warnings */
#ifdef CZTV1
                       CZT_CAUSE_MISC, 
#else
                       CAUSE_MISC, 
#endif
                       CztCauseMiscom_interventionEnum); 
         }
      } /* if (msgType == CZT_MSG_RSRC_ST_REQ) */
      else
      {
         CZ_DBG_FATAL((CZ_PRNT_BUF, "Indication of the incoming X2-Resource \
                  Status Message failed. Sending X2-Error Indication.\n" ));
         errEvnt = czUtlGenPeerErrInd(&event->pdu, msgType, NULLP,
         /* cz001.301: Fix for the build warnings */
#ifdef CZTV1
                       CZT_CAUSE_MISC, 
#else
                       CAUSE_MISC, 
#endif
                       CztCauseMiscom_interventionEnum); 
      } /* else */
      /***
       * If we have a error indication buffer send it to 
       * peer using TPM and free the event structure
       */
      if (errEvnt)
      {
         czTpmSndToLower(peerCb, 
                         &(errEvnt->pdu),
                         peerCb->peerCfg.globalStreamId);
         CZ_FREEEVNT(errEvnt);
      }
      /* update error stats for inbound messages */
      /* czUtlUpdInErrSts(peerCb, msgType); */

      if (isNewCb)
      {
         CZ_DBG_INFO((CZ_PRNT_BUF, "isNewCb. Deleting RsrcCb .\n" ));
         /* The NULLP assignment ihas to be reflected in calling function
          * hence it is taken as double pointer and updated. */
         czNmmDelRsrcCb(*rsrcCb);
         *rsrcCb = NULLP;
      }
      CZ_FREEEVNT(event);

      CZ_DBG_ERROR((CZ_PRNT_BUF, "Sending %s to user failed",
                    ((msgType == CZT_MSG_RSRC_ST_UPD)?"Resource Status Update":
                    ((CztRegistration_RqststartEnum == rqst)?
                    ("Resource Status Request START "):
                    ("Resource Status Request STOP ")))));
   } /* if(ret != ROK) */
   
   CZ_RETVALUE(ret);

} /* czGpmSndRsrcInd */

/**
   @brief The function is used for sending Resource Status Response messages
          to the user.
   @details The function czGpmSndRsrcCfm is used for sending the Resource status
          response messages received from the peer to the user.
   @param[in] peerCb The peerCb is the control block to the peer on whose
               X2-interface the message is exchanged.
   @param[in] event The pdu of the X2 Message that is exchanged.
   @param[in] msgType The X2 Message type exchanged.
   @return
        - ROK     - If the procedure was successful.
        - RFAILED - If the procedure was unsuccessful due to any failure.
*/
#ifdef ANSI
PRIVATE S16 czGpmSndRsrcCfm
(
CzPeerCb* peerCb, 
CztEvnt*  event, 
U8       msgType
)
#else
PRIVATE S16 czGpmSndRsrcCfm(peerCb, event, msgType)
CzPeerCb* peerCb;
CztEvnt*  event; 
U8       msgType; 
#endif
{
   S16 ret = ROK;
   CztEvnt* errEvnt = NULLP;

   CZ_TRC2(czGpmSndRsrcCfm)
   CZ_PRNT_PEERID;
   CZ_DBG_INFO((CZ_PRNT_BUF, "Sending Resource Message Confirmation to the user .\n" ));

   /* update incoming message statistics */
   /* czUtlUpdInSts(peerCb, msgType); */

   /* Invoke UIM to send the confirmation */
   ret = czUimSndGpmCfm(peerCb, event);
   if(ret != ROK)
   {
      CZ_DBG_FATAL((CZ_PRNT_BUF, "Indicating X2-Resource status %s to user failed",
                    (msgType == CZT_MSG_RSRC_ST_RSP)?"response":"failure.\
                    Sending X2-Error Indication to Peer."));
      /* Generate an Error indication towards the peer */
      errEvnt = czUtlGenPeerErrInd(&event->pdu, msgType, NULLP,
         /* cz001.301: Fix for the build warnings */
#ifdef CZTV1
                       CZT_CAUSE_MISC, 
#else
                       CAUSE_MISC, 
#endif
                       CztCauseMiscom_interventionEnum); 
      if (errEvnt)
      {
         /* Send the error indication using TPM */
         czTpmSndToLower(peerCb, &(errEvnt->pdu), 
                         peerCb->peerCfg.globalStreamId);
         CZ_FREEEVNT(errEvnt);
      }
      CZ_FREEEVNT(event);
      /* czUtlUpdInErrSts(peerCb, msgType); */
   }
   
   CZ_RETVALUE(ret);

} /* czGpmSndRsrcCfm */


/**
   @brief The function handles a Resource Status Request for an existing eNB1
          measurement ID.
   @details 
   - If the Registration request is "stop" and if rsrcCb state is either
   CZ_RSRC_READY or CZ_RSRC_STOP_PROGRESS, its a valid message. Then
   based on the message direction it is sent to either user or peer, after
   changing the state to CZ_RSRC_STOP_PROGRESS. 
   - If the Registration request is "start" then, if the message direction is IN 
   Resource Status Failure is sent to the peer with cause value set to 
   "Existing Measurement ID". If the message direction is OUT, local error
   indication is sent to user.
   @param[in] rsrcCb The control block related to Resource Status Reporting 
              procedure.
   @param[in] event The pdu of the X2 Message that is exchanged.
   @param[in] rsrcInfo The information related to the Resource Status Reporting.
   @param[in] regnRqstIE The registration type for Resource Update Request (start/stop).
   @param[in] dir The direction in which the X2 Message is exchanged.
   @return
        - ROK     - If the procedure was successful.
        - RFAILED - If the procedure was unsuccessful due to any failure.
*/
#ifdef ANSI
PRIVATE S16 czGpmHdlOldRsrcCb 
(
CzRsrcCb       *rsrcCb, 
CztEvnt        *event,
CzRsrcInfo*    rsrcInfo,
CztRegistration_Rqst *regnRqstIE,
U8             dir
)
#else
PRIVATE S16 czGpmHdlOldRsrcCb (rsrcCb, event, rsrcInfo, regnRqstIE, dir)
CzRsrcCb      *rsrcCb;
CztEvnt       *event;
CzRsrcInfo*   rsrcInfo;
CztRegistration_Rqst *regnRqstIE;
U8            dir;
#endif
{

   S16 ret = ROK;
   U16 prevState;
   CztMeasurement_ID *eNb2MeasId = NULLP;
   U8 causeType;
   U8 causeVal;
   CZ_TRC2(czGpmHdlOldRsrcCb)

   /***
    * If we are in reporting state and received a "stop" request then
    * we need to change the state to CZ_RSRC_STOP_PROGRESS and
    * send the message to user or peer based on the message direction
    */
   if ((CztRegistration_RqststopEnum == regnRqstIE->val) &&
       ((rsrcCb->state == CZ_RSRC_READY) || 
        (rsrcCb->state == CZ_RSRC_STOP_PROGRESS)))
   {
      /***
       * Save the present state for roll back and
       * Change the rsrc state to Resource Status Request received
       */
      prevState = rsrcCb->state;
      rsrcCb->state = CZ_RSRC_STOP_PROGRESS;
        
      CZ_DBG_INFO((CZ_PRNT_BUF, "Resourse Cb State changed to \
               CZ_RSRC_STOP_PROGRESS.\n" ));
      /***
       * If incoming send it to user
       */
      if (CZ_DIR_IN == dir)
      {
         CZ_DBG_INFO((CZ_PRNT_BUF, "Receiving X2-Resource Status Update Request .\n" ));
         ret = czGpmSndRsrcInd(rsrcCb->peerCb, &rsrcCb, event, 
                               CZT_MSG_RSRC_ST_REQ, FALSE, 
                               CztRegistration_RqststopEnum);
      } /* if incoming message */
      /***
       * If outgoing send it to peer using TPT
       */
      else
      {
         CZ_DBG_INFO((CZ_PRNT_BUF, "Sending X2-Resource Status Update Request .\n" ));
         ret = czGpmRsrcStoreAndSendMsg(&rsrcCb, event, CZT_MSG_RSRC_ST_REQ, 
                                        FALSE);
      } /* if outgoing message */


      /***
       * If the message sending fails roll back to previous state 
       */
      if (ret != ROK)
      {
         rsrcCb->state = (CzRsrcState)prevState;
      }

      CZ_RETVALUE(ret);

   } /* if stop request/retransmission */
   else if ((CztRegistration_RqststartEnum == regnRqstIE->val) &&
            (rsrcCb->state == CZ_RSRC_START_PROGRESS) &&
            (CZ_DIR_IN == dir))

   {
      CZ_DBG_INFO((CZ_PRNT_BUF, "Retransmission of X2-Resource status \
               Request with Registration as start.\n" ));
      ret = czGpmSndRsrcInd(rsrcCb->peerCb, &rsrcCb, event, CZT_MSG_RSRC_ST_REQ, 
                            TRUE, CztRegistration_RqststartEnum);
      CZ_RETVALUE(ret);
   } /* if start retransmission */

   /* Get the eNB2 Measurement ID for reporting error */
   czUtlGetIE(&event->pdu, Cztid_ENB2_Measurement_ID, (TknU8 **)&eNb2MeasId);
   if (rsrcCb->eNb2MeasId != CZ_UNKNOWN_MEAS_ID)
   {
      CZ_DBG_INFO((CZ_PRNT_BUF, "Found eNB2 Measurement Id.\n" ));
      rsrcInfo->eNb2measId = rsrcCb->eNb2MeasId;
      CZ_DBG_INFO((CZ_PRNT_BUF, "Resource Cb Info. eNB1MeasID = %d, \
               eNB2MeasID = %d .\n", rsrcCb->eNb1MeasId, rsrcCb->eNb2MeasId ));
   }
   
   /***
    * Set the cause type to RADIONW and value to ExistingMeasurementID
    */
   if (CZ_DIR_IN == dir)
   {
      /*fixed for compilation in g++*/
      if(CztRegistration_RqststartEnum == regnRqstIE->val)
        {
           causeType = CAUSE_RADIONW;
           causeVal  =  CztCauseRadioNwexistingMeasurementIDEnum;
        }
      else
        {
         /* cz001.301: Fix for the build warnings */
#ifdef CZTV1
          causeType = CZT_CAUSE_PROTOCOL;
#else
          causeType = CAUSE_PROTOCOL;
#endif
          causeVal  = CztCauseProtmessage_not_compatible_with_receiver_stateEnum;
        } 
      CZ_DBG_ERROR((CZ_PRNT_BUF, "Sending X2-Resource Status Update failure. .\n" ));
      czGpmSndRsrcErr(rsrcCb->peerCb, event, rsrcInfo, CZT_MSG_RSRC_ST_REQ,causeType,causeVal);
   }
   CZ_SEND_LCL_RSRCPROC_ERR(rsrcCb->peerCb, 
       ((CZ_DIR_IN == dir)?(CZT_ERRTYPE_INV_PARAM):(CZT_ERRTYPE_INV_SDU_PARAM)), 
       ((CztRegistration_RqststartEnum == regnRqstIE->val)?
       (CZT_ERRCAUSE_DUPLICATE_ENB1_MEAS_ID):
       (CZT_ERRCAUSE_UNEXP_MSG)),
       CZT_MSG_RSRC_ST_REQ, rsrcInfo->eNb1measId, rsrcInfo->eNb2measId, 
       event, NULLP);

      CZ_DBG_ERROR((CZ_PRNT_BUF, "Unexpected message received.\n" ));
   CZ_RETVALUE(RFAILED);

} /* czGpmHdlOldRsrcCb */

/**
   @brief The function  czGpmHdlNewRsrcCb handles a new Resource Status Request.
   @details 
   - If the Registration request is "start", The new rsrcCb is added to the 
   resrcCp hash list cp in peerCb. 
      If the message direction is IN then the message is sent up to the user.
      Else If the message direction is OUT the message is sent to the peer.<br>
   - If the Registration request is "stop" then, 
      If the direction is IN then Resource Status Failure is sent to the 
      peer with cause value set to "Unknown Measurement ID". <br>
      Else If the message direction is OUT, local error indication is sent to user.
      <br>
   @param[in] peerCb The peerCb is the control block to the peer on whose
               X2-interface the message is exchanged.
   @param[in] event The pdu of the X2 Message that is exchanged.
   @param[in] regnRqstIE The registration type for Resource Update Request(start/stop).
   @param[in] eNb1MeasId The eNB1 Measurement ID for the request.
   @param[in] dir The direction in which the X2 Message is exchanged.
   @return
        - ROK     - If the procedure was successful.
        - ROUTRES - If the procedure was unsuccessful due to resource crunch.
        - RFAILED - If the procedure was unsuccessful due to any failure.
*/
#ifdef ANSI
PRIVATE S16 czGpmHdlNewRsrcCb 
(
CzPeerCb     *peerCb, 
CztEvnt      *event,
CztRegistration_Rqst *regnRqstIE,
U16          eNb1MeasId,
U8           dir
)
#else
PRIVATE S16 czGpmHdlNewRsrcCb (peerCb, event, regnRqstIE, eNb1MeasId, dir)
CzPeerCb     *peerCb;
CztEvnt      *event;
CztRegistration_Rqst *regnRqstIE;
U16          eNb1MeasId;
U8           dir;
#endif
{

   S16 ret = ROK;
   U16 prevState;
   CzRsrcCb* rsrcCb = NULLP;
   U32 rsrcStsCbKey = 0;
   CztMeasurement_ID *eNb2MeasId = NULLP;
   CztEvnt* rsrcStsFailEvnt = NULLP;
   CzRsrcInfo rsrcInfo = { CZ_UNKNOWN_MEAS_ID, CZ_UNKNOWN_MEAS_ID};

   CZ_TRC2(czGpmHdlNewRsrcCb)
   CZ_PRNT_PEERID;

   /***
    * Check if registration request is "start" or not.
    */
   if (CztRegistration_RqststartEnum == regnRqstIE->val)
   {
      CZ_DBG_INFO((CZ_PRNT_BUF, "Registration type is START .\n" ));
      /***
       * Create a new key with the eNb1MeasId and dir
       */
      CZ_MAKE_RSRC_KEY(rsrcStsCbKey, eNb1MeasId, CZ_RSRC_REQ, dir); 
#ifdef ALIGN_64BIT
      CZ_DBG_INFO((CZ_PRNT_BUF, "Created new resource key %d .\n", rsrcStsCbKey));
#else
      CZ_DBG_INFO((CZ_PRNT_BUF, "Created new resource key %ld .\n", rsrcStsCbKey));
#endif
   
      /***
       * create a new rsrcCb
       */
      czNmmAddRsrcCb(peerCb, rsrcStsCbKey, &rsrcCb, eNb1MeasId);
      if (!rsrcCb)
      {
         rsrcInfo.eNb1measId = eNb1MeasId;
         if (CZ_DIR_IN == dir)
         {
            czGpmSndRsrcErr(peerCb, event, &rsrcInfo, CZT_MSG_RSRC_ST_REQ, 
         /* cz001.301: Fix for the build warnings */
#ifdef CZTV1
                       CZT_CAUSE_MISC, 
#else
                       CAUSE_MISC, 
#endif
               CztCauseMiscom_interventionEnum);
         }
         CZ_SEND_LCL_RSRCPROC_ERR(peerCb, 
               CZT_ERRTYPE_SYS, 
               CZT_ERRCAUSE_RSRC_CB_CREATE_FAILED,
               CZT_MSG_RSRC_ST_REQ, 
               rsrcInfo.eNb1measId, CZ_UNKNOWN_MEAS_ID,
               event, NULLP);
         CZ_DBG_FATAL((CZ_PRNT_BUF, "Failed to add Resource Status Cb for"\
                       " Measurment Id: %d", eNb1MeasId));
         CZ_RETVALUE(ROUTRES);
      } /* if adding rsrcCb failed */
      CZ_DBG_INFO((CZ_PRNT_BUF, "Resource Cb Info. eNB1MeasID = %d, \
               eNB2MeasID = %d .\n", rsrcCb->eNb1MeasId, rsrcCb->eNb2MeasId ));

      prevState = rsrcCb->state;
      rsrcCb->state = CZ_RSRC_START_PROGRESS;
      CZ_DBG_INFO((CZ_PRNT_BUF, "State changed to CZ_RSRC_START_PROGRESS.\n" ));
   
      /***
       * If incoming message sent id up to the user
       */
      if (CZ_DIR_IN == dir)
      {
         CZ_DBG_INFO((CZ_PRNT_BUF, "Incoming X2-Resource Status Request Message.\n" ));
         ret = czGpmSndRsrcInd(peerCb, &rsrcCb, event, CZT_MSG_RSRC_ST_REQ, 
                               TRUE, CztRegistration_RqststartEnum);
      } /* if incoming */
      /***
       * If outgoing message send the message to peer
       */
      else
      {
         CZ_DBG_INFO((CZ_PRNT_BUF, "Outing X2-Resource Status Request Message.\n" ));
         ret = czGpmRsrcStoreAndSendMsg(&rsrcCb, event, CZT_MSG_RSRC_ST_REQ, 
                                TRUE);
      } /* if outgoing */

      /***
       * If the message sending fails roll back to previous state.
       * The new rsrcCb will be deleted incase of failure
       * hence check for NULLP
       */
      /* cz001.101 : Memory leak fix. */
      if ((ret != ROK) && (rsrcCb != NULLP))
      {
         CZ_DBG_ERROR((CZ_PRNT_BUF, "Error Occured. Roll back to previous state \
                  %d.\n", prevState ));
         rsrcCb->state = (CzRsrcState)prevState;
      }

   }
   else
   {
      CZ_DBG_INFO((CZ_PRNT_BUF, "Registration type is STOP .\n" ));
      /***
       * It is not a "start" request, obtain the eNB2 Measurement ID as well
       */
      czUtlGetIE(&event->pdu, Cztid_ENB2_Measurement_ID, (TknU8 **)&eNb2MeasId);
   
      /***
       * If the message is incoming send a error response back to peer.
       */
      if (CZ_DIR_IN == dir)
      {
         /* Fill the rsrcInfo with the eNB measurement IDs */
         rsrcInfo.eNb1measId = eNb1MeasId;
         if (eNb2MeasId)
         {
            /*kworks fix: type cast*/
            rsrcInfo.eNb2measId = (U16) eNb2MeasId->val;
         }
         CZ_DBG_INFO((CZ_PRNT_BUF, "Resource Cb Info. eNB1MeasID = %d, \
                  eNB2MeasID = %d .\n", rsrcInfo.eNb1measId, rsrcInfo.eNb2measId ));

         CZ_DBG_ERROR((CZ_PRNT_BUF, "X2-Resource Status Request received with \
                  unknown eNB ID %d. Sending X2-Resource Status \
                  Failure message to peer.\n", eNb1MeasId));
         /* Fill the Resource Status Failure event with the known information */
         rsrcStsFailEvnt = czUtlGenRsrcStsFail(NULLP, 
                    &rsrcInfo, 
                    CAUSE_RADIONW, 
                    CztCauseRadioNwunknown_eNB_Measurement_IDEnum); 
         /***
          * If we have a Resource Status Failure event built,
          * Invoke TPM to send it to lower layer 
          */
         if (rsrcStsFailEvnt) 
         { 
            czTpmSndToLower(peerCb, &(rsrcStsFailEvnt->pdu), 
                  peerCb->peerCfg.globalStreamId); 
            CZ_FREEEVNT(rsrcStsFailEvnt); 
         } 
      }
      /***
       * send a local error indication to the user in both directions.
       */
      CZ_SEND_LCL_RSRCPROC_ERR(peerCb, 
         ((CZ_DIR_IN == dir)?(CZT_ERRTYPE_INV_PARAM):(CZT_ERRTYPE_INV_SDU_PARAM)), 
         CZT_ERRCAUSE_INV_ENB1_MEAS_ID, 
         CZT_MSG_RSRC_ST_REQ, 
         rsrcInfo.eNb1measId, 
         rsrcInfo.eNb2measId, 
         event,
         NULLP);
      CZ_RETVALUE(RFAILED);
   }

   CZ_RETVALUE(ret);

} /* czGpmHdlNewRsrcCb */

/**
    @brief The function handles Resource Status Request in CZ_GP_CONNECTED state.
    @details 
   The message is checked for sanity with respect to the different IEs,
   If any of the IEs have semantic errors, error indication is sent 
   to user/peer based on message direction. <br>
        - If the message direction is IN, the eNB1 measurements ID is
          checked in rsrcStsCb hash list, if it is not present the message
          is passed to the user, and an entry is added for the
          measurement received, enb is made CZ_ENB2 and state changed 
          to CZ_RSRC_STS_REQ_RCVD. <br>
   If hash list entry is found and Registration request is "stop", 
   the entry is deleted and message passed to user, if the Registration 
   request is "start", then Resource Status Failure is sent to the peer with 
   cause value set to "Existing Measurement ID". <br>
        - If the message direction is OUT, it is checked in the
        rsrcStsCb, if not found, it is added to the hash list, and enb
        is made CZ_ENB1. If the measurement ID exists and the state is
        CZ_RSRC_STS_REQ_SENT then the message is for a retransmission. <br>
   All the other cases are considered failure and an appropriate message
   is sent to user/peer. <br>
    @param[in] peerCb The peerCb is the control block to the peer on whose
               X2-interface the message is exchanged.
    @param[in] msgType The X2 Message type that is exchanged.
    @param[in] event The pdu of the X2 Message that is exchanged.
    @param[in] dir The direction in which the X2 Message is exchanged.
    @return
        - ROK     - If the procedure was successful.
        - ROUTRES - If the procedure was unsuccessful due to resource crunch.
        - RFAILED - If the procedure was unsuccessful due to any failure.
*/
#ifdef ANSI
PUBLIC S16 czGpmHdlRsrcStsReq 
(
CzPeerCb* peerCb, 
U8        msgType,
CztEvnt*  event,
U8        dir
)
#else
PUBLIC S16 czGpmHdlRsrcStsReq (peerCb, msgType, event, dir)
CzPeerCb* peerCb;
U8        msgType;
CztEvnt*  event;
U8        dir;
#endif
{ 

   U32 rsrcStsCbKey = 0;
   CzRsrcCb* rsrcCb = NULLP;
   CztMeasurement_ID *eNB1MeasID = NULLP;
   CztRegistration_Rqst *regnRqstIE = NULLP;
   CzRsrcInfo rsrcInfo = {CZ_UNKNOWN_MEAS_ID, CZ_UNKNOWN_MEAS_ID};

   CZ_TRC2(czGpmHdlRsrcStsReq)
   CZ_PRNT_PEERID;

      CZ_DBG_INFO((CZ_PRNT_BUF, "Handling X2-Resource Status Request Message.\n" ));
   /***
    * Get eNB1 Measurement ID
    */
   czUtlGetIE(&event->pdu, Cztid_ENB1_Measurement_ID, (TknU8 **)&eNB1MeasID);
   /***
    * Get the registration request IE 
    */
   czUtlGetIE(&event->pdu, Cztid_Registration_Rqst,  (TknU8 **)&regnRqstIE);

   /***
    * In case of outgoing message check if the IEs are present or not
    */
   /*cz001.301: x2ap kwork fix*/
   if ((eNB1MeasID == NULL) || (regnRqstIE == NULL))
   {
      CZ_SEND_LCL_RSRCPROC_ERR(peerCb, CZT_ERRTYPE_INTERNAL, 
                   CZT_ERRCAUSE_MAND_PARAM_MISSING, CZT_MSG_RSRC_ST_REQ, 
                   CZ_UNKNOWN_MEAS_ID, CZ_UNKNOWN_MEAS_ID, event, NULLP);
      CZ_DBG_ERROR((CZ_PRNT_BUF, "Missing mandatory parameter"));
      CZ_RETVALUE(RFAILED);
   }
      rsrcInfo.eNb1measId = (U16)eNB1MeasID->val;
   /***
    * Obtain the key for searching rsrcCb from eNB1MeasurementID and direction
    **/
   CZ_MAKE_RSRC_KEY( rsrcStsCbKey, eNB1MeasID->val, CZ_RSRC_REQ, dir); 

   /***
    * Search for the rsrcStsCb in peerCb 
    **/
   czNmmFindRsrcCb(peerCb, rsrcStsCbKey, &rsrcCb);
   if (rsrcCb)
   {
      CZ_DBG_INFO((CZ_PRNT_BUF, "Resource Cb Info. eNB1MeasID = %d, \
               eNB2MeasID = %d .\n", rsrcCb->eNb1MeasId, rsrcCb->eNb2MeasId ));
      /***
       * Resource Cb found, handle the message for an existing
       * Resource Status Procedure
       */
      CZ_RETVALUE(czGpmHdlOldRsrcCb(rsrcCb, event, &rsrcInfo, regnRqstIE, dir));
   } /* if rsrcCb is found */
   /***
    * rsrcCb is not found, so It is a new eNB1 Measurement ID we need to
    * create the rsrcCb and process it appropriately based on direction.
    */
   else
   {
      /***
       * Resource Cb not found, handle the message for a new 
       * Resource Status Procedure
       */
      CZ_RETVALUE(czGpmHdlNewRsrcCb(peerCb, event, regnRqstIE, 
                      (eNB1MeasID->val), dir));
   } /* if rsrcCb is NOT found */

} /* czGpmHdlRsrcStsReq */

/**
    @brief The function handles Resource Status Response in CZ_CONNECTED state.
    @details 
   The message is checked for sanity with respect to the different IEs,
   If any of the IEs have semantic errors, error indication is sent 
   to user/peer based on message direction.
        - If the message direction is IN, the eNB1 measurements ID is
             checked in rsrcStsCb hash list, if it is not present Error
        Indication is sent to the peer.  If hash list entry
        is found, enb is CZ_ENB1 and state is CZ_RSRC_STS_REQ_SENT, state is changed to CZ_RSRC_READY and the message is passed to the user.
        - If the message direction is OUT, it is checked in the
        rsrcStsCb, if not found, and error is returned to user.
        If the measurement ID exists and the state is
        CZ_RSRC_STS_REQ_RCVD and enb is CZ_ENB2 then the state is changed to 
   CZ_RSRC_READY and the message is sent across
        to the peer, otherwise Error is returned to user.
    @param[in] peerCb The peerCb is the control block to the peer on whose
               X2-interface the message is exchanged.
    @param[in] msgType The X2 Message type that is exchanged.
    @param[in] event The pdu of the X2 Message that is exchanged.
    @param[in] dir The direction in which the X2 Message is exchanged.
    @return
        - ROK     - If the procedure was successful.
        - RFAILED - If the procedure was unsuccessful due to any failure.
*/
#ifdef ANSI
PUBLIC S16 czGpmHdlRsrcStsRsp 
(
CzPeerCb* peerCb, 
U8        msgType,
CztEvnt*  event, 
U8        dir
)
#else
PUBLIC S16 czGpmHdlRsrcStsRsp (peerCb, msgType, event, dir)
CzPeerCb* peerCb;
U8        msgType;
CztEvnt*  event; 
U8        dir;
#endif
{

   U32 rsrcStsCbKey = 0;
   CzRsrcCb* rsrcCb = NULLP;
   S16 ret = ROK;
   U16 prevState;
   CztMeasurement_ID *eNB1MeasID = NULLP;
   CztMeasurement_ID *eNB2MeasID = NULLP;
   CzRsrcInfo rsrcInfo = {CZ_UNKNOWN_MEAS_ID, CZ_UNKNOWN_MEAS_ID};

   CZ_TRC2(czGpmHdlRsrcStsRsp)
   CZ_PRNT_PEERID;
   CZ_DBG_INFO((CZ_PRNT_BUF, "Handling X2-Resource Status Request Message.\n" ));

   /***
    * Get eNB1, eNB2 Measurement IDs
    */
   czUtlGetIE(&event->pdu, Cztid_ENB1_Measurement_ID,  (TknU8 **)&eNB1MeasID);
   czUtlGetIE(&event->pdu, Cztid_ENB2_Measurement_ID,  (TknU8 **)&eNB2MeasID);
   /*cz001.301: x2ap kwork fix*/
   if ((eNB1MeasID == NULL) || (eNB2MeasID == NULL))
   {
      CZ_DBG_ERROR((CZ_PRNT_BUF, "Mandatory eNB Measurement ID not present "\
                    "in incoming X2-Resource Status Response message"));
      CZ_SEND_LCL_RSRCPROC_ERR(peerCb, CZT_ERRTYPE_INTERNAL, 
                   CZT_ERRCAUSE_MAND_PARAM_MISSING, CZT_MSG_RSRC_ST_RSP, 
                   rsrcInfo.eNb1measId, rsrcInfo.eNb2measId, event, NULLP);
      CZ_RETVALUE(RFAILED);
   }

   /* save the measurement IDs for reporting erros if any */
      rsrcInfo.eNb1measId = (U16)eNB1MeasID->val;
      rsrcInfo.eNb2measId = (U16)eNB2MeasID->val;

   CZ_DBG_INFO((CZ_PRNT_BUF, "Resource Cb Info. eNB1MeasID = %d, \
            eNB2MeasID = %d .\n", rsrcInfo.eNb1measId, rsrcInfo.eNb2measId ));
   /***
    * Obtain eNB1MeasurementID as the key for searching rsrcCb 
    **/
   CZ_MAKE_RSRC_KEY( rsrcStsCbKey, eNB1MeasID->val, CZ_RSRC_RSP, dir); 

   /***
    * Search for the rsrcStsCb in peerCb 
    **/
   czNmmFindRsrcCb(peerCb, rsrcStsCbKey, &rsrcCb);

   if (rsrcCb)
   {
      CZ_DBG_INFO((CZ_PRNT_BUF, "Resource Cb Info. eNB1MeasID = %d, \
               eNB2MeasID = %d .\n", rsrcCb->eNb1MeasId, rsrcCb->eNb2MeasId ));

      /***
       * save the current state for rolling back in case of processing errors
       */
      prevState = rsrcCb->state;

      /***
       * If we are in CZ_RSRC_START_PROGRESS state and received a response then
       * we need to change the state to CZ_RSRC_READY and
       * send the message to user or peer based on the message direction
       */
      if (rsrcCb->state == CZ_RSRC_START_PROGRESS)
      {

         /***
          * Obtain and save the eNB2 measurement ID and
          * Change the rsrc state to CZ_RSRC_READY 
          */
         rsrcCb->eNb2MeasId = eNB2MeasID->val;
         rsrcCb->state = CZ_RSRC_READY;
         CZ_DBG_INFO((CZ_PRNT_BUF, "State changed from CZ_RSRC_START_PROGRESS \
                  to CZ_RSRC_READY.\n" ));
        
         CZ_DBG_INFO((CZ_PRNT_BUF, "Resource Cb Info. eNB1MeasID = %d, \
                  eNB2MeasID = %d .\n", rsrcCb->eNb1MeasId, rsrcCb->eNb2MeasId ));
         /***
          * If incoming send it to user
          */
         if (CZ_DIR_IN == dir)
         {
            if(rsrcCb->retxBuf != NULLP)
            {
               CZ_FREEMBUF(rsrcCb->retxBuf);
            }
            ret = czGpmSndRsrcCfm(peerCb, event, CZT_MSG_RSRC_ST_RSP);
            /***
             * If the message sending fails roll back to previous state 
             */
            if (ret != ROK)
            {
               rsrcCb->state = (CzRsrcState)prevState;
            }
         } /* if incoming message */
         /***
          * If outgoing send it to peer using TPT
          */
         else
         {
            ret = czGpmSndRsrcMsgToPeer(peerCb, rsrcCb->eNb1MeasId, 
                  rsrcCb->eNb2MeasId, event, 
                  CZT_MSG_RSRC_ST_RSP);
            /***
             * If the message sending fails roll back to previous state 
             */
            if (ret != ROK)
            {
               rsrcCb->state = (CzRsrcState)prevState;
            }
         } /* if outgoing message */


         CZ_RETVALUE(ret);

      } /* if we are in REQ */
      /***
       * If we are in RPTNT_REQ_RCVD then we are waiting for an acknowledgement
       * for a "stop"
       */
      else if (rsrcCb->state == CZ_RSRC_STOP_PROGRESS)
      {
         CZ_DBG_INFO((CZ_PRNT_BUF, "Response for X2-Resource Status Request to \
                  STOP. Deleting the Resource Cb.\n" ));
         /***
          * Delete the resource Cb, Resource Status Update 
          * for this measurement ID has to be stopped.
          */
         /* cz001.101 : Memory leak fix. */
         czNmmDelRsrcCb(rsrcCb);
         rsrcCb = NULLP;

         /***
          * If incoming send it to user
          */
         if (CZ_DIR_IN == dir)
         {
            ret = czGpmSndRsrcCfm(peerCb, event, CZT_MSG_RSRC_ST_RSP);
         } /* if incoming message */
         /***
          * If outgoing send it to peer using TPT
          */
         else
         {
            ret = czGpmSndRsrcMsgToPeer(peerCb, eNB1MeasID->val, 
                           eNB2MeasID->val, event, CZT_MSG_RSRC_ST_RSP);
         } /* if outgoing message */

         CZ_RETVALUE(ret);
      } /* if response for "stop" */
   
      if (CZ_DIR_IN == dir)
      {
         czGpmSndRsrcErr(peerCb, event, &rsrcInfo, 
            CZT_MSG_RSRC_ST_RSP, 
         /* cz001.301: Fix for the build warnings */
#ifdef CZTV1
            CZT_CAUSE_PROTOCOL,
#else
            CAUSE_PROTOCOL,
#endif
            CztCauseProtmessage_not_compatible_with_receiver_stateEnum);
      }
      CZ_SEND_LCL_RSRCPROC_ERR(peerCb, CZT_ERRTYPE_INTERNAL, 
            CZT_ERRCAUSE_INV_STATE,
            CZT_MSG_RSRC_ST_RSP, 
            rsrcInfo.eNb1measId, rsrcInfo.eNb2measId, 
            event, NULLP);
#ifdef ALIGN_64BIT
      CZ_DBG_ERROR((CZ_PRNT_BUF, "Resource Status Response message received in \
         an invalid state, with Measurment ID1: %d, Measurement ID2: %d", 
         eNB1MeasID->val, eNB2MeasID->val)); 
#else
      CZ_DBG_ERROR((CZ_PRNT_BUF, "Resource Status Response message received in \
         an invalid state, with Measurment ID1: %ld, Measurement ID2: %ld", 
         eNB1MeasID->val, eNB2MeasID->val)); 
#endif
      /* We have processed this message */
      CZ_RETVALUE(ROK);

   } /* if rsrcCb is found */

   /***
    * If the rsrcCb is not found or RSRC_STS_RSP is received in an invalid state
    * then send an error indication to the user
    */
   if (CZ_DIR_IN == dir)
   {
      czGpmSndRsrcErr(peerCb, event, &rsrcInfo, 
         CZT_MSG_RSRC_ST_RSP, 
         CAUSE_RADIONW,
         CztCauseRadioNwunknown_eNB_Measurement_IDEnum);
   }
   CZ_SEND_LCL_RSRCPROC_ERR(peerCb, 
         ((CZ_DIR_IN == dir)?(CZT_ERRTYPE_INV_PARAM):(CZT_ERRTYPE_INV_SDU_PARAM)), 
         CZT_ERRCAUSE_INV_ENB1_MEAS_ID,
         CZT_MSG_RSRC_ST_RSP, 
         rsrcInfo.eNb1measId, rsrcInfo.eNb2measId, 
         event, NULLP);
#ifdef ALIGN_64BIT
   CZ_DBG_ERROR((CZ_PRNT_BUF, "Resource Status Response message received with \
   unknown Measurment ID1: %d, Measurement ID2: %d", 
   eNB1MeasID->val, eNB2MeasID->val)); 
#else
   CZ_DBG_ERROR((CZ_PRNT_BUF, "Resource Status Response message received with \
   unknown Measurment ID1: %ld, Measurement ID2: %ld", 
   eNB1MeasID->val, eNB2MeasID->val)); 
#endif
   CZ_RETVALUE(ROK);

} /* czGpmHdlRsrcStsRsp */

/**
    @brief The function handles Resource Status Failure in CZ_CONNECTED state.
    @details 
        - If the message direction is IN, the eNB1 measurements ID is
             checked in rsrcStsCb hash list, if it is not present Error
        Indication is sent to the peer.  if hash list entry
        is found the entry is deleted.
        - If the message direction is OUT, it is checked in the
        rsrcStsCb, if not found, local error indication is sent to
        user, If the measurement ID exists and the state is
        CZ_RSRC_STS_REQ_RCVD then the entry is deleted and the message is sent.
    @param[in] peerCb The peerCb is the control block to the peer on whose
               X2-interface the message is exchanged.
    @param[in] msgType The X2 Message type that is exchanged.
    @param[in] event The pdu of the X2 Message that is exchanged.
    @param[in] dir The direction in which the X2 Message is exchanged.
    @return
        - ROK     - If the procedure was successful.
        - RFAILED - If the procedure was unsuccessful due to any failure.
*/
#ifdef ANSI
PUBLIC S16 czGpmHdlRsrcStsFailr 
(
CzPeerCb* peerCb, 
U8        msgType,
CztEvnt*  event, 
U8        dir
)
#else
PUBLIC S16 czGpmHdlRsrcStsFailr (peerCb, msgType, event, dir)
CzPeerCb* peerCb; 
U8        msgType;
CztEvnt*  event; 
U8        dir;
#endif
{

   U32 rsrcStsCbKey = 0;
   U16 prevState;
   S16 ret = ROK;
   CzRsrcCb* rsrcCb = NULLP;
   CztMeasurement_ID *eNB1MeasID = NULLP;
   CztMeasurement_ID *eNB2MeasID = NULLP;
   CzRsrcInfo rsrcInfo = { CZ_UNKNOWN_MEAS_ID, CZ_UNKNOWN_MEAS_ID};

   CZ_TRC2(czGpmHdlRsrcStsFailr)
   CZ_PRNT_PEERID;
   CZ_DBG_INFO((CZ_PRNT_BUF, "Handling X2-Resource Status Failure Message.\n"));

   /***
    * Get eNB1 Measurement ID
    */
   czUtlGetIE(&event->pdu, Cztid_ENB1_Measurement_ID,  (TknU8 **)&eNB1MeasID);
   czUtlGetIE(&event->pdu, Cztid_ENB2_Measurement_ID,  (TknU8 **)&eNB2MeasID);
   /* cz001.301: x2ap kwork fix*/
   if ((eNB1MeasID == NULL) || (eNB2MeasID == NULL))
   {
      CZ_SEND_LCL_RSRCPROC_ERR(peerCb, CZT_ERRTYPE_INTERNAL, 
                   CZT_ERRCAUSE_MAND_PARAM_MISSING, CZT_MSG_RSRC_ST_FAIL, 
                   CZ_UNKNOWN_MEAS_ID, CZ_UNKNOWN_MEAS_ID, event, NULLP);
      CZ_DBG_ERROR((CZ_PRNT_BUF, "Mandatory eNB1 Measurement ID not present"\
                    "in Resource Status Failure message"));
      CZ_RETVALUE(RFAILED);
   }

   /* save the measurement IDs for reporting errors if any */
      rsrcInfo.eNb1measId =(U16) eNB1MeasID->val;
      rsrcInfo.eNb2measId =(U16) eNB2MeasID->val;

   CZ_DBG_INFO((CZ_PRNT_BUF, "Resource Cb Info. eNB1MeasID = %d, \
            eNB2MeasID = %d .\n", rsrcInfo.eNb1measId, rsrcInfo.eNb2measId ));

   /***
    * Obtain eNB1MeasurementID as the key for searching rsrcCb 
    **/
   CZ_MAKE_RSRC_KEY( rsrcStsCbKey, eNB1MeasID->val, CZ_RSRC_RSP, dir); 

   /***
    * Search for the rsrcStsCb in peerCb 
    **/
   czNmmFindRsrcCb(peerCb, rsrcStsCbKey, &rsrcCb);

   if (rsrcCb)
   {
      CZ_DBG_INFO((CZ_PRNT_BUF, "Resource Cb Info. eNB1MeasID = %d, \
               eNB2MeasID = %d .\n", rsrcCb->eNb1MeasId, rsrcCb->eNb2MeasId ));
      /***
       * Save the state for roll back
       */
      prevState = rsrcCb->state;

      /***
       * If we are in CZ_RSRC_START_PROGRESS state and received a failure then
       * send the message to user or peer based on the message direction
       * and delete the resource CB.
       * If the message is received in CZ_RSRC_STOP_PROGRESS state then
       * the same is sent across to user or peer, and state is be changed.
       */
      if ((rsrcCb->state == CZ_RSRC_START_PROGRESS) || 
          (rsrcCb->state == CZ_RSRC_STOP_PROGRESS))
      {

         /***
          * delete the rsrcCb because if we receive failure in 
          * CZ_RSRC_START_PROGRESS state, so Resource Status Reporting 
          * can not be initiated.
          */
         if (rsrcCb->state == CZ_RSRC_START_PROGRESS)
         {
            CZ_DBG_INFO((CZ_PRNT_BUF, "Resource Cb in CZ_RSRC_START_PROGRESS \
                     state. Deleting the Resource Cb\n" ));
            czNmmDelRsrcCb(rsrcCb);
            rsrcCb = NULLP;
         } /* if send failed */

         /*
          * If incoming send it to user
          */
         if (CZ_DIR_IN == dir)
         {
            ret = czGpmSndRsrcCfm(peerCb, event, CZT_MSG_RSRC_ST_FAIL);
         } /* if incoming message */
         /***
          * If outgoing send it to peer using TPT
          */
         else
         {
            ret = czGpmSndRsrcMsgToPeer(peerCb, eNB1MeasID->val,
                           eNB2MeasID->val, event, CZT_MSG_RSRC_ST_FAIL);
         } /* if outgoing message */

         /*
          * If sending fais roll back to previous state
          */
         /*kworks fix: checking prevState*/
         if ((ret != ROK) && (prevState == CZ_RSRC_STOP_PROGRESS))
         {
            rsrcCb->state = (CzRsrcState)prevState;
         }

         CZ_RETVALUE(ret);

      } /* if we are in REQ */
#ifdef ALIGN_64BIT
      CZ_DBG_ERROR((CZ_PRNT_BUF, "Resource Status Failure message received in \
      an invalid state, with Measurment ID1: %d, Measurement ID2: %d", 
         eNB1MeasID->val, eNB2MeasID->val)); 
#else
      CZ_DBG_ERROR((CZ_PRNT_BUF, "Resource Status Failure message received in \
      an invalid state, with Measurment ID1: %ld, Measurement ID2: %ld", 
         eNB1MeasID->val, eNB2MeasID->val)); 
#endif
      if (CZ_DIR_IN == dir)
      {
         czGpmSndRsrcErr(peerCb, event, &rsrcInfo, 
            CZT_MSG_RSRC_ST_FAIL, 
         /* cz001.301: Fix for the build warnings */
#ifdef CZTV1
            CZT_CAUSE_PROTOCOL,
#else
            CAUSE_PROTOCOL,
#endif
            CztCauseProtmessage_not_compatible_with_receiver_stateEnum);
      }
      CZ_SEND_LCL_RSRCPROC_ERR(peerCb, 
            CZT_ERRTYPE_INTERNAL,
            CZT_ERRCAUSE_INV_STATE,
            CZT_MSG_RSRC_ST_FAIL, 
            rsrcInfo.eNb1measId, rsrcInfo.eNb2measId, 
            event, NULLP);

      CZ_RETVALUE(RFAILED);

   } /* if rsrcCb is found */

   /*
    * If the rsrcCb is not found or RSRC_STS_FAIL is received in an invalid state
    * then send an error indication to the user
    */
   if (CZ_DIR_IN == dir)
   {
      czGpmSndRsrcErr(peerCb, event, &rsrcInfo, 
         CZT_MSG_RSRC_ST_FAIL, 
         CAUSE_RADIONW,
         CztCauseRadioNwunknown_eNB_Measurement_IDEnum);
   }
   CZ_SEND_LCL_RSRCPROC_ERR(peerCb, 
       ((CZ_DIR_IN == dir)?(CZT_ERRTYPE_INV_PARAM):(CZT_ERRTYPE_INV_SDU_PARAM)), 
                            CZT_ERRCAUSE_INV_ENB1_MEAS_ID,
                            CZT_MSG_RSRC_ST_FAIL, 
                            rsrcInfo.eNb1measId, rsrcInfo.eNb2measId, 
                            event, NULLP);

#ifdef ALIGN_64BIT
   CZ_DBG_ERROR((CZ_PRNT_BUF, "Resource Status Failure message received with \
   unknown Measurment ID1: %d, Measurement ID2: %d", 
      eNB1MeasID->val, eNB2MeasID->val)); 
#else
   CZ_DBG_ERROR((CZ_PRNT_BUF, "Resource Status Failure message received with \
   unknown Measurment ID1: %ld, Measurement ID2: %ld", 
      eNB1MeasID->val, eNB2MeasID->val)); 
#endif
   CZ_RETVALUE(ROK);

} /* czGpmHdlRsrcStsFailr */

/**
    @brief The function handles Resource Status Update in CZ_CONNECTED state.

    @details 
   The message is checked for sanity with respect to the different IEs,
   If any of the IEs have semantic errors, error indication is sent 
   to user/peer based on message direction.
        - If the message direction is IN, the eNB1 measurements ID is
             checked in rsrcStsCb hash list, if it is not present Error
        Indication is sent to the peer. if hash list entry
        is found and the state is CZ_RSRC_STS_READY the message is sent to user.
   Otherwise an error is sent to the peer.
        - If the message direction is OUT, it is checked in the
        rsrcStsCb, if not found, local error indication is sent to
        user, If the measurement ID exists and the state is
        CZ_RSRC_READY then the message is sent out. Otherwise an error is sent 
   to the user saying "invalid state".
    @param[in] peerCb The peerCb is the control block to the peer on whose
               X2-interface the message is exchanged.
    @param[in] msgType The X2 Message type that is exchanged.
    @param[in] event The pdu of the X2 Message that is exchanged.
    @param[in] dir The direction in which the X2 Message is exchanged.

    @return
        - ROK     - If the procedure was successful.
        - RFAILED - If the procedure was unsuccessful due to any failure.
*/
#ifdef ANSI
PUBLIC S16 czGpmHdlRsrcStsUpdt 
(
CzPeerCb* peerCb, 
U8        msgType,
CztEvnt*  event, 
U8        dir
)
#else
PUBLIC S16 czGpmHdlRsrcStsUpdt (peerCb, msgType, event, dir)
CzPeerCb* peerCb;
U8        msgType;
CztEvnt*  event; 
U8        dir;
#endif
{

   U32 rsrcStsCbKey = 0;
   S16 ret = ROK;
   CzRsrcCb* rsrcCb = NULLP;
   CztMeasurement_ID *eNB1MeasID = NULLP;
   CztMeasurement_ID *eNB2MeasID = NULLP;
   CzRsrcInfo rsrcInfo = {CZ_UNKNOWN_MEAS_ID, CZ_UNKNOWN_MEAS_ID};
   U8 causeType;
   U8 causeVal;

   CZ_TRC2(czGpmHdlRsrcStsUpdt)
   CZ_PRNT_PEERID;

   CZ_DBG_INFO((CZ_PRNT_BUF, "Handling X2-Resource Status Update Message.\n" ));
   /*
    * Get the Measurement IDs
    */
   czUtlGetIE(&event->pdu, Cztid_ENB1_Measurement_ID,  (TknU8 **)&eNB1MeasID);
   czUtlGetIE(&event->pdu, Cztid_ENB2_Measurement_ID,  (TknU8 **)&eNB2MeasID);
   /* cz001.301: x2ap kwork fix*/
   if ((eNB2MeasID == NULL) || (eNB1MeasID == NULL))
   {
      CZ_SEND_LCL_RSRCPROC_ERR(peerCb, CZT_ERRTYPE_INTERNAL, 
                   CZT_ERRCAUSE_MAND_PARAM_MISSING, CZT_MSG_RSRC_ST_UPD,
                   CZ_UNKNOWN_MEAS_ID, CZ_UNKNOWN_MEAS_ID, event, NULLP);

      CZ_DBG_ERROR((CZ_PRNT_BUF, "Mandatory eNB MeasID not present in"\
                    " Resource Status Update message"));
      CZ_RETVALUE(RFAILED);
   }

   /* save the measurement IDs for reporting erros if any */
      rsrcInfo.eNb1measId = (U16)eNB1MeasID->val;
      rsrcInfo.eNb2measId = (U16)eNB2MeasID->val;
   CZ_DBG_INFO((CZ_PRNT_BUF, "Resource Cb Info. eNB1MeasID = %d, \
            eNB2MeasID = %d .\n", rsrcInfo.eNb1measId, rsrcInfo.eNb2measId ));

   /*
    * Obtain eNB1MeasurementID as the key for searching rsrcCb 
    **/
   CZ_MAKE_RSRC_KEY( rsrcStsCbKey, eNB1MeasID->val, CZ_RSRC_RSP, dir); 

   /*
    * Search for the rsrcStsCb in peerCb 
    **/
   czNmmFindRsrcCb(peerCb, rsrcStsCbKey, &rsrcCb);

   if (rsrcCb)
   {
      CZ_DBG_INFO((CZ_PRNT_BUF, "Resource Cb Info. eNB1MeasID = %d, \
               eNB2MeasID = %d .\n", rsrcCb->eNb1MeasId, rsrcCb->eNb2MeasId ));
      /*
       * If we are in RPTNG state, just send the message to user or 
       * peer based on the message direction
       */
      if (rsrcCb->state == CZ_RSRC_READY)
      {

         CZ_DBG_INFO((CZ_PRNT_BUF, "State is CZ_RSRC_READY.\n" ));
         /*
          * If incoming send it to user
          */
         if (CZ_DIR_IN == dir)
         {
            ret = czGpmSndRsrcInd(peerCb, &rsrcCb, event, CZT_MSG_RSRC_ST_UPD, 
                                  FALSE, CztRegistration_RqststartEnum);
         } /* if incoming message */
         /*
          * If outgoing send it to peer using TPT
          */
         else
         {
            ret = czGpmSndRsrcMsgToPeer(peerCb, eNB1MeasID->val, 
                               eNB2MeasID->val, 
                               event, CZT_MSG_RSRC_ST_UPD);
         } /* if outgoing message */

         CZ_RETVALUE(ret);

      } /* if we are in REQ */

   } /* if rsrcCb is found */

   /*
    * If the rsrcCb is not found or RSRC_STS_UPD is received in an invalid state
    * then send an error indication to the user
    */
   if (CZ_DIR_IN == dir)
   {
        if (rsrcCb)
        {
       /* cz001.301: Fix for the build warnings */
#ifdef CZTV1
           causeType = CZT_CAUSE_PROTOCOL;
#else
           causeType = CAUSE_PROTOCOL;
#endif
           causeVal = CztCauseProtmessage_not_compatible_with_receiver_stateEnum;
        }
        else
        {
           causeType = CAUSE_RADIONW;
           causeVal = CztCauseRadioNwunknown_eNB_Measurement_IDEnum;
        }
        czGpmSndRsrcErr(peerCb, event, &rsrcInfo, CZT_MSG_RSRC_ST_UPD, 
                        causeType, causeVal);

        if (rsrcCb)
        {
           causeType = CZT_ERRTYPE_INTERNAL;
           causeVal = CZT_ERRCAUSE_INV_STATE;
        }
        else
        {
           causeType = CZT_ERRTYPE_INV_PARAM;
           causeVal = CZT_ERRCAUSE_INV_ENB1_MEAS_ID;
        }
         CZ_SEND_LCL_RSRCPROC_ERR(peerCb,causeType, causeVal,
                                  CZT_MSG_RSRC_ST_UPD, 
                                  rsrcInfo.eNb1measId, rsrcInfo.eNb2measId,
                                  event, NULLP);
   }
   else
   {
      if (rsrcCb)
      {
         causeType = CZT_ERRTYPE_INTERNAL;
         causeVal = CZT_ERRCAUSE_INV_STATE;
      }
      else
      {
         causeType = CZT_ERRTYPE_INV_SDU_PARAM;
         causeVal = CZT_ERRCAUSE_INV_ENB1_MEAS_ID;
      }
      CZ_SEND_LCL_RSRCPROC_ERR(peerCb, causeType, causeVal,
                               CZT_MSG_RSRC_ST_UPD, 
                               rsrcInfo.eNb1measId, rsrcInfo.eNb2measId, 
                               event, NULLP);
   }
   
#ifdef ALIGN_64BIT
   CZ_DBG_ERROR((CZ_PRNT_BUF, "Resource Status Update message with eNB1 \
   Measurement Id: (%d) received in an invalid state", eNB1MeasID->val));
#else
   CZ_DBG_ERROR((CZ_PRNT_BUF, "Resource Status Update message with eNB1 \
   Measurement Id: (%ld) received in an invalid state", eNB1MeasID->val));
#endif
   
   CZ_RETVALUE(RFAILED);

} /* czGpmHdlRsrcStsUpdt */

/**
    @brief The function  czGpmHdlLdInd handles Load Indication message in
           CZ_CONNECTED state.

    @details 
       - If the message direction is IN, the message is sent to user.
       - If the message direction is OUT, then the message is sent out. 
    @param[in] peerCb The peerCb is the control block to the peer on whose
               X2-interface the message is exchanged.
    @param[in] msgType The X2 Message type that is exchanged.
    @param[in] event The pdu of the X2 Message that is exchanged.
    @param[in] dir The direction in which the X2 Message is exchanged.

    @return
        - ROK     - If the procedure was successful.
        - RFAILED - If the procedure was unsuccessful due to any failure.
*/
#ifdef ANSI
PUBLIC S16 czGpmHdlLdInd 
(
CzPeerCb* peerCb, 
U8        msgType,
CztEvnt*  event,
U8        dir
)
#else
PUBLIC S16 czGpmHdlLdInd (peerCb, msgType, event, dir)
CzPeerCb* peerCb;
U8        msgType;
CztEvnt*  event;
U8        dir;
#endif
{

   S16 ret = ROK;

   CZ_TRC2(czGpmHdlLdInd)
   CZ_PRNT_PEERID;
   CZ_DBG_INFO((CZ_PRNT_BUF, "Handling X2-Load Indication Message.\n" ));

   /*
    * If incoming send it to user
    */
   if (CZ_DIR_IN == dir)
   {
      ret = czGpmSndRsrcInd(peerCb, NULLP, event, msgType, FALSE, 
                            CztRegistration_RqststartEnum);
   } /* if incoming message */
   /*
    * If outgoing send it to peer using TPT
    */
   else
   {
      /*
       * Send the message to peer
       */
      ret = czGpmSndRsrcMsgToPeer(peerCb, NULLD, NULLD, event, msgType);
               
   } /* if outgoing message */

   CZ_RETVALUE(ret);


} /* czGpmHdlLdInd */

/**
    @brief The function handles retransmission of Resource 
           status request message in CZ_CONNECTED state.

    @details 
       Using the eNb1 measurement ID find the rsrcCb. 
       - If the resource CB is found and the state is either
          - CZ_RSRC_START_PROGRESS 
          - CZ_RSRC_STOP_PROGRESS  
         <br>use TPM to send a previously stored buffer.

    @param[in] peerCb The peerCb is the control block to the peer on whose
               X2-interface the message is exchanged.
    @param[in] eNB1MeasID The eNB1 Measurement ID referred to in the message.

    @return
      - ROK     - If the procedure was successful.
      - RFAILED - If the procedure was unsuccessful due to any failure.
*/
#ifdef ANSI
PUBLIC S16 czGpmHdlRsrcRetxReq 
(
CzPeerCb* peerCb, 
U16       eNB1MeasID
)
#else
PUBLIC S16 czGpmHdlRsrcRetxReq (peerCb, eNB1MeasID)
CzPeerCb* peerCb;
U16       eNB1MeasID;
#endif
{

   S16 ret = ROK;
   CzRsrcCb* rsrcCb;
   U32 rsrcStsCbKey = 0;

   CZ_TRC2(czGpmHdlRsrcRetxReq)
   CZ_PRNT_PEERID;

   CZ_DBG_INFO((CZ_PRNT_BUF, "Handling retransmission of X2-Resource Status \
            request message.\n" ));
   /*
    * Obtain eNB1MeasurementID as the key for searching rsrcCb 
    */
   CZ_MAKE_RSRC_KEY( rsrcStsCbKey, eNB1MeasID, CZ_RSRC_REQ, CZ_DIR_OUT); 

   /*
    * Search for the rsrcStsCb in peerCb 
    */
   if ((ROK == czNmmFindRsrcCb(peerCb, rsrcStsCbKey, &rsrcCb)) &&
       ((rsrcCb->state == CZ_RSRC_START_PROGRESS) || 
        (rsrcCb->state == CZ_RSRC_STOP_PROGRESS)))
   {
      CZ_DBG_INFO((CZ_PRNT_BUF, "Resource Cb Info. eNB1MeasID = %d, \
               eNB2MeasID = %d .\n", rsrcCb->eNb1MeasId, rsrcCb->eNb2MeasId ));
      ret = czTpmSndStoredBuf(rsrcCb->peerCb, rsrcCb->retxBuf); 
      if(ret == ROK)
      {
         /* czUtlUpdOutSts(peerCb, CZT_MSG_RSRC_ST_REQ);  */
         CZ_RETVALUE(ROK);
      }
   } /* if rsrcCb found for rsrc sts req retx */
   /*
    * rsrcCb is not found for the retransmission request
    * send back an error.
    */
   if (ROK == ret)
   {
      CZ_DBG_ERROR((CZ_PRNT_BUF, "Retransmission Request received in invalid\
               state.\n" ));
      CZ_SEND_LCL_RSRCPROC_ERR(peerCb, 
            CZT_ERRTYPE_INTERNAL, 
            CZT_ERRCAUSE_INV_STATE, 
            CZT_MSG_RSRC_ST_REQ, 
            eNB1MeasID, CZ_UNKNOWN_MEAS_ID,
            NULLP, NULLP);
   }
   else
   {
      CZ_DBG_ERROR((CZ_PRNT_BUF, "Failed to send the stored buffer.\n" ));
      CZ_SEND_LCL_RSRCPROC_ERR(peerCb, 
            CZT_ERRTYPE_MSG_SEND_FAILED, 
            ret, 
            CZT_MSG_RSRC_ST_REQ, 
            eNB1MeasID,
            rsrcCb->eNb2MeasId, 
            NULLP, rsrcCb->retxBuf); 
   }
   /* czUtlUpdOutErrSts(peerCb, CZT_MSG_RSRC_ST_REQ); */
   CZ_RETVALUE(RFAILED);

} /* czGpmHdlRsrcRetxReq */

/**
    @brief The function handles Resource Status messages 
           received in states other than CZ_CONNECTED state.

    @details 
        - If the message direction is IN, remote error indication is
        generated and sent to peer.
        - If the message direction is OUT, local error indication is sent to the
        user.
    @param[in] peerCb The peerCb is the control block to the peer on whose
               X2-interface the message is exchanged.
    @param[in] msgType The X2 Message type that is exchanged.
    @param[in] event The pdu of the X2 Message that is exchanged.
    @param[in] dir The direction in which the X2 Message is exchanged.

    @return
        - ROK     - If the procedure was successful.
        - RFAILED - If the procedure was unsuccessful due to any failure.
*/
#ifdef ANSI
PUBLIC S16 czGpmRsrcFsmErr 
(
CzPeerCb* peerCb, 
U8        msgType,
CztEvnt*  event,
U8        dir
)
#else
PUBLIC S16 czGpmRsrcFsmErr (peerCb, msgType, event, dir)
CzPeerCb* peerCb;
U8        msgType;
CztEvnt*  event;
U8        dir;
#endif
{

   CZ_TRC2(czGpmRsrcFsmErr)
   CZ_PRNT_PEERID;

   /*
    * If the direction is IN send the error indication to peer
    */
   if (CZ_DIR_IN == dir)
   {
      CztEvnt* errIndEvnt = NULLP; 
      CZ_DBG_ERROR((CZ_PRNT_BUF, "X2-Resource Status Message received in \
               invalid peer state.\n" ));
      errIndEvnt = czUtlGenPeerErrInd(&event->pdu, msgType, NULLP,
       /* cz001.301: Fix for the build warnings */
#ifdef CZTV1
               CZT_CAUSE_PROTOCOL, 
#else
               CAUSE_PROTOCOL, 
#endif
               CztCauseProtmessage_not_compatible_with_receiver_stateEnum);
      if (errIndEvnt)
      { 
         if (ROK != (czTpmSndToLower(peerCb, &(errIndEvnt->pdu), 
                               peerCb->peerCfg.globalStreamId))) 
         { 
            CZ_DBG_ERROR((CZ_PRNT_BUF, "Failed to send Error Indication "\
                          "message to peer"));
         } 
         CZ_FREEEVNT(errIndEvnt); 
      } 
      CZ_FREEEVNT(event); 
   }
   /*
    * If the direction is OUT send the error indication to user
    */
   CZ_DBG_ERROR((CZ_PRNT_BUF, "X2-Resource Status Message attempted to send in\
            invalid peer state.\n" ));
   CZ_SEND_LCL_ERR_ALL(peerCb, CZT_ERRTYPE_INTERNAL, 
                  CZT_ERRCAUSE_INV_STATE, msgType, event, NULLP);

   CZ_RETVALUE(RFAILED);

} /* czGpmRsrcFsmErr */
 

/** @} */

/**********************************************************************
  
         End of file:     tr_ldm.c@@/main/2 - Tue Aug 30 18:36:12 2011
   
**********************************************************************/
 
/**********************************************************************
        Revision History:

**********************************************************************/
/********************************************************************90**
 
     ver       pat    init                  description
------------ -------- ---- ----------------------------------------------
/main/1      ---     rp              1. LTE-X2AP Initial Release.
          cz001.101  mm           1. CR Fixes, Memory Leak fixes.
/main/2      ---     pt           1. LTE-X2AP 3.1 release.
/main/3   cz001.301  akaranth     1. Fix for the build warnings.
                                  2. kwork fix.
 *********************************************************************91*/
