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


     File:     wr_emm_pws.c

     Sid:      $SID$

     Prg:      Sriky

*********************************************************************21*/
static const char* RLOG_MODULE_NAME="EMM";
static int RLOG_MODULE_ID=1;
static int RLOG_FILE_ID=32;


#include "wr.h"            /* Common headers and Data Structures */
#include "wr_utils.h"
#include "wr_emm.h"        /* Environment options */
#include "szt_asn.x"
#include "wr_emm_pws.h"
#include "wr_emm_pbm.h"
#include "wr_ifm_s1ap.h"
#include "wr_emm_mme.h"
#include "wr_emm_cfg.h"

EXTERN S16 wrEmmMmeSztFillTAC (U16 tac, S1apPdu *pdu, SztTAC *tAC);
EXTERN S16 wrEmmMmeSztFillPLMNId (S1apPdu *pdu, WrPlmnId *plmnId, SztPLMNidentity *plmnIe);
EXTERN S16 wrEmmMmeParsePlmnIe (SztPLMNidentity *plmnIe, WrPlmnId *plmnId );
EXTERN S16 wrPwsStartPrimaryEtws ( WrCellCb *cellCb );
EXTERN S16 wrPwsStartSecondaryEtws( WrCellCb *cellCb);
EXTERN S16 wrPwsStartCmas( WrCellCb *cellCb);
EXTERN Void wrPbmPwsStopPagingEtws (WrCellCb *cellCb, WrEtwsPagInfo *pagInfo);

PRIVATE WrEtwsPagInfo *wrPwsGetWaringPagingInfo
(        
 WrCellCb              *cellCb,
 WrWaringPagingType    type
 );

PRIVATE S16 wrPwsUpdWarningInfo
(
 WrPwsWarningInfo             *warningInfo,
 SztWarningAreaLst            **warnAreaLst,
 S1apPdu                      *warningReqPdu,
 U8                            isEtwsCmas
 );

PRIVATE S16 wrPwsParseWarningInfo
(
 WrPwsWarningInfo             *warningInfo,
 SztWarningAreaLst            **warnAreaLst,
 S1apPdu                      *warningReqPdu,
 WrPwsParsedWrngInfo          *parsedWrngInfo
 );

PRIVATE Void wrPwsInitializeCommonInfo
(
 WrPwsWarningInfo             *warningInfo
 );

PRIVATE S16 wrPwsInitializeCmasInfo
(
 WrPwsWarningInfo             *warningInfo,
 WrPwsParsedWrngInfo          *parsedWrngInfo
 );

PRIVATE S16 wrPwsCopyParsedWrngInfoToCmas
(
 WrPwsWarningInfo             *warningInfo,
 WrPwsParsedWrngInfo          *parsedWrngInfo
 );

PRIVATE Void wrPwsSetEtwsInfoToDefault
(
 WrPwsWarningInfo             *warningInfo
 );

PRIVATE S16 wrPwsInitializeEtwsInfo
(
 WrPwsWarningInfo             *warningInfo,
 WrPwsParsedWrngInfo          *parsedWrngInfo
 );

PRIVATE S16 wrPwsCopyParsedWrngInfoToEtws
(
 WrPwsWarningInfo             *warningInfo,
 WrPwsParsedWrngInfo          *parsedWrngInfo
 );

PRIVATE Void wrPwsCopyParsedWrngInfoToPrimEtws
(
 WrPwsWarningInfo             *warningInfo,
 WrPwsParsedWrngInfo          *parsedWrngInfo
 );

PRIVATE Void wrPwsCopyParsedWrngInfoToSecEtws
(
 WrPwsWarningInfo             *warningInfo,
 WrPwsParsedWrngInfo          *parsedWrngInfo
 );



PRIVATE Void wrPwsEtwsStopRepetitionTimer
(
 WrCellCb         *cellCb,
 U8               wrngType
 );

PRIVATE S16 wrPwsEtwsProcessWrng
(
 WrPwsWarningInfo             *warningInfo,
 SztWarningAreaLst            **warnAreaLst,
 WrMmeCb                      *mmeCb
 );

PRIVATE S16 wrPwsValidateWrngBasedOnType
(
 WrPwsWarningInfo          *warningInfo,
 WrPwsTranientInfo         *transInfo,
 WrMmeCb                   *mmeCb
 );

PRIVATE Void wrPwsInitTransientInfo
(
 WrPwsTranientInfo         *transInfo
 );

PRIVATE S16 wrPwsProcWrngWithWrngAreaList
(
 SztWarningAreaLst           *warnAreaLst,
 WrPwsWarningInfo            *warningInfo,
 WrCellCb                    *cellCb,
 WrPwsTranientInfo           *transInfo
 );

PRIVATE S16 wrPwsProcWrngWithoutWrngAreaList
(
 WrPwsWarningInfo            *warningInfo,
 WrCellCb                    *cellCb,
 WrPwsTranientInfo           *transInfo
 );

PRIVATE S16 wrPwsUpdEtwsInSib1SchedInfoLst
(
 WrCellCb              *cellCb,
 WrSibAction           action,
 WrPwsTranientInfo     *transInfo
 );

PRIVATE S16 wrPwsEtwsFindWrngArea
(
 SztWarningAreaLst           *warnAreaLst,
 WrPwsWarningInfo            *warningInfo,
 WrCellCb                    *cellCb,
 WrPwsTranientInfo           *transInfo
 );

PRIVATE S16 wrPwsEtwsProcBroadcast
(
 WrPwsWarningInfo            *warningInfo,
 WrCellCb                    *cellCb,
 WrPwsTranientInfo           *transInfo
 );

PRIVATE S16 wrPwsCmasProcessWrng
(
 WrPwsWarningInfo             *warningInfo,
 SztWarningAreaLst            **warnAreaLst,
 WrMmeCb                      *mmeCb
 );

PRIVATE WrPwsCmasInfo* wrPwsUtlGetRcvdCmasInfo
(
 WrPwsWarningInfo             *warningInfo
 );

PRIVATE WrPwsCmasInfo* wrPwsUtlGetDupCmas
(
 WrCellCb                  *cellCb,
 U32                       msgId,
 U32                       serialNum
 )__attribute__((noinline));
PRIVATE S16 wrPwsValidateCmasNotif
(
 WrPwsWarningInfo            *warnInfo
 );

PRIVATE S16 wrPwsStoreWrngInfo
(
 WrPwsWarningInfo             *warningInfo,
 WrCellCb                     *cellCb,
 Bool                         isCmas
 );

PRIVATE S16 wrPwsStorePrimEtwsWrng
(
 WrCellCb             *cellCb,
 WrPwsWarningInfo     *warningInfo,
 Bool                 isFirstWrng
 );

PRIVATE S16 wrPwsStoreSecEtwsWrng 
(
 WrCellCb             *cellCb,
 WrPwsWarningInfo     *warningInfo,
 Bool                 isFirstWrng
 );

PRIVATE S16 wrPwsStoreCmasWrng 
(
 WrCellCb            *cellCb,
 WrPwsWarningInfo    *warningInfo,
 Bool                isFirstWrng
 );

PRIVATE S16 wrPwsSegWrngMsg
(
 WrCellCb                     *cellCb,
 WrPwsSIBldInfo               *cmasInfo
 );

PRIVATE Void wrPwsSortInsertCmas
(
 CmLList                    *entryLnk,
 CmLListCp                  *lst
 );

PRIVATE Bool rmCmpRepPeriod
(
 WrPwsCmasInfo             *lnkNode,
 WrPwsCmasInfo             *entry
 );

PRIVATE S16 wrPwsValidateWarningNotif
(
 WrPwsWarningInfo            *currWarnInfo,
 WrPwsWarningInfo            *prevWarnInfo,
 Bool                        isCmas
 );

PRIVATE S16 wrPwsValidateSecondWarnNotif
(
 WrPwsWarningInfo            *warningInfo
 );

PRIVATE S16 wrPwsSelectBroadcastArea
(
 SztWarningAreaLst           *warnAreaLst,
 WrPwsWarningInfo            *rcvdWarningInfo,
 WrCellCb                    *cellCb,
 U8                          wrngType
 );

PRIVATE Bool wrPwsIsServCellIdPresentInLst
(
 WrCellCb                    *cellCb,
 SztECGILst                  *rcvdCellIdLst
 );

PRIVATE Bool wrPwsIsServCellTAIPresentInLst
(
 WrCellCb                    *cellCb,
 SztTAILstforWarning         *rcvdTAILst
 );

PRIVATE Bool wrPwsIsServCellEAIDPresentInLst
(
 WrCellCb                    *cellCb,
 SztEmergencyAreaIDLst       *rcvdEAIDLst,
 Bool                        isStart
 );

PRIVATE S16 wrPwsSndPagingwithEtwsInd(WrCellCb *cellCb);

PRIVATE S16 wrPwsSndPagingwithCmasInd(WrCellCb *cellCb);

PRIVATE S16 wrPwsStopPagingwithCmasInd(WrCellCb *cellCb);

PRIVATE S16 wrPwsSztFillWarningResponse
(
 WrCellCb   **cellCbLst,
 S1apPdu    **pdu,
 WrPwsWarningInfo *warnRsp,
 Bool broadCastCompletedIePres,
 Bool     validCell[]
 );

PRIVATE S16 wrPwsFillAndSndWrngResp
(
 WrCellCb                     *cellCb,
 WrPwsWarningInfo             *warningInfo,
 WrMmeCb                      *mmeCb,
 Bool                         broadCastCompletedIePres,
 Bool                         validCell[]
 );

PRIVATE S16 wrPwsFillAndSndKillResp
(
 WrCellCb                     *cellCb,
 U32                          msgId,
 U32                          serialNum,
 U16                         numberOfBroadcasts,
 U8                           eaType,
 WrMmeCb                      *mmeCb,
 Bool                         broadCastCancelledIePres,
 Bool                         validCell[]
 );

PRIVATE S16 wrPwsSztFillKillResponse
(
 WrCellCb **cellCbLst,
 S1apPdu    **pdu,
 U32      msgId,
 U32      serialNum,
 U16     numberOfBroadcasts,
 U8       emergencyAreaType,
 Bool     broadCastCancelledIePres,
 Bool     validCell[]
 );


PRIVATE S16 wrPwsHndlPrimEtwsSchdCfgCfm
(
 WrCellCb            *cellCb
 );

PRIVATE S16 wrPwsHndlSecEtwsSchdCfgCfm
(
 WrCellCb            *cellCb
 );

PRIVATE S16 wrPwsHndlCmasSchdCfgCfm
(
 WrCellCb            *cellCb
 );

PRIVATE S16 wrEmmPwsPrimEtwsRepTimerExp
(        
 WrCellCb                  *cellCb ,
 WrPwsMsgRepeatInfo        *repInfo 
 );

PRIVATE S16 wrEmmPwsSecEtwsRepTimerExp
(        
 WrCellCb                  *cellCb ,
 WrPwsMsgRepeatInfo        *repInfo 
 );

PRIVATE S16 wrEmmPwsCmasRepTimerExp
(        
 WrCellCb                  *cellCb ,
 WrPwsMsgRepeatInfo        *repInfo 
 );

PRIVATE S16 wrPwsSelectBroadcastAreaToStop
(
 SztWarningAreaLst           *warnAreaLst,
 U32                         msgId,
 U32                         serialNum,
 WrCellCb                    *cellCb
 );

PRIVATE S16 wrPwsValidateKillNotif
(
 U32                         msgId,
 U32                         serialNum,
 WrPwsWarningInfo            *warnInfo
 );

PRIVATE S16 wrPwsStartRepetionTmr
(
 U16                         wrngType,
 WrPwsEtwsInfo               *etwsInfo,
 WrPwsCmasInfo               *cmasInfo,
 WrCellCb                    *cellCb
 );

PRIVATE S16 wrPwsStopCmas
(
 WrCellCb            *cellCb
 );

PRIVATE S16 wrPwsStopPrimaryEtws
(
 WrCellCb            *cellCb
 );

PRIVATE S16 wrPwsStopSecondaryEtws
(
 WrCellCb            *cellCb
 );

PRIVATE S16 wrPwsStopBothEtws
(
 WrCellCb            *cellCb
 );

PRIVATE S16 wrPwsStopEtwsBasedOnType
(        
 WrCellCb            *cellCb
 );
PRIVATE S16 wrPwsFreePdu
(
  CmLListCp                  *pduLst
);

/** @brief This function handles the Write-replace warning message received
 *         from MME.
 *
 * @details This function is called from S1AP-IM  whenever Write replace warning
 *          message is received from MME.
 *
 *     Function: wrEmmPwsProcWarningReq
 *
 *         Processing steps:
 *         - Calls wrPwsUpdWarningInfo to Parse the incomimg Write-Replace warning 
 *         message and put the received parameters in the appropriate local structure.
 *         - Calls wrPwsFillAndSndWrngResp to fill the Write-replace warning reaponse 
 *         message and send it to the MME.
 *         - Calls wrPwsCmasProcessWrng or wrPwsEtwsProcessWrng to process the received
 *         warning message.
 *
 *
 * @param [in] peerId : Peer Identifier 
 * @param [in] warningReqPdu : S1AP PDU for Write replace warning request.
 * 
 * @return S16
 *    -# Success : ROK
 *    -# Failure : RFAILED
 */
PUBLIC S16 wrEmmPwsProcWarningReq(U32 peerId, S1apPdu *warningReqPdu)
{ 
   S16                       ret = ROK;
   WrPwsWarningInfo          rcvdWrngInfo;
   SztWarningAreaLst         *warnAreaLst = NULLP; 
   WrMmeCb                   *mmeCb = NULLP;
   U8                         isEtwsCmas = 0;
   WrEmmCb                   *emmCb = NULLP;
   Bool                      validCell[wrEmmCb.numCells];

   emmCb = &wrEmmCb;

   RLOG0(L_EVENT,"[S1AP]:Received WRITE-REPLACE WARNING REQUEST");

   mmeCb = wrEmmMmeGetMmeByPeerId(peerId);
   if (mmeCb == NULLP) 
   {
      /* Dont respond with write relace response here as the state is invalid to transmit   */
      /* any response at all.  */
      RLOG0(L_ERROR,"Failed to get MME control block.");
      RETVALUE(RFAILED);
   }
   isEtwsCmas = emmCb->pwsEtwsCmas;
   /* warning information is set to 0 */
   cmMemset((U8*)(&rcvdWrngInfo), 0, sizeof(WrPwsWarningInfo));

   /* Parse the incomimg Write-Replace warning message and put the received
      parameters in the appropriate local structure. */
   ret = wrPwsUpdWarningInfo(&rcvdWrngInfo, &warnAreaLst, warningReqPdu, isEtwsCmas);

   if(ret == RFAILED)
   {
      /* Fill the Write-replace warning reaponse message and send it to the MME. */
      wrPwsFillAndSndWrngResp(*(emmCb->cellCb), &rcvdWrngInfo, mmeCb, FALSE, validCell);
      if(ret == RFAILED)
      {
         RLOG0(L_ERROR,"Failed to send warning response message.");
      }
      RETVALUE(RFAILED);
   }
   switch(isEtwsCmas)
   {
       case WR_PWS_ETWS:
       {
                /* Process the received ETWS message. */
                ret = wrPwsEtwsProcessWrng(&rcvdWrngInfo, &warnAreaLst, mmeCb);
       }
       break;
       case WR_PWS_CMAS:
       {

                /* Process the received CMAS message. */
                ret = wrPwsCmasProcessWrng(&rcvdWrngInfo, &warnAreaLst, mmeCb);
       }
       break;
       default:
       {
                RLOG0(L_WARNING,"wrEmmPwsProcWarningReq: PWS is disabled in the System.\n");
                wrPwsFillAndSndWrngResp(*(emmCb->cellCb), &rcvdWrngInfo, mmeCb, FALSE, validCell);
       }      
   }
   RETVALUE(ret);
} /* end of wrEmmPwsProcWarningReq */

/** @brief This function updates the parameters received in the Write replace
 * warning request to the appropriate PWS data structures. 
 *
 * @details This function updates the parameters received in the Write replace
 *  warning request to the appropriate PWS data structures.
 *
 *     Function: wrPwsUpdWarningInfo
 *
 *         Processing steps:
 *         - Initialize parsed warning info structure.
 *         - Calls wrPwsInitializeCmasInfo to initialize the CMAS node if
 *         received warning message is a CMAS message.
 *         - Calls wrPwsInitializeEtwsInfo to initialize the ETWS node if
 *         received warning message is a ETWS message.
 *
 *
 * @param [in] warningInfo : Warning Info Structre
 * @param [in] warnAreaLst : Warning area list information
 * @param [in] warningReqPdu : Warning request PDU
 * @param [in] isCmas : CMAS flag
 * 
 * @return S16
 *    -# Success : ROK
 *    -# Failure : RFAILED
 */
PRIVATE S16 wrPwsUpdWarningInfo
(
 WrPwsWarningInfo             *warningInfo,
 SztWarningAreaLst            **warnAreaLst,
 S1apPdu                      *warningReqPdu,
 U8                            isEtwsCmas
 )
{
   WrPwsParsedWrngInfo         parsedWrngInfo;
   S16                         ret = ROK;

   TRC2(wrPwsUpdWarningInfo);

   /* Initialize warning info */
   parsedWrngInfo.msgId = WR_PWS_INVALID_MSGID;
   parsedWrngInfo.serialNum = 0;
   parsedWrngInfo.emergencyAreaType = WR_PWS_EATYPE_NONE;
   parsedWrngInfo.warnMsgContentsIePres = FALSE; 
   parsedWrngInfo.dataCodingSchemeIePres = FALSE;
   parsedWrngInfo.repetitionPeriod = 0; 
   parsedWrngInfo.numBrdcstReq = 0;
   parsedWrngInfo.notificationType = 0;
   parsedWrngInfo.msg = NULLP;
   parsedWrngInfo.msgLen = 0;
   parsedWrngInfo.warningType = 0xff;
   parsedWrngInfo.dataCodingScheme = 0;
   parsedWrngInfo.extRepetitionPeriodPres = 0;
   parsedWrngInfo.concrntMsgInd = FALSE;
   parsedWrngInfo.extRepetitionPeriod = 0;

   /* Parse the received Write-replace warning request message. */
   ret = wrPwsParseWarningInfo(warningInfo, warnAreaLst, warningReqPdu , &parsedWrngInfo);
   if(RFAILED == ret)
   {
       RLOG0(L_ERROR,"Failed to parse the Write-replace warning message.");
   }

   if(isEtwsCmas & WR_PWS_CMAS)
   {
      wrPwsInitializeCmasInfo(warningInfo, &parsedWrngInfo);
      if(parsedWrngInfo.warnMsgContentsIePres && parsedWrngInfo.dataCodingSchemeIePres)
   {
            if((parsedWrngInfo.concrntMsgInd == FALSE) && (warningInfo->cmasLst.count > 1))
            {
                RLOG0(L_ERROR, "CMAS: Missing Concurrent IE");
                RETVALUE(RFAILED);
            }
   }
   else
   {
           RLOG0(L_ERROR,"wrPwsUpdWarningInfo:Error in Parsing Warning Message.\n");
           RETVALUE(RFAILED);
      }
   }
   else
   {
      /* Case for ETWS Enabled and PWS Disabled both */
      wrPwsInitializeEtwsInfo(warningInfo, &parsedWrngInfo);
   } /* end else etws. */

   /* Validate the IEs */
   RETVALUE(ROK);
} /* end of wrPwsUpdWarningInfo */

/** @brief This function parsed the received Write replace warning request.
 *
 * @details This function parsed the received Write replace warning request and
 * stores the received parameters to the WrPwsParsedWarningInfo structure.
 *
 *     Function: wrPwsParseWarningInfo
 *
 *         Processing steps:
 *         - Parse each IE received in the write replace warning request.
 *         - Store the received parameters in the WrPwsParsedWrngInfo.
 *
 *
 * @param [out] warningInfo : Warning Info Structre
 * @param [in] warnAreaLst : Warning area list information
 * @param [in] warningReqPdu : Warning request PDU
 * @param [out] parsedWrngInfo :Parsed warning Info 
 * 
 * @return S16
 *    -# Success : ROK
 *    -# Failure : RFAILED
 */
PRIVATE S16 wrPwsParseWarningInfo
(
 WrPwsWarningInfo             *warningInfo,
 SztWarningAreaLst            **warnAreaLst,
 S1apPdu                      *warningReqPdu,
 WrPwsParsedWrngInfo          *parsedWrngInfo
 )
{
   U16                        numCompWarnReq = 0, memberIdx=0;
   U32                        memberId = 0;
   SztWriteReplaceWarningRqst *warningRqst = NULLP;
   SztProtIE_Field_WriteReplaceWarningRqstIEs   *warnReqMember = NULLP;

   warningRqst = &(warningReqPdu->pdu.val.initiatingMsg.value.u.sztWriteReplaceWarningRqst);
   numCompWarnReq = warningRqst->protocolIEs.noComp.val;

   /* Accessing the IEs within the S1AP pdu */
   for(memberIdx = 0; memberIdx < numCompWarnReq; memberIdx++)
   {
      warnReqMember = &(warningRqst->protocolIEs.member[memberIdx]);
      memberId = warnReqMember->id.val;

      switch(warnReqMember->id.val)
      {
         case Sztid_MsgIdentifier:
            {
               wrUtlGetU32FrmBStr32(&(parsedWrngInfo->msgId),
                     &(warnReqMember->value.u.sztMsgIdentifier));
               break;
            }
         case Sztid_SerialNumber: 
            {
               wrUtlGetU32FrmBStr32(&(parsedWrngInfo->serialNum),
                     &(warnReqMember->value.u.sztSerialNumber));
               break;
            }
         case Sztid_WarningAreaLst:
            {
               *warnAreaLst = &warnReqMember->value.u.sztWarningAreaLst;
               parsedWrngInfo->emergencyAreaType = warnReqMember->value.u.sztWarningAreaLst.choice.val;
               break;
            }
         case Sztid_RepetitionPeriod:
            {
               parsedWrngInfo->repetitionPeriod = warnReqMember->value.u.sztRepetitionPeriod.val;
               break;
            }
         case Sztid_ExtendedRepetitionPeriod:
            {
               parsedWrngInfo->extRepetitionPeriodPres = TRUE;
               parsedWrngInfo->extRepetitionPeriod = warnReqMember->value.u.sztExtendedRepetitionPeriod.val;
               break;
            }
         case Sztid_NumberofBroadcastRqst:
            {
               parsedWrngInfo->numBrdcstReq = warnReqMember->value.u.sztNumberofBroadcastRqst.val;
               break;
            }
         case Sztid_WarningTyp:
            {
               parsedWrngInfo->warningType = warnReqMember->value.u.sztWarningTyp.val[1]; 
               parsedWrngInfo->warningType = parsedWrngInfo->warningType << 8; 
               parsedWrngInfo->warningType |= warnReqMember->value.u.sztWarningTyp.val[0];

               /* If warning Type IE received in warning Request message, then trigger the 
                * primary notification.*/ 
               parsedWrngInfo->notificationType = WR_PWS_PRI_NOTIFICATION; 
               break;
            }
         case Sztid_WarningSecurInfo:
            {
               /* TODO : This IE is of type TknStrOSXL. Is it required to do any type conversion. */
               memcpy(&warningInfo->etwsInfo.primEtwsInfo.wrngSecurity,
                     &warnReqMember->value.u.sztWarningSecurInfo.val[0], 
                     warnReqMember->value.u.sztWarningSecurInfo.len);
               warningInfo->etwsInfo.primEtwsInfo.wrngSecurityPres = FALSE;
               break;
            }
         case Sztid_DataCodingScheme:
            {
               parsedWrngInfo->dataCodingScheme = warnReqMember->value.u.sztDataCodingScheme.val[0];
               parsedWrngInfo->dataCodingSchemeIePres = TRUE;
               break;
            }
         case Sztid_WarningMsgContents:
            {
               parsedWrngInfo->msg = warnReqMember->value.u.sztWarningMsgContents.val;
               parsedWrngInfo->msgLen = warnReqMember->value.u.sztWarningMsgContents.len;
               parsedWrngInfo->warnMsgContentsIePres = TRUE;
               break;
            }
         case Sztid_ConcurrentWarningMsgIndicator:
            {
               parsedWrngInfo->concrntMsgInd = TRUE;
               break;
            }
         default:
            {
               RLOG1(L_ERROR,"Unknown IE or Option received:[%ld]", memberId); 
               break;
            }
      } /* end of switch */
   } /* end of for to traverse all received IEs */

   RETVALUE(ROK);
} /* wrPwsParseWarningInfo */


/** @brief This function is used to initialize the PWS warning data structure
 * which are common to both CMAS and ETWS.
 *
 * @details This function is used to initialize the common data structure within
 * WrPwsWarningInfo to default values.
 *
 *     Function: wrPwsInitializeCommonInfo
 *
 *         Processing steps:
 *         - Initialize warningInfo parameters common to CMAS and ETWS.
 *
 * @param [out] warningInfo : Warning Info Structre
 * 
 * @return Void
 *
 */
PRIVATE Void wrPwsInitializeCommonInfo
(
 WrPwsWarningInfo             *warningInfo
 )
{
   /* Initialize warningInfo. */
   warningInfo->siBldInfo.crntCmas = NULLP;
   warningInfo->siBldInfo.segNum = 0;
   warningInfo->siBldInfo.isLastSeg = FALSE;
   warningInfo->siBldInfo.segLen = 0;
   warningInfo->siBldInfo.segment = NULLP;
   warningInfo->currentCmas = NULLP;
   warningInfo->isPrimEtwsInProgress = FALSE;
   warningInfo->isSecEtwsInProgress = FALSE;
   warningInfo->isCmasInProgress = FALSE;
   warningInfo->sib10SiIdx = 0;
   warningInfo->sib11SiIdx = 0;
   warningInfo->sib12SiIdx = 0;
   warningInfo->tmrRunningForLastCmas = FALSE;

   RETVOID;
} /*wrPwsInitializeCommonInfo*/


/** @brief This function is used to initialize the PWS warning data structure
 *
 * @details This function is used to initialize the CMAS data structure within
 * WrPwsWarningInfo using the information filled in the WrPwsParsedWrngInfo while
 * parsing the write replace warning message.
 *
 *     Function: wrPwsInitializeCmasInfo
 *
 *         Processing steps:
 *         - Initialize warningInfo.
 *         - Store the received parameters from WrPwsParsedWrngInfo to the
 *         warningInfo.
 *         - Initialize remaining ETWS parameters.
 *
 * @param [out] warningInfo : Warning Info Structre
 * @param [in] parsedWrngInfo :Parsed warning Info 
 * 
 * @return S16
 *    -# Success : ROK
 *    -# Failure : RFAILED
 */
PRIVATE S16 wrPwsInitializeCmasInfo
(
 WrPwsWarningInfo             *warningInfo,
 WrPwsParsedWrngInfo          *parsedWrngInfo
 )
{
   S16 ret = ROK;

   warningInfo->rcvdWrngType = WR_PWS_CMAS_NOTIFICATION;
   /* Initialize warningInfo. */
   wrPwsInitializeCommonInfo(warningInfo);

   ret = wrPwsCopyParsedWrngInfoToCmas(warningInfo, parsedWrngInfo);

   /* Initialize the ETWS parameters. */
   wrPwsSetEtwsInfoToDefault(warningInfo);

   RETVALUE(ret);
}/*wrPwsInitializeCmasInfo*/


/** @brief This function is used to initialize the CMAS structure from parsed
 * warning info.
 *
 * @details This function is used to initialize the CMAS data structure within
 * WrPwsWarningInfo using the information filled in the WrPwsParsedWrngInfo while
 * parsing the write replace warning message.
 *
 *     Function: wrPwsCopyParsedWrngInfoToCmas
 *
 *         Processing steps:
 *         - Store the received parameters from WrPwsParsedWrngInfo to the
 *         warningInfo.
 *
 * @param [out] warningInfo : Warning Info Structre
 * @param [in] parsedWrngInfo :Parsed warning Info 
 * 
 * @return S16
 *    -# Success : ROK
 *    -# Failure : RFAILED
 */
