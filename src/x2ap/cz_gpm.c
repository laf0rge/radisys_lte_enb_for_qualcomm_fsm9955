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
  
     Desc:     This file represents Trillium X2AP Global Procedure
               Module.

     File:     cz_gpm.c

     Sid:      tr_gpm.c@@/main/2 - Tue Aug 30 18:36:11 2011

     Prg:       

**********************************************************************/
/* Header include files (.h) */
#include "envopt.h"        /* Environment options */
#include "envdep.h"        /* Environment dependent */
#include "envind.h"        /* Environment independent */

#include "gen.h"           /* General */
#include "ssi.h"           /* System services */
#include "cm5.h"           /* Common timer library            */
#include "cm_tkns.h"       /* Common Token defines */
#include "cm_llist.h"      /* Common Link List defines */
#include "cm_hash.h"       /* Common Hashlist defines */
#include "cm_tpt.h"
#include "cm_mblk.h"       /* Common memory allocation        */
#include "cm_inet.h"       /* Common socket library           */
#include "cm_pasn.h"
#include "sct.h" 
#include "lcz.h"           /* LCZ Interface defines */
#include "czt.h"           /* CZT Interface defines */
#include "cz.h"            /* X2AP defines */

/* Header/extern include files (.x) */
#include "gen.x"           /* General */
#include "ssi.x"           /* System services */
#include "cm_lib.x"        /* Common library definitions */
#include "cm_tkns.x"       /* Common Token definitions */
#include "cm_llist.x"      /* Common Link List definitions */
#include "cm_hash.x"       /* Common Hashlist definitions */
#include "cm_mblk.x"       /* Common memory allocation        */
#include "cm_inet.x"       /* Common socket library           */
#include "cm_tpt.x"
#include "cm_pasn.x"
#include "cm5.x"           /* Common timer definitions */
#include "sct.x" 
#include "lcz.x"           /* LCZ Interface includes */
#include "czt.x"           /* CZT Interface includes */
#include "cz.x"            /* X2AP includes */

/** @defgroup gpmmodule X2AP Global Procedure Module
 *  @{
 */


#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/** @var CzGpmFsmDef 
  The CzGpmFsmDef defines an event handler for the GPM. */
typedef S16 (*CzGpmFsmDef) ARGS((CzPeerCb *peerCb,
                                 U8 msgType,
                                 CztEvnt *event,
                                 U8 eventDir ));

PRIVATE S16 czGpmSendMsg ARGS((CzPeerCb   *peerCb, 
                               CztEvnt    *event, 
                               U8         msgType,
                               Bool       sendToUser));

PRIVATE S16 czGpmHdlSetupReq ARGS((CzPeerCb   *peerCb, 
                                   U8         msgType,
                                   CztEvnt    *event, 
                                   U8         eventDir));

PRIVATE S16 czGpmHdlSetupFail ARGS((CzPeerCb   *peerCb, 
                                    U8         msgType, 
                                    CztEvnt    *event, 
                                    U8         eventDir));

PRIVATE S16 czGpmHdlSetupAck ARGS((CzPeerCb   *peerCb, 
                                   U8         msgType, 
                                   CztEvnt    *event, 
                                   U8         eventDir));

PRIVATE S16 czGpmHdlSetupReqInCnted ARGS((CzPeerCb   *peerCb, 
                                          U8         msgType, 
                                          CztEvnt    *event, 
                                          U8         eventDir));

PRIVATE S16 czGpmHdlResetReq ARGS((CzPeerCb   *peerCb, 
                                   U8         msgType, 
                                   CztEvnt    *event, 
                                   U8         eventDir));

PRIVATE S16 czGpmHdlResetInCntng ARGS((CzPeerCb   *peerCb, 
                                       U8         msgType, 
                                       CztEvnt    *event, 
                                       U8         eventDir));

PRIVATE S16 czGpmHdlMsg ARGS((CzPeerCb   *peerCb, 
                              U8         msgType, 
                              CztEvnt    *event, 
                              U8         eventDir));

PRIVATE S16 czGpmHdlError ARGS((CzPeerCb   *peerCb, 
                                U8         msgType, 
                                CztEvnt    *event, 
                                U8         eventDir));

PRIVATE S16 czGpmHdlResetRsp ARGS((CzPeerCb   *peerCb, 
                                   U8         msgType, 
                                   CztEvnt    *event, 
                                   U8         eventDir));

PRIVATE S16 czGpmHdlSetupReqInCntng ARGS((CzPeerCb   *peerCb, 
                                          U8         msgType, 
                                          CztEvnt    *event, 
                                          U8         eventDir));

PRIVATE S16 czGpmHdlEnbCfgRsp ARGS((CzPeerCb   *peerCb, 
                                    U8         msgType, 
                                    CztEvnt    *event, 
                                    U8         eventDir));

PRIVATE S16 czGpmHdlEnbCfgReq ARGS((CzPeerCb   *peerCb, 
                                    U8         msgType, 
                                    CztEvnt    *event, 
                                    U8         eventDir));

PRIVATE S16 czGpmHdlEnbCfgUpdErr ARGS((CzPeerCb   *peerCb, 
                                       U8         msgType, 
                                       CztEvnt    *event, 
                                       U8         eventDir));

PRIVATE S16 czGpmHdlSetupAckInCnted ARGS((CzPeerCb   *peerCb, 
                                          U8         msgType, 
                                          CztEvnt    *event, 
                                          U8         eventDir));


PRIVATE S16 czGpmHdlSetupFailInCnted ARGS((CzPeerCb   *peerCb, 
                                           U8         msgType, 
                                           CztEvnt    *event, 
                                           U8         eventDir));

PRIVATE S16 czGpmHdlResetRspInCnted ARGS((CzPeerCb   *peerCb, 
                                          U8         msgType, 
                                          CztEvnt    *event, 
                                          U8         eventDir));

PRIVATE S16 czGpmErrRspToPeer ARGS((CzPeerCb   *peerCb, 
                                    CztEvnt    *event,
                                    U8         msgType));

PRIVATE S16 czGpmHdlMobChangeReq ARGS((CzPeerCb   *peerCb,
                                    U8         msgType,
                                    CztEvnt    *event,
                                    U8         eventDir));

PRIVATE S16 czGpmHdlMobChangeAck ARGS((CzPeerCb   *peerCb,
                                    U8         msgType,
                                    CztEvnt    *event,
                                    U8         eventDir));

PRIVATE S16 czGpmHdlMobChangeFail ARGS((CzPeerCb   *peerCb,
                                    U8         msgType,
                                    CztEvnt    *event,
                                    U8         eventDir));

PRIVATE S16 czGpmHdlRLFIndication ARGS((CzPeerCb   *peerCb,
                                    U8         msgType,
                                    CztEvnt    *event,
                                    U8         eventDir));

PRIVATE S16 czGpmHdlHoReport ARGS((CzPeerCb   *peerCb,
                                    U8         msgType,
                                    CztEvnt    *event,
                                    U8         eventDir));

PRIVATE S16 czGpmHdlCellActivationReq ARGS((CzPeerCb   *peerCb,
                                       U8         msgType,
                                       CztEvnt    *event,
                                       U8         eventDir));

PRIVATE S16 czGpmHdlCellActivationRes ARGS((CzPeerCb   *peerCb,
                                       U8         msgType,
                                       CztEvnt    *event,
                                       U8         eventDir));

PRIVATE S16 czGpmHdlCellActivationFail ARGS((CzPeerCb   *peerCb,
                                         U8         msgType,
                                         CztEvnt    *event,
                                         U8         eventDir));




/************* local utilities *************/
PRIVATE Void czGpmStartWaitTmr ARGS((CzPeerCb      *peerCb,
                                     CztX2AP_PDU   *msg,
                                     U8            status));


/**
 * @brief The CzGpmFsm is a two dimensional matrix of states and events for
 * the GPM event handlers
 */
PRIVATE CzGpmFsmDef CzGpmFsm[CZ_GP_STATE_MAX][CZ_EVNT_MAX + 1] =
{
    {
        /* CZ_GP_IDLE */
        czGpmHdlError,          /* CZ_EVNT_ERR_IND */
        czGpmHdlSetupReq,       /* CZ_EVNT_SETUP_REQ */
        czGpmHdlError,          /* CZ_EVNT_SETUP_RSP */
        czGpmHdlError,          /* CZ_EVNT_SETUP_FAIL */
        czGpmHdlError,          /* CZ_EVNT_RESET_REQ */
        czGpmHdlError,          /* CZ_EVNT_RESET_RSP */
        czGpmHdlEnbCfgUpdErr,   /* CZ_EVNT_ENB_CFG_UPD_REQ */
        czGpmHdlError,          /* CZ_EVNT_ENB_CFG_UPD_ACK */
        czGpmHdlError,          /* CZ_EVNT_ENB_CFG_UPD_FAIL */
        czGpmRsrcFsmErr,        /* CZ_EVNT_LOAD_IND */
        czGpmRsrcFsmErr,        /* CZ_EVNT_RES_STA_REQ */
        czGpmRsrcFsmErr,        /* CZ_EVNT_RES_STA_RSP */
        czGpmRsrcFsmErr,        /* CZ_EVNT_RES_STA_FAIL */
        czGpmRsrcFsmErr,        /* CZ_EVNT_RES_STA_UPDATE */
        czGpmHdlError,           /* CZ_EVNT_MOB_CHANGE_REQ */
        czGpmHdlError,           /* CZ_EVNT_MOB_CHANGE_ACK */
        czGpmHdlError,           /* CZ_EVNT_MOB_CHANGE_FAIL */
        czGpmHdlError,           /* CZ_EVNT_RLF_INDICATION */
        czGpmHdlError,           /* CZ_EVNT_HO_REPORT */
        czGpmHdlError,           /* CZ_EVNT_CELL_ACTIVATION_REQ */
        czGpmHdlError,           /* CZ_EVNT_CELL_ACTIVATION_RES */
        czGpmHdlError,           /* CZ_EVNT_CELL_ACTIVATION_FAIL */
        czGpmHdlError,           /* CZ_EVNT_PRIVATE */
    },
    {
       /* cz001.101 : The state CZ_GP_RESET is removed. The handlers
        * of this state are handled in state CZ_GP_CONNECTING. */
        /* CZ_GP_CONNECTING */
        czGpmHdlErrInd,            /* CZ_EVNT_ERR_IND */
        czGpmHdlSetupReqInCntng,   /* CZ_EVNT_SETUP_REQ */
        czGpmHdlSetupAck,          /* CZ_EVNT_SETUP_RSP */
        czGpmHdlSetupFail,         /* CZ_EVNT_SETUP_FAIL */
        czGpmHdlResetInCntng,      /* CZ_EVNT_RESET_REQ */
        czGpmHdlResetRsp,          /* CZ_EVNT_RESET_RSP */
        czGpmHdlEnbCfgUpdErr,      /* CZ_EVNT_ENB_CFG_UPD_REQ */
        czGpmHdlError,             /* CZ_EVNT_ENB_CFG_UPD_ACK */
        czGpmHdlError,             /* CZ_EVNT_ENB_CFG_UPD_FAIL */
        czGpmRsrcFsmErr,           /* CZ_EVNT_LOAD_IND */
        czGpmRsrcFsmErr,           /* CZ_EVNT_RES_STA_REQ */
        czGpmRsrcFsmErr,           /* CZ_EVNT_RES_STA_RSP */
        czGpmRsrcFsmErr,           /* CZ_EVNT_RES_STA_FAIL */
        czGpmRsrcFsmErr,           /* CZ_EVNT_RES_STA_UPDATE */
        czGpmHdlError,             /* CZ_EVNT_MOB_CHANGE_REQ */
        czGpmHdlError,             /* CZ_EVNT_MOB_CHANGE_RES */
        czGpmHdlError,             /* CZ_EVNT_MOB_CHANGE_FAIL */
        czGpmHdlError,             /* CZ_EVNT_RLF_INDICATION */
        czGpmHdlError,             /* CZ_EVNT_HO_REPORT */
        czGpmHdlError,             /* CZ_EVNT_CELL_ACTIVATION_REQ */
        czGpmHdlError,             /* CZ_EVNT_CELL_ACTIVATION_RES */
        czGpmHdlError,             /* CZ_EVNT_CELL_ACTIVATION_FAIL */
        czGpmHdlError,              /* CZ_EVNT_PRIVATE */
    },
    {
        /* CZ_GP_CONNECTED */
        czGpmHdlErrInd,              /* CZ_EVNT_ERR_IND */
        czGpmHdlSetupReqInCnted,     /* CZ_EVNT_SETUP_REQ */
        czGpmHdlSetupAckInCnted,     /* CZ_EVNT_SETUP_RSP capture responses
                                      * for retransmitted Setup Requests. */ 
        czGpmHdlSetupFailInCnted,    /* CZ_EVNT_SETUP_FAIL */
        czGpmHdlResetReq,            /* CZ_EVNT_RESET_REQ */
        czGpmHdlResetRspInCnted,     /* CZ_EVNT_RESET_RSP */
        czGpmHdlEnbCfgReq,           /* CZ_EVNT_ENB_CFG_UPD_REQ */
        czGpmHdlEnbCfgRsp,           /* CZ_EVNT_ENB_CFG_UPD_ACK */
        czGpmHdlEnbCfgRsp,           /* CZ_EVNT_ENB_CFG_UPD_FAIL */
        czGpmHdlLdInd,               /* CZ_EVNT_LOAD_IND */
        czGpmHdlRsrcStsReq,          /* CZ_EVNT_RES_STA_REQ */
        czGpmHdlRsrcStsRsp,          /* CZ_EVNT_RES_STA_RSP */
        czGpmHdlRsrcStsFailr,        /* CZ_EVNT_RES_STA_FAIL */
        czGpmHdlRsrcStsUpdt,         /* CZ_EVNT_RES_STA_UPDATE */
        czGpmHdlMobChangeReq,        /* CZ_EVNT_MOB_CHANGE_REQ */
        czGpmHdlMobChangeAck,        /* CZ_EVNT_MOB_CHANGE_ACK */
        czGpmHdlMobChangeFail,       /* CZ_EVNT_MOB_CHANGE_FAIL */
        czGpmHdlRLFIndication,       /* CZ_EVNT_RLF_INDICATION */
        czGpmHdlHoReport,            /* CZ_EVNT_HO_REPORT */
        czGpmHdlCellActivationReq,   /* CZ_EVNT_CELL_ACTIVATION_REQ */
        czGpmHdlCellActivationRes,   /* CZ_EVNT_CELL_ACTIVATION_RES */
        czGpmHdlCellActivationFail,  /* CZ_EVNT_CELL_ACTIVATION_FAIL */
        czGpmHdlMsg,                 /* CZ_EVNT_PRIVATE */
    },
};/* End of CzGpmFsm matrix */

#ifdef __cplusplus
}
#endif /* __cplusplus */

/** @def CZ_UPD_STS 
 * The macro CZ_UPD_STS is used to print successful processing of a message */
/* #define CZ_UPD_STS(_peerCb, _msgType, _dir)  \
{                                            \
 if(_dir == CZ_DIR_IN)                       \
 {                                           \
    CZ_DBG_INFO((CZ_PRNT_BUF,                \
     "Message %d received successfully.\n", msgType)); \
 }                                            \
 else                                         \
 {                                            \
    CZ_DBG_INFO((CZ_PRNT_BUF,                  \
        " Message %d sent successfully.\n", msgType)); \
 }                                             \
}
*/ 
/** @def CZ_GP_ERR_IND_USR 
 * The macro CZ_GP_ERR_IND_USR is used to give an error indication to the
 * user. */
/* #define CZ_GP_ERR_IND_USR(_peerCb, _event, _msgType, _prevState)\
{\
   S16 _ret = ROK; \
   _ret = czUimSndGpErrInd(_peerCb, _event);\
   if(_ret != ROK)\
   {\
      _peerCb->state = _prevState;\
      _ret = RFAILED;\
      CZ_FREEEVNT(_event); \
      czUtlUpdInErrSts(_peerCb, _msgType); \
      CZ_DBG_FATAL((CZ_PRNT_BUF, \
          "Indication of the received message %d failed.\
               Reverting the state to %d.\n", \
               _msgType, _prevState));\
   }\
   else\
   {\
      CZ_DBG_INFO((CZ_PRNT_BUF, \
          "Indication of the received message %d successful.\n", _msgType));\
   }\
}

*/
/**********************************************************************/
/* Public Function Definitions */
/**********************************************************************/

/**
    @brief The function czGpmFsmMsgHandler is the entry point for all the X2AP global
           procedural messages. 
    @details This function takes the incoming or the outgoing message and
    invokes the event handler using the currently state and this message.
    The result is returned to the calling module UIM.

    @param[in] peerCb   The peerCb is the control block to the peer on whose 
                        X2-interface the message is exchanged.
    @param[in] czEvent    The event that has come from peer or user.
    @param[in] msgType  The X2 global proceduraal message that is exchanged.
    @param[in] eventDir The Message direction: 
                        - CZ_DIR_OUT: If from user, 
                        - CZ_DIR_IN: If from peer. 

    @return
        - ROK     - If the procedure was successful.
        - ROUTRES - If the procedure was unsuccessful due to resource crunch.
        - RFAILED - If the procedure was unsuccessful due to any failure.
*/
#ifdef ANSI
PUBLIC S16 czGpmFsmMsgHandler
(
CzPeerCb  *peerCb,
CztEvnt   *czEvent,
U8        msgType,
U8        eventDir
)
#else /* Else if ANSI not defined */
PUBLIC S16 czGpmFsmMsgHandler (peerCb, czEvent, msgType, eventDir)
CzPeerCb  *peerCb;
CztEvnt   *czEvent;
U8        msgType;
U8        eventDir;
#endif /* End of Else */
{
   U8 eventIdx;
   S16 ret;

   CZ_TRC2(czGpmFsmMsgHandler);
   CZ_PRNT_PEERID;

   ret = ROK;

   /* Get Event type from the PDU */
   /* UI has checked for validity of the message for GP
      The Mapping is got from static global structure which is reviewed
      hence there is no need to check for range of the event */

   CZ_GET_EVENT_MSG_MAP(msgType, eventIdx);

   CZ_DBG_INFO((CZ_PRNT_BUF,"Handling of message with Id %d received in \
            state %d in %d direction\n", 
            msgType, peerCb->state, eventDir));

   /* update the total msg statistics */
   if (CZ_DIR_IN == eventDir)
   {
      czUtlUpdInSts(peerCb, msgType);
   }
   else if (CZ_DIR_OUT == eventDir)
   {
      czUtlUpdOutSts(peerCb, msgType);
   }

   ret = CzGpmFsm[peerCb->state][eventIdx](peerCb, msgType, czEvent, eventDir);

   /* If the handler fails update the error msg statistics */
   if (ROK != ret)
   {
      if (CZ_DIR_IN == eventDir)
      {
         czUtlUpdInErrSts(peerCb, msgType);
      }
      else if (CZ_DIR_OUT == eventDir)
      {
         czUtlUpdOutErrSts(peerCb, msgType);
      }
   }

   CZ_RETVALUE(ret);
}

