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

     Type:     C include file

     Desc:     This file contains


     File:     wr_emm_pws.h

     Sid:      $SID$

     Prg:      Sriky

*********************************************************************21*/


#ifndef __WR_EMM_PWS_H__
#define __WR_EMM_PWS_H__

#ifdef __cplusplus
extern "C" {
#endif

#define WR_PWS_INVALID_TYPE  0x7f
#define WR_PWS_MS_UNIT                      1000    /*This macro defines the unit in millisecond.*/
#define WR_PWS_WRNGRESP_WITH_BRDCSTCOMPIE   3      /*This macro will be used when warning response
                                                     has to be sent with broadcast completed area list IE.*/
#define WR_PWS_WRNGRESP_WO_BRDCSTCOMPIE     2      /*This macro will be used when warning response 
                                                     has to be sent without broadcast completed area list IE.*/
#define WR_PWS_MAX_BRDCAST                  65535  /*This macro defines the maximum number of broadcast.*/ 

#define WR_PWS_INVALID_MSGID                65535  /*This macro defines default message ID.*/

#define WR_PWS_WARNMSG_SEGSIZE              90     /*Warning Message Segment Size in bytes*/

#define WR_PWS_ETWS                         1
#define WR_PWS_CMAS                         2
typedef RgrSegmentInfo WrPwsSegmentInfo;

/** @brief This structure is used to store the list of PDUs for each segment of the warning message. */
/*typedef struct wrPwsSegmentInfo
{
   CmLList     cmasSegPduLstLnk;
   Buffer*     pdu;
}WrPwsSegmentInfo;*/


typedef struct wrPwsMsgRepeatInfo
{
   U8          wrngType;
   PTR         wrngInfo;
   PTR         cellCb;
   CmTimer     repetitionTmr;
}WrPwsMsgRepeatInfo;

/** @brief This structure is used to store ETWS Secondary notification Information.
 */
typedef struct wrPwsWrngMsgInfo
{
   CmLListCp           segPduLst;         /*!< List of PDUs for all the segments of the secondary ETWS notification. */
   /*Buffer              *wrngMsgPtr;*/
   U8                    *wrngMsgPtr;       /*!< wrngMsgPtr: starting pointer of the secondary notification. */
   U32                   msgLen; 
   U8                  emergencyAreaType; /*!< 0-TAI, 1-CellIDs, 2-EAID, 3-For all active cells
                                             which means on S1 we haven't received EAList IE; */         
   U8                  dataCodingScheme;  /*!< dataCodingScheme:Identifies the alphabet or coding employed for the message
                                               characters.This value is transparently passed from EPC to UE. */
   U16                 repetitionPeriod;  /*!< Repetition Period unit  in Sec and
                                               range spcifies min is 1 sec and max ~1 HOUR. */
   S32                 numBroadcastReq;   /*!< numBroadcastReq:indicates the number of times
                                               a message is to be broadcast. */
   Bool                readyToSch;
   WrPwsMsgRepeatInfo  repetitionInfo;
}WrPwsWrngMsgInfo;

/** @brief This structure is used to store ETWS primary notification information.
 */
typedef struct wrPwsPrimEtwsInfo
{
   U8                  emergencyAreaType; /*!< 0-TAI, 1-CellIDs, 2-EAID, 3-For all active cells
                                             which means on S1 we haven't received EAList IE; */         
   U8                  wrngSecurity[50];  /*!< Warning Security:Specifies the security information
                                               needed for securing the Primary Notification. */
   U16                 warningType;       /*!< Warning Type:Specifies the disaster type 
                                               in the Primary Notification. */
   U16                 repetitionPeriod;  /*!< Repetition Period unit  in Sec and
                                               range spcifies min is 1 sec and max ~1 HOUR. */
   S32                 numBroadcastReq;   /*!< numBroadcastReq:indicates the number of times
                                               a message is to be broadcast. */
   Bool                readyToSch;
   Bool                wrngSecurityPres;
   WrPwsMsgRepeatInfo  repetitionInfo;
   Buffer              *pdu;    /* Encoded PDU for SIB10 */
}WrPwsPrimEtwsInfo;

/* Fix for ccpu00139096 */
/** @brief This structure is used to store Paging info, which can be
 * used while removing the paging entry, while concurrent ETWS is received.
 */
typedef struct wrEtwsPagInfo
{
   U16                 pf;   /*!< Paging Frame */
   U8                  po;   /*!< Paging Occusion */
}WrEtwsPagInfo;

/** @brief This enum specifies the type paging for warning message 
 */
typedef enum wrWaringPagingType
{
   WR_START_PAG_PRI_ETWS = 0,
   WR_START_PAG_SEC_ETWS,
   WR_START_PAG_ETWS,
   WR_STOP_PAG_ETWS,
   WR_MAX_WARING_PAG
}WrWaringPagingType;

/** @brief This structure is used to store PWS ETWS information.
 */
typedef struct wrPwsEtwsInfo
{
   U8                  notificationType; /*!< Notfication type: Specifies the type of notification needs to trigger 
                                              those values indicates 1-for primary, 2- for secondary, 3-both. */
   U16                 wrngMsgId;        /*!< Message Identifier:Used to identify the warning message.*/
   U16                 wrngSerialNum;    /*!< Serial Number:Specifies the particular message from
                                              the same source and alters when mesage identifier is changed. */
   WrPwsPrimEtwsInfo   primEtwsInfo;     /*!< ETWS Primary notification information. */
   WrPwsWrngMsgInfo    secEtwsInfo;      /*!< ETWS Secondary notification information. */
   /* Fix for ccpu00139096 */
   WrEtwsPagInfo       waringPagingInfo[WR_MAX_WARING_PAG]; /*!< ETWS Paging info*/
}WrPwsEtwsInfo;


/** @brief This structure is used to store PWS CMAS information.
 */
typedef struct wrPwsCmasInfo
{
   CmLList             cmasLstLnk;
   U32                 wrngMsgId;         /*!< Message Identifier: Used to identify the warning message.*/
   U32                 wrngSerialNum;     /*!< Serial Number: Specifies the particular message from
                                               the same source and alters when mesage identifier is changed. */
   Bool                waitToStop;
   WrPwsWrngMsgInfo    wrngMsgInfo;
   /*Counter for CMAS  ccpu00127868  */
   U16                 numberOfBroadcasts;         /*!< Message counter for Cell ID Cancelled, TAI Cancelled 
                       Emergency Area ID Cancelled .*/
}WrPwsCmasInfo;



typedef struct wrPwsSIBldInfo
{
   WrPwsCmasInfo       *crntCmas;
   U8                  segNum;
   Bool                isLastSeg;
   U16                 segLen;
   Buffer              *segment;
}WrPwsSIBldInfo;

/** @brief This structure is used to store transient information which can be
 * passed as to different functions as parameter.
 */
typedef struct wrPwsTransientInfo 
{
   Bool                      validCell[WR_MAX_CELLS];
   Bool                      broadCastCompletedIePres;
   Bool                      isDuplicate;
   Bool                      isEtwsPresent;
   Bool                      validSecEtws;
   U8                        wrngType;
   U16                       cellIdx;
   U8                        wrngToProcess;
   U16                       wrngSibType;
   U16                       wrngSiIdx;
   U16                       cfgType;
}WrPwsTranientInfo;


/** @brief This structure is used to store PWS information.
 */
typedef struct wrPwsWarningInfo
{
   WrPwsEtwsInfo  etwsInfo;   /*!< ETWS information */
   CmLListCp      cmasLst;    /*!< cmasLst: List of nodes needed to support multiple warning
                                   messages for CMAS implementatation. WrPwsCmasInfo*/
   WrPwsSIBldInfo siBldInfo;
   WrPwsCmasInfo  *currentCmas;
   Bool           isPrimEtwsInProgress;
   Bool           isSecEtwsInProgress;
   Bool           isCmasInProgress;
   U8             rcvdWrngType;
   U8             sib10SiIdx;
   U8             sib11SiIdx;
   U8             sib12SiIdx;
   Bool           tmrRunningForLastCmas;
}WrPwsWarningInfo;



typedef struct wrPwsParsedWrngInfo
{
   U16                 repetitionPeriod; 
   U32                 numBrdcstReq; 
   U32                 msgId;
   U32                 serialNum;
   U8                  notificationType; 
   U8                  *msg;
   U32                 msgLen;
   U8                  emergencyAreaType; 
   U8                  dataCodingScheme;
   U8                  wrngSecurity[50];
   U16                 warningType;
   Bool                warnMsgContentsIePres;
   Bool                dataCodingSchemeIePres;
   Bool                extRepetitionPeriodPres;
   Bool                concrntMsgInd;
   U32                 extRepetitionPeriod;
}WrPwsParsedWrngInfo;

/** @brief This structure is used to store the Configured Emergency Area IDs
 * and for each of those areas whether the warning notification broadcasting is possible or not
 * and also indicates validity for the useage of each EAID.
 */
typedef struct wrActCfgEaid
{
        Bool   isWarningMsgBroadcastInit; /*!< TRUE for warning message is initiated in this emergency Area, otherwise FALSE */
        Bool   isKillInit;
        U32    eaid;                      /*!< Emergency Area ID */
}WrActCfgEaid;

/** @brief This structure is used to store the Configured SIB10 information
 * and validity of the usage for the configured SIB10 info.
 */
typedef struct wrCfgSIB10Info
{
        U16             sib10ManPerdUsage;/*!< Transmitting cycle and number of transmitting times of SIB10 is configured or not by OAM.*/
        U16             sib10NumOfBroadcasts;  /*!< Specifies the number of transmitting times of SIB10. */
        U16             sib10RepetitionPeriod; /*!< Specifies the transmitting cycle of SIB10. */
}WrCfgSIB10Info;


/** @def WR_PWS_MAX_NUM_EAIDS
 * This Macro indicates the maximum no of Emergency Area Ids can configure from OAM per Cell.
 */
#define WR_PWS_MAX_NUM_EAIDS    15  /*!< Macro for Maximum no of Eaids allowed in EA list per cell*/

/** @brief This enum is used to declare the different types of Emergency Area indications.
 * These enum members fills the emergencyAreaType in WrPwsEmergencyAreaList.
 */
typedef enum wrEmergergencyAreaType
{
   /* PH04_CMAS_TEST */
   WR_PWS_EATYPE_CELLID /*= 1 BROADCASTCOMPLETEDAREALST_CELLID_BROADCAST*/, /*!< Emergency Area Type is CellID */
   WR_PWS_EATYPE_TAI/* = BROADCASTCOMPLETEDAREALST_TAI_BROADCAST*/,      /*!< Emergency Area Type is TAI */
   WR_PWS_EATYPE_EAI/* = BROADCASTCOMPLETEDAREALST_EMERGENCYAREAID_BROADCAST*/,      /*!< Emergency Area Type is EAID */
   WR_PWS_EATYPE_NONE      /*!< EAList IE is not received in warning req, broadcast for all active Cells */
} WrEmergergencyAreaType;


/** @brief This enum is used to declare the different types of notifications
 * This enum members fills the notificationType in WrPwsWarningInfo.
 */
typedef enum wrNotificationType
{
   WR_PWS_PRI_NOTIFICATION = 1,  /*!< Broadcast the Primary notification only */
   WR_PWS_SEC_NOTIFICATION,      /*!< Broadcast the Secondary notification only */
   WR_PWS_BOTH_NOTIFICATIONS,    /*!< Broadcast the both Primary and Secondary */
   WR_PWS_CMAS_NOTIFICATION
} WrNotificationType;

/*EXTERN S16 wrEmmPwsProcWarningReq(WrEmmCb *emmCb, WrMmeCb *mmeCb, S1apPdu *warningReqPdu); */

/****************** PWS Module Interface Function Declarations **************************/

EXTERN S16 wrEmmPwsProcWarningReq(U32 peerId, S1apPdu *warningReqPdu);

EXTERN S16 wrEmmPwsProcKillReq(U32 peerId, S1apPdu *killReqPdu);

EXTERN S16 wrPwsHndlSchdCfgCfm
(
   WrCellId cellId,
   U32      transId
);

EXTERN S16 wrPwsHndlSchdCfgFail
(
   WrCellId cellId,
   U32      transId
);

EXTERN S16 wrPwsHndlStopWrngSiCfgCfm
(
   WrCellId cellId,
   U32      transId
);



EXTERN S16 wrPwsPrntPdu
(
  CmLListCp                  *pduLst
);

EXTERN S16 wrPwsCpySegLst
(
  RgrWarningSiCfgReqInfo     *cfgReq,
  CmLListCp                  *pduLstSrc
);

EXTERN WrPwsCmasInfo* wrPwsGetNextNode
(
  CmLListCp                 *cmasLst,
  WrPwsCmasInfo             *cmasInfo
);


EXTERN S16 wrEmmPwsProcRepTimerExp
(
Void                         *cb,
U8                           wrUmmTmrExpEvnt
);

EXTERN S16 wrEmmupdatePwsConfig
(
U8                           newpwsEtwsCmasval, 
U8                           oldpwsEtwsCmasval
);
#ifdef __cplusplus
}
#endif
#endif /*__WR_EMM_PWS_H__*/



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
