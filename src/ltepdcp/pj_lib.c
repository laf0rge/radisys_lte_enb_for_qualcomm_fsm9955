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
  
        Name:    LTE-RLC Layer - Upper Interface
    
        Type:    C file
  
        Desc:    C source code for the hook functions of LTE-PDCP

                 This file contains the following functions:
         
                 --pjLibCmpInit
                 --pjLibIntInit
                 --pjLibCpInit
                 --pjLibUpInit
                 --pjLibCmpReset
                 --pjLibCmpSdu
                 --pjLibIntProt
                 --pjLibIntVerify
                 --pjLibDecipherSdu
                 --pjLibSndFeedback
                 --pjLibCmpClose
                 --pjLibIntClose
                 --pjLibCpClose
                 --pjLibUpClose

                 Portable functions for software hooks:

                 --pjLibCmpInitReq
                 --pjLibIntInitReq
                 --pjLibCpInitReq
                 --pjLibUpInitReq
                 --pjLibCmpResetReq
                 --pjLibCmpReq
                 --pjLibIntProtReq
                 --pjLibCipherReq
                 --pjLibDecipherReq
                 --pjLibIntVerReq
                 --pjLibDeCmpReq
                 --pjLibCmpCloseReq
                 --pjLibIntCloseReq
                 --pjLibCiperCloseReq
                 --pjLibUpCloseReq 

 
        File:    gp_pj_lib.c

        Sid:      eq_lib.c@@/main/TeNB_Main_BR/tenb_main_ccb/1 - Wed Jul 22 18:21:35 2015
  
        Prg:     nm
  
*********************************************************************21*/

/** @file gp_pj_lib.c
@brief PDPC Hook Interface
*/


/* header (.h) include files */
#include "envopt.h"        /* environment options */
#include "envdep.h"        /* environment dependent */
#include "envind.h"        /* environment independent */

#include "gen.h"           /* general */
#include "ssi.h"           /* system services */
#include "cm5.h"           /* common timer defines */
#include "cm_tkns.h"       /* common tokens defines */
#include "cm_mblk.h"       /* common memory allocation library defines */
#include "cm_llist.h"      /* common link list  defines  */
#include "cm_hash.h"       /* common hash list  defines */
#include "cm_lte.h"        /* common LTE defines */
#include "cpj.h"                /* RRC layer */
#include "pju.h"                /* PDCP service user */
#include "lpj.h"                /* LPJ defines */
#include "pj_err.h"
#include "pj_env.h"        /* RLC environment options */
#include "pj_lib.h"     /* LIB options */
#include "pj.h"            /* RLC defines */
#include "pj_dl.h"

/* extern (.x) include files */
#include "gen.x"           /* general */
#include "ssi.x"           /* system services */

#include "cm5.x"           /* common timer library */
#include "cm_tkns.x"       /* common tokens */
#include "cm_mblk.x"       /* common memory allocation */
#include "cm_llist.x"      /* common link list */
#include "cm_hash.x"       /* common hash list */
#include "cm_lte.x"        /* common LTE includes */
#include "cm_lib.x"        /* common memory allocation library */
#include "cpj.x"           /* CPJ */
#include "pju.x"           /* PJU */
#include "lpj.x"           /* LPJ */

#include "pj.x"
#include "pj_udx.h"
#include "pj_udx.x"
#include "pj_dl.x"
#include "pj_lib.x"     /* LIB */

#ifdef __cplusplus
EXTERN "C" {
#endif /* __cplusplus */


/****************************************************************************
 *                         Portable hooks for asynchronous 
 ***************************************************************************/

#ifdef PJ_CMP_ASYNC
#ifdef ANSI
PUBLIC S16 cmPkLibObdCmpInitCfm
(
Pst * pst,
PjCmpCxt cmpCxt,
PTR ctxId
)
#else
PUBLIC S16 cmPkLibObdCmpInitCfm(pst, cmpCxt, ctxId)
Pst * pst;
PjCmpCxt cmpCxt;
PTR ctxId;
#endif
{
   Buffer *mBuf = NULLP;
   TRC3(cmPkLibObdCmpInitCfm)

   if (SGetMsg(pst->region, pst->pool, &mBuf) != ROK) {
#if (ERRCLASS & ERRCLS_ADD_RES)
      SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
         __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
         (ErrVal)ELIB001, (ErrVal)0, "Packing failed");
#endif
      RETVALUE(RFAILED);
   }
   if (cmPkPtr(ctxId, mBuf) != ROK) {
#if (ERRCLASS & ERRCLS_ADD_RES)
      SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
         __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
         (ErrVal)ELIB002, (ErrVal)0, "Packing failed");
#endif
      PJ_FREE_BUF(mBuf);
      RETVALUE(RFAILED);
   }
   if (cmPkPjCmpCxt(&cmpCxt, mBuf) != ROK) {
#if (ERRCLASS & ERRCLS_ADD_RES)
      SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
         __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
         (ErrVal)ELIB003, (ErrVal)0, "Packing failed");
#endif
      PJ_FREE_BUF(mBuf);
      RETVALUE(RFAILED);
   }
   pst->event = (Event) OBD_EVT_CMP_INIT_CFM;
   RETVALUE(SPstTsk(pst,mBuf));
}

#ifdef ANSI
PUBLIC S16 cmUnpkLibObdCmpInitCfm
(
ObdCmpInitCfm func,
Pst *pst,
Buffer *mBuf
)
#else
PUBLIC S16 cmUnpkLibObdCmpInitCfm(func, pst, mBuf)
ObdCmpInitCfm func;
Pst *pst;
Buffer *mBuf;
#endif
{
   PjCmpCxt cmpCxt;
   PTR ctxId;
   
   TRC3(cmUnpkLibObdCmpInitCfm)

   if (cmUnpkPjCmpCxt(&cmpCxt, mBuf) != ROK) {
      PJ_FREE_BUF(mBuf);
#if (ERRCLASS & ERRCLS_ADD_RES)
      SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
         __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
         (ErrVal)ELIB004, (ErrVal)0, "Packing failed");
#endif
      RETVALUE(RFAILED);
   }
   if (cmUnpkPtr(&ctxId, mBuf) != ROK) {
      PJ_FREE_BUF(mBuf);
#if (ERRCLASS & ERRCLS_ADD_RES)
      SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
         __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
         (ErrVal)ELIB005, (ErrVal)0, "Packing failed");
#endif
      RETVALUE(RFAILED);
   }
   PJ_FREE_BUF(mBuf);
   RETVALUE((*func)(pst, cmpCxt, ctxId));
}

