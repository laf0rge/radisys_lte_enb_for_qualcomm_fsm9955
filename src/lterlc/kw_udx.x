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
 
    Name:   LTE RLC layer
 
    Type:   C include file
 
    Desc:   Defines required by LTE MAC
 
    File:   kw_udx.x
 
    Sid:      kw_udx.x@@/main/TeNB_Main_BR/5 - Mon Aug 11 16:43:29 2014
 
    Prg:   mraj
 
**********************************************************************/
/** @file kw_udx.x
@brief This file contains basic data structures for the Uplink Downlink 
interface.
*/
 

/*
*     The defines in this file correspond to structures and variables 
*     used by the following TRILLIUM software:
*
*     part no.             description
*     --------     ----------------------------------------------
*
*/
 
#ifndef __UDX_X__
#define __UDX_X__

#include "ckw.h"
#include "ckw.x"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

typedef struct kwNackInfo
{
   U8 isSegment;
   U16 sn;      /*!< Nack Sn */
   U16 soStart; /*!< Segment offset start */
   U16 soEnd;   /*!< Segment offset End */
}KwNackInfo;

typedef struct kwUdxStaPdu
{
   U16 ackSn;
   /* TODO : KW_MAX_NACK_CNT should set to MAx SR delay 
      It is tradeoff to consider array based instead of dynamic
      as there are chances where we might loose Status information
      on spill over*/
   U8 nackCnt;         /* Cnt of Nacks in the array*/
   KwNackInfo nackInfo[KW_MAX_NACK_CNT];
}KwUdxStaPdu;

typedef struct kwUdxBufLst
{
   CmLListCp rlsTxLst;      /*!< Stores to be released Tx PDUs */
   CmLListCp rlsRetxLst;    /*!< Stores to be released Retx PDUs */
   CmLListCp rlsSduLst;     /*!< Stores to be released SDUs*/
}KwUdxBufLst;

/* ### Umapathi*/
#if 0
typedef struct kwDlStaPdu
{
    CmLList lstEnt;   /*!< List entry for SDU */
    U16 pduSz;        /*!< Length of status PDU size */
    Buffer *mBuf;     /*!< Stores complete statusPdu to be dispatched */
}KwUdxDlStaPdu;
#endif

typedef struct kwStatusPdu
{
   U16         ackSn;
   KwNackInfo  nackInfo[KW_MAX_NACK_CNT];
   U16         nackCount;
   U16         controlBo;
}KwUdxDlStaPdu;
/* ### Umapathi*/

#if 0
typedef struct  kwUdxCntrlInfo
{
   S32          cntrlBo;
   U16          cntrlCnt; 
   KwCntrlInfo  cntrlInfo[KW_UDX_MAX_CNTRL_INFO]; 
}KwUdxCntrlInfo;
#endif
/**
  * @brief
  * UDX APIs
  */
typedef S16 (*UdxBndCfm) ARGS((Pst* pst, SuId suId,U8  status ));

typedef S16 (*UdxBndReq) ARGS((Pst* pst, SuId suId,SpId  spId ));

typedef S16 (*UdxUbndReq) ARGS((Pst* pst, SuId suId,Reason reason));

typedef S16 (*UdxCfgReq) ARGS((Pst *pst, SpId spId, CkwCfgInfo *cfgInfo));

typedef S16 (*UdxCfgCfm) ARGS((Pst* pst,SuId suId,CkwCfgCfmInfo *cfmInfo));

typedef S16 (*UdxStaUpdReq) ARGS((Pst* pst,
                                  SpId spId,
                                  CmLteRlcId *rlcId,
                                  KwUdxStaPdu *pStaPdu ));

typedef S16 (*UdxUeIdChgReq) ARGS((Pst *pst, 
                                   SpId spId, 
                                   U32 transId, 
                                   CkwUeInfo *ueInfo, 
                                   CkwUeInfo *newUeInfo));

typedef S16 (*UdxUeIdChgCfm) ARGS((Pst *pst, 
                                   SuId suId, 
                                   U32 transId, 
                                   CmStatus status));

typedef S16 (*UdxStaUpdCfm) ARGS((Pst* pst,
                                  SuId  suId,
                                  CmLteRlcId *rlcId,
                                  KwUdxBufLst *pBufLst));

typedef S16 (*UdxStaPduReq) ARGS((Pst* pst,
                                  SpId  spId, 
                                  CmLteRlcId *rlcId,
                                  KwUdxDlStaPdu *pStaPdu));

typedef S16 (*UdxStaProhTmrStart) ARGS((Pst* pst,
                                         SuId  suId,
                                         CmLteRlcId  *rlcId));

#ifdef LTE_L2_MEAS
typedef S16 (*UdxL2MeasReq) ARGS((Pst* pst,
                                KwL2MeasReqEvt *measReqEvt));

typedef S16 (*UdxL2MeasSendReq) ARGS((Pst* pst,
                                    U8 measType));

typedef S16 (*UdxL2MeasStopReq) ARGS((Pst* pst,
                                    U8 measType));
#endif

