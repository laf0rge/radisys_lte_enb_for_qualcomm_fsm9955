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
  
     Desc:     This file represents Trillium X2AP Basic Mobility Procedure
               module.

     File:     cz_bmp.c

     Sid:      tr_bmp.c@@/main/2 - Tue Aug 30 18:36:09 2011

     Prg:      sv 

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
#include "cm_mblk.h"       /* Common memory allocation        */
#include "cm_inet.h"       /* Common socket library           */
#include "cm_pasn.h"       /* Common ASN.1 library           */
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
#include "cm_pasn.x"       /* Common ASN.1 Library           */
#include "sct.x"           /* SCT interface defines           */
#include "czt.x"           /* CZT interface structures        */
#include "lcz.x"           /* X2AP layer management           */
#include "cz.x"            /* X2AP layer structures           */



/* local defines */
PRIVATE CztLclErrInd czBmpErrInd;
PRIVATE CztUeInfo czUeInfo;

/** @defgroup bmpmodule X2AP Basic Mobility Procedure Module 
@{
*/

/** The maximum number of Basic Mobility Procedure related events */
#define CZ_BMP_HO_MAX_EVENTS 6

/* U32 UeCbKey =  eNodeB Type | old X2AP UE ID in current PDU.
keyPrefix = eNodeB Type [U8]
keyValue = old X2AP UE ID in current PDU; [U16]
key = (eNodeB Type | old X2AP UE ID in current PDU)  [U32]
key = (0x00 | <eNodeB Type> | <old X2AP UE ID in current PDU>)  [U32]  

*/

/** This macro gets the actual key from input event, msgdirection, keyValue */
#define CZ_BMP_GET_EVENT_KEY(_keyVal, _hoMsgEvnt, _msgDirectn, _keyPrefix, \
                             _key)  \
                  _keyPrefix = ((CZ_DIR_OUT == _msgDirectn) ?  \
                  (czBmpEvntKeyMap[_hoMsgEvnt][0]): \
                  (czBmpEvntKeyMap[_hoMsgEvnt][1])); \
      _key = PutHiWord(_keyVal, _keyPrefix);


/** This macro gets the keyPrefix[eNodeB Type] from input event, message
    direction */
#define CZ_BMP_GET_EVENT_KEY_PREFIX(_hoMsgEvnt,_msgDirectn,_keyPrefix)\
      _keyPrefix = ((CZ_DIR_OUT == _msgDirectn) ? \
                   (czBmpEvntKeyMap[_hoMsgEvnt][0]): \
                  (czBmpEvntKeyMap[_hoMsgEvnt][1])); \


/** This macro gets the actual key from input keyValue and keyPrefix */
#define CZ_BMP_GET_KEY(_keyVal, _keyPrefix, _key)  \
\
      _key = PutHiWord(_keyVal, _keyPrefix);

/** This macro gets the keyPrefix[eNodeB Type] from input key */
#define CZ_BMP_GET_KEY_PREFIX(_key, _keyPrefix)  \
      _keyPrefix = GetHiWord(_key); \
      _keyPrefix = GetLoByte(_keyPrefix); 

/** This macro gets the keyValue(old UE X2AP ID value) from input key */
#define CZ_BMP_GET_KEY_VAL(_key, keyVal)  \
      _keyVal = GetLoWord(_key, _keyVal); \


/** This macro validates if UE X2AP ID is within the valid range of allowed
    values for it. Range 0 to 4095 */
#define CZ_BMP_IS_VALID_UE_X2AP_ID(_ueId) \
    ((_ueId < CZ_MAX_UE_X2AP_ID)?(TRUE):(FALSE)) 

/** This macro fills the required UE identification information carried on
    X2 interface */
#define CZ_BMP_FILL_UE_INFO(_oldId, _newId) \
{                                               \
   CZ_SET_ZERO((&czUeInfo), sizeof(CztUeInfo)); \
        czUeInfo.oldUEX2APId = _oldId; \
        czUeInfo.newUEX2APId = _newId; \
}

/* cz001.301 : removing the macro CZ_BMP_FILL_GEN_ERR_INFO,
 * as it is redundant */
/** This macro fills the information required for Error Indication 
    to send to UIM */
#define CZ_BMP_FILL_UIM_ERR_INFO(_peerCb, _cType, _cVal, _msgType, _oldId, \
                                 _newId) \
{                                                                 \
   CZ_SET_ZERO((&czBmpErrInd), sizeof(CztLclErrInd));                  \
   czBmpErrInd.peerId = (_peerCb->peerId);                            \
   czBmpErrInd.causeType = _cType;                                  \
   czBmpErrInd.causeValue = _cVal;                                  \
   czBmpErrInd.msgType    = _msgType;                               \
   czBmpErrInd.u.ueInfo.oldUEX2APId = _oldId;                       \
   czBmpErrInd.u.ueInfo.newUEX2APId = _newId;                       \
}

/** This macro fills the general information required for Error Indication 
    to send to UIM */
#define CZ_BMP_FILL_UIM_GEN_ERR_INFO(_peerCb, _cType, _cVal, _msgType) \
{                                               \
   CZ_SET_ZERO((&czBmpErrInd), sizeof(CztLclErrInd));  \
   czBmpErrInd.peerId = (_peerCb->peerId);      \
   czBmpErrInd.causeType = _cType;  \
   czBmpErrInd.causeValue = _cVal;   \
   czBmpErrInd.msgType    = _msgType;  \
}

/** This macro fills error information related to UE */
#define CZ_BMP_FILL_UIM_UE_ERR_INFO(_peerCb, _msgType, _oldId, _newId) \
{                                               \
   CZ_SET_ZERO((&czBmpErrInd), sizeof(CztLclErrInd));  \
   czBmpErrInd.peerId = (_peerCb->peerId);      \
   czBmpErrInd.msgType  = _msgType;                 \
   czBmpErrInd.u.ueInfo.oldUEX2APId = _oldId;                      \
   czBmpErrInd.u.ueInfo.newUEX2APId = _newId;                      \
}

/** This macro fills information related to the message and peer */
#define CZ_BMP_FILL_UIM_MSG_INFO(_peerCb, _msgType ) \
{                                               \
   CZ_SET_ZERO((&czBmpErrInd), sizeof(CztLclErrInd));  \
   czBmpErrInd.peerId = (_peerCb->peerId);      \
   czBmpErrInd.msgType  = _msgType;                 \
}

/** This macro fills error cause information */
#define CZ_BMP_FILL_UIM_CAUSE_INFO(_cType, _cVal ) \
{                                               \
   czBmpErrInd.causeType = _cType;  \
   czBmpErrInd.causeValue = _cVal;   \
}

/** Macro indicating that message originated from user */
#define CZ_BMP_MSG_FRM_USER "User"
/** Macro indicating that message originated from peer */
#define CZ_BMP_MSG_FRM_PEER "Peer"


#ifdef ALIGN_64BIT                                                            

/** Informational Logs in BMP entry function invoked from other modules */
#define CZ_BMP_LOG_ENTRY_INFO(_peerCb, _bmpMsgEvnt, _msgDirectn)              \
{                                                                             \
   CZ_DBG_INFO((CZ_PRNT_BUF, "Handling (%s) message from (%s) through BMP for"\
                "peer(%d)\n", czBmpEvntPrintMap[_bmpMsgEvnt],                  \
   ((_msgDirectn == CZ_DIR_OUT) ? (CZ_BMP_MSG_FRM_USER):(CZ_BMP_MSG_FRM_PEER)) \
               ,(_peerCb->peerId)));                                          \
}

/****** All info / error logging for outgoing event handlers *********/
/** Function entry product log for outgoing message handler */
#define CZ_BMP_LOG_ENTRY_OUT_HDLR_INFO(_peerCb, _oldId, _newId, _bmpMsgEvnt)  \
{                                                                             \
   CZ_DBG_INFO((CZ_PRNT_BUF, "Outgoing BMP event (%s) for an UE(%d:%d) to " \
         "peer(%d)", czBmpEvntPrintMap[_bmpMsgEvnt], _oldId, _newId, \
         (_peerCb->peerId)));                                          \
}

/** Logging outgoing event information */
#define CZ_BMP_LOG_OUT_HDLR_INFO(_peerCb,_oldId,_newId,_bmpMsgEvnt,_streamId) \
{                                                                             \
   CZ_DBG_INFO((CZ_PRNT_BUF, "Sending BMP event (%s) for an UE(%d:%d)"      \
               "to peer(%d) on stream(%d)",                                   \
   czBmpEvntPrintMap[_bmpMsgEvnt], _oldId, _newId,                            \
   (_peerCb->peerId), _streamId));                                            \
}

/** Logging error event for an outgoing message */
#define CZ_BMP_LOG_OUT_HDLR_ERR(_peerCb, _oldId, _newId, _bmpMsgEvnt, _cause) \
{                                                                             \
   CZ_DBG_ERROR((CZ_PRNT_BUF, "Sending BMP event (%s) for an UE(%d:%d)"     \
               "to peer(%d) failed, cause(%d)",                               \
   czBmpEvntPrintMap[_bmpMsgEvnt], _oldId, _newId,                            \
   (_peerCb->peerId), _cause));                                               \
}

/** Function exit product log for outgoing message handler */
#define CZ_BMP_LOG_EXIT_OUT_HDLR_INFO(_peerCb,_oldId,_newId,_bmpMsgEvnt,      \
                                      _streamId)                              \
{                                                                             \
   CZ_DBG_INFO((CZ_PRNT_BUF, "Sent BMP event (%s) for an UE(%d:%d)"           \
               "to peer(%d) on stream(%d)",                                  \
   czBmpEvntPrintMap[_bmpMsgEvnt], _oldId, _newId,                            \
   (_peerCb->peerId), _streamId));                                            \
}
/****End Of all info / error logging for outgoing event handlers*************/

/****All info / error logging for incoming event handlers*************/

/** Function entry product log for incoming message handler */
#define CZ_BMP_LOG_ENTRY_IN_HDLR_INFO(_peerCb, _oldId, _newId, _bmpMsgEvnt)    \
{                                                                              \
   CZ_DBG_INFO((CZ_PRNT_BUF, "Received BMP event (%s) for an UE(%d:%d) from "  \
         "peer(%d)", czBmpEvntPrintMap[_bmpMsgEvnt], _oldId, _newId,          \
         (_peerCb->peerId)));                                                  \
}

/** Logging incoming event information */
#define CZ_BMP_LOG_IN_HDLR_INFO(_peerCb,_oldId,_newId,_bmpMsgEvnt)            \
{                                                                              \
   CZ_DBG_INFO((CZ_PRNT_BUF, "Delivering BMP event (%s) from peer(%d) for an" \
                "UE(%d:%d) to user",                                           \
   czBmpEvntPrintMap[_bmpMsgEvnt], (_peerCb->peerId), _oldId, _newId));        \
}

/** Logging error event for an incoming message */
#define CZ_BMP_LOG_IN_HDLR_ERR(_peerCb, _oldId, _newId, _bmpMsgEvnt, _cause)  \
{                                                                             \
   CZ_DBG_ERROR((CZ_PRNT_BUF, "Delivering BMP event (%s) to user for an "     \
                 "UE(%d:%d) received from peer(%d) failed, cause(%d)",       \
   czBmpEvntPrintMap[_bmpMsgEvnt], _oldId, _newId,                            \
   (_peerCb->peerId), _cause));                                               \
}

/** Function exit product log for incoming message handler */
#define CZ_BMP_LOG_EXIT_IN_HDLR_INFO(_peerCb,_oldId,_newId,_bmpMsgEvnt)        \
{                                                                              \
   CZ_DBG_INFO((CZ_PRNT_BUF, "Delivered BMP event (%s) from peer(%d) for an"  \
                "UE(%d:%d) to user",                                           \
   czBmpEvntPrintMap[_bmpMsgEvnt], (_peerCb->peerId), _oldId, _newId));        \
}

/****End Of all info / error logging for incoming handlers*************/

/** Log for timer track */
#define CZ_BMP_LOG_TMR_EXPIRY_INFO(_ueCb, _tmr)                                \
{                                                                              \
   CZ_DBG_INFO((CZ_PRNT_BUF, "Timer(%d) expired for UE(%d:%d) with peer (%d)",\
   _tmr, (_ueCb->oldUEX2APId),  (_ueCb->newUEX2APId), (ueCb->peerCb->peerId)));\
}

/** Special information to user while generating error indication to peer */
#define CZ_BMP_LOG_ERRINFO_TO_USR(_peerCb, _oldId, _newId, _bmpMsgEvnt,      \
        _causeVal, _lclCauseVal)                                             \
{                                                                            \
   CZ_DBG_INFO((CZ_PRNT_BUF, "Generating Error Indication on (%s) for an"    \
               "UE(%d:%d) to send to peer(%d): Cause(%d), local Cause(%d)", \
               czBmpEvntPrintMap[_bmpMsgEvnt],_oldId, _newId,                \
               (_peerCb->peerId), _causeVal, _lclCauseVal));                 \
}


#else  /* ALIGN_64BIT */                                                                       

/** Informational Logs in BMP entry function invoked from other modules */
#define CZ_BMP_LOG_ENTRY_INFO(_peerCb, _bmpMsgEvnt, _msgDirectn)              \
{                                                                             \
   CZ_DBG_INFO((CZ_PRNT_BUF, "Handling (%s) message from (%s) through BMP for"\
                "peer(%ld)\n", czBmpEvntPrintMap[_bmpMsgEvnt],                 \
   ((_msgDirectn == CZ_DIR_OUT) ? (CZ_BMP_MSG_FRM_USER):(CZ_BMP_MSG_FRM_PEER)) \
               ,(_peerCb->peerId)));                                          \
}

/****All info / error logging for outgoing event handlers*************/

/** Function entry product log for outgoing message handler */
#define CZ_BMP_LOG_ENTRY_OUT_HDLR_INFO(_peerCb, _oldId, _newId, _bmpMsgEvnt)  \
{                                                                             \
   CZ_DBG_INFO((CZ_PRNT_BUF, "Outgoing BMP event (%s) for an UE(%d:%d) to " \
         "peer(%ld)", czBmpEvntPrintMap[_bmpMsgEvnt], _oldId, _newId, \
         (_peerCb->peerId)));                                          \
}

/** Logging outgoing event information */
#define CZ_BMP_LOG_OUT_HDLR_INFO(_peerCb,_oldId,_newId,_bmpMsgEvnt,_streamId) \
{                                                                             \
   CZ_DBG_INFO((CZ_PRNT_BUF, "Sending BMP event (%s) for an UE(%d:%d)"      \
               "to peer(%ld) on stream(%d)",                                   \
   czBmpEvntPrintMap[_bmpMsgEvnt], _oldId, _newId,                            \
   (_peerCb->peerId), _streamId));                                            \
}

/** Logging error event for an outgoing message */
#define CZ_BMP_LOG_OUT_HDLR_ERR(_peerCb, _oldId, _newId, _bmpMsgEvnt, _cause) \
{                                                                             \
   CZ_DBG_ERROR((CZ_PRNT_BUF, "Sending BMP event (%s) for an UE(%d:%d)"     \
               "to peer(%ld) failed, cause(%d)",                               \
   czBmpEvntPrintMap[_bmpMsgEvnt], _oldId, _newId,                            \
   (_peerCb->peerId), _cause));                                               \
}

/** Function exit product log for outgoing message handler */
#define CZ_BMP_LOG_EXIT_OUT_HDLR_INFO(_peerCb, _oldId, _newId, _bmpMsgEvnt,   \
                                      _streamId)                              \
{                                                                             \
   CZ_DBG_INFO((CZ_PRNT_BUF, "Sent BMP event (%s) for an UE(%d:%d)"           \
               "to peer(%ld) on stream(%d)",                                  \
   czBmpEvntPrintMap[_bmpMsgEvnt], _oldId, _newId,                            \
   (_peerCb->peerId), _streamId));                                            \
}

/****End Of all info / error logging for outgoing event handlers*************/

/** Log for timer track */
#define CZ_BMP_LOG_TMR_EXPIRY_INFO(_ueCb, _tmr)                                \
{                                                                              \
   CZ_DBG_INFO((CZ_PRNT_BUF, "Timer(%d) expired for UE(%d:%d) with peer (%ld)",\
   _tmr, (_ueCb->oldUEX2APId),  (_ueCb->newUEX2APId), (ueCb->peerCb->peerId)));\
}

/** Special information to user while generating error indication to peer */

#define CZ_BMP_LOG_ERRINFO_TO_USR(_peerCb, _oldId, _newId, _bmpMsgEvnt,      \
        _causeVal, _lclCauseVal)                                             \
{                                                                            \
   CZ_DBG_INFO((CZ_PRNT_BUF, "Generating Error Indication on (%s) for an"    \
               "UE(%d:%d) to send to peer(%ld): Cause(%d), local Cause(%d)", \
               czBmpEvntPrintMap[_bmpMsgEvnt],_oldId, _newId,                \
               (_peerCb->peerId), _causeVal, _lclCauseVal));                 \
}

/****All info / error logging for incoming event handlers*************/

/** Function entry product log for incoming message handler */
#define CZ_BMP_LOG_ENTRY_IN_HDLR_INFO(_peerCb, _oldId, _newId, _bmpMsgEvnt)    \
{                                                                              \
   CZ_DBG_INFO((CZ_PRNT_BUF, "Received BMP event (%s) for an UE(%d:%d) from "  \
         "peer(%ld)", czBmpEvntPrintMap[_bmpMsgEvnt], _oldId, _newId,          \
         (_peerCb->peerId)));                                                  \
}

/** Logging incoming event information */
#define CZ_BMP_LOG_IN_HDLR_INFO(_peerCb,_oldId,_newId,_bmpMsgEvnt)            \
{                                                                              \
   CZ_DBG_INFO((CZ_PRNT_BUF, "Delivering BMP event (%s) from peer(%ld) for an" \
                "UE(%d:%d) to user",                                           \
   czBmpEvntPrintMap[_bmpMsgEvnt], (_peerCb->peerId), _oldId, _newId));        \
}

/** Logging error event for an incoming message */
#define CZ_BMP_LOG_IN_HDLR_ERR(_peerCb, _oldId, _newId, _bmpMsgEvnt, _cause)  \
{                                                                             \
   CZ_DBG_ERROR((CZ_PRNT_BUF, "Delivering BMP event (%s) to user for an "     \
                 "UE(%d:%d) received from peer(%ld) failed, cause(%d)",       \
   czBmpEvntPrintMap[_bmpMsgEvnt], _oldId, _newId,                            \
   (_peerCb->peerId), _cause));                                               \
}

/** Function exit product log for incoming message handler */
#define CZ_BMP_LOG_EXIT_IN_HDLR_INFO(_peerCb,_oldId,_newId,_bmpMsgEvnt)        \
{                                                                              \
   CZ_DBG_INFO((CZ_PRNT_BUF, "Delivered BMP event (%s) from peer(%ld) for an"  \
                "UE(%d:%d) to user",                                           \
   czBmpEvntPrintMap[_bmpMsgEvnt], (_peerCb->peerId), _oldId, _newId));        \
}
/****End of all info / error logging for incoming handlers*************/

#endif  /* ALIGN_64BIT */


/* Local typedefs */


/* Forward references */

PRIVATE S16 czBmpIsPeerValid ARGS((CzPeerCb   *peerCb,
                                   CztEvnt    *evnt,
                                   U8         hoMsgType,
                                   U8         msgDirectn));

PRIVATE S16 czBmpHdlOutErrInd ARGS((CzPeerCb  *peerCb,
                                   CzUeCb    *ueCb,
                                   CztEvnt   *evnt,                    
                                   U8        hoMsgType,
                                   U16       oldUEX2APIdVal));

PRIVATE S16 czBmpHdlInErrInd ARGS((CzPeerCb  *peerCb,
                                   CzUeCb    *ueCb,
                                   CztEvnt   *evnt,                    
                                   U8        hoMsgType,
                                   U16       oldUEX2APIdVal));

/** syntax of the Basic Mobility Procedure handlers */
typedef S16 (*CZPF) ARGS((CzPeerCb  *peerCb, 
                          CzUeCb    *ueCb, 
                          CztEvnt   *evnt,
                          U8       hoMsgType,
                          U16       oldUEX2APIdVal));


PRIVATE S16 czBmpHdlOutHoReq ARGS((CzPeerCb   *peerCb,
                                   CzUeCb     *ueCb,            
                                   CztEvnt    *evnt,                 
                                   U8         hoMsgType,
                                   U16        oldUEX2APIdVal));


PRIVATE S16 czBmpHdlInHoReq ARGS((CzPeerCb   *peerCb,
                                  CzUeCb     *ueCb,            
                                  CztEvnt    *evnt,                 
                                  U8         hoMsgType,
                                  U16        oldUEX2APIdVal));

PRIVATE S16 czBmpHdlOutHoAck ARGS((CzPeerCb   *peerCb,
                                   CzUeCb     *ueCb,            
                                   CztEvnt    *evnt,                 
                                   U8         hoMsgType,
                                   U16        oldUEX2APIdVal));

PRIVATE S16 czBmpHdlInHoAck ARGS((CzPeerCb   *peerCb,
                                  CzUeCb     *ueCb,            
                                  CztEvnt    *evnt,                 
                                  U8         hoMsgType,
                                  U16        oldUEX2APIdVal));

PRIVATE S16 czBmpHdlOutHoCancel ARGS((CzPeerCb   *peerCb,
                                      CzUeCb     *ueCb,
                                      CztEvnt    *evnt,
                                      U8         hoMsgType,
                                      U16        oldUEX2APIdVal));

PRIVATE S16 czBmpHdlInHoCancel ARGS((CzPeerCb   *peerCb,
                                     CzUeCb     *ueCb,           
                                     CztEvnt    *evnt,                 
                                     U8         hoMsgType,
                                     U16        oldUEX2APIdVal));

PRIVATE S16 czBmpHdlOutHoFail ARGS((CzPeerCb   *peerCb,
                                    CzUeCb     *ueCb,           
                                    CztEvnt    *evnt,                 
                                    U8         hoMsgType,
                                    U16        oldUEX2APIdVal));

PRIVATE S16 czBmpHdlInHoFail ARGS((CzPeerCb   *peerCb,
                                   CzUeCb     *ueCb,           
                                   CztEvnt    *evnt,                 
                                   U8         hoMsgType,
                                   U16        oldUEX2APIdVal));

PRIVATE S16 czBmpHdlOutSnStTrnsfr ARGS((CzPeerCb   *peerCb,
                                        CzUeCb     *ueCb,
                                        CztEvnt    *evnt,                 
                                        U8        hoMsgType,
                                        U16        oldUEX2APIdVal));

PRIVATE S16 czBmpHdlInSnStTrnsfr ARGS((CzPeerCb   *peerCb,
                                       CzUeCb     *ueCb,           
                                       CztEvnt    *evnt,                 
                                       U8         hoMsgType,
                                       U16        oldUEX2APIdVal));

PRIVATE S16 czBmpHdlOutUeCnxtRelease ARGS((CzPeerCb   *peerCb,
                                           CzUeCb     *ueCb,
                                           CztEvnt    *evnt,                 
                                           U8         hoMsgType,
                                           U16        oldUEX2APIdVal));

PRIVATE S16 czBmpHdlInUeCnxtRelease ARGS((CzPeerCb   *peerCb,
                                          CzUeCb     *ueCb,           
                                          CztEvnt    *evnt,                 
                                          U8         hoMsgType,
                                          U16        oldUEX2APIdVal));

PRIVATE S16 czBmpHdlOutEvntErr ARGS((CzPeerCb   *peerCb,
                                      CzUeCb     *ueCb,        
                                      CztEvnt    *evnt,                 
                                      U8         hoMsgType,
                                      U16        oldUEX2APIdVal));

PRIVATE S16 czBmpHdlInHoReqErr ARGS((CzPeerCb   *peerCb,
                                     CzUeCb     *ueCb,           
                                     CztEvnt    *evnt,                 
                                     U8         hoMsgType,
                                     U16        oldUEX2APIdVal));

PRIVATE S16 czBmpHdlInEvntErr ARGS((CzPeerCb   *peerCb,
                                     CzUeCb     *ueCb,           
                                     CztEvnt    *evnt,                 
                                     U8         hoMsgType,
                                     U16        oldUEX2APIdVal));

PRIVATE S16 czBmpHdlInHoRspEvntErr ARGS((CzPeerCb *peerCb, 
                                         CzUeCb     *ueCb,           
                                         CztEvnt    *evnt,
                                         U8         hoMsgType,
                                         U16        oldUEX2APIdVal));


PRIVATE S16 czBmpHdlInReqErr ARGS((CzPeerCb *peerCb, 
                                        CzUeCb   *ueCb,           
                                        CztEvnt  *evnt,
                                        U8        hoMsgType,
                                        U16       oldUEX2APIdVal));


PRIVATE S16 czBmpSndErrIndToPeer ARGS((CzPeerCb  *peerCb,   
                                       CztEvnt   *evnt,     
                                       CztUeInfo *bmpUeInfo,
                                       U8        hoMsgType,
                                       U8        causeType,
                                       U8        causeVal,
                                       U8        lclCauseVal));

PRIVATE S16 czBmpSndFailRspToPeer ARGS((CzPeerCb  *peerCb,
                                        CztEvnt   *evnt,     
                                        CztUeInfo *bmpUeInfo,
                                        CzIeInfo  *errIeInfo,
                                        U8        causeType,
                                        U8        causeVal,
                                        U8        lclCauseVal));

PRIVATE S16 czBmpSndFailErrToPeer ARGS((CzPeerCb  *peerCb,
                                        CztEvnt   *evnt,
                                        CztUeInfo *bmpUeInfo,
                                        U8        hoMsgType,
                                        CztStrmId streamId)); /* window's warning fix changed data type to CztStrmId from u8 */

PRIVATE S16 czBmpGenLclErrInd ARGS((CzPeerCb  *peerCb,
                                    CztEvnt   *evnt,
                                    U8        causeType,
                                    U8        causeVal));

PRIVATE S16 czBmpGenErrIndEvntErr ARGS((CzPeerCb  *peerCb,
                                        CztEvnt   *evnt,
                                        U8        msgDirectn,
                                        U8        causeVal));


PRIVATE S16 czBmpIsErrIndValid ARGS((CzPeerCb  *peerCb,   
                                     U8        hoMsgType,
                                     U8        msgDirectn));



/* cz001.301 : Removed the function czBmpIsHoReqValid
 * Reason: The function of the czBmpIsHoReqValid is mainly to 
 * validate the Intigratity and encryption Algorithm, These checks 
 * has to be done by the application not the X2AP. So as this is Redundant code.*/