#ifdef ANSI
PUBLIC S16 cmPkLibObdCmpCfm
(
Pst * pst,
PTR cmpCxtId,
PjLibTrans libTrans,
Buffer * opSdu
)
#else
PUBLIC S16 cmPkLibObdCmpCfm(pst, cmpCxtId, libTrans, opSdu)
Pst * pst;
PTR cmpCxtId;
PjLibTrans libTrans;
Buffer * opSdu;
#endif
{
   Buffer *mBuf = NULLP;
   MsgLen msgLen = 0;
   TRC3(cmPkLibObdCmpCfm)

   if (pst->selector == OBD_SEL_LC) {
      if ( opSdu != NULLP )
      {
      if (SFndLenMsg(opSdu, &msgLen) != ROK) {
#if (ERRCLASS & ERRCLS_ADD_RES)
         SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
            __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
            (ErrVal)ELIB022, (ErrVal)0, "Packing failed");
#endif
         PJ_FREE_BUF(opSdu);
         RETVALUE(RFAILED);
      }
      if (SCpyMsgMsg(opSdu, pst->region, pst->pool, &mBuf) != ROK) {
#if (ERRCLASS & ERRCLS_ADD_RES)
         SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
            __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
            (ErrVal)ELIB023, (ErrVal)0, "Packing failed");
#endif
         RETVALUE(RFAILED);
      }
      }
      else
      {
         msgLen = 0;
         if (SGetMsg(pst->region, pst->pool, &mBuf) != ROK) {
#if (ERRCLASS & ERRCLS_ADD_RES)
            SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
                  __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
                  (ErrVal)ELIB036, (ErrVal)0, "Packing failed");
#endif
            RETVALUE(RFAILED);
         }
      }
      CMCHKPK(cmPkMsgLen, msgLen, mBuf);

   }
   else
   {
   if (SGetMsg(pst->region, pst->pool, &mBuf) != ROK) {
#if (ERRCLASS & ERRCLS_ADD_RES)
      SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
         __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
         (ErrVal)ELIB021, (ErrVal)0, "Packing failed");
#endif
      RETVALUE(RFAILED);
   }
   }

   if (cmPkPjLibTrans(&libTrans, mBuf) != ROK) {
#if (ERRCLASS & ERRCLS_ADD_RES)
      SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
         __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
         (ErrVal)ELIB025, (ErrVal)0, "Packing failed");
#endif
      PJ_FREE_BUF(mBuf);
      RETVALUE(RFAILED);
   }
   if (cmPkPtr(cmpCxtId, mBuf) != ROK) {
#if (ERRCLASS & ERRCLS_ADD_RES)
      SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
         __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
         (ErrVal)ELIB026, (ErrVal)0, "Packing failed");
#endif
      PJ_FREE_BUF(mBuf);
      RETVALUE(RFAILED);
   }
   pst->event = (Event) OBD_EVT_CMP_CFM;
   RETVALUE(SPstTsk(pst,mBuf));
}

#ifdef ANSI
PUBLIC S16 cmUnpkLibObdCmpCfm
(
ObdCmpCfm func,
Pst *pst,
Buffer *mBuf
)
#else
PUBLIC S16 cmUnpkLibObdCmpCfm(func, pst, mBuf)
ObdCmpCfm func;
Pst *pst;
Buffer *mBuf;
#endif
{
   PTR cmpCxtId;
   PjLibTrans libTrans;
   Buffer *opSdu;
   
   TRC3(cmUnpkLibObdCmpCfm)
   
   opSdu = NULLP;

   if (cmUnpkPtr(&cmpCxtId, mBuf) != ROK) {
      PJ_FREE_BUF(mBuf);
#if (ERRCLASS & ERRCLS_ADD_RES)
      SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
         __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
         (ErrVal)ELIB027, (ErrVal)0, "Packing failed");
#endif
      RETVALUE(RFAILED);
   }
   if (cmUnpkPjLibTrans(&libTrans, mBuf) != ROK) {
      PJ_FREE_BUF(mBuf);
#if (ERRCLASS & ERRCLS_ADD_RES)
      SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
         __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
         (ErrVal)ELIB028, (ErrVal)0, "Packing failed");
#endif
      RETVALUE(RFAILED);
   }
   if (pst->selector == OBD_SEL_LC) {
      MsgLen msgLen, totalMsgLen;
      CMCHKUNPK(cmUnpkMsgLen, &msgLen, mBuf);
      if (SFndLenMsg(mBuf, &totalMsgLen) != ROK)
         RETVALUE(RFAILED);
      if ( msgLen != 0 )
      {
         if (SSegMsg(mBuf, totalMsgLen-msgLen, &opSdu) != ROK)
            RETVALUE(RFAILED);
      }
   }

   PJ_FREE_BUF(mBuf);
   RETVALUE((*func)(pst, cmpCxtId, libTrans, opSdu));
}

#ifdef ANSI
PUBLIC S16 cmPkLibObdDecmpCfm
(
Pst * pst,
PTR cmpCxtId,
PjLibTrans libTrans,
Buffer * opSdu,
Buffer * fbPkt
)
#else
PUBLIC S16 cmPkLibObdDecmpCfm(pst, cmpCxtId, libTrans, opSdu, fbPkt)
Pst * pst;
PTR cmpCxtId;
PjLibTrans libTrans;
Buffer * opSdu;
Buffer * fbPkt;
#endif
{
   Buffer *mBuf = NULLP;
   Buffer *tBuf = NULLP;
   MsgLen msgLen = 0;
   TRC3(cmPkLibObdDecmpCfm)

   if (fbPkt != NULLP) {
      if (SFndLenMsg(fbPkt, &msgLen) != ROK) {
#if (ERRCLASS & ERRCLS_ADD_RES)
         SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
            __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
            (ErrVal)ELIB060, (ErrVal)0, "Packing failed");
#endif
         RETVALUE(RFAILED);
      }
      if (SCpyMsgMsg(fbPkt, pst->region, pst->pool, &mBuf) != ROK) {
#if (ERRCLASS & ERRCLS_ADD_RES)
         SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
            __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
            (ErrVal)ELIB061, (ErrVal)0, "Packing failed");
#endif
         RETVALUE(RFAILED);
      }
   }else{
      if (SGetMsg(pst->region, pst->pool, &mBuf) != ROK) {
#if (ERRCLASS & ERRCLS_ADD_RES)
         SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
               __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
               (ErrVal)ELIB059, (ErrVal)0, "Packing failed");
#endif
         RETVALUE(RFAILED);
      }

      msgLen = 0;
   }
   CMCHKPK(cmPkMsgLen, msgLen, mBuf);

   if (opSdu != NULLP) {
      if (SFndLenMsg(opSdu, &msgLen) != ROK) {
#if (ERRCLASS & ERRCLS_ADD_RES)
         SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
            __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
            (ErrVal)ELIB060, (ErrVal)0, "Packing failed");
#endif
         PJ_FREE_BUF(mBuf);
         RETVALUE(RFAILED);
      }
      if (SCpyMsgMsg(opSdu, pst->region, pst->pool, &tBuf) != ROK) {
#if (ERRCLASS & ERRCLS_ADD_RES)
         SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
            __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
            (ErrVal)ELIB061, (ErrVal)0, "Packing failed");
#endif
         PJ_FREE_BUF(mBuf);
         RETVALUE(RFAILED);
      }
      if (SCatMsg(mBuf, tBuf, M1M2) != ROK) {
#if (ERRCLASS & ERRCLS_ADD_RES)
         SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
            __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
            (ErrVal)ELIB061, (ErrVal)0, "Packing failed");