/**********************************************************************/

/**
    @brief The function czGpmWaitTmrHdl handles timer expiry of "Time To Wait" timer 
    for Global Procedures Request Messages..
          
    @details It is specifically used with Time To Wait timer for:
    - X2 Setup Request
    - eNB Configuration Update Request
    The flags wait is reset in this function.
    Whenever a Time to Wait IE is received in Setup/ Config Update
    failure from peer, the respective flag is set and the timer is started
    with the value that is received.

    @param[in] peerCb     The peerCb is the control block to the peer on whose 
                          X2-interface the message is exchanged.
    @return
        - ROK     - If the procedure was successful.
        - ROUTRES - If the procedure was unsuccessful due to resource crunch.
        - RFAILED - If the procedure was unsuccessful due to any failure.
*/
#ifdef ANSI
PUBLIC S16 czGpmWaitTmrHdl 
(
CzPeerCb  *peerCb
)
#else /* Else if ANSI not defined */
PUBLIC S16 czGpmWaitTmrHdl (peerCb)
CzPeerCb  *peerCb;
#endif /* End of else */
{
   U8          status;
   CzCztSapCb  *cztSapCb = NULLP;
   CztStaInd   tmrExpStaInd;
   S16         ret = ROK;

   CZ_TRC2(czGpmWaitTmrHdl);
   CZ_PRNT_PEERID;

   /* Reset wait flag */
   peerCb->gpmCb.wait = FALSE;


   if(peerCb->state == CZ_GP_CONNECTED)
   {
      status = CZT_STATUS_CFGUPD_TW_EXPIRY;
      CZ_DBG_INFO((CZ_PRNT_BUF, "Wait Timer expired for eNB Config Update \
               Message. Wait flag is made FALSE. \n"));
   }
   else
   {
      status = CZT_STATUS_SETUPREQ_TW_EXPIRY;
      CZ_DBG_INFO((CZ_PRNT_BUF, "Wait Timer expired for eNB X2-Setup \
               Message. Wait flag is made FALSE. \n"));
   }
   cztSapCb = peerCb->cztSapCb;
   CZ_SET_ZERO(&tmrExpStaInd, sizeof(CztStaInd)); 
   tmrExpStaInd.peerId = peerCb->peerId;
   tmrExpStaInd.type = CZT_STA_TYPE_TMR_EXPIRY;
   tmrExpStaInd.status = status;
   tmrExpStaInd.reason = CZT_REASON_NOT_APPL;

   /* Indicate the user about the expiry of wait timer */
   ret = czUimSndStaInd(cztSapCb, &tmrExpStaInd);
   if(ROK != ret)
   {
      CZ_DBG_FATAL((CZ_PRNT_BUF, "Delivering Status Indication with "\
               "TimeToWait(%d) expiry failed.\n", \
               status));
   }
   CZ_RETVALUE(ROK);
}

/**********************************************************************/


/**
    @brief The function czGpmRetransmit is used for retransmission of the stored
    buffer.
          
    @details This function handles the retransmission of:
    - X2 Setup Request
    - eNB Configuration Update Request
    - Resource Status Request 
    - Reset Request 

    towards the peer. Based on the message type it validates whether it is a
    proper retransmission request. If so then it sends the stored buffer in the
    peerCb again to the peer through TPM.

    If message type is Resource Status Request it calls czGpmHdlRsrcRetxReq. 

    @param[in] peerCb        The peerCb is the control block to the peer on whose 
                             X2-interface the message is exchanged.
    @param[in] msgType       The msg represents the X2 Message that is exchanged.
    @param[in] measurementId eNodeB1 Measurement Id required to findout the
                             resource Cb at X2AP.
    @return
        - ROK     - If the procedure was successful.
        - ROUTRES - If the procedure was unsuccessful due to resource crunch.
        - RFAILED - If the procedure was unsuccessful due to any failure.
*/

#ifdef ANSI
PUBLIC S16 czGpmRetransmit
(
CzPeerCb  *peerCb,
U8        msgType,
U16       measurementId
)
#else /* Else if ANSI not defined */
PUBLIC S16 czGpmRetransmit (peerCb, msgType, measurementId)
CzPeerCb  *peerCb;
U8        msgType;
U16       measurementId;
#endif /* End of else */
{
   S16 ret;
   U8 prevState, prevProcDir;

   CZ_TRC2(czGpmRetransmit);
   CZ_PRNT_PEERID;

   ret = ROK;
   prevState = peerCb->state;
   prevProcDir = peerCb->gpmCb.procPending;


   /* It can be sent without any check for GP procedure
    * since the presence of PDU validates itself.
    * It is cleared once the response is received. */
   /* verify the message type */

   if(msgType == CZT_MSG_RSRC_ST_REQ)
   {
      if(peerCb->state != CZ_GP_CONNECTED)
      {
         CZ_DBG_ERROR((CZ_PRNT_BUF, "Retransmit Request invoked by user in \
                                    invalid state for CZT_MSG_RSRC_ST_REQ\n"));
         CZ_SEND_LCL_GEN_ERR(peerCb, CZT_ERRTYPE_INTERNAL,
               CZT_ERRCAUSE_INV_STATE, msgType );
         CZ_RETVALUE(RFAILED);
      }

      ret = czGpmHdlRsrcRetxReq(peerCb, measurementId);
      CZ_RETVALUE(ret);
   }

   /* Setup/Reset are retransmitted only in if outgoing Setup/Reset procedures
    * are pending
    * eNB COnfig Update is retransmitted only in CONNECTED state */

   if(((msgType == CZT_MSG_X2_SETUP) &&
          (peerCb->gpmCb.procPending != CZ_SRC_SETUP))
            || ((msgType == CZT_MSG_RESET) &&
               (peerCb->gpmCb.procPending != CZ_SRC_RESET))
            || ((msgType == CZT_MSG_CFG_UPD_REQ) &&
               (peerCb->state != CZ_GP_CONNECTED)))
   {
      CZ_DBG_ERROR((CZ_PRNT_BUF, "Retransmit Request received in Invalid \
               state(%d) for msgType %d\n",peerCb->state, msgType ));
      CZ_SEND_LCL_GEN_ERR(peerCb, CZT_ERRTYPE_INTERNAL,
            CZT_ERRCAUSE_INV_STATE, msgType );
      CZ_RETVALUE(RFAILED);
   }

   if((peerCb->gpmCb.retxBuf) != NULLP)
   {
      ret = czTpmSndStoredBuf(peerCb, peerCb->gpmCb.retxBuf); 
      if(ret != ROK)
      {
         CZ_SEND_LCL_GEN_ERR(peerCb, CZT_ERRTYPE_MSG_SEND_FAILED,
               ret, msgType );
         CZ_DBG_FATAL((CZ_PRNT_BUF,
                  "Retransmission of the message %d failed.\n", msgType));
         ret = RFAILED;
      }
      else
      {
         CZ_DBG_INFO((CZ_PRNT_BUF, 
                  "Retransmission of the message %d successful.\n", msgType));
      }
   }
   else
   {
      /* Give Error indication to the user.*/
      CZ_SEND_LCL_GEN_ERR(peerCb, CZT_ERRTYPE_INTERNAL,
            CZT_ERRCAUSE_INV_STATE, msgType );
      CZ_DBG_FATAL((CZ_PRNT_BUF, 
               "No Retransmission Buffer for the message %d\n", msgType));
      ret = RFAILED;
   }
   CZ_RETVALUE(ret);
}


/**
    @brief The function czGpmHdlErrInd is called when X2 Error Indication
           is being sent or received. 
    @details If the Error Indication is received for the Setup Procedure, The state
             is changed to CZ_GP_IDLE state. 
             If the Error indication is for Resource Management, that module is called,
             else the Error Indication is passed to the user transperantly.
    @param[in] peerCb The peerCb is the control block to the peer on whose 
                  X2-interface the message is exchanged.
    @param[in] msgType The msg represents the X2 Message that is exchanged.
    @param[in] event   The event that has come from peer or user.
    @param[in] eventDir Event direction from user or peer.
    @return
        - ROK     - If the procedure was successful.
        - ROUTRES - If the procedure was unsuccessful due to resource crunch.
        - RFAILED - If the procedure was unsuccessful due to any failure.
*/
#ifdef ANSI
PUBLIC S16 czGpmHdlErrInd 
(
CzPeerCb  *peerCb, 
U8         msgType, 
CztEvnt   *event, 
U8         eventDir
)
#else /* Else if ANSI not defined */
PUBLIC S16 czGpmHdlErrInd (peerCb, msgType, event, eventDir)
CzPeerCb  *peerCb;
U8         msgType;
CztEvnt   *event;
U8         eventDir;
#endif /* End of else */
{
   U8 prevState, prevProcDir;
   S16 ret = ROK;
   CztCriticalityDiag *critDiaIE;

   CZ_TRC2(czGpmHdlErrInd);
   CZ_PRNT_PEERID;

   critDiaIE   = NULLP;
   prevState   = peerCb->state;
   prevProcDir = peerCb->gpmCb.procPending;

   /* If error message is for setup failure , go to idle state
      else pass it to user/transport */

   ret = czUtlGetIE( &(event->pdu), Cztid_CriticalityDiag,
         (TknU8 **)&critDiaIE);

   if((ret == ROK) && (critDiaIE != NULLP))
   {
      if((critDiaIE->procedureCode.pres == PRSNT_NODEF) &&
            (critDiaIE->triggeringMsg.pres == PRSNT_NODEF) &&
            (critDiaIE->procedureCode.val == Cztid_x2Setup) &&
            (critDiaIE->triggeringMsg.val == X2AP_PDU_SUCCESSFULOUTCOME))
      {
         czNmmResetPeer(peerCb);
         czCb.genSts.nmbPeerConnected--;
         czCb.genSts.nmbPeerConnFail++;
         CZ_DBG_INFO((CZ_PRNT_BUF, 
                  "State Changed to CZ_GP_IDLE from Peer.\
                  Error Indication Received for Setup Ack\n"));
      }

      /* cz001.301: Changing the peer state to CZ_GP_IDLE,
       * If eNB receives error msg for reset msg. 
       * To allow app to send X2 setup once again.*/

      else if((critDiaIE->procedureCode.pres == PRSNT_NODEF) &&
            (critDiaIE->triggeringMsg.pres == PRSNT_NODEF) &&
            (critDiaIE->procedureCode.val == Cztid_reset) &&
            (critDiaIE->triggeringMsg.val == X2AP_PDU_INITIATINGMSG))
      {
         /* if error message is for Reset Req, revert the state and
            remove Rtx buff */
         if (peerCb->gpmCb.procPending & CZ_SRC_RESET)
         {
            /* Free the retransmission PDU. Change state to CONNECTED */
            /*     CZ_FREEMBUF(peerCb->gpmCb.retxBuf); */
            CZ_DBG_INFO((CZ_PRNT_BUF,
                     "State Changed to CZ_GP_CONNECTED by Err-Ind for Reset \
                     from Peer.\n"));
            /* cz001.301 : freeing the Retransmission buffer */
            CZ_FREEMBUF((peerCb->gpmCb.retxBuf));
            peerCb->state             = CZ_GP_IDLE;
            peerCb->gpmCb.procPending &= ~CZ_SRC_RESET;
         }
      }
   }

   if(eventDir == CZ_DIR_IN)
   {
      /* Update the total incoming error indication counter */
      czUtlUpdInSts(peerCb, CZT_MSG_ERR_IND);

      /* Indicate to the user */
      CZ_DBG_INFO((CZ_PRNT_BUF, 
               "Error Indication received from Peer.\n"));
      CZ_GP_ERR_IND_USR(peerCb, event, msgType, prevState );
   }
   else
   {
      /* Update the total incoming error indication counter */
      czUtlUpdOutSts(peerCb, CZT_MSG_ERR_IND);

      CZ_DBG_INFO((CZ_PRNT_BUF, 
               "Error Indication sent from User.\n"));
      czGpmSendMsg(peerCb, event, msgType, TRUE);
   }
   CZ_RETVALUE(ret);
}/* end of czGpmHdlErrInd */


/**********************************************************************/
/* FSM Private Function Definitions */
/**********************************************************************/

/**
    @brief  The function czGpmSendMsg sends the global procedure message to peer.
    @details It sends the message out of X2AP to SCTP. It calls 
    czTpmSndToLower. It sends error indication incase if it is not able to send
    out the message.
    This function is used when the send failure does not call for roll back to
    previous states.
    
    @param[in] peerCb The peerCb is the control block to the peer on whose 
                  X2-interface the message is exchanged.
    @param[in] event   The event that has come from peer or user.
    @param[in] msgType The msg represents the X2 Message that is exchanged.
    @param[in] sendUser Flag which tells whether to send the error indication
                       to peer.
    @return
        - ROK     - If the procedure was successful.
        - ROUTRES - If the procedure was unsuccessful due to resource crunch.
        - RFAILED - If the procedure was unsuccessful due to any failure.
*/
#ifdef ANSI
PRIVATE S16 czGpmSendMsg
(
CzPeerCb     *peerCb,
CztEvnt      *event,
U8           msgType,
Bool         sendUser
)
#else/* Else if ANSI not defined */
PRIVATE S16 czGpmSendMsg (peerCb, event, msgType, sendUser)
CzPeerCb    *peerCb;
CztEvnt     *event;
U8          msgType;
Bool        sendUser;
#endif/*End of else */
{
   S16  ret = ROK;

   CZ_TRC2(czGpmSendMsg);
   CZ_PRNT_PEERID;

   ret = czTpmSndToLower(peerCb, &(event->pdu), 
         peerCb->peerCfg.globalStreamId); 

   if(ret != ROK)
   {
      CZ_DBG_FATAL((CZ_PRNT_BUF, 
               "Transmitting of the message %d failed.\n", msgType));

      /* Update error statistics for this incoming msg */
      czUtlUpdOutErrSts(peerCb, msgType);

      if(sendUser == TRUE)
      {
         CZ_SEND_LCL_ERR_ALL(peerCb, CZT_ERRTYPE_MSG_SEND_FAILED, ret, msgType,
               event, NULLP);
      }
      else
      {
         CZ_FREEEVNT(event);
      }
   }
   else
   {
      CZ_FREEEVNT(event);
      CZ_DBG_INFO((CZ_PRNT_BUF, 
               "The message %d sent successfully.\n", msgType));
   }
   CZ_RETVALUE(ret);
}

/**
    @brief The function czGpmHdlSetupReq is called when X2 Setup request is being sent or received.
           
    @details It is valid only in CZ_GP_IDLE. It changes the state to CZ_GP_CONNECTING.
    - If the procedure is outgoing, the function in transport module first (if needed)
    opens an association towards the peer. Further the X2-Setup is encoded and
    sent to the peer. The procedure direction bit for CZ_SRC_SETUP,
       If the wait timer is ON, the outgoing request fails and error is returned.
    - If the request is received it is saved as CZ_TGT_SETUP.

    @param[in] peerCb The peerCb is the control block to the peer on whose 
                  X2-interface the message is exchanged.
    @param[in] msgType The msg represents the X2 Message that is exchanged.
    @param[in] event   The event that has come from peer or user.
    @param[in] eventDir Event direction from user or peer.
    @return
        - ROK     - If the procedure was successful.
        - ROUTRES - If the procedure was unsuccessful due to resource crunch.
        - RFAILED - If the procedure was unsuccessful due to any failure.
*/
#ifdef ANSI
PRIVATE S16 czGpmHdlSetupReq 
(
CzPeerCb  *peerCb, 
U8         msgType, 
CztEvnt   *event, 
U8         eventDir
)
#else /* Else if ANSI not defined */
PRIVATE S16 czGpmHdlSetupReq (peerCb, msgType, event, eventDir)
CzPeerCb  *peerCb; 
U8         msgType; 
CztEvnt   *event; 
U8         eventDir;
#endif /* End of else */
{
   S16 ret;
   U8 prevState, prevProcDir;

   CZ_TRC2(czGpmHdlSetupReq);
   CZ_PRNT_PEERID;

   ret         = ROK;
   prevState   = peerCb->state;
   prevProcDir = peerCb->gpmCb.procPending;

   /*
      Reset all Values in Cb.
      Change state to CONNECTING
      Set direction of the procedure to that of the message. */
   /* Incoming X2Setup Request message from Peer */

   if(eventDir == CZ_DIR_IN)
   {
      /* Its an INCOMING SETUP Request 
       * indicate it to the user */

      peerCb->state             = CZ_GP_CONNECTING;
      peerCb->gpmCb.procPending = CZ_TGT_SETUP;

      CZ_DBG_INFO((CZ_PRNT_BUF, 
               "State Changed to CZ_GP_CONNECTING from the peer.\n"));

      ret = czUimSndGpmInd(peerCb, event);
      if(ret != ROK)
      {
         peerCb->state             = prevState;
         peerCb->gpmCb.procPending = prevProcDir;
         ret                       = RFAILED;

         CZ_DBG_FATAL((CZ_PRNT_BUF, "Delivering the incoming "\
                  "X2-Setup Request Message failed in \
                  state CZ_GP_IDLE. Sending a X2-Setup \
                  Failure Response to the peer.\
                  Reverting the state to CZ_GP_IDLE."));

         /* Falied to inform user. Now build SetUp Fail message to Peer */
         czGpmErrRspToPeer(peerCb, event, msgType);
      } /* if(ret != ROK) */
   } /* if(eventDir == CZ_DIR_IN) */
   else
   {
      /* Its an outgoing Setup Request towards peer
       * Send it to the transport */
      /* First check if the wait timer is running,
       * If so send the Request only else send the Error Indication */

      if (peerCb->gpmCb.wait != TRUE)
      {
         peerCb->state             = CZ_GP_CONNECTING;
         peerCb->gpmCb.procPending = CZ_SRC_SETUP;

         CZ_DBG_INFO((CZ_PRNT_BUF, 
                  "State Changed to CZ_GP_CONNECTING from User.\n"));
         if ((peerCb->assocCb->assocState) != CZ_ASSOC_UP)
         {
            /* Create an association if necessary. */
            ret =  czTpmSndAssocReq(&(event->pdu), peerCb->assocCb);
         }
         else
         {
            /* store the PDU for retransmission and then send */
            ret = czTpmStoreAndSendMsg(peerCb, &(event->pdu),
                  peerCb->peerCfg.globalStreamId,
                  &(peerCb->gpmCb.retxBuf));
         }
         if(ret != ROK)
         {
            peerCb->state             = prevState;
            peerCb->gpmCb.procPending = prevProcDir;

            CZ_DBG_FATAL((CZ_PRNT_BUF,
                     "The message %d failed to be sent.\
                     Reverting the state to CZ_GP_IDLE.\n", msgType));
            CZ_SEND_LCL_ERR_ALL(peerCb, CZT_ERRTYPE_MSG_SEND_FAILED, ret, 
                  msgType, event, NULLP);
            ret = RFAILED;
         } /* if(ret != ROK) */
         else
         {
            CZ_FREEEVNT(event); 
         }
      } /* if (peerCb->gpmCb.wait != TRUE) */
      else
      {
         CZ_SEND_LCL_ERR_ALL(peerCb, CZT_ERRTYPE_INTERNAL, 
               CZT_ERRCAUSE_WAIT_TMR_ON, 
               CZT_MSG_X2_SETUP,event, NULLP);
         CZ_DBG_ERROR((CZ_PRNT_BUF, 
                  "Wait timer is running for Setup Message\n"));
         ret = RFAILED;
      }
   } /* else ..(eventDir == CZ_DIR_OUT) */

   if(ROK == ret)
   {
      CZ_UPD_STS(peerCb, msgType, eventDir);
   }

   CZ_RETVALUE (ret);
}