/** Single Basic Mobility Management Procedure Matrix for an UE intended to be
    handed over to a peer eNodeB over an X2 interface. 
  * This state machine matrix is referred for the all the outgoing Basic Mobility 
  * Management messages only when this eNodeB is in ESTABLISHED state with a
  * peer eNodeB in question. */

PRIVATE CZPF CzBmpOutSm[CZ_BMP_HO_MAX_EVENTS][CZ_BMP_HO_MAX_STATES] =
{
   {
   czBmpHdlOutHoReq,          /* HO Request in Handover Idle State */ 
   czBmpHdlOutEvntErr,        /* HO Request in Handover Progress State */ 
   czBmpHdlOutEvntErr         /* HO Request in Handover Prepared State */ 
   },

   {
   czBmpHdlOutEvntErr,         /* HO Request Acknowledge in Handover Idle State */ 
   czBmpHdlOutHoAck,           /* HO Request Acknowledge in Handover Progress State */ 
   czBmpHdlOutEvntErr          /* HO Request Acknowledge in Handover Prepared State */ 
   },

   {
   czBmpHdlOutEvntErr,        /* HO Preparation Failure in Handover Idle State */ 
   czBmpHdlOutHoFail,         /* HO Preparation Failure in Handover Progress State */ 
   czBmpHdlOutEvntErr         /* HO Preparation Failure in Handover Prepared State */ 
   },

   {
   czBmpHdlOutEvntErr,        /* HO Cancel in Handover Idle State */
   czBmpHdlOutHoCancel,       /* HO Cancel in Handover Progress State */ 
   czBmpHdlOutHoCancel        /* HO Cancel in Handover Prepared State */ 
   },

   {
   czBmpHdlOutEvntErr,        /* SN Status Transfer in Handover Idle State */ 
   czBmpHdlOutEvntErr,        /* SN Status Transfer in Handover Progress State */ 
   czBmpHdlOutSnStTrnsfr      /* SN Status Transfer in Handover Prepared State */ 
   },

   {
   czBmpHdlOutEvntErr,         /* UE Context Release in Handover Idle State */ 
   czBmpHdlOutEvntErr,         /* UE Context Release in Handover Progress State */ 
   czBmpHdlOutUeCnxtRelease    /* UE Context Release in Handover Prepared State */ 
   }

};


/** Single Basic Mobility Management Procedure Matrix for an UE intended to be
    handed over to a peer eNodeB over an X2 interface. 
  * This state machine matrix is referred for the all the incoming Basic Mobility 
  * Management messages only when this eNodeB is in ESTABLISHED state with a
  * peer eNodeB in question. */

PRIVATE CZPF CzBmpIncSm[CZ_BMP_HO_MAX_EVENTS][CZ_BMP_HO_MAX_STATES] =
{
   {
   czBmpHdlInHoReq,            /* HO Request in Handover Idle State */ 
   czBmpHdlInHoReqErr,         /* HO Request in Handover Progress State */ 
   czBmpHdlInHoReqErr          /* HO Request in Handover Prepared State */ 
   },

   {
   czBmpHdlInHoRspEvntErr,     /* HO Request Acknowledge in Handover Idle State */ 
   czBmpHdlInHoAck,            /* HO Request Acknowledge in Handover Progress State */ 
   czBmpHdlInEvntErr           /* HO Request Acknowledge in Handover Prepared State */ 
   },

   {
   czBmpHdlInHoRspEvntErr,     /* HO Preparation Failure in Handover Idle State */ 
   czBmpHdlInHoFail,           /* HO Preparation Failure in Handover Progress State */ 
   czBmpHdlInEvntErr           /* HO Preparation Failure in Handover Prepared State */ 
   },

   {
   czBmpHdlInReqErr,           /* HO Cancel in Handover Idle State */
   czBmpHdlInHoCancel,         /* HO Cancel in Handover Progress State */ 
   czBmpHdlInHoCancel          /* HO Cancel in Handover Prepared State */ 
   },

   {
   czBmpHdlInReqErr,          /* SN Status Transfer in Handover Idle State */ 
   czBmpHdlInEvntErr,         /* SN Status Transfer in Handover Progress State */ 
   czBmpHdlInSnStTrnsfr       /* SN Status Transfer in Handover Prepared State */ 
   },

   {
   czBmpHdlInEvntErr,         /* UE Context Release in Handover Idle State */ 
   czBmpHdlInEvntErr,         /* UE Context Release in Handover Progress State */ 
   czBmpHdlInUeCnxtRelease    /* UE Context Release in Handover Prepared State */ 
   }

};



/**
  This event key map is used to get the eNodeB type, that is, whether source or
  target of a particular procedure based on the message type and its direction
  and key is calculated from it as, Key =  old X2AP ID + eNodeB type 
  source/target of mobility management procedure. */
PRIVATE U8 czBmpEvntKeyMap[CZ_BMP_HO_MAX_EVENTS][CZ_MAX_MSG_DIRECTN] = 
{
  /****Message Direction Value****/  /* X2AP MM Message */       

/* Procedure     Procedure
   based         based   
   src eNodeB    tgt eNodeB */  

   { CZ_SRC,       CZ_TGT },            /*  CZ_EVNT_HO_REQ */
   { CZ_TGT,       CZ_SRC },            /*  CZ_EVNT_HO_REQ_ACK */ 
   { CZ_TGT,       CZ_SRC },            /*  CZ_EVNT_HO_PREP_FAIL */
   { CZ_SRC,       CZ_TGT },            /*  CZ_EVNT_HO_CANCEL */
   { CZ_SRC,       CZ_TGT },            /*  CZ_EVNT_SN_STATUS_TRFR */
   { CZ_TGT,       CZ_SRC }             /*  CZ_EVNT_UE_CXT_RELEASE */
  
};

#ifdef DEBUGP
/** Matrix containing the description of the BMP events */
PRIVATE const S8 * czBmpEvntPrintMap[] = 
{
  /* X2AP BMP Message */       
  "Handover Request",
  "Handover Request Acknowledge",
  "Handover Preparation Failure",
  "Handover Cancel",
  "SN Status Transfer",
  "UE Context Release",
  "Unknown Event"
};

/** States in Basic Mobility Procedure */
PRIVATE const S8 * czBmpStatePrintMap[] = 
{
  "Handover Idle",
  "Handover Progress",
  "Handover Prepared",
};

#endif /* DEBUGP */

/**
    * @brief This function is the entry point for all the X2AP basic mobility
      procedural messages. This is invoked from Upper Interface module
      to process the outgoing X2AP mobility management messages.
      This is invoked from Transport module to process the incoming
      X2AP mobility management messages.

    * @details This function checks if the mobility message to handle is received
      in state of active peer involved for UE handover.
      It retrieves:
      - An associated X2AP mobility message event from X2AP ASN message type.
      - An UE control block from the existing hash list of active UE handover
        control blocks.  
      It then invoked individual function handlers based on mobility event 
      considered if in valid or an invalid state for the associated event.
    *  
    * @param[in] peerCb      The active peerCb from this eNodeB involved for an
                             UE handover.
    * @param[in] evnt        The pointer with X2AP ASN PDU structure received
                             from peer for an UE handover.
    * @param[in] hoMsgType   Basic Mobility management message, represented
                             by Trillium X2AP Message value.
    * @param[in] msgDirectn  The Message direction: 
                            -CZ_DIR_OUT: If from user, 
                            -CZ_DIR_IN: If from peer. 
    * @note: For a list of Trillium X2AP message representation, refer to czt.h.
    *  
    * @return 
    * -# ROK if success.
    * -# RFAILED otherwise.
    * 
    */

#ifdef ANSI
PUBLIC S16 czBmpHdlHoEvnt
(
CzPeerCb   *peerCb,                 
CztEvnt    *evnt,                    
U8          hoMsgType,
U8          msgDirectn             
)
#else /* ANSI */
PUBLIC S16 czBmpHdlHoEvnt(peerCb, evnt, hoMsgType, msgDirectn)
CzPeerCb   *peerCb;                
CztEvnt    *evnt;                   
U8          hoMsgType;
U8          msgDirectn;           
#endif /* ANSI */
{
   U8 hoMsgEvnt; 
   U16 ueCbKeyPrefx = 0;
   U32 ueCbKey = 0;
   CzUeCb *ueCb = NULLP; 
   CztUE_X2AP_ID *oldUEX2APId = NULLP;
   U16  oldUEX2APIdVal = CZ_UNKNOWN_UE_X2AP_ID;
   S16 ret = ROK;
   U8  state;

   CZ_TRC2(czBmpHdlHoEvnt)

   if(CZ_DIR_OUT == msgDirectn)
   {
      /* Update the total statistics outgoing message */
      czUtlUpdOutSts(peerCb, hoMsgType);
   }
   else if(CZ_DIR_IN == msgDirectn)
   {
      /* Update the total statistics of incoming message */
      czUtlUpdInSts(peerCb, hoMsgType);
   }
   ret = czBmpIsPeerValid(peerCb, evnt, hoMsgType, msgDirectn);
 
   if(ROK != ret) /* Peer not in a valid state */
   {
      CZ_RETVALUE(ret);
   }
 
   /*******************************************************/
   /* Retrieve the X2AP Basic Mobility Management Message mapped event */
   /*******************************************************/
   CZ_GET_EVENT_MSG_MAP(hoMsgType, hoMsgEvnt);

   CZ_BMP_LOG_ENTRY_INFO(peerCb, hoMsgEvnt, msgDirectn);

   czUtlGetIE(&(evnt->pdu), Cztid_Old_eNB_UE_X2AP_ID, (TknU8 **)(&oldUEX2APId));

   /* No need to check from peer because this IE is mandatory in all
      BMP messages and is dropped using decoder and TPM */
   if((CZ_DIR_OUT == msgDirectn) && 
      ((NULLP == oldUEX2APId) || 
       (oldUEX2APId->pres != PRSNT_NODEF) ||
       (!(CZ_BMP_IS_VALID_UE_X2AP_ID(oldUEX2APId->val)))))
   {
      if(oldUEX2APId)
      {
         /*kworks fix: type cast*/
         oldUEX2APIdVal = (U16) oldUEX2APId->val;
      }
#ifdef ALIGN_64BIT
      CZ_DBG_ERROR((CZ_PRNT_BUF, "Invalid old UE X2AP ID(%d)"\
                  " when sending mobility event(%s) to peer(%d)",\
                   oldUEX2APIdVal, czBmpEvntPrintMap[hoMsgEvnt],\
                   (peerCb->peerId)));
#else /* ALIGN_64BIT */
      CZ_DBG_ERROR((CZ_PRNT_BUF, "Invalid old UE X2AP ID(%d)"\
                  " when sending mobility event(%s) to peer(%ld)",\
                   oldUEX2APIdVal, czBmpEvntPrintMap[hoMsgEvnt],\
                   (peerCb->peerId)));
#endif /* ALIGN_64BIT */
      czUtlUpdOutErrSts(peerCb, hoMsgType);

      /* Inform back to user */
      /* Invoke a certain function in UIM to generate this */
      CZ_BMP_FILL_UIM_UE_ERR_INFO(peerCb, hoMsgType, oldUEX2APIdVal, \
                                  CZ_UNKNOWN_UE_X2AP_ID);

      czBmpGenLclErrInd(peerCb, evnt, CZT_ERRTYPE_INV_SDU_PARAM,
                        CZT_ERRCAUSE_INV_OLD_UE_X2AP_ID);

      CZ_RETVALUE(RFAILED);
   }
   /*kworks fix: Added NULLP Check and type cast*/
   if(NULLP != oldUEX2APId)
   {
      oldUEX2APIdVal = (U16) oldUEX2APId->val;
   }
   /*******************************************************/
   /* Retrieve the key for this message */
   /*******************************************************/
   CZ_BMP_GET_EVENT_KEY(oldUEX2APIdVal, hoMsgEvnt, msgDirectn, ueCbKeyPrefx,
                        ueCbKey);

   /* Retrieve the UeCb pointer from the peer */ 
   czNmmFindUeCb(peerCb, ueCbKey, &ueCb);

   /*******************************************************/
   /* If UeCb present invoke BMP FSM handler to process the message */
   /* Else, No UE Cb Found - Assume the state is Idle/ Invalid
    * Handle appropriately based on mobility message */ 
   /*******************************************************/

   if(NULLP != ueCb)
   {
      state = ueCb->state;
   }
   else
   {
      state = CZ_BMP_HO_IDLE;
   }

   /*******************************************************/
   if(CZ_DIR_OUT == msgDirectn)
   {
#ifdef ALIGN_64BIT
      CZ_DBG_INFO((CZ_PRNT_BUF, "Outgoing event (%d), through BMP in state(%d)"\
                   "for peer(%d)", hoMsgEvnt, state, peerCb->peerId));
#else /* ALIGN_64BIT */
      CZ_DBG_INFO((CZ_PRNT_BUF, "Outgoing event (%d), through BMP in state(%d)"\
                   "for peer(%ld)", hoMsgEvnt, state, peerCb->peerId));
#endif /* ALIGN_64BIT */

      ret = CzBmpOutSm[hoMsgEvnt][state](peerCb, ueCb, evnt, 
                                         hoMsgType, oldUEX2APIdVal);

      /* The message couldnt not be sent out to network
         update error message statistics */
                if (ROK != ret)
                {
                   czUtlUpdOutErrSts(peerCb, hoMsgType);
                }
   }
   /*******************************************************/

   /*******************************************************/
   else if( CZ_DIR_IN == msgDirectn)
   {
#ifdef ALIGN_64BIT
      CZ_DBG_INFO((CZ_PRNT_BUF, "Incoming event(%d), through BMP in state(%d)"\
                   "from peer(%d)", hoMsgEvnt, state, peerCb->peerId));
#else /* ALIGN_64BIT */
      CZ_DBG_INFO((CZ_PRNT_BUF, "Incoming event(%d), through BMP in state(%d)"\
                   "from peer(%ld)", hoMsgEvnt, state, peerCb->peerId));
#endif /* ALIGN_64BIT */
      ret = CzBmpIncSm[hoMsgEvnt][state](peerCb, ueCb, evnt,
                                         hoMsgType, oldUEX2APIdVal);

      /* If the message is not delivered to the user update error
         message statistics */
                if (ROK != ret)
                {
                    czUtlUpdInErrSts(peerCb, hoMsgType);
                }
   }
   /*******************************************************/

   CZ_RETVALUE(ret);

} /* End of function czBmpHdlHoEvnt*/


/**
    * @brief This function validates that all the X2AP basic mobility
      procedural messages received from user or from peer is actually 
      in state of active peer involved for UE handover.

    * @details 
    When peer is not actively connected this function generates:<br>
  - An X2AP Handover Preparation Failure for received Handover Request
        from peer.
  - An X2AP Error Indication for all class2 messages received from peer
       if peer is not actively connected.

      This function is invoked from czBmpHdlHoEvnt.

    * @param[in] peerCb      The active peerCb from this eNodeB involved for an
                             UE handover.
    * @param[in] evnt        The pointer with X2AP ASN PDU structure received
                             from user or from peer for an UE handover.
    * @param[in] hoMsgType   Basic Mobility management message value, represented
                             by Trillium X2AP Message value.
    * @param[in] msgDirectn  The Message direction: 
                            -CZ_DIR_OUT: If from user, 
                            -CZ_DIR_IN: If from peer. 
    * @note: For a list of Trillium X2AP message representation, refer to czt.h.
    *  
    * @return 
    * -# ROK if success.
    * -# RFAILED otherwise.
    * 
    */


#ifdef ANSI
PRIVATE S16 czBmpIsPeerValid
(
CzPeerCb   *peerCb,                 
CztEvnt    *evnt,                    
U8         hoMsgType,
U8         msgDirectn             
)
#else /* ANSI */
PRIVATE S16 czBmpIsPeerValid(peerCb, evnt, hoMsgType, msgDirectn)
CzPeerCb   *peerCb;                
CztEvnt    *evnt;                   
U8         hoMsgType;
U8         msgDirectn;           
#endif /* ANSI */
{
   U8  hoMsgEvnt;
   CztUE_X2AP_ID *oldUEX2APId = NULLP;
   CztUE_X2AP_ID *newUEX2APId = NULLP;
   CztStrmId streamId;
   S16 ret = ROK;

   CZ_TRC2(czBmpIsPeerValid);

   /* cz001.101 : BMP accepts incoming messages in CZ_GP_CONNECTING state.
    * i.e. when Setup/Reset is Pending. Assumes that the ressponse for 
    * Setup/Reset arrives later than current message. */
   /* If in IDLE State inform back to user or peer with valid error response
      error indication */
   /* If in CONNECTING state BMP messages are not allowed from user 
      as X2 is not up yet */
   /* None of the BMP messages are allowed */
   if((CZ_GP_IDLE == peerCb->state) ||
      ((CZ_GP_CONNECTING == peerCb->state) && (CZ_DIR_OUT == msgDirectn)))
   {
      CZ_GET_EVENT_MSG_MAP(hoMsgType, hoMsgEvnt);
      if(CZ_DIR_OUT == msgDirectn)
      {
#ifdef ALIGN_64BIT
         CZ_DBG_ERROR((CZ_PRNT_BUF, "Mobility event(%s) received from user"\
                      "when peer (%d) not in active state(%d)\n", 
                       czBmpEvntPrintMap[hoMsgEvnt], (peerCb->peerId),\
                       (peerCb->state)));
#else /* ALIGN_64BIT */
         CZ_DBG_ERROR((CZ_PRNT_BUF, "Mobility event(%s) received from user"\
                      "when peer (%ld) not in active state(%d)\n", 
                       czBmpEvntPrintMap[hoMsgEvnt], (peerCb->peerId),\
                       (peerCb->state)));
#endif /* ALIGN_64BIT */

         /* Increment the outgoing error counter for this message resulted in 
            error */
          czUtlUpdOutErrSts(peerCb, hoMsgType); 

         /* Inform back to user */
         /* Invoke a certain function in UIM to generate this */
         CZ_BMP_FILL_UIM_MSG_INFO(peerCb, hoMsgType);

         czBmpGenLclErrInd(peerCb, evnt, CZT_ERRTYPE_INTERNAL,
                           CZT_ERRCAUSE_INV_STATE);
      }
      else
      {
#ifdef ALIGN_64BIT
         CZ_DBG_ERROR((CZ_PRNT_BUF, "Mobility event(%s) received from peer"\
                   "(%d) while not in active state(%d)\n", 
                 czBmpEvntPrintMap[hoMsgEvnt], (peerCb->peerId),\
                 (peerCb->state)));
#else /* ALIGN_64BIT */
         CZ_DBG_ERROR((CZ_PRNT_BUF, "Mobility event(%s) received from peer"\
                   "(%ld) while not in active state(%d)\n", 
                 czBmpEvntPrintMap[hoMsgEvnt], (peerCb->peerId),\
                 (peerCb->state)));
#endif /* ALIGN_64BIT */
         /* Increment the incoming error counter for this message resulted in 
            error */
          czUtlUpdInErrSts(peerCb, hoMsgType); 

         /* Inform back to peer */
         /* No need to check the return value */
         if(CZT_MSG_HO_REQ == hoMsgType)
         {
            czUtlGetIE(&(evnt->pdu), Cztid_Old_eNB_UE_X2AP_ID, 
                        (TknU8 **)(&oldUEX2APId));
            /*kworks fix:Added NULLP Check*/
            if(oldUEX2APId != NULLP)
            { 
               CZ_BMP_FILL_UE_INFO(oldUEX2APId->val, CZ_UNKNOWN_UE_X2AP_ID);
            }
            streamId = czUtlGetStream(peerCb, &streamId);
         }
         else
         { 
            czUtlGetIE(&(evnt->pdu), Cztid_New_eNB_UE_X2AP_ID, 
                       (TknU8 **)(&newUEX2APId));
            if(newUEX2APId) 
            {  /*kworks fix : typecast to U16*/
               czUeInfo.newUEX2APId = (U16) newUEX2APId->val; 
            } 
            streamId = peerCb->peerCfg.globalStreamId;
         }
         czBmpSndFailErrToPeer(peerCb, evnt, &czUeInfo, hoMsgType, streamId);
      }
      CZ_RETVALUE(RFAILED); /* Peer is not valid */
   }
   /* Rest of the states are valid - CONNECTED/CONNECTING */
   CZ_RETVALUE(ret);
} /* End of function czBmpIsPeerValid */


/*******************************************************/
/* Handling X2AP Error Indication in Basic mobility
   management procedure module. */
/*******************************************************/

/**
    * @brief This function is the entry point for handling X2AP Error Indication
      received from peer or from user. Error Indication can be sent or received
      on previously sent or received basic mobility management procedural messages. 
    * @details This function retrieves:
      - An UE control block from the existing hash list of active UE handover
        control blocks using oldUEX2APId value if present in error indication.  
      - It then delegates the function handling to different internal error
        indication handling functions to process on ueCb and cause values
        individually.

      -# Error Indication can be received from user to send to peer:
       - When incoming HO Request Ack has errors.
       - When incoming HO Preparation Failure has errors.
       - When incoming HO Cancel has errors.
       - When incoming SN Status Transfer has errors.
       - When incoming UE Context Release has errors.
      -# Error Indication can be received from peer when:
       - When outgoing HO Request Ack has errors.
       - When outgoing HO Preparation Failure has errors.
       - When outgoing HO Cancel has errors.
       - When outgoing SN Status Transfer has errors.
       - When outgoing UE Context Release has errors.

    *  
    * @param[in] peerCb      The active peerCb from this eNodeB involved for an
                             UE handover.
    * @param[in] evnt        The pointer with X2AP ASN PDU structure received
                             from user or from peer for an UE handover.
    * @param[in] hoMsgType   Basic Mobility management message, represented
                             by Trillium X2AP Message value.
    * @param[in] msgDirectn  The Message direction: 
                            -CZ_DIR_OUT: If from user, 
                            -CZ_DIR_IN: If from peer. 
    *  
    * @return 
    * -# ROK if success.
    * -# RFAILED otherwise.
    * 
    */

#ifdef ANSI
PUBLIC S16 czBmpHdlErrInd
(
CzPeerCb  *peerCb,                
CztEvnt   *evnt,                    
U8        hoMsgType,
U8        msgDirectn             
)
#else /* ANSI */
PUBLIC S16 czBmpHdlErrInd(peerCb, evnt, hoMsgType, msgDirectn)
CzPeerCb  *peerCb;                
CztEvnt   *evnt;                   
U8        hoMsgType;
U8        msgDirectn;           
#endif /* ANSI */
{
   U8 hoMsgEvnt; 
   U16 ueCbKeyPrefx = 0, hoEvntPrefx = 0;
   U32 ueCbKey = 0;
   CzUeCb *ueCb = NULLP;
   U16 oldUEX2APIdVal = CZ_UNKNOWN_UE_X2AP_ID;
   CztUE_X2AP_ID *oldUEX2APId = NULLP;
   S16 ret;

   CZ_TRC2(czBmpHdlErrInd)

   if(CZ_DIR_OUT == msgDirectn)
   {
      /* Update total outgoing statistics for error indication */
      czUtlUpdOutSts(peerCb, CZT_MSG_ERR_IND);
   }
   else if(CZ_DIR_IN == msgDirectn)
   {
      /* Update total incoming statistics for error indication */
      czUtlUpdInSts(peerCb, CZT_MSG_ERR_IND);
   }

#ifdef ALIGN_64BIT
   CZ_DBG_INFO((CZ_PRNT_BUF, "Sending / Receiving Error Indication for peer \
                             peer(%d): ", peerCb->peerId)); 
#else /* ALIGN_64BIT */
   CZ_DBG_INFO((CZ_PRNT_BUF, "Sending / Receiving Error Indication for peer \
                             peer(%ld): ", peerCb->peerId)); 
#endif /* ALIGN_64BIT */

   ret = czBmpIsErrIndValid(peerCb, hoMsgType, msgDirectn);
   if(ROK != ret)
   {
      czBmpGenErrIndEvntErr(peerCb, evnt, msgDirectn, (U8)ret); /*windows warning fixed by typecasting S16 to U8*/
      CZ_RETVALUE(ret);
   }

   /**********************************************************/
   /* First let us check based on OldUEX2AP ID to act on UeCb*/
   /**********************************************************/
   czUtlGetIE(&(evnt->pdu), Cztid_Old_eNB_UE_X2AP_ID, (TknU8 **)&oldUEX2APId);

   if((NULLP != oldUEX2APId) && (oldUEX2APId->pres == PRSNT_NODEF))
   {
      /*kworks fix: type cast*/
      oldUEX2APIdVal = (U16)oldUEX2APId->val;

      /*******************************************************/
      /* Retrieve the X2AP Basic Mobility Management Message mapped event */
      /*******************************************************/
      CZ_GET_EVENT_MSG_MAP(hoMsgType, hoMsgEvnt);

      /*******************************************************/
      /* Retrieve the key for this error indication based on
         message sent earlier. */
      /*******************************************************/
      CZ_BMP_GET_EVENT_KEY_PREFIX(hoMsgEvnt, msgDirectn, hoEvntPrefx);
      ueCbKeyPrefx = (CZ_SRC == hoEvntPrefx) ? (CZ_TGT):(CZ_SRC);
      ueCbKey = PutHiWord(oldUEX2APIdVal, ueCbKeyPrefx);

      /* Retrieve the UeCb pointer from the peer */ 
      ret = czNmmFindUeCb(peerCb, ueCbKey, &ueCb);

      /*******************************************************/
      /* If no UE CB, for some reasons, if user has sent the 
       * error indication, inform the user about it */
      /*******************************************************/
   }

   /*******************************************************/
   /* We are still ok with oldUEX2APID being not present,
    * as this is optional, continuing,.... */
   /*******************************************************/
   if(CZ_DIR_OUT == msgDirectn)
   {
      ret = czBmpHdlOutErrInd(peerCb, ueCb, evnt, hoMsgType, oldUEX2APIdVal); 
      /* The message couldnt not be sent out to network
         update error message statistics */
      if (ROK != ret)
      {
         czUtlUpdOutErrSts(peerCb, CZT_MSG_ERR_IND);
      }
   }
   else
   {
      ret = czBmpHdlInErrInd(peerCb, ueCb, evnt, hoMsgType, oldUEX2APIdVal); 

      /* If the message is not delivered to the user update error
         message statistics */
      if (ROK != ret)
      {
         czUtlUpdOutErrSts(peerCb, CZT_MSG_ERR_IND);
      }
   }

   CZ_RETVALUE(ret);
} /* End of function czBmpHdlErrInd */