#endif
         PJ_FREE_BUF(mBuf);
         PJ_FREE_BUF(tBuf);
         RETVALUE(RFAILED);
      }

      if (tBuf != NULLP)
         PJ_FREE_BUF(tBuf);
   } else {
      msgLen = 0;
   }
   CMCHKPK(cmPkMsgLen, msgLen, mBuf);

   if (cmPkPjLibTrans(&libTrans, mBuf) != ROK) {
#if (ERRCLASS & ERRCLS_ADD_RES)
      SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
         __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
         (ErrVal)ELIB063, (ErrVal)0, "Packing failed");
#endif
      PJ_FREE_BUF(mBuf);
      RETVALUE(RFAILED);
   }
   if (cmPkPtr(cmpCxtId, mBuf) != ROK) {
#if (ERRCLASS & ERRCLS_ADD_RES)
      SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
         __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
         (ErrVal)ELIB064, (ErrVal)0, "Packing failed");
#endif
      PJ_FREE_BUF(mBuf);
      RETVALUE(RFAILED);
   }
   pst->event = (Event) OBD_EVT_DECMP_CFM;
   RETVALUE(SPstTsk(pst,mBuf));
}

#ifdef ANSI
PUBLIC S16 cmUnpkLibObdDecmpCfm
(
ObdDecmpCfm func,
Pst *pst,
Buffer *mBuf
)
#else
PUBLIC S16 cmUnpkLibObdDecmpCfm(func, pst, mBuf)
ObdDecmpCfm func;
Pst *pst;
Buffer *mBuf;
#endif
{
   PTR cmpCxtId;
   PjLibTrans libTrans;
   Buffer *opSdu = NULLP;
   Buffer *fbPkt = NULLP;
   
   TRC3(cmUnpkLibObdDecmpCfm)

   if (cmUnpkPtr(&cmpCxtId, mBuf) != ROK) {
      PJ_FREE_BUF(mBuf);
#if (ERRCLASS & ERRCLS_ADD_RES)
      SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
         __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
         (ErrVal)ELIB065, (ErrVal)0, "Packing failed");
#endif
      RETVALUE(RFAILED);
   }
   if (cmUnpkPjLibTrans(&libTrans, mBuf) != ROK) {
      PJ_FREE_BUF(mBuf);
#if (ERRCLASS & ERRCLS_ADD_RES)
      SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
         __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
         (ErrVal)ELIB066, (ErrVal)0, "Packing failed");
#endif
      RETVALUE(RFAILED);
   }
   if (pst->selector == OBD_SEL_LC) {
      MsgLen msgLen, totalMsgLen;
      CMCHKUNPK(cmUnpkMsgLen, &msgLen, mBuf);
      if (SFndLenMsg(mBuf, &totalMsgLen) != ROK)
         RETVALUE(RFAILED);
      if(msgLen != 0)
         if (SSegMsg(mBuf, totalMsgLen-msgLen, &opSdu) != ROK)
            RETVALUE(RFAILED);
   }

   if (pst->selector == OBD_SEL_LC) {
      MsgLen msgLen, totalMsgLen;
      CMCHKUNPK(cmUnpkMsgLen, &msgLen, mBuf);
      if (SFndLenMsg(mBuf, &totalMsgLen) != ROK)
         RETVALUE(RFAILED);
      if(msgLen != 0)
         if (SSegMsg(mBuf, totalMsgLen-msgLen, &fbPkt) != ROK)
            RETVALUE(RFAILED);
   }

   PJ_FREE_BUF(mBuf);
   RETVALUE((*func)(pst, cmpCxtId, libTrans, opSdu, fbPkt));
}


/***********************************************************
*
*     Func : cmPkPjRohc
*
*
*     Desc : PjHdrCmp
  Robust header compression Information
*
*
*     Ret  : S16
*
*     Notes:
*
*     File  : 
*
**********************************************************/
#ifdef ANSI
PUBLIC S16 cmPkPjRohc
(
PjRohc *param,
Buffer *mBuf
)
#else
PUBLIC S16 cmPkPjRohc(param, mBuf)
PjRohc *param;
Buffer *mBuf;
#endif
{

   TRC3(cmPkPjRohc)

   CMCHKPK(SPkU16, param->profId, mBuf);
   CMCHKPK(SPkU16, param->maxCid, mBuf);
   CMCHKPK(SPkU8, param->hdrCmpUsed, mBuf);
   RETVALUE(ROK);
}


/***********************************************************
*
*     Func : cmUnpkPjRohc
*
*
*     Desc : PjHdrCmp
  Robust header compression Information
*
*
*     Ret  : S16
*
*     Notes:
*
*     File  : 
*
**********************************************************/
#ifdef ANSI
PUBLIC S16 cmUnpkPjRohc
(
PjRohc *param,
Buffer *mBuf
)
#else
PUBLIC S16 cmUnpkPjRohc(param, mBuf)
PjRohc *param;
Buffer *mBuf;
#endif
{

   TRC3(cmUnpkPjRohc)

   CMCHKUNPK(SUnpkU8, &param->hdrCmpUsed, mBuf);
   CMCHKUNPK(SUnpkU16, &param->maxCid, mBuf);
   CMCHKUNPK(SUnpkU16, &param->profId, mBuf);
   RETVALUE(ROK);
}



/***********************************************************
*
*     Func : cmPkPjCmpCxt
*
*
*     Desc : PjCmpCxt
Context for ROHC
*
*
*     Ret  : S16
*
*     Notes:
*
*     File  : 
*
**********************************************************/
#ifdef ANSI
PUBLIC S16 cmPkPjCmpCxt
(
PjCmpCxt *param,
Buffer *mBuf
)
#else
PUBLIC S16 cmPkPjCmpCxt(param, mBuf)
PjCmpCxt *param;
Buffer *mBuf;
#endif
{

   TRC3(cmPkPjCmpCxt)

   CMCHKPK(cmPkLteRbId, param->rbId, mBuf);
   CMCHKPK(cmPkLteRnti, param->ueId, mBuf);
   CMCHKPK(cmPkLteCellId, param->cellId, mBuf);
   CMCHKPK(SPkU8, param->rbType, mBuf);
   CMCHKPK(SPkU16, param->cfgTxId, mBuf);
   RETVALUE(ROK);
}



/***********************************************************
*
*     Func : cmUnpkPjCmpCxt
*
*
*     Desc : PjCmpCxt
Context for ROHC
*
*
*     Ret  : S16
*
*     Notes:
*
*     File  : 
*
**********************************************************/
#ifdef ANSI
PUBLIC S16 cmUnpkPjCmpCxt
(
PjCmpCxt *param,
Buffer *mBuf
)
#else
PUBLIC S16 cmUnpkPjCmpCxt(param, mBuf)
PjCmpCxt *param;
Buffer *mBuf;
#endif
{

   TRC3(cmUnpkPjCmpCxt)

   CMCHKUNPK(SUnpkU16, &param->cfgTxId, mBuf);
   CMCHKUNPK(SUnpkU8, &param->rbType, mBuf);
   CMCHKUNPK(cmUnpkLteCellId, &param->cellId, mBuf);
   CMCHKUNPK(cmUnpkLteRnti, &param->ueId, mBuf);
   CMCHKUNPK(cmUnpkLteRbId, &param->rbId, mBuf);
   RETVALUE(ROK);
}

#endif /* PJ_CMP_ASYNC */

