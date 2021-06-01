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
               

     File:     wr_umm_trans.h

     Sid:      $SID$ 

     Prg:      Sriky 

*********************************************************************21*/
#ifndef __WR_UMM_TRANS_H__
#define __WR_UMM_TRANS_H__

#ifdef __cplusplus
extern "C" {
#endif /* end of __cplusplus */

#include "wr_cmn.h"
#include "wr_umm_msg.h"
#include "wr_umm.h"
#include "wr_umm_rrccon.h"
#include "wr_umm_rabsetup.h"
#include "wr_umm_rabmod.h"
#include "wr_umm_rabrel.h"
#include "wr_umm_lcrabrel.h"
#include "wr_umm_initcntsetup.h"
#include "wr_umm_ctxtrel.h"
#include "wr_umm_rrcreestab.h"
#include "wr_umm_meas.h"
#include "wr_umm_ctxtmod.h"
#include "wr_umm_dlnas.h"
#include "wr_umm_s1srcho.h"
#include "wr_umm_x2tgtho.h"
#include "wr_umm_x2srcho.h"
#include "wr_umm_s1tgtho.h"

/*eCSFB added for eCSFB */
#include "wr_umm_dlcdma.h"
#include "wr_umm_ecsfb.h"
#include "wr_umm_lteadv.h"

/* The below definitions are for return values for incoming transaction   */
/* processing. Each existing transaction my return any of the following   */
/* values. If the value is anything other than WR_UMM_TRANS_OK, the       */
/* processing of incoming transaction is aborted.                         */
#define WR_UMM_TRANS_OK                0
#define WR_UMM_TRANS_FAIL              1
#define WR_UMM_TRANS_DEFER_ONGOING     2
#define WR_UMM_TRANS_DEFER_INCOMING    3


/* The following macros are used to manage the state of configuration     */
/* towards each layer. Transactions may maintain state variables for each */
/* configuration managed by them.                                         */
/* WR_UMM_CFG_IDLE - No request has been initated for the configuration   */
/* WR_UMM_CFG_SENT - Request has been dispatched and response is awaited  */
/* WR_UMM_CFG_SUCC - The configuration has been successfully completed    */
/* WR_UMM_CFG_FAIL - The configuration has failed.                        */
/* WR_UMM_CFG_ERRO - Some internal error happened while trying to         */
/*                   configure the lower layer.                           */
#define WR_UMM_CFG_IDLE                     0
#define WR_UMM_CFG_SENT                     1
#define WR_UMM_CFG_SUCC                     2
#define WR_UMM_CFG_FAIL                     3
#define WR_UMM_CFG_ERRO                     4

typedef enum enTransactionType {
	WR_UMM_RRC_SETUP_TRANS=0,
	WR_UMM_INIT_CTXT_TRANS, 
	WR_UMM_ERAB_SETUP_TRANS,
	WR_UMM_UL_NAS_TRANS,   
	WR_UMM_DL_NAS_TRANS, 
	WR_UMM_LC_RAB_RLS_TRANS,
	WR_UMM_RRC_REESTAB_TRANS,
	WR_UMM_CTXT_REL_TRANS, 
	WR_UMM_RAB_REL_TRANS, 
	WR_UMM_ERAB_MOD_TRANS,
	WR_UMM_UE_CTXT_MOD_TRANS,
	WR_UMM_MEAS_TRANS,   
	WR_UMM_S1_SRC_HO_TRANS,
	WR_UMM_S1_TGT_HO_TRANS,
	WR_UMM_X2_SRC_HO_TRANS,
	WR_UMM_X2_TGT_HO_TRANS,
	WR_UMM_CSFB_REDIR_TRANS,
   WR_UMM_ECSFB_TRANS,
   WR_UMM_DL_CDMA_TRANS,
	WR_UMM_RAD_RES_MOD_TRANS,
	WR_UMM_LOCATION_REPORT_TRANS,
	WR_UMM_ANR_MEAS_TRANS,
   WR_UMM_EVENT_ANR_TRANS,
   WR_UMM_RRC_CSFB_TRANS,
   WR_UMM_LTE_ADV_TRANS,
   WR_UMM_CSFB_CCO_TRANS,
   WR_UMM_TX_MODE_CHNG_TRANS,
   WR_UMM_PERD_STRONG_CELL_MEAS_TRANS,
	WR_UMM_TRANS_UKNW,         
	WR_UMM_MAX_TRANS_TYPE
} enUmmTransactionType;

#define WR_CREATE_NEW_TRANS                 0
/* The below definitions are for transaction states for incoming message  */
/* processing.Necessary state need to be enhanced based on diff features  */

typedef enum 
{
   WR_UMM_TRANS_INUSE=0,
   WR_UMM_TRANS_DONE,
   WR_UMM_TRANS_STATE_ROLLBACK,
   WR_UMM_TRANS_CALL_DONE,
   WR_UMM_TRANS_DELETE_UE,
   WR_UMM_TRANS_DEFFERED,
   WR_UMM_TRANS_RELEASE_CALL
} enUmmTransactionState;

/* SPS changes starts */
typedef struct {
   Bool isUeUlSpsEnabled;
   Bool isUeDlSpsEnabled;
   WrSpsCfg spsCfg;
} WrSpsCfgTransCb;
/* SPS changes ends */

/* Macros for bitmap Value for the Different Physical config dedicated IE's 
 * This macros are used to map teh resource type reconfigured during the 
 * RRM Ue Recfg Req
 */
#define WR_UMM_PHY_CFG_DED_SR_IE           1
#define WR_UMM_PHY_CFG_DED_CQI_IE          2
#define WR_UMM_PHY_CFG_DED_SRS_IE          4

/** @brief This structure holds the UE specific transaction 
           Control block
  -  CmLList      lnk     This parameter specifies that this structure
                          is a node in the Transaction control block linked list.
  -  U32         transId  This parameter specifies the transaction Identity. This
                          uniquely identifies the transaction.
  -  U8          transTyp This parameter specifies the type of the transaction
  -  U8          state    This parameter specifies the state of the transaction
  - WrUmmIncMsg  *msg     This parameter holds the address of the incoming message
                          to the transaction.
  - U32          event    This parameter specifies the event received for the 
                          transaction
  -  WrUeCb      *ueCb    This parameter holds the address of the Ue control block
                          of the current transaction.
  -  union
  -  {
  -     WrRrcConTransCb   rrcContranscb  This parameter refers to the Transaction control
                                       information specific to RRC Connection Setup procedure
  -     WrUmmRabSetupTrans rabSetupTrans This parameter refers to the Transaction control
                                       information specific to E-RAB Setup procedure
  -     WrInitCntxtSetupTransCb initCtxtCb This parameter refers to the Transaction control
                                       information specific to Initial Context Setup 
                                       procedure
  -     WrUmmRabRelTransCb rabRelTransCb This parameter refers to the Transaction control
                                       information specific to E-RAB Release procedure
  -     WrUmmLcRabRelTransCb lcRabRelTransCb This parameter refers to the Transaction control
                                       information specific to Local RAB Release procedure
  -     WrUmmCtxtRelTrans    ctxtRelTrans This parameter refers to the Transaction control
                                       information specific to Context Release procedure
  -     WrRrcReEstabTransCb  rrcReEstabTransCb This parameter refers to the Transaction control
                                       information specific to RRC Reestablishment procedure
  -     WrUmmRabModifyTransCb rabModifyTransCb This parameter refers to the Transaction control
                                       information specific to E-RAB Modify procedure
  -     WrUmmCtxtModTrans   ctxtModTrans This parameter refers to the Transaction control
                                       information specific to UE Context Modification procedure
      WrUmmDlNasTransCb     dlNasTrans This transaction refers to the Transaction Control
                                       information specific to DL NAS delivery
  -  } u;

*/
typedef struct wrUmmTransCb
{
   CmLList                   lnk;
   WrUmmTimer                timer;
   WrUmmTimer                timerGrd;
   CmLListCp                 tmrLst;
   CmLListCp                 deferredTrans;
   U32                       transId;
   U8                        transTyp;
   U8                        state;
   U8                       errorCause;
   WrUmmIncMsg               *msg;
   U32                       event;
   WrUeCb                    *ueCb;
   U16                       numDefMsgCount;
   U16                   transRefCount;
   WrUmmIncMsg               *defList[WR_UMM_DEF_LIST];
   /* SPS changes starts */
   WrSpsCfgTransCb           spsCfgTransCb;
   /* SPS changes ends */
   union
   {
      WrRrcConTransCb           rrcContranscb;
      WrUmmRabSetupTrans        rabSetupTrans;
      WrInitCntxtSetupTransCb   initCtxtCb;
      WrUmmRabRelTransCb        rabRelTransCb;
      WrUmmLcRabRelTransCb      lcRabRelTransCb;
      WrUmmCtxtRelTrans         ctxtRelTrans;
      WrRrcReEstabTransCb       rrcReEstabTransCb;
      WrUmmRabModifyTransCb     rabModifyTransCb;
      WrUmmCtxtModTrans         ctxtModTrans;
      WrUmmDlNasTransCb         dlNasTrans;
      WrUmmMeasTransInfo        measTransCb;
      WrUmmS1SrcHoTrans         s1SrcHoTrans;
      WrUmmS1TgtHoTrans         s1TgtHoTrans;
      WrUmmX2TgtHoTrans         x2TgtHoTrans;
      WrUmmX2SrcHoTrans         x2SrcHoTrans;
      WrUmmCsfbTransCb          csfbTrans;
      WrUmmCsfbCcoTransCb       csfbGeranTrans;
      WrUmmEcsfbTransCb         ecsfbTransCb;
      WrUmmAnrMeasTransInfo     anrMeasTrans;
      WrUmmEventAnrTransInfo    eventAnrTrans;
      WrUmmRadResMod            radResMod;
      WrUmmDlCdmaTransCb         dlCdmaTrans;
      WrUmmTxModeChngTransCb    txModeChngTrans;
      WrLteAdvTransCb           lteAdvTransCb;
      WrUmmPerdStrongCellMeasTransInfo     perdStrongCellMeasTrans;
   } u;
} WrUmmTransCb;

EXTERN S16 wrUmmCsfbRelUe(WrUmmTransCb     *transCb);

#define WR_GET_CELL_ID_FRM_TRANSID(_transId) (((_transId) & 0x00f00000) >> 20)
#define WR_GET_UE_ID_FRM_TRANSID(_transId)   (((_transId) & 0x000ff000) >> 12)
#define WR_GET_TRANS_FRM_TRANSID(_transId)   (((_transId) & 0x00000ff0) >> 4)
#define WR_SET_TRANS_ID(_transId, _cellId, _ueId, _trans) \
                            _transId = (((_cellId & 0x0000000f) << 20) |\
                                        ((_ueId   & 0x000000ff) << 12) |\
                                        ((_trans  & 0x000000ff) << 4))

EXTERN Void wrUmmTransComplete(WrUmmTransCb *transCb);
typedef S16 (*WrUmmTransProcessor)(WrUmmTransCb *transCb);
typedef S16 (*WrUmmTransMsgPrcr)(WrUmmTransCb *transCb,WrUmmTransCb *defTransCb);
typedef S16 (*WrUmmTransAllowMsg)(WrUmmTransCb *tranCb, WrUmmIncMsg *msg);
typedef S16 (*WrUmmStopTransPrcr)(WrUmmTransCb *tranCb, WrUmmTransCb *defTrans);
typedef S16 (*WrUmmNewTransPrcr)(WrUmmTransCb *tranCb, WrUmmTransCb *incTrans);
typedef S16 (*WrUmmTransRelease)(WrUmmTransCb *tranCb);
EXTERN void wrUmmRlsIncMsg(WrUmmIncMsg **msg);
EXTERN S16 wrUmmS1GetTransIdFromMsg(WrUeCb *ueCb, WrUmmIncMsg *s1Pdu, 
                                                               U32 *transId);
EXTERN S16 wrUmmUuGetTransIdFromMsg(WrUeCb *ueCb, WrUmmIncMsg *uuPdu, 
                                                               U32 *transId);

/* The list of functions needed by transaction control block              */
EXTERN S16 wrUmmRabSetupProcessor(WrUmmTransCb *transCb);
EXTERN S16 wrUmmRabModifyProcessor(WrUmmTransCb *transCb);
EXTERN S16 wrUmmInitCtxtProcessor(WrUmmTransCb  *transCb);
EXTERN S16 wrUmmUlNasProcessor(WrUmmTransCb  *transCb);
EXTERN S16 wrUmmDlNasProcessor(WrUmmTransCb  *transCb);
EXTERN S16 wrUmmCtxtRelProcessor(WrUmmTransCb *transCb);
EXTERN S16 wrUmmLcRabRelProcessor(WrUmmTransCb  *transCb);
EXTERN S16 wrUmmRabRelProcessor (WrUmmTransCb  *transCb);
EXTERN S16 wrUmmRrcSetupProcessor (WrUmmTransCb  *transCb);
EXTERN S16 wrUmmRrcReEstabTransProcessor(WrUmmTransCb *transCb);
EXTERN S16 wrUmmX2TgtHoProcessor(WrUmmTransCb *transCb);
EXTERN S16 wrUmmX2SrcHoProcessor(WrUmmTransCb *transCb);
EXTERN S16 wrUmmS1TgtHoProcessor(WrUmmTransCb *transCb);
EXTERN S16 wrUmmS1SrcHoProcessor(WrUmmTransCb *transCb);
EXTERN S16 wrUmmCsfbProcessor(WrUmmTransCb *transCb);
EXTERN S16 wrUmmRrcCsfbProcessor(WrUmmTransCb *transCb);
/* eCSFB added for Ecsfb */
EXTERN S16 wrUmmEcsfbProcessor(WrUmmTransCb *transCb);
EXTERN S16 wrUmmLocReptProcessor(WrUmmTransCb *transCb);

EXTERN S16 wrUmmMeasTransRel(WrUmmTransCb *measTransCb);
EXTERN S16 wrUmmAnrMeasTransRel(WrUmmTransCb *measTransCb);
EXTERN S16 wrUmmEventAnrTransRel(WrUmmTransCb *measTransCb);
EXTERN S16 wrUmmPerdStrongCellMeasTransRel(WrUmmTransCb *measTransCb);

/* eCSFB - Added for eCSFB */
EXTERN S16 wrUmmDlCdmaTransRel(WrUmmTransCb *transCb);
EXTERN S16 wrUmmTxModeChngTransRel(WrUmmTransCb *transCb);

EXTERN S16 wrUmmRrcConTransRel(WrUmmTransCb *transCb);
EXTERN S16 wrUmmInitCtxtTransRel(WrUmmTransCb *transCb);
EXTERN S16 wrUmmRabSetupTransRel(WrUmmTransCb *transCb);
EXTERN S16 wrUmmRabModifyTransRel(WrUmmTransCb *transCb);
EXTERN S16 wrUmmUlNasTransRel(WrUmmTransCb *transCb);
EXTERN S16 wrUmmDlNasTransRel(WrUmmTransCb *transCb);
EXTERN S16 wrUmmCtxtRelTransRel(WrUmmTransCb *transCb);
EXTERN S16 wrUmmRabRelTransRel(WrUmmTransCb *transCb);
EXTERN S16 wrUmmLcRabRelTransRel(WrUmmTransCb *transCb);
EXTERN S16 wrUmmRrcReestabTransRel(WrUmmTransCb *transCb);
EXTERN S16 wrUmmCtxtModTransRel(WrUmmTransCb *transCb);
/* warning fix */
EXTERN S16 wrUmmCtxtModSndCtxtModRsp(WrUmmTransCb *transCb);
EXTERN S16 wrUmmS1SrcHoTransRel(WrUmmTransCb *transCb);
EXTERN S16 wrUmmS1TgtHoTransRel(WrUmmTransCb *transCb);
EXTERN S16 wrUmmX2SrcHoTransRel(WrUmmTransCb *transCb);
EXTERN S16 wrUmmX2TgtHoTransRel(WrUmmTransCb *transCb);
EXTERN S16 wrUmmPrcTgtEnbRelocTmrExpiry(WrUeCb *ueCb); 
EXTERN S16 wrUmmCsfbTransRel(WrUmmTransCb *transCb);
EXTERN S16 wrUmmRrcCsfbTransRel(WrUmmTransCb *transCb);
/* eCSFB added for Ecsfb */
EXTERN S16 wrUmmEcsfbTransRel(WrUmmTransCb *transCb);
EXTERN S16 wrUmmLocReptTransRel (WrUmmTransCb *transCb);
EXTERN S16 wrUmmDefIncTransRel(WrUmmTransCb *transCb);
EXTERN S16 wrUmmDefIncNewTrans(WrUmmTransCb *transCb, WrUmmTransCb *defTransCb);
EXTERN S16 wrUmmDefIncTransMsg(WrUmmTransCb *transCb, WrUmmTransCb *defTransCb);
EXTERN S16 wrUmmDefIncStopTrans(WrUmmTransCb *transCb, WrUmmTransCb *defTransCb);
EXTERN Void wrUmmBldSndLocRept (WrUmmTransCb *transCb);

/* The functions that yeild transaction type based on the message         */
EXTERN S16 wrUmmGetUuTransType(WrUeCb *ueCb, WrUmmIncMsg *msg, U32 *transTyp);
EXTERN S16 wrUmmGetS1TransType(S1apPdu *msg, U32 *transTyp);

/* The functions that yield the transaction value based on message        */
EXTERN S16 wrUmmS1GetTransIdFromMsg(WrUeCb *ueCb, WrUmmIncMsg *msg, 
                                    U32 *transId);
EXTERN S16 wrUmmMeasProcessor (WrUmmTransCb  *transCb);
EXTERN S16 wrUmmAnrMeasProcessor (WrUmmTransCb  *transCb);
EXTERN S16 wrUmmEventAnrProcessor (WrUmmTransCb  *transCb);
EXTERN S16 wrUmmPerdStrongCellMeasProcessor (WrUmmTransCb  *transCb);

/* eCSFB - Added for eCSFB */
EXTERN S16 wrUmmDlCdmaProcessor (WrUmmTransCb  *transCb);

EXTERN S16 wrUmmGetX2TransType(WrUmmIncMsg *msg, U32 *transTyp);
EXTERN S16 wrUmmX2GetTransIdFromMsg(WrUeCb  *ueCb, WrUmmIncMsg *msg,
                                    U32 *transId);

PUBLIC S16 wrUmmCtxtModProcessor(WrUmmTransCb *transCb);
/* The list of functions needed by transaction control block    and          */
/* new incoming transaction control block                                    */
EXTERN  S16  wrUmmDefIncTrans (WrUmmTransCb *tranCb);
EXTERN  S16  wrUmmIncNewTransProcessor(WrUmmTransCb *tranCb,
      WrUmmTransCb *defTransCb);
EXTERN  S16  wrUmmRabSetupNewTransProcessor(WrUmmTransCb *tranCb, 
                                          WrUmmTransCb *incTrans);
EXTERN  S16  wrUmmRabRelNewTransProcessor(WrUmmTransCb *tranCb, 
                                          WrUmmTransCb *incTrans);
EXTERN  S16 wrUmmRabModNewTransProcessor(WrUmmTransCb *tranCb, 
                                          WrUmmTransCb *incTrans);
EXTERN  S16 wrUmmRrcConnNewTransProcessor(WrUmmTransCb *transCb,
                                          WrUmmTransCb  *incTrans);

EXTERN  S16  wrUmmInitCtxtNewTransProcessor(WrUmmTransCb *tranCb, 
                                          WrUmmTransCb *incTrans);
EXTERN  S16  wrUmmReestabNewTransProcessor(WrUmmTransCb *tranCb, 
                                          WrUmmTransCb *incTrans);
EXTERN S16 wrUmmCtxtRelNewTransProcessor(WrUmmTransCb *transCb,
                WrUmmTransCb *incTrans);
EXTERN  S16  wrUmmCsfbPrcDeferTrans(WrUmmTransCb *tranCb, 
                                          WrUmmTransCb *deferTrans);
EXTERN  S16  wrUmmRrcCsfbPrcDeferTrans(WrUmmTransCb *tranCb, 
                                          WrUmmTransCb *deferTrans);
EXTERN  S16  wrUmmEcsfbPrcDeferTrans(WrUmmTransCb *tranCb, 
                                          WrUmmTransCb *deferTrans);
EXTERN  S16  wrUmmDlCdmaPrcDeferTrans(WrUmmTransCb *tranCb, 
                                          WrUmmTransCb *deferTrans);
EXTERN  S16  wrUmmRrcCsfbNewTransProcessor(WrUmmTransCb *tranCb, 
                                          WrUmmTransCb *incTrans);
/* eCSFB added for Ecsfb */
EXTERN  S16  wrUmmCsfbNewTransProcessor(WrUmmTransCb *tranCb, 
                                          WrUmmTransCb *incTrans);
/* eCSFB added for Ecsfb */
EXTERN S16   wrUmmEcsfbNewTransProcessor(WrUmmTransCb *transCb,
                                WrUmmTransCb *incTrans);
EXTERN  S16  wrUmmEventAnrNewTransProcessor(WrUmmTransCb *tranCb, 
            WrUmmTransCb *incTrans);

/* eCSFB - Added for eCSFB */
EXTERN  S16  wrUmmDlCdmaNewTransProcessor(WrUmmTransCb *tranCb,
            WrUmmTransCb *incTrans);

EXTERN  S16  wrUmmAnrMeasNewTransProcessor(WrUmmTransCb *tranCb, 
            WrUmmTransCb *incTrans);
EXTERN  S16  wrUmmMeasNewTransProcessor(WrUmmTransCb *tranCb, 
            WrUmmTransCb *incTrans);
EXTERN  S16  wrUmmPerdStrongCellMeasNewTransProcessor(WrUmmTransCb *tranCb, 
            WrUmmTransCb *incTrans);

EXTERN  S16  wrUmmDlNasNewTransProcessor(WrUmmTransCb *tranCb, 
                                         WrUmmTransCb *incTrans);

EXTERN  S16  wrUmmStopReEstabTransPrcs(WrUmmTransCb *tranCb, 
                                          WrUmmTransCb *deferTrans);
EXTERN S16 wrUmmRadResConfModProcessor(WrUmmTransCb *transCb);
EXTERN S16 wrUmmRadResConfModTransRel(WrUmmTransCb *transCb);
EXTERN S16 wrUmmCqiReptTmrExp(WrUeCb    *ueCb);
EXTERN S16 wrUmmRadResModNewTransProcessor(WrUmmTransCb *transCb,
                                      WrUmmTransCb *incTransCb);
EXTERN S16 wrUmmIncTxModeChngNewTransProcessor(WrUmmTransCb *transCb,
                                      WrUmmTransCb *incTransCb);
EXTERN S16 wrUmmTxModeChngProcessor(WrUmmTransCb *transCb);
/* HO_REEST */
EXTERN  S16  wrUmmS1SrcHoNewTransProcessor(WrUmmTransCb *tranCb, 
      WrUmmTransCb *incTrans );
EXTERN  S16  wrUmmX2SrcHoNewTransProcessor(WrUmmTransCb *tranCb, 
      WrUmmTransCb *incTrans);
EXTERN  S16  wrUmmS1TgtHoNewTransProcessor(WrUmmTransCb *tranCb,
      WrUmmTransCb *incTrans);
EXTERN  S16  wrUmmX2TgtHoNewTransProcessor(WrUmmTransCb *tranCb,
      WrUmmTransCb *incTrans);
EXTERN  S16  wrUmmFwdUuIndPdu (WrUmmTransCb *transCb);
EXTERN  WrUmmTransCb*  wrUmmGetHoTransCb (WrUeCb *ueCb);

EXTERN S16  wrStartTransTmr(WrUmmTransCb *cb, S16 tmrEvnt, U32 delay);
EXTERN Void wrStopTransTmr(WrUmmTransCb *cb, S16 event);
EXTERN void wrUmmStopTransTimers(WrUmmTransCb *transCb);
EXTERN void wrUmmStopUeTimers(WrUeCb *cb);
EXTERN S16 wrUmmDefRabRelIncCtxtRelProcessor(WrUmmTransCb *transCb,
                WrUmmTransCb *incTrans);
EXTERN S16 wrUmmUtlUpdateSRBsInfo (WrUmmTransCb *transCb);
EXTERN S16 wrUmmRelCall (WrUmmTransCb *transCb);
EXTERN S16 wrUmmCreatCsfbTrans (WrUmmTransCb *transCb);
EXTERN S16 wrUmmStopTransaction (WrUmmTransCb *transCb);
EXTERN S16 wrUmmGetIntMsgTransType (WrUmmIncMsg *msg, U32 *transTyp);
EXTERN S16 wrUmmSchdUeStaInd (RgrUeStaIndInfo *ueStaInd);
EXTERN S16 wrUmmGetOnGoingRrcTransId
(
WrUeCb                       *ueCb,
U8                           dlDcchMsg,
U8                           transId,
U32                          *rrcTransId
);
EXTERN S16 wrUmmDefRabModIncHoProcessor
(
WrUmmTransCb                 *transCb,
WrUmmTransCb                 *incTrans
);
EXTERN S16 wrUmmDefRabModIncCtxtRelProcessor 
(
WrUmmTransCb                 *transCb,
WrUmmTransCb                 *incTrans
);
EXTERN S16 wrUmmDefRabModIncReestProcessor
(
WrUmmTransCb                 *transCb,
WrUmmTransCb                 *incTrans
);
EXTERN S16 wrUmmCtxtRelPrcTmrExpiry
(
WrUmmTransCb                 *transCb
);
EXTERN S16 wrUmmCreateIntCsfb
 (
   WrUmmCellCb               *cellCb,
   WrUeCb                    *ueCb,
   U8                        intMsgType
 );
EXTERN S16 wrUmmCsfbCdmaHdlMeasRpt
(
 WrUmmTransCb                       *ummTransCb,
 NhuMeasResults                     *measRpt,
 WrUmmMeasRptInfo                   *measRptInfo
);
EXTERN S16 wrUmmEcsfbCdmaHdlMeasRpt
(
 WrUmmTransCb                       *ummTransCb,
 NhuMeasResults                     *measRpt,
 WrUmmMeasRptInfo                   *measRptInfo
);
PUBLIC S16 wrUmmDefRabRelIncReestProcessor(WrUmmTransCb *transCb,
                      WrUmmTransCb *incTrans);
PUBLIC S16 wrUmmDefInitCtxtIncCtxtRelProcessor(WrUmmTransCb *transCb,
                WrUmmTransCb *incTrans);
PUBLIC S16 wrUmmDefInitCtxtIncReestProcessor(WrUmmTransCb *transCb,
                WrUmmTransCb *incTrans);
PUBLIC S16 wrUmmDefRrcConnIncReestProcessor
(
 WrUmmTransCb                *transCb,
 WrUmmTransCb                *incTrans
);

/* LTE_ADV starts */
EXTERN S16 wrUmmLteAdvProcessor (WrUmmTransCb *transCb);
EXTERN S16 wrUmmLteAdvTransRel (WrUmmTransCb *transCb);
/* LTE_ADV ends */
EXTERN S16 wrUmmRabSetupSndS1apRsp(WrUmmTransCb  *transCb);
EXTERN S16 wrUmmRabModSndS1apRsp(WrUmmTransCb *transCb);
EXTERN S16 wrUmmRabRelSndS1apRsp(WrUmmTransCb *transCb);
EXTERN  S16 wrUmmAddDefTransToUeTransLst(WrUmmTransCb *transCb, WrUmmTransCb *defTransCb);

EXTERN S16 wrUmmCsfbGeranCcoProcessor(WrUmmTransCb    *transCb);
EXTERN S16 wrUmmCsfbGeranCcoTransRel(WrUmmTransCb    *transCb);
EXTERN S16 wrUmmCsfbGeranCcoNewTransProcessor
(
 WrUmmTransCb    *transCb,
 WrUmmTransCb *incTrans
 );
EXTERN S16 wrUmmCsfbGeranFillArfcnSndRrcRelToUe
(
 WrUmmTransCb *transCb
);
EXTERN S16 wrUmmCsfbGeranBuildSndRedir
(
  WrUmmTransCb     *transCb
);
EXTERN S16 wrUmmCsfbGeranBldSendCco
(  
  WrUmmGeranCellCb        *geranCell,      
  WrUmmTransCb             *transCb        
);
EXTERN S16 wrUmmCsfbGeranSendCco
(
 WrUmmTransCb *transCb
);
EXTERN S16 wrUmmCsfbGeranPrcRrcMsg
(
 WrUmmTransCb    *transCb
);
EXTERN S16 wrUmmCsfbGeranSndRrcConRelMsg
(
  WrUmmTransCb                *transCb
);
EXTERN WrUmmRedirectInfo* wrUmmFillGeranRedirectInfo
(
    WrUmmTransCb             *transCb
);
EXTERN Bool wrFndCandidateNeighs
(
  WrUmmTransCb   *transCb
);

EXTERN WrUmmRedirectInfo* wrUmmFillRedirectInfo
(
  WrUmmTransCb              *transCb
);


EXTERN S16 wrUmmMeasGapSchdUeRecfgReq
(
 WrUmmTransCb                 *transCb
 );

/* warning fix */
EXTERN S16 wrUmmUeCapSndUeCapEnq(WrUmmTransCb    *transCb, WrUmmRatLst *ratLst);
EXTERN S16 wrUmmRrcDecReqHdl(WrUmmTransCb *transCb,TknStrOSXL *str,U16 sduType);
EXTERN S16 wrUmmUeCapHdlUeCapEncCfm(WrUeCb  *ueCb,NhuEncCfmSdus   *cfmSdu, U32 transId);
EXTERN S16 wrUmmUeCapHdlUeRaCapEncCfm(WrUeCb *ueCb, NhuEncCfmSdus    *cfmSdu);
EXTERN S16 wrUmmSndUeCapEncReq(WrUeCb  *ueCb,U32  transId,NhuUECapInfo  *ueCapInfo);
/* warning fix */
EXTERN S16 wrUmmHdlAndPrcRabMsg(WrUmmTransCb *incTrans, WrUmmMsgCause *cause);
EXTERN S16 wrUmmCsfbFillDfltArfcnSndRrcRelToUe ( WrUmmTransCb *transCb);

/* SPS changes starts */
PUBLIC Void wrUmmInitCtxtSchdUlSpsLchCfgReq(RmuUeRecfgRsp *rrmUeRecfgResp, U32 numErabs, Void *pErabList, RgrUeSpsCfg *ueSpsCfg);
PUBLIC Void wrUmmRabSetupSchdUlSpsLchCfgReq(RmuUeRecfgRsp *rrmUeRecfgResp, U32 numErabs, Void *pErabList, RgrUeSpsCfg *ueSpsCfg);
PUBLIC Void wrUmmRabRelSchdUlSpsLchCfgReq(RmuUeRecfgRsp *rrmUeRecfgResp, U32 numErabs, Void *pErabList, RgrUeSpsCfg *ueSpsCfg);
PUBLIC Void wrUmmRabModifySchdUlSpsLchCfgReq(RmuUeRecfgRsp *rrmUeRecfgResp, U32 numErabs, Void *pErabList, RgrUeSpsCfg *ueSpsCfg, WrUeCb *ueCb);
PUBLIC Void wrUmmS1TgtHoSchdUlSpsLchCfgReq(RmuUeRecfgRsp *rrmUeRecfgResp, U32 numErabs, Void *pErabList, RgrUeSpsCfg *ueSpsCfg);
PUBLIC Void wrUmmX2TgtHoSchdUlSpsLchCfgReq(RmuUeRecfgRsp *rrmUeRecfgResp, U32 numErabs, Void *pErabList, RgrUeSpsCfg *ueSpsCfg);
/* SPS changes ends */

EXTERN S16 wrUmmHoTgtPrcRrcDecCfm(WrUmmTransCb *transCb, WrUeCapInfo **ueCap);
EXTERN S16 wrUtlFillHoCmdTrnspCntrWthMobility
(
WrUmmTransCb                 *transCb,
NhuEncReqSdus                *dcchEncReq,
WrUmmMeasTransInfo           *measTransCb,
RmuUeHoRsp                   *rrmUeHoResp,
WrUmmHoSpsInfo               *srcToTgtSpsInfo,
WrSpsCfg                     *pSpsCfg,
Bool                         isUeUlSpsEnabled
);
/*DRX QCI*/
EXTERN S16 wrUmmCheckGBRBearer
(
WrUmmIncMsg                  *msg
);

EXTERN S16 wrRatSelectionAlgorithm (WrUmmTransCb	*transCb, wrCsfbRatType *ratType);

EXTERN Void wrUmmUpdateUeEmerState
(
WrUmmTransCb              *transCb,
WrUmmCellCb               *cellCb
);



#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* End of __WR_UMM_TRANS_H__ */

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