PRIVATE S16 wrPwsCopyParsedWrngInfoToCmas
(
 WrPwsWarningInfo             *warningInfo,
 WrPwsParsedWrngInfo          *parsedWrngInfo
 )
{
   WrPwsCmasInfo              *cmasInfo = NULLP;

   /* Allocate memory for new CMAS message. */
   /*  This memory will be freed whenever the particular CMAS is stopped due to 
    *  kill request or due to number of broadcast reached to 0. */ 
   WR_ALLOC(&cmasInfo, sizeof(WrPwsCmasInfo));
   if (cmasInfo == NULLP)
   {
      RLOG0(L_FATAL, "Memory allocation failed.");
      RETVALUE(RFAILED); 
   }

   cmasInfo->wrngMsgInfo.wrngMsgPtr = parsedWrngInfo->msg;
   cmasInfo->wrngMsgInfo.msgLen = parsedWrngInfo->msgLen;
   cmasInfo->wrngMsgInfo.emergencyAreaType = parsedWrngInfo->emergencyAreaType;
   cmasInfo->wrngMsgInfo.dataCodingScheme = parsedWrngInfo->dataCodingScheme;
   cmasInfo->wrngMsgId = (U16)(parsedWrngInfo->msgId);
   cmasInfo->wrngSerialNum = (U16)(parsedWrngInfo->serialNum);
   cmasInfo->waitToStop = FALSE;

   if(parsedWrngInfo->extRepetitionPeriodPres)
   {
      cmasInfo->wrngMsgInfo.repetitionPeriod = parsedWrngInfo->extRepetitionPeriod;
   }
   else
   {
      cmasInfo->wrngMsgInfo.repetitionPeriod = parsedWrngInfo->repetitionPeriod;
   }

   cmasInfo->wrngMsgInfo.numBroadcastReq = parsedWrngInfo->numBrdcstReq;
   cmasInfo->waitToStop = FALSE;
   cmasInfo->wrngMsgInfo.readyToSch = TRUE;
   cmLListInit(&cmasInfo->wrngMsgInfo.segPduLst); 
   cmasInfo->cmasLstLnk.node = (PTR)cmasInfo;
   cmLListAdd2Tail(&(warningInfo->cmasLst), &(cmasInfo->cmasLstLnk));
   RETVALUE(ROK);
} /* wrPwsCopyParsedWrngInfoToCmas */    


/** @brief This function is used to initialize the ETWS structre with default
 * values.
 *
 * @details This function is used to initialize the ETWS data structure within
 * WrPwsWarningInfo to default values.
 *
 *     Function: wrPwsSetEtwsInfoToDefault
 *
 *         Processing steps:
 *         - Initialize ETWS parameters.
 *
 * @param [out] warningInfo : Warning Info Structre
 * 
 * @return Void
 */
PRIVATE Void wrPwsSetEtwsInfoToDefault
(
 WrPwsWarningInfo             *warningInfo
 )
{
   warningInfo->etwsInfo.wrngMsgId = WR_PWS_INVALID_MSGID; 
   warningInfo->etwsInfo.wrngSerialNum = 0;
   warningInfo->etwsInfo.primEtwsInfo.emergencyAreaType = WR_PWS_EATYPE_NONE;
   warningInfo->etwsInfo.primEtwsInfo.warningType = WR_PWS_INVALID_TYPE;
   warningInfo->etwsInfo.primEtwsInfo.repetitionPeriod = 0;
   warningInfo->etwsInfo.primEtwsInfo.numBroadcastReq = 0; 
   warningInfo->etwsInfo.secEtwsInfo.wrngMsgPtr = NULLP;
   warningInfo->etwsInfo.secEtwsInfo.msgLen = 0;
   warningInfo->etwsInfo.secEtwsInfo.emergencyAreaType = WR_PWS_EATYPE_NONE; 
   warningInfo->etwsInfo.secEtwsInfo.dataCodingScheme = 0;
   warningInfo->etwsInfo.secEtwsInfo.repetitionPeriod = 0;
   warningInfo->etwsInfo.secEtwsInfo.numBroadcastReq = 0;
   warningInfo->etwsInfo.secEtwsInfo.readyToSch = TRUE;
   cmLListInit(&(warningInfo->etwsInfo.secEtwsInfo.segPduLst)); 

   RETVOID; 
} /* wrPwsSetEtwsInfoToDefault */


/** @brief This function is used to initialize the PWS warning data structure
 *
 * @details This function is used to initialize the ETWS data structure within
 * WrPwsWarningInfo using the information filled in the WrPwsParsedWrngInfo while
 * parsing the write replace warning message.
 *
 *     Function: wrPwsInitializeEtwsInfo
 *
 *         Processing steps:
 *         - Initialize warningInfo.
 *         - Store the received parameters from WrPwsParsedWrngInfo to the
 *         warningInfo.
 *         - Initialize remaining CMAS parameters.
 *
 * @param [out] warningInfo : Warning Info Structre
 * @param [in] parsedWrngInfo :Parsed warning Info 
 * 
 * @return S16
 *    -# Success : ROK
 *    -# Failure : RFAILED
 */
PRIVATE S16 wrPwsInitializeEtwsInfo
(
 WrPwsWarningInfo             *warningInfo,
 WrPwsParsedWrngInfo          *parsedWrngInfo
 )
{
   S16                        ret = ROK;

   /* Initialize warningInfo. */
   wrPwsInitializeCommonInfo(warningInfo);

   ret = wrPwsCopyParsedWrngInfoToEtws(warningInfo, parsedWrngInfo);   

   /* Initialize the CMAS list. */ 
   cmLListInit(&warningInfo->cmasLst); 

   RETVALUE(ret);
}/*wrPwsInitializeEtwsInfo*/

/** @brief This function is used to initialize the PWS warning data structure
 * from the pased ETWS info.
 *
 * @details This function is used to initialize the ETWS data structure within
 * WrPwsWarningInfo using the information filled in the WrPwsParsedWrngInfo while
 * parsing the write replace warning message.
 *
 *     Function: wrPwsCopyParsedWrngInfoToEtws
 *
 *         Processing steps:
 *         - Store the received parameters from WrPwsParsedWrngInfo to the
 *         warningInfo.
 *
 * @param [out] warningInfo : Warning Info Structre
 * @param [in] parsedWrngInfo :Parsed warning Info 
 * 
 * @return S16
 *    -# Success : ROK
 *    -# Failure : RFAILED
 */
PRIVATE S16 wrPwsCopyParsedWrngInfoToEtws
(
 WrPwsWarningInfo             *warningInfo,
 WrPwsParsedWrngInfo          *parsedWrngInfo
 )
{
   U8                         notificationType = 0;

   warningInfo->etwsInfo.wrngMsgId = (U16)(parsedWrngInfo->msgId);
   warningInfo->etwsInfo.wrngSerialNum = (U16)(parsedWrngInfo->serialNum);

   notificationType = parsedWrngInfo->notificationType;

   /* Validate secondary notification needs to trigger or not */
   if (parsedWrngInfo->warnMsgContentsIePres)
   {
      if(notificationType == WR_PWS_PRI_NOTIFICATION)
      {
         notificationType = WR_PWS_BOTH_NOTIFICATIONS;
      }
      else if(notificationType == 0)
      {
         notificationType = WR_PWS_SEC_NOTIFICATION;
      }
   }

   warningInfo->etwsInfo.notificationType = notificationType;
   warningInfo->rcvdWrngType = notificationType;

   if((warningInfo->etwsInfo.notificationType == WR_PWS_PRI_NOTIFICATION))
   {
      wrPwsCopyParsedWrngInfoToPrimEtws(warningInfo, parsedWrngInfo);
   } 
   else if((warningInfo->etwsInfo.notificationType == WR_PWS_SEC_NOTIFICATION))
   {
      wrPwsCopyParsedWrngInfoToSecEtws(warningInfo, parsedWrngInfo);
   } 
   else if((warningInfo->etwsInfo.notificationType == WR_PWS_BOTH_NOTIFICATIONS))
   {
      wrPwsCopyParsedWrngInfoToPrimEtws(warningInfo, parsedWrngInfo);
      wrPwsCopyParsedWrngInfoToSecEtws(warningInfo, parsedWrngInfo);
   }
   else
   {
       RLOG0(L_ERROR,"notification type not set for ETWS.");
   }

   RETVALUE(ROK);
}/* wrPwsCopyParsedWrngInfoToEtws */


/** @brief This function is used to initialize the PWS warning data structure
 * from the parsed  Primary ETWS info.
 *
 * @details This function is used to initialize the prim ETWS data structure within
 * WrPwsWarningInfo using the information filled in the WrPwsParsedWrngInfo while
 * parsing the write replace warning message.
 *
 *     Function: wrPwsCopyParsedWrngInfoToPrimEtws
 *
 *         Processing steps:
 *         - Store the received parameters from WrPwsParsedWrngInfo to the
 *         warningInfo.
 *
 * @param [out] warningInfo : Warning Info Structre
 * @param [in] parsedWrngInfo :Parsed warning Info 
 * 
 * @return S16
 *    -# Success : ROK
 *    -# Failure : RFAILED
 */
PRIVATE Void wrPwsCopyParsedWrngInfoToPrimEtws
(
 WrPwsWarningInfo             *warningInfo,
 WrPwsParsedWrngInfo          *parsedWrngInfo
 )
{
   warningInfo->etwsInfo.primEtwsInfo.emergencyAreaType = parsedWrngInfo->emergencyAreaType;
   warningInfo->etwsInfo.primEtwsInfo.warningType = parsedWrngInfo->warningType;
   warningInfo->etwsInfo.primEtwsInfo.numBroadcastReq = parsedWrngInfo->numBrdcstReq;

   if(parsedWrngInfo->extRepetitionPeriodPres)
   {
      warningInfo->etwsInfo.primEtwsInfo.repetitionPeriod = 
         parsedWrngInfo->extRepetitionPeriod;
   }
   else
   {
      warningInfo->etwsInfo.primEtwsInfo.repetitionPeriod = 
         parsedWrngInfo->repetitionPeriod;
   }

   RETVOID;
}/* wrPwsCopyParsedWrngInfoToPrimEtws */ 


/** @brief This function is used to initialize the PWS warning data structure
 * from the parsed  Secondary ETWS info.
 *
 * @details This function is used to initialize the secondary ETWS data structure within
 * WrPwsWarningInfo using the information filled in the WrPwsParsedWrngInfo while
 * parsing the write replace warning message.
 *
 *     Function: wrPwsCopyParsedWrngInfoToSecEtws
 *
 *         Processing steps:
 *         - Store the received parameters from WrPwsParsedWrngInfo to the
 *         warningInfo.
 *
 * @param [out] warningInfo : Warning Info Structre
 * @param [in] parsedWrngInfo :Parsed warning Info 
 * 
 * @return S16
 *    -# Success : ROK
 *    -# Failure : RFAILED
 */
PRIVATE Void wrPwsCopyParsedWrngInfoToSecEtws
(
 WrPwsWarningInfo             *warningInfo,
 WrPwsParsedWrngInfo          *parsedWrngInfo
 )
{
   warningInfo->etwsInfo.secEtwsInfo.wrngMsgPtr = parsedWrngInfo->msg;
   warningInfo->etwsInfo.secEtwsInfo.msgLen = parsedWrngInfo->msgLen;

   warningInfo->etwsInfo.secEtwsInfo.emergencyAreaType = parsedWrngInfo->emergencyAreaType; 
   warningInfo->etwsInfo.secEtwsInfo.dataCodingScheme = parsedWrngInfo->dataCodingScheme;
   warningInfo->etwsInfo.secEtwsInfo.numBroadcastReq = parsedWrngInfo->numBrdcstReq;

   if(parsedWrngInfo->extRepetitionPeriodPres)
   {
      warningInfo->etwsInfo.secEtwsInfo.repetitionPeriod = 
         parsedWrngInfo->extRepetitionPeriod;
   }
   else
   {
      warningInfo->etwsInfo.secEtwsInfo.repetitionPeriod = 
         parsedWrngInfo->repetitionPeriod;
   }
   cmLListInit(&(warningInfo->etwsInfo.secEtwsInfo.segPduLst)); 

   RETVOID;
}/* wrPwsCopyParsedWrngInfoToSecEtws */

/** @brief This function is a utility function to stop the repetition period
 * timer for the ETWS warning message.
 *
 * @details This function is a utility function to sop the repetition period
 * timer for the ETWS warning message based on warning type.
 *
 *     Function: wrPwsEtwsStopRepetitionTimer
 *
 *         Processing steps:
 *
 * @param [in] cellCb : Cell Control block
 * @param [in] wrngType : Warning type
 * 
 * @return Void
 */
PRIVATE Void  wrPwsEtwsStopRepetitionTimer
(
 WrCellCb         *cellCb, 
 U8               wrngType
 )
{
   if(wrngType == WR_PWS_PRI_NOTIFICATION)
   {
      if(!(cellCb->warningInfo->etwsInfo.primEtwsInfo.readyToSch))
      {
         wrStopTmr((PTR)&(cellCb->warningInfo->etwsInfo.primEtwsInfo.repetitionInfo),
               WR_TMR_REPETITION_PERIOD );
         cellCb->warningInfo->etwsInfo.primEtwsInfo.readyToSch = TRUE;
      }
   }
   else if(wrngType == WR_PWS_SEC_NOTIFICATION)
   {
      if(!(cellCb->warningInfo->etwsInfo.secEtwsInfo.readyToSch))
      {
         wrStopTmr((PTR)&(cellCb->warningInfo->etwsInfo.secEtwsInfo.repetitionInfo),
               WR_TMR_REPETITION_PERIOD );
         cellCb->warningInfo->etwsInfo.secEtwsInfo.readyToSch = TRUE;
      }
   }
   else if(wrngType == WR_PWS_BOTH_NOTIFICATIONS)
   {
      if(!(cellCb->warningInfo->etwsInfo.primEtwsInfo.readyToSch))
      {
         wrStopTmr((PTR)&(cellCb->warningInfo->etwsInfo.primEtwsInfo.repetitionInfo),
               WR_TMR_REPETITION_PERIOD );
         cellCb->warningInfo->etwsInfo.primEtwsInfo.readyToSch = TRUE;
      }
      if(!(cellCb->warningInfo->etwsInfo.secEtwsInfo.readyToSch))
      {
         wrStopTmr((PTR)&(cellCb->warningInfo->etwsInfo.secEtwsInfo.repetitionInfo),
               WR_TMR_REPETITION_PERIOD );
         cellCb->warningInfo->etwsInfo.secEtwsInfo.readyToSch = TRUE;
      }
   }

   RETVOID;
}/* wrPwsEtwsStopRepetitionTimer */

/** @brief This function is used to process ETWS warning message.
 *
 * @details This function is used to process ETWS warning message.
 *
 *     Function: wrPwsEtwsProcessWrng
 *
 *         Processing steps:
 *         - Validate the ETWS notification.
 *         - Store the received warning message into cellCb.
 *         - Fill the Write-replace warning reaponse message and send it to the
 *         MME.
 *         - Configure MAC.
 *
 * @param [in] warningInfo : Warning Info Structre
 * @param [in] warnAreaLst : Warning Area List
 * @param [in] mmeCb : MME control block
 * 
 * @return S16
 *    -# Success : ROK
 *    -# Failure : RFAILED
 */
PRIVATE S16 wrPwsEtwsProcessWrng
(
 WrPwsWarningInfo         *warningInfo,
 SztWarningAreaLst        **warnAreaLst,
 WrMmeCb                  *mmeCb
 )
{
   WrPwsTranientInfo         transInfo;
   S16                       ret = ROK;
   WrCellCb                  *cellCb = NULLP;
   WrEmmCb                   *emmCb;

   emmCb = &wrEmmCb;

   wrPwsInitTransientInfo(&transInfo);

   ret = wrPwsValidateWrngBasedOnType(warningInfo, &transInfo, mmeCb);
   if(ret == RFAILED)
   {
      RLOG0(L_ERROR,"wrPwsValidateWrngBasedOnType:Unable to process the"
         "warning message due to validation failure.");
      RETVALUE(RFAILED);
   }

   for (transInfo.cellIdx = 0; transInfo.cellIdx <  wrEmmCb.numCells; transInfo.cellIdx++)
   { 
      cellCb =wrEmmCb.cellCb[transInfo.cellIdx]; 
      ret = ROK;

      if(cellCb->warningInfo != NULLP)
      {
         if(cellCb->warningInfo->etwsInfo.wrngMsgId != WR_PWS_INVALID_MSGID)
         {
            transInfo.isEtwsPresent = TRUE;
         }
      }

      ret = wrPwsEtwsFindWrngArea(*warnAreaLst, warningInfo, cellCb, &transInfo);
      if(ret == RFAILED)
      {
         RLOG0(L_ERROR," wrPwsEtwsFindWrngArea:Failed to store the received"
            "warning message.");
         continue;
      }

      /* Fill the Write-replace warning reaponse message and send it to the MME. */
      ret = wrPwsFillAndSndWrngResp(*(emmCb->cellCb), warningInfo, mmeCb, 
            transInfo.broadCastCompletedIePres, transInfo.validCell);
      if(ret == RFAILED)
      {
         RLOG0(L_ERROR,"wrPwsFillAndSndWrngResp:Failed to send warning"
            "response message.");
         continue;
      } 

      ret = wrPwsEtwsProcBroadcast(warningInfo, cellCb, &transInfo);
      if(RFAILED == ret)
      {
         RLOG0(L_ERROR,"wrPwsEtwsProcBroadcast:Unable to broadcast the"
            "warning message.");
         continue;
      }
   } /* end for */

   RETVALUE(ROK);
}/*wrPwsEtwsProcessWrng*/


/** @brief This function is used to validate ETWS message based on the warning
 * type.
 *
 * @details This function is used to validate received ETWS message based to the
 * type of ETWS message. This function also sets the transient information
 * parameters.
 *
 *     Function: wrPwsValidateWrngBasedOnType
 *
 *         Processing steps:
 *         - If the received warning message is primary ETWS, validate it.
 *         - If the received warning message is secondary ETWS, validate it.
 *         - If the received warning message is for both primary and secondary
 *           then validate it.
 *
 * @param [in] warningInfo : Warning Info Structre
 * @param [out] transInfo : TransientInfo
 * @param [in] mmeCb : MME control block
 * 
 * @return S16
 *    -# Success : ROK
 *    -# Failure : RFAILED
 */
PRIVATE S16 wrPwsValidateWrngBasedOnType
(
 WrPwsWarningInfo          *warningInfo,
 WrPwsTranientInfo         *transInfo,
 WrMmeCb                   *mmeCb
 )
{
   S16            ret = ROK;
   WrEmmCb        *emmCb = NULLP;

   emmCb = &wrEmmCb;

   /* Validate the ETWS notification. */
   if(warningInfo->etwsInfo.notificationType == WR_PWS_SEC_NOTIFICATION)
   { 
      RLOG0(L_DEBUG,"Processing Secondary ETWS.");
      transInfo->wrngType = WR_PWS_SEC_NOTIFICATION;
      transInfo->wrngToProcess = WR_START_SEC_ETWS_IND;
      transInfo->wrngSibType = WR_SIB_11;
      transInfo->cfgType = WR_SIB1_SEC_ETWS_TRANS_ID;
      ret = wrPwsValidateSecondWarnNotif(warningInfo);
      if( RFAILED == ret)
      {
         /* The received ETWS message shall not be broadcasted.*/
         /* Fill the Write-replace warning reaponse message and send it to the MME. */
         ret = wrPwsFillAndSndWrngResp(*(emmCb->cellCb), warningInfo, 
               mmeCb, FALSE, transInfo->validCell);
         if(ret == RFAILED)
         {
            RLOG0(L_ERROR,"wrPwsFillAndSndWrngResp:Failed to send warning"
               "response message.");
            RETVALUE(RFAILED);
         }

         RLOG0(L_ERROR,
                  "The broadcast of this warning message shall not be commensed"
            "because of validation failure.");
         RETVALUE(RFAILED);
      }
   }
   else if(warningInfo->etwsInfo.notificationType == WR_PWS_PRI_NOTIFICATION)
   {
      /* No need to set transient information here, as the transient info by 
       * default contains Primary ETWS paramters.*/
      RLOG0(L_DEBUG,"Processing Primary ETWS.");
   }
   else if(warningInfo->etwsInfo.notificationType == WR_PWS_BOTH_NOTIFICATIONS)
   {
       RLOG0(L_DEBUG,"Processing both Primary and secondary ETWS.");

      transInfo->wrngType = WR_PWS_BOTH_NOTIFICATIONS;
      transInfo->wrngToProcess = WR_START_BOTH_ETWS_IND;
      transInfo->wrngSibType = WR_SIB_10;
      transInfo->cfgType = WR_SIB1_BOTH_ETWS_TRANS_ID;

      ret = wrPwsValidateSecondWarnNotif(warningInfo);
      if( RFAILED == ret)
      {
         transInfo->validSecEtws = FALSE;
         RLOG0(L_ERROR,"wrPwsValidateSecondWarnNotif:The broadcast of secondary"
            "warning message shall not be commensed  because of validation"
            "failure.");
         /* The received ETWS message shall not be broadcasted.*/
         /* Fill the Write-replace warning reaponse message and send it to the MME. */
         ret = wrPwsFillAndSndWrngResp(*(emmCb->cellCb), warningInfo, 
               mmeCb, FALSE, transInfo->validCell);
         if(ret == RFAILED)
         {
            RLOG0(L_ERROR,"wrPwsFillAndSndWrngResp:Failed to send warning" 
               "response message.");
            RETVALUE(RFAILED);
         }
         RETVALUE(RFAILED);
      }
   }

   RETVALUE(ret); 
} /*wrPwsValidateWrngBasedOnType*/


/** @brief This function is used to initialize the transient information block.
 *
 * @details This function is used to initialize the transient information block.
 *
 *     Function: wrPwsInitTransientInfo
 *
 *         Processing steps:
 *         - Initialize the parameters in transient info.
 *
 * @param [in] transInfo : Transient information
 * 
 * @return Void
 *
 */
PRIVATE Void wrPwsInitTransientInfo
(
 WrPwsTranientInfo         *transInfo
 )
{
   U16          cellIdx = 0;

   for (cellIdx = 0; cellIdx <  wrEmmCb.numCells; cellIdx++)
   {
      transInfo->validCell[cellIdx] = FALSE;
   } 

   transInfo->cellIdx = 0;
   transInfo->wrngType = WR_PWS_PRI_NOTIFICATION;
   transInfo-> broadCastCompletedIePres = FALSE;
   transInfo->isDuplicate = FALSE;
   transInfo->isEtwsPresent = FALSE;
   transInfo->validSecEtws = TRUE;
   transInfo->wrngToProcess = WR_START_PRI_ETWS_IND;
   transInfo->wrngSibType = WR_SIB_10;
   transInfo->wrngSiIdx  =  WR_PBM_SIB10_SI_INDEX;
   transInfo->cfgType = WR_SIB1_PRI_ETWS_TRANS_ID;

   RETVOID;
}


/** @brief This function is used to process the received ETWS warning message
 * when it contains Warning Area list IE.
 *
 * @details This function is used to process the received ETWS warning message
 * when it contains the Warning Area List IE.
 *
 *     Function: wrPwsProcWrngWithWrngAreaList
 *           - This function will check if the recived ETWS message is duplicate
 *          or not for all the warning areas mentioned in the warning area list
 *          IE.
 *          - If the message is duplicate it will be ignored.
 *          - If the new message is received, it will replaced and stored.
 *
 *
 * @param [in] warnAreaLst : Warning Area List
 * @param [in] warningInfo : Warning Info Structre
 * @param [in] cellCb : Cell control block
 * @param [in] transInfo : Transient Info
 * 
 * @return S16
 *    -# Success : ROK
 *    -# Failure : RFAILED
 */
PRIVATE S16 wrPwsProcWrngWithWrngAreaList
(
 SztWarningAreaLst        *warnAreaLst,
 WrPwsWarningInfo         *warningInfo,
 WrCellCb                 *cellCb,
 WrPwsTranientInfo        *transInfo
 )
{
   S16                      ret = ROK;

   ret = wrPwsSelectBroadcastArea(warnAreaLst, warningInfo, 
         cellCb, transInfo->wrngType);

   /* If validation is successful. */ 
   if(ROK == ret)
   {
      RLOG0(L_DEBUG,"Found the broadcast area in the list and validation done.");

      transInfo->validCell[transInfo->cellIdx] = TRUE;
      transInfo->broadCastCompletedIePres = TRUE;

      if( transInfo->isEtwsPresent)
      {
         wrPwsEtwsStopRepetitionTimer(cellCb, cellCb->warningInfo->etwsInfo.notificationType);
      }

      /* Store the received warning message into cellCb. */
      ret = wrPwsStoreWrngInfo(warningInfo, cellCb, FALSE);
      if(ret == RFAILED)
      {
         RLOG0(L_ERROR,"wrPwsStoreWrngInfo:Failed to store the received"
            "warning message.");
         RETVALUE(RFAILED); 
      }
   }
   else if(RFAILED == ret)
   {
      /* If the flow reaches here then it means that already same Etws warning message
         is going on in the particular warninh area. */

      RLOG0(L_ERROR,"Duplicate ETWS warning message received. Ignore the message.");

      transInfo->validCell[transInfo->cellIdx] = TRUE;
      transInfo->broadCastCompletedIePres = FALSE;
      transInfo->isDuplicate = TRUE;
   } /* else */
   else if(ret == WR_PWS_NO_MATCH_FOUND)
   {
      /* No match found for warning area type.*/
      RLOG0(L_ERROR," No match found for warning area type.");
   }

   RETVALUE(ROK);
} /* wrPwsProcWrngWithWrngAreaList */


/** @brief This function is used to process the received ETWS warning message
 * when it does not contain Warning Area list IE.
 *
 * @details This function is used to process the received ETWS warning message
 * when it does not contain the Warning Area List IE.
 *
 *     Function: wrPwsProcWrngWithoutWrngAreaList
 *          - This function will check if the recived ETWS message is duplicate
 *          or not for all the cells.
 *          - If the message is duplicate it will be ignored.
 *          - If the new message is received, it will replaced and stored.
 *
 * @param [in] warningInfo : Warning Info Structre
 * @param [in] cellCb : Cell control block
 * @param [in] transInfo : Transient Info
 * 
 * @return S16
 *    -# Success : ROK
 *    -# Failure : RFAILED
 */
PRIVATE S16 wrPwsProcWrngWithoutWrngAreaList
(
 WrPwsWarningInfo         *warningInfo,
 WrCellCb                 *cellCb,
 WrPwsTranientInfo        *transInfo
 )
{
   S16                     ret = ROK;

   /* Warning area list IE is not present. */
   if(NULLP != cellCb->warningInfo)
   { 
      ret = wrPwsValidateWarningNotif(warningInfo, cellCb->warningInfo, FALSE);
      if(ret != ROK)
      {
         /* If flow reaches here then it means that this is the duplicate 
            secondary ETWS warning message.*/
         RLOG0(L_ERROR,"wrPwsValidateWarningNotif:Duplicate ETWS warning"
            "message received. Ignore the messsage.");
         transInfo->validCell[transInfo->cellIdx] = TRUE;
         transInfo->broadCastCompletedIePres = FALSE;
         transInfo->isDuplicate = TRUE;
      }
      else
      {
         transInfo->validCell[transInfo->cellIdx] = TRUE;
         transInfo->broadCastCompletedIePres = TRUE;

         if(transInfo->isEtwsPresent)
         { 
            wrPwsEtwsStopRepetitionTimer(cellCb, cellCb->warningInfo->etwsInfo.notificationType);
         }

         /* Store the received warning message into cellCb. */
         ret = wrPwsStoreWrngInfo(warningInfo, cellCb, FALSE);
         if(ret == RFAILED)
         {
            RLOG0(L_ERROR,"wrPwsStoreWrngInfo:Failed to store the received"
               "warning message.");
            RETVALUE(RFAILED);
         }
      }
   }
   else
   {
      transInfo->validCell[transInfo->cellIdx] = TRUE;
      transInfo->broadCastCompletedIePres = TRUE;

      /* Store the received warning message into cellCb. */
      ret = wrPwsStoreWrngInfo(warningInfo, cellCb, FALSE);
      if(ret == RFAILED)
      {
         RLOG0(L_ERROR,"wrPwsStoreWrngInfo:Failed to store the received"
            " warning message.");
         RETVALUE(RFAILED);
      }
   }

   RETVALUE(ROK);
}/* wrPwsProcWrngWithoutWrngAreaList */