#ifdef PJ_SEC_ASYNC
#ifdef ANSI
PUBLIC S16 cmPkLibObdIntInitCfm
(
Pst * pst,
PjSecCxt secCtx,
PTR ctxId
)
#else
PUBLIC S16 cmPkLibObdIntInitCfm(pst, secCtx, ctxId)
Pst * pst;
PjSecCxt secCtx;
PTR ctxId;
#endif
{
   Buffer *mBuf = NULLP;
   TRC3(cmPkLibObdIntInitCfm)

   if (SGetMsg(pst->region, pst->pool, &mBuf) != ROK) {
#if (ERRCLASS & ERRCLS_ADD_RES)
      SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
         __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
         (ErrVal)ELIB006, (ErrVal)0, "Packing failed");
#endif
      RETVALUE(RFAILED);
   }
   if (cmPkPtr(ctxId, mBuf) != ROK) {
#if (ERRCLASS & ERRCLS_ADD_RES)
      SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
         __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
         (ErrVal)ELIB007, (ErrVal)0, "Packing failed");
#endif
      PJ_FREE_BUF(mBuf);
      RETVALUE(RFAILED);
   }
   if (cmPkPjSecCxt(&secCtx, mBuf) != ROK) {
#if (ERRCLASS & ERRCLS_ADD_RES)
      SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
         __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
         (ErrVal)ELIB008, (ErrVal)0, "Packing failed");
#endif
      PJ_FREE_BUF(mBuf);
      RETVALUE(RFAILED);
   }
   pst->event = (Event) OBD_EVT_INT_INIT_CFM;
   RETVALUE(SPstTsk(pst,mBuf));
}

#ifdef ANSI
PUBLIC S16 cmUnpkLibObdIntInitCfm
(
ObdIntInitCfm func,
Pst *pst,
Buffer *mBuf
)
#else
PUBLIC S16 cmUnpkLibObdIntInitCfm(func, pst, mBuf)
ObdIntInitCfm func;
Pst *pst;
Buffer *mBuf;
#endif
{
   PjSecCxt secCtx;
   PTR ctxId;
   
   TRC3(cmUnpkLibObdIntInitCfm)

   if (cmUnpkPjSecCxt(&secCtx, mBuf) != ROK) {
      PJ_FREE_BUF(mBuf);
#if (ERRCLASS & ERRCLS_ADD_RES)
      SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
         __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
         (ErrVal)ELIB009, (ErrVal)0, "Packing failed");
#endif
      RETVALUE(RFAILED);
   }
   if (cmUnpkPtr(&ctxId, mBuf) != ROK) {
      PJ_FREE_BUF(mBuf);
#if (ERRCLASS & ERRCLS_ADD_RES)
      SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
         __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
         (ErrVal)ELIB010, (ErrVal)0, "Packing failed");
#endif
      RETVALUE(RFAILED);
   }
   PJ_FREE_BUF(mBuf);
   RETVALUE((*func)(pst, secCtx, ctxId));
}

#ifdef ANSI
PUBLIC S16 cmPkLibObdCpInitCfm
(
Pst * pst,
PjSecCxt secCxt,
PTR ctxId
)
#else
PUBLIC S16 cmPkLibObdCpInitCfm(pst, secCxt, ctxId)
Pst * pst;
PjSecCxt secCxt;
PTR ctxId;
#endif
{
   Buffer *mBuf = NULLP;
   TRC3(cmPkLibObdCpInitCfm)

   if (SGetMsg(pst->region, pst->pool, &mBuf) != ROK) {
#if (ERRCLASS & ERRCLS_ADD_RES)
      SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
         __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
         (ErrVal)ELIB011, (ErrVal)0, "Packing failed");
#endif
      RETVALUE(RFAILED);
   }
   if (cmPkPtr(ctxId, mBuf) != ROK) {
#if (ERRCLASS & ERRCLS_ADD_RES)
      SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
         __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
         (ErrVal)ELIB012, (ErrVal)0, "Packing failed");
#endif
      PJ_FREE_BUF(mBuf);
      RETVALUE(RFAILED);
   }
   if (cmPkPjSecCxt(&secCxt, mBuf) != ROK) {
#if (ERRCLASS & ERRCLS_ADD_RES)
      SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
         __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
         (ErrVal)ELIB013, (ErrVal)0, "Packing failed");
#endif
      PJ_FREE_BUF(mBuf);
      RETVALUE(RFAILED);
   }
   pst->event = (Event) OBD_EVT_CP_INIT_CFM;
   RETVALUE(SPstTsk(pst,mBuf));
}

#ifdef ANSI
PUBLIC S16 cmUnpkLibObdCpInitCfm
(
ObdCpInitCfm func,
Pst *pst,
Buffer *mBuf
)
#else
PUBLIC S16 cmUnpkLibObdCpInitCfm(func, pst, mBuf)
ObdCpInitCfm func;
Pst *pst;
Buffer *mBuf;
#endif
{
   PjSecCxt secCxt;
   PTR ctxId;
   
   TRC3(cmUnpkLibObdCpInitCfm)

   if (cmUnpkPjSecCxt(&secCxt, mBuf) != ROK) {
      PJ_FREE_BUF(mBuf);
#if (ERRCLASS & ERRCLS_ADD_RES)
      SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
         __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
         (ErrVal)ELIB014, (ErrVal)0, "Packing failed");
#endif
      RETVALUE(RFAILED);
   }
   if (cmUnpkPtr(&ctxId, mBuf) != ROK) {
      PJ_FREE_BUF(mBuf);
#if (ERRCLASS & ERRCLS_ADD_RES)
      SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
         __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
         (ErrVal)ELIB015, (ErrVal)0, "Packing failed");
#endif
      RETVALUE(RFAILED);
   }
   PJ_FREE_BUF(mBuf);
   RETVALUE((*func)(pst, secCxt, ctxId));
}

#ifdef ANSI
PUBLIC S16 cmPkLibObdUpInitCfm
(
Pst * pst,
PjSecCxt secCxt,
PTR ctxId
)
#else
PUBLIC S16 cmPkLibObdUpInitCfm(pst, secCxt, ctxId)
Pst * pst;
PjSecCxt secCxt;
PTR ctxId;
#endif
{
   Buffer *mBuf = NULLP;
   TRC3(cmPkLibObdUpInitCfm)

   if (SGetMsg(pst->region, pst->pool, &mBuf) != ROK) {
#if (ERRCLASS & ERRCLS_ADD_RES)
      SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
         __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
         (ErrVal)ELIB016, (ErrVal)0, "Packing failed");
#endif
      RETVALUE(RFAILED);
   }
   if (cmPkPtr(ctxId, mBuf) != ROK) {
#if (ERRCLASS & ERRCLS_ADD_RES)
      SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
         __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
         (ErrVal)ELIB017, (ErrVal)0, "Packing failed");
#endif
      PJ_FREE_BUF(mBuf);
      RETVALUE(RFAILED);
   }
   if (cmPkPjSecCxt(&secCxt, mBuf) != ROK) {
#if (ERRCLASS & ERRCLS_ADD_RES)
      SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
         __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
         (ErrVal)ELIB018, (ErrVal)0, "Packing failed");