/**********************************************************************/


/**
    @brief The function czGpmHdlSetupReqInCnted is called when X2 Setup Request
           is being sent or received in CZ_GP_CONNECTED state.
    @details The state is changed to CZ_GP_CONNECTING state. 
    - If Setup is outgoing, the existing procedures are terminated, example UE HO. 
      In procedure direction set the CZ_SRC_SETUP bit if it is outgoing.
      Send it to the transport.
    - If Setup Request is incoming, set the CZ_TGT_SETUP bit in procedure direction.
      Indicate this to the user.

    @param[in] peerCb   The peerCb is the control block to the peer on whose 
                        X2-interface the message is exchanged.
    @param[in] msgType  The msg represents the X2 Message that is exchanged.
    @param[in] event    The event that has come from peer or user.
    @param[in] eventDir Event direction from user or peer.
    @return
        - ROK     - If the procedure was successful.
        - ROUTRES - If the procedure was unsuccessful due to resource crunch.
        - RFAILED - If the procedure was unsuccessful due to any failure.
*/
#ifdef ANSI
PRIVATE S16 czGpmHdlSetupReqInCnted 
(
CzPeerCb  *peerCb, 
U8         msgType, 
CztEvnt   *event, 
U8         eventDir
)
#else /* Else if ANSI not defined */
PRIVATE S16 czGpmHdlSetupReqInCnted (peerCb, msgType, event, eventDir)
CzPeerCb  *peerCb; 
U8         msgType; 
CztEvnt   *event; 
U8         eventDir;
#endif /* End of else */
{
   S16 ret;
   U8 prevState, prevProcDir;

   CZ_TRC2(czGpmHdlSetupReqInCnted);
   CZ_PRNT_PEERID;

   ret         = ROK;
   prevState   = peerCb->state;
   prevProcDir = peerCb->gpmCb.procPending;

   /* If wait timer is running it has to be for eNB Config Update
    * Stop that timer */

   if(peerCb->gpmCb.wait == TRUE)
   {
      czStopTmr(CZ_TMR_TIME_TO_WAIT, peerCb);
      peerCb->gpmCb.wait = FALSE;
      CZ_DBG_INFO((CZ_PRNT_BUF, 
               "Wait Timer stopped.\n"));
   }
   if (eventDir == CZ_DIR_IN)
   {
      /* Its an incoming SETUP request 
       * indicate it to the user */

      /* RESET the peer. Since X2 SETUP REQUEST does the same thing as the
         RESET procedure does.
         Reset can be done when response is being sent */

      peerCb->state             = CZ_GP_CONNECTING;
      peerCb->gpmCb.procPending = prevProcDir | CZ_TGT_SETUP;
      czCb.genSts.nmbPeerConnected--;

      CZ_DBG_INFO((CZ_PRNT_BUF, 
               "State Changed to CZ_GP_CONNECTING from Peer.\n"));

      ret = czUimSndGpmInd(peerCb, event);
      if(ret != ROK)
      {
         peerCb->state             = prevState;
         peerCb->gpmCb.procPending = prevProcDir;
         ret = RFAILED;

         CZ_DBG_FATAL((CZ_PRNT_BUF, "Delivering the incoming "\
                  "X2-Setup Request Message failed in \
                  state CZ_GP_CONNECTED. Sending a X2-Setup \
                  Failure Response to the peer.\
                  Reverting the state to CZ_GP_CONNECTED.\n"));

         /* Falied to inform user. Now build SetUp Fail message to Peer */
         czGpmErrRspToPeer(peerCb, event, msgType);

         czCb.genSts.nmbPeerConnected++;
      } /* if(ret != ROK) */
   }/* if (eventDir == CZ_DIR_IN) */
   else
   {
      /* Its an outgoing request
       * Send it to the trasnport */

      /* RESET the peer. Since X2 SETUP REQUEST does the same thing as the RESET
       * PROCEDURE does.*/
      czNmmResetPeer(peerCb);

      peerCb->state             = CZ_GP_CONNECTING;
      /* czNmmResetPeer resets the procPending hence use previously stored
       * value */
      peerCb->gpmCb.procPending = prevProcDir | CZ_SRC_SETUP;
      czCb.genSts.nmbPeerConnected--;

      CZ_DBG_INFO((CZ_PRNT_BUF, 
               "State Changed to CZ_GP_CONNECTING from User.\n"));

      /* store the PDU for retransmission */
      ret =  czTpmStoreAndSendMsg(peerCb, &(event->pdu),
            peerCb->peerCfg.globalStreamId,
            &(peerCb->gpmCb.retxBuf));

      if(ret != ROK)
      {
         peerCb->state             = prevState;
         peerCb->gpmCb.procPending = prevProcDir;
         czCb.genSts.nmbPeerConnected++;

         CZ_DBG_FATAL((CZ_PRNT_BUF,
                  "The message %d failed to be sent.\
                  Reverting the state to CZ_GP_CONNECTED.\n", msgType));
         CZ_SEND_LCL_ERR_ALL(peerCb, CZT_ERRTYPE_MSG_SEND_FAILED, 
               ret, msgType, event, NULLP);
         ret = RFAILED;
      } /* if(ret != ROK) */
      else
      {
         CZ_FREEEVNT(event); 
      }
   } /* else ..(dir == CZ_DIR_OUT) */

   if(ROK == ret)
   {
      CZ_UPD_STS(peerCb, msgType, eventDir);
   }

   CZ_RETVALUE (ret);
}

/**********************************************************************/
/**
    @brief The function czGpmHdlSetupFail is called when X2 Setup failure 
           response/ Error Indication is being sent or received. 

    @details It is valid only in CZ_GP_CONNECTING. 
            Resolving pending crossover Requests.
            1.If the Response is outgoing, the procedure direction should have 
            CZ_TGT_SETUP set. Then the response it sent out.
            It changes the state to CZ_GP_IDLE. Clear all other procedures
            on this interface and any resources reserved.
            Else the procedure fails.
            2. If the Response message is incoming, the procedure
            direction should have CZ_SRC_SETUP set. The message is passed 
            to user.
            It changes the state to CZ_GP_IDLE. Clear all other procedures
            on this interface and any resources reserved.
            Else the procedure fails.

    @param[in] peerCb   The peerCb is the control block to the peer on whose 
                        X2-interface the message is exchanged.
    @param[in] msgType  The msg represents the X2 Message that is exchanged.
    @param[in] event    The event that has come from peer or user.
    @param[in] eventDir Event direction from user or peer.
    @return
        - ROK     - If the procedure was successful.
        - ROUTRES - If the procedure was unsuccessful due to resource crunch.
        - RFAILED - If the procedure was unsuccessful due to any failure.
*/
#ifdef ANSI
PRIVATE S16 czGpmHdlSetupFail 
(
CzPeerCb  *peerCb, 
U8         msgType, 
CztEvnt   *event, 
U8         eventDir
)
#else /* Else if ANSI not defined */
PRIVATE S16 czGpmHdlSetupFail (peerCb, msgType, event, eventDir)
CzPeerCb  *peerCb; 
U8         msgType; 
CztEvnt   *event; 
U8         eventDir;
#endif /*End of else */
{
   U8       prevState, prevProcDir;
   CztEvnt  *dstEvnt = NULLP;
   S16      ret = ROK;

   CZ_TRC2(czGpmHdlSetupFail);
   CZ_PRNT_PEERID;

   prevState   = peerCb->state;
   prevProcDir = peerCb->gpmCb.procPending;

   /* Check for the direction
      Reset all values */
   if(eventDir == CZ_DIR_OUT)
   {
      /* Its an outgoing Response */
      if(peerCb->gpmCb.procPending & CZ_TGT_SETUP)
      {
         /* Setup Procedure direction is incoming */
         /* Send to transport */

         /* RsrcCb and UeCb already cleared when request was initiated
          * No Procedures were allowed after that.*/

         czNmmResetPeer(peerCb);
         czCb.genSts.nmbPeerConnFail++;

         CZ_DBG_INFO((CZ_PRNT_BUF, 
                  "State Changed to CZ_GP_IDLE. X2-Setup Fail from User.\n"));

         ret = czTpmSndToLower(peerCb, &(event->pdu), 
               peerCb->peerCfg.globalStreamId);

         if(ret != ROK)
         {
            peerCb->state             = prevState;
            peerCb->gpmCb.procPending = prevProcDir;

            CZ_DBG_FATAL((CZ_PRNT_BUF,
                     "The message X2-Setup Failure failed to be sent.\
                     Reverting the state to CZ_GP_CONNECTING.\n"));
            CZ_SEND_LCL_ERR_ALL(peerCb, CZT_ERRTYPE_MSG_SEND_FAILED, 
                  ret, msgType, event, NULLP);
         } /* if(ret != ROK) */
         else
         {
            CZ_FREEEVNT(event); 
         }
      }
      else
      {
         /* Setup Procedure direction is outgoing */
         CZ_SEND_LCL_ERR_ALL(peerCb, CZT_ERRTYPE_INTERNAL, 
               CZT_ERRCAUSE_INV_STATE, msgType, event, NULLP);
         CZ_DBG_ERROR((CZ_PRNT_BUF, "User attempted to send X2-Setup Failure \
                  Response message for a request that was not received."));
         ret = RFAILED;

      }/* else */
   }/* if(eventDir == CZ_DIR_OUT) */
   else if(eventDir == CZ_DIR_IN)
   {
      /* Its an incoming Response */

      if(peerCb->gpmCb.procPending & CZ_SRC_SETUP)
      {
         /* Setup Procedure direction is outgoing */

         /* start wait timer if "Time To Wait" IE is present
          * in the incoming failure response */
         czNmmResetPeer(peerCb);
         czCb.genSts.nmbPeerConnFail++;

         czGpmStartWaitTmr(peerCb, &(event->pdu), 
               CZT_STATUS_SETUPREQ_TMR_STARTED); 

         CZ_DBG_INFO((CZ_PRNT_BUF, 
                  "State Changed to CZ_GP_IDLE X2-Setup Failure from Peer.\n"));

         /* indicate to the user */
         ret = czUimSndGpmCfm(peerCb, event);
         if(ret != ROK)
         {
            peerCb->state             = prevState;
            peerCb->gpmCb.procPending = prevProcDir;
            czStopTmr(CZ_TMR_TIME_TO_WAIT, peerCb);
            peerCb->gpmCb.wait = FALSE;
            ret = RFAILED;

            CZ_DBG_FATAL((CZ_PRNT_BUF, "Indication of incoming X2-Setup \
                         Failure Message Failed.\
                         Reverting the state to CZ_GP_CONNECTING.\n"));
            /* Falied to inform user. Now build Error message to Peer */
            czGpmErrRspToPeer(peerCb, event, msgType);
         } /* if(ret != ROK) */
      }/* if(peerCb->gpmCb.procPending & CZ_SRC_SETUP) */
      else
      {
         /* Setup Procedure direction is incoming */
         dstEvnt = NULLP;

         CZ_DBG_ERROR((CZ_PRNT_BUF, 
                  "X2-Setup Response Received for Request that was not\
                   sent. Replying with X2-Error Indication Message.\n"));

         /* Setup Procedure direction is incoming */
         /* generate Peer error indication */

         dstEvnt = czUtlGenPeerErrInd (&(event->pdu), msgType, NULLP, 
         /* cz001.301: Fix for the build warnings */
#ifdef CZTV1
               CZT_CAUSE_PROTOCOL,
#else
               CAUSE_PROTOCOL,
#endif
               CztCauseProtmessage_not_compatible_with_receiver_stateEnum);
         if(dstEvnt != NULLP)
         {
            czGpmSendMsg(peerCb, dstEvnt, msgType, TRUE);
         }
         CZ_FREEEVNT(event);
         ret = RFAILED;
      }/* else ..(peerCb->gpmCb.procPending) */
   }/* else if(eventDir == CZ_DIR_IN) */
   if(ROK == ret)
   { 
      CZ_UPD_STS(peerCb, msgType, eventDir);
   }
   CZ_RETVALUE(ret);
}

/**********************************************************************/


/**
    @brief The function czGpmHdlSetupReqInCntng handles Setup Request in 
           CZ_GP_CONNECTING state.
    @details 
              Reset the local node. Free all the UE procedures and the other
              global procedures.
    
    @code
    EXITING PROCEDURE | INCOMING MESSAGE |
    DIRECTION BITS    | DIRECTION        | ACTION TAKEN
    ------------------|------------------|-----------------------
   CZ_TGT_SETUP       | CZ_IN            | Probably a retransmmision from peer.
                      |                  | Indicate to the user.
                      | CZ_OUT           | Setup Procedure already initiated
                      |                  | by peer, Hence return error to the user.
                      |                  |
   CZ_SRC_SETUP       | CZ_IN            | Probably a crossover of Setup Procedures.
                      |                  | Set CZ_TGT_SETUP in proPending.
                      |                  | Indicate to the user.
                      | CZ_OUT           | Already Setup Procedure initiated by user.
                      |                  | User should use retransmission primitive.
                      |                  | Return Error to user.
                      |                  |
   CZ_TGT_RESET       | CZ_IN            | Peer behaving inconsistently. Return
                      |                  | Setup failure to the Peer.
                      | CZ_OUT           | There is a pending Reset procedure
                      |                  | initiated by peer.
                      |                  | Hence return error to the user.
                      |                  |
   CZ_SRC_RESET       | CZ_IN            | Probably a crossover of Setup and
                      |                  | Reset Procedures.
                      |                  | Set CZ_TGT_RESET in proPending.
                      |                  | Indicate to the user.
                      | CZ_OUT           | Already Reset Procedure initiated by user.
                      |                  | User should wait for this to complete.
                      |                  | Return Error to user.
    @endcode                      
    
    @param[in] peerCb   The peerCb is the control block to the peer on whose 
                        X2-interface the message is exchanged.
    @param[in] msgType  The msg represents the X2 Message that is exchanged.
    @param[in] event    The event that has come from peer or user.
    @param[in] eventDir Event direction from user or peer.
    @return
        - ROK     - If the procedure was successful.
        - ROUTRES - If the procedure was unsuccessful due to resource crunch.
        - RFAILED - If the procedure was unsuccessful due to any failure.
*/
#ifdef ANSI
PRIVATE S16 czGpmHdlSetupReqInCntng
(
CzPeerCb  *peerCb,
U8         msgType,
CztEvnt   *event,
U8         eventDir
)
#else /* Else if ANSI not defined */
PRIVATE S16 czGpmHdlSetupReqInCntng (peerCb, msgType, event, eventDir)
CzPeerCb  *peerCb;
U8         msgType; 
CztEvnt   *event;
U8         eventDir;
#endif /* End of else */
{
   U8 prevState, prevProcDir;
   S16 ret = ROK;

   CZ_TRC2(czGpmHdlSetupReqInCntng);
   CZ_PRNT_PEERID;

   prevState   = peerCb->state;
   prevProcDir = peerCb->gpmCb.procPending;

   if(eventDir == CZ_DIR_IN )
   {
      /*Request is incoming */

      /* Check if incoming RESET is pending */
      if(peerCb->gpmCb.procPending == CZ_TGT_RESET)
      {
         /* Peer behaving inconsistently
          * Reject the Reset */
         CZ_DBG_INFO((CZ_PRNT_BUF, 
                  " Peer behaving inconsistently by sending X2-Setup after a \
                  X2-Reset, reject incoming X2-Reset.\n"));
         czGpmErrRspToPeer(peerCb, event, msgType);
      }
      else
      {
         /* If procPending has CZ_SRC_SETUP or CZ_SRC_RESET is already set
          * Then it is a crossover 
          * If CZ_TGT_SETUP is already set, this is probably a retransmission */

         peerCb->gpmCb.procPending |= CZ_TGT_SETUP;

         /* Incoming Request, indicate to the user */
         ret = czUimSndGpmInd(peerCb, event);
         if(ret != ROK)
         {
            peerCb->gpmCb.procPending = prevProcDir;
            ret = RFAILED;

            CZ_DBG_FATAL((CZ_PRNT_BUF, "Indication of incoming X2-Setup " \
                     "Request Message in state CZ_GP_CONNECTING failed. \
                      Replying with an X2-Error Indication."));
            /* Falied to inform user. Now build SetUp Fail message to peer */
            czGpmErrRspToPeer(peerCb, event, msgType);
         } /* if(ret != ROK) */
         else
         {
            CZ_UPD_STS(peerCb, msgType, CZ_DIR_IN );
         }
      }
   }/* End of if (eventDir == CZ_DIR_IN)*/
   else
   {
      CZ_DBG_ERROR((CZ_PRNT_BUF, 
               "X2-Setup/Reset Procedure already in progress.\
               New Setup cannot be sent. Use retransmission \
               if necessary. \n"));
      /* Error indication to the user */
      CZ_SEND_LCL_ERR_ALL(peerCb, CZT_ERRTYPE_INTERNAL, 
            CZT_ERRCAUSE_INV_STATE, msgType, event, NULLP);
      ret = RFAILED;
   }/* End of else */
   CZ_RETVALUE (ret);
} /* End of czGpmHdlSetupReqInCntng */

/**********************************************************************/