#ifdef LCUDX
EXTERN S16 cmPkUdxBndCfm ARGS((Pst* pst, SuId suId,U8  status ));
EXTERN S16 cmPkUdxBndReq ARGS((Pst* pst, SuId suId,SpId  spId ));
EXTERN S16 cmPkUdxUbndReq ARGS((Pst* pst, SuId suId,Reason reason));
EXTERN S16 cmPkUdxCfgReq ARGS((Pst *pst, SpId spId, CkwCfgInfo *cfgInfo));
EXTERN S16 cmPkUdxCfgCfm ARGS(( Pst* pst,SuId suId,CkwCfgCfmInfo *cfmInfo));
EXTERN S16 cmPkUdxStaUpdReq ARGS((Pst* pst,SpId spId,CmLteRlcId *rlcId,
                               KwUdxStaPdu         *pStaPdu ));
EXTERN S16 cmPkUdxUeIdChgReq ARGS((Pst *pst, SpId spId, U32 transId, 
                                  CkwUeInfo *ueInfo, CkwUeInfo *newUeInfo));
#if 0
EXTERN S16 cmPkUdxUeIdChgCfm ARGS((Pst *pst, SuId suId, U32 transId, 
                                   CkwUeInfo *ueInfo, CmStatus status));
#else
EXTERN S16 cmPkUdxUeIdChgCfm ARGS((Pst *pst, SuId suId, U32 transId, 
                                   CmStatus status));

#endif


EXTERN S16 cmPkUdxStaUpdCfm ARGS((Pst* pst,SuId  suId,CmLteRlcId  *rlcId,
                             KwUdxBufLst  *pBufLst));

EXTERN S16 cmPkUdxStaPduReq ARGS(( Pst* pst,SpId  spId, CmLteRlcId *rlcId,
                             KwUdxDlStaPdu  *pStaPdu ));
EXTERN S16 cmPkUdxStaProhTmrStart ARGS((Pst *pst, 
                                        SpId spId, 
                                        CmLteRlcId *rlcId));

#ifdef LTE_L2_MEAS
EXTERN S16 cmPkUdxL2MeasReq ARGS((Pst *pst, KwL2MeasReqEvt *measReqEvt)); 
EXTERN S16 cmPkUdxL2MeasSendReq ARGS((Pst *pst, U8 measType)); 
EXTERN S16 cmPkUdxL2MeasStopReq ARGS((Pst *pst, U8 measType)); 
#endif

EXTERN S16 cmUnpkUdxBndCfm ARGS((UdxBndCfm func,Pst* pst, Buffer *mBuf ));
EXTERN S16 cmUnpkUdxBndReq ARGS((UdxBndReq func,Pst* pst, Buffer *mBuf ));
EXTERN S16 cmUnpkUdxUbndReq ARGS((UdxUbndReq func,Pst* pst, Buffer *mBuf));
EXTERN S16 cmUnpkUdxCfgReq ARGS((UdxCfgReq func,Pst *pst, Buffer *mBuf));
EXTERN S16 cmUnpkUdxCfgCfm ARGS((UdxCfgCfm func , Pst* pst,Buffer *mBuf));
EXTERN S16 cmUnpkUdxStaUpdReq ARGS((UdxStaUpdReq func,Pst* pst,Buffer *mBuf));
EXTERN S16 cmUnpkUdxUeIdChgReq ARGS((UdxUeIdChgReq func,Pst *pst, Buffer *mBuf)); 
EXTERN S16 cmUnpkUdxUeIdChgCfm ARGS((UdxUeIdChgCfm func,Pst *pst, Buffer *mBuf)); 

EXTERN S16 cmUnpkUdxStaUpdCfm ARGS((UdxStaUpdCfm func,Pst* pst,Buffer *mBuf));

EXTERN S16 cmUnpkUdxStaPduReq ARGS((UdxStaPduReq func, Pst* pst, Buffer *mBuf));

EXTERN S16 cmUnpkUdxStaProhTmrStart ARGS((UdxStaProhTmrStart func,
                                          Pst* pst,
                                          Buffer *mBuf));

#ifdef LTE_L2_MEAS
EXTERN S16 cmUnpkUdxL2MeasReq ARGS((UdxL2MeasReq func, Pst* pst, Buffer *mBuf));
EXTERN S16 cmUnpkUdxL2MeasSendReq ARGS((UdxL2MeasSendReq func, Pst* pst, Buffer *mBuf));
EXTERN S16 cmUnpkUdxL2MeasStopReq ARGS((UdxL2MeasStopReq func, Pst* pst, Buffer *mBuf));
#endif
#endif

#ifdef KW
EXTERN S16 KwUlUdxBndReq ARGS((Pst* pst, SuId suId, SpId spId));

EXTERN S16 KwDlUdxBndReq ARGS((Pst* pst, SuId suId, SpId spId));

EXTERN S16 KwDlUdxBndCfm ARGS((Pst* pst, SuId suId, U8 status));

EXTERN S16 KwUlUdxBndCfm ARGS((Pst* pst, SuId suId, U8 status));

EXTERN S16 KwDlUdxCfgCfm ARGS((Pst* pst, SuId suId, CkwCfgCfmInfo *cfmInfo));