#endif
      PJ_FREE_BUF(mBuf);
      RETVALUE(RFAILED);
   }
   pst->event = (Event) OBD_EVT_UP_INIT_CFM;
   RETVALUE(SPstTsk(pst,mBuf));
}

#ifdef ANSI
PUBLIC S16 cmUnpkLibObdUpInitCfm
(
ObdUpInitCfm func,
Pst *pst,
Buffer *mBuf
)
#else
PUBLIC S16 cmUnpkLibObdUpInitCfm(func, pst, mBuf)
ObdUpInitCfm func;
Pst *pst;
Buffer *mBuf;
#endif
{
   PjSecCxt secCxt;
   PTR ctxId;
   
   TRC3(cmUnpkLibObdUpInitCfm)

   if (cmUnpkPjSecCxt(&secCxt, mBuf) != ROK) {
      PJ_FREE_BUF(mBuf);
#if (ERRCLASS & ERRCLS_ADD_RES)
      SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
         __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
         (ErrVal)ELIB019, (ErrVal)0, "Packing failed");
#endif
      RETVALUE(RFAILED);
   }
   if (cmUnpkPtr(&ctxId, mBuf) != ROK) {
      PJ_FREE_BUF(mBuf);
#if (ERRCLASS & ERRCLS_ADD_RES)
      SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
         __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
         (ErrVal)ELIB020, (ErrVal)0, "Packing failed");
#endif
      RETVALUE(RFAILED);
   }
   PJ_FREE_BUF(mBuf);
   RETVALUE((*func)(pst, secCxt, ctxId));
}

#ifdef ANSI
PUBLIC S16 cmPkLibObdCipherCfm
(
Pst * pst,
PTR cxtId,
PjLibTrans libTrans,
Buffer * opSdu
)
#else
PUBLIC S16 cmPkLibObdCipherCfm(pst, cxtId, libTrans, opSdu)
Pst * pst;
PTR cxtId;
PjLibTrans libTrans;
Buffer * opSdu;
#endif
{
   Buffer *mBuf = NULLP;
   MsgLen msgLen = 0;
   TRC3(cmPkLibObdCipherCfm)

   if (pst->selector == OBD_SEL_LC) {
      if ( opSdu != NULLP ) {
          if (cmPkPtr((PTR)opSdu, mBuf) != ROK) {
#if (ERRCLASS & ERRCLS_ADD_RES)
            SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
               __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
                (ErrVal)ELIB041, (ErrVal)0, "Packing failed");
#endif
            PJ_FREE_BUF(opSdu);
             PJ_FREE_BUF(mBuf);
            RETVALUE(RFAILED);
         }
      }
   }
   else
   {
      if (SGetMsg(pst->region, pst->pool, &mBuf) != ROK) {
#if (ERRCLASS & ERRCLS_ADD_RES)
         SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
               __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
               (ErrVal)ELIB036, (ErrVal)0, "Packing failed");
#endif
         PJ_FREE_BUF(opSdu);
         RETVALUE(RFAILED);
      }
   }


   if (cmPkPjLibTrans(&libTrans, mBuf) != ROK) {
#if (ERRCLASS & ERRCLS_ADD_RES)
      SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
         __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
         (ErrVal)ELIB040, (ErrVal)0, "Packing failed");
#endif
      PJ_FREE_BUF(opSdu);
      PJ_FREE_BUF(mBuf);
      RETVALUE(RFAILED);
   }
   if (cmPkPtr(cxtId, mBuf) != ROK) {
#if (ERRCLASS & ERRCLS_ADD_RES)
      SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
         __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
         (ErrVal)ELIB041, (ErrVal)0, "Packing failed");
#endif
      PJ_FREE_BUF(opSdu);
      PJ_FREE_BUF(mBuf);
      RETVALUE(RFAILED);
   }
   pst->event = (Event) OBD_EVT_CIPHER_CFM;
   RETVALUE(SPstTsk(pst,mBuf));
}

#ifdef ANSI
PUBLIC S16 cmUnpkLibObdCipherCfm
(
ObdCipherCfm func,
Pst *pst,
Buffer *mBuf
)
#else
PUBLIC S16 cmUnpkLibObdCipherCfm(func, pst, mBuf)
ObdCipherCfm func;
Pst *pst;
Buffer *mBuf;
#endif
{
   PTR cxtId;
   PjLibTrans libTrans;
   Buffer *opSdu;
   
   TRC3(cmUnpkLibObdCipherCfm)

   opSdu = NULLP;

   if (cmUnpkPtr(&cxtId, mBuf) != ROK) {
      PJ_FREE_BUF(mBuf);
#if (ERRCLASS & ERRCLS_ADD_RES)
      SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
         __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
         (ErrVal)ELIB042, (ErrVal)0, "Packing failed");
#endif
      RETVALUE(RFAILED);
   }
   if (cmUnpkPjLibTrans(&libTrans, mBuf) != ROK) {
      PJ_FREE_BUF(mBuf);
#if (ERRCLASS & ERRCLS_ADD_RES)
      SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
         __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
         (ErrVal)ELIB043, (ErrVal)0, "Packing failed");
#endif
      RETVALUE(RFAILED);
   }

   if (pst->selector == OBD_SEL_LC) {
      MsgLen msgLen, totalMsgLen;
      CMCHKUNPK(cmUnpkMsgLen, &msgLen, mBuf);
      if (SFndLenMsg(mBuf, &totalMsgLen) != ROK)
         RETVALUE(RFAILED);
      if ( msgLen != 0 )
      {
         if (SSegMsg(mBuf, totalMsgLen-msgLen, &opSdu) != ROK)
         {
            RETVALUE(RFAILED);
         }
      }
   }

   PJ_FREE_BUF(mBuf);
   RETVALUE((*func)(pst, cxtId, libTrans, opSdu));
}

#ifdef ANSI
PUBLIC S16 cmPkLibObdDecipherCfm
(
Pst * pst,
PTR cxtId,
PjLibTrans libTrans,
Buffer * opSdu
)
#else
PUBLIC S16 cmPkLibObdDecipherCfm(pst, cxtId, libTrans, opSdu)
Pst * pst;
PTR cxtId;
PjLibTrans libTrans;
Buffer * opSdu;
#endif
{
   Buffer *mBuf = NULLP;
   MsgLen msgLen = 0;
   TRC3(cmPkLibObdDecipherCfm)

   if (pst->selector == OBD_SEL_LC) {
      if ( opSdu != NULLP)
      {
         if (SFndLenMsg(opSdu, &msgLen) != ROK) {
#if (ERRCLASS & ERRCLS_ADD_RES)
            SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
               __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
               (ErrVal)ELIB045, (ErrVal)0, "Packing failed");
#endif
            RETVALUE(RFAILED);
         }
         if (SCpyMsgMsg(opSdu, pst->region, pst->pool, &mBuf) != ROK) {
#if (ERRCLASS & ERRCLS_ADD_RES)
            SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
               __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
               (ErrVal)ELIB046, (ErrVal)0, "Packing failed");
#endif
            RETVALUE(RFAILED);
         }
      }
      else
      {
         msgLen = 0;
         if (SGetMsg(pst->region, pst->pool, &mBuf) != ROK) {
#if (ERRCLASS & ERRCLS_ADD_RES)
            SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
                  __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
                  (ErrVal)ELIB036, (ErrVal)0, "Packing failed");
#endif
            RETVALUE(RFAILED);
         }
      }
      CMCHKPK(cmPkMsgLen, msgLen, mBuf);

   }
   else
   {
   if (SGetMsg(pst->region, pst->pool, &mBuf) != ROK) {
#if (ERRCLASS & ERRCLS_ADD_RES)
      SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
         __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
         (ErrVal)ELIB044, (ErrVal)0, "Packing failed");
#endif
      RETVALUE(RFAILED);
   }
   }

   if (cmPkPjLibTrans(&libTrans, mBuf) != ROK) {
#if (ERRCLASS & ERRCLS_ADD_RES)
      SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
         __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
         (ErrVal)ELIB048, (ErrVal)0, "Packing failed");