/**
    @brief The function czGpmHdlSetupAck is called when X2 Setup Success Response.
           is being sent or received. It is valid only in CZ_GP_CONNECTING.
           
    @details 
            -# If the Response is outgoing, the procedure direction should have 
            CZ_TGT_SETUP set. Then the response it sent out and state changed to
            CZ_GP_CONNECTED. Unset the CZ_TGT_SETUP in procedure direction.
            Rest of the crossover scenarios are taken care in 
            CZ_GP_CONNECTED state. Reset the UE/resource contexts. So that fresh
            procedures can be initiated in CONNECTED state.
            Else the procedure fails.
            -# If the Response message is incoming, the procedure
            direction should have CZ_SRC_SETUP set. The message is passed 
            to user, and the state is changed to CZ_GP_CONNECTED.
            Unset the CZ_SRC_SETUP in procedure direction.
            Rest of the crossover scenarios are taken care in 
            CZ_GP_CONNECTED state.
            Else the procedure fails.

    @param[in] peerCb   The peerCb is the control block to the peer on whose 
                        X2-interface the message is exchanged.
    @param[in] msgType  The msg represents the X2 Message that is exchanged.
    @param[in] event    The event that has come from peer or user.
    @param[in] eventDir Event direction from user or peer.
    @return
        - ROK     - If the procedure was successful.
        - ROUTRES - If the procedure was unsuccessful due to resource crunch.
        - RFAILED - If the procedure was unsuccessful due to any failure.
*/
#ifdef ANSI
PRIVATE S16 czGpmHdlSetupAck
(
CzPeerCb  *peerCb,
U8         msgType, 
CztEvnt   *event,
U8         eventDir
)
#else /* Else if ANSI not defined */
PRIVATE S16 czGpmHdlSetupAck (peerCb, msgType, event, eventDir)
CzPeerCb  *peerCb;
U8         msgType; 
CztEvnt   *event;
U8         eventDir;
#endif /* End of else */
{
   U8       prevState, prevProcDir;
   CztEvnt  *dstEvnt = NULLP;
   S16      ret = ROK;

   CZ_TRC2(czGpmHdlSetupAck);
   CZ_PRNT_PEERID;

   prevState   = peerCb->state;
   prevProcDir = peerCb->gpmCb.procPending;

   if (eventDir == CZ_DIR_IN)
   {
      /* Its an incoming Setup Ack */

      if (peerCb->gpmCb.procPending & CZ_SRC_SETUP) 
      {
         /* Setup Procedure is outgoing */
         /*
            Free the retransmission PDU.
            change state to CONNECTED
            */
         CZ_FREEMBUF((peerCb->gpmCb.retxBuf));

         /* If there was a crossover
          * match for rest of the responses in CONNECTED state */
         peerCb->state = CZ_GP_CONNECTED;
         peerCb->gpmCb.procPending &= ~CZ_SRC_SETUP;
         czCb.genSts.nmbPeerConnected++;
         czCb.genSts.maxNumPeerConn++;

         CZ_DBG_INFO((CZ_PRNT_BUF, 
                  "State Changed to CZ_GP_CONNECTED by X2-Setup Ack from Peer.\
                  \n"));

         ret = czUimSndGpmCfm(peerCb, event);
         if(ret != ROK)
         {
            peerCb->state             = CZ_GP_IDLE;
            peerCb->gpmCb.procPending = 0;
            czCb.genSts.nmbPeerConnected--;
            czCb.genSts.nmbPeerConnFail++;

            CZ_DBG_FATAL((CZ_PRNT_BUF, "Indicating the incoming "\
                  "X2-Setup Ack Message failed in \
                  state CZ_GP_CONNECTING. Sending a X2-Error \
                  Indication to the peer."));
            /* 
               Falied to inform user. Now build 
               Error indication message to Peer 
               */
            czGpmErrRspToPeer(peerCb, event, msgType);
            ret                       = RFAILED;
         } /* if(ret != ROK) */
      } /* if (peerCb->gpmCb.procPending & CZ_SRC_SETUP) */
      else
      {
         /* if (peerCb->gpmCb.procPending & CZ_TGT_SETUP) */
         /* Setup Procedure is incoming */
         /* generate Peer error indication */

         CZ_DBG_ERROR((CZ_PRNT_BUF, 
                  "Setup Response received for unsent X2-Setup Request \n"));
         dstEvnt = czUtlGenPeerErrInd (&(event->pdu), msgType, NULLP,
         /* cz001.301: Fix for the build warnings */
#ifdef CZTV1
               CZT_CAUSE_PROTOCOL,
#else
               CAUSE_PROTOCOL,
#endif
               CztCauseProtmessage_not_compatible_with_receiver_stateEnum);
         if(dstEvnt != NULLP)
         {
            czGpmSendMsg(peerCb, dstEvnt, msgType, TRUE);
         }
         CZ_FREEEVNT(event);
         ret = RFAILED;
      }
   } /* if (eventDir == CZ_DIR_IN) */
   else
   {
      /* Its an outgoing Setup Ack */
      if (peerCb->gpmCb.procPending & CZ_TGT_SETUP) 
      {
         /* Setup Procedure is incoming */
         /* Send it out to the transport */

         CZ_DBG_INFO((CZ_PRNT_BUF, 
                  "State Changed to CZ_GP_CONNECTED by X2-Setup Ack from User. \
                  \n"));

         czNmmResetPeer(peerCb);
         peerCb->state              = CZ_GP_CONNECTED;
         peerCb->gpmCb.procPending = prevProcDir & ~CZ_TGT_SETUP;
         czCb.genSts.nmbPeerConnected++;
         czCb.genSts.maxNumPeerConn++;

         ret = czTpmSndToLower(peerCb, &(event->pdu), 
               peerCb->peerCfg.globalStreamId);
         if(ret != ROK)
         {
            peerCb->state             = prevState;
            peerCb->gpmCb.procPending = prevProcDir;
            czCb.genSts.nmbPeerConnected--;

            CZ_SEND_LCL_ERR_ALL(peerCb, CZT_ERRTYPE_MSG_SEND_FAILED, 
                  ret, msgType, event, NULLP);
            CZ_DBG_FATAL((CZ_PRNT_BUF,
                     "The X2-Setup Ack message failed to be sent.\
                     Reverting the state to CZ_GP_CONNECTED.\n"));
         } /* if(ret != ROK) */
         else
         {
            CZ_FREEEVNT(event);
         }
      } /* if ((peerCb->gpmCb.procPending == CZ_TGT) */
      else
      {
         /* Setup Procedure is outgoing */

         CZ_DBG_ERROR((CZ_PRNT_BUF, 
                  "X2-Setup Ack is being sent for Setup Request that is not\
                  received\n"));
         /* Send Error Indication to the User */
         CZ_SEND_LCL_ERR_ALL(peerCb, CZT_ERRTYPE_INTERNAL, \
               CZT_ERRCAUSE_INV_STATE, msgType, event, NULLP);
         ret = RFAILED;
      }
   } /* else ..(dir == CZ_DIR_OUT) */
   if(ret == ROK)
   {
      CZ_UPD_STS(peerCb, msgType, eventDir);
   }
   CZ_RETVALUE(ret);
}

/**********************************************************************/

/**
    @brief The function czGpmHdlMsg is called when private message is being sent or
           received in CZ_GP_CONNECTED state.
    @details Any private message sent or received is passed 
             transperently through the GP module .
    @param[in] peerCb   The peerCb is the control block to the peer on whose 
                        X2-interface the message is exchanged.
    @param[in] msgType  The msg represents the X2 Message that is exchanged.
    @param[in] event    The event that has come from peer or user.
    @param[in] eventDir Event direction from user or peer.
    @return
        - ROK     - If the procedure was successful.
        - ROUTRES - If the procedure was unsuccessful due to resource crunch.
        - RFAILED - If the procedure was unsuccessful due to any failure.
*/
#ifdef ANSI
PRIVATE S16 czGpmHdlMsg
(
CzPeerCb  *peerCb, 
U8         msgType, 
CztEvnt   *event, 
U8         eventDir
)
#else /* Else if ANSI not defined */
PRIVATE S16 czGpmHdlMsg (peerCb, msgType, event, eventDir)
CzPeerCb  *peerCb;
U8         msgType;
CztEvnt   *event;
U8         eventDir;
#endif /* End of else */
{
   /* Private Message */
   U8   prevState, prevProcDir;
   S16  ret = ROK;

   CZ_TRC2(czGpmHdlMsg);
   CZ_PRNT_PEERID;

   prevState   = peerCb->state;
   prevProcDir = peerCb->gpmCb.procPending;

   if (eventDir == CZ_DIR_IN)
   {
      /* Incoming Request indicate to the user */
      ret = czUimSndGpmInd(peerCb, event);
      if(ret != ROK)
      {
         CZ_DBG_FATAL((CZ_PRNT_BUF, "Indicating the incoming "\
                  "Message %d failed in \
                  state CZ_GP_CONNECTED. Sending a X2-Error \
                  Indication to the peer.", \
                  msgType));
         /* Falied to inform user. Now build SetUp Fail message to Peer */
         czGpmErrRspToPeer(peerCb, event, msgType);
      } /* if(ret != ROK) */
   }
   else
   {
      /* outgoing message, send to peer */
      czGpmSendMsg(peerCb, event, msgType, TRUE);
   }
   CZ_RETVALUE(ret);
}

/**********************************************************************/

/**
    @brief The function czGpmHdlResetInCntng is called when RESET REQ msg is
           exchanged in CZ_GP_CONNECTING state.
    @details 
 
    @code
    EXITING PROCEDURE | INCOMING MESSAGE |
    DIRECTION BITS    | DIRECTION        | ACTION TAKEN
    ------------------|------------------|-----------------------
   CZ_TGT_RESET       | CZ_IN            | Probably a retransmmision from peer.
                      |                  | Indicate to the user.
                      | CZ_OUT           | Reset Procedure already initiated
                      |                  | by peer, Hence return error to the user.
                      |                  |
   CZ_SRC_RESET       | CZ_IN            | Probably a crossover of Reset Procedures.
                      |                  | Set CZ_TGT_RESET in proPending.
                      |                  | Indicate to the user.
                      | CZ_OUT           | Already Reset Procedure initiated by user.
                      |                  | User should use retransmission primitive.
                      |                  | Return Error to user.
                      |                  |
   CZ_TGT_SETUP       | CZ_IN            | Peer behaving inconsistently. Return
                      |                  | Error Indication to the Peer.
                      | CZ_OUT           | There is a pending Setup procedure
                      |                  | initiated by peer.
                      |                  | Hence return error to the user.
                      |                  |
   CZ_SRC_SETUP       | CZ_IN            | Probably a crossover of Reset and
                      |                  | Reset Procedures.
                      |                  | Set CZ_TGT_RESET in proPending.
                      |                  | Indicate to the user.
                      | CZ_OUT           | Already Setup Procedure initiated by user.
                      |                  | User should wait for this to complete.
                      |                  | Return Error to user.
    @endcode

    @param[in] peerCb   The peerCb is the control block to the peer on whose 
                        X2-interface the message is exchanged.
    @param[in] msgType  The msg represents the X2 Message that is exchanged.
    @param[in] event    The event that has come from peer or user.
    @param[in] eventDir Event direction from user or peer.
    @return
        - ROK     - If the procedure was successful.
        - ROUTRES - If the procedure was unsuccessful due to resource crunch.
        - RFAILED - If the procedure was unsuccessful due to any failure.
*/
#ifdef ANSI
PRIVATE S16 czGpmHdlResetInCntng 
(
CzPeerCb   *peerCb, 
U8          msgType, 
CztEvnt    *event, 
U8          eventDir
)
#else /* Else if ANSI not defined */
PRIVATE S16 czGpmHdlResetInCntng (peerCb, msgType, event, eventDir)
CzPeerCb  *peerCb;
U8         msgType;
CztEvnt   *event;
U8         eventDir;
#endif /*End of else */
{
   U8 prevState, prevProcDir;
   S16 ret = ROK;

   CZ_TRC2(czGpmHdlResetInCntng);
   CZ_PRNT_PEERID;

   prevState   = peerCb->state;
   prevProcDir = peerCb->gpmCb.procPending;

   /*
      Reset all Values in Cb.
      Change state to RESET
      Set direction of the procedure to that of the message. */

   if (eventDir == CZ_DIR_IN)
   {
      /* Its an incoming RESET
       * indicate it to the user */
      /* If procPending has CZ_SRC_SETUP or CZ_SRC_RESET is already set
       * Then it is a crossover 
       * If CZ_TGT_RESET is already set, this is probably a retransmission */

      /* Check if incoming Setup is pending */
      if(peerCb->gpmCb.procPending == CZ_TGT_SETUP)
      {
         /* Peer behaving inconsistently
          * Reject the Setup */
         CZ_DBG_INFO((CZ_PRNT_BUF, 
                  " Peer behaving inconsistently,by sending a X2-Setup after \
                  an X2-Reset. Reject incoming Setup.\n"));
         czGpmErrRspToPeer(peerCb, event, msgType);
      }
      else
      {
         peerCb->gpmCb.procPending = peerCb->gpmCb.procPending | CZ_TGT_RESET;

         CZ_DBG_INFO((CZ_PRNT_BUF, 
                  "Incoming Reset in CONNECTING State.\n"));

         ret = czUimSndGpmInd(peerCb, event);
         if(ret != ROK)
         {
            peerCb->state             = prevState;
            peerCb->gpmCb.procPending = prevProcDir;
            ret = RFAILED;

            CZ_DBG_FATAL((CZ_PRNT_BUF, "Indicating the incoming "\
                     "X2-Reset Request Message failed in \
                     state CZ_GP_CONNECTING. Sending a X2-Error \
                     Indication to the peer.\
                     Reverting the state to CZ_GP_CONNECTING."));
            /* Falied to inform user. Now build SetUp Fail message to Peer */
            czGpmErrRspToPeer(peerCb, event, msgType);
         } /* if(ret != ROK) */
      }
   } /*  if (eventDir == CZ_DIR_IN) */
   else
   {
      /* Its an outgoing RESET request
       * This is an invalid scenario, since X2AP is in CZ_CONNECTING state.
       */
      CZ_DBG_ERROR((CZ_PRNT_BUF, 
               "X2-Reset Request is being sent in invalid state, \
               CZ_GP_CONNECTING. \n"));

      /* Send Error Indication to the User */
      CZ_SEND_LCL_ERR_ALL(peerCb, CZT_ERRTYPE_INTERNAL, 
            CZT_ERRCAUSE_INV_STATE, msgType,
            event, NULLP);
      ret = RFAILED;
   } /* else */

   if(ROK == ret)
   {
      CZ_UPD_STS(peerCb, msgType, eventDir);
   }
   CZ_RETVALUE (ret);
}/* end of czGpmHdlResetInCntng */

/* cz001.101 : The state CZ_GP_RESET is removed. The handlers
 * of this state are handled in state CZ_GP_CONNECTING. */

/**********************************************************************/

/**
    @brief The function czGpmHdlResetReq is called when X2 Setup Reset is being sent
           or received in CZ_GP_CONNECTED state.
    @details The state is changed to CZ_GP_CONNECTING state. 
    - If the procedure is outgoing the existing procedures are terminated, example UE HO.
             The procedure direction is set as CZ_SRC_RESET.
    - If the request is received it is set as CZ_TGT_RESET.

    @param[in] peerCb    The peerCb is the control block to the peer on whose 
                         X2-interface the message is exchanged.
    @param[in] msgType   The msg represents the X2 Message that is exchanged.
    @param[in] event     The event that has come from peer or user.
    @param[in] eventDir  Event direction from user or peer.
    @return
        - ROK     - If the procedure was successful.
        - ROUTRES - If the procedure was unsuccessful due to resource crunch.
        - RFAILED - If the procedure was unsuccessful due to any failure.
*/
#ifdef ANSI
PRIVATE S16 czGpmHdlResetReq 
(
CzPeerCb  *peerCb, 
U8         msgType, 
CztEvnt   *event, 
U8         eventDir
)
#else /* Else if ANSI not defined */
PRIVATE S16 czGpmHdlResetReq (peerCb, msgType, event, eventDir)
CzPeerCb  *peerCb;
U8         msgType;
CztEvnt   *event;
U8         eventDir;
#endif /* End of else */
{
   U8 prevState, prevProcDir;
   S16 ret = ROK;

   CZ_TRC2(czGpmHdlResetReq);
   CZ_PRNT_PEERID;

   prevState   = peerCb->state;
   prevProcDir = peerCb->gpmCb.procPending;

   /*
      Reset all Values in Cb.
      Change state to RESET
      Set direction of the procedure to that of the message. */

   if (eventDir == CZ_DIR_IN)
   {
      /* Its an incoming RESET
       * indicate it to the user */
      /* Reset peer now. We wont wait for the response from the peer */

      peerCb->state             = CZ_GP_CONNECTING;
      peerCb->gpmCb.procPending = prevProcDir | CZ_TGT_RESET;
      czCb.genSts.nmbPeerConnected--;

      CZ_DBG_INFO((CZ_PRNT_BUF, "State Changed to CZ_GP_CONNECTING by \
                             receiving X2-Reset Request from Peer.\n"));

      ret = czUimSndGpmInd(peerCb, event);
      if(ret != ROK)
      {
         peerCb->state             = prevState;
         peerCb->gpmCb.procPending = prevProcDir;
         ret = RFAILED;
         czCb.genSts.nmbPeerConnected++;

         CZ_DBG_FATAL((CZ_PRNT_BUF, "Indicating the incoming "\
                  "X2-Reset Request Message failed in \
                  state CZ_GP_CONNECTED. Sending a X2-Error \
                  Indication to the peer.\
                  Reverting the state to CZ_GP_CONNECTED."));
         /* Falied to inform user. Now build SetUp Fail message to Peer */
         czGpmErrRspToPeer(peerCb, event, msgType);
      } /* if(ret != ROK) */
   } /*  if (eventDir == CZ_DIR_IN) */
   else
   {
      /* Its an outgoing RESET request
       * Send it to the trasnport */

      /* Reset peer now. We wont wait for the response from the peer */

      czNmmResetPeer(peerCb);
      peerCb->state             = CZ_GP_CONNECTING;

      /* czNmmResetPeer would have reset the procPending
       * Hence use prevProcDir. */

      peerCb->gpmCb.procPending = prevProcDir | CZ_SRC_RESET;
      czCb.genSts.nmbPeerConnected--;

      CZ_DBG_INFO((CZ_PRNT_BUF, 
               "State Changed to CZ_GP_CONNECTING by X2-Reset Request \
               from User.\n"));

      /* Store the PDU for retransmission */
      ret =  czTpmStoreAndSendMsg(peerCb, &(event->pdu),
            peerCb->peerCfg.globalStreamId,
            &(peerCb->gpmCb.retxBuf));
      if(ret != ROK)
      {
         peerCb->state             = prevState;
         peerCb->gpmCb.procPending = prevProcDir;
         czCb.genSts.nmbPeerConnected++;

         CZ_DBG_FATAL((CZ_PRNT_BUF,
                  "Tramission of the message X2-Reset Request failed.\
                  Reverting the state to CZ_GP_CONNECTED.\n"));
         CZ_SEND_LCL_ERR_ALL(peerCb, CZT_ERRTYPE_MSG_SEND_FAILED, 
               ret, msgType , event, NULLP);
         ret = RFAILED;
      } /* if(ret != ROK) */
      else
      {
         CZ_FREEEVNT(event);
      }
   } /* else */

   if(ROK == ret)
   {
      CZ_UPD_STS(peerCb, msgType, eventDir);
   }

   CZ_RETVALUE (ret);
}/* end of czGpmHdlResetReq */