EXTERN S16 KwUlUdxCfgCfm ARGS((Pst* pst, SuId suId, CkwCfgCfmInfo *cfmInfo));

EXTERN S16 KwDlUdxUeIdChgCfm ARGS((Pst *pst, 
                                   SuId suId, 
                                   U32 transId,  
                                   CmStatus status));

EXTERN S16 KwUlUdxUeIdChgCfm ARGS((Pst *pst, 
                                   SuId suId, 
                                   U32 transId, 
                                   CmStatus status));

EXTERN S16 KwUlUdxUbndReq ARGS((Pst* pst, SuId suId, Reason reason));

EXTERN S16 KwDlUdxUbndReq ARGS((Pst* pst, SuId suId, Reason reason));

EXTERN S16 KwUlUdxUeIdChgReq ARGS((Pst *pst, 
                                   SpId spId, 
                                   U32 transId, 
                                   CkwUeInfo *ueInfo, 
                                   CkwUeInfo *newUeInfo));

EXTERN S16 KwUlUdxCfgReq ARGS((Pst *pst, SpId spId, CkwCfgInfo *cfgInfo));

EXTERN S16 KwDlUdxCfgReq ARGS((Pst *pst, SpId spId, CkwCfgInfo *cfgInfo));

EXTERN S16 KwUlUdxStaUpdReq ARGS((Pst* pst,
                                  SpId spId,
                                  CmLteRlcId *rlcId,
                                  KwUdxStaPdu *pStaPdu));

EXTERN S16 KwDlUdxStaUpdReq ARGS((Pst* pst,
                                  SpId spId,
                                  CmLteRlcId *rlcId,
                                  KwUdxStaPdu *pStaPdu));

EXTERN S16 KwDlUdxStaUpdCfm ARGS((Pst* pst,
                                  SuId suId,
                                  CmLteRlcId *rlcId,
                                  KwUdxBufLst *pBufLst));

EXTERN S16 KwUlUdxStaUpdCfm ARGS((Pst* pst,
                                  SuId suId,
                                  CmLteRlcId *rlcId,
                                  KwUdxBufLst *pBufLst));

EXTERN S16 KwUlUdxStaPduReq ARGS((Pst *pst,
                                  SpId spId,
                                  CmLteRlcId *rlcId,
                                  KwUdxDlStaPdu *pStaPdu ));

EXTERN S16 KwDlUdxStaPduReq ARGS((Pst *pst,
                                  SpId spId,
                                  CmLteRlcId *rlcId,
                                  KwUdxDlStaPdu *pStaPdu));

EXTERN S16 KwDlUdxUeIdChgReq ARGS((Pst *pst, 
                                   SpId spId, 
                                   U32 transId, 
                                   CkwUeInfo *ueInfo, 
                                   CkwUeInfo *newUeInfo));

EXTERN S16 KwDlUdxCfgReq ARGS((Pst *pst, SpId spId, CkwCfgInfo *cfgInfo));


EXTERN S16 KwUlUdxStaUpdCfm ARGS((Pst* pst,
                                  SuId suId,
                                  CmLteRlcId *rlcId,
                                  KwUdxBufLst *pBufLst));

EXTERN S16 KwDlUdxStaPduReq ARGS((Pst *pst,
                                  SpId spId,
                                  CmLteRlcId *rlcId,
                                  KwUdxDlStaPdu *pStaPdu));

EXTERN S16 KwUlUdxStaProhTmrStart ARGS((Pst *pst,
                                        SpId spId,
                                        CmLteRlcId *rlcId ));

EXTERN S16 KwDlUdxStaProhTmrStart ARGS((Pst *pst,
                                        SpId spId,
                                        CmLteRlcId *rlcId ));

#ifdef LTE_L2_MEAS
EXTERN S16 KwDlUdxL2MeasReq ARGS((Pst *pst, KwL2MeasReqEvt *measReqEvt)); 

EXTERN S16 KwDlUdxL2MeasSendReq ARGS((Pst *pst, U8 status)); 

EXTERN S16 KwDlUdxL2MeasStopReq ARGS((Pst *pst, U8 status)); 

EXTERN S16 KwUlUdxL2MeasReq ARGS((Pst* pst, KwL2MeasReqEvt *measReqEvt));

EXTERN S16 KwUlUdxL2MeasSendReq ARGS((Pst *pst, U8 status));

EXTERN S16 KwUlUdxL2MeasStopReq ARGS((Pst* pst, U8 measType));
#endif

#endif

#ifdef __cplusplus
}
#endif /* __cplusplus */
#endif /* __UDX_X__ */


/**********************************************************************
         End of file:     kw_udx.x@@/main/TeNB_Main_BR/5 - Mon Aug 11 16:43:29 2014
**********************************************************************/
/**********************************************************************
 
        Revision history:
 
**********************************************************************/
/********************************************************************90**
 
     ver       pat    init                  description
------------ -------- ---- ----------------------------------------------
/main/1      ---       mraj   1. LTE RLC Initial Release
*********************************************************************91*/