#endif
      PJ_FREE_BUF(mBuf);
      RETVALUE(RFAILED);
   }
   if (cmPkPtr(cxtId, mBuf) != ROK) {
#if (ERRCLASS & ERRCLS_ADD_RES)
      SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
         __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
         (ErrVal)ELIB049, (ErrVal)0, "Packing failed");
#endif
      PJ_FREE_BUF(mBuf);
      RETVALUE(RFAILED);
   }
   pst->event = (Event) OBD_EVT_DECIPHER_CFM;
   RETVALUE(SPstTsk(pst,mBuf));
}

#ifdef ANSI
PUBLIC S16 cmUnpkLibObdDecipherCfm
(
ObdDecipherCfm func,
Pst *pst,
Buffer *mBuf
)
#else
PUBLIC S16 cmUnpkLibObdDecipherCfm(func, pst, mBuf)
ObdDecipherCfm func;
Pst *pst;
Buffer *mBuf;
#endif
{
   PTR cxtId;
   PjLibTrans libTrans;
   Buffer *opSdu= NULLP;
   
   TRC3(cmUnpkLibObdDecipherCfm)

   if (cmUnpkPtr(&cxtId, mBuf) != ROK) {
      PJ_FREE_BUF(mBuf);
#if (ERRCLASS & ERRCLS_ADD_RES)
      SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
         __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
         (ErrVal)ELIB050, (ErrVal)0, "Packing failed");
#endif
      RETVALUE(RFAILED);
   }
   if (cmUnpkPjLibTrans(&libTrans, mBuf) != ROK) {
      PJ_FREE_BUF(mBuf);
#if (ERRCLASS & ERRCLS_ADD_RES)
      SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
         __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
         (ErrVal)ELIB051, (ErrVal)0, "Packing failed");
#endif
      RETVALUE(RFAILED);
   }
   if (pst->selector == OBD_SEL_LC) {
      MsgLen msgLen, totalMsgLen;
      CMCHKUNPK(cmUnpkMsgLen, &msgLen, mBuf);
      if (SFndLenMsg(mBuf, &totalMsgLen) != ROK)
         RETVALUE(RFAILED);
      if ( msgLen != 0 )
      {
         if (SSegMsg(mBuf, totalMsgLen-msgLen, &opSdu) != ROK)
            RETVALUE(RFAILED);
      }
   }

   PJ_FREE_BUF(mBuf);
   RETVALUE((*func)(pst, cxtId, libTrans, opSdu));
}




/***********************************************************
*
*     Func : cmPkPjSecCxt
*
*
*     Desc : PjSecCxt
Context for Security Algorithm
*
*
*     Ret  : S16
*
*     Notes:
*
*     File  : 
*
**********************************************************/
#ifdef ANSI
PUBLIC S16 cmPkPjSecCxt
(
PjSecCxt *param,
Buffer *mBuf
)
#else
PUBLIC S16 cmPkPjSecCxt(param, mBuf)
PjSecCxt *param;
Buffer *mBuf;
#endif
{

   TRC3(cmPkPjSecCxt)

   CMCHKPK(cmPkLteRnti, param->ueId, mBuf);
   CMCHKPK(cmPkLteCellId, param->cellId, mBuf);
   CMCHKPK(SPkU16, param->cfgTxId, mBuf);
   RETVALUE(ROK);
}



/***********************************************************
*
*     Func : cmUnpkPjSecCxt
*
*
*     Desc : PjSecCxt
Context for Security Algorithm
*
*
*     Ret  : S16
*
*     Notes:
*
*     File  : 
*
**********************************************************/
#ifdef ANSI
PUBLIC S16 cmUnpkPjSecCxt
(
PjSecCxt *param,
Buffer *mBuf
)
#else
PUBLIC S16 cmUnpkPjSecCxt(param, mBuf)
PjSecCxt *param;
Buffer *mBuf;
#endif
{

   TRC3(cmUnpkPjSecCxt)

   CMCHKUNPK(SUnpkU16, &param->cfgTxId, mBuf);
   CMCHKUNPK(cmUnpkLteCellId, &param->cellId, mBuf);
   CMCHKUNPK(cmUnpkLteRnti, &param->ueId, mBuf);
   RETVALUE(ROK);
}


/***********************************************************
*
*     Func : cmPkPjIntInfo
*
*
*     Desc : PjIntInfo
  PDCP Integrity Algo Information
*
*
*     Ret  : S16
*
*     Notes:
*
*     File  : 
*
**********************************************************/
#ifdef ANSI
PUBLIC S16 cmPkPjIntInfo
(
PjIntInfo *param,
Buffer *mBuf
)
#else
PUBLIC S16 cmPkPjIntInfo(param, mBuf)
PjIntInfo *param;
Buffer *mBuf;
#endif
{

   S32 i;
   TRC3(cmPkPjIntInfo)

   for (i=PJ_INTG_KEY_LEN-1; i >= 0; i--) {
      CMCHKPK(SPkU8, param->intKey[i], mBuf);
   }
   CMCHKPK(SPkU8, param->algoType, mBuf);
   RETVALUE(ROK);
}



/***********************************************************
*
*     Func : cmUnpkPjIntInfo
*
*
*     Desc : PjIntInfo
  PDCP Integrity Algo Information
*
*
*     Ret  : S16
*
*     Notes:
*
*     File  : 
*
**********************************************************/
#ifdef ANSI
PUBLIC S16 cmUnpkPjIntInfo
(
PjIntInfo *param,
Buffer *mBuf
)
#else
PUBLIC S16 cmUnpkPjIntInfo(param, mBuf)
PjIntInfo *param;
Buffer *mBuf;
#endif
{

   S32 i;
   TRC3(cmUnpkPjIntInfo)

   CMCHKUNPK(SUnpkU8, &param->algoType, mBuf);
   for (i=0; i<PJ_INTG_KEY_LEN; i++) {
      CMCHKUNPK(SUnpkU8, &param->intKey[i], mBuf);
   }
   RETVALUE(ROK);
}



/***********************************************************
*
*     Func : cmPkPjSecInp
*
*
*     Desc : PjSecInp
Input parameters for security algorithm
*
*
*     Ret  : S16
*
*     Notes:
*
*     File  : 
*
**********************************************************/
#ifdef ANSI
PUBLIC S16 cmPkPjSecInp
(
PjSecInp *param,
Buffer *mBuf
)
#else
PUBLIC S16 cmPkPjSecInp(param, mBuf)
PjSecInp *param;
Buffer *mBuf;
#endif
{

   TRC3(cmPkPjSecInp)

   CMCHKPK(SPkU8, param->dir, mBuf);
   CMCHKPK(cmPkLteRbId, param->rbId, mBuf);
   CMCHKPK(SPkU32, param->count, mBuf);
   RETVALUE(ROK);
}