/**
    * @brief This function handles sending X2AP Error Indication to peer.
      Error Indication can be sent on previously received basic mobility
      management procedural messages. 

      -# Error Indication can be received from user to send to peer:
       - When incoming HO Request Ack has errors.
       - When incoming HO Preparation Failure has errors.
       - When incoming HO Cancel has errors.
       - When incoming SN Status Transfer has errors.
       - When incoming UE Context Release has errors.

    * @details If error indication is to be sent on Handover Request Acknowledge
       then the respective UE control block's state is switched back to previous
       state.
    *  
    * @param[in] peerCb      The active peerCb from this eNodeB involved for an
                             UE handover.
    * @param[in] ueCb        An UECb pointer from this eNodeB for an UE handover.
    * @param[in] evnt        The pointer with X2AP ASN PDU structure received
                             from user for an UE handover.
    * @param[in] hoMsgType   Basic Mobility management message, represented
                             by Trillium X2AP Message value.
    * @param[in] oldUEX2APIdVal Old UE X2AP ID Value present in the outgoing 
                                Error Indication.
    *  
    * @return 
    * -# ROK if success.
    * -# RFAILED otherwise.
    * 
    */
#ifdef ANSI
PRIVATE S16 czBmpHdlOutErrInd
(
CzPeerCb  *peerCb,                
CzUeCb    *ueCb,
CztEvnt   *evnt,                    
U8        hoMsgType,
U16       oldUEX2APIdVal
)
#else /* ANSI */
PRIVATE S16 czBmpHdlOutErrInd(peerCb, ueCb, evnt, hoMsgType, oldUEX2APIdVal)
CzPeerCb  *peerCb;                
CzUeCb    *ueCb;
CztEvnt   *evnt;                   
U8        hoMsgType;
U16       oldUEX2APIdVal;
#endif /* ANSI */
{
   CztUE_X2AP_ID *newUEX2APId = NULLP;
   CztCause *causeIE = NULLP;
   U16 newUEX2APIdVal = CZ_UNKNOWN_UE_X2AP_ID;
   U8 hoMsgEvnt;
   U8 causeType, causeVal;
   S16 ret = ROK;

   CZ_TRC2(czBmpHdlOutErrInd)

   CZ_GET_EVENT_MSG_MAP(hoMsgType, hoMsgEvnt);

#ifdef ALIGN_64BIT
   CZ_DBG_INFO((CZ_PRNT_BUF, "Outgoing Error Indication on BMP event(%s)"\
             " to peer(%d)\n", czBmpEvntPrintMap[hoMsgEvnt], peerCb->peerId)); 
#else /* ALIGN_64BIT */
   CZ_DBG_INFO((CZ_PRNT_BUF, "Outgoing Error Indication on BMP event(%s)"\
             " to peer(%ld)\n", czBmpEvntPrintMap[hoMsgEvnt], peerCb->peerId)); 
#endif /* ALIGN_64BIT */

   /*******************************************************/
   /* If no UE CB, for some reasons, user has sent the 
    * error indication with oldUEX2AP ID badly, 
    * inform the user about it */
   /*******************************************************/
   if((CZ_UNKNOWN_UE_X2AP_ID != oldUEX2APIdVal) && (!ueCb)) 
   {
      /* Inform back to user */
      /* Invoke a certain function in UIM to generate this */
      CZ_BMP_FILL_UIM_UE_ERR_INFO(peerCb, CZT_MSG_ERR_IND,
                                  oldUEX2APIdVal, CZ_UNKNOWN_UE_X2AP_ID);

      czBmpGenLclErrInd(peerCb, evnt, CZT_ERRTYPE_INV_SDU_PARAM,
                        CZT_ERRCAUSE_INV_OLD_UE_X2AP_ID);
      CZ_RETVALUE(RFAILED);
   } 
   /*******************************************************/
   /* Found the UeCB using OldUEX2AP ID */
   /*******************************************************/
   else if(ueCb) 
   {
      /*******************************************************/
      /* Let us take help from ueCb's current state to act upon
       * ueCb */
      /*******************************************************/
      switch(ueCb->state)
      {
         case CZ_BMP_HO_PROGRESS: /* Means HO Request was received for
                                      this UE earlier. */
         {
            czNmmDelUeCb(ueCb);
            ueCb= NULLP;
            break;
         }                         
         default: 
         {  
            ret = ROKPENDING; 
            break;
         }
      }
   }

   /* No hints available yet */
   if(ROKPENDING == ret)
   {
      /*******************************************************/
      /* Let us take help from cause type and value to act upon ueCb */
      /*******************************************************/
      czUtlGetIE(&(evnt->pdu), Cztid_Cause, (TknU8 **)&causeIE);
      if((NULLP != causeIE) && (PRSNT_NODEF == causeIE->choice.pres))
      {
         causeType = causeIE->choice.val;
         causeVal = czUtlGetCause(causeIE);
         /* Check for which BMP procedure */
         switch(hoMsgType)
         {
            case CZT_MSG_HO_ACK: /* To send Error Indication for HO Req Ack */
            {
               if(CztCauseRadioNwunknown_new_eNB_UE_X2AP_IDEnum == causeVal)
               {  
                  /* Allow the peer to send another HO ACK reset the related fields */
                  ueCb->newUEX2APId = CZ_UNKNOWN_UE_X2AP_ID; 
                  ueCb->state = CZ_BMP_HO_PROGRESS; 
               }
               /* Rest of the procCodes are grouped in default: */
               /* HO Preparation Fail */
               /* HO Cancel */ 
               /* Not required to act on UeCB, as received HO Prep Fail/ HO Cancel
                * is errored with decoding. 
                  Anyway this case never happens, found the UE CB, 
                  when we are here. */
               /* SN Status Transfer */ 
               /* Not required to handle, as SN Status Transfer can be sent
                  multiple times. */
               /* UE Ctxt Release */
               /* Not required to handle, as previously received UE Ctxt
                  Release makes it already delete the UE CB when it was sent
                  earlier. This case will never happen, as this is the case
                  when we find the UE CB, when we are here. */
               break;
            }
            default: /* < Log as info > */
               break;
         }
      } /* Help with causeIE */
   } /* Continuation with ROKPENDING */

   /* Nothing more can be done, directly send out upto TPM */

   /* Finally we are ready to send to TPM :) */

   czUtlGetIE(&(evnt->pdu), Cztid_New_eNB_UE_X2AP_ID, (TknU8 **)&newUEX2APId);
   /*kworks fix: type cast*/
   newUEX2APIdVal = ((NULLP != newUEX2APId) && 
                     (PRSNT_NODEF == newUEX2APId->pres))?\
                   (U16)(newUEX2APId->val):(CZ_UNKNOWN_UE_X2AP_ID);
#ifdef ALIGN_64BIT
   CZ_DBG_INFO((CZ_PRNT_BUF, "Sending Error Indication on BMP event (%s)"\
                "for an UE(%d:%d) to peer(%d) on stream(%d)",\
                czBmpEvntPrintMap[hoMsgEvnt],oldUEX2APIdVal, newUEX2APIdVal,\
               (peerCb->peerId), (peerCb->peerCfg.globalStreamId))); 
#else /* ALIGN_64BIT */
   CZ_DBG_INFO((CZ_PRNT_BUF, "Sending Error Indication on BMP event (%s)"\
                "for an UE(%d:%d) to peer(%ld) on stream(%d)",\
                czBmpEvntPrintMap[hoMsgEvnt],oldUEX2APIdVal, newUEX2APIdVal,\
               (peerCb->peerId), (peerCb->peerCfg.globalStreamId))); 
#endif /* ALIGN_64BIT */

   /*******************************************************/
   /* Invoke a certain function in Transport module to send it out to peer */
   /*******************************************************/
   ret = czTpmSndToLower(peerCb, &(evnt->pdu), 
                         (peerCb->peerCfg.globalStreamId));

   /* Nothing to roll back */
   if(ROK != ret)
   {
#ifndef ALIGN_64BIT
      CZ_DBG_FATAL((CZ_PRNT_BUF, "Transmission of the outcoming X2-Error \
               Indication failed towards the peer(%ld) for UE(%d:%d)\n", 
               (peerCb->peerId), oldUEX2APIdVal, newUEX2APIdVal));
#else /* ALIGN_64BIT */
      CZ_DBG_FATAL((CZ_PRNT_BUF, "Transmission of the outcoming X2-Error \
               Indication failed towards the peer(%d) for UE(%d:%d)\n", 
               (peerCb->peerId), oldUEX2APIdVal, newUEX2APIdVal));
#endif /* ALIGN_64BIT */
      /* Inform the user. 
         Invoke a certain function in UIM to generate this */
      CZ_BMP_FILL_UIM_UE_ERR_INFO(peerCb, CZT_MSG_ERR_IND,
                                  oldUEX2APIdVal, newUEX2APIdVal);

      czBmpGenLclErrInd(peerCb, evnt, CZT_ERRTYPE_MSG_SEND_FAILED,
                        (U8)ret); /* windows warning fixed by typecasting S16 to U8 */
   }
   else
   {
      CZ_FREEEVNT(evnt); /* All Done in processing this message */
   }

   CZ_RETVALUE(ret);

} /* End of function czBmpHdlOutErrInd */


/**
    * @brief This function handles delivering X2AP Error Indication to user.
      Error Indication can be received from peer on previously sent basic mobility
      management procedural messages. 

      Error Indication can be received from peer :
       - When outgoing HO Request Ack has errors.
       - When outgoing HO Preparation Failure has errors.
       - When outgoing HO Cancel has errors.
       - When outgoing SN Status Transfer has errors.
       - When outgoing UE Context Release has errors.

    * @details If error indication is received on Handover Request Acknowledge
       then the respective UE control block's state is switched back to previous
       state.
    *  
    * @param[in] peerCb      The active peerCb from this eNodeB involved for an
                             UE handover.
    * @param[in] ueCb        An UECb pointer from this eNodeB for an UE handover.
    * @param[in] evnt        The pointer with X2AP ASN PDU structure received
                             from peer for an UE handover.
    * @param[in] hoMsgType   Basic Mobility management message, represented
                             by Trillium X2AP Message value.
    * @param[in] oldUEX2APIdVal Old UE X2AP ID Value present in the incoming 
                                Error Indication.
    *  
    * @return 
    * -# ROK if success.
    * -# RFAILED otherwise.
    * 
    */


#ifdef ANSI
PRIVATE S16 czBmpHdlInErrInd
(
CzPeerCb  *peerCb,                
CzUeCb    *ueCb,
CztEvnt   *evnt,                    
U8        hoMsgType,
U16       oldUEX2APIdVal
)
#else /* ANSI */
PRIVATE S16 czBmpHdlInErrInd(peerCb, ueCb, evnt, hoMsgType, oldUEX2APIdVal)
CzPeerCb  *peerCb;                
CzUeCb    *ueCb;
CztEvnt   *evnt;                   
U8        hoMsgType;
U16       oldUEX2APIdVal;
#endif /* ANSI */
{
   CztCause *causeIE = NULLP;
   U8 causeType, causeVal;
   S16 ret = ROK;

   CZ_TRC2(czBmpHdlInErrInd)
   CZ_PRNT_PEERID

   /*******************************************************/
   /* If no UE CB, for some reasons, with peer sending the 
    * error indication to us with oldUEX2AP ID badly, 
    * inform the user about it */
   /*******************************************************/
   if((CZ_UNKNOWN_UE_X2AP_ID != oldUEX2APIdVal) && (!ueCb)) 
   {
      /* Not acting anything when received from peer because:
         1. Not able to find ueCB or unknow old UE X2AP ID.
         2. No point in sending Error Indication again back to peer, indicating
              invalid ID(s). 
         3. Best thing to drop it without acting but still give it to the user. */ 
      czUtlUpdInErrSts(peerCb, CZT_MSG_ERR_IND);
   }   
   /************************************************************/
   /* Found the UeCB using OldUEX2AP ID when received from peer.*/
   /************************************************************/
   else if(ueCb) 
   {
      /*******************************************************/
      /* Let us take help from ueCb's current state to act upon
       * ueCb */
      /*******************************************************/
      switch(ueCb->state)
      {
         case CZ_BMP_HO_PROGRESS: /* Means HO Request was sent for this UE */
         {
            czStopTmr(CZ_TMR_TRELOCPREP, (PTR *)ueCb); 
            czNmmDelUeCb(ueCb);
            ueCb= NULLP;
            break;
         }                         
         default: 
         {
            ret = ROKPENDING; 
            break;
         }
      }
   }

   /* No hints available yet */
   if(ROKPENDING == ret)
   {

      /*******************************************************/
      /* Let us take help from cause type and value to act upon ueCb */
      /*******************************************************/
      czUtlGetIE(&(evnt->pdu), Cztid_Cause, (TknU8 **)&causeIE);
      if((NULLP != causeIE) && (PRSNT_NODEF == causeIE->choice.pres))
      {
         causeType = causeIE->choice.val;
         causeVal = czUtlGetCause(causeIE);

         /* Check for which BMP procedure */
         switch(hoMsgType)
         {
            case CZT_MSG_HO_ACK: /* Received Error Indication for HO Request Ack */
            {
               if(CztCauseRadioNwunknown_new_eNB_UE_X2AP_IDEnum == causeVal)
               {  
                  /* Allow the user to send another HO ACK reset the
                   * related fields */
                   ueCb->newUEX2APId = CZ_UNKNOWN_UE_X2AP_ID; 
                   ueCb->state = CZ_BMP_HO_PROGRESS; 
               }
               /* There can be other cause types, with which peer has sent this */
               /* cz001.301: Fix for the build warnings */

#ifdef CZTV1
               else if((causeType > CAUSE_RADIONW) && (causeType <= CZT_CAUSE_MISC))
#else
               else if((causeType > CAUSE_RADIONW) && (causeType <= CAUSE_MISC))
#endif
               {
                  /* Let us delete this UECb, though it is believed ack was
                   * good when it was sent */
                  czNmmDelUeCb(ueCb);
                  ueCb= NULLP;
               }
               break;
            }
            /* Rest of the procCodes are grouped in default: */
            /* HO Preparation Fail */
            /* HO Cancel */ 
            /* Not required to handle, as HO Prep Fail/ HO Cancel makes it
               already delete the UE CB when it was sent earlier. 
               Anyway this case never happens, found the UE CB, 
               */
            /* SN Status Transfer */ 
            /* Not required to handle, as SN Status Transfer can be received
               multiple times. */
            /* UE Ctxt Release */
            /* Not required to handle, as UE Ctxt Release makes it already
               delete the UE CB when it was sent earlier. This case
               never happened, this is the case when the UE CB is found 
                */
               default: /* < Log as info > */
                  break;
         }
      } /* Help with causeIE */
   } /* Continuation with ROKPENDING */

   /* Nothing more can be done, directly send out upto UIM */

   /* Finally it is ready to send to UIM */

   ret = czUimSndGpErrInd(peerCb, evnt);
   if(ROK != ret)
   {
      CZ_DBG_FATAL((CZ_PRNT_BUF, "Indication of the incoming X2-Error \
               Indication failed for UE(%d:%d)\n",
               oldUEX2APIdVal, CZ_UNKNOWN_UE_X2AP_ID));

      /* Update the error statistics for the incoming error indication msgs */
      czUtlUpdInErrSts(peerCb, CZT_MSG_ERR_IND);

      CZ_FREEEVNT(evnt);
   }
   /* Peer is well aware about ErrInd it sent to use, no need to inform
    * the peer again. */

   CZ_RETVALUE(ret);

} /* End of function czBmpHdlInErrInd */


/*******************************************************/
/* End of handling X2AP Error Indication in Basic mobility
   management procedure module. */
/*******************************************************/


/**
    * @brief This function handles the X2AP Handover Request message
    * received from user or peer, for an UE in active handover. 
    * 
    * @details It generates a local Error Indication back to user
      - If duplicate UECb is found for received oldUEX2APIdVal.
      - If new UECb could not be created due to memory loss probably.
      - If message was not not able to deliver right upto user when upper
        SAP is not found for this peer or in invalid state or memory loss.
    *  
    * @param[in] peerCb        The active peerCb pointer from this eNodeB for an
                               UE handover.
    * @param[in] ueCb          An UECb pointer from this eNodeB for an UE handover.
    * @param[in] evnt          The pointer with X2AP ASN PDU structure received at
                               this state for an UE handover to send to peer.
    * @param[in] hoMsgType     Basic Mobility management message(CZT_MSG_HO_REQ),
                               represented by Trillium X2AP Message value.
    * @param[in] oldUEX2APIdVal Old UE X2AP ID Value present in the outgoing 
                                Handover Request.
    *  
    * @return 
    * -# ROK on success. 
    * -# RFAILED if error encountered within the function.
    * 
    */
#ifdef ANSI
PRIVATE S16 czBmpHdlOutHoReq
(
CzPeerCb   *peerCb,               
CzUeCb     *ueCb,               
CztEvnt    *evnt,                
U8         hoMsgType,
U16        oldUEX2APIdVal
)
#else /* ANSI */
PRIVATE S16 czBmpHdlOutHoReq(peerCb, ueCb, evnt, hoMsgType, oldUEX2APIdVal)
CzPeerCb   *peerCb; 
CzUeCb     *ueCb;            
CztEvnt    *evnt;                 
U8         hoMsgType;
U16        oldUEX2APIdVal;
#endif /* ANSI */
{
   U32 ueCbKey = 0;
   U16 ueCbKeyPrefx = CZ_SRC;
   S16 ret = ROK;

   CZ_TRC2(czBmpHdlOutHoReq)
   CZ_PRNT_PEERID;

   CZ_BMP_LOG_ENTRY_OUT_HDLR_INFO(peerCb,oldUEX2APIdVal,CZ_UNKNOWN_UE_X2AP_ID,
                                  CZ_EVNT_HO_REQ);

   /*******************************************************/
   /* No existing ueCb present in the ueCb's list for this UE */
   /*******************************************************/
   if(!ueCb) 
   {
      /*******************************************************/
      /* Retrieve the key for this message */
      /*******************************************************/
      CZ_BMP_GET_KEY(oldUEX2APIdVal, ueCbKeyPrefx, ueCbKey);

      /*******************************************************/
      /* Allocate a new UE CB for this message */
      /*******************************************************/
      ret = czNmmAddUeCb(peerCb, ueCbKey, &ueCb, oldUEX2APIdVal);
 
      if(ROK != ret)/* No need to null check NULLP == ueCb */
      {
         CZ_DBG_FATAL((CZ_PRNT_BUF, "Failed to allocate UeCb. UE(%d:%d)\n ", \
                  oldUEX2APIdVal, CZ_UNKNOWN_UE_X2AP_ID));
         /* No logging error required NMM does that */ 
         /* Failure either memory loss or insertion failed */ 
         /* Inform back to user */
         /* Invoke a certain function in UIM to generate this */
         CZ_BMP_FILL_UIM_UE_ERR_INFO(peerCb, CZT_MSG_HO_REQ,
                                  oldUEX2APIdVal, CZ_UNKNOWN_UE_X2AP_ID);

         czBmpGenLclErrInd(peerCb, evnt, CZT_ERRTYPE_SYS,
                        CZT_ERRCAUSE_UE_CB_CREATE_FAILED);
         CZ_RETVALUE(ret);
      }
   }
   /*******************************************************/
   /* Now Start TRelocprep timer for this UE */
   /*******************************************************/
   ret = czStartTmr(CZ_TMR_TRELOCPREP, czCb.nodeCb.protCfg.tRELOCprep,
              (PTR *) ueCb);
#if (ERRCLASS & ERRCLS_INT_PAR)
   if(ret != ROK)
   {
      CZ_DBG_ERROR((CZ_PRNT_BUF, 
               "Could not start the TRelocprep timer.\n"));
      CZ_SEND_LCL_GEN_ERR(peerCb, CZT_ERRTYPE_SYS,
            CZT_ERRCAUSE_TMR_ERR, CZT_MSG_HO_REQ); 
   }
#endif /* (ERRCLASS & ERRCLS_INT_PAR) */

   /* Finally - success; Now change the state */
   ueCb->state = CZ_BMP_HO_PROGRESS;

   czUtlGetStream(peerCb, &(ueCb->streamId));

   CZ_BMP_LOG_OUT_HDLR_INFO(peerCb, oldUEX2APIdVal, CZ_UNKNOWN_UE_X2AP_ID, 
                            CZ_EVNT_HO_REQ, (ueCb->streamId));

   /*******************************************************/
   /* Invoke a certain function in Transport module to send it out to peer */
   /*******************************************************/
   ret = czTpmSndToLower(peerCb, &(evnt->pdu), ueCb->streamId);

   /*******************************************************/
   /* Message is not sent out Roll Back*/
   /*******************************************************/
   if(ROK != ret)
   {
#ifndef ALIGN_64BIT
      CZ_DBG_FATAL((CZ_PRNT_BUF, "Transmission of the outcoming X2-Handover \
               Request failed towards the peer(%ld) for UE(%d:%d)\n", 
               (peerCb->peerId), oldUEX2APIdVal, CZ_UNKNOWN_UE_X2AP_ID));
#else /* ALIGN_64BIT */
      CZ_DBG_FATAL((CZ_PRNT_BUF, "Transmission of the outcoming X2-Handover \
               Request failed towards the peer(%d) for UE(%d:%d)\n", 
               (peerCb->peerId), oldUEX2APIdVal, CZ_UNKNOWN_UE_X2AP_ID));
#endif /* ALIGN_64BIT */
      CZ_BMP_LOG_OUT_HDLR_ERR(peerCb, oldUEX2APIdVal, CZ_UNKNOWN_UE_X2AP_ID,
                              CZ_EVNT_HO_REQ, ret);

      /* Increment the outgoing error counter for this message */
      /* czUtlUpdOutErrSts(peerCb, CZT_MSG_HO_REQ); */

      czStopTmr(CZ_TMR_TRELOCPREP, (PTR *)ueCb); 

      /*******************************************************/
      /* Remove the entry of this UE from peerCb List, to help user send it again */
      /*******************************************************/
      czNmmDelUeCb(ueCb);
      ueCb= NULLP;

      /* Inform the user */
      CZ_BMP_FILL_UIM_UE_ERR_INFO(peerCb, CZT_MSG_HO_REQ,
                                 oldUEX2APIdVal, CZ_UNKNOWN_UE_X2AP_ID);
      czBmpGenLclErrInd(peerCb, evnt, CZT_ERRTYPE_MSG_SEND_FAILED, (U8)ret); /* windows warning fixed by typecasting from U16 to U8 */
   }
   else
   {
      CZ_BMP_LOG_EXIT_OUT_HDLR_INFO(peerCb, oldUEX2APIdVal, 
                                    CZ_UNKNOWN_UE_X2AP_ID,
                                    CZ_EVNT_HO_REQ, ueCb->streamId);

      /* Increment the outgoing counter for this message */
      /* czUtlUpdOutSts(peerCb, CZT_MSG_HO_REQ); */

      CZ_FREEEVNT(evnt); /* All Done in processing this message */
   }   

   CZ_RETVALUE(ret);

} /* End of function czBmpHdlOutHoReq */


/**
    * @brief This function handles the X2AP Handover Request message
    * received from user or peer, for an UE in active handover. 
    * 
    * @details It generates an X2AP Handover Preparation Failure to the peer:
      - If duplicate UECb is found for received oldUEX2APIdVal.
      - If logical range of UE Context Information IE does not match with the one
        configured locally for this eNodeB.
      - If message was not not able to deliver right upto user when upper
        SAP is not found for this peer or in invalid state or memory loss.
    *  
    * @param[in] peerCb        The active peerCb pointer from this eNodeB for an
                               UE handover.
    * @param[in] ueCb          An UECb pointer from this eNodeB for an UE handover.
    * @param[in] evnt          The pointer with X2AP ASN PDU structure received at
                               this state for an UE handover to send to peer.
    * @param[in] hoMsgType     Basic Mobility management message(CZT_MSG_HO_REQ),
                               represented  by Trillium X2AP Message value.
    * @param[in] oldUEX2APIdVal Old UE X2AP ID Value present in the incoming 
                                Handover Request.
    *  
    * @return 
    * -# ROK on success. 
    * -# RFAILED if error encountered within the function.
    * 
    */