/**********************************************************************/

/**
    @brief The function czGpmHdlEnbCfgRsp handles eNB Config Update Acknowledge
           or Failure Response in CZ_GP_CONNECTED state.
    @details If it is incoming Failure message  and the message contains
             Time to wait IE, then start the timer using that value.
             Otherwise the Response message is passed through GPM transperantly towards
             peer or user.
    @param[in] peerCb The peerCb is the control block to the peer on whose 
                  X2-interface the message is exchanged.
    @param[in] msgType The msg represents the X2 Message that is exchanged.
    @param[in] event   The event that has come from peer or user.
    @param[in] eventDir Event direction from user or peer.
    @return
        - ROK     - If the procedure was successful.
        - ROUTRES - If the procedure was unsuccessful due to resource crunch.
        - RFAILED - If the procedure was unsuccessful due to any failure.
*/
#ifdef ANSI
PRIVATE S16 czGpmHdlEnbCfgRsp
(
CzPeerCb  *peerCb,
U8         msgType,
CztEvnt   *event,
U8         eventDir
)
#else /* Else if ANSI not defined */
PRIVATE S16 czGpmHdlEnbCfgRsp (peerCb, msgType, event, eventDir)
CzPeerCb  *peerCb;
U8         msgType;
CztEvnt   *event;
U8         eventDir;
#endif /* End of else */
{
   S16  ret = ROK;

   CZ_TRC2(czGpmHdlEnbCfgRsp);
   CZ_PRNT_PEERID;

   if (eventDir == CZ_DIR_IN)
   {
      /* Start wait timer if "Time To Wait" IE is present
       * in the incoming failure response 
       * In case of success response it is not present (decoding)
       */
      CZ_FREEMBUF(peerCb->gpmCb.retxBuf);

      if(msgType == CZT_MSG_CFG_UPD_FAIL)
      {
         czGpmStartWaitTmr(peerCb, &(event->pdu), 
                           CZT_STATUS_CFGUPD_TMR_STARTED); 
      }
      /* Indicate to the user */
       ret = czUimSndGpmCfm(peerCb, event);
       if(ret != ROK)
       {
          czStopTmr(CZ_TMR_TIME_TO_WAIT, peerCb);
          peerCb->gpmCb.wait = FALSE;

          ret = RFAILED;

          CZ_DBG_FATAL((CZ_PRNT_BUF, "Indicating the incoming \
                   X2-eNB Config Update Response Message failed \
                   in CZ_GP_CONNECTED state. \
                   Sending a X2-Error Indication to the peer. \n"
                   ));
          /* Falied to inform user. Now build Error Indication message to Peer */
          czGpmErrRspToPeer(peerCb, event, msgType);
       } /* if(ret != ROK) */
   }/* if (eventDir == CZ_DIR_IN) */
   else
   {
      czGpmSendMsg(peerCb, event, msgType, TRUE);
   }
   if(ROK == ret)
   {
      CZ_UPD_STS(peerCb, msgType, eventDir);
   }

   CZ_RETVALUE(ret);
}/* end of czGpmHdlEnbCfgRsp */

/**********************************************************************/

/**
    @brief The function czGpmHdlEnbCfgReq handles eNB Config Update Request
           in CZ_GP_CONNECTED state.
           
    @details If the eNB Config update is outgoing. Check if the wait timer is
            running. If yes, give an error indication to the user, indicating
            to wait for the wait timer to expire.
            First check if the wait timer is running,
            Send the message out only if retransmission buffer is empty.
            If it is not empty, that means eNB has already sent a config
            update request for which it has not received any response yet.
            Hence it cant start another config update request unless the 
            first one is completed. If user wants to retransmit the same
            CFG UPD REQ msg, he can use CzUiCztGpRetxReq primitive.

    
    @param[in] peerCb   The peerCb is the control block to the peer on whose 
                        X2-interface the message is exchanged.
    @param[in] msgType  The msg represents the X2 Message that is exchanged.
    @param[in] event    The event that has come from peer or user.
    @param[in] eventDir Event direction from user or peer.
    @return
        - ROK     - If the procedure was successful.
        - ROUTRES - If the procedure was unsuccessful due to resource crunch.
        - RFAILED - If the procedure was unsuccessful due to any failure.
*/
#ifdef ANSI
PRIVATE S16 czGpmHdlEnbCfgReq
(
CzPeerCb  *peerCb, 
U8         msgType, 
CztEvnt   *event, 
U8         eventDir
)
#else /* Else if ANSI not defined */
PRIVATE S16 czGpmHdlEnbCfgReq(peerCb, msgType, event, eventDir)
CzPeerCb  *peerCb;
U8         msgType;
CztEvnt   *event;
U8         eventDir;
#endif /* End of else */
{
   U8   prevState, prevProcDir;
   S16  ret = ROK;

   CZ_TRC2(czGpmHdlEnbCfgReq);
   CZ_PRNT_PEERID;

   prevState   = peerCb->state;
   prevProcDir = peerCb->gpmCb.procPending;

   if (eventDir == CZ_DIR_IN)
   {
      /* Its an incoming eNodeB Config Request
       * indicate it to the user */

      ret = czUimSndGpmInd(peerCb, event);
      if(ret != ROK)
      {
         /* Falied to inform user. Now build SetUp Fail message to Peer */
         ret = RFAILED;

         CZ_DBG_FATAL((CZ_PRNT_BUF, "Indicating the incoming "\
                  "X2-Config Update Request Message failed in \
                  state CZ_GP_CONNECTED. Sending a X2-eNB \
                  Config Update Failure Response to the peer." \
                  ));
         czGpmErrRspToPeer(peerCb, event, msgType);
      } /* if(ret != ROK) */
   }/* if (eventDir == CZ_DIR_IN) */
   else
   {
      /* Its an outgoing eNodeB Config request
       * Send it to the trasnport */
      /* First check if the wait timer is running,
       * if send the Request only if it is not running else send error Ind */
      /* Send the message out only if gpmCb.retxBuf is NULLP. If it is not NULL,
       * that means eNB has already sent a config update request for which it
       * has not received any response yet. And it cant start another config update
       * request unless the first one is completed. If user wants to resend the
       * same CFG UPD REQ msg, he can use CzUiCztGpRetxReq primitive */

      if (peerCb->gpmCb.wait != TRUE && 
            peerCb->gpmCb.retxBuf == NULLP)
      {
         /* Store buffer for retransmission before sending */

         ret = czTpmStoreAndSendMsg(peerCb, &(event->pdu), 
               peerCb->peerCfg.globalStreamId,
               &(peerCb->gpmCb.retxBuf)); 
         if(ret == ROK)
         {
            CZ_FREEEVNT(event); 
         }
      } /* if (peerCb->gpmCb.wait != TRUE) */
      else
      {
         if(peerCb->gpmCb.wait)
         {
            CZ_SEND_LCL_ERR_ALL(peerCb, CZT_ERRTYPE_INTERNAL, \
                  CZT_ERRCAUSE_WAIT_TMR_ON, \
                  CZT_MSG_CFG_UPD_REQ,event, NULLP);
            CZ_DBG_ERROR((CZ_PRNT_BUF, 
                     "Wait timer is running for eNB Config Update Message.\
                     Hence cannot send the new Request.\n"));
         }
         else
         {
            CZ_SEND_LCL_ERR_ALL(peerCb, CZT_ERRTYPE_INTERNAL, \
                  CZT_ERRCAUSE_PARALLEL_CFGUPD_TRANS, \
                  CZT_MSG_CFG_UPD_REQ, event, NULLP);
            CZ_DBG_ERROR((CZ_PRNT_BUF, "No parallel transaction allowed for \
                     config update request. Hence cannot send the Request.\n"));
         }
         ret = RFAILED;
      }/* else */

   }/* else ..(eventDir == CZ_DIR_OUT) */

   if(ROK == ret)
   { 
      CZ_UPD_STS(peerCb, msgType, eventDir);
   }
   else
   {
       CZ_DBG_FATAL((CZ_PRNT_BUF, "Failed to transmit the X2-eNB Config Update \
                     Request Message state CZ_GP_CONNECTED.\n"));
   }
   CZ_RETVALUE(ret);
}/* czGpmHdlEnbCfgReq */

/**********************************************************************/

/**
    @brief The function czGpmHdlResetRsp handles Reset Response in
            CZ_GP_CONNECTING state.

    @details
            The state is moved to CZ_GP_CONNECTED.

            -# If the Response is outgoing, the procedure direction should have 
            CZ_TGT_RESET set. Then the response it sent out and state changed to
            CZ_GP_CONNECTED. Unset the CZ_TGT_RESET in procedure direction.
            Rest of the crossover scenarios are taken care in 
            CZ_GP_CONNECTED state. The UE/Resource contexts are reset so that
            new procedures can be started in CONNECTED state.
            Else the procedure fails.
            -# If the Response message is incoming, the procedure
            direction should have CZ_SRC_RESET set. The message is passed 
            to user, and the state is changed to CZ_GP_CONNECTED.
            Unset the CZ_SRC_RESET in procedure direction.
            Rest of the crossover scenarios are taken care in 
            CZ_GP_CONNECTED state.
            Else the procedure fails.

    @param[in] peerCb The peerCb is the control block to the peer on whose 
                  X2-interface the message is exchanged.
    @param[in] msgType The msg represents the X2 Message that is exchanged.
    @param[in] event   The event that has come from peer or user.
    @param[in] eventDir Event direction from user or peer.
    @return
        - ROK     - If the procedure was successful.
        - ROUTRES - If the procedure was unsuccessful due to resource crunch.
        - RFAILED - If the procedure was unsuccessful due to any failure.
*/
#ifdef ANSI
PRIVATE S16 czGpmHdlResetRsp
(
CzPeerCb   *peerCb,
U8          msgType,
CztEvnt    *event,
U8          eventDir
)
#else /* Else if ANSI not defined */
PRIVATE S16 czGpmHdlResetRsp (peerCb, msgType, event, eventDir)
CzPeerCb   *peerCb;
U8          msgType;
CztEvnt    *event;
U8          eventDir;
#endif /* End of else */
{
   CztEvnt   *dstEvnt;
   U8         prevState, prevProcDir;
   S16        ret = ROK;

   CZ_TRC2(czGpmHdlResetRsp);
   CZ_PRNT_PEERID;

   prevState   = peerCb->state;
   prevProcDir = peerCb->gpmCb.procPending;

   /* Move to CONNECTED state; Delete all the UeCbs and RsrcCbs */
   if (eventDir == CZ_DIR_IN)
   {
      /* Its an incoming Reset Response */
      if (peerCb->gpmCb.procPending & CZ_SRC_RESET) 
      {
         /* Free the retransmission PDU. Change state to CONNECTED */
         CZ_FREEMBUF(peerCb->gpmCb.retxBuf);
         CZ_DBG_INFO((CZ_PRNT_BUF, 
                  "State Changed to CZ_GP_CONNECTED by X2-Reset Response \
                  from Peer.\n"));

         /* If there was a crossover
          * match for rest of the responses in CONNECTED state */
         peerCb->state             = CZ_GP_CONNECTED;
         peerCb->gpmCb.procPending &= ~CZ_SRC_RESET;
         czCb.genSts.nmbPeerConnected++;

         ret = czUimSndGpmCfm(peerCb, event);
         if(ret != ROK)
         {
            peerCb->state             = prevState;
            peerCb->gpmCb.procPending = prevProcDir;
            ret = RFAILED;
            czCb.genSts.nmbPeerConnected--;

            CZ_DBG_FATAL((CZ_PRNT_BUF, "Indicating the incoming "\
                     "X2-Reset Response Message failed in \
                     state CZ_GP_CONNECTING. Sending a X2-Error \
                     Indication to the peer.\
                     Reverting the state to CZ_GP_CONNECTING." \
                     ));

            /* Falied to inform user. Now build Error Ind message to Peer */
            czGpmErrRspToPeer(peerCb, event, msgType);
         } /* if(ret != ROK) */
      } /* if ((peerCb->gpmCb.procPending == CZ_SRC) */
      else
      {
         dstEvnt = NULLP;
         /* Reset Procedure is incoming */
         /* Send Error Indication to the Peer */

         CZ_DBG_ERROR((CZ_PRNT_BUF, 
                  "Reset Response received for Reset Request that was never \
                  sent. Sending an X2- Error Indication to the peer.\n"));
         dstEvnt = czUtlGenPeerErrInd (&(event->pdu), msgType, NULLP, 
         /* cz001.301: Fix for the build warnings */
#ifdef CZTV1
               CZT_CAUSE_PROTOCOL,
#else
               CAUSE_PROTOCOL,
#endif
               CztCauseProtmessage_not_compatible_with_receiver_stateEnum);
         if(dstEvnt != NULLP)
         {
            czGpmSendMsg(peerCb, dstEvnt, msgType, TRUE);
         }
         CZ_FREEEVNT(event);
         ret = RFAILED;
      }/* else ..peerCb->gpmCb.procPending == CZ_TGT */
   } /* if (eventDir == CZ_DIR_IN) */
   else
   {
      /* Its an outgoing Reset Response */
      if (peerCb->gpmCb.procPending & CZ_TGT_RESET) 
      {
         /* Send it out to the transport */
         CZ_FREEMBUF(peerCb->gpmCb.retxBuf);
         CZ_DBG_INFO((CZ_PRNT_BUF, 
                     "State Changed to CZ_GP_CONNECTED by X2-Reset Response \
                      from the User.\n"));

         czNmmResetPeer(peerCb);
         peerCb->state              = CZ_GP_CONNECTED;
         peerCb->gpmCb.procPending = (prevProcDir & ~CZ_TGT_RESET);
         czCb.genSts.nmbPeerConnected++;

         ret = czTpmSndToLower(peerCb, &(event->pdu), 
               peerCb->peerCfg.globalStreamId); 
         if(ret != ROK)
         {
            peerCb->state             = prevState;
            peerCb->gpmCb.procPending = prevProcDir;
            czCb.genSts.nmbPeerConnected--;

            CZ_DBG_FATAL((CZ_PRNT_BUF,
                     "Transmitting the message X2-Reset Response failed.\
                     Reverting the state to CZ_GP_CONNECTING.\n"));
            CZ_SEND_LCL_ERR_ALL(peerCb, CZT_ERRTYPE_MSG_SEND_FAILED, 
                  ret, msgType, event, NULLP);
         } /* if(ret != ROK) */
         else
         {
            CZ_FREEEVNT(event);
         }
      } /* if ((peerCb->gpmCb.procPending == CZ_TGT) */
      else
      {
         /* Reset Procedure is outgoing */
         /* Send Error Indication to the User */
         ret = RFAILED;

         CZ_DBG_ERROR((CZ_PRNT_BUF, 
                  "Reset Response sent for X2-Reset Request that was never \
                  received.\n"));
         CZ_SEND_LCL_ERR_ALL(peerCb, CZT_ERRTYPE_INTERNAL, 
               CZT_ERRCAUSE_INV_STATE, msgType, event, NULLP);
      } /* else peerCb->gpmCb.procPending == CZ_SRC */
   } /* else .. dir = CZ_DIR_OUT */ 

   if(ROK == ret)
   {
      CZ_UPD_STS(peerCb, msgType, eventDir);
   }
   CZ_RETVALUE(ret);
}/* end of czGpmHdlResetRsp */

/**********************************************************************/

/**
    @brief The function czGpmHdlError is called when an event is invalid 
           in a particular state.
           
    @details
           It returns RFAILED. Logs the scenario.
           If the error was due to outgoing message local error indication
           is given to the user.
           
           If the error was due to incoming message, appropriate error
           message is formed and sent to the peer. Find it below.

    @param[in] peerCb The peerCb is the control block to the peer on whose 
                  X2-interface the message is exchanged.
    @param[in] msgType The msg represents the X2 Message that is exchanged.
    @param[in] event   The event that has come from peer or user.
    @param[in] eventDir Event direction from user or peer.
    @return
        - RFAILED - The procedure was unsuccessful due to any failure.
*/
#ifdef ANSI
PRIVATE S16 czGpmHdlError
(
CzPeerCb  *peerCb,
U8         msgType,
CztEvnt   *event,
U8         eventDir
)
#else /* Else if ANSI not defined */
PRIVATE S16 czGpmHdlError (peerCb, msgType, event, eventDir)
CzPeerCb  *peerCb;
U8         msgType;
CztEvnt   *event;
U8         eventDir;
#endif /* End of else */
{
   CztEvnt *dstEvnt;
   U8       prevState, prevProcDir;

   CZ_TRC2(czGpmHdlError);
   CZ_PRNT_PEERID;
   
   prevState   = peerCb->state;
   prevProcDir = peerCb->gpmCb.procPending;
   
   if (eventDir == CZ_DIR_IN)
   {
      dstEvnt = NULLP;

      /* Its an incoming X2 Message
       * Send a failure response */

      CZ_DBG_ERROR((CZ_PRNT_BUF, 
         "X2 Message %d received in state %d. Sending Error Response to the \
         peer.\n", msgType, prevState));

      dstEvnt = czUtlGenPeerErrInd (&(event->pdu), msgType, NULLP,
         /* cz001.301: Fix for the build warnings */
#ifdef CZTV1
                                    CZT_CAUSE_PROTOCOL,
#else
                                    CAUSE_PROTOCOL,
#endif
            CztCauseProtmessage_not_compatible_with_receiver_stateEnum);

      if(dstEvnt != NULLP)
      {
         czGpmSendMsg(peerCb, dstEvnt, msgType, TRUE);
      }
      CZ_FREEEVNT(event);
   }/* if (eventDir == CZ_DIR_IN) */ 
   else
   {
      /* Its an outgoing X2 Message
       * return an Error */

      CZ_DBG_ERROR((CZ_PRNT_BUF, 
               "X2 Message %d attempted to send in state %d\n",
               msgType, prevState));
      /* local error indication */
      CZ_SEND_LCL_ERR_ALL(peerCb, CZT_ERRTYPE_INTERNAL, \
                          CZT_ERRCAUSE_INV_STATE, msgType, event, NULLP);
   }
   CZ_RETVALUE (RFAILED);
}