/***********************************************************
*
*     Func : cmUnpkPjSecInp
*
*
*     Desc : PjSecInp
Input parameters for security algorithm
*
*
*     Ret  : S16
*
*     Notes:
*
*     File  : 
*
**********************************************************/
#ifdef ANSI
PUBLIC S16 cmUnpkPjSecInp
(
PjSecInp *param,
Buffer *mBuf
)
#else
PUBLIC S16 cmUnpkPjSecInp(param, mBuf)
PjSecInp *param;
Buffer *mBuf;
#endif
{

   TRC3(cmUnpkPjSecInp)

   CMCHKUNPK(SUnpkU32, &param->count, mBuf);
   CMCHKUNPK(cmUnpkLteRbId, &param->rbId, mBuf);
   CMCHKUNPK(SUnpkU8, &param->dir, mBuf);
   RETVALUE(ROK);
}

#endif /* PJ_SEC_ASYNC */


#if (defined(PJ_SEC_ASYNC) || defined(PJ_CMP_ASYNC))


#endif /*  PJ_SEC_ASYNC || PJ_CMP_ASYNC */

/***********************************************************
*
*     Func : cmPkPjLibTrans
*
*
*     Desc : PjLibTrans
  Transaction Id for Security Algorithm
*
*
*     Ret  : S16
*
*     Notes:
*
*     File  : 
*
**********************************************************/
#ifdef ANSI
PUBLIC S16 cmPkPjLibTrans
(
PjLibTrans *param,
Buffer *mBuf
)
#else
PUBLIC S16 cmPkPjLibTrans(param, mBuf)
PjLibTrans *param;
Buffer *mBuf;
#endif
{

   TRC3(cmPkPjLibTrans)

   CMCHKPK(cmPkPtr, (PTR)param->ueCb, mBuf);
   CMCHKPK(cmPkLteRbId, param->rbId, mBuf);
   CMCHKPK(SPkU8, param->rbType, mBuf);
   CMCHKPK(SPkU32, param->count, mBuf);
   RETVALUE(ROK);
}



/***********************************************************
*
*     Func : cmUnpkPjLibTrans
*
*
*     Desc : PjLibTrans
  Transaction Id for Security Algorithm
*
*
*     Ret  : S16
*
*     Notes:
*
*     File  : 
*
**********************************************************/
#ifdef ANSI
PUBLIC S16 cmUnpkPjLibTrans
(
PjLibTrans *param,
Buffer *mBuf
)
#else
PUBLIC S16 cmUnpkPjLibTrans(param, mBuf)
PjLibTrans *param;
Buffer *mBuf;
#endif
{

   TRC3(cmUnpkPjLibTrans)

   CMCHKUNPK(SUnpkU32, &param->count, mBuf);
   CMCHKUNPK(SUnpkU8, &param->rbType, mBuf);
   CMCHKUNPK(cmUnpkLteRbId, &param->rbId, mBuf);
   CMCHKUNPK(cmUnpkPtr, (PTR*)&param->ueCb, mBuf);
   RETVALUE(ROK);
}

#ifdef ANSI
PUBLIC S16 cmPkLibObdIntProtCfm
(
Pst * pst,
PjLibTrans *libTrans,
U32 macI
)
#else
PUBLIC S16 cmPkLibObdIntProtCfm(pst, cxtId, libTrans, macI)
Pst * pst;
PjLibTrans libTrans;
U32 macI;
#endif
{
   Buffer *mBuf = NULLP;
   TRC3(cmPkLibObdIntProtCfm)

   if (SGetMsg(pst->region, pst->pool, &mBuf) != ROK) {
#if (ERRCLASS & ERRCLS_ADD_RES)
      SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
         __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
         (ErrVal)ELIB029, (ErrVal)0, "Packing failed");
#endif
      RETVALUE(RFAILED);
   }
   if (SPkU32(macI, mBuf) != ROK) {
#if (ERRCLASS & ERRCLS_ADD_RES)
      SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
         __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
         (ErrVal)ELIB030, (ErrVal)0, "Packing failed");
#endif
      PJ_FREE_BUF(mBuf);
      RETVALUE(RFAILED);
   }
   if (cmPkPtr((PTR)libTrans, mBuf) != ROK) {
   /*if (cmPkPjLibTrans(&libTrans, mBuf) != ROK) {*/
#if (ERRCLASS & ERRCLS_ADD_RES)
      SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
         __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
         (ErrVal)ELIB031, (ErrVal)0, "Packing failed");
#endif
      PJ_FREE_BUF(mBuf);
      RETVALUE(RFAILED);
   }
   pst->event = (Event) OBD_EVT_INT_PROT_CFM;
   RETVALUE(SPstTsk(pst,mBuf));
}

#ifdef ANSI
PUBLIC S16 cmUnpkLibObdIntProtCfm
(
ObdIntProtCfm func,
Pst *pst,
Buffer *mBuf
)
#else
PUBLIC S16 cmUnpkLibObdIntProtCfm(func, pst, mBuf)
ObdIntProtCfm func;
Pst *pst;
Buffer *mBuf;
#endif
{
   PjLibTrans *libTrans;
   U32 macI;
   
   TRC3(cmUnpkLibObdIntProtCfm)

   if (cmUnpkPtr((U32 *)&libTrans, mBuf) != ROK) {
   /*if (cmUnpkPjLibTrans(&libTrans, mBuf) != ROK) {*/
      PJ_FREE_BUF(mBuf);
#if (ERRCLASS & ERRCLS_ADD_RES)
      SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
         __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
         (ErrVal)ELIB034, (ErrVal)0, "Packing failed");
#endif
      RETVALUE(RFAILED);
   }
   if (SUnpkU32(&macI, mBuf) != ROK) {
      PJ_FREE_BUF(mBuf);
#if (ERRCLASS & ERRCLS_ADD_RES)
      SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
         __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
         (ErrVal)ELIB035, (ErrVal)0, "Packing failed");
#endif
      RETVALUE(RFAILED);
   }
   PJ_FREE_BUF(mBuf);
   RETVALUE((*func)(pst, libTrans, macI));
}

#ifdef ANSI
PUBLIC S16 cmPkLibObdIntVerCfm
(
Pst * pst,
PjLibTrans *libTrans,
Status status
)
#else
PUBLIC S16 cmPkLibObdIntVerCfm(pst, libTrans, status)
Pst * pst;
PjLibTrans *libTrans;
Status status;
#endif
{
   Buffer *mBuf = NULLP;
   TRC3(cmPkLibObdIntVerCfm)

   if (SGetMsg(pst->region, pst->pool, &mBuf) != ROK) {
#if (ERRCLASS & ERRCLS_ADD_RES)
      SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
         __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
         (ErrVal)ELIB052, (ErrVal)0, "Packing failed");
#endif
      RETVALUE(RFAILED);
   }
   if (SPkS16(status, mBuf) != ROK) {
#if (ERRCLASS & ERRCLS_ADD_RES)
      SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
         __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
         (ErrVal)ELIB053, (ErrVal)0, "Packing failed");