/** @brief This function is used to update the ETWS SIB in SIB1 scheduling info
 * list.
 *
 * @details This function is used to update the ETWS SIB in SIB1 scheduling info
 *  list.
 *
 *     Function: wrPwsUpdEtwsInSib1SchedInfoLst
 *
 *         Processing steps:
 *         - First add the SIB indicated by wrngSibType  in the trans info. This
 *         parameter indicates SIB10 when the received warning message is 
 *         Primary ETWS or Both primary and secondar ETWS. This will be set to
 *         SIB11 if the received warning message is secondary ETWS.
 *         - once added the SIB10, store the index.
 *         - If the received warning message contains both Primary and secondary
 *         ETWS then again SIB11 will be added to the list and index will be
 *         updated.
 *
 * @param [in] cellCb : Cell control block
 * @param [in] action : Add/ Delete
 * @param [in] transInfo : Transient info
 * 
 * @return S16
 *    -# Success : ROK
 *    -# Failure : RFAILED
 */
PRIVATE S16 wrPwsUpdEtwsInSib1SchedInfoLst
(
 WrCellCb              *cellCb,
 WrSibAction           action,
 WrPwsTranientInfo     *transInfo
 )
{
   S16       ret = ROK;
   U8        retransSiIndex = 0;

   /* Configure SIB1 scheduling info. */
   if (WR_PBM_SEND_NONE == wrPbmUpdSiSchedInfoLstInSIB1(cellCb,
            action,
            transInfo->wrngSibType,
            &retransSiIndex))
   {
      RLOG0(L_ERROR,"wrPbmUpdSiSchedInfoLstInSIB1:SIB1 SI schedulinginfo"
         "update failed because of unknown periodicity.");
      RETVALUE(RFAILED);
   }

   if(transInfo->wrngType == WR_PWS_SEC_NOTIFICATION)
   {
      cellCb->warningInfo->sib11SiIdx = retransSiIndex;
   }
   else if(transInfo->wrngType == WR_PWS_PRI_NOTIFICATION)
   {
      cellCb->warningInfo->sib10SiIdx = retransSiIndex;
   }
   else if(transInfo->wrngType == WR_PWS_BOTH_NOTIFICATIONS)
   {
      cellCb->warningInfo->sib10SiIdx = retransSiIndex;

      if(transInfo->validSecEtws)
      {
         transInfo->wrngSibType = WR_SIB_11;

         /* Configure SIB1 scheduling info. */
         if (WR_PBM_SEND_NONE == wrPbmUpdSiSchedInfoLstInSIB1(cellCb,
                  action,
                  transInfo->wrngSibType,
                  &retransSiIndex))
         {
            RLOG0(L_ERROR,"wrPbmUpdSiSchedInfoLstInSIB1:SIB1 SI schedulin"
               "info update failed because of unknown periodicity.");
            RETVALUE(RFAILED);
         }
         cellCb->warningInfo->sib11SiIdx = retransSiIndex;
      }
   } 

   RETVALUE(ret);
}/*wrPwsUpdEtwsInSib1SchedInfoLst*/


/** @brief This function is used to find the warning areas in which received
 * ETWS warning need to be broadcasted.
 *
 * @details This function is used find the warning areas in which received
 *  ETWS warning need to be broadcasted.
 *
 *
 *     Function: wrPwsEtwsFindWrngArea
 *
 *         Processing steps:
 *         - This function will find out the warning areas in which the warning
 *         message needs to be broadcasted based on whether warning area list ie
 *         is recived or not.
 *
 * @param [in] warnAreaLst : Warning Area List
 * @param [in] warningInfo : Warning Info Structre
 * @param [in] cellCb : Cell control block
 * @param [in] transInfo : Transient Info
 * 
 * @return S16
 *    -# Success : ROK
 *    -# Failure : RFAILED
 */
PRIVATE S16 wrPwsEtwsFindWrngArea
(
 SztWarningAreaLst         *warnAreaLst,
 WrPwsWarningInfo          *warningInfo,
 WrCellCb                  *cellCb,
 WrPwsTranientInfo         *transInfo
 )
{
   S16                       ret = ROK;

   /* Validate the received warning message. Find out if any duplicate 
      ETWS warning message is going on. */ 
   if(warnAreaLst != NULLP)
   {
      RLOG0(L_DEBUG,"Warning Area List IE present in the received"
         "write-replace warning.");

      ret = wrPwsProcWrngWithWrngAreaList(warnAreaLst, 
            warningInfo, cellCb, transInfo);
      if(ret == RFAILED)
      {
         RLOG0(L_ERROR,"wrPwsProcWrngWithWrngAreaList:Failed to store the" 
            "received warning message.");
         RETVALUE(RFAILED);
      }
   }
   else
   {
      ret = wrPwsProcWrngWithoutWrngAreaList( warningInfo, cellCb, transInfo);
      if(ret == RFAILED)
      {
         RLOG0(L_ERROR,"wrPwsProcWrngWithoutWrngAreaList:Failed to store the"
            "received warning message.");
         RETVALUE(RFAILED);
      }
   }

   RETVALUE(ret);
} /* wrPwsEtwsFindWrngArea */


/** @brief This function is used to broadcast the warning message.
 *
 * @details This function is used to broadcast the warning message.
 *
 *     Function: wrPwsEtwsProcBroadcast
 *
 *         Processing steps:
 *         - For the new warning message, add the SIB10/11 to the SIB1
 *         scheduling info list.
 *         - Configure MAC with SIB1.
 *         - Start Paging with etws indication.
 *         - Start the warning message.
 *
 * @param [in] warningInfo : Warning Info Structre
 * @param [in] cellCb : Cell control block
 * @param [in] transInfo : Transient info
 * 
 * @return S16
 *    -# Success : ROK
 *    -# Failure : RFAILED
 */
PRIVATE S16 wrPwsEtwsProcBroadcast
(
 WrPwsWarningInfo          *warningInfo,
 WrCellCb                  *cellCb,
 WrPwsTranientInfo         *transInfo
 )
{
   S16                       ret = ROK;

   /* Find out the specific cells in which to start the broadcast and paging. */ 
   if(transInfo->validCell[transInfo->cellIdx] == TRUE)
   {
      if(!transInfo->isDuplicate)
      {
         if(RFAILED == wrPwsUpdEtwsInSib1SchedInfoLst(cellCb,
                  WR_SIB_ACTION_ADD,
                  transInfo))
         {
            RLOG0(L_ERROR,"wrPwsUpdEtwsInSib1SchedInfoLst:SIB1 SI schedulin"
               "info update failed.");
            RETVALUE(RFAILED);
         }

         /* Configure MAC with updated SIB1 information. */
         if (wrEmmPbmSib1Config(cellCb->cellId, transInfo->cfgType) != ROK)
         {
            RLOG0(L_ERROR,"wrEmmPbmSib1Config:SIB1 SI schedulin info"
               "configuration failed.");
            RETVALUE(RFAILED);
         }

         /* Send the paging message with the ETWS indication. */  
         ret = wrPwsSndPagingwithEtwsInd(cellCb);
         if(ret == RFAILED)
         {
            RLOG0(L_ERROR,"wrPwsSndPagingwithEtwsInd:Failed to start paging"
               "with ETWS indication.");
            RETVALUE(RFAILED);
         }

         if ( RFAILED == wrPbmStartWarningMsg(cellCb, transInfo->wrngToProcess))
         {
            RLOG0(L_ERROR,"wrPbmStartWarningMsg:Unable to process the warning"
               "message.");
            RETVALUE(RFAILED);
         }
      }
      else
      {
         /* Duplicate ETWS received. */
         RLOG0(L_DEBUG,"Duplicate ETWS. Ignore the message.");
      }
   } 

   RETVALUE(ret);
} /* wrPwsEtwsProcBroadcast */


/** @brief This function is used to process CMAS warning message.
 *
 * @details This function is used to process CMAS warning message.
 *
 *     Function: wrPwsCmasProcessWrng
 *
 *         Processing steps:
 *         - Validate the CMAS notification.
 *         - Store the received warning message into cellCb.
 *         - Fill the Write-replace warning reaponse message and send it to the
 *         MME.
 *         - Configure MAC.
 *
 * @param [in] warningInfo : Warning Info Structre
 * @param [in] warnAreaLst : Warning Area List
 * @param [in] mmeCb : MME control block
 * 
 * @return S16
 *    -# Success : ROK
 *    -# Failure : RFAILED
 */
PRIVATE S16 wrPwsCmasProcessWrng
(
 WrPwsWarningInfo             *warningInfo,
 SztWarningAreaLst            **warnAreaLst,
 WrMmeCb                      *mmeCb
 )
{
   U8                        retransSiIndex = 0;
   U16                       cellIdx = 0;
   U32                       msgId = 0, serialNum = 0;
   S16                       ret;
   WrCellCb                  *cellCb = NULLP;
   Bool                      validCell[wrEmmCb.numCells];
   Bool                      broadCastCompletedIePres = FALSE;
   WrPwsCmasInfo             *tempCmas = NULLP, *cmasNode = NULLP;
   WrEmmCb                   *emmCb;

   emmCb = &wrEmmCb;

   for (cellIdx = 0; cellIdx <  wrEmmCb.numCells; cellIdx++)
   {
      validCell[cellIdx] = FALSE;
   } 

   /* Validate the CMAS notification. */
   ret =  wrPwsValidateCmasNotif(warningInfo);
   if( RFAILED == ret )
   {
      /* The received CMAS message shall not be broadcasted.*/
      /* Fill the Write-replace warning reaponse message and send it to the MME. */
      ret = wrPwsFillAndSndWrngResp(*(emmCb->cellCb), warningInfo, mmeCb, FALSE, validCell); 
      if(ret == RFAILED)
      {
          RLOG0(L_ERROR,"Failed to send warning response message.");
         RETVALUE(RFAILED);
      }

      RLOG0(L_ERROR,
         "The broadcast of this warning message shall not be commensed.");
      RETVALUE(RFAILED);
   }

   for (cellIdx = 0; cellIdx <  wrEmmCb.numCells; cellIdx++)
   { 
      cellCb =wrEmmCb.cellCb[cellIdx]; 

      /* Validate the received warning message. Find out if any duplicate 
         ETWS warning message is going on. */ 
      if(*warnAreaLst != NULLP)
      {
         ret = wrPwsSelectBroadcastArea(*warnAreaLst, warningInfo, 
               cellCb, WR_PWS_CMAS_NOTIFICATION);

         /* If validation is successful. */ 
         if(ROK == ret)
         {
            /* Mark the cell. */
            validCell[cellIdx] = TRUE;
            broadCastCompletedIePres = TRUE;

            /* Store the received warning message into cellCb. */
            ret = wrPwsStoreWrngInfo(warningInfo, cellCb, TRUE);
            if(ret == RFAILED)
            {
               RLOG0(L_ERROR,"wrPwsStoreWrngInfo:Failed to store the received" 
                  "warning message.");
               RETVALUE(RFAILED);
            }
         }
         else if(RFAILED == ret)
         {
            /* If the flow reaches here then it means already same CMAS warning message
               is going on in the particular warning area. */
            RLOG0(L_DEBUG,"Duplicate CMAS warning message received. It will" 
               "not be broadcasted.");

         } /* else */
         else if(ret == WR_PWS_NO_MATCH_FOUND)
         { 
            /* No match found for warning area type.*/
            /*broadCastCompletedIePres = FALSE;*/
            RLOG0(L_DEBUG,"No match found for the CMAS.");
         }
      }
      else
      {
         /* Warning Area List IE not present. */
         if(NULLP != cellCb->warningInfo)
         {
            ret = wrPwsValidateWarningNotif(warningInfo, cellCb->warningInfo, TRUE);
            if(ret != ROK)
            {
              RLOG0(L_DEBUG,"wrPwsValidateWarningNotif:Duplicate CMAS warning" 
                 "message received. It will not be broadcasted.");
            }
            else
            {
               validCell[cellIdx] = TRUE;
               broadCastCompletedIePres = TRUE;

               /* Store the received warning message into cellCb. */
               ret = wrPwsStoreWrngInfo(warningInfo, cellCb, TRUE);
               if(ret == RFAILED)
               {
                  RLOG0(L_ERROR,
                     "Unable to process the warning message. ");
                  RETVALUE(RFAILED);
               }
            }
         }
         else
         {
            /* This is the first warning message. */
            validCell[cellIdx] = TRUE;
            broadCastCompletedIePres = TRUE;

            /* Store the received warning message into cellCb. */
            ret = wrPwsStoreWrngInfo(warningInfo, cellCb, TRUE);
            if(ret == RFAILED)
            {
               RLOG0(L_ERROR,"wrPwsStoreWrngInfo:Failed to store the received"
                  "warning message.");
               RETVALUE(RFAILED);
            }
         }
      }
   } /* end for */

   /* Free the CMAS node in the received warning info. */
   /* Get the CMAS list. */
   tempCmas = wrPwsUtlGetRcvdCmasInfo(warningInfo);
   if (NULLP != tempCmas)
   {
      msgId = tempCmas->wrngMsgId;
      serialNum = tempCmas->wrngSerialNum;
   }    

   /* Fill the Write-replace warning reaponse message and send it to the MME. */
   ret = wrPwsFillAndSndWrngResp(*(emmCb->cellCb), warningInfo, mmeCb, 
         broadCastCompletedIePres, validCell);
   if(ret == RFAILED)
   {
      RLOG0(L_ERROR,"wrPwsFillAndSndWrngResp:Failed to send warning"
         "response message.");
      RETVALUE(RFAILED);
   }

   for (cellIdx = 0; cellIdx <  wrEmmCb.numCells; cellIdx++)
   { 
      /* Find out the specific cells in which to start the broadcast and paging. */ 
      if(validCell[cellIdx] == TRUE)
      {
         RLOG0(L_DEBUG,"Valid cell found to broadcast");
         cellCb =wrEmmCb.cellCb[cellIdx]; 
         /* If this is the only CMAS . */
         /* SRI :: ccpu00139418 : Check if any other CMAS is on going */
         if(FALSE == cellCb->warningInfo->isCmasInProgress)
         {
            /* Get the CMAS node. */
            cmasNode = wrPwsUtlGetDupCmas(cellCb, msgId, serialNum);

            /* update the currentCmas. */
            cellCb->warningInfo->currentCmas = cmasNode;

            /* Configure SIB1 scheduling info. */
            if (WR_PBM_SEND_NONE == wrPbmUpdSiSchedInfoLstInSIB1(cellCb,
                     WR_SIB_ACTION_ADD,
                     WR_SIB_12,
                     &retransSiIndex))
            {
               RLOG0(L_ERROR,"wrPbmUpdSiSchedInfoLstInSIB1:SIB1 SI scheduling"
                  "info update failed.");
               RETVALUE(RFAILED);
            }

            cellCb->warningInfo->sib12SiIdx = retransSiIndex;

            /* Configure MAC with updated SIB1 information. */
            if (wrEmmPbmSib1Config(cellCb->cellId, WR_SIB1_CMAS_TRANS_ID) != ROK)
            {
               RLOG0(L_ERROR,"wrEmmPbmSib1Config:SIB1 SI schedulin info"
                  "configuration failed.");
               RETVALUE(RFAILED);
            }

            /* Send the paging message with the CMAS indication. */  
            ret =  wrPwsSndPagingwithCmasInd(cellCb);
            if(ret == RFAILED)
            {
               RLOG0(L_ERROR,"wrPwsSndPagingwithCmasInd:Failed to start paging" 
                  "with CMAS indication.");
               RETVALUE(RFAILED);
            }

            if ( RFAILED == wrPbmStartWarningMsg(cellCb, WR_START_CMAS_IND))
            {
               RLOG0(L_ERROR,"wrPbmStartWarningMsg:Unable to process the"
                   "warning message.");
               RETVALUE(RFAILED);
            }
            /* ccpu00139418 */
            cellCb->warningInfo->isCmasInProgress = TRUE;
         }
      }
   } /* end for */

   RETVALUE(ROK);
} /* wrPwsCmasProcessWrng */


/** @brief This function is a utility function to get the received CMAS message.
 *
 * @details This function is a utility function to get the received CMAS
 * message.
 *
 *     Function: wrPwsUtlGetRcvdCmasInfo
 *
 *         Processing steps:
 *
 * @param [in] warningInfo : Warning Info Structre
 * 
 * @return S16
 *    -# Success : ROK
 *    -# Failure : RFAILED
 */
PRIVATE WrPwsCmasInfo* wrPwsUtlGetRcvdCmasInfo
(
 WrPwsWarningInfo             *warningInfo
 )
{
   WrPwsCmasInfo             *cmasInfoRcvd = NULLP;
   CmLListCp                 *cmasLstRcvd = NULLP;
   CmLList                   *cmasLstLnkRcvd = NULLP;

   cmasLstRcvd = &(warningInfo->cmasLst);
   CM_LLIST_FIRST_NODE(cmasLstRcvd, cmasLstLnkRcvd);

   if (NULLP != cmasLstLnkRcvd)
   {
      cmasInfoRcvd = (WrPwsCmasInfo *)(cmasLstLnkRcvd->node);
   }

   RETVALUE(cmasInfoRcvd);

} /* wrPwsUtlGetRcvdCmasInfo */

/** @brief This function is a utility function to get the duplicate CMAS message.
 *
 * @details This function is a utility function to get the duplicate CMAS
 * message.
 *
 *     Function: wrPwsUtlGetDupCmas
 *
 *         Processing steps:
 *
 * @param [in] cellCb : Cell Control Block
 * @param [in] msgId : Message ID
 * @param [in] serialNum : Serial Number
 * 
 * @return S16
 *    -# Success : ROK
 *    -# Failure : RFAILED
 */
PRIVATE WrPwsCmasInfo* wrPwsUtlGetDupCmas
(
 WrCellCb                  *cellCb,
 U32                       msgId,
 U32                       serialNum
 )
{
   CmLListCp                  *cmasLst = NULLP;
   CmLList                    *cmasLstLnk = NULLP;
   WrPwsCmasInfo              *cmasInfoPtr = NULLP;

   if(cellCb->warningInfo != NULLP)
   {
      /* check for each cmas node. */
      cmasLst = &(cellCb->warningInfo->cmasLst);

      CM_LLIST_FIRST_NODE(cmasLst, cmasLstLnk);

      while (NULLP != cmasLstLnk)
      {
         cmasInfoPtr = (WrPwsCmasInfo *)(cmasLstLnk->node);
         if(NULLP != cmasInfoPtr)
         {
            if ((cmasInfoPtr->wrngMsgId == msgId) &&
                  (cmasInfoPtr->wrngSerialNum == serialNum))
            {
               RETVALUE(cmasInfoPtr);
            }
         }
         cmasLstLnk = cmasLstLnk->next;
      } /* end while */
   }
   /*ccpu00128704 : Shuold Return Null if there is no cmas node with given msgId and SerialNo.*/
   RETVALUE (NULLP);
}/* wrPwsUtlGetDupCmas */

/** @brief This function is a utility function to get the next CMAS message.
 *
 * @details This function is a utility function to get the next CMAS
 * message.
 *
 *     Function: wrPwsGetNextNode
 *
 *         Processing steps:
 *
 * @param [in] cmasLst : CMAS List
 * @param [in] cmasInfo : CMAS Info
 * 
 * @return S16
 *    -# Success : ROK
 *    -# Failure : RFAILED
 */
PUBLIC WrPwsCmasInfo* wrPwsGetNextNode
(
 CmLListCp                 *cmasLst,
 WrPwsCmasInfo             *cmasInfo
 )
{
   CmLList                    *cmasLstLnk = NULLP;
   WrPwsCmasInfo              *cmasInfoPtr = NULLP;

   /* check for each cmas node. */
   CM_LLIST_FIRST_NODE(cmasLst, cmasLstLnk);

   while (NULLP != cmasLstLnk)
   {
      cmasInfoPtr = (WrPwsCmasInfo *)(cmasLstLnk->node);
      if(NULLP != cmasInfoPtr)
      {
         cmasInfoPtr = (WrPwsCmasInfo *)(cmasLstLnk->node);
         if ((cmasInfoPtr->wrngMsgId == cmasInfo->wrngMsgId) &&
               (cmasInfoPtr->wrngSerialNum == cmasInfo->wrngSerialNum))
         {
            if(NULLP != cmasLstLnk->next)
            {
               cmasLstLnk = cmasLstLnk->next;
               cmasInfoPtr = (WrPwsCmasInfo *)(cmasLstLnk->node);
               RETVALUE(cmasInfoPtr);
            }
            else
            {
               CM_LLIST_FIRST_NODE(cmasLst, cmasLstLnk);
               if (NULLP != cmasLstLnk)
               {
                  cmasInfoPtr = (WrPwsCmasInfo *)(cmasLstLnk->node);
                  RETVALUE(cmasInfoPtr);
               }
            }
         }
      }
      cmasLstLnk = cmasLstLnk->next;
   } /* end while */

   RETVALUE(cmasInfoPtr);
}/* wrPwsUtlGetDupCmas */
#ifdef DEBUGP
/** @brief This function is a utility function to print the segment PDU.
 *
 * @details This function is a utility function to print the segment PDU.
 *
 *     Function: wrPwsPrntPdu
 *
 *         Processing steps:
 *            - Go through the PDU list corresponding to each segment and print
 *            the segment.
 *
 * @param [in] pduLst : PDU List
 * 
 * @return S16
 *    -# Success : ROK
 *    -# Failure : RFAILED
 */
PUBLIC S16 wrPwsPrntPdu
(
 CmLListCp                  *pduLst
 )
{
   MsgLen                    msgLen;
   CmLList                    *pduLnk   = NULLP;
   WrPwsSegmentInfo           *pdu = NULLP;

   /* Get the first pdu. */
   CM_LLIST_FIRST_NODE(pduLst, pduLnk);

   while (NULLP != pduLnk)
   {
      pdu = (WrPwsSegmentInfo*)(pduLnk->node);

      RLOG0(L_INFO,"wrPwsPrntPdu : segment list node : \n");
      SPrntMsg(pdu->pdu, 0, 0);
      if (SFndLenMsg(pdu->pdu, &msgLen) != ROK)
         RETVALUE(RFAILED);

      RLOG1(L_INFO,"wrPwsPrntPdu : MsgLen1 = %d\n",msgLen);
      pduLnk = pduLnk->next;
   }

   RETVALUE(ROK);
}/* wrPwsPrntPdu */
#endif

/** @brief This function is a utility function to copy the segment list.
 *
 * @details This function is a utility function to copy the segment list.
 *
 *     Function: wrPwsCpySegLst
 *
 *         Processing steps:
 *
 * @param [in] cfgReq
 * @param [in] pduLstSrc : PDU List
 * 
 * @return S16
 *    -# Success : ROK
 *    -# Failure : RFAILED
 */
PUBLIC S16 wrPwsCpySegLst
(
 RgrWarningSiCfgReqInfo     *cfgReq,
 CmLListCp                  *pduLstSrc
 )
{
   CmLList                    *pduLnkSrc   = NULLP;
   WrPwsSegmentInfo           *pdu = NULLP;
   WrPwsSegmentInfo           *segInfo = NULLP;

   /* Get the first pdu. */
   CM_LLIST_FIRST_NODE(pduLstSrc, pduLnkSrc);

   while (NULLP != pduLnkSrc)
   {
      pdu = (WrPwsSegmentInfo*)(pduLnkSrc->node);

      WR_ALLOC(&segInfo, sizeof(WrPwsSegmentInfo));
      if (segInfo == NULLP)
      {
         RLOG0(L_FATAL, "Memory allocation failed.");
         RETVALUE(RFAILED);
      }

      /* Allocate memory for pdu in the new message segment. */
      WR_ALLOCMBUF(wrCb.mem.region, wrCb.mem.pool, &(segInfo->pdu));
      if (segInfo->pdu == NULLP)
      {
         RLOG0(L_FATAL, "Memory allocation failed.");
         RETVALUE(RFAILED);
      }

      SCpyMsgMsg(pdu->pdu,wrCb.mem.region, wrCb.mem.pool , &(segInfo->pdu));

      /* Adjust the link node. */
      segInfo->cmasSegPduLstLnk.node = (PTR)segInfo;
      segInfo->cmasSegPduLstLnk.node = (PTR)segInfo;

      cmLListAdd2Tail(&(cfgReq->siPduLst),
            &(segInfo->cmasSegPduLstLnk));

      pduLnkSrc = pduLnkSrc->next;
   }

   RETVALUE(ROK);
}/* wrPwsCpySegLst */

/** @brief This function is used to validate the received CMAS notification
 * against abnormal conditions.
 *
 * @details This function is used to validate the received CMAS notification
 * against abnormal condition. The function dcides the behavior based on number
 * of broadcast and repetition period values.
 *
 *     Function: wrPwsValidateCmasNotif
 *
 *         Processing steps:
 *         - If rpetiotion period and number of broadcast requested is 0 then cmas
 *         messege will not be broadcasted.
 *         - If repetition period is 0 and number of broadcast requested is not
 *         equal to 1 then cmas message will not be broadcasted.
 *
 * @param [in] warnInfo 
 * 
 * @return S16
 *    -# Success : ROK
 *    -# Failure : RFAILED
 */
PRIVATE S16 wrPwsValidateCmasNotif
(
 WrPwsWarningInfo            *warnInfo
 )
{
   CmLListCp                  *cmasLst = NULLP;
   CmLList                    *cmasLstLnk = NULLP;
   WrPwsCmasInfo              *cmasInfoPtr = NULLP;

   cmasLst = &(warnInfo->cmasLst);

   CM_LLIST_FIRST_NODE(cmasLst, cmasLstLnk);

   if (NULLP != cmasLstLnk)
   {
      cmasInfoPtr = (WrPwsCmasInfo *)(cmasLstLnk->node);
      if(NULLP != cmasInfoPtr)
      {
         if(cmasInfoPtr->wrngMsgInfo.numBroadcastReq == 0)
         {
            if(cmasInfoPtr->wrngMsgInfo.repetitionPeriod ==0)
            {
               RLOG0(L_ERROR,
                  " The CMAS message will not be"
                  " broadcasted as repetition period and number of"
                  " broadcast requested is 0. ");
               /* The received warning message shall not be broadcasted. */
               RETVALUE(RFAILED); 
            }
            else
            {
               /* The received warning message shall be broadcasted until
                  a kill request is received from the MME for that message. */
               cmasInfoPtr->waitToStop = TRUE;
            }
         } /* end if numBroadcastReq is 0. */

         if(cmasInfoPtr->wrngMsgInfo.repetitionPeriod == 0)
         {
            if(cmasInfoPtr->wrngMsgInfo.numBroadcastReq != 1)
            {
               RLOG0(L_ERROR,
                  " wrPwsValidateCmasNotif : The CMAS message will not be"
                  " broadcasted as repetition period is 0 and number of "
                  "broadcast requested is not equal to 1 ");
               /* The received warning message shall not be broadcasted. */
               RETVALUE(RFAILED); 
            }       
         } /* end if repetitionPeriod is 0. */
      }
      else
      {
         RLOG0(L_INFO,"No CMAS message present in the system.");
         RETVALUE(RFAILED); 
      }
   } /* end if */
   else
   {
       RLOG0(L_INFO,"No CMAS message present in the system. ");
   }
   RETVALUE(ROK);
} /* end of wrPwsValidateCmasNotif */

/** @brief This function function is used to store the validated received
 * warning message to the cell control block warning info structure.
 *
 * @details This function is used to store the validated received warning
 * message to the cell control block info structure. 
 *
 *     Function: wrPwsStoreWrngInfo
 *
 *         Processing steps:
 *
 * @param [in] warningInfo : WrPwsWarningInfo to be stored
 * @param [in] cellCb : cell control block
 * @param [in] isCmas : falg indicating whether the message in CMAS or not
 * 
 * @return S16
 *    -# Success : ROK
 *    -# Failure : RFAILED
 */