#ifdef ANSI
PRIVATE S16 czBmpHdlInHoReq
(
CzPeerCb   *peerCb,             
CzUeCb     *ueCb,
CztEvnt    *evnt,                 
U8         hoMsgType,
U16        oldUEX2APIdVal
)
#else /* ANSI */
PRIVATE S16 czBmpHdlInHoReq(peerCb, ueCb, evnt, hoMsgType, oldUEX2APIdVal)
CzPeerCb   *peerCb;             
CzUeCb     *ueCb;
CztEvnt    *evnt;                 
U8         hoMsgType;
U16        oldUEX2APIdVal;
#endif /* ANSI */
{
   U16 ueCbKeyPrefx = CZ_TGT;
   U32 ueCbKey = 0;
   /* cz001.101 : Memory leak fix. */
   CzIeInfo errIeInfo[CZ_MAX_IDX];
   /* cz001.301 : Removed Unused variable */

   S16 ret = ROK;

   CZ_TRC2(czBmpHdlInHoReq)

   CZ_SET_ZERO(errIeInfo, CZ_MAX_IDX * sizeof(CzIeInfo)); 

   CZ_BMP_LOG_ENTRY_IN_HDLR_INFO(peerCb, oldUEX2APIdVal, CZ_UNKNOWN_UE_X2AP_ID, 
                                 CZ_EVNT_HO_REQ);

   CZ_BMP_FILL_UE_INFO(oldUEX2APIdVal, CZ_UNKNOWN_UE_X2AP_ID);

    /* No need to check If Old UE X2AP ID is out of logical range */
    /* This is done by decoder */

/* cz001.301 : Removed the function czBmpIsHoReqValid
 * Reason: The function of the czBmpIsHoReqValid is mainly to 
 * validate the Intigratity and encryption Algorithm, These checks 
 * has to be done by the application not the X2AP. So as this is Redundant code.*/


   /**************************************************************/
   /* No existing ueCb present in the ueCb's list for this UE
     OR
     No errors encountered yet as of now from previous validations*/
   /**************************************************************/

   if(!ueCb) 
   {
      /*******************************************************/
      /* Retrieve the key for this message */
      /*******************************************************/
      CZ_BMP_GET_KEY(oldUEX2APIdVal, ueCbKeyPrefx, ueCbKey);

      /*******************************************************/
      /* Allocate a new UE CB for this message */
      /*******************************************************/
      ret = czNmmAddUeCb(peerCb, ueCbKey, &ueCb, oldUEX2APIdVal);
 
      if(ROK != ret)
      {
         CZ_DBG_FATAL((CZ_PRNT_BUF, "Failed to allocate UeCb. UE(%d:%d)\n ", \
                  oldUEX2APIdVal, CZ_UNKNOWN_UE_X2AP_ID));
         /* No logging error required NMM does that */ 
         /* Failure either memory loss or insertion failed */ 
         /* Inform back to peer */
         /* czUtlUpdInErrSts(peerCb, CZT_MSG_HO_REQ); */
         czBmpSndFailRspToPeer(peerCb, evnt, &czUeInfo, NULLP, 
         /* cz001.301: Fix for the build warnings */ 
#ifdef CZTV1
                               CZT_CAUSE_MISC,
#else
                               CAUSE_MISC,
#endif
                               CztCauseMiscom_interventionEnum,
                               CZT_ERRCAUSE_UE_CB_CREATE_FAILED);
         CZ_RETVALUE(RFAILED);
      }
   }
   /* All success in receiving this request */
   /* Increment the incoming successful counter for this message */
   /* czUtlUpdInSts(peerCb, CZT_MSG_HO_REQ); */

   /* Now change the state :) */
   ueCb->state = CZ_BMP_HO_PROGRESS;

   czUtlGetStream(peerCb, &(ueCb->streamId));

   CZ_BMP_LOG_IN_HDLR_INFO(peerCb, oldUEX2APIdVal, CZ_UNKNOWN_UE_X2AP_ID,
                           CZ_EVNT_HO_REQ);

   /*******************************************************/
   /* Invoke a certain function in UIM to send it up to user */
   /*******************************************************/
   ret = czUimSndBmpInd(peerCb, evnt);

   /*******************************************************/
   /* Message could not be delivered to user. 
    * Try informing the peer with HO Preparation Failure */
   /* May be invalid upper SAP */ 
   /* Message is not sent upto user Roll Back */
   /*******************************************************/
   if(ROK != ret)
   {
#ifndef ALIGN_64BIT
      CZ_DBG_FATAL((CZ_PRNT_BUF, "Indication of the incoming X2-Handover \
               Request failed from the peer(%ld) for UE(%d:%d)\n", 
               (peerCb->peerId), oldUEX2APIdVal, CZ_UNKNOWN_UE_X2AP_ID));
#else /* ALIGN_64BIT */
      CZ_DBG_FATAL((CZ_PRNT_BUF, "Indication of the incoming X2-Handover \
               Request failed from the peer(%d) for UE(%d:%d)\n", 
               (peerCb->peerId), oldUEX2APIdVal, CZ_UNKNOWN_UE_X2AP_ID));
#endif /* ALIGN_64BIT */
      CZ_BMP_LOG_IN_HDLR_ERR(peerCb, oldUEX2APIdVal, CZ_UNKNOWN_UE_X2AP_ID,
                             CZ_EVNT_HO_REQ, ret);

      /*******************************************************/
      /* Remove the entry of this UE from peerCb List */
      /*******************************************************/
      czNmmDelUeCb(ueCb);
      ueCb= NULLP;

      /* Inform back to peer */
      /* czUtlUpdInErrSts(peerCb, CZT_MSG_HO_REQ); */
      czBmpSndFailRspToPeer(peerCb, evnt, &czUeInfo, NULLP, 
      /* cz001.301: Fix for the build warnings */
#ifdef CZTV1
                            CZT_CAUSE_MISC,
#else
                            CAUSE_MISC,
#endif
                            CztCauseMiscom_interventionEnum,
                            CZT_ERRCAUSE_MSG_NOT_DELIVERED);
      CZ_RETVALUE(RFAILED);
   }

   CZ_BMP_LOG_EXIT_IN_HDLR_INFO(peerCb, oldUEX2APIdVal, CZ_UNKNOWN_UE_X2AP_ID, 
                                CZ_EVNT_HO_REQ);

   CZ_RETVALUE(ROK);

} /* End of function czBmpHdlInHoReq */


/**
    * @brief This function handles the X2AP Handover Request Acknowledge message
    * received from user , for an UE in active handover. 
    * 
    * @details It generates a local Error Indication back to user
      - If UECb is not found based on oldUEX2APIdVal.
      - If message was not not able to send to lower interface 
        due to error in lower interface SAP or association for this peer.
    *  
    * @param[in] peerCb     The active peerCb pointer from this eNodeB for an
                            UE handover.
    * @param[in] ueCb       An UECb pointer from this eNodeB for an UE handover.
    * @param[in] evnt       The pointer with X2AP ASN PDU structure received at
                            this state for an UE handover to send to peer.
    * @param[in] hoMsgType     Basic Mobility management message(CZT_MSG_HO_ACK),
                               represented  by Trillium X2AP Message value.
    * @param[in] oldUEX2APIdVal Old UE X2AP ID Value present in the outgoing 
                                Handover Request Acknowledge.
    *  
    * @return 
    * -# ROK on success. 
    * -# RFAILED if error encountered within the function and 
       dependent functions.
    *
    */

#ifdef ANSI
PRIVATE S16 czBmpHdlOutHoAck
(
CzPeerCb   *peerCb,
CzUeCb     *ueCb,               
CztEvnt    *evnt, 
U8         hoMsgType,
U16        oldUEX2APIdVal
)
#else /* ANSI */
PRIVATE S16 czBmpHdlOutHoAck(peerCb, ueCb, evnt, hoMsgType, oldUEX2APIdVal)
CzPeerCb   *peerCb;
CzUeCb     *ueCb;            
CztEvnt    *evnt;                  
U8         hoMsgType;
U16        oldUEX2APIdVal;
#endif /* ANSI */
{
   CztUE_X2AP_ID *newUEX2APId = NULLP;
   U16 newUEX2APIdVal = CZ_UNKNOWN_UE_X2AP_ID;
   S16 ret;

   CZ_TRC2(czBmpHdlOutHoAck)

   CZ_BMP_LOG_ENTRY_OUT_HDLR_INFO(peerCb, oldUEX2APIdVal, newUEX2APIdVal, 
                                  CZ_EVNT_HO_REQ_ACK);

   czUtlGetIE(&(evnt->pdu), Cztid_New_eNB_UE_X2AP_ID, (TknU8 **)&newUEX2APId);

   /* New UE X2AP ID 's check for logical range is done by encoder */
   if((NULLP == newUEX2APId) || 
      (PRSNT_NODEF != newUEX2APId->pres))
   {
      if(newUEX2APId)
      { 
         /*kworks fix: type cast*/ 
         newUEX2APIdVal = (U16)newUEX2APId->val;
      }
#ifdef ALIGN_64BIT
      CZ_DBG_ERROR((CZ_PRNT_BUF, "Invalid new UE X2APId for an UE(%d:%d) when "\
                    "sending Handover Request Acknowledge to peer(%d)", \
                     oldUEX2APIdVal, newUEX2APIdVal, (peerCb->peerId)));
#else /* ALIGN_64BIT */
      CZ_DBG_ERROR((CZ_PRNT_BUF, "Invalid new UE X2APId for an UE(%d:%d) when "\
                    "sending Handover Request Acknowledge to peer(%ld)", \
                     oldUEX2APIdVal, newUEX2APIdVal, (peerCb->peerId)));
#endif /* ALIGN_64BIT */

      /* Inform back to user */
      /* Invoke a certain function in UIM to generate this */
      CZ_BMP_FILL_UIM_UE_ERR_INFO(peerCb, CZT_MSG_HO_ACK,
                                  oldUEX2APIdVal, newUEX2APIdVal);
      czBmpGenLclErrInd(peerCb, evnt, CZT_ERRTYPE_INV_SDU_PARAM,
                        CZT_ERRCAUSE_INV_NEW_UE_X2AP_ID);
      CZ_RETVALUE(RFAILED);
   }

   /* No Timer to send HO ACK */
   /*kworks fix:type cast*/
   ueCb->newUEX2APId = (U16) newUEX2APId->val;

   /* Finally - success; Now change the state */
   ueCb->state = CZ_BMP_HO_PREPARED;

   CZ_BMP_LOG_OUT_HDLR_INFO(peerCb, oldUEX2APIdVal, (ueCb->newUEX2APId), 
                           CZ_EVNT_HO_REQ_ACK,  ueCb->streamId);

   /*******************************************************/
   /* Invoke a certain function in Transport module to send it out to peer */
   /*******************************************************/
   ret = czTpmSndToLower(peerCb, &(evnt->pdu), ueCb->streamId);

    /* Message is not sent out Roll Back*/
   if(ROK != ret)
   {
#ifndef ALIGN_64BIT
      CZ_DBG_FATAL((CZ_PRNT_BUF, "Transmission of the outcoming X2-Handover Ack\
               failed towards the peer(%ld) for UE(%d:%d)\n", (peerCb->peerId), 
               oldUEX2APIdVal, ueCb->newUEX2APId));
#else /* ALIGN_64BIT */
      CZ_DBG_FATAL((CZ_PRNT_BUF, "Transmission of the outcoming X2-Handover Ack\
               failed towards the peer(%d) for UE(%d:%d)\n", (peerCb->peerId), 
               oldUEX2APIdVal, ueCb->newUEX2APId));
#endif /* ALIGN_64BIT */
      ueCb->state = CZ_BMP_HO_PROGRESS;
      ueCb->newUEX2APId = CZ_UNKNOWN_UE_X2AP_ID;
      /*kworks fix: type cast*/
      newUEX2APIdVal = (U16) newUEX2APId->val;

      CZ_BMP_LOG_OUT_HDLR_ERR(peerCb, oldUEX2APIdVal, newUEX2APIdVal,
                              CZ_EVNT_HO_REQ_ACK, ret);

      /* Increment the outgoing error counter for this message */
      /* czUtlUpdOutErrSts(peerCb, CZT_MSG_HO_ACK); */

      /* Let us not remove this ueCb, user can send 
      * HO prep fail or HO req ACK again, unless local release */

      CZ_BMP_FILL_UIM_UE_ERR_INFO(peerCb, CZT_MSG_HO_ACK,
                                 oldUEX2APIdVal, (newUEX2APId->val));
      czBmpGenLclErrInd(peerCb, evnt, CZT_ERRTYPE_MSG_SEND_FAILED, (U8)ret); /* windows warning fixed by typecasting S16 to U8 */
   }
   else
   {
      CZ_BMP_LOG_EXIT_OUT_HDLR_INFO(peerCb, oldUEX2APIdVal, (ueCb->newUEX2APId),
                                    CZ_EVNT_HO_REQ_ACK, ueCb->streamId);
      /* Increment the outgoing counter for this message */
      /* czUtlUpdOutSts(peerCb, CZT_MSG_HO_ACK); */

      CZ_FREEEVNT(evnt); /* All Done in processing this message */
   }

   CZ_RETVALUE(ret);

} /* End of function czBmpHdlOutHoAck */


/**
    * @brief This function handles the X2AP Handover Request Acknowledge message
    * received from peer, for an UE in active handover. 
    * 
    * @details It generates an X2 Error Indication to the peer:
      - If UECb is not found based on oldUEX2APIdVal.
      - If message was not not able to deliver right upto user when upper
        SAP is not found for this peer or in invalid state or memory loss.

    * @param[in] peerCb     The active peerCb pointer from this eNodeB for an
                            UE handover.
    * @param[in] ueCb       An UECb pointer from this eNodeB for an UE handover.
    * @param[in] evnt       The pointer with X2AP ASN PDU structure received at
                            this state for an UE handover to send to peer.
    * @param[in] hoMsgType     Basic Mobility management message(CZT_MSG_HO_ACK),
                               represented  by Trillium X2AP Message value.
    * @param[in] oldUEX2APIdVal Old UE X2AP ID Value present in the incoming 
                                Handover Request Acknowledge.
  
    *  
    * @return 
    * -# ROK on success. 
    * -# RFAILED if error encountered within the function.
    *
    */

#ifdef ANSI
PRIVATE S16 czBmpHdlInHoAck
(
CzPeerCb   *peerCb,               
CzUeCb     *ueCb,               
CztEvnt    *evnt, 
U8         hoMsgType,
U16        oldUEX2APIdVal
)
#else /* ANSI */
PRIVATE S16 czBmpHdlInHoAck(peerCb, ueCb, evnt, hoMsgType, oldUEX2APIdVal)
CzPeerCb   *peerCb;              
CzUeCb     *ueCb;
CztEvnt    *evnt;                  
U8         hoMsgType;
U16        oldUEX2APIdVal;
#endif /* ANSI */
{
   CztUE_X2AP_ID *newUEX2APId = NULLP;
   U16 newUEX2APIdVal = CZ_UNKNOWN_UE_X2AP_ID;
   S16 ret;

   CZ_TRC2(czBmpHdlInHoAck)


   czUtlGetIE(&(evnt->pdu), Cztid_New_eNB_UE_X2AP_ID, (TknU8 **)&newUEX2APId);
   
   /*kworks fix : Added NULLP check*/
   if(newUEX2APId != NULLP)
   {
      newUEX2APIdVal = (U16) newUEX2APId->val;
   }
   CZ_BMP_FILL_UE_INFO(oldUEX2APIdVal, newUEX2APIdVal);

   /* New UE X2AP ID 's logical range check is done by decoder */

   CZ_BMP_LOG_ENTRY_IN_HDLR_INFO(peerCb, oldUEX2APIdVal, newUEX2APIdVal, 
                                 CZ_EVNT_HO_REQ_ACK);

   /*******************************************************/
   /* Stop the TRelocprep timer since this preparation is done */
   /*******************************************************/
   czStopTmr(CZ_TMR_TRELOCPREP, (PTR *)ueCb);

   /*******************************************************/
   /* Start the TX2Relocoverall timer now to track the completion of handover */
   /*******************************************************/
   ret = czStartTmr(CZ_TMR_TX2RELOCOVERALL, czCb.nodeCb.protCfg.tX2RELOCOverall,
              (PTR *)ueCb);
#if (ERRCLASS & ERRCLS_INT_PAR)
   if(ret != ROK)
   {
      CZ_DBG_ERROR((CZ_PRNT_BUF, 
               "Could not start the TX2Relocoverall timer.\n"));
      CZ_SEND_LCL_GEN_ERR(peerCb, CZT_ERRTYPE_SYS,
            CZT_ERRCAUSE_TMR_ERR, CZT_MSG_HO_ACK); 
   }
#endif /* (ERRCLASS & ERRCLS_INT_PAR) */

   /* Change the state */
   ueCb->state = CZ_BMP_HO_PREPARED;

   ueCb->newUEX2APId = newUEX2APIdVal;

   /* Increment the incoming successful counter for this message */
   /* czUtlUpdInSts(peerCb, CZT_MSG_HO_ACK); */

   CZ_BMP_LOG_IN_HDLR_INFO(peerCb, oldUEX2APIdVal, newUEX2APIdVal,
                           CZ_EVNT_HO_REQ_ACK);

   /*******************************************************/
   /* Invoke a certain function in UI module to send it up to user */
   /*******************************************************/
   ret = czUimSndBmpCfm(peerCb, evnt);

   /* Message is not sent upto user - Roll Back */
   /* May be because of memory loss */
   if(ROK != ret)
   {
#ifndef ALIGN_64BIT
      CZ_DBG_FATAL((CZ_PRNT_BUF, "Indication of the incoming X2-Handover Ack \
               failed from the peer(%ld) for UE(%d:%d)\n", (peerCb->peerId), 
               oldUEX2APIdVal, newUEX2APIdVal));
#else /* ALIGN_64BIT */
      CZ_DBG_FATAL((CZ_PRNT_BUF, "Indication of the incoming X2-Handover Ack \
               failed from the peer(%d) for UE(%d:%d)\n", (peerCb->peerId), 
               oldUEX2APIdVal, newUEX2APIdVal));
#endif /* ALIGN_64BIT */

      ueCb->newUEX2APId = CZ_UNKNOWN_UE_X2AP_ID;
      ueCb->state = CZ_BMP_HO_PROGRESS;

      /*******************************************************/
      /* Stop the TX2Relocoverall timer since this preparation is incomplete */
      /*******************************************************/
      czStopTmr(CZ_TMR_TX2RELOCOVERALL, (PTR *)ueCb);

      /* Start TRelocprep timer for this UE to track cancel again */
      ret = czStartTmr(CZ_TMR_TRELOCPREP, czCb.nodeCb.protCfg.tRELOCprep,
            (PTR *) ueCb);
#if (ERRCLASS & ERRCLS_INT_PAR)
      if(ret != ROK)
      {
         CZ_DBG_ERROR((CZ_PRNT_BUF, 
                  "Could not start the TX2Relocoverall timer.\n"));
         CZ_SEND_LCL_GEN_ERR(peerCb, CZT_ERRTYPE_SYS,
               CZT_ERRCAUSE_TMR_ERR, CZT_MSG_HO_ACK); 
      }
#endif /* (ERRCLASS & ERRCLS_INT_PAR) */

      CZ_BMP_LOG_IN_HDLR_ERR(peerCb, oldUEX2APIdVal, newUEX2APIdVal,
                             CZ_EVNT_HO_REQ_ACK, ret);

      czBmpSndErrIndToPeer(peerCb, evnt, &czUeInfo, 
                           CZT_MSG_HO_ACK, 
      /* cz001.301: Fix for the build warnings */
#ifdef CZTV1
                           CZT_CAUSE_MISC,
#else 
                           CAUSE_MISC,
#endif
                           CztCauseMiscom_interventionEnum,
                           CZT_ERRCAUSE_MSG_NOT_DELIVERED);
      CZ_RETVALUE(ret);
   }

   CZ_BMP_LOG_EXIT_IN_HDLR_INFO(peerCb, oldUEX2APIdVal, newUEX2APIdVal, 
                                CZ_EVNT_HO_REQ_ACK);
   CZ_RETVALUE(ROK); /* All Done */

} /* End of function czBmpHdlInHoAck */



/**
    * @brief This function handles the X2AP Handover Cancel
    * received from user, for an UE in active handover to send to peer.
    * 
    * @details It generates a local Error Indication back to user
      - If UECb is not found based on oldUEX2APIdVal.
      - If Cancel has new UE X2AP ID and it does not match with the one present in 
        existing UECb for this UE.
      - If message was not able to send to lower interface 
        due to error in lower interface SAP or association for this peer.

    * @param[in] peerCb      The active peerCb from this eNodeB involved for an
                             UE handover.
    * @param[in] ueCb        An UECb pointer from this eNodeB for an UE handover.
    * @param[in] evnt        The pointer with X2AP ASN PDU structure received
                             from user for an UE handover.
    * @param[in] hoMsgType   Basic Mobility management message, represented
                             by Trillium X2AP Message value. - CZT_MSG_HO_CANCEL.
    * @param[in] oldUEX2APIdVal The old UE X2AP ID present in the received 
                                mobility message.
    * @note: For a list of Trillium X2AP message representation refer to czt.h.
    *  
    * @return 
    * -# ROK on success. 
    * -# RFAILED if error encountered within the function.
    * 
    */


#ifdef ANSI
PRIVATE S16 czBmpHdlOutHoCancel
(
CzPeerCb   *peerCb,
CzUeCb     *ueCb,               
CztEvnt    *evnt,                    
U8         hoMsgType,
U16        oldUEX2APIdVal
)
#else /* ANSI */
PRIVATE S16 czBmpHdlOutHoCancel(peerCb, ueCb, evnt, hoMsgType, oldUEX2APIdVal)
CzPeerCb   *peerCb;               
CzUeCb     *ueCb;               
CztEvnt    *evnt;                   
U8         hoMsgType;
U16        oldUEX2APIdVal;
#endif /* ANSI */
{
   CztUE_X2AP_ID *newUEX2APId = NULLP;
   U16 newUEX2APIdVal = CZ_UNKNOWN_UE_X2AP_ID;
   CztStrmId streamId;
   S16 ret = ROK;

   CZ_TRC2(czBmpHdlOutHoCancel)

   CZ_BMP_LOG_ENTRY_OUT_HDLR_INFO(peerCb, oldUEX2APIdVal, newUEX2APIdVal, 
                                  CZ_EVNT_HO_CANCEL);


   czUtlGetIE(&(evnt->pdu), Cztid_New_eNB_UE_X2AP_ID, (TknU8 **)&newUEX2APId);

   /* New UE X2AP Id is not mandatory in Cancel. But if present we should
       make sure that cancel is sent on this UE only */
   if((NULLP != newUEX2APId) && (PRSNT_NODEF == newUEX2APId->pres)) 
   {
      /*kworks fix: type cast*/ 
      newUEX2APIdVal = (U16)newUEX2APId->val;
      if(newUEX2APIdVal != ueCb->newUEX2APId)
      {
#ifdef ALIGN_64BIT
         CZ_DBG_ERROR((CZ_PRNT_BUF, "Invalid newUEX2APId for an UE(%d:%d)"\
                      "when sending Handover Cancel to peer(%d)", \
                       oldUEX2APIdVal, newUEX2APIdVal, (peerCb->peerId)));
#else /* ALIGN_64BIT */
         CZ_DBG_ERROR((CZ_PRNT_BUF, "Invalid newUEX2APId for an UE(%d:%d)"\
                      "when sending Handover Cancel to peer(%ld)", \
                       oldUEX2APIdVal, newUEX2APIdVal, (peerCb->peerId)));
#endif /* ALIGN_64BIT */

         /* Increment the outgoing error counter for this message */
         /* czUtlUpdOutErrSts(peerCb, CZT_MSG_HO_CANCEL); */

         /* Inform back to user */
         /* Invoke a certain function in UIM to generate this */
         CZ_BMP_FILL_UIM_UE_ERR_INFO(peerCb, CZT_MSG_HO_CANCEL,
                                 oldUEX2APIdVal, newUEX2APIdVal);
         czBmpGenLclErrInd(peerCb, evnt, CZT_ERRTYPE_INV_SDU_PARAM,
                           CZT_ERRCAUSE_INV_NEW_UE_X2AP_ID);

         CZ_RETVALUE(RFAILED);
      }
   }

   /*******************************************************/
   /* Though stack itself can generate the HO Cancel due to timer(s)
    * expiry, this is a function invoked whenever HO Cancel is initiated
    * by user to send out */
   /* Hence try stopping relevant timers(Void *)ueCb.
      Though these timers are mutually exclusive, it is good to
      perform the two operations within same function. */ 
   /*******************************************************/

   czStopUeTmrs(ueCb);

   streamId = ueCb->streamId;
 
   /* No explicit state change required, as we are deleting the UECb itself */
   /*******************************************************/
   /* Delete this ueCb as HO cancel is generated */
   /*******************************************************/
   czNmmDelUeCb(ueCb);
   ueCb= NULLP;

   CZ_BMP_LOG_OUT_HDLR_INFO(peerCb, oldUEX2APIdVal, newUEX2APIdVal, 
                            CZ_EVNT_HO_CANCEL, streamId);

   /*******************************************************/
   /* Invoke a certain function in Transport module to send it out to peer */
   /*******************************************************/
   ret = czTpmSndToLower(peerCb, &(evnt->pdu), streamId);

   if(ROK != ret)
   {
#ifndef ALIGN_64BIT
      CZ_DBG_FATAL((CZ_PRNT_BUF, "Transmission of the outcoming X2-Handover \
               Cancel failed towards the peer(%ld) for UE(%d:%d)\n", 
               (peerCb->peerId), oldUEX2APIdVal, newUEX2APIdVal));
#else /* ALIGN_64BIT */
      CZ_DBG_FATAL((CZ_PRNT_BUF, "Transmission of the outcoming X2-Handover \
               Cancel failed towards the peer(%d) for UE(%d:%d)\n", 
               (peerCb->peerId), oldUEX2APIdVal, newUEX2APIdVal));
#endif /* ALIGN_64BIT */
      CZ_BMP_LOG_OUT_HDLR_ERR(peerCb, oldUEX2APIdVal, newUEX2APIdVal,
                              CZ_EVNT_HO_CANCEL, ret);

      /* Increment the outgoing error counter for this message */
      /* czUtlUpdOutErrSts(peerCb, CZT_MSG_HO_CANCEL); */

      /* Inform back to user */
      /* Invoke a certain function in UIM to generate this */
      CZ_BMP_FILL_UIM_UE_ERR_INFO(peerCb, CZT_MSG_HO_CANCEL,
                                 oldUEX2APIdVal, newUEX2APIdVal);
      czBmpGenLclErrInd(peerCb, evnt, CZT_ERRTYPE_MSG_SEND_FAILED, (U8)ret); /* windows warning fixed by typecasting S16 to U8 */
   }
   else
   {
      CZ_BMP_LOG_EXIT_OUT_HDLR_INFO(peerCb, oldUEX2APIdVal, newUEX2APIdVal, 
                      CZ_EVNT_HO_CANCEL, streamId);

      /* Increment the outgoing success counter for this message */
      /* czUtlUpdOutSts(peerCb, CZT_MSG_HO_CANCEL); */

      CZ_FREEEVNT(evnt); /* All Done in processing this message */
   }

   CZ_RETVALUE(ret);

} /* End of function czBmpHdlOutHoCancel */


/**
    * @brief This function handles the X2AP Handover Cancel
    * received from peer, for an UE in active handover. 
    * 
    * @details It generates an X2 Error Indication to the peer:
      - If UECb is not found based on oldUEX2APIdVal.
      - If Cancel has new UE X2AP ID and it does not match with the one present in 
        existing UECb for this UE.
      - If message was not not able to deliver right upto user when upper
        SAP is not found for this peer or in invalid state or memory loss.

    * @param[in] peerCb      The active peerCb from this eNodeB involved for an
                             UE handover.
    * @param[in] ueCb        An UECb pointer from this eNodeB for an UE handover.
    * @param[in] evnt        The pointer with X2AP ASN PDU structure received
                             from peer for an UE handover.
    * @param[in] hoMsgType   Basic Mobility management message, represented
                             by Trillium X2AP Message value. - CZT_MSG_HO_CANCEL.
    * @param[in] oldUEX2APIdVal The old UE X2AP ID present in the received 
                                mobility message.
    * @note: For a list of Trillium X2AP message representation refer to czt.h.
    *  
    * @return 
    * -# ROK on success. 
    * -# RFAILED if error encountered within the function.
    * 
  */


