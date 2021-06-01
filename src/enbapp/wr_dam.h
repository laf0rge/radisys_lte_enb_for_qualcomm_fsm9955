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
               

     File:     wr_dam.h

     Sid:      $SID$ 

     Prg:      Sriky 

*********************************************************************21*/
#ifndef __WR_DAM_H__
#define __WR_DAM_H__

#include "wr.h"
#include "wr_umm.h"
#include "wr_ifm_dam.h"

#ifdef __cplusplus
extern "C" {
#endif /* end of __cplusplus */

/** @def WR_DAM_MAX_CELLS 
 * This Macro defines the MAX cells supported 
 */
#define WR_DAM_MAX_CELLS                    2
 
/** @def WR_DAM_REESTABLISHMENT
 * This Macro defines that reestablishment is the cause for changing
 * the tunnel state
 */
#define WR_DAM_REESTABLISHMENT              1

/** @def WR_DAM_HO_END_MARKER_RCVD
 * This Macro defines that END Marker reception is the cause for changing
 * the tunnel state
 */
#define WR_DAM_HO_END_MARKER_RCVD           2

/** @def WR_DAM_HO_UE_UM_DAT_FWD
 * This Macro defines that data forwading  for state change is for a UM bearer
 * is needed.
 */
#define WR_DAM_HO_UE_UM_DAT_FWD             3
/* HO_REEST */
/** @def WR_DAM_SRC_HO_FAILURE 
 * This Macro defines that HO failure at src eNB due Re-establishment Or
 * any other reason
 */
#define WR_DAM_SRC_HO_FAILURE               4


/** @def WR_DAM_BUFFERING_PKT_LIMIT
 * This Macro defines the number of packets that can be buffered for a tunnel.
 */
#define WR_DAM_BUFFERING_PKT_LIMIT     (1024)

#define WR_DAM_TMR_END_MARKER_TMR              40
#define WR_DAM_TMR_INACTIVITY                    8
/* Maximum RABS is 32. 
 */ 
#define WR_DAM_MAX_DRBS     32


#define WR_DAM_MAX_SDU_CNT     8

#define  WR_EG_TEID_DATATYPE   4

#define  WR_EG_IE_TYPE_TEID     16

/* @def WR_DAM_CALC_TEID
* This macro calculates the TeId from cellId,ueId,rbId.
*/
#define WR_DAM_CALC_TEID(pdcpId,lclTeId,tnlType) \
{ \
    lclTeId  = ((pdcpId->cellId & 0x000000ff) << 24); \
    lclTeId |= ((pdcpId->ueId & 0x0000ffff) << 8); \
    lclTeId |= (pdcpId->rbId & 0x0000001F); \
    lclTeId |= (tnlType      & 0x00000007); \
}

/* ccpu00135472 start */
#define WR_GET_RBID_FROM_LOC_TEID(locTeId, rbId)  \
{\
   (rbId) = ((locTeId) & 0x000000f8) >> 3;\
}
/* ccpu00135472 end */

/* Data Structure for Data Forwarding and Buffering at DAM */
typedef struct wrDamEgtpDataBuf
{
   CmLList      lnk;         /* Link of Linked List */
   Buffer       *data;       /* Data Packet */
} WrDamEgtpDataBuf;

/** @def WR_DAM_TNL_FWD_STATE_INITIAL
 * This Macro defines that no forwarding needs to be done.
 */
#define WR_DAM_TNL_STATE_FWD_INITIAL      0
/** @def WR_DAM_TNL_FWD_STATE_OLD_PKTS
 * This Macro defines the forwarding of old packets from PDCP.
 */
#define WR_DAM_TNL_STATE_FWD_OLD_PKTS     1
/** @def WR_DAM_TNL_FWD_STATE_NEW_PKTS
 * This Macro defines the forwarding of new packets from SGW.
 */
#define WR_DAM_TNL_STATE_FWD_NEW_PKTS     2

#define WR_DAM_TNL_STATE_SOURCE_FWD_OLD_PKTS    3
 
#define WR_DAM_TNL_DL_FWDSTATE_NEW        3

/** 
 * @brief This structure contains the DAM tunnel information
 *
 * @details These are the structure members
 * - U32          lclTeId    local tunnel id
 * - U32          remTeid    remote tunnel id
 * - CmTptAddr    dstAddr    destination ip address
 * - CmTptAddr    lclAddr    source ip address
 * - U32          transId    transaction id (temporarily stores the id from 
 *                           application during add/delete/modify procedures)
 * - U8           tnlState   State of the tunnel
 * - U8           fwdState   data forwarding state of the tunnel
 * - CmLListCp    datBuffLst Linked list with data buffer (WrDamEgtpDataBuf)
 */
typedef struct wrDamTnlCb
{
   U32                       locTeId;
   U32                       sduIdCntr;
   U32                       remTeid;
   CmTptAddr                 dstAddr;
   CmTptAddr                 lclAddr;
   U32                       transId;
   U8                        tnlState;
   U8                        fwdState;
   U8                        dscp;
   CmLListCp                 datBuffLst;
   Bool                      isEndMarkRcv;
}WrDamTnlCb;

/** @def WR_DAM_TNL_STATE_NORMAL
 * This Macro defines the Normal state of the tunnel.
 */
#define WR_DAM_DRB_STATE_NORMAL           1
/** @def WR_DAM_DRB_STATE_BUFFERING
 * This Macro defines the Buffering state of the tunnel.
 */
#define WR_DAM_DRB_STATE_BUFFERING        2
/** @def WR_DAM_TNL_DL_FWDSTATE_NEW
 * This Macro defines the forwarding state of DL forwarding tunnel to 
 * forward new packets.
 */
#define WR_DAM_TNL_DL_FWDSTATE_NEW        3

/** 
 * @brief This structure contains the DAM DRB Information
 *
 * @details These are the structure members
 * -   CmLtePdcpId   pdcpId      PDCP Identity of the RAB
 * -   U32           sduIdCntr   Sdu Id Counter
 * -   WrDamTnlCb    *normalTnl  Normal Tunnel Control block
 * -   WrDamTnlCb    *dlFwdTnl   DL data forwarding tunnel Cb
 * -   WrDamTnlCb    *ulFwdTnl   UL data forwarding tunnel Cb
 */
typedef struct wrDamDrbCb
{
   CmLtePdcpId               pdcpId;
   U32                       sduIdCntr;
   U8                        tnlPriority;
   U8                        state;
   WrDamTnlCb                *normalTnl;
   WrDamTnlCb                *dlFwdTnl;
   WrDamTnlCb                *ulFwdTnl;
} WrDamDrbCb;

#define WR_DAM_UESTATE_NORMAL     0
#define WR_DAM_UESTATE_HO         1

/** 
 * @brief This structure contains the DAM UE Information
 *
 * @details These are the structure members
 * - CmTimer        inactivityTmr  inactivity timer structure
 * - CmTimer        endMrkTmr      end marker timer structure
 * - U16            ueId           unique UE ID
 * - U16            cellId         Cell ID in which UE is present
 * - U16            ueIdx          UE Index
 * - Bool           dataRcvd       flag to indicate if ue has been active
 * - U8             reestabInProgress Flag to indicate if UE is under
 *                                    reestablishment
 * - U8             ueState        State of the UE
 * - U32            numDrbs        number of created so far for the UE
 * - WrDamDrbCb     *drbs[WR_DAM_MAX_DRBS] Drb database
 * - Bool           isFullCfg      indicate the need for full configuration for the UE
 */
typedef struct wrDamUeCb
{
   CmTimer                   inactivityTmr;
   CmTimer                   endMrkTmr;
   U16                       ueId;
   U16                       cellId;
   U16                       ueIdx;
   U8                        dataRcvd;
   U8                        reestabInProgress;
   U8                        ueState;
   U32                       numTunnels;
   U32                       numDrbs;
/* Start - Fix for ccpu00123981 */
   U16                       expiryCnt;
/* End - Fix for ccpu00123981 */
   WrDamDrbCb                *drbs[WR_DAM_MAX_DRBS];
   U32                       ulTpt;
   U32                       dlTpt;
   WrHoType                  hoType;
   Bool                      isFullCfg; /* RRC-REL10-Upgrade */
} WrDamUeCb;

/** 
 * @brief This structure contains the DAM UE Index Information
 *
 * @details These are the structure members
 * - CmLlist        lnk            inactivity timer structure
 * - U16            ids            unique UE ID
 * - WrDamUeCb      *ueCb          tunnel data base
 */
typedef struct wrDamUeIdx
{
   CmLList                   lnk;
   U16                       idx;
   WrDamUeCb                 *ueCb;
} WrDamUeIdx;

/** 
 * @brief This structure contains the DAM UE Index data base
 *
 * @details These are the structure members
 * - CmLlistCp      freeLst        UE Index Free List
 * - CmLlistCp      inUseLst       UE Index In use List
 * - U16            numIdxs        Number of UE Indexes
 * - WrDamUeidx     *idxs          UE Indexes
 */
typedef struct wrDamUeIdxCp
{
   CmLListCp                freeLst;
   CmLListCp                inuseLst;
   U16                      numIdxs;
   WrDamUeIdx               *idxs;
} WrDamUeIdxCp;

/** 
 * @brief This structure contains the DAM Cell Information
 *
 * @details These are the structure members
 * - U8            cellInUse  flag to indicate if cell is operational
 *                            (initialized)
 * - U8            cellId     unique cell id
 * - U16           startRnti  starting range of UE IDs
 * - U16           numRntis   maximum UE IDs that can be used
 * - WrDamUeIdxCp  ueIdxCp    UE Index Data Base
 * - WrDamUeCb     **ueCbs    UE Data Base
 */
typedef struct wrDamCellCb
{
   U8                        cellInUse;
   U8                        cellId;
   U16                       startRnti;
   U16                       numRntis;
   WrDamUeIdxCp              ueIdxCp;
   WrDamUeCb                 **ueCbs;
} WrDamCellCb;

/** 
 * @brief This structure contains the DAM information.
 *
 * @details These are the structure members
 * - CmTqCp       tqCp                 timer queue for DAM
 * - CmTqType     tq[WR_NUM_TQENTRY]   timer queue type info
 * - CmTptAddr    srcAddr              eNodeB Address
 * - WrDamCellCb  cells[WR_DAM_MAX_CELLS]  cell data base
 * - U32          inactvTmrVal         ue inactivity timer value
 * - U32          endMrkTmrVal         ue end marker timer value
 * - Ticks        timeRes;             Time resolution
 */
typedef struct wrDamCb
{
   CmTqCp                    tqCp;
   CmTqType                  tq[WR_NUM_TQENTRY];
   TskInit                   init;
   CmTptAddr                 srcAddr;
   Pst                       wrAppPst;
   Pst                       wrDamToRrmPst;
   WrDamCellCb               cells[WR_DAM_MAX_CELLS];
   U32                       inactvTmrVal;
   U16                       expiryCnt;
   U32                       endMrkTmrVal;
   WrLiSapCb                 **egtSap;
   WrLiSapCb                 **pjuSap;
   Ticks                     timeRes; 

} WrDamCb;


#define WR_ALLOC_DATA_APP_WC(_buf, _size)         \
{\
   if (SGetSBuf(WR_DATAPP_MEM_REGION, WR_MEM_POOL, (Data **)(_buf), _size) != ROK)\
   {                                                         \
      WR_DIAG_LVL0(0x0ff00001, WR_DIAG_NA, SS_DIAG_INV_ARG,\
            "SSI memory allocation of %d \
            bytes %d failed in :%d",_size, __LINE__, 0, 0)   \
      (*_buf) = NULLP;                                       \
   }                                                         \
}


#define WR_ALLOC_DATA_APP(_buf, _size)         \
{\
   if (SGetSBuf(WR_DATAPP_MEM_REGION, WR_MEM_POOL, (Data **)(_buf), _size) == ROK)\
   {                                                         \
      WR_SET_ZERO((*_buf), _size);                           \
   }                                                         \
   else                                                      \
   {                                                         \
      WR_DIAG_LVL0(0x0ff00001, WR_DIAG_NA, SS_DIAG_INV_ARG,\
            "SSI memory allocation of %d \
            bytes %d failed in :%d",_size, __LINE__, 0, 0)   \
      (*_buf) = NULLP;                                       \
   }                                                         \
}

/* initialize the memCp & allocate memory for the event structure */
#define WR_ALLOCEVNT_DATA_APP(_evntPtr, _size)\
{\
    Mem _mem; \
   _mem.region = WR_DATAPP_MEM_REGION; \
   _mem.pool = 0; \
   (*_evntPtr) = NULLP; \
   if(cmAllocEvnt((Size) _size, WR_MEM_PDU_SIZE, &_mem,   \
                     (Ptr *)(_evntPtr)) != ROK)  \
   {                                            \
      RETVALUE(RFAILED);                         \
   }                         \
}



#define WR_DATA_APP_FREEMBUF(_mBuf)                            \
do{                                                   \
   if (_mBuf != NULLP)                                \
   {                                                  \
      (Void)SPutMsg((_mBuf));                            \
      (_mBuf) = NULLP;                                   \
   }                                                  \
}while(0)


#define WR_FREE_DATA_APP(_buf, _size)   \
{\
   (Void) SPutSBuf(wrDamCb.init.region, WR_MEM_POOL, (Data *) _buf, (Size) _size); \
   (_buf) = NULLP;\
}

#ifdef SS_LOCKLESS_MEMORY
#define WR_FREE_DATA_APP_SHAREBLE_BUF(_buf, _size)   \
{\
   (Void) SPutStaticBuffer(wrDamCb.init.region, WR_MEM_POOL, (Data *) _buf, \
                           (Size) _size, SS_SHARABLE_MEMORY); \
   (_buf) = NULLP;\
}
#else
#define WR_FREE_DATA_APP_SHAREBLE_BUF(_buf, _size)   \
{\
      (Void) SPutSBuf(wrDamCb.init.region, WR_MEM_POOL, (Data *) _buf, (Size) _size); \
      (_buf) = NULLP;\
}

#endif

EXTERN WrDamCb               wrDamCb;

//EXTERN S16 wrDamTnlCreatReq(U32 transId, WrDamTnlInfo *tnlInfo);
//EXTERN S16 wrDamTnlDelReq(U32 transId, WrDamTnlId *tnlId);
//EXTERN S16 wrDamCellAddReq(U32 transId, WrDamCellInfo *cellInfo);
/* FIX for - ccpu00127682 (MEM_LEAK) */
#define WR_GET_DAM_CELLCB(_cellCb, _cellId){\
   U32 _idx;\
   _cellCb = NULLP;\
   for(_idx = 0; _idx < WR_DAM_MAX_CELLS; _idx++) {\
      if (wrDamCb.cells[idx].ueCbs != NULLP) {\
         if (wrDamCb.cells[_idx].cellId == _cellId) {\
            _cellCb = &wrDamCb.cells[_idx];\
         }\
      }\
   }\
}