PRIVATE S16 wrPwsStoreWrngInfo
(
  WrPwsWarningInfo             *warningInfo,
  WrCellCb                     *cellCb,
  Bool                         isCmas
)
{
   S16                        ret = ROK;
   Bool                       isFirstWrng = TRUE;

   /* If this is the first warning message. */
   if(NULLP == cellCb->warningInfo)
   {
      isFirstWrng = TRUE;
      RLOG0(L_INFO,"Storing the first warning message. ");

      /* Allocate the memory for warning info within cellCb. */
       WR_ALLOC(&(cellCb->warningInfo), sizeof(WrPwsWarningInfo));
       if(NULLP == cellCb->warningInfo)
       {
          RLOG0(L_FATAL, "Memory allocation failed.");
          RETVALUE(RFAILED);
       }

      /* Initialize the memory. */
       cmMemcpy((U8 *)(cellCb->warningInfo),
                  (U8 *)warningInfo,
                  sizeof(WrPwsWarningInfo));
   }
   else
   {
      RLOG0(L_INFO,"Storing the new warning message. ");
      isFirstWrng = FALSE;
   }

   if(isCmas)
   {
       ret = wrPwsStoreCmasWrng(cellCb, warningInfo, isFirstWrng);
       if ( RFAILED == ret)
       {
           RLOG0(L_ERROR,"wrPwsStoreCmasWrng:Failed to store CMAS warning"
              "message.");
           RETVALUE(RFAILED);
       }
   }
   else if(warningInfo->etwsInfo.notificationType == WR_PWS_SEC_NOTIFICATION)
   {
       ret = wrPwsStoreSecEtwsWrng(cellCb, warningInfo, isFirstWrng);
       if ( RFAILED == ret)
       {
          RLOG0(L_ERROR,"wrPwsStoreSecEtwsWrng:Failed to store Secondary ETWS");
          RETVALUE(RFAILED);
       }
   }
   else if(warningInfo->etwsInfo.notificationType == WR_PWS_PRI_NOTIFICATION)
   {
       ret = wrPwsStorePrimEtwsWrng(cellCb, warningInfo, isFirstWrng);
       if ( RFAILED == ret)
       {
           RLOG0(L_ERROR,"wrPwsStorePrimEtwsWrng:Failed to store primary ETWS");
           RETVALUE(RFAILED);
       }
   }
   else if(warningInfo->etwsInfo.notificationType == WR_PWS_BOTH_NOTIFICATIONS)
   {
       ret = wrPwsStorePrimEtwsWrng(cellCb, warningInfo, isFirstWrng);
       if ( RFAILED == ret)
       {
          RLOG0(L_ERROR,"wrPwsStorePrimEtwsWrng:Failed to store primary ETWS");
          RETVALUE(RFAILED);
       }
 
       ret = wrPwsStoreSecEtwsWrng(cellCb, warningInfo, isFirstWrng);
       if ( RFAILED == ret)
       {
           RLOG0(L_ERROR,"wrPwsStoreSecEtwsWrng:Failed to store Secondary"
              "ETWS");
           RETVALUE(RFAILED);
       }
   } 
      
   RETVALUE(ROK);
} /* wrPwsStoreWrngInfo */


/** @brief This function function is used to store the primary ETWS warning
 * message to cell control block.
 *
 * @details This function is used to store the primary ETWS warning
 *   message to cell control block.
 *
 *     Function: wrPwsStorePrimEtwsWrng
 *
 *         Processing steps:
 *
 * @param [in] cellCb : cell control block
 * @param [in] warningInfo : warning info
 * @param [in] isFirstWrng : Boolean
 * 
 * @return S16
 *    -# Success : ROK
 *    -# Failure : RFAILED
 */
PRIVATE S16 wrPwsStorePrimEtwsWrng 
(
   WrCellCb             *cellCb,
   WrPwsWarningInfo     *warningInfo,
   Bool                isFirstWrng
)
{
    S16                 ret = ROK;
    Buffer              *pdu = NULLP;

    RLOG0(L_DEBUG,"Storing the Primary ETWS  warning message.");

    /* If number of broadacst is 0 for primary ETWS, then replace it with the
     * configured value.  */
    if(warningInfo->etwsInfo.primEtwsInfo.numBroadcastReq == 0)
    {
       warningInfo->etwsInfo.primEtwsInfo.numBroadcastReq =
                                cellCb->sib10Info.sib10NumOfBroadcasts;
    }

    /* If repetition period is 0 for primary ETWS, then replace it with the
     * configured value.  */
    if(warningInfo->etwsInfo.primEtwsInfo.repetitionPeriod == 0)
    {
       warningInfo->etwsInfo.primEtwsInfo.repetitionPeriod =
                              cellCb->sib10Info.sib10RepetitionPeriod;
    }

    if(!isFirstWrng) 
    {
       if(cellCb->warningInfo->etwsInfo.wrngMsgId != WR_PWS_INVALID_MSGID) 
       {
          if(cellCb->warningInfo->isPrimEtwsInProgress)
          {
             wrEmmPbmWrngStopSi(cellCb->cellId, WR_PBM_SIB10_SI_INDEX);
          }
          if(cellCb->warningInfo->isSecEtwsInProgress)
          {
              wrEmmPbmWrngStopSi(cellCb->cellId, WR_PBM_SIB11_SI_INDEX);
          }
          if(ROK !=  wrPwsStopEtwsBasedOnType(cellCb))
          {
             RLOG0(L_WARNING,"wrPwsStopEtwsBasedOnType:"
                "Unable to stop ongoing primary ETWS.");
          }
       }

       /* Copy the ETWS information.*/
       cmMemcpy((U8 *)&(cellCb->warningInfo->etwsInfo),
                    (U8 *)&(warningInfo->etwsInfo),
                    sizeof(WrPwsEtwsInfo));
         
       /* Update the current CMAS in the warning info. */
       cellCb->warningInfo->siBldInfo.crntCmas = NULLP;
    }

    /* Encode each PDU. */
    /* Allocate memory for the PDU. */
    WR_ALLOCMBUF(wrCb.mem.region, wrCb.mem.pool, &pdu);
    if ( pdu == NULLP)
    {
       RLOG0(L_FATAL, "Memory allocation failed.");
       WR_FREE(cellCb->warningInfo, sizeof(WrPwsWarningInfo));
       RETVALUE(RFAILED);
    }

    wrEmmPbmBldEncWrngSeg(cellCb->cellId, WR_PBM_SIB10_SI_INDEX, &pdu);
    if ( pdu == NULLP)
    {
       RLOG0(L_ERROR,"wrEmmPbmBldEncWrngSeg:Failed to encode the Buffer"
          "for primary ETWS.");
       WR_FREE(cellCb->warningInfo, sizeof(WrPwsWarningInfo));
       WR_FREEMBUF(pdu);
       RETVALUE(RFAILED);
    }

    /* Allocate memory for pdu in the new message segment. */
    WR_ALLOCMBUF(wrCb.mem.region, wrCb.mem.pool, 
             &(cellCb->warningInfo->etwsInfo.primEtwsInfo.pdu));
    if (cellCb->warningInfo->etwsInfo.primEtwsInfo.pdu == NULLP)
    {
       RLOG0(L_FATAL, "Memory allocation failed.");
       WR_FREE(cellCb->warningInfo, sizeof(WrPwsWarningInfo));
       WR_FREEMBUF(pdu);
       RETVALUE(RFAILED);
    }

    SCpyMsgMsg(pdu,wrCb.mem.region, wrCb.mem.pool , 
                 &(cellCb->warningInfo->etwsInfo.primEtwsInfo.pdu));

    RETVALUE(ret);
} /* wrPwsStorePrimEtwsWrng */    


/** @brief This function function is used to store the secondary ETWS warning
 * message to cell control block.
 *
 * @details This function is used to store the secondary ETWS warning
 *   message to cell control block.
 *
 *     Function: wrPwsStoreSecEtwsWrng
 *
 *         Processing steps:
 *
 * @param [in] cellCb : cell control block
 * @param [in] warningInfo : warning info
 * @param [in] isFirstWrng : Boolean
 * 
 * @return S16
 *    -# Success : ROK
 *    -# Failure : RFAILED
 */
PRIVATE S16 wrPwsStoreSecEtwsWrng 
(
   WrCellCb             *cellCb,
   WrPwsWarningInfo     *warningInfo,
   Bool                 isFirstWrng
)
{
    S16                 ret = ROK;

   RLOG0(L_DEBUG,"Storing the Secondary ETWS warning message.");

    if(!isFirstWrng)
     {
       if(warningInfo->etwsInfo.notificationType != WR_PWS_BOTH_NOTIFICATIONS) 
       {
          if(cellCb->warningInfo->etwsInfo.wrngMsgId != WR_PWS_INVALID_MSGID) 
          {
             if(cellCb->warningInfo->isPrimEtwsInProgress)
             {
                wrEmmPbmWrngStopSi(cellCb->cellId, WR_PBM_SIB10_SI_INDEX);
             }
             if(cellCb->warningInfo->isSecEtwsInProgress)
             {
                 wrEmmPbmWrngStopSi(cellCb->cellId, WR_PBM_SIB11_SI_INDEX);
             }
             if(ROK !=  wrPwsStopEtwsBasedOnType(cellCb))
             {
                RLOG0(L_WARNING,"wrPwsStopEtwsBasedOnType :"
                   "Unable to stop ongoing primary ETWS.");
             }
          }

          /* The incoming messgage is ETWS message. Add the ETWS information.*/
          cmMemcpy((U8 *)&(cellCb->warningInfo->etwsInfo),
                  (U8 *)&(warningInfo->etwsInfo),
                  sizeof(WrPwsEtwsInfo));
       }
    }

    /* Update the current CMAS in the warning info. */
    cellCb->warningInfo->siBldInfo.crntCmas =NULLP;

    /* Segment the message content and encode. */
    wrPwsSegWrngMsg(cellCb, &(cellCb->warningInfo->siBldInfo));

    RETVALUE(ret);
}/* wrPwsStoreSecEtwsWrng */    


/** @brief This function function is used to store the CMAS warning message.
 *
 * @details This function is used to store the CMAS warning message.
 *
 *     Function: wrPwsStoreCmasWrng
 *
 *         Processing steps:
 *
 * @param [in] cellCb : cell control block
 * @param [in] warningInfo : warning info
 * @param [in] isFirstWrng : Boolean
 * 
 * @return S16
 *    -# Success : ROK
 *    -# Failure : RFAILED
 */
PRIVATE S16 wrPwsStoreCmasWrng 
(
 WrCellCb             *cellCb,
 WrPwsWarningInfo     *warningInfo,
 Bool                isFirstWrng
 )
{
   S16                 ret = ROK;
   CmLListCp           *cmasLst = NULLP;
   CmLList             *cmasLstLnk = NULLP;
   WrPwsCmasInfo       *cmasInfoPtr = NULLP;

   RLOG0(L_DEBUG,"Storing the first CMAS warning message.");
   /* If the new incoming message is a CMAS message then add to the CMAS list. */
   cmasLst = &(warningInfo->cmasLst);

   CM_LLIST_FIRST_NODE(cmasLst, cmasLstLnk);

   if (NULLP != cmasLstLnk)
   {
      cmasInfoPtr = (WrPwsCmasInfo *)(cmasLstLnk->node);
      if(NULLP != cmasInfoPtr)
      {
         /* Update the current CMAS in the warning info. */
         cellCb->warningInfo->siBldInfo.crntCmas = cmasInfoPtr;

         /* Segment the message content and encode. */
         wrPwsSegWrngMsg(cellCb, &(cellCb->warningInfo->siBldInfo));

         if(isFirstWrng)
         {
            cmLListDelFrm(&(cellCb->warningInfo->cmasLst), 
                  &(cmasInfoPtr->cmasLstLnk));
         }
         /* Add the encoded segments to the segment PDU list with the cmas structure. */
         cmasInfoPtr->cmasLstLnk.node = (PTR)cmasInfoPtr;

         /* Add to the CMAS List in the sorted order. */
         wrPwsSortInsertCmas(&(cmasInfoPtr->cmasLstLnk), 
               &(cellCb->warningInfo->cmasLst));

         /* If this is the first CMAS then updae the currentCmas. */ 
         if(isFirstWrng)
         {
            cellCb->warningInfo->currentCmas = cmasInfoPtr;
         }
      }
      else
      {
         RLOG0(L_ERROR,"Failed to  get CMAS info.");
         WR_FREE(cellCb->warningInfo, sizeof(WrPwsWarningInfo));
         RETVALUE(RFAILED);
      }
   }

   RETVALUE(ret);
} /* wrPwsStoreCmasWrng */


/** @brief This function function is used to segment the warning message.
 *
 * @details This function is used to segment the warning message.
 *
 *     Function: wrPwsSegWrngMsg 
 *
 *         Processing steps:
 *
 * @param [in] cellCb : cell control block
 * @param [in] siInfo : SI Info 
 * 
 * @return S16
 *    -# Success : ROK
 *    -# Failure : RFAILED
 */
PRIVATE S16 wrPwsSegWrngMsg
(
 WrCellCb                     *cellCb,
 WrPwsSIBldInfo               *siInfo
 )
{
   Bool             isCmas =FALSE;
   U8               segSize = 0;
   U8               numSeg = 0; 
   S16              ret = ROK;
   U32              msgLen = 0;
   Buffer           *wrngContent = NULLP;
   Buffer           *pdu = NULLP;
   Buffer           *tmp2 = NULLP;
   WrPwsCmasInfo    *cmasWarnInfo = NULLP;
   WrPwsSegmentInfo *segInfo = NULLP; 
   MsgLen           msgSegLen = 0;

   if(siInfo->crntCmas != NULLP)
   {
      cmasWarnInfo = siInfo->crntCmas;
      isCmas = TRUE;
   }
   /* Calculate the segment size. */
   segSize = WR_PWS_WARNMSG_SEGSIZE; /* 1160 (145 bytes * 8 )bits. */

   WR_ALLOCMBUF(wrCb.mem.region, wrCb.mem.pool, &wrngContent); 
   if ( wrngContent == NULLP)
   {
      RLOG0(L_FATAL, "Memory allocation failed.");
      RETVALUE(RFAILED);
   }

   if(isCmas)
   { 
      msgLen = cmasWarnInfo->wrngMsgInfo.msgLen;

      /* convert the OSXL to Buffer */
      SAddPstMsgMult((Data *)(cmasWarnInfo->wrngMsgInfo.wrngMsgPtr),
            (MsgLen)(msgLen),
            (Buffer *)wrngContent);
   }
   else
   {
      msgLen = cellCb->warningInfo->etwsInfo.secEtwsInfo.msgLen;
      SAddPstMsgMult((Data *)(cellCb->warningInfo->etwsInfo.secEtwsInfo.wrngMsgPtr),
            (MsgLen)(msgLen),
            (Buffer *)wrngContent);
   }

   /* Now wrngContent has the complete warning message. */ 
   while(msgLen>0)
   {
      /* Segment the message. */
      ret = SSegMsg(wrngContent,segSize,&tmp2);

      if ( ret == RFAILED )
      {
         RLOG0(L_ERROR, "SSegMsg returned RFAILED.This is the last segment.");

         if ( tmp2 )
         {
            SPutMsg(tmp2);
         }
         siInfo->segNum = numSeg;
         siInfo->isLastSeg = TRUE;
         siInfo->segLen = segSize;
         siInfo->segment = wrngContent;
      }
      if( ret == ROK )
      {
         RLOG0(L_DEBUG,"SSegMsg returned ROK. This is not the last segment.");

         /* Fill the segmnt Info. */
         siInfo->segNum = numSeg;
         siInfo->isLastSeg = FALSE;
         siInfo->segLen = segSize;
         siInfo->segment = wrngContent;
      }
      else if( ROKDNA == ret )
      {
 
         RLOG0(L_INFO," SSegMsg returned ROKDNA. This is the last segment.");

         /*  Fill the segmnt Info. */
         siInfo->segNum = numSeg;
         siInfo->isLastSeg = TRUE;
         if (SFndLenMsg(wrngContent, &msgSegLen) != ROK)
         {
            WR_FREEMBUF(wrngContent);
            RETVALUE(RFAILED);
         }
         siInfo->segLen = msgSegLen;
         siInfo->segment = wrngContent;
      }

      /* Encode each PDU. */
      /* Allocate memory for the PDU. */
      WR_ALLOCMBUF(wrCb.mem.region, wrCb.mem.pool, &pdu);
      if ( pdu == NULLP)
      {
         RLOG0(L_FATAL, "Memory allocation failed.");
         WR_FREEMBUF(wrngContent);
         RETVALUE(RFAILED); 
      }

      if(isCmas)
      {
         wrEmmPbmBldEncWrngSeg(cellCb->cellId, WR_PBM_SIB12_SI_INDEX, &pdu);
      }
      else
      {
         wrEmmPbmBldEncWrngSeg(cellCb->cellId, WR_PBM_SIB11_SI_INDEX, &pdu);
      }

      if ( pdu == NULLP)
      {
         RLOG0(L_ERROR,"Failed to encode the Buffer.");
         WR_FREEMBUF(pdu);
         WR_FREEMBUF(wrngContent);
         RETVALUE(RFAILED);
      }

      /* Store the PDU in the PDU list. */
      /* Allocate memory for new message segment. */
      WR_ALLOC(&segInfo, sizeof(WrPwsSegmentInfo));
      if (segInfo == NULLP)
      {
         RLOG0(L_FATAL, "Memory allocation failed.");
         WR_FREEMBUF(pdu);
         WR_FREEMBUF(wrngContent);
         RETVALUE(RFAILED); 
      }

      /* Allocate memory for pdu in the new message segment. */
      WR_ALLOCMBUF(wrCb.mem.region, wrCb.mem.pool, &(segInfo->pdu));
      if (segInfo->pdu == NULLP)
      {
         RLOG0(L_FATAL, "Memory allocation failed.");
         WR_FREEMBUF(pdu);
         WR_FREEMBUF(wrngContent);
         WR_FREE(segInfo, sizeof(WrPwsSegmentInfo));
         RETVALUE(RFAILED); 
      }

      SCpyMsgMsg(pdu,wrCb.mem.region, wrCb.mem.pool , &(segInfo->pdu)); 

      /* Adjust the link node. */
      segInfo->cmasSegPduLstLnk.node = (PTR)segInfo;

      if(isCmas)
      {
         cmLListAdd2Tail(&(cmasWarnInfo->wrngMsgInfo.segPduLst), 
               &(segInfo->cmasSegPduLstLnk)); 
      }
      else
      {
         cmLListAdd2Tail(&(cellCb->warningInfo->etwsInfo.secEtwsInfo.segPduLst), 
               &(segInfo->cmasSegPduLstLnk)); 
      }

      /* update the msgLen. */
      if(tmp2)
      {
         if (SFndLenMsg(tmp2, (MsgLen*)&msgLen) != ROK)
            break;
         wrngContent  = tmp2;
         tmp2 = NULLP;
         numSeg = numSeg + 1;
      }
      else
      {
         /* update the pointers tmp1 and tmp2. */
         msgLen = 0;
      }
   } /* end while */

   /* Free the buffer. */ 
   WR_FREEMBUF(wrngContent);

   RETVALUE(ROK); 
} /* wrPwsSegWrngMsg */

/** @brief This function function is used to insert the cmas node to cmas list
 * in sorted order
 *
 * @details This function is used to insert the cmas node to cmas list in sorted
 * order based on the repetition period.
 *
 *     Function: wrPwsSortInsertCmas
 *
 *         Processing steps:
 *          - Serch the position of node to be inserted bad on the repetition
 *          period.
 *          - Insert the cmas node at proper place.
 *
 * @param [in] entryLnk
 * @param [in] lst
 * 
 * @return Void
 */
PRIVATE Void wrPwsSortInsertCmas
(
 CmLList                    *entryLnk,
 CmLListCp                  *lst
 )
{
   Bool                       ret = FALSE;
   CmLList                    *crntLnk = NULLP;

   CM_LLIST_FIRST_NODE(lst,crntLnk);

   /* traverse from start of list */
   while(crntLnk != NULLP)
   {
      /* check if the entry node repetition period is greater than 
         lnk node repetition period */
      ret = rmCmpRepPeriod((WrPwsCmasInfo*)crntLnk->node,
            (WrPwsCmasInfo*)entryLnk->node);
      if(ret == TRUE)
      {
         cmLListInsCrnt(lst, entryLnk);
         break;
      }
      CM_LLIST_NEXT_NODE(lst,crntLnk);
   }
   if(ret == FALSE)
   {
      cmLListAdd2Tail(lst, entryLnk);
   }

   RETVOID;
} /* wrPwsSortInsertCmas */

/** @brief This function function is used to compare the repition period of the
 * node to be inserted with an existing cmas node.
 *
 * @details This function is used to compare the repetition period of the node
 * to be inserted with an existing cmas node's repetition period.
 *
 *     Function: rmCmpRepPeriod 
 *
 *         Processing steps:
 *          - If repetition period of the cmas node to be inserted is greater
 *          than the existing node's repetition period then return FALSE.
 *          - Else return TRUE.
 *
 * @param [in] lnkNode
 * @param [in] entry
 * 
 * @return Void
 */
PRIVATE Bool rmCmpRepPeriod
(
 WrPwsCmasInfo             *lnkNode,
 WrPwsCmasInfo             *entry
 )
{
   if(entry->wrngMsgInfo.repetitionPeriod  > lnkNode->wrngMsgInfo.repetitionPeriod)
   {
      RETVALUE(FALSE);
   }
   else
   {
      RETVALUE(TRUE);
   }
} /* rmCmpRepPeriod*/

/** @brief This function function is used to validate the received warning
 * message based on message id and serial number.
 *
 * @details This function is used to validate the received warning message based
 * on the message id and serial number for both CMAS and ETWS.
 *
 *     Function: wrPwsValidateWarningNotif
 *
 *         Processing steps:
 *
 * @param [in] currWarnInfo
 * @param [in] prevWarnInfo
 * @param [in] isCmas
 * 
 * @return S16
 *    -# Success : ROK
 *    -# Failure : RFAILED
 */
PRIVATE S16 wrPwsValidateWarningNotif
(
 WrPwsWarningInfo            *currWarnInfo,
 WrPwsWarningInfo            *prevWarnInfo,
 Bool                        isCmas
 )
{
   CmLListCp                  *cmasLstCur = NULLP, *cmasLstPrev = NULLP;
   CmLList                    *cmasLstLnkCur = NULLP, *cmasLstLnkPrev = NULLP;
   WrPwsCmasInfo              *cmasInfoPtrCur = NULLP, *cmasInfoPtrPrev = NULLP;

   if(!isCmas)
   {
      if ((currWarnInfo->etwsInfo.wrngMsgId == prevWarnInfo->etwsInfo.wrngMsgId) &&
            (currWarnInfo->etwsInfo.wrngSerialNum == prevWarnInfo->etwsInfo.wrngSerialNum))
      {
          RLOG0(L_ERROR,"Validation for received ETWS warning message failed.");
         RETVALUE(RFAILED);
      } 
      else 
      {
         RETVALUE(ROK);
      }
   }
   else
   {
      /* check for each cmas node. */
      cmasLstPrev = &(prevWarnInfo->cmasLst);
      cmasLstCur = &(currWarnInfo->cmasLst); 

      CM_LLIST_FIRST_NODE(cmasLstPrev, cmasLstLnkPrev);
      CM_LLIST_FIRST_NODE(cmasLstCur, cmasLstLnkCur);

      while (NULLP != cmasLstLnkPrev)
      {
         cmasInfoPtrPrev = (WrPwsCmasInfo *)(cmasLstLnkPrev->node);
         if(NULLP != cmasInfoPtrPrev)
         {
            cmasInfoPtrCur = (WrPwsCmasInfo *)(cmasLstLnkCur->node);
            if ((cmasInfoPtrCur->wrngMsgId == cmasInfoPtrPrev->wrngMsgId) &&
                  (cmasInfoPtrCur->wrngSerialNum == cmasInfoPtrPrev->wrngSerialNum))
            {
               RLOG0(L_ERROR,"Validation for received CMAS warning"
                  "message failed.");
               RETVALUE(RFAILED);
            }
            cmasLstLnkPrev = cmasLstLnkPrev->next;
         }
      } /* end while */
   } /* end else */

   RETVALUE(ROK);
} /* end of wrPwsValidateWarningNotif */

/** @brief This function function is used to validate the received secondary
 * ETWS warning message against abnormal conditions.
 *
 * @details This function is used to validate the received secondary ETWS warning 
 * message against abnormal conditions. The behavior is decided based on the 
 * number of broadcast and repetition period values.
 *
 *     Function: wrPwsValidateSecondWarningNotif
 *
 *         Processing steps:
 *         - If number of broadcast requested is 0 then the secondary ETWS will
 *         not be broadcasted.
 *         - If repetition period is 0 and the number of broadcast requested is 
 *         not equal to 1 then the secondary ETWS will not be broadcasted.
 *
 * @param [in] warningInfo
 * 
 * @return S16
 *    -# Success : ROK
 *    -# Failure : RFAILED
 */
PRIVATE S16 wrPwsValidateSecondWarnNotif
(
 WrPwsWarningInfo            *warningInfo
 )
{
   /* Put here all the conditions in which case secondary ETWS shall
      not be broadcasted. */
   if (0 == warningInfo->etwsInfo.secEtwsInfo.numBroadcastReq)
   {
      RLOG0(L_ERROR,"Received secondary warning notification shall not be"
         "broadcasted as no of broadcast requested is 0.");
      RETVALUE(RFAILED);
   }
   if ((0 == warningInfo->etwsInfo.secEtwsInfo.repetitionPeriod) && 
         (warningInfo->etwsInfo.secEtwsInfo.numBroadcastReq != 1))
   {
      RLOG0(L_ERROR,"Received secondary warning notification shall not be"
         "broadcasted as repetition period is 0 and the no of broadcast"
         "requested is not 1.");
      RETVALUE(RFAILED);
   }
   RETVALUE(ROK);
} /* end of wrPwsValidateSecondWarnNotif */

/** @brief This function is used to mark the broadcast success cells 
 *         in the received warning area list.
 * @details This function is used to mark the broadcast intation success cells
 *          in the warning area list and also allocates the warningInfo dynamically 
 *          for selected cells and intiliase the necesary values.
 *          - The same list is used for preparing the warning response message 
 *            with broacast completed area list.
 * @param[in] warningReqPdu  : Received Warning Request.
 * @param[in] newWarningInfo : newly received warning information.
 * @param[in, out] cellCbLst : Cell Control Block.
 * @return S16 
 *     -# Success : ROK
 *     -# Failure : RFAILED 
 */
/* PH04_CMAS : This function will be modified as part of CMAS feature. */
PRIVATE S16 wrPwsSelectBroadcastArea
(
 SztWarningAreaLst           *warnAreaLst,
 WrPwsWarningInfo            *rcvdWarningInfo,
 WrCellCb                    *cellCb,
 U8                          wrngType
 )
{
   Bool        isCmas = FALSE;
   S16         ret = ROK;
   U8          eaType = 0;
   WrPwsCmasInfo *cmasInfoPtr = NULLP;

   if(wrngType == WR_PWS_SEC_NOTIFICATION)
   {
      eaType = rcvdWarningInfo->etwsInfo.secEtwsInfo.emergencyAreaType; 
   }
   else if(wrngType == WR_PWS_PRI_NOTIFICATION || 
         wrngType == WR_PWS_BOTH_NOTIFICATIONS) 
   {
      eaType = rcvdWarningInfo->etwsInfo.primEtwsInfo.emergencyAreaType; 
   }

   else if(wrngType == WR_PWS_CMAS_NOTIFICATION)
   {
      /* Get the CMAS node. */
      if(NULLP != (cmasInfoPtr = wrPwsUtlGetRcvdCmasInfo(rcvdWarningInfo)))
      {
         eaType = cmasInfoPtr->wrngMsgInfo.emergencyAreaType; 
         isCmas = TRUE;
      }
   }

   switch(eaType)
   {
      case WARNINGAREALST_CELLIDLST:
         {  
            if (TRUE == wrPwsIsServCellIdPresentInLst(cellCb,
                     &warnAreaLst->val.cellIDLst))
            {
               RLOG0(L_DEBUG,"Cell Id present in the list.");

               if(cellCb->warningInfo)
               {
                  /* validate and create pws information */
                  ret = wrPwsValidateWarningNotif(rcvdWarningInfo, cellCb->warningInfo, isCmas);
               }
            }
            else
            {
               RLOG0(L_DEBUG,"Cell Id  is not present in the list.");
               ret = WR_PWS_NO_MATCH_FOUND;
            }
            break;
         }
      case WARNINGAREALST_TRACKINGAREALSTFORWARNING:
         {
            if (TRUE == wrPwsIsServCellTAIPresentInLst(cellCb,
                     &warnAreaLst->val.trackingAreaLstforWarning))
            {
               RLOG0(L_DEBUG,"Cell Tracking area Id present in the list.");

               if(cellCb->warningInfo)
               {
                  /* validate and create pws information */
                  ret = wrPwsValidateWarningNotif(rcvdWarningInfo, cellCb->warningInfo, isCmas);
               }
            }
            else
            {  
               RLOG0(L_ERROR,"Cell Tracking area Id is not  present in the list.");
               ret = WR_PWS_NO_MATCH_FOUND;
            }
            break;
         }
      case WARNINGAREALST_EMERGENCYAREAIDLST:
         {
            if (TRUE == wrPwsIsServCellEAIDPresentInLst(cellCb,
                     &warnAreaLst->val.emergencyAreaIDLst, FALSE)) /* ccpu00128226 */ 
            {
               RLOG0(L_DEBUG,"Cell Emergency area Id present in the list.");

               if(cellCb->warningInfo)
               {
                  /* validate and create pws information */
                  ret = wrPwsValidateWarningNotif(rcvdWarningInfo, cellCb->warningInfo, isCmas);
               }
            }
            else
            {
               RLOG0(L_ERROR,"Cell Emergency area Id is not present in the list.");
               ret = WR_PWS_NO_MATCH_FOUND;
            }
            break;
         }
      case WR_PWS_EATYPE_NONE:
         {
            RLOG0(L_WARNING,"Emergency area type is not mentioned.");

            if(cellCb->warningInfo)
            {
               /* validate and create pws information */
               ret = wrPwsValidateWarningNotif(rcvdWarningInfo, cellCb->warningInfo, isCmas);
            }
            break;
         }
      default:
         {
            RLOG0(L_ERROR,"Emergency area type is invalid.");
            RETVALUE(WR_PWS_NO_MATCH_FOUND); 
         }
   } /* end of switch */
   RETVALUE(ret);
} /* end of wrPwsSelectBroadcastArea */