#endif
      PJ_FREE_BUF(mBuf);
      RETVALUE(RFAILED);
   }
   if (cmPkPtr((PTR)libTrans, mBuf) != ROK) {
   /*if (cmPkPjLibTrans(&libTrans, mBuf) != ROK) {*/
#if (ERRCLASS & ERRCLS_ADD_RES)
      SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
         __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
         (ErrVal)ELIB054, (ErrVal)0, "Packing failed");
#endif
      PJ_FREE_BUF(mBuf);
      RETVALUE(RFAILED);
   }
   pst->event = (Event) OBD_EVT_INT_VER_CFM;
   RETVALUE(SPstTsk(pst,mBuf));
}

#ifdef ANSI
PUBLIC S16 cmUnpkLibObdIntVerCfm
(
ObdIntVerCfm func,
Pst *pst,
Buffer *mBuf
)
#else
PUBLIC S16 cmUnpkLibObdIntVerCfm(func, pst, mBuf)
ObdIntVerCfm func;
Pst *pst;
Buffer *mBuf;
#endif
{
   PjLibTrans *libTrans;
   Status status;
   
   TRC3(cmUnpkLibObdIntVerCfm)

   if (cmUnpkPtr((U32 *)&libTrans, mBuf) != ROK) {
   /*if (cmUnpkPjLibTrans(&libTrans, mBuf) != ROK) {*/
      PJ_FREE_BUF(mBuf);
#if (ERRCLASS & ERRCLS_ADD_RES)
      SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
         __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
         (ErrVal)ELIB057, (ErrVal)0, "Packing failed");
#endif
      RETVALUE(RFAILED);
   }
   if (SUnpkS16(&status, mBuf) != ROK) {
      PJ_FREE_BUF(mBuf);
#if (ERRCLASS & ERRCLS_ADD_RES)
      SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
         __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
         (ErrVal)ELIB058, (ErrVal)0, "Packing failed");
#endif
      RETVALUE(RFAILED);
   }
   PJ_FREE_BUF(mBuf);
   RETVALUE((*func)(pst, libTrans, status));
}

#ifdef ANSI
PUBLIC S16 cmPkLibObdCipherCfm
(
Pst * pst,
PjLibTrans *libTrans,
Buffer * opSdu
)
#else
PUBLIC S16 cmPkLibObdCipherCfm(pst, libTrans, opSdu)
Pst * pst;
PjLibTrans *libTrans;
Buffer * opSdu;
#endif
{
   TRC3(cmPkLibObdCipherCfm)

   if (cmPkPtr((PTR)libTrans, opSdu) != ROK) {
   /*if (cmPkPjLibTrans(&libTrans, mBuf) != ROK) {*/
#if (ERRCLASS & ERRCLS_ADD_RES)
      SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
         __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
         (ErrVal)ELIB040, (ErrVal)0, "Packing failed");
#endif
      PJ_FREE_BUF(opSdu);
      RETVALUE(RFAILED);
   }
#ifdef QCOM_TOBE_REMOVED 
     if(SValidateDfltRegnPtr(libTrans) != 0)
     {
        CRASH();
     }
#endif
   pst->event = (Event) OBD_EVT_CIPHER_CFM;
   RETVALUE(SPstTsk(pst,opSdu));
}

#ifdef ANSI
PUBLIC S16 cmUnpkLibObdCipherCfm
(
ObdCipherCfm func,
Pst *pst,
Buffer *mBuf
)
#else
PUBLIC S16 cmUnpkLibObdCipherCfm(func, pst, mBuf)
ObdCipherCfm func;
Pst *pst;
Buffer *mBuf;
#endif
{
   PjLibTrans *libTrans;
   
   TRC3(cmUnpkLibObdCipherCfm)


   if (cmUnpkPtr((U32 *)&libTrans, mBuf) != ROK) {
   /*if (cmUnpkPjLibTrans(&libTrans, mBuf) != ROK) {*/
      PJ_FREE_BUF(mBuf);
#if (ERRCLASS & ERRCLS_ADD_RES)
      SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
         __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
         (ErrVal)ELIB043, (ErrVal)0, "Packing failed");
#endif
      RETVALUE(RFAILED);
   }
#ifdef QCOM_TOBE_REMOVED 
     if(SValidateDfltRegnPtr(libTrans) != 0)
     {
        CRASH();
     }
#endif



   RETVALUE((*func)(pst, libTrans, mBuf));
}

#ifdef ANSI
PUBLIC S16 cmPkLibObdDecipherCfm
(
Pst * pst,
PjLibTrans *libTrans,
Buffer * opSdu
)
#else
PUBLIC S16 cmPkLibObdDecipherCfm(pst, libTrans, opSdu)
Pst * pst;
PjLibTrans *libTrans;
Buffer * opSdu;
#endif
{
   TRC3(cmPkLibObdDecipherCfm)

   if (cmPkPtr((PTR)libTrans, opSdu) != ROK) {
   /*if (cmPkPjLibTrans(&libTrans, mBuf) != ROK) {*/
#if (ERRCLASS & ERRCLS_ADD_RES)
      SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
         __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
         (ErrVal)ELIB048, (ErrVal)0, "Packing failed");
#endif
      PJ_FREE_BUF(opSdu);
      RETVALUE(RFAILED);
   }
#ifdef QCOM_TOBE_REMOVED 
     if(SValidateDfltRegnPtr(libTrans) != 0)
     {
        CRASH();
     }
#endif
   pst->event = (Event) OBD_EVT_DECIPHER_CFM;
   RETVALUE(SPstTsk(pst,opSdu));
}

#ifdef ANSI
PUBLIC S16 cmUnpkLibObdDecipherCfm
(
ObdDecipherCfm func,
Pst *pst,
Buffer *mBuf
)
#else
PUBLIC S16 cmUnpkLibObdDecipherCfm(func, pst, mBuf)
ObdDecipherCfm func;
Pst *pst;
Buffer *mBuf;
#endif
{
   PjLibTrans *libTrans;
   
   TRC3(cmUnpkLibObdDecipherCfm)

   if (cmUnpkPtr((U32 *)&libTrans, mBuf) != ROK) {
   /*if (cmUnpkPjLibTrans(&libTrans, mBuf) != ROK) {*/
      PJ_FREE_BUF(mBuf);
#if (ERRCLASS & ERRCLS_ADD_RES)
      SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
         __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
         (ErrVal)ELIB051, (ErrVal)0, "Packing failed");
#endif
      RETVALUE(RFAILED);
   }
#ifdef QCOM_TOBE_REMOVED 
     if(SValidateDfltRegnPtr(libTrans) != 0)
     {
        CRASH();
     }
#endif
   RETVALUE((*func)(pst, libTrans, mBuf));
}

#ifdef __cplusplus
}
#endif /* __cplusplus */
/********************************************************************30**
         End of file:     eq_lib.c@@/main/TeNB_Main_BR/tenb_main_ccb/1 - Wed Jul 22 18:21:35 2015
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
/main/1      ---      nm   1. LTERLC Release 2.1
*********************************************************************91*/