/**********************************************************************/

/**
    @brief The function czGpmHdlEnbCfgUpdErr handles eNB Config Update Request
           in any state other than CZ_GP_CONNECTED state.

    @details If it is incoming eNB Config Update Request, eNB Config Failure
             response is being sent. Else if it is outgoing error is indicated to the user.

    @param[in] peerCb The peerCb is the control block to the peer on whose 
                  X2-interface the message is exchanged.
    @param[in] msgType The msg represents the X2 Message that is exchanged.
    @param[in] event   The event that has come from peer or user.
    @param[in] eventDir Event direction from user or peer.
    @return
        - ROK     - If the procedure was successful.
        - ROUTRES - If the procedure was unsuccessful due to resource crunch.
        - RFAILED - If the procedure was unsuccessful due to any failure.
*/
#ifdef ANSI
PRIVATE S16 czGpmHdlEnbCfgUpdErr
(
CzPeerCb  *peerCb,
U8        msgType,
CztEvnt   *event,
U8        eventDir
)
#else /* Else if ANSI not defined */
PRIVATE S16 czGpmHdlEnbCfgUpdErr (peerCb, msgType, event, eventDir)
CzPeerCb  *peerCb;
U8        msgType;
CztEvnt   *event;
U8        eventDir;
#endif /* End of else */
{
   CztEvnt *dstEvnt;
   U8      prevState, prevProcDir;

   CZ_TRC2(czGpmHdlEnbCfgUpdErr);
   CZ_PRNT_PEERID;

   prevState   = peerCb->state;
   prevProcDir = peerCb->gpmCb.procPending;

   if (eventDir == CZ_DIR_IN)
   {
      /* Its an incoming eNodeB Config Request
       * Send a failure response */

      CZ_DBG_ERROR((CZ_PRNT_BUF, 
               "eNodeB Config Update Request Message received in invalid \
               state %d. eNodeB Config Update Failure response sent. \n",
               prevState));

      dstEvnt = czUtlGenX2CfgUpdFailure (&(event->pdu), 
         /* cz001.301: Fix for the build warnings */
#ifdef CZTV1
                                 CZT_CAUSE_PROTOCOL,
#else
                                 CAUSE_PROTOCOL,
#endif
            CztCauseProtmessage_not_compatible_with_receiver_stateEnum);

      CZ_FREEEVNT(event);
      if(dstEvnt != NULLP)
      {
         czGpmSendMsg(peerCb, dstEvnt, msgType, TRUE);
      }
   } /* if (eventDir == CZ_DIR_IN) */
   else
   {
      /* Its an outgoing eNodeB Config request
       * return an Error */

      CZ_DBG_ERROR((CZ_PRNT_BUF, 
               "eNodeB Config Update Message attempted to send in invalid \
               state %d\n", prevState));
      /* local error indication */
      CZ_SEND_LCL_ERR_ALL(peerCb, CZT_ERRTYPE_INTERNAL, 
            CZT_ERRCAUSE_INV_STATE, msgType, event, NULLP);

   }
   CZ_RETVALUE(RFAILED);
}/* end of czGpmHdlEnbCfgUpdErr */

/**********************************************************************/
/**
    @brief The function czGpmHdlSetupAckInCnted handles eNB Config Update Request
           in any state other than CZ_GP_CONNECTED state.
           
    @details
            Resolving pending crossover Requests.
            -# If the Response is outgoing, the procedure direction should have 
            CZ_TGT_SETUP set. Then the response it sent out.
            Unset the CZ_TGT_SETUP in procedure direction.
            Else the procedure fails.
            -# If the Response message is incoming, the procedure
            direction should have CZ_SRC_SETUP set. The message is passed 
            to user. If indication fails state is changed to IDLE.
            Unset the CZ_SRC_SETUP in procedure direction.
            Else the procedure fails.

    @param[in] peerCb The peerCb is the control block to the peer on whose 
                  X2-interface the message is exchanged.
    @param[in] msgType The msg represents the X2 Message that is exchanged.
    @param[in] event   The event that has come from peer or user.
    @param[in] eventDir Event direction from user or peer.
    @return
        - ROK     - If the procedure was successful.
        - ROUTRES - If the procedure was unsuccessful due to resource crunch.
        - RFAILED - If the procedure was unsuccessful due to any failure.
*/
#ifdef ANSI
PRIVATE S16 czGpmHdlSetupAckInCnted
(
CzPeerCb   *peerCb,
U8          msgType,
CztEvnt    *event,
U8          eventDir
)
#else /* Else if ANSI not defined */
PRIVATE S16 czGpmHdlSetupAckInCnted (peerCb, msgType, event, eventDir)
CzPeerCb  *peerCb;
U8         msgType;
CztEvnt   *event;
U8         eventDir;
#endif /* End of else */
{
   CztEvnt  *dstEvnt;
   Bool     freeEvent = TRUE;
   S16      ret = ROK;

   CZ_TRC2(czGpmHdlSetupAckInCnted);
   CZ_PRNT_PEERID;

   if (eventDir == CZ_DIR_IN)
   {
      /* Its an incoming Setup Ack */
      if (peerCb->gpmCb.procPending & CZ_SRC_SETUP)  
      {
         /* Setup Procedure is outgoing */
         /* no more responses expected */
         peerCb->gpmCb.procPending &= ~CZ_SRC_SETUP;

         CZ_DBG_INFO((CZ_PRNT_BUF, "Setup response for crossed over request \
                  received.\n"));

         freeEvent   = FALSE;
         /*  Free the retransmission PDU. */
         CZ_FREEMBUF(peerCb->gpmCb.retxBuf);

         /* Indicate to the user */
         ret = czUimSndGpmCfm(peerCb, event);
         if(ret != ROK)
         {
            czNmmResetPeer(peerCb);
            czCb.genSts.nmbPeerConnFail++;
            czCb.genSts.nmbPeerConnected--;
            ret = RFAILED;

            CZ_DBG_FATAL((CZ_PRNT_BUF, "Indicating the incoming "\
                     "X2-Setup Ack Message failed in \
                     state CZ_GP_CONNECTED. Sending an X2-Error \
                     Indication to the peer." \
                     ));
            /* Falied to inform user. Now build Err Ind message to Peer */
            czGpmErrRspToPeer(peerCb, event, msgType);
         } /* if(ret != ROK) */
      }/* if (peerCb->gpmCb.procPending == CZ_SRC) */
      else
      {
         dstEvnt = NULLP;
         /* Setup Procedure is incoming */

         /* generate Peer error indication */
         CZ_DBG_ERROR((CZ_PRNT_BUF, 
                  "Setup Ack received for Setup Request that was\
                  never sent. Sending an Error Indication to the peer.\n"));

         dstEvnt = czUtlGenPeerErrInd (&(event->pdu), msgType, NULLP,
         /* cz001.301: Fix for the build warnings */
#ifdef CZTV1
               CZT_CAUSE_PROTOCOL,
#else
               CAUSE_PROTOCOL,
#endif
               CztCauseProtmessage_not_compatible_with_receiver_stateEnum);

         if(dstEvnt != NULLP)
         {
            czGpmSendMsg(peerCb, dstEvnt, msgType, TRUE);
         }
         ret = RFAILED;
      }/* else */
   }/* if (eventDir == CZ_DIR_IN) */
   else
   {
      /* Its an outgoing Setup Ack */
      if (peerCb->gpmCb.procPending & CZ_TGT_SETUP)
      {
         /* Setup Procedure is incoming */

         /* no more responses expected */
         peerCb->gpmCb.procPending &= ~CZ_TGT_SETUP;

         CZ_DBG_INFO((CZ_PRNT_BUF, 
                  "X2-Setup Ack for crossed over request \
                  sent.\n"));

         ret = czTpmSndToLower(peerCb, &(event->pdu), 
               peerCb->peerCfg.globalStreamId); 
         if(ret != ROK)
         {
            CZ_DBG_FATAL((CZ_PRNT_BUF,
                     "Transmitting of the message X2-Setup Ack failed\
                     in CZ_GP_CONNECTED state.\n"));
            freeEvent = FALSE;

            CZ_SEND_LCL_ERR_ALL(peerCb, CZT_ERRTYPE_MSG_SEND_FAILED, 
                  ret, msgType, event, NULLP);
         } /* if(ret != ROK) */
      }
      else
      {
         /* Setup Procedure is outgoing */
         ret = RFAILED;

         CZ_DBG_ERROR((CZ_PRNT_BUF, 
                  "Setup Ack attempted to send for Setup Request\
                  that was never received.\n"));

         /* Send Error Indication to the User */
         freeEvent   = FALSE;
         CZ_SEND_LCL_ERR_ALL(peerCb, CZT_ERRTYPE_INTERNAL, \
               CZT_ERRCAUSE_INV_STATE, msgType, event, NULLP);
      }
   }/* else ..dir = CZ_DIR_OUT */
   if(ROK == ret)
   {
      CZ_UPD_STS(peerCb, msgType, eventDir);
   }
   if(freeEvent)
   {
      CZ_FREEEVNT(event);
   }
   CZ_RETVALUE(ret);

}/* end of czGpmHdlSetupAckInCnted */

/**********************************************************************/

/**
    @brief The function czGpmHdlSetupFailInCnted handles eNB Config Update
           Request in any state other than CZ_GP_CONNECTED state.
    @details 
            Resolving pending crossover Requests.
            -# If the Response is outgoing, the procedure direction should have 
            CZ_TGT_SETUP set. Then the response it sent out.
            It changes the state to CZ_GP_IDLE. Clear all other procedures
            on this interface and any resources reserved.
            Else the procedure fails.
            -# If the Response message is incoming, the procedure
            direction should have CZ_SRC_SETUP set. The message is passed 
            to user.
            It changes the state to CZ_GP_IDLE. Clear all other procedures
            on this interface and any resources reserved.
            Else the procedure fails.

    @param[in] peerCb   The peerCb is the control block to the peer on whose 
                        X2-interface the message is exchanged.
    @param[in] msgType  The msg represents the X2 Message that is exchanged.
    @param[in] event    The event that has come from peer or user.
    @param[in] eventDir Event direction from user or peer.
    @return
        - ROK     - If the procedure was successful.
        - ROUTRES - If the procedure was unsuccessful due to resource crunch.
        - RFAILED - If the procedure was unsuccessful due to any failure.
*/
#ifdef ANSI
PRIVATE S16 czGpmHdlSetupFailInCnted
(
CzPeerCb  *peerCb,
U8         msgType,
CztEvnt   *event,
U8         eventDir
)
#else /* Else if ANSI not defined */
PRIVATE S16 czGpmHdlSetupFailInCnted (peerCb, msgType, event, eventDir)
CzPeerCb  *peerCb;
U8         msgType;
CztEvnt   *event;
U8         eventDir;
#endif /* End of else */
{
   CztEvnt *dstEvnt;
   U8      prevState, prevProcDir;
   Bool    freeEvnt = TRUE;
   S16     ret = ROK;

   CZ_TRC2(czGpmHdlSetupFailInCnted);
   CZ_PRNT_PEERID;

   prevState   = peerCb->state;
   prevProcDir = peerCb->gpmCb.procPending;

   /* Check for the direction
      Reset all values */

   if (eventDir == CZ_DIR_OUT)
   {
      if(peerCb->gpmCb.procPending & CZ_TGT_SETUP)
      {
         /* Setup Procedure direction is incoming */
         /* Send to transport */
         /* Clear RsrcCb and UeCb */
         czNmmResetPeer(peerCb);
         czCb.genSts.nmbPeerConnected--;
         czCb.genSts.nmbPeerConnFail++;

         CZ_DBG_INFO((CZ_PRNT_BUF,"State Changed to CZ_GP_IDLE by X2-Setup \
                  Failure from User.\n"));

         ret = czTpmSndToLower(peerCb, &(event->pdu), 
               peerCb->peerCfg.globalStreamId); 
         if(ret != ROK)
         {
            peerCb->state             = prevState;
            peerCb->gpmCb.procPending = prevProcDir;
            freeEvnt    = FALSE;
            
            CZ_DBG_FATAL((CZ_PRNT_BUF,
                     "Transmitting the message X2-Setup Failure failed.\
                     Reverting the state to CZ_GP_CONNECTED.\n"));
            CZ_SEND_LCL_ERR_ALL(peerCb, CZT_ERRTYPE_MSG_SEND_FAILED, 
                  ret, msgType, event, NULLP);
         } /* if(ret != ROK) */
      }/* else */
      /* Its an outgoing Response */
      else
      {
         CZ_DBG_ERROR((CZ_PRNT_BUF, 
                  "Setup Response attempted to send to a Request\
                  that was never received.\n"));
         /* Setup Procedure direction is outgoing */
         CZ_SEND_LCL_ERR_ALL(peerCb, CZT_ERRTYPE_INTERNAL,
               CZT_ERRCAUSE_INV_STATE, msgType, event, NULLP);
         ret = RFAILED;
         freeEvnt    = FALSE;
      }
   }/* if (eventDir == CZ_DIR_OUT) */
   else
   {
      /* Its an incoming Response */
      if(peerCb->gpmCb.procPending & CZ_SRC_SETUP)
      {
         /* Setup Procedure direction is outgoing */

         /* start wait timer if "Time To Wait" IE is present
          * in the incoming failure response */

         czGpmStartWaitTmr(peerCb, &(event->pdu), 
                            CZT_STATUS_SETUPREQ_TMR_STARTED); 

         /* Clear RsrcCb and UeCb */
         czNmmResetPeer(peerCb);
         czCb.genSts.nmbPeerConnFail++;
         czCb.genSts.nmbPeerConnected--;

         CZ_DBG_INFO((CZ_PRNT_BUF,"State Changed to CZ_GP_IDLE by X2-Setup \
                  Failure from Peer.\n"));

         freeEvnt    = FALSE;
         /* Release the PDU kept for retranmission */
         CZ_FREEMBUF(peerCb->gpmCb.retxBuf);

         /* Indicate to the user */
         ret = czUimSndGpmCfm(peerCb, event);
         if(ret != ROK)
         {
            peerCb->state             = prevState;
            peerCb->gpmCb.procPending = prevProcDir;
            czStopTmr(CZ_TMR_TIME_TO_WAIT, peerCb);
            peerCb->gpmCb.wait = FALSE;
            ret = RFAILED;

            CZ_DBG_FATAL((CZ_PRNT_BUF, "Indicating the incoming "\
                     "X2-Setup Failure Message failed. \
                     Sending a X2-Error \
                     Indication to the peer.\
                     Reverting the state to CZ_GP_CONNECTED." \
                     ));
            /* Falied to inform user. Now build Error Indication message to Peer */
            czGpmErrRspToPeer(peerCb, event, msgType);
         } /* if(ret != ROK) */
      } /* if(peerCb->gpmCb.procPending & CZ_SRC) */
      else
      {
         /* Setup Procedure direction is incoming */
         dstEvnt = NULLP;
         CZ_DBG_ERROR((CZ_PRNT_BUF, "Setup Response received for a Request \
                that was never sent. Sending Error Indication to the peer.\n"));

         /* Setup Procedure direction is incoming */
         /* generate Peer error indication */
         dstEvnt = czUtlGenPeerErrInd (&(event->pdu), msgType, NULLP,
         /* cz001.301: Fix for the build warnings */
#ifdef CZTV1
               CZT_CAUSE_PROTOCOL,
#else
               CAUSE_PROTOCOL,
#endif
               CztCauseProtmessage_not_compatible_with_receiver_stateEnum);
         if(dstEvnt != NULLP)
         {
            czGpmSendMsg(peerCb, dstEvnt, msgType, TRUE);
         }
         ret = RFAILED;
      } /* else */
   } /* else ..dir == CZ_DIR_IN */
   if(ROK == ret)
   {
      CZ_UPD_STS(peerCb, msgType, eventDir);
   }
   if(freeEvnt == TRUE)
   { 
      CZ_FREEEVNT(event);
   }

   CZ_RETVALUE(ret);
}/* end of czGpmHdlSetupFailInCnted */

/**********************************************************************/

/**
    @brief The function czGpmHdlResetRspInCnted handles RESET RESPONSE in CZ_GP_CONNECTED state.
    
    @details
            This function handles the reset response in connected state.
            Resolving pending crossover Requests.
            -# If the Response is outgoing, the procedure direction should have 
            CZ_TGT_RESET set. Then the response it sent out.
            Unset the CZ_TGT_RESET in procedure direction.
            Else the procedure fails.
            -# If the Response message is incoming, the procedure
            direction should have CZ_SRC_RESET set. The message is passed 
            to user.
            Unset the CZ_SRC_RESET in procedure direction.
            Else the procedure fails.

    @param[in] peerCb The peerCb is the control block to the peer on whose 
                  X2-interface the message is exchanged.
    @param[in] msgType  Message type .
    @param[in] event    The msg represents the X2 Message that is exchanged.
    @param[in] eventDir Direction whether from user or from target.
    @return
        - ROK     - If the procedure was successful.
        - ROUTRES - If the procedure was unsuccessful due to resource crunch.
        - RFAILED - If the procedure was unsuccessful due to any failure.
*/
#ifdef ANSI
PRIVATE S16 czGpmHdlResetRspInCnted
(
CzPeerCb   *peerCb,
U8          msgType,
CztEvnt    *event,
U8          eventDir
)
#else /* Else if ANSI not defined */
PRIVATE S16 czGpmHdlResetRspInCnted (peerCb, msgType, event, eventDir)
 CzPeerCb  *peerCb;
 U8         msgType;
 CztEvnt   *event;
 U8         eventDir;