PRIVATE Bool wrPwsIsServCellIdPresentInLst
(
 WrCellCb                    *cellCb,
 SztECGILst                  *rcvdCellIdLst
 )
{
   SztEUTRAN_CGI             *ecgi         = NULLP;
   WrPlmnId                  *cellPlmnId   = NULLP;
   SztPLMNidentity           *plmnIe       = NULLP;
   U8                        numPlmnIds    = 0;
   WrPlmnId                  plmnId;
   U32                       plmnIndex;
   U32                       ecgiIdx;
   U32                       cellIdShifted;
   U8                        cellIdOct[4];

   cellIdShifted = cellCb->sib1.cellIdentity << 4;
   cellIdOct[0] = (cellIdShifted & 0xff000000) >> 24;
   cellIdOct[1] = (cellIdShifted & 0x00ff0000) >> 16;
   cellIdOct[2] = (cellIdShifted & 0x0000ff00) >> 8;
   cellIdOct[3] = (cellIdShifted & 0x000000ff) >> 0;

   numPlmnIds = cellCb->sib1.numPlmnIds;

   for (ecgiIdx = 0; ecgiIdx < rcvdCellIdLst->noComp.val; ecgiIdx++)
   {
      ecgi = &rcvdCellIdLst->member[ecgiIdx];
      /* compare serving cell cellId with received cell Id in ECGI */
      if ((cellIdOct[0] == ecgi->cell_ID.val[0]) &&
            (cellIdOct[1] == ecgi->cell_ID.val[1]) &&
            (cellIdOct[2] == ecgi->cell_ID.val[2]) &&
            (cellIdOct[3] == ecgi->cell_ID.val[3]))
      {
         /* Parse S1AP PLMN ID IE from ECGI IE */
         plmnIe = &ecgi->pLMNidentity;
         wrEmmMmeParsePlmnIe(plmnIe, &plmnId);

         /* compare the received PLMNID with the cell PLMNIDs */
         for (plmnIndex = 0; plmnIndex < numPlmnIds; plmnIndex++)
         {
            cellPlmnId = &cellCb->sib1.plmns[plmnIndex].plmnId;
            if(wrEmmPlmnPlmnsEqual(&plmnId, cellPlmnId)) 
            {
               RETVALUE(TRUE);
            }
         } /* end of for loop for serving cell PLMN Ids comparison */
      } /* end of if for Cell ID comparison */
   } /* end of for to traverse the received ECGI list */
   RETVALUE(FALSE);
} /* end of wrPwsIsServCellIdPresentInLst */ 


PRIVATE Bool wrPwsIsServCellTAIPresentInLst
(
 WrCellCb                    *cellCb,
 SztTAILstforWarning         *rcvdTAILst
 )
{
   SztTAI                    *rcvdTAI      = NULLP;
   WrPlmnId                  *cellPlmnId   = NULLP;
   SztPLMNidentity           *plmnIe       = NULLP;
   U8                        cellTac[2]    = {0};
   U8                        numPlmnIds    = 0;
   WrPlmnId                  rcvdPlmnId;
   U32                       plmnIndex;
   U32                       taiIndex;

   /* Get the Tracking Area Code,num of PLMNs and PLMN IDs from SIB1 */
   cellTac[1] = (U8)(cellCb->sib1.tac & 0xff);
   cellTac[0] = (U8)((cellCb->sib1.tac & 0xff00)>>8);

   numPlmnIds = cellCb->sib1.numPlmnIds;

   for (taiIndex = 0; (taiIndex < rcvdTAILst->noComp.val); taiIndex++)
   {
      rcvdTAI =  &rcvdTAILst->member[taiIndex];

      if ((cellTac[0] == rcvdTAI->tAC.val[0]) && 
            (cellTac[1] == rcvdTAI->tAC.val[1]))
      {
         /* Parse S1AP PLMN ID IE */
         plmnIe = &rcvdTAI->pLMNidentity;
         wrEmmMmeParsePlmnIe(plmnIe, &rcvdPlmnId);

         /* compare the received PLMNID with the cell PLMNIDs */
         for (plmnIndex = 0; plmnIndex < numPlmnIds; plmnIndex++)
         {
            cellPlmnId = &cellCb->sib1.plmns[plmnIndex].plmnId;
            if(wrEmmPlmnPlmnsEqual(&rcvdPlmnId, cellPlmnId)) 
            {
               RETVALUE(TRUE);
            }
         } /* end of for loop for serving cell PLMN Ids comparison */
      } /* end of if for TAC comparison */
   } /* end of for to traverse the received TAI List */
   RETVALUE(FALSE);
} /* end of wrPwsIsServCellTAIPresentInLst */ 

PRIVATE Bool wrPwsIsServCellEAIDPresentInLst 
(
 WrCellCb                    *cellCb,
 SztEmergencyAreaIDLst       *rcvdEAIDLst,
 Bool                        isStop
 )
{
   SztEmergencyAreaID        *rcvdEaid = NULLP;
   U32                       eaidIdx;
   U32                       rcvdEaidIdx;
   U32                       eaidShifted; 
   U8                        cellEaid[4];

   /* compare the configured Emenrgency Area ID list in Cell control block */
   for (eaidIdx = 0; (eaidIdx < cellCb->numOfCfgActiveEaids); eaidIdx++)
   {
      eaidShifted = cellCb->eaidLst[eaidIdx].eaid << 8;
      cellEaid[0] = (eaidShifted & 0xff000000) >> 24; 
      cellEaid[1] = (eaidShifted & 0x00ff0000) >> 16; 
      cellEaid[2] = (eaidShifted & 0x0000ff00) >> 8; 

      /* Traverse iand compare the received EAID with configured EAID */
      for (rcvdEaidIdx = 0; rcvdEaidIdx < rcvdEAIDLst->noComp.val; rcvdEaidIdx++)
      {
         rcvdEaid = &rcvdEAIDLst->member[rcvdEaidIdx];
         if ((cellEaid[0] == rcvdEaid->val[0]) &&
               (cellEaid[1] == rcvdEaid->val[1]) &&
               (cellEaid[2] == rcvdEaid->val[2]))
         {
            if(isStop)
            {
               cellCb->eaidLst[eaidIdx].isKillInit = TRUE;
            }
            else
            {
               cellCb->eaidLst[eaidIdx].isWarningMsgBroadcastInit = TRUE;
            }
            RETVALUE(TRUE);
         } /* end of if to compare EAIDs */
      } /* end of for to traverse all the recevied EAIDs */
   } /* end of for to traverse all the configured EAIDs */  
   RETVALUE(FALSE);
} /* end of wrPwsIsServCellEAIDPresentInLst */ 

/** @brief This function is used to send the RRC:paging message with
 *         etws-indicator set TRUE.
 *
 * @details
 *
 *     Function: wrPwsSndPagingwithEtwsInd
 *
 *         Processing steps:
 *         - Prepare and send the RRC:Paging message with ETWS-indcater
 *           in the specified cell, call the PBM Module function 
 *           "wrPbmStartPagingMsg"  @sa wrPbmStartPagingMsg. 
 *
 * @param[in] cellCb : cell control block.
 * @return S16
 *     -# Success : ROK
 *     -# Failure : RFAILED 
 */
PRIVATE S16 wrPwsSndPagingwithEtwsInd(WrCellCb *cellCb)
{

   if ( RFAILED == wrPbmStartPagingMsg(cellCb, WR_PAGE_ETWS_IND))
   {
      RLOG0(L_ERROR,"RRC:Paging msg with etws indication aleady"
         "transmiting.Hence not required to enable again");
      RETVALUE(RFAILED);
   }
   RETVALUE(ROK);
} /* end of wrPwsSndPagingwithEtwsInd */

/** @brief This function is used to send the RRC:paging message with
 *         etws-indicator set TRUE.
 *
 * @details
 *
 *     Function: wrPwsSndPagingwithCmasInd
 *
 *         Processing steps:
 *         - Prepare and send the RRC:Paging message with CMAS-indcater
 *           in the specified cell, call the PBM Module function 
 *           "wrPbmStartPagingMsg"  @sa wrPbmStartPagingMsg. 
 *
 * @param[in] cellCb : cell control block.
 * @return S16
 *     -# Success : ROK
 *     -# Failure : RFAILED 
 */
PRIVATE S16 wrPwsSndPagingwithCmasInd(WrCellCb *cellCb)
{
   if ( RFAILED == wrPbmStartPagingMsg(cellCb, WR_PAGE_CMAS_IND))
   {
      RLOG0(L_ERROR,"RRC:Paging msg with cmas indication aleady"
         "transmiting.Hence not required to enable again");
      RETVALUE(RFAILED);
   }
   RETVALUE(ROK);
} /* end of wrPwsSndPagingwithCmasInd */

/** @brief This function is used to stop the RRC:paging message with
 *         cmas-indicator set TRUE.
 *
 * @details This function is used to stop the RRC:paging message with
 *          cmas-indicator set TRUE.
 *
 *     Function: wrPwsStopPagingwithCmasInd
 *
 *         Processing steps:
 *         - Call PBM module's wrPbmStopPagingMsg.
 *
 * @param[in] cellCb : cell control block.
 * @return S16
 *     -# Success : ROK
 *     -# Failure : RFAILED 
 */
PRIVATE S16 wrPwsStopPagingwithCmasInd(WrCellCb *cellCb)
{

   if ( RFAILED == wrPbmStopPagingMsg(cellCb, WR_PAGE_CMAS_IND))
   {
      RLOG0(L_ERROR,"Unable to stop the RRC:Paging msg with cmas indication.");
      RETVALUE(RFAILED);
   }
   RETVALUE(ROK);
} /* end of wrPwsStopPagingwithCmasInd */

/** @brief This function is used to fill the write replacement warning response message.
 * @details This function prepares the warning response message with broadcast completed area list.
 * @param[in]       cellCbLst : List of Cell control Blocks belongs to EnodeB
 * @param[out] warningRespPdu : S1AP:Warning Response PDU buffer.
 * @param[in, out] warningRsp : warning Info contains broadcasted initation success marked list.
 * @param[in] broadCastCompletedIePres : TRUE: Include broadcat completed area list, Otherwise not.
 * @return S16
 *   -# Success : ROK
 *   -# Failure : RFAILED
 */
PRIVATE S16 wrPwsSztFillWarningResponse
(
 WrCellCb   **cellCbLst,
 S1apPdu    **pdu,
 WrPwsWarningInfo *warnRsp,
 Bool broadCastCompletedIePres,
 Bool  validCell[]
 )
{
   SztProtIE_Field_WriteReplaceWarningRespIEs *ie     = NULLP;
   SztTAI_Broadcast_Item                      *tai = NULLP;
   SztCellID_Broadcast_Item                   *cellId = NULLP;
   WrCellCb                                   *cellCb = NULLP;
   U8                                         taiIdx, cellIdx, numComp = 0;
   U8                                         len = 0;    /* KW fix for LTE_ADV */ 
   U16                                        msgId=0, serialNum=0;
   U8                                         protoMember = 0, eaType = 0;
   S1apPdu                                    *warnRspPdu; 
   WrPwsCmasInfo                              *cmasInfoPtr = NULLP;
   U32                                        id;
   SztEmergencyAreaID_Broadcast_Item          *eaId = NULLP;
   SztCompletedCellinEAI_Item                 *ieId = NULLP;
   SztCompletedCellinTAI_Item                 *ieIdTai = NULLP;
   U32                       eaidShifted; /*  ccpu00128226   */ 
   U8                        cellEaid[4];  /*  ccpu00128226   */

   /* Initialize memory control point */
   if(cmAllocEvnt(sizeof(S1apPdu), WR_SZ_MEM_SDU_SIZE, &wrCb.mem,
            (Ptr *)&warnRspPdu) != ROK)
   {
      RLOG0(L_FATAL, "Memory allocation failed.");
      RETVALUE(RFAILED);
   }
   *pdu = warnRspPdu;

   if (!warnRspPdu || !warnRsp || !cellCbLst)
   {
      RLOG0(L_ERROR,"Failed to fill the warning response.");
      if(warnRspPdu)
      {
         WR_FREE_EVNT(warnRspPdu);
      }
      RETVALUE(RFAILED);
   }

   wrFillTknU8(&(warnRspPdu->pdu.choice), S1AP_PDU_SUCCESSFULOUTCOME);
   wrFillTknU8(&(warnRspPdu->pdu.val.successfulOutcome.pres), PRSNT_NODEF);
   wrFillTknU32(&(warnRspPdu->pdu.val.successfulOutcome.procedureCode),
         Sztid_WriteReplaceWarning);
   wrFillTknU32(&(warnRspPdu->pdu.val.successfulOutcome.criticality),
         SztCriticalityrejectEnum);

   wrFillTknU8(&(warnRspPdu->pdu.val.successfulOutcome.value.u.\
            sztWriteReplaceWarningResp.pres), PRSNT_NODEF);

   /* Always adding 2 Mandatory + 1 Optional Paramters */
   if (broadCastCompletedIePres) 
   { 
      numComp = WR_PWS_WRNGRESP_WITH_BRDCSTCOMPIE;
   }
   else
   {
      numComp = WR_PWS_WRNGRESP_WO_BRDCSTCOMPIE;
   }

   wrFillTknU16(&(warnRspPdu->pdu.val.successfulOutcome.value.u.\
            sztWriteReplaceWarningResp.protocolIEs.noComp), numComp);
   if ((cmGetMem(warnRspPdu,\
               numComp * sizeof(SztProtIE_Field_WriteReplaceWarningRespIEs),\
               (Ptr *)&warnRspPdu->pdu.val.successfulOutcome.value.u.\
               sztWriteReplaceWarningResp.protocolIEs.member)) != ROK)
   {
      WR_FREE_EVNT(warnRspPdu);
      RLOG0(L_FATAL, "Memory allocation failed.");
      RETVALUE(RFAILED);
   }

   /* IE1 - Filling Warning mesage Type */
   ie = &warnRspPdu->pdu.val.successfulOutcome.value.u.\
        sztWriteReplaceWarningResp.protocolIEs.member[protoMember++];

   /* Warning Message ID */
   wrFillTknU8(&(ie->pres), PRSNT_NODEF);
   wrFillTknU32(&(ie->id), Sztid_MsgIdentifier);
   wrFillTknU32(&(ie->criticality),SztCriticalityrejectEnum);

   len = 16;
   if(warnRsp->rcvdWrngType == WR_PWS_CMAS_NOTIFICATION)
   {
      /* Get the CMAS node. */
      if(NULLP != (cmasInfoPtr = wrPwsUtlGetRcvdCmasInfo(warnRsp)))
      {
         msgId = cmasInfoPtr->wrngMsgId;
         serialNum = cmasInfoPtr->wrngSerialNum;
         eaType = cmasInfoPtr->wrngMsgInfo.emergencyAreaType; 
      }
      else
      {
           RLOG0(L_ERROR,"CMAS information not available.");
         WR_FREE_EVNT(warnRspPdu);
         RETVALUE(RFAILED);
      }
   }
   else if(warnRsp->rcvdWrngType == WR_PWS_PRI_NOTIFICATION || 
         warnRsp->rcvdWrngType == WR_PWS_BOTH_NOTIFICATIONS)
   {
      msgId = warnRsp->etwsInfo.wrngMsgId;
      serialNum = warnRsp->etwsInfo.wrngSerialNum;
      eaType = warnRsp->etwsInfo.primEtwsInfo.emergencyAreaType;
   }
   else if(warnRsp->rcvdWrngType == WR_PWS_SEC_NOTIFICATION)
   {
      msgId = warnRsp->etwsInfo.wrngMsgId;
      serialNum = warnRsp->etwsInfo.wrngSerialNum;
      eaType = warnRsp->etwsInfo.secEtwsInfo.emergencyAreaType;
   }
   else
   {
      /* Filling of msgId and serialNum in case of Disabled PWS */
      msgId = warnRsp->etwsInfo.wrngMsgId;
      serialNum = warnRsp->etwsInfo.wrngSerialNum;
   }

   wrFillTknBStr32(&(ie->value.u.sztMsgIdentifier), len,\
         msgId);

   /* IE2 - Filling Serial number */
   ie = &warnRspPdu->pdu.val.successfulOutcome.value.u.\
        sztWriteReplaceWarningResp.protocolIEs.member[protoMember++];

   /* Serial number */
   wrFillTknU8(&(ie->pres), PRSNT_NODEF);
   wrFillTknU32(&(ie->id), Sztid_SerialNumber);
   wrFillTknU32(&(ie->criticality),SztCriticalityrejectEnum);

   len = 16;
   wrFillTknBStr32(&(ie->value.u.sztSerialNumber), len,\
         serialNum);


   if (broadCastCompletedIePres) 
   {
      /* IE3 - Filling Broad Complete Area List */
      ie = &warnRspPdu->pdu.val.successfulOutcome.value.u.\
           sztWriteReplaceWarningResp.protocolIEs.member[protoMember++];

      /* Broad Completed Area List IE*/
      wrFillTknU8(&(ie->pres), PRSNT_NODEF);
      wrFillTknU32(&(ie->id), Sztid_BroadcastCompletedAreaLst);
      wrFillTknU32(&(ie->criticality), SztCriticalityignoreEnum);
      switch (eaType)
      {
         case WR_PWS_EATYPE_TAI:
            {
               wrFillTknU8(&(ie->value.u.sztBroadcastCompletedAreaLst.choice),\
                     1);    
               numComp = 1; /* small cell contain 1 cell, 
                               each cell is have 1 TAI only */
               wrFillTknU16(&(ie->value.u.sztBroadcastCompletedAreaLst.\
                        val.tAI_Broadcast.noComp), numComp);
               if ((cmGetMem(warnRspPdu,\
                           numComp * sizeof(SztTAI_Broadcast_Item),\
                           (Ptr *)&ie->value.u.sztBroadcastCompletedAreaLst.\
                           val.tAI_Broadcast.member)) != ROK)
               { 
                  RLOG0(L_FATAL,"Memory allocation failed.");
                  WR_FREE_EVNT(warnRspPdu);
                  RETVALUE(RFAILED);
               }

               for (cellIdx = 0, taiIdx = 0; cellIdx < wrEmmCb.numCells; cellIdx++)
               {
                  if(validCell[cellIdx] == TRUE)
                  {
                     cellCb = cellCbLst[cellIdx];
                     tai = &ie->value.u.sztBroadcastCompletedAreaLst.val.tAI_Broadcast.member[taiIdx++];
                     wrFillTknU8(&(tai->pres), PRSNT_NODEF);
                     wrFillTknU8(&(tai->tAI.pres), PRSNT_NODEF);

                     wrEmmMmeSztFillPLMNId(warnRspPdu,
                           &(cellCb->sib1.plmns[0].plmnId),
                           &(tai->tAI.pLMNidentity));

                     /* Tracking Area Code */
                     wrEmmMmeSztFillTAC((cellCb->sib1.tac), *pdu, &(tai->tAI.tAC));

                     /* Fill completedCellinTAI IE */
                     numComp = 1;
                     wrFillTknU16(&(tai->completedCellinTAI.noComp), numComp);
                     /* Allocate memory for broadcast PLMNs */
                     if((cmGetMem(warnRspPdu, numComp * sizeof(SztCompletedCellinTAI_Item),
                                 (Ptr*)&(tai->completedCellinTAI.member))) !=ROK)
                     {
                        RLOG0(L_FATAL,"Memory allocation failed."); 
                        WR_FREE_EVNT(warnRspPdu);
                        RETVALUE(RFAILED);
                     }


                     ieIdTai = &tai->completedCellinTAI.member[0];
                     wrFillTknU8(&(ieIdTai->pres), PRSNT_NODEF);
                     wrFillTknU8(&(ieIdTai->eCGI.pres), PRSNT_NODEF);

                     wrEmmMmeSztFillPLMNId(warnRspPdu,
                           &(cellCb->sib1.plmns[0].plmnId),
                           &(ieIdTai->eCGI.pLMNidentity));

                     /* ccpu00136516  */
                     len = 28;
                     wrFillTknBStr32(&(ieIdTai->eCGI.cell_ID), len,
                                             cellCb->sib1.cellIdentity);
                  }
                  else
                  {
                     RLOG0(L_ERROR,"valid cell not found to fill the TAI.");
                  }
               } /* end of for to traverse the cell List */
               break;
            }
         case WR_PWS_EATYPE_EAI:
            {
               wrFillTknU8(&(ie->value.u.sztBroadcastCompletedAreaLst.choice),\
                     2);    

               numComp = 1; /* small cell contain 1 cell, 
                               each cell is have 1 CellID */
               wrFillTknU16(&(ie->value.u.sztBroadcastCompletedAreaLst.\
                        val.emergencyAreaID_Broadcast.noComp), numComp);

               if ((cmGetMem(warnRspPdu,\
                           numComp * sizeof(SztEmergencyAreaID_Broadcast_Item),\
                           (Ptr *)&ie->value.u.sztBroadcastCompletedAreaLst.\
                           val.emergencyAreaID_Broadcast.member)) != ROK)
               {  
                  RLOG0(L_FATAL,"Memory allocation failed.");
                  WR_FREE_EVNT(warnRspPdu);
                  RETVALUE(RFAILED);
               }


               for (cellIdx = 0; cellIdx < wrEmmCb.numCells; cellIdx++)
               {
                  if(validCell[cellIdx] == TRUE)
                  {
                     cellCb = cellCbLst[cellIdx];
                     eaId = &ie->value.u.sztBroadcastCompletedAreaLst.val.emergencyAreaID_Broadcast.member[0];
                     wrFillTknU8(&(eaId->pres), PRSNT_NODEF);

                     numComp = 0;
                     for(id = 0; id < cellCb->numOfCfgActiveEaids; id++)
                     {
                        if(cellCb->eaidLst[id].isWarningMsgBroadcastInit)
                        {
                           numComp++;
                        }
                     }

                     /* Allocate memory for broadcast PLMNs */
                     if((cmGetMem(warnRspPdu, numComp * sizeof(SztEmergencyAreaID),
                                 (Ptr*)&(eaId->emergencyAreaID))) !=ROK)
                     {
                        RLOG0(L_FATAL,"Memory allocation failed.");
                        WR_FREE_EVNT(warnRspPdu);
                        RETVALUE(RFAILED);
                     }
                     len = 3;
                     for(id = 0; id < cellCb->numOfCfgActiveEaids; id++)
                     {
                        if(cellCb->eaidLst[id].isWarningMsgBroadcastInit)
                        {
                           /*  ccpu00128226  start  */   
                           eaidShifted = cellCb->eaidLst[id].eaid << 8;
                           cellEaid[0] = (eaidShifted & 0xff000000) >> 24; 
                           cellEaid[1] = (eaidShifted & 0x00ff0000) >> 16; 
                           cellEaid[2] = (eaidShifted & 0x0000ff00) >> 8;
                           wrFillTknStrOSXL(&(eaId->emergencyAreaID), len,
                                 (Data*)&(cellEaid), &(warnRspPdu->memCp));
                           /*  ccpu00128226  end  */
                        }
                     }

                     numComp = 1;
                     wrFillTknU16(&(eaId->completedCellinEAI.noComp), numComp);
                     /* Allocate memory for broadcast PLMNs */
                     if((cmGetMem(warnRspPdu, numComp * sizeof(SztCompletedCellinEAI_Item),
                                 (Ptr*)&(eaId->completedCellinEAI.member))) !=ROK)
                     {
                        RLOG0(L_FATAL,"Memory allocation failed.");
                        WR_FREE_EVNT(warnRspPdu);
                        RETVALUE(RFAILED);
                     }

                     ieId = &eaId->completedCellinEAI.member[0];
                     wrFillTknU8(&(ieId->pres), PRSNT_NODEF);
                     wrFillTknU8(&(ieId->eCGI.pres), PRSNT_NODEF);

                     wrEmmMmeSztFillPLMNId(warnRspPdu,
                           &(cellCb->sib1.plmns[0].plmnId),
                           &(ieId->eCGI.pLMNidentity));

                     /* ccpu00136516  */
                     len = 28;
                     wrFillTknBStr32(&(ieId->eCGI.cell_ID), len, 
                                           cellCb->sib1.cellIdentity);
                  }
                  else
                  {
                     RLOG0(L_ERROR,"valid cell not found to fill the EAID.");
                  }
               }
               break;
            }
         case WR_PWS_EATYPE_NONE:
         case WR_PWS_EATYPE_CELLID:
            {
               wrFillTknU8(&(ie->value.u.sztBroadcastCompletedAreaLst.choice),0);

               numComp = 1; /* small cell contain 1 cell, 
                               each cell is have 1 CellID */
               wrFillTknU16(&(ie->value.u.sztBroadcastCompletedAreaLst.\
                        val.cellID_Broadcast.noComp), numComp);

               if ((cmGetMem(warnRspPdu,\
                           numComp * sizeof(SztCellID_Broadcast_Item),\
                           (Ptr *)&ie->value.u.sztBroadcastCompletedAreaLst.\
                           val.cellID_Broadcast.member)) != ROK)
               {
                  RLOG0(L_FATAL,"Memory allocation failed.");
                  WR_FREE_EVNT(warnRspPdu);
                  RETVALUE(RFAILED);
               }

               for (cellIdx = 0; cellIdx < wrEmmCb.numCells; cellIdx++)
               {
                  cellCb = cellCbLst[cellIdx];
                  if(validCell[cellIdx] == TRUE)
                  {
                     cellId = &ie->value.u.sztBroadcastCompletedAreaLst.val.cellID_Broadcast.member[0];
                     wrFillTknU8(&(cellId->pres), PRSNT_NODEF);
                     wrFillTknU8(&(cellId->eCGI.pres), PRSNT_NODEF);

                     wrEmmMmeSztFillPLMNId(warnRspPdu,
                           &(cellCb->sib1.plmns[0].plmnId),
                           &(cellId->eCGI.pLMNidentity));

                     /* ccpu00136516  */
                     len = 28;
                     wrFillTknBStr32(&(cellId->eCGI.cell_ID), len, 
                                              cellCb->sib1.cellIdentity);
                  }
                  else
                  {
                     RLOG0(L_ERROR,"valid cell not found to fill the  Cell ID.");
                  }
               }
               break;     
            }
         default:
            {
               /* TODO:ALL cellIDs or TAIs or EAIDs of all supported cells by EnodeB
                * include */
               RLOG0(L_ERROR,"unknown type of broadcast area choice.");
               WR_FREE_EVNT(warnRspPdu);
               RETVALUE(RFAILED);   
            }
      }
   }
   RETVALUE(ROK);   
} /* end of wrPwsSztFillWarningResponse */