#ifdef ANSI
PRIVATE S16 czBmpHdlInHoCancel
(
CzPeerCb   *peerCb,
CzUeCb     *ueCb,               
CztEvnt    *evnt,                    
U8         hoMsgType,
U16        oldUEX2APIdVal
)
#else /* ANSI */
PRIVATE S16 czBmpHdlInHoCancel(peerCb, ueCb, evnt, hoMsgType, oldUEX2APIdVal)
CzPeerCb   *peerCb;               
CzUeCb     *ueCb;               
CztEvnt    *evnt;                   
U8         hoMsgType;
U16        oldUEX2APIdVal;
#endif /* ANSI */
{
   U16 newUEX2APIdVal = CZ_UNKNOWN_UE_X2AP_ID;
   CztUE_X2AP_ID *newUEX2APId = NULLP;
   S16 ret;

   CZ_TRC2(czBmpHdlInHoCancel)

   CZ_BMP_LOG_ENTRY_IN_HDLR_INFO(peerCb, oldUEX2APIdVal, newUEX2APIdVal, 
                                 CZ_EVNT_HO_CANCEL);

   CZ_BMP_FILL_UE_INFO(oldUEX2APIdVal, newUEX2APIdVal);


   czUtlGetIE(&(evnt->pdu), Cztid_New_eNB_UE_X2AP_ID, 
         (TknU8 **)&newUEX2APId);

   /*******************************************************/
   /* New UE X2AP Id is not mandatory in Cancel. But if present we should
      make sure that cancel is received on this UE only. */
   /*******************************************************/
   if((NULLP != newUEX2APId) && (newUEX2APId->val != ueCb->newUEX2APId))
   {
#ifdef ALIGN_64BIT
      CZ_DBG_ERROR((CZ_PRNT_BUF, "Invalid newUEX2APId for an UE(%d:%d)"\
               "when received Handover Cancel from peer(%d)", \
               oldUEX2APIdVal, newUEX2APIdVal, peerCb->peerId));
#else /* ALIGN_64BIT */
      CZ_DBG_ERROR((CZ_PRNT_BUF, "Invalid newUEX2APId for an UE(%d:%d)"\
               "when received Handover Cancel from peer(%ld)", \
               oldUEX2APIdVal, newUEX2APIdVal, peerCb->peerId));
#endif
      /*kworks fix: type cast*/ 
      czUeInfo.newUEX2APId = (U16) newUEX2APId->val;
      czBmpSndErrIndToPeer(peerCb, evnt, &czUeInfo, 
            CZT_MSG_HO_CANCEL, 
            CAUSE_RADIONW,
            CztCauseRadioNwunknown_new_eNB_UE_X2AP_IDEnum,
            CZT_ERRCAUSE_INV_NEW_UE_X2AP_ID);
      CZ_RETVALUE(RFAILED);
   }


   /* No explicit state change required, as we are deleting the UECb itself */
   /* No timers for an incoming cancel */

   /*******************************************************/
   /* Remove the entry of this UE from peerCb List */
   /*******************************************************/
   czNmmDelUeCb(ueCb);
   ueCb= NULLP;

   /* Increment the incoming successful counter for this message */
   /* czUtlUpdInSts(peerCb, CZT_MSG_HO_CANCEL); */

   CZ_BMP_LOG_IN_HDLR_INFO(peerCb, oldUEX2APIdVal, (czUeInfo.newUEX2APId),
                           CZ_EVNT_HO_CANCEL);

   /*******************************************************/
   /* Invoke a certain function in UIM to send it up to user */
   /*******************************************************/
   ret = czUimSndBmpInd(peerCb, evnt);

   /* There is no roll back check required, as UECb is already deleted due to 
    * receipt of HO Cancel. But help peer aware of it not delivered to user.*/
   if(ROK != ret)
   {
#ifndef ALIGN_64BIT
      CZ_DBG_FATAL((CZ_PRNT_BUF, "Indication of the incoming X2-Handover Cancel\
               failed from the peer(%ld) for UE(%d:%d)\n", (peerCb->peerId), 
               oldUEX2APIdVal, czUeInfo.newUEX2APId));
#else /* ALIGN_64BIT */
      CZ_DBG_FATAL((CZ_PRNT_BUF, "Indication of the incoming X2-Handover Cancel\
               failed from the peer(%d) for UE(%d:%d)\n", (peerCb->peerId), 
               oldUEX2APIdVal, czUeInfo.newUEX2APId));
#endif /* ALIGN_64BIT */
      CZ_BMP_LOG_IN_HDLR_ERR(peerCb, oldUEX2APIdVal, (czUeInfo.newUEX2APId),
                             CZ_EVNT_HO_CANCEL, ret);

      /*******************************************************/
      /* Inform back to peer */
      /* Try generating ErrIndication to peer with
         appropriate cause */
      /* Invoke a certain function in UTL to generate this */
      /*******************************************************/
      czBmpSndErrIndToPeer(peerCb, evnt, &czUeInfo, 
                           CZT_MSG_HO_CANCEL, 
      /* cz001.301: Fix for the build warnings */
#ifdef CZTV1
                           CZT_CAUSE_MISC,
#else
                           CAUSE_MISC,
#endif
                           CztCauseMiscom_interventionEnum,
                           CZT_ERRCAUSE_MSG_NOT_DELIVERED);
      CZ_RETVALUE(ret);
   }

   CZ_BMP_LOG_EXIT_IN_HDLR_INFO(peerCb, oldUEX2APIdVal, (czUeInfo.newUEX2APId), 
                                CZ_EVNT_HO_CANCEL);

   CZ_RETVALUE(ROK);

} /* End of function czBmpHdlInHoCancel */


/**
    * @brief This function handles the X2AP Handover Preparation Failure
    * received from user, for an UE in active handover to send to peer.
    * 
    * @details It generates a local Error Indication back to user
      - If UECb is not found based on oldUEX2APIdVal.
      - If message was not not able to send to lower interface 
        due to error in lower interface SAP or association for this peer.

    * @param[in] peerCb      The active peerCb from this eNodeB involved for an
                             UE handover.
    * @param[in] ueCb        An UECb pointer from this eNodeB for an UE handover.
    * @param[in] evnt        The pointer with X2AP ASN PDU structure received
                             from user for an UE handover.
    * @param[in] hoMsgType   Basic Mobility management message, represented
                             by Trillium X2AP Message value. - CZT_MSG_HO_FAIL.
    * @param[in] oldUEX2APIdVal The old UE X2AP ID present in the received 
                                mobility message.
    * @note: For a list of Trillium X2AP message representation, refer to czt.h.
    *  
    * @return 
    * -# ROK on success. 
    * -# RFAILED if error encountered within the function.
    * 
    */

#ifdef ANSI
PRIVATE S16 czBmpHdlOutHoFail
(
CzPeerCb   *peerCb,
CzUeCb     *ueCb,               
CztEvnt    *evnt,                  
U8         hoMsgType,
U16        oldUEX2APIdVal
)
#else /* ANSI */
PRIVATE S16 czBmpHdlOutHoFail(peerCb, ueCb, evnt, hoMsgType, oldUEX2APIdVal)
CzPeerCb   *peerCb;
CzUeCb     *ueCb;            
CztEvnt    *evnt;                  
U8         hoMsgType;
U16        oldUEX2APIdVal;
#endif /* ANSI */
{
   CztStrmId streamId;
   S16 ret = ROK;

   CZ_TRC2(czBmpHdlOutHoFail)

   CZ_BMP_LOG_ENTRY_OUT_HDLR_INFO(peerCb, oldUEX2APIdVal, CZ_UNKNOWN_UE_X2AP_ID,
                                  CZ_EVNT_HO_PREP_FAIL);
 
   streamId = ueCb->streamId;

   /* No explicit state change required, as we are deleting the UECb itself */
   /*******************************************************/
   /* Delete this ueCb as HO Preparation Failure is generated */
   /*******************************************************/
   czNmmDelUeCb(ueCb);
   ueCb= NULLP;

   CZ_BMP_LOG_OUT_HDLR_INFO(peerCb, oldUEX2APIdVal, CZ_UNKNOWN_UE_X2AP_ID, 
                            CZ_EVNT_HO_PREP_FAIL, streamId);

   /*******************************************************/
   /* Invoke a certain function in Transport module to send it out to peer */
   /*******************************************************/
   ret = czTpmSndToLower(peerCb, &(evnt->pdu), streamId);

   /* No check of RFAILED here, as no explicit roll back required */
   /* Don not remove this ueCb with failure, user can try sending HO 
      Preparation Failure again, unless local release */

   if(ROK != ret)
   {
#ifndef ALIGN_64BIT
      CZ_DBG_FATAL((CZ_PRNT_BUF, "Transmission of the outcoming X2-Handover \
               Failure failed towards the peer(%ld) for UE(%d:%d)\n", 
               (peerCb->peerId), oldUEX2APIdVal, CZ_UNKNOWN_UE_X2AP_ID));
#else /* ALIGN_64BIT */
      CZ_DBG_FATAL((CZ_PRNT_BUF, "Transmission of the outcoming X2-Handover \
               Failure failed towards the peer(%d) for UE(%d:%d)\n", 
               (peerCb->peerId), oldUEX2APIdVal, CZ_UNKNOWN_UE_X2AP_ID));
#endif /* ALIGN_64BIT */
      CZ_BMP_LOG_OUT_HDLR_ERR(peerCb, oldUEX2APIdVal, CZ_UNKNOWN_UE_X2AP_ID,
                              CZ_EVNT_HO_PREP_FAIL, ret);

      /* Increment the outgoing error counter for this message */
      /* czUtlUpdOutErrSts(peerCb, CZT_MSG_HO_FAIL); */

      /* Inform back to user */
      /* Invoke a certain function in UIM to generate this */
      CZ_BMP_FILL_UIM_UE_ERR_INFO(peerCb, CZT_MSG_HO_FAIL,
                                 oldUEX2APIdVal, CZ_UNKNOWN_UE_X2AP_ID);
      czBmpGenLclErrInd(peerCb, evnt, CZT_ERRTYPE_MSG_SEND_FAILED, (U8)ret); /* windows warning fixed by typecasting from S16 to U8 */
   }
   else
   {
      CZ_BMP_LOG_EXIT_OUT_HDLR_INFO(peerCb, oldUEX2APIdVal,
                                    CZ_UNKNOWN_UE_X2AP_ID, 
                                    CZ_EVNT_HO_PREP_FAIL, streamId);

      /* Increment the outgoing success counter for this message */
      /* czUtlUpdOutSts(peerCb, CZT_MSG_HO_FAIL); */

      CZ_FREEEVNT(evnt); /* All Done in processing this message */
   }

   CZ_RETVALUE(ret);

} /* End of function czBmpHdlOutHoFail */



/**
    * @brief This function handles the X2AP Handover Preparation Failure
    * received from peer, for an UE in active handover. 
    * 
    * @details It generates an X2 Error Indication to the peer:
      - If UECb is not found based on oldUEX2APIdVal.
      - If message was not not able to deliver right upto user when upper
        SAP is not found for this peer or in invalid state or memory loss.

    * @param[in] peerCb      The active peerCb from this eNodeB involved for an
                             UE handover.
    * @param[in] ueCb        An UECb pointer from this eNodeB for an UE handover.
    * @param[in] evnt        The pointer with X2AP ASN PDU structure received
                             from peer for an UE handover.
    * @param[in] hoMsgType   Basic Mobility management message, represented
                             by Trillium X2AP Message value. - CZT_MSG_HO_FAIL.
    * @param[in] oldUEX2APIdVal The old UE X2AP ID present in the received 
                                mobility message.
    * @note: For a list of Trillium X2AP message representation, refer to czt.h.
    *  
    * @return 
    * -# ROK on success. 
    * -# RFAILED if error encountered within the function.
    * 
  */

#ifdef ANSI
PRIVATE S16 czBmpHdlInHoFail
(
CzPeerCb   *peerCb,
CzUeCb     *ueCb,               
CztEvnt    *evnt,                  
U8         hoMsgType,
U16        oldUEX2APIdVal
)
#else /* ANSI */
PRIVATE S16 czBmpHdlInHoFail(peerCb, ueCb, evnt, hoMsgType, oldUEX2APIdVal)
CzPeerCb   *peerCb;
CzUeCb     *ueCb;            
CztEvnt    *evnt;                  
U8         hoMsgType;
U16        oldUEX2APIdVal;
#endif /* ANSI */
{
   S16 ret;
 
   CZ_TRC2(czBmpHdlInHoFail)

   CZ_BMP_FILL_UE_INFO(oldUEX2APIdVal, CZ_UNKNOWN_UE_X2AP_ID);

   /* Valid UECb present */
   /*******************************************************/
   /* Stop the TRelocprep timer since this preparation is incomplete
    * due to failure */
   /*******************************************************/
   czStopTmr(CZ_TMR_TRELOCPREP, (PTR *)ueCb);

   /* No need to start TX2Relocoverall Timer, as this preparation is
      incomplete */

   /* No explicit state change required, as the UECb itself */
   /*******************************************************/
   /* Delete this ueCb as HO Preparation Failure is received */
   /*******************************************************/
   czNmmDelUeCb(ueCb);
   ueCb= NULLP;

   /* Increment the incoming counter for this message */
   /* czUtlUpdInSts(peerCb, CZT_MSG_HO_FAIL); */

   CZ_BMP_LOG_IN_HDLR_INFO(peerCb, oldUEX2APIdVal, CZ_UNKNOWN_UE_X2AP_ID,
                           CZ_EVNT_HO_PREP_FAIL);

   /*******************************************************/
   /* Invoke a certain function in UI module to send it up to user */
   /*******************************************************/
   ret = czUimSndBmpCfm(peerCb, evnt);

   /* There is no roll back check required, as UECb is already deleted due to 
    * receipt of HO Preparation Failure.But help peer aware of it not delivered
      to user.*/
   if(ROK != ret)
   {
#ifndef ALIGN_64BIT
      CZ_DBG_FATAL((CZ_PRNT_BUF,"Indication of the incoming X2-Handover Failure\
               failed from the peer(%ld) for UE(%d:%d)\n", (peerCb->peerId), 
               oldUEX2APIdVal, CZ_UNKNOWN_UE_X2AP_ID));
#else /* ALIGN_64BIT */
      CZ_DBG_FATAL((CZ_PRNT_BUF,"Indication of the incoming X2-Handover Failure\
               failed from the peer(%d) for UE(%d:%d)\n", (peerCb->peerId), 
               oldUEX2APIdVal, CZ_UNKNOWN_UE_X2AP_ID));
#endif /* ALIGN_64BIT */
      CZ_BMP_LOG_IN_HDLR_ERR(peerCb, oldUEX2APIdVal, CZ_UNKNOWN_UE_X2AP_ID,
                             CZ_EVNT_HO_PREP_FAIL, ret);
      /*******************************************************/
      /* Inform back to peer */
      /* Try generating ErrIndication to peer with
         appropriate cause */
      /* Invoke a certain function in UTL to generate this */
      /*******************************************************/
      czBmpSndErrIndToPeer(peerCb, evnt, &czUeInfo, 
                           CZT_MSG_HO_FAIL, 
      /* cz001.301: Fix for the build warnings */
#ifdef CZTV1
                           CZT_CAUSE_MISC,
#else
                           CAUSE_MISC,
#endif
                           CztCauseMiscom_interventionEnum,
                           CZT_ERRCAUSE_MSG_NOT_DELIVERED);
      CZ_RETVALUE(ret);
   }

   CZ_BMP_LOG_EXIT_IN_HDLR_INFO(peerCb, oldUEX2APIdVal, CZ_UNKNOWN_UE_X2AP_ID, 
                                CZ_EVNT_HO_PREP_FAIL);

   CZ_RETVALUE(ROK); /* All Done */

} /* End of function czBmpHdlInHoFail */



/**
    * @brief This function handles the X2AP SN Status Transfer
    * received from user, for an UE in active handover to send to peer.
    * 
    * @details It generates a local Error Indication back to user
      - If UECb is not found based on oldUEX2APIdVal.
      - If new UE X2AP ID being sent does not match with the one present in 
        existing UECb for this UE.
      - If message was not able to send to lower interface 
        due to error in lower interface SAP or association for this peer.

    * @param[in] peerCb      The active peerCb from this eNodeB involved for an
                             UE handover.
    * @param[in] ueCb        An UECb pointer from this eNodeB for an UE handover.
    * @param[in] evnt        The pointer with X2AP ASN PDU structure received
                             from user for an UE handover.
    * @param[in] hoMsgType   Basic Mobility management message, represented
                             by Trillium X2AP Message value. - CZT_MSG_SN_STATUS_TRFR.
    * @param[in] oldUEX2APIdVal The old UE X2AP ID present in the received 
                                mobility message.
    * @note: For a list of Trillium X2AP message representation, refer to czt.h.
    *  
    * @return 
    * -# ROK on success. 
    * -# RFAILED if error encountered within the function.
    * 
    */

#ifdef ANSI
PRIVATE S16 czBmpHdlOutSnStTrnsfr
(
CzPeerCb   *peerCb,
CzUeCb     *ueCb,               
CztEvnt    *evnt,                   
U8         hoMsgType,
U16        oldUEX2APIdVal
)
#else /* ANSI */
PRIVATE S16 czBmpHdlOutSnStTrnsfr(peerCb, ueCb, evnt, hoMsgType, oldUEX2APIdVal)
CzPeerCb   *peerCb;               
CzUeCb     *ueCb;               
CztEvnt    *evnt;                  
U8         hoMsgType;
U16        oldUEX2APIdVal;
#endif /* ANSI */
{
   CztUE_X2AP_ID *newUEX2APId = NULLP;
   U16 newUEX2APIdVal = CZ_UNKNOWN_UE_X2AP_ID;
   S16 ret = ROK;

   CZ_TRC2(czBmpHdlOutSnStTrnsfr)
  
   czUtlGetIE(&(evnt->pdu), Cztid_New_eNB_UE_X2AP_ID, (TknU8 **)&newUEX2APId);
   if((NULLP != newUEX2APId) && (PRSNT_NODEF == newUEX2APId->pres))
   {
      /*kworks fix: type cast*/
      newUEX2APIdVal = (U16) newUEX2APId->val;
   }

   CZ_BMP_LOG_ENTRY_OUT_HDLR_INFO(peerCb, oldUEX2APIdVal, newUEX2APIdVal, 
                                  CZ_EVNT_SN_STATUS_TRFR);
   
   /*******************************************************/
   /* New UE X2AP Id is mandatory in SN Status Transfer. 
      Make sure that it is sent on this UE only */
   /*******************************************************/
   if((CZ_UNKNOWN_UE_X2AP_ID == newUEX2APIdVal) || 
       (newUEX2APIdVal != ueCb->newUEX2APId))
   {
#ifdef ALIGN_64BIT
      CZ_DBG_ERROR((CZ_PRNT_BUF, "Invalid newUEX2APId for an UE(%d:%d)"\
                    "when sending SN Status Transfer to peer(%d)", \
                     oldUEX2APIdVal, newUEX2APIdVal, peerCb->peerId));
#else /* ALIGN_64BIT */
      CZ_DBG_ERROR((CZ_PRNT_BUF, "Invalid newUEX2APId for an UE(%d:%d)"\
                    "when sending SN Status Transfer to peer(%ld)", \
                     oldUEX2APIdVal, newUEX2APIdVal, peerCb->peerId));
#endif /* ALIGN_64BIT */

      /* Increment the outgoing error counter for this message */
      /* czUtlUpdOutErrSts(peerCb, CZT_MSG_SN_STATUS_TRFR); */

      /* Inform back to user in both the cases */
      /* Invoke a certain function in UIM to generate this */
      CZ_BMP_FILL_UIM_UE_ERR_INFO(peerCb, CZT_MSG_SN_STATUS_TRFR,
                                 oldUEX2APIdVal, newUEX2APIdVal);
      czBmpGenLclErrInd(peerCb, evnt, CZT_ERRTYPE_INV_SDU_PARAM,
                        CZT_ERRCAUSE_INV_NEW_UE_X2AP_ID);
      CZ_RETVALUE(RFAILED);
   }

   CZ_BMP_LOG_OUT_HDLR_INFO(peerCb, oldUEX2APIdVal, newUEX2APIdVal, 
                           CZ_EVNT_SN_STATUS_TRFR, ueCb->streamId);

   /*******************************************************/
   /* Invoke a certain function in Transport module to send it out to peer */
   /*******************************************************/
   ret = czTpmSndToLower(peerCb, &(evnt->pdu), ueCb->streamId);

   if(ROK != ret)
   {
#ifndef ALIGN_64BIT
      CZ_DBG_FATAL((CZ_PRNT_BUF, "Transmission of the outcoming X2-SN State \
               Transfer failed towards the peer(%ld) for UE(%d:%d)\n", 
               (peerCb->peerId), oldUEX2APIdVal, newUEX2APIdVal));
#else /* ALIGN_64BIT */
      CZ_DBG_FATAL((CZ_PRNT_BUF, "Transmission of the outcoming X2-SN State \
               Transfer failed towards the peer(%d) for UE(%d:%d)\n", 
               (peerCb->peerId), oldUEX2APIdVal, newUEX2APIdVal));
#endif /* ALIGN_64BIT */
      /*******************************************************/
      /* Let us not remove this ueCb, as user can send multiple
       * SN status transfer(s) again, unless HO Cancel / local release */
      /* No explicit state change required, as SN Status Transfer can be sent
         * multiple times in HO Prepared state */
      /*******************************************************/
      CZ_BMP_LOG_OUT_HDLR_ERR(peerCb, oldUEX2APIdVal, newUEX2APIdVal,
                              CZ_EVNT_SN_STATUS_TRFR, ret);

      /* Increment the outgoing error counter for this message */
      /* czUtlUpdOutErrSts(peerCb, CZT_MSG_SN_STATUS_TRFR); */

      /* Inform back to user */
      /* Invoke a certain function in UIM to generate this */
      CZ_BMP_FILL_UIM_UE_ERR_INFO(peerCb, CZT_MSG_SN_STATUS_TRFR,
                                 oldUEX2APIdVal, newUEX2APIdVal);
      czBmpGenLclErrInd(peerCb, evnt, CZT_ERRTYPE_MSG_SEND_FAILED, (U8)ret);/* windows warning fixed by typecasting from S16 to U8 */ 
   }
   else
   {
      CZ_BMP_LOG_EXIT_OUT_HDLR_INFO(peerCb, oldUEX2APIdVal, newUEX2APIdVal, 
                                    CZ_EVNT_SN_STATUS_TRFR, ueCb->streamId);

      /* Increment the outgoing counter for this message */
      /* czUtlUpdOutSts(peerCb, CZT_MSG_SN_STATUS_TRFR); */

      CZ_FREEEVNT(evnt); /* All Done in processing this message */
   }

   CZ_RETVALUE(ret);

} /* End of function czBmpHdlOutSnStTrnsfr */


 /**
    * @brief This function handles the X2AP SN Status Transfer message
    * received from peer, for an UE in active handover. 
    * 
    * @details It generates an X2 Error Indication to the peer:
      - If UECb is not found based on oldUEX2APIdVal.
      - If received new UE X2AP ID does not match with the one present in 
        existing UECb for this UE.
      - If message was not able to deliver right upto user when upper
        SAP is not found for this peer or in invalid state or memory loss.

    * @param[in] peerCb      The active peerCb from this eNodeB involved for an
                             UE handover.
    * @param[in] ueCb        An UECb pointer from this eNodeB for an UE handover.
    * @param[in] evnt        The pointer with X2AP ASN PDU structure received
                             from peer for an UE handover.
    * @param[in] hoMsgType   Basic Mobility management message, represented
                             by Trillium X2AP Message value. - CZT_MSG_SN_STATUS_TRFR.
    * @param[in] oldUEX2APIdVal The old UE X2AP ID present in the received 
                                mobility message.
    * @note: For a list of Trillium X2AP message representation, refer to czt.h.
    *  
    * @return 
    * -# ROK on success. 
    * -# RFAILED if error encountered within the function.
    * 
    */

#ifdef ANSI
PRIVATE S16 czBmpHdlInSnStTrnsfr
(
CzPeerCb   *peerCb,
CzUeCb     *ueCb,               
CztEvnt    *evnt,                  
U8         hoMsgType,
U16        oldUEX2APIdVal
)
#else /* ANSI */
PRIVATE S16 czBmpHdlInSnStTrnsfr(peerCb, ueCb, evnt, hoMsgType, oldUEX2APIdVal)
CzPeerCb   *peerCb;               
CzUeCb     *ueCb;               
CztEvnt    *evnt;                  
U8         hoMsgType;
U16        oldUEX2APIdVal;
#endif /* ANSI */
{
   CztUE_X2AP_ID *newUEX2APId = NULLP;
   U16 newUEX2APIdVal = CZ_UNKNOWN_UE_X2AP_ID;
   S16 ret;

   CZ_TRC2(czBmpHdlInSnStTrnsfr)

   CZ_BMP_FILL_UE_INFO(oldUEX2APIdVal, newUEX2APIdVal);

   czUtlGetIE(&(evnt->pdu), Cztid_New_eNB_UE_X2AP_ID, (TknU8 **)&newUEX2APId);
   if(NULLP != newUEX2APId)
   {
      /*kworks fix: type cast*/
      newUEX2APIdVal = (U16)newUEX2APId->val;
      czUeInfo.newUEX2APId = newUEX2APIdVal;
   }

   CZ_BMP_LOG_ENTRY_IN_HDLR_INFO(peerCb, oldUEX2APIdVal, newUEX2APIdVal, 
                                  CZ_EVNT_SN_STATUS_TRFR);

   /*******************************************************/
   /* New UE X2AP Id is mandatory in SN Status Transfer. 
     Make sure that it is sent on this UE only. Need not check for
    NULL, as itis dropped by decoder if not present.
    But, must make that this value is for this UE only. */
   /* Valid UECb present */
   /*******************************************************/
   if ((CZ_UNKNOWN_UE_X2AP_ID == newUEX2APIdVal) || 
            (newUEX2APIdVal != ueCb->newUEX2APId))
   {
#ifdef ALIGN_64BIT
      CZ_DBG_ERROR((CZ_PRNT_BUF, "Invalid newUEX2APId for an UE(%d:%d)"\
                    "when received SN Status Transfer from peer(%d)", \
                     oldUEX2APIdVal, newUEX2APIdVal, peerCb->peerId));
#else /* ALIGN_64BIT */
      CZ_DBG_ERROR((CZ_PRNT_BUF, "Invalid newUEX2APId for an UE(%d:%d)"\
                    "when received SN Status Transfer from peer(%ld)", \
                     oldUEX2APIdVal, newUEX2APIdVal, peerCb->peerId));
#endif /* ALIGN_64BIT */
      /*******************************************************/
      /* Inform back to peer */
      /* Try generating ErrIndication to peer with
         appropriate cause */
      /* Invoke a certain function in UTL to generate this */
      /*******************************************************/
      czBmpSndErrIndToPeer(peerCb, evnt, &czUeInfo, 
                           CZT_MSG_SN_STATUS_TRFR, 
                           CAUSE_RADIONW,
                           CztCauseRadioNwunknown_new_eNB_UE_X2AP_IDEnum,
                           CZT_ERRCAUSE_INV_NEW_UE_X2AP_ID);
      CZ_RETVALUE(RFAILED);
   }
   /* Increment the incoming counter for this message */
   /* czUtlUpdInSts(peerCb, CZT_MSG_SN_STATUS_TRFR); */

   /*******************************************************/
   /* No explicit state change required, as SN Status Transfer can be received
    * multiple times in HO Prepared state */
   /*******************************************************/

   CZ_BMP_LOG_IN_HDLR_INFO(peerCb, oldUEX2APIdVal, newUEX2APIdVal,
                           CZ_EVNT_SN_STATUS_TRFR);

   /*******************************************************/
   /* Invoke a certain function in UI module to send it up to user */
   /*******************************************************/
   ret = czUimSndBmpInd(peerCb, evnt);

   /* There is no roll back check required, as SN Status Transfer can be received
    * multiple times in HO prepared state. But help peer aware of it not delivered
      to user.*/
   if(ROK != ret)
   {
#ifndef ALIGN_64BIT
      CZ_DBG_FATAL((CZ_PRNT_BUF, "Indication of the incoming X2-SN Transfer \
               failed from the peer(%ld) for UE(%d:%d)\n", (peerCb->peerId), 
               oldUEX2APIdVal, newUEX2APIdVal));
#else /* ALIGN_64BIT */
      CZ_DBG_FATAL((CZ_PRNT_BUF, "Indication of the incoming X2-SN Transfer \
               failed from the peer(%d) for UE(%d:%d)\n", (peerCb->peerId), 
               oldUEX2APIdVal, newUEX2APIdVal));
#endif /* ALIGN_64BIT */
      CZ_BMP_LOG_IN_HDLR_ERR(peerCb, oldUEX2APIdVal, newUEX2APIdVal,
                             CZ_EVNT_SN_STATUS_TRFR, ret);

      /*******************************************************/
      /* Inform back to peer */
      /* Try generating ErrIndication to peer with
         appropriate cause */
      /* Invoke a certain function in UTL to generate this */
      /*******************************************************/
      czBmpSndErrIndToPeer(peerCb, evnt, &czUeInfo, 
                           CZT_MSG_SN_STATUS_TRFR, 
      /* cz001.301: Fix for the build warnings */
#ifdef CZTV1
                           CZT_CAUSE_MISC,
#else
                           CAUSE_MISC,
#endif
                           CztCauseMiscom_interventionEnum,
                           CZT_ERRCAUSE_MSG_NOT_DELIVERED);
      CZ_RETVALUE(ret);
   }

   CZ_BMP_LOG_EXIT_IN_HDLR_INFO(peerCb, oldUEX2APIdVal, newUEX2APIdVal, 
                                CZ_EVNT_SN_STATUS_TRFR);

   CZ_RETVALUE(ROK); /* All Done */

} /* End of function czBmpHdlInSnStTrnsfr */


 /**
    * @brief This function handles the X2AP UE Context Release message
    * received from user, for an UE in active handover to send to peer.
    * 
    * @details It generates a local Error Indication back to user
      - If UECb is not found based on oldUEX2APIdVal.
      - If new UE X2AP ID being sent does not match with the one present in 
        existing UECb for this UE.
      - If message is not sent to lower interface 
        due to error in lower interface SAP or association for this peer.

    * @param[in] peerCb      The active peerCb from this eNodeB involved for an
                             UE handover.
    * @param[in] ueCb        An UECb pointer from this eNodeB for an UE handover.
    * @param[in] evnt        The pointer with X2AP ASN PDU structure received
                             from user for an UE handover.
    * @param[in] hoMsgType   Basic Mobility management message, represented
                             by Trillium X2AP Message value. - CZT_MSG_UE_CXT_RELEASE.
    * @param[in] oldUEX2APIdVal The old UE X2AP ID present in the received 
                                mobility message.
    * @note: For a list of Trillium X2AP message representation, refer to czt.h.
    *  
    * @return 
    * -# ROK on success. 
    * -# RFAILED if error encountered within the function.
    * 
    */