/* @def WR_DAM_CALC_WAIT_TIME 
* This macro calculates the time.
*/
#define WR_DAM_CALC_WAIT_TIME(_delay) (_delay/((1000/SS_TICKS_SEC) * wrDamCb.timeRes))

#ifdef FLAT_BUFFER_OPT 
/* FIX for - ccpu00127682 (MEM_LEAK) */
EXTERN S16 wrDamEgtpDatIndFB
(
 Pst                          *pst,
 FlatBuffer                     *eguMsg,
 U32 tnlId
 );
EXTERN S16 WrIfmPdcpDatReqFB
(
 CmLtePdcpId                  *pdcpId,
 PjuSduId                     sduId,
 U8                           priority,
 FlatBuffer                   *mBuf
 );
#endif

EXTERN S16 wrDamHndlFwdPkts(Buffer            *mBuf,
                            WrDamEgtpDataBuf  *egtpBufInfo,
                            WrDamDrbCb  *drbCb,
                            EgtUEvnt    *eguEvtMsg,
                            Bool        pktDiscard,
                            U8          msgType);

EXTERN S16 wrDamCfgReq(LwrMngmt      *cfg);
EXTERN S16 wrDamCntrlReq(LwrMngmt    *cntrl);

EXTERN S16 wrDamStartUeInactvTmr(WrDamUeCb *ueCb);
/* RLC_DL_MAX_RETX fix */
EXTERN S16 wrDamPdcpDatCfm(CmLtePdcpId pdcpId);
EXTERN S16 wrDamHdlTmrExp(S16 event, WrDamUeCb *ueCb);
EXTERN U32 wrDamGetActiveUeCnt(U32 cellId);
EXTERN Void  wrDamReCfgReq(U32 transId,U16 cellId,U16 crnti);
EXTERN S16 wrDamReestabReq(U32 transId,U16 cellId,U16 oldCrnti,U16 newCrnti);
EXTERN Void  wrDamPrintThroughput(Void);