/** @brief This function is used to prepare the warning response message and
 * triggers the S1-AP IM module to send the warning reponse message to the MME .
 * @details This function is performed the following activiteis.
 * - Allocate the S1-AP PDU memory.
 * - Prepare the S1AP: Write-replacement warning Response message by using the 
 *   "wrPwsSztFillWarningResponse" internal function. @sa wrPwsSztFillWarningResponse.
 * - Call the S1-AP IM module function to send the warning response message to MME.
 * @param[in, out] emmCb: EMM Control block contains List of Cell control Blocks supporting in EnodeB
 * @param[in] mmeCb: MME Control block from which MME ENodeB received the warning Req 
 * @param[out] warningRsp: warning Info contains either broadcast is successfully initated 
 * or not in the spcified warning area list which is received in warning req.
 * @return S16 
 *     -# Success : ROK
 *     -# Failure : RFAILED 
 */
PRIVATE S16 wrPwsFillAndSndWrngResp
(
 WrCellCb                     *cellCb,
 WrPwsWarningInfo             *warningInfo,
 WrMmeCb                      *mmeCb,
 Bool                         broadCastCompletedIePres,
 Bool                         validCell[]
 )
{
   S16                       ret;
   SztUDatEvnt               uDatEvnt;

   RLOG0(L_EVENT,"[S1AP]:Sending WRITE-REPLACE WARNING RESPONSE");

   ret = wrPwsSztFillWarningResponse(&cellCb, 
         &(uDatEvnt.pdu), 
         warningInfo,  
         broadCastCompletedIePres,
         validCell);
   if (ret != ROK)
   {
      RETVALUE(RFAILED);
   }

   uDatEvnt.transId.pres = PRSNT_NODEF;
   uDatEvnt.transId.val  = 1;
   uDatEvnt.peerId.pres  = PRSNT_NODEF;
   uDatEvnt.peerId.val   = mmeCb->peerId;

   /* Send the pdu to the MME */
   if((WrIfmS1apSndMgmtMsg(&uDatEvnt)) != ROK)
   {
      RETVALUE(RFAILED);
   }

   RETVALUE(ROK); 
}/*wrPwsFillAndSndWrngResp*/

PUBLIC S16 wrEmmPwsProcKillReq(U32 peerId, S1apPdu *killReqPdu)
{
   Bool                      validCell[wrEmmCb.numCells], isMsgValid[wrEmmCb.numCells];
   Bool                      broadCastCancelledIePres = TRUE;
   SztWarningAreaLst         *warnAreaLst = NULLP; 
   WrMmeCb                   *mmeCb;
   U16                       cellIdx = 0;
   S16                       ret;
   WrCellCb                  *cellCb = NULLP;
   WrPwsCmasInfo              *cmasNode = NULLP, *crntCmas = NULLP;
   U8                         emergencyAreaType = WR_PWS_EATYPE_NONE;
   U16                        numCompKillReq = 0;
   U16                        memberIdx=0;
   U32                        msgId=0, serialNum=0;
   SztKillRqst                *killRqst = NULLP;
   SztProtIE_Field_KillRqstIEs *killReqMember;
   WrEmmCb                    *emmCb;
   U16                        numberOfBroadcasts = 0;
   TRC2(wrEmmPwsProcKillReq);

   RLOG0(L_DEBUG,"Received Kill message from MME. ");

   RLOG0(L_EVENT,"[S1AP]:Received KILL REQUEST");
   for (cellIdx = 0; cellIdx <  wrEmmCb.numCells; cellIdx++)
   {
      validCell[cellIdx] = FALSE;
      isMsgValid[cellIdx] = TRUE;
   }
   emmCb = &wrEmmCb;

   mmeCb = wrEmmMmeGetMmeByPeerId(peerId);
   if (mmeCb == NULLP)
   {
      /* Dont respond with any here as the state is invalid to transmit   */
      /* any response at all. May be an error indication may be sent      */
      RETVALUE(RFAILED);
   }

   /* Parse the kill request. */

   /* Get the reference of the kill Message */
   killRqst = &(killReqPdu->pdu.val.initiatingMsg.value.u.sztKillRqst);
   numCompKillReq = killRqst->protocolIEs.noComp.val;

   /* Accessing the IEs within the S1AP pdu */
   for(memberIdx = 0; memberIdx < numCompKillReq; memberIdx++)
   {
      killReqMember = &(killRqst->protocolIEs.member[memberIdx]);

      switch(killReqMember->id.val)
      {
         case Sztid_MsgIdentifier:
            {
               wrUtlGetU32FrmBStr32(&msgId,
                     &(killReqMember->value.u.sztMsgIdentifier));
               break;
            }
         case Sztid_SerialNumber: 
            {
               wrUtlGetU32FrmBStr32(&serialNum,
                     &(killReqMember->value.u.sztSerialNumber));
               break;
            }
         case Sztid_WarningAreaLst:
            {
               warnAreaLst = &killReqMember->value.u.sztWarningAreaLst;
               emergencyAreaType = killReqMember->value.u.sztWarningAreaLst.choice.val;
               break;
            }
         default:
            {
               RLOG1(L_ERROR,"Unknown IE or Option received:[%ld]", 
                  killReqMember->id.val);
               break;
            }
      } /* end of switch */
   } /* end of for to traverse all received IEs */


   for (cellIdx = 0; cellIdx <  wrEmmCb.numCells; cellIdx++)
   { 
      cellCb =wrEmmCb.cellCb[cellIdx]; 

      if(NULLP != cellCb->warningInfo)
      {
         /* Get the CMAS node. */
         cmasNode = wrPwsUtlGetDupCmas(cellCb, msgId, serialNum); 
         if (cmasNode == NULLP)
         {
            RLOG0(L_ERROR,"wrPwsUtlGetDupCmas::"
               "Failed to get the duplicate node.");
            broadCastCancelledIePres = FALSE;
            isMsgValid[cellIdx]= FALSE;
         }
         else
         {
             numberOfBroadcasts = cmasNode->numberOfBroadcasts;
         } 

      }
      else
      {
         RLOG0(L_DEBUG,"No warning Info for CELL ID.");
      }

      /* Valiate the Kill request based on message ID and serial number and area list received. */
      if(warnAreaLst != NULLP)
      {
         ret = wrPwsSelectBroadcastAreaToStop(warnAreaLst, msgId, serialNum, cellCb);

         if(ret == ROK)
         {
            /* If flow reaches here then it means that broadcast area is matching but
               no warning message is getting broadcsted in that area with the same
               msgID and serial number. */
            validCell[cellIdx] = TRUE;
         }
         else if(ret == RFAILED)
         {
            broadCastCancelledIePres = FALSE;
            isMsgValid[cellIdx] = FALSE;
         }
         else if(ret == WR_PWS_NO_MATCH_FOUND)
         {
            /* No match found for warning area type.*/
            broadCastCancelledIePres = FALSE;
            isMsgValid[cellIdx] = FALSE;
         }
      }
      else
      {
         validCell[cellIdx] = TRUE;
      }
   }
   /* Send the Kill response. */
   wrPwsFillAndSndKillResp(*(emmCb->cellCb), msgId, serialNum,numberOfBroadcasts,
         emergencyAreaType, mmeCb, broadCastCancelledIePres, validCell);

   for(cellIdx = 0; cellIdx <  wrEmmCb.numCells; cellIdx++)
   { 
      if(validCell[cellIdx] == TRUE && isMsgValid[cellIdx] == TRUE)
      {
         cellCb  = wrEmmCb.cellCb[cellIdx];

         if((NULLP != cellCb->warningInfo)&& (NULLP != cmasNode))/* Klockwork P1 warning fix */
         {
            /* Get the current ongoing warning message.*/
            crntCmas = cellCb->warningInfo->currentCmas;
            if(NULLP != crntCmas)
            {
               /* If this is the current CMAS being broadcasted then stop it. */
               if((crntCmas->wrngMsgId == cmasNode->wrngMsgId) &&
                     (crntCmas->wrngSerialNum == cmasNode->wrngSerialNum))
               {
                  /*wrEmmPbmWrngStopSi(cellCb->cellId, WR_PBM_SIB12_SI_INDEX);*/
                  crntCmas->waitToStop = FALSE;
                  crntCmas->wrngMsgInfo.numBroadcastReq = 0;
                  RLOG2(L_INFO,"wrEmmPwsProcKillReq:" "Current CMAS Msgid:%lu SerialNum:%lu",crntCmas->wrngMsgId, crntCmas->wrngSerialNum);  
                  RETVALUE(ROK);
                  /*ccpu00128753:Set this flag if kill req come for current(Ongoing) Cmas msg*/
            }
         }
            if(cmasNode->wrngMsgInfo.readyToSch == FALSE)
         {
         wrStopTmr((PTR)&(cmasNode->wrngMsgInfo.repetitionInfo), WR_TMR_REPETITION_PERIOD);
            } 
            RLOG2(L_INFO,"wrEmmPwsProcKillReq:" "CMAS present in List Msgid:%lu SerialNum:%lu",cmasNode->wrngMsgId,cmasNode->wrngSerialNum);  
            wrPwsFreePdu(&(cmasNode->wrngMsgInfo.segPduLst)); 
         cmLListDelFrm(&(cellCb->warningInfo->cmasLst), &(cmasNode->cmasLstLnk));
         WR_FREE(cmasNode, sizeof(WrPwsCmasInfo));
               }
               else
               {
              RLOG0(L_ERROR,"wrEmmPwsProcKillReq:"
                     "There is no Warning Info Present in the System.");
                  RETVALUE(RFAILED);
               } 

      }
   }

   RETVALUE(ROK);
} /* wrEmmPwsProcKillReq */





PRIVATE S16 wrPwsFillAndSndKillResp
(
 WrCellCb                     *cellCb,
 U32                          msgId,
 U32                          serialNum,
 U16                 numberOfBroadcasts,
 U8                           eaType,
 WrMmeCb                      *mmeCb,
 Bool                         broadCastCancelledIePres,
 Bool                         validCell[]
 )
{
   S16                       ret;
   SztUDatEvnt               uDatEvnt;

   ret = wrPwsSztFillKillResponse(&cellCb, 
         &(uDatEvnt.pdu), 
         msgId, serialNum,numberOfBroadcasts,eaType,
         broadCastCancelledIePres,
         validCell);
   if (ret != ROK)
   {
      RETVALUE(RFAILED);
   }

   uDatEvnt.transId.pres = PRSNT_NODEF;
   uDatEvnt.transId.val  = 1;
   uDatEvnt.peerId.pres  = PRSNT_NODEF;
   uDatEvnt.peerId.val   = mmeCb->peerId;

   /* Send the pdu to the MME */
   if((WrIfmS1apSndMgmtMsg(&uDatEvnt)) != ROK)
   {
      RETVALUE(RFAILED);
   }

   RETVALUE(ROK); 
}/*wrPwsFillAndSndKillResp*/




PRIVATE S16 wrPwsSztFillKillResponse
(
 WrCellCb **cellCbLst,
 S1apPdu    **pdu,
 U32      msgId,
 U32      serialNum,
 U16      numberOfBroadcasts,
 U8       emergencyAreaType,
 Bool     broadCastCancelledIePres ,
 Bool     validCell[]
 )
{
   SztProtIE_Field_KillRespIEs                *ie = NULLP;
   SztTAI_Cancelled_Item                      *tai = NULLP;
   SztEmergencyAreaID_Cancelled_Item          *eaId = NULLP;
   SztCellID_Cancelled_Item                   *cellId = NULLP;
   WrCellCb                                   *cellCb = NULLP;
   U8                                         taiIdx, cellIdx, numComp = 0;
   U8                                         len = 0;      /* KW fix for LTE_ADV */ 
   U8                                         protoMember = 0;
   S1apPdu                                    *killRspPdu;
   U32                                        id;
   SztCancelledCellinTAI_Item                 *ieIdTai = NULLP;
   SztCancelledCellinEAI_Item                 *ieId = NULLP;
   U32                                        eaidShifted;  
   U8                                         cellEaid[4]; 

   /* Initialize memory control point */
   if(cmAllocEvnt(sizeof(S1apPdu), WR_SZ_MEM_SDU_SIZE, &wrCb.mem,
            (Ptr *)&killRspPdu) != ROK)
   {
      RLOG0(L_FATAL, "Memory allocation failed.");
      RETVALUE(RFAILED);
   }
   *pdu = killRspPdu;


   if (!killRspPdu || !cellCbLst)
   {
      RLOG0(L_ERROR,"Failed to fill the kill response.");
      if(killRspPdu)
      {
         WR_FREE_EVNT(killRspPdu);
      }
      RETVALUE(RFAILED);
   }

   RLOG0(L_EVENT,"[S1AP]:Sending KILL RESPONSE");

   wrFillTknU8(&(killRspPdu->pdu.choice), SZT_TRGR_SUCCESS_OUTCOME);
   wrFillTknU8(&(killRspPdu->pdu.val.successfulOutcome.pres), PRSNT_NODEF);
   wrFillTknU32(&(killRspPdu->pdu.val.successfulOutcome.procedureCode),
         Sztid_Kill);
   wrFillTknU32(&(killRspPdu->pdu.val.successfulOutcome.criticality),
         SztCriticalityrejectEnum);

   wrFillTknU8(&(killRspPdu->pdu.val.successfulOutcome.value.u.\
            sztKillResp.pres), PRSNT_NODEF);


   /* Always adding 2 Mandatory + 1 Optional Paramters */
   if (broadCastCancelledIePres) 
   { 
      numComp = 3;
   }
   else
   {
      numComp = 2;
   }

   wrFillTknU16(&(killRspPdu->pdu.val.successfulOutcome.value.u.\
            sztKillResp.protocolIEs.noComp), numComp);
   if ((cmGetMem(killRspPdu,\
               numComp * sizeof(SztProtIE_Field_KillRespIEs),\
               (Ptr *)&killRspPdu->pdu.val.successfulOutcome.value.u.\
               sztKillResp.protocolIEs.member)) != ROK)
   {
      WR_FREE_EVNT(killRspPdu);
      RLOG0(L_FATAL, "Memory allocation failed.");
      RETVALUE(RFAILED);
   }

   /* IE1 - Filling Warning mesage Type */
   ie = &killRspPdu->pdu.val.successfulOutcome.value.u.\
        sztKillResp.protocolIEs.member[protoMember++];

   /* Warning Message ID */
   wrFillTknU8(&(ie->pres), PRSNT_NODEF);
   wrFillTknU32(&(ie->id), Sztid_MsgIdentifier);
   wrFillTknU32(&(ie->criticality),SztCriticalityrejectEnum);

   len = 16;
   wrFillTknBStr32(&(ie->value.u.sztMsgIdentifier), len,\
         msgId);

   /* IE2 - Filling Serial number */
   ie = &killRspPdu->pdu.val.successfulOutcome.value.u.\
        sztKillResp.protocolIEs.member[protoMember++];

   /* Serial number */
   wrFillTknU8(&(ie->pres), PRSNT_NODEF);
   wrFillTknU32(&(ie->id), Sztid_SerialNumber);
   wrFillTknU32(&(ie->criticality),SztCriticalityrejectEnum);

   len = 16;
   wrFillTknBStr32(&(ie->value.u.sztSerialNumber), len,\
         serialNum);

   /* IE3 - Filling broadcast cancelled area list IE */
   /* TODO : Filling of the boadcast cancelled area list ie needs to addressed properly. */
   if (broadCastCancelledIePres) 
   {
      /* IE3 - Filling Broad Complete Area List */
      ie = &killRspPdu->pdu.val.successfulOutcome.value.u.\
           sztKillResp.protocolIEs.member[protoMember++];

      /* Broad Completed Area List IE*/
      wrFillTknU8(&(ie->pres), PRSNT_NODEF);
      wrFillTknU32(&(ie->id), Sztid_BroadcastCancelledAreaLst);
      wrFillTknU32(&(ie->criticality), SztCriticalityignoreEnum);

      switch (emergencyAreaType)
      {
         case WR_PWS_EATYPE_TAI:
            {
               wrFillTknU8(&(ie->value.u.sztBroadcastCancelledAreaLst.choice),1);

               numComp = 1; /* small cell contain 1 cell, 
                               each cell is have 1 TAI only */
               wrFillTknU16(&(ie->value.u.sztBroadcastCancelledAreaLst.\
                        val.tAI_Cancelled.noComp), numComp);
               if ((cmGetMem(killRspPdu,\
                           numComp * sizeof(SztTAI_Cancelled_Item),\
                           (Ptr *)&ie->value.u.sztBroadcastCancelledAreaLst.\
                           val.tAI_Cancelled.member)) != ROK)
               {
                  RLOG0(L_FATAL,"Memory allocation failed.");
                  WR_FREE_EVNT(killRspPdu);
                  RETVALUE(RFAILED);
               }

               for (cellIdx = 0, taiIdx = 0; cellIdx < wrEmmCb.numCells; cellIdx++)
               {
                  if(validCell[cellIdx] == TRUE)
                  {
                     cellCb = cellCbLst[cellIdx];
                     tai = &ie->value.u.sztBroadcastCancelledAreaLst.val.tAI_Cancelled.member[taiIdx++];
                     wrFillTknU8(&(tai->pres), PRSNT_NODEF);
                     wrFillTknU8(&(tai->tAI.pres), PRSNT_NODEF);

                     wrEmmMmeSztFillPLMNId(killRspPdu,
                           &(cellCb->sib1.plmns[0].plmnId),
                           &(tai->tAI.pLMNidentity));

                     /* Tracking Area Code */
                     wrEmmMmeSztFillTAC((cellCb->sib1.tac), *pdu, &(tai->tAI.tAC));

                     /* Fill completedCellinTAI IE */
                     numComp = 1;
                     wrFillTknU16(&(tai->cancelledCellinTAI.noComp), numComp);
                     /* Allocate memory for broadcast PLMNs */
                     if((cmGetMem(killRspPdu, numComp * sizeof(SztCancelledCellinTAI_Item),
                                 (Ptr*)&(tai->cancelledCellinTAI.member))) !=ROK)
                     {
                        RLOG0(L_FATAL,"Memory allocation failed.");
                        WR_FREE_EVNT(killRspPdu);
                        RETVALUE(RFAILED);
                     }

                     ieIdTai = &tai->cancelledCellinTAI.member[0];
                     wrFillTknU8(&(ieIdTai->pres), PRSNT_NODEF);
                     wrFillTknU8(&(ieIdTai->eCGI.pres), PRSNT_NODEF);

                     wrEmmMmeSztFillPLMNId(killRspPdu,
                           &(cellCb->sib1.plmns[0].plmnId),
                           &(ieIdTai->eCGI.pLMNidentity));

                     /* ccpu00136516 */ 
                     len = 28;
                     wrFillTknBStr32(&(ieIdTai->eCGI.cell_ID), len, 
                                           cellCb->sib1.cellIdentity);
                     /*  ccpu00127868  */ 
                     wrFillTknU32(&(ieIdTai->numberOfBroadcasts), numberOfBroadcasts);
                     /*  ccpu00127868  */

                  }
               } /* end of for to traverse the cell List */

               break;
            }
         case WR_PWS_EATYPE_EAI:
            {
               wrFillTknU8(&(ie->value.u.sztBroadcastCancelledAreaLst.choice),2);

               numComp = 1; /* small cell contain 1 cell, 
                               each cell is have 1 CellID */
               wrFillTknU16(&(ie->value.u.sztBroadcastCancelledAreaLst.\
                        val.emergencyAreaID_Cancelled.noComp), numComp);

               if ((cmGetMem(killRspPdu,\
                           numComp * sizeof(SztEmergencyAreaID_Cancelled_Item),\
                           (Ptr *)&ie->value.u.sztBroadcastCancelledAreaLst.\
                           val.emergencyAreaID_Cancelled.member)) != ROK)
               {
                  RLOG0(L_FATAL,"Memory allocation failed.");
                  WR_FREE_EVNT(killRspPdu);
                  RETVALUE(RFAILED);
               }


               for ( cellIdx = 0; cellIdx < wrEmmCb.numCells; cellIdx++)
               {
                  if(validCell[cellIdx] == TRUE)
                  {
                     cellCb = cellCbLst[cellIdx];
                     eaId = &ie->value.u.sztBroadcastCancelledAreaLst.val.emergencyAreaID_Cancelled.member[0];
                     wrFillTknU8(&(eaId->pres), PRSNT_NODEF);

                     numComp = 0;
                     for(id = 0; id < cellCb->numOfCfgActiveEaids; id++)
                     {
                        if(cellCb->eaidLst[id].isKillInit)
                        {
                           numComp++;
                        }
                     }

                     /* Allocate memory for broadcast PLMNs */
                     if((cmGetMem(killRspPdu, numComp * sizeof(SztEmergencyAreaID),
                                 (Ptr*)&(eaId->emergencyAreaID))) !=ROK)
                     {
                        RLOG0(L_FATAL,"Memory allocation failed.");
                        WR_FREE_EVNT(killRspPdu);
                        RETVALUE(RFAILED);
                     }
                     len = 3;
                     for(id = 0; id < cellCb->numOfCfgActiveEaids; id++)
                     {
                        if(cellCb->eaidLst[id].isKillInit)
                        {
                           eaidShifted = cellCb->eaidLst[id].eaid << 8;
                           cellEaid[0] = (eaidShifted & 0xff000000) >> 24;
                           cellEaid[1] = (eaidShifted & 0x00ff0000) >> 16;
                           cellEaid[2] = (eaidShifted & 0x0000ff00) >> 8;

                           wrFillTknStrOSXL(&(eaId->emergencyAreaID), len,
                                 (Data*)&(cellEaid), &(killRspPdu->memCp));
                          /* wrFillTknStrOSXL(&(eaId->emergencyAreaID), len,
                               (Data*)&(cellCb->eaidLst[id].eaid), &(killRspPdu->memCp));*/
                        }
                     }

                     numComp = 1;
                     wrFillTknU16(&(eaId->cancelledCellinEAI.noComp), numComp);
                     /* Allocate memory for broadcast PLMNs */
                     if((cmGetMem(killRspPdu, numComp * sizeof(SztCancelledCellinEAI_Item),
                                 (Ptr*)&(eaId->cancelledCellinEAI.member))) !=ROK)
                     {
                        RLOG0(L_FATAL,"Memory allocation failed.");
                        WR_FREE_EVNT(killRspPdu);
                        RETVALUE(RFAILED);
                     }

                     ieId = &eaId->cancelledCellinEAI.member[0];
                     wrFillTknU8(&(ieId->pres), PRSNT_NODEF);
                     wrFillTknU8(&(ieId->eCGI.pres), PRSNT_NODEF);

                     wrEmmMmeSztFillPLMNId(killRspPdu,
                           &(cellCb->sib1.plmns[0].plmnId),
                           &(ieId->eCGI.pLMNidentity));

                     /* ccpu00136516 */ 
                     len = 28;
                     wrFillTknBStr32(&(ieId->eCGI.cell_ID), len,
                                         cellCb->sib1.cellIdentity);

                     /*  ccpu00127868  */ 
                     
                     wrFillTknU32(&(ieId->numberOfBroadcasts), numberOfBroadcasts);
                     /*  ccpu00127868  */

                  }
               }

               break;
            }
         case WR_PWS_EATYPE_NONE:
         case WR_PWS_EATYPE_CELLID:
            wrFillTknU8(&(ie->value.u.sztBroadcastCancelledAreaLst.choice),0);

            numComp = 1; /* small cell contain 1 cell, 
                            each cell is have 1 CellID */
            wrFillTknU16(&(ie->value.u.sztBroadcastCancelledAreaLst.\
                     val.cellID_Cancelled.noComp), numComp);

            if ((cmGetMem(killRspPdu,\
                        numComp * sizeof(SztCellID_Cancelled_Item),\
                        (Ptr *)&ie->value.u.sztBroadcastCancelledAreaLst.\
                        val.cellID_Cancelled.member)) != ROK)
            { 
               RLOG0(L_FATAL,"Memory allocation failed.");
               WR_FREE_EVNT(killRspPdu);
               RETVALUE(RFAILED);
            }

            for (cellIdx = 0; cellIdx < wrEmmCb.numCells; cellIdx++)
            {
               if(validCell[cellIdx] == TRUE)
               {
                  cellCb = cellCbLst[cellIdx];
                  cellId = &ie->value.u.sztBroadcastCancelledAreaLst.val.cellID_Cancelled.member[0];
                  wrFillTknU8(&(cellId->pres), PRSNT_NODEF);
                  wrFillTknU8(&(cellId->eCGI.pres), PRSNT_NODEF);

                  wrEmmMmeSztFillPLMNId(killRspPdu,
                        &(cellCb->sib1.plmns[0].plmnId),
                        &(cellId->eCGI.pLMNidentity));

                  /* ccpu00136516 */ 
                  len = 28;
                  wrFillTknBStr32(&(cellId->eCGI.cell_ID), len, 
                                          cellCb->sib1.cellIdentity);

                  /*  ccpu00127868  */ 
                  
                  wrFillTknU32(&(cellId->numberOfBroadcasts), numberOfBroadcasts);
                  /*  ccpu00127868  */
               }
            }

            break;     
         default:
            {
               /* TODO:ALL cellIDs or TAIs or EAIDs of all suported cells by EnodeB
                * include */
               RLOG0(L_ERROR,"unknown type of broadcast area choice.");
               WR_FREE_EVNT(killRspPdu);
               RETVALUE(RFAILED);   
            }
      }
   }
   RETVALUE(ROK);   
} /* end of wrPwsSztFillKillResponse */

PRIVATE S16 wrPwsSelectBroadcastAreaToStop
(
 SztWarningAreaLst           *warnAreaLst,
 U32                         msgId,
 U32                         serialNum ,
 WrCellCb                    *cellCb
 )
{
   S16         ret = RFAILED;
   U8          eaType = 0;

   eaType = warnAreaLst->choice.val;

   switch(eaType)
   {
      case WARNINGAREALST_CELLIDLST:
         {  
            if (TRUE == wrPwsIsServCellIdPresentInLst(cellCb,
                     &warnAreaLst->val.cellIDLst))
            {
               RLOG0(L_INFO,"Cell Id present in the list.");

               if(cellCb->warningInfo)
               {
                  /* validate and create pws information */
                  ret = wrPwsValidateKillNotif(msgId, serialNum, cellCb->warningInfo);
               }
            }
            else
            {
               RLOG0(L_ERROR," Cell Id is not present in the list.");
               ret = WR_PWS_NO_MATCH_FOUND;
            }

            break;
         }
      case WARNINGAREALST_TRACKINGAREALSTFORWARNING:
         {
            if (TRUE == wrPwsIsServCellTAIPresentInLst(cellCb,
                     &warnAreaLst->val.trackingAreaLstforWarning))
            {
               RLOG0(L_DEBUG,"Cell Tracking area Id present in the list.");

               if(cellCb->warningInfo)
               {
                  /* validate and create pws information */
                  ret = wrPwsValidateKillNotif(msgId, serialNum, cellCb->warningInfo);
               }
            }
            else
            {
               RLOG0(L_ERROR,"Cell Tracking area Id is not present in the list.");
               ret = WR_PWS_NO_MATCH_FOUND;
            }

            break;
         }
      case WARNINGAREALST_EMERGENCYAREAIDLST:
         {
            if (TRUE == wrPwsIsServCellEAIDPresentInLst(cellCb,
                     &warnAreaLst->val.emergencyAreaIDLst, TRUE))
            {
               RLOG0( L_DEBUG,"Cell Emergency area Id present in the list.");

               if(cellCb->warningInfo)
               {
                  /* validate and create pws information */
                  ret = wrPwsValidateKillNotif(msgId, serialNum, cellCb->warningInfo);
               }
            }
            else
            {
               RLOG0(L_ERROR,"Cell Emergency area Id is not present in the list.");
               ret = WR_PWS_NO_MATCH_FOUND;
            }

            break;
         }
      case WR_PWS_EATYPE_NONE:
         {
            RLOG0(L_WARNING,"Emergency area type is not mentioned.");

            if(cellCb->warningInfo)
            {
               /* validate and create pws information */
               ret = wrPwsValidateKillNotif(msgId, serialNum, cellCb->warningInfo);
            }
            break;
         }
      default:
         {
            RLOG0(L_ERROR,"Emergency area type is invalid.");
            RETVALUE(WR_PWS_NO_MATCH_FOUND); 
         }
   } /* end of switch */
   RETVALUE(ret);
} /* end of wrPwsSelectBroadcastAreaToKill */



