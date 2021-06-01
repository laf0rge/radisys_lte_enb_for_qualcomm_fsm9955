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
               

     File:     wr_emm_neighenb.h

     Sid:      $SID$ 

     Prg:      Sriky 

*********************************************************************21*/

#ifndef __WR_EMM_NEIGHENB_H__
#define __WR_EMM_NEIGHENB_H__

#include "wr.h"
#include "wr_cmn.h"
#include "wr_emm.h"
#include "wr_ifm_s1ap.h"

#ifdef __cplusplus
extern "C" {
#endif /* end of __cplusplus */

/*Default value has been set to 6 sec */
#define WR_EMM_X2_SETUP_DFLT_TMR_VAL 20000
#define WR_EMM_X2_SETUP_FAIL_TMR_VAL 20000
#define WR_EMM_X2_SETUP_TIME_TO_WAIT 5
/* ccpu00125633 : wait Timer is changed from 1000 to 2000 since this value should be greater than
 *  sctp freeze timer for X2AP-Interface monitoring */
#define WR_ENB_CON_WAIT_TIMER        2000
#define WR_EMM_X2_RST_RETX_DFLT_TMR_VAL  1000
#define WR_EMM_X2_SETUP_MAX_RTX_VAL 0xFF 
#define WR_EMM_X2_RST_MAX_RTX_VAL 0xFF 
#define WR_EMM_X2_CFG_UPD_MAX_RTX_VAL 0xFF
#define WR_EMM_ENB_CFG_TRANSFER_RTX_CNT 0xFF

#define WR_EMM_X2_CFG_UPD_TMR_VAL      20000
#define WR_EMM_X2_CFG_UPD_FAIL_TMR_VAL 20000

#define WR_CZ_CAUSE_RADIONW      0
#define WR_CZ_CAUSE_TRANSPORT    1
#define WR_CZ_CAUSE_PROTOCOL     2
#define WR_CZ_CAUSE_MISC         3

#define WR_EMM_X2_CONFIG_EXTEN_IE_COUNT 2
#define WR_EMM_X2_CONFIG_FAIL_IE_COUNT 2
#define WR_EMM_X2_CONFIG_TIME_TO_WAIT 5
#define WR_EMM_X2_MACRO_ENBID_LEN 20
#define WR_EMM_X2_HOME_ENBID_LEN 28
#define WR_EMM_X2_NEIGH_EXTEN_IE_COUNT 1 /*!< Number members in neighbor list extension */

typedef struct ueX2apId
{
  Bool oldeNBUeX2apIdPres;
  U32  oldeNBUeX2apId;
  Bool neweNBUeX2apIdPres;
  U32  neweNBUeX2apId;
} UeX2apId;

#if 0
#define WR_GET_ENBID_FROM_PEERID(_eNbId,_peerId) {\
       _eNbId = wrEmmCb.x2ApEnbIdLst[_peerId];\
}

#define WR_GET_X2AP_PEER_ID(_peerId, eNBId, enbIdx) {\
        _peerId = enbIdx + 1; \
       wrEmmCb.x2ApEnbIdLst[_peerId] = eNBId;\
}
#endif

/*
 * Maximum no. of pools an eNB can belong to
*/

EXTERN S16 wrEmmNghEnbBldEBCfgUptReq(CztEvnt **pdu,WrEmmEnbCfgUpd *cellInfo);
EXTERN S16 wrEmmNghEnbSndEnCfgUpReq(WrNeighEnbCb *nghEnbCb,WrEmmEnbCfgUpd *cellInfo);
EXTERN S16 wrEmmNghEnbBldEBCfgUptReqFail(CztEvnt **pdu);
EXTERN S16 wrEmmNghEnbBldEBCfgUptReqAck(CztEvnt **pdu);
EXTERN S16 wrEmmNghEnbSndEnCfgUpReqAck(U32 peerId);
EXTERN S16 wrEmmNghEnbSndEnCfgUpReqFail(U32 peerId);
EXTERN S16 wrEmmNghEnbBldX2SetupReq(CztEvnt  **pdu);
EXTERN S16 wrEmmNghEnbSndX2SetupReq(WrNeighEnbCb *nghEnbCb);
EXTERN S16 wrEmmNghEnbSndX2Setup(WrNeighEnbCb  *nghEnbCb);
EXTERN S16 wrEmmNghEnbPrcStaInd(CztStaInd *cztSta);
EXTERN S16 wrEmmNghEnbSetupTmrExpiry(PTR);
EXTERN S16 wrEmmNghEnbSetupWaitTmrExpiry(PTR);
EXTERN S16 wrEmmNghEnbMsgTmrExpiry(PTR);
EXTERN S16 wrEmmNghEnodebIdCmp(WrNeighEnbCb *enbnghCb);
EXTERN S16 wrEmmNghEcgiCmp(WrEutranNeighCellCb *nghSrvdCellCb,WrNeighEnbCb *enbnghCb);
EXTERN S16 wrEmmBldEBCfgUptReq(CztEvnt  **pdu,WrEmmEnbCfgUpd *cellInfo);
EXTERN S16 wrEmmBldEBCfgUptReqFail(CztEvnt **pdu,Bool incTimeToWait);
EXTERN S16 wrEmmSndEnCfgUpReqFail(U32 peerId,Bool incTimeToWait);
EXTERN S16 wrEmmBldEBCfgUptReqAck(CztEvnt  **pdu);
EXTERN S16 wrEmmSndEnCfgUpReqAck(U32 peerId);
EXTERN S16 wrEmmNghEnbPrcRcvdPdu(U32 peerId,CztEvnt *pdu);
EXTERN S16 wrEmmNghTrigCfgUpd(WrCellCb *cellCb);
EXTERN S16 wrEmmNghTrigReset(Void );
EXTERN S16 wrEmmNghBrngAllX2Down(Void );

EXTERN U8 wrFillBwCfg(U8 dlBwIn);
EXTERN WrNrEutranFreqCb* wrEmmAnrGetEutranFreqNode(U16 earfcnDl,WrNrEutranCb *eutranCb);
EXTERN S16 wrEmmNghEnbParsePlmnIe(CztPLMN_Identity  *plmnIe, WrPlmnId  *plmnId); 
EXTERN S16 wrEmmNghEnbParseTknStr4Ie(TknStr4 *ie, U32 *lvalue);
EXTERN S16 wrEmmSendSetupReqNghEnbInfoToSon(WrCellCb  *cellCb, WrNeighEnbCb *enbNghCb,
           CztX2SetupRqst  *setupReq);
EXTERN S16 wrEmmSendSetupRspNghEnbInfoToSon(WrCellCb  *cellCb, WrNeighEnbCb  *enbNghCb,  
           CztX2SetupResp  *setupRsp);
EXTERN S16 wrEmmSendCfgUpdateNghEnbInfoToSon(WrCellCb *cellCb, WrNeighEnbCb  *enbNghCb,
           CztENBConfigUpd  *cfgUpdate);
EXTERN S16 wrEmmSendNghEnbDelToSon(WrCellCb *cellCb, WrNeighEnbCb *enbNghCb);
PUBLIC S16 wrEmmNghTrigCfgUpdNghCellMod
(
 WrCellCb  *cellCb,
 U32       enbId
);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* End of __WR_EMM_NEIGHENB_H__ */

/*********************************************************************31*
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