#ifdef ANSI
PRIVATE S16 czBmpHdlOutUeCnxtRelease
(
CzPeerCb   *peerCb,
CzUeCb     *ueCb,               
CztEvnt    *evnt,                  
U8         hoMsgType,
U16        oldUEX2APIdVal
)
#else /* ANSI */
PRIVATE S16 czBmpHdlOutUeCnxtRelease(peerCb, ueCb, evnt, hoMsgType, 
                                     oldUEX2APIdVal)
CzPeerCb   *peerCb;
CzUeCb     *ueCb;            
CztEvnt    *evnt;                  
U8         hoMsgType;
U16        oldUEX2APIdVal;
#endif /* ANSI */
{
   CztUE_X2AP_ID *newUEX2APId = NULLP;
   U16 newUEX2APIdVal = CZ_UNKNOWN_UE_X2AP_ID;
   CztStrmId streamId;
   S16 ret = ROK;

   CZ_TRC2(czBmpHdlOutUeCnxtRelease)

   czUtlGetIE(&(evnt->pdu), Cztid_New_eNB_UE_X2AP_ID, (TknU8 **)&newUEX2APId);
   if((NULLP != newUEX2APId) && (PRSNT_NODEF == newUEX2APId->pres))
   {
      /*kworks fix: type cast*/
      newUEX2APIdVal = (U16) newUEX2APId->val;
   }

   CZ_BMP_LOG_ENTRY_OUT_HDLR_INFO(peerCb, oldUEX2APIdVal, newUEX2APIdVal, 
                                  CZ_EVNT_UE_CXT_RELEASE);

   /*******************************************************/
   /* New UE X2AP Id is mandatory in UE Context Release. 
    * Make sure that it is sent on this UE only */
   /*******************************************************/
   if ((CZ_UNKNOWN_UE_X2AP_ID == newUEX2APIdVal) || 
        (newUEX2APIdVal != ueCb->newUEX2APId))
   {
#ifdef ALIGN_64BIT
      CZ_DBG_ERROR((CZ_PRNT_BUF, "Invalid newUEX2APId for an UE(%d:%d)"\
                    "when sending UE Context Release to peer(%d)", \
                     oldUEX2APIdVal, newUEX2APIdVal, (peerCb->peerId)));
#else /* ALIGN_64BIT */
      CZ_DBG_ERROR((CZ_PRNT_BUF, "Invalid newUEX2APId for an UE(%d:%d)"\
                    "when sending UE Context Release to peer(%ld)", \
                     oldUEX2APIdVal, newUEX2APIdVal, (peerCb->peerId)));
#endif /* ALIGN_64BIT */

      /* Increment the outgoing error counter for this message */
      /* czUtlUpdOutErrSts(peerCb, CZT_MSG_UE_CXT_RELEASE); */

      /* Inform back to user in both the cases */
      /* Invoke a certain function in UIM to generate this */
      CZ_BMP_FILL_UIM_UE_ERR_INFO(peerCb, CZT_MSG_UE_CXT_RELEASE,
                                 oldUEX2APIdVal, newUEX2APIdVal);
      czBmpGenLclErrInd(peerCb, evnt, CZT_ERRTYPE_INV_SDU_PARAM,
                        CZT_ERRCAUSE_INV_NEW_UE_X2AP_ID);

      CZ_RETVALUE(RFAILED);
   }

   /*******************************************************/
   /* No Timer to send UE Context Release */
   /* No explicit state change required, as we are deleting the UECb itself */
   /* Delete this ueCb as UE Context Release is generated */
   /*******************************************************/
   streamId = ueCb->streamId;
   czNmmDelUeCb(ueCb);
   ueCb= NULLP;

   CZ_BMP_LOG_OUT_HDLR_INFO(peerCb, oldUEX2APIdVal, newUEX2APIdVal, 
                            CZ_EVNT_UE_CXT_RELEASE, streamId);

   /*******************************************************/
   /* Invoke a certain function in Transport module to send it out to peer */
   /*******************************************************/
   ret = czTpmSndToLower(peerCb, &(evnt->pdu), streamId);

   if(ROK != ret)
   {
#ifndef ALIGN_64BIT
      CZ_DBG_FATAL((CZ_PRNT_BUF, "Transmission of the outcoming X2-UE Context \
               Release failed towards the peer(%ld) for UE(%d:%d)\n", 
               (peerCb->peerId), oldUEX2APIdVal, newUEX2APIdVal));
#else /* ALIGN_64BIT */
      CZ_DBG_FATAL((CZ_PRNT_BUF, "Transmission of the outcoming X2-UE Context \
               Release failed towards the peer(%d) for UE(%d:%d)\n", 
               (peerCb->peerId), oldUEX2APIdVal, newUEX2APIdVal));
#endif /* ALIGN_64BIT */
      CZ_BMP_LOG_OUT_HDLR_ERR(peerCb, oldUEX2APIdVal, newUEX2APIdVal,
                              CZ_EVNT_UE_CXT_RELEASE, ret);

      /* Increment the outgoing error counter for this message */
      /* czUtlUpdOutErrSts(peerCb, CZT_MSG_UE_CXT_RELEASE); */

      /* Inform back to user */
      /* Invoke a certain function in UIM to generate this */
      CZ_BMP_FILL_UIM_UE_ERR_INFO(peerCb, CZT_MSG_UE_CXT_RELEASE,
                                 oldUEX2APIdVal, newUEX2APIdVal);
      czBmpGenLclErrInd(peerCb, evnt, CZT_ERRTYPE_MSG_SEND_FAILED, (U8)ret);/* windows warning fixed by typecasting from S16 to U8 */ 
   }
   else
   {
      CZ_BMP_LOG_EXIT_OUT_HDLR_INFO(peerCb, oldUEX2APIdVal, newUEX2APIdVal, 
                                    CZ_EVNT_UE_CXT_RELEASE, streamId);

      /* Increment the outgoing counter for this message */
      /* czUtlUpdOutSts(peerCb, CZT_MSG_UE_CXT_RELEASE); */

      CZ_FREEEVNT(evnt); /* All Done in processing this message */
   }

   CZ_RETVALUE(ret);

} /* End of function czBmpHdlOutUeCnxtRelease */



  /**
    * @brief This function handles the X2AP UE Context Release message
    * received from peer, for an UE in active handover. 
    * 
    * @details It generates an X2 Error Indication to the peer:
      - If UECb is not found based on oldUEX2APIdVal.
      - If received new UE X2AP ID does not match with the one present in 
        existing UECb for this UE.
      - If message was not not able to deliver right upto user when upper
        SAP is not found for this peer or in invalid state or memory loss.

    * @param[in] peerCb      The active peerCb from this eNodeB involved for an
                             UE handover.
    * @param[in] ueCb        An UECb pointer from this eNodeB for an UE handover.
    * @param[in] evnt        The pointer with X2AP ASN PDU structure received
                             from peer for an UE handover.
    * @param[in] hoMsgType   Basic Mobility management message, represented
                             by Trillium X2AP Message value. - CZT_MSG_UE_CXT_RELEASE.
    * @param[in] oldUEX2APIdVal The old UE X2AP ID present in the received 
                                mobility message.
    * @note: For a list of Trillium X2AP message representation refer to czt.h.
    *  
    * @return 
    * -# ROK on success. 
    * -# RFAILED if error encountered within the function.
    * 
    */

#ifdef ANSI
PRIVATE S16 czBmpHdlInUeCnxtRelease
(
CzPeerCb   *peerCb,
CzUeCb     *ueCb,               
CztEvnt    *evnt,                  
U8         hoMsgType,
U16        oldUEX2APIdVal
)
#else /* ANSI */
PRIVATE S16 czBmpHdlInUeCnxtRelease(peerCb, ueCb, evnt, hoMsgType, 
                                    oldUEX2APIdVal)
CzPeerCb   *peerCb;
CzUeCb     *ueCb;            
CztEvnt    *evnt;                  
U8         hoMsgType;
U16        oldUEX2APIdVal;
#endif /* ANSI */
{
   CztUE_X2AP_ID *newUEX2APId = NULLP;
   U16 newUEX2APIdVal = CZ_UNKNOWN_UE_X2AP_ID;
   S16 ret;

   CZ_TRC2(czBmpHdlInUeCnxtRelease)

   CZ_BMP_FILL_UE_INFO(oldUEX2APIdVal, newUEX2APIdVal);

   czUtlGetIE(&(evnt->pdu), Cztid_New_eNB_UE_X2AP_ID, (TknU8 **)&newUEX2APId);
   if(NULLP != newUEX2APId)
   {
      /*kworks fix: type cast*/
      newUEX2APIdVal = (U16)newUEX2APId->val;
      czUeInfo.newUEX2APId = newUEX2APIdVal;
   }

   CZ_BMP_LOG_ENTRY_IN_HDLR_INFO(peerCb, oldUEX2APIdVal, newUEX2APIdVal, 
                                  CZ_EVNT_UE_CXT_RELEASE);

   /*******************************************************/
   /* Valid UECb present. New UE X2AP Id is mandatory in UE Context Release. 
    *  Make sure that it is sent on this UE only. Need not check for
    *   NULL, as it would have been dropped by decoder if not present. */
   /*******************************************************/
   if((CZ_UNKNOWN_UE_X2AP_ID == newUEX2APIdVal) || 
           (newUEX2APIdVal != ueCb->newUEX2APId))
   {
#ifdef ALIGN_64BIT
      CZ_DBG_ERROR((CZ_PRNT_BUF, "Invalid newUEX2APId for an UE(%d:%d)"\
                      "when received UE Context Release from peer(%d)", \
                       oldUEX2APIdVal, newUEX2APIdVal, peerCb->peerId));
#else /* ALIGN_64BIT */
      CZ_DBG_ERROR((CZ_PRNT_BUF, "Invalid newUEX2APId for an UE(%d:%d)"\
                      "when received UE Context Release from peer(%ld)", \
                       oldUEX2APIdVal, newUEX2APIdVal, peerCb->peerId));
#endif /* ALIGN_64BIT */

      /*******************************************************/
      /* Inform back to peer */
      /* Try generating ErrIndication to peer with
         appropriate cause */
      /* Invoke a certain function in UTL to generate this */
      /*******************************************************/
      czBmpSndErrIndToPeer(peerCb, evnt, &czUeInfo, 
                           CZT_MSG_UE_CXT_RELEASE, 
                           CAUSE_RADIONW,
                           CztCauseRadioNwunknown_new_eNB_UE_X2AP_IDEnum,
                           CZT_ERRCAUSE_INV_NEW_UE_X2AP_ID);
      CZ_RETVALUE(RFAILED);
   }

   /*******************************************************/
   /* Stop the TX2RelocOverall timer since this handover is complete
   * due to UE Context Release */
   /*******************************************************/
   czStopTmr(CZ_TMR_TX2RELOCOVERALL, (PTR *)ueCb);

   /* No explicit state change required, as we are deleting the UECb itself */

   /*******************************************************/
   /* Delete this ueCb as UE Context Release is received */
   /*******************************************************/
   czNmmDelUeCb(ueCb);
   ueCb= NULLP;

   /* Increment the incoming counter for this message */
   /* czUtlUpdInSts(peerCb, CZT_MSG_UE_CXT_RELEASE); */

   CZ_BMP_LOG_IN_HDLR_INFO(peerCb, oldUEX2APIdVal, newUEX2APIdVal,
                          CZ_EVNT_UE_CXT_RELEASE);

   /*******************************************************/
   /* Invoke a certain function in UI module to send it up to user */
   /*******************************************************/
   ret = czUimSndBmpInd(peerCb, evnt);

   /* There is no roll back check required, as UECb is already deleted due to 
   * receipt of UE Context Release. But help peer aware of it not delivered
   to user.*/
   if(ROK != ret)
   {
#ifndef ALIGN_64BIT
      CZ_DBG_FATAL((CZ_PRNT_BUF, "Indication of the incoming X2-UE Context \
               Release failed from the peer(%ld) for UE(%d:%d)\n", 
               (peerCb->peerId), oldUEX2APIdVal, newUEX2APIdVal));
#else /* ALIGN_64BIT */
      CZ_DBG_FATAL((CZ_PRNT_BUF, "Indication of the incoming X2-UE Context \
               Release failed from the peer(%d) for UE(%d:%d)\n", 
               (peerCb->peerId), oldUEX2APIdVal, newUEX2APIdVal));
#endif /* ALIGN_64BIT */
      CZ_BMP_LOG_IN_HDLR_ERR(peerCb, oldUEX2APIdVal, newUEX2APIdVal,
                             CZ_EVNT_UE_CXT_RELEASE, ret);

      /*******************************************************/
      /* Inform back to peer */
      /* Try generating ErrIndication to peer with
         appropriate cause */
      /* Invoke a certain function in UTL to generate this */
      /*******************************************************/
      czBmpSndErrIndToPeer(peerCb, evnt, &czUeInfo, 
                           CZT_MSG_UE_CXT_RELEASE, 
      /* cz001.301: Fix for the build warnings */
#ifdef CZTV1
                           CZT_CAUSE_MISC,
#else
                           CAUSE_MISC,
#endif
                           CztCauseMiscom_interventionEnum,
                           CZT_ERRCAUSE_MSG_NOT_DELIVERED);
      CZ_RETVALUE(ret);
   }

   CZ_BMP_LOG_EXIT_IN_HDLR_INFO(peerCb, oldUEX2APIdVal, newUEX2APIdVal, 
                  CZ_EVNT_UE_CXT_RELEASE);

   CZ_RETVALUE(ROK); /* All Done */

} /* End of function czBmpHdlInUeCnxtRelease */



 /**
    * @brief This function handles the basic mobility management related to timers'
    * expiry. 
    * 
    * @details The timers handled are:
        - CZ_TMR_TRELOCPREP,
        - CZ_TMR_TRELOCOVERALL.   <br>
    * Handover Cancel is sent to peer as a part of handling these timers' expiry. 
    * A status indication is triggered to inform the user about these timers' expiry.

    * @param[in] ueCb     An UECb pointer from this eNodeB for an UE handover.
    * @param[in] tmrEvnt  A Timer event which triggered the associated BMP timer
    *                     expiry.
    *  
    * @return 
    * -# ROK on success. 
    * -# RFAILED if error encountered within the function.
    *
    * 
    */
#ifdef ANSI
PUBLIC S16 czBmpHdlTmrEvnt
(
CzUeCb   *ueCb,               
S16      tmrEvnt
)
#else /* ANSI */
PUBLIC S16 czBmpHdlTmrEvnt(ueCb, tmrEvnt)
CzUeCb   *ueCb;               
S16      tmrEvnt;               
#endif /* ANSI */
{
   CzPeerCb  *peerCb;
   CztEvnt  *cancelEvnt = NULLP;
   CztStaInd tmrExpStaInd;
   CzCztSapCb  *ueCztSapCb = NULLP;  
   U16 oldUEX2APIdVal, newUEX2APIdVal;
   U32 peerId;
   CztStrmId streamId;
   U8 causeVal = 0, lclStatVal;
   S16 ret = ROK;

   CZ_TRC2(czBmpHdlTmrEvnt)

   switch(tmrEvnt)
   {
      case CZ_TMR_TRELOCPREP: /* TRelocprep expired*/
      {
         CZ_BMP_LOG_TMR_EXPIRY_INFO(ueCb, tmrEvnt);
         /*******************************************************/
         /* Generate the HO Cancel since peer hasnt completed with HO
           Preparation Failure or HO Request Ack. */
         /* Cause value Timer TRELOCPREP_EXPIRY */
         /*******************************************************/
         causeVal = CztCauseRadioNwtrelocprep_expiryEnum;
         lclStatVal = CZT_STATUS_UE_TRELOCPREP_EXPIRY; 
         break;  
      }
     
      case CZ_TMR_TX2RELOCOVERALL: /* TX2RelocOverall expired*/
      {
         CZ_BMP_LOG_TMR_EXPIRY_INFO(ueCb, tmrEvnt);
         /*******************************************************/
        /* Generate the HO Cancel since peer hasnt completed with UE Context
           Release */
         /* Cause value timer TX2RELOCOVERALL_EXPIRY*/
         /*******************************************************/
         causeVal = CztCauseRadioNwtx2relocoverall_expiryEnum;
         lclStatVal = CZT_STATUS_UE_TX2RELOCOVERALL_EXPIRY;
         break; 
      }
      default: /* No more timers */
      {  
         /* Dont require to raise an alarm or inform the user */  
         CZ_DBG_ERROR((CZ_PRNT_BUF, "Unknown timer for BMP(%d) with UE(%p)", \
                       tmrEvnt, (Void *)ueCb));
         CZ_RETVALUE(ROK);
      }
   }

   peerCb = ueCb->peerCb;
   peerId = peerCb->peerId;
   ueCztSapCb = peerCb->cztSapCb;
   oldUEX2APIdVal = ueCb->oldUEX2APId;
   newUEX2APIdVal = ueCb->newUEX2APId;
   streamId = ueCb->streamId;

   /*******************************************************/
   /* Let us remove this ueCb, user can still perform
    * local release if wrong in state of peer or cancel
    * cannot be sent out */
   /* Delete this ueCb as HO cancel is to be generated */
   /*******************************************************/
   
   czNmmDelUeCb(ueCb);
   ueCb= NULLP;

   /* If Reset is active do not act on BMP HO Cancel */
   if(CZ_GP_CONNECTING == peerCb->state)
   {
#ifdef ALIGN_64BIT
      CZ_DBG_ERROR((CZ_PRNT_BUF, "Peer(%d) in Reset state with timer(%d) "\
                 "expiry on UE(%d:%d), aborting...",       \
                 peerId, lclStatVal, oldUEX2APIdVal, newUEX2APIdVal));
#else /* ALIGN_64BIT */
      CZ_DBG_ERROR((CZ_PRNT_BUF, "Peer(%ld) in Reset state with timer(%d) "\
                 "expiry on UE(%d:%d), aborting...",       \
                 peerId, lclStatVal, oldUEX2APIdVal, newUEX2APIdVal));
#endif /* ALIGN_64BIT */
      goto staInd;
   }

   CZ_BMP_FILL_UE_INFO(oldUEX2APIdVal, newUEX2APIdVal);
   /* ccpu00129802 */
   if(tmrEvnt == CZ_TMR_TRELOCPREP)
   {
   cancelEvnt = czUtlGenHoCancel(&czUeInfo, CAUSE_RADIONW, causeVal);
   if(NULLP != cancelEvnt)
   {
      CZ_BMP_LOG_OUT_HDLR_INFO(peerCb, oldUEX2APIdVal, newUEX2APIdVal, 
                               CZ_EVNT_HO_CANCEL, streamId);

      /*******************************************************/
      /* Invoke a certain function in Transport module to send it out to peer */
      /*******************************************************/
      ret = czTpmSndToLower(peerCb, &(cancelEvnt->pdu), streamId);

      if(ROK != ret)
      {
#ifndef ALIGN_64BIT
      CZ_DBG_FATAL((CZ_PRNT_BUF, "Transmission of the outcoming X2-Handover \
               Cancel failed towards the peer(%ld) for UE(%d:%d)\n", 
               (peerCb->peerId), oldUEX2APIdVal, newUEX2APIdVal));
#else /* ALIGN_64BIT */
      CZ_DBG_FATAL((CZ_PRNT_BUF, "Transmission of the outcoming X2-Handover \
               Cancel failed towards the peer(%d) for UE(%d:%d)\n", 
               (peerCb->peerId), oldUEX2APIdVal, newUEX2APIdVal));
#endif /* ALIGN_64BIT */
         CZ_BMP_LOG_OUT_HDLR_ERR(peerCb, oldUEX2APIdVal, newUEX2APIdVal,
                                 CZ_EVNT_HO_CANCEL, ret);

         /* Increment the outgoing counter for this message */
         czUtlUpdOutErrSts(peerCb, CZT_MSG_HO_CANCEL);

         
         /* Inform back to user */
         /* Invoke a certain function in UIM to generate this */
         CZ_SEND_LCL_GEN_ERR(peerCb, CZT_ERRTYPE_MSG_SEND_FAILED, 
                             ret,
                             CZT_MSG_HO_CANCEL);
      }
      else
      {
         CZ_BMP_LOG_EXIT_OUT_HDLR_INFO(peerCb, oldUEX2APIdVal, newUEX2APIdVal, 
                      CZ_EVNT_HO_CANCEL, streamId);

         /* Increment the outgoing error counter for this message */
         czUtlUpdOutSts(peerCb, CZT_MSG_HO_CANCEL);
      }
      CZ_FREEEVNT(cancelEvnt);
   }
   else
   {
#ifdef ALIGN_64BIT
      CZ_DBG_ERROR((CZ_PRNT_BUF, "Building HO Cancel with timer(%d) "\
               "expiry on UE(%d:%d) to peer(%d) failed\n",\
                lclStatVal, oldUEX2APIdVal,newUEX2APIdVal, (peerCb->peerId)));
#else /* ALIGN_64BIT */
      CZ_DBG_ERROR((CZ_PRNT_BUF, "Building HO Cancel with timer(%d) "\
               "expiry on UE(%d:%d) to peer(%ld) failed\n",\
                lclStatVal, oldUEX2APIdVal,newUEX2APIdVal, (peerCb->peerId)));
#endif /* ALIGN_64BIT */
   }
   }
   /* ccpu00129802 */
staInd:
   /* Inform user about this action */
   /* Invoke a certain function in UIM to generate this */
   CZ_SET_ZERO(&tmrExpStaInd, sizeof(CztStaInd)); 
   tmrExpStaInd.peerId = peerId;
   tmrExpStaInd.type = CZT_STA_TYPE_TMR_EXPIRY;
   tmrExpStaInd.status = lclStatVal;
   tmrExpStaInd.reason = CZT_REASON_NOT_APPL;
   tmrExpStaInd.oldUEX2APId = oldUEX2APIdVal;

   ret = czUimSndStaInd(ueCztSapCb, &tmrExpStaInd);
   if(ROK != ret)
   {
#ifdef ALIGN_64BIT
      CZ_DBG_ERROR((CZ_PRNT_BUF, "Delivering Status Indication with timer(%d) "\
                 "expiry on UE(%d:%d), peer(%d) failed",       \
                 lclStatVal, oldUEX2APIdVal, newUEX2APIdVal,          \
                 peerId));                                            
#else /* ALIGN_64BIT */
      CZ_DBG_ERROR((CZ_PRNT_BUF, "Delivering Status Indication with timer(%d) "\
                 "expiry on UE(%d:%d), peer(%ld) failed",       \
                 lclStatVal, oldUEX2APIdVal, newUEX2APIdVal,          \
                 peerId));                                            
#endif /* ALIGN_64BIT */
   }

   CZ_RETVALUE(ROK);

} /* End of function czBmpHdlTmrEvnt */

/*******************************************************/
/* All the Error handler functions to process outgoing 
  X2AP basic mobility management messages in an unexpected 
  state.*/
/*******************************************************/