PRIVATE S16 wrPwsValidateKillNotif
(
 U32                         msgId,
 U32                         serialNum,
 WrPwsWarningInfo            *warnInfo
 )
{
   CmLListCp                  *cmasLst = NULLP;
   CmLList                    *cmasLstLnk = NULLP;
   WrPwsCmasInfo              *cmasInfoPtr = NULLP;

   /* check for each cmas node. */
   cmasLst = &(warnInfo->cmasLst);

   CM_LLIST_FIRST_NODE(cmasLst, cmasLstLnk);

   while (NULLP != cmasLstLnk)
   {
      cmasInfoPtr = (WrPwsCmasInfo *)(cmasLstLnk->node);
      if(NULLP != cmasInfoPtr)
      {
         if ((msgId == cmasInfoPtr->wrngMsgId) &&
               (serialNum == cmasInfoPtr->wrngSerialNum))
         {
            RETVALUE(ROK);
         }
         cmasLstLnk = cmasLstLnk->next;
      }
   } /* end while */

   RETVALUE(RFAILED);
} /* end of wrPwsValidateKillNotif */

PUBLIC S16 wrPwsHndlSchdCfgCfm
(
 WrCellId cellId,
 U32      transId
 )
{
   U16                       siIndex;
   S16                       ret = ROK;
   WrCellCb                  *cellCb;

   /* Get the SI index from the transId. */
   siIndex = (U16)(transId & 0xffff);

   /* Get the celCb. */
   WR_GET_CELLCB(cellCb,cellId); 
   if (cellCb == NULLP)
   {
      RLOG0(L_ERROR,"Failed to get the cellCb.");
      RETVALUE(RFAILED);
   }

   /* If the confirm is received for the CMAS message then do following. */
   if( WR_PBM_SIB12_SI_INDEX == siIndex)
   {
      ret = wrPwsHndlCmasSchdCfgCfm(cellCb);
      if(RFAILED == ret)
      {
         RLOG0(L_ERROR,"wrPwsHndlCmasSchdCfgCfm:Unable to handle" 
            "Configuration confirm for CMAS");
         RETVALUE(RFAILED);
      }
   } /* if confirm is for CMAS. */
   else if( WR_PBM_SIB11_SI_INDEX == siIndex)
   {
      ret = wrPwsHndlSecEtwsSchdCfgCfm(cellCb);
      if(RFAILED == ret)
      {
         RLOG0(L_ERROR,"wrPwsHndlSecEtwsSchdCfgCfm:Unable to handle"
               "Configuration confirm for Secondary ETWS.");
         RETVALUE(RFAILED);
      }
   } /* if confirm is for sec ETWS. */
   else if( WR_PBM_SIB10_SI_INDEX == siIndex)
   {
      ret = wrPwsHndlPrimEtwsSchdCfgCfm(cellCb); 
      if(RFAILED == ret)
      {
         RLOG0(L_ERROR,"wrPwsHndlPrimEtwsSchdCfgCfm:Unable to handle"
            "Configuration confirm for Primary ETWS.");
         RETVALUE(RFAILED);
      }
   } /* if confirm is for prim ETWS. */

   RETVALUE(ret);
} /* wrPwsHndlSchdCfgCfm */


PRIVATE S16 wrPwsHndlSecEtwsSchdCfgCfm
(
 WrCellCb            *cellCb
 )
{
   S16                       ret = ROK;
   WrPwsEtwsInfo             *crntEtws = NULLP;
   U8                 retransSiIndex = 0;

   /* Get the current ongoing warning message.*/
   crntEtws = &(cellCb->warningInfo->etwsInfo);

   /* Update the isSecEtwsInProgress flag. */ 
   cellCb->warningInfo->isSecEtwsInProgress = FALSE;

   /* decrement the number of broadcast requested by 1. */
   crntEtws->secEtwsInfo.numBroadcastReq =
      crntEtws->secEtwsInfo.numBroadcastReq - 1;

   /*  As we have received the SCH cfm ,after broad cast of SIB11 
    *  we need to remove the SIB11 from the SIB1 */ 

   retransSiIndex = cellCb->warningInfo->sib11SiIdx;
   /* Configure SIB1 scheduling info. */
   if (WR_PBM_SEND_NONE == wrPbmUpdSiSchedInfoLstInSIB1(cellCb,
            WR_SIB_ACTION_REMOVE,
            WR_SIB_11,
            &retransSiIndex))
   {
      RLOG0(L_ERROR,"wrPbmUpdSiSchedInfoLstInSIB1 :"
         "SIB1 SI scheduling info update failed.");
      RETVALUE(RFAILED);
   }

   /* Configure MAC with updated SIB1 information. */
   if (wrEmmPbmSib1Config(cellCb->cellId,WR_SIB1_SEC_ETWS_TRANS_ID) != ROK)
   {
      RLOG0(L_ERROR,"wrEmmPbmSib1Config :"
         "SIB1 SI schedulin info configuration failed.");
      RETVALUE(RFAILED);
   }

   if(cellCb->warningInfo->etwsInfo.notificationType == WR_PWS_BOTH_NOTIFICATIONS)
   {

      retransSiIndex = cellCb->warningInfo->sib10SiIdx;
      /* Configure SIB1 scheduling info. */
      if (WR_PBM_SEND_NONE == wrPbmUpdSiSchedInfoLstInSIB1(cellCb,
               WR_SIB_ACTION_REMOVE,
               WR_SIB_10,
               &retransSiIndex))
      {
         RLOG0(L_ERROR,"wrPbmUpdSiSchedInfoLstInSIB1 :"
            "SIB1 SI scheduling info update failed.");
         RETVALUE(RFAILED);
      }

      /* Configure MAC with updated SIB1 information. */
      if (wrEmmPbmSib1Config(cellCb->cellId,WR_SIB1_PRI_ETWS_TRANS_ID) != ROK)
      {
         RLOG0(L_ERROR,"wrEmmPbmSib1Config :"
            "SIB1 SI schedulin info configuration failed.");
         RETVALUE(RFAILED);
      }
   } /* received warning type is both */


   /* If number of broadcast requested reaches to 0 then delete the CMAS node.*/
   if(crntEtws->secEtwsInfo.numBroadcastReq <= 0)
   {
      RLOG0(L_DEBUG,"Number of broadcast requested reached to 0 for secondary"
         "ETWS.");
      RLOG0(L_DEBUG,"Stop repetition period timer for Secondary ETWS warning" 
         "message.");
      wrStopTmr((PTR)&(crntEtws->secEtwsInfo.repetitionInfo), WR_TMR_REPETITION_PERIOD );
      cellCb->warningInfo->etwsInfo.secEtwsInfo.readyToSch = TRUE;
      cellCb->warningInfo->etwsInfo.wrngMsgId = WR_PWS_INVALID_MSGID;
      wrPwsFreePdu(&(cellCb->warningInfo->etwsInfo.secEtwsInfo.segPduLst));
   }
   else if(crntEtws->secEtwsInfo.readyToSch)
   {
      RLOG0(L_DEBUG,"Repetition period timer expired for the secondary ETWS."
         "Configure MAC to schedule it again.");

      ret = wrPwsStartSecondaryEtws(cellCb);
      if( ret != ROK )
      {
         RLOG0(L_ERROR,"Unable to start Secondary ETWS.");
      }
   }
   else
   {
      RLOG0(L_DEBUG,"Secondary ETWS is not ready to schedule."
         "Wait for repetiotion period timer to expire.");
      cellCb->warningInfo->isSecEtwsInProgress = FALSE;
   } 

   RETVALUE(ret); 
} /* wrPwsHndlSecEtwsSchdCfgCfm */   


PRIVATE S16 wrPwsHndlPrimEtwsSchdCfgCfm
(
 WrCellCb            *cellCb
 )
{
   S16                       ret = ROK;
   WrPwsEtwsInfo             *crntEtws = NULLP;
   U8                 retransSiIndex = 0;

   /* Get the current ongoing warning message.*/
   crntEtws = &(cellCb->warningInfo->etwsInfo);

   /* Update the isPrimEtwsInProgress flag. */ 
   cellCb->warningInfo->isPrimEtwsInProgress = FALSE;

   /* decrement the number of broadcast requested by 1. */
   crntEtws->primEtwsInfo.numBroadcastReq =
      crntEtws->primEtwsInfo.numBroadcastReq - 1;


   /*  As we have received the SCH cfm ,after broad cast of SIB10 
    *  we need to remove the SIB10 from the SIB1 */ 
   if(cellCb->warningInfo->etwsInfo.notificationType == WR_PWS_PRI_NOTIFICATION)
   {
      retransSiIndex = cellCb->warningInfo->sib10SiIdx;
      /* Configure SIB1 scheduling info. */
      if (WR_PBM_SEND_NONE == wrPbmUpdSiSchedInfoLstInSIB1(cellCb,
               WR_SIB_ACTION_REMOVE,
               WR_SIB_10,
               &retransSiIndex))
      {
         RLOG0(L_ERROR,"wrPbmUpdSiSchedInfoLstInSIB1 :"
            "SIB1 SI scheduling info update failed.");
         RETVALUE(RFAILED);
      }

      /* Configure MAC with updated SIB1 information. */
      if (wrEmmPbmSib1Config(cellCb->cellId,WR_SIB1_PRI_ETWS_TRANS_ID) != ROK)
      {
         RLOG0(L_ERROR,"wrEmmPbmSib1Config :"
            "SIB1 SI schedulin info configuration failed.");
         RETVALUE(RFAILED);
      }
   }

   /* If number of broadcast requested reaches to 0 then delete the Primary node.*/
   if(crntEtws->primEtwsInfo.numBroadcastReq <= 0)
   {
      RLOG0(L_DEBUG,"Number of broadcast requested reached to 0 for primary"
         "ETWS.");

      RLOG0(L_DEBUG,"Stop repetition period timer for Primary ETWS"
         "warning message.");
      wrStopTmr((PTR)&(crntEtws->primEtwsInfo.repetitionInfo), WR_TMR_REPETITION_PERIOD );
      cellCb->warningInfo->etwsInfo.primEtwsInfo.readyToSch = TRUE;
      cellCb->warningInfo->etwsInfo.wrngMsgId = WR_PWS_INVALID_MSGID;

   }
   else if(crntEtws->primEtwsInfo.readyToSch)
   {
      RLOG0(L_DEBUG,"Repetition period timer expired for the primary ETWS."
         "Configure MAC to schedule it again.");
      ret = wrPwsStartPrimaryEtws(cellCb);
      if( ret != ROK )
      {
         RLOG0(L_DEBUG,"wrPwsStartPrimaryEtws:Unable to start Primary ETWS.");
      }
   }
   else
   {
      RLOG0(L_DEBUG,"Primary ETWS is not ready to schedule."
         "Wait for repetition period tim to expire.");
   }

   RETVALUE(ret); 
} /* wrPwsHndlPrimEtwsSchdCfgCfm */



PRIVATE S16 wrPwsHndlCmasSchdCfgCfm
(
 WrCellCb            *cellCb
 )
{
   U8                        cnt = 0;
   S16                       ret = ROK;
   WrPwsCmasInfo             *crntCmas = NULLP;
   WrPwsCmasInfo             *cmasNode = NULLP;
   CmLListCp                 *cmasLst = NULLP;
   Bool                      isStopped = FALSE;
   U8                        retransSiIndex;
   /* Update the isCmasInProgress flag. */
   cellCb->warningInfo->isCmasInProgress = FALSE;

   /* Get the current ongoing warning message.*/
   crntCmas = cellCb->warningInfo->currentCmas;
   if (crntCmas == NULLP)
   {
      RLOG0(L_ERROR ,"crntCmas is NULL");
      RETVALUE(RFAILED);
   }
   ret = wrPwsStopCmas(cellCb);
   if( ret != ROK )
   {
         RLOG0(L_WARNING,"wrPwsHndlCmasSchdCfgCfm : Unable to stop ongoing CMAS. \n");
   }

   /* Get the CMAS list. */
   cmasLst  = &(cellCb->warningInfo->cmasLst);

   if(!crntCmas->waitToStop)
   {
      /* decrement the number of broadcast requested by 1. */
      crntCmas->wrngMsgInfo.numBroadcastReq =
         crntCmas->wrngMsgInfo.numBroadcastReq - 1;

      /* If number of broadcast requested reaches to 0 then delete the CMAS node.*/
      if(crntCmas->wrngMsgInfo.numBroadcastReq <= 0)
      {
         RLOG0(L_DEBUG,"Number of broadcast reached to 0 for CMAS.");

         RLOG0(L_DEBUG,"Stop repetition period timer for CMAS warning message.");
         wrStopTmr((PTR)&(crntCmas->wrngMsgInfo.repetitionInfo), WR_TMR_REPETITION_PERIOD );
         /* Get the CMAS node. */
         cmasNode = wrPwsGetNextNode(&(cellCb->warningInfo->cmasLst), crntCmas);

         /* Remove the cmas node from the cmas list and free it. */
         wrPwsFreePdu(&(crntCmas->wrngMsgInfo.segPduLst)); 
         cmLListDelFrm(cmasLst, &(crntCmas->cmasLstLnk));
         WR_FREE(crntCmas, sizeof(WrPwsCmasInfo));
         isStopped = TRUE;
      }
   }

   /* If the num of broadcasts requested reaches to 0 and this iss the only
      CMAS then delete the CMAS, update the sib1 scheduling info, and stop
      paging. */
   if(cellCb->warningInfo->cmasLst.count > 0)
   {
      /* If not the only CMAS then pick up the
         priority CMAS and configure MAC. */
      RLOG0(L_DEBUG,"Multiple CMAS in the system. Select the next CMAS to schedule.");

      if(!isStopped)
      {
         cmasNode = wrPwsGetNextNode(&(cellCb->warningInfo->cmasLst), crntCmas);
      }

      for(cnt=0; (cnt < cellCb->warningInfo->cmasLst.count); cnt++)
      {
         if(NULLP != cmasNode)
         {
            if(cmasNode->wrngMsgInfo.readyToSch == TRUE) /*Assuming Timer will not expire before confirm comes from MAC*/
            {
               cellCb->warningInfo->currentCmas = cmasNode;
               RLOG0(L_INFO,
                     "wrPwsHndlCmasSchdCfgCfm:"
                     "Configuring MAC to schedule the next CMAS. \n");
               /* Configure SIB1 scheduling info. */
               if (WR_PBM_SEND_NONE == wrPbmUpdSiSchedInfoLstInSIB1(cellCb,
                        WR_SIB_ACTION_ADD,
                        WR_SIB_12,
                        &retransSiIndex))
               {
                  RLOG0(L_ERROR,"CMAS node is NULL.");
                  RETVALUE(RFAILED);
               }

               cellCb->warningInfo->sib12SiIdx = retransSiIndex;

               /* Configure MAC with updated SIB1 information. */
               if (wrEmmPbmSib1Config(cellCb->cellId, WR_SIB1_CMAS_TRANS_ID) != ROK)
               {
                  RLOG0(L_ERROR ,"wrPwsGetNextNode:cmasNode is NULL.");
                  RETVALUE(RFAILED);
               }


               /* Create the SIs and configure the MAC for SI transmission. */
               RLOG0(L_INFO,"Configuring the next CMAS in List\n");

               ret =  wrPwsSndPagingwithCmasInd(cellCb);
               if(ret == RFAILED)
               {
                  RLOG0(L_DEBUG,"No CMAS message ready to schedule.");
               }

               if (RFAILED == wrPbmStartWarningMsg(cellCb,WR_START_CMAS_IND))
               {
                  RLOG0(L_ERROR,"wrEmmPwsCmasRepTimerExp: :"
                        "Unable to process the warning message. \n");
                  RETVALUE(RFAILED);
               }
               cellCb->warningInfo->isCmasInProgress = TRUE;

               break;
            }
            else
            {
               RLOG0(L_DEBUG,"wrPwsStartCmas: Unable to start CMAS.");
            }
         }
         else
         {
            RLOG0(L_ERROR,"wrPwsHndlCmasSchdCfgCfm:"
                  "CMAS node is NULL.");
            RETVALUE(RFAILED);
         }

         cmasNode = wrPwsGetNextNode(&(cellCb->warningInfo->cmasLst), cmasNode); 
      }

   }/* else last CMAS */
   RETVALUE(ret); 
} /* wrPwsHndlCmasSchdCfgCfm */


PUBLIC S16 wrPwsHndlSchdCfgFail
(
 WrCellId cellId,
 U32      transId
 )
{
   U16                       siIndex;
   WrCellCb                  *cellCb;
   WrPwsCmasInfo             *crntCmas = NULLP, *cmasNode = NULLP;
   CmLListCp                 *cmasLst = NULLP;
   CmLList                   *cmasLstLnk = NULLP;

   /* Get the SI index from the transId. */
   siIndex = (U16)(transId & 0xffff);
   /* Get the celCb. */
   WR_GET_CELLCB(cellCb,cellId); 
   if (cellCb == NULLP)
   {
      RLOG0(L_ERROR,"Failed to get the cellCb.");
      RETVALUE(RFAILED);
   }

   /* If the confirm is received for the CMAS message then do following. */
   if( WR_PBM_SIB12_SI_INDEX == siIndex)
   {
      /* Get the current ongoing warning message.*/
      crntCmas = cellCb->warningInfo->currentCmas;

      /* Get the CMAS list. */
      cmasLst  = &(cellCb->warningInfo->cmasLst);

      /* If not the only CMAS then pick up the
         priority CMAS and configure MAC. */

      if (NULLP != &(crntCmas->cmasLstLnk))
      {
         cmasNode = (WrPwsCmasInfo *)(crntCmas->cmasLstLnk.node);
         if(NULLP != cmasNode)
         {
            cellCb->warningInfo->currentCmas = cmasNode;
         }
      }
      else
      {
         CM_LLIST_FIRST_NODE(cmasLst, cmasLstLnk);
         if (NULLP != cmasLstLnk)
         {
            cellCb->warningInfo->currentCmas = (WrPwsCmasInfo*)(cmasLstLnk->node);
         }
      }
   } /* if confirm is for CMAS. */
   else if( WR_PBM_SIB11_SI_INDEX == siIndex)
   {
      /* TODO : Reschedule the etws. */
   } /* if confirm is for sec ETWS. */
   else if( WR_PBM_SIB10_SI_INDEX == siIndex)
   {
      /* TODO : Reschedule the etws. */
   } /* if confirm is for prim ETWS. */

   RETVALUE(ROK);

} /* wrPwsHndlSchdCfgFail */


/* This function is added for CMAS phase2.*/
PUBLIC S16 wrPwsHndlStopWrngSiCfgCfm
(
 WrCellId cellId,
 U32      transId
 )
{
   U8                        retransSiIndex = 0;
   WrCellCb                  *cellCb;
   WrPwsCmasInfo             *crntCmas = NULLP;
   WrPwsCmasInfo             *cmasNode = NULLP;
   CmLListCp                 *cmasLst = NULLP;
   CmLList                   *cmasLstLnk = NULLP;

   /* Get the SI index from the transId. */
   /*siIndex = transId & 0xffff;*/
   /* Get the celCb. */
   WR_GET_CELLCB(cellCb,cellId); 
   if (cellCb == NULLP)
   {
      RLOG0(L_ERROR,"Failed to get the cellCb.");
      RETVALUE(RFAILED);
   }

   /* If the confirm is received for the CMAS message then do following. */

   /* Get the current ongoing warning message.*/
   crntCmas = cellCb->warningInfo->currentCmas;

   /* Get the CMAS list. */
   cmasLst  = &(cellCb->warningInfo->cmasLst);

   /* Remove the cmas node from the cmas list and free it. */
   cmLListDelFrm(cmasLst, &(crntCmas->cmasLstLnk));
   WR_FREE(crntCmas, sizeof(WrPwsCmasInfo));

   if(cellCb->warningInfo->cmasLst.count == 0)
   {
      wrPwsStopPagingwithCmasInd(cellCb);

      retransSiIndex = cellCb->warningInfo->sib12SiIdx;
      /* Configure SIB1 scheduling info. */
      if (WR_PBM_SEND_NONE == wrPbmUpdSiSchedInfoLstInSIB1(cellCb,
               WR_SIB_ACTION_REMOVE,
               WR_SIB_12,
               &retransSiIndex))
      {
          RLOG0(L_ERROR,"wrPbmUpdSiSchedInfoLstInSIB1:SIB1 SI scheduling"
             "info update failed.");
      }

      /* Configure MAC with updated SIB1 information. */
      if (wrEmmPbmSib1Config(cellCb->cellId, WR_SIB1_KILL_TRANS_ID) != ROK)
      {
         RLOG0(L_ERROR,"wrEmmPbmSib1Config:SIB1 SI scheduling info"
            "configuration failed.");
      }

   } /* if last CMAS. */
   else
   {
      /* If not the only CMAS then pick up the
         priority CMAS and configure MAC. */
      if (NULLP != &(crntCmas->cmasLstLnk))
      {
         cmasNode = (WrPwsCmasInfo *)(crntCmas->cmasLstLnk.node);
         if(NULLP != cmasNode)
         {
            cellCb->warningInfo->currentCmas = cmasNode;
         }
      }
      else
      {
         CM_LLIST_FIRST_NODE(cmasLst, cmasLstLnk);
         if (NULLP != cmasLstLnk)
         {
            cellCb->warningInfo->currentCmas = (WrPwsCmasInfo*)(cmasLstLnk->node);
         }
      }
   }/* else last CMAS */

   RETVALUE(ROK);
} /* wrPwsHndlStopWrngSiCfgCfm */

PUBLIC S16 wrEmmPwsProcRepTimerExp
(        
 Void                         *cb,
 U8                           wrUmmTmrExpEvnt
 )
{
   WrPwsMsgRepeatInfo        *repInfo = NULLP;
   U8                        wrngType;
   S16                       ret = ROK;
   WrCellCb                  *cellCb = NULLP;

   TRC2(wrEmmPwsProcRepTimerExp)

      RLOG0(L_DEBUG,"Repetition period timer expired.");

   repInfo = (WrPwsMsgRepeatInfo*)cb;

   wrngType = repInfo->wrngType;
   cellCb = (WrCellCb*)repInfo->cellCb;

   if(wrngType == WR_PWS_PRI_NOTIFICATION)
   {
      ret = wrEmmPwsPrimEtwsRepTimerExp(cellCb, repInfo);
      if(RFAILED == ret)
      {
         RLOG0(L_ERROR,"wrEmmPwsPrimEtwsRepTimerExp:Unable to handle repetition"
            "period timer expiry for Primary ETWS.");
         RETVALUE(RFAILED);
      }
   }  
   if(wrngType == WR_PWS_SEC_NOTIFICATION)
   {  
      ret = wrEmmPwsSecEtwsRepTimerExp(cellCb, repInfo);
      if(RFAILED == ret)
      {
         RLOG0(L_ERROR,"wrEmmPwsSecEtwsRepTimerExp:Unable to handle repetition"
            "period timer expiry for Secondary ETWS.");
         RETVALUE(RFAILED);
      }

   }
   else if(wrngType == WR_PWS_CMAS_NOTIFICATION)
   {
      ret = wrEmmPwsCmasRepTimerExp(cellCb, repInfo);
      if(RFAILED == ret)
      {
         RLOG0(L_ERROR,"wrEmmPwsCmasRepTimerExp:Unable to handle repetition"
            "period timer expiry for CMAS.");
         RETVALUE(RFAILED);
      }
   }

   RETVALUE(ret);
} /* wrEmmPwsProcRepTimerExp */           


PRIVATE S16 wrEmmPwsPrimEtwsRepTimerExp
(        
 WrCellCb                  *cellCb ,
 WrPwsMsgRepeatInfo        *repInfo 
 )
{
   S16                       ret = ROK;
   WrPwsEtwsInfo             *etwsInfo = NULLP;
   U8               retransSiIndex = 0;

   RLOG0(L_DEBUG,"Repetition period timer expired for Primary ETWS.");

   /* Extract the etwsInfo. */
   etwsInfo = (WrPwsEtwsInfo*)(repInfo->wrngInfo);
   etwsInfo->primEtwsInfo.readyToSch = TRUE;       
   /* Start broacasting again if number of broadcast requested counter
      not reached to zero. */
   if(etwsInfo->primEtwsInfo.numBroadcastReq > 0)
   {
      /* COnfigure SIB10. */
      RLOG0(L_DEBUG,"Configuring MAC for SIB10");


      /* Configure SIB1 scheduling info. */
      if (WR_PBM_SEND_NONE == wrPbmUpdSiSchedInfoLstInSIB1(cellCb,
               WR_SIB_ACTION_ADD,
               WR_SIB_10,
               &retransSiIndex))
      {
         RLOG0(L_ERROR," wrPwsUpdEtwsInSib1SchedInfoLst SIB1: SI "
            "schedulinginfo update failed because of unknown periodicity.");
         RETVALUE(RFAILED);
      }

      cellCb->warningInfo->sib10SiIdx = retransSiIndex;
      /* Configure MAC with updated SIB1 information. */
      if (wrEmmPbmSib1Config(cellCb->cellId,WR_SIB1_PRI_ETWS_TRANS_ID) != ROK)
      {
         RLOG0(L_ERROR,"wrEmmPbmSib1Config:"
            "SIB1 SI schedulin info configuration failed.");
         RETVALUE(RFAILED);
      }
      if( cellCb->warningInfo->etwsInfo.notificationType == WR_PWS_PRI_NOTIFICATION)
      {
         /* Send the paging message with the ETWS indication. */  
         ret = wrPwsSndPagingwithEtwsInd(cellCb);
         if(ret == RFAILED)
         {
            RLOG0(L_ERROR,"wrPwsSndPagingwithEtwsInd:"
               "Failed to start paging with ETWS indication.");
            RETVALUE(RFAILED);
         }
         if ( RFAILED == wrPbmStartWarningMsg(cellCb,WR_START_PRI_ETWS_IND))
         {
            RLOG0(L_ERROR,"wrPbmStartWarningMsg:"
               "Unable to process the warning message.");
            RETVALUE(RFAILED);
         }
      }
   }

   RETVALUE(ret);
} /* wrEmmPwsPrimEtwsRepTimerExp */


PRIVATE S16 wrEmmPwsSecEtwsRepTimerExp
(        
 WrCellCb                  *cellCb ,
 WrPwsMsgRepeatInfo        *repInfo 
 )
{
   S16                       ret = ROK;
   WrPwsEtwsInfo             *etwsInfo = NULLP;
   U8               retransSiIndex = 0;

   RLOG0(L_DEBUG ,"Repetition period timer expired for Secondary ETWS.");

   /* Extract the etwsInfo. */
   etwsInfo = (WrPwsEtwsInfo*)(repInfo->wrngInfo);
   etwsInfo->secEtwsInfo.readyToSch = TRUE;       
   /* Start broacasting again if number of broadcast requested counter
      not reached to zero. */
   if(cellCb->warningInfo->isSecEtwsInProgress == FALSE)
   {
      if(etwsInfo->secEtwsInfo.numBroadcastReq > 0)
      {
         /* COnfigure SIB11. */
         RLOG0(L_DEBUG,"Configure MAC for SIB11.");

         if (WR_PBM_SEND_NONE == wrPbmUpdSiSchedInfoLstInSIB1(cellCb,
                  WR_SIB_ACTION_ADD,
                  WR_SIB_11,
                  &retransSiIndex))
         {
            RLOG0(L_ERROR,"wrPbmUpdSiSchedInfoLstInSIB1:SIB1 SI" 
               "schedulinginfo update failed because of unknown periodicity");
            RETVALUE(RFAILED);
         }

         cellCb->warningInfo->sib11SiIdx = retransSiIndex;
         /* Configure MAC with updated SIB1 information. */
         if (wrEmmPbmSib1Config(cellCb->cellId, WR_SIB1_SEC_ETWS_TRANS_ID) != ROK)
         {
            RLOG0(L_ERROR,"wrEmmPbmSib1Config:"
               "SIB1 SI schedulin info configuration failed.");
            RETVALUE(RFAILED);
         }

         /* Send the paging message with the ETWS indication. */  
         ret = wrPwsSndPagingwithEtwsInd(cellCb);
         if(ret == RFAILED)
         {
            RLOG0(L_ERROR,"wrPwsSndPagingwithEtwsInd:"
               "Failed to start paging with ETWS indication.");
            RETVALUE(RFAILED);
         }
   if(cellCb->warningInfo->etwsInfo.notificationType == WR_PWS_BOTH_NOTIFICATIONS)
         {
            if ( RFAILED == wrPbmStartWarningMsg(cellCb,WR_START_BOTH_ETWS_IND))
            {
               RLOG0(L_ERROR,"wrPbmStartWarningMsg :"
                  "Unable to process the warning message");
               RETVALUE(RFAILED);
            }
         }
         else 
         {
            if ( RFAILED == wrPbmStartWarningMsg(cellCb,WR_START_SEC_ETWS_IND))
            {
               RLOG0(L_ERROR,"wrPbmStartWarningMsg :"
                  "Unable to process the warning message");
               RETVALUE(RFAILED);
            }
         }

      }
   }

   RETVALUE(ret);
} /* wrEmmPwsSecEtwsRepTimerExp */


