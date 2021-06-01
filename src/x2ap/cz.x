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
  
     Type:     C include file
  
     Desc:     Defines required by LTE X2AP.

     File:     cz.x

     Sid:      cz.x@@/main/2 - Tue Aug 30 18:36:05 2011

     Prg:      

**********************************************************************/

/*
*     The defines in this file correspond to structures and variables 
*     used by the following TRILLIUM software:
*
*     part no.             description
*     --------     ----------------------------------------------
*     1000373        LTE X2AP
*
*/


#ifndef __CZX__
#define __CZX__


#ifdef __cplusplus
EXTERN "C" {
#endif /* __cplusplus */

/** @enum czDir
        Direction of the messages.
 */
typedef enum czDir
{
   CZ_DIR_OUT = 0, /**< User Event */
   CZ_DIR_IN,    /**< Network Event */
   CZ_DIR_IN_OUT  /**< handles crossover of Setup/Reset Requests */
}CzDir;

/** @enum czProcDir 
   Direction of the procedures.
 * This new attribute of direction is to make sure both the incoming and
 * outgoing requests are responded before changing to the next state */
typedef enum czProcDir
{
   CZ_TGT_SETUP = 1, /**< Incoming Setup */
   CZ_SRC_SETUP = 2, /**< Outgoing Setup */
   CZ_TGT_RESET = 4, /**< Incoming Reset */
   CZ_SRC_RESET = 8  /**< Outgoing Reset */
}CzProcDir;

/** @enum czEnbType 
    This enum represents X2AP eNodeB type based on
    triggering Handover Request sent from this eNodeB(SRC) or received
    from peer eNodeB(TGT). */
typedef enum czEnbType
{
   CZ_SRC = 1,     /**< Source eNodeB */
   CZ_TGT,         /**< Target eNodeB */
   CZ_UNKNOWN_NODE /**< Unknown eNodeB type */
}CzEnbType;

/** @enum czProcType 
    X2AP Procedural categories */
typedef enum czProcType
{
   CZ_GP = 0,      /**< Global Procedure */
   CZ_BMP,         /**< Basic Mobility Management Procedure */
   CZ_ERR,         /**< Error Indication Procedure */
   CZ_PRIV,        /**< Private Message Procedure */
   CZ_PROC_UNKNOWN /**< Unknown Procedure */
}CzProcType;

/*******************************************************/
/* Basic Mobility Management Module */
/*******************************************************/

/** @ingroup bmpmodule 
    @enum czBmpEvnt 
    Basic Mobility Management Events */
typedef enum czBmpEvnt
{
   CZ_EVNT_HO_REQ = 0,        /**< Handover Request Event */
   CZ_EVNT_HO_REQ_ACK,        /**< Handover Request Acknowledge Event */
   CZ_EVNT_HO_PREP_FAIL,      /**< Handover Preparation Failure Event */
   CZ_EVNT_HO_CANCEL,         /**< Handover Cancel Event */
   CZ_EVNT_SN_STATUS_TRFR,    /**< SN Status Transfer Event */
   CZ_EVNT_UE_CXT_RELEASE     /**< UE Context Release Event */
}CzBmpEvnt;

/** @ingroup bmpmodule
    @enum czBmpState 
   Basic Mobility Management States */ 
typedef enum czBmpState
{
   CZ_BMP_HO_IDLE = 0,  /**< UE Handover in Idle */
   CZ_BMP_HO_PROGRESS,  /**< UE Handover in Progress */ 
   CZ_BMP_HO_PREPARED   /**< UE Handover in Prepared */
}CzBmpState;

/*******************************************************/
/* End Of Basic Mobility Management Module */
/*******************************************************/

/*******************************************************/
/* Global Procedures  Module */
/*******************************************************/

/**
   @ingroup gpmmodule
 * @enum czGpmEvent 
    Global Procedures Events */
typedef enum czGpmEvent
{
    CZ_EVNT_ERR_IND = 0,        /**< Error Indication Event */
    CZ_EVNT_SETUP_REQ,          /**< X2 Setup Request Event */
    CZ_EVNT_SETUP_RSP,          /**< X2 Setup Response Event */
    CZ_EVNT_SETUP_FAIL,         /**< X2 Setup Failure Event*/
    CZ_EVNT_RESET_REQ,          /**< Reset Event */
    CZ_EVNT_RESET_RSP,          /**< Reset Response Event */
    CZ_EVNT_ENB_CFG_UPD_REQ,    /**< eNB Configuration Update Request Event */
    CZ_EVNT_ENB_CFG_UPD_ACK,    /**< eNB Configuration Update Acknowledge Event */
    CZ_EVNT_ENB_CFG_UPD_FAIL,   /**< eNB Configuration Update Failure Event */
    CZ_EVNT_LOAD_IND,           /**< Load Information Event */
    CZ_EVNT_RES_STA_REQ,        /**< Resource Status Request Event */
    CZ_EVNT_RES_STA_RSP,        /**< Resource Status Response Event */
    CZ_EVNT_RES_STA_FAIL,       /**< Resource Status Failure Event */
    CZ_EVNT_RES_STA_UPDATE,     /**< Resource Status Update Event */
    CZ_EVNT_MOB_CHANGE_REQ,
    CZ_EVNT_MOB_CHANGE_ACK,
    CZ_EVNT_MOB_CHANGE_FAIL,
    CZ_EVNT_RLF_INDICATION,
    CZ_EVNT_HO_REPORT,
    CZ_EVNT_CELL_ACTIVATION_REQ,
    CZ_EVNT_CELL_ACTIVATION_RES,
    CZ_EVNT_CELL_ACTIVATION_FAIL,
    CZ_EVNT_PRIVATE,            /**< X2AP Private Message Event */
    CZ_EVNT_MAX                 /**< X2AP Maximum Global Procedural Events */
}CzGpmEvent; 

/**
   @ingroup gpmmodule
   @enum czGpStates 
    State for the GP FSM which represents the X2 interface
                      status towards a peer eNodeB.
*/
typedef enum czGpStates
{
   CZ_GP_IDLE,             /**< Default state, X2 interface is down */
   CZ_GP_CONNECTING,       /**< Setup/Reset request is initiated from user or peer. */
   CZ_GP_CONNECTED,        /**< The X2 interface is up and is used 
                                for handovers and other global procedures. */
   CZ_GP_STATE_MAX         /**< X2AP Maximum Global Procedural State */
}CzGpStates;

/*******************************************************/
/* Resource Status Handling  */
/*******************************************************/

/**
 @ingroup ldmmodule
 * @enum czRsrcState 
  Resource status procedure states
 */
typedef enum czRsrcState
{
   CZ_RSRC_IDLE = 0,
   CZ_RSRC_START_PROGRESS,
   CZ_RSRC_READY,
   CZ_RSRC_STOP_PROGRESS
}CzRsrcState;

/**
 @ingroup ldmmodule
 * @enum czRsrcEnbType 
   @brief This indicates which eNodeB, we are? eNodeB1 or eNodeB2.
 */
typedef enum czRsrcEnbType
{
   CZ_ENB1 = 0x10000,
   CZ_ENB2 = 0x20000
}CzRsrcEnbType;

/** 
 @ingroup ldmmodule
 @struct czRsrcInfo
  @brief  This structure contains the Resource Information
  */
typedef struct czRsrcInfo
{
   U16  eNb1measId;        /**< eNodeB Measurement Id 1*/
   U16  eNb2measId;        /**< eNodeB Measurement Id 2*/ 
}CzRsrcInfo;


/*******************************************************/
/* End of Resource Status Handling */
/*******************************************************/

/** 
   @ingroup gpmmodule
 * @struct czGpmCb
    @brief The CzGpmCb maintains the details of an X2 interface towards a peer
    from this eNodeB.
    */
typedef struct czGpmCb
{
   Bool      procPending;    /**< Keeps track of all the incoming/outgoing
                               Setup/Reset Procedures. */
   Bool      wait;           /**< TRUE if the timer is running to wait to send:
                                    - The next eNB Config Update Request or
                                    - The next X2 Setup Request depending on the
                                      peer state */
   CmTimer   gpTimer;        /**< GpTimer used based on TimeToWait IE Value
                                   received in X2 Setup Failure or eNB Config Uupdate
                                   Failure message received from peer. */
   Buffer    *retxBuf;       /**< Stored encoded buffer for retransmission OR
                                  pending Setup Message PDU while the association
                                  is created.
                                  TPM stotes the outgoing Setup Message PDU
                                  while the association is created. It is sent
                                  after association is established.
                                  This is used and sent when we receive a 
                                  retransmission request from user for pending
                                  procedures:
                                   - X2 Setup 
                                   - eNB Configuration Update */
   U8        retxMsgType;    /**< Message type of the GP message stored for
                                   retransmission */
}CzGpmCb;

/**@struct czEndpCb 
   @brief X2AP end point control block.
   @details This structure defines the end point  parameters present in the
   end point configration structure. Also this end point control block will be
   uppdated for the sate one after getting the end point open confirmation from
   sctp*/

typedef struct czEndpCb
{
   CmHashListCp    assocCp;         /**< SU assoc Id based associations HL */
   UConnId         locSpEndpId;     /**< SCTP allocated SP end point Id */
   UConnId         locSuEndpId;     /**< X2AP allocated SU end point Id */
   State           state;           /**< End point state */
   CztPort         port;            /**< SCTP port for this end point */
   CzNetAddrLst    srcAddrLst;      /**< Source address list */
}CzEndpCb;

/**@struct czSctSapCb 
   @brief X2AP Sap Control block.
   @details This structure stores the sct sap parameters configured during the sctp
   configarton . Also this sct sap control block will be
   uppdated for the sate one after getting the bind confirmation from
   sctp
*/
typedef struct czSctSapCb           
{
   State         state;             /**< State of this SAP */

   SuId         suId;               /**< X2AP SAP ID 
                                         allowed values 0 to maximum 
                                         service provider SAPs minus 1. */
   SpId          spId;              /**< SCTP service provider SAP ID.*/
   Pst           pst;               /**< Service user post structure */
   Ent           contEnt;           /**< Controlling entity */
   U32           trcMask;           /**< Trace Mask */
   S32           trcLen;            /**< Length of the data buffer to be traced.
                                         Range is 1 to 32676.*/
   U8            maxBndRetry;       /**< Maximum number of bind retries allowed */   
   TmrCfg        tBndTmr;           /**< Bind timer configuation. */  
   U8            retryCnt;          /**< Interface retry count */
   CmTimer       timers[CZ_MAX_SCT_SAP_TMRS]; /**< Max simultenous SCTP SAP timers. */
   Bool          cfgDone;            /**< To detect reconfiguration. */
   CzEndpCb      endpCb;             /**< End point control block reference */
   UConnId       nxtSuAssocId;       /**< X2AP SU association Id increment for
                                       per association*/
}CzSctSapCb;

/**@struct czCztSapCb 
   @brief X2AP upper Sap Control block.
   @details This structure stores the CZT SAP parameters configured during the SCTP
   configuration .Also this CZT SAP control block will be
   updated for the state before sending the confirmation to the X2AP User.
*/

typedef struct czCztSapCb           /**< X2AP Upper SAP CB */
{
   SuId         suId;               /**< Service User Id */
   SpId         spId;               /**< Service provider Id */
   Pst          pst;                /**< Service user post structure */
   State        state;              /**< State of this SAP */
   Bool         resCong;            /**< Resource congestion state */
}CzCztSapCb;

/** @struct czPeerCbCfg
    @brief This structure represents Trillium LTE X2AP's
    peer eNodeB's configuration control block.
    @details It stores all the information required to configure a peer eNodeB. */
typedef struct czPeerCbCfg
{
   Bool           isDynamCfgd;      /**< To indicate if this peer can be 
                                         dynamically configured.  */
   U16            eAValue;          /**< Encryption Algoritham supported by us
                                        for this peer. */
   U16            iPAValue;         /**< Integrity Algoritham supported by us
                                        for this peer. */
   U32            trcMask;          /**< Trace Mask for a peer configured by
                                         Layer Manager. This is useful to trace 
                                         the information for a peer. */
   S32            trcLen;           /**< Trace Length for a peer configured by
                                         Layer Manager. This is useful to trace 
                                         the information for a peer. */
   CztStrmId      globalStreamId;   /**< Stream Id for common / global procedures. */ 
   CzPeerNodeSts  peerSts;          /**< Peer statistics */
   CmNetAddr      priDstAddr;       /**< Peer's primary destination address */
}CzPeerCbCfg;

typedef struct czAssocCb CzAssocCb;  

/** @struct czPeerCb
    @brief This structure represents Trillium LTE X2AP's
    peer eNodeB's control block.
    @details It stores all the information required for a peer eNodeB processing
    on a X2 interface. */
typedef struct czPeerCb                  
{
   CmHashListEnt  hashEnt;       /**< Peer Hashlist Entry */
   U32            peerId;        /**< An unique peer identification. */
   U8             state;         /**< State of the X2 Interface with the peer. */
   CzPeerCbCfg    peerCfg;       /**< All configuration related to this peer. */
   CzCztSapCb     *cztSapCb;     /**< Upper CZT SAP reference for this peer. */
   CzSctSapCb     *sctSapCb;     /**< Lower SCT SAP reference for this peer. */
   CzAssocCb      *assocCb;      /**< Pointer to SCTP association for this peer. */
   U16            nextStreamId;  /**< Next stream for UE association signaling 
                                      procedures.*/
   CzGpmCb        gpmCb;         /**< Global Procedure Module controlling the
                                      X2 interface global procedures. */
   CmHashListCp   ueCbHlCp;      /**< Basic Mobility Procedure Module controlling
                                      UE involved in mobility management over X2. */ 
   CmHashListCp   resrcCbHlCp;   /**< Load Management handling controlled by Global
                                      Procedure Module. */
   Bool           delPcb;        /**< Bool type */
}CzPeerCb;

/** @struct czAssocCb 
    @brief This structure represents Association control block */
struct czAssocCb               
{
   UConnId         spAssocId;          /**< This parameter specifies the service
                                            providers association ID*/
   UConnId         suAssocId;          /**<  This parameter specifies the
                                              service user association ID.*/
   State           assocState;         /**< This parameter specifies the SCTP
                                            association status.*/
   CzPeerCb        *peer;              /**< Pointer to peer control block */
   CzAssocCfg      *assocCfg;          /**< Pointer to association configuration block */
};


/** @ingroup ldmmodule 
    @struct czRsrcCb 
    @brief This structure represents Resource Control block.
    @details The context and scope of resource control block is always within a peer
    eNodeB control block for an established X2 interface with the same peer.
 */
typedef struct czRsrcCb 
{
   CmHashListEnt actRsrcEnt;    /**< Hashlist manipulator */
   U32           rsrcCbKey;     /**< Resource update control block key */
   U16           eNb1MeasId;    /**< E-NodeB1 Measurement Id */
   U16           eNb2MeasId;    /**< E-NodeB2 Measurement Id */
   CzRsrcState   state;         /**< State of this resource cb */
   CzPeerCb      *peerCb;       /**< Peer eNodeB reference for this UE 
                                     involved in mobility. */
   Buffer        *retxBuf;      /**< Stored encoded buffer for retransmission.
                                     This is used and sent to peer when we 
                                     receive a retransmission request from peer
                                     for pending Resource Status Reporting
                                     initiation start or stop procedure */
}CzRsrcCb;

/*******************************************************/
/* End of Global Procedure Module */
/*******************************************************/

/** @struct czIeInfo
  @brief This structure contains the IE Information. 
  @details This is useful in building the
  failure response or error indication to peer if incoming message from peer
  results in decoding or logical errors in processing the same.
  */
typedef struct czIeInfo
{
   Bool pres;        /**<  Whether Present or not */
   U8   ieId;        /**<  IE ID */ 
   U8   crit;        /**<  IE Criticality */ 
   U8   typOfErr;    /**<  Type of Error */ 
}CzIeInfo;
 
typedef CztUeInfo CzMsgInfo;   /**< X2AP Message Information */

/*******************************************************/
/* Basic Mobility Management Module */
/*******************************************************/

/** 
    @ingroup bmpmodule
    @struct czUeCb 
    @brief This structure represents an UE Control block.
    @details The context and scope of UE control block is always within a peer
    eNodeB control block for an established X2 interface with the same peer.
    The UE control block is used only during the basic mobility management
    procedure[handover management] carried over X2 interface. 
 */
typedef struct czUeCb 
{
   CmHashListEnt actUeEnt;      /**< Hashlist manipulator */
   U32           ueCbKey;       /**< Active UE control block key */
   U8            eNbType;       /**< Target or Source eNodeB involved in mobility
                                     of an UE. */
   U16           oldUEX2APId;   /**< Old X2AP Identifier from source
                                     eNodeB for an UE involved in mobility. */
   U16           newUEX2APId;   /**< New X2AP Identifier from target eNodeB 
                                     for an UE involved in mobility. */
   U8            state;         /**< State of this UE involved in mobility
                                     procedure. */
   CzPeerCb      *peerCb;       /**< Peer eNodeB reference for this UE 
                                     involved in mobility. */
   CmTimer       bmpTimers[CZ_BMP_MAX_TIMERS];  /**< Mobility Management Timers:
                                    -Timer TRelocprep to monitor UE handover 
                                     preparation.
                                    -Timer TX2RelocOverall to monitor UE handover
                                     completion. */
   CztStrmId     streamId;      /**< SCTP stream id to use with all the mobility
                                     management messages involved.  */
}CzUeCb;

/*******************************************************/
/* End of Basic Mobility Management Module */
/*******************************************************/

/*******************************************************/
/* Dynamic Peer Control */
/*******************************************************/

typedef struct czDynamPeerAssocEntry
{
   CmNetAddr       peerAddr;        /**< Net address of the dynamic peer */
   CztPort         port;            /**< Port of the dynamic peer */
} CzDynamPeerAssocEntry;

typedef struct czDynamPeerAssocCb
{
   CmHashListEnt           hashEnt;        /**< Transient peerCb Hashlist
                                               Entry for dynamic peers */
   CzDynamPeerAssocEntry   peerAssocEntry; /**< Hash list entry for
                                               Association */
   CzPeerCb               *peerCbPtr;      /**< Pointer to the peerCb */
   SctAssocIndParams      assocParams;
} CzDynamPeerAssocCb;

/*******************************************************/
/* End Of Dynamic Peer Control */
/*******************************************************/

/** @struct czEnbCb 
    @brief eNodeB Control block. 
    @details This structure represents LTE eNodeB on X2 Interface. */

typedef struct czEnbCb          
{
   CzProtCfg      protCfg;        /**< Node / Protocol configuration. */
   Bool           cfgDone;        /**< To indicate if the node configuration 
                                      is completed or not. */
   CmHashListCp   peerCbLst;      /**< Hash list of peer control blocks. */

   CmHashListCp   dynamPeerAssocLst;  /**< Hash list of dynamic peer addresses and
                                         port for configuring dynamic peers. */
}CzEnbCb;


/** @struct _czCb 
    @brief This structure represents Trillium LTE X2AP layer.
    @details This is a global control block. 
*/
 
typedef struct _czCb
{
   TskInit         init;               /**< Layer task initialization info. */
   CzGenCfg        genCfg;             /**< Layer general configuration */
   CzEnbCb         nodeCb;             /**< Layer node configuration */
   CzCztSapCb      **cztSapLst;        /**< Layer pointers to upper SAPs */
   CzSctSapCb      **sctSapLst;        /**< Layer pointers to lower SAPs */
   CmTqCp          czGenTqCp;          /**< Layer general timing queue control point */
   CmTqType        czGenTq[CZ_TQSIZE]; /**< Layer general timing queue */
   CmTqCp          czLyrTqCp;          /**< Timing queue control point */
   CmTqType        czLyrTq[CZ_TQSIZE]; /**< Timing queue */
   CzGenSts        genSts;             /**< Layer general statistics */
   CzGenSta        genSta;             /**< Layer general status */
   Bool            shutdown;           /**< Layer shutdown control */
#ifdef SS_MULTIPLE_PROCS
   U8            used;                 /**< Whether Used or not */
#endif /* endif SS_MULTIPLE_PROCS */
}CzCb;

/* Declaration of Global CzCb */
#ifdef SS_MULTIPLE_PROCS
   EXTERN CzCb czCbLst[CZ_MAX_INSTANCES];
   EXTERN CzCb *czCbPtr;
#else /* SS_MULTIPLE_PROCS */
   EXTERN CzCb czCb;
#endif /* SS_MULTIPLE_PROCS */


/* Layer Active timer */
/* Expiry timer  */
EXTERN Void czProcLyrTmrExpiry ARGS(( Ptr      cb, S16 tmrEvnt));

#ifdef SS_MULTIPLE_PROCS
   EXTERN S16 czActvTmr ARGS((ProcId proc,Ent ent, Inst inst));
#else /* else SS_MULTIPLE_PROCS */
   EXTERN S16 czActvTmr ARGS((Void));
#endif /* endif SS_MULTIPLE_PROCS */

EXTERN S16 czLmmGetSId ARGS((SystemId *s));

/* cz001.101 : Correction in usage of trace Flag */
/* to send Trce to layer manager */
EXTERN S16 czLmmSendLmTrc    ARGS((S32 trcLen, CzTrc *trcParam, Buffer *mBuf));

EXTERN S16 czRegInitTmr ARGS((Void));   


/*******************************************************/
/* Node Management Module */
/*******************************************************/
/* This function is used to find the PeerCB */ 
EXTERN S16 czNmmFndPeer ARGS((U32     peerId, CzPeerCb **peerCb));        

/* This function is used  to Initialize the PeerList*/
EXTERN S16 czNmmInitPeerLst ARGS((Void));

/* This Function is Called to allocate an available peer control block*/
EXTERN S16 czNmmAllocPeerCb ARGS((U32  peerId, CzPeerCb  **peerCb));

/* This Function is used to deallocate the Memory for the PeerCB*/
EXTERN S16 czNmmDeallocPeerCb ARGS((CzPeerCb *peer));

/* This Function is used to deallocate the Memory for alll the PeerCB*/
EXTERN S16 czNmmDeallocAllPeerCb ARGS((SpId sapId, U8 opn));

/* This Function is used to disable the Memory for the PeerCB*/
EXTERN S16 czNmmDisablePeer ARGS((CzPeerCb *peer, Bool assocTerm));

/* This Function is used to reset the PeerCB*/
EXTERN Void czNmmResetPeer ARGS((CzPeerCb *peer));

/* This function allocates and adds a new Resource CB from the hash list within peerCb
   using ueCbKey as the key */
EXTERN S16 czNmmAddRsrcCb ARGS((CzPeerCb *peerCb, 
                                U32       rsrcCbKey, 
                                CzRsrcCb  **rsrcCb, 
                                U16       eNb1MeasId));

/* This function finds an Resource CB from the hash list within peerCb using rsrcCbKey
   as the key */
EXTERN S16 czNmmFindRsrcCb ARGS((CzPeerCb *peerCb, 
                                 U32      rsrcCbKey, 
                                 CzRsrcCb **rsrcCb));        

EXTERN S16 czUbndCztSap ARGS((SpId  sapId));        

/* This function removes and deallocates an entry of an Resource CB from the hash list
   within peerCb using rsrcCbKey as the key */
EXTERN S16 czNmmDelRsrcCb ARGS((CzRsrcCb   *rsrcCb));        

/* To Shutdown the X2AP layer */
EXTERN S16 czLmmShutdown     ARGS((Void));

/* This function allocates and adds a new UE CB the hash list within peerCb
   using ueCbKey as the key */
EXTERN S16 czNmmAddUeCb ARGS((CzPeerCb *peerCb, 
                              U32      ueCbKey, 
                              CzUeCb   **ueCb, 
                              U16      oldUEX2APId));

/* This function finds an UE CB from the hash list within peerCb using ueCBKey
   as the key */
EXTERN S16 czNmmFindUeCb ARGS((CzPeerCb *peerCb, U32      ueCbKey, 
                               CzUeCb   **ueCb));        

/* This function removes and deallocates an entry of an UE CB from the hash list
   within peerCb using ueCBKey as the key */
EXTERN S16 czNmmDelUeCb ARGS((CzUeCb  *ueCb));        

/* This function Matched the Peer based on Address */
EXTERN CzPeerCb* czNmmMatchPeer ARGS((CztNetAddrLst *dstNAddrLst, 
                                                     SctPort port));

EXTERN S16 czNmmHdlErrInd ARGS((CzPeerCb *peerCb,
                                CztEvnt  *evnt, 
                                U8       msgDirection));

/*******************************************************/
/* Dynamic Peer(s) Control by NMM */
/*******************************************************/

EXTERN S16 czNmmAllocDynamPeerCb ARGS((SuId     suId, 
                                       CzPeerCb **peer));

EXTERN S16 czNmmDeallocDynamPeerCb ARGS((CzPeerCb *peer));

EXTERN S16 czNmmFindDynamPeerAssocCb ARGS((CztNetAddrLst        *dstAddrLst, 
                                           CztPort             dstPort,
                                           CzDynamPeerAssocCb  **peerAssoc));


EXTERN S16 czNmmRemDynamPeerAssoc ARGS((CzPeerCb *peer));

/*******************************************************/
/* End of Dynamic Peer(s) Control by NMM */
/*******************************************************/

/*******************************************************/
/* Global Procedures Module */
/*******************************************************/

EXTERN S16 czGpmFsmMsgHandler ARGS((CzPeerCb *peerCb, CztEvnt *pdu, U8 msgType,
                                                                      U8 dir));

EXTERN S16 czGpmWaitTmrHdl ARGS((CzPeerCb *peerCb));

EXTERN S16 czGpmRetransmit ARGS((CzPeerCb *peerCb, U8 msgType, U16 transId));

EXTERN S16 czGpmHdlErrInd ARGS((CzPeerCb  *peerCb,
                                U8         msgType,
                                CztEvnt    *event,
                                U8         eventDir));    

EXTERN S16 czGpmHdlRsrcRetxReq ARGS((CzPeerCb* peerCb, U16 eNB1MeasID));

EXTERN S16 czGpmRsrcFsmErr ARGS((CzPeerCb* peerCb, U8 msgType,
                                 CztEvnt* event,U8 dir));

EXTERN S16 czGpmHdlLdInd ARGS((CzPeerCb* peerCb, U8 msgType,
                               CztEvnt* event,U8 dir));

EXTERN S16 czGpmHdlRsrcStsReq ARGS((CzPeerCb* peerCb, U8 msgType,
                                    CztEvnt* event,U8 dir));

EXTERN S16 czGpmHdlRsrcStsRsp ARGS((CzPeerCb* peerCb, U8 msgType,
                                    CztEvnt* event, U8 dir));

EXTERN S16 czGpmHdlRsrcStsFailr ARGS((CzPeerCb* peerCb, U8 msgType,
                                      CztEvnt* event, U8 dir));

EXTERN S16 czGpmHdlRsrcStsUpdt ARGS((CzPeerCb* peerCb, U8 msgType,
                                     CztEvnt* event, U8 dir));

/*******************************************************/
/* End of Global Procedures Module */
/*******************************************************/


/*******************************************************/
/* Upper API Related Functions */
/*******************************************************/

EXTERN S16 czUimSndGpmInd ARGS((CzPeerCb *peerCb, CztEvnt   *event));        

EXTERN S16 czUimSndGpmCfm ARGS((CzPeerCb *peerCb, CztEvnt   *event));        

EXTERN S16 czUimSndBmpInd ARGS((CzPeerCb *peerCb, CztEvnt   *event));        

EXTERN S16 czUimSndBmpCfm ARGS((CzPeerCb *peerCb, CztEvnt   *event));        

EXTERN S16 czUimSndGpErrInd ARGS((CzPeerCb *peerCb, CztEvnt   *event)); 

EXTERN S16 czUimSndStaInd ARGS((CzCztSapCb *sapCb, CztStaInd  *staInd)); 

EXTERN S16 czUimSndPeerAssocInd ARGS((CzPeerCb *peerCb, 
                                             CztPeerAssocInd   *event)); 

EXTERN S16 czUimSndErrInd  ARGS((CzCztSapCb  *sapCb, CztLclErrInd   *errInd,
                                 CztEvnt     *event,   Buffer       *mBuf)); 

/*******************************************************/
/* End of Upper API Related Functions */
/*******************************************************/


/*******************************************************/
/* Timer Module */
/*******************************************************/

/** 
     @ingroup tmrmodule
     @enum CzTmrType
     X2AP Timers */
typedef enum
{
   CZ_TMR_TIME_TO_WAIT = 1,  /**< TimeToWait timer received in X2AP Global
                                  failure response. */
   CZ_TMR_TRELOCPREP,        /**< TRelocprep timer to track handover preparation
                                  at source eNodeB. */
   CZ_TMR_TX2RELOCOVERALL,   /**< TX2RelocOverall timer to track the completion
                                  of successful handover at source eNodeB. */
   CZ_TMR_SCT_BIND            /**< Lower Interface bind timer to track the
                                  successful bind operation with lower SAP. */
}CzTmrType;

/* Public variable declarations */
EXTERN Void czGetSId ARGS((SystemId *s));
/* Timer Module APIs */
/* This function starts an identified and valid timer at X2AP */
EXTERN S16 czSchedGenTmr ARGS((S16 tmrEvent, Void  *cb, U8 action));        

/* This function handles an event on expiry of general timer 
   started earlier. */
EXTERN Void czProcGenTmrExpiry ARGS((Ptr  cb, S16 tmrEvnt));        
/* This function starts an identified and valid timer at X2AP */
EXTERN S16 czStartTmr ARGS((CzTmrType tmrType, U16 delay , Void  *tmrCb));        

/* This function stops an identified and valid timer at X2AP */
EXTERN S16 czStopTmr ARGS((CzTmrType tmrType,  Void  *tmrCb));        

/* This function handles an event on expiry of X2AP timer 
   started earlier. */
EXTERN S16 czProcTmrExpiry ARGS((Void  *tmrCb, S16 tmrEvnt));        

EXTERN S16 czStopUeTmrs ARGS((CzUeCb   *tmrCb));

/*******************************************************/
/* End of Timer Module */
/*******************************************************/

/*******************************************************/
/* Basic Mobility Management Module */
/*******************************************************/

/*
 * This function is the entry point for all the X2AP basic mobility
      procedural messages. 
      - This is invoked from UIM to process the outgoing X2AP mobility management
        messages.
      - This is invoked from TPM to process the incoming X2AP mobility management
        messages.
*/
EXTERN S16 czBmpHdlHoEvnt ARGS((CzPeerCb   *peerCb,
                                CztEvnt    *evnt,
                                U8          hoMsgType,
                                U8          msgDirectn));
 

/*
 * This function is the entry point for handling X2AP Error Indication
   for basic mobility management procedures.
      - This is invoked from UIM to process the outgoing X2AP Error Indication.
      - This is invoked from TPM to process the incoming X2AP Error Indication.
*/
EXTERN S16 czBmpHdlErrInd ARGS((CzPeerCb  *peerCb,
                                CztEvnt    *evnt,
                                U8         hoMsgType,
                                U8         msgDirectn));             
                                

/* This function handles the basic mobility management related timers'
    expiry. TMR module invokes this when it receives the respective
    timer fire. 
*/
EXTERN S16 czBmpHdlTmrEvnt ARGS((CzUeCb   *ueCb,
                                 S16      tmrEvnt));


/*******************************************************/
/* End of Basic Mobility Management Module */
/*******************************************************/

/*******************************************************/
/* Encoder-Decoder Module */
/*******************************************************/
EXTERN S16 czEdmEncode ARGS((Mem *mem, 
                          CztX2AP_PDU *x2apPdu, 
                          Buffer **mBuf));

EXTERN S16 czEdmDecode ARGS((CztEvnt **event, 
                          Mem    *sMem, 
                          Buffer *mBuf, 
                          CmPAsnErr  *pasnErr));

EXTERN S16 czGetIE  ARGS((CztX2AP_PDU *pdu, 
                          U32 id, 
                          TknU8 **ie));

/*********************************************************/
/* End of Encoder-Decoder Module and PASN Interface Control*/
/*********************************************************/

/* LIM Module */
EXTERN Void czLmmSendLmAlarm ARGS((U16 category,
                                U16 event,
                                U16 cause,
                                CzUstaDgn *dgn));

/* Lower interface module interfaces */

/*******************************************************/
/* Transport Module and Lower Interface control */
/*******************************************************/

EXTERN S16 czTpmSndStoredBuf ARGS((CzPeerCb   *peerCb,Buffer  *mBuf));
   
EXTERN S16 czTpmStoreAndSendMsg   ARGS((CzPeerCb *peerCb, 
                                        CztX2AP_PDU  *pdu,
                                        CztStrmId streamId, 
                                        Buffer  **mBuf));

EXTERN S16 czTpmSndToLower   ARGS((CzPeerCb *peerCb, 
                                   CztX2AP_PDU  *pdu,
                                   CztStrmId streamId));

EXTERN S16 czTpmRcvFromLower ARGS((Buffer *mBuf,
                                   CzPeerCb *peerCb));

EXTERN S16 czTpmSndSetUpReq  ARGS((CzPeerCb *peerCb, 
                                   Buffer *mBuf));

EXTERN S16 czTpmHndlTermInd  ARGS((SuId  suId, 
                                   UConnId  assocId,
                                   U8  assocIdType));

EXTERN S16 czTpmHndlStaInd ARGS((U32  peerId,
                                 SctStatus  status,
                                 SctCause   cause,           
                                 CmNetAddr        *dstNAddr,        
                                 Buffer           *mBuf));

EXTERN S16 czTpmTermAssoc ARGS((CzAssocCb  *assocCb, 
                                Bool  assocAbort));

EXTERN S16 czTpmHndlAssocInd ARGS((SuId  suId, 
                                   CzSctSapCb  *sctSapCb,
                                   SctAssocIndParams   *assocParams));


EXTERN S16 czTpmSndAssocReq  ARGS((CztX2AP_PDU *pdu, CzAssocCb  *assocCb));

EXTERN S16 czTpmDynamPeerConfig ARGS((SuId suId, 
                                         SctAssocIndParams  *assocParams));

/*******************************************************/
/* End Of Transport Module and Lower Interface control */
/*******************************************************/

/*******************************************************/
/* Utilities Module */
/*******************************************************/

/* This function generates an alarm to LM Parameters
  PeerCb responsible when encountered an error to generate an alarm.
 pdu which caused the error 
 msgDirection -> Direction to specify whether message received from peer or from user.
*/
EXTERN S16 czUtlGenAlarm ARGS((CzPeerCb *peerCb,
                               CztX2AP_PDU *pdu, 
                               U8 msgDirection));

EXTERN U8 czUtlGetMsgType ARGS((CztX2AP_PDU   *pdu,
                                 Bool         *isResponse,
                                 S16           *procType));


/* This function maps X2AP message to Trillium X2AP message using static
          map.
 */
EXTERN S16 czUtlGetMsgEvnt ARGS((CztX2AP_PDU   *pdu));

/*
 * This function retrieves the Trillium mapping of procedure code
          based on different types of X2AP messages.
 */
EXTERN S16 czUtlGetProcTypeVal ARGS((S16   procCode));

/*
 * This function retrieves the the IE indicated in "id". The IE 
 *        pointer is returned in "ie".
 */
EXTERN S16 czUtlGetIE ARGS(( CztX2AP_PDU *pdu, U32 id, TknU8 **ie));


/* This function generates a X2AP Error Indication message towards the peer. */
EXTERN CztEvnt* czUtlGenPeerErrInd ARGS((CztX2AP_PDU *pdu, 
                                             U8          msgType, 
                                             CzMsgInfo   *msgInfo,
                                             U8          causeType,
                                             U8          causeVal));

/* This function generates a HO Cancel message to send towards the peer. */
EXTERN CztEvnt* czUtlGenHoCancel ARGS((CztUeInfo  *ueInfo,
                                           U8        causeType,
                                           U8        causeVal));

/* This function generates a HO Preparation Failure response message to send
   towards the peer. */
EXTERN CztEvnt* czUtlGenHoPrepFailure ARGS((CzIeInfo *idLst, 
                                                CztUeInfo  *ueInfo,
                                                U8        causeType,
                                                U8        causeVal));

EXTERN S16 czUtlGetProcMsgCrit ARGS((CztX2AP_PDU  *pdu,
                                     U8           *procCode,
                                     U8           *msgType,
                                     U8           *criticality,
                                     U8           *msgId));

EXTERN CztEvnt* czUtlGenX2CfgUpdFailure ARGS((CztX2AP_PDU *pdu, 
                                              U8          causeType,
                                              U8          causeVal));

/* This function generates a Resource Status Failure message towards the peer */
/* The result pdu formed/ built. This is an output parameter 
   causeVal -> Cause Value to fill if valid for the respective message.
*/ 
EXTERN CztEvnt* czUtlGenRsrcStsFail ARGS((CzIeInfo *idLst, 
                                                   CzRsrcInfo  *rsrcInfo,
                                          U8  causeType,   U8   causeVal));

EXTERN CztEvnt* czUtlGenX2SetupFailure ARGS((CzIeInfo *idLst, 
                                             U8       causeType,
                                             U8       causeVal));

EXTERN CztEvnt* czUtlGenMobChgFailure ARGS((CzIeInfo *idLst,
                                             CztECGI   *eNB1CellEcgi,
                                             CztECGI   *eNB2CellEcgi,
                                             U8       causeType,
                                             U8       causeVal));

EXTERN CztEvnt* czUtlGenCellActFailure ARGS((CzIeInfo *idLst,
                                             U8       causeType,
                                             U8       causeVal));

EXTERN  S16 czUtlFillTknBStr32 ARGS((
                                      TknBStr32  *ptr,
                                       U16         len,
                                       U32         val));


EXTERN S16 czUtlGetStream (CzPeerCb  *peerCb, CztStrmId  *strmId);


EXTERN U8 czUtlGetCause(CztCause *causeIE);


/*****Statistics Update Utilities ****/
EXTERN S16 czUtlUpdInErrSts ARGS((CzPeerCb   *peer,
                                  U16        evntType)); 

EXTERN S16 czUtlUpdOutErrSts ARGS((CzPeerCb  *peer,
                                  U16        evntType)); 

EXTERN S16 czUtlUpdInSts ARGS((CzPeerCb   *peer,
                               U16        evntType)); 

EXTERN S16 czUtlUpdOutSts ARGS((CzPeerCb   *peer,
                                U16        evntType)); 

EXTERN Void czUtlFillCauseIe ARGS((CztCause *causeIe, 
                                   U8        causeType,
                                   U8        causeVal));
EXTERN S16 czUtlGetErrIdLst ARGS((CztX2AP_PDU       *pdu,
                                  U8                msgId,
                                  CzIeInfo          *ieId,
                                  CztUeInfo         *ueInfo));

EXTERN CztEvnt* czUtlSndPeerErrInd ARGS((Void  *pdu, U8  causeType,
                                                   U8   causeVal));

EXTERN CztEvnt* czUtlSndRspMsg ARGS((CztX2AP_PDU *pdu, CmPAsnErr   *pasnErr));

/*kworks fix: change type S16 to U8*/
EXTERN U8 czEvntMap[CZT_MSG_MAX];
EXTERN U8 czX2MsgMap[CZ_MAX_MSG_PROC_CODE][CZ_MAX_MSG_CHOICE_VAL];
EXTERN U8 czGetProcCrit[CZ_MAX_MSG_PROC_CODE];

/*******************************************************/
/* End Of Utilities Module */
/*******************************************************/


#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __CZX__ */


/**********************************************************************
  
         End of file:     cz.x@@/main/2 - Tue Aug 30 18:36:05 2011
   
**********************************************************************/

/**********************************************************************
        Revision History:

**********************************************************************/
/********************************************************************90**
 
     ver       pat    init                  description
------------ -------- ---- ----------------------------------------------
/main/1      ---        ds              1. LTE-X2AP Initial Release.
            cz001.101   mm              1. CR Fixes, Memory Leak fixes.
/main/2      ---        pt              1. LTE-X2AP 3.1 release.
*********************************************************************91*/