/**
    * @brief This function handles the X2AP mobility management message
    * received from user, in an unexpected state for an UE in
      active handover. 
    * 
    * @details It generates a Local Error Indication back to user with required
      information about peer and UE.

    * @param[in] peerCb      The active peerCb from this eNodeB involved for an
                             UE handover.
    * @param[in] ueCb        An UECb pointer from this eNodeB for an UE handover.
    * @param[in] evnt        The pointer with X2AP ASN PDU structure received
                             from user for an UE handover.
    * @param[in] hoMsgType   Basic Mobility management message, represented
                             by Trillium X2AP Message value.
    * @param[in] oldUEX2APIdVal The old UE X2AP ID if present in the received 
                                mobility message.
    * @note: For a list of Trillium X2AP message representation refer to czt.h.
    *  
    * @return RFAILED. 
    */

#ifdef ANSI
PRIVATE S16 czBmpHdlOutEvntErr
(
CzPeerCb   *peerCb,
CzUeCb     *ueCb,             
CztEvnt    *evnt,                 
U8         hoMsgType,
U16        oldUEX2APIdVal
)
#else /* ANSI */
PRIVATE S16 czBmpHdlOutEvntErr(peerCb, ueCb, evnt, hoMsgType, oldUEX2APIdVal)
CzPeerCb   *peerCb;
CzUeCb     *ueCb;             
CztEvnt    *evnt;                 
U8         hoMsgType;
U16        oldUEX2APIdVal;
#endif /* ANSI */
{
   CztUE_X2AP_ID *newUEX2APId = NULLP;
   U16 newUEX2APIdVal = CZ_UNKNOWN_UE_X2AP_ID;
   U8 state = 0xFF, causeVal = CZT_ERRCAUSE_UNEXP_MSG;
   S16 hoMsgEvnt;

   CZ_TRC2(czBmpHdlOutEvntErr)

   /*******************************************************/
   /* Inform back to user the message to send to peer is received in wrong
     state */
   /*******************************************************/

   /* New UE X2AP ID is not present in Handover Request and Failure */
   /* Let us avoid the calculation of getting this IE. */
   if((CZT_MSG_HO_REQ != hoMsgType) && (CZT_MSG_HO_FAIL != hoMsgType))
   {
      czUtlGetIE(&(evnt->pdu), Cztid_New_eNB_UE_X2AP_ID, 
                 (TknU8 **)&newUEX2APId);
      if((NULLP != newUEX2APId) && (PRSNT_NODEF == newUEX2APId->pres))
      {
         /*kworks fix: type cast*/
         newUEX2APIdVal =(U16) newUEX2APId->val;
      }
   }

   CZ_GET_EVENT_MSG_MAP(hoMsgType, hoMsgEvnt);

   /* If UECb is present, for some reasons, invalid 
      old UE X2AP ID has been filled by user */
   if(ueCb) 
   {
      state = ueCb->state;
      causeVal = CZT_ERRCAUSE_INV_STATE;
   }
   /* Klock works Fix */
   /* czBmpEvntPrintMap only has Basic mobility procedure messages */
   /* hoMsgEvnt beyond 6 cannot be possible */
   if (CZ_BMP_HO_MAX_EVENTS < hoMsgEvnt)
   {
      hoMsgEvnt = CZ_BMP_HO_MAX_EVENTS;
   }
#ifdef ALIGN_64BIT
   CZ_DBG_ERROR((CZ_PRNT_BUF, "Mobility event(%s) received in state(%s) "\
                 "for an UE(%d:%d) from user to send to peer(%d) \n", 
                 czBmpEvntPrintMap[hoMsgEvnt],\
      ((state <= CZ_BMP_HO_PREPARED)? (czBmpStatePrintMap[state]):("Invalid")),\
                 oldUEX2APIdVal, newUEX2APIdVal, (peerCb->peerId)));
#else /* ALIGN_64BIT */
   CZ_DBG_ERROR((CZ_PRNT_BUF, "Mobility event(%s) received in state(%s) "\
                 "for an UE(%d:%d) from user to send to peer(%ld) \n", 
                 czBmpEvntPrintMap[hoMsgEvnt],\
      ((state <= CZ_BMP_HO_PREPARED)? (czBmpStatePrintMap[state]):("Invalid")),\
                 oldUEX2APIdVal, newUEX2APIdVal, (peerCb->peerId)));
#endif /* ALIGN_64BIT */

   CZ_BMP_FILL_UIM_UE_ERR_INFO(peerCb, hoMsgType,
                               oldUEX2APIdVal, newUEX2APIdVal);

   czBmpGenLclErrInd(peerCb, evnt, CZT_ERRTYPE_INTERNAL,
                     causeVal);


   CZ_RETVALUE(RFAILED);
   
} /* End of function czBmpHdlOutEvntErr */




/*******************************************************/
/* All the Error handler functions to process incoming 
  X2AP basic mobility management messages in an unexpected 
  state.*/
/*******************************************************/


/**
    * @brief This function handles the X2AP Handover Request message
    * received from peer, in an unexpected state for an UE in
      active handover. 
    * 
    * @details It generates an X2 Error Indication to the peer from which
      the message was received in wrong state.

    * @param[in] peerCb      The active peerCb from this eNodeB involved for an
                             UE handover.
    * @param[in] ueCb        An UECb pointer from this eNodeB for an UE handover.
    * @param[in] evnt        The pointer with X2AP ASN PDU structure received
                             from peer for an UE handover.
    * @param[in] hoMsgType   Basic Mobility management message, represented
                             by Trillium X2AP Message value.
    * @param[in] oldUEX2APIdVal The old UE X2AP ID if present in the received 
                                mobility message.
    * @note: For a list of Trillium X2AP message representation refer to czt.h.

    * @return RFAILED. 
    * 
    */
#ifdef ANSI
PRIVATE S16 czBmpHdlInHoReqErr
(
CzPeerCb   *peerCb,
CzUeCb     *ueCb,             
CztEvnt    *evnt,
U8         hoMsgType,
U16        oldUEX2APIdVal
)
#else /* ANSI */
PRIVATE S16 czBmpHdlInHoReqErr(peerCb, ueCb, evnt, hoMsgType, oldUEX2APIdVal)
CzPeerCb   *peerCb;
CzUeCb     *ueCb;             
CztEvnt    *evnt;                 
U8         hoMsgType;
U16        oldUEX2APIdVal;
#endif /* ANSI */
{
   CztStrmId streamId;
   U8 state = 0xFF;

   CZ_TRC2(czBmpHdlInHoReqErr)
   
   /* Inform back to peer that this message is received in wrong
     state */

   if(ueCb)
   {
      state = ueCb->state;
      streamId = ueCb->streamId;
   }
   else
   {
      czUtlGetStream(peerCb, &streamId);
   }

#ifdef ALIGN_64BIT
   CZ_DBG_ERROR((CZ_PRNT_BUF, "Handover Request received in state(%s) "\
                 "for an UE(%d:%d) from peer(%d)\n", \
     ((state <= CZ_BMP_HO_PREPARED)? (czBmpStatePrintMap[state]):("Invalid")),\
                 oldUEX2APIdVal, CZ_UNKNOWN_UE_X2AP_ID, peerCb->peerId));
#else /* ALIGN_64BIT */
   CZ_DBG_ERROR((CZ_PRNT_BUF, "Handover Request received in state(%s) "\
                 "for an UE(%d:%d) from peer(%ld)\n", \
     ((state <= CZ_BMP_HO_PREPARED)? (czBmpStatePrintMap[state]):("Invalid")),\
                 oldUEX2APIdVal, CZ_UNKNOWN_UE_X2AP_ID, peerCb->peerId));
#endif /* ALIGN_64BIT */

   CZ_BMP_FILL_UE_INFO(oldUEX2APIdVal, CZ_UNKNOWN_UE_X2AP_ID); 

   /* Inform back to peer */
   /* No need to check the return value */
   czBmpSndFailErrToPeer(peerCb, evnt, &czUeInfo, CZT_MSG_HO_REQ, streamId);

   CZ_RETVALUE(RFAILED);
} /* End of function czBmpHdlInHoReqErr */


/**
    * @brief This function handles the X2AP Handover Request Acknowledge message
      and Handover Preparation Failure received from peer, in an unexpected state
      for an UE in active handover. 
    * 
    * @details It simply ignores these messages from peer without 
    * generating error indication to peer. But user is informed about the 
    * receipt of these response events.

    * @param[in] peerCb      The active peerCb from this eNodeB involved for an
                             UE handover.
    * @param[in] ueCb        An UECb pointer from this eNodeB for an UE handover.
    * @param[in] evnt        The pointer with X2AP ASN PDU structure received
                             from peer for an UE handover.
    * @param[in] hoMsgType   Basic Mobility management message, represented
                             by Trillium X2AP Message value.
    * @param[in] oldUEX2APIdVal The old UE X2AP ID if present in the received 
                                mobility message.
    * @note: For a list of Trillium X2AP message representation, refer to czt.h.
    *  
    * @return RFAILED. 
    * 
    */
#ifdef ANSI
PRIVATE S16 czBmpHdlInHoRspEvntErr
(
CzPeerCb   *peerCb,
CzUeCb     *ueCb,             
CztEvnt    *evnt,                
U8         hoMsgType,
U16        oldUEX2APIdVal
)
#else /* ANSI */
PRIVATE S16 czBmpHdlInHoRspEvntErr(peerCb, ueCb, evnt, hoMsgType, 
                                   oldUEX2APIdVal)
CzPeerCb   *peerCb;
CzUeCb     *ueCb;             
CztEvnt    *evnt;                 
U8         hoMsgType;
U16        oldUEX2APIdVal;
#endif /* ANSI */
{
   CztUE_X2AP_ID *newUEX2APId = NULLP;
   U16 newUEX2APIdVal = CZ_UNKNOWN_UE_X2AP_ID;
   U8 state = 0xFF;
   S16 hoMsgEvnt;

   CZ_TRC2(czBmpHdlInHoRspEvntErr)

   /* Increment the incoming counter for this message resulted in 
      error */
   /* czUtlUpdInErrSts(peerCb, hoMsgType); */

   /* New UE X2AP ID is not present in Handover Failure though */
   czUtlGetIE(&(evnt->pdu), Cztid_New_eNB_UE_X2AP_ID, (TknU8 **)&newUEX2APId);
   if(NULLP != newUEX2APId)
   {
      /*kworks fix: type cast*/
      newUEX2APIdVal = (U16) newUEX2APId->val;
   }

   CZ_GET_EVENT_MSG_MAP(hoMsgType, hoMsgEvnt);

   if(ueCb) 
   {
      state = ueCb->state;
   }
   
   /* Klock works Fix */
   /* czBmpEvntPrintMap only has Basic mobility procedure messages */
   /* hoMsgEvnt beyond 6 cannot be possible */
   if (CZ_BMP_HO_MAX_EVENTS < hoMsgEvnt)
   {
      hoMsgEvnt = CZ_BMP_HO_MAX_EVENTS;
   }
#ifdef ALIGN_64BIT
   CZ_DBG_ERROR((CZ_PRNT_BUF, "Mobility response event(%s) received in wrong"\
                 "state (%s) for an UE(%d:%d) from peer(%d) \n", \
                 czBmpEvntPrintMap[hoMsgEvnt],\
     ((state <= CZ_BMP_HO_PREPARED)? (czBmpStatePrintMap[state]):("Invalid")),\
                 oldUEX2APIdVal, newUEX2APIdVal, (peerCb->peerId)));
#else /* ALIGN_64BIT */
   CZ_DBG_ERROR((CZ_PRNT_BUF, "Mobility response event(%s) received in wrong"\
                 "state (%s) for an UE(%d:%d) from peer(%ld) \n", \
                 czBmpEvntPrintMap[hoMsgEvnt],\
     ((state <= CZ_BMP_HO_PREPARED)? (czBmpStatePrintMap[state]):("Invalid")),\
                 oldUEX2APIdVal, newUEX2APIdVal, (peerCb->peerId)));
#endif /* ALIGN_64BIT */

  
   /* No need to inform back to peer */
   /*******************************************************/
   /* The source eNB shall ignore any HANDOVER REQUEST ACKNOWLEDGE or
      HANDOVER PREPARATION FAILURE message received after the initiation
      of the Handover Cancel procedure and remove any reference and release
      any resources related to the concerned X2 UE-associated signalling.*/
   /*******************************************************/
   CZ_BMP_FILL_UIM_UE_ERR_INFO(peerCb, hoMsgType,
                               oldUEX2APIdVal, newUEX2APIdVal);
   czBmpGenLclErrInd(peerCb, evnt, CZT_ERRTYPE_FROM_PEER, 
                     CZT_ERRCAUSE_INV_STATE);

   CZ_RETVALUE(RFAILED); /* All Done */
} /* End of function czBmpHdlInHoRspEvntErr */




/**
    * @brief This function handles the X2AP Handover Cancel and SN Status Transfer
    * message received from peer, in an unexpected state for an UE in active handover. 
    * 
    * @details It simply ignores the HO Cancel and SN Status Transfer from peer without 
    * generating error indication to peer.

    * @param[in] peerCb      The active peerCb from this eNodeB involved for an
                             UE handover.
    * @param[in] ueCb        An UECb pointer from this eNodeB for an UE handover.
    * @param[in] evnt        The pointer with X2AP ASN PDU structure received
                             from peer for an UE handover.
    * @param[in] hoMsgType   Basic Mobility management message, represented
                             by Trillium X2AP Message value.
    * @param[in] oldUEX2APIdVal The old UE X2AP ID if present in the received 
                                mobility message.
    * @note: For a list of Trillium X2AP message representation, refer to czt.h.
    *  
    * @return RFAILED. 
    * 
    */
#ifdef ANSI
PRIVATE S16 czBmpHdlInReqErr
(
CzPeerCb   *peerCb,
CzUeCb     *ueCb,             
CztEvnt    *evnt,                
U8         hoMsgType,
U16        oldUEX2APIdVal
)
#else /* ANSI */
PRIVATE S16 czBmpHdlInReqErr(peerCb, ueCb, evnt, hoMsgType, oldUEX2APIdVal)
CzPeerCb   *peerCb;
CzUeCb     *ueCb;             
CztEvnt    *evnt;                 
U8         hoMsgType;
U16        oldUEX2APIdVal;
#endif /* ANSI */
{
   CztUE_X2AP_ID *newUEX2APId = NULLP;
   U16 newUEX2APIdVal = CZ_UNKNOWN_UE_X2AP_ID;
   U8 state = 0xFF;
   S16 hoMsgEvnt;

   CZ_TRC2(czBmpHdlInReqErr)

   /* Increment the incoming counter for this message resulted in 
      error */
   /* czUtlUpdInErrSts(peerCb, hoMsgType); */

   /* New UE X2AP ID is not mandatory in Handover Cancel though */
   czUtlGetIE(&(evnt->pdu), Cztid_New_eNB_UE_X2AP_ID, (TknU8 **)&newUEX2APId);
   if(NULLP != newUEX2APId)
   {
      /*kworks fix: tpye cast*/ 
      newUEX2APIdVal = (U16) newUEX2APId->val;
   }

   CZ_GET_EVENT_MSG_MAP(hoMsgType, hoMsgEvnt);

   if(ueCb) 
   {
      state = ueCb->state;
   }
   /* Klock works Fix */
   /* czBmpEvntPrintMap only has Basic mobility procedure messages */
   /* hoMsgEvnt beyond 6 cannot be possible */
   if (CZ_BMP_HO_MAX_EVENTS < hoMsgEvnt)
   {
      hoMsgEvnt = CZ_BMP_HO_MAX_EVENTS;
   }
#ifdef ALIGN_64BIT
   CZ_DBG_ERROR((CZ_PRNT_BUF, "Mobility event(%s) received in wrong state (%s)"\
                 " for an UE(%d:%d) from peer(%d) \n", 
                 czBmpEvntPrintMap[hoMsgEvnt],\
     ((state <= CZ_BMP_HO_PREPARED)? (czBmpStatePrintMap[state]):("Invalid")),\
                 oldUEX2APIdVal, newUEX2APIdVal, (peerCb->peerId)));
#else /* ALIGN_64BIT */
   CZ_DBG_ERROR((CZ_PRNT_BUF, "Mobility event(%s) received in wrong state (%s)"\
                 " for an UE(%d:%d) from peer(%ld) \n", 
                 czBmpEvntPrintMap[hoMsgEvnt],\
     ((state <= CZ_BMP_HO_PREPARED)? (czBmpStatePrintMap[state]):("Invalid")),\
                 oldUEX2APIdVal, newUEX2APIdVal, (peerCb->peerId)));
#endif /* ALIGN_64BIT */

   /* No need to inform back to peer */
   /*******************************************************/
   /* Spec says if HO Cancel is received for which there is no 
   *  context found, target eNB shall ignore this message.
      If the target eNB receives SN Status Transfer message
      for an UE for which no prepared handover exists at the
      target eNB, the target eNB shall ignore the message. */
   /*******************************************************/

   CZ_FREEEVNT(evnt);

   CZ_RETVALUE(RFAILED); /* All Done */
} /* End of function czBmpHdlInReqErr */




/**
    * @brief This function handles the X2AP all mobility management 
    * class2 messages and class1 associated failure messages received from peer,
    * in an unexpected state for an UE in active handover. 
    * 
    * @details It generates an X2 Error Indication to the peer from which
      the message was received in wrong state.

    * @param[in] peerCb      The active peerCb from this eNodeB involved for an
                             UE handover.
    * @param[in] ueCb        An UECb pointer from this eNodeB for an UE handover.
    * @param[in] evnt        The pointer with X2AP ASN PDU structure received
                             from peer for an UE handover.
    * @param[in] hoMsgType   Basic Mobility management message, represented
                             by Trillium X2AP Message value.
    * @param[in] oldUEX2APIdVal The old UE X2AP ID if present in the received 
                                mobility message.
    * @note: For a list of Trillium X2AP message representation refer to czt.h.
    *  
    * @return RFAILED. 
    * 
    */
#ifdef ANSI
PRIVATE S16 czBmpHdlInEvntErr
(
CzPeerCb   *peerCb,
CzUeCb     *ueCb,             
CztEvnt    *evnt,                
U8         hoMsgType,
U16        oldUEX2APIdVal
)
#else /* ANSI */
PRIVATE S16 czBmpHdlInEvntErr(peerCb, ueCb, evnt, hoMsgType, oldUEX2APIdVal)
CzPeerCb   *peerCb;
CzUeCb     *ueCb;             
CztEvnt    *evnt;                 
U8         hoMsgType;
U16        oldUEX2APIdVal;
#endif /* ANSI */
{
   CztUE_X2AP_ID *newUEX2APId = NULLP;
   U8 state = 0xFF;
   S16 hoMsgEvnt;

   CZ_TRC2(czBmpHdlInEvntErr)

   /* Inform back to peer that this message is received in wrong
     state */

   CZ_BMP_FILL_UE_INFO(oldUEX2APIdVal, CZ_UNKNOWN_UE_X2AP_ID);

   /* New UE X2AP ID is not present in Handover Failure */
   /* Let us avoid the calculation of getting this IE */
   if(hoMsgType != CZT_MSG_HO_FAIL)
   {
      czUtlGetIE(&(evnt->pdu), Cztid_New_eNB_UE_X2AP_ID, 
                  (TknU8 **)&newUEX2APId);
      if(NULLP != newUEX2APId)
      {
         /*kworks fix: type cast*/
         czUeInfo.newUEX2APId = (U16) newUEX2APId->val;
      }
   }

   CZ_GET_EVENT_MSG_MAP(hoMsgType, hoMsgEvnt);

   if(ueCb) 
   {
      state = ueCb->state;
   } 
   /* Klock works Fix */
   /* czBmpEvntPrintMap only has Basic mobility procedure messages */
   /* hoMsgEvnt beyond 6 cannot be possible */
   if (CZ_BMP_HO_MAX_EVENTS < hoMsgEvnt)
   {
      hoMsgEvnt = CZ_BMP_HO_MAX_EVENTS;
   }
#ifdef ALIGN_64BIT
   CZ_DBG_ERROR((CZ_PRNT_BUF, "Mobility event(%s) received in wrong state (%s)"\
                 " for an UE(%d:%d) from peer(%d) \n", 
                 czBmpEvntPrintMap[hoMsgEvnt],\
      ((state <= CZ_BMP_HO_PREPARED)? (czBmpStatePrintMap[state]):("Invalid")),\
                 oldUEX2APIdVal, czUeInfo.newUEX2APId, peerCb->peerId));
#else /* ALIGN_64BIT */
   CZ_DBG_ERROR((CZ_PRNT_BUF, "Mobility event(%s) received in wrong state (%s)"\
                 " for an UE(%d:%d) from peer(%ld) \n", 
                 czBmpEvntPrintMap[hoMsgEvnt],\
      ((state <= CZ_BMP_HO_PREPARED)? (czBmpStatePrintMap[state]):("Invalid")),\
                 oldUEX2APIdVal, czUeInfo.newUEX2APId, peerCb->peerId));
#endif /* ALIGN_64BIT */

   /* Inform back to peer */
   /* No need to check the return value */
   czBmpSndErrIndToPeer(peerCb, evnt, &czUeInfo, 
                        hoMsgType,
      /* cz001.301: Fix for the build warnings */
#ifdef CZTV1
                        CZT_CAUSE_PROTOCOL,
#else
                        CAUSE_PROTOCOL,
#endif
                    CztCauseProtmessage_not_compatible_with_receiver_stateEnum,
                        CZT_ERRCAUSE_INV_STATE);

   CZ_RETVALUE(RFAILED); /* All Done */
} /* End of function czBmpHdlInEvntErr */


/**
  * @brief Function to validate Error Indication message.
    * @details This function validates if X2AP Error Indication message 
     to send to peer or received from peer, in valid state and on proper
     messages like class2 and class1 response messages only.

    * @param[in] peerCb      The active peerCb from this eNodeB involved for an
                             UE handover.
    * @param[in] hoMsgType   Basic Mobility management message, represented
                             by Trillium X2AP Message value.
    * @param[in] msgDirectn  The Message direction: 
                            -# CZ_DIR_OUT: If from user, 
                            -# CZ_DIR_IN: If from peer. 
    * @note: For a list of Trillium X2AP message representation refer to czt.h.
    *  
    * @return 
    * -# CZT_ERRCAUSE_INV_STATE if peer state is not valid.
    * -# CZT_ERRCAUSE_UNEXP_MSG if Error Indication on Handover Request.
    * -# ROK otherwise.
    */

#ifdef ANSI
PRIVATE S16 czBmpIsErrIndValid
(
CzPeerCb  *peerCb,   
U8        hoMsgType,
U8        msgDirectn
)
#else /* ANSI */
PRIVATE S16 czBmpIsErrIndValid(peerCb, hoMsgType, msgDirectn)
CzPeerCb  *peerCb;       
U8        hoMsgType;  
U8        msgDirectn;
#endif /* ANSI */
{
   S16 lclCauseVal = ROK;
  
    CZ_TRC2(czBmpIsErrIndValid);

   /* Sending / Receiving Error Indication for all BMP messages is not valid
    * other than in connected state */
   /* Allow the Error Indication from peer in connecting state from peer;
      If we arrived at connecting state in the middle for BMP transaction,
      for which peer might respond with error indication to associated
      BMP message */  
   if((CZ_GP_IDLE == peerCb->state) ||
      ((CZ_GP_CONNECTING == peerCb->state) && (CZ_DIR_OUT == msgDirectn)))
   {
#ifdef ALIGN_64BIT
      CZ_DBG_ERROR((CZ_PRNT_BUF, "Error Indication from(%s) "\
                      "when peer (%d) in not active state(%d)\n",\
    ((msgDirectn == CZ_DIR_OUT) ? (CZ_BMP_MSG_FRM_USER):(CZ_BMP_MSG_FRM_PEER)),\
     (peerCb->peerId), (peerCb->state)));
#else /* ALIGN_64BIT */
      CZ_DBG_ERROR((CZ_PRNT_BUF, "Error Indication from(%s) "\
                      "when peer (%ld) in not active state(%d)\n",\
    ((msgDirectn == CZ_DIR_OUT) ? (CZ_BMP_MSG_FRM_USER):(CZ_BMP_MSG_FRM_PEER)),\
     (peerCb->peerId), (peerCb->state)));
#endif /* ALIGN_64BIT */

      lclCauseVal = CZT_ERRCAUSE_INV_STATE;
   }

   /* Sending/receiving Error Indication for HO Req is not valid, as HO
      Prep Fail is expected in any case for it. */
   else if(CZT_MSG_HO_REQ == hoMsgType)
   {
#ifdef ALIGN_64BIT
      CZ_DBG_ERROR((CZ_PRNT_BUF, "Error Indication for Handover Request"\
                      "is invalid for peer (%d)\n", (peerCb->peerId)));
#else /* ALIGN_64BIT */
      CZ_DBG_ERROR((CZ_PRNT_BUF, "Error Indication for Handover Request"\
                      "is invalid for peer (%ld)\n", (peerCb->peerId)));
#endif /* ALIGN_64BIT */
      lclCauseVal = CZT_ERRCAUSE_UNEXP_MSG;
   }

   CZ_RETVALUE(lclCauseVal);

} /* End of function czBmpIsErrIndValid */


/**
  * @brief Function to generate Local Error Indication message.
    * @details This function generates a Local Error Indication message
   if X2AP Error Indication message sent to peer or received from peer, 
   is invalid.

    * @param[in] peerCb      The active peerCb from this eNodeB involved for an
                             UE handover.
    * @param[in] evnt        The pointer with X2AP ASN PDU structure for this
                             message.
    * @param[in] msgDirectn  The Message direction: 
                            -# CZ_DIR_OUT: If from user, 
                            -# CZ_DIR_IN: If from peer. 
    * @param[in] causeVal    The local error cause value to fill in the error
                             indication to inform the user.
    * @note: For a list of Trillium X2AP message representation, refer to czt.h.
    *  
    * @return 
    * -# CZT_ERRCAUSE_INV_STATE if peer state is not valid.
    * -# CZT_ERRCAUSE_UNEXP_MSG if Error Indication on Handover Request.
    * -# ROK otherwise.
    */