PRIVATE S16 wrEmmPwsCmasRepTimerExp
(        
 WrCellCb                  *cellCb ,
 WrPwsMsgRepeatInfo        *repInfo 
 )
{
   S16                       ret = ROK;
   WrPwsCmasInfo             *cmasInfo = NULLP;

   U8                        retransSiIndex = 0;
   /* Extract the cmasInfo. */
   cmasInfo = (WrPwsCmasInfo*)(repInfo->wrngInfo);

   RLOG2(L_DEBUG,"Repetition period timer expired for the CMAS warning message"
      "with message Id:[%d] and Serial Number: [%d]", 
      cmasInfo->wrngMsgId, cmasInfo->wrngSerialNum);
   /*printf("Repetition period timer expired for the CMAS warning message:with message Id:[%d] and Serial Number: [%d]",cmasInfo->wrngMsgId, cmasInfo->wrngSerialNum);*/

   /* Set the falg as TRUE. */
   cmasInfo->wrngMsgInfo.readyToSch = TRUE;       

   /* cmas_fix2 */
   if(cellCb->warningInfo->isCmasInProgress == FALSE) 
   {
         if(((cmasInfo->wrngMsgInfo.numBroadcastReq > 0 ) 
                  || TRUE == cmasInfo->waitToStop))
         {
         /* Configure SIB1 scheduling info. */
         if (WR_PBM_SEND_NONE == wrPbmUpdSiSchedInfoLstInSIB1(cellCb,
                  WR_SIB_ACTION_ADD,
                  WR_SIB_12,
                  &retransSiIndex))
         {
            RLOG0(L_ERROR,"current CMAS is NULL.");
            RETVALUE(RFAILED);
         }

         cellCb->warningInfo->sib12SiIdx = retransSiIndex;

         /* Configure MAC with updated SIB1 information. */
         if (wrEmmPbmSib1Config(cellCb->cellId, WR_SIB1_CMAS_TRANS_ID) != ROK)
            {
            RLOG0(L_ERROR,"wrPwsCmasProcessWrng:"
                     "SIB1 SI schedulin info configuration failed.\n");
               RETVALUE(RFAILED);
            }


         /* COnfigure SIB12. */
         /* Create the SIs and configure the MAC for SI transmission. */
         RLOG0(L_INFO,"wrEmmPwsCmasRepTimerExp:" "No of broadcast is not 0."
                  "Configure it to MAC.\n");

         cellCb->warningInfo->currentCmas = cmasInfo;
         ret =  wrPwsSndPagingwithCmasInd(cellCb);
         if(ret == RFAILED)
         {
            RLOG0(L_ERROR,"wrPwsCmasProcessWrng:"
                     "Failed to start paging with CMAS indication.\n");
            RETVALUE(RFAILED);
         }

         if (RFAILED == wrPbmStartWarningMsg(cellCb,WR_START_CMAS_IND))
         {
            RLOG0(L_ERROR,"wrEmmPwsCmasRepTimerExp: :"
                     "Unable to process the warning message. \n");
            RETVALUE(RFAILED);
         }

             cellCb->warningInfo->isCmasInProgress = TRUE;
         }
   }
   else
   {
      RLOG0(L_INFO,  "wrEmmPwsCmasRepTimerExp: Can not schedule the next CMAS now"
               "as warning SI config confirm is not received for earlier configuration."
               "Need to wait for configuration confirm. \n");
   }

   RETVALUE(ret);
} /* wrEmmPwsCmasRepTimerExp */


PRIVATE S16 wrPwsStartRepetionTmr
(
 U16                         wrngType,
 WrPwsEtwsInfo               *etwsInfo,
 WrPwsCmasInfo               *cmasInfo,
 WrCellCb                    *cellCb
 )
{
   WrPwsEtwsInfo               *crntEtwsInfo = NULLP;
   WrPwsCmasInfo               *crntCmasInfo = NULLP;

   if(wrngType == WR_PWS_CMAS_NOTIFICATION)
   {
      /* cmas_fix2 : ccpu00127659 */ 
      /* Update the isCmasInProgress flag. */

      crntCmasInfo = cellCb->warningInfo->currentCmas;
      crntCmasInfo->wrngMsgInfo.repetitionInfo.wrngType = WR_PWS_CMAS_NOTIFICATION;
      crntCmasInfo->wrngMsgInfo.repetitionInfo.wrngInfo = (PTR)(cmasInfo);
      crntCmasInfo->wrngMsgInfo.repetitionInfo.cellCb = (PTR)cellCb;
      cmInitTimers(&(crntCmasInfo->wrngMsgInfo.repetitionInfo.repetitionTmr), 1);
      RLOG1(L_DEBUG,"Starting repetition period timer for the CMAS"
         " warning message for [%d] ms.",
         cmasInfo->wrngMsgInfo.repetitionPeriod * WR_PWS_MS_UNIT);
      wrStartTmr((PTR)&(crntCmasInfo->wrngMsgInfo.repetitionInfo),
            WR_TMR_REPETITION_PERIOD,
            (cmasInfo->wrngMsgInfo.repetitionPeriod * WR_PWS_MS_UNIT));
      crntCmasInfo->wrngMsgInfo.readyToSch = FALSE;
   }
   else if(wrngType == WR_PWS_PRI_NOTIFICATION)
   {
      crntEtwsInfo = &(cellCb->warningInfo->etwsInfo);

      /* Update the isPrimEtwsInProgress flag. */
      cellCb->warningInfo->isPrimEtwsInProgress = TRUE;

      crntEtwsInfo->primEtwsInfo.repetitionInfo.wrngType = WR_PWS_PRI_NOTIFICATION;
      crntEtwsInfo->primEtwsInfo.repetitionInfo.wrngInfo = (PTR)(etwsInfo);
      crntEtwsInfo->primEtwsInfo.repetitionInfo.cellCb = (PTR)cellCb;
      cmInitTimers(&(crntEtwsInfo->primEtwsInfo.repetitionInfo.repetitionTmr), 1);

      RLOG1(L_DEBUG,"Starting repetition period timer for the primary ETWS"
         "warning message for [%d] ms",
         etwsInfo->primEtwsInfo.repetitionPeriod * WR_PWS_MS_UNIT);

      wrStartTmr((PTR)&(crntEtwsInfo->primEtwsInfo.repetitionInfo),
            WR_TMR_REPETITION_PERIOD,
            (etwsInfo->primEtwsInfo.repetitionPeriod * WR_PWS_MS_UNIT));
      crntEtwsInfo->primEtwsInfo.readyToSch = FALSE;
   }
   else if(wrngType == WR_PWS_SEC_NOTIFICATION)
   {
      crntEtwsInfo = &(cellCb->warningInfo->etwsInfo);

      /* Update the isSecEtwsInProgress flag. */
      cellCb->warningInfo->isSecEtwsInProgress = TRUE;

      crntEtwsInfo->secEtwsInfo.repetitionInfo.wrngType = WR_PWS_SEC_NOTIFICATION;
      crntEtwsInfo->secEtwsInfo.repetitionInfo.wrngInfo = (PTR)(etwsInfo);
      crntEtwsInfo->secEtwsInfo.repetitionInfo.cellCb = (PTR)cellCb;
      cmInitTimers(&(crntEtwsInfo->secEtwsInfo.repetitionInfo.repetitionTmr), 1);
      wrStartTmr((PTR)&(crntEtwsInfo->secEtwsInfo.repetitionInfo),
            WR_TMR_REPETITION_PERIOD,
            (etwsInfo->secEtwsInfo.repetitionPeriod * WR_PWS_MS_UNIT));
      RLOG0(L_DEBUG,"Started repetition period timer for the Secondary"
         "ETWS warning message. \n");
      crntEtwsInfo->secEtwsInfo.readyToSch = FALSE;  
   }

   RETVALUE(ROK);
} /* wrPwsStartRepetionTmr */


PUBLIC S16 wrPwsStartPrimaryEtws 
(        
 WrCellCb            *cellCb
 )
{
   S16                ret = ROK;

   /* Create the SIs and configure the MAC for SI transmission. */
   ret = wrEmmPbmWrngSiConfig(cellCb->cellId, WR_PBM_SIB10_SI_INDEX);
   if(ret == RFAILED)
   {
      RLOG0(L_ERROR,"wrPwsStartPrimaryEtws :"
         "Warning SI schedulin info configuration failed.");
      RETVALUE(RFAILED);
   }

   /* Start the repetition period timer. */
   wrPwsStartRepetionTmr(WR_PWS_PRI_NOTIFICATION, 
         &(cellCb->warningInfo->etwsInfo), NULLP , cellCb);

   RETVALUE(ROK);
} /* wrPwsStartPrimaryEtws */


PUBLIC S16 wrPwsStartSecondaryEtws 
(        
 WrCellCb            *cellCb
 )
{
   S16                  ret = ROK;

   /* Create the SIs and configure the MAC for SI transmission. */
   ret = wrEmmPbmWrngSiConfig(cellCb->cellId, WR_PBM_SIB11_SI_INDEX);
   if(ret == RFAILED)
   {
      RLOG0(L_ERROR,"wrEmmPbmWrngSiConfig:"
         "Warning SI scheduling info configuration failed");
      RETVALUE(RFAILED);
   }

   /* Start the repetition period timer. */
   wrPwsStartRepetionTmr(WR_PWS_SEC_NOTIFICATION, 
         &(cellCb->warningInfo->etwsInfo), NULLP , cellCb);

   RETVALUE(ROK);
} /* wrPwsStartSecondaryEtws */


PUBLIC S16 wrPwsStartCmas 
(        
 WrCellCb            *cellCb
 )
{
   S16                       ret = ROK;

   if((cellCb->warningInfo->currentCmas != NULL)&&
         (cellCb->warningInfo->cmasLst.count > 0 || 
          TRUE == cellCb->warningInfo->currentCmas->waitToStop)) 
   {
      /*printf("Sending Message Serial Number:%d \n",cellCb->warningInfo->currentCmas->wrngMsgId);*/
      /* if(cellCb->warningInfo->isCmasInProgress == FALSE)*/
      {
         /*cellCb->warningInfo->isCmasInProgress = TRUE;*/
         /* Create the SIs and configure the MAC for SI transmission. */
         ret = wrEmmPbmWrngSiConfig(cellCb->cellId, WR_PBM_SIB12_SI_INDEX);

         if(ret == RFAILED)
         {
            RLOG0(L_ERROR,"wrEmmPbmWrngSiConfig:"
                  "Warning SI scheduling info configuration failed");
            RETVALUE(RFAILED);
         }
      }
      /* CMAS  ccpu00127868 start  */
      if(cellCb->warningInfo->currentCmas->numberOfBroadcasts != WR_PWS_MAX_BRDCAST)
         cellCb->warningInfo->currentCmas->numberOfBroadcasts++;
      /* CMAS  ccpu00127868 end  */

      wrPwsStartRepetionTmr(WR_PWS_CMAS_NOTIFICATION, 
            NULLP, (cellCb->warningInfo->currentCmas), cellCb);
   }
   RETVALUE(ROK);
} /* wrPwsStartCmas */

PRIVATE S16 wrPwsStopCmas 
(        
 WrCellCb            *cellCb
 )
{
   S16              ret = ROK;
   U8               retransSiIndex = 0;

   if(ret == RFAILED)
   {
      RLOG0(L_ERROR,"wrPwsStopPagingwithCmasInd:"
         "Failed to stop paging with CMAS indication.");
      RETVALUE(RFAILED);
   }

   retransSiIndex = cellCb->warningInfo->sib12SiIdx;
   /* Configure SIB1 scheduling info. */
   if (WR_PBM_SEND_NONE == wrPbmUpdSiSchedInfoLstInSIB1(cellCb,
            WR_SIB_ACTION_REMOVE,
            WR_SIB_12,
            &retransSiIndex))
   {
      RLOG0(L_ERROR,"wrPbmUpdSiSchedInfoLstInSIB1:"
         "SIB1 SI scheduling info update failed.");
      RETVALUE(RFAILED);
   }

   /* Configure MAC with updated SIB1 information. */
   if (wrEmmPbmSib1Config(cellCb->cellId, WR_SIB1_TRANS_ID) != ROK)
   {
      RLOG0(L_ERROR,"wrEmmPbmSib1Config:"
         "SIB1 SI schedulin info configuration failed.");
      RETVALUE(RFAILED);
   }

   cellCb->warningInfo->currentCmas = NULLP;

   RETVALUE(ROK);
}  /* wrPwsStopCmas */


PRIVATE S16 wrPwsStopSecondaryEtws 
(        
 WrCellCb            *cellCb
 )
{
   U8                 retransSiIndex = 0;
   WrEtwsPagInfo      *pagInfo; 

   /* Stop the paging message with the ETWS indication. */
   /* Fix for ccpu00139096 */
   pagInfo = wrPwsGetWaringPagingInfo(cellCb, WR_START_PAG_SEC_ETWS);
   wrPbmPwsStopPagingEtws(cellCb, pagInfo);

   retransSiIndex = cellCb->warningInfo->sib11SiIdx;
   /* Configure SIB1 scheduling info. */
   if (WR_PBM_SEND_NONE == wrPbmUpdSiSchedInfoLstInSIB1(cellCb,
            WR_SIB_ACTION_REMOVE,
            WR_SIB_11,
            &retransSiIndex))
   {
      RLOG0(L_ERROR,"wrPbmUpdSiSchedInfoLstInSIB1:"
         "SIB1 SI scheduling info update failed.");
      RETVALUE(RFAILED);
   }

   /* Configure MAC with updated SIB1 information. */
   if (wrEmmPbmSib1Config(cellCb->cellId, WR_SIB1_SEC_ETWS_TRANS_ID) != ROK)
   {
      RLOG0(L_ERROR,"wrEmmPbmSib1Config"
         "SIB1 SI scheduling info configuration failed.");
      RETVALUE(RFAILED);
   }

   cellCb->warningInfo->isSecEtwsInProgress = FALSE;
   cellCb->warningInfo->etwsInfo.wrngMsgId = WR_PWS_INVALID_MSGID;

   RETVALUE(ROK);
} /* wrPwsStopSecondaryEtws */

PRIVATE S16 wrPwsStopPrimaryEtws 
(        
 WrCellCb            *cellCb
 )
{
   U8                retransSiIndex = 0;
   WrEtwsPagInfo     *pagInfo; 

   /* Stop the paging message with the ETWS indication. */
   /* Fix for ccpu00139096 */
   pagInfo = wrPwsGetWaringPagingInfo(cellCb, WR_START_PAG_SEC_ETWS);
   wrPbmPwsStopPagingEtws(cellCb, pagInfo);
   pagInfo = wrPwsGetWaringPagingInfo(cellCb, WR_START_PAG_PRI_ETWS);
   wrPbmPwsStopPagingEtws(cellCb, pagInfo);
   pagInfo = wrPwsGetWaringPagingInfo(cellCb, WR_STOP_PAG_ETWS);
   wrPbmPwsStopPagingEtws(cellCb, pagInfo);


   retransSiIndex = cellCb->warningInfo->sib10SiIdx;
   /* Configure SIB1 scheduling info. */
   if (WR_PBM_SEND_NONE == wrPbmUpdSiSchedInfoLstInSIB1(cellCb,
            WR_SIB_ACTION_REMOVE,
            WR_SIB_10,
            &retransSiIndex))
   {
      RLOG0(L_ERROR,"wrPbmUpdSiSchedInfoLstInSIB1 :"
         "SIB1 SI scheduling info update failed.");
      RETVALUE(RFAILED);
   }

   /* Configure MAC with updated SIB1 information. */
   if (wrEmmPbmSib1Config(cellCb->cellId, WR_SIB1_PRI_ETWS_TRANS_ID) != ROK)
   {
      RLOG0(L_ERROR,"wrEmmPbmSib1Config :"
         "SIB1 SI schedulin info configuration failed.");
      RETVALUE(RFAILED);
   }
   cellCb->warningInfo->etwsInfo.wrngMsgId = WR_PWS_INVALID_MSGID;

   RETVALUE(ROK);
} /* wrPwsStopPrimaryEtws */

PRIVATE S16 wrPwsStopBothEtws 
(        
 WrCellCb            *cellCb
 )
{
   U8                retransSiIndex = 0;
   WrEtwsPagInfo     *pagInfo; 

   /* Stop the paging message with the ETWS indication. */
   /* Fix for ccpu00139096 */
   pagInfo = wrPwsGetWaringPagingInfo(cellCb, WR_START_PAG_PRI_ETWS);
   wrPbmPwsStopPagingEtws(cellCb, pagInfo);
   if(pagInfo->pf != cellCb->warningInfo->etwsInfo.waringPagingInfo[WR_START_PAG_SEC_ETWS].pf)
   {
      pagInfo = wrPwsGetWaringPagingInfo(cellCb, WR_START_PAG_SEC_ETWS);
      wrPbmPwsStopPagingEtws(cellCb, pagInfo);
   }
   pagInfo = wrPwsGetWaringPagingInfo(cellCb, WR_START_PAG_ETWS);
   wrPbmPwsStopPagingEtws(cellCb, pagInfo);
   pagInfo = wrPwsGetWaringPagingInfo(cellCb, WR_STOP_PAG_ETWS);
   wrPbmPwsStopPagingEtws(cellCb, pagInfo);

   retransSiIndex = cellCb->warningInfo->sib11SiIdx;
   /* Remove SIB11 from the SIB1 scheduling info list. */
   if (WR_PBM_SEND_NONE == wrPbmUpdSiSchedInfoLstInSIB1(cellCb,
            WR_SIB_ACTION_REMOVE,
            WR_SIB_11,
            &retransSiIndex))
   {
      RLOG0(L_ERROR,"wrPbmUpdSiSchedInfoLstInSIB1:"
         "SIB1 SI scheduling info update failed.");
      RETVALUE(RFAILED);
   }

   retransSiIndex = cellCb->warningInfo->sib10SiIdx;
   /* Remove SIB10 from the SIB1 scheduling info list. */
   if (WR_PBM_SEND_NONE == wrPbmUpdSiSchedInfoLstInSIB1(cellCb,
            WR_SIB_ACTION_REMOVE,
            WR_SIB_10,
            &retransSiIndex))
   {
      RLOG0(L_ERROR,"wrPbmUpdSiSchedInfoLstInSIB1:"
         "SIB1 SI scheduling info update failed.");
      RETVALUE(RFAILED);
   }

   /* Configure MAC with updated SIB1 information. */
   if (wrEmmPbmSib1Config(cellCb->cellId, WR_SIB1_BOTH_ETWS_TRANS_ID) != ROK)
   {
      RLOG0(L_ERROR,"wrEmmPbmSib1Config:"
         "SIB1 SI schedulin info configuration failed.");
      RETVALUE(RFAILED);
   }
   cellCb->warningInfo->etwsInfo.wrngMsgId = WR_PWS_INVALID_MSGID;

   RETVALUE(ROK);
} /* wrPwsStopBothEtws */


PRIVATE S16 wrPwsStopEtwsBasedOnType
(        
 WrCellCb            *cellCb
 )
{
   S16               ret = ROK;

   if(cellCb->warningInfo->etwsInfo.notificationType == WR_PWS_PRI_NOTIFICATION) 
   {
      ret = wrPwsStopPrimaryEtws(cellCb); 
      if( ret != ROK )
      {
         RLOG0(L_WARNING,"wrPwsStopPrimaryEtws:"
            "Unable to stop ongoing primary ETWS.");
      }
   }
   else if(cellCb->warningInfo->etwsInfo.notificationType == WR_PWS_SEC_NOTIFICATION) 
   {
      ret = wrPwsStopSecondaryEtws(cellCb); 
      if( ret != ROK )
      {
         RLOG0(L_WARNING,"wrPwsStopSecondaryEtws:Unable to stop"
            "ongoing primary ETWS.");
      }
   }
   else if(cellCb->warningInfo->etwsInfo.notificationType == WR_PWS_BOTH_NOTIFICATIONS)
   {
      if(cellCb->warningInfo->etwsInfo.wrngMsgId != WR_PWS_INVALID_MSGID)
      {
         /* If secondary or Primary ETWS is not already going on. */
         if(!(cellCb->warningInfo->isSecEtwsInProgress)
               || !( cellCb->warningInfo->etwsInfo.secEtwsInfo.readyToSch)
               || !(cellCb->warningInfo->isPrimEtwsInProgress)
               || !(cellCb->warningInfo->etwsInfo.primEtwsInfo.readyToSch))
         {
            ret = wrPwsStopBothEtws(cellCb);
            if( ret != ROK )
            {
               RLOG0(L_WARNING,"wrPwsStopBothEtws:Unable to stop both primary"
                  "and secondary ETWS.");
            }
         }
      }
   }

   RETVALUE(ROK);
} /* wrPwsStopEtwsBasedOnType */ 



S16 wrEmmupdatePwsConfig(U8 newpwsEtwsCmasval, U8 oldpwsEtwsCmasval)
{
      WrCellCb  *cellCb  = NULLP;
      S16        cellIdx = 0;
      
      /*printf("######newpwsEtwsCmasval:%d oldpwsEtwsCmasval: %d \n",newpwsEtwsCmasval,oldpwsEtwsCmasval);*/
      for(cellIdx = 0; cellIdx < wrEmmCb.numCells; ++cellIdx)
      {
           cellCb = wrEmmCb.cellCb[cellIdx];
           if (NULLP != cellCb->warningInfo)
           {
               switch(oldpwsEtwsCmasval)
              {
                    case WR_PWS_ETWS:
                    {
                        if(cellCb->warningInfo->etwsInfo.notificationType == WR_PWS_PRI_NOTIFICATION) 
                        { 
                           if(cellCb->warningInfo->etwsInfo.primEtwsInfo.readyToSch == TRUE)
                           {    
                                
                                cellCb->warningInfo->etwsInfo.primEtwsInfo.numBroadcastReq = 0;
                           }
                           else
                           {
                                
                                wrStopTmr((PTR)&(cellCb->warningInfo->etwsInfo.primEtwsInfo.repetitionInfo), WR_TMR_REPETITION_PERIOD );
                                WR_FREEMBUF(cellCb->warningInfo->etwsInfo.primEtwsInfo.pdu); 
                                WR_FREE(cellCb->warningInfo, sizeof(WrPwsWarningInfo));
                           } 
                        }
                        else if(cellCb->warningInfo->etwsInfo.notificationType == WR_PWS_SEC_NOTIFICATION)
                        {
                             if(cellCb->warningInfo->etwsInfo.secEtwsInfo.readyToSch == TRUE)
                             {
                                cellCb->warningInfo->etwsInfo.secEtwsInfo.numBroadcastReq = 0;
                             }
                             else
                             {
                                 wrStopTmr((PTR)&(cellCb->warningInfo->etwsInfo.secEtwsInfo.repetitionInfo),WR_TMR_REPETITION_PERIOD );
                                 wrPwsFreePdu(&(cellCb->warningInfo->etwsInfo.secEtwsInfo.segPduLst));
                                 WR_FREE(cellCb->warningInfo, sizeof(WrPwsWarningInfo));

                             }
 
                        } 
                        else if(cellCb->warningInfo->etwsInfo.notificationType == WR_PWS_BOTH_NOTIFICATIONS) 
                        {
                             if(cellCb->warningInfo->etwsInfo.primEtwsInfo.readyToSch == TRUE)
                             {

                                cellCb->warningInfo->etwsInfo.primEtwsInfo.numBroadcastReq = 0;
                             }
                             else
                             {

                                wrStopTmr((PTR)&(cellCb->warningInfo->etwsInfo.primEtwsInfo.repetitionInfo), WR_TMR_REPETITION_PERIOD );
                                WR_FREEMBUF(cellCb->warningInfo->etwsInfo.primEtwsInfo.pdu); 
                             }
                             if(cellCb->warningInfo->etwsInfo.secEtwsInfo.readyToSch == TRUE)
                             {
                                cellCb->warningInfo->etwsInfo.secEtwsInfo.numBroadcastReq = 0;
                             }
                             else
                             {
                                 wrStopTmr((PTR)&(cellCb->warningInfo->etwsInfo.secEtwsInfo.repetitionInfo),WR_TMR_REPETITION_PERIOD );
                                 wrPwsFreePdu(&(cellCb->warningInfo->etwsInfo.secEtwsInfo.segPduLst));
                             }
                        }
                         break;
                    }
                    case WR_PWS_CMAS:
                    {    
                       CmLListCp                  *cmasLst = NULLP;
                       CmLList                    *cmasLstLnk = NULLP;
                       WrPwsCmasInfo              *cmasInfoPtr = NULLP;

                       cmasLst = &(cellCb->warningInfo->cmasLst);
                       CM_LLIST_FIRST_NODE(cmasLst, cmasLstLnk);
                       while(cmasLst->count !=0)
                       {
                          cmasInfoPtr = (WrPwsCmasInfo *)(cmasLstLnk->node);
                          if(NULLP != cmasInfoPtr)
                          {   
                             if(cmasInfoPtr->wrngMsgInfo.readyToSch == FALSE)
                             {
                                wrStopTmr((PTR)&(cmasInfoPtr->wrngMsgInfo.repetitionInfo), WR_TMR_REPETITION_PERIOD);
                             }
                             wrPwsFreePdu(&(cmasInfoPtr->wrngMsgInfo.segPduLst));
                             /*printf("deleted CMAS warning message:with message Id:[%d] and Serial Number: [%d]\n",cmasInfoPtr->wrngMsgId, cmasInfoPtr->wrngSerialNum);*/
                             cmLListDelFrm(cmasLst, &(cmasInfoPtr->cmasLstLnk));
                             WR_FREE(cmasInfoPtr, sizeof(WrPwsCmasInfo));
                          }    
                          CM_LLIST_NEXT_NODE(cmasLst, cmasLstLnk);
                       }    

                      break;
                          CM_LLIST_NEXT_NODE(cmasLst, cmasLstLnk);
                    }
                    default:
                    {
                         RLOG0(L_INFO,"wrEmmupdatePwsConfig:No action to be taken\n"); 
                    }  
               }
            }
            else
            {
                 RLOG0(L_INFO,"wrEmmupdatePwsConfig:There is no warning Going on\n"); 
            }
      }
   RETVALUE(ROK);
}
/** @brief This function is a utility function to free the segment PDU.
 *
 * @details This function is a utility function to free the segment PDU.
 *
 *     Function: wrPwsFreePdu
 *
 *         Processing steps:
 *
 * @param [in] pduLst : PDU List
 * 
 * @return S16
 *    -# Success : ROK
 *    -# Failure : RFAILED
 */
PRIVATE S16 wrPwsFreePdu
(
  CmLListCp                  *pduLst
)
{
   CmLList                    *pduLnk   = NULLP;
   WrPwsSegmentInfo           *pdu = NULLP;

   /* Get the first pdu. */
   CM_LLIST_FIRST_NODE(pduLst, pduLnk);

   while (pduLst->count != 0)
   {
      pdu = (WrPwsSegmentInfo*)(pduLnk->node);

      /* Remove the segment PDU from the pdu list and free it. */
      WR_FREEMBUF(pdu->pdu);
      cmLListDelFrm(pduLst, pduLnk);
      WR_FREE(pdu, sizeof(WrPwsSegmentInfo));
      CM_LLIST_NEXT_NODE(pduLst,pduLnk);
   }

   RETVALUE(ROK);
}/* wrPwsFreePdu */


/* Fix for ccpu00139096 */
/** @brief this function is used get warning info
 *
 * @details
 *
 *     Function: wrPwsGetWaringPagingInfo
 *
 *         Processing steps:
 *         - return the warning info depends on received warning type
 *
 * @param[in] cellCb : cell control block.
 * @param[in] type   : warning  type.
 *
 * @return WrEtwsPagInfo*
 *              - pointer to WrEtwsPagInfo
 */

PRIVATE WrEtwsPagInfo *wrPwsGetWaringPagingInfo
(        
 WrCellCb              *cellCb,
 WrWaringPagingType    type
)
{
   RETVALUE (&cellCb->warningInfo->etwsInfo.waringPagingInfo[type]);  
} /*end of wrPwsGetWaringPagingInfo*/

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