EXTERN S16 wrDamDeInit(void);

/* Timer Functions */
EXTERN S16  wrDamRegInitTmr(Void);
EXTERN S16  wrDamDeRegTmr(Void);
EXTERN S16  wrDamActvTmr (Ent ent, Inst inst);
EXTERN S16  wrDamStartTmr(PTR cb, S16 tmrEvnt, U32 delay);
EXTERN Void wrDamStopTmr(PTR cb, S16 event);
EXTERN  WrDamCellCb *wrDamGetCell (U16 cellId);
EXTERN S16 wrIfmDamEgtpErrInd(U32 lclTeid, U32 numDrbs, U16 crnti);
EXTERN Void wrDamSetDataRcvdFlag (U16 cellId, U16 ueId);
EXTERN S16 wrDamHdlDlPkts (EgUMsgHdr *msgHdr, WrDamDrbCb *drbCb, WrDamTnlCb *tnlCb, WrDamUeCb *ueCb, Buffer *mBuf);
#ifdef DAM_PDCP_OPT
EXTERN  S16 wrDamFbDatInd(U8 *tmpBuf,U8 *buf,U32 bufLen);
EXTERN S16 PjUiPjuDatReqFBPdcpOpt (U16 cellId,U16 ueId,U8 rbId,U32 sduId,FlatBuffer *mBuf);
#endif 
#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* End of __WR_DAM_H__ */


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