#ifdef ANSI
PRIVATE S16 czBmpGenErrIndEvntErr
(
CzPeerCb  *peerCb,   
CztEvnt   *evnt,
U8        msgDirectn,
U8        causeVal
)
#else /* ANSI */
PRIVATE S16 czBmpGenErrIndEvntErr(peerCb, evnt, msgDirectn, causeVal)
CzPeerCb  *peerCb;       
CztEvnt   *evnt;
U8        msgDirectn;
U8        causeVal;
#endif /* ANSI */
{
   S16 ret;
   CZ_TRC2(czBmpGenErrIndEvntErr);

   if(CZ_DIR_OUT == msgDirectn)
   {
      /* Increment the outgoing error counter for this message 
     resulted in error */
      czUtlUpdOutErrSts(peerCb, CZT_MSG_ERR_IND);

      /* Inform back to user */
      /* Invoke a certain function in UIM to generate this */
      CZ_BMP_FILL_UIM_GEN_ERR_INFO(peerCb, CZT_ERRTYPE_INTERNAL, 
                                   causeVal,
                                   CZT_MSG_ERR_IND); 
   }
   else /* From peer */
   {
      /* Increment the incoming error counter for this message 
     resulted in error */
      czUtlUpdInErrSts(peerCb, CZT_MSG_ERR_IND);

      /* Not acting anything when received from peer becoz:
           1. No point in sending Error Indication again back to peer, indicating
              invalid ID(s). 
           2. Best thing to drop it without acting and give it to user. */ 

      /* Inform the user that event has occurred w.r.t peer
         Invoke a certain function in UIM to generate this */
      CZ_BMP_FILL_UIM_GEN_ERR_INFO(peerCb, CZT_ERRTYPE_FROM_PEER, 
                                   causeVal,
                                   CZT_MSG_ERR_IND); 
   }
   ret = czUimSndErrInd (peerCb->cztSapCb, &czBmpErrInd, evnt, NULLP); 
   if(ROK != ret)
   {
      CZ_FREEEVNT(evnt);
   }

   CZ_RETVALUE(ROK);
} /* End of function czBmpGenErrIndEvntErr */


/**
    * @brief This function generates local error indication to deliver to
      user by using structure CztLclErrInd.
    * 
    * @details The error may occur in different handlers during the UE handover
      or invalid/logical error cases when handling incoming or outgoing 
      X2AP BMP messages. 

    * @param[in] peerCb      The active peerCb from this eNodeB involved for an
                             UE handover.
    * @param[in] evnt        The pointer with X2AP ASN PDU structure. 
    * @param[in] causeType   The local error cause type to fill in the error
                             indication to inform the user.
    * @param[in] causeVal    The local error cause value to fill in the error
                             indication to inform the user.
    * @return ROK.
    * 
    */

#ifdef ANSI
PRIVATE S16 czBmpGenLclErrInd
(
CzPeerCb  *peerCb,   
CztEvnt   *evnt,     
U8        causeType,
U8        causeVal
)
#else /* ANSI */
PRIVATE S16 czBmpGenLclErrInd(peerCb, evnt, causeType, causeVal)
CzPeerCb  *peerCb;       
CztEvnt   *evnt;         
U8        causeType;
U8        causeVal;
#endif /* ANSI */
{
   S16  ret = ROK;

   CZ_TRC2(czBmpGenLclErrInd)

   CZ_BMP_FILL_UIM_CAUSE_INFO(causeType, causeVal);

   ret = czUimSndErrInd (peerCb->cztSapCb, &czBmpErrInd, evnt, NULLP); 
   if(ROK != ret)
   {
      CZ_FREEEVNT(evnt);
   }

   CZ_RETVALUE(ROK); /* All Done in this function */
} /* End of function czBmpGenLclErrInd */



/**
    * @brief This function sends the Handover Preparation Failure or 
      X2 Error Indication message to peer.
    * 
    * @details 
      -# It generates Handover Preparation Failure to the peer as a result of 
         the incoming Handover Request from peer which resulted in error.
         - This is invoked in cases when Handover Request message is received
           from peer:
          - In wrong state of UE handover.
          - With invalid UE context value of old UE X2AP ID.
          - With invalid UE security capabilities IE values.

      It generates an X2 Error Indication to the peer as a result of 
      the error triggerred from the incoming class2 or class1 response message
      from peer earlier.

     -# It generates an X2 Error Indication to the peer as a result of 
        the error triggerred from the incoming message earlier.
        - This is invoked in cases when message is received from peer:
         - In wrong state of UE handover.
         - With invalid UE context values like old UE X2AP ID or new UE X2AP ID.

      The generated message is sent to the peer by invoking TPM.
    * @param[in] peerCb      The active peerCb from this eNodeB involved for an
                             UE handover.
    * @param[in] evnt        The pointer with X2AP ASN PDU structure received
                             from peer for an UE handover.
    * @param[in] bmpUeInfo   The pointer to UE information structure with
                             required UE X2AP IDs.
    * @param[in] hoMsgType   Basic Mobility management message, represented
                             by Trillium X2AP Message value, received from peer.
                             This message triggers to send back the error indication 
                             to peer.
    * @param[in] streamId    The stream ID to send the Handover Preparation Failure
                             or Error Indication to peer.
    * @note: For a list of Trillium X2AP message representation and local error 
      cause values, refer to czt.h.
    *  
    * @return ROK.
    * 
    */


#ifdef ANSI
PRIVATE S16 czBmpSndFailErrToPeer
(
CzPeerCb  *peerCb,   
CztEvnt   *evnt,     
CztUeInfo *bmpUeInfo,
U8        hoMsgType,
CztStrmId streamId  /* window's warning fix changed data type to CztStrmId from u8 */
)
#else /* ANSI */
PRIVATE S16 czBmpSndFailErrToPeer(peerCb, evnt, bmpUeInfo, hoMsgType, streamId)
CzPeerCb  *peerCb;       
CztEvnt   *evnt;         
CztUeInfo *bmpUeInfo;
U8        hoMsgType;  
CztStrmId streamId; /* window's warning fix changed data type to CztStrmId from u8 */
#endif /* ANSI */
{
   CztEvnt *rspErrEvnt = NULLP;         
   U16     oldUEX2APIdVal = CZ_UNKNOWN_UE_X2AP_ID;
   U16     newUEX2APIdVal = CZ_UNKNOWN_UE_X2AP_ID;
   U8      rspMsgType;
   S16     ret = ROK;

   CZ_TRC2(czBmpSndFailErrToPeer)
   CZ_PRNT_PEERID;

   /* Increment the incoming error counter for this message resulted in 
      error */
   /* czUtlUpdInErrSts(peerCb, hoMsgType); */

#if (ERRCLASS & ERRCLS_INT_PAR)
   if(NULLP == bmpUeInfo)
   {
      CZ_DBG_ERROR((CZ_PRNT_BUF, "bmpUeInfo is NULLP\n"));
      CZ_RETVALUE(RFAILED); 
   }
#endif /* (ERRCLASS & ERRCLS_INT_PAR) */
   oldUEX2APIdVal = bmpUeInfo->oldUEX2APId;
   newUEX2APIdVal = bmpUeInfo->newUEX2APId;
 
   switch(hoMsgType)
   {
      case CZT_MSG_HO_REQ: /* Handover Request */
      {
         rspErrEvnt = czUtlGenHoPrepFailure(((CzIeInfo *)NULLP), 
                                            (CzMsgInfo *)bmpUeInfo,
      /* cz001.301: Fix for the build warnings */
#ifdef CZTV1
                                            CZT_CAUSE_PROTOCOL,
#else
                                            CAUSE_PROTOCOL,
#endif
                    CztCauseProtmessage_not_compatible_with_receiver_stateEnum);

         rspMsgType = CZT_MSG_HO_FAIL;

#ifdef ALIGN_64BIT
         CZ_DBG_INFO((CZ_PRNT_BUF, "Generating HO Preparation Failure"\
                       "on HO Request to peer(%d) with UE(%d:%d)"\
                       "on stream %d\n", (peerCb->peerId), oldUEX2APIdVal,\
                       newUEX2APIdVal, streamId));
#else /* ALIGN_64BIT */
         CZ_DBG_INFO((CZ_PRNT_BUF, "Generating HO Preparation Failure"\
                       "on HO Request to peer(%ld) with UE(%d:%d)"\
                       "on stream %d\n", (peerCb->peerId), oldUEX2APIdVal,\
                       newUEX2APIdVal, streamId));
#endif /* ALIGN_64BIT */
         break;
      }
      case CZT_MSG_HO_ACK: /* Handover Request Ack */
      case CZT_MSG_HO_FAIL: /* Handover Fail */
      case CZT_MSG_HO_CANCEL: /* Handover Cancel */
      case CZT_MSG_SN_STATUS_TRFR: /* SN Status Transfer */
      case CZT_MSG_UE_CXT_RELEASE: /* UE Context Release */
      {        
         rspErrEvnt = czUtlGenPeerErrInd(&(evnt->pdu), hoMsgType, 
                                         ((CzMsgInfo *)bmpUeInfo),
      /* cz001.301: Fix for the build warnings */
#ifdef CZTV1
                                            CZT_CAUSE_PROTOCOL,
#else
                                         CAUSE_PROTOCOL,
#endif
                    CztCauseProtmessage_not_compatible_with_receiver_stateEnum);

         rspMsgType = CZT_MSG_ERR_IND;

#ifdef ALIGN_64BIT
         CZ_DBG_INFO((CZ_PRNT_BUF, "Generating Error Indication "\
                      "on %d to peer(%d) with UE(%d:%d) on stream(%d)\n",\
                   hoMsgType, (peerCb->peerId), oldUEX2APIdVal, newUEX2APIdVal,\
                     streamId));
#else /* ALIGN_64BIT */
         CZ_DBG_INFO((CZ_PRNT_BUF, "Generating Error Indication "\
                      "on %d to peer(%ld) with UE(%d:%d) on stream(%d)\n",\
                   hoMsgType, (peerCb->peerId), oldUEX2APIdVal, newUEX2APIdVal,\
                     streamId));
#endif /* ALIGN_64BIT */
         break;   
      }
      default:
      {
         CZ_DBG_ERROR((CZ_PRNT_BUF, "Invalid Message Type\n"));
         CZ_RETVALUE(RFAILED); 
      }
   }  
   if(NULLP != rspErrEvnt)
   {
      /* Increment the outgoing counter for this message */
      czUtlUpdOutSts(peerCb, rspMsgType);

      /*******************************************************/
      /* Invoke a certain function in Transport module to send it out to peer */
      /*******************************************************/
      ret = czTpmSndToLower(peerCb, &(rspErrEvnt->pdu), streamId);

      if(ROK != ret)
      {
#ifdef ALIGN_64BIT
         CZ_DBG_FATAL((CZ_PRNT_BUF, "Sending Handover Error "\
               "Response/Indication %d to peer(%d) failed on UE(%d:%d)\n",\
                rspMsgType, (peerCb->peerId), oldUEX2APIdVal, \
                newUEX2APIdVal));
#else /* ALIGN_64BIT */
         CZ_DBG_FATAL((CZ_PRNT_BUF, "Sending Handover Error "\
               "Response/Indication %d to peer(%ld) failed on UE(%d:%d)\n",\
                rspMsgType, (peerCb->peerId), oldUEX2APIdVal, \
                newUEX2APIdVal));
#endif /* ALIGN_64BIT */
         /* Increment the error outgoing counter for this message */
         czUtlUpdOutErrSts(peerCb, rspMsgType);
      }
      CZ_FREEEVNT(rspErrEvnt);
   }
   else /* Failure Response / Error Indication cannot be built */
   {
#ifdef ALIGN_64BIT
      CZ_DBG_ERROR((CZ_PRNT_BUF, "Building Handover Error "\
               "Response/Indication %d to peer(%d) failed on UE(%d:%d)\n",\
                rspMsgType, (peerCb->peerId), oldUEX2APIdVal, \
                newUEX2APIdVal));
#else /* ALIGN_64BIT */
      CZ_DBG_ERROR((CZ_PRNT_BUF, "Building Handover Error "\
               "Response/Indication %d to peer(%ld) failed on UE(%d:%d)\n",\
                rspMsgType, (peerCb->peerId), oldUEX2APIdVal, \
                newUEX2APIdVal));
#endif /* ALIGN_64BIT */
      /* Increment the error outgoing counter for this message */
      czUtlUpdOutErrSts(peerCb, rspMsgType);
   }

   /* Inform the user too that event has occurred w.r.t peer
      Invoke a certain function in UIM to generate this */
   CZ_BMP_FILL_UIM_ERR_INFO(peerCb, CZT_ERRTYPE_FROM_PEER,
                            CZT_ERRCAUSE_INV_STATE, hoMsgType, 
                            oldUEX2APIdVal, newUEX2APIdVal);

   ret = czUimSndErrInd (peerCb->cztSapCb, &czBmpErrInd, evnt, NULLP); 
   if(ROK != ret)
   {
      CZ_FREEEVNT(evnt);
   }

   CZ_RETVALUE(ROK);

} /* End of function czBmpSndFailErrToPeer */


/**
    * @brief This function sends the Handover Preparation Failure message to peer.
    * 
    * @details It generates Handover Preparation Failure to the peer as a result of 
      the incoming Handover Request from peer which resulted in error.
    * This is invoked in cases when Handover Request message is received from peer:
      - In wrong state of UE handover.
      - With invalid UE context value of old UE X2AP ID.
      - With invalid UE security capabilities IE values.

    * @param[in] peerCb      The active peerCb from this eNodeB involved for an
                             UE handover.
    * @param[in] evnt        The pointer with X2AP ASN PDU structure received
                             from peer for an UE handover.
    * @param[in] bmpUeInfo   The pointer to UE information structure with
                             required UE X2AP IDs.
    * @param[in] errIeInfo   The pointer to error IE information in received
                             Handover Request.
    * @param[in] causeType   The cause type to fill in the error indication
                             to send to peer. This is ASN.1 specific.
    * @param[in] causeVal    The cause value to fill in the error indication
                             to send to peer. This is ASN.1 specific.
    * @param[in] lclCauseVal The local error cause value to inform the user locally
                             about this event. 
    * @note: For a list of Trillium X2AP message representation and local error 
      cause values, refer to czt.h.
    *  
    * @return ROK.
    * 
    */

#ifdef ANSI
PRIVATE S16 czBmpSndFailRspToPeer
(
CzPeerCb  *peerCb,   
CztEvnt   *evnt,     
CztUeInfo *bmpUeInfo,
CzIeInfo  *errIeInfo,
U8        causeType,
U8        causeVal,
U8        lclCauseVal
)
#else /* ANSI */
PRIVATE S16 czBmpSndFailRspToPeer(peerCb, evnt, bmpUeInfo, errIeInfo, 
                                  causeType, causeVal, lclCauseVal)
CzPeerCb  *peerCb;       
CztEvnt   *evnt;         
CztUeInfo *bmpUeInfo;
CzIeInfo  *errIeInfo;
U8        causeType;
U8        causeVal;
U8        lclCauseVal;
#endif /* ANSI */
{
   CztEvnt *rspErrEvnt = NULLP;
   U16     oldUEX2APIdVal = CZ_UNKNOWN_UE_X2AP_ID;
   U16     newUEX2APIdVal = CZ_UNKNOWN_UE_X2AP_ID;
   CztStrmId streamId;
   S16     ret = ROK;

   CZ_TRC2(czBmpSndFailRspToPeer)
   /* cz001.101: Klockworks fix */
#if (ERRCLASS & ERRCLS_INT_PAR)
   if(NULLP == bmpUeInfo)
   {
      CZ_DBG_ERROR((CZ_PRNT_BUF, "bmpUeInfo is NULLP\n"));
      CZ_RETVALUE(RFAILED); 
   }
#endif /* (ERRCLASS & ERRCLS_INT_PAR) */
   oldUEX2APIdVal = bmpUeInfo->oldUEX2APId;
   newUEX2APIdVal = bmpUeInfo->newUEX2APId;

   rspErrEvnt = czUtlGenHoPrepFailure(errIeInfo, ((CzMsgInfo *)bmpUeInfo),
                                      causeType, causeVal);
   if(NULLP != rspErrEvnt)
   {
      czUtlGetStream(peerCb, &streamId);
#ifdef ALIGN_64BIT
      CZ_DBG_INFO((CZ_PRNT_BUF, "Generating Handover Preparation Failure "\
                      "to peer(%d) with UE(%d:%d) on stream(%d)\n",\
                      (peerCb->peerId), oldUEX2APIdVal, newUEX2APIdVal,\
                      streamId));
#else /* ALIGN_64BIT */
      CZ_DBG_INFO((CZ_PRNT_BUF, "Generating Handover Preparation Failure "\
                      "to peer(%ld) with UE(%d:%d) on stream(%d)\n",\
                      (peerCb->peerId), oldUEX2APIdVal, newUEX2APIdVal,\
                      streamId));
#endif /* ALIGN_64BIT */
      /*******************************************************/
      /* Invoke a certain function in Transport module to send it out to peer */
      /*******************************************************/
      ret = czTpmSndToLower(peerCb, &(rspErrEvnt->pdu), streamId);

      if(ROK == ret)
      {
         /* Increment the outgoing counter for this message */
         czUtlUpdOutSts(peerCb, CZT_MSG_HO_FAIL);
      }
      else
      {
#ifdef ALIGN_64BIT
         CZ_DBG_FATAL((CZ_PRNT_BUF, "Sending Handover Preparation Failure"\
                "to peer(%d) failed on UE(%d:%d)\n",\
                (peerCb->peerId), oldUEX2APIdVal, \
                newUEX2APIdVal));
#else /* ALIGN_64BIT */
         CZ_DBG_FATAL((CZ_PRNT_BUF, "Sending Handover Preparation Failure"\
                "to peer(%ld) failed on UE(%d:%d)\n",\
                (peerCb->peerId), oldUEX2APIdVal, \
                newUEX2APIdVal));
#endif /* ALIGN_64BIT */
         /* Increment the error outgoing counter for this message */
         czUtlUpdOutErrSts(peerCb, CZT_MSG_HO_FAIL);
      }
      CZ_FREEEVNT(rspErrEvnt);
   }
   else /* Failure Response not built */
   {
#ifdef ALIGN_64BIT
      CZ_DBG_ERROR((CZ_PRNT_BUF, "Building Failure Response"\
                   "to peer(%d) failed on UE(%d:%d)\n",\
                   (peerCb->peerId), oldUEX2APIdVal, \
                   newUEX2APIdVal));
#else /* ALIGN_64BIT */
      CZ_DBG_ERROR((CZ_PRNT_BUF, "Building Failure Response"\
                   "to peer(%ld) failed on UE(%d:%d)\n",\
                   (peerCb->peerId), oldUEX2APIdVal, \
                   newUEX2APIdVal));
#endif /* ALIGN_64BIT */

      /* Increment the error outgoing counter for this message */
      czUtlUpdOutErrSts(peerCb, CZT_MSG_HO_FAIL);
   }

   if(CZT_ERRCAUSE_MSG_NOT_DELIVERED == lclCauseVal)
   {
      CZ_FREEEVNT(evnt);
      CZ_RETVALUE(ROK); /* All done in this function */
   }
      
   /* Inform the user too that event has occurred w.r.t peer
      Invoke a certain function in UIM to generate this */
   CZ_BMP_FILL_UIM_ERR_INFO(peerCb, CZT_ERRTYPE_FROM_PEER,
                            lclCauseVal, CZT_MSG_HO_REQ, 
                            oldUEX2APIdVal, newUEX2APIdVal);

   ret = czUimSndErrInd (peerCb->cztSapCb, &czBmpErrInd, evnt, NULLP); 
   if(ROK != ret)
   {
      CZ_FREEEVNT(evnt);
   }

   CZ_RETVALUE(ROK); /* All done in this function */

} /* End of function czBmpSndFailRspToPeer */


/**
    * @brief This function sends the X2 Error Indication message to peer.
    * 
    * @details It generates an X2 Error Indication to the peer as a result of 
      the error triggerred from the incoming message from peer earlier.
    * This is invoked in cases when message is received from peer:
      - In wrong state of UE handover.
      - With invalid UE context values like old UE X2AP ID or new UE X2AP ID.

    * @param[in] peerCb      The active peerCb from this eNodeB involved for an
                             UE handover.
    * @param[in] evnt        The pointer with X2AP ASN PDU structure received
                             from peer for an UE handover.
    * @param[in] bmpUeInfo   The pointer to the UE information structure with
                             required UE X2AP IDs.
    * @param[in] hoMsgType   Basic Mobility management message, represented
                             by Trillium X2AP Message value, received from peer.
                             This message triggers to send back the error indication 
                             to peer.
    * @param[in] causeType   The cause type to fill in the error indication
                             to send to peer. This is ASN.1 specific.
    * @param[in] causeVal    The cause value to fill in the error indication
                             to send to peer. This is ASN.1 specific.
    * @param[in] lclCauseVal The local error cause value to inform the user locally
                             about this event resulted in error.
    * @note: For a list of Trillium X2AP message representation and local error 
      cause values, refer to czt.h.
    *  
    * @return ROK.
    * 
    */

#ifdef ANSI
PRIVATE S16 czBmpSndErrIndToPeer 
(
CzPeerCb  *peerCb,
CztEvnt   *evnt,   
CztUeInfo *bmpUeInfo,
U8        hoMsgType,
U8        causeType,
U8        causeVal,
U8        lclCauseVal
)
#else /* ANSI */
PRIVATE S16 czBmpSndErrIndToPeer(peerCb, evnt, bmpUeInfo, hoMsgType, 
                                 causeType, causeVal, lclCauseVal)
CzPeerCb  *peerCb;       
CztEvnt   *evnt;         
CztUeInfo *bmpUeInfo;
U8        hoMsgType;  
U8        causeType;
U8        causeVal;
U8        lclCauseVal;
#endif /* ANSI */
{
   CztEvnt *rspErrEvnt = NULLP;
   U16     oldUEX2APIdVal = CZ_UNKNOWN_UE_X2AP_ID;
   U16     newUEX2APIdVal = CZ_UNKNOWN_UE_X2AP_ID;
   S16     ret = ROK;

   CZ_TRC2(czBmpSndErrorIndToPeer)

   /* Increment the incoming error counter for this message resulted in 
   error */
   /* czUtlUpdInErrSts(peerCb, hoMsgType); */

   if(NULLP != bmpUeInfo)
   {
      oldUEX2APIdVal = bmpUeInfo->oldUEX2APId;
      newUEX2APIdVal = bmpUeInfo->newUEX2APId;
   }

   rspErrEvnt = czUtlGenPeerErrInd(&(evnt->pdu), hoMsgType, 
                                   ((CzMsgInfo *)bmpUeInfo),
                                   causeType, causeVal);

   if(NULLP != rspErrEvnt)
   {
#ifdef ALIGN_64BIT
      CZ_DBG_INFO((CZ_PRNT_BUF, "Generating Error Indication "\
                  "on %d to peer(%d) with UE(%d:%d) on stream(%d)\n",\
                   hoMsgType, (peerCb->peerId), oldUEX2APIdVal, newUEX2APIdVal,\
                   (peerCb->peerCfg.globalStreamId)));
#else /* ALIGN_64BIT */
      CZ_DBG_INFO((CZ_PRNT_BUF, "Generating Error Indication "\
                  "on %d to peer(%ld) with UE(%d:%d) on stream(%d)\n",\
                   hoMsgType, (peerCb->peerId), oldUEX2APIdVal, newUEX2APIdVal,\
                   (peerCb->peerCfg.globalStreamId)));
#endif /* ALIGN_64BIT */

      /* Increment the outgoing counter for this message */
      czUtlUpdOutSts(peerCb, CZT_MSG_ERR_IND);

      /*******************************************************/
      /* Invoke a certain function in Transport module to send it out to peer */
      /*******************************************************/
      ret = czTpmSndToLower(peerCb, &(rspErrEvnt->pdu), 
                            (peerCb->peerCfg.globalStreamId));

      if(ROK != ret)
      {
#ifdef ALIGN_64BIT
         CZ_DBG_FATAL((CZ_PRNT_BUF, "Sending Error Indication"\
                "to peer(%d) failed on UE(%d:%d)\n",\
                (peerCb->peerId), oldUEX2APIdVal, \
                newUEX2APIdVal));
#else /* ALIGN_64BIT */
         CZ_DBG_FATAL((CZ_PRNT_BUF, "Sending Error Indication"\
                "to peer(%ld) failed on UE(%d:%d)\n",\
                (peerCb->peerId), oldUEX2APIdVal, \
                newUEX2APIdVal));
#endif /* ALIGN_64BIT */

         /* Increment the error outgoing counter for this message */
         czUtlUpdOutErrSts(peerCb, CZT_MSG_ERR_IND);
      }
      CZ_FREEEVNT(rspErrEvnt);
   }
   else /* Error Indication cannot be built */
   {
#ifdef ALIGN_64BIT
      CZ_DBG_ERROR((CZ_PRNT_BUF, "Building Error Indication"\
                   "to peer(%d) failed on UE(%d:%d)\n",\
                (peerCb->peerId), oldUEX2APIdVal, \
                newUEX2APIdVal));
#else /* ALIGN_64BIT */
      CZ_DBG_ERROR((CZ_PRNT_BUF, "Building Error Indication"\
                   "to peer(%ld) failed on UE(%d:%d)\n",\
                (peerCb->peerId), oldUEX2APIdVal, \
                newUEX2APIdVal));
#endif /* ALIGN_64BIT */
      /* Increment the error outgoing counter for this message */
      czUtlUpdOutErrSts(peerCb, CZT_MSG_ERR_IND);
   }

   if(CZT_ERRCAUSE_MSG_NOT_DELIVERED == lclCauseVal)
   {
      CZ_FREEEVNT(evnt);
      CZ_RETVALUE(ROK); /* All done in this function */
   }
      
   /* Inform the user too that event has occurred w.r.t peer
      Invoke a certain function in UIM to generate this */
   CZ_BMP_FILL_UIM_ERR_INFO(peerCb, CZT_ERRTYPE_FROM_PEER,
                            lclCauseVal, hoMsgType, 
                            oldUEX2APIdVal, newUEX2APIdVal);

   ret = czUimSndErrInd (peerCb->cztSapCb, &czBmpErrInd, evnt, NULLP); 
   if(ROK != ret)
   {
      CZ_FREEEVNT(evnt);
   }

   CZ_RETVALUE(ROK); /* All done in this function */

} /* End of function czBmpSndErrIndToPeer */

/* cz001.301 : Removed the function czBmpIsHoReqValid
 * Reason: The function of the czBmpIsHoReqValid is mainly to 
 * validate the Intigratity and encryption Algorithm, These checks 
 * has to be done by the application not the X2AP. So as this is Redundant code.*/

/** @} */

/**********************************************************************
  
         End of file:     tr_bmp.c@@/main/2 - Tue Aug 30 18:36:09 2011
   
**********************************************************************/
 
/**********************************************************************
        Revision History:

**********************************************************************/
/********************************************************************90**
 
     ver       pat    init                  description
------------ -------- ---- ----------------------------------------------
/main/1      ---        sv               1. LTE-X2AP Initial Release.
            cz001.101   mm               1. CR Fixes, Memory Leak fixes.
                                         2. BMP accepts messages in 
                                            CZ_GP_CONNECTING state.
/main/2      ---        pt               1. LTE-X2AP 3.1 release.
/main/3   cz001.301  akaranth            1. Fix for the build warnings.
                                         2. Removed the function czBmpIsHoReqValid.
                                            as it is redundant.
*********************************************************************91*/