#endif /* End of else */
{
   CztEvnt *dstEvnt = NULLP;
   S16      ret = ROK;

   CZ_TRC2(czGpmHdlResetRspInCnted);
   CZ_PRNT_PEERID;

   if (eventDir == CZ_DIR_IN)
   {
      /* Its an incoming Reset Ack */
      if (peerCb->gpmCb.procPending & CZ_SRC_RESET)  
      {
         /* Reset Procedure is outgoing */
         /*
            Free the retransmission PDU.
            state is already CONNECTED
            */
         CZ_FREEMBUF(peerCb->gpmCb.retxBuf);

         /* no more responses expected */
         peerCb->gpmCb.procPending &= ~CZ_SRC_RESET;

         CZ_DBG_INFO((CZ_PRNT_BUF,"Reset response for crossed over request \
                  received.\n"));
         /* Indicate to the user */
         ret = czUimSndGpmCfm(peerCb, event);
         if(ret != ROK)
         {
            ret = RFAILED;

            CZ_DBG_FATAL((CZ_PRNT_BUF, "Indicating the incoming " \
                     "X2-Reset Response Message failed in \
                     CZ_GP_CONNECTED state. Sending an \
                     X2-Error Indication to the peer.\n" \
                     ));

            /* Falied to inform user. Now build Error Indication message to Peer */
            czGpmErrRspToPeer(peerCb, event, msgType);
         } /* if(ret != ROK) */
      }
      else
      {
         dstEvnt = NULLP;
         /* Reset Procedure is incoming */

         /* generate Peer error indication */
         CZ_DBG_ERROR((CZ_PRNT_BUF, 
                  "Reset Response received for Reset Request that\
                  was never sent. Sending an Error indication to the peer.\n"));
         dstEvnt = czUtlGenPeerErrInd (&(event->pdu), msgType, NULLP,
         /* cz001.301: Fix for the build warnings */
#ifdef CZTV1
               CZT_CAUSE_PROTOCOL,
#else
               CAUSE_PROTOCOL,
#endif
               CztCauseProtmessage_not_compatible_with_receiver_stateEnum);

         if(dstEvnt != NULLP)
         {
            czGpmSendMsg(peerCb, dstEvnt, msgType, TRUE);
         }
         CZ_FREEEVNT(event);
         ret = RFAILED;
      }
   }/* if (eventDir == CZ_DIR_IN) */
   else
   {
      /* Its an outgoing Reset Ack */
      if (peerCb->gpmCb.procPending & CZ_TGT_RESET)
      {
         /* Reset Procedure is incoming */
         /* no more responses expected */
         peerCb->gpmCb.procPending &= ~CZ_TGT_RESET;

         CZ_DBG_INFO((CZ_PRNT_BUF, 
                  "Reset response for crossed over request \
                  sent.\n"));

         ret = czTpmSndToLower(peerCb, &(event->pdu), 
               peerCb->peerCfg.globalStreamId); 
         if(ret != ROK)
         {
            CZ_DBG_FATAL((CZ_PRNT_BUF,
                     "Transmitting the X2-Reset Response message failed\
                     in CZ_GP_CONNECTED state.\n"));
            CZ_SEND_LCL_ERR_ALL(peerCb, CZT_ERRTYPE_MSG_SEND_FAILED, 
                  ret, msgType, event, NULLP);
         } /* if(ret != ROK) */
         else
         {
            CZ_FREEEVNT(event);
         }
      }
      else
      {
         /* Reset Procedure is outgoing */

         CZ_DBG_ERROR((CZ_PRNT_BUF, 
                  "Reset Response attempted to send for Reset\
                  Request that was never received.\n"));

         /* Send Error Indication to the User */
         CZ_SEND_LCL_ERR_ALL(peerCb, CZT_ERRTYPE_INTERNAL, \
               CZT_ERRCAUSE_INV_STATE, msgType, \
               event, NULLP);
         ret = RFAILED;
      }
   } /* else eventDir == CZ_DIR_OUT) */
   if(ROK == ret)
   { 
      CZ_UPD_STS(peerCb, msgType, eventDir);
   }
   CZ_RETVALUE(ret);
} /* end of czGpmHdlResetRspInCnted */


/**********************************************************************/
/* Private utilities */
/**********************************************************************/

/**
    @brief The function czGpmStartWaitTmr starts the "Time To Wait" timer.
    @details This function looks for "Time To Wait IE" in the given PDU and
             starts a timer witht that value.
    @param[in] peerCb Peer control block.
    @param[in] msg    The msg is the PDU which has a Time To wait IE.
    @param[in] status Status to fill the staInd, whether timer is for setupFail
                      or cfgupd fail.
*/ 
#ifdef ANSI
PRIVATE Void czGpmStartWaitTmr
(
CzPeerCb    *peerCb,
CztX2AP_PDU *msg,
U8          status
)
#else /* Else if ANSI not defined */
PRIVATE Void czGpmStartWaitTmr (peerCb, msg, status)
CzPeerCb    *peerCb;
CztX2AP_PDU *msg;
U8          status;
#endif /* End of else */
{
   CztTimeToWait  *waitIe = NULLP;
   CztStaInd      tmrStaInd;
   CzCztSapCb     *cztSapCb = NULLP;
   S32            wait = 0;
   S16            ret = ROK;

   CZ_TRC2(czGpmStartWaitTmr);
   CZ_PRNT_PEERID;
   
   /* Start wait timer */
   czUtlGetIE( msg, Cztid_TimeToWait, (TknU8 **)&waitIe);
   if(waitIe != NULLP)
   {
      if(waitIe->pres == PRSNT_NODEF)
      {
         switch(waitIe->val)
         {
            case CztTimeToWaitv1sEnum:
               wait = 1;
               break;
            case CztTimeToWaitv2sEnum:
               wait = 2;
               break;
            case CztTimeToWaitv5sEnum:
               wait = 5;
               break;
            case CztTimeToWaitv10sEnum:
               wait = 10;
               break;
            case CztTimeToWaitv20sEnum:
               wait = 20;
               break;
            case CztTimeToWaitv60sEnum:
               wait = 60;
               break;
            default:
               break;
         } /* switch */
         /* cz001.301: converting wait time to SSI ticks. */
         if(wait == 0)
            wait = 1;

         /* Convert seconds to milliseconds*/
         CZ_CONV_SEC_TO_MSEC(wait);

         ret = czStartTmr(CZ_TMR_TIME_TO_WAIT, wait, peerCb);
#if (ERRCLASS & ERRCLS_INT_PAR)
         if(ret != ROK)
         {
            CZ_DBG_FATAL((CZ_PRNT_BUF, 
                     "Could not start the Time to Wait timer.\n"));
            CZ_SEND_LCL_GEN_ERR(peerCb, CZT_ERRTYPE_SYS,
                  CZT_ERRCAUSE_TMR_ERR, CZT_MSG_X2_SETUP_RSP); 
         }
         else
#endif /* End of #if (ERRCLASS & ERRCLS_INT_PAR) */
         {
            CZ_DBG_INFO((CZ_PRNT_BUF, 
               "Started the Time to Wait timer with value %ld.\n",  wait));
            peerCb->gpmCb.wait = TRUE;
            cztSapCb = peerCb->cztSapCb;
            CZ_SET_ZERO(&tmrStaInd, sizeof(CztStaInd)); 
            tmrStaInd.peerId = peerCb->peerId;
            tmrStaInd.type = CZT_STA_TYPE_TMR_START;
            tmrStaInd.status = status;
            tmrStaInd.reason = CZT_REASON_NOT_APPL;

            ret = czUimSndStaInd(cztSapCb, &tmrStaInd);
            if(ROK != ret)
            {
               CZ_DBG_ERROR((CZ_PRNT_BUF, "Delivering Status Indication with "\
                             "timeToWait expiry failed" \
                             ));
            }
         } /* else */
      } /* if(waitIe->pres == PRSNT_NODEF) */
   }/* if(waitIe != NULLP) */
   else
   {
      CZ_DBG_INFO((CZ_PRNT_BUF, "Time to Wait IE was not found.\n"));
   }
} /* end of czGpmStartWaitTmr */


/**
    @brief The function czGpmErrRspToPeer sends the error resposne to peer
    @details This function builds the response or error indication message
    based on the message type and sends it to the peer.
    @code
           Erroneous Incoming Message |     Error Response to peer
           --------------------------------------------------------------
           Setup Request              | Setup Failure
           Setup Ack                  | X2AP Error Indication message
           Setup Failure              | X2AP Error Indication message
           Reset Request              | X2AP Error Indication message
           Reset Response             | X2AP Error Indication message
           eNB Config Update Request  | eNB Config Update Failure
           eNB Config Update Response | X2AP Error Indication message
           eNB Config Update Failure  | X2AP Error Indication message
           Other X2AP Gloabal message | X2AP Error Indication message
           
    @endcode

    @param[in] peerCb  Peer control block
    @param[in] event   The event is the X2AP message received from peer.
    @param[in] msgType Message type for which the error response has to be generated.
*/ 
#ifdef ANSI
PRIVATE S16 czGpmErrRspToPeer 
(
CzPeerCb *peerCb,
CztEvnt  *event,
U8       msgType
)
#else /* Else if ANSI not defined */
PRIVATE S16 czGpmErrRspToPeer (peerCb, event, msgType)
CzPeerCb *peerCb;
CztEvnt  *event;
U8        msgType;
#endif /* End of else */
{
   S16      ret = ROK;
   CztEvnt* dstEvnt = NULLP;
   U8       rspMsgType = 0;
   TknU8    *ieTkn1, *ieTkn2;
   U8        cnt = 0;
   CzIeInfo   ieId[CZ_MAX_IDX];

   CZ_TRC2(czGpmErrRspToPeer);
   CZ_PRNT_PEERID;

   if(msgType == CZT_MSG_X2_SETUP)
   {
      rspMsgType = CZT_MSG_X2_SETUP_FAIL;
      dstEvnt = czUtlGenX2SetupFailure (NULLP, 
         /* cz001.301: Fix for the build warnings */
#ifdef CZTV1
               CZT_CAUSE_MISC,
#else
               CAUSE_MISC,
#endif
            CztCauseMiscom_interventionEnum);
   }
   else if(msgType == CZT_MSG_CFG_UPD_REQ)
   {
      rspMsgType = CZT_MSG_CFG_UPD_FAIL;
      dstEvnt =  czUtlGenX2CfgUpdFailure(&(event->pdu), 
         /* cz001.301: Fix for the build warnings */
#ifdef CZTV1
                  CZT_CAUSE_PROTOCOL,
#else
                  CAUSE_PROTOCOL,
#endif
            CztCauseProtmessage_not_compatible_with_receiver_stateEnum);
   }
   else if(msgType == CZT_MSG_MOB_CHANGE_REQ)
   {
      rspMsgType = CZT_MSG_MOB_CHANGE_FAIL;

      czUtlGetIE(&(event->pdu), Cztid_ENB1_Cell_ID, &ieTkn1);
      czUtlGetIE(&(event->pdu), Cztid_ENB2_Cell_ID, &ieTkn2);
      /*cz001.301: x2ap kwork fixes*/
      if((ieTkn1 != NULLP) && (ieTkn1->pres != PRSNT_NODEF))
      {
         CZ_FILL_IE_ID(ieId[cnt], Cztid_ENB1_Cell_ID,
                      CztCriticalityrejectEnum, CztTypOfErrnot_understoodEnum);
         cnt++;
      }
      /*cz001.301: x2ap kwork fixes*/
      if((ieTkn2 != NULLP) && (ieTkn2->pres != PRSNT_NODEF)) 
      {
          CZ_FILL_IE_ID(ieId[cnt], Cztid_ENB2_Cell_ID,
                       CztCriticalityrejectEnum, CztTypOfErrnot_understoodEnum);
          cnt++;
      }

      /*cz001.301: x2ap kwork fix*/
      if((ieTkn1 != NULLP) && (ieTkn2 != NULLP))
      {
         dstEvnt = czUtlGenMobChgFailure(NULLP,(CztECGI *)ieTkn1 , 
                                         (CztECGI *)ieTkn2, 
         /* cz001.301: Fix for the build warnings */
#ifdef CZTV1
                                          CZT_CAUSE_MISC,
#else
                                          CAUSE_MISC,
#endif
                                          CztCauseMiscunspecifiedEnum);
      }
   } /* else if(msgType == CZT_MSG_MOB_CHANGE_REQ) */
   else if(msgType == CZT_MSG_CELL_ACTIVATION_REQ)
   {
      rspMsgType = CZT_MSG_CELL_ACTIVATION_FAIL;
      dstEvnt =  czUtlGenCellActFailure(NULLP, 
         /* cz001.301: Fix for the build warnings */
#ifdef CZTV1
            CZT_CAUSE_MISC,
#else
            CAUSE_MISC,
#endif
                          CztCauseMiscunspecifiedEnum);
   }
   else
   {
      rspMsgType = CZT_MSG_ERR_IND;
      dstEvnt = czUtlGenPeerErrInd (&(event->pdu), msgType, NULLP,
         /* cz001.301: Fix for the build warnings */
#ifdef CZTV1
            CZT_CAUSE_PROTOCOL,
#else
            CAUSE_PROTOCOL,
#endif
            CztCauseProtmessage_not_compatible_with_receiver_stateEnum);
   }
   if(dstEvnt != NULLP)
   {
      czGpmSendMsg(peerCb, dstEvnt, rspMsgType, FALSE);
   }
   CZ_FREEEVNT(event);
   CZ_RETVALUE(ret);
}/* end of czGpmErrRspToPeer */


/**
     @brief  The function czGpmHdlMobChangeReq is called when Mobility Change Request
             is being sent or received.

     @details  It is valid only in  CZ_GP_CONNECTED state.

     @param[in] peerCb   The peerCb is the control block to the peer on whose
                         X2-interface the message is exchanged.
     @param[in] msgType  The msg represents the X2 Message that is exchanged.
     @param[in] event    The event that has come from peer or user.
     @param[in] eventDir Event direction from user or peer.

     @return 
          - ROK     - If the procedure was successful.
          - ROUTRES - If the procedure was unsuccessful due to resource crunch.
          - RFAILED - If the procedure was unsuccessful due to any failure.
*/

#ifdef ANSI
PUBLIC S16 czGpmHdlMobChangeReq
(
CzPeerCb  *peerCb,
U8         msgType,
CztEvnt   *event,
U8         eventDir
)
#else /* Else if ANSI not defined */
PUBLIC S16 czGpmHdlMobChangeReq(peerCb, msgType, event, eventDir)
CzPeerCb  *peerCb;
U8         msgType;
CztEvnt   *event;
U8         eventDir;
#endif /* End of else */
{
   S16 ret;

   CZ_TRC2(czGpmHdlMobChangeReq);
   CZ_PRNT_PEERID;

   ret = ROK;

   if(eventDir == CZ_DIR_IN)
   {
      /* Its an INCOMING MOBILITY CHANGE Request
       * indicate it to the user */

      CZ_DBG_INFO((CZ_PRNT_BUF, "INCOMING MOBILITY CHANGE REQUEST\n"));

      ret = czUimSndGpmInd(peerCb, event);
      if(ret != ROK)
      {
         ret  = RFAILED;

         CZ_DBG_FATAL((CZ_PRNT_BUF, "Delivering the incoming \
                  Mobility Change Request Request Message failed in \
                  state CZ_GP_CONNECTED. Sending a Mobility Change \
                  Failure Response to the peer."));

         czGpmErrRspToPeer(peerCb, event, msgType);
      } /* if(ret != ROK) */
   } /* if(eventDir == CZ_DIR_IN) */
   else
   {
      /* Its an outgoing Mobility Change Request towards peer
         * Send it to the transport */

      CZ_DBG_INFO((CZ_PRNT_BUF,"Outgoing MOBILITY CHANGE REQUEST \
                                from user.\n"));

      ret = czTpmSndToLower(peerCb, &(event->pdu),
                            peerCb->peerCfg.globalStreamId);

      if(ret != ROK)
      {
         CZ_DBG_FATAL((CZ_PRNT_BUF,"The message %d failed to be sent.\n",
                       msgType));

         CZ_SEND_LCL_ERR_ALL(peerCb, CZT_ERRTYPE_MSG_SEND_FAILED, ret,
                                msgType, event, NULLP);
          ret = RFAILED;
      } /* if(ret != ROK) */
      else
      {
         CZ_FREEEVNT(event);
      }
   } /* else ..(eventDir == CZ_DIR_OUT) */
  if(ROK == ret)
   {
      CZ_UPD_STS(peerCb, msgType, eventDir);
   }

   CZ_RETVALUE (ret);
}/* End of czGpmHdlMobChangeReq */

/**

     @brief  The function czGpmHdlMobChangeAck is called when Mobility Change Acknowledgement
             is being sent or received.

     @details  it is valid only in  CZ_GP_CONNECTED state.

     @param[in] peerCb   The peerCb is the control block to the peer on whose
                         X2-interface the message is exchanged.
     @param[in] msgType  The msg represents the X2 Message that is exchanged.
     @param[in] event    The event that has come from peer or user.
     @param[in] eventDir Event direction from user or peer.

     @return
         - ROK     - If the procedure was successful.
         - ROUTRES - If the procedure was unsuccessful due to resource crunch.
         - RFAILED - If the procedure was unsuccessful due to any failure.
*/
#ifdef ANSI
PUBLIC S16 czGpmHdlMobChangeAck
(
CzPeerCb  *peerCb,
U8         msgType,
CztEvnt   *event,
U8         eventDir
)
#else /* Else if ANSI not defined */
PUBLIC S16 czGpmHdlMobChangeAck(peerCb, msgType, event, eventDir)
CzPeerCb  *peerCb;
U8         msgType;
CztEvnt   *event;
U8         eventDir;
#endif /* End of else */
{
   S16  ret = ROK;

   CZ_TRC2(czGpmHdlMobChangeAck);
   CZ_PRNT_PEERID;

   if (eventDir == CZ_DIR_IN)
   {
      /* Its an incoming Mobility settings change Ack */

      /* Indicate to the user */
      ret = czUimSndGpmCfm(peerCb, event);

      if(ret != ROK)
      {
         ret = RFAILED;
         CZ_DBG_FATAL((CZ_PRNT_BUF, "Indicating the incoming \
                   Mobility Change Acknowledge Message failed \
                   in CZ_GP_CONNECTED state. \
                   Sending a X2-Error Indication to the peer. \n"));
         /* Falied to inform user. Now build Error Indication message to Peer */
         czGpmErrRspToPeer(peerCb, event, msgType);
       } /* if(ret != ROK) */
    }/* if (eventDir == CZ_DIR_IN) */
    else
    {
      /* Its an outgoing Mobility Setting Change  Ack */
      /* Send it out to the transport */

      ret = czTpmSndToLower(peerCb, &(event->pdu),
                            peerCb->peerCfg.globalStreamId);

      if(ret != ROK)
      {
         CZ_DBG_FATAL((CZ_PRNT_BUF,"The message %d failed to be sent.\n",
                       msgType));

         CZ_SEND_LCL_ERR_ALL(peerCb, CZT_ERRTYPE_MSG_SEND_FAILED,
                             ret, msgType, event, NULLP);

      } /* if(ret != ROK) */
      else
      {
         CZ_FREEEVNT(event);
      }
   } /* else ..(dir == CZ_DIR_OUT) */
   if(ROK == ret)
   {
      CZ_UPD_STS(peerCb, msgType, eventDir);
   }

   CZ_RETVALUE(ret);
} /* End of czGpmHdlMobChangeAck */



