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
               

     File:     wr_emm_mme.h

     Sid:      $SID$ 

     Prg:      Sriky 

*********************************************************************21*/

#ifndef __WR_EMM_MME_H__
#define __WR_EMM_MME_H__

#include "wr.h"
#include "wr_emm.h"
#include "wr_ifm_s1ap.h"
#include "wr_smm_init.h"
#include "wr_umm_rrccon.h"

#ifdef __cplusplus
extern "C" {
#endif /* end of __cplusplus */

#define WR_EMM_MME_GET_MME(_mmeId) wrEmmCb.mmeCont.mmes[_mmeId]
/** 
 * @brief This structure contains EnodeB Information
 * @details These are the structure members
 * - U32                       enbType     ENodeB type.
 * - U32                       cellId      Cell Id.
 * - U32                       tac         Tracking Area Code.
 * - WrPlmnId                  plmnId      PLMN ID.
 */
typedef struct wrEmmMmeEnbInfo
{
   U32                       enbType;
   U32                       cellId;
   U32                       tac;
   WrPlmnId                  plmnId;
} WrEmmMmeEnbInfo;

/**
 * @brief This structure contains EnodeB Configration Transfer information.
 * @details These are the structure members
 * -Bool                      isReply        TRUE:Add SON Information Reply IE,
 *                                           FALSE:Add SON Inforamtion Request.
 * -WrEmmMmeEnbInfo           targetInfo     Target EnodeB Information.
 * -WrEmmMmeEnbInfo           sourceInfo     Source EnodeB Information.
 */
typedef struct wrEmmMmeEnbCfgTrnsfr
{
   Bool                      isReply;
   WrEmmMmeEnbInfo           targetInfo;
   WrEmmMmeEnbInfo           sourceInfo;
} WrEmmMmeEnbCfgTrnsfr;

EXTERN S16 wrEmmMmeBldSetupReq(S1apPdu **pdu);
EXTERN S16 wrEmmMmeBldMmeCfgUpdAck(S1apPdu **pdu);
EXTERN S16 wrEmmMmeBldWriteRepResp(S1apPdu **pdu, U16 serialNum);
EXTERN S16 wrEmmMmeBldKillRsp(S1apPdu **pdu, U16 serialNum);
EXTERN S16 wrEmmMmeBldMmeCfgUpdFail(S1apPdu **pdu, WrUmmMsgCause *cause, 
                                    U8 timeToWaitSecs);
EXTERN S16 wrEmmMmeBldEnbCfgUpd(S1apPdu **pdu);
EXTERN S16 wrEmmMmeBldEnbCfgTrnsfr(S1apPdu **pdu, 
                                   WrEmmMmeEnbCfgTrnsfr *trnsfrInfo);
EXTERN S16 wrEmmNghEnbSndResetReq(U32  peerId,
                                  WrUmmMsgCause *cause);
EXTERN S16 wrGenerateAddNeighCfgReq(LwrMngmt *cfm);
EXTERN S16 wrEmmMmeEnbCfgTransferRspTmrExpiry(WrNeighEnbCb *nghEnbCb);
EXTERN Void wrEmmMmeDown(WrMmeCb *mmeCb);
EXTERN S16 wrEmmMmeSendResetReq(U32 mmeId,U8 causeType,
               U32 cause,U16 numCons,U8 rstType,
               WrEmmS1ConId *conIds);
EXTERN S16 wrEmmMmeBldResetReq(S1apPdu **pdu,U8 rstType,U16 numCons,U8 choice,U32 cause,WrEmmS1ConId *conIds);
EXTERN S16 wrEmmMmeSendCfgUpdateFail(WrMmeId mmeId,WrUmmMsgCause *cause,U8 timeToWaitSecs);
EXTERN S16 wrEmmMmeBldEnbCfgUpdFail(S1apPdu **pdu,WrUmmMsgCause *cause,U8 timeToWaitSecs);
EXTERN S16 wrEmmMmeSendEnbErrInd(WrMmeId mmeId,WrUmmMsgCause *cause,U32 *eNbUeId,U32 *mmeUeId);
EXTERN S16 wrEmmMmeBldEnbErrInd(S1apPdu **pdu,WrUmmMsgCause *cause,U32 *eNbUeId,U32 *mmeUeId);
EXTERN S16 wrEmmMmeParsePlmnIe(SztPLMNidentity *plmnIe,WrPlmnId *plmnId);
EXTERN S16 wrEmmMmeEnbCfgUpdWaitTmrExp(U32 mmeId);
EXTERN S16 wrEmmMmeEnbCfgRspTmrExp(U32 mmeId);
EXTERN S16 wrEmmMmeResetRspTmrExp(U32 mmeId);
EXTERN S16 wrEmmGetOvldStatus(U32 mmeId);
EXTERN Bool wrEmmGetRelCap(U32 mmeId);
EXTERN Bool wrEmmGetMmeStatus(U32 mmeId);
EXTERN S16 wrEmmMmePrcErrInd(U32 peerId,S1apPdu *pdu);
EXTERN S16 wrEmmMmePrcS1Abort(SztAbortS1 *abortS1);
EXTERN S16 wrEmmMmeEnbRst(WrCellCb *cellCb);
EXTERN S16 wrEmmMmePeerRestart(WrCellId cellId);
EXTERN S16 wrEmmMmeTstEnbRst(U8 rstType);
EXTERN S16 wrEmmMmeBrngAllS1Down(Void);
PUBLIC S16 wrEmmMmeEnbConfigTransfer(WrEmmMmeEnbCfgTrnsfr *trnsfrInfo,
                               U32 enbCfgTransTmrVal,WrNeighEnbCb *neighEnbCb);
EXTERN S16 wrEmmMmeSztFillLAC(U16 lac, SztLAC *lAC);
EXTERN S16 wrEmmMmeSztFillTAC(U16 tac, S1apPdu *pdu, SztTAC *tAC);
EXTERN S16 wrEmmMmeSztFillPLMNId (S1apPdu *pdu, WrPlmnId *plmnId, SztPLMNidentity *plmnIe);
EXTERN S16 wrEmmGetLdBalMme(WrRrcConTransCb  *rrccon);
EXTERN S16 wrEmmGetMmeFrmStmsi(U8 mmeCode,U8 *causeTauRequiredSet,
                               U32 establishCause,WrRrcConTransCb *rrccon);
EXTERN S16 wrEmmMmeBrngCellPwrUp(Void);


EXTERN U32 wrEmmGetGummeiOvldStatus(WrGummei *gummei, WrMmeId mmeId);

EXTERN S16 wrEmmMmeSztFillCI(U16 inCi,SztCI *outCi);
#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* End of __WR_EMM_MME_H__ */

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