/**
     @brief  The function czGpmHdlMobChangeFail is called when Mobility Change Failure
            is being sent or received. 

     @details  It is valid only in  CZ_GP_CONNECTED state.

     @param[in] peerCb   The peerCb is the control block to the peer on whose
                         X2-interface the message is exchanged.
     @param[in] msgType  The msg represents the X2 Message that is exchanged.
     @param[in] event    The event that has come from peer or user.
     @param[in] eventDir Event direction from user or peer.

     @return  
              Following are the possible return values.
               - ROK     - If the procedure was successful.
               - ROUTRES - If the procedure was unsuccessful due to resource crunch.
               - RFAILED - If the procedure was unsuccessful due to any failure.
*/
#ifdef ANSI
PUBLIC S16 czGpmHdlMobChangeFail
(
CzPeerCb  *peerCb,
U8         msgType,
CztEvnt   *event,
U8         eventDir
)
#else /* Else if ANSI not defined */
PUBLIC S16 czGpmHdlMobChangeFail(peerCb, msgType, event, eventDir)
CzPeerCb  *peerCb;
U8         msgType;
CztEvnt   *event;
U8         eventDir;
#endif /* End of else */
{
   S16  ret = ROK;

   CZ_TRC2(czGpmHdlMobChangeFail);
   CZ_PRNT_PEERID;

   /* Check for the direction */
   if (eventDir == CZ_DIR_IN)
   {
      /* Its an incoming Mobility Change Fail */
      /* Indicate to the user */
      ret = czUimSndGpmCfm(peerCb, event);
      if(ret != ROK)
      {
         ret = RFAILED;

         CZ_DBG_FATAL((CZ_PRNT_BUF, "Indicating the incoming \
                   Mobility Change Failure Response Message failed \
                   in CZ_GP_CONNECTED state. \
                   Sending a X2-Error Indication to the peer. \n"));
         /* Falied to inform user. Now build Error Indication message to Peer */
         czGpmErrRspToPeer(peerCb, event, msgType);
       
      } /* if(ret != ROK) */
   }/* if (eventDir == CZ_DIR_IN) */
   else
   {
      /* Its an outgoing Mobility Change Fail */
      /* Send it out to the transport */

       ret = czTpmSndToLower(peerCb, &(event->pdu),
                             peerCb->peerCfg.globalStreamId);

      if(ret != ROK)
      {
         CZ_DBG_FATAL((CZ_PRNT_BUF,"The message %d failed to be sent.\n",
                       msgType));

         CZ_SEND_LCL_ERR_ALL(peerCb, CZT_ERRTYPE_MSG_SEND_FAILED,
                             ret, msgType, event, NULLP);
      } /* if(ret != ROK) */
      else
      {
         CZ_FREEEVNT(event);
      }
   } /* else ..(dir == CZ_DIR_OUT) */

   if(ROK == ret)
   {
      CZ_UPD_STS(peerCb, msgType, eventDir);
   }
   
   CZ_RETVALUE(ret);
}

/**
     @brief  The function czGpmHdlRLFIndication handles RLF Indication in  CZ_GP_CONNECTED state.

     @details
          - If the message direction is IN, the message is sent to user.
          - If the message direction is OUT, then the message is sent out.


     @param[in] peerCb   The peerCb is the control block to the peer on whose
                         X2-interface the message is exchanged.
     @param[in] msgType  The msg represents the X2 Message that is exchanged.
     @param[in] event    The event that has come from peer or user.
     @param[in] eventDir Event direction from user or peer.

     @return 
         - ROK     - If the procedure was successful.
         - ROUTRES - If the procedure was unsuccessful due to resource crunch.
         - RFAILED - If the procedure was unsuccessful due to any failure.
*/
#ifdef ANSI
PUBLIC S16 czGpmHdlRLFIndication
(
CzPeerCb  *peerCb,
U8         msgType,
CztEvnt   *event,
U8         eventDir
)
#else /* Else if ANSI not defined */
PUBLIC S16 czGpmHdlRLFIndication(peerCb, msgType, event, eventDir)
CzPeerCb  *peerCb;
U8         msgType;
CztEvnt   *event;
U8         eventDir;
#endif /* End of else */
{
   S16 ret;

   CZ_TRC2(czGpmHdlRLFIndication);
   CZ_PRNT_PEERID;

   ret = ROK;

    if(eventDir == CZ_DIR_IN)
   {
      /* Its an INCOMING RLF INDICATION
       * indicate it to the user */

      CZ_DBG_INFO((CZ_PRNT_BUF,
                   "Incoming RLF INDICATION\n"));

      ret = czUimSndGpmInd(peerCb, event);
      if(ret != ROK)
      {
         ret = RFAILED;

         CZ_DBG_FATAL((CZ_PRNT_BUF, "Delivering the incoming "\
                  "RLF Indication Message failed in \
                  state CZ_GP_CONNECTED.")); 

         czGpmErrRspToPeer(peerCb, event, msgType);
      } /* if(ret != ROK) */
   } /* if(eventDir == CZ_DIR_IN) */
   else
   {
      /* Its an outgoing RLF Indication towards peer
       * Send it to the transport */

      CZ_DBG_INFO((CZ_PRNT_BUF,
                   "Outgoing RLF INDICATION from user.\n"));
     
      ret = czTpmSndToLower(peerCb, &(event->pdu),
             peerCb->peerCfg.globalStreamId);

      if(ret != ROK)
      {
          CZ_DBG_FATAL((CZ_PRNT_BUF,
                       "The message %d failed to be sent.\n", msgType));

          CZ_SEND_LCL_ERR_ALL(peerCb, CZT_ERRTYPE_MSG_SEND_FAILED, ret,
                              msgType, event, NULLP);
          ret = RFAILED;
      } /* if(ret != ROK) */
      else
      {
          CZ_FREEEVNT(event);
      }
   } /* else ..(eventDir == CZ_DIR_OUT) */
  if(ROK == ret)
   {
      CZ_UPD_STS(peerCb, msgType, eventDir);
   }

    CZ_RETVALUE (ret);
}/* End of czGpmHdlRLFIndication */


/**

     @brief  The function czGpmHdlHoReport handles HO Report in CZ_GP_CONNECTED state.

    @details
         - If the message direction is IN, the message is sent to user.
         - If the message direction is OUT, then the message is sent out.

     @param[in] peerCb   The peerCb is the control block to the peer on whose
                         X2-interface the message is exchanged.
     @param[in] msgType  The msg represents the X2 Message that is exchanged.
     @param[in] event    The event that has come from peer or user.
     @param[in] eventDir Event direction from user or peer.

     @return  
         - ROK     - If the procedure was successful.
         - ROUTRES - If the procedure was unsuccessful due to resource crunch.
         - RFAILED - If the procedure was unsuccessful due to any failure.
*/
#ifdef ANSI
PUBLIC S16 czGpmHdlHoReport
(
CzPeerCb  *peerCb,
U8         msgType,
CztEvnt   *event,
U8         eventDir
)
#else /* Else if ANSI not defined */
PUBLIC S16 czGpmHdlHoReport(peerCb, msgType, event, eventDir)
CzPeerCb  *peerCb;
U8         msgType;
CztEvnt   *event;
U8         eventDir;
#endif /* End of else */
{
   S16 ret;

   CZ_TRC2(czGpmHdlHoReport);
   CZ_PRNT_PEERID;

   ret = ROK;

    if(eventDir == CZ_DIR_IN)
   {
      /* Its an INCOMING HANDOVER REPORT message
       * indicate it to the user */

      CZ_DBG_INFO((CZ_PRNT_BUF,
                   "Incoming HANDOVER REPORT message\n"));

      ret = czUimSndGpmInd(peerCb, event);
      if(ret != ROK)
      {
         ret = RFAILED;

         CZ_DBG_FATAL((CZ_PRNT_BUF, "Delivering the incoming "\
                  "Handover Report Message failed in \
                  state CZ_GP_CONNECTED.")); 

         czGpmErrRspToPeer(peerCb, event, msgType);
      } /* if(ret != ROK) */
   } /* if(eventDir == CZ_DIR_IN) */
   else
   {
      /* Its an outgoing Mobility Change Request towards peer
       * Send it to the transport */

      CZ_DBG_INFO((CZ_PRNT_BUF,
                   "Outgoing HANDOVER REPORT from user.\n"));

      ret = czTpmSndToLower(peerCb, &(event->pdu),
                             peerCb->peerCfg.globalStreamId);

      if(ret != ROK)
      {
         CZ_DBG_FATAL((CZ_PRNT_BUF,
                       "The message %d failed to be sent.\n", msgType));

         CZ_SEND_LCL_ERR_ALL(peerCb, CZT_ERRTYPE_MSG_SEND_FAILED, ret,
                             msgType, event, NULLP);
         ret = RFAILED;
      } /* if(ret != ROK) */
      else
      {
         CZ_FREEEVNT(event);
      }
   } /* else ..(eventDir == CZ_DIR_OUT) */
   if(ROK == ret)
   {
      CZ_UPD_STS(peerCb, msgType, eventDir);
   }

   CZ_RETVALUE (ret);
}/* End of czGpmHdlHoReport */


/**
     @brief  The function czGpmHdlCellActivationReq is called when Cell Activation Request
            is being sent or received.

     @details  It is valid only in  CZ_GP_CONNECTED state.

     @param[in] peerCb   The peerCb is the control block to the peer on whose
                         X2-interface the message is exchanged.
     @param[in] msgType  The msg represents the X2 Message that is exchanged.
     @param[in] event    The event that has come from peer or user.
     @param[in] eventDir Event direction from user or peer.

     @return  
         - ROK     - If the procedure was successful.
         - ROUTRES - If the procedure was unsuccessful due to resource crunch.
         - RFAILED - If the procedure was unsuccessful due to any failure.
*/
#ifdef ANSI
PUBLIC S16 czGpmHdlCellActivationReq
(
CzPeerCb  *peerCb,
U8         msgType,
CztEvnt   *event,
U8         eventDir
)
#else /* Else if ANSI not defined */
PUBLIC S16 czGpmHdlCellActivationReq(peerCb, msgType, event, eventDir)
CzPeerCb  *peerCb;
U8         msgType;
CztEvnt   *event;
U8         eventDir;
#endif /* End of else */
{
   S16 ret;

   CZ_TRC2(czGpmHdlCellActivationReq);
   CZ_PRNT_PEERID;

   ret = ROK;

   if(eventDir == CZ_DIR_IN)
   {
      /* Its an INCOMING CELL ACTIVATION Request
       * indicate it to the user */

      CZ_DBG_INFO((CZ_PRNT_BUF,
               "Incoming CELL ACTIVATION REQUEST\n"));

      ret = czUimSndGpmInd(peerCb, event);
      if(ret != ROK)
      {
         ret = RFAILED;
         CZ_DBG_FATAL((CZ_PRNT_BUF, "Delivering the incoming \
                  Cell Activation Request Message failed in \
                  state CZ_GP_CONNECTED. Sending a Cell Activation \
                  Failure Response to the peer."));

         czGpmErrRspToPeer(peerCb, event, msgType);
      } /* if(ret != ROK) */
   } /* if(eventDir == CZ_DIR_IN) */
   else
   {
      /* Its an outgoing Cell Activation Request towards peer
       * Send it to the transport */

       CZ_DBG_INFO((CZ_PRNT_BUF,
                  "Outgoing CELL ACTIVATION REQUEST from user.\n"));
            
      ret = czTpmSndToLower(peerCb, &(event->pdu),
                            peerCb->peerCfg.globalStreamId);
       
      if(ret != ROK)
      {
         CZ_DBG_FATAL((CZ_PRNT_BUF,
                  "The message %d failed to be sent.\n", msgType));

         CZ_SEND_LCL_ERR_ALL(peerCb, CZT_ERRTYPE_MSG_SEND_FAILED, ret,
                             msgType, event, NULLP);
         ret = RFAILED;
      } /* if(ret != ROK) */
      else
      {
         CZ_FREEEVNT(event);
      }
   } /* else ..(eventDir == CZ_DIR_OUT) */
   if(ROK == ret)
   {
      CZ_UPD_STS(peerCb, msgType, eventDir);
   }

   CZ_RETVALUE (ret);
}/* End of czGpmHdlCellActivationReq */

/**

     @brief  The function czGpmHdlCellActivationRes is called when Cell Activation Response
            is being sent or received.

     @details  It is valid only in  CZ_GP_CONNECTED state.

     @param[in] peerCb   The peerCb is the control block to the peer on whose
                         X2-interface the message is exchanged.
     @param[in] msgType  The msg represents the X2 Message that is exchanged.
     @param[in] event    The event that has come from peer or user.
     @param[in] eventDir Event direction from user or peer.

     @return 
         - ROK     - If the procedure was successful.
         - ROUTRES - If the procedure was unsuccessful due to resource crunch.
         - RFAILED - If the procedure was unsuccessful due to any failure.
*/
#ifdef ANSI
PUBLIC S16 czGpmHdlCellActivationRes
(
CzPeerCb  *peerCb,
U8         msgType,
CztEvnt   *event,
U8         eventDir
)
#else /* Else if ANSI not defined */
PUBLIC S16 czGpmHdlCellActivationRes(peerCb, msgType, event, eventDir)
CzPeerCb  *peerCb;
U8         msgType;
CztEvnt   *event;
U8         eventDir;
#endif /* end of else */
{
   S16  ret = ROK;

   CZ_TRC2(czGpmHdlCellActivationRes);
   CZ_PRNT_PEERID;

   if (eventDir == CZ_DIR_IN)
   {
      /* Its an incoming Cell Activation Ack */
      /* Indicate to the user */
      ret = czUimSndGpmCfm(peerCb, event);
      if(ret != ROK)
      {
         ret = RFAILED;

         CZ_DBG_FATAL((CZ_PRNT_BUF, "Indicating the incoming \
                   Cell Activation Response Message failed \
                   in CZ_GP_CONNECTED state. \
                   Sending a X2-Error Indication to the peer. \n"
                   ));
         /* Falied to inform user. Now build Error Indication message to Peer */
         czGpmErrRspToPeer(peerCb, event, msgType);

      } /* if(ret != ROK) */
   }/* if (eventDir == CZ_DIR_IN) */
   else
   {
      /* Its an outgoing Cell Activation Response */
      /* Send it out to the transport */

      ret = czTpmSndToLower(peerCb, &(event->pdu),
            peerCb->peerCfg.globalStreamId);
      if(ret != ROK)
      {
         CZ_DBG_FATAL((CZ_PRNT_BUF,
                  "The message %d failed to be sent.\n", msgType));

         CZ_SEND_LCL_ERR_ALL(peerCb, CZT_ERRTYPE_MSG_SEND_FAILED,
                             ret, msgType, event, NULLP);
      } /* if(ret != ROK) */
      else
      {
         CZ_FREEEVNT(event);
      }
   } /* else ..(dir == CZ_DIR_OUT) */
   if(ROK == ret)
   {
      CZ_UPD_STS(peerCb, msgType, eventDir);
   }

   CZ_RETVALUE(ret);
}/* End of czGpmHdlCellActivationRes */

/**

     @brief  The function czGpmHdlCellActivationFail is called when Cell Activation Failure
            is being sent or received.

     @details It is valid only in  CZ_GP_CONNECTED state.

     @param[in] peerCb   The peerCb is the control block to the peer on whose
                         X2-interface the message is exchanged.
     @param[in] msgType  The msg represents the X2 Message that is exchanged.
     @param[in] event    The event that has come from peer or user.
     @param[in] eventDir Event direction from user or peer.

     @return 
         - ROK     - If the procedure was successful.
         - ROUTRES - If the procedure was unsuccessful due to resource crunch.
         - RFAILED - If the procedure was unsuccessful due to any failure.
*/
#ifdef ANSI
PUBLIC S16 czGpmHdlCellActivationFail
(
CzPeerCb  *peerCb,
U8         msgType,
CztEvnt   *event,
U8         eventDir
)
#else /* Else if ANSI not defined */
PUBLIC S16 czGpmHdlCellActivationFail(peerCb, msgType, event, eventDir)
CzPeerCb  *peerCb;
U8         msgType;
CztEvnt   *event;
U8         eventDir;
#endif /* End of else */
{
   S16  ret = ROK;

   CZ_TRC2(czGpmHdlCellActivationFail);
   CZ_PRNT_PEERID;

   /* Check for the direction */
   if (eventDir == CZ_DIR_IN)
   {
      /* Indicate to the user */
       ret = czUimSndGpmCfm(peerCb, event);
       if(ret != ROK)
       {
          ret = RFAILED;

          CZ_DBG_FATAL((CZ_PRNT_BUF, "Indicating the incoming \
                     Cell Activation Failure Message failed. \
                     Sending a X2-Error \
                     Indication to the peer."\
                     ));
          /* Falied to inform user. Now build Error Indication message to Peer */
          czGpmErrRspToPeer(peerCb, event, msgType);
       } /* if(ret != ROK) */
   }/* if (eventDir == CZ_DIR_IN) */
   else
   {
       /* Send to transport */
         ret = czTpmSndToLower(peerCb, &(event->pdu),
               peerCb->peerCfg.globalStreamId);
         if(ret != ROK)
         {
            CZ_DBG_FATAL((CZ_PRNT_BUF,
                  "The message %d failed to be sent.\n", msgType));

            CZ_SEND_LCL_ERR_ALL(peerCb, CZT_ERRTYPE_MSG_SEND_FAILED,
                                ret, msgType, event, NULLP);
         } /* if(ret != ROK) */
          else
         {
            CZ_FREEEVNT(event);
         }
   }/* else ..dir == CZ_DIR_OUT */
   if(ROK == ret)
   {
      CZ_UPD_STS(peerCb, msgType, eventDir);
   }

   CZ_RETVALUE(ret);
}/* End of czGpmHdlCellActivationFail*/
  


                                          
/** @} */

/**********************************************************************
  
         End of file:     tr_gpm.c@@/main/2 - Tue Aug 30 18:36:11 2011
   
**********************************************************************/
 
/**********************************************************************
        Revision History:

**********************************************************************/
/********************************************************************90**
 
     ver       pat    init                  description
------------ -------- ---- ----------------------------------------------
/main/1      ---        mm              1. LTE-X2AP Initial Release.
            cz001.101   mm              1. CR Fixes, Memory Leak fixes.
                                        2. The state CZ_GP_RESET is removed.
                                        The handlers of this state are handled
                                        in state CZ_GP_CONNECTING.
/main/2      ---        pt              1. LTE-X2AP 3.1 release.
/main/3    cz001.301 akaranth           1. Fix for the build warnings. 
                                        2. Klockwork warning fixed.
                                        3. Reverting the states if X2AP receives the error msg
                                           for the sent reset msg.
                                        4. Converting the wait value to the SSI ticks.
                                        5. Changing the peer state to CZ_GP_IDLE,
                                           If eNB receives error msg for reset msg 
                                           To allow app to send X2 setup once again.
*********************************************************************91*/
